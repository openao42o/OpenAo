// StatisticPage61Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "spacecowboydbeditortool.h"
#include "StatisticPage61Dlg.h"
#include "SpaceCowboyDBEditorToolDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define COMBO_ITEMKIND_OFFSET			3
#define COMBO_ITEMKIND_ALL				2

/////////////////////////////////////////////////////////////////////////////
// CStatisticPage61Dlg dialog

IMPLEMENT_DYNCREATE(CStatisticPage61Dlg, CPropertyPage)

CStatisticPage61Dlg::CStatisticPage61Dlg()
	: CPropertyPage(CStatisticPage61Dlg::IDD)
{
	//{{AFX_DATA_INIT(CStatisticPage61Dlg)
	m_ctl_doTotalCount = 0.0;
	m_ctl_strItemInfo = _T("");
	//}}AFX_DATA_INIT
	m_pMainDlg = (CSpaceCowboyDBEditorToolDlg*)AfxGetMainWnd();
	m_pMapItemInfo		= &m_pMainDlg->m_mapItemInfo;
	m_pMapItemInfoName	= &m_pMainDlg->m_mapItemInfoName;
	m_pODBCStmt			= m_pMainDlg->m_pODBCStmt;
}


void CStatisticPage61Dlg::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStatisticPage61Dlg)
	DDX_Control(pDX, IDC_COMBO_ITEM_NAME, m_ctl_comboItemNameList);
	DDX_Control(pDX, IDC_COMBO_ITEM_KIND, m_ctl_comboItemKindList);
	DDX_Text(pDX, IDC_EDIT_TOTAL_COUNT, m_ctl_doTotalCount);
	DDX_Control(pDX, IDC_GRIDCTRL_VIEW_61, m_gridView);
	DDX_Text(pDX, IDC_EDIT_SEARCH_ITEM, m_ctl_strItemInfo);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CStatisticPage61Dlg, CPropertyPage)
	//{{AFX_MSG_MAP(CStatisticPage61Dlg)
	ON_CBN_SELCHANGE(IDC_COMBO_ITEM_KIND, OnSelchangeComboItemKind)
	ON_CBN_SELCHANGE(IDC_COMBO_ITEM_NAME, OnSelchangeComboItemName)
	ON_BN_CLICKED(IDC_BTN_SEARCH, OnBtnSearch)
	ON_BN_CLICKED(IDC_BTN_SAVE_AS_FILE, OnBtnSaveAsFile)
	ON_CBN_DROPDOWN(IDC_COMBO_ITEM_NAME, OnDropdownComboItemName)
	ON_CBN_EDITCHANGE(IDC_COMBO_ITEM_NAME, OnEditchangeComboItemName)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStatisticPage61Dlg message handlers

BOOL CStatisticPage61Dlg::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here

	m_ctl_comboItemKindList.AddString("Item Kind");
	m_ctl_comboItemKindList.AddString("-----------------");
	// 2008-06-03 by cmkwon, AdminTool, DBTool 사용시 아이템 검색시 콤보박스에서 검색 기능 추가(K0000143) - 
	//m_ctl_comboItemKindList.AddString("ALL");
	m_ctl_comboItemKindList.AddString(CAtumSJ::GetItemKindName(ITEMKIND_ALL_ITEM));		// 2008-06-03 by cmkwon, AdminTool, DBTool 사용시 아이템 검색시 콤보박스에서 검색 기능 추가(K0000143) - 
	for (int i = 0; g_arrItemKindString[i].byItemKind0 != ITEMKIND_UNKNOWN; i++)
	{
		if(IS_GENERAL_ITEM(g_arrItemKindString[i].byItemKind0))
		{
			m_ctl_comboItemKindList.AddString(g_arrItemKindString[i].szItemKindName);
		}
	}
	m_ctl_comboItemKindList.SetCurSel(0);

// 2008-06-03 by cmkwon, AdminTool, DBTool 사용시 아이템 검색시 콤보박스에서 검색 기능 추가(K0000143) - 아래와 같이 수정
// 	// 아이템네임 리스트 생성
// 	m_ctl_comboItemNameList.AddString("All Item");
// 	m_ctl_comboItemNameList.AddString("-----------------");
// 	ez_map<string, ITEM>::iterator itr = m_pMapItemInfoName->begin();
// 	while (m_pMapItemInfoName->end() != itr)
// 	{
// 		ITEM *pItem = &itr->second;
// 
// 		if (IS_GENERAL_ITEM(pItem->Kind))
// 		{
// 			m_ctl_comboItemNameList.AddString(pItem->ItemName);
// 		}
// 
// 		itr++;
// 	}
// 	m_ctl_comboItemNameList.SetCurSel(0);
	///////////////////////////////////////////////////////////////////////////////
	// 2008-06-03 by cmkwon, AdminTool, DBTool 사용시 아이템 검색시 콤보박스에서 검색 기능 추가(K0000143) - 
	this->OnSelchangeComboItemKind();


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CStatisticPage61Dlg::OnSelchangeComboItemKind() 
{
	// TODO: Add your control notification handler code here

	///////////////////////////////////////////////////////////////////////////////
	// 2008-06-03 by cmkwon, AdminTool, DBTool 사용시 아이템 검색시 콤보박스에서 검색 기능 추가(K0000143) - 
	m_ctl_strItemInfo.Empty();
	UpdateData(FALSE);

	BYTE byItemKind = this->GetSelectedItemKindFromItemKindComboBox();
	this->ResetItemInfoList(byItemKind);

// 2008-06-03 by cmkwon, AdminTool, DBTool 사용시 아이템 검색시 콤보박스에서 검색 기능 추가(K0000143) - 위와 같이 수정 함
// 	int nComboIdx = m_ctl_comboItemKindList.GetCurSel();
// 	if (nComboIdx < COMBO_ITEMKIND_OFFSET && nComboIdx != COMBO_ITEMKIND_ALL)
// 	{
// 		return;
// 	}
// 
// 	CString csItemKindName;	
// 	m_ctl_comboItemKindList.GetWindowText(csItemKindName);
// 	int nItemKind = CAtumSJ::GetItemKindByItemKindName((LPSTR)(LPCSTR)csItemKindName);
// 
// 	// 아이템 리스트 생성
// 	m_ctl_comboItemNameList.ResetContent();
// 	CString tmpStr;
// 	tmpStr.Format(STRMSG_S_SCADMINTOOL_0043, csItemKindName);
// 	m_ctl_comboItemNameList.AddString((LPCSTR)tmpStr);
// 	m_ctl_comboItemNameList.AddString("-----------------");
// 	ez_map<string, ITEM>::iterator itr = m_pMapItemInfoName->begin();
// 	while (m_pMapItemInfoName->end() != itr)
// 	{
// 		ITEM *pItem = &itr->second;
// 
// 		if (IS_GENERAL_ITEM(pItem->Kind)
// 			&& (nComboIdx == COMBO_ITEMKIND_ALL || pItem->Kind == nItemKind))
// 		{
// 			m_ctl_comboItemNameList.AddString(pItem->ItemName);
// 		}
// 
// 		itr++;
// 	}
// 	m_ctl_comboItemNameList.SetCurSel(0);
}

void CStatisticPage61Dlg::OnSelchangeComboItemName() 
{
	// TODO: Add your control notification handler code here
	m_ctl_strItemInfo.Empty();
	UpdateData(FALSE);

	int nItem = m_ctl_comboItemNameList.GetCurSel();
	if (nItem < 2)
	{
		return;
	}

	CString itemName;
	m_ctl_comboItemNameList.GetLBText(nItem, itemName);
	ITEM *pItem = m_pMapItemInfoName->findEZ_ptr((LPCSTR)itemName);

	// 2008-06-03 by cmkwon, AdminTool, DBTool 사용시 아이템 검색시 콤보박스에서 검색 기능 추가(K0000143) - 아이템 선택시 ItemKind 까지 설정하는 것 제외
	//m_ctl_comboItemKindList.SetCurSel(pItem->Kind+COMBO_ITEMKIND_OFFSET);	

	m_ctl_strItemInfo.Format("[%8d]%s", pItem->ItemNum, pItem->ItemName);
	UpdateData(FALSE);
}

void CStatisticPage61Dlg::OnBtnSearch() 
{
	// TODO: Add your control notification handler code here
	UpdateData();

	int nItem = m_ctl_comboItemNameList.GetCurSel();
	if (nItem < 2)
	{
		MessageBox(STRERR_S_SCADMINTOOL_0039);
		return;
	}

	CString itemName;
	m_ctl_comboItemNameList.GetLBText(nItem, itemName);
	ITEM *pItem = m_pMapItemInfoName->findEZ_ptr((LPCSTR)itemName);
	if (pItem == NULL)
	{
		MessageBox(STRERR_S_SCADMINTOOL_0039);
		return;
	}

	if(FALSE == IS_COUNTABLE_ITEM(pItem->Kind))
	{
		ViewNonCountableItemTotalCount(pItem->ItemNum);
		ViewNonCountableItemOwnerList(pItem->ItemNum);
	}
	else
	{
		ViewCountableItemTotalCount(pItem->ItemNum);
		ViewCountableItemOwnerList(pItem->ItemNum);
	}
}

void CStatisticPage61Dlg::OnBtnSaveAsFile() 
{
	// TODO: Add your control notification handler code here
	CString tempStr;
	CString strTm;
	GetDlgItem(IDC_STATIC_1)->GetWindowText(strTm);
	tempStr.Format("%s	%10.0f", strTm, m_ctl_doTotalCount);
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


///////////////////////////////////////////////////////////////////////////////
/// \fn			void CStatisticPage61Dlg::ViewCountableItemOwnerList(INT i_ItemNum)
/// \brief		
/// \author		cmkwon
/// \date		2006-01-19 ~ 2006-01-19
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CStatisticPage61Dlg::ViewCountableItemOwnerList(INT i_ItemNum)
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
	m_gridView.SetColumnCount(7);
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
	Item.strText.Format("Item Count");
	m_gridView.SetItem(&Item);

	CString szSQLQuery;
	// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
	//szSQLQuery.Format("DROP VIEW VItemCount");
	szSQLQuery.Format(QUERY_080702_0181);
// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
//	BOOL bRet = m_pODBCStmt->ExecuteQuery((LPCSTR)szSQLQuery);
	SQLHSTMT hstmt = m_pODBCStmt->GetSTMTHandle();
	SQLINTEGER arrCB2[2] = {SQL_NTS,SQL_NTS};
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SZQUERY, 0, (LPSTR)(LPCSTR)szSQLQuery, 0,			&arrCB2[1]);
	BOOL bRet = m_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_080827_0122));
	m_pODBCStmt->FreeStatement();

// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
// 	szSQLQuery.Format("CREATE VIEW VItemCount AS \
// 						SELECT TOP 100 s.accountuniqueNumber, SUM(CAST(s.CurrentCount AS FLOAT)) as Count \
// 							FROM td_store s, td_account a WITH (NOLOCK) \
// 							WHERE s.ItemNum = %d AND s.accountuniquenumber = a.accountuniquenumber AND a.accountType < 128 \
// 							GROUP BY s.accountuniqueNumber, s.ItemNum \
// 							ORDER BY SUM(CAST(s.CurrentCount AS FLOAT)) DESC", i_ItemNum);
	szSQLQuery.Format(QUERY_080702_0182, i_ItemNum);
// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
//	bRet = m_pODBCStmt->ExecuteQuery((LPCSTR)szSQLQuery);
	arrCB2[1] = SQL_NTS;
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SZQUERY, 0, (LPSTR)(LPCSTR)szSQLQuery, 0,			&arrCB2[1]);
	bRet = m_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_080827_0122));
	m_pODBCStmt->FreeStatement();
	if(FALSE == bRet)
	{
		MessageBox("Error");
		return;
	}

// 2007-08-09 by cmkwon, 삭제된 캐릭터 처리(RACE_DELETED_CHARACTER) - 검색 결과에서 제외 한다(0 = c.Race&%d AND )
// 	szSQLQuery.Format("SELECT c.accountname, c.charactername, dbo.atum_GetGearString(c.unitkind) as unit, c.InfluenceType, c.level, tm.Count \
// 						FROM td_character c, (SELECT accountuniquenumber, max(experience) as maxexp, count(*) as numofcharacter	FROM td_character \
// 													WHERE 0 = Race&%d AND race < 128 and charactername not like 'loadtest%%'	group by accountuniquenumber) c2, td_account a, VItemCount tm \
// 						WHERE 0 = c.Race&%d AND c.Race < 128 AND c.accountuniquenumber = c2.accountuniquenumber and c.experience >= c2.maxexp AND (c.AccountUniqueNumber in (select AccountUniqueNumber from VItemCount)) \
// 							AND a.accountuniquenumber = c.accountuniquenumber and tm.AccountUniqueNumber = c.accountuniquenumber \
// 						ORDER BY tm.Count desc", RACE_DELETED_CHARACTER, RACE_DELETED_CHARACTER);
// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
//	szSQLQuery.Format(QUERY_080702_0183, RACE_DELETED_CHARACTER, RACE_DELETED_CHARACTER);
//	bRet = m_pODBCStmt->ExecuteQuery((LPCSTR)szSQLQuery);
	USHORT tmExcludeRace = RACE_DELETED_CHARACTER;
	arrCB2[1] = SQL_NTS;
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_USHORT, SQL_SMALLINT, 0, 0, &(tmExcludeRace), 0,				&arrCB2[1]);
	bRet = m_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_080827_0183));
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
	float fTotalCount = 0;
	
	SQLBindCol(m_pODBCStmt->m_hstmt, 1, SQL_C_CHAR, szAccName, SIZE_MAX_ACCOUNT_NAME,		&arrCB[1]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 2, SQL_C_CHAR, szCharName, SIZE_MAX_CHARACTER_NAME,	&arrCB[2]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 3, SQL_C_CHAR, szUnitkind, 10,						&arrCB[3]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 4, SQL_C_TINYINT, &byInfluenceType, 0,					&arrCB[4]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 5, SQL_C_TINYINT, &byLevel, 0,							&arrCB[5]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 6, SQL_C_FLOAT, &fTotalCount, 0,						&arrCB[6]);

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
		Item.strText.Format("%10.0f", fTotalCount);
		m_gridView.SetItem(&Item);
	}
	m_gridView.UpdateData();

	m_pODBCStmt->FreeStatement();	// clean up

	m_gridView.AutoSize();
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			void CStatisticPage61Dlg::ViewNonCountableItemOwnerList(INT i_ItemNum)
/// \brief		
/// \author		cmkwon
/// \date		2006-01-19 ~ 2006-01-19
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CStatisticPage61Dlg::ViewNonCountableItemOwnerList(INT i_ItemNum)
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
	m_gridView.SetColumnCount(7);
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
	Item.strText.Format("Item Count");
	m_gridView.SetItem(&Item);

	CString szSQLQuery;
	// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
	//szSQLQuery.Format("DROP VIEW VItemCount");
	szSQLQuery.Format(QUERY_080702_0184);
// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
//	BOOL bRet = m_pODBCStmt->ExecuteQuery((LPCSTR)szSQLQuery);
	SQLHSTMT hstmt = m_pODBCStmt->GetSTMTHandle();
	SQLINTEGER arrCB2[2] = {SQL_NTS,SQL_NTS};
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SZQUERY, 0, (LPSTR)(LPCSTR)szSQLQuery, 0,			&arrCB2[1]);
	BOOL bRet = m_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_080827_0122));
	m_pODBCStmt->FreeStatement();

// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
// 	szSQLQuery.Format("CREATE VIEW VItemCount AS \
// 						SELECT TOP 100 s.accountuniqueNumber, count(s.ItemNum) as Count \
// 							FROM td_store s, td_account a WITH (NOLOCK) \
// 							WHERE s.ItemNum = %d AND s.accountuniquenumber = a.accountuniquenumber AND a.accountType < 128 \
// 							GROUP BY s.accountuniqueNumber, s.ItemNum \
// 							ORDER BY count(*) DESC", i_ItemNum);
	szSQLQuery.Format(QUERY_080702_0185, i_ItemNum);
// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
//	bRet = m_pODBCStmt->ExecuteQuery((LPCSTR)szSQLQuery);
	arrCB2[1] = SQL_NTS;
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SZQUERY, 0, (LPSTR)(LPCSTR)szSQLQuery, 0,			&arrCB2[1]);
	bRet = m_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_080827_0122));
	m_pODBCStmt->FreeStatement();
	if(FALSE == bRet)
	{
		MessageBox("Error");
		return;
	}

	// 2007-08-09 by cmkwon, 삭제된 캐릭터 처리(RACE_DELETED_CHARACTER) - 검색 결과에서 제외 한다(0 = c.Race&%d AND )
// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
// 	szSQLQuery.Format("SELECT c.accountname, c.charactername, dbo.atum_GetGearString(c.unitkind) as unit, c.InfluenceType, c.level, tm.Count \
// 							FROM td_character c, (SELECT accountuniquenumber, max(experience) as maxexp, count(*) as numofcharacter	FROM td_character \
// 													WHERE 0 = Race&%d AND race < 128 and charactername not like 'loadtest%%'	group by accountuniquenumber) c2, td_account a, VItemCount tm \
// 							WHERE 0 = c.Race&%d AND c.Race < 128 and c.accountuniquenumber = c2.accountuniquenumber and c.experience >= c2.maxexp AND (c.AccountUniqueNumber in (select AccountUniqueNumber from VItemCount)) \
// 									AND a.accountuniquenumber = c.accountuniquenumber and tm.AccountUniqueNumber = c.accountuniquenumber \
// 							ORDER BY tm.Count desc", RACE_DELETED_CHARACTER, RACE_DELETED_CHARACTER);
// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
//	szSQLQuery.Format(QUERY_080702_0186, RACE_DELETED_CHARACTER, RACE_DELETED_CHARACTER);
//	bRet = m_pODBCStmt->ExecuteQuery((LPCSTR)szSQLQuery);
	USHORT tmExcludeRace = RACE_DELETED_CHARACTER;
	arrCB2[1] = SQL_NTS;
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_USHORT, SQL_SMALLINT, 0, 0, &(tmExcludeRace), 0,				&arrCB2[1]);
	bRet = m_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_080827_0186));
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
	float fTotalCount = 0;
	
	SQLBindCol(m_pODBCStmt->m_hstmt, 1, SQL_C_CHAR, szAccName, SIZE_MAX_ACCOUNT_NAME,		&arrCB[1]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 2, SQL_C_CHAR, szCharName, SIZE_MAX_CHARACTER_NAME,	&arrCB[2]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 3, SQL_C_CHAR, szUnitkind, 10,						&arrCB[3]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 4, SQL_C_TINYINT, &byInfluenceType, 0,					&arrCB[4]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 5, SQL_C_TINYINT, &byLevel, 0,							&arrCB[5]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 6, SQL_C_FLOAT, &fTotalCount, 0,						&arrCB[6]);

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
		Item.strText.Format("%10.0f", fTotalCount);
		m_gridView.SetItem(&Item);
	}
	m_gridView.UpdateData();

	m_pODBCStmt->FreeStatement();	// clean up

	m_gridView.AutoSize();
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			void CStatisticPage61Dlg::ViewCountableItemTotalCount(INT i_ItemNum)
/// \brief		
/// \author		cmkwon
/// \date		2006-01-19 ~ 2006-01-19
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CStatisticPage61Dlg::ViewCountableItemTotalCount(INT i_ItemNum)
{
	m_ctl_doTotalCount	= 0;

	CString szSQLQuery;
// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
// 	szSQLQuery.Format("SELECT SUM(CAST(CurrentCount AS FLOAT)) \
// 							FROM td_Store s, td_account a WITH (NOLOCK)\
// 							WHERE s.ItemNum= %d and s.AccountUniqueNumber = a.AccountUniqueNumber and a.AccountType < 128 \
// 							GROUP by s.ItemNum", i_ItemNum);
// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
// 	szSQLQuery.Format(QUERY_080702_0187, i_ItemNum);
// 	float fTotalCount = 0;
// 	SQLBindCol(m_pODBCStmt->m_hstmt, 1, SQL_C_FLOAT, &fTotalCount, 0,	NULL);
// 	BOOL bRet = m_pODBCStmt->ExecuteQuery((LPCSTR)szSQLQuery);
// 	if (!bRet)
// 	{
// 		// clean up
// 		m_pODBCStmt->FreeStatement();
// 		MessageBox("Error");
// 		return;
// 	}
	SQLHSTMT hstmt = m_pODBCStmt->GetSTMTHandle();
	SQLINTEGER arrCB2[2] = {SQL_NTS,SQL_NTS};
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &(i_ItemNum), 0,			&arrCB2[1]);
	BOOL bRet = m_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_080827_0187));
	if(FALSE == bRet)
	{
 		m_pODBCStmt->FreeStatement();
 		MessageBox("Error");
 		return;
	}
 	float fTotalCount = 0;
 	SQLBindCol(m_pODBCStmt->m_hstmt, 1, SQL_C_FLOAT, &fTotalCount, 0,	NULL);
	bRet = SQLFetch(m_pODBCStmt->m_hstmt);
	if(SQL_NO_DATA != bRet)
	{
		m_ctl_doTotalCount = fTotalCount;
	}
	m_pODBCStmt->FreeStatement();	// clean up
	UpdateData(FALSE);
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			void CStatisticPage61Dlg::ViewNonCountableItemTotalCount(INT i_ItemNum)
/// \brief		
/// \author		cmkwon
/// \date		2006-01-19 ~ 2006-01-19
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CStatisticPage61Dlg::ViewNonCountableItemTotalCount(INT i_ItemNum)
{
	m_ctl_doTotalCount	= 0;

	CString szSQLQuery;
// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
// 	szSQLQuery.Format("SELECT COUNT(s.ItemNum) \
// 							FROM td_Store s, td_account a WITH (NOLOCK)\
// 							WHERE s.ItemNum= %d and s.AccountUniqueNumber = a.AccountUniqueNumber and a.AccountType < 128 \
// 							GROUP by s.ItemNum", i_ItemNum);
// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
// 	szSQLQuery.Format(QUERY_080702_0188, i_ItemNum);
// 	float fTotalCount = 0;
// 	SQLBindCol(m_pODBCStmt->m_hstmt, 1, SQL_C_FLOAT, &fTotalCount, 0,	NULL);
// 	BOOL bRet = m_pODBCStmt->ExecuteQuery((LPCSTR)szSQLQuery);
// 	if (!bRet)
// 	{
// 		// clean up
// 		m_pODBCStmt->FreeStatement();
// 		MessageBox("Error");
// 		return;
// 	}
	SQLHSTMT hstmt = m_pODBCStmt->GetSTMTHandle();
	SQLINTEGER arrCB2[2] = {SQL_NTS,SQL_NTS};
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &(i_ItemNum), 0,			&arrCB2[1]);
	BOOL bRet = m_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_080827_0187));
	if(FALSE == bRet)
	{
		m_pODBCStmt->FreeStatement();
		MessageBox("Error");
		return;
	}
	float fTotalCount = 0;
	SQLBindCol(m_pODBCStmt->m_hstmt, 1, SQL_C_FLOAT, &fTotalCount, 0,	NULL);
	bRet = SQLFetch(m_pODBCStmt->m_hstmt);
	if(SQL_NO_DATA != bRet)
	{
		m_ctl_doTotalCount = fTotalCount;
	}
	m_pODBCStmt->FreeStatement();	// clean up
	UpdateData(FALSE);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BYTE CStatisticPage61Dlg::GetSelectedItemKindFromItemKindComboBox(void)
/// \brief		// 2008-06-03 by cmkwon, AdminTool, DBTool 사용시 아이템 검색시 콤보박스에서 검색 기능 추가(K0000143) - CStatisticPage61Dlg::GetSelectedItemKindFromItemKindComboBox() 추가
/// \author		cmkwon
/// \date		2008-06-03 ~ 2008-06-03
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BYTE CStatisticPage61Dlg::GetSelectedItemKindFromItemKindComboBox(void)
{
	BYTE byItemKind		= ITEMKIND_ALL_ITEM;
	int nComboIdx		= m_ctl_comboItemKindList.GetCurSel();
	if (nComboIdx <= COMBO_ITEMKIND_OFFSET)
	{
		return byItemKind;
	}

	CString csItemKindName;	
	m_ctl_comboItemKindList.GetWindowText(csItemKindName);
	byItemKind = CAtumSJ::GetItemKindByItemKindName((LPSTR)(LPCSTR)csItemKindName);
	if(ITEMKIND_UNKNOWN == byItemKind)
	{
		byItemKind = ITEMKIND_ALL_ITEM;
	}

	return byItemKind;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CStatisticPage61Dlg::ResetItemInfoList(BYTE i_byItemKind, CString *i_pcsPartOfItemName/*=NULL*/)
/// \brief		// 2008-06-03 by cmkwon, AdminTool, DBTool 사용시 아이템 검색시 콤보박스에서 검색 기능 추가(K0000143) - 
/// \author		cmkwon
/// \date		2008-05-30 ~ 2008-05-30
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CStatisticPage61Dlg::ResetItemInfoList(BYTE i_byItemKind, CString *i_pcsPartOfItemName/*=NULL*/)
{
	if(ITEMKIND_UNKNOWN == i_byItemKind)
	{
		return FALSE;
	}

	// 아이템 리스트 생성
	m_ctl_comboItemNameList.ResetContent();
	CString tmpStr;
	tmpStr.Format(STRMSG_S_SCADMINTOOL_0043, CAtumSJ::GetItemKindName(i_byItemKind));
	m_ctl_comboItemNameList.AddString((LPCSTR)tmpStr);
	m_ctl_comboItemNameList.AddString("-----------------");
	ez_map<string, ITEM>::iterator itr = m_pMapItemInfoName->begin();
	while (m_pMapItemInfoName->end() != itr)
	{
		ITEM *pItem = &itr->second;
		
		if (IS_GENERAL_ITEM(pItem->Kind)
			&& (ITEMKIND_ALL_ITEM == i_byItemKind || pItem->Kind == i_byItemKind))
		{
			if(NULL == i_pcsPartOfItemName
				|| strstr(pItem->ItemName, *i_pcsPartOfItemName))
			{
				m_ctl_comboItemNameList.AddString(pItem->ItemName);
			}
		}
		
		itr++;
	}

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2008-06-03 by cmkwon, AdminTool, DBTool 사용시 아이템 검색시 콤보박스에서 검색 기능 추가(K0000143) - 
/// \author		cmkwon
/// \date		2008-06-03 ~ 2008-06-03
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CStatisticPage61Dlg::OnDropdownComboItemName() 
{
	// TODO: Add your control notification handler code here
	BYTE byItemKind = this->GetSelectedItemKindFromItemKindComboBox();
	
	CString csSearchName;
	if(FALSE == util::in_range(0, m_ctl_comboItemNameList.GetCurSel(), 1))
	{
		m_ctl_comboItemNameList.GetWindowText(csSearchName);
		if(csSearchName.IsEmpty())
		{
			this->ResetItemInfoList(byItemKind);
			return;
		}
	}

	this->ResetItemInfoList(byItemKind, &csSearchName);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2008-06-03 by cmkwon, AdminTool, DBTool 사용시 아이템 검색시 콤보박스에서 검색 기능 추가(K0000143) - 
/// \author		cmkwon
/// \date		2008-06-03 ~ 2008-06-03
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CStatisticPage61Dlg::OnEditchangeComboItemName() 
{
	// TODO: Add your control notification handler code here
	m_ctl_comboItemNameList.ShowDropDown(FALSE);	// DropDown 리스트를 닫는다. 검색을 위해서
}
