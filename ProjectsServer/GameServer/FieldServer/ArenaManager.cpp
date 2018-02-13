// ArenaManager.cpp: implementation of the CArenaManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ArenaManager.h"
#include "ArenaTeam.h"
#include "ArenaDeathMatchRoom.h"
#include "FieldIOCP.h"
#include "FieldGlobal.h"
#include "ArenaTickManager.h"
#include "AtumLogSender.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CArenaManager::CArenaManager()
{
	m_mtmapArenaTeamManager.clearLock();
	m_mtmapArenaDeathMatchRoom.clearLock();
	m_vectorDBArenaInfo.clear();
	m_vectorDBArenaMapInfo.clear();
	m_ReserveTeamNum	= 0;
	m_mtvectArenaMapManager.clearLock();
	m_MaxChannelPerMap	= 0;
	m_TotalArenaMapCount= 0;

	m_pTickManager = new CArenaTickManager(this);

	m_pArenaTeamManager		= NULL;
	m_pArenaDeathMatchRoom	= NULL;
}

CArenaManager::~CArenaManager()
{
	util::del_array(m_pArenaTeamManager);
	util::del_array(m_pArenaDeathMatchRoom);
	util::del(m_pTickManager);
}

void CArenaManager::InitArenaManager(CFieldIOCP *i_pFIOCP)
{
	m_pFieldIOCP16	=	i_pFIOCP;
		
	m_pTickManager->InitThread();

	//////////////////////////////////////////////////////////////////////////
	// 2007-04-17 by dhjin, TeamManager 초기화
	util::del_array(m_pArenaTeamManager);
	m_pArenaTeamManager	= new CArenaTeam[SIZE_MAX_TEAM];
	int i;
	for(i = 0; i < SIZE_MAX_TEAM; i++)
	{
		m_pArenaTeamManager[i].InitArenaTeamManager();
	}

	//////////////////////////////////////////////////////////////////////////
	// 2007-04-19 by dhjin, DeathMatchRoomManager 초기화
	util::del_array(m_pArenaDeathMatchRoom);
	m_pArenaDeathMatchRoom = new CArenaDeathMatchRoom[SIZE_MAX_ARENA_ROOM];
	for(i = 0; i < SIZE_MAX_ARENA_ROOM; i++)
	{
		m_pArenaDeathMatchRoom[i].InitArenaDeathMatchRoom(m_pFieldIOCP16);
	}

}

void CArenaManager::InitDBArenaInfo(vectorSDBARENA_INFO* i_pvectorDBArenaInfo)
{
	m_vectorDBArenaInfo.clear();

	m_vectorDBArenaInfo.assign(i_pvectorDBArenaInfo->begin(), i_pvectorDBArenaInfo->end());
}


void CArenaManager::InitDBArenaMapInfo(vectorSDBARENA_MAPINFO* i_pvectorDBArenaMapInfo)
{
	m_vectorDBArenaMapInfo.clear();

	m_vectorDBArenaMapInfo.assign(i_pvectorDBArenaMapInfo->begin(), i_pvectorDBArenaMapInfo->end());

	//////////////////////////////////////////////////////////////////////////
	// 2007-04-19 by dhjin, ARENA 맵 정보만 얻어와서 맵 관리 맵에 할당한다.
	mt_auto_lock mta(&m_mtvectArenaMapManager);
	SARENA_MAP_MANAGER			tmMap;
	util::zero(&tmMap, sizeof(SARENA_MAP_MANAGER));
	m_MaxChannelPerMap = SIZE_MAX_ARENA_ROOM/m_TotalArenaMapCount;
	vectorSDBARENA_MAPINFO::iterator itrMapInfo = m_vectorDBArenaMapInfo.begin();
	MapIndex_t	tmMapIndex = 0;
	for(; itrMapInfo != m_vectorDBArenaMapInfo.end(); itrMapInfo++)
	{
		if(tmMapIndex != itrMapInfo->ArenaMapIndex)
		{// 2007-04-30 by dhjin, 맵이 다르면 m_mtvectArenaMapManager에 셋팅한다.
			// 2007-05-10 by dhjin, 맵 채널이 몇 개인지 체크를 하여 최대 채널수 보다 넘지 못하게 설정을 한다.
			CFieldMapProject *pFMPro = m_pFieldIOCP16->GetFieldMapProjectByMapIndex(itrMapInfo->ArenaMapIndex);
			if(NULL == pFMPro)
			{
				continue;
			}

			m_MaxChannelPerMap = min(m_MaxChannelPerMap, pFMPro->GetNumberOfMapChannels());
			for(int ChannelCount = 0; ChannelCount < m_MaxChannelPerMap; ChannelCount++)
			{
				tmMap.ArenaMap			= itrMapInfo->ArenaMapIndex;
				tmMap.ArenaMapChannel	= ChannelCount;
				tmMap.bArenaChannelSet	= FALSE;
				m_mtvectArenaMapManager.push_back(tmMap);
				util::zero(&tmMap, sizeof(SARENA_MAP_MANAGER));
			}
			tmMapIndex	= itrMapInfo->ArenaMapIndex;
			itrMapInfo++;
		}
	}

}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CArenaManager::SetTotalMapCount(int i_nTotalMapCount)
/// \brief		
/// \author		dhjin
/// \date		2007-04-30 ~ 2007-04-30
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CArenaManager::SetTotalMapCount(int i_nTotalMapCount)
{
	m_TotalArenaMapCount = i_nTotalMapCount;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CArenaManager::SetTeamState(int i_nTeamNum, BYTE i_nTeamState)
/// \brief		아레나 팀의 상태를 변경한다
/// \author		dhjin
/// \date		2007-05-04 ~ 2007-05-04
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CArenaManager::SetTeamState(int i_nTeamNum, BYTE i_nTeamState)
{
	//////////////////////////////////////////////////////////////////////////
	// 2007-04-18 by dhjin, 팀 번호에 맞는 팀 클래스를 찾는다. 팀 번호가 없다면 오류
	mt_auto_lock mta(&m_mtmapArenaTeamManager);
	CArenaTeam * pATeam = this->FindTeam(i_nTeamNum);
	if(NULL == pATeam)
	{
		return;
	}

	pATeam->SetTeamState(i_nTeamState);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CArenaManager::SendRequestTeamInfo(CFieldIOCPSocket * i_pFis, BYTE i_nTeamMode, int i_nStartNum, BYTE i_byRequetTeamState, MapIndex_t i_byRequestArenaMap)
/// \brief		방 목록을 전송한다
/// \author		dhjin
/// \date		2007-04-25 ~ 2007-04-25
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CArenaManager::SendRequestTeamInfo(CFieldIOCPSocket * i_pFis, BYTE i_nTeamMode, int i_nStartNum, BYTE i_byRequetTeamState, MapIndex_t i_byRequestArenaMap)	// 2012-04-12 by jhseol, 아레나 추가개발 - 선택한 아레나 맵 파라미터 추가
{
	INIT_MSG_WITH_BUFFER(MSG_FC_ARENA_REQUEST_TEAM_OK, T_FC_ARENA_REQUEST_TEAM_OK, pSArenaRequestTeamOk, SendBuf);
	pSArenaRequestTeamOk->ArenaTeamInfoListCount	= this->MakeMSG_FC_ARENA_REQUEST_TEAMLIST_OK((SARENA_TEAM_INFO*)(SendBuf+MSG_SIZE(MSG_FC_ARENA_REQUEST_TEAM_OK)), 
		i_pFis, i_nTeamMode, i_nStartNum, &pSArenaRequestTeamOk->ArenaTeamInfoTotalCount, i_byRequetTeamState, i_byRequestArenaMap);			// 2012-04-12 by jhseol, 아레나 추가개발 - 선택한 아레나 맵 파라미터 추가
	pSArenaRequestTeamOk->RequetTeamState			= i_byRequetTeamState;
	if(0 >= pSArenaRequestTeamOk->ArenaTeamInfoListCount)
	{
		i_pFis->SendErrorMessage(T_FC_ARENA_REQUEST_TEAM, ERR_ARENA_NO_SUCH_TEAM);
		return;
	}

	//////////////////////////////////////////////////////////////////////////
	// 2007-04-23 by dhjin, 방 목록 전송
	i_pFis->SendAddData(SendBuf, MSG_SIZE(MSG_FC_ARENA_REQUEST_TEAM_OK)+pSArenaRequestTeamOk->ArenaTeamInfoListCount*sizeof(SARENA_TEAM_INFO));
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			INT	CArenaManager::MakeMSG_FC_ARENA_REQUEST_TEAMLIST_OK(SARENA_TEAM_INFO *o_pArenaTeamInfo, CFieldIOCPSocket * i_pFis, BYTE i_nTeamMode, int i_nStartNum, int * o_nTotalCount, BYTE i_byRequetTeamState, MapIndex_t i_byRequestArenaMap)
/// \brief		방 목록 리스트를 작성한다
/// \author		dhjin
/// \date		2007-04-25 ~ 2007-04-25
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
INT	CArenaManager::MakeMSG_FC_ARENA_REQUEST_TEAMLIST_OK(SARENA_TEAM_INFO *o_pArenaTeamInfo, CFieldIOCPSocket * i_pFis, BYTE i_nTeamMode, int i_nStartNum, int * o_nTotalCount, BYTE i_byRequetTeamState, MapIndex_t i_byRequestArenaMap)	// 2012-04-12 by jhseol, 아레나 추가개발 - 선택한 아레나 맵 파라미터 추가
{
	mt_auto_lock mta(&m_mtmapArenaTeamManager);

	INT	nTeamListCount	= 0;
	int	Count = 0;

	//////////////////////////////////////////////////////////////////////////
	// 2007-04-23 by dhjin, 팀이 존재하는지 체크
	if(m_mtmapArenaTeamManager.empty())
	{
		return nTeamListCount;
	}

	//////////////////////////////////////////////////////////////////////////
	// 2007-04-30 by dhjin, 유저 조건에 맞는 총 팀을 전송하기 위해 검색 한다
	mtmapINT2ArenaTeamManager::iterator itr_TotalCount = m_mtmapArenaTeamManager.begin();
	for(; itr_TotalCount != m_mtmapArenaTeamManager.end(); itr_TotalCount++)
	{
		//////////////////////////////////////////////////////////////////////////////
		// 2012-04-12 by jhseol, 아레나 추가개발 - 선택한 아레나 맵 과 같은 맵의 팀인지 체크
		if(ARENA_MAP_SELECT_ALL != itr_TotalCount->second->GetTeamSelectArenaMapNum())
		{
			if(i_byRequestArenaMap != itr_TotalCount->second->GetTeamSelectArenaMapNum())
			{
				// 2012-04-12 by jhseol, 아레나 추가개발 - 추후에 맵별로 검색할 기능을 추가할때 사용할 부분
			}
		}
		// end 2012-04-12 by jhseol, 아레나 추가개발 - 선택한 아레나 맵 과 같은 맵의 팀인지 체크


		if(ARENA_REQUEST_TEAM_STATE_OUR == i_byRequetTeamState)
		{// 2007-06-08 by dhjin, 우리 팀만 
			if(i_pFis->m_character.InfluenceType != itr_TotalCount->second->GetTeamInfl()
				//|| i_pFis->m_AfsNeedMfsInfo.MFSServerID != itr_TotalCount->second->GetTeamServerID()	// 2008-01-03 by dhjin, 아레나 통합 - 
				)
			{
				continue;
			}
		}
		else if(ARENA_REQUEST_TEAM_STATE_JOINING == i_byRequetTeamState)
		{// 2007-06-08 by dhjin, 우리 팀 같은 레벨 등급 (진행준인 방 제외)
			if(i_pFis->m_character.InfluenceType != itr_TotalCount->second->GetTeamInfl()
				//|| i_pFis->m_AfsNeedMfsInfo.MFSServerID != itr_TotalCount->second->GetTeamServerID()	// 2008-01-03 by dhjin, 아레나 통합 - 
				|| FALSE == itr_TotalCount->second->CheckTeamLevel(i_pFis->m_character.Level)
				|| ARENA_STATE_JOINING != itr_TotalCount->second->GetTeamState() )
			{
				continue;
			}
		}		
		else if(ARENA_REQUEST_TEAM_STATE_OTHER == i_byRequetTeamState)
		{// 2007-06-08 by dhjin, 상대 팀만 (진행중인 방 제외)
		// if(i_pFis->m_character.InfluenceType == itr_TotalCount->second->GetTeamInfl()
			if((i_pFis->m_character.InfluenceType == itr_TotalCount->second->GetTeamInfl()
				)//&& i_pFis->m_AfsNeedMfsInfo.MFSServerID == itr_TotalCount->second->GetTeamServerID() )	// 2008-01-03 by dhjin, 아레나 통합 - 
				|| ARENA_STATE_JOINING > itr_TotalCount->second->GetTeamState()
				|| ARENA_STATE_READY_FINISH < itr_TotalCount->second->GetTeamState() )
			{
				continue;
			}
		}
		else
		{
			continue;
		}

		(*o_nTotalCount)++;
	}

	//////////////////////////////////////////////////////////////////////////
	// 2007-04-30 by dhjin, 클라이언트에서 요청한 팀 리스트 처음 번호부터 ARENA_MAX_REQUEST_TEAM 만큼 메세지를 만든다
	mtmapINT2ArenaTeamManager::iterator itr = m_mtmapArenaTeamManager.begin();
	for(; itr != m_mtmapArenaTeamManager.end(); itr++)
	{
		//////////////////////////////////////////////////////////////////////////////
		// 2012-04-12 by jhseol, 아레나 추가개발 - 선택한 아레나 맵 과 같은 맵의 팀인지 체크
		if(ARENA_MAP_SELECT_ALL != itr->second->GetTeamSelectArenaMapNum())
		{
			if(i_byRequestArenaMap != itr->second->GetTeamSelectArenaMapNum())
			{
				// 2012-04-12 by jhseol, 아레나 추가개발 - 추후에 맵별로 검색할 기능을 추가할때 사용할 부분
			}
		}
		// end 2012-04-12 by jhseol, 아레나 추가개발 - 선택한 아레나 맵 과 같은 맵의 팀인지 체크

//////////////////////////////////////////////////////////////////////////
// 2007-06-05 by dhjin, 대기 팀 정보 얻어오기 밑에 사항으로 수정.
//		if(i_pFis->m_character.InfluenceType != itr->second->GetTeamInfl()
//			|| i_pFis->m_character.Level < itr->second->GetTeamStartLevel() 
//			|| i_pFis->m_character.Level > itr->second->GetTeamEndLevel()
//			|| ARENA_STATE_JOINING != itr->second->GetTeamState())
//		{
//			continue;
//		}
		//////////////////////////////////////////////////////////////////////////
		// 2008-03-07 by dhjin, 아레나 통합 - 대회 서버인지 체크
		//if(JAMBOREE_SERVER_ID == i_pFis->m_AfsNeedMfsInfo.MFSServerID
		//	|| JAMBOREE_SERVER_ID == itr->second->GetTeamServerID()
		//	)
		//{
		//	if(JAMBOREE_SERVER_ID != i_pFis->m_AfsNeedMfsInfo.MFSServerID
		//		|| JAMBOREE_SERVER_ID != itr->second->GetTeamServerID()
		//		)
		//	{
		//		continue;
		//	}
		//}

		if(ARENA_REQUEST_TEAM_STATE_OUR == i_byRequetTeamState)
		{// 2007-06-08 by dhjin, 우리 팀만 
			if(i_pFis->m_character.InfluenceType != itr->second->GetTeamInfl()
				//|| i_pFis->m_AfsNeedMfsInfo.MFSServerID != itr->second->GetTeamServerID()	// 2008-01-03 by dhjin, 아레나 통합 - 
				)
			{
				continue;
			}
		}
		else if(ARENA_REQUEST_TEAM_STATE_JOINING == i_byRequetTeamState)
		{// 2007-06-08 by dhjin, 우리 팀 같은 레벨 등급 (진행준인 방 제외)
			if(i_pFis->m_character.InfluenceType != itr->second->GetTeamInfl()
				//|| i_pFis->m_AfsNeedMfsInfo.MFSServerID != itr->second->GetTeamServerID()	// 2008-01-03 by dhjin, 아레나 통합 - 
				|| FALSE == itr->second->CheckTeamLevel(i_pFis->m_character.Level)
				|| ARENA_STATE_JOINING != itr->second->GetTeamState() )
			{
				continue;
			}
		}		
		else if(ARENA_REQUEST_TEAM_STATE_OTHER == i_byRequetTeamState)
		{// 2007-06-08 by dhjin, 상대 팀만 (진행중인 방 제외)
		//	if(i_pFis->m_character.InfluenceType == itr->second->GetTeamInfl()
			if((i_pFis->m_character.InfluenceType == itr->second->GetTeamInfl()
				)//&& i_pFis->m_AfsNeedMfsInfo.MFSServerID == itr->second->GetTeamServerID() )	// 2008-01-03 by dhjin, 아레나 통합 - 
				|| ARENA_STATE_JOINING > itr->second->GetTeamState()
				|| ARENA_STATE_READY_FINISH < itr->second->GetTeamState() )
			{
				continue;
			}
		}
		else
		{
			continue;
		}

		//////////////////////////////////////////////////////////////////////////
		// 2007-05-11 by dhjin, 클라이언트에서 요청한 팀 번호 다음을 찾아 ARENA_MAX_REQUEST_TEAM 만큼 전송한다.
		if(i_nStartNum >= itr->first
			|| Count >= ARENA_MAX_REQUEST_TEAM)
		{
			continue;
		}

		o_pArenaTeamInfo[nTeamListCount].TeamLock			=	itr->second->GetTeamLock();
		o_pArenaTeamInfo[nTeamListCount].TeamEvent			=	itr->second->GetTeamEvent();
		o_pArenaTeamInfo[nTeamListCount].TeamSize			=	itr->second->GetTeamSize();
		o_pArenaTeamInfo[nTeamListCount].CurrentTeamSize	=	itr->second->GetCurrentTeamSize();
		o_pArenaTeamInfo[nTeamListCount].ArenaMode			=	itr->second->GetTeamMode();
		o_pArenaTeamInfo[nTeamListCount].TeamNum			=	itr->second->GetTeamNum();
		o_pArenaTeamInfo[nTeamListCount].ArenaState			=	itr->second->GetTeamState();
		o_pArenaTeamInfo[nTeamListCount].TeamStartLevel		=	itr->second->GetTeamStartLevel();
		o_pArenaTeamInfo[nTeamListCount].TeamEndLevel		=	itr->second->GetTeamEndLevel();
		o_pArenaTeamInfo[nTeamListCount].ArenaMap			=	itr->second->GetTeamSelectArenaMapNum();	// 2012-04-12 by jhseol, 아레나 추가개발 - 선택한 아레나 맵
		
		CFieldIOCPSocket *tm_pFieldIOCPSocket = this->m_pFieldIOCP16->GetFieldIOCPSocket(itr->second->GetTeamFirstCharacterClientIndex());
		if(NULL == tm_pFieldIOCPSocket)
		{// 2007-05-11 by dhjin, 유저가 존재하지 않는다면 NULL로 준다
			util::zero(&o_pArenaTeamInfo[nTeamListCount].CharacterName, SIZE_MAX_CHARACTER_NAME);
		}
		util::strncpy(o_pArenaTeamInfo[nTeamListCount].CharacterName, tm_pFieldIOCPSocket->m_character.CharacterName, SIZE_MAX_CHARACTER_NAME);
		nTeamListCount++;
 		Count++;
	}
		
	return nTeamListCount;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CArenaManager::SendCreatTeamInfo(CFieldIOCPSocket * i_pFis)
/// \brief		팀 생성을 위해 요청한 DB정보를 전송한다
/// \author		dhjin
/// \date		2007-04-26 ~ 2007-04-26
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CArenaManager::SendCreatTeamInfo(CFieldIOCPSocket * i_pFis)
{
	//////////////////////////////////////////////////////////////////////////
	// 2007-04-23 by dhjin, 메세지 만들기
	INIT_MSG_WITH_BUFFER(MSG_FC_ARENA_REQUEST_CREATE_TEAMINFO_OK, T_FC_ARENA_REQUEST_CREATE_TEAMINFO_OK, pSArenaRequestCreateTeamInfo, SendBuf);
	pSArenaRequestCreateTeamInfo->ArenaRequetCreateTeamInfoListCount = this->MakeMSG_FC_ARENA_REQUEST_CREATE_TEAMINFO((SARENA_REQUEST_CREATE_TEAMINFO*)(SendBuf+MSG_SIZE(MSG_FC_ARENA_REQUEST_CREATE_TEAMINFO_OK)));
	if(0 >= pSArenaRequestCreateTeamInfo->ArenaRequetCreateTeamInfoListCount)
	{
		return;
	}

	//////////////////////////////////////////////////////////////////////////
	// 2007-04-23 by dhjin, 정보 전송
	i_pFis->SendAddData(SendBuf, MSG_SIZE(MSG_FC_ARENA_REQUEST_CREATE_TEAMINFO_OK)+pSArenaRequestCreateTeamInfo->ArenaRequetCreateTeamInfoListCount*sizeof(SARENA_REQUEST_CREATE_TEAMINFO));

}

///////////////////////////////////////////////////////////////////////////////
/// \fn			INT CArenaManager::MakeMSG_FC_ARENA_REQUEST_CREATE_TEAMINFO(SARENA_REQUEST_CREATE_TEAMINFO *o_pArenaRequestTeamInfo)
/// \brief		팀 생성을 위해 요청한 DB정보 자료 만들기
/// \author		dhjin
/// \date		2007-04-26 ~ 2007-04-26
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
INT CArenaManager::MakeMSG_FC_ARENA_REQUEST_CREATE_TEAMINFO(SARENA_REQUEST_CREATE_TEAMINFO *o_pArenaRequestTeamInfo)
{
	int	ArenaRequestTeamInfoCount = 0;

	vectorSDBARENA_MAPINFO::iterator itr = m_vectorDBArenaMapInfo.begin();
	for(; itr != m_vectorDBArenaMapInfo.end(); itr++)
	{
		o_pArenaRequestTeamInfo[ArenaRequestTeamInfoCount].ArenaMode		= itr->ArenaMode;
		o_pArenaRequestTeamInfo[ArenaRequestTeamInfoCount].ReqMemberNum	= itr->ReqMemberNum;
		o_pArenaRequestTeamInfo[ArenaRequestTeamInfoCount].ArenaMap			= itr->ArenaMapIndex;	// 2012-04-12 by jhseol, 아레나 추가개발 - 선택한 아레나 맵
		ArenaRequestTeamInfoCount++;
	}
	
	return ArenaRequestTeamInfoCount;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			INT	CArenaManager::CreateTeam(MSG_FC_ARENA_CREATE_TEAM * i_pSMSG)
/// \brief		팀을 생성한다
/// \author		dhjin
/// \date		2007-04-25 ~ 2007-04-25
/// \warning	
///
/// \param		
/// \return		팀 번호
///////////////////////////////////////////////////////////////////////////////
INT	CArenaManager::CreateTeam(MSG_FC_ARENA_CREATE_TEAM * i_pSMSG, CFieldIOCPSocket * i_pFISock)
{
	//////////////////////////////////////////////////////////////////////////
	// 2007-04-25 by dhjin, 팀이 생성 가능한 DB조건인지 체크한다.
	if(!this->DBCheckCreateTeam(i_pSMSG))
	{
		return STATE_ERROR;
	}
	
	mt_auto_lock mta(&m_mtmapArenaTeamManager);
	//////////////////////////////////////////////////////////////////////////
	// 2007-04-17 by dhjin, 할당할 수 있는 팀 번호를 찾는다.
	int	FindTeamNum	= FindEmptyTeam();
	if (STATE_ERROR == FindTeamNum)
	{
		return STATE_ERROR;
	}
	
	CArenaTeam * tmArenaTeam = GetArenaTeam(FindTeamNum);
	//////////////////////////////////////////////////////////////////////////
	// 2007-04-17 by dhjin, 팀 생성.
	tmArenaTeam->SetTeamInfl(i_pFISock->m_character.InfluenceType);
	tmArenaTeam->SetTeamLevel(i_pFISock->m_character.Level);
	tmArenaTeam->SetTeamLock(i_pSMSG->TeamLock);
	tmArenaTeam->SetTeamMode(i_pSMSG->ArenaMode);
	tmArenaTeam->SetTeamNum(FindTeamNum);
	tmArenaTeam->SetTeamPW(i_pSMSG->TeamPW);
	tmArenaTeam->SetTeamSize(i_pSMSG->TeamSize);
	tmArenaTeam->SetTeamSelectArenaMapNum(i_pSMSG->ArenaMap);	// 2012-04-12 by jhseol, 아레나 추가개발 - 선택한 아레나 맵
	if(1 == i_pSMSG->TeamSize)
	{
		tmArenaTeam->SetTeamState(ARENA_STATE_READY);
	}
	else
	{
		tmArenaTeam->SetTeamState(ARENA_STATE_JOINING);
	}
	//tmArenaTeam->SetTeamServerID(i_pFISock->m_AfsNeedMfsInfo.MFSServerID);	// 2008-01-03 by dhjin, 아레나 통합 -
	tmArenaTeam->InsertTeamList(i_pSMSG->ClientIndex);
	m_mtmapArenaTeamManager.insertNoLock(FindTeamNum, tmArenaTeam);

	// 2007-06-05 by dhjin, 팀 참가 순위 시간 설정.
	tmArenaTeam->SetTeamJoinRankTime();

	return FindTeamNum;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CArenaManager::DBCheckCreateTeam(MSG_FC_ARENA_CREATE_TEAM *i_pSMSG)
/// \brief		팀 생성하기전에 DB조건에 부합하지는 체크한다
/// \author		dhjin
/// \date		2007-04-25 ~ 2007-04-25
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CArenaManager::DBCheckCreateTeam(MSG_FC_ARENA_CREATE_TEAM *i_pSMSG)
{
	vectorSDBARENA_MAPINFO::iterator itr = m_vectorDBArenaMapInfo.begin();
	for(; itr != m_vectorDBArenaMapInfo.end(); itr++)
	{
		if(i_pSMSG->ArenaMode == itr->ArenaMode
			&& i_pSMSG->TeamSize == itr->ReqMemberNum)
		{
			return TRUE;
		}
	}

	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			INT	CArenaManager::FindEmptyTeam()
/// \brief		할당 할 수 있는 팀 번호를 찾는다
/// \author		dhjin
/// \date		2007-04-25 ~ 2007-04-25
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
INT	CArenaManager::FindEmptyTeam()
{
	//////////////////////////////////////////////////////////////////////////
	// 2007-04-17 by dhjin, 처음 예약번호 할당
	if(0 == m_ReserveTeamNum)
	{
		int	ReturnReserveTeamNum = m_ReserveTeamNum;
		m_ReserveTeamNum = STATE_ERROR;
		return ReturnReserveTeamNum;
	}
	
	if(STATE_ERROR != m_ReserveTeamNum)
	{// 2007-04-18 by dhjin, 예약 번호로 할당 할 수있으면 할당하고 예약 번호는 Error값으로 변경하여 없어지는 팀 번호를 받기까지 대기한다.
		int tmReturnValue	= m_ReserveTeamNum;
		m_ReserveTeamNum  = STATE_ERROR;
		return tmReturnValue;
	}

	if(SIZE_MAX_TEAM < m_mtmapArenaTeamManager.size())
	{
		return STATE_ERROR;
	}
		
	for(int ReTurnTeamNum = 0;  ReTurnTeamNum < SIZE_MAX_TEAM; ReTurnTeamNum++)
	{
		mtmapINT2ArenaTeamManager::iterator itr = m_mtmapArenaTeamManager.find(ReTurnTeamNum);
		if(itr == m_mtmapArenaTeamManager.end())
		{
			return ReTurnTeamNum;
		}
	}
	
	return STATE_ERROR;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			CArenaTeam *	CArenaManager::FindTeam(INT i_nTeamNum)
/// \brief		팀 포인터를 리턴한다
/// \author		dhjin
/// \date		2007-04-27 ~ 2007-04-27
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
CArenaTeam * CArenaManager::FindTeam(INT i_nTeamNum)
{
	if(0 == m_mtmapArenaTeamManager.size())
	{
		return NULL;
	}

	mtmapINT2ArenaTeamManager::iterator itr = m_mtmapArenaTeamManager.find(i_nTeamNum);
	if(itr == m_mtmapArenaTeamManager.end())
	{
		return NULL;
	}

	return itr->second;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			Err_t CArenaManager::InsertMember(MSG_FC_ARENA_ENTER_TEAM *i_pSMSG, CFieldIOCPSocket * i_pFISock)
/// \brief		팀원 추가
/// \author		dhjin
/// \date		2007-04-25 ~ 2007-04-25
/// \warning	
///
/// \param		
/// \return		Error
///////////////////////////////////////////////////////////////////////////////
Err_t CArenaManager::InsertMember(MSG_FC_ARENA_ENTER_TEAM *i_pSMSG, CFieldIOCPSocket * i_pFISock)
{
	//////////////////////////////////////////////////////////////////////////
	// 2007-04-18 by dhjin, 케릭터 상태 체크
	if(!i_pFISock->IsValidCharacter(FALSE))
	{
		return ERR_INVALID_CHARACTER;
	}

	//////////////////////////////////////////////////////////////////////////
	// 2007-04-18 by dhjin, 팀 번호에 맞는 팀 클래스를 찾는다. 팀 번호가 없다면 오류
	mt_auto_lock mta(&m_mtmapArenaTeamManager);
	CArenaTeam * pTeam = this->FindTeam(i_pSMSG->TeamNum);
	if(NULL == pTeam)
	{
		return ERR_ARENA_NO_SUCH_TEAM;
	}

	//////////////////////////////////////////////////////////////////////////
	// 2007-05-21 by dhjin, 팀과 세력이 같지 않으면 오류
	if(i_pFISock->m_character.InfluenceType != pTeam->GetTeamInfl()
		//|| i_pFISock->m_AfsNeedMfsInfo.MFSServerID != pTeam->GetTeamServerID()	// 2008-01-03 by dhjin, 아레나 통합 - 
		)
	{
		return ERR_PROTOCOL_REQ_INFLUENCE_NOT_MATCHED;
	}

	//////////////////////////////////////////////////////////////////////////
	// 2007-04-18 by dhjin, 팀의 아레나 모드가 같은지 체크
	if(i_pSMSG->ArenaMode != pTeam->GetTeamMode())
	{
		return ERR_ARENA_NOT_MATCH_MODE;
	}

	//////////////////////////////////////////////////////////////////////////
	// 2007-04-18 by dhjin, 같은 레벨 등급인지 체크
	if(!pTeam->CheckTeamLevel(i_pFISock->m_character.Level))
	{
		return ERR_ARENA_NOT_MATCH_LEVEL;
	}

	//////////////////////////////////////////////////////////////////////////
	// 2007-06-05 by dhjin, 매칭중이거나, 진행중일때는 참가 할 수 없다.
	if(ARENA_STATE_READY_FINISH == pTeam->GetTeamState())
	{	
		return ERR_ARENA_STATE_TEAM_READY;
	}
	if(ARENA_STATE_READY_FINISH < pTeam->GetTeamState())
	{	
		return ERR_ARENA_STATE_TEAM_WARING;
	}
	
	//////////////////////////////////////////////////////////////////////////
	// 2007-04-18 by dhjin, 팀의 비밀 번호가 같은지 체크
	if(pTeam->GetTeamLock())
	{
		if(strcmp(i_pSMSG->TeamPW ,pTeam->GetTeamPW()))
		{
			return ERR_ARENA_NOT_MATCH_PW;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// 2007-04-18 by dhjin, 팀원이 꽉 찬 상태인지 체크
	if(pTeam->CheckFullMember())
	{
		return ERR_ARENA_FULL_TEAMLIST;
	}
	
	//////////////////////////////////////////////////////////////////////////
	// 2007-04-18 by dhjin, 팀원 추가
	pTeam->InsertTeamList(i_pSMSG->ClientIndex);

//	DBGOUT("[ARENA] T : InsertMember : Team = %d, State = %d, TeamSize = %d\r\n" 
//			, pTeam->GetTeamNum(), pTeam->GetTeamState(), pTeam->GetTeamSize());
	return ERR_NO_ERROR;
}

//////////////////////////////////////////////////////////////////////////
// 2007-06-05 by dhjin, 팀 참가 가능 시간 값으로 올림 차순 정렬한다
struct ArenaTeam_Sort_Increase_By_TeamJoinRankTime: binary_function<CArenaTeam*, CArenaTeam*, bool>
{
	bool operator()(CArenaTeam * ATeam, CArenaTeam * BTeam)
	{
		return *(ATeam)->GetTeamJoinRankTime() < *(BTeam)->GetTeamJoinRankTime();	// 올림 차순 정렬
	};
};

///////////////////////////////////////////////////////////////////////////////
/// \fn			Err_t CArenaManager::QuickInsertMember(CFieldIOCPSocket * i_pFISock, BYTE * o_byArenaMode, BYTE * o_byTeamSize, INT * o_nTeamNum)
/// \brief		빠른 참가로 팀원추가
/// \author		dhjin
/// \date		2007-06-05 ~ 2007-06-05
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
Err_t CArenaManager::QuickInsertMember(CFieldIOCPSocket * i_pFISock, BYTE * o_byArenaMode, BYTE * o_byTeamSize, INT * o_nTeamNum)
{
	// 2007-06-05 by dhjin, 케릭터 상태 체크
	if(!i_pFISock->IsValidCharacter(FALSE))
	{
		return ERR_INVALID_CHARACTER;
	}

	mt_auto_lock mtTeam(&m_mtmapArenaTeamManager);

	/////////////////////////////////////////////////////////////////////////
	// 2007-06-05 by dhjin, Team관련 복사 본을 만들어 정렬을 한다.
	vector<CArenaTeam*>			vectCopyArenaTeamManager;
	vectCopyArenaTeamManager.clear();
	mtmapINT2ArenaTeamManager::iterator itrtmTeam = m_mtmapArenaTeamManager.begin();
	for(; itrtmTeam != m_mtmapArenaTeamManager.end(); itrtmTeam++)
	{
		////////////////////////////////////////////////////////////////////////////
		//// 2008-03-07 by dhjin, 아레나 통합 - 대회 서버인지 체크
		//if(JAMBOREE_SERVER_ID == i_pFISock->m_AfsNeedMfsInfo.MFSServerID
		//	|| JAMBOREE_SERVER_ID == itrtmTeam->second->GetTeamServerID()
		//	)
		//{
		//	if(JAMBOREE_SERVER_ID != i_pFISock->m_AfsNeedMfsInfo.MFSServerID
		//		|| JAMBOREE_SERVER_ID != itrtmTeam->second->GetTeamServerID()
		//		)
		//	{
		//		continue;
		//	}
		//}

		if(ARENA_STATE_JOINING == itrtmTeam->second->GetTeamState()						// 참가 가능 상태인지 체크
			&& i_pFISock->m_character.InfluenceType == itrtmTeam->second->GetTeamInfl()	// 같은 세력인지 체크
			//&& i_pFISock->m_AfsNeedMfsInfo.MFSServerID == itrtmTeam->second->GetTeamServerID()	// 2008-01-03 by dhjin, 아레나 통합 - 
			&& itrtmTeam->second->CheckTeamLevel(i_pFISock->m_character.Level)			// 레벨 등급이 같은지 체크
			&& FALSE == itrtmTeam->second->GetTeamLock()									// 비번 방인지 체크
			&& FALSE == itrtmTeam->second->CheckFullMember())							// 최대 인원인지 체크
		{
			vectCopyArenaTeamManager.push_back(itrtmTeam->second);
		}
	}
	sort(vectCopyArenaTeamManager.begin(), vectCopyArenaTeamManager.end(), ArenaTeam_Sort_Increase_By_TeamJoinRankTime());
	
	//////////////////////////////////////////////////////////////////////////
	// 2007-06-05 by dhjin, 제일 먼저 참가 가능한 팀을 가져온다.
	vector<CArenaTeam*>::iterator pTeam = vectCopyArenaTeamManager.begin();
	for(; pTeam != vectCopyArenaTeamManager.end(); pTeam++)
	{
		// 2007-06-07 by dhjin, 팀에 빠른 참가가 가능한지 체크
		if(ARENA_STATE_JOINING == (*pTeam)->GetTeamState()						// 참가 가능 상태인지 체크
			&& i_pFISock->m_character.InfluenceType == (*pTeam)->GetTeamInfl()	// 같은 세력인지 체크
			//&& i_pFISock->m_AfsNeedMfsInfo.MFSServerID == (*pTeam)->GetTeamServerID()	// 2008-01-03 by dhjin, 아레나 통합 - 
			&& (*pTeam)->CheckTeamLevel(i_pFISock->m_character.Level)			// 레벨 등급이 같은지 체크
			&& FALSE == (*pTeam)->GetTeamLock()									// 비번 방인지 체크
			&& FALSE == (*pTeam)->CheckFullMember())							// 최대 인원인지 체크
		{
			*o_byArenaMode	= (*pTeam)->GetTeamMode();
			*o_byTeamSize	= (*pTeam)->GetTeamSize();
			*o_nTeamNum		= (*pTeam)->GetTeamNum();
			//////////////////////////////////////////////////////////////////////////
			// 2007-06-07 by dhjin, 팀원 추가
			(*pTeam)->InsertTeamList(i_pFISock->m_character.ClientIndex);
			
			return ERR_NO_ERROR;
		}
	}		

	return ERR_ARENA_NO_SUCH_TEAM;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CArenaManager::SendTeamMemberList(int i_nTeamNum)
/// \brief		클라이언트들의 팀 정보를 갱신하기 위해 보낸다
/// \author		dhjin
/// \date		2007-04-25 ~ 2007-04-25
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CArenaManager::SendTeamMemberList(int i_nTeamNum)
{
	mt_auto_lock mta(&m_mtmapArenaTeamManager);
	CArenaTeam * pTeam = this->FindTeam(i_nTeamNum);
	if(NULL == pTeam)
	{
		return;
	}

	//////////////////////////////////////////////////////////////////////////
	// 2007-04-23 by dhjin, 팀 리스트 얻어오기
	vectorClientIndex *pvectClient = pTeam->GetTeamList();
	if(NULL == pvectClient)
	{
		return;
	}
	vectorClientIndex	tmvectClient	= *pvectClient;
	BYTE				tmTeamState		= pTeam->GetTeamState();
	mta.auto_unlock_cancel();


	//////////////////////////////////////////////////////////////////////////
	// 2007-04-23 by dhjin, 메세지 만들기
	INIT_MSG_WITH_BUFFER(MSG_FC_ARENA_TEAM_MEMBER_LIST, T_FC_ARENA_TEAM_MEMBER_LIST, pSArenaTeamMemberList, SendBuf);
	pSArenaTeamMemberList->ArenaTeamState					= tmTeamState;		
	pSArenaTeamMemberList->ArenaTeamCharacterNameListCount	= this->MakeMSG_FC_ARENA_TEAM_MEMBERLIST_OK((SARENA_TEAM_CHARACTERINFO*)(SendBuf+MSG_SIZE(MSG_FC_ARENA_TEAM_MEMBER_LIST)), &tmvectClient);
	if(0 >= pSArenaTeamMemberList->ArenaTeamCharacterNameListCount)
	{
		return;
	}

	//////////////////////////////////////////////////////////////////////////
	// 2007-04-23 by dhjin, 팀원 모두에게 정보 전송
	vectorClientIndex::iterator itr = tmvectClient.begin();
	for(; itr != tmvectClient.end(); itr++)
	{
		CFieldIOCPSocket *pFISoc = this->m_pFieldIOCP16->GetFieldIOCPSocket(*itr);
		if(NULL == pFISoc
			|| FALSE == pFISoc->IsValidCharacter(FALSE)
			|| i_nTeamNum != pFISoc->m_ArenaInfo.TeamNum)
		{
//			// 2007-05-18 by cmkwon, 테스트 로그
//			g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ARENA] ErrorTeamMember : SendTeamMemberList - TeamNum(%4d) TeamState(%30s) CharacterName(%30s)\r\n" 
//						, pFISoc->m_ArenaInfo.TeamNum, GET_ARENA_STATE_STRING(pFISoc->m_ArenaInfo.State), pFISoc->m_character.CharacterName);
			continue;
		}
		
		if(ARENA_STATE_FIGHTING_WARREADY > pFISoc->m_ArenaInfo.State)
		{// 2007-05-22 by dhjin, 아레나 대전 중이 아닌 유저에게만 팀 정보를 전송한다.
			pFISoc->SendAddData(SendBuf, MSG_SIZE(MSG_FC_ARENA_TEAM_MEMBER_LIST)+pSArenaTeamMemberList->ArenaTeamCharacterNameListCount*sizeof(SARENA_TEAM_CHARACTERINFO));
		}
	}	
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			vectorClientIndex * CArenaManager::GetMemberList(int i_nTeamNum)
/// \brief		팀 맴버를 찾아 리턴
/// \author		dhjin
/// \date		2007-04-25 ~ 2007-04-25
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
vectorClientIndex * CArenaManager::GetMemberList(int i_nTeamNum)
{
	//////////////////////////////////////////////////////////////////////////
	// 2007-04-18 by dhjin, 팀 번호에 맞는 팀 클래스를 찾는다. 팀 번호가 없다면 오류
	mtmapINT2ArenaTeamManager::iterator itr = m_mtmapArenaTeamManager.find(i_nTeamNum);
	if(itr == m_mtmapArenaTeamManager.end())
	{
		return NULL;
	}

	return itr->second->GetTeamList();
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CArenaManager::SendTeamMemberState(int i_nTeamNum, BYTE i_nState)
/// \brief		팀의 ARENA_STATE를 팀원에게 전송한다
/// \author		dhjin
/// \date		2007-04-25 ~ 2007-04-25
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CArenaManager::SendTeamMemberState(int i_nTeamNum, BYTE i_nState)
{
	//////////////////////////////////////////////////////////////////////////
	// 2007-04-18 by dhjin, 팀 번호에 맞는 팀 클래스를 찾는다. 팀 번호가 없다면 오류
	mt_auto_lock mta(&m_mtmapArenaTeamManager);
	CArenaTeam * pTeam = this->FindTeam(i_nTeamNum);
	if(NULL == pTeam)
	{
		return;
	}

	//////////////////////////////////////////////////////////////////////////
	// 2007-04-23 by dhjin, 팀 리스트 얻어오기
	vectorClientIndex *ptmvectClient = pTeam->GetTeamList();
	if(NULL == ptmvectClient)
	{
		return;
	}

	//////////////////////////////////////////////////////////////////////////
	// 2007-04-23 by dhjin, 메세지 만들기
	if(ARENA_STATE_READY == i_nState)
	{
		INIT_MSG_WITH_BUFFER(MSG_FC_ARENA_TEAM_READY, T_FC_ARENA_TEAM_READY, pArenaTeamReady, SendBuf);
		pArenaTeamReady->ArenaMode = pTeam->GetTeamMode();
		pArenaTeamReady->ArenaState = pTeam->GetTeamState();
		pArenaTeamReady->TeamNum = pTeam->GetTeamNum();
		
		//////////////////////////////////////////////////////////////////////////
		// 2007-04-23 by dhjin, 팀원 모두에게 정보 전송
		vectorClientIndex::iterator itr = ptmvectClient->begin();
		for(; itr != ptmvectClient->end(); itr++)
		{
			CFieldIOCPSocket *pFISoc = this->m_pFieldIOCP16->GetFieldIOCPSocket(*itr);
			if(NULL == pFISoc
				|| FALSE == pFISoc->IsValidCharacter(FALSE))
			{
//				// 2007-05-18 by cmkwon, 테스트 로그
//				g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ARENA] ErrorTeamMember SendTeamMemberState - TeamNum(%4d) TeamState(%30s) CharacterName(%30s)\r\n" 
//						, pFISoc->m_ArenaInfo.TeamNum, GET_ARENA_STATE_STRING(pFISoc->m_ArenaInfo.State), pFISoc->m_character.CharacterName);
				continue;
			}
			if(ARENA_STATE_READY_FINISH > pFISoc->m_ArenaInfo.State)
			{// 2007-05-22 by dhjin, ARENA_STATE_READY_FINISH 상태 이상인 유저는 상태 유지 시켜준다.
				pFISoc->m_ArenaInfo.State	=	i_nState;
			}
	
			pFISoc->SendAddData(SendBuf, MSG_SIZE(MSG_FC_ARENA_TEAM_READY));
		}
	}

}

///////////////////////////////////////////////////////////////////////////////
/// \fn			INT CArenaManager::MakeMSG_FC_ARENA_TEAM_MEMBERLIST_OK(SARENA_TEAM_CHARACTERINFO * o_pArenaTeamMemberInfo, vectorClientIndex *i_pvectClient)
/// \brief		팀의 맴버 갱신 정보를 만든다
/// \author		dhjin
/// \date		2007-04-25 ~ 2007-04-25
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
INT CArenaManager::MakeMSG_FC_ARENA_TEAM_MEMBERLIST_OK(SARENA_TEAM_CHARACTERINFO * o_pArenaTeamMemberInfo, vectorClientIndex *i_pvectClient)
{
	INT	nTeamListCount = 0;

	vectorClientIndex::iterator itr = i_pvectClient->begin();
	for(; itr != i_pvectClient->end(); itr++)
	{
		CFieldIOCPSocket *pFISoc = this->m_pFieldIOCP16->GetFieldIOCPSocket(*itr);
		if(NULL == pFISoc
			|| FALSE == pFISoc->IsValidCharacter(FALSE))
		{
//			// 2007-05-18 by cmkwon, 테스트 로그
//			g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ARENA] ErrorTeamMember MakeMSG_FC_ARENA_TEAM_MEMBERLIST_OK - TeamNum(%4d) TeamState(%30s) CharacterName(%30s)\r\n" 
//						, pFISoc->m_ArenaInfo.TeamNum, GET_ARENA_STATE_STRING(pFISoc->m_ArenaInfo.State), pFISoc->m_character.CharacterName);
			continue;
		}
		o_pArenaTeamMemberInfo[nTeamListCount].ClientIndex  = pFISoc->m_character.ClientIndex;
		o_pArenaTeamMemberInfo[nTeamListCount].ArenaState	= pFISoc->m_ArenaInfo.State;
		util::strncpy(o_pArenaTeamMemberInfo[nTeamListCount].CharacterName, pFISoc->m_character.CharacterName, SIZE_MAX_CHARACTER_NAME);
		o_pArenaTeamMemberInfo[nTeamListCount].PlayCharacterType	= pFISoc->m_nPlayCharacterType;		// 2012-06-08 by jhseol, 아레나 추가개발part2 - 케릭터 : 타입 복사
		nTeamListCount++;
	}

	return nTeamListCount;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			Err_t CArenaManager::DeleteMember(CFieldIOCPSocket * i_pFISocket)
/// \brief		팀의 맴버를 삭제하고 팀의 맴버가 존재하면 팀의 맴버 갱신 정보를 보낸다
/// \author		dhjin
/// \date		2007-04-25 ~ 2007-04-25
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
Err_t CArenaManager::DeleteMember(CFieldIOCPSocket * i_pFISocket)
{
	//////////////////////////////////////////////////////////////////////////
	// 2007-04-18 by dhjin, 팀 번호에 맞는 팀 클래스를 찾는다. 팀 번호가 없다면 오류
	mt_auto_lock mta(&m_mtmapArenaTeamManager);
	CArenaTeam * pTeam = this->FindTeam(i_pFISocket->m_ArenaInfo.TeamNum);
	if(NULL == pTeam)
	{
		return ERR_ARENA_NO_SUCH_TEAM;
	}

	//////////////////////////////////////////////////////////////////////////
	// 2007-04-18 by dhjin, 팀의 아레나 모드가 같은지 체크
	if(i_pFISocket->m_ArenaInfo.ArenaMode != pTeam->GetTeamMode())
	{
		return ERR_ARENA_NOT_MATCH_MODE;
	}
	
	// 2008-01-16 by dhjin,  아레나 통합 - 임시로 변경했던 유저 세력을 정상값으로 변경
	i_pFISocket->m_character.InfluenceType	= pTeam->GetTeamInfl();

	//////////////////////////////////////////////////////////////////////////
	// 2007-04-18 by dhjin, 팀원 삭제
	pTeam->DeleteTeamList(i_pFISocket->m_character.ClientIndex);

//	// 2007-05-18 by cmkwon, 테스트 로그
//	g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ARENA] ReaveTeam - TeamNum(%4d) AState(%30s) DMRoomNum(%d) CharName(%s)\r\n" 
//			, i_pFISocket->m_ArenaInfo.TeamNum, GET_ARENA_STATE_STRING(i_pFISocket->m_ArenaInfo.State), i_pFISocket->m_ArenaInfo.DeathMatchRoomNum, i_pFISocket->m_character.CharacterName);
	
//	DBGOUT("[ARENA] T : DeleteTeamList : Team = %d, State = %d, TeamSize = %d\r\n" 
//			, pTeam->GetTeamNum(), pTeam->GetTeamState(), pTeam->GetTeamSize());

	//////////////////////////////////////////////////////////////////////////
	// 2007-04-23 by dhjin, 팀원이 존재하는지 체크한다
	if(pTeam->CheckEmptyMember())
	{
		this->DeleteTeam(pTeam);
		return ERR_NO_ERROR;
	}
	
	//////////////////////////////////////////////////////////////////////////
	// 2007-04-26 by dhjin, ARENA_STATE_READY 였다면 ARENA_STATE_JOINING로 변경
	if(ARENA_STATE_READY == pTeam->GetTeamState()
		|| ARENA_STATE_READY_FINISH == pTeam->GetTeamState())
	{
		pTeam->SetTeamState(ARENA_STATE_JOINING);
		// 2007-06-05 by dhjin, 팀 참가 순위 시간 설정.
		pTeam->SetTeamJoinRankTime();
	}

	//////////////////////////////////////////////////////////////////////////
	// 2007-05-04 by dhjin, 아레나 대전 중이면 리턴한다.
	if(ARENA_STATE_FIGHTING_WARREADY == i_pFISocket->m_ArenaInfo.State
		|| ARENA_STATE_FIGHTING_WARING == i_pFISocket->m_ArenaInfo.State
		|| ARENA_STATE_FIGHTING_WARFINISH == i_pFISocket->m_ArenaInfo.State)
	{
		return ERR_NO_ERROR;
	}

	vectorClientIndex *pvectClient = pTeam->GetTeamList();
	vectorClientIndex::iterator itr = pvectClient->begin();
	for(; itr != pvectClient->end(); itr++)
	{
		CFieldIOCPSocket *tm_pFISocket		= this->m_pFieldIOCP16->GetFieldIOCPSocket(*itr);
		if(NULL == tm_pFISocket
			|| FALSE == tm_pFISocket->IsValidCharacter(FALSE))
		{
//			// 2007-05-18 by cmkwon, 테스트 로그
//			g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ARENA] ErrorTeamMember DeleteMember - TeamNum(%4d) TeamState(%30s) CharacterName(%30s)\r\n" 
//						, tm_pFISocket->m_ArenaInfo.TeamNum, GET_ARENA_STATE_STRING(tm_pFISocket->m_ArenaInfo.State), tm_pFISocket->m_character.CharacterName);
			continue;
		}

		if(ARENA_STATE_READY_FINISH > tm_pFISocket->m_ArenaInfo.State)
		{// 2007-05-22 by dhjin, ARENA_STATE_READY_FINISH 상태 이상인 유저는 상태 유지 시켜준다.
			tm_pFISocket->m_ArenaInfo.State	= ARENA_STATE_JOINING;
		}
	}
	mta.auto_unlock_cancel();			// 2007-05-14 by cmkwon, lock 해제

	//////////////////////////////////////////////////////////////////////////
	// 2007-04-23 by dhjin, 팀원이 있다면 정보 보내기
	this->SendTeamMemberList(i_pFISocket->m_ArenaInfo.TeamNum);

	return ERR_NO_ERROR;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CArenaManager::DeleteTeam(CArenaTeam * i_pArenaTeam)
/// \brief		팀 삭제
/// \author		dhjin
/// \date		2007-04-25 ~ 2007-04-25
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CArenaManager::DeleteTeam(CArenaTeam * i_pArenaTeam)
{
	//////////////////////////////////////////////////////////////////////////
	// 2007-04-19 by dhjin, 삭제하는 팀 번호를 팀 예약 번호에 할당한다.
	m_ReserveTeamNum	= i_pArenaTeam->GetTeamNum();
	
//	// 2007-05-18 by cmkwon, 테스트 로그
//	g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ARENA] DeleteTeam - TeamNum(%4d) TeamState(%30s) TeamSize(%4d)\r\n" 
//			, i_pArenaTeam->GetTeamNum(), GET_ARENA_STATE_STRING(i_pArenaTeam->GetTeamState()), i_pArenaTeam->GetTeamSize());
	
	//////////////////////////////////////////////////////////////////////////
	// 2007-04-18 by dhjin, m_mtmapArenaTeamManager에서는 삭제
	m_mtmapArenaTeamManager.deleteNoLock(i_pArenaTeam->GetTeamNum());

	//////////////////////////////////////////////////////////////////////////
	// 2007-04-18 by dhjin, m_pArenaTeamManager 초기화
	i_pArenaTeam->InitArenaTeamManager();

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BYTE CArenaManager::GetTeamState(int i_nTeamNum)
/// \brief		팀의 ARENA_STATE
/// \author		dhjin
/// \date		2007-04-25 ~ 2007-04-25
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BYTE CArenaManager::GetTeamState(int i_nTeamNum)
{
	//////////////////////////////////////////////////////////////////////////
	// 2007-04-18 by dhjin, 팀 번호에 맞는 팀 클래스를 찾는다. 팀 번호가 없다면 오류
	mt_auto_lock mta(&m_mtmapArenaTeamManager);
	CArenaTeam * pTeam = this->FindTeam(i_nTeamNum);
	if(NULL == pTeam)
	{
		return ARENA_STATE_NONE;
	}
//	DBGOUT("[ARENA] T : GetTeamState : Team = %d, State = %d, TeamSize = %d\r\n" 
//			, pTeam->GetTeamNum(), pTeam->GetTeamState(), pTeam->GetTeamSize());

	return pTeam->GetTeamState();	
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CArenaManager::ChectTeamFullReadyFinish(int i_nTeamNum)
/// \brief		팀원이 전부 ARENA_STATE_READY_FINISH인지 체크하여 TRUE이면 매칭을 시작한다
/// \author		dhjin
/// \date		2007-05-02 ~ 2007-05-02
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CArenaManager::ChectTeamFullReadyFinish(int i_nTeamNum)
{
	//////////////////////////////////////////////////////////////////////////
	// 2007-04-18 by dhjin, 팀 번호에 맞는 팀 클래스를 찾는다. 팀 번호가 없다면 오류
	mt_auto_lock mta(&m_mtmapArenaTeamManager);
	CArenaTeam * pATeam = this->FindTeam(i_nTeamNum);
	if(NULL == pATeam)
	{
		return FALSE;
	}

	//////////////////////////////////////////////////////////////////////////
	// 2007-05-04 by dhjin, 팀원이 꽉차 있는지 체크 
	if(!pATeam->CheckFullMember())
	{
		return FALSE;
	}
	
	//////////////////////////////////////////////////////////////////////////
	// 2007-05-02 by dhjin, 팀원이 모두 ARENA_STATE_READY_FINISH 인지 체크
	vectorClientIndex * pClientIndex = pATeam->GetTeamList();
	vectorClientIndex::iterator itr = pClientIndex->begin();
	for(; itr != pClientIndex->end(); itr++)
	{
		CFieldIOCPSocket *pFISocket = this->m_pFieldIOCP16->GetFieldIOCPSocket(*itr);
		if(NULL == pFISocket
			|| FALSE == pFISocket->IsValidCharacter(FALSE)
			|| i_nTeamNum != pFISocket->m_ArenaInfo.TeamNum)
		{
//			// 2007-05-18 by cmkwon, 테스트 로그
//			g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ARENA] ErrorTeamMember ChectTeamFullReadyFinish - TeamNum(%4d) TeamState(%30s) CharacterName(%30s)\r\n" 
//						, pFISocket->m_ArenaInfo.TeamNum, GET_ARENA_STATE_STRING(pFISocket->m_ArenaInfo.State), pFISocket->m_character.CharacterName);
			continue;
		}
		if(ARENA_STATE_READY_FINISH != pFISocket->m_ArenaInfo.State)
		{
			return FALSE;
		}
	}
	
	//////////////////////////////////////////////////////////////////////////
	// 2007-05-02 by dhjin, 팀 상태 ARENA_STATE_READY_FINISH로 변경
	pATeam->SetTeamState(ARENA_STATE_READY_FINISH);

	//////////////////////////////////////////////////////////////////////////
	// 2007-05-11 by dhjin, 팀 매칭 순번을 위해 시간값을 설정한다
	pATeam->SetTeamReadyFinishTime();

//	DBGOUT("[ARENA] T : ChectTeamFullReadyFinish : Team = %d, State = %d, TeamSize = %d\r\n" 
//			, pATeam->GetTeamNum(), pATeam->GetTeamState(), pATeam->GetTeamSize());
	
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
// 2007-05-11 by dhjin, 팀 준비 완료 시간 값으로 올림 차순 정렬한다
struct ArenaTeam_Sort_Increase_By_TeamReadyFinishTime: binary_function<CArenaTeam*, CArenaTeam*, bool>
{
	bool operator()(CArenaTeam * ATeam, CArenaTeam * BTeam)
	{
		return *(ATeam)->GetTeamReadyFinishTime() < *(BTeam)->GetTeamReadyFinishTime();	// 올림 차순 정렬
	};
};

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CArenaManager::TeamMatch(int i_nTeamNum)
/// \brief		동일한 조건을 가진 팀을 찾아서 아레나 맵으로 이동시킨다
/// \author		dhjin
/// \date		2007-04-25 ~ 2007-04-25
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CArenaManager::TeamMatch(int i_nTeamNum)
{
	mt_auto_lock mtTeam(&m_mtmapArenaTeamManager);

	//////////////////////////////////////////////////////////////////////////
	// 2007-04-18 by dhjin, 팀 번호에 맞는 팀 클래스를 찾는다. 팀 번호가 없다면 오류
	CArenaTeam * pATeam = this->FindTeam(i_nTeamNum);
	if(NULL == pATeam
		|| ARENA_STATE_READY_FINISH != pATeam->GetTeamState())
	{
		return FALSE;
	}
	
	//////////////////////////////////////////////////////////////////////////
	// 2007-05-11 by dhjin, Team관련 복사 본을 만들어 정렬을 한다.
	vector<CArenaTeam*>			vectCopyArenaTeamManager;
	vectCopyArenaTeamManager.clear();
	mtmapINT2ArenaTeamManager::iterator itrtmTeam = m_mtmapArenaTeamManager.begin();
	for(; itrtmTeam != m_mtmapArenaTeamManager.end(); itrtmTeam++)
	{
		//////////////////////////////////////////////////////////////////////////
		// 2008-06-19 by dhjin, 같은 서버 세력 이벤트 팀도 매칭 가능하게 밑과 같이 수정
//		if(ARENA_STATE_READY_FINISH == itrtmTeam->second->GetTeamState()				// 대기 상태인지 체크
//			&& pATeam->GetTeamMode() == itrtmTeam->second->GetTeamMode()				// 같은 방 모드인지 체크
////			&& pATeam->GetTeamInfl() != itrtmTeam->second->GetTeamInfl()				// 서로 다른 세력인지 체크
//			&& FALSE == (pATeam->GetTeamInfl() == itrtmTeam->second->GetTeamInfl()
//						&& pATeam->GetTeamServerID() == itrtmTeam->second->GetTeamServerID() )	// 2008-01-03 by dhjin, 아레나 통합 - 
//			&& pATeam->GetTeamStartLevel() == itrtmTeam->second->GetTeamStartLevel()	// 레벨 등급이 같은지 체크
//			&& pATeam->GetTeamSize() == itrtmTeam->second->GetTeamSize())				// 방 크기가 같은지 체크
//		{
//			vectCopyArenaTeamManager.push_back(itrtmTeam->second);
//		}

		if(pATeam->GetTeamEvent()
			|| itrtmTeam->second->GetTeamEvent())
		{
			if(!pATeam->GetTeamEvent()
				|| !itrtmTeam->second->GetTeamEvent()
				)
			{
				continue;
			}		

			if(pATeam->GetTeamNum() == itrtmTeam->second->GetTeamNum())
			{// 2008-06-24 by dhjin, 버그 수정
				continue;
			}

			// 2007-04-19 by dhjin, 팀이 매칭 가능한지 체크 
			if(ARENA_STATE_READY_FINISH == itrtmTeam->second->GetTeamState()				// 대기 상태인지 체크
				&& pATeam->GetTeamMode() == itrtmTeam->second->GetTeamMode()				// 같은 방 모드인지 체크
				&& pATeam->GetTeamStartLevel() == itrtmTeam->second->GetTeamStartLevel()	// 레벨 등급이 같은지 체크
				&& pATeam->GetTeamSize() == itrtmTeam->second->GetTeamSize()				// 방 크기가 같은지 체크
				//////////////////////////////////////////////////////////////////////////////
				// start 2012-04-12 by jhseol, 아레나 추가개발 - 선택한 아레나 맵 체크
				&& (pATeam->GetTeamSelectArenaMapNum() == itrtmTeam->second->GetTeamSelectArenaMapNum() ||
					ARENA_MAP_SELECT_RANDOM == pATeam->GetTeamSelectArenaMapNum()	||
					ARENA_MAP_SELECT_RANDOM == itrtmTeam->second->GetTeamSelectArenaMapNum())
				// end 2012-04-12 by jhseol, 아레나 추가개발 - 선택한 아레나 맵 체크
				)
			{
				vectCopyArenaTeamManager.push_back(itrtmTeam->second);
				continue;
			}
		}

		if(ARENA_STATE_READY_FINISH == itrtmTeam->second->GetTeamState()				// 대기 상태인지 체크
			&& pATeam->GetTeamMode() == itrtmTeam->second->GetTeamMode()				// 같은 방 모드인지 체크
//			&& pATeam->GetTeamInfl() != itrtmTeam->second->GetTeamInfl()				// 서로 다른 세력인지 체크
			&& FALSE == (pATeam->GetTeamInfl() == itrtmTeam->second->GetTeamInfl()
						) //&& pATeam->GetTeamServerID() == itrtmTeam->second->GetTeamServerID() )	// 2008-01-03 by dhjin, 아레나 통합 - 
			&& pATeam->GetTeamStartLevel() == itrtmTeam->second->GetTeamStartLevel()	// 레벨 등급이 같은지 체크
			&& pATeam->GetTeamSize() == itrtmTeam->second->GetTeamSize()				// 방 크기가 같은지 체크
			//////////////////////////////////////////////////////////////////////////////
			// start 2012-04-12 by jhseol, 아레나 추가개발 - 선택한 아레나 맵 체크
			&& (pATeam->GetTeamSelectArenaMapNum() == itrtmTeam->second->GetTeamSelectArenaMapNum() ||
				ARENA_MAP_SELECT_RANDOM == pATeam->GetTeamSelectArenaMapNum()	||
				ARENA_MAP_SELECT_RANDOM == itrtmTeam->second->GetTeamSelectArenaMapNum())
			// end 2012-04-12 by jhseol, 아레나 추가개발 - 선택한 아레나 맵 체크
			)
		{
			vectCopyArenaTeamManager.push_back(itrtmTeam->second);
		}
	}
	sort(vectCopyArenaTeamManager.begin(), vectCopyArenaTeamManager.end(), ArenaTeam_Sort_Increase_By_TeamReadyFinishTime());

	BOOL NotMatch = TRUE;

	//////////////////////////////////////////////////////////////////////////
	// 2007-04-19 by dhjin, 대전 조건을 만족하는 상대 팀을 찾는다.
	CArenaTeam * pBTeam;
	vector<CArenaTeam*>::iterator itrBTeam = vectCopyArenaTeamManager.begin();
	
	for(; itrBTeam != vectCopyArenaTeamManager.end(); itrBTeam++)
	{
		pBTeam = *itrBTeam;

		if(pATeam->GetTeamEvent()
			|| pBTeam->GetTeamEvent())
		{
			if(!pATeam->GetTeamEvent()
				|| !pBTeam->GetTeamEvent()
				)
			{
				continue;
			}
			
			if(pATeam->GetTeamNum() == pBTeam->GetTeamNum())
			{// 2008-06-24 by dhjin, 버그 수정
				continue;
			}

			// 2007-04-19 by dhjin, 팀이 매칭 가능한지 체크 
			if(ARENA_STATE_READY_FINISH == pBTeam->GetTeamState()				// 대기 상태인지 체크
				&& pATeam->GetTeamMode() == pBTeam->GetTeamMode()				// 같은 방 모드인지 체크
				&& pATeam->GetTeamStartLevel() == pBTeam->GetTeamStartLevel()	// 레벨 등급이 같은지 체크
				&& pATeam->GetTeamSize() == pBTeam->GetTeamSize()				// 방 크기가 같은지 체크
				//////////////////////////////////////////////////////////////////////////////
				// start 2012-04-12 by jhseol, 아레나 추가개발 - 선택한 아레나 맵 체크
				&& (pATeam->GetTeamSelectArenaMapNum() == pBTeam->GetTeamSelectArenaMapNum() ||
					ARENA_MAP_SELECT_RANDOM == pATeam->GetTeamSelectArenaMapNum()	||
					ARENA_MAP_SELECT_RANDOM == pBTeam->GetTeamSelectArenaMapNum())
				// end 2012-04-12 by jhseol, 아레나 추가개발 - 선택한 아레나 맵 체크
				)
			{
				NotMatch = FALSE;
				break;
			}
		}

		// 2007-04-19 by dhjin, 팀이 매칭 가능한지 체크 
		if(ARENA_STATE_READY_FINISH == pBTeam->GetTeamState()				// 대기 상태인지 체크
			&& pATeam->GetTeamMode() == pBTeam->GetTeamMode()				// 같은 방 모드인지 체크
			&& FALSE == (pATeam->GetTeamInfl() == pBTeam->GetTeamInfl()
						)//&& pATeam->GetTeamServerID() == pBTeam->GetTeamServerID() )	// 2008-01-03 by dhjin, 아레나 통합 - 
			&& pATeam->GetTeamStartLevel() == pBTeam->GetTeamStartLevel()	// 레벨 등급이 같은지 체크
			&& pATeam->GetTeamSize() == pBTeam->GetTeamSize()				// 방 크기가 같은지 체크
			//////////////////////////////////////////////////////////////////////////////
			// start 2012-04-12 by jhseol, 아레나 추가개발 - 선택한 아레나 맵 체크
			&& (pATeam->GetTeamSelectArenaMapNum() == pBTeam->GetTeamSelectArenaMapNum() ||
				ARENA_MAP_SELECT_RANDOM == pATeam->GetTeamSelectArenaMapNum()	||
				ARENA_MAP_SELECT_RANDOM == pBTeam->GetTeamSelectArenaMapNum())
			// end 2012-04-12 by jhseol, 아레나 추가개발 - 선택한 아레나 맵 체크
			)
		{
			NotMatch = FALSE;
			break;
		}
	}	
	

	//////////////////////////////////////////////////////////////////////////
	// 2007-04-27 by dhjin, 대전 할 팀을 찾지 못했다면 리턴한다
	if(NotMatch)
	{
		return FALSE;
	}

	MAP_CHANNEL_INDEX tmMap;
	util::zero(&tmMap, sizeof(MAP_CHANNEL_INDEX));
	//////////////////////////////////////////////////////////////////////////
	// 2007-04-20 by dhjin, 생성할 수 있는 맵이 있는지 체크하여 맵 정보를 반환한다.
	
	if(!this->FindEmptyRoom(&tmMap.MapIndex, &tmMap.ChannelIndex, pATeam->GetTeamSelectArenaMapNum(), pBTeam->GetTeamSelectArenaMapNum()))	// 2012-04-12 by jhseol, 아레나 추가개발 - 선택한 아레나 맵 체크
	{
		return FALSE;
	}

	//////////////////////////////////////////////////////////////////////////
	// 2007-04-23 by dhjin, 방 생성
	if(!this->CreateDeathMatchRoom(tmMap, pATeam, pBTeam))
	{
		// 2007-05-08 by dhjin, 방 생성 실패시 맵할당 초기화
		this->DeleteArenaMapManager(&tmMap);
		return FALSE;
	}

	//////////////////////////////////////////////////////////////////////////
	// 2007-05-09 by dhjin, 방 생성에 성공하였으면 팀 상태를 대전 상태로 변경한다.
	pATeam->SetTeamState(ARENA_STATE_FIGHTING_WARREADY);
	pBTeam->SetTeamState(ARENA_STATE_FIGHTING_WARREADY);

	CAtumLogSender::SendLogMessageArenaTeamMatch(pATeam->GetTeamMode(), pATeam->GetTeamSize(), pATeam->GetTeamStartLevel(), pATeam->GetTeamEndLevel(), pATeam->GetTeamSelectArenaMapNum());	// 2012-04-12 by jhseol, 아레나 추가개발 - 선택한 아레나 맵 체크

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CArenaManager::FindEmptyRoom(MapIndex_t * o_nMapIndex, ChannelIndex_t * o_nChannelIndex, MapIndex_t i_nTeamAMapIndex, MapIndex_t i_nTeamBMapIndex)
/// \brief		생성할 수 있는 아레나 맵과 채널이 존재하는지 체크하여 맵과 채널 정보를 준다
/// \author		dhjin
/// \date		2007-04-25 ~ 2007-04-25
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CArenaManager::FindEmptyRoom(MapIndex_t * o_nMapIndex, ChannelIndex_t * o_nChannelIndex, MapIndex_t i_nTeamAMapIndex, MapIndex_t i_nTeamBMapIndex)	// 2012-04-12 by jhseol, 아레나 추가개발 - 선택한 아레나 맵 파라미터 추가
{
	mt_auto_lock mtMap(&m_mtvectArenaMapManager);
	//////////////////////////////////////////////////////////////////////////////
	// 2012-04-12 by jhseol, 아레나 추가개발 - 서로 매칭된 양팀의 맵이 모두 랜덤일 경우 서버에서 맵을 지정			
	MapIndex_t	tempMapIndex = i_nTeamAMapIndex;
	if(ARENA_MAP_SELECT_RANDOM == i_nTeamAMapIndex)
	{
		if(ARENA_MAP_SELECT_RANDOM == i_nTeamBMapIndex)
		{
			if(FALSE == m_TotalArenaMapCount)
			{
				char	szSystemLog[256];
				sprintf(szSystemLog, "[ERROR] Loading Arena Map Info Empty[%d]\r\n", m_TotalArenaMapCount);
				g_pFieldGlobal->WriteSystemLog(szSystemLog);
			}
			else
			{
				tempMapIndex = i_nTeamAMapIndex + ((RAND100()%m_TotalArenaMapCount) + TRUE);	// start 2012-04-12 by jhseol, 아레나 추가개발 - RAND100()%m_TotalArenaMapCount 결과에 +1을 해줘야 재대로 된 맵을 지정할 수 있다
			}
		}
		else
		{
			tempMapIndex = i_nTeamBMapIndex;
		}
	}
	// end 2012-04-12 by jhseol, 아레나 추가개발 - 서로 매칭된 양팀의 맵이 모두 랜덤일 경우 서버에서 맵을 지정
	mtvectSARENA_MAP_MANAGER::iterator itr = m_mtvectArenaMapManager.begin();
	for(; itr != m_mtvectArenaMapManager.end(); itr++)
	{
		if(tempMapIndex != itr->ArenaMap)	// 2012-04-12 by jhseol, 아레나 추가개발 - 선택한 아레나 맵과 같은지 체크
		{
			continue;
		}
		if(FALSE == itr->bArenaChannelSet)
		{
			*o_nMapIndex			= itr->ArenaMap;
			*o_nChannelIndex		= itr->ArenaMapChannel;
			itr->bArenaChannelSet	= TRUE;
			
//			DBGOUT("[ARENA] D : FindEmptyRoom : MapNum = %d, MapChannel = %d, Set = %d\r\n" 
//				, itr->ArenaMap, itr->ArenaMapChannel, itr->bArenaChannelSet);

			return TRUE;
		}
	}

	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CArenaManager::CreateDeathMatchRoom(MAP_CHANNEL_INDEX i_SMapInfo, CArenaTeam * i_pATeam, CArenaTeam * i_pBTeam)
/// \brief		아레나 방을 생성한다
/// \author		dhjin
/// \date		2007-04-25 ~ 2007-04-25
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CArenaManager::CreateDeathMatchRoom(MAP_CHANNEL_INDEX i_SMapInfo, CArenaTeam * i_pATeam, CArenaTeam * i_pBTeam)
{
	//////////////////////////////////////////////////////////////////////////
	// 2007-04-19 by dhjin, 방 생성
	mt_auto_lock mtDeahMatch(&m_mtmapArenaDeathMatchRoom);
	int EmptyRoomNum;
	for(EmptyRoomNum = 0; EmptyRoomNum < SIZE_MAX_ARENA_ROOM; EmptyRoomNum++)
	{
		if(!m_mtmapArenaDeathMatchRoom.count(EmptyRoomNum))
		{
			break;
		}
	}
	
	//////////////////////////////////////////////////////////////////////////
	// 2007-05-08 by dhjin, 빈방이 없을때
	if(SIZE_MAX_ARENA_ROOM <= EmptyRoomNum)
	{
		return FALSE;
	}

	// 2008-04-11 by dhjin. 아레나 통합 - 관전자가 아레나 방 생성전에 들어올 경우 버그 발생으로 InitDeathMatchRoomState()함수로 위치 변경
//	// 2008-02-20 by dhjin, 아레나 통합 - 맵에 유저 정보 초기화
//	CFieldMapChannel *pInitFMChann = m_pFieldIOCP16->GetFieldMapChannelByMapChannelIndex(i_SMapInfo);
//	pInitFMChann->ResetUserMapChannel();
//	pInitFMChann->InitmapCharacterUniqueNumberMapProject();

	//////////////////////////////////////////////////////////////////////////
	// 2007-04-25 by dhjin, 방 정보 입력
	SARENA_PAY_INFO tmPayInfo;
	if(!this->GetPayInfo(i_pATeam->GetTeamMode(), i_pATeam->GetTeamSize(), &tmPayInfo))
	{
		return FALSE;
	}

	CArenaDeathMatchRoom * tmArenaDMRoom = this->GetArenaDeathMatchRoom(EmptyRoomNum);
	tmArenaDMRoom->SetPayInfo(&tmPayInfo);
	tmArenaDMRoom->SetDeathMatchRoomNum(EmptyRoomNum);
	tmArenaDMRoom->SetDeathMatchMap(i_SMapInfo);
	if(INFLUENCE_TYPE_VCN == i_pATeam->GetTeamInfl())
	{
		tmArenaDMRoom->SetVCNTeam(i_pATeam->GetTeamList(), i_pATeam->GetTeamNum());
		tmArenaDMRoom->SetANITeam(i_pBTeam->GetTeamList(), i_pBTeam->GetTeamNum());
		// 2008-01-10 by dhjin, 아레나 통합 - 필드 서버 ID 설정
		//tmArenaDMRoom->SetVCNMFSID(i_pATeam->GetTeamServerID());
		tmArenaDMRoom->SetVCNMFSInfl(i_pATeam->GetTeamInfl());
		//tmArenaDMRoom->SetANIMFSID(i_pBTeam->GetTeamServerID());
		tmArenaDMRoom->SetANIMFSInfl(i_pBTeam->GetTeamInfl());
		// 2008-01-16 by dhjin, 아레나 통합 - 팀 세력 임의 설정하여 유저 세력을 임의로 변경한다.
		i_pATeam->SetTeamArenaInfl(INFLUENCE_TYPE_VCN);
		i_pBTeam->SetTeamArenaInfl(INFLUENCE_TYPE_ANI);
	}
	else
	{
		tmArenaDMRoom->SetVCNTeam(i_pBTeam->GetTeamList(), i_pBTeam->GetTeamNum());
		tmArenaDMRoom->SetANITeam(i_pATeam->GetTeamList(), i_pATeam->GetTeamNum());
		// 2008-01-10 by dhjin, 아레나 통합 - 필드 서버 ID 설정
		//tmArenaDMRoom->SetVCNMFSID(i_pBTeam->GetTeamServerID());
		tmArenaDMRoom->SetVCNMFSInfl(i_pBTeam->GetTeamInfl());
		//tmArenaDMRoom->SetANIMFSID(i_pATeam->GetTeamServerID());
		tmArenaDMRoom->SetANIMFSInfl(i_pATeam->GetTeamInfl());
		// 2008-01-16 by dhjin, 아레나 통합 - 팀 세력 임의 설정하여 유저 세력을 임의로 변경한다.
		i_pATeam->SetTeamArenaInfl(INFLUENCE_TYPE_ANI);
		i_pBTeam->SetTeamArenaInfl(INFLUENCE_TYPE_VCN);
	}


	BYTE	tmDeathCount;
	BYTE	tmPlayLimitedTime;
	if(!this->GetDeathMatchDBInfo(i_pATeam->GetTeamMode(), i_pATeam->GetTeamSize(), &tmDeathCount, &tmPlayLimitedTime))
	{
		return FALSE;
	}
	tmArenaDMRoom->SetPlayLimitedTime(tmPlayLimitedTime);
	tmArenaDMRoom->SetVCNDeathCount(tmDeathCount);
	tmArenaDMRoom->SetANIDeathCount(tmDeathCount);
	tmArenaDMRoom->SetCreateRoomTime();
	m_mtmapArenaDeathMatchRoom.insertNoLock(EmptyRoomNum, tmArenaDMRoom);	

	//////////////////////////////////////////////////////////////////////////
	// 2007-04-23 by dhjin, 데스매치 방 정보 전송 A팀.
	MSG_FC_ARENA_ENTER_ROOM tmSArenaEnterRoomA;
	util::zero(&tmSArenaEnterRoomA, sizeof(MSG_FC_ARENA_ENTER_ROOM));
	tmSArenaEnterRoomA.ArenaMode		= ARENAMODE_DEATHMATCH;
	tmSArenaEnterRoomA.ArenaState		= ARENA_STATE_FIGHTING_WARREADY;
	tmSArenaEnterRoomA.VCNDeathCount	= tmDeathCount;
	tmSArenaEnterRoomA.ANIDeathCount	= tmDeathCount;
	tmSArenaEnterRoomA.PlayLimitedTime	= tmPlayLimitedTime;
	tmSArenaEnterRoomA.StartRoomTime	= *(tmArenaDMRoom)->GetCreateRoomTime();
//	tmSArenaEnterRoomA.StartRoomTime.AddDateTime(0,0,0,0,1,0);	// 2007-05-07 by dhjin, 대기 시간을 더해서 전송한다.
	tmSArenaEnterRoomA.TeamNum			= i_pATeam->GetTeamNum();
	tmSArenaEnterRoomA.ArenaServerTime.SetCurrentDateTime();	// 2009-09-10 by jwlee, 아레나 서버 시간 오차 버그 수정 - 
	this->SendDeathMatchEnterRoom(&tmSArenaEnterRoomA, i_pATeam->GetTeamList(), EmptyRoomNum);

	//////////////////////////////////////////////////////////////////////////
	// 2007-04-23 by dhjin, 데스매치 방 정보 전송 B팀.
	MSG_FC_ARENA_ENTER_ROOM tmSArenaEnterRoomB;
	util::zero(&tmSArenaEnterRoomB, sizeof(MSG_FC_ARENA_ENTER_ROOM));
	tmSArenaEnterRoomB.ArenaMode		= ARENAMODE_DEATHMATCH;
	tmSArenaEnterRoomB.ArenaState		= ARENA_STATE_FIGHTING_WARREADY;
	tmSArenaEnterRoomB.VCNDeathCount	= tmDeathCount;
	tmSArenaEnterRoomB.ANIDeathCount	= tmDeathCount;
	tmSArenaEnterRoomB.PlayLimitedTime	= tmPlayLimitedTime;
	tmSArenaEnterRoomB.StartRoomTime	= *(tmArenaDMRoom)->GetCreateRoomTime();
//	tmSArenaEnterRoomB.StartRoomTime.AddDateTime(0,0,0,0,1,0);	// 2007-05-07 by dhjin, 대기 시간을 더해서 전송한다.
	tmSArenaEnterRoomB.TeamNum			= i_pBTeam->GetTeamNum();
	tmSArenaEnterRoomB.ArenaServerTime.SetCurrentDateTime();	// 2009-09-10 by jwlee, 아레나 서버 시간 오차 버그 수정 - 
	this->SendDeathMatchEnterRoom(&tmSArenaEnterRoomB, i_pBTeam->GetTeamList(), EmptyRoomNum);

//	// 2007-05-18 by cmkwon, 테스트 로그
//	g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ARENA] DMRoomCreate(1-1) - DMRoomNum(%4d) AState(%30s) BTeamNum(%4d) ATeamNum(%4d) MapInfo(%s)\r\n" 
//			, tmArenaDMRoom->GetDeathMatchRoomNum(), GET_ARENA_STATE_STRING(tmArenaDMRoom->GetDeathMatchRoomNum()), i_pBTeam->GetTeamNum(), i_pATeam->GetTeamNum(), GET_MAP_STRING(i_SMapInfo));

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CArenaManager::SendDeathMatchEnterRoom(MSG_FC_ARENA_ENTER_ROOM *i_pSMSG, vectorClientIndex *i_pvectClient, INT i_nDeathMatchRoomNum)
/// \brief		데스매치 방 입장을 시킨다
/// \author		dhjin
/// \date		2007-04-25 ~ 2007-04-25
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CArenaManager::SendDeathMatchEnterRoom(MSG_FC_ARENA_ENTER_ROOM *i_pSMSG, vectorClientIndex *i_pvectClient, INT i_nDeathMatchRoomNum)
{
	//////////////////////////////////////////////////////////////////////////
	// 2007-04-23 by dhjin, 메세지 만들기
	INIT_MSG_WITH_BUFFER(MSG_FC_ARENA_ENTER_ROOM, T_FC_ARENA_ENTER_ROOM, pArenaEnterRoom, SendBuf);
	pArenaEnterRoom->ArenaMode			= i_pSMSG->ArenaMode;
	pArenaEnterRoom->ArenaState			= i_pSMSG->ArenaState;
	pArenaEnterRoom->VCNDeathCount		= i_pSMSG->VCNDeathCount;
	pArenaEnterRoom->ANIDeathCount		= i_pSMSG->ANIDeathCount;
	pArenaEnterRoom->PlayLimitedTime	= i_pSMSG->PlayLimitedTime;
	pArenaEnterRoom->StartRoomTime		= i_pSMSG->StartRoomTime;
	pArenaEnterRoom->TeamNum			= i_pSMSG->TeamNum;
	pArenaEnterRoom->ArenaServerTime	= i_pSMSG->ArenaServerTime;		// 2009-09-10 by jwlee, 아레나 서버 시간 오차 버그 수정 - 
		
	//////////////////////////////////////////////////////////////////////////
	// 2007-04-23 by dhjin, 팀원 모두에게 정보 전송
	vectorClientIndex::iterator itr = i_pvectClient->begin();
	for(; itr != i_pvectClient->end(); itr++)
	{
		CFieldIOCPSocket *pFISoc = this->m_pFieldIOCP16->GetFieldIOCPSocket(*itr);
		if(NULL == pFISoc
			|| FALSE == pFISoc->IsValidCharacter(FALSE)
			|| i_pSMSG->TeamNum != pFISoc->m_ArenaInfo.TeamNum)
		{
//			// 2007-05-18 by cmkwon, 테스트 로그
//			g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ARENA] ErrorTeamMember SendDeathMatchEnterRoom - TeamNum(%4d) TeamState(%30s) CharacterName(%30s)\r\n" 
//						, pFISoc->m_ArenaInfo.TeamNum, GET_ARENA_STATE_STRING(pFISoc->m_ArenaInfo.State), pFISoc->m_character.CharacterName);
			continue;
		}
		pFISoc->m_ArenaInfo.State				= i_pSMSG->ArenaState;
		pFISoc->m_ArenaInfo.DeathMatchRoomNum	= i_nDeathMatchRoomNum;
		pFISoc->SendAddData(SendBuf, MSG_SIZE(MSG_FC_ARENA_ENTER_ROOM));
	}
	
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CArenaManager::SendDeathMatchRoomInfo(int i_nTeamNum, int i_nDeathMatchRoomNum, BYTE i_nInfl)
/// \brief		데스매치 모드에서 피격 당해 상대편 데스카운트를 차감하고 정보를 전송한다.
/// \author		dhjin
/// \date		2007-04-25 ~ 2007-04-25
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CArenaManager::SendDeathMatchRoomInfo(int i_nTeamNum, int i_nDeathMatchRoomNum, BYTE i_nInfl)
{
	mt_auto_lock mta(&m_mtmapArenaDeathMatchRoom);

	mtmapINT2ArenaDeathMatchRoom::iterator itr = m_mtmapArenaDeathMatchRoom.find(i_nDeathMatchRoomNum);
	if(itr == m_mtmapArenaDeathMatchRoom.end())
	{
		return;
	}

	itr->second->DecreaseDeathCount(i_nInfl);	

}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CArenaManager::SendDeathMatchUserWarpCity(int i_nDeathMatchRoomNum, CFieldIOCPSocket * i_pFis)
/// \brief		데스매치가 끝나 마을로 워프를 요청
/// \author		dhjin
/// \date		2007-04-25 ~ 2007-04-25
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CArenaManager::SendDeathMatchUserWarpCity(int i_nDeathMatchRoomNum, CFieldIOCPSocket * i_pFis)
{	
	mt_auto_lock mta(&m_mtmapArenaDeathMatchRoom);

	mtmapINT2ArenaDeathMatchRoom::iterator itr = m_mtmapArenaDeathMatchRoom.find(i_nDeathMatchRoomNum);
	if(itr == m_mtmapArenaDeathMatchRoom.end())
	{
		return;
	}
	//////////////////////////////////////////////////////////////////////////
	// 2007-06-18 by dhjin, 관전자는 하지 않는다.
	if(FALSE == i_pFis->m_ArenaInfo.WatchState)
	{
		itr->second->ReadyUserWarp(i_pFis);
	}

	if (COMPARE_BODYCON_BIT(i_pFis->m_character.BodyCondition, BODYCON_DEAD_MASK))
	{
		i_pFis->CharacterDeadGameStartRoutine(FALSE, i_pFis->m_character.HP, i_pFis->m_character.DP);		
	}

	// 2008-01-15 by dhjin, 아레나 통합 - 밑에는 불필요
//	//////////////////////////////////////////////////////////////////////////
//	// 2007-06-18 by dhjin, 마을로 워프 시 아레나 맵으로 이동하기 전 HP, DP, SP, EP로 설정한다. (관전은 하지 않는다.)
//	if(FALSE == i_pFis->m_ArenaInfo.WatchState)
//	{
//		i_pFis->ArenaWarpCitySetHPDPSPEP();
//	}

	//////////////////////////////////////////////////////////////////////////
	// 2007-06-20 by dhjin, 관전자 리스트에서 관전자를 삭제한다. 관전자 처리를 따로한다.
	if(i_pFis->m_ArenaInfo.WatchState)
	{
		itr->second->DeleteWatchUserList(i_pFis->m_character.ClientIndex);
		i_pFis->m_ArenaInfo.WatchState = FALSE;

		// 2008-01-15 by dhjin, 아레나 통합 - 밑에는 불필요
//		i_pFis->WarpToCityMap();
		// 2007-05-09 by dhjin, 격추/피격/데스매치 방 정보 초기화
		i_pFis->m_ArenaInfo.ShootingDown		= 0;
		i_pFis->m_ArenaInfo.SufferingAttack		= 0;
		i_pFis->m_ArenaInfo.LostPoint			= 0;
		i_pFis->m_ArenaInfo.DeathMatchRoomNum	= -1;
		i_pFis->m_ArenaInfo.State = ARENA_STATE_NONE;
		return;

	}
	mta.auto_unlock_cancel();

	// 2008-01-15 by dhjin, 아레나 통합 - 밑에는 불필요
//	i_pFis->WarpToCityMap();
	// 2007-05-09 by dhjin, 격추/피격/데스매치 방 정보 초기화
	i_pFis->m_ArenaInfo.ShootingDown		= 0;
	i_pFis->m_ArenaInfo.SufferingAttack		= 0;
	i_pFis->m_ArenaInfo.LostPoint			= 0;
	i_pFis->m_ArenaInfo.DeathMatchRoomNum	= -1;

	//////////////////////////////////////////////////////////////////////////
	// 2007-05-02 by dhjin, 유저 워프 시킨 후 유저의 팀 상태를 체크하여 유저 및 팀의 상태를 변경한다
	mt_auto_lock mtTeam(&m_mtmapArenaTeamManager);
	CArenaTeam * pTeam = this->FindTeam(i_pFis->m_ArenaInfo.TeamNum);
	if(NULL == pTeam)
	{
		return;
	}

	if(pTeam->CheckFullMember())
	{
		pTeam->SetTeamState(ARENA_STATE_READY);
		i_pFis->m_ArenaInfo.State = ARENA_STATE_READY;
	}
	else
	{
		pTeam->SetTeamState(ARENA_STATE_JOINING);
		i_pFis->m_ArenaInfo.State = ARENA_STATE_READY;
		// 2007-06-05 by dhjin, 팀 참가 순위 시간 설정.
		pTeam->SetTeamJoinRankTime();
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CArenaManager::SendTeamChat(BYTE * i_pDATA, int i_nSize, int i_nTeamNum)
/// \brief		아레나 팀원에게 채팅을 전송한다.
/// \author		dhjin
/// \date		2007-05-02 ~ 2007-05-02
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CArenaManager::SendTeamChat(BYTE * i_pDATA, int i_nSize, int i_nTeamNum)
{
	//////////////////////////////////////////////////////////////////////////
	// 2007-05-02 by dhjin, 팀 찾기
	mt_auto_lock mta(&m_mtmapArenaTeamManager);
	CArenaTeam * pTeam = this->FindTeam(i_nTeamNum);
	if(NULL == pTeam)
	{
		return;
	}

	//////////////////////////////////////////////////////////////////////////
	// 2007-05-02 by dhjin, 팀원에게 채팅 전송
	vectorClientIndex * pClientIndex = pTeam->GetTeamList();
	vectorClientIndex::iterator itr = pClientIndex->begin();
	for(; itr != pClientIndex->end(); itr++)
	{
		CFieldIOCPSocket *pFISocket = this->m_pFieldIOCP16->GetFieldIOCPSocket(*itr);
		if(NULL == pFISocket
			|| FALSE == pFISocket->IsValidCharacter(FALSE)
			|| i_nTeamNum != pFISocket->m_ArenaInfo.TeamNum)
		{
//			// 2007-05-18 by cmkwon, 테스트 로그
//			g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ARENA] ErrorTeamMember SendTeamChat - TeamNum(%4d) TeamState(%30s) CharacterName(%30s)\r\n" 
//						, pFISocket->m_ArenaInfo.TeamNum, GET_ARENA_STATE_STRING(pFISocket->m_ArenaInfo.State), pFISocket->m_character.CharacterName);
//
			continue;
		}
		pFISocket->SendAddData(i_pDATA, i_nSize);
	}		

}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CArenaManager::GetPayInfo(BYTE i_nTeamMode, BYTE i_nTeamSize, SARENA_PAY_INFO * o_sPayInfo)
/// \brief		생성 될 아레나 방 보상을 얻어온다.
/// \author		dhjin
/// \date		2007-04-25 ~ 2007-04-25
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CArenaManager::GetPayInfo(BYTE i_nTeamMode, BYTE i_nTeamSize, SARENA_PAY_INFO * o_sPayInfo)
{
	vectorSDBARENA_INFO::iterator itr = m_vectorDBArenaInfo.begin();
	for(; itr != m_vectorDBArenaInfo.end(); itr++)
	{
		if(i_nTeamMode == itr->ArenaMode
			&& i_nTeamSize == itr->ReqMemberNum)
		{
			o_sPayInfo->PayInfluencePointWIN	= itr->PayInfluencePointWIN;
			o_sPayInfo->PayInfluencePointLOSE	= itr->PayInfluencePointLOSE;
			o_sPayInfo->PayWarPointWIN			= itr->PayWarPointWIN;
			o_sPayInfo->PayWarPointLOSE			= itr->PayWarPointLOSE;
			//////////////////////////////////////////////////////////////////////////////
			// 2012-05-23 by jhseol, 아레나 추가개발 - 최소 보상 워포 필드 추가
			o_sPayInfo->DefaultPayWarPointWIN	= itr->DefaultPayWarPointWIN;
			o_sPayInfo->DefaultPayWarPointLOSE	= itr->DefaultPayWarPointLOSE;
			// end 2012-05-23 by jhseol, 아레나 추가개발 - 최소 보상 워포 필드 추가

			return TRUE;
		}
	}

	return FALSE;	
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CArenaManager::GetDeathMatchDBInfo(BYTE i_nTeamMode, BYTE i_nTeamSize, BYTE * o_nDeathMatchCount, BYTE * o_nPlayLimitedTime)
/// \brief		생성 될 아레나 데스매치 모드시 처음 데스매치 카운트와 경기 제한 시간을 얻어낸다
/// \author		dhjin
/// \date		2007-04-25 ~ 2007-04-25
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CArenaManager::GetDeathMatchDBInfo(BYTE i_nTeamMode, BYTE i_nTeamSize, BYTE * o_nDeathMatchCount, BYTE * o_nPlayLimitedTime)
{
	if(ARENAMODE_DEATHMATCH != i_nTeamMode)
	{
		return FALSE;
	}

	vectorSDBARENA_MAPINFO::iterator itr = m_vectorDBArenaMapInfo.begin();
	for(; itr != m_vectorDBArenaMapInfo.end(); itr++)
	{
		if(i_nTeamMode == itr->ArenaMode
			&& i_nTeamSize == itr->ReqMemberNum)
		{
			*o_nDeathMatchCount = itr->WINCondition;
			*o_nPlayLimitedTime	= itr->PlayLimitedTime;
			return TRUE;
		}
	}

	return FALSE;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			void CArenaManager::DeleteDeathMatchRoom(INT i_nDeathMatchRoomNum)
/// \brief		아레나 데스매치 방 삭제 
/// \author		dhjin
/// \date		2007-04-25 ~ 2007-04-25
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CArenaManager::DeleteDeathMatchRoom(INT i_nDeathMatchRoomNum)
{
	MAP_CHANNEL_INDEX	tmMapInfo;

	//////////////////////////////////////////////////////////////////////////
	// 2007-04-20 by dhjin, 방 삭제
	if(!this->DeleteDeathMatchRoomByRoomNum(i_nDeathMatchRoomNum, &tmMapInfo))
	{
		return;
	}

	//////////////////////////////////////////////////////////////////////////
	// 2007-04-24 by dhjin, 맵관리에서 삭제
	if(!this->DeleteArenaMapManager(&tmMapInfo))
	{
		return;
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CArenaManager::DeleteDeathMatchRoomByRoomNum(INT i_nDeathMatchRoomNum, MAP_CHANNEL_INDEX * o_pMapInfo)
/// \brief		DeleteDeathMatchRoom()에서 호출, 아레나 데스매치 모드 방 삭제 후 방에 MapIndex리턴.
/// \author		dhjin
/// \date		2007-04-27 ~ 2007-04-27
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CArenaManager::DeleteDeathMatchRoomByRoomNum(INT i_nDeathMatchRoomNum, MAP_CHANNEL_INDEX * o_pMapInfo)
{
//	mt_auto_lock mtDeahMatch(&m_mtmapArenaDeathMatchRoom);
	
	mtmapINT2ArenaDeathMatchRoom::iterator itr = m_mtmapArenaDeathMatchRoom.find(i_nDeathMatchRoomNum);
	if(itr == m_mtmapArenaDeathMatchRoom.end())
	{
		return	FALSE;
	}
	o_pMapInfo = itr->second->GetMapInfo();

	itr = m_mtmapArenaDeathMatchRoom.erase(itr);
	
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CArenaManager::DeleteArenaMapManager(MAP_CHANNEL_INDEX * i_pMapInfo)
/// \brief		아레나 대전이 끝나면서 맵의 채널값을 FALSE로
/// \author		dhjin
/// \date		2007-04-27 ~ 2007-04-27
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CArenaManager::DeleteArenaMapManager(MAP_CHANNEL_INDEX * i_pMapInfo)
{
	mt_auto_lock mtMap(&m_mtvectArenaMapManager);
	mtvectSARENA_MAP_MANAGER::iterator itrMap = m_mtvectArenaMapManager.begin();
	for(; itrMap != m_mtvectArenaMapManager.end(); itrMap++)
	{
		if(i_pMapInfo->MapIndex == itrMap->ArenaMap
			&& i_pMapInfo->ChannelIndex == itrMap->ArenaMapChannel)
		{
//			// 2007-05-18 by cmkwon, 테스트 로그
//			g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ARENA] DeleteArenaMapManager - MapInfo(%d:%d) UseFlag(%d)\r\n" 
//				, itrMap->ArenaMap, itrMap->ArenaMapChannel, itrMap->bArenaChannelSet);

			itrMap->bArenaChannelSet	=	FALSE;
			return TRUE;
		}
	}
	
	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CArenaManager::DoMinutelyWorkArena(ATUM_DATE_TIME *pDateTime)
/// \brief		분마다 수행한다
/// \author		dhjin
/// \date		2007-04-27 ~ 2007-04-27
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CArenaManager::DoMinutelyWorkArena(ATUM_DATE_TIME *pDateTime)
{
	mt_auto_lock mtDeahMatch(&m_mtmapArenaDeathMatchRoom);
	CArenaDeathMatchRoom *pADMRoom = NULL;

//	DBGOUT("[ARENA] D : Before DoMinutelyWorkArena : DMROOM Total Count = %d\r\n" 
//			, m_mtmapArenaDeathMatchRoom.size());

	//////////////////////////////////////////////////////////////////////////
	// 2007-04-20 by dhjin, DeathMatchRoom분마다 수행 되어야 되는 일을 수행한다
	mtmapINT2ArenaDeathMatchRoom::iterator itrDM = m_mtmapArenaDeathMatchRoom.begin();
	for(; itrDM != m_mtmapArenaDeathMatchRoom.end(); itrDM++)
	{
		pADMRoom = itrDM->second;
//		DBGOUT("[ARENA] D : DoMinutelyWorkArena : DMROOM = %d\r\n" 
//			, itrDM->second->GetDeathMatchRoomNum());
		pADMRoom->DoMinutelyWorkArenaDeathMatchRoom(pDateTime);
	}
	
	//////////////////////////////////////////////////////////////////////////
	// 2007-05-07 by dhjin, 초기화 가능한 방은 초기화 한다.
	itrDM = m_mtmapArenaDeathMatchRoom.begin();
	while(itrDM != m_mtmapArenaDeathMatchRoom.end())
	{
		pADMRoom = itrDM->second;
		if(pADMRoom->InitDeathMatchRoomState())
		{
			if(!this->DeleteArenaMapManager(pADMRoom->GetMapInfo()))
			{
				itrDM++;
				continue;
			}

//			// 2007-05-18 by cmkwon, 테스트 로그
//			g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ARENA] DMRoomReset(5-1) - DRoomNum(%4d) AState(%30s) BTeamNum(%4d) ATeamNum(%4d) MapInfo(%s)\r\n" 
//				, pADMRoom->GetDeathMatchRoomNum(), GET_ARENA_STATE_STRING(pADMRoom->GetDeathMatchState()), pADMRoom->GetVCNTeamNum() , pADMRoom->GetANITeamNum(), GET_MAP_STRING(*(pADMRoom->GetMapInfo())));
		
			// 2008-04-11 by dhjin. 아레나 통합 - 관전자가 아레나 방 생성전에 들어올 경우 버그 발생으로 InitDeathMatchRoomState()함수로 위치 변경
			// 2008-02-20 by dhjin, 아레나 통합 - 맵에 유저 정보 초기화
			CFieldMapChannel *pInitFMChann = m_pFieldIOCP16->GetFieldMapChannelByMapChannelIndex(*pADMRoom->GetMapInfo());
			pInitFMChann->ResetUserMapChannel();
			pInitFMChann->InitmapCharacterUniqueNumberMapProject();

			pADMRoom->InitArenaDeathMatchRoom(m_pFieldIOCP16);
			itrDM = m_mtmapArenaDeathMatchRoom.erase(itrDM);
			continue;
		}
		itrDM++;
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CArenaManager::WarpToDeathMatchRoom(CFieldIOCPSocket * i_pFISocket)
/// \brief		데스매치 방으로 유저를 워프시킨다.
/// \author		dhjin
/// \date		2007-05-04 ~ 2007-05-04
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CArenaManager::WarpToDeathMatchRoom(CFieldIOCPSocket * i_pFISocket)
{
	mt_auto_lock mtDeahMatch(&m_mtmapArenaDeathMatchRoom);
	
	mtmapINT2ArenaDeathMatchRoom::iterator itr = m_mtmapArenaDeathMatchRoom.find(i_pFISocket->m_ArenaInfo.DeathMatchRoomNum);
	if(itr == m_mtmapArenaDeathMatchRoom.end())
	{
		return;
	}
	
	MAP_CHANNEL_INDEX tmMapInfo = *(itr->second->GetMapInfo());
	mtDeahMatch.auto_unlock_cancel();

	int	nWarpObjectIndex	= i_pFISocket->GetFieldMapChannel(&tmMapInfo)->GetWarpObjectIndexW(i_pFISocket->m_character.InfluenceType);
	
	//////////////////////////////////////////////////////////////////////////
	// 2007-05-10 by dhjin, 아레나 상점 이용 중이었다면 초기화 시킨다
	if(i_pFISocket->m_pCurrentBuildingNPC)
	{
		i_pFISocket->m_pCurrentBuildingNPC = NULL;
	}

	if (COMPARE_BODYCON_BIT(i_pFISocket->m_character.BodyCondition, BODYCON_DEAD_MASK))
	{
		i_pFISocket->CharacterDeadGameStartRoutine(TRUE, i_pFISocket->m_character.HP, i_pFISocket->m_character.DP);		
	}

	//////////////////////////////////////////////////////////////////////////
	// 2007-05-11 by dhjin, 아레나 맵으로 이동하기 전에 현재 HP, DP, SP, EP를 저장하고 만피 시킨 후 워프
	i_pFISocket->ArenaWarpSetHPDPSPEP();

	i_pFISocket->WarpToMap(tmMapInfo, nWarpObjectIndex);

	//////////////////////////////////////////////////////////////////////////
	// 2007-06-04 by dhjin, 아레나 전용 아이템을 지급한다.
	this->ArenaItemSupply(i_pFISocket, tmMapInfo.MapIndex);

}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CArenaManager::CheckFullMemberByFinishRoom(int i_nTeam)
/// \brief		아레나 대전 끝난후 팀의 상태를 변경하기 위해 팀원이 풀인지 아닌지 체크하여 상태값을 변경한다
/// \author		dhjin
/// \date		2007-05-04 ~ 2007-05-04
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CArenaManager::CheckFullMemberByFinishRoom(int i_nTeam)
{
	//////////////////////////////////////////////////////////////////////////
	// 2007-05-02 by dhjin, 유저 워프 시킨 후 유저의 팀 상태를 체크하여 유저 및 팀의 상태를 변경한다
	mt_auto_lock mtTeam(&m_mtmapArenaTeamManager);
	CArenaTeam * pTeam = this->FindTeam(i_nTeam);
	if(NULL == pTeam)
	{
		return;
	}

	if(pTeam->CheckFullMember())
	{
		pTeam->SetTeamState(ARENA_STATE_READY);
	}
	else
	{
		pTeam->SetTeamState(ARENA_STATE_JOINING);
		// 2007-06-05 by dhjin, 팀 참가 순위 시간 설정.
		pTeam->SetTeamJoinRankTime();
	}
}	

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CArenaManager::SendTeamMatchIngState(int i_nTeamNum)
/// \brief		아레나 팀 매칭 상태를 유저에게 전송한다.
/// \author		dhjin
/// \date		2007-05-07 ~ 2007-05-07
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CArenaManager::SendTeamMatchIngState(int i_nTeamNum)
{
	mt_auto_lock mta(&m_mtmapArenaTeamManager);
	//////////////////////////////////////////////////////////////////////////
	// 2007-04-23 by dhjin, 팀 리스트 얻어오기
	vectorClientIndex *pvectClient = this->GetMemberList(i_nTeamNum);
	if(NULL == pvectClient)
	{
		return;
	}
	vectorClientIndex tmvectClient = *pvectClient;
	mta.auto_unlock_cancel();

	//////////////////////////////////////////////////////////////////////////
	// 2007-04-23 by dhjin, 팀원 모두에게 정보 전송
	vectorClientIndex::iterator itr = tmvectClient.begin();
	for(; itr != tmvectClient.end(); itr++)
	{
		CFieldIOCPSocket *pFISocket = this->m_pFieldIOCP16->GetFieldIOCPSocket(*itr);
		if(NULL == pFISocket
			|| FALSE == pFISocket->IsValidCharacter(FALSE)
			|| i_nTeamNum != pFISocket->m_ArenaInfo.TeamNum)
		{
//			// 2007-05-18 by cmkwon, 테스트 로그
//			g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ARENA] ErrorTeamMember SendTeamMatchIngState - TeamNum(%4d) TeamState(%30s) CharacterName(%30s)\r\n" 
//						, pFISocket->m_ArenaInfo.TeamNum, GET_ARENA_STATE_STRING(pFISocket->m_ArenaInfo.State), pFISocket->m_character.CharacterName);
			continue;
		}
		pFISocket->SendAddMessageType(T_FC_ARENA_TEAM_MATCHING);
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			CArenaTeam * CArenaManager::GetArenaTeam(int i_nTeamNum)
/// \brief		팀 번호로 m_pArenaTeamManager의 팀을 가져온다.
/// \author		dhjin
/// \date		2007-05-08 ~ 2007-05-08
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
CArenaTeam * CArenaManager::GetArenaTeam(int i_nTeamNum)						
{
	return &m_pArenaTeamManager[i_nTeamNum];
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			CArenaDeathMatchRoom * CArenaManager::GetArenaDeathMatchRoom(int i_nRoomNum)
/// \brief		방 번호로 m_pArenaDeathMatchRoom의 방을 가져온다.
/// \author		dhjin
/// \date		2007-05-08 ~ 2007-05-08
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
CArenaDeathMatchRoom * CArenaManager::GetArenaDeathMatchRoom(int i_nRoomNum)
{
	return &m_pArenaDeathMatchRoom[i_nRoomNum];
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CArenaManager::CheckTeamFullMember(int i_nTeamNum)
/// \brief		팀 구성원이 풀인지 체크한다.
/// \author		dhjin
/// \date		2007-05-10 ~ 2007-05-10
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CArenaManager::CheckTeamFullMember(int i_nTeamNum)
{
	mt_auto_lock mtTeam(&m_mtmapArenaTeamManager);
	CArenaTeam * pTeam = this->FindTeam(i_nTeamNum);
	if(NULL == pTeam)
	{
		return FALSE;
	}

	if(pTeam->CheckFullMember())
	{
		return TRUE;
	}	

	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BYTE CArenaManager::GetTeamSize(int i_nTeamNum)
/// \brief		팀 최대 인원 가져오기.
/// \author		dhjin
/// \date		2007-05-17 ~ 2007-05-17
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BYTE CArenaManager::GetTeamSize(int i_nTeamNum)
{
	mt_auto_lock mta(&m_mtmapArenaTeamManager);
	CArenaTeam * pTeam = this->FindTeam(i_nTeamNum);
	if(NULL == pTeam)
	{
		return FALSE;
	}
	
	return pTeam->GetTeamSize();	
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CArenaManager::SendOtherTeamInfo(CFieldIOCPSocket * i_pFISoc)
/// \brief		상대 팀 정보를 전송한다.
/// \author		dhjin
/// \date		2007-05-23 ~ 2007-05-23
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CArenaManager::SendOtherTeamInfo(CFieldIOCPSocket * i_pFISoc)
{
	mt_auto_lock mta(&m_mtmapArenaTeamManager);
	//////////////////////////////////////////////////////////////////////////
	// 2007-05-11 by dhjin, Team관련 복사 본을 만들어 정렬을 한다.
	vectSARENA_OTHER_TEAM_INFO			vectCopyArenaTeamManager;
	vectCopyArenaTeamManager.clear();
	mtmapINT2ArenaTeamManager::iterator itrtmTeam = m_mtmapArenaTeamManager.begin();
	for(; itrtmTeam != m_mtmapArenaTeamManager.end(); itrtmTeam++)
	{
//		if(i_pFISoc->m_character.InfluenceType != itrtmTeam->second->GetTeamInfl()		// 서로 다른 세력인지 체크
		//////////////////////////////////////////////////////////////////////////
		// 2008-03-07 by dhjin, 아레나 통합 - 대회 서버인지 체크
	/*	if(JAMBOREE_SERVER_ID == i_pFISoc->m_AfsNeedMfsInfo.MFSServerID
			|| JAMBOREE_SERVER_ID == itrtmTeam->second->GetTeamServerID()
			)
		{
			if(JAMBOREE_SERVER_ID != i_pFISoc->m_AfsNeedMfsInfo.MFSServerID
				|| JAMBOREE_SERVER_ID != itrtmTeam->second->GetTeamServerID()
				)
			{
				continue;
			}
		}*/
		
		if(FALSE == (i_pFISoc->m_character.InfluenceType == itrtmTeam->second->GetTeamInfl()		
					 )//&& i_pFISoc->m_AfsNeedMfsInfo.MFSServerID == itrtmTeam->second->GetTeamServerID() )	// 2008-01-03 by dhjin, 아레나 통합 - 다른 팀인지 체크
			&& ARENA_STATE_READY_FINISH	== itrtmTeam->second->GetTeamState()			// 팀 상태가 ARENA_STATE_READY_FINISH이어야 된다.
			&& (i_pFISoc->m_character.Level >= itrtmTeam->second->GetTeamStartLevel()	// 레벨 등급이 같은지 체크
				&& i_pFISoc->m_character.Level <= itrtmTeam->second->GetTeamEndLevel() ) )
		{
			SARENA_OTHER_TEAM_INFO	tmSArenaOtherTeamInfo;
			util::zero(&tmSArenaOtherTeamInfo, sizeof(SARENA_OTHER_TEAM_INFO));
			tmSArenaOtherTeamInfo.ArenaMode	= itrtmTeam->second->GetTeamMode();
			tmSArenaOtherTeamInfo.TeamSize	= itrtmTeam->second->GetTeamSize();
			vectCopyArenaTeamManager.push_back(tmSArenaOtherTeamInfo);
		}
	}
	mta.auto_unlock_cancel();

	//////////////////////////////////////////////////////////////////////////
	// 2007-04-23 by dhjin, 팀이 존재하는지 체크
	if(vectCopyArenaTeamManager.empty())
	{
		i_pFISoc->SendErrorMessage(T_FC_ARENA_REQUEST_OTHER_TEAM_INFO, ERR_ARENA_NO_SUCH_TEAM);
		return;
	}
	
	//////////////////////////////////////////////////////////////////////////
	// 2007-05-25 by dhjin, Arena DeathMatch모드 메세지 만들기
	INIT_MSG_WITH_BUFFER(MSG_FC_ARENA_REQUEST_OTHER_TEAM_INFO_OK, T_FC_ARENA_REQUEST_OTHER_TEAM_INFO_OK, pSArenaOtherTeamInfo, SendBuf);
	pSArenaOtherTeamInfo->ArenaMode					= ARENAMODE_DEATHMATCH;
	pSArenaOtherTeamInfo->ArenaOtherTeamListCount	= this->MakeMSG_FC_ARENA_REQUEST_OTHER_TEAM_INFO_OK((SARENA_REQUEST_OTHER_TEAM_INFO_OK*)(SendBuf+MSG_SIZE(MSG_FC_ARENA_REQUEST_OTHER_TEAM_INFO_OK)), &vectCopyArenaTeamManager, ARENAMODE_DEATHMATCH);
	if(0 >= pSArenaOtherTeamInfo->ArenaOtherTeamListCount)
	{
		return;
	}
	i_pFISoc->SendAddData(SendBuf, MSG_SIZE(MSG_FC_ARENA_REQUEST_OTHER_TEAM_INFO_OK)+pSArenaOtherTeamInfo->ArenaOtherTeamListCount*sizeof(SARENA_REQUEST_OTHER_TEAM_INFO_OK));

}

///////////////////////////////////////////////////////////////////////////////
/// \fn			INT	CArenaManager::MakeMSG_FC_ARENA_REQUEST_OTHER_TEAM_INFO_OK(SARENA_REQUEST_OTHER_TEAM_INFO_OK * o_pArenaOtherTeamInfo, vectSARENA_OTHER_TEAM_INFO *i_pvectTeam, BYTE i_byArenaMode)
/// \brief		상대 팀 검색
/// \author		dhjin
/// \date		2007-05-25 ~ 2007-05-25
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
INT	CArenaManager::MakeMSG_FC_ARENA_REQUEST_OTHER_TEAM_INFO_OK(SARENA_REQUEST_OTHER_TEAM_INFO_OK * o_pArenaOtherTeamInfo
																, vectSARENA_OTHER_TEAM_INFO *i_pvectTeam, BYTE i_byArenaMode)
{
	INT	nTeamListCount = 0;

	vectSARENA_OTHER_TEAM_INFO::iterator itr = i_pvectTeam->begin();
	for(; itr != i_pvectTeam->end(); itr++)
	{
		if(i_byArenaMode != itr->ArenaMode)
		{// 2007-05-25 by dhjin, 팀 모드가 틀리면 
			continue;
		}
		
		BYTE tmTeamSize =  itr->TeamSize; // 2007-05-25 by dhjin, 팀인원 최대 수를 저장하고 메세지에 없으면 추가하고 있으면 카운트를 증가시킨다.
		BOOL CheckTeamCountIncrease = FALSE;

		for(int i =0; i < nTeamListCount; i++)
		{
			if(tmTeamSize == o_pArenaOtherTeamInfo[i].MemberCount)
			{// 2007-05-25 by dhjin, 팀이 있으면 증가 
				o_pArenaOtherTeamInfo[i].TeamCount++;
				CheckTeamCountIncrease = TRUE;
				break;
			}
		}

		if(!CheckTeamCountIncrease)
		{// 2007-05-25 by dhjin, 팀 최대인원을 만족하는 팀이 없으면 추가하고 증가.
			o_pArenaOtherTeamInfo[nTeamListCount].MemberCount	= tmTeamSize;
			o_pArenaOtherTeamInfo[nTeamListCount].TeamCount++;
			nTeamListCount++;
		}
	}

	return nTeamListCount;	
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CArenaManager::SendArenaTeamInfoByGMCommand(CFieldIOCPSocket * i_pFISoc, BYTE i_byInfl)
/// \brief		GM명령어 /아레나
/// \author		dhjin
/// \date		2007-05-23 ~ 2007-05-23
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CArenaManager::SendArenaTeamInfoByGMCommand(CFieldIOCPSocket * i_pFISoc, BYTE i_byInfl)
{
	mt_auto_lock mta(&m_mtmapArenaTeamManager);
	//////////////////////////////////////////////////////////////////////////
	// 2007-05-11 by dhjin, Team관련 복사 본을 만들어 정렬을 한다.
	vectSARENA_GM_COMMAND_INFO_TEAM			vectCopyArenaTeamManager;
	vectCopyArenaTeamManager.clear();
	mtmapINT2ArenaTeamManager::iterator itrtmTeam = m_mtmapArenaTeamManager.begin();
	for(; itrtmTeam != m_mtmapArenaTeamManager.end(); itrtmTeam++)
	{
		if(i_byInfl == itrtmTeam->second->GetTeamInfl())		// 같은 세력인지 체크
		{
			SARENA_GM_COMMAND_INFO_TEAM		tmArenaGMCommandInfoTeam;
			util::zero(&tmArenaGMCommandInfoTeam, sizeof(SARENA_GM_COMMAND_INFO_TEAM));
			tmArenaGMCommandInfoTeam.ArenaMode			= itrtmTeam->second->GetTeamMode();
			tmArenaGMCommandInfoTeam.TeamState			= itrtmTeam->second->GetTeamState();
			tmArenaGMCommandInfoTeam.CurrenMemberCount	= itrtmTeam->second->GetCurrentTeamSize();
			tmArenaGMCommandInfoTeam.Level				= itrtmTeam->second->GetTeamStartLevel();
			tmArenaGMCommandInfoTeam.MemberCount		= itrtmTeam->second->GetTeamSize();
			vectCopyArenaTeamManager.push_back(tmArenaGMCommandInfoTeam);
		}
	}
	mta.auto_unlock_cancel();

	//////////////////////////////////////////////////////////////////////////
	// 2007-04-23 by dhjin, 팀이 존재하는지 체크
	if(vectCopyArenaTeamManager.empty())
	{
		i_pFISoc->SendString128(STRING_128_USER_NOTICE, STRMSG_CS_STRING_ARENA_NOT_SEARCH);
		return;
	}
	
	vector<SARENA_GM_COMMAND_INFO>		vectGMCommandInfo;
	vectGMCommandInfo.clear();
	INT	nTeamListCount = 0;
	INT	ReadyUserTotalCount = 0;
	INT	MatchIngUserTotalCount = 0;
	INT	ArenaFightIngUserTotalCount = 0;

	//////////////////////////////////////////////////////////////////////////
	// 2007-05-25 by dhjin, 출력에 필요한 정보를 만든다.
	vectSARENA_GM_COMMAND_INFO_TEAM::iterator itr = vectCopyArenaTeamManager.begin();
	for(; itr != vectCopyArenaTeamManager.end(); itr++)
	{
		BYTE tmTeamMode =  itr->ArenaMode; // 2007-05-25 by dhjin, 팀 모드를 저장하고 메세지에 없으면 추가하고 있으면 카운트를 증가시킨다.
		BYTE tmTeamStartLevel =  itr->Level; // 2007-05-25 by dhjin, 팀 시작 레벨 저장하고 메세지에 없으면 추가하고 있으면 카운트를 증가시킨다.
		BYTE tmTeamSize =  itr->MemberCount; // 2007-05-25 by dhjin, 팀인원 최대 수를 저장하고 메세지에 없으면 추가하고 있으면 카운트를 증가시킨다.
		BOOL CheckTeamCountIncrease = FALSE;

		vector<SARENA_GM_COMMAND_INFO>::iterator itrGMInfo = vectGMCommandInfo.begin();
		for(; itrGMInfo != vectGMCommandInfo.end(); itrGMInfo++)
		{
			if(tmTeamMode == (*itrGMInfo).ArenaMode
				&& tmTeamSize == (*itrGMInfo).MemberCount
				&& tmTeamStartLevel == (*itrGMInfo).Level)
			{// 2007-05-25 by dhjin, 팀이 있으면 증가 
				(*itrGMInfo).TeamCount++;
				this->SetMSGArenaUseUser(itr->TeamState, itr->CurrenMemberCount, &ReadyUserTotalCount, &MatchIngUserTotalCount, &ArenaFightIngUserTotalCount);
				CheckTeamCountIncrease = TRUE;
				break;
			}
		}

		if(!CheckTeamCountIncrease)
		{// 2007-05-25 by dhjin, 팀 최대인원을 만족하는 팀이 없으면 추가하고 증가.
			SARENA_GM_COMMAND_INFO	tmArenaGMInfo;
			util::zero(&tmArenaGMInfo, sizeof(SARENA_GM_COMMAND_INFO));
			tmArenaGMInfo.ArenaMode		= tmTeamMode;
			tmArenaGMInfo.Level			= tmTeamStartLevel;
			tmArenaGMInfo.MemberCount	= tmTeamSize;
			tmArenaGMInfo.TeamCount++;
			vectGMCommandInfo.push_back(tmArenaGMInfo);
			this->SetMSGArenaUseUser(itr->TeamState, itr->CurrenMemberCount, &ReadyUserTotalCount, &MatchIngUserTotalCount, &ArenaFightIngUserTotalCount);
			nTeamListCount++;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// 2007-05-28 by dhjin, 전송
	BYTE SendBuf[SIZE_MAX_PACKET];
	int			nSendBytes		= 0;
	*(MessageType_t*)(SendBuf)	= T_FC_ARENA_GM_COMMAND_INFO_HEADER;
	nSendBytes					= SIZE_FIELD_TYPE_HEADER;
		
	for (int i=0; i < vectGMCommandInfo.size(); i++)
	{
		if(nSendBytes + MSG_SIZE(MSG_FC_ARENA_GM_COMMAND_INFO) > SIZE_MAX_PACKET)
		{
			i_pFISoc->SendAddData(SendBuf, nSendBytes);
			nSendBytes					= 0;
		}

		*((MessageType_t*)(SendBuf + nSendBytes))	= T_FC_ARENA_GM_COMMAND_INFO;
		nSendBytes									+= SIZE_FIELD_TYPE_HEADER;
		MSG_FC_ARENA_GM_COMMAND_INFO *pRMsg			= (MSG_FC_ARENA_GM_COMMAND_INFO*)(SendBuf + nSendBytes);
		*pRMsg										= vectGMCommandInfo[i];
		nSendBytes									+= sizeof(MSG_FC_ARENA_GM_COMMAND_INFO);
	}

	if(nSendBytes > 0)
	{
		i_pFISoc->SendAddData(SendBuf, nSendBytes);
		nSendBytes						= 0;
	}

	INIT_MSG(MSG_FC_ARENA_GM_COMMAND_INFO_DONE, T_FC_ARENA_GM_COMMAND_INFO_DONE, pArenaGMCommandInfoDone, SendBuf);
	pArenaGMCommandInfoDone->ReadyUserTotalCount			= ReadyUserTotalCount;
	pArenaGMCommandInfoDone->MatchIngUserTotalCount			= MatchIngUserTotalCount;
	pArenaGMCommandInfoDone->ArenaFightIngUserTotalCount	= ArenaFightIngUserTotalCount;
	i_pFISoc->SendAddData(SendBuf, MSG_SIZE(MSG_FC_ARENA_GM_COMMAND_INFO_DONE));
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CArenaManager::SetMSGArenaUseUser(BYTE i_byArenaState, BYTE i_byMemberCount, INT * o_ReadyUserTotalCount, INT * o_MatchIngUserTotalCount, INT * o_ArenaFightIngUserTotalCount)
/// \brief		아레나 이용하는 총 유저수를 구한다.
/// \author		dhjin
/// \date		2007-05-25 ~ 2007-05-25
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CArenaManager::SetMSGArenaUseUser(BYTE i_byArenaState, BYTE i_byMemberCount, INT * o_ReadyUserTotalCount
										, INT * o_MatchIngUserTotalCount, INT * o_ArenaFightIngUserTotalCount)
{
	if(ARENA_STATE_NONE == i_byArenaState)
	{
		return;
	}
	else if(ARENA_STATE_JOINING == i_byArenaState
			|| ARENA_STATE_READY == i_byArenaState)
	{
		*o_ReadyUserTotalCount += i_byMemberCount;
	}
	else if(ARENA_STATE_READY_FINISH == i_byArenaState)
	{
		*o_MatchIngUserTotalCount += i_byMemberCount;
	}
	else if(ARENA_STATE_READY_FINISH < i_byArenaState)
	{
		*o_ArenaFightIngUserTotalCount += i_byMemberCount;
	}

}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CArenaManager::ArenaItemSupply(CFieldIOCPSocket * i_pFISoc, INT i_nMapIndex)
/// \brief		아레나 전용 아이템 지급 설정.
/// \author		dhjin
/// \date		2007-06-04 ~ 2007-06-04
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CArenaManager::ArenaItemSupply(CFieldIOCPSocket * i_pFISoc, INT i_nMapIndex)
{
	mt_auto_lock mta(&m_mtmapArenaTeamManager);
	CArenaTeam * pTeam = this->FindTeam(i_pFISoc->m_ArenaInfo.TeamNum);
	if(NULL == pTeam)
	{
		return;
	}
	BYTE	tmTeamMode = pTeam->GetTeamMode();
	BYTE	tmTeamSize = pTeam->GetTeamSize();
	BYTE	tmTeamStartLevel = pTeam->GetTeamStartLevel();
	mta.auto_unlock_cancel();

	//////////////////////////////////////////////////////////////////////////
	// 2007-06-04 by dhjin, 지급 될 아레나 전용 아이템을 검색한다.
	vectorSDBARENA_MAPINFO::iterator itr = m_vectorDBArenaMapInfo.begin();
	for(; itr != m_vectorDBArenaMapInfo.end(); itr++)
	{
		if(i_nMapIndex == itr->ArenaMapIndex
			&& tmTeamMode == itr->ArenaMode
			&& tmTeamSize == itr->ReqMemberNum)
		{// 2007-06-04 by dhjin, 지급 될 아레나 아이템을 조건이 맞으면 아이템을 지급한다.
			if(itr->LvDSupplyItemNum1 != NULL)
			{
				// 2012-06-18 by jhseol, 아레나 추가개발part2 - 지급 : 아레나 시작 지급 아이템 DB에 있는 아이템으로 지급 하도록 수정
				// 2012-09-14 by jhseol, 아레나 추가개발 restore - 리뉴얼 적용 상태에서 자신의 케릭터로만 플레이 하도록 수정
#ifdef S_ARENA_EX_1ST_RESTORE_JHSEOL
				i_pFISoc->InsertArenaItem(this->GetTeamSupplyItemNum(itr->LvDSupplyItemNum1, tmTeamStartLevel), itr->LvDSupplyItemCount1);
#else
				i_pFISoc->InsertArenaItem(itr->LvDSupplyItemNum1, itr->LvDSupplyItemCount1);
#endif	//#ifdef S_ARENA_EX_1ST_RESTORE_JHSEOL
				// end 2012-09-14 by jhseol, 아레나 추가개발 restore - 리뉴얼 적용 상태에서 자신의 케릭터로만 플레이 하도록 수정
				// end2012-06-18 by jhseol, 아레나 추가개발part2 - 지급 : 아레나 시작 지급 아이템 DB에 있는 아이템으로 지급 하도록 수정
			}
			if(itr->LvDSupplyItemNum2 != NULL)
			{
				// 2012-06-18 by jhseol, 아레나 추가개발part2 - 지급 : 아레나 시작 지급 아이템 DB에 있는 아이템으로 지급 하도록 수정
				// 2012-09-14 by jhseol, 아레나 추가개발 restore - 리뉴얼 적용 상태에서 자신의 케릭터로만 플레이 하도록 수정
#ifdef S_ARENA_EX_1ST_RESTORE_JHSEOL
				i_pFISoc->InsertArenaItem(this->GetTeamSupplyItemNum(itr->LvDSupplyItemNum2, tmTeamStartLevel), itr->LvDSupplyItemCount2);
#else
				i_pFISoc->InsertArenaItem(itr->LvDSupplyItemNum2, itr->LvDSupplyItemCount2);
#endif	//#ifdef S_ARENA_EX_1ST_RESTORE_JHSEOL
				// end2012-06-18 by jhseol, 아레나 추가개발part2 - 지급 : 아레나 시작 지급 아이템 DB에 있는 아이템으로 지급 하도록 수정
			}
			if(itr->LvDSupplyItemNum3 != NULL)
			{
				// 2012-06-18 by jhseol, 아레나 추가개발part2 - 지급 : 아레나 시작 지급 아이템 DB에 있는 아이템으로 지급 하도록 수정
				//i_pFISoc->InsertArenaItem(MONEY_ITEM_NUMBER, itr->LvDSupplyItemCount3);			// 2008-04-28 by dhjin, 아레나 통합 - 지급 아이템 3번은 SPI로 추가, 변경시 꼭 this->GetTeamSupplyItemNum(itr->LvDSupplyItemNum2, tmTeamStartLevel)로 돌린다.
				i_pFISoc->InsertArenaItem(itr->LvDSupplyItemNum3, itr->LvDSupplyItemCount3);
				// end2012-06-18 by jhseol, 아레나 추가개발part2 - 지급 : 아레나 시작 지급 아이템 DB에 있는 아이템으로 지급 하도록 수정
			}
			// 2012-09-14 by jhseol, 아레나 추가개발 part3 - 아레나 지급 아이템 추가 (아레나 전용 탄약 재충전 카드)
#ifdef _ARENA_ADDITIONAL_SUPPLY_ITEMS		// 2012-11-08 by jhseol, 아레나 지급 아이템 수정 #ifndef --> #ifdef 로 수정
			if(itr->LvDSupplyItemNum4 != NULL)
			{
				// 2012-06-18 by jhseol, 아레나 추가개발part2 - 지급 : 아레나 시작 지급 아이템 DB에 있는 아이템으로 지급 하도록 수정
				i_pFISoc->InsertArenaItem(itr->LvDSupplyItemNum4, itr->LvDSupplyItemCount4);
				// end2012-06-18 by jhseol, 아레나 추가개발part2 - 지급 : 아레나 시작 지급 아이템 DB에 있는 아이템으로 지급 하도록 수정
			}
			if (itr->LvDSupplyItemNum5 != NULL)
			{
				i_pFISoc->InsertArenaItem(itr->LvDSupplyItemNum5, itr->LvDSupplyItemCount5);
			}
#endif
			// end 2012-09-14 by jhseol, 아레나 추가개발 part3 - 아레나 지급 아이템 추가 (아레나 전용 탄약 재충전 카드)
			return;
		}
	}

}

///////////////////////////////////////////////////////////////////////////////
/// \fn			INT	CArenaManager::GetTeamSupplyItemNum(INT i_nitemNum, BYTE i_nTeamStartLevel)
/// \brief		팀 등급에 맞는 아레나 전용 아이템 번호로 설정한다.
/// \author		dhjin
/// \date		2007-06-04 ~ 2007-06-04
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
INT	CArenaManager::GetTeamSupplyItemNum(INT i_nitemNum, BYTE i_nTeamStartLevel)
{
	if(ARENA_STAGE_D_START_LEVEL == i_nTeamStartLevel)
	{
		return i_nitemNum;
	}
	else if(ARENA_STAGE_B_START_LEVEL == i_nTeamStartLevel)
	{
		return i_nitemNum+1;
	}
	else if(ARENA_STAGE_C_START_LEVEL == i_nTeamStartLevel)
	{
		return i_nitemNum+2;
	}
	else if(ARENA_STAGE_A_START_LEVEL == i_nTeamStartLevel)
	{
		return i_nitemNum+3;
	}

	return i_nitemNum;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CArenaManager::InsertWatchUserListW(CFieldIOCPSocket * i_pFISoc, MAP_CHANNEL_INDEX i_MapInfo)
/// \brief		맵 정보에 해당하는 아레나 방에 관전 유저를 추가 한다.
/// \author		dhjin
/// \date		2007-06-15 ~ 2007-06-15
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CArenaManager::InsertWatchUserListW(CFieldIOCPSocket * i_pFISoc, MAP_CHANNEL_INDEX i_MapInfo)
{
	mt_auto_lock mtDeahMatch(&m_mtmapArenaDeathMatchRoom);
	CArenaDeathMatchRoom *pDMRoom = NULL;	

	mtmapINT2ArenaDeathMatchRoom::iterator itr = m_mtmapArenaDeathMatchRoom.begin();
	for(; itr != m_mtmapArenaDeathMatchRoom.end(); itr++)
	{
		pDMRoom = itr->second;
		if(i_MapInfo == *(pDMRoom->GetMapInfo())
			&& (ARENA_STATE_FIGHTING_WARREADY == pDMRoom->GetDeathMatchState()
				|| ARENA_STATE_FIGHTING_WARING == pDMRoom->GetDeathMatchState())
			&& 12 >= pDMRoom->GetANITeamSize())
		{// 2007-06-15 by dhjin, 아레나 진행 중인 맵이면 관전 유저를 추가한다.
			pDMRoom->InsertWatchUserList(i_pFISoc->m_character.ClientIndex);
			i_pFISoc->m_ArenaInfo.DeathMatchRoomNum = pDMRoom->GetDeathMatchRoomNum();
			i_pFISoc->m_ArenaInfo.ArenaMode			= ARENAMODE_DEATHMATCH;
			i_pFISoc->m_ArenaInfo.WatchState		= TRUE;
			i_pFISoc->m_ArenaInfo.State				= pDMRoom->GetDeathMatchState();
			return TRUE;
		}
	}
		
	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CArenaManager::DeleteWatchUserListW(CFieldIOCPSocket * i_pFISoc)
/// \brief		맵 정보에 해당하는 아레나 방에 관전 유저를 삭제 한다.
/// \author		dhjin
/// \date		2007-06-15 ~ 2007-06-15
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CArenaManager::DeleteWatchUserListW(CFieldIOCPSocket * i_pFISoc)
{
	mt_auto_lock mtDeahMatch(&m_mtmapArenaDeathMatchRoom);
	
	mtmapINT2ArenaDeathMatchRoom::iterator itr = m_mtmapArenaDeathMatchRoom.find(i_pFISoc->m_ArenaInfo.DeathMatchRoomNum);
	if(itr == m_mtmapArenaDeathMatchRoom.end())
	{
		return;
	}
	
	itr->second->DeleteWatchUserList(i_pFISoc->m_character.ClientIndex);
	i_pFISoc->m_ArenaInfo.ResetArenaInfo();
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CArenaManager::SendWatchUserATRoomInfo(CFieldIOCPSocket * i_pFISoc, MAP_CHANNEL_INDEX i_MapInfo)
/// \brief		아레나 맵에 있는 유저의 정보를 관전하는 유저에게 전송한다.
/// \author		dhjin
/// \date		2007-06-18 ~ 2007-06-18
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CArenaManager::SendWatchUserATRoomInfo(CFieldIOCPSocket * i_pFISoc, MAP_CHANNEL_INDEX i_MapInfo)
{
	mt_auto_lock mtDeahMatch(&m_mtmapArenaDeathMatchRoom);
	CArenaDeathMatchRoom *pDMRoom = NULL;	

	mtmapINT2ArenaDeathMatchRoom::iterator itr = m_mtmapArenaDeathMatchRoom.begin();
	for(; itr != m_mtmapArenaDeathMatchRoom.end(); itr++)
	{
		pDMRoom = itr->second;
		if(i_MapInfo == *(pDMRoom->GetMapInfo())
			&& ARENA_STATE_FIGHTING_WARING == pDMRoom->GetDeathMatchState()
			&& 12 >= pDMRoom->GetANITeamSize())
		{// 2007-06-18 by dhjin, 아레나 맵에 있는 유저의 정보를 관전하는 유저에게 전송한다.
			pDMRoom->SendWatchStartInfo();
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			MAP_CHANNEL_INDEX * CArenaManager::GetArenaDeathMatchRoomMapInfo(int i_nRoomNum)
/// \brief		아레나 통합 - 데스매치 방 맵 정보 얻어오기
/// \author		dhjin
/// \date		2008-01-09 ~ 2008-01-09
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
MAP_CHANNEL_INDEX * CArenaManager::GetArenaDeathMatchRoomMapInfo(int i_nRoomNum)
{
	mt_auto_lock mtDeahMatch(&m_mtmapArenaDeathMatchRoom);

	CArenaDeathMatchRoom * tmArenaDMRoom = this->GetArenaDeathMatchRoom(i_nRoomNum);

	if(NULL == tmArenaDMRoom)
	{
		return NULL;
	}

	return tmArenaDMRoom->GetMapInfo();
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BYTE CArenaManager::GetTeamArenaInflW(INT i_nTeamNum)
/// \brief		아레나 통합 - 팀 아레나 시합 동안의 세력 가져오기
/// \author		dhjin
/// \date		2008-01-16 ~ 2008-01-16
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BYTE CArenaManager::GetTeamArenaInflW(INT i_nTeamNum)
{
	mt_auto_lock mta(&m_mtmapArenaTeamManager);
	CArenaTeam * pTeam = this->FindTeam(i_nTeamNum);
	if(NULL == pTeam)
	{
		return FALSE;
	}
	
	return pTeam->GetTeamArenaInfl();
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BYTE CArenaManager::GetTeamInflW(INT i_nTeamNum)
/// \brief		아레나 통합 - 팀 세력 가져오기
/// \author		dhjin
/// \date		2008-01-16 ~ 2008-01-16
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BYTE CArenaManager::GetTeamInflW(INT i_nTeamNum)
{
	mt_auto_lock mta(&m_mtmapArenaTeamManager);
	CArenaTeam * pTeam = this->FindTeam(i_nTeamNum);
	if(NULL == pTeam)
	{
		return FALSE;
	}
	
	return pTeam->GetTeamInfl();
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CArenaManager::SetTeamEventW(INT i_nTeamNum, BOOL i_bSet)
/// \brief		아레나 통합 - 이벤트 속성 할당하기
/// \author		dhjin
/// \date		2008-03-10 ~ 2008-03-10
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CArenaManager::SetTeamEventW(INT i_nTeamNum, BOOL i_bSet)
{
	mt_auto_lock mta(&m_mtmapArenaTeamManager);
	CArenaTeam * pTeam = this->FindTeam(i_nTeamNum);
	if(NULL == pTeam)
	{
		return FALSE;
	}	

	if(ARENA_STATE_READY_FINISH <=  pTeam->GetTeamState())
	{
		return FALSE;
	}

	pTeam->SetTeamEvent(i_bSet);

	return TRUE;
}