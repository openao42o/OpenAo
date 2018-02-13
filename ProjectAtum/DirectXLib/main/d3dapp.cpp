#define STRICT

#include "stdafx.h"
#include <windows.h>
#include <windowsx.h>
#include <basetsd.h>
#include <stdio.h>
#include <tchar.h>
#include <d3d9.h>
#include "D3DApp.h"
#include "D3DUtil.h"
#include "DXUtil.h"
#include "D3DRes.h"
#include "DbgOut_C.h"
#include "FunctionLog.h"
#include "AtumApplication.h"

#include "CharacterInfo.h"
#include "MemPoolClient.h"
#include "CustomOptimizer.h"

int (WINAPIV * __vsnprintf)(char *, size_t, const char*, va_list) = _vsnprintf;
int (WINAPIV * __vsnwprintf)(wchar_t *, size_t, const wchar_t*, va_list) = _vsnwprintf;

#include "dxerr9.h"

#ifdef _ADVANCED_CONFIG
// You'll need this include file and library linked.
#include <dxerr9.h>
#pragma comment(lib, "dxerr9.lib")
#endif

#ifdef _DEBUG
void DbgOutDeviceCaps(D3DCAPS9 caps);
#endif
//-----------------------------------------------------------------------------
// Global access to the app (needed for the global WndProc())
//-----------------------------------------------------------------------------
CD3DApplication* g_pApp = NULL;

// 2007-04-10 by bhsohn 포커스 잃어 버렸을때의 버그 처리
constexpr float	LOST_DEVICE_SLEEP_TIME = 100.0f;
constexpr float	LOST_DEVICE_ELASPTIME = LOST_DEVICE_SLEEP_TIME / 1000.0f;

#define MEMORYPOOL_DEFAULT_COUNT		1024

#define WND_MOUSELOCK_PIXEL				5		// 2012-12-05 by mspark, 마우스 가두기 옵션 설정 시 마우스가 좌, 우, 하단 범위를 벗어나던 버그 수정 


//-----------------------------------------------------------------------------
// Name: CD3DApplication()
// Desc: Constructor
//-----------------------------------------------------------------------------
CD3DApplication::CD3DApplication()
{
	m_hInstance			= NULL;
	m_dwCurrentMode		= -1;
	m_hCursor			= NULL;
	m_dwCursor			= 32512;
    g_pApp				= this;
	m_ptCursor.x		= 0;
	m_ptCursor.y		= 0;
	m_bShowCursor		= FALSE;
	m_dwIcon			= IDI_MAIN_ICON;

    m_dwNumAdapters     = 0;
    m_dwAdapter         = 0L;
    m_pD3D              = NULL;
    m_pd3dDevice        = NULL;
	// 2010-09-29 by jskim, SpriteDevice Lost 되었을 때 구조 변경
	m_pd3dxSprite		= NULL;
	// end 2010-09-29 by jskim, SpriteDevice Lost 되었을 때 구조 변경
    m_hWnd              = NULL;
    m_hWndFocus         = NULL;
    m_hMenu             = NULL;
    m_bActive           = FALSE;
    m_bReady            = FALSE;
    m_bHasFocus			= FALSE;
    m_dwCreateFlags     = 0L;

    m_bFrameMoving      = TRUE;
    m_bSingleStep       = FALSE;
    m_fFPS              = 0.0f;
    m_strDeviceStats[0] = _T('\0');
    m_strFrameStats[0]  = _T('\0');

    m_strWindowTitle    = _T("Nemerian");
    m_dwCreationWidth   = 400;
    m_dwCreationHeight  = 300;
    m_bUseDepthBuffer   = FALSE;
    m_dwMinDepthBits    = 16;
    m_dwMinStencilBits  = 0;
    m_bShowCursorWhenFullscreen = FALSE;

	m_nWidth = 800;
	m_nHeight = 600;

	m_hHangulDC = NULL;
	
	m_bDeviceLost = FALSE;

    // When m_bClipCursorWhenFullscreen is TRUE, the cursor is limited to
    // the device window when the app goes fullscreen.  This prevents users
    // from accidentally clicking outside the app window on a multimon system.
    // This flag is turned off by default for debug builds, since it makes 
    // multimon debugging difficult.
#if defined(_DEBUG) || defined(DEBUG)
    m_bClipCursorWhenFullscreen = FALSE;
#else
    m_bClipCursorWhenFullscreen = TRUE;
	m_bWindowed = FALSE;
#endif

	// 2008-11-06 by bhsohn 마우스 가두기 모드 보완
	m_nSetCursel = FALSE;
	m_rcClipRect.left = m_rcClipRect.top = m_rcClipRect.right = m_rcClipRect.bottom = 0;

}




//-----------------------------------------------------------------------------
// Name: WndProc()
// Desc: Static msg handler which passes messages to the application class.
//-----------------------------------------------------------------------------
LRESULT CALLBACK WndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	FLOG( "LRESULT CALLBACK WndProc()" );
    return g_pApp->MsgProc( hWnd, uMsg, wParam, lParam );
}

// 2005-01-13 by jschoi
// 웹과 연동을 위한 윈도우 프로시져
LRESULT CALLBACK WebWndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch(uMsg)
	{
//	case WM_KEYDOWN:
//		if(wParam == VK_ESCAPE && g_pApp)
//		{
//			g_pApp->DestroyWebWindow();
//			return 0;
//		}
//		break;

//	case WM_NCLBUTTONDOWN:
//		if(wParam == 20)	// 종료 버튼 클릭 시
//		{	
//			return 0;
//		}
//		break;

	case WM_CREATE:
//		if(g_pD3dApp)
//		{
//			g_pD3dApp->SetWeb(TRUE);
//			g_pApp->HideWebWindow();
//		}
		break;
	case WM_CLOSE:
//		if(g_pApp && g_pD3dApp)
//		{
//			g_pD3dApp->SetWeb(FALSE);
//			g_pApp->HideWebWindow();
			return 0;
// 		}
//		break;
	}
    return DefWindowProc(hWnd,uMsg,wParam,lParam);
}



//-----------------------------------------------------------------------------
// Name: Create()
// Desc:
//-----------------------------------------------------------------------------
HRESULT CD3DApplication::Create( HINSTANCE hInstance )
{
	FLOG( "CD3DApplication::Create( HINSTANCE hInstance )" );
    HRESULT hr;
	DWORD err = 0;
	BOOL borderless = FALSE;
//	__try{
		m_hInstance = hInstance;

		// Create the Direct3D object
		m_pD3D = Direct3DCreate9( D3D_SDK_VERSION );
		if( m_pD3D == NULL )
		{
			MessageBox( NULL, STRMSG_C_DIRECTX_0001, STRMSG_WINDOW_TEXT, MB_OK );//"다이렉트X 버전이 낮습니다. 다이렉트X 9.0 이상을 설치하십시요"
			return DisplayErrorMsg( D3DAPPERR_NODIRECT3D, MSGERR_APPMUSTEXIT );
		}

		if (m_nHeight == 0 && m_nWidth == 0)
		{
			D3DDISPLAYMODE d3ddm;
			m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm); //Get native screen resolution

			m_nWidth = d3ddm.Width;
			m_nHeight = d3ddm.Height;
			borderless = TRUE;
		}

		// Build a list of Direct3D adapters, modes and devices. The
		// ConfirmDevice() callback is used to confirm that only devices that
		// meet the app's requirements are considered.
		if( FAILED( hr = BuildDeviceList() ) )
		{
			SAFE_RELEASE( m_pD3D );
			DbgOut("BuildDeviceList failed!");
			return DisplayErrorMsg( hr, MSGERR_APPMUSTEXIT );
		}

		// Unless a substitute hWnd has been specified, create a window to
		// render into
		if( m_hWnd == NULL)
		{
			// Register the windows class
			WNDCLASS wndClass = { CS_DBLCLKS, WndProc, 0, 0, hInstance,
								  LoadIcon( hInstance, MAKEINTRESOURCE(m_dwIcon) ),
								  NULL,
								  //LoadCursor( NULL, IDC_ARROW ),
								  (HBRUSH)GetStockObject(WHITE_BRUSH),
								  NULL, WINDOWTEXT_NAME_CLIENT };
			RegisterClass( &wndClass );
			m_hCursor = LoadCursor(m_hInstance, MAKEINTRESOURCE(m_dwCursor) );// 사용자 마우스 커서
//			m_hCursor = LoadCursor( NULL, IDC_ARROW );
			// Set the window's initial style
			if (borderless)
			{
				//2015-03-08 by killburne BORDERLESS window
				m_dwWindowStyle = WS_EX_TOPMOST | WS_POPUP; //Borderless

				//Set client res to Desktop resolution
				m_dwCreationWidth = m_nWidth;
				m_dwCreationHeight = m_nHeight;

			}
			else if( m_bWindowed )
			{
			
//				m_dwWindowStyle = WS_OVERLAPPED|WS_CAPTION|WS_MINIMIZEBOX|WS_VISIBLE;
//				m_dwWindowStyle = WS_OVERLAPPED|WS_CAPTION|WS_SYSMENU|WS_THICKFRAME|
//								  WS_MINIMIZEBOX|WS_VISIBLE;
// 2007-12-21 by dgwoo 창모드 지원
//				m_dwWindowStyle = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | 
//                          WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_VISIBLE;
				// 2008-12-15 by bhsohn 비스타에서 IME시스템 동작 안되는 문제 해결
// 				m_dwWindowStyle =  WS_CAPTION | WS_SYSMENU |
//                           WS_MINIMIZEBOX | WS_VISIBLE;
				//WS_OVERLAPPEDWINDOW

				m_dwWindowStyle =  WS_OVERLAPPED| WS_CAPTION | WS_SYSMENU |WS_MINIMIZEBOX;
				// end 2008-12-15 by bhsohn 비스타에서 IME시스템 동작 안되는 문제 해결

				// 2007-07-04 by dgwoo 윈도우(디버그) 모드에서도 윈도우 사이즈의 인자를 받아 적용시킨다.
				m_dwCreationWidth = m_nWidth;
				m_dwCreationHeight = m_nHeight;
			}
			else
				m_dwWindowStyle = WS_POPUP | WS_SYSMENU | WS_VISIBLE;
			// Set the window's initial width
			RECT rc;
			SetRect( &rc, 0, 0, m_dwCreationWidth, m_dwCreationHeight );
			AdjustWindowRect( &rc, m_dwWindowStyle, TRUE );
 

			// Create the render window
			m_hWnd = CreateWindow( _T(WINDOWTEXT_NAME_CLIENT), m_strWindowTitle, m_dwWindowStyle,
								   CW_USEDEFAULT, CW_USEDEFAULT,
								   m_dwCreationWidth, m_dwCreationHeight, 0L,
								   NULL,
								   hInstance, 0L );
		}

		// The focus window can be a specified to be a different window than the
		// device window.  If not, use the device window as the focus window.
		if( m_hWndFocus == NULL )
			m_hWndFocus = m_hWnd;

		// Save window properties
		m_dwWindowStyle = GetWindowLong( m_hWnd, GWL_STYLE );
		GetWindowRect( m_hWnd, &m_rcWindowBounds );
		GetClientRect( m_hWnd, &m_rcWindowClient );

		// Initialize the application timer
		DXUtil_Timer( TIMER_START );

		m_hHangulDC = CreateCompatibleDC(NULL);
		// Initialize the app's custom scene stuff
		if( FAILED( hr = OneTimeSceneInit() ) )
		{
			SAFE_RELEASE( m_pD3D );
			DbgOut("OneTimeSceneInit failed!");
			return DisplayErrorMsg( hr, MSGERR_APPMUSTEXIT );
		}

		// Initialize the 3D environment for the app
		if( FAILED( hr = Initialize3DEnvironment() ) )
		{
			SAFE_RELEASE( m_pD3D );
			DbgOut("Initialize3DEnvironment failed!");
			return DisplayErrorMsg( hr, MSGERR_APPMUSTEXIT );
		}

		// The app is ready to go
		m_bReady = TRUE;
//	}
//	__except(err)
//	{
//		;
//	}

    return S_OK;
}


//-----------------------------------------------------------------------------
// Name: SortModesCallback()
// Desc: Callback function for sorting display modes (used by BuildDeviceList).
//-----------------------------------------------------------------------------
static int SortModesCallback( const VOID* arg1, const VOID* arg2 )
{
	FLOG( "static int SortModesCallback()" );
    D3DDISPLAYMODE* p1 = (D3DDISPLAYMODE*)arg1;
    D3DDISPLAYMODE* p2 = (D3DDISPLAYMODE*)arg2;

    if( p1->Format > p2->Format )   return -1;
    if( p1->Format < p2->Format )   return +1;
    if( p1->Width  < p2->Width )    return -1;
    if( p1->Width  > p2->Width )    return +1;
    if( p1->Height < p2->Height )   return -1;
    if( p1->Height > p2->Height )   return +1;

    return 0;
}


#include <fstream>

//-----------------------------------------------------------------------------
// Name: BuildDeviceList()
// Desc:
//-----------------------------------------------------------------------------
HRESULT CD3DApplication::BuildDeviceList()
{
	// define strongly typed anonymous constant data
	constexpr struct
	{
		D3DDEVTYPE type;
		TCHAR* desc;
	}
	devinf[] =
	{
		{ D3DDEVTYPE_HAL , _T("HAL") },
		{ D3DDEVTYPE_REF , _T("REF") },
	};


    auto bHALExists = false;
	auto bHALIsWindowedCompatible = false;
	auto bHALIsDesktopCompatible = false;
	auto bHALIsSampleCompatible = false;

	static ofstream dbgfile { "device3d.log" };

	dbgfile << "m_pD3D->GetAdapterCount()" << " => " << m_pD3D->GetAdapterCount() << endl;

    // Loop through all the adapters on the system (usually, there's just one
    // unless more than one graphics card is present).
    for (UINT iAdapter = 0; iAdapter < m_pD3D->GetAdapterCount(); ++iAdapter)
    {
        // Fill in adapter info
        auto pAdapter  = m_Adapters + m_dwNumAdapters;

		m_pD3D->GetAdapterIdentifier(iAdapter, 0, &pAdapter->d3dAdapterIdentifier);
        m_pD3D->GetAdapterDisplayMode(iAdapter, &pAdapter->d3ddmDesktop);

        pAdapter->dwNumDevices    = 0;
        pAdapter->dwCurrentDevice = 0;

        // Enumerate all display modes on this adapter
		vector<D3DDISPLAYMODE> modes;

        D3DFORMAT formats[20];
        DWORD dwNumFormats = 0;
        //DWORD dwNumModes  = 0;

        // Add the adapter's current desktop format to the list of formats
        formats[dwNumFormats++] = pAdapter->d3ddmDesktop.Format;

		// Warn the user if the requested format is not supported
        auto modes_c = m_pD3D->GetAdapterModeCount(iAdapter, D3DFMT_X8R8G8B8);

		dbgfile << '[' << iAdapter << ']' <<
			"m_pD3D->GetAdapterModeCount(iAdapter, D3DFMT_X8R8G8B8)" << " => " << modes_c << endl;

		if (modes_c == 0) MessageBox(NULL, "This adapter does not support D3DFMT_X8R8G8B8.\n" STRMSG_REPFB,
			"Warning", MB_OK & MB_ICONWARNING);

		for (auto mode = 0; mode < modes_c; ++mode)
        {
            // Get the display mode attributes
            D3DDISPLAYMODE DisplayMode;

			m_pD3D->EnumAdapterModes(iAdapter, D3DFMT_X8R8G8B8, mode, &DisplayMode);

            // Filter out low-resolution modes
            if (DisplayMode.Width  < 800 || DisplayMode.Height < 600) continue;

			// Check if the mode already exists (to filter out refresh rates)
			auto exists = false;

            for (auto& mode : modes)
            
				if (mode.Width == DisplayMode.Width &&
					mode.Height == DisplayMode.Height &&
					mode.Format == DisplayMode.Format)
				{
					exists = true;
					break;
				}

            // If we found a new mode, add it to the list of modes
            if (!exists)
            {
				DisplayMode.RefreshRate = 0;

				modes.push_back(DisplayMode);

                // Check if the mode's format already exists
				DWORD f = 0;

                while (f < dwNumFormats)
					if (DisplayMode.Format == formats[f]) break;
					else ++f;

                // If the format is new, add it to the list
                if (f== dwNumFormats) formats[dwNumFormats++] = DisplayMode.Format;
            }
        }

        // Add devices to adapter
        for (auto& inf : devinf)
        {
            // Fill in device info
            auto pDevice = pAdapter->devices + pAdapter->dwNumDevices;
            pDevice->DeviceType     = inf.type;
            m_pD3D->GetDeviceCaps(iAdapter, inf.type, &pDevice->d3dCaps);
            pDevice->strDesc        = inf.desc;
            pDevice->dwNumModes     = 0;
            pDevice->dwCurrentMode  = 0;
            pDevice->bCanDoWindowed = false;
            pDevice->bWindowed      = false;
			pDevice->MultiSampleTypeFullscreen = D3DMULTISAMPLE_NONE;
			pDevice->MultiSampleTypeWindowed = D3DMULTISAMPLE_NONE;

			dbgfile
				<< '[' << iAdapter << ']'
				<< '[' << inf.desc << ']'
				<< "Info (" << pDevice->d3dCaps.DevCaps << ')'
				<< endl;

            // Examine each format supported by the adapter to see if it will
            // work with this device and meets the needs of the application.
			struct
			{
				bool confirmed;
				DWORD behavior;
				D3DFORMAT depthformat;
			}
			extinfo[20];

            for (DWORD f = 0; f < min(dwNumFormats, 20); ++f)
            {
                extinfo[f].confirmed = false;
                extinfo[f].depthformat = D3DFMT_UNKNOWN;

                // Skip formats that cannot be used as render targets on this device
                if (FAILED(m_pD3D->CheckDeviceType(iAdapter, pDevice->DeviceType, formats[f], formats[f], FALSE)))
                {
					dbgfile
						<< '[' << iAdapter << ']'
                		<< '[' << inf.desc << ']'
						<< "Skipped (" << formats[f] << ')'
                		<< endl;

					continue;
                }

                if (pDevice->DeviceType == D3DDEVTYPE_HAL)
                {
                    // This system has a HAL device
                    bHALExists = true;

					// All devices that support d3d9 support windowed mode
                    bHALIsWindowedCompatible = true;
					
                	// HAL can run in a window for the current desktop mode
                    if (f == 0) bHALIsDesktopCompatible = true;

					// Confirm the device/format for HW vertex processing
					if (pDevice->d3dCaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
					{

						//extinfo[f].behavior = D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_PUREDEVICE;
						extinfo[f].behavior = D3DCREATE_HARDWARE_VERTEXPROCESSING;

						extinfo[f].confirmed = true;

						dbgfile
							<< '[' << iAdapter << ']'
							<< '[' << inf.desc << ']'
							<< "Found HW device (" << formats[f] << ')'
							<< endl;
					}

                }

                // Confirm the device/format for SW vertex processing
				// try to use D3DCREATE_SOFTWARE_VERTEXPROCESSING
                if (!extinfo[f].confirmed)
                {
					extinfo[f].behavior = D3DCREATE_SOFTWARE_VERTEXPROCESSING;

					extinfo[f].confirmed = true;

					dbgfile
						<< '[' << iAdapter << ']'
						<< '[' << inf.desc << ']'
						<< "Found SW device (" << formats[f] << ')'
						<< endl;
                }

                // Find a suitable depth/stencil buffer format for this device/format
                if (extinfo[f].confirmed && m_bUseDepthBuffer)
                {
                    if (!FindDepthStencilFormat(iAdapter, pDevice->DeviceType,
                        formats[f], &extinfo[f].depthformat))
                    {
                        extinfo[f].confirmed = false;

						dbgfile
							<< '[' << iAdapter << ']'
							<< '[' << inf.desc << ']'
							<< "Couldn't find depth/stencil buffer (" << formats[f] << ')'
							<< endl;
                    }
                }
            }

            // Add all enumerated display modes with confirmed formats to the
            // device's list of valid modes
            for (auto m = 0; m < modes.size(); ++m)
            {
				auto& mode = modes[m];

                for (DWORD f = 0; f < min(dwNumFormats, 20); ++f)
                
                    if (mode.Format == formats[f] && extinfo[f].confirmed
						
						&& pDevice->DeviceType == D3DDEVTYPE_HAL)
						
					{
							// Add this mode to the device's list of valid modes
							pDevice->modes[pDevice->dwNumModes].Width      = mode.Width;
							pDevice->modes[pDevice->dwNumModes].Height     = mode.Height;
							pDevice->modes[pDevice->dwNumModes].Format     = mode.Format;
							pDevice->modes[pDevice->dwNumModes].dwBehavior = extinfo[f].behavior;
							pDevice->modes[pDevice->dwNumModes].DepthStencilFormat = extinfo[f].depthformat;
							pDevice->dwNumModes++;

							bHALIsSampleCompatible = true;

							if (mode.Width == m_nWidth &&
								mode.Height == m_nHeight &&
								(mode.Format == D3DFMT_X8R8G8B8 ||
									mode.Format == D3DFMT_R5G6B5 ||
									mode.Format == D3DFMT_X1R5G5B5))
							{
								m_dwCurrentMode = m;

								dbgfile
									<< '[' << iAdapter << ']'
									<< '[' << inf.desc << ']'
									<< "Selected device #1 (" << mode.Width << 'x' << mode.Height << ':' << extinfo[f].behavior << ')'
									<< endl;

								break;
							}
                        }
                    

				if (m_dwCurrentMode != -1) break;
				
            }

			if (m_dwCurrentMode == -1)
			{
				for (auto m = 0; m < modes.size(); ++m)
				{
					auto& mode = modes[m];

					for (DWORD f = 0; f < min(dwNumFormats, 20); ++f)
					
						if (mode.Format == formats[f] && extinfo[f].confirmed
							
							&& pDevice->DeviceType != D3DDEVTYPE_HAL)
						
						{
							// Add this mode to the device's list of valid modes
							pDevice->modes[pDevice->dwNumModes].Width = mode.Width;
							pDevice->modes[pDevice->dwNumModes].Height = mode.Height;
							pDevice->modes[pDevice->dwNumModes].Format = mode.Format;
							pDevice->modes[pDevice->dwNumModes].dwBehavior = extinfo[f].behavior;
							pDevice->modes[pDevice->dwNumModes].DepthStencilFormat = extinfo[f].depthformat;
							pDevice->dwNumModes++;

							bHALIsSampleCompatible = false;

							if (mode.Width == m_nWidth &&
								mode.Height == m_nHeight &&
								(mode.Format == D3DFMT_X8R8G8B8 ||
									mode.Format == D3DFMT_R5G6B5 ||
									mode.Format == D3DFMT_X1R5G5B5))
							{
								m_dwCurrentMode = m;

								dbgfile
									<< '[' << iAdapter << ']'
									<< '[' << inf.desc << ']'
									<< "Selected device #2 (" << mode.Width << 'x' << mode.Height << ':' << extinfo[f].behavior << ')'
									<< endl;

								break;
							}
						}
					
					if (m_dwCurrentMode != -1) break;
				}
			}

            // Check if the device is compatible with the desktop display mode
            // (which was added initially as formats[0])
			if (extinfo[0].confirmed)
            {
                pDevice->bCanDoWindowed = true;
                pDevice->bWindowed      = true;
            }

            // If valid modes were found, keep this device
            if(pDevice->dwNumModes > 0) pAdapter->dwNumDevices++;

			if (m_dwCurrentMode != -1) break;
        }

        // If valid devices were found, keep this adapter
        if (pAdapter->dwNumDevices > 0) m_dwNumAdapters++;

		if (m_dwCurrentMode != -1) break;
    }
	
	if (m_dwCurrentMode == -1)
	{
		MessageBox(NULL, STRMSG_C_DIRECTX_0002, STRMSG_WINDOW_TEXT, MB_OK);

		PostQuitMessage(0);		
    }

    // Return an error if no compatible devices were found
    if (m_dwNumAdapters == 0) return D3DAPPERR_NOCOMPATIBLEDEVICES;

    // Pick a default device that can render into a window
    // (This code assumes that the HAL device comes before the REF
    // device in the device array).
    for( DWORD a=0; a<m_dwNumAdapters; a++ )
    {
        for( DWORD d=0; d < m_Adapters[a].dwNumDevices; d++ )
        {
            if( m_Adapters[a].devices[d].bWindowed )
            {
                m_Adapters[a].dwCurrentDevice = d;
                m_dwAdapter = a;
                m_bWindowed = TRUE;

                // Display a warning message
                if( m_Adapters[a].devices[d].DeviceType == D3DDEVTYPE_REF )
                {
                    if( !bHALExists )
                        DisplayErrorMsg( D3DAPPERR_NOHARDWAREDEVICE, MSGWARN_SWITCHEDTOREF );
                    else if( !bHALIsSampleCompatible )
                        DisplayErrorMsg( D3DAPPERR_HALNOTCOMPATIBLE, MSGWARN_SWITCHEDTOREF );
                    else if( !bHALIsWindowedCompatible )
                        DisplayErrorMsg( D3DAPPERR_NOWINDOWEDHAL, MSGWARN_SWITCHEDTOREF );
                    else if( !bHALIsDesktopCompatible )
                        DisplayErrorMsg( D3DAPPERR_NODESKTOPHAL, MSGWARN_SWITCHEDTOREF );
                    else // HAL is desktop compatible, but not sample compatible
                        DisplayErrorMsg( D3DAPPERR_NOHALTHISMODE, MSGWARN_SWITCHEDTOREF );
                }

                return S_OK;
            }
        }
    }

    return D3DAPPERR_NOWINDOWABLEDEVICES;
}




//-----------------------------------------------------------------------------
// Name: FindDepthStencilFormat()
// Desc: Finds a depth/stencil format for the given device that is compatible
//       with the render target format and meets the needs of the app.
//-----------------------------------------------------------------------------
BOOL CD3DApplication::FindDepthStencilFormat( UINT iAdapter, 
											 D3DDEVTYPE DeviceType,
											 D3DFORMAT TargetFormat, 
											 D3DFORMAT* pDepthStencilFormat )
{
	FLOG( "CD3DApplication::FindDepthStencilFormat( UINT iAdapter,D3DDEVTYPE DeviceType,D3DFORMAT TargetFormat,D3DFORMAT* pDepthStencilFormat )" );
//    if( m_dwMinDepthBits <= 16 && m_dwMinStencilBits == 0 )
//    {
//        if( SUCCEEDED( m_pD3D->CheckDeviceFormat( iAdapter, DeviceType,
//            TargetFormat, D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, D3DFMT_D16 ) ) )
//        {
//            if( SUCCEEDED( m_pD3D->CheckDepthStencilMatch( iAdapter, DeviceType,
//                TargetFormat, TargetFormat, D3DFMT_D16 ) ) )
//            {
//                *pDepthStencilFormat = D3DFMT_D16;
//                return TRUE;
//            }
//        }
//    }
//
//    if( m_dwMinDepthBits <= 15 && m_dwMinStencilBits <= 1 )
//    {
//        if( SUCCEEDED( m_pD3D->CheckDeviceFormat( iAdapter, DeviceType,
//            TargetFormat, D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, D3DFMT_D15S1 ) ) )
//        {
//            if( SUCCEEDED( m_pD3D->CheckDepthStencilMatch( iAdapter, DeviceType,
//                TargetFormat, TargetFormat, D3DFMT_D15S1 ) ) )
//            {
//                *pDepthStencilFormat = D3DFMT_D15S1;
//                return TRUE;
//            }
//        }
//    }
//
//    if( m_dwMinDepthBits <= 24 && m_dwMinStencilBits == 0 )
//    {
//        if( SUCCEEDED( m_pD3D->CheckDeviceFormat( iAdapter, DeviceType,
//            TargetFormat, D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, D3DFMT_D24X8 ) ) )
//        {
//            if( SUCCEEDED( m_pD3D->CheckDepthStencilMatch( iAdapter, DeviceType,
//                TargetFormat, TargetFormat, D3DFMT_D24X8 ) ) )
//            {
//                *pDepthStencilFormat = D3DFMT_D24X8;
//                return TRUE;
//            }
//        }
//    }
//
//    if( m_dwMinDepthBits <= 24 && m_dwMinStencilBits <= 8 )
//    {
//        if( SUCCEEDED( m_pD3D->CheckDeviceFormat( iAdapter, DeviceType,
//            TargetFormat, D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, D3DFMT_D24S8 ) ) )
//        {
//            if( SUCCEEDED( m_pD3D->CheckDepthStencilMatch( iAdapter, DeviceType,
//                TargetFormat, TargetFormat, D3DFMT_D24S8 ) ) )
//            {
//                *pDepthStencilFormat = D3DFMT_D24S8;
//                return TRUE;
//            }
//        }
//    }
//
//    if( m_dwMinDepthBits <= 24 && m_dwMinStencilBits <= 4 )
//    {
//        if( SUCCEEDED( m_pD3D->CheckDeviceFormat( iAdapter, DeviceType,
//            TargetFormat, D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, D3DFMT_D24X4S4 ) ) )
//        {
//            if( SUCCEEDED( m_pD3D->CheckDepthStencilMatch( iAdapter, DeviceType,
//                TargetFormat, TargetFormat, D3DFMT_D24X4S4 ) ) )
//            {
//                *pDepthStencilFormat = D3DFMT_D24X4S4;
//                return TRUE;
//            }
//        }
//    }
//
//    if( m_dwMinDepthBits <= 32 && m_dwMinStencilBits == 0 )
//    {
//        if( SUCCEEDED( m_pD3D->CheckDeviceFormat( iAdapter, DeviceType,
//            TargetFormat, D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, D3DFMT_D32 ) ) )
//        {
//            if( SUCCEEDED( m_pD3D->CheckDepthStencilMatch( iAdapter, DeviceType,
//                TargetFormat, TargetFormat, D3DFMT_D32 ) ) )
//            {
//                *pDepthStencilFormat = D3DFMT_D32;
//                return TRUE;
//            }
//        }
//    }

	// 수정

	if( SUCCEEDED( m_pD3D->CheckDeviceFormat( iAdapter, DeviceType,
		TargetFormat, D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, D3DFMT_D32 ) ) )
	{
		if( SUCCEEDED( m_pD3D->CheckDepthStencilMatch( iAdapter, DeviceType,
			TargetFormat, TargetFormat, D3DFMT_D32 ) ) )
		{
			*pDepthStencilFormat = D3DFMT_D32;
			return TRUE;
		}
	}
	
	if( SUCCEEDED( m_pD3D->CheckDeviceFormat( iAdapter, DeviceType,
		TargetFormat, D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, D3DFMT_D24S8 ) ) )
	{
		if( SUCCEEDED( m_pD3D->CheckDepthStencilMatch( iAdapter, DeviceType,
			TargetFormat, TargetFormat, D3DFMT_D24S8 ) ) )
		{
			*pDepthStencilFormat = D3DFMT_D24S8;
			return TRUE;
		}
	}
	
	if( SUCCEEDED( m_pD3D->CheckDeviceFormat( iAdapter, DeviceType,
		TargetFormat, D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, D3DFMT_D24X8 ) ) )
	{
		if( SUCCEEDED( m_pD3D->CheckDepthStencilMatch( iAdapter, DeviceType,
			TargetFormat, TargetFormat, D3DFMT_D24X8 ) ) )
		{
			*pDepthStencilFormat = D3DFMT_D24X8;
			return TRUE;
		}
	}
	
	if( SUCCEEDED( m_pD3D->CheckDeviceFormat( iAdapter, DeviceType,
		TargetFormat, D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, D3DFMT_D24X4S4 ) ) )
	{
		if( SUCCEEDED( m_pD3D->CheckDepthStencilMatch( iAdapter, DeviceType,
			TargetFormat, TargetFormat, D3DFMT_D24X4S4 ) ) )
		{
			*pDepthStencilFormat = D3DFMT_D24X4S4;
			return TRUE;
		}
	}
	
	if( SUCCEEDED( m_pD3D->CheckDeviceFormat( iAdapter, DeviceType,
		TargetFormat, D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, D3DFMT_D16 ) ) )
	{
		if( SUCCEEDED( m_pD3D->CheckDepthStencilMatch( iAdapter, DeviceType,
			TargetFormat, TargetFormat, D3DFMT_D16 ) ) )
		{
			*pDepthStencilFormat = D3DFMT_D16;
			return TRUE;
		}
	}
	if( SUCCEEDED( m_pD3D->CheckDeviceFormat( iAdapter, DeviceType,
		TargetFormat, D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, D3DFMT_D15S1 ) ) )
	{
		if( SUCCEEDED( m_pD3D->CheckDepthStencilMatch( iAdapter, DeviceType,
			TargetFormat, TargetFormat, D3DFMT_D15S1 ) ) )
		{
			*pDepthStencilFormat = D3DFMT_D15S1;
			return TRUE;
		}
	}
	
	
	
	

    return FALSE;
}




//-----------------------------------------------------------------------------
// Name: MsgProc()
// Desc: Message handling function.
//-----------------------------------------------------------------------------
LRESULT CD3DApplication::MsgProc( HWND hWnd, UINT uMsg, 
								 WPARAM wParam,
                                 LPARAM lParam )
{
	FLOG( "CD3DApplication::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )" );
    HRESULT hr;
	
    switch( uMsg )
    {
	case WM_PAINT:
		// Handle paint messages when the app is not ready
		if( m_pd3dDevice && !m_bReady )
		{
			if( m_bWindowed )
			{
				Render();
				m_pd3dDevice->Present( NULL, NULL, NULL, NULL );
			}
		}
		break;
		
	case WM_ACTIVATEAPP:
		m_bHasFocus = (BOOL) wParam;
		break;
		
	case WM_GETMINMAXINFO:
		((MINMAXINFO*)lParam)->ptMinTrackSize.x = 100;
		((MINMAXINFO*)lParam)->ptMinTrackSize.y = 100;
		break;
		
	case WM_ENTERSIZEMOVE:
		{
			// Halt frame movement while the app is sizing or moving
			Pause( TRUE );

			// 2008-11-06 by bhsohn 마우스 가두기 모드 보완			
			{
				SetClipCursor(FALSE);
			}
		}
		break;
		
	case WM_SIZE:
		// Check to see if we are losing our window...
		if( SIZE_MAXHIDE==wParam || SIZE_MINIMIZED==wParam )
		{
			if( m_bClipCursorWhenFullscreen && !m_bWindowed )
				ClipCursor( NULL );
			m_bActive = FALSE;
			// 2007-12-05 by bhsohn 디바이스 잃어 버렸을떄에 대한 처리			
			//DXUtil_Timer( TIMER_STOP );
			
		}
		else
		{
			m_bActive = TRUE;
			// 2007-12-05 by bhsohn 디바이스 잃어 버렸을떄에 대한 처리			
			//DXUtil_Timer( TIMER_START );
		}
		break;
		
	case WM_EXITSIZEMOVE:
		Pause( FALSE );
		
		if( m_bActive && m_bWindowed )
		{
			// 2008-11-06 by bhsohn 마우스 가두기 모드 보완			
			{
				RefreshMouseMode();
			}

			RECT rcClientOld;
			rcClientOld = m_rcWindowClient;
			
			// Update window properties
			GetWindowRect( m_hWnd, &m_rcWindowBounds );
			GetClientRect( m_hWnd, &m_rcWindowClient );
			
			if( rcClientOld.right - rcClientOld.left !=
				m_rcWindowClient.right - m_rcWindowClient.left ||
				rcClientOld.bottom - rcClientOld.top !=
				m_rcWindowClient.bottom - m_rcWindowClient.top)
			{
				// A new window size will require a new backbuffer
				// size, so the 3D structures must be changed accordingly.
				m_bReady = FALSE;
				
				m_d3dpp.BackBufferWidth  = m_rcWindowClient.right - m_rcWindowClient.left;
				m_d3dpp.BackBufferHeight = m_rcWindowClient.bottom - m_rcWindowClient.top;
				
				// Resize the 3D environment
				if( FAILED( hr = Resize3DEnvironment() ) )
				{
					DisplayErrorMsg( D3DAPPERR_RESIZEFAILED, MSGERR_APPMUSTEXIT );
					return 0;
				}
				
				m_bReady = TRUE;
			}			
		}
		
		break;
		
	case WM_SETCURSOR:
		// Turn off Windows cursor in fullscreen mode
		// 2008-01-08 by dgwoo 창모드에서 마우스 포인터가 창영역에 
		// 있을때 윈도우 마우스는 그리지 않는다.
		//if( m_bActive && m_bReady )&& !m_bWindowed )
		if( m_bActive && m_bReady )
		{
			SetCursor( NULL );
			if( m_bShowCursorWhenFullscreen && m_bShowCursor)
			{
				m_pd3dDevice->ShowCursor( TRUE );
				m_bShowCursor = TRUE;
			}
			return TRUE; // prevent Windows from setting cursor to window class cursor
		}
		break;
		
	case WM_MOUSEMOVE:
		if( m_bActive && m_bReady && m_pd3dDevice != NULL )
		{
			POINT ptCursor;
			GetCursorPos( &ptCursor );
			if( !m_bWindowed )
			{
				ScreenToClient( m_hWnd, &ptCursor );
			}
			CheckMouseReverse(&ptCursor);
			m_ptCursor = ptCursor;
		}
		break;
		
	case WM_ENTERMENULOOP:
		// Pause the app when menus are displayed
		Pause(TRUE);
		break;
		
	case WM_EXITMENULOOP:
		Pause(FALSE);
		break;
		
	case WM_CONTEXTMENU:
		// No context menus allowed in fullscreen mode
		if( m_bWindowed == FALSE )
			break;
		
		// Handle the app's context menu (via right mouse click)
		TrackPopupMenuEx( GetSubMenu( LoadMenu( 0, MAKEINTRESOURCE(IDR_POPUP) ), 0 ),
			TPM_VERTICAL, LOWORD(lParam), HIWORD(lParam), hWnd, NULL );
		break;
		
	case WM_NCHITTEST:
		// Prevent the user from selecting the menu in fullscreen mode
		if( !m_bWindowed )
			return HTCLIENT;
		
		break;
		
	case WM_POWERBROADCAST:
		switch( wParam )
		{
#ifndef PBT_APMQUERYSUSPEND
#define PBT_APMQUERYSUSPEND 0x0000
#endif
		case PBT_APMQUERYSUSPEND:
			// At this point, the app should save any data for open
			// network connections, files, etc., and prepare to go into
			// a suspended mode.
			return TRUE;
			
#ifndef PBT_APMRESUMESUSPEND
#define PBT_APMRESUMESUSPEND 0x0007
#endif
		case PBT_APMRESUMESUSPEND:
			// At this point, the app should recover any data, network
			// connections, files, etc., and resume running from when
			// the app was suspended.
			if( FAILED( hr = Resize3DEnvironment() ) )		// 2006-07-13 by ispark
			{
				DisplayErrorMsg( D3DAPPERR_RESIZEFAILED, MSGERR_APPMUSTEXIT );
				return 0;
			}
			return TRUE;
		}
		break;
		
        case WM_SYSCOMMAND:
            // Prevent moving/sizing and power loss in fullscreen mode
#ifdef _DEBUG
			if( LOWORD(wParam) == 61696 &&
				HIWORD(wParam) == 0 &&
				LOWORD(lParam) == 13 &&
				HIWORD(lParam) == 0)
			{	// Alt + Return 키를 누른경우
				if( m_bActive && m_bReady )
				{
					Pause( TRUE );
					
					if( FAILED( ToggleFullscreen() ) )
					{
						DisplayErrorMsg( D3DAPPERR_RESIZEFAILED, MSGERR_APPMUSTEXIT );
						return 0;
					}
					
					Pause( FALSE );                        
				}
				return 0;			
			}
#endif
			
			if(	LOWORD(wParam) == 61696 && 
				HIWORD(wParam) == 0 &&
				LOWORD(lParam) == 0 &&
				HIWORD(lParam) == 0)
			{	// 그냥 Alt 키만 누른경우
				return 0;
			}
			
			// ALT + F4 종료
#ifndef _DEBUG
			if(	LOWORD(wParam) == 61536 && 
				HIWORD(wParam) == 0 &&
				LOWORD(lParam) == 0 &&
				HIWORD(lParam) == 0)
			{
				return 0;
			}
#endif
			
            switch( wParam )
            {
			case SC_MOVE:
			case SC_SIZE:
			case SC_MAXIMIZE:
			case SC_KEYMENU:
			case SC_MONITORPOWER:
				if( FALSE == m_bWindowed )
					return 1;
				break;
            }
            break;
			
			case WM_COMMAND:
			{
				switch( LOWORD(wParam) )
				{
                case IDM_TOGGLESTART:
                    // Toggle frame movement
                    m_bFrameMoving = !m_bFrameMoving;
                    DXUtil_Timer( m_bFrameMoving ? TIMER_START : TIMER_STOP );
                    break;
					
                case IDM_SINGLESTEP:
                    // Single-step frame movement
                    if( FALSE == m_bFrameMoving )
                        DXUtil_Timer( TIMER_ADVANCE );
                    else
                        DXUtil_Timer( TIMER_STOP );
                    m_bFrameMoving = FALSE;
                    m_bSingleStep  = TRUE;
                    break;
					
                case IDM_CHANGEDEVICE:
                    // Prompt the user to select a new device or mode
                    if( m_bActive && m_bReady )
                    {
                        Pause(TRUE);
						
                        if( FAILED( hr = UserSelectNewDevice() ) )
                            return 0;
						
                        Pause(FALSE);
                    }
                    return 0;
					
                case IDM_TOGGLEFULLSCREEN:
                    // Toggle the fullscreen/window mode
                    if( m_bActive && m_bReady )
                    {
                        Pause( TRUE );
                        
                        if( FAILED( ToggleFullscreen() ) )
                        {
                            DisplayErrorMsg( D3DAPPERR_RESIZEFAILED, MSGERR_APPMUSTEXIT );
                            return 0;
                        }
						
                        Pause( FALSE );                        
                    }
                    return 0;
					
                case IDM_EXIT:
                    // Recieved key/menu command to exit app
                    SendMessage( hWnd, WM_CLOSE, 0, 0 );
                    return 0;
				}
				break;
			}
			case WM_CLOSE:
			{
#ifdef C_CLIENT_FILE_LOG
				g_cCustomOptimizer.WriteLog("WM_CLOSE #1"); // 2013-05-23 by bhsohn 클라이언트 파일로그(LOG) 시스템 추가
#endif
				if(m_hHangulDC)
				{
					// 2006-07-05 by ispark
//					DeleteObject(m_hHangulDC);
					DeleteDC(m_hHangulDC);
				}
#ifdef C_CLIENT_FILE_LOG
				g_cCustomOptimizer.WriteLog("WM_CLOSE #2"); // 2013-05-23 by bhsohn 클라이언트 파일로그(LOG) 시스템 추가
#endif
				// 2008-06-16 by bhsohn 태국 게임 종료후 웹페이지 뜨우게 함
				g_pD3dApp->ExcuteExplore();
#ifdef C_CLIENT_FILE_LOG
				g_cCustomOptimizer.WriteLog("WM_CLOSE #3"); // 2013-05-23 by bhsohn 클라이언트 파일로그(LOG) 시스템 추가
#endif

				// 2008-11-06 by bhsohn 마우스 가두기 모드 보완
				SetClipCursor(FALSE);
#ifdef C_CLIENT_FILE_LOG
				g_cCustomOptimizer.WriteLog("WM_CLOSE #4"); // 2013-05-23 by bhsohn 클라이언트 파일로그(LOG) 시스템 추가
#endif

				DBGOUT("CD3DApplication::MsgProc(), WM_CLOSE\n");
				Cleanup3DEnvironment();
#ifdef C_CLIENT_FILE_LOG
				g_cCustomOptimizer.WriteLog("WM_CLOSE #5"); // 2013-05-23 by bhsohn 클라이언트 파일로그(LOG) 시스템 추가
#endif
				DestroyWindow( hWnd );
#ifdef C_CLIENT_FILE_LOG
				g_cCustomOptimizer.WriteLog("WM_CLOSE #6"); // 2013-05-23 by bhsohn 클라이언트 파일로그(LOG) 시스템 추가
#endif
				PostQuitMessage(0);
#ifdef C_CLIENT_FILE_LOG
				g_cCustomOptimizer.WriteLog("WM_CLOSE #7"); // 2013-05-23 by bhsohn 클라이언트 파일로그(LOG) 시스템 추가
#endif
			}
    }
	// 2008-12-15 by bhsohn 비스타에서 IME시스템 동작 안되는 문제 해결
	if(WM_IME_SETCONTEXT == uMsg)
	{
// 		lparam &= ~( ISC_SHOWUICOMPOSITIONWINDOW
// 
//                 /*   |   ISC_SHOWUIALLCANDIDATEWINDOW    */ );
// 
// [??] ime ?? ??? ?? ?? ??? ???? ??|??? ???
		//lParam |= ISC_SHOWUIALLCANDIDATEWINDOW;
		lParam = 0;
		return DefWindowProc( hWnd, uMsg, wParam, lParam );

	}
	
	if(g_input.GetImeRtnValue())
	{
		//DbgOut("g_input.GetImeRtnValue() \n");
		g_input.SetImeRtnValue(FALSE);
		return 0;
	}
	// end 2008-12-15 by bhsohn 비스타에서 IME시스템 동작 안되는 문제 해결
	
    return DefWindowProc( hWnd, uMsg, wParam, lParam );
}




//-----------------------------------------------------------------------------
// Name: Initialize3DEnvironment()
// Desc:
//-----------------------------------------------------------------------------
HRESULT CD3DApplication::Initialize3DEnvironment()
{
	FLOG( "CD3DApplication::Initialize3DEnvironment()" );
    HRESULT hr;

    D3DAdapterInfo* pAdapterInfo = &m_Adapters[m_dwAdapter];
    D3DDeviceInfo*  pDeviceInfo  = &pAdapterInfo->devices[pAdapterInfo->dwCurrentDevice];
    D3DModeInfo*    pModeInfo    = &pDeviceInfo->modes[pDeviceInfo->dwCurrentMode];

    // Prepare window for possible windowed/fullscreen change
    AdjustWindowForChange();

    // Set up the presentation parameters
    ZeroMemory( &m_d3dpp, sizeof(m_d3dpp) );
    m_d3dpp.Windowed               = pDeviceInfo->bWindowed;
    m_d3dpp.BackBufferCount        = 1;
    m_d3dpp.SwapEffect             = D3DSWAPEFFECT_DISCARD;
    m_d3dpp.EnableAutoDepthStencil = m_bUseDepthBuffer;
    m_d3dpp.AutoDepthStencilFormat = pModeInfo->DepthStencilFormat;
    m_d3dpp.hDeviceWindow          = m_hWnd;
	m_d3dpp.Flags = 0; // = D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;
    if( m_bWindowed )
    {
		m_dwWindowStyle = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
        m_d3dpp.BackBufferWidth  = m_rcWindowClient.right - m_rcWindowClient.left;
        m_d3dpp.BackBufferHeight = m_rcWindowClient.bottom - m_rcWindowClient.top;
        m_d3dpp.BackBufferFormat = pAdapterInfo->d3ddmDesktop.Format;
		if (!g_pD3dApp->m_bVSync
#ifdef _ADVANCED_CONFIG
			|| !g_pD3dApp->m_bAdvConfVSync
#endif	// _ADVANCED_CONFIG
			)
			m_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
		else
			m_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;
    }
    else
    {
        m_d3dpp.BackBufferWidth  = pModeInfo->Width;
        m_d3dpp.BackBufferHeight = pModeInfo->Height;
        m_d3dpp.BackBufferFormat = pModeInfo->Format;
		if (!g_pD3dApp->m_bVSync
#ifdef _ADVANCED_CONFIG
			|| !g_pD3dApp->m_bAdvConfVSync
#endif	// _ADVANCED_CONFIG
			)
			m_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
		else
			m_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;
//		m_d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
    }

#ifdef _ADVANCED_CONFIG
		switch (g_pD3dApp->m_nAdvConfAntiAliasing)
		{
		DWORD quality;
		case 0:
			m_d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
			break;
		case 2:

			if (m_pD3D->CheckDeviceMultiSampleType(m_dwAdapter,
				pDeviceInfo->DeviceType, m_d3dpp.BackBufferFormat, FALSE,
				D3DMULTISAMPLE_2_SAMPLES, &quality) == D3D_OK)
			{
				m_d3dpp.MultiSampleType = D3DMULTISAMPLE_2_SAMPLES;
			}
			else
			{
				DbgOut("2 Samples not available!");
			}
			break;
		case 4:
			if (m_pD3D->CheckDeviceMultiSampleType(m_dwAdapter,
				pDeviceInfo->DeviceType, m_d3dpp.BackBufferFormat, FALSE,
				D3DMULTISAMPLE_4_SAMPLES, &quality) == D3D_OK)
			{
				m_d3dpp.MultiSampleType = D3DMULTISAMPLE_4_SAMPLES;
			}
			else
			{
				DbgOut("4 Samples not available!");
			}
			break;
		case 8:
			if (m_pD3D->CheckDeviceMultiSampleType(m_dwAdapter,
				pDeviceInfo->DeviceType, m_d3dpp.BackBufferFormat, FALSE,
				D3DMULTISAMPLE_8_SAMPLES, &quality) == D3D_OK)
			{
				m_d3dpp.MultiSampleType = D3DMULTISAMPLE_8_SAMPLES;
			}
			else
			{
				DbgOut("8 Samples not available!");
			}
			break;
		default:
			m_d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
			break;
		}
		m_d3dpp.MultiSampleQuality = (DWORD)g_pD3dApp->m_nAdvConfAntiAliasQuality;
#else
	//if (pDeviceInfo->bWindowed)
	//	m_d3dpp.MultiSampleType = pDeviceInfo->MultiSampleTypeWindowed;
	//else
	//	m_d3dpp.MultiSampleType = pDeviceInfo->MultiSampleTypeFullscreen;
	DWORD quality;

	auto antialias = false;

	if (SUCCEEDED(m_pD3D->CheckDeviceMultiSampleType(m_dwAdapter,
		pDeviceInfo->DeviceType, D3DFMT_X8R8G8B8, pDeviceInfo->bWindowed,
		D3DMULTISAMPLE_4_SAMPLES, &quality) == D3D_OK) && quality > 0)
	{
		m_d3dpp.MultiSampleType = D3DMULTISAMPLE_4_SAMPLES;
		m_d3dpp.MultiSampleQuality = 0;
		// m_d3dpp.Flags &= ~D3DPRESENTFLAG_LOCKABLE_BACKBUFFER; // 2016-09-06 panoskj to enable multisampling

		antialias = true;
	}
	else
	{
		if (pDeviceInfo->bWindowed)
			m_d3dpp.MultiSampleType = pDeviceInfo->MultiSampleTypeWindowed;
		else
			m_d3dpp.MultiSampleType = pDeviceInfo->MultiSampleTypeFullscreen;
	}

#endif

    if( pDeviceInfo->d3dCaps.PrimitiveMiscCaps & D3DPMISCCAPS_NULLREFERENCE )
    {
        // Warn user about null ref device that can't render anything
		DisplayErrorMsg(D3DAPPERR_NULLREFDEVICE, 0);
	}

	// Create the device
	hr = m_pD3D->CreateDevice(m_dwAdapter, pDeviceInfo->DeviceType,
		m_hWndFocus, pModeInfo->dwBehavior,
		&m_d3dpp, &m_pd3dDevice);

	// 2015-10-19 panoskj if D3DCREATE_HARDWARE_VERTEXPROCESSING failed try this
	if (FAILED(hr)) hr = m_pD3D->CreateDevice(m_dwAdapter, pDeviceInfo->DeviceType,
		m_hWndFocus, D3DCREATE_MIXED_VERTEXPROCESSING,
		&m_d3dpp, &m_pd3dDevice);

	// 2015-10-19 panoskj if D3DCREATE_HARDWARE_VERTEXPROCESSING failed try this
	if (FAILED(hr)) hr = m_pD3D->CreateDevice(m_dwAdapter, pDeviceInfo->DeviceType,
		m_hWndFocus, D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&m_d3dpp, &m_pd3dDevice);
	if (FAILED(hr)) {
		if (pDeviceInfo->bWindowed)
			m_d3dpp.MultiSampleType = pDeviceInfo->MultiSampleTypeWindowed;
		else
			m_d3dpp.MultiSampleType = pDeviceInfo->MultiSampleTypeFullscreen;
		// Create the device
		hr = m_pD3D->CreateDevice(m_dwAdapter, pDeviceInfo->DeviceType,
			m_hWndFocus, pModeInfo->dwBehavior,
			&m_d3dpp, &m_pd3dDevice);

		// 2015-10-19 panoskj if D3DCREATE_HARDWARE_VERTEXPROCESSING failed try this
		if (FAILED(hr)) hr = m_pD3D->CreateDevice(m_dwAdapter, pDeviceInfo->DeviceType,
			m_hWndFocus, D3DCREATE_MIXED_VERTEXPROCESSING,
			&m_d3dpp, &m_pd3dDevice);

		// 2015-10-19 panoskj if D3DCREATE_HARDWARE_VERTEXPROCESSING failed try this
		if (FAILED(hr)) hr = m_pD3D->CreateDevice(m_dwAdapter, pDeviceInfo->DeviceType,
			m_hWndFocus, D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&m_d3dpp, &m_pd3dDevice);
	}
	
	if (SUCCEEDED(hr))
    {
#ifndef _ADVANCED_CONFIG // forcefully add antialias
		if (antialias) m_pd3dDevice->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, TRUE);
#endif

        // When moving from fullscreen to windowed mode, it is important to
        // adjust the window size after recreating the device rather than
        // beforehand to ensure that you get the window size you want.  For
        // example, when switching from 640x480 fullscreen to windowed with
        // a 1000x600 window on a 1024x768 desktop, it is impossible to set
        // the window size to 1000x600 until after the display mode has
        // changed to 1024x768, because windows cannot be larger than the
        // desktop.
        if( m_bWindowed )
        {
			if( g_cCustomOptimizer.m_bCoordinates )
			{
				SetWindowPos( m_hWnd, HWND_NOTOPMOST, g_cCustomOptimizer.m_nX, g_cCustomOptimizer.m_nY,
					( m_rcWindowBounds.right - m_rcWindowBounds.left ),
					( m_rcWindowBounds.bottom - m_rcWindowBounds.top ), SWP_SHOWWINDOW );
			}
			else
			{
				SetWindowPos( m_hWnd, HWND_NOTOPMOST, m_rcWindowBounds.left, m_rcWindowBounds.top,
					( m_rcWindowBounds.right - m_rcWindowBounds.left ),
					( m_rcWindowBounds.bottom - m_rcWindowBounds.top ), SWP_SHOWWINDOW );
			}
        }
		// 2010-09-29 by jskim, SpriteDevice Lost 되었을 때 구조 변경
		D3DXCreateSprite(m_pd3dDevice , &m_pd3dxSprite);
		// end 2010-09-29 by jskim, SpriteDevice Lost 되었을 때 구조 변경

        // Store device Caps
		m_pd3dDevice->GetDeviceCaps(&m_d3dCaps);
		#ifdef _DEBUG
//		DbgOutDeviceCaps(m_d3dCaps);
		#endif
        m_dwCreateFlags = pModeInfo->dwBehavior;

		// 2005-08-10 by ispark
		// 2005-08-09 by ispark
		// Only accept devices which support the new depth bias caps
		if(!((m_d3dCaps.RasterCaps & D3DPRASTERCAPS_SLOPESCALEDEPTHBIAS) && (m_d3dCaps.RasterCaps & D3DPRASTERCAPS_DEPTHBIAS)))
		{
		   DBGOUT("D3DPRASTERCAPS_SLOPESCALEDEPTHBIAS : Failure\nD3DPRASTERCAPS_DEPTHBIAS : Failure\n");
		   g_pD3dApp->m_nDephbias = -1;
		}
		else
		{
		   DBGOUT("D3DPRASTERCAPS_SLOPESCALEDEPTHBIAS : Success\nD3DPRASTERCAPS_DEPTHBIAS : Success\n");
			g_pD3dApp->m_nDephbias = 1;		// 2005-08-10 by ispark 사용 가능
		}

        // Store device description
        if( pDeviceInfo->DeviceType == D3DDEVTYPE_REF )
            lstrcpy( m_strDeviceStats, TEXT("REF") );
        else if( pDeviceInfo->DeviceType == D3DDEVTYPE_HAL )
            lstrcpy( m_strDeviceStats, TEXT("HAL") );
        else if( pDeviceInfo->DeviceType == D3DDEVTYPE_SW )
            lstrcpy( m_strDeviceStats, TEXT("SW") );

        if( pModeInfo->dwBehavior & D3DCREATE_HARDWARE_VERTEXPROCESSING &&
            pModeInfo->dwBehavior & D3DCREATE_PUREDEVICE )
        {
            if( pDeviceInfo->DeviceType == D3DDEVTYPE_HAL )
                lstrcat( m_strDeviceStats, TEXT(" (pure hw vp)") );
            else
                lstrcat( m_strDeviceStats, TEXT(" (simulated pure hw vp)") );
        }
        else if( pModeInfo->dwBehavior & D3DCREATE_HARDWARE_VERTEXPROCESSING )
        {
            if( pDeviceInfo->DeviceType == D3DDEVTYPE_HAL )
                lstrcat( m_strDeviceStats, TEXT(" (hw vp)") );
            else
                lstrcat( m_strDeviceStats, TEXT(" (simulated hw vp)") );
        }
        else if( pModeInfo->dwBehavior & D3DCREATE_MIXED_VERTEXPROCESSING )
        {
            if( pDeviceInfo->DeviceType == D3DDEVTYPE_HAL )
                lstrcat( m_strDeviceStats, TEXT(" (mixed vp)") );
            else
                lstrcat( m_strDeviceStats, TEXT(" (simulated mixed vp)") );
        }
        else if( pModeInfo->dwBehavior & D3DCREATE_SOFTWARE_VERTEXPROCESSING )
        {
            lstrcat( m_strDeviceStats, TEXT(" (sw vp)") );
        }

        if( pDeviceInfo->DeviceType == D3DDEVTYPE_HAL )
        {
            lstrcat( m_strDeviceStats, TEXT(": ") );
            lstrcat( m_strDeviceStats, pAdapterInfo->d3dAdapterIdentifier.Description );
        }

        // Store render target surface desc
		// 2005-01-04 by jschoi
        LPDIRECT3DSURFACE9 pBackBuffer;
        m_pd3dDevice->GetBackBuffer( 0,0, D3DBACKBUFFER_TYPE_MONO, &pBackBuffer );
        pBackBuffer->GetDesc( &m_d3dsdBackBuffer );
        pBackBuffer->Release();

        // Set up the fullscreen cursor by dhkwon
        if( m_bShowCursorWhenFullscreen && !m_bWindowed )
        {
/*
            HCURSOR hCursor;
#ifdef _WIN64
            hCursor = (HCURSOR)GetClassLongPtr( m_hWnd, GCLP_HCURSOR );
#else
            hCursor = (HCURSOR)GetClassLong( m_hWnd, GCL_HCURSOR );
#endif
            D3DUtil_SetDeviceCursor( m_pd3dDevice, hCursor, TRUE );
//*/
            D3DUtil_SetDeviceCursor( m_pd3dDevice, m_hCursor, TRUE );
			if(m_bShowCursor)
				m_pd3dDevice->ShowCursor( TRUE );
        }

        // Confine cursor to fullscreen window
        if( m_bClipCursorWhenFullscreen )
        {
            if (!m_bWindowed )
            {
                RECT rcWindow;
                GetWindowRect( m_hWnd, &rcWindow );
                ClipCursor( &rcWindow );
            }
            else
            {
                ClipCursor( NULL );
            }
        }

        // Initialize the app's device-dependent objects
        hr = InitDeviceObjects();
        if( SUCCEEDED(hr) )
        {
            hr = RestoreDeviceObjects();
            if( SUCCEEDED(hr) )
            {
                m_bActive = TRUE;
                return S_OK;
            }
        }

        // Cleanup before we try again
        InvalidateDeviceObjects();
        DeleteDeviceObjects();
        SAFE_RELEASE( m_pd3dDevice );
    }
	else
	{
		char buffer[128];

		sprintf_s(buffer, "D3DX CreateDevice failed !!\nError: %s\nDescription: %s\nAntialias: %d Behavior: %d\n",
			DXGetErrorString9(hr), DXGetErrorDescription9(hr), antialias ? 1 : 0, pModeInfo->dwBehavior);

		MessageBox(nullptr, buffer, "DirectX Error", MB_OK);
	}

    // If that failed, fall back to the reference rasterizer
    if( pDeviceInfo->DeviceType == D3DDEVTYPE_HAL )
    {
        // Select the default adapter
        m_dwAdapter = 0L;
        pAdapterInfo = &m_Adapters[m_dwAdapter];

        // Look for a software device
        for( UINT i=0L; i<pAdapterInfo->dwNumDevices; i++ )
        {
            if( pAdapterInfo->devices[i].DeviceType == D3DDEVTYPE_REF )
            {
                pAdapterInfo->dwCurrentDevice = i;
                pDeviceInfo = &pAdapterInfo->devices[i];
                m_bWindowed = pDeviceInfo->bWindowed;
                break;
            }
        }

        // Try again, this time with the reference rasterizer
        if( pAdapterInfo->devices[pAdapterInfo->dwCurrentDevice].DeviceType ==
            D3DDEVTYPE_REF )
        {
            // Make sure main window isn't topmost, so error message is visible
            SetWindowPos( m_hWnd, HWND_NOTOPMOST,
                          m_rcWindowBounds.left, m_rcWindowBounds.top,
                          ( m_rcWindowBounds.right - m_rcWindowBounds.left ),
                          ( m_rcWindowBounds.bottom - m_rcWindowBounds.top ),
                          SWP_SHOWWINDOW );
            AdjustWindowForChange();

            // Let the user know we are switching from HAL to the reference rasterizer
            DisplayErrorMsg( hr, MSGWARN_SWITCHEDTOREF );

            hr = Initialize3DEnvironment();
        }
    }
	
    return hr;
}




//-----------------------------------------------------------------------------
// Name:
// Desc:
//-----------------------------------------------------------------------------
HRESULT CD3DApplication::Resize3DEnvironment()
{
	FLOG( "CD3DApplication::Resize3DEnvironment()" );
    HRESULT hr;

    // Release all vidmem objects
//	DBGOUT("Resize3DEnvironment->InvalidateDeviceObjects Start\n");
    if( FAILED( hr = InvalidateDeviceObjects() ) )
        return hr;
//	DBGOUT("Resize3DEnvironment->InvalidateDeviceObjects End\n");

// 2010-09-29 by jskim, SpriteDevice Lost 되었을 때 구조 변경
	if(m_pd3dxSprite)
	{
		m_pd3dxSprite->Release();
	}
// end 2010-09-29 by jskim, SpriteDevice Lost 되었을 때 구조 변경

//	DBGOUT("m_pd3dDevice->Reset Start\n");
    // Reset the device
    if( FAILED( hr = m_pd3dDevice->Reset( &m_d3dpp ) ) )
        return hr;
//	DBGOUT("m_pd3dDevice->Reset End\n");

    // Store render target surface desc
    LPDIRECT3DSURFACE9 pBackBuffer;
    m_pd3dDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &pBackBuffer );
    pBackBuffer->GetDesc( &m_d3dsdBackBuffer );
    pBackBuffer->Release();

    // Set up the fullscreen cursor
    if( m_bShowCursorWhenFullscreen && !m_bWindowed )
    {
/*
            HCURSOR hCursor;
#ifdef _WIN64
        hCursor = (HCURSOR)GetClassLongPtr( m_hWnd, GCLP_HCURSOR );
#else
        hCursor = (HCURSOR)GetClassLong( m_hWnd, GCL_HCURSOR );
#endif
        D3DUtil_SetDeviceCursor( m_pd3dDevice, hCursor, TRUE );
//*/
        D3DUtil_SetDeviceCursor( m_pd3dDevice, m_hCursor, TRUE );
		if(m_bShowCursor)
			m_pd3dDevice->ShowCursor( TRUE );
    }

    // Confine cursor to fullscreen window
    if( m_bClipCursorWhenFullscreen )
    {
        if (!m_bWindowed )
        {
            RECT rcWindow;
            GetWindowRect( m_hWnd, &rcWindow );
            ClipCursor( &rcWindow );
        }
        else
        {
            ClipCursor( NULL );
        }
    }

    // Initialize the app's device-dependent objects
    hr = RestoreDeviceObjects();
    if( FAILED(hr) )
        return hr;

    // If the app is paused, trigger the rendering of the current frame
    if( FALSE == m_bFrameMoving )
    {
        m_bSingleStep = TRUE;
        DXUtil_Timer( TIMER_START );
        DXUtil_Timer( TIMER_STOP );
    }

    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: ToggleFullScreen()
// Desc: Called when user toggles between fullscreen mode and windowed mode
//-----------------------------------------------------------------------------
HRESULT CD3DApplication::ToggleFullscreen()
{
	FLOG( "CD3DApplication::ToggleFullscreen()" );
    // Get access to current adapter, device, and mode
    D3DAdapterInfo* pAdapterInfo = &m_Adapters[m_dwAdapter];
    D3DDeviceInfo*  pDeviceInfo  = &pAdapterInfo->devices[pAdapterInfo->dwCurrentDevice];
    D3DModeInfo*    pModeInfo    = &pDeviceInfo->modes[pDeviceInfo->dwCurrentMode];

    // Need device change if going windowed and the current device
    // can only be fullscreen
    if( !m_bWindowed && !pDeviceInfo->bCanDoWindowed )
        return ForceWindowed();

    m_bReady = FALSE;

    // Toggle the windowed state
    m_bWindowed = !m_bWindowed;
    pDeviceInfo->bWindowed = m_bWindowed;

    // Prepare window for windowed/fullscreen change
    AdjustWindowForChange();

    // Set up the presentation parameters
    m_d3dpp.Windowed               = pDeviceInfo->bWindowed;
    if( m_bWindowed )
        m_d3dpp.MultiSampleType    = pDeviceInfo->MultiSampleTypeWindowed;
    else
        m_d3dpp.MultiSampleType    = pDeviceInfo->MultiSampleTypeFullscreen;
    m_d3dpp.AutoDepthStencilFormat = pModeInfo->DepthStencilFormat;
    m_d3dpp.hDeviceWindow          = m_hWnd;
    if( m_bWindowed )
    {
        m_d3dpp.BackBufferWidth  = m_rcWindowClient.right - m_rcWindowClient.left;
        m_d3dpp.BackBufferHeight = m_rcWindowClient.bottom - m_rcWindowClient.top;
        m_d3dpp.BackBufferFormat = pAdapterInfo->d3ddmDesktop.Format;
    }
    else
    {
        m_d3dpp.BackBufferWidth  = pModeInfo->Width;
        m_d3dpp.BackBufferHeight = pModeInfo->Height;
        m_d3dpp.BackBufferFormat = pModeInfo->Format;
//		m_d3dpp.FullScreen_PresentationInterval =  D3DPRESENT_INTERVAL_IMMEDIATE;
//		m_d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
    }

    // Resize the 3D device
    if( FAILED( Resize3DEnvironment() ) )
    {
        if( m_bWindowed )
            return ForceWindowed();
        else
            return E_FAIL;
    }

    // When moving from fullscreen to windowed mode, it is important to
    // adjust the window size after resetting the device rather than
    // beforehand to ensure that you get the window size you want.  For
    // example, when switching from 640x480 fullscreen to windowed with
    // a 1000x600 window on a 1024x768 desktop, it is impossible to set
    // the window size to 1000x600 until after the display mode has
    // changed to 1024x768, because windows cannot be larger than the
    // desktop.
    if( m_bWindowed )
    {
        SetWindowPos( m_hWnd, HWND_NOTOPMOST,
                      m_rcWindowBounds.left, m_rcWindowBounds.top,
                      ( m_rcWindowBounds.right - m_rcWindowBounds.left ),
                      ( m_rcWindowBounds.bottom - m_rcWindowBounds.top ),
                      SWP_SHOWWINDOW );
    }
    m_bReady = TRUE;
    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: ForceWindowed()
// Desc: Switch to a windowed mode, even if that means picking a new device
//       and/or adapter
//-----------------------------------------------------------------------------
HRESULT CD3DApplication::ForceWindowed()
{
	FLOG( "CD3DApplication::ForceWindowed()" );
    HRESULT hr;
    D3DAdapterInfo* pAdapterInfoCur = &m_Adapters[m_dwAdapter];
    D3DDeviceInfo*  pDeviceInfoCur  = &pAdapterInfoCur->devices[pAdapterInfoCur->dwCurrentDevice];
    BOOL bFoundDevice = FALSE;

    if( pDeviceInfoCur->bCanDoWindowed )
    {
        bFoundDevice = TRUE;
    }
    else
    {
        // Look for a windowable device on any adapter
        D3DAdapterInfo* pAdapterInfo;
        DWORD dwAdapter;
        D3DDeviceInfo* pDeviceInfo;
        DWORD dwDevice;
        for( dwAdapter = 0; dwAdapter < m_dwNumAdapters; dwAdapter++ )
        {
            pAdapterInfo = &m_Adapters[dwAdapter];
            for( dwDevice = 0; dwDevice < pAdapterInfo->dwNumDevices; dwDevice++ )
            {
                pDeviceInfo = &pAdapterInfo->devices[dwDevice];
                if( pDeviceInfo->bCanDoWindowed )
                {
                    m_dwAdapter = dwAdapter;
                    pDeviceInfoCur = pDeviceInfo;
                    pAdapterInfo->dwCurrentDevice = dwDevice;
                    bFoundDevice = TRUE;
                    break;
                }
            }
            if( bFoundDevice )
                break;
        }
    }

    if( !bFoundDevice )
        return E_FAIL;

    pDeviceInfoCur->bWindowed = TRUE;
    m_bWindowed = TRUE;

    // Now destroy the current 3D device objects, then reinitialize

    m_bReady = FALSE;

    // Release all scene objects that will be re-created for the new device
    InvalidateDeviceObjects();
    DeleteDeviceObjects();

    // Release display objects, so a new device can be created
    if( m_pd3dDevice->Release() > 0L )
        return DisplayErrorMsg( D3DAPPERR_NONZEROREFCOUNT, MSGERR_APPMUSTEXIT );

    // Create the new device
    if( FAILED( hr = Initialize3DEnvironment() ) )
        return DisplayErrorMsg( hr, MSGERR_APPMUSTEXIT );
    m_bReady = TRUE;

    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: AdjustWindowForChange()
// Desc: Prepare the window for a possible change between windowed mode and
//       fullscreen mode.  This function is virtual and thus can be overridden
//       to provide different behavior, such as switching to an entirely
//       different window for fullscreen mode (as in the MFC sample apps).
//-----------------------------------------------------------------------------
HRESULT CD3DApplication::AdjustWindowForChange()
{
	FLOG( "CD3DApplication::AdjustWindowForChange()" );
    if( m_bWindowed )
    {
        // Set windowed-mode style
// 2007-12-21 by dgwoo 창모드 지원
//        SetWindowLong( m_hWnd, GWL_STYLE, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | 
//								  WS_MINIMIZEBOX | WS_VISIBLE );
        SetWindowLong( m_hWnd, GWL_STYLE, m_dwWindowStyle);
		
		HMENU XButton;
		XButton = ::GetSystemMenu(m_hWnd, FALSE); 
		EnableMenuItem(XButton,SC_CLOSE, (MF_DISABLED|MF_GRAYED));
        if( m_hMenu != NULL )
            SetMenu( m_hWnd, m_hMenu );
    }
    else
    {
        // Set fullscreen-mode style
        SetWindowLong( m_hWnd, GWL_STYLE, WS_POPUP | WS_SYSMENU | WS_VISIBLE );
        m_hMenu = (HMENU)SetMenu( m_hWnd, NULL );
    }
    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: UserSelectNewDevice()
// Desc: Displays a dialog so the user can select a new adapter, device, or
//       display mode, and then recreates the 3D environment if needed
//-----------------------------------------------------------------------------
HRESULT CD3DApplication::UserSelectNewDevice()
{
	FLOG( "CD3DApplication::UserSelectNewDevice()" );
    HRESULT hr;

    // Can't display dialogs in fullscreen mode
    if( m_bWindowed == FALSE )
    {
        if( FAILED( ToggleFullscreen() ) )
        {
            DisplayErrorMsg( D3DAPPERR_RESIZEFAILED, MSGERR_APPMUSTEXIT );
            return E_FAIL;
        }
    }

    // Prompt the user to change the mode
    if( IDOK != DialogBoxParam( (HINSTANCE)GetModuleHandle(NULL),
                                MAKEINTRESOURCE(IDD_SELECTDEVICE), m_hWnd,
                                (DLGPROC)SelectDeviceProc, (LPARAM)this ) )
        return S_OK;

    // Get access to the newly selected adapter, device, and mode
    DWORD dwDevice;
    dwDevice  = m_Adapters[m_dwAdapter].dwCurrentDevice;
    m_bWindowed = m_Adapters[m_dwAdapter].devices[dwDevice].bWindowed;

    // Release all scene objects that will be re-created for the new device
    InvalidateDeviceObjects();
    DeleteDeviceObjects();

    // Release display objects, so a new device can be created
    if( m_pd3dDevice->Release() > 0L )
        return DisplayErrorMsg( D3DAPPERR_NONZEROREFCOUNT, MSGERR_APPMUSTEXIT );

    // Inform the display class of the change. It will internally
    // re-create valid surfaces, a d3ddevice, etc.
    if( FAILED( hr = Initialize3DEnvironment() ) )
        return DisplayErrorMsg( hr, MSGERR_APPMUSTEXIT );

    // If the app is paused, trigger the rendering of the current frame
    if( FALSE == m_bFrameMoving )
    {
        m_bSingleStep = TRUE;
        DXUtil_Timer( TIMER_START );
        DXUtil_Timer( TIMER_STOP );
    }

    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: SelectDeviceProc()
// Desc: Windows message handling function for the device select dialog
//-----------------------------------------------------------------------------
INT_PTR CALLBACK CD3DApplication::SelectDeviceProc( HWND hDlg, 
												   UINT msg,
                                                   WPARAM wParam, 
												   LPARAM lParam )
{
 	FLOG( "INT_PTR CALLBACK CD3DApplication::SelectDeviceProc)" );
   // Get access to the UI controls
    HWND hwndAdapterList        = GetDlgItem( hDlg, IDC_ADAPTER_COMBO );
    HWND hwndDeviceList         = GetDlgItem( hDlg, IDC_DEVICE_COMBO );
	// 2005-01-04 by jschoi - 주석
//    HWND hwndFullscreenModeList = GetDlgItem( hDlg, IDC_FULLSCREENMODES_COMBO );
    HWND hwndWindowedRadio      = GetDlgItem( hDlg, IDC_WINDOW );
    HWND hwndFullscreenRadio    = GetDlgItem( hDlg, IDC_FULLSCREEN );
    HWND hwndMultiSampleList    = GetDlgItem( hDlg, IDC_MULTISAMPLE_COMBO );
    BOOL bUpdateDlgControls     = FALSE;

    // Static state for adapter/device/mode selection
    static CD3DApplication* pd3dApp;
    static DWORD  dwOldAdapter, dwNewAdapter;
    static DWORD  dwOldDevice,  dwNewDevice;
    static DWORD  dwOldMode,    dwNewMode;
    static BOOL   bOldWindowed, bNewWindowed;
    static D3DMULTISAMPLE_TYPE OldMultiSampleTypeWindowed, NewMultiSampleTypeWindowed;
    static D3DMULTISAMPLE_TYPE OldMultiSampleTypeFullscreen, NewMultiSampleTypeFullscreen;

    // Working variables
    D3DAdapterInfo* pAdapter;
    D3DDeviceInfo*  pDevice;

    // Handle the initialization message
    if( WM_INITDIALOG == msg )
    {
        // Old state
        pd3dApp      = (CD3DApplication*)lParam;
        dwOldAdapter = pd3dApp->m_dwAdapter;
        pAdapter     = &pd3dApp->m_Adapters[dwOldAdapter];

        dwOldDevice  = pAdapter->dwCurrentDevice;
        pDevice      = &pAdapter->devices[dwOldDevice];

        dwOldMode    = pDevice->dwCurrentMode;
        bOldWindowed = pDevice->bWindowed;
        OldMultiSampleTypeWindowed = pDevice->MultiSampleTypeWindowed;
        OldMultiSampleTypeFullscreen = pDevice->MultiSampleTypeFullscreen;

        // New state is initially the same as the old state
        dwNewAdapter = dwOldAdapter;
        dwNewDevice  = dwOldDevice;
        dwNewMode    = dwOldMode;
        bNewWindowed = bOldWindowed;
        NewMultiSampleTypeWindowed = OldMultiSampleTypeWindowed;
        NewMultiSampleTypeFullscreen = OldMultiSampleTypeFullscreen;

        // Set flag to update dialog controls below
        bUpdateDlgControls = TRUE;
    }

    if( WM_COMMAND == msg )
    {
        // Get current UI state
        bNewWindowed  = Button_GetCheck( hwndWindowedRadio );

        if( IDOK == LOWORD(wParam) )
        {
            // Handle the case when the user hits the OK button. Check if any
            // of the options were changed
            if( dwNewAdapter != dwOldAdapter || dwNewDevice  != dwOldDevice  ||
                dwNewMode    != dwOldMode    || bNewWindowed != bOldWindowed ||
                NewMultiSampleTypeWindowed != OldMultiSampleTypeWindowed ||
                NewMultiSampleTypeFullscreen != OldMultiSampleTypeFullscreen )
            {
                pd3dApp->m_dwAdapter = dwNewAdapter;

                pAdapter = &pd3dApp->m_Adapters[dwNewAdapter];
                pAdapter->dwCurrentDevice = dwNewDevice;

                pAdapter->devices[dwNewDevice].dwCurrentMode = dwNewMode;
                pAdapter->devices[dwNewDevice].bWindowed     = bNewWindowed;
                pAdapter->devices[dwNewDevice].MultiSampleTypeWindowed = NewMultiSampleTypeWindowed;
                pAdapter->devices[dwNewDevice].MultiSampleTypeFullscreen = NewMultiSampleTypeFullscreen;

                EndDialog( hDlg, IDOK );
            }
            else
                EndDialog( hDlg, IDCANCEL );

            return TRUE;
        }
        else if( IDCANCEL == LOWORD(wParam) )
        {
            // Handle the case when the user hits the Cancel button
            EndDialog( hDlg, IDCANCEL );
            return TRUE;
        }
        else if( CBN_SELENDOK == HIWORD(wParam) )
        {
            if( LOWORD(wParam) == IDC_ADAPTER_COMBO )
            {
                dwNewAdapter = ComboBox_GetCurSel( hwndAdapterList );
                pAdapter     = &pd3dApp->m_Adapters[dwNewAdapter];

                dwNewDevice  = pAdapter->dwCurrentDevice;
                dwNewMode    = pAdapter->devices[dwNewDevice].dwCurrentMode;
                bNewWindowed = pAdapter->devices[dwNewDevice].bWindowed;
            }
            else if( LOWORD(wParam) == IDC_DEVICE_COMBO )
            {
                pAdapter     = &pd3dApp->m_Adapters[dwNewAdapter];

                dwNewDevice  = ComboBox_GetCurSel( hwndDeviceList );
                dwNewMode    = pAdapter->devices[dwNewDevice].dwCurrentMode;
                bNewWindowed = pAdapter->devices[dwNewDevice].bWindowed;
            }
			// 2005-01-04 by jschoi - 주석
//            else if( LOWORD(wParam) == IDC_FULLSCREENMODES_COMBO )
//            {
//                dwNewMode = ComboBox_GetCurSel( hwndFullscreenModeList );
//            }
            else if( LOWORD(wParam) == IDC_MULTISAMPLE_COMBO )
            {
                DWORD dwItem = ComboBox_GetCurSel( hwndMultiSampleList );
                if( bNewWindowed )
                    NewMultiSampleTypeWindowed = (D3DMULTISAMPLE_TYPE)ComboBox_GetItemData( hwndMultiSampleList, dwItem );
                else
                    NewMultiSampleTypeFullscreen = (D3DMULTISAMPLE_TYPE)ComboBox_GetItemData( hwndMultiSampleList, dwItem );
            }
        }
        // Keep the UI current
        bUpdateDlgControls = TRUE;
    }

    // Update the dialog controls
    if( bUpdateDlgControls )
    {
        // Reset the content in each of the combo boxes
        ComboBox_ResetContent( hwndAdapterList );
        ComboBox_ResetContent( hwndDeviceList );
		// 2005-01-04 by jschoi - 주석
//        ComboBox_ResetContent( hwndFullscreenModeList );
        ComboBox_ResetContent( hwndMultiSampleList );

        pAdapter = &pd3dApp->m_Adapters[dwNewAdapter];
        pDevice  = &pAdapter->devices[dwNewDevice];

        // Add a list of adapters to the adapter combo box
        for( DWORD a=0; a < pd3dApp->m_dwNumAdapters; a++ )
        {
            // Add device name to the combo box
            DWORD dwItem = ComboBox_AddString( hwndAdapterList,
                             pd3dApp->m_Adapters[a].d3dAdapterIdentifier.Description );

            // Set the item data to identify this adapter
            ComboBox_SetItemData( hwndAdapterList, dwItem, a );

            // Set the combobox selection on the current adapater
            if( a == dwNewAdapter )
                ComboBox_SetCurSel( hwndAdapterList, dwItem );
        }

        // Add a list of devices to the device combo box
        for( DWORD d=0; d < pAdapter->dwNumDevices; d++ )
        {
            // Add device name to the combo box
            DWORD dwItem = ComboBox_AddString( hwndDeviceList,
                                               pAdapter->devices[d].strDesc );

            // Set the item data to identify this device
            ComboBox_SetItemData( hwndDeviceList, dwItem, d );

            // Set the combobox selection on the current device
            if( d == dwNewDevice )
                ComboBox_SetCurSel( hwndDeviceList, dwItem );
        }

		DWORD m;
        // Add a list of modes to the mode combo box
        for( m=0; m < pDevice->dwNumModes; m++ )
        {
            DWORD BitDepth = 16;
            if( pDevice->modes[m].Format == D3DFMT_X8R8G8B8 ||
                pDevice->modes[m].Format == D3DFMT_A8R8G8B8 ||
                pDevice->modes[m].Format == D3DFMT_R8G8B8 )
            {
                BitDepth = 32;
            }

            // Add mode desc to the combo box
            TCHAR strMode[80];
            _stprintf( strMode, _T("%ld x %ld x %ld"), pDevice->modes[m].Width,
                                                       pDevice->modes[m].Height,
                                                       BitDepth );
			// 2005-01-04 by jschoi
//            DWORD dwItem = ComboBox_AddString( hwndFullscreenModeList, strMode );

            // Set the item data to identify this mode
			// 2005-01-04 by jschoi
//            ComboBox_SetItemData( hwndFullscreenModeList, dwItem, m );

            // Set the combobox selection on the current mode
			// 2005-01-04 by jschoi
//            if( m == dwNewMode )
//                ComboBox_SetCurSel( hwndFullscreenModeList, dwItem );
        }

        // Add a list of multisample modes to the multisample combo box
        for( m=0; m <= 16; m++ )
        {
            TCHAR strDesc[50];

            D3DFORMAT fmt;
            if( bNewWindowed )
                fmt = pd3dApp->m_Adapters[dwNewAdapter].d3ddmDesktop.Format;
            else
                fmt = pDevice->modes[dwNewMode].Format;

            if ( m == 1 ) // 1 is not a valid multisample type
                continue;

			// 2005-01-04 by jschoi
//            if( SUCCEEDED( pd3dApp->m_pD3D->CheckDeviceMultiSampleType( dwNewAdapter,
//                pDevice->DeviceType, fmt, bNewWindowed, (D3DMULTISAMPLE_TYPE)m ) ) )
            if( SUCCEEDED( pd3dApp->m_pD3D->CheckDeviceMultiSampleType( dwNewAdapter,
                pDevice->DeviceType, fmt, bNewWindowed, (D3DMULTISAMPLE_TYPE)m,NULL ) ) )
            {
                if( m == 0 )
                    lstrcpy( strDesc, _T("none") );
                else
                    wsprintf( strDesc, _T("%d samples"), m );

                // Add device name to the combo box
                DWORD dwItem = ComboBox_AddString( hwndMultiSampleList, strDesc );

                // Set the item data to identify this multisample type
                ComboBox_SetItemData( hwndMultiSampleList, dwItem, m );

                // Set the combobox selection on the current multisample type
                if( bNewWindowed )
                {
                    if( (D3DMULTISAMPLE_TYPE)m == NewMultiSampleTypeWindowed || m == 0 )
                        ComboBox_SetCurSel( hwndMultiSampleList, dwItem );
                }
                else
                {
                    if( (D3DMULTISAMPLE_TYPE)m == NewMultiSampleTypeFullscreen || m == 0 )
                        ComboBox_SetCurSel( hwndMultiSampleList, dwItem );
                }
            }
        }
        DWORD dwItem = ComboBox_GetCurSel( hwndMultiSampleList );
        if( bNewWindowed )
            NewMultiSampleTypeWindowed = (D3DMULTISAMPLE_TYPE)ComboBox_GetItemData( hwndMultiSampleList, dwItem );
        else
            NewMultiSampleTypeFullscreen = (D3DMULTISAMPLE_TYPE)ComboBox_GetItemData( hwndMultiSampleList, dwItem );
        EnableWindow( hwndMultiSampleList, ComboBox_GetCount( hwndMultiSampleList ) > 1);
        EnableWindow( hwndWindowedRadio, pDevice->bCanDoWindowed );

        if( bNewWindowed )
        {
            Button_SetCheck( hwndWindowedRadio,   TRUE );
            Button_SetCheck( hwndFullscreenRadio, FALSE );
			// 2005-01-04 by jschoi
//            EnableWindow( hwndFullscreenModeList, FALSE );
        }
        else
        {
            Button_SetCheck( hwndWindowedRadio,   FALSE );
            Button_SetCheck( hwndFullscreenRadio, TRUE );
			// 2005-01-04 by jschoi
//            EnableWindow( hwndFullscreenModeList, TRUE );
        }
        return TRUE;
    }

    return FALSE;
}




//-----------------------------------------------------------------------------
// Name: Run()
// Desc:
//-----------------------------------------------------------------------------
INT CD3DApplication::Run()
{
	FLOG( "CD3DApplication::Run()" );
    BOOL bGotMsg;
    MSG  msg;
	DWORD err = 0;
//	__try{
		// Load keyboard accelerators
		HACCEL hAccel = LoadAccelerators( NULL, MAKEINTRESOURCE(IDR_MAIN_ACCEL) );

		// Now we're ready to recieve and process Windows messages.
		msg.message = WM_NULL;
		PeekMessage( &msg, NULL, 0U, 0U, PM_NOREMOVE );

		// 2005-01-13 by jschoi - 웹과 연동을 위한 윈도우 생성
		// 웹과 연동을 위한 윈도우 클래스 등록
		LPSTR lpszClass = "WebWindow";
		WNDCLASSEX WndClassEx;
		WndClassEx.cbSize = sizeof(WNDCLASSEX);
		WndClassEx.cbClsExtra = 0;
		WndClassEx.cbWndExtra = 0;
		WndClassEx.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
		WndClassEx.hCursor = LoadCursor(NULL,IDC_ARROW);
		WndClassEx.hIcon = NULL;
		WndClassEx.hIconSm = NULL;
		WndClassEx.hInstance = m_hInstance;
		WndClassEx.lpfnWndProc = (WNDPROC)WebWndProc;
		WndClassEx.lpszClassName = lpszClass;
		WndClassEx.lpszMenuName = NULL;
		WndClassEx.style = NULL;
		RegisterClassEx( &WndClassEx );

		// 웹과 연동을 위한 차일드 윈도우 생성
		// 2006-11-17 by ispark, 테스트
		// 2007.04.24 by bhsohn China IME Working
//#if defined(LANGUAGE_ENGLISH) || defined(LANGUAGE_VIETNAM) || defined(YEDANG_RELEASE) || defined(LANGUAGE_CHINA)
//#else
//		CreateWebWindow();
//#endif
		// 2008-03-11 by dgwoo 메모리 풀
		CMemPoolClient::AddObject(sizeof(CBodyConditionInfo),MEMORYPOOL_DEFAULT_COUNT);
		CMemPoolClient::AddObject(sizeof(CEffectInfo),MEMORYPOOL_DEFAULT_COUNT);

		if(!CMemPoolClient::InitObjects())
		{// 실패.
			DBGOUT("Memory Pool Error\n");
		}
		// 2008-03-11 by dgwoo 메모리 풀
		while( WM_QUIT != msg.message  )
		{
			// Use PeekMessage() if the app is active, so we can use idle time to
			// render the scene. Else, use GetMessage() to avoid eating CPU time.
			// 2007-12-05 by bhsohn 디바이스 잃어 버렸을떄에 대한 처리
//			if( m_bActive )
//				bGotMsg = PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE );
//			else
//				bGotMsg = GetMessage( &msg, NULL, 0U, 0U );
			bGotMsg = PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE );

			// 2008-03-17 by bhsohn 윈도우 이동시 Tick안들어오는 문제 처리
			//DBGOUT("CD3DApplication::Run() bGotMsg[%d] Msg[%d][%d]\n", bGotMsg, 
			//								msg.message,msg.wParam);
			
			// 2008-04-07 by bhsohn 시스템키 걸러내는 시스템 추가
			if(msg.message == WM_SYSKEYDOWN )
			{
				// 허용하는 시스템 키인지 체크
				if(!g_pD3dApp->IsSysKeyDownPermisiion(msg.wParam,0))
				{
					bGotMsg = FALSE;
//					DBGOUT("CD3DApplication::Run() bGotMsg[%d] Msg[%d][%d]\n", bGotMsg, 
//						msg.message,msg.wParam);
				}
			}
			// 2007-12-30 by dgwoo Alt + F6으로 인한 capture되는것을 막음.
//			if((msg.message == WM_SYSKEYDOWN && msg.wParam == VK_F6)
//				|| (msg.message == WM_SYSKEYDOWN && msg.wParam == VK_ESCAPE)
//				||(msg.message == WM_NCLBUTTONDOWN && msg.wParam == 2 && !g_pD3dApp->IsPossibleWindowMove()))// 2008-03-17 by bhsohn 윈도우 이동시 Tick안들어오는 문제 처리
			// 2009-02-24 by bhsohn 캐쉬샵 충전 웹페이지 추가
// 			if(msg.message == WM_NCLBUTTONDOWN && msg.wParam == 2 && !g_pD3dApp->IsPossibleWindowMove())// 2008-03-17 by bhsohn 윈도우 이동시 Tick안들어오는 문제 처리
// 			{				
// 				bGotMsg = FALSE;
// 			}
			if(msg.message == WM_NCLBUTTONDOWN && msg.wParam == 2 )// 2008-03-17 by bhsohn 윈도우 이동시 Tick안들어오는 문제 처리
			{
				if(!g_pD3dApp->IsPossibleWindowMove())
				
					bGotMsg = FALSE;				
			}
			// end 2009-02-24 by bhsohn 캐쉬샵 충전 웹페이지 추가
			if( bGotMsg )
			{
				// Translate and dispatch the message
				if( 0 == TranslateAccelerator( m_hWnd, hAccel, &msg ) )
				{
					TranslateMessage( &msg );
					DispatchMessage( &msg );
				}
			}
			else
			{
				if( m_bDeviceLost )
				{
					// Yield some CPU time to other processes
					// 2007-04-10 by bhsohn 포커스 잃어 버렸을때의 버그 처리
					//Sleep( 100 ); // 100 milliseconds
					Sleep( LOST_DEVICE_SLEEP_TIME ); // 100 milliseconds

					LostDeviceTick();			// 2006-09-27 by ispark
				}
				
				// 2007-12-05 by bhsohn 디바이스 잃어 버렸을떄에 대한 처리
				if(FALSE == m_bActive)
				{
					Sleep( LOST_DEVICE_SLEEP_TIME ); // 100 milliseconds					
				}
				// end 2007-12-05 by bhsohn 디바이스 잃어 버렸을떄에 대한 처리

				// Render a frame during idle time (no messages are waiting)
				// 2007-12-05 by bhsohn 디바이스 잃어 버렸을떄에 대한 처리
				//if( m_bActive && m_bReady )
				if( m_bReady )
				{
					if( FAILED( Render3DEnvironment() ) )
					{
						DBGOUT("[WM_CLOSE] FAILED(Render3DEnvironment())" );
#ifdef C_CLIENT_FILE_LOG
						g_cCustomOptimizer.WriteLog("[WM_CLOSE] FAILED(Render3DEnvironment())"); // 2013-05-23 by bhsohn 클라이언트 파일로그(LOG) 시스템 추가
#endif
						SendMessage( m_hWnd, WM_CLOSE, 0, 0 );
					}
				}
			}
		}
//	}
//	__except(err){
//		;
//	}
		CMemPoolClient::AllDeleteObject();

		return (INT)msg.wParam;
}




//-----------------------------------------------------------------------------
// Name: Render3DEnvironment()
// Desc: Draws the scene.
//-----------------------------------------------------------------------------
HRESULT CD3DApplication::Render3DEnvironment()
{
	g_cCustomOptimizer.Tick();
	
    HRESULT hr;

	// Test the cooperative level to see if it's okay to render
	if( FAILED( hr = m_pd3dDevice->TestCooperativeLevel() ) )
	{
		// If the device was lost, do not render until we get it back
		if( D3DERR_DEVICELOST == hr )
		{
#ifdef ONLY_FULL_WINDOW_HSSON
			if( g_pD3dApp->m_dwGameState != _GAMEOVER )
			{
				MessageBox( NULL, STRMSG_C_101123_0401, "", MB_OK );
				g_pD3dApp->m_dwGameState = _GAMEOVER;
			}
#endif
			m_bDeviceLost = TRUE;
			// 2007-06-18 by bhsohn 알트텝 관련 버그 처리
			g_pD3dApp->SetKeyBoardLostDevice(m_bDeviceLost);

			// 2006-08-07 by ispark, 강제 종료시 게임 종료
			if(g_pD3dApp->m_bGameShutDown)
			{
				char szTemp[256] = {0, };
					sprintf ( szTemp , "%s\n[%s]" , STRMSG_C_060608_0000 , "D3DERR_DEVICELOST_1" );

				g_pD3dApp->NetworkErrorMsgBox( szTemp );	// "서버와 접속이 종료되었습니다. 게임이 종료됩니다."
				//g_pD3dApp->NetworkErrorMsgBox(STRMSG_C_060608_0000);	// "서버와 접속이 종료되었습니다. 게임이 종료됩니다."
			}
			// 2007-04-10 by bhsohn 포커스 잃어 버렸을때의 버그 처리
			//return S_OK;
		}

		// Check if the device needs to be resized.
		if( D3DERR_DEVICENOTRESET == hr )
		{
			// If we are windowed, read the desktop mode and use the same format for
			// the back buffer
			if( m_bWindowed )
			{
				D3DAdapterInfo* pAdapterInfo = &m_Adapters[m_dwAdapter];
				m_pD3D->GetAdapterDisplayMode( m_dwAdapter, &pAdapterInfo->d3ddmDesktop );
				m_d3dpp.BackBufferFormat = pAdapterInfo->d3ddmDesktop.Format;
			}

			if( FAILED( hr = Resize3DEnvironment() ) )
				return hr;

			// 2007-04-10 by bhsohn 포커스 잃어 버렸을때의 버그 처리
			return hr;
		}
		// 2007-04-10 by bhsohn 포커스 잃어 버렸을때의 버그 처리
		//return hr;
	}
	else
	{
		m_bDeviceLost = FALSE;
		// 2007-06-18 by bhsohn 알트텝 관련 버그 처리
		g_pD3dApp->SetKeyBoardLostDevice(m_bDeviceLost);
	}

    // Get the app's time, in seconds. Skip rendering if no time elapsed
    FLOAT fAppTime        = DXUtil_Timer( TIMER_GETAPPTIME );
    FLOAT fElapsedAppTime = DXUtil_Timer( TIMER_GETELAPSEDTIME );
    if( ( 0.0f == fElapsedAppTime ) && m_bFrameMoving )
        return S_OK;

    // FrameMove (animate) the scene
    if( m_bFrameMoving || m_bSingleStep )
    {
        // Store the time for the app
        m_fTime        = fAppTime;
        m_fElapsedTime = fElapsedAppTime;

		// 2007-04-10 by bhsohn 포커스 잃어 버렸을때의 버그 처리	
		if(m_bDeviceLost )
		{
			m_fElapsedTime = LOST_DEVICE_ELASPTIME;
		}

        // Frame move the scene
        if( FAILED( hr = FrameMove() ) )
            return hr;

        m_bSingleStep = FALSE;
		
    }

	// 2007-04-10 by bhsohn 포커스 잃어 버렸을때의 버그 처리	
	if(m_bDeviceLost )
	{			
		return S_OK;
		
    }

    // Render the scene as normal
    if( FAILED( hr = Render() ) )
        return hr;

    // Keep track of the frame count
    {
        static FLOAT fLastTime = 0.0f;
        static DWORD dwFrames  = 0L;
        FLOAT fTime = DXUtil_Timer( TIMER_GETABSOLUTETIME );
        ++dwFrames;

        // Update the scene stats once per second
        if( fTime - fLastTime > 1.0f )
        {
            m_fFPS    = dwFrames / (fTime - fLastTime);
            fLastTime = fTime;
            dwFrames  = 0L;

            // Get adapter's current mode so we can report
            // bit depth (back buffer depth may be unknown)
            D3DDISPLAYMODE mode;
            m_pD3D->GetAdapterDisplayMode(m_dwAdapter, &mode);

            _stprintf( m_strFrameStats, _T("%.02f fps (%dx%dx%d)"), m_fFPS,
                       m_d3dsdBackBuffer.Width, m_d3dsdBackBuffer.Height,
                       mode.Format==D3DFMT_X8R8G8B8?32:16 );
            D3DAdapterInfo* pAdapterInfo = &m_Adapters[m_dwAdapter];
            D3DDeviceInfo*  pDeviceInfo  = &pAdapterInfo->devices[pAdapterInfo->dwCurrentDevice];
            D3DModeInfo*    pModeInfo    = &pDeviceInfo->modes[pDeviceInfo->dwCurrentMode];
            if( m_bUseDepthBuffer )
            {

                switch( pModeInfo->DepthStencilFormat )
                {
                case D3DFMT_D16:
                    lstrcat( m_strFrameStats, _T(" (D16)") );
                    break;
                case D3DFMT_D15S1:
                    lstrcat( m_strFrameStats, _T(" (D15S1)") );
                    break;
                case D3DFMT_D24X8:
                    lstrcat( m_strFrameStats, _T(" (D24X8)") );
                    break;
                case D3DFMT_D24S8:
                    lstrcat( m_strFrameStats, _T(" (D24S8)") );
                    break;
                case D3DFMT_D24X4S4:
                    lstrcat( m_strFrameStats, _T(" (D24X4S4)") );
                    break;
                case D3DFMT_D32:
                    lstrcat( m_strFrameStats, _T(" (D32)") );
                    break;
                }
            }

            D3DMULTISAMPLE_TYPE MultiSampleType;
            if( m_bWindowed )
                MultiSampleType = pDeviceInfo->MultiSampleTypeWindowed;
            else
                MultiSampleType = pDeviceInfo->MultiSampleTypeFullscreen;
            switch( MultiSampleType )
            {
            case D3DMULTISAMPLE_2_SAMPLES:
                lstrcat( m_strFrameStats, _T(" (2x Multisample)") );
                break;
            case D3DMULTISAMPLE_3_SAMPLES:
                lstrcat( m_strFrameStats, _T(" (3x Multisample)") );
                break;
            case D3DMULTISAMPLE_4_SAMPLES:
                lstrcat( m_strFrameStats, _T(" (4x Multisample)") );
                break;
            case D3DMULTISAMPLE_5_SAMPLES:
                lstrcat( m_strFrameStats, _T(" (5x Multisample)") );
                break;
            case D3DMULTISAMPLE_6_SAMPLES:
                lstrcat( m_strFrameStats, _T(" (6x Multisample)") );
                break;
            case D3DMULTISAMPLE_7_SAMPLES:
                lstrcat( m_strFrameStats, _T(" (7x Multisample)") );
                break;
            case D3DMULTISAMPLE_8_SAMPLES:
                lstrcat( m_strFrameStats, _T(" (8x Multisample)") );
                break;
            case D3DMULTISAMPLE_9_SAMPLES:
                lstrcat( m_strFrameStats, _T(" (9x Multisample)") );
                break;
            case D3DMULTISAMPLE_10_SAMPLES:
                lstrcat( m_strFrameStats, _T(" (10x Multisample)") );
                break;
            case D3DMULTISAMPLE_11_SAMPLES:
                lstrcat( m_strFrameStats, _T(" (11x Multisample)") );
                break;
            case D3DMULTISAMPLE_12_SAMPLES:
                lstrcat( m_strFrameStats, _T(" (12x Multisample)") );
                break;
            case D3DMULTISAMPLE_13_SAMPLES:
                lstrcat( m_strFrameStats, _T(" (13x Multisample)") );
                break;
            case D3DMULTISAMPLE_14_SAMPLES:
                lstrcat( m_strFrameStats, _T(" (14x Multisample)") );
                break;
            case D3DMULTISAMPLE_15_SAMPLES:
                lstrcat( m_strFrameStats, _T(" (15x Multisample)") );
                break;
            case D3DMULTISAMPLE_16_SAMPLES:
                lstrcat( m_strFrameStats, _T(" (16x Multisample)") );
                break;
            }
        }
    }

    // Show the frame on the primary surface.
    hr = m_pd3dDevice->Present( NULL, NULL, NULL, NULL );

	// 2005-01-17 by jschoi - DeviceLost 처리 추가
    if( D3DERR_DEVICELOST == hr )
	{
        m_bDeviceLost = TRUE;
		
		// 2007-06-18 by bhsohn 알트텝 관련 버그 처리
		g_pD3dApp->SetKeyBoardLostDevice(m_bDeviceLost);

		if(g_pD3dApp->m_bGameShutDown)
		{
			char szTemp[256] = {0, };
				sprintf ( szTemp , "%s\n[%s]" , STRMSG_C_060608_0000 , "D3DERR_DEVICELOST_2" );

			g_pD3dApp->NetworkErrorMsgBox( szTemp );	// "서버와 접속이 종료되었습니다. 게임이 종료됩니다."
			//g_pD3dApp->NetworkErrorMsgBox(STRMSG_C_060608_0000);	// "서버와 접속이 종료되었습니다. 게임이 종료됩니다."
        }
	}

    return S_OK;
}


//-----------------------------------------------------------------------------
// Name: Pause()
// Desc: Called in to toggle the pause state of the app.
//-----------------------------------------------------------------------------
VOID CD3DApplication::Pause( BOOL bPause )
{
	FLOG( "CD3DApplication::Pause( BOOL bPause )" );
    static DWORD dwAppPausedCount = 0L;

    dwAppPausedCount += ( bPause ? +1 : -1 );
    m_bReady          = ( dwAppPausedCount ? FALSE : TRUE );

    // Handle the first pause request (of many, nestable pause requests)
    if( bPause && ( 1 == dwAppPausedCount ) )
    {
        // Stop the scene from animating
        if( m_bFrameMoving )
            DXUtil_Timer( TIMER_STOP );
    }

    if( 0 == dwAppPausedCount )
    {
        // Restart the timers
        if( m_bFrameMoving ){
            DXUtil_Timer( TIMER_START );
		}
    }
}




//-----------------------------------------------------------------------------
// Name: Cleanup3DEnvironment()
// Desc: Cleanup scene objects
//-----------------------------------------------------------------------------
VOID CD3DApplication::Cleanup3DEnvironment()
{
	FLOG( "CD3DApplication::Cleanup3DEnvironment()" );
    m_bActive = FALSE;
    m_bReady  = FALSE;

    if( m_pd3dDevice )
    {
        InvalidateDeviceObjects();
        DeleteDeviceObjects();

        m_pd3dDevice->Release();
        m_pD3D->Release();

        m_pd3dDevice = NULL;
        m_pD3D       = NULL;
    }

    FinalCleanup();
}




//-----------------------------------------------------------------------------
// Name: DisplayErrorMsg()
// Desc: Displays error messages in a message box
//-----------------------------------------------------------------------------
HRESULT CD3DApplication::DisplayErrorMsg( HRESULT hr, DWORD dwType )
{
	FLOG( "CD3DApplication::DisplayErrorMsg( HRESULT hr, DWORD dwType )" );
    TCHAR strMsg[512];

    switch( hr )
    {
        case D3DAPPERR_NODIRECT3D:
            _tcscpy( strMsg, _T("Could not initialize Direct3D. You may\n")
                             _T("want to check that the latest version of\n")
                             _T("DirectX is correctly installed on your\n")
                             _T("system.  Also make sure that this program\n")
                             _T("was compiled with header files that match\n")
                             _T("the installed DirectX DLLs.") );
            break;

        case D3DAPPERR_NOCOMPATIBLEDEVICES:
            _tcscpy( strMsg, _T("Could not find any compatible Direct3D\n")
                             _T("devices.") );
            break;

        case D3DAPPERR_NOWINDOWABLEDEVICES:
            _tcscpy( strMsg, _T("This sample cannot run in a desktop\n")
                             _T("window with the current display settings.\n")
                             _T("Please change your desktop settings to a\n")
                             _T("16- or 32-bit display mode and re-run this\n")
                             _T("sample.") );
            break;

        case D3DAPPERR_NOHARDWAREDEVICE:
            _tcscpy( strMsg, _T("No hardware-accelerated Direct3D devices\n")
                             _T("were found.") );
            break;

        case D3DAPPERR_HALNOTCOMPATIBLE:
            _tcscpy( strMsg, _T("This sample requires functionality that is\n")
                             _T("not available on your Direct3D hardware\n")
                             _T("accelerator.") );
            break;

        case D3DAPPERR_NOWINDOWEDHAL:
            _tcscpy( strMsg, _T("Your Direct3D hardware accelerator cannot\n")
                             _T("render into a window.\n")
                             _T("Press F2 while the app is running to see a\n")
                             _T("list of available devices and modes.") );
            break;

        case D3DAPPERR_NODESKTOPHAL:
            _tcscpy( strMsg, _T("Your Direct3D hardware accelerator cannot\n")
                             _T("render into a window with the current\n")
                             _T("desktop display settings.\n")
                             _T("Press F2 while the app is running to see a\n")
                             _T("list of available devices and modes.") );
            break;

        case D3DAPPERR_NOHALTHISMODE:
            _tcscpy( strMsg, _T("This sample requires functionality that is\n")
                             _T("not available on your Direct3D hardware\n")
                             _T("accelerator with the current desktop display\n")
                             _T("settings.\n")
                             _T("Press F2 while the app is running to see a\n")
                             _T("list of available devices and modes.") );
            break;

        case D3DAPPERR_MEDIANOTFOUND:
            _tcscpy( strMsg, _T("Could not load required media." ) );
            break;

        case D3DAPPERR_RESIZEFAILED:
            _tcscpy( strMsg, _T("Could not reset the Direct3D device." ) );
            break;

        case D3DAPPERR_NONZEROREFCOUNT:
            _tcscpy( strMsg, _T("A D3D object has a non-zero reference\n")
                             _T("count (meaning things were not properly\n")
                             _T("cleaned up).") );
            break;

        case D3DAPPERR_NULLREFDEVICE:
            _tcscpy( strMsg, _T("Warning: Nothing will be rendered.\n")
                             _T("The reference rendering device was selected, but your\n")
                             _T("computer only has a reduced-functionality reference device\n")
                             _T("installed.  Install the DirectX SDK to get the full\n")
                             _T("reference device.\n") );
            break;

        case E_OUTOFMEMORY:
            _tcscpy( strMsg, _T("Not enough memory.") );
            break;

        case D3DERR_OUTOFVIDEOMEMORY:
            _tcscpy( strMsg, _T("Not enough video memory.") );
            break;

        default:
            _tcscpy( strMsg, _T("Generic application error. Enable\n")
                             _T("debug output for detailed information."));
    }

    if( MSGERR_APPMUSTEXIT == dwType )
    {
        _tcscat( strMsg, _T("\n\nAceOnline will now exit.") );
        MessageBox( NULL, strMsg, m_strWindowTitle, MB_ICONERROR|MB_OK );

        // Close the window, which shuts down the app
        if( m_hWnd )
            SendMessage( m_hWnd, WM_CLOSE, 0, 0 );
    }
    else
    {
        if( MSGWARN_SWITCHEDTOREF == dwType )
            _tcscat( strMsg, _T("\n\nSwitching to the reference rasterizer,\n")
                             _T("a software device that implements the entire\n")
                             _T("Direct3D feature set, but runs very slowly.") );
        MessageBox( NULL, strMsg, m_strWindowTitle, MB_ICONWARNING|MB_OK );
    }

    return hr;
}
#ifdef _DEBUG
void DbgOutDeviceCaps(D3DCAPS9 caps)
{
    DBGOUT("-----------Device Info--------------\n");
    DBGOUT("D3DDEVTYPE : %d\n", caps.DeviceType);
    DBGOUT("AdapterOrdinal :%d\n", caps.AdapterOrdinal);

	DBGOUT("-------Caps from DX7 Draw-----------\n");
	DBGOUT("Caps:%08x,Caps2:%08x,Caps3:%08x,PresentationIntervals:%08x\n",
			caps.Caps, caps.Caps2, caps.Caps3, caps.PresentationIntervals );

    /* Cursor Caps */
	DBGOUT("-----------Cursor Caps--------------\n");
    DBGOUT("CursorCaps : %08x\n",caps.CursorCaps);

    /* 3D Device Caps */
	DBGOUT("----------3D Device Caps------------\n");
    DBGOUT("DevCaps : %08x\n",caps.DevCaps);

    DBGOUT("PrimitiveMiscCaps : %08x\n",caps.PrimitiveMiscCaps);
    DBGOUT("RasterCaps : %08x\n",caps.RasterCaps);
    DBGOUT("ZCmpCaps : %08x\n",caps.ZCmpCaps);
    DBGOUT("SrcBlendCaps : %08x\n",caps.SrcBlendCaps);
    DBGOUT("DestBlendCaps : %08x\n",caps.DestBlendCaps);
    DBGOUT("AlphaCmpCaps : %08x\n",caps.AlphaCmpCaps);
    DBGOUT("ShadeCaps : %08x\n",caps.ShadeCaps);
    DBGOUT("TextureCaps : %08x\n",caps.TextureCaps);
    DBGOUT("TextureFilterCaps : %08x\n",caps.TextureFilterCaps);
    DBGOUT("CubeTextureFilterCaps : %08x\n",caps.CubeTextureFilterCaps);
    DBGOUT("VolumeTextureFilterCaps : %08x\n",caps.VolumeTextureFilterCaps);
    DBGOUT("TextureAddressCaps : %08x\n",caps.TextureAddressCaps);
    DBGOUT("VolumeTextureAddressCaps : %08x\n",caps.VolumeTextureAddressCaps);

    DBGOUT("LineCaps : %08x\n",caps.LineCaps);

    DBGOUT("MaxTextureWidth : %d, MaxTextureHeight : %d\n",caps.MaxTextureWidth, caps.MaxTextureHeight);
    DBGOUT("MaxVolumeExtent : %08x\n",caps.MaxVolumeExtent);

    DBGOUT("MaxTextureRepeat : %08x\n",caps.MaxTextureRepeat);
    DBGOUT("MaxTextureAspectRatio : %08x\n",caps.MaxTextureAspectRatio);
    DBGOUT("MaxAnisotropy : %08x\n",caps.MaxAnisotropy);
    DBGOUT("MaxVertexW : %f\n",caps.MaxVertexW);

    DBGOUT("GuardBandLeft : %f\n",caps.GuardBandLeft);
    DBGOUT("GuardBandTop : %f\n",caps.GuardBandTop);
    DBGOUT("GuardBandRight : %f\n",caps.GuardBandRight);
    DBGOUT("GuardBandBottom : %f\n",caps.GuardBandBottom);

    DBGOUT("ExtentsAdjust : %f\n",caps.ExtentsAdjust);
    DBGOUT("StencilCaps : %08x\n",caps.StencilCaps);

    DBGOUT("FVFCaps : %08x\n",caps.FVFCaps);
    DBGOUT("TextureOpCaps : %08x\n",caps.TextureOpCaps);
    DBGOUT("MaxTextureBlendStages : %08x\n",caps.MaxTextureBlendStages);
    DBGOUT("MaxSimultaneousTextures : %08x\n",caps.MaxSimultaneousTextures);

    DBGOUT("VertexProcessingCaps : %08x\n",caps.VertexProcessingCaps);
    DBGOUT("MaxActiveLights : %08x\n",caps.MaxActiveLights);
    DBGOUT("MaxUserClipPlanes : %08x\n",caps.MaxUserClipPlanes);
    DBGOUT("MaxVertexBlendMatrices : %08x\n",caps.MaxVertexBlendMatrices);
    DBGOUT("MaxVertexBlendMatrixIndex : %08x\n",caps.MaxVertexBlendMatrixIndex);

    DBGOUT("MaxPointSize : %f\n",caps.MaxPointSize);

    DBGOUT("MaxPrimitiveCount : %d\n",caps.MaxPrimitiveCount);
    DBGOUT("MaxVertexIndex : %d\n",caps.MaxVertexIndex);
    DBGOUT("MaxStreams : %d\n",caps.MaxStreams);
    DBGOUT("MaxStreamStride : %d\n",caps.MaxStreamStride);

    DBGOUT("VertexShaderVersion : %08x\n",caps.VertexShaderVersion);
    DBGOUT("MaxVertexShaderConst : %08x\n",caps.MaxVertexShaderConst);

    DBGOUT("PixelShaderVersion : %08x\n",caps.PixelShaderVersion);
	// 2005-01-04 by jschoi
 //   DBGOUT("MaxPixelShaderValue : %f\n",caps.MaxPixelShaderValue);
	DBGOUT("-----------------------------------\n");

}
#endif
///////////////////////////////////////////////////////////////////////////////
/// \fn			void CD3DApplication::SetAlphaRenderState(int nAlphaValue)
/// \brief		텍스쳐 알파 값 셋팅
/// \author		ispark
/// \date		2006-11-10
///////////////////////////////////////////////////////////////////////////////
void CD3DApplication::SetAlphaRenderState(int nAlphaValue)
{
	g_pD3dDev->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
	g_pD3dDev->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );		
	g_pD3dDev->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
	g_pD3dDev->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_MODULATE); 
	g_pD3dDev->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE); 
	g_pD3dDev->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR); 
	g_pD3dDev->SetRenderState( D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB( nAlphaValue, 255, 255, 255 ) );
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CD3DApplication::InitAlphaRenderState()
/// \brief		
/// \author		ispark
/// \date		2006-11-10 ~ 2006-11-10
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CD3DApplication::InitAlphaRenderState()
{
	g_pD3dDev->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );
	g_pD3dDev->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );		
	g_pD3dDev->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
	g_pD3dDev->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_DISABLE); 
	g_pD3dDev->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE); 
//	g_pD3dDev->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR); 
//	g_pD3dDev->SetRenderState( D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CD3DApplication::IsDeviceLost()
/// \brief		
/// \author		// 2007-04-12 by bhsohn 포커스 잃어 버렸을때의 추가버그 처리
/// \date		2007-04-12 ~ 2007-04-12
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CD3DApplication::IsDeviceLost()
{
	return m_bDeviceLost;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2008-11-06 by bhsohn 마우스 가두기 모드 보완
/// \date		2008-11-06 ~ 2008-11-06
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CD3DApplication::RefreshMouseMode()
{	
	int nSetCursel = FALSE;
	if(g_pD3dApp->IsOptionEtc(OPTION_RADIO_MOUSE_LOCK))
	{
		nSetCursel = TRUE;
	}
	SetClipCursor(nSetCursel);
	
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2008-11-06 by bhsohn 마우스 가두기 모드 보완
/// \date		2008-11-06 ~ 2008-11-06
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CD3DApplication::SetClipCursor(int nSetCursel)
{	
	if(!g_pD3dApp->IsWindowMode())
	{
		// 윈도우 모드가 아니다.
		return;
	}
	//DbgOut("nSetCursel[%d] \n", nSetCursel);
	
	if(!m_nSetCursel 
		&& !nSetCursel)
	{
		return;
	}
	
	if(nSetCursel)
	{
		RECT rcWindow;
		GetWindowRect( m_hWnd, &rcWindow );

		int nWndHeight = g_pD3dApp->GetBackBufferDesc().Height;
		int nCliHeight = g_pD3dApp->GetHeight();
		int nCapHeight = abs(nWndHeight-nCliHeight);

		// 2012-12-05 by mspark, 마우스 가두기 옵션 설정 시 마우스가 좌, 우, 하단 범위를 벗어나던 버그 수정 
		rcWindow.left += WND_MOUSELOCK_PIXEL;
		rcWindow.right -= WND_MOUSELOCK_PIXEL;
		rcWindow.bottom -= WND_MOUSELOCK_PIXEL;
		// end 2012-12-05 by mspark, 마우스 가두기 옵션 설정 시 마우스가 좌, 우, 하단 범위를 벗어나던 버그 수정 

		if(!g_pD3dApp->IsPossibleWindowMove())
		{
			//DbgOut("g_pD3dApp->IsPossibleWindowMove\n");
			rcWindow.top += (nCapHeight);
		}
		if((m_nSetCursel == nSetCursel)
			&&(m_rcClipRect.left == rcWindow.left)
			&&(m_rcClipRect.top == rcWindow.top)
			&&(m_rcClipRect.right == rcWindow.right)
			&&(m_rcClipRect.bottom == rcWindow.bottom))
		{
			// 기존하고 바뀐게 없다.
		}
		else
		{			
			ClipCursor( &rcWindow );
		}		
		m_rcClipRect = rcWindow;
	}
	else
	{
		ClipCursor( NULL );
	}

	m_nSetCursel = nSetCursel;
	
}