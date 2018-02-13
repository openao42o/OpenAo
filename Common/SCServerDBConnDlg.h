#if !defined(AFX_SCSERVERDBCONNDLG_H__6C50E6DE_C064_4FB1_99EA_CFE69AAAB826__INCLUDED_)
#define AFX_SCSERVERDBCONNDLG_H__6C50E6DE_C064_4FB1_99EA_CFE69AAAB826__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SCServerDBConnDlg.h : header file
//

#include "AtumParam.h"

/////////////////////////////////////////////////////////////////////////////
// CSCServerDBConnDlg dialog

class CSCServerDBConnDlg : public CDialog
{
// Construction
public:
	CSCServerDBConnDlg(BOOL i_bServer = TRUE, BOOL i_bServerLogin = TRUE, BOOL i_bDBLogin = TRUE, const char *i_szServerNameToSelect = NULL, CWnd* pParent = NULL);   // standard constructor

	void SetAttr(BOOL i_bServer = TRUE, BOOL i_bServerLogin = TRUE, BOOL i_bDBLogin = TRUE);

// Dialog Data
	//{{AFX_DATA(CSCServerDBConnDlg)
	enum { IDD = IDD_DIALOG_SERVER_DB_CONNECTION };
	CComboBox	m_ComboSelectServer;
	CString	m_szUIDServer;
	CString	m_szPWDServer;
	CString	m_szUIDDB;
	CString	m_szPWDDB;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSCServerDBConnDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	CString						m_szServerName;
	GAME_SERVER_INFO_FOR_ADMIN	*m_pServerInfo4Admin;
	BOOL						m_bServer;
	BOOL						m_bServerLogin;
	BOOL						m_bDBLogin;

protected:

	// Generated message map functions
	//{{AFX_MSG(CSCServerDBConnDlg)
	afx_msg void OnButtonConnect();
	afx_msg void OnButtonCancel();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCSERVERDBCONNDLG_H__6C50E6DE_C064_4FB1_99EA_CFE69AAAB826__INCLUDED_)
