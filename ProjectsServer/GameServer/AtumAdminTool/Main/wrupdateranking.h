#if !defined(AFX_WRUPDATERANKING_H__330C9DE6_A772_415B_9FD8_8DE29179B371__INCLUDED_)
#define AFX_WRUPDATERANKING_H__330C9DE6_A772_415B_9FD8_8DE29179B371__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// wrupdateranking.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CWRUpdateRanking dialog
class CWRankingManagement;
class CWRUpdateRanking : public CDialog
{
// Construction
public:
	CWRUpdateRanking(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CWRUpdateRanking)
	enum { IDD = IDD_DLG_WRK_UPDATE_RANKING };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWRUpdateRanking)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	CWRankingManagement			*m_pWRankManager;

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CWRUpdateRanking)
	afx_msg void OnBtnUpdateService();
	afx_msg void OnBtnArrangeLocalRanking();
	afx_msg void OnBtnUpdateWranking();
	afx_msg void OnBtnDownloadWranking();
	afx_msg void OnBtnReloadWranking();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WRUPDATERANKING_H__330C9DE6_A772_415B_9FD8_8DE29179B371__INCLUDED_)
