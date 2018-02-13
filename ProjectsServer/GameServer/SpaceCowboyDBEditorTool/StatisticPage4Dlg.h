#if !defined(AFX_STATISTICPAGE4DLG_H__48D9FDFE_DD91_479A_8041_E4BC4C198D18__INCLUDED_)
#define AFX_STATISTICPAGE4DLG_H__48D9FDFE_DD91_479A_8041_E4BC4C198D18__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StatisticPage4Dlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CStatisticPage4Dlg dialog

class CStatisticPage4Dlg : public CPropertyPage
{
	DECLARE_DYNCREATE(CStatisticPage4Dlg)

// Construction
public:
	void ViewRanking(void);
	void ViewRankingByCreatedDate(void);

	void ViewDistributionByLastLoginDate(void);
	void ViewDistributionByCreatedDate(void);

	CStatisticPage4Dlg();
	~CStatisticPage4Dlg();
	CGridCtrl m_GridStaGearLevelDistribution;
	CODBCStatement					*m_pODBCStmt; //DB 접근 클래스 포인터
	CSpaceCowboyDBEditorToolDlg		*m_pMainDlg; 

// Dialog Data
	//{{AFX_DATA(CStatisticPage4Dlg)
	enum { IDD = IDD_STA_SHEET_4 };
	int		m_ctl_nRankCount;
	COleDateTime	m_ctl_Date;
	COleDateTime	m_ctl_Time;
	COleDateTime	m_ctl_CreatedDateStart;
	COleDateTime	m_ctl_CreatedTimeStart;
	COleDateTime	m_ctl_CreatedDateEnd;
	COleDateTime	m_ctl_CreatedTimeEnd;
	BOOL	m_ctl_bCheckLastLoginDate;
	BOOL	m_ctl_bByCreated;
	BOOL	m_ctl_bDistChart;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CStatisticPage4Dlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CStatisticPage4Dlg)
	afx_msg void OnButtonSt4Select();
	afx_msg void OnButtonSt4Save();
	afx_msg void OnCheck1();
	afx_msg void OnCheck2();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STATISTICPAGE4DLG_H__48D9FDFE_DD91_479A_8041_E4BC4C198D18__INCLUDED_)
