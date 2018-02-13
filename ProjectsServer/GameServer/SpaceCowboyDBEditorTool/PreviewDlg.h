#if !defined(AFX_PREVIEWDLG_H__49F1DC90_5E6F_403C_8E8B_EFA0366FE147__INCLUDED_)
#define AFX_PREVIEWDLG_H__49F1DC90_5E6F_403C_8E8B_EFA0366FE147__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PreviewDlg.h : header file
//
class CQuestEditorDlg;

/////////////////////////////////////////////////////////////////////////////
// CPreviewDlg dialog

class CPreviewDlg : public CDialog
{
// Construction
public:
	bool CheckValidation();
	bool InsertQuest();
	CPreviewDlg(CWnd* pParent = NULL);   // standard constructor
	CQuestEditorDlg* m_pQuestEditorDlg;
// Dialog Data
	//{{AFX_DATA(CPreviewDlg)
	enum { IDD = IDD_DIALOG_PREVIEW };
	CRichEditCtrl	m_ctrlRichEditPreview;
	CEdit	m_ctrlEditPreview;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPreviewDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPreviewDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonEnd();
	afx_msg void OnButtonFilesave();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PREVIEWDLG_H__49F1DC90_5E6F_403C_8E8B_EFA0366FE147__INCLUDED_)
