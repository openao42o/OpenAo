// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__D5C60B9C_78F8_4240_A330_46A1E259127E__INCLUDED_)
#define AFX_STDAFX_H__D5C60B9C_78F8_4240_A330_46A1E259127E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <assert.h>

// Local Header Files
#pragma warning(disable:4786)
#include <Winbase.h>
#include <winsock2.h>
#include <list>
#include <vector>
#include <map>
#include <set>
#include <queue>
#include <algorithm>
#include <fstream>

#include <MMSYSTEM.H>
#include "AtumProtocol.h"
#include "WinPacket.h"

#include "DbgOut_C.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__D5C60B9C_78F8_4240_A330_46A1E259127E__INCLUDED_)
