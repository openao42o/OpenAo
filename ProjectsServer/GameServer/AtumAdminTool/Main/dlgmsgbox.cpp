// dlgmsgbox.cpp : implementation file
//

#include "stdafx.h"
#include "..\atumadmintool.h"
#include "dlgmsgbox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgMsgBox dialog


CDlgMsgBox::CDlgMsgBox(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgMsgBox::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgMsgBox)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_bIsForMainServer		= TRUE;
}


void CDlgMsgBox::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgMsgBox)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgMsgBox, CDialog)
	//{{AFX_MSG_MAP(CDlgMsgBox)
	ON_BN_CLICKED(IDC_BTN_FOR_TESTSERVER, OnBtnForTestserver)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgMsgBox message handlers

void CDlgMsgBox::OnBtnForTestserver() 
{
	// TODO: Add your control notification handler code here

	m_bIsForMainServer		= FALSE;		// 2009-11-24 by cmkwon, 월드랭킹 실행시 질문 메시지박스 수정 - 
	
	CDialog::OnOK();	
}

void CDlgMsgBox::OnOK() 
{
	// TODO: Add extra validation here
	
	m_bIsForMainServer		= TRUE;			// 2009-11-24 by cmkwon, 월드랭킹 실행시 질문 메시지박스 수정 - 


	CDialog::OnOK();
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2009-11-24 by cmkwon, 월드랭킹 실행시 질문 메시지박스 수정 - 
/// \author		cmkwon
/// \date		2009-11-24 ~ 2009-11-24
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CDlgMsgBox::GetIsForMainServer(void)
{
	return m_bIsForMainServer;
}

BOOL CDlgMsgBox::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	// 2009-11-24 by cmkwon, 월드랭킹 실행시 질문 메시지박스 수정 - 
	GetDlgItem(IDC_STATIC_QUESTION)->SetWindowText("Do you excute WRM(World Ranking Management)?");
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
