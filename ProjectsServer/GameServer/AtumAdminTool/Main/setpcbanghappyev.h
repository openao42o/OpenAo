#if !defined(AFX_SETPCBANGHAPPYEV_H__895B7D0E_DCAE_45E6_8A29_A7B84A0C781A__INCLUDED_)
#define AFX_SETPCBANGHAPPYEV_H__895B7D0E_DCAE_45E6_8A29_A7B84A0C781A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// setpcbanghappyev.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSetPCBangHappyEv dialog

class CSetPCBangHappyEv : public CDialog
{
// Construction
public:
	CSetPCBangHappyEv(SHAPPY_HOUR_EVENT *i_pHappyEv, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSetPCBangHappyEv)
	enum { IDD = IDD_DLG_SET_PCBANG_HAPPYEV };
		// NOTE: the ClassWizard will add data members here
	COleDateTime	m_OleDatePCBangStart;
	COleDateTime	m_OleTimePCBangStart;
	COleDateTime	m_OleDatePCBangEnd;
	COleDateTime	m_OleTimePCBangEnd;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSetPCBangHappyEv)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

public:
	SHAPPY_HOUR_EVENT			m_pcBangEv;

// Implementation
protected:


	// Generated message map functions
	//{{AFX_MSG(CSetPCBangHappyEv)
	afx_msg void OnBtnDefaultValues();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETPCBANGHAPPYEV_H__895B7D0E_DCAE_45E6_8A29_A7B84A0C781A__INCLUDED_)
