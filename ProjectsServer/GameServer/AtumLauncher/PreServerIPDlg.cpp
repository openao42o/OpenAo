// PreServerIPDlg.cpp : implementation file
//

#include "stdafx.h"
#include "AtumLauncher.h"
#include "PreServerIPDlg.h"
#include "AtumLauncherDlg.h"
#include "CommandHistory.h"
#include "BrowseForFolder.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPreServerIPDlg dialog


CPreServerIPDlg::CPreServerIPDlg(CString i_szPreServerIP, CString i_szBin, CString i_szPath, CString i_szPreServerIPHistory, CString i_szBinHistory, CString i_szPathHistory, CWnd* pParent /* = NULL */)
	: CDialog(CPreServerIPDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPreServerIPDlg)
	m_preserver_ip = i_szPreServerIP;
	m_executable_bin = i_szBin;
	m_execute_path = i_szPath;
	m_bUpdateClientFile = FALSE;
	m_bWindowMode = FALSE;
	//}}AFX_DATA_INIT

	m_szPreServerIPHistory = i_szPreServerIPHistory;
	m_szExecuteBinHistory = i_szBinHistory;
	m_szExecutePathHistory = i_szPathHistory;
}

CPreServerIPDlg::~CPreServerIPDlg()
{
}

void CPreServerIPDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPreServerIPDlg)
	DDX_Control(pDX, IDC_COMBO_PATH, m_ComboPath);
	DDX_Control(pDX, IDC_COMBO_BIN, m_ComboBin);
	DDX_Control(pDX, IDC_COMBO_PRESERVER_IP, m_ComboPreServerIP);
	DDX_Check(pDX, IDC_CLIENT_UPDATE, m_bUpdateClientFile);
	DDX_Check(pDX, IDC_WINDOW_MODE, m_bWindowMode);
	DDX_CBString(pDX, IDC_COMBO_BIN, m_executable_bin);
	DDX_CBString(pDX, IDC_COMBO_PATH, m_execute_path);
	DDX_CBString(pDX, IDC_COMBO_PRESERVER_IP, m_preserver_ip);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPreServerIPDlg, CDialog)
	//{{AFX_MSG_MAP(CPreServerIPDlg)
	ON_BN_CLICKED(IDC_WINDOW_MODE, OnWindowMode)
	ON_BN_CLICKED(IDOK, OnOk)
	ON_BN_CLICKED(IDC_BUTTON_BIN_SELECT, OnButtonBinSelect)
	ON_BN_CLICKED(IDC_BUTTON_PATH_SELECT, OnButtonPathSelect)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPreServerIPDlg message handlers

void CPreServerIPDlg::OnWindowMode()
{
	// TODO: Add your control notification handler code here

	UpdateData();
// 2008-01-03 by cmkwon, 윈도우모드 상태 저장하기 - 버그 수정
//	if (m_bWindowMode == TRUE)
//	{
//		((CAtumLauncherDlg*)AfxGetMainWnd())->GetDlgItem(IDC_COMBO_WINDOW_DEGREE_LAUNCHER)->EnableWindow(FALSE);
//	}
//	else
//	{
//		((CAtumLauncherDlg*)AfxGetMainWnd())->GetDlgItem(IDC_COMBO_WINDOW_DEGREE_LAUNCHER)->EnableWindow(TRUE);
//	}
}

BOOL CPreServerIPDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here
	m_ComboPreServerIP.InitHistoryComboBox(m_preserver_ip, m_szPreServerIPHistory, TRUE);
	m_ComboBin.InitHistoryComboBox(m_executable_bin, m_szExecuteBinHistory, FALSE);
	m_ComboPath.InitHistoryComboBox(m_execute_path, m_szExecutePathHistory, FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPreServerIPDlg::OnOk()
{
	// TODO: Add your control notification handler code here
	UpdateData();

	// 경로에 "\" 붙이기
	m_execute_path.TrimRight("\\"); m_execute_path += "\\";
	UpdateData(FALSE);

	m_szPreServerIPHistory = m_ComboPreServerIP.GetHistoryString();
	m_szExecuteBinHistory = m_ComboBin.GetHistoryString();
	m_szExecutePathHistory = m_ComboPath.GetHistoryString();

	CDialog::OnOK();
}

void CPreServerIPDlg::OnButtonBinSelect() 
{
	// TODO: Add your control notification handler code here
	bool bOpen = TRUE;

	CFileDialog fDlg(bOpen, STRMSG_S_ATUMLAUNCHER_0006, bOpen ? NULL : _T(""), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("Executable Files (*.atm;*.exe)|*.atm;*.exe|All Files (*.*)|*.*||"));
	fDlg.m_ofn.lpstrInitialDir = m_executable_bin;
	if (fDlg.DoModal() != IDOK)
		return;
	m_executable_bin = fDlg.GetPathName();

	GetDlgItem(IDC_COMBO_BIN)->SetWindowText(m_executable_bin);

	UpdateData();

	return;
}

void CPreServerIPDlg::OnButtonPathSelect() 
{
	// TODO: Add your control notification handler code here
	UpdateData();

	CBrowseForFolder bf;	
	bf.hWndOwner = this->m_hWnd;
	bf.strTitle = STRMSG_S_ATUMLAUNCHER_0007;
	CString sz;
	if (!bf.GetFolder(sz, (LPCSTR)m_execute_path))
		return;

	sz.TrimRight("\\"); sz += "\\";

	GetDlgItem(IDC_COMBO_PATH)->SetWindowText(sz);

	UpdateData();
}
