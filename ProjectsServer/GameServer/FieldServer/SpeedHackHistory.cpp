// SpeedHackHistory.cpp: implementation of the CSpeedHackHistory class.
// 2013-01-29 by hskim, 스피드핵 자동 블럭 기능 구현
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SpeedHackHistory.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSpeedHackHistory::CSpeedHackHistory()
{
	Clear();
}

CSpeedHackHistory::~CSpeedHackHistory()
{
	
}

void CSpeedHackHistory::Clear()
{
	m_nBlockThreshold = SPEED_HACK_HISTORY_THRESHOLD;
	m_nDetectedCount = 0;
	m_atimeResetRecord.SetCurrentDateTime(TRUE);
}

void CSpeedHackHistory::SetBlockThreshold(int nThreshold)
{
	m_nBlockThreshold = nThreshold;
}

void CSpeedHackHistory::InsertResult(BOOL bResult)
{
	ATUM_DATE_TIME tmCurTime;
	tmCurTime.SetCurrentDateTime();

	if( SPEED_HACK_RESET_PERIOD_SEC < abs(tmCurTime.GetTimeDiffTimeInSeconds(m_atimeResetRecord)) )
	{
		m_nDetectedCount = 0;
		m_atimeResetRecord.SetCurrentDateTime();
	}

	if( TRUE == bResult )
	{
		m_nDetectedCount++;
	}
}

BOOL CSpeedHackHistory::IsBlock()
{
	if( m_nDetectedCount >= m_nBlockThreshold )
	{
		return TRUE;
	}
		
	return FALSE;
}
