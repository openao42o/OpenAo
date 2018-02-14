#include "stdafx.h"
#include "TickManager.h"

CTickManager::CTickManager(DWORD i_nTickInterval) :
	m_nPrevDateTime { },
	m_nTickInterval { i_nTickInterval },
	m_multimapTickEvent { },
	m_threadWorker { },
	m_dwThreadID { 0 },
	m_mutexWorker { },
	m_flagWorker { false },
	m_eventWorker { }
{
	m_nPrevDateTime.Reset();
}

DWORD CTickManager::ThreadWorker()
{
	while (true)
	{
		// first sleep and check if there is an event
		auto status = SleepAndWaitForEvent(m_nTickInterval);
		
		// have we been asked to quit?
		if (status == cv_status::no_timeout && GetShutDownFlag()) break;

		// process all events
		ATUM_DATE_TIME currentDateTime;
		currentDateTime.SetCurrentDateTime();

		ProcessTickEvent(&currentDateTime);

		DoEveryTickWork(&currentDateTime);

		if(currentDateTime.Month != m_nPrevDateTime.Month)
			DoMonthlyWork(&currentDateTime);

		if (currentDateTime.Day != m_nPrevDateTime.Day)
			DoDailyWork(&currentDateTime);

		if (currentDateTime.Hour != m_nPrevDateTime.Hour)
			DoHourlyWork(&currentDateTime);

		if (currentDateTime.Minute != m_nPrevDateTime.Minute)
			DoMinutelyWork(&currentDateTime);

		if (currentDateTime.Second != m_nPrevDateTime.Second)
			DoSecondlyWork(&currentDateTime);

		m_nPrevDateTime = currentDateTime;
	} // end while

	return 5;
}

void CTickManager::ProcessTickEvent(ATUM_DATE_TIME *i_pDateTime)
{
	mt_multimap<ATUM_DATE_TIME, TICK_EVENT>::iterator itrMain;
	mt_multimap<ATUM_DATE_TIME, TICK_EVENT>::iterator itrEnd;

	vector<TICK_EVENT> vecTickEvent;		// p

	m_multimapTickEvent.lock();

	itrEnd = m_multimapTickEvent.upper_bound(*i_pDateTime);
	if (itrEnd == m_multimapTickEvent.begin())
	{
		m_multimapTickEvent.unlock();
		return;
	}

	for (itrMain = m_multimapTickEvent.begin(); itrMain != itrEnd; ++itrMain)
	{
		///////////////////////////////////////////////////////////////////////////////////////
		// s
		//
		// p
		//
		// DoTickEvent(i_pDateTime, &itrMain->second);

		/////////
		// p

		vecTickEvent.push_back(itrMain->second);

		// end 2
		///////////////////////////////////////////////////////////////////////////////////////
	}

	m_multimapTickEvent.erase(m_multimapTickEvent.begin(), itrEnd);
	m_multimapTickEvent.unlock();

	// start 2011-10-10 by h
	for(int i=0; i<vecTickEvent.size(); i++)
	{
		DoTickEvent(i_pDateTime, &vecTickEvent[i]);
	}
	// end 2011-10-10 by hskim, EP
}

void CTickManager::InsertTickEvent(INT i_TickEventType, ATUM_DATE_TIME *i_pStartTime, INT i_nTickEventParam1 /* = 0 */, INT i_nTickEventParam2 /* = 0 */, void *i_vpTickEventParam1 /* = NULL */, void *i_vpTickEventParam2 /* = NULL */)
{
	TICK_EVENT tmpEvent;
	tmpEvent.TickEventType		= i_TickEventType;
	tmpEvent.nTickEventParam1	= i_nTickEventParam1;
	tmpEvent.nTickEventParam2	= i_nTickEventParam2;
	tmpEvent.vpTickEventParam1	= i_vpTickEventParam1;
	tmpEvent.vpTickEventParam2	= i_vpTickEventParam2;

	m_multimapTickEvent.lock();

	m_multimapTickEvent.insertNoLock(*i_pStartTime, tmpEvent);

	m_multimapTickEvent.unlock();

//#ifdef _DEBUG
//	DBGOUT("n", i_pStartTime->GetDateTimeString(STRNBUF(SIZE_MAX_ATUM_DATE_TIME_STRING)));
//#endif
}

void CTickManager::InsertTickEvent(INT i_TickEventType, INT i_nTimeIntervalInSeconds, INT i_nTickEventParam1 /* = 0 */, INT i_nTickEventParam2 /* = 0 */, void *i_vpTickEventParam1 /* = NULL */, void *i_vpTickEventParam2 /* = NULL */)
{
	ATUM_DATE_TIME tmpDateTime;
	tmpDateTime.SetCurrentDateTime();
	tmpDateTime.AddDateTime(0, 0, 0, 0, 0, i_nTimeIntervalInSeconds);

	InsertTickEvent(i_TickEventType, &tmpDateTime, i_nTickEventParam1, i_nTickEventParam2, i_vpTickEventParam1, i_vpTickEventParam2);
}


DWORD WINAPI __WorkerThread(LPVOID lpParam)
{
	if (!lpParam) return 0x100;

	return static_cast<CTickManager*>(lpParam)->ThreadWorker();
}

bool CTickManager::InitThread()
{
	m_flagWorker = false;
	m_dwThreadID = 0;

	try
	{
		m_threadWorker = thread { __WorkerThread, LPVOID(this) };

		return true;
	}
	catch (const system_error& error)
	{
		return false;
	}

}

bool CTickManager::CleanThread()
{
	if (m_threadWorker.joinable())
	{
		SetShutDownFlag();

		m_eventWorker.notify_one();

		m_threadWorker.join();

		m_flagWorker = false;
		m_dwThreadID = 0;

		return true;
	}

	return false;
}
