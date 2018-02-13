#include "Stdafx.h"
#include "global.h"

CGlobal *			g_pGlobal = NULL;


CSystemLogManager	CGlobal::ms_SystemLogManager;
///////////////////////////////////////////////////////////////////////////////
// Method
///////////////////////////////////////////////////////////////////////////////
CGlobal::CGlobal()
{
	m_hMutexMonoInstance = NULL;
	if(g_pGlobal)
	{
		return;
	}

	g_pGlobal = this;
}

CGlobal::~CGlobal()
{
	if(m_hMutexMonoInstance)
	{
		::CloseHandle(m_hMutexMonoInstance);
		m_hMutexMonoInstance = NULL;
	}
}


///////////////////////////////////////////////////////////////////////////////
// Property
///////////////////////////////////////////////////////////////////////////////
CSystemLogManager * CGlobal::GetSystemLogManagerPtr(void)
{
	return &ms_SystemLogManager;
}


///////////////////////////////////////////////////////////////////////////////
// Method
///////////////////////////////////////////////////////////////////////////////

BOOL CGlobal::InitGlobal(char * i_szServerName)
{
	if(m_hMutexMonoInstance)
	{
		return FALSE;
	}

	m_hMutexMonoInstance = CreateMutex(NULL, TRUE, i_szServerName);
	if(NULL == m_hMutexMonoInstance)
	{
		MessageBox(NULL, "CreateMutex Error", i_szServerName, MB_OK);
		return FALSE;
	}

	if(ERROR_ALREADY_EXISTS == ::GetLastError())
	{
		MessageBox(NULL, "ERROR : \nApplication is running already...", i_szServerName, MB_OK);
		return FALSE;
	}
	return TRUE;
}


BOOL CGlobal::WriteSystemLog(char *szLogMsg, BOOL bTimeHeaderFlag)
{
	return ms_SystemLogManager.WriteSystemLog(szLogMsg, bTimeHeaderFlag);
}

BOOL CGlobal::WriteSystemLogEX(BOOL bPrintDBGOUT, char* pFormat, ...)
{
	char szLogMsg[1024];
	va_list args;
	va_start(args, pFormat);
	vsprintf(szLogMsg, pFormat, args);

	if (bPrintDBGOUT)
	{
		DBGOUT(szLogMsg);
	}

	return ms_SystemLogManager.WriteSystemLog(szLogMsg, TRUE);
}
