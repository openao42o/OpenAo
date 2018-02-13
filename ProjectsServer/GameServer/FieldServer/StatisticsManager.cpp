// StatisticsManager.cpp: implementation of the CStatisticsManager class.
// 2012-01-16 by hskim, 통계 - 화패
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "StatisticsManager.h"
#include "FieldGlobal.h"
#include "FieldIOCP.h"
#include "AtumLogSender.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CStatisticsManager::CStatisticsManager(DWORD i_nTickInterval /*= STATISTICS_TICK_INTERVAL*/) : CTickManager(i_nTickInterval)
{
	m_pFieldIOCP = NULL;

	Clear();
}

CStatisticsManager::~CStatisticsManager()
{
	Destroy();
}

void CStatisticsManager::Clear()
{
	for(int i=0; i<STATISTICS_KIND_END; i++)
	{
		m_Money[i][STATISTICS_MONEY_FUNCTION_ADD] = 0;
		m_Money[i][STATISTICS_MONEY_FUNCTION_SUB] = 0;
	}
}

BOOL CStatisticsManager::Create(CFieldIOCP *pFieldIOCP)
{
	m_pFieldIOCP = pFieldIOCP;

	CTickManager::InitThread();

	return TRUE;
}

void CStatisticsManager::Destroy()
{
	Clear();
}

void CStatisticsManager::Add(StatisticsMoneyKind Type, INT64 Value)
{
	if( NULL == m_pFieldIOCP )
	{
		return ;
	}

	if( Type < STATISTICS_SPI || STATISTICS_KIND_END <= Type ) 
	{
		return ;
	}

	if( Value < 0 )
	{
		Value *= -1;
	}

	//mt_auto_lock mtA(&m_Lock);		// 성능상 약간의 오차 발생은 무시 하자

	m_Money[Type][STATISTICS_MONEY_FUNCTION_ADD] += (StatisticsMoneyUnit_t)Value;
}

void CStatisticsManager::Sub(StatisticsMoneyKind Type, INT64 Value)
{
	if( NULL == m_pFieldIOCP )
	{
		return ;
	}

	if( Type < STATISTICS_SPI || STATISTICS_KIND_END <= Type ) 
	{
		return ;
	}

	if( Value < 0 )
	{
		Value *= -1;
	}

	//mt_auto_lock mtA(&m_Lock);		// 성능상 약간의 오차 발생은 무시 하자

	m_Money[Type][STATISTICS_MONEY_FUNCTION_SUB] += (StatisticsMoneyUnit_t)Value;
}

void CStatisticsManager::Sub(ItemNum_t Type, INT64 Value)
{
	if( NULL == m_pFieldIOCP )
	{
		return ;
	}
	
	if( 0 == Type )
	{
		return ;
	}

	switch( Type )
	{
	case ITEM_NUM_SYSTEM_ORB :

		Sub(STATISTICS_SYSTEM_ORB, Value);

		break;

	case ITEM_NUM_QUASAR_ORB :

		Sub(STATISTICS_QUASAR_ORB, Value);

		break;

	case ITEM_NUM_CLUSTER_ORB :

		Sub(STATISTICS_CLUSTER_ORB, Value);

		break;

	case ITEM_NUM_GALAXY_ORG :

		Sub(STATISTICS_GALAXY_ORG, Value);

		break;

	case ITEM_NUM_UNIVERSE_ORB :

		Sub(STATISTICS_UNIVERSE_ORB, Value);

		break;

	default :
		break;
	}
}

void CStatisticsManager::DoTickEvent(ATUM_DATE_TIME *pDateTime, TICK_EVENT *pTickEvent)
{
#ifdef _DEBUG
	DBGOUT(STRMSG_S_F2NOTIFY_0141, pDateTime->GetDateTimeString(STRNBUF(SIZE_MAX_ATUM_DATE_TIME_STRING)));
#endif

	/*
	switch (pTickEvent->TickEventType)
	{
	default:
		break;
	}
	*/

	return;
}

void CStatisticsManager::DoEveryTickWork(ATUM_DATE_TIME *pDateTime)
{
}

void CStatisticsManager::DoDailyWork(ATUM_DATE_TIME *pDateTime)
{
}

void CStatisticsManager::DoHourlyWork(ATUM_DATE_TIME *pDateTime)
{	
}

void CStatisticsManager::DoMinutelyWork(ATUM_DATE_TIME *pDateTime)
{
	MSG_FL_LOG_STATISTICS_MONEY Money;
	
	// 아레나 서버일 경우 저장하지 않는다
	
	if( TRUE == g_pFieldGlobal->IsArenaServer() )
	{
		return ;
	}
	
	// 획득
	
	Money.FunctionType = STATISTICS_MONEY_FUNCTION_ADD;
	
	Money.SPI = m_Money[STATISTICS_SPI][STATISTICS_MONEY_FUNCTION_ADD];
	Money.WarPoint = m_Money[STATISTICS_WAR_POINT][STATISTICS_MONEY_FUNCTION_ADD];
	Money.SystemOrb = m_Money[STATISTICS_SYSTEM_ORB][STATISTICS_MONEY_FUNCTION_ADD];
	Money.QuasarOrb = m_Money[STATISTICS_QUASAR_ORB][STATISTICS_MONEY_FUNCTION_ADD];
	Money.ClusterOrb = m_Money[STATISTICS_CLUSTER_ORB][STATISTICS_MONEY_FUNCTION_ADD];
	Money.GalaxyOrb = m_Money[STATISTICS_GALAXY_ORG][STATISTICS_MONEY_FUNCTION_ADD];
	Money.UniverseOrb = m_Money[STATISTICS_UNIVERSE_ORB][STATISTICS_MONEY_FUNCTION_ADD];
	
	for(int i=0; i<STATISTICS_KIND_END; i++)
	{
		m_Money[i][STATISTICS_MONEY_FUNCTION_ADD] = 0;
	}
	
	CAtumLogSender::SendLogMessageStatisticsMeney(&Money);
	
	// 차감
	
	Money.FunctionType = STATISTICS_MONEY_FUNCTION_SUB;
	
	Money.SPI = m_Money[STATISTICS_SPI][STATISTICS_MONEY_FUNCTION_SUB];
	Money.WarPoint = m_Money[STATISTICS_WAR_POINT][STATISTICS_MONEY_FUNCTION_SUB];
	Money.SystemOrb = m_Money[STATISTICS_SYSTEM_ORB][STATISTICS_MONEY_FUNCTION_SUB];
	Money.QuasarOrb = m_Money[STATISTICS_QUASAR_ORB][STATISTICS_MONEY_FUNCTION_SUB];
	Money.ClusterOrb = m_Money[STATISTICS_CLUSTER_ORB][STATISTICS_MONEY_FUNCTION_SUB];
	Money.GalaxyOrb = m_Money[STATISTICS_GALAXY_ORG][STATISTICS_MONEY_FUNCTION_SUB];
	Money.UniverseOrb = m_Money[STATISTICS_UNIVERSE_ORB][STATISTICS_MONEY_FUNCTION_SUB];
	
	for(int j=0; j<STATISTICS_KIND_END; j++)
	{
		m_Money[j][STATISTICS_MONEY_FUNCTION_SUB] = 0;
	}
	
	CAtumLogSender::SendLogMessageStatisticsMeney(&Money);	
}

void CStatisticsManager::DoSecondlyWork(ATUM_DATE_TIME *pDateTime)
{
}

void CStatisticsManager::DoMonthlyWork(ATUM_DATE_TIME *pDateTime)
{
}
