// SCOutPostDlg.cpp : implementation file
//

#include "stdafx.h"
#include "atumadmintool.h"
#include "SCOutPostDlg.h"
#include "AtumAdminToolDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSCOutPostDlg dialog


CSCOutPostDlg::CSCOutPostDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSCOutPostDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSCOutPostDlg)
	m_nCityMap = 0;
	m_nGuildUID = 0;
	m_byInfluence = 0;
	m_bNextWarTimeSet = FALSE;
	m_nOutPostMap = 0;	
	m_GetDate = COleDateTime::GetCurrentTime();
	m_GetTime = COleDateTime::GetCurrentTime();
	m_NextWarDate = COleDateTime::GetCurrentTime();
	m_NextWarTime = COleDateTime::GetCurrentTime();
	//}}AFX_DATA_INIT
	m_pMainDlg = (CAtumAdminToolDlg*)AfxGetMainWnd();
	m_pODBCStmt = new CODBCStatement;
}

void CSCOutPostDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSCOutPostDlg)
	DDX_Text(pDX, IDC_EDIT_CITYMAP, m_nCityMap);
	DDX_Text(pDX, IDC_EDIT_GUILDUID, m_nGuildUID);
	DDX_Text(pDX, IDC_EDIT_INFLUENCE, m_byInfluence);
	DDX_Text(pDX, IDC_EDIT_NEXTWARTIMESET, m_bNextWarTimeSet);
	DDX_Text(pDX, IDC_EDIT_OUTPOSTMAP, m_nOutPostMap);		
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_GETDATE, m_GetDate);	
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_GETTIME, m_GetTime);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_NEXTWARDATE, m_NextWarDate);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_NEXTWARTIME, m_NextWarTime);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_GRID_OUTPOST_INFO_LIST, m_GridOutPostInfo);
}

BEGIN_MESSAGE_MAP(CSCOutPostDlg, CDialog)
	//{{AFX_MSG_MAP(CSCOutPostDlg)
	ON_BN_CLICKED(IDC_BUTTON_SELECT, OnButtonSelect)
	ON_BN_CLICKED(IDC_BUTTON_UPDATE, OnButtonUpdate)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CSCOutPostDlg::InitGrid()
{
	m_GridOutPostInfo.SetBkColor(0xFFFFFF);

	int m_nRows = 1;
	int m_nCols = 7;
	int m_nFixRows = 1;
	
	m_GridOutPostInfo.SetEditable(TRUE);
	m_GridOutPostInfo.SetListMode(TRUE);
	m_GridOutPostInfo.SetSingleRowSelection(TRUE);
	m_GridOutPostInfo.EnableSelection(TRUE);
	m_GridOutPostInfo.SetHeaderSort(FALSE);
	m_GridOutPostInfo.SetFrameFocusCell(FALSE);
	m_GridOutPostInfo.SetTrackFocusCell(FALSE);

	m_GridOutPostInfo.SetRowCount(m_nRows);
	m_GridOutPostInfo.SetColumnCount(m_nCols);
	m_GridOutPostInfo.SetFixedRowCount(m_nFixRows);

	// 칼럼 만들기
	m_nCols = 0;
	GV_ITEM Item;
	Item.mask = GVIF_TEXT|GVIF_FORMAT;
	Item.row = 0;
	Item.nFormat = ((DWORD)DT_LEFT|DT_VCENTER|DT_SINGLELINE);

	Item.col = m_nCols++;
	Item.strText.Format("OutPostMap");
	m_GridOutPostInfo.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("CityMap");
	m_GridOutPostInfo.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("GetTime");
	m_GridOutPostInfo.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("NextWarTime");
	m_GridOutPostInfo.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("NextWarTimeSet");
	m_GridOutPostInfo.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("Influence");
	m_GridOutPostInfo.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("GuildUID");
	m_GridOutPostInfo.SetItem(&Item);

	m_GridOutPostInfo.AutoSize();
	m_GridOutPostInfo.ExpandColumnsToFit();

	CCellRange tmpCellRange(1, 0, m_GridOutPostInfo.GetRowCount()-1, m_GridOutPostInfo.GetColumnCount()-1);
	m_GridOutPostInfo.ClearCells(tmpCellRange);
}

/////////////////////////////////////////////////////////////////////////////
// CSCOutPostDlg message handlers
void CSCOutPostDlg::DBLoadOutPostInfo()
{
	/************************************************************************
	CREATE PROCEDURE dbo.atum_LoadOutPostInfo
	AS
		SELECT O.OutPostMapIndex, O.OutPostCityMapIndex, O.OutPostGetTime, O.OutPostNextWarTime
				, O.OutPostNextWarTimeSet, O.OutPostGetInfl, O.OutPostGuildUID, G.GuildName
		FROM td_OutPostInfo AS O LEFT JOIN td_Guild AS G 
		ON O.OutPostGuildUID = G.GuildUniqueNumber
	GO
	************************************************************************/
	// 2009-07-09 by cmkwon, 호출 함수 변경(에러 정보 체크를 위해) - 아래와 같이 수정
	//RETCODE	ret = SQLExecDirect(m_pODBCStmt->m_hstmt, PROCEDURE_080822_0017, SQL_NTS);
	RETCODE	ret = m_pODBCStmt->ExecuteQuery(PROCEDURE_080822_0017);
	// 2009-08-13 by cmkwon, AdminTool 거점전 설정 버그 수정 - 리턴값 체크 버그 수정
	if(FALSE == ret)
	{
		m_pODBCStmt->FreeStatement();
		return;
	}
	
	SOUTPOST_INFO		tmOutPostInfo;
	util::zero(&tmOutPostInfo, sizeof(SOUTPOST_INFO));
	SQL_TIMESTAMP_STRUCT	GetTime;
	SQL_TIMESTAMP_STRUCT	NextWarTime;

	SQLINTEGER	arrCB[9] = {SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS};
	SQLBindCol(m_pODBCStmt->m_hstmt, 1, SQL_C_ULONG, &tmOutPostInfo.OutPostMapIndex, 0, &arrCB[1]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 2, SQL_C_ULONG, &tmOutPostInfo.OutPostCityMapIndex, 0, &arrCB[2]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 3, SQL_C_TIMESTAMP, &GetTime, 0, &arrCB[3]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 4, SQL_C_TIMESTAMP, &NextWarTime, 0, &arrCB[4]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 5, SQL_C_TINYINT, &tmOutPostInfo.OutPostNextWarTimeSet, 0, &arrCB[5]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 6, SQL_C_TINYINT, &tmOutPostInfo.OutPostInfluence, 0, &arrCB[6]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 7, SQL_C_ULONG, &tmOutPostInfo.OutPostGuildUID, 0, &arrCB[7]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 8, SQL_C_CHAR, &tmOutPostInfo.OutPostGuildName, SIZE_MAX_GUILD_NAME, &arrCB[8]);

	m_SOutPostInfo.clear();
	while ( (ret = SQLFetch(m_pODBCStmt->m_hstmt)) != SQL_NO_DATA)
	{
		tmOutPostInfo.OutPostGetTime		=	GetTime;
		tmOutPostInfo.OutPostNextWarTime	=	NextWarTime;
		m_SOutPostInfo.push_back(tmOutPostInfo);
		util::zero(&tmOutPostInfo, sizeof(SOUTPOST_INFO));
	}
	m_pODBCStmt->FreeStatement();


	this->InsertOutPostInfo();
}

void CSCOutPostDlg::InsertOutPostInfo()
{
	vectSOutPostInfo::iterator	itr = m_SOutPostInfo.begin();
	for(; itr != m_SOutPostInfo.end(); itr++)
	{
		int m_nCols = 0;

		m_GridOutPostInfo.SetRowCount(m_GridOutPostInfo.GetRowCount()+1);

		GV_ITEM Item;
		Item.mask = GVIF_TEXT|GVIF_FORMAT;
		Item.row = m_GridOutPostInfo.GetRowCount()-1;
		Item.nFormat = ((DWORD)DT_LEFT|DT_VCENTER|DT_SINGLELINE);

		Item.col = m_nCols++;
		Item.strText.Format("%d", itr->OutPostMapIndex);
		m_GridOutPostInfo.SetItem(&Item);

		Item.col = m_nCols++;
		Item.strText.Format("%d", itr->OutPostCityMapIndex);
		m_GridOutPostInfo.SetItem(&Item);

		Item.col = m_nCols++;
		Item.strText.Format("%s", itr->OutPostGetTime.GetDateTimeString().GetBuffer());
		m_GridOutPostInfo.SetItem(&Item);

		Item.col = m_nCols++;
		Item.strText.Format("%s", itr->OutPostNextWarTime.GetDateTimeString().GetBuffer());
		m_GridOutPostInfo.SetItem(&Item);

		Item.col = m_nCols++;
		Item.strText.Format("%d", itr->OutPostNextWarTimeSet);
		m_GridOutPostInfo.SetItem(&Item);

		Item.col = m_nCols++;
		Item.strText.Format("%d", itr->OutPostInfluence);
		m_GridOutPostInfo.SetItem(&Item);

		Item.col = m_nCols++;
		Item.strText.Format("%d", itr->OutPostGuildUID);
		m_GridOutPostInfo.SetItem(&Item);
		
		m_GridOutPostInfo.UpdateData();
		m_GridOutPostInfo.AutoSize();
	}

}

BOOL CSCOutPostDlg::OnInitDialog() 
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
	this->DBLoadOutPostInfo();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSCOutPostDlg::OnButtonSelect() 
{
	// TODO: Add your control notification handler code here
	CCellID cellID = m_GridOutPostInfo.GetFocusCell();
	if(0 >= cellID.row)
	{
		return;
	}

	int SelectOutPostMap				= atoi(m_GridOutPostInfo.GetItemText(cellID.row, 0));
	vectSOutPostInfo::iterator	itr = m_SOutPostInfo.begin();
	for(; itr != m_SOutPostInfo.end(); itr++)
	{
		if(SelectOutPostMap == itr->OutPostMapIndex)
		{
			m_nCityMap			= itr->OutPostCityMapIndex;
			m_nGuildUID			= itr->OutPostGuildUID;
			m_byInfluence		= itr->OutPostInfluence;
			m_bNextWarTimeSet	= itr->OutPostNextWarTimeSet;
			m_nOutPostMap		= itr->OutPostMapIndex;
			m_GetDate.SetDate(itr->OutPostGetTime.Year, itr->OutPostGetTime.Month, itr->OutPostGetTime.Day);
			m_GetTime.SetTime(itr->OutPostGetTime.Hour, itr->OutPostGetTime.Minute, itr->OutPostGetTime.Second);
			m_NextWarDate.SetDate(itr->OutPostNextWarTime.Year, itr->OutPostNextWarTime.Month, itr->OutPostNextWarTime.Day);
			m_NextWarTime.SetTime(itr->OutPostNextWarTime.Hour, itr->OutPostNextWarTime.Minute, itr->OutPostNextWarTime.Second);
		}
	}
	// 2013-01-21 by jhseol, NGC 전전기지 트리거 시스템 - 어드민 툴에서 혼동을 막기위해 수정 가능한 항목만 활성화
#ifdef S_TRIGGER_OUTPOST_JHSEOL	// - 어드민 툴에서 혼동을 막기위해 수정 가능한 항목만 활성화
	if ( MAP_INFLUENCE_OUTPOST_TRIGGER == SelectOutPostMap )
	{
		GetDlgItem(IDC_EDIT_NEXTWARTIMESET)->EnableWindow(FALSE);
		GetDlgItem(IDC_DATETIMEPICKER_NEXTWARDATE)->EnableWindow(FALSE);
		GetDlgItem(IDC_DATETIMEPICKER_NEXTWARTIME)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_EDIT_NEXTWARTIMESET)->EnableWindow(TRUE);
		GetDlgItem(IDC_DATETIMEPICKER_NEXTWARDATE)->EnableWindow(TRUE);
		GetDlgItem(IDC_DATETIMEPICKER_NEXTWARTIME)->EnableWindow(TRUE);
	}
#endif
	// end 2013-01-21 by jhseol, NGC 전전기지 트리거 시스템 - 어드민 툴에서 혼동을 막기위해 수정 가능한 항목만 활성화

	UpdateData(FALSE);
}

void CSCOutPostDlg::OnButtonUpdate() 
{
	// 2009-02-04 by cmkwon, AdminTool의 GM 권한 수정 - 전진기지전전 정보 검색은 가능, 수정 불가
	if(FALSE == m_pMainDlg->IsManagerAdministrator())
	{
		AfxMessageBox(ADSTRMSG_090204_0001);
		return;
	}

	UpdateData(TRUE);

	// TODO: Add your control notification handler code here
	if(IDCANCEL == AfxMessageBox("If you want to update, you must restart FieldServer", MB_OKCANCEL))
	{
		return;
	}

	if(0 != m_byInfluence
		&& 2 != m_byInfluence
		&& 4 != m_byInfluence)
	{
		AfxMessageBox("OutPost update error !!");
		return;	
	}

	if(0 == m_byInfluence
		&& 0 != m_nGuildUID)
	{
		AfxMessageBox("OutPost update error !!");
		return;		
	}

	if(0 != m_byInfluence
		&& 0 == m_nGuildUID)
	{
		AfxMessageBox("OutPost update error !!");
		return;	
	}

	ATUM_DATE_TIME	UpdateTime;
	UpdateTime.SetDateTime(m_NextWarDate.GetYear(), m_NextWarDate.GetMonth(), m_NextWarDate.GetDay()
							, m_NextWarTime.GetHour(), m_NextWarTime.GetMinute(), m_NextWarTime.GetSecond());

	if(FALSE == util::in_range(MIN_SUPPORT_YEAR, UpdateTime.Year, MAX_SUPPORT_YEAR))
	{// 2008-06-27 by cmkwon, 날짜시간 관련 버그 수정(1970 ~ 2037) - td_OutPostInfo.OutPostNextWarTime 체크
		char szTemp[1024];
		sprintf(szTemp, "Invalid year !!(Support Year:%d~%d)", MIN_SUPPORT_YEAR, MAX_SUPPORT_YEAR);
		AfxMessageBox(szTemp);
		return;
	}
	
	// 2013-01-21 by jhseol, NGC 전전기지 트리거 시스템 - 어드민 툴에서 혼동을 막기위해 NGC트리거 전진기지만 시간 초기화
#ifdef S_TRIGGER_OUTPOST_JHSEOL	// - 어드민 툴에서 혼동을 막기위해 NGC트리거 전진기지만 시간 초기화
	ATUM_DATE_TIME atCurrentTime(TRUE);
	atCurrentTime.AddDateTime(0,0,0,1,0,0);
	if ( UpdateTime < atCurrentTime )
	{
		char szTemp[1024];
		sprintf(szTemp, "Invalid Time !!\r\nCurrent Time:%s\r\nUpdate Time:%s", atCurrentTime.GetDateTimeString(STRNBUF(SIZE_MAX_ATUM_DATE_TIME_STRING)), UpdateTime.GetDateTimeString(STRNBUF(SIZE_MAX_ATUM_DATE_TIME_STRING)));
		AfxMessageBox(szTemp);
		return;
	}

	if ( MAP_INFLUENCE_OUTPOST_TRIGGER == m_nOutPostMap )
	{
		vectSOutPostInfo::iterator itr = m_SOutPostInfo.begin();
		for ( ; itr != m_SOutPostInfo.end() ; itr++ )
		{
			if ( 6016 == itr->OutPostMapIndex )
			{
				ATUM_DATE_TIME atCurrentTime;
				atCurrentTime.SetCurrentDateTime(TRUE);
				ATUM_DATE_TIME at6016WarTime = itr->OutPostNextWarTime;
				at6016WarTime.AddDateTime(0,0,-4,-1,0,0);
				
				if ( 0 > at6016WarTime.GetTimeDiffTimeInSeconds(atCurrentTime) )
				{
					UpdateTime = itr->OutPostNextWarTime;
					UpdateTime.AddDateTime(0,0,1,0,0,0);
				}
				else
				{
					at6016WarTime.AddDateTime(0,0,0,1,0,0);
					UpdateTime = at6016WarTime;
				}
			}
		}
	}
	else if ( 6016 == m_nOutPostMap )		// 바크시티를 업데이트 할 경우 NGC 도 같이 바크시티 시간에 맞춰 업데이트 해준다.
	{
		vectSOutPostInfo::iterator itr = m_SOutPostInfo.begin();
		for ( ; itr != m_SOutPostInfo.end() ; itr++ )
		{
			if ( MAP_INFLUENCE_OUTPOST_TRIGGER == itr->OutPostMapIndex )
			{
				ATUM_DATE_TIME atCurrentTime;
				atCurrentTime.SetCurrentDateTime(TRUE);
				ATUM_DATE_TIME at6016WarTime = UpdateTime;
				at6016WarTime.AddDateTime(0,0,-4,-1,0,0);

				if ( 0 > at6016WarTime.GetTimeDiffTimeInSeconds(atCurrentTime) )
				{
					itr->OutPostNextWarTime = UpdateTime;
					itr->OutPostNextWarTime.AddDateTime(0,0,1,0,0,0);
				}
				else
				{
					at6016WarTime.AddDateTime(0,0,0,1,0,0);
					itr->OutPostNextWarTime = at6016WarTime;
				}

				if ( itr->OutPostNextWarTime < atCurrentTime )
				{
					char szTemp[1024];
					sprintf(szTemp, "%d OutPost Map Invalid Time !!\r\nCurrent Time:%s\r\nUpdate Time:%s", MAP_INFLUENCE_OUTPOST_TRIGGER, atCurrentTime.GetDateTimeString(STRNBUF(SIZE_MAX_ATUM_DATE_TIME_STRING)), itr->OutPostNextWarTime.GetDateTimeString(STRNBUF(SIZE_MAX_ATUM_DATE_TIME_STRING)));
					AfxMessageBox(szTemp);
					return;
				}

				char tmUpdateTime2[SIZE_MAX_SQL_DATETIME_STRING];
				itr->OutPostNextWarTime.GetSQLDateTimeString(tmUpdateTime2, SIZE_MAX_SQL_DATETIME_STRING);	
				SQLBindParameter(m_pODBCStmt->m_hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SQL_DATETIME_STRING, 0, tmUpdateTime2, 0, NULL);
				SQLBindParameter(m_pODBCStmt->m_hstmt, 2, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &itr->OutPostNextWarTimeSet, 0, NULL);
				SQLBindParameter(m_pODBCStmt->m_hstmt, 3, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &itr->OutPostInfluence, 0, NULL);
				SQLBindParameter(m_pODBCStmt->m_hstmt, 4, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &itr->OutPostGuildUID, 0, NULL);
				SQLBindParameter(m_pODBCStmt->m_hstmt, 5, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &itr->OutPostMapIndex, 0, NULL);
				SQLBindParameter(m_pODBCStmt->m_hstmt, 6, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &itr->OutPostCityMapIndex, 0, NULL);
				SQLBindParameter(m_pODBCStmt->m_hstmt, 7, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &itr->OutPostInfluence, 0, NULL);
				SQLBindParameter(m_pODBCStmt->m_hstmt, 8, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &itr->OutPostGuildUID, 0, NULL);
				SQLBindParameter(m_pODBCStmt->m_hstmt, 9, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &itr->OutPostCityMapIndex, 0, NULL);
				BOOL bRet2 = m_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_080827_0269));
				if (!bRet2)
				{
					m_pODBCStmt->FreeStatement();		// cleanup
					
					AfxMessageBox("OutPost update error 2 !!");
					return;
				}
				m_pODBCStmt->FreeStatement();	// cleanup
				break;
			}
		}
	}
#endif
	// end 2013-01-21 by jhseol, NGC 전전기지 트리거 시스템 - 어드민 툴에서 혼동을 막기위해 NGC트리거 전진기지만 시간 초기화
	
	CString szQuery;
	// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
	//szQuery.Format("UPDATE dbo.td_OutPostInfo \
	//				SET OutPostNextWarTime = '%s', OutPostNextWarTimeSet = %d, OutPostGetInfl = %d, OutPostGuildUID = %d \
	//				WHERE OutPostMapIndex = %d AND OutPostCityMapIndex = %d \
	//				UPDATE dbo.td_CityLeaderInfo \
	//				SET Influence = %d, CharacterUID = (SELECT GuildCommanderUniqueNumber FROM dbo.td_Guild WHERE GuildUniqueNumber = %d) \
	//				WHERE MapIndex = %d ", UpdateTime.GetSQLDateTimeString(STRNBUF(SIZE_MAX_ATUM_DATE_TIME_STRING))
	//				, m_bNextWarTimeSet, m_byInfluence, m_nGuildUID, m_nOutPostMap, m_nCityMap, m_byInfluence, m_nGuildUID, m_nCityMap);	// 2008-06-19 by dhjin, 전진기지 길드 수정시 td_cityLeaderInfo에 유저 설정되게 수정
// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
//	szQuery.Format(QUERY_080702_0269, UpdateTime.GetSQLDateTimeString(STRNBUF(SIZE_MAX_ATUM_DATE_TIME_STRING))
//					, m_bNextWarTimeSet, m_byInfluence, m_nGuildUID, m_nOutPostMap, m_nCityMap, m_byInfluence, m_nGuildUID, m_nCityMap);
	char tmUpdateTime[SIZE_MAX_SQL_DATETIME_STRING];
	UpdateTime.GetSQLDateTimeString(tmUpdateTime, SIZE_MAX_SQL_DATETIME_STRING);	
	SQLBindParameter(m_pODBCStmt->m_hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SQL_DATETIME_STRING, 0, tmUpdateTime, 0, NULL);
	SQLBindParameter(m_pODBCStmt->m_hstmt, 2, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &m_bNextWarTimeSet, 0, NULL);
	SQLBindParameter(m_pODBCStmt->m_hstmt, 3, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &m_byInfluence, 0, NULL);
	SQLBindParameter(m_pODBCStmt->m_hstmt, 4, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &m_nGuildUID, 0, NULL);
	SQLBindParameter(m_pODBCStmt->m_hstmt, 5, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &m_nOutPostMap, 0, NULL);
	SQLBindParameter(m_pODBCStmt->m_hstmt, 6, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &m_nCityMap, 0, NULL);
	SQLBindParameter(m_pODBCStmt->m_hstmt, 7, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &m_byInfluence, 0, NULL);
	SQLBindParameter(m_pODBCStmt->m_hstmt, 8, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &m_nGuildUID, 0, NULL);
	SQLBindParameter(m_pODBCStmt->m_hstmt, 9, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &m_nCityMap, 0, NULL);
	BOOL bRet = m_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_080827_0269));
	if (!bRet)
	{
		m_pODBCStmt->FreeStatement();		// cleanup

		AfxMessageBox("OutPost update error !!");
		return;
	}
	m_pODBCStmt->FreeStatement();	// cleanup	

	this->InitGrid();
	this->DBLoadOutPostInfo();

}
