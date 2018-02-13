#include "StdAfx.h"
#include "LogManager.h"

CLogManager::CLogManager()
{
	InitializeCriticalSection(&m_criticalSection);
	m_hFile		= NULL;
	memset(m_szFileName, 0x00, 512);
	memset(m_szDirectory, 0x00, 512);

	m_bChangingFlagFile = FALSE;
}

// destructor
CLogManager::~CLogManager()
{
	CloseFile();
	DeleteCriticalSection(&m_criticalSection);
}

void CLogManager::Clean()
{
	CloseFile();
}

BOOL CLogManager::InitLogManger(BOOL bUse, LPCWSTR szFileName, LPCWSTR szDirectory)
{
	if(!bUse
		|| lstrcmp(szFileName, NULL) == 0
		|| lstrcmp(szDirectory, NULL) == 0
		|| lstrlen(szFileName) > 511
		|| lstrlen(szDirectory) > 511)
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

BOOL CLogManager::OpenFile(LPCWSTR szFileName, LPCWSTR szDirectory)
{
	if(m_hFile != NULL)
	{	
		return FALSE;
	}

	if(false == CreateDirectory(szDirectory, NULL))
	{
		// error
		int nErr = GetLastError();
		SetLastError(0);

		if(NO_ERROR != nErr
			&& ERROR_ALREADY_EXISTS != nErr)
		{
			TCHAR szTemp[256];
			wsprintf(szTemp, L"[ERROR] CSystemLogManager::OpenFile_ CreateDirectory_ error, LastError[%d] FileName[%s] Directory[%s]\r\n",
				nErr, szFileName, szDirectory);
			MessageBox(NULL, szTemp, L"ERROR", MB_OK);
			return FALSE;
		}
	}
	
	util::zero(m_szFileName, 512);
//	StringCchCopy(m_szFileName, 512, szFileName);
	lstrcpy(m_szFileName, szFileName);
	util::zero(m_szDirectory, 512);
//	StringCchCopy(m_szDirectory, 512, szDirectory);
	lstrcpy(m_szDirectory, szDirectory);

	time_t		ltime;
//	struct tm	*today = NULL;
	struct tm	today;
	TCHAR		szCurrTime[128];
	_tzset();
	time(&ltime);
	localtime_s(&today, &ltime);
	wcsftime(szCurrTime, 128, L"%Y%m%d_%H%M%S.log", &today);
	
	wsprintf(m_szOpenedFilePath, L"%s%s_%s", m_szDirectory, m_szFileName, szCurrTime);

	m_hFile = CreateFile(m_szOpenedFilePath, GENERIC_WRITE, FILE_SHARE_READ, NULL,
							CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
	if ( m_hFile == INVALID_HANDLE_VALUE)
	{
		// error
		int nErr = GetLastError();
		SetLastError(0);

		TCHAR szTemp[256];
		wsprintf(szTemp, L"[ERROR] CSystemLogManager::OpenFile_ CreateFile_ error, LastError[%d] OpenFileName[%s]\r\n",
			nErr, m_szOpenedFilePath);
		MessageBox(NULL, szTemp, L"ERROR", MB_OK);
		return FALSE;
	}

	return TRUE;
}

BOOL CLogManager::CloseFile()
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

BOOL CLogManager::ChangeFile()
{
	m_bChangingFlagFile = TRUE;
	EnterCriticalSection(&m_criticalSection);	
	CloseFile();
	TCHAR szTempFileName[512];
	TCHAR szTempDirectory[512];
	util::zero(szTempFileName, 512);
	lstrcpy(szTempFileName, m_szFileName);
	util::zero(szTempDirectory, 512);
	lstrcpy(szTempDirectory, m_szDirectory);

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

BOOL CLogManager::WriteSystemLog(LPCWSTR log, BOOL bTimeFlag)
{
	if(lstrlen(log) >= 1024 - 18
		|| (m_bChangingFlagFile == FALSE && m_hFile == INVALID_HANDLE_VALUE)
		|| (m_bChangingFlagFile == FALSE && m_hFile == 0))
	{	// error, file not opened
//		DbgOut("\r\nCSystemLogManager::WriteSystemLog error, m_hFile[0x%X] StringSize[%d]\n",
//			m_hFile, lstrlen(log));
		return FALSE;
	}

	TCHAR szLogLineBuffer[1024];
	TCHAR szTime[128];
	time_t ltime;
//	struct tm *today = NULL;
	struct tm today;
	DWORD	nWritten;
	DWORD	dwFileSize = 0;

	if(bTimeFlag)
	{

		EnterCriticalSection(&m_criticalSection);
		
		///////////////////////////////////////////////////////////////////////////////
		// 2007-02-28 by cmkwon, 위치 이동함
		time(&ltime);
//		today = localtime(&ltime);
		// 2007-07-25 by cmkwon, 엑셀로 로딩 할때를 위해서 구분자로 아래와 같이 수정함
		//strftime(szTime, 128, "[%m-%d %H:%M:%S] ", today );
		localtime_s(&today, &ltime);
		wcsftime(szTime, 128, L"%m-%d %H:%M:%S|", &today );
		lstrcpy(szLogLineBuffer, szTime);
		lstrcat(szLogLineBuffer, log);

		dwFileSize = GetFileSize(m_hFile, NULL);
		if(dwFileSize > 3*1024*1024)
		{	// System Log File이 3 MBytes 보다 크면 새로운 File을 open한다.
			if(ChangeFile() == FALSE)
			{
				LeaveCriticalSection(&m_criticalSection);
				return FALSE;
			}
		}
			
		DWORD nDataSize = 0;
		WORD wd = 0xFEFF;
		WriteFile(m_hFile, &wd, 2, &nDataSize, NULL);
		WriteFile(m_hFile, szLogLineBuffer, lstrlen(szLogLineBuffer) * sizeof(TCHAR), &nWritten, NULL);
		LeaveCriticalSection(&m_criticalSection);
	}
	else
	{
		szLogLineBuffer[0] = NULL;
		lstrcpy(szLogLineBuffer, L"                  ");
		lstrcat(szLogLineBuffer, log);

		EnterCriticalSection(&m_criticalSection);
		WriteFile(m_hFile, szLogLineBuffer, lstrlen(szLogLineBuffer) * sizeof(TCHAR), &nWritten, NULL);
		dwFileSize = GetFileSize(m_hFile, NULL);
		LeaveCriticalSection(&m_criticalSection);
	}
	return TRUE;
}
