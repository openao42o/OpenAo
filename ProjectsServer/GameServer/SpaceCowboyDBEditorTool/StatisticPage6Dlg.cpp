// StatisticPage6Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "spacecowboydbeditortool.h"
#include "StatisticPage6Dlg.h"
#include "SpaceCowboyDBEditorToolDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStatisticPage6Dlg property page

IMPLEMENT_DYNCREATE(CStatisticPage6Dlg, CPropertyPage)

CStatisticPage6Dlg::CStatisticPage6Dlg() : CPropertyPage(CStatisticPage6Dlg::IDD)
{
	//{{AFX_DATA_INIT(CStatisticPage6Dlg)
	m_ctl_bCharacterRank = FALSE;
	m_ctl_bAccountRank = FALSE;
	m_ctl_nRankCount = 100;
	m_ctl_doTotalMoney = 0.0;
	//}}AFX_DATA_INIT
	m_pMainDlg = (CSpaceCowboyDBEditorToolDlg*)AfxGetMainWnd();
	m_pODBCStmt				= m_pMainDlg->m_pODBCStmt;
}

CStatisticPage6Dlg::~CStatisticPage6Dlg()
{
// 2006-01-23 by cmkwon
//	util::del(m_pODBCStmt);
}

void CStatisticPage6Dlg::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStatisticPage6Dlg)
	DDX_Check(pDX, IDC_CHECK_CHARACTER, m_ctl_bCharacterRank);
	DDX_Check(pDX, IDC_CHECK_ACCOUNT, m_ctl_bAccountRank);
	DDX_Text(pDX, IDC_EDIT_RANK_COUNT, m_ctl_nRankCount);
	DDV_MinMaxInt(pDX, m_ctl_nRankCount, 1, 500);
	DDX_Control(pDX, IDC_CUSTOM_STA_ITEMPOSSESS, m_GridStaItemPossess);
	DDX_Text(pDX, IDC_EDIT_TOTAL_MONEY, m_ctl_doTotalMoney);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CStatisticPage6Dlg, CPropertyPage)
	//{{AFX_MSG_MAP(CStatisticPage6Dlg)
	ON_BN_CLICKED(IDC_BUTTON_ST6_SELECT, OnButtonSt6Select)
	ON_BN_CLICKED(IDC_BUTTON_ST5_SAVE, OnButtonSt5Save)
	ON_BN_CLICKED(IDC_CHECK_ACCOUNT, OnCheckAccount)
	ON_BN_CLICKED(IDC_CHECK_CHARACTER, OnCheckCharacter)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStatisticPage6Dlg message handlers

void CStatisticPage6Dlg::OnButtonSt6Select() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	ViewTotalMoney();

	if(m_ctl_bAccountRank == TRUE)
	{
		ViewMoneyRankByAccount();
	}
	if(m_ctl_bCharacterRank == TRUE)
	{
		ViewMoneyRankByCharacter();
	}
}

// 2005-11-28 by cmkwon, 아래와 같이 수정함
//void CStatisticPage6Dlg::ViewMoneyRank()
//{
//	m_GridStaItemPossess.SetBkColor(0xFFFFFF);
//	m_GridStaItemPossess.SetHeaderSort();
//
//	int m_nRows = 1;
//	int m_nCols = 6;
//
//	m_GridStaItemPossess.SetEditable(FALSE);
//	m_GridStaItemPossess.SetListMode(TRUE);
//	m_GridStaItemPossess.SetSingleRowSelection(TRUE);
//	m_GridStaItemPossess.EnableSelection(TRUE);
//	m_GridStaItemPossess.SetFrameFocusCell(FALSE);
//	m_GridStaItemPossess.SetTrackFocusCell(FALSE);
//
//	m_GridStaItemPossess.SetRowCount(m_nRows);
//	m_GridStaItemPossess.SetColumnCount(m_nCols);
//	m_GridStaItemPossess.SetFixedRowCount(1);
//
//	// 칼럼 만들기
//	m_nCols = 0;
//	GV_ITEM Item;
//	Item.mask = GVIF_TEXT|GVIF_FORMAT;
//	Item.row = 0;
//	Item.nFormat = DT_LEFT|DT_VCENTER|DT_SINGLELINE;
//
//	Item.col = m_nCols++;
//	Item.strText.Format("POSSESS");
//	m_GridStaItemPossess.SetItem(&Item);
//
//	Item.col = m_nCols++;
//	Item.strText.Format("ACCOUNT UNIQUE NUMBER");
//	m_GridStaItemPossess.SetItem(&Item);
//
//	Item.col = m_nCols++;
//	Item.strText.Format("CURRENT COUNT");
//	m_GridStaItemPossess.SetItem(&Item);
//
//	Item.col = m_nCols++;
//	Item.strText.Format("CHARACTER NAME");
//	m_GridStaItemPossess.SetItem(&Item);
//
//	Item.col = m_nCols++;
//	Item.strText.Format("ACCOUNT NAME");
//	m_GridStaItemPossess.SetItem(&Item);
//
//	Item.col = m_nCols++;
//	Item.strText.Format("LEVEL");
//	m_GridStaItemPossess.SetItem(&Item);
//
//	//데이터 insert
//	//DB 접속
//	if (!m_pODBCStmt->Init(m_pMainDlg->m_pServerInfo4Admin->DBIP, m_pMainDlg->m_pServerInfo4Admin->DBPort, m_pMainDlg->m_pServerInfo4Admin->DBName,
//							m_pMainDlg->m_pServerInfo4Admin->DBUID, m_pMainDlg->m_pServerInfo4Admin->DBPWD, GetSafeHwnd())
//	)
//	{
//		return;
//	}
//
//	CString szSQLQuery;
//	szSQLQuery.Format("drop view MoneyItemTable");
//
//	BOOL bRet = m_pODBCStmt->ExecuteQuery((LPCSTR)szSQLQuery);
//
//	szSQLQuery.Format("create view MoneyItemTable as select top 300 * from td_store where itemnum=7000022 order by currentcount desc"
//		);
//
//	bRet = m_pODBCStmt->ExecuteQuery((LPCSTR)szSQLQuery);
//
//	szSQLQuery.Format("select m.possess, m.accountuniquenumber, m.currentcount, c.charactername, c.AccountName, c.level from MoneyItemTable m, td_character c where c.uniquenumber = m.possess and c.race < 128 order by m.currentcount desc");
//
//	SQLINTEGER cb1, cb2, cb3, cb4, cb5, cb6;
//
//	char tempString[20], tempString2[20];
//	int tempSelectData, tempSelectData2, tempSelectData3, tempSelectData4;
//	SQLBindCol(m_pODBCStmt->m_hstmt, 1, SQL_C_ULONG, &tempSelectData, 0, &cb1);
//	SQLBindCol(m_pODBCStmt->m_hstmt, 2, SQL_C_ULONG, &tempSelectData2, 0, &cb2);
//	SQLBindCol(m_pODBCStmt->m_hstmt, 3, SQL_C_ULONG, &tempSelectData3, 0, &cb3);
//	SQLBindCol(m_pODBCStmt->m_hstmt, 4, SQL_C_CHAR, &tempString, 20, &cb4);
//	SQLBindCol(m_pODBCStmt->m_hstmt, 5, SQL_C_CHAR, &tempString2, 20, &cb5);
//	SQLBindCol(m_pODBCStmt->m_hstmt, 6, SQL_C_ULONG, &tempSelectData4, 0, &cb6);
//
//	bRet = m_pODBCStmt->ExecuteQuery((LPCSTR)szSQLQuery);
//	if (!bRet)
//	{
//		// clean up
//		m_pODBCStmt->FreeStatement();
//		MessageBox("Error");
//		return;
//	}
//
//	int nCount = 0;
//	m_GridStaItemPossess.SetRowCount(nCount+1);
//	// DB에 값이 없을때까지 loop를 돈다
//	while ( (bRet = SQLFetch(m_pODBCStmt->m_hstmt)) != SQL_NO_DATA)
//	{
//		m_GridStaItemPossess.SetRowCount(nCount+2);
//
//		//select된 값을 GridDetail GridCtrl에 넣어준다.
//		Item.row = nCount+1;
//		Item.col = 0;
//		Item.strText.Format("%d", tempSelectData);
//		m_GridStaItemPossess.SetItem(&Item);
//		m_GridStaItemPossess.UpdateData();
//
//		Item.col = 1;
//		Item.strText.Format("%d", tempSelectData2);
//		m_GridStaItemPossess.SetItem(&Item);
//		m_GridStaItemPossess.UpdateData();
//
//		Item.col = 2;
//		Item.strText.Format("%d", tempSelectData3);
//		m_GridStaItemPossess.SetItem(&Item);
//		m_GridStaItemPossess.UpdateData();
//
//		Item.col = 3;
//		Item.strText.Format("%s", tempString);
//		m_GridStaItemPossess.SetItem(&Item);
//		m_GridStaItemPossess.UpdateData();
//
//		Item.col = 4;
//		Item.strText.Format("%s", tempString2);
//		m_GridStaItemPossess.SetItem(&Item);
//		m_GridStaItemPossess.UpdateData();
//
//		Item.col = 5;
//		Item.strText.Format("%d", tempSelectData4);
//		m_GridStaItemPossess.SetItem(&Item);
//		m_GridStaItemPossess.UpdateData();
//
//		nCount++;
//	}
//
//	// clean up
//	m_pODBCStmt->FreeStatement();
//
//	m_GridStaItemPossess.AutoSize();
//}

void CStatisticPage6Dlg::ViewMoneyRankByCharacter(void)
{
	m_GridStaItemPossess.SetBkColor(0xFFFFFF);
	m_GridStaItemPossess.SetHeaderSort();

	m_GridStaItemPossess.SetEditable(FALSE);
	m_GridStaItemPossess.SetListMode(TRUE);
	m_GridStaItemPossess.SetSingleRowSelection(TRUE);
	m_GridStaItemPossess.EnableSelection(TRUE);
	m_GridStaItemPossess.SetFrameFocusCell(FALSE);
	m_GridStaItemPossess.SetTrackFocusCell(FALSE);

	m_GridStaItemPossess.SetRowCount(1);
	m_GridStaItemPossess.SetColumnCount(8);
	m_GridStaItemPossess.SetFixedRowCount(1);

	// 칼럼 만들기
	GV_ITEM Item;
	Item.mask = GVIF_TEXT|GVIF_FORMAT;
	Item.nFormat = DT_LEFT|DT_VCENTER|DT_SINGLELINE;
	Item.row = 0;

	Item.col = 0;
	Item.strText.Format("Num");
	m_GridStaItemPossess.SetItem(&Item);
	Item.col = 1;
	Item.strText.Format("Account Name");
	m_GridStaItemPossess.SetItem(&Item);
	Item.col = 2;
	Item.strText.Format("Character Name");
	m_GridStaItemPossess.SetItem(&Item);
	Item.col = 3;
	Item.strText.Format("Unitkind");
	m_GridStaItemPossess.SetItem(&Item);
	Item.col = 4;
	Item.strText.Format("InfluenceType");
	m_GridStaItemPossess.SetItem(&Item);
	Item.col = 5;
	Item.strText.Format("Level");
	m_GridStaItemPossess.SetItem(&Item);
	Item.col = 6;
	Item.strText.Format("Total Play Time");
	m_GridStaItemPossess.SetItem(&Item);
	Item.col = 7;
	Item.strText.Format("Total Money(SPI)");
	m_GridStaItemPossess.SetItem(&Item);

	//데이터 insert
	//DB 접속
// 2006-01-23 by cmkwon
//	if (!m_pODBCStmt->Init(m_pMainDlg->m_pServerInfo4Admin->DBIP, m_pMainDlg->m_pServerInfo4Admin->DBPort, m_pMainDlg->m_pServerInfo4Admin->DBName,
//							m_pMainDlg->m_pServerInfo4Admin->DBUID, m_pMainDlg->m_pServerInfo4Admin->DBPWD, GetSafeHwnd())
//	)
//	{
//		return;
//	}

	m_ctl_nRankCount = max(1, m_ctl_nRankCount);
	m_ctl_nRankCount = min(500, m_ctl_nRankCount);

	CString szSQLQuery;
	// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
	//szSQLQuery.Format("drop view MoneyItemTable");
	szSQLQuery.Format(QUERY_080702_0193);
// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
//	BOOL bRet = m_pODBCStmt->ExecuteQuery((LPCSTR)szSQLQuery);
	SQLHSTMT hstmt = m_pODBCStmt->GetSTMTHandle();
	SQLINTEGER arrCB2[2] = {SQL_NTS,SQL_NTS};
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SZQUERY, 0, (LPSTR)(LPCSTR)szSQLQuery, 0,			&arrCB2[1]);
	BOOL bRet = m_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_080827_0122));

	// 2007-08-09 by cmkwon, 삭제된 캐릭터 처리(RACE_DELETED_CHARACTER) - 검색 결과에서 제외 한다(0 = c.Race&%d AND )
// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
// 	szSQLQuery.Format("CREATE VIEW MoneyItemTable AS \
// 						SELECT TOP %d s.Possess, SUM(CAST(s.CurrentCount AS FLOAT)) AS TotalCount FROM td_store s, td_character c WITH (NOLOCK) \
// 						WHERE s.itemnum=%d and s.Possess = c.uniqueNumber and 0 = c.Race&%d AND c.Race < 128 GROUP BY s.Possess ORDER BY SUM(CAST(s.CurrentCount AS FLOAT)) DESC"
// 						, m_ctl_nRankCount, MONEY_ITEM_NUMBER, RACE_DELETED_CHARACTER);
	///////////////////////////////////////////////////////////////////////////////	
	// 2008-07-08 by cmkwon, MySQL 지원 구현 - 
#if defined(DB_SERVER_MYSQL)
	szSQLQuery.Format(QUERY_080702_0194, MONEY_ITEM_NUMBER, RACE_DELETED_CHARACTER, m_ctl_nRankCount);
#else
	szSQLQuery.Format(QUERY_080702_0194, m_ctl_nRankCount, MONEY_ITEM_NUMBER, RACE_DELETED_CHARACTER);
#endif
// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
//	bRet = m_pODBCStmt->ExecuteQuery((LPCSTR)szSQLQuery);
	arrCB2[1] = SQL_NTS;
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SZQUERY, 0, (LPSTR)(LPCSTR)szSQLQuery, 0,			&arrCB2[1]);
	bRet = m_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_080827_0122));
	m_pODBCStmt->FreeStatement();
	if(FALSE == bRet)
	{
		MessageBox("Error");
		return;
	}

	// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
	//szSQLQuery.Format("select c.AccountName, c.charactername, dbo.atum_GetGearString(c.unitkind), c.InfluenceType, c.level, c.TotalPlayTime, m.TotalCount from MoneyItemTable m, td_character c WITH (NOLOCK) \
	//	where c.uniquenumber = m.possess and c.race < 128 order by m.TotalCount desc");
// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
//	szSQLQuery.Format(QUERY_080702_0195);
//	bRet = m_pODBCStmt->ExecuteQuery((LPCSTR)szSQLQuery);
	bRet = m_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_080827_0195));
	if (!bRet)
	{
		// clean up
		m_pODBCStmt->ProcessLogMessagesForStmt("CStatisticPage6Dlg::ViewMoneyRankByCharacter_");
		MessageBox("Error");
		return;
	}

	SQLINTEGER arrCB[8]={SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS, SQL_NTS,SQL_NTS,SQL_NTS};
	char szAccName[SIZE_MAX_ACCOUNT_NAME];
	char szCharName[SIZE_MAX_CHARACTER_NAME];
	char szUnitkind[10];
	BYTE byInfluenceType = 0;
	BYTE byLevel = 0;
	float fTotalSPI = 0;
	LONGLONG llTotalTime = 0;
	
	SQLBindCol(m_pODBCStmt->m_hstmt, 1, SQL_C_CHAR, szAccName, SIZE_MAX_ACCOUNT_NAME,		&arrCB[1]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 2, SQL_C_CHAR, szCharName, SIZE_MAX_CHARACTER_NAME,	&arrCB[2]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 3, SQL_C_CHAR, szUnitkind, 10,							&arrCB[3]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 4, SQL_C_TINYINT, &byInfluenceType, 0,					&arrCB[4]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 5, SQL_C_TINYINT, &byLevel, 0,							&arrCB[5]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 6, SQL_C_UBIGINT, &llTotalTime, 0,						&arrCB[6]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 7, SQL_C_FLOAT, &fTotalSPI, 0,							&arrCB[7]);

	// DB에 값이 없을때까지 loop를 돈다
	while ( (bRet = SQLFetch(m_pODBCStmt->m_hstmt)) != SQL_NO_DATA)
	{
		int nNewRowIdx = m_GridStaItemPossess.GetRowCount();
		m_GridStaItemPossess.SetRowCount(nNewRowIdx+1);

		//select된 값을 GridDetail GridCtrl에 넣어준다.
		Item.row		= nNewRowIdx;
		Item.col		= 0;
		Item.strText.Format("%d", nNewRowIdx);
		m_GridStaItemPossess.SetItem(&Item);

		Item.col		= 1;
		Item.strText.Format("%s", szAccName);
		m_GridStaItemPossess.SetItem(&Item);

		Item.col		= 2;
		Item.strText.Format("%s", szCharName);
		m_GridStaItemPossess.SetItem(&Item);

		Item.col		= 3;
		Item.strText.Format("%s", szUnitkind);
		m_GridStaItemPossess.SetItem(&Item);

		Item.col		= 4;
		Item.strText.Format("%s", CAtumSJ::GetInfluenceTypeString(byInfluenceType));
		m_GridStaItemPossess.SetItem(&Item);

		Item.col		= 5;
		Item.strText.Format("%d", byLevel);
		m_GridStaItemPossess.SetItem(&Item);

		Item.col		= 6;
		Item.strText.Format("%-15s(%d sec)", ATUM_DATE_TIME::GetDateTimeStringFromSeconds(llTotalTime, string()), (int)llTotalTime);
		m_GridStaItemPossess.SetItem(&Item);

		Item.col		= 7;
		Item.strText.Format("%10.0f", fTotalSPI);
		m_GridStaItemPossess.SetItem(&Item);
	}
	m_GridStaItemPossess.UpdateData();

	m_pODBCStmt->FreeStatement();	// clean up

	m_GridStaItemPossess.AutoSize();
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			void CStatisticPage6Dlg::ViewMoneyRankByAccount(void)
/// \brief		
/// \author		cmkwon
/// \date		2006-01-18 ~ 2006-01-18
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CStatisticPage6Dlg::ViewMoneyRankByAccount(void)
{
	m_GridStaItemPossess.SetBkColor(0xFFFFFF);
	m_GridStaItemPossess.SetHeaderSort();

	m_GridStaItemPossess.SetEditable(FALSE);
	m_GridStaItemPossess.SetListMode(TRUE);
	m_GridStaItemPossess.SetSingleRowSelection(TRUE);
	m_GridStaItemPossess.EnableSelection(TRUE);
	m_GridStaItemPossess.SetFrameFocusCell(FALSE);
	m_GridStaItemPossess.SetTrackFocusCell(FALSE);

	m_GridStaItemPossess.SetRowCount(1);
	m_GridStaItemPossess.SetColumnCount(7);
	m_GridStaItemPossess.SetFixedRowCount(1);

	// 칼럼 만들기
	GV_ITEM Item;
	Item.mask = GVIF_TEXT|GVIF_FORMAT;
	Item.nFormat = DT_LEFT|DT_VCENTER|DT_SINGLELINE;
	Item.row = 0;

	Item.col = 0;
	Item.strText.Format("Num");
	m_GridStaItemPossess.SetItem(&Item);
	Item.col = 1;
	Item.strText.Format("Account Name");
	m_GridStaItemPossess.SetItem(&Item);
	Item.col = 2;
	Item.strText.Format("Character Name");
	m_GridStaItemPossess.SetItem(&Item);
	Item.col = 3;
	Item.strText.Format("Unitkind");
	m_GridStaItemPossess.SetItem(&Item);
	Item.col = 4;
	Item.strText.Format("InfluenceType");
	m_GridStaItemPossess.SetItem(&Item);
	Item.col = 5;
	Item.strText.Format("Level");
	m_GridStaItemPossess.SetItem(&Item);
	Item.col = 6;
	Item.strText.Format("Total Money(SPI)");
	m_GridStaItemPossess.SetItem(&Item);

	//데이터 insert
	//DB 접속
// 2006-01-23 by cmkwon
//	if (!m_pODBCStmt->Init(m_pMainDlg->m_pServerInfo4Admin->DBIP, m_pMainDlg->m_pServerInfo4Admin->DBPort, m_pMainDlg->m_pServerInfo4Admin->DBName,
//							m_pMainDlg->m_pServerInfo4Admin->DBUID, m_pMainDlg->m_pServerInfo4Admin->DBPWD, GetSafeHwnd())
//	)
//	{
//		return;
//	}

	m_ctl_nRankCount = max(1, m_ctl_nRankCount);
	m_ctl_nRankCount = min(500, m_ctl_nRankCount);

	CString szSQLQuery;
	// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
	//szSQLQuery.Format("drop view MoneyItemTable");
	szSQLQuery.Format(QUERY_080702_0196);
// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
//	BOOL bRet = m_pODBCStmt->ExecuteQuery((LPCSTR)szSQLQuery);
	SQLHSTMT hstmt = m_pODBCStmt->GetSTMTHandle();
	SQLINTEGER arrCB2[3] = {SQL_NTS,SQL_NTS,SQL_NTS};
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SZQUERY, 0, (LPSTR)(LPCSTR)szSQLQuery, 0,			&arrCB2[1]);
	BOOL bRet = m_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_080827_0122));
	m_pODBCStmt->FreeStatement();

// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
// 	szSQLQuery.Format("CREATE VIEW MoneyItemTable AS \
// 						SELECT TOP %d s.AccountUniqueNumber, SUM(CAST(s.CurrentCount AS FLOAT)) AS TotalCount \
// 						FROM td_store s, td_account a WITH (NOLOCK) \
// 						WHERE s.itemnum=%d and s.AccountUniqueNumber = a.AccountUniqueNumber and a.AccountType < 128 \
// 						GROUP BY s.AccountUniqueNumber \
// 						ORDER BY SUM(CAST(s.CurrentCount AS FLOAT)) DESC", m_ctl_nRankCount, MONEY_ITEM_NUMBER);

///////////////////////////////////////////////////////////////////////////////	
// 2008-07-08 by cmkwon, MySQL 지원 구현 - 
#if defined(DB_SERVER_MYSQL)
	szSQLQuery.Format(QUERY_080702_0197, MONEY_ITEM_NUMBER, m_ctl_nRankCount);
#else
	szSQLQuery.Format(QUERY_080702_0197, m_ctl_nRankCount, MONEY_ITEM_NUMBER);
#endif

// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
//	bRet = m_pODBCStmt->ExecuteQuery((LPCSTR)szSQLQuery);
	arrCB2[1] = SQL_NTS;
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SZQUERY, 0, (LPSTR)(LPCSTR)szSQLQuery, 0,			&arrCB2[1]);
	bRet = m_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_080827_0122));
	m_pODBCStmt->FreeStatement();
	if(FALSE == bRet)
	{
		MessageBox("PROCEDURE_080827_0122");
		return;
	}

	// 2007-08-09 by cmkwon, 삭제된 캐릭터 처리(RACE_DELETED_CHARACTER) - 검색 결과에서 제외 한다(0 = c.Race&%d AND )
// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
// 	szSQLQuery.Format("SELECT c.accountname, c.charactername, dbo.atum_GetGearString(c.unitkind) as unit, c.InfluenceType, c.level, tmM.TotalCount \
// 							FROM td_character c, (SELECT accountuniquenumber, max(experience) as maxexp, count(*) as numofcharacter	FROM td_character \
// 													WHERE 0 = Race&%d AND race < 128 and charactername not like 'loadtest%%'	group by accountuniquenumber) c2, td_account a, MoneyItemTable tmM \
// 							WHERE 0 = c.Race&%d AND c.Race < 128 AND c.accountuniquenumber = c2.accountuniquenumber and c.experience >= c2.maxexp AND (c.AccountUniqueNumber in (select AccountUniqueNumber from MoneyItemTable)) \
// 									AND a.accountuniquenumber = c.accountuniquenumber and tmM.AccountUniqueNumber = c.accountuniquenumber \
// 							ORDER BY tmM.TotalCount desc", m_ctl_nRankCount, RACE_DELETED_CHARACTER, RACE_DELETED_CHARACTER);
// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
//	szSQLQuery.Format(QUERY_080702_0198, m_ctl_nRankCount, RACE_DELETED_CHARACTER, RACE_DELETED_CHARACTER);
//	bRet = m_pODBCStmt->ExecuteQuery((LPCSTR)szSQLQuery);
	USHORT tmExcludeRace = RACE_DELETED_CHARACTER;
	arrCB2[1] = arrCB2[2] = SQL_NTS;
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &(m_ctl_nRankCount), 0,		&arrCB2[1]);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_USHORT, SQL_SMALLINT, 0, 0, &(tmExcludeRace), 0,		&arrCB2[2]);
	bRet = m_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_080827_0198));
	if (!bRet)
	{
		// clean up
		m_pODBCStmt->FreeStatement();
		MessageBox("Error");
		return;
	}

	SQLINTEGER arrCB[7]={SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS, SQL_NTS,SQL_NTS};
	char szAccName[SIZE_MAX_ACCOUNT_NAME];
	char szCharName[SIZE_MAX_CHARACTER_NAME];
	char szUnitkind[10];
	BYTE byInfluenceType = 0;
	BYTE byLevel = 0;
	float fTotalSPI = 0;
	
	SQLBindCol(m_pODBCStmt->m_hstmt, 1, SQL_C_CHAR, szAccName, SIZE_MAX_ACCOUNT_NAME,		&arrCB[1]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 2, SQL_C_CHAR, szCharName, SIZE_MAX_CHARACTER_NAME,	&arrCB[2]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 3, SQL_C_CHAR, szUnitkind, 10,						&arrCB[3]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 4, SQL_C_TINYINT, &byInfluenceType, 0,					&arrCB[4]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 5, SQL_C_TINYINT, &byLevel, 0,							&arrCB[5]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 6, SQL_C_FLOAT, &fTotalSPI, 0,							&arrCB[6]);

	// DB에 값이 없을때까지 loop를 돈다
	while ( (bRet = SQLFetch(m_pODBCStmt->m_hstmt)) != SQL_NO_DATA)
	{
		int nNewRowIdx = m_GridStaItemPossess.GetRowCount();
		m_GridStaItemPossess.SetRowCount(nNewRowIdx+1);

		//select된 값을 GridDetail GridCtrl에 넣어준다.
		Item.row		= nNewRowIdx;
		Item.col		= 0;
		Item.strText.Format("%d", nNewRowIdx);
		m_GridStaItemPossess.SetItem(&Item);

		Item.col		= 1;
		Item.strText.Format("%s", szAccName);
		m_GridStaItemPossess.SetItem(&Item);

		Item.col		= 2;
		Item.strText.Format("%s", szCharName);
		m_GridStaItemPossess.SetItem(&Item);

		Item.col		= 3;
		Item.strText.Format("%s", szUnitkind);
		m_GridStaItemPossess.SetItem(&Item);

		Item.col		= 4;
		Item.strText.Format("%s", CAtumSJ::GetInfluenceTypeString(byInfluenceType));
		m_GridStaItemPossess.SetItem(&Item);

		Item.col		= 5;
		Item.strText.Format("%d", byLevel);
		m_GridStaItemPossess.SetItem(&Item);

		Item.col		= 6;
		Item.strText.Format("%10.0f", fTotalSPI);
		m_GridStaItemPossess.SetItem(&Item);
	}
	m_GridStaItemPossess.UpdateData();

	m_pODBCStmt->FreeStatement();	// clean up

	m_GridStaItemPossess.AutoSize();
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			void CStatisticPage6Dlg::ViewTotalMoney(void)
/// \brief		
/// \author		cmkwon
/// \date		2006-01-18 ~ 2006-01-18
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CStatisticPage6Dlg::ViewTotalMoney(void)
{
// 2006-01-23 by cmkwon
//	if (!m_pODBCStmt->Init(m_pMainDlg->m_pServerInfo4Admin->DBIP, m_pMainDlg->m_pServerInfo4Admin->DBPort, m_pMainDlg->m_pServerInfo4Admin->DBName,
//							m_pMainDlg->m_pServerInfo4Admin->DBUID, m_pMainDlg->m_pServerInfo4Admin->DBPWD, GetSafeHwnd())
//	)
//	{
//		return;
//	}

	CString szSQLQuery;
	// 2007-08-09 by cmkwon, 삭제된 캐릭터 처리(RACE_DELETED_CHARACTER) - 검색 결과에서 제외 한다(0 = c.Race&%d AND )
// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
// 	szSQLQuery.Format("SELECT SUM(CAST(CurrentCount AS FLOAT)) \
// 							FROM td_Store s, td_character c WITH (NOLOCK)\
// 							WHERE s.ItemNum= %d and s.Possess = c.uniqueNumber and 0 = c.Race&%d AND c.Race < 128 \
// 							GROUP by s.ItemNum", MONEY_ITEM_NUMBER, RACE_DELETED_CHARACTER);

// 	szSQLQuery.Format(QUERY_080702_0199, MONEY_ITEM_NUMBER, RACE_DELETED_CHARACTER);
// 	float fTotalSPI = 0;
// 	SQLBindCol(m_pODBCStmt->m_hstmt, 1, SQL_C_FLOAT, &fTotalSPI, 0,	NULL);
// 	BOOL bRet = m_pODBCStmt->ExecuteQuery((LPCSTR)szSQLQuery);
// 	if (!bRet)
// 	{
// 		// clean up
// 		m_pODBCStmt->FreeStatement();
// 		MessageBox("Error");
// 		return;
// 	}
	USHORT tmExcludeRace = RACE_DELETED_CHARACTER;
	int tmMoneyItemNum = MONEY_ITEM_NUMBER;
	SQLHSTMT hstmt = m_pODBCStmt->GetSTMTHandle();
	SQLINTEGER arrCB2[3] = {SQL_NTS,SQL_NTS,SQL_NTS};
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &(tmMoneyItemNum), 0,			&arrCB2[1]);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_USHORT, SQL_SMALLINT, 0, 0, &(tmExcludeRace), 0,			&arrCB2[2]);
	BOOL bRet = m_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_080827_0199));
 	if (!bRet)
 	{
 		// clean up
 		m_pODBCStmt->FreeStatement();
 		MessageBox("Error");
 		return;
 	}
	arrCB2[1] = SQL_NTS;
 	float fTotalSPI = 0;
 	SQLBindCol(m_pODBCStmt->m_hstmt, 1, SQL_C_FLOAT, &fTotalSPI, 0,	&arrCB2[1]);
	bRet = SQLFetch(m_pODBCStmt->m_hstmt);
	if(SQL_NO_DATA != bRet)
	{
		m_ctl_doTotalMoney = fTotalSPI;
		UpdateData(FALSE);
	}
	m_pODBCStmt->FreeStatement();	// clean up
}


// 2006-01-18 by cmkwon, 
//void CStatisticPage6Dlg::ViewUniAvgMoney()
//{
//	m_GridStaItemPossess.SetBkColor(0xFFFFFF);
//	m_GridStaItemPossess.SetHeaderSort();
//
//	int m_nRows = 1;
//	int m_nCols;
//	if(m_valueRadioUnitAvgMoney == 0)
//		m_nCols = 3;
//	else
//		m_nCols = 2;
//
//	m_GridStaItemPossess.SetEditable(FALSE);
//	m_GridStaItemPossess.SetListMode(TRUE);
//	m_GridStaItemPossess.SetSingleRowSelection(TRUE);
//	m_GridStaItemPossess.EnableSelection(TRUE);
//	m_GridStaItemPossess.SetFrameFocusCell(FALSE);
//	m_GridStaItemPossess.SetTrackFocusCell(FALSE);
//
//	m_GridStaItemPossess.SetRowCount(m_nRows);
//	m_GridStaItemPossess.SetColumnCount(m_nCols);
//	m_GridStaItemPossess.SetFixedRowCount(1);
//
//	// 칼럼 만들기
//	m_nCols = 0;
//	GV_ITEM Item;
//	Item.mask = GVIF_TEXT|GVIF_FORMAT;
//	Item.row = 0;
//	Item.nFormat = DT_LEFT|DT_VCENTER|DT_SINGLELINE;
//
//	if(m_valueRadioUnitAvgMoney == 0)
//	{
//		Item.col = m_nCols++;
//		Item.strText.Format("LEVEL");
//		m_GridStaItemPossess.SetItem(&Item);
//
//		Item.col = m_nCols++;
//		Item.strText.Format("UNITKIND");
//		m_GridStaItemPossess.SetItem(&Item);
//
//		Item.col = m_nCols++;
//		Item.strText.Format("AVG MONEY");
//		m_GridStaItemPossess.SetItem(&Item);
//	}
//	else
//	{
//		Item.col = m_nCols++;
//		Item.strText.Format("ACCOUNT UNIQUE NUMBER");
//		m_GridStaItemPossess.SetItem(&Item);
//
//		Item.col = m_nCols++;
//		Item.strText.Format("AVG MONEY");
//		m_GridStaItemPossess.SetItem(&Item);
//	}
//
//	//데이터 insert
//	//DB 접속
//	if (!m_pODBCStmt->Init(m_pMainDlg->m_pServerInfo4Admin->DBIP, m_pMainDlg->m_pServerInfo4Admin->DBPort, m_pMainDlg->m_pServerInfo4Admin->DBName,
//							m_pMainDlg->m_pServerInfo4Admin->DBUID, m_pMainDlg->m_pServerInfo4Admin->DBPWD, GetSafeHwnd())
//	)
//	{
//		return;
//	}
//
//	CString szSQLQuery;
//	int tempSelectData, tempSelectData2;
//	float tempSelectData3;
//
//	int tempSelectData4;
//	float tempSelectData5;
//	if(m_valueRadioUnitAvgMoney == 0)
//	{
//		szSQLQuery.Format("select level, unitkind, avg(currentcount) from td_character, td_store \
//			where td_character.accountuniquenumber = td_store.accountuniquenumber and td_store.itemnum = 7000022 \
//			and (td_character.uniquenumber = td_store.possess or td_store.possess = 0) \
//			group by level, unitkind order by level, unitkind");
//
//		SQLINTEGER cb1, cb2, cb3;
//		
//		SQLBindCol(m_pODBCStmt->m_hstmt, 1, SQL_C_ULONG, &tempSelectData, 0, &cb1);
//		SQLBindCol(m_pODBCStmt->m_hstmt, 2, SQL_C_ULONG, &tempSelectData2, 0, &cb2);
//		SQLBindCol(m_pODBCStmt->m_hstmt, 3, SQL_C_FLOAT, &tempSelectData3, 0, &cb3);
//	}
//	else
//	{
//		szSQLQuery.Format("select s.accountuniquenumber, avg( cast(currentcount as float)) From td_character, td_Store s \
//			Where s.itemnum = 7000022 and td_character.accountuniquenumber = s.accountuniquenumber \
//			group by s.accountuniquenumber order by avg( cast(currentcount as float)) desc");
//
//		SQLINTEGER cb1, cb2;
//		SQLBindCol(m_pODBCStmt->m_hstmt, 1, SQL_C_ULONG, &tempSelectData4, 0, &cb1);
//		SQLBindCol(m_pODBCStmt->m_hstmt, 2, SQL_C_FLOAT, &tempSelectData5, 0, &cb2);
//	}
//	
//	BOOL bRet = m_pODBCStmt->ExecuteQuery((LPCSTR)szSQLQuery);
//	if (!bRet)
//	{
//		// clean up
//		m_pODBCStmt->FreeStatement();
//		MessageBox("Error");
//		return;
//	}
//
//	int nCount = 0;
//	m_GridStaItemPossess.SetRowCount(nCount+1);
//	// DB에 값이 없을때까지 loop를 돈다
//	while ( (bRet = SQLFetch(m_pODBCStmt->m_hstmt)) != SQL_NO_DATA)
//	{
//		m_GridStaItemPossess.SetRowCount(nCount+2);
//
//		//select된 값을 GridDetail GridCtrl에 넣어준다.
//		if(m_valueRadioUnitAvgMoney == 0)
//		{
//			Item.row = nCount+1;
//			Item.col = 0;
//			Item.strText.Format("%d", tempSelectData);
//			m_GridStaItemPossess.SetItem(&Item);
//			m_GridStaItemPossess.UpdateData();
//
//			Item.col = 1;
//			Item.strText.Format("%d", tempSelectData2);
//			m_GridStaItemPossess.SetItem(&Item);
//			m_GridStaItemPossess.UpdateData();
//
//			Item.col = 2;
//			Item.strText.Format("%f", tempSelectData3);
//			m_GridStaItemPossess.SetItem(&Item);
//			m_GridStaItemPossess.UpdateData();
//		}
//		else
//		{
//			Item.row = nCount+1;
//			Item.col = 0;
//			Item.strText.Format("%d", tempSelectData4);
//			m_GridStaItemPossess.SetItem(&Item);
//			m_GridStaItemPossess.UpdateData();
//
//			Item.col = 1;
//			Item.strText.Format("%f", tempSelectData5);
//			m_GridStaItemPossess.SetItem(&Item);
//			m_GridStaItemPossess.UpdateData();
//		}
//
//		nCount++;
//	}
//
//	// clean up
//	m_pODBCStmt->FreeStatement();
//
//	m_GridStaItemPossess.AutoSize();
//}
//
//void CStatisticPage6Dlg::ViewRareItemAvg()
//{
//	m_GridStaItemPossess.SetBkColor(0xFFFFFF);
//	m_GridStaItemPossess.SetHeaderSort();
//
//	int m_nRows = 1;
//	int m_nCols;
//	if(m_valueRadioRareItemAvg == 0)
//		m_nCols = 3;
//	else
//		m_nCols = 2;
//
//	m_GridStaItemPossess.SetEditable(FALSE);
//	m_GridStaItemPossess.SetListMode(TRUE);
//	m_GridStaItemPossess.SetSingleRowSelection(TRUE);
//	m_GridStaItemPossess.EnableSelection(TRUE);
//	m_GridStaItemPossess.SetFrameFocusCell(FALSE);
//	m_GridStaItemPossess.SetTrackFocusCell(FALSE);
//
//	m_GridStaItemPossess.SetRowCount(m_nRows);
//	m_GridStaItemPossess.SetColumnCount(m_nCols);
//	m_GridStaItemPossess.SetFixedRowCount(1);
//
//	// 칼럼 만들기
//	m_nCols = 0;
//	GV_ITEM Item;
//	Item.mask = GVIF_TEXT|GVIF_FORMAT;
//	Item.row = 0;
//	Item.nFormat = DT_LEFT|DT_VCENTER|DT_SINGLELINE;
//
//	if(m_valueRadioRareItemAvg == 0)
//	{
//		Item.col = m_nCols++;
//		Item.strText.Format("LEVEL");
//		m_GridStaItemPossess.SetItem(&Item);
//
//		Item.col = m_nCols++;
//		Item.strText.Format("UNITKIND");
//		m_GridStaItemPossess.SetItem(&Item);
//
//		Item.col = m_nCols++;
//		Item.strText.Format("AVG MONEY");
//		m_GridStaItemPossess.SetItem(&Item);
//	}
//	else
//	{
//		Item.col = m_nCols++;
//		Item.strText.Format("ACCOUNT UNIQUE NUMBER");
//		m_GridStaItemPossess.SetItem(&Item);
//
//		Item.col = m_nCols++;
//		Item.strText.Format("AVG MONEY");
//		m_GridStaItemPossess.SetItem(&Item);
//	}
//
//	//데이터 insert
//	//DB 접속
//	if (!m_pODBCStmt->Init(m_pMainDlg->m_pServerInfo4Admin->DBIP, m_pMainDlg->m_pServerInfo4Admin->DBPort, m_pMainDlg->m_pServerInfo4Admin->DBName,
//							m_pMainDlg->m_pServerInfo4Admin->DBUID, m_pMainDlg->m_pServerInfo4Admin->DBPWD, GetSafeHwnd())
//	)
//	{
//		return;
//	}
//
//	CString szSQLQuery;
//	int tempSelectData, tempSelectData2;
//	float tempSelectData3;
//
//	int tempSelectData4;
//	float tempSelectData5;
//	if(m_valueRadioRareItemAvg == 0)
//	{
//		szSQLQuery.Format("select level, unitkind, avg( cast(case when prefixcodenum <> 0 then 1 else 0 end +case when suffixcodenum <> 0 then 1 else 0 end as float ))/2*100 as rareitem퍼센트 from atum.td_character, td_store where td_character.accountuniquenumber = td_store.accountuniquenumber and (td_character.uniquenumber = td_store.possess or td_store.possess = 0) group by level, unitkind order by level, unitkind");
//
//		SQLINTEGER cb1, cb2, cb3;
//		
//		SQLBindCol(m_pODBCStmt->m_hstmt, 1, SQL_C_ULONG, &tempSelectData, 0, &cb1);
//		SQLBindCol(m_pODBCStmt->m_hstmt, 2, SQL_C_ULONG, &tempSelectData2, 0, &cb2);
//		SQLBindCol(m_pODBCStmt->m_hstmt, 3, SQL_C_FLOAT, &tempSelectData3, 0, &cb3);
//	}
//	else
//	{
//		szSQLQuery.Format("select td_store.accountuniquenumber,  avg( cast(case when prefixcodenum <> 0 then 1 else 0 end +case when suffixcodenum <> 0 then 1 else 0 end as float ))/2*100 as rareitem퍼센트 from td_store, td_character where td_character.accountuniquenumber = td_store.accountuniquenumber group by td_store.accountuniquenumber");
//
//		SQLINTEGER cb1, cb2;
//		SQLBindCol(m_pODBCStmt->m_hstmt, 1, SQL_C_ULONG, &tempSelectData4, 0, &cb1);
//		SQLBindCol(m_pODBCStmt->m_hstmt, 2, SQL_C_FLOAT, &tempSelectData5, 0, &cb2);
//	}
//	
//	BOOL bRet = m_pODBCStmt->ExecuteQuery((LPCSTR)szSQLQuery);
//	if (!bRet)
//	{
//		// clean up
//		m_pODBCStmt->FreeStatement();
//		MessageBox("Error");
//		return;
//	}
//
//	int nCount = 0;
//	m_GridStaItemPossess.SetRowCount(nCount+1);
//	// DB에 값이 없을때까지 loop를 돈다
//	while ( (bRet = SQLFetch(m_pODBCStmt->m_hstmt)) != SQL_NO_DATA)
//	{
//		m_GridStaItemPossess.SetRowCount(nCount+2);
//
//		//select된 값을 GridDetail GridCtrl에 넣어준다.
//		if(m_valueRadioRareItemAvg == 0)
//		{
//			Item.row = nCount+1;
//			Item.col = 0;
//			Item.strText.Format("%d", tempSelectData);
//			m_GridStaItemPossess.SetItem(&Item);
//			m_GridStaItemPossess.UpdateData();
//
//			Item.col = 1;
//			Item.strText.Format("%d", tempSelectData2);
//			m_GridStaItemPossess.SetItem(&Item);
//			m_GridStaItemPossess.UpdateData();
//
//			Item.col = 2;
//			Item.strText.Format("%f", tempSelectData3);
//			m_GridStaItemPossess.SetItem(&Item);
//			m_GridStaItemPossess.UpdateData();
//		}
//		else
//		{
//			Item.row = nCount+1;
//			Item.col = 0;
//			Item.strText.Format("%d", tempSelectData4);
//			m_GridStaItemPossess.SetItem(&Item);
//			m_GridStaItemPossess.UpdateData();
//
//			Item.col = 1;
//			Item.strText.Format("%f", tempSelectData5);
//			m_GridStaItemPossess.SetItem(&Item);
//			m_GridStaItemPossess.UpdateData();
//		}
//		nCount++;
//	}
//
//	// clean up
//	m_pODBCStmt->FreeStatement();
//
//	m_GridStaItemPossess.AutoSize();
//}
//
//void CStatisticPage6Dlg::ViewEnchantAvg()
//{
//	m_GridStaItemPossess.SetBkColor(0xFFFFFF);
//	m_GridStaItemPossess.SetHeaderSort();
//
//	int m_nRows = 1;
//	int m_nCols;
//	if(m_valueRadioenchantAvg == 0)
//		m_nCols = 3;
//	else
//		m_nCols = 2;
//
//	m_GridStaItemPossess.SetEditable(FALSE);
//	m_GridStaItemPossess.SetListMode(TRUE);
//	m_GridStaItemPossess.SetSingleRowSelection(TRUE);
//	m_GridStaItemPossess.EnableSelection(TRUE);
//	m_GridStaItemPossess.SetFrameFocusCell(FALSE);
//	m_GridStaItemPossess.SetTrackFocusCell(FALSE);
//
//	m_GridStaItemPossess.SetRowCount(m_nRows);
//	m_GridStaItemPossess.SetColumnCount(m_nCols);
//	m_GridStaItemPossess.SetFixedRowCount(1);
//
//	// 칼럼 만들기
//	m_nCols = 0;
//	GV_ITEM Item;
//	Item.mask = GVIF_TEXT|GVIF_FORMAT;
//	Item.row = 0;
//	Item.nFormat = DT_LEFT|DT_VCENTER|DT_SINGLELINE;
//
//	if(m_valueRadioenchantAvg == 0)
//	{
//		Item.col = m_nCols++;
//		Item.strText.Format("LEVEL");
//		m_GridStaItemPossess.SetItem(&Item);
//
//		Item.col = m_nCols++;
//		Item.strText.Format("UNITKIND");
//		m_GridStaItemPossess.SetItem(&Item);
//
//		Item.col = m_nCols++;
//		Item.strText.Format("AVG MONEY");
//		m_GridStaItemPossess.SetItem(&Item);
//	}
//	else
//	{
//		Item.col = m_nCols++;
//		Item.strText.Format("ACCOUNT UNIQUE NUMBER");
//		m_GridStaItemPossess.SetItem(&Item);
//
//		Item.col = m_nCols++;
//		Item.strText.Format("AVG MONEY");
//		m_GridStaItemPossess.SetItem(&Item);
//	}
//
//	//데이터 insert
//	//DB 접속
//	if (!m_pODBCStmt->Init(m_pMainDlg->m_pServerInfo4Admin->DBIP, m_pMainDlg->m_pServerInfo4Admin->DBPort, m_pMainDlg->m_pServerInfo4Admin->DBName,
//							m_pMainDlg->m_pServerInfo4Admin->DBUID, m_pMainDlg->m_pServerInfo4Admin->DBPWD, GetSafeHwnd())
//	)
//	{
//		return;
//	}
//
//	CString szSQLQuery;
//	int tempSelectData, tempSelectData2;
//	float tempSelectData3;
//
//	int tempSelectData4;
//	float tempSelectData5;
//	if(m_valueRadioenchantAvg == 0)
//	{
//		szSQLQuery.Format("select level, unitkind, avg(numofenchants) from atum.td_character, td_store where td_character.accountuniquenumber = td_store.accountuniquenumber and (td_character.uniquenumber = td_store.possess or td_store.possess = 0)	group by level, unitkind order by level, unitkind");
//
//		SQLINTEGER cb1, cb2, cb3;
//		
//		SQLBindCol(m_pODBCStmt->m_hstmt, 1, SQL_C_ULONG, &tempSelectData, 0, &cb1);
//		SQLBindCol(m_pODBCStmt->m_hstmt, 2, SQL_C_ULONG, &tempSelectData2, 0, &cb2);
//		SQLBindCol(m_pODBCStmt->m_hstmt, 3, SQL_C_FLOAT, &tempSelectData3, 0, &cb3);
//	}
//	else
//	{
//		szSQLQuery.Format("select td_store.accountuniquenumber,  avg(numofenchants)   from td_store, td_character where td_character.accountuniquenumber = td_store.accountuniquenumber and (td_character.uniquenumber = td_store.possess or td_store.possess = 0) group by td_store.accountuniquenumber");
//
//		SQLINTEGER cb1, cb2;
//		SQLBindCol(m_pODBCStmt->m_hstmt, 1, SQL_C_ULONG, &tempSelectData4, 0, &cb1);
//		SQLBindCol(m_pODBCStmt->m_hstmt, 2, SQL_C_FLOAT, &tempSelectData5, 0, &cb2);
//	}
//	
//	BOOL bRet = m_pODBCStmt->ExecuteQuery((LPCSTR)szSQLQuery);
//	if (!bRet)
//	{
//		// clean up
//		m_pODBCStmt->FreeStatement();
//		MessageBox("Error");
//		return;
//	}
//
//	int nCount = 0;
//	m_GridStaItemPossess.SetRowCount(nCount+1);
//	// DB에 값이 없을때까지 loop를 돈다
//	while ( (bRet = SQLFetch(m_pODBCStmt->m_hstmt)) != SQL_NO_DATA)
//	{
//		m_GridStaItemPossess.SetRowCount(nCount+2);
//
//		//select된 값을 GridDetail GridCtrl에 넣어준다.
//		if(m_valueRadioenchantAvg == 0)
//		{
//			Item.row = nCount+1;
//			Item.col = 0;
//			Item.strText.Format("%d", tempSelectData);
//			m_GridStaItemPossess.SetItem(&Item);
//			m_GridStaItemPossess.UpdateData();
//
//			Item.col = 1;
//			Item.strText.Format("%d", tempSelectData2);
//			m_GridStaItemPossess.SetItem(&Item);
//			m_GridStaItemPossess.UpdateData();
//
//			Item.col = 2;
//			Item.strText.Format("%f", tempSelectData3);
//			m_GridStaItemPossess.SetItem(&Item);
//			m_GridStaItemPossess.UpdateData();
//		}
//		else
//		{
//			Item.row = nCount+1;
//			Item.col = 0;
//			Item.strText.Format("%d", tempSelectData4);
//			m_GridStaItemPossess.SetItem(&Item);
//			m_GridStaItemPossess.UpdateData();
//
//			Item.col = 1;
//			Item.strText.Format("%f", tempSelectData5);
//			m_GridStaItemPossess.SetItem(&Item);
//			m_GridStaItemPossess.UpdateData();
//		}
//		nCount++;
//	}
//
//	// clean up
//	m_pODBCStmt->FreeStatement();
//
//	m_GridStaItemPossess.AutoSize();
//}
//
//void CStatisticPage6Dlg::ViewMultiShotItem()
//{
//	m_GridStaItemPossess.SetBkColor(0xFFFFFF);
//	m_GridStaItemPossess.SetHeaderSort();
//
//	int m_nRows = 1;
//	int m_nCols;
//	if(m_valueRadioMultiShotItem == 0)
//		m_nCols = 3;
//	else
//		m_nCols = 2;
//
//	m_GridStaItemPossess.SetEditable(FALSE);
//	m_GridStaItemPossess.SetListMode(TRUE);
//	m_GridStaItemPossess.SetSingleRowSelection(TRUE);
//	m_GridStaItemPossess.EnableSelection(TRUE);
//	m_GridStaItemPossess.SetFrameFocusCell(FALSE);
//	m_GridStaItemPossess.SetTrackFocusCell(FALSE);
//
//	m_GridStaItemPossess.SetRowCount(m_nRows);
//	m_GridStaItemPossess.SetColumnCount(m_nCols);
//	m_GridStaItemPossess.SetFixedRowCount(1);
//
//	// 칼럼 만들기
//	m_nCols = 0;
//	GV_ITEM Item;
//	Item.mask = GVIF_TEXT|GVIF_FORMAT;
//	Item.row = 0;
//	Item.nFormat = DT_LEFT|DT_VCENTER|DT_SINGLELINE;
//
//	if(m_valueRadioMultiShotItem == 0)
//	{
//		Item.col = m_nCols++;
//		Item.strText.Format("LEVEL");
//		m_GridStaItemPossess.SetItem(&Item);
//
//		Item.col = m_nCols++;
//		Item.strText.Format("UNITKIND");
//		m_GridStaItemPossess.SetItem(&Item);
//
//		Item.col = m_nCols++;
//		Item.strText.Format("AVG MONEY");
//		m_GridStaItemPossess.SetItem(&Item);
//	}
//	else
//	{
//		Item.col = m_nCols++;
//		Item.strText.Format("ACCOUNT UNIQUE NUMBER");
//		m_GridStaItemPossess.SetItem(&Item);
//
//		Item.col = m_nCols++;
//		Item.strText.Format("AVG MONEY");
//		m_GridStaItemPossess.SetItem(&Item);
//	}
//
//	//데이터 insert
//	//DB 접속
//	if (!m_pODBCStmt->Init(m_pMainDlg->m_pServerInfo4Admin->DBIP, m_pMainDlg->m_pServerInfo4Admin->DBPort, m_pMainDlg->m_pServerInfo4Admin->DBName,
//							m_pMainDlg->m_pServerInfo4Admin->DBUID, m_pMainDlg->m_pServerInfo4Admin->DBPWD, GetSafeHwnd())
//	)
//	{
//		return;
//	}
//
//	CString szSQLQuery;
//	int tempSelectData, tempSelectData2;
//	float tempSelectData3;
//
//	int tempSelectData4;
//	float tempSelectData5;
//	if(m_valueRadioMultiShotItem == 0)
//	{
//		szSQLQuery.Format("select level, unitkind, avg( cast(case when i.Kind >= 40 and i.Kind < 49/* and (s.prefixcodenum = X1 or s.suffixCodeNum = X2 )*/ then 1 else 0 end as float)) from atum.td_character, td_store s , ti_Item i where s.ItemNum = i.ItemNum and td_character.accountuniquenumber = s.accountuniquenumber and (td_character.uniquenumber = s.possess or s.possess = 0) group by level, unitkind order by level, unitkind");
//
//		SQLINTEGER cb1, cb2, cb3;
//		
//		SQLBindCol(m_pODBCStmt->m_hstmt, 1, SQL_C_ULONG, &tempSelectData, 0, &cb1);
//		SQLBindCol(m_pODBCStmt->m_hstmt, 2, SQL_C_ULONG, &tempSelectData2, 0, &cb2);
//		SQLBindCol(m_pODBCStmt->m_hstmt, 3, SQL_C_FLOAT, &tempSelectData3, 0, &cb3);
//	}
//	else
//	{
//		szSQLQuery.Format("select s.accountuniquenumber, avg( cast(case when i.Kind >= 40 and i.Kind < 49/* and (s.prefixcodenum = X1 or s.suffixCodeNum = X2 )*/ then 1 else 0 end as float)) From td_character, td_Store s, ti_Item i Where s.ItemNum = i.ItemNum and td_character.accountuniquenumber = s.accountuniquenumber group by s.accountuniquenumber");
//
//		SQLINTEGER cb1, cb2;
//		SQLBindCol(m_pODBCStmt->m_hstmt, 1, SQL_C_ULONG, &tempSelectData4, 0, &cb1);
//		SQLBindCol(m_pODBCStmt->m_hstmt, 2, SQL_C_FLOAT, &tempSelectData5, 0, &cb2);
//	}
//	
//	BOOL bRet = m_pODBCStmt->ExecuteQuery((LPCSTR)szSQLQuery);
//	if (!bRet)
//	{
//		// clean up
//		m_pODBCStmt->FreeStatement();
//		MessageBox("Error");
//		return;
//	}
//
//	int nCount = 0;
//	m_GridStaItemPossess.SetRowCount(nCount+1);
//	// DB에 값이 없을때까지 loop를 돈다
//	while ( (bRet = SQLFetch(m_pODBCStmt->m_hstmt)) != SQL_NO_DATA)
//	{
//		m_GridStaItemPossess.SetRowCount(nCount+2);
//
//		//select된 값을 GridDetail GridCtrl에 넣어준다.
//		if(m_valueRadioMultiShotItem == 0)
//		{
//			Item.row = nCount+1;
//			Item.col = 0;
//			Item.strText.Format("%d", tempSelectData);
//			m_GridStaItemPossess.SetItem(&Item);
//			m_GridStaItemPossess.UpdateData();
//
//			Item.col = 1;
//			Item.strText.Format("%d", tempSelectData2);
//			m_GridStaItemPossess.SetItem(&Item);
//			m_GridStaItemPossess.UpdateData();
//
//			Item.col = 2;
//			Item.strText.Format("%f", tempSelectData3);
//			m_GridStaItemPossess.SetItem(&Item);
//			m_GridStaItemPossess.UpdateData();
//		}
//		else
//		{
//			Item.row = nCount+1;
//			Item.col = 0;
//			Item.strText.Format("%d", tempSelectData4);
//			m_GridStaItemPossess.SetItem(&Item);
//			m_GridStaItemPossess.UpdateData();
//
//			Item.col = 1;
//			Item.strText.Format("%f", tempSelectData5);
//			m_GridStaItemPossess.SetItem(&Item);
//			m_GridStaItemPossess.UpdateData();
//		}
//		nCount++;
//	}
//
//	// clean up
//	m_pODBCStmt->FreeStatement();
//
//	m_GridStaItemPossess.AutoSize();
//}
//
//void CStatisticPage6Dlg::ViewEnchantCardAvg()
//{
//	m_GridStaItemPossess.SetBkColor(0xFFFFFF);
//	m_GridStaItemPossess.SetHeaderSort();
//
//	int m_nRows = 1;
//	int m_nCols;
//	if(m_valueRadioEnchantCard == 0)
//		m_nCols = 3;
//	else
//		m_nCols = 2;
//
//	m_GridStaItemPossess.SetEditable(FALSE);
//	m_GridStaItemPossess.SetListMode(TRUE);
//	m_GridStaItemPossess.SetSingleRowSelection(TRUE);
//	m_GridStaItemPossess.EnableSelection(TRUE);
//	m_GridStaItemPossess.SetFrameFocusCell(FALSE);
//	m_GridStaItemPossess.SetTrackFocusCell(FALSE);
//
//	m_GridStaItemPossess.SetRowCount(m_nRows);
//	m_GridStaItemPossess.SetColumnCount(m_nCols);
//	m_GridStaItemPossess.SetFixedRowCount(1);
//
//	// 칼럼 만들기
//	m_nCols = 0;
//	GV_ITEM Item;
//	Item.mask = GVIF_TEXT|GVIF_FORMAT;
//	Item.row = 0;
//	Item.nFormat = DT_LEFT|DT_VCENTER|DT_SINGLELINE;
//
//	if(m_valueRadioEnchantCard == 0)
//	{
//		Item.col = m_nCols++;
//		Item.strText.Format("LEVEL");
//		m_GridStaItemPossess.SetItem(&Item);
//
//		Item.col = m_nCols++;
//		Item.strText.Format("UNITKIND");
//		m_GridStaItemPossess.SetItem(&Item);
//
//		Item.col = m_nCols++;
//		Item.strText.Format("AVG MONEY");
//		m_GridStaItemPossess.SetItem(&Item);
//	}
//	else
//	{
//		Item.col = m_nCols++;
//		Item.strText.Format("ACCOUNT UNIQUE NUMBER");
//		m_GridStaItemPossess.SetItem(&Item);
//
//		Item.col = m_nCols++;
//		Item.strText.Format("AVG MONEY");
//		m_GridStaItemPossess.SetItem(&Item);
//	}
//
//	//데이터 insert
//	//DB 접속
//	if (!m_pODBCStmt->Init(m_pMainDlg->m_pServerInfo4Admin->DBIP, m_pMainDlg->m_pServerInfo4Admin->DBPort, m_pMainDlg->m_pServerInfo4Admin->DBName,
//							m_pMainDlg->m_pServerInfo4Admin->DBUID, m_pMainDlg->m_pServerInfo4Admin->DBPWD, GetSafeHwnd())
//	)
//	{
//		return;
//	}
//
//	CString szSQLQuery;
//	int tempSelectData, tempSelectData2;
//	float tempSelectData3;
//
//	int tempSelectData4;
//	float tempSelectData5;
//	if(m_valueRadioEnchantCard == 0)
//	{
//		szSQLQuery.Format("select level, unitkind, avg( cast(case when i.Kind = 21 then 1 else 0 end as float)) from atum.td_character, td_store s , ti_Item i where s.ItemNum = i.ItemNum and td_character.accountuniquenumber = s.accountuniquenumber and (td_character.uniquenumber = s.possess or s.possess = 0) group by level, unitkind order by level, unitkind");
//
//		SQLINTEGER cb1, cb2, cb3;
//		
//		SQLBindCol(m_pODBCStmt->m_hstmt, 1, SQL_C_ULONG, &tempSelectData, 0, &cb1);
//		SQLBindCol(m_pODBCStmt->m_hstmt, 2, SQL_C_ULONG, &tempSelectData2, 0, &cb2);
//		SQLBindCol(m_pODBCStmt->m_hstmt, 3, SQL_C_FLOAT, &tempSelectData3, 0, &cb3);
//	}
//	else
//	{
//		szSQLQuery.Format("select s.accountuniquenumber, avg( cast(case when i.Kind = 21 then 1 else 0 end as float)) From td_character, td_Store s, ti_Item i Where s.ItemNum = i.ItemNum and td_character.accountuniquenumber = s.accountuniquenumber group by s.accountuniquenumber");
//
//		SQLINTEGER cb1, cb2;
//		SQLBindCol(m_pODBCStmt->m_hstmt, 1, SQL_C_ULONG, &tempSelectData4, 0, &cb1);
//		SQLBindCol(m_pODBCStmt->m_hstmt, 2, SQL_C_FLOAT, &tempSelectData5, 0, &cb2);
//	}
//	
//	BOOL bRet = m_pODBCStmt->ExecuteQuery((LPCSTR)szSQLQuery);
//	if (!bRet)
//	{
//		// clean up
//		m_pODBCStmt->FreeStatement();
//		MessageBox("Error");
//		return;
//	}
//
//	int nCount = 0;
//	m_GridStaItemPossess.SetRowCount(nCount+1);
//	// DB에 값이 없을때까지 loop를 돈다
//	while ( (bRet = SQLFetch(m_pODBCStmt->m_hstmt)) != SQL_NO_DATA)
//	{
//		m_GridStaItemPossess.SetRowCount(nCount+2);
//
//		//select된 값을 GridDetail GridCtrl에 넣어준다.
//		if(m_valueRadioEnchantCard == 0)
//		{
//			Item.row = nCount+1;
//			Item.col = 0;
//			Item.strText.Format("%d", tempSelectData);
//			m_GridStaItemPossess.SetItem(&Item);
//			m_GridStaItemPossess.UpdateData();
//
//			Item.col = 1;
//			Item.strText.Format("%d", tempSelectData2);
//			m_GridStaItemPossess.SetItem(&Item);
//			m_GridStaItemPossess.UpdateData();
//
//			Item.col = 2;
//			Item.strText.Format("%f", tempSelectData3);
//			m_GridStaItemPossess.SetItem(&Item);
//			m_GridStaItemPossess.UpdateData();
//		}
//		else
//		{
//			Item.row = nCount+1;
//			Item.col = 0;
//			Item.strText.Format("%d", tempSelectData4);
//			m_GridStaItemPossess.SetItem(&Item);
//			m_GridStaItemPossess.UpdateData();
//
//			Item.col = 1;
//			Item.strText.Format("%f", tempSelectData5);
//			m_GridStaItemPossess.SetItem(&Item);
//			m_GridStaItemPossess.UpdateData();
//		}
//		nCount++;
//	}
//
//	// clean up
//	m_pODBCStmt->FreeStatement();
//
//	m_GridStaItemPossess.AutoSize();
//}
//
//void CStatisticPage6Dlg::ViewAvgMissionClear()
//{
//	m_GridStaItemPossess.SetBkColor(0xFFFFFF);
//	m_GridStaItemPossess.SetHeaderSort();
//
//	int m_nRows = 1;
//	int m_nCols;
//	if(m_valueRadioAvgMissionClear == 0)
//		m_nCols = 3;
//	else
//		m_nCols = 2;
//
//	m_GridStaItemPossess.SetEditable(FALSE);
//	m_GridStaItemPossess.SetListMode(TRUE);
//	m_GridStaItemPossess.SetSingleRowSelection(TRUE);
//	m_GridStaItemPossess.EnableSelection(TRUE);
//	m_GridStaItemPossess.SetFrameFocusCell(FALSE);
//	m_GridStaItemPossess.SetTrackFocusCell(FALSE);
//
//	m_GridStaItemPossess.SetRowCount(m_nRows);
//	m_GridStaItemPossess.SetColumnCount(m_nCols);
//	m_GridStaItemPossess.SetFixedRowCount(1);
//
//	// 칼럼 만들기
//	m_nCols = 0;
//	GV_ITEM Item;
//	Item.mask = GVIF_TEXT|GVIF_FORMAT;
//	Item.row = 0;
//	Item.nFormat = DT_LEFT|DT_VCENTER|DT_SINGLELINE;
//
//	if(m_valueRadioAvgMissionClear == 0)
//	{
//		Item.col = m_nCols++;
//		Item.strText.Format("LEVEL");
//		m_GridStaItemPossess.SetItem(&Item);
//
//		Item.col = m_nCols++;
//		Item.strText.Format("UNITKIND");
//		m_GridStaItemPossess.SetItem(&Item);
//
//		Item.col = m_nCols++;
//		Item.strText.Format("AVG MONEY");
//		m_GridStaItemPossess.SetItem(&Item);
//	}
//	else
//	{
//		Item.col = m_nCols++;
//		Item.strText.Format("ACCOUNT UNIQUE NUMBER");
//		m_GridStaItemPossess.SetItem(&Item);
//
//		Item.col = m_nCols++;
//		Item.strText.Format("AVG MONEY");
//		m_GridStaItemPossess.SetItem(&Item);
//	}
//
//	//데이터 insert
//	//DB 접속
//	if (!m_pODBCStmt->Init(m_pMainDlg->m_pServerInfo4Admin->DBIP, m_pMainDlg->m_pServerInfo4Admin->DBPort, m_pMainDlg->m_pServerInfo4Admin->DBName,
//							m_pMainDlg->m_pServerInfo4Admin->DBUID, m_pMainDlg->m_pServerInfo4Admin->DBPWD, GetSafeHwnd())
//	)
//	{
//		return;
//	}
//
//	CString szSQLQuery;
//	int tempSelectData, tempSelectData2;
//	float tempSelectData3;
//
//	int tempSelectData4;
//	float tempSelectData5;
//	if(m_valueRadioAvgMissionClear == 0)
//	{
//		szSQLQuery.Format("select level, unitkind, avg( cast(case when td_CharacterQuest.characteruniquenumber >0 then 1 else 0 end as float)) from atum.td_character left outer join td_CharacterQuest on td_character.uniquenumber = td_CharacterQuest.CharacterUniqueNumber group by level, unitkind order by level, unitkind");
//
//		SQLINTEGER cb1, cb2, cb3;
//		
//		SQLBindCol(m_pODBCStmt->m_hstmt, 1, SQL_C_ULONG, &tempSelectData, 0, &cb1);
//		SQLBindCol(m_pODBCStmt->m_hstmt, 2, SQL_C_ULONG, &tempSelectData2, 0, &cb2);
//		SQLBindCol(m_pODBCStmt->m_hstmt, 3, SQL_C_FLOAT, &tempSelectData3, 0, &cb3);
//	}
//	else
//	{
//		szSQLQuery.Format("select td_character.accountuniquenumber, sum(case when td_CharacterQuest.characteruniquenumber >0 then 1 else 0 end) from td_character left outer join td_CharacterQuest on td_character.uniquenumber = td_CharacterQuest.CharacterUniqueNumber group by td_character.accountuniquenumber");
//
//		SQLINTEGER cb1, cb2;
//		SQLBindCol(m_pODBCStmt->m_hstmt, 1, SQL_C_ULONG, &tempSelectData4, 0, &cb1);
//		SQLBindCol(m_pODBCStmt->m_hstmt, 2, SQL_C_FLOAT, &tempSelectData5, 0, &cb2);
//	}
//	
//	BOOL bRet = m_pODBCStmt->ExecuteQuery((LPCSTR)szSQLQuery);
//	if (!bRet)
//	{
//		// clean up
//		m_pODBCStmt->FreeStatement();
//		MessageBox("Error");
//		return;
//	}
//
//	int nCount = 0;
//	m_GridStaItemPossess.SetRowCount(nCount+1);
//	// DB에 값이 없을때까지 loop를 돈다
//	while ( (bRet = SQLFetch(m_pODBCStmt->m_hstmt)) != SQL_NO_DATA)
//	{
//		m_GridStaItemPossess.SetRowCount(nCount+2);
//
//		//select된 값을 GridDetail GridCtrl에 넣어준다.
//		if(m_valueRadioAvgMissionClear == 0)
//		{
//			Item.row = nCount+1;
//			Item.col = 0;
//			Item.strText.Format("%d", tempSelectData);
//			m_GridStaItemPossess.SetItem(&Item);
//			m_GridStaItemPossess.UpdateData();
//
//			Item.col = 1;
//			Item.strText.Format("%d", tempSelectData2);
//			m_GridStaItemPossess.SetItem(&Item);
//			m_GridStaItemPossess.UpdateData();
//
//			Item.col = 2;
//			Item.strText.Format("%f", tempSelectData3);
//			m_GridStaItemPossess.SetItem(&Item);
//			m_GridStaItemPossess.UpdateData();
//		}
//		else
//		{
//			Item.row = nCount+1;
//			Item.col = 0;
//			Item.strText.Format("%d", tempSelectData4);
//			m_GridStaItemPossess.SetItem(&Item);
//			m_GridStaItemPossess.UpdateData();
//
//			Item.col = 1;
//			Item.strText.Format("%f", tempSelectData5);
//			m_GridStaItemPossess.SetItem(&Item);
//			m_GridStaItemPossess.UpdateData();
//		}
//		nCount++;
//	}
//
//	// clean up
//	m_pODBCStmt->FreeStatement();
//
//	m_GridStaItemPossess.AutoSize();
//}

void CStatisticPage6Dlg::OnButtonSt5Save() 
{
	// TODO: Add your control notification handler code here
	CString tempStr;
	CString strTm;
	GetDlgItem(IDC_STATIC_1)->GetWindowText(strTm);
	tempStr.Format("%s	%10.0f", strTm, m_ctl_doTotalMoney);
	m_pMainDlg->m_pWriteLog->WriteLogFile((char*)(LPCTSTR)tempStr, tempStr.GetLength(), m_pMainDlg, 1);

	for(int i = 0; i<m_GridStaItemPossess.GetRowCount();i++)
	{
		tempStr.Empty();
		for(int j = 0; j< m_GridStaItemPossess.GetColumnCount();j++)
		{
			tempStr += m_GridStaItemPossess.GetItemText( i, j);
			tempStr += "|\t";
		}
		m_pMainDlg->m_pWriteLog->WriteLogFile((char*)(LPCTSTR)tempStr, tempStr.GetLength(), m_pMainDlg, 1);
	}
}

void CStatisticPage6Dlg::OnCheckAccount() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if(m_ctl_bAccountRank)
	{
		m_ctl_bCharacterRank = FALSE;
	}
	UpdateData(FALSE);
}

void CStatisticPage6Dlg::OnCheckCharacter() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if(m_ctl_bCharacterRank)
	{
		m_ctl_bAccountRank = FALSE;
	}
	UpdateData(FALSE);
}