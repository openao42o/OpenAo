#if !defined(AFX_SQLREADERDLG_H__AC14196B_A548_45EB_98A0_42B356BF4317__INCLUDED_)
#define AFX_SQLREADERDLG_H__AC14196B_A548_45EB_98A0_42B356BF4317__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SQLReaderDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSQLReaderDlg dialog

class CSQLReaderDlg : public CDialog
{
// Construction
public:
	CSQLReaderDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSQLReaderDlg)
	enum { IDD = IDD_DIALOG_SQL_READER };
	CString	m_szSQLFilePath;
	BOOL	m_bCBArray;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSQLReaderDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSQLReaderDlg)
	afx_msg void OnOk();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SQLREADERDLG_H__AC14196B_A548_45EB_98A0_42B356BF4317__INCLUDED_)
