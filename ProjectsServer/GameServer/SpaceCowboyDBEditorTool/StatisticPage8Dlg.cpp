// StatisticPage8Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "spacecowboydbeditortool.h"
#include "StatisticPage8Dlg.h"
#include "SpaceCowboyDBEditorToolDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStatisticPage8Dlg property page

IMPLEMENT_DYNCREATE(CStatisticPage8Dlg, CPropertyPage)

CStatisticPage8Dlg::CStatisticPage8Dlg() : CPropertyPage(CStatisticPage8Dlg::IDD)
{
	//{{AFX_DATA_INIT(CStatisticPage8Dlg)
	m_valueCheckDailyMission = FALSE;
	m_valueCheckSpecMission = FALSE;
	m_valueDTByDayMissionStartDate = COleDateTime::GetCurrentTime();
	m_valueDTByDayMissionEndDate = COleDateTime::GetCurrentTime();
	m_valueDTByDayMissionStartTime = COleDateTime::GetCurrentTime();
	m_valueDTByDayMissionEndTime = COleDateTime::GetCurrentTime();
	m_valueDTBySpecMissionStartDate = COleDateTime::GetCurrentTime();
	m_valueDTBySpecMissionEndDate = COleDateTime::GetCurrentTime();
	m_valueDTBySpecMissionStartTime = COleDateTime::GetCurrentTime();
	m_valueDTBySpecMissionEndTime = COleDateTime::GetCurrentTime();
	m_ctlnMissionNumber = 0;
	m_ctlnResultCount = 0;
	m_ctlbProgressMission = FALSE;
	//}}AFX_DATA_INIT
	m_pMainDlg = (CSpaceCowboyDBEditorToolDlg*)AfxGetMainWnd();
	m_pODBCStmt				= m_pMainDlg->m_pODBCStmt;
}

CStatisticPage8Dlg::~CStatisticPage8Dlg()
{
// 2006-01-23 by cmkwon
//	util::del(m_pODBCStmt);
}

void CStatisticPage8Dlg::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStatisticPage8Dlg)
	DDX_Check(pDX, IDC_CHECK1, m_valueCheckDailyMission);
	DDX_Check(pDX, IDC_CHECK2, m_valueCheckSpecMission);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_ST3_DAILY_CONNECTION_STARTDATE, m_valueDTByDayMissionStartDate);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_ST3_DAILY_CONNECTION_ENDDATE, m_valueDTByDayMissionEndDate);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_ST1_DAILY_ACCOUNT_STARTTIME8, m_valueDTByDayMissionStartTime);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_ST1_DAILY_ACCOUNT_ENDTIME8, m_valueDTByDayMissionEndTime);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_ST3_DAILY_CONNECTION_STARTDATE2, m_valueDTBySpecMissionStartDate);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_ST3_DAILY_CONNECTION_ENDDATE2, m_valueDTBySpecMissionEndDate);
	DDX_DateTimeCtrl(pDX, IDC_TIMEPICKER_SPECMISSION_START, m_valueDTBySpecMissionStartTime);
	DDX_DateTimeCtrl(pDX, IDC_TIMEPICKER_SPECMISSION_END, m_valueDTBySpecMissionEndTime);
	DDX_Text(pDX, IDC_EDIT_MISSION_NUM, m_ctlnMissionNumber);
	DDX_Text(pDX, IDC_EDIT_RESULT_COUNT, m_ctlnResultCount);
	DDX_Control(pDX, IDC_CUSTOM_STA_MISSION, m_GridStaMission);
	DDX_Check(pDX, IDC_CHECK3, m_ctlbProgressMission);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CStatisticPage8Dlg, CPropertyPage)
	//{{AFX_MSG_MAP(CStatisticPage8Dlg)
	ON_BN_CLICKED(IDC_BUTTON_ST8_SELECT, OnButtonSt8Select)
	ON_BN_CLICKED(IDC_BUTTON_ST8_SAVE, OnButtonSt8Save)
	ON_BN_CLICKED(IDC_CHECK1, OnCheck1)
	ON_BN_CLICKED(IDC_CHECK2, OnCheck2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStatisticPage8Dlg message handlers

void CStatisticPage8Dlg::OnButtonSt8Select() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if(m_valueCheckDailyMission)
	{
		ViewDailyMission();
	}
	if(m_valueCheckSpecMission)
	{
		if(m_ctlbProgressMission)
		{
			ViewSpecProgressMission(m_ctlnMissionNumber);
		}
		else
		{
			ViewSpecCompleteMission(m_ctlnMissionNumber);
		}
	}
}

void CStatisticPage8Dlg::ViewDailyMission(void)
{
	m_GridStaMission.SetBkColor(0xFFFFFF);
	m_GridStaMission.SetHeaderSort();

	int m_nRows = 1;
	int m_nCols = 2;

	m_GridStaMission.SetEditable(FALSE);
	m_GridStaMission.SetListMode(TRUE);
	m_GridStaMission.SetSingleRowSelection(TRUE);
	m_GridStaMission.EnableSelection(TRUE);
	m_GridStaMission.SetFrameFocusCell(FALSE);
	m_GridStaMission.SetTrackFocusCell(FALSE);

	m_GridStaMission.SetRowCount(m_nRows);
	m_GridStaMission.SetColumnCount(m_nCols);
	m_GridStaMission.SetFixedRowCount(1);

	// 칼럼 만들기
	m_nCols = 0;
	GV_ITEM Item;
	Item.mask = GVIF_TEXT|GVIF_FORMAT;
	Item.row = 0;
	Item.nFormat = DT_LEFT|DT_VCENTER|DT_SINGLELINE;

	Item.col = m_nCols++;
	Item.strText.Format("DATE");
	m_GridStaMission.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("COUNT");
	m_GridStaMission.SetItem(&Item);

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
// 	szStartDate.Format("%04d-%02d-%02d %02d:%02d:%02d", m_valueDTByDayMissionStartDate.GetYear(), m_valueDTByDayMissionStartDate.GetMonth(), m_valueDTByDayMissionStartDate.GetDay()
// 		, m_valueDTByDayMissionStartTime.GetHour(), m_valueDTByDayMissionStartTime.GetMinute(), m_valueDTByDayMissionStartTime.GetSecond());
// 	szEndDate.Format("%04d-%02d-%02d %02d:%02d:%02d", m_valueDTByDayMissionEndDate.GetYear(), m_valueDTByDayMissionEndDate.GetMonth(), m_valueDTByDayMissionEndDate.GetDay()
// 		, m_valueDTByDayMissionEndTime.GetHour(), m_valueDTByDayMissionEndTime.GetMinute(), m_valueDTByDayMissionEndTime.GetSecond());
// 2008-08-28 by cmkwon, MySQL 관련 날짜시간 관련 오류 수정 - 
//	szStartDate.Format("%04d%02d%02d %02d:%02d:%02d", m_valueDTByDayMissionStartDate.GetYear(), m_valueDTByDayMissionStartDate.GetMonth(), m_valueDTByDayMissionStartDate.GetDay()
	szStartDate.Format(SQL_DATETIME_STRING_FORMAT, m_valueDTByDayMissionStartDate.GetYear(), m_valueDTByDayMissionStartDate.GetMonth(), m_valueDTByDayMissionStartDate.GetDay()
		, m_valueDTByDayMissionStartTime.GetHour(), m_valueDTByDayMissionStartTime.GetMinute(), m_valueDTByDayMissionStartTime.GetSecond());
// 2008-08-28 by cmkwon, MySQL 관련 날짜시간 관련 오류 수정 - 
//	szEndDate.Format("%04d%02d%02d %02d:%02d:%02d", m_valueDTByDayMissionEndDate.GetYear(), m_valueDTByDayMissionEndDate.GetMonth(), m_valueDTByDayMissionEndDate.GetDay()
	szEndDate.Format(SQL_DATETIME_STRING_FORMAT, m_valueDTByDayMissionEndDate.GetYear(), m_valueDTByDayMissionEndDate.GetMonth(), m_valueDTByDayMissionEndDate.GetDay()
		, m_valueDTByDayMissionEndTime.GetHour(), m_valueDTByDayMissionEndTime.GetMinute(), m_valueDTByDayMissionEndTime.GetSecond());

	CString szSQLQuery;
	// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
	//szSQLQuery.Format("DROP VIEW Temp_View");
// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
//	szSQLQuery.Format(QUERY_080702_0213);
//	BOOL bRet = m_pODBCStmt->ExecuteQuery((LPCSTR)szSQLQuery);
	BOOL bRet = m_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_080827_0213));
	m_pODBCStmt->FreeStatement();
	
// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
// 	szSQLQuery.Format("CREATE VIEW Temp_View AS \
// 		SELECT * FROM  atum_backup_log_user_quest_pk_dead WITH (NOLOCK) WHERE logtype = 21 and time > '%s' and time < '%s' \
// 		UNION ALL \
// 		SELECT * FROM  atum_log_user_quest_pk_dead WITH (NOLOCK) WHERE logtype = 21 and time > '%s' and time < '%s'", szStartDate, szEndDate, szStartDate, szEndDate);
// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
	szSQLQuery.Format(QUERY_080702_0214, szStartDate, szEndDate, szStartDate, szEndDate);
//	bRet = m_pODBCStmt->ExecuteQuery((LPCSTR)szSQLQuery);
	SQLBindParameter(m_pODBCStmt->m_hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SZQUERY, 0, (LPSTR)(LPCSTR)szSQLQuery, 0, NULL);
	bRet = m_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_080827_0214));
	m_pODBCStmt->FreeStatement();

	// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
	//szSQLQuery.Format("select date, count(*) from (select time, dbo.atum_getonlydate(time) as date	from Temp_View l	where logtype = 21 and time > '%s' and time < '%s') ll group by date order by date"
	//	,szStartDate, szEndDate);
// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
//	szSQLQuery.Format(QUERY_080702_0215,szStartDate, szEndDate);
	SQLBindParameter(m_pODBCStmt->m_hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SQL_DATETIME_STRING, 0, (LPSTR)(LPCSTR)szStartDate, 0, NULL);
	SQLBindParameter(m_pODBCStmt->m_hstmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SQL_DATETIME_STRING, 0, (LPSTR)(LPCSTR)szEndDate, 0, NULL);
	bRet = m_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_080827_0215));
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



	m_ctlnResultCount		= 0;
	int nCount = 0;
	m_GridStaMission.SetRowCount(nCount+1);
	// DB에 값이 없을때까지 loop를 돈다
	while ( (bRet = SQLFetch(m_pODBCStmt->m_hstmt)) != SQL_NO_DATA)
	{
		m_GridStaMission.SetRowCount(nCount+2);

		//select된 값을 GridDetail GridCtrl에 넣어준다.
		Item.col = 0;
		Item.row = nCount+1;
		Item.strText.Format("%s", tempDate);
		m_GridStaMission.SetItem(&Item);

		Item.col = 1;
		Item.strText.Format("%d", tempSelectData);
		m_GridStaMission.SetItem(&Item);
		m_GridStaMission.UpdateData();
		m_ctlnResultCount += tempSelectData;

		nCount++;
	}

	// clean up
	m_pODBCStmt->FreeStatement();

	m_GridStaMission.AutoSize();

	UpdateData(FALSE);
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			void CStatisticPage8Dlg::ViewSpecCompleteMission(int i_nQuestIndex)
/// \brief		
/// \author		cmkwon
/// \date		2006-03-08 ~ 2006-03-08
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CStatisticPage8Dlg::ViewSpecCompleteMission(int i_nQuestIndex)
{
	m_GridStaMission.SetBkColor(0xFFFFFF);
	m_GridStaMission.SetHeaderSort();

	int m_nRows = 1;
	int m_nCols = 7;

	m_GridStaMission.SetEditable(FALSE);
	m_GridStaMission.SetListMode(TRUE);
	m_GridStaMission.SetSingleRowSelection(TRUE);
	m_GridStaMission.EnableSelection(TRUE);
	m_GridStaMission.SetFrameFocusCell(FALSE);
	m_GridStaMission.SetTrackFocusCell(FALSE);

	m_GridStaMission.SetRowCount(m_nRows);
	m_GridStaMission.SetColumnCount(m_nCols);
	m_GridStaMission.SetFixedRowCount(1);

	// 칼럼 만들기
	GV_ITEM Item;
	Item.mask = GVIF_TEXT|GVIF_FORMAT;
	Item.nFormat = DT_LEFT|DT_VCENTER|DT_SINGLELINE;
	Item.row = 0;

	Item.col = 0;
	Item.strText.Format("Num");
	m_GridStaMission.SetItem(&Item);
	Item.col = 1;
	Item.strText.Format("Account Name");
	m_GridStaMission.SetItem(&Item);
	Item.col = 2;
	Item.strText.Format("Character Name");
	m_GridStaMission.SetItem(&Item);
	Item.col = 3;
	Item.strText.Format("Unitkind");
	m_GridStaMission.SetItem(&Item);
	Item.col = 4;
	Item.strText.Format("InfluenceType");
	m_GridStaMission.SetItem(&Item);
	Item.col = 5;
	Item.strText.Format("Level");
	m_GridStaMission.SetItem(&Item);
	Item.col = 6;
	Item.strText.Format("Mission Complete Time");
	m_GridStaMission.SetItem(&Item);

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
// 	szStartDate.Format("%04d-%02d-%02d %02d:%02d:%02d", m_valueDTBySpecMissionStartDate.GetYear(), m_valueDTBySpecMissionStartDate.GetMonth(), m_valueDTBySpecMissionStartDate.GetDay()
// 		, m_valueDTBySpecMissionStartTime.GetHour(), m_valueDTBySpecMissionStartTime.GetMinute(), m_valueDTBySpecMissionStartTime.GetSecond());
// 	szEndDate.Format("%04d-%02d-%02d %02d:%02d:%02d", m_valueDTBySpecMissionEndDate.GetYear(), m_valueDTBySpecMissionEndDate.GetMonth(), m_valueDTBySpecMissionEndDate.GetDay()
// 		, m_valueDTBySpecMissionEndTime.GetHour(), m_valueDTBySpecMissionEndTime.GetMinute(), m_valueDTBySpecMissionEndTime.GetSecond());
// 2008-08-28 by cmkwon, MySQL 관련 날짜시간 관련 오류 수정 - 
//	szStartDate.Format("%04d%02d%02d %02d:%02d:%02d", m_valueDTBySpecMissionStartDate.GetYear(), m_valueDTBySpecMissionStartDate.GetMonth(), m_valueDTBySpecMissionStartDate.GetDay()
	szStartDate.Format(SQL_DATETIME_STRING_FORMAT, m_valueDTBySpecMissionStartDate.GetYear(), m_valueDTBySpecMissionStartDate.GetMonth(), m_valueDTBySpecMissionStartDate.GetDay()
		, m_valueDTBySpecMissionStartTime.GetHour(), m_valueDTBySpecMissionStartTime.GetMinute(), m_valueDTBySpecMissionStartTime.GetSecond());
// 2008-08-28 by cmkwon, MySQL 관련 날짜시간 관련 오류 수정 - 
//	szEndDate.Format("%04d%02d%02d %02d:%02d:%02d", m_valueDTBySpecMissionEndDate.GetYear(), m_valueDTBySpecMissionEndDate.GetMonth(), m_valueDTBySpecMissionEndDate.GetDay()
	szEndDate.Format(SQL_DATETIME_STRING_FORMAT, m_valueDTBySpecMissionEndDate.GetYear(), m_valueDTBySpecMissionEndDate.GetMonth(), m_valueDTBySpecMissionEndDate.GetDay()
		, m_valueDTBySpecMissionEndTime.GetHour(), m_valueDTBySpecMissionEndTime.GetMinute(), m_valueDTBySpecMissionEndTime.GetSecond());

	CString szSQLQuery;
// 2006-07-12 by cmkwon, td_character 테이블 참조로 변경
// 	szSQLQuery.Format("DROP VIEW TmView_SpecQuest");
// 	BOOL bRet = m_pODBCStmt->ExecuteQuery((LPCSTR)szSQLQuery);
// 	m_pODBCStmt->FreeStatement();
// 	
// 	szSQLQuery.Format("CREATE VIEW TmView_SpecQuest AS \
// 		SELECT * FROM  atum_backup_log_user_quest_pk_dead WHERE logtype = %d and Param1= %d and  time > '%s' and time < '%s' \
// 		UNION ALL \
// 		SELECT * FROM  atum_log_user_quest_pk_dead WHERE logtype = %d and Param1= %d and time > '%s' and time < '%s'"
// 		, T1_FL_LOG_QUEST_COMPLETION, i_nQuestIndex, szStartDate, szEndDate
// 		, T1_FL_LOG_QUEST_COMPLETION, i_nQuestIndex, szStartDate, szEndDate);
// 	bRet = m_pODBCStmt->ExecuteQuery((LPCSTR)szSQLQuery);
// 	m_pODBCStmt->FreeStatement();
// 
// 	szSQLQuery.Format("SELECT c.AccountName, c.CharacterName, atum.atum_GetGearString(c.unitkind), c.InfluenceType, c.level, tmV.Time \
// 		FROM td_character c, TmView_SpecQuest tmV \
// 		WHERE c.uniqueNumber = tmV.CharacterUniqueNumber");

	// 2007-08-09 by cmkwon, 삭제된 캐릭터 처리(RACE_DELETED_CHARACTER) - 검색 결과에서 제외 한다(0 = c.Race&%d AND )
// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
// 	szSQLQuery.Format("SELECT c.AccountName, c.CharacterName, dbo.atum_GetGearString(c.unitkind), c.InfluenceType, c.level, q.QuestTimeStamp \
// 		FROM td_character c, td_characterQuest q WITH (NOLOCK) \
// 		WHERE q.QuestIndex = %d and q.QuestState = 2 and q.QuestTimeStamp > \'%s\' and q.QuestTimeStamp < \'%s\' and q.CharacterUniqueNumber = c.uniqueNumber AND 0 = c.Race&%d\
// 		ORDER BY q.QuestTimeStamp",	i_nQuestIndex, szStartDate, szEndDate, RACE_DELETED_CHARACTER);
// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
//	szSQLQuery.Format(QUERY_080702_0216, i_nQuestIndex, szStartDate, szEndDate, RACE_DELETED_CHARACTER);
	char tmStartDate[SIZE_MAX_SQL_DATETIME_STRING];
	sprintf(tmStartDate, SQL_DATETIME_STRING_FORMAT, m_valueDTBySpecMissionStartDate.GetYear(), m_valueDTBySpecMissionStartDate.GetMonth(), m_valueDTBySpecMissionStartDate.GetDay(), m_valueDTBySpecMissionStartDate.GetHour(), m_valueDTBySpecMissionStartDate.GetMinute(), m_valueDTBySpecMissionStartDate.GetSecond());
	char tmEndDate[SIZE_MAX_SQL_DATETIME_STRING];
	sprintf(tmEndDate, SQL_DATETIME_STRING_FORMAT, m_valueDTBySpecMissionEndDate.GetYear(), m_valueDTBySpecMissionEndDate.GetMonth(), m_valueDTBySpecMissionEndDate.GetDay(), m_valueDTBySpecMissionEndDate.GetHour(), m_valueDTBySpecMissionEndDate.GetMinute(), m_valueDTBySpecMissionEndDate.GetSecond());
	SQLBindParameter(m_pODBCStmt->m_hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &i_nQuestIndex, 0, NULL);
	SQLBindParameter(m_pODBCStmt->m_hstmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SQL_DATETIME_STRING, 0, tmStartDate, 0, NULL);
	SQLBindParameter(m_pODBCStmt->m_hstmt, 3, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SQL_DATETIME_STRING, 0, tmEndDate, 0, NULL);
	USHORT	tmRaceDeletedCharacter = RACE_DELETED_CHARACTER;
	SQLBindParameter(m_pODBCStmt->m_hstmt, 4, SQL_PARAM_INPUT, SQL_C_USHORT, SQL_SMALLINT, 0, 0, &tmRaceDeletedCharacter, 0, NULL);
	BOOL bRet = m_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_080827_0216));
	if (!bRet)
	{
		// clean up
		m_pODBCStmt->FreeStatement();
		MessageBox("Error");
		return;
	}

	SQLINTEGER arrCB[7]={SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS, SQL_NTS,SQL_NTS};
	char szAccName[SIZE_MAX_ACCOUNT_NAME];
	char szCharName[SIZE_MAX_CHARACTER_NAME];
	char szUnitkind[10];
	BYTE byInfluenceType = 0;
	BYTE byLevel = 0;
	char szDate[SIZE_MAX_SQL_DATETIME_STRING];

	SQLBindCol(m_pODBCStmt->m_hstmt, 1, SQL_C_CHAR, szAccName, SIZE_MAX_ACCOUNT_NAME,		&arrCB[1]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 2, SQL_C_CHAR, szCharName, SIZE_MAX_CHARACTER_NAME,	&arrCB[2]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 3, SQL_C_CHAR, szUnitkind, 10,							&arrCB[3]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 4, SQL_C_TINYINT, &byInfluenceType, 0,					&arrCB[4]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 5, SQL_C_TINYINT, &byLevel, 0,							&arrCB[5]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 6, SQL_C_CHAR, &szDate, SIZE_MAX_SQL_DATETIME_STRING,	&arrCB[6]);

	// DB에 값이 없을때까지 loop를 돈다
	while ( (bRet = SQLFetch(m_pODBCStmt->m_hstmt)) != SQL_NO_DATA)
	{
		int nNewRowIdx = m_GridStaMission.GetRowCount();
		m_GridStaMission.SetRowCount(nNewRowIdx+1);

		//select된 값을 GridDetail GridCtrl에 넣어준다.
		Item.row		= nNewRowIdx;
		Item.col		= 0;
		Item.strText.Format("%d", nNewRowIdx);
		m_GridStaMission.SetItem(&Item);

		Item.col		= 1;
		Item.strText.Format("%s", szAccName);
		m_GridStaMission.SetItem(&Item);

		Item.col		= 2;
		Item.strText.Format("%s", szCharName);
		m_GridStaMission.SetItem(&Item);

		Item.col		= 3;
		Item.strText.Format("%s", szUnitkind);
		m_GridStaMission.SetItem(&Item);

		Item.col		= 4;
		Item.strText.Format("%s", CAtumSJ::GetInfluenceTypeString(byInfluenceType));
		m_GridStaMission.SetItem(&Item);

		Item.col		= 5;
		Item.strText.Format("%d", byLevel);
		m_GridStaMission.SetItem(&Item);
		
		Item.col		= 6;
		Item.strText.Format("%s", szDate);
		m_GridStaMission.SetItem(&Item);
	}
	m_GridStaMission.UpdateData();

	m_pODBCStmt->FreeStatement();	// clean up

	m_GridStaMission.AutoSize();

	m_ctlnResultCount = m_GridStaMission.GetRowCount() - 1;
	UpdateData(FALSE);
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			void CStatisticPage8Dlg::ViewSpecProgressMission(int i_nQuestIndex)
/// \brief		
/// \author		cmkwon
/// \date		2006-03-09 ~ 2006-03-09
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CStatisticPage8Dlg::ViewSpecProgressMission(int i_nQuestIndex)
{
	m_GridStaMission.SetBkColor(0xFFFFFF);
	m_GridStaMission.SetHeaderSort();

	int m_nRows = 1;
	int m_nCols = 7;

	m_GridStaMission.SetEditable(FALSE);
	m_GridStaMission.SetListMode(TRUE);
	m_GridStaMission.SetSingleRowSelection(TRUE);
	m_GridStaMission.EnableSelection(TRUE);
	m_GridStaMission.SetFrameFocusCell(FALSE);
	m_GridStaMission.SetTrackFocusCell(FALSE);

	m_GridStaMission.SetRowCount(m_nRows);
	m_GridStaMission.SetColumnCount(m_nCols);
	m_GridStaMission.SetFixedRowCount(1);

	// 칼럼 만들기
	GV_ITEM Item;
	Item.mask = GVIF_TEXT|GVIF_FORMAT;
	Item.nFormat = DT_LEFT|DT_VCENTER|DT_SINGLELINE;
	Item.row = 0;

	Item.col = 0;
	Item.strText.Format("Num");
	m_GridStaMission.SetItem(&Item);
	Item.col = 1;
	Item.strText.Format("Account Name");
	m_GridStaMission.SetItem(&Item);
	Item.col = 2;
	Item.strText.Format("Character Name");
	m_GridStaMission.SetItem(&Item);
	Item.col = 3;
	Item.strText.Format("Unitkind");
	m_GridStaMission.SetItem(&Item);
	Item.col = 4;
	Item.strText.Format("InfluenceType");
	m_GridStaMission.SetItem(&Item);
	Item.col = 5;
	Item.strText.Format("Level");
	m_GridStaMission.SetItem(&Item);
	Item.col = 6;
	Item.strText.Format("Mission Start Time");
	m_GridStaMission.SetItem(&Item);

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
// 	szStartDate.Format("%04d-%02d-%02d %02d:%02d:%02d", m_valueDTBySpecMissionStartDate.GetYear(), m_valueDTBySpecMissionStartDate.GetMonth(), m_valueDTBySpecMissionStartDate.GetDay()
// 		, m_valueDTBySpecMissionStartTime.GetHour(), m_valueDTBySpecMissionStartTime.GetMinute(), m_valueDTBySpecMissionStartTime.GetSecond());
// 	szEndDate.Format("%04d-%02d-%02d %02d:%02d:%02d", m_valueDTBySpecMissionEndDate.GetYear(), m_valueDTBySpecMissionEndDate.GetMonth(), m_valueDTBySpecMissionEndDate.GetDay()
// 		, m_valueDTBySpecMissionEndTime.GetHour(), m_valueDTBySpecMissionEndTime.GetMinute(), m_valueDTBySpecMissionEndTime.GetSecond());
// 2008-08-28 by cmkwon, MySQL 관련 날짜시간 관련 오류 수정 - 
//	szStartDate.Format("%04d%02d%02d %02d:%02d:%02d", m_valueDTBySpecMissionStartDate.GetYear(), m_valueDTBySpecMissionStartDate.GetMonth(), m_valueDTBySpecMissionStartDate.GetDay()
	szStartDate.Format(SQL_DATETIME_STRING_FORMAT, m_valueDTBySpecMissionStartDate.GetYear(), m_valueDTBySpecMissionStartDate.GetMonth(), m_valueDTBySpecMissionStartDate.GetDay()
		, m_valueDTBySpecMissionStartTime.GetHour(), m_valueDTBySpecMissionStartTime.GetMinute(), m_valueDTBySpecMissionStartTime.GetSecond());
// 2008-08-28 by cmkwon, MySQL 관련 날짜시간 관련 오류 수정 - 
//	szEndDate.Format("%04d%02d%02d %02d:%02d:%02d", m_valueDTBySpecMissionEndDate.GetYear(), m_valueDTBySpecMissionEndDate.GetMonth(), m_valueDTBySpecMissionEndDate.GetDay()
	szEndDate.Format(SQL_DATETIME_STRING_FORMAT, m_valueDTBySpecMissionEndDate.GetYear(), m_valueDTBySpecMissionEndDate.GetMonth(), m_valueDTBySpecMissionEndDate.GetDay()
		, m_valueDTBySpecMissionEndTime.GetHour(), m_valueDTBySpecMissionEndTime.GetMinute(), m_valueDTBySpecMissionEndTime.GetSecond());

	CString szSQLQuery;
// 2006-07-12 by cmkwon, 필요 없는 부분임
// 	szSQLQuery.Format("DROP VIEW TmView_SpecQuest");
// 	BOOL bRet = m_pODBCStmt->ExecuteQuery((LPCSTR)szSQLQuery);
// 	m_pODBCStmt->FreeStatement();
// 	
// 	szSQLQuery.Format("CREATE VIEW TmView_SpecQuest AS \
// 		SELECT * FROM  atum_backup_log_user_quest_pk_dead WHERE logtype = %d and Param1= %d and  time > '%s' and time < '%s' \
// 		UNION ALL \
// 		SELECT * FROM  atum_log_user_quest_pk_dead WHERE logtype = %d and Param1= %d and time > '%s' and time < '%s'"
// 		, T1_FL_LOG_QUEST_COMPLETION, i_nQuestIndex, szStartDate, szEndDate
// 		, T1_FL_LOG_QUEST_COMPLETION, i_nQuestIndex, szStartDate, szEndDate);
// 	bRet = m_pODBCStmt->ExecuteQuery((LPCSTR)szSQLQuery);
// 	m_pODBCStmt->FreeStatement();

	// 2007-08-09 by cmkwon, 삭제된 캐릭터 처리(RACE_DELETED_CHARACTER) - 검색 결과에서 제외 한다(0 = c.Race&%d AND )
// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
// 	szSQLQuery.Format("SELECT c.AccountName, c.CharacterName, dbo.atum_GetGearString(c.unitkind), c.InfluenceType, c.level, q.QuestTimeStamp \
// 		FROM td_character c, td_characterQuest q WITH (NOLOCK) \
// 		WHERE q.QuestIndex = %d and q.QuestState = 1 and q.QuestTimeStamp > \'%s\' and q.QuestTimeStamp < \'%s\' and q.CharacterUniqueNumber = c.uniqueNumber AND 0 = c.Race&%d\
// 		ORDER BY q.QuestTimeStamp",	i_nQuestIndex, szStartDate, szEndDate, RACE_DELETED_CHARACTER);
// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
//	szSQLQuery.Format(QUERY_080702_0217, i_nQuestIndex, szStartDate, szEndDate, RACE_DELETED_CHARACTER);
	char tmStartDate[SIZE_MAX_SQL_DATETIME_STRING];
	sprintf(tmStartDate, SQL_DATETIME_STRING_FORMAT, m_valueDTBySpecMissionStartDate.GetYear(), m_valueDTBySpecMissionStartDate.GetMonth(), m_valueDTBySpecMissionStartDate.GetDay(), m_valueDTBySpecMissionStartDate.GetHour(), m_valueDTBySpecMissionStartDate.GetMinute(), m_valueDTBySpecMissionStartDate.GetSecond());
	char tmEndDate[SIZE_MAX_SQL_DATETIME_STRING];
	sprintf(tmEndDate, SQL_DATETIME_STRING_FORMAT, m_valueDTBySpecMissionEndDate.GetYear(), m_valueDTBySpecMissionEndDate.GetMonth(), m_valueDTBySpecMissionEndDate.GetDay(), m_valueDTBySpecMissionEndDate.GetHour(), m_valueDTBySpecMissionEndDate.GetMinute(), m_valueDTBySpecMissionEndDate.GetSecond());
	SQLBindParameter(m_pODBCStmt->m_hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &i_nQuestIndex, 0, NULL);
	SQLBindParameter(m_pODBCStmt->m_hstmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SQL_DATETIME_STRING, 0, tmStartDate, 0, NULL);
	SQLBindParameter(m_pODBCStmt->m_hstmt, 3, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SQL_DATETIME_STRING, 0, tmEndDate, 0, NULL);
	USHORT	tmRaceDeletedCharacter = RACE_DELETED_CHARACTER;
	SQLBindParameter(m_pODBCStmt->m_hstmt, 4, SQL_PARAM_INPUT, SQL_C_USHORT, SQL_SMALLINT, 0, 0, &tmRaceDeletedCharacter, 0, NULL);
	BOOL bRet = m_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_080827_0217));
	if (!bRet)
	{
		// clean up
		m_pODBCStmt->FreeStatement();
		MessageBox("Error");
		return;
	}

	SQLINTEGER arrCB[7]={SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS, SQL_NTS,SQL_NTS};
	char szAccName[SIZE_MAX_ACCOUNT_NAME];
	char szCharName[SIZE_MAX_CHARACTER_NAME];
	char szUnitkind[10];
	BYTE byInfluenceType = 0;
	BYTE byLevel = 0;
	char szDate[SIZE_MAX_SQL_DATETIME_STRING];

	SQLBindCol(m_pODBCStmt->m_hstmt, 1, SQL_C_CHAR, szAccName, SIZE_MAX_ACCOUNT_NAME,		&arrCB[1]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 2, SQL_C_CHAR, szCharName, SIZE_MAX_CHARACTER_NAME,	&arrCB[2]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 3, SQL_C_CHAR, szUnitkind, 10,							&arrCB[3]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 4, SQL_C_TINYINT, &byInfluenceType, 0,					&arrCB[4]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 5, SQL_C_TINYINT, &byLevel, 0,							&arrCB[5]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 6, SQL_C_CHAR, &szDate, SIZE_MAX_SQL_DATETIME_STRING,	&arrCB[6]);


	// DB에 값이 없을때까지 loop를 돈다
	while ( (bRet = SQLFetch(m_pODBCStmt->m_hstmt)) != SQL_NO_DATA)
	{
		int nNewRowIdx = m_GridStaMission.GetRowCount();
		m_GridStaMission.SetRowCount(nNewRowIdx+1);

		//select된 값을 GridDetail GridCtrl에 넣어준다.
		Item.row		= nNewRowIdx;
		Item.col		= 0;
		Item.strText.Format("%d", nNewRowIdx);
		m_GridStaMission.SetItem(&Item);

		Item.col		= 1;
		Item.strText.Format("%s", szAccName);
		m_GridStaMission.SetItem(&Item);

		Item.col		= 2;
		Item.strText.Format("%s", szCharName);
		m_GridStaMission.SetItem(&Item);

		Item.col		= 3;
		Item.strText.Format("%s", szUnitkind);
		m_GridStaMission.SetItem(&Item);

		Item.col		= 4;
		Item.strText.Format("%s", CAtumSJ::GetInfluenceTypeString(byInfluenceType));
		m_GridStaMission.SetItem(&Item);

		Item.col		= 5;
		Item.strText.Format("%d", byLevel);
		m_GridStaMission.SetItem(&Item);
		
		Item.col		= 6;
		Item.strText.Format("%s", szDate);
		m_GridStaMission.SetItem(&Item);
	}
	m_GridStaMission.UpdateData();

	m_pODBCStmt->FreeStatement();	// clean up

	m_GridStaMission.AutoSize();

	m_ctlnResultCount = m_GridStaMission.GetRowCount() - 1;
	UpdateData(FALSE);
}

void CStatisticPage8Dlg::OnButtonSt8Save() 
{
	// TODO: Add your control notification handler code here
	CString tempStr;
	for(int i = 0; i<m_GridStaMission.GetRowCount();i++)
	{
		tempStr = "";
		for(int j = 0; j< m_GridStaMission.GetColumnCount();j++)
		{
			tempStr += m_GridStaMission.GetItemText( i, j);
			tempStr += "|\t";
		}
		m_pMainDlg->m_pWriteLog->WriteLogFile((char*)(LPCTSTR)tempStr, tempStr.GetLength(), m_pMainDlg, 1);
	}
}

void CStatisticPage8Dlg::OnCheck1() 
{
	// TODO: Add your control notification handler code here
	UpdateData();

	GetDlgItem(IDC_DATETIMEPICKER_ST3_DAILY_CONNECTION_STARTDATE)->EnableWindow(m_valueCheckDailyMission);
	GetDlgItem(IDC_DATETIMEPICKER_ST1_DAILY_ACCOUNT_STARTTIME8)->EnableWindow(m_valueCheckDailyMission);
	GetDlgItem(IDC_DATETIMEPICKER_ST3_DAILY_CONNECTION_ENDDATE)->EnableWindow(m_valueCheckDailyMission);
	GetDlgItem(IDC_DATETIMEPICKER_ST1_DAILY_ACCOUNT_ENDTIME8)->EnableWindow(m_valueCheckDailyMission);
	
	if(m_valueCheckDailyMission)
	{
		GetDlgItem(IDC_DATETIMEPICKER_ST3_DAILY_CONNECTION_STARTDATE2)->EnableWindow(!m_valueCheckDailyMission);
		GetDlgItem(IDC_TIMEPICKER_SPECMISSION_START)->EnableWindow(!m_valueCheckDailyMission);
		GetDlgItem(IDC_DATETIMEPICKER_ST3_DAILY_CONNECTION_ENDDATE2)->EnableWindow(!m_valueCheckDailyMission);
		GetDlgItem(IDC_TIMEPICKER_SPECMISSION_END)->EnableWindow(!m_valueCheckDailyMission);
		GetDlgItem(IDC_EDIT_MISSION_NUM)->EnableWindow(!m_valueCheckDailyMission);
		GetDlgItem(IDC_CHECK3)->EnableWindow(!m_valueCheckDailyMission);
		m_valueCheckSpecMission = !m_valueCheckDailyMission;
		UpdateData(FALSE);
	}
}

void CStatisticPage8Dlg::OnCheck2() 
{
	// TODO: Add your control notification handler code here
	UpdateData();

	GetDlgItem(IDC_DATETIMEPICKER_ST3_DAILY_CONNECTION_STARTDATE2)->EnableWindow(m_valueCheckSpecMission);
	GetDlgItem(IDC_TIMEPICKER_SPECMISSION_START)->EnableWindow(m_valueCheckSpecMission);
	GetDlgItem(IDC_DATETIMEPICKER_ST3_DAILY_CONNECTION_ENDDATE2)->EnableWindow(m_valueCheckSpecMission);
	GetDlgItem(IDC_TIMEPICKER_SPECMISSION_END)->EnableWindow(m_valueCheckSpecMission);	
	GetDlgItem(IDC_EDIT_MISSION_NUM)->EnableWindow(m_valueCheckSpecMission);	
	GetDlgItem(IDC_CHECK3)->EnableWindow(m_valueCheckSpecMission);	
	if(m_valueCheckSpecMission)
	{
		GetDlgItem(IDC_DATETIMEPICKER_ST3_DAILY_CONNECTION_STARTDATE)->EnableWindow(!m_valueCheckSpecMission);
		GetDlgItem(IDC_DATETIMEPICKER_ST1_DAILY_ACCOUNT_STARTTIME8)->EnableWindow(!m_valueCheckSpecMission);
		GetDlgItem(IDC_DATETIMEPICKER_ST3_DAILY_CONNECTION_ENDDATE)->EnableWindow(!m_valueCheckSpecMission);
		GetDlgItem(IDC_DATETIMEPICKER_ST1_DAILY_ACCOUNT_ENDTIME8)->EnableWindow(!m_valueCheckSpecMission);
		m_valueCheckDailyMission = !m_valueCheckSpecMission;
		UpdateData(FALSE);
	}
}
