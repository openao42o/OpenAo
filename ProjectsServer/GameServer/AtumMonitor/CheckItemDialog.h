#if !defined(AFX_CHECKITEMDIALOG_H__63C9ACD7_DA90_478C_B244_816E8DD64E61__INCLUDED_)
#define AFX_CHECKITEMDIALOG_H__63C9ACD7_DA90_478C_B244_816E8DD64E61__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CheckItemDialog.h : header file
//
#include "GridCtrl/GridCtrl.h"
#include "CheckDBData.h"
/////////////////////////////////////////////////////////////////////////////
// CCheckItemDialog dialog

class CCheckItemDialog : public CDialog
{
// Construction
public:
	CCheckItemDialog(CWnd* pParent = NULL);   // standard constructor
	void InitLists(CGridCtrl* i_CheckItemTables, CImageList* i_ImageTables);
	void ViewLists(CGridCtrl* i_ItemTables, int i_Flag); // 플래그 값 FLAG VALUES = CHECK_DATA, VIEW_OLD_DATA, VIEW_NEW_DATA
	BOOL InsertItemDataList( BOOL Flag, vectCashCheckData* i_vectCheckData );	//리스트 박스에 출력할 데이터를 삽입하는 함수
// Dialog Data
	//{{AFX_DATA(CCheckItemDialog)
	enum { IDD = IDD_CHECKITEMDIALOG };
	CGridCtrl	m_CheckItemTables;	// Old Items
	CGridCtrl	m_CheckItemTables1;	// New Items
	CImageList	m_ilCheckItemTables1;	// Old View
	CImageList	m_ilCheckItemTables2;	// New view
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCheckItemDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCheckItemDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public: // 변수 정의
	CCheckDBData m_CheckDBData;

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHECKITEMDIALOG_H__63C9ACD7_DA90_478C_B244_816E8DD64E61__INCLUDED_)
