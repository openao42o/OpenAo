#pragma once

#include "AtumParam.h"
#include "SystemLogManager.h"

//////////////////////////////////////////////////////////////////////////
// Configuration File Path

#ifdef ARENA
#define GLOBAL_CONFIG_FILE_PATH					(CONFIG_ROOT+string("./ArenaGlobal.cfg")).c_str()
#else
#define GLOBAL_CONFIG_FILE_PATH					(CONFIG_ROOT+string("./global.cfg")).c_str()
#endif

#define DGA_CONFIG_FILE_PATH					(CONFIG_ROOT+string("./dga.cfg")).c_str()
#define IM_SERVER_CONFIG_FILE_PATH				(CONFIG_ROOT+string("./im.cfg")).c_str()
#define FIELD_SERVER_CONFIG_FILE_PATH			(CONFIG_ROOT+string("./field.cfg")).c_str()
#define NPC_SERVER_CONFIG_FILE_PATH				(CONFIG_ROOT+string("./npc.cfg")).c_str()
#define HAPPYHOUR_EVENT_CONFIG_FILE_PATH		(CONFIG_ROOT+string("./happyHourEvent.cfg")).c_str()
#define LOCALIZATION_CONFIG_DIRECTORY_PATH		(CONFIG_ROOT+string("./localization")).c_str()
#define SECURITY_HACKSHIELD_DIRECTORY_PATH		(CONFIG_ROOT+string("./Security/HackShield")).c_str()		// 2008-04-03 by cmkwon, 핵쉴드 서버 연동 시스템 수정 - 디렉토리만 설정함

#define	RESOBJ_DIRECTORY_PATH					(CONFIG_ROOT+string("./../map/Res-Obj")).c_str()							// 2007-05-28 by cmkwon
#define	RESTEX_DIRECTORY_PATH					(CONFIG_ROOT+string("./../map/Res-Tex")).c_str()						// 2007-07-18 by cmkwon, omi.tex도 체크섬을 체크 루틴 추가
#define	RESEXE_DIRECTORY_PATH					(CONFIG_ROOT+string("./../map/Res-EXE")).c_str()						// 2008-09-17 by cmkwon, 클라이언트 실행파일도 체크섬 체크 추가 - 


#define CONFIG_NAME_DB_SERVER_IP						"DBServerIP"
#define CONFIG_NAME_DB_SERVER_PORT						"DBServerPort"
#define CONFIG_NAME_DB_SERVER_DATABASE_NAEE				"DBServerDatabaseName"
#define CONFIG_NAME_LOGDB_DB_SERVER_IP					"LOGDB_DBServerIP"
#define CONFIG_NAME_LOGDB_DB_SERVER_PORT				"LOGDB_DBServerPort"
#define CONFIG_NAME_LOGDB_DB_SERVER_DATABASE_NAEE		"LOGDB_DBServerDatabaseName"

//////////////////////////////////////////////////////////////////////////
// Timer Variables
enum : UINT
{
	TIMERID_TRAFFIC = 100,
	TIMERID_ALIVE_CHECK = 101,
	TIMERID_RECONNECT = 102,
	TIMERID_SERVER_GROUP_INFO = 103,

	TIMERGAP_RECONNECT = 5000,			// 5 sec
	TIMERGAP_TRAFFIC = 10000,			// 10 sec
	TIMERGAP_SERVER_GROUP_INFO = 30000,	// 30 sec
	TIMERGAP_ALIVE_CHECK = 60000,		// 1 min
	TIMERGAP_TRIGGER_SYSTEM = 60000,	// 1 min
	TIMERGAP_LOGSYSTEM = 300000,		// 5 min
	TIMERGAP_GLOGSYSTEM = 550000,		// 9 min and 50 sec	
};

// 2008-03-03 by cmkwon, SIZE_MAX_ODBC_CONN_STRING 정의 위치 변경 - AtumParam.h 로 위치 이동 함 
//#define SIZE_MAX_ODBC_CONN_STRING				128

class CGlobalGameServer;

// Global Variable
extern CGlobalGameServer *	g_pGlobal;		// 전역변수관리를 위한 전역클래스포인터
extern char	*				ArrCommandList[];			// Command List
extern char	*				ArrCommandListforUser[];	// 일반 유저를 위한 Command List
extern char	*				ArrGameMasterCommandList[];	// 2007-11-08 by cmkwon, GM 명령어 정리 - 운영자 Command List


//////////////////////////////////////////////////////////////////////////
// Global Function
//////////////////////////////////////////////////////////////////////////
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);	// 윈도우 메세지 프로시저
//int GDeleteNewLine(char *str);

struct SServerGroupForMame
{
	char		szServerGroupName[SIZE_MAX_SERVER_NAME];
	int			nServerIDforMGame;
};
typedef vector<SServerGroupForMame>			vectorServerGroupForMGame;

///////////////////////////////////////////////////////////////////////////////
/// \class		CGlobalGameServer
///
/// \brief		전역변수를 클래스로 만들어 처리를 위해
/// \author		cmkwon
/// \version	
/// \date		2004-03-18 ~ 2004-03-18
/// \warning	객체는 하나만 만들어야 한다.
///////////////////////////////////////////////////////////////////////////////
class CIOCP;
class CGlobalGameServer // : public CGlobal
{
	// copied from global
#pragma region CGlobal

public:

	bool InitGlobal(const char* servername, const char* logfilename, HINSTANCE hInstance, UINT IDI_Icon, int nCmdShow);
	bool CreateDuplicateRun(char * i_szServerName);

	static bool CheckWriteLog(char *szLogMsg);
	static bool WriteSystemLog(char *szLogMsg, bool bTimeHeaderFlag = true);
	static bool WriteSystemLogEX(bool bPrintDBGOUT, const char* pFormat, ...);
	static bool WriteSystemLineEX(bool bPrintDBGOUT, const char* pFormat, ...);

protected:
	virtual void VMemAlloc() = 0;

	HANDLE m_hMutexMonoInstance;

	static CSystemLogManager ms_SystemLogManager;

#pragma endregion


public:
	CGlobalGameServer();
	virtual ~CGlobalGameServer();

	///////////////////////////////////////////////////////////////////////////
	// Public Properties
	void SetInstanceHandle(HINSTANCE i_hInstance) { m_hInstanceHandle = i_hInstance; }
	HINSTANCE GetInstanceHandle() const { return m_hInstanceHandle; }
	void SetMainWndHandle(HWND i_hWnd) { m_hMainWndHandle = i_hWnd; }
	HWND GetMainWndHandle() const { return m_hMainWndHandle; }
	UINT GetTimerIDTraffic() const { return m_nTimerIDTraffic; }
	UINT GetTimerIDAliveCheck() const { return m_nTimerIDAliveCheck; }
	UINT GetTimerIDReconnect() const { return m_nTimerIDReconnect; }

	void SetIPLocal(const char *i_szIP) { strncpy(m_szIPLocal, i_szIP, SIZE_MAX_IPADDRESS); }
	const char *GetIPLocal() const { return m_szIPLocal; }
	void SetPublicIPLocal(const char *i_szIP) { strncpy(m_szPublicIPLocal, i_szIP, SIZE_MAX_IPADDRESS); }
	const char *GetPublicIPLocal() const { return m_szPublicIPLocal; }
	void SetIPPreServer(const char *i_szIP) { strncpy(m_szIPPreServer, i_szIP, SIZE_MAX_IPADDRESS); }
	const char *GetIPPreServer() const { return m_szIPPreServer; }
	void SetPortListening(int i_nPort) { m_nPortListening = i_nPort; }
	int GetPortListening() const { return m_nPortListening; }
	void SetPortPreServer(int i_nPort) { m_nPortPreServer = i_nPort; }
	int GetPortPreServer() const { return m_nPortPreServer; }

	CIOCP* GetGIOCP() const { return m_pGIOCP; }

	void SetServerGroupName(const char *i_szServerGroupName) { strncpy(m_szServerGroupName, i_szServerGroupName, SIZE_MAX_SERVER_NAME); }
	const char* GetServerGroupName() const { return m_szServerGroupName; }

	bool IsTestServer() const { return m_bIsTestServer; }

	//////////////////////////////////////////////////////////////////////////
	// Method
	bool StartTimerTraffic();
	void EndTimerTraffic();
	bool StartTimerAliveCheck();
	void EndTimerAliveCheck();
	bool StartTimerReconnect();
	void EndTimerReconnect();

	void CheckServerThread();
	void CalculateIOCPTraffic();
	void CheckClientAlive() const;


	const char* GetODBCUID() const { return m_szODBCUID; }
	const char* GetODBCPASSWORD() const { return m_szODBCPASSWORD; }

	const char* GetLogDBODBCUID() const { return m_szLogDBODBCUID; }
	const char* GetLogDBODBCPASSWORD() const { return m_szLogDBODBCPASSWORD; }

	USHORT AuthAdminToolUser(const char *i_pUID, const char *i_pPWD);

	bool LoadStringList(setString *o_psetStirng, const char *i_szFilePath);

	// 욕설 및 금지 아이디
	bool LoadAbuseAndProhibitedName();
	bool CheckProhibitedName(const char *i_szName);

	///////////////////////////////////////////////////////////////////////////////
	// 2008-07-11 by cmkwon, MySQL 지원을 위해 수정(IP,Port 로 DB 서버 연결) - 
	const char *GetDBServerIP() const { return m_szDBServerIP; }
	int GetDBServerPort() const { return m_nDBServerPort; }
	const char *GetDBServerDatabaseName() const { return m_szDBServerDatabaseName; }
	const char *GetLogDBDBServerIP() const { return m_szLogDBDBServerIP; }
	int GetLogDBDBServerPort() const { return m_nLogDBDBServerPort; }
	const char *GetLogDBDBServerDatabaseName() const { return m_szLogDBDBServerDatabaseName; }

	///////////////////////////////////////////////////////////////////////////////
	// 2008-06-05 by cmkwon, AdminTool, Monitor 접근 가능 IP를 server config file 에 설정하기 - 
	vector<string>			m_AllowedToolIPList;
	void AddAllowedToolIP(const char *i_szAllowedToolIP);
	BOOL CheckAllowedToolIP(const char *i_szToolIP);
public:

	virtual bool InitServerSocket() = 0;						// 소켓관련 초기화 함수
	virtual bool EndServerSocket();							// 소켓관련 종료 함수

protected:
	//////////////////////////////////////////////////////////////////////////
	// 윈도우 관련 멤버 변수
	HINSTANCE			m_hInstanceHandle;				// 실행된 인스턴스 핸들
	HWND				m_hMainWndHandle;				// 실행된 메인 윈도우 핸들

	//////////////////////////////////////////////////////////////////////////
	// 윈도우 타이버 관련 멤버 변수
	UINT				m_nTimerIDTraffic;				// 모든 서버에서 Bandwidth 계산을 위한 TimerID
	UINT				m_nTimerIDAliveCheck;			// 모든 서버에서 클라이언트의 Alive 체크를 위한 TimerID
	UINT				m_nTimerIDReconnect;			// 서버(IMServer, FieldServer, NPCServer)에서 상위 서버와 접속이 종료 되었을때 재접속을 위한 TimerID

	//////////////////////////////////////////////////////////////////////////
	// Configuration Variables
	char				m_szIPLocal[SIZE_MAX_IPADDRESS];				// All (Pre, IM, Field, NPC)
	char				m_szPublicIPLocal[SIZE_MAX_IPADDRESS];			// All (Pre, IM, Field, NPC)// 2006-05-23 by cmkwon
	char				m_szIPPreServer[SIZE_MAX_IPADDRESS];			// two (IM, Field)
	int					m_nPortListening;								// one (Field)
	int					m_nPortPreServer;								// two (IM, Field)

	CSystemLogManager	m_FieldGameLogManager;

	char				m_szODBCDSN[SIZE_MAX_ODBC_CONN_STRING];			// ODBC_DSN
	char				m_szODBCUID[SIZE_MAX_ODBC_CONN_STRING];			// ODBC_UID
	char				m_szODBCPASSWORD[SIZE_MAX_ODBC_CONN_STRING];		// ODBC_PASSWD

	char				m_szLogDBODBCDSN[SIZE_MAX_ODBC_CONN_STRING];			// LogDB_ODBC_DSN
	char				m_szLogDBODBCUID[SIZE_MAX_ODBC_CONN_STRING];			// LogDB_ODBC_UID
	char				m_szLogDBODBCPASSWORD[SIZE_MAX_ODBC_CONN_STRING];		// LogDB_ODBC_PASSWD

	char				m_szDBServerIP[SIZE_MAX_ODBC_CONN_STRING];
	int					m_nDBServerPort;
	char				m_szDBServerDatabaseName[SIZE_MAX_ODBC_CONN_STRING];

	char				m_szLogDBDBServerIP[SIZE_MAX_ODBC_CONN_STRING];
	int					m_nLogDBDBServerPort;
	char				m_szLogDBDBServerDatabaseName[SIZE_MAX_ODBC_CONN_STRING];


	char				m_szServerGroupName[SIZE_MAX_SERVER_NAME];		// one (IM)

	setString			m_setAbuseWords;
	setString			m_setProhibitedNames;

	BOOL				m_bIsTestServer;								// TestServer 여부

	CIOCP *				m_pGIOCP;
};

typedef mt_set<UID32_t>		mtsetUID32;

namespace server
{
	template<typename... args_t>
	void log(bool dbgout, const char* str, args_t... args) { g_pGlobal->WriteSystemLogEX(dbgout, str, args...); }

	template<typename... args_t>
	void log(const char* str, args_t... args) { g_pGlobal->WriteSystemLogEX(false, str, args...); }

	template<typename... args_t>
	void writeline(bool dbgout, const char* str, args_t... args) { g_pGlobal->WriteSystemLineEX(dbgout, str, args...); }

	template<typename... args_t>
	void writeline(const char* str, args_t... args) { g_pGlobal->WriteSystemLineEX(false, str, args...); }
}