// StatisticPage102Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "spacecowboydbeditortool.h"
#include "StatisticPage102Dlg.h"
#include "SpaceCowboyDBEditorToolDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStatisticPage102Dlg property page

IMPLEMENT_DYNCREATE(CStatisticPage102Dlg, CPropertyPage)

CStatisticPage102Dlg::CStatisticPage102Dlg() : CPropertyPage(CStatisticPage102Dlg::IDD)
{
	//{{AFX_DATA_INIT(CStatisticPage102Dlg)
	m_edit_Rank_Count = 1;
	//}}AFX_DATA_INIT
	m_pMainDlg				= (CSpaceCowboyDBEditorToolDlg*)AfxGetMainWnd();
	m_pODBCStmt				= m_pMainDlg->m_pODBCStmt;
	m_RadioWP				= FALSE;
	m_RadioCWP				= FALSE;
}

CStatisticPage102Dlg::~CStatisticPage102Dlg()
{
}

void CStatisticPage102Dlg::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStatisticPage102Dlg)
	DDX_Text(pDX, IDC_EDIT_RANK_COUNT, m_edit_Rank_Count);
	DDV_MinMaxInt(pDX, m_edit_Rank_Count, 1, 1000);
	DDX_Control(pDX, IDC_GRIDCTRL_VIEW_102, m_gridView102);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CStatisticPage102Dlg, CPropertyPage)
	//{{AFX_MSG_MAP(CStatisticPage102Dlg)
	ON_BN_CLICKED(IDC_BTN_SEARCH, OnBtnSearch)
	ON_BN_CLICKED(IDC_BTN_SAVE_AS_FILE, OnBtnSaveAsFile)
	ON_BN_CLICKED(IDC_RADIO_WP, OnRadioWp)
	ON_BN_CLICKED(IDC_RADIO_CWP, OnRadioCwp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStatisticPage102Dlg message handlers

void CStatisticPage102Dlg::OnBtnSearch() 
{
	// TODO: Add your control notification handler code here
	UpdateData();

	if(FALSE == m_RadioWP
		&& FALSE == m_RadioCWP)
	{
		return;
	}

	m_edit_Rank_Count = max(1, m_edit_Rank_Count);
	m_edit_Rank_Count = min(1000, m_edit_Rank_Count);

	ViewWarPointRanking(m_edit_Rank_Count);	
}

void CStatisticPage102Dlg::OnBtnSaveAsFile() 
{
	// TODO: Add your control notification handler code here
	if(0 >= m_gridView102.GetRowCount())
	{
		return;
	}
	
	CString tempStr;
	for(int i = 0; i<m_gridView102.GetRowCount();i++)
	{
		tempStr.Empty();
		for(int j = 0; j< m_gridView102.GetColumnCount();j++)
		{
			tempStr += m_gridView102.GetItemText( i, j);
			tempStr += "|\t";
		}
		m_pMainDlg->m_pWriteLog->WriteLogFile((char*)(LPCTSTR)tempStr, tempStr.GetLength(), m_pMainDlg, 1);
	}	
}

void CStatisticPage102Dlg::ViewWarPointRanking(int i_nWarPointRankCount)
{
	m_gridView102.SetBkColor(0xFFFFFF);
	m_gridView102.SetHeaderSort();

	m_gridView102.SetEditable(FALSE);
	m_gridView102.SetListMode(TRUE);
	m_gridView102.SetSingleRowSelection(TRUE);
	m_gridView102.EnableSelection(TRUE);
	m_gridView102.SetFrameFocusCell(FALSE);
	m_gridView102.SetTrackFocusCell(FALSE);

	m_gridView102.SetRowCount(1);
	m_gridView102.SetColumnCount(11);
	m_gridView102.SetFixedRowCount(1);

	// 칼럼 만들기
	GV_ITEM Item;
	Item.mask = GVIF_TEXT|GVIF_FORMAT;
	Item.nFormat = DT_LEFT|DT_VCENTER|DT_SINGLELINE;
	Item.row = 0;

	Item.col = 0;
	Item.strText.Format("Num");
	m_gridView102.SetItem(&Item);
	Item.col = 1;
	Item.strText.Format("Account Name");
	m_gridView102.SetItem(&Item);
	Item.col = 2;
	Item.strText.Format("Character Name");
	m_gridView102.SetItem(&Item);
	Item.col = 3;
	Item.strText.Format("Unitkind");
	m_gridView102.SetItem(&Item);
	Item.col = 4;
	Item.strText.Format("InfluenceType");
	m_gridView102.SetItem(&Item);
	Item.col = 5;
	Item.strText.Format("Level");
	m_gridView102.SetItem(&Item);
	Item.col = 6;
	Item.strText.Format("War Point");
	m_gridView102.SetItem(&Item);
	Item.col = 7;
	Item.strText.Format("CWar Point");
	m_gridView102.SetItem(&Item);
	Item.col = 8;
	Item.strText.Format("Win Count");
	m_gridView102.SetItem(&Item);
	Item.col = 9;
	Item.strText.Format("Lose Count");
	m_gridView102.SetItem(&Item);
	Item.col = 10;
	Item.strText.Format("Disconnect");
	m_gridView102.SetItem(&Item);

	CString szSQLQuery;
	CString	OrderByTemp;
	if(TRUE == m_RadioWP)
	{
		OrderByTemp.Format("WarPoint");
	}
	else
	{
		OrderByTemp.Format("CumulativeWarPoint");
	}

	// 2007-08-09 by cmkwon, 삭제된 캐릭터(RACE_DELETED_CHARACTER) 처리 - 검색 결과에서 제외 한다(0 = Race&%d AND )
	// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
	//szSQLQuery.Format("SELECT TOP %d accountname, charactername, dbo.atum_GetGearString(unitkind), InfluenceType, level, WarPoint, CumulativeWarPoint, ArenaWin, ArenaLose, ArenaDisConnect \
	//	FROM td_character WITH (NOLOCK) \
	//	WHERE 0 = Race&%d AND race < 128 \
	//	ORDER BY %s DESC", i_nWarPointRankCount, RACE_DELETED_CHARACTER, OrderByTemp);

// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
// ///////////////////////////////////////////////////////////////////////////////	
// // 2008-07-08 by cmkwon, MySQL 지원 구현 - 
// #if defined(DB_SERVER_MYSQL)
// 	szSQLQuery.Format(QUERY_080702_0134, RACE_DELETED_CHARACTER, OrderByTemp, i_nWarPointRankCount);
// #else
// 	szSQLQuery.Format(QUERY_080702_0134, i_nWarPointRankCount, RACE_DELETED_CHARACTER, OrderByTemp);
// #endif
// 	BOOL bRet = m_pODBCStmt->ExecuteQuery((LPCSTR)szSQLQuery);
	USHORT tmExRace = RACE_DELETED_CHARACTER;
	SQLHSTMT hstmt = m_pODBCStmt->GetSTMTHandle();
	SQLINTEGER arrCB2[4] = {SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS};
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &(i_nWarPointRankCount), 0,						&arrCB2[1]);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_USHORT, SQL_SMALLINT, 0, 0, &(tmExRace), 0,								&arrCB2[2]);
	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_TABLE_NAME, 0, (LPSTR)(LPCSTR)OrderByTemp, 0,	&arrCB2[3]);
	BOOL bRet = m_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_080827_0134));
	if (!bRet)
	{
		// clean up
		m_pODBCStmt->FreeStatement();
		MessageBox("Error");
		return;
	}

	SQLINTEGER arrCB[11]={SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS, SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS};
	char szAccName[SIZE_MAX_ACCOUNT_NAME];
	char szCharName[SIZE_MAX_CHARACTER_NAME];
	char szUnitkind[10];
	BYTE byInfluenceType = 0;
	BYTE byLevel = 0;
	int WP = 0;
	int CWP = 0;
	int ArenaWin = 0;
	int ArenaLose = 0;
	int ArenaDisConnect = 0;
	
	SQLBindCol(m_pODBCStmt->m_hstmt, 1, SQL_C_CHAR, szAccName, SIZE_MAX_ACCOUNT_NAME,		&arrCB[1]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 2, SQL_C_CHAR, szCharName, SIZE_MAX_CHARACTER_NAME,	&arrCB[2]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 3, SQL_C_CHAR, szUnitkind, 10,						&arrCB[3]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 4, SQL_C_TINYINT, &byInfluenceType, 0,					&arrCB[4]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 5, SQL_C_TINYINT, &byLevel, 0,							&arrCB[5]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 6, SQL_C_LONG, &WP, 0,							&arrCB[6]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 7, SQL_C_LONG, &CWP, 0,							&arrCB[7]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 8, SQL_C_LONG, &ArenaWin, 0,							&arrCB[8]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 9, SQL_C_LONG, &ArenaLose, 0,							&arrCB[9]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 10, SQL_C_LONG, &ArenaDisConnect, 0,					&arrCB[10]);

	// DB에 값이 없을때까지 loop를 돈다
	while ( (bRet = SQLFetch(m_pODBCStmt->m_hstmt)) != SQL_NO_DATA)
	{
		int nNewRowIdx = m_gridView102.GetRowCount();
		m_gridView102.SetRowCount(nNewRowIdx+1);

		//select된 값을 GridDetail GridCtrl에 넣어준다.
		Item.row		= nNewRowIdx;
		Item.col		= 0;
		Item.strText.Format("%d", nNewRowIdx);
		m_gridView102.SetItem(&Item);

		Item.col		= 1;
		Item.strText.Format("%s", szAccName);
		m_gridView102.SetItem(&Item);

		Item.col		= 2;
		Item.strText.Format("%s", szCharName);
		m_gridView102.SetItem(&Item);

		Item.col		= 3;
		Item.strText.Format("%s", szUnitkind);
		m_gridView102.SetItem(&Item);

		Item.col		= 4;
		Item.strText.Format("%s", CAtumSJ::GetInfluenceTypeString(byInfluenceType));
		m_gridView102.SetItem(&Item);

		Item.col		= 5;
		Item.strText.Format("%d", byLevel);
		m_gridView102.SetItem(&Item);

		Item.col		= 6;
		Item.strText.Format("%d", WP);
		m_gridView102.SetItem(&Item);
		
		Item.col		= 7;
		Item.strText.Format("%d", CWP);
		m_gridView102.SetItem(&Item);

		Item.col		= 8;
		Item.strText.Format("%d", ArenaWin);
		m_gridView102.SetItem(&Item);
		
		Item.col		= 9;
		Item.strText.Format("%d", ArenaLose);
		m_gridView102.SetItem(&Item);
		
		Item.col		= 10;
		Item.strText.Format("%d", ArenaDisConnect);
		m_gridView102.SetItem(&Item);

	}
	m_gridView102.UpdateData();

	m_pODBCStmt->FreeStatement();	// clean up

	m_gridView102.AutoSize();
}

void CStatisticPage102Dlg::OnRadioWp() 
{
	// TODO: Add your control notification handler code here
	m_RadioWP	= TRUE;
	m_RadioCWP	= FALSE;
	
	UpdateData();
}

void CStatisticPage102Dlg::OnRadioCwp() 
{
	// TODO: Add your control notification handler code here
	m_RadioWP	= FALSE;
	m_RadioCWP	= TRUE;

	UpdateData();
}
