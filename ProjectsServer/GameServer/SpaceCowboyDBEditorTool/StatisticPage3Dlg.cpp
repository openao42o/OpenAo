// StatisticPage3Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "spacecowboydbeditortool.h"
#include "StatisticPage3Dlg.h"
#include "SpaceCowboyDBEditorToolDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStatisticPage3Dlg property page

IMPLEMENT_DYNCREATE(CStatisticPage3Dlg, CPropertyPage)

CStatisticPage3Dlg::CStatisticPage3Dlg() : CPropertyPage(CStatisticPage3Dlg::IDD)
{
	//{{AFX_DATA_INIT(CStatisticPage3Dlg)
	m_valueCheckDailyConnection = FALSE;
	m_valueDTDailyConnectionStartDate = COleDateTime::GetCurrentTime() - COleDateTimeSpan(1, 0 , 0, 0);
	m_valueDTDailyConnectionEndDate = COleDateTime::GetCurrentTime();
	m_valueCheckByHourConnection = FALSE;
	m_valueDTByHourConnectionStartDate = COleDateTime::GetCurrentTime() - COleDateTimeSpan(1, 0 , 0, 0);
	m_valueDTByHourConnectionEndDate = COleDateTime::GetCurrentTime();
	m_valueCheckByDayConnection = FALSE;
	m_valueDTByDayConnectionStartDate = COleDateTime::GetCurrentTime() - COleDateTimeSpan(1, 0 , 0, 0);
	m_valueDTByDayConnectionEndDate = COleDateTime::GetCurrentTime();
	m_valueCheckByDayHourConnection = FALSE;
	m_valueDTByDayHourConnectionStartDate = COleDateTime::GetCurrentTime() - COleDateTimeSpan(1, 0 , 0, 0);
	m_valueDTByDayHourConnectionEndDate = COleDateTime::GetCurrentTime();
	m_valueCheckDailyConnectionSort = FALSE;
	m_valueDTByDayConnectionStartTime = COleDateTime::GetCurrentTime();
	m_valueDTByDayConnectionEndTime = COleDateTime::GetCurrentTime();
	m_valueDTDailyConnectionStartTime = COleDateTime::GetCurrentTime();
	m_valueDTDailyConnectionEndTime = COleDateTime::GetCurrentTime();
	m_valueDTByHourConnectionStartTime = COleDateTime::GetCurrentTime();
	m_valueDTByHourConnectionEndTime = COleDateTime::GetCurrentTime();
	m_valueDTByDayHourConnectionStartTime = COleDateTime::GetCurrentTime();
	m_valueDTByDayHourConnectionEndTime = COleDateTime::GetCurrentTime();
	//}}AFX_DATA_INIT
	m_pMainDlg = (CSpaceCowboyDBEditorToolDlg*)AfxGetMainWnd();
	m_pODBCStmt				= m_pMainDlg->m_pODBCStmt;
}

CStatisticPage3Dlg::~CStatisticPage3Dlg()
{
// 2006-01-23 by cmkwon
//	util::del(m_pODBCStmt);
}

void CStatisticPage3Dlg::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStatisticPage3Dlg)
	DDX_Check(pDX, IDC_CHECK_ST3_DAILY_CONNECTION, m_valueCheckDailyConnection);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_ST3_DAILY_CONNECTION_STARTDATE, m_valueDTDailyConnectionStartDate);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_ST3_DAILY_CONNECTION_ENDDATE, m_valueDTDailyConnectionEndDate);
	DDX_Check(pDX, IDC_CHECK2, m_valueCheckByHourConnection);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_ST3_DAILY_CONNECTION_STARTDATE2, m_valueDTByHourConnectionStartDate);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_ST3_DAILY_CONNECTION_ENDDATE2, m_valueDTByHourConnectionEndDate);
	DDX_Check(pDX, IDC_CHECK3, m_valueCheckByDayConnection);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_ST3_DAILY_CONNECTION_STARTDATE3, m_valueDTByDayConnectionStartDate);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_ST3_DAILY_CONNECTION_ENDDATE3, m_valueDTByDayConnectionEndDate);
	DDX_Check(pDX, IDC_CHECK4, m_valueCheckByDayHourConnection);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_ST3_DAILY_CONNECTION_STARTDATE4, m_valueDTByDayHourConnectionStartDate);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_ST3_DAILY_CONNECTION_ENDDATE4, m_valueDTByDayHourConnectionEndDate);
	DDX_Check(pDX, IDC_CHECK5, m_valueCheckDailyConnectionSort);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_ST1_DAILY_ACCOUNT_STARTTIME4, m_valueDTByDayConnectionStartTime);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_ST1_DAILY_ACCOUNT_ENDTIME3, m_valueDTByDayConnectionEndTime);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_ST1_DAILY_ACCOUNT_STARTTIME5, m_valueDTDailyConnectionStartTime);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_ST1_DAILY_ACCOUNT_ENDTIME4, m_valueDTDailyConnectionEndTime);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_ST1_DAILY_ACCOUNT_STARTTIME6, m_valueDTByHourConnectionStartTime);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_ST1_DAILY_ACCOUNT_ENDTIME5, m_valueDTByHourConnectionEndTime);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_ST1_DAILY_ACCOUNT_STARTTIME7, m_valueDTByDayHourConnectionStartTime);
	DDX_Control(pDX, IDC_CUSTOM_STA_SAMECONNECTION, m_GridStaConnection);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_ST1_DAILY_ACCOUNT_ENDTIME6, m_valueDTByDayHourConnectionEndTime);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CStatisticPage3Dlg, CPropertyPage)
	//{{AFX_MSG_MAP(CStatisticPage3Dlg)
	ON_BN_CLICKED(IDC_BUTTON_ST3_SELECT, OnButtonSt3Select)
	ON_BN_CLICKED(IDC_CHECK_ST3_DAILY_CONNECTION, OnCheckSt3DailyConnection)
	ON_BN_CLICKED(IDC_CHECK2, OnCheck2)
	ON_BN_CLICKED(IDC_CHECK3, OnCheck3)
	ON_BN_CLICKED(IDC_CHECK4, OnCheck4)
	ON_BN_CLICKED(IDC_BUTTON_ST3_SAVE, OnButtonSt3Save)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStatisticPage3Dlg message handlers

void CStatisticPage3Dlg::OnButtonSt3Select() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if(m_valueCheckDailyConnection == TRUE)
	{
		ViewDailyConnection();
	}
	if(m_valueCheckByHourConnection == TRUE)
	{
		ViewByHourConnection();
	}
	if(m_valueCheckByDayConnection == TRUE)
	{
		ViewByDayConnection();
	}
	if(m_valueCheckByDayHourConnection == TRUE)
	{
		ViewByDayHourConnection();
	}
}

void CStatisticPage3Dlg::ViewDailyConnection()
{
	m_GridStaConnection.SetBkColor(0xFFFFFF);
	m_GridStaConnection.SetHeaderSort();

	int m_nRows = 1;
	int m_nCols = 2;

	m_GridStaConnection.SetEditable(FALSE);
	m_GridStaConnection.SetListMode(TRUE);
	m_GridStaConnection.SetSingleRowSelection(TRUE);
	m_GridStaConnection.EnableSelection(TRUE);
	m_GridStaConnection.SetFrameFocusCell(FALSE);
	m_GridStaConnection.SetTrackFocusCell(FALSE);

	m_GridStaConnection.SetRowCount(m_nRows);
	m_GridStaConnection.SetColumnCount(m_nCols);
	m_GridStaConnection.SetFixedRowCount(1);

	// 칼럼 만들기
	m_nCols = 0;
	GV_ITEM Item;
	Item.mask = GVIF_TEXT|GVIF_FORMAT;
	Item.row = 0;
	Item.nFormat = DT_LEFT|DT_VCENTER|DT_SINGLELINE;

	Item.col = m_nCols++;
	Item.strText.Format("DATE");
	m_GridStaConnection.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("ACCOUNT");
	m_GridStaConnection.SetItem(&Item);

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
// 	szStartDate.Format("%04d-%02d-%02d %02d:%02d:%02d", m_valueDTDailyConnectionStartDate.GetYear(), m_valueDTDailyConnectionStartDate.GetMonth(), m_valueDTDailyConnectionStartDate.GetDay()
// 		, m_valueDTDailyConnectionStartTime.GetHour(), m_valueDTDailyConnectionStartTime.GetMinute(), m_valueDTDailyConnectionStartTime.GetSecond());
// 	szEndDate.Format("%04d-%02d-%02d %02d:%02d:%02d", m_valueDTDailyConnectionEndDate.GetYear(), m_valueDTDailyConnectionEndDate.GetMonth(), m_valueDTDailyConnectionEndDate.GetDay()
// 		, m_valueDTDailyConnectionEndTime.GetHour(), m_valueDTDailyConnectionEndTime.GetMinute(), m_valueDTDailyConnectionEndTime.GetSecond());
// 2008-08-28 by cmkwon, MySQL 관련 날짜시간 관련 오류 수정 - 
//	szStartDate.Format("%04d%02d%02d %02d:%02d:%02d", m_valueDTDailyConnectionStartDate.GetYear(), m_valueDTDailyConnectionStartDate.GetMonth(), m_valueDTDailyConnectionStartDate.GetDay()
	szStartDate.Format(SQL_DATETIME_STRING_FORMAT, m_valueDTDailyConnectionStartDate.GetYear(), m_valueDTDailyConnectionStartDate.GetMonth(), m_valueDTDailyConnectionStartDate.GetDay()
		, m_valueDTDailyConnectionStartTime.GetHour(), m_valueDTDailyConnectionStartTime.GetMinute(), m_valueDTDailyConnectionStartTime.GetSecond());
// 2008-08-28 by cmkwon, MySQL 관련 날짜시간 관련 오류 수정 - 
//	szEndDate.Format("%04d%02d%02d %02d:%02d:%02d", m_valueDTDailyConnectionEndDate.GetYear(), m_valueDTDailyConnectionEndDate.GetMonth(), m_valueDTDailyConnectionEndDate.GetDay()
	szEndDate.Format(SQL_DATETIME_STRING_FORMAT, m_valueDTDailyConnectionEndDate.GetYear(), m_valueDTDailyConnectionEndDate.GetMonth(), m_valueDTDailyConnectionEndDate.GetDay()
		, m_valueDTDailyConnectionEndTime.GetHour(), m_valueDTDailyConnectionEndTime.GetMinute(), m_valueDTDailyConnectionEndTime.GetSecond());

	CString szSQLQuery;

	// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
	//szSQLQuery.Format("DROP VIEW user_count_timeline");
	szSQLQuery.Format(QUERY_080702_0146);
// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
//	BOOL bRet = m_pODBCStmt->ExecuteQuery((LPCSTR)szSQLQuery);
	SQLHSTMT hstmt = m_pODBCStmt->GetSTMTHandle();
	SQLINTEGER arrCB2[2] = {SQL_NTS,SQL_NTS};
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SZQUERY, 0, (LPSTR)(LPCSTR)szSQLQuery, 0,			&arrCB2[1]);
	BOOL bRet = m_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_080827_0122));
	m_pODBCStmt->FreeStatement();

	// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
	//szSQLQuery.Format("DROP VIEW tm_user_count_timeline");
	szSQLQuery.Format(QUERY_080702_0147);
// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
//	bRet = m_pODBCStmt->ExecuteQuery((LPCSTR)szSQLQuery);
	arrCB2[1] = SQL_NTS;
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SZQUERY, 0, (LPSTR)(LPCSTR)szSQLQuery, 0,			&arrCB2[1]);
	bRet = m_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_080827_0122));
	m_pODBCStmt->FreeStatement();

	// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
	//szSQLQuery.Format("CREATE VIEW tm_user_count_timeline AS \
	//	SELECT * FROM atum_backup_log_server_map WITH (NOLOCK) where mapindex = 0 and time > '%s' and time < '%s' \
	//	UNION ALL \
	//	SELECT * FROM atum_log_server_map WITH (NOLOCK) where mapindex = 0 and time > '%s' and time < '%s'", szStartDate, szEndDate, szStartDate, szEndDate);
	szSQLQuery.Format(QUERY_080702_0148, szStartDate, szEndDate, szStartDate, szEndDate);
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
	//szSQLQuery.Format("create view user_count_timeline as \
	//	select time, dbo.atum_GetOnlyDate(time) as date, CAST(DATENAME(yy, time)+'-'+DATENAME(mm, time)+'-'+DATENAME(dd, time)+' '+DATENAME(hh, time)+':00' as datetime) as datehour,	DATEPART(hh, time) as hour,	sum(clientcount) as clientcount	from tm_user_count_timeline where mapindex = 0 and time > '%s' and time < '%s'	group by time"
	//	, szStartDate, szEndDate);
	szSQLQuery.Format(QUERY_080702_0149, szStartDate, szEndDate);
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
// 	szSQLQuery.Format("select time, clientcount from user_count_timeline where time > '%s' and time < '%s'"
// 		, szStartDate, szEndDate);
// 	if(m_valueCheckDailyConnectionSort == TRUE)
// 		szSQLQuery += "order by clientcount desc";
// 	else
// 		szSQLQuery += "order by time desc";
	szSQLQuery.Format(QUERY_080702_0150_1, szStartDate, szEndDate);
	if(m_valueCheckDailyConnectionSort == TRUE)
	{
		szSQLQuery += QUERY_080702_0150_2_A;
	}
	else
	{
		szSQLQuery += QUERY_080702_0150_2_B;
	}
// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
//	bRet = m_pODBCStmt->ExecuteQuery((LPCSTR)szSQLQuery);
	arrCB2[1] = SQL_NTS;
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SZQUERY, 0, (LPSTR)(LPCSTR)szSQLQuery, 0,			&arrCB2[1]);
	bRet = m_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_080827_0122));
	if (!bRet)
	{// clean up

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
	m_GridStaConnection.SetRowCount(nCount+1);
	// DB에 값이 없을때까지 loop를 돈다
	while ( (bRet = SQLFetch(m_pODBCStmt->m_hstmt)) != SQL_NO_DATA)
	{
		m_GridStaConnection.SetRowCount(nCount+2);

		//select된 값을 GridDetail GridCtrl에 넣어준다.
		Item.col = 0;
		Item.row = nCount+1;
		Item.strText.Format("%s", tempDate);
		m_GridStaConnection.SetItem(&Item);

		Item.col = 1;
		Item.strText.Format("%d", tempSelectData);
		m_GridStaConnection.SetItem(&Item);
		m_GridStaConnection.UpdateData();

		nCount++;
	}

	// clean up
	m_pODBCStmt->FreeStatement();

	m_GridStaConnection.AutoSize();
}

void CStatisticPage3Dlg::ViewByHourConnection()
{
	m_GridStaConnection.SetBkColor(0xFFFFFF);
	m_GridStaConnection.SetHeaderSort();

	int m_nRows = 1;
	int m_nCols = 4;

	m_GridStaConnection.SetEditable(FALSE);
	m_GridStaConnection.SetListMode(TRUE);
	m_GridStaConnection.SetSingleRowSelection(TRUE);
	m_GridStaConnection.EnableSelection(TRUE);
	m_GridStaConnection.SetFrameFocusCell(FALSE);
	m_GridStaConnection.SetTrackFocusCell(FALSE);

	m_GridStaConnection.SetRowCount(m_nRows);
	m_GridStaConnection.SetColumnCount(m_nCols);
	m_GridStaConnection.SetFixedRowCount(1);

	// 칼럼 만들기
	m_nCols = 0;
	GV_ITEM Item;
	Item.mask = GVIF_TEXT|GVIF_FORMAT;
	Item.row = 0;
	Item.nFormat = DT_LEFT|DT_VCENTER|DT_SINGLELINE;

	Item.col = m_nCols++;
	Item.strText.Format("HOUR");
	m_GridStaConnection.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("MAXCOUNT");
	m_GridStaConnection.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("MINCOUNT");
	m_GridStaConnection.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("AVGCOUNT");
	m_GridStaConnection.SetItem(&Item);

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
// 	szStartDate.Format("%04d-%02d-%02d %02d:%02d:%02d", m_valueDTByHourConnectionStartDate.GetYear(), m_valueDTByHourConnectionStartDate.GetMonth(), m_valueDTByHourConnectionStartDate.GetDay()
// 		, m_valueDTByHourConnectionStartTime.GetHour(), m_valueDTByHourConnectionStartTime.GetMinute(), m_valueDTByHourConnectionStartTime.GetSecond());
// 	szEndDate.Format("%04d-%02d-%02d %02d:%02d:%02d", m_valueDTByHourConnectionEndDate.GetYear(), m_valueDTByHourConnectionEndDate.GetMonth(), m_valueDTByHourConnectionEndDate.GetDay()
// 		, m_valueDTByHourConnectionEndTime.GetHour(), m_valueDTByHourConnectionEndTime.GetMinute(), m_valueDTByHourConnectionEndTime.GetSecond());
// 2008-08-28 by cmkwon, MySQL 관련 날짜시간 관련 오류 수정 - 
//	szStartDate.Format("%04d%02d%02d %02d:%02d:%02d", m_valueDTByHourConnectionStartDate.GetYear(), m_valueDTByHourConnectionStartDate.GetMonth(), m_valueDTByHourConnectionStartDate.GetDay()
	szStartDate.Format(SQL_DATETIME_STRING_FORMAT, m_valueDTByHourConnectionStartDate.GetYear(), m_valueDTByHourConnectionStartDate.GetMonth(), m_valueDTByHourConnectionStartDate.GetDay()
		, m_valueDTByHourConnectionStartTime.GetHour(), m_valueDTByHourConnectionStartTime.GetMinute(), m_valueDTByHourConnectionStartTime.GetSecond());
// 2008-08-28 by cmkwon, MySQL 관련 날짜시간 관련 오류 수정 - 
//	szEndDate.Format("%04d%02d%02d %02d:%02d:%02d", m_valueDTByHourConnectionEndDate.GetYear(), m_valueDTByHourConnectionEndDate.GetMonth(), m_valueDTByHourConnectionEndDate.GetDay()
	szEndDate.Format(SQL_DATETIME_STRING_FORMAT, m_valueDTByHourConnectionEndDate.GetYear(), m_valueDTByHourConnectionEndDate.GetMonth(), m_valueDTByHourConnectionEndDate.GetDay()
		, m_valueDTByHourConnectionEndTime.GetHour(), m_valueDTByHourConnectionEndTime.GetMinute(), m_valueDTByHourConnectionEndTime.GetSecond());

	CString szSQLQuery;
	// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
	//szSQLQuery.Format("drop view user_count_timeline");
	szSQLQuery.Format(QUERY_080702_0151);
// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
//	BOOL bRet = m_pODBCStmt->ExecuteQuery((LPCSTR)szSQLQuery);
	SQLHSTMT hstmt = m_pODBCStmt->GetSTMTHandle();
	SQLINTEGER arrCB2[2] = {SQL_NTS,SQL_NTS};
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SZQUERY, 0, (LPSTR)(LPCSTR)szSQLQuery, 0,			&arrCB2[1]);
	BOOL bRet = m_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_080827_0122));
	m_pODBCStmt->FreeStatement();

	// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
	//szSQLQuery.Format("drop view tm_user_count_timeline");
	szSQLQuery.Format(QUERY_080702_0152);
// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
//	bRet = m_pODBCStmt->ExecuteQuery((LPCSTR)szSQLQuery);
	hstmt = m_pODBCStmt->GetSTMTHandle();
	arrCB2[1] = SQL_NTS;
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SZQUERY, 0, (LPSTR)(LPCSTR)szSQLQuery, 0,			&arrCB2[1]);
	bRet = m_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_080827_0122));
	m_pODBCStmt->FreeStatement();

// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
//	szSQLQuery.Format("CREATE VIEW tm_user_count_timeline AS \
//		SELECT * FROM atum_backup_log_server_map WITH (NOLOCK) where mapindex = 0 and time > '%s' and time < '%s' \
//		UNION ALL \
//		SELECT * FROM atum_log_server_map WITH (NOLOCK) where mapindex = 0 and time > '%s' and time < '%s'", szStartDate, szEndDate, szStartDate, szEndDate);
	szSQLQuery.Format(QUERY_080702_0153, szStartDate, szEndDate, szStartDate, szEndDate);
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
	//szSQLQuery.Format("create view user_count_timeline	as	select	time, dbo.atum_GetOnlyDate(time) as date, CAST(DATENAME(yy, time)+'-'+DATENAME(mm, time)+'-'+DATENAME(dd, time)+' '+DATENAME(hh, time)+':00' as datetime) as datehour,	DATEPART(hh, time) as hour,	sum(clientcount) as clientcount	from tm_user_count_timeline where mapindex = 0 and time > '%s' and time < '%s'	group by time"
	//	, szStartDate, szEndDate);
	szSQLQuery.Format(QUERY_080702_0154, szStartDate, szEndDate);
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
	//szSQLQuery.Format("select hour, max(clientcount) as maxcount, min(clientcount) as mincount,avg(clientcount) as avgcount from user_count_timeline group by hour order by hour");
// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
//	szSQLQuery.Format(QUERY_080702_0155);
//	bRet = m_pODBCStmt->ExecuteQuery((LPCSTR)szSQLQuery);
	bRet = m_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_080827_0155));
	if (!bRet)
	{
		// clean up
		m_pODBCStmt->FreeStatement();
		MessageBox("Error");
		return;
	}


	SQLINTEGER cb1, cb2, cb3, cb4;

	char tempDate[20];
	memset(tempDate, 0, sizeof(tempDate));
	int tempSelectData, tempSelectData2, tempSelectData3;
	SQLBindCol(m_pODBCStmt->m_hstmt, 1, SQL_C_CHAR, &tempDate, 20, &cb1);
	SQLBindCol(m_pODBCStmt->m_hstmt, 2, SQL_C_ULONG, &tempSelectData, 0, &cb2);
	SQLBindCol(m_pODBCStmt->m_hstmt, 3, SQL_C_ULONG, &tempSelectData2, 0, &cb3);
	SQLBindCol(m_pODBCStmt->m_hstmt, 4, SQL_C_ULONG, &tempSelectData3, 0, &cb4);

	int nCount = 0;
	m_GridStaConnection.SetRowCount(nCount+1);
	// DB에 값이 없을때까지 loop를 돈다
	while ( (bRet = SQLFetch(m_pODBCStmt->m_hstmt)) != SQL_NO_DATA)
	{
		m_GridStaConnection.SetRowCount(nCount+2);

		//select된 값을 GridDetail GridCtrl에 넣어준다.
		Item.col = 0;
		Item.row = nCount+1;
		Item.strText.Format("%s", tempDate);
		m_GridStaConnection.SetItem(&Item);

		Item.col = 1;
		Item.strText.Format("%d", tempSelectData);
		m_GridStaConnection.SetItem(&Item);
		m_GridStaConnection.UpdateData();

		Item.col = 2;
		Item.strText.Format("%d", tempSelectData2);
		m_GridStaConnection.SetItem(&Item);
		m_GridStaConnection.UpdateData();

		Item.col = 3;
		Item.strText.Format("%d", tempSelectData3);
		m_GridStaConnection.SetItem(&Item);
		m_GridStaConnection.UpdateData();

		nCount++;
	}

	// clean up
	m_pODBCStmt->FreeStatement();

	m_GridStaConnection.AutoSize();
}

void CStatisticPage3Dlg::ViewByDayConnection()
{
	m_GridStaConnection.SetBkColor(0xFFFFFF);
	m_GridStaConnection.SetHeaderSort();

	int m_nRows = 1;
	int m_nCols = 4;

	m_GridStaConnection.SetEditable(FALSE);
	m_GridStaConnection.SetListMode(TRUE);
	m_GridStaConnection.SetSingleRowSelection(TRUE);
	m_GridStaConnection.EnableSelection(TRUE);
	m_GridStaConnection.SetFrameFocusCell(FALSE);
	m_GridStaConnection.SetTrackFocusCell(FALSE);

	m_GridStaConnection.SetRowCount(m_nRows);
	m_GridStaConnection.SetColumnCount(m_nCols);
	m_GridStaConnection.SetFixedRowCount(1);

	// 칼럼 만들기
	m_nCols = 0;
	GV_ITEM Item;
	Item.mask = GVIF_TEXT|GVIF_FORMAT;
	Item.row = 0;
	Item.nFormat = DT_LEFT|DT_VCENTER|DT_SINGLELINE;

	Item.col = m_nCols++;
	Item.strText.Format("DATE");
	m_GridStaConnection.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("MAXCOUNT");
	m_GridStaConnection.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("MINCOUNT");
	m_GridStaConnection.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("AVGCOUNT");
	m_GridStaConnection.SetItem(&Item);

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
// 	szStartDate.Format("%04d-%02d-%02d %02d:%02d:%02d", m_valueDTByDayConnectionStartDate.GetYear(), m_valueDTByDayConnectionStartDate.GetMonth(), m_valueDTByDayConnectionStartDate.GetDay()
// 		, m_valueDTByDayConnectionStartTime.GetHour(), m_valueDTByDayConnectionStartTime.GetMinute(), m_valueDTByDayConnectionStartTime.GetSecond());
// 	szEndDate.Format("%04d-%02d-%02d %02d:%02d:%02d", m_valueDTByDayConnectionEndDate.GetYear(), m_valueDTByDayConnectionEndDate.GetMonth(), m_valueDTByDayConnectionEndDate.GetDay()
// 		, m_valueDTByDayConnectionEndTime.GetHour(), m_valueDTByDayConnectionEndTime.GetMinute(), m_valueDTByDayConnectionEndTime.GetSecond());
// 2008-08-28 by cmkwon, MySQL 관련 날짜시간 관련 오류 수정 - 
//	szStartDate.Format("%04d%02d%02d %02d:%02d:%02d", m_valueDTByDayConnectionStartDate.GetYear(), m_valueDTByDayConnectionStartDate.GetMonth(), m_valueDTByDayConnectionStartDate.GetDay()
	szStartDate.Format(SQL_DATETIME_STRING_FORMAT, m_valueDTByDayConnectionStartDate.GetYear(), m_valueDTByDayConnectionStartDate.GetMonth(), m_valueDTByDayConnectionStartDate.GetDay()
		, m_valueDTByDayConnectionStartTime.GetHour(), m_valueDTByDayConnectionStartTime.GetMinute(), m_valueDTByDayConnectionStartTime.GetSecond());
// 2008-08-28 by cmkwon, MySQL 관련 날짜시간 관련 오류 수정 - 
//	szEndDate.Format("%04d%02d%02d %02d:%02d:%02d", m_valueDTByDayConnectionEndDate.GetYear(), m_valueDTByDayConnectionEndDate.GetMonth(), m_valueDTByDayConnectionEndDate.GetDay()
	szEndDate.Format(SQL_DATETIME_STRING_FORMAT, m_valueDTByDayConnectionEndDate.GetYear(), m_valueDTByDayConnectionEndDate.GetMonth(), m_valueDTByDayConnectionEndDate.GetDay()
		, m_valueDTByDayConnectionEndTime.GetHour(), m_valueDTByDayConnectionEndTime.GetMinute(), m_valueDTByDayConnectionEndTime.GetSecond());

	CString szSQLQuery;
	
	// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
	//szSQLQuery.Format("drop view user_count_timeline");
	szSQLQuery.Format(QUERY_080702_0156);
// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
//	BOOL bRet = m_pODBCStmt->ExecuteQuery((LPCSTR)szSQLQuery);
	SQLHSTMT hstmt = m_pODBCStmt->GetSTMTHandle();
	SQLINTEGER arrCB2[2] = {SQL_NTS,SQL_NTS};
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SZQUERY, 0, (LPSTR)(LPCSTR)szSQLQuery, 0,			&arrCB2[1]);
	BOOL bRet = m_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_080827_0122));
	m_pODBCStmt->FreeStatement();

	// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
	//szSQLQuery.Format("drop view tm_user_count_timeline");
	szSQLQuery.Format(QUERY_080702_0157);
// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
//	bRet = m_pODBCStmt->ExecuteQuery((LPCSTR)szSQLQuery);
	arrCB2[1] = SQL_NTS;
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SZQUERY, 0, (LPSTR)(LPCSTR)szSQLQuery, 0,			&arrCB2[1]);
	bRet = m_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_080827_0122));
	m_pODBCStmt->FreeStatement();


	// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
	//szSQLQuery.Format("CREATE VIEW tm_user_count_timeline AS \
	//	SELECT * FROM atum_backup_log_server_map WITH (NOLOCK) where mapindex = 0 and time > '%s' and time < '%s' \
	//	UNION ALL \
	//	SELECT * FROM atum_log_server_map WITH (NOLOCK) where mapindex = 0 and time > '%s' and time < '%s'", szStartDate, szEndDate, szStartDate, szEndDate);
	szSQLQuery.Format(QUERY_080702_0158, szStartDate, szEndDate, szStartDate, szEndDate);
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
	//szSQLQuery.Format("create view user_count_timeline	as	select	time, dbo.atum_GetOnlyDate(time) as date, CAST(DATENAME(yy, time)+'-'+DATENAME(mm, time)+'-'+DATENAME(dd, time)+' '+DATENAME(hh, time)+':00' as datetime) as datehour,	DATEPART(hh, time) as hour,	sum(clientcount) as clientcount	from tm_user_count_timeline where mapindex = 0 and time > '%s' and time < '%s'	group by time"
	//	, szStartDate, szEndDate);
	szSQLQuery.Format(QUERY_080702_0159, szStartDate, szEndDate);
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
	//szSQLQuery.Format("select date, max(clientcount) as maxcount, avg(clientcount) as avgcount, min(clientcount) as mincount from user_count_timeline group by date	order by date");

// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
//	szSQLQuery.Format(QUERY_080702_0160);
//	bRet = m_pODBCStmt->ExecuteQuery((LPCSTR)szSQLQuery);
	bRet = m_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_080827_0160));
	if (!bRet)
	{
		// clean up
		m_pODBCStmt->FreeStatement();
		MessageBox("Error");
		return;
	}

	SQLINTEGER cb1, cb2, cb3, cb4;

	char tempDate[20];
	memset(tempDate, 0, sizeof(tempDate));
	int tempSelectData, tempSelectData2, tempSelectData3;
	SQLBindCol(m_pODBCStmt->m_hstmt, 1, SQL_C_CHAR, &tempDate, 20, &cb1);
	SQLBindCol(m_pODBCStmt->m_hstmt, 2, SQL_C_ULONG, &tempSelectData, 0, &cb2);
	SQLBindCol(m_pODBCStmt->m_hstmt, 3, SQL_C_ULONG, &tempSelectData2, 0, &cb3);
	SQLBindCol(m_pODBCStmt->m_hstmt, 4, SQL_C_ULONG, &tempSelectData3, 0, &cb4);


	int nCount = 0;
	m_GridStaConnection.SetRowCount(nCount+1);
	// DB에 값이 없을때까지 loop를 돈다
	while ( (bRet = SQLFetch(m_pODBCStmt->m_hstmt)) != SQL_NO_DATA)
	{
		m_GridStaConnection.SetRowCount(nCount+2);

		//select된 값을 GridDetail GridCtrl에 넣어준다.
		Item.col = 0;
		Item.row = nCount+1;
		Item.strText.Format("%s", tempDate);
		m_GridStaConnection.SetItem(&Item);

		Item.col = 1;
		Item.strText.Format("%d", tempSelectData);
		m_GridStaConnection.SetItem(&Item);
		m_GridStaConnection.UpdateData();

		Item.col = 2;
		Item.strText.Format("%d", tempSelectData2);
		m_GridStaConnection.SetItem(&Item);
		m_GridStaConnection.UpdateData();

		Item.col = 3;
		Item.strText.Format("%d", tempSelectData3);
		m_GridStaConnection.SetItem(&Item);
		m_GridStaConnection.UpdateData();

		nCount++;
	}

	// clean up
	m_pODBCStmt->FreeStatement();

	m_GridStaConnection.AutoSize();
}

void CStatisticPage3Dlg::ViewByDayHourConnection()
{
	m_GridStaConnection.SetBkColor(0xFFFFFF);
	m_GridStaConnection.SetHeaderSort();

	int m_nRows = 1;
	int m_nCols = 4;

	m_GridStaConnection.SetEditable(FALSE);
	m_GridStaConnection.SetListMode(TRUE);
	m_GridStaConnection.SetSingleRowSelection(TRUE);
	m_GridStaConnection.EnableSelection(TRUE);
	m_GridStaConnection.SetFrameFocusCell(FALSE);
	m_GridStaConnection.SetTrackFocusCell(FALSE);

	m_GridStaConnection.SetRowCount(m_nRows);
	m_GridStaConnection.SetColumnCount(m_nCols);
	m_GridStaConnection.SetFixedRowCount(1);

	// 칼럼 만들기
	m_nCols = 0;
	GV_ITEM Item;
	Item.mask = GVIF_TEXT|GVIF_FORMAT;
	Item.row = 0;
	Item.nFormat = DT_LEFT|DT_VCENTER|DT_SINGLELINE;

	Item.col = m_nCols++;
	Item.strText.Format("DATE");
	m_GridStaConnection.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("MAXCOUNT");
	m_GridStaConnection.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("MINCOUNT");
	m_GridStaConnection.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("AVGCOUNT");
	m_GridStaConnection.SetItem(&Item);

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
// 	szStartDate.Format("%04d-%02d-%02d %02d:%02d:%02d", m_valueDTByDayHourConnectionStartDate.GetYear(), m_valueDTByDayHourConnectionStartDate.GetMonth(), m_valueDTByDayHourConnectionStartDate.GetDay()
// 		, m_valueDTByDayHourConnectionStartTime.GetHour(), m_valueDTByDayHourConnectionStartTime.GetMinute(), m_valueDTByDayHourConnectionStartTime.GetSecond());
// 	szEndDate.Format("%04d-%02d-%02d %02d:%02d:%02d", m_valueDTByDayHourConnectionEndDate.GetYear(), m_valueDTByDayHourConnectionEndDate.GetMonth(), m_valueDTByDayHourConnectionEndDate.GetDay()
// 		, m_valueDTByDayHourConnectionEndTime.GetHour(), m_valueDTByDayHourConnectionEndTime.GetMinute(), m_valueDTByDayHourConnectionEndTime.GetSecond());
// 2008-08-28 by cmkwon, MySQL 관련 날짜시간 관련 오류 수정 - 
//	szStartDate.Format("%04d%02d%02d %02d:%02d:%02d", m_valueDTByDayHourConnectionStartDate.GetYear(), m_valueDTByDayHourConnectionStartDate.GetMonth(), m_valueDTByDayHourConnectionStartDate.GetDay()
	szStartDate.Format(SQL_DATETIME_STRING_FORMAT, m_valueDTByDayHourConnectionStartDate.GetYear(), m_valueDTByDayHourConnectionStartDate.GetMonth(), m_valueDTByDayHourConnectionStartDate.GetDay()
		, m_valueDTByDayHourConnectionStartTime.GetHour(), m_valueDTByDayHourConnectionStartTime.GetMinute(), m_valueDTByDayHourConnectionStartTime.GetSecond());
// 2008-08-28 by cmkwon, MySQL 관련 날짜시간 관련 오류 수정 - 
//	szEndDate.Format("%04d%02d%02d %02d:%02d:%02d", m_valueDTByDayHourConnectionEndDate.GetYear(), m_valueDTByDayHourConnectionEndDate.GetMonth(), m_valueDTByDayHourConnectionEndDate.GetDay()
	szEndDate.Format(SQL_DATETIME_STRING_FORMAT, m_valueDTByDayHourConnectionEndDate.GetYear(), m_valueDTByDayHourConnectionEndDate.GetMonth(), m_valueDTByDayHourConnectionEndDate.GetDay()
		, m_valueDTByDayHourConnectionEndTime.GetHour(), m_valueDTByDayHourConnectionEndTime.GetMinute(), m_valueDTByDayHourConnectionEndTime.GetSecond());

	CString szSQLQuery;

	// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
	//szSQLQuery.Format("drop view user_count_timeline");
	szSQLQuery.Format(QUERY_080702_0161);
// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
//	BOOL bRet = m_pODBCStmt->ExecuteQuery((LPCSTR)szSQLQuery);
	SQLHSTMT hstmt = m_pODBCStmt->GetSTMTHandle();
	SQLINTEGER arrCB2[2] = {SQL_NTS,SQL_NTS};
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SZQUERY, 0, (LPSTR)(LPCSTR)szSQLQuery, 0,			&arrCB2[1]);
	BOOL bRet = m_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_080827_0122));
	m_pODBCStmt->FreeStatement();
	
	// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
	//szSQLQuery.Format("drop view tm_user_count_timeline");
	szSQLQuery.Format(QUERY_080702_0162);
// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
//	bRet = m_pODBCStmt->ExecuteQuery((LPCSTR)szSQLQuery);
	arrCB2[1] = SQL_NTS;
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SZQUERY, 0, (LPSTR)(LPCSTR)szSQLQuery, 0,			&arrCB2[1]);
	bRet = m_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_080827_0122));
	m_pODBCStmt->FreeStatement();

	// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
	//szSQLQuery.Format("CREATE VIEW tm_user_count_timeline AS \
	//	SELECT * FROM atum_backup_log_server_map WITH (NOLOCK) where mapindex = 0 and time > '%s' and time < '%s' \
	//	UNION ALL \
	//	SELECT * FROM atum_log_server_map WITH (NOLOCK) where mapindex = 0 and time > '%s' and time < '%s'", szStartDate, szEndDate, szStartDate, szEndDate);
	szSQLQuery.Format(QUERY_080702_0163, szStartDate, szEndDate, szStartDate, szEndDate);
// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
//		bRet = m_pODBCStmt->ExecuteQuery((LPCSTR)szSQLQuery);
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
	//szSQLQuery.Format("create view user_count_timeline	as	select	time, dbo.atum_GetOnlyDate(time) as date, CAST(DATENAME(yy, time)+'-'+DATENAME(mm, time)+'-'+DATENAME(dd, time)+' '+DATENAME(hh, time)+':00' as datetime) as datehour,	DATEPART(hh, time) as hour,	sum(clientcount) as clientcount	from tm_user_count_timeline where mapindex = 0 and time > '%s' and time < '%s'	group by time"
	//	, szStartDate, szEndDate);
	szSQLQuery.Format(QUERY_080702_0164, szStartDate, szEndDate);
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
	//szSQLQuery.Format("select datehour, max(clientcount) as maxcount, min(clientcount) as mincount, avg(clientcount) as avgcount from user_count_timeline group by datehour	order by datehour desc");
// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
//	szSQLQuery.Format(QUERY_080702_0165);
//	bRet = m_pODBCStmt->ExecuteQuery((LPCSTR)szSQLQuery);
	bRet = m_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_080827_0165));
	if (!bRet)
	{
		// clean up
		m_pODBCStmt->FreeStatement();
		MessageBox("Error");
		return;
	}


	SQLINTEGER cb1, cb2, cb3, cb4;

	char tempDate[20];
	memset(tempDate, 0, sizeof(tempDate));
	int tempSelectData, tempSelectData2, tempSelectData3;
	SQLBindCol(m_pODBCStmt->m_hstmt, 1, SQL_C_CHAR, &tempDate, 20, &cb1);
	SQLBindCol(m_pODBCStmt->m_hstmt, 2, SQL_C_ULONG, &tempSelectData, 0, &cb2);
	SQLBindCol(m_pODBCStmt->m_hstmt, 3, SQL_C_ULONG, &tempSelectData2, 0, &cb3);
	SQLBindCol(m_pODBCStmt->m_hstmt, 4, SQL_C_ULONG, &tempSelectData3, 0, &cb4);

	int nCount = 0;
	m_GridStaConnection.SetRowCount(nCount+1);
	// DB에 값이 없을때까지 loop를 돈다
	while ( (bRet = SQLFetch(m_pODBCStmt->m_hstmt)) != SQL_NO_DATA)
	{
		m_GridStaConnection.SetRowCount(nCount+2);

		//select된 값을 GridDetail GridCtrl에 넣어준다.
		Item.col = 0;
		Item.row = nCount+1;
		Item.strText.Format("%s", tempDate);
		m_GridStaConnection.SetItem(&Item);

		Item.col = 1;
		Item.strText.Format("%d", tempSelectData);
		m_GridStaConnection.SetItem(&Item);
		m_GridStaConnection.UpdateData();

		Item.col = 2;
		Item.strText.Format("%d", tempSelectData2);
		m_GridStaConnection.SetItem(&Item);
		m_GridStaConnection.UpdateData();

		Item.col = 3;
		Item.strText.Format("%d", tempSelectData3);
		m_GridStaConnection.SetItem(&Item);
		m_GridStaConnection.UpdateData();

		nCount++;
	}

	// clean up
	m_pODBCStmt->FreeStatement();

	m_GridStaConnection.AutoSize();
}

void CStatisticPage3Dlg::OnCheckSt3DailyConnection() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if(m_valueCheckDailyConnection == TRUE)
	{
		m_valueCheckByDayHourConnection = FALSE;
		m_valueCheckByDayConnection = FALSE;
		m_valueCheckByHourConnection = FALSE;
	}
	UpdateData(FALSE);
}

void CStatisticPage3Dlg::OnCheck2() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if(m_valueCheckByHourConnection == TRUE)
	{
		m_valueCheckByDayHourConnection = FALSE;
		m_valueCheckByDayConnection = FALSE;
		m_valueCheckDailyConnection = FALSE;
	}
	UpdateData(FALSE);
}

void CStatisticPage3Dlg::OnCheck3() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if(m_valueCheckByDayConnection == TRUE)
	{
		m_valueCheckByDayHourConnection = FALSE;
		m_valueCheckDailyConnection = FALSE;
		m_valueCheckByHourConnection = FALSE;
	}
	UpdateData(FALSE);
}

void CStatisticPage3Dlg::OnCheck4() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if(m_valueCheckByDayHourConnection == TRUE)
	{
		m_valueCheckDailyConnection = FALSE;
		m_valueCheckByDayConnection = FALSE;
		m_valueCheckByHourConnection = FALSE;
	}
	UpdateData(FALSE);
}

void CStatisticPage3Dlg::OnButtonSt3Save() 
{
	// TODO: Add your control notification handler code here
	CString tempStr;
	for(int i = 0; i<m_GridStaConnection.GetRowCount();i++)
	{
		tempStr = "";
		for(int j = 0; j< m_GridStaConnection.GetColumnCount();j++)
		{
			tempStr += m_GridStaConnection.GetItemText( i, j);
			tempStr += "|\t";
		}
		m_pMainDlg->m_pWriteLog->WriteLogFile((char*)(LPCTSTR)tempStr, tempStr.GetLength(), m_pMainDlg, 1);
	}
}
