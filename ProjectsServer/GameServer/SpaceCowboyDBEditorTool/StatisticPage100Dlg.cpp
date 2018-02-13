// StatisticPage100Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "spacecowboydbeditortool.h"
#include "StatisticPage100Dlg.h"
#include "SpaceCowboyDBEditorToolDlg.h"
#include <time.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStatisticPage100Dlg property page

IMPLEMENT_DYNCREATE(CStatisticPage100Dlg, CPropertyPage)

CStatisticPage100Dlg::CStatisticPage100Dlg() : CPropertyPage(CStatisticPage100Dlg::IDD)
{
	//{{AFX_DATA_INIT(CStatisticPage100Dlg)
	m_ctl_CreatedDateStart = COleDateTime::GetCurrentTime();
	m_ctl_CreatedTimeStart = COleDateTime::GetCurrentTime();
	m_ctl_CreatedDateEnd = COleDateTime::GetCurrentTime();
	m_ctl_CreatedTimeEnd = COleDateTime::GetCurrentTime();
	m_10Min = 0;
	m_20Min = 0;
	m_2Hour = 0;
	m_30Min = 0;
	m_3Hour = 0;
	m_4Hour = 0;
	m_5Hour = 0;
	m_60Min = 0;
	m_6Hour = 0;
	m_6UpHour = 0;
	m_90Min = 0;
	m_Sido;
	m_GunGu;
	m_PCBangName;
	m_ctl_bPCBang = FALSE;
	//}}AFX_DATA_INIT
	m_pMainDlg = (CSpaceCowboyDBEditorToolDlg*)AfxGetMainWnd();
	m_pODBCStmt						= m_pMainDlg->m_pODBCStmt;
}

CStatisticPage100Dlg::~CStatisticPage100Dlg()
{
	m_lUserStayTime.clear();
	m_lUserStayDaily.clear();
}

void CStatisticPage100Dlg::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStatisticPage100Dlg)
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER1, m_ctl_CreatedDateStart);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER2, m_ctl_CreatedTimeStart);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER3, m_ctl_CreatedDateEnd);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER4, m_ctl_CreatedTimeEnd);
	DDX_Text(pDX, IDC_EDIT_10Min, m_10Min);
	DDX_Text(pDX, IDC_EDIT_20Min, m_20Min);
	DDX_Text(pDX, IDC_EDIT_2Hour, m_2Hour);
	DDX_Text(pDX, IDC_EDIT_30Min, m_30Min);
	DDX_Text(pDX, IDC_EDIT_3Hour, m_3Hour);
	DDX_Text(pDX, IDC_EDIT_4Hour, m_4Hour);
	DDX_Text(pDX, IDC_EDIT_5Hour, m_5Hour);
	DDX_Text(pDX, IDC_EDIT_60Min, m_60Min);
	DDX_Text(pDX, IDC_EDIT_6Hour, m_6Hour);
	DDX_Text(pDX, IDC_EDIT_6UpHour, m_6UpHour);
	DDX_Text(pDX, IDC_EDIT_90Min, m_90Min);
	DDX_Text(pDX, IDC_EDIT_Sido, m_Sido);
	DDX_Text(pDX, IDC_EDIT_GunGu, m_GunGu);
	DDX_Text(pDX, IDC_EDIT_PCBangName, m_PCBangName);
	DDX_Control(pDX, IDC_CUSTOM_STA_USERSTAYTIME, m_GridStaUserStayTime);
	DDX_Control(pDX, IDC_CUSTOM_UserStayDaily, m_GridStaUserStayDaily);
	DDX_Check(pDX, IDC_CHECK_PCBANG, m_ctl_bPCBang);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CStatisticPage100Dlg, CPropertyPage)
	//{{AFX_MSG_MAP(CStatisticPage100Dlg)
	ON_BN_CLICKED(IDC_BUTTON_ST100_SELECT, OnButtonSt100Select)
	ON_BN_CLICKED(IDC_CHECK_PCBANG, OnCheckPcbang)
	ON_BN_CLICKED(IDC_BUTTON_ST100_SAVE, OnButtonSt100Save)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStatisticPage100Dlg message handlers

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CStatisticPage100Dlg::OnButtonSt100Select() 
/// \brief		
/// \author		dhjin
/// \date		2006-08-07 ~ 2006-08-07
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CStatisticPage100Dlg::OnButtonSt100Select() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);


	m_lUserStayTime.clear();

	if (FALSE == GetUserStayTimeData(m_ctl_bPCBang))
	{
		return;
	}

	DisplayUserStayTime();
	DisplayUserStayDaily();
	DisplayUserEachTime();
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CStatisticPage100Dlg::Szformat()
/// \brief		
/// \author		dhjin
/// \date		2007-01-23 ~ 2007-01-23
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CStatisticPage100Dlg::Szformat()
{
// 2008-01-24 by cmkwon, S_A: 날짜 관련 로컬리 제이션 처리 버그 수정 - 형식을 아래와 같이 수정
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
	
	if (m_Sido.GetLength() == 0)
	{
		szSido = '%';
	}
	else
	{
		szSido.Format("%s",m_Sido);
	}

	if (m_GunGu.GetLength() == 0)
	{
		szGuGun = '%';
	}
	else
	{
		szGuGun.Format("%s",m_GunGu);
	}

	if (m_PCBangName.GetLength() == 0)
	{
		szPCbangName = '%';
	}
	else
	{
		szPCbangName.Format("%s",m_PCBangName);
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CStatisticPage100Dlg::GetUserStayTimeData(BOOL i_bPCBangUser)
/// \brief		// 2006-12-18 by cmkwon, 인자 추가(BOOL i_bPCBangUser)
/// \author		dhjin
/// \date		2006-08-07 ~ 2006-08-07
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CStatisticPage100Dlg::GetUserStayTimeData(BOOL i_bPCBangUser)
{
	CString szSQLQuery;
	// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
	//szSQLQuery.Format("drop view dbo.Game_Log_Start_End");

	szSQLQuery.Format(QUERY_080702_0122);
// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
//	BOOL bRet = m_pODBCStmt->ExecuteQuery((LPCSTR)szSQLQuery);
	SQLHSTMT hstmt = m_pODBCStmt->GetSTMTHandle();
	SQLINTEGER arrCB2[2] = {SQL_NTS,SQL_NTS};
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SZQUERY, 0, (LPSTR)(LPCSTR)szSQLQuery, 0,			&arrCB2[1]);
	BOOL bRet = m_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_080827_0122));
	m_pODBCStmt->FreeStatement();		// clean up

	Szformat();

	if(FALSE == i_bPCBangUser)
	{
		// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
		//szSQLQuery.Format("Create View dbo.Game_Log_Start_End \
		//					as \
		//					select Time, LogType, CharacterUniqueNumber, AccountName, CharacterName, TotalPlayTime, PlayTime, PCBangUID \
		//					from dbo.atum_backup_log_user_game_start_end WITH (NOLOCK) \
		//					where Time >= '%s' and Time <= '%s' \
		//					union \
		//					select Time, LogType, CharacterUniqueNumber, AccountName, CharacterName, TotalPlayTime, PlayTime, PCBangUID \
		//					from dbo.atum_log_user_game_start_end WITH (NOLOCK) \
		//					where Time >= '%s' and Time <= '%s'",szStartDate,szEndDate,szStartDate,szEndDate);
		szSQLQuery.Format(QUERY_080702_0123,szStartDate,szEndDate,szStartDate,szEndDate);
	}
	else
	{
		// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
		//szSQLQuery.Format("Create View dbo.Game_Log_Start_End \
		//					as \
		//					select Time, LogType, CharacterUniqueNumber, AccountName, CharacterName, TotalPlayTime, PCBangPlayTime AS PlayTime, PCBangUID \
		//					from dbo.atum_backup_log_user_game_start_end WITH (NOLOCK) \
		//					where Time >= '%s' and Time <= '%s' \
		//					and PCBangUID IN \
		//					(select UID from dbo.td_PCBang WITH (NOLOCK) \
		//					where Addr_Sido like '%%%s%%' and Addr_SiGuGun like '%%%s%%' and Branch_Name like '%%%s%%') \
		//					union \
		//					select Time, LogType, CharacterUniqueNumber, AccountName, CharacterName, TotalPlayTime, PCBangPlayTime AS PlayTime, PCBangUID \
		//					from dbo.atum_log_user_game_start_end WITH (NOLOCK) \
		//					where Time >= '%s' and Time <= '%s' \
		//					and PCBangUID IN \
		//					(select UID from dbo.td_PCBang WITH (NOLOCK) \
		//					where Addr_Sido like '%%%s%%' and Addr_SiGuGun like '%%%s%%' and Branch_Name like '%%%s%%')",
		//					szStartDate,szEndDate,szSido,szGuGun,szPCbangName,szStartDate,szEndDate,szSido,szGuGun,szPCbangName);
		szSQLQuery.Format(QUERY_080702_0124, szStartDate,szEndDate,szSido,szGuGun,szPCbangName,szStartDate,szEndDate,szSido,szGuGun,szPCbangName);
	}

// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
//	bRet = m_pODBCStmt->ExecuteQuery((LPCSTR)szSQLQuery);
	hstmt = m_pODBCStmt->GetSTMTHandle();
	arrCB2[1] = SQL_NTS;
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SZQUERY, 0, (LPSTR)(LPCSTR)szSQLQuery, 0,			&arrCB2[1]);
	bRet = m_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_080827_0122));
	if (!bRet)
	{
		m_pODBCStmt->FreeStatement();		// clean up
		MessageBox("Error");
		return FALSE;
	}

	SQLINTEGER arrCB[9]={SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS, SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS};
	SQLCHAR  byAccountName[256]=" ,";
	SQLCHAR	 byCharacterName[256]=" ,";	
	SQLCHAR  byStartTime[30]=" ,";
	SQLCHAR	 byEndTime[30]=" ,";
	INT		 byStayTime = 0;
	INT		 byLevel	= 0;
	INT		 byPCBangUID= 0;
	SQLCHAR	 byCreatedTime[30]={NULL, };		// 2007-07-12 by cmkwon, 검색 결과에 캐릭터 생성시간 추가함
		
	SQLBindCol(m_pODBCStmt->m_hstmt, 1, SQL_C_CHAR, byAccountName,		sizeof(byAccountName),		&arrCB[1]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 2, SQL_C_CHAR, byCharacterName,	sizeof(byCharacterName),	&arrCB[2]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 3, SQL_C_ULONG, &byLevel,			0,							&arrCB[3]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 4, SQL_C_CHAR, byStartTime,		sizeof(byStartTime),		&arrCB[4]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 5, SQL_C_CHAR, byEndTime,			sizeof(byEndTime),			&arrCB[5]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 6, SQL_C_ULONG, &byStayTime,		0,							&arrCB[6]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 7, SQL_C_ULONG, &byPCBangUID,		0,							&arrCB[7]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 8, SQL_C_CHAR, byCreatedTime,		30,							&arrCB[8]);		// 2007-07-12 by cmkwon, 검색 결과에 캐릭터 생성시간 추가함
	

	CString strTemp;	
	// 2007-07-12 by cmkwon, 검색 결과에 캐릭터 생성시간 추가함 
	// 2007-08-09 by cmkwon, 삭제된 캐릭터(RACE_DELETED_CHARACTER) 처리 - 검색 결과에서 제외 한다(0 = c.Race&%d AND )
// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
// 	strTemp.Format("select a.AccountName, a.CharacterName, c.Level, a.START_TIME, b.END_TIME, b.PlayTime, b.PCBangUID, c.CreatedTime \
// 				   from \
// 				   (select CharacterUniqueNumber, AccountName, CharacterName, Time as START_TIME, TotalPlayTime \
// 				   from dbo.Game_Log_Start_End \
// 				   where LogType = 19) as a, \
// 				   (select CharacterUniqueNumber, AccountName, CharacterName, Time as END_TIME, TotalPlayTime, PlayTime, PCBangUID \
// 				   from dbo.Game_Log_Start_End \
// 				   where LogType = 20) as b, \
// 				   dbo.td_Character as c \
// 				   where 0 = c.Race&%d AND a.CharacterUniqueNumber = b.CharacterUniqueNumber  \
// 				   and (b.TotalPlayTime - a.TotalPlayTime) = PlayTime  \
// 				   and a.CharacterUniqueNumber = c.UniqueNumber \
// 				   and a.START_TIME < b.END_TIME \
//				   order by  a.START_TIME", RACE_DELETED_CHARACTER); 

// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
//	strTemp.Format(QUERY_080702_0125, RACE_DELETED_CHARACTER); 
//	szSQLQuery = strTemp;
//	bRet = m_pODBCStmt->ExecuteQuery((LPCSTR)szSQLQuery);
	bRet = m_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_080827_0125));
	if (!bRet)
	{

		m_pODBCStmt->FreeStatement();		// clean up
		MessageBox("Error");
		return FALSE;
	}

	_m_UserStayTime TempStayTime;

	// DB에 값이 없을때까지 loop를 돈다
	while ( (bRet = SQLFetch(m_pODBCStmt->m_hstmt)) != SQL_NO_DATA)
	{
		TempStayTime.m_AccountName		= byAccountName;
		TempStayTime.m_CharacterName	= byCharacterName;
		TempStayTime.m_StartTime		= byStartTime;
		TempStayTime.m_EndTime			= byEndTime;
		TempStayTime.m_StayTime			= byStayTime;
		TempStayTime.m_Level			= byLevel;
		TempStayTime.m_PCBangUID		= byPCBangUID;
		TempStayTime.m_CharacterCreatedTime	= byCreatedTime;		// 2007-07-12 by cmkwon, 검색 결과에 캐릭터 생성시간 추가함
		m_lUserStayTime.push_back(TempStayTime);
	}

	// clean up
	m_pODBCStmt->FreeStatement();
	
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CStatisticPage100Dlg::DisplayUserStayTime()
/// \brief		
/// \author		dhjin
/// \date		2006-08-07 ~ 2006-08-07
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CStatisticPage100Dlg::DisplayUserStayTime()
{

	m_GridStaUserStayTime.SetBkColor(0xFFFFFF);
	m_GridStaUserStayTime.SetHeaderSort();

	m_GridStaUserStayTime.SetEditable(FALSE);
	m_GridStaUserStayTime.SetListMode(TRUE);
	m_GridStaUserStayTime.SetSingleRowSelection(TRUE);
	m_GridStaUserStayTime.EnableSelection(TRUE);
	m_GridStaUserStayTime.SetFrameFocusCell(FALSE);
	m_GridStaUserStayTime.SetTrackFocusCell(FALSE);

	m_GridStaUserStayTime.SetRowCount(1);
	m_GridStaUserStayTime.SetColumnCount(9);		// 2007-07-12 by cmkwon, 검색 결과에 캐릭터 생성시간 추가함 - 그리드컨트롤에 컬럼추가
	m_GridStaUserStayTime.SetFixedRowCount(1);

	// 칼럼 만들기
	GV_ITEM Item;
	Item.mask = GVIF_TEXT|GVIF_FORMAT;
	Item.nFormat = DT_LEFT|DT_VCENTER|DT_SINGLELINE;
	Item.row = 0;

	Item.col = 0;
	Item.strText.Format("Num");
	m_GridStaUserStayTime.SetItem(&Item);
	Item.col = 1;
	Item.strText.Format("AccountName");
	m_GridStaUserStayTime.SetItem(&Item);
	Item.col = 2;
	Item.strText.Format("CharacterName");
	m_GridStaUserStayTime.SetItem(&Item);
	Item.col = 3;
	Item.strText.Format("StartTime");
	m_GridStaUserStayTime.SetItem(&Item);
	Item.col = 4;
	Item.strText.Format("EndTime");
	m_GridStaUserStayTime.SetItem(&Item);
	Item.col = 5;
	Item.strText.Format("Level");
	m_GridStaUserStayTime.SetItem(&Item);
	Item.col = 6;
	if(FALSE == m_ctl_bPCBang)
	{
		Item.strText.Format("StayTime(S)");
	}
	else
	{
		Item.strText.Format("PCB_StayTime(S)");
	}
	m_GridStaUserStayTime.SetItem(&Item);
	Item.col = 7;
	Item.strText.Format("PCB_UID");
	m_GridStaUserStayTime.SetItem(&Item);

	// 2007-07-12 by cmkwon, 검색 결과에 캐릭터 생성시간 추가함 - 그리드컨트롤 추가된 컬럼 이름
	Item.col = 8;
	Item.strText.Format("Character created time");
	m_GridStaUserStayTime.SetItem(&Item);

	list<_m_UserStayTime>::iterator  itr = m_lUserStayTime.begin();
	while (itr != m_lUserStayTime.end())
	{
		int nNewRowIdx = m_GridStaUserStayTime.GetRowCount();
		m_GridStaUserStayTime.SetRowCount(nNewRowIdx+1);
		
		//select된 값을 GridDetail GridCtrl에 넣어준다.
		Item.row		= nNewRowIdx;
		Item.col		= 0;
		Item.strText.Format("%d", nNewRowIdx);
		m_GridStaUserStayTime.SetItem(&Item);
		
		Item.col		= 1;
		Item.strText.Format("%s", itr->m_AccountName);
		m_GridStaUserStayTime.SetItem(&Item);
		
		Item.col		= 2;
		Item.strText.Format("%s", itr->m_CharacterName);
		m_GridStaUserStayTime.SetItem(&Item);
		
		Item.col		= 3;
		Item.strText.Format("%s", itr->m_StartTime);
		m_GridStaUserStayTime.SetItem(&Item);
		
		Item.col		= 4;
		Item.strText.Format("%s", itr->m_EndTime);
		m_GridStaUserStayTime.SetItem(&Item);
		
		Item.col		= 5;
		Item.strText.Format("%d", itr->m_Level);
		m_GridStaUserStayTime.SetItem(&Item);
		
		Item.col		= 6;
		Item.strText.Format("%d", itr->m_StayTime);
		m_GridStaUserStayTime.SetItem(&Item);
		
		Item.col		= 7;
		Item.strText.Format("%d", itr->m_PCBangUID);
		m_GridStaUserStayTime.SetItem(&Item);	
		
		// 2007-07-12 by cmkwon, 검색 결과에 캐릭터 생성시간 추가함 - 그리드컨트롤에 추가된 컬럼 결과 출력
		Item.col		= 8;
		Item.strText.Format("%s", itr->m_CharacterCreatedTime);
		m_GridStaUserStayTime.SetItem(&Item);	
		itr++;
	}
	m_GridStaUserStayTime.UpdateData();

	m_GridStaUserStayTime.AutoSize();

}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CStatisticPage100Dlg::DisplayUserStayDaily()
/// \brief		
/// \author		dhjin
/// \date		2006-08-07 ~ 2006-08-07
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CStatisticPage100Dlg::DisplayUserStayDaily()
{
	_m_UserStayDaily	TempUserStayDaily;
	CDAILYDATE			CurrentDate;
	CString				tempDate;
	CString				CurrentCheck;
	CString				NextStandardCheck;
	char *				tt;
	int					DayStayTime				= 0;
	int					DayStayPeopleCount		= 0;
	
	list<_m_UserStayTime>::iterator  itr = m_lUserStayTime.begin();
	while (itr != m_lUserStayTime.end())
	{
		tempDate.Format(_T("%s"),itr->m_StartTime);//m_DBlistData[serNum]에 시간 값.
		tt						= LPSTR(LPCTSTR(tempDate)); 
		CurrentDate.Year		= atoi(strtok(tt,"-"));
		CurrentDate.Month		= atoi(strtok(NULL,"-"));
		CurrentDate.Day			= atoi(strtok(NULL," "));
		
		CTime Curenttime(CurrentDate.Year,CurrentDate.Month,CurrentDate.Day,0,0,0);
		CurrentCheck.Format(_T("%4d-%02d-%02d %02d:%02d:%02d"),Curenttime.GetYear(),Curenttime.GetMonth(),
								Curenttime.GetDay(),Curenttime.GetHour(),Curenttime.GetMinute(),
								Curenttime.GetSecond());
		
		CTime NextStandardtime(CurrentDate.Year,CurrentDate.Month,CurrentDate.Day, (0 + 24),0,0);
		NextStandardCheck.Format(_T("%4d-%02d-%02d %02d:%02d:%02d"),NextStandardtime.GetYear(),NextStandardtime.GetMonth(),
									NextStandardtime.GetDay(),NextStandardtime.GetHour(),
									NextStandardtime.GetMinute(),NextStandardtime.GetSecond());

		for ( ; (itr->m_StartTime) < NextStandardCheck; itr++ )
		{
			DayStayTime = DayStayTime + itr->m_StayTime;
			DayStayPeopleCount++;
	
			if ( ++itr == m_lUserStayTime.end())
			{
				break;
			}
			itr--;
	
		}

		CurrentCheck.Format(_T("%4d-%02d-%02d"),Curenttime.GetYear(),Curenttime.GetMonth(),
								Curenttime.GetDay());

		TempUserStayDaily.m_StartTime	= CurrentCheck;
		TempUserStayDaily.m_AveStayTime = DayStayTime / DayStayPeopleCount;
		TempUserStayDaily.m_TotalStayTime = DayStayTime;
		m_lUserStayDaily.push_back(TempUserStayDaily);
		
		DayStayTime			= 0;
		DayStayPeopleCount	= 0;
	}

	m_GridStaUserStayDaily.SetBkColor(0xFFFFFF);
	m_GridStaUserStayDaily.SetHeaderSort();

	m_GridStaUserStayDaily.SetEditable(FALSE);
	m_GridStaUserStayDaily.SetListMode(TRUE);
	m_GridStaUserStayDaily.SetSingleRowSelection(TRUE);
	m_GridStaUserStayDaily.EnableSelection(TRUE);
	m_GridStaUserStayDaily.SetFrameFocusCell(FALSE);
	m_GridStaUserStayDaily.SetTrackFocusCell(FALSE);

	m_GridStaUserStayDaily.SetRowCount(1);
	m_GridStaUserStayDaily.SetColumnCount(4);
	m_GridStaUserStayDaily.SetFixedRowCount(1);

	// 칼럼 만들기
	GV_ITEM Item;
	Item.mask = GVIF_TEXT|GVIF_FORMAT;
	Item.nFormat = DT_LEFT|DT_VCENTER|DT_SINGLELINE;
	Item.row = 0;

	Item.col = 0;
	Item.strText.Format("Num");
	m_GridStaUserStayDaily.SetItem(&Item);
	Item.col = 1;
	Item.strText.Format("Daily");
	m_GridStaUserStayDaily.SetItem(&Item);
	Item.col = 2;
	Item.strText.Format("AVE(S)");
	m_GridStaUserStayDaily.SetItem(&Item);	
	Item.col = 3;
	Item.strText.Format("Total(S)");
	m_GridStaUserStayDaily.SetItem(&Item);	

	list<_m_UserStayDaily>::iterator  itrD = m_lUserStayDaily.begin();
	while (itrD != m_lUserStayDaily.end())
	{
		int nNewRowIdx = m_GridStaUserStayDaily.GetRowCount();
		m_GridStaUserStayDaily.SetRowCount(nNewRowIdx+1);
		
		//select된 값을 GridDetail GridCtrl에 넣어준다.
		Item.row		= nNewRowIdx;
		Item.col		= 0;
		Item.strText.Format("%d", nNewRowIdx);
		m_GridStaUserStayDaily.SetItem(&Item);
		
		Item.col		= 1;
		Item.strText.Format("%s", itrD->m_StartTime);
		m_GridStaUserStayDaily.SetItem(&Item);
		
		Item.col		= 2;
		Item.strText.Format("%d", itrD->m_AveStayTime);
		m_GridStaUserStayDaily.SetItem(&Item);
		
		Item.col		= 3;
		Item.strText.Format("%d", itrD->m_TotalStayTime);
		m_GridStaUserStayDaily.SetItem(&Item);
		
		itrD++;
	}
	m_GridStaUserStayDaily.UpdateData();

	m_GridStaUserStayDaily.AutoSize();
	
	m_lUserStayDaily.clear();
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CStatisticPage100Dlg::DisplayUserEachTime()
/// \brief		
/// \author		dhjin
/// \date		2006-08-07 ~ 2006-08-07
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CStatisticPage100Dlg::DisplayUserEachTime()
{
	int m_Temp10Min = 0;
	int m_Temp20Min = 0;
	int m_Temp30Min = 0;
	int m_Temp60Min = 0;
	int m_Temp90Min = 0;
	int m_Temp2Hour = 0;
	int m_Temp3Hour = 0;
	int m_Temp4Hour = 0;
	int m_Temp5Hour = 0;
	int m_Temp6Hour = 0;
	int m_Temp6UpHour = 0;

	list<_m_UserStayTime>::iterator  itr = m_lUserStayTime.begin();
	while (itr != m_lUserStayTime.end())
	{
		if ( (itr->m_StayTime >= 0) && (itr->m_StayTime < 600 ) )
		{
			m_Temp10Min++;
		}
		
		else if ( (itr->m_StayTime >= 600) && (itr->m_StayTime < 1200 ) )
		{
			m_Temp20Min++;
		}
		
		else if ( (itr->m_StayTime >= 1200) && (itr->m_StayTime < 1800 ) )
		{
			m_Temp30Min++;
		}

		else if ( (itr->m_StayTime >= 1800) && (itr->m_StayTime < 3600 ) )
		{
			m_Temp60Min++;
		}
		
		else if ( (itr->m_StayTime >= 3600) && (itr->m_StayTime < 5400 ) )
		{
			m_Temp90Min++;
		}

		else if ( (itr->m_StayTime >= 5400) && (itr->m_StayTime < 7200 ) )
		{
			m_Temp2Hour++;
		}

		else if ( (itr->m_StayTime >= 7200) && (itr->m_StayTime < 10800 ) )
		{
			m_Temp3Hour++;
		}

		else if ( (itr->m_StayTime >= 10800) && (itr->m_StayTime < 14400 ) )
		{
			m_Temp4Hour++;
		}

		else if ( (itr->m_StayTime >= 14400) && (itr->m_StayTime < 18000 ) )
		{
			m_Temp5Hour++;
		}

		else if ( (itr->m_StayTime >= 18000) && (itr->m_StayTime < 21600 ) )
		{
			m_Temp6Hour++;
		}

		else if ( itr->m_StayTime >= 21600 )
		{
			m_Temp6UpHour++;
		}

		itr++;
	}

	m_10Min		= m_Temp10Min;
	m_20Min		= m_Temp20Min;
	m_30Min		= m_Temp30Min;
	m_60Min		= m_Temp60Min;
	m_90Min		= m_Temp90Min;
	m_2Hour		= m_Temp2Hour;
	m_3Hour		= m_Temp3Hour;
	m_4Hour		= m_Temp4Hour;
	m_5Hour		= m_Temp5Hour;
	m_6Hour		= m_Temp6Hour;
	m_6UpHour	= m_Temp6UpHour;

	UpdateData(FALSE);
	UpdateData(TRUE);

}

void CStatisticPage100Dlg::OnCheckPcbang() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
}

void CStatisticPage100Dlg::OnButtonSt100Save() 
{
	// TODO: Add your control notification handler code here
	CString tempStr;
	for(int i = 0; i<m_GridStaUserStayTime.GetRowCount();i++)
	{
		tempStr = "";
		for(int j = 0; j< m_GridStaUserStayTime.GetColumnCount();j++)
		{
			tempStr += m_GridStaUserStayTime.GetItemText( i, j);
			tempStr += "|\t";
		}
		m_pMainDlg->m_pWriteLog->WriteLogFile((char*)(LPCTSTR)tempStr, tempStr.GetLength(), m_pMainDlg, 1);
	}
}
