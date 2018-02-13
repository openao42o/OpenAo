// PkNormalTimer.h: interface for the CPkNormalTimer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PKNORMALTIMER_H__E5BB3127_6832_4691_82DF_25D66169C3CF__INCLUDED_)
#define AFX_PKNORMALTIMER_H__E5BB3127_6832_4691_82DF_25D66169C3CF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "windows.h"

class CPkNormalTimer  
{
public:
	CPkNormalTimer();
	virtual ~CPkNormalTimer();
	void		SetPkDelay();
	void		ReleasePkDelay();
	void		SetCityWar();
	void		ReleaseCityWar();
	void		SetImmediatePk();						// 즉시 공격 가능
	void		Tick(float fElapsedTime);
	BOOL		IsPkEnableNormalOrderTarget();
	BOOL		IsCityWar() { return m_bIsCityWar; }	// 현재 공성전 중인가?
	
private:
	float		m_fPkDelayTime;				// PK시 5초 딜레이
	BOOL		m_bPkDelayStart;			// PK 딜레이 시작
	BOOL		m_bIsCityWar;				// 공성전 중인가?
	DWORD		m_dwPkDelayTimeState;		// 시간 표시 상태
};

#endif // !defined(AFX_PKNORMALTIMER_H__E5BB3127_6832_4691_82DF_25D66169C3CF__INCLUDED_)
