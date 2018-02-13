#if !defined(AFX_LOGTABUSER_H__C550664F_38CC_4244_9C78_1468D20576FA__INCLUDED_)
#define AFX_LOGTABUSER_H__C550664F_38CC_4244_9C78_1468D20576FA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LogTabUser.h : header file
//
#include "SCGridHelper.h"
#include "ODBCStatement.h"

class CSCLogAdminDlg;

/////////////////////////////////////////////////////////////////////////////
// CLogTabUser dialog

class CLogTabUser : public CDialog
{
// Construction
public:
	CLogTabUser(CDialog *i_pMainDlg, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CLogTabUser)
	enum { IDD = IDD_LOG_TAB_USER };
	BOOL	m_bCheckAccountName;
	CString	m_szAccountName;
	BOOL	m_bCheckCharacterName;
	CString	m_szCharacterName;
	BOOL	m_bCheckDate;
	CTime	m_dateStart;
	CTime	m_dateEnd;
	BOOL	m_bCheckMaxRowCount;
	int		m_nMaxRowCount;
	CEdit	m_EditTotalLogRowCount;
	BOOL	m_bCheckUserDead;
	BOOL	m_bCheckUserExpChange;
	BOOL	m_bCheckUserGameEnd;
	BOOL	m_bCheckUserGameStart;
	BOOL	m_bCheckUserLevelChange;
	BOOL	m_bCheckUserPVPLose;
	BOOL	m_bCheckUserPVPWin;
	BOOL	m_bCheckUserQuestEnd;
	BOOL	m_bCheckUserSpeedHack;
	BOOL	m_bCheckUserMemoryHack;
	BOOL	m_bCheckUserStatChange;
	BOOL	m_bCheckUserWarp;
	BOOL	m_bCheckUserCreateCharacter;
	BOOL	m_bCheckUserDeleteCharacter;
	BOOL	m_bCheckUserPvELoss;				// 2006-03-06 by cmkwon
	BOOL	m_bCheckUserPvEWin;					// 2006-03-06 by cmkwon
	CTime	m_TimeEnd;
	CTime	m_TimeStart;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLogTabUser)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	CSCGridCtrl		m_GridUserLog;
	CSCLogAdminDlg	*m_pMainDlg;
	CODBCStatement	*m_pODBCStmt;

	void ResetVariables();
	void ResetControls();

	BOOL GetLogAndInsertToGrid(T1<T0::FL_LOG> T1_LogType, LPCSTR szTableName, LPCSTR szContentString1, LPCSTR szContentString2 = NULL);
	void SetContentsByLogType(T1<T0::FL_LOG> T1_LogType, LPCSTR i_szContents1, LPCSTR i_szContents2, CString &o_szContents);

	// 2011-04-20 by shcho, 러시아 로그에서 MemoryHackCheckType(14) 제거
	size_t CLogTabUser::strpos(const string &haystack, const string &needle);
	//end  2011-04-20 by shcho, 러시아 로그에서 MemoryHackCheckType(14)

protected:

	// Generated message map functions
	//{{AFX_MSG(CLogTabUser)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonOk();
	afx_msg void OnButtonReset();
	afx_msg void OnCheckAccountName();
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

#endif // !defined(AFX_LOGTABUSER_H__C550664F_38CC_4244_9C78_1468D20576FA__INCLUDED_)
