// CheckItemDialog.cpp : implementation file
//

#include "stdafx.h"
#include "atummonitor.h"
#include "CheckItemDialog.h"
#include "GridCtrl/GridCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCheckItemDialog dialog


CCheckItemDialog::CCheckItemDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CCheckItemDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCheckItemDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

void CCheckItemDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCheckItemDialog)
	DDX_GridControl(pDX, IDD_CHECKITEMDIALOG, m_CheckItemTables);
	DDX_GridControl(pDX, IDD_CHECKITEMDIALOG2, m_CheckItemTables1);

	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCheckItemDialog, CDialog)
//{{AFX_MSG_MAP(CCheckItemDialog)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCheckItemDialog message handlers

// void CCheckItemDialog::OnClickNewdatalist(NMHDR* pNMHDR, LRESULT* pResult) 
// {
// 	// TODO: Add your control notification handler code here
// 	
// 	*pResult = 0;
// }
// 
// void CCheckItemDialog::OnClickOlddatalist(NMHDR* pNMHDR, LRESULT* pResult) 
// {
// 	// TODO: Add your control notification handler code here
// 	
// 	*pResult = 0;
// }

void CCheckItemDialog::InitLists(CGridCtrl* i_CheckItemTables, CImageList* i_ImageTables)
{
//	CGridCtrl	m_CheckItemTables;
//	CImageList	m_ilCheckItemTables2;
	i_ImageTables->DeleteImageList();
	i_ImageTables->Create(24, 12, ILC_COLOR32, 2, 2);
	
	
	CBitmap bmCheckImage;
	bmCheckImage.LoadBitmap(IDB_BITMAP_CHECK_IMAGE);
	i_ImageTables->Add(&bmCheckImage, RGB(0,0,0));
	i_CheckItemTables->SetImageList(i_ImageTables);
	
	i_CheckItemTables->SetBkColor(0xFFFFFF);
	
	int m_nRows = 1;
	int m_nCols = 6;
	
	i_CheckItemTables->SetEditable(FALSE);
	i_CheckItemTables->SetListMode(TRUE);
	
	i_CheckItemTables->EnableSelection(TRUE);
	i_CheckItemTables->SetFrameFocusCell(FALSE);
	i_CheckItemTables->SetTrackFocusCell(FALSE);
	
	i_CheckItemTables->SetRowCount(m_nRows);
	i_CheckItemTables->SetColumnCount(m_nCols);
	i_CheckItemTables->SetFixedRowCount(1);
	
	// 칼럼 만들기
	m_nCols = 0;
	GV_ITEM Item;
	Item.mask = GVIF_TEXT|GVIF_FORMAT;
	Item.row = 0;
	Item.nFormat = GRID_CELL_FORMAT;
	
	Item.col = m_nCols++;
	Item.strText.Format("Item_Number");
	i_CheckItemTables->SetItem(&Item);
	
	Item.col = m_nCols++;
	Item.strText.Format("Cash Price");
	i_CheckItemTables->SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("Price");
	i_CheckItemTables->SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("ItemAttribute");
	i_CheckItemTables->SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("Time");
	i_CheckItemTables->SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("UniqueNumber");
	i_CheckItemTables->SetItem(&Item);
	
	// arrange grid
	i_CheckItemTables->AutoSize();
	i_CheckItemTables->ExpandColumnsToFit();
	
	// clean all cells
	CCellRange tmpCellRange(1, 0, i_CheckItemTables->GetRowCount()-1, i_CheckItemTables->GetColumnCount()-1);
	i_CheckItemTables->ClearCells(tmpCellRange);

}

void CCheckItemDialog::ViewLists(CGridCtrl* i_ItemTables, int i_Flag /* FLAG VALUES = CHECK_DATA, VIEW_OLD_DATA, VIEW_NEW_DATA */)
{
	GV_ITEM Item;
	
	int nSize = m_CheckDBData.GetData(i_Flag)->size();
	
	cashItemCheckData* pTable;
	vectCashCheckData* ViewOldData = m_CheckDBData.GetData(i_Flag);
	vectCashCheckData::iterator itr(ViewOldData->begin());

	for(; itr != ViewOldData->end(); itr++) 
	{
		pTable = &*itr;

// 		pTable->ItemNumber			= itr->ItemNumber;
// 		pTable->CashPrice			= itr->CashPrice;
// 		pTable->Price				= itr->Price;
// 		pTable->ItemAttribute		= itr->ItemAttribute;
// 		pTable->Time				= itr->Time;
// 		pTable->UniqueNumber		= itr->UniqueNumber;

		Item.mask	= GVIF_TEXT|GVIF_FORMAT;
		Item.nFormat	= DT_LEFT|DT_VCENTER|DT_SINGLELINE;
		
		int nNewRowIdx = i_ItemTables->GetRowCount();
		i_ItemTables->SetRowCount(nNewRowIdx+1);
		
		//select된 값을 GridDetail GridCtrl에 넣어준다.
		Item.row		= nNewRowIdx;
		Item.col		= 0;
		Item.strText.Format("%d", pTable->ItemNumber);
		Item.strText.Remove('$');
		i_ItemTables->SetItem(&Item);
		
		Item.col++;
		Item.strText.Format("%d", pTable->CashPrice);
		i_ItemTables->SetItem(&Item);
		
		Item.col++;
		Item.strText.Format("%d", pTable->Price);
		i_ItemTables->SetItem(&Item);
		
		Item.col++;
		Item.strText.Format("%d", pTable->ItemAttribute);
		i_ItemTables->SetItem(&Item);
		
		Item.col++;
		Item.strText.Format("%d", pTable->Time);
		i_ItemTables->SetItem(&Item);
		
		Item.col++;
		Item.strText.Format("%d", pTable->UniqueNumber);
		i_ItemTables->SetItem(&Item);

	}
	
	i_ItemTables->UpdateData();
}

// 리스트에 출력할 데이터 삽입을 위한 함수
// BOOL CCheckItemDialog::InsertItemDataList( BOOL Flag, vectCashCheckData* i_vectCheckData ) // 플래그로 예전 데이터 삽입과 신규데이터 삽입을 판단한다.
// {
// 	int icnt = 0;
// 	CString	str[7];
// 	vectCashCheckData::iterator iter(i_vectCheckData->begin());
// 	if(OLD_DATA==Flag)
// 	{	// 옛날 데이터다
// 
// 		
// 		for( ; iter!=i_vectCheckData->end() ; iter++)
// 		{
// 			str[0].Format("%d",icnt);
// 			str[1].Format("%d",iter->ItemNumber);
// 			str[2].Format("%d",iter->CashPrice);
// 			str[3].Format("%d",iter->Price);
// 			str[4].Format("%d",iter->ItemAttribute);
// 			str[5].Format("%d",iter->Time);
// 			str[6].Format("%d",iter->UniqueNumber);
// 			
// 			m_OldDataList.InsertItem(icnt, str[0] );
// 			m_OldDataList.SetItemText(icnt,0,str[1]);
// 			m_OldDataList.SetItemText(icnt,1,str[2]);
// 			m_OldDataList.SetItemText(icnt,2,str[3]);
// 			m_OldDataList.SetItemText(icnt,3,str[4]);
// 			m_OldDataList.SetItemText(icnt,4,str[5]);
// 			m_OldDataList.SetItemText(icnt,5,str[6]);
// 			// 증가
// 			icnt++;
// 		}
// 	}
// 	if(NEW_DATA==Flag)
// 	{	// 신규 데이터다
// 		
// 		for( ; iter!=i_vectCheckData->end() ; iter++)
// 		{
// 			str[0].Format("%d",icnt);
// 			str[1].Format("%d",iter->ItemNumber);
// 			str[2].Format("%d",iter->CashPrice);
// 			str[3].Format("%d",iter->Price);
// 			str[4].Format("%d",iter->ItemAttribute);
// 			str[5].Format("%d",iter->Time);
// 			str[6].Format("%d",iter->UniqueNumber);
// 			
// 			m_NewDataList.InsertItem(icnt, str[0] );
// 			m_NewDataList.SetItemText(icnt,0,str[1]);
// 			m_NewDataList.SetItemText(icnt,1,str[2]);
// 			m_NewDataList.SetItemText(icnt,2,str[3]);
// 			m_NewDataList.SetItemText(icnt,3,str[4]);
// 			m_NewDataList.SetItemText(icnt,4,str[5]);
// 			m_NewDataList.SetItemText(icnt,5,str[6]);
// 			// 증가
// 			icnt++;
// 		}
// 	}
// 	
// 	return TRUE;
// }

BOOL CCheckItemDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	// Old
	this->InitLists(&m_CheckItemTables, &m_ilCheckItemTables1);
	this->ViewLists(&m_CheckItemTables, VIEW_OLD_DATA);

	//New
	this->InitLists(&m_CheckItemTables1,&m_ilCheckItemTables2);
	this->ViewLists(&m_CheckItemTables1, VIEW_NEW_DATA);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCheckItemDialog::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnClose();
}
