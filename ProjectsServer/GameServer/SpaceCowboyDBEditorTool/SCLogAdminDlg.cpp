// SCLogAdminDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SCLogAdminDlg.h"
#include "SpaceCowboyDBEditorToolDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSCLogAdminDlg dialog


CSCLogAdminDlg::CSCLogAdminDlg(CSpaceCowboyDBEditorToolDlg *i_pMainDlg, CWnd* pParent /* = NULL */)
	: CDialog(CSCLogAdminDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSCLogAdminDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pMainDlg = i_pMainDlg;

	m_pODBCStmt		= i_pMainDlg->m_pODBCStmt;		// 2007-11-08 by cmkwon, 모든 로그 검색 기능 SCDBTool로 이동 -

	m_pmapItemInfo		= &(i_pMainDlg->m_mapItemInfo);		// 2007-11-08 by cmkwon, 모든 로그 검색 기능 SCDBTool로 이동 -
	m_pmapItemInfoName	= &(i_pMainDlg->m_mapItemInfoName);	// 2007-11-08 by cmkwon, 모든 로그 검색 기능 SCDBTool로 이동 -
}


void CSCLogAdminDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSCLogAdminDlg)
	DDX_Control(pDX, IDC_TAB_LOG_ADMIN_TOOL, m_TabLogTool);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSCLogAdminDlg, CDialog)
	//{{AFX_MSG_MAP(CSCLogAdminDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSCLogAdminDlg message handlers

BOOL CSCLogAdminDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

// 2007-11-08 by cmkwon, 모든 로그 검색 기능 SCDBTool로 이동 -
//	// Connect DB
//	if (!m_ODBCStmt.Init(m_pMainDlg->m_pServerInfo4Admin->DBIP, m_pMainDlg->m_pServerInfo4Admin->DBPort, m_pMainDlg->m_pServerInfo4Admin->DBName,
//							m_pMainDlg->m_pServerInfo4Admin->DBUID, m_pMainDlg->m_pServerInfo4Admin->DBPWD, GetSafeHwnd())
//	)
//	{
//		MessageBox(STRERR_S_SCADMINTOOL_0013);
//		EndDialog(-1);
//		return FALSE;
//	}

	m_TabLogTool.InsertItem(0, "Connection Log");
	m_TabLogTool.InsertItem(1, "User Log");
	m_TabLogTool.InsertItem(2, "CharacterItem Log");
	m_TabLogTool.InsertItem(3, "GuildItem Log");
	m_TabLogTool.InsertItem(4, "ServerIntegration Log");

	m_TabLogTool.Init(this);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
