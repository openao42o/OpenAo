// CountdownChecker.cpp: implementation of the CCountdownChecker class.
//Copyright[2002] MasangSoft
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CountdownChecker.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCountdownChecker::CCountdownChecker()
{
    memset(m_arrCountdownCheck, 0x00, sizeof(m_arrCountdownCheck[0]) * COUNT_MAX_COUNTDOWN_COUNT);
}

CCountdownChecker::~CCountdownChecker()
{

}


BOOL CCountdownChecker::IsValidCountType(BYTE i_byCountType)
{
    if(i_byCountType >= COUNT_MAX_COUNTDOWN_COUNT)
    {
        return FALSE;
    }

    return TRUE;
}

void CCountdownChecker::ResetCountdownChecker(void)
{
    memset(m_arrCountdownCheck, 0x00, sizeof(m_arrCountdownCheck[0]) * COUNT_MAX_COUNTDOWN_COUNT);
}


BOOL CCountdownChecker::StartCountDown(BYTE i_byCountType, DWORD i_dwMSTimeGap, DWORD i_dwCurTick)
{
    if(FALSE == IsValidCountType(i_byCountType))
    {
        return FALSE;
    }

    m_arrCountdownCheck[i_byCountType].bStartedFlag            = TRUE;
    m_arrCountdownCheck[i_byCountType].dwMSCountdownTimeGap    = i_dwMSTimeGap;
    m_arrCountdownCheck[i_byCountType].dwStartedTick        = i_dwCurTick;
    return TRUE;
}


BOOL CCountdownChecker::IsEndedCountDown(BYTE i_byCountType, DWORD i_dwCurTick)
{
    if(FALSE == IsValidCountType(i_byCountType))
    {
        return FALSE;
    }

    if(FALSE == m_arrCountdownCheck[i_byCountType].bStartedFlag
        || i_dwCurTick - m_arrCountdownCheck[i_byCountType].dwStartedTick < m_arrCountdownCheck[i_byCountType].dwMSCountdownTimeGap-500)
    {
        DBGOUT("CCountdownChecker::IsEndedCountDown Countdown Error, CountType(%d) StartFlag(%2d) TimeGap(%5d)\n"
            , i_byCountType, m_arrCountdownCheck[i_byCountType].bStartedFlag, i_dwCurTick - m_arrCountdownCheck[i_byCountType].dwStartedTick);
        return FALSE;
    }

    m_arrCountdownCheck[i_byCountType].bStartedFlag            = FALSE;
    m_arrCountdownCheck[i_byCountType].dwMSCountdownTimeGap    = 0;
    m_arrCountdownCheck[i_byCountType].dwStartedTick        = 0;
    return TRUE;
}
