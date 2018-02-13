// DlgGuardGgreement.cpp : implementation file
//

#include "stdafx.h"
#include "atumlauncher.h"
#include "DlgGuardAgreement.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgGuardAgreement dialog


CDlgGuardAgreement::CDlgGuardAgreement(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgGuardAgreement::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgGuardAgreement)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pGuardAgreementHost		= NULL;
}


void CDlgGuardAgreement::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgGuardAgreement)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgGuardAgreement, CDialog)
	//{{AFX_MSG_MAP(CDlgGuardAgreement)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgGuardAgreement message handlers

BOOL CDlgGuardAgreement::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	

	///////////////////////////////////////////////////////////////////////////////
	// 2009-08-31 by cmkwon, Gameforge4D 게임가드 동의창 띄우기 - 
	GetDlgItem(IDOK)->SetWindowText(STRMSG_090831_0002);
	GetDlgItem(IDCANCEL)->SetWindowText(STRMSG_090831_0003);

	RECT rtMainWnd;
	this->GetWindowRect(&rtMainWnd);
	RECT rtOKBtnWnd;
	GetDlgItem(IDOK)->GetWindowRect(&rtOKBtnWnd);

	const int nMargin = 0;
	RECT rtWebWnd;
	rtWebWnd.left		= rtMainWnd.left + nMargin;
	rtWebWnd.top		= rtMainWnd.top + nMargin;
	rtWebWnd.right		= rtMainWnd.right - 12 - nMargin;
	rtWebWnd.bottom		= rtOKBtnWnd.top - 12 - nMargin;

	m_pGuardAgreementHost = new Host(m_hWnd, STRMSG_090902_0001, NULL, NULL, NULL, &rtWebWnd);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgGuardAgreement::OnCancel() 
{
	// TODO: Add extra cleanup here

	m_pGuardAgreementHost->Release();
	m_pGuardAgreementHost = NULL;

	CDialog::OnCancel();
}
