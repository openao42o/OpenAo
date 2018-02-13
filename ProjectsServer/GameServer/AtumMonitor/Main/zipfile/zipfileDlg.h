// zipfileDlg.h : header file
//

#if !defined(AFX_ZIPFILEDLG_H__0724E8B7_99A7_4236_9E11_44A13DEB329A__INCLUDED_)
#define AFX_ZIPFILEDLG_H__0724E8B7_99A7_4236_9E11_44A13DEB329A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CZipfileDlg dialog

class CZipfileDlg : public CDialog
{
// Construction
public:
	CZipfileDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CZipfileDlg)
	enum { IDD = IDD_ZIPFILE_DIALOG };
	BOOL	m_bEncode;
	CString	m_strNumber;
	CString	m_szTargetFileName;
	CString	m_szEncodeStr;
	CString	m_szTargetDir;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CZipfileDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
	void ReadProfile();
	void WriteProfile();

protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CZipfileDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBUTTONAddFile();
	afx_msg void OnBUTTONDelete();
	afx_msg void OnBUTTONMake();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ZIPFILEDLG_H__0724E8B7_99A7_4236_9E11_44A13DEB329A__INCLUDED_)
