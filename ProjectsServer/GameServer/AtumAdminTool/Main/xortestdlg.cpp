// xortestdlg.cpp : implementation file
//

#include "stdafx.h"
#include "atumadmintool.h"
#include "xortestdlg.h"
#include "XOR_Lib.h"			// 2007-10-24 by cmkwon, 서버 정보 암호화 - 헤더파일 추가

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXORTestDlg dialog


CXORTestDlg::CXORTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CXORTestDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CXORTestDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CXORTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CXORTestDlg)
	DDX_Control(pDX, IDC_EDIT_SOURCE_STRING, m_ctlEditSourceString);
	DDX_Control(pDX, IDC_EDIT_KEY_STRING, m_ctlEditKeyString);
	DDX_Control(pDX, IDC_EDIT_ENCOEDED_STRING, m_ctlEditEncodedString);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CXORTestDlg, CDialog)
	//{{AFX_MSG_MAP(CXORTestDlg)
	ON_BN_CLICKED(IDC_BTN_XOR_ENCODE, OnBtnXorEncode)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXORTestDlg message handlers

void CXORTestDlg::OnBtnXorEncode() 
{
	// TODO: Add your control notification handler code here

	m_ctlEditEncodedString.SetWindowText("");	// 초기화

	char szSourceString[1024];
	util::zero(szSourceString, 1024);

	m_ctlEditSourceString.GetWindowText(szSourceString, 1024);
	int nSourceLen = strlen(szSourceString);
	if(0 >= nSourceLen)
	{
		MessageBox("Input Source String !!");
		return;
	}

	char szKeyString[1024];
	util::zero(szKeyString, 1024);
	m_ctlEditKeyString.GetWindowText(szKeyString, 1024);
	if(0 >= strlen(szKeyString))
	{
		MessageBox("Input Key String !!");
		return;
	}

	BYTE byEncodedBinary[1024];
	util::zero(byEncodedBinary, 1024);
	if(FALSE == XOR::XOREncode(byEncodedBinary, szSourceString, szKeyString))
	{
		MessageBox("SC Encrytion encode failure !!");
		return;
	}

	char szEncodedString[1024];
	util::zero(szEncodedString, 1024);
	if(FALSE == XOR::XORBinary2String(szEncodedString, byEncodedBinary, nSourceLen))
	{
		MessageBox("SC Encrytion encode failure !!");
		return;
	}

	m_ctlEditEncodedString.SetWindowText(szEncodedString);	
}
