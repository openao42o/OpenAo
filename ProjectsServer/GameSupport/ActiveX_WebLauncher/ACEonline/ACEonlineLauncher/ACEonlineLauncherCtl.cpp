// ACEonlineLauncherCtl.cpp : Implementation of the CACEonlineLauncherCtrl ActiveX Control class.

#include "stdafx.h"
#include "ACEonlineLauncher.h"
#include "ACEonlineLauncherCtl.h"
#include "ACEonlineLauncherPpg.h"
#include "DbgOut_C.h"	// 2012-08-13 by mspark, 에이스 온라인 ActiveX 작업


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CACEonlineLauncherCtrl, COleControl)


/////////////////////////////////////////////////////////////////////////////
// Message map

BEGIN_MESSAGE_MAP(CACEonlineLauncherCtrl, COleControl)
	//{{AFX_MSG_MAP(CACEonlineLauncherCtrl)
	// NOTE - ClassWizard will add and remove message map entries
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG_MAP
	ON_OLEVERB(AFX_IDS_VERB_PROPERTIES, OnProperties)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Dispatch map

BEGIN_DISPATCH_MAP(CACEonlineLauncherCtrl, COleControl)
	//{{AFX_DISPATCH_MAP(CACEonlineLauncherCtrl)
	// NOTE - ClassWizard will add and remove dispatch map entries
	//    DO NOT EDIT what you see in these blocks of generated code !
	// 2012-08-13 by mspark, 에이스 온라인 ActiveX 작업
	DISP_FUNCTION(CACEonlineLauncherCtrl, "SetGameArgument", SetGameArgument, 
				VT_I2, VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR)
	// end 2012-08-13 by mspark, 에이스 온라인 ActiveX 작업
	//}}AFX_DISPATCH_MAP
	DISP_FUNCTION_ID(CACEonlineLauncherCtrl, "AboutBox", DISPID_ABOUTBOX, AboutBox, VT_EMPTY, VTS_NONE)
END_DISPATCH_MAP()


/////////////////////////////////////////////////////////////////////////////
// Event map

BEGIN_EVENT_MAP(CACEonlineLauncherCtrl, COleControl)
	//{{AFX_EVENT_MAP(CACEonlineLauncherCtrl)
	// NOTE - ClassWizard will add and remove event map entries
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_EVENT_MAP
END_EVENT_MAP()


/////////////////////////////////////////////////////////////////////////////
// Property pages

// TODO: Add more property pages as needed.  Remember to increase the count!
BEGIN_PROPPAGEIDS(CACEonlineLauncherCtrl, 1)
	PROPPAGEID(CACEonlineLauncherPropPage::guid)
END_PROPPAGEIDS(CACEonlineLauncherCtrl)


/////////////////////////////////////////////////////////////////////////////
// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CACEonlineLauncherCtrl, "ACEONLINELAUNCHER.ACEonlineLauncherCtrl.1",
	0xff05edc4, 0xfad0, 0x48a4, 0xaa, 0x30, 0xe0, 0xcb, 0x82, 0x5a, 0x93, 0xc4)


/////////////////////////////////////////////////////////////////////////////
// Type library ID and version

IMPLEMENT_OLETYPELIB(CACEonlineLauncherCtrl, _tlid, _wVerMajor, _wVerMinor)


/////////////////////////////////////////////////////////////////////////////
// Interface IDs

const IID BASED_CODE IID_DACEonlineLauncher =
		{ 0x641fcd62, 0xa3a, 0x41b6, { 0x95, 0xf7, 0x63, 0x69, 0xb2, 0xd2, 0xbb, 0x13 } };
const IID BASED_CODE IID_DACEonlineLauncherEvents =
		{ 0x787821bb, 0x2f68, 0x4bdf, { 0x87, 0x7, 0x2f, 0x36, 0x3a, 0x5e, 0xbd, 0xf4 } };


/////////////////////////////////////////////////////////////////////////////
// Control type information

static const DWORD BASED_CODE _dwACEonlineLauncherOleMisc =
	OLEMISC_ACTIVATEWHENVISIBLE |
	OLEMISC_SETCLIENTSITEFIRST |
	OLEMISC_INSIDEOUT |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_RECOMPOSEONRESIZE;

IMPLEMENT_OLECTLTYPE(CACEonlineLauncherCtrl, IDS_ACEONLINELAUNCHER, _dwACEonlineLauncherOleMisc)

// 2012-08-13 by mspark, 에이스 온라인 ActiveX 작업
#define GAME_PATH			"SOFTWARE\\ACEonline"
#define GAME_REG_VAL		"ExecutePath"
#define GAME_EXEC			"ACEonline.exe"
// end 2012-08-13 by mspark, 에이스 온라인 ActiveX 작업


/////////////////////////////////////////////////////////////////////////////
// CACEonlineLauncherCtrl::CACEonlineLauncherCtrlFactory::UpdateRegistry -
// Adds or removes system registry entries for CACEonlineLauncherCtrl

BOOL CACEonlineLauncherCtrl::CACEonlineLauncherCtrlFactory::UpdateRegistry(BOOL bRegister)
{
	// TODO: Verify that your control follows apartment-model threading rules.
	// Refer to MFC TechNote 64 for more information.
	// If your control does not conform to the apartment-model rules, then
	// you must modify the code below, changing the 6th parameter from
	// afxRegApartmentThreading to 0.

	if (bRegister)
		return AfxOleRegisterControlClass(
			AfxGetInstanceHandle(),
			m_clsid,
			m_lpszProgID,
			IDS_ACEONLINELAUNCHER,
			IDB_ACEONLINELAUNCHER,
			afxRegApartmentThreading,
			_dwACEonlineLauncherOleMisc,
			_tlid,
			_wVerMajor,
			_wVerMinor);
	else
		return AfxOleUnregisterClass(m_clsid, m_lpszProgID);
}


/////////////////////////////////////////////////////////////////////////////
// CACEonlineLauncherCtrl::CACEonlineLauncherCtrl - Constructor

CACEonlineLauncherCtrl::CACEonlineLauncherCtrl()
{
	InitializeIIDs(&IID_DACEonlineLauncher, &IID_DACEonlineLauncherEvents);

	// TODO: Initialize your control's instance data here.
	// 2012-08-13 by mspark, 에이스 온라인 ActiveX 작업
	m_strACEonlineInstallPath = "";
	// end 2012-08-13 by mspark, 에이스 온라인 ActiveX 작업

	// 2012-08-13 by mspark, 에이스 온라인 ActiveX 작업 - 뮤텍스 작업
	m_hMutexMonoInstance = NULL;
	// end 2012-08-13 by mspark, 에이스 온라인 ActiveX 작업 - 뮤텍스 작업
}


/////////////////////////////////////////////////////////////////////////////
// CACEonlineLauncherCtrl::~CACEonlineLauncherCtrl - Destructor

CACEonlineLauncherCtrl::~CACEonlineLauncherCtrl()
{
	// TODO: Cleanup your control's instance data here.
	// 2012-08-13 by mspark, 에이스 온라인 ActiveX 작업 - 뮤텍스 작업
	if(m_hMutexMonoInstance)
	{
		::CloseHandle(m_hMutexMonoInstance);
		m_hMutexMonoInstance = NULL;
	}
	// end 2012-08-13 by mspark, 에이스 온라인 ActiveX 작업 - 뮤텍스 작업
}


/////////////////////////////////////////////////////////////////////////////
// CACEonlineLauncherCtrl::OnDraw - Drawing function

void CACEonlineLauncherCtrl::OnDraw(
			CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid)
{
	// TODO: Replace the following code with your own drawing code.
	//pdc->FillRect(rcBounds, CBrush::FromHandle((HBRUSH)GetStockObject(WHITE_BRUSH)));
	//pdc->Ellipse(rcBounds);
}


/////////////////////////////////////////////////////////////////////////////
// CACEonlineLauncherCtrl::DoPropExchange - Persistence support

void CACEonlineLauncherCtrl::DoPropExchange(CPropExchange* pPX)
{
	ExchangeVersion(pPX, MAKELONG(_wVerMinor, _wVerMajor));
	COleControl::DoPropExchange(pPX);

	// TODO: Call PX_ functions for each persistent custom property.

}


/////////////////////////////////////////////////////////////////////////////
// CACEonlineLauncherCtrl::OnResetState - Reset control to default state

void CACEonlineLauncherCtrl::OnResetState()
{
	COleControl::OnResetState();  // Resets defaults found in DoPropExchange

	// TODO: Reset any other control state here.
}


/////////////////////////////////////////////////////////////////////////////
// CACEonlineLauncherCtrl::AboutBox - Display an "About" box to the user

void CACEonlineLauncherCtrl::AboutBox()
{
	CDialog dlgAbout(IDD_ABOUTBOX_ACEONLINELAUNCHER);
	dlgAbout.DoModal();
}


/////////////////////////////////////////////////////////////////////////////
// CACEonlineLauncherCtrl message handlers

// 2012-08-13 by mspark, 에이스 온라인 ActiveX 작업
short CACEonlineLauncherCtrl::SetGameArgument(LPCTSTR szGameId, LPCTSTR szGamePassWord, LPCTSTR szSeed, LPCTSTR szGameType, LPCTSTR szAuthenticationKey, LPCTSTR szBirthday) 
{
	// TODO: Add your dispatch handler code here
	// 2012-08-13 by mspark, 에이스 온라인 ActiveX 작업 - 뮤텍스 작업
	char szEncryptMutexName[1024];
	sprintf(szEncryptMutexName, "%s%s", szGamePassWord, szSeed);
	m_hMutexMonoInstance = CreateMutex(NULL, TRUE, szEncryptMutexName);
	if(NULL == m_hMutexMonoInstance)
	{
		MessageBox("CreateMutex Error", "ACEonlineLauncher", MB_OK);
		return FALSE;
	}
	// end 2012-08-13 by mspark, 에이스 온라인 ActiveX 작업 - 뮤텍스 작업

	m_strACEonlineInstallPath.Format("%s", GAME_PATH);
	
	DWORD dwReturn = 0;
	HKEY hKey = NULL;
	DWORD dwType = REG_SZ;
	char cACEonlineInstallFullNameBuffer[256];
	DWORD dwBufferSize = sizeof(cACEonlineInstallFullNameBuffer);
	
	dwReturn = ::RegOpenKeyEx(HKEY_LOCAL_MACHINE,_T(m_strACEonlineInstallPath),0,KEY_READ,&hKey);
	
	if(dwReturn == ERROR_SUCCESS)
	{		
		dwReturn = ::RegQueryValueEx(hKey, TEXT(GAME_REG_VAL), NULL, &dwType, (LPBYTE)cACEonlineInstallFullNameBuffer, &dwBufferSize );
		
		::RegCloseKey(hKey);
	}
	else
	{
		return FALSE;
	}

	{		
		char szParameter[MAX_PATH];
		memset(szParameter, 0x00, MAX_PATH);
		
		wsprintf(szParameter,"%s %s %s %s %s %s", 
			szGameId, szGamePassWord, szSeed, szGameType, szAuthenticationKey, szBirthday);			
		
		SHELLEXECUTEINFO sExecInfo;		
		memset(&sExecInfo, 0x00, sizeof(SHELLEXECUTEINFO));
		
		sExecInfo.cbSize		= sizeof(SHELLEXECUTEINFO);
		sExecInfo.hwnd			= AfxGetMainWnd()->GetSafeHwnd();
		sExecInfo.lpFile		= GAME_EXEC;
		sExecInfo.lpParameters	= szParameter;
		sExecInfo.lpDirectory	= (char*)cACEonlineInstallFullNameBuffer;
		sExecInfo.hProcess		= NULL;
		sExecInfo.nShow			= SW_SHOWNORMAL;
		sExecInfo.fMask			= SEE_MASK_FLAG_DDEWAIT|SEE_MASK_FLAG_NO_UI;
		
		OSVERSIONINFO osv;
		
		// 운영체제 버전 조사
		osv.dwOSVersionInfoSize=sizeof(OSVERSIONINFO);
		GetVersionEx(&osv);
		
		if(osv.dwMajorVersion  <= 5)
		{
			// 비스타, 7.0이상
			sExecInfo.lpVerb		= "open";
		}
		else
		{
			// 비스타, 윈도우7
			sExecInfo.lpVerb		= "runas";
		}
		
		
		if(FALSE == ShellExecuteEx(&sExecInfo))
		{
			DbgOut("ShellExecuteEx FALSE[%s\\%s]\n", (char*)cACEonlineInstallFullNameBuffer, GAME_EXEC);
			return 0;
		}
	}
	
	return 1;
}
// end 2012-08-13 by mspark, 에이스 온라인 ActiveX 작업