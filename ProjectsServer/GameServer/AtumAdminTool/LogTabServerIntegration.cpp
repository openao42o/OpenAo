// LogTabServerIntegration.cpp : implementation file
//

#include "stdafx.h"
#include "atumadmintool.h"
#include "LogTabServerIntegration.h"
#include "SCLogAdminDlg.h"
#include "AtumProtocol.h"
#include "AtumSJ.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLogTabServerIntegration dialog


CLogTabServerIntegration::CLogTabServerIntegration(CDialog *i_pMainDlg, CWnd* pParent /*=NULL*/)
	: CDialog(CLogTabServerIntegration::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLogTabServerIntegration)
	m_szAccountName = _T("");
	m_bCheckAccountName = FALSE;
	m_dateEnd = CTime::GetCurrentTime();
	m_timeEnd = CTime::GetCurrentTime();
	m_dateStart = (CTime::GetCurrentTime() - CTimeSpan(1, 0 , 0, 0));
	m_timeStart = CTime::GetCurrentTime();
	m_bDate = TRUE;
	m_bCheckAccountUID = FALSE;
	m_nAccountUID = 0;
	m_bCheckSourceCharacterName = FALSE;
	m_bCheckSourceCharacterUID = FALSE;
	m_bCheckSourceDBNum = FALSE;
	m_szSourceCharacterName = _T("");
	m_nSourceCharacterUID = 0;
	m_nSourceDBNum = 0;
	m_bCheckTargetCharacterName = FALSE;
	m_bCheckTargetCharacterUID = FALSE;
	m_bCheckTargetDBNum = FALSE;
	m_szTargetCharacterName = _T("");
	m_nTargetCharacterUID = 0;
	m_nTargetDBNum = 0;
	m_nTotalLogCounts = 0;
	//}}AFX_DATA_INIT
	m_pMainDlg = (CSCLogAdminDlg*)i_pMainDlg;
	m_pODBCStmt = &m_pMainDlg->m_ODBCStmt;
}

void CLogTabServerIntegration::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLogTabServerIntegration)
	DDX_Text(pDX, IDC_EDIT_ACCOUNTNAME, m_szAccountName);
	DDX_Check(pDX, IDC_CHECK_AccountName, m_bCheckAccountName);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_END_DATE, m_dateEnd);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_END_TIME, m_timeEnd);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_START_DATE, m_dateStart);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_START_TIME, m_timeStart);
	DDX_Check(pDX, IDC_CHECK_DATE, m_bDate);
	DDX_Check(pDX, IDC_CHECK_AccountUID, m_bCheckAccountUID);
	DDX_Text(pDX, IDC_EDIT_ACCOUNTUID, m_nAccountUID);
	DDX_Check(pDX, IDC_CHECK_SOURCE_CHARACTER_NAME, m_bCheckSourceCharacterName);
	DDX_Check(pDX, IDC_CHECK_SOURCE_CHARACTER_UID, m_bCheckSourceCharacterUID);
	DDX_Check(pDX, IDC_CHECK_SOURCE_DB_NUM, m_bCheckSourceDBNum);
	DDX_Text(pDX, IDC_EDIT_SOURCE_CHARACTER_NAME, m_szSourceCharacterName);
	DDX_Text(pDX, IDC_EDIT_SOURCE_CHARACTER_UID, m_nSourceCharacterUID);
	DDX_Text(pDX, IDC_EDIT_SOURCE_DB_NUM, m_nSourceDBNum);
	DDX_Check(pDX, IDC_CHECK_TARGET_CHARACTER_NAME, m_bCheckTargetCharacterName);
	DDX_Check(pDX, IDC_CHECK_TARGET_CHARACTER_UID, m_bCheckTargetCharacterUID);
	DDX_Check(pDX, IDC_CHECK_TARGET_DB_NUM, m_bCheckTargetDBNum);
	DDX_Text(pDX, IDC_EDIT_TARGET_CHARACTER_NAME, m_szTargetCharacterName);
	DDX_Text(pDX, IDC_EDIT_TARGET_CHARACTER_UID, m_nTargetCharacterUID);
	DDX_Text(pDX, IDC_EDIT_TARGET_DB_NUM, m_nTargetDBNum);
	DDX_Text(pDX, IDC_EDIT_TOTAL_LOG_COUNTS, m_nTotalLogCounts);
	//}}AFX_DATA_MAP
	DDX_GridControl(pDX, IDC_GRID_SERVERINTEGRATION_LOG, m_GridServerIntegrationLog);
}


BEGIN_MESSAGE_MAP(CLogTabServerIntegration, CDialog)
	//{{AFX_MSG_MAP(CLogTabServerIntegration)
	ON_BN_CLICKED(IDC_BUTTON_SEARCH, OnButtonSearch)
	ON_BN_CLICKED(IDC_CHECK_AccountName, OnCHECKAccountName)
	ON_BN_CLICKED(IDC_CHECK_DATE, OnCheckDate)
	ON_BN_CLICKED(IDC_CHECK_AccountUID, OnCHECKAccountUID)
	ON_BN_CLICKED(IDC_CHECK_SOURCE_CHARACTER_NAME, OnCheckSourceCharacterName)
	ON_BN_CLICKED(IDC_CHECK_SOURCE_CHARACTER_UID, OnCheckSourceCharacterUid)
	ON_BN_CLICKED(IDC_CHECK_SOURCE_DB_NUM, OnCheckSourceDbNum)
	ON_BN_CLICKED(IDC_CHECK_TARGET_CHARACTER_NAME, OnCheckTargetCharacterName)
	ON_BN_CLICKED(IDC_CHECK_TARGET_CHARACTER_UID, OnCheckTargetCharacterUid)
	ON_BN_CLICKED(IDC_CHECK_TARGET_DB_NUM, OnCheckTargetDbNum)
	ON_BN_CLICKED(IDC_BUTTON_RESET, OnButtonReset)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, OnButtonSave)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLogTabServerIntegration message handlers

BOOL CLogTabServerIntegration::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)
	{
		m_pMainDlg->EndDialog(-1);
		return TRUE;
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}
 
BOOL CLogTabServerIntegration::OnInitDialog() 
{
	CDialog::OnInitDialog();

	this->InitGrid();
	this->ResetVariables();
	this->ResetControls();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CLogTabServerIntegration::ResetVariables()
{
	m_nAccountUID = 0;
	m_bCheckAccountUID = FALSE;
	m_szAccountName = _T("");
	m_bCheckAccountName = FALSE;
	m_dateEnd = CTime::GetCurrentTime();
	m_timeEnd = CTime::GetCurrentTime();
	m_dateStart = (CTime::GetCurrentTime() - CTimeSpan(1, 0 , 0, 0));
	m_timeStart = CTime::GetCurrentTime();
	m_bDate = TRUE;
	m_bCheckSourceCharacterName = FALSE;
	m_bCheckSourceCharacterUID = FALSE;
	m_bCheckSourceDBNum = FALSE;
	m_szSourceCharacterName = _T("");
	m_nSourceCharacterUID = 0;
	m_nSourceDBNum = 0;
	m_bCheckTargetCharacterName = FALSE;
	m_bCheckTargetCharacterUID = FALSE;
	m_bCheckTargetDBNum = FALSE;
	m_szTargetCharacterName = _T("");
	m_nTargetCharacterUID = 0;
	m_nTargetDBNum = 0;
	m_nTotalLogCounts = 0;

	UpdateData(FALSE);
}

void CLogTabServerIntegration::ResetControls()
{
	GetDlgItem(IDC_EDIT_ACCOUNTUID)->EnableWindow(m_bCheckAccountUID);
	GetDlgItem(IDC_EDIT_ACCOUNTNAME)->EnableWindow(m_bCheckAccountName);
	GetDlgItem(IDC_DATETIMEPICKER_START_DATE)->EnableWindow(m_bDate);
	GetDlgItem(IDC_DATETIMEPICKER_END_DATE)->EnableWindow(m_bDate);
	GetDlgItem(IDC_DATETIMEPICKER_START_TIME)->EnableWindow(m_bDate);
	GetDlgItem(IDC_DATETIMEPICKER_END_TIME)->EnableWindow(m_bDate);	
	GetDlgItem(IDC_EDIT_SOURCE_CHARACTER_NAME)->EnableWindow(m_bCheckSourceCharacterName);
	GetDlgItem(IDC_EDIT_SOURCE_CHARACTER_UID)->EnableWindow(m_bCheckSourceCharacterUID);
	GetDlgItem(IDC_EDIT_SOURCE_DB_NUM)->EnableWindow(m_bCheckSourceDBNum);
	GetDlgItem(IDC_EDIT_TARGET_CHARACTER_NAME)->EnableWindow(m_bCheckTargetCharacterName);	
	GetDlgItem(IDC_EDIT_TARGET_CHARACTER_UID)->EnableWindow(m_bCheckTargetCharacterUID);
	GetDlgItem(IDC_EDIT_TARGET_DB_NUM)->EnableWindow(m_bCheckTargetDBNum);	
}

void CLogTabServerIntegration::InitGrid()
{
	m_GridServerIntegrationLog.SetBkColor(0xFFFFFF);

	int m_nRows = 1;
	int m_nCols = 10;

	m_GridServerIntegrationLog.SetEditable(FALSE);
	m_GridServerIntegrationLog.SetListMode(TRUE);
	m_GridServerIntegrationLog.SetSingleRowSelection(TRUE);
	m_GridServerIntegrationLog.EnableSelection(TRUE);
	m_GridServerIntegrationLog.SetFrameFocusCell(FALSE);
	m_GridServerIntegrationLog.SetTrackFocusCell(FALSE);

	m_GridServerIntegrationLog.SetRowCount(m_nRows);
	m_GridServerIntegrationLog.SetColumnCount(m_nCols);
	m_GridServerIntegrationLog.SetFixedRowCount(1);
// 2008-03-03 by cmkwon, 서버군 통합 로그 타입 보여주기 - 
//	m_GridServerIntegrationLog.SetColumnWidth(0,0);

	// 칼럼 만들기
	m_nCols = 0;
	GV_ITEM Item;
	Item.mask = GVIF_TEXT|GVIF_FORMAT;
	Item.row = 0;
	Item.nFormat = DT_LEFT|DT_VCENTER|DT_SINGLELINE;

// 2008-03-03 by cmkwon, 서버군 통합 로그 타입 보여주기 - 
// 	Item.col = m_nCols++;
// 	Item.strText.Format("Number");
// 	m_GridServerIntegrationLog.SetItem(&Item);
 	Item.col = m_nCols++;
 	Item.strText.Format("Log Type");
 	m_GridServerIntegrationLog.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("AccountUID");
	m_GridServerIntegrationLog.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("AccountName");
	m_GridServerIntegrationLog.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("SourceDBNum");
	m_GridServerIntegrationLog.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("SourceCharacterUID");
	m_GridServerIntegrationLog.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("SourceCharacterName");
	m_GridServerIntegrationLog.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("TargetDBNum");
	m_GridServerIntegrationLog.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("TargetCharacterUID");
	m_GridServerIntegrationLog.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("TargetCharacterName");
	m_GridServerIntegrationLog.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("Time");
	m_GridServerIntegrationLog.SetItem(&Item);


	// arrange grid
	m_GridServerIntegrationLog.AutoSize();
	m_GridServerIntegrationLog.ExpandColumnsToFit();

	// clean all cells
	CCellRange tmpCellRange(1, 0, m_GridServerIntegrationLog.GetRowCount()-1, m_GridServerIntegrationLog.GetColumnCount()-1);
	m_GridServerIntegrationLog.ClearCells(tmpCellRange);	
}

BOOL CLogTabServerIntegration::MakeSzQueryForComplete()
{
	SzQuery.Empty();

	if (TRUE == m_bCheckAccountUID)
	{
		if(FALSE == SzQuery.IsEmpty())
		{
			SzQuery += " and";
		}
		CString	TempSzQuery;
		TempSzQuery.Format(" AccountUID = %d", m_nAccountUID);
		SzQuery += TempSzQuery;
	}

	if (TRUE == m_bCheckAccountName)
	{
		if(FALSE == SzQuery.IsEmpty())
		{
			SzQuery += " and";
		}
		CString	TempSzQuery;
		TempSzQuery.Format(" AccountName = '%s'", m_szAccountName);
		SzQuery += TempSzQuery;
	}

	if (TRUE == m_bCheckSourceCharacterUID)
	{
		if(FALSE == SzQuery.IsEmpty())
		{
			SzQuery += " and";
		}
		CString	TempSzQuery;
		TempSzQuery.Format(" SourceCharacterUID = %d", m_nSourceCharacterUID);
		SzQuery += TempSzQuery;
	}

	if (TRUE == m_bCheckSourceCharacterName)
	{
		if(FALSE == SzQuery.IsEmpty())
		{
			SzQuery += " and";
		}
		CString	TempSzQuery;
		TempSzQuery.Format(" SourceCharacterName = '%s'", m_szSourceCharacterName);
		SzQuery += TempSzQuery;
	}

	if (TRUE == m_bCheckSourceDBNum)
	{
		if(FALSE == SzQuery.IsEmpty())
		{
			SzQuery += " and";
		}
		CString	TempSzQuery;
		TempSzQuery.Format(" SourceDBNum = %d", m_nSourceDBNum);
		SzQuery += TempSzQuery;
	}

	if (TRUE == m_bCheckTargetCharacterUID)
	{
		if(FALSE == SzQuery.IsEmpty())
		{
			SzQuery += " and";
		}
		CString	TempSzQuery;
		TempSzQuery.Format(" TargetCharacterUID = %d", m_nTargetCharacterUID);
		SzQuery += TempSzQuery;
	}

	if (TRUE == m_bCheckTargetCharacterName)
	{
		if(FALSE == SzQuery.IsEmpty())
		{
			SzQuery += " and";
		}
		CString	TempSzQuery;
		TempSzQuery.Format(" TargetCharacterName = '%s'", m_szTargetCharacterName);
		SzQuery += TempSzQuery;
	}

	if (TRUE == m_bCheckTargetDBNum)
	{
		if(FALSE == SzQuery.IsEmpty())
		{
			SzQuery += " and";
		}
		CString	TempSzQuery;
		TempSzQuery.Format(" TargetDBNum = %d", m_nTargetDBNum);
		SzQuery += TempSzQuery;
	}

	if (TRUE == m_bDate)
	{
		if(FALSE == SzQuery.IsEmpty())
		{
			SzQuery += " and";
		}
		CString	TempSzQuery;
		TempSzQuery.Format(" Time >= '%d-%d-%d %d:%d:%d' and Time <= '%d-%d-%d %d:%d:%d'", 
			m_dateStart.GetYear(), m_dateStart.GetMonth(), m_dateStart.GetDay(),
			m_timeStart.GetHour(), m_timeStart.GetMinute(), m_timeStart.GetSecond(),
			m_dateEnd.GetYear(), m_dateEnd.GetMonth(), m_dateEnd.GetDay(),
			m_timeEnd.GetHour(), m_timeEnd.GetMinute(), m_timeEnd.GetSecond());
		SzQuery += TempSzQuery;
	}

	if (TRUE == SzQuery.IsEmpty())
	{
		return FALSE;
	}

	return TRUE;
}

void CLogTabServerIntegration::OnButtonSearch() 
{
	// TODO: Add your control notification handler code here
	this->InitGrid();
	UpdateData(TRUE);
	char szAccName[SIZE_MAX_SZQUERY];
	util::zero(szAccName,SIZE_MAX_SZQUERY);
	/************************************************************************
	--------------------------------------------------------------------------------
	-- TABLE NAME: atum_log_select_integration
	-- DESC      : 2007-01-30 by dhjin, 서버 이전,통합 로그 검색
	--				// 2008-03-03 by cmkwon, 서버군 통합 로그 타입 보여주기 - 검색 결과에 로그타입 추가
	--------------------------------------------------------------------------------
	CREATE  PROCEDURE dbo.atum_log_select_integration
		@i_str				VARCHAR(2000)
	AS
		IF (@i_str = '')
		BEGIN
			-- // 2008-03-03 by cmkwon, 서버군 통합 로그 타입 보여주기 - 검색 결과에 로그타입 추가
			SELECT AccountUID, AccountName, SourceDBNum, SourceCharacterUID, SourceCharacterName, 
				TargetDBNum, TargetCharacterUID, TargetCharacterName, Time, LogType
			FROM atum2_db_account.dbo.atum_log_integration WITH (NOLOCK) 
			order by Time
		END
		ELSE
		BEGIN
			-- // 2008-03-03 by cmkwon, 서버군 통합 로그 타입 보여주기 - 검색 결과에 로그타입 추가
			DECLARE @Query VARCHAR(3000)
			SET @Query = 'SELECT AccountUID, AccountName, SourceDBNum, SourceCharacterUID, 
					SourceCharacterName, TargetDBNum, TargetCharacterUID, TargetCharacterName, Time, LogType	
					FROM atum2_db_account.dbo.atum_log_integration WITH (NOLOCK) where ' 
					+@i_str + 'order by Time'
			exec (@Query)
		END	
	GO
	************************************************************************/
	if(TRUE == this->MakeSzQueryForComplete())
	{
		strcpy(szAccName, SzQuery);
	}
	
	SQLBindParameter(m_pODBCStmt->m_hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SZQUERY, 0, szAccName, 0, NULL);
	// 2009-07-09 by cmkwon, 호출 함수 변경(에러 정보 체크를 위해) - 아래와 같이 수정
	//RETCODE ret = SQLExecDirect(m_pODBCStmt->m_hstmt, PROCEDURE_080822_0016, SQL_NTS);
	RETCODE ret = m_pODBCStmt->ExecuteQuery(PROCEDURE_080822_0016);
	// 2009-08-13 by cmkwon, AdminTool 거점전 설정 버그 수정 - 리턴값 체크 버그 수정
	if (FALSE == ret)
	{
		SQLFreeStmt(m_pODBCStmt->m_hstmt, SQL_CLOSE);
		return;
	}

	SQLINTEGER arrCB[11] = {SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS, SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS
							,SQL_NTS};

	SERVERINTEGRATION_LOG_INFO tmServerIntegrationInfo;	
	util::zero(&tmServerIntegrationInfo, sizeof(tmServerIntegrationInfo));

	SQL_TIMESTAMP_STRUCT tmpTime;

	SQLBindCol(m_pODBCStmt->m_hstmt, 1, SQL_C_LONG, &tmServerIntegrationInfo.AccountUID, 0,									&arrCB[1]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 2, SQL_C_CHAR, tmServerIntegrationInfo.AccountName, SIZE_MAX_ACCOUNT_NAME,				&arrCB[2]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 3, SQL_C_LONG, &tmServerIntegrationInfo.SourceDBNum, 0,								&arrCB[3]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 4, SQL_C_LONG, &tmServerIntegrationInfo.SourceCharacterUID, 0,							&arrCB[4]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 5, SQL_C_CHAR, tmServerIntegrationInfo.SourceCharacterName, SIZE_MAX_CHARACTER_NAME,	&arrCB[5]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 6, SQL_C_LONG, &tmServerIntegrationInfo.TargetDBNum, 0,								&arrCB[6]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 7, SQL_C_LONG, &tmServerIntegrationInfo.TargetCharacterUID, 0,							&arrCB[7]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 8, SQL_C_CHAR, tmServerIntegrationInfo.TargetCharacterName, SIZE_MAX_ACCOUNT_NAME,		&arrCB[8]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 9, SQL_C_TIMESTAMP, &tmpTime, 0,														&arrCB[9]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 10, SQL_C_TINYINT, &tmServerIntegrationInfo.byLogTy, 0,								&arrCB[10]);	// 2008-03-03 by cmkwon, 서버군 통합 로그 타입 보여주기 - 

	m_vectSERVERINTEGRATION_LOG_INFO.clear();
	// DB에 값이 없을때까지 loop를 돈다
	while ( (ret = SQLFetch(m_pODBCStmt->m_hstmt)) != SQL_NO_DATA)
	{
		tmServerIntegrationInfo.Time		= tmpTime;
		m_vectSERVERINTEGRATION_LOG_INFO.push_back(tmServerIntegrationInfo);

		util::zero(&tmServerIntegrationInfo, sizeof(tmServerIntegrationInfo));
	}
	m_pODBCStmt->FreeStatement();	// clean up

	m_nTotalLogCounts = m_vectSERVERINTEGRATION_LOG_INFO.size();

	GV_ITEM Item;
	Item.mask = GVIF_TEXT|GVIF_FORMAT;
	Item.row = 1;
	Item.nFormat = GRID_CELL_FORMAT;

	for(int i=0; i < m_nTotalLogCounts; i++)
	{
		int nNewRowIdx = m_GridServerIntegrationLog.GetRowCount();
		m_GridServerIntegrationLog.SetRowCount(nNewRowIdx+1);

		//select된 값을 GridDetail GridCtrl에 넣어준다.
		Item.row		= nNewRowIdx;
		Item.col		= 0;
		// 2008-03-03 by cmkwon, 서버군 통합 로그 타입 보여주기 - 
		//Item.strText.Format("%d", "Number");
		Item.strText.Format("%s", GetGameLogTypeString(m_vectSERVERINTEGRATION_LOG_INFO[i].byLogTy));
		m_GridServerIntegrationLog.SetItem(&Item);
					
		Item.col		= 1;
		Item.strText.Format("%d", m_vectSERVERINTEGRATION_LOG_INFO[i].AccountUID);
		m_GridServerIntegrationLog.SetItem(&Item);
		
		Item.col		= 2;
		Item.strText.Format("%s", m_vectSERVERINTEGRATION_LOG_INFO[i].AccountName);
		m_GridServerIntegrationLog.SetItem(&Item);	
		
		Item.col		= 3;
		Item.strText.Format("%d", m_vectSERVERINTEGRATION_LOG_INFO[i].SourceDBNum);
		m_GridServerIntegrationLog.SetItem(&Item);
		
		Item.col		= 4;
		Item.strText.Format("%d", m_vectSERVERINTEGRATION_LOG_INFO[i].SourceCharacterUID);
		m_GridServerIntegrationLog.SetItem(&Item);

		Item.col		= 5;
		Item.strText.Format("%s", m_vectSERVERINTEGRATION_LOG_INFO[i].SourceCharacterName);
		m_GridServerIntegrationLog.SetItem(&Item);

		Item.col		= 6;
		Item.strText.Format("%d", m_vectSERVERINTEGRATION_LOG_INFO[i].TargetDBNum);
		m_GridServerIntegrationLog.SetItem(&Item);
		
		Item.col		= 7;
		Item.strText.Format("%d", m_vectSERVERINTEGRATION_LOG_INFO[i].TargetCharacterUID);
		m_GridServerIntegrationLog.SetItem(&Item);
		
		Item.col		= 8;
		Item.strText.Format("%s", m_vectSERVERINTEGRATION_LOG_INFO[i].TargetCharacterName);
		m_GridServerIntegrationLog.SetItem(&Item);

		Item.col		= 9;
		Item.strText.Format("%s", m_vectSERVERINTEGRATION_LOG_INFO[i].Time.GetDateTimeString().GetBuffer());
		m_GridServerIntegrationLog.SetItem(&Item);
	}
	
	m_GridServerIntegrationLog.UpdateData();
	m_GridServerIntegrationLog.AutoSize();

	UpdateData(FALSE);
}

void CLogTabServerIntegration::OnCHECKAccountUID() 
{
	// TODO: Add your control notification handler code here
	UpdateData();

	GetDlgItem(IDC_EDIT_ACCOUNTUID)->EnableWindow(m_bCheckAccountUID);	
}

void CLogTabServerIntegration::OnCHECKAccountName() 
{
	// TODO: Add your control notification handler code here
	UpdateData();

	GetDlgItem(IDC_EDIT_ACCOUNTNAME)->EnableWindow(m_bCheckAccountName);
}

void CLogTabServerIntegration::OnCheckDate() 
{
	// TODO: Add your control notification handler code here
	UpdateData();

	GetDlgItem(IDC_DATETIMEPICKER_START_DATE)->EnableWindow(m_bDate);
	GetDlgItem(IDC_DATETIMEPICKER_END_DATE)->EnableWindow(m_bDate);
	GetDlgItem(IDC_DATETIMEPICKER_START_TIME)->EnableWindow(m_bDate);
	GetDlgItem(IDC_DATETIMEPICKER_END_TIME)->EnableWindow(m_bDate);	
}

void CLogTabServerIntegration::OnCheckSourceCharacterName() 
{
	// TODO: Add your control notification handler code here
	UpdateData();

	GetDlgItem(IDC_EDIT_SOURCE_CHARACTER_NAME)->EnableWindow(m_bCheckSourceCharacterName);
}

void CLogTabServerIntegration::OnCheckSourceCharacterUid() 
{
	// TODO: Add your control notification handler code here
	UpdateData();

	GetDlgItem(IDC_EDIT_SOURCE_CHARACTER_UID)->EnableWindow(m_bCheckSourceCharacterUID);	
}

void CLogTabServerIntegration::OnCheckSourceDbNum() 
{
	// TODO: Add your control notification handler code here
	UpdateData();

	GetDlgItem(IDC_EDIT_SOURCE_DB_NUM)->EnableWindow(m_bCheckSourceDBNum);
}

void CLogTabServerIntegration::OnCheckTargetCharacterName() 
{
	// TODO: Add your control notification handler code here
	UpdateData();

	GetDlgItem(IDC_EDIT_TARGET_CHARACTER_NAME)->EnableWindow(m_bCheckTargetCharacterName);	
}

void CLogTabServerIntegration::OnCheckTargetCharacterUid() 
{
	// TODO: Add your control notification handler code here
	UpdateData();

	GetDlgItem(IDC_EDIT_TARGET_CHARACTER_UID)->EnableWindow(m_bCheckTargetCharacterUID);		
}

void CLogTabServerIntegration::OnCheckTargetDbNum() 
{
	// TODO: Add your control notification handler code here
	UpdateData();

	GetDlgItem(IDC_EDIT_TARGET_DB_NUM)->EnableWindow(m_bCheckTargetDBNum);		
}

void CLogTabServerIntegration::OnButtonReset() 
{
	// TODO: Add your control notification handler code here
	this->ResetVariables();
	this->ResetControls();
}

void CLogTabServerIntegration::OnButtonSave() 
{
	// TODO: Add your control notification handler code here
	int nRowCount = m_GridServerIntegrationLog.GetRowCount();
	if(1 == nRowCount)
	{
		MessageBox("No data !!");
		return;
	}

	CSystemLogManager resultLog;
	if(FALSE == resultLog.InitLogManger(TRUE, "UserLog", "./resultLog/"))
	{
		return;
	}

	for(int i=0; i < nRowCount; i++)
	{
		char szResult[2048];
		util::zero(szResult, 2048);
		sprintf(szResult, "%s;%s;%s;%s;%s;%s\r\n",
			m_GridServerIntegrationLog.GetItemText(i, 0), m_GridServerIntegrationLog.GetItemText(i, 1), m_GridServerIntegrationLog.GetItemText(i, 2),
			m_GridServerIntegrationLog.GetItemText(i, 3), m_GridServerIntegrationLog.GetItemText(i, 4), m_GridServerIntegrationLog.GetItemText(i, 5));
		resultLog.WriteSystemLog(szResult, FALSE);
	}
	MessageBox("Save success !!");
}
