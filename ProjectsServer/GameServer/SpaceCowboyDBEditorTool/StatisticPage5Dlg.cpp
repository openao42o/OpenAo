// StatisticPage5Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "spacecowboydbeditortool.h"
#include "StatisticPage5Dlg.h"
#include "SpaceCowboyDBEditorToolDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStatisticPage5Dlg property page

IMPLEMENT_DYNCREATE(CStatisticPage5Dlg, CPropertyPage)

CStatisticPage5Dlg::CStatisticPage5Dlg() : CPropertyPage(CStatisticPage5Dlg::IDD)
{
	//{{AFX_DATA_INIT(CStatisticPage5Dlg)
	m_TotalSellCash = 0;
	m_ctl_CreatedDateStart = COleDateTime::GetCurrentTime();
	m_ctl_CreateTimeStart = COleDateTime::GetCurrentTime();
	m_ctl_CreateDateEnd = COleDateTime::GetCurrentTime();
	m_ctl_CreateTimeEnd = COleDateTime::GetCurrentTime();
	//}}AFX_DATA_INIT
	m_pMainDlg = (CSpaceCowboyDBEditorToolDlg*)AfxGetMainWnd();
// 2007-11-07 by cmkwon, 로그 DB 서버 따로 구축하기 - 하나의 연결로 처리
//	m_pODBCStmt1		= new CODBCStatement();
	// 2008-03-20 by cmkwon, 버그 수정 - Cash Item 검색시는 따로 연결을 해야 한다.
	//m_pODBCStmt1		= m_pMainDlg->m_pODBCStmt;		// 2007-11-07 by cmkwon, 로그 DB 서버 따로 구축하기 - 하나의 연결로 처리
	m_pODBCStmt1		= new CODBCStatement();			// 2008-03-20 by cmkwon, 버그 수정 - Cash Item 검색시는 따로 연결을 해야 한다.

	m_pMapIteminfo		= &m_pMainDlg->m_mapItemInfo;
}

CStatisticPage5Dlg::~CStatisticPage5Dlg()
{

// 2007-11-07 by cmkwon, 로그 DB 서버 따로 구축하기 - 하나의 연결로 처리
//	util::del(m_pODBCStmt1);

	util::del(m_pODBCStmt1);		// 2008-03-20 by cmkwon, 버그 수정 - Cash Item 검색시는 따로 연결을 해야 한다.
}

void CStatisticPage5Dlg::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStatisticPage5Dlg)
	DDX_Text(pDX, IDC_EDIT_TotalSellCash, m_TotalSellCash);
	DDX_Control(pDX, IDC_CUSTOM_STA_CASHITEM, m_GridStaCashItem);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER1, m_ctl_CreatedDateStart);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER2, m_ctl_CreateTimeStart);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER3, m_ctl_CreateDateEnd);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER4, m_ctl_CreateTimeEnd);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CStatisticPage5Dlg, CPropertyPage)
	//{{AFX_MSG_MAP(CStatisticPage5Dlg)
	ON_BN_CLICKED(IDC_BUTTON_ST5_SELECT, OnButtonSt5Select)
	ON_BN_CLICKED(IDC_BUTTON_ST5_SAVE, OnButtonSt5Save)
//	ON_BN_CLICKED(IDC_CHECK1, OnCheck1)
//	ON_BN_CLICKED(IDC_CHECK2, OnCheck2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStatisticPage5Dlg message handlers

void CStatisticPage5Dlg::OnButtonSt5Select() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

// 2007-11-07 by cmkwon, 로그 DB 서버 따로 구축하기 - 하나의 연결로 처리	
//	if (!m_pODBCStmt1->Init(m_pMainDlg->m_pServerInfo4Admin->DBIP, m_pMainDlg->m_pServerInfo4Admin->DBPort, m_pMainDlg->m_pServerInfo4Admin->DBName,
//							m_pMainDlg->m_pServerInfo4Admin->DBUID, m_pMainDlg->m_pServerInfo4Admin->DBPWD, GetSafeHwnd())
//	)
//	{
//		return;
//	}

	///////////////////////////////////////////////////////////////////////////////
	// 2008-03-20 by cmkwon, 버그 수정 - Cash Item 검색시는 따로 연결을 해야 한다.
	if( FALSE == m_pODBCStmt1->Init(m_pMainDlg->m_pServerInfo4Admin->LogDBIP, m_pMainDlg->m_pServerInfo4Admin->LogDBPort, m_pMainDlg->m_pServerInfo4Admin->DBName,
								m_pMainDlg->m_pServerInfo4Admin->LogDBUID, m_pMainDlg->m_pServerInfo4Admin->LogDBPWD, GetSafeHwnd()) )
	{
		return;
	}

	SQLINTEGER	cb1,cb2,cb3;
	int			szItemNum;
	int			szSellCount;
	int			szSellCash;
	BOOL		check = 0;
	_m_data		m_data;
	list<_m_data> m_listdata;
	int			TotalSellCash = 0;

	CString		tDBNAME;
	CString		szStartDate, szEndDate; 
	
// 2008-01-24 by cmkwon, S_A: 날짜 관련 로컬리 제이션 처리 버그 수정 - 아래와 같은 형식으로 수정
// 	szStartDate.Format("%04d-%02d-%02d %02d:%02d:%02d",
// 		m_ctl_CreatedDateStart.GetYear(),m_ctl_CreatedDateStart.GetMonth(),m_ctl_CreatedDateStart.GetDay(),
// 		m_ctl_CreateTimeStart.GetHour(),m_ctl_CreateTimeStart.GetMinute(),m_ctl_CreateTimeStart.GetSecond());
// 	szEndDate.Format("%04d-%02d-%02d %02d:%02d:%02d",
// 		m_ctl_CreateDateEnd.GetYear(),m_ctl_CreateDateEnd.GetMonth(),m_ctl_CreateDateEnd.GetDay(),
// 		m_ctl_CreateTimeEnd.GetHour(),m_ctl_CreateTimeEnd.GetMinute(),m_ctl_CreateTimeEnd.GetSecond());
// 2008-08-28 by cmkwon, MySQL 관련 날짜시간 관련 오류 수정 - 
//	szStartDate.Format("%04d%02d%02d %02d:%02d:%02d",
	szStartDate.Format(SQL_DATETIME_STRING_FORMAT,
		m_ctl_CreatedDateStart.GetYear(),m_ctl_CreatedDateStart.GetMonth(),m_ctl_CreatedDateStart.GetDay(),
		m_ctl_CreateTimeStart.GetHour(),m_ctl_CreateTimeStart.GetMinute(),m_ctl_CreateTimeStart.GetSecond());
// 2008-08-28 by cmkwon, MySQL 관련 날짜시간 관련 오류 수정 - 
//	szEndDate.Format("%04d%02d%02d %02d:%02d:%02d",
	szEndDate.Format(SQL_DATETIME_STRING_FORMAT,
		m_ctl_CreateDateEnd.GetYear(),m_ctl_CreateDateEnd.GetMonth(),m_ctl_CreateDateEnd.GetDay(),
		m_ctl_CreateTimeEnd.GetHour(),m_ctl_CreateTimeEnd.GetMinute(),m_ctl_CreateTimeEnd.GetSecond());

	CString szSQLQuery;

	for(int sernum=1; sernum <= SIZE_MAX_SERVER_GROUP_COUNT; sernum++)
	{
		// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
		//szSQLQuery.Format("use atum2_db_%d", sernum);
		szSQLQuery.Format(QUERY_080702_0179, sernum);
		
		BOOL bRet = m_pODBCStmt1->ExecuteQuery((LPCSTR)szSQLQuery);
		m_pODBCStmt1->FreeStatement();			// clean up		
		if (!bRet)
		{
			continue;
		}		

		// 2007-09-17 by cmkwon, 쿼리수정
		//#define ITEM_ATTR_CASH_ITEM				0x00010000		// 65536, 유료화 아이템
		// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
		//szSQLQuery.Format("select i.itemNum, count(i.itemNum) as Sellcount, count(i.ItemNum)*i.CashPrice as SellCash \
		//	from atum_log_item_buy_sell il, ti_item i WITH (NOLOCK) where il.ItemNum = i.itemNum and 0 <> i.ItemAttribute & 65536 and il.time >= '%s' and il.time <= '%s' \
		//	group by i.itemNum, i.CashPrice", szStartDate,szEndDate);
// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
//		szSQLQuery.Format(QUERY_080702_0180, szStartDate,szEndDate);		
//		bRet = m_pODBCStmt1->ExecuteQuery((LPCSTR)szSQLQuery);
		SQLHSTMT hstmt = m_pODBCStmt1->GetSTMTHandle();
		SQLINTEGER arrCB2[3] = {SQL_NTS,SQL_NTS,SQL_NTS};
		SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SQL_DATETIME_STRING, 0, (LPSTR)(LPCSTR)szStartDate, 0,		&arrCB2[1]);
		SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SQL_DATETIME_STRING, 0, (LPSTR)(LPCSTR)szEndDate, 0,			&arrCB2[2]);
		bRet = m_pODBCStmt1->ExecuteQuery((char*)(PROCEDURE_080827_0180));
		if (!bRet)
		{
			m_pODBCStmt1->FreeStatement();			// clean up

			char szTemp[1024];
			sprintf(szTemp, "DB \'atum2_db_%d\' error !!", sernum);
			MessageBox(szTemp);
			continue;
		}
		
		SQLBindCol(m_pODBCStmt1->m_hstmt, 1, SQL_C_ULONG, &szItemNum,	   0,	&cb1);
		SQLBindCol(m_pODBCStmt1->m_hstmt, 2, SQL_C_ULONG, &szSellCount,    0,	&cb2);
		SQLBindCol(m_pODBCStmt1->m_hstmt, 3, SQL_C_ULONG, &szSellCash,     0,	&cb3);
		// DB에 값이 없을때까지 loop를 돈다
		while ( (bRet = SQLFetch(m_pODBCStmt1->m_hstmt)) != SQL_NO_DATA)
		{
			list<_m_data>::iterator  itr = m_listdata.begin();
			while (itr != m_listdata.end())
			{
				
				if ( (itr->m_ItemNum) == szItemNum )
				{
					itr->m_SellCount	= itr->m_SellCount+szSellCount;
					itr->m_SellCash		= itr->m_SellCash+szSellCash;
					TotalSellCash = TotalSellCash + szSellCash;
					check	= 1;
					break;
				}
				itr++;
			}
			
			if (check != 1)
			{
				m_data.m_ItemNum	= szItemNum;
				m_data.m_SellCount  = szSellCount;
				m_data.m_SellCash	= szSellCash;
				m_listdata.push_back(m_data);
				TotalSellCash = TotalSellCash + szSellCash;
			}
			check = 0;
		}
		m_pODBCStmt1->FreeStatement();
	}
	m_pODBCStmt1->Clean();
	
	m_GridStaCashItem.SetBkColor(0xFFFFFF);
	m_GridStaCashItem.SetHeaderSort();

	m_GridStaCashItem.SetEditable(FALSE);
	m_GridStaCashItem.SetListMode(TRUE);
	m_GridStaCashItem.SetSingleRowSelection(TRUE);
	m_GridStaCashItem.EnableSelection(TRUE);
	m_GridStaCashItem.SetFrameFocusCell(FALSE);
	m_GridStaCashItem.SetTrackFocusCell(FALSE);

	m_GridStaCashItem.SetRowCount(1);
	m_GridStaCashItem.SetColumnCount(5);
	m_GridStaCashItem.SetFixedRowCount(1);

	// 칼럼 만들기
	GV_ITEM Item;
	Item.mask = GVIF_TEXT|GVIF_FORMAT;
	Item.nFormat = DT_LEFT|DT_VCENTER|DT_SINGLELINE;
	Item.row = 0;

	Item.col = 0;
	Item.strText.Format("Num");
	m_GridStaCashItem.SetItem(&Item);
	Item.col = 1;
	Item.strText.Format("ItemInfo");
	m_GridStaCashItem.SetItem(&Item);
	Item.col = 2;
	Item.strText.Format("SellCount");
	m_GridStaCashItem.SetItem(&Item);
	Item.col = 3;
	Item.strText.Format("EachSellPrice");
	m_GridStaCashItem.SetItem(&Item);
	Item.col = 4;
	Item.strText.Format("SellPrice");
	m_GridStaCashItem.SetItem(&Item);
	
	list<_m_data>::iterator itr = m_listdata.begin();
	while ( itr != m_listdata.end() )
	{
		int nNewRowIdx = m_GridStaCashItem.GetRowCount();
		m_GridStaCashItem.SetRowCount(nNewRowIdx+1);

		//select된 값을 GridDetail GridCtrl에 넣어준다.
		Item.row		= nNewRowIdx;
		Item.col		= 0;
		Item.strText.Format("%d", nNewRowIdx);
		m_GridStaCashItem.SetItem(&Item);

		Item.col		= 1;
		if(m_pMapIteminfo->findEZ_ptr(itr->m_ItemNum))
		{
			Item.strText.Format("[%d]  %s", itr->m_ItemNum, m_pMapIteminfo->findEZ_ptr(itr->m_ItemNum)->ItemName);
		}
		else
		{
			Item.strText.Format("%d", itr->m_ItemNum);
		}
		m_GridStaCashItem.SetItem(&Item);

		Item.col		= 2;
		Item.strText.Format("%d", itr->m_SellCount);
		m_GridStaCashItem.SetItem(&Item);

		Item.col		= 3;
		Item.strText.Format("%d", (itr->m_SellCash)/(itr->m_SellCount));
		m_GridStaCashItem.SetItem(&Item);
		
		Item.col		= 4;
		Item.strText.Format("%d", itr->m_SellCash);
		m_GridStaCashItem.SetItem(&Item);
		
		itr++;
	}
	m_TotalSellCash = TotalSellCash;
	
	m_GridStaCashItem.UpdateData();
	m_GridStaCashItem.AutoSize();
	UpdateData(FALSE);

//	UpdateData(TRUE);
//	if(m_valueCheckCashItemSell == TRUE)
//	{
// 2006-02-03 by cmkwon, 구현을 완료하지 않음
//		ViewCashItemSell();
//	}
//	if(m_valueCheckCashItemBuy == TRUE)
//	{
// 2006-02-03 by cmkwon, 구현을 완료하지 않음
//		ViewCashItemBuy();
//	}	
}

// 2006-02-03 by cmkwon, 구현을 완료하지 않음
//void CStatisticPage5Dlg::ViewCashItemSell()
//{
//	m_GridStaCashItem.SetBkColor(0xFFFFFF);
//	m_GridStaCashItem.SetHeaderSort();
//
//	int m_nRows = 1;
//	int m_nCols = 3;
//
//	m_GridStaCashItem.SetEditable(FALSE);
//	m_GridStaCashItem.SetListMode(TRUE);
//	m_GridStaCashItem.SetSingleRowSelection(TRUE);
//	m_GridStaCashItem.EnableSelection(TRUE);
//	m_GridStaCashItem.SetFrameFocusCell(FALSE);
//	m_GridStaCashItem.SetTrackFocusCell(FALSE);
//
//	m_GridStaCashItem.SetRowCount(m_nRows);
//	m_GridStaCashItem.SetColumnCount(m_nCols);
//	m_GridStaCashItem.SetFixedRowCount(1);
//
//	// 칼럼 만들기
//	m_nCols = 0;
//	GV_ITEM Item;
//	Item.mask = GVIF_TEXT|GVIF_FORMAT;
//	Item.row = 0;
//	Item.nFormat = DT_LEFT|DT_VCENTER|DT_SINGLELINE;
//
//	Item.col = m_nCols++;
//	Item.strText.Format("DATE");
//	m_GridStaCashItem.SetItem(&Item);
//
//	Item.col = m_nCols++;
//	Item.strText.Format("ITEMNUM");
//	m_GridStaCashItem.SetItem(&Item);
//
//	Item.col = m_nCols++;
//	Item.strText.Format("COUNT");
//	m_GridStaCashItem.SetItem(&Item);
//
//	//데이터 insert
//	//DB 접속
//// 2006-01-23 by cmkwon
////	if (!m_pODBCStmt1->Init(m_pMainDlg->m_pServerInfo4Admin->DBIP, m_pMainDlg->m_pServerInfo4Admin->DBPort, m_pMainDlg->m_pServerInfo4Admin->DBName,
////							m_pMainDlg->m_pServerInfo4Admin->DBUID, m_pMainDlg->m_pServerInfo4Admin->DBPWD, GetSafeHwnd())
////	)
////	{
////		return;
////	}
//	CString szSQLQuery;
//// 2006-02-03 by cmkwon, 수정 중에 Stop
////	szSQLQuery.Format("DROP VIEW Temp_View");
////	BOOL bRet = m_pODBCStmt1->ExecuteQuery((LPCSTR)szSQLQuery);
////
////	szSQLQuery.Format("CREATE VIEW Temp_View AS \
////		select left(convert(char(20), time,120),10), itemnum, sum(changecount) from atum_log_item_buy_sell where itemnum in (select itemnum from ti_item where itemattribute > 255) and logtype = 38 group by left(convert(char(20), time,120),10), itemnum order by left(convert(char(20), time,120),10), itemnum")
//
//	szSQLQuery.Format("select left(convert(char(20), time,120),10), itemnum, sum(changecount) from atum_log_item_buy_sell where itemnum in (select itemnum from ti_item where itemattribute > 255) and logtype = 38 group by left(convert(char(20), time,120),10), itemnum order by left(convert(char(20), time,120),10), itemnum");
//
//	SQLINTEGER cb1, cb2, cb3;
//
//	char tempDate[20];
//	memset(tempDate, 0, sizeof(tempDate));
//	int tempSelectData, tempSelectData2;
//	SQLBindCol(m_pODBCStmt1->m_hstmt, 1, SQL_C_CHAR, &tempDate, 20, &cb1);
//	SQLBindCol(m_pODBCStmt1->m_hstmt, 2, SQL_C_ULONG, &tempSelectData, 0, &cb2);
//	SQLBindCol(m_pODBCStmt1->m_hstmt, 3, SQL_C_ULONG, &tempSelectData2, 0, &cb3);
//
//	bRet = m_pODBCStmt1->ExecuteQuery((LPCSTR)szSQLQuery);
//	if (!bRet)
//	{
//		// clean up
//		m_pODBCStmt1->FreeStatement();
//		MessageBox("Error");
//		return;
//	}
//
//	int nCount = 0;
//	m_GridStaCashItem.SetRowCount(nCount+1);
//	// DB에 값이 없을때까지 loop를 돈다
//	while ( (bRet = SQLFetch(m_pODBCStmt1->m_hstmt)) != SQL_NO_DATA)
//	{
//		m_GridStaCashItem.SetRowCount(nCount+2);
//
//		//select된 값을 GridDetail GridCtrl에 넣어준다.
//		Item.col = 0;
//		Item.row = nCount+1;
//		Item.strText.Format("%s", tempDate);
//		m_GridStaCashItem.SetItem(&Item);
//
//		Item.col = 1;
//		Item.strText.Format("%d", tempSelectData);
//		m_GridStaCashItem.SetItem(&Item);
//
//		Item.col = 2;
//		Item.strText.Format("%d", tempSelectData2);
//		m_GridStaCashItem.SetItem(&Item);
//
//		m_GridStaCashItem.UpdateData();
//
//		nCount++;
//	}
//
//	// clean up
//	m_pODBCStmt1->FreeStatement();
//
//	m_GridStaCashItem.AutoSize();
//}
//
//void CStatisticPage5Dlg::ViewCashItemBuy()
//{
//	m_GridStaCashItem.SetBkColor(0xFFFFFF);
//	m_GridStaCashItem.SetHeaderSort();
//
//	int m_nRows = 1;
//	int m_nCols = 3;
//
//	m_GridStaCashItem.SetEditable(FALSE);
//	m_GridStaCashItem.SetListMode(TRUE);
//	m_GridStaCashItem.SetSingleRowSelection(TRUE);
//	m_GridStaCashItem.EnableSelection(TRUE);
//	m_GridStaCashItem.SetFrameFocusCell(FALSE);
//	m_GridStaCashItem.SetTrackFocusCell(FALSE);
//
//	m_GridStaCashItem.SetRowCount(m_nRows);
//	m_GridStaCashItem.SetColumnCount(m_nCols);
//	m_GridStaCashItem.SetFixedRowCount(1);
//
//	// 칼럼 만들기
//	m_nCols = 0;
//	GV_ITEM Item;
//	Item.mask = GVIF_TEXT|GVIF_FORMAT;
//	Item.row = 0;
//	Item.nFormat = DT_LEFT|DT_VCENTER|DT_SINGLELINE;
//
//	Item.col = m_nCols++;
//	Item.strText.Format("DATE");
//	m_GridStaCashItem.SetItem(&Item);
//
//	Item.col = m_nCols++;
//	Item.strText.Format("ITEMNUM");
//	m_GridStaCashItem.SetItem(&Item);
//
//	Item.col = m_nCols++;
//	Item.strText.Format("COUNT");
//	m_GridStaCashItem.SetItem(&Item);
//
//	//데이터 insert
//	//DB 접속
//// 2006-01-23 by cmkwon
////	if (!m_pODBCStmt1->Init(m_pMainDlg->m_pServerInfo4Admin->DBIP, m_pMainDlg->m_pServerInfo4Admin->DBPort, m_pMainDlg->m_pServerInfo4Admin->DBName,
////							m_pMainDlg->m_pServerInfo4Admin->DBUID, m_pMainDlg->m_pServerInfo4Admin->DBPWD, GetSafeHwnd())
////	)
////	{
////		return;
////	}
//	CString szSQLQuery;
//	szSQLQuery.Format("select left(convert(char(20), time,120),10), itemnum, sum(changecount) from atum_log_item_buy_sell where itemnum in (select itemnum from ti_item where itemattribute > 255) and logtype = 37 group by left(convert(char(20), time,120),10), itemnum order by left(convert(char(20), time,120),10), itemnum"
//		);
//
//	SQLINTEGER cb1, cb2, cb3;
//
//	char tempDate[20];
//	memset(tempDate, 0, sizeof(tempDate));
//	int tempSelectData, tempSelectData2;
//	SQLBindCol(m_pODBCStmt1->m_hstmt, 1, SQL_C_CHAR, &tempDate, 20, &cb1);
//	SQLBindCol(m_pODBCStmt1->m_hstmt, 2, SQL_C_ULONG, &tempSelectData, 0, &cb2);
//	SQLBindCol(m_pODBCStmt1->m_hstmt, 3, SQL_C_ULONG, &tempSelectData2, 0, &cb3);
//
//	BOOL bRet = m_pODBCStmt1->ExecuteQuery((LPCSTR)szSQLQuery);
//	if (!bRet)
//	{
//		// clean up
//		m_pODBCStmt1->FreeStatement();
//		MessageBox("Error");
//		return;
//	}
//
//	int nCount = 0;
//	m_GridStaCashItem.SetRowCount(nCount+1);
//	// DB에 값이 없을때까지 loop를 돈다
//	while ( (bRet = SQLFetch(m_pODBCStmt1->m_hstmt)) != SQL_NO_DATA)
//	{
//		m_GridStaCashItem.SetRowCount(nCount+2);
//
//		//select된 값을 GridDetail GridCtrl에 넣어준다.
//		Item.col = 0;
//		Item.row = nCount+1;
//		Item.strText.Format("%s", tempDate);
//		m_GridStaCashItem.SetItem(&Item);
//
//		Item.col = 1;
//		Item.strText.Format("%d", tempSelectData);
//		m_GridStaCashItem.SetItem(&Item);
//
//		Item.col = 2;
//		Item.strText.Format("%d", tempSelectData2);
//		m_GridStaCashItem.SetItem(&Item);
//
//		m_GridStaCashItem.UpdateData();
//
//		nCount++;
//	}
//
//	// clean up
//	m_pODBCStmt1->FreeStatement();
//
//	m_GridStaCashItem.AutoSize();
//}

void CStatisticPage5Dlg::OnButtonSt5Save() 
{
	// TODO: Add your control notification handler code here
	CString tempStr;
	for(int i = 0; i<m_GridStaCashItem.GetRowCount();i++)
	{
		tempStr = "";
		for(int j = 0; j< m_GridStaCashItem.GetColumnCount();j++)
		{
			tempStr += m_GridStaCashItem.GetItemText( i, j);
			tempStr += "|\t";
		}
		m_pMainDlg->m_pWriteLog->WriteLogFile((char*)(LPCTSTR)tempStr, tempStr.GetLength(), m_pMainDlg, 1);
	}
}
/*
void CStatisticPage5Dlg::OnCheck1() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if(m_valueCheckCashItemSell == TRUE)
	{
		m_valueCheckCashItemBuy = FALSE;
	}
	UpdateData(FALSE);
}

void CStatisticPage5Dlg::OnCheck2() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if(m_valueCheckCashItemBuy == TRUE)
	{
		m_valueCheckCashItemSell = FALSE;
	}
	UpdateData(FALSE);
}
*/
