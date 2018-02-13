#if !defined(AFX_PREMIUMINFODLG_H__1497DD93_C681_4C97_8169_94692C46110B__INCLUDED_)
#define AFX_PREMIUMINFODLG_H__1497DD93_C681_4C97_8169_94692C46110B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PremiumInfoDlg.h : header file
//
#include "AtumParam.h"

#define STR_NORMAL_PREMIUM_CARD			"Normal Premium"
#define STR_SUPER_PREMIUM_CARD			"Super Premium"
#define STR_UNKNOWN_PREMIUM_CARD		"None"

struct SPREMIUM_INFO
{
	UID32_t			u64PremiumUID;
	INT				nItemNum;
	ATUM_DATE_TIME	atimeCreated;
	ATUM_DATE_TIME	atimeUpdated;
	ATUM_DATE_TIME	atimeExpire;
};
/////////////////////////////////////////////////////////////////////////////
// CPremiumInfoDlg dialog
class CODBCStatement;
class CPremiumInfoDlg : public CDialog
{
// Construction
public:
	CPremiumInfoDlg(BOOL i_bEnableEdit, CString i_strAccName, UID32_t i_u32AccUID, CODBCStatement *i_pODBC, CWnd* pParent=NULL);   // standard constructor

	BOOL LoadPremiumInfoBYAccountUID(SPREMIUM_INFO *o_pPremiumInfo, UID32_t i_accUID);

	char *GetStringPremiumItemNameByItemNum(INT i_nItemNum);
	INT GetItemNumByPremiumItemName(char *i_szPremiumItem);
// Dialog Data
	//{{AFX_DATA(CPremiumInfoDlg)
	enum { IDD = IDD_DLG_PREMIUM };
	CComboBox	m_ctlComboBoxPremiumItem;
	CButton	m_ctlBtnDelete;
	CButton	m_ctlBtnUpdate;
	CButton	m_ctlBtnInsert;
	CString	m_ctl_strAccountName;
	CTime	m_ctimeCreatedDate;
	CTime	m_ctimeCreatedTime;
	CTime	m_ctimeExpireDate;
	CTime	m_ctimeExpireTime;
	CTime	m_ctimeUpdatedDate;
	CTime	m_ctimeUpdatedTime;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPremiumInfoDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	BOOL				m_bEnableEdit;		// 2006-04-15 by cmkwon
// Implementation
protected:
	CODBCStatement		*m_pODBCStmt;
	UID32_t				m_uidAccountUID;
	SPREMIUM_INFO		m_premiumInfo;

	// Generated message map functions
	//{{AFX_MSG(CPremiumInfoDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnReload();
	afx_msg void OnBtnInsert();
	afx_msg void OnBtnDelete();
	afx_msg void OnBtnUpdate();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PREMIUMINFODLG_H__1497DD93_C681_4C97_8169_94692C46110B__INCLUDED_)
