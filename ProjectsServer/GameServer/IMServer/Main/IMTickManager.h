#ifndef _IM_TICK_MANAGER_H_
#define _IM_TICK_MANAGER_H_

#include "TickManager.h"
#include "IMIOCPSocket.h"

#define IM_DEFAULT_TICK_INTERVAL			500		// 0.5 second

// event types
#define IM_TICK_EVENT_START_GUILD_WAR		0

class CIMTickManager : public CTickManager
{
public:
	CIMTickManager(CIMIOCP *i_pIMIOCP, DWORD i_nTickInterval = IM_DEFAULT_TICK_INTERVAL);
	virtual ~CIMTickManager();

	void DoTickEvent(ATUM_DATE_TIME *pDateTime, TICK_EVENT *pTickEvent);
	void DoEveryTickWork(ATUM_DATE_TIME *pDateTime);	// 매번 수행해야 할 일
	void DoDailyWork(ATUM_DATE_TIME *pDateTime);		// 하루에 한 번 씩 해야 할 일
	void DoHourlyWork(ATUM_DATE_TIME *pDateTime);		// 매 시간마다 한 번 씩 해야 할 일
	void DoMinutelyWork(ATUM_DATE_TIME *pDateTime);		// 매 분마다 한 번 씩 해야 할 일
	void DoSecondlyWork(ATUM_DATE_TIME *pDateTime);		// 매 초마다 한 번 씩 해야 할 일
	void DoMonthlyWork(ATUM_DATE_TIME *pDateTime);	// 2005-12-27 by cmkwon, 한달에 한 번 씩 해야 할 일

public:
	CIMIOCP		*m_pIMIOCP4;
};

#endif // _ATUM_IM_TICK_MANAGER_H_
