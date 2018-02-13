// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
// #include <windows.h>
#include <afxwin.h>
#include <Afxdisp.h>

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>


#include "../Common/ErrorDefine.h"
#include "../Common/mt_stl.h"
#include "../Common/LogManager.h"
#include "../Common/GLogDefine.h"
#include "../Common/GLogDefineQuery.h"
#include "../Common/Config.h"
#include "../Common/XOR_Lib.h"
#include "GLogGlobal.h"
#include "GLogSourceDB.h"
#include "GLogDestDB.h"
#include "GLogManager.h"




// TODO: reference additional headers your program requires here
