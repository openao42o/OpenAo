// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//  are changed infrequently
//

#pragma once

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers

#define _WIN32_WINNT 0x0501

#pragma warning(disable:4786)

#include <afxwin.h>
#include <afxdisp.h>

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

// Local Header Files
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

#include "DbgOut_Pre.h"
#include "mt_stl.h"

#include "DefineGlobal.h"
#include "GlobalGameServer.h"
#include "StringDefineQuery.h"			// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
#include "StringDefineProcedure.h"	// 2008-08-22 by cmkwon, MySQL 통합(Procedure 뽑아내기) - 

#include "CompileUtils.h"

using namespace std;
