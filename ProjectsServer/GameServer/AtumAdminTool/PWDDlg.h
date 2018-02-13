#if !defined(AFX_PWDDLG_H__84FB8602_91FC_4EFA_9474_50916938CF21__INCLUDED_)
#define AFX_PWDDLG_H__84FB8602_91FC_4EFA_9474_50916938CF21__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PWDDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPWDDlg dialog

class CPWDDlg : public CDialog
{
// Construction
public:
	CPWDDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPWDDlg)
	enum { IDD = IDD_DIALOG_PWD };
	CString	m_ctlStrPassword;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPWDDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPWDDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PWDDLG_H__84FB8602_91FC_4EFA_9474_50916938CF21__INCLUDED_)
