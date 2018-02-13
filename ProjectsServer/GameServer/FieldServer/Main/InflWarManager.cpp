// InflWarManager.cpp: implementation of the CInflWarManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "InflWarManager.h"
#include "FieldIOCP.h"
#include "FieldGlobal.h"
#include "AtumLogSender.h"			// 2008-04-01 by dhjin, 모선전, 거점전 정보창 기획안 - 

///////////////////////////////////////////////////////////////////////////////
/// \fn			CInflWarData::CInflWarData()
/// \brief		
/// \author		cmkwon
/// \date		2006-04-13 ~ 2006-04-13
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
CInflWarData::CInflWarData()
{
	InfluenceType			= INFLUENCE_TYPE_UNKNOWN;
	//MGameServerID			= 0;
	WartimeStage			= 0;
	ContributionPoint		= 0;
	InflLeaderCharacterUID	= 0;
	InflSub1LeaderCharacterUID = 0;
	InflSub2LeaderCharacterUID = 0;
	
	m_pInflWarInfoList		= NULL;
	m_pCurInflWarInfo		= NULL;
	m_bSummonJacoMonster	= FALSE;


	fInflHPRepairRate			= 0.0f;		// 2009-08-20 by cmkwon, 아레나 회복률 버그 수정 - 초기화 해야함
	fInflDPRepairRate			= 0.0f;		// 2009-08-20 by cmkwon, 아레나 회복률 버그 수정 - 초기화 해야함
	fInflSPRepairRate			= 0.0f;		// 2009-08-20 by cmkwon, 아레나 회복률 버그 수정 - 초기화 해야함

	ConsecutiveVictories		= 0;		// 2013-05-09 by hskim, 세력 포인트 개선
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			CInflWarData::~CInflWarData()
/// \brief		
/// \author		cmkwon
/// \date		2006-04-13 ~ 2006-04-13
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
CInflWarData::~CInflWarData()
{
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			mt_auto_lock *CInflWarData::GetmtlockPtr(void)
/// \brief		
/// \author		cmkwon
/// \date		2006-04-13 ~ 2006-04-13
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
mt_lock *CInflWarData::GetmtlockPtr(void)
{
	return &m_mtlock;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CInflWarData::InitCInflWarData(SINFLUENCE_WAR_DATA *i_pInflWarData, vectDB_INFLUENCE_WAR_INFO *i_pInflWarInfoList)
/// \brief		
/// \author		cmkwon
/// \date		2006-04-13 ~ 2006-04-13
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CInflWarData::InitCInflWarData(SINFLUENCE_WAR_DATA *i_pInflWarData, vectDB_INFLUENCE_WAR_INFO *i_pInflWarInfoList)
{
	mt_auto_lock mtA(GetmtlockPtr());
	InfluenceType			= i_pInflWarData->InfluenceType;
	//MGameServerID			= i_pInflWarData->MGameServerID;
	WartimeStage			= i_pInflWarData->WartimeStage;
	ContributionPoint		= i_pInflWarData->ContributionPoint;
	InflLeaderCharacterUID	= i_pInflWarData->InflLeaderCharacterUID;
	InflSub1LeaderCharacterUID = i_pInflWarData->InflSub1LeaderCharacterUID;
	InflSub2LeaderCharacterUID = i_pInflWarData->InflSub2LeaderCharacterUID;
	util::strncpy(InflLeaderCharacterName, i_pInflWarData->InflLeaderCharacterName, SIZE_MAX_CHARACTER_NAME);	// 2007-12-03 by dhjin
	util::strncpy(InflSubLeader1CharacterName, i_pInflWarData->InflSubLeader1CharacterName, SIZE_MAX_CHARACTER_NAME);	// 2007-12-03 by dhjin
	util::strncpy(InflSubLeader2CharacterName, i_pInflWarData->InflSubLeader2CharacterName, SIZE_MAX_CHARACTER_NAME);	// 2007-12-03 by dhjin
	ConsecutiveVictories	= i_pInflWarData->ConsecutiveVictories;		// 2013-05-09 by hskim, 세력 포인트 개선

	m_pInflWarInfoList		= i_pInflWarInfoList;

	m_pCurInflWarInfo		= this->GetInflWarInfoByWartimeStage(WartimeStage);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CInflWarData::SetCurrentStepInflWarData(INT i_nBossMonsterUID, byte i_byInfl)
/// \brief		선전 포고 - 현재 모선 단계로 m_pCurInflWarInfo를 설정한다.
/// \author		dhjin
/// \date		2009-01-14 ~ 2009-01-14
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CInflWarData::SetCurrentStepInflWarData(INT i_nBossMonsterUID, byte i_byInfl)
{
	mt_auto_lock mtA(GetmtlockPtr());

	m_pCurInflWarInfo = this->GetInflWarInfoByKillBossMonsterUID(i_nBossMonsterUID, i_byInfl);

	// 2009-07-02 by cmkwon, 서버 실행시 실행 관련 로그 추가 - 
	g_pFieldGlobal->WriteSystemLogEX(TRUE, "  [Notify] DeclarationNoti CInflWarData::SetCurrentStepInflWarData# %s BossMonsterNum(%d) CurInflWarInfo(WartimeStage:%d) \r\n"
		, CAtumSJ::GetInfluenceTypeString(i_byInfl), i_nBossMonsterUID, m_pCurInflWarInfo->WartimeStage);

// 2009-07-02 by cmkwon, 선전포고 관련 버그 수정 - SetCompensationForDecalationOfWarWin() 함수에서 처리
//// 2009-03-10 by dhjin, 단계별 모선 시스템 - 세력전 승리 시 지급 되는 이벤트 수정
//	this->fInflHPRepairRate = m_pCurInflWarInfo->HPRepairRate;
//	this->fInflDPRepairRate = m_pCurInflWarInfo->DPRepairRate;
//	this->fInflSPRepairRate = m_pCurInflWarInfo->SPRepairRate;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			int CInflWarData::CalcWartimeStage(int i_nContibutionPoint)
/// \brief		
/// \author		cmkwon
/// \date		2006-04-13 ~ 2006-04-13
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int CInflWarData::CalcWartimeStage(int i_nContibutionPoint)
{
	if(NULL == m_pInflWarInfoList){					return 0;}

	int nBeforeStage = 0;
	for(int i=0; i < m_pInflWarInfoList->size(); i++)		// 2006-04-13 by cmkwon, 오름차순으로 정렬되어있다
	{
		SDB_INFLUENCE_WAR_INFO *pInflWarInfo = &((*m_pInflWarInfoList)[i]);
		if(i_nContibutionPoint == pInflWarInfo->ReqContributionPoint)
		{
			return pInflWarInfo->WartimeStage;
		}
		else if(i_nContibutionPoint < pInflWarInfo->ReqContributionPoint)
		{
			return nBeforeStage;
		}
		nBeforeStage		= pInflWarInfo->WartimeStage;
	}

	return nBeforeStage;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			SDB_INFLUENCE_WAR_INFO *CInflWarData::GetInflWarInfoByWartimeStage(int i_nWartimeStage)
/// \brief		
/// \author		cmkwon
/// \date		2006-04-13 ~ 2006-04-13
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
SDB_INFLUENCE_WAR_INFO *CInflWarData::GetInflWarInfoByWartimeStage(int i_nWartimeStage)
{
	if(NULL == m_pInflWarInfoList){					return NULL;}

	for(int i=0; i < m_pInflWarInfoList->size(); i++)		// 2006-04-13 by cmkwon, 오름차순으로 정렬되어있다
	{
		SDB_INFLUENCE_WAR_INFO *pInflWarInfo = &((*m_pInflWarInfoList)[i]);
		if(i_nWartimeStage == pInflWarInfo->WartimeStage)
		{
			return pInflWarInfo;
		}
	}
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			SDB_INFLUENCE_WAR_INFO *CInflWarData::GetInflWarInfoByKillBossMonsterUID(int i_nBossMonsterUID, byte i_byInfl)
/// \brief		단계별 모선 시스템 
/// \author		dhjin
/// \date		2009-03-10 ~ 2009-03-10
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
SDB_INFLUENCE_WAR_INFO *CInflWarData::GetInflWarInfoByKillBossMonsterUID(int i_nBossMonsterUID, byte i_byInfl)
{
	if(NULL == m_pInflWarInfoList){					return NULL;}
	if(0 == i_nBossMonsterUID)
	{
		for(int i=0; i < m_pInflWarInfoList->size(); i++)		// 2006-04-13 by cmkwon, 오름차순으로 정렬되어있다
		{
			SDB_INFLUENCE_WAR_INFO *pInflWarInfo = &((*m_pInflWarInfoList)[i]);
			if(i_byInfl == pInflWarInfo->InfluenceType
				&& 5 == pInflWarInfo->WartimeStage)
			{
				return pInflWarInfo;
			}
		}			
	}

	for(int i=0; i < m_pInflWarInfoList->size(); i++)		// 2006-04-13 by cmkwon, 오름차순으로 정렬되어있다
	{
		SDB_INFLUENCE_WAR_INFO *pInflWarInfo = &((*m_pInflWarInfoList)[i]);
		if(i_nBossMonsterUID == pInflWarInfo->BossMonsterUnitKind)
		{
			return pInflWarInfo;
		}
	}
	
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			SDB_INFLUENCE_WAR_INFO *CInflWarData::GetInflWarInfoByContributionPoint(int i_nContributionPoint)
/// \brief		
/// \author		cmkwon
/// \date		2006-04-14 ~ 2006-04-14
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
SDB_INFLUENCE_WAR_INFO *CInflWarData::GetInflWarInfoByContributionPoint(int i_nContributionPoint)
{
	if(NULL == m_pInflWarInfoList
		|| m_pInflWarInfoList->empty())
	{
		return NULL;
	}

	SDB_INFLUENCE_WAR_INFO *pBeforInfo = &((*m_pInflWarInfoList)[0]);
	for(int i=0; i < m_pInflWarInfoList->size(); i++)		// 2006-04-13 by cmkwon, 오름차순으로 정렬되어있다
	{
		SDB_INFLUENCE_WAR_INFO *pInflWarInfo = &((*m_pInflWarInfoList)[i]);
		if(i_nContributionPoint == pInflWarInfo->ReqContributionPoint)
		{
			return pInflWarInfo;
		}
		else if(i_nContributionPoint < pInflWarInfo->ReqContributionPoint)
		{
			return pBeforInfo;
		}
		pBeforInfo = pInflWarInfo;
	}

	return pBeforInfo;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			SDB_INFLUENCE_WAR_INFO *CInflWarData::GetCurInflWarInfo(void)
/// \brief		
/// \author		cmkwon
/// \date		2006-04-13 ~ 2006-04-13
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
SDB_INFLUENCE_WAR_INFO *CInflWarData::GetCurInflWarInfo(void)
{
	return m_pCurInflWarInfo;
}

void CInflWarData::SetWartimeStage(SDB_INFLUENCE_WAR_INFO *i_pInflWarInfo)
{
	WartimeStage		= i_pInflWarInfo->WartimeStage;
	m_pCurInflWarInfo	= i_pInflWarInfo;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CInflWarData::IsSummonJacoMonster(void)
/// \brief		
/// \author		cmkwon
/// \date		2006-04-20 ~ 2006-04-20
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CInflWarData::IsSummonJacoMonster(void)
{
	return m_bSummonJacoMonster;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		cmkwon
/// \date		2006-04-20 ~ 2006-04-20
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CInflWarData::SetSummonJacoMonster(BOOL i_bSummon)
{
	m_bSummonJacoMonster	= i_bSummon;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// 2008-03-26 by dhjin, 모선전 정보 표시 기획안 - 
CMSWarInfoDisPlay::CMSWarInfoDisPlay()
{
	this->InitMSWarInfoDisPlay();
}

CMSWarInfoDisPlay::~CMSWarInfoDisPlay()
{

}

void CMSWarInfoDisPlay::InitMSWarInfoDisPlay()
{
	m_mtvectMSWARINFO_DISPLAY.clear();
}

void CMSWarInfoDisPlay::InitMSWarOptionType(BYTE i_byLeaderInfluence)
{
	if(IS_VCN_INFLUENCE_TYPE(i_byLeaderInfluence))
	{
		BCUMSWarOptionType = MS_WAR_OPTION_TYPE_UNKNOWN;
		BCUMSWarOptionType |= MS_WAR_OPTION_TYPE_ATT_ACTIVATED;
		BCUMSWarOptionType |= MS_WAR_OPTION_TYPE_ATT_ACTIVE;	
		BCUMSWarOptionType |= MS_WAR_OPTION_TYPE_ATT_ACTIVE_TIME;	
		BCUMSWarOptionType |= MS_WAR_OPTION_TYPE_ATT_TELEPORT_HP;	
		BCUMSWarOptionType |= MS_WAR_OPTION_TYPE_ATT_STRATEGYPOINT_LIVE;				
		BCUMSWarOptionType |= MS_WAR_OPTION_TYPE_DEF_STRATEGYPOINT_HP;
		BCUMSWarOptionType |= MS_WAR_OPTION_TYPE_DEF_STRATEGYPOINT_LIVE;		
		BCUMSWarOptionType |= MS_WAR_OPTION_TYPE_LEADER;		
	}
	else
	{
		ANIMSWarOptionType = MS_WAR_OPTION_TYPE_UNKNOWN;
		ANIMSWarOptionType |= MS_WAR_OPTION_TYPE_ATT_ACTIVATED;
		ANIMSWarOptionType |= MS_WAR_OPTION_TYPE_ATT_ACTIVE;	
		ANIMSWarOptionType |= MS_WAR_OPTION_TYPE_ATT_ACTIVE_TIME;	
		ANIMSWarOptionType |= MS_WAR_OPTION_TYPE_ATT_TELEPORT_HP;	
		ANIMSWarOptionType |= MS_WAR_OPTION_TYPE_ATT_STRATEGYPOINT_LIVE;				
		ANIMSWarOptionType |= MS_WAR_OPTION_TYPE_DEF_STRATEGYPOINT_HP;
		ANIMSWarOptionType |= MS_WAR_OPTION_TYPE_DEF_STRATEGYPOINT_LIVE;		
		ANIMSWarOptionType |= MS_WAR_OPTION_TYPE_LEADER;			
	}
}

void CMSWarInfoDisPlay::MSWarInfoDisPlayClear(BYTE i_byMonsterInfluence)
{
	mt_auto_lock mta(&m_mtvectMSWARINFO_DISPLAY);
	mtvectMSWARINFO_DISPLAY::iterator itr = m_mtvectMSWARINFO_DISPLAY.begin();
	while(itr != m_mtvectMSWARINFO_DISPLAY.end())
	{
// 2009-10-12 by cmkwon, 프리스카 제거 방안 적용 - CMSWarInfoDisPlay::MSWarInfoDisPlayClear#
//		if(IS_SAME_CHARACTER_8_MAP_INFLUENCE(i_byMonsterInfluence, itr->MapInfluenceType))
		if(CAtumSJ::IsSameCharacterInfluence8MapInfluence(i_byMonsterInfluence, itr->MapInfluenceType))		// 2009-10-12 by cmkwon, 프리스카 제거 방안 적용 - 
		{
			itr = m_mtvectMSWARINFO_DISPLAY.erase(itr);
			continue;
		}
		itr++;
	}
	
}

void CMSWarInfoDisPlay::AllGetMSWarInfoDisPlay(vectMSWARINFO_DISPLAY * o_VectMSWarInfoDisPlay)
{
	mt_auto_lock mta(&m_mtvectMSWARINFO_DISPLAY);

	o_VectMSWarInfoDisPlay->assign(m_mtvectMSWARINFO_DISPLAY.begin(), m_mtvectMSWARINFO_DISPLAY.end());
}

void CMSWarInfoDisPlay::SetMSWarInfoDisPlay(MSWARINFO_DISPLAY * i_pMSWARINFO_DISPLAY)
{
	mt_auto_lock mta(&m_mtvectMSWARINFO_DISPLAY);

	mtvectMSWARINFO_DISPLAY::iterator itr = m_mtvectMSWARINFO_DISPLAY.begin();
	for(; itr != m_mtvectMSWARINFO_DISPLAY.end(); itr++)
	{
		if(i_pMSWARINFO_DISPLAY->MapIndex == itr->MapIndex)
		{
			itr->Belligerence	= i_pMSWARINFO_DISPLAY->Belligerence;
			itr->HPRate			= i_pMSWARINFO_DISPLAY->HPRate;
			itr->TelePortState	= i_pMSWARINFO_DISPLAY->TelePortState;
			return;
		}
	}

	m_mtvectMSWARINFO_DISPLAY.push_back(*i_pMSWARINFO_DISPLAY);
}

void CMSWarInfoDisPlay::SetMSWarInfoDisPlayByHPRate(MapIndex_t i_nMapindex, INT i_nHPRate)
{
	mt_auto_lock mta(&m_mtvectMSWARINFO_DISPLAY);

	mtvectMSWARINFO_DISPLAY::iterator itr = m_mtvectMSWARINFO_DISPLAY.begin();
	for(; itr != m_mtvectMSWARINFO_DISPLAY.end(); itr++)
	{
		if(i_nMapindex == itr->MapIndex)
		{
			itr->HPRate			= i_nHPRate;
			return;
		}
	}
}

void CMSWarInfoDisPlay::SetMSWarInfoDisPlayByCreateTeleport(MapIndex_t i_nMapindex, INT i_nHPRate, INT i_nTelePortState, BYTE i_byBelligerence)
{
	mt_auto_lock mta(&m_mtvectMSWARINFO_DISPLAY);

	mtvectMSWARINFO_DISPLAY::iterator itr = m_mtvectMSWARINFO_DISPLAY.begin();
	for(; itr != m_mtvectMSWARINFO_DISPLAY.end(); itr++)
	{
		if(i_nMapindex == itr->MapIndex)
		{
			itr->HPRate			= i_nHPRate;
			itr->TelePortState	= i_nTelePortState;
			itr->Belligerence	= i_byBelligerence;
			return;
		}
	}
}

void CMSWarInfoDisPlay::SetMSWarInfoDisPlayByChangeTeleportState(MapIndex_t i_nMapindex, INT i_nHPRate, INT i_nTelePortState, ATUM_DATE_TIME * i_pTelePortBuildingStartTime /* = NULL*/, ATUM_DATE_TIME * i_pTelePortBuildingEndTime /* = NULL*/)
{
	mt_auto_lock mta(&m_mtvectMSWARINFO_DISPLAY);

	mtvectMSWARINFO_DISPLAY::iterator itr = m_mtvectMSWARINFO_DISPLAY.begin();
	for(; itr != m_mtvectMSWARINFO_DISPLAY.end(); itr++)
	{
		if(i_nMapindex == itr->MapIndex)
		{
			itr->HPRate			= i_nHPRate;
			itr->TelePortState	= i_nTelePortState;
			if(TELEPORT_STATE_BUILDING == i_nTelePortState)
			{
				itr->TelePortBuildingStartTime	= *i_pTelePortBuildingStartTime;
				itr->TelePortBuildingEndTime	= *i_pTelePortBuildingEndTime;
			}
			return;
		}
	}
}

void CMSWarInfoDisPlay::SetMSWarOptionType(BYTE i_byLeaderInfluence, SHORT i_byMSWarOptionType)
{
	if(IS_VCN_INFLUENCE_TYPE(i_byLeaderInfluence))
	{
		BCUMSWarOptionType = i_byMSWarOptionType;
	}
	else
	{
		ANIMSWarOptionType = i_byMSWarOptionType;
	}
}

SHORT CMSWarInfoDisPlay::GetMSWarOptionType(BYTE i_byLeaderInfluence)
{	
	if(IS_VCN_INFLUENCE_TYPE(i_byLeaderInfluence))
	{
		return BCUMSWarOptionType;
	}
	else
	{
		return ANIMSWarOptionType;
	}
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// 2015-05-01 by killburne

CStrategicPointDisplay::CStrategicPointDisplay()
{
	InitStrategicPointDisplay();
}

CStrategicPointDisplay::~CStrategicPointDisplay()
{
	InitStrategicPointDisplay();
}

void CStrategicPointDisplay::InitStrategicPointDisplay()
{
	m_mtvectSTRATEGICPOINT_DISPLAY.clear();
}

void CStrategicPointDisplay::GetAllStrategicPointDisplay(vector<SSTRATEGYPOINT_DISPLAY_INFO>* o_VectStrategicPointDisplay)
{
	mt_auto_lock mta(&m_mtvectSTRATEGICPOINT_DISPLAY);

	char szSystemLog[256];
	sprintf(szSystemLog, "Getting All StrategicPointInfoDisplay Count(%d)\r\n", m_mtvectSTRATEGICPOINT_DISPLAY.size());
	g_pFieldGlobal->WriteSystemLog(szSystemLog);
	o_VectStrategicPointDisplay->assign(m_mtvectSTRATEGICPOINT_DISPLAY.begin(), m_mtvectSTRATEGICPOINT_DISPLAY.end());
}

bool CStrategicPointDisplay::GetStrategicPointDisplay(MapIndex_t mapIndex, SSTRATEGYPOINT_DISPLAY_INFO* o_StrategicPointDisplay)
{
	mt_auto_lock mta(&m_mtvectSTRATEGICPOINT_DISPLAY);

	for (auto &info : m_mtvectSTRATEGICPOINT_DISPLAY)
	{
		if (info.MapIndex == mapIndex)
		{
			CopyMemory(o_StrategicPointDisplay, &info, sizeof SSTRATEGYPOINT_DISPLAY_INFO);
			return true;
		}
	}

	return false;
}

void CStrategicPointDisplay::SetStrategicPointDisplay(SSTRATEGYPOINT_DISPLAY_INFO* i_pSTRATEGICPOINT_DISPLAY)
{
	mt_auto_lock mta(&m_mtvectSTRATEGICPOINT_DISPLAY);
	char szSystemLog[256];
	for (auto &strPoint : m_mtvectSTRATEGICPOINT_DISPLAY)
	{
		if (i_pSTRATEGICPOINT_DISPLAY->MapIndex == strPoint.MapIndex)
		{
			strPoint.HPRate = i_pSTRATEGICPOINT_DISPLAY->HPRate;
			
			sprintf(szSystemLog, "Updating StrategicPointInfoDisplay MapIndex(%d) InfluenceType(%d) HPRate(%d)\r\n", i_pSTRATEGICPOINT_DISPLAY->MapIndex, (INT)i_pSTRATEGICPOINT_DISPLAY->InfluenceType, (INT)i_pSTRATEGICPOINT_DISPLAY->HPRate);
			g_pFieldGlobal->WriteSystemLog(szSystemLog);
			return;
		}
	}

	sprintf(szSystemLog, "Adding StrategicPointInfoDisplay MapIndex(%d) InfluenceType(%d)\r\n", i_pSTRATEGICPOINT_DISPLAY->MapIndex, (INT)i_pSTRATEGICPOINT_DISPLAY->InfluenceType);
	g_pFieldGlobal->WriteSystemLog(szSystemLog);
	m_mtvectSTRATEGICPOINT_DISPLAY.push_back(*i_pSTRATEGICPOINT_DISPLAY);
}

void CStrategicPointDisplay::SetStrategicPointDisplay(SSUMMON_STRATEGYPOINT_INFO* i_pSTRATEGICPOINT_INFO)
{
	mt_auto_lock mta(&m_mtvectSTRATEGICPOINT_DISPLAY);

	for (auto &strPoint : m_mtvectSTRATEGICPOINT_DISPLAY)
	{
		if (i_pSTRATEGICPOINT_INFO->MapIndex == strPoint.MapIndex)
		{
			return;
		}
	}

	char szSystemLog[256];
	sprintf(szSystemLog, "Adding StrategicPointInfoDisplay MapIndex(%d) InfluenceType(%d)\r\n", i_pSTRATEGICPOINT_INFO->MapIndex, (INT)i_pSTRATEGICPOINT_INFO->InfluenceType);
	g_pFieldGlobal->WriteSystemLog(szSystemLog);
	SSTRATEGYPOINT_DISPLAY_INFO display;
	util::zero(&display, sizeof SSTRATEGYPOINT_DISPLAY_INFO);
	display.HPRate = MS_WAR_HP_RATE;
	display.InfluenceType = i_pSTRATEGICPOINT_INFO->InfluenceType;
	display.MapIndex = i_pSTRATEGICPOINT_INFO->MapIndex;
	display.SummonStrategyPointTime = i_pSTRATEGICPOINT_INFO->SummonStrategyPointTime;
	
	m_mtvectSTRATEGICPOINT_DISPLAY.push_back(display);
}

void CStrategicPointDisplay::SetStrategicPointDisplayHP(MapIndex_t i_nMapindex, FLOAT i_nHPRate)
{
	mt_auto_lock mta(&m_mtvectSTRATEGICPOINT_DISPLAY);

	for (auto &strPoint : m_mtvectSTRATEGICPOINT_DISPLAY)
	{
		if (strPoint.MapIndex == i_nMapindex)
		{
			strPoint.HPRate = i_nHPRate;
			char szSystemLog[256];
			sprintf(szSystemLog, "Updating StrategicPointInfoDisplayHP MapIndex(%d) HPRate(%d)\r\n", i_nMapindex, (INT)i_nHPRate);
			g_pFieldGlobal->WriteSystemLog(szSystemLog);
			break;
		}
	}
}

void CStrategicPointDisplay::RemoveStrategicPointDisplay(MapIndex_t i_nMapindex)
{
	mt_auto_lock mta(&m_mtvectSTRATEGICPOINT_DISPLAY);

	vector<SSTRATEGYPOINT_DISPLAY_INFO>::iterator it = find_if(m_mtvectSTRATEGICPOINT_DISPLAY.begin(), m_mtvectSTRATEGICPOINT_DISPLAY.end(), [i_nMapindex](const SSTRATEGYPOINT_DISPLAY_INFO &strPoint){
		return strPoint.MapIndex == i_nMapindex;
	});
	if (it == m_mtvectSTRATEGICPOINT_DISPLAY.end())
	{
		return;
	}
	char szSystemLog[256];
	sprintf(szSystemLog, "Removing StrategicPointInfoDisplay MapIndex(%d)\r\n", i_nMapindex);
	g_pFieldGlobal->WriteSystemLog(szSystemLog);
	m_mtvectSTRATEGICPOINT_DISPLAY.erase(it);
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// 2009-01-12 by dhjin, 선전 포고
CDeclarationOfWar::CDeclarationOfWar()
{
	this->InitDeclarationOfWar();
}

CDeclarationOfWar::~CDeclarationOfWar()
{

}

void CDeclarationOfWar::InitDeclarationOfWar()
{
	m_mtvectSDECLARATION_OF_WAR.clear();
	m_pCurrentBCUWarStepInfo = NULL;
	m_pCurrentANIWarStepInfo = NULL;
	m_pBeforeBCUWarStartTime.Reset();
	m_pBeforeANIWarStartTime.Reset();
	util::zero(&m_ForbidTime, sizeof(SDECLARATION_OF_WAR_FORBID_TIME));
}

BOOL CDeclarationOfWar::SetDBDeclarationOfWar(mtvectSDECLARATION_OF_WAR * i_pVectDeclarationOfWar, SDECLARATION_OF_WAR_FORBID_TIME * i_pForbidTime)
{
	mt_auto_lock mta(&m_mtvectSDECLARATION_OF_WAR);
	m_mtvectSDECLARATION_OF_WAR.clear();
	m_mtvectSDECLARATION_OF_WAR.assign(i_pVectDeclarationOfWar->begin(), i_pVectDeclarationOfWar->end());


	///////////////////////////////////////////////////////////////////////////////
	// 2009-07-02 by cmkwon, 선전포고 관련 버그 수정 - 스텝은 BCU 세력 정보를 ANI세력도 같도록 설정한다.
	mtvectSDECLARATION_OF_WAR::iterator itr = m_mtvectSDECLARATION_OF_WAR.begin();
	for(; itr != m_mtvectSDECLARATION_OF_WAR.end(); itr++)
	{
		SDECLARATION_OF_WAR *pTmWar = &*itr;
		if(pTmWar->Influence == INFLUENCE_TYPE_ANI)
		{
			SDECLARATION_OF_WAR *pBCUWarStep = this->FindMSWarStep(INFLUENCE_TYPE_VCN, pTmWar->MSWarStep);
			if(NULL == pBCUWarStep)
			{
				// 2009-07-02 by cmkwon, 서버 실행시 실행 관련 로그 추가 - 	
				g_pFieldGlobal->WriteSystemLogEX(TRUE, "  [Notify] DeclarationNoti CDeclarationOfWar::SetDBDeclarationOfWar# Check error ANI Step(%d), BCU StepPtr(0x%X) \r\n", pTmWar->MSWarStep, pBCUWarStep);
				return FALSE;
			}
			pTmWar->MSWarStepStartTime		= pBCUWarStep->MSWarStepStartTime;
			pTmWar->MSWarStepEndTime		= pBCUWarStep->MSWarStepEndTime;
		}
	}

	m_pCurrentBCUWarStepInfo	= this->FindMSWarStepByCurrentTime(INFLUENCE_TYPE_VCN);
	m_pCurrentANIWarStepInfo	= this->FindMSWarStepByCurrentTime(INFLUENCE_TYPE_ANI);

// 2009-07-02 by cmkwon, 선전포고 관련 버그 수정 - 위와 같이 수정 함
// 	ATUM_DATE_TIME CurrentTime;
// 	CurrentTime.SetCurrentDateTime();
// 
// 	bool CheckBCUTimeSet = FALSE;
// 	bool CheckANITimeSet = FALSE;
// 
// 	mtvectSDECLARATION_OF_WAR::iterator itr = m_mtvectSDECLARATION_OF_WAR.begin();
// 	for(; itr != m_mtvectSDECLARATION_OF_WAR.end(); itr++)
// 	{
// 		if(CheckBCUTimeSet
// 			&& CheckANITimeSet)
// 		{// 2009-02-09 by dhjin, 선전 포고 - BCU, ANI 둘 다 설정 되었다면 나간다.
// 			break;
// 		}
// 		
// 		if(!CheckBCUTimeSet
// 			&& INFLUENCE_TYPE_VCN == itr->Influence
// 			&& itr->MSWarStepEndTime > CurrentTime
// 			)
// 		{// 2009-02-09 by dhjin, 선전 포고 - 현재 시간에 포함 되거나 늦은 시간을 설정한다.
// 			m_pCurrentBCUWarStepInfo = itr;
// 			CheckBCUTimeSet = TRUE;
// 		}
// 		if(!CheckANITimeSet
// 			&& INFLUENCE_TYPE_ANI == itr->Influence
// 			&& itr->MSWarStepEndTime > CurrentTime
// 			)
// 		{
// 			m_pCurrentANIWarStepInfo = itr;
// 			CheckANITimeSet = TRUE;
// 		}
// 	}

	if(NULL == m_pCurrentBCUWarStepInfo
		|| NULL ==  m_pCurrentANIWarStepInfo)
	{
		g_pFieldGlobal->WriteSystemLogEX(TRUE, "[Error] DeclarationOfWar : Can't set period of DeclarationOfWar !!, BUC(%s) ANI(%s) \r\n"
			, m_pCurrentBCUWarStepInfo==NULL?"NULL":m_pCurrentBCUWarStepInfo->MSWarStepStartTime.GetDateTimeString().GetBuffer()
			, m_pCurrentANIWarStepInfo==NULL?"NULL":m_pCurrentANIWarStepInfo->MSWarStepStartTime.GetDateTimeString().GetBuffer());
		return FALSE;	
	}

	// 2009-07-02 by cmkwon, 서버 실행시 실행 관련 로그 추가 - 	
	g_pFieldGlobal->WriteSystemLogEX(TRUE, "  [Notify] DeclarationNoti CDeclarationOfWar::SetDBDeclarationOfWar# 200 BUC(%s ~ %s, Step(%2d) MSNum(%d) EndState(%d)) \r\n"
		, m_pCurrentBCUWarStepInfo->MSWarStepStartTime.GetDateTimeString().GetBuffer(), m_pCurrentBCUWarStepInfo->MSWarStepEndTime.GetDateTimeString().GetBuffer()
		, m_pCurrentBCUWarStepInfo->MSWarStep, m_pCurrentBCUWarStepInfo->MSNum, m_pCurrentBCUWarStepInfo->MSWarEndState);
	g_pFieldGlobal->WriteSystemLogEX(TRUE, "  [Notify] DeclarationNoti CDeclarationOfWar::SetDBDeclarationOfWar# 300 ANI(%s ~ %s Step(%2d) MSNum(%d) EndState(%d)) \r\n"
		, m_pCurrentANIWarStepInfo->MSWarStepStartTime.GetDateTimeString().GetBuffer(), m_pCurrentANIWarStepInfo->MSWarStepEndTime.GetDateTimeString().GetBuffer()
		, m_pCurrentANIWarStepInfo->MSWarStep, m_pCurrentANIWarStepInfo->MSNum, m_pCurrentANIWarStepInfo->MSWarEndState);

	util::zero(&m_ForbidTime, sizeof(SDECLARATION_OF_WAR_FORBID_TIME));
	m_ForbidTime.DayOfWeek			= i_pForbidTime->DayOfWeek;
	m_ForbidTime.ForbidStartTime	= i_pForbidTime->ForbidStartTime;
	m_ForbidTime.ForbidEndTime		= i_pForbidTime->ForbidEndTime;

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CDeclarationOfWar::SetBeforeWarStartTime(INT i_nBCUBeforeMin, INT i_nANIBeforeMin)
/// \brief		전쟁단계로 모선전 분위기 시작 시간을 설정한다.
/// \author		dhjin
/// \date		2009-01-14 ~ 2009-01-14
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CDeclarationOfWar::SetBeforeWarStartTime(INT i_nBCUBeforeMin, INT i_nANIBeforeMin)
{
	mt_auto_lock mta(&m_mtvectSDECLARATION_OF_WAR);
	m_pBeforeBCUWarStartTime = m_pCurrentBCUWarStepInfo->MSWarStartTime;
	m_pBeforeBCUWarStartTime.AddDateTime(0,0,0,0, -i_nBCUBeforeMin,0);
	m_pBeforeANIWarStartTime = m_pCurrentANIWarStepInfo->MSWarStartTime;
	m_pBeforeANIWarStartTime.AddDateTime(0,0,0,0, -i_nANIBeforeMin,0);

	// 2009-07-02 by cmkwon, 선전포고 관련 버그 수정 - 
	if(m_pCurrentBCUWarStepInfo->MSWarStepStartTime > m_pBeforeBCUWarStartTime)
	{
		m_pBeforeBCUWarStartTime = m_pCurrentBCUWarStepInfo->MSWarStepStartTime;
	}
	if(m_pCurrentANIWarStepInfo->MSWarStepStartTime > m_pBeforeANIWarStartTime)
	{
		m_pBeforeANIWarStartTime = m_pCurrentANIWarStepInfo->MSWarStepStartTime;
	}
}

BYTE CDeclarationOfWar::GetCurrentMSWarStep(BYTE i_byInfl)
{
	mt_auto_lock mta(&m_mtvectSDECLARATION_OF_WAR);

	if(INFLUENCE_TYPE_VCN == i_byInfl)
	{
		return m_pCurrentBCUWarStepInfo->MSWarStep;
	}
	else if(INFLUENCE_TYPE_ANI == i_byInfl)
	{
		return m_pCurrentANIWarStepInfo->MSWarStep;
	}

	return FALSE;
}

BYTE CDeclarationOfWar::GetCurrentMSWarEndState(BYTE i_byInfl)
{
	mt_auto_lock mta(&m_mtvectSDECLARATION_OF_WAR);

	if(INFLUENCE_TYPE_VCN == i_byInfl)
	{
		return m_pCurrentBCUWarStepInfo->MSWarEndState;
	}
	else if(INFLUENCE_TYPE_ANI == i_byInfl)
	{
		return m_pCurrentANIWarStepInfo->MSWarEndState;
	}

	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CDeclarationOfWar::GetBeforeMSWarEndInfo(BYTE i_byInfl, INT * o_nMSBossMonsterUID, BYTE * o_MSWarEndState)
/// \brief		
/// \author		dhjin
/// \date		2009-03-10 ~ 2009-03-10
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CDeclarationOfWar::GetBeforeMSWarEndInfo(BYTE i_byInfl, INT * o_nMSBossMonsterUID, BYTE * o_MSWarEndState)
{
	mt_auto_lock mta(&m_mtvectSDECLARATION_OF_WAR);

	BYTE BeforeStep = 0;
	if(1 < m_pCurrentBCUWarStepInfo->MSWarStep
		&& MSWAR_NEXT_LEADER_SETP > m_pCurrentBCUWarStepInfo->MSWarStep)
	{
		BeforeStep = m_pCurrentBCUWarStepInfo->MSWarStep - 1;
	}
	else if(1 == m_pCurrentBCUWarStepInfo->MSWarStep)
	{
		BeforeStep = 1;
	}
	else
	{
		return;
	}

	mtvectSDECLARATION_OF_WAR::iterator itr = m_mtvectSDECLARATION_OF_WAR.begin();
	for(; itr != m_mtvectSDECLARATION_OF_WAR.end(); itr++)
	{
		if(i_byInfl == itr->Influence
			&& BeforeStep == itr->MSWarStep)
		{
			*o_nMSBossMonsterUID = itr->MSNum;
			*o_MSWarEndState = itr->MSWarEndState;
			return;
		}
	}	
	
	return;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			INT CDeclarationOfWar::GetCurrentMSUID(BYTE i_byInfl)
/// \brief		단계별 모선 시스템 - 현재 주기 소환된 모선UID값 가져오기
/// \author		dhjin
/// \date		2009-03-10 ~ 2009-03-10
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
INT CDeclarationOfWar::GetCurrentMSUID(BYTE i_byInfl)
{
	mt_auto_lock mta(&m_mtvectSDECLARATION_OF_WAR);

	if(INFLUENCE_TYPE_VCN == i_byInfl)
	{
		return m_pCurrentBCUWarStepInfo->MSNum;
	}
	else if(INFLUENCE_TYPE_ANI == i_byInfl)
	{
		return m_pCurrentANIWarStepInfo->MSNum;
	}

	return FALSE;
}

void CDeclarationOfWar::SetMSWarStepByMSWarStart(SDECLARATION_OF_WAR * i_pDeclarationOfWar)
{
	mt_auto_lock mta(&m_mtvectSDECLARATION_OF_WAR);
	if(m_pCurrentBCUWarStepInfo->Influence == i_pDeclarationOfWar->Influence)
	{
		m_pCurrentBCUWarStepInfo->MSAppearanceMap	= i_pDeclarationOfWar->MSAppearanceMap;
		m_pCurrentBCUWarStepInfo->MSNum				= i_pDeclarationOfWar->MSNum;
		m_pCurrentBCUWarStepInfo->MSWarEndState		= MSWARING;
		m_pCurrentBCUWarStepInfo->NCP				= i_pDeclarationOfWar->NCP;
	}
	else if(m_pCurrentANIWarStepInfo->Influence == i_pDeclarationOfWar->Influence)
	{
		m_pCurrentANIWarStepInfo->MSAppearanceMap	= i_pDeclarationOfWar->MSAppearanceMap;
		m_pCurrentANIWarStepInfo->MSNum				= i_pDeclarationOfWar->MSNum;
		m_pCurrentANIWarStepInfo->MSWarEndState		= MSWARING;
		m_pCurrentANIWarStepInfo->NCP				= i_pDeclarationOfWar->NCP;
	}
}

void CDeclarationOfWar::SetMSWarEndState(BYTE i_byInfl, INT i_nEndState)
{
	mt_auto_lock mta(&m_mtvectSDECLARATION_OF_WAR);

	if(INFLUENCE_TYPE_VCN == i_byInfl)
	{
		m_pCurrentBCUWarStepInfo->MSWarEndState = i_nEndState;
	}
	else if(INFLUENCE_TYPE_ANI == i_byInfl)
	{
		m_pCurrentANIWarStepInfo->MSWarEndState = i_nEndState;
	}
}

Err_t CDeclarationOfWar::SetSelectWarTime(MSG_FC_INFO_DECLARATION_MSWAR_SET * i_pMsg, BYTE i_byInfl, BYTE * o_bySelect, ATUM_DATE_TIME * o_MSWarStartTime)
{
	mt_auto_lock mta(&m_mtvectSDECLARATION_OF_WAR);

	///////////////////////////////////////////////////////////////////////////////
	// 2009-07-02 by cmkwon, 선전포고 관련 버그 수정 - 아래와 같이 수정 함.
	SDECLARATION_OF_WAR *pNextWarStep = (i_byInfl == INFLUENCE_TYPE_VCN) ? this->FindNextMSWarStep(m_pCurrentBCUWarStepInfo) : this->FindNextMSWarStep(m_pCurrentANIWarStepInfo);
	if(NULL == pNextWarStep
		|| pNextWarStep->MSWarStep != i_pMsg->MSWarStep)
	{
		return ERR_NO_SUCH_DECLARATION_MSWAR_INFO;
	}

	if(0 >= pNextWarStep->SelectCount)
	{
		return ERR_NO_MORE_DECLARATION_MSWAR_SET;
	}

	if(i_pMsg->GiveUp)
	{
		pNextWarStep->GiveUp			= TRUE;
	}
	else
	{
		ATUM_DATE_TIME tmLimitEndTime = pNextWarStep->MSWarStepEndTime;
		tmLimitEndTime.AddDateTime(0, 0, 0, 0, -180);
		if(CheckForbidTime(&(i_pMsg->MSWarStartTime))
			|| pNextWarStep->MSWarStepStartTime > i_pMsg->MSWarStartTime
			|| tmLimitEndTime < i_pMsg->MSWarStartTime)
		{// 2009-07-02 by cmkwon, 선전포고 지도자 전쟁 시간 설정 관련 수정 - (스텝 시작 시간) ~ (스텝 종료 시간 - 180분<3시간>), 제한 시간 불가 <== 모선전도 해당 스텝 기간안에 종료가 되어야 한다.
			return ERR_INVALID_DECLARATION_MSWAR_TIME;
		}
		pNextWarStep->MSWarStartTime	= i_pMsg->MSWarStartTime;				
		pNextWarStep->GiveUp			= FALSE;
	}
	pNextWarStep->SelectCount--;
	*o_MSWarStartTime	= pNextWarStep->MSWarStartTime;
	*o_bySelect			= pNextWarStep->SelectCount;	
	return ERR_NO_ERROR;

// 2009-07-02 by cmkwon, 선전포고 관련 버그 수정 - 위와 같이 수정 함.
// 	BYTE BeforeMSWarStep = 0;
// 
// 	mtvectSDECLARATION_OF_WAR::iterator itr = m_mtvectSDECLARATION_OF_WAR.begin();
// 	for(; itr != m_mtvectSDECLARATION_OF_WAR.end(); itr++)
// 	{
// 		if(i_byInfl == itr->Influence 
// 			&& i_pMsg->MSWarStep == itr->MSWarStep
// 			)
// 		{
// 			if(0 == itr->SelectCount)
// 			{
// 				return ERR_NO_MORE_DECLARATION_MSWAR_SET;
// 			}
// 	
// 			if(i_pMsg->GiveUp)
// 			{
// 				itr->GiveUp = TRUE;
// 			}
// 			else
// 			{
// 				if(CheckForbidTime(&(i_pMsg->MSWarStartTime))
// 					|| itr->MSWarStepStartTime > i_pMsg->MSWarStartTime
// 					|| itr->MSWarStepEndTime < i_pMsg->MSWarStartTime
// 					|| BeforeMSWarStep != m_pCurrentBCUWarStepInfo->MSWarStep
// 					)
// 				{
// 					return ERR_INVALID_DECLARATION_MSWAR_TIME;
// 				}
// 				itr->MSWarStartTime = i_pMsg->MSWarStartTime;				
// 				itr->GiveUp = FALSE;
// 			}
// 			itr->SelectCount--;
// 			*o_MSWarStartTime	= itr->MSWarStartTime;
// 			*o_bySelect = itr->SelectCount;
// 
// 			return ERR_NO_ERROR;
// 		}
// 		BeforeMSWarStep = itr->MSWarStep;
// 	}	
// 
// 	return ERR_NO_SUCH_DECLARATION_MSWAR_INFO;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CDeclarationOfWar::SetMSWarFinish(BYTE i_bySummonInfl, BYTE i_byWinInfl, ATUM_DATE_TIME * i_pEndTime)
/// \brief		전쟁 종료 후 값 설정
/// \author		dhjin
/// \date		2009-01-13 ~ 2009-01-13
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CDeclarationOfWar::SetMSWarFinish(BYTE i_bySummonInfl, BYTE i_byWinInfl, ATUM_DATE_TIME * i_pEndTime)
{
	mt_auto_lock mta(&m_mtvectSDECLARATION_OF_WAR);

	if(INFLUENCE_TYPE_ANI == i_bySummonInfl)
	{// 알링턴 모선이라면 바이제니유에서 공격한 모선이다.
		if(i_bySummonInfl != i_byWinInfl)
		{
			m_pCurrentBCUWarStepInfo->MSWarEndState = MSWAR_END_WIN;
		}
		else
		{
			m_pCurrentBCUWarStepInfo->MSWarEndState = MSWAR_END_LOSS;
		}
		m_pCurrentBCUWarStepInfo->MSWarEndTime = *i_pEndTime;
		
	}
	else if(INFLUENCE_TYPE_VCN == i_bySummonInfl)
	{
		if(i_bySummonInfl != i_byWinInfl)
		{
			m_pCurrentANIWarStepInfo->MSWarEndState = MSWAR_END_WIN;
		}
		else
		{
			m_pCurrentANIWarStepInfo->MSWarEndState = MSWAR_END_LOSS;
		}
		m_pCurrentANIWarStepInfo->MSWarEndTime = *i_pEndTime;
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CDeclarationOfWar::CheckNextMSWarStep(ATUM_DATE_TIME * i_pCurrentTime)
/// \brief		다음 전쟁 주기인지 확인
///				// 2009-07-02 by cmkwon, 선전포고 관련 버그 수정 - 
/// \author		dhjin
/// \date		2009-01-14 ~ 2009-01-14
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CDeclarationOfWar::CheckNextMSWarStep(ATUM_DATE_TIME * i_pCurrentTime) 
{
	mt_auto_lock mta(&m_mtvectSDECLARATION_OF_WAR);

	///////////////////////////////////////////////////////////////////////////////
	// 2009-07-02 by cmkwon, 선전포고 관련 버그 수정 - 아래와 같이 수정함.
	if(m_pCurrentBCUWarStepInfo->MSWarStepEndTime > *i_pCurrentTime
		|| m_pCurrentBCUWarStepInfo->MSWarStep == MSWAR_NEXT_LEADER_SETP)
	{
		return FALSE;
	}

	SDECLARATION_OF_WAR *pNextBCUWarStep = this->FindNextMSWarStep(m_pCurrentBCUWarStepInfo);
	SDECLARATION_OF_WAR *pNextANIWarStep = this->FindNextMSWarStep(m_pCurrentANIWarStepInfo);
	if(NULL == pNextBCUWarStep
		|| NULL == pNextANIWarStep)
	{
		g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ERROR] DeclarationNoti CDeclarationOfWar::CheckNextMSWarStep# BUC(Next:0x%X Cur: %d) ANINextStep(Next:0x%X Cur: %d) \r\n"
			, pNextBCUWarStep, m_pCurrentBCUWarStepInfo->MSWarStep, pNextANIWarStep, m_pCurrentANIWarStepInfo->MSWarStep);
		return FALSE;
	}

	m_pCurrentBCUWarStepInfo	= pNextBCUWarStep;
	m_pCurrentANIWarStepInfo	= pNextANIWarStep;
	
	g_pFieldGlobal->WriteSystemLogEX(TRUE, "  [Notify] DeclarationNoti CDeclarationOfWar::CheckNextMSWarStep# BUC(%s~%s %2d %8d) ANI(%s~%s %2d %8d) \r\n"
		, m_pCurrentBCUWarStepInfo->MSWarStepStartTime.GetDateTimeString().GetBuffer(), m_pCurrentBCUWarStepInfo->MSWarStepEndTime.GetDateTimeString().GetBuffer()
		, m_pCurrentBCUWarStepInfo->MSWarStep, m_pCurrentBCUWarStepInfo->MSNum
		, m_pCurrentANIWarStepInfo->MSWarStepStartTime.GetDateTimeString().GetBuffer(), m_pCurrentANIWarStepInfo->MSWarStepEndTime.GetDateTimeString().GetBuffer()
		, m_pCurrentANIWarStepInfo->MSWarStep, m_pCurrentANIWarStepInfo->MSNum);
	return TRUE;

// 2009-07-02 by cmkwon, 선전포고 관련 버그 수정 - 위와 같이 수정 함.
//	ATUM_DATE_TIME CurrentTime;
//	CurrentTime.SetCurrentDateTime();
//	BOOL bCheckBCUUpdate = FALSE;
//	BOOL bCheckANIUpdate = FALSE;
//
// 	if(m_pCurrentBCUWarStepInfo->MSWarStepEndTime <= CurrentTime)
// 	{// 주기가 지났다면 
// 		mtvectSDECLARATION_OF_WAR::iterator itr = m_mtvectSDECLARATION_OF_WAR.begin();
// 		for(; itr != m_mtvectSDECLARATION_OF_WAR.end(); itr++)
// 		{
// 			if(MSWAR_NEXT_LEADER_SETP > itr->MSWarStep)
// 			{
// 				if(m_pCurrentBCUWarStepInfo->Influence == itr->Influence
// 					&& m_pCurrentBCUWarStepInfo->MSWarStep+1 == itr->MSWarStep
// 					&& FALSE == bCheckBCUUpdate
// 					)
// 				{// 다음주기로 업데이트
// 					m_pCurrentBCUWarStepInfo = itr;
// 					bCheckBCUUpdate = TRUE;
// 				}
// 				if(m_pCurrentANIWarStepInfo->Influence == itr->Influence
// 					&& m_pCurrentANIWarStepInfo->MSWarStep+1 == itr->MSWarStep
// 					&& FALSE == bCheckANIUpdate
// 					)
// 				{
// 					m_pCurrentANIWarStepInfo = itr;
// 					bCheckANIUpdate = TRUE;
// 				}
// 			}
// // 2009-03-16 by dhjin, 선전 포고 - 버그 밑과 같이 수정
// //			else if(MSWAR_NEXT_LEADER_SETP == itr->MSWarStep
// //				&& MSWAR_FINAL_SETP == m_pCurrentBCUWarStepInfo->MSWarStep)
// //			{
// //				return FALSE;
// //			}
// 			else if(m_pCurrentBCUWarStepInfo->Influence == itr->Influence
// 				&& MSWAR_NEXT_LEADER_SETP == itr->MSWarStep
// 				&& FALSE == bCheckBCUUpdate)
// 			{
// 				m_pCurrentBCUWarStepInfo = itr;
// 				bCheckBCUUpdate = TRUE;
// 			}
// 			else if(m_pCurrentANIWarStepInfo->Influence == itr->Influence
// 				&& MSWAR_NEXT_LEADER_SETP == itr->MSWarStep
// 				&& FALSE == bCheckANIUpdate)
// 			{
// 				m_pCurrentANIWarStepInfo = itr;
// 				bCheckANIUpdate = TRUE;
// 			}
// 
// 			if(bCheckBCUUpdate
// 				&& bCheckANIUpdate)
// 			{
// 				return TRUE;
// 			}
// 		}
// 	}
// 	
// 	return FALSE;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2009-07-02 by cmkwon, 선전포고 관련 버그 수정 - 
/// \author		cmkwon
/// \date		2009-07-02 ~ 2009-07-02
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
SDECLARATION_OF_WAR * CDeclarationOfWar::FindNextMSWarStep(SDECLARATION_OF_WAR *i_pCurWarStep)
{
	mt_auto_lock mta(&m_mtvectSDECLARATION_OF_WAR);
	
	SDECLARATION_OF_WAR *pLastWarStep = NULL;
	mtvectSDECLARATION_OF_WAR::iterator itr = m_mtvectSDECLARATION_OF_WAR.begin();
	for(; itr != m_mtvectSDECLARATION_OF_WAR.end(); itr++)
	{
		SDECLARATION_OF_WAR *pTmWar = &*itr;
		if(pTmWar->Influence != i_pCurWarStep->Influence)
		{// 2009-07-02 by cmkwon, 세력이 다르면 continue
			continue;
		}

		if(pTmWar->MSWarStep == i_pCurWarStep->MSWarStep+1)
		{
			return pTmWar;
		}
		if(MSWAR_NEXT_LEADER_SETP == pTmWar->MSWarStep)
		{
			pLastWarStep = pTmWar;
		}
	}

	return pLastWarStep;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2009-07-02 by cmkwon, 선전포고 관련 버그 수정 - 
/// \author		cmkwon
/// \date		2009-07-02 ~ 2009-07-02
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
SDECLARATION_OF_WAR * CDeclarationOfWar::FindMSWarStep(BYTE i_byInfl, BYTE i_byMSWarStep)
{
	mt_auto_lock mta(&m_mtvectSDECLARATION_OF_WAR);
	mtvectSDECLARATION_OF_WAR::iterator itr = m_mtvectSDECLARATION_OF_WAR.begin();
	for(; itr != m_mtvectSDECLARATION_OF_WAR.end(); itr++)
	{
		SDECLARATION_OF_WAR *pTmWar = &*itr;
		if(pTmWar->Influence == i_byInfl
			&& pTmWar->MSWarStep == i_byMSWarStep)
		{
			return pTmWar;
		}
	}

	return NULL;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2009-07-02 by cmkwon, 선전포고 관련 버그 수정 - 
/// \author		cmkwon
/// \date		2009-07-02 ~ 2009-07-02
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
SDECLARATION_OF_WAR * CDeclarationOfWar::FindMSWarStepByCurrentTime(BYTE i_byInfl)
{
	mt_auto_lock mta(&m_mtvectSDECLARATION_OF_WAR);
	ATUM_DATE_TIME atCurTime { true };

	mtvectSDECLARATION_OF_WAR::iterator itr = m_mtvectSDECLARATION_OF_WAR.begin();
	for(; itr != m_mtvectSDECLARATION_OF_WAR.end(); itr++)
	{
		SDECLARATION_OF_WAR *pTmWar = &*itr;
		if(pTmWar->Influence == i_byInfl
			&& pTmWar->MSWarStepEndTime > atCurTime)
		{
			return pTmWar;
		}
	}
	
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2009-07-02 by cmkwon, 선전포고 관련 버그 수정 - 
/// \author		cmkwon
/// \date		2009-07-03 ~ 2009-07-03
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BYTE CDeclarationOfWar::GetMostMSWarStepOfWinStep(BYTE i_byInfl, int *o_pnBossMonNum)
{
	mt_auto_lock mta(&m_mtvectSDECLARATION_OF_WAR);

	SDECLARATION_OF_WAR *pMostStepOfWin = NULL;
	mtvectSDECLARATION_OF_WAR::iterator itr = m_mtvectSDECLARATION_OF_WAR.begin();
	for(; itr != m_mtvectSDECLARATION_OF_WAR.end(); itr++)
	{
		SDECLARATION_OF_WAR *pTmWar = &*itr;
		if(pTmWar->Influence != i_byInfl
			|| MSWAR_END_WIN != pTmWar->MSWarEndState)
		{
			continue;
		}

		if(NULL == pMostStepOfWin
			|| pMostStepOfWin->MSWarStep < pTmWar->MSWarStep)
		{
			pMostStepOfWin = pTmWar;
		}
	}
	
	if(NULL == pMostStepOfWin)
	{
		return 0;
	}

	*o_pnBossMonNum		= pMostStepOfWin->MSNum;
	return pMostStepOfWin->MSWarStep;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CDeclarationOfWar::CheckBCUMSWarBeforeStartTime(ATUM_DATE_TIME * i_pCurrentTime)
/// \brief		전쟁기운 시작 시간인지 체크
/// \author		dhjin
/// \date		2009-01-13 ~ 2009-01-13
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CDeclarationOfWar::CheckBCUMSWarBeforeStartTime(ATUM_DATE_TIME * i_pCurrentTime)
{
	mt_auto_lock mta(&m_mtvectSDECLARATION_OF_WAR);

// 2009-07-02 by cmkwon, 선전포고 관련 버그 수정 - 	
// 	if(MSWAR_NOT_START == m_pCurrentBCUWarStepInfo->MSWarEndState
// 		&& m_pBeforeBCUWarStartTime <= *i_pCurrentTime
// 		&& FALSE == m_pCurrentBCUWarStepInfo->GiveUp
// 		)
// 	{
// 		return TRUE;
// 	}
//	
//	return FALSE;
	///////////////////////////////////////////////////////////////////////////////
	// 2009-07-02 by cmkwon, 선전포고 관련 버그 수정 - 
	if(MSWAR_NOT_START != m_pCurrentBCUWarStepInfo->MSWarEndState
		|| m_pCurrentBCUWarStepInfo->GiveUp)
	{// 2009-07-06 by cmkwon, 
		return FALSE;
	}

	if(m_pBeforeBCUWarStartTime > *i_pCurrentTime)
	{// 2009-07-06 by cmkwon, 현재 시간이 전쟁준비 시간에 미치지 않는다
		return FALSE;
	}
	
	if(m_pCurrentBCUWarStepInfo->MSWarStartTime < *i_pCurrentTime)
	{// 2009-07-06 by cmkwon, 현재 시간이 전쟁 시작시간 보다 경과 되었으면 이번 스텝의 전쟁은 진행되지 않는다.
		return FALSE;
	}

	g_pFieldGlobal->WriteSystemLogEX(TRUE, "  [Notify] DeclarationNoti CDeclarationOfWar::CheckBCUMSWarBeforeStartTime# BCU BeforeStartTime(%s) StartTime(%s) \r\n"
		, m_pBeforeBCUWarStartTime.GetDateTimeString().GetBuffer(), m_pCurrentBCUWarStepInfo->MSWarStartTime.GetDateTimeString().GetBuffer());
	return TRUE;	
}

BOOL CDeclarationOfWar::CheckANIMSWarBeforeStartTime(ATUM_DATE_TIME * i_pCurrentTime)
{
	mt_auto_lock mta(&m_mtvectSDECLARATION_OF_WAR);
	
// 2009-07-02 by cmkwon, 선전포고 관련 버그 수정 - 	
// 	if(MSWAR_NOT_START == m_pCurrentANIWarStepInfo->MSWarEndState
// 		&& m_pBeforeANIWarStartTime <= *i_pCurrentTime
// 		&& FALSE == m_pCurrentANIWarStepInfo->GiveUp
// 		)
// 	{
// 		return TRUE;
// 	}
// 	
// 	return FALSE;
	///////////////////////////////////////////////////////////////////////////////
	// 2009-07-02 by cmkwon, 선전포고 관련 버그 수정 - 	
	if(MSWAR_NOT_START != m_pCurrentANIWarStepInfo->MSWarEndState
		|| m_pCurrentANIWarStepInfo->GiveUp)
	{// 2009-07-06 by cmkwon, 
		return FALSE;
	}
	
	if(m_pBeforeANIWarStartTime > *i_pCurrentTime)
	{// 2009-07-06 by cmkwon, 현재 시간이 전쟁준비 시간에 미치지 않는다
		return FALSE;
	}
	
	if(m_pCurrentANIWarStepInfo->MSWarStartTime < *i_pCurrentTime)
	{// 2009-07-06 by cmkwon, 현재 시간이 전쟁 시작시간 보다 경과 되었으면 이번 스텝의 전쟁은 진행되지 않는다.
		return FALSE;
	}

	g_pFieldGlobal->WriteSystemLogEX(TRUE, "  [Notify] DeclarationNoti CDeclarationOfWar::CheckBCUMSWarBeforeStartTime# ANI BeforeStartTime(%s) StartTime(%s) \r\n"
		, m_pBeforeANIWarStartTime.GetDateTimeString().GetBuffer(), m_pCurrentANIWarStepInfo->MSWarStartTime.GetDateTimeString().GetBuffer());
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CDeclarationOfWar::CheckBCUMSWarStartTime(ATUM_DATE_TIME * i_pCurrentTime)
/// \brief		전쟁 시작 시간인지 체크
/// \author		dhjin
/// \date		2009-01-12 ~ 2009-01-12
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CDeclarationOfWar::CheckBCUMSWarStartTime(ATUM_DATE_TIME * i_pCurrentTime)
{	
	mt_auto_lock mta(&m_mtvectSDECLARATION_OF_WAR);
	
	if(MSWARING_BEFORE == m_pCurrentBCUWarStepInfo->MSWarEndState
		&& m_pCurrentBCUWarStepInfo->MSWarStartTime <= *i_pCurrentTime 
		&& FALSE == m_pCurrentBCUWarStepInfo->GiveUp
		)
	{
		return TRUE;
	}
	
	return FALSE;
}

BOOL CDeclarationOfWar::CheckANIMSWarStartTime(ATUM_DATE_TIME * i_pCurrentTime)
{	
	mt_auto_lock mta(&m_mtvectSDECLARATION_OF_WAR);
	
	if(MSWARING_BEFORE == m_pCurrentANIWarStepInfo->MSWarEndState
		&& m_pCurrentANIWarStepInfo->MSWarStartTime <= *i_pCurrentTime 
		&& FALSE == m_pCurrentANIWarStepInfo->GiveUp
		)
	{
		return TRUE;
	}
	
	return FALSE;	
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CDeclarationOfWar::CheckForbidTime(ATUM_DATE_TIME * i_pHopeTime)
/// \brief		전쟁 시작 시간으로 부적합한 시간인지 체크
/// \author		dhjin
/// \date		2009-01-13 ~ 2009-01-13
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CDeclarationOfWar::CheckForbidTime(ATUM_DATE_TIME * i_pHopeTime)
{
	auto localTime = tm(*i_pHopeTime);

	ATUM_DATE_TIME CurrentTime;
	CurrentTime.SetCurrentDateTime();
	if((m_ForbidTime.DayOfWeek == localTime.tm_wday
		&& m_ForbidTime.ForbidStartTime.Hour <= i_pHopeTime->Hour
		&& m_ForbidTime.ForbidEndTime.Hour > i_pHopeTime->Hour)
		|| MSWAR_FORBID_GAP_HOUR >= i_pHopeTime->GetTimeDiffTimeInHours(CurrentTime)
		)
	{
		return TRUE;
	}

	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			INT	CDeclarationOfWar::MakeMSG_FC_INFO_MSWARINFO_RESULT_OK(SDECLARATION_OF_WAR * o_pDeclarationOfWarInfo, SDECLARATION_OF_WAR_FORBID_TIME * o_pForbidTime)
/// \brief		클라이언트 전송 할 선전 포고 정보 만들기
/// \author		dhjin
/// \date		2009-01-13 ~ 2009-01-13
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
INT	CDeclarationOfWar::MakeMSG_FC_INFO_DECLARATION_MSWAR_INFO_OK(SDECLARATION_OF_WAR * o_pDeclarationOfWarInfo, SDECLARATION_OF_WAR_FORBID_TIME * o_pForbidTime)
{
	mt_auto_lock mta(&m_mtvectSDECLARATION_OF_WAR);
	
	int	ResultListCount = 0;

	mtvectSDECLARATION_OF_WAR::iterator itr = m_mtvectSDECLARATION_OF_WAR.begin();
	for(; itr != m_mtvectSDECLARATION_OF_WAR.end(); itr++)
	{
		o_pDeclarationOfWarInfo[ResultListCount].Influence			= itr->Influence;
		o_pDeclarationOfWarInfo[ResultListCount].MSWarStep			= itr->MSWarStep;
		o_pDeclarationOfWarInfo[ResultListCount].NCP				= itr->NCP;
		o_pDeclarationOfWarInfo[ResultListCount].MSNum				= itr->MSNum;
		o_pDeclarationOfWarInfo[ResultListCount].MSAppearanceMap	= itr->MSAppearanceMap;
		o_pDeclarationOfWarInfo[ResultListCount].MSWarStepStartTime	= itr->MSWarStepStartTime;
		o_pDeclarationOfWarInfo[ResultListCount].MSWarStepEndTime	= itr->MSWarStepEndTime;
		o_pDeclarationOfWarInfo[ResultListCount].MSWarStartTime		= itr->MSWarStartTime;
		o_pDeclarationOfWarInfo[ResultListCount].MSWarEndTime		= itr->MSWarEndTime;
		o_pDeclarationOfWarInfo[ResultListCount].SelectCount		= itr->SelectCount;
		o_pDeclarationOfWarInfo[ResultListCount].GiveUp				= itr->GiveUp;
		o_pDeclarationOfWarInfo[ResultListCount].MSWarEndState		= itr->MSWarEndState;
		ResultListCount++;
	}
	
	o_pForbidTime->DayOfWeek		=	m_ForbidTime.DayOfWeek;
	o_pForbidTime->ForbidStartTime	=	m_ForbidTime.ForbidStartTime;
	o_pForbidTime->ForbidEndTime	=	m_ForbidTime.ForbidEndTime;

	return ResultListCount;	
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CDeclarationOfWar::GetOwnerInflOfConflictArea(BYTE i_byInfl)
/// \brief		현재 분쟁지역 소유 세력인지 체크
/// \author		dhjin
/// \date		2009-01-13 ~ 2009-01-13
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CDeclarationOfWar::GetOwnerInflOfConflictArea(BYTE i_byInfl)
{
	ATUM_DATE_TIME CurrentTime;
	CurrentTime.SetCurrentDateTime();

	mt_auto_lock mta(&m_mtvectSDECLARATION_OF_WAR);

	BOOL OwnerResult = FALSE; 

	mtvectSDECLARATION_OF_WAR::iterator itr = m_mtvectSDECLARATION_OF_WAR.begin();
	for(; itr != m_mtvectSDECLARATION_OF_WAR.end(); itr++)
	{
		if(i_byInfl == itr->Influence
			&& MSWAR_END_LOSS == itr->MSWarEndState)
		{
			OwnerResult = FALSE;
		}
		else if(i_byInfl == itr->Influence
				&& MSWAR_END_WIN == itr->MSWarEndState)
		{
			OwnerResult = TRUE;
		}
	}	

	return OwnerResult;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BYTE CDeclarationOfWar::GetLastMSWinStep(BYTE i_byInfl)
/// \brief		모선전 최종 승리 단계를 가져온다.
/// \author		dhjin
/// \date		2009-03-31 ~ 2009-03-31
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BYTE CDeclarationOfWar::GetLastMSWinStep(BYTE i_byInfl)
{
	mt_auto_lock mta(&m_mtvectSDECLARATION_OF_WAR);

	BOOL LastMSWinStep = 0; 

	mtvectSDECLARATION_OF_WAR::iterator itr = m_mtvectSDECLARATION_OF_WAR.begin();
	for(; itr != m_mtvectSDECLARATION_OF_WAR.end(); itr++)
	{
		if(i_byInfl == itr->Influence
			&& MSWAR_END_WIN == itr->MSWarEndState)
		{
			LastMSWinStep = itr->MSWarStep;
		}
	}	

	return LastMSWinStep;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CDeclarationOfWar::CheckSPSummonPossibleTime()
/// \brief		전략포인트 시간 설정 수정 - 모선 시간에 전략포인트가 소환 되어 있지 않게 체크
/// \author		dhjin
/// \date		2009-03-30 ~ 2009-03-30
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CDeclarationOfWar::CheckSPSummonPossibleTime()
{
	mt_auto_lock mta { m_mtvectSDECLARATION_OF_WAR };

	ATUM_DATE_TIME CheckTime { true };

	CheckTime.AddDateTime(0,0,0, 0, (STRATEGYPOINT_SUMMONTIME_BY_INFLUENCEBOSS/2 + 3),0);
	
	// todo : improve this hardcoded part

	if(m_pCurrentBCUWarStepInfo->MSWarStartTime <= CheckTime
		&& CheckTime.GetTimeDiffTimeInMinutes(m_pCurrentBCUWarStepInfo->MSWarStartTime) < 150 // 2017-01-02 panoskj
		&& FALSE == m_pCurrentBCUWarStepInfo->GiveUp
		&& !(MSWAR_END_WIN == m_pCurrentBCUWarStepInfo->MSWarEndState
			|| MSWAR_END_LOSS == m_pCurrentBCUWarStepInfo->MSWarEndState)
		)
	{
		return FALSE;
	}
	if(m_pCurrentANIWarStepInfo->MSWarStartTime <= CheckTime 
		&& CheckTime.GetTimeDiffTimeInMinutes(m_pCurrentANIWarStepInfo->MSWarStartTime) < 150 // 2017-01-02 panoskj
		&& FALSE == m_pCurrentANIWarStepInfo->GiveUp
		&& !(MSWAR_END_WIN == m_pCurrentANIWarStepInfo->MSWarEndState
			|| MSWAR_END_LOSS == m_pCurrentANIWarStepInfo->MSWarEndState)
		)
	{
		return FALSE;
	}
	
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
// 2012-11-13 by jhseol, 전쟁 시스템 리뉴얼 - 거점전
BOOL CDeclarationOfWar::CheckMSWarTime(USHORT i_Year, BYTE i_Month, BYTE i_Day)
{
	if( i_Year	== m_pCurrentBCUWarStepInfo->MSWarStartTime.Year &&
		i_Month	== m_pCurrentBCUWarStepInfo->MSWarStartTime.Month &&
		i_Day	== m_pCurrentBCUWarStepInfo->MSWarStartTime.Day)
	{
		return TRUE;
	}
	if( i_Year	== m_pCurrentANIWarStepInfo->MSWarStartTime.Year &&
		i_Month	== m_pCurrentANIWarStepInfo->MSWarStartTime.Month &&
		i_Day	== m_pCurrentANIWarStepInfo->MSWarStartTime.Day)
	{
		return TRUE;
	}
	return FALSE;
}
// end 2012-11-13 by jhseol, 전쟁 시스템 리뉴얼 - 거점전

///////////////////////////////////////////////////////////////////////////////
// 2013-02-26 by jhseol, 전쟁 시스템 리뉴얼 부가옵션 - 모선전 시작 시간 가져오기
ATUM_DATE_TIME CDeclarationOfWar::GetMSWarStartTime(BYTE i_nInfluenceType)
{
	if( INFLUENCE_TYPE_VCN == i_nInfluenceType )
	{
		return m_pCurrentBCUWarStepInfo->MSWarStartTime;
	}
	else if ( INFLUENCE_TYPE_ANI == i_nInfluenceType )
	{
		return m_pCurrentANIWarStepInfo->MSWarStartTime;
	}
	ATUM_DATE_TIME ErrorTime;
	ErrorTime.SetDateTime(1986,2,28,0,0,0);
	return ErrorTime;
}
// end 2013-02-26 by jhseol, 전쟁 시스템 리뉴얼 부가옵션 - 모선전 시작 시간 가져오기
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// 2006-04-13 by cmkwon


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CInflWarManager::CInflWarManager()
{
	InitializeCriticalSection(&m_criticallStrategyPointSummonRange);
}

CInflWarManager::~CInflWarManager()
{
	DeleteCriticalSection(&m_criticallStrategyPointSummonRange);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CInflWarManager::InitCInflWarManager(CFieldIOCP *i_pFIOCP)
/// \brief		
/// \author		cmkwon
/// \date		2006-04-13 ~ 2006-04-13
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CInflWarManager::InitCInflWarManager(CFieldIOCP *i_pFIOCP)
{
	m_pFieldIOCP14		= i_pFIOCP;

	m_mtvectSummonMonsterDataList.clear();

	m_mtvectSummonStrategyPointInfo.clear();			// 2007-02-23 by dhjin,
	m_mtvectSummonStrategyPointInfo.reserve(12);		// 2007-02-23 by dhjin,	거점 수 현재 10개 

	util::zero(&m_SummonVCNBossMonsterInfo, sizeof(SSUMMONBOSSMONSTER_INFO));
	util::zero(&m_SummonANIBossMonsterInfo, sizeof(SSUMMONBOSSMONSTER_INFO));

	this->InitStrategyPoint();

	m_VCNLeaderGuildUID	= 0;
	m_ANILeaderGuildUID = 0;

	m_VCNTeleportMapIndex	= 0;
	m_ANITeleportMapIndex	= 0;

	m_VCNSubLeader1GuildUID = 0;
	m_VCNSubLeader2GuildUID = 0;
	m_ANISubLeader1GuildUID = 0;
	m_ANISubLeader2GuildUID = 0;

	util::zero(&m_SPOLLDATE_DB, sizeof(SPOLLDATE_DB));
	this->LoadVoterListByDB();
	this->LoadLeaderCandidateByDB();

	this->ResetMSWarInfoResult();		// 2008-04-01 by dhjin, 모선전, 거점전 정보창 기획안 - 모선전 결과 정보
	this->ResetSPWarInfoResult();       // 2008-04-01 by dhjin, 모선전, 거점전 정보창 기획안 - 거점전 결과 정보
	
	//////////////////////////////////////////////////////////////////////////
	// 2009-01-12 by dhjin, 선전 포고
	m_DeclarationOfWar.InitDeclarationOfWar();		

	m_bIsDoingInfluenceWarCheck = FALSE;	// 2012-11-26 by jhseol, 전쟁 시스템 리뉴얼 - 전쟁시 트리거 무적상태(일시정지), TRUE = 전쟁 중이다, FALSE = 전쟁 중이 아니다.
#ifdef NEMERIAN_NATION_BALANCE_BUFF_PENALITY
	setDiffVictory();
#endif
}

void CInflWarManager::OnMinutelySPManager(ATUM_DATE_TIME * pDateTime)
{
	ATUM_DATE_TIME now { true };

	auto localStTime = tm(now);

	// calculate SP spawn times if it's a new day
	if (now > m_atBeforeCheckTime && now.Day != m_atBeforeCheckTime.Day)	// 2013-01-14 by jhseol, 전쟁 시스템 리뉴얼 - 거점전, 날짜 체크 조건문 수정
	{
		// 하루가 지났다면 거점 정보를 다시 셋팅한다.
		server::log(true, "  It's a new day, resetting SP times, last check time(%s), current time(%s)\r\n",
			m_atBeforeCheckTime.GetDateTimeString().GetBuffer(),
			now.GetDateTimeString().GetBuffer());
		
		m_atBeforeCheckTime.SetCurrentDateTime();

		////////////////////////////////////////////////////////////////////////
		// 오늘 요일에 맞는 거점 시간을 가져온다
		mt_auto_lock mta { m_mtvectRenewalStrategyPointSummonTime };

		for (auto& spinfo : m_mtvectRenewalStrategyPointSummonTime)
		{
			if (SHORT(localStTime.tm_wday) == spinfo.DayOfWeek)		// 오늘과 같은 요일의 정보를 기준으로 
			{
				auto StartTime { now };
				auto EndTime { now };

				StartTime.Hour = spinfo.StartTime.Hour;
				StartTime.Minute = spinfo.StartTime.Minute;
				StartTime.Second = spinfo.StartTime.Second;
				EndTime.Hour = spinfo.EndTime.Hour;
				EndTime.Minute = spinfo.EndTime.Minute;
				EndTime.Second = spinfo.EndTime.Second;

				auto	tmTodayBCUSummonCount = spinfo.CountBCU;	// 다음의 락 해제를 위해 임시변수에 값 저장
				auto	tmTodayANISummonCount = spinfo.CountANI;	// 다음의 락 해제를 위해 임시변수에 값 저장
				mta.auto_unlock_cancel();	// 조건문에 걸리고 시간값을 가져왔다면 더이상 필요 없으니 락 해제. 후에 break로 반복문 탈출 코드 있음

											////////////////////////////////////////////////////////////////////////
											// 거점 생성 가능 시간대의 범위
				auto TotalSummonTime = EndTime.GetTimeDiffTimeInSeconds(StartTime);
				auto MaxSummonTimeGap = TotalSummonTime;

				////////////////////////////////////////////////////////////////////////
				// 거점 생성 가능 시간대를 생성할 거점의 수로 나눠 1게의 거점당 생성해야할 평균 시간을 계산한다
				auto SettingSummonStrategyPointCount = tmTodayBCUSummonCount + tmTodayANISummonCount;
				if (0 < SettingSummonStrategyPointCount)
				{
					MaxSummonTimeGap = TotalSummonTime / SettingSummonStrategyPointCount;
				}

				////////////////////////////////////////////////////////////////////////
				// 평균 시간에서 거점 최소 텀 30분을 빼주고 남은 가용 시간범위를 계산한다. 여기서 뺀 30분은 후에 다시 더해준다.
				MaxSummonTimeGap -= RenewalStrategyPointSummonTimeTermMin;		// 2013-03-18 by jhseol, 거점 생성시간 수정 - 거점 생성을 시작시간 정각 부터 하기 위해 최소텀을 빼준다
				if (0 > MaxSummonTimeGap)
				{
					MaxSummonTimeGap = 0;
				}

				////////////////////////////////////////////////////////////////////////
				// 셋팅할 거점 정보 로그 기록
				server::log(TRUE, "  [Notify] S_WAR_SYSTEM_RENEWAL_STRATEGYPOINT_JHSEOL #Reset Step.2 - Today Strategy Point Info : Start Time(%s), End Time(%s), BCUCount(%d), ANICount(%d), MinSummonTimeGap(%d), MaxSummonTimeGap(%d)\r\n"
					, StartTime.GetDateTimeString().GetBuffer(), EndTime.GetDateTimeString().GetBuffer()
					, tmTodayBCUSummonCount, tmTodayANISummonCount, RenewalStrategyPointSummonTimeTermMin, MaxSummonTimeGap + RenewalStrategyPointSummonTimeTermMin);

				vector<ATUM_DATE_TIME> tmRandomSummonTime;
				tmRandomSummonTime.clear();

				////////////////////////////////////////////////////////////////////////
				// 생성할 거점의 수만큼 셋팅을 한다.
				for (int forloopindex = 0; forloopindex < SettingSummonStrategyPointCount; forloopindex++)
				{
					int RandomSummonTimeGap = RANDI(0, MaxSummonTimeGap);
					StartTime.AddDateTime(0, 0, 0, 0, 0, RandomSummonTimeGap);
					tmRandomSummonTime.push_back(StartTime);

					if (S_WAR_SYSTEM_RENEWAL_STRATEGYPOINT_SUMMON_TIME_VARIANCE_JHSEOL)

						StartTime.AddDateTime(0, 0, 0, 0, 0, RenewalStrategyPointSummonTimeTermMin + MaxSummonTimeGap - RandomSummonTimeGap);

					else StartTime.AddDateTime(0, 0, 0, 0, 0, RenewalStrategyPointSummonTimeTermMin);
				}

				server::log(TRUE, "  [Notify] S_WAR_SYSTEM_RENEWAL_STRATEGYPOINT_JHSEOL #Reset Step.3 - Summon Time Setting Success\r\n");

				////////////////////////////////////////////////////////////////////////
				// mtvect 속성의 정보를 일반 벡터로 카피
				vectSSTRATEGYPOINT_MAP_INFLUENCESTRATEGYPOINT_INFO tmVectSTRATEGYPOINT_MAP_INFLUENCESTRATEGYPOINT_INFO;		// 거점전 맵 정보를 저장할 임시 벡터
				tmVectSTRATEGYPOINT_MAP_INFLUENCESTRATEGYPOINT_INFO.clear();
				mt_auto_lock mtMap(&m_mtvectStrategyPointMapInfluenceInfo);
				mtvectSSTRATEGYPOINT_MAP_INFLUENCESTRATEGYPOINT_INFO::iterator copy_itr = m_mtvectStrategyPointMapInfluenceInfo.begin();
				for (; copy_itr < m_mtvectStrategyPointMapInfluenceInfo.end(); copy_itr++)
				{
					tmVectSTRATEGYPOINT_MAP_INFLUENCESTRATEGYPOINT_INFO.push_back(*copy_itr);
				}
				mtMap.auto_unlock_cancel();		// 복사가 완료 되어 더이상 필요 없으니 락 헤제

				server::log(TRUE, "  [Notify] S_WAR_SYSTEM_RENEWAL_STRATEGYPOINT_JHSEOL #Reset Step.4 - Map Influence Copy Success : Map Count(%d)\r\n", tmVectSTRATEGYPOINT_MAP_INFLUENCESTRATEGYPOINT_INFO.size());

				////////////////////////////////////////////////////////////////////////
				// 기존 정보를 지우고 거점전 맵의 수만큼 리스트를 추가한다.
				mt_auto_lock mtb(&m_mtvectStrategyPointSummonTimeInfo);
				m_mtvectStrategyPointSummonTimeInfo.clear();
				SSTRATEGYPOINT_SUMMONTIME_INFO tmSSTRATEGYPOINT_SUMMONTIME_INFO;
				util::zero(&tmSSTRATEGYPOINT_SUMMONTIME_INFO, sizeof(SSTRATEGYPOINT_SUMMONTIME_INFO));
				vectSSTRATEGYPOINT_MAP_INFLUENCESTRATEGYPOINT_INFO::iterator tmmap_itr = tmVectSTRATEGYPOINT_MAP_INFLUENCESTRATEGYPOINT_INFO.begin();
				for (; tmmap_itr < tmVectSTRATEGYPOINT_MAP_INFLUENCESTRATEGYPOINT_INFO.end(); tmmap_itr++)
				{
					tmSSTRATEGYPOINT_SUMMONTIME_INFO.MapIndex = tmmap_itr->MapIndex;
					m_mtvectStrategyPointSummonTimeInfo.push_back(tmSSTRATEGYPOINT_SUMMONTIME_INFO);
				}

				server::log(TRUE, "  [Notify] S_WAR_SYSTEM_RENEWAL_STRATEGYPOINT_JHSEOL #Reset Step.5 - Strategy Point Summon Time Info Init(Delete & New Insert) Success\r\n");

				////////////////////////////////////////////////////////////////////////
				// 거점리스트들을 정렬 기준에 상관없이 임의로 섞는다.
				std::random_shuffle(m_mtvectStrategyPointSummonTimeInfo.begin(), m_mtvectStrategyPointSummonTimeInfo.end());
				mtvectSSTRATEGYPOINT_SUMMONTIME_INFO::iterator shuffle_itr = m_mtvectStrategyPointSummonTimeInfo.begin();
				int tmRandomSummonTimeIndex = 0;		// 임시 저장된 거점 설정 백터의 인덱스
				int tmBCUSummonCount = 0;				// 셋팅 완료된 BCU 세력의 거점 수
				int tmANISummonCount = 0;				// 셋팅 완료된 ANI 세력의 거점 수
				for (; shuffle_itr < m_mtvectStrategyPointSummonTimeInfo.end(); shuffle_itr++)
				{
					util::zero(&shuffle_itr->SummonTime, sizeof(ATUM_DATE_TIME));
					shuffle_itr->SummonAttribute = FALSE;
					shuffle_itr->SummonCount = 0;
					////////////////////////////////////////////////////////////////////////
					// 거점의 맵번호로 세력을 비교해서 각 새력에 셋팅된 거점의 수만큼만 셋팅이 되도록 한다.
					vectSSTRATEGYPOINT_MAP_INFLUENCESTRATEGYPOINT_INFO::iterator map_itr = tmVectSTRATEGYPOINT_MAP_INFLUENCESTRATEGYPOINT_INFO.begin();
					for (; map_itr < tmVectSTRATEGYPOINT_MAP_INFLUENCESTRATEGYPOINT_INFO.end(); map_itr++)
					{
						if (shuffle_itr->MapIndex == map_itr->MapIndex)
						{
							if (IS_MAP_INFLUENCE_VCN(map_itr->Influence))
							{
								////////////////////////////////////////////////////////////////////////
								// BCU세력의 셋팅 완료 된 거점 수와 셋팅할 거점의 수를 비교한다.
								if (tmBCUSummonCount < tmTodayBCUSummonCount)
								{
									////////////////////////////////////////////////////////////////////////
									// BCU세력에 아직 셋팅되지 않은 거점이 있음을 의미함으로 셋팅하고 셋팅 완료된 BCU 세력 거점수 증가
									shuffle_itr->SummonAttribute = TRUE;
									shuffle_itr->SummonCount = 1;
									tmBCUSummonCount++;
								}
							}
							else
							{
								////////////////////////////////////////////////////////////////////////
								// ANI세력의 셋팅 완료 된 거점 수와 셋팅할 거점의 수를 비교한다.
								if (tmANISummonCount < tmTodayANISummonCount)
								{
									////////////////////////////////////////////////////////////////////////
									// ANI세력에 아직 셋팅되지 않은 거점이 있음을 의미함으로 셋팅하고 셋팅 완료된 ANI 세력 거점수 증가
									shuffle_itr->SummonAttribute = TRUE;
									shuffle_itr->SummonCount = 1;
									tmANISummonCount++;
								}
							}
						}
					}
					if (TRUE == shuffle_itr->SummonAttribute)
					{
						////////////////////////////////////////////////////////////////////////
						// 셋팅 완료 된 거점의 총 수와 임시로 저장한 거점의 생성 시간의 크기를 비교한다.
						if (tmRandomSummonTimeIndex < tmRandomSummonTime.size())
						{
							////////////////////////////////////////////////////////////////////////
							// 아직 셋팅되지 않은 거점이 있음을 의미한다.
							shuffle_itr->SummonTime = tmRandomSummonTime[tmRandomSummonTimeIndex];
							tmRandomSummonTimeIndex++;
						}
					}
					if (NULL != m_pFieldIOCP14)
					{
						QPARAM_STRATEGYPOINT_SUMMON_INFO *pQparam = new QPARAM_STRATEGYPOINT_SUMMON_INFO;
						pQparam->MapIndex = shuffle_itr->MapIndex;
						pQparam->SummonCount = shuffle_itr->SummonCount;
						pQparam->SummonTime = shuffle_itr->SummonTime;
						pQparam->SummonAttribute = shuffle_itr->SummonAttribute;
						m_pFieldIOCP14->m_pAtumDBManager->MakeAndEnqueueQuery(QT_UpdateStrategyPointSummonInfo, NULL, 0, pQparam);
						server::log(TRUE, "  [Notify] S_WAR_SYSTEM_RENEWAL_STRATEGYPOINT_JHSEOL #Reset Step.Finish - Strategy Point Setting Done : MapIndex(%d), SummonCount(%d), SummonTime(%s), SummonAttribute(%d)\r\n"
							, shuffle_itr->MapIndex, shuffle_itr->SummonCount, shuffle_itr->SummonTime.GetDateTimeString().GetBuffer(), shuffle_itr->SummonAttribute);
					}
					else
					{
						server::log(TRUE, "  [ERROR!] S_WAR_SYSTEM_RENEWAL_STRATEGYPOINT_JHSEOL #Reset Step.Finish - m_pFieldIOCP14 is NULL : MapIndex(%d), SummonCount(%d), SummonTime(%s), SummonAttribute(%d)\r\n"
							, shuffle_itr->MapIndex, shuffle_itr->SummonCount, shuffle_itr->SummonTime.GetDateTimeString().GetBuffer(), shuffle_itr->SummonAttribute);
					}
				}
				break;
			}
		}
	}

	// this conditional is eliminated at compile time
	auto condition = S_WAR_SYSTEM_RENEWAL_STRATEGYPOINT_SUB_OPTION_JHSEOL ?
		m_DeclarationOfWar.CheckSPSummonPossibleTime() :
		!m_DeclarationOfWar.CheckMSWarTime(now.Year, now.Month, now.Day);

	if (condition)
	{
		// 2017-01-03 too much logging, panoskj
		// this conditional is eliminated at compile time
		//if (S_WAR_SYSTEM_RENEWAL_STRATEGYPOINT_SUB_OPTION_JHSEOL)
		//	server::log(true, "  [Notify] S_WAR_SYSTEM_RENEWAL_STRATEGYPOINT_SUB_OPTION_JHSEOL #Summon Step.1 - Summon Process Start\r\n");
		//else server::log(true, "  [Notify] S_WAR_SYSTEM_RENEWAL_STRATEGYPOINT_JHSEOL #Summon Step.1 - Summon Process Start\r\n");

		mt_auto_lock mta { m_mtvectRenewalStrategyPointSummonTime };

		for (auto& spnewinfo : m_mtvectRenewalStrategyPointSummonTime)
		{
			if (SHORT(localStTime.tm_wday) == spnewinfo.DayOfWeek)		// 오늘과 같은 요일의 정보를 기준으로 
			{
				auto tmStartTime { now };

				tmStartTime.Hour = spnewinfo.StartTime.Hour;
				tmStartTime.Minute = spnewinfo.StartTime.Minute;
				tmStartTime.Second = spnewinfo.StartTime.Second;

				auto tmEndTime { now };

				tmEndTime.Hour = spnewinfo.EndTime.Hour;
				tmEndTime.Minute = spnewinfo.EndTime.Minute;
				tmEndTime.Second = spnewinfo.EndTime.Second;

				mta.auto_unlock_cancel();		// 조건문에 걸리고 시간값을 가져왔다면 더이상 필요 없으니 락 해제. 후에 break로 반복문 탈출 코드 있음

				if (now >= tmStartTime && now < tmEndTime)
				{
					server::log("  [Notify] S_WAR_SYSTEM_RENEWAL_STRATEGYPOINT_JHSEOL #Summon Step.2 - CurrentTime in Today War Time : StartTime(%s), EndTime(%s)\r\n"
						, tmStartTime.GetDateTimeString().GetBuffer(), tmEndTime.GetDateTimeString().GetBuffer());

					mt_auto_lock mtb { m_mtvectStrategyPointSummonTimeInfo };
					
					for (auto& spinfo : m_mtvectStrategyPointSummonTimeInfo)
					{
						if (!spinfo.SummonAttribute || 0 >= spinfo.SummonCount)		// 소환하지 않는 거점 skip

							continue;
						
						if (now < spinfo.SummonTime)								// 아직 소환이 안됐다면 skip
						{
							server::log("  [Notify] S_WAR_SYSTEM_RENEWAL_STRATEGYPOINT_JHSEOL #Summon Step.3 - Still did not have time to summon. This Summon Strategy Point Pass : MapIndex(%d), SummonTime(%s)\r\n",
								spinfo.MapIndex, spinfo.SummonTime.GetDateTimeString().GetBuffer());
							
							continue;
						}

						if (300 > now.GetTimeDiffTimeInSeconds(spinfo.SummonTime))	// 현제 시간과 소환 해야할 시간의 차이가 5분 이내면 소환을 한다.
						{
							if (NULL == m_pFieldIOCP14)
							{
								server::log("  [ERROR!] S_WAR_SYSTEM_RENEWAL_STRATEGYPOINT_JHSEOL #Summon Step.3 - m_pFieldIOCP14 is NULL, Summon Info : MapIndex(%d), SummonTime(%s)\r\n"
									, spinfo.MapIndex, spinfo.SummonTime.GetDateTimeString().GetBuffer());
								
								continue;
							}
							server::log("  [Notify] S_WAR_SYSTEM_RENEWAL_STRATEGYPOINT_JHSEOL #Summon Step.3 - Summon Strategy Point : MapIndex(%d), SummonTime(%s)\r\n"
								, spinfo.MapIndex, spinfo.SummonTime.GetDateTimeString().GetBuffer());
							spinfo.SummonAttribute = FALSE;
							spinfo.SummonCount = 0;
							auto tmMapIndex = spinfo.MapIndex;			// 다음의 락 해제를 위해 임시변수에 값 저장
							mtb.auto_unlock_cancel();						// 조건문에 걸리고 필요 정보를 가져오면 더이상 필요 없으니 락 해제. 후에 break로 반복문 탈출 코드 있음

							auto pQparam = new QPARAM_UPDATE_STRATEGYPOINTSUMMONINFO_COUNT;
							pQparam->MapIndex = tmMapIndex;
							pQparam->SummonCount = 0;
							m_pFieldIOCP14->m_pAtumDBManager->MakeAndEnqueueQuery(QT_UpdateStrategyPointSummonInfoBySummon, NULL, 0, pQparam);

							// 이미 소환중인 거점맵인지 체크할 변수
							auto bSummonCheck = false;	
							
							mt_auto_lock mtc { m_mtvectSummonStrategyPointInfo };
							
							// 거점이 소환된 맵이 있다면
							for (auto& inf : m_mtvectSummonStrategyPointInfo)
								// 지금 소환할 거점맵인지 확인
								if (inf.MapIndex == tmMapIndex)			
									// 맞으면 이미 거점이 소환중이다.
									bSummonCheck = true;				
							
							// 거점 조회가 끝났음로 락 헤제.
							mtc.auto_unlock_cancel();						

							if (!m_pFieldIOCP14->m_OutPostManager.CheckALLOutPostWaring() && !bSummonCheck)	// 전진기지전이 아니고 거점 생성이 가능하다면 거점소환
							{
								auto pFMapChann = m_pFieldIOCP14->GetFieldMapChannelByMapChannelIndex(MAP_CHANNEL_INDEX(tmMapIndex, 0));
								
								if (pFMapChann)
								{
									INIT_MSG_WITH_BUFFER(MSG_FN_MONSTER_STRATEGYPOINT_SUMMON, T_FN_MONSTER_STRATEGYPOINT_SUMMON, pSummon, SendBuf);
									pSummon->MapIndex = tmMapIndex;
									pFMapChann->Send2NPCServerW(SendBuf, MSG_SIZE(MSG_FN_MONSTER_STRATEGYPOINT_SUMMON));
									server::log("  [Notify] S_WAR_SYSTEM_RENEWAL_STRATEGYPOINT_JHSEOL #Summon Step.4 - Strategy Point Summon Done\r\n");
								}
								else
								{
									server::log("  [ERROR!] S_WAR_SYSTEM_RENEWAL_STRATEGYPOINT_JHSEOL #Summon Step.4 - pFMapChann is NULL, MapIndex(%d)\r\n", tmMapIndex);
								}
							}
							else
							{
								server::log("  [Notify] S_WAR_SYSTEM_RENEWAL_STRATEGYPOINT_JHSEOL #Summon Step.4 - OutPost Start OR This map Strategy Point Summoning : OutPostState(%d), SummonCheck(%d)\r\n"
									, m_pFieldIOCP14->m_OutPostManager.CheckALLOutPostWaring(), bSummonCheck);
							}
							break;
						}
					}
					server::log("  [Notify] S_WAR_SYSTEM_RENEWAL_STRATEGYPOINT_JHSEOL #Summon Step.Finish - Do not have to be summoned anymore.\r\n");
				}

				else
				{
					server::log("  [Notify] Spawning SPs is disabled at this time, summon time (%s ~ %s)\r\n",
						tmStartTime.GetDateTimeString().GetBuffer(),
						tmEndTime.GetDateTimeString().GetBuffer());
				}

				break;
			}
		}
	}
	else
	{
		if (S_WAR_SYSTEM_RENEWAL_STRATEGYPOINT_SUB_OPTION_JHSEOL)
		{
			auto cBCUMswarStartTime = m_DeclarationOfWar.GetMSWarStartTime(INFLUENCE_TYPE_VCN).GetDateTimeString().GetBuffer();
			auto cANIMswarStartTime = m_DeclarationOfWar.GetMSWarStartTime(INFLUENCE_TYPE_ANI).GetDateTimeString().GetBuffer();

			server::log("  [Notify] Cannot spawn SPs, a MSWAR is underway, BCU(%s), ANI(%s)\r\n", cBCUMswarStartTime, cANIMswarStartTime);
		}

		else server::log("  [Notify] Cannot spawn SPs, a MSWAR is today\r\n");
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CInflWarManager::OnDoMinutelyWorkInflWarManager(ATUM_DATE_TIME *pDateTime)
/// \brief		
/// \author		cmkwon
/// \date		2006-04-14 ~ 2006-04-14
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CInflWarManager::OnDoMinutelyWorkInflWarManager(ATUM_DATE_TIME *pDateTime)
{
	// 2009-03-05 by cmkwon, 선전포고 아레나 서버 처리 - 아레나 서버는 세력전 관련 처리 필요 없음
	if (g_pFieldGlobal->IsArenaServer()) return;

	if (m_DeclarationOfWar.CheckNextMSWarStep(pDateTime)) SetCurrentStepInflWarDataW();
	
	// BCU
	if (m_DeclarationOfWar.CheckBCUMSWarBeforeStartTime(pDateTime)) MSWarBeforeStart(INFLUENCE_TYPE_VCN);
	
	// BCU 모선전 시작
	else if (m_DeclarationOfWar.CheckBCUMSWarStartTime(pDateTime)) MSWarStart(INFLUENCE_TYPE_VCN);
	
	// ANI
	if (m_DeclarationOfWar.CheckANIMSWarBeforeStartTime(pDateTime)) MSWarBeforeStart(INFLUENCE_TYPE_ANI);
	
	// ANI 모선전 시작
	else if (m_DeclarationOfWar.CheckANIMSWarStartTime(pDateTime)) MSWarStart(INFLUENCE_TYPE_ANI);
	

	// 2007-11-16 by dhjin, 지도자 선출 시간 체크하여 지도자 득표 결과 전송
	// 2008-09-10 by cmkwon, 통합아레나 서버에서는 체크 필요 없음
	if (CheckLeaderelectionDate(pDateTime))	SendLeaderelectionInfo();
		
	///////////////////////////////////////////////////////////////////////////////
	// 2012-11-13 by jhseol, 전쟁 시스템 리뉴얼 - 거점전
	if (S_WAR_SYSTEM_RENEWAL_STRATEGYPOINT_JHSEOL) OnMinutelySPManager(pDateTime);

	else
	{
		//////////////////////////////////////////////////////////////////////////
		// 2007-03-02 by dhjin, 전략포인트 랜덤 생성 주기 설정.
		mt_auto_lock mta(this->GetStrategyPointSummonTimeInfo());
		if(TRUE == this->CheckStrategyPointSummon())
		{
			if(TRUE == this->CompareStrategyPointSummonTime())
			{// 2007-03-02 by dhjin, 주기가 끝났다면 새로 설정한다.
				mta.auto_unlock_cancel();
				this->LoadStrategyPointSummonInfo();			
			}
			else
			{
				mtvectSSTRATEGYPOINT_SUMMONTIME_INFO::iterator itr = m_mtvectStrategyPointSummonTimeInfo.begin();
				while(itr != m_mtvectStrategyPointSummonTimeInfo.end())
				{	
					if(0 >= itr->SummonCount)
					{
						itr++;
						continue;
					}

					if(0 < itr->SummonTime.GetTimeDiffToCurrentTimeInSeconds())
					{// 2007-03-02 by dhjin, 소환 시간이 지났다면 소환 한다.
						// 2009-03-23 by dhjin, 전략포인트 시간 설정 수정 - 밑과 같이 모선 시간을 체크하여 만약 모선 시간에 발생한다면 다음 주기로 넘긴다.
						// 밑과 같이 수정한다.
						if(!m_DeclarationOfWar.CheckSPSummonPossibleTime()
							|| FALSE == m_pFieldIOCP14->m_OutPostManager.CheckOutPostWarTimeStrategyPoint(&(itr->SummonTime))
							|| CheckSPSummonMapIndex(itr->MapIndex))
						{
							// 2009-03-23 by dhjin, 전략포인트 시간 설정 수정 - m_mtvectALLSPSummonTimeInfo에서 소환된 정보 삭제
							this->DeleteALLSPSummonTimeInfoByMapIndex(itr->MapIndex);

							if (1 < itr->SummonCount)
							{// 다음 소환 값이 있다면 설정한다.	
								this->GetNextSPSummonTimeByALLSPSummonTimeInfo(itr->MapIndex, &itr->SummonTime, &itr->SummonAttribute);
								QPARAM_UPDATE_STRATEGYPOINTSUMMONINFO_TIME *pQparam = new QPARAM_UPDATE_STRATEGYPOINTSUMMONINFO_TIME;
								pQparam->MapIndex		= itr->MapIndex;
								pQparam->SummonTime		= itr->SummonTime;
								pQparam->SummonAttribute	= itr->SummonAttribute;		// 2008-04-08 by dhjin, 소환 가능 시간 설정 - 
								this->m_pFieldIOCP14->m_pAtumDBManager->MakeAndEnqueueQuery(QT_UpdateStrategyPointSummonInfoBySummonTime, NULL, 0, pQparam);
								char szSysLog[1024];
								wsprintf(szSysLog, "  [Notify]: Strategypoint SummonTime Update By MS or Outpost or SP, MapIndex(%d), SummonTime(%s)\r\n"
									, itr->MapIndex, itr->SummonTime.GetDateTimeString().GetBuffer());
								g_pFieldGlobal->WriteSystemLog(szSysLog);
							}
							else
							{// 다음 소환 값이 없다면 다음 주기로 넘긴다.
								itr->SummonTime.AddDateTime(1,0,0, 0,0,0);
								QPARAM_UPDATE_STRATEGYPOINTSUMMONINFO_TIME *pQparam = new QPARAM_UPDATE_STRATEGYPOINTSUMMONINFO_TIME;
								pQparam->MapIndex		= itr->MapIndex;
								pQparam->SummonTime		= itr->SummonTime;
								pQparam->SummonAttribute	= itr->SummonAttribute;		// 2008-04-08 by dhjin, 소환 가능 시간 설정 - 
								this->m_pFieldIOCP14->m_pAtumDBManager->MakeAndEnqueueQuery(QT_UpdateStrategyPointSummonInfoBySummonTime, NULL, 0, pQparam);
								char szSysLog[1024];
								wsprintf(szSysLog, "  [Notify]: Startegypoint SummonTime Update Next Period, MapIndex(%d)\r\n"
									, itr->MapIndex);
								g_pFieldGlobal->WriteSystemLog(szSysLog);
							}
						}
	//					if (TRUE == m_SummonVCNBossMonsterInfo.bSummonBossMonster
	//						|| TRUE == m_SummonANIBossMonsterInfo.bSummonBossMonster)
	//					{// 2007-03-07 by dhjin, 전함이 소환 중이라면 전략포인트 소환 시간을 다시 설정한다.
	//						this->SetStrategyPointSummonTime(itr, STRATEGYPOINT_SUMMON_GAP_BY_INFLUENCEWAR);
	//					}
	//					else if(FALSE == m_pFieldIOCP14->m_OutPostManager.CheckOutPostWarTimeStrategyPoint(&(itr->SummonTime)))
	//					{// 2007-10-18 by cmkwon, else if로 수정// 2007-09-03 by dhjin, 전진기지 전쟁이 발생하는 시간이면 소환 시간을 다시 설정한다.
	//						this->SetStrategyPointSummonTime(itr, STRATEGYPOINT_SUMMON_GAP_BY_OUTPOST);
	//					}
						else
						{
							if ( 0 < itr->SummonCount)
							{
								CFieldMapChannel *pFMapChann = m_pFieldIOCP14->GetFieldMapChannelByMapChannelIndex(MAP_CHANNEL_INDEX(itr->MapIndex, 0));
								if(pFMapChann)
								{
									INIT_MSG_WITH_BUFFER(MSG_FN_MONSTER_STRATEGYPOINT_SUMMON, T_FN_MONSTER_STRATEGYPOINT_SUMMON, pSummon, SendBuf);
									pSummon->MapIndex = itr->MapIndex;
									pFMapChann->Send2NPCServerW(SendBuf, MSG_SIZE(MSG_FN_MONSTER_STRATEGYPOINT_SUMMON));
								}
								
								QPARAM_UPDATE_STRATEGYPOINTSUMMONINFO_COUNT *pQparam = new QPARAM_UPDATE_STRATEGYPOINTSUMMONINFO_COUNT;
								pQparam->MapIndex		= itr->MapIndex;
								itr->SummonCount		-= 1;
								pQparam->SummonCount	= itr->SummonCount;
								this->m_pFieldIOCP14->m_pAtumDBManager->MakeAndEnqueueQuery(QT_UpdateStrategyPointSummonInfoBySummon, NULL, 0, pQparam);
								
								// 2009-03-23 by dhjin, 전략포인트 시간 설정 수정 - m_mtvectALLSPSummonTimeInfo에서 소환된 정보 삭제
								this->DeleteALLSPSummonTimeInfoByMapIndex(itr->MapIndex);

								if (0 < itr->SummonCount)
								{
	// 2009-03-23 by dhjin, 전략포인트 시간 설정 수정 - m_mtvectALLSPSummonTimeInfo에서 정보를 가져와서 소환 시간을 설정한다.
	// 2007-03-05 by dhjin, 소환 후 소환 되어야 하는 정보가 있다면 소환 시간을 다시 설정한다.
	//								this->SetStrategyPointSummonTime(itr, STRATEGYPOINT_SUMMON_GAP_BY_INFLUENCEWAR);
									this->GetNextSPSummonTimeByALLSPSummonTimeInfo(itr->MapIndex, &itr->SummonTime, &itr->SummonAttribute);
									QPARAM_UPDATE_STRATEGYPOINTSUMMONINFO_TIME *pQparam = new QPARAM_UPDATE_STRATEGYPOINTSUMMONINFO_TIME;
									pQparam->MapIndex		= itr->MapIndex;
									pQparam->SummonTime		= itr->SummonTime;
									pQparam->SummonAttribute	= itr->SummonAttribute;		// 2008-04-08 by dhjin, 소환 가능 시간 설정 - 
									this->m_pFieldIOCP14->m_pAtumDBManager->MakeAndEnqueueQuery(QT_UpdateStrategyPointSummonInfoBySummonTime, NULL, 0, pQparam);		
									char szSysLog[1024];
									wsprintf(szSysLog, "  [Notify]: Strategypoint SummonTime Update, MapIndex(%d), SummonTime(%s)\r\n"
									, itr->MapIndex, itr->SummonTime.GetDateTimeString().GetBuffer());
									g_pFieldGlobal->WriteSystemLog(szSysLog);
								}
							}
						}

					}
					itr++;
				}
				mta.auto_unlock_cancel();
			}
		}
	}
	// end 2012-11-13 by jhseol, 전쟁 시스템 리뉴얼 - 거점전

	// 2007-09-16 by dhjin, 텔레포트 처리
	TelePortInfoBuingByTick(pDateTime);
	
	if (!m_mtvectSummonMonsterDataList.empty())
	{
		ATUM_DATE_TIME now { true };

		mt_auto_lock mtA { m_mtvectSummonMonsterDataList };

		auto itr = m_mtvectSummonMonsterDataList.begin();

		while (itr != m_mtvectSummonMonsterDataList.end())
		{
			auto pSummonData = &*itr;

			auto nRemainMinute = pSummonData->SummonTerm - now.GetTimeDiffTimeInMinutes(pSummonData->atimeInsertedTime);

			if (nRemainMinute <= 0)
			{
				///////////////////////////////////////////////////////////////////////////////
				// 2006-04-14 by cmkwon, 몬스터 소환 처리
				auto pFMapChann = m_pFieldIOCP14->GetFieldMapChannelByMapChannelIndex(MAP_CHANNEL_INDEX(pSummonData->SummonMapIndex, 0));

				if (pFMapChann)
				{
					INIT_MSG_WITH_BUFFER(MSG_FN_ADMIN_SUMMON_MONSTER, T_FN_ADMIN_SUMMON_MONSTER, pSSummon, SendBuf);
					pSSummon->ChannelIndex = pFMapChann->GetMapChannelIndex().ChannelIndex;
					util::strncpy(pSSummon->CharacterName, "InfluenceWar", SIZE_MAX_CHARACTER_NAME);
					pSSummon->MonsterUnitKind = pSummonData->pMonsterInfo->MonsterUnitKind;
					pSSummon->NumOfMonster = pSummonData->SummonCount;
					pSSummon->Position = pSummonData->SummonPosition;

					// 2010. 07. 05 by hsLee. 몬스터 소환 관련. (밸런스 데이터 누락 수정.)
					pSSummon->MonsterBalanceData.Init(0);

					pFMapChann->Send2NPCServerW(SendBuf, MSG_SIZE(MSG_FN_ADMIN_SUMMON_MONSTER));
				}

				///////////////////////////////////////////////////////////////////////////////
				// 2006-04-14 by cmkwon, 세력전 보스 몬스터 소환 정보 클라이어트로 전송
				INIT_MSG_WITH_BUFFER(MSG_FC_WAR_BOSS_MONSTER_SUMMON_DATA, T_FC_WAR_BOSS_MONSTER_SUMMON_DATA, pSWarData, SendBuf);

				pSWarData->SummonMonsterUnitkind = pSummonData->pMonsterInfo->MonsterUnitKind;
				pSWarData->RemainMinute = 0;
				pSWarData->SummonMonsterTime = now;
				pSWarData->ContributionPoint = SetMSWarInfoContributionPoint(pSummonData->pMonsterInfo->MonsterUnitKind);			// 2008-04-22 by dhjin, 모선전, 거점전 정보창 기획안 - 세력포인트
				pSWarData->BossStep = pSummonData->BossStep;		// 2009-03-10 by dhjin, 단계별 모선 시스템 - 모선 단계
				pSWarData->BeforeWinCheck = pSummonData->BeforeWinCheck;				// 2009-03-10 by dhjin, 단계별 모선 시스템 - 전 단계 승패 여부
				m_pFieldIOCP14->SendMessageToAllClients(SendBuf, MSG_SIZE(MSG_FC_WAR_BOSS_MONSTER_SUMMON_DATA), INFLUENCE_TYPE_VCN | INFLUENCE_TYPE_ANI);

				///////////////////////////////////////////////////////////////////////////////
				// 2006-04-18 by cmkwon, 자코 몬스터 소환 취소 - 세력전보스몬스터 소환시부터 자코 몬스터 소환 안됨
				SetSummonJacoMonsterW(GET_SAME_CHARACTER_INFL_BY_MONSTER_BELL(pSummonData->pMonsterInfo->Belligerence), FALSE);
				INIT_MSG(MSG_FN_NPCSERVER_SUMMON_JACO_MONSTER, T_FN_NPCSERVER_SUMMON_JACO_MONSTER, pSJacoMonster, SendBuf);
				pSJacoMonster->Belligerence0 = (BELL_INFLUENCE_VCN == pSummonData->pMonsterInfo->Belligerence) ? BELL_INFLUENCE_ANI : BELL_INFLUENCE_VCN;
				pSJacoMonster->IsSummonJacoMonster = FALSE;

				// 2007-08-22 by cmkwon, 함수명 변경(<-Send2NPCServer)
				m_pFieldIOCP14->Send2NPCServerByTCP(SendBuf, MSG_SIZE(MSG_FN_NPCSERVER_SUMMON_JACO_MONSTER));

				if (BELL_INFLUENCE_VCN == pSummonData->pMonsterInfo->Belligerence)
				{// 2007-02-06 by dhjin, 소환시 소환된 정보를 가진다.
					m_SummonVCNBossMonsterInfo.bSummonBossMonster = TRUE;
					m_SummonVCNBossMonsterInfo.SummonBossTime = now;
					m_SummonVCNBossMonsterInfo.SummonMonsterUnitkind = pSummonData->pMonsterInfo->MonsterUnitKind;
					m_SummonVCNBossMonsterInfo.ContributionPoint = SetMSWarInfoContributionPoint(pSummonData->pMonsterInfo->MonsterUnitKind);	// 2008-04-22 by dhjin, 모선전, 거점전 정보창 기획안 - 세력포인트
					m_SummonVCNBossMonsterInfo.BossStep = pSummonData->BossStep;			// 2009-03-10 by dhjin, 단계별 모선 시스템 - 모선 단계
					m_SummonVCNBossMonsterInfo.BeforeWinCheck = pSummonData->BeforeWinCheck;				// 2009-03-10 by dhjin, 단계별 모선 시스템 - 전 단계 승패 여부
				}
				else
				{
					m_SummonANIBossMonsterInfo.bSummonBossMonster = TRUE;
					m_SummonANIBossMonsterInfo.SummonBossTime = now;
					m_SummonANIBossMonsterInfo.SummonMonsterUnitkind = pSummonData->pMonsterInfo->MonsterUnitKind;
					m_SummonANIBossMonsterInfo.ContributionPoint = SetMSWarInfoContributionPoint(pSummonData->pMonsterInfo->MonsterUnitKind);	// 2008-04-22 by dhjin, 모선전, 거점전 정보창 기획안 - 세력포인트
					m_SummonANIBossMonsterInfo.BossStep = pSummonData->BossStep;			// 2009-03-10 by dhjin, 단계별 모선 시스템 - 모선 단계
					m_SummonANIBossMonsterInfo.BeforeWinCheck = pSummonData->BeforeWinCheck;				// 2009-03-10 by dhjin, 단계별 모선 시스템 - 전 단계 승패 여부
				}

				///////////////////////////////////////////////////////////////////////////////
				// 2006-04-14 by cmkwon
				itr = m_mtvectSummonMonsterDataList.erase(itr);

				// 2012-07-02 by hskim, 모선전 개선 - 모선전 진행시 스톤즈 루인 진입 불가
				auto pFMPro = reinterpret_cast<CFieldMapProject*>(m_pFieldIOCP14->GetMapWorkspace()->GetMapProjectByMapIndex(STONES_RUIN_MAP_INDEX));

				if (pFMPro)
				{
					auto m_MaxChannelPerMap = pFMPro->GetNumberOfMapChannels();

					for (auto ChannelCount = 0; ChannelCount < m_MaxChannelPerMap; ChannelCount++)
					{
						auto pTargetFieldMapChannel = pFMPro->GetFieldMapChannelByIndex(ChannelCount);

						if (pTargetFieldMapChannel)

							pTargetFieldMapChannel->WarpToCityMapFieldMapChannelByInfluence(INFLUENCE_TYPE_UNKNOWN);
					}
				}
				// 2012-07-02 by hskim, 모선전 개선 - 모선전 진행시 스톤즈 루인 진입 불가

				continue;
			}

			if (5 == nRemainMinute % 10 || 3 == nRemainMinute || 1 == nRemainMinute)
			{
				///////////////////////////////////////////////////////////////////////////////
				// 2006-04-14 by cmkwon, 세력전 보스 몬스터 소환 정보 클라이어트로 전송
				INIT_MSG_WITH_BUFFER(MSG_FC_WAR_BOSS_MONSTER_SUMMON_DATA, T_FC_WAR_BOSS_MONSTER_SUMMON_DATA, pSWarData, SendBuf);
				pSWarData->SummonMonsterUnitkind = pSummonData->pMonsterInfo->MonsterUnitKind;
				pSWarData->RemainMinute = nRemainMinute;
				pSWarData->BossStep = pSummonData->BossStep;		// 2009-03-10 by dhjin, 단계별 모선 시스템 - 모선 단계
				pSWarData->BeforeWinCheck = pSummonData->BeforeWinCheck;				// 2009-03-10 by dhjin, 단계별 모선 시스템 - 전 단계 승패 여부
				m_pFieldIOCP14->SendMessageToAllClients(SendBuf, MSG_SIZE(MSG_FC_WAR_BOSS_MONSTER_SUMMON_DATA), INFLUENCE_TYPE_VCN | INFLUENCE_TYPE_ANI);
			}

			itr++;
		}
		
		SetDoingInfluenceWar();	// 2012-11-26 by jhseol, 전쟁 시스템 리뉴얼 - 전쟁시 트리거 무적상태(일시정지), 지금이 전쟁중인지 확인하는 함수.(모선전 체크가 목적)
	}
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			void CInflWarManager::OnInfluenceBossMonsterDeadInflWarManager(MONSTER_INFO *pMonInfo)
/// \brief		
/// \author		cmkwon
/// \date		2006-04-14 ~ 2006-04-14
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CInflWarManager::OnInfluenceBossMonsterDeadInflWarManager(MONSTER_INFO *pMonInfo)
{
	if(NULL == pMonInfo){					return;}
	
	// 2007-08-23 by cmkwon, 모선전 보스 몬스터 구분 처리 - 아래와 같이 수정함, 추가된 디파인 사용
	//if(FALSE == IS_INFLWAR_MONSTER(pMonInfo->Belligerence))
	if(FALSE == IS_MOTHERSHIPWAR_MONSTER(pMonInfo->Belligerence))
	{// 2006-11-20 by cmkwon

		return;
	}

	///////////////////////////////////////////////////////////////////////////////
	// 2006-12-13 by cmkwon, 몬스터와 적대 세력의 InfluenceType을 구한다.
	BYTE byInflTy = INFLUENCE_TYPE_UNKNOWN;
	BYTE byLoseInflTy = INFLUENCE_TYPE_UNKNOWN;		// 2008-01-07 by dhjin, 세력 보상 수정 - 
	if(IS_BELL_VCN(pMonInfo->Belligerence))	
	{
		byInflTy	= INFLUENCE_TYPE_ANI;
		byLoseInflTy = INFLUENCE_TYPE_VCN;		// 2008-01-07 by dhjin, 세력 보상 수정 - 
	}
	else
	{
		byInflTy	= INFLUENCE_TYPE_VCN;
		byLoseInflTy = INFLUENCE_TYPE_ANI;		// 2008-01-07 by dhjin, 세력 보상 수정 - 
	}

	///////////////////////////////////////////////////////////////////////////////
	// 2006-04-14 by cmkwon, 
	CInflWarData *pInflWarData = GetInflWarDataByInflType(byInflTy);
	if(NULL == pInflWarData)
	{
		return;
	}

	// 2007-02-06 by dhjin, 소환된 보스 (전함)이 격추 되었으므로  초기화 한다.
	if(IS_BELL_VCN(pMonInfo->Belligerence))
	{
		util::zero(&m_SummonVCNBossMonsterInfo, sizeof(SSUMMONBOSSMONSTER_INFO));
	}
	else
	{
		util::zero(&m_SummonANIBossMonsterInfo, sizeof(SSUMMONBOSSMONSTER_INFO));
	}

	SetDoingInfluenceWar();	// 2012-11-26 by jhseol, 전쟁 시스템 리뉴얼 - 전쟁시 트리거 무적상태(일시정지), 전쟁 종료를 실시간으로 확인 하기 위해 함수호출(모선전 종료)

	mt_auto_lock mtA(pInflWarData->GetmtlockPtr());	

	// 2013-05-09 by hskim, 세력 포인트 개선
	SDB_INFLUENCE_WAR_INFO *pNewInfo = pInflWarData->GetCurInflWarInfo();		// 위치 변경

#ifdef S_UPGRADE_INFLUENCE_POINT_HSKIM
	CalcContributionPoint(pNewInfo->InfluenceType, CONTRIBUTIONPOINT_BOSS_MONSTER_WINNER, CONTRIBUTIONPOINT_BOSS_MONSTER_LOSER);
	CalcConsecutiveVictoriesPoint(pNewInfo->InfluenceType);
#else
	///////////////////////////////////////////////////////////////////////////////
	// 2006-04-14 by cmkwon, 세력전 기여도 증가
	pInflWarData->ContributionPoint += ADD_CONTRIBUTIONPOINT_BOSS_MONSTER;
	int nContributionPoint = pInflWarData->ContributionPoint;
#endif
// end 2013-05-09 by hskim, 세력 포인트 개선

//////////////////////////////////////////////////////////////////////////
// 2009-03-10 by dhjin, 단계별 모선 시스템 - 밑과 같이 전시단계를 수정하여 보상을 지급한다.
//////////////////////////////////////////////////////////////////////////
// 2009-01-12 by dhjin, 선전 포고 - 밑과 같이 수정 전시단계는 여기서 설정하지 않는다. 다음 주기로 바뀔때 설정함.
//	///////////////////////////////////////////////////////////////////////////////
//	// 2006-04-17 by cmkwon, 보스 몬스터 격추로 전시단계 다시 설정됨
//	SDB_INFLUENCE_WAR_INFO *pNewInfo = pInflWarData->GetInflWarInfoByContributionPoint(pInflWarData->ContributionPoint);
//	pInflWarData->SetWartimeStage(pNewInfo);
//	SDB_INFLUENCE_WAR_INFO *pNewInfo = pInflWarData->GetCurInflWarInfo();

// 2009-07-02 by cmkwon, 선전포고 관련 버그 수정 - 아래와 같이 수정함.
// 	SDB_INFLUENCE_WAR_INFO *pNewInfo;
// 	if(INFLUENCE_TYPE_VCN == byInflTy)
// 	{
// 		pInflWarData->SetCurrentStepInflWarData(this->m_DeclarationOfWar.GetCurrentMSUID(INFLUENCE_TYPE_VCN), INFLUENCE_TYPE_VCN);
// 		pNewInfo = pInflWarData->GetCurInflWarInfo();
// //		CInflWarData *pBCUInflWarData = GetInflWarDataByInflType(INFLUENCE_TYPE_VCN);
// //		if(NULL == pBCUInflWarData)
// //		{
// //			return;
// //		}
// //		pBCUInflWarData->SetCurrentStepInflWarData(this->m_DeclarationOfWar.GetCurrentMSUID(INFLUENCE_TYPE_VCN), INFLUENCE_TYPE_VCN);
// //		pNewInfo = pBCUInflWarData->GetCurInflWarInfo();
// 	}
// 	else
// 	{
// 		pInflWarData->SetCurrentStepInflWarData(this->m_DeclarationOfWar.GetCurrentMSUID(INFLUENCE_TYPE_ANI), INFLUENCE_TYPE_ANI);
// 		pNewInfo = pInflWarData->GetCurInflWarInfo();		
// //		CInflWarData *pANIInflWarData = GetInflWarDataByInflType(INFLUENCE_TYPE_ANI);
// //		if(NULL == pANIInflWarData)
// //		{
// //			return;
// //		}
// //		pANIInflWarData->SetCurrentStepInflWarData(this->m_DeclarationOfWar.GetCurrentMSUID(INFLUENCE_TYPE_ANI), INFLUENCE_TYPE_ANI);
// //		pNewInfo = pANIInflWarData->GetCurInflWarInfo();		
// 	}
	///////////////////////////////////////////////////////////////////////////////
	// 2009-07-02 by cmkwon, 선전포고 관련 버그 수정 - 	
	pInflWarData->SetCurrentStepInflWarData(pMonInfo->MonsterUnitKind, byInflTy);
	this->SetCompensationForDecalationOfWarWin(byInflTy, pMonInfo->MonsterUnitKind);			// 2009-07-02 by cmkwon, 선전포고 관련 버그 수정 - 

// 2009-03-10 by dhjin, 단계별 모선 시스템 - 세력전 승리 시 지급 되는 이벤트 수정 - SetCurrentStepInflWarData에서 할당하므로 주석처리한다.
//////////////////////////////////////////////////////////////////////////
// 2009-03-31 by dhjin, 선전 포고 - 세력전 승리 시 지급 되는 이벤트 수정 - 세력 이벤트를 설정하고 클라이언트에게 전송한다.
//	pInflWarData->fInflHPRepairRate = pNewInfo->HPRepairRate;
//	pInflWarData->fInflDPRepairRate = pNewInfo->DPRepairRate;
//	pInflWarData->fInflSPRepairRate = pNewInfo->SPRepairRate;
	INIT_MSG_WITH_BUFFER(MSG_FC_WAR_INFLUENCE_DATA, T_FC_WAR_INFLUENCE_DATA, pSInflData, SendBufInflData);
	pSInflData->byInfluenceType		= pNewInfo->InfluenceType;
	pSInflData->fHPRepairRate		= pInflWarData->fInflHPRepairRate;
	pSInflData->fDPRepairRate		= pInflWarData->fInflDPRepairRate;
	pSInflData->fSPRepairRate		= pInflWarData->fInflSPRepairRate;
	m_pFieldIOCP14->SendMessageToAllClients(SendBufInflData, MSG_SIZE(MSG_FC_WAR_INFLUENCE_DATA), pNewInfo->InfluenceType);
	mtA.auto_unlock_cancel();

	///////////////////////////////////////////////////////////////////////////////
	// 2006-04-27 by cmkwon
	char szSysLog[1024];
	wsprintf(szSysLog, "  [Notify]: InflWar OnInflBossMonsterDead InflTy[%d] CurWartimeStage(%d)\r\n"
		, pNewInfo->InfluenceType, pInflWarData->WartimeStage);
	g_pFieldGlobal->WriteSystemLog(szSysLog);
	DBGOUT(szSysLog);

// 2013-05-09 by hskim, 세력 포인트 개선
#ifdef S_UPGRADE_INFLUENCE_POINT_HSKIM
#else
	///////////////////////////////////////////////////////////////////////////////
	// 2006-04-19 by cmkwon, 도시맵 유저에게 기여도 전송
	INIT_MSG_WITH_BUFFER(MSG_FC_WAR_UPDATE_CONTRIBUTION_POINT_OK, T_FC_WAR_UPDATE_CONTRIBUTION_POINT_OK, pSConPoint, SendBuf);
	pSConPoint->byInfluenceType		= pNewInfo->InfluenceType;
	pSConPoint->nContributionPoint	= nContributionPoint;
	m_pFieldIOCP14->SendMessageToCityClients(SendBuf, MSG_SIZE(MSG_FC_WAR_UPDATE_CONTRIBUTION_POINT_OK), pSConPoint->byInfluenceType);

	///////////////////////////////////////////////////////////////////////////////
	// 2006-04-25 by cmkwon, DB에 기여도 저장
	QPARAM_UPDATE_INFLUENCE_WAR_DATA *pS2DB = new QPARAM_UPDATE_INFLUENCE_WAR_DATA;
	pS2DB->InfluenceType0		= pNewInfo->InfluenceType;
	//pS2DB->MGameServerID0		= g_pFieldGlobal->GetMGameServerID();
	pS2DB->WartimeStage0		= pNewInfo->WartimeStage;
	pS2DB->ContributionPoint0	= nContributionPoint;
	m_pFieldIOCP14->m_pAtumDBManager->MakeAndEnqueueQuery(QT_UpdateInfluenceWarData, NULL, 0, pS2DB);
#endif
// end 2013-05-09 by hskim, 세력 포인트 개선

//////////////////////////////////////////////////////////////////////////
// 2009-01-12 by dhjin, 선전 포고 - 밑과 같이 주석 처리(DB에 값 저장할 필요 없다.)
//	///////////////////////////////////////////////////////////////////////////////
//	// 2006-07-12 by cmkwon, 분쟁 지역 소유 세력 수정
//	QPARAM_UPDATE_OWNEROFCONFLICTAREA *pOwner2DB = new QPARAM_UPDATE_OWNEROFCONFLICTAREA;
//	pOwner2DB->InfluenceType0	= pNewInfo->InfluenceType;
//	pOwner2DB->MGameServerID0	= g_pFieldGlobal->GetMGameServerID();
//	m_pFieldIOCP14->m_pAtumDBManager->MakeAndEnqueueQuery(QT_UpdateOwnerOfConflictArea, NULL, 0, pOwner2DB);

// 2009-03-31 by dhjin, 선전 포고 - 세력전 승리 시 지급 되는 이벤트 수정 - 위로 올림
//	///////////////////////////////////////////////////////////////////////////////
//	// 2006-04-21 by cmkwon
//	INIT_MSG(MSG_FC_WAR_INFLUENCE_DATA, T_FC_WAR_INFLUENCE_DATA, pSInflData, SendBuf);
//	pSInflData->byInfluenceType		= pNewInfo->InfluenceType;
//	pSInflData->fHPRepairRate		= pNewInfo->HPRepairRate;
//	pSInflData->fDPRepairRate		= pNewInfo->DPRepairRate;
//	pSInflData->fSPRepairRate		= pNewInfo->SPRepairRate;
//	m_pFieldIOCP14->SendMessageToAllClients(SendBuf, MSG_SIZE(MSG_FC_WAR_INFLUENCE_DATA), pNewInfo->InfluenceType);

// 2008-05-20 by cmkwon, 모든 이벤트(HappyHoure,MotherShip,Item) 그룹 동시에 가능하게 - 모선전 이벤트 진행
// 	///////////////////////////////////////////////////////////////////////////////
// 	// 2006-04-14 by cmkwon,
// 	if(0 < pNewInfo->EventTime
// 		&& FALSE == m_pFieldIOCP14->m_gameEventManager.IsDoingAllGameEvent(INFLUENCE_TYPE_ALL_MASK))		// 2006-04-21 by cmkwon, 전체 이벤트가 진행중이 아닐때만
// 	{// 2006-04-17 by cmkwon, 게임 이벤트 처리
// 
// 		char szTemp[1024];
// 		if(m_pFieldIOCP14->m_gameEventManager.StartGameEvent(GAME_EVENT_TYPE_EXP, pNewInfo->EventExperience, pNewInfo->EventTime, 1, CHARACTER_MAX_LEVEL, byInflTy))	// 2008-03-14 by dhjin, Level별 해피아워 이벤트 구현 -
// 		{
// 			sprintf(szTemp, STRMSG_S_F2NOTIFY_0116
// 				, CGameEventManager::GetGameEventTypeString(GAME_EVENT_TYPE_EXP), pNewInfo->EventExperience, pNewInfo->EventTime);
// 			m_pFieldIOCP14->SendString128ToAllClients(STRING_128_USER_NOTICE, szTemp, TRUE, byInflTy);
// 		}
// 		if(m_pFieldIOCP14->m_gameEventManager.StartGameEvent(GAME_EVENT_TYPE_SPI, pNewInfo->EventDropSPI, pNewInfo->EventTime, 1, CHARACTER_MAX_LEVEL, byInflTy))		// 2008-03-14 by dhjin, Level별 해피아워 이벤트 구현 -
// 		{
// 			sprintf(szTemp, STRMSG_S_F2NOTIFY_0116
// 				, CGameEventManager::GetGameEventTypeString(GAME_EVENT_TYPE_SPI), pNewInfo->EventDropSPI, pNewInfo->EventTime);
// 			m_pFieldIOCP14->SendString128ToAllClients(STRING_128_USER_NOTICE, szTemp, TRUE, byInflTy);
// 		}
// 		if(m_pFieldIOCP14->m_gameEventManager.StartGameEvent(GAME_EVENT_TYPE_EXP_NO_DOWN, pNewInfo->EventDownExperience, pNewInfo->EventTime, 1, CHARACTER_MAX_LEVEL, byInflTy))		// 2008-03-14 by dhjin, Level별 해피아워 이벤트 구현 -
// 		{
// 			sprintf(szTemp, STRMSG_S_F2NOTIFY_0116
// 				, CGameEventManager::GetGameEventTypeString(GAME_EVENT_TYPE_EXP_NO_DOWN), pNewInfo->EventDownExperience, pNewInfo->EventTime);
// 			m_pFieldIOCP14->SendString128ToAllClients(STRING_128_USER_NOTICE, szTemp, TRUE, byInflTy);
// 		}
// 		if(m_pFieldIOCP14->m_gameEventManager.StartGameEvent(GAME_EVENT_TYPE_DROP_ITEM, pNewInfo->EventDropItem, pNewInfo->EventTime, 1, CHARACTER_MAX_LEVEL, byInflTy))			// 2008-03-14 by dhjin, Level별 해피아워 이벤트 구현 -
// 		{
// 			sprintf(szTemp, STRMSG_S_F2NOTIFY_0116
// 				, CGameEventManager::GetGameEventTypeString(GAME_EVENT_TYPE_DROP_ITEM), pNewInfo->EventDropItem, pNewInfo->EventTime);
// 			m_pFieldIOCP14->SendString128ToAllClients(STRING_128_USER_NOTICE, szTemp, TRUE, byInflTy);
// 		}
// 		if(m_pFieldIOCP14->m_gameEventManager.StartGameEvent(GAME_EVENT_TYPE_DROP_REAR_ITEM, pNewInfo->EventDropRareItem, pNewInfo->EventTime, 1, CHARACTER_MAX_LEVEL, byInflTy))		// 2008-03-14 by dhjin, Level별 해피아워 이벤트 구현 -
// 		{
// 			sprintf(szTemp, STRMSG_S_F2NOTIFY_0116
// 				, CGameEventManager::GetGameEventTypeString(GAME_EVENT_TYPE_DROP_REAR_ITEM), pNewInfo->EventDropRareItem, pNewInfo->EventTime);
// 			m_pFieldIOCP14->SendString128ToAllClients(STRING_128_USER_NOTICE, szTemp, TRUE, byInflTy);
// 		}
// 	}
	///////////////////////////////////////////////////////////////////////////////	
	// 2008-05-20 by cmkwon, 모든 이벤트(HappyHoure,MotherShip,Item) 그룹 동시에 가능하게 - 
	if(pNewInfo->IsValidGameEvent())
	{
		if(FALSE == m_pFieldIOCP14->m_gameEventManager.IsProgressGameEventGroup(byInflTy, GAME_EVENT_GROUP_MOTHERSHIP))
		{
			g_pFieldGlobal->WriteSystemLogEX(TRUE, "[Notify]: GAME_EVENT_GROUP_MOTHERSIP start ! WarTimeStage(%d) InfluenceType(%d)\r\n", pNewInfo->WartimeStage, pNewInfo->InfluenceType);

			m_pFieldIOCP14->m_gameEventManager.StartGameEvent(GAME_EVENT_TYPE_EXP, pNewInfo->EventExperience, pNewInfo->EventTime, 1, CHARACTER_MAX_LEVEL, byInflTy, GAME_EVENT_GROUP_MOTHERSHIP);
 			m_pFieldIOCP14->m_gameEventManager.StartGameEvent(GAME_EVENT_TYPE_SPI, pNewInfo->EventDropSPI, pNewInfo->EventTime, 1, CHARACTER_MAX_LEVEL, byInflTy, GAME_EVENT_GROUP_MOTHERSHIP);
 			m_pFieldIOCP14->m_gameEventManager.StartGameEvent(GAME_EVENT_TYPE_EXP_NO_DOWN, pNewInfo->EventDownExperience, pNewInfo->EventTime, 1, CHARACTER_MAX_LEVEL, byInflTy, GAME_EVENT_GROUP_MOTHERSHIP);
 			m_pFieldIOCP14->m_gameEventManager.StartGameEvent(GAME_EVENT_TYPE_DROP_ITEM, pNewInfo->EventDropItem, pNewInfo->EventTime, 1, CHARACTER_MAX_LEVEL, byInflTy, GAME_EVENT_GROUP_MOTHERSHIP);
 			m_pFieldIOCP14->m_gameEventManager.StartGameEvent(GAME_EVENT_TYPE_DROP_REAR_ITEM, pNewInfo->EventDropRareItem, pNewInfo->EventTime, 1, CHARACTER_MAX_LEVEL, byInflTy, GAME_EVENT_GROUP_MOTHERSHIP);

			// 2008-05-20 by cmkwon, 모든 이벤트(HappyHoure,MotherShip,Item) 그룹 동시에 가능하게 - 해당 세력 유저에게 전송
			INIT_MSG_WITH_BUFFER(MSG_FC_INFO_GET_HAPPY_HOUR_EVENT_INFO_OK, T_FC_INFO_GET_HAPPY_HOUR_EVENT_INFO_OK, pSHappy, SendBuf);		// 2013-05-09 by hskim, 세력 포인트 개선
			pSHappy->byStateType		= GAME_EVENT_GROUP_MOTHERSHIP_START;
			pSHappy->byInfluenceType4	= byInflTy;
			pSHappy->StartATime.SetCurrentDateTime(TRUE);
			pSHappy->EndATime			= pSHappy->StartATime;
			pSHappy->EndATime.AddDateTime(0, 0, 0, 0, pNewInfo->EventTime);
			pSHappy->fEXPRate2			= pNewInfo->EventExperience;
			pSHappy->fSPIRate2			= pNewInfo->EventDropSPI;
			pSHappy->fEXPRepairRate2	= pNewInfo->EventDownExperience;
			pSHappy->fDropItemRate2		= pNewInfo->EventDropItem;
			pSHappy->fDropRareRate2		= pNewInfo->EventDropRareItem;
			m_pFieldIOCP14->SendMessageToAllClients(SendBuf, MSG_SIZE(MSG_FC_INFO_GET_HAPPY_HOUR_EVENT_INFO_OK), pSHappy->byInfluenceType4);
		}
		else
		{
			g_pFieldGlobal->WriteSystemLogEX(TRUE, "[Notify]: GAME_EVENT_GROUP_MOTHERSIP error (progress other GameEvent)!! WarTimeStage(%d) InfluenceType(%d)\r\n", pNewInfo->WartimeStage, pNewInfo->InfluenceType);
		}
	}

// 2012-07-12 by hskim, 모선전 종료시 아이템 추가 지급 - 세력의 영광 (7037060)
#if S_MS_WAR_FIN_SUPPLY_ITEM_ADD
	// 모선전 종료시 세력의 영광 (7037060) 일괄 지급
	const int nItemNumAdd = 7037060;
	ITEM *pItemAdd = m_pFieldIOCP14->GetItemInfo(nItemNumAdd);
	if(pItemAdd)
	{
		m_pFieldIOCP14->InsertItemAllCharacter(nItemNumAdd, 1, pNewInfo->InfluenceType, TRUE);
	}
#endif
// end 2012-07-12 by hskim, 모선전 종료시 아이템 추가 지급 - 세력의 영광 (7037060)

	if(0 != pNewInfo->SupplyItemNum
		&& 0 < pNewInfo->SupplyItemCount)
	{// 2006-04-17 by cmkwon, 아아템 지급
		ITEM *pItem = m_pFieldIOCP14->GetItemInfo(pNewInfo->SupplyItemNum);
		if(pItem)
		{
			m_pFieldIOCP14->InsertItemAllCharacter(pNewInfo->SupplyItemNum, pNewInfo->SupplyItemCount, pNewInfo->InfluenceType, TRUE);
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// 2007-05-16 by dhjin, 추가됨 2번째 아이템 지급이 존재하면 지급
	if(0 != pNewInfo->SupplyItemNum2
		&& 0 < pNewInfo->SupplyItemCount2)
	{// 2006-04-17 by cmkwon, 아아템 지급
		ITEM *pItem = m_pFieldIOCP14->GetItemInfo(pNewInfo->SupplyItemNum2);
		if(pItem)
		{
			m_pFieldIOCP14->InsertItemAllCharacter(pNewInfo->SupplyItemNum2, pNewInfo->SupplyItemCount2, pNewInfo->InfluenceType, TRUE);
		}
	}

	// 2006-04-17 by cmkwon, 중급보스 몬스터 소환 이벤트
	for(int i=0; i<MAX_INFLUENCEWAR_EVENT_SUMMON_COUNT; i++)
	{
		if(0 >= pNewInfo->SummonMonsterUnitKind[i]
			|| 0 >= pNewInfo->SummonCount[i]
			|| 0 >= pNewInfo->SummonMapIndex[i])
		{
			continue;
		}
		
		MONSTER_INFO *pMonInfo = m_pFieldIOCP14->GetMonsterInfo(pNewInfo->SummonMonsterUnitKind[i]);
		if(NULL == pMonInfo)
		{
			continue;
		}
		CFieldMapProject *pFMapPro = m_pFieldIOCP14->GetFieldMapProjectByMapIndex(pNewInfo->SummonMapIndex[i]);
		if(pFMapPro)
		{// 2006-04-17 by cmkwon, 모든 채널에 몬스터를 소환한다.
			MSG_FN_ADMIN_SUMMON_MONSTER tmSummon;
				util::zero(&tmSummon, sizeof(MSG_FN_ADMIN_SUMMON_MONSTER));

			tmSummon.ChannelIndex			= 0;
			util::strncpy(tmSummon.CharacterName, "InfluenceWar", SIZE_MAX_CHARACTER_NAME);
			tmSummon.MonsterUnitKind		= pNewInfo->SummonMonsterUnitKind[i];
			tmSummon.NumOfMonster			= pNewInfo->SummonCount[i];
			tmSummon.Position				= AVECTOR3(pNewInfo->SummonPositionX[i], pNewInfo->SummonPositionY[i], pNewInfo->SummonPositionZ[i]);

			// 2010. 07. 05 by hsLee. 몬스터 소환 관련. (밸런스 데이터 누락 수정.)
			tmSummon.MonsterBalanceData.Init(0);

			pFMapPro->SendSummonMessageToAllChannels(&tmSummon);
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// 2007-05-15 by dhjin, WarPoint 증가 (300점)
	m_pFieldIOCP14->SendWarPointToInflClient(byInflTy, ADD_WARPOINT_BOSS_MONSTER);
	m_pFieldIOCP14->SendWarPointToInflClient(byLoseInflTy, ADD_WARPOINT_BOSS_MONSTER_BY_LOSE);	// 2008-01-07 by dhjin, 세력 보상 수정 - 
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CInflWarManager::OnInfluenceBossMonsterAutoDestroyedInflWarManager(MONSTER_INFO *pMonInfo)
/// \brief		모선전 방어시에도 보상은 지급한다. 세력 포인트, 전쟁 포인트, 미스터리 캡슐
/// \author		dhjin
/// \date		2007-05-15 ~ 2007-05-15
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CInflWarManager::OnInfluenceBossMonsterAutoDestroyedInflWarManager(MONSTER_INFO *pMonInfo)
{
	if(NULL == pMonInfo){					return;}
	
	// 2007-08-23 by cmkwon, 모선전 보스 몬스터 구분 처리 - 아래와 같이 수정함, 추가된 디파인 사용
	//if(FALSE == IS_INFLWAR_MONSTER(pMonInfo->Belligerence))
	if(FALSE == IS_MOTHERSHIPWAR_MONSTER(pMonInfo->Belligerence))
	{// 2006-11-20 by cmkwon

		return;
	}

	///////////////////////////////////////////////////////////////////////////////
	// 2006-12-13 by cmkwon, 몬스터와 적대 세력의 InfluenceType을 구한다.
	BYTE byInflTy = INFLUENCE_TYPE_UNKNOWN;
	BYTE byWinInflTy = INFLUENCE_TYPE_UNKNOWN;		// 2007-05-17 by dhjin, 방어전에 성공 한 세력은 모선을 띄운 세력과는 반대 세력이므로 보상 지급 시 주의 한다

	if(IS_BELL_VCN(pMonInfo->Belligerence))
	{
		byInflTy	= INFLUENCE_TYPE_ANI;
		byWinInflTy	= INFLUENCE_TYPE_VCN;
	}
	else
	{
		byInflTy	= INFLUENCE_TYPE_VCN;
		byWinInflTy = INFLUENCE_TYPE_ANI;
	}

	///////////////////////////////////////////////////////////////////////////////
	// 2006-04-14 by cmkwon, 
	CInflWarData *pInflWarData = GetInflWarDataByInflType(byInflTy);
	if(NULL == pInflWarData)
	{
		return;
	}
	CInflWarData *pWinInflWarData = GetInflWarDataByInflType(byWinInflTy);
	if(NULL == pWinInflWarData)
	{
		return;
	}

	// 2007-02-06 by dhjin, 소환된 보스 (전함)이 격추 되었으므로  초기화 한다.
	if(IS_BELL_VCN(pMonInfo->Belligerence))
	{
		util::zero(&m_SummonVCNBossMonsterInfo, sizeof(SSUMMONBOSSMONSTER_INFO));
	}
	else
	{
		util::zero(&m_SummonANIBossMonsterInfo, sizeof(SSUMMONBOSSMONSTER_INFO));
	}
	
	SetDoingInfluenceWar();	// 2013-04-17 by jhseol, 비 전쟁시 트리거 이용 불가능 현상 버그수정 (모선 자동 파괴 시 에도 종료 체크를 하도록 추가)

	///////////////////////////////////////////////////////////////////////////////
	// 2006-04-17 by cmkwon, 모선전 관련 정보만 얻어온다.
	SDB_INFLUENCE_WAR_INFO *pNewInfo = pInflWarData->GetInflWarInfoByContributionPoint(pInflWarData->ContributionPoint);

// 2013-05-09 by hskim, 세력 포인트 개선
#ifdef S_UPGRADE_INFLUENCE_POINT_HSKIM
	CalcContributionPoint(pNewInfo->InfluenceType, CONTRIBUTIONPOINT_BOSS_MONSTER_WINNER, CONTRIBUTIONPOINT_BOSS_MONSTER_LOSER);
	CalcConsecutiveVictoriesPoint(pNewInfo->InfluenceType);		// 2013-05-09 by hskim, 세력 포인트 개선
#else
	mt_auto_lock mtA(pWinInflWarData->GetmtlockPtr());	
	///////////////////////////////////////////////////////////////////////////////
	// 2006-04-14 by cmkwon, 세력전 기여도 증가
	pWinInflWarData->ContributionPoint += ADD_CONTRIBUTIONPOINT_BOSS_MONSTER;
	int nContributionPoint = pWinInflWarData->ContributionPoint;
	mtA.auto_unlock_cancel();
#endif
// end 2013-05-09 by hskim, 세력 포인트 개선

	///////////////////////////////////////////////////////////////////////////////
	// 2006-04-27 by cmkwon
	char szSysLog[1024];
	wsprintf(szSysLog, "  [Notify]: InflWar OnInflBossMonsterAutoDestroyed InflTy[%d] CurWartimeStage(%d)\r\n"
		, byWinInflTy, pWinInflWarData->WartimeStage);
	g_pFieldGlobal->WriteSystemLog(szSysLog);
	DBGOUT(szSysLog);

// 2013-05-09 by hskim, 세력 포인트 개선
#ifdef S_UPGRADE_INFLUENCE_POINT_HSKIM
#else
	///////////////////////////////////////////////////////////////////////////////
	// 2006-04-19 by cmkwon, 도시맵 유저에게 기여도 전송
	INIT_MSG_WITH_BUFFER(MSG_FC_WAR_UPDATE_CONTRIBUTION_POINT_OK, T_FC_WAR_UPDATE_CONTRIBUTION_POINT_OK, pSConPoint, SendBuf);
	pSConPoint->byInfluenceType		= byWinInflTy;
	pSConPoint->nContributionPoint	= nContributionPoint;
	m_pFieldIOCP14->SendMessageToCityClients(SendBuf, MSG_SIZE(MSG_FC_WAR_UPDATE_CONTRIBUTION_POINT_OK), byWinInflTy);

	///////////////////////////////////////////////////////////////////////////////
	// 2006-04-25 by cmkwon, DB에 기여도 저장
	QPARAM_UPDATE_INFLUENCE_WAR_DATA *pS2DB = new QPARAM_UPDATE_INFLUENCE_WAR_DATA;
	pS2DB->InfluenceType0		= byWinInflTy;
	//pS2DB->MGameServerID0		= g_pFieldGlobal->GetMGameServerID();
	pS2DB->WartimeStage0		= pWinInflWarData->WartimeStage;
	pS2DB->ContributionPoint0	= nContributionPoint;
	m_pFieldIOCP14->m_pAtumDBManager->MakeAndEnqueueQuery(QT_UpdateInfluenceWarData, NULL, 0, pS2DB);
#endif
// end 2013-05-09 by hskim, 세력 포인트 개선

	if(0 != pNewInfo->SupplyItemNum
		&& 0 < pNewInfo->SupplyItemCount)
	{// 2006-04-17 by cmkwon, 아아템 지급
		ITEM *pItem = m_pFieldIOCP14->GetItemInfo(pNewInfo->SupplyItemNum);
		if(pItem)
		{
			m_pFieldIOCP14->InsertItemAllCharacter(pNewInfo->SupplyItemNum, pNewInfo->SupplyItemCount, byWinInflTy, TRUE);
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// 2007-05-16 by dhjin, 추가됨 2번째 아이템 지급이 존재하면 지급
	if(0 != pNewInfo->SupplyItemNum2
		&& 0 < pNewInfo->SupplyItemCount2)
	{// 2006-04-17 by cmkwon, 아아템 지급
		ITEM *pItem = m_pFieldIOCP14->GetItemInfo(pNewInfo->SupplyItemNum2);
		if(pItem)
		{
			m_pFieldIOCP14->InsertItemAllCharacter(pNewInfo->SupplyItemNum2, pNewInfo->SupplyItemCount2, byWinInflTy, TRUE);
		}
	}
	
	//////////////////////////////////////////////////////////////////////////
	// 2007-05-15 by dhjin, WarPoint 증가 (300점)
	m_pFieldIOCP14->SendWarPointToInflClient(byWinInflTy, ADD_WARPOINT_BOSS_MONSTER);
	m_pFieldIOCP14->SendWarPointToInflClient(byInflTy, ADD_WARPOINT_BOSS_MONSTER_BY_LOSE);	// 2008-01-07 by dhjin, 세력 보상 수정 - 
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			vectDB_INFLUENCE_WAR_INFO *CInflWarManager::GetInflWarInfoByInflType(BYTE i_byInflTy)
/// \brief		
/// \author		cmkwon
/// \date		2006-04-13 ~ 2006-04-13
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
vectDB_INFLUENCE_WAR_INFO *CInflWarManager::GetInflWarInfoByInflType(BYTE i_byInflTy)
{
	if(INFLUENCE_TYPE_VCN == i_byInflTy)
	{
		return &m_vectVCNInflWarInfoList;
	}
	if(INFLUENCE_TYPE_ANI == i_byInflTy)
	{
		return &m_vectANIInflWarInfoList;
	}

	return NULL;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			vectDB_INFLUENCE_WAR_INFO *CInflWarManager::GetVCNInflWarInfo(void)
/// \brief		
/// \author		cmkwon
/// \date		2006-04-13 ~ 2006-04-13
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
vectDB_INFLUENCE_WAR_INFO *CInflWarManager::GetVCNInflWarInfo(void)
{
	return GetInflWarInfoByInflType(INFLUENCE_TYPE_VCN);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			vectDB_INFLUENCE_WAR_INFO *CInflWarManager::GetANIInflWarInfo(void)
/// \brief		
/// \author		cmkwon
/// \date		2006-04-13 ~ 2006-04-13
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
vectDB_INFLUENCE_WAR_INFO *CInflWarManager::GetANIInflWarInfo(void)
{
	return GetInflWarInfoByInflType(INFLUENCE_TYPE_ANI);
}

struct sort_SDB_INFLUENCE_WAR_INFO_byWartimeStage
{
	bool operator()(SDB_INFLUENCE_WAR_INFO op1, SDB_INFLUENCE_WAR_INFO op2)
	{
		return op1.WartimeStage < op2.WartimeStage;		// 오름차순 정렬
	}
};
///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CInflWarManager::InsertInfluenceWarInfo(SDB_INFLUENCE_WAR_INFO *i_pInflWarInfo)
/// \brief		
/// \author		cmkwon
/// \date		2006-04-13 ~ 2006-04-13
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CInflWarManager::InsertInfluenceWarInfo(SDB_INFLUENCE_WAR_INFO *i_pInflWarInfo)
{
	if(INFLUENCE_TYPE_VCN != i_pInflWarInfo->InfluenceType
		&& INFLUENCE_TYPE_ANI != i_pInflWarInfo->InfluenceType)
	{// 2006-04-13 by cmkwon, 세력 체크
		return FALSE;
	}

	vectDB_INFLUENCE_WAR_INFO *pInflWarInfoList = this->GetInflWarInfoByInflType(i_pInflWarInfo->InfluenceType);
	if(NULL == pInflWarInfoList)
	{
		return FALSE;
	}

	for(int i=0; i < pInflWarInfoList->size(); i++)
	{
		if(i_pInflWarInfo->WartimeStage == (*pInflWarInfoList)[i].WartimeStage)
		{// 2006-04-13 by cmkwon, 이미 WartimeStage가 같은 데이터가 존재한다.
			return FALSE;
		}
	}

	///////////////////////////////////////////////////////////////////////////////
	// 2006-04-13 by cmkwon, 추가하기
	pInflWarInfoList->push_back(*i_pInflWarInfo);

	///////////////////////////////////////////////////////////////////////////////
	// 2006-04-13 by cmkwon, 전시단계 오름차순으로 정렬한다.
	sort(pInflWarInfoList->begin(), pInflWarInfoList->end(), sort_SDB_INFLUENCE_WAR_INFO_byWartimeStage());
	return TRUE;
}





///////////////////////////////////////////////////////////////////////////////
/// \fn			CInflWarData *CInflWarManager::GetInflWarDataByInflType(BYTE i_byInflTy)
/// \brief		
/// \author		cmkwon
/// \date		2006-04-13 ~ 2006-04-13
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
CInflWarData *CInflWarManager::GetInflWarDataByInflType(BYTE i_byInflTy)
{
	if(INFLUENCE_TYPE_VCN == i_byInflTy)
	{
		return &m_VCNInflWarData;
	}
	else if(INFLUENCE_TYPE_ANI == i_byInflTy)
	{
		return &m_ANIInflWarData;
	}

	return NULL;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL *CInflWarManager::GetINFLUENCE_WAR_INFO_ByInflType(SDB_INFLUENCE_WAR_INFO **o_ppInflWarInfo, BYTE i_byInflTy)
/// \brief		
/// \author		cmkwon
/// \date		2006-04-21 ~ 2006-04-21
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CInflWarManager::GetINFLUENCE_WAR_INFO_ByInflType(SDB_INFLUENCE_WAR_INFO **o_ppInflWarInfo, BYTE i_byInflTy)
{
	*o_ppInflWarInfo = NULL;

	CInflWarData *pInflWarData = GetInflWarDataByInflType(i_byInflTy);
	if(NULL == pInflWarData)
	{
		return FALSE;
	}

	SDB_INFLUENCE_WAR_INFO *pInflWarInfo = pInflWarData->GetCurInflWarInfo();
	if(NULL == pInflWarInfo)
	{
		return FALSE;
	}
	if(0 >= pInflWarInfo->HPRepairRate
		&& 0 >= pInflWarInfo->DPRepairRate
		&& 0 >= pInflWarInfo->SPRepairRate)
	{
		return FALSE;
	}

	*o_ppInflWarInfo = pInflWarInfo;
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CInflWarManager::InitInflWarData(SINFLUENCE_WAR_DATA *i_pInflWarData)
/// \brief		
/// \author		cmkwon
/// \date		2006-04-13 ~ 2006-04-13
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CInflWarManager::InitInflWarData(SINFLUENCE_WAR_DATA *i_pInflWarData)
{
	CInflWarData *pInflWarData = GetInflWarDataByInflType(i_pInflWarData->InfluenceType);
	if(NULL == pInflWarData)
	{
		return FALSE;
	}

	this->SetMSWarOptionTypeW(i_pInflWarData->InfluenceType, i_pInflWarData->MSWarOptionType);	// 2008-04-10 by dhjin, 모선전 정보 표시 기획안 - 

	pInflWarData->InitCInflWarData(i_pInflWarData, GetInflWarInfoByInflType(i_pInflWarData->InfluenceType));
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CInflWarManager::SetCurrentStepInflWarDataW()
/// \brief		선전 포고 - 현재 모선 단계로 m_pCurInflWarInfo를 설정한다.
/// \author		dhjin
/// \date		2009-01-14 ~ 2009-01-14
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CInflWarManager::SetCurrentStepInflWarDataW()
{
	CInflWarData *pBCUInflWarData = GetInflWarDataByInflType(INFLUENCE_TYPE_VCN);
	if(NULL == pBCUInflWarData)
	{
		return;
	}
	// 2009-03-10 by dhjin, 단계별 모선 시스템 - 밑과 같이 수정
//	pBCUInflWarData->SetCurrentStepInflWarData(this->m_DeclarationOfWar.GetCurrentMSWarStep(INFLUENCE_TYPE_VCN));
	pBCUInflWarData->SetCurrentStepInflWarData(this->m_DeclarationOfWar.GetCurrentMSUID(INFLUENCE_TYPE_VCN), INFLUENCE_TYPE_VCN);

	CInflWarData *pANIInflWarData = GetInflWarDataByInflType(INFLUENCE_TYPE_ANI);
	if(NULL == pANIInflWarData)
	{
		return;
	}
	// 2009-03-10 by dhjin, 단계별 모선 시스템 - 밑과 같이 수정
//	pANIInflWarData->SetCurrentStepInflWarData(this->m_DeclarationOfWar.GetCurrentMSWarStep(INFLUENCE_TYPE_ANI));
	pANIInflWarData->SetCurrentStepInflWarData(this->m_DeclarationOfWar.GetCurrentMSUID(INFLUENCE_TYPE_ANI), INFLUENCE_TYPE_ANI);

	SDB_INFLUENCE_WAR_INFO *pBCUCurInfoByWartimeStage = pBCUInflWarData->GetCurInflWarInfo();
	SDB_INFLUENCE_WAR_INFO *pANICurInfoByWartimeStage = pANIInflWarData->GetCurInflWarInfo();
	m_DeclarationOfWar.SetBeforeWarStartTime(pBCUCurInfoByWartimeStage->ReqContributionPoint, pANICurInfoByWartimeStage->ReqContributionPoint);

}

///////////////////////////////////////////////////////////////////////////////
/// \fn			int CInflWarManager::GetContributionPoint(BYTE i_byInflTy)
/// \brief		
/// \author		cmkwon
/// \date		2006-04-13 ~ 2006-04-13
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int CInflWarManager::GetContributionPoint(BYTE i_byInflTy)
{
	CInflWarData *pInflWarData = GetInflWarDataByInflType(i_byInflTy);
	if(NULL == pInflWarData)
	{
		return 0;
	}

	return pInflWarData->ContributionPoint;
}

//MS War Steps , when this count of NCP is reached, MS will spawn
int ncpMSPoints[] = {
	50000,
	100000,
	150000,
	200000,
	250000,
	300000,
	350000,
	400000,
	500000,
	550000,
	600000,
	650000,
	700000,
	750000,
	800000,
	850000,
	900000,
	950000
};

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CInflWarManager::AddContributionPoint(BYTE i_byInflTy, INT i_nAddValue)
/// \brief		
/// \author		cmkwon
/// \date		2006-04-13 ~ 2006-04-13
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CInflWarManager::AddContributionPoint(BYTE i_byInflTy, INT i_nAddValue)
{
	CInflWarData *pInflWarData = GetInflWarDataByInflType(i_byInflTy);
	if(NULL == pInflWarData)
	{
		return FALSE;
	}

//	char	szSysLog[1024];		// 2009-01-12 by dhjin, 선전 포고 - 모선 띄우는 방식 변경으로 주석처리
	
	mt_auto_lock mtA(pInflWarData->GetmtlockPtr());
	SDB_INFLUENCE_WAR_INFO *pCurInfoByWartimeStage = pInflWarData->GetCurInflWarInfo();
//////////////////////////////////////////////////////////////////////////
// 2009-01-12 by dhjin, 선전 포고 - 모선 띄우는 방식 변경으로 주석처리
//	SDB_INFLUENCE_WAR_INFO *pCurInfo = pInflWarData->GetInflWarInfoByContributionPoint(pInflWarData->ContributionPoint);
//	SDB_INFLUENCE_WAR_INFO *pNewInfo = pInflWarData->GetInflWarInfoByContributionPoint(pInflWarData->ContributionPoint + i_nAddValue);

	///////////////////////////////////////////////////////////////////////////////
	// 2006-04-13 by cmkwon, 기여도를 더한다.
	pInflWarData->ContributionPoint += i_nAddValue;
	int nContributionPoint = pInflWarData->ContributionPoint;

	mtA.auto_unlock_cancel();

	///////////////////////////////////////////////////////////////////////////////
	// 2006-04-19 by cmkwon, 도시맵 유저에게 기여도 전송
	INIT_MSG_WITH_BUFFER(MSG_FC_WAR_UPDATE_CONTRIBUTION_POINT_OK, T_FC_WAR_UPDATE_CONTRIBUTION_POINT_OK, pSConPoint, SendBuf);
	pSConPoint->byInfluenceType		= pCurInfoByWartimeStage->InfluenceType;
	pSConPoint->nContributionPoint	= nContributionPoint;
	m_pFieldIOCP14->SendMessageToCityClients(SendBuf, MSG_SIZE(MSG_FC_WAR_UPDATE_CONTRIBUTION_POINT_OK), pSConPoint->byInfluenceType);

	///////////////////////////////////////////////////////////////////////////////
	// 2006-04-14 by cmkwon, DB에 기여도 저장
	QPARAM_UPDATE_INFLUENCE_WAR_DATA *pS2DB = new QPARAM_UPDATE_INFLUENCE_WAR_DATA;
	pS2DB->InfluenceType0		= pCurInfoByWartimeStage->InfluenceType;
	//pS2DB->MGameServerID0		= g_pFieldGlobal->GetMGameServerID();
	pS2DB->WartimeStage0		= pCurInfoByWartimeStage->WartimeStage;
	pS2DB->ContributionPoint0	= nContributionPoint;
	m_pFieldIOCP14->m_pAtumDBManager->MakeAndEnqueueQuery(QT_UpdateInfluenceWarData, NULL, 0, pS2DB);

#ifdef _NCP_BASED_MSWAR
	for (int i = 0; i < sizeof(ncpMSPoints); i++)
	{
		if (ncpMSPoints[i] == nContributionPoint 
			|| (ncpMSPoints[i] < nContributionPoint && ncpMSPoints[i] > (nContributionPoint - i_nAddValue))) //Make sure even if more than 1 ncp is added, the MS will spawn
		{
			if (COMPARE_INFLUENCE(pCurInfoByWartimeStage->InfluenceType, INFLUENCE_TYPE_VCN))
			{
				char buf[512];
				sprintf(buf, "\\eBCU\\r reached \\m%d NCP\\r. \\cHoros\\r Mothership War will start soon.", ncpMSPoints[i]);
				m_pFieldIOCP14->SendWorldNotification(buf, TRUE);
			}
			else if (COMPARE_INFLUENCE(pCurInfoByWartimeStage->InfluenceType, INFLUENCE_TYPE_ANI))
			{
				char buf[512];
				sprintf(buf, "\\cANI\\r reached \\m%d NCP\\r. \\eAnubis\\r Mothership War will start soon.", ncpMSPoints[i]);
				m_pFieldIOCP14->SendWorldNotification(buf, TRUE);
			}
			MSWarStart(pCurInfoByWartimeStage->InfluenceType);
		}
		else if (nContributionPoint == (ncpMSPoints[i] - 5000) || ((ncpMSPoints[i] - 5000) < nContributionPoint && (ncpMSPoints[i] - 5000) > (nContributionPoint - i_nAddValue))
			|| nContributionPoint == (ncpMSPoints[i] - 1000) || ((ncpMSPoints[i] - 1000) < nContributionPoint && (ncpMSPoints[i] - 1000) > (nContributionPoint - i_nAddValue))
			|| nContributionPoint == (ncpMSPoints[i] - 100) || ((ncpMSPoints[i] - 100) < nContributionPoint && (ncpMSPoints[i] - 100) > (nContributionPoint - i_nAddValue)))
		{
			if (COMPARE_INFLUENCE(pCurInfoByWartimeStage->InfluenceType, INFLUENCE_TYPE_VCN))
			{
				char buf[512];
				sprintf(buf, "\\y[\\rWARNING\\y] Only \\m%d NCP\\y left until the launch of \\cHoros\\y!", (ncpMSPoints[i] - nContributionPoint));
				m_pFieldIOCP14->SendWorldNotification(buf, TRUE);
			}
			else if (COMPARE_INFLUENCE(pCurInfoByWartimeStage->InfluenceType, INFLUENCE_TYPE_ANI))
			{
				char buf[512];
				sprintf(buf, "\\y[\\rWARNING\\y] Only \\m%d NCP\\y left until the launch of \\eAnubis\\y!", (ncpMSPoints[i] - nContributionPoint));
				m_pFieldIOCP14->SendWorldNotification(buf, TRUE);
			}
		}
	}
#endif

//////////////////////////////////////////////////////////////////////////
// 2009-01-12 by dhjin, 선전 포고 - 모선 띄우는 방식 변경으로 주석처리
//	if(pCurInfo == pNewInfo)
//	{// 2006-04-14 by cmkwon, 세력전보스몬스터 혹은 자코몬스터 소환 없음
//		if(pCurInfo->IsSummonJacoMonster
//			&& FALSE == pInflWarData->IsSummonJacoMonster())
//		{
//
//			INIT_MSG(MSG_FN_NPCSERVER_SUMMON_JACO_MONSTER, T_FN_NPCSERVER_SUMMON_JACO_MONSTER, pSJacoMonster, SendBuf);
//			pSJacoMonster->Belligerence0	= GET_SAME_MONSTER_BELL_BY_CHARACTER_INFLTYPE(pNewInfo->InfluenceType);
//			pSJacoMonster->IsSummonJacoMonster	= TRUE;
//			// 2007-08-22 by cmkwon, 함수명 변경(<-Send2NPCServer)
//			//if(m_pFieldIOCP14->Send2NPCServer(SendBuf, MSG_SIZE(MSG_FN_NPCSERVER_SUMMON_JACO_MONSTER)))
//			if(m_pFieldIOCP14->Send2NPCServerByTCP(SendBuf, MSG_SIZE(MSG_FN_NPCSERVER_SUMMON_JACO_MONSTER)))
//			{// 2006-04-25 by cmkwon, NPCServer로 전송 성공시에 
//
//				pInflWarData->SetSummonJacoMonster(TRUE);
//
//				///////////////////////////////////////////////////////////////////////////////
//				// 2006-04-20 by cmkwon
//				INIT_MSG(MSG_FC_WAR_JACO_MONSTER_SUMMON, T_FC_WAR_JACO_MONSTER_SUMMON, pSJaco, SendBuf);
//				pSJaco->nBelligerence0			= GET_SAME_MONSTER_BELL_BY_CHARACTER_INFLTYPE(pNewInfo->InfluenceType);
//				m_pFieldIOCP14->SendMessageToAllClients(SendBuf, MSG_SIZE(MSG_FC_WAR_JACO_MONSTER_SUMMON), INFLUENCE_TYPE_VCN|INFLUENCE_TYPE_ANI);
//			}
//		}
//		return TRUE;
//	}
//
//	wsprintf(szSysLog, "  [Notify]: InflWar InflTy[%d] CurWartimeStage(%d) ChangeWartimeStage(%d) CurContributionPoint(%d) BossMonster(%8d) IsJacoSummon(%d)\r\n"
//		, pNewInfo->InfluenceType, pInflWarData->WartimeStage, pNewInfo->WartimeStage, nContributionPoint, pNewInfo->BossMonsterUnitKind, pNewInfo->IsSummonJacoMonster);
//	g_pFieldGlobal->WriteSystemLog(szSysLog);
//	DBGOUT(szSysLog);
//
//	///////////////////////////////////////////////////////////////////////////////
//	// 2006-04-18 by cmkwon, 세력전 보스 몬스터 소환 처리
//	if(0 != pNewInfo->BossMonsterUnitKind)
//	{		
//		MONSTER_INFO *pBossMon = m_pFieldIOCP14->GetMonsterInfo(pNewInfo->BossMonsterUnitKind);
//		if(NULL == pBossMon
//			|| FALSE == IS_MOTHERSHIPWAR_MONSTER(pBossMon->Belligerence)
////			|| FALSE == IS_INFLWAR_MONSTER(pBossMon->Belligerence)			// 2007-08-21 by dhjin, 모선전 관련 모선, 전략포인트만 구분 가능하게 변경
//			|| FALSE == COMPARE_MPOPTION_BIT(pBossMon->MPOption, MPOPTION_BIT_BOSS_MONSTER))
//		{
//			char szErr[1024];
//			wsprintf(szErr, "CInflWarManager::AddContributionPoint_ Error, MonsterUnitKind(%8d) Belligerence(%d)"
//				, pNewInfo->BossMonsterUnitKind, (NULL==pBossMon)?0:pBossMon->Belligerence);
//			g_pFieldGlobal->WriteSystemLog(szErr);
//			DbgOut("%s\rn", szErr);
//			return TRUE;
//		}
//		
//		///////////////////////////////////////////////////////////////////////////////
//		// 2006-04-14 by cmkwon, 세력전 보스 몬스터 소환 정보 추가
//		SINFLBOSS_MONSTER_SUMMON_DATA tmSummon;
//		tmSummon.pMonsterInfo		= pBossMon;
//		tmSummon.SummonCount		= pNewInfo->BossMonsterCount;
//		tmSummon.SummonTerm			= pNewInfo->BossMonsterSummonTerm;
//		tmSummon.atimeInsertedTime.SetCurrentDateTime(TRUE);
//		tmSummon.SummonMapIndex		= pNewInfo->BossMonsterSummonMapIndex;
//		tmSummon.SummonPosition.x	= pNewInfo->BossMonsterSummonPositionX;
//		tmSummon.SummonPosition.y	= pNewInfo->BossMonsterSummonPositionY;
//		tmSummon.SummonPosition.z	= pNewInfo->BossMonsterSummonPositionZ;
//		m_mtvectSummonMonsterDataList.pushBackLock(tmSummon);
//
//		///////////////////////////////////////////////////////////////////////////////
//		// 2006-04-14 by cmkwon, 세력전 보스 몬스터 소환 정보 클라이어트로 전송
//		INIT_MSG(MSG_FC_WAR_BOSS_MONSTER_SUMMON_DATA, T_FC_WAR_BOSS_MONSTER_SUMMON_DATA, pSWarData, SendBuf);
//		pSWarData->SummonMonsterUnitkind	= pBossMon->MonsterUnitKind;
//		pSWarData->RemainMinute				= pNewInfo->BossMonsterSummonTerm;
//		m_pFieldIOCP14->SendMessageToAllClients(SendBuf, MSG_SIZE(MSG_FC_WAR_BOSS_MONSTER_SUMMON_DATA), INFLUENCE_TYPE_VCN|INFLUENCE_TYPE_ANI);
//
//		//////////////////////////////////////////////////////////////////////////
//		// 2008-04-01 by dhjin, 모선전, 거점전 정보창 기획안 - 
//		this->SetMSWarInfoResultByWarStart(IS_VCN_INFLUENCE_TYPE(pNewInfo->InfluenceType) ? INFLUENCE_TYPE_VCN : INFLUENCE_TYPE_ANI, pNewInfo->BossMonsterUnitKind, pNewInfo->ReqContributionPoint);
//
//	}
//
//	///////////////////////////////////////////////////////////////////////////////
//	// 2006-04-18 by cmkwon, 자코몬스터 소환 처리
//	if(FALSE != pNewInfo->IsSummonJacoMonster)
//	{
//		INIT_MSG(MSG_FN_NPCSERVER_SUMMON_JACO_MONSTER, T_FN_NPCSERVER_SUMMON_JACO_MONSTER, pSJacoMonster, SendBuf);
//		pSJacoMonster->Belligerence0	= GET_SAME_MONSTER_BELL_BY_CHARACTER_INFLTYPE(pNewInfo->InfluenceType);
//		pSJacoMonster->IsSummonJacoMonster	= TRUE;
//		// 2007-08-22 by cmkwon, 함수명 변경(<-Send2NPCServer)
//		//if(m_pFieldIOCP14->Send2NPCServer(SendBuf, MSG_SIZE(MSG_FN_NPCSERVER_SUMMON_JACO_MONSTER)))
//		if(m_pFieldIOCP14->Send2NPCServerByTCP(SendBuf, MSG_SIZE(MSG_FN_NPCSERVER_SUMMON_JACO_MONSTER)))
//		{// 2006-04-25 by cmkwon, NPCServer로 전송 성공시에 
//
//			pInflWarData->SetSummonJacoMonster(TRUE);
//
//			///////////////////////////////////////////////////////////////////////////////
//			// 2006-04-20 by cmkwon
//			INIT_MSG(MSG_FC_WAR_JACO_MONSTER_SUMMON, T_FC_WAR_JACO_MONSTER_SUMMON, pSJaco, SendBuf);
//			pSJaco->nBelligerence0			= GET_SAME_MONSTER_BELL_BY_CHARACTER_INFLTYPE(pNewInfo->InfluenceType);
//			m_pFieldIOCP14->SendMessageToAllClients(SendBuf, MSG_SIZE(MSG_FC_WAR_JACO_MONSTER_SUMMON), INFLUENCE_TYPE_VCN|INFLUENCE_TYPE_ANI);
//		}
//	}
	return TRUE;
}

// 2013-05-09 by hskim, 세력 포인트 개선
BOOL CInflWarManager::CalcContributionPoint(BYTE i_byWinnerInflTy, INT i_nWinnerValue, INT i_nLoserValue)
{
#ifdef S_UPGRADE_INFLUENCE_POINT_HSKIM
	BYTE byApplyInfl = INFLUENCE_TYPE_VCN;		// BCU 버프 적용
	int CurrentDiffContributionPoint = 0;
	int nAbsDiff = 0;

	if( i_byWinnerInflTy != INFLUENCE_TYPE_VCN && i_byWinnerInflTy != INFLUENCE_TYPE_ANI )
	{
		return FALSE;
	}

	if( TRUE == COMPARE_INFLUENCE(i_byWinnerInflTy, INFLUENCE_TYPE_VCN) )
	{
		AddContributionPoint(INFLUENCE_TYPE_VCN, i_nWinnerValue);
		AddContributionPoint(INFLUENCE_TYPE_ANI, i_nLoserValue);
	}
	else
	{
		AddContributionPoint(INFLUENCE_TYPE_ANI, i_nWinnerValue);
		AddContributionPoint(INFLUENCE_TYPE_VCN, i_nLoserValue);
	}

	CurrentDiffContributionPoint = GetTurnAroundPoint();
	nAbsDiff = abs(CurrentDiffContributionPoint);

	if( CurrentDiffContributionPoint < 0 )
	{
		// ANI 버프 적용
		byApplyInfl = INFLUENCE_TYPE_ANI;
	}

	m_pFieldIOCP14->DeleteBuffSkillAllCharacter(INFLUENCE_TYPE_VCN, DES_SKILLTYPE_TRUN_AROUND);
	m_pFieldIOCP14->DeleteBuffSkillAllCharacter(INFLUENCE_TYPE_ANI, DES_SKILLTYPE_TRUN_AROUND);

	// 2013-08-01 by jhseol, 역전의 버프 리뉴얼
	if( nAbsDiff > TURN_AROUND_BUFF_SKILL_NEW_THRESHOLD )
	{
		m_pFieldIOCP14->InsertBuffSkillAllCharacter(byApplyInfl, TURN_AROUND_BUFF_SKILL_1ST);
	}
	// 기존코드 주석차리
// 	if( nAbsDiff >= TURN_AROUND_BUFF_SKILL_1ST_THRESHOLD )
// 	{
// 		if( nAbsDiff >= TURN_AROUND_BUFF_SKILL_2ND_THRESHOLD )
// 		{
// 			m_pFieldIOCP14->InsertBuffSkillAllCharacter(byApplyInfl, TURN_AROUND_BUFF_SKILL_2ND);
// 		}
// 		else
// 		{
// 			m_pFieldIOCP14->InsertBuffSkillAllCharacter(byApplyInfl, TURN_AROUND_BUFF_SKILL_1ST);
// 		}
// 	}
	// end 2013-08-01 by jhseol, 역전의 버프 리뉴얼

	g_pFieldGlobal->WriteSystemLogEX(TRUE, "[Notify] CalcContributionPoint :: WinnerInfluenceType [%d] WinnerContributionPoint [%d] LoserContributionPoint [%d]\r\n", 
		i_byWinnerInflTy, i_nWinnerValue, i_nLoserValue);
#endif

	return TRUE;
}

BOOL CInflWarManager::CalcConsecutiveVictoriesPoint(BYTE i_byWinnerInFlTy)
{
#ifdef S_UPGRADE_INFLUENCE_POINT_HSKIM
	// 에러 체크 
	if( i_byWinnerInFlTy != INFLUENCE_TYPE_VCN && i_byWinnerInFlTy != INFLUENCE_TYPE_ANI )
	{
		return FALSE;
	}	

	// 정보 갱신

	int ConsecutiveVictoriesBCU = 0;
	int ConsecutiveVictoriesANI = 0;

	int OldConsecutiveVictoriesBCU = 0;
	int OldConsecutiveVictoriesANI = 0;

	CInflWarData *pInflWarDataBCU = GetInflWarDataByInflType(INFLUENCE_TYPE_VCN);
	CInflWarData *pInflWarDataANI = GetInflWarDataByInflType(INFLUENCE_TYPE_ANI);

	if( NULL == pInflWarDataBCU || NULL == pInflWarDataANI )
	{
		return FALSE;
	}

	if( TRUE == COMPARE_INFLUENCE(i_byWinnerInFlTy, INFLUENCE_TYPE_VCN) )
	{
		mt_auto_lock mtA(pInflWarDataBCU->GetmtlockPtr());
		OldConsecutiveVictoriesBCU = pInflWarDataBCU->ConsecutiveVictories;
		pInflWarDataBCU->ConsecutiveVictories = min(pInflWarDataBCU->ConsecutiveVictories++, 8);
		ConsecutiveVictoriesBCU = pInflWarDataBCU->ConsecutiveVictories;
		mtA.auto_unlock_cancel();

		mt_auto_lock mtB(pInflWarDataANI->GetmtlockPtr());
		OldConsecutiveVictoriesANI = pInflWarDataANI->ConsecutiveVictories;
		pInflWarDataANI->ConsecutiveVictories = 0;
		ConsecutiveVictoriesANI = pInflWarDataANI->ConsecutiveVictories;
		mtB.auto_unlock_cancel();
	}

	if( TRUE == COMPARE_INFLUENCE(i_byWinnerInFlTy, INFLUENCE_TYPE_ANI) )
	{
		mt_auto_lock mtA(pInflWarDataBCU->GetmtlockPtr());
		OldConsecutiveVictoriesBCU = pInflWarDataBCU->ConsecutiveVictories;
		pInflWarDataBCU->ConsecutiveVictories = 0;
		ConsecutiveVictoriesBCU = pInflWarDataBCU->ConsecutiveVictories;
		mtA.auto_unlock_cancel();

		mt_auto_lock mtB(pInflWarDataANI->GetmtlockPtr());
		OldConsecutiveVictoriesANI = pInflWarDataANI->ConsecutiveVictories;
		pInflWarDataANI->ConsecutiveVictories = min(pInflWarDataANI->ConsecutiveVictories++, 8);
		ConsecutiveVictoriesANI = pInflWarDataANI->ConsecutiveVictories;
		mtB.auto_unlock_cancel();
	}

	// 액션 수행

	if( 0 != OldConsecutiveVictoriesBCU && 0 == ConsecutiveVictoriesBCU )
	{
		//BCU 버프 지우기
		m_pFieldIOCP14->DeleteBuffSkillAllCharacter(INFLUENCE_TYPE_VCN, DES_SKILLTYPE_CONSECUTIVE_VICTORIES);
	}

	if( 0 != OldConsecutiveVictoriesANI && 0 == ConsecutiveVictoriesANI )
	{
		//ANI 버프 지우기
		m_pFieldIOCP14->DeleteBuffSkillAllCharacter(INFLUENCE_TYPE_ANI, DES_SKILLTYPE_CONSECUTIVE_VICTORIES);
	}

	INIT_MSG_WITH_BUFFER(MSG_FC_WAR_INFLUENCE_CONSECUTIVE_VICTORITES, T_FC_WAR_INFLUENCE_CONSECUTIVE_VICTORITES, pInflVictory, pInflVictorySendBuf);
	pInflVictory->PointBCU	= ConsecutiveVictoriesBCU;
	pInflVictory->PointANI	= ConsecutiveVictoriesANI;
	m_pFieldIOCP14->SendMessageToAllClients(pInflVictorySendBuf, MSG_SIZE(MSG_FC_WAR_INFLUENCE_CONSECUTIVE_VICTORITES),INFLUENCE_TYPE_VCN|INFLUENCE_TYPE_ANI);

	// DB 저장

	QPARAM_UPDATE_INFLUENCE_CONSECUTIVE_VICTORIES *pBCUDB = new QPARAM_UPDATE_INFLUENCE_CONSECUTIVE_VICTORIES;
	pBCUDB->InfluenceType0		= INFLUENCE_TYPE_VCN;
	pBCUDB->MGameServerID0		= g_pFieldGlobal->GetMGameServerID();
	pBCUDB->ConsecutiveVictories		= ConsecutiveVictoriesBCU;
	m_pFieldIOCP14->m_pAtumDBManager->MakeAndEnqueueQuery(QT_UpdateInfluenceConsecutiveVictorites, NULL, 0, pBCUDB);

	QPARAM_UPDATE_INFLUENCE_CONSECUTIVE_VICTORIES *pANIDB = new QPARAM_UPDATE_INFLUENCE_CONSECUTIVE_VICTORIES;
	pANIDB->InfluenceType0		= INFLUENCE_TYPE_ANI;
	pANIDB->MGameServerID0		= g_pFieldGlobal->GetMGameServerID();
	pANIDB->ConsecutiveVictories		= ConsecutiveVictoriesANI;
	m_pFieldIOCP14->m_pAtumDBManager->MakeAndEnqueueQuery(QT_UpdateInfluenceConsecutiveVictorites, NULL, 0, pANIDB);

	g_pFieldGlobal->WriteSystemLogEX(TRUE, "[Notify] CalcConsecutiveVictoriesPoint :: WinnerInfluenceType [%d] ConsecutiveVictoriesBCU [%d] ConsecutiveVictoriesANI [%d]\r\n", 
		i_byWinnerInFlTy, ConsecutiveVictoriesBCU, ConsecutiveVictoriesANI);
#endif

	return TRUE;
}

int CInflWarManager::GetTurnAroundPoint()
{
#ifdef S_UPGRADE_INFLUENCE_POINT_HSKIM
	int CurrentContributionPointBCU = GetContributionPoint(INFLUENCE_TYPE_VCN);
	int CurrentContributionPointANI = GetContributionPoint(INFLUENCE_TYPE_ANI);

	return (CurrentContributionPointANI - CurrentContributionPointBCU);
#else
	return 0;
#endif
}

INT	CInflWarManager::GetConsecutiveVictoriesPoint(BYTE i_byWinnerInFlTy)
{
#ifdef S_UPGRADE_INFLUENCE_POINT_HSKIM
	int nPoint = 0;
	CInflWarData *pInflWarData = GetInflWarDataByInflType(i_byWinnerInFlTy);

	if( NULL == pInflWarData )
	{
		return 0;
	}

	mt_auto_lock mtA(pInflWarData->GetmtlockPtr());
	nPoint = pInflWarData->ConsecutiveVictories;
	mtA.auto_unlock_cancel();

	return nPoint;
#else
	return 0;
#endif
}
// end 2013-05-09 by hskim, 세력 포인트 개선
// 2013-08-01 by jhseol, 역전의 버프 리뉴얼
float CInflWarManager::GetPVPBuffPercent(int i_CurrentDiffContributionPoint)
{
	int nAbsDiff = abs(i_CurrentDiffContributionPoint);
	nAbsDiff -= TURN_AROUND_BUFF_SKILL_NEW_THRESHOLD;
	if ( 0 > nAbsDiff )
	{
		return 0.0f;
	}
	else
	{
		return (float)(min(nAbsDiff, TURN_AROUND_BUFF_SKILL_MAXIMUM_POINT_GAP));
	}
}
// end 2013-08-01 by jhseol, 역전의 버프 리뉴얼

///////////////////////////////////////////////////////////////////////////////
/// \fn			float CInflWarManager::GetHPRepairAddRateByInflTy(BYTE i_byInflTy)
/// \brief		
/// \author		cmkwon
/// \date		2006-04-20 ~ 2006-04-20
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
float CInflWarManager::GetHPRepairAddRateByInflTy(BYTE i_byInflTy)
{
	CInflWarData *pInflWarData = GetInflWarDataByInflType(i_byInflTy);
	if(NULL == pInflWarData)
	{
		return 0.0f;
	}
// 2009-03-31 by dhjin, 선전 포고 - 세력전 승리 시 지급 되는 이벤트 수정 - 밑과 같이 수정 
//	return pInflWarData->GetCurInflWarInfo()->HPRepairRate;
	return pInflWarData->fInflHPRepairRate;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			float CInflWarManager::GetDPRepairAddRateByInflTy(BYTE i_byInflTy)
/// \brief		
/// \author		cmkwon
/// \date		2006-04-20 ~ 2006-04-20
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
float CInflWarManager::GetDPRepairAddRateByInflTy(BYTE i_byInflTy)
{
	CInflWarData *pInflWarData = GetInflWarDataByInflType(i_byInflTy);
	if(NULL == pInflWarData)
	{
		return 0.0f;
	}

// 2009-03-31 by dhjin, 선전 포고 - 세력전 승리 시 지급 되는 이벤트 수정 - 밑과 같이 수정 
//	return pInflWarData->GetCurInflWarInfo()->DPRepairRate;
	return pInflWarData->fInflDPRepairRate;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			float CInflWarManager::GetSPRepairAddRateByInflTy(BYTE i_byInflTy)
/// \brief		
/// \author		cmkwon
/// \date		2006-04-20 ~ 2006-04-20
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
float CInflWarManager::GetSPRepairAddRateByInflTy(BYTE i_byInflTy)
{
	CInflWarData *pInflWarData = GetInflWarDataByInflType(i_byInflTy);
	if(NULL == pInflWarData)
	{
		return 0.0f;
	}

// 2009-03-31 by dhjin, 선전 포고 - 세력전 승리 시 지급 되는 이벤트 수정 - 밑과 같이 수정 
//	return pInflWarData->GetCurInflWarInfo()->SPRepairRate;
	return pInflWarData->fInflSPRepairRate;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			void CInflWarManager::GetAllLeaderFISocket(BYTE i_byInflTy, CFieldIOCPSocket **i_ppLeaderFISoc, CFieldIOCPSocket **i_ppSubLeader1FISoc, CFieldIOCPSocket **i_ppSubLeader2FISoc)
/// \brief		
/// \author		cmkwon
/// \date		2006-04-20 ~ 2006-04-20
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CInflWarManager::GetAllLeaderFISocket(BYTE i_byInflTy, CFieldIOCPSocket **i_ppLeaderFISoc, CFieldIOCPSocket **i_ppSubLeader1FISoc, CFieldIOCPSocket **i_ppSubLeader2FISoc)
{
	*i_ppLeaderFISoc		= NULL;
	*i_ppSubLeader1FISoc	= NULL;
	*i_ppSubLeader2FISoc	= NULL;

	CInflWarData *pInflWarData = GetInflWarDataByInflType(i_byInflTy);
	if(NULL == pInflWarData)
	{
		return;
	}

	*i_ppLeaderFISoc		= m_pFieldIOCP14->GetFieldIOCPSocketByCharacterUniqueNumber(pInflWarData->InflLeaderCharacterUID);
	*i_ppSubLeader1FISoc	= m_pFieldIOCP14->GetFieldIOCPSocketByCharacterUniqueNumber(pInflWarData->InflSub1LeaderCharacterUID);
	*i_ppSubLeader2FISoc	= m_pFieldIOCP14->GetFieldIOCPSocketByCharacterUniqueNumber(pInflWarData->InflSub2LeaderCharacterUID);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CInflWarManager::IsLeader(BYTE i_byInflTy, UID32_t i_charUID)
/// \brief		// 2007-10-06 by cmkwon, 부지도자 2명의 호칭을 다르게 설정 - 인자 제거 (, BOOL i_bSubLeader/*=FALSE*/)
/// \author		dhjin
/// \date		2006-12-08 ~ 2006-12-08
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CInflWarManager::IsLeader(BYTE i_byInflTy, UID32_t i_charUID)
{
	CInflWarData *pInflWarData = GetInflWarDataByInflType(i_byInflTy);
	if(NULL == pInflWarData)
	{
		return FALSE;
	}

	return pInflWarData->InflLeaderCharacterUID == i_charUID;
// 2007-10-06 by cmkwon, 부지도자 2명의 호칭을 다르게 설정 - 위와 같이 리더만 체크
//	if(FALSE == i_bSubLeader)
//	{
//		return pInflWarData->InflLeaderCharacterUID == i_charUID;
//	}
//
//	if(i_charUID == pInflWarData->InflSub1LeaderCharacterUID
//		|| i_charUID == pInflWarData->InflSub2LeaderCharacterUID)
//	{
//		return TRUE;
//	}
//	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CInflWarManager::IsSubLeader1(BYTE i_byInflTy, UID32_t i_charUID)
/// \brief		// 2007-10-06 by cmkwon, 부지도자 2명의 호칭을 다르게 설정 -
/// \author		cmkwon
/// \date		2007-10-06 ~ 2007-10-06
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CInflWarManager::IsSubLeader1(BYTE i_byInflTy, UID32_t i_charUID)
{
	CInflWarData *pInflWarData = GetInflWarDataByInflType(i_byInflTy);
	if(NULL == pInflWarData)
	{
		return FALSE;
	}

	return i_charUID == pInflWarData->InflSub1LeaderCharacterUID;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CInflWarManager::IsSubLeader2(BYTE i_byInflTy, UID32_t i_charUID)
/// \brief		// 2007-10-06 by cmkwon, 부지도자 2명의 호칭을 다르게 설정 -
/// \author		cmkwon
/// \date		2007-10-06 ~ 2007-10-06
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CInflWarManager::IsSubLeader2(BYTE i_byInflTy, UID32_t i_charUID)
{
	CInflWarData *pInflWarData = GetInflWarDataByInflType(i_byInflTy);
	if(NULL == pInflWarData)
	{
		return FALSE;
	}

	return i_charUID == pInflWarData->InflSub2LeaderCharacterUID;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CInflWarManager::SetLeader8SubLeaderRACE(CHARACTER *io_pCharacter)
/// \brief		// 2007-10-06 by cmkwon, 부지도자 2명의 호칭을 다르게 설정 -
/// \author		cmkwon
/// \date		2007-10-06 ~ 2007-10-06
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CInflWarManager::SetLeader8SubLeaderRACE(CHARACTER *io_pCharacter)
{
	if(INFLUENCE_TYPE_NORMAL == io_pCharacter->InfluenceType)
	{// 2007-10-06 by cmkwon, 일반 세력은 체크 필요 없다
		return FALSE;
	}

	if(this->IsLeader(io_pCharacter->InfluenceType, io_pCharacter->CharacterUniqueNumber))
	{
		io_pCharacter->Race	|= RACE_INFLUENCE_LEADER;
		return TRUE;
	}

	if(this->IsSubLeader1(io_pCharacter->InfluenceType, io_pCharacter->CharacterUniqueNumber))
	{
		io_pCharacter->Race	|= RACE_INFLUENCE_SUBLEADER_1;
		return TRUE;
	}

	if(this->IsSubLeader2(io_pCharacter->InfluenceType, io_pCharacter->CharacterUniqueNumber))
	{
		io_pCharacter->Race	|= RACE_INFLUENCE_SUBLEADER_2;
		return TRUE;
	}

	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CInflWarManager::SetSummonJacoMonsterW(BYTE i_byInflTy, BOOL i_bSummon)
/// \brief		
/// \author		cmkwon
/// \date		2006-04-20 ~ 2006-04-20
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CInflWarManager::SetSummonJacoMonsterW(BYTE i_byInflTy, BOOL i_bSummon)
{
	CInflWarData *pInflWarData = GetInflWarDataByInflType(i_byInflTy);
	if(NULL == pInflWarData)
	{
		return;
	}
	pInflWarData->SetSummonJacoMonster(i_bSummon);
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CInflWarManager::GetSummonInflBossMonsterData(MSG_FC_WAR_BOSS_MONSTER_SUMMON_DATA *o_pSummonData, BYTE i_byInflTy)
/// \brief		
/// \author		cmkwon
/// \date		2006-04-20 ~ 2006-04-20
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CInflWarManager::GetSummonInflBossMonsterData(MSG_FC_WAR_BOSS_MONSTER_SUMMON_DATA *o_pSummonData, BYTE i_byInflTy)
{
	if(FALSE == COMPARE_INFLUENCE(i_byInflTy, INFLUENCE_TYPE_VCN|INFLUENCE_TYPE_ANI))
	{
		return FALSE;
	}

	if(m_mtvectSummonMonsterDataList.empty())
	{
		return FALSE;
	}

	int				nMonsterBell = GET_SAME_MONSTER_BELL_BY_CHARACTER_INFLTYPE(i_byInflTy);
	ATUM_DATE_TIME	atumCur(true);
	mt_auto_lock mtA(&m_mtvectSummonMonsterDataList);

	mtvectSummonMonsterData::iterator itr(m_mtvectSummonMonsterDataList.begin());
	while(itr != m_mtvectSummonMonsterDataList.end())
	{
		SINFLBOSS_MONSTER_SUMMON_DATA *pSummonData = &*itr;
		if(nMonsterBell == pSummonData->pMonsterInfo->Belligerence)
		{
			o_pSummonData->SummonMonsterUnitkind	= pSummonData->pMonsterInfo->MonsterUnitKind;
			o_pSummonData->RemainMinute				= pSummonData->SummonTerm - atumCur.GetTimeDiffTimeInMinutes(pSummonData->atimeInsertedTime);
			o_pSummonData->BossStep					= pSummonData->BossStep;			// 2009-03-10 by dhjin, 단계별 모선 시스템 - 모선 단계
			o_pSummonData->BeforeWinCheck			= pSummonData->BeforeWinCheck;				// 2009-03-10 by dhjin, 단계별 모선 시스템 - 전 단계 승패 여부
			return TRUE;
		}
		itr++;
	}

	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CInflWarManager::GetVCNSummonedInflBossMonsterData(MSG_FC_WAR_BOSS_MONSTER_SUMMON_DATA *o_pSummonData)
/// \brief		
/// \author		dhjin
/// \date		2007-02-06 ~ 2007-02-06
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CInflWarManager::GetVCNSummonedInflBossMonsterData(MSG_FC_WAR_BOSS_MONSTER_SUMMON_DATA *o_pSummonData)
{
	//////////////////////////////////////////////////////////////////////////
	// 2007-02-06 by dhjin, 전함이 소환되고 난 뒤 접속하는 유저에게 정보를 보여주기 위해 추가
	if (TRUE == m_SummonVCNBossMonsterInfo.bSummonBossMonster)
	{
		o_pSummonData->SummonMonsterUnitkind	= m_SummonVCNBossMonsterInfo.SummonMonsterUnitkind;
		o_pSummonData->RemainMinute				= 0;
		o_pSummonData->SummonMonsterTime		= m_SummonVCNBossMonsterInfo.SummonBossTime;
		o_pSummonData->ContributionPoint		= m_SummonVCNBossMonsterInfo.ContributionPoint;			// 2008-04-22 by dhjin, 모선전, 거점전 정보창 기획안 - 세력포인트
		o_pSummonData->BossStep					= m_SummonVCNBossMonsterInfo.BossStep;				// 2009-03-10 by dhjin, 단계별 모선 시스템 - 모선 단계
		o_pSummonData->BeforeWinCheck			= m_SummonVCNBossMonsterInfo.BeforeWinCheck;				// 2009-03-10 by dhjin, 단계별 모선 시스템 - 전 단계 승패 여부
		return TRUE;
	}
	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CInflWarManager::GetANISummonedInflBossMonsterData(MSG_FC_WAR_BOSS_MONSTER_SUMMON_DATA *o_pSummonData)
/// \brief		
/// \author		dhjin
/// \date		2007-02-06 ~ 2007-02-06
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CInflWarManager::GetANISummonedInflBossMonsterData(MSG_FC_WAR_BOSS_MONSTER_SUMMON_DATA *o_pSummonData)
{
	//////////////////////////////////////////////////////////////////////////
	// 2007-02-06 by dhjin, 전함이 소환되고 난 뒤 접속하는 유저에게 정보를 보여주기 위해 추가
	if (TRUE == m_SummonANIBossMonsterInfo.bSummonBossMonster)
	{
		o_pSummonData->SummonMonsterUnitkind	= m_SummonANIBossMonsterInfo.SummonMonsterUnitkind;
		o_pSummonData->RemainMinute				= 0;
		o_pSummonData->SummonMonsterTime		= m_SummonANIBossMonsterInfo.SummonBossTime;
		o_pSummonData->ContributionPoint		= m_SummonANIBossMonsterInfo.ContributionPoint;		// 2008-04-22 by dhjin, 모선전, 거점전 정보창 기획안 - 세력포인트
		o_pSummonData->BossStep					= m_SummonANIBossMonsterInfo.BossStep;				// 2009-03-10 by dhjin, 단계별 모선 시스템 - 모선 단계
		o_pSummonData->BeforeWinCheck			= m_SummonANIBossMonsterInfo.BeforeWinCheck;				// 2009-03-10 by dhjin, 단계별 모선 시스템 - 전 단계 승패 여부
		return TRUE;
	}
	return FALSE;
}

// 2007-05-17 by cmkwon, 함수 수정
/////////////////////////////////////////////////////////////////////////////////
///// \fn			void CInflWarManager::OnInfluenceBossMonsterAutoDestroyedInflWarManager(BOOL i_bVCNInflTyForInit)
///// \brief		
///// \author		dhjin
///// \date		2007-02-06 ~ 2007-02-06
///// \warning	
/////
///// \param		
///// \return		
/////////////////////////////////////////////////////////////////////////////////
//void CInflWarManager::OnInfluenceBossMonsterAutoDestroyedInflWarManager(BOOL i_bVCNInflTyForInit)
//{
//	if(TRUE == i_bVCNInflTyForInit)
//	{
//		util::zero(&m_SummonVCNBossMonsterInfo, sizeof(SSUMMONBOSSMONSTER_INFO));
//	}
//	else
//	{
//		util::zero(&m_SummonANIBossMonsterInfo, sizeof(SSUMMONBOSSMONSTER_INFO));
//	}
//}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CInflWarManager::SetSubLeader(BYTE i_byInflTy, UID32_t i_CharacterUID, CHAR * i_pCharacterName, BYTE SubLeaderNum)
/// \brief		
/// \author		dhjin
/// \date		2007-02-14 ~ 2007-02-14
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CInflWarManager::SetSubLeader(BYTE i_byInflTy, UID32_t i_CharacterUID, CHAR * i_pCharacterName, BYTE SubLeaderNum)
{
	CInflWarData *pInflWarData = GetInflWarDataByInflType(i_byInflTy);
	if(NULL == pInflWarData)
	{
		return FALSE;
	}

	mt_auto_lock mtA(pInflWarData->GetmtlockPtr());
	if(1 == SubLeaderNum)
	{
		pInflWarData->InflSub1LeaderCharacterUID = i_CharacterUID;
		util::strncpy(pInflWarData->InflSubLeader1CharacterName, i_pCharacterName, SIZE_MAX_CHARACTER_NAME);	// 2008-01-03 by dhjin, 전장 정보에 부지도자 안보이는 버그로 수정
	}
	else if(2 == SubLeaderNum)
	{
		pInflWarData->InflSub2LeaderCharacterUID = i_CharacterUID;
		util::strncpy(pInflWarData->InflSubLeader2CharacterName, i_pCharacterName, SIZE_MAX_CHARACTER_NAME);	// 2008-01-03 by dhjin, 전장 정보에 부지도자 안보이는 버그로 수정
	}
	else 
	{
		return FALSE;
	}

	return TRUE;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			void CInflWarManager::InsertSummonStrategyPointInfo(SSUMMON_STRATEGYPOINT_INFO *i_pSSUMMON_STRATEGYPOINT_INFO)
/// \brief		
/// \author		dhjin
/// \date		2007-02-27 ~ 2007-02-27
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CInflWarManager::InsertSummonStrategyPointInfo(SSUMMON_STRATEGYPOINT_INFO *i_pSSUMMON_STRATEGYPOINT_INFO)
{
	m_mtvectSummonStrategyPointInfo.pushBackLock(*i_pSSUMMON_STRATEGYPOINT_INFO);
	if (!CheckSummonBoss())
	{
		m_StrategicPointInfoDisplay.SetStrategicPointDisplay(i_pSSUMMON_STRATEGYPOINT_INFO);
		SendStrategicPointInfoForAllUser(i_pSSUMMON_STRATEGYPOINT_INFO->MapIndex);
	}
	SetDoingInfluenceWar();	// 2012-11-26 by jhseol, 전쟁 시스템 리뉴얼 - 전쟁시 트리거 무적상태(일시정지), 지금이 전쟁중인지 확인하는 함수.(거점전 체크가 목적)
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			void CInflWarManager::DeleteSummonStrategyPointInfo(MAP_CHANNEL_INDEX	i_MapChannel)
/// \brief		
/// \author		dhjin
/// \date		2007-02-27 ~ 2007-02-27
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CInflWarManager::DeleteSummonStrategyPointInfo(MAP_CHANNEL_INDEX	i_MapChannel)
{
	mt_auto_lock mtA(&m_mtvectSummonStrategyPointInfo);

	mtvectorSSUMMON_STRATEGYPOINT_INFO::iterator	itr = m_mtvectSummonStrategyPointInfo.begin();
	while(itr != m_mtvectSummonStrategyPointInfo.end())
	{
		if(i_MapChannel.MapIndex == itr->MapIndex)
		{
			m_mtvectSummonStrategyPointInfo.erase(itr);
			SetDoingInfluenceWar();	// 2012-11-26 by jhseol, 전쟁 시스템 리뉴얼 - 전쟁시 트리거 무적상태(일시정지), 전쟁 종료를 실시간으로 확인 하기 위해 함수호출(거점전 종료)
			break;
		}
		itr++;
	}

	m_StrategicPointInfoDisplay.RemoveStrategicPointDisplay(i_MapChannel.MapIndex);
	SendStrategicPointInfoForAllUser(i_MapChannel.MapIndex);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CInflWarManager::DeleteSummonStrategyPointInfoByInfluenceType(BYTE i_InfluenceType)
/// \brief		
/// \author		dhjin
/// \date		2007-02-27 ~ 2007-02-27
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CInflWarManager::DeleteSummonStrategyPointInfoByInfluenceType(BYTE i_InfluenceType)
{
	mt_auto_lock mtA(&m_mtvectSummonStrategyPointInfo);
	
	mtvectorSSUMMON_STRATEGYPOINT_INFO::iterator	itr = m_mtvectSummonStrategyPointInfo.begin();
	while(itr != m_mtvectSummonStrategyPointInfo.end())
	{
		if(i_InfluenceType == itr->InfluenceType)
		{
			m_mtvectSummonStrategyPointInfo.erase(itr);
			SetDoingInfluenceWar();	// 2012-11-26 by jhseol, 전쟁 시스템 리뉴얼 - 전쟁시 트리거 무적상태(일시정지), 전쟁 종료를 실시간으로 확인 하기 위해 함수호출(거점전 종료)
			continue;
		}
		itr++;
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CInflWarManager::CheckSummonStrategyPointInfoByMapIndex(INT i_nMapIndex)
/// \brief		맵이 전략포인트와 연관있는지 체크
/// \author		dhjin
/// \date		2007-10-06 ~ 2007-10-06
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CInflWarManager::CheckSummonStrategyPointInfoByMapIndex(INT i_nMapIndex)
{
	mt_auto_lock mtA(&m_mtvectSummonStrategyPointInfo);
	
	mtvectorSSUMMON_STRATEGYPOINT_INFO::iterator	itr = m_mtvectSummonStrategyPointInfo.begin();
	while(itr != m_mtvectSummonStrategyPointInfo.end())
	{
		if(i_nMapIndex == itr->MapIndex)
		{
			return TRUE;			
		}
		
		CFieldMapProject *pCurFMPro = m_pFieldIOCP14->GetFieldMapProjectByMapIndex(itr->MapIndex);
		if(pCurFMPro)
		{
			// 2008-06-20 by cmkwon, EP3 백맵 시스템 수정(맵당 백맵을 2개 설정 할 수 있다) - 아래와 같이 수정
			//if(i_nMapIndex == pCurFMPro->m_nBeforeMapIndex)
			if(i_nMapIndex == pCurFMPro->m_nBeforeMapIndex
				|| i_nMapIndex == pCurFMPro->m_nBeforeMapIndex2)	// 2008-06-20 by cmkwon, EP3 백맵 시스템 수정(맵당 백맵을 2개 설정 할 수 있다) - 
			{
				return TRUE;
			}
		}
		itr++;
	}

	return FALSE;
}

void CInflWarManager::UpdateSummonStrategicPointInfoHP(MapIndex_t i_nMapindex, FLOAT i_nHPRate)
{
	m_StrategicPointInfoDisplay.SetStrategicPointDisplayHP(i_nMapindex, i_nHPRate);
	SendStrategicPointInfoForAllUser(i_nMapindex);
}

void CInflWarManager::GetAllSummonStrategicPointInfo(vector<SSTRATEGYPOINT_DISPLAY_INFO>* o_VectStrategicPointDisplay)
{
	m_StrategicPointInfoDisplay.GetAllStrategicPointDisplay(o_VectStrategicPointDisplay);
}

mtvectSSTRATEGYPOINT_SUMMONTIME_INFO * CInflWarManager::GetStrategyPointSummonTimeInfo(void)	
{
	return &m_mtvectStrategyPointSummonTimeInfo;
}

SSTRATEGYPOINT_SUMMON_RANGE * CInflWarManager::GetStrategyPointSummonRange(void)
{
	return &m_StrategyPointSummonRange;
}

mtvectSSTRATEGYPOINT_NOT_SUMMON_TIME * CInflWarManager::GetStrategyPointNotSummonTime(void)
{
	return &m_mtvectStrategyPointNotSummonTime;
}

void CInflWarManager::InitStrategyPointSummonTimeInfo(vectSSTRATEGYPOINT_SUMMONTIME_INFO *i_pvectSStrategypointSummonTimeInfo)
{
	mt_auto_lock mta(this->GetStrategyPointSummonTimeInfo());

	m_mtvectStrategyPointSummonTimeInfo.clear();

	m_mtvectStrategyPointSummonTimeInfo.assign(i_pvectSStrategypointSummonTimeInfo->begin(), i_pvectSStrategypointSummonTimeInfo->end());
}

//////////////////////////////////////////////////////////////////////////
// 2012-11-13 by jhseol, 전쟁 시스템 리뉴얼 - 거점전
void CInflWarManager::InitRenewalStrategyPointSummonTime(vectSRENEWAL_STRATEGYPOINT_SUMMON_TIME *i_pvectRenewalSStrategypointSummonTime)
{
	mt_auto_lock mta(this->GetRenewalStrategyPointSummonTime());
	
	m_mtvectRenewalStrategyPointSummonTime.clear();
	
	m_mtvectRenewalStrategyPointSummonTime.assign(i_pvectRenewalSStrategypointSummonTime->begin(), i_pvectRenewalSStrategypointSummonTime->end());
}

mtvectSRENEWAL_STRATEGYPOINT_SUMMON_TIME * CInflWarManager::GetRenewalStrategyPointSummonTime(void)	
{
	return &m_mtvectRenewalStrategyPointSummonTime;
}

void CInflWarManager::InitRenewalStrategyPointSummonMapInfo(vectSSTRATEGYPOINT_MAP_INFLUENCESTRATEGYPOINT_INFO *i_pvectStrategyPointMapInfluenceInfo)
{
	mt_auto_lock mta(this->GetRenewalStrategyPointSummonMapInfo());
	
	m_mtvectStrategyPointMapInfluenceInfo.clear();
	
	m_mtvectStrategyPointMapInfluenceInfo.assign(i_pvectStrategyPointMapInfluenceInfo->begin(), i_pvectStrategyPointMapInfluenceInfo->end());
}

mtvectSSTRATEGYPOINT_MAP_INFLUENCESTRATEGYPOINT_INFO * CInflWarManager::GetRenewalStrategyPointSummonMapInfo()
{
	return &m_mtvectStrategyPointMapInfluenceInfo;
}
// end 2012-11-13 by jhseol, 전쟁 시스템 리뉴얼 - 거점전

// 2012-11-26 by jhseol, 전쟁 시스템 리뉴얼 - 전쟁시 트리거 무적상태(일시정지), 지금이 전쟁중인지 확인하는 함수
void CInflWarManager::SetDoingInfluenceWar()
{
	if (S_WAR_SYSTEM_RENEWAL_STRATEGYPOINT_JHSEOL)
	{
		BOOL IsDoingStrategyPointWar	= FALSE;
		BOOL IsDoingOutPostWar			= FALSE;
		BOOL IsDoingBossMonsterWar		= FALSE;

		if( !m_mtvectSummonStrategyPointInfo.empty() )	// 거점이 소환된 맵이 있다면
		{
			IsDoingStrategyPointWar = TRUE;
		}
		
		if ( NULL != m_pFieldIOCP14 )
		{
			if(	TRUE == m_pFieldIOCP14->m_OutPostManager.CheckALLOutPostWaring() )	// 전진기지전 상태 확인
			{
				IsDoingOutPostWar = TRUE;
			}
		}
			
		if (TRUE == m_SummonVCNBossMonsterInfo.bSummonBossMonster || TRUE == m_SummonANIBossMonsterInfo.bSummonBossMonster )	// 모선전 확인
		{
			IsDoingBossMonsterWar = TRUE;
		}

		if( IsDoingStrategyPointWar || IsDoingOutPostWar || IsDoingBossMonsterWar )
		{
			if (!m_bIsDoingInfluenceWarCheck)
				OnInfluenceWarStart();

			m_bIsDoingInfluenceWarCheck = TRUE;
		}
		else
		{
			if (m_bIsDoingInfluenceWarCheck)
				
				OnInfluenceWarEnd();

			m_bIsDoingInfluenceWarCheck = FALSE;
		}

		server::log(false, "  [Notify] S_WAR_SYSTEM_RENEWAL_STRATEGYPOINT_JHSEOL - CInflWarManager::SetDoingInfluenceWar, IsDoingStrategyPointWar(%d), IsDoingOutPostWar(%d), IsDoingBossMonsterWar(%d) = m_bIsDoingInfluenceWarCheck(%d)\r\n"
			, IsDoingStrategyPointWar, IsDoingOutPostWar, IsDoingBossMonsterWar, m_bIsDoingInfluenceWarCheck);

		// end 2013-03-25 by jhseol, 비전쟁시 트리거 이용 불가능 현상관련 추가로그
	}
}

#ifdef MS_TAKE_NO_DAMAGE 
BOOL CInflWarManager::IsDoingStrategyPointWar()
{
	if (TRUE != m_mtvectSummonStrategyPointInfo.empty())	// 거점이 소환된 맵이 있다면
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}
#endif

BOOL CInflWarManager::IsDoingInfluenceWar()
{
	return m_bIsDoingInfluenceWarCheck;
}
// end 2012-11-26 by jhseol, 전쟁 시스템 리뉴얼 - 전쟁시 트리거 무적상태(일시정지), 지금이 전쟁중인지 확인하는 함수
void CInflWarManager::InitStrategyPointSummonRange(SSTRATEGYPOINT_SUMMON_RANGE *i_pStrategyPointSummonRange)
{
	EnterCriticalSection(&m_criticallStrategyPointSummonRange);
	util::zero(&m_StrategyPointSummonRange, sizeof(SSTRATEGYPOINT_SUMMON_RANGE));

	m_StrategyPointSummonRange = *i_pStrategyPointSummonRange;
	LeaveCriticalSection(&m_criticallStrategyPointSummonRange);
}
#ifdef MS_TAKE_NO_DAMAGE
UINT CInflWarManager::BlockDameMS()
{
	UINT _return = 0;
		if (CInflWarManager::MSwar_M == 0)
		{
			CInflWarManager::MSwar_M = CInflWarManager::SYS_M;
			CInflWarManager::MSwar_H = CInflWarManager::SYS_H;
		}
		if (CInflWarManager::m_calculation(CInflWarManager::SYS_H, CInflWarManager::SYS_M, CInflWarManager::MSwar_H, CInflWarManager::MSwar_M)< CInflWarManager::MS_SAFE_TIME)
		{
			_return = CInflWarManager::MS_SAFE_TIME-CInflWarManager::m_calculation(CInflWarManager::SYS_H, CInflWarManager::SYS_M, CInflWarManager::MSwar_H, CInflWarManager::MSwar_M);
		}
		else
		{
			_return = 0;
		}
	return _return;
}
#endif
void CInflWarManager::InitStrategyPointNotSummonTime(vectSSTRATEGYPOINT_NOT_SUMMON_TIME *i_pvectSStrategyPointNotSummonTime)
{
	mt_auto_lock mta(this->GetStrategyPointNotSummonTime());

	m_mtvectStrategyPointNotSummonTime.clear();

	m_mtvectStrategyPointNotSummonTime.assign(i_pvectSStrategyPointNotSummonTime->begin(), i_pvectSStrategyPointNotSummonTime->end());
}

void CInflWarManager::InitStrategyPoint()
{
	this->LoadStrategyPointSummonInfo();
	
}
#ifdef MS_TAKE_NO_DAMAGE
UINT CInflWarManager::m_calculation(int new_h, int new_m, int old_h, int old_m)
{
	int minutes = 0;
	if (new_h == old_h)
	{
		minutes = new_m - old_m;
	}
	else
	{
		minutes = (60 - old_m) + new_m;
	}
	return minutes;
}
#endif
void CInflWarManager::LoadStrategyPointSummonInfo()
{
	//////////////////////////////////////////////////////////////////////////
	// 2012-11-13 by jhseol, 전쟁 시스템 리뉴얼 - 거점전 정보 로드(요일별 셋팅 정보 및 거점 설정 정보)
	if (S_WAR_SYSTEM_RENEWAL_STRATEGYPOINT_JHSEOL)
	{
		m_atBeforeCheckTime.SetCurrentDateTime();	// 로드한 시간 저장
		this->m_pFieldIOCP14->m_pAtumDBManager->MakeAndEnqueueQuery(QT_LoadRenewalStrategyPointSummonInfo, NULL, 0, NULL);
	}
	else this->m_pFieldIOCP14->m_pAtumDBManager->MakeAndEnqueueQuery(QT_LoadStrategyPointSummonInfo, NULL, 0, NULL);
}

void CInflWarManager::LoadStrategyPointSummonRange()
{
	this->m_pFieldIOCP14->m_pAtumDBManager->MakeAndEnqueueQuery(QT_LoadStrategyPointSummonRange, NULL, 0, NULL);
}

void CInflWarManager::LoadStrategyPointNotSummonTime()
{
	this->m_pFieldIOCP14->m_pAtumDBManager->MakeAndEnqueueQuery(QT_LoadStrategyPointNotSummonTime, NULL, 0, NULL);
}

BOOL CInflWarManager::CheckStrategyPointSummon()
{
	EnterCriticalSection(&m_criticallStrategyPointSummonRange);
	if(TRUE == m_StrategyPointSummonRange.StrategyPointSummonCheck)
	{
		LeaveCriticalSection(&m_criticallStrategyPointSummonRange);
		return TRUE;
	}
	LeaveCriticalSection(&m_criticallStrategyPointSummonRange);

	return FALSE;
}

BOOL CInflWarManager::CompareStrategyPointSummonTime()
{
	EnterCriticalSection(&m_criticallStrategyPointSummonRange);
	if(0 < m_StrategyPointSummonRange.EndTime.GetTimeDiffToCurrentTimeInSeconds())
	{// 2007-03-02 by dhjin, 아직 주기가 끝나지 않았다.
		LeaveCriticalSection(&m_criticallStrategyPointSummonRange);
		return TRUE;
	}
	LeaveCriticalSection(&m_criticallStrategyPointSummonRange);
	
	return FALSE;
}

void CInflWarManager::SetAllStrategyPointSummonTime()
{
	mt_auto_lock mta(this->GetStrategyPointSummonTimeInfo());
	
	if(FALSE == m_StrategyPointSummonRange.StrategyPointSummonCheck)
	{// 2008-08-20 by dhjin, 만약 생성주기값이 초기화가 안되어있다면 리턴한다.
		return;
	}
	
	//////////////////////////////////////////////////////////////////////////
	// 2007-03-12 by dhjin, 
	// mt4HourStartTime : 한 주기의 StartTime값에 4시간을 더한다. 주기 끝나는 시점에 앞뒤로 시간차를 두어 중복현상을 피한다.
	// mt4HourEndTime   : 한 주기의 EndTime값에 4시간을 빼준다.
	// mtCurTime		: 현재 시간을 구하고 현재 시간이 시작 시간 보다 전이면 시작 시간을 현재 시간으로 설정한다.
	ATUM_DATE_TIME mt4HourStartTime;
	mt4HourStartTime = m_StrategyPointSummonRange.StartTime;
	mt4HourStartTime.AddDateTime(0,0,0,4,0,0);
	ATUM_DATE_TIME mt4HourEndTime;		
	mt4HourEndTime = m_StrategyPointSummonRange.EndTime;
	mt4HourEndTime.AddDateTime(0,0,0,-4,0,0);
	ATUM_DATE_TIME mtCurTime;			
	mtCurTime.SetCurrentDateTime();
	mtCurTime.AddDateTime(0,0,0,0,10,0);
	if(0 < mt4HourStartTime.GetTimeDiffTimeInSeconds(mtCurTime))
	{// 2007-03-05 by dhjin, 현재 시간이 시작 시간 보다 전이면 시작 시간을 현재 시간으로 설정한다.
		mtCurTime = mt4HourStartTime;
	}

	//////////////////////////////////////////////////////////////////////////
	// 2007-03-12 by dhjin, 
	// Range		: 현재 시간과 주기 마지막 시간과 차이나는 분을 구하여 랜덤 설정 변수(날짜)로 사용한다.
	// TimeRange	: 현재 시간과 주기 마지막 시간과 차이나는 분을 구하여 랜덤 설정 변수(초)로 사용한다.
	int Range = mt4HourEndTime.GetTimeDiffTimeInMinutes(mtCurTime);	
	int TimeRange = 0;												

	// 2009-10-26 by cmkwon, 거점 로딩시 로그 추가 - 
	g_pFieldGlobal->WriteSystemLogEX(TRUE, "  [Notify] CInflWarManager::SetAllStrategyPointSummonTime# 11000 \r\n");

	// 2007-03-12 by dhjin, 요일별 소환 될 수 있는 최대 수를 관리하기 위해 초기화를 한다.
	this->InitStrategyPointDay(&mtCurTime, &mt4HourEndTime);

	// 2009-10-26 by cmkwon, 거점 로딩시 로그 추가 - 
	g_pFieldGlobal->WriteSystemLogEX(TRUE, "  [Notify] CInflWarManager::SetAllStrategyPointSummonTime# 12000 \r\n");

	mtvectSSTRATEGYPOINT_SUMMONTIME_INFO::iterator itr = m_mtvectStrategyPointSummonTimeInfo.begin();
	for(; itr != m_mtvectStrategyPointSummonTimeInfo.end(); itr++)
	{
		if(FALSE == this->CompareStrategyPointDay()
			|| 0 >= Range)
		{// 2007-03-07 by dhjin, 더 이상 할당할 요일이 없다. 다음 주기로 넘긴다.
			itr->SummonTime.AddDateTime(1,0,0, 0,0,0);
			QPARAM_UPDATE_STRATEGYPOINTSUMMONINFO_TIME *pQparam = new QPARAM_UPDATE_STRATEGYPOINTSUMMONINFO_TIME;
			pQparam->MapIndex		= itr->MapIndex;
			pQparam->SummonTime		= itr->SummonTime;
			pQparam->SummonAttribute	= FALSE;		// 2008-04-08 by dhjin, 소환 가능 시간 설정 - 
			this->m_pFieldIOCP14->m_pAtumDBManager->MakeAndEnqueueQuery(QT_UpdateStrategyPointSummonInfoBySummonTime, NULL, 0, pQparam);
			continue;
		}
	
		// 2007-03-07 by dhjin, 요일에 할당한다.
		if(0 >= itr->SummonCount)
		{
			continue;
		}
		
		// 2007-03-12 by dhjin, itr->SummonCount = 0 은 더 이상 소환 할 수 없으므로 설정에서 제외한다.
		// 2007-03-12 by dhjin, 날짜를 랜덤하게 설정한다.
		itr->SummonTime = mtCurTime;
		itr->SummonTime.AddDateTime(0,0,0, 0,(RANDI(1, Range)),0);
		ATUM_DATE_TIME tmMakeTime;
		tmMakeTime.SetDateTime(itr->SummonTime.Year, itr->SummonTime.Month, itr->SummonTime.Day, 0, 0, 0);

		//////////////////////////////////////////////////////////////////////////
		// 2007-03-12 by dhjin
		// tmCheckRangeTime : 설정 된 날짜가 어느 주기의 날인지 체크하여 한 날짜에 랜덤하게 설정 가능한 범위의 초를 설정하기 위한 변수.
		// tmIncreaseDay	: 루프 수를 줄이기 위한 변수 값 랜덤으로 설정이 실패하면 날자를 하나씩 더하여 소환 가능한 날자에 설정한다.
		int				tmCheckRangeTime = 1;		
		int				tmIncreaseDay = 0;
		
		// 2009-10-26 by cmkwon, 거점 로딩시 로그 추가 - 
		g_pFieldGlobal->WriteSystemLogEX(TRUE, "  [Notify] CInflWarManager::SetAllStrategyPointSummonTime# 12100 tmMakeTime(%s) tmCheckRangeTime(%d) itrSummTime(%s) \r\n"
			, tmMakeTime.GetDateTimeString().GetBuffer(), tmCheckRangeTime, itr->SummonTime.GetDateTimeString().GetBuffer());

		while(FALSE == this->CompareStrategyPointDayByOneDay(&tmMakeTime, &tmCheckRangeTime))
		{// 2007-03-09 by dhjin, 소환 가능한 날자가 있는지 체크하여 그 날자를 우선 설정한다.
			itr->SummonTime = mtCurTime;
			itr->SummonTime.AddDateTime(0,0,tmIncreaseDay, 0,0,0);
			tmIncreaseDay++;
			tmMakeTime.SetDateTime(itr->SummonTime.Year, itr->SummonTime.Month, itr->SummonTime.Day, 0, 0, 0);
		}

		// 2009-10-26 by cmkwon, 거점 로딩시 로그 추가 - 
		g_pFieldGlobal->WriteSystemLogEX(TRUE, "  [Notify] CInflWarManager::SetAllStrategyPointSummonTime# 12200 tmMakeTime(%s) tmCheckRangeTime(%d) itrSummTime(%s) \r\n"
			, tmMakeTime.GetDateTimeString().GetBuffer(), tmCheckRangeTime, itr->SummonTime.GetDateTimeString().GetBuffer());

		// 2007-03-12 by dhjin, 설정된 날짜에 소환 될 수 있는 count값을 1 증가 시킨다.
		this->IncreaseStrategyPointDay(&(itr->SummonTime));


		//////////////////////////////////////////////////////////////////////////
		// 날자 설정 값이 시작 날이거나 끝날이면 랜덤하게 설정할 시간 Range값을 수정한다.
		if(1 == m_mtvectStrategyPointDay.size())
		{
			Range = mt4HourEndTime.GetTimeDiffTimeInMinutes(mtCurTime);	// 2008-04-04 by dhjin, 소환 가능 시간 설정 - 버그 수정, 첫날 소환 안될때 다음날로 설정이 되어 마지막날의 차이를 다시 계산한다.	
			TimeRange = Range;
			tmMakeTime = mtCurTime;
		}
		else if(1 == tmCheckRangeTime)
		{
			ATUM_DATE_TIME	tmPlusOneDay;
			tmPlusOneDay.SetDateTime(mtCurTime.Year, mtCurTime.Month, mtCurTime.Day+1,0,0,0);
			TimeRange = tmPlusOneDay.GetTimeDiffTimeInMinutes(mtCurTime);
			tmMakeTime = mtCurTime;
		}
		else if(m_mtvectStrategyPointDay.size() == tmCheckRangeTime)
		{
			TimeRange = mt4HourEndTime.GetTimeDiffTimeInMinutes(tmMakeTime);
		}
		else
		{
			TimeRange = 24 * 60;
		}

		BOOL SummonAttribute = FALSE;		// 2008-04-08 by dhjin, 소환 가능 시간 설정 - 


		// 2009-10-26 by cmkwon, 거점 로딩시 로그 추가 - 
		g_pFieldGlobal->WriteSystemLogEX(TRUE, "  [Notify] CInflWarManager::SetAllStrategyPointSummonTime# 12300 tmMakeTime(%s) TimeRange(%d) itrSummTime(%s) \r\n"
			, tmMakeTime.GetDateTimeString().GetBuffer(), TimeRange, itr->SummonTime.GetDateTimeString().GetBuffer());

		// 2007-03-12 by dhjin, 시간을 요일별 금지 시간을 피해 랜덤하게 설정한다.
		if(FALSE == this->CheckStrategyPointNotSummonTime(&tmMakeTime, TimeRange, &SummonAttribute))
		{
			char szErr[1024];
			wsprintf(szErr, "CInflWarManager::Not_SummonTime Error");
			g_pFieldGlobal->WriteSystemLog(szErr);
			DbgOut("%s\rn", szErr);
			// 2007-03-29 by dhjin, 할당하지 못했다면 다음 주기에 소환한다.
			itr->SummonTime.AddDateTime(1,0,0, 0,0,0);
			QPARAM_UPDATE_STRATEGYPOINTSUMMONINFO_TIME *pQparam = new QPARAM_UPDATE_STRATEGYPOINTSUMMONINFO_TIME;
			pQparam->MapIndex		= itr->MapIndex;
			pQparam->SummonTime		= itr->SummonTime;
			pQparam->SummonAttribute	= FALSE;		// 2008-04-08 by dhjin, 소환 가능 시간 설정 - 
			this->m_pFieldIOCP14->m_pAtumDBManager->MakeAndEnqueueQuery(QT_UpdateStrategyPointSummonInfoBySummonTime, NULL, 0, pQparam);
			continue;
		}
		
		// 2009-10-26 by cmkwon, 거점 로딩시 로그 추가 - 
		g_pFieldGlobal->WriteSystemLogEX(TRUE, "  [Notify] CInflWarManager::SetAllStrategyPointSummonTime# 12400 tmMakeTime(%s) TimeRange(%d) itrSummTime(%s) \r\n"
			, tmMakeTime.GetDateTimeString().GetBuffer(), TimeRange, itr->SummonTime.GetDateTimeString().GetBuffer());

		// 2007-03-12 by dhjin, 설정된 소환시간 정보를 구조체에 할당한다.
		itr->SummonTime = tmMakeTime;
		// 2007-03-12 by dhjin, 설정된 소환시간 정보를 DB에 업데이트 한다.
		QPARAM_UPDATE_STRATEGYPOINTSUMMONINFO_TIME *pQparam = new QPARAM_UPDATE_STRATEGYPOINTSUMMONINFO_TIME;
		pQparam->MapIndex		= itr->MapIndex;
		pQparam->SummonTime		= tmMakeTime;
		pQparam->SummonAttribute	= SummonAttribute;		// 2008-04-08 by dhjin, 소환 가능 시간 설정 - 
		this->m_pFieldIOCP14->m_pAtumDBManager->MakeAndEnqueueQuery(QT_UpdateStrategyPointSummonInfoBySummonTime, NULL, 0, pQparam);
	}

	// 2009-10-26 by cmkwon, 거점 로딩시 로그 추가 - 
	g_pFieldGlobal->WriteSystemLogEX(TRUE, "  [Notify] CInflWarManager::SetAllStrategyPointSummonTime# 13000 \r\n");

}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CInflWarManager::SetAllStrategyPointSummonTimeNew()
/// \brief		전략포인트 시간 설정 수정 - 전략포인트 소환 시간 전부 업데이트 
/// \author		dhjin
/// \date		2009-03-23 ~ 2009-03-23
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CInflWarManager::SetAllStrategyPointSummonTimeNew()
{
	mt_auto_lock mta(this->GetStrategyPointSummonTimeInfo());

	// 2009-07-02 by cmkwon, 서버 실행시 실행 관련 로그 추가 - 
	g_pFieldGlobal->WriteSystemLogEX(TRUE, "  [Notify] CInflWarManager::SetAllStrategyPointSummonTimeNew# SummonFlag(%d) SummonRange(%d) MaxSummonCntPerDaily(%d) Period(%s~%s) NewSummonRange(%d) NewMaxSummonCntPerDaily(%d) SummonTimeInfoListCnt(%d) \r\n"
		, m_StrategyPointSummonRange.StrategyPointSummonCheck, m_StrategyPointSummonRange.SummonRange, m_StrategyPointSummonRange.MaxSummonCountDaily
		, m_StrategyPointSummonRange.StartTime.GetDateTimeString().GetBuffer(), m_StrategyPointSummonRange.EndTime.GetDateTimeString().GetBuffer()
		, m_StrategyPointSummonRange.NewSummonRange, m_StrategyPointSummonRange.NewMaxSummonCountDaily, m_mtvectStrategyPointSummonTimeInfo.size());

	if(FALSE == m_StrategyPointSummonRange.StrategyPointSummonCheck
		|| 0 >= m_mtvectStrategyPointSummonTimeInfo.size()
		)
	{// 2008-08-20 by dhjin, 만약 생성주기값이 초기화가 안되어있다면 리턴한다.
		return;
	}
	
	//////////////////////////////////////////////////////////////////////////
	// 2007-03-12 by dhjin, 
	// mt4HourStartTime : 한 주기의 StartTime값에 4시간을 더한다. 주기 끝나는 시점에 앞뒤로 시간차를 두어 중복현상을 피한다.
	// mt4HourEndTime   : 한 주기의 EndTime값에 4시간을 빼준다.
	// mtCurTime		: 현재 시간을 구하고 현재 시간이 시작 시간 보다 전이면 시작 시간을 현재 시간으로 설정한다.
	ATUM_DATE_TIME mt4HourStartTime;
	mt4HourStartTime = m_StrategyPointSummonRange.StartTime;
	mt4HourStartTime.AddDateTime(0,0,0,4,0,0);
	ATUM_DATE_TIME mt4HourEndTime;		
	mt4HourEndTime = m_StrategyPointSummonRange.EndTime;
	mt4HourEndTime.AddDateTime(0,0,0,-4,0,0);
	ATUM_DATE_TIME mtCurTime;			
	mtCurTime.SetCurrentDateTime();
	mtCurTime.AddDateTime(0,0,0,0,10,0);
	if(0 < mt4HourStartTime.GetTimeDiffTimeInSeconds(mtCurTime))
	{// 2007-03-05 by dhjin, 현재 시간이 시작 시간 보다 전이면 시작 시간을 현재 시간으로 설정한다.
		mtCurTime = mt4HourStartTime;
	}	

	//////////////////////////////////////////////////////////////////////////
	// 2007-03-12 by dhjin, 
	// Range		: 현재 시간과 주기 마지막 시간과 차이나는 분을 구하여 랜덤 설정 변수(날짜)로 사용한다.
	// TimeRange	: 현재 시간과 주기 마지막 시간과 차이나는 분을 구하여 랜덤 설정 변수(초)로 사용한다.
	int Range = mt4HourEndTime.GetTimeDiffTimeInMinutes(mtCurTime);	
	if(Range <= 0)
	{// 2009-03-23 by dhjin, 주기 끝나는 시간 체크 오류시 리턴
		return;
	}
	int TimeRange = 0;
	// 전략포인트 설정 가능 시간 정보를 저장하며 총 설정 가능 시간을 얻는다.
	INT TotalSPPMin = this->SetSPPossibleTimeInfo(&mtCurTime, &mt4HourEndTime);
	BYTE MaxSPSummonCountByMapIndex = 0;
	BYTE TotalSPSummonCount = 0;
	// 전략포인트가 한 맵에 최대 몇 개가 소환 되어야 될지 체크하며 총 소환 수로 총 설정 가능 시간을 나눠 랜덤 돌릴 분을 구한다.
	this->SetSPSummonCountInfo(&MaxSPSummonCountByMapIndex, &TotalSPSummonCount);

	// 2009-07-02 by cmkwon, 서버 실행시 실행 관련 로그 추가 - 
	g_pFieldGlobal->WriteSystemLogEX(TRUE, "  [Notify] CInflWarManager::SetAllStrategyPointSummonTimeNew# Period(%s ~ %s) Range(%d minutes) TotalSPPMin(%d) TotalSPSummonCount(%d) \r\n"
		, mt4HourStartTime.GetDateTimeString().GetBuffer(), mt4HourEndTime.GetDateTimeString().GetBuffer(), Range, TotalSPPMin, TotalSPSummonCount);
	if(0 >= TotalSPSummonCount)
	{// 2009-07-02 by cmkwon, 거검전 관련 버그 수정 - 
		return;
	}

// 2010-01-07 by cmkwon, 거점전 최시 시간텀을 10분으로 변경 - 기존 30분
//	INT SPPMin = max(30, TotalSPPMin/TotalSPSummonCount);		// 2009-11-11 by cmkwon, 전략포인트 소환 관련 수정 - 최소 30분 이상 차이가 나도록
	INT SPPMin = max(10, TotalSPPMin/TotalSPSummonCount);		// 2009-11-11 by cmkwon, 전략포인트 소환 관련 수정 - 최소 30분 이상 차이가 나도록
	INT SPStepMin = 0;

	// 전략포인트 소환될 맵을 랜덤하게 정렬한다.
	std::random_shuffle(m_mtvectStrategyPointSummonTimeInfo.begin(), m_mtvectStrategyPointSummonTimeInfo.end());
	// 한 맵에 소환이 많은 순으로 정렬한다.
	this->SortBySPSummonCount();

	m_mtvectALLSPSummonTimeInfo.clear();
	mt_auto_lock AllSP(&m_mtvectALLSPSummonTimeInfo);
	SSTRATEGYPOINT_SUMMONTIME_INFO tmSPSummonInfo;
	util::zero(&tmSPSummonInfo, sizeof(SSTRATEGYPOINT_SUMMONTIME_INFO));
	for(int SPSCountByMapindex = 1; SPSCountByMapindex <= MaxSPSummonCountByMapIndex; SPSCountByMapindex++)
	{
		mtvectSSTRATEGYPOINT_SUMMONTIME_INFO::iterator itr = m_mtvectStrategyPointSummonTimeInfo.begin();
		for(; itr != m_mtvectStrategyPointSummonTimeInfo.end(); itr++)
		{
			if(SPSCountByMapindex <= itr->SummonCount)
			{// 한 주기에 소환 되어야 하는 정보를 전부 m_mtvectALLSPSummonTimeInfo 저장한다.
				tmSPSummonInfo.MapIndex	= itr->MapIndex;
				this->CalcSPSummonTime(SPStepMin, SPPMin, &(tmSPSummonInfo.SummonTime), &(tmSPSummonInfo.SummonAttribute));
				if(!tmSPSummonInfo.SummonTime.IsValid())
				{
					tmSPSummonInfo.SummonTime.SetCurrentDateTime();
					tmSPSummonInfo.SummonTime.AddDateTime(1,0,0,0,0,0);
				}
				SPStepMin += SPPMin;
				m_mtvectALLSPSummonTimeInfo.push_back(tmSPSummonInfo);

				g_pFieldGlobal->WriteSystemLogEX(TRUE, "  [Notify]: AllStrategypoint Summon Info, MapIndex(%d), SummonTime(%s) SPStepMin(%4d) SPPMin(%4d)\r\n"
					, tmSPSummonInfo.MapIndex, tmSPSummonInfo.SummonTime.GetDateTimeString().GetBuffer(), SPStepMin, SPPMin);
				
				if(1 == SPSCountByMapindex)
				{// 첫 정보만 업데이트 한다. 다음 정보는 소환 되면 m_mtvectALLSPSummonTimeInfo에서 정보를 가져와 업데이트를 해준다.
					itr->SummonTime			= tmSPSummonInfo.SummonTime;
					itr->SummonAttribute	= tmSPSummonInfo.SummonAttribute;
					QPARAM_UPDATE_STRATEGYPOINTSUMMONINFO_TIME *pQparam = new QPARAM_UPDATE_STRATEGYPOINTSUMMONINFO_TIME;
					pQparam->MapIndex		= itr->MapIndex;
					pQparam->SummonTime		= itr->SummonTime;
					pQparam->SummonAttribute	= itr->SummonAttribute;		// 2008-04-08 by dhjin, 소환 가능 시간 설정 - 
					this->m_pFieldIOCP14->m_pAtumDBManager->MakeAndEnqueueQuery(QT_UpdateStrategyPointSummonInfoBySummonTime, NULL, 0, pQparam);		
				}
				util::zero(&tmSPSummonInfo, sizeof(SSTRATEGYPOINT_SUMMONTIME_INFO));
			}
		}
	}
	AllSP.auto_unlock_cancel();
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			INT CInflWarManager::SetSPPossibleTimeInfo(ATUM_DATE_TIME *i_pSTime, ATUM_DATE_TIME *i_pETime)
/// \brief		전략포인트 시간 설정 수정 - 전략포인트 소환 가능한 시간값 전부 저장
/// \author		dhjin
/// \date		2009-03-23 ~ 2009-03-23
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
INT CInflWarManager::SetSPPossibleTimeInfo(ATUM_DATE_TIME *i_pSTime, ATUM_DATE_TIME *i_pETime)
{
	m_vectSSPPossibleTimeInfo.clear();
	SSPPossibleTimeInfo	SPPTimeInfo;
	INT TotalSPPossibleMin = 0;
	ATUM_DATE_TIME CheckSTime;
	CheckSTime	= *i_pSTime;
	CheckSTime.Hour = CheckSTime.Minute = CheckSTime.Second	= 0;	// 2009-11-11 by cmkwon, 전략포인트 소환 관련 수정 - 00:00:00 로 시간 초기화

	mt_auto_lock mta(this->GetStrategyPointNotSummonTime());

	for(; CheckSTime < *i_pETime; CheckSTime.AddDateTime(0,0,1,0,0,0))
	{// 주기 마지막날까지 체크
		util::zero(&SPPTimeInfo, sizeof(SSPPossibleTimeInfo));
		
		auto localStTime = tm(CheckSTime);
		
		vectSSTRATEGYPOINT_NOT_SUMMON_TIME::iterator itrSt = m_mtvectStrategyPointNotSummonTime.begin();
		while(itrSt != m_mtvectStrategyPointNotSummonTime.end())
		{// 2007-03-30 by dhjin, 첫날 금지시간 얻어오기
			if(localStTime.tm_wday == itrSt->DayOfWeek)
			{
				if(itrSt->CheckSummon)
				{// 소환 가능 시간 범위는 하루에 한 개만 나올수있다.
					SPPTimeInfo.StartTime.SetDateTime(CheckSTime.Year, CheckSTime.Month, CheckSTime.Day
														, itrSt->StartTime.Hour, itrSt->StartTime.Minute, itrSt->StartTime.Second);
					SPPTimeInfo.EndTime.SetDateTime(CheckSTime.Year, CheckSTime.Month, CheckSTime.Day
														, itrSt->EndTime.Hour, itrSt->EndTime.Minute, itrSt->EndTime.Second);


					///////////////////////////////////////////////////////////////////////////////
					// 2010-01-28 by cmkwon, 거점전 소환 시간 관련 수정(거점전 진행 시간 빼는것 제외) - 기간 이외의 시간 체크
					if(*i_pSTime >= SPPTimeInfo.EndTime
						|| *i_pETime <= SPPTimeInfo.StartTime)
					{
						break;
					}

// 2010-01-28 by cmkwon, 거점전 소환 시간 관련 수정(거점전 진행 시간 빼는것 제외) - 
// 					if(SPPTimeInfo.EndTime > *i_pETime)
// 					{// 2009-11-11 by cmkwon, 전략포인트 소환 관련 수정 - 주기 종료시간 체크
// 						SPPTimeInfo.EndTime = *i_pETime;
// 					}
// 
// 					if(*i_pSTime > SPPTimeInfo.StartTime
// 						&& *i_pSTime < SPPTimeInfo.EndTime)
// 					{// 설정된 시간이 현재 시간보다 전 시간이면 현재 시간을 시작 시간으로 잡는다.
// 						SPPTimeInfo.StartTime.SetDateTime(CheckSTime.Year, CheckSTime.Month, CheckSTime.Day
// 															, i_pSTime->Hour, i_pSTime->Minute, i_pSTime->Second);
// 					}
// 					else if(*i_pSTime > SPPTimeInfo.EndTime)
// 					{// 설정된 시간의 끝시간이 현재 시간보다 전 시간이면 설정될 수 있는 시간이 없다.
// 						break;
// 					}
					///////////////////////////////////////////////////////////////////////////////
					// 2010-01-28 by cmkwon, 거점전 소환 시간 관련 수정(거점전 진행 시간 빼는것 제외) - 
					if(*i_pSTime > SPPTimeInfo.StartTime)
					{
 						SPPTimeInfo.StartTime.SetDateTime(CheckSTime.Year, CheckSTime.Month, CheckSTime.Day, i_pSTime->Hour, i_pSTime->Minute, i_pSTime->Second);
					}
					if(*i_pETime < SPPTimeInfo.EndTime)
					{
 						SPPTimeInfo.EndTime = *i_pETime;
					}
					
					SPPTimeInfo.PossibleMinRange = SPPTimeInfo.EndTime.GetTimeDiffTimeInMinutes(SPPTimeInfo.StartTime);
					SPPTimeInfo.CheckSummon = TRUE;
					m_vectSSPPossibleTimeInfo.push_back(SPPTimeInfo);		// 2010-01-28 by cmkwon, 거점전 소환 시간 관련 수정(거점전 진행 시간 빼는것 제외) - 
					TotalSPPossibleMin += SPPTimeInfo.PossibleMinRange;		// 2010-01-28 by cmkwon, 거점전 소환 시간 관련 수정(거점전 진행 시간 빼는것 제외) - 

// 2010-01-28 by cmkwon, 거점전 소환 시간 관련 수정(거점전 진행 시간 빼는것 제외) - 
// 					if(OUTPOST_WARTIME < SPPTimeInfo.PossibleMinRange)
// 					{// 2009-11-11 by cmkwon, 전략포인트 소환 관련 수정 - 
// 						SPPTimeInfo.EndTime.AddDateTime(0, 0, 0, 0, -OUTPOST_WARTIME);		// 2009-11-11 by cmkwon, 전략포인트 소환 관련 수정 - 
// 						SPPTimeInfo.PossibleMinRange	-= OUTPOST_WARTIME;					// 2009-11-11 by cmkwon, 전략포인트 소환 관련 수정 - 
// 						m_vectSSPPossibleTimeInfo.push_back(SPPTimeInfo);
// 						TotalSPPossibleMin += SPPTimeInfo.PossibleMinRange;
// 					}

				}
				else
				{// 소환 금지 시간은 소환 가능 시간 범위가 하루에 두 개 나온다. 
					SPPTimeInfo.StartTime.SetDateTime(CheckSTime.Year, CheckSTime.Month, CheckSTime.Day
														, 0, 0, 0);
					SPPTimeInfo.EndTime.SetDateTime(CheckSTime.Year, CheckSTime.Month, CheckSTime.Day
														, itrSt->StartTime.Hour, itrSt->StartTime.Minute, itrSt->StartTime.Second);

					if(SPPTimeInfo.EndTime > *i_pETime)
					{// 2009-11-11 by cmkwon, 전략포인트 소환 관련 수정 - 주기 종료시간 체크
						SPPTimeInfo.EndTime = *i_pETime;
					}

					SPPTimeInfo.PossibleMinRange = SPPTimeInfo.EndTime.GetTimeDiffTimeInMinutes(SPPTimeInfo.StartTime);
					if(*i_pSTime > SPPTimeInfo.StartTime
						&& *i_pSTime < SPPTimeInfo.EndTime)
					{// 설정된 시간이 현재 시간보다 전 시간이면 현재 시간을 시작 시간으로 잡는다.
						SPPTimeInfo.StartTime.SetDateTime(CheckSTime.Year, CheckSTime.Month, CheckSTime.Day
															, i_pSTime->Hour, i_pSTime->Minute, i_pSTime->Second);
					}
					if(*i_pSTime < SPPTimeInfo.EndTime)
					{
						SPPTimeInfo.CheckSummon = FALSE;
						m_vectSSPPossibleTimeInfo.push_back(SPPTimeInfo);		// 2010-01-28 by cmkwon, 거점전 소환 시간 관련 수정(거점전 진행 시간 빼는것 제외) - 
						TotalSPPossibleMin += SPPTimeInfo.PossibleMinRange;		// 2010-01-28 by cmkwon, 거점전 소환 시간 관련 수정(거점전 진행 시간 빼는것 제외) - 

// 2010-01-28 by cmkwon, 거점전 소환 시간 관련 수정(거점전 진행 시간 빼는것 제외) - 
// 						if(OUTPOST_WARTIME < SPPTimeInfo.PossibleMinRange)
// 						{// 2009-11-11 by cmkwon, 전략포인트 소환 관련 수정 - 
// 							SPPTimeInfo.EndTime.AddDateTime(0, 0, 0, 0, -OUTPOST_WARTIME);		// 2009-11-11 by cmkwon, 전략포인트 소환 관련 수정 - 
// 							SPPTimeInfo.PossibleMinRange	-= OUTPOST_WARTIME;					// 2009-11-11 by cmkwon, 전략포인트 소환 관련 수정 - 
// 							m_vectSSPPossibleTimeInfo.push_back(SPPTimeInfo);
// 							TotalSPPossibleMin += SPPTimeInfo.PossibleMinRange;
// 						}
					}
					SPPTimeInfo.StartTime.SetDateTime(CheckSTime.Year, CheckSTime.Month, CheckSTime.Day, itrSt->EndTime.Hour, itrSt->EndTime.Minute, itrSt->EndTime.Second);
					SPPTimeInfo.EndTime.SetDateTime(CheckSTime.Year, CheckSTime.Month, CheckSTime.Day, 23, 59, 59);		// 2009-11-11 by cmkwon, 전략포인트 소환 관련 수정 - 23:59:59 로 변경

					if(SPPTimeInfo.EndTime > *i_pETime)
					{// 2009-11-11 by cmkwon, 전략포인트 소환 관련 수정 - 주기 종료시간 체크
						SPPTimeInfo.EndTime = *i_pETime;
					}

					if(*i_pSTime > SPPTimeInfo.StartTime
						&& *i_pSTime < SPPTimeInfo.EndTime)
					{// 설정된 시간이 현재 시간보다 전 시간이면 현재 시간을 시작 시간으로 잡는다.
						SPPTimeInfo.StartTime.SetDateTime(CheckSTime.Year, CheckSTime.Month, CheckSTime.Day
															, i_pSTime->Hour, i_pSTime->Minute, i_pSTime->Second);
					}
					else if(*i_pSTime > SPPTimeInfo.EndTime)
					{// 설정된 시간의 끝시간이 현재 시간보다 전 시간이면 설정될 수 있는 시간이 없다.
						break;
					}
					SPPTimeInfo.PossibleMinRange = SPPTimeInfo.EndTime.GetTimeDiffTimeInMinutes(SPPTimeInfo.StartTime);
					SPPTimeInfo.CheckSummon = FALSE;
					m_vectSSPPossibleTimeInfo.push_back(SPPTimeInfo);		// 2010-01-28 by cmkwon, 거점전 소환 시간 관련 수정(거점전 진행 시간 빼는것 제외) - 
					TotalSPPossibleMin += SPPTimeInfo.PossibleMinRange;		// 2010-01-28 by cmkwon, 거점전 소환 시간 관련 수정(거점전 진행 시간 빼는것 제외) - 

// 2010-01-28 by cmkwon, 거점전 소환 시간 관련 수정(거점전 진행 시간 빼는것 제외) - 
// 					if(OUTPOST_WARTIME < SPPTimeInfo.PossibleMinRange)
// 					{// 2009-11-11 by cmkwon, 전략포인트 소환 관련 수정 - 
// 						SPPTimeInfo.EndTime.AddDateTime(0, 0, 0, 0, -OUTPOST_WARTIME);		// 2009-11-11 by cmkwon, 전략포인트 소환 관련 수정 - 
// 						SPPTimeInfo.PossibleMinRange	-= OUTPOST_WARTIME;					// 2009-11-11 by cmkwon, 전략포인트 소환 관련 수정 - 
// 						m_vectSSPPossibleTimeInfo.push_back(SPPTimeInfo);
// 						TotalSPPossibleMin += SPPTimeInfo.PossibleMinRange;
// 					}

				}
				break;
			}
			itrSt++;
		}
	}

	///////////////////////////////////////////////////////////////////////////////
	// 2009-11-11 by cmkwon, 전략포인트 소환 관련 수정 - 소환 가능한 시간 시스템 로그에 남기기
	{
		vectSSPPossibleTimeInfo::iterator itrSPPTInfo(m_vectSSPPossibleTimeInfo.begin());
		for(; itrSPPTInfo != m_vectSSPPossibleTimeInfo.end(); itrSPPTInfo++)
		{
			SSPPossibleTimeInfo *pPTInfo = &*itrSPPTInfo;

			g_pFieldGlobal->WriteSystemLogEX(TRUE, "  [Notify] CInflWarManager::SetSPPossibleTimeInfo# PossibleTime(%s ~ %s) TermMinutes(%4d) CheckSummon(%d) \r\n"
				, pPTInfo->StartTime.GetDateTimeString().GetBuffer(), pPTInfo->EndTime.GetDateTimeString().GetBuffer()
				, pPTInfo->PossibleMinRange, pPTInfo->CheckSummon);
		}
	}

	return TotalSPPossibleMin;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CInflWarManager::SetSPSummonCountInfo(BYTE *o_pMaxSPSummonCountByMapIndex, BYTE *o_pTotalSPSummonCount)
/// \brief		전략포인트 시간 설정 수정 - 한 주기에 소환 되어야 되는 맵 당 수와 총 수를 계산한다.
/// \author		dhjin
/// \date		2009-03-30 ~ 2009-03-30
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CInflWarManager::SetSPSummonCountInfo(BYTE *o_pMaxSPSummonCountByMapIndex, BYTE *o_pTotalSPSummonCount)
{
	mtvectSSTRATEGYPOINT_SUMMONTIME_INFO::iterator itr = m_mtvectStrategyPointSummonTimeInfo.begin();
	for(; itr != m_mtvectStrategyPointSummonTimeInfo.end(); itr++)
	{
		if(*o_pMaxSPSummonCountByMapIndex < itr->SummonCount)
		{
			*o_pMaxSPSummonCountByMapIndex = itr->SummonCount;
		}
		*o_pTotalSPSummonCount += itr->SummonCount;
	}	
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CInflWarManager::SortBySPSummonCount()
/// \brief		맵당 소환 되는 수가 제일 많은 맵 순으로 정렬한다.
/// \author		dhjin
/// \date		2009-03-30 ~ 2009-03-30
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CInflWarManager::SortBySPSummonCount()
{
	sort(m_mtvectStrategyPointSummonTimeInfo.begin(), m_mtvectStrategyPointSummonTimeInfo.end(), SSTRATEGYPOINT_SUMMONTIME_INFO_BY_SUMMONCOUNT());
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CInflWarManager::GetNextSPSummonTimeByALLSPSummonTimeInfo(INT i_nMapindex, ATUM_DATE_TIME *o_pSummonTime, BOOL *o_bCheckSummon)
/// \brief		전략포인트 시간 설정 수정 - 다음 전략포인트 소환되는 정보 얻어오기
/// \author		dhjin
/// \date		2009-03-30 ~ 2009-03-30
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CInflWarManager::GetNextSPSummonTimeByALLSPSummonTimeInfo(INT i_nMapindex, ATUM_DATE_TIME *o_pSummonTime, BOOL *o_bCheckSummon)
{
	mtvectSSTRATEGYPOINT_SUMMONTIME_INFO::iterator itr = m_mtvectALLSPSummonTimeInfo.begin();
	for(; itr != m_mtvectALLSPSummonTimeInfo.end(); itr++)
	{
		if(i_nMapindex == itr->MapIndex)
		{
			*o_pSummonTime		= itr->SummonTime;
			*o_bCheckSummon		= itr->SummonAttribute;
			return TRUE;
		}
	}
	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CInflWarManager::CheckSPSummonMapIndex(INT i_nMapIndex)
/// \brief		해당 맵에 전략포인트가 생성 되어 있는지 체크
/// \author		dhjin
/// \date		2009-03-31 ~ 2009-03-31
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CInflWarManager::CheckSPSummonMapIndex(INT i_nMapIndex)
{
	mt_auto_lock mta(&m_mtvectSummonStrategyPointInfo);

	mtvectorSSUMMON_STRATEGYPOINT_INFO::iterator	itr = m_mtvectSummonStrategyPointInfo.begin();
	while(itr != m_mtvectSummonStrategyPointInfo.end())
	{
		if(i_nMapIndex == itr->MapIndex)
		{
			return TRUE;
		}
		itr++;
	}

	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CInflWarManager::DeleteALLSPSummonTimeInfoByMapIndex(INT i_nMapindex)
/// \brief		전략포인트 시간 설정 수정 - 소환된 전략포인트 정보 삭제
/// \author		dhjin
/// \date		2009-03-30 ~ 2009-03-30
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CInflWarManager::DeleteALLSPSummonTimeInfoByMapIndex(INT i_nMapindex)
{
	mtvectSSTRATEGYPOINT_SUMMONTIME_INFO::iterator itr = m_mtvectALLSPSummonTimeInfo.begin();
	for(; itr != m_mtvectALLSPSummonTimeInfo.end(); itr++)
	{
		if(i_nMapindex == itr->MapIndex)
		{
			itr = m_mtvectALLSPSummonTimeInfo.erase(itr);
			return TRUE;
		}
	}
	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CInflWarManager::CalcSPSummonTime(INT i_nSPStepMin, INT i_nSPPMin, ATUM_DATE_TIME *o_pSPTime, BOOL *o_bCheckSummon)
/// \brief		전략포인트 시간 설정 수정 - 전략포인트 소환 시간 계산
/// \author		dhjin
/// \date		2009-03-24 ~ 2009-03-24
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CInflWarManager::CalcSPSummonTime(INT i_nSPStepMin, INT i_nSPPMin, ATUM_DATE_TIME *o_pSPTime, BOOL *o_bCheckSummon)
{
	INT RandomSPMin = RANDI(0, i_nSPPMin);
	RandomSPMin += i_nSPStepMin;
	vectSSPPossibleTimeInfo::iterator itr = m_vectSSPPossibleTimeInfo.begin();
	for(; itr != m_vectSSPPossibleTimeInfo.end(); itr++)
	{
		if(itr->PossibleMinRange >= RandomSPMin)
		{
			(*o_pSPTime).SetDateTime(itr->StartTime.Year, itr->StartTime.Month, itr->StartTime.Day, itr->StartTime.Hour, itr->StartTime.Minute, 0);
			(*o_pSPTime).AddDateTime(0,0,0,0,RandomSPMin,0);
			*o_bCheckSummon = itr->CheckSummon;
			return;
		}
		
		RandomSPMin -= itr->PossibleMinRange;
	}
}

void CInflWarManager::SetStrategyPointSummonTime(SSTRATEGYPOINT_SUMMONTIME_INFO *i_pStrategyPointSummonTimeInfo, INT i_nSummonHourGap)
{
	//////////////////////////////////////////////////////////////////////////
	// 2007-03-12 by dhjin, 
	// mt4HourStartTime : 한 주기의 StartTime값에 4시간을 더한다. 주기 끝나는 시점에 앞뒤로 시간차를 두어 중복현상을 피한다.
	// mt4HourEndTime   : 한 주기의 EndTime값에 4시간을 빼준다.
	// mtCurTime		: 현재 시간을 구하고 현재 시간이 시작 시간 보다 전이면 시작 시간을 현재 시간으로 설정한다. 
	//					  전략포인트가 생성 되고 난 다음 호출되는 부분이기 때문에 지금 시간으로부터 i_nSummonHourGap을 더한다.
	ATUM_DATE_TIME mt4HourStartTime;
	mt4HourStartTime = m_StrategyPointSummonRange.StartTime;
	mt4HourStartTime.AddDateTime(0,0,0,4,0,0);
	ATUM_DATE_TIME mt4HourEndTime;
	mt4HourEndTime = m_StrategyPointSummonRange.EndTime;
	mt4HourEndTime.AddDateTime(0,0,0,-4,0,0);
	ATUM_DATE_TIME mtCurTime;
	mtCurTime.SetCurrentDateTime();
	mtCurTime.AddDateTime(0,0,0,i_nSummonHourGap,0,0);
	if(0 < mt4HourStartTime.GetTimeDiffTimeInSeconds(mtCurTime))
	{// 2007-03-05 by dhjin, 현재 시간이 시작 시간 보다 전이면 시작 시간을 현재 시간으로 설정한다.
		mtCurTime = mt4HourStartTime;
	}

	//////////////////////////////////////////////////////////////////////////
	// 2007-03-12 by dhjin, 
	// Range		: 현재 시간과 주기 마지막 시간과 차이나는 초를 구하여 랜덤 설정 변수(날짜)로 사용한다.
	// TimeRange	: 현재 시간과 주기 마지막 시간과 차이나는 초를 구하여 랜덤 설정 변수(초)로 사용한다.
	int Range = mt4HourEndTime.GetTimeDiffTimeInMinutes(mtCurTime);
	int TimeRange = 0;

	if(FALSE == this->CompareStrategyPointDay()
		|| 0 >= Range)
	{// 2007-03-07 by dhjin, 더 이상 할당할 요일이 없다. 다음 주기로 넘긴다.
		i_pStrategyPointSummonTimeInfo->SummonTime.AddDateTime(1,0,0, 0,0,0);
		QPARAM_UPDATE_STRATEGYPOINTSUMMONINFO_TIME *pQparam = new QPARAM_UPDATE_STRATEGYPOINTSUMMONINFO_TIME;
		pQparam->MapIndex		= i_pStrategyPointSummonTimeInfo->MapIndex;
		pQparam->SummonTime		= i_pStrategyPointSummonTimeInfo->SummonTime;
		pQparam->SummonAttribute	= FALSE;		// 2008-04-08 by dhjin, 소환 가능 시간 설정 - 
		this->m_pFieldIOCP14->m_pAtumDBManager->MakeAndEnqueueQuery(QT_UpdateStrategyPointSummonInfoBySummonTime, NULL, 0, pQparam);
		return;
	}
	
	// 2007-03-07 by dhjin, 요일에 할당한다.
	i_pStrategyPointSummonTimeInfo->SummonTime = mtCurTime;
	i_pStrategyPointSummonTimeInfo->SummonTime.AddDateTime(0,0,0, 0,(RANDI(1, Range)),0);
	ATUM_DATE_TIME tmMakeTime;
	tmMakeTime.SetDateTime(i_pStrategyPointSummonTimeInfo->SummonTime.Year, i_pStrategyPointSummonTimeInfo->SummonTime.Month, i_pStrategyPointSummonTimeInfo->SummonTime.Day, 0, 0, 0);

	//////////////////////////////////////////////////////////////////////////
	// 2007-03-12 by dhjin
	// tmCheckRangeTime : 설정 된 날짜가 어느 주기의 날인지 체크하여 한 날짜에 랜덤하게 설정 가능한 범위의 초를 설정하기 위한 변수.
	// tmIncreaseDay	: 루프 수를 줄이기 위한 변수 값 랜덤으로 설정이 실패하면 날자를 하나씩 더하여 소환 가능한 날자에 설정한다.
	int				tmCheckRangeTime = 1;		
	int				tmIncreaseDay = 0;		
	
	while(FALSE == this->CompareStrategyPointDayByOneDay(&tmMakeTime, &tmCheckRangeTime))
	{// 2007-03-09 by dhjin, 소환 가능한 날자가 있는지 체크하여 그 날자를 우선 설정한다.
		i_pStrategyPointSummonTimeInfo->SummonTime = mtCurTime;
		i_pStrategyPointSummonTimeInfo->SummonTime.AddDateTime(0,0,tmIncreaseDay, 0,0,0);
		tmIncreaseDay++;
		tmMakeTime.SetDateTime(i_pStrategyPointSummonTimeInfo->SummonTime.Year, i_pStrategyPointSummonTimeInfo->SummonTime.Month, i_pStrategyPointSummonTimeInfo->SummonTime.Day, 0, 0, 0);
	}
	this->IncreaseStrategyPointDay(&(i_pStrategyPointSummonTimeInfo->SummonTime));

	//////////////////////////////////////////////////////////////////////////
	// 날자 설정 값이 시작 날이거나 끝날이면 랜덤하게 설정할 시간 Range값을 수정한다.
	if(1 == m_mtvectStrategyPointDay.size())
	{
		TimeRange = Range;
		tmMakeTime = mtCurTime;
	}
	else if(1 == tmCheckRangeTime)
	{
		ATUM_DATE_TIME	tmPlusOneDay;
		tmPlusOneDay.SetDateTime(mtCurTime.Year, mtCurTime.Month, mtCurTime.Day+1,0,0,0);
		TimeRange = tmPlusOneDay.GetTimeDiffTimeInMinutes(mtCurTime);
		tmMakeTime = mtCurTime;
	}
	else if(m_mtvectStrategyPointDay.size() == tmCheckRangeTime)
	{
		TimeRange = mt4HourEndTime.GetTimeDiffTimeInMinutes(tmMakeTime);
	}
	else
	{
		TimeRange = 24 * 60;
	}

	BOOL SummonAttribute = FALSE;		// 2008-04-08 by dhjin, 소환 가능 시간 설정 - 

	// 2007-03-12 by dhjin, 시간을 요일별 금지 시간을 피해 랜덤하게 설정한다.
	if(FALSE == this->CheckStrategyPointNotSummonTime(&tmMakeTime, TimeRange, &SummonAttribute))
	{
		char szErr[1024];
		wsprintf(szErr, "CInflWarManager::Not_SummonTime Error");
		g_pFieldGlobal->WriteSystemLog(szErr);
		DbgOut("%s\rn", szErr);
		// 2007-03-29 by dhjin, 할당하지 못했다면 다음 주기에 소환한다.
		i_pStrategyPointSummonTimeInfo->SummonTime.AddDateTime(1,0,0, 0,0,0);
		QPARAM_UPDATE_STRATEGYPOINTSUMMONINFO_TIME *pQparam = new QPARAM_UPDATE_STRATEGYPOINTSUMMONINFO_TIME;
		pQparam->MapIndex		= i_pStrategyPointSummonTimeInfo->MapIndex;
		pQparam->SummonTime		= i_pStrategyPointSummonTimeInfo->SummonTime;
		pQparam->SummonAttribute	= FALSE;		// 2008-04-08 by dhjin, 소환 가능 시간 설정 - 
		this->m_pFieldIOCP14->m_pAtumDBManager->MakeAndEnqueueQuery(QT_UpdateStrategyPointSummonInfoBySummonTime, NULL, 0, pQparam);
		return;
	}

	// 2007-03-12 by dhjin, 설정된 소환시간 정보를 구조체에 할당한다.
	i_pStrategyPointSummonTimeInfo->SummonTime = tmMakeTime;
	// 2007-03-12 by dhjin, 설정된 소환시간 정보를 DB에 업데이트 한다.
	QPARAM_UPDATE_STRATEGYPOINTSUMMONINFO_TIME *pQparam = new QPARAM_UPDATE_STRATEGYPOINTSUMMONINFO_TIME;
	pQparam->MapIndex		= i_pStrategyPointSummonTimeInfo->MapIndex;
	pQparam->SummonTime		= tmMakeTime;
	pQparam->SummonAttribute= SummonAttribute;		// 2008-04-08 by dhjin, 소환 가능 시간 설정 - 
	this->m_pFieldIOCP14->m_pAtumDBManager->MakeAndEnqueueQuery(QT_UpdateStrategyPointSummonInfoBySummonTime, NULL, 0, pQparam);

}

BOOL CInflWarManager::CheckStrategyPointNotSummonTime(ATUM_DATE_TIME *i_ptmTime, int i_nRange, BOOL *o_bSummonAttribute)
{
	auto localTime = tm(*i_ptmTime);
	
	mt_auto_lock mta(this->GetStrategyPointNotSummonTime());
	vectSSTRATEGYPOINT_NOT_SUMMON_TIME::iterator itr = m_mtvectStrategyPointNotSummonTime.begin();
	while(itr != m_mtvectStrategyPointNotSummonTime.end())
	{
		// 2009-10-26 by cmkwon, 거점 로딩시 로그 추가 - 
		g_pFieldGlobal->WriteSystemLogEX(TRUE, "  [Notify] CInflWarManager::CheckStrategyPointNotSummonTime 10000 Day(%d) Time(%s ~ %s) \r\n"
			, itr->DayOfWeek, itr->StartTime.GetDateTimeString().GetBuffer(), itr->EndTime.GetDateTimeString().GetBuffer());

		if(localTime.tm_wday == itr->DayOfWeek)
		{
			*o_bSummonAttribute = itr->CheckSummon;		// 2008-04-08 by dhjin, 소환 가능 시간 설정 - 
			int tmStime = ((itr->StartTime.Hour) * 60 * 60) + ((itr->StartTime.Minute) * 60) + ((itr->StartTime.Second));
			int tmEtime = ((itr->EndTime.Hour) * 60 * 60) + ((itr->EndTime.Minute) * 60) + ((itr->EndTime.Second));
			ATUM_DATE_TIME	tmTime;
			tmTime	= *i_ptmTime;
			tmTime.AddDateTime(0,0,0, 0,(RANDI(0, i_nRange)),0);
			int tmTimeRange  = ((tmTime.Hour) * 60 * 60) + ((tmTime.Minute) * 60) + ((tmTime.Second));

			int nCount	= 0; // 2007-03-29 by dhjin, 무한 루프 탈출을 위한 변수.

			//////////////////////////////////////////////////////////////////////////
			// 2008-04-04 by dhjin, 소환 가능 시간 설정 - 
			if(itr->CheckSummon)
			{
				while(FALSE == ((tmStime <= tmTimeRange) && (tmEtime >= tmTimeRange)))
				{// 2008-04-04 by dhjin, 소환 가능 시간이면 다시 설정.
					if(30 <= nCount)
					{// 2007-03-29 by dhjin, 30번 랜덤으로 설정이 안되면 강제 셋팅
						if(23 > itr->EndTime.Hour)
						{// 2008-04-04 by dhjin, 소환 가능 마지막 시간이 23시 이전이면 셋팅한다.
							tmTime.SetDateTime(i_ptmTime->Year, i_ptmTime->Month, i_ptmTime->Day, itr->EndTime.Hour, itr->EndTime.Minute, itr->EndTime.Second);
							*i_ptmTime = tmTime;	// 2007-03-29 by dhjin, 랜덤으로 생성이 안되면 강제로 금지시간 이후에 생성 되도록 설정.
							return TRUE;
						}
						if(1 <= itr->StartTime.Hour)
						{// 2008-04-04 by dhjin, 소환 가능 시작 시간이 1시 이후이면 셋팅한다.
							tmTime.SetDateTime(i_ptmTime->Year, i_ptmTime->Month, i_ptmTime->Day, itr->StartTime.Hour, itr->StartTime.Minute, itr->StartTime.Second);
							*i_ptmTime = tmTime;	// 2007-03-29 by dhjin, 랜덤으로 생성이 안되면 강제로 금지시간 이후에 생성 되도록 설정.
							return TRUE;
						}
						return FALSE;	// 2007-03-29 by dhjin, 양쪽다 설정이 안된다면 다음 주기로 넘긴다.
					}
					tmTime	= *i_ptmTime;
					tmTime.AddDateTime(0,0,0, 0,(RANDI(0, i_nRange)),0);
					tmTimeRange  = ((tmTime.Hour) * 60 * 60) + ((tmTime.Minute) * 60) + ((tmTime.Second));
					nCount++;
				}
			}
			else
			{
				while((tmStime <= tmTimeRange) && (tmEtime >= tmTimeRange))
				{// 2007-03-09 by dhjin, 금지 시간이면 다시 설정.
					if(30 <= nCount)
					{// 2007-03-29 by dhjin, 30번 랜덤으로 설정이 안되면 강제 셋팅
						if(23 > itr->EndTime.Hour)
						{// 2007-03-29 by dhjin, 금지 마지막 시간이 23시 이전이면 셋팅한다.
							tmTime.SetDateTime(i_ptmTime->Year, i_ptmTime->Month, i_ptmTime->Day, itr->EndTime.Hour, itr->EndTime.Minute, itr->EndTime.Second);
							*i_ptmTime = tmTime;	// 2007-03-29 by dhjin, 랜덤으로 생성이 안되면 강제로 금지시간 이후에 생성 되도록 설정.
							return TRUE;
						}
						if(1 <= itr->StartTime.Hour)
						{// 2007-03-29 by dhjin, 금지 시작 시간이 1시 이후이면 셋팅한다.
							tmTime.SetDateTime(i_ptmTime->Year, i_ptmTime->Month, i_ptmTime->Day, itr->StartTime.Hour, itr->StartTime.Minute, itr->StartTime.Second);
							*i_ptmTime = tmTime;	// 2007-03-29 by dhjin, 랜덤으로 생성이 안되면 강제로 금지시간 이후에 생성 되도록 설정.
							return TRUE;
						}
						return FALSE;	// 2007-03-29 by dhjin, 양쪽다 설정이 안된다면 다음 주기로 넘긴다.
					}
					tmTime	= *i_ptmTime;
					tmTime.AddDateTime(0,0,0, 0,(RANDI(0, i_nRange)),0);
					tmTimeRange  = ((tmTime.Hour) * 60 * 60) + ((tmTime.Minute) * 60) + ((tmTime.Second));
					nCount++;
				}
			}

			*i_ptmTime = tmTime;
			return TRUE;
		}
		itr++;
	}

	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CInflWarManager::InitStrategyPointDay(ATUM_DATE_TIME *i_pSTime, ATUM_DATE_TIME *i_pETime)
/// \brief		전략포인트 생성 가능한 날짜별 Count 정보 초기화
/// \author		dhjin
/// \date		2007-03-12 ~ 2007-03-12
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CInflWarManager::InitStrategyPointDay(ATUM_DATE_TIME *i_pSTime, ATUM_DATE_TIME *i_pETime)
{
	ATUM_DATE_TIME tmSTime;
	tmSTime.SetDateTime(i_pSTime->Year, i_pSTime->Month, i_pSTime->Day,0,0,0);
	ATUM_DATE_TIME StartTime;	// 2007-03-29 by dhjin, 시작날 비교값
	StartTime	= tmSTime;
	ATUM_DATE_TIME EndTime;		// 2007-03-29 by dhjin, 끝날 비교값
	EndTime.SetDateTime(i_pETime->Year, i_pETime->Month, i_pETime->Day,0,0,0);

	SSTRATEGYPOINT_DAY	tmStrategyPointDay;
	util::zero(&tmStrategyPointDay, sizeof(SSTRATEGYPOINT_DAY));
	m_mtvectStrategyPointDay.clear();
	
	//////////////////////////////////////////////////////////////////////////
	// 2007-03-29 by dhjin, 금지시간으로 인해 설정되면 안되는 시작, 마지막날 값 없애기
	auto localStTime = tm(*i_pSTime);
	
	ATUM_DATE_TIME tmSNotSTime;
	ATUM_DATE_TIME tmSNotETime;

	//////////////////////////////////////////////////////////////////////////
	// 2008-04-04 by dhjin, 소환 가능 시간 설정 - 
	BOOL CheckSummonForStart	= FALSE;
	BOOL CheckSummonForEnd		= FALSE;

	mt_auto_lock mta(this->GetStrategyPointNotSummonTime());
	vectSSTRATEGYPOINT_NOT_SUMMON_TIME::iterator itrSt = m_mtvectStrategyPointNotSummonTime.begin();
	while(itrSt != m_mtvectStrategyPointNotSummonTime.end())
	{// 2007-03-30 by dhjin, 첫날 금지시간 얻어오기
		if(localStTime.tm_wday == itrSt->DayOfWeek)
		{
			tmSNotSTime.SetDateTime(i_pSTime->Year, i_pSTime->Month, i_pSTime->Day, itrSt->StartTime.Hour, itrSt->StartTime.Minute, itrSt->StartTime.Second);
			tmSNotETime.SetDateTime(i_pSTime->Year, i_pSTime->Month, i_pSTime->Day, itrSt->EndTime.Hour, itrSt->EndTime.Minute, itrSt->EndTime.Second);
			if(itrSt->CheckSummon)
			{// 2008-04-04 by dhjin, 소환 가능 시간 설정 - 
				CheckSummonForStart = TRUE;
			}

			break;
		}
		itrSt++;
	}

	auto localEtTime = tm(*i_pETime);

	ATUM_DATE_TIME tmENotSTime;
	ATUM_DATE_TIME tmENotETime;
	vectSSTRATEGYPOINT_NOT_SUMMON_TIME::iterator itrEt = m_mtvectStrategyPointNotSummonTime.begin();
	while(itrEt != m_mtvectStrategyPointNotSummonTime.end())
	{// 2007-03-30 by dhjin, 마지막날 금지시간 얻어오기
		if(localEtTime.tm_wday == itrEt->DayOfWeek)
		{
			tmENotSTime.SetDateTime(i_pETime->Year, i_pETime->Month, i_pETime->Day, itrEt->StartTime.Hour, itrEt->StartTime.Minute, itrEt->StartTime.Second);
			tmENotETime.SetDateTime(i_pETime->Year, i_pETime->Month, i_pETime->Day, itrEt->EndTime.Hour, itrEt->EndTime.Minute, itrEt->EndTime.Second);
			if(itrEt->CheckSummon)
			{// 2008-04-04 by dhjin, 소환 가능 시간 설정 - 
				CheckSummonForEnd = TRUE;
			}
			
			break;
		}
		itrEt++;
	}
	mta.auto_unlock_cancel();
	
	while(tmSTime <= *i_pETime)
	{
		if(0 == tmSTime.GetTimeDiffTimeInHours(StartTime))
		{// 2007-03-29 by dhjin, 시작날 비교해서 금지시간으로 인해 설정이 불가능하면 다음 날짜로
			//////////////////////////////////////////////////////////////////////////
			// 2008-04-04 by dhjin, 소환 가능 시간 설정 - 
			if(CheckSummonForStart)
			{
				if(0 > i_pSTime->GetTimeDiffTimeInSeconds(tmSNotSTime)
					|| 0 < i_pSTime->GetTimeDiffTimeInSeconds(tmSNotETime)
					|| 23 <= tmSNotETime.Hour)
				{
					tmSTime.AddDateTime(0,0,1,0,0,0);
					i_pSTime->SetDateTime(tmSTime.Year, tmSTime.Month, tmSTime.Day, 0, 0, 1);
					continue;
				}
			}
			else
			{
				if(0 < i_pSTime->GetTimeDiffTimeInSeconds(tmSNotSTime)
					&& 0 > i_pSTime->GetTimeDiffTimeInSeconds(tmSNotETime)
					&& 23 <= tmSNotETime.Hour)
				{
					tmSTime.AddDateTime(0,0,1,0,0,0);
					i_pSTime->SetDateTime(tmSTime.Year, tmSTime.Month, tmSTime.Day, 0, 0, 1);
					continue;
				}
			}

		}

		if(0 == tmSTime.GetTimeDiffTimeInHours(EndTime))
		{// 2007-03-29 by dhjin, 끝날 비교해서 금지시간으로 인해 설정이 불가능하면 다음 날짜로
			//////////////////////////////////////////////////////////////////////////
			// 2008-04-04 by dhjin, 소환 가능 시간 설정 - 
			if(CheckSummonForEnd)
			{
				if(0 > i_pETime->GetTimeDiffTimeInSeconds(tmENotSTime)
					|| 0 < i_pETime->GetTimeDiffTimeInSeconds(tmENotETime)
					|| 1 >= tmENotSTime.Hour)
				{
					tmSTime.AddDateTime(0,0,1,0,0,0);
					continue;
				}
			
			}
			else
			{
				if(0 < i_pETime->GetTimeDiffTimeInSeconds(tmENotSTime)
					&& 0 > i_pETime->GetTimeDiffTimeInSeconds(tmENotETime)
					&& 1 >= tmENotSTime.Hour)
				{
					tmSTime.AddDateTime(0,0,1,0,0,0);
					continue;
				}
			}

		}

		tmStrategyPointDay.Year		= tmSTime.Year;
		tmStrategyPointDay.Month	= tmSTime.Month;
		tmStrategyPointDay.Day		= tmSTime.Day;
		tmStrategyPointDay.MaxCount = 0;
		m_mtvectStrategyPointDay.push_back(tmStrategyPointDay);

		util::zero(&tmStrategyPointDay, sizeof(SSTRATEGYPOINT_DAY));
		tmSTime.AddDateTime(0,0,1,0,0,0);
	}
	
	mtvectSSTRATEGYPOINT_SUMMONTIME_INFO::iterator itr = m_mtvectStrategyPointSummonTimeInfo.begin();
	while(itr != m_mtvectStrategyPointSummonTimeInfo.end())
	{// 2007-03-08 by dhjin, 전략포인트 소환 시간값을 설정하기 전에 소환되면 안되는 Max값을 체크하기 위해 미리 소환된 정보를 요일에 업데이트 한다.
		if(0 == itr->SummonCount)
		{
			ATUM_DATE_TIME tmTime;
			tmTime.SetDateTime(itr->SummonTime.Year, itr->SummonTime.Month, itr->SummonTime.Day, 0, 0, 0);
			ATUM_DATE_TIME tmITime;
			tmITime.SetDateTime(i_pSTime->Year, i_pSTime->Month, i_pSTime->Day, 0, 0, 0);
			if(0 == tmITime.GetTimeDiffTimeInHours(tmTime))
			{
				this->IncreaseStrategyPointDay(&tmTime);
			}
		}
		itr++;
	}

}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CInflWarManager::CompareStrategyPointDay()
/// \brief		전략포인트 생성이 가능한 날짜가 있는 지 체크한다.
/// \author		dhjin
/// \date		2007-03-12 ~ 2007-03-12
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CInflWarManager::CompareStrategyPointDay()
{
	this->CurrentStrategyPointDay();
	
	mtvectSSTRATEGYPOINT_DAY::iterator itr = m_mtvectStrategyPointDay.begin();
	while(itr != m_mtvectStrategyPointDay.end())
	{
		if(itr->MaxCount < m_StrategyPointSummonRange.MaxSummonCountDaily)
		{// 2007-03-08 by dhjin, 할당할 수 있는 요일이 있다.
			return TRUE;
		}
		
		itr++;
	}

	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CInflWarManager::CurrentStrategyPointDay()
/// \brief		m_mtvectStrategyPointDay구조체에서 지난 날짜는 불필요한 정보이므로 삭제한다.
/// \author		dhjin
/// \date		2007-03-12 ~ 2007-03-12
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CInflWarManager::CurrentStrategyPointDay()
{
	ATUM_DATE_TIME tmCurTime;
	tmCurTime.SetCurrentDateTime();
	tmCurTime.Hour = 0;
	tmCurTime.Minute = 0;
	tmCurTime.Second = 0;

	mtvectSSTRATEGYPOINT_DAY::iterator itr = m_mtvectStrategyPointDay.begin();
	while(itr != m_mtvectStrategyPointDay.end())
	{
		ATUM_DATE_TIME tmTime;
		tmTime.SetDateTime(itr->Year, itr->Month, itr->Day, 0, 0, 0);

		if(0 > tmTime.GetTimeDiffTimeInSeconds(tmCurTime))
		{// 2007-03-08 by dhjin, 현재 시간보다 전에 요일이면 삭제한다.
			itr = m_mtvectStrategyPointDay.erase(itr);
			continue;
		}
		itr++;
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CInflWarManager::IncreaseStrategyPointDay(ATUM_DATE_TIME *i_pTime)
/// \brief		전략포인트가 설정된 날짜에 소환 count값을 1 증가시킨다.
/// \author		dhjin
/// \date		2007-03-12 ~ 2007-03-12
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CInflWarManager::IncreaseStrategyPointDay(ATUM_DATE_TIME *i_pTime)
{
	ATUM_DATE_TIME tmITime;
	tmITime.SetDateTime(i_pTime->Year, i_pTime->Month, i_pTime->Day, 0, 0, 0);

	mtvectSSTRATEGYPOINT_DAY::iterator itr = m_mtvectStrategyPointDay.begin();
	while(itr != m_mtvectStrategyPointDay.end())
	{
		ATUM_DATE_TIME tmTime;
		tmTime.SetDateTime(itr->Year, itr->Month, itr->Day, 0, 0, 0);
		if(0 == tmITime.GetTimeDiffTimeInHours(tmTime))
		{
			itr->MaxCount += 1;
			return;
		}
		itr++;
	}	
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CInflWarManager::CompareStrategyPointDayByOneDay(ATUM_DATE_TIME *i_pTime, int *o_nRangeTime)
/// \brief		전략포인트가 생성 가능한지 한 날짜를 입력받아 체크하여 리턴한다.
/// \author		dhjin
/// \date		2007-03-12 ~ 2007-03-12
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CInflWarManager::CompareStrategyPointDayByOneDay(ATUM_DATE_TIME *i_pTime, int *o_nRangeTime)
{
	*o_nRangeTime = 1;
	mtvectSSTRATEGYPOINT_DAY::iterator itr = m_mtvectStrategyPointDay.begin();
	while(itr != m_mtvectStrategyPointDay.end())
	{
		// 2009-10-26 by cmkwon, 거점 로딩시 로그 추가 - 
		g_pFieldGlobal->WriteSystemLogEX(TRUE, "  [Notify] CInflWarManager::CompareStrategyPointDayByOneDay# 10000 itr(%d-%d-%d) MaxCount(%d) i_ptime(%s) RangeTime(%d)\r\n"
			, itr->Year, itr->Month, itr->Day, itr->MaxCount, i_pTime->GetDateTimeString().GetBuffer(), *o_nRangeTime);

		ATUM_DATE_TIME tmTime;
		tmTime.SetDateTime(itr->Year, itr->Month, itr->Day, 0, 0, 0);
		if(0 == i_pTime->GetTimeDiffTimeInHours(tmTime))
		{
			if(m_StrategyPointSummonRange.MaxSummonCountDaily > itr->MaxCount)
			{
				return TRUE;
			}
			return FALSE;
		}
		(*o_nRangeTime)++;
		itr++;
	}

	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CInflWarManager::CheckSummonBossAndStrategyPoint()
/// \brief		
/// \author		dhjin
/// \date		2007-04-26 ~ 2007-04-26
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CInflWarManager::CheckSummonBossAndStrategyPoint()
{
	if(TRUE == m_SummonVCNBossMonsterInfo.bSummonBossMonster
		|| TRUE == m_SummonANIBossMonsterInfo.bSummonBossMonster
		|| FALSE == m_mtvectSummonStrategyPointInfo.empty())
	{
		return TRUE;
	}
	
	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CInflWarManager::CheckSummonBoss()
/// \brief		모선전 정보 표시 기획안 - 
/// \author		dhjin
/// \date		2008-03-28 ~ 2008-03-28
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CInflWarManager::CheckSummonBoss() 
{
	if(TRUE == m_SummonVCNBossMonsterInfo.bSummonBossMonster
		|| TRUE == m_SummonANIBossMonsterInfo.bSummonBossMonster)
	{
		return TRUE;
	}
	
	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CInflWarManager::CheckSummonOnlyStrategyPoint()
/// \brief		전쟁 보상 추가안
/// \author		dhjin
/// \date		2008-12-24 ~ 2008-12-24
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CInflWarManager::CheckSummonOnlyStrategyPoint()
{
	if(FALSE == m_SummonVCNBossMonsterInfo.bSummonBossMonster
		&& FALSE == m_SummonANIBossMonsterInfo.bSummonBossMonster
		&& FALSE == m_mtvectSummonStrategyPointInfo.empty())
	{
		return TRUE;
	}

	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CInflWarManager::CheckBossMonsterSummoningByAttackInfl(byte i_byAttackerInfluence)
/// \brief		공격자의 세력으로 상대 세력 전함이 소환 되어 있는지 체크
/// \author		dhjin
/// \date		2007-09-05 ~ 2007-09-05
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CInflWarManager::CheckBossMonsterSummoningByAttackInfl(byte i_byAttackerInfluence)
{
	if(INFLUENCE_TYPE_VCN == i_byAttackerInfluence)
	{
		return this->CheckANIBossMonsterSummoning();
	}

	if(INFLUENCE_TYPE_ANI == i_byAttackerInfluence)
	{
		return this->CheckVCNBossMonsterSummoning();
	}

	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CInflWarManager::CheckBossMonsterSummoningByMapInfluence(INT i_nMapInfluenceType)
/// \brief		모선전 정보 표시 기획안 - 맵세력으로 모선이 소환 되어 있는지 체크
/// \author		dhjin
/// \date		2008-03-27 ~ 2008-03-27
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CInflWarManager::CheckBossMonsterSummoningByMapInfluence(INT i_nMapInfluenceType)
{
	if(IS_MAP_INFLUENCE_VCN(i_nMapInfluenceType))
	{
		return this->CheckVCNBossMonsterSummoning();
	}

	if(IS_MAP_INFLUENCE_ANI(i_nMapInfluenceType))
	{
		return this->CheckANIBossMonsterSummoning();
	}

	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CInflWarManager::CheckVCNBossMonsterSummoning()
/// \brief		바이제니유 전함 소환 되어 있는지 체크
/// \author		dhjin
/// \date		2007-09-05 ~ 2007-09-05
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CInflWarManager::CheckVCNBossMonsterSummoning()
{
	if(m_SummonVCNBossMonsterInfo.bSummonBossMonster)
	{
		return TRUE;
	}
	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CInflWarManager::CheckANIBossMonsterSummoning()
/// \brief		알링턴 전함 소환 되어 있는지 체크
/// \author		dhjin
/// \date		2007-09-05 ~ 2007-09-05
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CInflWarManager::CheckANIBossMonsterSummoning()
{
	if(m_SummonANIBossMonsterInfo.bSummonBossMonster)
	{
		return TRUE;
	}
	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CInflWarManager::GameStartSetLeader_SubLeaderGuildUID()
/// \brief		지도자, 여단장인 부지도자 여단 UID 설정  
/// \author		dhjin
/// \date		2007-09-07 ~ 2007-10-06
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CInflWarManager::GameStartSetLeader_SubLeaderGuildUID()
{
	CInflWarData *pInflWarData = GetInflWarDataByInflType(INFLUENCE_TYPE_VCN);
	if(NULL == pInflWarData)
	{
		return;
	}
	// 2007-10-06 by dhjin, 바이제니유 지도자 여단 설정 
	m_VCNLeaderGuildUID = m_pFieldIOCP14->m_GuildWarManager.GetLeaderGuildUID(pInflWarData->InflLeaderCharacterUID);
	// 2007-10-06 by dhjin, 바이제니유 부지도자1 여단 설정
	if(FALSE == m_pFieldIOCP14->m_GuildWarManager.CheckGuildCommander(pInflWarData->InflSub1LeaderCharacterUID, &m_VCNSubLeader1GuildUID))
	{
		m_VCNSubLeader1GuildUID = 0;
	}
	// 2007-10-06 by dhjin, 바이제니유 부지도자2 여단 설정
	if(FALSE == m_pFieldIOCP14->m_GuildWarManager.CheckGuildCommander(pInflWarData->InflSub2LeaderCharacterUID, &m_VCNSubLeader2GuildUID))
	{
		m_VCNSubLeader2GuildUID = 0;
	}

	pInflWarData = GetInflWarDataByInflType(INFLUENCE_TYPE_ANI);
	if(NULL == pInflWarData)
	{
		return;
	}
	// 2007-10-06 by dhjin, 알링턴 지도자 여단 설정 
	m_ANILeaderGuildUID = m_pFieldIOCP14->m_GuildWarManager.GetLeaderGuildUID(pInflWarData->InflLeaderCharacterUID);
	// 2007-10-06 by dhjin, 알링턴 부지도자1 여단 설정
	if(FALSE == m_pFieldIOCP14->m_GuildWarManager.CheckGuildCommander(pInflWarData->InflSub1LeaderCharacterUID, &m_ANISubLeader1GuildUID))
	{
		m_ANISubLeader1GuildUID = 0;
	}
	// 2007-10-06 by dhjin, 알링턴 부지도자2 여단 설정
	if(FALSE == m_pFieldIOCP14->m_GuildWarManager.CheckGuildCommander(pInflWarData->InflSub1LeaderCharacterUID, &m_ANISubLeader2GuildUID))
	{
		m_ANISubLeader2GuildUID = 0;
	}

}

///////////////////////////////////////////////////////////////////////////////
/// \fn			UID32_t CInflWarManager::GetVCNLeaderGuildUID()
/// \brief		바이제니유 지도자 여단 UID
/// \author		dhjin
/// \date		2007-09-07 ~ 2007-09-07
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
UID32_t CInflWarManager::GetVCNLeaderGuildUID()
{
	return m_VCNLeaderGuildUID;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			UID32_t CInflWarManager::GetANILeaderGuildUID()
/// \brief		알링턴 지도자 여단 UID
/// \author		dhjin
/// \date		2007-09-07 ~ 2007-09-07
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
UID32_t CInflWarManager::GetANILeaderGuildUID()
{
	return m_ANILeaderGuildUID;
}

UID32_t CInflWarManager::GetVCNSubLeader1GuildUID()
{
	return m_VCNSubLeader1GuildUID;
}

UID32_t CInflWarManager::GetVCNSubLeader2GuildUID()
{
	return m_VCNSubLeader2GuildUID;
}

UID32_t CInflWarManager::GetANISubLeader1GuildUID()
{
	return m_ANISubLeader1GuildUID;
}

UID32_t CInflWarManager::GetANISubLeader2GuildUID()
{
	return m_ANISubLeader2GuildUID;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CInflWarManager::SetSubLeaderGuildUID(int i_nSubLeaderRank, byte i_byInfluence, UID32_t i_nCharacterUID)
/// \brief		여단장인 부지도자 여단 UID 설정
/// \author		dhjin
/// \date		2007-10-06 ~ 2007-10-06
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CInflWarManager::SetSubLeaderGuildUID(int i_nSubLeaderRank, byte i_byInfluence, UID32_t i_nCharacterUID)
{
	UID32_t tmSubLeaderGuildUID = 0;
	if(FALSE == m_pFieldIOCP14->m_GuildWarManager.CheckGuildCommander(i_nCharacterUID, &tmSubLeaderGuildUID))
	{// 2007-10-06 by dhjin, 여단장인지 체크한다.
		return;
	}	

	if(1 == i_nSubLeaderRank
		&& INFLUENCE_TYPE_VCN == i_byInfluence)
	{
		m_VCNSubLeader1GuildUID = tmSubLeaderGuildUID;
	}
	else if(2 == i_nSubLeaderRank
		&& INFLUENCE_TYPE_VCN == i_byInfluence)
	{
		m_VCNSubLeader2GuildUID = tmSubLeaderGuildUID;
	}
	else if(1 == i_nSubLeaderRank
		&& INFLUENCE_TYPE_ANI == i_byInfluence)
	{
		m_ANISubLeader1GuildUID = tmSubLeaderGuildUID;
	}
	else if(2 == i_nSubLeaderRank
		&& INFLUENCE_TYPE_ANI == i_byInfluence)
	{
		m_ANISubLeader2GuildUID = tmSubLeaderGuildUID;
	}	
	
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CInflWarManager::CheckLeader_SubLeaderGuildUID(UID32_t i_nGuildUID)
/// \brief		지도자 여단인지 체크
/// \author		dhjin
/// \date		2007-09-07 ~ 2007-09-07
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CInflWarManager::CheckLeader_SubLeaderGuildUID(UID32_t i_nGuildUID)
{
	if(i_nGuildUID == GetVCNLeaderGuildUID()
		|| i_nGuildUID == GetANILeaderGuildUID()
		|| i_nGuildUID == GetVCNSubLeader1GuildUID()
		|| i_nGuildUID == GetVCNSubLeader2GuildUID()
		|| i_nGuildUID == GetANISubLeader1GuildUID()
		|| i_nGuildUID == GetANISubLeader2GuildUID())
	{
		return TRUE;
	}
	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CInflWarManager::InsertTelePortInfoBuing(STELEPORT_INFO_BUILDING * i_pTeleportInfoBuilding)
/// \brief		텔레포트를 빌딩 진행중으로 변경하면 정보를 저장하여 빌딩 시간을 체크한다.
/// \author		dhjin
/// \date		2007-09-15 ~ 2007-09-15
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CInflWarManager::InsertTelePortInfoBuing(STELEPORT_INFO_BUILDING * i_pTeleportInfoBuilding)
{
	mt_auto_lock mta(&m_mtvectTeleportInfoBuilding);

	for(int i=0; i < m_mtvectTeleportInfoBuilding.size(); i++)
	{
		if(m_mtvectTeleportInfoBuilding[i].MapIndex == i_pTeleportInfoBuilding->MapIndex)
		{
			return FALSE;
		}
	}

	m_mtvectTeleportInfoBuilding.push_back(*i_pTeleportInfoBuilding);
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CInflWarManager::TelePortInfoBuingByTick(ATUM_DATE_TIME * i_pCurrnetTime)
/// \brief		텔레포트 정보 1분마다 체크하여 정보 변경
/// \author		dhjin
/// \date		2007-09-15 ~ 2007-09-15
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CInflWarManager::TelePortInfoBuingByTick(ATUM_DATE_TIME * i_pCurrnetTime)
{
	mt_auto_lock mta(&m_mtvectTeleportInfoBuilding);

	mtvectSTELEPORT_INFO_BUILDING::iterator itr = m_mtvectTeleportInfoBuilding.begin();
	while(itr != m_mtvectTeleportInfoBuilding.end())
	{
		if(itr->bBuilded)
		{
			itr++;
			continue;
		}

		if(TELEPORT_BUILDING_TIME_SECOND <= i_pCurrnetTime->GetTimeDiffTimeInSeconds(itr->BuildStartTime))
		{// 2007-09-15 by dhjin, 빌딩이 완성되었다. 이전에 정보는 삭제한다. (텔레포트 소환은 항상 하나를 유지한다.)
			CFieldMapChannel *pFMChann = m_pFieldIOCP14->GetFieldMapChannelByMapChannelIndex(MAP_CHANNEL_INDEX(itr->MapIndex, 0));
			if(NULL == pFMChann)
			{
				itr = m_mtvectTeleportInfoBuilding.erase(itr);
				continue;
			}

			// 2007-09-20 by cmkwon, 텔레포트 소환 관련 수정
			if(TELEPORT_STATE_BUILDING != pFMChann->GetTelePortState())
			{
				itr = m_mtvectTeleportInfoBuilding.erase(itr);
				continue;
			}

			// 2007-09-16 by dhjin, 몬스터 BODYCON값 변경
			CFieldMonster	*pFMon = pFMChann->GetFieldMonster(pFMChann->m_ChangeBodyConMonsterIndex, 0);
			if(NULL == pFMon
				|| FALSE == pFMon->IsValidMonster()
				|| FALSE == IS_TELEPORT_MONSTER(pFMon->MonsterInfoPtr->Belligerence))
			{
				itr = m_mtvectTeleportInfoBuilding.erase(itr);
				continue;
			}

			MapIndex_t BeforeTeleportMapIdx = 0;
			if(IS_MAP_INFLUENCE_VCN(itr->MapInfluence))
			{
				BeforeTeleportMapIdx	= m_ANITeleportMapIndex;
				m_ANITeleportMapIndex	= itr->MapIndex;		// 2007-09-16 by dhjin, 빌딩 완료 맵 번호
			}
			else
			{
				BeforeTeleportMapIdx	= m_VCNTeleportMapIndex;
				m_VCNTeleportMapIndex	= itr->MapIndex;		// 2007-09-16 by dhjin, 빌딩 완료 맵 번호
			}

			if(0 != BeforeTeleportMapIdx)
			{
				///////////////////////////////////////////////////////////////////////////////
				// 2007-09-20 by cmkwon, 텔레포트 수정 - 기존 텔레포트 처리
				CFieldMapChannel *pBeforeTeleportFMChann = m_pFieldIOCP14->GetFieldMapChannelByMapChannelIndex(MAP_CHANNEL_INDEX(BeforeTeleportMapIdx, 0));
				if(pBeforeTeleportFMChann)
				{
					CFieldMonster	*pBeforeTeleportFMon = pBeforeTeleportFMChann->GetFieldMonster(pBeforeTeleportFMChann->m_ChangeBodyConMonsterIndex, 0);
					if(pBeforeTeleportFMon
						&& pBeforeTeleportFMon->IsValidMonster()
						&& IS_TELEPORT_MONSTER(pBeforeTeleportFMon->MonsterInfoPtr->Belligerence))
					{
						CLEAR_BODYCON_BIT(pBeforeTeleportFMon->BodyCondition, BODYCON_MON_BUILD_MASK);		// 2007-09-20 by cmkwon, 텔레포트 수정 - 
						pBeforeTeleportFMChann->FieldSendMonsterChangeInfo(pBeforeTeleportFMon, T_FN_MONSTER_CHANGE_BODYCONDITION);
						pBeforeTeleportFMChann->FieldSendMonsterChangeInfo(pBeforeTeleportFMon, T_FC_MONSTER_CHANGE_BODYCONDITION);

						pBeforeTeleportFMChann->SetTelePortState(TELEPORT_STATE_READY);

						//////////////////////////////////////////////////////////////////////////
						// 2008-03-28 by dhjin, 모선전 정보 표시 기획안 - 
						this->SetMSWarInfo(pBeforeTeleportFMChann->GetMapChannelIndex().MapIndex, T_MSWARINFO_MODIFY_CREATE_TELEPORT, MS_WAR_HP_RATE, TELEPORT_STATE_READY, pBeforeTeleportFMon->MonsterInfoPtr->Belligerence);
					}
					else
					{
						pBeforeTeleportFMChann->SetTelePortState(TELEPORT_STATE_NONE);
					}

					INIT_MSG_WITH_BUFFER(MSG_FC_EVENT_TELEPORT_DESTROY, T_FC_EVENT_TELEPORT_DESTROY, pSTeleport, SendBuf);
					pSTeleport->MapIndex					= BeforeTeleportMapIdx;
					pSTeleport->bInactivateByOtherTeleport	= TRUE;
					m_pFieldIOCP14->SendMessageToAllClients(SendBuf, MSG_SIZE(MSG_FC_EVENT_TELEPORT_DESTROY),INFLUENCE_TYPE_VCN|INFLUENCE_TYPE_ANI);
				}
			}

			pFMChann->SetTelePortState(TELEPORT_STATE_COMPLETE);
			itr->bBuilded	= TRUE;								// 2007-09-16 by dhjin, 빌딩 완료 플래그 

			//////////////////////////////////////////////////////////////////////////
			// 2008-03-28 by dhjin, 모선전 정보 표시 기획안 -
			// 2010. 05. 31 by hsLee 인피니티 필드 2차 난이도 조절. (오브젝트 몬스터 밸런스 적용 문제 수정.)
			//this->SetMSWarInfo(itr->MapIndex, T_MSWARINFO_MODIFY_CHANGE_TELEPORT_STATE, CalcGradeByRate(pFMon->MonsterInfoPtr->MonsterHP, pFMon->CurrentHP, MS_WAR_HP_RATE), TELEPORT_STATE_COMPLETE, pFMon->MonsterInfoPtr->Belligerence);
			this->SetMSWarInfo(itr->MapIndex, T_MSWARINFO_MODIFY_CHANGE_TELEPORT_STATE, CalcGradeByRate(pFMon->MonsterInfoExtend.fMaxHP, pFMon->CurrentHP, MS_WAR_HP_RATE), TELEPORT_STATE_COMPLETE, pFMon->MonsterInfoPtr->Belligerence);
			// End 2010. 05. 31 by hsLee 인피니티 필드 2차 난이도 조절. (오브젝트 몬스터 밸런스 적용 문제 수정.)

			CLEAR_BODYCON_BIT(pFMon->BodyCondition, BODYCON_MON_BUILDING);			// 2007-09-20 by cmkwon, 텔레포트 수정 - 
			SET_BODYCON_BIT(pFMon->BodyCondition,  BODYCON_MON_BUILDED);
			pFMChann->FieldSendMonsterChangeInfo(pFMon, T_FN_MONSTER_CHANGE_BODYCONDITION);
			pFMChann->FieldSendMonsterChangeInfo(pFMon, T_FC_MONSTER_CHANGE_BODYCONDITION);

			INIT_MSG_WITH_BUFFER(MSG_FC_EVENT_TELEPORT_BUILDCOMPLETE, T_FC_EVENT_TELEPORT_BUILDCOMPLETE, pSTeleport, SendBuf);
			pSTeleport->MapIndex			= itr->MapIndex;
			m_pFieldIOCP14->SendMessageToAllClients(SendBuf, MSG_SIZE(MSG_FC_EVENT_TELEPORT_BUILDCOMPLETE),INFLUENCE_TYPE_VCN|INFLUENCE_TYPE_ANI);
		}
		itr++;
	}

	// 2007-09-16 by dhjin, 텔레포트 몬스터 삭제할 부분이 있으면 삭제한다.
	itr = m_mtvectTeleportInfoBuilding.begin();
	while(itr != m_mtvectTeleportInfoBuilding.end())
	{
		if(FALSE == itr->bBuilded)
		{
			itr++;
			continue;
		}

		if( (IS_MAP_INFLUENCE_VCN(itr->MapInfluence) && m_ANITeleportMapIndex != itr->MapIndex)
				|| (IS_MAP_INFLUENCE_ANI(itr->MapInfluence) && m_VCNTeleportMapIndex != itr->MapIndex) )
		{// 2007-09-16 by dhjin, 이미 설치된 텔레포트 몬스터가 삭제 대상이면 삭제한다.
			
			itr = m_mtvectTeleportInfoBuilding.erase(itr);
			continue;
		}		
		itr++;
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CInflWarManager::ClearTelePortByInfluenceWarEnd(BYTE i_byDefenceCharInfluence)
/// \brief		세력전 종료로 텔레포트 정보를 초기화한다. 
///				// 2009-03-25 by cmkwon, 모선전시 텔레포트 초기화 관련 버그 수정 - 변수명 i_byDefenceCharInfluence로 변경
/// \author		dhjin
/// \date		2007-09-16 ~ 2007-09-16
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CInflWarManager::ClearTelePortByInfluenceWarEnd(BYTE i_byDefenceCharInfluence)
{
	mt_auto_lock mta(&m_mtvectTeleportInfoBuilding);

// 2009-03-25 by cmkwon, 모선전시 텔레포트 초기화 관련 버그 수정 - 아래와 같이 수정 함
// 	if(IS_VCN_INFLUENCE_TYPE(i_byCharInfluence))
// 	{
// 		m_VCNTeleportMapIndex	= 0;
// 	}
// 	if(IS_ANI_INFLUENCE_TYPE(i_byCharInfluence))
// 	{
// 		m_ANITeleportMapIndex	= 0;
// 	}
// 
// 	mtvectSTELEPORT_INFO_BUILDING::iterator itr = m_mtvectTeleportInfoBuilding.begin();
// 	while(itr != m_mtvectTeleportInfoBuilding.end())
// 	{	
// 		if(IS_SAME_CHARACTER_8_MAP_INFLUENCE(i_byCharInfluence, itr->MapInfluence))
// 		{
// 			CFieldMapChannel *pFMChann = m_pFieldIOCP14->GetFieldMapChannelByMapChannelIndex(MAP_CHANNEL_INDEX(itr->MapIndex, 0));
// 			if(NULL == pFMChann)
// 			{
// 				itr = m_mtvectTeleportInfoBuilding.erase(itr);
// 				continue;
// 			}						
// 
// 			pFMChann->SetTelePortState(TELEPORT_STATE_NOT_SUMMON);
// 			pFMChann->SetTelePortSummonFirst(FALSE);
// 			pFMChann->m_ChangeBodyConMonsterIndex = 0;
// 		}		
// 		itr++;
// 	}
// 
// 	if(IS_VCN_INFLUENCE_TYPE(i_byCharInfluence))
// 	{
// 		m_VCNTeleportMapIndex	= 0;
// 	}
// 	if(IS_ANI_INFLUENCE_TYPE(i_byCharInfluence))
// 	{
// 		m_ANITeleportMapIndex	= 0;
// 	}
	///////////////////////////////////////////////////////////////////////////////
	// 2009-03-25 by cmkwon, 모선전시 텔레포트 초기화 관련 버그 수정 - 
	if(IS_VCN_INFLUENCE_TYPE(i_byDefenceCharInfluence))
	{
 		m_ANITeleportMapIndex	= 0;
	}
	else
	{
		m_VCNTeleportMapIndex	= 0;
	}
	mta.auto_unlock_cancel();

	///////////////////////////////////////////////////////////////////////////////	
    // 2009-03-25 by cmkwon, 모선전시 텔레포트 초기화 관련 버그 수정 - 아래와 같이 해당 방어 세력의 맵채널에 텔레포트 관련 설정을 초기화 한다.
	mt_auto_lock mtSP(this->GetStrategyPointSummonTimeInfo());
	mtvectSSTRATEGYPOINT_SUMMONTIME_INFO::iterator itrSP = m_mtvectStrategyPointSummonTimeInfo.begin();
	for(; itrSP != m_mtvectStrategyPointSummonTimeInfo.end(); itrSP++)
	{
		CFieldMapChannel *pFMChann = m_pFieldIOCP14->GetFieldMapChannelByMapChannelIndex(MAP_CHANNEL_INDEX(itrSP->MapIndex, 0));
		if(pFMChann
// 2009-10-12 by cmkwon, 프리스카 제거 방안 적용 - CInflWarManager::ClearTelePortByInfluenceWarEnd#
//			&& IS_SAME_CHARACTER_8_MAP_INFLUENCE(i_byDefenceCharInfluence, pFMChann->GetMapInfluenceTypeW())
			&& CAtumSJ::IsSameCharacterInfluence8MapInfluence(i_byDefenceCharInfluence, pFMChann->GetMapInfluenceTypeW())	// 2009-10-12 by cmkwon, 프리스카 제거 방안 적용 -  
			)
		{
			pFMChann->SetTelePortState(TELEPORT_STATE_NOT_SUMMON);
			pFMChann->SetTelePortSummonFirst(FALSE);
			pFMChann->m_ChangeBodyConMonsterIndex = 0;
		}
	}
	mtSP.auto_unlock_cancel();

	//////////////////////////////////////////////////////////////////////////
	// 2008-03-28 by dhjin, 모선전 정보 표시 기획안 - 모선전 정보 값 삭제한다.
	this->m_MSWarInfoDisPlay.MSWarInfoDisPlayClear(i_byDefenceCharInfluence);		// 2009-03-25 by cmkwon, 모선전시 텔레포트 초기화 관련 버그 수정 - 
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CInflWarManager::DeleteTelePortInfoBuing(MapIndex_t i_mapIdx)
/// \brief		// 2007-09-20 by cmkwon, 텔레포트 소환 관련 수정 - 
/// \author		cmkwon
/// \date		2007-09-20 ~ 2007-09-20
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CInflWarManager::DeleteTelePortInfoBuing(MapIndex_t i_mapIdx)
{
	mt_auto_lock mta(&m_mtvectTeleportInfoBuilding);

	if(m_VCNTeleportMapIndex == i_mapIdx)
	{
		m_VCNTeleportMapIndex = 0;
	}
	if(m_ANITeleportMapIndex == i_mapIdx)
	{
		m_ANITeleportMapIndex = 0;
	}

	mtvectSTELEPORT_INFO_BUILDING::iterator itr = m_mtvectTeleportInfoBuilding.begin();
	while(itr != m_mtvectTeleportInfoBuilding.end())
	{
		if(itr->MapIndex == i_mapIdx)
		{
			itr = m_mtvectTeleportInfoBuilding.erase(itr);
			continue;
		}
		itr++;
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			MapIndex_t CInflWarManager::GetTeleportMapIndex(BYTE i_byInfluence)
/// \brief		
/// \author		dhjin
/// \date		2007-09-16 ~ 2007-09-16
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
MapIndex_t CInflWarManager::GetTeleportMapIndex(BYTE i_byInfluence)
{
	if(IS_VCN_INFLUENCE_TYPE(i_byInfluence))
	{
		return m_VCNTeleportMapIndex;
	}
	else if(IS_ANI_INFLUENCE_TYPE(i_byInfluence))
	{
		return m_ANITeleportMapIndex;
	}

	return 0;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CInflWarManager::SetPollDate(SPOLLDATE_DB * i_pSPOLLDATE_DB)
/// \brief		DB에서 로딩한 값 저장
/// \author		dhjin
/// \date		2007-10-29 ~ 2007-10-29
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CInflWarManager::SetPollDate(SPOLLDATE_DB * i_pSPOLLDATE_DB)
{
	if(NULL == i_pSPOLLDATE_DB)
	{
		return;
	}
	
	m_SPOLLDATE_DB.ApplicationStartDate		= i_pSPOLLDATE_DB->ApplicationStartDate;
	m_SPOLLDATE_DB.ApplicationEndDate		= i_pSPOLLDATE_DB->ApplicationEndDate;
	m_SPOLLDATE_DB.VoteStartDate			= i_pSPOLLDATE_DB->VoteStartDate;
	m_SPOLLDATE_DB.VoteEndDate				= i_pSPOLLDATE_DB->VoteEndDate;
	m_SPOLLDATE_DB.Election					= i_pSPOLLDATE_DB->Election;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CInflWarManager::LoadVoterListByDB()
/// \brief		DB에서 로딩 투표자 리스트 
/// \author		dhjin
/// \date		2007-10-29 ~ 2007-10-29
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CInflWarManager::LoadVoterListByDB()
{
	this->m_pFieldIOCP14->m_pAtumDBManager->MakeAndEnqueueQuery(QT_LoadVoterList, NULL, 0, NULL);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CInflWarManager::LoadLeaderCandidateByDB()
/// \brief		DB에서 로딩 지도자 후보 리스트
/// \author		dhjin
/// \date		2007-10-29 ~ 2007-10-29
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CInflWarManager::LoadLeaderCandidateByDB()
{
	this->m_pFieldIOCP14->m_pAtumDBManager->MakeAndEnqueueQuery(QT_LoadLeaderCandidate, NULL, 0, NULL);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CInflWarManager::InitVoterListByDB(mtvectSVOTER_LIST * i_pvectSVOTER_LIST)
/// \brief		DB에서 로딩한 투표자 리스트 저장 
/// \author		dhjin
/// \date		2007-10-29 ~ 2007-10-29
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CInflWarManager::InitVoterListByDB(mtvectSVOTER_LIST * i_pvectSVOTER_LIST)
{
	mt_auto_lock mta(&m_mtvectSVOTER_LIST);
	
	m_mtvectSVOTER_LIST.clear();

	m_mtvectSVOTER_LIST.assign(i_pvectSVOTER_LIST->begin(), i_pvectSVOTER_LIST->end());

}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CInflWarManager::InitLeaderCandidateByDB(mtvectSLEADER_CANDIDATE * i_pvectSLEADER_CANDIDATE)
/// \brief		DB에서 로딩한 지도자 후보 리스트 저장 
/// \author		dhjin
/// \date		2007-10-29 ~ 2007-10-29
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CInflWarManager::InitLeaderCandidateByDB(mtvectSLEADER_CANDIDATE * i_pvectSLEADER_CANDIDATE)
{
	mt_auto_lock mta(&m_mtvectSLEADER_CANDIDATE);
	
	m_mtvectSLEADER_CANDIDATE.clear();

	m_mtvectSLEADER_CANDIDATE.assign(i_pvectSLEADER_CANDIDATE->begin(), i_pvectSLEADER_CANDIDATE->end());	

}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CInflWarManager::GetLeaderCandidateInfo(INT i_nLeaderCandidateNum, BYTE i_byInfluence, SLEADER_CANDIDATE *o_pSLEADER_CANDIDATE)
/// \brief		선택된 지도자 후보 정보 값 가져오기
/// \author		dhjin
/// \date		2007-10-30 ~ 2007-10-30
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CInflWarManager::GetLeaderCandidateInfo(INT i_nLeaderCandidateNum, BYTE i_byInfluence, SLEADER_CANDIDATE *o_pSLEADER_CANDIDATE)
{
	mt_auto_lock mta(&m_mtvectSLEADER_CANDIDATE);

	mtvectSLEADER_CANDIDATE::iterator itr = m_mtvectSLEADER_CANDIDATE.begin();
	for(; itr != m_mtvectSLEADER_CANDIDATE.end(); itr++)
	{
		if(i_nLeaderCandidateNum == itr->LeaderCandidateNum
			&& i_byInfluence == itr->Influence)
		{
			o_pSLEADER_CANDIDATE->AccountUID		= itr->AccountUID;
			util::strncpy(o_pSLEADER_CANDIDATE->CampaignPromises, itr->CampaignPromises, SIZE_MAX_CAMPAIGNPROMISES);
			util::strncpy(o_pSLEADER_CANDIDATE->CharacterName, itr->CharacterName, SIZE_MAX_CHARACTER_NAME);
			o_pSLEADER_CANDIDATE->CharacterUID		= itr->CharacterUID;
			o_pSLEADER_CANDIDATE->DeleteCandidate	= itr->DeleteCandidate;
			util::strncpy(o_pSLEADER_CANDIDATE->GuildName, itr->GuildName, SIZE_MAX_GUILD_NAME);
			o_pSLEADER_CANDIDATE->GuildUID			= itr->GuildUID;
			o_pSLEADER_CANDIDATE->Influence			= itr->Influence;
			o_pSLEADER_CANDIDATE->LeaderCandidateNum= itr->LeaderCandidateNum;
			o_pSLEADER_CANDIDATE->PollCount			= itr->PollCount;
			return TRUE;
		}
	}

	return FALSE;	
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CInflWarManager::CheckLeaderCandidateUser(UID32_t i_nAccountUID, UID32_t i_nCharacterUID)
/// \brief		지도자 후보로 등록 되어 있는 유저인지 체크
/// \author		dhjin
/// \date		2007-10-29 ~ 2007-10-29
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CInflWarManager::CheckLeaderCandidateUser(UID32_t i_nAccountUID, UID32_t i_nCharacterUID)
{
	mt_auto_lock mta(&m_mtvectSLEADER_CANDIDATE);

	mtvectSLEADER_CANDIDATE::iterator itr = m_mtvectSLEADER_CANDIDATE.begin();
	for(; itr != m_mtvectSLEADER_CANDIDATE.end(); itr++)
	{
		if(i_nAccountUID == itr->AccountUID
			&& i_nCharacterUID == itr->CharacterUID)
		{
			return TRUE;
		}
	}

	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CInflWarManager::SendLeaderCandidateList(CFieldIOCPSocket *i_pFISoc)
/// \brief		지도자 후보 리스트 전송
/// \author		dhjin
/// \date		2007-10-30 ~ 2007-10-30
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CInflWarManager::SendLeaderCandidateList(CFieldIOCPSocket *i_pFISoc)
{

	BOOL	CheckLeaderCandidateUser	= FALSE;		// 2007-10-30 by dhjin, 후보 등록한 유저인지 체크 변수
	
	// 2007-10-29 by dhjin, 후보 등록된 유저인지 체크
	if(this->CheckLeaderCandidateUser(i_pFISoc->m_character.AccountUniqueNumber, i_pFISoc->m_character.CharacterUniqueNumber))
	{// 2007-10-30 by dhjin, 후보 등록한 유저이면 TRUE로 
		CheckLeaderCandidateUser = TRUE;
	}

	// 2007-10-30 by dhjin, 전송
	INT		nSendBytes = 0;
	INIT_MSG_WITH_BUFFER(MSG_FC_CITY_POLL_REQUEST_LEADER_CANDIDATE_LIST_OK_HEADER, T_FC_CITY_POLL_REQUEST_LEADER_CANDIDATE_LIST_OK_HEADER, pSendMsgHeader, SendBuf);
	pSendMsgHeader->RegLeaderCandidate		= CheckLeaderCandidateUser;
	nSendBytes = MSG_SIZE(MSG_FC_CITY_POLL_REQUEST_LEADER_CANDIDATE_LIST_OK_HEADER);		
	
	mt_auto_lock mta(&m_mtvectSLEADER_CANDIDATE);

	mtvectSLEADER_CANDIDATE::iterator itr = m_mtvectSLEADER_CANDIDATE.begin();
	for(; itr != m_mtvectSLEADER_CANDIDATE.end(); itr++)
	{
		if(i_pFISoc->m_character.InfluenceType != itr->Influence
			|| TRUE == itr->DeleteCandidate)
		{// 2007-11-06 by dhjin, 다른 세력이거나 삭제된 후보이면 통과한다.
			continue;
		}

		if(nSendBytes + MSG_SIZE(MSG_FC_CITY_POLL_REQUEST_LEADER_CANDIDATE_LIST_OK) > SIZE_MAX_PACKET)
		{
			i_pFISoc->SendAddData(SendBuf, nSendBytes);
			nSendBytes	= 0;
		}

		*((MessageType_t*)(SendBuf + nSendBytes))					= T_FC_CITY_POLL_REQUEST_LEADER_CANDIDATE_LIST_OK;		
		nSendBytes													+= SIZE_FIELD_TYPE_HEADER;
		MSG_FC_CITY_POLL_REQUEST_LEADER_CANDIDATE_LIST_OK *pSendMsg	= (MSG_FC_CITY_POLL_REQUEST_LEADER_CANDIDATE_LIST_OK*)(SendBuf + nSendBytes);
		pSendMsg->LeaderCandidateNum								= itr->LeaderCandidateNum;
		pSendMsg->CharacterUID										= itr->CharacterUID;
		pSendMsg->GuildUID											= itr->GuildUID;
		pSendMsg->DeleteCandidate									= itr->DeleteCandidate;
		util::strncpy(pSendMsg->CharacterName, itr->CharacterName, SIZE_MAX_CHARACTER_NAME);
		util::strncpy(pSendMsg->GuildName, itr->GuildName, SIZE_MAX_CHARACTER_NAME);
		nSendBytes									+= sizeof(MSG_FC_CITY_POLL_REQUEST_LEADER_CANDIDATE_LIST_OK);
	}

	if(nSendBytes > 0)
	{
		i_pFISoc->SendAddData(SendBuf, nSendBytes);
		nSendBytes	= 0;
	}
	
	mta.auto_unlock_cancel();

	i_pFISoc->SendAddMessageType(T_FC_CITY_POLL_REQUEST_LEADER_CANDIDATE_LIST_OK_DONE);
	
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CInflWarManager::CheckPollDateByApplication(ATUM_DATE_TIME * i_pCurrentTime)
/// \brief		지도자 후보 등록 기간이지 체크
/// \author		dhjin
/// \date		2007-10-30 ~ 2007-10-30
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CInflWarManager::CheckPollDateByApplication(ATUM_DATE_TIME * i_pCurrentTime)
{
	if(m_SPOLLDATE_DB.ApplicationStartDate <= *i_pCurrentTime
		&& m_SPOLLDATE_DB.ApplicationEndDate >= *i_pCurrentTime)
	{
		return TRUE;
	}

	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CInflWarManager::CheckPollDateByVote(ATUM_DATE_TIME * i_pCurrentTime)
/// \brief		투표 기간이지 체크
/// \author		dhjin
/// \date		2007-10-30 ~ 2007-10-30
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CInflWarManager::CheckPollDateByVote(ATUM_DATE_TIME * i_pCurrentTime)
{
	if(m_SPOLLDATE_DB.VoteStartDate <= *i_pCurrentTime
		&& m_SPOLLDATE_DB.VoteEndDate >= *i_pCurrentTime)
	{
		return TRUE;
	}

	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CInflWarManager::CheckPollDateRange(ATUM_DATE_TIME * i_pCurrentTime)
/// \brief		선거 기간인지 체크
/// \author		dhjin
/// \date		2007-11-20 ~ 2007-11-20
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CInflWarManager::CheckPollDateRange(ATUM_DATE_TIME * i_pCurrentTime)
{
	if(m_SPOLLDATE_DB.ApplicationStartDate <= *i_pCurrentTime
		&& m_SPOLLDATE_DB.Election >= *i_pCurrentTime)
	{
		return TRUE;
	}

	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CInflWarManager::CheckLeaderelectionDate(ATUM_DATE_TIME * i_pCurrentTime)
/// \brief		지도자 선출 시간 체크
/// \author		dhjin
/// \date		2007-11-16 ~ 2007-11-16
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CInflWarManager::CheckLeaderelectionDate(ATUM_DATE_TIME * i_pCurrentTime)
{
	if(0 == m_SPOLLDATE_DB.Election.GetTimeDiffTimeInMinutes(*i_pCurrentTime))
	{
		return TRUE;
	}

	return FALSE;
}

#ifdef NEMERIAN_NATION_BALANCE_BUFF_PENALITY
BOOL CInflWarManager::ElectionDateIsInPast(ATUM_DATE_TIME * i_pCurrentTime)
{
	if (m_SPOLLDATE_DB.Election.GetTimeDiffTimeInMinutes(*i_pCurrentTime) > 0) {
		return TRUE;
	}

	return FALSE;
}
#endif

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CInflWarManager::SendLeaderelectionInfo()
/// \brief		지도자 선출 정보 전송
/// \author		dhjin
/// \date		2007-11-16 ~ 2007-11-16
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CInflWarManager::SendLeaderelectionInfo()
{
	INIT_MSG_WITH_BUFFER(MSG_FC_CITY_POLL_LEADER_ELECTION_INFO, T_FC_CITY_POLL_LEADER_ELECTION_INFO, pVCNSendMsg, VCNSendBuf);
	INIT_MSG_WITH_BUFFER(MSG_FC_CITY_POLL_LEADER_ELECTION_INFO, T_FC_CITY_POLL_LEADER_ELECTION_INFO, pANISendMsg, ANISendBuf);
	
	INT VCNPollCount = 0;			// 지도자 득표 수
	INT ANIPollCount = 0;
	INT TotalVCNPollCount = 0;
	INT TotalANIPollCount = 0;
	
	mt_auto_lock mta(&m_mtvectSLEADER_CANDIDATE);

	mtvectSLEADER_CANDIDATE::iterator itr = m_mtvectSLEADER_CANDIDATE.begin();
	for(; itr != m_mtvectSLEADER_CANDIDATE.end(); itr++)
	{
		if(COMPARE_INFLUENCE(itr->Influence, INFLUENCE_TYPE_VCN)
			&& FALSE == itr->DeleteCandidate)
		{
			if(VCNPollCount < itr->PollCount)
			{
				pVCNSendMsg->LeaderCandidateNum = itr->LeaderCandidateNum;
				util::strncpy(pVCNSendMsg->CharacterName, itr->CharacterName, SIZE_MAX_CHARACTER_NAME);
				util::strncpy(pVCNSendMsg->GuildName, itr->GuildName, SIZE_MAX_GUILD_NAME);
				VCNPollCount	= itr->PollCount;
			}
			TotalVCNPollCount	+= itr->PollCount;
		}
		else if(COMPARE_INFLUENCE(itr->Influence, INFLUENCE_TYPE_ANI)
				&& FALSE == itr->DeleteCandidate)
		{
			if(ANIPollCount < itr->PollCount)
			{
				pANISendMsg->LeaderCandidateNum = itr->LeaderCandidateNum;
				util::strncpy(pANISendMsg->CharacterName, itr->CharacterName, SIZE_MAX_CHARACTER_NAME);
				util::strncpy(pANISendMsg->GuildName, itr->GuildName, SIZE_MAX_GUILD_NAME);
				ANIPollCount	= itr->PollCount;
			}
			TotalANIPollCount	+= itr->PollCount;			
		}
	}
	mta.auto_unlock_cancel();

	if(0 != TotalVCNPollCount)
	{
		pVCNSendMsg->Percent	= (VCNPollCount * 100) / TotalVCNPollCount;
	}
	if(0 != TotalANIPollCount)
	{
		pANISendMsg->Percent	= (ANIPollCount * 100) / TotalANIPollCount;
	}

	m_pFieldIOCP14->SendMessageToAllClients(VCNSendBuf, MSG_SIZE(MSG_FC_CITY_POLL_LEADER_ELECTION_INFO), INFLUENCE_TYPE_VCN);
	m_pFieldIOCP14->SendMessageToAllClients(ANISendBuf, MSG_SIZE(MSG_FC_CITY_POLL_LEADER_ELECTION_INFO), INFLUENCE_TYPE_ANI);
	
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			void CInflWarManager::InsertLeaderCandidate(SLEADER_CANDIDATE * i_pSLEADER_CANDIDATE)
/// \brief		지도자 후보 등록
/// \author		dhjin
/// \date		2007-10-30 ~ 2007-10-30
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CInflWarManager::InsertLeaderCandidate(SLEADER_CANDIDATE * i_pSLEADER_CANDIDATE)
{
	mt_auto_lock mta(&m_mtvectSLEADER_CANDIDATE);

	if(NULL != i_pSLEADER_CANDIDATE)
	{
		m_mtvectSLEADER_CANDIDATE.push_back(*i_pSLEADER_CANDIDATE);
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CInflWarManager::UpdateDeleteLeaderCandidate(INT i_nCharacterUID)
/// \brief		지도자 후보 탈퇴
/// \author		dhjin
/// \date		2007-10-30 ~ 2007-10-30
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CInflWarManager::UpdateDeleteLeaderCandidate(INT i_nCharacterUID)
{
	mt_auto_lock mta(&m_mtvectSLEADER_CANDIDATE);

	mtvectSLEADER_CANDIDATE::iterator itr = m_mtvectSLEADER_CANDIDATE.begin();
	for(; itr != m_mtvectSLEADER_CANDIDATE.end(); itr++)
	{
		if(i_nCharacterUID == itr->CharacterUID)
		{
			itr->DeleteCandidate	= TRUE;
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CInflWarManager::DeleteLeaderCandidate(INT i_nCharacterUID)
/// \brief		지도자 후보 게임 데이타 상에 삭제, DB에만 지워지는 버그 수정
/// \author		dhjin
/// \date		2008-06-05 ~ 2008-06-05
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CInflWarManager::DeleteLeaderCandidate(INT i_nCharacterUID)
{
	mt_auto_lock mta(&m_mtvectSLEADER_CANDIDATE);

	mtvectSLEADER_CANDIDATE::iterator itr = m_mtvectSLEADER_CANDIDATE.begin();
	for(; itr != m_mtvectSLEADER_CANDIDATE.end(); itr++)
	{
		if(i_nCharacterUID == itr->CharacterUID)
		{
			itr = m_mtvectSLEADER_CANDIDATE.erase(itr);
			return;
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CInflWarManager::CheckPossibleVoterUser(UID32_t i_nAccountUID)
/// \brief		투표한 유저인지 체크
/// \author		dhjin
/// \date		2007-10-31 ~ 2007-10-31
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CInflWarManager::CheckPossibleVoterUser(UID32_t i_nAccountUID)
{
	mt_auto_lock mta(&m_mtvectSVOTER_LIST);

	mtvectSVOTER_LIST::iterator itr = m_mtvectSVOTER_LIST.begin();
	for(; itr != m_mtvectSVOTER_LIST.end(); itr++)
	{
		if(i_nAccountUID == itr->AccountUID)
		{
			return FALSE;
		}
	}

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CInflWarManager::LeaderCandidateVote(INT i_nLeaderCandidateNum, BYTE i_byInfluence, INT i_nPollPoint)
/// \brief		지도자 후보에게 투표한 결과(득표수증가)
/// \author		dhjin
/// \date		2007-10-31 ~ 2007-10-31
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CInflWarManager::LeaderCandidateVote(INT i_nLeaderCandidateNum, BYTE i_byInfluence, INT i_nPollPoint)
{
	mt_auto_lock mta(&m_mtvectSLEADER_CANDIDATE);

	mtvectSLEADER_CANDIDATE::iterator itr = m_mtvectSLEADER_CANDIDATE.begin();
	for(; itr != m_mtvectSLEADER_CANDIDATE.end(); itr++)
	{
		if(i_nLeaderCandidateNum == itr->LeaderCandidateNum
			&& i_byInfluence	== itr->Influence
			&& FALSE == itr->DeleteCandidate)
		{
			itr->PollCount += i_nPollPoint;
			QPARAM_UPDATE_LEADERPOLLCOUNT *pQParam = new QPARAM_UPDATE_LEADERPOLLCOUNT;	
			pQParam->LeaderCandidateNum		= itr->LeaderCandidateNum;
			pQParam->Influence				= itr->Influence;
			pQParam->PollCount				= itr->PollCount;
			this->m_pFieldIOCP14->m_pAtumDBManager->MakeAndEnqueueQuery(QT_UpdateLeaderPollCount, NULL, 0, pQParam);
			return TRUE;
		}
	}

	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CInflWarManager::InsertVoterUser(SVOTER_LIST *i_pSVOTER_LIST)
/// \brief		투표한 유저 추가
/// \author		dhjin
/// \date		2007-10-31 ~ 2007-10-31
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CInflWarManager::InsertVoterUser(SVOTER_LIST *i_pSVOTER_LIST)
{
	mt_auto_lock mta(&m_mtvectSVOTER_LIST);

	if(NULL != i_pSVOTER_LIST)
	{
		QPARAM_INSERT_VOTER_LIST *pQParam = new QPARAM_INSERT_VOTER_LIST;
		pQParam->AccountUID			= i_pSVOTER_LIST->AccountUID;
		pQParam->CharacterUID		= i_pSVOTER_LIST->CharacterUID;
		pQParam->LeaderCandidateNum	= i_pSVOTER_LIST->LeaderCandidateNum;
		this->m_pFieldIOCP14->m_pAtumDBManager->MakeAndEnqueueQuery(QT_InsertVoterList, NULL, 0, pQParam);
		m_mtvectSVOTER_LIST.push_back(*i_pSVOTER_LIST);
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CInflWarManager::SendPollDate(CFieldIOCPSocket *i_pFISoc)
/// \brief		선거 일정 전송
/// \author		dhjin
/// \date		2007-10-31 ~ 2007-10-31
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CInflWarManager::SendPollDate(CFieldIOCPSocket *i_pFISoc)
{
	INIT_MSG_WITH_BUFFER(MSG_FC_CITY_POLL_REQUEST_POLL_DATE_OK, T_FC_CITY_POLL_REQUEST_POLL_DATE_OK, pSendMsg, SendBuf);
	pSendMsg->ApplicationStartDate		= m_SPOLLDATE_DB.ApplicationStartDate;
	pSendMsg->ApplicationEndDate		= m_SPOLLDATE_DB.ApplicationEndDate;
	pSendMsg->VoteStartDate				= m_SPOLLDATE_DB.VoteStartDate;
	pSendMsg->VoteEndDate				= m_SPOLLDATE_DB.VoteEndDate;
	pSendMsg->Election					= m_SPOLLDATE_DB.Election;
	if(NULL != i_pFISoc)
	{
		i_pFISoc->SendAddData(SendBuf, MSG_SIZE(MSG_FC_CITY_POLL_REQUEST_POLL_DATE_OK));
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CInflWarManager::CheckPossibleDeleteLeaderCandidate(UID32_t i_nCharacterUID)
/// \brief		지도자 후보 탈퇴가 가능한 유저인지 체크
/// \author		dhjin
/// \date		2007-11-02 ~ 2007-11-02
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CInflWarManager::CheckPossibleDeleteLeaderCandidate(UID32_t i_nCharacterUID)
{
	mt_auto_lock mta(&m_mtvectSLEADER_CANDIDATE);

	mtvectSLEADER_CANDIDATE::iterator itr = m_mtvectSLEADER_CANDIDATE.begin();
	for(; itr != m_mtvectSLEADER_CANDIDATE.end(); itr++)
	{
		if(i_nCharacterUID == itr->CharacterUID
			&& FALSE == itr->DeleteCandidate)
		{
			return TRUE;
		}
	}

	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CInflWarManager::CheckRegLeaderCandidateGuild(UID32_t i_GuildUID)
/// \brief		전진기지 프로텍터 데미지 계산에 지도자 후보 여단 제외 - 지도자 후보 등록한 여단인지 체크
/// \author		dhjin
/// \date		2008-04-02 ~ 2008-04-02
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CInflWarManager::CheckRegLeaderCandidateGuild(UID32_t i_GuildUID)
{
	mt_auto_lock mta(&m_mtvectSLEADER_CANDIDATE);

	mtvectSLEADER_CANDIDATE::iterator itr = m_mtvectSLEADER_CANDIDATE.begin();
	for(; itr != m_mtvectSLEADER_CANDIDATE.end(); itr++)
	{
		if(i_GuildUID == itr->GuildUID)
		{
			return TRUE;
		}
	}

	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			INT CInflWarManager::GetBossWarStrateyPointSummoningCountByInfl(BYTE i_byInfluence)
/// \brief		모선전 종료시 남아있는 전략포인트 수 체크하여 반환
/// \author		dhjin
/// \date		2007-11-06 ~ 2007-11-06
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
INT CInflWarManager::GetBossWarStrateyPointSummoningCountByInfl(BYTE i_byInfluence)
{
	mt_auto_lock mta(&m_mtvectSummonStrategyPointInfo);

	INT	tmStrategyPointSummoningCount = 0;
	mtvectorSSUMMON_STRATEGYPOINT_INFO::iterator	itr = m_mtvectSummonStrategyPointInfo.begin();
	while(itr != m_mtvectSummonStrategyPointInfo.end())
	{
		if(i_byInfluence == itr->InfluenceType)
		{
			tmStrategyPointSummoningCount++;
		}
		itr++;
	}

	return tmStrategyPointSummoningCount;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CInflWarManager::SendCityInfoInfluence(CFieldIOCPSocket *i_pFISoc)
/// \brief		전장 정보 중 세력 정보를 보낸다.
/// \author		dhjin
/// \date		2007-12-03 ~ 2007-12-03
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CInflWarManager::SendCityInfoInfluence(CFieldIOCPSocket *i_pFISoc)
{
	CInflWarData *pVCNInflWarData = GetInflWarDataByInflType(INFLUENCE_TYPE_VCN);
	mt_auto_lock mtV(pVCNInflWarData->GetmtlockPtr());
	INIT_MSG_WITH_BUFFER(MSG_FC_CITY_WARINFO_INFLUENCE_OK, T_FC_CITY_WARINFO_INFLUENCE_OK, pSendMsg, SendBuf);
	pSendMsg->VCNInfluencePoint			= pVCNInflWarData->ContributionPoint;
	util::strncpy(pSendMsg->VCNInfluenceLeader, pVCNInflWarData->InflLeaderCharacterName, SIZE_MAX_CHARACTER_NAME);
	util::strncpy(pSendMsg->VCNInfluenceSubLeader1, pVCNInflWarData->InflSubLeader1CharacterName, SIZE_MAX_CHARACTER_NAME);
	util::strncpy(pSendMsg->VCNInfluenceSubLeader2, pVCNInflWarData->InflSubLeader2CharacterName, SIZE_MAX_CHARACTER_NAME);
	mtV.auto_unlock_cancel();

	CInflWarData *pANIInflWarData = GetInflWarDataByInflType(INFLUENCE_TYPE_ANI);
	mt_auto_lock mtA(pANIInflWarData->GetmtlockPtr());
	pSendMsg->ANIInfluencePoint			= pANIInflWarData->ContributionPoint;
	util::strncpy(pSendMsg->ANIInfluenceLeader, pANIInflWarData->InflLeaderCharacterName, SIZE_MAX_CHARACTER_NAME);
	util::strncpy(pSendMsg->ANIInfluenceSubLeader1, pANIInflWarData->InflSubLeader1CharacterName, SIZE_MAX_CHARACTER_NAME);
	util::strncpy(pSendMsg->ANIInfluenceSubLeader2, pANIInflWarData->InflSubLeader2CharacterName, SIZE_MAX_CHARACTER_NAME);
	mtA.auto_unlock_cancel();

	if(NULL != i_pFISoc)
	{
		i_pFISoc->SendAddData(SendBuf, MSG_SIZE(MSG_FC_CITY_WARINFO_INFLUENCE_OK));
	}

}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CInflWarManager::InsertMSWarInfoByMSWarStart(MapIndex_t i_nStrategypointMapindex, INT i_nStrategypointMapInfluence, BYTE i_byStrategypointBelligerence)
/// \brief		전략포인트 소환된 정보를 설정한다.
/// \author		dhjin
/// \date		2008-03-26 ~ 2008-03-26
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CInflWarManager::InsertMSWarInfoByMSWarStart(MapIndex_t i_nStrategypointMapindex, INT i_nStrategypointMapInfluence, BYTE i_byStrategypointBelligerence)
{
	MSWARINFO_DISPLAY SMSWarInfoDisPlay;
	util::zero(&SMSWarInfoDisPlay, sizeof(MSWARINFO_DISPLAY));
	
	SMSWarInfoDisPlay.MapIndex			= i_nStrategypointMapindex;
	SMSWarInfoDisPlay.MapInfluenceType  = i_nStrategypointMapInfluence;
	SMSWarInfoDisPlay.HPRate			= MS_WAR_HP_RATE;
	SMSWarInfoDisPlay.TelePortState		= TELEPORT_STATE_NOT_SUMMON;
	SMSWarInfoDisPlay.Belligerence		= i_byStrategypointBelligerence;

	this->m_MSWarInfoDisPlay.SetMSWarInfoDisPlay(&SMSWarInfoDisPlay);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CInflWarManager::SendMSWarInfoForAllUser()
/// \brief		모선전 정보 표시 기획안 - 모선전 정보 전송
/// \author		dhjin
/// \date		2008-03-27 ~ 2008-03-27
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CInflWarManager::SendMSWarInfoForAllUser()
{
	vectMSWARINFO_DISPLAY VectMSWarInfoDisPlay;
	VectMSWarInfoDisPlay.clear();
	
	this->m_MSWarInfoDisPlay.AllGetMSWarInfoDisPlay(&VectMSWarInfoDisPlay);
	
	INIT_MSG_WITH_BUFFER(MSG_FC_INFO_MSWARINFO_DISPLAY, T_FC_INFO_MSWARINFO_DISPLAY, pMSWarInfoDisPlayInfo, MSWarInfoDisPlayInfoSendBuf);
	pMSWarInfoDisPlayInfo->MSWarInfoDisPlayListCount = this->MakeMSG_FC_INFO_MSWARINFO_DISPLAY((SMSWARINFO_DISPLAY*)(MSWarInfoDisPlayInfoSendBuf+MSG_SIZE(MSG_FC_INFO_MSWARINFO_DISPLAY)), &VectMSWarInfoDisPlay);
	if(0 >= pMSWarInfoDisPlayInfo->MSWarInfoDisPlayListCount)
	{
		return;
	}

	m_pFieldIOCP14->SendMessageToAllClients(MSWarInfoDisPlayInfoSendBuf, MSG_SIZE(MSG_FC_INFO_MSWARINFO_DISPLAY)+pMSWarInfoDisPlayInfo->MSWarInfoDisPlayListCount*sizeof(SMSWARINFO_DISPLAY), INFLUENCE_TYPE_VCN|INFLUENCE_TYPE_ANI);
}

void CInflWarManager::SendStrategicPointInfoListForAllUser()
{
	vector<SSTRATEGYPOINT_DISPLAY_INFO> VectStrategicPointInfoDisplay;
	VectStrategicPointInfoDisplay.clear();
	m_StrategicPointInfoDisplay.GetAllStrategicPointDisplay(&VectStrategicPointInfoDisplay);

	INIT_MSG_WITH_BUFFER(MSG_FC_INFO_STRATEGICPOINTINFO_DISPLAY_LIST, T_FC_INFO_STRATEGICPOINTINFO_DISPLAY_LIST, pStrategicPointInfoDisplayInfo, StrategicPointInfoDisplayInfoSendBuf);
	pStrategicPointInfoDisplayInfo->StrategicPointInfoDisplayListCount = this->MakeMSG_FC_INFO_STRATEGICPOINT_DISPLAY((SSTRATEGYPOINT_DISPLAY_INFO*)(StrategicPointInfoDisplayInfoSendBuf + MSG_SIZE(MSG_FC_INFO_STRATEGICPOINTINFO_DISPLAY_LIST)), VectStrategicPointInfoDisplay);
	if (0 >= pStrategicPointInfoDisplayInfo->StrategicPointInfoDisplayListCount)
	{
		return;
	}

	m_pFieldIOCP14->SendMessageToAllClients(StrategicPointInfoDisplayInfoSendBuf, MSG_SIZE(MSG_FC_INFO_STRATEGICPOINTINFO_DISPLAY_LIST) + pStrategicPointInfoDisplayInfo->StrategicPointInfoDisplayListCount*sizeof(SSTRATEGYPOINT_DISPLAY_INFO), INFLUENCE_TYPE_VCN | INFLUENCE_TYPE_ANI);
}

void CInflWarManager::SendStrategicPointInfoForAllUser(MapIndex_t mapIndex)
{
	SSTRATEGYPOINT_DISPLAY_INFO StrategicPointInfoDisplay;
	if (!m_StrategicPointInfoDisplay.GetStrategicPointDisplay(mapIndex, &StrategicPointInfoDisplay))
	{
		StrategicPointInfoDisplay.HPRate = -1;
		StrategicPointInfoDisplay.InfluenceType = INFLUENCE_TYPE_UNKNOWN;
		StrategicPointInfoDisplay.MapIndex = mapIndex;
		StrategicPointInfoDisplay.SummonStrategyPointTime.SetCurrentDateTime();
	}

	INIT_MSG_WITH_BUFFER(MSG_FC_INFO_STRATEGICPOINTINFO_DISPLAY, T_FC_INFO_STRATEGICPOINTINFO_DISPLAY, pStrategicPointInfoDisplayInfo, StrategicPointInfoDisplayInfoSendBuf);
	CopyMemory((StrategicPointInfoDisplayInfoSendBuf + SIZE_FIELD_TYPE_HEADER), &StrategicPointInfoDisplay, sizeof SSTRATEGYPOINT_DISPLAY_INFO);

	m_pFieldIOCP14->SendMessageToAllClients(StrategicPointInfoDisplayInfoSendBuf, MSG_SIZE(MSG_FC_INFO_STRATEGICPOINTINFO_DISPLAY), INFLUENCE_TYPE_VCN | INFLUENCE_TYPE_ANI);
}


void CInflWarManager::SendStrategicPointInfoForUser(CFieldIOCPSocket *i_pFISoc)
{
	vector<SSTRATEGYPOINT_DISPLAY_INFO> VectStrategicPointInfoDisplay;
	VectStrategicPointInfoDisplay.clear();

	m_StrategicPointInfoDisplay.GetAllStrategicPointDisplay(&VectStrategicPointInfoDisplay);

	INIT_MSG_WITH_BUFFER(MSG_FC_INFO_STRATEGICPOINTINFO_DISPLAY_LIST, T_FC_INFO_STRATEGICPOINTINFO_DISPLAY_LIST, pStrategicPointInfoDisplayInfo, StrategicPointInfoDisplayInfoSendBuf);
	pStrategicPointInfoDisplayInfo->StrategicPointInfoDisplayListCount = this->MakeMSG_FC_INFO_STRATEGICPOINT_DISPLAY((SSTRATEGYPOINT_DISPLAY_INFO*)(StrategicPointInfoDisplayInfoSendBuf + MSG_SIZE(MSG_FC_INFO_STRATEGICPOINTINFO_DISPLAY_LIST)), VectStrategicPointInfoDisplay);
	if (0 >= pStrategicPointInfoDisplayInfo->StrategicPointInfoDisplayListCount)
	{
		return;
	}

	i_pFISoc->SendAddData(StrategicPointInfoDisplayInfoSendBuf, MSG_SIZE(MSG_FC_INFO_STRATEGICPOINTINFO_DISPLAY_LIST) + pStrategicPointInfoDisplayInfo->StrategicPointInfoDisplayListCount*sizeof(SSTRATEGYPOINT_DISPLAY_INFO));
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CInflWarManager::SendMSWarInfoForUser(CFieldIOCPSocket *i_pFISoc)
/// \brief		모선전 정보 표시 기획안 - 모선전 정보 전송
/// \author		dhjin
/// \date		2008-03-27 ~ 2008-03-27
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CInflWarManager::SendMSWarInfoForUser(CFieldIOCPSocket *i_pFISoc)
{
	vectMSWARINFO_DISPLAY VectMSWarInfoDisPlay;
	VectMSWarInfoDisPlay.clear();
	
	this->m_MSWarInfoDisPlay.AllGetMSWarInfoDisPlay(&VectMSWarInfoDisPlay);
	
	INIT_MSG_WITH_BUFFER(MSG_FC_INFO_MSWARINFO_DISPLAY, T_FC_INFO_MSWARINFO_DISPLAY, pMSWarInfoDisPlayInfo, MSWarInfoDisPlayInfoSendBuf);
	pMSWarInfoDisPlayInfo->MSWarInfoDisPlayListCount = this->MakeMSG_FC_INFO_MSWARINFO_DISPLAY((SMSWARINFO_DISPLAY*)(MSWarInfoDisPlayInfoSendBuf+MSG_SIZE(MSG_FC_INFO_MSWARINFO_DISPLAY)), &VectMSWarInfoDisPlay);
	if(0 >= pMSWarInfoDisPlayInfo->MSWarInfoDisPlayListCount)
	{
		return;
	}

	i_pFISoc->SendAddData(MSWarInfoDisPlayInfoSendBuf, MSG_SIZE(MSG_FC_INFO_MSWARINFO_DISPLAY)+pMSWarInfoDisPlayInfo->MSWarInfoDisPlayListCount*sizeof(SMSWARINFO_DISPLAY));
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			INT CInflWarManager::MakeMSG_FC_INFO_MSWARINFO_DISPLAY(SMSWARINFO_DISPLAY *o_pMSWarInfoDisPlay, vectMSWARINFO_DISPLAY * i_pVectMSWarInfoDisPlay)
/// \brief		모선전 정보 표시 기획안 - MSG_FC_INFO_MSWARINFO_DISPLAY
/// \author		dhjin
/// \date		2008-03-27 ~ 2008-03-27
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
INT CInflWarManager::MakeMSG_FC_INFO_MSWARINFO_DISPLAY(SMSWARINFO_DISPLAY *o_pMSWarInfoDisPlay, vectMSWARINFO_DISPLAY * i_pVectMSWarInfoDisPlay)
{
	int	MSWarInfoDisPlayListCount = 0;

	vectMSWARINFO_DISPLAY::iterator itr = i_pVectMSWarInfoDisPlay->begin();
	for(; itr != i_pVectMSWarInfoDisPlay->end(); itr++)
	{
		o_pMSWarInfoDisPlay[MSWarInfoDisPlayListCount].MapIndex					= itr->MapIndex;
		o_pMSWarInfoDisPlay[MSWarInfoDisPlayListCount].MapInfluenceType			= itr->MapInfluenceType;
		o_pMSWarInfoDisPlay[MSWarInfoDisPlayListCount].HPRate					= itr->HPRate;
		o_pMSWarInfoDisPlay[MSWarInfoDisPlayListCount].TelePortState			= itr->TelePortState;
		o_pMSWarInfoDisPlay[MSWarInfoDisPlayListCount].TelePortBuildingStartTime= itr->TelePortBuildingStartTime;
		o_pMSWarInfoDisPlay[MSWarInfoDisPlayListCount].TelePortBuildingEndTime	= itr->TelePortBuildingEndTime;
		MSWarInfoDisPlayListCount++;
	}
	
	return MSWarInfoDisPlayListCount;
}

INT CInflWarManager::MakeMSG_FC_INFO_STRATEGICPOINT_DISPLAY(SSTRATEGYPOINT_DISPLAY_INFO *o_StrategicPointInfoDisPlay, vector<SSTRATEGYPOINT_DISPLAY_INFO>& i_pVectStrategicPointInfoDisPlay)
{
	int	StrategicPointInfoDisPlayListCount = 0;

	for (auto &itr : i_pVectStrategicPointInfoDisPlay)
	{
		o_StrategicPointInfoDisPlay[StrategicPointInfoDisPlayListCount].MapIndex = itr.MapIndex;
		o_StrategicPointInfoDisPlay[StrategicPointInfoDisPlayListCount].InfluenceType = itr.InfluenceType;
		o_StrategicPointInfoDisPlay[StrategicPointInfoDisPlayListCount].HPRate = itr.HPRate;
		o_StrategicPointInfoDisPlay[StrategicPointInfoDisPlayListCount].SummonStrategyPointTime = itr.SummonStrategyPointTime;
		StrategicPointInfoDisPlayListCount++;
	}

	return StrategicPointInfoDisPlayListCount;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CInflWarManager::SetMSWarInfo(MapIndex_t i_nMapindex, INT i_nMSWarInfoModifyType, INT i_nHPRate/*= STATE_ERROR*/, INT i_nTelePortState/*= STATE_ERROR*/, BYTE i_byBelligerence/*= STATE_ERROR*/)
/// \brief		모선전 정보 표시 기획안 - 정보 수정
/// \author		dhjin
/// \date		2008-03-27 ~ 2008-03-27
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////	
void CInflWarManager::SetMSWarInfo(MapIndex_t i_nMapindex, INT i_nMSWarInfoModifyType, INT i_nHPRate/*= STATE_ERROR*/, INT i_nTelePortState/*= STATE_ERROR*/, BYTE i_byBelligerence/*= STATE_ERROR*/)
{
	switch(i_nMSWarInfoModifyType)
	{
		case T_MSWARINFO_MODIFY_UNKNOWN:			return;			// 알수 없음
		case T_MSWARINFO_MODIFY_HPRATE:
			{// HP등급 변경
				if(STATE_ERROR != i_nHPRate)
				{
					this->m_MSWarInfoDisPlay.SetMSWarInfoDisPlayByHPRate(i_nMapindex, i_nHPRate);
				}
			}
			break;
		case T_MSWARINFO_MODIFY_CREATE_TELEPORT:
			{// Teleport 생성
				if(STATE_ERROR != i_nHPRate
					&& STATE_ERROR != i_nTelePortState
					&& IS_TELEPORT_MONSTER(i_byBelligerence)
					)
				{
					this->m_MSWarInfoDisPlay.SetMSWarInfoDisPlayByCreateTeleport(i_nMapindex, i_nHPRate, i_nTelePortState, i_byBelligerence);
				}
			}
			break;	
		case T_MSWARINFO_MODIFY_CHANGE_TELEPORT_STATE:
			{// Teleport 생성
				if(STATE_ERROR != i_nHPRate
					&& STATE_ERROR != i_nTelePortState
					&& IS_TELEPORT_MONSTER(i_byBelligerence)
					)
				{
					//////////////////////////////////////////////////////////////////////////
					// 2008-04-22 by dhjin, 모선전 정보 표시 기획안 - 텔레포트 빌딩 완료 시간
					if(TELEPORT_STATE_BUILDING == i_nTelePortState)
					{
						INIT_MSG_WITH_BUFFER(MSG_FC_EVENT_CLICK_TELEPORT_OK, T_FC_EVENT_CLICK_TELEPORT_OK, pMsg, SendBuf);
						pMsg->MapIndex			= i_nMapindex;
						pMsg->StartTime.SetCurrentDateTime();
						pMsg->EndTime			= pMsg->StartTime;
						pMsg->EndTime.Second = 0;
						pMsg->EndTime.AddDateTime(0,0,0,0,1,TELEPORT_BUILDING_TIME_SECOND);
						m_pFieldIOCP14->SendMessageToAllClients(SendBuf, MSG_SIZE(MSG_FC_EVENT_CLICK_TELEPORT_OK));
						this->m_MSWarInfoDisPlay.SetMSWarInfoDisPlayByChangeTeleportState(i_nMapindex, i_nHPRate, i_nTelePortState, &pMsg->StartTime, &pMsg->EndTime);
					}	
					else
					{
						this->m_MSWarInfoDisPlay.SetMSWarInfoDisPlayByChangeTeleportState(i_nMapindex, i_nHPRate, i_nTelePortState);
					}
				}
			}
			break;
		default:
			{
				return;
			}
			break;
	}
	this->SendMSWarInfoForAllUser();
	return;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CInflWarManager::InitMSWarOptionTypeW(BYTE i_byLeaderInfluence)
/// \brief		모선전 정보 표시 기획안 -  모선전 정보 옵션 초기화 
/// \author		dhjin
/// \date		2008-03-28 ~ 2008-03-28
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CInflWarManager::InitMSWarOptionTypeW(BYTE i_byLeaderInfluence)
{
	this->m_MSWarInfoDisPlay.InitMSWarOptionType(i_byLeaderInfluence);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CInflWarManager::SetMSWarOptionTypeW(BYTE i_byLeaderInfluence, SHORT i_byMSWarOptionType)
/// \brief		모선전 정보 표시 기획안 - 모선전 정보 옵션 저장 
/// \author		dhjin
/// \date		2008-03-28 ~ 2008-03-28
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CInflWarManager::SetMSWarOptionTypeW(BYTE i_byLeaderInfluence, SHORT i_byMSWarOptionType)
{
	this->m_MSWarInfoDisPlay.SetMSWarOptionType(i_byLeaderInfluence, i_byMSWarOptionType);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CInflWarManager::SendMSWarOptionTypeForInfluenceAllUser(BYTE i_byLeaderInfluence)
/// \brief		모선전 정보 표시 기획안 - 모선전 정보 옵션 세력에 모든 유저에게 전송하기
/// \author		dhjin
/// \date		2008-03-28 ~ 2008-03-28
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CInflWarManager::SendMSWarOptionTypeForInfluenceAllUser(BYTE i_byLeaderInfluence)
{
	INIT_MSG_WITH_BUFFER(MSG_FC_INFO_MSWARINFO_DISPLAY_OPTION_OK, T_FC_INFO_MSWARINFO_DISPLAY_OPTION_OK, pMSWarInfoDisPlayOption, MSWarInfoDisPlayOptionSendBuf);
	pMSWarInfoDisPlayOption->MSWarOptionType		= this->m_MSWarInfoDisPlay.GetMSWarOptionType(i_byLeaderInfluence);
	m_pFieldIOCP14->SendMessageToAllClients(MSWarInfoDisPlayOptionSendBuf, MSG_SIZE(MSG_FC_INFO_MSWARINFO_DISPLAY_OPTION_OK), i_byLeaderInfluence);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CInflWarManager::SendMSWarOptionTypeForInfluenceUser(CFieldIOCPSocket *i_pFISoc)
/// \brief		모선전 정보 표시 기획안 - 모선전 정보 옵션 유저에게 전송하기
/// \author		dhjin
/// \date		2008-03-28 ~ 2008-03-28
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CInflWarManager::SendMSWarOptionTypeForInfluenceUser(CFieldIOCPSocket *i_pFISoc)
{
	INIT_MSG_WITH_BUFFER(MSG_FC_INFO_MSWARINFO_DISPLAY_OPTION_OK, T_FC_INFO_MSWARINFO_DISPLAY_OPTION_OK, pMSWarInfoDisPlayOption, MSWarInfoDisPlayOptionSendBuf);
	pMSWarInfoDisPlayOption->MSWarOptionType		= this->m_MSWarInfoDisPlay.GetMSWarOptionType(i_pFISoc->m_character.InfluenceType);
	i_pFISoc->SendAddData(MSWarInfoDisPlayOptionSendBuf, MSG_SIZE(MSG_FC_INFO_MSWARINFO_DISPLAY_OPTION_OK));
}

//////////////////////////////////////////////////////////////////////////
// 2008-04-01 by dhjin, 모선전, 거점전 정보창 기획안 - 모선전 결과 정보
void CInflWarManager::ResetMSWarInfoResult()
{
	m_vectMSWarInfoResult.clear();
}

void CInflWarManager::ResetSPWarInfoResult()
{
	m_vectSPWarInfoResult.clear();
}

void CInflWarManager::DeleteMSWarInfoResult(INT i_nMonsterUID)
{
	vectSMSWAR_INFO_RESULT::iterator itr = m_vectMSWarInfoResult.begin();
	for(; itr != m_vectMSWarInfoResult.end(); itr++)
	{
		if(i_nMonsterUID == itr->MonsterUID)
		{
			itr = m_vectMSWarInfoResult.erase(itr);
			return;
		}
	}
}

void CInflWarManager::DeleteSPWarInfoResult(INT i_nSPSummonMapIndex)
{
	vectSSPWAR_INFO_RESULT::iterator itr = m_vectSPWarInfoResult.begin();
	for(; itr != m_vectSPWarInfoResult.end(); itr++)
	{
		if(i_nSPSummonMapIndex == itr->SPSummonMapIndex)
		{
			itr = m_vectSPWarInfoResult.erase(itr);
			return;
		}
	}
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			void CInflWarManager::SetMSWarInfoResultByWarStart(BYTE i_byAttInfluence, INT i_nMonsterUID, INT i_nContributionPoint)
/// \brief		모선전, 거점전 정보창 기획안 - 모선전 시작 시 정보 설정
/// \author		dhjin
/// \date		2008-04-01 ~ 2008-04-01
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CInflWarManager::SetMSWarInfoResultByWarStart(BYTE i_byAttInfluence, INT i_nMonsterUID, INT i_nContributionPoint)
{
	SMSWAR_INFO_RESULT MSWarInfoResult;
	MSWarInfoResult.AttInfluence		= i_byAttInfluence;
	MSWarInfoResult.MonsterUID			= i_nMonsterUID;
	MSWarInfoResult.ContributionPoint	= i_nContributionPoint;
	MSWarInfoResult.MSWarStartTime.SetCurrentDateTime();
	
	vectSMSWAR_INFO_RESULT::iterator itr = m_vectMSWarInfoResult.begin();
	for(; itr != m_vectMSWarInfoResult.end(); itr++)
	{
		if(i_nMonsterUID == itr->MonsterUID)
		{
			return;
		}
	}

	m_vectMSWarInfoResult.push_back(MSWarInfoResult);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CInflWarManager::SetMSWarInfoResultByWarEnd(INT i_nMonsterUID, BYTE i_byWinInfluence)
/// \brief		모선전, 거점전 정보창 기획안 - 모선전 종료 시 정보 설정
/// \author		dhjin
/// \date		2008-04-01 ~ 2008-04-01
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CInflWarManager::SetMSWarInfoResultByWarEnd(INT i_nMonsterUID, BYTE i_byWinInfluence)
{
	vectSMSWAR_INFO_RESULT::iterator itr = m_vectMSWarInfoResult.begin();
	for(; itr != m_vectMSWarInfoResult.end(); itr++)
	{
		if(i_nMonsterUID == itr->MonsterUID)
		{
			itr->WinInfluence		= i_byWinInfluence;
			return;
		}
	}
	
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CInflWarManager::SendLogMSWarInfoResult(INT i_nMonsterUID)
/// \brief		모선전, 거점전 정보창 기획안 - 모선전 종료 시 정보 로그 저장
/// \author		dhjin
/// \date		2008-04-01 ~ 2008-04-01
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CInflWarManager::SendLogMSWarInfoResult(INT i_nMonsterUID)
{
	vectSMSWAR_INFO_RESULT::iterator itr = m_vectMSWarInfoResult.begin();
	for(; itr != m_vectMSWarInfoResult.end(); itr++)
	{
		if(i_nMonsterUID == itr->MonsterUID)
		{
			// 2012-08-30 by jhseol, 풀로그 남기기
#ifdef S_FULL_LOG_JHSEOL
			// 2008-08-28 by dhjin, 버그 수정, 게임DB에 남겨야 Admintool로 초기화가 가능하다.
			MSG_FL_LOG_MS_WAR MSWarLog;
			MSWarLog.AttInfluence		= itr->AttInfluence;
			MSWarLog.ContributionPoint	= itr->ContributionPoint;
			MSWarLog.MonsterUID			= itr->MonsterUID;
			MSWarLog.WinInfluence		= itr->WinInfluence;
			MSWarLog.MSWarStartTime		= itr->MSWarStartTime;

			CAtumLogSender::SendLogMessageMSWar(&MSWarLog);
#endif	//#ifdef S_FULL_LOG_JHSEOL
			// end 2012-08-30 by jhseol, 풀로그 남기기

			QPARAM_INSERT_MSWAR_LOG *pQParam = new QPARAM_INSERT_MSWAR_LOG;	
			pQParam->AttInfluence		= itr->AttInfluence;
			pQParam->ContributionPoint	= itr->ContributionPoint;
			pQParam->MonsterUID			= itr->MonsterUID;
			pQParam->WinInfluence		= itr->WinInfluence;
			pQParam->MSWarStartTime		= itr->MSWarStartTime;
			this->m_pFieldIOCP14->m_pAtumDBManager->MakeAndEnqueueQuery(QT_InsertMSWarLog, NULL, 0, pQParam);
	
			return;
		}
	}

}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CInflWarManager::SetSPWarInfoResultByWarStart(BYTE i_byAttInfluence, INT i_nSPSummonMapIndex)
/// \brief		모선전, 거점전 정보창 기획안 - 거점전 시작 시 정보 설정
/// \author		dhjin
/// \date		2008-04-01 ~ 2008-04-01
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CInflWarManager::SetSPWarInfoResultByWarStart(BYTE i_byAttInfluence, INT i_nSPSummonMapIndex)
{
	SSPWAR_INFO_RESULT SPWarInfoResult;
	SPWarInfoResult.AttInfluence		= i_byAttInfluence;
	SPWarInfoResult.SPSummonMapIndex	= i_nSPSummonMapIndex;
	SPWarInfoResult.SPWarStartTime.SetCurrentDateTime();

	vectSSPWAR_INFO_RESULT::iterator itr = m_vectSPWarInfoResult.begin();
	for(; itr != m_vectSPWarInfoResult.end(); itr++)
	{
		// 2008-07-24 by dhjin, 거점 로그 기록에 안남던 버그 수정
//		if(i_nSPSummonMapIndex != itr->SPSummonMapIndex)
		if(i_nSPSummonMapIndex == itr->SPSummonMapIndex)
		{
			return;
		}
	}

	m_vectSPWarInfoResult.push_back(SPWarInfoResult);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CInflWarManager::SetSPWarInfoResultByWarEnd(BYTE i_byWinInfluence)
/// \brief		모선전, 거점전 정보창 기획안 - 거점전 종료 시 정보 설정
/// \author		dhjin
/// \date		2008-04-01 ~ 2008-04-01
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CInflWarManager::SetSPWarInfoResultByWarEnd(INT i_nSPSummonMapIndex, BYTE i_byWinInfluence)
{
	vectSSPWAR_INFO_RESULT::iterator itr = m_vectSPWarInfoResult.begin();
	for(; itr != m_vectSPWarInfoResult.end(); itr++)
	{
		if(i_nSPSummonMapIndex == itr->SPSummonMapIndex)
		{
			itr->WinInfluence		= i_byWinInfluence;
			return;
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CInflWarManager::SendLogSPWarInfoResult()
/// \brief		모선전, 거점전 정보창 기획안 - 거점전 종료 시 정보 로그 저장
/// \author		dhjin
/// \date		2008-04-01 ~ 2008-04-01
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CInflWarManager::SendLogSPWarInfoResult(INT i_nSPSummonMapIndex)
{
	vectSSPWAR_INFO_RESULT::iterator itr = m_vectSPWarInfoResult.begin();
	for(; itr != m_vectSPWarInfoResult.end(); itr++)
	{
		if(i_nSPSummonMapIndex == itr->SPSummonMapIndex)
		{
			// 2012-08-30 by jhseol, 풀로그 남기기
#ifdef S_FULL_LOG_JHSEOL
			// 2008-08-28 by dhjin, 버그 수정, 게임DB에 남겨야 Admintool로 초기화가 가능하다.
			MSG_FL_LOG_SP_WAR SPWarLog;
			SPWarLog.AttInfluence		= itr->AttInfluence;
			SPWarLog.SPSummonMapIndex   = itr->SPSummonMapIndex;
			SPWarLog.WinInfluence		= itr->WinInfluence;
			SPWarLog.SPWarStartTime		= itr->SPWarStartTime;

			//CAtumLogSender::SendLogMessageSPWar(&SPWarLog);	// 2012-12-14 by jhseol, 거점전 로그 두번 남기는 현상 수정. 주석처리함.
#endif	//#ifdef S_FULL_LOG_JHSEOL
			// end 2012-08-30 by jhseol, 풀로그 남기기
			QPARAM_INSERT_SPWAR_LOG *pQParam = new QPARAM_INSERT_SPWAR_LOG;	
			pQParam->AttInfluence		= itr->AttInfluence;
			pQParam->SPSummonMapIndex	= itr->SPSummonMapIndex;
			pQParam->WinInfluence		= itr->WinInfluence;
			pQParam->SPWarStartTime		= itr->SPWarStartTime;
			this->m_pFieldIOCP14->m_pAtumDBManager->MakeAndEnqueueQuery(QT_InsertSPWarLog, NULL, 0, pQParam);
	
			return;
		}
	}
}

#ifdef NEMERIAN_NATION_BALANCE_BUFF_PENALITY
void CInflWarManager::setDiffVictory() {
	this->m_pFieldIOCP14->m_pAtumDBManager->MakeAndEnqueueQuery(QT_GetWarVictoryDiff, m_pFieldIOCP14->GetFieldIOCPSocket(0), 0, NULL);
}
#endif

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CInflWarManager::SetMSWarInfoContributionPoint(INT i_nMonsterIdx)
/// \brief		모선전, 거점전 정보창 기획안 - 세력 포인트
/// \author		dhjin
/// \date		2008-04-22 ~ 2008-04-22
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
INT CInflWarManager::SetMSWarInfoContributionPoint(INT i_nMonsterIdx)
{
	vectSMSWAR_INFO_RESULT::iterator itr = m_vectMSWarInfoResult.begin();
	for(; itr != m_vectMSWarInfoResult.end(); itr++)
	{
		if(i_nMonsterIdx == itr->MonsterUID)
		{
			return itr->ContributionPoint;
		}
	}

	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2009-12-04 by cmkwon, 거점전시 백맵/백맵2 몬스터 소환 처리 버그 수정 - 
/// \author		cmkwon
/// \date		2009-12-04 ~ 2009-12-04
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CInflWarManager::CheckCreateableMonsterInMap(INT i_nMapIdx)
{
	vectSSPWAR_INFO_RESULT::iterator itr = m_vectSPWarInfoResult.begin();
	for(; itr != m_vectSPWarInfoResult.end(); itr++)
	{
		SSPWAR_INFO_RESULT *pSSPWarInfo = &*itr;
		if(i_nMapIdx == pSSPWarInfo->SPSummonMapIndex)
		{
			return FALSE;
		}

		const MAP_INFO *pMapInfo = CAtumSJ::GetMapInfo(pSSPWarInfo->SPSummonMapIndex);
		if(pMapInfo)
		{
			if(i_nMapIdx == pMapInfo->BeforeMapIndex
				|| i_nMapIdx == pMapInfo->BeforeMapIndex2)
			{
				return FALSE;
			}
		}
	}
	
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CInflWarManager::MSWarBeforeStart(BYTE i_byInflTy)
/// \brief		선전 포고 - 모선전 시작전 분위기 조성	
/// \author		dhjin
/// \date		2009-01-13 ~ 2009-01-13
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CInflWarManager::MSWarBeforeStart(BYTE i_byInflTy)
{
	CInflWarData *pInflWarData = GetInflWarDataByInflType(i_byInflTy);
	if(NULL == pInflWarData)
	{
		return;
	}

	mt_auto_lock mtA(pInflWarData->GetmtlockPtr());
	SDB_INFLUENCE_WAR_INFO *pCurInfoByWartimeStage = pInflWarData->GetCurInflWarInfo();
	
	if(FALSE != pCurInfoByWartimeStage->IsSummonJacoMonster)
	{
		INIT_MSG_WITH_BUFFER(MSG_FN_NPCSERVER_SUMMON_JACO_MONSTER, T_FN_NPCSERVER_SUMMON_JACO_MONSTER, pSJacoMonster, SendBuf);
		pSJacoMonster->Belligerence0	= GET_SAME_MONSTER_BELL_BY_CHARACTER_INFLTYPE(pCurInfoByWartimeStage->InfluenceType);
		pSJacoMonster->IsSummonJacoMonster	= TRUE;
		if(m_pFieldIOCP14->Send2NPCServerByTCP(SendBuf, MSG_SIZE(MSG_FN_NPCSERVER_SUMMON_JACO_MONSTER)))
		{// 2006-04-25 by cmkwon, NPCServer로 전송 성공시에 

			pInflWarData->SetSummonJacoMonster(TRUE);

			///////////////////////////////////////////////////////////////////////////////
			// 2006-04-20 by cmkwon
			INIT_MSG_WITH_BUFFER(MSG_FC_WAR_JACO_MONSTER_SUMMON, T_FC_WAR_JACO_MONSTER_SUMMON, pSJaco, SendBuf);
			pSJaco->nBelligerence0			= GET_SAME_MONSTER_BELL_BY_CHARACTER_INFLTYPE(pCurInfoByWartimeStage->InfluenceType);
			m_pFieldIOCP14->SendMessageToAllClients(SendBuf, MSG_SIZE(MSG_FC_WAR_JACO_MONSTER_SUMMON), INFLUENCE_TYPE_VCN|INFLUENCE_TYPE_ANI);
		}
	}

	this->m_DeclarationOfWar.SetMSWarEndState(i_byInflTy, MSWARING_BEFORE);

	mtA.auto_unlock_cancel();			// 2009-07-02 by cmkwon, 선전포고 관련 버그 수정 - 여기에서 락해제

	// 2009-07-02 by cmkwon, 서버 실행시 실행 관련 로그 추가 - 
	g_pFieldGlobal->WriteSystemLogEX(TRUE, "  [Notify] DeclarationNoti CInflWarManager::MSWarBeforeStart# %s WarTimeStage(%d) BossMonsterNum(%8d) IsSummonJacoMonster(%d) \r\n"
		, CAtumSJ::GetInfluenceTypeString(pCurInfoByWartimeStage->InfluenceType), pCurInfoByWartimeStage->WartimeStage, pCurInfoByWartimeStage->BossMonsterUnitKind, pCurInfoByWartimeStage->IsSummonJacoMonster);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CInflWarManager::MSWarStart(BYTE i_byInflTy)
/// \brief		선전 포고 - 모선전 시작
/// \author		dhjin
/// \date		2009-01-12 ~ 2009-01-12
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CInflWarManager::MSWarStart(BYTE i_byInflTy)
{
	CInflWarData *pInflWarData = GetInflWarDataByInflType(i_byInflTy);
	if(NULL == pInflWarData)
	{
		return;
	}

	mt_auto_lock mtA(pInflWarData->GetmtlockPtr());
	SDB_INFLUENCE_WAR_INFO *pCurInfoByWartimeStage = pInflWarData->GetCurInflWarInfo();
	// 2009-03-10 by dhjin, 단계별 모선 시스템
	BYTE BeforeWinCheck = 0;	// 2009-03-10 by dhjin, 단계별 모선 시스템 - 전 단계 승패 여부
	BYTE BossSummonStep = 0;	// 2009-03-10 by dhjin, 단계별 모선 시스템 - 다음 단계 모선이므로 pCurInfoByWartimeStage에 단계는 전단계가 된다 따라서 따로 구한다.
	INT SummonBossMonsterUID = GetMSBossSummonValue(i_byInflTy, &BeforeWinCheck);
	vectDB_INFLUENCE_WAR_INFO *pInflWarInfoList = this->GetInflWarInfoByInflType(i_byInflTy);
	if(NULL == pInflWarInfoList)
	{
		return;
	}
	for(int i=0; i < pInflWarInfoList->size(); i++)
	{
		SDB_INFLUENCE_WAR_INFO *pInflWarInfo = &((*pInflWarInfoList)[i]);
		if(SummonBossMonsterUID == pInflWarInfo->BossMonsterUnitKind
			&& i_byInflTy == pInflWarInfo->InfluenceType)
		{
			BossSummonStep = pInflWarInfo->WartimeStage;
			break;
		}
	}

	// 2009-07-02 by cmkwon, 서버 실행시 실행 관련 로그 추가 - 
	g_pFieldGlobal->WriteSystemLogEX(TRUE, "  [Notify] DeclarationNoti CInflWarManager::MSWarStart#       %s WarTimeStage(%d) BossMonsterNum(%8d) ==> RealBossMonsterNum(%8d) BeforeWinCheck(%d) \r\n"
		, CAtumSJ::GetInfluenceTypeString(pCurInfoByWartimeStage->InfluenceType), pCurInfoByWartimeStage->WartimeStage, pCurInfoByWartimeStage->BossMonsterUnitKind, SummonBossMonsterUID, BeforeWinCheck);

	///////////////////////////////////////////////////////////////////////////////
	// 2006-04-18 by cmkwon, 세력전 보스 몬스터 소환 처리
	if(0 != SummonBossMonsterUID)	// 2009-03-10 by dhjin, 단계별 모선 시스템
	{		
		MONSTER_INFO *pBossMon = m_pFieldIOCP14->GetMonsterInfo(SummonBossMonsterUID);	// 2009-03-10 by dhjin, 단계별 모선 시스템
		if(NULL == pBossMon
			|| FALSE == IS_MOTHERSHIPWAR_MONSTER(pBossMon->Belligerence)
			|| FALSE == COMPARE_MPOPTION_BIT(pBossMon->MPOption, MPOPTION_BIT_BOSS_MONSTER))
		{
			char szErr[1024];
			wsprintf(szErr, "CInflWarManager::MSWarStart Error, MonsterUnitKind(%8d) Belligerence(%d)"
				, SummonBossMonsterUID, (NULL==pBossMon)?0:pBossMon->Belligerence);	// 2009-03-10 by dhjin, 단계별 모선 시스템
			g_pFieldGlobal->WriteSystemLog(szErr);
			DbgOut("%s\rn", szErr);
			return;
		}
		
		///////////////////////////////////////////////////////////////////////////////
		// 2006-04-14 by cmkwon, 세력전 보스 몬스터 소환 정보 추가
		SINFLBOSS_MONSTER_SUMMON_DATA tmSummon;
		tmSummon.pMonsterInfo		= pBossMon;
		tmSummon.SummonCount		= pCurInfoByWartimeStage->BossMonsterCount;
		tmSummon.SummonTerm			= pCurInfoByWartimeStage->BossMonsterSummonTerm;
		tmSummon.atimeInsertedTime.SetCurrentDateTime(TRUE);
		tmSummon.SummonMapIndex		= pCurInfoByWartimeStage->BossMonsterSummonMapIndex;
		tmSummon.SummonPosition.x	= pCurInfoByWartimeStage->BossMonsterSummonPositionX;
		tmSummon.SummonPosition.y	= pCurInfoByWartimeStage->BossMonsterSummonPositionY;
		tmSummon.SummonPosition.z	= pCurInfoByWartimeStage->BossMonsterSummonPositionZ;
		tmSummon.BossStep			= BossSummonStep;					// 2009-03-10 by dhjin, 단계별 모선 시스템 - 모선 단계
		tmSummon.BeforeWinCheck		= BeforeWinCheck;			// 2009-03-10 by dhjin, 단계별 모선 시스템 - 전 단계 승패 여부
		m_mtvectSummonMonsterDataList.pushBackLock(tmSummon);

		///////////////////////////////////////////////////////////////////////////////
		// 2006-04-14 by cmkwon, 세력전 보스 몬스터 소환 정보 클라이어트로 전송
		INIT_MSG_WITH_BUFFER(MSG_FC_WAR_BOSS_MONSTER_SUMMON_DATA, T_FC_WAR_BOSS_MONSTER_SUMMON_DATA, pSWarData, SendBuf);
		pSWarData->SummonMonsterUnitkind	= pBossMon->MonsterUnitKind;
		pSWarData->RemainMinute				= pCurInfoByWartimeStage->BossMonsterSummonTerm;
		pSWarData->BossStep					= BossSummonStep;			// 2009-03-10 by dhjin, 단계별 모선 시스템 - 모선 단계
		pSWarData->BeforeWinCheck			= BeforeWinCheck;				// 2009-03-10 by dhjin, 단계별 모선 시스템 - 전 단계 승패 여부
		m_pFieldIOCP14->SendMessageToAllClients(SendBuf, MSG_SIZE(MSG_FC_WAR_BOSS_MONSTER_SUMMON_DATA), INFLUENCE_TYPE_VCN|INFLUENCE_TYPE_ANI);

		//////////////////////////////////////////////////////////////////////////
		// 2008-04-01 by dhjin, 모선전, 거점전 정보창 기획안 - 
		this->SetMSWarInfoResultByWarStart(IS_VCN_INFLUENCE_TYPE(pCurInfoByWartimeStage->InfluenceType) ? INFLUENCE_TYPE_VCN : INFLUENCE_TYPE_ANI, SummonBossMonsterUID, pInflWarData->ContributionPoint);		// 2009-03-10 by dhjin, 단계별 모선 시스템

		// 모선 시작 관련 저장되어야 되는 값
		SDECLARATION_OF_WAR DeclarationOfWar;
		util::zero(&DeclarationOfWar, sizeof(SDECLARATION_OF_WAR));
		DeclarationOfWar.Influence			=	i_byInflTy;
		DeclarationOfWar.MSAppearanceMap	=	pCurInfoByWartimeStage->BossMonsterSummonMapIndex;
		DeclarationOfWar.MSNum				=	SummonBossMonsterUID;		// 2009-03-10 by dhjin, 단계별 모선 시스템
		DeclarationOfWar.NCP				=	pInflWarData->ContributionPoint;
		this->m_DeclarationOfWar.SetMSWarStepByMSWarStart(&DeclarationOfWar);

		QPARAM_UPDATE_START_DECLARATION_OF_WAR *pQParam = new QPARAM_UPDATE_START_DECLARATION_OF_WAR;
		pQParam->Influence					=	i_byInflTy;	
		pQParam->MSWarStep					=	m_DeclarationOfWar.GetCurrentMSWarStep(i_byInflTy);
		pQParam->NCP						=	pInflWarData->ContributionPoint;
		pQParam->MSNum						=	SummonBossMonsterUID;		// 2009-03-10 by dhjin, 단계별 모선 시스템
		pQParam->MSAppearanceMap			=	pCurInfoByWartimeStage->BossMonsterSummonMapIndex;
		m_pFieldIOCP14->m_pAtumDBManager->MakeAndEnqueueQuery(QT_UpdateStartDeclarationOfWar, NULL, 0, pQParam);		
	}

}
 
// 2009-07-02 by cmkwon, 선전포고 관련 버그 수정 - 
/////////////////////////////////////////////////////////////////////////////////
///// \fn			void CInflWarManager::SetInflEventRate()
///// \brief		선전 포고 - 세력전 승리 시 지급 되는 이벤트 수정
///// \author		dhjin
///// \date		2009-03-31 ~ 2009-03-31
///// \warning	
/////
///// \param		
///// \return		
/////////////////////////////////////////////////////////////////////////////////
// void CInflWarManager::SetInflEventRate()
// {
// 	CInflWarData *pBCUInflWarData = GetInflWarDataByInflType(INFLUENCE_TYPE_VCN);
// 	if(pBCUInflWarData)
// 	{
// 		SDB_INFLUENCE_WAR_INFO		*m_pLastMSWinInflInfo;
// 		m_pLastMSWinInflInfo = pBCUInflWarData->GetInflWarInfoByWartimeStage(this->m_DeclarationOfWar.GetLastMSWinStep(INFLUENCE_TYPE_VCN));
// 		if(NULL == m_pLastMSWinInflInfo)
// 		{
// 			pBCUInflWarData->fInflHPRepairRate = 0;
// 			pBCUInflWarData->fInflDPRepairRate = 0;
// 			pBCUInflWarData->fInflSPRepairRate = 0;
// 		}
// 		else
// 		{
// 			pBCUInflWarData->fInflHPRepairRate = m_pLastMSWinInflInfo->HPRepairRate;
// 			pBCUInflWarData->fInflDPRepairRate = m_pLastMSWinInflInfo->DPRepairRate;
// 			pBCUInflWarData->fInflSPRepairRate = m_pLastMSWinInflInfo->SPRepairRate;
// 		}
// 	}
// 
// 	CInflWarData *pANIInflWarData = GetInflWarDataByInflType(INFLUENCE_TYPE_ANI);
// 	if(pANIInflWarData)
// 	{
// 		SDB_INFLUENCE_WAR_INFO		*m_pLastMSWinInflInfo;
// 		m_pLastMSWinInflInfo = pANIInflWarData->GetInflWarInfoByWartimeStage(this->m_DeclarationOfWar.GetLastMSWinStep(INFLUENCE_TYPE_ANI));
// 		if(NULL == m_pLastMSWinInflInfo)
// 		{
// 			pANIInflWarData->fInflHPRepairRate = 0;
// 			pANIInflWarData->fInflDPRepairRate = 0;
// 			pANIInflWarData->fInflSPRepairRate = 0;
// 		}
// 		else
// 		{
// 			pANIInflWarData->fInflHPRepairRate = m_pLastMSWinInflInfo->HPRepairRate;
// 			pANIInflWarData->fInflDPRepairRate = m_pLastMSWinInflInfo->DPRepairRate;
// 			pANIInflWarData->fInflSPRepairRate = m_pLastMSWinInflInfo->SPRepairRate;
// 		}
// 
// 	}
// }


///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2009-07-02 by cmkwon, 선전포고 관련 버그 수정 - 
/// \author		cmkwon
/// \date		2009-07-02 ~ 2009-07-02
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CInflWarManager::SetCompensationForDecalationOfWarWin(BYTE i_byInflTy, INT i_BossMonNum/*=0*/)
{
	g_pFieldGlobal->WriteSystemLogEX(TRUE, "  [Notify] DeclarationNoti CInflWarManager::SetCompensationForDecalationOfWarWin# 100 %s i_BossMonNum(%d) \r\n"
		, CAtumSJ::GetInfluenceTypeString(i_byInflTy), i_BossMonNum);

	CInflWarData *pInflWarData = GetInflWarDataByInflType(i_byInflTy);
	if(NULL == pInflWarData)
	{
		g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ERROR] DeclarationNoti CInflWarManager::SetCompensationForDecalationOfWarWin# 110 %s pInfoWarData(0x%X) \r\n", CAtumSJ::GetInfluenceTypeString(i_byInflTy), pInflWarData);
		return;
	}

	///////////////////////////////////////////////////////////////////////////////
	// 2009-07-03 by cmkwon, 승리한 최고 스텝의 정보를 가져온다.
	INT		nBossMonNum	= i_BossMonNum;
	BYTE	byMSWarStep	= 0;
	if(0 >= nBossMonNum)
	{
		byMSWarStep = m_DeclarationOfWar.GetMostMSWarStepOfWinStep(i_byInflTy, &nBossMonNum);
	}
	if(0 >= nBossMonNum)
	{
		g_pFieldGlobal->WriteSystemLogEX(TRUE, "  [Notify] DeclarationNoti CInflWarManager::SetCompensationForDecalationOfWarWin# 120 %s MostWinBossMonNum(%d) MostMSWarStep(%d) \r\n"
			, CAtumSJ::GetInfluenceTypeString(i_byInflTy), nBossMonNum, byMSWarStep);
		pInflWarData->fInflHPRepairRate		= 0.0f;
		pInflWarData->fInflDPRepairRate		= 0.0f;
		pInflWarData->fInflSPRepairRate		= 0.0f;
		return;
	}

	SDB_INFLUENCE_WAR_INFO *pLastWinWarInfo = pInflWarData->GetInflWarInfoByKillBossMonsterUID(nBossMonNum, i_byInflTy);
	if(NULL == pLastWinWarInfo)
	{
		g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ERROR] DeclarationNoti CInflWarManager::SetCompensationForDecalationOfWarWin# 130 %s MostWinBossMonNum(%d) MostMSWarStep(%d) \r\n"
			, CAtumSJ::GetInfluenceTypeString(i_byInflTy), nBossMonNum, byMSWarStep);
		
		pInflWarData->fInflHPRepairRate		= 0.0f;
		pInflWarData->fInflDPRepairRate		= 0.0f;
		pInflWarData->fInflSPRepairRate		= 0.0f;
		return;
	}
	
	g_pFieldGlobal->WriteSystemLogEX(TRUE, "  [Notify] DeclarationNoti CInflWarManager::SetCompensationForDecalationOfWarWin# 200 %s MostWinBossMonNum(%d) MostMSWarStep(%d) WarTimeStage(%d) \r\n"
		, CAtumSJ::GetInfluenceTypeString(i_byInflTy), nBossMonNum, byMSWarStep, pLastWinWarInfo->WartimeStage);
	
	// 2009-08-20 by cmkwon, 모선전 회복률 버그 수정 - 아래와 같이 마지막 승리 정보로 설정
	//pInflWarData->fInflHPRepairRate		= pInflWarData->GetCurInflWarInfo()->HPRepairRate;
	//pInflWarData->fInflDPRepairRate		= pInflWarData->GetCurInflWarInfo()->DPRepairRate;
	//pInflWarData->fInflSPRepairRate		= pInflWarData->GetCurInflWarInfo()->SPRepairRate;
	pInflWarData->fInflHPRepairRate		= pLastWinWarInfo->HPRepairRate;
	pInflWarData->fInflDPRepairRate		= pLastWinWarInfo->DPRepairRate;
	pInflWarData->fInflSPRepairRate		= pLastWinWarInfo->SPRepairRate;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			INT CInflWarManager::GetMSBossSummonValue(BYTE i_byInflTy, BYTE * o_byBeforeWinCheck)
/// \brief		단계별 모선 시스템 - 이전 주기에 소환된 모선 정보를 바탕으로 새로 소환되어야 되는 모선UID를 계산한다.
/// \author		dhjin
/// \date		2009-03-10 ~ 2009-03-10
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
INT CInflWarManager::GetMSBossSummonValue(BYTE i_byInflTy, BYTE * o_byBeforeWinCheck)
{
	INT SummonBossMonsterUID = 0;
	BYTE EndState = 0;
	this->m_DeclarationOfWar.GetBeforeMSWarEndInfo(i_byInflTy, &SummonBossMonsterUID, &EndState); // 소환되어야되는 모선 UID
	vectDB_INFLUENCE_WAR_INFO *pInflWarInfoList = this->GetInflWarInfoByInflType(i_byInflTy);
	if(NULL == pInflWarInfoList)
	{
		return FALSE;
	}

	if(0 == SummonBossMonsterUID)
	{
		*o_byBeforeWinCheck = MSBOSSSTEP_UNKNOWN_BEFOREWAR;
		return (*pInflWarInfoList)[4].BossMonsterUnitKind;
	}
	
	for(int i=0; i < pInflWarInfoList->size(); i++)
	{
		if(SummonBossMonsterUID == (*pInflWarInfoList)[i].BossMonsterUnitKind
			&& MSWAR_END_WIN == EndState)
		{
			*o_byBeforeWinCheck = MSBOSSSTEP_WIN_BEFOREWAR;
			return (*pInflWarInfoList)[i].WinBossMonsterUnitKind;
		}
		if(SummonBossMonsterUID == (*pInflWarInfoList)[i].BossMonsterUnitKind
			&& MSWAR_END_LOSS == EndState)
		{
			*o_byBeforeWinCheck = MSBOSSSTEP_LOSS_BEFOREWAR;
			return (*pInflWarInfoList)[i].LossBossMonsterUnitKind;
		}
		if(SummonBossMonsterUID == (*pInflWarInfoList)[i].BossMonsterUnitKind
			&& MSWAR_END_WIN != EndState
			&& MSWAR_END_LOSS != EndState)
		{// 모선 진행중 서버 다운으로 인하여 종료가 되지 않았다면 
			*o_byBeforeWinCheck = MSBOSSSTEP_UNKNOWN_BEFOREWAR;
			return SummonBossMonsterUID;
		}
	}
	return FALSE;
}

// 2013-01-16 by jhseol, 지도자 권한에서 케릭터 이름 변경시 통제탑 정보도 실시간 변경
///////////////////////////////////////////////////////////////////////////////
/// \fn			void CInflWarManager::LeaderChangeName(BYTE i_nInfluenceType, USHORT i_nLeaderType, char* i_stName)
/// \brief		지도자 권한에서 케릭터 이름 변경시 통제탑 정보도 실시간 변경
/// \author		jhseol
/// \date		2013-01-16
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CInflWarManager::LeaderChangeName(BYTE i_nInfluenceType, USHORT i_nLeaderType, char* i_stName)
{
	if( COMPARE_INFLUENCE(i_nInfluenceType, INFLUENCE_TYPE_VCN) )
	{
		mt_auto_lock mtBCU(m_VCNInflWarData.GetmtlockPtr());
		if( COMPARE_RACE(i_nLeaderType, RACE_INFLUENCE_LEADER) )
		{
			util::strncpy(m_VCNInflWarData.InflLeaderCharacterName, i_stName, SIZE_MAX_CHARACTER_NAME);
		}
		else if( COMPARE_RACE(i_nLeaderType, RACE_INFLUENCE_SUBLEADER_1) )
		{
			util::strncpy(m_VCNInflWarData.InflSubLeader1CharacterName, i_stName, SIZE_MAX_CHARACTER_NAME);
		}
		else if( COMPARE_RACE(i_nLeaderType, RACE_INFLUENCE_SUBLEADER_2) )
		{
			util::strncpy(m_VCNInflWarData.InflSubLeader2CharacterName, i_stName, SIZE_MAX_CHARACTER_NAME);
		}
	}
	else if( COMPARE_INFLUENCE(i_nInfluenceType, INFLUENCE_TYPE_ANI) )
	{
		mt_auto_lock mtANI(m_ANIInflWarData.GetmtlockPtr());
		if( COMPARE_RACE(i_nLeaderType, RACE_INFLUENCE_LEADER) )
		{
			util::strncpy(m_ANIInflWarData.InflLeaderCharacterName, i_stName, SIZE_MAX_CHARACTER_NAME);
		}
		else if( COMPARE_RACE(i_nLeaderType, RACE_INFLUENCE_SUBLEADER_1) )
		{
			util::strncpy(m_ANIInflWarData.InflSubLeader1CharacterName, i_stName, SIZE_MAX_CHARACTER_NAME);
		}
		else if( COMPARE_RACE(i_nLeaderType, RACE_INFLUENCE_SUBLEADER_2) )
		{
			util::strncpy(m_ANIInflWarData.InflSubLeader2CharacterName, i_stName, SIZE_MAX_CHARACTER_NAME);
		}
	}
}
// end 2013-01-16 by jhseol, 지도자 권한에서 케릭터 이름 변경시 통제탑 정보도 실시간 변경

// 2015-09-14 Future, added events on infl war start & end
void CInflWarManager::OnInfluenceWarStart()
{
	// 2015-09-14 Future, disabled Monster Maps during Influence war
	/*if (!g_pFieldGlobal->GetInfluenceWarDisabledMonsterMaps().empty())
	{
		INIT_MSG_WITH_BUFFER(MSG_FN_NPCSERVER_DELETE_MONSTER_IN_MAPCHANNEL, T_FN_NPCSERVER_DELETE_MONSTER_IN_MAPCHANNEL, pMonsterDelete, SendBuff);

		// Send Monster Delete Requests to the NPC server for each disabled map
		for (auto itr = g_pFieldGlobal->GetInfluenceWarDisabledMonsterMaps().begin()
			; itr != g_pFieldGlobal->GetInfluenceWarDisabledMonsterMaps().end()
			; itr++)
		{

			CFieldMapChannel* pMapChannel = m_pFieldIOCP14->GetFieldMapChannelByMapChannelIndex(MAP_CHANNEL_INDEX(*itr, 0));

			if (pMapChannel)
			{
				pMonsterDelete->mapChann = pMapChannel->GetMapChannelIndex();
				pMonsterDelete->bAllFlag = TRUE;
				pMonsterDelete->bell1 = -1;
				pMonsterDelete->bell2 = -1;
				pMonsterDelete->excludeBell1 = -1;
				pMonsterDelete->excludeBell2 = -1;
				pMonsterDelete->bNotCreateMonster = TRUE;

				m_pFieldIOCP14->Send2NPCServerByTCP(SendBuff, MSG_SIZE(MSG_FN_NPCSERVER_DELETE_MONSTER_IN_MAPCHANNEL));
			}
			else
			{
				g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ERROR] User specified disabled Monster Map by Influence War (Index: %d) not found!", *itr);
			}
		}
	}*/
}

// 2015-09-14 Future, added events on infl war start & end
void CInflWarManager::OnInfluenceWarEnd()
{
	// 2015-09-14 Future, disabled Monster Maps during Influence war
	/*if (!g_pFieldGlobal->GetInfluenceWarDisabledMonsterMaps().empty())
	{
		INIT_MSG_WITH_BUFFER(MSG_FN_MONSTER_CREATE_IN_MAPCHANNEL_BYVALUE, T_FN_MONSTER_CREATE_IN_MAPCHANNEL_BYVALUE, pMonsterCreate, SendBuf);

		// Send Moster Create Request to the NPC Server for each disabled map
		for (auto& mapidx : g_pFieldGlobal->GetInfluenceWarDisabledMonsterMaps())
		{
			CFieldMapChannel* pMapChannel = m_pFieldIOCP14->GetFieldMapChannelByMapChannelIndex(MAP_CHANNEL_INDEX(mapidx, 0));

			if (pMapChannel)
			{
				pMonsterCreate->mapChann = pMapChannel->GetMapChannelIndex();
				pMapChannel->Send2NPCServerW(SendBuf, MSG_SIZE(MSG_FN_MONSTER_CREATE_IN_MAPCHANNEL_BYVALUE));
			}
			
			else server::log(" [ERROR] User specified disabled Monster Map by Influence War (Index: %d) not found!", mapidx);
			
		}
	}*/
}

// 2015-12-17 Future, retrieval of the dominating nation using the NCP
BYTE CInflWarManager::GetDominatingInflByContributionPoints()
{
	// Get NCP of both nations
	int bcuNCP = GetContributionPoint(INFLUENCE_TYPE_VCN);
	int aniNCP = GetContributionPoint(INFLUENCE_TYPE_ANI);

	if (bcuNCP > aniNCP)
		return INFLUENCE_TYPE_VCN;
	else
		return INFLUENCE_TYPE_ANI;
		
}
