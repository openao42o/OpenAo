// SCBadUserAdminDlg.cpp : implementation file
//

#include "stdafx.h"
#include "AtumAdminTool.h"
#include "SCBadUserAdminDlg.h"
#include "AtumAdminToolDlg.h"
#include "BlockAccountDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSCBadUserAdminDlg dialog


CSCBadUserAdminDlg::CSCBadUserAdminDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSCBadUserAdminDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSCBadUserAdminDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pMainDlg = (CAtumAdminToolDlg*)AfxGetMainWnd();
	m_pODBCStmt = new CODBCStatement;
}

CSCBadUserAdminDlg::~CSCBadUserAdminDlg()
{
	util::del(m_pODBCStmt);
}

void CSCBadUserAdminDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSCBadUserAdminDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
	DDX_GridControl(pDX, IDC_GRID_BAD_USER, m_GridBadUser);
}


BEGIN_MESSAGE_MAP(CSCBadUserAdminDlg, CDialog)
	//{{AFX_MSG_MAP(CSCBadUserAdminDlg)
	ON_BN_CLICKED(IDC_BUTTON_LOAD_DATA, OnButtonLoadData)
	ON_BN_CLICKED(IDC_BUTTON_REFRESH, OnButtonRefresh)
	ON_COMMAND(ID_CMG_DELETE_BLOCKED_USER, OnCmgDeleteBlockedUser)
	ON_COMMAND(ID_CMG_UNBLOCK, OnCmgUnblock)
	ON_COMMAND(ID_CMG_BLOCK_ACCOUNT, OnCmgBlockAccount)
	ON_COMMAND(ID_CMG_BLOCK_CHATTING, OnCmgBlockChatting)
	ON_BN_CLICKED(IDC_BUTTON_BLOCK_USER, OnButtonBlockUser)
	//}}AFX_MSG_MAP
	ON_NOTIFY(NM_RCLICK, IDC_GRID_BAD_USER, OnGridRClick)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSCBadUserAdminDlg message handlers

void CSCBadUserAdminDlg::OnCancel() 
{
	// TODO: Add extra cleanup here

	CDialog::OnCancel();
}

BOOL CSCBadUserAdminDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here
	m_GridBadUser.InitGrid(NUM_COLUMNS_BAD_USER_GRID, g_arrBadUserColumnNames);

	BAD_USER_INFO tmpInfo;
	tmpInfo.AccountName = "Me";
	tmpInfo.BlockType = BLOCK_TYPE_CHATTING_BLOCK;
	tmpInfo.PrevBlock = "Nothing";
	tmpInfo.StartTime.SetCurrentDateTime();
	tmpInfo.EndTime.SetCurrentDateTime();
	tmpInfo.EndTime.Day += 7;
	tmpInfo.AdminName = "scblocktest";
	tmpInfo.Reason = "sample";
	m_GridBadUser.InsertGridBadUser(&tmpInfo);
	m_GridBadUser.InsertGridBadUser(&tmpInfo);
	m_GridBadUser.InsertGridBadUser(&tmpInfo);

	BAD_USER_INFO ret = m_GridBadUser.GetGridBadUser(1);

	ATUM_DATE_TIME aa;
	aa.SetCurrentDateTime();
	aa.AddDateTime(0, 0, 40, 0, 0, 0);
	aa.AddDateTime(0, 0, 0, 25, 0, 0);

	// Connect DB
	if (!m_pODBCStmt->Init(m_pMainDlg->m_pServerInfo4Admin->DBIP, m_pMainDlg->m_pServerInfo4Admin->DBPort, m_pMainDlg->m_pServerInfo4Admin->DBName,
							m_pMainDlg->m_pServerInfo4Admin->DBUID, m_pMainDlg->m_pServerInfo4Admin->DBPWD, GetSafeHwnd())
	)
	{
		MessageBox(STRERR_S_SCADMINTOOL_0013);
		EndDialog(-1);
		return FALSE;
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSCBadUserAdminDlg::OnButtonLoadData() 
{
	// TODO: Add your control notification handler code here
	
}

void CSCBadUserAdminDlg::OnButtonRefresh() 
{
	// TODO: Add your control notification handler code here
	
}

// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 사용하지 않는 부분임
// BOOL CSCBadUserAdminDlg::GetBadUsers()
// {
// 	CString szSQLQuery;
// 	// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
// 	//szSQLQuery.Format("select * from td_AccountBlock\
// 	//					where EndTime > GetDate()");
// 	szSQLQuery.Format(QUERY_080702_0016);	// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
// 
// 	struct TEMP_BAD_USER
// 	{
// 		INT				badUserIndex;
// 		char			AccountName[SIZE_MAX_ACCOUNT_NAME];
// 		INT				BlockType;
// 		INT				PrevBlockIndex;
// 		SQL_TIMESTAMP_STRUCT	StartTime;
// 		SQL_TIMESTAMP_STRUCT	EndTime;
// 		char			AdminName[SIZE_MAX_ACCOUNT_NAME];
// 		char			Reason[50];
// 	};
// 	TEMP_BAD_USER ret;
// 
// 	SQLINTEGER cb1, cb2, cb3, cb4, cb5, cb6, cb7, cb8;
// 
// 	SQLBindCol(m_pODBCStmt->m_hstmt, 1, SQL_C_LONG, &ret.badUserIndex, 0, &cb1);
// 	cb2 = SQL_NTS;
// 	SQLBindCol(m_pODBCStmt->m_hstmt, 2, SQL_C_CHAR, ret.AccountName, SIZE_MAX_ACCOUNT_NAME, &cb2);
// 	SQLBindCol(m_pODBCStmt->m_hstmt, 3, SQL_C_LONG, &ret.BlockType, 0, &cb3);
// 	SQLBindCol(m_pODBCStmt->m_hstmt, 4, SQL_C_LONG, &ret.PrevBlockIndex, 0, &cb4);
// 	SQLBindCol(m_pODBCStmt->m_hstmt, 5, SQL_C_TIMESTAMP, &ret.StartTime, 0, &cb5);
// 	SQLBindCol(m_pODBCStmt->m_hstmt, 6, SQL_C_TIMESTAMP, &ret.EndTime, 0, &cb6);
// 	cb7 = SQL_NTS;
// 	SQLBindCol(m_pODBCStmt->m_hstmt, 7, SQL_C_CHAR, ret.AdminName, SIZE_MAX_ACCOUNT_NAME, &cb7);
// 	cb8 = SQL_NTS;
// 	SQLBindCol(m_pODBCStmt->m_hstmt, 8, SQL_C_CHAR, ret.Reason, 50, &cb8);
// 
// 	BOOL bRet = m_pODBCStmt->ExecuteQuery((LPCSTR)szSQLQuery);
// 	if (!bRet)
// 	{
// 		// clean up
// 		m_pODBCStmt->FreeStatement();
// 		MessageBox("Error");
// 		return FALSE;
// 	}
// 
// 	int nCount = 0;
// 	memset(&ret, 0x00, sizeof(TEMP_BAD_USER));
// 	while ( (bRet = SQLFetch(m_pODBCStmt->m_hstmt)) != SQL_NO_DATA)
// 	{
// 		BAD_USER_INFO tmpBadUser;
// 		tmpBadUser.AccountName = ret.AccountName;
// 
// 		m_GridBadUser.InsertGridBadUser(&tmpBadUser);
// 
// 		nCount++;
// 		memset(&ret, 0x00, sizeof(TEMP_BAD_USER));
// 	}
// 
// 	// clean up
// 	m_pODBCStmt->FreeStatement();
// 
// 	m_GridBadUser.ArrangeGrid();
// 
// 	return TRUE;
// }

void CSCBadUserAdminDlg::OnGridRClick(NMHDR *pNotifyStruct, LRESULT* /*pResult*/)
{
	UpdateData();

    NM_GRIDVIEW* pItem = (NM_GRIDVIEW*) pNotifyStruct;

	if (pItem->iRow == 0)
	{
		return;
	}

	m_GridBadUser.SetFocusCell(-1,-1);
	m_GridBadUser.SetSelectedRange(pItem->iRow, 0, pItem->iRow, m_GridBadUser.GetColumnCount()-1, TRUE, TRUE);

	// show context menu
	CMenu muTemp, *pContextMenu;
	muTemp.LoadMenu(IDR_CONTEXT_MENU_GRID_BAD_USER);
	CPoint point;
	GetCursorPos(&point);
	pContextMenu = muTemp.GetSubMenu(0);
	pContextMenu->TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y, this);
}

void CSCBadUserAdminDlg::OnCmgDeleteBlockedUser() 
{
	// TODO: Add your command handler code here
	
}

void CSCBadUserAdminDlg::OnCmgUnblock() 
{
	// TODO: Add your command handler code here
	
}

void CSCBadUserAdminDlg::OnCmgBlockAccount() 
{
	// TODO: Add your command handler code here
	
}

void CSCBadUserAdminDlg::OnCmgBlockChatting() 
{
	// TODO: Add your command handler code here
	
}

void CSCBadUserAdminDlg::OnButtonBlockUser() 
{
	// TODO: Add your control notification handler code here
}
