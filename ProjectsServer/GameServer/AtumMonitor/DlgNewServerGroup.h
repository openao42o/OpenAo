#if !defined(AFX_DLGNEWSERVERGROUP_H__28E394F5_C34B_4603_AB2A_9FA38A78B392__INCLUDED_)
#define AFX_DLGNEWSERVERGROUP_H__28E394F5_C34B_4603_AB2A_9FA38A78B392__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgNewServerGroup.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgNewServerGroup dialog

class CDlgNewServerGroup : public CDialog
{
// Construction
public:
	CDlgNewServerGroup(BOOL bNewServerGroup, CString csName = "", CWnd* pParent = NULL);   // standard constructor

	CString		m_csNewServerGroupName;
	BOOL		m_bNewServerGroup;
	CString		m_csBeforeServerGroupName;
// Dialog Data
	//{{AFX_DATA(CDlgNewServerGroup)
	enum { IDD = IDD_DLG_NEWSERVERGROUP };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgNewServerGroup)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgNewServerGroup)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGNEWSERVERGROUP_H__28E394F5_C34B_4603_AB2A_9FA38A78B392__INCLUDED_)
