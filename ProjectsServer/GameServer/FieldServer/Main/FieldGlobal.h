// FieldGlobal.h: interface for the CFieldGlobal class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FIELDGLOBAL_H__B6793946_A3E2_4778_A6DA_1EE835B77558__INCLUDED_)
#define AFX_FIELDGLOBAL_H__B6793946_A3E2_4778_A6DA_1EE835B77558__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DGAEventManager.h"

#define SIZE_FIELD2LOG_WINSOCKET		10

class CLogWinSocket;
typedef vector<CLogWinSocket*>			vectorF2LWSocketPtr;
typedef set<string>						setstring;					// 2006-08-18 by dhjin

class CFieldGlobal;
extern CFieldGlobal *			g_pFieldGlobal;
#ifdef NEMERIAN_NATION_BALANCE_BUFF_PENALITY
//TODO: init this variable in g_pFieldGlobal
#ifndef G_DIFFVICTORYWAR
extern int			g_diffVictoryWarByroo;
#define G_DIFFVICTORYWAR
#endif
#endif /* NEMERIAN_NATION_BALANCE_BUFF_PENALITY */

///////////////////////////////////////////////////////////////////////////////
/// \class		CFieldGlobal
///
/// \brief		
/// \author		cmkwon
/// \version	
/// \date		2004-03-19 ~ 2004-03-19
/// \warning	
///////////////////////////////////////////////////////////////////////////////
class CFieldIOCP;

class CPreWinSocket;
class CIMWinSocket;
class CArenaFieldWinSocket;
class CPCBangIPManager;
class CFieldGlobal : public CGlobalGameServer  
{
public:
	CFieldGlobal();
	virtual ~CFieldGlobal();

	///////////////////////////////////////////////////////////////////////////
	// Property
	void SetLastTickLogSystem(DWORD i_dwTick);
	DWORD GetLastTickLogSystem(void);
	void SetFieldServerGroupName(char *i_szFieldServerGroupName);
	char *GetFieldServerGroupName(void);
	void SetIPLogServer(char *i_szIP);
	char *GetIPLogServer(void);
	void SetIPIMServer(char *i_szIP);
	void SetIPNPCServer(char *i_szIP);		// 2013-04-01 by hskim, NPCServer 패킷으로 위조 공격 방어
	char *GetIPNPCServer(void);				// 2013-04-01 by hskim, NPCServer 패킷으로 위조 공격 방어
	char *GetIPIMServer(void);
	char *GetIPVoIP1to1Server(void);
	char *GetIPVoIPNtoNServer(void);

	void SetPortLogServer(int i_nPort);
	int GetPortLogServer(void);
	void SetPortIMServer(int i_nPort);
	int GetPortIMServer(void);
	int GetPortVoIP1to1Server(void);
	int GetPortVoIPNtoNServer(void);

	// 2013-03-13 by hskim, 웹 캐시 상점
	void SetWebInterface(BOOL bWebInterface);
	BOOL IsWebInterface(void);
	void SetWebCashShop(BOOL bWebCashShop);
	BOOL IsWebCashShop(void);
	void SetPortWebInterface(int nWebInterfacePort);
	int GetPortWebInterface(void);
	// 2013-03-13 by hskim, 웹 캐시 상점

	CPreWinSocket * GetField2PreWinSocket(void);
	CIMWinSocket * GetField2IMWinSocket(void);
	CArenaFieldWinSocket * GetField2ArenaFieldWinSocket(void);				// 2007-12-26 by dhjin, 아레나 통합 - 

	void ChecklRequestInCRTLib(void);

	char *GetMainORTestServerName(void);
	char *GetGamePublisherName(void);
	BOOL GetIsJamboreeServer(void);

	///////////////////////////////////////////////////////////////////////////
	// Method
	BOOL CreateAllF2WSocket(HWND i_hWnd);
	BOOL CreateField2PreWinSocket(HWND i_hWnd);
	BOOL CreateField2IMWinSocket(HWND i_hWnd);
	BOOL CreateField2ArenaFieldWinSocket(HWND i_hWnd);						// 2007-12-26 by dhjin, 아레나 통합 - 
	void DestroyAllF2LWSocket(void);
	void DestroyField2PreWinSocket(void);
	void DestroyField2IMWinSocket(void);
	void DestroyField2ArenaFieldWinSocket(void);									// 2007-12-26 by dhjin, 아레나 통합 - 
	BOOL SendLogFieldServer2LogServer(BYTE *szLogMsg, int nLength);
	BOOL SendLogFieldServer2LogServerORGFile(BYTE *szLogMsg, int nLength);
	BOOL ConnectAllF2LWSocket(char *i_szSvrIP, int i_nSvrPort);
	BOOL ReConnectAllF2LWSocket(char *i_szSvrIP, int i_nSvrPort);
	BOOL IsConnectedAllF2LWSocket(void);
	void OnF2LAsyncEvent(SOCKET i_hSocket, LONG i_nEvent);

	// 2015-09-14 Future, disabled Monster Maps during Influence war
/*	void AddInfluenceWarDisabledMonsterMap(MapIndex_t mapIndex);
	const vector<MapIndex_t>& GetInfluenceWarDisabledMonsterMaps();*/

	// 2009-04-20 by cmkwon, F2L 관련 시스템 수정 - 
	BOOL OnF2LClosed(CLogWinSocket *i_pF2LWinSoc);

	void SendArenaServerMFSInfo();						// 2008-01-17 by dhjin, 아레나 통합 - 아레나 서버로 필드 서버 정보 전송
	// 2008-01-31 by cmkwon, 계정 블럭/해제 명령어로 가능한 시스템 구현 - 
	BOOL Send2PreServer(BYTE *i_pData, int i_nDataLen);
	
	// 2008-09-08 by cmkwon, SCMonitor에서 ReloadVersionInfo시에 일부 체크섬파일(.\Res-Tex\*.*)도 리로드하기 - 
	BOOL LoadResObjCheckListW(BOOL i_bReloadOnlyRexTexDirectory=FALSE);

	// 2009-03-05 by cmkwon, 선전포고 아레나 서버 처리 - 
	BOOL IsArenaServer(void);

	// virtual Function
	bool InitServerSocket() override;				// 소켓관련 초기화 함수
	bool EndServerSocket() override;					// 소켓관련 종료 함수

	void VMemAlloc() override;

	virtual BOOL LoadDGAConfiguration(void);
	virtual BOOL LoadConfiguration(void);				// Server 설정을 위한 Configuration Load

	// start 2011-11-03 by shcho, yedang 셧다운제 구현 - 체크 함수 추가
	// 나이 체크 함수
	BOOL checkSHUTDOWNMINORS(int birthdayYears /* 생일 */ , ATUM_DATE_TIME currentTime/* 현재 시간 */);	
	// END 2011-11-03 by shcho, yedang 셧다운제 구현 - 체크 함수 추가

	BOOL CheckAllowedNPCServerIP(const char *i_szIP);		// 2013-04-01 by hskim, NPCServer 패킷으로 위조 공격 방어

	// 2015-11-25 Future, Multiple IP Restriction System
#ifdef S_IP_UNIQUE_CONNECTION
	BOOL GetMultipleIPRestrictionFlag();
#endif // S_IP_UNIQUE_CONNECTION

protected:
	//////////////////////////////////////////////////////////////////////////
	// 윈도우 타이버 관련 멤버 변수
	DWORD				m_dwLastTickLogSystem;			// FieldServer에서 현재 접속 정보를 LogServer로 전송하기 위한 TimerID

	//////////////////////////////////////////////////////////////////////////
	// Configuration Variables
	char				m_szFieldServerGroupName[SIZE_MAX_SERVER_NAME];	// one (Field)
	char				m_szIPLogServer[SIZE_MAX_IPADDRESS];			// one (Field)
	char				m_szIPNPCServer[SIZE_MAX_IPADDRESS];			// one (Field) - // 2013-04-01 by hskim, NPCServer 패킷으로 위조 공격 방어
	char				m_szIPIMServer[SIZE_MAX_IPADDRESS];				// one (Field)
	char				m_szIPVoIP1to1Server[SIZE_MAX_IPADDRESS];		// one (Field)
	char				m_szIPVoIPNtoNServer[SIZE_MAX_IPADDRESS];		// one (Field)
	int					m_nPortLogServer;								// one (Field)
	int					m_nPortIMServer;								// one (Field)
	int					m_nPortVoIP1to1Server;							// one (Field)
	int					m_nPortVoIPNtoNServer;							// one (Field)

	vectorF2LWSocketPtr	m_vectorF2LWSocketPtr;
	int					m_nSendIndexF2LWSocket;
	CPreWinSocket *		m_pField2PreWinSocket;
	CIMWinSocket *		m_pField2IMWinSocket;
	CArenaFieldWinSocket*	m_pField2ArenaFieldWinSocket;					// 2007-12-26 by dhjin, 아레나 통합 - 

	// 2006-05-30 by cmkwon
	long				*m_plRequestInCRTLib;			

	char				m_szMainORTestServerName[SIZE_MAX_SERVER_NAME];			// 2007-04-09 by cmkwon
	char				m_szGamePublisherName[SIZE_MAX_GAME_PUBLISHER_NAME];	// 2007-04-09 by cmkwon
	BOOL				m_bIsJamboreeServer;									// 2007-04-09 by cmkwon

#ifdef S_IP_UNIQUE_CONNECTION
	BOOL				m_bMultipleIPRestriction;
#endif
	
public:
	//BOOL				LoadPCBangIPListFromConfig(void);	// 2006-08-18 by dhjin
	setstring			m_setPCBangIPList;					// 2006-08-18 by dhjin
	
	BOOL				m_bCheckLogServer;
	BOOL				m_bEventActivated;

	BOOL				m_bCashShopServiceFlag;							// 2007-01-10 by cmkwon, 
	BOOL				m_bUseSecondaryPasswordSystemFlag;				// 2007-09-12 by cmkwon, 베트남 2차패스워드 구현 - CFieldGlobal 에 멤버 변수 추가
	BOOL GetUseSecondaryPasswordSystemFlag(void);						// 2007-09-12 by cmkwon, 베트남 2차패스워드 구현 - CFieldGlobal 에 멤버 함수 추가

	DWORD				m_dwLastWndMsg;
	DWORD				m_dwLastWndMsgTick;
	DWORD				m_dwLastWndMsgWParam;
	DWORD				m_dwLastWndMsgLParam;
	DWORD				m_dwLastSocketMessage;
	DWORD				m_dwLastSocketMessageParam1;
	DWORD				m_dwLastSocketMessageParam2;

	CPCBangIPManager	*m_pPCBangIPManager;

	// 2007-12-26 by dhjin, 아레나 통합 - 아레나 서버 정보
	SARENA_SERVER_INFO	m_sArenaServerInfo;	



	///////////////////////////////////////////////////////////////////////////////
	// 2008-04-29 by cmkwon, 서버군 정보 DB에 추가(신규 계정 캐릭터 생성 제한 시스템추가) - 
	SDBSERVER_GROUP		m_DBServerGroup;
	void SetDBServerGroup(SDBSERVER_GROUP *i_pDBServGroup);

	///////////////////////////////////////////////////////////////////////////////
	// 2008-07-30 by cmkwon, Yedang_Kor 빌링 DBServer 직접 연결하도록 수정 - 
	char				m_szBillingDBServerIP[SIZE_MAX_ODBC_CONN_STRING];
	int					m_nBillingDBServerPort;
	char				m_szBillingDBServerDatabaseName[SIZE_MAX_ODBC_CONN_STRING];
	char				m_szBillingDBServerUserID[SIZE_MAX_ODBC_CONN_STRING];
	char				m_szBillingDBServerPassword[SIZE_MAX_ODBC_CONN_STRING];
	char *GetBillingDBServerIP(void);
	int GetBillingDBServerPort(void);
	char *GetBillingDBServerDatabaseName(void);
	char *GetBillingDBServerUserID(void);
	char *GetBillingDBServerPassword(void);

	///////////////////////////////////////////////////////////////////////////////
	// 2009-06-03 by cmkwon, 세력 선택시 제한 시스템 사용 여부 플래그 추가 - 
	BOOL				m_bUseInflSelectionRestrictSystem;
	BOOL GetUseInflSelectionRestrictSystem(void);
	void SetUseInflSelectionRestrictSystem(BOOL i_bFlag);

	///////////////////////////////////////////////////////////////////////////////
	// 2013-12-07 by killburne, dga config	
	int						m_dgaMode;
	ATUM_DATE_TIME			m_dgaTime;

	int						m_dgaItemMinCount;
	int						m_dgaItemMaxCount;
	vector<dgaItem*>		m_dgaItems;

	int						m_rndDgaItemMinCount;
	int						m_rndDgaItemMaxCount;
	int						m_rndDgaDailyMaxCount;
	vector<dgaItem*>		m_rndDgaItems;
	vector<int>				m_rndDgaMaps;

	// 2015-09-14 Future, disabled Monster Maps during Influence war
	//vector<MapIndex_t>		m_vecInfluenceWarDisabledMonsterMaps;
};
#endif // !defined(AFX_FIELDGLOBAL_H__B6793946_A3E2_4778_A6DA_1EE835B77558__INCLUDED_)
