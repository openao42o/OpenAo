
// CrashReporter.h: Hauptheaderdatei für die PROJECT_NAME-Anwendung
//

#pragma once

#ifndef __AFXWIN_H__
	#error "'stdafx.h' vor dieser Datei für PCH einschließen"
#endif

#include "resource.h"		// Hauptsymbole


// CCrashReporterApp:
// Siehe CrashReporter.cpp für die Implementierung dieser Klasse
//

class CCrashReporterApp : public CWinApp
{
public:
	CCrashReporterApp();

// Überschreibungen
public:
	virtual BOOL InitInstance();

// Implementierung

	DECLARE_MESSAGE_MAP()
};

extern CCrashReporterApp theApp;