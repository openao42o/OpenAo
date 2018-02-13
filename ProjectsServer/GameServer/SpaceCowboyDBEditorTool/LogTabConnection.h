#if !defined(AFX_LOGTABCONNECTION_H__B433400A_6497_4257_82B8_C408C1AE1F63__INCLUDED_)
#define AFX_LOGTABCONNECTION_H__B433400A_6497_4257_82B8_C408C1AE1F63__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LogTabConnection.h : header file
//
#include "SCGridHelper.h"
#include "ODBCStatement.h"
#include "AtumProtocol.h"

class CSCLogAdminDlg;

/////////////////////////////////////////////////////////////////////////////
// CLogTabConnection dialog

class CLogTabConnection : public CDialog
{
// Construction
public:
	CLogTabConnection(CDialog *i_pMainDlg, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CLogTabConnection)
	enum { IDD = IDD_LOG_TAB_CONNECTION };
	CIPAddressCtrl	m_ctlIPAddress;
	BOOL	m_bCheckAccountName;
	BOOL	m_bCheckIPAddress;
	CString	m_szAccountName;
	BOOL	m_bCheckCharacterName;
	CString	m_szCharacterName;
	BOOL	m_bCheckDate;
	CTime	m_dateStart;
	CTime	m_dateEnd;
	BOOL	m_bCheckMaxRowCount;
	int		m_nMaxRowCount;
	CEdit	m_EditTotalLogRowCount;
	BOOL	m_bCheckLogLogin;
	BOOL	m_bCheckLogLogout;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLogTabConnection)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:

public:
	CSCGridCtrl		m_GridConnectionLog;
	CSCLogAdminDlg	*m_pMainDlg;
	CODBCStatement	*m_pODBCStmt;

	void ResetVariables();
	void ResetControls();

	BOOL GetLogAndInsertToGrid(T1<T0::FL_LOG> T1_LogType, LPCSTR szTableName);

protected:

	// Generated message map functions
	//{{AFX_MSG(CLogTabConnection)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonOk();
	afx_msg void OnButtonReset();
	afx_msg void OnCheckAccountName();
	afx_msg void OnCheckIPAddress();
	afx_msg void OnCheckCharacter();
	afx_msg void OnCheckDate();
	afx_msg void OnCheckMaxRowCount();
	afx_msg void OnButtonSelectAll();
	afx_msg void OnButtonDiselectAll();
	afx_msg void OnBtnSaveResult();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LOGTABCONNECTION_H__B433400A_6497_4257_82B8_C408C1AE1F63__INCLUDED_)
