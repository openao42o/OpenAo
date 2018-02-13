#if !defined(AFX_WEAPONITEM_H__8866154A_4664_48D2_B830_B2C6AD80DD4E__INCLUDED_)
#define AFX_WEAPONITEM_H__8866154A_4664_48D2_B830_B2C6AD80DD4E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WeaponItem.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CWeaponItem dialog
#include "resource.h"
#include "stdafx.h"
#include "GridCtrl/GridCtrl.h" //For GridCtrl
#include "ODBCStatement.h"
#include "AtumProtocol.h"
#include "AtumSJ.h"

class CSpaceCowboyDBEditorToolDlg;

class CWeaponItem : public CDialog
{
// Construction
public:
	void SetDetailDialog(int row);
	void ChangeGridDate(CGridCtrl* oriGrid, CGridCtrl* newGrid, int type, int row);
	void InsertItemToGridCurrent(_TI_ITEM *i_pTi_Item, int i_nRow);
	void GetCurrentItemList();
	CWeaponItem(CWnd* pParent = NULL);   // standard constructor
	CWeaponItem::~CWeaponItem();

// Dialog Data
	//{{AFX_DATA(CWeaponItem)
	enum { IDD = IDD_DIALOG_WEAPON_ITEM };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	CGridCtrl		m_GridCurrent;
	CGridCtrl		m_GridUpdate;
	CODBCStatement					*m_pODBCStmt; //DB 접근 클래스 포인터
	CSpaceCowboyDBEditorToolDlg				*m_pMainDlg; 
	CWeaponItemDetail*				m_pWeaponItemDetailDlg;
	

	void InitGridCurrent(); // GridTotal GridCtrl의 초기화
	void InitGridUpdate(); // GridTotal GridCtrl의 초기화
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWeaponItem)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CWeaponItem)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonInsert();
	afx_msg void OnButtonUpdate();
	afx_msg void OnButtonDelete();
	afx_msg void OnButtonRefresh();
	afx_msg void OnButtonCancel();
	afx_msg void OnButtonView();
	afx_msg void OnButtonServerupdate();
	afx_msg void OnButtonEnd();
	afx_msg void OnButtonRefreshupdate();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WEAPONITEM_H__8866154A_4664_48D2_B830_B2C6AD80DD4E__INCLUDED_)
