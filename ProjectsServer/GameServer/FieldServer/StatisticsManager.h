// StatisticsManager.h: interface for the CStatisticsManager class.
// 2012-01-16 by hskim, 통계 - 화패
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STATISTICSMANAGER_H__7CA872CF_FA6B_4626_A124_52C1D2FF36CE__INCLUDED_)
#define AFX_STATISTICSMANAGER_H__7CA872CF_FA6B_4626_A124_52C1D2FF36CE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TickManager.h"

class CFieldIOCP;

class CStatisticsManager : public CTickManager
{
public:
	CStatisticsManager(DWORD i_nTickInterval = STATISTICS_TICK_INTERVAL);
	virtual ~CStatisticsManager();

	void Clear();
	BOOL Create(CFieldIOCP *pFieldIOCP);
	void Destroy();

	void Add(StatisticsMoneyKind Type, INT64 Value);
	void Sub(StatisticsMoneyKind Type, INT64 Value);
	void Sub(ItemNum_t ItemNum, INT64 Value);

public:
	void DoTickEvent(ATUM_DATE_TIME *pDateTime, TICK_EVENT *pTickEvent);
	void DoEveryTickWork(ATUM_DATE_TIME *pDateTime);
	void DoDailyWork(ATUM_DATE_TIME *pDateTime);
	void DoHourlyWork(ATUM_DATE_TIME *pDateTime);
	void DoMinutelyWork(ATUM_DATE_TIME *pDateTime);
	void DoSecondlyWork(ATUM_DATE_TIME *pDateTime);
	void DoMonthlyWork(ATUM_DATE_TIME *pDateTime);

public:
	CFieldIOCP	*m_pFieldIOCP;

	StatisticsMoneyUnit_t	m_Money[STATISTICS_KIND_END][STATISTICS_MONEY_FUNCTION_MAX];
};

#endif // !defined(AFX_STATISTICSMANAGER_H__7CA872CF_FA6B_4626_A124_52C1D2FF36CE__INCLUDED_)
