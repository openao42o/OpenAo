#pragma once

#include <thread>
#include <timeapi.h>

#include "IOCPSocket.h"
#include "ThreadCheck.h"
#include "QueueINT.h"


constexpr auto COUNT_MAX_IOCP_THREAD = 500;		// IOCP 스레드 최대 개수, IOCP 스레드 배열의 크기, 실제 생성되는 스레드 개수는 아님
constexpr auto COUNT_SOCKET_WRITE_THREAD = 50;		// IOCP Wirte Operation을 위한 스레드수

enum : BitFlag8_t
{
	DO2_NONE = 0x00,
	DO2_DBGOUT = 0x01,
	DO2_SYSLOG = 0x02,
	DO2_STR128 = 0x04,
	DO2_ALL = (DO2_DBGOUT | DO2_SYSLOG | DO2_STR128)
};

DWORD WINAPI WorkerThread(void* lpParam);
DWORD WINAPI ListenerThread(void* lpParam);

struct IOCPWorkerTLSDATA
{
#ifdef _ATUM_FIELD_SERVER
	vector<ClientIndex_t> clientIndexVector;		// 다른 캐릭터들에게 정보를 보낼 때 set에 대한 buffer로 사용

	IOCPWorkerTLSDATA() : clientIndexVector { } { clientIndexVector.reserve(1000); }
#endif
};

class CIOCP
{
	friend class CFieldIOCPSocket;
	friend class CNPCIOCPSocket;
	friend class CPreIOCPSocket;
	friend class CFieldMapWorkspace;
	friend class CFieldMapProject;
	friend class CIMIOCPSocket;

public:
	CIOCP(DWORD dwStartIndex, DWORD dwSessionSize, int nPort, char *szLocalIP, ENServerType ServerType, size_t SizeOfSocket);
	
	virtual ~CIOCP()
	{
		DeleteCriticalSection(&m_crtlClientArray);
		DeleteCriticalSection(&m_crtlTotalTrafficInfo);
	}

	///////////////////////////////////////////////////////////////////////////
	// Property
	DWORD GetServerType() const { return m_nServerType; }
	const char* GetLocalIPAddress() const { return  m_szLocalIPAddress; } // todo : check this out, temporaray workaround

	int GetListenerPort() const { return m_nListenerPort; }
	DWORD GetCurrentClientCounts() const { return m_nCurrentClientCounts; }
	DWORD GetArrayClientSize() const { return m_dwArrayClientSize; }
	bool GetListeningFlag() const { return m_bListeningFlag; }
	bool GetServiceStartFlag() const { return m_bServiceStartFlag; }

	bool GetFlagCalcTrafficInfo() const { return m_bFlagCalcTrafficInfo; }
	void SetFlagCalcTrafficInfo(bool bFlag) { m_bFlagCalcTrafficInfo = bFlag; }

	EN_NETWORK_STATE GetIOCPNetworkState() const { return m_enIOCPNetworkState; };
	void SetIOCPNetworkState(EN_NETWORK_STATE i_enNetState) { m_enIOCPNetworkState = i_enNetState; };

	BOOL Bind(UINT nPort);
	BOOL AddIoCompletionPort(CIOCPSocket* pIOCPSocket) const;
	CIOCPSocket* FindEmptyIOCPSocket(DWORD dwIdx = 0xFFFFFFFF); // todo : fix this temporary workaround // COUNT_MAX_SOCKET_SESSION);
	CIOCPSocket* GetIOCPSocket(DWORD dwIdx) const;
	
	// this call may block
	void ListenerClose();
	
	DWORD GetCurrentServerTime() const { return m_dwTimeStarted + (timeGetTime() - m_dwTickStarted) / 1000; }	// 서버가 실행된 이후 지난 시간을 초단위로 반환
	DWORD GetCurrentServerTimeInMilliSeconds() const { return timeGetTime() - m_dwTickStarted; }	// 서버가 실행된 이후 지난 시간을 Milli-Seconds 단위로 반환
	
	static void GetLocalAddress(char *o_szIP);
	
	void LockClientArrayIndex() { EnterCriticalSection(&m_crtlClientArray); }
	void UnlockClientArrayIndex() { LeaveCriticalSection(&m_crtlClientArray); }
	
	BOOL PushClientIndex(int i_nIdx);
	
	// DevX manages bandwidth

	void LockTotalTrafficInfo() { EnterCriticalSection(&m_crtlTotalTrafficInfo); }
	void UnlockTotalTrafficInfo() { LeaveCriticalSection(&m_crtlTotalTrafficInfo); }

	STrafficInfo *GetPTCPTrafficInfo() { return &m_TCPTrafficInfo[1]; }
	STrafficInfo *GetPUDPTrafficInfo() { return &m_UDPTrafficInfo[1]; }

	void UpdateTCPTrafficInfo(STrafficInfo *pTCPTraffic) { LockTotalTrafficInfo(); m_TCPTrafficInfo[0] += *pTCPTraffic; UnlockTotalTrafficInfo(); }
	void UpdateUDPTrafficInfo(STrafficInfo *pUDPTraffic) { LockTotalTrafficInfo(); m_UDPTrafficInfo[0] += *pUDPTraffic; UnlockTotalTrafficInfo(); }

	// TLS 관련

	//BOOL TLSDataInit(void* &lpvData);
	//BOOL TLSDataClean(void* &lpvData);
	static IOCPWorkerTLSDATA* GetIOCPWorkerTLSDATA() { return &workerdata; }

	thread_local static IOCPWorkerTLSDATA workerdata;

	// virtual멤버함수
	// CIOCP 객체 초기화 관련
	virtual BOOL IOCPInit();
	virtual void IOCPClean();

	// Thread Proc
	virtual DWORD Worker();
	virtual BOOL Listen();
	BOOL Writer();

	// 접속자 Alive Check
	virtual void CalcTotalTrafficInfo();
	virtual SThreadInfo *CheckIOCPThread(DWORD i_dwThreadIdToExclude);
	void ClientCheck();

	// 2010-04-08 by cmkwon, 서버에서 맵로딩시 AlivePacket 전송 추가 - 
	virtual void SendAlivePacket2OtherServers() { } // do nothing

	// static 멤버함수
	// WinSock Library 초기화 관련
	static bool SocketInit();
	static void SocketClean();

	// Monitor
	void SendMessageToMonitor(BYTE *pData, int nDataSize);
	template<template<MessageType_t> typename msg_t, MessageType_t t>
	void SendMessageToMonitor(msg_t<t>& msg) { this->SendMessageToMonitor(reinterpret_cast<BYTE*>(&msg), msg.size()); }

	void SendMessageToAdminTool(BYTE *pData, int nDataSize);
	template<template<MessageType_t> typename msg_t, MessageType_t t>
	void SendMessageToAdminTool(msg_t<t>& msg) { this->SendMessageToAdminTool(reinterpret_cast<BYTE*>(&msg), msg.size()); }

	BOOL InsertMonitorIOCPSocketPtr(CIOCPSocket *i_pSocket);
	BOOL DeleteMonitorIOCPSocketPtr(CIOCPSocket *i_pSocket);

protected:

	DWORD				m_nServerType;							// IOCP Server Type, 생성자의 인자로 설정됨										
	HANDLE				m_hCompletionPort;						// IOCompletionPort 핸들	
	//HANDLE				m_hWorkerThread[COUNT_MAX_IOCP_THREAD];	// Worker Thread 핸들 배열
	//HANDLE				m_hThreadSocketWriteArray[COUNT_SOCKET_WRITE_THREAD];	// 실제 Socket에 데이타를 Write하는 스레드 핸들 배열
	thread				m_threadWorker[COUNT_MAX_IOCP_THREAD];
	thread				m_threadSocketWrite[COUNT_SOCKET_WRITE_THREAD];
	BOOL				m_bThreadSocketWriteEndFlag;			// Write Thread 종료 플래그

	// 소켓 Listenning 관련
	//HANDLE				m_hListenerThread;						// 리스너 스레드 핸들
	thread				m_threadListener;
	int					m_nListenerPort;						// Listener port, Open할 포트 혹은 Opened 포트
	SOCKET				m_hListener;							// 리스너 소켓 핸들	
	BOOL				m_bListeningFlag;						// 리스너가 스레드가 정상적으로 작동하는지의 플래그
	BOOL				m_bServiceStartFlag;					// 서비스 플래그(FALSE이면 서비스 중지중, TRUE이면 정상동작중)	

	DWORD				m_nCurrentClientCounts;					// 현재 연결된 IOCPSocket Counts
	CIOCPSocket*		m_pArrayIOCPSocket;					// CIOCPSocket의 포인터, 생성자에서 동적으로 최대 지원수 만큼 생성되어 할당된다.
	//CIOCPSocket*		m_ArrayClient[COUNT_MAX_SOCKET_SESSION];// CIOCPSocket의 포인터 배열로 위의 m_pArrayIOCPSocket을 편리하게 사용하기 위한 변수

	const size_t		m_SizeOfSocket;
	CIOCPSocket*		getSocket(size_t index) const { return reinterpret_cast<CIOCPSocket*>(reinterpret_cast<char*>(m_pArrayIOCPSocket) + (index * m_SizeOfSocket)); }

	CQueueINT			m_queueClientIndex;
	DWORD				m_dwArrayClientSize;					// 최대 지원 접속자수, 생성자의 인자로 받음

	DWORD				m_dwStartClientIndex;					// 사용되지 않는 IOCPSocket을 요청시 이변수보다 큰 Index로 할당됨
	DWORD				m_dwLastClientIndex;					// IOCPSocket을 원형으로 할당하기 위한변수
	int					m_dwWorkerCount;						// 실행될 Worker 스레드 숫자, Worker Thread count = CPU_NUM + 2
	CRITICAL_SECTION	m_crtlClientArray;						// 소켓 배열의 인덱스값의 동기화를 위한 criticalsection

	time_t				m_dwTimeStarted;						// 서버가 실행된 시간, 초단위
	DWORD				m_dwTickStarted;						// 서버가 실행된 시간, 밀리세컨드단위
	char				m_szLocalIPAddress[SIZE_MAX_IPADDRESS];	// 서버가 실행된 PC의 LocalIP Address

	// TLS 관련
	//DWORD				m_dwTlsIndex;							// For TLS

	// Bandwidth 계산 관련
	CRITICAL_SECTION	m_crtlTotalTrafficInfo;					// Bandwidth 계산을 위한 변수들의 동기화를 위한 크리티컬 섹션
	BOOL				m_bFlagCalcTrafficInfo;					// Bandwidth 계산을 할것인지의 플래그
	STrafficInfo		m_TCPTrafficInfo[2];					// TCP 통신 Bandwidth 계산을 위한 변수, 두번째는 계산된결과다 저장된다.
	STrafficInfo		m_UDPTrafficInfo[2];					// UDP 통신 Bandwidth 계산을 위한 변수, 두번째는 계산된결과다 저장된다.

	// 스레드 체크 관련
	CThreadCheck		m_IOCPThreadCheck;						// Worker 혹은 Listener 스레드가 죽거나 Block 상태를 체크하기위한 변수

	// IOCP Server 패킷 레벨 시스템 관련
	EN_NETWORK_STATE	m_enIOCPNetworkState;
	UINT				m_uTrafficMaxBandwidth;					// 패킷 레벨 시스템에서 사용할 기준 Bandwidth로 단위는 bps(bit per second)

	// static 멤버 변수
	static bool			m_bSocketInitFlag;						// WinSock Library 초기화 플래그
	
	// Monitor
	mtvectorIOCPSocket	m_MonitorIOCPSocketPtrVector;

};