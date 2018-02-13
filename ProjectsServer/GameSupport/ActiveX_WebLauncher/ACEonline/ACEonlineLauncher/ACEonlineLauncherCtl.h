#if !defined(AFX_ACEONLINELAUNCHERCTL_H__D463A4F9_A2FE_40E0_8989_7CF2AAB57C05__INCLUDED_)
#define AFX_ACEONLINELAUNCHERCTL_H__D463A4F9_A2FE_40E0_8989_7CF2AAB57C05__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// ACEonlineLauncherCtl.h : Declaration of the CACEonlineLauncherCtrl ActiveX Control class.

/////////////////////////////////////////////////////////////////////////////
// CACEonlineLauncherCtrl : See ACEonlineLauncherCtl.cpp for implementation.

class CACEonlineLauncherCtrl : public COleControl
{
	DECLARE_DYNCREATE(CACEonlineLauncherCtrl)

// Constructor
public:
	CACEonlineLauncherCtrl();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CACEonlineLauncherCtrl)
	public:
	virtual void OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid);
	virtual void DoPropExchange(CPropExchange* pPX);
	virtual void OnResetState();
	//}}AFX_VIRTUAL

	// 2012-08-13 by mspark, 에이스 온라인 ActiveX 작업 - 뮤텍스 작업
	HANDLE	m_hMutexMonoInstance;
	// end 2012-08-13 by mspark, 에이스 온라인 ActiveX 작업 - 뮤텍스 작업

// Implementation
protected:
	~CACEonlineLauncherCtrl();

	DECLARE_OLECREATE_EX(CACEonlineLauncherCtrl)    // Class factory and guid
	DECLARE_OLETYPELIB(CACEonlineLauncherCtrl)      // GetTypeInfo
	DECLARE_PROPPAGEIDS(CACEonlineLauncherCtrl)     // Property page IDs
	DECLARE_OLECTLTYPE(CACEonlineLauncherCtrl)		// Type name and misc status

// Message maps
	//{{AFX_MSG(CACEonlineLauncherCtrl)
		// NOTE - ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

// Dispatch maps
	//{{AFX_DISPATCH(CACEonlineLauncherCtrl)
		// NOTE - ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	// 2012-08-13 by mspark, 에이스 온라인 ActiveX 작업
	afx_msg short SetGameArgument(LPCTSTR szGameId, LPCTSTR szGamePassWord, LPCTSTR szSeed, LPCTSTR szGameType, LPCTSTR szAuthenticationKey, LPCTSTR szBirthday);
	// end 2012-08-13 by mspark, 에이스 온라인 ActiveX 작업
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()

	afx_msg void AboutBox();

// Event maps
	//{{AFX_EVENT(CACEonlineLauncherCtrl)
		// NOTE - ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_EVENT
	DECLARE_EVENT_MAP()

// Dispatch and event IDs
public:
	enum {
	//{{AFX_DISP_ID(CACEonlineLauncherCtrl)
		// NOTE: ClassWizard will add and remove enumeration elements here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DISP_ID
	};

private:
	// 2012-08-13 by mspark, 에이스 온라인 ActiveX 작업
	CString m_strACEonlineInstallPath;
	// end 2012-08-13 by mspark, 에이스 온라인 ActiveX 작업
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ACEONLINELAUNCHERCTL_H__D463A4F9_A2FE_40E0_8989_7CF2AAB57C05__INCLUDED)
