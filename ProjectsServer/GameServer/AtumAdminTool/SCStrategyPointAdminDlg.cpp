// SCStrategyPointAdminDlg.cpp : implementation file
//

#include "stdafx.h"
#include "atumadmintool.h"
#include "SCStrategyPointAdminDlg.h"
#include "AtumAdminToolDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// SCStrategyPointAdminDlg dialog


CSCStrategyPointAdminDlg::CSCStrategyPointAdminDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSCStrategyPointAdminDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(SCStrategyPointAdminDlg)
//	m_MaxSummonCountDaily = 0;
	m_StrategyPointSummonCheck = 0;
	m_SummonRange = 0;
	m_bSummonCheckFriday = FALSE;
	m_bSummonCheckMonday = FALSE;
	m_bSummonCheckSaturday = FALSE;
	m_bSummonCheckSunday = FALSE;
	m_bSummonCheckThursday = FALSE;
	m_bSummonCheckTuesday = FALSE;
	m_bSummonCheckWednesday = FALSE;
	//}}AFX_DATA_INIT
	m_pMainDlg = (CAtumAdminToolDlg*)AfxGetMainWnd();
	m_pODBCStmt = new CODBCStatement;
}


void CSCStrategyPointAdminDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(SCStrategyPointAdminDlg)
//	DDX_Text(pDX, IDC_EDIT_MAX_SUMMON_COUNT_DAILY, m_MaxSummonCountDaily);
//	DDV_MinMaxInt(pDX, m_MaxSummonCountDaily, 0, 1000);
	DDX_Text(pDX, IDC_EDIT_STRATEGYPOINT_SUMMON_CHECK, m_StrategyPointSummonCheck);
	DDV_MinMaxInt(pDX, m_StrategyPointSummonCheck, 0, 1);
	DDX_Text(pDX, IDC_EDIT_SUMMON_RANGE, m_SummonRange);
	DDV_MinMaxInt(pDX, m_SummonRange, 1, 100);
	DDX_DateTimeCtrl(pDX, IDC_DATETIME_EFRIDAY, m_eFriday);
	DDX_DateTimeCtrl(pDX, IDC_DATETIME_EMONDAY, m_eMonday);
	DDX_DateTimeCtrl(pDX, IDC_DATETIME_ESATURDAY, m_eSaturday);
	DDX_DateTimeCtrl(pDX, IDC_DATETIME_ESUNDAY, m_eSunday);
	DDX_DateTimeCtrl(pDX, IDC_DATETIME_ETHURSDAY, m_eThursday);
	DDX_DateTimeCtrl(pDX, IDC_DATETIME_ETUESDAY, m_eTuesday);
	DDX_DateTimeCtrl(pDX, IDC_DATETIME_EWEDNESDAY, m_eWednesday);
	DDX_DateTimeCtrl(pDX, IDC_DATETIME_SFRIDAY, m_sFriday);
	DDX_DateTimeCtrl(pDX, IDC_DATETIME_SMONDAY, m_sMonday);
	DDX_DateTimeCtrl(pDX, IDC_DATETIME_SSATURDAY, m_sSaturday);
	DDX_DateTimeCtrl(pDX, IDC_DATETIME_SSUNDAY, m_sSunday);
	DDX_DateTimeCtrl(pDX, IDC_DATETIME_STHURSDAY, m_sThursday);
	DDX_DateTimeCtrl(pDX, IDC_DATETIME_STUESDAY, m_sTuesday);
	DDX_DateTimeCtrl(pDX, IDC_DATETIME_SWEDNESDAY, m_sWednesday);
	DDX_Check(pDX, IDC_SUMMON_CHECK_FRIDAY, m_bSummonCheckFriday);
	DDX_Check(pDX, IDC_SUMMON_CHECK_MONDAY, m_bSummonCheckMonday);
	DDX_Check(pDX, IDC_SUMMON_CHECK_SATURDAY, m_bSummonCheckSaturday);
	DDX_Check(pDX, IDC_SUMMON_CHECK_SUNDAY, m_bSummonCheckSunday);
	DDX_Check(pDX, IDC_SUMMON_CHECK_THURSDAY, m_bSummonCheckThursday);
	DDX_Check(pDX, IDC_SUMMON_CHECK_TUESDAY, m_bSummonCheckTuesday);
	DDX_Check(pDX, IDC_SUMMON_CHECK_WEDNESDAY, m_bSummonCheckWednesday);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_GRID_STRATEGYPOINT_INFO_LIST, m_GridStrategyPointInfo);
	DDX_Control(pDX, IDC_GRID_STRATEGYPOINT_RANGE_LIST, m_GridStrategyPointRange);
}


BEGIN_MESSAGE_MAP(CSCStrategyPointAdminDlg, CDialog)
	//{{AFX_MSG_MAP(SCStrategyPointAdminDlg)
	ON_BN_CLICKED(IDC_BUTTON_STRATEGYPOINT_SUMMON_RANGE, OnButtonStrategypointSummonRange)
	ON_BN_CLICKED(IDC_BUTTON_UPDATE_STRATEGYPOINT_NOTSUMMONTIME, OnButtonUpdateStrategypointNotsummontime)
	ON_BN_CLICKED(IDC_BUTTON_OK, OnButtonOk)
	ON_BN_CLICKED(IDC_BUTTON_STRATEGYPOINT_SUMMON_RANGE_INIT, OnButtonStrategypointSummonRangeInit)
	ON_BN_CLICKED(IDC_BUTTON_REFRESH, OnButtonRefresh)
	ON_BN_CLICKED(IDC_SUMMON_CHECK_SUNDAY, OnSummonCheckSunday)
	ON_BN_CLICKED(IDC_SUMMON_CHECK_FRIDAY, OnSummonCheckFriday)
	ON_BN_CLICKED(IDC_SUMMON_CHECK_MONDAY, OnSummonCheckMonday)
	ON_BN_CLICKED(IDC_SUMMON_CHECK_SATURDAY, OnSummonCheckSaturday)
	ON_BN_CLICKED(IDC_SUMMON_CHECK_THURSDAY, OnSummonCheckThursday)
	ON_BN_CLICKED(IDC_SUMMON_CHECK_TUESDAY, OnSummonCheckTuesday)
	ON_BN_CLICKED(IDC_SUMMON_CHECK_WEDNESDAY, OnSummonCheckWednesday)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// SCStrategyPointAdminDlg message handlers

BOOL CSCStrategyPointAdminDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (!m_pODBCStmt->Init(m_pMainDlg->m_pServerInfo4Admin->DBIP, m_pMainDlg->m_pServerInfo4Admin->DBPort, m_pMainDlg->m_pServerInfo4Admin->DBName,
							m_pMainDlg->m_pServerInfo4Admin->DBUID, m_pMainDlg->m_pServerInfo4Admin->DBPWD, GetSafeHwnd())
	)
	{
		MessageBox(STRERR_S_SCADMINTOOL_0013);
		EndDialog(-1);
		return FALSE;
	}
	
	this->InitGrid();
	this->DBLoadStrategyPointSummonTimeInfo();
	this->DBLoadStrategyPointSummonRangeInfo();
	this->DBLoadStrategyPointNotSummonTime();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSCStrategyPointAdminDlg::InitGrid()
{
	this->InitGridStrategyPointSummonTimeInfo();
	this->InitGridStrategyPointSummonRange();
}

void CSCStrategyPointAdminDlg::InitGridStrategyPointSummonTimeInfo()
{
	//////////////////////////////////////////////////////////////////////////
	// 2007-03-06 by dhjin, 전략포인트 생성 정보 그리드 
	m_GridStrategyPointInfo.SetBkColor(0xFFFFFF);

	int m_nRows = 1;
	int m_nCols = 4;
	int m_nFixRows = 1;
	
	m_GridStrategyPointInfo.SetEditable(TRUE);
	m_GridStrategyPointInfo.SetListMode(TRUE);
	m_GridStrategyPointInfo.SetSingleRowSelection(TRUE);
	m_GridStrategyPointInfo.EnableSelection(TRUE);
	m_GridStrategyPointInfo.SetHeaderSort(FALSE);
	m_GridStrategyPointInfo.SetFrameFocusCell(FALSE);
	m_GridStrategyPointInfo.SetTrackFocusCell(FALSE);

	m_GridStrategyPointInfo.SetRowCount(m_nRows);
	m_GridStrategyPointInfo.SetColumnCount(m_nCols);
	m_GridStrategyPointInfo.SetFixedRowCount(m_nFixRows);

	// 칼럼 만들기
	m_nCols = 0;
	GV_ITEM Item;
	Item.mask = GVIF_TEXT|GVIF_FORMAT;
	Item.row = 0;
	Item.nFormat = ((DWORD)DT_LEFT|DT_VCENTER|DT_SINGLELINE);

	Item.col = m_nCols++;
	Item.strText.Format("Map");
	m_GridStrategyPointInfo.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("Count");
	m_GridStrategyPointInfo.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("SummonTime");
	m_GridStrategyPointInfo.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("Attribute");
	m_GridStrategyPointInfo.SetItem(&Item);

	m_GridStrategyPointInfo.AutoSize();
	m_GridStrategyPointInfo.ExpandColumnsToFit();

	CCellRange tmpCellRange(1, 0, m_GridStrategyPointInfo.GetRowCount()-1, m_GridStrategyPointInfo.GetColumnCount()-1);
	m_GridStrategyPointInfo.ClearCells(tmpCellRange);
}

void CSCStrategyPointAdminDlg::InitGridStrategyPointSummonRange()
{
	//////////////////////////////////////////////////////////////////////////
	// 2007-03-06 by dhjin, 전략포인트 주기 정보 그리드
	m_GridStrategyPointRange.SetBkColor(0xFFFFFF);

	int m_nRows = 1;
	int m_nCols = 5;
	int m_nFixRows = 1;
	
	m_GridStrategyPointRange.SetEditable(TRUE);
	m_GridStrategyPointRange.SetListMode(TRUE);
	m_GridStrategyPointRange.SetSingleRowSelection(TRUE);
	m_GridStrategyPointRange.EnableSelection(TRUE);
	m_GridStrategyPointRange.SetHeaderSort(FALSE);
	m_GridStrategyPointRange.SetFrameFocusCell(FALSE);
	m_GridStrategyPointRange.SetTrackFocusCell(FALSE);

	m_GridStrategyPointRange.SetRowCount(m_nRows);
	m_GridStrategyPointRange.SetColumnCount(m_nCols);
	m_GridStrategyPointRange.SetFixedRowCount(m_nFixRows);

	// 칼럼 만들기
	m_nCols = 0;
	GV_ITEM Item;
	Item.mask = GVIF_TEXT|GVIF_FORMAT;
	Item.row = 0;
	Item.nFormat = ((DWORD)DT_LEFT|DT_VCENTER|DT_SINGLELINE);

	Item.col = m_nCols++;
	Item.strText.Format("StrategyPoint ON/OFF");
	m_GridStrategyPointRange.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("NowRange");
	m_GridStrategyPointRange.SetItem(&Item);

//	Item.col = m_nCols++;
//	Item.strText.Format("NowMaxDaily");
//	m_GridStrategyPointRange.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("StartTime");
	m_GridStrategyPointRange.SetItem(&Item);
	
	Item.col = m_nCols++;
	Item.strText.Format("EndTime");
	m_GridStrategyPointRange.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("NextRange");
	m_GridStrategyPointRange.SetItem(&Item);

//	Item.col = m_nCols++;
//	Item.strText.Format("MaxDaily");
//	m_GridStrategyPointRange.SetItem(&Item);

	m_GridStrategyPointInfo.AutoSize();
	m_GridStrategyPointInfo.ExpandColumnsToFit();

	CCellRange tmpCellRange(1, 0, m_GridStrategyPointRange.GetRowCount()-1, m_GridStrategyPointRange.GetColumnCount()-1);
	m_GridStrategyPointRange.ClearCells(tmpCellRange);
}

void CSCStrategyPointAdminDlg::InsertGridStrategyPointSummonTimeInfo()
{

	vectSSTRATEGYPOINT_SUMMONTIME_INFO::iterator itr = m_vectSStrategyPointSummonTimeInfo.begin();
	while(itr != m_vectSStrategyPointSummonTimeInfo.end())
	{
		int m_nCols = 0;

		m_GridStrategyPointInfo.SetRowCount(m_GridStrategyPointInfo.GetRowCount()+1);

		GV_ITEM Item;
		Item.mask = GVIF_TEXT|GVIF_FORMAT;
		Item.row = m_GridStrategyPointInfo.GetRowCount()-1;
		Item.nFormat = ((DWORD)DT_LEFT|DT_VCENTER|DT_SINGLELINE);

		Item.col = m_nCols++;
		Item.strText.Format("%d", itr->MapIndex);
		m_GridStrategyPointInfo.SetItem(&Item);

		Item.col = m_nCols++;
		Item.strText.Format("%d", itr->SummonCount);
		m_GridStrategyPointInfo.SetItem(&Item);

		Item.col = m_nCols++;
		Item.strText.Format("%s", itr->SummonTime.GetDateTimeString().GetBuffer());
		m_GridStrategyPointInfo.SetItem(&Item);

		//////////////////////////////////////////////////////////////////////////
		// 2008-04-08 by dhjin, 소환 가능 시간 설정 - 
		Item.col = m_nCols++;
		if(itr->SummonAttribute)
		{
			Item.strText.Format("%s", "Summon");
		}
		else
		{
			Item.strText.Format("%s", "NotSummon");
		}
		m_GridStrategyPointInfo.SetItem(&Item);

		itr++;
	}
	
	m_GridStrategyPointInfo.UpdateData();
	m_GridStrategyPointInfo.AutoSize();
}

void CSCStrategyPointAdminDlg::InsertGridStrategyPointSummonRange()
{
	int m_nCols = 0;

	m_GridStrategyPointRange.SetRowCount(m_GridStrategyPointRange.GetRowCount()+1);

	GV_ITEM Item;
	Item.mask = GVIF_TEXT|GVIF_FORMAT;
	Item.row = m_GridStrategyPointRange.GetRowCount()-1;
	Item.nFormat = ((DWORD)DT_LEFT|DT_VCENTER|DT_SINGLELINE);

	Item.col = m_nCols++;
	Item.strText.Format("%d", m_SStrategyPointSummonRange.StrategyPointSummonCheck);
	m_GridStrategyPointRange.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("%d", m_SStrategyPointSummonRange.SummonRange);
	m_GridStrategyPointRange.SetItem(&Item);

//	Item.col = m_nCols++;
//	Item.strText.Format("%d", m_SStrategyPointSummonRange.MaxSummonCountDaily);
//	m_GridStrategyPointRange.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("%s", m_SStrategyPointSummonRange.StartTime.GetDateTimeString().GetBuffer());
	m_GridStrategyPointRange.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("%s", m_SStrategyPointSummonRange.EndTime.GetDateTimeString().GetBuffer());
	m_GridStrategyPointRange.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("%d", m_SStrategyPointSummonRange.NewSummonRange);
	m_GridStrategyPointRange.SetItem(&Item);

//	Item.col = m_nCols++;
//	Item.strText.Format("%d", m_SStrategyPointSummonRange.NewMaxSummonCountDaily);
//	m_GridStrategyPointRange.SetItem(&Item);

	
	m_GridStrategyPointRange.UpdateData();
	m_GridStrategyPointRange.AutoSize();
}

void CSCStrategyPointAdminDlg::InsertStrategyPointNotSummonTime()
{
	
	vectSSTRATEGYPOINT_NOT_SUMMON_TIME::iterator itr = m_vectSStrategyPointNotSummonTime.begin();
	while(itr != m_vectSStrategyPointNotSummonTime.end())
	{
		if(DAY_OF_WEEK_SUMDAY == itr->DayOfWeek)
		{
			m_sSunday.SetTime(itr->StartTime.Hour, itr->StartTime.Minute, itr->StartTime.Second);
			m_eSunday.SetTime(itr->EndTime.Hour, itr->EndTime.Minute, itr->EndTime.Second);
			m_bSummonCheckSunday = itr->CheckSummon;
		}
		if(DAY_OF_WEEK_MONDAY == itr->DayOfWeek)
		{
			m_sMonday.SetTime(itr->StartTime.Hour, itr->StartTime.Minute, itr->StartTime.Second);
			m_eMonday.SetTime(itr->EndTime.Hour, itr->EndTime.Minute, itr->EndTime.Second);			
			m_bSummonCheckMonday = itr->CheckSummon;
		}		
		if(DAY_OF_WEEK_TUESDAY == itr->DayOfWeek)
		{
			m_sTuesday.SetTime(itr->StartTime.Hour, itr->StartTime.Minute, itr->StartTime.Second);
			m_eTuesday.SetTime(itr->EndTime.Hour, itr->EndTime.Minute, itr->EndTime.Second);			
			m_bSummonCheckTuesday = itr->CheckSummon;
		}
		if(DAY_OF_WEEK_WEDNESDAY == itr->DayOfWeek)
		{
			m_sWednesday.SetTime(itr->StartTime.Hour, itr->StartTime.Minute, itr->StartTime.Second);
			m_eWednesday.SetTime(itr->EndTime.Hour, itr->EndTime.Minute, itr->EndTime.Second);			
			m_bSummonCheckWednesday = itr->CheckSummon;
		}
		if(DAY_OF_WEEK_THURSDAY == itr->DayOfWeek)
		{
			m_sThursday.SetTime(itr->StartTime.Hour, itr->StartTime.Minute, itr->StartTime.Second);
			m_eThursday.SetTime(itr->EndTime.Hour, itr->EndTime.Minute, itr->EndTime.Second);			
			m_bSummonCheckThursday = itr->CheckSummon;
		}
		if(DAY_OF_WEEK_FRIDAY == itr->DayOfWeek)
		{
			m_sFriday.SetTime(itr->StartTime.Hour, itr->StartTime.Minute, itr->StartTime.Second);
			m_eFriday.SetTime(itr->EndTime.Hour, itr->EndTime.Minute, itr->EndTime.Second);			
			m_bSummonCheckFriday = itr->CheckSummon;
		}
		if(DAY_OF_WEEK_SATURDAY == itr->DayOfWeek)
		{
			m_sSaturday.SetTime(itr->StartTime.Hour, itr->StartTime.Minute, itr->StartTime.Second);
			m_eSaturday.SetTime(itr->EndTime.Hour, itr->EndTime.Minute, itr->EndTime.Second);			
			m_bSummonCheckSaturday = itr->CheckSummon;
		}

		itr++;
	}

	UpdateData(FALSE);
}

void CSCStrategyPointAdminDlg::DBLoadStrategyPointSummonTimeInfo()
{	
	/*[Stored Query Definition]************************************************
	--------------------------------------------------------------------------------
	-- TABLE NAME: dbo.atum_LoadAllStrategyPointSummonInfo
	-- DESC      : -- // 2008-04-08 by dhjin, 소환 가능 시간 설정 - 생성해야 할 전략포인트 정보 전체 가져오는 프로시저 (SCAdminTool에서 사용)
	-- 2007-02-26 by dhjin,
	--------------------------------------------------------------------------------
	CREATE PROCEDURE dbo.atum_LoadAllStrategyPointSummonInfo
	AS
		SELECT MapIndex, SummonCount, SummonTime, SummonAttribute FROM dbo.td_StrategyPointSummonInfo WITH (NOLOCK) ORDER BY SummonTime
	GO
	**************************************************************************/
	// 2009-07-09 by cmkwon, 호출 함수 변경(에러 정보 체크를 위해) - 아래와 같이 수정
	//RETCODE	ret = SQLExecDirect(m_pODBCStmt->m_hstmt, PROCEDURE_080822_0018, SQL_NTS);
	RETCODE	ret = m_pODBCStmt->ExecuteQuery(PROCEDURE_080822_0018);
	// 2009-08-13 by cmkwon, AdminTool 거점전 설정 버그 수정 - 리턴값 체크 버그 수정
	if (FALSE == ret)
	{
		MessageBox("DB Execute Error !!(atum_LoadAllStrategyPointSummonInfo)");
		m_pODBCStmt->FreeStatement();
		return;
	}

	SQLINTEGER	arrCB[5] = {SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS};  // 2008-04-08 by dhjin, 소환 가능 시간 설정 - 
	SSTRATEGYPOINT_SUMMONTIME_INFO	tmStrategyPointSummonTimeInfo;
	util::zero(&tmStrategyPointSummonTimeInfo, sizeof(SSTRATEGYPOINT_SUMMONTIME_INFO));
	m_vectSStrategyPointSummonTimeInfo.clear();
	SQL_TIMESTAMP_STRUCT	tmTime;

	SQLBindCol(m_pODBCStmt->m_hstmt, 1, SQL_C_LONG, &tmStrategyPointSummonTimeInfo.MapIndex, 0, &arrCB[1]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 2, SQL_C_UTINYINT, &tmStrategyPointSummonTimeInfo.SummonCount, 0, &arrCB[2]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 3, SQL_C_TIMESTAMP, &tmTime, 0, &arrCB[3]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 4, SQL_C_UTINYINT, &tmStrategyPointSummonTimeInfo.SummonAttribute, 0, &arrCB[4]);	// 2008-04-08 by dhjin, 소환 가능 시간 설정 - 
	
	while ( (ret = SQLFetch(m_pODBCStmt->m_hstmt)) != SQL_NO_DATA)
	{
		tmStrategyPointSummonTimeInfo.SummonTime	=	tmTime;
		m_vectSStrategyPointSummonTimeInfo.push_back(tmStrategyPointSummonTimeInfo);
		util::zero(&tmStrategyPointSummonTimeInfo, sizeof(SSTRATEGYPOINT_SUMMONTIME_INFO));
	}

	m_pODBCStmt->FreeStatement();

	this->InsertGridStrategyPointSummonTimeInfo();
	m_GridStrategyPointInfo.Refresh();
}

void CSCStrategyPointAdminDlg::DBLoadStrategyPointSummonRangeInfo()
{
	/*[Stored Query Definition]************************************************
	--------------------------------------------------------------------------------
	-- TABLE NAME: dbo.atum_LoadStrategyPointSummonRange
	-- DESC      : 전략포인트 생성 주기 가져오는 프로시저
	-- 2007-02-26 by dhjin,
	--------------------------------------------------------------------------------
	CREATE PROCEDURE dbo.atum_LoadStrategyPointSummonRange
	AS
		SELECT SummonRange, StrategyPointSummonCheck, MaxSummonCountDaily
		FROM atum2_db_account.dbo.td_StrategyPointSummonRange
	GO
	**************************************************************************/
	// 2009-07-09 by cmkwon, 호출 함수 변경(에러 정보 체크를 위해) - 아래와 같이 수정
	//RETCODE	ret = SQLExecDirect(m_pODBCStmt->m_hstmt, PROCEDURE_080822_0019, SQL_NTS);
	RETCODE	ret = m_pODBCStmt->ExecuteQuery(PROCEDURE_080822_0019);
	// 2009-08-13 by cmkwon, AdminTool 거점전 설정 버그 수정 - 리턴값 체크 버그 수정
	if (FALSE == ret)
	{
		MessageBox("DB Execute Error !!(atum_LoadStrategyPointSummonRange)");
		m_pODBCStmt->FreeStatement();
		return;
	}

	SQLINTEGER	arrCB[9] = {SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS};
	util::zero(&m_SStrategyPointSummonRange, sizeof(SSTRATEGYPOINT_SUMMON_RANGE));

	SQLBindCol(m_pODBCStmt->m_hstmt, 1, SQL_C_ULONG, &m_SStrategyPointSummonRange.NewSummonRange, 0, &arrCB[1]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 2, SQL_C_TINYINT, &m_SStrategyPointSummonRange.StrategyPointSummonCheck, 0, &arrCB[2]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 3, SQL_C_TINYINT, &m_SStrategyPointSummonRange.NewMaxSummonCountDaily, 0, &arrCB[3]);

	ret = SQLFetch(m_pODBCStmt->m_hstmt);
	m_pODBCStmt->FreeStatement();
	// 2009-08-13 by cmkwon, AdminTool 거점전 설정 버그 수정 - 리턴값 체크 추가
	if(SQL_SUCCESS != ret && SQL_SUCCESS_WITH_INFO != ret)
	{
		MessageBox("DB Fetch Error !!(atum_LoadStrategyPointSummonRange)");
		return;
	}
	
	/*[Stored Query Definition]************************************************
	--------------------------------------------------------------------------------
	-- TABLE NAME: dbo.atum_LoadStrategyPointSummonRangeTimeInfo
	-- DESC      : 생성해야 할 전략포인트 생성 주기 시간 정보를 얻어오는 프로시저.
	-- 2007-03-29 by dhjin,
	--------------------------------------------------------------------------------
	CREATE PROCEDURE dbo.atum_LoadStrategyPointSummonRangeTimeInfo
	AS
		SELECT StartTime, EndTime, SummonRange, StrategyPointSummonCheck, MaxSummonCountDaily FROM dbo.td_StrategyPointSummonRangeTimeInfo WITH (NOLOCK)
	GO
	**************************************************************************/
	// 2009-07-09 by cmkwon, 호출 함수 변경(에러 정보 체크를 위해) - 아래와 같이 수정
	//ret = SQLExecDirect(m_pODBCStmt->m_hstmt, PROCEDURE_080822_0020, SQL_NTS);
	ret = m_pODBCStmt->ExecuteQuery(PROCEDURE_080822_0020);
	// 2009-08-13 by cmkwon, AdminTool 거점전 설정 버그 수정 - 리턴값 체크 버그 수정
	if (FALSE == ret)
	{
		MessageBox("DB Execute Error !!(atum_LoadStrategyPointSummonRangeTimeInfo)");
		m_pODBCStmt->FreeStatement();
		return;
	}

	SQL_TIMESTAMP_STRUCT	tmStartTime;
	SQL_TIMESTAMP_STRUCT	tmEndTime;

	SQLBindCol(m_pODBCStmt->m_hstmt, 1, SQL_C_TIMESTAMP, &tmStartTime, 0, &arrCB[4]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 2, SQL_C_TIMESTAMP, &tmEndTime, 0, &arrCB[5]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 3, SQL_C_ULONG, &m_SStrategyPointSummonRange.SummonRange, 0, &arrCB[6]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 4, SQL_C_TINYINT, &m_SStrategyPointSummonRange.StrategyPointSummonCheck, 0, &arrCB[7]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 5, SQL_C_TINYINT, &m_SStrategyPointSummonRange.MaxSummonCountDaily, 0, &arrCB[8]);
	
	ret = SQLFetch(m_pODBCStmt->m_hstmt);

	if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)// && ret == SQL_NO_DATA)
	{
		m_pODBCStmt->FreeStatement();
		return;
	}

	m_SStrategyPointSummonRange.StartTime	= tmStartTime;
	m_SStrategyPointSummonRange.EndTime		= tmEndTime;	

	m_SummonRange				= m_SStrategyPointSummonRange.SummonRange;
	m_StrategyPointSummonCheck	= m_SStrategyPointSummonRange.StrategyPointSummonCheck;
//	m_MaxSummonCountDaily		= m_SStrategyPointSummonRange.MaxSummonCountDaily;

	m_pODBCStmt->FreeStatement();

	this->InsertGridStrategyPointSummonRange();
	m_GridStrategyPointRange.Refresh();

	UpdateData(FALSE);
}

void CSCStrategyPointAdminDlg::DBUpdateStrategyPointSummonRangeInfo()
{
	/*[Stored Query Definition]************************************************
	--------------------------------------------------------------------------------
	-- TABLE NAME: dbo.atum_UpdateStrategyPointSummonRange
	-- DESC      : 전략포인트 생성 주기 업데이트 프로시저
	-- 2007-02-26 by dhjin,
	--------------------------------------------------------------------------------
	CREATE PROCEDURE dbo.atum_UpdateStrategyPointSummonRange
		@i_NewSummonRange					SMALLINT,
		@i_NewStrategyPointSummonCheck		TINYINT,
		@i_NewMaxSummonCountDaily			TINYINT
	AS
		IF(0=@i_NewStrategyPointSummonCheck)
		BEGIN
			UPDATE atum2_db_account.dbo.td_StrategyPointSummonRange
			SET StrategyPointSummonCheck = @i_NewStrategyPointSummonCheck
		END
		ELSE
		BEGIN
			UPDATE atum2_db_account.dbo.td_StrategyPointSummonRange 
			SET SummonRange = @i_NewSummonRange, StrategyPointSummonCheck = @i_NewStrategyPointSummonCheck, 
				MaxSummonCountDaily = @i_NewMaxSummonCountDaily
		END
	GO
	**************************************************************************/
	UpdateData(TRUE);

	SQLBindParameter(m_pODBCStmt->m_hstmt, 1, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &m_SummonRange, 0, NULL);
	SQLBindParameter(m_pODBCStmt->m_hstmt, 2, SQL_PARAM_INPUT, SQL_C_TINYINT, SQL_TINYINT, 0, 0, &m_StrategyPointSummonCheck, 0, NULL);
//	SQLBindParameter(m_pODBCStmt->m_hstmt, 3, SQL_PARAM_INPUT, SQL_C_TINYINT, SQL_TINYINT, 0, 0, &m_MaxSummonCountDaily, 0, NULL);

	int nTempCount = 0;		// 2009-07-01 by cmkwon, AdminTool(전략 포인트 관련 설정 버그 수정) - 
	SQLBindParameter(m_pODBCStmt->m_hstmt, 3, SQL_PARAM_INPUT, SQL_C_TINYINT, SQL_TINYINT, 0, 0, &nTempCount, 0, NULL);
	
// 2009-07-01 by cmkwon, AdminTool(전략 포인트 관련 설정 버그 수정) - 아래와 같이 수정함, 로그 DbgOut에 출력됨 
//	RETCODE	ret = SQLExecDirect(m_pODBCStmt->m_hstmt, PROCEDURE_080822_0021, SQL_NTS);
//	if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO && ret != SQL_NO_DATA)
	if(FALSE == m_pODBCStmt->ExecuteQuery(PROCEDURE_080822_0021))
	{
		MessageBox("DB Execute Error !!(atum_UpdateStrategyPointSummonRange)");
		m_pODBCStmt->FreeStatement();
		return;
	}

	m_pODBCStmt->FreeStatement();
}

void CSCStrategyPointAdminDlg::OnButtonStrategypointSummonRange() 
{
	// TODO: Add your control notification handler code here

	// 2009-02-04 by cmkwon, AdminTool의 GM 권한 수정 - 전략포인트전 정보 검색은 가능, 수정 불가
	if(FALSE == m_pMainDlg->IsManagerAdministrator())
	{
		AfxMessageBox(ADSTRMSG_090204_0001);
		return;
	}

	int nRet = MessageBox("If you click this button, All setting will be applied to the server after current setting EndTime", NULL, MB_YESNO);
	if(IDNO == nRet)
	{// 2006-05-03 by cmkwon, IMServer와 FieldServer가 종료 상태에만 변경해야 한다.
		UpdateData(FALSE);
		return;
	}
	
	this->DBUpdateStrategyPointSummonRangeInfo();
	this->InitGridStrategyPointSummonRange();
	this->DBLoadStrategyPointSummonRangeInfo();
}

void CSCStrategyPointAdminDlg::DBLoadStrategyPointNotSummonTime()
{
	/*[Stored Query Definition]************************************************
	--------------------------------------------------------------------------------
	-- TABLE NAME: dbo.atum_LoadStrategyPointNotSummonTime
	-- DESC      : 전략포인트 생성되면 안되는 시간 가져오는 프로시저
	-- 2007-02-26 by dhjin,
	--------------------------------------------------------------------------------
	CREATE PROCEDURE dbo.atum_LoadStrategyPointNotSummonTime
	AS
		SELECT DayOfWeek, StartTime, EndTime, CheckSummon FROM atum2_db_account.dbo.td_StrategyPointNotSummonTime
	GO
	**************************************************************************/
	// 2009-07-09 by cmkwon, 호출 함수 변경(에러 정보 체크를 위해) - 아래와 같이 수정
	//RETCODE	ret = SQLExecDirect(m_pODBCStmt->m_hstmt, PROCEDURE_080822_0022, SQL_NTS);
	RETCODE	ret = m_pODBCStmt->ExecuteQuery(PROCEDURE_080822_0022);
	// 2009-08-13 by cmkwon, AdminTool 거점전 설정 버그 수정 - 리턴값 체크 버그 수정
	if (FALSE == ret)
	{
		MessageBox("DB Execute Error !!(atum_LoadStrategyPointNotSummonTime)");
		m_pODBCStmt->FreeStatement();
		return;
	}

	SQLINTEGER	arrCB[5] = {SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS};	// 2008-04-04 by dhjin, 소환 가능 시간 설정 - 
	SSTRATEGYPOINT_NOT_SUMMON_TIME	tmStrategyPointNotSummonTime;
	util::zero(&tmStrategyPointNotSummonTime, sizeof(SSTRATEGYPOINT_NOT_SUMMON_TIME));
	m_vectSStrategyPointNotSummonTime.clear();
	SQL_TIMESTAMP_STRUCT	tmSTime;
	SQL_TIMESTAMP_STRUCT	tmETime;

	SQLBindCol(m_pODBCStmt->m_hstmt, 1, SQL_C_LONG, &tmStrategyPointNotSummonTime.DayOfWeek, 0, &arrCB[1]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 2, SQL_C_TIMESTAMP, &tmSTime, 0, &arrCB[2]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 3, SQL_C_TIMESTAMP, &tmETime, 0, &arrCB[3]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 4, SQL_C_TINYINT, &tmStrategyPointNotSummonTime.CheckSummon, 0, &arrCB[4]);	// 2008-04-04 by dhjin, 소환 가능 시간 설정 - 
	
	while ( (ret = SQLFetch(m_pODBCStmt->m_hstmt)) != SQL_NO_DATA)
	{
		tmStrategyPointNotSummonTime.StartTime	=	tmSTime;
		tmStrategyPointNotSummonTime.EndTime	=	tmETime;

		///////////////////////////////////////////////////////////////////////////////
		// 2008-08-21 by cmkwon, 시간만 사용하지만 기본 날짜를 오늘 날짜로 설정함.
		ATUM_DATE_TIME atCur { true };
		tmStrategyPointNotSummonTime.StartTime.Year		= tmStrategyPointNotSummonTime.EndTime.Year		= atCur.Year;
		tmStrategyPointNotSummonTime.StartTime.Month	= tmStrategyPointNotSummonTime.EndTime.Month	= atCur.Month;
		tmStrategyPointNotSummonTime.StartTime.Day		= tmStrategyPointNotSummonTime.EndTime.Day		= atCur.Day;

		m_vectSStrategyPointNotSummonTime.push_back(tmStrategyPointNotSummonTime);
		util::zero(&tmStrategyPointNotSummonTime, sizeof(SSTRATEGYPOINT_NOT_SUMMON_TIME));
	}

	m_pODBCStmt->FreeStatement();	

	this->InsertStrategyPointNotSummonTime();
}

void CSCStrategyPointAdminDlg::DBUpdateStrategyPointNotSummonTime()
{
	/*[Stored Query Definition]************************************************
	--------------------------------------------------------------------------------
	-- TABLE NAME: dbo.atum_UpdateStrategyPointNotSummonTime
	-- DESC      : 전략포인트 생성되면 안되는 시간 업데이트 프로시저
	-- 2007-02-26 by dhjin,
	--------------------------------------------------------------------------------
	CREATE PROCEDURE dbo.atum_UpdateStrategyPointNotSummonTime
		@i_SSunday				VARCHAR(30),
		@i_ESunday				VARCHAR(30),
		@i_CheckSummonSunday	TINYINT,
		@i_SMonday				VARCHAR(30),
		@i_EMonday				VARCHAR(30),
		@i_CheckSummonMonday	TINYINT,
		@i_STuesday				VARCHAR(30),
		@i_ETuesday				VARCHAR(30),
		@i_CheckSummonTuesday	TINYINT,
		@i_SWednesday			VARCHAR(30),
		@i_EWednesday			VARCHAR(30),
		@i_CheckSummonWednesday	TINYINT,
		@i_SThursday			VARCHAR(30),
		@i_EThursday			VARCHAR(30),
		@i_CheckSummonThursday	TINYINT,
		@i_SFirday				VARCHAR(30),
		@i_EFirday				VARCHAR(30),
		@i_CheckSummonFirday	TINYINT,
		@i_SSaturday			VARCHAR(30),
		@i_ESaturday			VARCHAR(30),
		@i_CheckSummonSaturday	TINYINT
	AS
		UPDATE atum2_db_account.dbo.td_StrategyPointNotSummonTime SET StartTime = @i_SSunday, EndTime = @i_ESunday, CheckSummon = @i_CheckSummonSunday			WHERE DayOfWeek = 0
		UPDATE atum2_db_account.dbo.td_StrategyPointNotSummonTime SET StartTime = @i_SMonday, EndTime = @i_EMonday, CheckSummon = @i_CheckSummonMonday			WHERE DayOfWeek = 1
		UPDATE atum2_db_account.dbo.td_StrategyPointNotSummonTime SET StartTime = @i_STuesday, EndTime = @i_ETuesday, CheckSummon = @i_CheckSummonTuesday		WHERE DayOfWeek = 2
		UPDATE atum2_db_account.dbo.td_StrategyPointNotSummonTime SET StartTime = @i_SWednesday, EndTime = @i_EWednesday, CheckSummon = @i_CheckSummonWednesday	WHERE DayOfWeek = 3
		UPDATE atum2_db_account.dbo.td_StrategyPointNotSummonTime SET StartTime = @i_SThursday, EndTime = @i_EThursday, CheckSummon = @i_CheckSummonThursday	WHERE DayOfWeek = 4
		UPDATE atum2_db_account.dbo.td_StrategyPointNotSummonTime SET StartTime = @i_SFirday, EndTime = @i_EFirday, CheckSummon = @i_CheckSummonFirday			WHERE DayOfWeek = 5
		UPDATE atum2_db_account.dbo.td_StrategyPointNotSummonTime SET StartTime = @i_SSaturday, EndTime = @i_ESaturday, CheckSummon = @i_CheckSummonSaturday	WHERE DayOfWeek = 6
	GO
	**************************************************************************/
	UpdateData(TRUE);
 	char tmSSunday[SIZE_MAX_SQL_DATETIME_STRING];
	sprintf(tmSSunday, "%02d:%02d:%02d.000", m_sSunday.GetHour(), m_sSunday.GetMinute(), m_sSunday.GetSecond());
	char tmESunday[SIZE_MAX_SQL_DATETIME_STRING];
	sprintf(tmESunday, "%02d:%02d:%02d.000", m_eSunday.GetHour(), m_eSunday.GetMinute(), m_eSunday.GetSecond());
	char tmSMonday[SIZE_MAX_SQL_DATETIME_STRING];
	sprintf(tmSMonday, "%02d:%02d:%02d.000", m_sMonday.GetHour(), m_sMonday.GetMinute(), m_sMonday.GetSecond());
	char tmEMonday[SIZE_MAX_SQL_DATETIME_STRING];
	sprintf(tmEMonday, "%02d:%02d:%02d.000", m_eMonday.GetHour(), m_eMonday.GetMinute(), m_eMonday.GetSecond());
	char tmSTuesday[SIZE_MAX_SQL_DATETIME_STRING];
	sprintf(tmSTuesday, "%02d:%02d:%02d.000", m_sTuesday.GetHour(), m_sTuesday.GetMinute(), m_sTuesday.GetSecond());
	char tmETuesday[SIZE_MAX_SQL_DATETIME_STRING];
	sprintf(tmETuesday, "%02d:%02d:%02d.000", m_eTuesday.GetHour(), m_eTuesday.GetMinute(), m_eTuesday.GetSecond());
	char tmSWendesday[SIZE_MAX_SQL_DATETIME_STRING];
	sprintf(tmSWendesday, "%02d:%02d:%02d.000", m_sWednesday.GetHour(), m_sWednesday.GetMinute(), m_sWednesday.GetSecond());
	char tmEWendesday[SIZE_MAX_SQL_DATETIME_STRING];
	sprintf(tmEWendesday, "%02d:%02d:%02d.000", m_eWednesday.GetHour(), m_eWednesday.GetMinute(), m_eWednesday.GetSecond());
	char tmSThursday[SIZE_MAX_SQL_DATETIME_STRING];
	sprintf(tmSThursday, "%02d:%02d:%02d.000", m_sThursday.GetHour(), m_sThursday.GetMinute(), m_sThursday.GetSecond());
	char tmEThursday[SIZE_MAX_SQL_DATETIME_STRING];
	sprintf(tmEThursday, "%02d:%02d:%02d.000", m_eThursday.GetHour(), m_eThursday.GetMinute(), m_eThursday.GetSecond());
	char tmSFriday[SIZE_MAX_SQL_DATETIME_STRING];
	sprintf(tmSFriday, "%02d:%02d:%02d.000", m_sFriday.GetHour(), m_sFriday.GetMinute(), m_sFriday.GetSecond());
	char tmEFriday[SIZE_MAX_SQL_DATETIME_STRING];
	sprintf(tmEFriday, "%02d:%02d:%02d.000", m_eFriday.GetHour(), m_eFriday.GetMinute(), m_eFriday.GetSecond());
	char tmSSaturday[SIZE_MAX_SQL_DATETIME_STRING];
	sprintf(tmSSaturday, "%02d:%02d:%02d.000", m_sSaturday.GetHour(), m_sSaturday.GetMinute(), m_sSaturday.GetSecond());
	char tmESaturday[SIZE_MAX_SQL_DATETIME_STRING];
	sprintf(tmESaturday, "%02d:%02d:%02d.000", m_eSaturday.GetHour(), m_eSaturday.GetMinute(), m_eSaturday.GetSecond());
	
	SQLBindParameter(m_pODBCStmt->m_hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SQL_DATETIME_STRING, 0, tmSSunday, 0, NULL);
	SQLBindParameter(m_pODBCStmt->m_hstmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SQL_DATETIME_STRING, 0, tmESunday, 0, NULL);
	SQLBindParameter(m_pODBCStmt->m_hstmt, 3, SQL_PARAM_INPUT, SQL_C_TINYINT, SQL_TINYINT, 0, 0, &m_bSummonCheckSunday, 0, NULL);
	SQLBindParameter(m_pODBCStmt->m_hstmt, 4, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SQL_DATETIME_STRING, 0, tmSMonday, 0, NULL);
	SQLBindParameter(m_pODBCStmt->m_hstmt, 5, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SQL_DATETIME_STRING, 0, tmEMonday, 0, NULL);
	SQLBindParameter(m_pODBCStmt->m_hstmt, 6, SQL_PARAM_INPUT, SQL_C_TINYINT, SQL_TINYINT, 0, 0, &m_bSummonCheckMonday, 0, NULL);
	SQLBindParameter(m_pODBCStmt->m_hstmt, 7, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SQL_DATETIME_STRING, 0, tmSTuesday, 0, NULL);
	SQLBindParameter(m_pODBCStmt->m_hstmt, 8, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SQL_DATETIME_STRING, 0, tmETuesday, 0, NULL);
	SQLBindParameter(m_pODBCStmt->m_hstmt, 9, SQL_PARAM_INPUT, SQL_C_TINYINT, SQL_TINYINT, 0, 0, &m_bSummonCheckTuesday, 0, NULL);
	SQLBindParameter(m_pODBCStmt->m_hstmt, 10, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SQL_DATETIME_STRING, 0, tmSWendesday, 0, NULL);
	SQLBindParameter(m_pODBCStmt->m_hstmt, 11, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SQL_DATETIME_STRING, 0, tmEWendesday, 0, NULL);
	SQLBindParameter(m_pODBCStmt->m_hstmt, 12, SQL_PARAM_INPUT, SQL_C_TINYINT, SQL_TINYINT, 0, 0, &m_bSummonCheckWednesday, 0, NULL);
	SQLBindParameter(m_pODBCStmt->m_hstmt, 13, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SQL_DATETIME_STRING, 0, tmSThursday, 0, NULL);
	SQLBindParameter(m_pODBCStmt->m_hstmt, 14, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SQL_DATETIME_STRING, 0, tmEThursday, 0, NULL);
	SQLBindParameter(m_pODBCStmt->m_hstmt, 15, SQL_PARAM_INPUT, SQL_C_TINYINT, SQL_TINYINT, 0, 0, &m_bSummonCheckThursday, 0, NULL);
	SQLBindParameter(m_pODBCStmt->m_hstmt, 16, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SQL_DATETIME_STRING, 0, tmSFriday, 0, NULL);
	SQLBindParameter(m_pODBCStmt->m_hstmt, 17, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SQL_DATETIME_STRING, 0, tmEFriday, 0, NULL);
	SQLBindParameter(m_pODBCStmt->m_hstmt, 18, SQL_PARAM_INPUT, SQL_C_TINYINT, SQL_TINYINT, 0, 0, &m_bSummonCheckFriday, 0, NULL);
	SQLBindParameter(m_pODBCStmt->m_hstmt, 19, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SQL_DATETIME_STRING, 0, tmSSaturday, 0, NULL);
	SQLBindParameter(m_pODBCStmt->m_hstmt, 20, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SQL_DATETIME_STRING, 0, tmESaturday, 0, NULL);
	SQLBindParameter(m_pODBCStmt->m_hstmt, 21, SQL_PARAM_INPUT, SQL_C_TINYINT, SQL_TINYINT, 0, 0, &m_bSummonCheckSaturday, 0, NULL);

	
	// 2009-07-09 by cmkwon, 호출 함수 변경(에러 정보 체크를 위해) - 아래와 같이 수정
	//RETCODE	ret = SQLExecDirect(m_pODBCStmt->m_hstmt, PROCEDURE_080822_0023, SQL_NTS);
	RETCODE	ret = m_pODBCStmt->ExecuteQuery(PROCEDURE_080822_0023);
	// 2009-08-13 by cmkwon, AdminTool 거점전 설정 버그 수정 - 리턴값 체크 버그 수정
	if (FALSE == ret)
	{
		MessageBox("DB Execute Error !!(atum_UpdateStrategyPointNotSummonTime)");
		m_pODBCStmt->FreeStatement();
		return;
	}

	m_pODBCStmt->FreeStatement();

	this->DBLoadStrategyPointNotSummonTime();
}

void CSCStrategyPointAdminDlg::DBInitStrategyPointSummonRange()
{
	/*[Stored Query Definition]************************************************
	--------------------------------------------------------------------------------
	-- TABLE NAME: atum_InitStrategyPointSummonRange
	-- DESC      : 전략포인트 설정 주기 및 생성 초기화 프로시저.
	-- 2007-02-26 by dhjin,
	--------------------------------------------------------------------------------
	CREATE PROCEDURE dbo.atum_InitStrategyPointSummonRange
		@i_SummonRange					SMALLINT,
		@i_StrategyPointSummonCheck		TINYINT,
		@i_MaxSummonCountDaily			TINYINT
	-- // 2009-02-17 by cmkwon, MARK for Yedang					-- // 2008-09-04 by cmkwon, 예당 보안관련 숨긴 계정 적용 하기(SQL Server 2005이상만 지원하는 기능) - 
	AS
		-- // 2009-07-09 by cmkwon, 예당 AdminTool 거점 설정 문제 해결 - 아래와 같이 DELETE 쿼리로 수정함.
		--TRUNCATE TABLE	dbo.td_StrategyPointSummonInfo
		--TRUNCATE TABLE	dbo.td_StrategyPointSummonRangeTimeInfo
		--TRUNCATE TABLE	atum2_db_account.dbo.td_StrategyPointSummonRange
		DELETE FROM	dbo.td_StrategyPointSummonInfo
		DELETE FROM dbo.td_StrategyPointSummonRangeTimeInfo
		DELETE FROM atum2_db_account.dbo.td_StrategyPointSummonRange
		
		INSERT INTO dbo.td_StrategyPointSummonInfo 
			SELECT MapIndex, 0, GetDATE(), NULL
			FROM atum2_db_account.dbo.ti_StrategyPointSummonMapIndex
			WHERE MapIndex NOT IN (SELECT MapIndex FROM dbo.td_StrategyPointSummonInfo)
		  
		IF(0=@i_StrategyPointSummonCheck)
		BEGIN
			INSERT INTO atum2_db_account.dbo.td_StrategyPointSummonRange
			VALUES(0, 0, 0)
			
			-- // 2008-01-09 by dhjin, DB 입력되는 날짜 형식 변경
			INSERT INTO dbo.td_StrategyPointSummonRangeTimeInfo
			VALUES('19000000 00:00:00', '19000000 00:00:00', 0, 0, 0)
		END
		ELSE
		BEGIN
			INSERT INTO atum2_db_account.dbo.td_StrategyPointSummonRange
			VALUES(@i_SummonRange, 1, @i_MaxSummonCountDaily)
			  
			UPDATE dbo.td_StrategyPointSummonInfo SET SummonCount = SummonCount + 1
				
			INSERT INTO dbo.td_StrategyPointSummonRangeTimeInfo
			VALUES(GetDATE(), DateAdd(day, @i_SummonRange, GetDATE()), @i_SummonRange, @i_StrategyPointSummonCheck, @i_MaxSummonCountDaily)
		END
	GO
	**************************************************************************/
	UpdateData(TRUE);

	SQLBindParameter(m_pODBCStmt->m_hstmt, 1, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &m_SummonRange, 0, NULL);
	SQLBindParameter(m_pODBCStmt->m_hstmt, 2, SQL_PARAM_INPUT, SQL_C_TINYINT, SQL_TINYINT, 0, 0, &m_StrategyPointSummonCheck, 0, NULL);
//	SQLBindParameter(m_pODBCStmt->m_hstmt, 3, SQL_PARAM_INPUT, SQL_C_TINYINT, SQL_TINYINT, 0, 0, &m_MaxSummonCountDaily, 0, NULL);

	int nTempCount = 0;		// 2009-07-01 by cmkwon, AdminTool(전략 포인트 관련 설정 버그 수정) - 
	SQLBindParameter(m_pODBCStmt->m_hstmt, 3, SQL_PARAM_INPUT, SQL_C_TINYINT, SQL_TINYINT, 0, 0, &nTempCount, 0, NULL);
	
// 2009-07-01 by cmkwon, AdminTool(전략 포인트 관련 설정 버그 수정) - 아래와 같이 수정함, 로그 DbgOut에 출력됨
//	RETCODE	ret = SQLExecDirect(m_pODBCStmt->m_hstmt, PROCEDURE_080822_0024, SQL_NTS);
// 	if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO && ret != SQL_NO_DATA)
	if(FALSE == m_pODBCStmt->ExecuteQuery(PROCEDURE_080822_0024))
	{
		MessageBox("DB Execute Error !!(atum_InitStrategyPointSummonRange)");
		m_pODBCStmt->FreeStatement();
		return;
	}

	m_pODBCStmt->FreeStatement();
	
}

void CSCStrategyPointAdminDlg::OnButtonUpdateStrategypointNotsummontime() 
{
	// TODO: Add your control notification handler code here

	// 2009-02-04 by cmkwon, AdminTool의 GM 권한 수정 - 전략포인트전 정보 검색은 가능, 수정 불가
	if(FALSE == m_pMainDlg->IsManagerAdministrator())
	{
		AfxMessageBox(ADSTRMSG_090204_0001);
		return;
	}

	int nRet = MessageBox("If you click this button, StrategyPoint change SummonTime ", NULL, MB_YESNO);
	if(IDNO == nRet)
	{
		UpdateData(FALSE);
		return;
	}

	UpdateData(TRUE);

	if(FALSE == m_SStrategyPointSummonRange.StrategyPointSummonCheck)
	{// 2008-08-20 by dhjin, 거점 초기와 안되어있을 경우 리턴한다.
		MessageBox("StrategyPointSummonCheck check Plz");
		UpdateData(FALSE);
		return;
	}

	if(m_sSunday		> m_eSunday
		|| m_sMonday	> m_eMonday
		|| m_sTuesday	> m_eTuesday
		|| m_sWednesday > m_eWednesday
		|| m_sThursday	> m_eThursday
		|| m_sFriday	> m_eFriday
		|| m_sSaturday	> m_eSaturday)
	{
		MessageBox("Time Error");
		UpdateData(FALSE);
		return;
	}

	this->DBUpdateStrategyPointNotSummonTime();

	// 2007-11-19 by cmkwon, 진정시스템 업데이트 - 아래와 같이 함수로 처리함

	if (!m_pMainDlg->SendMsgTypeToPreServer(T_PA_ADMIN_UPDATE_STRATEGYPOINT_NOTSUMMONTIME))
	{
		MessageBox("Disconnected from PreServer !!");		
	}
}

void CSCStrategyPointAdminDlg::OnButtonOk() 
{
	// TODO: Add your control notification handler code here
	EndDialog(0);
}

void CSCStrategyPointAdminDlg::OnButtonStrategypointSummonRangeInit() 
{
	// TODO: Add your control notification handler code here

	// 2009-02-04 by cmkwon, AdminTool의 GM 권한 수정 - 전략포인트전 정보 검색은 가능, 수정 불가
	if(FALSE == m_pMainDlg->IsManagerAdministrator())
	{
		AfxMessageBox(ADSTRMSG_090204_0001);
		return;
	}

	int nRet = MessageBox("If you want to initialize, you must restart FieldServer", NULL, MB_YESNO);
	if(IDNO == nRet)
	{// 2006-05-03 by cmkwon, IMServer와 FieldServer가 종료 상태에만 변경해야 한다.
		UpdateData(FALSE);
		return;
	}
	
	this->DBInitStrategyPointSummonRange();
	this->InitGridStrategyPointSummonRange();
	this->DBLoadStrategyPointSummonRangeInfo();	
	this->InitGridStrategyPointSummonTimeInfo();
	this->DBLoadStrategyPointSummonTimeInfo();
}

void CSCStrategyPointAdminDlg::OnButtonRefresh() 
{
	// TODO: Add your control notification handler code here
	this->InitGrid();
	this->DBLoadStrategyPointSummonTimeInfo();
	this->DBLoadStrategyPointSummonRangeInfo();
	this->DBLoadStrategyPointNotSummonTime();
}

void CSCStrategyPointAdminDlg::OnSummonCheckSunday() 
{
	// TODO: Add your control notification handler code here
	if(m_bSummonCheckSunday)
	{
		m_bSummonCheckSunday = FALSE;
	}
	else
	{
		m_bSummonCheckSunday = TRUE;
	}
	
}

void CSCStrategyPointAdminDlg::OnSummonCheckFriday() 
{
	// TODO: Add your control notification handler code here
	if(m_bSummonCheckFriday)
	{
		m_bSummonCheckFriday = FALSE;
	}
	else
	{
		m_bSummonCheckFriday = TRUE;
	}	
}

void CSCStrategyPointAdminDlg::OnSummonCheckMonday() 
{
	// TODO: Add your control notification handler code here
	if(m_bSummonCheckMonday)
	{
		m_bSummonCheckMonday = FALSE;
	}
	else
	{
		m_bSummonCheckMonday = TRUE;
	}	
}

void CSCStrategyPointAdminDlg::OnSummonCheckSaturday() 
{
	// TODO: Add your control notification handler code here
	if(m_bSummonCheckSaturday)
	{
		m_bSummonCheckSaturday = FALSE;
	}
	else
	{
		m_bSummonCheckSaturday = TRUE;
	}	
}

void CSCStrategyPointAdminDlg::OnSummonCheckThursday() 
{
	// TODO: Add your control notification handler code here
	if(m_bSummonCheckThursday)
	{
		m_bSummonCheckThursday = FALSE;
	}
	else
	{
		m_bSummonCheckThursday = TRUE;
	}	
}

void CSCStrategyPointAdminDlg::OnSummonCheckTuesday() 
{
	// TODO: Add your control notification handler code here
	if(m_bSummonCheckTuesday)
	{
		m_bSummonCheckTuesday = FALSE;
	}
	else
	{
		m_bSummonCheckTuesday = TRUE;
	}	
}

void CSCStrategyPointAdminDlg::OnSummonCheckWednesday() 
{
	// TODO: Add your control notification handler code here
	if(m_bSummonCheckWednesday)
	{
		m_bSummonCheckWednesday = FALSE;
	}
	else
	{
		m_bSummonCheckWednesday = TRUE;
	}	
}
