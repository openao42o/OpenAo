#if !defined(AFX_BLOCKACCOUNTDLG_H__9C2B5008_D48B_4C7B_9EDC_F91C1BEAF11F__INCLUDED_)
#define AFX_BLOCKACCOUNTDLG_H__9C2B5008_D48B_4C7B_9EDC_F91C1BEAF11F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BlockAccountDlg.h : header file
//

#include "AtumDateTime.h"
#include "AtumParam.h"

/////////////////////////////////////////////////////////////////////////////
// CBlockAccountDlg dialog


class CBlockAccountDlg : public CDialog
{
// Construction
public:
	CBlockAccountDlg(BOOL i_bBlocked, CString i_strAccName, SBLOCKED_ACCOUNT_INFO *i_pBlockedInfo, CWnd* pParent=NULL);   // standard constructor

	BOOL					m_bBlocked;
	ATUM_DATE_TIME			m_atimeStart;
	ATUM_DATE_TIME			m_atimeEnd;
	int						m_nBlockType;
	

// Dialog Data
	//{{AFX_DATA(CBlockAccountDlg)
	enum { IDD = IDD_DLG_BLOCK_ACCOUNT };
	CComboBox	m_ComboBlockType;
	CString	m_szBlockReason;
	CString	m_szBlockReasonForOnlyAdmin;
	CString	m_szUserName;
	COleDateTime	m_ctl_StartDate;
	COleDateTime	m_ctl_StartTime;
	COleDateTime	m_ctl_EndDate;
	COleDateTime	m_ctl_EndTime;
	CString	m_ctl_strAdminName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBlockAccountDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CBlockAccountDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnOk();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BLOCKACCOUNTDLG_H__9C2B5008_D48B_4C7B_9EDC_F91C1BEAF11F__INCLUDED_)
