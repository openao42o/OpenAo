// StatisticPage101Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "spacecowboydbeditortool.h"
#include "StatisticPage101Dlg.h"
#include "SpaceCowboyDBEditorToolDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStatisticPage101Dlg property page

IMPLEMENT_DYNCREATE(CStatisticPage101Dlg, CPropertyPage)

CStatisticPage101Dlg::CStatisticPage101Dlg() : CPropertyPage(CStatisticPage101Dlg::IDD)
{
	//{{AFX_DATA_INIT(CStatisticPage101Dlg)
	m_ctl_CreatedTimeStart = COleDateTime::GetCurrentTime();
	m_ctl_CreatedDateEnd = COleDateTime::GetCurrentTime();
	m_ctl_CreatedTimeEnd = COleDateTime::GetCurrentTime();
	m_edit_MaxRankCount = 0;
	m_ctl_CreatedDateStart = COleDateTime::GetCurrentTime();
	m_ctl_bIncrease		= FALSE;
	m_ctl_bDecrease		= FALSE;
	m_edit_PCBangLevel  = 3;
	//}}AFX_DATA_INIT
	m_pMainDlg = (CSpaceCowboyDBEditorToolDlg*)AfxGetMainWnd();
// 2007-11-07 by cmkwon, 로그 DB 서버 따로 구축하기 - 하나의 연결로 처리
//	m_pODBCStmt2			= new CODBCStatement();
	// 2008-03-20 by cmkwon, 버그 수정 - Cash Item 검색시는 따로 연결을 해야 한다.
	//m_pODBCStmt2			= m_pMainDlg->m_pODBCStmt;	// 2007-11-07 by cmkwon, 로그 DB 서버 따로 구축하기 - 하나의 연결로 처리
	m_pODBCStmt2			= new CODBCStatement();		// 2008-03-20 by cmkwon, 버그 수정 - Cash Item 검색시는 따로 연결을 해야 한다.
}

CStatisticPage101Dlg::~CStatisticPage101Dlg()
{
// 2007-11-07 by cmkwon, 로그 DB 서버 따로 구축하기 - 하나의 연결로 처리
//	util::del(m_pODBCStmt2);
	util::del(m_pODBCStmt2);		// 2008-03-20 by cmkwon, 버그 수정 - Cash Item 검색시는 따로 연결을 해야 한다.
}

void CStatisticPage101Dlg::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStatisticPage101Dlg)
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER2, m_ctl_CreatedTimeStart);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER3, m_ctl_CreatedDateEnd);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER4, m_ctl_CreatedTimeEnd);
	DDX_Text(pDX, IDC_EDIT_MaxRankCount, m_edit_MaxRankCount);
	DDX_Control(pDX, IDC_CUSTOM_STA_PCBangInfo, m_GridStaPCBang);
	DDX_Control(pDX, IDC_CUSTOM_PCBangDaily, m_GridStaPCBangDaily);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER1, m_ctl_CreatedDateStart);
	DDX_Check(pDX, IDC_CHECK_Increase, m_ctl_bIncrease);
	DDX_Check(pDX, IDC_CHECK_Decrease, m_ctl_bDecrease);
	DDX_Text(pDX, IDC_EDIT_PCBangLevel, m_edit_PCBangLevel);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CStatisticPage101Dlg, CPropertyPage)
	//{{AFX_MSG_MAP(CStatisticPage101Dlg)
	ON_BN_CLICKED(IDC_BUTTON_ST101_SELECT, OnButtonSt101Select)
	ON_BN_CLICKED(IDC_BUTTON_ST101_SAVE, OnButtonSt101Save)
	ON_BN_CLICKED(IDC_CHECK_Increase, OnIncreaseSort)
	ON_BN_CLICKED(IDC_CHECK_Decrease, OnDecreaseSort)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStatisticPage101Dlg message handlers

void CStatisticPage101Dlg::OnButtonSt101Select() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	m_lPCBangTotal.clear();
	m_lPCBangTotalByDaily.clear();

	if (FALSE == GetPCBangPlayData())
	{
		MessageBox("PCBang_DBLoadError");
		return;
	}
	
	DisPlayPCBangTotalPlayTime();

	if (FALSE == GetPCBangPlayTimeDailyData())
	{
		MessageBox("PCBangDaily_DBLoadError");
		return;
	}
	
	DisPlayPCBangTotalPlayTimeDaily();
	
}

struct PCBangTotalPlayTime_Sort_ObjectDecrease
{
	bool operator()(const m_PCBangTotal &pPCBangTotal1, const m_PCBangTotal &pPCBangTotal2) const
	{
		return pPCBangTotal1.m_TotalPCBangPlayTime > pPCBangTotal2.m_TotalPCBangPlayTime;	// 내림 차순 정렬을 위해 반대로 함
	};
};

struct PCBangTotalPlayTime_Sort_ObjectIncrease
{
	bool operator()(const m_PCBangTotal &pPCBangTotal1, const m_PCBangTotal &pPCBangTotal2) const
	{
		return pPCBangTotal1.m_TotalPCBangPlayTime < pPCBangTotal2.m_TotalPCBangPlayTime;	// 내림 차순 정렬을 위해 반대로 함
	};
};

BOOL CStatisticPage101Dlg::GetPCBangPlayData()
{
// 2007-11-07 by cmkwon, 로그 DB 서버 따로 구축하기 - 하나의 연결로 처리
//	if (!m_pODBCStmt2->Init(m_pMainDlg->m_pServerInfo4Admin->DBIP, m_pMainDlg->m_pServerInfo4Admin->DBPort, m_pMainDlg->m_pServerInfo4Admin->DBName,
//							m_pMainDlg->m_pServerInfo4Admin->DBUID, m_pMainDlg->m_pServerInfo4Admin->DBPWD, GetSafeHwnd())
//	)
//	{
//		return FALSE;
//	}

	///////////////////////////////////////////////////////////////////////////////
	// 2008-03-20 by cmkwon, 버그 수정 - Cash Item 검색시는 따로 연결을 해야 한다.
	if( FALSE == m_pODBCStmt2->Init(m_pMainDlg->m_pServerInfo4Admin->LogDBIP, m_pMainDlg->m_pServerInfo4Admin->LogDBPort, m_pMainDlg->m_pServerInfo4Admin->DBName,
								m_pMainDlg->m_pServerInfo4Admin->LogDBUID, m_pMainDlg->m_pServerInfo4Admin->LogDBPWD, GetSafeHwnd()) )
	{
		return FALSE;
	}

	m_PCBangTotal PCBangTotal;
	
	CString		tDBNAME;
	CString		szStartDate, szEndDate; 

	int			m_lPCBangTotalCheck = 0;
	
// 2008-01-24 by cmkwon, S_A: 날짜 관련 로컬리 제이션 처리 버그 수정 - 아래와 같은 형식으로 수정
// 	szStartDate.Format("%04d-%02d-%02d %02d:%02d:%02d",
// 		m_ctl_CreatedDateStart.GetYear(),m_ctl_CreatedDateStart.GetMonth(),m_ctl_CreatedDateStart.GetDay(),
// 		m_ctl_CreatedTimeStart.GetHour(),m_ctl_CreatedTimeStart.GetMinute(),m_ctl_CreatedTimeStart.GetSecond());
// 	szEndDate.Format("%04d-%02d-%02d %02d:%02d:%02d",
// 		m_ctl_CreatedDateEnd.GetYear(),m_ctl_CreatedDateEnd.GetMonth(),m_ctl_CreatedDateEnd.GetDay(),
// 		m_ctl_CreatedTimeEnd.GetHour(),m_ctl_CreatedTimeEnd.GetMinute(),m_ctl_CreatedTimeEnd.GetSecond());
// 2008-08-28 by cmkwon, MySQL 관련 날짜시간 관련 오류 수정 - 
//	szStartDate.Format("%04d%02d%02d %02d:%02d:%02d",
	szStartDate.Format(SQL_DATETIME_STRING_FORMAT,
		m_ctl_CreatedDateStart.GetYear(),m_ctl_CreatedDateStart.GetMonth(),m_ctl_CreatedDateStart.GetDay(),
		m_ctl_CreatedTimeStart.GetHour(),m_ctl_CreatedTimeStart.GetMinute(),m_ctl_CreatedTimeStart.GetSecond());
// 2008-08-28 by cmkwon, MySQL 관련 날짜시간 관련 오류 수정 - 
//	szEndDate.Format("%04d%02d%02d %02d:%02d:%02d",
	szEndDate.Format(SQL_DATETIME_STRING_FORMAT,
		m_ctl_CreatedDateEnd.GetYear(),m_ctl_CreatedDateEnd.GetMonth(),m_ctl_CreatedDateEnd.GetDay(),
		m_ctl_CreatedTimeEnd.GetHour(),m_ctl_CreatedTimeEnd.GetMinute(),m_ctl_CreatedTimeEnd.GetSecond());

	CString szSQLQuery;

	for(int sernum=1; sernum <= SIZE_MAX_SERVER_GROUP_COUNT; sernum++)
	{
		// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
		//szSQLQuery.Format("use atum2_db_%d", sernum);
		szSQLQuery.Format(QUERY_080702_0126, sernum);
		BOOL bRet = m_pODBCStmt2->ExecuteQuery((LPCSTR)szSQLQuery);
		m_pODBCStmt2->FreeStatement();			// clean up
		if (!bRet)
		{
			continue;
		}
		
		// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
		//szSQLQuery.Format("drop view PCBangPlayTime");
		szSQLQuery.Format(QUERY_080702_0127);
// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
//		bRet = m_pODBCStmt2->ExecuteQuery((LPCSTR)szSQLQuery);
		SQLHSTMT hstmt = m_pODBCStmt2->GetSTMTHandle();
		SQLINTEGER arrCB2[2] = {SQL_NTS,SQL_NTS};
		SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SZQUERY, 0, (LPSTR)(LPCSTR)szSQLQuery, 0,			&arrCB2[1]);
		bRet = m_pODBCStmt2->ExecuteQuery((char*)(PROCEDURE_080827_0122));
		m_pODBCStmt2->FreeStatement();			// clean up

		// 2007-01-23 by dhjin, backupLog, Log를 합친 뷰를 생성
// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
// 		szSQLQuery.Format("Create View dbo.PCBangPlayTime \
// 							as \
// 							select sum(PCBangPlayTime) AS TotalPCBangPlayTime, PCBangUID  \
// 							from dbo.atum_log_user_game_start_end WITH (NOLOCK) where LogType = 20 and Time >= '%s' and Time <= '%s' \
// 							group by PCBangUID having PCBangUID IS NOT NULL \
// 							UNION \
// 							select sum(PCBangPlayTime) AS TotalPCBangPlayTime, PCBangUID  \
// 							from dbo.atum_backup_log_user_game_start_end WITH (NOLOCK) where LogType = 20 and Time >= '%s' and Time <= '%s'\
// 							group by PCBangUID having PCBangUID IS NOT NULL"
// 							, szStartDate,szEndDate, szStartDate,szEndDate);
		szSQLQuery.Format(QUERY_080702_0128, szStartDate,szEndDate, szStartDate,szEndDate);
// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
//		bRet = m_pODBCStmt2->ExecuteQuery((LPCSTR)szSQLQuery);
		hstmt = m_pODBCStmt2->GetSTMTHandle();
		arrCB2[1] = SQL_NTS;
		SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SZQUERY, 0, (LPSTR)(LPCSTR)szSQLQuery, 0,			&arrCB2[1]);
		bRet = m_pODBCStmt2->ExecuteQuery((char*)(PROCEDURE_080827_0122));
		if (!bRet)
		{
			m_pODBCStmt2->FreeStatement();		// clean up
			continue;
		}
		
		// 2007-01-23 by dhjin, 각 PC방에서 총 플레이한 시간을 가져온다
		SQLINTEGER arrCB[10]={SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS};
		UID32_t  byPCBangUID				= 0;
		SQLCHAR	 byPCBangName[50]			= " ,";	
		INT		 byTotalPCBangPlayTime		= 0;
		SQLCHAR	 byPCBangTel[14]			= " ,";
		SQLCHAR	 byPCBangZipCode[15]		= " ,";
		SQLCHAR	 byPCBangAddr_Sido[10]		= " ,";
		SQLCHAR	 byPCBangAddr_SiGuGun[20]	= " ,";
		SQLCHAR	 byPCBangAddr_Dong[100]		= " ,";
		SQLCHAR	 byPCBangAddr_Detail[50]	= " ,";
		BYTE	 byPCBangLevel				= 0;


		SQLBindCol(m_pODBCStmt2->m_hstmt, 1, SQL_C_ULONG, &byPCBangUID,									0,			&arrCB[1]);
		SQLBindCol(m_pODBCStmt2->m_hstmt, 2, SQL_C_CHAR, byPCBangName,				sizeof(byPCBangName),			&arrCB[2]);
		SQLBindCol(m_pODBCStmt2->m_hstmt, 3, SQL_C_ULONG, &byTotalPCBangPlayTime,						0,			&arrCB[3]);
		SQLBindCol(m_pODBCStmt2->m_hstmt, 4, SQL_C_CHAR, byPCBangTel,				sizeof(byPCBangTel),			&arrCB[4]);
		SQLBindCol(m_pODBCStmt2->m_hstmt, 5, SQL_C_CHAR, byPCBangZipCode,			sizeof(byPCBangZipCode),		&arrCB[5]);
		SQLBindCol(m_pODBCStmt2->m_hstmt, 6, SQL_C_CHAR, byPCBangAddr_Sido,			sizeof(byPCBangAddr_Sido),		&arrCB[6]);
		SQLBindCol(m_pODBCStmt2->m_hstmt, 7, SQL_C_CHAR, byPCBangAddr_SiGuGun,		sizeof(byPCBangAddr_SiGuGun),	&arrCB[7]);
		SQLBindCol(m_pODBCStmt2->m_hstmt, 8, SQL_C_CHAR, byPCBangAddr_Dong,			sizeof(byPCBangAddr_Dong),		&arrCB[8]);
		SQLBindCol(m_pODBCStmt2->m_hstmt, 9, SQL_C_CHAR, byPCBangAddr_Detail,		sizeof(byPCBangAddr_Detail),	&arrCB[9]);
		SQLBindCol(m_pODBCStmt2->m_hstmt, 10, SQL_C_UTINYINT, &byPCBangLevel,							0,	&arrCB[10]);

// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
// 		szSQLQuery.Format("select TotalTime.PCBangUID, PCBang.Branch_Name, TotalTime.TotalPCBangPlayTime, \
// 							PCBang.Branch_Tel, PCBang.ZipCode, PCBang.Addr_Sido, PCBang.Addr_SiGuGun, \
// 							PCBang.Addr_Dong, PCBang.Addr_Detail, PCBang.User_Level \
// 							from \
// 							dbo.PCBangPlayTime AS TotalTime INNER JOIN dbo.td_PCBang AS PCBang \
// 							on TotalTime.PCBangUID = PCBang.UID");

// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
//		szSQLQuery.Format(QUERY_080702_0129);
//		bRet = m_pODBCStmt2->ExecuteQuery((LPCSTR)szSQLQuery);
		bRet = m_pODBCStmt2->ExecuteQuery((char*)(PROCEDURE_080827_0129));		
		if (!bRet)
		{
			m_pODBCStmt2->FreeStatement();			// clean up
			return FALSE;
		}
		
		// DB에 값이 없을때까지 loop를 돈다
		while ( (bRet = SQLFetch(m_pODBCStmt2->m_hstmt)) != SQL_NO_DATA)
		{
			if(m_edit_PCBangLevel > byPCBangLevel)
			{// 2007-06-25 by dhjin, PC방 등급보다 낮으면 저장하지 않는다.
				continue;
			}

			PCBangTotal.m_PCBangUID				= byPCBangUID;
			PCBangTotal.m_PCBangName			= byPCBangName;
			PCBangTotal.m_TotalPCBangPlayTime	= byTotalPCBangPlayTime;
			PCBangTotal.m_PCBangTel				= byPCBangTel;
			PCBangTotal.m_PCBangZipCode			= byPCBangZipCode;
			PCBangTotal.m_PCBangAddr_Sido		= byPCBangAddr_Sido;
			PCBangTotal.m_PCBangAddr_SiGuGun	= byPCBangAddr_SiGuGun;
			PCBangTotal.m_PCBangAddr_Dong		= byPCBangAddr_Dong;
			PCBangTotal.m_PCBangAddr_Detail		= byPCBangAddr_Detail;
			PCBangTotal.m_PCBangLevel			= byPCBangLevel;					// 2007-06-25 by dhjin

			vector<m_PCBangTotal>::iterator  itr = m_lPCBangTotal.begin();
			while (itr != m_lPCBangTotal.end())
			{// 2007-01-23 by dhjin, 존재하는 PCBangUID이면 총 플레이타임을 더한다.
				if ( (itr->m_PCBangUID) == byPCBangUID )
				{
					itr->m_TotalPCBangPlayTime	= itr->m_TotalPCBangPlayTime+byTotalPCBangPlayTime;
					m_lPCBangTotalCheck = 1;
					break;
				}
				itr++;
			}

			if (m_lPCBangTotalCheck == 0)
			{// 2007-01-23 by dhjin, m_lPCBangTotal 존재하지 않으면 추가
				m_lPCBangTotal.push_back(PCBangTotal);
			}

			m_lPCBangTotalCheck = 0;		// 2007-01-23 by dhjin, Check초기화
		}
		m_pODBCStmt2->FreeStatement();
	}//	for(int sernum=1; sernum <= SIZE_MAX_SERVER_GROUP_COUNT; sernum++)
	m_pODBCStmt2->Clean();
	
	return TRUE;
}

void CStatisticPage101Dlg::DisPlayPCBangTotalPlayTime()
{
	m_GridStaPCBang.SetBkColor(0xFFFFFF);
	m_GridStaPCBang.SetHeaderSort();

	m_GridStaPCBang.SetEditable(FALSE);
	m_GridStaPCBang.SetListMode(TRUE);
	m_GridStaPCBang.SetSingleRowSelection(TRUE);
	m_GridStaPCBang.EnableSelection(TRUE);
	m_GridStaPCBang.SetFrameFocusCell(FALSE);
	m_GridStaPCBang.SetTrackFocusCell(FALSE);

	m_GridStaPCBang.SetRowCount(1);
	m_GridStaPCBang.SetColumnCount(11);
	m_GridStaPCBang.SetFixedRowCount(1);

	// 칼럼 만들기
	GV_ITEM Item;
	Item.mask = GVIF_TEXT|GVIF_FORMAT;
	Item.nFormat = DT_LEFT|DT_VCENTER|DT_SINGLELINE;
	Item.row = 0;

	Item.col = 0;
	Item.strText.Format("NUM");
	m_GridStaPCBang.SetItem(&Item);
	Item.col = 1;
	Item.strText.Format("UID");
	m_GridStaPCBang.SetItem(&Item);
	Item.col = 2;
	Item.strText.Format("Level");
	m_GridStaPCBang.SetItem(&Item);
	Item.col = 3;
	Item.strText.Format("PCBangName");
	m_GridStaPCBang.SetItem(&Item);
	Item.col = 4;
	Item.strText.Format("Total Connected Time");
	m_GridStaPCBang.SetItem(&Item);
	Item.col = 5;
	Item.strText.Format("Tel");
	m_GridStaPCBang.SetItem(&Item);
	Item.col = 6;
	Item.strText.Format("ZipCode");
	m_GridStaPCBang.SetItem(&Item);
	Item.col = 7;
	Item.strText.Format("Addr1");
	m_GridStaPCBang.SetItem(&Item);
	Item.col = 8;
	Item.strText.Format("Addr2");
	m_GridStaPCBang.SetItem(&Item);
	Item.col = 9;
	Item.strText.Format("Addr3");
	m_GridStaPCBang.SetItem(&Item);
	Item.col = 10;
	Item.strText.Format("Addr4");
	m_GridStaPCBang.SetItem(&Item);

	if (TRUE == m_ctl_bIncrease)
	{
		sort(m_lPCBangTotal.begin(), m_lPCBangTotal.end() ,PCBangTotalPlayTime_Sort_ObjectIncrease());
	}
	
	if (TRUE == m_ctl_bDecrease)
	{
		sort(m_lPCBangTotal.begin(), m_lPCBangTotal.end() ,PCBangTotalPlayTime_Sort_ObjectDecrease());
	}
	
	vector<m_PCBangTotal>::iterator  itr = m_lPCBangTotal.begin();
	for (int i=0; i < m_edit_MaxRankCount; i++)
	//while (itr != m_lPCBangTotal.end())
	{
		if (itr == m_lPCBangTotal.end())
		{
			break;
		}
		
		int nNewRowIdx = m_GridStaPCBang.GetRowCount();
		m_GridStaPCBang.SetRowCount(nNewRowIdx+1);
		
		//select된 값을 GridDetail GridCtrl에 넣어준다.
		Item.row		= nNewRowIdx;
		Item.col		= 0;
		Item.strText.Format("%d", nNewRowIdx);
		m_GridStaPCBang.SetItem(&Item);
		
		Item.col		= 1;
		Item.strText.Format("%d", itr->m_PCBangUID);
		m_GridStaPCBang.SetItem(&Item);

		Item.col		= 2;
		Item.strText.Format("%d", itr->m_PCBangLevel);
		m_GridStaPCBang.SetItem(&Item);
		
		Item.col		= 3;
		Item.strText.Format("%s", itr->m_PCBangName);
		m_GridStaPCBang.SetItem(&Item);
		
		Item.col		= 4;
		Item.strText.Format("%d", itr->m_TotalPCBangPlayTime);
		m_GridStaPCBang.SetItem(&Item);
		
		Item.col		= 5;
		Item.strText.Format("%s", itr->m_PCBangTel);
		m_GridStaPCBang.SetItem(&Item);
		
		Item.col		= 6;
		Item.strText.Format("%s", itr->m_PCBangZipCode);
		m_GridStaPCBang.SetItem(&Item);
		
		Item.col		= 7;
		Item.strText.Format("%s", itr->m_PCBangAddr_Sido);
		m_GridStaPCBang.SetItem(&Item);
		
		Item.col		= 8;
		Item.strText.Format("%s", itr->m_PCBangAddr_SiGuGun);
		m_GridStaPCBang.SetItem(&Item);	
		
		Item.col		= 9;
		Item.strText.Format("%s", itr->m_PCBangAddr_Dong);
		m_GridStaPCBang.SetItem(&Item);	
		
		Item.col		= 10;
		Item.strText.Format("%s", itr->m_PCBangAddr_Detail);
		m_GridStaPCBang.SetItem(&Item);	

		itr++;
	}
	m_GridStaPCBang.UpdateData();

	m_GridStaPCBang.AutoSize();
}

BOOL CStatisticPage101Dlg::GetPCBangPlayTimeDailyData()
{
// 2007-11-07 by cmkwon, 로그 DB 서버 따로 구축하기 - 하나의 연결로 처리
//	if (!m_pODBCStmt2->Init(m_pMainDlg->m_pServerInfo4Admin->DBIP, m_pMainDlg->m_pServerInfo4Admin->DBPort, m_pMainDlg->m_pServerInfo4Admin->DBName,
//							m_pMainDlg->m_pServerInfo4Admin->DBUID, m_pMainDlg->m_pServerInfo4Admin->DBPWD, GetSafeHwnd())
//	)
//	{
//		return FALSE;
//	}

	m_PCBangTotalByDaily PCBangTotalByDaily;
	
	CString		tDBNAME;
	CString		szStartDate, szEndDate; 

	CDAILYDATE			CurrentDate;
	CString				PreCheck;
	CString				CurrentCheck;
	CString				NextStandardCheck;
	ATUM_DATE_TIME		tempDate;
	int					m_lPCBangTotalByDailyCheck = 0;

// 2008-01-24 by cmkwon, S_A: 날짜 관련 로컬리 제이션 처리 버그 수정 - 아래와 같은 형식으로 수정		
// 	szStartDate.Format("%04d-%02d-%02d %02d:%02d:%02d",
// 		m_ctl_CreatedDateStart.GetYear(),m_ctl_CreatedDateStart.GetMonth(),m_ctl_CreatedDateStart.GetDay(),
// 		m_ctl_CreatedTimeStart.GetHour(),m_ctl_CreatedTimeStart.GetMinute(),m_ctl_CreatedTimeStart.GetSecond());
// 	szEndDate.Format("%04d-%02d-%02d %02d:%02d:%02d",
// 		m_ctl_CreatedDateEnd.GetYear(),m_ctl_CreatedDateEnd.GetMonth(),m_ctl_CreatedDateEnd.GetDay(),
// 		m_ctl_CreatedTimeEnd.GetHour(),m_ctl_CreatedTimeEnd.GetMinute(),m_ctl_CreatedTimeEnd.GetSecond());
// 2008-08-28 by cmkwon, MySQL 관련 날짜시간 관련 오류 수정 - 
//	szStartDate.Format("%04d%02d%02d %02d:%02d:%02d",
	szStartDate.Format(SQL_DATETIME_STRING_FORMAT,
		m_ctl_CreatedDateStart.GetYear(),m_ctl_CreatedDateStart.GetMonth(),m_ctl_CreatedDateStart.GetDay(),
		m_ctl_CreatedTimeStart.GetHour(),m_ctl_CreatedTimeStart.GetMinute(),m_ctl_CreatedTimeStart.GetSecond());
// 2008-08-28 by cmkwon, MySQL 관련 날짜시간 관련 오류 수정 - 
//	szEndDate.Format("%04d%02d%02d %02d:%02d:%02d",
	szEndDate.Format(SQL_DATETIME_STRING_FORMAT,
		m_ctl_CreatedDateEnd.GetYear(),m_ctl_CreatedDateEnd.GetMonth(),m_ctl_CreatedDateEnd.GetDay(),
		m_ctl_CreatedTimeEnd.GetHour(),m_ctl_CreatedTimeEnd.GetMinute(),m_ctl_CreatedTimeEnd.GetSecond());

	CString szSQLQuery;

	for(int sernum=1; sernum <= SIZE_MAX_SERVER_GROUP_COUNT; sernum++)
	{
		// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
		//szSQLQuery.Format("use atum2_db_%d", sernum);
		szSQLQuery.Format(QUERY_080702_0130, sernum);
		BOOL bRet = m_pODBCStmt2->ExecuteQuery((LPCSTR)szSQLQuery);
		if (!bRet)
		{
			m_pODBCStmt2->FreeStatement();			// clean up
			continue;
		}
		
		// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
		//szSQLQuery.Format("drop view Log_PCBangTotalPlayTimeByEachDay");
		szSQLQuery.Format(QUERY_080702_0131);
// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
//		bRet = m_pODBCStmt2->ExecuteQuery((LPCSTR)szSQLQuery);
		SQLHSTMT hstmt = m_pODBCStmt2->GetSTMTHandle();
		SQLINTEGER arrCB2[2] = {SQL_NTS,SQL_NTS};
		SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SZQUERY, 0, (LPSTR)(LPCSTR)szSQLQuery, 0,			&arrCB2[1]);
		bRet = m_pODBCStmt2->ExecuteQuery((char*)(PROCEDURE_080827_0122));


		// 2007-01-23 by dhjin, backupLog, Log를 합친 뷰를 생성
// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
// 		szSQLQuery.Format("Create View dbo.Log_PCBangTotalPlayTimeByEachDay	\
// 							as \
// 							select Time, PCBangUID , PCBangPlayTime \
// 							from dbo.atum_backup_log_user_game_start_end WITH (NOLOCK) \
// 							where LogType = 20 and Time >= '%s' and Time <= '%s' \
// 							and PCBangUID IN \
// 							(select UID from dbo.td_PCBang WITH (NOLOCK)) \
// 							UNION \
// 							select Time, PCBangUID , PCBangPlayTime \
// 							from dbo.atum_log_user_game_start_end WITH (NOLOCK) \
// 							where LogType = 20 and Time >= '%s' and Time <= '%s' \
// 							and PCBangUID IN \
// 							(select UID from dbo.td_PCBang WITH (NOLOCK))"
// 							, szStartDate,szEndDate, szStartDate,szEndDate);

		szSQLQuery.Format(QUERY_080702_0132, szStartDate,szEndDate, szStartDate,szEndDate);
// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
//		bRet = m_pODBCStmt2->ExecuteQuery((LPCSTR)szSQLQuery);
		hstmt = m_pODBCStmt2->GetSTMTHandle();
		arrCB2[1] = SQL_NTS;
		SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SZQUERY, 0, (LPSTR)(LPCSTR)szSQLQuery, 0,			&arrCB2[1]);
		bRet = m_pODBCStmt2->ExecuteQuery((char*)(PROCEDURE_080827_0122));
		if (!bRet)
		{
			m_pODBCStmt2->FreeStatement();		// clean up
			return FALSE;
		}
		
		SQLINTEGER arrCB[4]={SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS};
		SQL_TIMESTAMP_STRUCT		tmSqlTime;
		util::zero(&tmSqlTime, sizeof(tmSqlTime));
		UID32_t  byPCBangUID				= 0;
		INT		 byTotalPCBangPlayTime		= 0;

		SQLBindCol(m_pODBCStmt2->m_hstmt, 1, SQL_C_TIMESTAMP, &tmSqlTime,				0,			&arrCB[1]);
		SQLBindCol(m_pODBCStmt2->m_hstmt, 2, SQL_C_ULONG, &byPCBangUID,					0,			&arrCB[1]);
		SQLBindCol(m_pODBCStmt2->m_hstmt, 3, SQL_C_ULONG, &byTotalPCBangPlayTime,		0,			&arrCB[3]);

		// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
		//szSQLQuery.Format("Select Time, PCBangUID, PCBangPlayTime \
		//					From dbo.Log_PCBangTotalPlayTimeByEachDay \
		//					WITH (NOLOCK)");

// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
//		szSQLQuery.Format(QUERY_080702_0133);
//		bRet = m_pODBCStmt2->ExecuteQuery((LPCSTR)szSQLQuery);
		bRet = m_pODBCStmt2->ExecuteQuery((char*)(PROCEDURE_080827_0133));
		if (!bRet)
		{
			m_pODBCStmt2->FreeStatement();			// clean up
			return FALSE;
		}
		
		// DB에 값이 없을때까지 loop를 돈다
		while ( (bRet = SQLFetch(m_pODBCStmt2->m_hstmt)) != SQL_NO_DATA)
		{
			tempDate				= tmSqlTime;
			CurrentDate.Year		= tempDate.Year;
			CurrentDate.Month		= tempDate.Month;
			CurrentDate.Day			= tempDate.Day;
		
			CTime Curenttime(CurrentDate.Year,CurrentDate.Month,CurrentDate.Day,0,0,0);
			CurrentCheck.Format(_T("%4d-%02d-%02d"),Curenttime.GetYear(),Curenttime.GetMonth(),
								Curenttime.GetDay());
			
//			CTime Pretime(CurrentDate.Year,CurrentDate.Month,CurrentDate.Day,(0 - 24),0,0);
//			PreCheck.Format(_T("%4d-%02d-%02d"),Pretime.GetYear(),Pretime.GetMonth(),
//								Pretime.GetDay());	
//			
//			CTime NextStandardtime(CurrentDate.Year,CurrentDate.Month,CurrentDate.Day, (0 + 24),0,0);
//			NextStandardCheck.Format(_T("%4d-%02d-%02d"),NextStandardtime.GetYear(),NextStandardtime.GetMonth(),
//									NextStandardtime.GetDay());

			PCBangTotalByDaily.m_PCBangDate				= CurrentCheck;
			PCBangTotalByDaily.m_PCBangUID				= byPCBangUID;
			PCBangTotalByDaily.m_PCBangPlayTime			= byTotalPCBangPlayTime;

			list<m_PCBangTotalByDaily>::iterator  itr = m_lPCBangTotalByDaily.begin();
			while (itr != m_lPCBangTotalByDaily.end())
			{// 2007-01-23 by dhjin, 전날과 다음날을 비교하여 그 사이 날이면 플레이시간 더한다.
//				if ( (itr->m_PCBangDate) > PreCheck && (itr->m_PCBangDate) < NextStandardCheck )
				if ( (itr->m_PCBangDate) == CurrentCheck )
				{
					itr->m_PCBangPlayTime			= itr->m_PCBangPlayTime+byTotalPCBangPlayTime;
					itr->m_Count				= itr->m_Count+1;
					m_lPCBangTotalByDailyCheck = 1;
					break;
				}
				itr++;
			}

			if (m_lPCBangTotalByDailyCheck == 0)
			{// 2007-01-23 by dhjin, m_lPCBangTotal 존재하지 않으면 추가
				PCBangTotalByDaily.m_Count	= 1;
				m_lPCBangTotalByDaily.push_back(PCBangTotalByDaily);
			}
		
			m_lPCBangTotalByDailyCheck = 0;		// 2007-01-23 by dhjin, Check초기화
		}
		m_pODBCStmt2->FreeStatement();
	}//	for(int sernum=1; sernum <= SIZE_MAX_SERVER_GROUP_COUNT; sernum++)

	return TRUE;
}

void CStatisticPage101Dlg::DisPlayPCBangTotalPlayTimeDaily()
{
	m_GridStaPCBangDaily.SetBkColor(0xFFFFFF);
	m_GridStaPCBangDaily.SetHeaderSort();

	m_GridStaPCBangDaily.SetEditable(FALSE);
	m_GridStaPCBangDaily.SetListMode(TRUE);
	m_GridStaPCBangDaily.SetSingleRowSelection(TRUE);
	m_GridStaPCBangDaily.EnableSelection(TRUE);
	m_GridStaPCBangDaily.SetFrameFocusCell(FALSE);
	m_GridStaPCBangDaily.SetTrackFocusCell(FALSE);

	m_GridStaPCBangDaily.SetRowCount(1);
	m_GridStaPCBangDaily.SetColumnCount(4);
	m_GridStaPCBangDaily.SetFixedRowCount(1);

	// 칼럼 만들기
	GV_ITEM Item;
	Item.mask = GVIF_TEXT|GVIF_FORMAT;
	Item.nFormat = DT_LEFT|DT_VCENTER|DT_SINGLELINE;
	Item.row = 0;

	Item.col = 0;
	Item.strText.Format("Num");
	m_GridStaPCBangDaily.SetItem(&Item);
	Item.col = 1;
	Item.strText.Format("Daily");
	m_GridStaPCBangDaily.SetItem(&Item);
	Item.col = 2;
	Item.strText.Format("AVE(S)");
	m_GridStaPCBangDaily.SetItem(&Item);	
	Item.col = 3;
	Item.strText.Format("Total(S)");
	m_GridStaPCBangDaily.SetItem(&Item);	

	list<m_PCBangTotalByDaily>::iterator  itr = m_lPCBangTotalByDaily.begin();
	while (itr != m_lPCBangTotalByDaily.end())
	{
		int nNewRowIdx = m_GridStaPCBangDaily.GetRowCount();
		m_GridStaPCBangDaily.SetRowCount(nNewRowIdx+1);
		
		//select된 값을 GridDetail GridCtrl에 넣어준다.
		Item.row		= nNewRowIdx;
		Item.col		= 0;
		Item.strText.Format("%d", nNewRowIdx);
		m_GridStaPCBangDaily.SetItem(&Item);
		
		Item.col		= 1;
		Item.strText.Format("%s", itr->m_PCBangDate);
		m_GridStaPCBangDaily.SetItem(&Item);
		
		Item.col		= 2;
		Item.strText.Format("%d", itr->m_PCBangPlayTime/itr->m_Count);
		m_GridStaPCBangDaily.SetItem(&Item);
		
		Item.col		= 3;
		Item.strText.Format("%d", itr->m_PCBangPlayTime);
		m_GridStaPCBangDaily.SetItem(&Item);
	
		itr++;
	}
	m_GridStaPCBangDaily.UpdateData();

	m_GridStaPCBangDaily.AutoSize();

}

void CStatisticPage101Dlg::OnIncreaseSort()
{
	UpdateData(TRUE);

	if(TRUE == m_ctl_bDecrease)
	{
		m_ctl_bDecrease = FALSE;
	}

	m_ctl_bIncrease = TRUE;

	UpdateData(FALSE);
}

void CStatisticPage101Dlg::OnDecreaseSort()
{
	UpdateData(TRUE);

	if(TRUE == m_ctl_bIncrease)
	{
		m_ctl_bIncrease = FALSE;
	}

	m_ctl_bDecrease = TRUE;	
	UpdateData(FALSE);
}

void CStatisticPage101Dlg::OnButtonSt101Save() 
{

	// TODO: Add your control notification handler code here
	CString tempStr;
	for(int i = 0; i<m_GridStaPCBang.GetRowCount();i++)
	{
		tempStr = "";
		for(int j = 0; j< m_GridStaPCBang.GetColumnCount();j++)
		{
			tempStr += m_GridStaPCBang.GetItemText( i, j);
			tempStr += "|\t";
		}
		m_pMainDlg->m_pWriteLog->WriteLogFile((char*)(LPCTSTR)tempStr, tempStr.GetLength(), m_pMainDlg, 1);
	}	
}
