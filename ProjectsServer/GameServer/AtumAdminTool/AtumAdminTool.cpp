// AtumAdminTool.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "AtumAdminTool.h"
#include "AtumAdminToolDlg.h"
#include "VMemPool.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAtumAdminToolApp

BEGIN_MESSAGE_MAP(CAtumAdminToolApp, CWinApp)
	//{{AFX_MSG_MAP(CAtumAdminToolApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAtumAdminToolApp construction

CAtumAdminToolApp::CAtumAdminToolApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CAtumAdminToolApp object

CAtumAdminToolApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CAtumAdminToolApp initialization

BOOL CAtumAdminToolApp::InitInstance()
{
	AfxEnableControlContainer();

	///////////////////////////////////////////////////////////////////////////////
	// 2006-04-10 by cmkwon, Initialize winsock 2.0
	CWinSocket::SocketInit();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	VMemPool::vmPoolAddObject(sizeof(CSendPacket), 1000);
	VMemPool::vmPoolAddObject(sizeof(CRecvPacket), 100);	

	if(VMemPool::vmPoolInit() == FALSE)
	{
		return FALSE;
	}

	SetRegistryKey(STRMSG_REG_STRING_REGISTRYKEY_NAME);

// 2008-06-12 by cmkwon, 예당 에서도 접속 가능하게 수정 하면서 주석 처리 함.
// #ifdef _DEBUG		// 2006-04-10 by cmkwon, 디버그 모드는 마상소프트에서만 실행가능
// 	///////////////////////////////////////////////////////////////////////////////
// 	// 2006-04-10 by cmkwon, 실행 체크
// 	char szLocalIP[SIZE_MAX_IPADDRESS];
// 	util::zero(szLocalIP, SIZE_MAX_IPADDRESS);
// 	this->GetPublicLocalIP(szLocalIP);
// 	if(FALSE == IS_MASANG_IP(szLocalIP))
// 	{
// 		AfxMessageBox("Your computer can't excute SCAdminTool by your ip address!!" );
// 		return FALSE;
// 	}
// #endif// End_#ifdef _DEBUG

	///////////////////////////////////////////////////////////////////////////////
	// 2007-10-24 by cmkwon, 서버 정보 암호화 - 암호화 되어 있는 데이터를 복호화 한다
	GDecryptGameServerInfoByXOR();

	CAtumAdminToolDlg dlg;
	m_pMainWnd = &dlg;
	ReadProfile();
	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

void CAtumAdminToolApp::WriteProfile()
{
	// registry path: HKEY_CURRENT_USER\Software\SpaceCowboy\AtumAdminTool\Configuration
	CAtumAdminToolDlg *pDlg = (CAtumAdminToolDlg*)AfxGetMainWnd();

	if(pDlg)
	{
#ifndef _ATUM_ADMIN_RELEASE
		WriteProfileString(_T("Configuration"), _T("UID"), pDlg->m_UID);
		WriteProfileString(_T("Configuration"), _T("PWD"), pDlg->m_PWD);
#endif// _ATUM_ADMIN_RELEASE_end_ifndef
		WriteProfileString(_T("Configuration"), _T("Server"), pDlg->m_szServerName);

		// CSCUserAdminToolDlg
		WriteProfileString(_T("Configuration"), _T("AccountNameInput"), m_szAccountNameInputReg);
	}
}

void CAtumAdminToolApp::ReadProfile()
{
	// registry path: HKEY_CURRENT_USER\Software\Atum Online\AtumAdminTool\Configuration
	CAtumAdminToolDlg *pDlg = (CAtumAdminToolDlg*)AfxGetMainWnd();

	if(pDlg)
	{
#ifndef _ATUM_ADMIN_RELEASE
		pDlg->m_UID = GetProfileString( _T("Configuration"), _T("UID"), "");
		pDlg->m_PWD = GetProfileString( _T("Configuration"), _T("PWD"), "");
#endif// _ATUM_ADMIN_RELEASE_end_ifndef
		pDlg->m_szServerName = GetProfileString( _T("Configuration"), _T("Server"), "");

		// CSCUserAdminToolDlg
		m_szAccountNameInputReg = GetProfileString( _T("Configuration"), _T("AccountNameInput"), "");
	}
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			char *CAtumAdminToolApp::GetPublicLocalIP(char *o_szLocalIP)
/// \brief		
/// \author		cmkwon
/// \date		2006-04-10 ~ 2006-04-10
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
char *CAtumAdminToolApp::GetPublicLocalIP(char *o_szLocalIP)
{
	GGetLocalIP(o_szLocalIP, IP_TYPE_PUBLIC);
	return o_szLocalIP;
}