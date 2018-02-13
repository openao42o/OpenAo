#if !defined(AFX_SCBADUSERADMINDLG_H__0B5322CE_88D6_43F6_8054_94B320FBDA22__INCLUDED_)
#define AFX_SCBADUSERADMINDLG_H__0B5322CE_88D6_43F6_8054_94B320FBDA22__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SCBadUserAdminDlg.h : header file
//

class CAtumAdminToolDlg;

#include "ODBCStatement.h"
#include "GridCtrl/GridCtrl.h"
#include "AtumParam.h"
#include "SCGridHelper.h"

/////////////////////////////////////////////////////////////////////////////
// CSCBadUserAdminDlg dialog

class CSCBadUserAdminDlg : public CDialog
{
// Construction
public:
	CSCBadUserAdminDlg(CWnd* pParent = NULL);   // standard constructor
	~CSCBadUserAdminDlg();

// Dialog Data
	//{{AFX_DATA(CSCBadUserAdminDlg)
	enum { IDD = IDD_DIALOG_BAD_USER_ADMIN_TOOL };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSCBadUserAdminDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	BOOL GetBadUsers();

public:
	CSCGridCtrl			m_GridBadUser;

	CAtumAdminToolDlg	*m_pMainDlg;
	CODBCStatement		*m_pODBCStmt;

protected:

	// Generated message map functions
	//{{AFX_MSG(CSCBadUserAdminDlg)
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonLoadData();
	afx_msg void OnButtonRefresh();
	afx_msg void OnButtonBlockChatting();
	afx_msg void OnButtonBlockAccount();
	afx_msg void OnCmgDeleteBlockedUser();
	afx_msg void OnCmgUnblock();
	afx_msg void OnCmgBlockAccount();
	afx_msg void OnCmgBlockChatting();
	afx_msg void OnButtonBlockUser();
	//}}AFX_MSG
    afx_msg void OnGridRClick(NMHDR *pNotifyStruct, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCBADUSERADMINDLG_H__0B5322CE_88D6_43F6_8054_94B320FBDA22__INCLUDED_)
