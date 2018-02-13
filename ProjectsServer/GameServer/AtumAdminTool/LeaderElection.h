#if !defined(AFX_LEADERELECTION_H__0E12F658_41A2_46C2_A03B_5CD4913D3808__INCLUDED_)
#define AFX_LEADERELECTION_H__0E12F658_41A2_46C2_A03B_5CD4913D3808__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LeaderElection.h : header file
//
#include "GridCtrl/GridCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CLeaderElection dialog
class CODBCStatement;
class CLeaderElection : public CDialog
{
// Construction
public:
	CLeaderElection(CODBCStatement *i_pODBCStat, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CLeaderElection)
	enum { IDD = IDD_DLG_LEADER_ELECTION };
	COleDateTime	m_ApplicationDateStart;
	COleDateTime	m_ApplicationDateEnd;
	COleDateTime	m_ApplicationTimeStart;
	COleDateTime	m_ApplicationTimeEnd;
	COleDateTime	m_ElectionDate;
	COleDateTime	m_ElectionTime;
	COleDateTime	m_VoteDateEnd;
	COleDateTime	m_VoteDateStart;
	COleDateTime	m_VoteTimeEnd;
	COleDateTime	m_VoteTimeStart;
	CString	m_Election_State;
	int		m_TotalVoteOfNumber;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLeaderElection)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	CGridCtrl			m_GridVoteResultList;
	CODBCStatement		*m_pODBCStmtLeaderElection;
	BOOL				m_CheckAUTOUpdate;
	INT					m_TotalVoteCount;
	mtvectSLEADER_CANDIDATE		m_mtvectSLEADER_CANDIDATE;

	void				GetPollDateByDB();		// 2007-10-31 by dhjin, 선거 기간 정보 가져오기
	void				AllDisableWindowDate();		// 2007-11-01 by dhjin, 선거 기간 정보 전부 비활성화
	void				AllEnableWindowDate();		// 2007-11-01 by dhjin, 선거 기간 정보 전부 활성화
	void				InitGridVoteResultList();	// 2007-11-01 by dhjin, 선거 결과창 초기화
	void				InsertGridVoteResultList();	// 2007-11-01 by dhjin, 선거 결과창 보여주기

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CLeaderElection)
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnAutoEdit();
	afx_msg void OnBtnGmEdit();
	afx_msg void OnBtnReload();
	afx_msg void OnBtnOk();
	afx_msg void OnBtnResult();
	afx_msg void OnBtnSaveResult();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LEADERELECTION_H__0E12F658_41A2_46C2_A03B_5CD4913D3808__INCLUDED_)
