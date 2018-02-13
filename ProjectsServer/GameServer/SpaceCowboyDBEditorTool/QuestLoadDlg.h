#if !defined(AFX_QUESTLOADDLG_H__BA3B9C4A_B1D8_41B9_8529_46C31626B36F__INCLUDED_)
#define AFX_QUESTLOADDLG_H__BA3B9C4A_B1D8_41B9_8529_46C31626B36F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// QuestLoadDlg.h : header file
//
#include "QuestLoaderParser.h"
#include "Quest.h"
#include <assert.h>
/////////////////////////////////////////////////////////////////////////////
// CQuestLoadDlg dialog

class CQuestLoadDlg : public CDialog
{
// Construction
public:
	void InitQuestEditor();
	void ReflectQuestEditor();
	CQuestLoadDlg(CWnd* pParent = NULL);   // standard constructor
	~CQuestLoadDlg();
	BOOL GetQuestFilesPath(char *o_szQuestDefinedPath, vector<string> *o_pVecQuestFiles);
	CString m_strOriginalFile;
	CQuestEditorDlg* m_pQuestEditorDlg;
	ez_map<INT, CQuest*> QuestInfoMap;
	ez_map<INT, CQuest*>::iterator itr;
	
// Dialog Data
	//{{AFX_DATA(CQuestLoadDlg)
	enum { IDD = IDD_DIALOG_QUESTLOAD };
	CComboBox	m_ctrlQuestLoad;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CQuestLoadDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CQuestLoadDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonSelect();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_QUESTLOADDLG_H__BA3B9C4A_B1D8_41B9_8529_46C31626B36F__INCLUDED_)
