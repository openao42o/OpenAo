#if !defined(AFX_PREUSERCOUNTDLG_H__89915075_221E_4221_AAC9_22A68C0A87BA__INCLUDED_)
#define AFX_PREUSERCOUNTDLG_H__89915075_221E_4221_AAC9_22A68C0A87BA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PreUserCountDlg.h : header file
//

#define DIALOG_TYPE_LIMIT_USER				0
#define DIALOG_TYPE_LIMIT_MONSTER			1


/////////////////////////////////////////////////////////////////////////////
// CPreUserCountDlg dialog

class CPreUserCountDlg : public CDialog
{
// Construction
public:
	CPreUserCountDlg(int nDlgType, UINT nLimitCount, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPreUserCountDlg)
	enum { IDD = IDD_DLG_PRE_USER_COUNT };
	UINT	m_ctl_uiLimitGroupUserCounts;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPreUserCountDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPreUserCountDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PREUSERCOUNTDLG_H__89915075_221E_4221_AAC9_22A68C0A87BA__INCLUDED_)
