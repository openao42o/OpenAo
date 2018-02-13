// PWDDlg.cpp : implementation file
//

#include "stdafx.h"
#include "atumadmintool.h"
#include "PWDDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPWDDlg dialog


CPWDDlg::CPWDDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPWDDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPWDDlg)
	m_ctlStrPassword = _T("");
	//}}AFX_DATA_INIT
}


void CPWDDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPWDDlg)
	DDX_Text(pDX, IDC_EDIT_PWD, m_ctlStrPassword);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPWDDlg, CDialog)
	//{{AFX_MSG_MAP(CPWDDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPWDDlg message handlers
