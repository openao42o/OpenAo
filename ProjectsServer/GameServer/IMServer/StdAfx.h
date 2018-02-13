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

#define _WIN32_WINNT 0x0400

// Windows Header Files:
#include <windows.h>

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
#include <queue>
#include <algorithm>
#include <string>
#include <fstream>
#include <functional>


#include "DbgOut_IM.h"
#include "mt_stl.h"

#include "DefineGlobal.h"
#include "GlobalGameServer.h"
#include "AtumProtocol.h"
#include "StringDefineQuery.h"			// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
#include "StringDefineProcedure.h"	// 2008-08-22 by cmkwon, MySQL 통합(Procedure 뽑아내기) - 


using namespace std;


typedef mt_vector<SCALLGM>					mtvectSCALLGM;							// 2006-05-08 by cmkwon
typedef vector<ClientIndex_t>				vectClientIndex_t;			// 2007-01-31 by cmkwon


// TODO: reference additional headers your program requires here

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__A9DB83DB_A9FD_11D0_BFD1_444553540000__INCLUDED_)
