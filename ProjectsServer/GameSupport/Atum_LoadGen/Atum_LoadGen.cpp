// Atum_LoadGen.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "Atum_LoadGen.h"
#include "Atum_LoadGenDlg.h"
#include "Mmsystem.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define REG1_NAME_MAIN							"Configuration"

#define REG2_NAME_RECONNECT_TIME				"Reconnect Time"
#define REG2_NAME_SEND_PRI_ATTACK				"Primary weapon attack flag"
#define REG2_NAME_SEND_SEC_ATTACK				"Secondary weapon attack flag"
#define REG2_NAME_PRI_REATTACK_TIME				"Primary weapon reattack time"
#define REG2_NAME_SEC_REATTACK_TIME				"Secondary weapon reattack time"

/////////////////////////////////////////////////////////////////////////////
// CAtum_LoadGenApp

BEGIN_MESSAGE_MAP(CAtum_LoadGenApp, CWinApp)
	//{{AFX_MSG_MAP(CAtum_LoadGenApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAtum_LoadGenApp construction

CAtum_LoadGenApp::CAtum_LoadGenApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CAtum_LoadGenApp object

CAtum_LoadGenApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CAtum_LoadGenApp initialization

BOOL CAtum_LoadGenApp::InitInstance()
{
	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif
	
	CWinSocket::SocketInit();
	srand(timeGetTime());
	// todo : deprecate
	random::init();

	CAtum_LoadGenDlg dlg;
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

void CAtum_LoadGenApp::WriteProfile()
{
	CAtum_LoadGenDlg *pDlg = (CAtum_LoadGenDlg*)AfxGetMainWnd();

	if(pDlg)
	{
		WriteProfileString(_T(REG1_NAME_MAIN), _T("Field Server IP"), pDlg->m_ctl_strFieldServerIP);
		WriteProfileInt( _T(REG1_NAME_MAIN), _T("Field Server Port"), pDlg->m_ctl_uiFieldServerPort);
		WriteProfileInt( _T(REG1_NAME_MAIN), _T("Client Count"), pDlg->m_ctl_uiClientCount);
		WriteProfileInt( _T(REG1_NAME_MAIN), _T(REG2_NAME_RECONNECT_TIME), pDlg->m_ctl_uiReconnectTime);
		WriteProfileString(_T(REG1_NAME_MAIN), _T("Character Name"), pDlg->m_ctl_strCharacterName);
		WriteProfileInt( _T(REG1_NAME_MAIN), _T("Character Name Start Index"), pDlg->m_ctl_uiCharacterNameStartIndex);
		WriteProfileInt( _T(REG1_NAME_MAIN), _T("Character Name End Index"), pDlg->m_ctl_uiCharacterNameEndIndex);
		WriteProfileInt( _T(REG1_NAME_MAIN), _T("X Start"), pDlg->m_ctl_nX_Start);
		WriteProfileInt( _T(REG1_NAME_MAIN), _T("X End"), pDlg->m_ctl_nX_End);		
		WriteProfileInt( _T(REG1_NAME_MAIN), _T("Y"), pDlg->m_ctl_nY);
		WriteProfileInt( _T(REG1_NAME_MAIN), _T("Z Start"), pDlg->m_ctl_nZ_Start);
		WriteProfileInt( _T(REG1_NAME_MAIN), _T("Z End"), pDlg->m_ctl_nZ_End);
		WriteProfileInt( _T(REG1_NAME_MAIN), _T("Speed"), pDlg->m_ctl_uiSpeed);
		WriteProfileInt( _T(REG1_NAME_MAIN), _T("Send Move Count per Second"), pDlg->m_ctl_uiSendMoveCount);
		WriteProfileInt( _T(REG1_NAME_MAIN), _T(REG2_NAME_SEND_PRI_ATTACK), pDlg->m_ctl_bSendAttackFlag);
		WriteProfileInt( _T(REG1_NAME_MAIN), _T(REG2_NAME_SEND_SEC_ATTACK), pDlg->m_ctl_bSendAttackSec);
		WriteProfileInt( _T(REG1_NAME_MAIN), _T(REG2_NAME_PRI_REATTACK_TIME), pDlg->m_ctl_nPriReattakTime);
		WriteProfileInt( _T(REG1_NAME_MAIN), _T(REG2_NAME_SEC_REATTACK_TIME), pDlg->m_ctl_nSecReattakTime);
	}	
}

void CAtum_LoadGenApp::ReadProfile()
{
	CAtum_LoadGenDlg *pDlg = (CAtum_LoadGenDlg*)AfxGetMainWnd();

	if(pDlg)
	{
		pDlg->m_ctl_strFieldServerIP			=  GetProfileString(_T(REG1_NAME_MAIN), _T("Field Server IP"), "61.39.170.140");	// 2009-12-28 by cmkwon, 마상회사 IP 변경 - 기존(121.134.114.)
		pDlg->m_ctl_uiFieldServerPort			= GetProfileInt( _T(REG1_NAME_MAIN), _T("Field Server Port"), 15103);
		pDlg->m_ctl_uiClientCount				= GetProfileInt( _T(REG1_NAME_MAIN), _T("Client Count"), 50);
		pDlg->m_ctl_uiReconnectTime				= GetProfileInt( _T(REG1_NAME_MAIN), _T(REG2_NAME_RECONNECT_TIME), 20);
		pDlg->m_ctl_strCharacterName			= GetProfileString(_T(REG1_NAME_MAIN), _T("Character Name"), ATUM_LOAD_TEST_PREFIX_ACCOUNT_NAME);
		pDlg->m_ctl_uiCharacterNameStartIndex	= GetProfileInt( _T(REG1_NAME_MAIN), _T("Character Name Start Index"), 0);
		pDlg->m_ctl_uiCharacterNameEndIndex		= GetProfileInt( _T(REG1_NAME_MAIN), _T("Character Name End Index"), pDlg->m_ctl_uiCharacterNameStartIndex + pDlg->m_ctl_uiClientCount);
		pDlg->m_ctl_nX_Start					= GetProfileInt( _T(REG1_NAME_MAIN), _T("X Start"), 200);
		pDlg->m_ctl_nX_End						= GetProfileInt( _T(REG1_NAME_MAIN), _T("X End"), 9000);
		pDlg->m_ctl_nY							= GetProfileInt( _T(REG1_NAME_MAIN), _T("Y"), 50);
		pDlg->m_ctl_nZ_Start					= GetProfileInt( _T(REG1_NAME_MAIN), _T("Z Start"), 200);
		pDlg->m_ctl_nZ_End						= GetProfileInt( _T(REG1_NAME_MAIN), _T("Z End"), 9000);
		pDlg->m_ctl_uiSpeed						= GetProfileInt( _T(REG1_NAME_MAIN), _T("Speed"), 5);
		pDlg->m_ctl_uiSendMoveCount				= GetProfileInt( _T(REG1_NAME_MAIN), _T("Send Move Count per Second"), 5);
		pDlg->m_ctl_bSendAttackFlag				= GetProfileInt( _T(REG1_NAME_MAIN), _T(REG2_NAME_SEND_PRI_ATTACK), TRUE);
		pDlg->m_ctl_bSendAttackSec				= GetProfileInt( _T(REG1_NAME_MAIN), _T(REG2_NAME_SEND_SEC_ATTACK), TRUE);
		pDlg->m_ctl_nPriReattakTime				= GetProfileInt( _T(REG1_NAME_MAIN), _T(REG2_NAME_PRI_REATTACK_TIME), 1000);
		pDlg->m_ctl_nSecReattakTime				= GetProfileInt( _T(REG1_NAME_MAIN), _T(REG2_NAME_SEC_REATTACK_TIME), 2000);
	}	
}

int CAtum_LoadGenApp::ExitInstance() 
{
	// TODO: Add your specialized code here and/or call the base class

	CWinSocket::SocketClean();
	
	return CWinApp::ExitInstance();
}
