// logtabblockaccount.cpp : implementation file
//

#include "stdafx.h"
#include "atumadmintool.h"
#include "logtabblockaccount.h"
#include "SCLogAdminDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLogTabBlockAccount dialog


CLogTabBlockAccount::CLogTabBlockAccount(CDialog *i_pMainDlg, CWnd* pParent /*=NULL*/)
	: CDialog(CLogTabBlockAccount::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLogTabBlockAccount)
	m_bBlockLog = FALSE;
	m_bUnblockLog = FALSE;
	m_bCheckAdminAccountName = FALSE;
	m_bCheckBlockType = FALSE;
	m_csAdminAccountName = _T("");
	//}}AFX_DATA_INIT

	m_pMainDlg		= (CSCLogAdminDlg*)i_pMainDlg;
	m_pODBCStmt		= &m_pMainDlg->m_ODBCStmt;

	this->ResetVariables();
}


void CLogTabBlockAccount::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLogTabBlockAccount)
	DDX_Control(pDX, IDC_COMBO_BLOCK_TYPE, m_comboBlockTypeList);
	DDX_Control(pDX, IDC_EDIT_TOTAL_LOG_ROW_COUNT, m_EditTotalLogRowCount);
	DDX_Check(pDX, IDC_CHECK_MAX_ROW_COUNT, m_bCheckMaxRowCount);
	DDX_Text(pDX, IDC_EDIT_MAX_ROW_COUNT, m_nMaxRowCount);
	DDX_Check(pDX, IDC_CHECK_ACCOUNT_NAME, m_bCheckAccountName);
	DDX_Text(pDX, IDC_EDIT_ACCOUNT_NAME, m_szAccountName);
	DDX_Check(pDX, IDC_CHECK_DATE, m_bCheckDate);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_END_DATE, m_EndDate);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_END_TIME, m_EndTime);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_START_DATE, m_StartDate);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_START_TIME, m_StartTime);
	DDX_Check(pDX, IDC_CHECK_LOG_BLOCK, m_bBlockLog);
	DDX_Check(pDX, IDC_CHECK_LOG_UNBLOCK, m_bUnblockLog);
	DDX_Check(pDX, IDC_CHECK_ADMIN_ACCOUNT_NAME, m_bCheckAdminAccountName);
	DDX_Check(pDX, IDC_CHECK_BLOCKEDTYPE, m_bCheckBlockType);
	DDX_Text(pDX, IDC_EDIT_ADMIN_ACCOUNT_NAME, m_csAdminAccountName);
	//}}AFX_DATA_MAP
	DDX_GridControl(pDX, IDC_GRID_BLOCKACCOUNT_LOG, m_GridBlockAccountLog);
}


BEGIN_MESSAGE_MAP(CLogTabBlockAccount, CDialog)
	//{{AFX_MSG_MAP(CLogTabBlockAccount)
	ON_BN_CLICKED(IDC_BUTTON_OK, OnButtonOk)
	ON_BN_CLICKED(IDC_BUTTON_SELECT_ALL, OnButtonSelectAll)
	ON_BN_CLICKED(IDC_BUTTON_RESET, OnButtonReset)
	ON_BN_CLICKED(IDC_BUTTON_DISELECT_ALL, OnButtonDiselectAll)
	ON_BN_CLICKED(IDC_BTN_SAVE_RESULT, OnBtnSaveResult)
	ON_BN_CLICKED(IDC_CHECK_ACCOUNT_NAME, OnCheckAccountName)
	ON_BN_CLICKED(IDC_CHECK_ADMIN_ACCOUNT_NAME, OnCheckAdminAccountName)
	ON_BN_CLICKED(IDC_CHECK_BLOCKEDTYPE, OnCheckBlockedtype)
	ON_BN_CLICKED(IDC_CHECK_DATE, OnCheckDate)
	ON_BN_CLICKED(IDC_CHECK_LOG_BLOCK, OnCheckLogBlock)
	ON_BN_CLICKED(IDC_CHECK_MAX_ROW_COUNT, OnCheckMaxRowCount)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLogTabBlockAccount message handlers

BOOL CLogTabBlockAccount::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)
	{
		m_pMainDlg->EndDialog(-1);
		return TRUE;
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			void CLogTabBlockAccount::InitGrid(void)
/// \brief		// 2008-01-30 by cmkwon, 계정 블럭 로그 남기기 구현 - CLogTabBlockAccount::InitGrid() 추가
/// \author		cmkwon
/// \date		2008-01-30 ~ 2008-01-30
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CLogTabBlockAccount::InitGrid(void)
{
	CSCGridCtrl *pSCGridCtrl = &m_GridBlockAccountLog;

	const char *arrColName[] = {"Log Type", "Date", "AccountName"
								, "Block type", "Block Start Date", "Block End Date"
								, "Admin AccountName", "Reason for user", "Reason for Admin"};
	pSCGridCtrl->InitGrid(9, arrColName);

}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CLogTabBlockAccount::ResetVariables(void)
/// \brief		// 2008-01-30 by cmkwon, 계정 블럭 로그 남기기 구현 - CLogTabBlockAccount::ResetVariables() 추가
/// \author		cmkwon
/// \date		2008-01-30 ~ 2008-01-30
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CLogTabBlockAccount::ResetVariables(void)
{
	m_bCheckMaxRowCount		= TRUE;
	m_nMaxRowCount			= 100;
	m_bCheckAccountName		= FALSE;
	m_bCheckDate			= FALSE;
	m_bBlockLog				= TRUE;		// 2013-04-01 by bckim, 캐나다 운영툴 편의성 보완
	m_bUnblockLog			= TRUE;		// 2013-04-01 by bckim, 캐나다 운영툴 편의성 보완
	m_bCheckAdminAccountName	= FALSE;
	m_bCheckBlockType			= FALSE;
	m_csAdminAccountName		= "";
	m_szAccountName				= "";

	
	m_EndDate = COleDateTime::GetCurrentTime();
	m_EndTime = COleDateTime::GetCurrentTime();
	//m_StartDate = COleDateTime::GetCurrentTime()- COleDateTimeSpan(1, 0 , 0, 0);	// 2013-04-01 by bckim, 캐나다 운영툴 편의성 보완
	m_StartDate = COleDateTime::GetCurrentTime();
	m_StartTime = COleDateTime::GetCurrentTime();
	m_StartTime.SetTime(0,0,0);			// 2013-04-01 by bckim, 캐나다 운영툴 편의성 보완
	m_EndTime.SetTime(23,59,59);		// 2013-04-01 by bckim, 캐나다 운영툴 편의성 보완
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CLogTabBlockAccount::ResetControls(void)
/// \brief		// 2008-01-30 by cmkwon, 계정 블럭 로그 남기기 구현 - CLogTabBlockAccount::ResetControls() 추가
/// \author		cmkwon
/// \date		2008-01-30 ~ 2008-01-30
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CLogTabBlockAccount::ResetControls(void)
{
	UpdateData();
	
	GetDlgItem(IDC_EDIT_ACCOUNT_NAME)->EnableWindow(m_bCheckAccountName);
	GetDlgItem(IDC_COMBO_BLOCK_TYPE)->EnableWindow(m_bCheckBlockType);
	GetDlgItem(IDC_EDIT_ADMIN_ACCOUNT_NAME)->EnableWindow(m_bCheckAdminAccountName);
	GetDlgItem(IDC_DATETIMEPICKER_START_DATE)->EnableWindow(m_bCheckDate);
	GetDlgItem(IDC_DATETIMEPICKER_START_TIME)->EnableWindow(m_bCheckDate);
	GetDlgItem(IDC_DATETIMEPICKER_END_DATE)->EnableWindow(m_bCheckDate);
	GetDlgItem(IDC_DATETIMEPICKER_END_TIME)->EnableWindow(m_bCheckDate);
	GetDlgItem(IDC_EDIT_MAX_ROW_COUNT)->EnableWindow(m_bCheckMaxRowCount);

}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2008-01-30 by cmkwon, 계정 블럭 로그 남기기 구현 - CLogTabBlockAccount::GetLogAndInsertToGrid() 추가
/// \author		cmkwon
/// \date		2008-01-30 ~ 2008-01-30
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CLogTabBlockAccount::GetLogAndInsertToGrid(T1<T0::FL_LOG> T1_LogType, LPCSTR szTableName)
{
	CString szQuery;
// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
//	szQuery.Format("select top %d ", min(m_nMaxRowCount, SIZE_MAX_LOG_ROW_COUNT));
	szQuery.Format(QUERY_080702_0050_1, min(m_nMaxRowCount, SIZE_MAX_LOG_ROW_COUNT));	

// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
// 	szQuery += " i.LogStringKO, l.Time, l.AccountName, l.BlockedType, l.StartDate, l.EndDate, l.AdminAccountName, l.BlockedReason, l.BlockedReasonForOnlyAdmin";
// 	szQuery += " from atum_log_type_info i, ";
// 	szQuery += szTableName;
// 	szQuery += " l WITH (NOLOCK) where i.logtype = l.logtype";
	szQuery += QUERY_080702_0050_2;
	szQuery += QUERY_080702_0050_3;
	szQuery += szTableName;
	szQuery += QUERY_080702_0050_4;

	// log type
	CString szLogTypeCond;
// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
//	szLogTypeCond.Format(" and l.logtype = %d ", T1_LogType);
	szLogTypeCond.Format(QUERY_080702_0050_5, T1_LogType);
	szQuery += szLogTypeCond;

	// account
	if (m_bCheckAccountName)
	{
		CString szAccountCond;
		// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
		//szAccountCond.Format(" and l.accountname = \'%s\' ", m_szAccountName);
		szAccountCond.Format(QUERY_080702_0050_6, m_szAccountName);
		szQuery += szAccountCond;
	}

	// Blocked Type
	if (m_bCheckBlockType)
	{
		CString csBlockTyString;
		m_comboBlockTypeList.GetWindowText(csBlockTyString);

		CString szAccountCond;
		// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
		//szAccountCond.Format(" and l.BlockedType = %d", GetBlockTypeBYBlockTypeString((LPSTR)(LPCSTR)csBlockTyString));
		szAccountCond.Format(QUERY_080702_0050_7, GetBlockTypeBYBlockTypeString((LPSTR)(LPCSTR)csBlockTyString));
		szQuery += szAccountCond;
	}

	// Admin AccountName
	if (m_bCheckAdminAccountName)
	{
		CString szAccountCond;
		// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
		//szAccountCond.Format(" and l.AdminAccountName = \'%s\' ", m_csAdminAccountName);
		szAccountCond.Format(QUERY_080702_0050_8, m_csAdminAccountName);
		szQuery += szAccountCond;
	}

	// date
	if (m_bCheckDate)
	{
		ATUM_DATE_TIME atDateStart;
		ATUM_DATE_TIME atDateEnd;
		atDateStart.SetDateTime(m_StartDate.GetYear(), m_StartDate.GetMonth(), m_StartDate.GetDay(), m_StartTime.GetHour(), m_StartTime.GetMinute(), m_StartTime.GetSecond());
		atDateEnd.SetDateTime(m_EndDate.GetYear(), m_EndDate.GetMonth(), m_EndDate.GetDay(), m_EndTime.GetHour(), m_EndTime.GetMinute(), m_EndTime.GetSecond());

		CString csLogDate;
		// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
		//csLogDate.Format(" AND l.Time >= '%s' AND l.Time <= '%s' "
		//	, atDateStart.GetSQLDateTimeString(STRNBUF(SIZE_MAX_SQL_DATETIME_STRING))
		//	, atDateEnd.GetSQLDateTimeString(STRNBUF(SIZE_MAX_SQL_DATETIME_STRING)));
		csLogDate.Format(QUERY_080702_0050_9
			, atDateStart.GetSQLDateTimeString(STRNBUF(SIZE_MAX_SQL_DATETIME_STRING))
			, atDateEnd.GetSQLDateTimeString(STRNBUF(SIZE_MAX_SQL_DATETIME_STRING)));
		szQuery += csLogDate;
	}

	// Block Period Date

	// ordering
	// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
	//szQuery += " order by l.time desc ";
	szQuery += QUERY_080702_0050_10;
#if defined(DB_SERVER_MYSQL)
	// 2008-07-08 by cmkwon, MySQL 지원 구현 - 
	{
		char szTm[1024];
		sprintf(szTm, QUERY_080702_0050_11, min(m_nMaxRowCount, SIZE_MAX_LOG_ROW_COUNT));
		szQuery += szTm;
	}
#endif


// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
//	if (!m_pODBCStmt->ExecuteQuery(szQuery))

	m_pMainDlg->m_pMainDlg->ExchangeAccountDBName(&szQuery);			// 2013-04-30 by bckim. 베트남 운영툴관련 처리	
		 
	SQLHSTMT hstmt = m_pODBCStmt->GetSTMTHandle();
	SQLINTEGER arrCB2[3] = {SQL_NTS,SQL_NTS,SQL_NTS};
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_TABLE_NAME, 0, (LPSTR)(LPCSTR)szTableName, 0,		&arrCB2[1]);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SZQUERY, 0, (LPSTR)(LPCSTR)szQuery, 0,			&arrCB2[2]);	
	BOOL bRet = m_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_080827_0050));
	if(FALSE == bRet)
	{
		m_pODBCStmt->FreeStatement();
		return FALSE;
	}

	SQLINTEGER arrCB[10] = {SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS, SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS};
	char LogTypeString[30];
	SBLOCKED_ACCOUNT_INFO blockedAccInfo;
	SQL_TIMESTAMP_STRUCT sqlLogTime, sqlStartTime, sqlEndTime;

	util::zero(LogTypeString, 30);
	util::zero(&blockedAccInfo, sizeof(SBLOCKED_ACCOUNT_INFO));
	util::zero(&sqlLogTime, sizeof(SQL_TIMESTAMP_STRUCT));
	util::zero(&sqlStartTime, sizeof(SQL_TIMESTAMP_STRUCT));
	util::zero(&sqlEndTime, sizeof(SQL_TIMESTAMP_STRUCT));

	SQLBindCol(m_pODBCStmt->m_hstmt, 1, SQL_C_CHAR, LogTypeString, 30,												&arrCB[1]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 2, SQL_C_TIMESTAMP, &sqlLogTime, 0,											&arrCB[2]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 3, SQL_C_CHAR, blockedAccInfo.szBlockedAccountName, SIZE_MAX_ACCOUNT_NAME,		&arrCB[3]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 4, SQL_C_UTINYINT, &blockedAccInfo.enBlockedType, 0,							&arrCB[4]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 5, SQL_C_TIMESTAMP, &sqlStartTime, 0,											&arrCB[5]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 6, SQL_C_TIMESTAMP, &sqlEndTime, 0,											&arrCB[6]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 7, SQL_C_CHAR, blockedAccInfo.szBlockAdminAccountName, SIZE_MAX_ACCOUNT_NAME,	&arrCB[7]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 8, SQL_C_CHAR, blockedAccInfo.szBlockedReasonForUser, 200,						&arrCB[8]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 9, SQL_C_CHAR, blockedAccInfo.szBlockedReasonForOnlyAdmin, 200,				&arrCB[9]);

	while (SQLFetch(m_pODBCStmt->m_hstmt) != SQL_NO_DATA)
	{
		ATUM_DATE_TIME atTimeLog;
		atTimeLog						= sqlLogTime;
		blockedAccInfo.atimeStartTime	= sqlStartTime;
		blockedAccInfo.atimeEndTime		= sqlEndTime;

		this->InsertGridRow(LogTypeString, &atTimeLog, &blockedAccInfo);

		util::zero(LogTypeString, 30);
		util::zero(&blockedAccInfo, sizeof(SBLOCKED_ACCOUNT_INFO));
		util::zero(&sqlLogTime, sizeof(SQL_TIMESTAMP_STRUCT));
		util::zero(&sqlStartTime, sizeof(SQL_TIMESTAMP_STRUCT));
		util::zero(&sqlEndTime, sizeof(SQL_TIMESTAMP_STRUCT));
	}

//	m_GridBlockAccountLog.ArrangeGrid();  // 2013-04-01 by bckim, 캐나다 운영툴 편의성 보완
	// reason for user 및 reason for admin 사항의 길이가 길때 세부항목을 확인할수 없어, 컬럼을 특정사이즈로 고정후 
	// 다시 autosize() 되는것을 막기위해 주석처리함 

	m_pODBCStmt->FreeStatement();

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CLogTabBlockAccount::InsertGridRow(char *i_szLogString, ATUM_DATE_TIME *i_patLogTime, SBLOCKED_ACCOUNT_INFO *i_pBlockAccInfo)
/// \brief		// 2008-01-30 by cmkwon, 계정 블럭 로그 남기기 구현 - CLogTabBlockAccount::InsertGridRow() 추가
/// \author		cmkwon
/// \date		2008-01-30 ~ 2008-01-30
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CLogTabBlockAccount::InsertGridRow(char *i_szLogString, ATUM_DATE_TIME *i_patLogTime, SBLOCKED_ACCOUNT_INFO *i_pBlockAccInfo)
{
	CSCGridCtrl *i_pSCGridCtrl = &m_GridBlockAccountLog;
	i_pSCGridCtrl->SetRowCount(i_pSCGridCtrl->GetRowCount()+1);

	GV_ITEM Item;
	Item.mask = GVIF_TEXT|GVIF_FORMAT;
	Item.row = i_pSCGridCtrl->GetRowCount()-1;
	Item.nFormat = GRID_CELL_FORMAT;

	int nColIndex = 0;
	Item.col = nColIndex++;
	Item.strText = i_szLogString;
	i_pSCGridCtrl->SetItem(&Item);

	Item.col = nColIndex++;
	Item.strText = i_patLogTime->GetDateTimeString().GetBuffer();
	i_pSCGridCtrl->SetItem(&Item);

	Item.col = nColIndex++;
	Item.strText = i_pBlockAccInfo->szBlockedAccountName;
	i_pSCGridCtrl->SetItem(&Item);

	Item.col = nColIndex++;
	Item.strText.Format("%s", GetStringBLOCKED_TYPE(i_pBlockAccInfo->enBlockedType));
	i_pSCGridCtrl->SetItem(&Item);

	Item.col = nColIndex++;
	Item.strText = i_pBlockAccInfo->atimeStartTime.GetDateTimeString().GetBuffer();
	i_pSCGridCtrl->SetItem(&Item);

	Item.col = nColIndex++;
	Item.strText = i_pBlockAccInfo->atimeEndTime.GetDateTimeString().GetBuffer();
	i_pSCGridCtrl->SetItem(&Item);

	Item.col = nColIndex++;
	Item.strText = i_pBlockAccInfo->szBlockAdminAccountName;
	i_pSCGridCtrl->SetItem(&Item);

	Item.col = nColIndex++;
	Item.strText = i_pBlockAccInfo->szBlockedReasonForUser;
	i_pSCGridCtrl->SetColumnWidth(Item.col,200);		// 2013-04-01 by bckim, 캐나다 운영툴 편의성 보완
	i_pSCGridCtrl->SetItem(&Item);

	Item.col = nColIndex++;
	Item.strText = i_pBlockAccInfo->szBlockedReasonForOnlyAdmin;
	i_pSCGridCtrl->SetColumnWidth(Item.col,200);		// 2013-04-01 by bckim, 캐나다 운영툴 편의성 보완
	i_pSCGridCtrl->SetItem(&Item);
}

BOOL CLogTabBlockAccount::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	this->InitGrid();

	m_comboBlockTypeList.AddString(GetStringBLOCKED_TYPE(T_BLOCKEDACCOUNT_NORMAL));
	m_comboBlockTypeList.AddString(GetStringBLOCKED_TYPE(T_BLOCKEDACCOUNT_MONEY_RELATED));
	m_comboBlockTypeList.AddString(GetStringBLOCKED_TYPE(T_BLOCKEDACCOUNT_ITEM_RELATED));
	m_comboBlockTypeList.AddString(GetStringBLOCKED_TYPE(T_BLOCKEDACCOUNT_SPEEDHACK_RELATED));
	m_comboBlockTypeList.AddString(GetStringBLOCKED_TYPE(T_BLOCKEDACCOUNT_CHAT_RELATED));
	m_comboBlockTypeList.AddString(GetStringBLOCKED_TYPE(T_BLOCKEDACCOUNT_CHAT_GAMEBUG));
	m_comboBlockTypeList.AddString(GetStringBLOCKED_TYPE(T_BLOCKEDACCOUNT_MEMORYHACK_AUTOBLOCK));		// 2012-12-14 by hskim, 메모리핵 자동 블럭 기능 구현
	m_comboBlockTypeList.AddString(GetStringBLOCKED_TYPE(T_BLOCKEDACCOUNT_SPEEDHACK_AUTOBLOCK));		// 2013-01-29 by hskim, 스피드핵 자동 블럭 기능 구현
	m_comboBlockTypeList.SetCurSel(0);

	this->ResetControls();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CLogTabBlockAccount::OnButtonOk() 
{
	// TODO: Add your control notification handler code here
	UpdateData();

	///////////////////////////////////////////////////////////////////////////////
	// 2008-01-30 by cmkwon, 유효성 체크
	if(m_bCheckBlockType)
	{
		CString csBlockTyString;
		m_comboBlockTypeList.GetWindowText(csBlockTyString);

		if(T_BLOCKEDACCOUNT_UNKNOWN == GetBlockTypeBYBlockTypeString((LPSTR)(LPCSTR)csBlockTyString))
		{
			AfxMessageBox("Invalid Block Type !!");
			return;
		}
	}


	m_EditTotalLogRowCount.SetWindowText("");

	// 그리드 정리하기
	this->InitGrid();

	BOOL bRet = TRUE;
	if (bRet && m_bBlockLog)
	{
		bRet = GetLogAndInsertToGrid(T1_FL_LOG_BLOCK_ACCOUNT, QUERY_080901_0003);		// 2008-09-01 by cmkwon, StringDefineQuery.h로 추출 - 
	}

	if (bRet && m_bUnblockLog)
	{
		bRet = GetLogAndInsertToGrid(T1_FL_LOG_UNBLOCK_ACCOUNT, QUERY_080901_0004);		// 2008-09-01 by cmkwon, StringDefineQuery.h로 추출 - 
	}

	m_GridBlockAccountLog.SortTextItems(COL_CONN_LOG_DATE_TIME, FALSE);

	// 리턴된 로그 수 할당
	char szRowCountBuf[10];
	m_EditTotalLogRowCount.SetWindowText(itoa(m_GridBlockAccountLog.GetRowCount() - 1, szRowCountBuf, 10));	
}

void CLogTabBlockAccount::OnButtonSelectAll() 
{
	// TODO: Add your control notification handler code here
	m_bBlockLog		= TRUE;
	m_bUnblockLog	= TRUE;

	UpdateData(FALSE);
}

void CLogTabBlockAccount::OnButtonReset() 
{
	// TODO: Add your control notification handler code here
	
	this->InitGrid();

	this->ResetVariables();
	UpdateData(FALSE);
	
	this->ResetControls();
}

void CLogTabBlockAccount::OnButtonDiselectAll() 
{
	// TODO: Add your control notification handler code here
	m_bBlockLog		= FALSE;
	m_bUnblockLog	= FALSE;

	UpdateData(FALSE);
	
}

void CLogTabBlockAccount::OnBtnSaveResult() 
{
	// TODO: Add your control notification handler code here
	
	int nRowCount = m_GridBlockAccountLog.GetRowCount();
	if(1 == nRowCount)
	{
		MessageBox("No data !!");
		return;
	}

	CSystemLogManager resultLog;
	if(FALSE == resultLog.InitLogManger(TRUE, "BlockAccount", "./resultLog/"))
	{
		return;
	}

	for(int i=0; i < nRowCount; i++)
	{
		char szResult[2048];
		util::zero(szResult, 2048);
		sprintf(szResult, "%s;%s;%s;%s;%s;%s;%s;%s;%s\r\n",
			m_GridBlockAccountLog.GetItemText(i, 0), m_GridBlockAccountLog.GetItemText(i, 1), m_GridBlockAccountLog.GetItemText(i, 2),
			m_GridBlockAccountLog.GetItemText(i, 3), m_GridBlockAccountLog.GetItemText(i, 4), m_GridBlockAccountLog.GetItemText(i, 5),
			m_GridBlockAccountLog.GetItemText(i, 6), m_GridBlockAccountLog.GetItemText(i, 7), m_GridBlockAccountLog.GetItemText(i, 8));
		resultLog.WriteSystemLog(szResult, FALSE);
	}
	MessageBox("Save success !!");	
}

void CLogTabBlockAccount::OnCheckAccountName() 
{
	// TODO: Add your control notification handler code here
	this->ResetControls();
}

void CLogTabBlockAccount::OnCheckAdminAccountName() 
{
	// TODO: Add your control notification handler code here
	this->ResetControls();
}

void CLogTabBlockAccount::OnCheckBlockedtype() 
{
	// TODO: Add your control notification handler code here
	this->ResetControls();
}

void CLogTabBlockAccount::OnCheckDate() 
{
	// TODO: Add your control notification handler code here
	this->ResetControls();
}

void CLogTabBlockAccount::OnCheckLogBlock() 
{
	// TODO: Add your control notification handler code here
	this->ResetControls();
}


void CLogTabBlockAccount::OnCheckMaxRowCount() 
{
	// TODO: Add your control notification handler code here
	this->ResetControls();
	
	if(FALSE == m_bCheckMaxRowCount)
	{
		m_nMaxRowCount = SIZE_MAX_LOG_ROW_COUNT;
	}
	else
	{
		m_nMaxRowCount = 100;		
	}
	UpdateData(FALSE);
}
