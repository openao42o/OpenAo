#if !defined(AFX_STATISTICPAGE5DLG_H__F8BC45A8_11D3_4611_BFDE_0B89A74A0AA0__INCLUDED_)
#define AFX_STATISTICPAGE5DLG_H__F8BC45A8_11D3_4611_BFDE_0B89A74A0AA0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StatisticPage5Dlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CStatisticPage5Dlg dialog

class CStatisticPage5Dlg : public CPropertyPage
{
	DECLARE_DYNCREATE(CStatisticPage5Dlg)

// Construction
public:
//	void ViewCashItemBuy();
//	void ViewCashItemSell();
	CStatisticPage5Dlg();
	~CStatisticPage5Dlg();
	CGridCtrl m_GridStaCashItem;
	CODBCStatement					*m_pODBCStmt1; //DB 접근 클래스 포인터
	CSpaceCowboyDBEditorToolDlg		*m_pMainDlg; 
	ez_map<INT, ITEM>				*m_pMapIteminfo;

	typedef struct _m_data
	{
		int		m_ItemNum;
		CString m_ItemName;
		int		m_SellCount;
		int		m_SellCash;
		int		m_TotalSellCash;
	};

// Dialog Data
	//{{AFX_DATA(CStatisticPage5Dlg)
	enum { IDD = IDD_STA_SHEET_5 };
	int		m_TotalSellCash;
	COleDateTime	m_ctl_CreatedDateStart;
	COleDateTime	m_ctl_CreateTimeStart;
	COleDateTime	m_ctl_CreateDateEnd;
	COleDateTime	m_ctl_CreateTimeEnd;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CStatisticPage5Dlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CStatisticPage5Dlg)
	afx_msg void OnButtonSt5Select();
	afx_msg void OnButtonSt5Save();
//	afx_msg void OnCheck1();
//	afx_msg void OnCheck2();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STATISTICPAGE5DLG_H__F8BC45A8_11D3_4611_BFDE_0B89A74A0AA0__INCLUDED_)
