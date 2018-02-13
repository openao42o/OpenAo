#if !defined(AFX_LOGINSUCCESSDLG_H__F00F814F_BD5B_4F59_88DB_763A77777CB6__INCLUDED_)
#define AFX_LOGINSUCCESSDLG_H__F00F814F_BD5B_4F59_88DB_763A77777CB6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LoginSuccessDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CLoginSuccessDlg dialog

class CLoginSuccessDlg : public CDialog
{
// Construction
public:
	CLoginSuccessDlg(CString cmdLine, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CLoginSuccessDlg)
	enum { IDD = IDD_DIALOG_LOGIN_SUCCESS };
	CString	m_CmdLine;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLoginSuccessDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CLoginSuccessDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LOGINSUCCESSDLG_H__F00F814F_BD5B_4F59_88DB_763A77777CB6__INCLUDED_)
