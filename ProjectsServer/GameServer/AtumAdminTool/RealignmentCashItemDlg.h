#if !defined(AFX_REALIGNMENTCASHITEMDLG_H__323EC069_BD75_4EF2_9BF3_9EF9909660BC__INCLUDED_)
#define AFX_REALIGNMENTCASHITEMDLG_H__323EC069_BD75_4EF2_9BF3_9EF9909660BC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RealignmentCashItemDlg.h : header file
//

#include "SCGridHelper.h"		// 2009-01-28 by cmkwon, 캐쉬샾 수정(추천탭,신상품 추가) - 
#include "ODBCStatement.h"		// 2009-01-28 by cmkwon, 캐쉬샾 수정(추천탭,신상품 추가) - 

#define GRID_COL_REINDEX_ITEMNUM				1
#define GRID_COL_REINDEX_ITEMNAME				2
#define GRID_COL_REINDEX_RECOMMAND_ITEM			3
#define GRID_COL_REINDEX_NEW_ITEM				4
#define GRID_COL_REINDEX_LIMITED_EDITION		5
#define GRID_COL_REINDEX_PREVIOUSE_INDEX		6


struct SAT_SHOP_REALIGN_ITEM
{
	INT		ItemNum;
	char	ItemName[SIZE_MAX_ITEM_NAME];
	BYTE	CashShopBit;
	BYTE	OriginalCashShopBit;
	INT		TempRows;
	INT		RemainCountForLimitedEdition;		
	INT		RealignmentIndex;
	INT		PreviousIndex;
};
typedef vector<SAT_SHOP_REALIGN_ITEM>		vectSAT_SHOP_REALIGN_ITEM;

struct sort_SAT_SHOP_REALIGN_ITEM_by_Realign
{
	bool operator()(SAT_SHOP_REALIGN_ITEM op1, SAT_SHOP_REALIGN_ITEM op2)
	{
		return op1.RealignmentIndex < op2.RealignmentIndex;		// 오름차순 정렬
		//return op1.TempRows < op2.TempRows;	
	}
};

/////////////////////////////////////////////////////////////////////////////
// CRealignmentCashItemDlg dialog
class CAtumAdminToolDlg;
class CRealignmentCashItemDlg : public CDialog
{
// Construction
public:
	CRealignmentCashItemDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CRealignmentCashItemDlg();
	

// Dialog Data
	//{{AFX_DATA(CRealignmentCashItemDlg)
	enum { IDD = IDD_DLG_REALIGNMENT_CASH_ITEM };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRealignmentCashItemDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL
	afx_msg void OnGridCurRealignCashItem(NMHDR *pNotifyStruct, LRESULT* pResult);


// Implementation

	SAT_SHOP_REALIGN_ITEM*			m_pTargetItem;
	
	void _InitGrid(CGridCtrl *i_pGridCtrl);
	void InitGridCurRealignCashItem(void);
	INT _AddCashItem(CGridCtrl *i_pGridCtrl, SAT_SHOP_REALIGN_ITEM *i_pShopItem);
	void ViewGrid(CGridCtrl *i_pGridCtrl);

	SAT_SHOP_REALIGN_ITEM *FindShopItem(int i_ItemNum);
	BOOL DBQueryLoadCashRealignItem(vectSAT_SHOP_REALIGN_ITEM *o_pVectItemList);
	BOOL DBQueryUpdateCashRealignItem(vectSAT_SHOP_REALIGN_ITEM *i_pVectItemList);

protected:

	NM_GRIDVIEW*					m_ptempItem;

	CGridCtrl						m_GridCurRealignCashItem;
	CImageList						m_ilCheckImageList;	

	vectSAT_SHOP_REALIGN_ITEM		m_vectCurRealignCashItemList;

	CODBCStatement			m_odbcStmt2;
	CAtumAdminToolDlg *		m_pMainDlg;

	// Generated message map functions
	//{{AFX_MSG(CRealignmentCashItemDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnRealignmentUpdateToDB();
	//afx_msg void OnUpRealignmentSpin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonUP();
	afx_msg void OnUp();
	afx_msg void OnDown();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REALIGNMENTCASHITEMDLG_H__323EC069_BD75_4EF2_9BF3_9EF9909660BC__INCLUDED_)
