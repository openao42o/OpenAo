// StatisticPage9Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "spacecowboydbeditortool.h"
#include "StatisticPage9Dlg.h"
#include "SpaceCowboyDBEditorToolDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStatisticPage9Dlg property page

IMPLEMENT_DYNCREATE(CStatisticPage9Dlg, CPropertyPage)

CStatisticPage9Dlg::CStatisticPage9Dlg() : CPropertyPage(CStatisticPage9Dlg::IDD)
{
	//{{AFX_DATA_INIT(CStatisticPage9Dlg)
	m_valueCheckConnectionUser = FALSE;
	m_valueDTConnectUserStartDate = COleDateTime::GetCurrentTime();
	m_valueDTConnectUserEndDate = COleDateTime::GetCurrentTime();
	m_valueDTConnectUserStartTime = COleDateTime::GetCurrentTime();
	m_valueDTConnectUserEndTime = COleDateTime::GetCurrentTime();
	m_valueDTConnectUserStartDate2	= COleDateTime::GetCurrentTime();
	m_valueDTConnectUserEndDate2	= COleDateTime::GetCurrentTime();
	m_valueDTConnectUserStartTime2	= COleDateTime::GetCurrentTime();
	m_valueDTConnectUserEndTime2	= COleDateTime::GetCurrentTime();
	m_valueCheckEachDay = FALSE;
	//}}AFX_DATA_INIT
	m_pMainDlg = (CSpaceCowboyDBEditorToolDlg*)AfxGetMainWnd();
	m_pODBCStmt				= m_pMainDlg->m_pODBCStmt;
}

CStatisticPage9Dlg::~CStatisticPage9Dlg()
{
// 2006-01-23 by cmkwon
//	util::del(m_pODBCStmt);
}

void CStatisticPage9Dlg::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStatisticPage9Dlg)
	DDX_Check(pDX, IDC_CHECK1, m_valueCheckConnectionUser);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_ST3_DAILY_CONNECTION_STARTDATE, m_valueDTConnectUserStartDate);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_ST3_DAILY_CONNECTION_ENDDATE, m_valueDTConnectUserEndDate);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_ST1_DAILY_ACCOUNT_STARTTIME8, m_valueDTConnectUserStartTime);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_ST1_DAILY_ACCOUNT_ENDTIME8, m_valueDTConnectUserEndTime);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_EACHDAY_STARTDATE, m_valueDTConnectUserStartDate2);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_EACHDAY_ENDDATE, m_valueDTConnectUserEndDate2);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_EACHDAY_STARTTIME, m_valueDTConnectUserStartTime2);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_EACHDAY_ENDTIME, m_valueDTConnectUserEndTime2);
	DDX_Control(pDX, IDC_CUSTOM_STA_CONNECTUSER, m_GridStaConnectUser);
	DDX_Check(pDX, IDC_CHECK2, m_valueCheckEachDay);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CStatisticPage9Dlg, CPropertyPage)
	//{{AFX_MSG_MAP(CStatisticPage9Dlg)
	ON_BN_CLICKED(IDC_BUTTON_ST9_SELECT, OnButtonSt9Select)
	ON_BN_CLICKED(IDC_BUTTON_ST9_SAVE, OnButtonSt9Save)
	ON_BN_CLICKED(IDC_CHECK1, OnCheck1)
	ON_BN_CLICKED(IDC_CHECK2, OnCheck2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStatisticPage9Dlg message handlers

void CStatisticPage9Dlg::OnButtonSt9Select() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if(m_valueCheckConnectionUser)
	{
		ViewConnectionUser();
	}

	if(m_valueCheckEachDay)
	{// 2006-09-21 by cmkwon, 날짜별 접속자수
		ViewConnectionUserEachDay();
	}
}

void CStatisticPage9Dlg::ViewConnectionUser()
{
	m_GridStaConnectUser.SetBkColor(0xFFFFFF);
	m_GridStaConnectUser.SetHeaderSort();

	int m_nRows = 1;
	int m_nCols = 7;

	m_GridStaConnectUser.SetEditable(FALSE);
	m_GridStaConnectUser.SetListMode(TRUE);
	m_GridStaConnectUser.SetSingleRowSelection(TRUE);
	m_GridStaConnectUser.EnableSelection(TRUE);
	m_GridStaConnectUser.SetFrameFocusCell(FALSE);
	m_GridStaConnectUser.SetTrackFocusCell(FALSE);

	m_GridStaConnectUser.SetRowCount(m_nRows);
	m_GridStaConnectUser.SetColumnCount(m_nCols);
	m_GridStaConnectUser.SetFixedRowCount(1);

	// 칼럼 만들기
	m_nCols = 0;
	GV_ITEM Item;
	Item.mask = GVIF_TEXT|GVIF_FORMAT;
	Item.row = 0;
	Item.nFormat = DT_LEFT|DT_VCENTER|DT_SINGLELINE;

	Item.col = m_nCols++;
	Item.strText.Format("Total Count");
	m_GridStaConnectUser.SetItem(&Item);
	Item.col = m_nCols++;
	Item.strText.Format("Normal");
	m_GridStaConnectUser.SetItem(&Item);
	Item.col = m_nCols++;
	Item.strText.Format("BCU");
	m_GridStaConnectUser.SetItem(&Item);
	Item.col = m_nCols++;
	Item.strText.Format("ANI");
	m_GridStaConnectUser.SetItem(&Item);
	Item.col = m_nCols++;
	Item.strText.Format("BCU+ANI");
	m_GridStaConnectUser.SetItem(&Item);
	Item.col = m_nCols++;
	Item.strText.Format("BCU Rate(%%)");
	m_GridStaConnectUser.SetItem(&Item);
	Item.col = m_nCols++;
	Item.strText.Format("ANI Rate(%%)");
	m_GridStaConnectUser.SetItem(&Item);
	
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
// 	szStartDate.Format("%04d-%02d-%02d %02d:%02d:%02d", m_valueDTConnectUserStartDate.GetYear(), m_valueDTConnectUserStartDate.GetMonth(), m_valueDTConnectUserStartDate.GetDay()
// 		, m_valueDTConnectUserStartTime.GetHour(), m_valueDTConnectUserStartTime.GetMinute(), m_valueDTConnectUserStartTime.GetSecond());
// 	szEndDate.Format("%04d-%02d-%02d %02d:%02d:%02d", m_valueDTConnectUserEndDate.GetYear(), m_valueDTConnectUserEndDate.GetMonth(), m_valueDTConnectUserEndDate.GetDay()
// 		, m_valueDTConnectUserEndTime.GetHour(), m_valueDTConnectUserEndTime.GetMinute(), m_valueDTConnectUserEndTime.GetSecond());
// 2008-08-28 by cmkwon, MySQL 관련 날짜시간 관련 오류 수정 - 
//	szStartDate.Format("%04d%02d%02d %02d:%02d:%02d", m_valueDTConnectUserStartDate.GetYear(), m_valueDTConnectUserStartDate.GetMonth(), m_valueDTConnectUserStartDate.GetDay()
	szStartDate.Format(SQL_DATETIME_STRING_FORMAT, m_valueDTConnectUserStartDate.GetYear(), m_valueDTConnectUserStartDate.GetMonth(), m_valueDTConnectUserStartDate.GetDay()
		, m_valueDTConnectUserStartTime.GetHour(), m_valueDTConnectUserStartTime.GetMinute(), m_valueDTConnectUserStartTime.GetSecond());
// 2008-08-28 by cmkwon, MySQL 관련 날짜시간 관련 오류 수정 - 
//	szEndDate.Format("%04d%02d%02d %02d:%02d:%02d", m_valueDTConnectUserEndDate.GetYear(), m_valueDTConnectUserEndDate.GetMonth(), m_valueDTConnectUserEndDate.GetDay()
	szEndDate.Format(SQL_DATETIME_STRING_FORMAT, m_valueDTConnectUserEndDate.GetYear(), m_valueDTConnectUserEndDate.GetMonth(), m_valueDTConnectUserEndDate.GetDay()
		, m_valueDTConnectUserEndTime.GetHour(), m_valueDTConnectUserEndTime.GetMinute(), m_valueDTConnectUserEndTime.GetSecond());

	CString szSQLQuery;
	/*******************************************************************************
	DROP VIEW tm_user_connection
	go
	CREATE VIEW tm_user_connection
	as
		select distinct c.AccountUniqueNumber, c.selectableinfluencemask 
		from atum_log_connection l WITH (NOLOCK) inner join td_character c on c.accountuniquenumber = l.accountuniquenumber 
		where time > '20080101 00:00:00' and time < '20080301 00:00:00'	
		UNION (SELECT DISTINCT c.AccountUniqueNumber, c.selectableinfluencemask \
				FROM atum_log_connection l WITH (NOLOCK) INNER join td_character c \
					ON c.accountuniquenumber = l.accountuniquenumber \
				WHERE time > '20080101 00:00:00' and time < '20080301 00:00:00'
	go

	select selectableinfluencemask, count(selectableinfluencemask)
	from tm_user_connection
	group by selectableinfluencemask
	*******************************************************************************/

// 2008-01-14 by cmkwon, T_D: Connection User Count 보여지는것 세력별로 수정 - 아래와 같이 수정
//	szSQLQuery.Format("select count(distinct AccountUniqueNumber) from atum_log_connection WITH (NOLOCK) where time > '%s' and time < '%s'"
//		,szStartDate, szEndDate);

	// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
	//szSQLQuery.Format("DROP VIEW tm_user_connection");
// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
//	szSQLQuery.Format(QUERY_080702_0224);
//	BOOL bRet = m_pODBCStmt->ExecuteQuery((LPCSTR)szSQLQuery);
	BOOL bRet = m_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_080827_0224));

// 2008-01-15 by cmkwon, T_D: Connection User Count 보여지는것 세력별로 수정 - VIEW가 없을수도 있다.
// 	if (!bRet)
// 	{
// 		m_pODBCStmt->FreeStatement();		// clean up
// 		MessageBox("ExecuteQuery Error 1");
// 		return;
// 	}
	
// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
// 	szSQLQuery.Format("CREATE VIEW tm_user_connection AS \
// 		SELECT DISTINCT c.AccountUniqueNumber, c.selectableinfluencemask \
// 		FROM atum_log_connection l WITH (NOLOCK) inner join td_character c WITH (NOLOCK) \
// 			ON c.accountuniquenumber = l.accountuniquenumber \
// 		WHERE time > '%s' and time < '%s' \
// 		UNION (SELECT DISTINCT c.AccountUniqueNumber, c.selectableinfluencemask \
// 				FROM atum_log_connection l WITH (NOLOCK) INNER join td_character c \
// 					ON c.accountuniquenumber = l.accountuniquenumber \
// 				WHERE time > '%s' and time < '%s')"
// 				, szStartDate, szEndDate, szStartDate, szEndDate);
// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 

// 2009-03-30 by cmkwon, AdminTool 유저 카운트 버그 수정(로그로 검색 해야 함) - 아래와 같이 로그로부터 검색 해야함.
//// 2009-02-10 by cmkwon, 세력 유저 카운트 관련 수정 - 아래처럼 QUERY_090209_0300로 수정됨
////	szSQLQuery.Format(QUERY_080702_0225, szStartDate, szEndDate, szStartDate, szEndDate);
//	szSQLQuery.Format(QUERY_090209_0300, szStartDate, szEndDate);		// 2009-02-10 by cmkwon, 세력 유저 카운트 관련 수정 - 기존 계정의 로그인 정보로 검색 하던것을 캐릭터 최종 접속시간 검색으로 수정
	szSQLQuery.Format(QUERY_090330_0302, szStartDate, szEndDate, szStartDate, szEndDate);		// 2009-03-30 by cmkwon, AdminTool 유저 카운트 버그 수정(로그로 검색 해야 함) - 로그로부터 검색 해야함.

	SQLBindParameter(m_pODBCStmt->m_hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SZQUERY, 0, (LPSTR)(LPCSTR)szSQLQuery, 0, NULL);
	bRet = m_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_080827_0225));
	if (!bRet)
	{
		m_pODBCStmt->FreeStatement();		// clean up
		MessageBox("ExecuteQuery Error 2");
		return;
	}

// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
// 	szSQLQuery.Format("SELECT selectableinfluencemask, COUNT(selectableinfluencemask) \
// 						FROM tm_user_connection \
// 						GROUP BY selectableinfluencemask");
// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
//	szSQLQuery.Format(QUERY_080702_0226);
//	bRet = m_pODBCStmt->ExecuteQuery((LPCSTR)szSQLQuery);
	bRet = m_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_080827_0226));	
	if (!bRet)
	{
		// clean up
		m_pODBCStmt->FreeStatement();
		MessageBox("Error");
		return;
	}

	int		arrnUserCnts[INFLUENCE_TYPE_COUNT] = {0,0,0,0};
	SQLINTEGER arrCB[3] = {SQL_NTS, SQL_NTS, SQL_NTS};
	BYTE	byInflMask;
	int		nUserCnts;
	SQLBindCol(m_pODBCStmt->m_hstmt, 1, SQL_C_UTINYINT, &byInflMask, 0,	&arrCB[1]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 2, SQL_C_LONG, &nUserCnts, 0,		&arrCB[2]);
	

	// DB에 값이 없을때까지 loop를 돈다
	while ( (bRet = SQLFetch(m_pODBCStmt->m_hstmt)) != SQL_NO_DATA)
	{
		switch(byInflMask)
		{
		case INFLUENCE_TYPE_VCN:
		case INFLUENCE_TYPE_ANI:
			{
					arrnUserCnts[GetArrayIndexByInfluenceType(byInflMask)] = nUserCnts;
			}
			break;
		default:
			{
					arrnUserCnts[0]		= nUserCnts;	// 2008-01-14 by cmkwon, 일반 세력
			}
		}
		byInflMask	= 0;
		nUserCnts	= 0;
	}

	m_pODBCStmt->FreeStatement();	// clean up

	int nCount	= 2;
	int	nCol	= 0;
	m_GridStaConnectUser.SetRowCount(nCount);
	Item.col = nCol++;
	Item.row = nCount-1;
	Item.strText.Format("%d", arrnUserCnts[0]+arrnUserCnts[1]+arrnUserCnts[2]);
	m_GridStaConnectUser.SetItem(&Item);

	Item.col = nCol++;
	Item.row = nCount-1;
	Item.strText.Format("%d", arrnUserCnts[0]);
	m_GridStaConnectUser.SetItem(&Item);

	Item.col = nCol++;
	Item.row = nCount-1;
	Item.strText.Format("%d", arrnUserCnts[1]);
	m_GridStaConnectUser.SetItem(&Item);

	Item.col = nCol++;
	Item.row = nCount-1;
	Item.strText.Format("%d", arrnUserCnts[2]);
	m_GridStaConnectUser.SetItem(&Item);

	Item.col = nCol++;
	Item.row = nCount-1;
	Item.strText.Format("%d", arrnUserCnts[1]+arrnUserCnts[2]);
	m_GridStaConnectUser.SetItem(&Item);

	Item.col = nCol++;
	Item.row = nCount-1;
	Item.strText.Format("%4.1f", ( (arrnUserCnts[1]+arrnUserCnts[2])==0 ) ? 0 : (100 * (float)arrnUserCnts[1])/(arrnUserCnts[1]+arrnUserCnts[2]));
	m_GridStaConnectUser.SetItem(&Item);

	Item.col = nCol++;
	Item.row = nCount-1;
	Item.strText.Format("%4.1f", ( (arrnUserCnts[1]+arrnUserCnts[2])==0 ) ? 0 : (100 * (float)arrnUserCnts[2])/(arrnUserCnts[1]+arrnUserCnts[2]));
	m_GridStaConnectUser.SetItem(&Item);

	m_GridStaConnectUser.UpdateData();

	m_GridStaConnectUser.AutoSize();
}

// 2008-01-15 by cmkwon, T_D: Connection User Count 보여지는것 세력별로 수정 - 
struct SDATE_USER_COUNT
{
	char	Date[SIZE_MAX_SQL_DATETIME_STRING];	
	int		arrUserCnts[INFLUENCE_TYPE_COUNT];
};
typedef vector<SDATE_USER_COUNT>			vectSDATE_USER_COUNT;

struct find_if_SDATE_USER_COUNT_BY_Date
{
	find_if_SDATE_USER_COUNT_BY_Date(char *i_szDate)
	{
		util::strncpy(m_szDate, i_szDate, SIZE_MAX_SQL_DATETIME_STRING);
	};
	bool operator()(const SDATE_USER_COUNT dateUserCnt)
	{
		if(0 != strncmp(dateUserCnt.Date, m_szDate, SIZE_MAX_SQL_DATETIME_STRING))
		{
			return false;
		}

		return true;	}
	char m_szDate[SIZE_MAX_SQL_DATETIME_STRING];
};

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CStatisticPage9Dlg::ViewConnectionUserEachDay(void)
/// \brief		
/// \author		cmkwon
/// \date		2006-09-21 ~ 2006-09-21
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CStatisticPage9Dlg::ViewConnectionUserEachDay(void)
{
	m_GridStaConnectUser.SetBkColor(0xFFFFFF);
	m_GridStaConnectUser.SetHeaderSort();

	int m_nRows = 1;
	int m_nCols = 8;

	m_GridStaConnectUser.SetEditable(FALSE);
	m_GridStaConnectUser.SetListMode(TRUE);
	m_GridStaConnectUser.SetSingleRowSelection(TRUE);
	m_GridStaConnectUser.EnableSelection(TRUE);
	m_GridStaConnectUser.SetFrameFocusCell(FALSE);
	m_GridStaConnectUser.SetTrackFocusCell(FALSE);

	m_GridStaConnectUser.SetRowCount(m_nRows);
	m_GridStaConnectUser.SetColumnCount(m_nCols);
	m_GridStaConnectUser.SetFixedRowCount(1);

	// 칼럼 만들기
	m_nCols = 0;
	GV_ITEM Item;
	Item.mask = GVIF_TEXT|GVIF_FORMAT;
	Item.row = 0;
	Item.nFormat = DT_LEFT|DT_VCENTER|DT_SINGLELINE;

	Item.col = m_nCols++;
	Item.strText.Format("Connect Date");
	m_GridStaConnectUser.SetItem(&Item);
	Item.col = m_nCols++;
	Item.strText.Format("Total Count");
	m_GridStaConnectUser.SetItem(&Item);
	Item.col = m_nCols++;
	Item.strText.Format("Normal");
	m_GridStaConnectUser.SetItem(&Item);
	Item.col = m_nCols++;
	Item.strText.Format("BCU");
	m_GridStaConnectUser.SetItem(&Item);
	Item.col = m_nCols++;
	Item.strText.Format("ANI");
	m_GridStaConnectUser.SetItem(&Item);
	Item.col = m_nCols++;
	Item.strText.Format("BCU+ANI");
	m_GridStaConnectUser.SetItem(&Item);
	Item.col = m_nCols++;
	Item.strText.Format("BCU Rate(%%)");
	m_GridStaConnectUser.SetItem(&Item);
	Item.col = m_nCols++;
	Item.strText.Format("ANI Rate(%%)");
	m_GridStaConnectUser.SetItem(&Item);

	CString szStartDate, szEndDate;
// 2008-01-24 by cmkwon, S_A: 날짜 관련 로컬리 제이션 처리 버그 수정 - 아래와 같은 형식으로 수정
// 	szStartDate.Format("%04d-%02d-%02d %02d:%02d:%02d", m_valueDTConnectUserStartDate2.GetYear(), m_valueDTConnectUserStartDate2.GetMonth(), m_valueDTConnectUserStartDate2.GetDay()
// 		, m_valueDTConnectUserStartTime2.GetHour(), m_valueDTConnectUserStartTime2.GetMinute(), m_valueDTConnectUserStartTime2.GetSecond());
// 	szEndDate.Format("%04d-%02d-%02d %02d:%02d:%02d", m_valueDTConnectUserEndDate2.GetYear(), m_valueDTConnectUserEndDate2.GetMonth(), m_valueDTConnectUserEndDate2.GetDay()
// 		, m_valueDTConnectUserEndTime2.GetHour(), m_valueDTConnectUserEndTime2.GetMinute(), m_valueDTConnectUserEndTime2.GetSecond());
// 2008-08-28 by cmkwon, MySQL 관련 날짜시간 관련 오류 수정 - 
//	szStartDate.Format("%04d%02d%02d %02d:%02d:%02d", m_valueDTConnectUserStartDate2.GetYear(), m_valueDTConnectUserStartDate2.GetMonth(), m_valueDTConnectUserStartDate2.GetDay()
	szStartDate.Format(SQL_DATETIME_STRING_FORMAT, m_valueDTConnectUserStartDate2.GetYear(), m_valueDTConnectUserStartDate2.GetMonth(), m_valueDTConnectUserStartDate2.GetDay()
		, m_valueDTConnectUserStartTime2.GetHour(), m_valueDTConnectUserStartTime2.GetMinute(), m_valueDTConnectUserStartTime2.GetSecond());
// 2008-08-28 by cmkwon, MySQL 관련 날짜시간 관련 오류 수정 - 
//	szEndDate.Format("%04d%02d%02d %02d:%02d:%02d", m_valueDTConnectUserEndDate2.GetYear(), m_valueDTConnectUserEndDate2.GetMonth(), m_valueDTConnectUserEndDate2.GetDay()
	szEndDate.Format(SQL_DATETIME_STRING_FORMAT, m_valueDTConnectUserEndDate2.GetYear(), m_valueDTConnectUserEndDate2.GetMonth(), m_valueDTConnectUserEndDate2.GetDay()
		, m_valueDTConnectUserEndTime2.GetHour(), m_valueDTConnectUserEndTime2.GetMinute(), m_valueDTConnectUserEndTime2.GetSecond());

	CString szSQLQuery;
	/*******************************************************************************
	-- // 2008-01-15 by cmkwon, T_D: Connection User Count 보여지는것 세력별로 수정 - 쿼리
	DROP VIEW tm_user_connection
	GO
	CREATE VIEW tm_user_connection
	AS
		SELECT dbo.atum_getonlydate(Time) AS conntedDate, c.AccountUniqueNumber, c.SelectableInfluenceMask
		FROM atum_log_connection l WITH(NOLOCK) INNER JOIN td_character c ON c.AccountUniqueNumber = l.AccountUniqueNumber
		WHERE Time >= '20080101 00:00:00' and Time <= '20080301 00:00:00'
		GROUP BY dbo.atum_getonlydate(Time), c.AccountUniqueNumber, c.SelectableInfluenceMask
		UNION (SELECT dbo.atum_getonlydate(Time) AS conntedDate, c.AccountUniqueNumber, c.SelectableInfluenceMask
				FROM atum_backup_log_connection l WITH(NOLOCK) INNER JOIN td_character c ON c.AccountUniqueNumber = l.AccountUniqueNumber
				WHERE Time >= '20080101 00:00:00' and Time <= '20080301 00:00:00'
				GROUP BY dbo.atum_getonlydate(Time), c.AccountUniqueNumber, c.SelectableInfluenceMask)

	GO

	SELECT l.conntedDate, l.SelectableInfluenceMask, COUNT(l.AccountUniqueNumber)
	FROM tm_user_connection l WITH(NOLOCK)
	GROUP BY l.conntedDate, l.SelectableInfluenceMask
	ORDER BY l.conntedDate
	*******************************************************************************/

// 2008-01-15 by cmkwon, T_D: Connection User Count 보여지는것 세력별로 수정 - 기존 소스
// 	///////////////////////////////////////////////////////////////////////////////
// 	// 2006-09-21 by cmkwon, 날짜별 접속자수 구하기
// 	szSQLQuery.Format("SELECT tm.Date, COUNT(tm.date) \
// 						FROM ( SELECT dbo.atum_getonlydate(Time) AS Date, AccountUniqueNumber \
// 								FROM atum_backup_log_connection WHERE Time >= '%s' and Time <= '%s' \
// 								GROUP BY dbo.atum_getonlydate(Time), AccountUniqueNumber \
// 								UNION (SELECT dbo.atum_getonlydate(Time) AS Date, AccountUniqueNumber \
// 										FROM atum_log_connection WHERE Time >= '%s' and Time <= '%s' \
// 										GROUP BY dbo.atum_getonlydate(Time), AccountUniqueNumber) ) tm \
// 						GROUP BY tm.Date ORDER BY tm.Date", szStartDate, szEndDate, szStartDate, szEndDate);

	// 2008-01-15 by cmkwon, T_D: Connection User Count 보여지는것 세력별로 수정 - 아래와 같이 수정함

	// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
	//szSQLQuery.Format("DROP VIEW tm_user_connection");
// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
//	szSQLQuery.Format(QUERY_080702_0227);
//	BOOL bRet = m_pODBCStmt->ExecuteQuery((LPCSTR)szSQLQuery);
	BOOL bRet = m_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_080827_0227));
// 2008-01-15 by cmkwon, T_D: Connection User Count 보여지는것 세력별로 수정 - VIEW가 없을수도 있다.
//	if (!bRet)
//	{
//		m_pODBCStmt->FreeStatement();		// clean up
//		MessageBox("ExecuteQuery Error 1");
//		return;
//	}
	
// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
// 	szSQLQuery.Format("CREATE VIEW tm_user_connection AS \
// 		SELECT dbo.atum_getonlydate(Time) AS conntedDate, c.AccountUniqueNumber, c.SelectableInfluenceMask \
// 		FROM atum_log_connection l WITH(NOLOCK) INNER JOIN td_character c ON c.AccountUniqueNumber = l.AccountUniqueNumber \
// 		WHERE Time >= '%s' and Time <= '%s' \
// 		GROUP BY dbo.atum_getonlydate(Time), c.AccountUniqueNumber, c.SelectableInfluenceMask \
// 		UNION (SELECT dbo.atum_getonlydate(Time) AS conntedDate, c.AccountUniqueNumber, c.SelectableInfluenceMask \
// 			FROM atum_backup_log_connection l WITH(NOLOCK) INNER JOIN td_character c ON c.AccountUniqueNumber = l.AccountUniqueNumber \
// 			WHERE Time >= '%s' and Time <= '%s' \
// 			GROUP BY dbo.atum_getonlydate(Time), c.AccountUniqueNumber, c.SelectableInfluenceMask)"
//		, szStartDate, szEndDate, szStartDate, szEndDate);
// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 

// 2009-03-30 by cmkwon, AdminTool 유저 카운트 버그 수정(로그로 검색 해야 함) - 아래와 같이 로그로부터 검색 해야함.
//// 2009-02-10 by cmkwon, 세력 유저 카운트 관련 수정 - 아래와 같이 수정 함.
////	szSQLQuery.Format(QUERY_080702_0228, szStartDate, szEndDate, szStartDate, szEndDate);
//	szSQLQuery.Format(QUERY_090209_0301, szStartDate, szEndDate);	// 2009-02-10 by cmkwon, 세력 유저 카운트 관련 수정 - 기존 계정의 로그인 정보로 검색 하던것을 캐릭터 최종 접속시간 검색으로 수정
	szSQLQuery.Format(QUERY_090330_0303, szStartDate, szEndDate, szStartDate, szEndDate);	// 2009-03-30 by cmkwon, AdminTool 유저 카운트 버그 수정(로그로 검색 해야 함) - 로그로부터 검색 해야함.

//	bRet = m_pODBCStmt->ExecuteQuery((LPCSTR)szSQLQuery);
	SQLBindParameter(m_pODBCStmt->m_hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SZQUERY, 0, (LPSTR)(LPCSTR)szSQLQuery, 0, NULL);
	bRet = m_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_080827_0228));	
	if (!bRet)
	{
		m_pODBCStmt->FreeStatement();		// clean up
		MessageBox("ExecuteQuery Error 2");
		return;
	}

// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
// 	szSQLQuery.Format("SELECT l.conntedDate, l.SelectableInfluenceMask, COUNT(l.AccountUniqueNumber) \
// 		FROM tm_user_connection l WITH(NOLOCK) \
// 		GROUP BY l.conntedDate, l.SelectableInfluenceMask \
// 		ORDER BY l.conntedDate");
// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
//		szSQLQuery.Format(QUERY_080702_0229);
//	bRet = m_pODBCStmt->ExecuteQuery((LPCSTR)szSQLQuery);
	bRet = m_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_080827_0229));	
	if (!bRet)
	{
		// clean up
		m_pODBCStmt->FreeStatement();
		MessageBox("Error");
		return;
	}

	vectSDATE_USER_COUNT	vectDateUserCntList;
	SQLINTEGER arrCB[4] = {SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS};
	char	tmDate[SIZE_MAX_SQL_DATETIME_STRING];	
	BYTE	byInflMask;
	int		nUserCnts;

	SQLBindCol(m_pODBCStmt->m_hstmt, 1, SQL_C_CHAR, &tmDate, SIZE_MAX_SQL_DATETIME_STRING,	&arrCB[1]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 2, SQL_C_UTINYINT, &byInflMask, 0,						&arrCB[2]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 3, SQL_C_LONG, &nUserCnts, 0,							&arrCB[3]);

	util::zero(tmDate, SIZE_MAX_SQL_DATETIME_STRING);
	byInflMask	= 0;
	nUserCnts	= 0;
	// DB에 값이 없을때까지 loop를 돈다
	while ( (bRet = SQLFetch(m_pODBCStmt->m_hstmt)) != SQL_NO_DATA)
	{
		vectSDATE_USER_COUNT::iterator itr = find_if(vectDateUserCntList.begin(), vectDateUserCntList.end(), find_if_SDATE_USER_COUNT_BY_Date(tmDate));
		if(itr == vectDateUserCntList.end())
		{
			SDATE_USER_COUNT tmDUserCnt;	util::zero(&tmDUserCnt, sizeof(SDATE_USER_COUNT));
			util::strncpy(tmDUserCnt.Date, tmDate, SIZE_MAX_SQL_DATETIME_STRING);
			switch(byInflMask)
			{
			case INFLUENCE_TYPE_VCN:
			case INFLUENCE_TYPE_ANI:
				{
						tmDUserCnt.arrUserCnts[GetArrayIndexByInfluenceType(byInflMask)] = nUserCnts;
				}
				break;
			default:
				{
						tmDUserCnt.arrUserCnts[0]		= nUserCnts;	// 2008-01-14 by cmkwon, 일반 세력
				}
			}
			vectDateUserCntList.push_back(tmDUserCnt);
		}
		else
		{
			switch(byInflMask)
			{
			case INFLUENCE_TYPE_VCN:
			case INFLUENCE_TYPE_ANI:
				{
						itr->arrUserCnts[GetArrayIndexByInfluenceType(byInflMask)] = nUserCnts;
				}
				break;
			default:
				{
						itr->arrUserCnts[0]		= nUserCnts;	// 2008-01-14 by cmkwon, 일반 세력
				}
			}
		}
		util::zero(tmDate, SIZE_MAX_SQL_DATETIME_STRING);
		byInflMask	= 0;
		nUserCnts	= 0;
	}
	m_pODBCStmt->FreeStatement();	// clean up
	
	///////////////////////////////////////////////////////////////////////////////
	// 2008-01-15 by cmkwon, T_D: Connection User Count 보여지는것 세력별로 수정 - 그리드에 추가하기
	SDATE_USER_COUNT totalDateUseCnt;
	util::zero(&totalDateUseCnt, sizeof(totalDateUseCnt));
	util::strncpy(totalDateUseCnt.Date, "SUM", SIZE_MAX_SQL_DATETIME_STRING);
	
	int nSize = vectDateUserCntList.size();
	for(int i=0; i <= nSize; i++)
	{
		SDATE_USER_COUNT *pDateUserCnt = NULL;
		if(i < nSize)
		{// 2008-01-15 by cmkwon, 날짜별 정보를 추가한다.
			pDateUserCnt = &(vectDateUserCntList[i]);

			// 2008-01-15 by cmkwon, 전체 정보에 합친다
			totalDateUseCnt.arrUserCnts[0]	+= pDateUserCnt->arrUserCnts[0];
			totalDateUseCnt.arrUserCnts[1]	+= pDateUserCnt->arrUserCnts[1];
			totalDateUseCnt.arrUserCnts[2]	+= pDateUserCnt->arrUserCnts[2];
			totalDateUseCnt.arrUserCnts[3]	+= pDateUserCnt->arrUserCnts[3];
		}
		else
		{// 2008-01-15 by cmkwon, 전체 정보를 추가한다.
			pDateUserCnt = &totalDateUseCnt;
		}

		int nNewRowIdx = m_GridStaConnectUser.GetRowCount();
		m_GridStaConnectUser.SetRowCount(nNewRowIdx+1);

		//select된 값을 GridDetail GridCtrl에 넣어준다.
		int	nCol	= 0;

		Item.row	= nNewRowIdx;
		Item.col	= nCol++;
		Item.strText.Format("%s", pDateUserCnt->Date);
		m_GridStaConnectUser.SetItem(&Item);

		Item.col	= nCol++;
		Item.row	= nNewRowIdx;
		Item.strText.Format("%d", pDateUserCnt->arrUserCnts[0]+pDateUserCnt->arrUserCnts[1]+pDateUserCnt->arrUserCnts[2]);
		m_GridStaConnectUser.SetItem(&Item);

		Item.col	= nCol++;
		Item.row	= nNewRowIdx;
		Item.strText.Format("%d", pDateUserCnt->arrUserCnts[0]);
		m_GridStaConnectUser.SetItem(&Item);

		Item.col	= nCol++;
		Item.row	= nNewRowIdx;
		Item.strText.Format("%d", pDateUserCnt->arrUserCnts[1]);
		m_GridStaConnectUser.SetItem(&Item);

		Item.col	= nCol++;
		Item.row	= nNewRowIdx;
		Item.strText.Format("%d", pDateUserCnt->arrUserCnts[2]);
		m_GridStaConnectUser.SetItem(&Item);

		Item.col	= nCol++;
		Item.row	= nNewRowIdx;
		Item.strText.Format("%d", pDateUserCnt->arrUserCnts[1]+pDateUserCnt->arrUserCnts[2]);
		m_GridStaConnectUser.SetItem(&Item);

		Item.col	= nCol++;
		Item.row	= nNewRowIdx;
		Item.strText.Format("%4.1f", ( (pDateUserCnt->arrUserCnts[1]+pDateUserCnt->arrUserCnts[2])==0 ) ? 0 : (100 * (float)pDateUserCnt->arrUserCnts[1])/(pDateUserCnt->arrUserCnts[1]+pDateUserCnt->arrUserCnts[2]));
		m_GridStaConnectUser.SetItem(&Item);

		Item.col	= nCol++;
		Item.row	= nNewRowIdx;
		Item.strText.Format("%4.1f", ( (pDateUserCnt->arrUserCnts[1]+pDateUserCnt->arrUserCnts[2])==0 ) ? 0 : (100 * (float)pDateUserCnt->arrUserCnts[2])/(pDateUserCnt->arrUserCnts[1]+pDateUserCnt->arrUserCnts[2]));
		m_GridStaConnectUser.SetItem(&Item);
	}
	m_GridStaConnectUser.UpdateData();
	m_GridStaConnectUser.AutoSize();
}

void CStatisticPage9Dlg::OnButtonSt9Save() 
{
	// TODO: Add your control notification handler code here
	CString tempStr;
	for(int i = 0; i<m_GridStaConnectUser.GetRowCount();i++)
	{
		tempStr = "";
		for(int j = 0; j< m_GridStaConnectUser.GetColumnCount();j++)
		{
			tempStr += m_GridStaConnectUser.GetItemText( i, j);
			tempStr += "|\t";
		}
		m_pMainDlg->m_pWriteLog->WriteLogFile((char*)(LPCTSTR)tempStr, tempStr.GetLength(), m_pMainDlg, 1);
	}
}

void CStatisticPage9Dlg::OnCheck1() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	m_valueCheckEachDay			= FALSE;

	UpdateData(FALSE);
}

void CStatisticPage9Dlg::OnCheck2() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	m_valueCheckConnectionUser	= FALSE;

	UpdateData(FALSE);	
}
