#if !defined(AFX_DLGMSGBOX_H__2AAF3BD0_7D11_4572_B6DC_3D811E2B4CF4__INCLUDED_)
#define AFX_DLGMSGBOX_H__2AAF3BD0_7D11_4572_B6DC_3D811E2B4CF4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// dlgmsgbox.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgMsgBox dialog

///////////////////////////////////////////////////////////////////////////////
/// \class		
///
/// \brief		// 2009-11-24 by cmkwon, 월드랭킹 실행시 질문 메시지박스 수정 - 
/// \author		cmkwon
/// \version	
/// \date		2009-11-24 ~ 2009-11-24
/// \warning	
///////////////////////////////////////////////////////////////////////////////
class CDlgMsgBox : public CDialog
{
// Construction
public:
	CDlgMsgBox(CWnd* pParent = NULL);   // standard constructor

	BOOL GetIsForMainServer(void);
	BOOL			m_bIsForMainServer;		// 2009-11-24 by cmkwon, 월드랭킹 실행시 질문 메시지박스 수정 - 

// Dialog Data
	//{{AFX_DATA(CDlgMsgBox)
	enum { IDD = IDD_DLG_MSGBOX };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgMsgBox)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgMsgBox)
	afx_msg void OnBtnForTestserver();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGMSGBOX_H__2AAF3BD0_7D11_4572_B6DC_3D811E2B4CF4__INCLUDED_)
