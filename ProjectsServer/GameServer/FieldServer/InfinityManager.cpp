// InfinityManager.cpp: implementation of the CInfinityManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "InfinityManager.h"
#include "FieldGlobal.h"
#include "InfinityTickManager.h"
#include "FieldIOCPSocket.h"			// 2010-04-05 by cmkwon, 인피 재입장 카드 관련 시스템 수정 - 

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CInfinityManager::CInfinityManager()
{
	InitializeCriticalSection(&m_criticalSectionCreate);	// 2009-09-09 ~ 2010-01 by dhjin, 인피니티 - 소스 체크
	this->InitInfinityManager();
}

CInfinityManager::~CInfinityManager()
{
	DeleteCriticalSection(&m_criticalSectionCreate);		// 2009-09-09 ~ 2010-01 by dhjin, 인피니티 - 소스 체크
	m_pTickManager->CleanThread();
	util::del(m_pTickManager);
}


void CInfinityManager::InitInfinityManager() 
{
	m_vectInfiModeInfo.clear();
	m_vectInfiMonsterInfo.clear();
	m_mtvectInfiBossRush.clear();
	m_mtvectInfiDefence.clear();
	m_mtvectInfiMShipBattle.clear();			// 2011-02-18 by hskim, 인피니티 3차 - 방 생성 작업
	m_nInfinityCreateUID = 1;
	m_mtInfinityDisConnectUserList.clearLock();		// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 팅긴 유저 재접속 처리
	m_pTickManager = new CInfinityTickManager(this);
}

void CInfinityManager::SetInfinityMapManagerW(CFieldMapWorkspace * i_pFieldMapWorkspace) {
	this->m_InfiMapManager.SetInfinityMapManager(i_pFieldMapWorkspace);
}	

BOOL CInfinityManager::SetDBManager(CAtumFieldDBManager * i_pAtumDBManager) {
	if(NULL == i_pAtumDBManager) {
		return FALSE;
	}
	m_pAtumDBManager = i_pAtumDBManager;

	return TRUE;
}

void CInfinityManager::SetCinemaInfoW(vectorCinemaInfo * i_pVectCinemaInfo) {
	this->m_Cinema.SetCinemaInfo(i_pVectCinemaInfo);
}

void CInfinityManager::SetRevisionInfoW(vectorRevisionInfo * i_pVectRevisionInfo) {
	this->m_Revision.SetRevisionInfo(i_pVectRevisionInfo);
}

void CInfinityManager::SetDBInfinityModeInfo(vectorInfinityModeInfo * i_pVectInfiModeInfo) {
	if(i_pVectInfiModeInfo) {
		m_vectInfiModeInfo.clear();
		m_vectInfiModeInfo.assign(i_pVectInfiModeInfo->begin(), i_pVectInfiModeInfo->end());	
		
		m_InfiMapManager.InitInfinityMapInfoList(&m_vectInfiModeInfo);
	}
}

void CInfinityManager::SetDBInfinityMonsterInfo(vectorInfinityMonsterInfo * i_pVectInfiMonsterInfo) {
	if(i_pVectInfiMonsterInfo) {
		m_vectInfiMonsterInfo.clear();
		m_vectInfiMonsterInfo.assign(i_pVectInfiMonsterInfo->begin(), i_pVectInfiMonsterInfo->end());
	}
}

void CInfinityManager::GetInfinityModeInfo(vectorInfinityModeInfo * o_pCopyVectInfiModeInfo) {
	if(o_pCopyVectInfiModeInfo) {
		o_pCopyVectInfiModeInfo->clear();
		o_pCopyVectInfiModeInfo->assign(m_vectInfiModeInfo.begin(), m_vectInfiModeInfo.end());
	}
}

INT CInfinityManager::MakeMsgInfinityPlayingList(INFINITY_READY_LIST * o_pInfinityPlayingList, MapIndex_t i_nInfinityMapIdx, eINFINITY_MODE i_nInfinityMode, BYTE i_byInfluenceType) 
{
	if( NULL == o_pInfinityPlayingList ) 
	{
		// 2009-09-09 ~ 2010-01 by dhjin, 인피니티 - 소스 체크
		return FALSE;
	}

	INT PlayingListCount = 0;

	switch (i_nInfinityMode) 
	{
		case INFINITY_MODE_BOSSRUSH: 
			{
				mt_auto_lock mta(&m_mtvectInfiBossRush);
				mtvectInfiBossRush::iterator itr = m_mtvectInfiBossRush.begin();

				for( ; itr != m_mtvectInfiBossRush.end(); itr++ ) 
				{
					if(SIZE_MAX_PACKET < MSG_SIZE(MSG_FC_INFINITY_READY_LIST_OK) + sizeof(INFINITY_READY_LIST) * (PlayingListCount+1)) 
					{
						// 2009-09-09 ~ 2010-01 by dhjin, 인피니티 - 소스 체크
						break;
					}

					if( i_nInfinityMapIdx == (*itr)->GetInfinityMapIndex() 
						&& INFINITY_STATE_UNPREPARED == (*itr)->GetInfinityState()
						&& i_byInfluenceType == (*itr)->GetInfluenceType() ) 
					{
						o_pInfinityPlayingList[PlayingListCount].InfinityCreateUID			= (*itr)->GetInfinityCreateUID();
						o_pInfinityPlayingList[PlayingListCount].PlayingRoomMemberCount		= (*itr)->GetPlayerListSize();
						o_pInfinityPlayingList[PlayingListCount].MaxMemberCount				= (*itr)->GetMaxPlayerSize();
						util::strncpy(o_pInfinityPlayingList[PlayingListCount].MasterName, (*itr)->GetMasterPlayerName(), SIZE_MAX_CHARACTER_NAME);
						util::strncpy(o_pInfinityPlayingList[PlayingListCount].InfinityTeamName, (*itr)->GetInfinityTeamName(), SIZE_MAX_PARTY_NAME);

						// 2010. 05. 19 by hsLee 인피니티 필드 2차 난이도 조절. (신호처리 + 몬스터 처리(서버) )
						o_pInfinityPlayingList[PlayingListCount].DifficultLevel				= (*itr)->GetDifficultyLevel();

						PlayingListCount++;
					}
				}

				mta.auto_unlock_cancel();
			} // case INFINITY_MODE_BOSSRUSH
			break; 

		case INFINITY_MODE_DEFENCE: 
			{
				mt_auto_lock mta(&m_mtvectInfiDefence);
				mtvectInfiDefence::iterator itr = m_mtvectInfiDefence.begin();

				for( ; itr != m_mtvectInfiDefence.end(); itr++ ) 
				{
					if( SIZE_MAX_PACKET < MSG_SIZE(MSG_FC_INFINITY_READY_LIST_OK) + sizeof(INFINITY_READY_LIST) * (PlayingListCount+1)) 
					{
						// 2009-09-09 ~ 2010-01 by dhjin, 인피니티 - 소스 체크
						break;
					}

					if( i_nInfinityMapIdx == (*itr)->GetInfinityMapIndex()
						&& INFINITY_STATE_UNPREPARED == (*itr)->GetInfinityState()
						&& i_byInfluenceType == (*itr)->GetInfluenceType() ) 
					{
						o_pInfinityPlayingList[PlayingListCount].InfinityCreateUID			= (*itr)->GetInfinityCreateUID();
						o_pInfinityPlayingList[PlayingListCount].PlayingRoomMemberCount		= (*itr)->GetPlayerListSize();
						o_pInfinityPlayingList[PlayingListCount].MaxMemberCount				= (*itr)->GetMaxPlayerSize();
						util::strncpy(o_pInfinityPlayingList[PlayingListCount].MasterName, (*itr)->GetMasterPlayerName(), SIZE_MAX_CHARACTER_NAME);
						util::strncpy(o_pInfinityPlayingList[PlayingListCount].InfinityTeamName, (*itr)->GetInfinityTeamName(), SIZE_MAX_PARTY_NAME);

						// 2010. 05. 19 by hsLee 인피니티 필드 2차 난이도 조절. (신호처리 + 몬스터 처리(서버) )
						o_pInfinityPlayingList[PlayingListCount].DifficultLevel				= (*itr)->GetDifficultyLevel();

						PlayingListCount++;
					}
				}

				mta.auto_unlock_cancel();
			} // case INFINITY_MODE_DEFENCE
			break; 	   

        // 2011-02-18 by hskim, 인피니티 3차 - 방 생성 작업 
		case INFINITY_MODE_MSHIPBATTLE:				
			{
				mt_auto_lock mta(&m_mtvectInfiMShipBattle);
				mtvectInfiMShipBattle::iterator itr = m_mtvectInfiMShipBattle.begin();
				
				for( ; itr != m_mtvectInfiMShipBattle.end(); itr++ ) 
				{
					if( SIZE_MAX_PACKET < MSG_SIZE(MSG_FC_INFINITY_READY_LIST_OK) + sizeof(INFINITY_READY_LIST) * (PlayingListCount+1)) 
					{
						break;
					}
					
					if( i_nInfinityMapIdx == (*itr)->GetInfinityMapIndex()
						&& INFINITY_STATE_UNPREPARED == (*itr)->GetInfinityState()
						&& i_byInfluenceType == (*itr)->GetInfluenceType() ) 
					{
						o_pInfinityPlayingList[PlayingListCount].InfinityCreateUID			= (*itr)->GetInfinityCreateUID();
						o_pInfinityPlayingList[PlayingListCount].PlayingRoomMemberCount		= (*itr)->GetPlayerListSize();
						o_pInfinityPlayingList[PlayingListCount].MaxMemberCount				= (*itr)->GetMaxPlayerSize();
						util::strncpy(o_pInfinityPlayingList[PlayingListCount].MasterName, (*itr)->GetMasterPlayerName(), SIZE_MAX_CHARACTER_NAME);
						util::strncpy(o_pInfinityPlayingList[PlayingListCount].InfinityTeamName, (*itr)->GetInfinityTeamName(), SIZE_MAX_PARTY_NAME);

						o_pInfinityPlayingList[PlayingListCount].DifficultLevel				= (*itr)->GetDifficultyLevel();
						
						PlayingListCount++;
					}
				}
				
				mta.auto_unlock_cancel();
			} // case INFINITY_MODE_MSHIPBATTLE
			break; 										   
       // end 2011-02-18 by hskim, 인피니티 3차 - 방 생성 작업
	}

	return PlayingListCount;
}

BOOL CInfinityManager::CheckInfinityModeLevel(InfiModeUID_t i_nInfinityModeUID, Lv_t i_Lv) {
	vectorInfinityModeInfo::iterator itr = m_vectInfiModeInfo.begin();
	for(; itr != m_vectInfiModeInfo.end(); itr++) {
		if(i_nInfinityModeUID == itr->InfinityModeUID) {
			if(i_Lv >= itr->MinLv && i_Lv <= itr->MaxLv) {
				return TRUE;
			}
			else {
				return FALSE;
			}
		}
	}
	return FALSE;
}
// 2010-03-23 by cmkwon, 인피니티 입장 캐쉬 아이템 구현 - 
BOOL CInfinityManager::CheckEntranceCount(InfiModeUID_t i_nInfinityModeUID, EntranceCount_t i_EntranceCount, int i_nAddLimiteCnt/*=0*/)
{
	vectorInfinityModeInfo::iterator itr = m_vectInfiModeInfo.begin();
	for(; itr != m_vectInfiModeInfo.end(); itr++) {
		if(i_nInfinityModeUID == itr->InfinityModeUID) {
// 2009-09-09 ~ 2010-01 by dhjin, 인피니티 - 소스 체크, 밑과 같이 수정
//			if(i_EntranceCount <= itr->EntranceCount) {
			if(i_EntranceCount < itr->EntranceCount + i_nAddLimiteCnt) // 2010-03-23 by cmkwon, 인피니티 입장 캐쉬 아이템 구현 - 추가 카운트
			{	
				return TRUE;
			}
			else 
			{
				return FALSE;
			}
		}
	}
	return FALSE;
}


// 2010. 05. 19 by hsLee 인피니티 필드 2차 난이도 조절. (신호처리 + 몬스터 처리(서버) )
/******************************************************************************************************************
**
**	인피니티 방 생성.
**
**	Create Info :	??. ??. ??
**
**	Update Info : 2010. 05. 28. by hsLee. - 인피니티 난이도 조절 관련, 방 생성시 넘겨 받을 방 난이도 인자 추가.
**
*******************************************************************************************************************/
//BOOL CInfinityManager::CreateInfinity(MSG_FC_INFINITY_CREATE * i_pInfinityCreateInfo, CFieldIOCPSocket * i_pFISoc, InfinityCreateUID_t * o_pCreateUID) 
BOOL CInfinityManager::CreateInfinity(MSG_FC_INFINITY_CREATE * i_pInfinityCreateInfo, CFieldIOCPSocket * i_pFISoc, InfinityCreateUID_t * o_pCreateUID , INT * o_nInfinityDifficultyLevel ) 
// End 2010. 05. 19 by hsLee 인피니티 필드 2차 난이도 조절. (신호처리 + 몬스터 처리(서버) )
{
	if(NULL == i_pInfinityCreateInfo
		|| NULL == i_pFISoc
		|| NULL == o_pCreateUID 

		// 2010. 05. 19 by hsLee 인피니티 필드 2차 난이도 조절. (신호처리 + 몬스터 처리(서버) )
		|| NULL == o_nInfinityDifficultyLevel
		// End 2010. 05. 19 by hsLee 인피니티 필드 2차 난이도 조절. (신호처리 + 몬스터 처리(서버) )
		) 
	{
		// 2009-09-09 ~ 2010-01 by dhjin, 인피니티 - 소스 체크
		return FALSE;
	}

	BOOL CheckInvalidInputValue = FALSE;
	INFINITY_MODEINFO InfinityModeInfo;
	util::zero(&InfinityModeInfo, sizeof(INFINITY_MODEINFO));

	vectorInfinityModeInfo::iterator itr = m_vectInfiModeInfo.begin();
	for(; itr != m_vectInfiModeInfo.end(); itr++) 
	{
		// 유효한 입력 값인지 체크하고 생성하려고 하는 인피 모드 정보를 가져온다.
		if(itr->InfinityModeUID == i_pInfinityCreateInfo->InfinityModeUID
			&& itr->MapIdx == i_pInfinityCreateInfo->MapIndex
			&& itr->ModeTypeNum == i_pInfinityCreateInfo->InfinityMode)
		{
			InfinityModeInfo = *itr;
			CheckInvalidInputValue = TRUE;
			break;
		}
	}

	if(FALSE == CheckInvalidInputValue) 
	{
		return FALSE;
	}

	// Cinema, Revision 정보 여기서 만들기
	vectorCinemaInfo		CinemaInfoList;
	util::zero(&CinemaInfoList, sizeof(vectorCinemaInfo));
	
	EnterCriticalSection(&m_criticalSectionCreate);		// 2009-09-09 ~ 2010-01 by dhjin, 인피니티 - 소스 체크


	switch (i_pInfinityCreateInfo->InfinityMode) 
	{
		case INFINITY_MODE_BOSSRUSH:
			{
				g_pFieldGlobal->WriteSystemLogEX(TRUE, "[Notify] [Infinity][%I64d] CInfinityManager::CreateInfinity# BossRush Created !, MapIdx(%d) \r\n", m_nInfinityCreateUID);

				CInfinityBossrush * InfiBossRush = new CInfinityBossrush();
				InfiBossRush->SetInfinityCreateUID(m_nInfinityCreateUID);
				InfiBossRush->SetModeInfo(&InfinityModeInfo);
				InfiBossRush->SetCinemaInfo(this->m_Cinema.GetCinemaInfo(InfinityModeInfo.CinemaNum, &CinemaInfoList));
				InfiBossRush->SetInfinityTeamName(i_pInfinityCreateInfo->InfinityTeamName);
				InfiBossRush->InitMasterPlayer(i_pFISoc);

				// 2010. 05. 19 by hsLee 인피니티 필드 2차 난이도 조절. (신호처리 + 몬스터 처리(서버) ) - 방의 난이도 레벨 설정.
				InfiBossRush->SetDifficultyLevel ( i_pInfinityCreateInfo->InfinityDifficultyLevel );
				*o_nInfinityDifficultyLevel = InfiBossRush->GetDifficultyLevel();
				// End 2010. 05. 19 by hsLee 인피니티 필드 2차 난이도 조절. (신호처리 + 몬스터 처리(서버) ) - 방의 난이도 레벨 설정.

				mt_auto_lock mta(&m_mtvectInfiBossRush);
				m_mtvectInfiBossRush.push_back(InfiBossRush);
				mta.auto_unlock_cancel();		
			} // case INFINITY_MODE_BOSSRUSH
			break; 

		case INFINITY_MODE_DEFENCE:
			{
				g_pFieldGlobal->WriteSystemLogEX(TRUE, "[Notify] [Infinity][%I64d] CInfinityManager::CreateInfinity# Defence Created !, MapIdx(%d) \r\n", m_nInfinityCreateUID, InfinityModeInfo.MapIdx);

				CInfinityDefence * InfiDefence = new CInfinityDefence();
				InfiDefence->SetInfinityCreateUID(m_nInfinityCreateUID);
				InfiDefence->SetModeInfo(&InfinityModeInfo);
				InfiDefence->SetCinemaInfo(this->m_Cinema.GetCinemaInfo(InfinityModeInfo.CinemaNum, &CinemaInfoList));
				InfiDefence->SetInfinityTeamName(i_pInfinityCreateInfo->InfinityTeamName);
				InfiDefence->InitMasterPlayer(i_pFISoc);

				// 2010. 05. 19 by hsLee 인피니티 필드 2차 난이도 조절. (신호처리 + 몬스터 처리(서버) ) - 방의 난이도 레벨 설정.
				InfiDefence->SetDifficultyLevel ( i_pInfinityCreateInfo->InfinityDifficultyLevel );
				*o_nInfinityDifficultyLevel = InfiDefence->GetDifficultyLevel();
				// End 2010. 05. 19 by hsLee 인피니티 필드 2차 난이도 조절. (신호처리 + 몬스터 처리(서버) ) - 방의 난이도 레벨 설정.

				mt_auto_lock mta(&m_mtvectInfiDefence);
				m_mtvectInfiDefence.push_back(InfiDefence);
				mta.auto_unlock_cancel();
			} // case INFINITY_MODE_DEFENCE
			break;

        // 2011-02-18 by hskim, 인피니티 3차 - 방 생성 작업
		case INFINITY_MODE_MSHIPBATTLE:					
			{
#ifdef S_INFINITY3_HSKIM		// ON/OFF 기능 구현
#else
				LeaveCriticalSection(&m_criticalSectionCreate);
				return FALSE;
#endif

				g_pFieldGlobal->WriteSystemLogEX(TRUE, "[Notify] [Infinity][%I64d] CInfinityManager::CreateInfinity# MShipBattle Created !, MapIdx(%d) \r\n", m_nInfinityCreateUID, InfinityModeInfo.MapIdx);
				
				CInfinityMShipBattle * InfiMShipBattle = new CInfinityMShipBattle();
				InfiMShipBattle->SetInfinityCreateUID(m_nInfinityCreateUID);
				InfiMShipBattle->SetModeInfo(&InfinityModeInfo);
				InfiMShipBattle->SetCinemaInfo(this->m_Cinema.GetCinemaInfo(InfinityModeInfo.CinemaNum, &CinemaInfoList));
				InfiMShipBattle->SetInfinityTeamName(i_pInfinityCreateInfo->InfinityTeamName);
				InfiMShipBattle->InitMasterPlayer(i_pFISoc);
				
				InfiMShipBattle->SetDifficultyLevel ( i_pInfinityCreateInfo->InfinityDifficultyLevel );
				*o_nInfinityDifficultyLevel = InfiMShipBattle->GetDifficultyLevel();
				
				mt_auto_lock mta(&m_mtvectInfiMShipBattle);
				m_mtvectInfiMShipBattle.push_back(InfiMShipBattle);
				mta.auto_unlock_cancel();
			} // case INFINITY_MODE_DEFENCE
			break;
        // end 2011-02-18 by hskim, 인피니티 3차 - 방 생성 작업

		default : 
			{
				LeaveCriticalSection(&m_criticalSectionCreate);		// 2009-09-09 ~ 2010-01 by dhjin, 인피니티 - 소스 체크
				return FALSE;
			} // default
	}

	*o_pCreateUID = m_nInfinityCreateUID;
	m_nInfinityCreateUID++;

	LeaveCriticalSection(&m_criticalSectionCreate);		// 2009-09-09 ~ 2010-01 by dhjin, 인피니티 - 소스 체크

	return TRUE;
}

Err_t CInfinityManager::JoinCheckInfinity(MSG_FC_INFINITY_JOIN * i_pInfinityJoinRequestInfo,  CFieldIOCPSocket * i_pFISoc) {
	if(NULL == i_pInfinityJoinRequestInfo
		|| NULL == i_pFISoc) {
		// 2009-09-09 ~ 2010-01 by dhjin, 인피니티 - 소스 체크
		return ERR_INFINITY_NULL_VALUE;
	}

	// 인피 가입 전 체크 사항 
	switch (i_pInfinityJoinRequestInfo->InfinityMode) {
	case INFINITY_MODE_BOSSRUSH: {
		mt_auto_lock mta(&m_mtvectInfiBossRush);
		mtvectInfiBossRush::iterator itr = m_mtvectInfiBossRush.begin();
		for(; itr != m_mtvectInfiBossRush.end(); itr++) { 
			if(i_pInfinityJoinRequestInfo->InfinityCreateUID == (*itr)->GetInfinityCreateUID()) {
				Err_t errCode = (*itr)->JoinCheck(i_pFISoc);
				return errCode;
			}
		}
		mta.auto_unlock_cancel();
								 } // case INFINITY_MODE_BOSSRUSH
		break; 
	case INFINITY_MODE_DEFENCE: {
		mt_auto_lock mta(&m_mtvectInfiDefence);
		mtvectInfiDefence::iterator itr = m_mtvectInfiDefence.begin();
		for(; itr != m_mtvectInfiDefence.end(); itr++) { 
			if(i_pInfinityJoinRequestInfo->InfinityCreateUID == (*itr)->GetInfinityCreateUID()) {
				Err_t errCode = (*itr)->JoinCheck(i_pFISoc);
				return errCode;
			}
		}
		mta.auto_unlock_cancel();
								} // case INFINITY_MODE_DEFENCE
		break;

	case INFINITY_MODE_MSHIPBATTLE: {				// 2011-02-18 by hskim, 인피니티 3차 - 방 생성 작업
		mt_auto_lock mta(&m_mtvectInfiMShipBattle);
		mtvectInfiMShipBattle::iterator itr = m_mtvectInfiMShipBattle.begin();
		for(; itr != m_mtvectInfiMShipBattle.end(); itr++) { 
			if(i_pInfinityJoinRequestInfo->InfinityCreateUID == (*itr)->GetInfinityCreateUID()) {
				Err_t errCode = (*itr)->JoinCheck(i_pFISoc);
				return errCode;
			}
		}
		mta.auto_unlock_cancel();
								} // case INFINITY_MODE_MSHIPBATTLE
		break;

	default : {
		return ERR_INFINITY_MODE;
			  } // default
	}	
	
	return ERR_INFINITY_CREATEUID;
}

Err_t CInfinityManager::JoinInfinity(eINFINITY_MODE i_eInfiMode, InfinityCreateUID_t i_CreateUID,  CFieldIOCPSocket * i_pFISoc) 
{
	if(NULL == i_pFISoc) 
	{
		// 2009-09-09 ~ 2010-01 by dhjin, 인피니티 - 소스 체크
		return ERR_INFINITY_NULL_VALUE;
	}

	// 인피 가입
	switch (i_eInfiMode) 
	{
		case INFINITY_MODE_BOSSRUSH: 
			{
				mt_auto_lock mta(&m_mtvectInfiBossRush);
				mtvectInfiBossRush::iterator itr = m_mtvectInfiBossRush.begin();
				for(; itr != m_mtvectInfiBossRush.end(); itr++) 
				{ 
					if(i_CreateUID == (*itr)->GetInfinityCreateUID()) 
					{
						Err_t errCode = (*itr)->Join(i_pFISoc);
						return errCode;
					}
				}
				mta.auto_unlock_cancel();
			} // case INFINITY_MODE_BOSSRUSH
			break; 

		case INFINITY_MODE_DEFENCE: 
			{
				mt_auto_lock mta(&m_mtvectInfiDefence);
				mtvectInfiDefence::iterator itr = m_mtvectInfiDefence.begin();
				for(; itr != m_mtvectInfiDefence.end(); itr++) 
				{ 
					if(i_CreateUID == (*itr)->GetInfinityCreateUID()) 
					{
						Err_t errCode = (*itr)->Join(i_pFISoc);
						return errCode;
					}
				}
				mta.auto_unlock_cancel();
			} // case INFINITY_MODE_DEFENCE
			break;

		case INFINITY_MODE_MSHIPBATTLE:			// 2011-02-18 by hskim, 인피니티 3차 - 방 생성 작업
			{
				mt_auto_lock mta(&m_mtvectInfiMShipBattle);
				mtvectInfiMShipBattle::iterator itr = m_mtvectInfiMShipBattle.begin();
				for(; itr != m_mtvectInfiMShipBattle.end(); itr++) 
				{ 
					if(i_CreateUID == (*itr)->GetInfinityCreateUID()) 
					{
						Err_t errCode = (*itr)->Join(i_pFISoc);
						return errCode;
					}
				}
				mta.auto_unlock_cancel();
			} // case INFINITY_MODE_MSHIPBATTLE
			break;

		default : 
			{
				return ERR_INFINITY_MODE;
			} // default
	}	
	
	return ERR_INFINITY_CREATEUID;
}

void CInfinityManager::GetPlayerListW(eINFINITY_MODE i_eInfiMode, InfinityCreateUID_t i_CreateUID, vectCFieldIOCPSocket * o_pInfinityMemberList, ClientIndex_t * o_pMasterUserClientIdx) {
	if(NULL == o_pInfinityMemberList
		|| NULL == o_pMasterUserClientIdx) {
		// 2009-09-09 ~ 2010-01 by dhjin, 인피니티 - 소스 체크
		return;
	}

	switch (i_eInfiMode) {
	case INFINITY_MODE_BOSSRUSH: {
		mt_auto_lock mta(&m_mtvectInfiBossRush);
		mtvectInfiBossRush::iterator itr = m_mtvectInfiBossRush.begin();
		for(; itr != m_mtvectInfiBossRush.end(); itr++) { 
			if(i_CreateUID == (*itr)->GetInfinityCreateUID()) {
				(*itr)->GetPlayerList(o_pInfinityMemberList, o_pMasterUserClientIdx);
				return;
			}
		}
		mta.auto_unlock_cancel();
								 } // case INFINITY_MODE_BOSSRUSH
		break; 
	case INFINITY_MODE_DEFENCE: {
		mt_auto_lock mta(&m_mtvectInfiDefence);
		mtvectInfiDefence::iterator itr = m_mtvectInfiDefence.begin();
		for(; itr != m_mtvectInfiDefence.end(); itr++) { 
			if(i_CreateUID == (*itr)->GetInfinityCreateUID()) {
				(*itr)->GetPlayerList(o_pInfinityMemberList, o_pMasterUserClientIdx);
				return;
			}
		}
		mta.auto_unlock_cancel();
								} // case INFINITY_MODE_DEFENCE
		break;
	case INFINITY_MODE_MSHIPBATTLE: {		// 2011-02-18 by hskim, 인피니티 3차 - 방 생성 작업
		mt_auto_lock mta(&m_mtvectInfiMShipBattle);
		mtvectInfiMShipBattle::iterator itr = m_mtvectInfiMShipBattle.begin();
		for(; itr != m_mtvectInfiMShipBattle.end(); itr++) { 
			if(i_CreateUID == (*itr)->GetInfinityCreateUID()) {
				(*itr)->GetPlayerList(o_pInfinityMemberList, o_pMasterUserClientIdx);
				return;
			}
		}
		mta.auto_unlock_cancel();
								} // case INFINITY_MODE_MSHIPBATTLE
		break;
	default : {
		return;
			  } // default
	}	
	
	return;
}

Err_t CInfinityManager::ChangeMasterUserW(eINFINITY_MODE i_eInfiMode, InfinityCreateUID_t i_CreateUID, CFieldIOCPSocket * i_pChangeMasterUserFISoc) {
	if(NULL == i_pChangeMasterUserFISoc) {
		// 2009-09-09 ~ 2010-01 by dhjin, 인피니티 - 소스 체크
		return ERR_INFINITY_NULL_VALUE;
	}

	switch (i_eInfiMode) {
	case INFINITY_MODE_BOSSRUSH: {
		mt_auto_lock mta(&m_mtvectInfiBossRush);
		mtvectInfiBossRush::iterator itr = m_mtvectInfiBossRush.begin();
		for(; itr != m_mtvectInfiBossRush.end(); itr++) { 
			if(i_CreateUID == (*itr)->GetInfinityCreateUID()) {
				Err_t errCode = (*itr)->ChangeMasterUser(i_pChangeMasterUserFISoc);
				return errCode;
			}
		}
		mta.auto_unlock_cancel();
								 } // case INFINITY_MODE_BOSSRUSH
		break; 
	case INFINITY_MODE_DEFENCE: {
		mt_auto_lock mta(&m_mtvectInfiDefence);
		mtvectInfiDefence::iterator itr = m_mtvectInfiDefence.begin();
		for(; itr != m_mtvectInfiDefence.end(); itr++) { 
			if(i_CreateUID == (*itr)->GetInfinityCreateUID()) {
				Err_t errCode = (*itr)->ChangeMasterUser(i_pChangeMasterUserFISoc);
				return errCode;
			}
		}
		mta.auto_unlock_cancel();
								} // case INFINITY_MODE_DEFENCE
		break;
	case INFINITY_MODE_MSHIPBATTLE: {			// 2011-02-18 by hskim, 인피니티 3차 - 방 생성 작업
		mt_auto_lock mta(&m_mtvectInfiMShipBattle);
		mtvectInfiMShipBattle::iterator itr = m_mtvectInfiMShipBattle.begin();
		for(; itr != m_mtvectInfiMShipBattle.end(); itr++) { 
			if(i_CreateUID == (*itr)->GetInfinityCreateUID()) {
				Err_t errCode = (*itr)->ChangeMasterUser(i_pChangeMasterUserFISoc);
				return errCode;
			}
		}
		mta.auto_unlock_cancel();
								} // case INFINITY_MODE_MSHIPBATTLE
		break;
	default : {
		return ERR_INFINITY_MODE;
			  } // default
	}	
	
	return ERR_INFINITY_CREATEUID;
}

Err_t CInfinityManager::LeaveInfinity(eINFINITY_MODE i_eInfiMode, InfinityCreateUID_t i_CreateUID, CFieldIOCPSocket * i_pFISoc) {
	if(NULL == i_pFISoc) {
		// 2009-09-09 ~ 2010-01 by dhjin, 인피니티 - 소스 체크
		return ERR_INFINITY_NULL_VALUE;
	}

	// 인피 탈퇴
	switch (i_eInfiMode) {
	case INFINITY_MODE_BOSSRUSH: {
		mt_auto_lock mta(&m_mtvectInfiBossRush);
		mtvectInfiBossRush::iterator itr = m_mtvectInfiBossRush.begin();
		for(; itr != m_mtvectInfiBossRush.end(); itr++) { 
			if(i_CreateUID == (*itr)->GetInfinityCreateUID()) {
				Err_t errCode = (*itr)->Leave(i_pFISoc);
				if(0 >= (*itr)->GetPlayerListSize())
				{
					// 2010-04-02 by cmkwon, 인피2차 추가 수정 - 
					g_pFieldGlobal->WriteSystemLogEX(TRUE, "[Notify] [Infinity][%I64d] CInfinityManager::LeaveInfinity# BossRush Call CInfinityMapManager::ResetInfinityMap# !, MapInfo<%d(%d)> \r\n"
						, (*itr)->GetInfinityCreateUID(), (*itr)->GetInfinityMapIndex(), (*itr)->GetInfinityChannelIndex());

					// 마지막 유저였다면 방 삭제
					this->m_InfiMapManager.ResetInfinityMap((*itr)->GetInfinityMapIndex(), (*itr)->GetInfinityChannelIndex());
					util::del(*itr);
					itr = m_mtvectInfiBossRush.erase(itr);
				}
				return errCode;
			}
		}
		mta.auto_unlock_cancel();
								 } // case INFINITY_MODE_BOSSRUSH
		break; 
	case INFINITY_MODE_DEFENCE: {
		mt_auto_lock mta(&m_mtvectInfiDefence);
		mtvectInfiDefence::iterator itr = m_mtvectInfiDefence.begin();
		for(; itr != m_mtvectInfiDefence.end(); itr++) { 
			if(i_CreateUID == (*itr)->GetInfinityCreateUID()) {
				Err_t errCode = (*itr)->Leave(i_pFISoc);
				if(0 >= (*itr)->GetPlayerListSize())
				{
					// 2010-04-02 by cmkwon, 인피2차 추가 수정 - 
					g_pFieldGlobal->WriteSystemLogEX(TRUE, "[Notify] [Infinity][%I64d] CInfinityManager::LeaveInfinity# Defence Call CInfinityMapManager::ResetInfinityMap# !, MapInfo<%d(%d)> \r\n"
						, (*itr)->GetInfinityCreateUID(), (*itr)->GetInfinityMapIndex(), (*itr)->GetInfinityChannelIndex());

					// 마지막 유저였다면 방 삭제
					this->m_InfiMapManager.ResetInfinityMap((*itr)->GetInfinityMapIndex(), (*itr)->GetInfinityChannelIndex());
					util::del(*itr);
					itr = m_mtvectInfiDefence.erase(itr);
				}
				return errCode;
			}
		}
		mta.auto_unlock_cancel();
								} // case INFINITY_MODE_DEFENCE
		break;
	case INFINITY_MODE_MSHIPBATTLE: {					// 2011-02-18 by hskim, 인피니티 3차 - 방 생성 작업
		mt_auto_lock mta(&m_mtvectInfiMShipBattle);
		mtvectInfiMShipBattle::iterator itr = m_mtvectInfiMShipBattle.begin();
		for(; itr != m_mtvectInfiMShipBattle.end(); itr++) { 
			if(i_CreateUID == (*itr)->GetInfinityCreateUID()) {
				Err_t errCode = (*itr)->Leave(i_pFISoc);
				if(0 >= (*itr)->GetPlayerListSize())
				{
					g_pFieldGlobal->WriteSystemLogEX(TRUE, "[Notify] [Infinity][%I64d] CInfinityManager::LeaveInfinity# MShipBattle Call CInfinityMapManager::ResetInfinityMap# !, MapInfo<%d(%d)> \r\n"
						, (*itr)->GetInfinityCreateUID(), (*itr)->GetInfinityMapIndex(), (*itr)->GetInfinityChannelIndex());

					this->m_InfiMapManager.ResetInfinityMap((*itr)->GetInfinityMapIndex(), (*itr)->GetInfinityChannelIndex());
					util::del(*itr);
					itr = m_mtvectInfiMShipBattle.erase(itr);
				}
				return errCode;
			}
		}
		mta.auto_unlock_cancel();
								} // case INFINITY_MODE_MSHIPBATTLE
		break;
	default : {
		return ERR_INFINITY_MODE;
			  } // default
	}	
	
	return ERR_INFINITY_CREATEUID;
}

Err_t CInfinityManager::BanInfinity(eINFINITY_MODE i_eInfiMode, InfinityCreateUID_t i_CreateUID, CFieldIOCPSocket * i_pBanFISoc) {
	if(NULL == i_pBanFISoc) {
		// 2009-09-09 ~ 2010-01 by dhjin, 인피니티 - 소스 체크
		return ERR_INFINITY_NULL_VALUE;
	}

	// 인피 추방
	switch (i_eInfiMode) {
	case INFINITY_MODE_BOSSRUSH: {
		mt_auto_lock mta(&m_mtvectInfiBossRush);
		mtvectInfiBossRush::iterator itr = m_mtvectInfiBossRush.begin();
		for(; itr != m_mtvectInfiBossRush.end(); itr++) { 
			if(i_CreateUID == (*itr)->GetInfinityCreateUID()) {
				Err_t errCode = (*itr)->Ban(i_pBanFISoc);
				return errCode;
			}
		}
		mta.auto_unlock_cancel();
								 } // case INFINITY_MODE_BOSSRUSH
		break; 
	case INFINITY_MODE_DEFENCE: {
		mt_auto_lock mta(&m_mtvectInfiDefence);
		mtvectInfiDefence::iterator itr = m_mtvectInfiDefence.begin();
		for(; itr != m_mtvectInfiDefence.end(); itr++) { 
			if(i_CreateUID == (*itr)->GetInfinityCreateUID()) {
				Err_t errCode = (*itr)->Ban(i_pBanFISoc);
				return errCode;
			}
		}
		mta.auto_unlock_cancel();
								} // case INFINITY_MODE_DEFENCE
		break;
	case INFINITY_MODE_MSHIPBATTLE: {			// 2011-02-18 by hskim, 인피니티 3차 - 방 생성 작업
		mt_auto_lock mta(&m_mtvectInfiMShipBattle);
		mtvectInfiMShipBattle::iterator itr = m_mtvectInfiMShipBattle.begin();
		for(; itr != m_mtvectInfiMShipBattle.end(); itr++) { 
			if(i_CreateUID == (*itr)->GetInfinityCreateUID()) {
				Err_t errCode = (*itr)->Ban(i_pBanFISoc);
				return errCode;
			}
		}
		mta.auto_unlock_cancel();
								} // case INFINITY_MODE_MSHIPBATTLE
		break;
	default : {
		return ERR_INFINITY_MODE;
			  } // default
	}	
	
	return ERR_INFINITY_CREATEUID;
}

Err_t CInfinityManager::StartInfinity(eINFINITY_MODE i_eInfiMode, InfinityCreateUID_t i_CreateUID, CFieldIOCPSocket * i_pFISoc)
{
	if(NULL == i_pFISoc 
		|| FALSE == i_pFISoc->IsValidCharacter(FALSE))
	{
		// 2009-09-09 ~ 2010-01 by dhjin, 인피니티 - 소스 체크
		return ERR_INFINITY_NULL_VALUE;
	}

	// 인피 시작
	switch (i_eInfiMode)
	{
	case INFINITY_MODE_BOSSRUSH:
		{
			mt_auto_lock mta(&m_mtvectInfiBossRush);
			///////////////////////////////////////////////////////////////////////////////
			// 2010-04-05 by cmkwon, 인피 재입장 카드 관련 시스템 수정 - 
			CInfinityBossrush *pInfinity = this->FindBossrushNoLock(i_CreateUID);
			if(pInfinity)
			{				
				Err_t errCode = pInfinity->Start(i_pFISoc, this->m_InfiMapManager.CreateInfinityMap(pInfinity->GetInfinityMapIndex()));
				return errCode;
			}
		} // case INFINITY_MODE_BOSSRUSH
		break; 
	case INFINITY_MODE_DEFENCE:
		{
			mt_auto_lock mta(&m_mtvectInfiDefence);
			///////////////////////////////////////////////////////////////////////////////
			// 2010-04-05 by cmkwon, 인피 재입장 카드 관련 시스템 수정 - 
			CInfinityDefence *pInfinity = this->FindDefenceNoLock(i_CreateUID);
			if(pInfinity)
			{
				Err_t errCode = pInfinity->Start(i_pFISoc, this->m_InfiMapManager.CreateInfinityMap(pInfinity->GetInfinityMapIndex()));
				return errCode;
			}
		} // case INFINITY_MODE_DEFENCE
		break;
	case INFINITY_MODE_MSHIPBATTLE:		// 2011-02-18 by hskim, 인피니티 3차 - 방 생성 작업
		{
			mt_auto_lock mta(&m_mtvectInfiMShipBattle);

			CInfinityMShipBattle *pInfinity = this->FindMShipBattleNoLock(i_CreateUID);
			if(pInfinity)
			{
   				Err_t errCode = pInfinity->Start(i_pFISoc, this->m_InfiMapManager.CreateInfinityMap(pInfinity->GetInfinityMapIndex()));
				return errCode;
			}
		} // case INFINITY_MODE_MSHIPBATTLE
		break;
	default :
		{
			return ERR_INFINITY_MODE;
		} // default
	}
	
	return ERR_INFINITY_CREATEUID;
}

BOOL CInfinityManager::GetRevisionInfoW(REVISIONINFO * o_pRevisionInfo, InfiModeUID_t i_InfinityModeUID, INT i_nUnitKind) {
	if(NULL == o_pRevisionInfo) {
		// 2009-09-09 ~ 2010-01 by dhjin, 인피니티 - 소스 체크
		return FALSE;
	}

	// 보정치 가져오기
	vectorInfinityModeInfo::iterator itr = m_vectInfiModeInfo.begin();
	for(; itr != m_vectInfiModeInfo.end(); itr++) {
		if(i_InfinityModeUID == itr->InfinityModeUID) {
			if(this->m_Revision.GetRevisionInfo(o_pRevisionInfo, itr->RevisionNum, i_nUnitKind)) {
				return TRUE;			
			}
			return FALSE;
		}
	}

	return FALSE;
}

Err_t CInfinityManager::UserMapLoadedComplete(eINFINITY_MODE i_eInfiMode, InfinityCreateUID_t i_CreateUID, MapIndex_t i_MapIndex)
{
	// 인피 맵 생성
	switch (i_eInfiMode)
	{
	case INFINITY_MODE_BOSSRUSH:
		{
			mt_auto_lock mta(&m_mtvectInfiBossRush);
			///////////////////////////////////////////////////////////////////////////////
			// 2010-04-05 by cmkwon, 인피 재입장 카드 관련 시스템 수정 - 
			CInfinityBossrush *pInfinity = this->FindBossrushNoLock(i_CreateUID);
			if(pInfinity)
			{
				return pInfinity->CreateMap(NULL);
			}

		} // case INFINITY_MODE_BOSSRUSH
		break; 
	case INFINITY_MODE_DEFENCE:
		{
			mt_auto_lock mta(&m_mtvectInfiDefence);
			///////////////////////////////////////////////////////////////////////////////
			// 2010-04-05 by cmkwon, 인피 재입장 카드 관련 시스템 수정 - 
			CInfinityDefence *pInfinity = this->FindDefenceNoLock(i_CreateUID);
			if(pInfinity)
			{
				return pInfinity->CreateMap(NULL);
			}
		} // case INFINITY_MODE_DEFENCE
		break;
	case INFINITY_MODE_MSHIPBATTLE:			// 2011-02-18 by hskim, 인피니티 3차 - 방 생성 작업
		{
			mt_auto_lock mta(&m_mtvectInfiMShipBattle);

			CInfinityMShipBattle *pInfinity = this->FindMShipBattleNoLock(i_CreateUID);
			if(pInfinity)
			{
				return pInfinity->CreateMap(NULL);
			}
		} // case INFINITY_MODE_MSHIPBATTLE
		break;
	default :
		{
			return ERR_INFINITY_MODE;
		} // default
	}
	
	return ERR_INFINITY_CREATEUID;
}


void CInfinityManager::StartTick() {
	m_pTickManager->InitThread();
}

void CInfinityManager::DoSecondlyWorkInfinity(ATUM_DATE_TIME *pDateTime) {
	// Tick
	
	{
		mt_auto_lock mta(&m_mtvectInfiBossRush);
		mtvectInfiBossRush::iterator itr = m_mtvectInfiBossRush.begin();
		while (itr != m_mtvectInfiBossRush.end())
		{
			if((*itr)->CheckDestory())
			{
				// 2010-04-02 by cmkwon, 인피2차 추가 수정 - 
				g_pFieldGlobal->WriteSystemLogEX(TRUE, "[Notify] [Infinity][%I64d] CInfinityManager::DoSecondlyWorkInfinity# BossRush Call CInfinityMapManager::ResetInfinityMap# !, MapInfo<%d(%d)> \r\n"
					, (*itr)->GetInfinityCreateUID(), (*itr)->GetInfinityMapIndex(), (*itr)->GetInfinityChannelIndex());

				this->m_InfiMapManager.ResetInfinityMap((*itr)->GetInfinityMapIndex(), (*itr)->GetInfinityChannelIndex());
				util::del(*itr);
				itr = m_mtvectInfiBossRush.erase(itr);
				continue;
			}
			(*itr)->DoSecondlyWorkInfinity(pDateTime);
			itr++;
		}
		mta.auto_unlock_cancel();
	}

	mt_auto_lock mtD(&m_mtvectInfiDefence);
	mtvectInfiDefence::iterator itrD = m_mtvectInfiDefence.begin();
	while (itrD != m_mtvectInfiDefence.end()) {
		if((*itrD)->CheckDestory())
		{
			// 2010-04-02 by cmkwon, 인피2차 추가 수정 - 
			g_pFieldGlobal->WriteSystemLogEX(TRUE, "[Notify] [Infinity][%I64d] CInfinityManager::DoSecondlyWorkInfinity# Defence Call CInfinityMapManager::ResetInfinityMap# !, MapInfo<%d(%d)> \r\n"
				, (*itrD)->GetInfinityCreateUID(), (*itrD)->GetInfinityMapIndex(), (*itrD)->GetInfinityChannelIndex());

			this->m_InfiMapManager.ResetInfinityMap((*itrD)->GetInfinityMapIndex(), (*itrD)->GetInfinityChannelIndex());			
			util::del(*itrD);
			itrD = m_mtvectInfiDefence.erase(itrD);
			continue;
		}
		(*itrD)->DoSecondlyWorkInfinity(pDateTime);
		itrD++;
	}
	mtD.auto_unlock_cancel();

	{		// 2011-02-18 by hskim, 인피니티 3차 - 방 생성 작업
		mt_auto_lock mta(&m_mtvectInfiMShipBattle);
		mtvectInfiMShipBattle::iterator itr = m_mtvectInfiMShipBattle.begin();
		while (itr != m_mtvectInfiMShipBattle.end())
		{
			if((*itr)->CheckDestory())
			{
				g_pFieldGlobal->WriteSystemLogEX(TRUE, "[Notify] [Infinity][%I64d] CInfinityManager::DoSecondlyWorkInfinity# MShipBattle Call CInfinityMapManager::ResetInfinityMap# !, MapInfo<%d(%d)> \r\n"
					, (*itr)->GetInfinityCreateUID(), (*itr)->GetInfinityMapIndex(), (*itr)->GetInfinityChannelIndex());
				
				this->m_InfiMapManager.ResetInfinityMap((*itr)->GetInfinityMapIndex(), (*itr)->GetInfinityChannelIndex());
				util::del(*itr);
				itr = m_mtvectInfiMShipBattle.erase(itr);
				continue;
			}
			(*itr)->DoSecondlyWorkInfinity(pDateTime);
			itr++;
		}
		mta.auto_unlock_cancel();
	}
}


void CInfinityManager::CreateKeyMonster_DeadForNextStepW(MapIndex_t i_MapIndex, ChannelIndex_t i_ChannelIdx, MonIdx_t i_CreateMonsterIdx) {
	mt_auto_lock mta(&m_mtvectInfiBossRush);
	mtvectInfiBossRush::iterator itr = m_mtvectInfiBossRush.begin();
	for(; itr != m_mtvectInfiBossRush.end(); itr++) {
		if(i_MapIndex == (*itr)->GetInfinityMapIndex() 
			&& i_ChannelIdx == (*itr)->GetInfinityChannelIndex()) {
			(*itr)->CreateKeyMonster_DeadForNextStep(i_CreateMonsterIdx);
			return;
		}
	}
	mta.auto_unlock_cancel();
	
	mt_auto_lock mtD(&m_mtvectInfiDefence);
	mtvectInfiDefence::iterator itrD = m_mtvectInfiDefence.begin();
	for(; itrD != m_mtvectInfiDefence.end(); itrD++) {
		if(i_MapIndex == (*itrD)->GetInfinityMapIndex() 
			&& i_ChannelIdx == (*itrD)->GetInfinityChannelIndex()) {
			(*itrD)->CreateKeyMonster_DeadForNextStep(i_CreateMonsterIdx);
			return;
		}
	}
	mtD.auto_unlock_cancel();
	
	mt_auto_lock mtT(&m_mtvectInfiMShipBattle);			// 2011-02-18 by hskim, 인피니티 3차 - 방 생성 작업
	mtvectInfiMShipBattle::iterator itrT = m_mtvectInfiMShipBattle.begin();
	for(; itrT != m_mtvectInfiMShipBattle.end(); itrT++) {
		if(i_MapIndex == (*itrT)->GetInfinityMapIndex() 
			&& i_ChannelIdx == (*itrT)->GetInfinityChannelIndex()) {
			(*itrT)->CreateKeyMonster_DeadForNextStep(i_CreateMonsterIdx);
			return;
		}
	}
	mtT.auto_unlock_cancel();
}

// 2010-04-09 by cmkwon, 인피2차 추가 수정(단계별 보상 추가) - 리턴값이 키몬스터 보상 처리 여부 플래그
void CInfinityManager::DeleteKeyMonster_DeadForNextStepW(BOOL *o_pbCompensationFlag, MapIndex_t i_MapIndex, ChannelIndex_t i_ChannelIdx, MonIdx_t i_DeadMonsterIdx)
{
// 2010-04-09 by cmkwon, 인피2차 추가 수정(단계별 보상 추가) - 
// 	mt_auto_lock mta(&m_mtvectInfiBossRush);
// 	mtvectInfiBossRush::iterator itr = m_mtvectInfiBossRush.begin();
// 	for(; itr != m_mtvectInfiBossRush.end(); itr++) {
// 		if(i_MapIndex == (*itr)->GetInfinityMapIndex() 
// 			&& i_ChannelIdx == (*itr)->GetInfinityChannelIndex()) {
// 			(*itr)->DeleteKeyMonster_DeadForNextStep(i_DeadMonsterIdx);
// 			return;
// 		}
// 	}
// 	mta.auto_unlock_cancel();
// 	
// 	mt_auto_lock mtD(&m_mtvectInfiDefence);
// 	mtvectInfiDefence::iterator itrD = m_mtvectInfiDefence.begin();
// 	for(; itrD != m_mtvectInfiDefence.end(); itrD++) {
// 		if(i_MapIndex == (*itrD)->GetInfinityMapIndex() 
// 			&& i_ChannelIdx == (*itrD)->GetInfinityChannelIndex()) {
// 			(*itrD)->DeleteKeyMonster_DeadForNextStep(i_DeadMonsterIdx);
// 			return;
// 		}
// 	}
// 	mtD.auto_unlock_cancel();
	///////////////////////////////////////////////////////////////////////////////
	// 2010-04-09 by cmkwon, 인피2차 추가 수정(단계별 보상 추가) - 
	{
		mt_auto_lock mtA(&m_mtvectInfiBossRush);
		CInfinityBossrush *pInfinity = this->FindBossrushNoLock(i_MapIndex, i_ChannelIdx);
		if(pInfinity)
		{
			pInfinity->DeleteKeyMonster_DeadForNextStep(o_pbCompensationFlag, i_DeadMonsterIdx);
			return;
		}
	}
	{
		mt_auto_lock mtA(&m_mtvectInfiDefence);
		CInfinityDefence *pInfinity = this->FindDefenceNoLock(i_MapIndex, i_ChannelIdx);
		if(pInfinity)
		{
			pInfinity->DeleteKeyMonster_DeadForNextStep(o_pbCompensationFlag, i_DeadMonsterIdx);
			return;
		}
	}
	{			// 2011-02-18 by hskim, 인피니티 3차 - 방 생성 작업
		mt_auto_lock mtA(&m_mtvectInfiMShipBattle);
		CInfinityMShipBattle *pInfinity = this->FindMShipBattleNoLock(i_MapIndex, i_ChannelIdx);
		if(pInfinity)
		{
			pInfinity->DeleteKeyMonster_DeadForNextStep(o_pbCompensationFlag, i_DeadMonsterIdx);
			return;
		}
	}
}

Err_t CInfinityManager::ChoiceTenderItemW(DiceCnt_t *o_pDiceResult, MSG_FC_INFINITY_TENDER_PUT_IN_TENDER * i_pPutInTenderInfo, ClientIndex_t i_PlayerClientIdx)
{
	switch (i_pPutInTenderInfo->InfinityMode)
	{
	case INFINITY_MODE_BOSSRUSH:
		{
			mt_auto_lock mta(&m_mtvectInfiBossRush);
			CInfinityBossrush *pInfinity = this->FindBossrushNoLock(i_pPutInTenderInfo->InfinityCreateUID);
			if(pInfinity)
			{
				return pInfinity->ChoiceTenderItem(o_pDiceResult, i_PlayerClientIdx, i_pPutInTenderInfo->ItemFieldIndex, i_pPutInTenderInfo->GiveUp);
			}
		} // case INFINITY_MODE_BOSSRUSH
		break; 
	case INFINITY_MODE_DEFENCE:
		{
			mt_auto_lock mta(&m_mtvectInfiDefence);
			CInfinityDefence *pInfinity = this->FindDefenceNoLock(i_pPutInTenderInfo->InfinityCreateUID);
			if(pInfinity)
			{
				return pInfinity->ChoiceTenderItem(o_pDiceResult, i_PlayerClientIdx, i_pPutInTenderInfo->ItemFieldIndex, i_pPutInTenderInfo->GiveUp);
			}
		} // case INFINITY_MODE_DEFENCE
		break;
	case INFINITY_MODE_MSHIPBATTLE:			// 2011-02-18 by hskim, 인피니티 3차 - 방 생성 작업
		{
			mt_auto_lock mta(&m_mtvectInfiMShipBattle);
			CInfinityMShipBattle *pInfinity = this->FindMShipBattleNoLock(i_pPutInTenderInfo->InfinityCreateUID);
			if(pInfinity)
			{
				return pInfinity->ChoiceTenderItem(o_pDiceResult, i_PlayerClientIdx, i_pPutInTenderInfo->ItemFieldIndex, i_pPutInTenderInfo->GiveUp);
			}
		} // case INFINITY_MODE_MSHIPBATTLE
		break;
	default :
		{
			return ERR_INFINITY_MODE;
		} // default
	}	
	
	return ERR_INFINITY_CREATEUID;
}

void CInfinityManager::ProcessingInfinityPenalty(char * i_szCharacterName, eINFINITY_MODE i_eInfiMode, InfinityCreateUID_t i_CreateUID) {
	if(NULL == i_szCharacterName) {
		// 2009-09-09 ~ 2010-01 by dhjin, 인피니티 - 소스 체크
		return;
	}
	
	// 2009-09-09 ~ 2010-01-13 by dhjin, 인피니티 - 죽은 유저 이름 정보 전송 추가, // 인피니티 - 인피 사망시 패널티 추가
	switch (i_eInfiMode) {
	case INFINITY_MODE_BOSSRUSH: {
		mt_auto_lock mta(&m_mtvectInfiBossRush);
		mtvectInfiBossRush::iterator itr = m_mtvectInfiBossRush.begin();
		for(; itr != m_mtvectInfiBossRush.end(); itr++) { 
			if(i_CreateUID == (*itr)->GetInfinityCreateUID()) {
				(*itr)->CalcLimitTimeByUserDeath(i_szCharacterName);
				return;
			}
		}
		mta.auto_unlock_cancel();
								 } // case INFINITY_MODE_BOSSRUSH
		break; 
 	case INFINITY_MODE_DEFENCE: {
 		mt_auto_lock mta(&m_mtvectInfiDefence);
 		mtvectInfiDefence::iterator itr = m_mtvectInfiDefence.begin();
 		for(; itr != m_mtvectInfiDefence.end(); itr++) { 
 			if(i_CreateUID == (*itr)->GetInfinityCreateUID()) {
 				(*itr)->CalcAliveForGameClearMonsterHPByUserDeath(i_szCharacterName);	
				return;
 			}
 		}
 		mta.auto_unlock_cancel();
 								} // case INFINITY_MODE_DEFENCE
 		break;
	case INFINITY_MODE_MSHIPBATTLE: {				// 2011-02-18 by hskim, 인피니티 3차 - 방 생성 작업
		mt_auto_lock mta(&m_mtvectInfiMShipBattle);
		mtvectInfiMShipBattle::iterator itr = m_mtvectInfiMShipBattle.begin();
		for(; itr != m_mtvectInfiMShipBattle.end(); itr++) { 
			if(i_CreateUID == (*itr)->GetInfinityCreateUID()) {
				(*itr)->CalcLimitTimeByUserDeath(i_szCharacterName);					// 패널티 적용 (제한 시간)
				(*itr)->CalcAliveForGameClearMonsterHPByUserDeath(i_szCharacterName);	// 패널티 적용 (아군 모선 HP)
				return;
			}
		}
		mta.auto_unlock_cancel();
								} // case INFINITY_MODE_MSHIPBATTLE
		break;
	default : {
		return;
			  } // default
	}	
	
	return;
}

void CInfinityManager::SendInfinityTeamChatW(eINFINITY_MODE i_eInfiMode, InfinityCreateUID_t i_CreateUID, BYTE * i_pDATA, int i_nSize) {
	// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 채팅
	switch (i_eInfiMode) {
	case INFINITY_MODE_BOSSRUSH: {
		mt_auto_lock mta(&m_mtvectInfiBossRush);
		mtvectInfiBossRush::iterator itr = m_mtvectInfiBossRush.begin();
		for(; itr != m_mtvectInfiBossRush.end(); itr++) { 
			if(i_CreateUID == (*itr)->GetInfinityCreateUID()) {
				(*itr)->SendInfinityTeamChat(i_pDATA, i_nSize);
				return;
			}
		}
		mta.auto_unlock_cancel();
								 } // case INFINITY_MODE_BOSSRUSH
		break; 
	case INFINITY_MODE_DEFENCE: {
		mt_auto_lock mta(&m_mtvectInfiDefence);
		mtvectInfiDefence::iterator itr = m_mtvectInfiDefence.begin();
		for(; itr != m_mtvectInfiDefence.end(); itr++) { 
			if(i_CreateUID == (*itr)->GetInfinityCreateUID()) {
				(*itr)->SendInfinityTeamChat(i_pDATA, i_nSize);
				return;
			}
		}
		mta.auto_unlock_cancel();
								} // case INFINITY_MODE_DEFENCE
		break;
	case INFINITY_MODE_MSHIPBATTLE: {			// 2011-02-18 by hskim, 인피니티 3차 - 방 생성 작업
		mt_auto_lock mta(&m_mtvectInfiMShipBattle);
		mtvectInfiMShipBattle::iterator itr = m_mtvectInfiMShipBattle.begin();
		for(; itr != m_mtvectInfiMShipBattle.end(); itr++) { 
			if(i_CreateUID == (*itr)->GetInfinityCreateUID()) {
				(*itr)->SendInfinityTeamChat(i_pDATA, i_nSize);
				return;
			}
		}
		mta.auto_unlock_cancel();
								} // case INFINITY_MODE_MSHIPBATTLE
		break;
	default : {
		return;
			  } // default
	}	
	
	return;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2010-04-05 by cmkwon, 인피 재입장 카드 관련 시스템 수정 - 
/// \author		cmkwon
/// \date		2010-04-05 ~ 2010-04-05
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
Err_t CInfinityManager::ImputeInfinityW(eINFINITY_MODE i_eInfiMode, InfinityCreateUID_t i_CreateUID)
{
	// 인피 맵 생성
	switch (i_eInfiMode)
	{
	case INFINITY_MODE_BOSSRUSH:
		{
			mt_auto_lock mta(&m_mtvectInfiBossRush);
			///////////////////////////////////////////////////////////////////////////////
			// 2010-04-05 by cmkwon, 인피 재입장 카드 관련 시스템 수정 - 
			CInfinityBossrush *pInfinity = this->FindBossrushNoLock(i_CreateUID);
			if(pInfinity)
			{
				return pInfinity->ImputeInfinity();
			}
			
		} // case INFINITY_MODE_BOSSRUSH
		break; 
	case INFINITY_MODE_DEFENCE:
		{
			mt_auto_lock mta(&m_mtvectInfiDefence);
			///////////////////////////////////////////////////////////////////////////////
			// 2010-04-05 by cmkwon, 인피 재입장 카드 관련 시스템 수정 - 
			CInfinityDefence *pInfinity = this->FindDefenceNoLock(i_CreateUID);
			if(pInfinity)
			{
				return pInfinity->ImputeInfinity();
			}
		} // case INFINITY_MODE_DEFENCE
		break;
	case INFINITY_MODE_MSHIPBATTLE:			// 2011-02-18 by hskim, 인피니티 3차 - 방 생성 작업
		{
			mt_auto_lock mta(&m_mtvectInfiMShipBattle);

			CInfinityMShipBattle *pInfinity = this->FindMShipBattleNoLock(i_CreateUID);
			if(pInfinity)
			{
				return pInfinity->ImputeInfinity();
			}
		} // case INFINITY_MODE_MSHIPBATTLE
		break;
	default :
		{
			return ERR_INFINITY_MODE;
		} // default
	}
	
	return ERR_INFINITY_CREATEUID;
}


// 2010. 05. 19 by hsLee 인피니티 필드 2차 난이도 조절. (신호처리 + 몬스터 처리(서버) )
/******************************************************************************************************************
**
**	인피니티 - 방에 난이도 레벨 변경.
**
**	Create Info :	2010. 05. 26. by hsLee.
**
**	Update Info :	
**
*******************************************************************************************************************/
Err_t CInfinityManager :: ChangeInfinityDifficultyLevel ( const INT i_cst_ChangeDifficultyLevel , eINFINITY_MODE i_eInfiMode , InfinityCreateUID_t i_CreateUID , CFieldIOCPSocket * i_pMasterUserFISoc )
{

	if( NULL == i_pMasterUserFISoc )
		return ERR_INFINITY_NULL_VALUE;
	
	switch ( i_eInfiMode )
	{

		case INFINITY_MODE_BOSSRUSH: 
			{
				mt_auto_lock mta ( &m_mtvectInfiBossRush );
				mtvectInfiBossRush::iterator itr = m_mtvectInfiBossRush.begin();
				for( ; itr != m_mtvectInfiBossRush.end(); itr++ ) 
				{ 
					if ( i_CreateUID == (*itr)->GetInfinityCreateUID() )
					{
						return (*itr)->ChangeDifficultyLevel( i_pMasterUserFISoc , i_cst_ChangeDifficultyLevel );
					}
				}
				mta.auto_unlock_cancel();
			} 
			break; 

		case INFINITY_MODE_DEFENCE: 
			{
				mt_auto_lock mta ( &m_mtvectInfiDefence );
				mtvectInfiDefence::iterator itr = m_mtvectInfiDefence.begin();
				for( ; itr != m_mtvectInfiDefence.end(); itr++ )
				{ 
					if ( i_CreateUID == (*itr)->GetInfinityCreateUID() ) 
					{
						return (*itr)->ChangeDifficultyLevel ( i_pMasterUserFISoc , i_cst_ChangeDifficultyLevel );
					}
				}
				mta.auto_unlock_cancel();
			}
			break;

		case INFINITY_MODE_MSHIPBATTLE:			// 2011-02-18 by hskim, 인피니티 3차 - 방 생성 작업
			{
				mt_auto_lock mta ( &m_mtvectInfiMShipBattle );
				mtvectInfiMShipBattle::iterator itr = m_mtvectInfiMShipBattle.begin();
				for( ; itr != m_mtvectInfiMShipBattle.end(); itr++ )
				{ 
					if ( i_CreateUID == (*itr)->GetInfinityCreateUID() ) 
					{
						return (*itr)->ChangeDifficultyLevel ( i_pMasterUserFISoc , i_cst_ChangeDifficultyLevel );
					}
				}
				mta.auto_unlock_cancel();
			}
			break;
	}	
	
	return ERR_INFINITY_DIFFICULTY_LEVEL_INVALID;

}

// End 2010. 05. 19 by hsLee 인피니티 필드 2차 난이도 조절. (신호처리 + 몬스터 처리(서버) )


// 2010. 07. 27 by hsLee 인피니티 2차 거점 방어 시네마 연출 스킵 처리.
/******************************************************************************************************************************
**
**	인피니티 엔딩 시네마 연출 스킵 활성화. 
**
**	Create Info :	2010. 07. 27.	by hsLee.
**
**	Update Info :	시네마 연출 스킵 처리에 정상 연출 종료에 대한 처리 추가. 'a_bNormalEnding'	2010. 08. 26. by hsLee.
**
*******************************************************************************************************************************/
Err_t CInfinityManager :: InfinitySkipEndingCinema ( eINFINITY_MODE i_eInfiMode , InfinityCreateUID_t i_CreateUID , CFieldIOCPSocket * i_pPlayerFISoc , const bool a_bNormalEnding /*= false*/ )
{

	if( NULL == i_pPlayerFISoc )
		return ERR_INFINITY_NULL_VALUE;
	
	switch ( i_eInfiMode )
	{
		
		case INFINITY_MODE_BOSSRUSH: 
			{
				mt_auto_lock mta ( &m_mtvectInfiBossRush );
				mtvectInfiBossRush::iterator itr = m_mtvectInfiBossRush.begin();
				for( ; itr != m_mtvectInfiBossRush.end(); itr++ ) 
				{ 
					if ( i_CreateUID == (*itr)->GetInfinityCreateUID() )
					{
						return (*itr)->SkipEndingCinema ( i_pPlayerFISoc , a_bNormalEnding );
					}
				}
				mta.auto_unlock_cancel();
			} 
			break; 
			
		case INFINITY_MODE_DEFENCE: 
			{
				mt_auto_lock mta ( &m_mtvectInfiDefence );
				mtvectInfiDefence::iterator itr = m_mtvectInfiDefence.begin();
				for( ; itr != m_mtvectInfiDefence.end(); itr++ )
				{ 
					if ( i_CreateUID == (*itr)->GetInfinityCreateUID() ) 
					{
						return (*itr)->SkipEndingCinema ( i_pPlayerFISoc , a_bNormalEnding );
					}
				}
				mta.auto_unlock_cancel();
			}
			break;

		case INFINITY_MODE_MSHIPBATTLE:			// 2011-02-18 by hskim, 인피니티 3차 - 방 생성 작업
			{
				mt_auto_lock mta ( &m_mtvectInfiMShipBattle );
				mtvectInfiMShipBattle::iterator itr = m_mtvectInfiMShipBattle.begin();
				for( ; itr != m_mtvectInfiMShipBattle.end(); itr++ )
				{ 
					if ( i_CreateUID == (*itr)->GetInfinityCreateUID() ) 
					{
						return (*itr)->SkipEndingCinema ( i_pPlayerFISoc , a_bNormalEnding );
					}
				}
				mta.auto_unlock_cancel();
			}
			break;
	}
	
	return ERR_INFINITY_MISMATCH_CREATEUID;

}


void CInfinityManager::SetDisConnectUserInfo(INFINITY_DISCONNECTUSER_INFO * i_pDisConnectUserInfo)
{
	if ( NULL == i_pDisConnectUserInfo ) 
	{
		// 2009-09-09 ~ 2010-01 by dhjin, 인피니티 - 소스 체크
		return;
	}

	// 2009-09-09 ~ 2010-01 by dhjin, 인피니티 - 소스 체크
	mt_auto_lock mta(&m_mtInfinityDisConnectUserList);
	mtvectorInfinityDisConnectUser::iterator itr = m_mtInfinityDisConnectUserList.begin();

	for(; itr != m_mtInfinityDisConnectUserList.end(); itr++)
	{
		if ( 0 == strnicmp(i_pDisConnectUserInfo->CharacterName, itr->CharacterName, SIZE_MAX_CHARACTER_NAME) )
		{
			// 만약 기존에 팅긴 정보가 존재한다면!! 업뎃해주자~~!~!!!
			itr->InfinityCreateUID	= i_pDisConnectUserInfo->InfinityCreateUID;
			itr->InfinityMode		= i_pDisConnectUserInfo->InfinityMode;
			return;
		}
	}

	// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 팅긴 유저 재접속 처리, 인피 튕긴 유저일 경우 정보 저장.
	m_mtInfinityDisConnectUserList.push_back(*i_pDisConnectUserInfo);
}

Err_t CInfinityManager::DisConnectUserReStart(char * i_DisConnectUserName, INFINITY_PLAYING_INFO * o_pInfinityPlayingInfo)
{
	if(	NULL == i_DisConnectUserName
		|| NULL == o_pInfinityPlayingInfo ) 
	{
		// 2009-09-09 ~ 2010-01 by dhjin, 인피니티 - 소스 체크
		return ERR_INFINITY_NULL_VALUE;
	}

	// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 팅긴 유저 재접속 처리, 인피 튕긴 유저 시작 처리
	mt_auto_lock mta(&m_mtInfinityDisConnectUserList);
	mtvectorInfinityDisConnectUser::iterator itr = m_mtInfinityDisConnectUserList.begin();

	for(; itr != m_mtInfinityDisConnectUserList.end(); itr++)
	{
		if ( 0 == strnicmp(i_DisConnectUserName, itr->CharacterName, SIZE_MAX_CHARACTER_NAME) )
		{
			o_pInfinityPlayingInfo->InfinityCreateUID	= itr->InfinityCreateUID;
			o_pInfinityPlayingInfo->ModeTypeNum			= itr->InfinityMode;
			return this->CheckIsCreateInfinityUID(o_pInfinityPlayingInfo);
		}
	}

	return ERR_INFINITY_CANNOT_SUCH_TEAM;
}


Err_t CInfinityManager::CheckIsCreateInfinityUID(INFINITY_PLAYING_INFO * o_pInfinityPlayingInfo) 
{
	if ( NULL == o_pInfinityPlayingInfo ) 
	{
		// 2009-09-09 ~ 2010-01 by dhjin, 인피니티 - 소스 체크
		return ERR_INFINITY_NULL_VALUE;
	}

	// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 팅긴 유저 재접속 처리
	switch ( o_pInfinityPlayingInfo->ModeTypeNum ) 
	{
		case INFINITY_MODE_BOSSRUSH: 
			{
				mt_auto_lock mta ( &m_mtvectInfiBossRush );
				mtvectInfiBossRush::iterator itr = m_mtvectInfiBossRush.begin();

				for(; itr != m_mtvectInfiBossRush.end(); itr++) 
				{ 
					if ( o_pInfinityPlayingInfo->InfinityCreateUID == (*itr)->GetInfinityCreateUID() ) 
					{
						o_pInfinityPlayingInfo->MapIdx = (*itr)->GetInfinityMapIndex();
						return ERR_NO_ERROR;
					}
				}

				mta.auto_unlock_cancel();

			} // case INFINITY_MODE_BOSSRUSH
			break; 

		case INFINITY_MODE_DEFENCE: 
			{
				mt_auto_lock mta ( &m_mtvectInfiDefence );
				mtvectInfiDefence::iterator itr = m_mtvectInfiDefence.begin();

				for(; itr != m_mtvectInfiDefence.end(); itr++) 
				{ 
					if( o_pInfinityPlayingInfo->InfinityCreateUID == (*itr)->GetInfinityCreateUID() ) 
					{
						o_pInfinityPlayingInfo->MapIdx = (*itr)->GetInfinityMapIndex();
						return ERR_NO_ERROR;
					}
				}

				mta.auto_unlock_cancel();

			} // case INFINITY_MODE_DEFENCE
			break;

		case INFINITY_MODE_MSHIPBATTLE:			// 2011-02-18 by hskim, 인피니티 3차 - 방 생성 작업
			{
				mt_auto_lock mta ( &m_mtvectInfiMShipBattle );
				mtvectInfiMShipBattle::iterator itr = m_mtvectInfiMShipBattle.begin();
				
				for(; itr != m_mtvectInfiMShipBattle.end(); itr++) 
				{ 
					if( o_pInfinityPlayingInfo->InfinityCreateUID == (*itr)->GetInfinityCreateUID() ) 
					{
						o_pInfinityPlayingInfo->MapIdx = (*itr)->GetInfinityMapIndex();
						return ERR_NO_ERROR;
					}
				}
				
				mta.auto_unlock_cancel();
				
			} // case INFINITY_MODE_MSHIPBATTLE
			break;

		default : 
			return ERR_INFINITY_CANNOT_SUCH_TEAM;
	}

	
	return ERR_INFINITY_CANNOT_SUCH_TEAM;
}


BOOL CInfinityManager::DeleteDisConnectUserInfo(char * i_DisConnectUserName)
{
	if ( NULL == i_DisConnectUserName ) 
	{
		// 2009-09-09 ~ 2010-01 by dhjin, 인피니티 - 소스 체크
		return FALSE;
	}

	// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 팅긴 유저 재접속 처리, 인피 튕긴 유저일 경우 정보 삭제.
	mt_auto_lock mta(&m_mtInfinityDisConnectUserList);

	mtvectorInfinityDisConnectUser::iterator itr = m_mtInfinityDisConnectUserList.begin();

	for(; itr != m_mtInfinityDisConnectUserList.end(); itr++)
	{
		if( 0 == strnicmp(i_DisConnectUserName, itr->CharacterName, SIZE_MAX_CHARACTER_NAME) )
		{
			m_mtInfinityDisConnectUserList.erase(itr);
			return TRUE;
		}
	}
	return FALSE;
}

void CInfinityManager::ReStartDisConnectUserW(InfinityCreateUID_t i_nInfinityCreateUID, eINFINITY_MODE i_eInfiMode, CFieldIOCPSocket * i_pUserFISoc) 
{

	if ( NULL == i_pUserFISoc )
	{
		// 2009-09-09 ~ 2010-01 by dhjin, 인피니티 - 소스 체크
		return;
	}
	
	// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 팅긴 유저 재접속 처리 고고씽
	switch ( i_eInfiMode )
	{
		case INFINITY_MODE_BOSSRUSH: 
			{
				mt_auto_lock mta( &m_mtvectInfiBossRush );
				mtvectInfiBossRush::iterator itr = m_mtvectInfiBossRush.begin();
				for(; itr != m_mtvectInfiBossRush.end(); itr++) 
				{ 
					if( i_nInfinityCreateUID == (*itr)->GetInfinityCreateUID() ) 
					{
						(*itr)->ReStartDisConnectUser(i_pUserFISoc);
						return;
					}
				}
				mta.auto_unlock_cancel();
			} // case INFINITY_MODE_BOSSRUSH
			break; 

		case INFINITY_MODE_DEFENCE: 
			{
				mt_auto_lock mta( &m_mtvectInfiDefence );
				mtvectInfiDefence::iterator itr = m_mtvectInfiDefence.begin();
				for(; itr != m_mtvectInfiDefence.end(); itr++) 
				{ 
					if( i_nInfinityCreateUID == (*itr)->GetInfinityCreateUID() ) 
					{
						(*itr)->ReStartDisConnectUser( i_pUserFISoc );
						return;
					}
				}
				mta.auto_unlock_cancel();
			} // case INFINITY_MODE_DEFENCE
			break;

		case INFINITY_MODE_MSHIPBATTLE:				// 2011-02-18 by hskim, 인피니티 3차 - 방 생성 작업
			{
				mt_auto_lock mta( &m_mtvectInfiMShipBattle );
				mtvectInfiMShipBattle::iterator itr = m_mtvectInfiMShipBattle.begin();
				for(; itr != m_mtvectInfiMShipBattle.end(); itr++) 
				{ 
					if( i_nInfinityCreateUID == (*itr)->GetInfinityCreateUID() ) 
					{
						(*itr)->ReStartDisConnectUser( i_pUserFISoc );
						return;
					}
				}
				mta.auto_unlock_cancel();
			} // case INFINITY_MODE_DEFENCE
			break;

		default : 
			return;
	}	
	
	return;
}


void CInfinityManager::CreateKeyMonster_AliveForGameClearW(MapIndex_t i_MapIndex, ChannelIndex_t i_ChannelIdx, MonIdx_t i_CreateMonsterIdx)
{
	mt_auto_lock mtD(&m_mtvectInfiDefence);
	mtvectInfiDefence::iterator itrD = m_mtvectInfiDefence.begin();
	for(; itrD != m_mtvectInfiDefence.end(); itrD++) {
		if(i_MapIndex == (*itrD)->GetInfinityMapIndex() 
			&& i_ChannelIdx == (*itrD)->GetInfinityChannelIndex()) {
			(*itrD)->CreateKeyMonster_AliveForGameClear(i_CreateMonsterIdx);
			return;
		}
	}
	mtD.auto_unlock_cancel();

	// start 2011-06-05 by hskim, 인피니티 3차 - 키몬스터 삭제시 종료 처리
	mt_auto_lock mtT(&m_mtvectInfiMShipBattle);
	mtvectInfiMShipBattle::iterator itrT = m_mtvectInfiMShipBattle.begin();
	for(; itrT != m_mtvectInfiMShipBattle.end(); itrT++) {
		if(i_MapIndex == (*itrT)->GetInfinityMapIndex() 
			&& i_ChannelIdx == (*itrT)->GetInfinityChannelIndex()) {
			(*itrT)->CreateKeyMonster_AliveForGameClear(i_CreateMonsterIdx);
			return;
		}
	}
	mtT.auto_unlock_cancel();
	// end 2011-06-05 by hskim, 인피니티 3차 - 키몬스터 삭제시 종료 처리
}

void CInfinityManager::DeleteKeyMonster_AliveForGameClearW(MapIndex_t i_MapIndex, ChannelIndex_t i_ChannelIdx, MonIdx_t i_DeadMonsterIdx)
{
	mt_auto_lock mtD(&m_mtvectInfiDefence);
	mtvectInfiDefence::iterator itrD = m_mtvectInfiDefence.begin();
	for(; itrD != m_mtvectInfiDefence.end(); itrD++) {
		if(i_MapIndex == (*itrD)->GetInfinityMapIndex() 
			&& i_ChannelIdx == (*itrD)->GetInfinityChannelIndex()) {
			(*itrD)->DeleteKeyMonster_AliveForGameClear(i_DeadMonsterIdx);
			return;
		}
	}
	mtD.auto_unlock_cancel();

	// start 2011-06-05 by hskim, 인피니티 3차 - 키몬스터 삭제시 종료 처리
	mt_auto_lock mtT(&m_mtvectInfiMShipBattle);
	mtvectInfiMShipBattle::iterator itrT = m_mtvectInfiMShipBattle.begin();
	for(; itrT != m_mtvectInfiMShipBattle.end(); itrT++) {
		if(i_MapIndex == (*itrT)->GetInfinityMapIndex()
			&& i_ChannelIdx == (*itrT)->GetInfinityChannelIndex()) {
			(*itrT)->DeleteKeyMonster_AliveForGameClear(i_DeadMonsterIdx);
			return;
		}
	}
	mtT.auto_unlock_cancel();
	// end 2011-06-05 by hskim, 인피니티 3차 - 키몬스터 삭제시 종료 처리
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2010-03-23 by cmkwon, 인피니티 입장 캐쉬 아이템 구현 - 
/// \author		cmkwon
/// \date		2010-03-23 ~ 2010-03-23
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
Err_t CInfinityManager::SendFtoA_INFINITY_START_CHECK_W(eINFINITY_MODE i_eInfiMode, InfinityCreateUID_t i_CreateUID)
{	
	switch (i_eInfiMode)
	{
	case INFINITY_MODE_BOSSRUSH:
		{
			mt_auto_lock mta(&m_mtvectInfiBossRush);
			mtvectInfiBossRush::iterator itr = m_mtvectInfiBossRush.begin();
			for(; itr != m_mtvectInfiBossRush.end(); itr++)
			{ 
				if(i_CreateUID == (*itr)->GetInfinityCreateUID())
				{
					return (*itr)->SendFtoA_INFINITY_START_CHECK();
				}
			}
			mta.auto_unlock_cancel();
		}// case INFINITY_MODE_BOSSRUSH
		break; 
	case INFINITY_MODE_DEFENCE: {
		mt_auto_lock mta(&m_mtvectInfiDefence);
		mtvectInfiDefence::iterator itr = m_mtvectInfiDefence.begin();
		for(; itr != m_mtvectInfiDefence.end(); itr++)
		{ 
			if(i_CreateUID == (*itr)->GetInfinityCreateUID())
			{
				return (*itr)->SendFtoA_INFINITY_START_CHECK();
			}
		}
		mta.auto_unlock_cancel();
								} // case INFINITY_MODE_DEFENCE
		break;
	case INFINITY_MODE_MSHIPBATTLE: {					// 2011-02-18 by hskim, 인피니티 3차 - 방 생성 작업
		mt_auto_lock mta(&m_mtvectInfiMShipBattle);
		mtvectInfiMShipBattle::iterator itr = m_mtvectInfiMShipBattle.begin();
		for(; itr != m_mtvectInfiMShipBattle.end(); itr++)
		{ 
			if(i_CreateUID == (*itr)->GetInfinityCreateUID())
			{
				return (*itr)->SendFtoA_INFINITY_START_CHECK();
			}
		}
		mta.auto_unlock_cancel();
								} // case INFINITY_MODE_MSHIPBATTLE
		break;
	default : {
		return ERR_INFINITY_MODE;
			  } // default
	}	
	
	return ERR_INFINITY_CREATEUID;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2010-03-23 by cmkwon, 인피니티 입장 캐쉬 아이템 구현 - 
/// \author		cmkwon
/// \date		2010-03-23 ~ 2010-03-23
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
CFieldIOCPSocket *CInfinityManager::GetMasterPlayerW(eINFINITY_MODE i_eInfiMode, InfinityCreateUID_t i_CreateUID)
{
	switch (i_eInfiMode)
	{
	case INFINITY_MODE_BOSSRUSH:
		{
			mt_auto_lock mta(&m_mtvectInfiBossRush);
			mtvectInfiBossRush::iterator itr = m_mtvectInfiBossRush.begin();
			for(; itr != m_mtvectInfiBossRush.end(); itr++)
			{ 
				if(i_CreateUID == (*itr)->GetInfinityCreateUID())
				{
					return (*itr)->GetMasterPlayer();
				}
			}
			mta.auto_unlock_cancel();
		}// case INFINITY_MODE_BOSSRUSH
		break; 
	case INFINITY_MODE_DEFENCE: {
		mt_auto_lock mta(&m_mtvectInfiDefence);
		mtvectInfiDefence::iterator itr = m_mtvectInfiDefence.begin();
		for(; itr != m_mtvectInfiDefence.end(); itr++)
		{ 
			if(i_CreateUID == (*itr)->GetInfinityCreateUID())
			{
				return (*itr)->GetMasterPlayer();
			}
		}
		mta.auto_unlock_cancel();
								} // case INFINITY_MODE_DEFENCE
		break;
	case INFINITY_MODE_MSHIPBATTLE: {				// 2011-02-18 by hskim, 인피니티 3차 - 방 생성 작업
		mt_auto_lock mta(&m_mtvectInfiMShipBattle);
		mtvectInfiMShipBattle::iterator itr = m_mtvectInfiMShipBattle.begin();
		for(; itr != m_mtvectInfiMShipBattle.end(); itr++)
		{ 
			if(i_CreateUID == (*itr)->GetInfinityCreateUID())
			{
				return (*itr)->GetMasterPlayer();
			}
		}
		mta.auto_unlock_cancel();
								} // case INFINITY_MODE_MSHIPBATTLE
		break;
	default : {
		return NULL;
			  } // default
	}	
	
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2010-03-23 by cmkwon, 인피니티 입장 캐쉬 아이템 구현 - 
/// \author		cmkwon
/// \date		2010-03-23 ~ 2010-03-23
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CInfinityManager::CheckInfinityStartCheckAckW(eINFINITY_STATE *o_pInfiRoomState, MSG_FtoA_INFINITY_START_CHECK_ACK *i_pStartCheckAck)
{
	switch (i_pStartCheckAck->InfinityMode)
	{
	case INFINITY_MODE_BOSSRUSH:
		{
			mt_auto_lock mta(&m_mtvectInfiBossRush);
			mtvectInfiBossRush::iterator itr = m_mtvectInfiBossRush.begin();
			for(; itr != m_mtvectInfiBossRush.end(); itr++)
			{ 
				if(i_pStartCheckAck->InfinityCreateUID == (*itr)->GetInfinityCreateUID())
				{
					return (*itr)->CheckInfinityStartCheckAck(o_pInfiRoomState, i_pStartCheckAck);
				}
			}
			mta.auto_unlock_cancel();
		}// case INFINITY_MODE_BOSSRUSH
		break; 
	case INFINITY_MODE_DEFENCE:
		{
			mt_auto_lock mta(&m_mtvectInfiDefence);
			mtvectInfiDefence::iterator itr = m_mtvectInfiDefence.begin();
			for(; itr != m_mtvectInfiDefence.end(); itr++)
			{ 
				if(i_pStartCheckAck->InfinityCreateUID == (*itr)->GetInfinityCreateUID())
				{
					return (*itr)->CheckInfinityStartCheckAck(o_pInfiRoomState, i_pStartCheckAck);
				}
			}
			mta.auto_unlock_cancel();
		} // case INFINITY_MODE_DEFENCE
		break;
	case INFINITY_MODE_MSHIPBATTLE:			// 2011-02-18 by hskim, 인피니티 3차 - 방 생성 작업
		{
			mt_auto_lock mta(&m_mtvectInfiMShipBattle);
			mtvectInfiMShipBattle::iterator itr = m_mtvectInfiMShipBattle.begin();
			for(; itr != m_mtvectInfiMShipBattle.end(); itr++)
			{ 
				if(i_pStartCheckAck->InfinityCreateUID == (*itr)->GetInfinityCreateUID())
				{
					return (*itr)->CheckInfinityStartCheckAck(o_pInfiRoomState, i_pStartCheckAck);
				}
			}
			mta.auto_unlock_cancel();
		} // case INFINITY_MODE_MSHIPBATTLE
		break;
	default :
		{
			return FALSE;
		} // default
	}	
	
	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2010-03-23 by cmkwon, 인피니티 입장 캐쉬 아이템 구현 - 
/// \author		cmkwon
/// \date		2010-03-23 ~ 2010-03-23
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CInfinityManager::SetAllPlayerStateW(eINFINITY_MODE i_eInfiMode, InfinityCreateUID_t i_CreateUID, eINFINITY_STATE i_InfiState)
{
	switch (i_eInfiMode)
	{
	case INFINITY_MODE_BOSSRUSH:
		{
			mt_auto_lock mta(&m_mtvectInfiBossRush);
			mtvectInfiBossRush::iterator itr = m_mtvectInfiBossRush.begin();
			for(; itr != m_mtvectInfiBossRush.end(); itr++)
			{ 
				if(i_CreateUID == (*itr)->GetInfinityCreateUID())
				{
					(*itr)->SetAllPlayerState(i_InfiState);
					return;
				}
			}
			mta.auto_unlock_cancel();
		}// case INFINITY_MODE_BOSSRUSH
		break; 
	case INFINITY_MODE_DEFENCE: {
		mt_auto_lock mta(&m_mtvectInfiDefence);
		mtvectInfiDefence::iterator itr = m_mtvectInfiDefence.begin();
		for(; itr != m_mtvectInfiDefence.end(); itr++)
		{ 
			if(i_CreateUID == (*itr)->GetInfinityCreateUID())
			{
				(*itr)->SetAllPlayerState(i_InfiState);
				return;
			}
		}
		mta.auto_unlock_cancel();
								} // case INFINITY_MODE_DEFENCE
		break;
	case INFINITY_MODE_MSHIPBATTLE: {				// 2011-02-18 by hskim, 인피니티 3차 - 방 생성 작업
		mt_auto_lock mta(&m_mtvectInfiMShipBattle);
		mtvectInfiMShipBattle::iterator itr = m_mtvectInfiMShipBattle.begin();
		for(; itr != m_mtvectInfiMShipBattle.end(); itr++)
		{ 
			if(i_CreateUID == (*itr)->GetInfinityCreateUID())
			{
				(*itr)->SetAllPlayerState(i_InfiState);
				return;
			}
		}
		mta.auto_unlock_cancel();
								} // case INFINITY_MODE_MSHIPBATTLE
		break;
	default : {
		return;
			  } // default
	}	
	
	return;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		cmkwon
/// \date		2010-04-05 ~ 2010-04-05
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
CInfinityBossrush *CInfinityManager::FindBossrushNoLock(InfinityCreateUID_t i_CreateUID)
{
	mtvectInfiBossRush::iterator itr(m_mtvectInfiBossRush.begin());
	for(; itr != m_mtvectInfiBossRush.end(); itr++)
	{
		CInfinityBossrush *pInfinity = *itr;
		if(i_CreateUID == pInfinity->GetInfinityCreateUID())
		{			
			return pInfinity;
		}
	}	
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		cmkwon
/// \date		2010-04-05 ~ 2010-04-05
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
CInfinityDefence *CInfinityManager::FindDefenceNoLock(InfinityCreateUID_t i_CreateUID)
{
	mtvectInfiDefence::iterator itr(m_mtvectInfiDefence.begin());
	for(; itr != m_mtvectInfiDefence.end(); itr++)
	{
		CInfinityDefence *pInfinity = *itr;
		if(i_CreateUID == pInfinity->GetInfinityCreateUID())
		{			
			return pInfinity;
		}
	}	
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		2011-02-18 by hskim, 인피니티 3차 - 방 생성 작업
/// \author		hskim
/// \date		2011-02-18
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
CInfinityMShipBattle *CInfinityManager::FindMShipBattleNoLock(InfinityCreateUID_t i_CreateUID)			
{
	// 2011-02-18 by hskim, 인피니티 3차 - 방 생성 작업
	
	mtvectInfiMShipBattle::iterator itr(m_mtvectInfiMShipBattle.begin());
	for(; itr != m_mtvectInfiMShipBattle.end(); itr++)
	{
		CInfinityMShipBattle *pInfinity = *itr;
		if(i_CreateUID == pInfinity->GetInfinityCreateUID())
		{			
			return pInfinity;
		}
	}	
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2010-04-09 by cmkwon, 인피2차 추가 수정(단계별 보상 추가) - 
/// \author		cmkwon
/// \date		2010-04-09 ~ 2010-04-09
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
CInfinityBossrush *CInfinityManager::FindBossrushNoLock(MapIndex_t i_MapIndex, ChannelIndex_t i_ChannelIdx)
{
	mtvectInfiBossRush::iterator itr(m_mtvectInfiBossRush.begin());
	for(; itr != m_mtvectInfiBossRush.end(); itr++)
	{
		CInfinityBossrush *pInfinity = *itr;
		if(i_MapIndex == pInfinity->GetInfinityMapIndex()
			&& i_ChannelIdx == pInfinity->GetInfinityChannelIndex())
		{
			return pInfinity;
		}
	}
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2010-04-09 by cmkwon, 인피2차 추가 수정(단계별 보상 추가) - 
/// \author		cmkwon
/// \date		2010-04-09 ~ 2010-04-09
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
CInfinityDefence *CInfinityManager::FindDefenceNoLock(MapIndex_t i_MapIndex, ChannelIndex_t i_ChannelIdx)
{
	mtvectInfiDefence::iterator itr(m_mtvectInfiDefence.begin());
	for(; itr != m_mtvectInfiDefence.end(); itr++)
	{
		CInfinityDefence *pInfinity = *itr;
		if(i_MapIndex == pInfinity->GetInfinityMapIndex()
			&& i_ChannelIdx == pInfinity->GetInfinityChannelIndex())
		{			
			return pInfinity;
		}
	}	
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2010-04-09 by cmkwon, 인피2차 추가 수정(단계별 보상 추가) -> 2011-02-18 by hskim, 인피니티 3차 - 방 생성 작업
/// \author		hskim
/// \date		2011-02-18
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
CInfinityMShipBattle *CInfinityManager::FindMShipBattleNoLock(MapIndex_t i_MapIndex, ChannelIndex_t i_ChannelIdx)
{
	// 2011-02-18 by hskim, 인피니티 3차 - 방 생성 작업

	mtvectInfiMShipBattle::iterator itr(m_mtvectInfiMShipBattle.begin());
	for(; itr != m_mtvectInfiMShipBattle.end(); itr++)
	{
		CInfinityMShipBattle *pInfinity = *itr;
		if(i_MapIndex == pInfinity->GetInfinityMapIndex()
			&& i_ChannelIdx == pInfinity->GetInfinityChannelIndex())
		{			
			return pInfinity;
		}
	}	
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2010-04-09 by cmkwon, 인피2차 추가 수정(단계별 보상 추가) - 
/// \author		cmkwon
/// \date		2010-04-12 ~ 2010-04-12
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
Err_t CInfinityManager::PushTenderItemW(MapIndex_t i_MapIndex, ChannelIndex_t i_ChannelIdx, CTenderItemInfo *i_pTenderItemInfo)
{
	///////////////////////////////////////////////////////////////////////////////
	// 2010-04-09 by cmkwon, 인피2차 추가 수정(단계별 보상 추가) - 
	{
		mt_auto_lock mtA(&m_mtvectInfiBossRush);
		CInfinityBossrush *pInfinity = this->FindBossrushNoLock(i_MapIndex, i_ChannelIdx);
		if(pInfinity)
		{
			return pInfinity->PushTenderItem(i_pTenderItemInfo);
		}
	}
	{
		mt_auto_lock mtA(&m_mtvectInfiDefence);
		CInfinityDefence *pInfinity = this->FindDefenceNoLock(i_MapIndex, i_ChannelIdx);
		if(pInfinity)
		{
			return pInfinity->PushTenderItem(i_pTenderItemInfo);
		}
	}
	{			// 2011-02-18 by hskim, 인피니티 3차 - 방 생성 작업
		mt_auto_lock mtA(&m_mtvectInfiMShipBattle);
		CInfinityMShipBattle *pInfinity = this->FindMShipBattleNoLock(i_MapIndex, i_ChannelIdx);
		if(pInfinity)
		{
			return pInfinity->PushTenderItem(i_pTenderItemInfo);
		}
	}
	return ERR_INFINITY_CREATEUID;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2010-04-06 by cmkwon, 인피2차 추가 수정 - 
/// \author		cmkwon
/// \date		2010-04-06 ~ 2010-04-06
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
Err_t CInfinityManager::CheckInfinityAllPlayerStateW(eINFINITY_MODE i_eInfiMode, InfinityCreateUID_t i_CreateUID, eINFINITY_STATE i_infiState)
{
	// 인피 시작
	switch (i_eInfiMode)
	{
	case INFINITY_MODE_BOSSRUSH:
		{
			mt_auto_lock mta(&m_mtvectInfiBossRush);
			///////////////////////////////////////////////////////////////////////////////
			// 2010-04-05 by cmkwon, 인피 재입장 카드 관련 시스템 수정 - 
			CInfinityBossrush *pInfinity = this->FindBossrushNoLock(i_CreateUID);
			if(pInfinity)
			{
//				g_pFieldGlobal->WriteSystemLogEX(TRUE, "[TEMP] [Infinity][%I64d] CInfinityManager::CheckInfinityAllPlayerStateW# 10000 !, InfiState(%d) Cnt(%d) \r\n"
//					, pInfinity->GetInfinityCreateUID(), pInfinity->GetInfinityState(), pInfinity->GetPlayerListSize());

				// 2012-07-17 by hskim, 인피니티 인원수 부족할대 START 할경우 예외 처리
				if( FALSE == pInfinity->CheckInfinityMinAdmission() )
				{
					return ERR_INFINITY_MIN_ADMISSIONCNT;
				}
				// end 2012-07-17 by hskim, 인피니티 인원수 부족할대 START 할경우 예외 처리

				if(FALSE == pInfinity->CheckInfinityAllPlayerState(i_infiState))
				{
					return ERR_INFINITY_NOT_ALL_READY;
				}
				return ERR_NO_ERROR;
			}
		} // case INFINITY_MODE_BOSSRUSH
		break; 
	case INFINITY_MODE_DEFENCE:
		{
			mt_auto_lock mta(&m_mtvectInfiDefence);
			///////////////////////////////////////////////////////////////////////////////
			// 2010-04-05 by cmkwon, 인피 재입장 카드 관련 시스템 수정 - 
			CInfinityDefence *pInfinity = this->FindDefenceNoLock(i_CreateUID);
			if(pInfinity)
			{
//				g_pFieldGlobal->WriteSystemLogEX(TRUE, "[TEMP] [Infinity][%I64d] CInfinityManager::CheckInfinityAllPlayerStateW# 20000 !, InfiState(%d) Cnt(%d) \r\n"
//					, pInfinity->GetInfinityCreateUID(), pInfinity->GetInfinityState(), pInfinity->GetPlayerListSize());

				// 2012-07-17 by hskim, 인피니티 인원수 부족할대 START 할경우 예외 처리
				if( FALSE == pInfinity->CheckInfinityMinAdmission() )
				{
					return ERR_INFINITY_MIN_ADMISSIONCNT;
				}
				// end 2012-07-17 by hskim, 인피니티 인원수 부족할대 START 할경우 예외 처리

				if(FALSE == pInfinity->CheckInfinityAllPlayerState(i_infiState))
				{
					return ERR_INFINITY_NOT_ALL_READY;
				}
				return ERR_NO_ERROR;
			}
		} // case INFINITY_MODE_DEFENCE
		break;
	case INFINITY_MODE_MSHIPBATTLE:				// 2011-02-18 by hskim, 인피니티 3차 - 방 생성 작업
		{
			mt_auto_lock mta(&m_mtvectInfiMShipBattle);

			CInfinityMShipBattle *pInfinity = this->FindMShipBattleNoLock(i_CreateUID);
			if(pInfinity)
			{				
				// 2012-07-17 by hskim, 인피니티 인원수 부족할대 START 할경우 예외 처리
				if( FALSE == pInfinity->CheckInfinityMinAdmission() )
				{
					return ERR_INFINITY_MIN_ADMISSIONCNT;
				}
				// end 2012-07-17 by hskim, 인피니티 인원수 부족할대 START 할경우 예외 처리

				if(FALSE == pInfinity->CheckInfinityAllPlayerState(i_infiState))
				{
					return ERR_INFINITY_NOT_ALL_READY;
				}
				return ERR_NO_ERROR;
			}
		} // case INFINITY_MODE_MSHIPBATTLE
		break;
	default :
		{
			return ERR_INFINITY_MODE;
		} // default
	}
	
	return ERR_INFINITY_CREATEUID;
}


// 2010. 05. 19 by hsLee 인피니티 필드 2차 난이도 조절. (신호처리 + 몬스터 처리(서버) )
/************************************************************************************************
**
**	인피니티 난이도 조절 - 유저에게 보여줄 난이도 정보 가져오기
**
**	Create Info :	2010-05-13 by shcho
**
**	Update Infi :	인자 포인터로 변경, 결과 값 리턴하도록 수정.	2010. 05. 24. by hsLee.
**
*************************************************************************************************/
BOOL CInfinityManager::Get_Difficulty_BonusInfo_ListData(vectorInfinity_DifficultyInfo_Bonus *p_vecInfinity_DifficulytList) 
{
	if ( NULL == p_vecInfinity_DifficulytList )
		return FALSE;

	p_vecInfinity_DifficulytList->clear();
	p_vecInfinity_DifficulytList->assign( m_vecInfinityDifficultyBonusInfo.begin() , m_vecInfinityDifficultyBonusInfo.end() );

	return TRUE;
}


/**********************************************************************************************
**
**	인피니티 난이도 조절 - 보너스 정보를 저장하기
**
**	Create Info :	2010-05-13 by shcho
**
**	Update Info :	인자명 변경, 결과 값 리턴하도록 수정.	2010. 05. 24. by hsLee.
**
***********************************************************************************************/
BOOL CInfinityManager::Set_Difficulty_BonusInfo_ListData(vectorInfinity_DifficultyInfo_Bonus *p_vecListInfo)
{
	if ( NULL == p_vecListInfo )
		return FALSE;

	m_vecInfinityDifficultyBonusInfo.clear();
	m_vecInfinityDifficultyBonusInfo.assign( p_vecListInfo->begin(), p_vecListInfo->end() );

	return TRUE;
}


/**********************************************************************************************
**
**	인피니티 난이도 조절 - 보너스 정보를 찾는다.
**
**	Create Info :	2010-05-13 by shcho
**
**	Update Info :	인자명 변경, 내부 처리 구현.	2010. 05. 24. by hsLee.
**
***********************************************************************************************/
const INFINITY_DIFFICULTY_BONUS_INFO *CInfinityManager :: Get_Difficulty_BonusInfo ( const int a_iStep )
{

	vectorInfinity_DifficultyInfo_Bonus::iterator it = m_vecInfinityDifficultyBonusInfo.begin();

	while ( it != m_vecInfinityDifficultyBonusInfo.end() )
	{
		if ( it->iIncreaseStep == a_iStep )
			return &*it;

		++it;
	}

	return NULL;

}


/**********************************************************************************************
**
**	인피니티 난이도 조절 - 몬스터 밸런스 정보를 찾는다.
**
**	Create Info :	2010-05-13 by shcho
**
**	Update Info :	인자명 변경, 내부 처리 구현.	2010. 05. 24. by hsLee.
**
***********************************************************************************************/
const INFINITY_DIFFICULTY_MONSTER_SETTING_INFO *CInfinityManager::Get_Difficulty_MonsterInfo(int iStep) 
{
	vectorInfinity_DifficultyInfo_Monster::iterator iter = m_vecInfinityDifficultyMonsterInfo.begin(); // 스텝에 따른 정보를 찾는다.
	
	while ( iter != m_vecInfinityDifficultyMonsterInfo.end() )
	{
		if ( iter->iIncreaseStep == iStep )
			return &*iter;

		++iter;
	}

	return NULL;
}


/**********************************************************************************************
**
**	인피니티 난이도 조절 - 몬스터 밸런스 정보를 저장한다.
**
**	Create Info :	2010-05-13 by shcho
**
**	Update Info :	인자명 변경.	2010. 05. 24. by hsLee.
**
***********************************************************************************************/
BOOL CInfinityManager::Set_Difficulty_MonsterInfo_ListData(vectorInfinity_DifficultyInfo_Monster* p_vecListInfo)
{
	if ( NULL == p_vecListInfo )
		return FALSE;

	m_vecInfinityDifficultyMonsterInfo.clear();
	m_vecInfinityDifficultyMonsterInfo.assign( p_vecListInfo->begin() , p_vecListInfo->end() );

	return TRUE;
}

// End 2010. 05. 19 by hsLee 인피니티 필드 2차 난이도 조절. (신호처리 + 몬스터 처리(서버) )


// 2010. 06. 04 by hsLee 인티피니 필드 2차 난이도 조절. (GM 명령어 추가. /nextscene(다음 시네마 씬 호출.) ) - GM 다음 씨네마 바로 호출.
/**************************************************************************************************************************************************
**
**	인피니티 : GM의 다음 시네마 바로 호출 처리.
**
**	Create Info :	2010. 06. 04 by hsLee.
**
**	Update Info :	인자명 변경.	2010. 05. 24. by hsLee.
**
**					한번 호출로 갱신 가능한 시네마 개수 지정 가능 처리. ( nUpdateSceneCount )	2010. 08. 20. by hsLee.
**
***************************************************************************************************************************************************/
void CInfinityManager :: UpdateNextSceneProc ( eINFINITY_MODE i_eInfiMode , InfinityCreateUID_t i_CreateUID , CFieldIOCPSocket * i_pMasterFISoc , int nUpdateSceneCount /*= 1*/ )
{

	if (!i_pMasterFISoc) return;

	nUpdateSceneCount = max ( nUpdateSceneCount , 1 );

	switch ( i_eInfiMode )
	{
		case INFINITY_MODE_BOSSRUSH :
			{
				mt_auto_lock mtR(&m_mtvectInfiBossRush);

				mtvectInfiBossRush::iterator itrR = m_mtvectInfiBossRush.begin();

				while ( itrR != m_mtvectInfiBossRush.end() )
				{
					if ( (*itrR)->GetInfinityCreateUID() == i_CreateUID )
					{
						(*itrR)->ProcessingCinema( i_pMasterFISoc , nUpdateSceneCount );

						break;
					}

					itrR++;
				}

				mtR.auto_unlock_cancel();
			}
			break;

		case INFINITY_MODE_DEFENCE :
			{
				mt_auto_lock mtD(&m_mtvectInfiDefence);

				mtvectInfiDefence::iterator itrD = m_mtvectInfiDefence.begin();

				while ( itrD != m_mtvectInfiDefence.end() )
				{
					if ( (*itrD)->GetInfinityCreateUID() == i_CreateUID )
					{
						(*itrD)->ProcessingCinema ( i_pMasterFISoc , nUpdateSceneCount );

						break;
					}
					
					itrD++;
				}

				mtD.auto_unlock_cancel();
			}
			break;

		case INFINITY_MODE_MSHIPBATTLE :				// 2011-02-18 by hskim, 인피니티 3차 - 방 생성 작업
			{
				mt_auto_lock mtD(&m_mtvectInfiMShipBattle);
				
				mtvectInfiMShipBattle::iterator itrD = m_mtvectInfiMShipBattle.begin();
				
				while ( itrD != m_mtvectInfiMShipBattle.end() )
				{
					if ( (*itrD)->GetInfinityCreateUID() == i_CreateUID )
					{
						(*itrD)->ProcessingCinema ( i_pMasterFISoc , nUpdateSceneCount );
						
						break;
					}
					
					itrD++;
				}
				
				mtD.auto_unlock_cancel();
			}
			break;
	}
	
}
// End 2010. 06. 04 by hsLee 인티피니 필드 2차 난이도 조절. (GM 명령어 추가. /nextscene(다음 시네마 씬 호출.) ) - GM 다음 씨네마 바로 호출.