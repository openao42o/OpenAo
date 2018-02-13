#if !defined(AFX_ACEONLINELAUNCHERPPG_H__A99DE8F2_5C61_46BB_B728_ADE42AE1BC5F__INCLUDED_)
#define AFX_ACEONLINELAUNCHERPPG_H__A99DE8F2_5C61_46BB_B728_ADE42AE1BC5F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// ACEonlineLauncherPpg.h : Declaration of the CACEonlineLauncherPropPage property page class.

////////////////////////////////////////////////////////////////////////////
// CACEonlineLauncherPropPage : See ACEonlineLauncherPpg.cpp.cpp for implementation.

class CACEonlineLauncherPropPage : public COlePropertyPage
{
	DECLARE_DYNCREATE(CACEonlineLauncherPropPage)
	DECLARE_OLECREATE_EX(CACEonlineLauncherPropPage)

// Constructor
public:
	CACEonlineLauncherPropPage();

// Dialog Data
	//{{AFX_DATA(CACEonlineLauncherPropPage)
	enum { IDD = IDD_PROPPAGE_ACEONLINELAUNCHER };
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Message maps
protected:
	//{{AFX_MSG(CACEonlineLauncherPropPage)
		// NOTE - ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ACEONLINELAUNCHERPPG_H__A99DE8F2_5C61_46BB_B728_ADE42AE1BC5F__INCLUDED)
