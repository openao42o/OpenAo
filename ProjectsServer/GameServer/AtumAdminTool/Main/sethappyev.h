#if !defined(AFX_SETHAPPYEV_H__AF27838C_06EE_4909_B04D_14B6488E7DD2__INCLUDED_)
#define AFX_SETHAPPYEV_H__AF27838C_06EE_4909_B04D_14B6488E7DD2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// sethappyev.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSetHappyEv dialog

class CSetHappyEv : public CDialog
{
// Construction
public:
	CSetHappyEv(BOOL i_bInsertHappyEv, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSetHappyEv)
	enum { IDD = IDD_DLG_SET_HAPPYEV };
	CComboBox	m_comboInfluenceTypeList;
	CComboBox	m_comboServerGroupID;
	CComboBox	m_comboDayOfWeek;
	COleDateTime	m_timeEvEnd;
	COleDateTime	m_timeEvStart;
	float	m_ctlfDropItemRate;
	float	m_ctlfDropRareItemRate;
	float	m_ctlfExpRate;
	float	m_ctlfExpRepairRate;
	float	m_ctlfSPIRate;
	float	m_ctlfWPRate;			// 2007-06-26 by dhjin, 워포인트 이벤트 관련 추가
	int		m_MinLevel;
	int		m_MaxLevel;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSetHappyEv)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	//}}AFX_VIRTUAL

public:
	SHAPPY_HOUR_EVENT		m_happyEv;

// Implementation
protected:
	BOOL					m_bInsertHappyEv;		// 2006-08-25 by cmkwon

	// Generated message map functions
	//{{AFX_MSG(CSetHappyEv)
	afx_msg void OnBtnDefaultValues();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETHAPPYEV_H__AF27838C_06EE_4909_B04D_14B6488E7DD2__INCLUDED_)
