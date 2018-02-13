#include "stdafx.h"
#include "PreGlobal.h"
#include "PreIOCP.h"
#include "Config.h"
#include "AtumSJ.h"

#include <iostream>
#include <thread>

CPreGlobal* g_pPreGlobal = nullptr;

CPreGlobal::CPreGlobal()
{
	if (g_pPreGlobal) return;

	g_pPreGlobal = this;

	Reset();
}

CPreGlobal::~CPreGlobal()
{
	// reset all member variables
	Reset();

	g_pGlobal = nullptr;
}

void CPreGlobal::VMemAlloc()
{
	VMemPool::vmPoolAddObject(sizeof(COverlapped), SIZE_MAX_PRESERVER_SESSION * 10);
	VMemPool::vmPoolAddObject(sizeof(CSendPacket), 100);		// 2008-02-22 by cmkwon, ServerPreServer->MasangPreServer 로 서비스 정보 전송 시스템 추가 - 
	VMemPool::vmPoolAddObject(sizeof(CRecvPacket), 100);		// 2008-02-22 by cmkwon, ServerPreServer->MasangPreServer 로 서비스 정보 전송 시스템 추가 - 
}

void CPreGlobal::Reset()
{
	m_AllowedIPList.clear();
	m_AllowedAccounts.clear();
	m_bRandomFieldSelect = FALSE; // Pre Server
	m_bIgnoreClientVersionUpdate = FALSE;

	m_AllServerGroupVectorForLoading.clear();

	m_nServerGroupLimiteUserCount = 999; // 2006-10-11 by cmkwon

	m_vectGameServerGroupList.clear(); // 2007-05-02 by cmkwon

	m_bIsOnlyChoiceServer = FALSE; // 2007-07-06 by cmkwon, OnlyChoiceServer 플래그 구현 추가

	util::zero(m_szGamePublisherName); // 2010-06-01 by shcho, GLogDB 관련 -
}

void CPreGlobal::AddDownloadHttpIP(const char* i_szIP)
{
	m_vectDownloadHttpServer.push_back(i_szIP);
}

const char* CPreGlobal::GetRandomDownloadHttpIP() const
{
	if (m_vectDownloadHttpServer.empty()) return nullptr;

	if (m_vectDownloadHttpServer.size() == 1) return m_vectDownloadHttpServer[0].c_str();

	return m_vectDownloadHttpServer[RANDI(m_vectDownloadHttpServer.size())].c_str();
}

BOOL CPreGlobal::CheckAllowedList(CServerGroup* i_pGroupInfo, char* i_szAccountName, char* i_szPrivateIP, USHORT i_nAccountType)
{
	if (m_AllowedAccounts.empty()) return TRUE;

	if (m_AllowedAccounts.find(i_szAccountName) != m_AllowedAccounts.end()) return TRUE;

	if (COMPARE_RACE(i_nAccountType, RACE_OPERATION | RACE_GAMEMASTER | RACE_MONITOR))
	{
		this->WriteSystemLogEX(TRUE, STRMSG_S_P2PRENOTIFY_0000, CAtumSJ::GetRaceString(i_nAccountType), i_szAccountName, i_szPrivateIP);
		return TRUE;
	}

	return FALSE;
}

void CPreGlobal::PrintAllowedList() const
{
	for (const auto& str : m_AllowedIPList) DBGOUT(" AllowedIP: \'%s\'\r\n", str.c_str());

	for (const auto& str : m_AllowedAccounts) DBGOUT(" AllowedAccount: \'%s\'\r\n", str.c_str());

	DBGOUT("\n");
}

BOOL CPreGlobal::CheckAllowedIP(const char* i_szPublicIP) const
{
	if (m_AllowedIPList.empty()) return TRUE;

	for (const auto& ipaddress : m_AllowedIPList)
	{
		if (ipaddress == i_szPublicIP)
		{
			g_pPreGlobal->WriteSystemLogEX(TRUE, " [Notify] Allowed IP(%s)\r\n", i_szPublicIP);
			return TRUE;
		}
	}

	g_pPreGlobal->WriteSystemLogEX(TRUE, " [Notify] Not Allowed IP(%s)\r\n", i_szPublicIP);
	return FALSE;
}

bool CPreGlobal::InitServerSocket()
{
	if (m_pGIOCP) return false;

	server::log(" [Notify] CPreGlobal::InitServerSocket# Starting... \r\n");

	m_pGIOCP = new CPreIOCP { m_nPortListening, m_szIPLocal };

	if (!m_pGIOCP->IOCPInit())
	{
		constexpr char tmp[] = " [Error] PreServer IOCPInit Error\r\n";

		server::log(tmp);

		MessageBox(NULL, tmp, "ERROR", MB_OK);

		return false;
	}

	// 2009-03-19 by cmkwon, 시스템 로그 추가 - 
	server::log(" [Notify] CPreGlobal::InitServerSocket# Started \r\n");

	return true;
}

bool CPreGlobal::EndServerSocket()
{
	server::log(" [Notify] CPreGlobal::EndServerSocket# \r\n");

	CGlobalGameServer::EndServerSocket();

	return true;
}

bool CPreGlobal::LoadConfiguration()
{
	DBGOUT("========================================\n");
	DBGOUT("Loading Server Configuration...\n");

	constexpr auto SIZE_BUFF = 512;

	auto configFileName = CONFIG_ROOT + string("pre.cfg");

	char buff[SIZE_BUFF];
	char buffBackup[SIZE_BUFF]; // 2008-07-11 by cmkwon, MySQL 지원을 위해 수정(IP,Port 로 DB 서버 연결) - 
	const char* token;
	char seps[] = " \t";

	/////////////////////////////////////////////
	// load MapWorkspace file...
	ifstream fin;
	CConfig config;

	if (!config.LoadUserDefinedVariables(GLOBAL_CONFIG_FILE_PATH))
	{
		char szError[1024];
		sprintf(szError, "[Error] LoadConfiguration LoadUserDefinedVariables() error, FileName[%s]\r\n"
		        , GLOBAL_CONFIG_FILE_PATH);
		this->WriteSystemLog(szError);
		DBGOUT(szError);
		return FALSE;
	}

	if (!config.LoadUserDefinedVariables(configFileName.c_str()))
	{
		char szSystemLog[256];
		sprintf(szSystemLog, "[Error] CPreIOCP::LoadAllServers Config Initialization Error1\r\n");
		this->WriteSystemLog(szSystemLog);
		DBGOUT(szSystemLog);
		return FALSE;
	}

	fin.open(configFileName.c_str());

	if (!fin.is_open())
	{
		char szSystemLog[256];
		sprintf(szSystemLog, "[Error] CPreIOCP::LoadAllServers Can't open config file\r\n");
		this->WriteSystemLog(szSystemLog);
		DBGOUT(szSystemLog);
		return FALSE;
	}

	while (true)
	{
		memset(buff, 0x00, SIZE_BUFF);
		
		if (fin.getline(buff, SIZE_BUFF).eof() && buff[0] == '\0') break;

		util::strncpy(buffBackup, buff, SIZE_BUFF); // 2008-07-11 by cmkwon, MySQL 지원을 위해 수정(IP,Port 로 DB 서버 연결) - 

		token = config.strtok(buff, seps);
		
		// ignore blank lines
		// ignore comments
		// ignore user defined variables
		if (token == NULL || strncmp(token, "#", 1) == 0 || strncmp(token, "$", 1) == 0) continue;

		if (stricmp(token, "LocalIPAddress") == 0)
		{
			token = config.strtok(NULL, seps);
			if (token == NULL)
			{
				char* msg = "Loading Configuration File Failed.\n";
				DBGOUT(msg);
				MessageBox(NULL, msg, "Error", MB_OK);
				return FALSE;
			}
			this->SetIPLocal((char*)token);
		}
		else if (stricmp(token, "LocalPublicIPAddress") == 0)
		{// 2006-05-23 by cmkwon,
			token = config.strtok(NULL, seps);
			if (token == NULL)
			{
				char* msg = "Loading Configuration File Failed.\n";
				DBGOUT(msg);
				MessageBox(NULL, msg, "Error", MB_OK);
				return FALSE;
			}
			this->SetPublicIPLocal((char*)token);
		}
		else if (stricmp(token, "LocalListenPort") == 0)
		{
			token = config.strtok(NULL, seps);
			if (token == NULL)
			{
				char* msg = "Loading Configuration File Failed.\n";
				DBGOUT(msg);
				MessageBox(NULL, msg, "Error", MB_OK);
				return FALSE;
			}
			if (strcmp(token, "") != 0)
			{
				this->SetPortListening(atoi(token));
			}
		}
		else if (stricmp(token, "ODBCDSN") == 0)
		{
			token = config.strtok(NULL, seps);
			if (token == NULL)
			{
				char* msg = "Loading Configuration File Failed. ODBCDSN\n";
				DBGOUT(msg);
				MessageBox(NULL, msg, "Error", MB_OK);
				return FALSE;
			}
			if (strcmp(token, "") != 0)
			{
				util::strncpy((char*)m_szODBCDSN, token, SIZE_MAX_ODBC_CONN_STRING);
			}
		}
		else if (stricmp(token, "ODBCUID") == 0)
		{
			token = config.strtok(NULL, seps);
			if (token == NULL)
			{
				char* msg = "Loading Configuration File Failed. ODBCUID\n";
				DBGOUT(msg);
				MessageBox(NULL, msg, "Error", MB_OK);
				return FALSE;
			}
			if (strcmp(token, "") != 0)
			{
				util::strncpy((char*)m_szODBCUID, token, SIZE_MAX_ODBC_CONN_STRING);
				///////////////////////////////////////////////////////////////////////////////
				// 2008-09-01 by cmkwon, global.cfg 파일에 DB ID/PWD 암호화 - 
				//BYTE byEncodedBinary[1024];
				//util::zero(byEncodedBinary, 1024);
				/*if(XOR::XORString2Binary(byEncodedBinary, (char*)token))
				{
					util::zero(m_szODBCUID, SIZE_MAX_ODBC_CONN_STRING);
					XOR::XOREncode((BYTE*)m_szODBCUID, byEncodedBinary, strlen(token)/2, STR_XOR_KEY_STRING_DB_ID_PWD);
				}*/
			}
		}
		else if (stricmp(token, "ODBCPASSWD") == 0)
		{
			token = config.strtok(NULL, seps);
			if (token == NULL)
			{
				char* msg = "Loading Configuration File Failed. ODBCPASSWD\n";
				DBGOUT(msg);
				MessageBox(NULL, msg, "Error", MB_OK);
				return FALSE;
			}
			if (strcmp(token, "") != 0)
			{
				util::strncpy((char*)m_szODBCPASSWORD, token, SIZE_MAX_ODBC_CONN_STRING);
				///////////////////////////////////////////////////////////////////////////////
				// 2008-09-01 by cmkwon, global.cfg 파일에 DB ID/PWD 암호화 - 
				//BYTE byEncodedBinary[1024];
				//util::zero(byEncodedBinary, 1024);
				/*if(XOR::XORString2Binary(byEncodedBinary, (char*)token))
				{
					util::zero(m_szODBCPASSWORD, SIZE_MAX_ODBC_CONN_STRING);
					XOR::XOREncode((BYTE*)m_szODBCPASSWORD, byEncodedBinary, strlen(token)/2, STR_XOR_KEY_STRING_DB_ID_PWD);
				}*/
			}
		}
		else if (stricmp(token, "ServerGroupLimiteUserCount") == 0)
		{// 2006-10-11 by cmkwon, 서버군별 최대 접속 유저수

			token = config.strtok(NULL, seps);
			if (token == NULL)
			{
				char szSystemLog[256];
				sprintf(szSystemLog, "[Error] CPreIOCP::LoadConfiguration Initialization Error2!!, FieldName: ServerGroupMaxUserCount\r\n");
				this->WriteSystemLog(szSystemLog);
				DBGOUT(szSystemLog);
				return FALSE;
			}

			m_nServerGroupLimiteUserCount = atoi(token);
			if (0 >= m_nServerGroupLimiteUserCount)
			{
				char szSystemLog[256];
				sprintf(szSystemLog, "[Error] CPreIOCP::LoadConfiguration ServerGroupLimiteUserCount Error!!, ServerGroupLimiteUserCount(%d)\r\n", m_nServerGroupLimiteUserCount);
				this->WriteSystemLog(szSystemLog);
				DBGOUT(szSystemLog);
				return FALSE;
			}
		}
		else if (stricmp(token, "ArenaServer") == 0)
		{
			token = config.strtok(NULL, seps);
			
			if (token == NULL)
			{
				char szSystemLog[256];
				sprintf(szSystemLog, "[Error] CPreIOCP::LoadArenaServer Config Initialization Error\r\n");
				WriteSystemLog(szSystemLog);
				DBGOUT(szSystemLog);
				return FALSE;
			}
			
			if (SIZE_MAX_SERVER_NAME <= strlen(token))
			{
				char szSystemLog[256];
				sprintf(szSystemLog, "[Error] CPreIOCP::LoadArenaServer error, ArenaServerName size overflow\r\n");
				WriteSystemLog(szSystemLog);
				DBGOUT(szSystemLog);
				return FALSE;
			}

			char szGroupName[SIZE_MAX_SERVER_NAME];
			util::strncpy(szGroupName, token, SIZE_MAX_SERVER_NAME);
			token = config.strtok(NULL, seps);
			
			if (NULL == token) return FALSE;

			int nMGameServerID = atoi(token);

			BOOL bEnableServerGroup = TRUE;

			if (!GetServerGroupForLoading(szGroupName))
			{
				m_AllServerGroupVectorForLoading.emplace_back(szGroupName, m_nServerGroupLimiteUserCount, bool(bEnableServerGroup));

				DBGOUT("ArenaServer: %20s, Enable(%d) MGameServerID(%5d)\n", szGroupName, bEnableServerGroup, nMGameServerID);
			}
		}
		else if (stricmp(token, "LauncherFileDownloadPath") == 0)
		{
			token = config.strtok(NULL, seps);
			if (token == NULL)
			{
				char szSystemLog[256];
				sprintf(szSystemLog, "[Error] CPreIOCP::LoadAllServers Config Initialization Error2: LauncherFileDownloadPath\r\n");
				this->WriteSystemLog(szSystemLog);
				DBGOUT(szSystemLog);
				return FALSE;
			}
			util::strncpy(this->m_szLauncherFileDownloadPath, (char*)token, SIZE_MAX_FTP_FILE_PATH);
		}
		else if (stricmp(token, "DeleteFileListDownloadPath") == 0)
		{
			token = config.strtok(NULL, seps);
			if (token == NULL)
			{
				char szSystemLog[256];
				sprintf(szSystemLog, "[Error] CPreIOCP::LoadAllServers Config Initialization Error2: DeleteFileListDownloadPath\r\n");
				this->WriteSystemLog(szSystemLog);
				DBGOUT(szSystemLog);
				return FALSE;
			}
			util::strncpy(this->m_szDeleteFileListDownloadPath, (char*)token, SIZE_MAX_FTP_FILE_PATH);
		}
		else if (stricmp(token, "NoticeFileDownloadPath") == 0)
		{
			token = config.strtok(NULL, seps);
			if (token == NULL)
			{
				char szSystemLog[256];
				sprintf(szSystemLog, "[Error] CPreIOCP::LoadAllServers Config Initialization Error2: NoticeFileDownloadPath\r\n");
				this->WriteSystemLog(szSystemLog);
				DBGOUT(szSystemLog);
				return FALSE;
			}
			util::strncpy(this->m_szNoticeFileDownloadPath, (char*)token, SIZE_MAX_FTP_FILE_PATH);
		}
		else if (stricmp(token, "ClientReInstallVersion") == 0)
		{
			token = config.strtok(NULL, seps);
			if (token == NULL)
			{
				char szSystemLog[256];
				sprintf(szSystemLog, "[Error] CPreIOCP::LoadAllServers Config Initialization Error2: ClientReInstallVersion\r\n");
				this->WriteSystemLog(szSystemLog);
				DBGOUT(szSystemLog);
				return FALSE;
			}

			m_ClientReinstallVersion.SetVersion((char*)token);
		}
		else if (stricmp(token, "CriticalUpdateVersion") == 0)
		{
			token = config.strtok(NULL, seps);
			if (token == NULL)
			{
				char szSystemLog[256];
				sprintf(szSystemLog, "[Error] CPreIOCP::LoadAllServers Config Initialization Error2: CriticalUpdateVersion\r\n");
				this->WriteSystemLog(szSystemLog);
				DBGOUT(szSystemLog);
				return FALSE;
			}

			VersionInfo tmpCriticalVersion;
			tmpCriticalVersion.SetVersion((char*)token);
			m_vectorCriticalUpdateVersion.push_back(tmpCriticalVersion);
		}
		else if (stricmp(token, "AllowedIP") == 0)
		{
			token = config.strtok(nullptr, seps);

			if (token && stricmp(token, "") != 0) m_AllowedIPList.emplace_back(token);
		}
		else if (stricmp(token, "AllowedAccount") == 0)
		{
			token = config.strtok(nullptr, seps);
			
			if (token && stricmp(token, "") != 0)
			{
				_strlwr((char*)token);
				m_AllowedAccounts.insert(token);
			}
		}
		else if (stricmp(token, "AllowedToolIP") == 0)
		{
			token = config.strtok(nullptr, seps);
			if (token) AddAllowedToolIP(token);
		}
		else if (stricmp(token, "RandomFieldSelect") == 0)
		{
			token = config.strtok(NULL, seps);

			if (token == NULL || (stricmp(token, "TRUE") != 0 && stricmp(token, "FALSE") != 0))
			{
				char szSystemLog[256];
				sprintf(szSystemLog, "[Error] CPreIOCP::LoadAllServers Config Initialization Error2: RandomFieldSelect\r\n");
				this->WriteSystemLog(szSystemLog);
				DBGOUT(szSystemLog);
				return FALSE;
			}

			if (stricmp(token, "TRUE") == 0)
			{
				m_bRandomFieldSelect = TRUE;
			}
			else if (stricmp(token, "FALSE") == 0)
			{
				m_bRandomFieldSelect = FALSE;
			}
			else
			{
				// unreachable
				return FALSE;
			}
		}
		else if (stricmp(token, "IgnoreClientVersionUpdate") == 0)
		{
			token = config.strtok(NULL, seps);

			if (token == NULL || (stricmp(token, "TRUE") != 0 && stricmp(token, "FALSE") != 0))
			{
				char szSystemLog[256];
				sprintf(szSystemLog, "[Error] CPreIOCP::LoadAllServers Config Initialization Error2: RandomFieldSelect\r\n");
				this->WriteSystemLog(szSystemLog);
				DBGOUT(szSystemLog);
				return FALSE;
			}

			if (stricmp(token, "TRUE") == 0)
			{
				m_bIgnoreClientVersionUpdate = TRUE;
			}
			else if (stricmp(token, "FALSE") == 0)
			{
				m_bIgnoreClientVersionUpdate = FALSE;
			}
			else
			{
				// unreachable
				return FALSE;
			}
		}
		else if (0 == stricmp(token, "IsTestServer"))
		{
			token = config.strtok(NULL, seps);
			if (token == NULL)
			{
				m_bIsTestServer = FALSE;
			}
			else
			{
				if (stricmp(token, "FALSE") == 0)
				{
					m_bIsTestServer = FALSE;
				}
				else
				{
					m_bIsTestServer = TRUE;
				}
			}
		}
		else if (0 == stricmp(token, "IsOnlyChoiceServer"))
		{// 2007-07-06 by cmkwon, IsOnlyChoiceServer 플래그 구현 추가

			m_bIsOnlyChoiceServer = FALSE; // 기본값
			token = config.strtok(NULL, seps);
			if (token
				&& 0 == stricmp(token, "TRUE"))
			{
				m_bIsOnlyChoiceServer = TRUE;
			}
		}
		else if (0 == stricmp(token, "DownloadHttpServerIP"))
		{// 2007-01-06 by cmkwon, 
			token = config.strtok(NULL, seps);
			if (token)
			{
				AddDownloadHttpIP(token);
			}
		}
		else if (0 == stricmp(token, "DownloadHttpServerPort"))
		{// 2007-01-06 by cmkwon, 
			token = config.strtok(NULL, seps);
			if (token)
			{
				m_nDownloadHttpServerPort = atoi(token);
			}
		}
		else if (0 == stricmp(token, "ClientUpdateDownloadHttpDir"))
		{// 2007-01-06 by cmkwon, 
			token = config.strtok(NULL, seps);
			if (token)
			{
				util::strncpy(m_szClientHttpUpdateDownloadDir, token, SIZE_MAX_FTP_FILE_PATH);
			}
		}
		else if (0 == stricmp(token, "LauncherFileDownloadHttpPath"))
		{// 2007-01-06 by cmkwon, 
			token = config.strtok(NULL, seps);
			if (token)
			{
				util::strncpy(m_szLauncherFileDownloadHttpPath, token, SIZE_MAX_FTP_FILE_PATH);
			}
		}
		else if (0 == stricmp(token, "DeleteFileListDownloadHttpPath"))
		{// 2007-01-06 by cmkwon, 
			token = config.strtok(NULL, seps);
			if (token)
			{
				util::strncpy(m_szDeleteFileListDownloadHttpPath, token, SIZE_MAX_FTP_FILE_PATH);
			}
		}
		else if (0 == stricmp(token, "NoticeFileDownloadHttpPath"))
		{// 2007-01-06 by cmkwon, 
			token = config.strtok(NULL, seps);
			if (token)
			{
				util::strncpy(m_szNoticeFileDownloadHttpPath, token, SIZE_MAX_FTP_FILE_PATH);
			}
		}
		else if (0 == stricmp(token, "VersionListFileDownloadHttpPath"))
		{// 2007-01-06 by cmkwon, 
			token = config.strtok(NULL, seps);
			if (token)
			{
				util::strncpy(m_szVersionListFileDownloadHttpPath, token, SIZE_MAX_FTP_FILE_PATH);
			}
		}
		else if (0 == stricmp(token, CONFIG_NAME_DB_SERVER_IP))
		{// 2008-07-11 by cmkwon, MySQL 지원을 위해 수정(IP,Port 로 DB 서버 연결) - 

			token = config.strtok(NULL, seps);
			if (NULL == token)
			{
				char szErr[1024];
				sprintf(szErr, "[ERROR] CPreGlobal::LoadConfiguration error !! %s, %s\r\n", CONFIG_NAME_DB_SERVER_IP, buffBackup);
				MessageBox(NULL, szErr, "Error", MB_OK);
				return FALSE;
			}

			util::strncpy(m_szDBServerIP, token, SIZE_MAX_ODBC_CONN_STRING);
		}
		else if (0 == stricmp(token, CONFIG_NAME_DB_SERVER_PORT))
		{// 2008-07-11 by cmkwon, MySQL 지원을 위해 수정(IP,Port 로 DB 서버 연결) - 

			token = config.strtok(NULL, seps);
			if (NULL == token)
			{
				char szErr[1024];
				sprintf(szErr, "[ERROR] CPreGlobal::LoadConfiguration error !! %s, %s\r\n", CONFIG_NAME_DB_SERVER_PORT, buffBackup);
				MessageBox(NULL, szErr, "Error", MB_OK);
				return FALSE;
			}

			m_nDBServerPort = atoi(token);
		}
		else if (0 == stricmp(token, CONFIG_NAME_DB_SERVER_DATABASE_NAEE))
		{// 2008-07-11 by cmkwon, MySQL 지원을 위해 수정(IP,Port 로 DB 서버 연결) - 

			token = config.strtok(NULL, seps);
			if (NULL == token)
			{
				char szErr[1024];
				sprintf(szErr, "[ERROR] CPreGlobal::LoadConfiguration error !! %s, %s\r\n", CONFIG_NAME_DB_SERVER_DATABASE_NAEE, buffBackup);
				MessageBox(NULL, szErr, "Error", MB_OK);
				return FALSE;
			}

			util::strncpy(m_szDBServerDatabaseName, token, SIZE_MAX_ODBC_CONN_STRING);
		}
		else if (stricmp(token, "GameServerGroup") == 0)
		{// 2007-05-02 by cmkwon

			token = config.strtok(NULL, seps);
			if (NULL == token)
			{
				continue;
			}
			char szTemp[CONFIG_SIZE_BUFF];
			util::strncpy(szTemp, token, CONFIG_SIZE_BUFF);
			vectString tokenList;
			CConfig::GetTokenList(&tokenList, szTemp, seps);
			if (3 > tokenList.size())
			{
				continue;
			}

			InsertGameServerGroup((char*)tokenList[0].c_str(), (char*)tokenList[1].c_str(), atoi(tokenList[2].c_str()));
		}
		else if (stricmp(token, "ServerGroupInfo") == 0)
		{
			// 2008-09-24 by cmkwon, Syntax = [ServerGroupName] [ServerGroupID] [Enable Flag]
			token = config.strtok(NULL, seps);
			if (NULL == token) continue;
			char szTemp[CONFIG_SIZE_BUFF];
			util::strncpy(szTemp, token, CONFIG_SIZE_BUFF);
			vectString tokenList;
			CConfig::GetTokenList(&tokenList, szTemp, seps);
			if (3 > tokenList.size())
			{
				if (0 != tokenList.size())
				{
					g_pGlobal->WriteSystemLogEX(TRUE, "[Error] CPreIOCP::LoadAllServers# error !! %s\r\n", buffBackup);
					return FALSE;
				}
				continue;
			}

			if (SIZE_MAX_SERVER_NAME <= strlen(tokenList[0].c_str()))
			{
				g_pGlobal->WriteSystemLogEX(TRUE, "[Error] CPreIOCP::LoadAllServers# longer than max servergroupname !! %s\r\n", buffBackup);
				return FALSE;
			}

			char szGroupName[SIZE_MAX_SERVER_NAME ];
			util::strncpy(szGroupName, tokenList[0].c_str(), SIZE_MAX_SERVER_NAME);
			
			//int nMGameServerID = atoi(tokenList[1].c_str());
			
			BOOL bEnableServerGroup = FALSE;
			if (0 == stricmp(tokenList[2].c_str(), "TRUE")) bEnableServerGroup = true;

			if (GetServerGroupForLoading(szGroupName))
			{
				server::log(true, "[Error] CPreIOCP::LoadAllServers# Same ServerGroupName registered already !! %s\r\n", buffBackup);
				
				return false;
			}
			
			m_AllServerGroupVectorForLoading.emplace_back(szGroupName, m_nServerGroupLimiteUserCount, bool(bEnableServerGroup));

			server::log(true, "	ServerGroup: %20s, Enable(%d)\r\n", szGroupName, bEnableServerGroup);
		}
		else if (stricmp(token, "GamePublisher") == 0)
		{
			token = config.strtok(NULL, seps);
			if (token) util::strncpy(m_szGamePublisherName, token, SIZE_MAX_GAME_PUBLISHER_NAME);
		}
		else if (stricmp(token, "ServerGroupName") == 0)
		{
			token = config.strtok(nullptr, seps);
			if (token) SetServerGroupName(token);
		}

		else
		{
			server::log(true, "Unrecognized token in configuration: \"%s\"\r\n", token);

			// configuration file error!
			// return FALSE;
		}
	}

	if (m_vectGameServerGroupList.empty())
	{
		char szSystemLog[1024];
		sprintf(szSystemLog, "[Error] CPreIOCP::LoadConfiguration GameServerGroupList error !!, GameServerGroupList is empty !!\r\n");
		WriteSystemLog(szSystemLog);
		DBGOUT(szSystemLog);
		return FALSE;
	}


	DBGOUT("========================================\n");

	fin.close();

	return TRUE;
}

CServerGroup* CPreGlobal::GetServerGroupForLoading(const char* szServerGruopName)
{
	for (auto& group : m_AllServerGroupVectorForLoading)
		
		if (strnicmp(group.m_ServerGroupName, szServerGruopName, SIZE_MAX_SERVER_NAME) == 0) return &group;

	return nullptr;
}

int CPreGlobal::GetEnableServerGroupCount()
{
	int nRetCnts = 0;

	for (auto& group : m_AllServerGroupVectorForLoading) if (group.m_bEnableServerGroup) nRetCnts++;
	
	return nRetCnts;
}

bool CPreGlobal::InsertGameServerGroup(char* i_szGameServerGroupName, char* i_szPreServerIP, int i_nTab8OrderIndex)
{
	if (0 >= i_nTab8OrderIndex)
	{// 2007-09-05 by cmkwon, EXE_1에 로그인 서버 선택 인터페이스 수정 - 체크추가
		return FALSE;
	}

	int nCurCnt = m_vectGameServerGroupList.size();
	if (COUNT_MAX_GAME_SERVER_GROUP_LIST <= nCurCnt)
	{
		return FALSE;
	}

	for (int i = 0; i < nCurCnt; i++)
	{
		if (0 == stricmp(i_szPreServerIP, m_vectGameServerGroupList[i].szPreServerIP0))
		{// 2007-05-02 by cmkwon, PreServerIP가 이미 등록되어 있음, 같은 IP는 하나만 등록가능
			return FALSE;
		}
	}

	SGAME_SERVER_GROUP tmGSGroup;
	util::zero(&tmGSGroup, sizeof(tmGSGroup));
	util::strncpy(tmGSGroup.szGameServerGroupName, i_szGameServerGroupName, SIZE_MAX_GAME_SERVER_GROUP_NAME);
	util::strncpy(tmGSGroup.szPreServerIP0, i_szPreServerIP, SIZE_MAX_IPADDRESS);

#ifdef S_TEMP_PRESERVER_MAKE_HSKIM
	util::strncpy(tmGSGroup.szPreServerIP0, TEMP_PRESERVER_MAKE_IP, SIZE_MAX_IPADDRESS);
#endif

	// 2007-09-05 by cmkwon, EXE_1에 로그인 서버 선택 인터페이스 수정 - 제거함, 포트는 기본포트를 사용함
	//	tmGSGroup.usPreServerPort0			= i_usPreServerPort;
	tmGSGroup.usPreServerTab8OrderIndex = i_nTab8OrderIndex;

	m_vectGameServerGroupList.push_back(tmGSGroup);
	return TRUE;
}

bool CPreGlobal::Make_MSG_PC_CONNECT_GET_GAME_SERVER_GROUP_LIST(MSG_PC_CONNECT_GET_GAME_SERVER_GROUP_LIST_OK* o_pGameServerGroupList)
{
	int nCurCnt = min(m_vectGameServerGroupList.size(), COUNT_MAX_GAME_SERVER_GROUP_LIST);

	for (int i = 0; i < nCurCnt; ++i)
	{
		util::strncpy(o_pGameServerGroupList->arrGameServerGroupList[i].szGameServerGroupName, m_vectGameServerGroupList[i].szGameServerGroupName);

		util::strncpy(o_pGameServerGroupList->arrGameServerGroupList[i].szPreServerIP0, m_vectGameServerGroupList[i].szPreServerIP0);

		o_pGameServerGroupList->arrGameServerGroupList[i].usPreServerPort0 = this->m_pGIOCP->GetListenerPort();
	}

	return m_vectGameServerGroupList.size() <= COUNT_MAX_GAME_SERVER_GROUP_LIST;
}

bool CPreGlobal::Make_MSG_PC_CONNECT_GET_NEW_GAME_SERVER_GROUP_LIST_OK(MSG_PC_CONNECT_GET_NEW_GAME_SERVER_GROUP_LIST_OK* o_pGameServerGroupList)
{	
	int nCurCnt = min(m_vectGameServerGroupList.size(), COUNT_MAX_GAME_SERVER_GROUP_LIST);

	for (int i = 0; i < nCurCnt; ++i)
	
		o_pGameServerGroupList->arrGameServerGroupList[i] = m_vectGameServerGroupList[i];

	return m_vectGameServerGroupList.size() <= COUNT_MAX_GAME_SERVER_GROUP_LIST;
}

namespace PreGlobal
{
	constexpr auto ServerName = "Pre Server";
	constexpr auto LogFileName = "PreSystem";

	bool Initialize(HINSTANCE hInstance, UINT IDI_Icon, int nCmdShow)
	{
#ifdef SERVERCONSOLE
		AllocConsole();

		freopen("CONOUT$", "w", stdout);
		freopen("CONIN$", "r", stdin);
#endif

		g_pPreGlobal = new CPreGlobal { };
		return g_pPreGlobal->InitGlobal(ServerName, LogFileName, hInstance, IDI_Icon, nCmdShow);
	}

	void Destroy()
	{
		VMemPool::vmPoolClean();

		CIOCP::SocketClean();

		server::log("%s End\r\n\r\n\r\n", ServerName);

		delete g_pPreGlobal;

		g_pPreGlobal = nullptr;

#ifdef SERVERCONSOLE
		FreeConsole();
#endif
	}

	bool Create(HWND hWnd)
	{
		SetLastError(0);

		g_pPreGlobal->SetMainWndHandle(hWnd);

		if (!g_pPreGlobal->LoadConfiguration())
		{
			server::log(" [ERROR] PreGlobal::CreateWndClass LoadConfiguration error!!\r\n");

			MessageBox(hWnd, "Loading configuration failed!", "ERROR", MB_OK);

			return false;
		}

		if (g_pPreGlobal->GetGIOCP())  return true;

		if (!g_pPreGlobal->InitServerSocket())

			server::log(" [ERROR] PreGlobal::CreateWndClass InitServerSocket error!!\r\n");

		else
		{
			g_pPreGlobal->StartTimerTraffic();
			g_pPreGlobal->StartTimerAliveCheck();
		}

		return true;
	}

	void DestroySocket()
	{
		g_pPreGlobal->EndServerSocket();
	}

	void OnTimerTraffic()
	{
		g_pPreGlobal->CheckServerThread();
		g_pPreGlobal->CalculateIOCPTraffic();
	}
	void OnTimerPingAlive()
	{
		g_pPreGlobal->CheckClientAlive();
	}
}