// FunctionLog.cpp: implementation of the CFunctionLog class.
//
//////////////////////////////////////////////////////////////////////
#ifdef _FUNCTION_LOG

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
//#include <afxwin.h>
#define DIRECTINPUT_VERSION 0x800
#define _WIN32_WINNT 0x0400
#include <windows.h>


#include "FunctionLog.h"
#include "FunctionLogGFunctions.h"
#include <tchar.h>
#include "DbgOut_c.h"

static int g_nFunctionDepth = 0;

CFunctionLog::CFunctionLog(char* szName)
{
	m_szFunctionName = szName;
	wsprintf(m_szFunctionLog, "IN	[%s] [%d]\n", m_szFunctionName, ++g_nFunctionDepth);
	GWriteFunctionLogSharedMemory( m_szFunctionLog, sizeof(m_szFunctionLog));
}

CFunctionLog::~CFunctionLog()
{
	wsprintf(m_szFunctionLog, "OUT	[%s] [%d]\n", m_szFunctionName, g_nFunctionDepth--);
	GWriteFunctionLogSharedMemory( m_szFunctionLog, sizeof(m_szFunctionLog));
	m_szFunctionName = NULL;
}


#endif // _FUNCTION_LOG_endif