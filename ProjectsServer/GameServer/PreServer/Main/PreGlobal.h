#pragma once

#include "GlobalGameServer.h"
#include "AtumProtocol.h"			// 2007-05-02 by cmkwon

using vectSGAME_SERVER_GROUP = vector<SGAME_SERVER_GROUP>;		// 2007-05-02 by cmkwon

class CPreIOCP;
class CPreIOCPSocket;
class CPreGlobal;
class CServerGroup;

extern CPreGlobal* g_pPreGlobal;

class CPreGlobal : public CGlobalGameServer
{
	friend class CPreIOCP;

public:
	CPreGlobal();
	~CPreGlobal();


	void Reset();

	VersionInfo GetClientReinstallVersion() const { return m_ClientReinstallVersion; }

	bool GetRandomFieldSelect() const { return m_bRandomFieldSelect; }

	const char* GetDownloadServerIP() const { return GetRandomDownloadHttpIP(); }
	int GetDownloadServerPort() const { return m_nDownloadHttpServerPort; }

	const char*	GetClientUpdateDownloadDir() const { return m_szClientHttpUpdateDownloadDir; }
	const char*	GetLauncherFileDownloadPath() const { return m_szLauncherFileDownloadHttpPath; }
	const char*	GetDeleteFileListDownloadPath() const { return m_szDeleteFileListDownloadHttpPath; }
	const char*	GetNoticeFileDownloadPath() const { return m_szNoticeFileDownloadHttpPath; }

	void AddDownloadHttpIP(const char * i_szIP);
	const char* GetRandomDownloadHttpIP() const;



	BOOL CheckAllowedList(CServerGroup *i_pGroupInfo, char * i_szAccountName, char * i_szPrivateIP, USHORT i_nAccountType);
	void PrintAllowedList() const;
	BOOL CheckAllowedIP(const char *i_szPublicIP) const;		// 2007-10-19 by cmkwon, AllowedIP 시스템 변경 - 허가 IP 체크 함수 

	CServerGroup* GetServerGroupForLoading(const char * szServerGruopName);
	int GetEnableServerGroupCount();	// 2008-02-22 by cmkwon, ServerPreServer->MasangPreServer 로 서비스 정보 전송 시스템 추가 - 

	// 2007-05-02 by cmkwon
	// 2007-09-05 by cmkwon, EXE_1에 로그인 서버 선택 인터페이스 수정 - 인자추가(, USHORT i_usTab8OrderIndex), 인자 제거(, USHORT i_usPreServerPort, 포트는 기본 포트 사용)
	bool InsertGameServerGroup(char *i_szGameServerGroupName, char *i_szPreServerIP, int i_nTab8OrderIndex);
	bool Make_MSG_PC_CONNECT_GET_GAME_SERVER_GROUP_LIST(MSG_PC_CONNECT_GET_GAME_SERVER_GROUP_LIST_OK *o_pGameServerGroupList);
	bool Make_MSG_PC_CONNECT_GET_NEW_GAME_SERVER_GROUP_LIST_OK(MSG_PC_CONNECT_GET_NEW_GAME_SERVER_GROUP_LIST_OK *o_pGameServerGroupList);	// 2007-09-05 by cmkwon, EXE_1에 로그인 서버 선택 인터페이스 수정 - 신버전 함수 추가

	// 2007-07-06 by cmkwon, OnlyChoiceServer 플래그 구현 추가
	bool GetOnlyChoiceServer() const { return m_bIsOnlyChoiceServer; }
	const char* GetGamePublisherName() const { return m_szGamePublisherName; }		// 2010-06-01 by shcho, GLogDB 관련 -

	// virtual Function
	bool InitServerSocket() override;				// 소켓관련 초기화 함수
	bool EndServerSocket() override;					// 소켓관련 종료 함수

	void VMemAlloc() override;

	bool LoadConfiguration();				// Server 설정을 위한 Configuration Load

protected:
	VersionInfo				m_ClientReinstallVersion;		// re-install version
	vector<VersionInfo>		m_vectorCriticalUpdateVersion;	// critical update version list
	vector<string>			m_AllowedIPList;
	set<string>				m_AllowedAccounts;
	BOOL					m_bRandomFieldSelect;			// Pre Server에서 최초로 접속해야 할 Field Server의 IP를 받을 때 Random으로 받을지에 대한 여부
	vector<CServerGroup>	m_AllServerGroupVectorForLoading;	// 다 로딩한 후에는 바로 삭제함

	///////////////////////////////////////////////////////////////////////////////	
	// 2007-05-02 by cmkwon, PreServer 여러개 등록
	vectSGAME_SERVER_GROUP	m_vectGameServerGroupList;

	// 2007-07-06 by cmkwon, OnlyChoiceServer 플래그 구현 추가
	BOOL					m_bIsOnlyChoiceServer;				// 2007-07-06 by cmkwon, OnlyChoiceServer 플래그 구현 추가

	char					m_szGamePublisherName[SIZE_MAX_GAME_PUBLISHER_NAME];		// 2010-06-01 by shcho, GLogDB 관련 -

public:
	BOOL					m_bIgnoreClientVersionUpdate;
	int						m_nServerGroupLimiteUserCount;		// 2006-10-11 by cmkwon

	char					m_szLauncherFileDownloadPath[SIZE_MAX_FTP_FILE_PATH];	// 
	char					m_szDeleteFileListDownloadPath[SIZE_MAX_FTP_FILE_PATH];	// DELFILELIST_FILE_NAME
	char					m_szNoticeFileDownloadPath[SIZE_MAX_FTP_FILE_PATH];		// NOTICE_FILE_NAME


	vector<string>			m_vectDownloadHttpServer;
	USHORT					m_nDownloadHttpServerPort;

	char					m_szClientHttpUpdateDownloadDir[SIZE_MAX_FTP_FILE_PATH];
	char					m_szLauncherFileDownloadHttpPath[SIZE_MAX_FTP_FILE_PATH];
	char					m_szDeleteFileListDownloadHttpPath[SIZE_MAX_FTP_FILE_PATH];
	char					m_szNoticeFileDownloadHttpPath[SIZE_MAX_FTP_FILE_PATH];
	char					m_szVersionListFileDownloadHttpPath[SIZE_MAX_FTP_FILE_PATH];
};

namespace PreGlobal
{	
	inline bool CheckAllowedIP(const char* ip) { return g_pPreGlobal->CheckAllowedToolIP(ip); }

	bool Initialize(HINSTANCE hInstance, UINT IDI_Icon, int nCmdShow);

	void Destroy();

	bool Create(HWND hWnd);

	void DestroySocket();

	void OnTimerTraffic();
	void OnTimerPingAlive();
}