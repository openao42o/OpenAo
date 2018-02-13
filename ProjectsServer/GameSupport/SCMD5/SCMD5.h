// SCMD5.h : main header file for the SCMD5 DLL
//

#if !defined(AFX_SCMD5_H__4F24EB81_5585_4401_A6CD_BCC569EB771E__INCLUDED_)
#define AFX_SCMD5_H__4F24EB81_5585_4401_A6CD_BCC569EB771E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CSCMD5App
// See SCMD5.cpp for the implementation of this class
//

class CSCMD5App : public CWinApp
{
public:
	CSCMD5App();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSCMD5App)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CSCMD5App)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCMD5_H__4F24EB81_5585_4401_A6CD_BCC569EB771E__INCLUDED_)
