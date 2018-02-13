#if !defined(AFX_EXPVIEWERDLG_H__114C6198_7213_4A34_9FFB_45797D7815C8__INCLUDED_)
#define AFX_EXPVIEWERDLG_H__114C6198_7213_4A34_9FFB_45797D7815C8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ExpViewerDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CExpViewerDlg dialog

class CExpViewerDlg : public CDialog
{
// Construction
public:
	CExpViewerDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CExpViewerDlg)
	enum { IDD = IDD_DLG_EXP_VIEWER };
	double	m_ctl_doTotalExp;
	int		m_ctl_nLevel;
	float	m_ctl_fPercent;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExpViewerDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CExpViewerDlg)
	afx_msg void OnBtnToLevel();
	virtual void OnOK();
	afx_msg void OnBtnToExp();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EXPVIEWERDLG_H__114C6198_7213_4A34_9FFB_45797D7815C8__INCLUDED_)
