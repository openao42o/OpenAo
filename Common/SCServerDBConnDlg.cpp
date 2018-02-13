// SCServerDBConnDlg.cpp : implementation file
//

#include "stdafx.h"
#include "AtumMonitor.h"
#include "SCServerDBConnDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern GAME_SERVER_INFO_FOR_ADMIN g_arrGameServers[];

/////////////////////////////////////////////////////////////////////////////
// CSCServerDBConnDlg dialog


CSCServerDBConnDlg::CSCServerDBConnDlg(BOOL i_bServer, BOOL i_bServerLogin, BOOL i_bDBLogin, const char *i_szServerNameToSelect /* = NULL */, CWnd* pParent /* = NULL */)
	: CDialog(CSCServerDBConnDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSCServerDBConnDlg)
	m_szUIDServer = _T("");
	m_szPWDServer = _T("");
	m_szUIDDB = _T("");
	m_szPWDDB = _T("");
	//}}AFX_DATA_INIT

	m_szServerName = i_szServerNameToSelect;
	SetAttr(i_bServer, i_bServerLogin, i_bDBLogin);
}

void CSCServerDBConnDlg::SetAttr(BOOL i_bServer /* = TRUE */, BOOL i_bServerLogin /* = TRUE */, BOOL i_bDBLogin /* = TRUE */)
{
	m_bServer = i_bServer;
	m_bServerLogin = i_bServerLogin;
	m_bDBLogin = i_bDBLogin;
}

void CSCServerDBConnDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSCServerDBConnDlg)
	DDX_Control(pDX, IDC_COMBO_SERVER_SBC, m_ComboSelectServer);
	DDX_Text(pDX, IDC_EDIT_UID_SERVER_SBC, m_szUIDServer);
	DDX_Text(pDX, IDC_EDIT_PWD_SERVER_SBC, m_szPWDServer);
	DDX_Text(pDX, IDC_EDIT_UID_DB_SBC, m_szUIDDB);
	DDX_Text(pDX, IDC_EDIT_PWD_DB_SBC, m_szPWDDB);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSCServerDBConnDlg, CDialog)
	//{{AFX_MSG_MAP(CSCServerDBConnDlg)
	ON_BN_CLICKED(IDC_BUTTON_CONNECT_SBC, OnButtonConnect)
	ON_BN_CLICKED(IDC_BUTTON_CANCEL_SBC, OnButtonCancel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSCServerDBConnDlg message handlers

void CSCServerDBConnDlg::OnButtonConnect() 
{
	// TODO: Add your control notification handler code here
	UpdateData();

	if (m_bServerLogin && m_szUIDServer == "")
	{
		MessageBox(STRCMD_CS_COMMON_DB_0001);
		GetDlgItem(IDC_EDIT_UID_SERVER_SBC)->SetFocus();
		((CEdit*)GetDlgItem(IDC_EDIT_UID_SERVER_SBC))->SetSel(0, -1);
		return;
	}

	if (m_bServerLogin && m_szPWDServer == "")
	{
		MessageBox(STRCMD_CS_COMMON_DB_0002);
		GetDlgItem(IDC_EDIT_PWD_SERVER_SBC)->SetFocus();
		((CEdit*)GetDlgItem(IDC_EDIT_PWD_SERVER_SBC))->SetSel(0, -1);
		return;
	}

	if (m_bDBLogin && m_szUIDDB == "")
	{
		MessageBox(STRCMD_CS_COMMON_DB_0003);
		GetDlgItem(IDC_EDIT_UID_DB_SBC)->SetFocus();
		((CEdit*)GetDlgItem(IDC_EDIT_UID_DB_SBC))->SetSel(0, -1);
		return;
	}

	if (m_bDBLogin && m_szPWDDB == "")
	{
		MessageBox(STRCMD_CS_COMMON_DB_0004);
		GetDlgItem(IDC_EDIT_PWD_DB_SBC)->SetFocus();
		((CEdit*)GetDlgItem(IDC_EDIT_PWD_DB_SBC))->SetSel(0, -1);
		return;
	}

	if (m_ComboSelectServer.GetCurSel() != -1)
	{
		m_pServerInfo4Admin = &g_arrGameServers[m_ComboSelectServer.GetCurSel()];
		m_ComboSelectServer.GetLBText(m_ComboSelectServer.GetCurSel(), m_szServerName);
	}
	else
	{
		m_pServerInfo4Admin = NULL;
	}

	CDialog::OnOK();
}

void CSCServerDBConnDlg::OnButtonCancel() 
{
	// TODO: Add your control notification handler code here
	
}

BOOL CSCServerDBConnDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	int nComboSelIndex = 0;
	for (int i = 0; g_arrGameServers[i].ServerName !=NULL; i++)
	{
		m_ComboSelectServer.AddString(g_arrGameServers[i].ServerName);
		if (m_szServerName == g_arrGameServers[i].ServerName)
		{
			nComboSelIndex = i;
		}
	}

	m_ComboSelectServer.SetCurSel(nComboSelIndex);

	GetDlgItem(IDC_COMBO_SERVER_SBC)->EnableWindow(m_bServer);
	GetDlgItem(IDC_EDIT_UID_SERVER_SBC)->EnableWindow(m_bServerLogin);
	GetDlgItem(IDC_EDIT_PWD_SERVER_SBC)->EnableWindow(m_bServerLogin);
	GetDlgItem(IDC_EDIT_UID_DB_SBC)->EnableWindow(m_bDBLogin);
	GetDlgItem(IDC_EDIT_PWD_DB_SBC)->EnableWindow(m_bDBLogin);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
