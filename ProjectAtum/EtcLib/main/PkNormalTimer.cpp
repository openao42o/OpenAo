// PkNormalTimer.cpp: implementation of the CPkNormalTimer class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "AtumApplication.h"
#include "Chat.h"
#include "PkNormalTimer.h"



#define DELAY_TIME	10.0f			// 2005-08-08 by ispark 딜레이 5초 -> 10초 변경

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPkNormalTimer::CPkNormalTimer()
{
	m_fPkDelayTime = DELAY_TIME;
	m_bPkDelayStart = FALSE;
	m_bIsCityWar = FALSE;
	m_dwPkDelayTimeState = (DWORD)DELAY_TIME;
}

CPkNormalTimer::~CPkNormalTimer()
{

}

void CPkNormalTimer::SetPkDelay()
{
	m_fPkDelayTime = DELAY_TIME;
	m_bPkDelayStart = TRUE;
	m_dwPkDelayTimeState = (DWORD)DELAY_TIME;
}

void CPkNormalTimer::ReleasePkDelay()
{
	m_fPkDelayTime = DELAY_TIME;
	m_bPkDelayStart = FALSE;
	m_dwPkDelayTimeState = (DWORD)DELAY_TIME;
}

void CPkNormalTimer::SetCityWar()
{
	m_bIsCityWar = TRUE;
}

void CPkNormalTimer::ReleaseCityWar()
{
	m_bIsCityWar = FALSE;
}

void CPkNormalTimer::Tick(float fElapsedTime)
{
	if(m_bPkDelayStart == TRUE && m_bIsCityWar == FALSE)
	{
		m_fPkDelayTime -= fElapsedTime;
		if(m_fPkDelayTime <= 0.0f)
		{
			g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_PK_0001,COLOR_RED);	//"0초 공격 가능합니다."
			m_fPkDelayTime = 0.0f;
			m_bPkDelayStart = FALSE;
		}
		else if(m_fPkDelayTime <= m_dwPkDelayTimeState)
		{
			char str[32];
			wsprintf(str,STRMSG_C_PK_0002,m_dwPkDelayTimeState);//"%d초"
			g_pD3dApp->m_pChat->CreateChatChild(str,COLOR_RED);	
			m_dwPkDelayTimeState--;
		}
	}
}

BOOL CPkNormalTimer::IsPkEnableNormalOrderTarget()
{
	if(m_fPkDelayTime == 0.0f || m_bIsCityWar == TRUE)
	{
		return TRUE;
	}
	return FALSE;
}


void CPkNormalTimer::SetImmediatePk()
{
	m_fPkDelayTime = 0.0f;
	m_bPkDelayStart = FALSE;
}