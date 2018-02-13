// SCBitFlag.h : main header file for the SCBitFlag application
//

#if !defined(AFX_SCBitFlag_H__3641A0F5_9D36_40C4_8833_8325156AFE01__INCLUDED_)
#define AFX_SCBitFlag_H__3641A0F5_9D36_40C4_8833_8325156AFE01__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CSCBitFlagApp:
// See SCBitFlag.cpp for the implementation of this class
//

class CSCBitFlagApp : public CWinApp
{
public:
	CSCBitFlagApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSCBitFlagApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CSCBitFlagApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCBitFlag_H__3641A0F5_9D36_40C4_8833_8325156AFE01__INCLUDED_)
