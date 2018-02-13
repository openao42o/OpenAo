// LogTabConnection.cpp : implementation file
//

#include "stdafx.h"
#include "LogTabConnection.h"
#include "SCGridHelper.h"
#include "SCLogAdminDlg.h"
#include "SystemLogManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLogTabConnection dialog


CLogTabConnection::CLogTabConnection(CDialog *i_pMainDlg, CWnd* pParent /* = NULL */)
	: CDialog(CLogTabConnection::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLogTabConnection)
	m_bCheckAccountName = FALSE;
	m_bCheckIPAddress = FALSE;
	m_szAccountName = _T("");
	m_bCheckCharacterName = FALSE;
	m_szCharacterName = _T("");
	m_bCheckDate = TRUE;
	m_dateStart = (CTime::GetCurrentTime() - CTimeSpan(1, 0 , 0, 0));
	m_dateEnd = CTime::GetCurrentTime();
	m_bCheckMaxRowCount = TRUE;
	m_nMaxRowCount = 100;
	m_bCheckLogLogin = FALSE;
	m_bCheckLogLogout = FALSE;
	//}}AFX_DATA_INIT
	m_pMainDlg = (CSCLogAdminDlg*)i_pMainDlg;
	// 2007-11-08 by cmkwon, 모든 로그 검색 기능 SCDBTool로 이동 -
	//m_pODBCStmt = &m_pMainDlg->m_ODBCStmt;
	m_pODBCStmt = m_pMainDlg->m_pODBCStmt;		// 2007-11-08 by cmkwon, 모든 로그 검색 기능 SCDBTool로 이동 -
}


void CLogTabConnection::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLogTabConnection)
	DDX_Control(pDX, IDC_IPADDRESS, m_ctlIPAddress);
	DDX_Check(pDX, IDC_CHECK_ACCOUNT_NAME, m_bCheckAccountName);
	DDX_Check(pDX, IDC_CHECK_IP_ADDRESS, m_bCheckIPAddress);
	DDX_Text(pDX, IDC_EDIT_ACCOUNT_NAME, m_szAccountName);
	DDX_Check(pDX, IDC_CHECK_CHARACTER, m_bCheckCharacterName);
	DDX_Text(pDX, IDC_EDIT_CHARACTER_NAME, m_szCharacterName);
	DDX_Check(pDX, IDC_CHECK_DATE, m_bCheckDate);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_START_DATE, m_dateStart);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_END_DATE, m_dateEnd);
	DDX_Check(pDX, IDC_CHECK_MAX_ROW_COUNT, m_bCheckMaxRowCount);
	DDX_Text(pDX, IDC_EDIT_MAX_ROW_COUNT, m_nMaxRowCount);
	DDX_Control(pDX, IDC_EDIT_TOTAL_LOG_ROW_COUNT, m_EditTotalLogRowCount);
	DDX_Check(pDX, IDC_CHECK_LOG_LOGIN, m_bCheckLogLogin);
	DDX_Check(pDX, IDC_CHECK_LOG_LOGOUT, m_bCheckLogLogout);
	//}}AFX_DATA_MAP
	DDX_GridControl(pDX, IDC_GRID_CONNECTION_LOG, m_GridConnectionLog);
}


BEGIN_MESSAGE_MAP(CLogTabConnection, CDialog)
	//{{AFX_MSG_MAP(CLogTabConnection)
	ON_BN_CLICKED(IDC_BUTTON_OK, OnButtonOk)
	ON_BN_CLICKED(IDC_BUTTON_RESET, OnButtonReset)
	ON_BN_CLICKED(IDC_CHECK_ACCOUNT_NAME, OnCheckAccountName)
	ON_BN_CLICKED(IDC_CHECK_IP_ADDRESS, OnCheckIPAddress)
	ON_BN_CLICKED(IDC_CHECK_CHARACTER, OnCheckCharacter)
	ON_BN_CLICKED(IDC_CHECK_DATE, OnCheckDate)
	ON_BN_CLICKED(IDC_CHECK_MAX_ROW_COUNT, OnCheckMaxRowCount)
	ON_BN_CLICKED(IDC_BUTTON_SELECT_ALL, OnButtonSelectAll)
	ON_BN_CLICKED(IDC_BUTTON_DISELECT_ALL, OnButtonDiselectAll)
	ON_BN_CLICKED(IDC_BTN_SAVE_RESULT, OnBtnSaveResult)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLogTabConnection message handlers

BOOL CLogTabConnection::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)
	{
		m_pMainDlg->EndDialog(-1);
		return TRUE;
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}

BOOL CLogTabConnection::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_GridConnectionLog.InitGrid(NUM_COLUMNS_CONNECTION_LOG_GRID, g_arrConnectionLogColumnNames);

	ResetVariables();
	ResetControls();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CLogTabConnection::ResetVariables()
{
	m_szCharacterName = _T("");
	m_szAccountName = _T("");
	m_bCheckCharacterName = FALSE;
	m_bCheckAccountName = FALSE;
	m_bCheckIPAddress = FALSE;			// 2008-01-11 by cmkwon, T_A: ConnectionLog 에 IP 검색 기능 추가 - 

	m_bCheckDate = TRUE;
	m_dateStart = (CTime::GetCurrentTime() - CTimeSpan(1, 0 , 0, 0));
	m_dateEnd = CTime::GetCurrentTime();
	m_bCheckMaxRowCount = TRUE;
	m_nMaxRowCount = 100;

	m_bCheckLogLogin = TRUE;
	m_bCheckLogLogout = TRUE;

	m_EditTotalLogRowCount.SetWindowText("");

	UpdateData(FALSE);
}

void CLogTabConnection::ResetControls()
{
	GetDlgItem(IDC_EDIT_ACCOUNT_NAME)->EnableWindow(m_bCheckAccountName);
	GetDlgItem(IDC_IPADDRESS)->EnableWindow(m_bCheckIPAddress);		// 2008-01-11 by cmkwon, T_A: ConnectionLog 에 IP 검색 기능 추가 - 
	GetDlgItem(IDC_EDIT_CHARACTER_NAME)->EnableWindow(m_bCheckCharacterName);
	GetDlgItem(IDC_DATETIMEPICKER_START_DATE)->EnableWindow(m_bCheckDate);
	GetDlgItem(IDC_DATETIMEPICKER_END_DATE)->EnableWindow(m_bCheckDate);
	GetDlgItem(IDC_EDIT_MAX_ROW_COUNT)->EnableWindow(m_bCheckMaxRowCount);
}
void CLogTabConnection::OnCheckAccountName() 
{
	// TODO: Add your control notification handler code here
	UpdateData();

	GetDlgItem(IDC_EDIT_ACCOUNT_NAME)->EnableWindow(m_bCheckAccountName);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CLogTabConnection::OnCheckIPAddress()
/// \brief		// 2008-01-11 by cmkwon, T_A: ConnectionLog 에 IP 검색 기능 추가 - CLogTabConnection::OnCheckIPAddress() 추가
/// \author		cmkwon
/// \date		2008-01-11 ~ 2008-01-11
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CLogTabConnection::OnCheckIPAddress() 
{
	// TODO: Add your control notification handler code here
	UpdateData();

	// 2008-01-11 by cmkwon, T_A: ConnectionLog 에 IP 검색 기능 추가 - 
	GetDlgItem(IDC_IPADDRESS)->EnableWindow(m_bCheckIPAddress);
}

void CLogTabConnection::OnCheckCharacter() 
{
	// TODO: Add your control notification handler code here
	UpdateData();

	GetDlgItem(IDC_EDIT_CHARACTER_NAME)->EnableWindow(m_bCheckCharacterName);
}

void CLogTabConnection::OnCheckDate() 
{
	// TODO: Add your control notification handler code here
	UpdateData();

	GetDlgItem(IDC_DATETIMEPICKER_START_DATE)->EnableWindow(m_bCheckDate);
	GetDlgItem(IDC_DATETIMEPICKER_END_DATE)->EnableWindow(m_bCheckDate);
}

void CLogTabConnection::OnCheckMaxRowCount() 
{
	// TODO: Add your control notification handler code here
	UpdateData();

	if (!m_bCheckMaxRowCount)
	{
		m_nMaxRowCount = SIZE_MAX_LOG_ROW_COUNT;
		UpdateData(FALSE);
	}
	else
	{
		m_nMaxRowCount = 100;
		UpdateData(FALSE);
	}

	GetDlgItem(IDC_EDIT_MAX_ROW_COUNT)->EnableWindow(m_bCheckMaxRowCount);
}

void CLogTabConnection::OnButtonOk() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	m_EditTotalLogRowCount.SetWindowText("");

	// 그리드 정리하기
	m_GridConnectionLog.InitGrid(NUM_COLUMNS_CONNECTION_LOG_GRID, g_arrConnectionLogColumnNames);

	BOOL bRet = TRUE;
	if (bRet && m_bCheckLogLogin)
	{
		bRet = GetLogAndInsertToGrid(T1_FL_LOG_LOGIN, QUERY_080901_0001);		// 2008-09-01 by cmkwon, StringDefineQuery.h로 추출 - 
		if(bRet)
		{
			bRet = GetLogAndInsertToGrid(T1_FL_LOG_LOGIN, QUERY_080901_0002);	// 2008-09-01 by cmkwon, StringDefineQuery.h로 추출 - 
		}
	}

	if (bRet && m_bCheckLogLogout)
	{
		bRet = GetLogAndInsertToGrid(T1_FL_LOG_LOGOUT, QUERY_080901_0001);		// 2008-09-01 by cmkwon, StringDefineQuery.h로 추출 - 
		if(bRet)
		{
			bRet = GetLogAndInsertToGrid(T1_FL_LOG_LOGOUT, QUERY_080901_0002);	// 2008-09-01 by cmkwon, StringDefineQuery.h로 추출 - 
		}
	}

	m_GridConnectionLog.SortTextItems(COL_CONN_LOG_DATE_TIME, FALSE);

	// 리턴된 로그 수 할당
	char szRowCountBuf[10];
	m_EditTotalLogRowCount.SetWindowText(itoa(m_GridConnectionLog.GetRowCount() - 1, szRowCountBuf, 10));
}

BOOL CLogTabConnection::GetLogAndInsertToGrid(T1<T0::FL_LOG> T1_LogType, LPCSTR szTableName)
{
	// max row count
	CString szQuery;
	if (m_bCheckMaxRowCount)
	{
		// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
		//szQuery.Format("select top %d ", min(m_nMaxRowCount, SIZE_MAX_LOG_ROW_COUNT));
		szQuery.Format(QUERY_080702_0092_1_A, min(m_nMaxRowCount, SIZE_MAX_LOG_ROW_COUNT));	// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
	}
	else
	{
		// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
		//szQuery = "select ";
		szQuery = QUERY_080702_0092_1_B;	// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
	}

// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
// 	szQuery += " l.accountname, l.Time, i.LogStringKO, l.IPAddress, l.ServerName";
// 	szQuery += " from atum_log_type_info i, ";
// 	szQuery += szTableName;
// 	szQuery += " l WITH (NOLOCK) where i.logtype = l.logtype ";

	szQuery += QUERY_080702_0092_2;		// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
	szQuery += QUERY_080702_0092_3;		// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
	szQuery += szTableName;
	szQuery += QUERY_080702_0092_4;		// 2008-07-02 by cmkwon, MySQL 지원 구현 - 

	// log type
	CString szLogTypeCond;
// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
//	szLogTypeCond.Format(" and l.logtype = %d ", T1_LogType);
	szLogTypeCond.Format(QUERY_080702_0092_5, T1_LogType);		// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
	szQuery += szLogTypeCond;

	// account
	if (m_bCheckAccountName)
	{
		CString szAccountCond;
		// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
		//szAccountCond.Format(" and l.accountname = \'%s\' ", m_szAccountName);
		szAccountCond.Format(QUERY_080702_0092_6, m_szAccountName);		// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
		szQuery += szAccountCond;
	}

	// date
	if (m_bCheckDate)
	{
		CString szDateCond;
		// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
		//szDateCond.Format(" and l.time > \'%s\' and l.time < \'%s\' ",
		//				(LPCSTR)m_dateStart.Format("%Y/%m/%d 0:0:0.0"),
		//				(LPCSTR)m_dateEnd.Format("%Y/%m/%d 23:59:59.999"));
		szDateCond.Format(QUERY_080702_0092_7,				// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
						(LPCSTR)m_dateStart.Format("%Y/%m/%d 0:0:0.0"),
						(LPCSTR)m_dateEnd.Format("%Y/%m/%d 23:59:59.999"));
		szQuery += szDateCond;
	}

	///////////////////////////////////////////////////////////////////////////////
	// 2008-01-11 by cmkwon, T_A: ConnectionLog 에 IP 검색 기능 추가 - 
	if(m_bCheckIPAddress)
	{
		BYTE arrbyIPAddr[4] = {0, 0, 0, 0};
		m_ctlIPAddress.GetAddress(arrbyIPAddr[0], arrbyIPAddr[1], arrbyIPAddr[2], arrbyIPAddr[3]);
		CString csFullIPAddr;
		if(0 < arrbyIPAddr[0])
		{
			CString csIPAddr;
			csIPAddr.Format("%d", arrbyIPAddr[0]);
			csFullIPAddr += csIPAddr;
			if(0 < arrbyIPAddr[1])
			{
				csIPAddr.Format(".%d", arrbyIPAddr[1]);
				csFullIPAddr += csIPAddr;
				if(0 < arrbyIPAddr[2])
				{
					csIPAddr.Format(".%d", arrbyIPAddr[2]);
					csFullIPAddr += csIPAddr;
					if(0 < arrbyIPAddr[3])
					{
						csIPAddr.Format(".%d", arrbyIPAddr[3]);
						csFullIPAddr += csIPAddr;
					}
				}
			}
		}
		csFullIPAddr += "%";
		CString csWhereIPAddress;
		// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
		//csWhereIPAddress.Format(" and CAST(CAST(substring(ipaddress, 1, 1) AS INT) AS VARCHAR(3)) + '.' + CAST(CAST(substring(ipaddress, 2, 1) AS INT) AS VARCHAR(3)) + '.' + CAST(CAST(substring(ipaddress, 3, 1) AS INT) AS VARCHAR(3)) + '.' + CAST(CAST(substring(ipaddress, 4, 1) AS INT) AS VARCHAR(3)) like \'%s\'"
		//	, csFullIPAddr);
		csWhereIPAddress.Format(QUERY_080702_0092_8, csFullIPAddr);		// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
		szQuery += csWhereIPAddress;
	}

	// ordering
// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
//	szQuery += " order by l.time desc ";
	szQuery += QUERY_080702_0092_9;			// 2008-07-02 by cmkwon, MySQL 지원 구현 - 

#if defined(DB_SERVER_MYSQL)
	// 2008-07-08 by cmkwon, MySQL 지원 구현 - 
	if(m_bCheckMaxRowCount)
	{
		char szTm[1024];
		sprintf(szTm, QUERY_080702_0092_10_A, min(m_nMaxRowCount, SIZE_MAX_LOG_ROW_COUNT));
		szQuery += szTm;
	}
	else
	{
		szQuery += QUERY_080702_0092_10_B;
	}
#endif

	CONNECTION_LOG_INFO ConnLog;

	SQLINTEGER cb1, cb2, cb3, cb4, cb5;
	char AccountName[SIZE_MAX_ACCOUNT_NAME];
	SQL_TIMESTAMP_STRUCT sqlTime;
	char LogTypeString[30];
	char IPAddress[4];
	char ServerName[SIZE_MAX_SERVER_NAME];

// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
//	if (!m_pODBCStmt->ExecuteQuery(szQuery))
	SQLHSTMT hstmt = m_pODBCStmt->GetSTMTHandle();
	SQLINTEGER arrCB2[3] = {SQL_NTS,SQL_NTS,SQL_NTS};
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_TABLE_NAME, 0, (LPSTR)(LPCSTR)szTableName, 0,		&arrCB2[1]);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SZQUERY, 0, (LPSTR)(LPCSTR)szQuery, 0,			&arrCB2[2]);
	BOOL bRet = m_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_080827_0010));
	if(FALSE == bRet)
	{
		m_pODBCStmt->FreeStatement();
		return FALSE;
	}
	cb1 = SQL_NTS;
	SQLBindCol(m_pODBCStmt->m_hstmt, 1, SQL_C_CHAR, AccountName, SIZE_MAX_ACCOUNT_NAME, &cb1);
	SQLBindCol(m_pODBCStmt->m_hstmt, 2, SQL_C_TIMESTAMP, &sqlTime, 0, &cb2);
	cb3 = SQL_NTS;
	SQLBindCol(m_pODBCStmt->m_hstmt, 3, SQL_C_CHAR, LogTypeString, 30, &cb3);
	SQLBindCol(m_pODBCStmt->m_hstmt, 4, SQL_C_BINARY, IPAddress, 4, &cb4);
	SQLBindCol(m_pODBCStmt->m_hstmt, 5, SQL_C_CHAR, ServerName, SIZE_MAX_SERVER_NAME, &cb5);


	while (SQLFetch(m_pODBCStmt->m_hstmt) != SQL_NO_DATA)
	{
		ConnLog.AccountName = AccountName;
		ConnLog.Time = sqlTime;
		ConnLog.LogType = LogTypeString;
		ConnLog.IPAddress = GetIPAddressString((BYTE*)IPAddress, STRNBUF(SIZE_MAX_IPADDRESS));
		ConnLog.ServerName = ServerName;

		m_GridConnectionLog.InsertGridConnectionLog(&ConnLog);

		int nRowCount = m_GridConnectionLog.GetRowCount() - 1;
		if (nRowCount >= max(100, (m_bCheckMaxRowCount?m_nMaxRowCount:SIZE_MAX_LOG_ROW_COUNT)))
		{
			MessageBox(STRERR_S_SCADMINTOOL_0012);
			m_pODBCStmt->FreeStatement();
			m_GridConnectionLog.ArrangeGrid();
			return FALSE;
		}
	}

	m_GridConnectionLog.ArrangeGrid();
	m_pODBCStmt->FreeStatement();
	return TRUE;
}

void CLogTabConnection::OnButtonReset() 
{
	// TODO: Add your control notification handler code here
	m_GridConnectionLog.InitGrid(NUM_COLUMNS_CONNECTION_LOG_GRID, g_arrConnectionLogColumnNames);
	ResetVariables();
	ResetControls();
}

void CLogTabConnection::OnButtonSelectAll() 
{
	UpdateData(TRUE);

	m_bCheckLogLogin = TRUE;
	m_bCheckLogLogout = TRUE;

	UpdateData(FALSE);
}

void CLogTabConnection::OnButtonDiselectAll() 
{
	UpdateData(TRUE);

	m_bCheckLogLogin = FALSE;
	m_bCheckLogLogout = FALSE;

	UpdateData(FALSE);
}

void CLogTabConnection::OnBtnSaveResult() 
{
	// TODO: Add your control notification handler code here
	int nRowCount = m_GridConnectionLog.GetRowCount();
	if(1 == nRowCount)
	{
		MessageBox("No data !!");
		return;
	}

	CSystemLogManager resultLog;
	if(FALSE == resultLog.InitLogManger(TRUE, "connectLog", "./resultLog/"))
	{
		return;
	}

	for(int i=0; i < nRowCount; i++)
	{
		char szResult[2048];
		util::zero(szResult, 2048);
		sprintf(szResult, "%s;%s;%s;%s;%s\r\n",
			m_GridConnectionLog.GetItemText(i, 0), m_GridConnectionLog.GetItemText(i, 1), m_GridConnectionLog.GetItemText(i, 2),
			m_GridConnectionLog.GetItemText(i, 3), m_GridConnectionLog.GetItemText(i, 4));
		resultLog.WriteSystemLog(szResult, FALSE);
	}
	MessageBox("Save success !!");	
}
