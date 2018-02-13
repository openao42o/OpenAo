// SCHappyHourEventAdminDlg.cpp : implementation file
//

#include "stdafx.h"
#include "atumadmintool.h"
#include "SCHappyHourEventAdminDlg.h"
#include "AtumAdminToolDlg.h"
#include "sethappyev.h"
#include "setpcbanghappyev.h"			// 2006-08-28 by cmkwon
#include "AtumSJ.h"						// 2007-10-30 by cmkwon, 세력별 해피아워 이벤트 구현 -

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSCHappyHourEventAdminDlg dialog


CSCHappyHourEventAdminDlg::CSCHappyHourEventAdminDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSCHappyHourEventAdminDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSCHappyHourEventAdminDlg)
	m_valueDTPTotalSetStartDate = COleDateTime::GetCurrentTime();
	m_valueDTPTotalSetEndDate = COleDateTime::GetCurrentTime();
	m_valueDTPTotalSetStartTime = COleDateTime::GetCurrentTime();
	m_valueDTPTotalSetEndTime = COleDateTime::GetCurrentTime();
	m_OleDatePCBangStart = COleDateTime::GetCurrentTime();
	m_OleTimePCBangStart = COleDateTime::GetCurrentTime();
	m_OleDatePCBangEnd = COleDateTime::GetCurrentTime();
	m_OleTimePCBangEnd = COleDateTime::GetCurrentTime();
	m_ctlOleDateNormalStart = COleDateTime::GetCurrentTime();
	m_ctlOleDateNormalEnd = COleDateTime::GetCurrentTime();
	m_ctlOleDateVCNStart = COleDateTime::GetCurrentTime();
	m_ctlOleDateVCNEnd = COleDateTime::GetCurrentTime();
	m_ctlOleDateANIStart = COleDateTime::GetCurrentTime();
	m_ctlOleDateANIEnd = COleDateTime::GetCurrentTime();
	m_ctlOleTimeNormalStart = COleDateTime::GetCurrentTime();
	m_ctlOleTimeNormalEnd = COleDateTime::GetCurrentTime();
	m_ctlOleTimeVCNStart = COleDateTime::GetCurrentTime();
	m_ctlOleTimeVCNEnd = COleDateTime::GetCurrentTime();
	m_ctlOleTimeANIStart = COleDateTime::GetCurrentTime();
	m_ctlOleTimeANIEnd = COleDateTime::GetCurrentTime();
	//}}AFX_DATA_INIT
	m_pMainDlg = (CAtumAdminToolDlg*)AfxGetMainWnd();
	m_pODBCStmt = new CODBCStatement;

// 2007-10-30 by cmkwon, 세력별 해피아워 이벤트 구현 - 기존 변수를 주석 처리하고 배열 변수를 추가함
//	m_nHappyEvUID			= 0;
	util::zero(&m_PCBangHappyEv, sizeof(m_PCBangHappyEv));		// 2006-08-25 by cmkwon

	// 2007-10-30 by cmkwon, 세력별 해피아워 이벤트 구현 - 초기화
	for(int i=0; i < INFLUENCE_TYPE_COUNT; i++)
	{
		m_arrHappyEvPeriodStart[i].Reset();
		m_arrHappyEvPeriodEND[i].Reset();
	}
}

// 소멸자
CSCHappyHourEventAdminDlg::~CSCHappyHourEventAdminDlg()
{
	util::del(m_pODBCStmt);
}

void CSCHappyHourEventAdminDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSCHappyHourEventAdminDlg)
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER1, m_valueDTPTotalSetStartDate);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER3, m_valueDTPTotalSetEndDate);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER2, m_valueDTPTotalSetStartTime);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER4, m_valueDTPTotalSetEndTime);
	DDX_DateTimeCtrl(pDX, IDC_DATEPICKER_PCBANG_START, m_OleDatePCBangStart);
	DDX_DateTimeCtrl(pDX, IDC_TIMEPICKER_PCBANG_START, m_OleTimePCBangStart);
	DDX_DateTimeCtrl(pDX, IDC_DATEPICKER_PCBANG_END, m_OleDatePCBangEnd);
	DDX_DateTimeCtrl(pDX, IDC_TIMEPICKER_PCBANG_END, m_OleTimePCBangEnd);
	DDX_Control(pDX, IDC_GRID_EVENT_DETAIL, m_GridDetail);
	DDX_DateTimeCtrl(pDX, IDC_DATEPICKER_NORMAL_START, m_ctlOleDateNormalStart);
	DDX_DateTimeCtrl(pDX, IDC_DATEPICKER_NORMAL_END, m_ctlOleDateNormalEnd);
	DDX_DateTimeCtrl(pDX, IDC_DATEPICKER_VCN_START, m_ctlOleDateVCNStart);
	DDX_DateTimeCtrl(pDX, IDC_DATEPICKER_VCN_END, m_ctlOleDateVCNEnd);
	DDX_DateTimeCtrl(pDX, IDC_DATEPICKER_ANI_START, m_ctlOleDateANIStart);
	DDX_DateTimeCtrl(pDX, IDC_DATEPICKER_ANI_END, m_ctlOleDateANIEnd);
	DDX_DateTimeCtrl(pDX, IDC_TIMEPICKER_NORMAL_START, m_ctlOleTimeNormalStart);
	DDX_DateTimeCtrl(pDX, IDC_TIMEPICKER_NORMAL_END, m_ctlOleTimeNormalEnd);
	DDX_DateTimeCtrl(pDX, IDC_TIMEPICKER_VCN_START, m_ctlOleTimeVCNStart);
	DDX_DateTimeCtrl(pDX, IDC_TIMEPICKER_VCN_END, m_ctlOleTimeVCNEnd);
	DDX_DateTimeCtrl(pDX, IDC_TIMEPICKER_ANI_START, m_ctlOleTimeANIStart);
	DDX_DateTimeCtrl(pDX, IDC_TIMEPICKER_ANI_END, m_ctlOleTimeANIEnd);
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_EDIT_DROPITEMRATE, m_PCBangHappyEv.fDropItemRate2);
	DDX_Text(pDX, IDC_EDIT_DROPRAREITEMRATE, m_PCBangHappyEv.fDropRareItemRate2);
	DDX_Text(pDX, IDC_EDIT_EXPRATE, m_PCBangHappyEv.fEXPRate2);
	DDX_Text(pDX, IDC_EDIT_EXPREPAIRRATE, m_PCBangHappyEv.fEXPRepairRate2);
	DDX_Text(pDX, IDC_EDIT_SPIRATE, m_PCBangHappyEv.fSPIRate2);
	DDX_Text(pDX, IDC_EDIT_WPRATE, m_PCBangHappyEv.fWarPointRate2);				// 2007-06-26 by dhjin, 워포인트 이벤트 관련 추가 
}


BEGIN_MESSAGE_MAP(CSCHappyHourEventAdminDlg, CDialog)
	//{{AFX_MSG_MAP(CSCHappyHourEventAdminDlg)
	ON_BN_CLICKED(IDC_BUTTON_TOTAL_UPDATE, OnButtonTotalUpdate)
	ON_BN_CLICKED(IDC_BUTTON_END, OnButtonEnd)
	ON_COMMAND(IDR_CONMENU_SELECTED_INSERT, OnConmenuSelectedInsert)
	ON_COMMAND(IDR_CONMENU_SELECTED_UPDATE, OnConmenuSelectedUpdate)
	ON_COMMAND(IDR_CONMENU_SELECTED_DELETE, OnConmenuSelectedDelete)
	ON_COMMAND(IDR_CONMENU_NOTSELECTED_INSERT, OnConmenuNotselectedInsert)
	ON_BN_CLICKED(IDC_BTN_UPDATE_PCBANG_HAPPYEV, OnBtnUpdatePcbangHappyev)
	ON_BN_CLICKED(IDC_BTN_RELOAD_HAPPYHOUREV, OnBtnReloadHappyEv)
	ON_BN_CLICKED(IDC_BTN_RELOAD_PCBANG_LIST, OnBtnReloadPCbangList)
	ON_BN_CLICKED(IDC_BTN_UPDATE_PERIOD_NORMAL, OnBtnUpdatePeriodNormal)
	ON_BN_CLICKED(IDC_BTN_UPDATE_PERIOD_VCN, OnBtnUpdatePeriodVcn)
	ON_BN_CLICKED(IDC_BTN_UPDATE_PERIOD_ANI, OnBtnUpdatePeriodAni)
	//}}AFX_MSG_MAP
	ON_NOTIFY(NM_RCLICK, IDC_GRID_EVENT_DETAIL, OnGridRClick)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSCHappyHourEventAdminDlg message handlers

BOOL CSCHappyHourEventAdminDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	// Connect DB
	if (!m_pODBCStmt->Init(m_pMainDlg->m_pServerInfo4Admin->DBIP, m_pMainDlg->m_pServerInfo4Admin->DBPort, m_pMainDlg->m_pServerInfo4Admin->DBName,
							m_pMainDlg->m_pServerInfo4Admin->DBUID, m_pMainDlg->m_pServerInfo4Admin->DBPWD, GetSafeHwnd())
	)
	{
		MessageBox(STRERR_S_SCADMINTOOL_0013);
		EndDialog(-1);
		return FALSE;
	}

	//현재 DB에 있는 값 불러온다.
	GetHappyHourEventDetailInfo();

	///////////////////////////////////////////////////////////////////////////////
	// 2007-10-02 by cmkwon, SCAdminTool 수정 권한 처리 - 해피아워 이벤트 처리
	// 2012-02-01 by jhseol, 베트남 GM 권한 제한
#ifdef S_MANAGER_ADMIN_HSSON
	GetDlgItem(IDC_BUTTON_TOTAL_UPDATE)->EnableWindow(COMPARE_RACE(m_pMainDlg->m_usManagerAccountType, RACE_OPERATION));
	GetDlgItem(IDC_BTN_UPDATE_PCBANG_HAPPYEV)->EnableWindow(COMPARE_RACE(m_pMainDlg->m_usManagerAccountType, RACE_OPERATION));
	GetDlgItem(IDC_BTN_RELOAD_HAPPYHOUREV)->EnableWindow(COMPARE_RACE(m_pMainDlg->m_usManagerAccountType, RACE_OPERATION));
	GetDlgItem(IDC_BTN_UPDATE_PERIOD_NORMAL)->EnableWindow(COMPARE_RACE(m_pMainDlg->m_usManagerAccountType, RACE_OPERATION));	// 2009-02-04 by cmkwon, AdminTool의 GM 권한 수정 - 이벤트 관련 검색은 가능, 수정 불가
	GetDlgItem(IDC_BTN_UPDATE_PERIOD_VCN)->EnableWindow(COMPARE_RACE(m_pMainDlg->m_usManagerAccountType, RACE_OPERATION));		// 2009-02-04 by cmkwon, AdminTool의 GM 권한 수정 - 이벤트 관련 검색은 가능, 수정 불가
	GetDlgItem(IDC_BTN_UPDATE_PERIOD_ANI)->EnableWindow(COMPARE_RACE(m_pMainDlg->m_usManagerAccountType, RACE_OPERATION));		// 2009-02-04 by cmkwon, AdminTool의 GM 권한 수정 - 이벤트 관련 검색은 가능, 수정 불가
#else
	GetDlgItem(IDC_BUTTON_TOTAL_UPDATE)->EnableWindow(m_pMainDlg->IsManagerAdministrator());
	GetDlgItem(IDC_BTN_UPDATE_PCBANG_HAPPYEV)->EnableWindow(m_pMainDlg->IsManagerAdministrator());
	GetDlgItem(IDC_BTN_RELOAD_HAPPYHOUREV)->EnableWindow(m_pMainDlg->IsManagerAdministrator());
	GetDlgItem(IDC_BTN_UPDATE_PERIOD_NORMAL)->EnableWindow(m_pMainDlg->IsManagerAdministrator());	// 2009-02-04 by cmkwon, AdminTool의 GM 권한 수정 - 이벤트 관련 검색은 가능, 수정 불가
	GetDlgItem(IDC_BTN_UPDATE_PERIOD_VCN)->EnableWindow(m_pMainDlg->IsManagerAdministrator());		// 2009-02-04 by cmkwon, AdminTool의 GM 권한 수정 - 이벤트 관련 검색은 가능, 수정 불가
	GetDlgItem(IDC_BTN_UPDATE_PERIOD_ANI)->EnableWindow(m_pMainDlg->IsManagerAdministrator());		// 2009-02-04 by cmkwon, AdminTool의 GM 권한 수정 - 이벤트 관련 검색은 가능, 수정 불가
#endif
	// end 2012-02-01 by jhseol, 베트남 GM 권한 제한
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CSCHappyHourEventAdminDlg::InitGridDetail()
{
	m_GridDetail.SetBkColor(0xFFFFFF);

	int m_nRows = 1;
	int m_nCols = 14;			// 2007-10-30 by cmkwon, 세력별 해피아워 이벤트 구현 - 세력 필드 추가

	m_GridDetail.SetEditable(FALSE);
	m_GridDetail.SetListMode(TRUE);
	m_GridDetail.SetSingleRowSelection(TRUE);
	m_GridDetail.EnableSelection(TRUE);
	m_GridDetail.SetFrameFocusCell(FALSE);
	m_GridDetail.SetTrackFocusCell(FALSE);

	m_GridDetail.SetRowCount(m_nRows);
	m_GridDetail.SetColumnCount(m_nCols);
	m_GridDetail.SetFixedRowCount(1);
	m_GridDetail.SetColumnWidth(0, 0);

	// 칼럼 만들기
	m_nCols = 0;
	GV_ITEM Item;
	Item.mask = GVIF_TEXT|GVIF_FORMAT;
	Item.row = 0;
	Item.nFormat = DT_LEFT|DT_VCENTER|DT_SINGLELINE;

	Item.col = m_nCols++;
	Item.strText.Format("EvnetUID");
	m_GridDetail.SetItem(&Item);

	// 2006-08-25 by cmkwon
	Item.col = m_nCols++;
	Item.strText.Format("ServerGroup");
	m_GridDetail.SetItem(&Item);

	// 2007-10-30 by cmkwon, 세력별 해피아워 이벤트 구현 -
	Item.col = m_nCols++;
	Item.strText.Format("Influence");
	m_GridDetail.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("Day Of Week");
	m_GridDetail.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("MinLevel");
	m_GridDetail.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("MaxLevel");
	m_GridDetail.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("Start Time");
	m_GridDetail.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("End Time");
	m_GridDetail.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("EXP Plus");
	m_GridDetail.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("SPI Plus");
	m_GridDetail.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("EXPRepair Plus");
	m_GridDetail.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("DropItem Plus");
	m_GridDetail.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("DropRareItem Plus");
	m_GridDetail.SetItem(&Item);

	// 2007-06-26 by dhjin, 워포인트 이벤트 관련 추가
	Item.col = m_nCols++;
	Item.strText.Format("WarPoint Plus");
	m_GridDetail.SetItem(&Item);	

	// arrange grid
	m_GridDetail.AutoSize();
	m_GridDetail.ExpandColumnsToFit();

	// clean all cells
	CCellRange tmpCellRange(1, 0, m_GridDetail.GetRowCount()-1, m_GridDetail.GetColumnCount()-1);
	m_GridDetail.ClearCells(tmpCellRange);
}



void CSCHappyHourEventAdminDlg::GetHappyHourEventDetailInfo()
{
	this->InitGridDetail();
	
	GV_ITEM Item;
	Item.mask = GVIF_TEXT|GVIF_FORMAT;
	Item.row = 1;
	Item.nFormat = GRID_CELL_FORMAT;
	
	CString szSQLQuery;
	// 2008-03-13 by dhjin, Level별 해피아워 이벤트 구현 - MinLevel, MaxLevel 추가
	// szSQLQuery.Format("SELECT uniquenumber, ServerGroupID, dayofweek, starttime, endtime, exprate, spirate, exprepairrate, dropitemrate, droprareitemrate, WarPointRate, InfluenceType, MinLevel, MaxLevel \
	//	FROM ti_HappyHourEvent WITH (NOLOCK) order by ServerGroupID, InfluenceType, dayofweek");
	// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
	//szSQLQuery.Format("SELECT uniquenumber, ServerGroupID, dayofweek, starttime, endtime, exprate, spirate, exprepairrate, dropitemrate, droprareitemrate, WarPointRate, InfluenceType, MinLevel, MaxLevel \
	//	FROM ti_HappyHourEvent WITH (NOLOCK) order by ServerGroupID, InfluenceType, dayofweek");

// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
//	szSQLQuery.Format(QUERY_080702_0025);		// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
//	BOOL bRet = m_pODBCStmt->ExecuteQuery((LPCSTR)szSQLQuery);
	BOOL bRet = m_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_080827_0025));
	if (!bRet)
	{
		// clean up
		m_pODBCStmt->FreeStatement();
		MessageBox("Error");
		return;
	}

	CString starttime;
	SQLINTEGER arrCB[15] = {SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS, SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS
							,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS};			// 2008-03-13 by dhjin, Level별 해피아워 이벤트 구현 - MinLevel, MaxLevel 추가

	SHAPPY_HOUR_EVENT tmHappyEv;	
	util::zero(&tmHappyEv, sizeof(tmHappyEv));

	SQL_TIMESTAMP_STRUCT tmpStartTime;
	SQL_TIMESTAMP_STRUCT tmpEndTime;

	SQLBindCol(m_pODBCStmt->m_hstmt, 1, SQL_C_ULONG, &tmHappyEv.EventUniqueNumber, 0,			&arrCB[1]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 2, SQL_C_ULONG, &tmHappyEv.ServerGroupID, 0,				&arrCB[2]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 3, SQL_C_ULONG, &tmHappyEv.DayOfWeek, 0,					&arrCB[3]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 4, SQL_C_TIMESTAMP, &tmpStartTime, 0,						&arrCB[4]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 5, SQL_C_TIMESTAMP, &tmpEndTime, 0,						&arrCB[5]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 6, SQL_C_FLOAT, &tmHappyEv.fEXPRate2, SIZE_MAX_ITEM_NAME,	&arrCB[6]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 7, SQL_C_FLOAT, &tmHappyEv.fSPIRate2, 0,					&arrCB[7]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 8, SQL_C_FLOAT, &tmHappyEv.fEXPRepairRate2, 0,				&arrCB[8]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 9, SQL_C_FLOAT, &tmHappyEv.fDropItemRate2, 0,				&arrCB[9]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 10, SQL_C_FLOAT, &tmHappyEv.fDropRareItemRate2, 0,			&arrCB[10]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 11, SQL_C_FLOAT, &tmHappyEv.fWarPointRate2, 0,				&arrCB[11]);		// 2007-06-26 by dhjin, 워포인트 이벤트 관련 추가
	SQLBindCol(m_pODBCStmt->m_hstmt, 12, SQL_C_TINYINT, &tmHappyEv.InfluenceType3, 0,			&arrCB[12]);		// 2007-10-30 by cmkwon, 세력별 해피아워 이벤트 구현 - InfluenceType 필드 추가
	SQLBindCol(m_pODBCStmt->m_hstmt, 13, SQL_C_ULONG, &tmHappyEv.MinLevel, 0,					&arrCB[13]);		// 2008-03-13 by dhjin, Level별 해피아워 이벤트 구현 - 
	SQLBindCol(m_pODBCStmt->m_hstmt, 14, SQL_C_ULONG, &tmHappyEv.MaxLevel, 0,					&arrCB[14]);		// 2008-03-13 by dhjin, Level별 해피아워 이벤트 구현 - 


// 2007-10-30 by cmkwon, 세력별 해피아워 이벤트 구현 - 기존 변수를 주석 처리하고 배열 변수를 추가함
//	m_nHappyEvUID			= 0;
	util::zero(&m_PCBangHappyEv, sizeof(m_PCBangHappyEv));		// 2006-08-28 by cmkwon
	m_mtvectHappyEventList.clear();			// 2006-08-25 by cmkwon, 
	// DB에 값이 없을때까지 loop를 돈다
	while ( (bRet = SQLFetch(m_pODBCStmt->m_hstmt)) != SQL_NO_DATA)
	{
		tmHappyEv.atimeStartTime2	= tmpStartTime;
		tmHappyEv.atimeEndTime2		= tmpEndTime;		
		m_mtvectHappyEventList.push_back(tmHappyEv);

		util::zero(&tmHappyEv, sizeof(tmHappyEv));
	}
	m_pODBCStmt->FreeStatement();	// clean up

	for(int i=0; i < m_mtvectHappyEventList.size(); i++)
	{
		SHAPPY_HOUR_EVENT *pHappyEv = &m_mtvectHappyEventList[i];
		switch(pHappyEv->DayOfWeek)
		{
		case DAY_OF_WEEK_SUMDAY:
		case DAY_OF_WEEK_MONDAY:
		case DAY_OF_WEEK_TUESDAY:
		case DAY_OF_WEEK_WEDNESDAY:
		case DAY_OF_WEEK_THURSDAY:
		case DAY_OF_WEEK_FRIDAY:
		case DAY_OF_WEEK_SATURDAY:
			{
				int nRowCount = m_GridDetail.GetRowCount();
				m_GridDetail.SetRowCount(nRowCount+1);
				InsertItemToGridDetail(pHappyEv, nRowCount);
			}
			break;
		case DAY_OF_WEEK_HAPPYHOUREVENT_PERIOD:
			{

				// 2007-10-30 by cmkwon, 세력별 해피아워 이벤트 구현 -
				ATUM_DATE_TIME *patStart	= this->GetHappyEvPeriodStartDateTime(pHappyEv->InfluenceType3);
				ATUM_DATE_TIME *patEnd		= this->GetHappyEvPeriodEndDateTime(pHappyEv->InfluenceType3);
				if(NULL == patStart
					|| NULL == patEnd)
				{
					continue;
				}

				*patStart		= pHappyEv->atimeStartTime2;
				*patEnd			= pHappyEv->atimeEndTime2;				
				switch(pHappyEv->InfluenceType3)
				{
				case INFLUENCE_TYPE_NORMAL:
					{
						m_ctlOleDateNormalStart.SetDate(patStart->Year, patStart->Month, patStart->Day);
						m_ctlOleTimeNormalStart.SetTime(patStart->Hour, patStart->Minute, 0);

						m_ctlOleDateNormalEnd.SetDate(patEnd->Year, patEnd->Month, patEnd->Day);
						m_ctlOleTimeNormalEnd.SetTime(patEnd->Hour, patEnd->Minute, 0);
					}
					break;
				case INFLUENCE_TYPE_VCN:
					{
						m_ctlOleDateVCNStart.SetDate(patStart->Year, patStart->Month, patStart->Day);
						m_ctlOleTimeVCNStart.SetTime(patStart->Hour, patStart->Minute, 0);

						m_ctlOleDateVCNEnd.SetDate(patEnd->Year, patEnd->Month, patEnd->Day);
						m_ctlOleTimeVCNEnd.SetTime(patEnd->Hour, patEnd->Minute, 0);
					}
					break;
				case INFLUENCE_TYPE_ANI:
					{
						m_ctlOleDateANIStart.SetDate(patStart->Year, patStart->Month, patStart->Day);
						m_ctlOleTimeANIStart.SetTime(patStart->Hour, patStart->Minute, 0);

						m_ctlOleDateANIEnd.SetDate(patEnd->Year, patEnd->Month, patEnd->Day);
						m_ctlOleTimeANIEnd.SetTime(patEnd->Hour, patEnd->Minute, 0);
					}
					break;
				case INFLUENCE_TYPE_ALL_MASK:
					{
						m_valueDTPTotalSetStartDate.SetDate(patStart->Year, patStart->Month, patStart->Day);
						m_valueDTPTotalSetStartTime.SetTime(patStart->Hour, patStart->Minute, 0);

						m_valueDTPTotalSetEndDate.SetDate(patEnd->Year, patEnd->Month, patEnd->Day);
						m_valueDTPTotalSetEndTime.SetTime(patEnd->Hour, patEnd->Minute, 0);
					}
					break;
				}
				UpdateData(FALSE);
			}
			break;
		case DAY_OF_WEEK_PCBANG_HAPPYHOUREVENT:
			{
				m_PCBangHappyEv					= *pHappyEv;
				m_OleDatePCBangStart.SetDate(m_PCBangHappyEv.atimeStartTime2.Year, m_PCBangHappyEv.atimeStartTime2.Month, m_PCBangHappyEv.atimeStartTime2.Day);
				m_OleTimePCBangStart.SetTime(m_PCBangHappyEv.atimeStartTime2.Hour, m_PCBangHappyEv.atimeStartTime2.Minute, 0);

				m_OleDatePCBangEnd.SetDate(m_PCBangHappyEv.atimeEndTime2.Year, m_PCBangHappyEv.atimeEndTime2.Month, m_PCBangHappyEv.atimeEndTime2.Day);
				m_OleTimePCBangEnd.SetTime(m_PCBangHappyEv.atimeEndTime2.Hour, m_PCBangHappyEv.atimeEndTime2.Minute, 0);

				UpdateData(FALSE);
			}
			break;
		}
	}

	m_GridDetail.AutoSize();
	m_GridDetail.ExpandColumnsToFit();
}

void CSCHappyHourEventAdminDlg::InsertItemToGridDetail(SHAPPY_HOUR_EVENT *i_pHappyEv, int i_nRow)
{
	// 칼럼 만들기
	int m_nCols = 0;

	// 컬럼에 값 넣기
	GV_ITEM Item;
	Item.mask = GVIF_TEXT|GVIF_FORMAT;
	Item.row = i_nRow;
	Item.nFormat = GRID_CELL_FORMAT;

	Item.col = m_nCols++;
	Item.strText.Format("%d", i_pHappyEv->EventUniqueNumber);
	m_GridDetail.SetItem(&Item);

	Item.col = m_nCols++;
	if(0 == i_pHappyEv->ServerGroupID)
	{
		Item.strText.Format("%s", STR_ALL_SERVERGROUP_NAME);
	}
	else
	{
		Item.strText.Format("server %d", 1 + (i_pHappyEv->ServerGroupID - SERVERGROUP_START_ID));
	}
	m_GridDetail.SetItem(&Item);

	// 2007-10-30 by cmkwon, 세력별 해피아워 이벤트 구현 - 세력 필드 추가	
	Item.col = m_nCols++;
	Item.strText.Format("%s", CAtumSJ::GetInfluenceTypeString(i_pHappyEv->InfluenceType3));
	m_GridDetail.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("%s", DayofWeekToString(i_pHappyEv->DayOfWeek));
	m_GridDetail.SetItem(&Item);

	// 2008-03-13 by dhjin, Level별 해피아워 이벤트 구현 - 
	Item.col = m_nCols++;
	Item.strText.Format("%d", i_pHappyEv->MinLevel);
	m_GridDetail.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("%d", i_pHappyEv->MaxLevel);
	m_GridDetail.SetItem(&Item);


	Item.col = m_nCols++;
	Item.strText.Format("%s", i_pHappyEv->atimeStartTime2.GetDateTimeString().GetBuffer());
	Item.strText = Item.strText.Right(8);
	m_GridDetail.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("%s", i_pHappyEv->atimeEndTime2.GetDateTimeString().GetBuffer());
	Item.strText = Item.strText.Right(8);
	m_GridDetail.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("%.1f", i_pHappyEv->fEXPRate2);
	m_GridDetail.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("%.1f", i_pHappyEv->fSPIRate2);
	m_GridDetail.SetItem(&Item);
	
	Item.col = m_nCols++;
	Item.strText.Format("%.1f", i_pHappyEv->fEXPRepairRate2);
	m_GridDetail.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("%.1f", i_pHappyEv->fDropItemRate2);
	m_GridDetail.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("%.1f", i_pHappyEv->fDropRareItemRate2);
	m_GridDetail.SetItem(&Item);

	// 2007-06-26 by dhjin, 워포인트 이벤트 관련 추가
	Item.col = m_nCols++;
	Item.strText.Format("%.1f", i_pHappyEv->fWarPointRate2);
	m_GridDetail.SetItem(&Item);
}

void CSCHappyHourEventAdminDlg::OnButtonTotalUpdate() 
{
	// TODO: Add your control notification handler code here
	UpdateData();

	ATUM_DATE_TIME *pATStart	= GetHappyEvPeriodStartDateTime(INFLUENCE_TYPE_ALL_MASK);
	ATUM_DATE_TIME *pATEnd		= GetHappyEvPeriodEndDateTime(INFLUENCE_TYPE_ALL_MASK);

	pATStart->SetDateTime(m_valueDTPTotalSetStartDate.GetYear(), m_valueDTPTotalSetStartDate.GetMonth(), m_valueDTPTotalSetStartDate.GetDay()
		, m_valueDTPTotalSetStartTime.GetHour(), m_valueDTPTotalSetStartTime.GetMinute(), 0);
	pATEnd->SetDateTime(m_valueDTPTotalSetEndDate.GetYear(), m_valueDTPTotalSetEndDate.GetMonth(), m_valueDTPTotalSetEndDate.GetDay()
		, m_valueDTPTotalSetEndTime.GetHour(), m_valueDTPTotalSetEndTime.GetMinute(), 0);

	// 2007-11-01 by cmkwon, DB 에 업데이트 한다.
	this->UpdateHappyHourEventPeriod(INFLUENCE_TYPE_ALL_MASK, pATStart, pATEnd);

	// 2007-11-01 by cmkwon, 모든 이벤트 정보를 다시 로딩한다.
	GetHappyHourEventDetailInfo();

// 2007-10-30 by cmkwon, 세력별 해피아워 이벤트 구현 - UpdateHappyHourEventPeriod() 함수로 처리, 위와 같이 수정 함
//	m_atiHappyEvStartDateTime.SetDateTime(m_valueDTPTotalSetStartDate.GetYear(), m_valueDTPTotalSetStartDate.GetMonth(), m_valueDTPTotalSetStartDate.GetDay()
//		, m_valueDTPTotalSetStartTime.GetHour(), m_valueDTPTotalSetStartTime.GetMinute(), 0);
//	m_atiHappyEvEndDateTime.SetDateTime(m_valueDTPTotalSetEndDate.GetYear(), m_valueDTPTotalSetEndDate.GetMonth(), m_valueDTPTotalSetEndDate.GetDay()
//		, m_valueDTPTotalSetEndTime.GetHour(), m_valueDTPTotalSetEndTime.GetMinute(), 0);
//
//	CString szQuery;
//	if(0 == m_nHappyEvUID)
//	{// 2006-08-25 by cmkwon, 추가해야함
//		szQuery.Format("INSERT ti_happyhourevent(ServerGroupID, dayofweek, starttime, endtime, exprate, spirate, exprepairrate, dropitemrate, droprareitemrate, WarPointRate) values( 0, %d, '%s', '%s', 0, 0, 0, 0, 0, 0)"
//			, DAY_OF_WEEK_HAPPYHOUREVENT_PERIOD
//			, m_atiHappyEvStartDateTime.GetDateTimeString(STRNBUF(SIZE_MAX_ATUM_DATE_TIME_STRING))
//			, m_atiHappyEvEndDateTime.GetDateTimeString(STRNBUF(SIZE_MAX_ATUM_DATE_TIME_STRING)));	// 2007-06-26 by dhjin, 워포인트 이벤트 관련 추가
//	}
//	else
//	{// 2006-08-25 by cmkwon, Update
//		szQuery.Format("UPDATE ti_happyhourevent SET starttime = '%s', endtime = '%s' where UniqueNumber= %d"
//			, m_atiHappyEvStartDateTime.GetDateTimeString(STRNBUF(SIZE_MAX_ATUM_DATE_TIME_STRING))
//			, m_atiHappyEvEndDateTime.GetDateTimeString(STRNBUF(SIZE_MAX_ATUM_DATE_TIME_STRING))
//			, m_nHappyEvUID);
//	}
//
//	BOOL bRet = m_pODBCStmt->ExecuteQuery((LPCSTR)szQuery);
//	if (!bRet)
//	{
//		m_pODBCStmt->FreeStatement();		// cleanup
//
//		MessageBox("HappyHourEvent period error !!");
//		return;
//	}
//	m_pODBCStmt->FreeStatement();	// cleanup
//
//	GetHappyHourEventDetailInfo();
}


CString CSCHappyHourEventAdminDlg::DayofWeekToString(int nDayofweek)
{
	// int형 요일값을 문자열값으로 변형시킨다
	switch(nDayofweek)
	{
	case 0:
		return "Sunday";
	case 1:
		return "Monday";
	case 2:
		return "Tuesday";
	case 3:
		return "Wednesday";
	case 4:
		return "Thursday";
	case 5:
		return "Friday";
	case 6:
		return "Saturday";
	}
	return "FALSE";
}
void CSCHappyHourEventAdminDlg::OnButtonEnd() 
{
	// TODO: Add your control notification handler code here
	EndDialog(0);
}

void CSCHappyHourEventAdminDlg::OnConmenuSelectedInsert() 
{
	// TODO: Add your command handler code here

	
	CSetHappyEv	dlg(TRUE, this);
	if (IDOK != dlg.DoModal())
	{
		return;
	}

	if(FALSE == CheckHappyHourEvent(&dlg.m_happyEv))
	{
		return;
	}

	// 각 ctrl의 값을 바방으로 insert한다.
	CString szQuery;

	// 2007-10-30 by cmkwon, 세력별 해피아워 이벤트 구현 - InfluenceType 필드 추가
	// 2008-01-24 by cmkwon, S_A: 날짜 관련 로컬리 제이션 처리 버그 수정 - GetDateTimeString() 를 GetSQLDateTimeString() 로 변경 함
// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
// 	szQuery.Format("INSERT ti_happyhourevent(ServerGroupID, dayofweek, starttime, endtime, exprate, spirate, exprepairrate, dropitemrate, droprareitemrate, WarPointRate, InfluenceType, MinLevel, MaxLevel \
// 					) values( %d, %d, '%s', '%s', %6.4f, %6.4f, %6.4f, %6.4f, %6.4f, %6.4f, %d, %d, %d)"
// 		, dlg.m_happyEv.ServerGroupID, dlg.m_happyEv.DayOfWeek
// 		, dlg.m_happyEv.atimeStartTime2.GetSQLDateTimeString(STRNBUF(SIZE_MAX_SQL_DATETIME_STRING)), dlg.m_happyEv.atimeEndTime2.GetSQLDateTimeString(STRNBUF(SIZE_MAX_SQL_DATETIME_STRING))
// 		, dlg.m_happyEv.fEXPRate2, dlg.m_happyEv.fSPIRate2, dlg.m_happyEv.fEXPRepairRate2
// 		, dlg.m_happyEv.fDropItemRate2, dlg.m_happyEv.fDropRareItemRate2, dlg.m_happyEv.fWarPointRate2, dlg.m_happyEv.InfluenceType3
// 		, dlg.m_happyEv.MinLevel, dlg.m_happyEv.MaxLevel);	// 2008-03-14 by dhjin, Level별 해피아워 이벤트 구현 - // 2007-06-26 by dhjin, 워포인트 이벤트 관련 추가

// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
// 	szQuery.Format(QUERY_080702_0026		// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
// 		, dlg.m_happyEv.ServerGroupID, dlg.m_happyEv.DayOfWeek
// 		, dlg.m_happyEv.atimeStartTime2.GetSQLDateTimeString(STRNBUF(SIZE_MAX_SQL_DATETIME_STRING)), dlg.m_happyEv.atimeEndTime2.GetSQLDateTimeString(STRNBUF(SIZE_MAX_SQL_DATETIME_STRING))
// 		, dlg.m_happyEv.fEXPRate2, dlg.m_happyEv.fSPIRate2, dlg.m_happyEv.fEXPRepairRate2
// 		, dlg.m_happyEv.fDropItemRate2, dlg.m_happyEv.fDropRareItemRate2, dlg.m_happyEv.fWarPointRate2, dlg.m_happyEv.InfluenceType3
// 		, dlg.m_happyEv.MinLevel, dlg.m_happyEv.MaxLevel);	// 2008-03-14 by dhjin, Level별 해피아워 이벤트 구현 - // 2007-06-26 by dhjin, 워포인트 이벤트 관련 추가
// 
// 	BOOL bRet = m_pODBCStmt->ExecuteQuery((LPCSTR)szQuery);
	char tmStartTime[SIZE_MAX_SQL_DATETIME_STRING];		dlg.m_happyEv.atimeStartTime2.GetSQLDateTimeString(tmStartTime, SIZE_MAX_SQL_DATETIME_STRING);
	char tmEndTime[SIZE_MAX_SQL_DATETIME_STRING];		dlg.m_happyEv.atimeEndTime2.GetSQLDateTimeString(tmEndTime, SIZE_MAX_SQL_DATETIME_STRING);
	SQLHSTMT hstmt = m_pODBCStmt->GetSTMTHandle();
	SQLINTEGER arrCB2[14] = {SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS, SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,
								SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS};
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &(dlg.m_happyEv.ServerGroupID), 0,			&arrCB2[1]);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &(dlg.m_happyEv.DayOfWeek), 0,				&arrCB2[2]);
	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SQL_DATETIME_STRING, 0, tmStartTime, 0,	&arrCB2[3]);
	SQLBindParameter(hstmt, 4, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SQL_DATETIME_STRING, 0, tmEndTime, 0,		&arrCB2[4]);
	SQLBindParameter(hstmt, 5, SQL_PARAM_INPUT, SQL_C_FLOAT, SQL_FLOAT, 0, 0, &(dlg.m_happyEv.fEXPRate2), 0,				&arrCB2[5]);
	SQLBindParameter(hstmt, 6, SQL_PARAM_INPUT, SQL_C_FLOAT, SQL_FLOAT, 0, 0, &(dlg.m_happyEv.fSPIRate2), 0,				&arrCB2[6]);
	SQLBindParameter(hstmt, 7, SQL_PARAM_INPUT, SQL_C_FLOAT, SQL_FLOAT, 0, 0, &(dlg.m_happyEv.fEXPRepairRate2), 0,			&arrCB2[7]);
	SQLBindParameter(hstmt, 8, SQL_PARAM_INPUT, SQL_C_FLOAT, SQL_FLOAT, 0, 0, &(dlg.m_happyEv.fDropItemRate2), 0,			&arrCB2[8]);
	SQLBindParameter(hstmt, 9, SQL_PARAM_INPUT, SQL_C_FLOAT, SQL_FLOAT, 0, 0, &(dlg.m_happyEv.fDropRareItemRate2), 0,		&arrCB2[9]);
	SQLBindParameter(hstmt, 10, SQL_PARAM_INPUT, SQL_C_FLOAT, SQL_FLOAT, 0, 0, &(dlg.m_happyEv.fWarPointRate2), 0,			&arrCB2[10]);
	SQLBindParameter(hstmt, 11, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &(dlg.m_happyEv.InfluenceType3), 0,		&arrCB2[11]);
	SQLBindParameter(hstmt, 12, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &(dlg.m_happyEv.MinLevel), 0,				&arrCB2[12]);
	SQLBindParameter(hstmt, 13, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &(dlg.m_happyEv.MaxLevel), 0,				&arrCB2[13]);	
	BOOL bRet = m_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_080827_0026));
	if (!bRet)
	{
		m_pODBCStmt->FreeStatement();		// cleanup

		MessageBox("HappyHourEvent insert error !!");
		return;
	}
	m_pODBCStmt->FreeStatement();	// cleanup

	this->GetHappyHourEventDetailInfo();
}

void CSCHappyHourEventAdminDlg::OnConmenuSelectedUpdate() 
{
	// TODO: Add your command handler code here
	CCellID cellID = m_GridDetail.GetFocusCell();
	if(0 >= cellID.row)
	{// 2006-08-25 by cmkwon, 선택 오류
		return;
	}

	int nHappyEvUID				= atoi(m_GridDetail.GetItemText(cellID.row, 0));
	SHAPPY_HOUR_EVENT	*pHappyEv = this->FindHappyHourEvent(nHappyEvUID);
	if(NULL == pHappyEv)
	{
		return;
	}

	CSetHappyEv	dlg(FALSE, this);
	dlg.m_happyEv = *pHappyEv;

	if (IDOK != dlg.DoModal())
	{
		return;
	}

	if(FALSE == CheckHappyHourEvent(&dlg.m_happyEv))
	{
		return;
	}


	CString szQuery;

	// 2007-10-30 by cmkwon, 세력별 해피아워 이벤트 구현 - InfluenceType 필드 추가
	// 2008-01-24 by cmkwon, S_A: 날짜 관련 로컬리 제이션 처리 버그 수정 - GetDateTimeString() 를 GetSQLDateTimeString() 로 변경 함
	// 2008-03-13 by dhjin, Level별 해피아워 이벤트 구현 - MinLevel, MaxLevel 필드 추가
// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
// 	szQuery.Format("UPDATE ti_happyhourevent SET ServerGroupID= %d, dayofweek= %d, starttime = '%s', endtime = '%s', exprate = %6.2f, spirate = %6.2f, exprepairrate = %6.2f, dropitemrate = %6.2f, droprareitemrate = %6.2f , WarPointRate = %6.2f, InfluenceType = %d, MinLevel = %d, MaxLevel = %d where uniquenumber= %d",
// 		dlg.m_happyEv.ServerGroupID, dlg.m_happyEv.DayOfWeek
// 		, dlg.m_happyEv.atimeStartTime2.GetSQLDateTimeString(STRNBUF(SIZE_MAX_SQL_DATETIME_STRING)), dlg.m_happyEv.atimeEndTime2.GetSQLDateTimeString(STRNBUF(SIZE_MAX_SQL_DATETIME_STRING))
// 		, dlg.m_happyEv.fEXPRate2, dlg.m_happyEv.fSPIRate2, dlg.m_happyEv.fEXPRepairRate2
// 		, dlg.m_happyEv.fDropItemRate2, dlg.m_happyEv.fDropRareItemRate2, dlg.m_happyEv.fWarPointRate2, dlg.m_happyEv.InfluenceType3
// 		, dlg.m_happyEv.MinLevel, dlg.m_happyEv.MaxLevel, dlg.m_happyEv.EventUniqueNumber);	

// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
// 	szQuery.Format(QUERY_080702_0027,		// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
// 		dlg.m_happyEv.ServerGroupID, dlg.m_happyEv.DayOfWeek
// 		, dlg.m_happyEv.atimeStartTime2.GetSQLDateTimeString(STRNBUF(SIZE_MAX_SQL_DATETIME_STRING)), dlg.m_happyEv.atimeEndTime2.GetSQLDateTimeString(STRNBUF(SIZE_MAX_SQL_DATETIME_STRING))
// 		, dlg.m_happyEv.fEXPRate2, dlg.m_happyEv.fSPIRate2, dlg.m_happyEv.fEXPRepairRate2
// 		, dlg.m_happyEv.fDropItemRate2, dlg.m_happyEv.fDropRareItemRate2, dlg.m_happyEv.fWarPointRate2, dlg.m_happyEv.InfluenceType3
// 		, dlg.m_happyEv.MinLevel, dlg.m_happyEv.MaxLevel, dlg.m_happyEv.EventUniqueNumber);	
// 
// 	BOOL bRet = m_pODBCStmt->ExecuteQuery((LPCSTR)szQuery);
	char tmStartTime[SIZE_MAX_SQL_DATETIME_STRING];		dlg.m_happyEv.atimeStartTime2.GetSQLDateTimeString(tmStartTime, SIZE_MAX_SQL_DATETIME_STRING);
	char tmEndTime[SIZE_MAX_SQL_DATETIME_STRING];		dlg.m_happyEv.atimeEndTime2.GetSQLDateTimeString(tmEndTime, SIZE_MAX_SQL_DATETIME_STRING);
	SQLHSTMT hstmt = m_pODBCStmt->GetSTMTHandle();
	SQLINTEGER arrCB2[15] = {SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS, SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,
								SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS};
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &(dlg.m_happyEv.ServerGroupID), 0,			&arrCB2[1]);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &(dlg.m_happyEv.DayOfWeek), 0,				&arrCB2[2]);
	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SQL_DATETIME_STRING, 0, tmStartTime, 0,	&arrCB2[3]);
	SQLBindParameter(hstmt, 4, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SQL_DATETIME_STRING, 0, tmEndTime, 0,		&arrCB2[4]);
	SQLBindParameter(hstmt, 5, SQL_PARAM_INPUT, SQL_C_FLOAT, SQL_FLOAT, 0, 0, &(dlg.m_happyEv.fEXPRate2), 0,				&arrCB2[5]);
	SQLBindParameter(hstmt, 6, SQL_PARAM_INPUT, SQL_C_FLOAT, SQL_FLOAT, 0, 0, &(dlg.m_happyEv.fSPIRate2), 0,				&arrCB2[6]);
	SQLBindParameter(hstmt, 7, SQL_PARAM_INPUT, SQL_C_FLOAT, SQL_FLOAT, 0, 0, &(dlg.m_happyEv.fEXPRepairRate2), 0,			&arrCB2[7]);
	SQLBindParameter(hstmt, 8, SQL_PARAM_INPUT, SQL_C_FLOAT, SQL_FLOAT, 0, 0, &(dlg.m_happyEv.fDropItemRate2), 0,			&arrCB2[8]);
	SQLBindParameter(hstmt, 9, SQL_PARAM_INPUT, SQL_C_FLOAT, SQL_FLOAT, 0, 0, &(dlg.m_happyEv.fDropRareItemRate2), 0,		&arrCB2[9]);
	SQLBindParameter(hstmt, 10, SQL_PARAM_INPUT, SQL_C_FLOAT, SQL_FLOAT, 0, 0, &(dlg.m_happyEv.fWarPointRate2), 0,			&arrCB2[10]);
	SQLBindParameter(hstmt, 11, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &(dlg.m_happyEv.InfluenceType3), 0,		&arrCB2[11]);
	SQLBindParameter(hstmt, 12, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &(dlg.m_happyEv.MinLevel), 0,				&arrCB2[12]);
	SQLBindParameter(hstmt, 13, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &(dlg.m_happyEv.MaxLevel), 0,				&arrCB2[13]);	
	SQLBindParameter(hstmt, 14, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &(dlg.m_happyEv.EventUniqueNumber), 0,		&arrCB2[14]);	
	BOOL bRet = m_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_080827_0027));
	if (!bRet)
	{
		m_pODBCStmt->FreeStatement();		// cleanup

		AfxMessageBox("HappyHourEvent update error !!");
		return;
	}
	m_pODBCStmt->FreeStatement();	// cleanup

	this->GetHappyHourEventDetailInfo();
}

void CSCHappyHourEventAdminDlg::OnConmenuSelectedDelete() 
{
	// TODO: Add your command handler code here
	
	CCellID cellID = m_GridDetail.GetFocusCell();
	if(0 >= cellID.row)
	{// 2006-08-25 by cmkwon, 선택 오류
		return;
	}

	int nHappyEvUID				= atoi(m_GridDetail.GetItemText(cellID.row, 0));
	SHAPPY_HOUR_EVENT	*pHappyEv = this->FindHappyHourEvent(nHappyEvUID);
	if(NULL == pHappyEv)
	{
		return;
	}

//	CString szQuery;
	// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
	//szQuery.Format("DELETE ti_happyhourevent WHERE uniquenumber = %d", pHappyEv->EventUniqueNumber);
// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
//	szQuery.Format(QUERY_080702_0261, pHappyEv->EventUniqueNumber);
	SQLBindParameter(m_pODBCStmt->m_hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pHappyEv->EventUniqueNumber, 0, NULL);	
	BOOL bRet = m_pODBCStmt->ExecuteQuery((char*)PROCEDURE_080827_0261);
	if (!bRet)
	{
		m_pODBCStmt->FreeStatement();		// cleanup

		MessageBox("HappyHourEvent delete error !!");
		return;
	}
	m_pODBCStmt->FreeStatement();		// cleanup

	this->GetHappyHourEventDetailInfo();
}

void CSCHappyHourEventAdminDlg::OnConmenuNotselectedInsert() 
{
	// TODO: Add your command handler code here
	OnConmenuSelectedInsert();
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CSCHappyHourEventAdminDlg::OnGridRClick(NMHDR *pNotifyStruct, LRESULT* /*pResult*/)
/// \brief		
/// \author		cmkwon
/// \date		2006-08-25 ~ 2006-08-25
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CSCHappyHourEventAdminDlg::OnGridRClick(NMHDR *pNotifyStruct, LRESULT* /*pResult*/)
{
	UpdateData();

    NM_GRIDVIEW* pGridView = (NM_GRIDVIEW*)pNotifyStruct;
	if (pGridView->iRow == 0)
	{// 2006-08-25 by cmkwon, 목차행 선택
		return;
	}

	// 2009-02-04 by cmkwon, AdminTool의 GM 권한 수정 - 이벤트 관련 검색은 가능, 수정 불가
	// 2012-02-01 by jhseol, 베트남 GM 권한 제한
#ifdef S_MANAGER_ADMIN_HSSON
	if(FALSE == COMPARE_RACE(m_pMainDlg->m_usManagerAccountType, RACE_OPERATION))
#else
	if(FALSE == m_pMainDlg->IsManagerAdministrator())
#endif
		// 2012-02-01 by jhseol, 베트남 GM 권한 제한
	{
		AfxMessageBox(ADSTRMSG_090204_0001);
		return;
	}

	m_GridDetail.SetFocusCell(-1,-1);
	m_GridDetail.SetSelectedRange(pGridView->iRow, 0, pGridView->iRow, m_GridDetail.GetColumnCount()-1, TRUE, TRUE);
	m_GridDetail.SetFocusCell(pGridView->iRow, pGridView->iColumn);

	// show context menu
	CMenu muTemp, *pContextMenu;
	muTemp.LoadMenu(IDR_CONTEXT_MENU_HAPPYEV);
	CPoint point;
	GetCursorPos(&point);

	if(pGridView->iRow > 0)
	{// 2006-08-25 by cmkwon, 유효한 행을 선택
		pContextMenu = muTemp.GetSubMenu(0);
	}
	else
	{// 2006-08-25 by cmkwon, 유효하지 않은 행을 선택
		pContextMenu = muTemp.GetSubMenu(1);
	}
	pContextMenu->TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y, this);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			SHAPPY_HOUR_EVENT * CSCHappyHourEventAdminDlg::FindHappyHourEvent(int i_nHappyEvUID)
/// \brief		
/// \author		cmkwon
/// \date		2006-08-25 ~ 2006-08-25
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
SHAPPY_HOUR_EVENT * CSCHappyHourEventAdminDlg::FindHappyHourEvent(int i_nHappyEvUID)
{
	for(int i=0; i < m_mtvectHappyEventList.size(); i++)
	{
		if(i_nHappyEvUID == m_mtvectHappyEventList[i].EventUniqueNumber)
		{
			return &m_mtvectHappyEventList[i];
		}
	}

	return NULL;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CSCHappyHourEventAdminDlg::CheckHappyHourEvent(SHAPPY_HOUR_EVENT *i_pHappyEv)
/// \brief		
/// \author		cmkwon
/// \date		2006-08-30 ~ 2006-08-30
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CSCHappyHourEventAdminDlg::CheckHappyHourEvent(SHAPPY_HOUR_EVENT *i_pHappyEv)
{
	// 2007-10-30 by cmkwon, 세력별 해피아워 이벤트 구현 - 기본값이 모두 0.0f 로 변경 함
	if(0.0f >= i_pHappyEv->fEXPRate2
		&& 0.0f >= i_pHappyEv->fSPIRate2
		&& 0.0f >= i_pHappyEv->fEXPRepairRate2
		&& 0.0f >= i_pHappyEv->fDropItemRate2
		&& 0.0f >= i_pHappyEv->fDropRareItemRate2
		&& 0.0f >= i_pHappyEv->fWarPointRate2)	// 2007-06-26 by dhjin, 워포인트 이벤트 관련 추가
	{
		AfxMessageBox("All HappyHourEvent Rate is default values !!");
		return FALSE;
	}

	// 2008-03-13 by dhjin, Level별 해피아워 이벤트 구현 - 
	if(i_pHappyEv->MaxLevel < i_pHappyEv->MinLevel)
	{
		AfxMessageBox("Please Check Minlevel");
		return FALSE;
	}
	else if(i_pHappyEv->MinLevel < 1
			|| i_pHappyEv->MinLevel > CHARACTER_MAX_LEVEL)	// 2010-03-11 by cmkwon, 지원레벨 110 상항 처리 빠진것 수정 - 
	{
		AfxMessageBox("Please Check Minlevel");
		return FALSE;
	}
	else if(i_pHappyEv->MaxLevel < 1
			|| i_pHappyEv->MaxLevel > CHARACTER_MAX_LEVEL)		// 2010-03-11 by cmkwon, 지원레벨 110 상항 처리 빠진것 수정 - 
	{
		AfxMessageBox("Please Check Maxlevel");
		return FALSE;
	}

	// 각 Rate의 범위를 체크한다.
	if (FALSE == util::in_range(0.0f, i_pHappyEv->fEXPRate2, 100.0f))
	{// 2007-10-30 by cmkwon, 세력별 해피아워 이벤트 구현 - 기본값이 모두 0.0f 로 변경 함
		AfxMessageBox("Please Check EXP Rate value.(0.0f ~ 100.0f)");
		return FALSE;
	}
	if(FALSE == util::in_range(0.0f, i_pHappyEv->fSPIRate2, 100.0f))
	{// 2007-10-30 by cmkwon, 세력별 해피아워 이벤트 구현 - 기본값이 모두 0.0f 로 변경 함
		AfxMessageBox("Please Check SPI Rate value(0.0f ~ 100.0f)");
		return FALSE;
	}
	if(FALSE == util::in_range(0.0f, i_pHappyEv->fEXPRepairRate2, 1.0f))
	{
		AfxMessageBox("Please Check EXP Repair Rate value(0.0f ~ 1.0f)");
		return FALSE;
	}
	if(FALSE == util::in_range(0.0f, i_pHappyEv->fDropItemRate2, 100.0f))
	{// 2007-10-30 by cmkwon, 세력별 해피아워 이벤트 구현 - 기본값이 모두 0.0f 로 변경 함
		AfxMessageBox("Please Check Drop Item Rate value(0.0f ~ 100.0f)");
		return FALSE;
	}
	if(FALSE == util::in_range(0.0f, i_pHappyEv->fDropRareItemRate2, 100.0f))
	{// 2007-10-30 by cmkwon, 세력별 해피아워 이벤트 구현 - 기본값이 모두 0.0f 로 변경 함
		AfxMessageBox("Please Check Drop Rare Item Rate value(0.0f ~ 100.0f)");
		return FALSE;
	}
	if(FALSE == util::in_range(0.0f, i_pHappyEv->fWarPointRate2, 100.0f))
	{// 2007-10-30 by cmkwon, 세력별 해피아워 이벤트 구현 - 기본값이 모두 0.0f 로 변경 함// 2007-06-26 by dhjin, 워포인트 이벤트 관련 추가
		AfxMessageBox("Please Check WarPoint Rate value(1.0f ~ 100.0f)");
		return FALSE;
	}
	
	// 2006-08-30 by cmkwon, 시간 설정 체크
	if(i_pHappyEv->atimeStartTime2 >= i_pHappyEv->atimeEndTime2)
	{
		AfxMessageBox("End Time must more then Start Time !!");
		return FALSE;
	}

	for(int i = 0; i < m_mtvectHappyEventList.size(); i++)
	{
		SHAPPY_HOUR_EVENT *pHappyEv = &m_mtvectHappyEventList[i];
		if(pHappyEv->EventUniqueNumber == i_pHappyEv->EventUniqueNumber
			|| pHappyEv->DayOfWeek != i_pHappyEv->DayOfWeek)
		{// 2006-08-30 by cmkwon, 수정된 항목이거나 요일이 같지 않으면 체크가 필요 없다
			continue;
		}

		if(0 != i_pHappyEv->ServerGroupID
			&& 0 != pHappyEv->ServerGroupID 
			&& pHappyEv->ServerGroupID != i_pHappyEv->ServerGroupID)
		{// 2006-08-30 by cmkwon, 모든 서버 항목이 아니면서 체크를 위한 서버군과 같이 않으면 체크 필요 없다
			continue;
		}

		if(pHappyEv->InfluenceType3 != i_pHappyEv->InfluenceType3)
		{	// 2007-10-30 by cmkwon, 세력별 해피아워 이벤트 구현 - 세력이 다르면 체크 필요 없다
			continue;
		}

		pHappyEv->atimeStartTime2.Year	= i_pHappyEv->atimeStartTime2.Year;
		pHappyEv->atimeStartTime2.Month	= i_pHappyEv->atimeStartTime2.Month;
		pHappyEv->atimeStartTime2.Day	= i_pHappyEv->atimeStartTime2.Day;
		pHappyEv->atimeEndTime2.Year	= i_pHappyEv->atimeStartTime2.Year;
		pHappyEv->atimeEndTime2.Month	= i_pHappyEv->atimeStartTime2.Month;
		pHappyEv->atimeEndTime2.Day		= i_pHappyEv->atimeStartTime2.Day;

		if(pHappyEv->atimeStartTime2 == i_pHappyEv->atimeStartTime2)
		{
			AfxMessageBox("Start Time or End Time error !!");
			return FALSE;
		}

		if(pHappyEv->atimeStartTime2 < i_pHappyEv->atimeStartTime2)
		{
			if(pHappyEv->atimeEndTime2 >= i_pHappyEv->atimeStartTime2)
			{
				AfxMessageBox("Start Time or End Time error !!");
				return FALSE;
			}
		}
		else
		{
			if(pHappyEv->atimeStartTime2 <= i_pHappyEv->atimeEndTime2)
			{
				AfxMessageBox("Start Time or End Time error !!");
				return FALSE;
			}
		}
	}

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			ATUM_DATE_TIME *CSCHappyHourEventAdminDlg::GetHappyEvPeriodStartDateTime(BYTE i_byInflTy)
/// \brief		// 2007-10-30 by cmkwon, 세력별 해피아워 이벤트 구현 - CSCHappyHourEventAdminDlg::GetHappyEvPeriodStartDateTime() 함수 추가
/// \author		cmkwon
/// \date		2007-11-01 ~ 2007-11-01
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
ATUM_DATE_TIME *CSCHappyHourEventAdminDlg::GetHappyEvPeriodStartDateTime(BYTE i_byInflTy)
{
	int nArrIdx = GetArrayIndexByInfluenceType(i_byInflTy);
	if(0 > nArrIdx)
	{
		return NULL;
	}
	return &m_arrHappyEvPeriodStart[nArrIdx];
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			ATUM_DATE_TIME *CSCHappyHourEventAdminDlg::GetHappyEvPeriodEndDateTime(BYTE i_byInflTy)
/// \brief		// 2007-10-30 by cmkwon, 세력별 해피아워 이벤트 구현 - CSCHappyHourEventAdminDlg::GetHappyEvPeriodEndDateTime() 함수 추가
/// \author		cmkwon
/// \date		2007-11-01 ~ 2007-11-01
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
ATUM_DATE_TIME *CSCHappyHourEventAdminDlg::GetHappyEvPeriodEndDateTime(BYTE i_byInflTy)
{
	int nArrIdx = GetArrayIndexByInfluenceType(i_byInflTy);
	if(0 > nArrIdx)
	{
		return NULL;
	}

	return &m_arrHappyEvPeriodEND[nArrIdx];
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CSCHappyHourEventAdminDlg::UpdateHappyHourEventPeriod(BYTE i_byInflTy, ATUM_DATE_TIME *i_pATStart, ATUM_DATE_TIME *i_pATEnd)
/// \brief		// 2007-10-30 by cmkwon, 세력별 해피아워 이벤트 구현 - CSCHappyHourEventAdminDlg::UpdateHappyHourEventPeriod() 함수 추가
/// \author		cmkwon
/// \date		2007-11-01 ~ 2007-11-01
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CSCHappyHourEventAdminDlg::UpdateHappyHourEventPeriod(BYTE i_byInflTy, ATUM_DATE_TIME *i_pATStart, ATUM_DATE_TIME *i_pATEnd)
{
// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
//	CString szQuery;
//	UID32_t EvendUID = 0;
//
// 	///////////////////////////////////////////////////////////////////////////////
// 	// 2007-11-01 by cmkwon, 해당 세력의 HappyHourEvnet 기간 데이터가 DB에 존재 하는지 체크
// // 2008-07-02 by cmkwon, MySQL 지원 구현 - 
// // 	szQuery.Format("SELECT UniqueNumber FROM ti_happyHourEvent WITH(NOLOCK) WHERE InfluenceType = %d AND DayOfWeek = %d"
// // 		, i_byInflTy, DAY_OF_WEEK_HAPPYHOUREVENT_PERIOD);
// 	szQuery.Format(QUERY_080702_0028, i_byInflTy, DAY_OF_WEEK_HAPPYHOUREVENT_PERIOD);	// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
// 	SQLBindCol(m_pODBCStmt->m_hstmt, 1, SQL_C_ULONG, &EvendUID, 0, NULL);
// 
// 	BOOL bRet = m_pODBCStmt->ExecuteQuery((LPCSTR)szQuery);
// 	if (!bRet)
// 	{
// 		m_pODBCStmt->FreeStatement();		// clean up
// 		MessageBox("HappyHourEvent period update error !!, 1");
// 		return FALSE;
// 	}
// 	SQLFetch(m_pODBCStmt->m_hstmt);
// 	m_pODBCStmt->FreeStatement();		// cleanup
// 
// 
// 	///////////////////////////////////////////////////////////////////////////////
// 	// 2007-11-01 by cmkwon, 
// 	if(FALSE == IS_VALID_UNIQUE_NUMBER(EvendUID))
// 	{// 2006-08-25 by cmkwon, 추가해야함
// 
// 		// 2007-06-26 by dhjin, 워포인트 이벤트 관련 추가
// 		// 2007-10-30 by cmkwon, 세력별 해피아워 이벤트 구현 - InfluenceType 필드 추가
// 		// 2008-01-24 by cmkwon, S_A: 날짜 관련 로컬리 제이션 처리 버그 수정 - GetDateTimeString() 를 GetSQLDateTimeString() 로 변경 함
// 		// 2008-03-14 by dhjin,  Level별 해피아워 이벤트 구현 - 세력관련 주기 설정은 기본으로 1~CHARACTER_MAX_LEVEL로 설정한다.
// 		// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
// 		//szQuery.Format("INSERT ti_happyhourevent(ServerGroupID, dayofweek, starttime, endtime, exprate, spirate, exprepairrate, dropitemrate, droprareitemrate, WarPointRate, InfluenceType, MinLevel, MaxLevel) values( 0, %d, '%s', '%s', 0, 0, 0, 0, 0, 0, %d, 1, 100)"
// 		//	, DAY_OF_WEEK_HAPPYHOUREVENT_PERIOD, i_pATStart->GetSQLDateTimeString(STRNBUF(SIZE_MAX_SQL_DATETIME_STRING))
// 		//	, i_pATEnd->GetSQLDateTimeString(STRNBUF(SIZE_MAX_SQL_DATETIME_STRING)), i_byInflTy);
// 		szQuery.Format(QUERY_080702_0262
// 			, DAY_OF_WEEK_HAPPYHOUREVENT_PERIOD, i_pATStart->GetSQLDateTimeString(STRNBUF(SIZE_MAX_SQL_DATETIME_STRING))
// 			, i_pATEnd->GetSQLDateTimeString(STRNBUF(SIZE_MAX_SQL_DATETIME_STRING)), i_byInflTy);
// 	}
// 	else
// 	{// 2006-08-25 by cmkwon, Update
// 		// 2008-01-24 by cmkwon, S_A: 날짜 관련 로컬리 제이션 처리 버그 수정 - GetDateTimeString() 를 GetSQLDateTimeString() 로 변경 함
// 		// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
// 		//szQuery.Format("UPDATE ti_happyhourevent SET starttime = '%s', endtime = '%s' where UniqueNumber= %d"
// 		//	, i_pATStart->GetSQLDateTimeString(STRNBUF(SIZE_MAX_SQL_DATETIME_STRING))
// 		//	, i_pATEnd->GetSQLDateTimeString(STRNBUF(SIZE_MAX_SQL_DATETIME_STRING))
// 		//	, EvendUID);
// 		szQuery.Format(QUERY_080702_0263
// 			, i_pATStart->GetSQLDateTimeString(STRNBUF(SIZE_MAX_SQL_DATETIME_STRING))
// 			, i_pATEnd->GetSQLDateTimeString(STRNBUF(SIZE_MAX_SQL_DATETIME_STRING))
// 			, EvendUID);
// 	}
// 
// 	bRet = m_pODBCStmt->ExecuteQuery((LPCSTR)szQuery);
// 	if (!bRet)
// 	{
// 		m_pODBCStmt->FreeStatement();		// cleanup
// 
// 		MessageBox("HappyHourEvent period error !!, 2");
// 		return FALSE;
// 	}
// 	m_pODBCStmt->FreeStatement();	// cleanup

	///////////////////////////////////////////////////////////////////////////////
	// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
	int nDayOfWeek = DAY_OF_WEEK_HAPPYHOUREVENT_PERIOD;
	char tmStartTime[SIZE_MAX_SQL_DATETIME_STRING];		i_pATStart->GetSQLDateTimeString(tmStartTime, SIZE_MAX_SQL_DATETIME_STRING);
	char tmEndTime[SIZE_MAX_SQL_DATETIME_STRING];		i_pATEnd->GetSQLDateTimeString(tmEndTime, SIZE_MAX_SQL_DATETIME_STRING);
	SQLHSTMT hstmt = m_pODBCStmt->GetSTMTHandle();
	SQLINTEGER arrCB2[5] = {SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS};
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &(nDayOfWeek), 0,							&arrCB2[1]);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &(i_byInflTy), 0,						&arrCB2[2]);
	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SQL_DATETIME_STRING, 0, tmStartTime, 0,	&arrCB2[3]);
	SQLBindParameter(hstmt, 4, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SQL_DATETIME_STRING, 0, tmEndTime, 0,		&arrCB2[4]);
	BOOL bRet = m_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_080827_0028));
	if(FALSE == bRet)
	{
 		MessageBox("HappyHourEvent period error !!, 2");
	}
 	m_pODBCStmt->FreeStatement();	// cleanup

	return TRUE;
}



void CSCHappyHourEventAdminDlg::OnBtnUpdatePcbangHappyev() 
{
	// TODO: Add your control notification handler code here

	CSetPCBangHappyEv dlg(&m_PCBangHappyEv, this);
	if(IDOK != dlg.DoModal())
	{
		return;
	}
	m_PCBangHappyEv				= dlg.m_pcBangEv;

//	CString szQuery;
	BOOL bRet;
	if(0 == m_PCBangHappyEv.EventUniqueNumber)
	{// 2006-08-28 by cmkwon, Insert

		// 2008-01-24 by cmkwon, S_A: 날짜 관련 로컬리 제이션 처리 버그 수정 - GetDateTimeString() 를 GetSQLDateTimeString() 로 변경 함
		// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
		//szQuery.Format("INSERT ti_happyhourevent(ServerGroupID, dayofweek, starttime, endtime, exprate, spirate, exprepairrate, dropitemrate, droprareitemrate, WarPointRate, MinLevel, MaxLevel) VALUES( 0, %d, '%s', '%s', %6.4f, %6.4f, %6.4f, %6.4f, %6.4f, %6.4f, 1, 100)"
// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
//		szQuery.Format(QUERY_080702_0264
//			, DAY_OF_WEEK_PCBANG_HAPPYHOUREVENT 
//			, m_PCBangHappyEv.atimeStartTime2.GetSQLDateTimeString(STRNBUF(SIZE_MAX_SQL_DATETIME_STRING))
//			, m_PCBangHappyEv.atimeEndTime2.GetSQLDateTimeString(STRNBUF(SIZE_MAX_SQL_DATETIME_STRING))
//			, m_PCBangHappyEv.fEXPRate2, m_PCBangHappyEv.fSPIRate2, m_PCBangHappyEv.fEXPRepairRate2
//			, m_PCBangHappyEv.fDropItemRate2, m_PCBangHappyEv.fDropRareItemRate2, m_PCBangHappyEv.fWarPointRate2);		// 2007-06-26 by dhjin, 워포인트 이벤트 관련 추가
		int tmWeek = DAY_OF_WEEK_PCBANG_HAPPYHOUREVENT;
		SQLBindParameter(m_pODBCStmt->m_hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &tmWeek, 0, NULL);
		char szATStart[SIZE_MAX_SQL_DATETIME_STRING];
		m_PCBangHappyEv.atimeStartTime2.GetSQLDateTimeString(szATStart, SIZE_MAX_SQL_DATETIME_STRING);	
		SQLBindParameter(m_pODBCStmt->m_hstmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SQL_DATETIME_STRING, 0, szATStart, 0, NULL);
		char szATEnd[SIZE_MAX_SQL_DATETIME_STRING];
		m_PCBangHappyEv.atimeEndTime2.GetSQLDateTimeString(szATEnd, SIZE_MAX_SQL_DATETIME_STRING);	
		SQLBindParameter(m_pODBCStmt->m_hstmt, 3, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SQL_DATETIME_STRING, 0, szATEnd, 0, NULL);
		SQLBindParameter(m_pODBCStmt->m_hstmt, 4, SQL_PARAM_INPUT, SQL_C_FLOAT, SQL_FLOAT, 0, 0, &m_PCBangHappyEv.fEXPRate2, 0, NULL);
		SQLBindParameter(m_pODBCStmt->m_hstmt, 5, SQL_PARAM_INPUT, SQL_C_FLOAT, SQL_FLOAT, 0, 0, &m_PCBangHappyEv.fSPIRate2, 0, NULL);
		SQLBindParameter(m_pODBCStmt->m_hstmt, 6, SQL_PARAM_INPUT, SQL_C_FLOAT, SQL_FLOAT, 0, 0, &m_PCBangHappyEv.fEXPRepairRate2, 0, NULL);
		SQLBindParameter(m_pODBCStmt->m_hstmt, 7, SQL_PARAM_INPUT, SQL_C_FLOAT, SQL_FLOAT, 0, 0, &m_PCBangHappyEv.fDropItemRate2, 0, NULL);
		SQLBindParameter(m_pODBCStmt->m_hstmt, 8, SQL_PARAM_INPUT, SQL_C_FLOAT, SQL_FLOAT, 0, 0, &m_PCBangHappyEv.fDropRareItemRate2, 0, NULL);
		SQLBindParameter(m_pODBCStmt->m_hstmt, 9, SQL_PARAM_INPUT, SQL_C_FLOAT, SQL_FLOAT, 0, 0, &m_PCBangHappyEv.fWarPointRate2, 0, NULL);
		bRet = m_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_080827_0264));

	}
	else
	{// 2006-08-28 by cmkwon, Update
		// 2008-01-24 by cmkwon, S_A: 날짜 관련 로컬리 제이션 처리 버그 수정 - GetDateTimeString() 를 GetSQLDateTimeString() 로 변경 함
		// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
		//szQuery.Format("UPDATE ti_happyhourevent SET starttime= '%s', endtime= '%s', exprate= %6.4f, spirate= %6.4f, exprepairrate= %6.4f, dropitemrate= %6.4f, droprareitemrate= %6.4f, WarPointRate= %6.4f WHERE UniqueNumber= %d"
// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
//		szQuery.Format(QUERY_080702_0265
//			, m_PCBangHappyEv.atimeStartTime2.GetSQLDateTimeString(STRNBUF(SIZE_MAX_SQL_DATETIME_STRING))
//			, m_PCBangHappyEv.atimeEndTime2.GetSQLDateTimeString(STRNBUF(SIZE_MAX_SQL_DATETIME_STRING))
//			, m_PCBangHappyEv.fEXPRate2, m_PCBangHappyEv.fSPIRate2, m_PCBangHappyEv.fEXPRepairRate2
//			, m_PCBangHappyEv.fDropItemRate2, m_PCBangHappyEv.fDropRareItemRate2, m_PCBangHappyEv.fWarPointRate2
//			, m_PCBangHappyEv.EventUniqueNumber);			// 2007-06-26 by dhjin, 워포인트 이벤트 관련 추가
		char szATStart[SIZE_MAX_SQL_DATETIME_STRING];
		m_PCBangHappyEv.atimeStartTime2.GetSQLDateTimeString(szATStart, SIZE_MAX_SQL_DATETIME_STRING);	
		SQLBindParameter(m_pODBCStmt->m_hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SQL_DATETIME_STRING, 0, szATStart, 0, NULL);
		char szATEnd[SIZE_MAX_SQL_DATETIME_STRING];
		m_PCBangHappyEv.atimeEndTime2.GetSQLDateTimeString(szATEnd, SIZE_MAX_SQL_DATETIME_STRING);	
		SQLBindParameter(m_pODBCStmt->m_hstmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SQL_DATETIME_STRING, 0, szATEnd, 0, NULL);
		SQLBindParameter(m_pODBCStmt->m_hstmt, 3, SQL_PARAM_INPUT, SQL_C_FLOAT, SQL_FLOAT, 0, 0, &m_PCBangHappyEv.fEXPRate2, 0, NULL);
		SQLBindParameter(m_pODBCStmt->m_hstmt, 4, SQL_PARAM_INPUT, SQL_C_FLOAT, SQL_FLOAT, 0, 0, &m_PCBangHappyEv.fSPIRate2, 0, NULL);
		SQLBindParameter(m_pODBCStmt->m_hstmt, 5, SQL_PARAM_INPUT, SQL_C_FLOAT, SQL_FLOAT, 0, 0, &m_PCBangHappyEv.fEXPRepairRate2, 0, NULL);
		SQLBindParameter(m_pODBCStmt->m_hstmt, 6, SQL_PARAM_INPUT, SQL_C_FLOAT, SQL_FLOAT, 0, 0, &m_PCBangHappyEv.fDropItemRate2, 0, NULL);
		SQLBindParameter(m_pODBCStmt->m_hstmt, 7, SQL_PARAM_INPUT, SQL_C_FLOAT, SQL_FLOAT, 0, 0, &m_PCBangHappyEv.fDropRareItemRate2, 0, NULL);
		SQLBindParameter(m_pODBCStmt->m_hstmt, 8, SQL_PARAM_INPUT, SQL_C_FLOAT, SQL_FLOAT, 0, 0, &m_PCBangHappyEv.fWarPointRate2, 0, NULL);
		SQLBindParameter(m_pODBCStmt->m_hstmt, 9, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &m_PCBangHappyEv.EventUniqueNumber, 0, NULL);
		bRet = m_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_080827_0265));
	}

//	BOOL bRet = m_pODBCStmt->ExecuteQuery((LPCSTR)szQuery);
	if (!bRet)
	{
		m_pODBCStmt->FreeStatement();		// cleanup

		MessageBox("PCBang HappyHourEvent update error !!");
		return;
	}
	m_pODBCStmt->FreeStatement();	// cleanup

	GetHappyHourEventDetailInfo();
}

void CSCHappyHourEventAdminDlg::OnBtnReloadHappyEv() 
{
	// TODO: Add your control notification handler code here

	// 2007-11-19 by cmkwon, 진정시스템 업데이트 - 아래와 같이 함수로 처리함
	//m_pMainDlg->m_pAdminPreSocket->WriteMessageType(T_PA_ADMIN_RELOAD_HAPPYEV);
	if(FALSE == m_pMainDlg->SendMsgTypeToPreServer(T_PA_ADMIN_RELOAD_HAPPYEV))
	{
		MessageBox("diconnected from preserver !!");
		return;
	}

	// 2007-11-01 by cmkwon, 응답 메시지 박스 추가
	Sleep(1000);
	MessageBox("All ServerGroup reload HappyHourEvent !!");
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CSCHappyHourEventAdminDlg::OnBtnReloadPCbangList()
/// \brief		
/// \author		dhjin
/// \date		2007-01-22 ~ 2007-01-22
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CSCHappyHourEventAdminDlg::OnBtnReloadPCbangList()
{
	// 2007-11-19 by cmkwon, 진정시스템 업데이트 - 아래와 같이 함수로 처리함
	//m_pMainDlg->m_pAdminPreSocket->WriteMessageType(T_PA_ADMIN_UPDATE_PCBANGLIST);	
	if(!m_pMainDlg->SendMsgTypeToPreServer(T_PA_ADMIN_UPDATE_PCBANGLIST))
	{
		MessageBox("disconnected from PreServer !!");
		return;
	}
	MessageBox("PCBang Reload Success !!");
}


void CSCHappyHourEventAdminDlg::OnBtnUpdatePeriodNormal() 
{
	// TODO: Add your control notification handler code here
	UpdateData();

	ATUM_DATE_TIME *pATStart	= GetHappyEvPeriodStartDateTime(INFLUENCE_TYPE_NORMAL);
	ATUM_DATE_TIME *pATEnd		= GetHappyEvPeriodEndDateTime(INFLUENCE_TYPE_NORMAL);

	pATStart->SetDateTime(m_ctlOleDateNormalStart.GetYear(), m_ctlOleDateNormalStart.GetMonth(), m_ctlOleDateNormalStart.GetDay()
		, m_ctlOleTimeNormalStart.GetHour(), m_ctlOleTimeNormalStart.GetMinute(), 0);
	pATEnd->SetDateTime(m_ctlOleDateNormalEnd.GetYear(), m_ctlOleDateNormalEnd.GetMonth(), m_ctlOleDateNormalEnd.GetDay()
		, m_ctlOleTimeNormalEnd.GetHour(), m_ctlOleTimeNormalEnd.GetMinute(), 0);

	// 2007-11-01 by cmkwon, DB 에 업데이트 한다.
	this->UpdateHappyHourEventPeriod(INFLUENCE_TYPE_NORMAL, pATStart, pATEnd);

	// 2007-11-01 by cmkwon, 모든 이벤트 정보를 다시 로딩한다.
	GetHappyHourEventDetailInfo();	
}

void CSCHappyHourEventAdminDlg::OnBtnUpdatePeriodVcn() 
{
	// TODO: Add your control notification handler code here
	
	UpdateData();

	ATUM_DATE_TIME *pATStart	= GetHappyEvPeriodStartDateTime(INFLUENCE_TYPE_VCN);
	ATUM_DATE_TIME *pATEnd		= GetHappyEvPeriodEndDateTime(INFLUENCE_TYPE_VCN);

	pATStart->SetDateTime(m_ctlOleDateVCNStart.GetYear(), m_ctlOleDateVCNStart.GetMonth(), m_ctlOleDateVCNStart.GetDay()
		, m_ctlOleTimeVCNStart.GetHour(), m_ctlOleTimeVCNStart.GetMinute(), 0);
	pATEnd->SetDateTime(m_ctlOleDateVCNEnd.GetYear(), m_ctlOleDateVCNEnd.GetMonth(), m_ctlOleDateVCNEnd.GetDay()
		, m_ctlOleTimeVCNEnd.GetHour(), m_ctlOleTimeVCNEnd.GetMinute(), 0);

	// 2007-11-01 by cmkwon, DB 에 업데이트 한다.
	this->UpdateHappyHourEventPeriod(INFLUENCE_TYPE_VCN, pATStart, pATEnd);

	// 2007-11-01 by cmkwon, 모든 이벤트 정보를 다시 로딩한다.
	GetHappyHourEventDetailInfo();	
}

void CSCHappyHourEventAdminDlg::OnBtnUpdatePeriodAni() 
{
	// TODO: Add your control notification handler code here
	
	UpdateData();

	ATUM_DATE_TIME *pATStart	= GetHappyEvPeriodStartDateTime(INFLUENCE_TYPE_ANI);
	ATUM_DATE_TIME *pATEnd		= GetHappyEvPeriodEndDateTime(INFLUENCE_TYPE_ANI);

	pATStart->SetDateTime(m_ctlOleDateANIStart.GetYear(), m_ctlOleDateANIStart.GetMonth(), m_ctlOleDateANIStart.GetDay()
		, m_ctlOleTimeANIStart.GetHour(), m_ctlOleTimeANIStart.GetMinute(), 0);
	pATEnd->SetDateTime(m_ctlOleDateANIEnd.GetYear(), m_ctlOleDateANIEnd.GetMonth(), m_ctlOleDateANIEnd.GetDay()
		, m_ctlOleTimeANIEnd.GetHour(), m_ctlOleTimeANIEnd.GetMinute(), 0);

	// 2007-11-01 by cmkwon, DB 에 업데이트 한다.
	this->UpdateHappyHourEventPeriod(INFLUENCE_TYPE_ANI, pATStart, pATEnd);

	// 2007-11-01 by cmkwon, 모든 이벤트 정보를 다시 로딩한다.
	GetHappyHourEventDetailInfo();	
}
