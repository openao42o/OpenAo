// SpaceCowboyDBEditorTool.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "SpaceCowboyDBEditorTool.h"
#include "SpaceCowboyDBEditorToolDlg.h"
#include "WinSocket.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSpaceCowboyDBEditorToolApp

BEGIN_MESSAGE_MAP(CSpaceCowboyDBEditorToolApp, CWinApp)
	//{{AFX_MSG_MAP(CSpaceCowboyDBEditorToolApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSpaceCowboyDBEditorToolApp construction

CSpaceCowboyDBEditorToolApp::CSpaceCowboyDBEditorToolApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CSpaceCowboyDBEditorToolApp object

CSpaceCowboyDBEditorToolApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CSpaceCowboyDBEditorToolApp initialization

BOOL CSpaceCowboyDBEditorToolApp::InitInstance()
{
	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

	CWinSocket::SocketInit();

	CSpaceCowboyDBEditorToolDlg dlg;
	m_pMainWnd = &dlg;

	///////////////////////////////////////////////////////////////////////////////
	// 2007-10-24 by cmkwon, 서버 정보 암호화 - 암호화 되어 있는 데이터를 복호화 한다
	GDecryptGameServerInfoByXOR();

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

int CSpaceCowboyDBEditorToolApp::ExitInstance() 
{
	// TODO: Add your specialized code here and/or call the base class

	CWinSocket::SocketClean();

	return CWinApp::ExitInstance();
}
