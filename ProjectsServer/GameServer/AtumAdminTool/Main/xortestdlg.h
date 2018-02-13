#if !defined(AFX_XORTESTDLG_H__B4186A30_0B8E_4BBE_A8A2_C7E55B81CFA9__INCLUDED_)
#define AFX_XORTESTDLG_H__B4186A30_0B8E_4BBE_A8A2_C7E55B81CFA9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// xortestdlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CXORTestDlg dialog

class CXORTestDlg : public CDialog
{
// Construction
public:
	CXORTestDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CXORTestDlg)
	enum { IDD = IDD_DLG_XOR_TEST };
	CEdit	m_ctlEditSourceString;
	CEdit	m_ctlEditKeyString;
	CEdit	m_ctlEditEncodedString;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CXORTestDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CXORTestDlg)
	afx_msg void OnBtnXorEncode();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_XORTESTDLG_H__B4186A30_0B8E_4BBE_A8A2_C7E55B81CFA9__INCLUDED_)
