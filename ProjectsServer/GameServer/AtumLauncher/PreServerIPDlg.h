#if !defined(AFX_PRESERVERIPDLG_H__8122AA2F_09CE_48C2_AECA_83B0D1E96DE0__INCLUDED_)
#define AFX_PRESERVERIPDLG_H__8122AA2F_09CE_48C2_AECA_83B0D1E96DE0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PreServerIPDlg.h : header file
//

#include "CommandHistory.h"
#include "historycombobox.h"

/////////////////////////////////////////////////////////////////////////////
// CPreServerIPDlg dialog

class CPreServerIPDlg : public CDialog
{
// Construction
public:
	CPreServerIPDlg(CString i_szPreServerIP, CString i_szBin, CString i_szPath,
					CString i_szPreServerIPHistory, CString i_szBinHistory, CString i_szPathHistory,
					CWnd* pParent = NULL);   // standard constructor
	~CPreServerIPDlg();

// Dialog Data
	//{{AFX_DATA(CPreServerIPDlg)
	enum { IDD = IDD_DIALOG_PRE_IP };
	CHistoryComboBox	m_ComboPath;
	CHistoryComboBox	m_ComboBin;
	CHistoryComboBox	m_ComboPreServerIP;
	BOOL	m_bUpdateClientFile;
	BOOL	m_bWindowMode;
	CString	m_executable_bin;
	CString	m_execute_path;
	CString	m_preserver_ip;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPreServerIPDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	CString m_szPreServerIPHistory;
	CString m_szExecuteBinHistory;
	CString m_szExecutePathHistory;

protected:

	// Generated message map functions
	//{{AFX_MSG(CPreServerIPDlg)
	afx_msg void OnWindowMode();
	virtual BOOL OnInitDialog();
	afx_msg void OnOk();
	afx_msg void OnButtonBinSelect();
	afx_msg void OnButtonPathSelect();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PRESERVERIPDLG_H__8122AA2F_09CE_48C2_AECA_83B0D1E96DE0__INCLUDED_)
