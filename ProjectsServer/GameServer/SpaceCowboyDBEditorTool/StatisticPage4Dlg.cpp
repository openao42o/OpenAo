// StatisticPage4Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "spacecowboydbeditortool.h"
#include "StatisticPage4Dlg.h"
#include "SpaceCowboyDBEditorToolDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStatisticPage4Dlg property page

IMPLEMENT_DYNCREATE(CStatisticPage4Dlg, CPropertyPage)

CStatisticPage4Dlg::CStatisticPage4Dlg() : CPropertyPage(CStatisticPage4Dlg::IDD)
{
	//{{AFX_DATA_INIT(CStatisticPage4Dlg)
	m_ctl_nRankCount = 100;
	m_ctl_Date = COleDateTime::GetCurrentTime();
	m_ctl_Time = COleDateTime::GetCurrentTime();
	m_ctl_CreatedDateStart = COleDateTime::GetCurrentTime();
	m_ctl_CreatedTimeStart = COleDateTime::GetCurrentTime();
	m_ctl_CreatedDateEnd = COleDateTime::GetCurrentTime();
	m_ctl_CreatedTimeEnd = COleDateTime::GetCurrentTime();
	m_ctl_bCheckLastLoginDate = FALSE;
	m_ctl_bByCreated = FALSE;
	m_ctl_bDistChart = FALSE;
	//}}AFX_DATA_INIT
	m_pMainDlg = (CSpaceCowboyDBEditorToolDlg*)AfxGetMainWnd();
	m_pODBCStmt				= m_pMainDlg->m_pODBCStmt;
}

CStatisticPage4Dlg::~CStatisticPage4Dlg()
{
// 2006-01-23 by cmkwon
//	util::del(m_pODBCStmt);
}

void CStatisticPage4Dlg::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStatisticPage4Dlg)
	DDX_Text(pDX, IDC_EDIT_RANK_COUNT, m_ctl_nRankCount);
	DDV_MinMaxInt(pDX, m_ctl_nRankCount, 1, 2000);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_DATE, m_ctl_Date);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_TIME, m_ctl_Time);
	DDX_DateTimeCtrl(pDX, IDC_DATEPICKER_CREATED_START, m_ctl_CreatedDateStart);
	DDX_DateTimeCtrl(pDX, IDC_TIMEPICKER_CREATED_START, m_ctl_CreatedTimeStart);
	DDX_DateTimeCtrl(pDX, IDC_DATEPICKER_CREATED_END, m_ctl_CreatedDateEnd);
	DDX_DateTimeCtrl(pDX, IDC_TIMEPICKER_CREATED_END, m_ctl_CreatedTimeEnd);
	DDX_Check(pDX, IDC_CHECK2, m_ctl_bCheckLastLoginDate);
	DDX_Check(pDX, IDC_CHECK_BY_CREATED, m_ctl_bByCreated);
	DDX_Control(pDX, IDC_CUSTOM_STA_GEARLEVEL_DISTRIBUTION, m_GridStaGearLevelDistribution);
	DDX_Check(pDX, IDC_CHECK_DIST, m_ctl_bDistChart);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CStatisticPage4Dlg, CPropertyPage)
	//{{AFX_MSG_MAP(CStatisticPage4Dlg)
	ON_BN_CLICKED(IDC_BUTTON_ST4_SELECT, OnButtonSt4Select)
	ON_BN_CLICKED(IDC_BUTTON_ST4_SAVE, OnButtonSt4Save)
	ON_BN_CLICKED(IDC_CHECK1, OnCheck1)
	ON_BN_CLICKED(IDC_CHECK2, OnCheck2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStatisticPage4Dlg message handlers

void CStatisticPage4Dlg::OnButtonSt4Select() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	if(FALSE == m_ctl_bDistChart)
	{
		if(FALSE == m_ctl_bByCreated)
		{
			ViewRanking();
		}
		else
		{
			ViewRankingByCreatedDate();
		}
	}
	else
	{
		if(FALSE == m_ctl_bByCreated)
		{
			ViewDistributionByLastLoginDate();
		}
		else
		{
			ViewDistributionByCreatedDate();
		}
	}
}

void CStatisticPage4Dlg::ViewRanking(void)
{
	m_GridStaGearLevelDistribution.SetBkColor(0xFFFFFF);
	m_GridStaGearLevelDistribution.SetHeaderSort();

	m_GridStaGearLevelDistribution.SetEditable(FALSE);
	m_GridStaGearLevelDistribution.SetListMode(TRUE);
	m_GridStaGearLevelDistribution.SetSingleRowSelection(TRUE);
	m_GridStaGearLevelDistribution.EnableSelection(TRUE);
	m_GridStaGearLevelDistribution.SetFrameFocusCell(FALSE);
	m_GridStaGearLevelDistribution.SetTrackFocusCell(FALSE);

	m_GridStaGearLevelDistribution.SetRowCount(1);
	m_GridStaGearLevelDistribution.SetColumnCount(9);
	m_GridStaGearLevelDistribution.SetFixedRowCount(1);

	// 칼럼 만들기
	GV_ITEM Item;
	Item.mask = GVIF_TEXT|GVIF_FORMAT;
	Item.nFormat = DT_LEFT|DT_VCENTER|DT_SINGLELINE;
	Item.row = 0;

	Item.col = 0;
	Item.strText.Format("Num");
	m_GridStaGearLevelDistribution.SetItem(&Item);
	Item.col = 1;
	Item.strText.Format("Account Name");
	m_GridStaGearLevelDistribution.SetItem(&Item);
	Item.col = 2;
	Item.strText.Format("Character Name");
	m_GridStaGearLevelDistribution.SetItem(&Item);
	Item.col = 3;
	Item.strText.Format("Unitkind");
	m_GridStaGearLevelDistribution.SetItem(&Item);
	Item.col = 4;
	Item.strText.Format("InfluenceType");
	m_GridStaGearLevelDistribution.SetItem(&Item);
	Item.col = 5;
	Item.strText.Format("Level");
	m_GridStaGearLevelDistribution.SetItem(&Item);
	Item.col = 6;
	Item.strText.Format("Experience");
	m_GridStaGearLevelDistribution.SetItem(&Item);
	Item.col = 7;
	Item.strText.Format("Total Play Time");
	m_GridStaGearLevelDistribution.SetItem(&Item);
	Item.col = 8;
	Item.strText.Format("Num of character");
	m_GridStaGearLevelDistribution.SetItem(&Item);

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
	m_ctl_nRankCount = min(2000, m_ctl_nRankCount);

	CString szSQLQuery;
	// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
	//szSQLQuery.Format("drop view LevelRanking");
	szSQLQuery.Format(QUERY_080702_0166);
// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
//	BOOL bRet = m_pODBCStmt->ExecuteQuery((LPCSTR)szSQLQuery);
	SQLHSTMT hstmt = m_pODBCStmt->GetSTMTHandle();
	SQLINTEGER arrCB2[2] = {SQL_NTS,SQL_NTS};
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SZQUERY, 0, (LPSTR)(LPCSTR)szSQLQuery, 0,			&arrCB2[1]);
	BOOL bRet = m_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_080827_0122));
	m_pODBCStmt->FreeStatement();

	// 2007-08-09 by cmkwon, 삭제된 캐릭터 처리(RACE_DELETED_CHARACTER) - 검색 결과에서 제외 한다(0 = c.Race&%d AND )
	// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
	//szSQLQuery.Format("CREATE VIEW LevelRanking AS \
	//	SELECT TOP %d c.accountname, c.charactername, dbo.atum_GetGearString(c.unitkind) as unit, c.InfluenceType, c.level, c2.maxexp, c.TotalPlayTime, c2.numofcharacter \
	//	FROM td_character c, (SELECT accountuniquenumber, max(experience) as maxexp, count(*) as numofcharacter	FROM td_character WITH (NOLOCK) \
	//							WHERE 0 = Race&%d AND race < 128 and charactername not like 'loadtest%%'	group by accountuniquenumber) c2, td_account a WITH (NOLOCK) \
	//	WHERE 0 = c.Race&%d AND c.race < 128 AND c.accountuniquenumber = c2.accountuniquenumber and c.experience >= c2.maxexp \
	//		AND (c2.maxexp != 0 OR charactername in (select max(charactername) FROM td_character group by accountuniquenumber)) \
	//		AND a.accountuniquenumber = c.accountuniquenumber", m_ctl_nRankCount, RACE_DELETED_CHARACTER, RACE_DELETED_CHARACTER);
///////////////////////////////////////////////////////////////////////////////	
// 2008-07-08 by cmkwon, MySQL 지원 구현 - 
#if defined(DB_SERVER_MYSQL)
	szSQLQuery.Format(QUERY_080702_0167_1, RACE_DELETED_CHARACTER, RACE_DELETED_CHARACTER);
#else
	szSQLQuery.Format(QUERY_080702_0167_1, m_ctl_nRankCount, RACE_DELETED_CHARACTER, RACE_DELETED_CHARACTER);
#endif
	if(m_ctl_bCheckLastLoginDate)
	{
		CString strTemp;
// 2008-01-24 by cmkwon, S_A: 날짜 관련 로컬리 제이션 처리 버그 수정 - 아래와 같은 형식으로 수정
// 		strTemp.Format("%s AND a.LastLoginDate >= '%04d-%02d-%02d %02d:%02d:%02d' ORDER BY c2.maxexp desc", szSQLQuery
// 			, m_ctl_Date.GetYear(), m_ctl_Date.GetMonth(), m_ctl_Date.GetDay()
// 			, m_ctl_Time.GetHour(), m_ctl_Time.GetMinute(), m_ctl_Time.GetSecond());
		// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
		//strTemp.Format("%s AND a.LastLoginDate >= '%04d%02d%02d %02d:%02d:%02d' ORDER BY c2.maxexp desc", szSQLQuery
		//	, m_ctl_Date.GetYear(), m_ctl_Date.GetMonth(), m_ctl_Date.GetDay()
		//	, m_ctl_Time.GetHour(), m_ctl_Time.GetMinute(), m_ctl_Time.GetSecond());
// 2008-08-28 by cmkwon, MySQL 관련 날짜시간 관련 오류 수정 - 
//		strTemp.Format(QUERY_080702_0167_2_A, szSQLQuery
//			, m_ctl_Date.GetYear(), m_ctl_Date.GetMonth(), m_ctl_Date.GetDay()
//			, m_ctl_Time.GetHour(), m_ctl_Time.GetMinute(), m_ctl_Time.GetSecond());
		char tmSQLDateTime[SIZE_MAX_SQL_DATETIME_STRING];
		sprintf(tmSQLDateTime, SQL_DATETIME_STRING_FORMAT, m_ctl_Date.GetYear(), m_ctl_Date.GetMonth(), m_ctl_Date.GetDay()
			, m_ctl_Time.GetHour(), m_ctl_Time.GetMinute(), m_ctl_Time.GetSecond());
		strTemp.Format(QUERY_080702_0167_2_A, szSQLQuery, tmSQLDateTime);
		szSQLQuery = strTemp;
	}
	else
	{
		CString strTemp;
		// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
		//strTemp.Format("%s ORDER BY c2.maxexp desc", szSQLQuery);
		strTemp.Format(QUERY_080702_0167_2_B, szSQLQuery);
		szSQLQuery = strTemp;
	}

///////////////////////////////////////////////////////////////////////////////	
// 2008-07-08 by cmkwon, MySQL 지원 구현 - 
#if defined(DB_SERVER_MYSQL)
	{
		char szTm[1024];
		sprintf(szTm, QUERY_080702_0167_3, m_ctl_nRankCount);
		szSQLQuery += szTm;
	}
#endif

// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
//	bRet = m_pODBCStmt->ExecuteQuery((LPCSTR)szSQLQuery);
	arrCB2[1] = SQL_NTS;
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SZQUERY, 0, (LPSTR)(LPCSTR)szSQLQuery, 0,			&arrCB2[1]);
	bRet = m_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_080827_0122));
	m_pODBCStmt->FreeStatement();
	if(bRet == FALSE)
	{
		MessageBox("Error");
		return;
	}

	SQLINTEGER arrCB[9]={SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS, SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS};
	char szAccName[SIZE_MAX_ACCOUNT_NAME];
	char szCharName[SIZE_MAX_CHARACTER_NAME];
	char szUnitkind[10];
	BYTE byInfluenceType = 0;
	BYTE byLevel = 0;
	Experience_t exp = 0;
	int nNumOfCharacter = 0;
	LONGLONG llTotalTime = 0;
	
	
	SQLBindCol(m_pODBCStmt->m_hstmt, 1, SQL_C_CHAR, szAccName, SIZE_MAX_ACCOUNT_NAME,		&arrCB[1]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 2, SQL_C_CHAR, szCharName, SIZE_MAX_CHARACTER_NAME,	&arrCB[2]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 3, SQL_C_CHAR, szUnitkind, 10,						&arrCB[3]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 4, SQL_C_TINYINT, &byInfluenceType, 0,					&arrCB[4]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 5, SQL_C_TINYINT, &byLevel, 0,							&arrCB[5]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 6, SQL_C_DOUBLE, &exp, 0,								&arrCB[6]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 7, SQL_C_UBIGINT, &llTotalTime, 0,						&arrCB[7]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 8, SQL_C_LONG, &nNumOfCharacter, 0,					&arrCB[8]);

	// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
	//szSQLQuery.Format("SELECT * FROM LevelRanking");
// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
//	szSQLQuery.Format(QUERY_080702_0168);
//	bRet = m_pODBCStmt->ExecuteQuery((LPCSTR)szSQLQuery);
	bRet = m_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_080827_0168));
	if (!bRet)
	{
		// clean up
		m_pODBCStmt->FreeStatement();
		MessageBox("Error");
		return;
	}

	// DB에 값이 없을때까지 loop를 돈다
	while ( (bRet = SQLFetch(m_pODBCStmt->m_hstmt)) != SQL_NO_DATA)
	{
		int nNewRowIdx = m_GridStaGearLevelDistribution.GetRowCount();
		m_GridStaGearLevelDistribution.SetRowCount(nNewRowIdx+1);

		//select된 값을 GridDetail GridCtrl에 넣어준다.
		Item.row		= nNewRowIdx;
		Item.col		= 0;
		Item.strText.Format("%d", nNewRowIdx);
		m_GridStaGearLevelDistribution.SetItem(&Item);

		Item.col		= 1;
		Item.strText.Format("%s", szAccName);
		m_GridStaGearLevelDistribution.SetItem(&Item);

		Item.col		= 2;
		Item.strText.Format("%s", szCharName);
		m_GridStaGearLevelDistribution.SetItem(&Item);

		Item.col		= 3;
		Item.strText.Format("%s", szUnitkind);
		m_GridStaGearLevelDistribution.SetItem(&Item);

		Item.col		= 4;
		Item.strText.Format("%s", CAtumSJ::GetInfluenceTypeString(byInfluenceType));
		m_GridStaGearLevelDistribution.SetItem(&Item);

		Item.col		= 5;
		Item.strText.Format("%d", byLevel);
		m_GridStaGearLevelDistribution.SetItem(&Item);

		Item.col		= 6;
		Item.strText.Format("%I64d", (INT64)exp);
		m_GridStaGearLevelDistribution.SetItem(&Item);

		Item.col		= 7;
		Item.strText.Format("%-15s (%d sec)", ATUM_DATE_TIME::GetDateTimeStringFromSeconds(llTotalTime, string()), (int)llTotalTime);
		m_GridStaGearLevelDistribution.SetItem(&Item);

		Item.col		= 8;
		Item.strText.Format("%d", nNumOfCharacter);
		m_GridStaGearLevelDistribution.SetItem(&Item);
	}
	m_GridStaGearLevelDistribution.UpdateData();

	// clean up
	m_pODBCStmt->FreeStatement();

	m_GridStaGearLevelDistribution.AutoSize();
}

void CStatisticPage4Dlg::ViewRankingByCreatedDate(void)
{
	m_GridStaGearLevelDistribution.SetBkColor(0xFFFFFF);
	m_GridStaGearLevelDistribution.SetHeaderSort();

	m_GridStaGearLevelDistribution.SetEditable(FALSE);
	m_GridStaGearLevelDistribution.SetListMode(TRUE);
	m_GridStaGearLevelDistribution.SetSingleRowSelection(TRUE);
	m_GridStaGearLevelDistribution.EnableSelection(TRUE);
	m_GridStaGearLevelDistribution.SetFrameFocusCell(FALSE);
	m_GridStaGearLevelDistribution.SetTrackFocusCell(FALSE);

	m_GridStaGearLevelDistribution.SetRowCount(1);
	m_GridStaGearLevelDistribution.SetColumnCount(9);
	m_GridStaGearLevelDistribution.SetFixedRowCount(1);

	// 칼럼 만들기
	GV_ITEM Item;
	Item.mask = GVIF_TEXT|GVIF_FORMAT;
	Item.nFormat = DT_LEFT|DT_VCENTER|DT_SINGLELINE;
	Item.row = 0;

	Item.col = 0;
	Item.strText.Format("Num");
	m_GridStaGearLevelDistribution.SetItem(&Item);
	Item.col = 1;
	Item.strText.Format("Account Name");
	m_GridStaGearLevelDistribution.SetItem(&Item);
	Item.col = 2;
	Item.strText.Format("Character Name");
	m_GridStaGearLevelDistribution.SetItem(&Item);
	Item.col = 3;
	Item.strText.Format("Unitkind");
	m_GridStaGearLevelDistribution.SetItem(&Item);
	Item.col = 4;
	Item.strText.Format("InfluenceType");
	m_GridStaGearLevelDistribution.SetItem(&Item);
	Item.col = 5;
	Item.strText.Format("Level");
	m_GridStaGearLevelDistribution.SetItem(&Item);
	Item.col = 6;
	Item.strText.Format("Experience");
	m_GridStaGearLevelDistribution.SetItem(&Item);
	Item.col = 7;
	Item.strText.Format("Total Play Time");
	m_GridStaGearLevelDistribution.SetItem(&Item);
	Item.col = 8;
	Item.strText.Format("Num of character");
	m_GridStaGearLevelDistribution.SetItem(&Item);

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
	m_ctl_nRankCount = min(2000, m_ctl_nRankCount);

	CString szSQLQuery;
	// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
	//szSQLQuery.Format("drop view LevelRanking");
	szSQLQuery.Format(QUERY_080702_0169);
// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
//	BOOL bRet = m_pODBCStmt->ExecuteQuery((LPCSTR)szSQLQuery);
	SQLHSTMT hstmt = m_pODBCStmt->GetSTMTHandle();
	SQLINTEGER arrCB2[2] = {SQL_NTS,SQL_NTS};
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SZQUERY, 0, (LPSTR)(LPCSTR)szSQLQuery, 0,			&arrCB2[1]);
	BOOL bRet = m_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_080827_0122));
	m_pODBCStmt->FreeStatement();
	
	// 2007-08-09 by cmkwon, 삭제된 캐릭터 처리(RACE_DELETED_CHARACTER) - 검색 결과에서 제외 한다(0 = c.Race&%d AND )
// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
// 	szSQLQuery.Format("CREATE VIEW LevelRanking AS \
// 		SELECT TOP %d c.accountname, c.charactername, dbo.atum_GetGearString(c.unitkind) as unit, c.InfluenceType, c.level, c2.maxexp, c.TotalPlayTime, c2.numofcharacter \
// 		FROM td_character c, (SELECT accountuniquenumber, max(experience) as maxexp, count(*) as numofcharacter	FROM td_character WITH (NOLOCK) \
// 								WHERE 0 = Race&%d AND race < 128 and charactername not like 'loadtest%%' group by accountuniquenumber) c2, td_account a WITH (NOLOCK) \
// 		WHERE 0 = c.Race&%d AND c.race < 128 and c.accountuniquenumber = c2.accountuniquenumber and c.experience >= c2.maxexp \
// 			AND (c2.maxexp != 0 OR charactername in (select max(charactername) FROM td_character group by accountuniquenumber))"
// 		, m_ctl_nRankCount, RACE_DELETED_CHARACTER, RACE_DELETED_CHARACTER);
	///////////////////////////////////////////////////////////////////////////////	
	// 2008-07-08 by cmkwon, MySQL 지원 구현 - 
#if defined(DB_SERVER_MYSQL)
	szSQLQuery.Format(QUERY_080702_0170_1, RACE_DELETED_CHARACTER, RACE_DELETED_CHARACTER);
#else
	szSQLQuery.Format(QUERY_080702_0170_1, m_ctl_nRankCount, RACE_DELETED_CHARACTER, RACE_DELETED_CHARACTER);
#endif

	CString strTemp;
// 2008-01-24 by cmkwon, S_A: 날짜 관련 로컬리 제이션 처리 버그 수정 - 아래와 같은 형식으로 수정
// 	strTemp.Format("%s AND c.CreatedTime >= '%04d-%02d-%02d %02d:%02d:%02d' AND c.CreatedTime <= '%04d-%02d-%02d %02d:%02d:%02d' ORDER BY c2.maxexp desc", szSQLQuery
// 		, m_ctl_CreatedDateStart.GetYear(), m_ctl_CreatedDateStart.GetMonth(), m_ctl_CreatedDateStart.GetDay()
// 		, m_ctl_CreatedTimeStart.GetHour(), m_ctl_CreatedTimeStart.GetMinute(), m_ctl_CreatedTimeStart.GetSecond()
// 		, m_ctl_CreatedDateEnd.GetYear(), m_ctl_CreatedDateEnd.GetMonth(), m_ctl_CreatedDateEnd.GetDay()
// 		, m_ctl_CreatedTimeEnd.GetHour(), m_ctl_CreatedTimeEnd.GetMinute(), m_ctl_CreatedTimeEnd.GetSecond());

// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
// 	strTemp.Format("%s AND c.CreatedTime >= '%04d%02d%02d %02d:%02d:%02d' AND c.CreatedTime <= '%04d%02d%02d %02d:%02d:%02d' ORDER BY c2.maxexp desc", szSQLQuery
// 		, m_ctl_CreatedDateStart.GetYear(), m_ctl_CreatedDateStart.GetMonth(), m_ctl_CreatedDateStart.GetDay()
// 		, m_ctl_CreatedTimeStart.GetHour(), m_ctl_CreatedTimeStart.GetMinute(), m_ctl_CreatedTimeStart.GetSecond()
// 		, m_ctl_CreatedDateEnd.GetYear(), m_ctl_CreatedDateEnd.GetMonth(), m_ctl_CreatedDateEnd.GetDay()
// 		, m_ctl_CreatedTimeEnd.GetHour(), m_ctl_CreatedTimeEnd.GetMinute(), m_ctl_CreatedTimeEnd.GetSecond());

// 2008-08-28 by cmkwon, MySQL 관련 날짜시간 관련 오류 수정 - 
// 	strTemp.Format(QUERY_080702_0170_2, szSQLQuery
// 		, m_ctl_CreatedDateStart.GetYear(), m_ctl_CreatedDateStart.GetMonth(), m_ctl_CreatedDateStart.GetDay()
// 		, m_ctl_CreatedTimeStart.GetHour(), m_ctl_CreatedTimeStart.GetMinute(), m_ctl_CreatedTimeStart.GetSecond()
// 		, m_ctl_CreatedDateEnd.GetYear(), m_ctl_CreatedDateEnd.GetMonth(), m_ctl_CreatedDateEnd.GetDay()
// 		, m_ctl_CreatedTimeEnd.GetHour(), m_ctl_CreatedTimeEnd.GetMinute(), m_ctl_CreatedTimeEnd.GetSecond());
	char tmStartSQLDateTime[SIZE_MAX_SQL_DATETIME_STRING];
	char tmEndSQLDateTime[SIZE_MAX_SQL_DATETIME_STRING];
	sprintf(tmStartSQLDateTime, SQL_DATETIME_STRING_FORMAT, m_ctl_CreatedDateStart.GetYear(), m_ctl_CreatedDateStart.GetMonth(), m_ctl_CreatedDateStart.GetDay()
		, m_ctl_CreatedTimeStart.GetHour(), m_ctl_CreatedTimeStart.GetMinute(), m_ctl_CreatedTimeStart.GetSecond());
	sprintf(tmEndSQLDateTime, SQL_DATETIME_STRING_FORMAT, m_ctl_CreatedDateEnd.GetYear(), m_ctl_CreatedDateEnd.GetMonth(), m_ctl_CreatedDateEnd.GetDay()
 		, m_ctl_CreatedTimeEnd.GetHour(), m_ctl_CreatedTimeEnd.GetMinute(), m_ctl_CreatedTimeEnd.GetSecond());
	strTemp.Format(QUERY_080702_0170_2, szSQLQuery, tmStartSQLDateTime, tmEndSQLDateTime);

///////////////////////////////////////////////////////////////////////////////	
// 2008-07-08 by cmkwon, MySQL 지원 구현 - 
#if defined(DB_SERVER_MYSQL)
	{
		char szTm[1024];
		sprintf(szTm, QUERY_080702_0170_3, m_ctl_nRankCount);
		strTemp += szTm;
	}
#endif

	szSQLQuery = strTemp;
	DbgOut(szSQLQuery);
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

	SQLINTEGER arrCB[9]={SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS, SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS};
	char szAccName[SIZE_MAX_ACCOUNT_NAME];
	char szCharName[SIZE_MAX_CHARACTER_NAME];
	char szUnitkind[10];
	BYTE byInfluenceType = 0;
	BYTE byLevel = 0;
	Experience_t exp = 0;
	int nNumOfCharacter = 0;
	LONGLONG llTotalTime = 0;
	
	
	SQLBindCol(m_pODBCStmt->m_hstmt, 1, SQL_C_CHAR, szAccName, SIZE_MAX_ACCOUNT_NAME,		&arrCB[1]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 2, SQL_C_CHAR, szCharName, SIZE_MAX_CHARACTER_NAME,	&arrCB[2]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 3, SQL_C_CHAR, szUnitkind, 10,						&arrCB[3]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 4, SQL_C_TINYINT, &byInfluenceType, 0,					&arrCB[4]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 5, SQL_C_TINYINT, &byLevel, 0,							&arrCB[5]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 6, SQL_C_DOUBLE, &exp, 0,								&arrCB[6]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 7, SQL_C_UBIGINT, &llTotalTime, 0,						&arrCB[7]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 8, SQL_C_LONG, &nNumOfCharacter, 0,					&arrCB[8]);

	// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
	//szSQLQuery.Format("SELECT * FROM LevelRanking");
// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
//	szSQLQuery.Format(QUERY_080702_0171);
//	bRet = m_pODBCStmt->ExecuteQuery((LPCSTR)szSQLQuery);
	bRet = m_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_080827_0171));
	if (!bRet)
	{
		// clean up
		m_pODBCStmt->FreeStatement();
		MessageBox("Error");
		return;
	}

	// DB에 값이 없을때까지 loop를 돈다
	while ( (bRet = SQLFetch(m_pODBCStmt->m_hstmt)) != SQL_NO_DATA)
	{
		int nNewRowIdx = m_GridStaGearLevelDistribution.GetRowCount();
		m_GridStaGearLevelDistribution.SetRowCount(nNewRowIdx+1);

		//select된 값을 GridDetail GridCtrl에 넣어준다.
		Item.row		= nNewRowIdx;
		Item.col		= 0;
		Item.strText.Format("%d", nNewRowIdx);
		m_GridStaGearLevelDistribution.SetItem(&Item);

		Item.col		= 1;
		Item.strText.Format("%s", szAccName);
		m_GridStaGearLevelDistribution.SetItem(&Item);

		Item.col		= 2;
		Item.strText.Format("%s", szCharName);
		m_GridStaGearLevelDistribution.SetItem(&Item);

		Item.col		= 3;
		Item.strText.Format("%s", szUnitkind);
		m_GridStaGearLevelDistribution.SetItem(&Item);

		Item.col		= 4;
		Item.strText.Format("%s", CAtumSJ::GetInfluenceTypeString(byInfluenceType));
		m_GridStaGearLevelDistribution.SetItem(&Item);

		Item.col		= 5;
		Item.strText.Format("%d", byLevel);
		m_GridStaGearLevelDistribution.SetItem(&Item);

		Item.col		= 6;
		Item.strText.Format("%I64d", (INT64)exp);
		m_GridStaGearLevelDistribution.SetItem(&Item);

		Item.col		= 7;
		Item.strText.Format("%-15s (%d sec)", ATUM_DATE_TIME::GetDateTimeStringFromSeconds(llTotalTime, string()), (int)llTotalTime);
		m_GridStaGearLevelDistribution.SetItem(&Item);

		Item.col		= 8;
		Item.strText.Format("%d", nNumOfCharacter);
		m_GridStaGearLevelDistribution.SetItem(&Item);
	}
	m_GridStaGearLevelDistribution.UpdateData();

	// clean up
	m_pODBCStmt->FreeStatement();

	m_GridStaGearLevelDistribution.AutoSize();
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CStatisticPage4Dlg::ViewDistributionByLastLoginDate(void)
/// \brief		
/// \author		cmkwon
/// \date		2006-07-06 ~ 2006-07-06
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CStatisticPage4Dlg::ViewDistributionByLastLoginDate(void)
{
	m_GridStaGearLevelDistribution.SetBkColor(0xFFFFFF);
	m_GridStaGearLevelDistribution.SetHeaderSort();

	m_GridStaGearLevelDistribution.SetEditable(FALSE);
	m_GridStaGearLevelDistribution.SetListMode(TRUE);
	m_GridStaGearLevelDistribution.SetSingleRowSelection(TRUE);
	m_GridStaGearLevelDistribution.EnableSelection(TRUE);
	m_GridStaGearLevelDistribution.SetFrameFocusCell(FALSE);
	m_GridStaGearLevelDistribution.SetTrackFocusCell(FALSE);

	m_GridStaGearLevelDistribution.SetRowCount(1);
	m_GridStaGearLevelDistribution.SetColumnCount(4);
	m_GridStaGearLevelDistribution.SetFixedRowCount(1);

	// 칼럼 만들기
	GV_ITEM Item;
	Item.mask = GVIF_TEXT|GVIF_FORMAT;
	Item.nFormat = DT_LEFT|DT_VCENTER|DT_SINGLELINE;
	Item.row = 0;

	Item.col = 0;
	Item.strText.Format("Num");
	m_GridStaGearLevelDistribution.SetItem(&Item);
	Item.col = 1;
	Item.strText.Format("InfluenceType");
	m_GridStaGearLevelDistribution.SetItem(&Item);
	Item.col = 2;
	Item.strText.Format("Level");
	m_GridStaGearLevelDistribution.SetItem(&Item);
	Item.col = 3;
	Item.strText.Format("User Count");
	m_GridStaGearLevelDistribution.SetItem(&Item);

	CString szSQLQuery;
	// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
	//szSQLQuery.Format("drop view LevelRanking");
	szSQLQuery.Format(QUERY_080702_0172);
// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
//	BOOL bRet = m_pODBCStmt->ExecuteQuery((LPCSTR)szSQLQuery);
	SQLHSTMT hstmt = m_pODBCStmt->GetSTMTHandle();
	SQLINTEGER arrCB2[2] = {SQL_NTS,SQL_NTS};
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SZQUERY, 0, (LPSTR)(LPCSTR)szSQLQuery, 0,			&arrCB2[1]);
	BOOL bRet = m_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_080827_0122));
	m_pODBCStmt->FreeStatement();
	
	// 2007-08-09 by cmkwon, 삭제된 캐릭터 처리(RACE_DELETED_CHARACTER) - 검색 결과에서 제외 한다(0 = c.Race&%d AND )
// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
// 	szSQLQuery.Format("CREATE VIEW LevelRanking AS \
// 		SELECT c.InfluenceType, c.level, COUNT(c.level) AS UserCnt\
// 		FROM td_character c, td_account a WITH (NOLOCK) \
// 		WHERE 0 = c.Race&%d AND c.Race < 128 AND a.accountuniquenumber = c.accountuniquenumber", RACE_DELETED_CHARACTER);
	szSQLQuery.Format(QUERY_080702_0174_1, RACE_DELETED_CHARACTER);
	if(m_ctl_bCheckLastLoginDate)
	{
		CString strTemp;
// 2008-01-24 by cmkwon, S_A: 날짜 관련 로컬리 제이션 처리 버그 수정 - 아래와 같은 형식으로 수정
// 		strTemp.Format("%s AND a.LastLoginDate >= '%04d-%02d-%02d %02d:%02d:%02d' GROUP BY c.InfluenceType, c.level", szSQLQuery
// 			, m_ctl_Date.GetYear(), m_ctl_Date.GetMonth(), m_ctl_Date.GetDay()
// 			, m_ctl_Time.GetHour(), m_ctl_Time.GetMinute(), m_ctl_Time.GetSecond());

// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
// 		strTemp.Format("%s AND a.LastLoginDate >= '%04d%02d%02d %02d:%02d:%02d' GROUP BY c.InfluenceType, c.level", szSQLQuery
// 			, m_ctl_Date.GetYear(), m_ctl_Date.GetMonth(), m_ctl_Date.GetDay()
// 			, m_ctl_Time.GetHour(), m_ctl_Time.GetMinute(), m_ctl_Time.GetSecond());

// 2008-08-28 by cmkwon, MySQL 관련 날짜시간 관련 오류 수정 - 
//		strTemp.Format(QUERY_080702_0174_2_A, szSQLQuery
//			, m_ctl_Date.GetYear(), m_ctl_Date.GetMonth(), m_ctl_Date.GetDay()
//			, m_ctl_Time.GetHour(), m_ctl_Time.GetMinute(), m_ctl_Time.GetSecond());
		char tmSQLDateTime[SIZE_MAX_SQL_DATETIME_STRING];
		sprintf(tmSQLDateTime, SQL_DATETIME_STRING_FORMAT, m_ctl_Date.GetYear(), m_ctl_Date.GetMonth(), m_ctl_Date.GetDay()
			, m_ctl_Time.GetHour(), m_ctl_Time.GetMinute(), m_ctl_Time.GetSecond());
		strTemp.Format(QUERY_080702_0174_2_A, szSQLQuery, tmSQLDateTime);
		szSQLQuery = strTemp;
	}
	else
	{
		CString strTemp;
		// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
		//strTemp.Format("%s GROUP BY c.InfluenceType, c.level", szSQLQuery);
		strTemp.Format(QUERY_080702_0174_2_B, szSQLQuery);
		szSQLQuery = strTemp;

	}
	DBGOUT(szSQLQuery);
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

	SQLINTEGER arrCB[4]={SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS};
	BYTE byInfluenceType = 0;
	BYTE byLevel = 0;
	INT nUserCnt = 0;
	
	
	SQLBindCol(m_pODBCStmt->m_hstmt, 1, SQL_C_TINYINT, &byInfluenceType, 0,			&arrCB[1]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 2, SQL_C_TINYINT, &byLevel, 0,					&arrCB[2]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 3, SQL_C_LONG, &nUserCnt, 0,					&arrCB[3]);

	// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
	//szSQLQuery.Format("SELECT * FROM LevelRanking ORDER BY InfluenceType, Level desc");
// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
//	szSQLQuery.Format(QUERY_080702_0175);
//	bRet = m_pODBCStmt->ExecuteQuery((LPCSTR)szSQLQuery);
	bRet = m_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_080827_0175));
	if (!bRet)
	{
		m_pODBCStmt->FreeStatement();		// clean up
		MessageBox("Error");
		return;
	}

	// DB에 값이 없을때까지 loop를 돈다
	while ( (bRet = SQLFetch(m_pODBCStmt->m_hstmt)) != SQL_NO_DATA)
	{
		int nNewRowIdx = m_GridStaGearLevelDistribution.GetRowCount();
		m_GridStaGearLevelDistribution.SetRowCount(nNewRowIdx+1);

		//select된 값을 GridDetail GridCtrl에 넣어준다.
		Item.row		= nNewRowIdx;
		Item.col		= 0;
		Item.strText.Format("%d", nNewRowIdx);
		m_GridStaGearLevelDistribution.SetItem(&Item);

		Item.col		= 1;
		Item.strText.Format("%s", CAtumSJ::GetInfluenceTypeString(byInfluenceType));
		m_GridStaGearLevelDistribution.SetItem(&Item);

		Item.col		= 2;
		Item.strText.Format("%d", byLevel);
		m_GridStaGearLevelDistribution.SetItem(&Item);

		Item.col		= 3;
		Item.strText.Format("%d", nUserCnt);
		m_GridStaGearLevelDistribution.SetItem(&Item);
	}
	m_GridStaGearLevelDistribution.UpdateData();

	// clean up
	m_pODBCStmt->FreeStatement();

	m_GridStaGearLevelDistribution.AutoSize();
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CStatisticPage4Dlg::ViewDistributionByCreatedDate(void)
/// \brief		
/// \author		cmkwon
/// \date		2006-07-06 ~ 2006-07-06
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CStatisticPage4Dlg::ViewDistributionByCreatedDate(void)
{
	m_GridStaGearLevelDistribution.SetBkColor(0xFFFFFF);
	m_GridStaGearLevelDistribution.SetHeaderSort();

	m_GridStaGearLevelDistribution.SetEditable(FALSE);
	m_GridStaGearLevelDistribution.SetListMode(TRUE);
	m_GridStaGearLevelDistribution.SetSingleRowSelection(TRUE);
	m_GridStaGearLevelDistribution.EnableSelection(TRUE);
	m_GridStaGearLevelDistribution.SetFrameFocusCell(FALSE);
	m_GridStaGearLevelDistribution.SetTrackFocusCell(FALSE);

	m_GridStaGearLevelDistribution.SetRowCount(1);
	m_GridStaGearLevelDistribution.SetColumnCount(4);
	m_GridStaGearLevelDistribution.SetFixedRowCount(1);

	// 칼럼 만들기
	GV_ITEM Item;
	Item.mask = GVIF_TEXT|GVIF_FORMAT;
	Item.nFormat = DT_LEFT|DT_VCENTER|DT_SINGLELINE;
	Item.row = 0;

	Item.col = 0;
	Item.strText.Format("Num");
	m_GridStaGearLevelDistribution.SetItem(&Item);
	Item.col = 1;
	Item.strText.Format("InfluenceType");
	m_GridStaGearLevelDistribution.SetItem(&Item);
	Item.col = 2;
	Item.strText.Format("Level");
	m_GridStaGearLevelDistribution.SetItem(&Item);
	Item.col = 3;
	Item.strText.Format("User Count");
	m_GridStaGearLevelDistribution.SetItem(&Item);

	CString szSQLQuery;
	// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
	//szSQLQuery.Format("drop view LevelRanking");
	szSQLQuery.Format(QUERY_080702_0176);
// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
//	BOOL bRet = m_pODBCStmt->ExecuteQuery((LPCSTR)szSQLQuery);
	SQLHSTMT hstmt = m_pODBCStmt->GetSTMTHandle();
	SQLINTEGER arrCB2[2] = {SQL_NTS,SQL_NTS};
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SZQUERY, 0, (LPSTR)(LPCSTR)szSQLQuery, 0,			&arrCB2[1]);
	BOOL bRet = m_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_080827_0122));
	m_pODBCStmt->FreeStatement();
	
	// 2007-08-09 by cmkwon, 삭제된 캐릭터 처리(RACE_DELETED_CHARACTER) - 검색 결과에서 제외 한다(0 = c.Race&%d AND )
// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
// 	szSQLQuery.Format("CREATE VIEW LevelRanking AS \
// 		SELECT c.InfluenceType, c.level, COUNT(c.level) AS UserCnt\
// 		FROM td_character c, td_account a WITH (NOLOCK) \
// 		WHERE 0 = c.Race&%d AND c.Race < 128 AND a.accountuniquenumber = c.accountuniquenumber", RACE_DELETED_CHARACTER);
	szSQLQuery.Format(QUERY_080702_0177_1, RACE_DELETED_CHARACTER);
	CString strTemp;
// 2008-01-24 by cmkwon, S_A: 날짜 관련 로컬리 제이션 처리 버그 수정 - 아래와 같은 형식으로 수정
// 	strTemp.Format("%s AND c.CreatedTime >= '%04d-%02d-%02d %02d:%02d:%02d' AND c.CreatedTime <= '%04d-%02d-%02d %02d:%02d:%02d' GROUP BY c.InfluenceType, c.level", szSQLQuery
// 		, m_ctl_CreatedDateStart.GetYear(), m_ctl_CreatedDateStart.GetMonth(), m_ctl_CreatedDateStart.GetDay()
// 		, m_ctl_CreatedTimeStart.GetHour(), m_ctl_CreatedTimeStart.GetMinute(), m_ctl_CreatedTimeStart.GetSecond()
// 		, m_ctl_CreatedDateEnd.GetYear(), m_ctl_CreatedDateEnd.GetMonth(), m_ctl_CreatedDateEnd.GetDay()
// 		, m_ctl_CreatedTimeEnd.GetHour(), m_ctl_CreatedTimeEnd.GetMinute(), m_ctl_CreatedTimeEnd.GetSecond());

// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
// 	strTemp.Format("%s AND c.CreatedTime >= '%04d%02d%02d %02d:%02d:%02d' AND c.CreatedTime <= '%04d%02d%02d %02d:%02d:%02d' GROUP BY c.InfluenceType, c.level", szSQLQuery
// 		, m_ctl_CreatedDateStart.GetYear(), m_ctl_CreatedDateStart.GetMonth(), m_ctl_CreatedDateStart.GetDay()
// 		, m_ctl_CreatedTimeStart.GetHour(), m_ctl_CreatedTimeStart.GetMinute(), m_ctl_CreatedTimeStart.GetSecond()
// 		, m_ctl_CreatedDateEnd.GetYear(), m_ctl_CreatedDateEnd.GetMonth(), m_ctl_CreatedDateEnd.GetDay()
// 		, m_ctl_CreatedTimeEnd.GetHour(), m_ctl_CreatedTimeEnd.GetMinute(), m_ctl_CreatedTimeEnd.GetSecond());

// 2008-08-28 by cmkwon, MySQL 관련 날짜시간 관련 오류 수정 - 
//	strTemp.Format(QUERY_080702_0177_2, szSQLQuery
//		, m_ctl_CreatedDateStart.GetYear(), m_ctl_CreatedDateStart.GetMonth(), m_ctl_CreatedDateStart.GetDay()
//		, m_ctl_CreatedTimeStart.GetHour(), m_ctl_CreatedTimeStart.GetMinute(), m_ctl_CreatedTimeStart.GetSecond()
//		, m_ctl_CreatedDateEnd.GetYear(), m_ctl_CreatedDateEnd.GetMonth(), m_ctl_CreatedDateEnd.GetDay()
//		, m_ctl_CreatedTimeEnd.GetHour(), m_ctl_CreatedTimeEnd.GetMinute(), m_ctl_CreatedTimeEnd.GetSecond());
	char tmStartSQLDateTime[SIZE_MAX_SQL_DATETIME_STRING];
	char tmEndSQLDateTime[SIZE_MAX_SQL_DATETIME_STRING];
	sprintf(tmStartSQLDateTime, SQL_DATETIME_STRING_FORMAT, m_ctl_CreatedDateStart.GetYear(), m_ctl_CreatedDateStart.GetMonth(), m_ctl_CreatedDateStart.GetDay()
		, m_ctl_CreatedTimeStart.GetHour(), m_ctl_CreatedTimeStart.GetMinute(), m_ctl_CreatedTimeStart.GetSecond());
	sprintf(tmEndSQLDateTime, SQL_DATETIME_STRING_FORMAT, m_ctl_CreatedDateEnd.GetYear(), m_ctl_CreatedDateEnd.GetMonth(), m_ctl_CreatedDateEnd.GetDay()
		, m_ctl_CreatedTimeEnd.GetHour(), m_ctl_CreatedTimeEnd.GetMinute(), m_ctl_CreatedTimeEnd.GetSecond());
	strTemp.Format(QUERY_080702_0177_2, szSQLQuery, tmStartSQLDateTime, tmEndSQLDateTime);

	szSQLQuery = strTemp;
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

	SQLINTEGER arrCB[4]={SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS};
	BYTE byInfluenceType = 0;
	BYTE byLevel = 0;
	INT nUserCnt = 0;
	
	
	SQLBindCol(m_pODBCStmt->m_hstmt, 1, SQL_C_TINYINT, &byInfluenceType, 0,			&arrCB[1]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 2, SQL_C_TINYINT, &byLevel, 0,					&arrCB[2]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 3, SQL_C_LONG, &nUserCnt, 0,					&arrCB[3]);
	// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
	//szSQLQuery.Format("SELECT * FROM LevelRanking ORDER BY InfluenceType, Level desc");
// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
//	szSQLQuery.Format(QUERY_080702_0178);
//	bRet = m_pODBCStmt->ExecuteQuery((LPCSTR)szSQLQuery);
	bRet = m_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_080827_0178));
	if (!bRet)
	{

		m_pODBCStmt->FreeStatement();		// clean up
		MessageBox("Error");
		return;
	}

	// DB에 값이 없을때까지 loop를 돈다
	while ( (bRet = SQLFetch(m_pODBCStmt->m_hstmt)) != SQL_NO_DATA)
	{
		int nNewRowIdx = m_GridStaGearLevelDistribution.GetRowCount();
		m_GridStaGearLevelDistribution.SetRowCount(nNewRowIdx+1);

		//select된 값을 GridDetail GridCtrl에 넣어준다.
		Item.row		= nNewRowIdx;
		Item.col		= 0;
		Item.strText.Format("%d", nNewRowIdx);
		m_GridStaGearLevelDistribution.SetItem(&Item);

		Item.col		= 1;
		Item.strText.Format("%s", CAtumSJ::GetInfluenceTypeString(byInfluenceType));
		m_GridStaGearLevelDistribution.SetItem(&Item);

		Item.col		= 2;
		Item.strText.Format("%d", byLevel);
		m_GridStaGearLevelDistribution.SetItem(&Item);

		Item.col		= 3;
		Item.strText.Format("%d", nUserCnt);
		m_GridStaGearLevelDistribution.SetItem(&Item);
	}
	m_GridStaGearLevelDistribution.UpdateData();

	// clean up
	m_pODBCStmt->FreeStatement();

	m_GridStaGearLevelDistribution.AutoSize();
}



void CStatisticPage4Dlg::OnButtonSt4Save() 
{
	// TODO: Add your control notification handler code here
	CString tempStr;
	for(int i = 0; i<m_GridStaGearLevelDistribution.GetRowCount();i++)
	{
		tempStr.Empty();
		for(int j = 0; j< m_GridStaGearLevelDistribution.GetColumnCount();j++)
		{
			tempStr += m_GridStaGearLevelDistribution.GetItemText( i, j);
			tempStr += "|\t";
		}
		m_pMainDlg->m_pWriteLog->WriteLogFile((char*)(LPCTSTR)tempStr, tempStr.GetLength(), m_pMainDlg, 1);
	}
}

void CStatisticPage4Dlg::OnCheck1() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
}

void CStatisticPage4Dlg::OnCheck2() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
}
