// StatisticPage1Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "spacecowboydbeditortool.h"
#include "StatisticPage1Dlg.h"
#include "SpaceCowboyDBEditorToolDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStatisticPage1Dlg property page

IMPLEMENT_DYNCREATE(CStatisticPage1Dlg, CPropertyPage)

CStatisticPage1Dlg::CStatisticPage1Dlg() : CPropertyPage(CStatisticPage1Dlg::IDD)
{
	//{{AFX_DATA_INIT(CStatisticPage1Dlg)
	m_valueCheckDailyAccount = FALSE;
	m_valueDTStartDate = COleDateTime::GetCurrentTime() - COleDateTimeSpan(1, 0 , 0, 0);
	m_valueDTEndDate = COleDateTime::GetCurrentTime();
	m_valueCheckUnregAccount = FALSE;
	m_valueCheckNewAccountAge = FALSE;
	m_valueDTNewAccountAge = COleDateTime::GetCurrentTime();
	m_valueDTSameAccountStartDate = COleDateTime::GetCurrentTime() - COleDateTimeSpan(1, 0 , 0, 0);
	m_valueDTSameAccountEndDate = COleDateTime::GetCurrentTime();
	m_valueCheckSameAccount = FALSE;
	m_valueDTStartTime = COleDateTime::GetCurrentTime();
	m_valueDTEndTime = COleDateTime::GetCurrentTime();
	m_valueDTNewAccountAgeTime = COleDateTime::GetCurrentTime();
	m_valueDTSameAccountStartTime = COleDateTime::GetCurrentTime();
	m_valueDTSameAccountEndTime = COleDateTime::GetCurrentTime();
	//}}AFX_DATA_INIT
	m_pMainDlg = (CSpaceCowboyDBEditorToolDlg*)AfxGetMainWnd();
	m_pODBCStmt				= m_pMainDlg->m_pODBCStmt;
}

CStatisticPage1Dlg::~CStatisticPage1Dlg()
{
// 2006-01-23 by cmkwon
//	util::del(m_pODBCStmt);
}

void CStatisticPage1Dlg::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStatisticPage1Dlg)
	DDX_Check(pDX, IDC_CHECK1, m_valueCheckDailyAccount);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_ST1_DAILY_ACCOUNT_STARTDATE, m_valueDTStartDate);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_ST1_DAILY_ACCOUNT_ENDDATE, m_valueDTEndDate);
	DDX_Check(pDX, IDC_CHECK2, m_valueCheckUnregAccount);
	DDX_Check(pDX, IDC_CHECK3, m_valueCheckNewAccountAge);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_ST1_NEW_ACCOUNT_DATE, m_valueDTNewAccountAge);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_ST1_SAME_ACCOUNT_STARTDATE, m_valueDTSameAccountStartDate);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_ST1_SAME_ACCOUNT_ENDDATE, m_valueDTSameAccountEndDate);
	DDX_Check(pDX, IDC_CHECK4, m_valueCheckSameAccount);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_ST1_DAILY_ACCOUNT_STARTTIME, m_valueDTStartTime);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_ST1_DAILY_ACCOUNT_ENDTIME, m_valueDTEndTime);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_ST1_DAILY_ACCOUNT_STARTTIME2, m_valueDTNewAccountAgeTime);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_ST1_DAILY_ACCOUNT_STARTTIME3, m_valueDTSameAccountStartTime);
	DDX_Control(pDX, IDC_CUSTOM_STA_NEWACCOUNT, m_GridStaNewAccount);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_ST1_DAILY_ACCOUNT_ENDTIME2, m_valueDTSameAccountEndTime);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CStatisticPage1Dlg, CPropertyPage)
	//{{AFX_MSG_MAP(CStatisticPage1Dlg)
	ON_BN_CLICKED(IDC_BUTTON_ST1_SELECT, OnButtonSt1Select)
	ON_BN_CLICKED(IDC_BUTTON_ST1_SAVE, OnButtonSt1Save)
	ON_BN_CLICKED(IDC_CHECK1, OnCheck1)
	ON_BN_CLICKED(IDC_CHECK2, OnCheck2)
	ON_BN_CLICKED(IDC_CHECK3, OnCheck3)
	ON_BN_CLICKED(IDC_CHECK4, OnCheck4)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStatisticPage1Dlg message handlers

void CStatisticPage1Dlg::OnButtonSt1Select() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if(m_valueCheckDailyAccount == TRUE)
	{
		ViewDailyAccount();
	}
	if(m_valueCheckUnregAccount == TRUE)
	{
		ViewUnRegAccount();
	}
	if(m_valueCheckNewAccountAge == TRUE)
	{
		ViewNewAccountAge();
	}
	if(m_valueCheckSameAccount == TRUE)
	{
		ViewSameAccount();
	}
	
}

struct STM_REGISTERDATE_DATA			// 2006-01-18 by cmkwon
{
	char		szRegisteredData[20];	// 가입일자
	int			nTotalCount;			// 총가입자수
	int			nCharCreatedCount;		// 캐릭터 생성 유저수

	void ResetTM_REGISTERDATE_DATA(void)
	{
		util::zero(szRegisteredData, 20);
		nTotalCount			= 0;
		nCharCreatedCount	= 0;
	}		
};

void CStatisticPage1Dlg::ViewDailyAccount()
{
	vector<STM_REGISTERDATE_DATA>	vectSTM_REGISTERDATE_DATA;
	STM_REGISTERDATE_DATA			tmRegisterData;

	CString szStartDate, szEndDate;
// 2008-01-24 by cmkwon, S_A: 날짜 관련 로컬리 제이션 처리 버그 수정 - 아래와 같은 형식으로 수정
// 	szStartDate.Format("%04d-%02d-%02d %02d:%02d:%02d", m_valueDTStartDate.GetYear(), m_valueDTStartDate.GetMonth(), m_valueDTStartDate.GetDay()
// 		, m_valueDTStartTime.GetHour(), m_valueDTStartTime.GetMinute(), m_valueDTStartTime.GetSecond());
// 	szEndDate.Format("%04d-%02d-%02d %02d:%02d:%02d", m_valueDTEndDate.GetYear(), m_valueDTEndDate.GetMonth(), m_valueDTEndDate.GetDay()
// 		, m_valueDTEndTime.GetHour(), m_valueDTEndTime.GetMinute(), m_valueDTEndTime.GetSecond());
// 2008-08-28 by cmkwon, MySQL 관련 날짜시간 관련 오류 수정 - 
//	szStartDate.Format("%04d%02d%02d %02d:%02d:%02d", m_valueDTStartDate.GetYear(), m_valueDTStartDate.GetMonth(), m_valueDTStartDate.GetDay()
	szStartDate.Format(SQL_DATETIME_STRING_FORMAT, m_valueDTStartDate.GetYear(), m_valueDTStartDate.GetMonth(), m_valueDTStartDate.GetDay()
		, m_valueDTStartTime.GetHour(), m_valueDTStartTime.GetMinute(), m_valueDTStartTime.GetSecond());
// 2008-08-28 by cmkwon, MySQL 관련 날짜시간 관련 오류 수정 - 
//	szEndDate.Format("%04d%02d%02d %02d:%02d:%02d", m_valueDTEndDate.GetYear(), m_valueDTEndDate.GetMonth(), m_valueDTEndDate.GetDay()
	szEndDate.Format(SQL_DATETIME_STRING_FORMAT, m_valueDTEndDate.GetYear(), m_valueDTEndDate.GetMonth(), m_valueDTEndDate.GetDay()
		, m_valueDTEndTime.GetHour(), m_valueDTEndTime.GetMinute(), m_valueDTEndTime.GetSecond());

	//데이터 insert
	//DB 접속
// 2006-01-23 by cmkwon
//	if (!m_pODBCStmt->Init(m_pMainDlg->m_pServerInfo4Admin->DBIP, m_pMainDlg->m_pServerInfo4Admin->DBPort, m_pMainDlg->m_pServerInfo4Admin->DBName,
//							m_pMainDlg->m_pServerInfo4Admin->DBUID, m_pMainDlg->m_pServerInfo4Admin->DBPWD, GetSafeHwnd())
//	)
//	{
//		return;
//	}

	CString szSQLQuery;

	///////////////////////////////////////////////////////////////////////////////
	// 2006-01-18 by cmkwon, 날짜별 총 가입자수 구하기
// 2006-01-18 by cmkwon
//	szSQLQuery.Format("select time, clientcount	from atum_log_server_map where mapindex = 0 and time > '%s' and time < '%s' order by time desc"
//		, szStartDate, szEndDate);

// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
// 	szSQLQuery.Format("SELECT dbo.atum_getonlydate(RegisteredDate), count(*) \
// 						FROM td_account WITH (NOLOCK) \
// 						WHERE RegisteredDate >= '%s' and RegisteredDate <= '%s' and accounttype = 0 \
// 						GROUP BY dbo.atum_getonlydate(RegisteredDate) \
// 						ORDER BY dbo.atum_getonlydate(RegisteredDate) ", szStartDate, szEndDate);

// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
// 	szSQLQuery.Format(QUERY_080702_0135, szStartDate, szEndDate);
// 	BOOL bRet = m_pODBCStmt->ExecuteQuery((LPCSTR)szSQLQuery);
	SQLHSTMT hstmt = m_pODBCStmt->GetSTMTHandle();
	SQLINTEGER arrCB2[3] = {SQL_NTS,SQL_NTS,SQL_NTS};
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SQL_DATETIME_STRING, 0, (LPSTR)(LPCSTR)szStartDate, 0,	&arrCB2[1]);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SQL_DATETIME_STRING, 0, (LPSTR)(LPCSTR)szEndDate, 0,		&arrCB2[2]);
	BOOL bRet = m_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_080827_0135));
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

	while ( (bRet = SQLFetch(m_pODBCStmt->m_hstmt)) != SQL_NO_DATA)
	{
		tmRegisterData.ResetTM_REGISTERDATE_DATA();		
		strcpy(tmRegisterData.szRegisteredData, tempDate);
		tmRegisterData.nTotalCount		= tempSelectData;

		vectSTM_REGISTERDATE_DATA.push_back(tmRegisterData);
	}
	// clean up
	m_pODBCStmt->FreeStatement();
	// 2006-01-18 by cmkwon
	///////////////////////////////////////////////////////////////////////////////
	
	///////////////////////////////////////////////////////////////////////////////
	// 2006-01-18 by cmkwon, 날짜별 캐릭을 만든 총 가입자수 구하기
// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
// 	szSQLQuery.Format("SELECT dbo.atum_getonlydate(RegisteredDate), count(*) \
// 						FROM td_account WITH (NOLOCK) \
// 						WHERE RegisteredDate >= '%s' and RegisteredDate <= '%s' and accounttype = 0 \
// 						and AccountUniqueNumber in (select DISTINCT a.accountuniquenumber from td_account a, td_character c where a.RegisteredDate >= '%s' and a.RegisteredDate <='%s' and a.AccountuniqueNumber = c.AccountUniqueNumber) \
// 						GROUP BY dbo.atum_getonlydate(RegisteredDate) \
// 						ORDER BY dbo.atum_getonlydate(RegisteredDate)", szStartDate, szEndDate, szStartDate, szEndDate);

// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
//	szSQLQuery.Format(QUERY_080702_0136, szStartDate, szEndDate, szStartDate, szEndDate);
//	bRet = m_pODBCStmt->ExecuteQuery((LPCSTR)szSQLQuery);
	hstmt = m_pODBCStmt->GetSTMTHandle();
	arrCB2[1] = arrCB2[2] = SQL_NTS;
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SQL_DATETIME_STRING, 0, (LPSTR)(LPCSTR)szStartDate, 0,	&arrCB2[1]);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SQL_DATETIME_STRING, 0, (LPSTR)(LPCSTR)szEndDate, 0,		&arrCB2[2]);
	bRet = m_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_080827_0136));
	if (!bRet)
	{
		// clean up
		m_pODBCStmt->FreeStatement();
		MessageBox("Error");
		return;
	}


	memset(tempDate, 0, sizeof(tempDate));
	SQLBindCol(m_pODBCStmt->m_hstmt, 1, SQL_C_CHAR, &tempDate, 20, &cb1);
	SQLBindCol(m_pODBCStmt->m_hstmt, 2, SQL_C_ULONG, &tempSelectData, 0, &cb2);

	while ( (bRet = SQLFetch(m_pODBCStmt->m_hstmt)) != SQL_NO_DATA)
	{
		for(int i=0; i < vectSTM_REGISTERDATE_DATA.size(); i++)
		{
			if(0 == strcmp(tempDate, vectSTM_REGISTERDATE_DATA[i].szRegisteredData))
			{
				vectSTM_REGISTERDATE_DATA[i].nCharCreatedCount	= tempSelectData;
			}
		}
	}
	// clean up
	m_pODBCStmt->FreeStatement();
	// 2006-01-18 by cmkwon
	///////////////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////////////
	// 2006-01-18 by cmkwon, 그리드 처리
	m_GridStaNewAccount.SetBkColor(0xFFFFFF);
	m_GridStaNewAccount.SetHeaderSort();

	m_GridStaNewAccount.SetEditable(FALSE);
	m_GridStaNewAccount.SetListMode(TRUE);
	m_GridStaNewAccount.SetSingleRowSelection(TRUE);
	m_GridStaNewAccount.EnableSelection(TRUE);
	m_GridStaNewAccount.SetFrameFocusCell(FALSE);
	m_GridStaNewAccount.SetTrackFocusCell(FALSE);

	m_GridStaNewAccount.SetRowCount(1);
	m_GridStaNewAccount.SetColumnCount(3);
	m_GridStaNewAccount.SetFixedRowCount(1);

	// 칼럼 만들기
	GV_ITEM Item;
	Item.mask		= GVIF_TEXT|GVIF_FORMAT;
	Item.nFormat	= DT_LEFT|DT_VCENTER|DT_SINGLELINE;
	Item.row		= 0;
	Item.col		= 0;

	Item.col		= 0;
	Item.strText.Format("Join Date");
	m_GridStaNewAccount.SetItem(&Item);

	Item.col		= 1;
	Item.strText.Format("Total New Member");
	m_GridStaNewAccount.SetItem(&Item);

	Item.col		= 2;
	Item.strText.Format("created character count of Total New Member");
	m_GridStaNewAccount.SetItem(&Item);

	for(int i=0; i < vectSTM_REGISTERDATE_DATA.size(); i++)
	{
		int nNewRowIdx = m_GridStaNewAccount.GetRowCount();
		m_GridStaNewAccount.SetRowCount(nNewRowIdx+1);

		Item.row		= nNewRowIdx;
		Item.col		= 0;
		Item.strText.Format("%s", vectSTM_REGISTERDATE_DATA[i].szRegisteredData);
		m_GridStaNewAccount.SetItem(&Item);

		Item.row		= nNewRowIdx;
		Item.col		= 1;
		Item.strText.Format("%d", vectSTM_REGISTERDATE_DATA[i].nTotalCount);
		m_GridStaNewAccount.SetItem(&Item);

		Item.row		= nNewRowIdx;
		Item.col		= 2;
		Item.strText.Format("%d", vectSTM_REGISTERDATE_DATA[i].nCharCreatedCount);
		m_GridStaNewAccount.SetItem(&Item);
	}
	m_GridStaNewAccount.UpdateData();

	m_GridStaNewAccount.AutoSize();
}

void CStatisticPage1Dlg::ViewUnRegAccount()
{
	m_GridStaNewAccount.SetBkColor(0xFFFFFF);
	m_GridStaNewAccount.SetHeaderSort();

	int m_nRows = 1;
	int m_nCols = 1;

	m_GridStaNewAccount.SetEditable(FALSE);
	m_GridStaNewAccount.SetListMode(TRUE);
	m_GridStaNewAccount.SetSingleRowSelection(TRUE);
	m_GridStaNewAccount.EnableSelection(TRUE);
	m_GridStaNewAccount.SetFrameFocusCell(FALSE);
	m_GridStaNewAccount.SetTrackFocusCell(FALSE);

	m_GridStaNewAccount.SetRowCount(m_nRows);
	m_GridStaNewAccount.SetColumnCount(m_nCols);
	m_GridStaNewAccount.SetFixedRowCount(1);

	// 칼럼 만들기
	m_nCols = 0;
	GV_ITEM Item;
	Item.mask = GVIF_TEXT|GVIF_FORMAT;
	Item.row = 0;
	Item.nFormat = DT_LEFT|DT_VCENTER|DT_SINGLELINE;

	Item.col = m_nCols++;
	Item.strText.Format("Number of people");
	m_GridStaNewAccount.SetItem(&Item);

	//데이터 insert
	//DB 접속
// 2006-01-23 by cmkwon
//	if (!m_pODBCStmt->Init(m_pMainDlg->m_pServerInfo4Admin->DBIP, m_pMainDlg->m_pServerInfo4Admin->DBPort, m_pMainDlg->m_pServerInfo4Admin->DBName,
//							m_pMainDlg->m_pServerInfo4Admin->DBUID, m_pMainDlg->m_pServerInfo4Admin->DBPWD, GetSafeHwnd())
//	)
//	{
//		return;
//	}

	///////////////////////////////////////////////////////////////////////////////
	// 2005-11-28 by cmkwon, 2주이상 접속하지 않은 유저
	CString szSQLQuery;
	// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
	//szSQLQuery.Format("select count(*) from td_account WITH (NOLOCK) where lastlogindate < dateadd(ww, -2, getdate())");

// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
// 	szSQLQuery.Format(QUERY_080702_0137);
// 	BOOL bRet = m_pODBCStmt->ExecuteQuery((LPCSTR)szSQLQuery);
	int tmTermDay = -14;	// 2주
	SQLHSTMT hstmt = m_pODBCStmt->GetSTMTHandle();
	SQLINTEGER arrCB2[2] = {SQL_NTS,SQL_NTS};
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &(tmTermDay), 0,	&arrCB2[1]);
	BOOL bRet = m_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_080827_0137));
	if (!bRet)
	{
		// clean up
		m_pODBCStmt->FreeStatement();
		MessageBox("Error");
		return;
	}

	SQLINTEGER cb1;

	int tempSelectData;
	SQLBindCol(m_pODBCStmt->m_hstmt, 1, SQL_C_ULONG, &tempSelectData, 0, &cb1);


	int nCount = 0;
	m_GridStaNewAccount.SetRowCount(nCount+1);
	// DB에 값이 없을때까지 loop를 돈다
	while ( (bRet = SQLFetch(m_pODBCStmt->m_hstmt)) != SQL_NO_DATA)
	{
		m_GridStaNewAccount.SetRowCount(nCount+2);

		//select된 값을 GridDetail GridCtrl에 넣어준다.
		Item.col = 0;
		Item.row = 1;
		Item.strText.Format("%d", tempSelectData);
		m_GridStaNewAccount.SetItem(&Item);
		m_GridStaNewAccount.UpdateData();

		nCount++;
	}

	// clean up
	m_pODBCStmt->FreeStatement();

	m_GridStaNewAccount.AutoSize();
}

void CStatisticPage1Dlg::ViewNewAccountAge()
{
	m_GridStaNewAccount.SetBkColor(0xFFFFFF);
	m_GridStaNewAccount.SetHeaderSort();

	int m_nRows = 1;
	int m_nCols = 2;

	m_GridStaNewAccount.SetEditable(FALSE);
	m_GridStaNewAccount.SetListMode(TRUE);
	m_GridStaNewAccount.SetSingleRowSelection(TRUE);
	m_GridStaNewAccount.EnableSelection(TRUE);
	m_GridStaNewAccount.SetFrameFocusCell(FALSE);
	m_GridStaNewAccount.SetTrackFocusCell(FALSE);

	m_GridStaNewAccount.SetRowCount(m_nRows);
	m_GridStaNewAccount.SetColumnCount(m_nCols);
	m_GridStaNewAccount.SetFixedRowCount(1);

	// 칼럼 만들기
	m_nCols = 0;
	GV_ITEM Item;
	Item.mask = GVIF_TEXT|GVIF_FORMAT;
	Item.row = 0;
	Item.nFormat = DT_LEFT|DT_VCENTER|DT_SINGLELINE;

	Item.col = m_nCols++;
	Item.strText.Format("BIRTHYEAR");
	m_GridStaNewAccount.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("Number of people");
	m_GridStaNewAccount.SetItem(&Item);

	//데이터 insert
	//DB 접속
// 2006-01-23 by cmkwon
//	if (!m_pODBCStmt->Init(m_pMainDlg->m_pServerInfo4Admin->DBIP, m_pMainDlg->m_pServerInfo4Admin->DBPort, m_pMainDlg->m_pServerInfo4Admin->DBName,
//							m_pMainDlg->m_pServerInfo4Admin->DBUID, m_pMainDlg->m_pServerInfo4Admin->DBPWD, GetSafeHwnd())
//	)
//	{
//		return;
//	}
	CString szNewAccountAge;
// 2008-01-24 by cmkwon, S_A: 날짜 관련 로컬리 제이션 처리 버그 수정 - 아래와 같은 형식으로 수정
// 	szNewAccountAge.Format("%04d-%02d-%02d %02d:%02d:%02d", m_valueDTNewAccountAge.GetYear(), m_valueDTNewAccountAge.GetMonth(), m_valueDTNewAccountAge.GetDay()
// 		, m_valueDTNewAccountAgeTime.GetHour(), m_valueDTNewAccountAgeTime.GetMinute(), m_valueDTNewAccountAgeTime.GetSecond());
// 2008-08-28 by cmkwon, MySQL 관련 날짜시간 관련 오류 수정 - 
//	szNewAccountAge.Format("%04d%02d%02d %02d:%02d:%02d", m_valueDTNewAccountAge.GetYear(), m_valueDTNewAccountAge.GetMonth(), m_valueDTNewAccountAge.GetDay()
	szNewAccountAge.Format(SQL_DATETIME_STRING_FORMAT, m_valueDTNewAccountAge.GetYear(), m_valueDTNewAccountAge.GetMonth(), m_valueDTNewAccountAge.GetDay()
		, m_valueDTNewAccountAgeTime.GetHour(), m_valueDTNewAccountAgeTime.GetMinute(), m_valueDTNewAccountAgeTime.GetSecond());

	CString szSQLQuery;
	// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
	//szSQLQuery.Format("select birthyear, count(*) from td_account WITH (NOLOCK) where registereddate >= '%s' group by birthyear order by birthyear"
	//	, szNewAccountAge);

// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
//	szSQLQuery.Format(QUERY_080702_0138, szNewAccountAge);
//	BOOL bRet = m_pODBCStmt->ExecuteQuery((LPCSTR)szSQLQuery);
	SQLHSTMT hstmt = m_pODBCStmt->GetSTMTHandle();
	SQLINTEGER arrCB2[2] = {SQL_NTS,SQL_NTS};
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SQL_DATETIME_STRING, 0, (LPSTR)(LPCSTR)szNewAccountAge, 0,	&arrCB2[1]);
	BOOL bRet = m_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_080827_0138));
	if (!bRet)
	{
		// clean up
		m_pODBCStmt->FreeStatement();
		MessageBox("Error");
		return;
	}

	SQLINTEGER cb1, cb2;

	char tempDate[9];
	memset(tempDate, 0, sizeof(tempDate));
	int tempSelectData;
	SQLBindCol(m_pODBCStmt->m_hstmt, 1, SQL_C_CHAR, &tempDate, 20, &cb1);
	SQLBindCol(m_pODBCStmt->m_hstmt, 2, SQL_C_ULONG, &tempSelectData, 0, &cb2);


	int nCount = 0;
	m_GridStaNewAccount.SetRowCount(nCount+1);
	// DB에 값이 없을때까지 loop를 돈다
	while ( (bRet = SQLFetch(m_pODBCStmt->m_hstmt)) != SQL_NO_DATA)
	{
		m_GridStaNewAccount.SetRowCount(nCount+2);

		//select된 값을 GridDetail GridCtrl에 넣어준다.
		Item.col = 0;
		Item.row = nCount+1;
		Item.strText.Format("%s", tempDate);
		m_GridStaNewAccount.SetItem(&Item);

		Item.col = 1;
		Item.strText.Format("%d", tempSelectData);
		m_GridStaNewAccount.SetItem(&Item);
		m_GridStaNewAccount.UpdateData();

		nCount++;
	}

	// clean up
	m_pODBCStmt->FreeStatement();

	m_GridStaNewAccount.AutoSize();
}

void CStatisticPage1Dlg::ViewSameAccount()
{
	m_GridStaNewAccount.SetBkColor(0xFFFFFF);
	m_GridStaNewAccount.SetHeaderSort();

	int m_nRows = 1;
	int m_nCols = 2;

	m_GridStaNewAccount.SetEditable(FALSE);
	m_GridStaNewAccount.SetListMode(TRUE);
	m_GridStaNewAccount.SetSingleRowSelection(TRUE);
	m_GridStaNewAccount.EnableSelection(TRUE);
	m_GridStaNewAccount.SetFrameFocusCell(FALSE);
	m_GridStaNewAccount.SetTrackFocusCell(FALSE);

	m_GridStaNewAccount.SetRowCount(m_nRows);
	m_GridStaNewAccount.SetColumnCount(m_nCols);
	m_GridStaNewAccount.SetFixedRowCount(1);

	// 칼럼 만들기
	m_nCols = 0;
	GV_ITEM Item;
	Item.mask = GVIF_TEXT|GVIF_FORMAT;
	Item.row = 0;
	Item.nFormat = DT_LEFT|DT_VCENTER|DT_SINGLELINE;

	Item.col = m_nCols++;
	Item.strText.Format("DATE");
	m_GridStaNewAccount.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("ACCOUNT");
	m_GridStaNewAccount.SetItem(&Item);

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
// 	szStartDate.Format("%04d-%02d-%02d %02d:%02d:%02d", m_valueDTSameAccountStartDate.GetYear(), m_valueDTSameAccountStartDate.GetMonth(), m_valueDTSameAccountStartDate.GetDay()
// 		, m_valueDTSameAccountStartTime.GetHour(), m_valueDTSameAccountStartTime.GetMinute(), m_valueDTSameAccountStartTime.GetSecond());
// 	szEndDate.Format("%04d-%02d-%02d %02d:%02d:%02d", m_valueDTSameAccountEndDate.GetYear(), m_valueDTSameAccountEndDate.GetMonth(), m_valueDTSameAccountEndDate.GetDay()
// 		, m_valueDTSameAccountEndTime.GetHour(), m_valueDTSameAccountEndTime.GetMinute(), m_valueDTSameAccountEndTime.GetSecond());
// 2008-08-28 by cmkwon, MySQL 관련 날짜시간 관련 오류 수정 - 
//	szStartDate.Format("%04d%02d%02d %02d:%02d:%02d", m_valueDTSameAccountStartDate.GetYear(), m_valueDTSameAccountStartDate.GetMonth(), m_valueDTSameAccountStartDate.GetDay()
	szStartDate.Format(SQL_DATETIME_STRING_FORMAT, m_valueDTSameAccountStartDate.GetYear(), m_valueDTSameAccountStartDate.GetMonth(), m_valueDTSameAccountStartDate.GetDay()
		, m_valueDTSameAccountStartTime.GetHour(), m_valueDTSameAccountStartTime.GetMinute(), m_valueDTSameAccountStartTime.GetSecond());
// 2008-08-28 by cmkwon, MySQL 관련 날짜시간 관련 오류 수정 - 
//	szEndDate.Format("%04d%02d%02d %02d:%02d:%02d", m_valueDTSameAccountEndDate.GetYear(), m_valueDTSameAccountEndDate.GetMonth(), m_valueDTSameAccountEndDate.GetDay()
	szEndDate.Format(SQL_DATETIME_STRING_FORMAT, m_valueDTSameAccountEndDate.GetYear(), m_valueDTSameAccountEndDate.GetMonth(), m_valueDTSameAccountEndDate.GetDay()
		, m_valueDTSameAccountEndTime.GetHour(), m_valueDTSameAccountEndTime.GetMinute(), m_valueDTSameAccountEndTime.GetSecond());

	CString szSQLQuery;
// 2006-02-02 by cmkwon
//	szSQLQuery.Format("select time, clientcount	from atum_log_server_map where mapindex = 0 and time > '%s' and time < '%s' order by time desc"
//		, szStartDate, szEndDate);

	// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
	//szSQLQuery.Format("DROP VIEW log_server_map");
	szSQLQuery.Format(QUERY_080702_0139);
// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
//	BOOL bRet = m_pODBCStmt->ExecuteQuery((LPCSTR)szSQLQuery);	
	SQLHSTMT hstmt = m_pODBCStmt->GetSTMTHandle();
	SQLINTEGER arrCB2[2] = {SQL_NTS,SQL_NTS};
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SZQUERY, 0, (LPSTR)(LPCSTR)szSQLQuery, 0,			&arrCB2[1]);
	BOOL bRet = m_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_080827_0122));
	m_pODBCStmt->FreeStatement();

// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
// 	szSQLQuery.Format("CREATE VIEW log_server_map AS \
// 		SELECT time, clientCount \
// 		FROM atum_backup_log_server_map WITH (NOLOCK) \
// 		WHERE mapindex = 0 and time > '%s' and time < '%s' \
// 		UNION ALL \
// 		SELECT time, clientCount \
// 		FROM atum_log_server_map WITH (NOLOCK) \
// 		WHERE mapindex = 0 and time > '%s' and time < '%s'", szStartDate, szEndDate, szStartDate, szEndDate);
	szSQLQuery.Format(QUERY_080702_0140, szStartDate, szEndDate, szStartDate, szEndDate);

// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
//	bRet = m_pODBCStmt->ExecuteQuery((LPCSTR)szSQLQuery);
	hstmt = m_pODBCStmt->GetSTMTHandle();
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
	//szSQLQuery.Format("SELECT time, clientCount \
	//	FROM log_server_map \
	//	ORDER BY time DESC");

// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
//	szSQLQuery.Format(QUERY_080702_0141);
//	bRet = m_pODBCStmt->ExecuteQuery((LPCSTR)szSQLQuery);
	bRet = m_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_080827_0141));
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
	m_GridStaNewAccount.SetRowCount(nCount+1);
	// DB에 값이 없을때까지 loop를 돈다
	while ( (bRet = SQLFetch(m_pODBCStmt->m_hstmt)) != SQL_NO_DATA)
	{
		m_GridStaNewAccount.SetRowCount(nCount+2);

		//select된 값을 GridDetail GridCtrl에 넣어준다.
		Item.col = 0;
		Item.row = nCount+1;
		Item.strText.Format("%s", tempDate);
		m_GridStaNewAccount.SetItem(&Item);

		Item.col = 1;
		Item.strText.Format("%d", tempSelectData);
		m_GridStaNewAccount.SetItem(&Item);
		m_GridStaNewAccount.UpdateData();

		nCount++;
	}

	// clean up
	m_pODBCStmt->FreeStatement();

	m_GridStaNewAccount.AutoSize();
}

void CStatisticPage1Dlg::OnButtonSt1Save() 
{
	// TODO: Add your control notification handler code here
	CString tempStr;
	for(int i = 0; i<m_GridStaNewAccount.GetRowCount();i++)
	{
		tempStr = "";
		for(int j = 0; j< m_GridStaNewAccount.GetColumnCount();j++)
		{
			tempStr += m_GridStaNewAccount.GetItemText( i, j);
			tempStr += "|\t";
		}
		m_pMainDlg->m_pWriteLog->WriteLogFile((char*)(LPCTSTR)tempStr, tempStr.GetLength(), m_pMainDlg, 1);
	}
}

void CStatisticPage1Dlg::OnCheck1() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if(m_valueCheckDailyAccount == TRUE)
	{
		m_valueCheckUnregAccount = FALSE;
		m_valueCheckNewAccountAge = FALSE;
		m_valueCheckSameAccount = FALSE;
	}
	UpdateData(FALSE);
}

void CStatisticPage1Dlg::OnCheck2() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if(m_valueCheckUnregAccount  == TRUE)
	{
		m_valueCheckDailyAccount = FALSE;
		m_valueCheckNewAccountAge = FALSE;
		m_valueCheckSameAccount = FALSE;
	}
	UpdateData(FALSE);
}

void CStatisticPage1Dlg::OnCheck3() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if(m_valueCheckNewAccountAge  == TRUE)
	{
		m_valueCheckUnregAccount = FALSE;
		m_valueCheckDailyAccount = FALSE;
		m_valueCheckSameAccount = FALSE;
	}
	UpdateData(FALSE);
}

void CStatisticPage1Dlg::OnCheck4() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if(m_valueCheckSameAccount  == TRUE)
	{
		m_valueCheckUnregAccount = FALSE;
		m_valueCheckNewAccountAge = FALSE;
		m_valueCheckDailyAccount = FALSE;
	}
	UpdateData(FALSE);
}
