// LogGlobal.cpp: implementation of the CLogGlobal class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
// 2008-11-21 by cmkwon, LogServer를 MFC에서 Win32 프로젝트로 변경 - 
//#include "LogServer.h"
#include "LogGlobal.h"
#include "LogIOCP.h"
#include "Config.h"

// 2008-11-21 by cmkwon, LogServer를 MFC에서 Win32 프로젝트로 변경 - 
//#ifdef _DEBUG
//#undef THIS_FILE
//static char THIS_FILE[]=__FILE__;
//#define new DEBUG_NEW
//#endif

CLogGlobal *					g_pLogGlobal = NULL;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLogGlobal::CLogGlobal()
{
	if (g_pLogGlobal) return;
	
	g_pLogGlobal = this;

	util::zero(m_szGamePublisherName);	// 2010-06-01 by shcho, GLogDB 관련 -

	m_bArenaFieldServerCheck = false;		// 2011-12-12 by hskim, GLog 2차
}

CLogGlobal::~CLogGlobal()
{
	EndServerSocket();

	g_pGlobal = nullptr;
}

void CLogGlobal::VMemAlloc()
{
	VMemPool::vmPoolAddObject(sizeof(COverlapped), SIZE_MAX_LOGSERVER_SESSION * 100);
}


bool CLogGlobal::InitServerSocket()
{
	if (m_pGIOCP) return false;
 
	server::log(" [Notify] CLogGlobal::InitServerSocket# Starting... \r\n");

	m_pGIOCP = new CLogIOCP(m_nPortListening, m_szIPLocal);

	if (!m_pGIOCP->IOCPInit())
	{
		constexpr auto msg = " [Error] LogServer IOCPInit Error\r\n";

		server::log(msg);

		MessageBox(NULL, msg, "ERROR", MB_OK);

		return false;
	}
	
	server::log(" [Notify] CLogGlobal::InitServerSocket# Started \r\n");

	return TRUE;
}

bool CLogGlobal::EndServerSocket()
{
	// 2009-03-19 by cmkwon, 시스템 로그 추가 - 아래와 같이 수정
	server::log("  [Notify] CLogGlobal::EndServerSocket# \r\n");

	CGlobalGameServer::EndServerSocket();

	return TRUE;
}


BOOL CLogGlobal::LoadConfiguration(void)
{
	DBGOUT("========================================\n");
	DBGOUT("Loading Server Informations...\n");

	const int SIZE_BUFF = 512;
	string configFileName = CONFIG_ROOT + string("./log.cfg");
	char buff[SIZE_BUFF];
	char buffBackup[SIZE_BUFF];		// 2008-07-11 by cmkwon, MySQL 지원을 위해 수정(IP,Port 로 DB 서버 연결) - 
	const char *token;
	char seps[] = " \t";

	/////////////////////////////////////////////
	// load MapWorkspace file...
	ifstream fin;
	CConfig config;

	if (!config.LoadUserDefinedVariables(GLOBAL_CONFIG_FILE_PATH))
	{
		server::log(" [Error] LoadConfiguration LoadUserDefinedVariables() error, FileName[%s]\r\n",
			GLOBAL_CONFIG_FILE_PATH);

		return FALSE;
	}

	if (!config.LoadUserDefinedVariables(configFileName.c_str()))
	{
		server::log(" [Error] CPreIOCP::LoadAllServers Config Initialization Error1\r\n");

		return FALSE;
	}

	fin.open(configFileName.c_str());
	if (! fin.is_open())
	{
		server::log(" [Error] CPreIOCP::LoadAllServers Can't open config file\r\n");

		return FALSE;
	}

	while (true)
	{
		util::zero(buff);
		BOOL bFlagFileEnd = fin.getline(buff, SIZE_BUFF).eof();
		if(bFlagFileEnd == TRUE
			&& strcmp(buff, "") == 0)
		{
			break;
		}

		util::strncpy(buffBackup, buff);	// 2008-07-11 by cmkwon, MySQL 지원을 위해 수정(IP,Port 로 DB 서버 연결) - 

		token = config.strtok(buff, seps);

		if (token == NULL
			|| strncmp(token, "#", 1) == 0
			|| strncmp(token, "$", 1) == 0)
		{	// ignore blank lines
			// ignore comments
			// ignore user defined variables

			continue;
		}

		if(stricmp(token, "LocalIPAddress") == 0)
		{
			token = config.strtok(NULL, seps);
			if (token == NULL)
			{
				char *msg = "Loading Configuration File Failed.\n";
				DBGOUT(msg);
				MessageBox(NULL, msg, "Error", MB_OK);
				return FALSE;
			}
			if(strcmp(token, "") == 0)
			{
				this->SetIPLocal("127.0.0.1");
			}
			else
			{
				this->SetIPLocal((char*)token);
			}
		}
		else if(stricmp(token, "LocalPublicIPAddress") == 0)
		{// 2006-05-23 by cmkwon,
			token = config.strtok(NULL, seps);
			if (token == NULL)
			{
				char *msg = "Loading Configuration File Failed.\n";
				DBGOUT(msg);
				MessageBox(NULL, msg, "Error", MB_OK);
				return FALSE;
			}
			this->SetPublicIPLocal((char*)token);
		}
		else if(stricmp(token, "LocalListenPort") == 0)
		{
			token = config.strtok(NULL, seps);
			if (token == NULL)
			{
				char *msg = "Loading Configuration File Failed.\n";
				DBGOUT(msg);
				MessageBox(NULL, msg, "Error", MB_OK);
				return FALSE;
			}
			if(strcmp(token, "") != 0)
			{
				this->SetPortListening(atoi(token));
			}
		}
		else if(stricmp(token, "ODBCDSN") == 0)
		{
			token = config.strtok(NULL, seps);
			if (token == NULL)
			{
				char *msg = "Loading Configuration File Failed. ODBCDSN\n";
				DBGOUT(msg);
				MessageBox(NULL, msg, "Error", MB_OK);
				return FALSE;
			}
			if(strcmp(token, "") != 0)
			{
				util::strncpy((char*)m_szODBCDSN, token, SIZE_MAX_ODBC_CONN_STRING);
			}
		}
		else if(_stricmp(token, "ODBCUID") == 0)
		{
			token = config.strtok(NULL, seps);
			
			if (!token)
			{
				constexpr auto msg = "Loading Configuration File Failed. ODBCUID\n";
				
				MessageBox(NULL, msg, "Error", MB_OK);

				return false;
			}

			if (*token != '\0') util::strncpy(m_szODBCUID, token);
			
		}
		else if(_stricmp(token, "ODBCPASSWD") == 0)
		{
			token = config.strtok(NULL, seps);
			
			if (!token)
			{
				constexpr auto msg = "Loading Configuration File Failed. ODBCPASSWD\n";
				
				MessageBox(NULL, msg, "Error", MB_OK);

				return false;
			}

			if (strcmp(token, "") != 0) util::strncpy(m_szODBCPASSWORD, token);
		}
		else if(_stricmp(token, "ServerGroupName") == 0)
		{
			token = config.strtok(NULL, seps);
			if(strcmp(token, "") != 0)
			{
				this->SetServerGroupName((char*)token);
			}
		}
		else if(0 == stricmp(token, CONFIG_NAME_DB_SERVER_IP))
		{// 2008-07-11 by cmkwon, MySQL 지원을 위해 수정(IP,Port 로 DB 서버 연결) - 
			
			token = config.strtok(NULL, seps);
			if (NULL == token)
			{
				char szErr[1024];
				sprintf(szErr, "[ERROR] CLogGlobal::LoadConfiguration error !! %s, %s\r\n", CONFIG_NAME_DB_SERVER_IP, buffBackup);
				MessageBox(NULL, szErr, "Error", MB_OK);
				return FALSE;
			}
			
			util::strncpy(m_szDBServerIP, token, SIZE_MAX_ODBC_CONN_STRING);
		}
		else if(0 == stricmp(token, CONFIG_NAME_DB_SERVER_PORT))
		{// 2008-07-11 by cmkwon, MySQL 지원을 위해 수정(IP,Port 로 DB 서버 연결) - 
			
			token = config.strtok(NULL, seps);
			if (NULL == token)
			{
				char szErr[1024];
				sprintf(szErr, "[ERROR] CLogGlobal::LoadConfiguration error !! %s, %s\r\n", CONFIG_NAME_DB_SERVER_PORT, buffBackup);
				MessageBox(NULL, szErr, "Error", MB_OK);
				return FALSE;
			}
			
			m_nDBServerPort = atoi(token);			
		}
		else if(0 == stricmp(token, CONFIG_NAME_DB_SERVER_DATABASE_NAEE))
		{// 2008-07-11 by cmkwon, MySQL 지원을 위해 수정(IP,Port 로 DB 서버 연결) - 
			
			token = config.strtok(NULL, seps);
			if (NULL == token)
			{
				char szErr[1024];
				sprintf(szErr, "[ERROR] CLogGlobal::LoadConfiguration error !! %s, %s\r\n", CONFIG_NAME_DB_SERVER_DATABASE_NAEE, buffBackup);
				MessageBox(NULL, szErr, "Error", MB_OK);
				return FALSE;
			}
			
			util::strncpy(m_szDBServerDatabaseName, token, SIZE_MAX_ODBC_CONN_STRING);
		}
		else if(0 == stricmp(token, "AllowedToolIP"))
		{// 2008-06-05 by cmkwon, AdminTool, Monitor 접근 가능 IP를 server config file 에 설정하기 - 
			
			token = config.strtok(NULL, seps);			
			if (token == NULL)
			{
				continue;
			}
			this->AddAllowedToolIP(token);
		}
		else if(stricmp(token, "ServerGroupInfo") == 0)
		{// 2008-09-24 by cmkwon, global.cfg 형식 수정 - ServerGroup 을 수정한 것임
			
			// 2008-09-24 by cmkwon, Syntax						= [ServerGroupName]			[ServerGroupID]		[Enable Flag]
			token = config.strtok(NULL, seps);
			if (NULL == token)
			{
				continue;
			}
			char szTemp[CONFIG_SIZE_BUFF];
			util::strncpy(szTemp, token, CONFIG_SIZE_BUFF);
			vectString tokenList;
			CConfig::GetTokenList(&tokenList, szTemp, seps);
			if(3 > tokenList.size())
			{
				if(0 != tokenList.size())
				{
					g_pGlobal->WriteSystemLogEX(TRUE, "[Error] CLogGlobal::LoadConfiguration# error !! %s\r\n", buffBackup);
					return FALSE;
				}
				continue;
			}
			
			if(SIZE_MAX_SERVER_NAME <= strlen(tokenList[0].c_str()))
			{
				g_pGlobal->WriteSystemLogEX(TRUE, "[Error] CLogGlobal::LoadConfiguration# longer than max servergroupname !! %s\r\n", buffBackup);
				return FALSE;
			}
			
			char	szGroupName[SIZE_MAX_SERVER_NAME];
			util::strncpy(szGroupName, tokenList[0].c_str(), SIZE_MAX_SERVER_NAME);
			int		nMGameServerID		= atoi(tokenList[1].c_str());			
			BOOL	bEnableServerGroup	= FALSE;			
			if(0 == stricmp(tokenList[2].c_str(), "TRUE"))
			{
				bEnableServerGroup	= TRUE;
			}
			
			//if(nMGameServerID < SERVERGROUP_START_ID)
			//{
			//	g_pGlobal->WriteSystemLogEX(TRUE, "[Error] CLogGlobal::LoadConfiguration# smaller than min ServerGroupID !! %s\r\n", buffBackup);
			//	return FALSE;
			//}
			
			//if (FALSE == InsertServerGroupforMGame(szGroupName, nMGameServerID))
			//{
			//	g_pGlobal->WriteSystemLogEX(TRUE, "[Error] CLogGlobal::LoadConfiguration# Same ServerGroupName registered already !! %s\r\n", buffBackup);
			//	return FALSE;
			//}
			
			g_pGlobal->WriteSystemLogEX(TRUE, "	ServerGroup: %20s, Enable(%d) MGameServerID(%5d)\r\n", szGroupName, bEnableServerGroup, nMGameServerID);
		}
		else if(0 == stricmp(token, "GamePublisher"))
		{	// 2010-06-01 by shcho, GLogDB 관련 -
			token = config.strtok(NULL, seps);
			if(token)
			{
				util::strncpy(m_szGamePublisherName, token, SIZE_MAX_GAME_PUBLISHER_NAME);
			}
		}
		// start 2011-12-12 by hskim, GLog 2차
		else if(0 == stricmp(token, "ArenaServerFlag"))
		{
			token = config.strtok(NULL, seps);
			if(NULL == token)
			{
				char	szSystemLog[256];
				sprintf(szSystemLog, "Loading Configuration File Failed: ArenaServerLoading\r\n");
				this->WriteSystemLog(szSystemLog);
				DBGOUT(szSystemLog);
				return FALSE;
			}
			
			if(0 == stricmp(token, "TRUE"))
			{
				m_bArenaFieldServerCheck	= TRUE;
			}
			else if(0 == stricmp(token, "FALSE"))
			{
				m_bArenaFieldServerCheck	= FALSE;
			}
		}
		// end 2011-12-12 by hskim, GLog 2차
		else
		{
			WriteSystemLogEX(true, "Unrecognized token in configuration: \"%s\"\r\n", token);

			// configuration file error!
			//assert(0);
		}
	}
	fin.close();
	DBGOUT("========================================\n");
	
	//m_nMGameServerID1 = GetServerIDforMGame(GetServerGroupName());
	return TRUE;
}

char *CLogGlobal::GetGamePublisherName(void)
{	// 2010-06-01 by shcho, GLogDB 관련 -
	return m_szGamePublisherName;
}
