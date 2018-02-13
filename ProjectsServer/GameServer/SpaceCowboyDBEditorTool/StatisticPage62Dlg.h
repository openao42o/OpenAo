#if !defined(AFX_STATISTICPAGE62DLG_H__B6F49376_944A_4CDC_A7BF_ED3E6F89C72F__INCLUDED_)
#define AFX_STATISTICPAGE62DLG_H__B6F49376_944A_4CDC_A7BF_ED3E6F89C72F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StatisticPage62Dlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CStatisticPage62Dlg dialog

class CStatisticPage62Dlg : public CPropertyPage
{
	DECLARE_DYNCREATE(CStatisticPage62Dlg)

// Construction
public:
	CStatisticPage62Dlg();
	~CStatisticPage62Dlg();

	void ViewRareItemRanking(int i_nPrefix, int i_nSuffix);
	void ViewRareItemTotalCount(int i_nPrefix, int i_nSuffix);

	CGridCtrl						m_gridView;
	CODBCStatement					*m_pODBCStmt; //DB 접근 클래스 포인터
	CSpaceCowboyDBEditorToolDlg		*m_pMainDlg;
	ez_map<INT, ITEM>				*m_pMapIteminfo;
	ez_map<INT, RARE_ITEM_INFO>		*m_pMapRareItemInfo;			// 2006-01-19 by cmkwon
	ez_map<string, RARE_ITEM_INFO>	*m_pMapRareItemInfoName;		// 2006-01-19 by cmkwon

// Dialog Data
	//{{AFX_DATA(CStatisticPage62Dlg)
	enum { IDD = IDD_STA_SHEET_62 };
	CComboBox	m_ctl_comboSuffix;
	CComboBox	m_ctl_comboPrefix;
	int		m_ctl_nTotalCount;
	CString	m_ctl_strPrefix;
	CString	m_ctl_strSuffix;
	int		m_ctl_nRankCount;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CStatisticPage62Dlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CStatisticPage62Dlg)
	afx_msg void OnBtnSaveAsFile();
	afx_msg void OnBtnSearch();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeComboPrefix();
	afx_msg void OnSelchangeComboSuffix();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STATISTICPAGE62DLG_H__B6F49376_944A_4CDC_A7BF_ED3E6F89C72F__INCLUDED_)
