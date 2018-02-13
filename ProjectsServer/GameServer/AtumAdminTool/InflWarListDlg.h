#if !defined(AFX_INFLWARLISTDLG_H__F1D6D929_E969_4B26_9B2F_C093034CF218__INCLUDED_)
#define AFX_INFLWARLISTDLG_H__F1D6D929_E969_4B26_9B2F_C093034CF218__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// InflWarListDlg.h : header file
//
#include "GridCtrl/GridCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CInflWarListDlg dialog
class CODBCStatement;
class CInflWarListDlg : public CDialog
{
// Construction
public:
	CInflWarListDlg(BOOL i_bEnableEdit, CODBCStatement *i_pODBCStat, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CInflWarListDlg)
	enum { IDD = IDD_DLG_INFLWAR_LIST };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInflWarListDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation

	void InitGridInflWarList(void);
	void LoadInflWarData(void);
	void DBInitAllInflWarData(void);
	void DBInitAllMonthlyGuildFame(void);
	void InsertInflWarData(SINFLUENCE_WAR_DATA *i_pInflWarData, char *i_szLeaderCharacterName, char *i_szSub1LeaderCharacterName, char *i_szSub2LeaderCharacterName);



	CGridCtrl				m_GridInflWarList;
	CODBCStatement			*m_pODBCStmt2;
	BOOL					m_bEnableEdit;

protected:

	// Generated message map functions
	//{{AFX_MSG(CInflWarListDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnReloadInflwardataList();
	afx_msg void OnBtnInitInflData();
	afx_msg void OnBtnInitMonthlyGuildfame();
	afx_msg void OnBtnLeaderElection();
	afx_msg void OnBtnDeclarationOfWar();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INFLWARLISTDLG_H__F1D6D929_E969_4B26_9B2F_C093034CF218__INCLUDED_)
