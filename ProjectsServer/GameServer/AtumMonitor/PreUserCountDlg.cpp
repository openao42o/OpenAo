// PreUserCountDlg.cpp : implementation file
//

#include "stdafx.h"
#include "AtumMonitor.h"
#include "PreUserCountDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CPreUserCountDlg dialog


CPreUserCountDlg::CPreUserCountDlg(int nDlgType, UINT nLimitCount, CWnd* pParent /*=NULL*/)
	: CDialog(CPreUserCountDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPreUserCountDlg)
	m_ctl_uiLimitGroupUserCounts = nLimitCount;
	//}}AFX_DATA_INIT
}


void CPreUserCountDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPreUserCountDlg)
	DDX_Text(pDX, IDC_EDIT_USER_COUNT, m_ctl_uiLimitGroupUserCounts);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPreUserCountDlg, CDialog)
	//{{AFX_MSG_MAP(CPreUserCountDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPreUserCountDlg message handlers

BOOL CPreUserCountDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPreUserCountDlg::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(TRUE);
	
	CDialog::OnOK();
}
