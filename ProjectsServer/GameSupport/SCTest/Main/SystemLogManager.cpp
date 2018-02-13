#include "stdafx.h"
#include "SystemLogManager.h"

#include <time.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/timeb.h>
#include <string.h>

// constructor
CSystemLogManager::CSystemLogManager()
{
	InitializeCriticalSection(&m_criticalSection);
	m_hFile		= NULL;
	memset(m_szFileName, 0x00, 512);
	memset(m_szDirectory, 0x00, 512);

	m_bChangingFlagFile = FALSE;
}

// destructor
CSystemLogManager::~CSystemLogManager()
{
	CloseFile();
	DeleteCriticalSection(&m_criticalSection);
}

void CSystemLogManager::Clean()
{
	CloseFile();
}

BOOL CSystemLogManager::InitLogManger(BOOL bUse, char *szFileName, char *szDirectory)
{
	if(!bUse
		|| strncmp(szFileName, "", 512) == 0
		|| strncmp(szDirectory, "", 512) == 0
		|| strlen(szFileName) > 511
		|| strlen(szDirectory) > 511)
	{
		return FALSE;
	}

	EnterCriticalSection(&m_criticalSection);
	if(!OpenFile(szFileName, szDirectory))
	{
		LeaveCriticalSection(&m_criticalSection);
		return FALSE;
	}
	LeaveCriticalSection(&m_criticalSection);
	return TRUE;
}

BOOL CSystemLogManager::OpenFile(char *szFileName, char *szDirectory)
{
	if(m_hFile != NULL){	return FALSE;}

	if(CreateDirectory(szDirectory, NULL) == 0)
	{
		// error
		int nErr = GetLastError();
		SetLastError(0);

		if(nErr != ERROR_ALREADY_EXISTS)
		{
			char szTemp[256];
			sprintf(szTemp, "[ERROR] CSystemLogManager::OpenFile_ CreateDirectory_ error, LastError[%d] FileName[%s] Directory[%s]\r\n",
				nErr, szFileName, szDirectory);
			MessageBox(NULL, szTemp, "ERROR", MB_OK);
			return FALSE;
		}
	}
	strncpy(m_szFileName, szFileName, 512);
	strncpy(m_szDirectory, szDirectory, 512);

	time_t		ltime;
	struct tm	*today = NULL;
	char		szCurrTime[128];
	_tzset();
	time(&ltime);
	strftime(szCurrTime, 128, "%Y%m%d_%H%M%S.log", localtime(&ltime));

	char szOpenFileName[1024];
	sprintf(szOpenFileName, "%s%s_%s", m_szDirectory, m_szFileName, szCurrTime);

	m_hFile = CreateFile(szOpenFileName, GENERIC_WRITE, FILE_SHARE_READ, NULL,
							CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
	if ( m_hFile == INVALID_HANDLE_VALUE)
	{
		// error
		int nErr = GetLastError();
		SetLastError(0);

		char szTemp[256];
		sprintf(szTemp, "[ERROR] CSystemLogManager::OpenFile_ CreateFile_ error, LastError[%d] OpenFileName[%s]\r\n",
			nErr, szOpenFileName);
		MessageBox(NULL, szTemp, "ERROR", MB_OK);
		return FALSE;
	}

	return TRUE;
}

BOOL CSystemLogManager::CloseFile()
{
	EnterCriticalSection(&m_criticalSection);
	if(m_hFile)
	{
		CloseHandle(m_hFile);
		m_hFile = NULL;
	}
	LeaveCriticalSection(&m_criticalSection);
	return TRUE;
}

BOOL CSystemLogManager::ChangeFile()
{
	m_bChangingFlagFile = TRUE;
	EnterCriticalSection(&m_criticalSection);	
	CloseFile();
	if(!OpenFile(m_szFileName, m_szDirectory))
	{
		LeaveCriticalSection(&m_criticalSection);
		m_bChangingFlagFile = FALSE;
		return FALSE;
	}	
	LeaveCriticalSection(&m_criticalSection);
	m_bChangingFlagFile = FALSE;
	return TRUE;
}

BOOL CSystemLogManager::WriteSystemLog(char* log, BOOL bTimeFlag)
{
	if(strlen(log) >= 1024 - 18
		|| (m_bChangingFlagFile == FALSE && m_hFile == INVALID_HANDLE_VALUE)
		|| (m_bChangingFlagFile == FALSE && m_hFile == 0))
	{	// error, file not opened
		DbgOut("\r\nCSystemLogManager::WriteSystemLog error, m_hFile[0x%X] StringSize[%d]\n",
			m_hFile, strlen(log));
		return FALSE;
	}

	char szLogLineBuffer[1024];
	char szTime[128];
	time_t ltime;
	struct tm *today = NULL;
	DWORD	nWritten;
	DWORD	dwFileSize = 0;

	if(bTimeFlag)
	{
		time(&ltime);
		today = localtime(&ltime);
		strftime(szTime, 128, "[%m-%d %H:%M:%S] ", today );
		strcpy(szLogLineBuffer, szTime);
		strcat(szLogLineBuffer, log);

		EnterCriticalSection(&m_criticalSection);
		dwFileSize = GetFileSize(m_hFile, NULL);
		if(dwFileSize > 1024 * 1024)
		{	// System Log File이 1 MBytes 보다 크면 새로운 File을 open한다.
			if(ChangeFile() == FALSE)
			{
				LeaveCriticalSection(&m_criticalSection);
				return FALSE;
			}
		}
		WriteFile(m_hFile, szLogLineBuffer, strlen(szLogLineBuffer), &nWritten, NULL);
		LeaveCriticalSection(&m_criticalSection);
	}
	else
	{
		szLogLineBuffer[0] = NULL;
		strcpy(szLogLineBuffer, "                  ");
		strcat(szLogLineBuffer, log);

		EnterCriticalSection(&m_criticalSection);
		WriteFile(m_hFile, szLogLineBuffer, strlen(szLogLineBuffer), &nWritten, NULL);
		dwFileSize = GetFileSize(m_hFile, NULL);
		LeaveCriticalSection(&m_criticalSection);
	}
	return TRUE;
}
