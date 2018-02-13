#if !defined(AFX_SETLANGUAGEDLG_H__A59AE6FE_9751_4A89_870A_859E4A7E736B__INCLUDED_)
#define AFX_SETLANGUAGEDLG_H__A59AE6FE_9751_4A89_870A_859E4A7E736B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SetLanguageDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSetLanguageDlg dialog

class CSetLanguageDlg : public CDialog
{
// Construction
public:
	CSetLanguageDlg(CString i_strLocalizationPath, int i_nLangType, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSetLanguageDlg)
	enum { IDD = IDD_DLG_LOCALIZATION };
	CEdit	m_ctl_editLocalPath;
	CString	m_ctl_strLocalPath;
	//}}AFX_DATA

	int					m_nLanguageType;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSetLanguageDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSetLanguageDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnRadioKorean();
	afx_msg void OnRadioJapanese();
	afx_msg void OnRadioEnglish();
	afx_msg void OnRadioChinese();
	afx_msg void OnRadioVietnamese();
	afx_msg void OnBtnSelectPath();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETLANGUAGEDLG_H__A59AE6FE_9751_4A89_870A_859E4A7E736B__INCLUDED_)
