#if !defined(AFX_LOGTABSERVERINTEGRATION_H__C09E8001_E80E_4E70_9E06_2C346E0583AF__INCLUDED_)
#define AFX_LOGTABSERVERINTEGRATION_H__C09E8001_E80E_4E70_9E06_2C346E0583AF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LogTabServerIntegration.h : header file
//
#include "SCGridHelper.h"
#include "ODBCStatement.h"

class CSCLogAdminDlg;

typedef vector<SERVERINTEGRATION_LOG_INFO>		vectSERVERINTEGRATION_LOG_INFO;

/////////////////////////////////////////////////////////////////////////////
// CLogTabServerIntegration dialog

class CLogTabServerIntegration : public CDialog
{
// Construction
public:
	CLogTabServerIntegration(CDialog *i_pMainDlg, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CLogTabServerIntegration)
	enum { IDD = IDD_LOG_TAB_SERVERINTEGRATION };
	CString	m_szAccountName;
	BOOL	m_bCheckAccountName;
	CTime	m_dateEnd;
	CTime	m_timeEnd;
	CTime	m_dateStart;
	CTime	m_timeStart;
	BOOL	m_bDate;
	BOOL	m_bCheckAccountUID;
	int		m_nAccountUID;
	BOOL	m_bCheckSourceCharacterName;
	BOOL	m_bCheckSourceCharacterUID;
	BOOL	m_bCheckSourceDBNum;
	CString	m_szSourceCharacterName;
	int		m_nSourceCharacterUID;
	int		m_nSourceDBNum;
	BOOL	m_bCheckTargetCharacterName;
	BOOL	m_bCheckTargetCharacterUID;
	BOOL	m_bCheckTargetDBNum;
	CString	m_szTargetCharacterName;
	int		m_nTargetCharacterUID;
	int		m_nTargetDBNum;
	int		m_nTotalLogCounts;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLogTabServerIntegration)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	CSCGridCtrl		m_GridServerIntegrationLog;
	CSCLogAdminDlg	*m_pMainDlg;
	CODBCStatement	*m_pODBCStmt;

	vectSERVERINTEGRATION_LOG_INFO m_vectSERVERINTEGRATION_LOG_INFO;

	void	InitGrid();
	void	ResetVariables();
	void	ResetControls();

	BOOL	MakeSzQueryForComplete();					// 2007-01-30 by dhjin, 쿼리완성을 위한 함수 

protected:
	CString		SzQuery;

	// Generated message map functions
	//{{AFX_MSG(CLogTabServerIntegration)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonSearch();
	afx_msg void OnCHECKAccountName();
	afx_msg void OnCheckDate();
	afx_msg void OnCHECKAccountUID();
	afx_msg void OnCheckSourceCharacterName();
	afx_msg void OnCheckSourceCharacterUid();
	afx_msg void OnCheckSourceDbNum();
	afx_msg void OnCheckTargetCharacterName();
	afx_msg void OnCheckTargetCharacterUid();
	afx_msg void OnCheckTargetDbNum();
	afx_msg void OnButtonReset();
	afx_msg void OnButtonSave();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LOGTABSERVERINTEGRATION_H__C09E8001_E80E_4E70_9E06_2C346E0583AF__INCLUDED_)
