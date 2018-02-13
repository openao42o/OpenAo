#if !defined(AFX_CASHSHOPMANAGEMENTDLG_H__33066C39_CBB9_40F3_9121_533CE72FA6FA__INCLUDED_)
#define AFX_CASHSHOPMANAGEMENTDLG_H__33066C39_CBB9_40F3_9121_533CE72FA6FA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// cashshopmanagementdlg.h : header file
//
#include "SCGridHelper.h"		// 2009-01-28 by cmkwon, 캐쉬샾 수정(추천탭,신상품 추가) - 
#include "ODBCStatement.h"		// 2009-01-28 by cmkwon, 캐쉬샾 수정(추천탭,신상품 추가) - 
#include "RealignmentCashItemDlg.h"	// 2013-02-05 by bckim, 캐시샵 추천탭 정렬순서부여

#define GRID_COL_INDEX_ITEMNUM				1
#define GRID_COL_INDEX_ITEMNAME				2
#define GRID_COL_INDEX_RECOMMAND_ITEM		3
#define GRID_COL_INDEX_NEW_ITEM				4
#define GRID_COL_INDEX_LIMITED_EDITION		5		// 2010-01-26 by cmkwon, 캐쉬 아이템 한정판매 시스템 구현 - 


struct SAT_SHOP_ITEM
{
	INT		ItemNum;
	char	ItemName[SIZE_MAX_ITEM_NAME];
	INT		CashShopTapIndex;
	BYTE	CashShopBit;
	BYTE	OriginalCashShopBit;
	BOOL	IsChangedItem;
	INT		RemainCountForLimitedEdition;		// 2010-01-26 by cmkwon, 캐쉬 아이템 한정판매 시스템 구현 - 
};
typedef vector<SAT_SHOP_ITEM>		vectSAT_SHOP_ITEM;

/////////////////////////////////////////////////////////////////////////////
// CCashShopManagementDlg dialog
class CAtumAdminToolDlg;			// 2009-01-28 by cmkwon, 캐쉬샾 수정(추천탭,신상품 추가) - 
class CCashShopManagementDlg : public CDialog
{
// Construction
public:
	CCashShopManagementDlg(CWnd* pParent = NULL);   // standard constructor
	// 2013-02-05 by bckim, 캐시샵 추천탭 정렬순서부여
	virtual ~CCashShopManagementDlg();
	// End. 2013-02-05 by bckim, 캐시샵 추천탭 정렬순서부여
	

// Dialog Data
	//{{AFX_DATA(CCashShopManagementDlg)
	enum { IDD = IDD_DLG_CASHSHOP_MANAGEMENT };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// 2013-02-05 by bckim, 캐시샵 추천탭 정렬순서부여
	CRealignmentCashItemDlg*		m_pCRealignmentCashItemDlg;
	// End. 2013-02-05 by bckim, 캐시샵 추천탭 정렬순서부여

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCashShopManagementDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	afx_msg void OnGridCurItem(NMHDR *pNotifyStruct, LRESULT* pResult);
	afx_msg void OnGridChangedItem(NMHDR *pNotifyStruct, LRESULT* pResult);


// Implementation
	void _InitGrid(CGridCtrl *i_pGridCtrl);
	void InitGridCurItem8GridChangedItem(void);
	void _AddCashItem(CGridCtrl *i_pGridCtrl, SAT_SHOP_ITEM *i_pShopItem);
	void ViewGrid(CGridCtrl *i_pGridCtrl);
	SAT_SHOP_ITEM *FindShopItem(int i_ItemNum);
	BOOL GetShopItemListFromGrid(vectSAT_SHOP_ITEM *o_pVectItemList, CGridCtrl *i_pGridCtrl);

	BOOL DBQueryLoadCashItem(vectSAT_SHOP_ITEM *o_pVectItemList);
	BOOL DBQueryUpdateCashItem(vectSAT_SHOP_ITEM *i_pVectItemList);

protected:

	CGridCtrl				m_GridCurCashItem;		// 2009-01-28 by cmkwon, 캐쉬샾 수정(추천탭,신상품 추가) - 
	CGridCtrl				m_GridChangedCashItem;	// 2009-01-28 by cmkwon, 캐쉬샾 수정(추천탭,신상품 추가) - 
	CImageList				m_ilCheckImageList;		// 2009-01-28 by cmkwon, 캐쉬샾 수정(추천탭,신상품 추가) - 
	vectSAT_SHOP_ITEM		m_vectCurCashItemList;	

	CODBCStatement			m_odbcStmt2;
	CAtumAdminToolDlg *		m_pMainDlg;
	ez_map<INT, ITEM>		m_mapItemInfo;


	// Generated message map functions
	//{{AFX_MSG(CCashShopManagementDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnDeleteFromChanged();
	afx_msg void OnBtnUpdateToDb();
	afx_msg void OnBtnRealignmentCashItem();	// 2013-02-05 by bckim, 캐시샵 추천탭 정렬순서부여
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CASHSHOPMANAGEMENTDLG_H__33066C39_CBB9_40F3_9121_533CE72FA6FA__INCLUDED_)
