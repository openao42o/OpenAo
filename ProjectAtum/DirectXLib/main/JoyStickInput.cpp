// JoyStickInput.cpp: implementation of the CJoyStickInput class.
//
//////////////////////////////////////////////////////////////////////
#define STRICT
#define DIRECTINPUT_VERSION 0x0800



#include "JoyStickInput.h"

#include "DXUtil.h"
#include <math.h>

#include "Contents.h"

#include "DefineGlobal.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#define				MAX_JOY_LIST			10	// 최대 10개까지 조이스틱 가능

LPDIRECTINPUT8				g_pJoyStickDI = NULL;         
LPDIRECTINPUTDEVICE8		g_pJoystickDevice = NULL;     

structJoyStickStats			g_struJoyStickStats;

structJoyStickKeyInfo		*g_struJoyStickKeyInfo = NULL;

int							g_nJoystickBtnLen = 0;

int							g_nJoystickCtrlLen = 0;
char						g_nJoystickCtrlList[MAX_JOY_LIST][MAX_PATH];
//char						g_chJoystickFeedDevice[MAX_PATH];
char						g_chJoystickCtrlDevice[MAX_PATH];



CJoyStickInput::CJoyStickInput()
{
	g_pJoyStickDI              = NULL;         
	g_pJoystickDevice        = NULL;     
	
	memset(&g_struJoyStickStats, 0x00, sizeof(structJoyStickStats))	;

	int nCnt =0;
	for(nCnt =0;nCnt<MAX_DIRECT_INPUT_JOSTICK;nCnt++)
	{
		m_nDIJ[nCnt] = 0;
	}
	g_struJoyStickKeyInfo = new structJoyStickKeyInfo[MAX_JOSTICK_KEY_OPTION];	
	for(nCnt = 0;nCnt < MAX_JOSTICK_KEY_OPTION;nCnt++)
	{
		memset(&g_struJoyStickKeyInfo[nCnt], 0x00, sizeof(structJoyStickKeyInfo));
	}
	g_nJoystickBtnLen = 0;	
	
	g_nJoystickCtrlLen = 0;
	for(nCnt = 0;nCnt < MAX_JOY_LIST;nCnt++)
	{
		memset(g_nJoystickCtrlList[nCnt], 0x00, MAX_PATH);
	}
	memset(g_chJoystickCtrlDevice, 0x00, MAX_PATH);
	


	InitSensity();
}

CJoyStickInput::~CJoyStickInput()
{	
	g_nJoystickCtrlLen = 0;
	memset(g_chJoystickCtrlDevice, 0x00, MAX_PATH);
	memset(&g_struJoyStickStats, 0x00, sizeof(structJoyStickStats))	;
	util::del_array(g_struJoyStickKeyInfo);
	FreeDirectInput();
}


//-----------------------------------------------------------------------------
// Name: InitDirectInput()
// Desc: Initialize the DirectInput variables.
//-----------------------------------------------------------------------------
HRESULT CJoyStickInput::InitDirectInput( HWND hDlg, char* i_pJoySelDeviceName )
{
    HRESULT hr;
		
	g_nJoystickCtrlLen = 0;
	strncpy(g_chJoystickCtrlDevice, i_pJoySelDeviceName, MAX_PATH);

	memset(&g_struJoyStickStats, 0x00, sizeof(structJoyStickStats))	;	

    // Register with the DirectInput subsystem and get a pointer
    // to a IDirectInput interface we can use.
    // Create a DInput object
    if( FAILED( hr = DirectInput8Create( GetModuleHandle(NULL), DIRECTINPUT_VERSION, 
                                         IID_IDirectInput8, (VOID**)&g_pJoyStickDI, NULL ) ) )
        return hr;

	if( FAILED( hr = g_pJoyStickDI->EnumDevices( DI8DEVCLASS_GAMECTRL, 
                                         EnumOnlyJoysticksListCallback,
                                         NULL, DIEDFL_ATTACHEDONLY ) ) )
	{
		// 현재 장치 리스트를 얻어온다.
	}

    // Look for a simple joystick we can use for this sample program.
    if( FAILED( hr = g_pJoyStickDI->EnumDevices( DI8DEVCLASS_GAMECTRL, 
                                         EnumJoysticksCallback,
                                         NULL, DIEDFL_ATTACHEDONLY ) ) )
        return hr;

    // Make sure we got a joystick
    if( NULL == g_pJoystickDevice )
    {
//        MessageBox( NULL, TEXT("Joystick not found. The sample will now exit."),  
//                    TEXT("DirectInput Sample"), 
//                    MB_ICONERROR | MB_OK );
//        EndDialog( hDlg, 0 );
        return -1;
    }

    // Set the data format to "simple joystick" - a predefined data format 
    //
    // A data format specifies which controls on a device we are interested in,
    // and how they should be reported. This tells DInput that we will be
    // passing a DIJOYSTATE2 structure to IDirectInputDevice::GetDeviceState().
    if( FAILED( hr = g_pJoystickDevice->SetDataFormat( &c_dfDIJoystick2 ) ) )
        return hr;

    // Set the cooperative level to let DInput know how this device should
    // interact with the system and with other DInput applications.
    if( FAILED( hr = g_pJoystickDevice->SetCooperativeLevel( hDlg, DISCL_EXCLUSIVE | 
                                                             DISCL_FOREGROUND ) ) )
        return hr;

    // Enumerate the joystick objects. The callback function enabled user
    // interface elements for objects that are found, and sets the min/max
    // values property for discovered axes.
    if( FAILED( hr = g_pJoystickDevice->EnumObjects( EnumObjectsCallback, 
                                                (VOID*)hDlg, DIDFT_ALL ) ) )
        return hr;

    return S_OK;
}

//-----------------------------------------------------------------------------
// Name: EnumJoysticksCallback()
// Desc: Called once for each enumerated joystick. If we find one, create a
//       device interface on it so we can play with it.
//-----------------------------------------------------------------------------
BOOL CALLBACK CJoyStickInput::EnumOnlyJoysticksListCallback( const DIDEVICEINSTANCE* pdidInstance,
                                     VOID* pContext )
{
	strncpy(g_nJoystickCtrlList[g_nJoystickCtrlLen], pdidInstance->tszInstanceName, MAX_PATH);
	g_nJoystickCtrlLen++;
	if(g_nJoystickCtrlLen >= MAX_JOY_LIST)
	{
		g_nJoystickCtrlLen =  MAX_JOY_LIST-1;
	}

	// 단지 리스트만 가져오자
	return DIENUM_CONTINUE;
}
//-----------------------------------------------------------------------------
// Name: EnumJoysticksCallback()
// Desc: Called once for each enumerated joystick. If we find one, create a
//       device interface on it so we can play with it.
//-----------------------------------------------------------------------------
BOOL CALLBACK CJoyStickInput::EnumJoysticksCallback( const DIDEVICEINSTANCE* pdidInstance,
                                     VOID* pContext )
{
    HRESULT hr;
	

//	if(strlen(g_chJoystickCtrlDevice) == 0)
//	{
//		// 정보가 없다면 첫번째 장치를 초기로
//		strncpy(g_chJoystickCtrlDevice, pdidInstance->tszInstanceName, MAX_PATH);
//	}
	if(0 != stricmp(g_chJoystickCtrlDevice, pdidInstance->tszInstanceName))
	{		
		// 원하는 조이스틱이 아니다.
		return DIENUM_CONTINUE;
	}	

    // Obtain an interface to the enumerated joystick.
    hr = g_pJoyStickDI->CreateDevice( pdidInstance->guidInstance, &g_pJoystickDevice, NULL );

    // If it failed, then we can't use this joystick. (Maybe the user unplugged
    // it while we were in the middle of enumerating it.)
    if( FAILED(hr) ) 
        return DIENUM_CONTINUE;

    // Stop enumeration. Note: we're just taking the first joystick we get. You
    // could store all the enumerated joysticks and let the user pick.
    return DIENUM_STOP;
}

//-----------------------------------------------------------------------------
// Name: EnumObjectsCallback()
// Desc: Callback function for enumerating objects (axes, buttons, POVs) on a 
//       joystick. This function enables user interface elements for objects
//       that are found to exist, and scales axes min/max values.
//-----------------------------------------------------------------------------
BOOL CALLBACK CJoyStickInput::EnumObjectsCallback( const DIDEVICEOBJECTINSTANCE* pdidoi,
                                   VOID* pContext )
{
    HWND hDlg = (HWND)pContext;

//    static int nSliderCount = 0;  // Number of returned slider controls
//    static int nPOVCount = 0;     // Number of returned POV controls

    // For axes that are returned, set the DIPROP_RANGE property for the
    // enumerated axis in order to scale min/max values.
    if( pdidoi->dwType & DIDFT_AXIS )
    {
        DIPROPRANGE diprg; 
        diprg.diph.dwSize       = sizeof(DIPROPRANGE); 
        diprg.diph.dwHeaderSize = sizeof(DIPROPHEADER); 
        diprg.diph.dwHow        = DIPH_BYID; 
        diprg.diph.dwObj        = pdidoi->dwType; // Specify the enumerated axis
        diprg.lMin              = -1000; 
        diprg.lMax              = +1000; 
    
        // Set the range for the axis
        if( FAILED( g_pJoystickDevice->SetProperty( DIPROP_RANGE, &diprg.diph ) ) ) 
            return DIENUM_STOP;
         
    }


    // Set the UI to reflect what objects the joystick supports
    if (pdidoi->guidType == GUID_XAxis)
    {
        g_struJoyStickStats.bXAxis = TRUE;
    }
    if (pdidoi->guidType == GUID_YAxis)
    {
        g_struJoyStickStats.bYAxis = TRUE;
    }
    if (pdidoi->guidType == GUID_ZAxis)
    {
        g_struJoyStickStats.bZAxis = TRUE;
    }
    if (pdidoi->guidType == GUID_RxAxis)
    {
        g_struJoyStickStats.bRXAxis = TRUE;
    }
    if (pdidoi->guidType == GUID_RyAxis)
    {
        g_struJoyStickStats.bRYAxis = TRUE;
    }
    if (pdidoi->guidType == GUID_RzAxis)
    {
        g_struJoyStickStats.bRZAxis = TRUE;
    }
    if (pdidoi->guidType == GUID_Slider)
    {
        g_struJoyStickStats.nSlideCnt++;
    }
    if (pdidoi->guidType == GUID_POV)
    {
		g_struJoyStickStats.nPovCnt++;        
    }
	if (pdidoi->guidType == GUID_Button)
    {
		// 버튼 갯수
		g_struJoyStickStats.nBtnCnt++;        
    }

	// 키정보값갱신
	UpdateJoysticKeyInfomation();

    return DIENUM_CONTINUE;
}


//-----------------------------------------------------------------------------
// Name: UpdateInputState()
// Desc: Get the input device's state and display it.
//-----------------------------------------------------------------------------
HRESULT CJoyStickInput::UpdateInputState()
{
    HRESULT     hr;
    //TCHAR       strText[512]; // Device state text
    DIJOYSTATE2 js;           // DInput joystick state 
    //TCHAR*      str;

    if( NULL == g_pJoystickDevice ) 
        return S_OK;

    // Poll the device to read the current state
    hr = g_pJoystickDevice->Poll(); 
    if( FAILED(hr) )  
    {
        // DInput is telling us that the input stream has been
        // interrupted. We aren't tracking any state between polls, so
        // we don't have any special reset that needs to be done. We
        // just re-acquire and try again.
        hr = g_pJoystickDevice->Acquire();
        while( hr == DIERR_INPUTLOST ) 
            hr = g_pJoystickDevice->Acquire();

        // hr may be DIERR_OTHERAPPHASPRIO or other errors.  This
        // may occur when the app is minimized or in the process of 
        // switching, so just try again later 
        return S_OK; 
    }

    // Get the input's device state
    if( FAILED( hr = g_pJoystickDevice->GetDeviceState( sizeof(DIJOYSTATE2), &js ) ) )
        return hr; // The device should have been acquired during the Poll()

    // Display joystick state to dialog	
	int	nDIJ[MAX_DIRECT_INPUT_JOSTICK];

	int nCnt =0;
	for(nCnt =0;nCnt<MAX_DIRECT_INPUT_JOSTICK;nCnt++)
	{		
		m_bChangeDIJ[nCnt] = FALSE;
		nDIJ[nCnt] = 0;
	}

	// X,Y,Z
	{
		nDIJ[DIRECT_INPUT_JOSTICK_X] = js.lX;		
		nDIJ[DIRECT_INPUT_JOSTICK_Y] = js.lY;
		nDIJ[DIRECT_INPUT_JOSTICK_Z] = js.lZ;
		
	}

	// Rx, Ry, Rz
	{
		nDIJ[DIRECT_INPUT_JOSTICK_RX] = js.lRx;		
		nDIJ[DIRECT_INPUT_JOSTICK_RY] = js.lRy;
		nDIJ[DIRECT_INPUT_JOSTICK_RZ] = js.lRz;
	}
	
	// Slider
	{
		nDIJ[DIRECT_INPUT_JOSTICK_SLIDER0] = js.rglSlider[0];		
		nDIJ[DIRECT_INPUT_JOSTICK_SLIDER1] = js.rglSlider[1];
	}

	// Points of view
	{
		nDIJ[DIRECT_INPUT_JOSTICK_POW0] = js.rgdwPOV[0];		
		nDIJ[DIRECT_INPUT_JOSTICK_POW1] = js.rgdwPOV[1];		
		nDIJ[DIRECT_INPUT_JOSTICK_POW2] = js.rgdwPOV[2];		
		nDIJ[DIRECT_INPUT_JOSTICK_POW3] = js.rgdwPOV[3];		
	}

	// Btn
	{
		int nCnt = 0;
		int nPos = DIRECT_INPUT_JOSTICK_BTN0;
		for( nCnt = 0; nCnt < 128; nCnt++ )
		{
			if(nPos >= MAX_DIRECT_INPUT_JOSTICK)
			{
				break;
			}
			if ( js.rgbButtons[nCnt] & 0x80 )
			{
				nDIJ[nPos] = true;
			}
			nPos++;
		}
	}	

	{
		int nCnt = 0;
		for( nCnt = 0; nCnt < MAX_DIRECT_INPUT_JOSTICK; nCnt++ )
		{
			if(nDIJ[nCnt] != m_nDIJ[nCnt])
			{
				m_bChangeDIJ[nCnt] = TRUE;
			}
			m_nDIJ[nCnt] = nDIJ[nCnt];			
		}
	}


    return S_OK;
}

//-----------------------------------------------------------------------------
// Name: FreeDirectInput()
// Desc: Initialize the DirectInput variables.
//-----------------------------------------------------------------------------
VOID CJoyStickInput::FreeDirectInput()
{
    // Unacquire the device one last time just in case 
    // the app tried to exit while the device is still acquired.
    if( g_pJoystickDevice) 
        g_pJoystickDevice->Unacquire();
    
    // Release any DirectInput objects.
    SAFE_RELEASE( g_pJoystickDevice);
    SAFE_RELEASE( g_pJoyStickDI );

	memset(g_chJoystickCtrlDevice, 0x00, MAX_PATH);
}

int CJoyStickInput::GetJoySitckInfo(int nIdx)
{
	if(nIdx < 0 || nIdx >= MAX_DIRECT_INPUT_JOSTICK)
	{
		return 0;
	}
	return m_nDIJ[nIdx];
}

BOOL CJoyStickInput::IsChangeJoySitckInfo(int nIdx)
{
	if(nIdx < 0 || nIdx >= MAX_DIRECT_INPUT_JOSTICK)
	{
		return FALSE;
	}
	return m_bChangeDIJ[nIdx];

}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2008-11-13 by bhsohn 조이스틱 작업
/// \date		2008-11-13 ~ 2008-11-13
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CJoyStickInput::IsJoystickWheel(int nKeyIndex)
{
	switch(nKeyIndex)
	{
	case DIRECT_INPUT_JOSTICK_X:	// x-axis position
	case DIRECT_INPUT_JOSTICK_Y:	// y-axis position
	case DIRECT_INPUT_JOSTICK_Z:	// z-axis position 
	case DIRECT_INPUT_JOSTICK_RX:	// x-axis rotation
	case DIRECT_INPUT_JOSTICK_RY:	// y-axis rotation
	case DIRECT_INPUT_JOSTICK_RZ:	// z-axis rotation
		
		return TRUE;

	default:

		return FALSE;
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2008-11-13 by bhsohn 조이스틱 작업
/// \date		2008-11-13 ~ 2008-11-13
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CJoyStickInput::IsJoystickPOV(int nKeyIndex)
{
	switch(nKeyIndex)
	{
	case DIRECT_INPUT_JOSTICK_POW0:		/* POV directions               */
	case DIRECT_INPUT_JOSTICK_POW1:
	case DIRECT_INPUT_JOSTICK_POW2:
	case DIRECT_INPUT_JOSTICK_POW3:
		{
			return TRUE;
		}
	}
	return FALSE;
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2008-11-13 by bhsohn 조이스틱 작업
/// \date		2008-11-13 ~ 2008-11-13
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CJoyStickInput::InitSensity()
{
	int nY = 0;
	int nCnt = 0;

	for(nY = 0;nY < MAX_JOYSTICK_SENSITY;nY++)
	{
		for(nCnt = 0;nCnt < MAX_JOYSTICK_VALUE;nCnt++)
		{
			m_SensyTable[nY][nCnt].fRepresentAtiveValue = -1;
			m_SensyTable[nY][nCnt].fAtiveValue = -1;
		}
		UpdateSenstity(nY);
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2008-11-13 by bhsohn 조이스틱 작업
/// \date		2008-11-13 ~ 2008-11-13
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
float CJoyStickInput::GetSensity(int i_nSensity, float i_fValue)
{
	float fRtn = i_fValue;
	if(i_nSensity >= MAX_JOYSTICK_SENSITY 
		|| i_nSensity < 0
		|| (MAX_JOYSTICK_DEFAULTSENSITY == i_nSensity))
	{
		return fRtn;
	}
	
	BOOL bMinnus = FALSE;
	if(i_fValue < 0.0f)
	{
		bMinnus = TRUE;
		i_fValue = -1.0f*(i_fValue);
	}
	int nIdx = (int)(i_fValue*100.0f);
	if(nIdx < 0)
	{
		nIdx = 0;
	}
	else if(nIdx >= MAX_JOYSTICK_VALUE)
	{
		nIdx = MAX_JOYSTICK_VALUE-1;
	}
	// 센스값
	fRtn = m_SensyTable[i_nSensity][nIdx].fAtiveValue;
	if(bMinnus)
	{
		fRtn *= -1.0f;
	}
	
	return fRtn;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2008-11-13 by bhsohn 조이스틱 작업
/// \date		2008-11-13 ~ 2008-11-13
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CJoyStickInput::UpdateSenstity(int i_nSensity)
{
	int nCnt = 0;

	float fSensity[MAX_JOYSTICK_SENSITY] =
	{
		0.1f, 
		0.5f,
		1.0f, 
		5.0f,
		9.0f
	};
	

	{
		for(nCnt = 0;nCnt < MAX_JOYSTICK_VALUE;nCnt++)
		{
			m_SensyTable[i_nSensity][nCnt].fRepresentAtiveValue = ((float)nCnt)/100.0f;
			m_SensyTable[i_nSensity][nCnt].fAtiveValue 
				= (float)pow(m_SensyTable[i_nSensity][nCnt].fRepresentAtiveValue, fSensity[i_nSensity]);
		}
	}

}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		변경된 정보 초기화
/// \author		// 2008-11-13 by bhsohn 조이스틱 작업
/// \date		2008-11-13 ~ 2008-11-13
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CJoyStickInput::InitChangeDIJ()
{
	int nCnt =0;
	for(nCnt =0;nCnt<MAX_DIRECT_INPUT_JOSTICK;nCnt++)
	{		
		m_bChangeDIJ[nCnt] = FALSE;		
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		변경된 정보 초기화
/// \author		// 2008-11-13 by bhsohn 조이스틱 작업
/// \date		2008-11-13 ~ 2008-11-13
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int CJoyStickInput::GetJoyStickBtnCnt()
{
	return g_struJoyStickStats.nBtnCnt;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2008-11-13 by bhsohn 조이스틱 작업
/// \date		2008-11-13 ~ 2008-11-13
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CJoyStickInput::UpdateJoysticKeyInfomation()
{
	char chJoysticInfo[MAX_DEFAULT_JOY_SETUP][MAX_PATH] =
	{
		STRMSG_C_081125_0200,
		STRMSG_C_081125_0201,
		STRMSG_C_081125_0202,
		STRMSG_C_081125_0203,
		STRMSG_C_081125_0204,
		STRMSG_C_081125_0205,
		STRMSG_C_081125_0206,
		STRMSG_C_081125_0207,
		STRMSG_C_081125_0208,
		STRMSG_C_081125_0209,
		STRMSG_C_081125_0210,
		STRMSG_C_081125_0211,
		STRMSG_C_081125_0212,
		STRMSG_C_081125_0213,
		STRMSG_C_081125_0214,
		STRMSG_C_081125_0215
	};
	//////////// 조이스틱 각키 값 선언 //////////// 
	int		nJoysticIdx[MAX_DEFAULT_JOY_SETUP] =
	{
		DIRECT_INPUT_JOSTICK_X,		//"X Axis(-)"
		DIRECT_INPUT_JOSTICK_X,		//"X Axis(+)"
		DIRECT_INPUT_JOSTICK_Y,		//"Y Axis(-)"
		DIRECT_INPUT_JOSTICK_Y,		//"Y Axis(+)"
		DIRECT_INPUT_JOSTICK_Z,		//"Z Axis(-)"
		DIRECT_INPUT_JOSTICK_Z,		//"Z Axis(+)"
		DIRECT_INPUT_JOSTICK_RX,		//"RX Axis(-)"
		DIRECT_INPUT_JOSTICK_RX,		//"RX Axis(+)"
		DIRECT_INPUT_JOSTICK_RY,		//"RY Axis(-)"
		DIRECT_INPUT_JOSTICK_RY,		//"RY Axis(+)"
		DIRECT_INPUT_JOSTICK_RZ,		//"RZ Axis(-)"
		DIRECT_INPUT_JOSTICK_RZ,		//"RZ Axis(+)"
		DIRECT_INPUT_JOSTICK_POW0,		//"십자버튼-좌"
		DIRECT_INPUT_JOSTICK_POW0,		//"십자버튼-우"
		DIRECT_INPUT_JOSTICK_POW0,		//"십자버튼-상"
		DIRECT_INPUT_JOSTICK_POW0		//"십자버튼-하"
	};

	int		nJoysticValue[MAX_DEFAULT_JOY_SETUP] =
	{
		-1000,		//"X Axis(-)"
		1000,		//"X Axis(+)"
		-1000,		//"Y Axis(-)"
		1000,		//"Y Axis(+)"
		-1000,		//"Z Axis(-)"
		1000,		//"Z Axis(+)"
		-1000,		//"RX Axis(-)"
		1000,		//"RX Axis(+)"
		-1000,		//"RY Axis(-)"
		1000,		//"RY Axis(+)"
		-1000,		//"RZ Axis(-)"
		1000,		//"RZ Axis(+)"
		27000,		//"십자버튼-좌"
		9000,		//"십자버튼-우"
		0,			//"십자버튼-상"
		18000		//"십자버튼-하"
	};
	int nPos = 0;
	int nCnt =0;
	for(nCnt =0; nCnt < MAX_DEFAULT_JOY_SETUP;nCnt++)
	{
		g_struJoyStickKeyInfo[nCnt].nKeyIdx = nJoysticIdx[nCnt];
		g_struJoyStickKeyInfo[nCnt].nKeyValue = nJoysticValue[nCnt];
		strncpy(g_struJoyStickKeyInfo[nCnt].chJoysticInfo, chJoysticInfo[nCnt], MAX_PATH);		
		nPos++;
	}

	for(nCnt =0; nCnt < g_struJoyStickStats.nBtnCnt;nCnt++)
	{
		if(nPos >= MAX_JOSTICK_KEY_OPTION)
		{
			break;
		}
		g_struJoyStickKeyInfo[nPos].nKeyIdx = DIRECT_INPUT_JOSTICK_BTN0+nCnt;
		g_struJoyStickKeyInfo[nPos].nKeyValue = 1;		
		wsprintf(g_struJoyStickKeyInfo[nPos].chJoysticInfo, STRMSG_C_081125_0216, nCnt+1);		//"버튼%d"
		nPos++;
	}
	for(nCnt = nPos; nCnt < MAX_JOSTICK_KEY_OPTION ;nCnt++)
	{
		// 나머지 값들
		g_struJoyStickKeyInfo[nCnt].nKeyIdx = -1;
		g_struJoyStickKeyInfo[nCnt].nKeyValue = -1;
		wsprintf(g_struJoyStickKeyInfo[nCnt].chJoysticInfo, "");		
	}

	// 조이스틱 셋팅수
	g_nJoystickBtnLen = nPos;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		각 키에 대한 매칭 밸류값
/// \author		// 2008-11-13 by bhsohn 조이스틱 작업
/// \date		2008-11-13 ~ 2008-11-13
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
structJoyStickKeyInfo *CJoyStickInput::GetKeyInfomation(int nKeyInfo)
{
	if(nKeyInfo < 0 || nKeyInfo>= MAX_JOSTICK_KEY_OPTION)
	{
		return NULL;
	}
	else if(-1 == g_struJoyStickKeyInfo[nKeyInfo].nKeyIdx )
	{
		// 값이 없다.
		return NULL;
	}
	return &g_struJoyStickKeyInfo[nKeyInfo];
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2008-11-13 by bhsohn 조이스틱 작업
/// \date		2008-11-13 ~ 2008-11-13
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int CJoyStickInput::GetKeyJoystickLen()
{
	return g_nJoystickBtnLen;

}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2008-11-13 by bhsohn 조이스틱 작업
/// \date		2008-11-13 ~ 2008-11-13
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CJoyStickInput::GetKeyKeyValue_To_JoysticInfo(int i_nKeyIndex, int* o_pJoyKeyIndex, int* o_pKeyValue)
{
	if( i_nKeyIndex < 0 || i_nKeyIndex>= MAX_JOSTICK_KEY_OPTION)
	{
		(*o_pJoyKeyIndex) = -1;
		(*o_pKeyValue) = -1;
		return;
	}
	(*o_pJoyKeyIndex) = g_struJoyStickKeyInfo[i_nKeyIndex].nKeyIdx;
	(*o_pKeyValue) = g_struJoyStickKeyInfo[i_nKeyIndex].nKeyValue;

}


char* CJoyStickInput::GetSelJoystickCtrlDevice()
{
	return g_chJoystickCtrlDevice;	
}

char* CJoyStickInput::GetJoystickCtrlDeviceList(int i_nIdx)
{
	if(i_nIdx >= MAX_JOY_LIST || i_nIdx < 0)
	{
		return NULL;
	}
	return g_nJoystickCtrlList[i_nIdx];	
}

int CJoyStickInput::GetJoystickCtrlDeviceListLen()
{
	return g_nJoystickCtrlLen;
}
