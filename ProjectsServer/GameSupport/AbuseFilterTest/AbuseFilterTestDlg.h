// AbuseFilterTestDlg.h : header file
//

#if !defined(AFX_ABUSEFILTERTESTDLG_H__B8713A46_21D5_4884_BAA8_8170524F42B2__INCLUDED_)
#define AFX_ABUSEFILTERTESTDLG_H__B8713A46_21D5_4884_BAA8_8170524F42B2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AbuseFilter.h"


/////////////////////////////////////////////////////////////////////////////
// CAbuseFilterTestDlg dialog

class CAbuseFilterTestDlg : public CDialog
{
// Construction
public:
	CAbuseFilterTestDlg(CWnd* pParent = NULL);	// standard constructor

	CAbuseFilter		m_AbuseFilter;
// Dialog Data
	//{{AFX_DATA(CAbuseFilterTestDlg)
	enum { IDD = IDD_ABUSEFILTERTEST_DIALOG };
	CString	m_ctl_strSourceString;
	CString	m_ctl_strResultString;
	UINT	m_ctl_uiFilterTotalBytes;
	UINT	m_ctl_uiFilterWordCounts;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAbuseFilterTestDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CAbuseFilterTestDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBtnFiltering();
	afx_msg void OnButton1();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ABUSEFILTERTESTDLG_H__B8713A46_21D5_4884_BAA8_8170524F42B2__INCLUDED_)
