// JoyStickFreedback.cpp: implementation of the CJoyStickFreedback class.
//
//////////////////////////////////////////////////////////////////////
#define STRICT
#define DIRECTINPUT_VERSION 0x0800


#include "JoyStickFreedback.h"

#include "DXUtil.h"
#include "FunctionLog.h"
#include "FunctionLogGFunctions.h"
#include <stdio.h>
#include <math.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

LPDIRECTINPUT8        g_pDI       = NULL;         
LPDIRECTINPUTDEVICE8  g_pDevice   = NULL;

//char						g_chJoystickCtrlDevice[MAX_PATH];
char						g_chJoystickFeedDevice[MAX_PATH];


CJoyStickFreedback::CJoyStickFreedback()
{
	m_bRqEffectStarted = FALSE;
	m_bEffectStarted = FALSE;
	m_pEffect = NULL;
	g_pDI       = NULL;         
	g_pDevice   = NULL;
	m_dwNumForceFeedbackAxis = 0;
	m_dwLastEffectSet = 0;

	memset(g_chJoystickFeedDevice, 0x00,  MAX_PATH);
}

CJoyStickFreedback::~CJoyStickFreedback()
{
	memset(g_chJoystickFeedDevice, 0x00,  MAX_PATH);

	FreeDirectInput();
}


//-----------------------------------------------------------------------------
// Name: InitDirectInput()
// Desc: Initialize the DirectInput variables.
//-----------------------------------------------------------------------------
HRESULT CJoyStickFreedback::InitDirectInput( HWND hDlg, char* i_pJoySelDeviceName )
{
    DIPROPDWORD dipdw;
    HRESULT     hr;

	strncpy(g_chJoystickFeedDevice, i_pJoySelDeviceName, MAX_PATH);

    // Register with the DirectInput subsystem and get a pointer
    // to a IDirectInput interface we can use.
    if( FAILED( hr = DirectInput8Create( GetModuleHandle(NULL), DIRECTINPUT_VERSION, 
                                         IID_IDirectInput8, (VOID**)&g_pDI, NULL ) ) )
    {
        return hr;
    }
    
    // Look for a force feedback device we can use
    if( FAILED( hr = g_pDI->EnumDevices( DI8DEVCLASS_GAMECTRL, 
                                         EnumFFDevicesCallback, NULL, 
                                         DIEDFL_ATTACHEDONLY | DIEDFL_FORCEFEEDBACK ) ) )
    {
        return hr;
    }

    if( NULL == g_pDevice )
    {
//        MessageBox( NULL, _T("Force feedback device not found. ")
//                          _T("The sample will now exit."), 
//                          _T("FFConst"), MB_ICONERROR | MB_OK );
//        EndDialog( hDlg, 0 );
        return -1;
    }

    // Set the data format to "simple joystick" - a predefined data format. A
    // data format specifies which controls on a device we are interested in,
    // and how they should be reported.
    //
    // This tells DirectInput that we will be passing a DIJOYSTATE structure to
    // IDirectInputDevice8::GetDeviceState(). Even though we won't actually do
    // it in this sample. But setting the data format is important so that the
    // DIJOFS_* values work properly.
    if( FAILED( hr = g_pDevice->SetDataFormat( &c_dfDIJoystick ) ) )
        return hr;

    // Set the cooperative level to let DInput know how this device should
    // interact with the system and with other DInput applications.
    // Exclusive access is required in order to perform force feedback.
    if( FAILED( hr = g_pDevice->SetCooperativeLevel( hDlg,
                                                     DISCL_EXCLUSIVE | 
                                                     DISCL_FOREGROUND ) ) )
    {
        return hr;
    }

    // Since we will be playing force feedback effects, we should disable the
    // auto-centering spring.
    dipdw.diph.dwSize       = sizeof(DIPROPDWORD);
    dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
    dipdw.diph.dwObj        = 0;
    dipdw.diph.dwHow        = DIPH_DEVICE;
    dipdw.dwData            = FALSE;

    if( FAILED( hr = g_pDevice->SetProperty( DIPROP_AUTOCENTER, &dipdw.diph ) ) )
        return hr;

    // Enumerate and count the axes of the joystick 
    if ( FAILED( hr = g_pDevice->EnumObjects( EnumAxesCallback, 
                                              (VOID*)&m_dwNumForceFeedbackAxis, DIDFT_AXIS ) ) )
        return hr;

    // This simple sample only supports one or two axis joysticks
    if( m_dwNumForceFeedbackAxis > 2 )
        m_dwNumForceFeedbackAxis = 2;

    // This application needs only one effect: Applying raw forces.
    DWORD           rgdwAxes[2]     = { DIJOFS_X, DIJOFS_Y };
    LONG            rglDirection[2] = { 0, 0 };
    DICONSTANTFORCE cf              = { 0 };

    DIEFFECT eff;
    ZeroMemory( &eff, sizeof(eff) );
    eff.dwSize                  = sizeof(DIEFFECT);
    eff.dwFlags                 = DIEFF_CARTESIAN | DIEFF_OBJECTOFFSETS;
    eff.dwDuration              = INFINITE;
    eff.dwSamplePeriod          = 0;
    eff.dwGain                  = DI_FFNOMINALMAX;
    eff.dwTriggerButton         = DIEB_NOTRIGGER;
    eff.dwTriggerRepeatInterval = 0;
    eff.cAxes                   = m_dwNumForceFeedbackAxis;
    eff.rgdwAxes                = rgdwAxes;
    eff.rglDirection            = rglDirection;
    eff.lpEnvelope              = 0;
    eff.cbTypeSpecificParams    = sizeof(DICONSTANTFORCE);
    eff.lpvTypeSpecificParams   = &cf;
    eff.dwStartDelay            = 0;

    // Create the prepared effect
    if( FAILED( hr = g_pDevice->CreateEffect( GUID_ConstantForce, 
                                              &eff, &m_pEffect, NULL ) ) )
    {
        return hr;
    }

    if( NULL == m_pEffect )
        return E_FAIL;

	// Init the time of the last force feedback effect
    m_dwLastEffectSet = timeGetTime();

    return S_OK;
}

//-----------------------------------------------------------------------------
// Name: EnumAxesCallback()
// Desc: Callback function for enumerating the axes on a joystick and counting
//       each force feedback enabled axis
//-----------------------------------------------------------------------------
BOOL CALLBACK CJoyStickFreedback::EnumAxesCallback( const DIDEVICEOBJECTINSTANCE* pdidoi,
                                VOID* pContext )
{
    DWORD* pdwNumForceFeedbackAxis = (DWORD*) pContext;

    if( (pdidoi->dwFlags & DIDOI_FFACTUATOR) != 0 )
        (*pdwNumForceFeedbackAxis)++;

    return DIENUM_CONTINUE;
}

//-----------------------------------------------------------------------------
// Name: EnumFFDevicesCallback()
// Desc: Called once for each enumerated force feedback device. If we find
//       one, create a device interface on it so we can play with it.
//-----------------------------------------------------------------------------
BOOL CALLBACK CJoyStickFreedback::EnumFFDevicesCallback( const DIDEVICEINSTANCE* pInst, 
                                     VOID* pContext )
{
    LPDIRECTINPUTDEVICE8 pDevice;
    HRESULT              hr;

//	if(strlen(g_chJoystickFeedDevice) == 0)
//	{
//		// 정보가 없다면 첫번째 장치를 초기로
//		strncpy(g_chJoystickFeedDevice, pInst->tszInstanceName, MAX_PATH);
//	}
	if(0 != stricmp(g_chJoystickFeedDevice, pInst->tszInstanceName))
	{		
		// 원하는 조이스틱이 아니다.
		return DIENUM_CONTINUE;
	}	


    // Obtain an interface to the enumerated force feedback device.
    hr = g_pDI->CreateDevice( pInst->guidInstance, &pDevice, NULL );

    // If it failed, then we can't use this device for some
    // bizarre reason.  (Maybe the user unplugged it while we
    // were in the middle of enumerating it.)  So continue enumerating
    if( FAILED(hr) ) 
        return DIENUM_CONTINUE;

    // We successfully created an IDirectInputDevice8.  So stop looking 
    // for another one.
    g_pDevice = pDevice;

    return DIENUM_STOP;
}




//-----------------------------------------------------------------------------
// Name: FreeDirectInput()
// Desc: Initialize the DirectInput variables.
//-----------------------------------------------------------------------------
VOID CJoyStickFreedback::FreeDirectInput()
{
    // Unacquire the device one last time just in case 
    // the app tried to exit while the device is still acquired.
    if( g_pDevice ) 
        g_pDevice->Unacquire();
    
    // Release any DirectInput objects.
    SAFE_RELEASE( m_pEffect );
    SAFE_RELEASE( g_pDevice );
    SAFE_RELEASE( g_pDI );

	m_bEffectStarted = FALSE;

	memset(g_chJoystickFeedDevice, 0x00,  MAX_PATH);
}



//-----------------------------------------------------------------------------
// Name: SetDeviceForcesXY()
// Desc: Apply the X and Y forces to the effect we prepared.
//-----------------------------------------------------------------------------
HRESULT CJoyStickFreedback::SetDeviceForcesXY(int nForceX, int nForceY)
{
	// Don't allow setting effect more often than
    // 100ms since every time an effect is set, the
    // device will jerk.
	DWORD dwCurrentTime = timeGetTime();
	if( dwCurrentTime - m_dwLastEffectSet < 100.0f )
    {
        // Don't allow setting effect more often than
        // 100ms since every time an effect is set, the
        // device will jerk.
        //
        // Note: This is not neccessary, and is specific to this sample
        return S_OK;
    }
	if(m_bRqEffectStarted)
	{
		m_bRqEffectStarted = FALSE;
		DeviceAcquire();
	}

	m_bEffectStarted = TRUE;

	nForceX = CoordToForce(nForceX);
	nForceY = CoordToForce(nForceY);
	m_dwLastEffectSet = dwCurrentTime; 
	
    // Modifying an effect is basically the same as creating a new one, except
    // you need only specify the parameters you are modifying
    LONG rglDirection[2] = { 0, 0 };

    DICONSTANTFORCE cf;

    if( m_dwNumForceFeedbackAxis == 1 )
    {
        // If only one force feedback axis, then apply only one direction and 
        // keep the direction at zero
        cf.lMagnitude = nForceX;
        rglDirection[0] = 0;
    }
    else
    {
        // If two force feedback axis, then apply magnitude from both directions 
        rglDirection[0] = nForceX;
        rglDirection[1] = nForceY;
        cf.lMagnitude = (DWORD)sqrt( (double)nForceX * (double)nForceX +
                                     (double)nForceY * (double)nForceY );
    }

    DIEFFECT eff;
    ZeroMemory( &eff, sizeof(eff) );
    eff.dwSize                = sizeof(DIEFFECT);
    eff.dwFlags               = DIEFF_CARTESIAN | DIEFF_OBJECTOFFSETS;
    eff.cAxes                 = m_dwNumForceFeedbackAxis;
    eff.rglDirection          = rglDirection;
    eff.lpEnvelope            = 0;
    eff.cbTypeSpecificParams  = sizeof(DICONSTANTFORCE);
    eff.lpvTypeSpecificParams = &cf;
    eff.dwStartDelay            = 0;

    // Now set the new parameters and start the effect immediately.
    return m_pEffect->SetParameters( &eff, DIEP_DIRECTION |
                                           DIEP_TYPESPECIFICPARAMS |
                                           DIEP_START );
}

void CJoyStickFreedback::StopDeviceForce()
{
	m_bEffectStarted = FALSE;

	if( m_pEffect ) 
	{
		m_pEffect->Stop(); // stop the effect		
	}
}

//-----------------------------------------------------------------------------
// Name: CoordToForce()
// Desc: Convert a coordinate 0 <= nCoord <= FEEDBACK_WINDOW_WIDTH 
//       to a force value in the range -DI_FFNOMINALMAX to +DI_FFNOMINALMAX.
//-----------------------------------------------------------------------------
INT CJoyStickFreedback::CoordToForce( INT nCoord )
{
	// 최대 최소 값이다.
    INT nForce = nCoord;

    // Keep force within bounds
    if( nForce < -DI_FFNOMINALMAX ) 
        nForce = -DI_FFNOMINALMAX;

    if( nForce > +DI_FFNOMINALMAX ) 
        nForce = +DI_FFNOMINALMAX;

    return nForce;
}

void CJoyStickFreedback::DeviceAcquire()
{
	// Make sure the device is acquired, if we are gaining focus.
	
	if(NULL == g_pDevice)
	{
		return;
	}
	g_pDevice->Acquire();

	if( m_pEffect ) 
	{
		m_pEffect->Start( 1, 0 ); // Start the effect		
	}
}

BOOL CJoyStickFreedback::IsEffectStarted()
{
	return m_bEffectStarted;
}

void CJoyStickFreedback::RqEffectStarted(BOOL bSet)
{
	m_bRqEffectStarted = bSet;
}
char* CJoyStickFreedback::GetJoystickFeedDevice()
{
	return g_chJoystickFeedDevice;	
}