#if !defined(AFX_PETITIONMANAGEMENTDLG_H__6232D65D_DCD4_42D9_A289_1D9B9BF5DEB4__INCLUDED_)
#define AFX_PETITIONMANAGEMENTDLG_H__6232D65D_DCD4_42D9_A289_1D9B9BF5DEB4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PetitionManagementDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPetitionManagementDlg dialog
class CAtumAdminToolDlg;
class CPetitionManagementDlg : public CDialog
{
// Construction
public:
	CPetitionManagementDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPetitionManagementDlg)
	enum { IDD = IDD_DLG_PETITION_MANAGEMENT };
	COleDateTime	m_ctlodtStartDate;
	COleDateTime	m_ctlodtStartTime;
	COleDateTime	m_ctlodtEndDate;
	COleDateTime	m_ctlodtEndTime;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPetitionManagementDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	CAtumAdminToolDlg			*m_pMainDlg; // 2007-11-19 by cmkwon, 진정시스템 업데이트 - AtumAdminToll 다이얼로그 클래스

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPetitionManagementDlg)
	afx_msg void OnBtnImmediatOn();
	afx_msg void OnBtnImmediatOff();
	afx_msg void OnBtnPeriodSetting();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PETITIONMANAGEMENTDLG_H__6232D65D_DCD4_42D9_A289_1D9B9BF5DEB4__INCLUDED_)
