// LogTabGuildItem.cpp : implementation file
//

#include "stdafx.h"
#include "LogTabGuildItem.h"
#include "SCLogAdminDlg.h"
#include "ODBCStatement.h"
#include "AtumProtocol.h"
#include "SelectItemDlg.h"
#include "SpaceCowboyDBEditorToolDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLogTabGuildItem dialog

CLogTabGuildItem::CLogTabGuildItem(CDialog *i_pMainDlg,CWnd* pParent /*=NULL*/)
	: CDialog(CLogTabGuildItem::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLogTabGuildItem)
	m_bCheckCharacterName = FALSE;
	m_bCheckAccountName = FALSE;
	m_bCheckDate = TRUE;
	m_bCheckDrawGuildStore = FALSE;
	m_bCheckGuildUID = FALSE;
	m_bCheckGuildName = FALSE;
	m_bCheckInsertGuildStore = FALSE;
	m_bCheckItemUID = FALSE;
	m_bCheckItemName = FALSE;
	m_bCheckItemNum = FALSE;
	m_bCheckMaxRowCount = FALSE;
	m_szAccountName = _T("");
	m_szCharacterName = _T("");
	m_szGuildName = _T("");
	m_nGuildUID = 0;
	m_nItemNum = 0;
	m_nItemUID = 0;
	m_nMaxRowCount = 100;
	m_nTotalLogRowCount = 0;
	m_szSelectedItemName = _T("");
	m_EndDate = COleDateTime::GetCurrentTime();
	m_EndTime = COleDateTime::GetCurrentTime();
	m_StartDate = COleDateTime::GetCurrentTime();
	m_StartTime = COleDateTime::GetCurrentTime();
	//}}AFX_DATA_INIT
	m_pMainDlg = (CSCLogAdminDlg*)i_pMainDlg;
	// 2007-11-08 by cmkwon, 모든 로그 검색 기능 SCDBTool로 이동 -
	//m_pODBCStmt = &m_pMainDlg->m_ODBCStmt;
	m_pODBCStmt = m_pMainDlg->m_pODBCStmt;		// 2007-11-08 by cmkwon, 모든 로그 검색 기능 SCDBTool로 이동 -

	m_pmapItemInfo		= m_pMainDlg->m_pmapItemInfo;		// 2007-11-08 by cmkwon, 모든 로그 검색 기능 SCDBTool로 이동 -
	m_pmapItemInfoName	= m_pMainDlg->m_pmapItemInfoName;	// 2007-11-08 by cmkwon, 모든 로그 검색 기능 SCDBTool로 이동 -
}


void CLogTabGuildItem::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLogTabGuildItem)
	DDX_Control(pDX, IDC_EDIT_TOTAL_LOG_ROW_COUNT, m_EditTotalLogRowCount);
	DDX_Check(pDX, IDC_CHECK_CHARACTERNAME, m_bCheckCharacterName);
	DDX_Check(pDX, IDC_CHECK_ACCOUNT_NAME, m_bCheckAccountName);
	DDX_Check(pDX, IDC_CHECK_DATE, m_bCheckDate);
	DDX_Check(pDX, IDC_CHECK_DRAW_GUILDSTORE, m_bCheckDrawGuildStore);
	DDX_Check(pDX, IDC_CHECK_GUILD_UID, m_bCheckGuildUID);
	DDX_Check(pDX, IDC_CHECK_GUILDNAME, m_bCheckGuildName);
	DDX_Check(pDX, IDC_CHECK_INSERT_GUILDSTORE, m_bCheckInsertGuildStore);
	DDX_Check(pDX, IDC_CHECK_ITEM_UID, m_bCheckItemUID);
	DDX_Check(pDX, IDC_CHECK_ITEMNAME, m_bCheckItemName);
	DDX_Check(pDX, IDC_CHECK_ITEMNUM, m_bCheckItemNum);
	DDX_Check(pDX, IDC_CHECK_MAX_ROW_COUNT, m_bCheckMaxRowCount);
	DDX_Text(pDX, IDC_EDIT_ACCOUNT_NAME, m_szAccountName);
	DDX_Text(pDX, IDC_EDIT_CHARACTER_NAME, m_szCharacterName);
	DDX_Text(pDX, IDC_EDIT_GUILD_NAME, m_szGuildName);
	DDX_Text(pDX, IDC_EDIT_GUILD_UID, m_nGuildUID);
	DDX_Text(pDX, IDC_EDIT_ITEM_NUM, m_nItemNum);
	DDX_Text(pDX, IDC_EDIT_ITEM_UID, m_nItemUID);
	DDX_Text(pDX, IDC_EDIT_MAX_ROW_COUNT, m_nMaxRowCount);
	DDX_Text(pDX, IDC_EDIT_TOTAL_LOG_ROW_COUNT, m_nTotalLogRowCount);
	DDX_Text(pDX, IDC_EDIT_SELECTED_ITEM, m_szSelectedItemName);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_END_DATE, m_EndDate);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_END_TIME, m_EndTime);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_START_DATE, m_StartDate);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_START_TIME, m_StartTime);
	//}}AFX_DATA_MAP
	DDX_GridControl(pDX, IDC_GRID_ITEM_LOG, m_GridItemLog);
}


BEGIN_MESSAGE_MAP(CLogTabGuildItem, CDialog)
	//{{AFX_MSG_MAP(CLogTabGuildItem)
	ON_BN_CLICKED(IDC_BUTTON_SEARCH, OnButtonSearch)
	ON_BN_CLICKED(IDC_CHECK_INSERT_GUILDSTORE, OnCheckInsertGuildstore)
	ON_BN_CLICKED(IDC_CHECK_DRAW_GUILDSTORE, OnCheckDrawGuildstore)
	ON_BN_CLICKED(IDC_CHECK_ACCOUNT_NAME, OnCheckAccountName)
	ON_BN_CLICKED(IDC_CHECK_CHARACTERNAME, OnCheckCharactername)
	ON_BN_CLICKED(IDC_CHECK_DATE, OnCheckDate)
	ON_BN_CLICKED(IDC_CHECK_GUILD_UID, OnCheckGuildUid)
	ON_BN_CLICKED(IDC_CHECK_GUILDNAME, OnCheckGuildname)
	ON_BN_CLICKED(IDC_CHECK_ITEM_UID, OnCheckItemUid)
	ON_BN_CLICKED(IDC_CHECK_ITEMNAME, OnCheckItemname)
	ON_BN_CLICKED(IDC_CHECK_ITEMNUM, OnCheckItemnum)
	ON_BN_CLICKED(IDC_CHECK_MAX_ROW_COUNT, OnCheckMaxRowCount)
	ON_BN_CLICKED(IDC_BUTTON_SELECT_ALL, OnButtonSelectAll)
	ON_BN_CLICKED(IDC_BUTTON_DISELECT_ALL, OnButtonDiselectAll)
	ON_BN_CLICKED(IDC_BUTTON_RESET, OnButtonReset)
	ON_BN_CLICKED(IDC_BTN_SAVE_RESULT, OnBtnSaveResult)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLogTabGuildItem message handlers

BOOL CLogTabGuildItem::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	ResetControls();

// 2007-11-08 by cmkwon, 모든 로그 검색 기능 SCDBTool로 이동 - 
//	// 데이타 로딩
//	CAtumDBHelper::LoadItemInfo(m_pODBCStmt, &m_mapItemInfo, &(m_pMainDlg->m_pMainDlg->m_Localization), &m_mapItemInfoName);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CLogTabGuildItem::ResetControls()
{
	GetDlgItem(IDC_EDIT_ACCOUNT_NAME)->EnableWindow(m_bCheckAccountName);
	GetDlgItem(IDC_EDIT_CHARACTER_NAME)->EnableWindow(m_bCheckCharacterName);
	GetDlgItem(IDC_EDIT_GUILD_NAME)->EnableWindow(m_bCheckGuildName);
	GetDlgItem(IDC_EDIT_GUILD_UID)->EnableWindow(m_bCheckGuildUID);
	GetDlgItem(IDC_EDIT_ITEM_NUM)->EnableWindow(m_bCheckItemNum);
	GetDlgItem(IDC_EDIT_ITEM_UID)->EnableWindow(m_bCheckItemUID);
	GetDlgItem(IDC_EDIT_MAX_ROW_COUNT)->EnableWindow(m_bCheckMaxRowCount);
//	GetDlgItem(IDC_EDIT_TOTAL_LOG_ROW_COUNT)->EnableWindow(m_EditTotalLogRowCount);
	GetDlgItem(IDC_EDIT_SELECTED_ITEM)->EnableWindow(m_bCheckItemName);

	GetDlgItem(IDC_DATETIMEPICKER_START_DATE)->EnableWindow(m_bCheckDate);
	GetDlgItem(IDC_DATETIMEPICKER_END_DATE)->EnableWindow(m_bCheckDate);
	GetDlgItem(IDC_DATETIMEPICKER_START_TIME)->EnableWindow(m_bCheckDate);
	GetDlgItem(IDC_DATETIMEPICKER_END_TIME)->EnableWindow(m_bCheckDate);
}

void CLogTabGuildItem::ResetValues()
{
	m_bCheckCharacterName = FALSE;
	m_bCheckAccountName = FALSE;
	m_bCheckDate = TRUE;
	m_bCheckGuildUID = FALSE;
	m_bCheckGuildName = FALSE;
	m_bCheckItemUID = FALSE;
	m_bCheckItemName = FALSE;
	m_bCheckItemNum = FALSE;
	m_bCheckMaxRowCount = FALSE;
	m_szAccountName = _T("");
	m_szCharacterName = _T("");
	m_szGuildName = _T("");
	m_nGuildUID = 0;
	m_nItemNum = 0;
	m_nItemUID = 0;
	m_nMaxRowCount = 100;
	m_nTotalLogRowCount = 0;
	m_szSelectedItemName = _T("");
	m_EndDate = COleDateTime::GetCurrentTime();
	m_EndTime = COleDateTime::GetCurrentTime();
	m_StartDate = COleDateTime::GetCurrentTime();
	m_StartTime = COleDateTime::GetCurrentTime();

	m_bCheckInsertGuildStore = FALSE;
	m_bCheckDrawGuildStore = FALSE;
	
	UpdateData(FALSE);
}

void CLogTabGuildItem::OnButtonSearch() 
{
	// TODO: Add your control notification handler code here

	UpdateData();
	m_EditTotalLogRowCount.SetWindowText("");

	// 그리드 정리하기
	InitGrid();

	BOOL bRet = TRUE;
	if (bRet && m_bCheckInsertGuildStore)
	{
		bRet = GetLogAndInsertToGrid(T1_FL_LOG_ITEM_INSERT_TO_GUILDSTORE, "atum_log_GuildItem", NULL);
		if(bRet)
		{
			bRet = GetLogAndInsertToGrid(T1_FL_LOG_ITEM_INSERT_TO_GUILDSTORE, "atum_backup_log_GuildItem", NULL);
		}
	}

	if (bRet && m_bCheckDrawGuildStore)
	{
		bRet = GetLogAndInsertToGrid(T1_FL_LOG_ITEM_DRAW_FROM_GUILDSTORE, "atum_log_GuildItem", NULL);
		if(bRet)
		{
			bRet = GetLogAndInsertToGrid(T1_FL_LOG_ITEM_DRAW_FROM_GUILDSTORE, "atum_backup_log_GuildItem", NULL);
		}
	}
	m_GridItemLog.SortTextItems(1, FALSE);

	// 리턴된 로그 수 할당
	char szRowCountBuf[10];
	m_EditTotalLogRowCount.SetWindowText(itoa(m_GridItemLog.GetRowCount() - 1, szRowCountBuf, 10));
}

BOOL CLogTabGuildItem::GetLogAndInsertToGrid(T1<T0::FL_LOG> T1_LogType, LPCSTR szTableName, LPCSTR szContentString)
{
	// max row count
	CString szQuery;
	if (m_bCheckMaxRowCount)
	{
		// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
		//szQuery.Format("select top %d ", min(m_nMaxRowCount, SIZE_MAX_LOG_ROW_COUNT));
		szQuery.Format(QUERY_080702_0093_1_A, min(m_nMaxRowCount, SIZE_MAX_LOG_ROW_COUNT));		// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
	}
	else
	{
		// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
		//szQuery = "select ";
		szQuery = QUERY_080702_0093_1_B;		// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
	}

// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
// 	szQuery += " l.* ";
// 	szQuery += szContentString;
// 	szQuery += " from atum_log_type_info i, ";
// 	szQuery += szTableName;
// 	szQuery += " l WITH (NOLOCK) where i.logtype = l.logtype ";

	szQuery += QUERY_080702_0093_2;		// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
	szQuery += szContentString;
	szQuery += QUERY_080702_0093_3;		// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
	szQuery += szTableName;
	szQuery += QUERY_080702_0093_4;		// 2008-07-02 by cmkwon, MySQL 지원 구현 - 

	// log type
	CString szLogTypeCond;
// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
//	szLogTypeCond.Format(" and l.logtype = %d ", T1_LogType);
	szLogTypeCond.Format(QUERY_080702_0093_5, T1_LogType);		// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
	szQuery += szLogTypeCond;
	
	if (m_bCheckAccountName)
	{// account
		CString temp;
		// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
		//temp.Format(" and l.AccountName = \'%s\' ", m_szAccountName);
		temp.Format(QUERY_080702_0093_6, m_szAccountName);		// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
		szQuery += temp;
	}

	if (m_bCheckCharacterName)
	{// character
		CString temp;
		// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
		//temp.Format(" and l.CharacterName = \'%s\' ", m_szCharacterName);
		temp.Format(QUERY_080702_0093_7, m_szCharacterName);	// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
		szQuery += temp;
	}

	if (m_bCheckDate)
	{// date
		CString temp;
		// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
		//temp.Format(" and l.time > \'%s\' and l.time < \'%s\' ",
		//				(LPCSTR)(m_StartDate.Format("%Y/%m/%d") + m_StartTime.Format(" %H:%M:%S.0")),
		//				(LPCSTR)(m_EndDate.Format("%Y/%m/%d") + m_EndTime.Format(" %H:%M:%S.0")));
		temp.Format(QUERY_080702_0093_8,		// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
						(LPCSTR)(m_StartDate.Format("%Y/%m/%d") + m_StartTime.Format(" %H:%M:%S.0")),
						(LPCSTR)(m_EndDate.Format("%Y/%m/%d") + m_EndTime.Format(" %H:%M:%S.0")));
		szQuery += temp;
	}

	if (m_bCheckGuildName)
	{// GuildName
		CString temp;
		// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
		//temp.Format(" and l.GuildName = \'%s\' ", m_szGuildName);
		temp.Format(QUERY_080702_0093_9, m_szGuildName);		// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
		szQuery += temp;
	}

	if (m_bCheckGuildUID)
	{// GuildUID
		CString temp;
		// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
		//temp.Format(" and l.GuildUID = %d ", m_nGuildUID);
		temp.Format(QUERY_080702_0093_10, m_nGuildUID);			// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
		szQuery += temp;
	}

	if (m_bCheckItemNum)
	{// ItemNum
		CString temp;
		// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
		//temp.Format(" and l.ItemNum = %d ", m_nItemNum);
		temp.Format(QUERY_080702_0093_11, m_nItemNum);			// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
		szQuery += temp;
	}

	if (m_bCheckItemUID)
	{// ItemUID
		CString temp;
		// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
		//temp.Format(" and l.ItemUID = %d ", m_nItemUID);
		temp.Format(QUERY_080702_0093_12, m_nItemUID);			// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
		szQuery += temp;
	}

	// ordering
	// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
	//szQuery += " order by time desc ";
	szQuery += QUERY_080702_0093_13;		// 2008-07-02 by cmkwon, MySQL 지원 구현 - 

#if defined(DB_SERVER_MYSQL)
	// 2008-07-08 by cmkwon, MySQL 지원 구현 - 
	if(m_bCheckMaxRowCount)
	{
		char szTm[1024];
		sprintf(szTm, QUERY_080702_0093_14_A, min(m_nMaxRowCount, SIZE_MAX_LOG_ROW_COUNT));
		szQuery += szTm;
	}
	else
	{
		szQuery += QUERY_080702_0093_14_B;
	}
#endif

	// clear vector
	memset(&m_LOG_GUILDSTORE_ITEM_INFO, 0x00, sizeof(LOG_GUILDSTORE_ITEM_INFO));

	RETCODE	ret;
	LOG_GUILDSTORE_ITEM_INFO GuilditemInfo;

	SQLINTEGER		arrCB[16] = {SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS,
								SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS};
	SQL_TIMESTAMP_STRUCT tmpTimeStamp;
	
// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
//	BOOL bRet = m_pODBCStmt->ExecuteQuery(szQuery);
	SQLHSTMT hstmt = m_pODBCStmt->GetSTMTHandle();
	SQLINTEGER arrCB2[3] = {SQL_NTS,SQL_NTS,SQL_NTS};
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_TABLE_NAME, 0, (LPSTR)(LPCSTR)szTableName, 0,		&arrCB2[1]);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SZQUERY, 0, (LPSTR)(LPCSTR)szQuery, 0,			&arrCB2[2]);	
	BOOL bRet = m_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_080827_0010));
	if (!bRet)
	{
		// error
		m_pODBCStmt->FreeStatement();
		return 0;
	}

	// Bind Columns
	SQLBindCol(m_pODBCStmt->m_hstmt,  1, SQL_C_TINYINT	, &GuilditemInfo.LogType		, 0						, &arrCB[1]);
	SQLBindCol(m_pODBCStmt->m_hstmt,  2, SQL_C_TIMESTAMP, &tmpTimeStamp					, 0						, &arrCB[2]);
	SQLBindCol(m_pODBCStmt->m_hstmt,  3, SQL_C_ULONG	, &GuilditemInfo.GuildUID		, 0						, &arrCB[3]);
	SQLBindCol(m_pODBCStmt->m_hstmt,  4, SQL_C_CHAR		, GuilditemInfo.GuildName		, SIZE_MAX_GUILD_NAME	, &arrCB[4]);
	SQLBindCol(m_pODBCStmt->m_hstmt,  5, SQL_C_ULONG	, &GuilditemInfo.AccountUID		, 0						, &arrCB[5]);
	SQLBindCol(m_pODBCStmt->m_hstmt,  6, SQL_C_CHAR		, GuilditemInfo.AccountName	, SIZE_MAX_ACCOUNT_NAME	, &arrCB[6]);
	SQLBindCol(m_pODBCStmt->m_hstmt,  7, SQL_C_ULONG	, &GuilditemInfo.CharacterUID	, 0						, &arrCB[7]);
	SQLBindCol(m_pODBCStmt->m_hstmt,  8, SQL_C_CHAR		, GuilditemInfo.CharacterName	,SIZE_MAX_CHARACTER_NAME, &arrCB[8]);
	SQLBindCol(m_pODBCStmt->m_hstmt,  9, SQL_C_SBIGINT	, &GuilditemInfo.ItemUID		, 0						, &arrCB[9]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 10, SQL_C_ULONG	, &GuilditemInfo.ItemNum        , 0						, &arrCB[10]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 11, SQL_C_CHAR		, GuilditemInfo.ItemName		, SIZE_MAX_ITEM_NAME	, &arrCB[11]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 12, SQL_C_ULONG	, &GuilditemInfo.PrefixCodeNum	, 0						, &arrCB[12]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 13, SQL_C_ULONG	, &GuilditemInfo.SuffixCodeNum	, 0						, &arrCB[13]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 14, SQL_C_ULONG	, &GuilditemInfo.ItemResultCount, 0						, &arrCB[14]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 15, SQL_C_LONG		, &GuilditemInfo.ItemChangeCount, 0						, &arrCB[15]);
	
	int nObjects = 0;
	memset(&GuilditemInfo, 0x00, sizeof(LOG_GUILDSTORE_ITEM_INFO));
	while ( (ret = SQLFetch(m_pODBCStmt->m_hstmt)) != SQL_NO_DATA)
	{
		// 2007-11-08 by cmkwon, 모든 로그 검색 기능 SCDBTool로 이동 -
		//ITEM *pItemInfo = m_mapItemInfo.findEZ_ptr(GuilditemInfo.ItemNum);
		ITEM *pItemInfo = m_pmapItemInfo->findEZ_ptr(GuilditemInfo.ItemNum);
		if(pItemInfo)
		{// 2006-03-27 by cmkwon, 아이템 이름 현지화 처리
			util::strncpy(m_LOG_GUILDSTORE_ITEM_INFO.ItemName, pItemInfo->ItemName, SIZE_MAX_ITEM_NAME);
		}
		
		m_LOG_GUILDSTORE_ITEM_INFO.LogType			= GuilditemInfo.LogType;
		m_LOG_GUILDSTORE_ITEM_INFO.Time				= tmpTimeStamp;
		m_LOG_GUILDSTORE_ITEM_INFO.GuildUID			= GuilditemInfo.GuildUID;
		util::strncpy(m_LOG_GUILDSTORE_ITEM_INFO.GuildName, GuilditemInfo.GuildName, SIZE_MAX_GUILD_NAME);
		m_LOG_GUILDSTORE_ITEM_INFO.AccountUID		= GuilditemInfo.AccountUID;
		util::strncpy(m_LOG_GUILDSTORE_ITEM_INFO.AccountName, GuilditemInfo.AccountName, SIZE_MAX_ACCOUNT_NAME);
		m_LOG_GUILDSTORE_ITEM_INFO.CharacterUID		= GuilditemInfo.CharacterUID;
		util::strncpy(m_LOG_GUILDSTORE_ITEM_INFO.CharacterName, GuilditemInfo.CharacterName, SIZE_MAX_CHARACTER_NAME);
		m_LOG_GUILDSTORE_ITEM_INFO.ItemUID			= GuilditemInfo.ItemUID;
		m_LOG_GUILDSTORE_ITEM_INFO.ItemNum			= GuilditemInfo.ItemNum;
		m_LOG_GUILDSTORE_ITEM_INFO.PrefixCodeNum	= GuilditemInfo.PrefixCodeNum;
		m_LOG_GUILDSTORE_ITEM_INFO.SuffixCodeNum	= GuilditemInfo.SuffixCodeNum;
		m_LOG_GUILDSTORE_ITEM_INFO.ItemResultCount	= GuilditemInfo.ItemResultCount;
		m_LOG_GUILDSTORE_ITEM_INFO.ItemChangeCount	= GuilditemInfo.ItemChangeCount;
		
		InsertGridItemLog();

		int nRowCount = m_GridItemLog.GetRowCount() - 1;
		if (nRowCount >= max(100, (m_bCheckMaxRowCount?m_nMaxRowCount:SIZE_MAX_LOG_ROW_COUNT)))
		{
			MessageBox(STRERR_S_SCADMINTOOL_0012);
			m_GridItemLog.ArrangeGrid();
			m_pODBCStmt->FreeStatement();
			return FALSE;
		}

		nObjects++;

		memset(&GuilditemInfo, 0x00, sizeof(LOG_GUILDSTORE_ITEM_INFO));
	}

	m_GridItemLog.ArrangeGrid();
	m_pODBCStmt->FreeStatement();
	return TRUE;
}

void CLogTabGuildItem::InitGrid()
{
	m_GridItemLog.SetBkColor(0xFFFFFF);

	int m_nRows = 1;
	int m_nCols = 10;

	m_GridItemLog.SetEditable(FALSE);
	m_GridItemLog.SetListMode(TRUE);
	m_GridItemLog.SetSingleRowSelection(TRUE);
	m_GridItemLog.EnableSelection(TRUE);
	m_GridItemLog.SetFrameFocusCell(FALSE);
	m_GridItemLog.SetTrackFocusCell(FALSE);

	m_GridItemLog.SetRowCount(m_nRows);
	m_GridItemLog.SetColumnCount(m_nCols);
	m_GridItemLog.SetFixedRowCount(1);
	m_GridItemLog.SetColumnWidth(0,0);

	// 칼럼 만들기
	m_nCols = 0;
	GV_ITEM Item;
	Item.mask = GVIF_TEXT|GVIF_FORMAT;
	Item.row = 0;
	Item.nFormat = DT_LEFT|DT_VCENTER|DT_SINGLELINE;

	Item.col = m_nCols++;
	Item.strText.Format("Unique Number");
	m_GridItemLog.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("DateTime");
	m_GridItemLog.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("GuildName");
	m_GridItemLog.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("AccountName");
	m_GridItemLog.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("CharacterName");
	m_GridItemLog.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("ItemUID");
	m_GridItemLog.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("ItemInfo");
	m_GridItemLog.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("LogType");
	m_GridItemLog.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("ChangeCount");
	m_GridItemLog.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("ResultCount");
	m_GridItemLog.SetItem(&Item);

	// arrange grid
	m_GridItemLog.AutoSize();
	m_GridItemLog.ExpandColumnsToFit();

	// clean all cells
	CCellRange tmpCellRange(1, 0, m_GridItemLog.GetRowCount()-1, m_GridItemLog.GetColumnCount()-1);
	m_GridItemLog.ClearCells(tmpCellRange);
}

void CLogTabGuildItem::InsertGridItemLog()
{
	m_GridItemLog.SetRowCount(m_GridItemLog.GetRowCount()+1);

	GV_ITEM Item;
	Item.mask = GVIF_TEXT|GVIF_FORMAT;
	Item.row = m_GridItemLog.GetRowCount()-1;
	Item.nFormat = GRID_CELL_FORMAT;

	Item.col		 = 1;
	Item.strText.Format("%s", m_LOG_GUILDSTORE_ITEM_INFO.Time.GetDateTimeString().GetBuffer());
	m_GridItemLog.SetItem(&Item);

	Item.col		 = 2;
	Item.strText.Format("%s", m_LOG_GUILDSTORE_ITEM_INFO.GuildName);
	m_GridItemLog.SetItem(&Item);

	Item.col		 = 3;
	Item.strText.Format("%s", m_LOG_GUILDSTORE_ITEM_INFO.AccountName);
	m_GridItemLog.SetItem(&Item);

	Item.col		 = 4;
	Item.strText.Format("%s", m_LOG_GUILDSTORE_ITEM_INFO.CharacterName);
	m_GridItemLog.SetItem(&Item);
	
	Item.col		 = 5;
	Item.strText.Format("%d", m_LOG_GUILDSTORE_ITEM_INFO.ItemUID);
	m_GridItemLog.SetItem(&Item);

	Item.col		 = 6;
	// 2007-11-08 by cmkwon, 모든 로그 검색 기능 SCDBTool로 이동 -
	//if(m_mapItemInfo.findEZ_ptr(m_LOG_GUILDSTORE_ITEM_INFO.ItemNum))
	if(m_pmapItemInfo->findEZ_ptr(m_LOG_GUILDSTORE_ITEM_INFO.ItemNum))
	{
		// 2007-11-08 by cmkwon, 모든 로그 검색 기능 SCDBTool로 이동 -
		//Item.strText.Format("[%d]  %s", m_LOG_GUILDSTORE_ITEM_INFO.ItemNum, m_mapItemInfo.findEZ_ptr(m_LOG_GUILDSTORE_ITEM_INFO.ItemNum)->ItemName);
		Item.strText.Format("[%d]  %s", m_LOG_GUILDSTORE_ITEM_INFO.ItemNum, m_pmapItemInfo->findEZ_ptr(m_LOG_GUILDSTORE_ITEM_INFO.ItemNum)->ItemName);
	}
	else
	{
		Item.strText.Format("%d", m_LOG_GUILDSTORE_ITEM_INFO.ItemNum);
	}
	m_GridItemLog.SetItem(&Item);

	CString temp;
	if (T1_FL_LOG_ITEM_INSERT_TO_GUILDSTORE == m_LOG_GUILDSTORE_ITEM_INFO.LogType)
	{
		 temp = "Insert";
	}
	else
	{
		 temp = "Draw";
	}
	Item.col		 = 7;
	Item.strText.Format("%s", temp);
	m_GridItemLog.SetItem(&Item);

	Item.col		 = 8;
	Item.strText.Format("%d", m_LOG_GUILDSTORE_ITEM_INFO.ItemChangeCount);
	m_GridItemLog.SetItem(&Item);

	Item.col		 = 9;
	Item.strText.Format("%d", m_LOG_GUILDSTORE_ITEM_INFO.ItemResultCount);
	m_GridItemLog.SetItem(&Item);

//	ArrangeGrid();
}

void CLogTabGuildItem::OnCheckInsertGuildstore() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	
}

void CLogTabGuildItem::OnCheckDrawGuildstore() 
{
	// TODO: Add your control notification handler code here
	UpdateData();

}

void CLogTabGuildItem::OnCheckAccountName() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	GetDlgItem(IDC_EDIT_ACCOUNT_NAME)->EnableWindow(m_bCheckAccountName);

}

void CLogTabGuildItem::OnCheckCharactername() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	GetDlgItem(IDC_EDIT_CHARACTER_NAME)->EnableWindow(m_bCheckCharacterName);

}

void CLogTabGuildItem::OnCheckDate() 
{
	// TODO: Add your control notification handler code here
	UpdateData();

	GetDlgItem(IDC_DATETIMEPICKER_START_DATE)->EnableWindow(m_bCheckDate);
	GetDlgItem(IDC_DATETIMEPICKER_END_DATE)->EnableWindow(m_bCheckDate);
	GetDlgItem(IDC_DATETIMEPICKER_START_TIME)->EnableWindow(m_bCheckDate);
	GetDlgItem(IDC_DATETIMEPICKER_END_TIME)->EnableWindow(m_bCheckDate);
}

void CLogTabGuildItem::OnCheckGuildUid() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	GetDlgItem(IDC_EDIT_GUILD_UID)->EnableWindow(m_bCheckGuildUID);	
}

void CLogTabGuildItem::OnCheckGuildname() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	GetDlgItem(IDC_EDIT_GUILD_NAME)->EnableWindow(m_bCheckGuildName);	
}

void CLogTabGuildItem::OnCheckItemUid() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	GetDlgItem(IDC_EDIT_ITEM_UID)->EnableWindow(m_bCheckItemUID);	
}

void CLogTabGuildItem::OnCheckItemname() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	GetDlgItem(IDC_EDIT_SELECTED_ITEM)->EnableWindow(m_bCheckItemName);	
	m_bCheckItemNum = TRUE;
	GetDlgItem(IDC_EDIT_ITEM_NUM)->EnableWindow(m_bCheckItemNum);	

	if (m_bCheckItemName)
	{
		// 2007-11-08 by cmkwon, 모든 로그 검색 기능 SCDBTool로 이동 -
		//CSelectItemDlg dlg(&m_mapItemInfo, &m_mapItemInfoName, FALSE);
		CSelectItemDlg dlg(m_pmapItemInfo, m_pmapItemInfoName, FALSE);
		if (IDOK != dlg.DoModal())
		{
			return;
		}

		m_nItemNum = dlg.m_nSelectedItemNum;
		m_szSelectedItemName = dlg.m_szSelectedItemName;

		UpdateData(FALSE);
	}
}

void CLogTabGuildItem::OnCheckItemnum() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	GetDlgItem(IDC_EDIT_ITEM_NUM)->EnableWindow(m_bCheckItemNum);	
}

void CLogTabGuildItem::OnCheckMaxRowCount() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	GetDlgItem(IDC_EDIT_MAX_ROW_COUNT)->EnableWindow(m_bCheckMaxRowCount);	
}

void CLogTabGuildItem::OnButtonSelectAll() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_bCheckInsertGuildStore	= TRUE;
	m_bCheckDrawGuildStore		= TRUE;
	UpdateData(FALSE);
}

void CLogTabGuildItem::OnButtonDiselectAll() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_bCheckInsertGuildStore	= FALSE;
	m_bCheckDrawGuildStore		= FALSE;
	UpdateData(FALSE);	
}

void CLogTabGuildItem::OnButtonReset() 
{
	// TODO: Add your control notification handler code here
	InitGrid();
	ResetValues();
	ResetControls();

}

void CLogTabGuildItem::OnBtnSaveResult() 
{
	// TODO: Add your control notification handler code here
	int nRowCount = m_GridItemLog.GetRowCount();
	if(1 == nRowCount)
	{
		MessageBox("No data !!");
		return;
	}

	CSystemLogManager resultLog;
	if(FALSE == resultLog.InitLogManger(TRUE, "GuilditemLog", "./resultLog/"))
	{
		return;
	}

	for(int i=0; i < nRowCount; i++)
	{
		char szResult[2048];
		util::zero(szResult, 2048);
		sprintf(szResult, "%s;%s;%s;%s;%s;%s;%s\r\n",
			m_GridItemLog.GetItemText(i, 0), m_GridItemLog.GetItemText(i, 1), m_GridItemLog.GetItemText(i, 2),
			m_GridItemLog.GetItemText(i, 3), m_GridItemLog.GetItemText(i, 4), m_GridItemLog.GetItemText(i, 5),
			m_GridItemLog.GetItemText(i, 6), m_GridItemLog.GetItemText(i, 7), m_GridItemLog.GetItemText(i, 8),
			m_GridItemLog.GetItemText(i, 9));
		resultLog.WriteSystemLog(szResult, FALSE);
	}
	MessageBox("Save success !!");	
}
