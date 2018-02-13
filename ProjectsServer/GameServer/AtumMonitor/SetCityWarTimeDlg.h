#if !defined(AFX_SETCITYWARTIMEDLG_H__FB3D46AF_C0F1_419C_A4A5_8708633A7BF7__INCLUDED_)
#define AFX_SETCITYWARTIMEDLG_H__FB3D46AF_C0F1_419C_A4A5_8708633A7BF7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SetCityWarTimeDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSetCityWarTimeDlg dialog
class CChannelInfoNode;
class CSetCityWarTimeDlg : public CDialog
{
// Construction
public:
	CSetCityWarTimeDlg(CChannelInfoNode * i_pChannelInfoNode, CWnd* pParent = NULL);   // standard constructor

public:
	ATUM_DATE_TIME		m_ATimeDefaultTime;
	ATUM_DATE_TIME		m_ATimeSettingTime;

public:
// Dialog Data
	//{{AFX_DATA(CSetCityWarTimeDlg)
	enum { IDD = IDD_DLG_SET_CITYWAR_TIME };
	CTime	m_ctlTime_DefaultDate;
	CTime	m_ctlTime_SettingDate;
	CTime	m_ctlTime_DefaultTime;
	CTime	m_ctlTime_SettingTime;
	CString	m_ctlStr_CityWarMapIndex;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSetCityWarTimeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSetCityWarTimeDlg)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETCITYWARTIMEDLG_H__FB3D46AF_C0F1_419C_A4A5_8708633A7BF7__INCLUDED_)
