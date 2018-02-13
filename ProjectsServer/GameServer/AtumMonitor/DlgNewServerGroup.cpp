// DlgNewServerGroup.cpp : implementation file
//

#include "stdafx.h"
#include "AtumMonitor.h"
#include "DlgNewServerGroup.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgNewServerGroup dialog


CDlgNewServerGroup::CDlgNewServerGroup(BOOL bNewServerGroup, CString csName, CWnd* pParent /*=NULL*/)
	: CDialog(CDlgNewServerGroup::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgNewServerGroup)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_bNewServerGroup = bNewServerGroup;
	m_csBeforeServerGroupName = csName;
}


void CDlgNewServerGroup::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgNewServerGroup)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgNewServerGroup, CDialog)
	//{{AFX_MSG_MAP(CDlgNewServerGroup)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgNewServerGroup message handlers

void CDlgNewServerGroup::OnOK() 
{
	// TODO: Add extra validation here
	GetDlgItem(IDC_EDIT_GROUPNAME)->GetWindowText(m_csNewServerGroupName);
	CDialog::OnOK();
}

BOOL CDlgNewServerGroup::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if(FALSE == m_bNewServerGroup)
	{
		this->SetWindowText("Rename Server Group");	
		GetDlgItem(IDC_EDIT_GROUPNAME)->SetWindowText(m_csBeforeServerGroupName);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
