// FieldServerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "AtumMonitor.h"
#include "FieldServerDlg.h"
#include "AtumProtocol.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFieldServerDlg dialog


CFieldServerDlg::CFieldServerDlg(const char* ip /* = NULL */, LPCSTR i_szIPHistory /* = NULL */, int port /* = 0 */, CWnd* pParent /* = NULL */)
	: CDialog(CFieldServerDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFieldServerDlg)
	m_port = 6003;
	m_ipaddr = _T("");
	//}}AFX_DATA_INIT
	if (ip == NULL && port == 0)
	{
		m_ipaddr = _T("");
		m_port = FIELD_SERVER_PORT;
	}
	else
	{
		m_ipaddr = _T(ip);
		m_port = port;
	}

	m_szIPHistory = i_szIPHistory;
}

CFieldServerDlg::~CFieldServerDlg()
{
}

void CFieldServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFieldServerDlg)
	DDX_Control(pDX, IDC_COMBO_SELECT_SERVER_IP, m_HistoryComboIPAddress);
	DDX_Text(pDX, IDC_COMBO_SELECT_SERVER_IP, m_ipaddr);
	DDX_Text(pDX, IDC_EDIT2, m_port);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFieldServerDlg, CDialog)
	//{{AFX_MSG_MAP(CFieldServerDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFieldServerDlg message handlers

void CFieldServerDlg::OnCancel()
{
	// TODO: Add extra cleanup here

	CDialog::OnCancel();
}

void CFieldServerDlg::OnOK()
{
	// TODO: Add extra validation here
	UpdateData();

	m_szIPHistory = m_HistoryComboIPAddress.GetHistoryString();

	CDialog::OnOK();
}

BOOL CFieldServerDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
//	if(pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_UP && GetFocus()->GetDlgCtrlID() == IDC_EDIT_SERVER_IP)
//	{
//		char *ip = NULL;
//		if ((ip = m_pIPHistory->GetNext()) == NULL)
//		{
//			GetDlgItem(IDC_EDIT_SERVER_IP)->SetFocus();
//			((CEdit*)GetDlgItem(IDC_EDIT_SERVER_IP))->SetSel(0, -1);
//		}
//		else
//		{
//			GetDlgItem(IDC_EDIT_SERVER_IP)->SetWindowText(ip);
//
//			GetDlgItem(IDC_EDIT_SERVER_IP)->SetFocus();
//			((CEdit*)GetDlgItem(IDC_EDIT_SERVER_IP))->SetSel(0, -1);
//		}
//
//		return TRUE;
//	}
//
//	if(pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_DOWN && GetFocus()->GetDlgCtrlID() == IDC_EDIT_SERVER_IP)
//	{
//		char *ip = NULL;
//		if ((ip = m_pIPHistory->GetPrev()) == NULL)
//		{
//			GetDlgItem(IDC_EDIT_SERVER_IP)->SetFocus();
//			((CEdit*)GetDlgItem(IDC_EDIT_SERVER_IP))->SetSel(0, -1);
//		}
//		else
//		{
//			GetDlgItem(IDC_EDIT_SERVER_IP)->SetWindowText(ip);
//
//			GetDlgItem(IDC_EDIT_SERVER_IP)->SetFocus();
//			((CEdit*)GetDlgItem(IDC_EDIT_SERVER_IP))->SetSel(0, -1);
//		}
//		return TRUE;
//	}

	return CDialog::PreTranslateMessage(pMsg);
}

BOOL CFieldServerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here
	m_HistoryComboIPAddress.InitHistoryComboBox(m_ipaddr, m_szIPHistory, TRUE);

	GetDlgItem(IDC_COMBO_SELECT_SERVER_IP)->SetFocus();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
