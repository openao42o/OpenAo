// StatisticPage7Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "spacecowboydbeditortool.h"
#include "StatisticPage7Dlg.h"
#include "SpaceCowboyDBEditorToolDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStatisticPage7Dlg property page

IMPLEMENT_DYNCREATE(CStatisticPage7Dlg, CPropertyPage)

CStatisticPage7Dlg::CStatisticPage7Dlg() : CPropertyPage(CStatisticPage7Dlg::IDD)
{
	//{{AFX_DATA_INIT(CStatisticPage7Dlg)
	m_valueCheckDailyPK = FALSE;
	m_valueDTDailyPKStartDate = COleDateTime::GetCurrentTime();
	m_valueDTDailyPKEndDate = COleDateTime::GetCurrentTime();
	m_valueCheckPKWinLog = FALSE;
	m_valueDTPKWinLogStartDate = COleDateTime::GetCurrentTime();
	m_valueDTPKWinLogEndDate = COleDateTime::GetCurrentTime();
	m_valueCheckPKLoseLog = FALSE;
	m_valueDTPKLoseLogStartDate = COleDateTime::GetCurrentTime();
	m_valueDTPKLoseLogEndDate = COleDateTime::GetCurrentTime();
	m_valueCheckCharacterPKCnt = FALSE;
	m_valueRadioCharacterWarCntStartDate = COleDateTime::GetCurrentTime();
	m_valueRadioCharacterWarCntEndDate = COleDateTime::GetCurrentTime();
	m_valueDTDailyPKStartTime = COleDateTime::GetCurrentTime();
	m_valueDTDailyPKEndTime = COleDateTime::GetCurrentTime();
	m_valueDTPKWinLogStartTime = COleDateTime::GetCurrentTime();
	m_valueDTPKWinLogEndTime = COleDateTime::GetCurrentTime();
	m_valueDTPKLoseLogStartTime = COleDateTime::GetCurrentTime();
	m_valueDTPKLoseLogEndTime = 0;
	m_valueRadioCharacterWarCntStartTime = COleDateTime::GetCurrentTime();
	m_valueRadioCharacterWarCntEndTime = COleDateTime::GetCurrentTime();
	//}}AFX_DATA_INIT
	m_pMainDlg = (CSpaceCowboyDBEditorToolDlg*)AfxGetMainWnd();
	m_pODBCStmt				= m_pMainDlg->m_pODBCStmt;
}

CStatisticPage7Dlg::~CStatisticPage7Dlg()
{
// 2006-01-23 by cmkwon
//	util::del(m_pODBCStmt);
}

void CStatisticPage7Dlg::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStatisticPage7Dlg)
	DDX_Check(pDX, IDC_CHECK1, m_valueCheckDailyPK);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_ST3_DAILY_CONNECTION_STARTDATE, m_valueDTDailyPKStartDate);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_ST3_DAILY_CONNECTION_ENDDATE, m_valueDTDailyPKEndDate);
	DDX_Check(pDX, IDC_CHECK2, m_valueCheckPKWinLog);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_ST3_DAILY_CONNECTION_STARTDATE2, m_valueDTPKWinLogStartDate);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_ST3_DAILY_CONNECTION_ENDDATE2, m_valueDTPKWinLogEndDate);
	DDX_Check(pDX, IDC_CHECK3, m_valueCheckPKLoseLog);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_ST3_DAILY_CONNECTION_STARTDATE3, m_valueDTPKLoseLogStartDate);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_ST3_DAILY_CONNECTION_ENDDATE3, m_valueDTPKLoseLogEndDate);
	DDX_Check(pDX, IDC_CHECK4, m_valueCheckCharacterPKCnt);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_ST3_DAILY_CONNECTION_STARTDATE4, m_valueRadioCharacterWarCntStartDate);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_ST3_DAILY_CONNECTION_ENDDATE5, m_valueRadioCharacterWarCntEndDate);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_ST1_DAILY_ACCOUNT_STARTTIME5, m_valueDTDailyPKStartTime);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_ST1_DAILY_ACCOUNT_ENDTIME4, m_valueDTDailyPKEndTime);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_ST1_DAILY_ACCOUNT_STARTTIME6, m_valueDTPKWinLogStartTime);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_ST1_DAILY_ACCOUNT_ENDTIME5, m_valueDTPKWinLogEndTime);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_ST1_DAILY_ACCOUNT_STARTTIME7, m_valueDTPKLoseLogStartTime);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_ST1_DAILY_ACCOUNT_ENDTIME7, m_valueDTPKLoseLogEndTime);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_ST1_DAILY_ACCOUNT_STARTTIME8, m_valueRadioCharacterWarCntStartTime);
	DDX_Control(pDX, IDC_CUSTOM_STA_WAR, m_GridStaWar);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_ST1_DAILY_ACCOUNT_ENDTIME8, m_valueRadioCharacterWarCntEndTime);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CStatisticPage7Dlg, CPropertyPage)
	//{{AFX_MSG_MAP(CStatisticPage7Dlg)
	ON_BN_CLICKED(IDC_BUTTON_ST7_SELECT, OnButtonSt7Select)
	ON_BN_CLICKED(IDC_BUTTON_ST7_SAVE, OnButtonSt7Save)
	ON_BN_CLICKED(IDC_CHECK1, OnCheck1)
	ON_BN_CLICKED(IDC_CHECK2, OnCheck2)
	ON_BN_CLICKED(IDC_CHECK3, OnCheck3)
	ON_BN_CLICKED(IDC_CHECK4, OnCheck4)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStatisticPage7Dlg message handlers

void CStatisticPage7Dlg::OnButtonSt7Select() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if(m_valueCheckDailyPK == TRUE)
	{
		ViewDailyPK();
	}
	if(m_valueCheckPKWinLog == TRUE)
	{
		ViewPKWinLog();
	}
	if(m_valueCheckPKLoseLog == TRUE)
	{
		ViewPKLoseLog();
	}
	if(m_valueCheckCharacterPKCnt == TRUE)
	{
		ViewCharacterPKCnt();
	}
}

void CStatisticPage7Dlg::ViewDailyPK()
{
	m_GridStaWar.SetBkColor(0xFFFFFF);
	m_GridStaWar.SetHeaderSort();

	int m_nRows = 1;
	int m_nCols = 2;

	m_GridStaWar.SetEditable(FALSE);
	m_GridStaWar.SetListMode(TRUE);
	m_GridStaWar.SetSingleRowSelection(TRUE);
	m_GridStaWar.EnableSelection(TRUE);
	m_GridStaWar.SetFrameFocusCell(FALSE);
	m_GridStaWar.SetTrackFocusCell(FALSE);

	m_GridStaWar.SetRowCount(m_nRows);
	m_GridStaWar.SetColumnCount(m_nCols);
	m_GridStaWar.SetFixedRowCount(1);

	// 칼럼 만들기
	m_nCols = 0;
	GV_ITEM Item;
	Item.mask = GVIF_TEXT|GVIF_FORMAT;
	Item.row = 0;
	Item.nFormat = DT_LEFT|DT_VCENTER|DT_SINGLELINE;

	Item.col = m_nCols++;
	Item.strText.Format("DATE");
	m_GridStaWar.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("COUNT");
	m_GridStaWar.SetItem(&Item);

	//데이터 insert
	//DB 접속
// 2006-01-23 by cmkwon
//	if (!m_pODBCStmt->Init(m_pMainDlg->m_pServerInfo4Admin->DBIP, m_pMainDlg->m_pServerInfo4Admin->DBPort, m_pMainDlg->m_pServerInfo4Admin->DBName,
//							m_pMainDlg->m_pServerInfo4Admin->DBUID, m_pMainDlg->m_pServerInfo4Admin->DBPWD, GetSafeHwnd())
//	)
//	{
//		return;
//	}
	CString szStartDate, szEndDate;
// 2008-01-24 by cmkwon, S_A: 날짜 관련 로컬리 제이션 처리 버그 수정 - 아래와 같은 형식으로 수정
// 	szStartDate.Format("%04d-%02d-%02d %02d:%02d:%02d", m_valueDTDailyPKStartDate.GetYear(), m_valueDTDailyPKStartDate.GetMonth(), m_valueDTDailyPKStartDate.GetDay()
// 		, m_valueDTDailyPKStartTime.GetHour(), m_valueDTDailyPKStartTime.GetMinute(), m_valueDTDailyPKStartTime.GetSecond());
// 	szEndDate.Format("%04d-%02d-%02d %02d:%02d:%02d", m_valueDTDailyPKEndDate.GetYear(), m_valueDTDailyPKEndDate.GetMonth(), m_valueDTDailyPKEndDate.GetDay()
// 		, m_valueDTDailyPKEndTime.GetHour(), m_valueDTDailyPKEndTime.GetMinute(), m_valueDTDailyPKEndTime.GetSecond());
// 2008-08-28 by cmkwon, MySQL 관련 날짜시간 관련 오류 수정 - 
//	szStartDate.Format("%04d%02d%02d %02d:%02d:%02d", m_valueDTDailyPKStartDate.GetYear(), m_valueDTDailyPKStartDate.GetMonth(), m_valueDTDailyPKStartDate.GetDay()
	szStartDate.Format(SQL_DATETIME_STRING_FORMAT, m_valueDTDailyPKStartDate.GetYear(), m_valueDTDailyPKStartDate.GetMonth(), m_valueDTDailyPKStartDate.GetDay()
		, m_valueDTDailyPKStartTime.GetHour(), m_valueDTDailyPKStartTime.GetMinute(), m_valueDTDailyPKStartTime.GetSecond());
// 2008-08-28 by cmkwon, MySQL 관련 날짜시간 관련 오류 수정 - 
//	szEndDate.Format("%04d%02d%02d %02d:%02d:%02d", m_valueDTDailyPKEndDate.GetYear(), m_valueDTDailyPKEndDate.GetMonth(), m_valueDTDailyPKEndDate.GetDay()
	szEndDate.Format(SQL_DATETIME_STRING_FORMAT, m_valueDTDailyPKEndDate.GetYear(), m_valueDTDailyPKEndDate.GetMonth(), m_valueDTDailyPKEndDate.GetDay()
		, m_valueDTDailyPKEndTime.GetHour(), m_valueDTDailyPKEndTime.GetMinute(), m_valueDTDailyPKEndTime.GetSecond());

	CString szSQLQuery;
	// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
	//szSQLQuery.Format("DROP VIEW Temp_View");
// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
//	szSQLQuery.Format(QUERY_080702_0201);
//	BOOL bRet = m_pODBCStmt->ExecuteQuery((LPCSTR)szSQLQuery);
	BOOL bRet = m_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_080827_0201));
// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
// 	szSQLQuery.Format("CREATE VIEW Temp_View AS \
// 		SELECT * FROM  atum_backup_log_user_quest_pk_dead WITH (NOLOCK) WHERE logtype = 22 and time > '%s' and time < '%s' \
// 		UNION ALL \
// 		SELECT * FROM  atum_log_user_quest_pk_dead WITH (NOLOCK) WHERE logtype = 22 and time > '%s' and time < '%s'", szStartDate, szEndDate, szStartDate, szEndDate);
// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
	szSQLQuery.Format(QUERY_080702_0202, szStartDate, szEndDate, szStartDate, szEndDate);
//	bRet = m_pODBCStmt->ExecuteQuery((LPCSTR)szSQLQuery);
	SQLBindParameter(m_pODBCStmt->m_hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SZQUERY, 0, (LPSTR)(LPCSTR)szSQLQuery, 0, NULL);
	bRet = m_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_080827_0202));

	// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
	//szSQLQuery.Format("select date, count(*) from (select time, dbo.atum_getonlydate(time) as date	from Temp_View l where logtype = 22 and time > '%s' and time < '%s') ll group by date	order by date"
	//	, szStartDate, szEndDate);
// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
//	szSQLQuery.Format(QUERY_080702_0203, szStartDate, szEndDate);
//	bRet = m_pODBCStmt->ExecuteQuery((LPCSTR)szSQLQuery);
	SQLBindParameter(m_pODBCStmt->m_hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SQL_DATETIME_STRING, 0, (LPSTR)(LPCSTR)szStartDate, 0, NULL);
	SQLBindParameter(m_pODBCStmt->m_hstmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SQL_DATETIME_STRING, 0, (LPSTR)(LPCSTR)szEndDate, 0, NULL);
	bRet = m_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_080827_0203));
	if (!bRet)
	{
		// clean up
		m_pODBCStmt->FreeStatement();
		MessageBox("Error");
		return;
	}
	SQLINTEGER cb1, cb2;

	char tempDate[20];
	memset(tempDate, 0, sizeof(tempDate));
	int tempSelectData;
	SQLBindCol(m_pODBCStmt->m_hstmt, 1, SQL_C_CHAR, &tempDate, 20, &cb1);
	SQLBindCol(m_pODBCStmt->m_hstmt, 2, SQL_C_ULONG, &tempSelectData, 0, &cb2);



	int nCount = 0;
	m_GridStaWar.SetRowCount(nCount+1);
	// DB에 값이 없을때까지 loop를 돈다
	while ( (bRet = SQLFetch(m_pODBCStmt->m_hstmt)) != SQL_NO_DATA)
	{
		m_GridStaWar.SetRowCount(nCount+2);

		//select된 값을 GridDetail GridCtrl에 넣어준다.
		Item.col = 0;
		Item.row = nCount+1;
		Item.strText.Format("%s", tempDate);
		m_GridStaWar.SetItem(&Item);

		Item.col = 1;
		Item.strText.Format("%d", tempSelectData);
		m_GridStaWar.SetItem(&Item);
		m_GridStaWar.UpdateData();

		nCount++;
	}

	// clean up
	m_pODBCStmt->FreeStatement();

	m_GridStaWar.AutoSize();
}

void CStatisticPage7Dlg::ViewPKWinLog()
{
	m_GridStaWar.SetBkColor(0xFFFFFF);
	m_GridStaWar.SetHeaderSort();

	int m_nRows = 1;
	int m_nCols = 6;

	m_GridStaWar.SetEditable(FALSE);
	m_GridStaWar.SetListMode(TRUE);
	m_GridStaWar.SetSingleRowSelection(TRUE);
	m_GridStaWar.EnableSelection(TRUE);
	m_GridStaWar.SetFrameFocusCell(FALSE);
	m_GridStaWar.SetTrackFocusCell(FALSE);

	m_GridStaWar.SetRowCount(m_nRows);
	m_GridStaWar.SetColumnCount(m_nCols);
	m_GridStaWar.SetFixedRowCount(1);

	// 칼럼 만들기
	m_nCols = 0;
	GV_ITEM Item;
	Item.mask = GVIF_TEXT|GVIF_FORMAT;
	Item.row = 0;
	Item.nFormat = DT_LEFT|DT_VCENTER|DT_SINGLELINE;

	Item.col = m_nCols++;
	Item.strText.Format("TIME");
	m_GridStaWar.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("CHARACTER NAME 1");
	m_GridStaWar.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("LEVEL 1");
	m_GridStaWar.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("CHARACTER NAME 2");
	m_GridStaWar.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("LEVEL 2");
	m_GridStaWar.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("MAP INDEX");
	m_GridStaWar.SetItem(&Item);

	//데이터 insert
	//DB 접속
// 2006-01-23 by cmkwon
//	if (!m_pODBCStmt->Init(m_pMainDlg->m_pServerInfo4Admin->DBIP, m_pMainDlg->m_pServerInfo4Admin->DBPort, m_pMainDlg->m_pServerInfo4Admin->DBName,
//							m_pMainDlg->m_pServerInfo4Admin->DBUID, m_pMainDlg->m_pServerInfo4Admin->DBPWD, GetSafeHwnd())
//	)
//	{
//		return;
//	}
	CString szStartDate, szEndDate;
// 2008-01-24 by cmkwon, S_A: 날짜 관련 로컬리 제이션 처리 버그 수정 - 아래와 같은 형식으로 수정
// 	szStartDate.Format("%04d-%02d-%02d %02d:%02d:%02d", m_valueDTPKWinLogStartDate.GetYear(), m_valueDTPKWinLogStartDate.GetMonth(), m_valueDTPKWinLogStartDate.GetDay()
// 		, m_valueDTPKWinLogStartTime.GetHour(), m_valueDTPKWinLogStartTime.GetMinute(), m_valueDTPKWinLogStartTime.GetSecond());
// 	szEndDate.Format("%04d-%02d-%02d %02d:%02d:%02d", m_valueDTPKWinLogEndDate.GetYear(), m_valueDTPKWinLogEndDate.GetMonth(), m_valueDTPKWinLogEndDate.GetDay()
// 		, m_valueDTPKWinLogEndTime.GetHour(), m_valueDTPKWinLogEndTime.GetMinute(), m_valueDTPKWinLogEndTime.GetSecond());
// 2008-08-28 by cmkwon, MySQL 관련 날짜시간 관련 오류 수정 - 
//	szStartDate.Format("%04d%02d%02d %02d:%02d:%02d", m_valueDTPKWinLogStartDate.GetYear(), m_valueDTPKWinLogStartDate.GetMonth(), m_valueDTPKWinLogStartDate.GetDay()
	szStartDate.Format(SQL_DATETIME_STRING_FORMAT, m_valueDTPKWinLogStartDate.GetYear(), m_valueDTPKWinLogStartDate.GetMonth(), m_valueDTPKWinLogStartDate.GetDay()
		, m_valueDTPKWinLogStartTime.GetHour(), m_valueDTPKWinLogStartTime.GetMinute(), m_valueDTPKWinLogStartTime.GetSecond());
// 2008-08-28 by cmkwon, MySQL 관련 날짜시간 관련 오류 수정 - 
//	szEndDate.Format("%04d%02d%02d %02d:%02d:%02d", m_valueDTPKWinLogEndDate.GetYear(), m_valueDTPKWinLogEndDate.GetMonth(), m_valueDTPKWinLogEndDate.GetDay()
	szEndDate.Format(SQL_DATETIME_STRING_FORMAT, m_valueDTPKWinLogEndDate.GetYear(), m_valueDTPKWinLogEndDate.GetMonth(), m_valueDTPKWinLogEndDate.GetDay()
		, m_valueDTPKWinLogEndTime.GetHour(), m_valueDTPKWinLogEndTime.GetMinute(), m_valueDTPKWinLogEndTime.GetSecond());

	CString szSQLQuery;
	// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
	//szSQLQuery.Format("DROP VIEW Temp_View");
// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
//	szSQLQuery.Format(QUERY_080702_0204);
//	BOOL bRet = m_pODBCStmt->ExecuteQuery((LPCSTR)szSQLQuery);
	BOOL bRet = m_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_080827_0204));
	m_pODBCStmt->FreeStatement();
	
// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
// 	szSQLQuery.Format("CREATE VIEW Temp_View AS \
// 		SELECT * FROM  atum_backup_log_user_quest_pk_dead WITH (NOLOCK) WHERE logtype = 22 and time > '%s' and time < '%s' \
// 		UNION ALL \
// 		SELECT * FROM  atum_log_user_quest_pk_dead WITH (NOLOCK) WHERE logtype = 22 and time > '%s' and time < '%s'", szStartDate, szEndDate, szStartDate, szEndDate);
// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
	szSQLQuery.Format(QUERY_080702_0205, szStartDate, szEndDate, szStartDate, szEndDate);
//	bRet = m_pODBCStmt->ExecuteQuery((LPCSTR)szSQLQuery);
	SQLBindParameter(m_pODBCStmt->m_hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SZQUERY, 0, (LPSTR)(LPCSTR)szSQLQuery, 0, NULL);
	bRet = m_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_080827_0205));
	m_pODBCStmt->FreeStatement();

	// 2007-08-09 by cmkwon, 삭제된 캐릭터 처리(RACE_DELETED_CHARACTER) - 검색 결과에서 제외 한다(0 = c.Race&%d AND )
// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
// 	szSQLQuery.Format("select time, c1.charactername, c1.level,  c2.charactername, c2.level, l.mapindex, dbo.atum_getonlydate(time) \
// 						from Temp_View l, td_character c1, td_character c2 where logtype = 22 and time > '%s' and time < '%s' \
// 							and l.characteruniquenumber = c1.uniquenumber and l.param1 = c2.uniquenumber AND 0 = c1.Race&%d\
// 						order by time"
// 						, szStartDate, szEndDate, RACE_DELETED_CHARACTER);
// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
//	szSQLQuery.Format(QUERY_080702_0206, szStartDate, szEndDate, RACE_DELETED_CHARACTER);
	SQLBindParameter(m_pODBCStmt->m_hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SQL_DATETIME_STRING, 0, (LPSTR)(LPCSTR)szStartDate, 0, NULL);
	SQLBindParameter(m_pODBCStmt->m_hstmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SQL_DATETIME_STRING, 0, (LPSTR)(LPCSTR)szEndDate, 0, NULL);
	USHORT	tmRaceDeletedCharacter = RACE_DELETED_CHARACTER;
	SQLBindParameter(m_pODBCStmt->m_hstmt, 3, SQL_PARAM_INPUT, SQL_C_USHORT, SQL_SMALLINT, 0, 0, &tmRaceDeletedCharacter, 0, NULL);
	bRet = m_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_080827_0206));
	if (!bRet)
	{
		// clean up
		m_pODBCStmt->FreeStatement();
		MessageBox("Error");
		return;
	}

	SQLINTEGER cb1, cb2, cb3, cb4, cb5, cb6;

	char tempDate[20], tempDate2[20], tempDate3[20];
	memset(tempDate, 0, sizeof(tempDate));
	int tempSelectData, tempSelectData2, tempSelectData3;
	SQLBindCol(m_pODBCStmt->m_hstmt, 1, SQL_C_CHAR, &tempDate, 20, &cb1);
	SQLBindCol(m_pODBCStmt->m_hstmt, 2, SQL_C_CHAR, &tempDate2, 20, &cb2);
	SQLBindCol(m_pODBCStmt->m_hstmt, 3, SQL_C_ULONG, &tempSelectData, 0, &cb3);
	SQLBindCol(m_pODBCStmt->m_hstmt, 4, SQL_C_CHAR, &tempDate3, 20, &cb4);
	SQLBindCol(m_pODBCStmt->m_hstmt, 5, SQL_C_ULONG, &tempSelectData2, 0, &cb5);
	SQLBindCol(m_pODBCStmt->m_hstmt, 6, SQL_C_ULONG, &tempSelectData3, 0, &cb6);



	int nCount = 0;
	m_GridStaWar.SetRowCount(nCount+1);
	// DB에 값이 없을때까지 loop를 돈다
	while ( (bRet = SQLFetch(m_pODBCStmt->m_hstmt)) != SQL_NO_DATA)
	{
		m_GridStaWar.SetRowCount(nCount+2);

		//select된 값을 GridDetail GridCtrl에 넣어준다.
		Item.col = 0;
		Item.row = nCount+1;
		Item.strText.Format("%s", tempDate);
		m_GridStaWar.SetItem(&Item);

		Item.col = 1;
		Item.strText.Format("%s", tempDate2);
		m_GridStaWar.SetItem(&Item);

		Item.col = 2;
		Item.strText.Format("%d", tempSelectData);
		m_GridStaWar.SetItem(&Item);

		Item.col = 3;
		Item.strText.Format("%s", tempDate3);
		m_GridStaWar.SetItem(&Item);

		Item.col = 4;
		Item.strText.Format("%d", tempSelectData2);
		m_GridStaWar.SetItem(&Item);

		Item.col = 5;
		Item.strText.Format("%d", tempSelectData3);
		m_GridStaWar.SetItem(&Item);

		m_GridStaWar.UpdateData();

		nCount++;
	}

	// clean up
	m_pODBCStmt->FreeStatement();

	m_GridStaWar.AutoSize();
}

void CStatisticPage7Dlg::ViewPKLoseLog()
{
	m_GridStaWar.SetBkColor(0xFFFFFF);
	m_GridStaWar.SetHeaderSort();

	int m_nRows = 1;
	int m_nCols = 6;

	m_GridStaWar.SetEditable(FALSE);
	m_GridStaWar.SetListMode(TRUE);
	m_GridStaWar.SetSingleRowSelection(TRUE);
	m_GridStaWar.EnableSelection(TRUE);
	m_GridStaWar.SetFrameFocusCell(FALSE);
	m_GridStaWar.SetTrackFocusCell(FALSE);

	m_GridStaWar.SetRowCount(m_nRows);
	m_GridStaWar.SetColumnCount(m_nCols);
	m_GridStaWar.SetFixedRowCount(1);

	// 칼럼 만들기
	m_nCols = 0;
	GV_ITEM Item;
	Item.mask = GVIF_TEXT|GVIF_FORMAT;
	Item.row = 0;
	Item.nFormat = DT_LEFT|DT_VCENTER|DT_SINGLELINE;

	Item.col = m_nCols++;
	Item.strText.Format("TIME");
	m_GridStaWar.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("CHARACTER NAME 1");
	m_GridStaWar.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("LEVEL 1");
	m_GridStaWar.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("CHARACTER NAME 2");
	m_GridStaWar.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("LEVEL 2");
	m_GridStaWar.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("MAP INDEX");
	m_GridStaWar.SetItem(&Item);

	//데이터 insert
	//DB 접속
// 2006-01-23 by cmkwon
//	if (!m_pODBCStmt->Init(m_pMainDlg->m_pServerInfo4Admin->DBIP, m_pMainDlg->m_pServerInfo4Admin->DBPort, m_pMainDlg->m_pServerInfo4Admin->DBName,
//							m_pMainDlg->m_pServerInfo4Admin->DBUID, m_pMainDlg->m_pServerInfo4Admin->DBPWD, GetSafeHwnd())
//	)
//	{
//		return;
//	}
	CString szStartDate, szEndDate;
// 2008-01-24 by cmkwon, S_A: 날짜 관련 로컬리 제이션 처리 버그 수정 - 아래와 같은 형식으로 수정
// 	szStartDate.Format("%04d-%02d-%02d %02d:%02d:%02d", m_valueDTPKLoseLogStartDate.GetYear(), m_valueDTPKLoseLogStartDate.GetMonth(), m_valueDTPKLoseLogStartDate.GetDay()
// 		, m_valueDTPKLoseLogStartTime.GetHour(), m_valueDTPKLoseLogStartTime.GetMinute(), m_valueDTPKLoseLogStartTime.GetSecond());
// 	szEndDate.Format("%04d-%02d-%02d %02d:%02d:%02d", m_valueDTPKLoseLogEndDate.GetYear(), m_valueDTPKLoseLogEndDate.GetMonth(), m_valueDTPKLoseLogEndDate.GetDay()
// 		, m_valueDTPKLoseLogEndTime.GetHour(), m_valueDTPKLoseLogEndTime.GetMinute(), m_valueDTPKLoseLogEndTime.GetSecond());
// 2008-08-28 by cmkwon, MySQL 관련 날짜시간 관련 오류 수정 - 
//	szStartDate.Format("%04d%02d%02d %02d:%02d:%02d", m_valueDTPKLoseLogStartDate.GetYear(), m_valueDTPKLoseLogStartDate.GetMonth(), m_valueDTPKLoseLogStartDate.GetDay()
	szStartDate.Format(SQL_DATETIME_STRING_FORMAT, m_valueDTPKLoseLogStartDate.GetYear(), m_valueDTPKLoseLogStartDate.GetMonth(), m_valueDTPKLoseLogStartDate.GetDay()
		, m_valueDTPKLoseLogStartTime.GetHour(), m_valueDTPKLoseLogStartTime.GetMinute(), m_valueDTPKLoseLogStartTime.GetSecond());
// 2008-08-28 by cmkwon, MySQL 관련 날짜시간 관련 오류 수정 - 
//	szEndDate.Format("%04d%02d%02d %02d:%02d:%02d", m_valueDTPKLoseLogEndDate.GetYear(), m_valueDTPKLoseLogEndDate.GetMonth(), m_valueDTPKLoseLogEndDate.GetDay()
	szEndDate.Format(SQL_DATETIME_STRING_FORMAT, m_valueDTPKLoseLogEndDate.GetYear(), m_valueDTPKLoseLogEndDate.GetMonth(), m_valueDTPKLoseLogEndDate.GetDay()
		, m_valueDTPKLoseLogEndTime.GetHour(), m_valueDTPKLoseLogEndTime.GetMinute(), m_valueDTPKLoseLogEndTime.GetSecond());

	CString szSQLQuery;
	// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
	//szSQLQuery.Format("DROP VIEW Temp_View");
// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
//	szSQLQuery.Format(QUERY_080702_0207);
//	BOOL bRet = m_pODBCStmt->ExecuteQuery((LPCSTR)szSQLQuery);
	BOOL bRet = m_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_080827_0207));
	m_pODBCStmt->FreeStatement();
	
// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
// 	szSQLQuery.Format("CREATE VIEW Temp_View AS \
// 		SELECT * FROM  atum_backup_log_user_quest_pk_dead WITH (NOLOCK) WHERE (logtype = 22 or logtype = 23) and time > '%s' and time < '%s' \
// 		UNION ALL \
// 		SELECT * FROM  atum_log_user_quest_pk_dead WITH (NOLOCK) WHERE (logtype = 22 or logtype = 23) and time > '%s' and time < '%s'", szStartDate, szEndDate, szStartDate, szEndDate);
// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
	szSQLQuery.Format(QUERY_080702_0208, szStartDate, szEndDate, szStartDate, szEndDate);
//	bRet = m_pODBCStmt->ExecuteQuery((LPCSTR)szSQLQuery);
	SQLBindParameter(m_pODBCStmt->m_hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SZQUERY, 0, (LPSTR)(LPCSTR)szSQLQuery, 0, NULL);
	bRet = m_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_080827_0208));
	m_pODBCStmt->FreeStatement();

	// 2007-08-09 by cmkwon, 삭제된 캐릭터 처리(RACE_DELETED_CHARACTER) - 검색 결과에서 제외 한다(0 = c.Race&%d AND )		
// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
// 	szSQLQuery.Format("select time, c1.charactername, c1.level,  c2.charactername, c2.level, l.mapindex, dbo.atum_getonlydate(time) \
// 						from Temp_View l, td_character c1, td_character c2 where (logtype = 22 or logtype = 23) and time > '%s' and time < '%s' \
// 							and l.characteruniquenumber = c1.uniquenumber and l.param1 = c2.uniquenumber AND 0 = c1.Race&%d\
// 						order by time"
// 						, szStartDate, szEndDate, RACE_DELETED_CHARACTER);
// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
//	szSQLQuery.Format(QUERY_080702_0209, szStartDate, szEndDate, RACE_DELETED_CHARACTER);
	SQLBindParameter(m_pODBCStmt->m_hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SQL_DATETIME_STRING, 0, (LPSTR)(LPCSTR)szStartDate, 0, NULL);
	SQLBindParameter(m_pODBCStmt->m_hstmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SQL_DATETIME_STRING, 0, (LPSTR)(LPCSTR)szEndDate, 0, NULL);
	USHORT	tmRaceDeletedCharacter = RACE_DELETED_CHARACTER;
	SQLBindParameter(m_pODBCStmt->m_hstmt, 3, SQL_PARAM_INPUT, SQL_C_USHORT, SQL_SMALLINT, 0, 0, &tmRaceDeletedCharacter, 0, NULL);
	bRet = m_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_080827_0209));
	if (!bRet)
	{
		// clean up
		m_pODBCStmt->FreeStatement();
		MessageBox("Error");
		return;
	}

	SQLINTEGER cb1, cb2, cb3, cb4, cb5, cb6;

	char tempDate[20], tempDate2[20], tempDate3[20];
	memset(tempDate, 0, sizeof(tempDate));
	int tempSelectData, tempSelectData2, tempSelectData3;
	SQLBindCol(m_pODBCStmt->m_hstmt, 1, SQL_C_CHAR, &tempDate, 20, &cb1);
	SQLBindCol(m_pODBCStmt->m_hstmt, 2, SQL_C_CHAR, &tempDate2, 20, &cb2);
	SQLBindCol(m_pODBCStmt->m_hstmt, 3, SQL_C_ULONG, &tempSelectData, 0, &cb3);
	SQLBindCol(m_pODBCStmt->m_hstmt, 4, SQL_C_CHAR, &tempDate3, 20, &cb4);
	SQLBindCol(m_pODBCStmt->m_hstmt, 5, SQL_C_ULONG, &tempSelectData2, 0, &cb5);
	SQLBindCol(m_pODBCStmt->m_hstmt, 6, SQL_C_ULONG, &tempSelectData3, 0, &cb6);


	int nCount = 0;
	m_GridStaWar.SetRowCount(nCount+1);
	// DB에 값이 없을때까지 loop를 돈다
	while ( (bRet = SQLFetch(m_pODBCStmt->m_hstmt)) != SQL_NO_DATA)
	{
		m_GridStaWar.SetRowCount(nCount+2);

		//select된 값을 GridDetail GridCtrl에 넣어준다.
		Item.col = 0;
		Item.row = nCount+1;
		Item.strText.Format("%s", tempDate);
		m_GridStaWar.SetItem(&Item);

		Item.col = 1;
		Item.strText.Format("%s", tempDate2);
		m_GridStaWar.SetItem(&Item);

		Item.col = 2;
		Item.strText.Format("%d", tempSelectData);
		m_GridStaWar.SetItem(&Item);

		Item.col = 3;
		Item.strText.Format("%s", tempDate3);
		m_GridStaWar.SetItem(&Item);

		Item.col = 4;
		Item.strText.Format("%d", tempSelectData2);
		m_GridStaWar.SetItem(&Item);

		Item.col = 5;
		Item.strText.Format("%d", tempSelectData3);
		m_GridStaWar.SetItem(&Item);

		m_GridStaWar.UpdateData();

		nCount++;
	}

	// clean up
	m_pODBCStmt->FreeStatement();

	m_GridStaWar.AutoSize();
}

void CStatisticPage7Dlg::ViewCharacterPKCnt()
{
	m_GridStaWar.SetBkColor(0xFFFFFF);
	m_GridStaWar.SetHeaderSort();

	int m_nRows = 1;
	int m_nCols = 3;

	m_GridStaWar.SetEditable(FALSE);
	m_GridStaWar.SetListMode(TRUE);
	m_GridStaWar.SetSingleRowSelection(TRUE);
	m_GridStaWar.EnableSelection(TRUE);
	m_GridStaWar.SetFrameFocusCell(FALSE);
	m_GridStaWar.SetTrackFocusCell(FALSE);

	m_GridStaWar.SetRowCount(m_nRows);
	m_GridStaWar.SetColumnCount(m_nCols);
	m_GridStaWar.SetFixedRowCount(1);

	// 칼럼 만들기
	m_nCols = 0;
	GV_ITEM Item;
	Item.mask = GVIF_TEXT|GVIF_FORMAT;
	Item.row = 0;
	Item.nFormat = DT_LEFT|DT_VCENTER|DT_SINGLELINE;

	Item.col = m_nCols++;
	Item.strText.Format("CHARACTER NAME");
	m_GridStaWar.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("LEVEL");
	m_GridStaWar.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("COUNT");
	m_GridStaWar.SetItem(&Item);

	//데이터 insert
	//DB 접속
// 2006-01-23 by cmkwon
//	if (!m_pODBCStmt->Init(m_pMainDlg->m_pServerInfo4Admin->DBIP, m_pMainDlg->m_pServerInfo4Admin->DBPort, m_pMainDlg->m_pServerInfo4Admin->DBName,
//							m_pMainDlg->m_pServerInfo4Admin->DBUID, m_pMainDlg->m_pServerInfo4Admin->DBPWD, GetSafeHwnd())
//	)
//	{
//		return;
//	}
	CString szStartDate, szEndDate;
// 2008-01-24 by cmkwon, S_A: 날짜 관련 로컬리 제이션 처리 버그 수정 - 아래와 같은 형식으로 수정
// 	szStartDate.Format("%04d-%02d-%02d %02d:%02d:%02d", m_valueRadioCharacterWarCntStartDate.GetYear(), m_valueRadioCharacterWarCntStartDate.GetMonth(), m_valueRadioCharacterWarCntStartDate.GetDay()
// 		, m_valueRadioCharacterWarCntStartTime.GetHour(), m_valueRadioCharacterWarCntStartTime.GetMinute(), m_valueRadioCharacterWarCntStartTime.GetSecond());
// 	szEndDate.Format("%04d-%02d-%02d %02d:%02d:%02d", m_valueRadioCharacterWarCntEndDate.GetYear(), m_valueRadioCharacterWarCntEndDate.GetMonth(), m_valueRadioCharacterWarCntEndDate.GetDay()
// 		, m_valueRadioCharacterWarCntEndTime.GetHour(), m_valueRadioCharacterWarCntEndTime.GetMinute(), m_valueRadioCharacterWarCntEndTime.GetSecond());
// 2008-08-28 by cmkwon, MySQL 관련 날짜시간 관련 오류 수정 - 
//	szStartDate.Format("%04d%02d%02d %02d:%02d:%02d", m_valueRadioCharacterWarCntStartDate.GetYear(), m_valueRadioCharacterWarCntStartDate.GetMonth(), m_valueRadioCharacterWarCntStartDate.GetDay()
	szStartDate.Format(SQL_DATETIME_STRING_FORMAT, m_valueRadioCharacterWarCntStartDate.GetYear(), m_valueRadioCharacterWarCntStartDate.GetMonth(), m_valueRadioCharacterWarCntStartDate.GetDay()
		, m_valueRadioCharacterWarCntStartTime.GetHour(), m_valueRadioCharacterWarCntStartTime.GetMinute(), m_valueRadioCharacterWarCntStartTime.GetSecond());
// 2008-08-28 by cmkwon, MySQL 관련 날짜시간 관련 오류 수정 - 
//	szEndDate.Format("%04d%02d%02d %02d:%02d:%02d", m_valueRadioCharacterWarCntEndDate.GetYear(), m_valueRadioCharacterWarCntEndDate.GetMonth(), m_valueRadioCharacterWarCntEndDate.GetDay()
	szEndDate.Format(SQL_DATETIME_STRING_FORMAT, m_valueRadioCharacterWarCntEndDate.GetYear(), m_valueRadioCharacterWarCntEndDate.GetMonth(), m_valueRadioCharacterWarCntEndDate.GetDay()
		, m_valueRadioCharacterWarCntEndTime.GetHour(), m_valueRadioCharacterWarCntEndTime.GetMinute(), m_valueRadioCharacterWarCntEndTime.GetSecond());

	CString szSQLQuery;
	// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
	//szSQLQuery.Format("DROP VIEW Temp_View");
// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
//	szSQLQuery.Format(QUERY_080702_0210);
//	BOOL bRet = m_pODBCStmt->ExecuteQuery((LPCSTR)szSQLQuery);
	BOOL bRet = m_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_080827_0210));
	m_pODBCStmt->FreeStatement();
	
// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
// 	szSQLQuery.Format("CREATE VIEW Temp_View AS \
// 		SELECT * FROM  atum_backup_log_user_quest_pk_dead WITH (NOLOCK) WHERE (logtype = 22 or logtype = 23) and time > '%s' and time < '%s' \
// 		UNION ALL \
// 		SELECT * FROM  atum_log_user_quest_pk_dead WITH (NOLOCK) WHERE (logtype = 22 or logtype = 23) and time > '%s' and time < '%s'", szStartDate, szEndDate, szStartDate, szEndDate);
// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
	szSQLQuery.Format(QUERY_080702_0211, szStartDate, szEndDate, szStartDate, szEndDate);
//	bRet = m_pODBCStmt->ExecuteQuery((LPCSTR)szSQLQuery);
	SQLBindParameter(m_pODBCStmt->m_hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SZQUERY, 0, (LPSTR)(LPCSTR)szSQLQuery, 0, NULL);
	bRet = m_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_080827_0211));
	m_pODBCStmt->FreeStatement();

	// 2007-08-09 by cmkwon, 삭제된 캐릭터 처리(RACE_DELETED_CHARACTER) - 검색 결과에서 제외 한다(0 = c.Race&%d AND )		
// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
// 	szSQLQuery.Format("select c1.charactername, (select level from td_character where charactername = c1.charactername) as level, count(*) as count \
// 						from Temp_View l, td_character c1, td_character c2 where (logtype = 22 or logtype = 23) and time > '%s' and time < '%s' \
// 							and l.characteruniquenumber = c1.uniquenumber and c1.uniquenumber = c2.uniquenumber AND 0 = c1.Race&%d\
// 						group by c1.charactername \
// 						order by count desc"
// 						, szStartDate, szEndDate, RACE_DELETED_CHARACTER);
// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
//	szSQLQuery.Format(QUERY_080702_0212, szStartDate, szEndDate, RACE_DELETED_CHARACTER);
	SQLBindParameter(m_pODBCStmt->m_hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SQL_DATETIME_STRING, 0, (LPSTR)(LPCSTR)szStartDate, 0, NULL);
	SQLBindParameter(m_pODBCStmt->m_hstmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SQL_DATETIME_STRING, 0, (LPSTR)(LPCSTR)szEndDate, 0, NULL);
	USHORT	tmRaceDeletedCharacter = RACE_DELETED_CHARACTER;
	SQLBindParameter(m_pODBCStmt->m_hstmt, 3, SQL_PARAM_INPUT, SQL_C_USHORT, SQL_SMALLINT, 0, 0, &tmRaceDeletedCharacter, 0, NULL);
	bRet = m_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_080827_0212));
	if (!bRet)
	{
		// clean up
		m_pODBCStmt->FreeStatement();
		MessageBox("Error");
		return;
	}
	SQLINTEGER cb1, cb2, cb3;

	char tempDate[20];
	memset(tempDate, 0, sizeof(tempDate));
	int tempSelectData, tempSelectData2;
	SQLBindCol(m_pODBCStmt->m_hstmt, 1, SQL_C_CHAR, &tempDate, 20, &cb1);
	SQLBindCol(m_pODBCStmt->m_hstmt, 2, SQL_C_ULONG, &tempSelectData, 0, &cb2);
	SQLBindCol(m_pODBCStmt->m_hstmt, 3, SQL_C_ULONG, &tempSelectData2, 0, &cb3);



	int nCount = 0;
	m_GridStaWar.SetRowCount(nCount+1);
	// DB에 값이 없을때까지 loop를 돈다
	while ( (bRet = SQLFetch(m_pODBCStmt->m_hstmt)) != SQL_NO_DATA)
	{
		m_GridStaWar.SetRowCount(nCount+2);

		//select된 값을 GridDetail GridCtrl에 넣어준다.
		Item.col = 0;
		Item.row = nCount+1;
		Item.strText.Format("%s", tempDate);
		m_GridStaWar.SetItem(&Item);

		Item.col = 1;
		Item.strText.Format("%d", tempSelectData);
		m_GridStaWar.SetItem(&Item);

		Item.col = 2;
		Item.strText.Format("%d", tempSelectData2);
		m_GridStaWar.SetItem(&Item);

		m_GridStaWar.UpdateData();

		nCount++;
	}

	// clean up
	m_pODBCStmt->FreeStatement();

	m_GridStaWar.AutoSize();
}

void CStatisticPage7Dlg::OnButtonSt7Save() 
{
	// TODO: Add your control notification handler code here
	CString tempStr;
	for(int i = 0; i<m_GridStaWar.GetRowCount();i++)
	{
		tempStr = "";
		for(int j = 0; j< m_GridStaWar.GetColumnCount();j++)
		{
			tempStr += m_GridStaWar.GetItemText( i, j);
			tempStr += "|\t";
		}
		m_pMainDlg->m_pWriteLog->WriteLogFile((char*)(LPCTSTR)tempStr, tempStr.GetLength(), m_pMainDlg, 1);
	}
}

void CStatisticPage7Dlg::OnCheck1() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if(m_valueCheckDailyPK  == TRUE)
	{
		m_valueCheckPKWinLog = FALSE;
		m_valueCheckPKLoseLog = FALSE;
		m_valueCheckCharacterPKCnt = FALSE;
	}
	UpdateData(FALSE);
}

void CStatisticPage7Dlg::OnCheck2() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if(m_valueCheckPKWinLog   == TRUE)
	{
		m_valueCheckDailyPK = FALSE;
		m_valueCheckPKLoseLog = FALSE;
		m_valueCheckCharacterPKCnt = FALSE;
	}
	UpdateData(FALSE);
}

void CStatisticPage7Dlg::OnCheck3() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if(m_valueCheckPKLoseLog   == TRUE)
	{
		m_valueCheckPKWinLog = FALSE;
		m_valueCheckDailyPK = FALSE;
		m_valueCheckCharacterPKCnt = FALSE;
	}
	UpdateData(FALSE);
}

void CStatisticPage7Dlg::OnCheck4() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if(m_valueCheckCharacterPKCnt   == TRUE)
	{
		m_valueCheckPKWinLog = FALSE;
		m_valueCheckPKLoseLog = FALSE;
		m_valueCheckDailyPK = FALSE;
	}
	UpdateData(FALSE);
}
