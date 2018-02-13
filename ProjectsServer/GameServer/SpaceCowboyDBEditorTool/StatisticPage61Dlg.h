#if !defined(AFX_STATISTICPAGE61DLG_H__98C2E4F3_54CD_4907_9649_3C088FFD0BF3__INCLUDED_)
#define AFX_STATISTICPAGE61DLG_H__98C2E4F3_54CD_4907_9649_3C088FFD0BF3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StatisticPage61Dlg.h : header file
//


///////////////////////////////////////////////////////////////////////////////
/// \class		CStatisticPage61Dlg
///
/// \brief		
/// \author		cmkwon
/// \version	
/// \date		2006-01-19 ~ 2006-01-19
/// \warning	
///////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// CStatisticPage61Dlg dialog

class CStatisticPage61Dlg : public CPropertyPage
{
	DECLARE_DYNCREATE(CStatisticPage61Dlg)

// Construction
public:
	CStatisticPage61Dlg();   // standard constructor

	void ViewCountableItemOwnerList(INT i_ItemNum);
	void ViewNonCountableItemOwnerList(INT i_ItemNum);
	void ViewCountableItemTotalCount(INT i_ItemNum);
	void ViewNonCountableItemTotalCount(INT i_ItemNum);

	///////////////////////////////////////////////////////////////////////////////
	// 2008-06-03 by cmkwon, AdminTool, DBTool 사용시 아이템 검색시 콤보박스에서 검색 기능 추가(K0000143) - 
	BYTE GetSelectedItemKindFromItemKindComboBox(void);
	BOOL ResetItemInfoList(BYTE i_byItemKind, CString *i_pcsPartOfItemName=NULL);


	CGridCtrl						m_gridView;
	CODBCStatement					*m_pODBCStmt; //DB 접근 클래스 포인터
	CSpaceCowboyDBEditorToolDlg		*m_pMainDlg;
	ez_map<INT, ITEM>				*m_pMapItemInfo;
	ez_map<string, ITEM>			*m_pMapItemInfoName;



	
// Dialog Data
	//{{AFX_DATA(CStatisticPage61Dlg)
	enum { IDD = IDD_STA_SHEET_61 };
	CComboBox	m_ctl_comboItemNameList;
	CComboBox	m_ctl_comboItemKindList;
	double	m_ctl_doTotalCount;
	CString	m_ctl_strItemInfo;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStatisticPage61Dlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CStatisticPage61Dlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeComboItemKind();
	afx_msg void OnSelchangeComboItemName();
	afx_msg void OnBtnSearch();
	afx_msg void OnBtnSaveAsFile();
	afx_msg void OnDropdownComboItemName();
	afx_msg void OnEditchangeComboItemName();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STATISTICPAGE61DLG_H__98C2E4F3_54CD_4907_9649_3C088FFD0BF3__INCLUDED_)
