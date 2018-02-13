// InfinityTickManager.h: interface for the CInfinityTickManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFINITYTICKMANAGER_H__058D44E4_C20D_4FEA_8AEF_EBB9E5A81616__INCLUDED_)
#define AFX_INFINITYTICKMANAGER_H__058D44E4_C20D_4FEA_8AEF_EBB9E5A81616__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TickManager.h"
#include "InfinityManager.h"

class CInfinityTickManager: public CTickManager  
{
public:
	CInfinityTickManager(CInfinityManager *i_pInfinityManager, DWORD i_nTickInterval = ARENA_TICK_INTERVAL);
	virtual ~CInfinityTickManager();
	
	void DoTickEvent(ATUM_DATE_TIME *pDateTime, TICK_EVENT *pTickEvent);
	void DoEveryTickWork(ATUM_DATE_TIME *pDateTime);	// 매번 수행해야 할 일
	void DoDailyWork(ATUM_DATE_TIME *pDateTime);		// 하루에 한 번 씩 해야 할 일
	void DoHourlyWork(ATUM_DATE_TIME *pDateTime);		// 매 시간마다 한 번 씩 해야 할 일
	void DoMinutelyWork(ATUM_DATE_TIME *pDateTime);		// 매 분마다 한 번 씩 해야 할 일
	void DoSecondlyWork(ATUM_DATE_TIME *pDateTime);		// 매 초마다 한 번 씩 해야 할 일
	void DoMonthlyWork(ATUM_DATE_TIME *pDateTime);
public:
	CInfinityManager			*m_pInfinityManager;
};

#endif // !defined(AFX_INFINITYTICKMANAGER_H__058D44E4_C20D_4FEA_8AEF_EBB9E5A81616__INCLUDED_)
