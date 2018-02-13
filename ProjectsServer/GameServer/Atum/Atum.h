#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

class CAtumDlg;

class CAtumApp : public CWinApp
{
public:
	CAtumApp();
	static void ReadProfile(CAtumDlg& instance);
	void WriteProfile(CAtumDlg& instance);

	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};

