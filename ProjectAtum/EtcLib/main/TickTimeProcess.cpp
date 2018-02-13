// CTickTimeProcess.cpp: implementation of the INFImageAnimation class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TickTimeProcess.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTickTimeProcess::CTickTimeProcess()
{
	FLOG( "CTickTimeProcess()" );

	m_vecClientTimer.reserve(40);
	m_vecClientTimer.clear();

	m_fFlowTotalTime = 0.0f;
}

CTickTimeProcess::~CTickTimeProcess()
{
	FLOG( "~CTickTimeProcess()" );
	
	m_vecClientTimer.clear();
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			TickTimeClientProcess(float fElapsedTime)
/// \brief		서버에서 메세지를 받아 클라이언트에서 Tick 타임계산 
/// \author		ydkim
/// \date		2004-10-01~
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CTickTimeProcess::Tick(float fElapsedTime)
{
	FLOG( "CTickTimeProcess::Tick()" );

	// 변경
	m_fFlowTotalTime = m_fFlowTotalTime + fElapsedTime;

	vector<ClientTimer_t>::iterator it = m_vecClientTimer.begin();
	while(it != m_vecClientTimer.end())
	{
		if(it->bPause == FALSE)
		{
			//it->fTimeflow = m_fFlowTotalTime*1000;
			if(it->fTimeflow < m_fFlowTotalTime)
			{
				it->fTimeflow += ((float)it->mexTimerEvent.TimeInterval)/1000;
				// 시간 지났다 메세지 보내라				
				MSG_FC_TIMER_TIMEOUT sMsg;				
//				DBGOUT("틱 타임 테스트 Interval(%d) Size(%d)\n", (int *)it->mexTimerEvent.TimeInterval, m_vecClientTimer.size());
				if(it->mexTimerEvent.bRepeatable)
				{
//					DBGOUT("틱 타임 테스트 리피터블 Interval(%d) bRepeatable(%d) nRemainedRepeatCount(%d)\n",
//						(int *)it->mexTimerEvent.TimeInterval, it->mexTimerEvent.bRepeatable, it->mexTimerEvent.nRemainedRepeatCount);
					it->mexTimerEvent.nRemainedRepeatCount--;
					
					sMsg.TimerEvent = it->mexTimerEvent;				
					g_pFieldWinSocket->SendMsg( T_FC_TIMER_TIMEOUT, (char*)&sMsg, sizeof(sMsg) );

					if(it->mexTimerEvent.nRemainedRepeatCount <= 0)
					{
						it = m_vecClientTimer.erase(it);
						continue;
					}					
				}
				else
				{
					sMsg.TimerEvent = it->mexTimerEvent;				
					g_pFieldWinSocket->SendMsg( T_FC_TIMER_TIMEOUT, (char*)&sMsg, sizeof(sMsg) );
//					DBGOUT("보내는 UID %d\n",sMsg.TimerEvent.TimerUID);					
//					DBGOUT("틱 타임 테스트 이레이져 Interval(%d) Size(%d)\n",
//						(int *)it->mexTimerEvent.TimeInterval,m_vecClientTimer.size());
					it = m_vecClientTimer.erase(it);
					continue;
				}				

				
			}
		}

		it++;
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			TickTimeClientProcess(float fElapsedTime)
/// \brief		서버에서 메세지를 받아 클라이언트에서 Tick 타임계산 
/// \author		ydkim
/// \date		2004-10-01~
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CTickTimeProcess::StartTimer(MSG_FC_TIMER_START_TIMER* pMsg)
{
	ClientTimer_t timeTemp;
	timeTemp.mexTimerEvent = pMsg->TimerEvent;
	timeTemp.bPause = FALSE;
	timeTemp.fTimeflow = m_fFlowTotalTime + ((float)pMsg->TimerEvent.TimeInterval)/1000;

	m_vecClientTimer.push_back(timeTemp);	
}

void CTickTimeProcess::StopTimer(MSG_FC_TIMER_STOP_TIMER* pMsg)
{
	vector<ClientTimer_t>::iterator it = m_vecClientTimer.begin();
	while(it != m_vecClientTimer.end())
	{
		if(it->mexTimerEvent.TimerUID == pMsg->TimerUID)
		{
			m_vecClientTimer.erase(it);

			break;
		}
		it++;
	}
}

void CTickTimeProcess::UpdateTimer(MSG_FC_TIMER_UPDATE_TIMER* pMsg)
{
	vector<ClientTimer_t>::iterator it = m_vecClientTimer.begin();
	while(it != m_vecClientTimer.end())
	{
		if(it->mexTimerEvent.TimerUID == pMsg->TimerEvent.TimerUID)
		{
			ClientTimer_t timeTemp;
			timeTemp.mexTimerEvent = pMsg->TimerEvent;
			timeTemp.bPause = FALSE;
			timeTemp.fTimeflow = pMsg->TimerEvent.TimeInterval;

			m_vecClientTimer.erase(it);
			m_vecClientTimer.push_back(timeTemp);

			break;
		}
		it++;
	}
}

void CTickTimeProcess::PauseTimer(MSG_FC_TIMER_PAUSE_TIMER* pMsg)
{
	vector<ClientTimer_t>::iterator it = m_vecClientTimer.begin();
	while(it != m_vecClientTimer.end())
	{
		if(it->mexTimerEvent.TimerUID == pMsg->TimerUID)
		{
			it->bPause = TRUE;
		}
		it++;
	}
}

void CTickTimeProcess::ContinueTimer(MSG_FC_TIMER_CONTINUE_TIMER* pMsg)
{
	vector<ClientTimer_t>::iterator it = m_vecClientTimer.begin();
	while(it != m_vecClientTimer.end())
	{
		if(it->mexTimerEvent.TimerUID == pMsg->TimerUID)
		{
			it->bPause = FALSE;
		}
		it++;
	}
}