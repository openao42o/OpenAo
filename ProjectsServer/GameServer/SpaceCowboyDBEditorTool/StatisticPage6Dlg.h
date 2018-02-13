#if !defined(AFX_STATISTICPAGE6DLG_H__37A784F3_0112_4DF4_B8D1_E6B3573941F4__INCLUDED_)
#define AFX_STATISTICPAGE6DLG_H__37A784F3_0112_4DF4_B8D1_E6B3573941F4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StatisticPage6Dlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CStatisticPage6Dlg dialog

class CStatisticPage6Dlg : public CPropertyPage
{
	DECLARE_DYNCREATE(CStatisticPage6Dlg)

// Construction
public:
// 2006-01-18 by cmkwon
//	void ViewAvgMissionClear();
//	void ViewEnchantCardAvg();
//	void ViewMultiShotItem();
//	void ViewEnchantAvg();
//	void ViewRareItemAvg();
//	void ViewUniAvgMoney();
	void ViewMoneyRankByCharacter(void);
	void ViewMoneyRankByAccount(void);
	void ViewTotalMoney(void);


	CStatisticPage6Dlg();
	~CStatisticPage6Dlg();
	CGridCtrl m_GridStaItemPossess;
	CODBCStatement					*m_pODBCStmt; //DB 접근 클래스 포인터
	CSpaceCowboyDBEditorToolDlg		*m_pMainDlg; 

// Dialog Data
	//{{AFX_DATA(CStatisticPage6Dlg)
	enum { IDD = IDD_STA_SHEET_6 };
	BOOL	m_ctl_bCharacterRank;
	BOOL	m_ctl_bAccountRank;
	int		m_ctl_nRankCount;
	double	m_ctl_doTotalMoney;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CStatisticPage6Dlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CStatisticPage6Dlg)
	afx_msg void OnButtonSt6Select();
	afx_msg void OnButtonSt5Save();
	afx_msg void OnCheckAccount();
	afx_msg void OnCheckCharacter();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STATISTICPAGE6DLG_H__37A784F3_0112_4DF4_B8D1_E6B3573941F4__INCLUDED_)
