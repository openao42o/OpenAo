#if !defined(AFX_BLOCKACCOUNTLISTDLG_H__611049B3_978F_48B3_A293_D4BADD51179D__INCLUDED_)
#define AFX_BLOCKACCOUNTLISTDLG_H__611049B3_978F_48B3_A293_D4BADD51179D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BlockAccountListDlg.h : header file
//
#include "GridCtrl/GridCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CBlockAccountListDlg dialog
class CODBCStatement;
class CBlockAccountListDlg : public CDialog
{
// Construction
public:
	CBlockAccountListDlg(CODBCStatement *i_pODBCStat, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CBlockAccountListDlg)
	enum { IDD = IDD_DLG_BLOCK_ACCOUNT_LIST };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBlockAccountListDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL


	CGridCtrl						m_GridBlockedList;
	CODBCStatement					*m_pODBCStmt1;

	void InitGridBlockedList(void);
	void InsertBlockedUser(SBLOCKED_ACCOUNT_INFO *pBlockedUser);


// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CBlockAccountListDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnReloadAccountBlockedList();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BLOCKACCOUNTLISTDLG_H__611049B3_978F_48B3_A293_D4BADD51179D__INCLUDED_)
