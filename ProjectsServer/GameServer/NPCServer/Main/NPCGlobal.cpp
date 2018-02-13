// NPCGlobal.cpp: implementation of the CNPCGlobal class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NPCGlobal.h"
#include "N2FSocket.h"
#include "NPCIOCP.h"
#include "Config.h"

CNPCGlobal *			g_pNPCGlobal = NULL;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNPCGlobal::CNPCGlobal()
{
	if(g_pNPCGlobal)
	{
		return;
	}
	g_pNPCGlobal = this;

	memset(m_szIPFieldServer, 0x00, SIZE_MAX_IPADDRESS);
	m_nPortFieldServer			= 0;

	m_pN2FSocket				= NULL;

	m_bIsArenaServer			= FALSE;		// 2008-09-10 by cmkwon, 통합아레나 맵설정(map.cfg) 관련 수정 - 
}

CNPCGlobal::~CNPCGlobal()
{
	CNPCGlobal::EndServerSocket();
	DestroyN2FSocket();

	g_pGlobal = nullptr;
}


///////////////////////////////////////////////////////////////////////////
// Property
///////////////////////////////////////////////////////////////////////////
void CNPCGlobal::SetIPFieldServer(char *i_szIP)
{
	strncpy_s(m_szIPFieldServer, i_szIP, SIZE_MAX_IPADDRESS);
}

char *CNPCGlobal::GetIPFieldServer(void)
{
	return m_szIPFieldServer;
}

void CNPCGlobal::SetPortFieldServer(int i_nPort)
{
	m_nPortFieldServer = i_nPort;
}

int CNPCGlobal::GetPortFieldServer(void)
{
	return m_nPortFieldServer;
}

CN2FSocket * CNPCGlobal::GetN2FSocket(void)
{
	return m_pN2FSocket;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CNPCGlobal::WriteN2FSocket(BYTE *pData, int nSize)
/// \brief		// 2007-11-26 by cmkwon, 몬스터 자동삭제 메시지 TCP로 전송(N->F) - CNPCGlobal::WriteN2FSocket() 추가
/// \author		cmkwon
/// \date		2007-11-26 ~ 2007-11-26
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CNPCGlobal::WriteN2FSocket(const BYTE *pData, int nSize) const
{
	if(NULL == m_pN2FSocket
		|| FALSE == m_pN2FSocket->IsConnected())
	{
		return FALSE;
	}

	return m_pN2FSocket->Write(LPCSTR(pData), nSize);
}

///////////////////////////////////////////////////////////////////////////
// Method
///////////////////////////////////////////////////////////////////////////
BOOL CNPCGlobal::CreateN2FSocket(HWND i_hWnd)
{
	if(m_pN2FSocket)
	{
		return FALSE;
	}

	m_pN2FSocket = new CN2FSocket(i_hWnd, WM_N2F_ASYNC_EVENT, WM_N2F_PACKET_NOTIFY);
	return TRUE;
}

void CNPCGlobal::DestroyN2FSocket(void)
{
	// 2009-03-19 by cmkwon, 시스템 로그 추가 - IMServer 
	this->WriteSystemLogEX(TRUE, "  [Notify] CNPCGlobal::DestroyN2FSocket# 0x%X IsConnected(%d) \r\n", m_pN2FSocket, (m_pN2FSocket)?m_pN2FSocket->IsConnected():FALSE);

	if(m_pN2FSocket && m_pN2FSocket->IsConnected())
	{
		m_pN2FSocket->CloseSocket();
	}
	util::del(m_pN2FSocket);
}

///////////////////////////////////////////////////////////////////////////
// virtual Function
///////////////////////////////////////////////////////////////////////////
bool CNPCGlobal::InitServerSocket()
{
	if (!GetN2FSocket() || !GetN2FSocket()->IsConnected()) return false;
	
	if (m_pGIOCP) return false;

	server::log(" [Notify] CNPCGlobal::InitServerSocket# Starting... \r\n");

	auto iocp = new CNPCIOCP(m_nPortListening, m_szIPLocal);
	
	m_pGIOCP = iocp;

	if (!m_pGIOCP->IOCPInit())
	{
		constexpr auto msg = " [Error] NPCServer IOCPInit Error\r\n";

		server::log(msg);

		MessageBox(NULL, msg, "ERROR", MB_OK);

		return FALSE;
	}

	if (!iocp->OpenUDPPortForOtherServer()) return false;
	
	server::log(" [Notify] CNPCGlobal::InitServerSocket# Started \r\n");

	return true;
}

bool CNPCGlobal::EndServerSocket()
{ 
	server::log(" [Notify] CNPCGlobal::EndServerSocket# \r\n");

	CGlobalGameServer::EndServerSocket();

	return TRUE;
}

void CNPCGlobal::VMemAlloc()
{
	VMemPool::vmPoolAddObject(sizeof(ActionInfo), SIZE_MAX_FIELDSERVER_SESSION * 5);
	VMemPool::vmPoolAddObject(sizeof(COverlapped), SIZE_MAX_UDP_CHANNEL_SESSION * 20);		// 2010-04-09 by cmkwon, NPCServer 메모리 부족으로 수정함 - 기존 세션당 100개
	VMemPool::vmPoolAddObject(sizeof(EVENTINFO), SIZE_MAX_UDP_CHANNEL_SESSION * 200);			// 2010-04-14 by cmkwon, 서버 메모리 부족 문제 수정 - 맵당 200개의 타일 이벤트 정보 설정 예상
	VMemPool::vmPoolAddObject(sizeof(CSendPacket), 1000);
	VMemPool::vmPoolAddObject(sizeof(CRecvPacket), 100);
}

BOOL CNPCGlobal::LoadConfiguration(void)
{
	const int SIZE_BUFF = 512;
	char		buff[SIZE_BUFF];
	char		buffBackup[SIZE_BUFF];	// 2008-07-11 by cmkwon, MySQL 지원을 위해 수정(IP,Port 로 DB 서버 연결) - 
	const char	*token;
	char		seps[] = " \t";
	ifstream	fin;
	CConfig		config;

	if (!config.LoadUserDefinedVariables(GLOBAL_CONFIG_FILE_PATH))
	{
		char	szError[1024];
		sprintf(szError, "[Error] LoadConfiguration LoadUserDefinedVariables() error, FileName[%s]\r\n"
			, FIELD_SERVER_CONFIG_FILE_PATH);
		this->WriteSystemLog(szError);
		DBGOUT(szError);
		return FALSE;
	}

	if (!config.LoadUserDefinedVariables(NPC_SERVER_CONFIG_FILE_PATH))
	{
		char	szError[1024];
		sprintf(szError, "[Error] LoadConfiguration LoadUserDefinedVariables() error, FileName[%s]\r\n"
			, NPC_SERVER_CONFIG_FILE_PATH);
		this->WriteSystemLog(szError);
		DBGOUT(szError);
		return FALSE;
	}

	fin.open(NPC_SERVER_CONFIG_FILE_PATH);
	if (!fin.is_open())
	{	// cannot open file...

		int nErr = GetLastError();
		SetLastError(0);
		char	szError[1024];
		sprintf(szError, "[Error] LoadConfiguration open() error, LastError[%d] ConfigFileName[%s]\r\n"
			, nErr, NPC_SERVER_CONFIG_FILE_PATH);
		this->WriteSystemLog(szError);
		DBGOUT(szError);
		return FALSE;
	}

	while(TRUE)
	{
		memset(buff, 0x00, SIZE_BUFF);
		BOOL bFlagFileEnd = fin.getline(buff, SIZE_BUFF).eof();
		if(bFlagFileEnd == TRUE
			&& strcmp(buff, "") == 0)
		{
			break;
		}

		util::strncpy(buffBackup, buff, SIZE_BUFF);	// 2008-07-11 by cmkwon, MySQL 지원을 위해 수정(IP,Port 로 DB 서버 연결) - 

		token = strtok(buff, seps);

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
				char *msg = "Loading Configuration File Failed1.\n";
				DBGOUT(msg);
				MessageBox(NULL, msg, "Error", MB_OK);
				return FALSE;
			}
			this->SetIPLocal((char*)token);
		}
		else if(stricmp(token, "LocalPublicIPAddress") == 0)
		{// 2006-05-23 by cmkwon,
			token = config.strtok(NULL, seps);
			if (token == NULL)
			{
				char *msg = "Loading Configuration File Failed2.\n";
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
				char *msg = "Loading Configuration File Failed3.\n";
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
				char *msg = "Loading Configuration File Failed6. ODBCDSN\n";
				DBGOUT(msg);
				MessageBox(NULL, msg, "Error", MB_OK);
				return FALSE;
			}
			if(strcmp(token, "") != 0)
			{
				util::strncpy((char*)m_szODBCDSN, token, SIZE_MAX_ODBC_CONN_STRING);
			}
		}
		else if(stricmp(token, "ODBCUID") == 0)
		{
			token = config.strtok(NULL, seps);
			if (token == NULL)
			{
				char *msg = "Loading Configuration File Failed7. ODBCUID\n";
				DBGOUT(msg);
				MessageBox(NULL, msg, "Error", MB_OK);
				return FALSE;
			}
			if(strcmp(token, "") != 0)
			{
				util::strncpy((char*)m_szODBCUID, token, SIZE_MAX_ODBC_CONN_STRING);
				///////////////////////////////////////////////////////////////////////////////
				// 2008-09-01 by cmkwon, global.cfg 파일에 DB ID/PWD 암호화 - 
				BYTE byEncodedBinary[1024];
				util::zero(byEncodedBinary, 1024);
				/*if(XOR::XORString2Binary(byEncodedBinary, (char*)token))
				{
					util::zero(m_szODBCUID, SIZE_MAX_ODBC_CONN_STRING);
					XOR::XOREncode((BYTE*)m_szODBCUID, byEncodedBinary, strlen(token)/2, STR_XOR_KEY_STRING_DB_ID_PWD);
				}*/
			}
		}
		else if(stricmp(token, "ODBCPASSWD") == 0)
		{
			token = config.strtok(NULL, seps);
			if (token == NULL)
			{
				char *msg = "Loading Configuration File Failed8. ODBCPASSWD\n";
				DBGOUT(msg);
				MessageBox(NULL, msg, "Error", MB_OK);
				return FALSE;
			}
			if(strcmp(token, "") != 0)
			{
				util::strncpy((char*)m_szODBCPASSWORD, token, SIZE_MAX_ODBC_CONN_STRING);
				///////////////////////////////////////////////////////////////////////////////
				// 2008-09-01 by cmkwon, global.cfg 파일에 DB ID/PWD 암호화 - 
				BYTE byEncodedBinary[1024];
				util::zero(byEncodedBinary, 1024);
				/*if(XOR::XORString2Binary(byEncodedBinary, (char*)token))
				{
					util::zero(m_szODBCPASSWORD, SIZE_MAX_ODBC_CONN_STRING);
					XOR::XOREncode((BYTE*)m_szODBCPASSWORD, byEncodedBinary, strlen(token)/2, STR_XOR_KEY_STRING_DB_ID_PWD);
				}*/
			}
		}
		else if(stricmp(token, "FieldServer") == 0)
		{
			token = config.strtok(NULL, seps);
			if(strcmp(token, "") != 0)
			{
				this->SetIPFieldServer((char*)token);
			}
			token = config.strtok(NULL, seps);
			if(strcmp(token, "") != 0)
			{
				this->SetPortFieldServer(atoi(token));
			}
		}
		else if(0 == stricmp(token, CONFIG_NAME_DB_SERVER_IP))
		{// 2008-07-11 by cmkwon, MySQL 지원을 위해 수정(IP,Port 로 DB 서버 연결) - 
			
			token = config.strtok(NULL, seps);
			if (NULL == token)
			{
				char szErr[1024];
				sprintf(szErr, "[ERROR] CNPCGlobal::LoadConfiguration error !! %s, %s\r\n", CONFIG_NAME_DB_SERVER_IP, buffBackup);
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
				sprintf(szErr, "[ERROR] CNPCGlobal::LoadConfiguration error !! %s, %s\r\n", CONFIG_NAME_DB_SERVER_PORT, buffBackup);
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
				sprintf(szErr, "[ERROR] CNPCGlobal::LoadConfiguration error !! %s, %s\r\n", CONFIG_NAME_DB_SERVER_DATABASE_NAEE, buffBackup);
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
		else if(0 == stricmp(token, "ArenaServerFlag"))
		{// 2008-09-10 by cmkwon, 통합아레나 맵설정(map.cfg) 관련 수정 - 
			token = config.strtok(NULL, seps);
			if(NULL == token)
			{				
				this->WriteSystemLogEX(TRUE, "Loading Configuration File failed !! %s \r\n", buffBackup);
				return FALSE;
			}
			if(0 == stricmp(token, "TRUE"))
			{
				m_bIsArenaServer	= TRUE;
			}
		}// 2008-09-10 by cmkwon, 통합아레나 맵설정(map.cfg) 관련 수정 - 
		// 2013-06-12 by hskim, 하나의 OS 에서 서버 동시 실행
#ifdef S_DISABLE_ALONE_RUN_MODE_HSKIM
		else if(stricmp(token, "ServerGroupName") == 0)
		{
			token = config.strtok(NULL, seps);
			if(strcmp(token, "") != 0)
			{
				this->SetServerGroupName((char*)token);
			}
		}
#endif
		// 2013-06-12 by hskim, 하나의 OS 에서 서버 동시 실행
		else
		{
			WriteSystemLogEX(true, "Unrecognized token in configuration: \"%s\"\r\n", token);

			// configuration file error!
			//assert(0);
		}
	}
	fin.close();

	if(strcmp(this->GetIPLocal(), "") == 0
		|| strcmp(this->GetIPFieldServer(), "") == 0
		|| this->GetPortFieldServer() == 0)
	{
		char	szError[1024];
		sprintf(szError, "[Error] LoadConfiguration() Loading Configuration Failed, LocalIP[%s] FieldServer[%s:%d]\r\n"
			, this->GetIPLocal(), this->GetIPFieldServer(), this->GetPortFieldServer());
		this->WriteSystemLog(szError);
		DBGOUT(szError);

		MessageBox(NULL, "Loading Configuration File Failed.100", "ERROR", MB_OK);
		return FALSE;
	}
	return true;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CNPCGlobal::GetIsArenaServer(void)
/// \brief		// 2008-09-10 by cmkwon, 통합아레나 맵설정(map.cfg) 관련 수정 - 
/// \author		cmkwon
/// \date		2008-09-10 ~ 2008-09-10
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CNPCGlobal::GetIsArenaServer(void)
{
	return m_bIsArenaServer;
}
