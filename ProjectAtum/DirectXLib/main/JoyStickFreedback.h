// JoyStickFreedback.h: interface for the CJoyStickFreedback class.
//
//////////////////////////////////////////////////////////////////////
#include <windows.h>
#include <dinput.h>

#if !defined(AFX_JOYSTICKFREEDBACK_H__88737502_9579_4B73_94EC_B33B643D736D__INCLUDED_)
#define AFX_JOYSTICKFREEDBACK_H__88737502_9579_4B73_94EC_B33B643D736D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CJoyStickFreedback  
{
public:
	CJoyStickFreedback();
	virtual ~CJoyStickFreedback();

	HRESULT InitDirectInput( HWND hDlg, char* i_pJoySelDeviceName );
	VOID FreeDirectInput();
	HRESULT SetDeviceForcesXY(int nForceX, int nForceY);
	void StopDeviceForce();
	BOOL IsEffectStarted();

	INT CoordToForce( INT nCoord );

	void DeviceAcquire();
	void RqEffectStarted(BOOL bSet);

	static BOOL CALLBACK EnumFFDevicesCallback( const DIDEVICEINSTANCE* pInst, VOID* pContext );
	static BOOL CALLBACK EnumAxesCallback( const DIDEVICEOBJECTINSTANCE* pdidoi, VOID* pContext );

	char* GetJoystickFeedDevice();
	
private:
	BOOL				m_bEffectStarted;
	LPDIRECTINPUTEFFECT   m_pEffect ;
	DWORD                 m_dwNumForceFeedbackAxis;
	DWORD					m_dwLastEffectSet;
	BOOL				m_bRqEffectStarted;

};

#endif // !defined(AFX_JOYSTICKFREEDBACK_H__88737502_9579_4B73_94EC_B33B643D736D__INCLUDED_)
