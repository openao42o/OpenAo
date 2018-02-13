// Atum_LoadGen.h : main header file for the ATUM_LOADGEN application
//

#if !defined(AFX_ATUM_LOADGEN_H__B627D88D_9B61_44B9_99DE_A3EEBE78AD51__INCLUDED_)
#define AFX_ATUM_LOADGEN_H__B627D88D_9B61_44B9_99DE_A3EEBE78AD51__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CAtum_LoadGenApp:
// See Atum_LoadGen.cpp for the implementation of this class
//

class CAtum_LoadGenApp : public CWinApp
{
public:
	CAtum_LoadGenApp();

	void ReadProfile();
	void WriteProfile();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAtum_LoadGenApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CAtum_LoadGenApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ATUM_LOADGEN_H__B627D88D_9B61_44B9_99DE_A3EEBE78AD51__INCLUDED_)
