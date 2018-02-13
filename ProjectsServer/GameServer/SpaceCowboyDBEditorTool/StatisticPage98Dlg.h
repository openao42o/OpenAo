#if !defined(AFX_STATISTICPAGE98DLG_H__8855B30D_2610_477B_82D9_245CA32B13BB__INCLUDED_)
#define AFX_STATISTICPAGE98DLG_H__8855B30D_2610_477B_82D9_245CA32B13BB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StatisticPage98Dlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CStatisticPage98Dlg dialog

class CStatisticPage98Dlg : public CPropertyPage
{
	DECLARE_DYNCREATE(CStatisticPage98Dlg)

// Construction
public:
	CStatisticPage98Dlg();
	~CStatisticPage98Dlg();

	void ViewGuildFameRanking(int i_nRankCount);

	CGridCtrl						m_gridView98;
	CODBCStatement					*m_pODBCStmt; //DB 접근 클래스 포인터
	CSpaceCowboyDBEditorToolDlg		*m_pMainDlg;

// Dialog Data
	//{{AFX_DATA(CStatisticPage98Dlg)
	enum { IDD = IDD_STA_SHEET_98 };
	int		m_ctl_nRankCount;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CStatisticPage98Dlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CStatisticPage98Dlg)
	afx_msg void OnBtnSearch();
	afx_msg void OnBtnSaveAsFile();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STATISTICPAGE98DLG_H__8855B30D_2610_477B_82D9_245CA32B13BB__INCLUDED_)
