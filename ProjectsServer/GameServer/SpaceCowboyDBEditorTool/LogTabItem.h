#if !defined(AFX_LOGTABITEM_H__ED975BD4_DFE3_496F_873C_1F71A1A60960__INCLUDED_)
#define AFX_LOGTABITEM_H__ED975BD4_DFE3_496F_873C_1F71A1A60960__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LogTabItem.h : header file
//
#include "SCGridHelper.h"
#include "mt_stl.h"

class CSCLogAdminDlg;
class CODBCStatement;

/////////////////////////////////////////////////////////////////////////////
// CLogTabItem dialog

class CLogTabItem : public CDialog
{
// Construction
public:
	CLogTabItem(CDialog *i_pMainDlg, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CLogTabItem)
	enum { IDD = IDD_LOG_TAB_ITEM };
	CEdit	m_EditTotalLogRowCount;
	CString	m_szCharacterName;
	BOOL	m_bCheckCharacterName;
	BOOL	m_bCheckDate;
	CTime	m_dateStart;
	CTime	m_dateEnd;
	int		m_nMaxRowCount;
	BOOL	m_bCheckAddItemByAdmin;
	BOOL	m_bCheckAddItemByCommand;
	BOOL	m_bCheckBuyItem;
	BOOL	m_bCheckCharacterItem;
	BOOL	m_bCheckDelItemByAdmin;
	BOOL	m_bCheckDrawFromStore;
	BOOL	m_bCheckGetItem;
	BOOL	m_bCheckInsertToStore;
	BOOL	m_bCheckMaxRowCount;
	BOOL	m_bCheckSellItem;
	BOOL	m_bCheckThrowAwayItem;
	BOOL	m_bCheckTradeRecv;
	BOOL	m_bCheckTradeSend;
	BOOL	m_bCheckUseEnergy;
	BOOL	m_bCheckUseEnchant;
	BOOL	m_bCheckUseItem;
	BOOL	m_bCheckAccountName;
	CString	m_szAccountName;
	CString	m_szAccountName2;
	CString	m_szCharacterName2;
	BOOL	m_bCheckAccountName2;
	BOOL	m_bCheckCharacterName2;
	CString	m_szSelectedItemName;
	int		m_nItemUID;
	BOOL	m_bCheckItemUID;
	BOOL	m_bCheckItemName;
	CTime	m_timeEnd;
	CTime	m_timeStart;
	BOOL	m_bCheckStoreStartItem;
	BOOL	m_bCheckBazaarBuy;
	BOOL	m_bCheckBazaarSell;
	BOOL	m_ctlbCheckGiftRecv;
	BOOL	m_ctlbCheckGiftSend;
	BOOL	m_ctlbCheckRearrangeItemDeleted;
	BOOL	m_ctlbCheckRearrangeItemMain;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLogTabItem)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	CSCGridCtrl		m_GridItemLog;
	CSCLogAdminDlg	*m_pMainDlg;
	CODBCStatement	*m_pODBCStmt;

	void ResetVariables();
	void ResetControls();

	BOOL GetLogAndInsertToGrid(T1<T0::FL_LOG> T1_LogType, LPCSTR szTableName, LPCSTR szContentString);

	// 2007-11-08 by cmkwon, 모든 로그 검색 기능 SCDBTool로 이동 -
	//ez_map<INT, ITEM>		m_mapItemInfo;
	//ez_map<string, ITEM>	m_mapItemInfoName;
	ez_map<INT, ITEM>		*m_pmapItemInfo;
	ez_map<string, ITEM>	*m_pmapItemInfoName;

	int		m_nSelectedItemNum;

protected:

	// Generated message map functions
	//{{AFX_MSG(CLogTabItem)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonOk();
	afx_msg void OnButtonReset();
	afx_msg void OnCheckAccountName();
	afx_msg void OnCheckCharacter();
	afx_msg void OnCheckDate();
	afx_msg void OnCheckMaxRowCount();
	afx_msg void OnCheckAccountName2();
	afx_msg void OnCheckCharacter2();
	afx_msg void OnCheckTradeRecv();
	afx_msg void OnCheckTradeSend();
	afx_msg void OnButtonSelectAll();
	afx_msg void OnButtonDiselectAll();
	afx_msg void OnButtonSelectItem();
	afx_msg void OnCheckItemNum();
	afx_msg void OnCheckItemUid();
	afx_msg void OnBtnSaveResult();
	afx_msg void OnCHECKBazaarSell();
	afx_msg void OnCHECKBazaarBuy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LOGTABITEM_H__ED975BD4_DFE3_496F_873C_1F71A1A60960__INCLUDED_)
