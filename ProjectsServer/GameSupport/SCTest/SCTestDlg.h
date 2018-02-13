// SCTestDlg.h : header file
//

#if !defined(AFX_SCTESTDLG_H__8DC0BBBD_8B1F_4462_B469_E058A5171017__INCLUDED_)
#define AFX_SCTESTDLG_H__8DC0BBBD_8B1F_4462_B469_E058A5171017__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CSCTestDlg dialog

class CSCTestDlg : public CDialog
{
// Construction
public:
	CSCTestDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CSCTestDlg)
	enum { IDD = IDD_SCTEST_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSCTestDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CSCTestDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCTESTDLG_H__8DC0BBBD_8B1F_4462_B469_E058A5171017__INCLUDED_)
