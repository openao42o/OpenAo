// LoginSuccessDlg.cpp : implementation file
//

#include "stdafx.h"
#include "AtumLauncher.h"
#include "LoginSuccessDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLoginSuccessDlg dialog


CLoginSuccessDlg::CLoginSuccessDlg(CString cmdLine, CWnd* pParent /*=NULL*/)
	: CDialog(CLoginSuccessDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLoginSuccessDlg)
	m_CmdLine = cmdLine;
	//}}AFX_DATA_INIT
}


void CLoginSuccessDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLoginSuccessDlg)
	DDX_Text(pDX, IDC_EDIT_CMD_LINE, m_CmdLine);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLoginSuccessDlg, CDialog)
	//{{AFX_MSG_MAP(CLoginSuccessDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLoginSuccessDlg message handlers
