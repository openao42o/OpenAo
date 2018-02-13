// EngineEditorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SpaceCowboyDBEditorTool.h"
#include "EngineItemDlg.h"
#include "SpaceCowboyDBEditorToolDlg.h"
#include "AtumProtocol.h"
#include "AtumError.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEngineItemDlg dialog


CEngineItemDlg::CEngineItemDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEngineItemDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEngineItemDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pMainDlg = (CSpaceCowboyDBEditorToolDlg*)AfxGetMainWnd();
	m_pODBCStmt				= m_pMainDlg->m_pODBCStmt;
	m_pEngineItemDetailDlg = NULL;
}

CEngineItemDlg::~CEngineItemDlg()
{
	util::del(m_pEngineItemDetailDlg);
}



void CEngineItemDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEngineItemDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
		DDX_Control(pDX, IDC_IE_CUSTOM_GRID_CURRENT, m_GridCurrent);
		DDX_Control(pDX, IDC_IE_CUSTOM_GRID_UPDATE, m_GridUpdate);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEngineItemDlg, CDialog)
	//{{AFX_MSG_MAP(CEngineItemDlg)
	ON_BN_CLICKED(IDC_IE_BUTTON_VIEW, OnButtonView)
	ON_BN_CLICKED(IDC_IE_BUTTON_INSERT, OnButtonInsert)
	ON_BN_CLICKED(IDC_IE_BUTTON_UPDATE, OnButtonUpdate)
	ON_BN_CLICKED(IDC_IE_BUTTON_DELETE, OnButtonDelete)
	ON_BN_CLICKED(IDC_IE_BUTTON_REFRESH, OnButtonRefresh)
	ON_BN_CLICKED(IDC_IE_BUTTON_CANCEL, OnButtonCancel)
	ON_BN_CLICKED(IDC_IE_BUTTON_REFRESHUPDATE, OnButtonRefreshupdate)
	ON_BN_CLICKED(IDC_IE_BUTTON_SERVERUPDATE, OnButtonServerupdate)
	ON_BN_CLICKED(IDC_IE_BUTTON_END, OnButtonEnd)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEngineItemDlg message handlers

void CEngineItemDlg::OnButtonView() 
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

	if (m_pEngineItemDetailDlg != NULL)
	{
		util::del(m_pEngineItemDetailDlg);
	}

	if (m_pEngineItemDetailDlg == NULL)
	{
		m_pEngineItemDetailDlg = new CEngineItemDetailDlg(this);
		m_pEngineItemDetailDlg->Create(IDD_DIALOG_ENGINE_DETAIL, this);
		m_pEngineItemDetailDlg->m_ctrlButtonSave.EnableWindow(FALSE);
		m_pEngineItemDetailDlg->m_ctrlButtonMake.EnableWindow(FALSE);
	}
	SetDetailDialog(tmpCellID.row);
	m_pEngineItemDetailDlg->ShowWindow(SW_SHOW);
}

void CEngineItemDlg::OnButtonInsert() 
{
	// TODO: Add your control notification handler code here
	if (m_pEngineItemDetailDlg != NULL)
	{
		util::del(m_pEngineItemDetailDlg);
	}

	if (m_pEngineItemDetailDlg == NULL)
	{
		m_pEngineItemDetailDlg = new CEngineItemDetailDlg(this);
		m_pEngineItemDetailDlg->Create(IDD_DIALOG_ENGINE_DETAIL, this);
		m_pEngineItemDetailDlg->m_ctrlButtonSave.EnableWindow(FALSE);
	}

	m_pEngineItemDetailDlg->ShowWindow(SW_SHOW);
}

void CEngineItemDlg::OnButtonUpdate() 
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

	if (m_pEngineItemDetailDlg != NULL)
	{
		util::del(m_pEngineItemDetailDlg);
	}

	if (m_pEngineItemDetailDlg == NULL)
	{
		m_pEngineItemDetailDlg = new CEngineItemDetailDlg(this);
		m_pEngineItemDetailDlg->Create(IDD_DIALOG_ENGINE_DETAIL, this);
		m_pEngineItemDetailDlg->m_ctrlButtonMake.EnableWindow(FALSE);
	}
	SetDetailDialog(tmpCellID.row);
	m_pEngineItemDetailDlg->ShowWindow(SW_SHOW);

	m_pEngineItemDetailDlg->m_nSelectRow = tmpCellID.row;
//	m_GridCurrent.DeleteRow(tmpCellID.row);
	m_GridCurrent.Refresh();
}

void CEngineItemDlg::OnButtonDelete() 
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

void CEngineItemDlg::ChangeGridDate(CGridCtrl *oriGrid, CGridCtrl *newGrid, int type, int row)
{
	// 칼럼 만들기
	newGrid->SetRowCount(newGrid->GetRowCount()+1);

	if(type == 1) //Delete 버튼을 눌렀을때
	{
		newGrid->SetItemText(newGrid->GetRowCount()-1, 0, "DELETE");
		for(int m_nCols = 0; m_nCols<41;m_nCols++)
			newGrid->SetItemText(newGrid->GetRowCount()-1, m_nCols+1, oriGrid->GetItemText(row,m_nCols));
	}
	else
	{
		for(int m_nCols = 0; m_nCols<42;m_nCols++)
			newGrid->SetItemText(newGrid->GetRowCount()-1, m_nCols, oriGrid->GetItemText(row,m_nCols+1));

	}

	newGrid->Refresh();
}

void CEngineItemDlg::OnButtonRefresh() 
{
	// TODO: Add your control notification handler code here
	m_GridCurrent.UpdateWindow();
}

void CEngineItemDlg::OnButtonCancel() 
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

void CEngineItemDlg::OnButtonRefreshupdate() 
{
	// TODO: Add your control notification handler code here
	m_GridUpdate.Refresh();
}

void CEngineItemDlg::OnButtonServerupdate() 
{
	// TODO: Add your control notification handler code here
	//DB에 값을 집어 넣는다.
	CString szQuery;
	BOOL bRet= 0;
	int initRow = m_GridUpdate.GetRowCount();
	for(int i = 0; i< initRow -1; i++)
	{
		if(m_GridUpdate.GetItemText(1, 0) == "DELETE")
		{
			//DELETE ROW
			// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
			//szQuery.Format("delete ti_item where itemnum = %s", m_GridUpdate.GetItemText(1, 1));
			szQuery.Format(QUERY_080702_0088, m_GridUpdate.GetItemText(1, 1));

			bRet = m_pODBCStmt->ExecuteQuery((LPCSTR)szQuery);
			TRACE(szQuery.Left(500));
			TRACE(szQuery.Right(500));
			if (!bRet)
			{
				// cleanup
				AfxMessageBox("Fail During Delete 1 Row Data");
				m_pODBCStmt->FreeStatement();
				return;
			}	
			//	szQuery = "[" + m_pMainDlg->m_valueEditID + "] " + "[" + m_pMainDlg->m_pServerInfo4Admin->DBUID + "] " + "[]+szQuery;
			m_pMainDlg->m_pWriteLog->WriteLogFile((char*)(LPCTSTR)szQuery, szQuery.GetLength(), m_pMainDlg);

			m_GridUpdate.DeleteRow(1);
			m_GridUpdate.Refresh();
		}
		else if(m_GridUpdate.GetItemText(1, 0) == "UPDATE")
		{
			if(m_GridUpdate.GetItemText(1, 16) == "")
				m_GridUpdate.SetItemText(1,16,"NULL");
			if(m_GridUpdate.GetItemText(1, 36) == "")
				m_GridUpdate.SetItemText(1,36,"NULL");
			//Update ROW
			// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
			//szQuery.Format("Update ti_item set kind = %s, itemname = '%s', abilitymin = %s, abilitymax = %s, reqrace = %s, reqattackpart = %s, reqdefensepart = %s, ReqFuelPart = %s, reqsoulpart = %s, reqshieldpart = %s, reqdodgepart = %s, requnitkind = %s, reqminlevel = %s, reqmaxlevel = %s, weight = %s, range = %s, position = %s, scarcity = %s, mintradequantity = %s, price = %s, cashprice = %s, [time] = %s, rangeangle = %s, boosterangle = %s, itemattribute = %s, [description] = '%s', SourceIndex = %s where itemnum = %s "				
			szQuery.Format(QUERY_080702_0089				
				, m_GridUpdate.GetItemText(1, 2), m_GridUpdate.GetItemText(1, 3)
				, m_GridUpdate.GetItemText(1, 4), m_GridUpdate.GetItemText(1, 5)
				, m_GridUpdate.GetItemText(1, 6), m_GridUpdate.GetItemText(1, 7)
				, m_GridUpdate.GetItemText(1, 8), m_GridUpdate.GetItemText(1, 9)
				, m_GridUpdate.GetItemText(1, 10), m_GridUpdate.GetItemText(1, 11)
				, m_GridUpdate.GetItemText(1, 12), m_GridUpdate.GetItemText(1, 13)
				, m_GridUpdate.GetItemText(1, 14), m_GridUpdate.GetItemText(1, 15)
				, m_GridUpdate.GetItemText(1, 16), m_GridUpdate.GetItemText(1, 17)
				, m_GridUpdate.GetItemText(1, 18), m_GridUpdate.GetItemText(1, 19)
				, m_GridUpdate.GetItemText(1, 20), m_GridUpdate.GetItemText(1, 21)
				, m_GridUpdate.GetItemText(1, 22), m_GridUpdate.GetItemText(1, 23)
				, m_GridUpdate.GetItemText(1, 24), m_GridUpdate.GetItemText(1, 25)
				, m_GridUpdate.GetItemText(1, 26), m_GridUpdate.GetItemText(1, 27)
				, m_GridUpdate.GetItemText(1, 28)
				, m_GridUpdate.GetItemText(1, 1)
				);
			TRACE(m_GridUpdate.GetItemText(1, 1));

			bRet = m_pODBCStmt->ExecuteQuery((LPCSTR)szQuery);
			TRACE(szQuery.Left(500));
			TRACE(szQuery.Right(500));

			if (!bRet)
			{
				// cleanup
				AfxMessageBox("Fail During Update 1 Row Data");
				m_pODBCStmt->FreeStatement();
				return;
			}	
			//	szQuery = "[" + m_pMainDlg->m_valueEditID + "] " + "[" + m_pMainDlg->m_pServerInfo4Admin->DBUID + "] " + "[]+szQuery;
			m_pMainDlg->m_pWriteLog->WriteLogFile((char*)(LPCTSTR)szQuery, szQuery.GetLength(), m_pMainDlg);

			m_GridUpdate.DeleteRow(1);
			m_GridUpdate.Refresh();
		}
		else if(m_GridUpdate.GetItemText(1, 0) == "INSERT")
		{
			if(m_GridUpdate.GetItemText(1, 16) == "")
				m_GridUpdate.SetItemText(1,16,"NULL");
			if(m_GridUpdate.GetItemText(1, 36) == "")
				m_GridUpdate.SetItemText(1,36,"NULL");
			//Update ROW
			// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
			//szQuery.Format("Insert into ti_item(itemnum, kind, itemname, abilitymin, abilitymax, reqrace, reqattackpart, reqdefensepart, ReqFuelPart, reqsoulpart, reqshieldpart, reqdodgepart, requnitkind, reqminlevel, reqmaxlevel, weight, range, position, scarcity, mintradequantity, price, cashprice, [time], rangeangle, boosterangle, itemattribute, [description], SourceIndex) values ( %s, %s, '%s', %s, %s,%s, %s,%s, %s,%s, %s,%s, %s,%s, %s,%s, %s,%s, %s,%s,%s, %s,%s,%s,%s, %s, '%s', %s)"				
			szQuery.Format(QUERY_080702_0090				
				, m_GridUpdate.GetItemText(1, 1)
				, m_GridUpdate.GetItemText(1, 2), m_GridUpdate.GetItemText(1, 3)
				, m_GridUpdate.GetItemText(1, 4), m_GridUpdate.GetItemText(1, 5)
				, m_GridUpdate.GetItemText(1, 6), m_GridUpdate.GetItemText(1, 7)
				, m_GridUpdate.GetItemText(1, 8), m_GridUpdate.GetItemText(1, 9)
				, m_GridUpdate.GetItemText(1, 10), m_GridUpdate.GetItemText(1, 11)
				, m_GridUpdate.GetItemText(1, 12), m_GridUpdate.GetItemText(1, 13)
				, m_GridUpdate.GetItemText(1, 14), m_GridUpdate.GetItemText(1, 15)
				, m_GridUpdate.GetItemText(1, 16), m_GridUpdate.GetItemText(1, 17)
				, m_GridUpdate.GetItemText(1, 18), m_GridUpdate.GetItemText(1, 19)
				, m_GridUpdate.GetItemText(1, 20), m_GridUpdate.GetItemText(1, 21)
				, m_GridUpdate.GetItemText(1, 22), m_GridUpdate.GetItemText(1, 23)
				, m_GridUpdate.GetItemText(1, 24), m_GridUpdate.GetItemText(1, 25)
				, m_GridUpdate.GetItemText(1, 26), m_GridUpdate.GetItemText(1, 27)
				, m_GridUpdate.GetItemText(1, 28), m_GridUpdate.GetItemText(1, 29)
				);
			TRACE(m_GridUpdate.GetItemText(1, 1));

			bRet = m_pODBCStmt->ExecuteQuery((LPCSTR)szQuery);
			TRACE(szQuery.Left(500));
			TRACE(szQuery.Right(500));

			if (!bRet)
			{
				// cleanup
				AfxMessageBox("Fail During Update 1 Row Data");
				m_pODBCStmt->FreeStatement();
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
	GetCurrentItemList();
}

void CEngineItemDlg::OnButtonEnd() 
{
	// TODO: Add your control notification handler code here
	EndDialog(0);
}

BOOL CEngineItemDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	///////////////////////////////////////////////////////////////////////////////
	// 2006-05-08 by cmkwon
	GetDlgItem(IDC_IE_BUTTON_INSERT)->EnableWindow(m_pMainDlg->GetEnbaleEdit());
	GetDlgItem(IDC_IE_BUTTON_UPDATE)->EnableWindow(m_pMainDlg->GetEnbaleEdit());
	GetDlgItem(IDC_IE_BUTTON_DELETE)->EnableWindow(m_pMainDlg->GetEnbaleEdit());

	// TODO: Add extra initialization here
// 2006-01-23 by cmkwon
//	if (!m_pODBCStmt->Init(m_pMainDlg->m_pServerInfo4Admin->DBIP, m_pMainDlg->m_pServerInfo4Admin->DBPort, m_pMainDlg->m_pServerInfo4Admin->DBName,
//							m_pMainDlg->m_pServerInfo4Admin->DBUID, m_pMainDlg->m_pServerInfo4Admin->DBPWD, GetSafeHwnd())
//	)
//	{
//		EndDialog(-1);
//		return FALSE;
//	}

	//GridCtrl 초기화
	InitGridCurrent();
	InitGridUpdate();

	//현재 DB에 있는 값 불러온다.
	GetCurrentItemList();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CEngineItemDlg::InitGridCurrent()
{
	m_GridCurrent.SetBkColor(0xFFFFFF);

	int m_nRows = 1;
	int m_nCols = 28;

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
	Item.strText.Format("ItemNum");
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("Kind");
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("ItemName");
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("AbilityMin");
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("AbilityMax");
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("ReqRace");
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("ReqAttackPart");
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("ReqDefensePart");
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("ReqFuelPart");
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("ReqSoulPart");
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("ReqShieldPart");
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("ReqDodgePart");
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("ReqUnitKind");
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("ReqMinLevel");
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("ReqMaxLevel");
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("Weight");
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("Range");
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("Position");
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("Scarcity");
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("MinTradeQuality");
	m_GridCurrent.SetItem(&Item);

#ifdef SHOP_PRICES_PER_BUILDING_NPC
	Item.col = m_nCols++;
	Item.strText.Format("SellingPrice");
	m_GridCurrent.SetItem(&Item);
#else
	Item.col = m_nCols++;
	Item.strText.Format("Price");
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("CashPrice");
	m_GridCurrent.SetItem(&Item);

#endif
	
	Item.col = m_nCols++;
	Item.strText.Format("Time");
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("RangeAngle");
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("BoosterAngle");
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("ItemAttribute");
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("Description");
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("SourceIndex");
	m_GridCurrent.SetItem(&Item);

	// arrange grid
	m_GridCurrent.AutoSize();
//	m_GridCurrent.ExpandColumnsToFit();

	// clean all cells
	CCellRange tmpCellRange(1, 0, m_GridCurrent.GetRowCount()-1, m_GridCurrent.GetColumnCount()-1);
	m_GridCurrent.ClearCells(tmpCellRange);
}

void CEngineItemDlg::InitGridUpdate()
{
	m_GridUpdate.SetBkColor(0xFFFFFF);

	int m_nRows = 1;
	int m_nCols = 29;

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
	Item.strText.Format("ItemNum");
	m_GridUpdate.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("Kind");
	m_GridUpdate.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("ItemName");
	m_GridUpdate.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("AbilityMin");
	m_GridUpdate.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("AbilityMax");
	m_GridUpdate.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("ReqRace");
	m_GridUpdate.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("ReqAttackPart");
	m_GridUpdate.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("ReqDefensePart");
	m_GridUpdate.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("ReqFuelPart");
	m_GridUpdate.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("ReqSoulPart");
	m_GridUpdate.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("ReqShieldPart");
	m_GridUpdate.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("ReqDodgePart");
	m_GridUpdate.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("ReqUnitKind");
	m_GridUpdate.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("ReqMinLevel");
	m_GridUpdate.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("ReqMaxLevel");
	m_GridUpdate.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("Weight");
	m_GridUpdate.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("Range");
	m_GridUpdate.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("Position");
	m_GridUpdate.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("Scarcity");
	m_GridUpdate.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("MinTradeQuality");
	m_GridUpdate.SetItem(&Item);

#ifdef SHOP_PRICES_PER_BUILDING_NPC
	Item.col = m_nCols++;
	Item.strText.Format("SellingPrice");
	m_GridUpdate.SetItem(&Item);
#else
	Item.col = m_nCols++;
	Item.strText.Format("Price");
	m_GridUpdate.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("CashPrice");
	m_GridUpdate.SetItem(&Item);
#endif

	Item.col = m_nCols++;
	Item.strText.Format("Time");
	m_GridUpdate.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("RangeAngle");
	m_GridUpdate.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("BoosterAngle");
	m_GridUpdate.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("ItemAttribute");
	m_GridUpdate.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("Description");
	m_GridUpdate.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("SourceIndex");
	m_GridUpdate.SetItem(&Item);

	// arrange grid
	m_GridUpdate.AutoSize();
//	m_GridUpdate.ExpandColumnsToFit();

	// clean all cells
	CCellRange tmpCellRange(1, 0, m_GridUpdate.GetRowCount()-1, m_GridUpdate.GetColumnCount()-1);
	m_GridUpdate.ClearCells(tmpCellRange);
}

void CEngineItemDlg::GetCurrentItemList()
{
	GV_ITEM Item;
	Item.mask = GVIF_TEXT|GVIF_FORMAT;
	Item.row = 1;
	Item.nFormat = ((DWORD)DT_LEFT|DT_VCENTER|DT_SINGLELINE);
	
	CString szSQLQuery;
	
	// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
	//szSQLQuery.Format("select itemnum, kind, itemname, abilitymin, abilitymax, reqrace, reqattackpart, reqdefensepart, ReqFuelPart, reqsoulpart, reqshieldpart, reqdodgepart, requnitkind, reqminlevel, reqmaxlevel, weight, range, position, scarcity, mintradequantity, price, cashprice, [time], rangeangle, boosterangle, itemattribute, description, sourceindex from ti_item where kind = %d"
	//	, ITEMKIND_SUPPORT);
#ifdef SHOP_PRICES_PER_BUILDING_NPC
	szSQLQuery.Format(QUERY_080702_0091_B, ITEMKIND_SUPPORT);
#else
	szSQLQuery.Format(QUERY_080702_0091, ITEMKIND_SUPPORT);
#endif

	SQLINTEGER cb1, cb2, cb3, cb4, cb5, cb6, cb7, cb8, cb9, cb10, cb11, cb12, cb13, cb14, cb15, cb16, cb17, cb18, cb19, cb20, cb21, cb22, cb23, cb24, cb25, cb26, cb27, cb28;

	_TI_ITEM		st_EngineItem;
	memset(&st_EngineItem, 0, sizeof(st_EngineItem));

	int i = 0;
	SQLBindCol(m_pODBCStmt->m_hstmt, ++i, SQL_C_ULONG, &st_EngineItem.ItemNum, 0, &cb1);
	SQLBindCol(m_pODBCStmt->m_hstmt, ++i, SQL_C_UTINYINT, &st_EngineItem.Kind, 0, &cb2);
	SQLBindCol(m_pODBCStmt->m_hstmt, ++i, SQL_C_CHAR, &st_EngineItem.ItemName, 20, &cb3);
	SQLBindCol(m_pODBCStmt->m_hstmt, ++i, SQL_C_FLOAT, &st_EngineItem.AbilityMin, 0, &cb4);
	SQLBindCol(m_pODBCStmt->m_hstmt, ++i, SQL_C_FLOAT, &st_EngineItem.AbilityMax, 0, &cb5);
	SQLBindCol(m_pODBCStmt->m_hstmt, ++i, SQL_C_SSHORT, &st_EngineItem.ReqRace, 0, &cb6);
	SQLBindCol(m_pODBCStmt->m_hstmt, ++i, SQL_C_SSHORT, &st_EngineItem.ReqAttackPart, 0, &cb7);
	SQLBindCol(m_pODBCStmt->m_hstmt, ++i, SQL_C_SSHORT, &st_EngineItem.ReqDefensePart, 0, &cb8);
	SQLBindCol(m_pODBCStmt->m_hstmt, ++i, SQL_C_SSHORT, &st_EngineItem.ReqFuelPart, 0, &cb9);
	SQLBindCol(m_pODBCStmt->m_hstmt, ++i, SQL_C_SSHORT, &st_EngineItem.ReqSoulPart, 0, &cb10);

	SQLBindCol(m_pODBCStmt->m_hstmt, ++i, SQL_C_SSHORT, &st_EngineItem.ReqShieldPart, 0, &cb11);
	SQLBindCol(m_pODBCStmt->m_hstmt, ++i, SQL_C_SSHORT, &st_EngineItem.ReqDodgePart, 0, &cb12);
	SQLBindCol(m_pODBCStmt->m_hstmt, ++i, SQL_C_ULONG, &st_EngineItem.ReqUnitKind, 0, &cb13);
	SQLBindCol(m_pODBCStmt->m_hstmt, ++i, SQL_C_UTINYINT, &st_EngineItem.ReqMinLevel, 0, &cb14);
	SQLBindCol(m_pODBCStmt->m_hstmt, ++i, SQL_C_UTINYINT, &st_EngineItem.ReqMaxLevel, 0, &cb15);

	SQLBindCol(m_pODBCStmt->m_hstmt, ++i, SQL_C_SSHORT, &st_EngineItem.Weight, 0, &cb16);
	SQLBindCol(m_pODBCStmt->m_hstmt, ++i, SQL_C_SSHORT, &st_EngineItem.Range, 0, &cb17);

	SQLBindCol(m_pODBCStmt->m_hstmt, ++i, SQL_C_UTINYINT, &st_EngineItem.Position, 0, &cb18);
	SQLBindCol(m_pODBCStmt->m_hstmt, ++i, SQL_C_UTINYINT, &st_EngineItem.Scarcity, 0, &cb19);

	SQLBindCol(m_pODBCStmt->m_hstmt, ++i, SQL_C_SSHORT, &st_EngineItem.MinTradeQuantity, 0, &cb20);

#ifdef SHOP_PRICES_PER_BUILDING_NPC
	SQLBindCol(m_pODBCStmt->m_hstmt, ++i, SQL_C_ULONG, &st_EngineItem.SellingPrice, 0, &cb21);
#else
	SQLBindCol(m_pODBCStmt->m_hstmt, ++i, SQL_C_ULONG, &st_EngineItem.Price, 0, &cb21);
	SQLBindCol(m_pODBCStmt->m_hstmt, ++i, SQL_C_ULONG, &st_EngineItem.CashPrice, 0, &cb22);
#endif
	SQLBindCol(m_pODBCStmt->m_hstmt, ++i, SQL_C_ULONG, &st_EngineItem.Time, 0, &cb23);

	SQLBindCol(m_pODBCStmt->m_hstmt, ++i, SQL_C_FLOAT, &st_EngineItem.RangeAngle, 0, &cb24);
	SQLBindCol(m_pODBCStmt->m_hstmt, ++i, SQL_C_FLOAT, &st_EngineItem.BoosterAngle, 0, &cb25);

	SQLBindCol(m_pODBCStmt->m_hstmt, ++i, SQL_C_ULONG, &st_EngineItem.ItemAttribute, 0, &cb26);
	SQLBindCol(m_pODBCStmt->m_hstmt, ++i, SQL_C_CHAR, &st_EngineItem.Description, 200, &cb27);
	SQLBindCol(m_pODBCStmt->m_hstmt, ++i, SQL_C_ULONG, &st_EngineItem.SourceIndex, 0, &cb28);

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
		InsertItemToGridCurrent(&st_EngineItem, nCount+1);

		nCount++;
		memset(&st_EngineItem, 0, sizeof(st_EngineItem));
	}

	// clean up
	m_pODBCStmt->FreeStatement();

	m_GridCurrent.AutoSize();

}

void CEngineItemDlg::InsertItemToGridCurrent(_TI_ITEM *i_pTi_Item, int i_nRow)
{	
	// 칼럼 만들기
	int m_nCols = 0;

	// 컬럼에 값 넣기
	GV_ITEM Item;
	Item.mask = GVIF_TEXT|GVIF_FORMAT;
	Item.row = i_nRow;
	Item.nFormat = ((DWORD)DT_LEFT|DT_VCENTER|DT_SINGLELINE);

	Item.col = m_nCols++;
	Item.strText.Format("%d", i_pTi_Item->ItemNum);
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("%d", i_pTi_Item->Kind);
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("%s", i_pTi_Item->ItemName);
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("%.1f", i_pTi_Item->AbilityMin);
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("%.1f", i_pTi_Item->AbilityMax);
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("%d", i_pTi_Item->ReqRace);
	m_GridCurrent.SetItem(&Item);
	
	Item.col = m_nCols++;
	Item.strText.Format("%d", i_pTi_Item->ReqAttackPart);
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("%d", i_pTi_Item->ReqDefensePart);
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("%d", i_pTi_Item->ReqFuelPart);
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("%d", i_pTi_Item->ReqSoulPart);
	m_GridCurrent.SetItem(&Item);
	/////////////////////////////////////////////////10 items

	Item.col = m_nCols++;
	Item.strText.Format("%d", i_pTi_Item->ReqShieldPart);
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("%d", i_pTi_Item->ReqDodgePart);
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("%d", i_pTi_Item->ReqUnitKind);
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("%d", i_pTi_Item->ReqMinLevel);
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("%d", i_pTi_Item->ReqMaxLevel);
	m_GridCurrent.SetItem(&Item);
	
	Item.col = m_nCols++;
	Item.strText.Format("%d", i_pTi_Item->Weight);
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("%d", i_pTi_Item->Range);
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("%d", i_pTi_Item->Position);
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("%d", i_pTi_Item->Scarcity);
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("%d", i_pTi_Item->MinTradeQuantity);
	m_GridCurrent.SetItem(&Item);
	/////////////////////////////////////////////////20items

#ifdef SHOP_PRICES_PER_BUILDING_NPC
	Item.col = m_nCols++;
	Item.strText.Format("%d", i_pTi_Item->SellingPrice);
	m_GridCurrent.SetItem(&Item);
#else
	Item.col = m_nCols++;
	Item.strText.Format("%d", i_pTi_Item->Price);
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("%d", i_pTi_Item->CashPrice);
	m_GridCurrent.SetItem(&Item);
#endif	// SHOP_PRICES_PER_BUILDING_NPC

	Item.col = m_nCols++;
	Item.strText.Format("%d", i_pTi_Item->Time);
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("%f", i_pTi_Item->RangeAngle);
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("%f", i_pTi_Item->BoosterAngle);
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("%d", i_pTi_Item->ItemAttribute);
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("%s", i_pTi_Item->Description);
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("%d", i_pTi_Item->SourceIndex);
	m_GridCurrent.SetItem(&Item);
}

void CEngineItemDlg::SetDetailDialog(int row)
{
	//선택된 셀을 바탕으로 값을 설정한다.

	m_pEngineItemDetailDlg->m_valueEditItemNum = m_GridCurrent.GetItemText(row,0);
	m_pEngineItemDetailDlg->m_ctrlComboKind.SetWindowText(CAtumSJ::GetItemKindName(atoi(m_GridCurrent.GetItemText(row,1))));
	m_pEngineItemDetailDlg->m_valueEditItemName = m_GridCurrent.GetItemText(row,2);
	m_pEngineItemDetailDlg->m_valueEditAbilityMin = m_GridCurrent.GetItemText(row,3);
	m_pEngineItemDetailDlg->m_valueEditAbilityMax = m_GridCurrent.GetItemText(row,4);
	m_pEngineItemDetailDlg->m_ctrlComboReqRace.SetWindowText(CAtumSJ::GetRaceString(atoi(m_GridCurrent.GetItemText(row,5))));
	m_pEngineItemDetailDlg->m_valueEditReqAttack = m_GridCurrent.GetItemText(row,6);
	m_pEngineItemDetailDlg->m_valueEditReqDefense = m_GridCurrent.GetItemText(row,7);
	m_pEngineItemDetailDlg->m_valueEditReqFuel = m_GridCurrent.GetItemText(row,8);
	m_pEngineItemDetailDlg->m_valueEditReqSoul = m_GridCurrent.GetItemText(row,9);
	m_pEngineItemDetailDlg->m_valueEditReqShield = m_GridCurrent.GetItemText(row,10);
	m_pEngineItemDetailDlg->m_valueEditReqDodge = m_GridCurrent.GetItemText(row,11);
	int tempBit = atoi((char*)(LPCTSTR)m_GridCurrent.GetItemText(row,12)); //unitkind
	if(IS_BGEAR((USHORT)tempBit) == TRUE)
		m_pEngineItemDetailDlg->m_valueCheckBGEAR = TRUE;
	if(IS_MGEAR((USHORT)tempBit) == TRUE)
		m_pEngineItemDetailDlg->m_valueCheckMGEAR = TRUE;
	if(IS_AGEAR((USHORT)tempBit) == TRUE)
		m_pEngineItemDetailDlg->m_valueCheckAGEAR = TRUE;
	if(IS_IGEAR((USHORT)tempBit) == TRUE)
		m_pEngineItemDetailDlg->m_valueCheckIGEAR = TRUE;
	m_pEngineItemDetailDlg->m_valueEditReqMinLevel = m_GridCurrent.GetItemText(row,13);
	m_pEngineItemDetailDlg->m_valueEditReqMaxLevel = m_GridCurrent.GetItemText(row,14);

	m_pEngineItemDetailDlg->m_valueEditWeight = m_GridCurrent.GetItemText(row,15);
	
	m_pEngineItemDetailDlg->m_valueEditRange = m_GridCurrent.GetItemText(row,16);
	m_pEngineItemDetailDlg->m_ctrlComboPosition.SetWindowText(CAtumSJ::GetPositionString(atoi(m_GridCurrent.GetItemText(row,17)))); //position
	
	m_pEngineItemDetailDlg->m_valueEditScarcity = m_GridCurrent.GetItemText(row,18);
	m_pEngineItemDetailDlg->m_valueEditMinTradeQuantity = m_GridCurrent.GetItemText(row,19);
	m_pEngineItemDetailDlg->m_valueEditPrice = m_GridCurrent.GetItemText(row,20);
	m_pEngineItemDetailDlg->m_valueEditCashPrice = m_GridCurrent.GetItemText(row,21);
	
	m_pEngineItemDetailDlg->m_valueEditTime = m_GridCurrent.GetItemText(row,22);
	m_pEngineItemDetailDlg->m_valueEditRangeAngle = m_GridCurrent.GetItemText(row,23);
	m_pEngineItemDetailDlg->m_valueEditBoosterAngle = m_GridCurrent.GetItemText(row,24);

	unsigned char tempValue = atoi((char*)(LPCTSTR)m_GridCurrent.GetItemText(row,25));
	if(tempValue & 0x01)
		m_pEngineItemDetailDlg->m_valueCheckAutoPickUp = TRUE;
	if(tempValue >> 1 & 0x01)
		m_pEngineItemDetailDlg->m_valueCheckNoTransfer = TRUE;
	if(tempValue >> 2 & 0x01)
		m_pEngineItemDetailDlg->m_valueCheckQuestItem = TRUE;
	if(tempValue >> 3 & 0x01)
		m_pEngineItemDetailDlg->m_valueCheckTimeLimite = TRUE;
	if(tempValue >> 16 & 0x01)
		m_pEngineItemDetailDlg->m_valueCheckCashItem = TRUE;

	m_pEngineItemDetailDlg->m_valueEditDescription = m_GridCurrent.GetItemText(row,26);
	m_pEngineItemDetailDlg->m_valueEditSourceIndex = m_GridCurrent.GetItemText(row,27);
	
	m_pEngineItemDetailDlg->UpdateData(FALSE);
}
