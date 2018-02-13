#if !defined(AFX_ACCOUNTINFODLG_H__7AC50441_DB62_4ADF_BFB7_4048EDABEBAC__INCLUDED_)
#define AFX_ACCOUNTINFODLG_H__7AC50441_DB62_4ADF_BFB7_4048EDABEBAC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AccountInfoDlg.h : header file
//
#include "ODBCStatement.h"

/////////////////////////////////////////////////////////////////////////////
// CAccountInfoDlg dialog

class CAccountInfoDlg : public CDialog
{
// Construction
public:
	CAccountInfoDlg(BOOL i_bEnableEdit, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAccountInfoDlg)
	enum { IDD = IDD_DIALOG_EDIT_ACCOUNT };
	CComboBox	m_ComboAccountType;
	CString	m_szAccountName;
	CString	m_szPassword;
	CString	m_szSex;
	CString	m_szRegisterdDate;
	CString	m_szLastLoginDate;
	BOOL	m_bAccountBlocked;
	BOOL	m_bChattingBlocked;
	CString	m_szBirthYear;
	CString	m_ctlcsSecPassword;
	//}}AFX_DATA

	USHORT			m_nAcountType;
	BOOL			m_bEnableEdit;			// 2006-04-15 by cmkwon

	// 2012-06-07 by hskim, 관리자 계정 보안 강화 - 계정 권한 변경시 로그 남김
	USHORT			m_nOldAcountType;
	BOOL			m_bChangeAccountType;
	// end 2012-06-07 by hskim, 관리자 계정 보안 강화 - 계정 권한 변경시 로그 남김
	

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAccountInfoDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	CODBCStatement *m_pODBCStmt;

	// Generated message map functions
	//{{AFX_MSG(CAccountInfoDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnOk();
	virtual void OnCancel();
	afx_msg void OnButtonMd5Pwd();
	afx_msg void OnButtonMd5Secpwd();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ACCOUNTINFODLG_H__7AC50441_DB62_4ADF_BFB7_4048EDABEBAC__INCLUDED_)
