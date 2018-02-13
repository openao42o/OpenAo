// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__A9DB83DB_A9FD_11D0_BFD1_444553540000__INCLUDED_)
#define AFX_STDAFX_H__A9DB83DB_A9FD_11D0_BFD1_444553540000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
//#include <afxwin.h>
#define DIRECTINPUT_VERSION 0x800
#define _WIN32_WINNT 0x0501
#include <windows.h>

//#define _WIN32_WINNT 0x0400
#pragma warning(disable:4786)
//#include <Winbase.h>
#include <tchar.h>
#include "DbgOut_C.h"

#include <map>
#include <string>
#include <vector>
using namespace std;
#include "SocketHeader.h"
#include "AtumParam.h"
#include "AtumProtocol.h"
#include "resource.h"
#include "ObjectDefine.h"
#include "AtumSJ.h"
#include "VoIPProtocol.h"
#include "Quest.h"
#include "FunctionLog.h"
#include "FunctionLogGFunctions.h"
#include "AtumTypedef.h"
#include "AtumDefine.h"
//#include "StringDefine.h"


#include "DebugAssert.h"
#include "AtumGlobal.h"
#include "AtumProtocolVoIP.h"
#include "HShield.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__A9DB83DB_A9FD_11D0_BFD1_444553540000__INCLUDED_)
