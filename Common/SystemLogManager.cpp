#include "stdafx.h"
#include "SystemLogManager.h"

#include <time.h>
#include <stdio.h>
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

BOOL CSystemLogManager::InitLogManger(BOOL bUse, const char *szFileName, const char *szDirectory)
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

	// 2009-02-23 by cmkwon, PC 재부팅 경과 시간 로금 남기기 추가
	int nElapsedSec	= GetTickCount()/1000;
	int nSecond		= nElapsedSec%60;
	int nMinute		= (nElapsedSec/60)%60;
	int nHour		= (nElapsedSec/3600)%24;
	int nDay		= nElapsedSec/86400;
	char szTemp[1024];
	sprintf(szTemp, "[Notify] elapsed time after turn on !! %2dDay %2dh:%2dm:%2ds \r\n", nDay, nHour, nMinute, nSecond);
	this->WriteSystemLog(szTemp);
	return TRUE;
}

BOOL CSystemLogManager::OpenFile(const char *szFileName, const char *szDirectory)
{
	if(m_hFile != NULL){	return FALSE;}

	if(CreateDirectory(szDirectory, NULL) == 0)
	{
		// error
		int nErr = GetLastError();
		SetLastError(0);

		if(nErr != ERROR_ALREADY_EXISTS)
		{
			///////////////////////////////////////////////////////////////////////////////
			// 2008-09-24 by cmkwon, 게임 로그 폴더  \Log\ 자동 생성되게 구현 - 
			char szTmDir[MAX_PATH];
			util::zero(szTmDir, MAX_PATH);
			memcpy(szTmDir, szDirectory, strlen(szDirectory)-1);	// 2008-09-24 by cmkwon, 마지막 / or \ 문자를 제거하고 복사

			char *pParentDirEnd = strrchr(szTmDir, '\\');
			if(NULL == pParentDirEnd)
			{
				pParentDirEnd = strrchr(szTmDir, '/');
			}			
			if(pParentDirEnd)
			{// 2008-09-24 by cmkwon, 2번째 상위 디렉토리가 있다면 생성해 본다.
				char szParentDirPath[MAX_PATH];
				util::zero(szParentDirPath, MAX_PATH);
				memcpy(szParentDirPath, szTmDir, min(pParentDirEnd-szTmDir, MAX_PATH));

				CreateDirectory(szParentDirPath, NULL);
				CreateDirectory(szDirectory, NULL);
				nErr = GetLastError();
				SetLastError(0);
			}

			if(NO_ERROR != nErr
				&& ERROR_ALREADY_EXISTS != nErr)
			{
				char szTemp[256];
				sprintf(szTemp, "[ERROR] CSystemLogManager::OpenFile_ CreateDirectory_ error, LastError[%d] FileName[%s] Directory[%s]\r\n",
					nErr, szFileName, szDirectory);
				MessageBox(NULL, szTemp, "ERROR", MB_OK);
				return FALSE;
			}
		}
	}
	util::strncpy(m_szFileName, szFileName, 512);
	util::strncpy(m_szDirectory, szDirectory, 512);

	time_t		ltime;
	struct tm	*today = NULL;
	char		szCurrTime[128];
	_tzset();
	time(&ltime);
	strftime(szCurrTime, 128, "%Y%m%d_%H%M%S.log", localtime(&ltime));
	
	sprintf(m_szOpenedFilePath, "%s%s_%s", m_szDirectory, m_szFileName, szCurrTime);

	m_hFile = CreateFile(m_szOpenedFilePath, GENERIC_WRITE, FILE_SHARE_READ, NULL,
							CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
	if ( m_hFile == INVALID_HANDLE_VALUE)
	{
		// error
		int nErr = GetLastError();
		SetLastError(0);

		char szTemp[256];
		sprintf(szTemp, "[ERROR] CSystemLogManager::OpenFile_ CreateFile_ error, LastError[%d] OpenFileName[%s]\r\n",
			nErr, m_szOpenedFilePath);
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
		DWORD dwSize = GetFileSize(m_hFile, NULL);
		CloseHandle(m_hFile);
		m_hFile = NULL;

		if(dwSize == 0)
		{
			DeleteFile(m_szOpenedFilePath);
		}
	}
	LeaveCriticalSection(&m_criticalSection);
	return TRUE;
}

BOOL CSystemLogManager::ChangeFile()
{
	m_bChangingFlagFile = TRUE;
	EnterCriticalSection(&m_criticalSection);	
	CloseFile();
	char szTempFileName[512];
	char szTempDirectory[512];
	util::strncpy(szTempFileName, m_szFileName, 512);
	util::strncpy(szTempDirectory, m_szDirectory, 512);
	if(!OpenFile(szTempFileName, szTempDirectory))
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
	if (strlen(log) >= 1024 - 18
		|| (m_bChangingFlagFile == FALSE && m_hFile == INVALID_HANDLE_VALUE)
		|| (m_bChangingFlagFile == FALSE && m_hFile == 0))
	{	// error, file not opened
		DbgOut("\r\nCSystemLogManager::WriteSystemLog error, m_hFile[0x%X] StringSize[%d]\n", m_hFile, strlen(log));
		return FALSE;
	}

	DWORD	nWritten;

	if (bTimeFlag)
	{
		char szLogLineBuffer[1024] = { };

		EnterCriticalSection(&m_criticalSection);
		
		auto ltime = time(nullptr);
		
		strftime(szLogLineBuffer, 128, "%m-%d %H:%M:%S|", localtime(&ltime));

		strcat(szLogLineBuffer, log);

		if (GetFileSize(m_hFile, nullptr) > 10485760) if (!ChangeFile())
		{
			LeaveCriticalSection(&m_criticalSection);
			return FALSE;
		}
	
		WriteFile(m_hFile, szLogLineBuffer, strlen(szLogLineBuffer), &nWritten, nullptr);
		LeaveCriticalSection(&m_criticalSection);
	}
	else
	{
		EnterCriticalSection(&m_criticalSection);
		WriteFile(m_hFile, log, strlen(log), &nWritten, nullptr);
		LeaveCriticalSection(&m_criticalSection);
	}

	return TRUE;
}
