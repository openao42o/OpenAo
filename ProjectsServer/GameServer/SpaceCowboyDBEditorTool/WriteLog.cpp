// WriteLog.cpp: implementation of the CWriteLog class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "WriteLog.h"
#include "SpaceCowboyDBEditorToolDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWriteLog::CWriteLog()
{
	::InitializeCriticalSection(&m_csLog);
}

CWriteLog::~CWriteLog()
{
	::DeleteCriticalSection(&m_csLog);
}

int CWriteLog::WriteLogFile(char *data, int size, CSpaceCowboyDBEditorToolDlg* pDlg, int type)
{
	::EnterCriticalSection(&m_csLog);

	SYSTEMTIME Er_systime;
	GetLocalTime(&Er_systime);

	char curDir[200];
	memset(curDir,0,sizeof(curDir));
	GetCurrentDirectory(200, curDir);

	char cur_Er_logfilename[255];
	memset(cur_Er_logfilename,0,sizeof(cur_Er_logfilename));
	if(type == 0)
	{
		wsprintf(cur_Er_logfilename, ".\\QueryLog(SpaceCowboyDBEditor)_%04d%02d%02d_%02d.txt",
			Er_systime.wYear, Er_systime.wMonth, Er_systime.wDay, Er_systime.wHour);
	}
	else
	{
		wsprintf(cur_Er_logfilename, ".\\StatisticLog(SpaceCowboyDBEditor)_%04d%02d%02d_%02d.txt",
			Er_systime.wYear, Er_systime.wMonth, Er_systime.wDay, Er_systime.wHour);
	}
	HANDLE ER_handle = CreateFile(cur_Er_logfilename, GENERIC_WRITE, FILE_SHARE_READ, NULL,
		OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	DWORD writtenBytes;
	char crt[2];
	char buf[255];
	ZeroMemory(buf,sizeof(buf));
	memset(crt,0,sizeof(crt));
	strncpy(crt, "\r\n",2);

	SetFilePointer(ER_handle,0,NULL,FILE_END);

	if(type == 0)
	{
		wsprintf(buf, "[%04d-%02d-%02d %02d:%02d:%02d] [%s] [%s] [%s] "
			, Er_systime.wYear, Er_systime.wMonth, Er_systime.wDay, Er_systime.wHour, Er_systime.wMinute, Er_systime.wSecond
			, pDlg->m_pServerInfo4Admin->DBUID
			, pDlg->m_pServerInfo4Admin->DBIP
			, pDlg->m_pServerInfo4Admin->DBName);
		WriteFile(ER_handle,buf,strlen(buf),&writtenBytes,NULL);
	}

	WriteFile(ER_handle,data,size,&writtenBytes,NULL);
	WriteFile(ER_handle, crt, 2, &writtenBytes, NULL); 
	CloseHandle(ER_handle);

	::LeaveCriticalSection(&m_csLog);
	return 1;
}
