// GlobalGameServer.cpp: implementation of the CGlobalGameServer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GlobalGameServer.h"
#include "IOCP.h"
#include "ODBCStatement.h"
#include "md5_lib_src.h"
#include "AtumDBManager.h"
#include <timeapi.h>
#include <fstream>

#pragma region Command Lists

char *ArrCommandList[] = {
	// Field Server
	STRCMD_CS_COMMAND_ITEMINFObyKIND_HELP,
	STRCMD_CS_COMMAND_ITEMINSERTbyKIND_HELP,
	STRCMD_CS_COMMAND_ITEMINSERTbyITEMNUMRANGE_HELP,
	STRCMD_CS_COMMAND_QUESTINFO_HELP,
	STRCMD_CS_COMMAND_QUESTDEL_HELP,
	STRCMD_CS_COMMAND_QUESTCOMPLETION_HELP,			// 2005-12-07 by cmkwon
	STRCMD_CS_COMMAND_STATINIT_HELP,
	STRCMD_CS_COMMAND_PARTYINFO_HELP,
	STRCMD_CS_COMMAND_GAMETIME_HELP,
	STRCMD_CS_COMMAND_STRINGLEVEL_HELP,
	STRCMD_CS_COMMAND_SKILLALL_HELP,
	STRCMD_CS_COMMAND_ITEMINSERTALL_HELP,
	STRCMD_CS_COMMAND_ITEMINSERTWEAPON_HELP,
	STRCMD_CS_COMMAND_ITEMDELALL_HELP,
	STRCMD_CS_COMMAND_ITEMINSERTbyITEMNUM_HELP,
	STRCMD_CS_COMMAND_ITEMDROP_HELP,
	STRCMD_CS_COMMAND_USERSINFOTOTAL_HELP,
	STRCMD_CS_COMMAND_DEBUGPRINTDBG_HELP,
	STRCMD_CS_COMMAND_DEBUGSETPARAMF_HELP,
	STRCMD_CS_COMMAND_BULLETCHARGE_HELP,
	STRCMD_CS_COMMAND_REPAIRALL_HELP,
	STRCMD_CS_COMMAND_REPAIRbyPARAM_HELP,
	STRCMD_CS_COMMAND_USERNORMALIZE_HELP,
	STRCMD_CS_COMMAND_USERSPECIALIZE_HELP,
	STRCMD_CS_COMMAND_POWERUP_HELP,
	STRCMD_CS_COMMAND_VARIABLESET_HELP,
	STRCMD_CS_COMMAND_LEVELSET_HELP,
	STRCMD_CS_COMMAND_PARTNERLEVELSET_HELP,		// 2011-09-05 by hskim, 파트너 시스템 2차
	STRCMD_CS_COMMAND_DEBUGPRINTMSGF_HELP,
	STRCMD_CS_COMMAND_GAMEEVENT_HELP,
	STRCMD_CS_COMMAND_PREMEUM_HELP,
	// 2008-02-14 by cmkwon, 도시점령전 명령어 제거
	//	STRCMD_CS_COMMAND_CITYWAR_HELP,
	STRCMD_CS_COMMAND_HAPPYHOUREVENT_HELP,
	STRCMD_CS_COMMAND_BONUSSTAT_HELP,

	// 2008-09-09 by cmkwon, /세력소환 명령어 인자 리스트에 기어타입 추가 - 
	//	STRCMD_CS_COMMAND_COMEONINFL_HELP,				// 2006-07-27 by cmkwon
	STRCMD_CS_COMMAND_COMEONINFL_HELP2,				// 2008-09-09 by cmkwon, /세력소환 명령어 인자 리스트에 기어타입 추가 - 

	STRCMD_CS_COMMAND_ITEMINMAP_HELP,				// 2006-07-27 by cmkwon
	STRCMD_CS_COMMAND_DISTRIBUTIONLEVEL_HELP,		// 2006-08-09 by dhjin
	STRCMD_CS_COMMAND_BONUSSTAT_POINT_HELP,
	STRCMD_CS_COMMAND_PCBANGUSERCOUNT_HELP,
	STRMSG_CS_COMMAND_WARPOINT_HELP,
	STRCMD_CS_COMMAND_WAPPOINTINMAP_HELP,		// 2007-11-05 by cmkwon, WP 지급 명령어 구현 -
	STRCMD_CS_COMMAND_ITEMALLUSER_HELP,			// 2008-02-20 by cmkwon, 명령어 추가(접속 중인 유저 모두에게 아이템 지급 - 

	// 2015-11-25 Future, IP Restriction System Commands
	STRCMD_CS_COMMAND_MULTI_IP_RESTRICTION_HELP_0,
	STRCMD_CS_COMMAND_MULTI_IP_RESTRICTION_HELP_1,
	STRCMD_CS_COMMAND_MULTI_IP_RESTRICTION_HELP_2,

	// IM Server
	STRCMD_CS_COMMAND_DEBUGSETPARAMI_HELP,
	STRCMD_CS_COMMAND_REGISTERADMIN_HELP,
	STRCMD_CS_COMMAND_DEBUGPRINTMSGI_HELP,
	STRCMD_CS_COMMAND_SERVERDOWN_HELP,
	STRCMD_CS_COMMAND_GUILDINFO_HELP,
	STRCMD_CS_COMMAND_WEATHERSET_HELP,
	STRCMD_CS_COMMAND_CONPOINT_HELP,
	STRCMD_CS_COMMAND_TOGGLE_CNC_HELP,
	nullptr
};

// 2007-11-08 by cmkwon, GM 명령어 정리 - 운영자 Command List
char *ArrGameMasterCommandList[] = {
	// Field Server
	STRCMD_CS_COMMAND_MOVE_HELP,
	STRCMD_CS_COMMAND_COORDINATE_HELP,
	STRCMD_CS_COMMAND_LIST_HELP,
	STRCMD_CS_COMMAND_USERSEND_HELP,
	STRCMD_CS_COMMAND_INFObyNAME_HELP,
	STRCMD_CS_COMMAND_MONSUMMON_HELP,
	STRCMD_CS_COMMAND_USERSINFOperMAP_HELP,
	STRCMD_CS_COMMAND_CHANNELINFO_HELP,
	STRCMD_CS_COMMAND_USERINVINCIBILITY_HELP,
	STRCMD_CS_COMMAND_USERINVISIABLE_HELP,
	STRCMD_CS_COMMAND_STEALTH_HELP,
	STRCMD_CS_COMMAND_RETURNALL_HELP,
	STRCMD_CS_COMMAND_CHANGEINFL_HELP,				// 2006-02-08 by cmkwon
	STRCMD_CS_COMMAND_STRATEGYPOINTINFO_HELP,
	STRCMD_CS_COMMAND_OBSERVER_REG_HELP,
	STRMSG_CS_COMMAND_WATCH_START_INFO_HELP,
	STRMSG_CS_COMMAND_WATCH_END_INFO_HELP,
	STRMSG_CS_COMMAND_ARENA_TEAM_INFO_HELP,
	STRCMD_CS_COMMAND_ARENAFORBID_HELP,			// 2007-07-11 by cmkwon, 아레나블럭 시스템 구현 - /? 결과리스트에 추가
	STRCMD_CS_COMMAND_ARENAFORBIDRelease_HELP,	// 2007-07-11 by cmkwon, 아레나블럭 시스템 구현 - /? 결과리스트에 추가
	STRCMD_CS_COMMAND_UsableSpeakerItem_HELP,	// 2007-08-27 by cmkwon, 추가
	STRCMD_CS_COMMAND_NEWACCOUNTBLOCK_HELP,		// 2008-01-31 by cmkwon, 계정 블럭/해제 명령어로 가능한 시스템 구현 -
	STRCMD_CS_COMMAND_NEWACCOUNTUNBLOCK_HELP,	// 2008-01-31 by cmkwon, 계정 블럭/해제 명령어로 가능한 시스템 구현 -

	STRCMD_CS_COMMAND_TEAMARENALEAVE_HELP, 		// 2008-03-10 by dhjin, 아레나 통합 - 
	STRCMD_CS_COMMAND_TARGETARENALEAVE_HELP, 	// 2008-03-10 by dhjin, 아레나 통합 - 
	STRCMD_CS_COMMAND_ARENAEVENT_HELP, 			// 2008-03-10 by dhjin, 아레나 통합 - 
	STRCMD_CS_COMMAND_ARENAEVENTRELEASE_HELP,	// 2008-03-10 by dhjin, 아레나 통합 -
	STRCMD_CS_COMMAND_INFLUENCEMARKEVENT_HELP,	// 2008-08-18 by dhjin, 세력마크이벤트
	STRCMD_CS_COMMAND_INFLUENCEMARKEVENTEND_HELP,	// 2008-08-18 by dhjin, 세력마크이벤트
	STRCMD_CS_COMMAND_PCBANGRELOADTIME_HELP,	// 2008-08-25 by dhjin, 태국 PC방 IP정보 로딩
	STRCMD_CS_COMMAND_KICK_HELP,				// 2008-09-09 by cmkwon, "/kick" 명령어 추가 - 

	STRCMD_CS_COMMAND_INFINITY_NEXT_SCENE,		// 2010. 06. 04 by hsLee 인티피니 필드 2차 난이도 조절. (GM 명령어 추가. /nextscene(다음 시네마 씬 호출.) )

	// IM Server
	STRCMD_CS_COMMAND_WHO_HELP,
	STRCMD_CS_COMMAND_GOUSER_HELP,
	STRCMD_CS_COMMAND_COMEON_HELP,
	STRCMD_CS_COMMAND_GUILDCOMEON_HELP,
	STRCMD_CS_COMMAND_GUILDSEND_HELP,
	STRCMD_CS_COMMAND_CHATFORBID_HELP,
	STRCMD_CS_COMMAND_CHATFORBIDRELEASE_HELP,
	STRCMD_CS_COMMAND_VIEWCALLGM_HELP,				// 2006-05-09 by cmkwon
	STRCMD_CS_COMMAND_BRINGCALLGM_HELP,				// 2006-05-09 by cmkwon
	STRCMD_CS_COMMAND_PrepareShutdown_HELP,			// 2007-08-27 by cmkwon, 서버다운준비 명령어 추가(SCAdminTool에서 SCMonitor의 PrepareShutdown을 진행 할 수 있게) - 
	STRCMD_CS_COMMAND_STARTCALLGM_HELP,				// 2007-11-19 by cmkwon, 진정시스템 업데이트 - /? 결과에 추가
	STRCMD_CS_COMMAND_ENDCALLGM_HELP,				// 2007-11-19 by cmkwon, 진정시스템 업데이트 - /? 결과에 추가
	nullptr
};

char *ArrCommandListforUser[] = {
	STRCMD_CS_COMMAND_MENT_HELP,
	STRCMD_CS_COMMAND_WHOAREYOU_HELP,
	STRCMD_CS_COMMAND_CHATPTOPFLAG_HELP,
	STRCMD_CS_COMMAND_COMMANDLIST_HELP,
	STRCMD_CS_COMMAND_CALLGM_HELP,					// 2006-05-09 by cmkwon
	STRCMD_CS_COMMAND_PLAYTIME_HELP,
	STRCMD_CS_COMMAND_SIGNBOARD_HELP,
	STRCMD_CS_COMMAND_SIGNBOARD_DEL_HELP,
	STRCMD_CS_COMMAND_INFLDITRIBUTION_HELP,			// 2006-02-08 by cmkwon
	STRCMD_CS_COMMAND_EntrancePermission_HELP,
	STRCMD_CS_COMMAND_EntrancePermissionDeny_HELP,
	STRCMD_CS_COMMAND_SUBLEADER1_HELP,			// 2007-10-06 by dhjin
	STRCMD_CS_COMMAND_SUBLEADER2_HELP,			// 2007-10-06 by dhjin

	// 2006-08-24 by cmkwon, 클라이언트에서만 사용하는 명령어 리스트
	STRCMD_C_COMMAND_BATTLE_HELP,
	STRCMD_C_COMMAND_SURRENDER_HELP,
	STRCMD_C_COMMAND_PARTYBATTLE_HELP,
	STRCMD_C_COMMAND_GUILDCOMBAT_HELP,
	STRCMD_C_COMMAND_GUILDSURRENDER_HELP,
	STRCMD_C_COMMAND_NAME_HELP,
	STRCMD_C_COMMAND_INITCHAT_HELP,
	STRCMD_C_COMMAND_REFUSEBATTLE_HELP,
	STRCMD_C_COMMAND_REFUSETRADE_HELP,

	// 2016-01-04 Future, leader warp restriction commands
#ifdef SC_LEADER_WARP_RESTRICTION
	STRCMD_CS_COMMAND_LEADER_FORBID_WARP_HELP,
	STRCMD_CS_COMMAND_LEADER_RELEASE_WARP_HELP,
#endif // SC_LEADER_WARP_RESTRICTION

	nullptr
};

#pragma endregion

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

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

bool CGlobalGameServer::InitGlobal(const char* servername, const char* logfilename, HINSTANCE hInstance, UINT IDI_Icon, int nCmdShow)
{
	if (!S_DISABLE_ALONE_RUN_MODE_HSKIM)
	{
		if (m_hMutexMonoInstance) return false;
		m_hMutexMonoInstance = CreateMutex(NULL, TRUE, servername);

		if (!m_hMutexMonoInstance)
		{
			MessageBox(NULL, "CreateMutex Error", servername, MB_OK);
			return false;
		}

		if (::GetLastError() == ERROR_ALREADY_EXISTS)
		{
			MessageBox(NULL, "ERROR : \nApplication is running already...", servername, MB_OK);
			return false;
		}
	}
	GetRootPath(CONFIG_ROOT, "./config_root.cfg");
	GetRootPath(CLIENT_ROOT, "./client_root.cfg");


	ms_SystemLogManager.InitLogManger(TRUE, logfilename, (CONFIG_ROOT + "../log/SystemLog/").c_str());

	server::log(" [START] %s\r\n", servername);

	if (!CIOCP::SocketInit())
	{
		server::log(" [ERROR] CGlobalGameServer::InitGlobal CIOCP::SocketInit error!!\r\n");
		return false;
	}

	VMemAlloc();

	if (!VMemPool::vmPoolInit())
	{
		server::log(" [ERROR] CGlobalGameServer::InitGlobal VMemPool::vmPoolInit error!!\r\n");
		return false;
	}

	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = (WNDPROC)WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_Icon));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = _T(servername);
	wcex.hIconSm = NULL;

	::RegisterClassEx(&wcex);

	g_pGlobal->SetInstanceHandle(hInstance); // Store instance handle in our global variable

	auto hWnd = CreateWindow(_T(servername), _T(servername), WS_OVERLAPPEDWINDOW, 350, 150, 400, 120, NULL, NULL, hInstance, NULL);		// 2013-06-12 by hskim, 하나의 OS 에서 서버 동시 실행

	if (!hWnd) return FALSE;

	g_pGlobal->SetMainWndHandle(hWnd);
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;


	// return InitInstance(hInstance, nCmdShow, servername, servername);
}

bool CGlobalGameServer::CreateDuplicateRun(char * i_szServerName)
{
	SetWindowText(nullptr, i_szServerName);

	if (S_DISABLE_ALONE_RUN_MODE_HSKIM)
	{
		if (m_hMutexMonoInstance) return false;

		m_hMutexMonoInstance = CreateMutex(nullptr, true, i_szServerName);

		if (!m_hMutexMonoInstance) return false;

		if (::GetLastError() == ERROR_ALREADY_EXISTS) return false;
	}

	return true;
}

bool CGlobalGameServer::CheckWriteLog(char* szLogMsg)
{
	// that means that /getserverinfo is not logged ??

	if (strlen(szLogMsg) == strlen(STRCMD_CS_COMMAND_SERVERINFO)
		
		&& _stricmp(szLogMsg, STRCMD_CS_COMMAND_SERVERINFO) == 0)
	
		return false;

	return true;
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

CGlobalGameServer::~CGlobalGameServer()
{
	EndServerSocket();

	g_pGlobal = nullptr;

	// added from CGlobal
	if (m_hMutexMonoInstance)
	{
		::CloseHandle(m_hMutexMonoInstance);
		m_hMutexMonoInstance = nullptr;
	}
}

bool CGlobalGameServer::EndServerSocket()
{
	server::log("CGlobalGameServer::EndServerSocket_1\r\n");

	if (m_hMainWndHandle)
	
		if (m_nTimerIDTraffic)
		{
			KillTimer(m_hMainWndHandle, TIMERID_TRAFFIC);
			m_nTimerIDTraffic = 0;
		}
		if (m_nTimerIDAliveCheck)
		{
			KillTimer(m_hMainWndHandle, TIMERID_ALIVE_CHECK);
			m_nTimerIDAliveCheck = 0;
		}
		if (m_nTimerIDReconnect)
		{
			KillTimer(m_hMainWndHandle, TIMERID_RECONNECT);
			m_nTimerIDReconnect = 0;
		}
	

	if (m_pGIOCP && m_pGIOCP->GetListeningFlag())
	
		m_pGIOCP->IOCPClean();

	util::del(m_pGIOCP);

	return false;
}



bool CGlobalGameServer::StartTimerTraffic()
{
	if (m_hMainWndHandle == NULL || m_nTimerIDTraffic) return FALSE;

	m_nTimerIDTraffic = ::SetTimer(m_hMainWndHandle, TIMERID_TRAFFIC, TIMERGAP_TRAFFIC, NULL);

	return TRUE;
}

void CGlobalGameServer::EndTimerTraffic()
{
	if (m_nTimerIDTraffic && m_hMainWndHandle) ::KillTimer(m_hMainWndHandle, TIMERID_TRAFFIC);

	m_nTimerIDTraffic = 0;
}

bool CGlobalGameServer::StartTimerAliveCheck()
{
	if (m_hMainWndHandle == NULL || m_nTimerIDAliveCheck) return false;

	m_nTimerIDAliveCheck = ::SetTimer(m_hMainWndHandle, TIMERID_ALIVE_CHECK, TIMERGAP_ALIVE_CHECK, NULL);

	return true;
}

void CGlobalGameServer::EndTimerAliveCheck()
{
	if (m_nTimerIDAliveCheck && m_hMainWndHandle) ::KillTimer(m_hMainWndHandle, TIMERID_ALIVE_CHECK);

	m_nTimerIDAliveCheck = 0;
}


bool CGlobalGameServer::StartTimerReconnect()
{
	if (m_hMainWndHandle == NULL || m_nTimerIDReconnect) return false;

	m_nTimerIDReconnect = ::SetTimer(m_hMainWndHandle, TIMERID_RECONNECT, TIMERGAP_RECONNECT, NULL);

	return true;
}

void CGlobalGameServer::EndTimerReconnect()
{
	if (m_nTimerIDReconnect && m_hMainWndHandle) ::KillTimer(m_hMainWndHandle, TIMERID_RECONNECT);

	m_nTimerIDReconnect = 0;
}

const char* GetProtocolTypeString(MessageType_t msgType);

void CGlobalGameServer::CheckServerThread()
{
	if (m_pGIOCP == nullptr) return;
	
	SThreadInfo *pSTInfo = m_pGIOCP->CheckIOCPThread(0);
	
	while (pSTInfo)
	{
		pSTInfo->bPrintOut = true;

		switch (pSTInfo->enThreadCheckType)
		{

		case THREAD_CHECK_TYPE_DB_WORKER:
			server::log(" [ERROR] deadlock : ThreadType(%10s), ThreadId(%5d), TimeGap(%5d) StartTick(%10d) DBThreadIndex(%4d) QueryType[%20s(%#04x)] QueryArraySize(%4d) CharacterUID(%6d), Param1(%2d) Param2(%d)\r\n"
				, pSTInfo->GetThreadCheckComment(pSTInfo->enThreadCheckType), pSTInfo->dwThreadId
				, timeGetTime() - pSTInfo->dwLastUseStartTick
				, pSTInfo->dwLastUseStartTick
				, pSTInfo->dwSocketIndex
				, GetDBQueryTypeString((EnumQueryType)pSTInfo->dwMessageType)
				, pSTInfo->dwMessageType
				, pSTInfo->nQueryArraySize
				, pSTInfo->dwCharacterUID
				, pSTInfo->nParam1, pSTInfo->nParam2);		// 2007-02-21 by cmkwon
		
			break;

		default:
			server::log(" [ERROR] deadlock : ThreadType(%10s), ThreadId(%5d), TimeGap(%5d) StartTick(%10d) SocketIndex(%4d) MessageType[%20s(%#04x)] Parma1(%2d) Param2(%d)\r\n"
				, pSTInfo->GetThreadCheckComment(pSTInfo->enThreadCheckType), pSTInfo->dwThreadId
				, timeGetTime() - pSTInfo->dwLastUseStartTick
				, pSTInfo->dwLastUseStartTick
				, pSTInfo->dwSocketIndex
				, GetProtocolTypeString(pSTInfo->dwMessageType)
				, pSTInfo->dwMessageType
				, pSTInfo->nParam1, pSTInfo->nParam2);		// 2007-02-21 by cmkwon
		
			break;
		}

		pSTInfo = m_pGIOCP->CheckIOCPThread(pSTInfo->dwThreadId);
	}
}

void CGlobalGameServer::CalculateIOCPTraffic()
{
	// Bandwidth 계산
	if (m_pGIOCP && m_pGIOCP->GetListeningFlag()) m_pGIOCP->CalcTotalTrafficInfo();					
}

void CGlobalGameServer::CheckClientAlive() const
{
	if (m_pGIOCP && m_pGIOCP->GetListeningFlag()) m_pGIOCP->ClientCheck();
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			USHORT CGlobalGameServer::AuthAdminToolUser(const char *i_pUID, const char *i_pPWD)
/// \brief		
///				// 2006-04-15 by cmkwon, 리턴값의 자료형 변경 - BOOL ==> USHORT
/// \author		cmkwon
/// \date		2006-04-15 ~ 2006-04-15
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
USHORT CGlobalGameServer::AuthAdminToolUser(const char *i_pUID, const char *i_pPWD)
{
	CODBCStatement ODBCStmt;
	// 2008-07-11 by cmkwon, MySQL 지원을 위해 수정(IP,Port 로 DB 서버 연결) - 아래와 같이 수정 함
	//ODBCStmt.Init((char*)GetODBCDSN(), (char*)GetODBCUID(), (char*)GetODBCPASSWORD());
	BOOL bRet = ODBCStmt.Init(this->GetDBServerIP(), this->GetDBServerPort(), this->GetDBServerDatabaseName(), (char*)this->GetODBCUID(), (char*)this->GetODBCPASSWORD(), this->GetMainWndHandle());
	if (FALSE == bRet)
	{
		// 2008-09-05 by cmkwon, DBServer 연결 실패 시스템 로그 남기기 - 
		g_pGlobal->WriteSystemLogEX(TRUE, "[DB Error] fail to connect DBServer(CGlobalGameServer::AuthAdminToolUser_) !! %s,%d %s %s %s\r\n"
			, g_pGlobal->GetDBServerIP(), g_pGlobal->GetDBServerPort(), g_pGlobal->GetDBServerDatabaseName(), (CHAR*)g_pGlobal->GetODBCUID(), (CHAR*)g_pGlobal->GetODBCPASSWORD());
		return 0;
	}

	char szPWDToEncode[SIZE_MAX_PASSWORD];
	util::strncpy(szPWDToEncode, i_pPWD, SIZE_MAX_PASSWORD);

	MD5 MD5_instance;
	unsigned char md5_string[16];
	char szEncodedString[33];
	USHORT	usRetRace = 0;			// 2006-04-15 by cmkwon

	MD5_instance.MD5Encode((char*)szPWDToEncode, md5_string);
	MD5_instance.MD5Binary2String(md5_string, szEncodedString);
	
	SQLHSTMT hstmt = ODBCStmt.GetSTMTHandle();
	SQLINTEGER arrCB2[4] = { SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS };
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_ACCOUNT_NAME, 0, (LPSTR)i_pUID, 0, &arrCB2[1]);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_PASSWORD_MD5_STRING, 0, (LPSTR)i_pPWD, 0, &arrCB2[2]);
	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_PASSWORD_MD5_STRING, 0, (LPSTR)szEncodedString, 0, &arrCB2[3]);
	bRet = ODBCStmt.ExecuteQuery(PROCEDURE_080827_0074);
	
	if (!bRet)
	{
		ODBCStmt.FreeStatement();
		ODBCStmt.Clean();
		return usRetRace;
	}

	SQLINTEGER arrCB = SQL_NTS;
	SQLBindCol(ODBCStmt.m_hstmt, 1, SQL_C_SHORT, &usRetRace, 0, &arrCB);
	while ((bRet = SQLFetch(ODBCStmt.m_hstmt)) != SQL_NO_DATA)
	{
		/*if(FALSE == COMPARE_RACE(usRetRace, RACE_OPERATION|RACE_GAMEMASTER))*/
		if (FALSE == COMPARE_RACE(usRetRace, RACE_OPERATION | RACE_GAMEMASTER | RACE_MONITOR))
		{// 2006-04-15 by cmkwon, 관리자/GM이 아니면 0으로 초기화 한다.
			usRetRace = 0;
		}
	}
	ODBCStmt.FreeStatement();
	ODBCStmt.Clean();

	return usRetRace;
}

bool CGlobalGameServer::LoadStringList(setString *o_psetStirng, const char *i_szFilePath)
{
	const int SIZE_BUFF = 512;
	char buff[SIZE_BUFF];

	ifstream fin;
	fin.open(i_szFilePath);

	if (!fin.is_open()) return false;

	while (true)
	{
		memset(buff, 0x00, SIZE_BUFF);

		bool bFlagFileEnd = fin.getline(buff, SIZE_BUFF).eof();

		if (bFlagFileEnd && strcmp(buff, "") == 0) break;

		// ignore blank lines
		// ignore comments
		// ignore user defined variables
		if (strcmp(buff, "") == 0 || strncmp(buff, "#", 1) == 0 || strncmp(buff, "$", 1) == 0) continue;

		_strlwr(buff);
		o_psetStirng->insert(buff);
	}
	fin.close();

	return true;
}

bool CGlobalGameServer::LoadAbuseAndProhibitedName()
{

	LoadStringList(&m_setAbuseWords, (CONFIG_ROOT + string("./AFList.txt")).c_str());

	LoadStringList(&m_setProhibitedNames, (CONFIG_ROOT + string("./prohibited_names.cfg")).c_str());

	return true;
}

bool CGlobalGameServer::CheckProhibitedName(const char *i_szName)
{
	char buff[SIZE_MAX_CHARACTER_NAME] { };

	strncpy(buff, i_szName, SIZE_MAX_CHARACTER_NAME);

	_strlwr(buff);

	if (m_setProhibitedNames.find(buff) != m_setProhibitedNames.end()) return false;

	for (auto& pAbuseWord : m_setAbuseWords)
	{

		if (pAbuseWord.length() > 0 && strstr(i_szName, pAbuseWord.c_str()) != nullptr)

			return false;
	}

	return true;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			void CGlobalGameServer::AddAllowedToolIP(char *i_szAllowedToolIP)
/// \brief		// 2008-06-05 by cmkwon, AdminTool, Monitor 접근 가능 IP를 server config file 에 설정하기 - 
/// \author		cmkwon
/// \date		2008-06-05
///////////////////////////////////////////////////////////////////////////////
void CGlobalGameServer::AddAllowedToolIP(const char *i_szAllowedToolIP)
{
	if (i_szAllowedToolIP && *i_szAllowedToolIP != '\0')
		
		m_AllowedToolIPList.emplace_back(i_szAllowedToolIP);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CGlobalGameServer::CheckAllowedToolIP(char *i_szToolIP)
/// \brief		// 2008-06-05 by cmkwon, AdminTool, Monitor 접근 가능 IP를 server config file 에 설정하기 - 
/// \author		cmkwon
/// \date		2008-06-05
///////////////////////////////////////////////////////////////////////////////
BOOL CGlobalGameServer::CheckAllowedToolIP(const char *i_szToolIP)
{
	// true for 127.0.0.1 only
	if (IS_SCADMINTOOL_CONNECTABLE_IP(i_szToolIP)) return true;

	for (auto& allowed : m_AllowedToolIPList) if (allowed == i_szToolIP) return true;
	
	server::log(" [Notify] CheckAllowedToolIP ==> Not Allowed (%s) !!\r\n", i_szToolIP);
	
	return false;
}
