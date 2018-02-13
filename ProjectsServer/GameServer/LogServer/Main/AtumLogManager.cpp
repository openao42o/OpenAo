#include "stdafx.h"
#include "AtumLogManager.h"
#include "AtumProtocol.h"

#include <time.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/timeb.h>
#include <string.h>

// constructor
CAtumLogManager::CAtumLogManager()
{
	m_hFile = 0;
	InitializeCriticalSection(&m_criticalSection);
}

// destructor
CAtumLogManager::~CAtumLogManager()
{
	CloseFile();
	DeleteCriticalSection(&m_criticalSection);
}

void CAtumLogManager::Clean()
{
}

BOOL CAtumLogManager::Init()
{
	OpenFile();
	return TRUE;
}

BOOL CAtumLogManager::OpenFile()
{
	time_t ltime;
	struct tm *today = NULL;

	_tzset();

	char szFileName[128];
	strcpy(szFileName, "AtumLog_");
	char szCurrTime[128];

	time(&ltime);
	today = localtime( &ltime );
	strftime(szCurrTime, 128, "%Y%m%d_%H%M%S.log", today );

	strcat(szFileName, szCurrTime);

	m_hFile = CreateFile(szFileName, GENERIC_WRITE, FILE_SHARE_READ, NULL,
							CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
	if ( m_hFile == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}

	return TRUE;
}

BOOL CAtumLogManager::CloseFile()
{
	CloseHandle(m_hFile);
	m_hFile = 0;

	return TRUE;
}

BOOL CAtumLogManager::ChangeFile()
{
	CloseFile();
	OpenFile();

	return TRUE;
}

BOOL CAtumLogManager::WriteLog(USHORT msgType, MSG_FL_LOG* log, char* szAction, ofstream* file)
{
	if ( m_hFile == INVALID_HANDLE_VALUE || m_hFile == 0  )
	{
		// error, file not opened
		return FALSE;
	}

	// log message가 초기화되지 않았으면 return함
	if (log->ConnectTime == 0)
	{
		return FALSE;
	}

	char szLogLineBuffer[SIZE_MAX_A_LOG_LINE];
	char CurrTime[128], ConnTime[128];
	struct tm *today;
	DWORD	nWritten;

	today = localtime( &log->CurrentTime );
	if (today == 0) { return FALSE; }
	strftime(CurrTime, 128, "%Y-%m-%d %H:%M:%S", today );

	today = localtime( &log->ConnectTime );
	if (today == 0) { return FALSE; }
	strftime(ConnTime, 128, "%Y-%m-%d %H:%M:%S", today );

	sprintf(szLogLineBuffer, "[%s|%s|%s|%s|%s|%s|%s|%d|%d|%d|%s]\r\n",
			getSZLogType(msgType),
			CurrTime,
			ConnTime,
			log->ClientIP,
			log->AccountName,
			log->CharacterName,
			log->MapName,
			(int)log->PosVector.x,
			(int)log->PosVector.y,
			(int)log->PosVector.z,
			szAction
			);

	EnterCriticalSection(&m_criticalSection);
	WriteFile(m_hFile, szLogLineBuffer, strlen(szLogLineBuffer), &nWritten, NULL);
	LeaveCriticalSection(&m_criticalSection);

	return TRUE;
}
