#if !defined(AFX_LINKDETAILDLG_H__7CD9F9C1_15A2_4ACD_B949_D4F9581EDCCA__INCLUDED_)
#define AFX_LINKDETAILDLG_H__7CD9F9C1_15A2_4ACD_B949_D4F9581EDCCA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LinkDetailDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CLinkDetailDlg dialog
#include "resource.h"

class CLinkDetailDlg : public CDialog
{
// Construction
public:
	CLinkDetailDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CLinkDetailDlg)
	enum { IDD = IDD_DIALOG_LINKDETAIL };
	CString	m_valueEditItemNum;
	CString	m_valueEditLink;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLinkDetailDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CLinkDetailDlg)
	afx_msg void OnButtonEnd();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LINKDETAILDLG_H__7CD9F9C1_15A2_4ACD_B949_D4F9581EDCCA__INCLUDED_)
