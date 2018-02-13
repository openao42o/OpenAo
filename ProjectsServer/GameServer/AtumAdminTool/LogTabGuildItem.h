#if !defined(AFX_LOGTABGUILDITEM_H__449FFE5E_B952_49C5_92AD_45315734AFE0__INCLUDED_)
#define AFX_LOGTABGUILDITEM_H__449FFE5E_B952_49C5_92AD_45315734AFE0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LogTabGuildItem.h : header file
//

#include "SCGridHelper.h"
#include "mt_stl.h"

class CSCLogAdminDlg;
class CODBCStatement;

/////////////////////////////////////////////////////////////////////////////
// CLogTabGuildItem dialog

class CLogTabGuildItem : public CDialog
{
// Construction
public:
	CLogTabGuildItem(CDialog *i_pMainDlg, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CLogTabGuildItem)
	enum { IDD = IDD_LOG_TAB_GUILDITEM };
	CEdit	m_EditTotalLogRowCount;
	BOOL	m_bCheckCharacterName;
	BOOL	m_bCheckAccountName;
	BOOL	m_bCheckDate;
	BOOL	m_bCheckDrawGuildStore;
	BOOL	m_bCheckGuildUID;
	BOOL	m_bCheckGuildName;
	BOOL	m_bCheckInsertGuildStore;
	BOOL	m_bCheckItemUID;
	BOOL	m_bCheckItemName;
	BOOL	m_bCheckItemNum;
	BOOL	m_bCheckMaxRowCount;
	CString	m_szAccountName;
	CString	m_szCharacterName;
	CString	m_szGuildName;
	int		m_nGuildUID;
	int		m_nItemNum;
	int		m_nItemUID;
	int		m_nMaxRowCount;
	int		m_nTotalLogRowCount;
	CString	m_szSelectedItemName;
	COleDateTime	m_EndDate;
	COleDateTime	m_EndTime;
	COleDateTime	m_StartDate;
	COleDateTime	m_StartTime;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLogTabGuildItem)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	CSCGridCtrl		m_GridItemLog;
	CSCLogAdminDlg	*m_pMainDlg;
	CODBCStatement	*m_pODBCStmt;
	
	ez_map<INT, ITEM>		m_mapItemInfo;
	ez_map<string, ITEM>	m_mapItemInfoName;
	LOG_GUILDSTORE_ITEM_INFO	m_LOG_GUILDSTORE_ITEM_INFO;

	BOOL GetLogAndInsertToGrid(T1<T0::FL_LOG> T1_LogType, LPCSTR szTableName, LPCSTR szContentString);
	void InitGrid();
	void InsertGridItemLog();
	void ResetControls();
	void ResetValues();

protected:

	// Generated message map functions
	//{{AFX_MSG(CLogTabGuildItem)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonSearch();
	afx_msg void OnCheckInsertGuildstore();
	afx_msg void OnCheckDrawGuildstore();
	afx_msg void OnCheckAccountName();
	afx_msg void OnCheckCharactername();
	afx_msg void OnCheckDate();
	afx_msg void OnCheckGuildUid();
	afx_msg void OnCheckGuildname();
	afx_msg void OnCheckItemUid();
	afx_msg void OnCheckItemname();
	afx_msg void OnCheckItemnum();
	afx_msg void OnCheckMaxRowCount();
	afx_msg void OnButtonSelectAll();
	afx_msg void OnButtonDiselectAll();
	afx_msg void OnButtonReset();
	afx_msg void OnBtnSaveResult();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LOGTABGUILDITEM_H__449FFE5E_B952_49C5_92AD_45315734AFE0__INCLUDED_)
