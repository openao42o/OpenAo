#if !defined(AFX_INFOGENDLG_H__F2632E33_5E5A_496A_9CB0_FBC6C8F67BE7__INCLUDED_)
#define AFX_INFOGENDLG_H__F2632E33_5E5A_496A_9CB0_FBC6C8F67BE7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// InfoGenDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CInfoGenDlg dialog

class CInfoGenDlg : public CDialog
{
// Construction
public:
	CInfoGenDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CInfoGenDlg)
	enum { IDD = IDD_INFO_GEN };
	CString	m_szInfoGenOutPath;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInfoGenDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CInfoGenDlg)
	afx_msg void OnButtonIgItem();
	afx_msg void OnButtonIgMonster();
	afx_msg void OnButtonIgMapobject();
	afx_msg void OnButtonIgAll();
	virtual BOOL OnInitDialog();
//	afx_msg void OnButtonIgQuest();
//	afx_msg void OnButtonIgQuestNpcTalk();
	afx_msg void OnButtonIgRareFix();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INFOGENDLG_H__F2632E33_5E5A_496A_9CB0_FBC6C8F67BE7__INCLUDED_)
