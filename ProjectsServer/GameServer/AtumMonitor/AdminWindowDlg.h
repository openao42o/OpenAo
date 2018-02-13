#if !defined(AFX_ADMINWINDOWDLG_H__15E7A766_46F9_44E2_8945_9BB4100892E9__INCLUDED_)
#define AFX_ADMINWINDOWDLG_H__15E7A766_46F9_44E2_8945_9BB4100892E9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AdminWindowDlg.h : header file
//

#include <SQLTYPES.H>
#include "CommandHistory.h"
#include "ODBCStatement.h"
#include "SCServerDBConnDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CAdminWindowDlg dialog

class CAdminWindowDlg : public CDialog
{
// Construction
public:
	CAdminWindowDlg(CWnd* pParent = NULL);   // standard constructor
	~CAdminWindowDlg();

// Dialog Data
	//{{AFX_DATA(CAdminWindowDlg)
	enum { IDD = IDD_DIALOG_ADMIN_COMMAND };
	CEdit	m_ctrlCmdOut;
	CString	m_szAdminCommand;
	CString	m_szDbgOutBin;
	CString	m_szCmdOut;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAdminWindowDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
	BOOL HandleAdminCommand(const char* cmd);
	BOOL ChangeUserPassword(CString szAccount, CString szPassword);
	BOOL RecoverUserPassword(CString szAccount);
	BOOL PrintAllBackupAccounts();
	void printf2(char* pFormat, ...);

	BOOL BlockAccount(CString szAccount);
	BOOL UnblockAccount(CString szAccount);
	BOOL ListupBlockedAccount();

	CODBCStatement m_odbcStmt;

	CCommandHistory	*m_pCmdHist;
	CSCServerDBConnDlg m_ServerDBConnDlg;

protected:

	// Generated message map functions
	//{{AFX_MSG(CAdminWindowDlg)
	afx_msg void OnOk();
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonExecDbout();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADMINWINDOWDLG_H__15E7A766_46F9_44E2_8945_9BB4100892E9__INCLUDED_)
