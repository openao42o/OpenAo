// MonsterDropDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SpaceCowboyDBEditorTool.h"
#include "MonsterDropDlg.h"
#include "SpaceCowboyDBEditorToolDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMonsterDropDlg dialog


CMonsterDropDlg::CMonsterDropDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMonsterDropDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMonsterDropDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pMainDlg = (CSpaceCowboyDBEditorToolDlg*)AfxGetMainWnd();
	m_pODBCStmt				= m_pMainDlg->m_pODBCStmt;
	m_pMonsterDropDetailDlg = NULL;
}


CMonsterDropDlg::~CMonsterDropDlg()
{
// 2006-01-23 by cmkwon
//	util::del(m_pODBCStmt);
	util::del(m_pMonsterDropDetailDlg);
}

void CMonsterDropDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMonsterDropDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
		DDX_Control(pDX, IDC_CUSTOM_CURRENT, m_GridCurrent);
		DDX_Control(pDX, IDC_CUSTOM_UPDATE, m_GridUpdate);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMonsterDropDlg, CDialog)
	//{{AFX_MSG_MAP(CMonsterDropDlg)
	ON_BN_CLICKED(IDC_BUTTON_END, OnButtonEnd)
	ON_BN_CLICKED(IDC_BUTTON_VIEW, OnButtonView)
	ON_BN_CLICKED(IDC_BUTTON_INSERT, OnButtonInsert)
	ON_BN_CLICKED(IDC_BUTTON_UPDATE, OnButtonUpdate)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, OnButtonDelete)
	ON_BN_CLICKED(IDC_BUTTON_REFRESH, OnButtonRefresh)
	ON_BN_CLICKED(IDC_BUTTON_CANCEL, OnButtonCancel)
	ON_BN_CLICKED(IDC_BUTTON_SERVERUPDATE, OnButtonServerupdate)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMonsterDropDlg message handlers

BOOL CMonsterDropDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	///////////////////////////////////////////////////////////////////////////////
	// 2006-05-08 by cmkwon
	GetDlgItem(IDC_BUTTON_INSERT)->EnableWindow(m_pMainDlg->GetEnbaleEdit());
	GetDlgItem(IDC_BUTTON_UPDATE)->EnableWindow(m_pMainDlg->GetEnbaleEdit());
	GetDlgItem(IDC_BUTTON_DELETE)->EnableWindow(m_pMainDlg->GetEnbaleEdit());
	
	// TODO: Add extra initialization here
// 2006-01-23 by cmkwon
//	if (!m_pODBCStmt->Init(m_pMainDlg->m_pServerInfo4Admin->DBIP, m_pMainDlg->m_pServerInfo4Admin->DBPort, m_pMainDlg->m_pServerInfo4Admin->DBName,
//							m_pMainDlg->m_pServerInfo4Admin->DBUID, m_pMainDlg->m_pServerInfo4Admin->DBPWD, GetSafeHwnd())
//	)
//	{
////		MessageBox(STRERR_S_SCADMINTOOL_0013);
//		EndDialog(-1);
//		return FALSE;
//	}

	InitGridCurrent();
	InitGridUpdate();

	//현재 DB에 있는 값 불러온다.
//	GetCurrentList();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CMonsterDropDlg::OnButtonEnd() 
{
	// TODO: Add your control notification handler code here
	EndDialog(0);
}

void CMonsterDropDlg::OnButtonView() 
{
	// TODO: Add your control notification handler code here
	//현재 선택된 row를 알아온다.
	CCellID tmpCellID = m_GridCurrent.GetFocusCell();

	if(tmpCellID.col == -1 || tmpCellID.row == -1)
	{
		// GridCtrl에서 row가 선택되지 않았다.
		AfxMessageBox("Please Select 1 Row");
		return;
	}

	if (m_pMonsterDropDetailDlg != NULL)
	{
		util::del(m_pMonsterDropDetailDlg);
	}

	if (m_pMonsterDropDetailDlg == NULL)
	{
		m_pMonsterDropDetailDlg = new CMonsterDropDetailDlg(this);
		m_pMonsterDropDetailDlg->Create(IDD_DIALOG_MONSTERDROPDETAIL, this);
		m_pMonsterDropDetailDlg->m_ctrlButtonSave.EnableWindow(FALSE);
		m_pMonsterDropDetailDlg->m_ctrlButtonMake.EnableWindow(FALSE);
	}
	SetDetailDialog(tmpCellID.row);
	m_pMonsterDropDetailDlg->ShowWindow(SW_SHOW);
}

void CMonsterDropDlg::InitGridCurrent()
{
	m_GridCurrent.SetBkColor(0xFFFFFF);

	int m_nRows = 1;
	int m_nCols = 7;

	m_GridCurrent.SetEditable(FALSE);
	m_GridCurrent.SetListMode(TRUE);
	m_GridCurrent.SetSingleRowSelection(TRUE);
	m_GridCurrent.EnableSelection(TRUE);
	m_GridCurrent.SetFrameFocusCell(FALSE);
	m_GridCurrent.SetTrackFocusCell(FALSE);
	m_GridCurrent.SetHeaderSort();

	m_GridCurrent.SetRowCount(m_nRows);
	m_GridCurrent.SetColumnCount(m_nCols);
	m_GridCurrent.SetFixedRowCount(1);

	// 칼럼 만들기
	m_nCols = 0;
	GV_ITEM Item;
	Item.mask = GVIF_TEXT|GVIF_FORMAT;
	Item.row = 0;
	Item.nFormat = DT_LEFT|DT_VCENTER|DT_SINGLELINE;

	Item.col = m_nCols++;
	Item.strText.Format("MonsterUniqueNumber");
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("ItemNum");
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("MinCount");
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("MaxCount");
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("Probability");
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("PrefixProbability");
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("SuffixProbability");
	m_GridCurrent.SetItem(&Item);

	// arrange grid
	m_GridCurrent.AutoSize();
//	m_GridCurrent.ExpandColumnsToFit();

	// clean all cells
	CCellRange tmpCellRange(1, 0, m_GridCurrent.GetRowCount()-1, m_GridCurrent.GetColumnCount()-1);
	m_GridCurrent.ClearCells(tmpCellRange);
}

void CMonsterDropDlg::InitGridUpdate()
{
	m_GridUpdate.SetBkColor(0xFFFFFF);

	int m_nRows = 1;
	int m_nCols = 8;

	m_GridUpdate.SetEditable(FALSE);
	m_GridUpdate.SetListMode(TRUE);
	m_GridUpdate.SetSingleRowSelection(TRUE);
	m_GridUpdate.EnableSelection(TRUE);
	m_GridUpdate.SetFrameFocusCell(FALSE);
	m_GridUpdate.SetTrackFocusCell(FALSE);
	m_GridUpdate.SetHeaderSort();

	m_GridUpdate.SetRowCount(m_nRows);
	m_GridUpdate.SetColumnCount(m_nCols);
	m_GridUpdate.SetFixedRowCount(1);

	// 칼럼 만들기
	m_nCols = 0;
	GV_ITEM Item;
	Item.mask = GVIF_TEXT|GVIF_FORMAT;
	Item.row = 0;
	Item.nFormat = DT_LEFT|DT_VCENTER|DT_SINGLELINE;

	Item.col = m_nCols++;
	Item.strText.Format("STATUS");
	m_GridUpdate.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("MonsterUniqueNumber");
	m_GridUpdate.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("ItemNum");
	m_GridUpdate.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("MinCount");
	m_GridUpdate.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("MaxCount");
	m_GridUpdate.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("Probability");
	m_GridUpdate.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("PrefixProbability");
	m_GridUpdate.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("SuffixProbability");
	m_GridUpdate.SetItem(&Item);

	// arrange grid
	m_GridUpdate.AutoSize();
//	m_GridUpdate.ExpandColumnsToFit();

	// clean all cells
	CCellRange tmpCellRange(1, 0, m_GridUpdate.GetRowCount()-1, m_GridUpdate.GetColumnCount()-1);
	m_GridUpdate.ClearCells(tmpCellRange);
}

void CMonsterDropDlg::GetCurrentList(int type, int uniquenumber)
{
	GV_ITEM Item;
	Item.mask = GVIF_TEXT|GVIF_FORMAT;
	Item.row = 1;
	Item.nFormat = ((DWORD)DT_LEFT|DT_VCENTER|DT_SINGLELINE);
	
	CString szSQLQuery;
	
// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
// 	if(type == 0)
// 		szSQLQuery.Format("select MonsterUniqueNumber ,ItemNum ,MinCount ,MaxCount ,Probability ,PrefixProbability ,SuffixProbability  from ti_monsteritem");
// 	else
// 		szSQLQuery.Format("select MonsterUniqueNumber ,ItemNum ,MinCount ,MaxCount ,Probability ,PrefixProbability ,SuffixProbability  from ti_monsteritem where monsteruniquenumber = %d",
// 		uniquenumber);
	if(type == 0)
	{
		szSQLQuery.Format(QUERY_080702_0115);
	}
	else
	{
		szSQLQuery.Format(QUERY_080702_0116, uniquenumber);
	}

	SQLINTEGER cb1, cb2, cb3, cb4, cb5, cb6, cb7;

	_TI_MONSTERITEM		st_MonsterItem;
	
	memset(&st_MonsterItem, 0, sizeof(st_MonsterItem));

	SQLBindCol(m_pODBCStmt->m_hstmt, 1, SQL_C_ULONG, &st_MonsterItem.MonsterUniqueNumber, 40, &cb1);
	SQLBindCol(m_pODBCStmt->m_hstmt, 2, SQL_C_ULONG, &st_MonsterItem.ItemNum, 0, &cb2);
	SQLBindCol(m_pODBCStmt->m_hstmt, 3, SQL_C_ULONG, &st_MonsterItem.MinCount, 0, &cb3);
	SQLBindCol(m_pODBCStmt->m_hstmt, 4, SQL_C_ULONG, &st_MonsterItem.MaxCount, 0, &cb4);
	SQLBindCol(m_pODBCStmt->m_hstmt, 5, SQL_C_ULONG, &st_MonsterItem.Probability, 0, &cb5);
	SQLBindCol(m_pODBCStmt->m_hstmt, 6, SQL_C_UTINYINT, &st_MonsterItem.PrefixProbability, 0, &cb6);
	SQLBindCol(m_pODBCStmt->m_hstmt, 7, SQL_C_UTINYINT, &st_MonsterItem.SuffixProbability, 0, &cb7);
	
	BOOL bRet = m_pODBCStmt->ExecuteQuery((LPCSTR)szSQLQuery);
	if (!bRet)
	{
		// clean up
		m_pODBCStmt->FreeStatement();
		MessageBox("Error");
		return;
	}

	int nCount = 0;
	m_GridCurrent.SetRowCount(nCount+1);
	// DB에 값이 없을때까지 loop를 돈다
	while ( (bRet = SQLFetch(m_pODBCStmt->m_hstmt)) != SQL_NO_DATA)
	{
		m_GridCurrent.SetRowCount(nCount+2);

		//select된 값을 GridDetail GridCtrl에 넣어준다.
		InsertItemToGridCurrent(&st_MonsterItem, nCount+1);

		nCount++;
		memset(&st_MonsterItem, 0, sizeof(st_MonsterItem));
	}

	// clean up
	m_pODBCStmt->FreeStatement();

	m_GridCurrent.AutoSize();
}

void CMonsterDropDlg::InsertItemToGridCurrent(_TI_MONSTERITEM *i_pTi_MonsterItem, int i_nRow)
{
	// 칼럼 만들기
	int m_nCols = 0;

	// 컬럼에 값 넣기
	GV_ITEM Item;
	Item.mask = GVIF_TEXT|GVIF_FORMAT;
	Item.row = i_nRow;
	Item.nFormat = ((DWORD)DT_LEFT|DT_VCENTER|DT_SINGLELINE);
	
	Item.col = m_nCols++;
	Item.strText.Format("%d", i_pTi_MonsterItem->MonsterUniqueNumber);
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("%d", i_pTi_MonsterItem->ItemNum);
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("%d", i_pTi_MonsterItem->MinCount);
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("%d", i_pTi_MonsterItem->MaxCount);
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("%d", i_pTi_MonsterItem->Probability);
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("%d", i_pTi_MonsterItem->PrefixProbability);
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("%d", i_pTi_MonsterItem->SuffixProbability);
	m_GridCurrent.SetItem(&Item);
}

void CMonsterDropDlg::SetDetailDialog(int row)
{
	//선택된 셀을 바탕으로 값을 설정한다.
	m_pMonsterDropDetailDlg->m_valueEditMonsterUniqueNumber = m_GridCurrent.GetItemText(row,0);
	m_pMonsterDropDetailDlg->m_valueEditItemNumber = m_GridCurrent.GetItemText(row,1);
	m_pMonsterDropDetailDlg->m_valueEditMinCount = m_GridCurrent.GetItemText(row,2);
	m_pMonsterDropDetailDlg->m_valueEditMaxCount = m_GridCurrent.GetItemText(row,3);
	m_pMonsterDropDetailDlg->m_valueEditProbability = m_GridCurrent.GetItemText(row,4);
	m_pMonsterDropDetailDlg->m_valueEditPrefixProbability = m_GridCurrent.GetItemText(row,5);
	m_pMonsterDropDetailDlg->m_valueEditSuffixProbability = m_GridCurrent.GetItemText(row,6);	

	m_pMonsterDropDetailDlg->UpdateData(FALSE);
}

void CMonsterDropDlg::OnButtonInsert() 
{
	// TODO: Add your control notification handler code here
	if (m_pMonsterDropDetailDlg != NULL)
	{
		util::del(m_pMonsterDropDetailDlg);
	}

	if (m_pMonsterDropDetailDlg == NULL)
	{
		m_pMonsterDropDetailDlg = new CMonsterDropDetailDlg(this);
		m_pMonsterDropDetailDlg->Create(IDD_DIALOG_MONSTERDROPDETAIL, this);
		m_pMonsterDropDetailDlg->m_ctrlButtonSave.EnableWindow(FALSE);
	}

	m_pMonsterDropDetailDlg->ShowWindow(SW_SHOW);
}

void CMonsterDropDlg::OnButtonUpdate() 
{
	// TODO: Add your control notification handler code here
	//현재 선택된 row를 알아온다.
	CCellID tmpCellID = m_GridCurrent.GetFocusCell();

	if(tmpCellID.col == -1 || tmpCellID.row == -1)
	{
		// GridCtrl에서 row가 선택되지 않았다.
		AfxMessageBox("Please Select 1 Row");
		return;
	}

	if (m_pMonsterDropDetailDlg != NULL)
	{
		util::del(m_pMonsterDropDetailDlg);
	}

	if (m_pMonsterDropDetailDlg == NULL)
	{
		m_pMonsterDropDetailDlg = new CMonsterDropDetailDlg(this);
		m_pMonsterDropDetailDlg->Create(IDD_DIALOG_MONSTERDROPDETAIL, this);
		m_pMonsterDropDetailDlg->m_ctrlButtonMake.EnableWindow(FALSE);
	}
	SetDetailDialog(tmpCellID.row);
	m_pMonsterDropDetailDlg->ShowWindow(SW_SHOW);

	//m_GridCurrent.DeleteRow(tmpCellID.row);
	m_pMonsterDropDetailDlg->m_nSelectRow = tmpCellID.row;
	m_GridCurrent.Refresh();
}

void CMonsterDropDlg::OnButtonDelete() 
{
	// TODO: Add your control notification handler code here
	//현재 선택된 row를 알아온다.
	CCellID tmpCellID = m_GridCurrent.GetFocusCell();

	if(tmpCellID.col == -1 || tmpCellID.row == -1)
	{
		// GridCtrl에서 row가 선택되지 않았다.
		AfxMessageBox("Please Select 1 Row");
		return;
	}

	ChangeGridDate(&m_GridCurrent, &m_GridUpdate, 1, tmpCellID.row);

	m_GridCurrent.DeleteRow(tmpCellID.row);
	m_GridCurrent.Refresh();
}

void CMonsterDropDlg::OnButtonRefresh() 
{
	// TODO: Add your control notification handler code here
	m_GridCurrent.UpdateWindow();
}

void CMonsterDropDlg::OnButtonCancel() 
{
	// TODO: Add your control notification handler code here
	CCellID tmpCellID = m_GridUpdate.GetFocusCell();

	if(tmpCellID.col == -1 || tmpCellID.row == -1)
	{
		// GridCtrl에서 row가 선택되지 않았다.
		AfxMessageBox("Please Select 1 Row");
		return;
	}

	ChangeGridDate(&m_GridUpdate, &m_GridCurrent, 2, tmpCellID.row);

	m_GridUpdate.DeleteRow(tmpCellID.row);
	m_GridUpdate.Refresh();
}

void CMonsterDropDlg::ChangeGridDate(CGridCtrl *oriGrid, CGridCtrl *newGrid, int type, int row)
{
	// 칼럼 만들기
	newGrid->SetRowCount(newGrid->GetRowCount()+1);

	if(type == 1) //Delete 버튼을 눌렀을때
	{
		newGrid->SetItemText(newGrid->GetRowCount()-1, 0, "DELETE");
		for(int m_nCols = 0; m_nCols<41;m_nCols++)
			newGrid->SetItemText(newGrid->GetRowCount()-1, m_nCols+1, oriGrid->GetItemText(row,m_nCols));
	}
	else if(type == 2) //Delete 버튼을 눌렀을때
	{
		for(int m_nCols = 0; m_nCols<41;m_nCols++)
			newGrid->SetItemText(newGrid->GetRowCount()-1, m_nCols, oriGrid->GetItemText(row,m_nCols+1));
	}

	newGrid->Refresh();
}

void CMonsterDropDlg::OnButtonServerupdate() 
{
	// TODO: Add your control notification handler code here
	//DB에 값을 집어 넣는다.
	CString szQuery;
	BOOL bRet= 0;
	int row = m_GridUpdate.GetRowCount();
	for(int i = 0; i< row; i++)
	{
		if(m_GridUpdate.GetItemText(1, 0) == "DELETE")
		{
			//DELETE ROW
			// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
			//szQuery.Format("delete ti_monsteritem where MonsterUniqueNumber = %s", m_GridUpdate.GetItemText(1, 1));
			szQuery.Format(QUERY_080702_0117, m_GridUpdate.GetItemText(1, 1));

			TRACE(szQuery.Left(500));
			bRet = m_pODBCStmt->ExecuteQuery((LPCSTR)szQuery);
			if (!bRet)
			{
				// cleanup
				AfxMessageBox("Fail During Delete 1 Row Data");
				m_pODBCStmt->FreeStatement();
				//	szQuery = "[" + m_pMainDlg->m_valueEditID + "] " + "[" + m_pMainDlg->m_pServerInfo4Admin->DBUID + "] " + "[]+szQuery;
				m_pMainDlg->m_pWriteLog->WriteLogFile((char*)(LPCTSTR)szQuery, szQuery.GetLength(), m_pMainDlg);
				return;
			}	
			//	szQuery = "[" + m_pMainDlg->m_valueEditID + "] " + "[" + m_pMainDlg->m_pServerInfo4Admin->DBUID + "] " + "[]+szQuery;
			m_pMainDlg->m_pWriteLog->WriteLogFile((char*)(LPCTSTR)szQuery, szQuery.GetLength(), m_pMainDlg);

			m_GridUpdate.DeleteRow(1);
			m_GridUpdate.Refresh();
		}
		else if(m_GridUpdate.GetItemText(1, 0) == "UPDATE")
		{
			//Update ROW
			// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
			//szQuery.Format("Update ti_MonsterItem set ItemNum =%s,MinCount=%s ,MaxCount=%s ,Probability=%s ,PrefixProbability=%s ,SuffixProbability=%s where monsteruniquenumber = %s"
			szQuery.Format(QUERY_080702_0118
				, m_GridUpdate.GetItemText(1, 2), m_GridUpdate.GetItemText(1, 3)
				, m_GridUpdate.GetItemText(1, 4), m_GridUpdate.GetItemText(1, 5)
				, m_GridUpdate.GetItemText(1, 6), m_GridUpdate.GetItemText(1, 7)
				, m_GridUpdate.GetItemText(1, 1)
				);
			TRACE(szQuery.Left(500));
			bRet = m_pODBCStmt->ExecuteQuery((LPCSTR)szQuery);

			if (!bRet)
			{
				// cleanup
				AfxMessageBox("Fail During Update 1 Row Data");
				m_pODBCStmt->FreeStatement();
				//	szQuery = "[" + m_pMainDlg->m_valueEditID + "] " + "[" + m_pMainDlg->m_pServerInfo4Admin->DBUID + "] " + "[]+szQuery;
				m_pMainDlg->m_pWriteLog->WriteLogFile((char*)(LPCTSTR)szQuery, szQuery.GetLength(), m_pMainDlg);
				return;
			}	
			//	szQuery = "[" + m_pMainDlg->m_valueEditID + "] " + "[" + m_pMainDlg->m_pServerInfo4Admin->DBUID + "] " + "[]+szQuery;
			m_pMainDlg->m_pWriteLog->WriteLogFile((char*)(LPCTSTR)szQuery, szQuery.GetLength(), m_pMainDlg);

			m_GridUpdate.DeleteRow(1);
			m_GridUpdate.Refresh();
		}
		else if(m_GridUpdate.GetItemText(1, 0) == "INSERT")
		{
			//Insert ROW
			// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
			//szQuery.Format("Insert into ti_monsteritem(MonsterUniqueNumber ,ItemNum ,MinCount ,MaxCount ,Probability ,PrefixProbability ,SuffixProbability) values ( %s, %s, %s, %s, %s, %s, %s)"
			szQuery.Format(QUERY_080702_0119
				, m_GridUpdate.GetItemText(1, 1)
				, m_GridUpdate.GetItemText(1, 2), m_GridUpdate.GetItemText(1, 3)
				, m_GridUpdate.GetItemText(1, 4), m_GridUpdate.GetItemText(1, 5)
				, m_GridUpdate.GetItemText(1, 6), m_GridUpdate.GetItemText(1, 7)
				);
			TRACE(szQuery.Left(500));

			bRet = m_pODBCStmt->ExecuteQuery((LPCSTR)szQuery);
			

			if (!bRet)
			{
				// cleanup
				AfxMessageBox("Fail During Update 1 Row Data");
				m_pODBCStmt->FreeStatement();
				//	szQuery = "[" + m_pMainDlg->m_valueEditID + "] " + "[" + m_pMainDlg->m_pServerInfo4Admin->DBUID + "] " + "[]+szQuery;
				m_pMainDlg->m_pWriteLog->WriteLogFile((char*)(LPCTSTR)szQuery, szQuery.GetLength(), m_pMainDlg);
				return;
			}	
			//	szQuery = "[" + m_pMainDlg->m_valueEditID + "] " + "[" + m_pMainDlg->m_pServerInfo4Admin->DBUID + "] " + "[]+szQuery;
			m_pMainDlg->m_pWriteLog->WriteLogFile((char*)(LPCTSTR)szQuery, szQuery.GetLength(), m_pMainDlg);

			m_GridUpdate.DeleteRow(1);
			m_GridUpdate.Refresh();
		}
		
	}
	// cleanup
	m_pODBCStmt->FreeStatement();
	GetCurrentList(0);
}
