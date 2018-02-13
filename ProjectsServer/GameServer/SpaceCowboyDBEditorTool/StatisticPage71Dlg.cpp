// StatisticPage71Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "spacecowboydbeditortool.h"
#include "StatisticPage71Dlg.h"
#include "SpaceCowboyDBEditorToolDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStatisticPage71Dlg property page

IMPLEMENT_DYNCREATE(CStatisticPage71Dlg, CPropertyPage)

CStatisticPage71Dlg::CStatisticPage71Dlg() : CPropertyPage(CStatisticPage71Dlg::IDD)
{
	//{{AFX_DATA_INIT(CStatisticPage71Dlg)
	m_ctl_nRankCount = 100;
	//}}AFX_DATA_INIT
	m_pMainDlg				= (CSpaceCowboyDBEditorToolDlg*)AfxGetMainWnd();
	m_pODBCStmt				= m_pMainDlg->m_pODBCStmt;
}

CStatisticPage71Dlg::~CStatisticPage71Dlg()
{
}

void CStatisticPage71Dlg::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStatisticPage71Dlg)
	DDX_Control(pDX, IDC_GRIDCTRL_VIEW_71, m_gridView71);
	DDX_Text(pDX, IDC_EDIT_RANK_COUNT, m_ctl_nRankCount);
	DDV_MinMaxInt(pDX, m_ctl_nRankCount, 1, 1000);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CStatisticPage71Dlg, CPropertyPage)
	//{{AFX_MSG_MAP(CStatisticPage71Dlg)
	ON_BN_CLICKED(IDC_BTN_SEARCH, OnBtnSearch)
	ON_BN_CLICKED(IDC_BTN_SAVE_AS_FILE, OnBtnSaveAsFile)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStatisticPage71Dlg message handlers

void CStatisticPage71Dlg::OnBtnSearch() 
{
	// TODO: Add your control notification handler code here
	UpdateData();

	m_ctl_nRankCount = max(1, m_ctl_nRankCount);
	m_ctl_nRankCount = min(1000, m_ctl_nRankCount);

	ViewPvPRanking(m_ctl_nRankCount);
}

void CStatisticPage71Dlg::OnBtnSaveAsFile() 
{
	// TODO: Add your control notification handler code here
	if(0 >= m_gridView71.GetRowCount())
	{
		return;
	}
	
	CString tempStr;
	for(int i = 0; i<m_gridView71.GetRowCount();i++)
	{
		tempStr.Empty();
		for(int j = 0; j< m_gridView71.GetColumnCount();j++)
		{
			tempStr += m_gridView71.GetItemText( i, j);
			tempStr += "|\t";
		}
		m_pMainDlg->m_pWriteLog->WriteLogFile((char*)(LPCTSTR)tempStr, tempStr.GetLength(), m_pMainDlg, 1);
	}	
}



///////////////////////////////////////////////////////////////////////////////
/// \fn			void CStatisticPage71Dlg::ViewPvPRanking(int i_nRankCount)
/// \brief		
/// \author		cmkwon
/// \date		2006-01-23 ~ 2006-01-23
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CStatisticPage71Dlg::ViewPvPRanking(int i_nRankCount)
{
	m_gridView71.SetBkColor(0xFFFFFF);
	m_gridView71.SetHeaderSort();

	m_gridView71.SetEditable(FALSE);
	m_gridView71.SetListMode(TRUE);
	m_gridView71.SetSingleRowSelection(TRUE);
	m_gridView71.EnableSelection(TRUE);
	m_gridView71.SetFrameFocusCell(FALSE);
	m_gridView71.SetTrackFocusCell(FALSE);

	m_gridView71.SetRowCount(1);
	m_gridView71.SetColumnCount(9);
	m_gridView71.SetFixedRowCount(1);

	// 칼럼 만들기
	GV_ITEM Item;
	Item.mask = GVIF_TEXT|GVIF_FORMAT;
	Item.nFormat = DT_LEFT|DT_VCENTER|DT_SINGLELINE;
	Item.row = 0;

	Item.col = 0;
	Item.strText.Format("Num");
	m_gridView71.SetItem(&Item);
	Item.col = 1;
	Item.strText.Format("Account Name");
	m_gridView71.SetItem(&Item);
	Item.col = 2;
	Item.strText.Format("Character Name");
	m_gridView71.SetItem(&Item);
	Item.col = 3;
	Item.strText.Format("Unitkind");
	m_gridView71.SetItem(&Item);
	Item.col = 4;
	Item.strText.Format("InfluenceType");
	m_gridView71.SetItem(&Item);
	Item.col = 5;
	Item.strText.Format("Level");
	m_gridView71.SetItem(&Item);
	Item.col = 6;
	Item.strText.Format("PvP Point");
	m_gridView71.SetItem(&Item);
	Item.col = 7;
	Item.strText.Format("Win Count");
	m_gridView71.SetItem(&Item);
	Item.col = 8;
	Item.strText.Format("Lose Count");
	m_gridView71.SetItem(&Item);

	CString szSQLQuery;

	// 2007-08-09 by cmkwon, 삭제된 캐릭터 처리(RACE_DELETED_CHARACTER) - 검색 결과에서 제외 한다(0 = c.Race&%d AND )
// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
// 	szSQLQuery.Format("SELECT TOP %d c.accountname, c.charactername, dbo.atum_GetGearString(c.unitkind), c.InfluenceType, c.level, c.PKWinPoint*10 + c.PKLossPoint, c.PKWinPoint, c.PKLossPoint \
// 		FROM td_character c WITH (NOLOCK) \
// 		WHERE 0 = c.Race&%d AND c.race < 128 \
// 		ORDER BY c.PKWinPoint*10 + c.PKLossPoint DESC", RACE_DELETED_CHARACTER, i_nRankCount);

// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
// ///////////////////////////////////////////////////////////////////////////////	
// // 2008-07-08 by cmkwon, MySQL 지원 구현 - 
// #if defined(DB_SERVER_MYSQL)
// 	szSQLQuery.Format(QUERY_080702_0200, RACE_DELETED_CHARACTER, i_nRankCount);
// #else
// 	szSQLQuery.Format(QUERY_080702_0200, i_nRankCount, RACE_DELETED_CHARACTER);
// #endif	
// 	BOOL bRet = m_pODBCStmt->ExecuteQuery((LPCSTR)szSQLQuery);
	USHORT tmExRace = RACE_DELETED_CHARACTER;
	SQLHSTMT hstmt = m_pODBCStmt->GetSTMTHandle();
	SQLINTEGER arrCB2[3] = {SQL_NTS,SQL_NTS,SQL_NTS};
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &(i_nRankCount), 0,						&arrCB2[1]);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_USHORT, SQL_SMALLINT, 0, 0, &(tmExRace), 0,						&arrCB2[2]);
	BOOL bRet = m_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_080827_0200));
	if (!bRet)
	{
		// clean up
		m_pODBCStmt->FreeStatement();
		MessageBox("Error");
		return;
	}

	SQLINTEGER arrCB[9]={SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS, SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS};
	char szAccName[SIZE_MAX_ACCOUNT_NAME];
	char szCharName[SIZE_MAX_CHARACTER_NAME];
	char szUnitkind[10];
	BYTE byInfluenceType = 0;
	BYTE byLevel = 0;
	int nPKPoint = 0;
	int nWinCount = 0;
	int nLoseCount = 0;
	
	SQLBindCol(m_pODBCStmt->m_hstmt, 1, SQL_C_CHAR, szAccName, SIZE_MAX_ACCOUNT_NAME,		&arrCB[1]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 2, SQL_C_CHAR, szCharName, SIZE_MAX_CHARACTER_NAME,	&arrCB[2]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 3, SQL_C_CHAR, szUnitkind, 10,						&arrCB[3]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 4, SQL_C_TINYINT, &byInfluenceType, 0,					&arrCB[4]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 5, SQL_C_TINYINT, &byLevel, 0,							&arrCB[5]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 6, SQL_C_LONG, &nPKPoint, 0,							&arrCB[6]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 7, SQL_C_LONG, &nWinCount, 0,							&arrCB[7]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 8, SQL_C_LONG, &nLoseCount, 0,							&arrCB[8]);

	// DB에 값이 없을때까지 loop를 돈다
	while ( (bRet = SQLFetch(m_pODBCStmt->m_hstmt)) != SQL_NO_DATA)
	{
		int nNewRowIdx = m_gridView71.GetRowCount();
		m_gridView71.SetRowCount(nNewRowIdx+1);

		//select된 값을 GridDetail GridCtrl에 넣어준다.
		Item.row		= nNewRowIdx;
		Item.col		= 0;
		Item.strText.Format("%d", nNewRowIdx);
		m_gridView71.SetItem(&Item);

		Item.col		= 1;
		Item.strText.Format("%s", szAccName);
		m_gridView71.SetItem(&Item);

		Item.col		= 2;
		Item.strText.Format("%s", szCharName);
		m_gridView71.SetItem(&Item);

		Item.col		= 3;
		Item.strText.Format("%s", szUnitkind);
		m_gridView71.SetItem(&Item);

		Item.col		= 4;
		Item.strText.Format("%s", CAtumSJ::GetInfluenceTypeString(byInfluenceType));
		m_gridView71.SetItem(&Item);

		Item.col		= 5;
		Item.strText.Format("%d", byLevel);
		m_gridView71.SetItem(&Item);

		Item.col		= 6;
		Item.strText.Format("%d", nPKPoint);
		m_gridView71.SetItem(&Item);
		
		Item.col		= 7;
		Item.strText.Format("%d", nWinCount);
		m_gridView71.SetItem(&Item);

		Item.col		= 8;
		Item.strText.Format("%d", nLoseCount);
		m_gridView71.SetItem(&Item);

		nPKPoint	= 0;
		nWinCount	= 0;
		nLoseCount	= 0;
	}
	m_gridView71.UpdateData();

	m_pODBCStmt->FreeStatement();	// clean up

	m_gridView71.AutoSize();
}
