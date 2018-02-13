// AtumAdminTool.h : main header file for the ATUMADMINTOOL application
//

#if !defined(AFX_ATUMADMINTOOL_H__B553826A_8427_4F0A_BFBC_1639A37970D7__INCLUDED_)
#define AFX_ATUMADMINTOOL_H__B553826A_8427_4F0A_BFBC_1639A37970D7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

#ifndef SIZE_MAP_TILE_SIZE
#define SIZE_MAP_TILE_SIZE 40
#endif

/////////////////////////////////////////////////////////////////////////////
// CAtumAdminToolApp:
// See AtumAdminTool.cpp for the implementation of this class
//

class CAtumAdminToolApp : public CWinApp
{
public:
	CAtumAdminToolApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAtumAdminToolApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	CString						m_szAccountNameInputReg;	// CSCUserAdminToolDlg

	char *GetPublicLocalIP(char *o_szLocalIP);


	void ReadProfile();
	void WriteProfile();

	//{{AFX_MSG(CAtumAdminToolApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ATUMADMINTOOL_H__B553826A_8427_4F0A_BFBC_1639A37970D7__INCLUDED_)
