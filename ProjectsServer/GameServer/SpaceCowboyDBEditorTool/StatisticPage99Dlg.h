#if !defined(AFX_STATISTICPAGE99DLG_H__D1D1808C_37F8_429A_A274_5E2912E14E8B__INCLUDED_)
#define AFX_STATISTICPAGE99DLG_H__D1D1808C_37F8_429A_A274_5E2912E14E8B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StatisticPage99Dlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CStatisticPage99Dlg dialog

class CStatisticPage99Dlg : public CPropertyPage
{
	DECLARE_DYNCREATE(CStatisticPage99Dlg)

// Construction
public:
	CStatisticPage99Dlg();
	~CStatisticPage99Dlg();

	void ViewIndividualFameRanking(int i_nRankCount);

	CGridCtrl						m_gridView99;
	CODBCStatement					*m_pODBCStmt; //DB 접근 클래스 포인터
	CSpaceCowboyDBEditorToolDlg		*m_pMainDlg;

// Dialog Data
	//{{AFX_DATA(CStatisticPage99Dlg)
	enum { IDD = IDD_STA_SHEET_99 };
	int		m_ctl_nRankCount;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CStatisticPage99Dlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CStatisticPage99Dlg)
	afx_msg void OnBtnSearch();
	afx_msg void OnBtnSaveAsFile();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STATISTICPAGE99DLG_H__D1D1808C_37F8_429A_A274_5E2912E14E8B__INCLUDED_)
