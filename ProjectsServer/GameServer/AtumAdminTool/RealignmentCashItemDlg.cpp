// RealignmentCashItemDlg.cpp : implementation file
//

#include "stdafx.h"
#include "atumadmintool.h"
#include "RealignmentCashItemDlg.h"
#include "AtumAdminToolDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRealignmentCashItemDlg dialog


CRealignmentCashItemDlg::CRealignmentCashItemDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRealignmentCashItemDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRealignmentCashItemDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	
	m_pMainDlg = (CAtumAdminToolDlg*)AfxGetMainWnd();
	m_pTargetItem			= NULL;
	m_ptempItem				= new NM_GRIDVIEW;
}

CRealignmentCashItemDlg::~CRealignmentCashItemDlg()
{
	util::del(m_ptempItem);
}

void CRealignmentCashItemDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRealignmentCashItemDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
	DDX_GridControl(pDX, IDC_GRID_CUR_REALIGN_ITEM, m_GridCurRealignCashItem);
}


BEGIN_MESSAGE_MAP(CRealignmentCashItemDlg, CDialog)
	//{{AFX_MSG_MAP(CRealignmentCashItemDlg)
	ON_BN_CLICKED(BTN_UPDATE_TO_DB, OnRealignmentUpdateToDB)
	//ON_NOTIFY(NM_OUTOFMEMORY, IDC_SPIN1, OnUpRealignmentSpin)
	ON_BN_CLICKED(IDC_BUTTON1, OnButtonUP)
	ON_BN_CLICKED(IDC_UP, OnUp)
	ON_BN_CLICKED(IDC_DOWN, OnDown)
	//}}AFX_MSG_MAP
	ON_NOTIFY(NM_CLICK, IDC_GRID_CUR_REALIGN_ITEM, OnGridCurRealignCashItem)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRealignmentCashItemDlg message handlers



BOOL CRealignmentCashItemDlg::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	// TODO: Add your specialized code here and/or call the base class
	return CDialog::OnNotify(wParam, lParam, pResult);
}

void CRealignmentCashItemDlg::OnGridCurRealignCashItem(NMHDR *pNotifyStruct, LRESULT* pResult)
{
	if( pNotifyStruct == NULL)
	{
		return;
	}
	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*) pNotifyStruct;
	if(0 > pItem->iRow &&  m_vectCurRealignCashItemList.size() < pItem->iRow )
	{
		return;
	}
	if(pItem->iRow )
	{
		CGridCellBase *pItemNumGCell = m_GridCurRealignCashItem.GetCell(pItem->iRow, GRID_COL_REINDEX_ITEMNUM);
		
		m_pTargetItem	= FindShopItem(atoi(pItemNumGCell->GetText()));
		if(NULL == m_pTargetItem)
		{
			AfxMessageBox("Can't find this Item from Shop List!!");
			return;
		}
	}
	return;	
}

void CRealignmentCashItemDlg::_InitGrid(CGridCtrl *i_pGridCtrl)
{
	i_pGridCtrl->SetBkColor(0xFFFFFF);
	
	int nRows = 1;
	int nCols = 7;
	
	i_pGridCtrl->SetEditable(FALSE);		 
	i_pGridCtrl->SetListMode(TRUE);
	i_pGridCtrl->SetSingleRowSelection(TRUE);
	i_pGridCtrl->EnableSelection(TRUE);
	i_pGridCtrl->SetFrameFocusCell(TRUE);
	i_pGridCtrl->SetTrackFocusCell(TRUE);
	
	i_pGridCtrl->SetRowCount(nRows);
	i_pGridCtrl->SetColumnCount(nCols);
	i_pGridCtrl->SetFixedRowCount(1);
	
	// 칼럼 만들기
	nCols			= 0;
	GV_ITEM Item;
	Item.mask		= GVIF_TEXT|GVIF_FORMAT;
	Item.row		= 0;
	Item.nFormat	= GRID_CELL_FORMAT;
	
	Item.col		= nCols++;
	Item.strText.Format("New_Order");
	i_pGridCtrl->SetItem(&Item);

	Item.col = GRID_COL_REINDEX_ITEMNUM;
	Item.strText.Format("ItemNum");
	i_pGridCtrl->SetItem(&Item);
	
	Item.col = GRID_COL_REINDEX_ITEMNAME;
	Item.strText.Format("ItemName");
	i_pGridCtrl->SetItem(&Item);
	
	Item.col = GRID_COL_REINDEX_RECOMMAND_ITEM;
	Item.strText.Format("Recommand");
	i_pGridCtrl->SetItem(&Item);
	
	Item.col = GRID_COL_REINDEX_NEW_ITEM;
	Item.strText.Format("NEW");
	i_pGridCtrl->SetItem(&Item);
	
	Item.col = GRID_COL_REINDEX_LIMITED_EDITION;
	Item.strText.Format("Limited Ed.");
	i_pGridCtrl->SetItem(&Item);

	Item.col = GRID_COL_REINDEX_PREVIOUSE_INDEX;
	Item.strText.Format("Pre_Order");
	i_pGridCtrl->SetItem(&Item);

	// arrange grid
	i_pGridCtrl->AutoSize();
	i_pGridCtrl->ExpandColumnsToFit();
	
	// clean all cells
	CCellRange tmpCellRange(1, 0, i_pGridCtrl->GetRowCount()-1, i_pGridCtrl->GetColumnCount()-1);
	i_pGridCtrl->ClearCells(tmpCellRange);
}

void CRealignmentCashItemDlg::InitGridCurRealignCashItem(void)
{
	sort(m_vectCurRealignCashItemList.begin(), m_vectCurRealignCashItemList.end(), sort_SAT_SHOP_REALIGN_ITEM_by_Realign());

	m_ilCheckImageList.DeleteImageList();
	m_ilCheckImageList.Create(24, 12, ILC_COLOR32, 1, 1);
	
	CBitmap bmCheckImage;
	bmCheckImage.LoadBitmap(IDB_BITMAP_CHECK_IMAGE);
	m_ilCheckImageList.Add(&bmCheckImage, RGB(0,0,0));
	
	m_GridCurRealignCashItem.SetImageList(&m_ilCheckImageList);
	this->_InitGrid(&m_GridCurRealignCashItem);
}

INT CRealignmentCashItemDlg::_AddCashItem(CGridCtrl *i_pGridCtrl, SAT_SHOP_REALIGN_ITEM *i_pShopItem)
{
	GV_ITEM Item;
	
	Item.mask		= GVIF_TEXT|GVIF_FORMAT;
	Item.nFormat	= DT_LEFT|DT_VCENTER|DT_SINGLELINE;
	
	int nNewRowIdx = i_pGridCtrl->GetRowCount();
	i_pGridCtrl->SetRowCount(nNewRowIdx+1);

	//select된 값을 GridDetail GridCtrl에 넣어준다.
	Item.row		= nNewRowIdx;
	Item.col		= 0;
	Item.nFormat	= DT_CENTER|DT_VCENTER|DT_SINGLELINE;
	Item.strText.Format("%d", nNewRowIdx);
	i_pGridCtrl->SetItem(&Item);
	
	Item.col		= GRID_COL_REINDEX_ITEMNUM;
	Item.strText.Format("%d", i_pShopItem->ItemNum);
	i_pGridCtrl->SetItem(&Item);
	
	Item.col		= GRID_COL_REINDEX_ITEMNAME;
	Item.nFormat	= DT_LEFT|DT_VCENTER|DT_SINGLELINE;
	Item.strText.Format("%s", i_pShopItem->ItemName);
	i_pGridCtrl->SetItem(&Item);
	
	Item.mask		= GVIF_FORMAT|GVIF_IMAGE|GVIF_BKCLR;
	Item.col		= GRID_COL_REINDEX_RECOMMAND_ITEM;
	Item.iImage		= IS_RECOMMEND_CASHSHOPITEM_BY_CASHSHOPINDEX(i_pShopItem->CashShopBit) ? 1 : 0;
	Item.crBkClr	= RGB(200, 200, 200);
	i_pGridCtrl->SetItem(&Item);
	
	Item.mask		= GVIF_FORMAT|GVIF_IMAGE|GVIF_BKCLR;
	Item.col		= GRID_COL_REINDEX_NEW_ITEM;
	Item.iImage		= IS_NEW_CASHSHOPITEM_BY_CASHSHOPINDEX(i_pShopItem->CashShopBit) ? 1 : 0;
	Item.crBkClr	= RGB(200, 200, 200);
	i_pGridCtrl->SetItem(&Item);
	
	Item.col		= GRID_COL_REINDEX_LIMITED_EDITION;
	Item.mask		= GVIF_TEXT|GVIF_FORMAT;
	Item.nFormat	= DT_LEFT|DT_VCENTER|DT_SINGLELINE;
	Item.strText.Format("%d", i_pShopItem->RemainCountForLimitedEdition);
	i_pGridCtrl->SetItem(&Item);

	Item.col		= GRID_COL_REINDEX_PREVIOUSE_INDEX;
	Item.mask		= GVIF_TEXT|GVIF_FORMAT;
	Item.nFormat	= DT_CENTER|DT_VCENTER|DT_SINGLELINE;
	Item.crBkClr	= RGB(200, 200, 200);
	Item.strText.Format("%d", i_pShopItem->PreviousIndex + 1);
	i_pGridCtrl->SetItem(&Item);	

	return nNewRowIdx;
}

void CRealignmentCashItemDlg::ViewGrid(CGridCtrl *i_pGridCtrl)
{
	BOOL bIsChangedGrid = (m_GridCurRealignCashItem.GetDlgCtrlID() == i_pGridCtrl->GetDlgCtrlID());
	
	vectSAT_SHOP_REALIGN_ITEM::iterator itr(m_vectCurRealignCashItemList.begin());
	for(; itr != m_vectCurRealignCashItemList.end(); itr++)
	{
		SAT_SHOP_REALIGN_ITEM *pShopItem = &*itr;
		itr->TempRows = _AddCashItem(i_pGridCtrl, pShopItem);
	}
	i_pGridCtrl->UpdateData();
	i_pGridCtrl->AutoSize();
}

SAT_SHOP_REALIGN_ITEM *CRealignmentCashItemDlg::FindShopItem(int i_ItemNum)
{
	vectSAT_SHOP_REALIGN_ITEM::iterator itr(m_vectCurRealignCashItemList.begin());
	for(; itr != m_vectCurRealignCashItemList.end(); itr++)
	{
		SAT_SHOP_REALIGN_ITEM *pShopItem = &*itr;
		if(i_ItemNum == pShopItem->ItemNum)
		{
			return pShopItem;
		}
	}
	
	return NULL;
}

BOOL CRealignmentCashItemDlg::DBQueryLoadCashRealignItem(vectSAT_SHOP_REALIGN_ITEM *o_pVectItemList)
{
	SQLHSTMT hstmt = m_odbcStmt2.GetSTMTHandle();
	BOOL bRet = m_odbcStmt2.ExecuteQuery(PROCEDURE_130204_0001);		//[atum_Load_Realignment_CashShop]
	if (!bRet)
	{
		m_odbcStmt2.FreeStatement();		// cleanup		
		AfxMessageBox("DBQueryLoadCashItem error !!");
		return FALSE;
	}	
	SQLINTEGER arrCB[6]={SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS};
	SAT_SHOP_REALIGN_ITEM tmShopItem;
	SQLBindCol(hstmt,  1, SQL_C_LONG, &tmShopItem.ItemNum, 0,					&arrCB[1]);
	SQLBindCol(hstmt,  2, SQL_C_CHAR, &tmShopItem.ItemName, SIZE_MAX_ITEM_NAME,	&arrCB[2]);
	SQLBindCol(hstmt,  3, SQL_C_TINYINT, &tmShopItem.CashShopBit, 0,			&arrCB[3]);
	SQLBindCol(hstmt,  4, SQL_C_LONG, &tmShopItem.RemainCountForLimitedEdition, 0,	&arrCB[4]);
	SQLBindCol(hstmt,  5, SQL_C_LONG, &tmShopItem.RealignmentIndex, 0,			&arrCB[5]);
	do
	{
		util::zero(&tmShopItem, sizeof(tmShopItem));
		tmShopItem.RemainCountForLimitedEdition		= UNLIMITED_REMAIN_COUNT_FOR_LIMITED_EDITION;	// 2010-01-26 by cmkwon, 캐쉬 아이템 한정판매 시스템 구현 - 초기값 설정 해야함.

		SQLRETURN ret = SQLFetch(hstmt);
		if(SQL_ERROR == ret)
		{
			m_odbcStmt2.ProcessLogMessagesForStmt(PROCEDURE_130204_0001);	//[atum_Load_Realignment_CashShop]
		}
		if(SQL_SUCCESS != ret && SQL_SUCCESS_WITH_INFO != ret)
		{
			break;
		}
		tmShopItem.OriginalCashShopBit	= tmShopItem.CashShopBit;
		tmShopItem.TempRows = 0;
		tmShopItem.PreviousIndex = tmShopItem.RealignmentIndex;
		o_pVectItemList->push_back(tmShopItem);
	}while(TRUE);

	m_odbcStmt2.FreeStatement();		// cleanup

	return TRUE;
}

BOOL CRealignmentCashItemDlg::DBQueryUpdateCashRealignItem(vectSAT_SHOP_REALIGN_ITEM *i_pVectItemList)
{
	SQLHSTMT hstmt = m_odbcStmt2.GetSTMTHandle();
	vectSAT_SHOP_REALIGN_ITEM::iterator itr(i_pVectItemList->begin());
	for (; itr != i_pVectItemList->end(); itr++)
	{
		SAT_SHOP_REALIGN_ITEM *pShopItem = &*itr;
		
		SQLINTEGER arrCB[4]={SQL_NTS,SQL_NTS};
		SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pShopItem->ItemNum, 0,			&arrCB[1]);
		SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_LONG, SQL_TINYINT, 0, 0, &pShopItem->RealignmentIndex, 0,	&arrCB[2]);
		BOOL bRet = m_odbcStmt2.ExecuteQuery(PROCEDURE_130204_0002);
		if (!bRet)
		{
			m_odbcStmt2.FreeStatement();		// cleanup		
			AfxMessageBox("DBQueryUpdateAdminNoticeInfo error !!");
			return FALSE;
		}
		m_odbcStmt2.FreeStatement();			// cleanup				
	}
	return TRUE;
}


BOOL CRealignmentCashItemDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	if ( FALSE == m_odbcStmt2.Init(m_pMainDlg->m_pServerInfo4Admin->DBIP, m_pMainDlg->m_pServerInfo4Admin->DBPort, m_pMainDlg->m_pServerInfo4Admin->DBName,
		m_pMainDlg->m_pServerInfo4Admin->DBUID, m_pMainDlg->m_pServerInfo4Admin->DBPWD, GetSafeHwnd()) )
	{
		char szTemp[1024];
		sprintf(szTemp, "Can not connect DBServer<%s(%s:%d)> !!"
			, m_pMainDlg->m_pServerInfo4Admin->DBName, m_pMainDlg->m_pServerInfo4Admin->DBIP
			, m_pMainDlg->m_pServerInfo4Admin->DBPort);
		MessageBox(szTemp);
		EndDialog(-1);
		return FALSE;
	}

	m_vectCurRealignCashItemList.clear();
	this->DBQueryLoadCashRealignItem(&m_vectCurRealignCashItemList);

	this->InitGridCurRealignCashItem();
	this->ViewGrid(&m_GridCurRealignCashItem);

	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CRealignmentCashItemDlg::OnRealignmentUpdateToDB() 
{
	// TODO: Add your control notification handler code here
	m_pTargetItem = NULL;	
	sort(m_vectCurRealignCashItemList.begin(), m_vectCurRealignCashItemList.end(), sort_SAT_SHOP_REALIGN_ITEM_by_Realign());

	vectSAT_SHOP_REALIGN_ITEM::iterator itr(m_vectCurRealignCashItemList.begin());
	
	for(int i=0; i < m_vectCurRealignCashItemList.size(); i++)
	{
		itr[i].RealignmentIndex = i;
	}
	
	MessageBox("Database is saved and reloading....!!");
	this->DBQueryUpdateCashRealignItem(&m_vectCurRealignCashItemList);
	this->OnInitDialog();
}

void CRealignmentCashItemDlg::OnUp() 
{
	// TODO: Add your control notification handler code here
	SAT_SHOP_REALIGN_ITEM *pTargetOver = NULL;

	if( m_pTargetItem == NULL )
	{
		return; 
	}
	vectSAT_SHOP_REALIGN_ITEM::iterator itr(m_vectCurRealignCashItemList.begin());
	for(; itr != m_vectCurRealignCashItemList.end(); itr++)
	{
		SAT_SHOP_REALIGN_ITEM *pShopItem = &*itr;
		if(m_pTargetItem->ItemNum == pShopItem->ItemNum)
		{
			if( itr == m_vectCurRealignCashItemList.begin() )
			{
				MessageBox("The first row!!");
				return;
			}
			pTargetOver = &*(--itr);
			swap(m_pTargetItem->RealignmentIndex,pTargetOver->RealignmentIndex);
			this->InitGridCurRealignCashItem();
			this->ViewGrid(&m_GridCurRealignCashItem);

			m_pTargetItem->TempRows--;
			m_ptempItem->iRow = m_pTargetItem->TempRows;

			CCellID currentCell;
			currentCell.row = m_pTargetItem->TempRows;
			currentCell.col = 0;
 			m_GridCurRealignCashItem.SelectRows(currentCell);

			this->OnGridCurRealignCashItem( (NMHDR*)m_ptempItem, NULL);

			return;
		}
	}
}

void CRealignmentCashItemDlg::OnDown() 
{
	// TODO: Add your control notification handler code here
	SAT_SHOP_REALIGN_ITEM *pTargetUnder = NULL;

	if( m_pTargetItem == NULL )
	{
		return; 
	}
	vectSAT_SHOP_REALIGN_ITEM::iterator itr(m_vectCurRealignCashItemList.begin());
	for(; itr != m_vectCurRealignCashItemList.end(); itr++)
	{
		SAT_SHOP_REALIGN_ITEM *pShopItem = &*itr;
		if(m_pTargetItem->ItemNum == pShopItem->ItemNum)
		{
			pTargetUnder = &*(++itr);
			if (itr == m_vectCurRealignCashItemList.end())
			{
				MessageBox("The last row!!");
				return;
			}
			swap(m_pTargetItem->RealignmentIndex,pTargetUnder->RealignmentIndex);
			this->InitGridCurRealignCashItem();
			this->ViewGrid(&m_GridCurRealignCashItem);

			m_pTargetItem->TempRows++;
			m_ptempItem->iRow = m_pTargetItem->TempRows;

			CCellID currentCell;
			currentCell = m_pTargetItem->TempRows;
			currentCell.col = 0;
 			m_GridCurRealignCashItem.SelectRows(currentCell);

			this->OnGridCurRealignCashItem( (NMHDR*)m_ptempItem, NULL);
			return;
		}
	}
}

void CRealignmentCashItemDlg::OnButtonUP() 
{
	// TODO: Add your control notification handler code here
	
}
