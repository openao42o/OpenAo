// StatisticPage2Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "spacecowboydbeditortool.h"
#include "StatisticPage2Dlg.h"
#include "SpaceCowboyDBEditorToolDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStatisticPage2Dlg property page

IMPLEMENT_DYNCREATE(CStatisticPage2Dlg, CPropertyPage)

CStatisticPage2Dlg::CStatisticPage2Dlg() : CPropertyPage(CStatisticPage2Dlg::IDD)
{
	//{{AFX_DATA_INIT(CStatisticPage2Dlg)
	m_valueCheckTotalAccumAccount = FALSE;
	m_valueCheckCurrentAccount = FALSE;
	m_valueCheckUnregAccount = FALSE;
	m_valueCheckExpectAccount = FALSE;
	//}}AFX_DATA_INIT
	m_pMainDlg = (CSpaceCowboyDBEditorToolDlg*)AfxGetMainWnd();
	m_pODBCStmt				= m_pMainDlg->m_pODBCStmt;
}

CStatisticPage2Dlg::~CStatisticPage2Dlg()
{
// 2006-01-23 by cmkwon
//	util::del(m_pODBCStmt);
}

void CStatisticPage2Dlg::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStatisticPage2Dlg)
	DDX_Check(pDX, IDC_CHECK1, m_valueCheckTotalAccumAccount);
	DDX_Check(pDX, IDC_CHECK2, m_valueCheckCurrentAccount);
	DDX_Check(pDX, IDC_CHECK3, m_valueCheckUnregAccount);
	DDX_Control(pDX, IDC_CUSTOM_STA_ACCUMACCOUNT, m_GridStaAccumAccount);
	DDX_Check(pDX, IDC_CHECK4, m_valueCheckExpectAccount);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CStatisticPage2Dlg, CPropertyPage)
	//{{AFX_MSG_MAP(CStatisticPage2Dlg)
	ON_BN_CLICKED(IDC_BUTTON_ST2_SELECT, OnButtonSt2Select)
	ON_BN_CLICKED(IDC_BUTTON_ST2_SAVE, OnButtonSt2Save)
	ON_BN_CLICKED(IDC_CHECK1, OnCheck1)
	ON_BN_CLICKED(IDC_CHECK2, OnCheck2)
	ON_BN_CLICKED(IDC_CHECK3, OnCheck3)
	ON_BN_CLICKED(IDC_CHECK4, OnCheck4)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStatisticPage2Dlg message handlers

void CStatisticPage2Dlg::OnButtonSt2Select() 
{
	// TODO: Add your control notification handler code here
	
	UpdateData(TRUE);

	this->ResetGridStaAccumAccount();

	if(m_valueCheckTotalAccumAccount == TRUE)
	{
		ViewTotalAccumAccount();
	}
	if(m_valueCheckCurrentAccount == TRUE)
	{
		ViewCurrentAccount();
	}
	if(m_valueCheckUnregAccount == TRUE)
	{
		ViewUnregAccount();
	}
	if(m_valueCheckExpectAccount == TRUE)
	{
		ViewExpectAccount();
	}
	
}

void CStatisticPage2Dlg::ViewTotalAccumAccount()
{
	//데이터 insert
	//DB 접속
// 2006-01-23 by cmkwon
//	if (!m_pODBCStmt->Init(m_pMainDlg->m_pServerInfo4Admin->DBIP, m_pMainDlg->m_pServerInfo4Admin->DBPort, m_pMainDlg->m_pServerInfo4Admin->DBName,
//							m_pMainDlg->m_pServerInfo4Admin->DBUID, m_pMainDlg->m_pServerInfo4Admin->DBPWD, GetSafeHwnd())
//	)
//	{
//		return;
//	}

	///////////////////////////////////////////////////////////////////////////////
	// 2005-11-28 by cmkwon, 마지막 AccountUniqueNumber
	CString szSQLQuery;
	// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
	//szSQLQuery.Format("SELECT TOP 1 (AccountUniqueNumber) FROM td_Account WITH (NOLOCK) ORDER BY AccountUniqueNumber DESC");

// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
//	szSQLQuery.Format(QUERY_080702_0142);
//	BOOL bRet = m_pODBCStmt->ExecuteQuery((LPCSTR)szSQLQuery);
	BOOL bRet = m_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_080827_0142));
	if (!bRet)
	{
		// clean up
		m_pODBCStmt->FreeStatement();
		MessageBox("Error");
		return;
	}

	SQLINTEGER cb1;

	int tempSelectData;
	SQLBindCol(m_pODBCStmt->m_hstmt, 1, SQL_C_ULONG, &tempSelectData, 0, &cb1);

	bRet = SQLFetch(m_pODBCStmt->m_hstmt);
	if (bRet != SQL_NO_DATA)
	{
		this->InsertGridData("Total cumulative member", tempSelectData);
	}

	// clean up
	m_pODBCStmt->FreeStatement();

	m_GridStaAccumAccount.AutoSize();
}

void CStatisticPage2Dlg::ViewCurrentAccount()
{
	//데이터 insert
	//DB 접속
// 2006-01-23 by cmkwon
//	if (!m_pODBCStmt->Init(m_pMainDlg->m_pServerInfo4Admin->DBIP, m_pMainDlg->m_pServerInfo4Admin->DBPort, m_pMainDlg->m_pServerInfo4Admin->DBName,
//							m_pMainDlg->m_pServerInfo4Admin->DBUID, m_pMainDlg->m_pServerInfo4Admin->DBPWD, GetSafeHwnd())
//	)
//	{
//		return;
//	}

	///////////////////////////////////////////////////////////////////////////////
	// 2005-11-28 by cmkwon, 현재 총 회원수
	CString szSQLQuery;
	// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
	//szSQLQuery.Format("SELECT COUNT(*) FROM td_Account WITH (NOLOCK)");

// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
//	szSQLQuery.Format(QUERY_080702_0143);
//	BOOL bRet = m_pODBCStmt->ExecuteQuery((LPCSTR)szSQLQuery);
	BOOL bRet = m_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_080827_0143));
	if (!bRet)
	{
		// clean up
		m_pODBCStmt->FreeStatement();
		MessageBox("Error");
		return;
	}

	SQLINTEGER cb1;

	int tempSelectData;
	SQLBindCol(m_pODBCStmt->m_hstmt, 1, SQL_C_ULONG, &tempSelectData, 0, &cb1);


	bRet = SQLFetch(m_pODBCStmt->m_hstmt);
	if (bRet != SQL_NO_DATA)
	{
		this->InsertGridData("Current Total Member", tempSelectData);
	}

	// clean up
	m_pODBCStmt->FreeStatement();

	m_GridStaAccumAccount.AutoSize();
}

void CStatisticPage2Dlg::ViewUnregAccount()
{
	//데이터 insert
	//DB 접속
// 2006-01-23 by cmkwon
//	if (!m_pODBCStmt->Init(m_pMainDlg->m_pServerInfo4Admin->DBIP, m_pMainDlg->m_pServerInfo4Admin->DBPort, m_pMainDlg->m_pServerInfo4Admin->DBName,
//							m_pMainDlg->m_pServerInfo4Admin->DBUID, m_pMainDlg->m_pServerInfo4Admin->DBPWD, GetSafeHwnd())
//	)
//	{
//		return;
//	}

	///////////////////////////////////////////////////////////////////////////////
	// 2005-11-28 by cmkwon, 총탈퇴회원수 = 마지막 AccountUniqueNumber - 현재 총 회원수
	CString szSQLQuery;
	// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
	//szSQLQuery.Format("SELECT ((SELECT TOP 1 (AccountUniqueNumber) FROM td_Account WITH (NOLOCK) ORDER BY AccountUniqueNumber DESC) - COUNT(*)) FROM td_Account");

// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
//	szSQLQuery.Format(QUERY_080702_0144);
//	BOOL bRet = m_pODBCStmt->ExecuteQuery((LPCSTR)szSQLQuery);
	BOOL bRet = m_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_080827_0144));
	if (!bRet)
	{
		// clean up
		m_pODBCStmt->FreeStatement();
		MessageBox("Error");
		return;
	}

	SQLINTEGER cb1;

	int tempSelectData;
	SQLBindCol(m_pODBCStmt->m_hstmt, 1, SQL_C_ULONG, &tempSelectData, 0, &cb1);


	bRet = SQLFetch(m_pODBCStmt->m_hstmt);
	if (bRet != SQL_NO_DATA)
	{
		this->InsertGridData("withdrawal member", tempSelectData);
	}
	// clean up
	m_pODBCStmt->FreeStatement();

	m_GridStaAccumAccount.AutoSize();
}

void CStatisticPage2Dlg::ViewExpectAccount()
{
	//데이터 insert
	//DB 접속
// 2006-01-23 by cmkwon
//	if (!m_pODBCStmt->Init(m_pMainDlg->m_pServerInfo4Admin->DBIP, m_pMainDlg->m_pServerInfo4Admin->DBPort, m_pMainDlg->m_pServerInfo4Admin->DBName,
//							m_pMainDlg->m_pServerInfo4Admin->DBUID, m_pMainDlg->m_pServerInfo4Admin->DBPWD, GetSafeHwnd())
//	)
//	{
//		return;
//	}

	CString szSQLQuery;
	// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
	//szSQLQuery.Format("SELECT (SELECT TOP 1 (AccountUniqueNumber) FROM td_Account WITH (NOLOCK) ORDER BY AccountUniqueNumber DESC) - ((SELECT TOP 1 (AccountUniqueNumber) FROM td_Account ORDER BY AccountUniqueNumber DESC) - COUNT(*))/datediff(dd, min(registereddate), getdate()) AS '현 잠정 회원수' FROM td_Account");

// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
//	szSQLQuery.Format(QUERY_080702_0145);
//	BOOL bRet = m_pODBCStmt->ExecuteQuery((LPCSTR)szSQLQuery);
	BOOL bRet = m_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_080827_0145));
	if (!bRet)
	{
		// clean up
		m_pODBCStmt->FreeStatement();
		MessageBox("Error");
		return;
	}

	SQLINTEGER cb1;

	int tempSelectData;
	SQLBindCol(m_pODBCStmt->m_hstmt, 1, SQL_C_ULONG, &tempSelectData, 0, &cb1);


	bRet = SQLFetch(m_pODBCStmt->m_hstmt);
	if (bRet != SQL_NO_DATA)
	{
		this->InsertGridData("Current temporary member", tempSelectData);
	}

	// clean up
	m_pODBCStmt->FreeStatement();

	m_GridStaAccumAccount.AutoSize();
}

void CStatisticPage2Dlg::OnButtonSt2Save() 
{
	// TODO: Add your control notification handler code here
	CString tempStr;
	for(int i = 0; i<m_GridStaAccumAccount.GetRowCount();i++)
	{
		tempStr = "";
		for(int j = 0; j< m_GridStaAccumAccount.GetColumnCount();j++)
		{
			tempStr += m_GridStaAccumAccount.GetItemText( i, j);
			tempStr += "|\t";
		}
		m_pMainDlg->m_pWriteLog->WriteLogFile((char*)(LPCTSTR)tempStr, tempStr.GetLength(), m_pMainDlg, 1);
	}
}

void CStatisticPage2Dlg::OnCheck1() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
// 2006-01-18 by cmkwon
//	if(m_valueCheckTotalAccumAccount  == TRUE)
//	{
//		m_valueCheckCurrentAccount = FALSE;
//		m_valueCheckUnregAccount = FALSE;
//		m_valueCheckExpectAccount = FALSE;
//	}
	UpdateData(FALSE);
}

void CStatisticPage2Dlg::OnCheck2() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
// 2006-01-18 by cmkwon
//	if(m_valueCheckCurrentAccount == TRUE)
//	{
//		m_valueCheckTotalAccumAccount = FALSE;
//		m_valueCheckUnregAccount = FALSE;
//		m_valueCheckExpectAccount = FALSE;
//	}
	UpdateData(FALSE);
}

void CStatisticPage2Dlg::OnCheck3() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
// 2006-01-18 by cmkwon
//	if(m_valueCheckUnregAccount == TRUE)
//	{
//		m_valueCheckCurrentAccount = FALSE;
//		m_valueCheckTotalAccumAccount = FALSE;
//		m_valueCheckExpectAccount = FALSE;
//	}
	UpdateData(FALSE);
}

void CStatisticPage2Dlg::OnCheck4() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
// 2006-01-18 by cmkwon
//	if(m_valueCheckExpectAccount == TRUE)
//	{
//		m_valueCheckCurrentAccount = FALSE;
//		m_valueCheckUnregAccount = FALSE;
//		m_valueCheckTotalAccumAccount = FALSE;
//	}
	UpdateData(FALSE);
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			void CStatisticPage2Dlg::ResetGridStaAccumAccount(void)
/// \brief		
/// \author		cmkwon
/// \date		2006-01-18 ~ 2006-01-18
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CStatisticPage2Dlg::ResetGridStaAccumAccount(void)
{
	m_GridStaAccumAccount.SetBkColor(0xFFFFFF);
	m_GridStaAccumAccount.SetHeaderSort();

	m_GridStaAccumAccount.SetEditable(FALSE);
	m_GridStaAccumAccount.SetListMode(TRUE);
	m_GridStaAccumAccount.SetSingleRowSelection(TRUE);
	m_GridStaAccumAccount.EnableSelection(TRUE);
	m_GridStaAccumAccount.SetFrameFocusCell(FALSE);
	m_GridStaAccumAccount.SetTrackFocusCell(FALSE);

	m_GridStaAccumAccount.SetRowCount(1);
	m_GridStaAccumAccount.SetColumnCount(2);
	m_GridStaAccumAccount.SetFixedRowCount(1);

	// 칼럼 만들기
	int nCols = 0;
	
	GV_ITEM Item;
	Item.mask		= GVIF_TEXT|GVIF_FORMAT;
	Item.row		= 0;
	Item.nFormat	= DT_LEFT|DT_VCENTER|DT_SINGLELINE;

	Item.col		= nCols++;
	Item.strText.Format("Type");
	m_GridStaAccumAccount.SetItem(&Item);

	Item.col		= nCols++;
	Item.strText.Format("Number of people");
	m_GridStaAccumAccount.SetItem(&Item);
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			void CStatisticPage2Dlg::InsertGridData(char * i_szRowName, int i_nTotalCount, int i_nNormalCount/*=0*/, int i_nSpecialCount/*=0*/)
/// \brief		
/// \author		cmkwon
/// \date		2006-01-18 ~ 2006-01-18
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CStatisticPage2Dlg::InsertGridData(char * i_szRowName, int i_nTotalCount, int i_nNormalCount/*=0*/, int i_nSpecialCount/*=0*/)
{
	int nNewRowIdx = m_GridStaAccumAccount.GetRowCount();
	m_GridStaAccumAccount.SetRowCount(nNewRowIdx+1);

	//select된 값을 GridDetail GridCtrl에 넣어준다.
	GV_ITEM Item;
	Item.mask		= GVIF_TEXT|GVIF_FORMAT;
	Item.nFormat	= DT_LEFT|DT_VCENTER|DT_SINGLELINE;

	Item.row		= nNewRowIdx;
	Item.col		= 0;
	Item.strText	= i_szRowName;
	m_GridStaAccumAccount.SetItem(&Item);

	Item.row		= nNewRowIdx;
	Item.col		= 1;
	Item.strText.Format("%d", i_nTotalCount);
	m_GridStaAccumAccount.SetItem(&Item);
	m_GridStaAccumAccount.UpdateData();
}