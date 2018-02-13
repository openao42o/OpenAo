#if !defined(AFX_ENGINEEDITORDLG_H__52FF2E27_648B_4A76_A515_7C8D23A3B4A1__INCLUDED_)
#define AFX_ENGINEEDITORDLG_H__52FF2E27_648B_4A76_A515_7C8D23A3B4A1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EngineEditorDlg.h : header file
//
#include "resource.h"
#include "stdafx.h"
#include "GridCtrl/GridCtrl.h" //For GridCtrl
#include "ODBCStatement.h"
#include "AtumProtocol.h"
#include "AtumSJ.h"

/////////////////////////////////////////////////////////////////////////////
// CEngineEditorDlg dialog
class CSpaceCowboyDBEditorToolDlg;

class CEngineItemDlg : public CDialog
{
// Construction
public:
	void SetDetailDialog(int row);
	void ChangeGridDate(CGridCtrl* oriGrid, CGridCtrl* newGrid, int type, int row);
	void InsertItemToGridCurrent(_TI_ITEM *i_pTi_Item, int i_nRow);
	void GetCurrentItemList();
	CEngineItemDlg(CWnd* pParent = NULL);   // standard constructor
	CEngineItemDlg::~CEngineItemDlg();

	enum { IDD = IDD_DIALOG_ENGINE_ITEM };

	CGridCtrl		m_GridCurrent;
	CGridCtrl		m_GridUpdate;
	CODBCStatement					*m_pODBCStmt; //DB 접근 클래스 포인터
	CSpaceCowboyDBEditorToolDlg				*m_pMainDlg;
	CEngineItemDetailDlg*				m_pEngineItemDetailDlg;

	void InitGridCurrent(); // GridTotal GridCtrl의 초기화
	void InitGridUpdate(); // GridTotal GridCtrl의 초기화

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEngineEditorDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CEngineEditorDlg)
	afx_msg void OnButtonView();
	afx_msg void OnButtonInsert();
	afx_msg void OnButtonUpdate();
	afx_msg void OnButtonDelete();
	afx_msg void OnButtonRefresh();
	afx_msg void OnButtonCancel();
	afx_msg void OnButtonRefreshupdate();
	afx_msg void OnButtonServerupdate();
	afx_msg void OnButtonEnd();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ENGINEEDITORDLG_H__52FF2E27_648B_4A76_A515_7C8D23A3B4A1__INCLUDED_)
