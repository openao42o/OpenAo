#ifndef _TICK_MANAGER_H_
#define _TICK_MANAGER_H_

#include "AtumParam.h"
#include "mt_stl.h"

#include <mutex>
#include <thread>

struct TICK_EVENT
{
	INT		TickEventType;
	INT		nTickEventParam1;
	INT		nTickEventParam2;
	void	*vpTickEventParam1;
	void	*vpTickEventParam2;
};

class CTickManager
{
public:
	explicit CTickManager(DWORD i_nTickInterval);
	virtual ~CTickManager() { CleanThread(); }

	DWORD ThreadWorker();

	void SetTickInterval(DWORD i_nTickInterval) { m_nTickInterval = i_nTickInterval; }
	DWORD GetTickInterval() const { return m_nTickInterval; }

	virtual void DoTickEvent(ATUM_DATE_TIME *pDateTime, TICK_EVENT *pTickEvent) = 0;
	virtual void DoEveryTickWork(ATUM_DATE_TIME *pDateTime) = 0;	// 매번 수행해야 할 일
	virtual void DoDailyWork(ATUM_DATE_TIME *pDateTime) = 0;		// 하루에 한 번 씩 해야 할 일
	virtual void DoHourlyWork(ATUM_DATE_TIME *pDateTime) = 0;		// 매 시간마다 한 번 씩 해야 할 일
	virtual void DoMinutelyWork(ATUM_DATE_TIME *pDateTime) = 0;		// 매 분마다 한 번 씩 해야 할 일
	virtual void DoSecondlyWork(ATUM_DATE_TIME *pDateTime) = 0;		// 매 초마다 한 번 씩 해야 할 일
	virtual void DoMonthlyWork(ATUM_DATE_TIME *pDateTime) = 0;		// 2005-12-27 by cmkwon, 한달에 한 번 씩 해야 할 일

	void InsertTickEvent(INT			i_TickEventType,
						ATUM_DATE_TIME	*i_pStartTime,
						INT				i_nTickEventParam1 = 0,
						INT				i_nTickEventParam2 = 0,
						void			*i_vpTickEventParam1 = NULL,
						void			*i_vpTickEventParam2 = NULL);

	void InsertTickEvent(INT			i_TickEventType,
						INT				i_nTimeIntervalInSeconds,
						INT				i_nTickEventParam1 = 0,
						INT				i_nTickEventParam2 = 0,
						void			*i_vpTickEventParam1 = NULL,
						void			*i_vpTickEventParam2 = NULL);

private:
	void ProcessTickEvent(ATUM_DATE_TIME *i_pDateTime);

public:
	ATUM_DATE_TIME	m_nPrevDateTime;
	DWORD			m_nTickInterval;	// in milli-seconds

private:
	mt_multimap<ATUM_DATE_TIME, TICK_EVENT>	m_multimapTickEvent;

public:
	bool InitThread();
	bool CleanThread();

	void SetShutDownFlag()
	{
		lock_guard<mutex> { m_mutexWorker };

		m_flagWorker = true;
	}

	bool GetShutDownFlag()
	{
		lock_guard<mutex> { m_mutexWorker };

		return m_flagWorker;
	}

	cv_status SleepAndWaitForEvent(DWORD duration)
	{
		unique_lock<mutex> lock { m_mutexWorker };

		return m_eventWorker.wait_for(lock, chrono::milliseconds(duration));
	}

protected:
	thread			m_threadWorker;
	DWORD			m_dwThreadID;

private:
	using condition = condition_variable;

	mutex			m_mutexWorker;
	bool			m_flagWorker;
	condition		m_eventWorker;
};

#endif // _ATUM_TICK_MANAGER_H_
