// INFImageAnimation.h: interface for the INFImageAnimation class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFCOUNTDOWN_H__7B409C14_93C0_4282_B7B5_8367FF326F11__INCLUDED_)
#define AFX_INFCOUNTDOWN_H__7B409C14_93C0_4282_B7B5_8367FF326F11__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

typedef struct
{
	BYTE		byCountdownType;
	DWORD		dwColorType;
	BOOL		bRenderFlag;
	int			nOldCount;

	float		fCountTimer;
	float		fFlowCount;
	char		szBattleName[32];
} CountDown_t;

class CINFGameCountDown
{

public:
	CINFGameCountDown();
	virtual ~CINFGameCountDown();
	//virtual HRESULT InitDeviceObjects();
	//virtual HRESULT RestoreDeviceObjects();
	//virtual HRESULT DeleteDeviceObjects();
	//virtual HRESULT InvalidateDeviceObjects();
	virtual void Render();
	virtual void Tick();
	
	void SetCountDown(BYTE byCountdownType, float fCountTime, DWORD dwColorType);	
	
protected:
	vector<CountDown_t>		m_vecCountDown;
	int						m_nOldCount;

};

#endif
