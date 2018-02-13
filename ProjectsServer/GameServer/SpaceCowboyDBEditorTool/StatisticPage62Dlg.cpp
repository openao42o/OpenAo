// StatisticPage62Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "spacecowboydbeditortool.h"
#include "StatisticPage62Dlg.h"
#include "SpaceCowboyDBEditorToolDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStatisticPage62Dlg property page

IMPLEMENT_DYNCREATE(CStatisticPage62Dlg, CPropertyPage)

CStatisticPage62Dlg::CStatisticPage62Dlg() : CPropertyPage(CStatisticPage62Dlg::IDD)
{
	//{{AFX_DATA_INIT(CStatisticPage62Dlg)
	m_ctl_nTotalCount = 0;
	m_ctl_strPrefix = _T("");
	m_ctl_strSuffix = _T("");
	m_ctl_nRankCount = 100;
	//}}AFX_DATA_INIT
	m_pMainDlg				= (CSpaceCowboyDBEditorToolDlg*)AfxGetMainWnd();
	m_pODBCStmt				= m_pMainDlg->m_pODBCStmt;
	m_pMapIteminfo			= &m_pMainDlg->m_mapItemInfo;
	m_pMapRareItemInfo		= &m_pMainDlg->m_mapRareItemInfo;
	m_pMapRareItemInfoName	= &m_pMainDlg->m_mapRareItemInfoName;
}

CStatisticPage62Dlg::~CStatisticPage62Dlg()
{
}

void CStatisticPage62Dlg::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStatisticPage62Dlg)
	DDX_Control(pDX, IDC_COMBO_SUFFIX, m_ctl_comboSuffix);
	DDX_Control(pDX, IDC_COMBO_PREFIX, m_ctl_comboPrefix);
	DDX_Text(pDX, IDC_EDIT_TOTAL_COUNT, m_ctl_nTotalCount);
	DDX_Text(pDX, IDC_EDIT_PREFIX, m_ctl_strPrefix);
	DDX_Text(pDX, IDC_EDIT_SUFFIX, m_ctl_strSuffix);
	DDX_Control(pDX, IDC_GRIDCTRL_VIEW_62, m_gridView);
	DDX_Text(pDX, IDC_EDIT_RANK_COUNT, m_ctl_nRankCount);
	DDV_MinMaxInt(pDX, m_ctl_nRankCount, 1, 1000);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CStatisticPage62Dlg, CPropertyPage)
	//{{AFX_MSG_MAP(CStatisticPage62Dlg)
	ON_BN_CLICKED(IDC_BTN_SAVE_AS_FILE, OnBtnSaveAsFile)
	ON_BN_CLICKED(IDC_BTN_SEARCH, OnBtnSearch)
	ON_CBN_SELCHANGE(IDC_COMBO_PREFIX, OnSelchangeComboPrefix)
	ON_CBN_SELCHANGE(IDC_COMBO_SUFFIX, OnSelchangeComboSuffix)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStatisticPage62Dlg message handlers

void CStatisticPage62Dlg::OnBtnSaveAsFile() 
{
	// TODO: Add your control notification handler code here
	CString tempStr;
	CString strTm;
	GetDlgItem(IDC_STATIC_1)->GetWindowText(strTm);
	tempStr.Format("%s	%10.0f", strTm, m_ctl_nTotalCount);
	m_pMainDlg->m_pWriteLog->WriteLogFile((char*)(LPCTSTR)tempStr, tempStr.GetLength(), m_pMainDlg, 1);

	for(int i = 0; i<m_gridView.GetRowCount();i++)
	{
		tempStr.Empty();
		for(int j = 0; j< m_gridView.GetColumnCount();j++)
		{
			tempStr += m_gridView.GetItemText( i, j);
			tempStr += "|\t";
		}
		m_pMainDlg->m_pWriteLog->WriteLogFile((char*)(LPCTSTR)tempStr, tempStr.GetLength(), m_pMainDlg, 1);
	}	
}

void CStatisticPage62Dlg::OnBtnSearch() 
{
	// TODO: Add your control notification handler code here
	UpdateData();

	int nPrefix = 0;
	int nSuffix = 0;

	CString strRareName;
	m_ctl_comboPrefix.GetLBText(m_ctl_comboPrefix.GetCurSel(), strRareName);
	RARE_ITEM_INFO *pRare = m_pMapRareItemInfoName->findEZ_ptr((LPCSTR)strRareName);
	if(pRare)
	{
		nPrefix = pRare->CodeNum;
	}

	m_ctl_comboSuffix.GetLBText(m_ctl_comboSuffix.GetCurSel(), strRareName);
	pRare = m_pMapRareItemInfoName->findEZ_ptr((LPCSTR)strRareName);
	if(pRare)
	{
		nSuffix = pRare->CodeNum;
	}

	if(0 == nPrefix && 0 == nSuffix)
	{
		AfxMessageBox("Select rare.(Prefix or Suffix)");
		return;
	}

	ViewRareItemRanking(nPrefix, nSuffix);
	ViewRareItemTotalCount(nPrefix, nSuffix);
}

BOOL CStatisticPage62Dlg::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here

	m_ctl_comboPrefix.AddString(STRMSG_S_SCADMINTOOL_0025);
	m_ctl_comboSuffix.AddString(STRMSG_S_SCADMINTOOL_0025);

	ez_map<string, RARE_ITEM_INFO>::iterator	itr(m_pMapRareItemInfoName->begin());
	for(; itr != m_pMapRareItemInfoName->end(); itr++)
	{
		RARE_ITEM_INFO *pRareItemInfo = &(itr->second);

		if (IS_RARE_PREFIX(pRareItemInfo->CodeNum))
		{
			m_ctl_comboPrefix.AddString(pRareItemInfo->Name);
		}
		else if(IS_RARE_SUFFIX(pRareItemInfo->CodeNum))
		{
			m_ctl_comboSuffix.AddString(pRareItemInfo->Name);
		}
	}
	m_ctl_comboPrefix.SetCurSel(0);
	m_ctl_comboSuffix.SetCurSel(0);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			void CStatisticPage62Dlg::ViewRareItemRanking(int i_nPrefix, int i_nSuffix)
/// \brief		
/// \author		cmkwon
/// \date		2006-01-19 ~ 2006-01-19
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CStatisticPage62Dlg::ViewRareItemRanking(int i_nPrefix, int i_nSuffix)
{
	m_gridView.SetBkColor(0xFFFFFF);
	m_gridView.SetHeaderSort();

	m_gridView.SetEditable(FALSE);
	m_gridView.SetListMode(TRUE);
	m_gridView.SetSingleRowSelection(TRUE);
	m_gridView.EnableSelection(TRUE);
	m_gridView.SetFrameFocusCell(FALSE);
	m_gridView.SetTrackFocusCell(FALSE);

	m_gridView.SetRowCount(1);
	m_gridView.SetColumnCount(10);
	m_gridView.SetFixedRowCount(1);

	// 칼럼 만들기
	GV_ITEM Item;
	Item.mask = GVIF_TEXT|GVIF_FORMAT;
	Item.nFormat = DT_LEFT|DT_VCENTER|DT_SINGLELINE;
	Item.row = 0;

	Item.col = 0;
	Item.strText.Format("Num");
	m_gridView.SetItem(&Item);
	Item.col = 1;
	Item.strText.Format("Account Name");
	m_gridView.SetItem(&Item);
	Item.col = 2;
	Item.strText.Format("Character Name");
	m_gridView.SetItem(&Item);
	Item.col = 3;
	Item.strText.Format("Unitkind");
	m_gridView.SetItem(&Item);
	Item.col = 4;
	Item.strText.Format("InfluenceType");
	m_gridView.SetItem(&Item);
	Item.col = 5;
	Item.strText.Format("Level");
	m_gridView.SetItem(&Item);
	Item.col = 6;
	Item.strText.Format("Item Info");
	m_gridView.SetItem(&Item);
	Item.col = 7;
	Item.strText.Format("Item Level");
	m_gridView.SetItem(&Item);
	Item.col = 8;
	Item.strText.Format("Prefix Info");
	m_gridView.SetItem(&Item);
	Item.col = 9;
	Item.strText.Format("Suffix Info");
	m_gridView.SetItem(&Item);

	m_ctl_nRankCount = max(1, m_ctl_nRankCount);
	m_ctl_nRankCount = min(1000, m_ctl_nRankCount);

	CString szSQLQuery;

	if(i_nPrefix && i_nSuffix)
	{
		// 2007-08-09 by cmkwon, 삭제된 캐릭터 처리(RACE_DELETED_CHARACTER) - 검색 결과에서 제외 한다(0 = c.Race&%d AND )
		// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
		//szSQLQuery.Format("SELECT TOP %d c.AccountName, c.CharacterName, dbo.atum_GetGearString(c.unitkind), c.InfluenceType, c.level, s.ItemNum, i.ReqMinLevel, s.PrefixCodeNum, s.SuffixCodeNum \
		//	FROM td_Store s, td_character c, ti_Item i WITH (NOLOCK) \
		//	WHERE s.Possess = c.UniqueNumber AND s.ItemNum = i.ItemNum and 0 = c.Race&%d AND c.Race < 128 \
		//		AND s.PrefixCodeNum = %d AND s.SuffixCodeNum = %d \
		//	ORDER BY i.ReqMinLevel DESC", m_ctl_nRankCount, RACE_DELETED_CHARACTER, i_nPrefix, i_nSuffix);
		///////////////////////////////////////////////////////////////////////////////	
		// 2008-07-08 by cmkwon, MySQL 지원 구현 - 
#if defined(DB_SERVER_MYSQL)
		szSQLQuery.Format(QUERY_080702_0189, RACE_DELETED_CHARACTER, i_nPrefix, i_nSuffix, m_ctl_nRankCount);
#else
		szSQLQuery.Format(QUERY_080702_0189, m_ctl_nRankCount, RACE_DELETED_CHARACTER, i_nPrefix, i_nSuffix);
#endif
	}
	else
	{
		// 2007-08-09 by cmkwon, 삭제된 캐릭터 처리(RACE_DELETED_CHARACTER) - 검색 결과에서 제외 한다(0 = c.Race&%d AND )
		// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
		//szSQLQuery.Format("SELECT TOP %d c.AccountName, c.CharacterName, dbo.atum_GetGearString(c.unitkind), c.InfluenceType, c.level, s.ItemNum, i.ReqMinLevel, s.PrefixCodeNum, s.SuffixCodeNum \
		//	FROM td_Store s, td_character c, ti_Item i WITH (NOLOCK) \
		//	WHERE s.Possess = c.UniqueNumber AND s.ItemNum = i.ItemNum and 0 = c.Race&%d AND c.Race < 128 \
		//		AND (%d <> 0 and s.PrefixCodeNum = %d OR %d <> 0 AND s.SuffixCodeNum = %d) \
		//	ORDER BY i.ReqMinLevel DESC", m_ctl_nRankCount, RACE_DELETED_CHARACTER, i_nPrefix, i_nPrefix, i_nSuffix, i_nSuffix);
		///////////////////////////////////////////////////////////////////////////////	
		// 2008-07-08 by cmkwon, MySQL 지원 구현 - 
#if defined(DB_SERVER_MYSQL)
		szSQLQuery.Format(QUERY_080702_0190, RACE_DELETED_CHARACTER, i_nPrefix, i_nPrefix, i_nSuffix, i_nSuffix, m_ctl_nRankCount);
#else
		szSQLQuery.Format(QUERY_080702_0190, m_ctl_nRankCount, RACE_DELETED_CHARACTER, i_nPrefix, i_nPrefix, i_nSuffix, i_nSuffix);
#endif
	}
// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
//	BOOL bRet = m_pODBCStmt->ExecuteQuery((LPCSTR)szSQLQuery);
	SQLHSTMT hstmt = m_pODBCStmt->GetSTMTHandle();
	SQLINTEGER arrCB2[2] = {SQL_NTS,SQL_NTS};
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SZQUERY, 0, (LPSTR)(LPCSTR)szSQLQuery, 0,			&arrCB2[1]);
	BOOL bRet = m_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_080827_0122));
	if (!bRet)
	{
		// clean up
		m_pODBCStmt->FreeStatement();
		MessageBox("Error");
		return;
	}

	SQLINTEGER arrCB[10]={SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS, SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS};
	char szAccName[SIZE_MAX_ACCOUNT_NAME];
	char szCharName[SIZE_MAX_CHARACTER_NAME];
	char szUnitkind[10];
	BYTE byInfluenceType = 0;
	BYTE byLevel = 0;
	int nItemNum = 0;
	int nReqMinLevel = 0;
	int nPrefixCodeNum = 0;
	int nSuffixCodeNum = 0;
	
	SQLBindCol(m_pODBCStmt->m_hstmt, 1, SQL_C_CHAR, szAccName, SIZE_MAX_ACCOUNT_NAME,		&arrCB[1]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 2, SQL_C_CHAR, szCharName, SIZE_MAX_CHARACTER_NAME,	&arrCB[2]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 3, SQL_C_CHAR, szUnitkind, 10,						&arrCB[3]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 4, SQL_C_TINYINT, &byInfluenceType, 0,					&arrCB[4]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 5, SQL_C_TINYINT, &byLevel, 0,							&arrCB[5]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 6, SQL_C_LONG, &nItemNum, 0,							&arrCB[6]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 7, SQL_C_LONG, &nReqMinLevel, 0,						&arrCB[7]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 8, SQL_C_LONG, &nPrefixCodeNum, 0,						&arrCB[8]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 9, SQL_C_LONG, &nSuffixCodeNum, 0,						&arrCB[9]);

	// DB에 값이 없을때까지 loop를 돈다
	while ( (bRet = SQLFetch(m_pODBCStmt->m_hstmt)) != SQL_NO_DATA)
	{
		int nNewRowIdx = m_gridView.GetRowCount();
		m_gridView.SetRowCount(nNewRowIdx+1);

		//select된 값을 GridDetail GridCtrl에 넣어준다.
		Item.row		= nNewRowIdx;
		Item.col		= 0;
		Item.strText.Format("%d", nNewRowIdx);
		m_gridView.SetItem(&Item);

		Item.col		= 1;
		Item.strText.Format("%s", szAccName);
		m_gridView.SetItem(&Item);

		Item.col		= 2;
		Item.strText.Format("%s", szCharName);
		m_gridView.SetItem(&Item);

		Item.col		= 3;
		Item.strText.Format("%s", szUnitkind);
		m_gridView.SetItem(&Item);

		Item.col		= 4;
		Item.strText.Format("%s", CAtumSJ::GetInfluenceTypeString(byInfluenceType));
		m_gridView.SetItem(&Item);

		Item.col		= 5;
		Item.strText.Format("%d", byLevel);
		m_gridView.SetItem(&Item);

		Item.col		= 6;
		if(m_pMapIteminfo->findEZ_ptr(nItemNum))
		{
			Item.strText.Format("[%d]%s", nItemNum, m_pMapIteminfo->findEZ_ptr(nItemNum)->ItemName);
		}
		else
		{
			Item.strText.Format("%d", nItemNum);
		}
		m_gridView.SetItem(&Item);
		
		Item.col		= 7;
		Item.strText.Format("%d", nReqMinLevel);
		m_gridView.SetItem(&Item);

		Item.col		= 8;
		if(nPrefixCodeNum
			&& m_pMapRareItemInfo->findEZ_ptr(nPrefixCodeNum))
		{
			Item.strText.Format("[%d]%s", nPrefixCodeNum, m_pMapRareItemInfo->findEZ_ptr(nPrefixCodeNum)->Name);
		}
		else
		{
			Item.strText.Format("%d", nPrefixCodeNum);
		}
		m_gridView.SetItem(&Item);

		Item.col		= 9;
		if(nSuffixCodeNum
			&& m_pMapRareItemInfo->findEZ_ptr(nSuffixCodeNum))
		{
			Item.strText.Format("[%d]%s", nSuffixCodeNum, m_pMapRareItemInfo->findEZ_ptr(nSuffixCodeNum)->Name);
		}
		else
		{
			Item.strText.Format("%d", nSuffixCodeNum);
		}
		m_gridView.SetItem(&Item);

		nReqMinLevel		= 0;
		nPrefixCodeNum		= 0;
		nSuffixCodeNum		= 0;
	}
	m_gridView.UpdateData();

	m_pODBCStmt->FreeStatement();	// clean up

	m_gridView.AutoSize();
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			void CStatisticPage62Dlg::ViewRareItemTotalCount(int i_nPrefix, int i_nSuffix)
/// \brief		
/// \author		cmkwon
/// \date		2006-01-19 ~ 2006-01-19
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CStatisticPage62Dlg::ViewRareItemTotalCount(int i_nPrefix, int i_nSuffix)
{
	m_ctl_nTotalCount	= 0;

	CString szSQLQuery;
	if(i_nPrefix && i_nSuffix)
	{
		// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
		//szSQLQuery.Format("SELECT COUNT(*) \
		//						FROM td_Store s, td_account a WITH (NOLOCK)\
		//						WHERE s.AccountUniqueNumber = a.AccountUniqueNumber and a.AccountType < 128 AND s.PrefixCodeNum = %d AND s.SuffixCodeNum = %d"
		//						, i_nPrefix, i_nSuffix);
		szSQLQuery.Format(QUERY_080702_0191, i_nPrefix, i_nSuffix);
	}
	else
	{
		// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
		//szSQLQuery.Format("SELECT COUNT(*) \
		//						FROM td_Store s, td_account a WITH (NOLOCK)\
		//						WHERE s.AccountUniqueNumber = a.AccountUniqueNumber and a.AccountType < 128 AND (%d <> 0 and s.PrefixCodeNum = %d OR %d <>0 and s.SuffixCodeNum = %d)"
		//						, i_nPrefix, i_nPrefix, i_nSuffix, i_nSuffix);
		szSQLQuery.Format(QUERY_080702_0192, i_nPrefix, i_nPrefix, i_nSuffix, i_nSuffix);
	}
// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
//	BOOL bRet = m_pODBCStmt->ExecuteQuery((LPCSTR)szSQLQuery);
	SQLHSTMT hstmt = m_pODBCStmt->GetSTMTHandle();
	SQLINTEGER arrCB2[2] = {SQL_NTS,SQL_NTS};
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SZQUERY, 0, (LPSTR)(LPCSTR)szSQLQuery, 0,			&arrCB2[1]);
	BOOL bRet = m_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_080827_0122));
	if (!bRet)
	{
		// clean up
		m_pODBCStmt->FreeStatement();
		MessageBox("Error");
		return;
	}
	float fTotalCount = 0;
	SQLBindCol(m_pODBCStmt->m_hstmt, 1, SQL_C_FLOAT, &fTotalCount, 0,	NULL);
	bRet = SQLFetch(m_pODBCStmt->m_hstmt);
	if(SQL_NO_DATA != bRet)
	{
		m_ctl_nTotalCount = fTotalCount;
	}
	m_pODBCStmt->FreeStatement();	// clean up
	UpdateData(FALSE);
}

void CStatisticPage62Dlg::OnSelchangeComboPrefix() 
{
	// TODO: Add your control notification handler code here

	m_ctl_strPrefix.Empty();
	UpdateData(FALSE);
	
	CString strRareName;
	m_ctl_comboPrefix.GetLBText(m_ctl_comboPrefix.GetCurSel(), strRareName);
	RARE_ITEM_INFO *pRare = m_pMapRareItemInfoName->findEZ_ptr((LPCSTR)strRareName);
	if(NULL == pRare)
	{
		return;
	}

	m_ctl_strPrefix.Format("[%4d]%s", pRare->CodeNum, pRare->Name);
	UpdateData(FALSE);
}

void CStatisticPage62Dlg::OnSelchangeComboSuffix() 
{
	// TODO: Add your control notification handler code here
	
	m_ctl_strSuffix.Empty();
	UpdateData(FALSE);
	
	CString strRareName;
	m_ctl_comboSuffix.GetLBText(m_ctl_comboSuffix.GetCurSel(), strRareName);
	RARE_ITEM_INFO *pRare = m_pMapRareItemInfoName->findEZ_ptr((LPCSTR)strRareName);
	if(NULL == pRare)
	{
		return;
	}

	m_ctl_strSuffix.Format("[%4d]%s", pRare->CodeNum, pRare->Name);
	UpdateData(FALSE);
}
