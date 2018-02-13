// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__1C1DB24D_799E_4B04_AA18_E4FE0F4C01F4__INCLUDED_)
#define AFX_STDAFX_H__1C1DB24D_799E_4B04_AA18_E4FE0F4C01F4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#define _WIN32_WINNT 0x0501

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxcview.h>
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxinet.h>

// Local Header Files
#pragma warning(disable:4786)
#include <winbase.h>
#include <winsock2.h>
#include <timeapi.h>
#include <list>
#include <vector>
#include <map>
#include <queue>
#include <algorithm>
#include <string>
#include <fstream>
#include <functional>


#include "DbgOut_Monitor.h"
#include "mt_stl.h"

#include "DefineGlobal.h"
#include "GlobalGameServer.h"
#include "StringDefineQuery.h"			// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
#include "StringDefineProcedure.h"	// 2008-08-22 by cmkwon, MySQL 통합(Procedure 뽑아내기) - 


using namespace std;

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__1C1DB24D_799E_4B04_AA18_E4FE0F4C01F4__INCLUDED_)
