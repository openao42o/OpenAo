// QuestData.cpp: implementation of the CQuestData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "QuestData.h"
#include "AtumApplication.h"
#include "QuestLoaderParser.h"
#include "StoreData.h"
#include "ShuttleChild.h"
#include "Interface.h"
#include "FieldWinSocket.h"
#include "INFCityBase.h"
#include "GameDataLast.h"
#include "AtumDatabase.h"
#include "Chat.h"
#include "INFGameMain.h"
#include "INFGameMainChat.h"
#include "INFQuest.h"
// 2008-09-22 by bhsohn EP3 캐릭터 창
//#include "INFCharacterInfo.h"
#include "INFCharacterInfoExtend.h"
#include "INFGameHelpDesk.h"
#include "INFMissionInfo.h"
#include "dxutil.h"

#define TUTORIAL_MISSION_1			1
#define TUTORIAL_MISSION_2			2
#define TUTORIAL_MISSION_3			3
#define TUTORIAL_MISSION_4			4
#define TUTORIAL_MISSION_11			11

#define QUEST_DATA_FILE				"./Res-Tex/quest.tex"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
 
CQuestData::CQuestData()
{
	g_pQuestData = this;
	m_nProgressCharacterQuestNum = 0;
//	QuestLoaderParser quest( QUEST_DATA_FILE, &m_mapQuest );
//	if(!quest.Parse())
//	if(!QuestLoaderParser::ParseFile( QUEST_DATA_FILE, &m_mapQuest ))

	// 2007-07-09 by bhsohn 퀘스트 정보 리드 방식 변경
	m_vectCQuestInfoList.clear();

	CGameData gameData;
	char buf[64];
	DBGOUT("## Quest ###############################################\n");
	strcpy( buf, "alkdjh flds flsjadhf ;kjdfha;sdflkha;s33" );
	if( !gameData.SetFile(QUEST_DATA_FILE, TRUE, buf, strlen(buf)) )
	{
		DBGOUT("Can't decoding : Quest File.\n");//퀘스트 파일을 디코딩할 수 없습니다.
		return;
	}

	DataHeader* pDefHeader = gameData.GetStartPosition();
	if(!pDefHeader)
	{
		DBGOUT("Can't Find Quest File.\n");//퀘스트 디피니션 파일이 없습니다.
		return;
	}
	DataHeader* pDataHeader = gameData.GetNext();
	while(pDataHeader)
	{
		// 2007-07-09 by bhsohn 퀘스트 정보 리드 방식 변경
		if(!QuestLoaderParser::ParseString( pDefHeader->m_pData, 
											pDefHeader->m_DataSize, 
											pDataHeader->m_pData, 
											pDataHeader->m_DataSize, 
											&m_mapQuest,
											&m_vectCQuestInfoList))
		{
			DBGOUT("Can't Pasing Quest File.\n");
			return;
		}
		pDataHeader = gameData.GetNext();
	}
#ifdef _DEBUG
//	CMapQuestIterator it = m_mapQuest.begin();
	DBGOUT("-------------------------QuestData Load----------------------------\n");
//	while(it != m_mapQuest.end() )
//	{
//		DBGOUT("Quest:%d[%s], NPC:%d[%s]\n", 
//			it->second->QuestIndex, it->second->QuestName, 
//			it->second->QuestNPCInfo.NPCIndex, it->second->QuestNPCInfo.NPCName);
//		it++;
//	}
	DBGOUT("-------------------------QuestData End-----------------------------\n");
#endif // _DEBUG_endif

	m_vecQuestMonsterCount.clear();	

	// 2008-11-11 by bhsohn 퀘스트 CheckSum추가
	m_bGetCheckSum = FALSE;

	// 2009. 05. 29 by ckPark SHA256 체크섬 시스템
	//m_uiCheckSum = 0;
	memset(m_uiCheckSum, 0, sizeof(m_uiCheckSum));
	// end 2009. 05. 29 by ckPark SHA256 체크섬 시스템

	m_nFileSize = 0;
	memset(m_chMapIdx, 0x00, 64);
	
}

CQuestData::~CQuestData()
{
	g_pQuestData = NULL;
	CMapQuestIterator it = m_mapQuest.begin();
	while(it != m_mapQuest.end())
	{
		util::del(it->second);
		it++;
	}
	CMapCharacterQuestIterator it2 = m_mapCharacterQuest.begin();
	while(it2 != m_mapCharacterQuest.end())
	{
		util::del(it2->second);
		it2++;
	}

	vector<Quest_Data*>::iterator itq = m_vecQuest.begin();
	while(itq != m_vecQuest.end())
	{
		util::del(*itq);
		itq++;
	}
	m_vecQuest.clear();
}

void CQuestData::Init()
{
	CMapCharacterQuestIterator it = m_mapCharacterQuest.begin();
	while( it != m_mapCharacterQuest.end())
	{
		util::del(it->second);
		it++;
	}
	m_mapCharacterQuest.clear();
	m_vecQuestMonsterCount.clear();						// 2006-01-25 by ispark, 
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CQuestData::IsSatisfyStartCondition(CQuest* pQuest)
/// \brief		퀘스트 시작정보를 만족하는가? 즉 퀘스트를 시작할 수 있는가?
///					USHORT			ReqUnitKind;
///					INT_RANGE		ReqLevel;			// 필요 레벨 하한
///					INT_RANGE		ReqAttackPart;
///					INT_RANGE		ReqDefensePart;
///					INT_RANGE		ReqFuelPart;
///					INT_RANGE		ReqSoulPart;
///					INT_RANGE		ReqShieldPart;
///					INT_RANGE		ReqDodgePart;
/// \author		dhkwon
/// \date		2004-04-28 ~ 2004-04-28
/// \warning	ReqItemVector는 제외한다. ReqItem을 보여줄 경우 연결퀘스트가 안보이는 경우가 있다.
///				ReqQuestIndexVector는 시작 요건에는 만족해야 하지만, 정보를 보여줄때는 검사하지 않아도 된다.
///				Experience_t	ReqExperience;		// check: 나중에 필요하면 사용하기, 지금은 사용하지 않고 있음! 20040422, kelovon
///				퀘스트가 이미 시작되었으면 TRUE를 리턴한다.
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CQuestData::IsSatisfyStartCondition(CQuest* pQuest)
{
	if( IsQuestProgress( pQuest->QuestIndex ) )
	{
		return TRUE;
	}
	if( CompareBitFlag( pQuest->ReqUnitKind, g_pShuttleChild->m_myShuttleInfo.UnitKind ) &&
		CompareIntRange( pQuest->ReqLevel, g_pShuttleChild->m_myShuttleInfo.Level ) && 
		CompareIntRange( pQuest->ReqAttackPart, g_pShuttleChild->m_myShuttleInfo.TotalGearStat.AttackPart ) && 
		CompareIntRange( pQuest->ReqDefensePart, g_pShuttleChild->m_myShuttleInfo.TotalGearStat.DefensePart ) && 
		CompareIntRange( pQuest->ReqFuelPart, g_pShuttleChild->m_myShuttleInfo.TotalGearStat.FuelPart ) && 
		CompareIntRange( pQuest->ReqSoulPart, g_pShuttleChild->m_myShuttleInfo.TotalGearStat.SoulPart ) && 
		CompareIntRange( pQuest->ReqShieldPart, g_pShuttleChild->m_myShuttleInfo.TotalGearStat.ShieldPart ) && 
		CompareIntRange( pQuest->ReqDodgePart, g_pShuttleChild->m_myShuttleInfo.TotalGearStat.DodgePart ) )
	{
		vector<INT>::iterator it = pQuest->ReqQuestIndexVector.begin();
		while(it != pQuest->ReqQuestIndexVector.end())
		{
			CMapCharacterQuestIterator itSolve = m_mapCharacterQuest.find((*it));
			if(	itSolve == m_mapCharacterQuest.end()
				|| itSolve->second->QuestState != QUEST_STATE_COMPLETED)
			{
				return FALSE;
			}
			it++;
		}
		it = pQuest->ReqRecvQuestIndexVector.begin();
		while(it != pQuest->ReqRecvQuestIndexVector.end())
		{
			CMapCharacterQuestIterator itSolve = m_mapCharacterQuest.find((*it));
			if(	itSolve == m_mapCharacterQuest.end() )
			{
				return FALSE;
			}
			it++;
		}
/*		vector<ITEM_W_COUNT>::iterator it3 =	pQuest->ReqItemVector.begin();
		while( it3 != pQuest->ReqItemVector.end())
		{
			if(!CompareItemCount(*it3))
			{
				return FALSE;
			}
			it3++;
		}
*/
		return TRUE;
	}
	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CQuestData::IsShowQuest(CQuest* pQuest)
/// \brief		퀘스트 리스트에서 보여줄 수 있는지 없는지 판단
/// \author		dhkwon
/// \date		2004-04-30 ~ 2004-04-30
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CQuestData::IsShowQuest(CQuest* pQuest)
{
	if( IsQuestProgress( pQuest->QuestIndex ) )
	{
		return TRUE;
	}
	if( CompareBitFlag( pQuest->ReqUnitKind, g_pShuttleChild->m_myShuttleInfo.UnitKind ) &&
		CompareIntRange( pQuest->ReqLevel, g_pShuttleChild->m_myShuttleInfo.Level ) && 
		CompareIntRange( pQuest->ReqAttackPart, g_pShuttleChild->m_myShuttleInfo.TotalGearStat.AttackPart ) && 
		CompareIntRange( pQuest->ReqDefensePart, g_pShuttleChild->m_myShuttleInfo.TotalGearStat.DefensePart ) && 
		CompareIntRange( pQuest->ReqFuelPart, g_pShuttleChild->m_myShuttleInfo.TotalGearStat.FuelPart ) && 
		CompareIntRange( pQuest->ReqSoulPart, g_pShuttleChild->m_myShuttleInfo.TotalGearStat.SoulPart ) && 
		CompareIntRange( pQuest->ReqShieldPart, g_pShuttleChild->m_myShuttleInfo.TotalGearStat.ShieldPart ) && 
		CompareIntRange( pQuest->ReqDodgePart, g_pShuttleChild->m_myShuttleInfo.TotalGearStat.DodgePart ) )
	{
		return TRUE;
	}
	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CQuestData::IsQuestCompleted(INT nQuestIndex)
/// \brief		퀘스트가 종료되었는지 확인
/// \author		dhkwon
/// \date		2004-04-28 ~ 2004-04-28
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CQuestData::IsQuestCompleted(INT nQuestIndex)
{
	CMapCharacterQuestIterator itSolve = m_mapCharacterQuest.find(nQuestIndex);
	if(itSolve != m_mapCharacterQuest.end())
	{
		if(itSolve->second->QuestState == QUEST_STATE_COMPLETED)
		{
			return TRUE;
		}
	}
	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CQuestData::IsQuestProgress(INT nQuestIndex)
/// \brief		퀘스트가 진행중인지 확인
/// \author		dhkwon
/// \date		2004-04-28 ~ 2004-04-28
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CQuestData::IsQuestProgress(INT nQuestIndex)
{
	CMapCharacterQuestIterator itSolve = m_mapCharacterQuest.find(nQuestIndex);
	if(itSolve != m_mapCharacterQuest.end())
	{
		if(itSolve->second->QuestState == QUEST_STATE_IN_PROGRESS)
		{
			return TRUE;
		}
	}
	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CQuestData::IsQuestProgress(INT nQuestIndex)
/// \brief		퀘스트가 시작안했는지 확인
/// \author		dhkwon
/// \date		2004-04-28 ~ 2004-04-28
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CQuestData::IsQuestNotStart(INT nQuestIndex)
{
	CMapCharacterQuestIterator itSolve = m_mapCharacterQuest.find(nQuestIndex);
	if(itSolve != m_mapCharacterQuest.end())
	{
		if(itSolve->second->QuestState == QUEST_STATE_NONE)
		{
			return TRUE;
		}
	}
	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CQuestData::IsExistStartQuest(int nNPCIndex), (구:IsExistQuest)
/// \brief		해당 NPC에게 시작할 퀘스트가 있는지 검색한다.(인터페이스에서 미션 버튼 활성화 목적)
/// \author		dhkwon
/// \date		2004-04-28 ~ 2004-04-28
/// \warning	1. 캐릭터가 해결한 퀘스트는 리턴한다.
///				2. 건물에서 진행중인 퀘스트가 TargetMeetNPCIndex와 nNPCIndex가 같으면 서버에 해결요청을 보낸다.
///				3. 기타 해결되지 않은 퀘스트
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CQuestData::IsExistStartQuest(int nNPCIndex)
{
	FLOG( "CQuestData::QuestIsStartQuest(int nNPCIndex)" );
	CMapQuestIterator it = m_mapQuest.begin();
	while(it != m_mapQuest.end())
	{
		CQuest* pQuest = it->second;
		CMapCharacterQuestIterator itSolve = m_mapCharacterQuest.find(pQuest->QuestIndex);
		if(itSolve != m_mapCharacterQuest.end())
		{
			MEX_QUEST_INFO* pCharacter = itSolve->second;
			if(pCharacter->QuestState == QUEST_STATE_COMPLETED)
			{
				it++;	// 이미 완료한 퀘스트 
				continue;
			}
			else if( pCharacter->QuestState == QUEST_STATE_IN_PROGRESS 
				&& pQuest->QuestNPCInfo.NPCIndex == nNPCIndex)
			{
				return TRUE;	// 완료되지 않은 퀘스트
			}
		}
		if( pQuest->QuestNPCInfo.NPCIndex == nNPCIndex )
		{
			if(IsShowQuest(pQuest))
			{
				return TRUE;
			}
		}
		it++;
	}
	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			CQuest* CQuestData::FindQuest(int nQuestIndex)
/// \brief		Quest를 찾는다.
/// \author		dhkwon
/// \date		2004-04-29 ~ 2004-04-29
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
CQuest* CQuestData::FindQuest(int nQuestIndex)
{
	CMapQuestIterator it = m_mapQuest.find(nQuestIndex);
	if(it != m_mapQuest.end())
	{
		return it->second;
	}
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			CQuest* FindFirstScenarioQuest(int nInfluence)
/// \brief		첫번째 시나리오 퀘스트를 찾는다
/// \author		ydkim
/// \date		2005-12-6
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
CQuest* CQuestData::FindFirstScenarioQuest(int nInfluence)
{
	CMapQuestIterator it = m_mapQuest.begin();
	while(it != m_mapQuest.end())
	{
		CQuest* pQuest = it->second;
		if(COMPARE_INFLUENCE(nInfluence, pQuest->ReqInfluenceType) && pQuest->QuestKind == QUEST_KIND_SCENARIO)
		{
			if(pQuest->ReqQuestIndexVector.size() == 0
				|| (pQuest->ReqQuestIndexVector.size() == 1 && pQuest->ReqQuestIndexVector[0] == 0) )
			{
				return pQuest;
			}			
		}		
		it++;
	}
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			MEX_QUEST_INFO* CQuestData::FindCharacterQuest(int nQuestIndex)
/// \brief		캐릭터(진행중인) 퀘스트를 찾는다.
/// \author		dhkwon
/// \date		2004-04-29 ~ 2004-04-29
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
MEX_QUEST_INFO* CQuestData::FindCharacterQuest(int nQuestIndex)
{
	CMapCharacterQuestIterator it = m_mapCharacterQuest.find(nQuestIndex);
	if(it != m_mapCharacterQuest.end())
	{
		return it->second;
	}
	return NULL;
}

void CQuestData::AddCharacterQuest(int nQuestIndex, MEX_QUEST_INFO* pCharacterQuest)
{
#ifdef _DEBUG
	if( m_mapCharacterQuest.find(pCharacterQuest->QuestIndex) != m_mapCharacterQuest.end() )
	{
		//이미 존재하는 퀘스트를 CharacterQuest에 넣습니다
		DBGOUT("Add CharacterQuest.(QuestIndex:%d)", pCharacterQuest->QuestIndex);
	}
#endif // _DEBUG
	ASSERT_ASSERT(m_mapCharacterQuest.find(pCharacterQuest->QuestIndex) == m_mapCharacterQuest.end());
	m_mapCharacterQuest[pCharacterQuest->QuestIndex] = pCharacterQuest;
	if(pCharacterQuest->QuestState == QUEST_STATE_IN_PROGRESS)
	{
		m_nProgressCharacterQuestNum++;
	}
}

void CQuestData::DeleteCharacterQuest(int nQuestIndex)
{
	CMapCharacterQuestIterator it = m_mapCharacterQuest.find(nQuestIndex);
	if( it != m_mapCharacterQuest.end() )
	{
		if(it->second->QuestState == QUEST_STATE_IN_PROGRESS)
		{
			m_nProgressCharacterQuestNum--;
		}
		util::del(it->second);
		m_mapCharacterQuest.erase(it);
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			vector<int> CQuestData::GetRaceQuestIndex()
/// \brief		종족에 따른 퀘스트 분류
///				
/// \author		ydkim
/// \date		2005-07-08
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
vector<int> CQuestData::GetRaceQuestIndex(int nfluenceType)
{
	FLOG( "CQuestData::GetRaceQuestIndex(int nfluenceType)" );

	vector<int> vecQuestIndex;

	CMapQuestIterator it = m_mapQuest.begin();
	while(it != m_mapQuest.end())
	{
		CQuest* pQuest = it->second;
		if( COMPARE_INFLUENCE(pQuest->ReqInfluenceType, nfluenceType) )
		{
			vecQuestIndex.push_back((int)pQuest->QuestIndex);			
		}
		it++;
	}

	return vecQuestIndex;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			vector<int> CQuestData::GetRaceQuestIndex_ToVector()
/// \brief		벡터에서 종족에 따른 퀘스트 분류
///				
/// \author		ydkim
/// \date		2005-07-08
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
vector<int> CQuestData::GetRaceQuestIndex_ToVector(int nfluenceType)
{
	vector<int> vecQuestIndex;

	vector<CQuest*>::iterator it = m_vectCQuestInfoList.begin();
	while(it != m_vectCQuestInfoList.end())
	{
		CQuest* pQuest = (*it);
		if( COMPARE_INFLUENCE(pQuest->ReqInfluenceType, nfluenceType) )
		{
			vecQuestIndex.push_back((int)pQuest->QuestIndex);			
		}
		it++;
	}

	return vecQuestIndex;

}


///////////////////////////////////////////////////////////////////////////////
/// \fn			void CQuestData::GetQuestList(int nNPCIndex)
/// \brief		건물에 시작 가능한 퀘스트를 넣는다.
///				진행중 미션중에 TargetMeetNPCIndex와 nNPCIndex가 같고, 해결가능한 것을 넣는다.
/// \author		dhkwon
/// \date		2004-04-28 ~ 2004-04-28
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
#define NPC_PROFESSOR_INDEX		8	// 교관 인덱스

void CQuestData::GetQuestList(int nNPCIndex)
{
	FLOG( "CQuestData::CheckQuest(int nNPCIndex)" );
	ASSERT_ASSERT(g_pInterface->m_pCityBase);

	g_pInterface->m_pCityBase->ResetCityMission();
	CMapQuestIterator it = m_mapQuest.begin();
	while(it != m_mapQuest.end())
	{
		CQuest* pQuest = it->second;
		if( pQuest->QuestNPCInfo.NPCIndex == nNPCIndex )
		{
			// 시작 가능, 진행중
			if( IsSatisfyStartCondition(pQuest))
			{
				g_pInterface->m_pCityBase->AddCityMission(pQuest);
			}
		}
		it++;
	}
	CMapCharacterQuestIterator it2 = m_mapCharacterQuest.begin();
	while(it2 != m_mapCharacterQuest.end())
	{
		MEX_QUEST_INFO* pCharacterQuest = it2->second;
		if(pCharacterQuest->QuestState == QUEST_STATE_IN_PROGRESS)
		{
			CQuest* pQuest = FindQuest( pCharacterQuest->QuestIndex );
			if(pQuest)
			{
				if( pQuest->QuestNPCInfo.NPCIndex != nNPCIndex &&
					CheckEndQuestCondition( pQuest, nNPCIndex ))// 해결 조건을 만족하면
				{
					g_pInterface->m_pCityBase->AddCityMission(pQuest);
				}
			}
			else
			{
				#ifdef _DEBUG
				DBGOUT("\n\n                           Error :  Quest Data (%d) \n\n",pCharacterQuest->QuestIndex);
				#else
				g_pD3dApp->NetworkErrorMsgBox(STRERR_C_RESOURCE_0002);//"리소스가 정상적이지 않습니다. 프로그램을 다시 인스톨하세요."
				#endif
			}
		}
		it2++;
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			NotifyStartQuest()
/// \brief		레벨업시에 시작 미션이 있는지 확인하고 메시지 띄움
/// \author		dhkwon
/// \date		2004-08-06 ~ 2004-08-06
///////////////////////////////////////////////////////////////////////////////
void CQuestData::NotifyStartQuest()
{
	CMapQuestIterator it = m_mapQuest.begin();
	
	vector<Quest_Data*>::iterator itqu = m_vecQuest.begin();
	while(itqu != m_vecQuest.end())
	{
		util::del(*itqu);
		itqu++;
	}
	m_vecQuest.clear();
	

	while(it != m_mapQuest.end())
	{
		CQuest* pQuest = it->second;

		Quest_Data *pCQuest = NULL;		
		
		// 시작 가능
		if( CompareBitFlag( pQuest->ReqUnitKind, g_pShuttleChild->m_myShuttleInfo.UnitKind ) &&
			CompareIntRange( pQuest->ReqLevel, g_pShuttleChild->m_myShuttleInfo.Level ) && 
			CompareIntRange( pQuest->ReqAttackPart, g_pShuttleChild->m_myShuttleInfo.TotalGearStat.AttackPart ) && 
			CompareIntRange( pQuest->ReqDefensePart, g_pShuttleChild->m_myShuttleInfo.TotalGearStat.DefensePart ) && 
			CompareIntRange( pQuest->ReqFuelPart, g_pShuttleChild->m_myShuttleInfo.TotalGearStat.FuelPart ) && 
			CompareIntRange( pQuest->ReqSoulPart, g_pShuttleChild->m_myShuttleInfo.TotalGearStat.SoulPart ) && 
			CompareIntRange( pQuest->ReqShieldPart, g_pShuttleChild->m_myShuttleInfo.TotalGearStat.ShieldPart ) && 
			CompareIntRange( pQuest->ReqDodgePart, g_pShuttleChild->m_myShuttleInfo.TotalGearStat.DodgePart ) )
		{
			BOOL bReqQuest = TRUE;
			vector<INT>::iterator it2 = pQuest->ReqQuestIndexVector.begin();
			while(it2 != pQuest->ReqQuestIndexVector.end())
			{
				CMapCharacterQuestIterator itSolve = m_mapCharacterQuest.find((*it2));
				if(	itSolve == m_mapCharacterQuest.end() ||
					itSolve->second->QuestState != QUEST_STATE_COMPLETED)
				{
					bReqQuest = FALSE;
					break;
				}
				it2++;
			}
			it2 = pQuest->ReqRecvQuestIndexVector.begin();
			while(it2 != pQuest->ReqRecvQuestIndexVector.end())
			{
				CMapCharacterQuestIterator itSolve = m_mapCharacterQuest.find((*it2));
				if(	itSolve == m_mapCharacterQuest.end() )
				{
					bReqQuest = FALSE;
					break;
				}
				it2++;
			}
			if( bReqQuest == TRUE &&
				m_mapCharacterQuest.find(pQuest->QuestIndex) == m_mapCharacterQuest.end() )
			{
				// Notify
								
				BUILDINGNPC* pBUILDING = g_pDatabase->GetServerBuildingNPCInfoFromNPCIndex( pQuest->QuestNPCInfo.NPCIndex );
				if(pBUILDING != NULL)
				{
					pCQuest = new Quest_Data;				
					memset(pCQuest, 0x00, sizeof(Quest_Data));

					MAP_INFO* mapname = g_pDatabase->GetMapInfo(pBUILDING->MapIndex);
					// 2005-08-02 by ispark
					// DBGOUT 찍음
					if(mapname == NULL)
					{
						DBGOUT("MAP_INFO Error -> Map Index %d\n",pBUILDING->MapIndex);
					}
					else
					{
						strcpy(pCQuest->szMAPName, mapname->MapName); 
					}
					
					pCQuest->nIndex = pQuest->QuestIndex;
					strcpy(pCQuest->szMissionName, pQuest->QuestName); 
					strcpy(pCQuest->szNPCName, pBUILDING->NPCName); 
					strcpy(pCQuest->szShopName, pBUILDING->BuildingName);
					pCQuest->nNPCIndex = pBUILDING->NPCIndex;
					m_vecQuest.push_back(pCQuest);
				}
				#ifdef _DEBUG
				else
				{
					//
					DBGOUT("%s Non Existent NPC Index(%d) \n",pQuest->QuestName, pQuest->QuestNPCInfo.NPCIndex );
				}
				#endif
			}
		}
		it++;
	}


	// 2007-06-12 by bhsohn 미션 인터페이스 수정안
//	int flag = g_pGameMain->m_pQuest->UpDataQuest();
//	// 미션이 있고 도움말이 꺼져있을때 실행한다
//	if(flag && g_pSOptionCharacter->sHelpDesk != FALSE 
//		    && g_pGameMain->m_nRightWindowInfo != RIGHT_WINDOW_INFO
//			//&& !g_pGameMain->m_pMissionInfo->GetMissionWindowState())
//			&& FALSE == g_pGameMain->IsShowTreeWnd()) 
//	{
//		g_pGameMain->m_bKnowON[LOW_LEVEL_MISSION_HELP] = TRUE;
//		g_pGameMain->m_pHelpDesk->SetLowLevelStringbyType(LOW_LEVEL_MISSION_HELP);
//	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CQuestData::CheckEndQuestCondition(CQuest* pQuest, int nNPCIndex)
/// \brief		진행중인 퀘스트가 해결 조건을 만족하는지 판단한다.
///				
/// \author		dhkwon
/// \date		2004-04-29 ~ 2004-04-29, 
///				2004-06-24 : TargetMeetNPCIndex만 만족하면 종료되는 것을 조건의 일부로 만듬
/// \warning	건물에 진입했을 경우, NPC에 대해당하는 것만 하지 말고, 전체 진행중인 퀘스트에 대해 한바퀴 돈다.
///				이미 시작된 퀘스트이기 때문에 StartCondition은 검사하지 않는다.
/// \param		
/// \return		if(TRUE) SendFieldSocketQuestRequestSuccess호출로 종료 요청
///////////////////////////////////////////////////////////////////////////////
BOOL CQuestData::CheckEndQuestCondition(CQuest* pQuest, int nNPCIndex)
{
	ASSERT_ASSERT(g_pInterface->m_pCityBase);

	if( IsQuestProgress(pQuest->QuestIndex) )// StartCondition은 검사하지 않는다.
	{
		if( //pQuest->TargetMeetNPCInfo.NPCIndex != 0 && 
			pQuest->TargetMeetNPCInfo.NPCIndex == 0 ||
			pQuest->TargetMeetNPCInfo.NPCIndex != nNPCIndex )
		{
			return FALSE;
		}
		vector<ITEM_W_COUNT>::iterator it =	pQuest->TargetItemVector.begin();
		while( it != pQuest->TargetItemVector.end())
		{
			if(!CompareItemCount(*it))
			{
				return FALSE;
			}
			it++;
		}
		return TRUE;
	}
	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CQuestData::SendFieldSocketQuestRequestStart(int nQuestIndex, int nQuestStartType, BOOL bPassQuest)
/// \brief		퀘스트의 시작을 보낸다.
/// \author		dhkwon
/// \date		2004-04-29 ~ 2004-04-29
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CQuestData::SendFieldSocketQuestRequestStart(int nQuestIndex, int nQuestStartType, BOOL bPassQuest, char* strSerialKey)
{
	// 시간 미션중 다른 시간미션을 진행할수없게 ..// 2004-12-02 by ydkim
	CQuest* pQuest = g_pQuestData->FindQuest(nQuestIndex);
	if(pQuest->TimeLimitInMinutes)
	{		
		CMapQuestIterator it = m_mapQuest.begin();
		while(it != m_mapQuest.end())
		{
			CQuest* pQuest = it->second;
			CMapCharacterQuestIterator itSolve = m_mapCharacterQuest.find(pQuest->QuestIndex);
			if(itSolve != m_mapCharacterQuest.end())
			{
				MEX_QUEST_INFO* pCharacter = itSolve->second;
				
				if( pCharacter->QuestState == QUEST_STATE_IN_PROGRESS 
					&& pQuest->TimeLimitInMinutes != 0)
				{
					g_pD3dApp->m_pChat->CreateChatChild(STRERR_C_QUEST_0001,COLOR_ERROR);//"현재 다른 제한시간 미션을 진행중입니다."
					return;
				}
			}
			
			it++;
		}
	}	

	MSG_FC_QUEST_REQUEST_START sMsg;
	sMsg.QuestIndex = nQuestIndex;
	sMsg.QuestStartType = nQuestStartType;
	sMsg.PassQuest = (BYTE)bPassQuest;
	sMsg.AVec3QuestPosition = g_pShuttleChild->m_vPos;
	if(strSerialKey != NULL)
		strncpy(sMsg.szCouponNumber,strSerialKey, SIZE_MAX_COUPON_NUMBER);
	
	//2011-05-16 by jhahn 인피3차 퀘스트 시작 오류 메세지 수정
	if(pQuest->QuestEpisodeType != QUEST_INFINITY_TYPE_3)
	{
	    g_pFieldWinSocket->SendMsg( T_FC_QUEST_REQUEST_START, (char*)&sMsg, sizeof(sMsg));

	}
	//end 2011-05-16 by jhahn 인피3차 퀘스트 시작 오류 메세지 수정
	
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CQuestData::SendFieldSocketQuestRequestSuccess(int nQuestIndex, int nQuestResult)
/// \brief		퀘스트의 종료를 보낸다. nQuestResult는 결과값(0이면 무시한다.)
/// \author		dhkwon
/// \date		2004-04-29 ~ 2004-04-29
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CQuestData::SendFieldSocketQuestRequestSuccess(int nQuestIndex, int nQuestResult)
{
// 2006-03-28 by ispark
//	CQuest* pQuest = g_pQuestData->FindQuest(nQuestIndex);
	CQuest* pQuest = FindQuest(nQuestIndex);
	if(NULL == pQuest)
	{
		return;
	}

	MSG_FC_QUEST_REQUEST_SUCCESS sMsg;
	sMsg.QuestIndex = nQuestIndex;
	sMsg.QuestResult = nQuestResult;
	// 2008-04-07 by bhsohn 퀘스트 시간 지연되는 문제 처리
	//sMsg.SpentTimeInSeconds = (pQuest->TimeLimitInMinutes * 60)-g_pGameMain->m_nTimeSecond;
	sMsg.SpentTimeInSeconds = (pQuest->TimeLimitInMinutes * 60)-((int)g_pGameMain->GetQuestTimeElapse());	

	sMsg.AVec3QuestPosition = g_pShuttleChild->m_vPos;
	g_pFieldWinSocket->SendMsg( T_FC_QUEST_REQUEST_SUCCESS, (char*)&sMsg, sizeof(sMsg));
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			CQuestData::SendFieldSocketQuestRequestSuccessCheck()
/// \brief		파티미션 종료 체크
/// \author		ispark
/// \date		2006-03-27 ~ 2006-03-27
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CQuestData::SendFieldSocketQuestRequestSuccessCheck(int nQuestIndex, int nQuestResult)
{
// 2006-03-28 by ispark
//	CQuest* pQuest = g_pQuestData->FindQuest(nQuestIndex);
	CQuest* pQuest = FindQuest(nQuestIndex);
	if(NULL == pQuest)
	{
		return;
	}

	MSG_FC_QUEST_REQUEST_SUCCESS_CHECK sMsg;
	sMsg.QuestIndex = nQuestIndex;
	sMsg.QuestResult = nQuestResult;	
	// 2008-04-07 by bhsohn 퀘스트 시간 지연되는 문제 처리
	//sMsg.SpentTimeInSeconds = (pQuest->TimeLimitInMinutes * 60)-g_pGameMain->m_nTimeSecond;
	sMsg.SpentTimeInSeconds = (pQuest->TimeLimitInMinutes * 60)-((int)g_pGameMain->GetQuestTimeElapse());

	sMsg.AVec3QuestPosition = g_pShuttleChild->m_vPos;
	g_pFieldWinSocket->SendMsg( T_FC_QUEST_REQUEST_SUCCESS_CHECK, (char*)&sMsg, sizeof(sMsg));
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			CQuestData::MakeCompleteProgressMission()
/// \brief		해결 가능한 미션 해결
/// \author		dhkwon
/// \date		2004-06-23 ~ 2004-06-23
/// \warning	CharacterQuest를 한바퀴 돌면서 현재 NPC를 이용해서 해결가능한지 검사한다.
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CQuestData::MakeCompleteProgressMission(int nNPCIndex)
{
	CMapCharacterQuestIterator it = m_mapCharacterQuest.begin();
	while(it != m_mapCharacterQuest.end())
	{
		MEX_QUEST_INFO* pCharacterQuest = it->second;
		if(pCharacterQuest->QuestState == QUEST_STATE_IN_PROGRESS)
		{
			CQuest* pQuest = FindQuest( pCharacterQuest->QuestIndex );
			if(pQuest)
			{
				if(CheckEndQuestCondition( pQuest, nNPCIndex ))// 해결 조건을 만족하면
				{
					SendFieldSocketQuestRequestSuccess( pCharacterQuest->QuestIndex );
				}
			}
			else
			{
				#ifdef _DEBUG
				DBGOUT("\n\n                            Error : Quest Data (%d) \n\n",pCharacterQuest->QuestIndex);
				#else
				g_pD3dApp->NetworkErrorMsgBox(STRERR_C_QUEST_0002);//"퀘스트 데이타가 정상적이지 않습니다. 프로그램을 다시 인스톨하세요."
				#endif
			}
		}
		it++;
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			CQuestData::SetQuestEndNPCTalk( BOOL bSuccessful, int nQuestIndex )
/// \brief		퀘스트 종료시 NPCTalk나 결과물 전달..
/// \author		dhkwon
/// \date		2004-07-07 ~ 2004-07-07
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CQuestData::SetQuestEndNPCTalk( BOOL bSuccessful, int nQuestIndex, INT		nExpOfCompensation)
{
	CQuest* pQuest = this->FindQuest( nQuestIndex );
	ASSERT_ASSERT( pQuest );
	if( bSuccessful )
	{
		if( pQuest && g_pInterface->m_pCityBase)
		{
			if( pQuest->TargetMeetNPCInfo.NPCIndex == pQuest->QuestNPCInfo.NPCIndex )
			{
				g_pInterface->m_pCityBase->SetNPCTalkInfo( pQuest->QuestNPCInfo.SuccessTalk );
			}
			else
			{
				g_pInterface->m_pCityBase->SetNPCTalkInfo( pQuest->TargetMeetNPCInfo.SuccessTalk );
			}
			char buf[256];
	//		vector<QUEST_PAY_ITEM>::iterator it = pQuest->EndPayItemVector.begin();// 퀘스트 수행 시 주어지는 아이템
	//		while(it != pQuest->EndPayItemVector.end())
	//		{
	//			if( IS_SAME_UNITKIND((*it).ToUnitKind, g_pShuttleChild->m_myShuttleInfo.UnitKind) == TRUE)
	//			{
	//				ITEM* pITEM = g_pDatabase->GetServerItemInfo( (*it).ItemNum );
	//				if( pITEM )
	//				{
	//					int nCount = (*it).Count;
	//					if( nCount > 1 )
	//					{
	//						wsprintf( buf, "아이템 '%s'이(가) %d 개 추가되었습니다", pITEM->ItemName, nCount );
	//					}
	//					else
	//					{
	//						wsprintf( buf, "아이템 '%s'이(가) 추가되었습니다", pITEM->ItemName );
	//					}
	//					g_pD3dApp->m_pChat->CreateChatChild( buf, COLOR_SKILL_USE ); // 녹색
	//				}
	//			}
	//			it++;
	//		}
			if(pQuest->EndPayBonusStat > 0)
			{
				wsprintf( buf, STRMSG_C_QUEST_0001, pQuest->EndPayBonusStat );//"보너스 스탯이 %d 개 추가 되었습니다"
				g_pD3dApp->m_pChat->CreateChatChild( buf, COLOR_SKILL_USE ); // 녹색
			}
//			if(pQuest->EndPayBonusSkillPoint > 0)	// 2005-11-16 by ispark
//			{
//				wsprintf( buf, STRMSG_C_QUEST_0002, pQuest->EndPayBonusSkillPoint );//"보너스 스킬 포인트가 %d 개 추가 되었습니다"
//				g_pD3dApp->m_pChat->CreateChatChild( buf, COLOR_SKILL_USE ); // 녹색
//			}
			if(pQuest->EndPayFame > 0)			// 명성이 높아짐
			{
				wsprintf( buf, STRMSG_C_QUEST_0003, pQuest->EndPayFame );//"명성이 %d 만큼 추가 되었습니다"
				g_pD3dApp->m_pChat->CreateChatChild( buf, COLOR_SKILL_USE ); // 녹색
			}
			if(pQuest->EndPayStatus > 0)		// 신분 상승함
			{
				wsprintf( buf, STRMSG_C_QUEST_0004, pQuest->EndPayStatus );//"신분이 %d 만큼 추가 되었습니다"
				g_pD3dApp->m_pChat->CreateChatChild( buf, COLOR_SKILL_USE ); // 녹색
			}
			if(pQuest->EndPayLevel > 0)		// 레벨 상승량(+1, +2 등)
			{
				wsprintf( buf, STRMSG_C_QUEST_0005, pQuest->EndPayLevel );//"레벨이 %d 만큼 올라갔습니다"
				g_pD3dApp->m_pChat->CreateChatChild( buf, COLOR_SKILL_USE ); // 녹색
			}
			// 2007-03-06 by bhsohn 퀘스트 완료시 경험치 처리 방식 변경
//			if(pQuest->EndPayExperience > 0)	// 경험치 상승량(+100, +200 등)
//			{				
//				wsprintf( buf, STRMSG_C_QUEST_0006, pQuest->EndPayExperience );//"경험치가 %d 만큼 올라갔습니다"
//				g_pD3dApp->m_pChat->CreateChatChild( buf, COLOR_SKILL_USE ); // 녹색
//			}
			if(nExpOfCompensation > 0)	// 경험치 상승량(+100, +200 등)
			{
				wsprintf( buf, STRMSG_C_QUEST_0006, nExpOfCompensation );//"경험치가 %d 만큼 올라갔습니다"				
				g_pD3dApp->m_pChat->CreateChatChild( buf, COLOR_SKILL_USE ); // 녹색
			}
			// end 2007-03-06 by bhsohn 퀘스트 완료시 경험치 처리 방식 변경

	//		if(pQuest->EndPayWarpMapIndex > 0)	// 워프할 맵
	//		{
	//			wsprintf( buf, "워프됩니다.", pQuest->EndPayWarpMapIndex );
	//			g_pD3dApp->m_pChat->CreateChatChild( buf, COLOR_SKILL_USE ); // 녹색
	//		}
		}
	}
	else // 실패시
	{
		if( pQuest && g_pInterface->m_pCityBase)
		{
			if( pQuest->TargetMeetNPCInfo.NPCIndex == pQuest->QuestNPCInfo.NPCIndex )
			{
				g_pInterface->m_pCityBase->SetNPCTalkInfo( pQuest->QuestNPCInfo.FailTalk );
			}
			else
			{
				g_pInterface->m_pCityBase->SetNPCTalkInfo( pQuest->TargetMeetNPCInfo.FailTalk );
			}
		}
	}

}

CQuest* CQuestData::GetProgressMission()
{
	CMapCharacterQuestIterator itSolve = m_mapCharacterQuest.begin();
	for(; itSolve != m_mapCharacterQuest.end(); itSolve++)
	{		
		if(itSolve->second->QuestState == QUEST_STATE_IN_PROGRESS)
		{
			CQuest *pQuest = FindQuest(itSolve->second->QuestIndex);
			if(pQuest)
			{
				return pQuest;
			}
		}
	}

	return NULL;
}

Quest_MonsterInfo* CQuestData::FindQuestMonsterCountInfo(int i_nMonUnitKind)
{
	vector<Quest_MonsterInfo>::iterator itr = m_vecQuestMonsterCount.begin();
	for(;itr != m_vecQuestMonsterCount.end(); itr++)
	{
		if(itr->nMonsterUnitkind == i_nMonUnitKind)
		{
			return &*itr;
		}
	}

	return NULL;
}


CQuest* CQuestData::FindMapHideQuest(int nMapIndex)
{
	CMapQuestIterator it = m_mapQuest.begin();
	while(it != m_mapQuest.end())
	{
		CQuest* pQuest = it->second;

		if(pQuest->ReqStartArea.MapIndex == nMapIndex)
		{
			return pQuest;
		}
		it++;
	}

	return NULL;
}

CQuest* CQuestData::FindPosCompleteQuest(int nMapIndex)
{
	CMapQuestIterator it = m_mapQuest.begin();
	while(it != m_mapQuest.end())
	{
		CQuest* pQuest = it->second;

		if(pQuest->TargetMeetMapArea.MapIndex == nMapIndex)
		{
			return pQuest;
		}
		it++;
	}

	return NULL;
}

void CQuestData::NoticeQuestTargetItem( int nItemNum, int nCount )
{
}

void CQuestData::CheckClientQuestList()
{
}


void CQuestData::CheckBeginnerQuest()
{
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		퀘스트 파일 체크섬 
/// \author		// 2008-11-11 by bhsohn 퀘스트 CheckSum추가
/// \date		2008-11-11 ~ 2008-11-11
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CQuestData::RefreshQuestCheckSum()
{
	CGameData gameData;
	char chFullMapPath[MAX_PATH], chMapIdx[64];	
	
	memset(chFullMapPath, 0x00, MAX_PATH);		
	memset(chMapIdx, 0x00, 64);
	
	wsprintf(chMapIdx, "quest.tex");
	strncpy(chFullMapPath, chMapIdx, MAX_PATH);
	
	g_pD3dApp->LoadPath( chFullMapPath, IDS_DIRECTORY_TEXTURE, chMapIdx);			
	
	int nFileSize = 0;

	// 2009. 05. 29 by ckPark SHA256 체크섬 시스템
// 	UINT uiCheckSum = 0;
// 	if(FALSE == gameData.GetCheckSum(&uiCheckSum, &nFileSize, chFullMapPath))
// 	{
// 	}	
// 	m_uiCheckSum = uiCheckSum;

	if(FALSE == gameData.GetCheckSum(m_uiCheckSum, &nFileSize, chFullMapPath))
	{
	}	
	// end 2009. 05. 29 by ckPark SHA256 체크섬 시스템


	m_nFileSize = nFileSize;
	strncpy(m_chMapIdx, chMapIdx, 64);
	m_bGetCheckSum = TRUE;
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		퀘스트 파일 체크섬 
/// \author		// 2008-11-11 by bhsohn 퀘스트 CheckSum추가
/// \date		2008-11-11 ~ 2008-11-11
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////


// 2009. 05. 29 by ckPark SHA256 체크섬 시스템
//BOOL CQuestData::GetQuestCheckSum(UINT* o_uiCheckSum,int *o_nFileSize, char* o_chMapIdx)
BOOL CQuestData::GetQuestCheckSum(BYTE o_byCheckSum[32],int *o_nFileSize, char* o_chMapIdx)
// end 2009. 05. 29 by ckPark SHA256 체크섬 시스템


{
	if(!m_bGetCheckSum)
	{
		return FALSE;
	}
	
	// 2009. 05. 29 by ckPark SHA256 체크섬 시스템
	//(*o_uiCheckSum) = m_uiCheckSum;
	memcpy(o_byCheckSum, m_uiCheckSum, sizeof(m_uiCheckSum));
	// end 2009. 05. 29 by ckPark SHA256 체크섬 시스템

	(*o_nFileSize) = m_nFileSize;
	strncpy(o_chMapIdx, m_chMapIdx, 64);

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CQuestData::GetMissionMasterQuestCompletedCount()
/// \brief		미션마스터로 등록 가능한지 여부.
/// \author		dgwoo
/// \date		2008-12-16 ~ 2008-12-16
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
DWORD CQuestData::GetMissionMasterQuestCompletedCount()
{
	DWORD nCompletedCount = 0;
	CMapCharacterQuestIterator itSolve = m_mapCharacterQuest.begin();
	for(; itSolve != m_mapCharacterQuest.end(); itSolve++)
	{		
		if(itSolve->second->QuestState == QUEST_STATE_COMPLETED)
		{
			CQuest *pQuest = FindQuest(itSolve->second->QuestIndex);			// 2013-06-04 by ssjung MissionMaster를 찾는 과정에서 익셉션 에러 안전코드 추가
			if(pQuest && pQuest->QuestPartyType == QUEST_PARTYTYPE_PARTY)
			{
				nCompletedCount++;
			}
		}
	}

	return nCompletedCount;
}