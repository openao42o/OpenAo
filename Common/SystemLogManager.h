#pragma once
#include "StdAfx.h"

class CSystemLogManager
{
public:
	CSystemLogManager();
	virtual ~CSystemLogManager();

	HANDLE GetFileHandle() const { return m_hFile; }
	BOOL InitLogManger(BOOL bUse = TRUE, const char *szFileName = "SystemLog", const char *szDirectory = "./SystemLog/");
	void Clean();

	BOOL OpenFile(const char *szFileName, const char *szDirectory);
	BOOL CloseFile();
	BOOL ChangeFile();
	BOOL WriteSystemLog(char* log, BOOL bTimeFlag = TRUE);

protected:
	CRITICAL_SECTION	m_criticalSection;
	HANDLE				m_hFile;
	BOOL				m_bChangingFlagFile;

	char				m_szFileName[512];
	char				m_szDirectory[512];
	char				m_szOpenedFilePath[1024];
};

