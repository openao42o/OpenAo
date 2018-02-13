#if !defined(AFX_DLGGUARDGGREEMENT_H__580D5CD7_CC15_4E0C_8F0B_F2EF4D9E47FF__INCLUDED_)
#define AFX_DLGGUARDGGREEMENT_H__580D5CD7_CC15_4E0C_8F0B_F2EF4D9E47FF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgGuardGgreement.h : header file
//

#include "IExplore.h"		// 2009-09-02 by cmkwon, Gameforge4D 게임 가드 동의창 WebPage로 처리 - 


/////////////////////////////////////////////////////////////////////////////
// CDlgGuardAgreement dialog

class CDlgGuardAgreement : public CDialog
{
// Construction
public:
	CDlgGuardAgreement(CWnd* pParent = NULL);   // standard constructor

	Host				*m_pGuardAgreementHost;		// 2009-09-02 by cmkwon, Gameforge4D 게임 가드 동의창 WebPage로 처리 - 


// Dialog Data
	//{{AFX_DATA(CDlgGuardAgreement)
	enum { IDD = IDD_DLG_GUARD_AGREEMENT };
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgGuardAgreement)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL


// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgGuardAgreement)
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGGUARDGGREEMENT_H__580D5CD7_CC15_4E0C_8F0B_F2EF4D9E47FF__INCLUDED_)
