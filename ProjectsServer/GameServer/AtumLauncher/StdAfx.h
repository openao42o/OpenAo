// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__AC95B629_8AB3_4EC6_870C_E011B126D90F__INCLUDED_)
#define AFX_STDAFX_H__AC95B629_8AB3_4EC6_870C_E011B126D90F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#define _WIN32_WINNT 0x0501

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#include <afxtempl.h>
#endif // _AFX_NO_AFXCMN_SUPPORT

// Local Header Files
#pragma warning(disable:4786)
#include <Winbase.h>
#include <winsock2.h>
#include <list>
#include <vector>
#include <map>
#include <queue>
#include <algorithm>
#include <string>
#include <fstream>
#include <functional>
#include <VersionHelpers.h>

#include "DbgOut_c.h"
#include "mt_stl.h"

#include "DefineGlobal.h"
#include "GlobalGameServer.h"

using namespace std;

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__AC95B629_8AB3_4EC6_870C_E011B126D90F__INCLUDED_)
