#if !defined(AFX_SETLIMITUSERCOUNTDLG_H__58CE3E73_074E_4095_984A_7E8C5AB12045__INCLUDED_)
#define AFX_SETLIMITUSERCOUNTDLG_H__58CE3E73_074E_4095_984A_7E8C5AB12045__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// setlimitusercountdlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSetLimitUserCountDlg dialog

class CSetLimitUserCountDlg : public CDialog
{
// Construction
public:
	CSetLimitUserCountDlg(int i_nLimitUserCount, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSetLimitUserCountDlg)
	enum { IDD = IDD_DLG_SET_LIMIT_USER_COUNT };
	int		m_nUserCount;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSetLimitUserCountDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSetLimitUserCountDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETLIMITUSERCOUNTDLG_H__58CE3E73_074E_4095_984A_7E8C5AB12045__INCLUDED_)
