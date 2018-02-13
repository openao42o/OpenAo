// IOCP.h: interface for the IOCP.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IOCP_H__57FB97C3_C117_4E67_99B3_4CA554A27807__INCLUDED_)
#define AFX_IOCP_H__57FB97C3_C117_4E67_99B3_4CA554A27807__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "StdAfx.h"
#include <Mmsystem.h>
#include "winpacket.h"
#include "IOCPSocket.h"
#include "ThreadCheck.h"
#include "QueueINT.h"

#define	COUNT_MAX_IOCP_THREAD							20		// IOCP 스레드 최대 개수, IOCP 스레드 배열의 크기, 실제 생성되는 스레드 개수는 아님
#define COUNT_SOCKET_WRITE_THREAD						2		// IOCP Wirte Operation을 위한 스레드수

// DBGOUT2
#define DO2_NONE	(BitFlag8_t)0x00
#define DO2_DBGOUT	(BitFlag8_t)0x01
#define DO2_SYSLOG	(BitFlag8_t)0x02
#define DO2_STR128	(BitFlag8_t)0x04
#define DO2_ALL		(BitFlag8_t)(DO2_DBGOUT|DO2_SYSLOG|DO2_STR128)


DWORD WINAPI WorkerThread(LPVOID lpParam);
DWORD WINAPI ListenerThread(LPVOID lpParam);

struct IOCPWorkerTLSDATA
{
#ifdef _ATUM_FIELD_SERVER
	IOCPWorkerTLSDATA()
	{
		clientIndexVector.reserve(1000);
	}

	~IOCPWorkerTLSDATA()
	{
	}

	vector<ClientIndex_t>	clientIndexVector;		// 다른 캐릭터들에게 정보를 보낼 때 set에 대한 buffer로 사용
#endif	// _ATUM_FIELD_SERVER_endif	
};

///////////////////////////////////////////////////////////////////////////////
/// \class		CIOCP
///
/// \brief		IOCP 최상위 클래스
/// \author		cmkwon
/// \version	
/// \date		2004-03-17 ~ 2004-03-17
/// \warning	
///////////////////////////////////////////////////////////////////////////////
class CIOCP
{
	friend class CFieldIOCPSocket;
	friend class CNPCIOCPSocket;
	friend class CPreIOCPSocket;
	friend class CFieldMapWorkspace;
	friend class CFieldMapProject;
	friend class CIMIOCPSocket;

public:
	CIOCP(DWORD dwStartIndex, DWORD dwSessionSize, int nPort,
		char *szLocalIP="127.0.0.1", ENServerType ServerType=ST_NORMAL_SERVER);
	virtual ~CIOCP();

	///////////////////////////////////////////////////////////////////////////
	// Property
	DWORD GetServerType(void){				return m_nServerType;}
	char* GetLocalIPAddress(void){			return m_szLocalIPAddress;};
	int GetListenerPort(void){				return m_nListenerPort;}
	DWORD GetCurrentClientCounts(void){		return m_nCurrentClientCounts;}
	DWORD GetArrayClientSize(void){			return m_dwArrayClientSize;}
	BOOL GetListeningFlag(void){			return m_bListeningFlag;}
	BOOL GetServiceStartFlag(void){			return m_bServiceStartFlag;}
	BOOL GetFlagCalcTrafficInfo(void){		return m_bFlagCalcTrafficInfo;}
	void SetFlagCalcTrafficInfo(BOOL bFlag){	m_bFlagCalcTrafficInfo = bFlag;}
	void SetIOCPNetworkState(EN_NETWORK_STATE i_enNetState){	m_enIOCPNetworkState = i_enNetState;};
	EN_NETWORK_STATE GetIOCPNetworkState(void){		return m_enIOCPNetworkState;};


	///////////////////////////////////////////////////////////////////////////
	// Method
	//
	BOOL Bind(UINT nPort);
	BOOL AddIoCompletionPort(CIOCPSocket* pIOCPSocket);
	CIOCPSocket* FindEmptyIOCPSocket(DWORD dwIdx = COUNT_MAX_SOCKET_SESSION);
	CIOCPSocket* GetIOCPSocket(DWORD dwIdx);	
	void ListenerClose(void);
	DWORD GetCurrentServerTime(void);				// 서버가 실행된 이후 지난 시간을 초단위로 반환
	DWORD GetCurrentServerTimeInMilliSeconds(void);	// 서버가 실행된 이후 지난 시간을 Milli-Seconds 단위로 반환
	void GetLocalAddress(char *o_szIP);
	void LockClientArrayIndex(void);
	void UnlockClientArrayIndex(void);
	BOOL PushClientIndex(int i_nIdx);
	
	// 서버의 Bandwidth 계산 관련
	void LockTotalTrafficInfo(void);
	void UnlockTotalTrafficInfo(void);	
	STrafficInfo *GetPTCPTrafficInfo(void);
	STrafficInfo *GetPUDPTrafficInfo(void);
	void UpdateTCPTrafficInfo(STrafficInfo *pTCPTraffic);
	void UpdateUDPTrafficInfo(STrafficInfo *pUDPTraffic);
	

	// TLS 관련
	BOOL TLSDataInit(LPVOID &lpvData);
	BOOL TLSDataClean(LPVOID &lpvData);
	IOCPWorkerTLSDATA* GetIOCPWorkerTLSDATA();



	////////////////////////////////////////////////////////////////////////////
	// virtual멤버함수
	// CIOCP 객체 초기화 관련
	virtual BOOL IOCPInit(void);
	virtual void IOCPClean(void);

	// Thread Proc
	virtual DWORD Worker(void);
	virtual BOOL Listen(void);
	BOOL Writer(void);

	// 접속자 Alive Check
	virtual void CalcTotalTrafficInfo(void);
	virtual SThreadInfo *CheckIOCPThread(DWORD i_dwThreadIdToExclude);
	void ClientCheck(void);
	
	////////////////////////////////////////////////////////////////////////////
	// static 멤버함수
	// WinSock Library 초기화 관련
	static BOOL SocketInit(void);
	static void SocketClean(void);

	///////////////////////////////////////////////////////////////////////////
	//
	static EN_NETWORK_STATE SeekNetworkState(DWORD i_dwMaxBandwidth, DWORD i_dwCurrentBandwidth);
	
	///////////////////////////////////////////////////////////////////////////
	// Monitor
	void SendMessageToMonitor(BYTE *pData, int nDataSize);
	void SendMessageToAdminTool(BYTE *pData, int nDataSize);
	BOOL InsertMonitorIOCPSocketPtr(CIOCPSocket *i_pSocket);
	BOOL DeleteMonitorIOCPSocketPtr(CIOCPSocket *i_pSocket);

protected:
	DWORD				m_nServerType;							// IOCP Server Type, 생성자의 인자로 설정됨										
	HANDLE				m_hCompletionPort;						// IOCompletionPort 핸들	
	HANDLE				m_hWorkerThread[COUNT_MAX_IOCP_THREAD];	// Worker Thread 핸들 배열
	HANDLE				m_hThreadSocketWriteArray[COUNT_SOCKET_WRITE_THREAD];	// 실제 Socket에 데이타를 Write하는 스레드 핸들 배열
	BOOL				m_bThreadSocketWriteEndFlag;			// Write Thread 종료 플래그

	///////////////////////////////////////////////////////////////////////////
	// 소켓 Listenning 관련
	HANDLE				m_hListenerThread;						// 리스너 스레드 핸들
	int					m_nListenerPort;						// Listener port, Open할 포트 혹은 Opened 포트
	SOCKET				m_hListener;							// 리스너 소켓 핸들	
	BOOL				m_bListeningFlag;						// 리스너가 스레드가 정상적으로 작동하는지의 플래그
	BOOL				m_bServiceStartFlag;					// 서비스 플래그(FALSE이면 서비스 중지중, TRUE이면 정상동작중)	
	
	DWORD				m_nCurrentClientCounts;					// 현재 연결된 IOCPSocket Counts
	CIOCPSocket			*m_pArrayIOCPSocket;					// CIOCPSocket의 포인터, 생성자에서 동적으로 최대 지원수 만큼 생성되어 할당된다.
	CIOCPSocket*		m_ArrayClient[COUNT_MAX_SOCKET_SESSION];// CIOCPSocket의 포인터 배열로 위의 m_pArrayIOCPSocket을 편리하게 사용하기 위한 변수

	CQueueINT			m_queueClientIndex;
	DWORD				m_dwArrayClientSize;					// 최대 지원 접속자수, 생성자의 인자로 받음

	DWORD				m_dwStartClientIndex;					// 사용되지 않는 IOCPSocket을 요청시 이변수보다 큰 Index로 할당됨
	DWORD				m_dwLastClientIndex;					// IOCPSocket을 원형으로 할당하기 위한변수
	int					m_dwWorkerCount;						// 실행될 Worker 스레드 숫자, Worker Thread count = CPU_NUM + 2
	CRITICAL_SECTION	m_crtlClientArray;						// 소켓 배열의 인덱스값의 동기화를 위한 criticalsection
	
	DWORD				m_dwTimeStarted;						// 서버가 실행된 시간, 초단위
	DWORD				m_dwTickStarted;						// 서버가 실행된 시간, 밀리세컨드단위
	char				m_szLocalIPAddress[SIZE_MAX_IPADDRESS];	// 서버가 실행된 PC의 LocalIP Address
	
	
	///////////////////////////////////////////////////////////////////////////
	// TLS 관련
	DWORD				m_dwTlsIndex;							// For TLS


	///////////////////////////////////////////////////////////////////////////
	// Bandwidth 계산 관련
	CRITICAL_SECTION	m_crtlTotalTrafficInfo;					// Bandwidth 계산을 위한 변수들의 동기화를 위한 크리티컬 섹션
	BOOL				m_bFlagCalcTrafficInfo;					// Bandwidth 계산을 할것인지의 플래그
	STrafficInfo		m_TCPTrafficInfo[2];					// TCP 통신 Bandwidth 계산을 위한 변수, 두번째는 계산된결과다 저장된다.
	STrafficInfo		m_UDPTrafficInfo[2];					// UDP 통신 Bandwidth 계산을 위한 변수, 두번째는 계산된결과다 저장된다.
	

	///////////////////////////////////////////////////////////////////////////
	// 스레드 체크 관련
	CThreadCheck		m_IOCPThreadCheck;						// Worker 혹은 Listener 스레드가 죽거나 Block 상태를 체크하기위한 변수

	///////////////////////////////////////////////////////////////////////////
	// IOCP Server 패킷 레벨 시스템 관련
	EN_NETWORK_STATE	m_enIOCPNetworkState;
	UINT				m_uTrafficMaxBandwidth;					// 패킷 레벨 시스템에서 사용할 기준 Bandwidth로 단위는 bps(bit per second)

	///////////////////////////////////////////////////////////////////////////
	// static 멤버 변수
	static BOOL			m_bSocketInitFlag;						// WinSock Library 초기화 플래그

	///////////////////////////////////////////////////////////////////////////
	// Monitor
	mtvectorIOCPSocket	m_MonitorIOCPSocketPtrVector;

};

#endif // !defined(AFX_IOCPSOCKET_H__57FB97C3_C117_4E67_99B3_4CA554A27807__INCLUDED_)
