#if !defined(AFX_FIELDSERVERDLG_H__3F414ED1_B064_43D1_B281_2837A14D957C__INCLUDED_)
#define AFX_FIELDSERVERDLG_H__3F414ED1_B064_43D1_B281_2837A14D957C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FieldServerDlg.h : header file
//

#include "CommandHistory.h"
#include "historycombobox.h"

/////////////////////////////////////////////////////////////////////////////
// CFieldServerDlg dialog

class CFieldServerDlg : public CDialog
{
// Construction
public:
	CFieldServerDlg(const char* ip = NULL, LPCSTR i_szIPHistory = NULL, int port = 0, CWnd* pParent = NULL);   // standard constructor
	~CFieldServerDlg();

// Dialog Data
	//{{AFX_DATA(CFieldServerDlg)
	enum { IDD = IDD_FIELD_SERVER_DLG };
	CHistoryComboBox	m_HistoryComboIPAddress;
	int		m_port;
	CString	m_ipaddr;
	//}}AFX_DATA

	CString m_szIPHistory;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFieldServerDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
//	CCommandHistory *m_pIPHistory;

protected:

	// Generated message map functions
	//{{AFX_MSG(CFieldServerDlg)
	virtual void OnCancel();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FIELDSERVERDLG_H__3F414ED1_B064_43D1_B281_2837A14D957C__INCLUDED_)
