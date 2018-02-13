#if !defined(AFX_WRSEVICESETTINGDLG_H__6F920A57_7CB6_4955_A1EE_33C9939A6F89__INCLUDED_)
#define AFX_WRSEVICESETTINGDLG_H__6F920A57_7CB6_4955_A1EE_33C9939A6F89__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// wrsevicesettingdlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CWRSeviceSettingDlg dialog
class CWRankingManagement;
class CWRSeviceSettingDlg : public CDialog
{
// Construction
public:
	CWRSeviceSettingDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CWRSeviceSettingDlg)
	enum { IDD = IDD_DLG_WRK_SERVICE_SETTING };
	CString	m_ctrlcsServiceUID;
	CString	m_ctrlcsServiceName;
	CString	m_ctrlcsSymbolImagePath;
	CString	m_ctrlcsServerGroupName;
	CString	m_ctrlcsServerGroupID;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWRSeviceSettingDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL


	CWRankingManagement			*m_pWRankManager;
	SWRK_SERVICE_INFO			m_RegServiceInfo;
	SWRK_SERVER_GROUP			m_RegServerGroupInfo;

	bool IsValidServiceInfo() const;

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CWRSeviceSettingDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnFindFile();
	afx_msg void OnBtnRegOrUpdate();
	afx_msg void OnBtnDeleteService();
	afx_msg void OnBtnDeleteServerGroupInfo();
	afx_msg void OnBtnUpdateServerGroupName();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WRSEVICESETTINGDLG_H__6F920A57_7CB6_4955_A1EE_33C9939A6F89__INCLUDED_)
