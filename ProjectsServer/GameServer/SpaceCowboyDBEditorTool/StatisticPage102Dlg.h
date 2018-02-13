#if !defined(AFX_STATISTICPAGE102DLG_H__DD253C12_552B_4912_826C_9F770B21EC90__INCLUDED_)
#define AFX_STATISTICPAGE102DLG_H__DD253C12_552B_4912_826C_9F770B21EC90__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StatisticPage102Dlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CStatisticPage102Dlg dialog

class CStatisticPage102Dlg : public CPropertyPage
{
	DECLARE_DYNCREATE(CStatisticPage102Dlg)

// Construction
public:
	CStatisticPage102Dlg();
	~CStatisticPage102Dlg();

	void ViewWarPointRanking(int i_nWarPointRankCount);

	CGridCtrl						m_gridView102;
	CODBCStatement					*m_pODBCStmt; //DB 접근 클래스 포인터
	CSpaceCowboyDBEditorToolDlg		*m_pMainDlg;

	BOOL m_RadioWP;
	BOOL m_RadioCWP;

// Dialog Data
	//{{AFX_DATA(CStatisticPage102Dlg)
	enum { IDD = IDD_STA_SHEET_102 };
	int		m_edit_Rank_Count;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CStatisticPage102Dlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CStatisticPage102Dlg)
	afx_msg void OnBtnSearch();
	afx_msg void OnBtnSaveAsFile();
	afx_msg void OnRadioWp();
	afx_msg void OnRadioCwp();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STATISTICPAGE102DLG_H__DD253C12_552B_4912_826C_9F770B21EC90__INCLUDED_)
