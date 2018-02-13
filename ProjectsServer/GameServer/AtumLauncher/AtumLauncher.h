#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

class CAtumLauncherDlg;

class CAtumLauncherApp : public CWinApp
{
public:
	CAtumLauncherApp();
	static void ReadProfile(CAtumLauncherDlg& instance);
	void WriteProfile(CAtumLauncherDlg& instance);

	static void RemainingProcessRemove();

	HANDLE m_hMutexMonoInstance;


	virtual BOOL InitInstance();
	virtual int ExitInstance();


	DECLARE_MESSAGE_MAP()
};
