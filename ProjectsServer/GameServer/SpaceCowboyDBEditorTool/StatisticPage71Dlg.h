#if !defined(AFX_STATISTICPAGE71DLG_H__B0609196_CD29_4CAB_AD4F_7925E85B3198__INCLUDED_)
#define AFX_STATISTICPAGE71DLG_H__B0609196_CD29_4CAB_AD4F_7925E85B3198__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StatisticPage71Dlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CStatisticPage71Dlg dialog

class CStatisticPage71Dlg : public CPropertyPage
{
	DECLARE_DYNCREATE(CStatisticPage71Dlg)

// Construction
public:
	CStatisticPage71Dlg();
	~CStatisticPage71Dlg();

	void ViewPvPRanking(int i_nRankCount);


	CGridCtrl						m_gridView71;
	CODBCStatement					*m_pODBCStmt; //DB 접근 클래스 포인터
	CSpaceCowboyDBEditorToolDlg		*m_pMainDlg;

// Dialog Data
	//{{AFX_DATA(CStatisticPage71Dlg)
	enum { IDD = IDD_STA_SHEET_71 };
	int		m_ctl_nRankCount;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CStatisticPage71Dlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CStatisticPage71Dlg)
	afx_msg void OnBtnSearch();
	afx_msg void OnBtnSaveAsFile();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STATISTICPAGE71DLG_H__B0609196_CD29_4CAB_AD4F_7925E85B3198__INCLUDED_)
