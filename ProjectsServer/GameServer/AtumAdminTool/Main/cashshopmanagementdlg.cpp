// cashshopmanagementdlg.cpp : implementation file
//

#include "stdafx.h"
#include "..\atumadmintool.h"
#include "cashshopmanagementdlg.h"
#include "AtumAdminToolDlg.h"			// 2009-01-28 by cmkwon, 캐쉬샾 수정(추천탭,신상품 추가) - 

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCashShopManagementDlg dialog


CCashShopManagementDlg::CCashShopManagementDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCashShopManagementDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCashShopManagementDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pMainDlg = (CAtumAdminToolDlg*)AfxGetMainWnd();
	m_pCRealignmentCashItemDlg		= NULL;			// 2013-02-05 by bckim, 캐시샵 추천탭 정렬순서부여
}

// 2013-02-05 by bckim, 캐시샵 추천탭 정렬순서부여
CCashShopManagementDlg::~CCashShopManagementDlg()
{
	util::del(m_pCRealignmentCashItemDlg);	
}
// End. 2013-02-05 by bckim, 캐시샵 추천탭 정렬순서부여



void CCashShopManagementDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCashShopManagementDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
	DDX_GridControl(pDX, IDC_GRID_CUR_ITEM, m_GridCurCashItem);
	DDX_GridControl(pDX, IDC_GRID_CHANGED_ITEM, m_GridChangedCashItem);
}


BEGIN_MESSAGE_MAP(CCashShopManagementDlg, CDialog)
	//{{AFX_MSG_MAP(CCashShopManagementDlg)
	ON_BN_CLICKED(IDC_BTN_DELETE_FROM_CHANGED, OnBtnDeleteFromChanged)
	ON_BN_CLICKED(IDC_BTN_UPDATE_TO_DB, OnBtnUpdateToDb)
	ON_BN_CLICKED(IDC_BTN_REALIGNMENT_CASH_ITEM, OnBtnRealignmentCashItem)	// 2013-02-05 by bckim, 캐시샵 추천탭 정렬순서부여
	//}}AFX_MSG_MAP
	ON_NOTIFY(NM_CLICK, IDC_GRID_CUR_ITEM, OnGridCurItem)
	ON_NOTIFY(NM_CLICK, IDC_GRID_CHANGED_ITEM, OnGridChangedItem)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCashShopManagementDlg message handlers

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CCashShopManagementDlg::OnGridCurItem(NMHDR *pNotifyStruct, LRESULT* pResult)
/// \brief		// 2009-01-28 by cmkwon, 캐쉬샾 수정(추천탭,신상품 추가) - 
/// \author		cmkwon
/// \date		2009-01-29 ~ 2009-01-29
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CCashShopManagementDlg::OnGridCurItem(NMHDR *pNotifyStruct, LRESULT* pResult)
{
	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*) pNotifyStruct;
	if(0 > pItem->iRow)
	{
		return;
	}

	///////////////////////////////////////////////////////////////////////////////
	// 2010-01-26 by cmkwon, 캐쉬 아이템 한정판매 시스템 구현 - 
	if(GRID_COL_INDEX_LIMITED_EDITION == pItem->iColumn)
	{
		// 2009-01-29 by cmkwon, 설정 가능한지 유효성 체크
		CGridCellBase *pItemNumGCell = m_GridCurCashItem.GetCell(pItem->iRow, GRID_COL_INDEX_ITEMNUM);
		SAT_SHOP_ITEM *pSelShopItem = FindShopItem(atoi(pItemNumGCell->GetText()));
		if(NULL == pSelShopItem)
		{
			AfxMessageBox("Can't find this Item from Shop List!!");
			return;
		}
		CGridCellBase *pGCell = m_GridCurCashItem.GetCell(pItem->iRow, pItem->iColumn);
		CString csMsg;
		csMsg.Format("Do you want to change this Item(%s) ?", pItemNumGCell->GetText());
		int nRet = AfxMessageBox(csMsg, MB_OKCANCEL);
		if(IDCANCEL == nRet)
		{
			return;
		}
		pSelShopItem->IsChangedItem		= TRUE;
		int nCurScrollPos = m_GridCurCashItem.GetScrollPos32(SB_VERT);
		
		this->InitGridCurItem8GridChangedItem();
		this->ViewGrid(&m_GridCurCashItem);
		this->ViewGrid(&m_GridChangedCashItem);
		
		m_GridCurCashItem.SetScrollPos32(SB_VERT, nCurScrollPos);
		return;
	}

	if (GRID_COL_INDEX_RECOMMAND_ITEM != pItem->iColumn && GRID_COL_INDEX_NEW_ITEM != pItem->iColumn)
	{
		return;
	}

	// 2009-01-29 by cmkwon, 설정 가능한지 유효성 체크
	CGridCellBase *pItemNumGCell = m_GridCurCashItem.GetCell(pItem->iRow, GRID_COL_INDEX_ITEMNUM);
	SAT_SHOP_ITEM *pSelShopItem = FindShopItem(atoi(pItemNumGCell->GetText()));
	if(NULL == pSelShopItem)
	{
		AfxMessageBox("Can't find this Item from Shop List!!");
		return;
	}
	ITEM *pItemInfo = m_mapItemInfo.findEZ_ptr(pSelShopItem->ItemNum);
	if(NULL == pItemInfo)
	{
		AfxMessageBox("Can't find this Item from ITEM List!!");
		return;
	}
	if(ITEMKIND_COLOR_ITEM == pItemInfo->Kind)
	{
		AfxMessageBox("You can't set Color Item to Recommand and New !!");
		return;		
	}
	
	// 2009-01-29 by cmkwon, 체크박스 이미지 변경
	CGridCellBase *pGCell = m_GridCurCashItem.GetCell(pItem->iRow, pItem->iColumn);
	int nOriginalImage	= pGCell->GetImage();
	if(0 == nOriginalImage)
	{
		pGCell->SetImage(1);
	}
	else
	{
		pGCell->SetImage(0);
	}
	m_GridCurCashItem.Invalidate();

	// 2009-01-29 by cmkwon, 확인 메시지 박스 
	CString csMsg;
	csMsg.Format("Do you want to change this Item(%s) ?", pItemNumGCell->GetText());
	int nRet = AfxMessageBox(csMsg, MB_OKCANCEL);
	if(IDCANCEL == nRet)
	{
		pGCell->SetImage(nOriginalImage);
		m_GridCurCashItem.Invalidate();
		return;
	}

	// 2009-01-28 by cmkwon, 캐쉬샾 수정(추천탭,신상품 추가) - IsChangedItem를 변경하면 자동으로 그리드는 변경이 된다.
	pSelShopItem->IsChangedItem		= TRUE;
	if(GRID_COL_INDEX_RECOMMAND_ITEM == pItem->iColumn)
	{
		pSelShopItem->CashShopBit	^= CASHSHOP_BIT_RECOMMEND;
	}
	else
	{
		pSelShopItem->CashShopBit	^= CASHSHOP_BIT_NEW;
	}

	int nCurScrollPos = m_GridCurCashItem.GetScrollPos32(SB_VERT);

	this->InitGridCurItem8GridChangedItem();
	this->ViewGrid(&m_GridCurCashItem);
	this->ViewGrid(&m_GridChangedCashItem);

	m_GridCurCashItem.SetScrollPos32(SB_VERT, nCurScrollPos);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CCashShopManagementDlg::OnGridChangedItem(NMHDR *pNotifyStruct, LRESULT* pResult)
/// \brief		// 2009-01-28 by cmkwon, 캐쉬샾 수정(추천탭,신상품 추가) - 
/// \author		cmkwon
/// \date		2009-01-29 ~ 2009-01-29
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CCashShopManagementDlg::OnGridChangedItem(NMHDR *pNotifyStruct, LRESULT* pResult)
{
	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*) pNotifyStruct;
	if(0 > pItem->iRow)
	{
		return;
	}
	
	if (GRID_COL_INDEX_RECOMMAND_ITEM != pItem->iColumn && GRID_COL_INDEX_NEW_ITEM != pItem->iColumn)
	{
		return;
	}

	CGridCellBase *pGCell = m_GridChangedCashItem.GetCell(pItem->iRow, pItem->iColumn);
	int nOriginalImage	= pGCell->GetImage();
	if(0 == nOriginalImage)
	{
		pGCell->SetImage(1);
	}
	else
	{
		pGCell->SetImage(0);
	}
	m_GridChangedCashItem.Invalidate();

	CGridCellBase *pItemNumGCell = m_GridChangedCashItem.GetCell(pItem->iRow, GRID_COL_INDEX_ITEMNUM);
	SAT_SHOP_ITEM *pSelShopItem = FindShopItem(atoi(pItemNumGCell->GetText()));
	if(NULL == pSelShopItem)
	{
		AfxMessageBox("Can't find this Item !!");
		return;
	}
	
	// 2009-01-28 by cmkwon, 캐쉬샾 수정(추천탭,신상품 추가) - 데이터를 변경
	if(GRID_COL_INDEX_RECOMMAND_ITEM == pItem->iColumn)
	{
		pSelShopItem->CashShopBit	^= CASHSHOP_BIT_RECOMMEND;
	}
	else
	{
		pSelShopItem->CashShopBit	^= CASHSHOP_BIT_NEW;
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2009-01-28 by cmkwon, 캐쉬샾 수정(추천탭,신상품 추가) - 
/// \author		cmkwon
/// \date		2009-01-29 ~ 2009-01-29
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CCashShopManagementDlg::_InitGrid(CGridCtrl *i_pGridCtrl)
{
	i_pGridCtrl->SetBkColor(0xFFFFFF);
	
	int nRows = 1;
	int nCols = 6;
	
	i_pGridCtrl->SetEditable(TRUE);		 // 2010-01-26 by cmkwon, 캐쉬 아이템 한정판매 시스템 구현 - 
	i_pGridCtrl->SetListMode(TRUE);
	i_pGridCtrl->SetSingleRowSelection(TRUE);
	i_pGridCtrl->EnableSelection(TRUE);
	i_pGridCtrl->SetFrameFocusCell(FALSE);
	i_pGridCtrl->SetTrackFocusCell(FALSE);
	
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
	Item.strText.Format("NUM");
	i_pGridCtrl->SetItem(&Item);
	
	Item.col = GRID_COL_INDEX_ITEMNUM;
	Item.strText.Format("ItemNum");
	i_pGridCtrl->SetItem(&Item);
	
	Item.col = GRID_COL_INDEX_ITEMNAME;
	Item.strText.Format("ItemName");
	i_pGridCtrl->SetItem(&Item);
	
	Item.col = GRID_COL_INDEX_RECOMMAND_ITEM;
	Item.strText.Format("Recommand");
	i_pGridCtrl->SetItem(&Item);
	
	Item.col = GRID_COL_INDEX_NEW_ITEM;
	Item.strText.Format("NEW");
	i_pGridCtrl->SetItem(&Item);
	
	// 2010-01-26 by cmkwon, 캐쉬 아이템 한정판매 시스템 구현 - 
	Item.col = GRID_COL_INDEX_LIMITED_EDITION;
	Item.strText.Format("Limited Edition");
	i_pGridCtrl->SetItem(&Item);

	// arrange grid
	i_pGridCtrl->AutoSize();
	i_pGridCtrl->ExpandColumnsToFit();
	
	// clean all cells
	CCellRange tmpCellRange(1, 0, i_pGridCtrl->GetRowCount()-1, i_pGridCtrl->GetColumnCount()-1);
	i_pGridCtrl->ClearCells(tmpCellRange);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CCashShopManagementDlg::InitGridCurItem8GridChangedItem(void)
/// \brief		// 2009-01-28 by cmkwon, 캐쉬샾 수정(추천탭,신상품 추가) - 
/// \author		cmkwon
/// \date		2009-01-29 ~ 2009-01-29
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CCashShopManagementDlg::InitGridCurItem8GridChangedItem(void)
{
	m_ilCheckImageList.DeleteImageList();
	m_ilCheckImageList.Create(24, 12, ILC_COLOR32, 1, 1);

	CBitmap bmCheckImage;
	bmCheckImage.LoadBitmap(IDB_BITMAP_CHECK_IMAGE);
	m_ilCheckImageList.Add(&bmCheckImage, RGB(0,0,0));

	m_GridCurCashItem.SetImageList(&m_ilCheckImageList);
	this->_InitGrid(&m_GridCurCashItem);

	m_GridChangedCashItem.SetImageList(&m_ilCheckImageList);
	this->_InitGrid(&m_GridChangedCashItem);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2009-01-28 by cmkwon, 캐쉬샾 수정(추천탭,신상품 추가) - 
/// \author		cmkwon
/// \date		2009-01-29 ~ 2009-01-29
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CCashShopManagementDlg::_AddCashItem(CGridCtrl *i_pGridCtrl, SAT_SHOP_ITEM *i_pShopItem)
{
	GV_ITEM Item;

	Item.mask		= GVIF_TEXT|GVIF_FORMAT;
	Item.nFormat	= DT_LEFT|DT_VCENTER|DT_SINGLELINE;
	
	int nNewRowIdx = i_pGridCtrl->GetRowCount();
	i_pGridCtrl->SetRowCount(nNewRowIdx+1);
	
	//select된 값을 GridDetail GridCtrl에 넣어준다.
	Item.row		= nNewRowIdx;
	Item.col		= 0;
	Item.strText.Format("%d", nNewRowIdx);
	i_pGridCtrl->SetItem(&Item);
	
	Item.col		= GRID_COL_INDEX_ITEMNUM;
	Item.strText.Format("%d", i_pShopItem->ItemNum);
	i_pGridCtrl->SetItem(&Item);
	
	Item.col		= GRID_COL_INDEX_ITEMNAME;
	Item.strText.Format("%s", i_pShopItem->ItemName);
	i_pGridCtrl->SetItem(&Item);
	
	Item.mask		= GVIF_FORMAT|GVIF_IMAGE|GVIF_BKCLR;
	Item.col		= GRID_COL_INDEX_RECOMMAND_ITEM;
	Item.iImage		= IS_RECOMMEND_CASHSHOPITEM_BY_CASHSHOPINDEX(i_pShopItem->CashShopBit) ? 1 : 0;
	Item.crBkClr	= RGB(200, 200, 200);
	i_pGridCtrl->SetItem(&Item);

	Item.mask		= GVIF_FORMAT|GVIF_IMAGE|GVIF_BKCLR;
	Item.col		= GRID_COL_INDEX_NEW_ITEM;
	Item.iImage		= IS_NEW_CASHSHOPITEM_BY_CASHSHOPINDEX(i_pShopItem->CashShopBit) ? 1 : 0;
	Item.crBkClr	= RGB(200, 200, 200);
	i_pGridCtrl->SetItem(&Item);

	///////////////////////////////////////////////////////////////////////////////
	// 2010-01-26 by cmkwon, 캐쉬 아이템 한정판매 시스템 구현 - 
	Item.col		= GRID_COL_INDEX_LIMITED_EDITION;
	Item.mask		= GVIF_TEXT|GVIF_FORMAT;
	Item.nFormat	= DT_LEFT|DT_VCENTER|DT_SINGLELINE;
	Item.strText.Format("%d", i_pShopItem->RemainCountForLimitedEdition);
	i_pGridCtrl->SetItem(&Item);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CCashShopManagementDlg::ViewGrid(CGridCtrl *i_pGridCtrl)
/// \brief		// 2009-01-28 by cmkwon, 캐쉬샾 수정(추천탭,신상품 추가) - 
/// \author		cmkwon
/// \date		2009-01-29 ~ 2009-01-29
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CCashShopManagementDlg::ViewGrid(CGridCtrl *i_pGridCtrl)
{
	BOOL bIsChangedGrid = (m_GridChangedCashItem.GetDlgCtrlID() == i_pGridCtrl->GetDlgCtrlID());

	vectSAT_SHOP_ITEM::iterator itr(m_vectCurCashItemList.begin());
	for(; itr != m_vectCurCashItemList.end(); itr++)
	{
		SAT_SHOP_ITEM *pShopItem = &*itr;
		if(bIsChangedGrid == pShopItem->IsChangedItem)
		{
			_AddCashItem(i_pGridCtrl, pShopItem);
		}
	}
	i_pGridCtrl->UpdateData();
	i_pGridCtrl->AutoSize();
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			SAT_SHOP_ITEM *CCashShopManagementDlg::FindShopItem(int i_ItemNum)
/// \brief		// 2009-01-28 by cmkwon, 캐쉬샾 수정(추천탭,신상품 추가) - 
/// \author		cmkwon
/// \date		2009-01-29 ~ 2009-01-29
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
SAT_SHOP_ITEM *CCashShopManagementDlg::FindShopItem(int i_ItemNum)
{
	vectSAT_SHOP_ITEM::iterator itr(m_vectCurCashItemList.begin());
	for(; itr != m_vectCurCashItemList.end(); itr++)
	{
		SAT_SHOP_ITEM *pShopItem = &*itr;
		if(i_ItemNum == pShopItem->ItemNum)
		{
			return pShopItem;
		}
	}

	return NULL;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2009-01-28 by cmkwon, 캐쉬샾 수정(추천탭,신상품 추가) - 
/// \author		cmkwon
/// \date		2009-01-29 ~ 2009-01-29
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CCashShopManagementDlg::GetShopItemListFromGrid(vectSAT_SHOP_ITEM *o_pVectItemList, CGridCtrl *i_pGridCtrl)
{
	int nRowCnt = i_pGridCtrl->GetRowCount();
	if(1 >= nRowCnt)
	{
		return FALSE;
	}

	for(int i=1; i < nRowCnt; i++)
	{
		CGridCellBase *pItemNumGCell = m_GridChangedCashItem.GetCell(i, GRID_COL_INDEX_ITEMNUM);
		SAT_SHOP_ITEM *pSelShopItem = FindShopItem(atoi(pItemNumGCell->GetText()));
		if(NULL == pSelShopItem)
		{
			continue;
		}

		///////////////////////////////////////////////////////////////////////////////
		// 2010-01-26 by cmkwon, 캐쉬 아이템 한정판매 시스템 구현 - 수정된 제한 값을 가져 온다.
		CGridCellBase *pRemainCntGCell = m_GridChangedCashItem.GetCell(i, GRID_COL_INDEX_LIMITED_EDITION);
		if(NULL == pRemainCntGCell)
		{
			pSelShopItem->RemainCountForLimitedEdition	= UNLIMITED_REMAIN_COUNT_FOR_LIMITED_EDITION; 
		}
		else
		{
			pSelShopItem->RemainCountForLimitedEdition = atoi(pRemainCntGCell->GetText());
		}

		o_pVectItemList->push_back(*pSelShopItem);
	}
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2009-01-28 by cmkwon, 캐쉬샾 수정(추천탭,신상품 추가) - 
/// \author		cmkwon
/// \date		2009-01-29 ~ 2009-01-29
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CCashShopManagementDlg::DBQueryLoadCashItem(vectSAT_SHOP_ITEM *o_pVectItemList)
{
	/*******************************************************************************
	--------------------------------------------------------------------------------
	-- PROCEDURE NAME	: atum_Load_CashShopBit
	-- DESC				: // 2009-01-28 by cmkwon, 캐쉬샾 수정(추천탭,신상품 추가) - atum_Load_CashShopBit() 프로시저 추가, 추천탭,신상품 설정된 리스트 가져오기
	--						// 2010-01-26 by cmkwon, 캐쉬 아이템 한정판매 시스템 구현 - cs.RemainCountForLimitedEdition
	--------------------------------------------------------------------------------
	CALL dbo.atum_Load_CashShopBit
	*******************************************************************************/
	SQLHSTMT hstmt = m_odbcStmt2.GetSTMTHandle();
	BOOL bRet = m_odbcStmt2.ExecuteQuery(PROCEDURE_090129_0295);
	if (!bRet)
	{
		m_odbcStmt2.FreeStatement();		// cleanup		
		AfxMessageBox("DBQueryLoadCashItem error !!");
		return FALSE;
	}	
	SQLINTEGER arrCB[6]={SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS};
	SAT_SHOP_ITEM tmShopItem;
	SQLBindCol(hstmt,  1, SQL_C_LONG, &tmShopItem.ItemNum, 0,					&arrCB[1]);
	SQLBindCol(hstmt,  2, SQL_C_CHAR, &tmShopItem.ItemName, SIZE_MAX_ITEM_NAME,	&arrCB[2]);
	SQLBindCol(hstmt,  3, SQL_C_LONG, &tmShopItem.CashShopTapIndex, 0,			&arrCB[3]);
	SQLBindCol(hstmt,  4, SQL_C_TINYINT, &tmShopItem.CashShopBit, 0,			&arrCB[4]);
	SQLBindCol(hstmt,  5, SQL_C_LONG, &tmShopItem.RemainCountForLimitedEdition, 0,	&arrCB[5]);		// 2010-01-26 by cmkwon, 캐쉬 아이템 한정판매 시스템 구현 - 
	
	do
	{
		util::zero(&tmShopItem, sizeof(tmShopItem));
		tmShopItem.RemainCountForLimitedEdition		= UNLIMITED_REMAIN_COUNT_FOR_LIMITED_EDITION;	// 2010-01-26 by cmkwon, 캐쉬 아이템 한정판매 시스템 구현 - 초기값 설정 해야함.

		SQLRETURN ret = SQLFetch(hstmt);
		if(SQL_ERROR == ret)
		{
			m_odbcStmt2.ProcessLogMessagesForStmt(PROCEDURE_090129_0295);
		}
		if(SQL_SUCCESS != ret && SQL_SUCCESS_WITH_INFO != ret)
		{
			break;
		}
		tmShopItem.OriginalCashShopBit	= tmShopItem.CashShopBit;	// 2009-01-29 by cmkwon, 원래 데이터 저장
		ITEM *pItemInfo = m_mapItemInfo.findEZ_ptr(tmShopItem.ItemNum);
		if(pItemInfo)
		{// 2006-03-27 by cmkwon, 아이템 이름 현지화 처리
			util::strncpy(tmShopItem.ItemName, pItemInfo->ItemName, SIZE_MAX_ITEM_NAME);
		}
		o_pVectItemList->push_back(tmShopItem);
	}while(TRUE);
	m_odbcStmt2.FreeStatement();		// cleanup

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2009-01-28 by cmkwon, 캐쉬샾 수정(추천탭,신상품 추가) - 
/// \author		cmkwon
/// \date		2009-01-29 ~ 2009-01-29
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CCashShopManagementDlg::DBQueryUpdateCashItem(vectSAT_SHOP_ITEM *i_pVectItemList)
{
	/*******************************************************************************
	--------------------------------------------------------------------------------
	-- PROCEDURE NAME	: atum_Update_CashShopBit
	-- DESC				: // 2009-01-28 by cmkwon, 캐쉬샾 수정(추천탭,신상품 추가) - atum_Update_CashShopBit() 프로시저 추가, 추천탭,신상품 설정된 리스트 가져오기
	--						// 2010-01-26 by cmkwon, 캐쉬 아이템 한정판매 시스템 구현 - 
	--------------------------------------------------------------------------------
	CALL dbo.atum_Update_CashShopBit
	*******************************************************************************/

	SQLHSTMT hstmt = m_odbcStmt2.GetSTMTHandle();
	
	vectSAT_SHOP_ITEM::iterator itr(i_pVectItemList->begin());
	for (; itr != i_pVectItemList->end(); itr++)
	{
		SAT_SHOP_ITEM *pShopItem = &*itr;
		
		SQLINTEGER arrCB[4]={SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS};
		SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pShopItem->ItemNum, 0,			&arrCB[1]);
		SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_TINYINT, SQL_TINYINT, 0, 0, &pShopItem->CashShopBit, 0,	&arrCB[2]);
		SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pShopItem->RemainCountForLimitedEdition, 0,	&arrCB[3]);		// 2010-01-26 by cmkwon, 캐쉬 아이템 한정판매 시스템 구현 - 
		BOOL bRet = m_odbcStmt2.ExecuteQuery(PROCEDURE_090129_0296);
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

struct sort_SAT_SHOP_ITEM_by_TapIdx
{
	bool operator()(SAT_SHOP_ITEM op1, SAT_SHOP_ITEM op2)
	{
		return op1.CashShopTapIndex < op2.CashShopTapIndex;		// 오름차순 정렬
	}
};

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2009-01-28 by cmkwon, 캐쉬샾 수정(추천탭,신상품 추가) - 
/// \author		cmkwon
/// \date		2009-01-29 ~ 2009-01-29
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CCashShopManagementDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	// Connect DB
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

	CAtumDBHelper::LoadItemInfo(&m_odbcStmt2, &m_mapItemInfo);

	// 2009-01-29 by cmkwon, DB에서 캐쉬 상점 아이템 정보 가져오기
	m_vectCurCashItemList.clear();
	this->DBQueryLoadCashItem(&m_vectCurCashItemList);

	// 2009-01-29 by cmkwon, 탭인덱스로 정렬
	sort(m_vectCurCashItemList.begin(), m_vectCurCashItemList.end(), sort_SAT_SHOP_ITEM_by_TapIdx());

	this->InitGridCurItem8GridChangedItem();	
	this->ViewGrid(&m_GridCurCashItem);
	this->ViewGrid(&m_GridChangedCashItem);

	// 2013-02-05 by bckim, 캐시샵 추천탭 정렬순서부여
	GetDlgItem(IDC_BTN_REALIGNMENT_CASH_ITEM)->EnableWindow(FALSE);

#ifdef	S_ADMINTOOL_CASHSHOP_REALIGNMENT_BCKIM	// 2013-02-05 by bckim, 캐시샵 추천탭 정렬순서부여
	GetDlgItem(IDC_BTN_REALIGNMENT_CASH_ITEM)->EnableWindow(TRUE);
#endif

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2009-01-28 by cmkwon, 캐쉬샾 수정(추천탭,신상품 추가) - 
/// \author		cmkwon
/// \date		2009-01-29 ~ 2009-01-29
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CCashShopManagementDlg::OnBtnDeleteFromChanged() 
{
	// TODO: Add your control notification handler code here

	CCellID tmpCellID = m_GridChangedCashItem.GetFocusCell();
	if(FALSE == tmpCellID.IsValid())
	{
		return;
	}
	
	CGridCellBase *pItemNumGCell = m_GridChangedCashItem.GetCell(tmpCellID.row, GRID_COL_INDEX_ITEMNUM);
	SAT_SHOP_ITEM *pSelShopItem = FindShopItem(atoi(pItemNumGCell->GetText()));
	if(NULL == pSelShopItem)
	{
		return;
	}

	// 2009-01-29 by cmkwon, 원래 데이터로 설정한다.
	pSelShopItem->IsChangedItem	= FALSE;
	pSelShopItem->CashShopBit	= pSelShopItem->OriginalCashShopBit;

	this->InitGridCurItem8GridChangedItem();
	this->ViewGrid(&m_GridCurCashItem);
	this->ViewGrid(&m_GridChangedCashItem);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2009-01-28 by cmkwon, 캐쉬샾 수정(추천탭,신상품 추가) - 
/// \author		cmkwon
/// \date		2009-01-29 ~ 2009-01-29
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CCashShopManagementDlg::OnBtnUpdateToDb() 
{
	// TODO: Add your control notification handler code here
	
	vectSAT_SHOP_ITEM tmShopItemList;
	if(FALSE == this->GetShopItemListFromGrid(&tmShopItemList, &m_GridChangedCashItem))
	{
		return;
	}
	
	this->DBQueryUpdateCashItem(&tmShopItemList);

	// 2009-01-29 by cmkwon, DB에서 캐쉬 상점 아이템 정보 가져오기
	m_vectCurCashItemList.clear();
	this->DBQueryLoadCashItem(&m_vectCurCashItemList);
	
	// 2009-01-29 by cmkwon, 탭인덱스로 정렬
	sort(m_vectCurCashItemList.begin(), m_vectCurCashItemList.end(), sort_SAT_SHOP_ITEM_by_TapIdx());
	
	this->InitGridCurItem8GridChangedItem();	
	this->ViewGrid(&m_GridCurCashItem);
	this->ViewGrid(&m_GridChangedCashItem);

	AfxMessageBox("Update to DB success !!");
}

// 2013-02-05 by bckim, 캐시샵 추천탭 정렬순서부여
void CCashShopManagementDlg::OnBtnRealignmentCashItem() 
{
	// TODO: Add your control notification handler code here
	
	if (m_pCRealignmentCashItemDlg != NULL)
	{
		util::del(m_pCRealignmentCashItemDlg);
	}
	
	if (m_pCRealignmentCashItemDlg == NULL)
	{
		m_pCRealignmentCashItemDlg = new CRealignmentCashItemDlg(this);

		m_pCRealignmentCashItemDlg->DoModal();
// 		if(FALSE == m_pCRealignmentCashItemDlg->Create(IDD_DLG_REALIGNMENT_CASH_ITEM, this))
// 		{
// 			return;
// 		}
	}
	//m_pCRealignmentCashItemDlg->ShowWindow(SW_SHOW);
}
// End. 2013-02-05 by bckim, 캐시샵 추천탭 정렬순서부여