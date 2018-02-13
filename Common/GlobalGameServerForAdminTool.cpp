// Temporary workaround to compile tools

#include "stdafx.h"
#include "GlobalGameServer.h"
#include "AtumDBManager.h"
#include <timeapi.h>
#include <fstream>

CGlobalGameServer* g_pGlobal = nullptr;

CSystemLogManager CGlobalGameServer::ms_SystemLogManager;

// internal helper
void GetRootPath(string& buffer, const char* filename)
{
	// Default Path
	buffer = "./";

	ifstream fin;

	fin.open(filename);

	if (!fin.is_open())
	{	// cannot open file...

		int nErr = GetLastError();
		SetLastError(0);
		char	szError[1024];
		char	szDir[1024];
		GetCurrentDirectory(1024, szDir);
		sprintf(szError, "\r\n[WARNING] Cannot find '%s': set default path '%s', Current Directory'%s'\r\n\r\n", filename, buffer, szDir);
		DBGOUT(szError);
		return;
	}

	constexpr auto seps = "\r\n";

	while (true)
	{
		char buff[1024] { };

		auto bFlagFileEnd = fin.getline(buff, 1024).eof();

		if (bFlagFileEnd && strcmp(buff, "") == 0) break;

		auto token = strtok(buff, seps);

		// ignore blank lines
		// ignore comments
		// ignore user defined variables
		if (token == nullptr || strncmp(token, "#", 1) == 0 || strncmp(token, "$", 1) == 0) continue;

		buffer = token;
	}

	auto c = buffer[buffer.size() - 1];

	if (c != '\\' && c != '/') buffer += "/";
}

bool CGlobalGameServer::InitGlobal(const char* servername, const char* logfilename, HINSTANCE, UINT, int)
{
	GetRootPath(CONFIG_ROOT, "./config_root.cfg");
	GetRootPath(CLIENT_ROOT, "./client_root.cfg");

	ms_SystemLogManager.InitLogManger(TRUE, logfilename, (CONFIG_ROOT + "../log/SystemLog/").c_str());

	return TRUE;
}

bool CGlobalGameServer::WriteSystemLog(char * szLogMsg, bool bTimeHeaderFlag)
{
	return ms_SystemLogManager.WriteSystemLog(szLogMsg, bTimeHeaderFlag);
}

bool CGlobalGameServer::WriteSystemLogEX(bool bPrintDBGOUT, const char * pFormat, ...)
{
	char szLogMsg[1024];
	va_list args;
	va_start(args, pFormat);
	vsprintf_s(szLogMsg, pFormat, args);

	if (bPrintDBGOUT) DBGOUT(szLogMsg);

	return ms_SystemLogManager.WriteSystemLog(szLogMsg, true);
}

bool CGlobalGameServer::WriteSystemLineEX(bool bPrintDBGOUT, const char * pFormat, ...)
{
	char szLogMsg[1024];
	va_list args;
	va_start(args, pFormat);
	vsprintf_s(szLogMsg, pFormat, args);

	if (bPrintDBGOUT) DBGOUT(szLogMsg);

	return ms_SystemLogManager.WriteSystemLog(szLogMsg, false);
}

CGlobalGameServer::CGlobalGameServer() :
	m_pGIOCP { nullptr }
{
	// added from CGlobal, todo : review  `m_pGIOCP { nullptr }`
	m_hMutexMonoInstance = nullptr;

	if (g_pGlobal) return;

	// todo : deprecate
	srand(timeGetTime());
	
	random::init();

	g_pGlobal = this;

	m_hInstanceHandle = NULL;
	m_hMainWndHandle = NULL;


	m_nTimerIDTraffic = 0;
	m_nTimerIDAliveCheck = 0;
	m_nTimerIDReconnect = 0;

	//////////////////////////////////////////////////////////////////////////
	// Configuration Variables
	util::zero(m_szIPLocal);
	util::zero(m_szIPPreServer);
	m_nPortListening = 0;
	m_nPortPreServer = 0;

	util::zero(m_szODBCDSN);
	util::zero(m_szODBCUID);
	util::zero(m_szODBCPASSWORD);

	// 2007-12-03 by cmkwon, 게임 로그 DB 따로 구축 하기 버그 수정 - 초기화
	util::zero(m_szLogDBODBCDSN);			// LogDB_ODBC_DSN
	util::zero(m_szLogDBODBCUID);			// LogDB_ODBC_UID
	util::zero(m_szLogDBODBCPASSWORD);		// LogDB_ODBC_PASSWD

	//////////////////////////////////////////////////////////////////////////
	// Configuration Variables
	util::zero(m_szServerGroupName);

	m_bIsTestServer = false;


	util::zero(m_szDBServerIP);
	m_nDBServerPort = 0;
	util::zero(m_szDBServerDatabaseName);

	util::zero(m_szLogDBDBServerIP);
	m_nLogDBDBServerPort = 0;
	util::zero(m_szLogDBDBServerDatabaseName);
}

CGlobalGameServer::~CGlobalGameServer() { }

bool CGlobalGameServer::EndServerSocket() { return false; }