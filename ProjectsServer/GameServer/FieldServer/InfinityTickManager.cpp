// InfinityTickManager.cpp: implementation of the CInfinityTickManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "InfinityTickManager.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CInfinityTickManager::CInfinityTickManager(CInfinityManager *i_pInfinityManager, DWORD i_nTickInterval /*= ARENA_TICK_INTERVAL */)
: CTickManager(i_nTickInterval)
{
	m_pInfinityManager = i_pInfinityManager;
}

CInfinityTickManager::~CInfinityTickManager()
{

}

void CInfinityTickManager::DoTickEvent(ATUM_DATE_TIME *pDateTime, TICK_EVENT *pTickEvent)
{
#ifdef _DEBUG
	DBGOUT(STRMSG_S_F2NOTIFY_0141, pDateTime->GetDateTimeString(STRNBUF(SIZE_MAX_ATUM_DATE_TIME_STRING)));
#endif
	
	return;
}

void CInfinityTickManager::DoEveryTickWork(ATUM_DATE_TIME *pDateTime)
{
}

void CInfinityTickManager::DoDailyWork(ATUM_DATE_TIME *pDateTime)
{
	
}

void CInfinityTickManager::DoHourlyWork(ATUM_DATE_TIME *pDateTime)
{
	
}

void CInfinityTickManager::DoMinutelyWork(ATUM_DATE_TIME *pDateTime)
{
}

void CInfinityTickManager::DoSecondlyWork(ATUM_DATE_TIME *pDateTime)
{
	m_pInfinityManager->DoSecondlyWorkInfinity(pDateTime);
}

void CInfinityTickManager::DoMonthlyWork(ATUM_DATE_TIME *pDateTime)
{
	
}