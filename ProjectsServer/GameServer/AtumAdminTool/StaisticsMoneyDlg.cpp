// StaisticsMoneyDlg.cpp : implementation file
//

#include "stdafx.h"
#include "atumadmintool.h"
#include "StaisticsMoneyDlg.h"

#include <AFXWIN.H>
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStaisticsMoneyDlg dialog
#define COLUMN_STRING "Count\tTime\tSPI Gap\tSPI Current\tWP Gap\tWP Current\tS-Orb Gap\tS-Orb Current\tQ-Orb Gap\tQ-Orb Current\tC-Orb Gap\tC-Orb Current\tG-Orb Gap\tG-Orb Current\tU-Orb Gap\tU-Orb Current\n"
char g_SaveStringValue[3000000] = {NULL,};
char g_SaveString[3000000] = {NULL,};

CStaisticsMoneyDlg::CStaisticsMoneyDlg(CAtumAdminToolDlg *i_pMainDlg, CWnd* pParent /*=NULL*/)
	: CDialog(CStaisticsMoneyDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CStaisticsMoneyDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	
	m_OleDateStart	= (CTime::GetCurrentTime() - CTimeSpan(1, 0 , 0, 0));
	m_OleDateEnd	= CTime::GetCurrentTime();
	m_OleTimeStart	= CTime::GetCurrentTime();
	m_OleTimeEnd	= CTime::GetCurrentTime();
	m_pMainDlg = i_pMainDlg;
	m_pODBCStmt = new CODBCStatement;
	m_pCurrentCD = NULL;
	// start 2012-02-27 by jhseol, 화폐통계툴 체크박스 초기화
	m_bCheckGapTimeYear = FALSE;
	m_bCheckGapTimeMonth = FALSE;
	m_bCheckGapTimeDay = FALSE;
	m_bCheckGapTimeHour = FALSE;
	m_bCheckGapTimeMinute = FALSE;
	// end 2012-02-27 by jhseol, 화폐통계툴 체크박스 초기화
	vectSStaisticsMoneyInfo_Search.clear();
	vectSStaisticsMoneyInfo_Result.clear();
	m_nIndex = 0;
	util::zero(&m_GapTime, sizeof(m_GapTime));
	util::zero(&StartTime, sizeof(StartTime));
	util::zero(&EndTime, sizeof(EndTime));
	m_pTab_show = NULL;
}

CStaisticsMoneyDlg::~CStaisticsMoneyDlg()
{
	util::del(m_pODBCStmt);
}


void CStaisticsMoneyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStaisticsMoneyDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	DDX_DateTimeCtrl(pDX, IDC_DATEPICKER_START, m_OleDateStart);
	DDX_DateTimeCtrl(pDX, IDC_DATEPICKER_END,	m_OleDateEnd);
	DDX_DateTimeCtrl(pDX, IDC_TIMEPICKER_START, m_OleTimeStart);
	DDX_DateTimeCtrl(pDX, IDC_TIMEPICKER_END,	m_OleTimeEnd);

	DDX_Control(pDX, IDC_COMBO_GAP_TIME_YEAR, m_ComboGapTimeYear);
	DDX_Control(pDX, IDC_COMBO_GAP_TIME_MONTH, m_ComboGapTimeMonth);
	DDX_Control(pDX, IDC_COMBO_GAP_TIME_DAY, m_ComboGapTimeDay);	
	DDX_Control(pDX, IDC_COMBO_GAP_TIME_HOUR, m_ComboGapTimeHour);	
	DDX_Control(pDX, IDC_COMBO_GAP_TIME_MINUTE, m_ComboGapTimeMinute);
	DDX_Control(pDX, IDC_PROGRESS1, m_Progress);
	DDX_Control(pDX, IDC_GRAPH_TAB, m_GraphTab);

	DDX_Check(pDX, IDC_CHECK_GAP_TIME_YEAR, m_bCheckGapTimeYear);
	DDX_Check(pDX, IDC_CHECK_GAP_TIME_MONTH, m_bCheckGapTimeMonth);
	DDX_Check(pDX, IDC_CHECK_GAP_TIME_DAY, m_bCheckGapTimeDay);
	DDX_Check(pDX, IDC_CHECK_GAP_TIME_HOUR, m_bCheckGapTimeHour);
	DDX_Check(pDX, IDC_CHECK_GAP_TIME_MINUTE, m_bCheckGapTimeMinute);


	//}}AFX_DATA_MAP
	DDX_GridControl(pDX, IDC_GRID_STAISTICSMONEY, m_GridStaisticsMoney);
}


BEGIN_MESSAGE_MAP(CStaisticsMoneyDlg, CDialog)
	//{{AFX_MSG_MAP(CStaisticsMoneyDlg)
		// NOTE: the ClassWizard will add message map macros here
	ON_BN_CLICKED(IDC_CHECK_GAP_TIME_YEAR, OnCheckGapTimeYear)
	ON_BN_CLICKED(IDC_CHECK_GAP_TIME_MONTH, OnCheckGapTimeMonth)
	ON_BN_CLICKED(IDC_CHECK_GAP_TIME_DAY, OnCheckGapTimeDay)
	ON_BN_CLICKED(IDC_CHECK_GAP_TIME_HOUR, OnCheckGapTimeHour)
	ON_BN_CLICKED(IDC_CHECK_GAP_TIME_MINUTE, OnCheckGapTimeMinute)

	ON_BN_CLICKED(IDC_BUTTON_SEARCH, OnButtonSearch)
	ON_BN_CLICKED(IDC_BUTTON_RESET, OnButtonReset)
	ON_BN_CLICKED(IDC_BUTTON_SAVE_FILE, OnButtonSaveFile)

	ON_NOTIFY(TCN_SELCHANGE, IDC_GRAPH_TAB, OnGraphChange)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStaisticsMoneyDlg message handlers


BOOL CStaisticsMoneyDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// start 2012-02-27 by jhseol, 화폐통계툴 결과창(표) 초기화 및 필드네임 추가
	m_GridStaisticsMoney.InitGrid(NUM_COLUMNS_STAISTICSMONEY, g_arrStaisicsMoneyColumnNames);
	m_GridStaisticsMoney.ArrangeGrid();		//표 정렬
	// end 2012-02-27 by jhseol, 화폐통계툴 결과창 초기화 및 필드네임 추가
	
	// start 2012-02-27 by jhseol, 화폐통계툴 체크박스 초기화
	m_bCheckGapTimeYear = FALSE;
	m_bCheckGapTimeMonth = FALSE;
	m_bCheckGapTimeDay = FALSE;
	m_bCheckGapTimeHour = FALSE;
	m_bCheckGapTimeMinute = FALSE;
	// end 2012-02-27 by jhseol, 화폐통계툴 체크박스 초기화
	
	CheckBoxCheck();	// start end 2012-02-27 by jhseol, 화폐통계툴 체크박스 활성화 함수
	
	// start 2012-02-27 by jhseol, 화폐통계툴 콤보박스 아이템 추가부분
	char szTemp[512]={0x00,};
	int i;
	for(i = 0 ; i < SIZE_MAX_GAPTIME_YEAR ; i++)
	{
		m_ComboGapTimeYear.AddString(itoa(i+1, szTemp, 10));
	}
	for(i = 0 ; i < SIZE_MAX_GAPTIME_MONTH ; i++)
	{
		m_ComboGapTimeMonth.AddString(itoa(i+1, szTemp, 10));
	}
	for(i = 0 ; i < SIZE_MAX_GAPTIME_DAY ; i++)
	{
		m_ComboGapTimeDay.AddString(itoa(i+1, szTemp, 10));
	}
	for(i = 0 ; i < SIZE_MAX_GAPTIME_HOUR ; i++)
	{
		m_ComboGapTimeHour.AddString(itoa(i+1, szTemp, 10));
	}
	for(i = 0 ; i < SIZE_MAX_GAPTIME_MINUTE ; i++)
	{
		m_ComboGapTimeMinute.AddString(itoa((i+1)*10, szTemp, 10));
	}
	m_ComboGapTimeYear.SetCurSel(0);
	m_ComboGapTimeMonth.SetCurSel(0);
	m_ComboGapTimeDay.SetCurSel(0);
	m_ComboGapTimeHour.SetCurSel(0);
	m_ComboGapTimeMinute.SetCurSel(0);
	// end 2012-02-27 by jhseol, 화폐통계툴 콤보박스 아이템 추가부분

	m_GraphTab.InsertItem(0,"SPI Graph");
	m_GraphTab.InsertItem(1,"WP Graph");
	m_GraphTab.InsertItem(2,"S-Orb Graph");
	m_GraphTab.InsertItem(3,"Q-Orb Graph");
	m_GraphTab.InsertItem(4,"C-Orb Graph");
	m_GraphTab.InsertItem(5,"G-Orb Graph");
	m_GraphTab.InsertItem(6,"U-Orb Graph");
	GetDlgItem(IDC_GRAPH_TAB)->EnableWindow(FALSE);
	
	CRect graphRect;
	m_GraphTab.GetClientRect(&graphRect);

	m_pTab_show = NULL;

	// start 2012-02-27 by jhseol, 화폐통계툴 DB연결부분
	if (!m_pODBCStmt->Init(m_pMainDlg->m_pServerInfo4Admin->DBIP, m_pMainDlg->m_pServerInfo4Admin->DBPort, m_pMainDlg->m_pServerInfo4Admin->DBName,
		m_pMainDlg->m_pServerInfo4Admin->DBUID, m_pMainDlg->m_pServerInfo4Admin->DBPWD, GetSafeHwnd()))
	{
		MessageBox(STRERR_S_SCADMINTOOL_0013);
		EndDialog(-1);
		return FALSE;
	}
	// end 2012-02-27 by jhseol, 화폐통계툴 DB연결부분

	return TRUE;
}



void CStaisticsMoneyDlg::OnButtonSearch()
{
	UpdateData();	//컨트롤러의 최신값을 읽어오게 하는 녀석!!! 이놈아!!!
	m_GridStaisticsMoney.InitGrid(NUM_COLUMNS_STAISTICSMONEY, g_arrStaisicsMoneyColumnNames);
	m_GridStaisticsMoney.ArrangeGrid();
	m_pODBCStmt->FreeStatement();
	vectSStaisticsMoneyInfo_Search.clear();
	vectSStaisticsMoneyInfo_Result.clear();

	// start 2012-02-27 by jhseol, 화폐통계툴 DB 파라미터 설정
	ATUM_DATE_TIME tempStartTime;
	util::zero(&tempStartTime,sizeof(tempStartTime));
	util::zero(&StartTime, sizeof(StartTime));
	util::zero(&EndTime, sizeof(EndTime));
	
	tempStartTime.Year		= m_OleDateStart.GetYear();
	tempStartTime.Month		= m_OleDateStart.GetMonth();
	tempStartTime.Day		= m_OleDateStart.GetDay();
	tempStartTime.Hour		= m_OleTimeStart.GetHour();
	tempStartTime.Minute	= m_OleTimeStart.GetMinute();
	tempStartTime.Second	= m_OleTimeStart.GetSecond();
	//tempStartTime.AddDateTime(0,0,0,0,-10,0);
	
	StartTime.year		= tempStartTime.Year;
	StartTime.month		= tempStartTime.Month;
	StartTime.day		= tempStartTime.Day;
	StartTime.hour		= tempStartTime.Hour;
	StartTime.minute	= tempStartTime.Minute;
	StartTime.second	= tempStartTime.Second;
	
	EndTime.year		= m_OleDateEnd.GetYear();
	EndTime.month		= m_OleDateEnd.GetMonth();
	EndTime.day			= m_OleDateEnd.GetDay();
	EndTime.hour		= m_OleTimeEnd.GetHour();
	EndTime.minute		= m_OleTimeEnd.GetMinute();
	EndTime.second		= m_OleTimeEnd.GetSecond();
	SQLINTEGER arrCB2[3] = {SQL_NTS,SQL_NTS,SQL_NTS};
	SQLBindParameter(m_pODBCStmt->m_hstmt, 1, SQL_PARAM_INPUT, SQL_C_TYPE_TIMESTAMP, SQL_TYPE_TIMESTAMP, 23, 0, &StartTime, 0,			&arrCB2[1]);
	SQLBindParameter(m_pODBCStmt->m_hstmt, 2, SQL_PARAM_INPUT, SQL_C_TYPE_TIMESTAMP, SQL_TYPE_TIMESTAMP, 23, 0, &EndTime, 0,			&arrCB2[2]);
	// end 2012-02-27 by jhseol, 화폐통계툴 DB 파라미터 설정

	// start 2012-02-27 by jhseol, 화폐통계툴 DB 프로시저 실행
	//------------------------------------------------------------------------------------
	//----- USE [atum2_db_1]
	//----- GO
	//----- / ****** Object:  StoredProcedure [dbo].[atum_log_Search_StatisticsMoney]    Script Date: 03/06/2012 08:49:44 ****** /
	//----- SET ANSI_NULLS ON
	//----- GO
	//----- SET QUOTED_IDENTIFIER ON
	//----- GO
	//----- ALTER PROCEDURE [dbo].[atum_log_Search_StatisticsMoney]
	//----- 	@StartTime	datetime2,
	//----- 	@EndTime	datetime2
	//----- -- // 2009-02-17 by cmkwon, MARK for Yedang					-- // 2008-09-04 by cmkwon, 예당 보안관련 숨긴 계정 적용 하기(SQL Server 2005이상만 지원하는 기능) - 
	//----- AS
	//----- BEGIN
	//----- SELECT [Function],[Time],[DeltaSPI],[DeltaWarPoint],[DeltaSystemOrb],[DeltaQuasarOrb],[DeltaClusterOrb],[DeltaGalaxyOrb],[DeltaUniverseOrb]
	//----- FROM [atum2_db_1].[dbo].[atum_log_statistics_money]
	//----- WHERE [Time] >= @StartTime AND [Time] < @EndTime
	//----- END
	//-------------------------------------------------------------------------------------
	BOOL bRet = m_pODBCStmt->ExecuteQuery((char*)PROCEDURE_120227_0001);
	if(FALSE == bRet)
	{
		m_pODBCStmt->ProcessLogMessagesForStmt(PROCEDURE_120227_0001);
		m_pODBCStmt->FreeStatement();
	}// end 2012-02-27 by jhseol, 화폐통계툴 DB 프로시저 실행

	// start 2012-02-27 by jhseol, 화폐통계툴 DB 변수 바인드
	int DBBind_Function;
	SQL_TIMESTAMP_STRUCT DBBind_Time;
	INT64 DBBind_SPI = 0, DBBind_WP = 0, DBBind_SOrb = 0, DBBind_QOrb = 0, DBBind_COrb = 0, DBBind_GOrb = 0, DBBind_UOrb = 0;
	SQLINTEGER cb[9];
	SQLBindCol(m_pODBCStmt->m_hstmt, 1, SQL_C_ULONG,		&DBBind_Function,	1,	&cb[0]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 2, SQL_C_TIMESTAMP,	&DBBind_Time,		0,	&cb[1]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 3, SQL_C_SBIGINT,		&DBBind_SPI,		0,	&cb[2]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 4, SQL_C_SBIGINT,		&DBBind_WP,			0,	&cb[3]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 5, SQL_C_SBIGINT,		&DBBind_SOrb,		0,	&cb[4]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 6, SQL_C_SBIGINT,		&DBBind_QOrb,		0,	&cb[5]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 7, SQL_C_SBIGINT,		&DBBind_COrb,		0,	&cb[6]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 8, SQL_C_SBIGINT,		&DBBind_GOrb,		0,	&cb[7]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 9, SQL_C_SBIGINT,		&DBBind_UOrb,		0,	&cb[8]);
	// end 2012-02-27 by jhseol, 화폐통계툴 DB 변수 바인드

	// start 2012-02-27 by jhseol, 화폐통계툴 DB 검색결과 벡터리스트 추가
	vectSStaisticsMoneyInfo_Search.clear();
	SSTAISTICSMONEY_INFO tempItem;
	util::zero(&tempItem, sizeof(tempItem));
	int indexNumber = 1;
	SQLRETURN sqlReturn;
	while ((sqlReturn = SQLFetch(m_pODBCStmt->m_hstmt)) != SQL_NO_DATA)
	{
		if (sqlReturn != SQL_SUCCESS)
		{
			m_pODBCStmt->ProcessLogMessagesForStmt((UCHAR*)"{call dbo.atum_log_Search_StatisticsMoney(?,?)}");
		}
		tempItem.Count			= indexNumber;
		tempItem.Function		= DBBind_Function;
		tempItem.Time			= DBBind_Time;
		tempItem.SPICurrent		= DBBind_SPI;
		tempItem.WPCurrent		= DBBind_WP;
		tempItem.SOrbCurrent	= DBBind_SOrb;
		tempItem.QOrbCurrent	= DBBind_QOrb;
		tempItem.COrbCurrent	= DBBind_COrb;
		tempItem.GOrbCurrent	= DBBind_GOrb;
		tempItem.UOrbCurrent	= DBBind_UOrb;
		tempItem.Sync			= FALSE;
		//m_GridStaisticsMoney.InsertGridItemStaisticsMoney(&tempItem);
		vectSStaisticsMoneyInfo_Search.push_back(tempItem);
		util::zero(&tempItem, sizeof(tempItem));
		indexNumber++;
		//if(indexNumber > 100) break;
	}
	DataArrangement();			// start end 2012-02-27 by jhseol, 화폐통계툴 DB 검색결과 리스트 정리 함수
	// end 2012-02-27 by jhseol, 화폐통계툴 DB 검색결과 벡터리스트 추가

	m_GridStaisticsMoney.ArrangeGrid();

	// start 2012-02-27 by jhseol, 화폐통계툴 그래프 배경
	m_pCurrentCD = GetDC();
	if(m_pCurrentCD != NULL)
	{
		m_pCurrentCD->Rectangle(GRAPH_BACKGROUND_POS_X_START, GRAPH_BACKGROUND_POS_Y_START, GRAPH_BACKGROUND_POS_X_END, GRAPH_BACKGROUND_POS_Y_END);
		ReleaseDC(m_pCurrentCD);
	}
	m_GraphTab.SetCurSel(0);
	DrawGraph(0);
	// end 2012-02-27 by jhseol, 화폐통계툴 그래프 배경

	GetDlgItem(IDC_GRAPH_TAB)->EnableWindow(TRUE);


}


void CStaisticsMoneyDlg::DataArrangement()			// start end 2012-02-27 by jhseol, 화폐통계툴 DB 검색결과 리스트 정리 함수
{
	vector<SSTAISTICSMONEY_INFO> temp_vectSStaisticsMoneyInfo_Plus;			// start end 2012-02-27 by jhseol, 화폐통계툴 DB 검색결과 중 증감 값 리스트
	vector<SSTAISTICSMONEY_INFO> temp_vectSStaisticsMoneyInfo_Minus;		// start end 2012-02-27 by jhseol, 화폐통계툴 DB 검색결과 중 차감 값 리스트
	vector<SSTAISTICSMONEY_INFO> temp_vectSStaisticsMoneyInfo_Plus_Sum_Minus;	// start end 2012-02-27 by jhseol, 화폐통계툴 DB 검색결과 중 같은 시간의 증감과 차감값의 합
	vector<SSTAISTICSMONEY_INFO> temp_vectSStaisticsMoneyInfo_Time;			// start end 2012-02-27 by jhseol, 화폐통계툴 DB 검색결과를 검색 시간별 통계 리스트
	temp_vectSStaisticsMoneyInfo_Plus.clear();
	temp_vectSStaisticsMoneyInfo_Minus.clear();
	temp_vectSStaisticsMoneyInfo_Plus_Sum_Minus.clear();
	temp_vectSStaisticsMoneyInfo_Time.clear();

	// start 2012-02-27 by jhseol, 화폐통계툴 DB 검색결과 증감 과 차감의 분류
	for(m_nIndex = 0; m_nIndex < vectSStaisticsMoneyInfo_Search.size(); m_nIndex++)
	{
		switch(vectSStaisticsMoneyInfo_Search[m_nIndex].Function)
		{
		case 0:
			temp_vectSStaisticsMoneyInfo_Plus.push_back(vectSStaisticsMoneyInfo_Search[m_nIndex]);
			break;
		case 1:
			temp_vectSStaisticsMoneyInfo_Minus.push_back(vectSStaisticsMoneyInfo_Search[m_nIndex]);
			break;
		}
	}// end 2012-02-27 by jhseol, 화폐통계툴 DB 검색결과 증감 과 차감의 분류

	// start 2012-02-27 by jhseol, 화폐통계툴 분류된 값을 같은 시간끼리 합산
	int i = 0, j = 0;
	int tempBigVectorSize=0, tempSmallVectorSize=0;
	BOOL syncCheck = FALSE;
	SSTAISTICSMONEY_INFO tempItem;
	util::zero(&tempItem, sizeof(tempItem));
	if(temp_vectSStaisticsMoneyInfo_Plus.size() >= temp_vectSStaisticsMoneyInfo_Minus.size())
	{
		tempBigVectorSize = temp_vectSStaisticsMoneyInfo_Plus.size();
		tempSmallVectorSize = temp_vectSStaisticsMoneyInfo_Minus.size();
		syncCheck = TRUE;
	}
	else
	{
		tempBigVectorSize = temp_vectSStaisticsMoneyInfo_Minus.size();
		tempSmallVectorSize = temp_vectSStaisticsMoneyInfo_Plus.size();
		syncCheck = FALSE;
	}
	for (i = 0; i < tempSmallVectorSize; i++)
	{
		tempItem.Time			= temp_vectSStaisticsMoneyInfo_Plus[i].Time;
		tempItem.SPICurrent		= temp_vectSStaisticsMoneyInfo_Plus[i].SPICurrent	+(temp_vectSStaisticsMoneyInfo_Minus[i].SPICurrent*-1);
		tempItem.WPCurrent		= temp_vectSStaisticsMoneyInfo_Plus[i].WPCurrent	+(temp_vectSStaisticsMoneyInfo_Minus[i].WPCurrent*-1);
		tempItem.SOrbCurrent	= temp_vectSStaisticsMoneyInfo_Plus[i].SOrbCurrent	+(temp_vectSStaisticsMoneyInfo_Minus[i].SOrbCurrent*-1);
		tempItem.QOrbCurrent	= temp_vectSStaisticsMoneyInfo_Plus[i].QOrbCurrent	+(temp_vectSStaisticsMoneyInfo_Minus[i].QOrbCurrent*-1);
		tempItem.COrbCurrent	= temp_vectSStaisticsMoneyInfo_Plus[i].COrbCurrent	+(temp_vectSStaisticsMoneyInfo_Minus[i].COrbCurrent*-1);
		tempItem.GOrbCurrent	= temp_vectSStaisticsMoneyInfo_Plus[i].GOrbCurrent	+(temp_vectSStaisticsMoneyInfo_Minus[i].GOrbCurrent*-1);
		tempItem.UOrbCurrent	= temp_vectSStaisticsMoneyInfo_Plus[i].UOrbCurrent	+(temp_vectSStaisticsMoneyInfo_Minus[i].UOrbCurrent*-1);
		tempItem.Sync			= TRUE;
		temp_vectSStaisticsMoneyInfo_Plus_Sum_Minus.push_back(tempItem);
	}// end 2012-02-27 by jhseol, 화폐통계툴 분류된 값을 같은 시간끼리 합산

/*	// start 2012-02-27 by jhseol, 화폐통계툴 싱크가 맞지 않는 아이템을 걸러내는 루틴. 전체를 검색 하기에 기능 활성화 시 엄청 느려짐.
	for(i = 0; i < tempBigVectorSize; i++)
	{
		for(j = 0; j < tempSmallVectorSize; j++)
		{
			if(syncCheck)
			{
				if(	temp_vectSStaisticsMoneyInfo_Plus[i].Time.Year		== temp_vectSStaisticsMoneyInfo_Minus[j].Time.Year &&
					temp_vectSStaisticsMoneyInfo_Plus[i].Time.Month	== temp_vectSStaisticsMoneyInfo_Minus[j].Time.Month &&
					temp_vectSStaisticsMoneyInfo_Plus[i].Time.Day		== temp_vectSStaisticsMoneyInfo_Minus[j].Time.Day &&
					temp_vectSStaisticsMoneyInfo_Plus[i].Time.Hour		== temp_vectSStaisticsMoneyInfo_Minus[j].Time.Hour &&
					temp_vectSStaisticsMoneyInfo_Plus[i].Time.Minute	== temp_vectSStaisticsMoneyInfo_Minus[j].Time.Minute )
				{
					tempItem.Time			= temp_vectSStaisticsMoneyInfo_Plus[i].Time;
					tempItem.SPICurrent		= temp_vectSStaisticsMoneyInfo_Plus[i].SPICurrent	+(temp_vectSStaisticsMoneyInfo_Minus[j].SPICurrent*-1);
					tempItem.WPCurrent		= temp_vectSStaisticsMoneyInfo_Plus[i].WPCurrent	+(temp_vectSStaisticsMoneyInfo_Minus[j].WPCurrent*-1);
					tempItem.SOrbCurrent	= temp_vectSStaisticsMoneyInfo_Plus[i].SOrbCurrent	+(temp_vectSStaisticsMoneyInfo_Minus[j].SOrbCurrent*-1);
					tempItem.QOrbCurrent	= temp_vectSStaisticsMoneyInfo_Plus[i].QOrbCurrent	+(temp_vectSStaisticsMoneyInfo_Minus[j].QOrbCurrent*-1);
					tempItem.COrbCurrent	= temp_vectSStaisticsMoneyInfo_Plus[i].COrbCurrent	+(temp_vectSStaisticsMoneyInfo_Minus[j].COrbCurrent*-1);
					tempItem.GOrbCurrent	= temp_vectSStaisticsMoneyInfo_Plus[i].GOrbCurrent	+(temp_vectSStaisticsMoneyInfo_Minus[j].GOrbCurrent*-1);
					tempItem.UOrbCurrent	= temp_vectSStaisticsMoneyInfo_Plus[i].UOrbCurrent	+(temp_vectSStaisticsMoneyInfo_Minus[j].UOrbCurrent*-1);
					tempItem.Sync			= TRUE;
					temp_vectSStaisticsMoneyInfo_Plus_Sum_Minus.push_back(tempItem);
				}
			}
			else
			{
				if(	temp_vectSStaisticsMoneyInfo_Minus[i].Time.Year	== temp_vectSStaisticsMoneyInfo_Plus[j].Time.Year &&
					temp_vectSStaisticsMoneyInfo_Minus[i].Time.Month	== temp_vectSStaisticsMoneyInfo_Plus[j].Time.Month &&
					temp_vectSStaisticsMoneyInfo_Minus[i].Time.Day		== temp_vectSStaisticsMoneyInfo_Plus[j].Time.Day &&
					temp_vectSStaisticsMoneyInfo_Minus[i].Time.Hour	== temp_vectSStaisticsMoneyInfo_Plus[j].Time.Hour &&
					temp_vectSStaisticsMoneyInfo_Minus[i].Time.Minute	== temp_vectSStaisticsMoneyInfo_Plus[j].Time.Minute )
				{
					tempItem.Time			= temp_vectSStaisticsMoneyInfo_Plus[j].Time;
					tempItem.SPICurrent		= temp_vectSStaisticsMoneyInfo_Plus[j].SPICurrent	+(temp_vectSStaisticsMoneyInfo_Minus[i].SPICurrent*-1);
					tempItem.WPCurrent		= temp_vectSStaisticsMoneyInfo_Plus[j].WPCurrent	+(temp_vectSStaisticsMoneyInfo_Minus[i].WPCurrent*-1);
					tempItem.SOrbCurrent	= temp_vectSStaisticsMoneyInfo_Plus[j].SOrbCurrent	+(temp_vectSStaisticsMoneyInfo_Minus[i].SOrbCurrent*-1);
					tempItem.QOrbCurrent	= temp_vectSStaisticsMoneyInfo_Plus[j].QOrbCurrent	+(temp_vectSStaisticsMoneyInfo_Minus[i].QOrbCurrent*-1);
					tempItem.COrbCurrent	= temp_vectSStaisticsMoneyInfo_Plus[j].COrbCurrent	+(temp_vectSStaisticsMoneyInfo_Minus[i].COrbCurrent*-1);
					tempItem.GOrbCurrent	= temp_vectSStaisticsMoneyInfo_Plus[j].GOrbCurrent	+(temp_vectSStaisticsMoneyInfo_Minus[i].GOrbCurrent*-1);
					tempItem.UOrbCurrent	= temp_vectSStaisticsMoneyInfo_Plus[j].UOrbCurrent	+(temp_vectSStaisticsMoneyInfo_Minus[i].UOrbCurrent*-1);
					tempItem.Sync			= TRUE;
					temp_vectSStaisticsMoneyInfo_Plus_Sum_Minus.push_back(tempItem);
				}
			}
		}
	}// end 2012-02-27 by jhseol, 화폐통계툴 싱크가 맞지 않는 아이템을 걸러내는 루틴. 전체를 검색 하기에 기능 활성화 시 엄청 느려짐.
*/
	
	// start 2012-02-27 by jhseol, 화폐통계툴 합산된 아이템으로 실직적인 편차와 누적치 계산
	int resultCount=0;
	for(m_nIndex = 0; m_nIndex < temp_vectSStaisticsMoneyInfo_Plus_Sum_Minus.size(); m_nIndex++)
	{
		int frontIndex;
		if(m_nIndex == 0)
		{
			temp_vectSStaisticsMoneyInfo_Plus_Sum_Minus[m_nIndex].SPIGap	= temp_vectSStaisticsMoneyInfo_Plus_Sum_Minus[m_nIndex].SPICurrent;
			temp_vectSStaisticsMoneyInfo_Plus_Sum_Minus[m_nIndex].WPGap		= temp_vectSStaisticsMoneyInfo_Plus_Sum_Minus[m_nIndex].WPCurrent;
			temp_vectSStaisticsMoneyInfo_Plus_Sum_Minus[m_nIndex].SOrbGap	= temp_vectSStaisticsMoneyInfo_Plus_Sum_Minus[m_nIndex].SOrbCurrent;
			temp_vectSStaisticsMoneyInfo_Plus_Sum_Minus[m_nIndex].QOrbGap	= temp_vectSStaisticsMoneyInfo_Plus_Sum_Minus[m_nIndex].QOrbCurrent;
			temp_vectSStaisticsMoneyInfo_Plus_Sum_Minus[m_nIndex].COrbGap	= temp_vectSStaisticsMoneyInfo_Plus_Sum_Minus[m_nIndex].COrbCurrent;
			temp_vectSStaisticsMoneyInfo_Plus_Sum_Minus[m_nIndex].GOrbGap	= temp_vectSStaisticsMoneyInfo_Plus_Sum_Minus[m_nIndex].GOrbCurrent;
			temp_vectSStaisticsMoneyInfo_Plus_Sum_Minus[m_nIndex].UOrbGap	= temp_vectSStaisticsMoneyInfo_Plus_Sum_Minus[m_nIndex].UOrbCurrent;
			temp_vectSStaisticsMoneyInfo_Time.push_back(temp_vectSStaisticsMoneyInfo_Plus_Sum_Minus[m_nIndex]);
			continue;
		}
		frontIndex = m_nIndex-1;
		temp_vectSStaisticsMoneyInfo_Plus_Sum_Minus[m_nIndex].SPIGap	= temp_vectSStaisticsMoneyInfo_Plus_Sum_Minus[m_nIndex].SPICurrent;
		temp_vectSStaisticsMoneyInfo_Plus_Sum_Minus[m_nIndex].WPGap		= temp_vectSStaisticsMoneyInfo_Plus_Sum_Minus[m_nIndex].WPCurrent;
		temp_vectSStaisticsMoneyInfo_Plus_Sum_Minus[m_nIndex].SOrbGap	= temp_vectSStaisticsMoneyInfo_Plus_Sum_Minus[m_nIndex].SOrbCurrent;
		temp_vectSStaisticsMoneyInfo_Plus_Sum_Minus[m_nIndex].QOrbGap	= temp_vectSStaisticsMoneyInfo_Plus_Sum_Minus[m_nIndex].QOrbCurrent;
		temp_vectSStaisticsMoneyInfo_Plus_Sum_Minus[m_nIndex].COrbGap	= temp_vectSStaisticsMoneyInfo_Plus_Sum_Minus[m_nIndex].COrbCurrent;
		temp_vectSStaisticsMoneyInfo_Plus_Sum_Minus[m_nIndex].GOrbGap	= temp_vectSStaisticsMoneyInfo_Plus_Sum_Minus[m_nIndex].GOrbCurrent;
		temp_vectSStaisticsMoneyInfo_Plus_Sum_Minus[m_nIndex].UOrbGap	= temp_vectSStaisticsMoneyInfo_Plus_Sum_Minus[m_nIndex].UOrbCurrent;

		temp_vectSStaisticsMoneyInfo_Plus_Sum_Minus[m_nIndex].SPICurrent	= temp_vectSStaisticsMoneyInfo_Plus_Sum_Minus[m_nIndex].SPICurrent	+ temp_vectSStaisticsMoneyInfo_Plus_Sum_Minus[frontIndex].SPICurrent;
		temp_vectSStaisticsMoneyInfo_Plus_Sum_Minus[m_nIndex].WPCurrent		= temp_vectSStaisticsMoneyInfo_Plus_Sum_Minus[m_nIndex].WPCurrent	+ temp_vectSStaisticsMoneyInfo_Plus_Sum_Minus[frontIndex].WPCurrent;
		temp_vectSStaisticsMoneyInfo_Plus_Sum_Minus[m_nIndex].SOrbCurrent	= temp_vectSStaisticsMoneyInfo_Plus_Sum_Minus[m_nIndex].SOrbCurrent	+ temp_vectSStaisticsMoneyInfo_Plus_Sum_Minus[frontIndex].SOrbCurrent;
		temp_vectSStaisticsMoneyInfo_Plus_Sum_Minus[m_nIndex].QOrbCurrent	= temp_vectSStaisticsMoneyInfo_Plus_Sum_Minus[m_nIndex].QOrbCurrent	+ temp_vectSStaisticsMoneyInfo_Plus_Sum_Minus[frontIndex].QOrbCurrent;
		temp_vectSStaisticsMoneyInfo_Plus_Sum_Minus[m_nIndex].COrbCurrent	= temp_vectSStaisticsMoneyInfo_Plus_Sum_Minus[m_nIndex].COrbCurrent	+ temp_vectSStaisticsMoneyInfo_Plus_Sum_Minus[frontIndex].COrbCurrent;
		temp_vectSStaisticsMoneyInfo_Plus_Sum_Minus[m_nIndex].GOrbCurrent	= temp_vectSStaisticsMoneyInfo_Plus_Sum_Minus[m_nIndex].GOrbCurrent	+ temp_vectSStaisticsMoneyInfo_Plus_Sum_Minus[frontIndex].GOrbCurrent;
		temp_vectSStaisticsMoneyInfo_Plus_Sum_Minus[m_nIndex].UOrbCurrent	= temp_vectSStaisticsMoneyInfo_Plus_Sum_Minus[m_nIndex].UOrbCurrent	+ temp_vectSStaisticsMoneyInfo_Plus_Sum_Minus[frontIndex].UOrbCurrent;
		// start 2012-02-27 by jhseol, 화폐통계툴 계산 중 의미없는(필요없는) 아이템 제외
		if(	temp_vectSStaisticsMoneyInfo_Plus_Sum_Minus[m_nIndex].SPICurrent	!= temp_vectSStaisticsMoneyInfo_Plus_Sum_Minus[frontIndex].SPICurrent		||
			temp_vectSStaisticsMoneyInfo_Plus_Sum_Minus[m_nIndex].WPCurrent		!= temp_vectSStaisticsMoneyInfo_Plus_Sum_Minus[frontIndex].WPCurrent		||
			temp_vectSStaisticsMoneyInfo_Plus_Sum_Minus[m_nIndex].SOrbCurrent	!= temp_vectSStaisticsMoneyInfo_Plus_Sum_Minus[frontIndex].SOrbCurrent	||
			temp_vectSStaisticsMoneyInfo_Plus_Sum_Minus[m_nIndex].QOrbCurrent	!= temp_vectSStaisticsMoneyInfo_Plus_Sum_Minus[frontIndex].QOrbCurrent	||
			temp_vectSStaisticsMoneyInfo_Plus_Sum_Minus[m_nIndex].COrbCurrent	!= temp_vectSStaisticsMoneyInfo_Plus_Sum_Minus[frontIndex].COrbCurrent	||
			temp_vectSStaisticsMoneyInfo_Plus_Sum_Minus[m_nIndex].GOrbCurrent	!= temp_vectSStaisticsMoneyInfo_Plus_Sum_Minus[frontIndex].GOrbCurrent	||
			temp_vectSStaisticsMoneyInfo_Plus_Sum_Minus[m_nIndex].UOrbCurrent	!= temp_vectSStaisticsMoneyInfo_Plus_Sum_Minus[frontIndex].UOrbCurrent )
		{
			temp_vectSStaisticsMoneyInfo_Plus_Sum_Minus[m_nIndex].Count	= ++resultCount;
			temp_vectSStaisticsMoneyInfo_Time.push_back(temp_vectSStaisticsMoneyInfo_Plus_Sum_Minus[m_nIndex]);
		}// end 2012-02-27 by jhseol, 화폐통계툴 계산 중 의미없는(필요없는) 아이템 제외
	}// end 2012-02-27 by jhseol, 화폐통계툴 합산된 아이템으로 실직적인 편차와 누적치 계산

	// start 2012-02-27 by jhseol, 화폐통계툴 검색 간격 시간 설정시 해당 시간대 별로 리스트 정리
	if (TRUE == GetGapTime())
	{
		ATUM_DATE_TIME standarTime, targetTime, nextStandarTime;
		util::zero(&standarTime,sizeof(standarTime));
		util::zero(&targetTime,sizeof(targetTime));
		util::zero(&nextStandarTime,sizeof(nextStandarTime));
		SSTAISTICSMONEY_INFO tempitem;
		util::zero(&tempitem,sizeof(tempitem));
		standarTime.Year	= StartTime.year;
		standarTime.Month	= StartTime.month;
		standarTime.Day		= StartTime.day;
		standarTime.Hour	= StartTime.hour;
		standarTime.Minute	= StartTime.minute;
		standarTime.Second	= StartTime.second;
		//standarTime.AddDateTime(0,0,0,0,10,0);
		int standardNumber = 0, frontStandardNumber = 0;
		m_nIndex = 0;
		while(m_nIndex < temp_vectSStaisticsMoneyInfo_Time.size())
		{
			//nextStandarTime = temp_vectSStaisticsMoneyInfo_Time[standardNumber].Time; 
			if(standarTime <= temp_vectSStaisticsMoneyInfo_Time[standardNumber].Time)
			{
				standarTime.AddDateTime(m_GapTime.Year, m_GapTime.Month, m_GapTime.Day, m_GapTime.Hour, m_GapTime.Minute, m_GapTime.Second);
				continue;
			}
			if((m_nIndex + 1) == temp_vectSStaisticsMoneyInfo_Time.size())
			{
				if(standardNumber == 0)
				{
					temp_vectSStaisticsMoneyInfo_Time[standardNumber].SPICurrent	= temp_vectSStaisticsMoneyInfo_Time[standardNumber].SPIGap;
					temp_vectSStaisticsMoneyInfo_Time[standardNumber].WPCurrent	= temp_vectSStaisticsMoneyInfo_Time[standardNumber].WPGap;
					temp_vectSStaisticsMoneyInfo_Time[standardNumber].SOrbCurrent	= temp_vectSStaisticsMoneyInfo_Time[standardNumber].SOrbGap;
					temp_vectSStaisticsMoneyInfo_Time[standardNumber].QOrbCurrent	= temp_vectSStaisticsMoneyInfo_Time[standardNumber].QOrbGap;
					temp_vectSStaisticsMoneyInfo_Time[standardNumber].COrbCurrent	= temp_vectSStaisticsMoneyInfo_Time[standardNumber].COrbGap;
					temp_vectSStaisticsMoneyInfo_Time[standardNumber].GOrbCurrent	= temp_vectSStaisticsMoneyInfo_Time[standardNumber].GOrbGap;
					temp_vectSStaisticsMoneyInfo_Time[standardNumber].UOrbCurrent	= temp_vectSStaisticsMoneyInfo_Time[standardNumber].UOrbGap;
				}
				if(standardNumber != frontStandardNumber)
				{
					temp_vectSStaisticsMoneyInfo_Time[standardNumber].SPICurrent	= temp_vectSStaisticsMoneyInfo_Time[frontStandardNumber].SPICurrent	+ temp_vectSStaisticsMoneyInfo_Time[standardNumber].SPIGap;
					temp_vectSStaisticsMoneyInfo_Time[standardNumber].WPCurrent	= temp_vectSStaisticsMoneyInfo_Time[frontStandardNumber].WPCurrent		+ temp_vectSStaisticsMoneyInfo_Time[standardNumber].WPGap;
					temp_vectSStaisticsMoneyInfo_Time[standardNumber].SOrbCurrent	= temp_vectSStaisticsMoneyInfo_Time[frontStandardNumber].SOrbCurrent	+ temp_vectSStaisticsMoneyInfo_Time[standardNumber].SOrbGap;
					temp_vectSStaisticsMoneyInfo_Time[standardNumber].QOrbCurrent	= temp_vectSStaisticsMoneyInfo_Time[frontStandardNumber].QOrbCurrent	+ temp_vectSStaisticsMoneyInfo_Time[standardNumber].QOrbGap;
					temp_vectSStaisticsMoneyInfo_Time[standardNumber].COrbCurrent	= temp_vectSStaisticsMoneyInfo_Time[frontStandardNumber].COrbCurrent	+ temp_vectSStaisticsMoneyInfo_Time[standardNumber].COrbGap;
					temp_vectSStaisticsMoneyInfo_Time[standardNumber].GOrbCurrent	= temp_vectSStaisticsMoneyInfo_Time[frontStandardNumber].GOrbCurrent	+ temp_vectSStaisticsMoneyInfo_Time[standardNumber].GOrbGap;
					temp_vectSStaisticsMoneyInfo_Time[standardNumber].UOrbCurrent	= temp_vectSStaisticsMoneyInfo_Time[frontStandardNumber].UOrbCurrent	+ temp_vectSStaisticsMoneyInfo_Time[standardNumber].UOrbGap;
				}
				vectSStaisticsMoneyInfo_Result.push_back(temp_vectSStaisticsMoneyInfo_Time[standardNumber]);
				m_nIndex++;
				continue;
			}
			m_nIndex++;
			if(standarTime <= temp_vectSStaisticsMoneyInfo_Time[m_nIndex].Time)
			{
				if(standardNumber == 0)
				{
					temp_vectSStaisticsMoneyInfo_Time[standardNumber].SPICurrent	= temp_vectSStaisticsMoneyInfo_Time[standardNumber].SPIGap;
					temp_vectSStaisticsMoneyInfo_Time[standardNumber].WPCurrent	= temp_vectSStaisticsMoneyInfo_Time[standardNumber].WPGap;
					temp_vectSStaisticsMoneyInfo_Time[standardNumber].SOrbCurrent	= temp_vectSStaisticsMoneyInfo_Time[standardNumber].SOrbGap;
					temp_vectSStaisticsMoneyInfo_Time[standardNumber].QOrbCurrent	= temp_vectSStaisticsMoneyInfo_Time[standardNumber].QOrbGap;
					temp_vectSStaisticsMoneyInfo_Time[standardNumber].COrbCurrent	= temp_vectSStaisticsMoneyInfo_Time[standardNumber].COrbGap;
					temp_vectSStaisticsMoneyInfo_Time[standardNumber].GOrbCurrent	= temp_vectSStaisticsMoneyInfo_Time[standardNumber].GOrbGap;
					temp_vectSStaisticsMoneyInfo_Time[standardNumber].UOrbCurrent	= temp_vectSStaisticsMoneyInfo_Time[standardNumber].UOrbGap;
				}
				else
				{
					temp_vectSStaisticsMoneyInfo_Time[standardNumber].SPICurrent	= temp_vectSStaisticsMoneyInfo_Time[frontStandardNumber].SPICurrent	+ temp_vectSStaisticsMoneyInfo_Time[standardNumber].SPIGap;
					temp_vectSStaisticsMoneyInfo_Time[standardNumber].WPCurrent	= temp_vectSStaisticsMoneyInfo_Time[frontStandardNumber].WPCurrent		+ temp_vectSStaisticsMoneyInfo_Time[standardNumber].WPGap;
					temp_vectSStaisticsMoneyInfo_Time[standardNumber].SOrbCurrent	= temp_vectSStaisticsMoneyInfo_Time[frontStandardNumber].SOrbCurrent	+ temp_vectSStaisticsMoneyInfo_Time[standardNumber].SOrbGap;
					temp_vectSStaisticsMoneyInfo_Time[standardNumber].QOrbCurrent	= temp_vectSStaisticsMoneyInfo_Time[frontStandardNumber].QOrbCurrent	+ temp_vectSStaisticsMoneyInfo_Time[standardNumber].QOrbGap;
					temp_vectSStaisticsMoneyInfo_Time[standardNumber].COrbCurrent	= temp_vectSStaisticsMoneyInfo_Time[frontStandardNumber].COrbCurrent	+ temp_vectSStaisticsMoneyInfo_Time[standardNumber].COrbGap;
					temp_vectSStaisticsMoneyInfo_Time[standardNumber].GOrbCurrent	= temp_vectSStaisticsMoneyInfo_Time[frontStandardNumber].GOrbCurrent	+ temp_vectSStaisticsMoneyInfo_Time[standardNumber].GOrbGap;
					temp_vectSStaisticsMoneyInfo_Time[standardNumber].UOrbCurrent	= temp_vectSStaisticsMoneyInfo_Time[frontStandardNumber].UOrbCurrent	+ temp_vectSStaisticsMoneyInfo_Time[standardNumber].UOrbGap;
				}
				vectSStaisticsMoneyInfo_Result.push_back(temp_vectSStaisticsMoneyInfo_Time[standardNumber]);
				frontStandardNumber = standardNumber;
				standardNumber = m_nIndex;
			}
			else
			{
				temp_vectSStaisticsMoneyInfo_Time[standardNumber].SPIGap		= temp_vectSStaisticsMoneyInfo_Time[standardNumber].SPIGap		+ temp_vectSStaisticsMoneyInfo_Time[m_nIndex].SPIGap;
				temp_vectSStaisticsMoneyInfo_Time[standardNumber].WPGap		= temp_vectSStaisticsMoneyInfo_Time[standardNumber].WPGap		+ temp_vectSStaisticsMoneyInfo_Time[m_nIndex].WPGap;
				temp_vectSStaisticsMoneyInfo_Time[standardNumber].SOrbGap		= temp_vectSStaisticsMoneyInfo_Time[standardNumber].SOrbGap	+ temp_vectSStaisticsMoneyInfo_Time[m_nIndex].SOrbGap;
				temp_vectSStaisticsMoneyInfo_Time[standardNumber].QOrbGap		= temp_vectSStaisticsMoneyInfo_Time[standardNumber].QOrbGap	+ temp_vectSStaisticsMoneyInfo_Time[m_nIndex].QOrbGap;
				temp_vectSStaisticsMoneyInfo_Time[standardNumber].COrbGap		= temp_vectSStaisticsMoneyInfo_Time[standardNumber].COrbGap	+ temp_vectSStaisticsMoneyInfo_Time[m_nIndex].COrbGap;
				temp_vectSStaisticsMoneyInfo_Time[standardNumber].GOrbGap		= temp_vectSStaisticsMoneyInfo_Time[standardNumber].GOrbGap	+ temp_vectSStaisticsMoneyInfo_Time[m_nIndex].GOrbGap;
				temp_vectSStaisticsMoneyInfo_Time[standardNumber].UOrbGap		= temp_vectSStaisticsMoneyInfo_Time[standardNumber].UOrbGap	+ temp_vectSStaisticsMoneyInfo_Time[m_nIndex].UOrbGap;
			}
		}
	}// end 2012-02-27 by jhseol, 화폐통계툴 검색 간격 시간 설정시 해당 시간대 별로 리스트 정리
	else
	{
		for(m_nIndex = 0; m_nIndex < temp_vectSStaisticsMoneyInfo_Time.size(); m_nIndex++)
		{
			vectSStaisticsMoneyInfo_Result.push_back(temp_vectSStaisticsMoneyInfo_Time[m_nIndex]);
		}

	}

	m_Progress.SetRange32(0, vectSStaisticsMoneyInfo_Result.size()-1);
	// start 2012-02-27 by jhseol, 화폐통계툴 최종 리스트 아이템 등록
	int waitTime=0;
	for(m_nIndex = 0; m_nIndex < vectSStaisticsMoneyInfo_Result.size(); m_nIndex++)
	{
		vectSStaisticsMoneyInfo_Result[m_nIndex].Count = m_nIndex+1;
		m_GridStaisticsMoney.InsertGridItemStaisticsMoney(&vectSStaisticsMoneyInfo_Result[m_nIndex]);
		m_Progress.SetPos(m_nIndex);
		//if(m_nIndex % 5000 == 0) Sleep(500);
	}// end 2012-02-27 by jhseol, 화폐통계툴 최종 리스트 아이템 등록
}


void CStaisticsMoneyDlg::OnButtonSaveFile()
{
	char *pSaveStringColumn = COLUMN_STRING;
	util::zero(g_SaveStringValue, sizeof(&g_SaveStringValue));
	util::zero(g_SaveString, sizeof(&g_SaveString));

	char timeString[30] = {NULL,};
	sprintf(g_SaveString, "%s", pSaveStringColumn);
	sprintf(timeString, "%d-%d-%d %d.%d.%d",
		m_GapTime.Year,
		m_GapTime.Month,
		m_GapTime.Day,
		m_GapTime.Hour,
		m_GapTime.Minute,
		m_GapTime.Second);

	FILE* filewrite = NULL;
	CString saveServerName		=(LPSTR)m_pMainDlg->m_pServerInfo4Admin->DBIP;
	CString saveDBName			=(LPSTR)m_pMainDlg->m_pServerInfo4Admin->DBName;
	CString saveSearchStartTime;
	saveSearchStartTime.Format("%s", (LPCSTR)(m_OleDateStart.Format("%Y-%m-%d")+m_OleTimeStart.Format(" %H.%M.%S.0")));
	CString saveSearchEndTime;
	saveSearchEndTime.Format("%s", (LPCSTR)(m_OleDateEnd.Format("%Y-%m-%d")+m_OleTimeEnd.Format(" %H.%M.%S.0")));
	CString saveTimeGap			=(LPSTR)timeString;
	CString savefileName		=	"StaisticsMoneyResult[ServerIP="+saveServerName+"][DBName="+saveDBName+"]["+
									"SearchTime=("+saveSearchStartTime+")~("+saveSearchEndTime+")]["+
									"TimeGAP="+saveTimeGap+"].txt";
	char szTemp[50]={0x00,};
	CString DebugingString;
	filewrite = fopen(savefileName.GetBuffer(sizeof(savefileName)),"w");
	if( filewrite != NULL ) 
	{
		fputs(g_SaveString, filewrite);
		DebugingString = "";
		DebugingString += itoa(vectSStaisticsMoneyInfo_Result.size(), szTemp,10);
		OutputDebugString("[SEOL Debuging Check] : vectSStaisticsMoneyInfo_Result.size = "+DebugingString+"\r\n");
		for(m_nIndex = 0; m_nIndex < vectSStaisticsMoneyInfo_Result.size(); m_nIndex++)
		{
			sprintf(timeString, "%d-%d-%d %d:%d:%d",
				vectSStaisticsMoneyInfo_Result[m_nIndex].Time.Year,
				vectSStaisticsMoneyInfo_Result[m_nIndex].Time.Month,
				vectSStaisticsMoneyInfo_Result[m_nIndex].Time.Day,
				vectSStaisticsMoneyInfo_Result[m_nIndex].Time.Hour,
				vectSStaisticsMoneyInfo_Result[m_nIndex].Time.Minute,
				vectSStaisticsMoneyInfo_Result[m_nIndex].Time.Second);
			sprintf(g_SaveStringValue, "%d\t%s\t%I64d\t%I64d\t%I64d\t%I64d\t%I64d\t%I64d\t%I64d\t%I64d\t%I64d\t%I64d\t%I64d\t%I64d\t%I64d\t%I64d\n",
				vectSStaisticsMoneyInfo_Result[m_nIndex].Count,
				timeString,
				vectSStaisticsMoneyInfo_Result[m_nIndex].SPIGap,
				vectSStaisticsMoneyInfo_Result[m_nIndex].SPICurrent,
				vectSStaisticsMoneyInfo_Result[m_nIndex].WPGap,
				vectSStaisticsMoneyInfo_Result[m_nIndex].WPCurrent,
				vectSStaisticsMoneyInfo_Result[m_nIndex].SOrbGap,
				vectSStaisticsMoneyInfo_Result[m_nIndex].SOrbCurrent,
				vectSStaisticsMoneyInfo_Result[m_nIndex].QOrbGap,
				vectSStaisticsMoneyInfo_Result[m_nIndex].QOrbCurrent,
				vectSStaisticsMoneyInfo_Result[m_nIndex].COrbGap,
				vectSStaisticsMoneyInfo_Result[m_nIndex].COrbCurrent,
				vectSStaisticsMoneyInfo_Result[m_nIndex].GOrbGap,
				vectSStaisticsMoneyInfo_Result[m_nIndex].GOrbCurrent,
				vectSStaisticsMoneyInfo_Result[m_nIndex].UOrbGap,
				vectSStaisticsMoneyInfo_Result[m_nIndex].UOrbCurrent
				);
			fputs(g_SaveStringValue, filewrite);
			util::zero(timeString,sizeof(&timeString));
			util::zero(g_SaveStringValue,sizeof(&g_SaveStringValue));
			m_Progress.SetPos(m_nIndex);
			if (m_nIndex % 1000 == 0)
			{
				DebugingString = "";
				DebugingString += itoa(m_nIndex,szTemp,10);
				OutputDebugString("[SEOL Debuging Check] : Save Index = "+DebugingString+"\r\n");
				Sleep(100);
			}
		}
	}
	fclose(filewrite);
	MessageBox("This Program Path is created successfully!\r\nFile Name : "+savefileName);
}

void CStaisticsMoneyDlg::OnGraphChange(NMHDR *pNMHDR, LRESULT *pResult)
{
	UpdateData();
	OutputDebugString("[SEOL Debuging Check] : TabControl Click. \r\n");
	if(m_pTab_show != NULL)
	{
		m_pTab_show->ShowWindow(SW_HIDE);
		m_pTab_show = NULL;
	}
	int index = m_GraphTab.GetCurSel();
	m_pCurrentCD = GetDC();
	if(m_pCurrentCD != NULL)
	{
		m_pCurrentCD->Rectangle(GRAPH_BACKGROUND_POS_X_START, GRAPH_BACKGROUND_POS_Y_START, GRAPH_BACKGROUND_POS_X_END, GRAPH_BACKGROUND_POS_Y_END);
		ReleaseDC(m_pCurrentCD);
	}
	DrawGraph(index);
}

void CStaisticsMoneyDlg::DrawGraph(int SelectColumn)
{
	vector<INT64> vectGraphItem;
	vector<SSTAISTICSMONEY_INFO>::iterator itritemResult = vectSStaisticsMoneyInfo_Result.begin();
	INT64		MaxItem = 0;
	INT64		Minitem = 0;
	CPen DefaultPen(PS_SOLID, 1, RGB(0,0,0));
	CPen DashPenGirdLine(PS_SOLID, 1, RGB(0,0,0));
	CPen DotPen(PS_DOT, 1, RGB(192,192,192));
	CPen DashPen(PS_DASH, 1, RGB(192,192,192));
	CPen DrawColorD(PS_SOLID, 2, RGB(0,0,0));
	CPen DrawColorR(PS_SOLID, 2, RGB(192,0,0));
	CPen DrawColorG(PS_SOLID, 2, RGB(0,192,0));
	CPen DrawColorB(PS_SOLID, 2, RGB(0,0,192));
	CPen DrawColorC(PS_SOLID, 2, RGB(0,192,192));
	CPen DrawColorM(PS_SOLID, 2, RGB(192,0,192));
	CPen DrawColorY(PS_SOLID, 2, RGB(192,192,0));
	CPen *SelectPen;

	switch(SelectColumn)
	{
	case 0:
		while(itritemResult != vectSStaisticsMoneyInfo_Result.end())
		{
			if (itritemResult->SPICurrent >= MaxItem)	MaxItem = itritemResult->SPICurrent;
			if (itritemResult->SPICurrent <= Minitem)	Minitem = itritemResult->SPICurrent;
			vectGraphItem.push_back(itritemResult->SPICurrent);
			itritemResult++;
		}
		SelectPen = &DrawColorD;
		break;
	case 1:
		while(itritemResult != vectSStaisticsMoneyInfo_Result.end())
		{
			if (itritemResult->WPCurrent >= MaxItem)	MaxItem = itritemResult->WPCurrent;
			if (itritemResult->WPCurrent <= Minitem)	Minitem = itritemResult->WPCurrent;
			vectGraphItem.push_back(itritemResult->WPCurrent);
			itritemResult++;
		}
		SelectPen = &DrawColorR;
		break;
	case 2:
		while(itritemResult != vectSStaisticsMoneyInfo_Result.end())
		{
			if (itritemResult->SOrbCurrent >= MaxItem)	MaxItem = itritemResult->SOrbCurrent;
			if (itritemResult->SOrbCurrent <= Minitem)	Minitem = itritemResult->SOrbCurrent;
			vectGraphItem.push_back(itritemResult->SOrbCurrent);
			itritemResult++;
		}
		SelectPen = &DrawColorG;
		break;
	case 3:
		while(itritemResult != vectSStaisticsMoneyInfo_Result.end())
		{
			if (itritemResult->QOrbCurrent >= MaxItem)	MaxItem = itritemResult->QOrbCurrent;
			if (itritemResult->QOrbCurrent <= Minitem)	Minitem = itritemResult->QOrbCurrent;
			vectGraphItem.push_back(itritemResult->QOrbCurrent);
			itritemResult++;
		}
		SelectPen = &DrawColorB;
		break;
	case 4:
		while(itritemResult != vectSStaisticsMoneyInfo_Result.end())
		{
			if (itritemResult->COrbCurrent >= MaxItem)	MaxItem = itritemResult->COrbCurrent;
			if (itritemResult->COrbCurrent <= Minitem)	Minitem = itritemResult->COrbCurrent;
			vectGraphItem.push_back(itritemResult->COrbCurrent);
			itritemResult++;
		}
		SelectPen = &DrawColorC;
		break;
	case 5:
		while(itritemResult != vectSStaisticsMoneyInfo_Result.end())
		{
			if (itritemResult->GOrbCurrent >= MaxItem)	MaxItem = itritemResult->GOrbCurrent;
			if (itritemResult->GOrbCurrent <= Minitem)	Minitem = itritemResult->GOrbCurrent;
			vectGraphItem.push_back(itritemResult->GOrbCurrent);
			itritemResult++;
		}
		SelectPen = &DrawColorM;
		break;
	case 6:
		while(itritemResult != vectSStaisticsMoneyInfo_Result.end())
		{
			if (itritemResult->UOrbCurrent >= MaxItem)	MaxItem = itritemResult->UOrbCurrent;
			if (itritemResult->UOrbCurrent <= Minitem)	Minitem = itritemResult->UOrbCurrent;
			vectGraphItem.push_back(itritemResult->UOrbCurrent);
			itritemResult++;
		}
		SelectPen = &DrawColorY;
		break;
	}
	double graphXGap;
	double graphYGap;
	BOOL MinMaxZero = FALSE;
	if(MaxItem == 0 && Minitem == 0)
	{
		MaxItem = 1;
		graphXGap = (double)(GRAPH_DRAW_WIDTH)/(double)vectSStaisticsMoneyInfo_Result.size();
		graphYGap = GRAPH_DRAW_HIGHT;
		MinMaxZero = TRUE;
	}
	else
	{
		if(Minitem <= 0) Minitem = Minitem*-1;
		graphXGap = (double)(GRAPH_DRAW_WIDTH)/(double)vectSStaisticsMoneyInfo_Result.size();
		graphYGap = (double)(GRAPH_DRAW_HIGHT)/(double)(MaxItem+Minitem);
		MinMaxZero = FALSE;
	}
	int LineCountGap = 0;
	if(vectGraphItem.size() >= 0		&& vectGraphItem.size() < 50)		LineCountGap = 1;
	else if(vectGraphItem.size() >= 50		&& vectGraphItem.size() < 100)		LineCountGap = 2;
	else if(vectGraphItem.size() >= 100		&& vectGraphItem.size() < 250)		LineCountGap = 10;
	else if(vectGraphItem.size() >= 250		&& vectGraphItem.size() < 500)		LineCountGap = 50;
	else if(vectGraphItem.size() >= 500		&& vectGraphItem.size() < 1000)		LineCountGap = 100;
	else if(vectGraphItem.size() >= 1000	&& vectGraphItem.size() < 5000)		LineCountGap = 200;
	else if(vectGraphItem.size() >= 5000	&& vectGraphItem.size() < 10000)	LineCountGap = 500;
	else if(vectGraphItem.size() >= 10000	&& vectGraphItem.size() < 50000)	LineCountGap = 1000;
	else if(vectGraphItem.size() >= 50000)										LineCountGap = 5000;
	
	m_pCurrentCD = GetDC();
	if(m_pCurrentCD != NULL)
	{
		m_pCurrentCD->MoveTo(GRAPH_DRAW_POS_X_START	,GRAPH_DRAW_POS_Y_START+(MaxItem*graphYGap));
		m_pCurrentCD->LineTo(GRAPH_DRAW_POS_X_STANDARD_END	,GRAPH_DRAW_POS_Y_START+(MaxItem*graphYGap));
		
		
		m_pCurrentCD->SelectObject(SelectPen);
		m_pCurrentCD->MoveTo(GRAPH_DRAW_POS_X_START	,GRAPH_DRAW_POS_Y_START+(MaxItem*graphYGap));
		for(m_nIndex = 0; m_nIndex < vectGraphItem.size(); m_nIndex++)
		{
			m_pCurrentCD->LineTo(GRAPH_DRAW_POS_X_START+((m_nIndex+1)*graphXGap)
				,GRAPH_DRAW_POS_Y_START+(MaxItem*graphYGap)-((double)vectGraphItem[m_nIndex]*graphYGap));
		}

		
		m_pCurrentCD->SelectObject(&DotPen);
		m_nIndex = 0;
		char DrawNo[10];
		while(m_nIndex <= vectGraphItem.size())
		{
			m_pCurrentCD->MoveTo(GRAPH_DRAW_POS_X_START+((m_nIndex)*graphXGap)	,GRAPH_DRAW_POS_Y_START);
			m_pCurrentCD->LineTo(GRAPH_DRAW_POS_X_START+((m_nIndex)*graphXGap)	,GRAPH_DRAW_POS_Y_END);
			//if(m_nIndex != vectGraphItem.size())
				m_pCurrentCD->TextOut(GRAPH_DRAW_POS_X_START+TEXTPOS+((m_nIndex)*graphXGap), GRAPH_DRAW_POS_Y_START+TEXTPOS+(MaxItem*graphYGap), itoa(m_nIndex,DrawNo,10));
			m_nIndex = m_nIndex+LineCountGap;
		}
		
		
		m_pCurrentCD->SelectObject(&DashPenGirdLine);
		m_pCurrentCD->MoveTo(GRAPH_DRAW_POS_X_START	,GRAPH_DRAW_POS_Y_START);
		m_pCurrentCD->LineTo(GRAPH_DRAW_POS_X_STANDARD_END	,GRAPH_DRAW_POS_Y_START);
		if(MinMaxZero==1)	m_pCurrentCD->TextOut(GRAPH_DRAW_POS_X_START+TEXTPOS, GRAPH_DRAW_POS_Y_START+TEXTPOS, "0");
		else				m_pCurrentCD->TextOut(GRAPH_DRAW_POS_X_START+TEXTPOS, GRAPH_DRAW_POS_Y_START+TEXTPOS, _i64toa(MaxItem,DrawNo,10));
		m_pCurrentCD->MoveTo(GRAPH_DRAW_POS_X_START	,GRAPH_DRAW_POS_Y_END);
		m_pCurrentCD->LineTo(GRAPH_DRAW_POS_X_STANDARD_END	,GRAPH_DRAW_POS_Y_END);
		m_pCurrentCD->TextOut(GRAPH_DRAW_POS_X_START+TEXTPOS, GRAPH_DRAW_POS_Y_END+TEXTPOS, _i64toa(Minitem*-1,DrawNo,10));
		
		
		m_pCurrentCD->SelectObject(&DashPen);
		int WidthLineCount = 6;
		for(m_nIndex = 1; m_nIndex < WidthLineCount; m_nIndex++)
		{
			m_pCurrentCD->MoveTo(GRAPH_DRAW_POS_X_START	,GRAPH_DRAW_POS_Y_START+((GRAPH_DRAW_HIGHT)/WidthLineCount)*m_nIndex);
			m_pCurrentCD->LineTo(GRAPH_DRAW_POS_X_STANDARD_END	,GRAPH_DRAW_POS_Y_START+((GRAPH_DRAW_HIGHT)/WidthLineCount)*m_nIndex);
			if(MinMaxZero==1)	m_pCurrentCD->TextOut(GRAPH_DRAW_POS_X_START+TEXTPOS, GRAPH_DRAW_POS_Y_START+TEXTPOS+((GRAPH_DRAW_HIGHT)/WidthLineCount)*m_nIndex, "0");
			else				m_pCurrentCD->TextOut(GRAPH_DRAW_POS_X_START+TEXTPOS, GRAPH_DRAW_POS_Y_START+TEXTPOS+((GRAPH_DRAW_HIGHT)/WidthLineCount)*m_nIndex, _i64toa(MaxItem-(((MaxItem+Minitem)/WidthLineCount)*m_nIndex),DrawNo,10));
		}
		
		ReleaseDC(m_pCurrentCD);
	}
}

void CStaisticsMoneyDlg::OnButtonReset()
{
	UpdateData();
	m_OleDateStart	= (CTime::GetCurrentTime() - CTimeSpan(1, 0 , 0, 0));
	m_OleDateEnd	= CTime::GetCurrentTime();
	m_OleTimeStart	= CTime::GetCurrentTime();
	m_OleTimeEnd	= CTime::GetCurrentTime();
	m_bCheckGapTimeDay = FALSE;
	m_bCheckGapTimeHour = FALSE;
	m_bCheckGapTimeMinute = FALSE;
	CheckBoxCheck();
	m_ComboGapTimeDay.SetCurSel(0);
	m_ComboGapTimeHour.SetCurSel(0);
	m_ComboGapTimeMinute.SetCurSel(0);
	m_GraphTab.SetCurSel(0);
	m_GridStaisticsMoney.InitGrid(NUM_COLUMNS_STAISTICSMONEY, g_arrStaisicsMoneyColumnNames);
	m_GridStaisticsMoney.ArrangeGrid();
	if(m_pTab_show != NULL)
	{
		m_pTab_show->ShowWindow(SW_HIDE);
		m_pTab_show = NULL;
	}
	GetDlgItem(IDC_GRAPH_TAB)->EnableWindow(FALSE);
	UpdateData(FALSE);
	m_pCurrentCD = GetDC();
	if(m_pCurrentCD != NULL)
	{
		m_pCurrentCD->Rectangle(GRAPH_BACKGROUND_POS_X_START, GRAPH_BACKGROUND_POS_Y_START, GRAPH_BACKGROUND_POS_X_END, GRAPH_BACKGROUND_POS_Y_END);
		ReleaseDC(m_pCurrentCD);
	}

}

void CStaisticsMoneyDlg::CheckBoxCheck()	// start end 2012-02-27 by jhseol, 화폐통계툴 체크박스 활성화 함수
{
	GetDlgItem(IDC_COMBO_GAP_TIME_YEAR)->EnableWindow(m_bCheckGapTimeYear);
	GetDlgItem(IDC_COMBO_GAP_TIME_MONTH)->EnableWindow(m_bCheckGapTimeMonth);
	GetDlgItem(IDC_COMBO_GAP_TIME_DAY)->EnableWindow(m_bCheckGapTimeDay);
	GetDlgItem(IDC_COMBO_GAP_TIME_HOUR)->EnableWindow(m_bCheckGapTimeHour);
	GetDlgItem(IDC_COMBO_GAP_TIME_MINUTE)->EnableWindow(m_bCheckGapTimeMinute);
}

void CStaisticsMoneyDlg::OnCheckGapTimeYear()
{
	if(m_bCheckGapTimeYear) m_bCheckGapTimeYear = FALSE;
	else m_bCheckGapTimeYear = TRUE;
	CheckBoxCheck();
}

void CStaisticsMoneyDlg::OnCheckGapTimeMonth()
{
	if(m_bCheckGapTimeMonth) m_bCheckGapTimeMonth = FALSE;
	else m_bCheckGapTimeMonth = TRUE;
	CheckBoxCheck();
}

void CStaisticsMoneyDlg::OnCheckGapTimeDay()
{
	if(m_bCheckGapTimeDay) m_bCheckGapTimeDay = FALSE;
	else m_bCheckGapTimeDay = TRUE;
	CheckBoxCheck();
}

void CStaisticsMoneyDlg::OnCheckGapTimeHour()
{
	if(m_bCheckGapTimeHour) m_bCheckGapTimeHour = FALSE;
	else m_bCheckGapTimeHour = TRUE;
	CheckBoxCheck();
}

void CStaisticsMoneyDlg::OnCheckGapTimeMinute()
{
	if(m_bCheckGapTimeMinute) m_bCheckGapTimeMinute = FALSE;
	else m_bCheckGapTimeMinute = TRUE;
	CheckBoxCheck();
}



BOOL CStaisticsMoneyDlg::GetGapTime()
{
	util::zero(&m_GapTime, sizeof(m_GapTime));
	if(m_bCheckGapTimeYear)
	{
		switch(m_ComboGapTimeYear.GetCurSel())
		{
		case 0:		m_GapTime.Year = 1;	break;
		case 1:		m_GapTime.Year = 2;	break;
		case 2:		m_GapTime.Year = 3;	break;
		case 3:		m_GapTime.Year = 4;	break;
		case 4:		m_GapTime.Year = 5;	break;
		case 5:		m_GapTime.Year = 6;	break;
		case 6:		m_GapTime.Year = 7;	break;
		case 7:		m_GapTime.Year = 8;	break;
		case 8:		m_GapTime.Year = 9;	break;
		case 9:		m_GapTime.Year = 10;	break;
		}
	}
	if(m_bCheckGapTimeMonth)
	{
		switch(m_ComboGapTimeMonth.GetCurSel())
		{
		case 0:		m_GapTime.Month = 1;	break;
		case 1:		m_GapTime.Month = 2;	break;
		case 2:		m_GapTime.Month = 3;	break;
		case 3:		m_GapTime.Month = 4;	break;
		case 4:		m_GapTime.Month = 5;	break;
		case 5:		m_GapTime.Month = 6;	break;
		case 6:		m_GapTime.Month = 7;	break;
		case 7:		m_GapTime.Month = 8;	break;
		case 8:		m_GapTime.Month = 9;	break;
		case 9:		m_GapTime.Month = 10;	break;
		case 10:	m_GapTime.Month = 11;	break;
		}
	}
	if(m_bCheckGapTimeDay)
	{
		switch(m_ComboGapTimeDay.GetCurSel())
		{
		case 0:		m_GapTime.Day = 1;	break;
		case 1:		m_GapTime.Day = 2;	break;
		case 2:		m_GapTime.Day = 3;	break;
		case 3:		m_GapTime.Day = 4;	break;
		case 4:		m_GapTime.Day = 5;	break;
		case 5:		m_GapTime.Day = 6;	break;
		case 6:		m_GapTime.Day = 7;	break;
		case 7:		m_GapTime.Day = 8;	break;
		case 8:		m_GapTime.Day = 9;	break;
		case 9:		m_GapTime.Day = 10;	break;
		case 10:	m_GapTime.Day = 11;	break;
		case 11:	m_GapTime.Day = 12;	break;
		case 12:	m_GapTime.Day = 13;	break;
		case 13:	m_GapTime.Day = 14;	break;
		case 14:	m_GapTime.Day = 15;	break;
		case 15:	m_GapTime.Day = 16;	break;
		case 16:	m_GapTime.Day = 17;	break;
		case 17:	m_GapTime.Day = 18;	break;
		case 18:	m_GapTime.Day = 19;	break;
		case 19:	m_GapTime.Day = 20;	break;
		case 20:	m_GapTime.Day = 21;	break;
		case 21:	m_GapTime.Day = 22;	break;
		case 22:	m_GapTime.Day = 23;	break;
		case 23:	m_GapTime.Day = 24;	break;
		case 24:	m_GapTime.Day = 25;	break;
		case 25:	m_GapTime.Day = 26;	break;
		case 26:	m_GapTime.Day = 27;	break;
		case 27:	m_GapTime.Day = 28;	break;
		case 28:	m_GapTime.Day = 29;	break;
		case 29:	m_GapTime.Day = 30;	break;
		case 30:	m_GapTime.Day = 31;	break;
		}
	}
	if(m_bCheckGapTimeHour)
	{
		switch(m_ComboGapTimeHour.GetCurSel())
		{
		case 0:		m_GapTime.Hour = 1;	break;
		case 1:		m_GapTime.Hour = 2;	break;
		case 2:		m_GapTime.Hour = 3;	break;
		case 3:		m_GapTime.Hour = 4;	break;
		case 4:		m_GapTime.Hour = 5;	break;
		case 5:		m_GapTime.Hour = 6;	break;
		case 6:		m_GapTime.Hour = 7;	break;
		case 7:		m_GapTime.Hour = 8;	break;
		case 8:		m_GapTime.Hour = 9;	break;
		case 9:		m_GapTime.Hour = 10;	break;
		case 10:	m_GapTime.Hour = 11;	break;
		case 11:	m_GapTime.Hour = 12;	break;
		case 12:	m_GapTime.Hour = 13;	break;
		case 13:	m_GapTime.Hour = 14;	break;
		case 14:	m_GapTime.Hour = 15;	break;
		case 15:	m_GapTime.Hour = 16;	break;
		case 16:	m_GapTime.Hour = 17;	break;
		case 17:	m_GapTime.Hour = 18;	break;
		case 18:	m_GapTime.Hour = 19;	break;
		case 19:	m_GapTime.Hour = 20;	break;
		case 20:	m_GapTime.Hour = 21;	break;
		case 21:	m_GapTime.Hour = 22;	break;
		case 22:	m_GapTime.Hour = 23;	break;
		}
	}
	if(m_bCheckGapTimeMinute)
	{
		switch(m_ComboGapTimeMinute.GetCurSel())
		{
		case 0:		m_GapTime.Minute = 10;	break;
		case 1:		m_GapTime.Minute = 20;	break;
		case 2:		m_GapTime.Minute = 30;	break;
		case 3:		m_GapTime.Minute = 40;	break;
		case 4:		m_GapTime.Minute = 50;	break;
		}
	}
	
	if(m_bCheckGapTimeYear || m_bCheckGapTimeMonth || m_bCheckGapTimeDay || m_bCheckGapTimeHour || m_bCheckGapTimeMinute) return TRUE;
	return FALSE;
}


