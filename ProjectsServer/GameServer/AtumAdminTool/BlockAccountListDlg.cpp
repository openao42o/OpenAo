// BlockAccountListDlg.cpp : implementation file
//

#include "stdafx.h"
#include "atumadmintool.h"
#include "BlockAccountListDlg.h"
#include "SCGridHelper.h"
#include "ODBCStatement.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBlockAccountListDlg dialog


CBlockAccountListDlg::CBlockAccountListDlg(CODBCStatement *i_pODBCStat, CWnd* pParent /*=NULL*/)
	: CDialog(CBlockAccountListDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBlockAccountListDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pODBCStmt1		= i_pODBCStat;
}


void CBlockAccountListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBlockAccountListDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
	DDX_GridControl(pDX, IDC_GRID_BLOCKED_LIST, m_GridBlockedList);
}


BEGIN_MESSAGE_MAP(CBlockAccountListDlg, CDialog)
	//{{AFX_MSG_MAP(CBlockAccountListDlg)
	ON_BN_CLICKED(IDC_BTN_RELOAD_ACCOUNT_BLOCKED_LIST, OnBtnReloadAccountBlockedList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBlockAccountListDlg message handlers


BOOL CBlockAccountListDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	this->InitGridBlockedList();

	this->OnBtnReloadAccountBlockedList();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}












///////////////////////////////////////////////////////////////////////////////
/// \fn			void CBlockAccountListDlg::InitGridBlockedList(void)
/// \brief		
/// \author		cmkwon
/// \date		2006-04-24 ~ 2006-04-24
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CBlockAccountListDlg::InitGridBlockedList(void)
{
	m_GridBlockedList.SetBkColor(0xFFFFFF);

	int m_nRows = 1;
	int m_nCols = 7;
	int m_nFixRows = 1;
	
	m_GridBlockedList.SetEditable(TRUE);
	m_GridBlockedList.SetListMode(TRUE);
	m_GridBlockedList.SetSingleRowSelection(TRUE);
	m_GridBlockedList.EnableSelection(TRUE);
	m_GridBlockedList.SetHeaderSort(FALSE);
	m_GridBlockedList.SetFrameFocusCell(FALSE);
	m_GridBlockedList.SetTrackFocusCell(FALSE);
//	m_GridBlockedList.EnableDragAndDrop(TRUE);
//	m_GridBlockedList.SetTextBkColor(RGB(0xFF, 0xFF, 0xE0));

	m_GridBlockedList.SetRowCount(m_nRows);
	m_GridBlockedList.SetColumnCount(m_nCols);
	m_GridBlockedList.SetFixedRowCount(m_nFixRows);
	//m_GridBlockedList.SetFixedColumnCount(m_nFixCols);

	// 칼럼 만들기
	m_nCols = 0;
	GV_ITEM Item;
	Item.mask = GVIF_TEXT|GVIF_FORMAT;
	Item.row = 0;
	Item.nFormat = GRID_CELL_FORMAT;

	Item.col = m_nCols++;
	Item.strText.Format(_T("Account Name"));
	m_GridBlockedList.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format(_T("Blocked Type"));
	m_GridBlockedList.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format(_T("Block Start DataTime"));
	m_GridBlockedList.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format(_T("Block End DataTime"));
	m_GridBlockedList.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format(_T("Admin AccountName"));
	m_GridBlockedList.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format(_T("Reason for user"));
	m_GridBlockedList.SetItem(&Item);

	// 2007-01-10 by cmkwon, 추가함
	Item.col = m_nCols++;
	Item.strText.Format(_T("Reason for only Admin"));
	m_GridBlockedList.SetItem(&Item);

	m_GridBlockedList.AutoSize();
	m_GridBlockedList.ExpandColumnsToFit();

	CCellRange tmpCellRange(1, 0, m_GridBlockedList.GetRowCount()-1, m_GridBlockedList.GetColumnCount()-1);
	m_GridBlockedList.ClearCells(tmpCellRange);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CBlockAccountListDlg::InsertBlockedUser(SBLOCKED_ACCOUNT_INFO *pBlockedUser)
/// \brief		
/// \author		cmkwon
/// \date		2006-04-24 ~ 2006-04-24
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CBlockAccountListDlg::InsertBlockedUser(SBLOCKED_ACCOUNT_INFO *pBlockedUser)
{
	int m_nCols = 0;

	m_GridBlockedList.SetRowCount(m_GridBlockedList.GetRowCount()+1);

	GV_ITEM Item;
	Item.mask = GVIF_TEXT|GVIF_FORMAT;
	Item.row = m_GridBlockedList.GetRowCount()-1;
	Item.nFormat = GRID_CELL_FORMAT;

	Item.col = m_nCols++;
	Item.strText.Format("%s", pBlockedUser->szBlockedAccountName);
	m_GridBlockedList.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("%s", GetStringBLOCKED_TYPE(pBlockedUser->enBlockedType));
	m_GridBlockedList.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("%s", pBlockedUser->atimeStartTime.GetDateTimeString().GetBuffer());
	m_GridBlockedList.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("%s", pBlockedUser->atimeEndTime.GetDateTimeString().GetBuffer());
	m_GridBlockedList.SetItem(&Item);
	
	
	Item.col = m_nCols++;
	Item.strText.Format("%s", pBlockedUser->szBlockAdminAccountName);		// 2012-06-05 by hskim, 버그 수정 : 어드민툴 블럭한 관리자 표기 에러
	m_GridBlockedList.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("%s", pBlockedUser->szBlockedReasonForUser);
	m_GridBlockedList.SetItem(&Item);

	// 2007-01-10 by cmkwon, 추가함
	Item.col = m_nCols++;
	Item.strText.Format("%s", pBlockedUser->szBlockedReasonForOnlyAdmin);
	m_GridBlockedList.SetItem(&Item);
}


void CBlockAccountListDlg::OnBtnReloadAccountBlockedList() 
{
	// TODO: Add your control notification handler code here
	this->InitGridBlockedList();

	CString szQuery;
// 2008-01-29 by cmkwon, T_A: 계정 블럭 정보 버그 수정
// 	szQuery.Format("SELECT AccountName, BlockedType, StartDate,	EndDate, AdminAccountName, BlockedReason \
// 					FROM td_BlockedAccounts WITH (NOLOCK)");	

// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
//	szQuery.Format("SELECT AccountName, BlockedType, StartDate,	EndDate, AdminAccountName, BlockedReason, BlockedReasonForOnlyAdmin \
//					FROM td_BlockedAccounts WITH (NOLOCK)");	

// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
//	szQuery.Format(QUERY_080702_0003);
//	BOOL bRet = m_pODBCStmt1->ExecuteQuery(szQuery);
	BOOL bRet = m_pODBCStmt1->ExecuteQuery((char*)(PROCEDURE_080827_0003));	
	if (!bRet)
	{
		MessageBox("DB Execute Error !!(CBlockAccountListDlg::OnBtnReloadAccountBlockedList_)");
		m_pODBCStmt1->FreeStatement();
		return;
	}

	SQLINTEGER arrCB[8] = {SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS, SQL_NTS,SQL_NTS,SQL_NTS};

	SBLOCKED_ACCOUNT_INFO tmBlockedUser;
	SQL_TIMESTAMP_STRUCT	arrSqlTime[2];

	SQLBindCol(m_pODBCStmt1->GetSTMTHandle(), 1, SQL_C_CHAR, tmBlockedUser.szBlockedAccountName, SIZE_MAX_ACCOUNT_NAME,			&arrCB[1]);
	SQLBindCol(m_pODBCStmt1->GetSTMTHandle(), 2, SQL_C_LONG, &tmBlockedUser.enBlockedType, 0,									&arrCB[2]);
	SQLBindCol(m_pODBCStmt1->GetSTMTHandle(), 3, SQL_C_TIMESTAMP, &arrSqlTime[0], 0,											&arrCB[3]);
	SQLBindCol(m_pODBCStmt1->GetSTMTHandle(), 4, SQL_C_TIMESTAMP, &arrSqlTime[1], 0,											&arrCB[4]);
	SQLBindCol(m_pODBCStmt1->GetSTMTHandle(), 5, SQL_C_CHAR, tmBlockedUser.szBlockAdminAccountName, SIZE_MAX_ACCOUNT_NAME,		&arrCB[5]);
	SQLBindCol(m_pODBCStmt1->GetSTMTHandle(), 6, SQL_C_CHAR, tmBlockedUser.szBlockedReasonForUser, SIZE_MAX_BLOCKED_ACCOUNT_REASON,	&arrCB[6]);
	SQLBindCol(m_pODBCStmt1->GetSTMTHandle(), 7, SQL_C_CHAR, tmBlockedUser.szBlockedReasonForOnlyAdmin, SIZE_MAX_BLOCKED_ACCOUNT_REASON,	&arrCB[7]);

	util::zero(&tmBlockedUser, sizeof(SBLOCKED_ACCOUNT_INFO));
	util::zero(arrSqlTime, sizeof(arrSqlTime[0])*2);
	while (SQL_NO_DATA != SQLFetch(m_pODBCStmt1->GetSTMTHandle()))
	{
		tmBlockedUser.atimeStartTime	= arrSqlTime[0];
		tmBlockedUser.atimeEndTime		= arrSqlTime[1];
		this->InsertBlockedUser(&tmBlockedUser);

		util::zero(&tmBlockedUser, sizeof(SBLOCKED_ACCOUNT_INFO));
		util::zero(arrSqlTime, sizeof(arrSqlTime[0])*2);
	}
	m_pODBCStmt1->FreeStatement();				// free statement
	m_GridBlockedList.Refresh();
}
