#if !defined(AFX_SELECTITEMDLG_H__616D97F7_BAC5_4289_B252_05E180B3BD1F__INCLUDED_)
#define AFX_SELECTITEMDLG_H__616D97F7_BAC5_4289_B252_05E180B3BD1F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SelectItemDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSelectItemDlg dialog

class CSelectItemDlg : public CDialog
{
	
// Construction
public:
	CSelectItemDlg(ez_map<INT, ITEM> *i_pMapItemInfo, ez_map<string, ITEM> *i_pMapItemInfoName, BOOL i_nEnableCount, CWnd* pParent = NULL);   // standard constructor


	///////////////////////////////////////////////////////////////////////////////
	// 2008-06-03 by cmkwon, AdminTool, DBTool 사용시 아이템 검색시 콤보박스에서 검색 기능 추가(K0000143) - 
	BYTE GetSelectedItemKindFromItemKindComboBox(void);
	BOOL ResetItemInfoList(BYTE i_byItemKind, CString *i_pcsPartOfItemName=NULL);

// Dialog Data
	//{{AFX_DATA(CSelectItemDlg)
	enum { IDD = IDD_DIALOG_SELECT_ITEM };
	int		m_nItemCount;
	CComboBox	m_ctl_comboItemNameList;
	CComboBox	m_ctl_comboItemKindList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSelectItemDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	ez_map<INT, ITEM>		*m_pMapItemInfo;
	ez_map<string, ITEM>	*m_pMapItemInfoName;

public:
	INT						m_nSelectedItemNum;
	CString					m_szSelectedItemName;
	BOOL					m_bEnableCount;

	// Generated message map functions
	//{{AFX_MSG(CSelectItemDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeComboItemKind();
	afx_msg void OnSelchangeComboItem();
	afx_msg void OnOk();
	afx_msg void OnDropdownComboItem();
	afx_msg void OnEditchangeComboItem();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SELECTITEMDLG_H__616D97F7_BAC5_4289_B252_05E180B3BD1F__INCLUDED_)
