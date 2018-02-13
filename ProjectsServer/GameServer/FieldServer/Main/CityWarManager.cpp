// CityWarManager.cpp: implementation of the CCityWarManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CityWarManager.h"
#include "FieldGlobal.h"
#include "FieldMapChannel.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CCityWarManager::CCityWarManager()
{
	m_pFieldIOCP11			= NULL;
	m_nSizeVectCityWarList	= 0;
}

CCityWarManager::~CCityWarManager()
{
	this->CleanCityWarManager();

	for (auto x : m_vectCityWarPtrList) delete x;
	m_vectCityWarPtrList.clear();
}

// 2008-07-11 by cmkwon, MySQL 지원을 위해 수정(IP,Port 로 DB 서버 연결) - 아래와 같이 수정 함
//BOOL CCityWarManager::InitCityWarManager(CFieldIOCP *i_pFieldIOCP, UCHAR *i_szDSN, UCHAR *i_szUID
//								   , UCHAR *i_szPASSWORD, vectorMAP_INFO *i_pVectAllMapInfo)
BOOL CCityWarManager::InitCityWarManager(CFieldIOCP *i_pFieldIOCP, const char *i_szServIP, int i_nServPort, const char *i_szDatabaseName, char *i_szUID, char *i_szPassword, HWND i_hWnd, vectorMAP_INFO *i_pVectAllMapInfo)
{
	m_ODBCStmt2.Lock();
	m_pFieldIOCP11				= i_pFieldIOCP;	

	// 2008-07-11 by cmkwon, MySQL 지원을 위해 수정(IP,Port 로 DB 서버 연결) - 아래와 같이 수정 함
	//BOOL bRet = m_ODBCStmt2.Init(i_szDSN, i_szUID, i_szPASSWORD);	
	BOOL bRet = m_ODBCStmt2.Init(i_szServIP, i_nServPort, i_szDatabaseName, i_szUID, i_szPassword, i_hWnd);	

	{
		if(FALSE == bRet)
		{
			m_ODBCStmt2.Unlock();
			return FALSE;
		}
		CCityWar::ms_pODBCStmt2		= &m_ODBCStmt2;
		CCityWar::ms_pFieldIOCP1	= m_pFieldIOCP11;
		m_vectCityWarPtrList.reserve(i_pVectAllMapInfo->size());

		for(int i=0; i < i_pVectAllMapInfo->size(); i++)
		{
			MAP_INFO *pMapInfo = &((*i_pVectAllMapInfo)[i]);
			if(IS_CITYWAR_MAP_INDEX(pMapInfo->MapIndex))
			{
				CCityWar *ptmCityWar = new CCityWar;
				if(ptmCityWar->InitCityWar(pMapInfo))
				{
					char szTemp[1024];
					sprintf(szTemp, STRMSG_S_F2CITYWAR_0004
						, ptmCityWar->MapIndex, ptmCityWar->MapName, ptmCityWar->CityMapIndex, ptmCityWar->CityWarQuestIndex
						, ptmCityWar->OccupyGuildUID, ptmCityWar->OccupyGuildName, ptmCityWar->OccupyGuildMasterCharacterUID
						, ptmCityWar->CityWarSettingTime.GetDateTimeString().GetBuffer());
					g_pFieldGlobal->WriteSystemLogEX(TRUE, szTemp);
					m_vectCityWarPtrList.push_back(ptmCityWar);
				}
			}
		}
		m_nSizeVectCityWarList	= m_vectCityWarPtrList.size();
	}
	m_ODBCStmt2.Unlock();

	///////////////////////////////////////////////////////////////////////////////
	// 도시점령전 맵에 cityWarPtr을 설정한다
	for(int i=0; i < m_nSizeVectCityWarList; i++)
	{			
		m_pFieldIOCP11->GetMapWorkspace()->SetCityWarFieldMapWorkspace(m_vectCityWarPtrList[i]);
	}
	return bRet;
}


void CCityWarManager::ResetCityWarManager(void)
{
}
void CCityWarManager::CleanCityWarManager(void)
{
	if(m_ODBCStmt2.IsReadyToUse())
	{
		CCityWar	*ptmCityWar;
		for(int i=0; i < m_nSizeVectCityWarList; i++)
		{
			ptmCityWar = m_vectCityWarPtrList[i];
			mt_auto_lock mtA(&ptmCityWar->m_mtvectCITY_WAR_DATAList);
			ptmCityWar->UpdateDBSumOfTex();
		}
	}
	m_ODBCStmt2.Clean();
}


void CCityWarManager::OnDoMinutelyWorkCityWarManager(ATUM_DATE_TIME *i_pCurDateTime)
{
	static vectCITYWAR_QUEST tmVectCITYWAR_QUEST;
	CCityWar	*ptmCityWar;
	int			nRemainMinute;
	int			bCityWarStarted, bCityWarEnded;
	for(int i=0; i < m_nSizeVectCityWarList; i++)
	{
		bCityWarStarted	= bCityWarEnded = FALSE;
		ptmCityWar = m_vectCityWarPtrList[i];
		mt_auto_lock mtA(&ptmCityWar->m_mtvectCITY_WAR_DATAList);
		if(0 == i_pCurDateTime->Minute%10)
		{
			ptmCityWar->UpdateDBSumOfTex();
		}
		switch(ptmCityWar->GetCityWarState())
		{
		case CITYWAR_STATE_NOMAL:
			{
				if(*i_pCurDateTime < ptmCityWar->CityWarSettingTime)
				{// 도시점령전 시작 시간이 경과하지 않음
					
					nRemainMinute = ptmCityWar->CityWarSettingTime.GetTimeDiffTimeInMinutes(*i_pCurDateTime);
					if(nRemainMinute <= TIMEGAP_CITYWAR_READY_TERM_MINUTE+1)
					{// 도시점령전 시작 준비 시간이 됨
						
						///////////////////////////////////////////////////////////////////////////////
						// 도시점령전 준비단계 처리
						ptmCityWar->SetReadyState();
					}
				}
				continue;
			}
			break;
		case CITYWAR_STATE_READY:
			{				
				if(*i_pCurDateTime < ptmCityWar->CityWarSettingTime)
				{// 도시점령전 남은 시간 알려주기

					nRemainMinute = ptmCityWar->CityWarSettingTime.GetTimeDiffTimeInMinutes(*i_pCurDateTime);
					if(nRemainMinute%5 == 0
						|| nRemainMinute == 1)
					{
						break;
					}
					continue;
				}

				///////////////////////////////////////////////////////////////////////////////
				// 도시점령전 시작 처리
				if(FALSE == ptmCityWar->SetStartState())
				{
					g_pFieldGlobal->WriteSystemLogEX(TRUE, STRMSG_S_F2CITYWAR_0005, ptmCityWar->MapIndex);
					break;
				}
				bCityWarStarted = TRUE;				
			}
			break;
		case CITYWAR_STATE_STARTED:
			{				
				nRemainMinute = ptmCityWar->m_ATimeWarEndTime.GetTimeDiffTimeInMinutes(*i_pCurDateTime);
				if(nRemainMinute > 0)
				{// 도시점령전 진행 중 남은시간 정보 알려주기

					if(nRemainMinute <= 5
						|| nRemainMinute%10 == 0)
					{
						break;
					}
					continue;
				}
				
				tmVectCITYWAR_QUEST.reserve(ptmCityWar->m_mtvectCITY_WAR_DATAList.size());
				tmVectCITYWAR_QUEST.clear();
				///////////////////////////////////////////////////////////////////////////////
				// 도시 점령전 종료 처리
				ptmCityWar->SetEndState(&tmVectCITYWAR_QUEST);
				bCityWarEnded = TRUE;
			}
			break;
		case CITYWAR_STATE_ENDED:
			{
				int nPassTimeMinute = i_pCurDateTime->GetTimeDiffTimeInMinutes(ptmCityWar->m_ATimeStateChangedTime);
				if(nPassTimeMinute >= 2)
				{// 도시점령전 종료 후 2분이 경과함
					
					///////////////////////////////////////////////////////////////////////////////
					// 도시점령전 종료 후 2분이 경과 후 처리
					ptmCityWar->SetNormalState();
				}
				continue;
			}
			break;
		}
		mtA.auto_unlock_cancel();

		if(bCityWarStarted)
		{
			m_pFieldIOCP11->OnCityWarStartFieldIOCP(ptmCityWar);
		}
		else if(bCityWarEnded)
		{
			m_pFieldIOCP11->OnCityWarEndFieldIOCP(ptmCityWar, &tmVectCITYWAR_QUEST);
		}
		else
		{
			m_pFieldIOCP11->OnCityWarInfoFieldIOCP(ptmCityWar, nRemainMinute);
		}
	}
}

CCityWar *CCityWarManager::OnCityWarQuestMonsterCreatedCityWarManager(MapIndex_t i_mapIdx)
{
	CCityWar *pCityWar = this->GetCityWarPtrByCityWarMapIndex(i_mapIdx);
	if(NULL == pCityWar)
	{
		return NULL;
	}

	pCityWar->OnCityWarQuestMonsterCreatedCityWar();
	return pCityWar;
}

CCityWar *CCityWarManager::OnCityWarQuestMonsterDeadCityWarManager(MapIndex_t i_mapIdx)
{
	CCityWar *pCityWar = this->GetCityWarPtrByCityWarMapIndex(i_mapIdx);
	if(NULL == pCityWar)
	{
		return NULL;
	}
	
	pCityWar->OnCityWarQuestMonsterDeadCityWar();
	return pCityWar;
}


CCityWar *CCityWarManager::GetCityWarPtrByCityWarMapIndex(MapIndex_t i_warMapIdx)
{
	for(int i=0; i < m_nSizeVectCityWarList; i++)
	{
		if(m_vectCityWarPtrList[i]->MapIndex == i_warMapIdx)
		{
			return m_vectCityWarPtrList[i];
		}
	}	
	return NULL;
}
CCityWar *CCityWarManager::GetCityWarPtrByOccupyGuildUID(UID32_t i_guildUID)
{
	if(0 == i_guildUID){							return NULL;}
	for(int i=0; i < m_nSizeVectCityWarList; i++)
	{
		if(m_vectCityWarPtrList[i]->m_OccupyCITY_WAR_DATAWhenWarStarted.GuildUID1 == i_guildUID)
		{
			return m_vectCityWarPtrList[i];
		}
	}	
	return NULL;
}
CCityWar *CCityWarManager::GetCityWarPtrByQuestIndex(INT i_questIndex)
{
	for(int i=0; i < m_nSizeVectCityWarList; i++)
	{
		if(m_vectCityWarPtrList[i]->CityWarQuestIndex == i_questIndex)
		{
			return m_vectCityWarPtrList[i];
		}
	}	
	return NULL;
}