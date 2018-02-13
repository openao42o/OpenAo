#if !defined(AFX_ACEONLINELAUNCHER_H__D77AF2E3_AEA7_4DC4_A5D0_3A11081A7629__INCLUDED_)
#define AFX_ACEONLINELAUNCHER_H__D77AF2E3_AEA7_4DC4_A5D0_3A11081A7629__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// ACEonlineLauncher.h : main header file for ACEONLINELAUNCHER.DLL

#if !defined( __AFXCTL_H__ )
	#error include 'afxctl.h' before including this file
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CACEonlineLauncherApp : See ACEonlineLauncher.cpp for implementation.

class CACEonlineLauncherApp : public COleControlModule
{
public:
	BOOL InitInstance();
	int ExitInstance();
};

extern const GUID CDECL _tlid;
extern const WORD _wVerMajor;
extern const WORD _wVerMinor;

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ACEONLINELAUNCHER_H__D77AF2E3_AEA7_4DC4_A5D0_3A11081A7629__INCLUDED)
