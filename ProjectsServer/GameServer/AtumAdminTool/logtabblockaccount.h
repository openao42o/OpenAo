#if !defined(AFX_LOGTABBLOCKACCOUNT_H__74BD9F1F_9CEA_4DA0_811A_406E43424823__INCLUDED_)
#define AFX_LOGTABBLOCKACCOUNT_H__74BD9F1F_9CEA_4DA0_811A_406E43424823__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// logtabblockaccount.h : header file
//
#include "SCGridHelper.h"
#include "ODBCStatement.h"
#include "AtumProtocol.h"

#include "AtumAdminToolDlg.h"		// 2013-04-30 by bckim. 海飘巢 款康砒包访 贸府	

/////////////////////////////////////////////////////////////////////////////
// CLogTabBlockAccount dialog
class CSCLogAdminDlg;
class CLogTabBlockAccount : public CDialog
{
// Construction
public:
	CLogTabBlockAccount(CDialog *i_pMainDlg, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CLogTabBlockAccount)
	enum { IDD = IDD_LOG_TAB_BLOCKACCOUNT };
	CComboBox	m_comboBlockTypeList;
	CEdit	m_EditTotalLogRowCount;
	BOOL	m_bCheckMaxRowCount;
	int		m_nMaxRowCount;
	BOOL	m_bCheckAccountName;
	CString	m_szAccountName;
	BOOL	m_bCheckDate;
	COleDateTime	m_EndDate;
	COleDateTime	m_EndTime;
	COleDateTime	m_StartDate;
	COleDateTime	m_StartTime;
	BOOL	m_bBlockLog;
	BOOL	m_bUnblockLog;
	BOOL	m_bCheckAdminAccountName;
	BOOL	m_bCheckBlockType;
	CString	m_csAdminAccountName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLogTabBlockAccount)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	CSCGridCtrl		m_GridBlockAccountLog;
	CSCLogAdminDlg	*m_pMainDlg;
	CODBCStatement	*m_pODBCStmt;

	void	InitGrid(void);
	void	ResetVariables(void);
	void	ResetControls(void);

	BOOL GetLogAndInsertToGrid(T1<T0::FL_LOG> T1_LogType, LPCSTR szTableName);
	void InsertGridRow(char *i_szLogString, ATUM_DATE_TIME *i_patLogTime, SBLOCKED_ACCOUNT_INFO *i_pBlockAccInfo);

protected:

	// Generated message map functions
	//{{AFX_MSG(CLogTabBlockAccount)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonOk();
	afx_msg void OnButtonSelectAll();
	afx_msg void OnButtonReset();
	afx_msg void OnButtonDiselectAll();
	afx_msg void OnBtnSaveResult();
	afx_msg void OnCheckAccountName();
	afx_msg void OnCheckAdminAccountName();
	afx_msg void OnCheckBlockedtype();
	afx_msg void OnCheckDate();
	afx_msg void OnCheckLogBlock();
	afx_msg void OnCheckMaxRowCount();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LOGTABBLOCKACCOUNT_H__74BD9F1F_9CEA_4DA0_811A_406E43424823__INCLUDED_)
