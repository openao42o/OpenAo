#if !defined(AFX_SETEVENTMONSTERDLG_H__10B31E06_7303_4D90_B4BB_7E7C087FA145__INCLUDED_)
#define AFX_SETEVENTMONSTERDLG_H__10B31E06_7303_4D90_B4BB_7E7C087FA145__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// seteventmonsterdlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSetEventMonsterDlg dialog

class CSetEventMonsterDlg : public CDialog
{
// Construction
public:
	CSetEventMonsterDlg(SEVENT_MONSTER *i_pEvMon, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSetEventMonsterDlg)
	enum { IDD = IDD_DLG_EVENT_MONSTER };
	CComboBox	m_comboServerGroupID;
	COleDateTime	m_odtStartDate;
	COleDateTime	m_odtStartTime;
	COleDateTime	m_odtEndDate;
	COleDateTime	m_odtEndTime;
	int		m_ctlnMapIndex;
	int		m_ctlnReqMinLevel;
	int		m_ctlnReqMaxLevel;
	BOOL	m_ctlbExceptObjMon;
	BOOL	m_ctlbExceptInflMon;
	BOOL	m_ctlbExceptNotAttackMon;
	int		m_ctlnMonsterNum;
	int		m_ctlnMonsterCount;
	int		m_ctlnDelayTime;
	int		m_ctlnProbability;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSetEventMonsterDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	SEVENT_MONSTER			*m_pEventMonser;

	// Generated message map functions
	//{{AFX_MSG(CSetEventMonsterDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETEVENTMONSTERDLG_H__10B31E06_7303_4D90_B4BB_7E7C087FA145__INCLUDED_)
