#ifndef AFX_SPACECOWBOYDBEDITORTOOL_H__C7AC4910_D69A_4E4E_BF43_5876DBA0EA1E__INCLUDED_)
#define AFX_SPACECOWBOYDBEDITORTOOL_H__C7AC4910_D69A_4E4E_BF43_5876DBA0EA1E__INCLUDED_
#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols


class CSpaceCowboyDBEditorToolApp : public CWinApp
{
public:
	CSpaceCowboyDBEditorToolApp();

	CString						m_szAccountNameInputReg;

	virtual BOOL InitInstance();
	virtual int ExitInstance();

	DECLARE_MESSAGE_MAP()
};


#endif