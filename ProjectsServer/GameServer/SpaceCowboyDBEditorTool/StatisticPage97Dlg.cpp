// StatisticPage97Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "spacecowboydbeditortool.h"
#include "StatisticPage97Dlg.h"
#include "SpaceCowboyDBEditorToolDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStatisticPage97Dlg property page

IMPLEMENT_DYNCREATE(CStatisticPage97Dlg, CPropertyPage)

CStatisticPage97Dlg::CStatisticPage97Dlg() : CPropertyPage(CStatisticPage97Dlg::IDD)
{
	//{{AFX_DATA_INIT(CStatisticPage97Dlg)
	m_valueStartDate	= COleDateTime::GetCurrentTime() - COleDateTimeSpan(1, 0 , 0, 0);
	m_valueEndDate		= COleDateTime::GetCurrentTime();
	m_valueStartTime	= COleDateTime::GetCurrentTime();
	m_valueEndTime		= COleDateTime::GetCurrentTime();
	m_ctl_bOnlyInflMonster = FALSE;
	//}}AFX_DATA_INIT
	m_pMainDlg				= (CSpaceCowboyDBEditorToolDlg*)AfxGetMainWnd();
	m_pODBCStmt				= m_pMainDlg->m_pODBCStmt;
}

CStatisticPage97Dlg::~CStatisticPage97Dlg()
{
}

void CStatisticPage97Dlg::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStatisticPage97Dlg)
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_START_DATE, m_valueStartDate);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_END_DATE, m_valueEndDate);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_START_TIME, m_valueStartTime);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_START_TIME, m_valueEndTime);
	DDX_Control(pDX, IDC_GRIDCTRL_VIEW_97, m_gridView97);
	DDX_Check(pDX, IDC_CHECK_INFL_MONSTER, m_ctl_bOnlyInflMonster);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CStatisticPage97Dlg, CPropertyPage)
	//{{AFX_MSG_MAP(CStatisticPage97Dlg)
	ON_BN_CLICKED(IDC_BTN_SEARCH, OnBtnSearch)
	ON_BN_CLICKED(IDC_BTN_SAVE_AS_FILE, OnBtnSaveAsFile)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStatisticPage97Dlg message handlers


void CStatisticPage97Dlg::OnBtnSearch() 
{
	// TODO: Add your control notification handler code here
	UpdateData();

	ViewBossMonsterDead(m_ctl_bOnlyInflMonster);
}

void CStatisticPage97Dlg::OnBtnSaveAsFile() 
{
	// TODO: Add your control notification handler code here
	if(0 >= m_gridView97.GetRowCount())
	{
		return;
	}
	
	CString tempStr;
	for(int i = 0; i<m_gridView97.GetRowCount();i++)
	{
		tempStr.Empty();
		for(int j = 0; j< m_gridView97.GetColumnCount();j++)
		{
			tempStr += m_gridView97.GetItemText( i, j);
			tempStr += "|\t";
		}
		m_pMainDlg->m_pWriteLog->WriteLogFile((char*)(LPCTSTR)tempStr, tempStr.GetLength(), m_pMainDlg, 1);
	}	
}

struct TM_CHARACTER_INFO
{
	char				szCharacterName0[SIZE_MAX_CHARACTER_NAME];
	char				szUnitKind[10];
	BYTE				byInfluenceType;
	BYTE				byLevel;
};
struct TM_BOSS_MONSTER_LOG
{
	INT					monsterUnitkind;
	MAP_CHANNEL_INDEX	mapChannelIdx0;
	char				szCreateTime[20];
	char				szDeadTime[20];
	TM_CHARACTER_INFO	tmCharacterInfo[SIZE_MAX_USERLIST_ON_BOSS_MONSTER_DEAD];
	char				szDropItemList[200];
};
typedef vector<TM_BOSS_MONSTER_LOG>			vectTM_BOSS_MONSTER_LOG;
void CStatisticPage97Dlg::ViewBossMonsterDead(BOOL i_bOnlyInfluenceMonster/*=FALSE*/)
{
	CString szStartDate, szEndDate;
// 2008-01-24 by cmkwon, S_A: 날짜 관련 로컬리 제이션 처리 버그 수정 - 아래와 같은 형식으로 수정
// 	szStartDate.Format("%04d-%02d-%02d %02d:%02d:%02d", m_valueStartDate.GetYear(), m_valueStartDate.GetMonth(), m_valueStartDate.GetDay()
// 		, m_valueStartTime.GetHour(), m_valueStartTime.GetMinute(), m_valueStartTime.GetSecond());
// 	szEndDate.Format("%04d-%02d-%02d %02d:%02d:%02d", m_valueEndDate.GetYear(), m_valueEndDate.GetMonth(), m_valueEndDate.GetDay()
// 		, m_valueEndTime.GetHour(), m_valueEndTime.GetMinute(), m_valueEndTime.GetSecond());
// 2008-08-28 by cmkwon, MySQL 관련 날짜시간 관련 오류 수정 - 
//	szStartDate.Format("%04d%02d%02d %02d:%02d:%02d", m_valueStartDate.GetYear(), m_valueStartDate.GetMonth(), m_valueStartDate.GetDay()
	szStartDate.Format(SQL_DATETIME_STRING_FORMAT, m_valueStartDate.GetYear(), m_valueStartDate.GetMonth(), m_valueStartDate.GetDay()
		, m_valueStartTime.GetHour(), m_valueStartTime.GetMinute(), m_valueStartTime.GetSecond());
// 2008-08-28 by cmkwon, MySQL 관련 날짜시간 관련 오류 수정 - 
//	szEndDate.Format("%04d%02d%02d %02d:%02d:%02d", m_valueEndDate.GetYear(), m_valueEndDate.GetMonth(), m_valueEndDate.GetDay()
	szEndDate.Format(SQL_DATETIME_STRING_FORMAT, m_valueEndDate.GetYear(), m_valueEndDate.GetMonth(), m_valueEndDate.GetDay()
		, m_valueEndTime.GetHour(), m_valueEndTime.GetMinute(), m_valueEndTime.GetSecond());

	m_gridView97.SetBkColor(0xFFFFFF);
	m_gridView97.SetHeaderSort();

	m_gridView97.SetEditable(FALSE);
	m_gridView97.SetListMode(TRUE);
	m_gridView97.SetSingleRowSelection(TRUE);
	m_gridView97.EnableSelection(TRUE);
	m_gridView97.SetFrameFocusCell(FALSE);
	m_gridView97.SetTrackFocusCell(FALSE);

	m_gridView97.SetRowCount(1);
	m_gridView97.SetColumnCount(16);
	m_gridView97.SetFixedRowCount(1);

	// 칼럼 만들기
	GV_ITEM Item;
	Item.mask = GVIF_TEXT|GVIF_FORMAT;
	Item.nFormat = DT_LEFT|DT_VCENTER|DT_SINGLELINE;
	Item.row = 0;

	Item.col = 0;
	Item.strText.Format("Num");
	m_gridView97.SetItem(&Item);
	Item.col = 1;
	Item.strText.Format("Monster Info");
	m_gridView97.SetItem(&Item);
	Item.col = 2;
	Item.strText.Format("Map Channel Index");
	m_gridView97.SetItem(&Item);
	Item.col = 3;
	Item.strText.Format("Create Time");
	m_gridView97.SetItem(&Item);
	Item.col = 4;
	Item.strText.Format("Dead Time");
	m_gridView97.SetItem(&Item);
	Item.col = 5;
	Item.strText.Format("Attack User 01(CharacterName(UnitKind, InfluenceType, Level)");
	m_gridView97.SetItem(&Item);
	Item.col = 6;
	Item.strText.Format("Attack User 02");
	m_gridView97.SetItem(&Item);
	Item.col = 7;
	Item.strText.Format("Attack User 03");
	m_gridView97.SetItem(&Item);
	Item.col = 8;
	Item.strText.Format("Attack User 04");
	m_gridView97.SetItem(&Item);
	Item.col = 9;
	Item.strText.Format("Attack User 05");
	m_gridView97.SetItem(&Item);
	Item.col = 10;
	Item.strText.Format("Attack User 06");
	m_gridView97.SetItem(&Item);
	Item.col = 11;
	Item.strText.Format("Attack User 07");
	m_gridView97.SetItem(&Item);
	Item.col = 12;
	Item.strText.Format("Attack User 08");
	m_gridView97.SetItem(&Item);
	Item.col = 13;
	Item.strText.Format("Attack User 09");
	m_gridView97.SetItem(&Item);
	Item.col = 14;
	Item.strText.Format("Attack User 10");
	m_gridView97.SetItem(&Item);
	Item.col = 15;
	Item.strText.Format("Drop Item List");
	m_gridView97.SetItem(&Item);

	CString szSQLQuery;
	// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
	//szSQLQuery.Format("DROP VIEW Temp_View");
// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
//	szSQLQuery.Format(QUERY_080702_0218);
//	BOOL bRet = m_pODBCStmt->ExecuteQuery((LPCSTR)szSQLQuery);
	BOOL bRet = m_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_080827_0218));
	m_pODBCStmt->FreeStatement();
	
// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
// 	szSQLQuery.Format("CREATE VIEW Temp_View AS \
// 		SELECT * FROM  atum_backup_log_monster_boss WITH (NOLOCK) WHERE time > '%s' and time < '%s' \
// 		UNION ALL \
// 		SELECT * FROM  atum_log_monster_boss WITH (NOLOCK) WHERE time > '%s' and time < '%s'", szStartDate, szEndDate, szStartDate, szEndDate);
// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
	szSQLQuery.Format(QUERY_080702_0219, szStartDate, szEndDate, szStartDate, szEndDate);
//	bRet = m_pODBCStmt->ExecuteQuery((LPCSTR)szSQLQuery);
	SQLBindParameter(m_pODBCStmt->m_hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SZQUERY, 0, (LPSTR)(LPCSTR)szSQLQuery, 0, NULL);
	bRet = m_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_080827_0219));
	m_pODBCStmt->FreeStatement();

// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
// 	szSQLQuery.Format("SELECT l.MonsterUnitKind, l.MapIndex, l.ChannelIndex, l.CreatedTime, l.DeadTime, l.AttackUser1, l.AttackUser2, l.AttackUser3, l.AttackUser4, l.AttackUser5 \
// 			, l.AttackUser6, l.AttackUser7, l.AttackUser8, l.AttackUser9, l.AttackUser10, l.DropItemList \
// 		FROM Temp_View l\
// 		ORDER BY l.Time DESC");
// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
//	szSQLQuery.Format(QUERY_080702_0220);
	bRet = m_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_080827_0220));
	if (!bRet)
	{
		// clean up
		m_pODBCStmt->FreeStatement();
		MessageBox("Error");
		return;
	}

	vectTM_BOSS_MONSTER_LOG		vectBossMonsterLogList;
	TM_BOSS_MONSTER_LOG			tmBossMonsterLog;
	SQLINTEGER arrCB[17]={SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS, SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,
							SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS, SQL_NTS,SQL_NTS};

	SQLBindCol(m_pODBCStmt->m_hstmt, 1, SQL_C_LONG, &tmBossMonsterLog.monsterUnitkind, 0,					&arrCB[1]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 2, SQL_C_USHORT, &tmBossMonsterLog.mapChannelIdx0.MapIndex, 0,			&arrCB[2]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 3, SQL_C_USHORT, &tmBossMonsterLog.mapChannelIdx0.ChannelIndex, 0,		&arrCB[3]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 4, SQL_C_CHAR, tmBossMonsterLog.szCreateTime, 20,						&arrCB[4]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 5, SQL_C_CHAR, tmBossMonsterLog.szDeadTime, 20,						&arrCB[5]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 6, SQL_C_CHAR, tmBossMonsterLog.tmCharacterInfo[0].szCharacterName0, SIZE_MAX_CHARACTER_NAME,	&arrCB[6]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 7, SQL_C_CHAR, tmBossMonsterLog.tmCharacterInfo[1].szCharacterName0, SIZE_MAX_CHARACTER_NAME,	&arrCB[7]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 8, SQL_C_CHAR, tmBossMonsterLog.tmCharacterInfo[2].szCharacterName0, SIZE_MAX_CHARACTER_NAME,	&arrCB[8]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 9, SQL_C_CHAR, tmBossMonsterLog.tmCharacterInfo[3].szCharacterName0, SIZE_MAX_CHARACTER_NAME,	&arrCB[9]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 10, SQL_C_CHAR, tmBossMonsterLog.tmCharacterInfo[4].szCharacterName0, SIZE_MAX_CHARACTER_NAME,	&arrCB[10]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 11, SQL_C_CHAR, tmBossMonsterLog.tmCharacterInfo[5].szCharacterName0, SIZE_MAX_CHARACTER_NAME,	&arrCB[11]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 12, SQL_C_CHAR, tmBossMonsterLog.tmCharacterInfo[6].szCharacterName0, SIZE_MAX_CHARACTER_NAME,	&arrCB[12]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 13, SQL_C_CHAR, tmBossMonsterLog.tmCharacterInfo[7].szCharacterName0, SIZE_MAX_CHARACTER_NAME,	&arrCB[13]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 14, SQL_C_CHAR, tmBossMonsterLog.tmCharacterInfo[8].szCharacterName0, SIZE_MAX_CHARACTER_NAME,	&arrCB[14]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 15, SQL_C_CHAR, tmBossMonsterLog.tmCharacterInfo[9].szCharacterName0, SIZE_MAX_CHARACTER_NAME,	&arrCB[15]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 16, SQL_C_CHAR, tmBossMonsterLog.szDropItemList, 200,					&arrCB[16]);


	util::zero(&tmBossMonsterLog, sizeof(tmBossMonsterLog));
	// DB에 값이 없을때까지 loop를 돈다
	while ( (bRet = SQLFetch(m_pODBCStmt->m_hstmt)) != SQL_NO_DATA)
	{
		vectBossMonsterLogList.push_back(tmBossMonsterLog);
		util::zero(&tmBossMonsterLog, sizeof(tmBossMonsterLog));
	}
	m_pODBCStmt->FreeStatement();	// clean up

	int i;
	for(i=0; i < vectBossMonsterLogList.size(); i++)
	{
		for(int j=0; j < SIZE_MAX_USERLIST_ON_BOSS_MONSTER_DEAD; j++)
		{
			TM_CHARACTER_INFO *pCharInfo = &(vectBossMonsterLogList[i].tmCharacterInfo[j]);
			if(0 == strcmp(pCharInfo->szCharacterName0, ""))
			{
				break;
			}
			// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
			//szSQLQuery.Format("SELECT dbo.atum_GetGearString(unitkind), InfluenceType, Level \
			//	FROM td_Character WITH (NOLOCK)\
			//	WHERE CharacterName = '%s'", pCharInfo->szCharacterName0);
// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
//			szSQLQuery.Format(QUERY_080702_0221, pCharInfo->szCharacterName0);
			SQLBindParameter(m_pODBCStmt->m_hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_CHARACTER_NAME, 0, pCharInfo->szCharacterName0, 0, NULL);
			bRet = m_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_080827_0221));

			arrCB[1] = arrCB[2] = arrCB[3] = SQL_NTS;
			SQLBindCol(m_pODBCStmt->m_hstmt, 1, SQL_C_CHAR, &pCharInfo->szUnitKind, 10,				&arrCB[1]);
			SQLBindCol(m_pODBCStmt->m_hstmt, 2, SQL_C_TINYINT, &pCharInfo->byInfluenceType, 0,		&arrCB[2]);
			SQLBindCol(m_pODBCStmt->m_hstmt, 3, SQL_C_TINYINT, &pCharInfo->byLevel, 0,				&arrCB[3]);
			if (bRet)
			{
				SQLFetch(m_pODBCStmt->m_hstmt);
			}
			m_pODBCStmt->FreeStatement();		// clean up
		}
	}

	for(i=0; i < vectBossMonsterLogList.size(); i++)
	{
		TM_BOSS_MONSTER_LOG *pBossLog = &vectBossMonsterLogList[i];
		MEX_MONSTER_INFO *pMonInfo = m_pMainDlg->GetMEXMonsterInfo(pBossLog->monsterUnitkind);
		if(i_bOnlyInfluenceMonster 
			&& (NULL == pMonInfo || FALSE == IS_INFLWAR_MONSTER(pMonInfo->Belligerence)))
		{
			continue;
		}

		int nNewRowIdx = m_gridView97.GetRowCount();
		m_gridView97.SetRowCount(nNewRowIdx+1);

		//select된 값을 GridDetail GridCtrl에 넣어준다.
		Item.row		= nNewRowIdx;
		Item.col		= 0;
		Item.strText.Format("%d", nNewRowIdx);
		m_gridView97.SetItem(&Item);

		Item.col		= 1;
		if(NULL == pMonInfo)
		{
			Item.strText.Format("%d", pBossLog->monsterUnitkind);
		}
		else
		{
			Item.strText.Format("%s(%d, Lv%d)", pMonInfo->MonsterName, pMonInfo->MonsterUnitKind, pMonInfo->Level);
		}		
		m_gridView97.SetItem(&Item);
		
		Item.col		= 2;
		Item.strText.Format("%d(%d)", pBossLog->mapChannelIdx0.MapIndex, pBossLog->mapChannelIdx0.ChannelIndex);
		m_gridView97.SetItem(&Item);
		
		Item.col		= 3;
		Item.strText.Format("%s", pBossLog->szCreateTime);
		m_gridView97.SetItem(&Item);

		Item.col		= 4;
		Item.strText.Format("%s", pBossLog->szDeadTime);
		m_gridView97.SetItem(&Item);

		for(int j=0; j < SIZE_MAX_USERLIST_ON_BOSS_MONSTER_DEAD; j++)
		{
			if(0 == strcmp(pBossLog->tmCharacterInfo[j].szCharacterName0, ""))
			{
				break;
			}
			Item.col		= 5+j;
			Item.strText.Format("%s(%s, %s, %d)", pBossLog->tmCharacterInfo[j].szCharacterName0, pBossLog->tmCharacterInfo[j].szUnitKind
				, CAtumSJ::GetInfluenceTypeString(pBossLog->tmCharacterInfo[j].byInfluenceType), pBossLog->tmCharacterInfo[j].byLevel);
			m_gridView97.SetItem(&Item);
		}

		Item.col		= 15;
		Item.strText.Format("%s", pBossLog->szDropItemList);
		m_gridView97.SetItem(&Item);
	}

	m_gridView97.UpdateData();
	m_gridView97.AutoSize();
}

