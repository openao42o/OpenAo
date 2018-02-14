#include "stdafx.h"
#include "SocketHeader.h"
#include "IOCP.h"
#include "GlobalGameServer.h"

thread_local IOCPWorkerTLSDATA CIOCP::workerdata { };

DWORD WINAPI ListenerThread(void* lpParam)
{
	return reinterpret_cast<CIOCP*>(lpParam)->Listen();
}

DWORD WINAPI WorkerThread(void* lpParam)
{
	return reinterpret_cast<CIOCP*>(lpParam)->Worker();
}

DWORD WINAPI IOCPSocketWriteThread(void* lpParam)
{
	return reinterpret_cast<CIOCP*>(lpParam)->Writer();
}

bool CIOCP::m_bSocketInitFlag = false;

bool CIOCP::SocketInit()
{
	if (m_bSocketInitFlag) return true;

	WSADATA wsaData;
		
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) return false;

	if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2)
	{
		WSACleanup();
		return false;
	}

	m_bSocketInitFlag = true;

	return true;
}

void CIOCP::SocketClean()
{
	if (m_bSocketInitFlag) WSACleanup();
}

CIOCP::CIOCP(DWORD dwStartIndex, DWORD dwSessionSize, int nPort, char *szLocalIP, ENServerType ServerType, size_t SizeOfSocket) :
	m_nServerType { ServerType },
	m_hCompletionPort { nullptr },
	//m_hWorkerThread { },
	//m_hThreadSocketWriteArray { },
	m_bThreadSocketWriteEndFlag { FALSE },
	//m_hListenerThread { nullptr },
	m_nListenerPort { nPort },
	m_hListener { INVALID_SOCKET },
	m_bListeningFlag { false },
	m_bServiceStartFlag { false },
	m_nCurrentClientCounts { 0 },
	m_pArrayIOCPSocket { nullptr },
	m_SizeOfSocket { SizeOfSocket },
	m_queueClientIndex { },
	m_dwArrayClientSize { dwSessionSize },
	m_dwStartClientIndex { min(dwStartIndex, dwSessionSize - 1) },
	m_dwLastClientIndex { 0 },
	m_dwWorkerCount { 0 },
	m_dwTimeStarted { 0 },
	m_dwTickStarted { 0 },
	m_szLocalIPAddress { },
	//m_dwTlsIndex { TLS_OUT_OF_INDEXES },
	m_bFlagCalcTrafficInfo { false },
	m_TCPTrafficInfo { },
	m_UDPTrafficInfo { },
	m_enIOCPNetworkState { EN_NETWORK_STATE_BEST },
	m_uTrafficMaxBandwidth { 50 * 1000 * 1000 }
{
	time(&m_dwTimeStarted);
	m_dwTickStarted = timeGetTime();

	m_TCPTrafficInfo[0].dwTimeGapSecond = m_dwTickStarted;
	m_UDPTrafficInfo[0].dwTimeGapSecond = m_dwTickStarted;

	m_queueClientIndex.InitQueueINT(m_dwStartClientIndex, m_dwArrayClientSize - 1);

	if (szLocalIP[0] == '\0' || strncmp(szLocalIP, "127.0.0.1", sizeof(m_szLocalIPAddress)) == 0)
	{
		char host[100];
		gethostname(host, 100);

		auto p = gethostbyname(host);

		if (p != nullptr)
		{
			auto ip = p->h_addr_list[1] != nullptr ? p->h_addr_list[1] : p->h_addr_list[0];

			snprintf(m_szLocalIPAddress, sizeof(m_szLocalIPAddress), "%hhu.%hhu.%hhu.%hhu", BYTE(ip[0]), BYTE(ip[1]), BYTE(ip[2]), BYTE(ip[3]));
		}
	}
	else util::strncpy(m_szLocalIPAddress, szLocalIP);

	InitializeCriticalSection(&m_crtlClientArray);
	InitializeCriticalSection(&m_crtlTotalTrafficInfo);

	CIOCPSocket::ms_pIOCP = this;
}

BOOL CIOCP::Bind(UINT nPort)
{
	m_hListener = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_IP, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (INVALID_SOCKET == m_hListener)
	{
		int nErr = GetLastError();
		SetLastError(0);
		server::log(true, "CIOCP::Bind WSASocket() LastError[%d]\r\n", nErr);
		return FALSE;
	}

	SOCKADDR_IN 	serv_addr;
	memset(&serv_addr, 0x00, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(nPort);

	if (SOCKET_ERROR == ::bind(m_hListener, LPSOCKADDR(&serv_addr), sizeof(serv_addr)))
	{
		int nErr = GetLastError();
		SetLastError(0);

		server::log(true, "CIOCP::Bind bind() LastError[%d] Port[%4d]\r\n", nErr, nPort);

		closesocket(m_hListener);
		m_hListener = INVALID_SOCKET;
		return FALSE;
	}
	DbgOut("Listener open success port[%d]\n", nPort);

	return TRUE;
}

BOOL CIOCP::AddIoCompletionPort(CIOCPSocket* pIOCPSocket) const
{
	if (!m_hCompletionPort || !pIOCPSocket->IsUsing() || pIOCPSocket->m_hSocket == INVALID_SOCKET) return false;
	
	auto hret = CreateIoCompletionPort(HANDLE(pIOCPSocket->m_hSocket), m_hCompletionPort, DWORD(pIOCPSocket), 0);

	if (hret == nullptr)
	{
		pIOCPSocket->Close(0x10005);

		return false;
	}

	return true;
}

CIOCPSocket* CIOCP::FindEmptyIOCPSocket(DWORD dwIdx)
{
	if (!m_pArrayIOCPSocket) return nullptr;

	CIOCPSocket *pRetSocket;

	if (dwIdx < m_dwArrayClientSize) pRetSocket = getSocket(dwIdx); // UDP 통신에 사용

	else
	{
		int nRetedIdx;

		if (!m_queueClientIndex.PopQueueINT(&nRetedIdx)) return nullptr;

		pRetSocket = GetIOCPSocket(nRetedIdx);
	}

	if (!pRetSocket || pRetSocket->IsUsing() || !pRetSocket->m_mtCritSecForClose.Try()) return nullptr;

	pRetSocket->m_mtCritSecForClose.Leave();

	return pRetSocket;
}

CIOCPSocket* CIOCP::GetIOCPSocket(DWORD dwIdx) const
{
	if (m_pArrayIOCPSocket == nullptr || dwIdx >= m_dwArrayClientSize) return nullptr;

	return getSocket(dwIdx); //m_ArrayClient[dwIdx];
}

void CIOCP::ListenerClose()
{
	m_bListeningFlag = false;

	if (m_hListener != INVALID_SOCKET)
		
		closesocket(m_hListener);

	m_hListener = INVALID_SOCKET;

	if (m_threadListener.joinable()) m_threadListener.join();

	m_bServiceStartFlag = false;
}

void CIOCP::GetLocalAddress(char* o_szIP)
{
	memset(o_szIP, 0x00, SIZE_MAX_IPADDRESS);

	char host[100];
	gethostname(host, 100);
	
	auto p = gethostbyname(host);

	if (p != nullptr)
	{
		auto ip = p->h_addr_list[0];
		sprintf(o_szIP, "%hhu.%hhu.%hhu.%hhu", (BYTE)ip[0], (BYTE)ip[1], (BYTE)ip[2], (BYTE)ip[3]);
	}
}

void CIOCP::ClientCheck()
{
	for (auto i = 0; i < m_dwArrayClientSize; i++)
	{
		if (!GetListeningFlag()) break;

		auto socket = getSocket(i);

		if (!socket->IsUsing() || socket->m_bUDPFlag) continue;

		if (socket->m_bMustClose)
		{
			socket->Close(0x10009);
			continue;
		}

		socket->ClientCheck();
	}
}

BOOL CIOCP::PushClientIndex(int i_nIdx)
{
	if (!m_queueClientIndex.IsValidDataNum(i_nIdx)) return true;

	return m_queueClientIndex.PushQueueINT(i_nIdx);
}

void CIOCP::CalcTotalTrafficInfo()
{
	if (!GetFlagCalcTrafficInfo()) return;

	auto dwCurTick = timeGetTime();

	///////////////////////////////////////////////////////////////////////////
	// DevX copies traffic info to a secondary buffer while traffic info is locked
	this->LockTotalTrafficInfo();

	m_TCPTrafficInfo[1] = m_TCPTrafficInfo[0];
	m_TCPTrafficInfo[0].Reset(dwCurTick);

	m_UDPTrafficInfo[1] = m_UDPTrafficInfo[0];
	m_UDPTrafficInfo[0].Reset(dwCurTick);

	this->UnlockTotalTrafficInfo();

	///////////////////////////////////////////////////////////////////////////
	// Time이 1보다 작을경우 1로 처리한다.
	m_TCPTrafficInfo[1].dwTimeGapSecond = max(1, (dwCurTick - m_TCPTrafficInfo[1].dwTimeGapSecond) / 1000);
	m_UDPTrafficInfo[1].dwTimeGapSecond = max(1, (dwCurTick - m_UDPTrafficInfo[1].dwTimeGapSecond) / 1000);

	///////////////////////////////////////////////////////////////////////////
	// 현재 사용되고 있는 IOCPSocket에서 계산을 위해 Bandwidth 정보를 가져온다
	int i = 0;
	while (GetListeningFlag()  && i < m_dwArrayClientSize && m_pArrayIOCPSocket)
	{
		auto socket = getSocket(i);
		if (socket->IsUsing())
		{
			if (!socket->m_bUDPFlag)
			{
				socket->LockTrafficInfo();
				m_TCPTrafficInfo[1] += socket->m_TrafficInfo;
				memset(&socket->m_TrafficInfo, 0x00, sizeof(STrafficInfo));
				socket->UnlockTrafficInfo();
			}
			else
			{
				socket->LockTrafficInfo();
				m_UDPTrafficInfo[1] += socket->m_TrafficInfo;
				memset(&socket->m_TrafficInfo, 0x00, sizeof(STrafficInfo));
				socket->UnlockTrafficInfo();
			}
		}
		i++;
	}

	///////////////////////////////////////////////////////////////////////////
	// TCP LanHeader 와 UDP LanHeader를 더하여 준다
	m_TCPTrafficInfo[1].dwBytesSend += m_TCPTrafficInfo[1].dwCountSendPacket * SIZE_TCP_LAN_HEADER;
	m_TCPTrafficInfo[1].dwBytesRecv += m_TCPTrafficInfo[1].dwCountRecvPacket * SIZE_TCP_LAN_HEADER;
	m_UDPTrafficInfo[1].dwBytesSend += m_UDPTrafficInfo[1].dwCountSendPacket * SIZE_UDP_LAN_HEADER;
	m_UDPTrafficInfo[1].dwBytesRecv += m_UDPTrafficInfo[1].dwCountRecvPacket * SIZE_UDP_LAN_HEADER;

	///////////////////////////////////////////////////////////////////////////
	// DevX calculates network state (inlined method)
	EN_NETWORK_STATE enNetState;
	{
		auto i_dwCurrentBandwidth = (m_TCPTrafficInfo[1].dwBytesRecv * 8) / m_TCPTrafficInfo[1].dwTimeGapSecond;

		if (m_uTrafficMaxBandwidth == 0 || i_dwCurrentBandwidth == 0) enNetState = EN_NETWORK_STATE_BEST;

		else if (m_uTrafficMaxBandwidth <= i_dwCurrentBandwidth) enNetState = EN_NETWORK_STATE_WORST;

		else
		{
			int nHalfBandwidth = m_uTrafficMaxBandwidth / 2;
			int nOne_Ten = m_uTrafficMaxBandwidth / 10;
			int nRemainBandwidth = m_uTrafficMaxBandwidth - i_dwCurrentBandwidth;

			if (nRemainBandwidth > nHalfBandwidth) enNetState = EN_NETWORK_STATE_BEST;
			else if (nRemainBandwidth > nHalfBandwidth - 1 * nOne_Ten) enNetState = EN_NETWORK_STATE_GOOD;
			else if (nRemainBandwidth > nHalfBandwidth - 2 * nOne_Ten) enNetState = EN_NETWORK_STATE_NORMAL;
			else if (nRemainBandwidth > nHalfBandwidth - 3 * nOne_Ten) enNetState = EN_NETWORK_STATE_BAD;
			else enNetState = EN_NETWORK_STATE_WORST;
		}
	}
	///////////////////////////////////////////////////////////////////////////
	

	if (enNetState != m_enIOCPNetworkState)
	{
		server::log(true, "[Notify] CIOCP::CalcTotalTrafficInfo_1 Network State Change (%d) ==> (%d)\r\n",
			m_enIOCPNetworkState, enNetState);
		
		m_enIOCPNetworkState = enNetState;
	}

	///////////////////////////////////////////////////////////////////////////
	// Bandwidth를 System Log로 남긴다.
	if (GetFlagCalcTrafficInfo())
	{
		STrafficInfo *pTCP = GetPTCPTrafficInfo();
		STrafficInfo *pUDP = GetPUDPTrafficInfo();

		UINT	TCPRecv_bps, TCPRecv_cps, TCPSend_bps, TCPSend_cps;
		UINT	UDPRecv_bps, UDPRecv_cps, UDPSend_bps, UDPSend_cps;

		TCPRecv_bps = (pTCP->dwBytesRecv * 8) / pTCP->dwTimeGapSecond;
		TCPRecv_cps = pTCP->dwCountRecvPacket / pTCP->dwTimeGapSecond;
		TCPSend_bps = (pTCP->dwBytesSend * 8) / pTCP->dwTimeGapSecond;
		TCPSend_cps = pTCP->dwCountSendPacket / pTCP->dwTimeGapSecond;
		UDPRecv_bps = (pUDP->dwBytesRecv * 8) / pUDP->dwTimeGapSecond;
		UDPRecv_cps = pUDP->dwCountRecvPacket / pUDP->dwTimeGapSecond;
		UDPSend_bps = (pUDP->dwBytesSend * 8) / pUDP->dwTimeGapSecond;
		UDPSend_cps = pUDP->dwCountSendPacket / pUDP->dwTimeGapSecond;

		char szSysLog[1024];

		sprintf(szSysLog, "Traffic\r\n\t\t\tTotal : Recv[%10d bps, %6d cps]	\t\t\t\t\t\t	Send[%10d bps, %6d cps]\r\n"
			, TCPRecv_bps + UDPRecv_bps, TCPRecv_cps + UDPRecv_cps, TCPSend_bps + UDPSend_bps, TCPSend_cps + UDPSend_cps);
		sprintf(&szSysLog[strlen(szSysLog)], "\t\t\tTCP   : Recv[%10d bps, %6d cps, %8d bytes, %6d Counts] Send[%10d bps, %6d cps, %8d bytes, %6d Counts] %d sec\r\n"
			, TCPRecv_bps, TCPRecv_cps, pTCP->dwBytesRecv, pTCP->dwCountRecvPacket, TCPSend_bps, TCPSend_cps, pTCP->dwBytesSend, pTCP->dwCountSendPacket, pTCP->dwTimeGapSecond);
		sprintf(&szSysLog[strlen(szSysLog)], "\t\t\tUDP   : Recv[%10d bps, %6d cps, %8d bytes, %6d Counts] Send[%10d bps, %6d cps, %8d bytes, %6d Counts] %d sec\r\n"
			, UDPRecv_bps, UDPRecv_cps, pUDP->dwBytesRecv, pUDP->dwCountRecvPacket, UDPSend_bps, UDPSend_cps, pUDP->dwBytesSend, pUDP->dwCountSendPacket, pUDP->dwTimeGapSecond);
		g_pGlobal->WriteSystemLog(szSysLog);
	}
}

BOOL CIOCP::Writer()
{
	server::log(true, "CIOCP::Writer_ WriterThread, \t\t\t\t\t ThreadID(%6d, 0x%X)\r\n", GetCurrentThreadId(), GetCurrentThreadId());

	SThreadInfo *pstInfo = NULL;
	SThreadInfo	stInfo;
	memset(&stInfo, 0x00, sizeof(SThreadInfo));
	stInfo.dwThreadId = GetCurrentThreadId();
	stInfo.enThreadCheckType = THREAD_CHECK_TYPE_IOCP_WRITER;
	m_IOCPThreadCheck.AddThreadInfo(stInfo);
	while (true)
	{
		pstInfo = m_IOCPThreadCheck.GetThreadInfo(stInfo.dwThreadId);
		if (pstInfo) break;
		this_thread::sleep_for(100ms);
	}

	int idx;

	while (!m_bThreadSocketWriteEndFlag)
	{
		pstInfo->dwLastUseStartTick = timeGetTime();
		pstInfo->bThreadUseFlag = true;
		for (idx = 0; idx < m_dwArrayClientSize; idx++)
		{
			pstInfo->dwSocketIndex = idx;
			getSocket(idx)->Write(false);
		}
		pstInfo->bThreadUseFlag = false;
		this_thread::sleep_for(30ms);	// check: 원활한 디버깅 위해서, 20031014, kelovon, 원래값 30ms
	}

	return 4;
}

BOOL CIOCP::IOCPInit()
{
	if (m_bServiceStartFlag) return true;

	server::log(true, "########################	Server IOCPInit		########################\r\n");

	SYSTEM_INFO SystemInfo;
	GetSystemInfo(&SystemInfo);

	m_dwWorkerCount = SystemInfo.dwNumberOfProcessors + 2;				// 시스템의 CPU 개수 + 2

	m_hCompletionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, SystemInfo.dwNumberOfProcessors);
	
	if (!m_hCompletionPort)
	{
		m_bServiceStartFlag = false;

		server::log(true, "[Error] CIOCP::IOCPInit CreateIoCompletionPort, LastError[%d] Processors[%d]\r\n", GetLastError(), SystemInfo.dwNumberOfProcessors);

		SetLastError(0);

		return false;
	}


	m_IOCPThreadCheck.SetCheckThreadCounts(m_dwWorkerCount + COUNT_SOCKET_WRITE_THREAD + 1);


	try { for (auto i = 0; i < m_dwWorkerCount; i++) m_threadWorker[i] = thread { WorkerThread, LPVOID(this) }; }

	catch (const system_error& error)
	{
		m_bServiceStartFlag = false;

		g_pGlobal->WriteSystemLogEX(true, "[Error] CIOCP::IOCPInit() WorkerThread create error, WorkerCount[%d]\r\n", m_dwWorkerCount);

		return false;
	}

	try { for (auto& t : m_threadSocketWrite) t = thread { IOCPSocketWriteThread, LPVOID(this) }; }

	catch (const system_error& error)
	{
		m_bServiceStartFlag = false;

		g_pGlobal->WriteSystemLogEX(true, "CIOCP::IOCPInit() IOCPSocketWriteThread thread create error, WorkerCount[%d]\r\n", m_dwWorkerCount);

		return false;
	}


	try { m_threadListener = thread { ListenerThread, LPVOID(this) }; }
	
	catch (const system_error& error)
	{
		m_bServiceStartFlag = false;

		g_pGlobal->WriteSystemLogEX(true, "CIOCP::IOCPInit() ListenerThread create error\r\n");

		return false;
	}

	m_bServiceStartFlag = true;		// 2008-09-08 by cmkwon, 위에서 설정하던것으로 모든것을 처리후에 여기에서 설정하다록 한다.
	
	return true;
}

void CIOCP::IOCPClean()
{
	ListenerClose();

	if (m_hCompletionPort)
	{
		m_bThreadSocketWriteEndFlag = true;

		for (auto& t : m_threadSocketWrite) if (t.joinable()) t.join();

		if (m_pArrayIOCPSocket) for (auto i = 0; i < m_dwArrayClientSize; i++)
		{
			auto socket = getSocket(i);

			if (socket->IsUsing()) socket->Close(0x00002);
		}

		for (auto i = 0; i < m_dwWorkerCount; i++) PostQueuedCompletionStatus(m_hCompletionPort, 0, NULL, NULL);

		for (auto i = 0; i < m_dwWorkerCount; i++) if (m_threadWorker[i].joinable()) m_threadWorker[i].join();

		CloseHandle(m_hCompletionPort);
		m_hCompletionPort = NULL;
	}

	server::log(true, "########################	Server IOCPClean	########################\r\n\r\n");
}

BOOL CIOCP::Listen()
{
	server::log(true, "CIOCP::Listen_ ListenerThread, \t\t\t\t ThreadID(%6d, 0x%X)\r\n", GetCurrentThreadId(), GetCurrentThreadId());

	if (m_hListener != INVALID_SOCKET) return FALSE;

	if (!Bind(m_nListenerPort))
	{
		MessageBox(NULL, "Cannot Bind Local Address!", NULL, MB_OK);
		return FALSE;
	}

	if (SOCKET_ERROR == listen(m_hListener, SOMAXCONN))
	{
		int nErr = WSAGetLastError();
		WSASetLastError(0);

		server::log("CIOCP::Listen listen() LastError[%d] hListener[0x%X]\r\n", nErr, m_hListener);

		return false;
	}
	m_bListeningFlag = true;

	SThreadInfo *pstInfo = NULL;
	SThreadInfo	stInfo;
	memset(&stInfo, 0x00, sizeof(SThreadInfo));
	stInfo.dwThreadId = GetCurrentThreadId();
	stInfo.enThreadCheckType = THREAD_CHECK_TYPE_IOCP_LISTENER;
	m_IOCPThreadCheck.AddThreadInfo(stInfo);
	while (true)
	{
		pstInfo = m_IOCPThreadCheck.GetThreadInfo(stInfo.dwThreadId);
		if (pstInfo) break;
		this_thread::sleep_for(100ms);
	}

	sockaddr_in addr;
	int nSize = sizeof(addr);
	
	while (true)
	{
		auto soc = WSAAccept(m_hListener, reinterpret_cast<sockaddr*>(&addr), &nSize, nullptr, 0);

		pstInfo->dwLastUseStartTick = timeGetTime();
		pstInfo->bThreadUseFlag = true;

		if (soc != INVALID_SOCKET)
		{
			char szAddress[SIZE_MAX_IPADDRESS];

			util::strncpy(szAddress, inet_ntoa(addr.sin_addr));

			int nPort = ntohs(addr.sin_port);

			if (szAddress[0] == '\0') DbgOut("\n Client IP Error");

			auto pIOCPSock = FindEmptyIOCPSocket();

			if (!pIOCPSock)
			{
				closesocket(soc);

				server::log("CIOCP::Listen FindEmptyIOCPSocket() CurrentClientNumber[%d] ClientIP[%s]\r\n", m_nCurrentClientCounts, szAddress);
				
				pstInfo->bThreadUseFlag = false;

				continue;
			}

			pIOCPSock->SetSocket(soc);					// 소켓을 할당한다

			

			if (pIOCPSock->m_hSocket != INVALID_SOCKET)
			{
				BOOL bOption = TRUE;
				setsockopt(pIOCPSock->m_hSocket, IPPROTO_TCP, TCP_NODELAY, (char*)&bOption, sizeof(bOption));
			}

			auto hret = CreateIoCompletionPort((HANDLE)soc, m_hCompletionPort, (DWORD)pIOCPSock, 0);
			
			if (hret == NULL)
			{
				pIOCPSock->Close(0x10006);

				int nErr = GetLastError();
				SetLastError(0);

				server::log(true, "CIOCP::Listen CreateIoCompletionPort() LastError[%d] hCompletionPort[%p] pIOCPSock[%p]\r\n",
					nErr, m_hCompletionPort, pIOCPSock);

				pstInfo->bThreadUseFlag = FALSE;
				continue;
			}

			pIOCPSock->SetPeerAddress(szAddress, nPort);
			pIOCPSock->OnConnect();										// User Overloading function
			pIOCPSock->Read();
		}
		else
		{	
			if (m_bListeningFlag)
			{
				int nErr = GetLastError();
				SetLastError(0);

				server::log(true, "CIOCP::Listen WSAAccept Failure, LastError[%d]\r\n", nErr);
			}
			break;
		}

		pstInfo->bThreadUseFlag = false;
	}

	pstInfo->bThreadUseFlag = false;

	return 3;
}

DWORD CIOCP::Worker()
{
	char	szTemp1[1024];
	sprintf(szTemp1, "CIOCP::Worker_ WorkerThread, \t\t\t\t\t ThreadID(%6d, 0x%X)\r\n", GetCurrentThreadId(), GetCurrentThreadId());
	g_pGlobal->WriteSystemLog(szTemp1);
	DBGOUT(szTemp1);

	COverlapped		*pOverlapped;
	DWORD			dwRead;
	CIOCPSocket*	pIOCPSocket;
	BOOL			bRet;

	SThreadInfo *pstInfo;
	SThreadInfo	stInfo;
	memset(&stInfo, 0x00, sizeof(SThreadInfo));
	stInfo.dwThreadId = GetCurrentThreadId();
	stInfo.enThreadCheckType = THREAD_CHECK_TYPE_IOCP_WORKER;
	m_IOCPThreadCheck.AddThreadInfo(stInfo);

	while (true)
	{
		pstInfo = m_IOCPThreadCheck.GetThreadInfo(stInfo.dwThreadId);
		if (pstInfo) break;
		Sleep(100);
	}

	// todo : remove srand call
	srand(timeGetTime());			// Random Number를 위해서
	random::init();
	
	while (true)
	{
		pOverlapped = nullptr;
		dwRead = 0;
		pIOCPSocket = nullptr;
		bRet = GetQueuedCompletionStatus(m_hCompletionPort, &dwRead, PULONG_PTR(&pIOCPSocket), reinterpret_cast<LPOVERLAPPED*>(&pOverlapped), INFINITE);
		pstInfo->dwLastUseStartTick = timeGetTime();
		pstInfo->bThreadUseFlag = TRUE;
		if (!bRet)
		{
			int errCode = GetLastError();
			SetLastError(0);

			if (ERROR_PORT_UNREACHABLE == errCode && pIOCPSocket)
				
				server::log(true, "[ERROR] CIOCP::Worker() UDP ERROR_PORT_UNREACHABLE Error, SocketIndex[%3d] ==> Error[ERROR_PORT_UNREACHABLE]\r\n", pIOCPSocket->m_nClientArrayIndex);


			if (ERROR_SUCCESS == errCode && 0 == dwRead)
			{
				SAFE_DELETE_COverlapped(pOverlapped);
				if (pIOCPSocket) pIOCPSocket->Close(0x00003);
			}
			else if (!pOverlapped)
			{
				if (pIOCPSocket) pIOCPSocket->Close(0x00004);
			}
			else
			{	
				SAFE_DELETE_COverlapped(pOverlapped);
				if (pIOCPSocket) pIOCPSocket->Close(0x00005);
			}
			pstInfo->bThreadUseFlag = FALSE;
			continue;
		}


		if (!pIOCPSocket || !pOverlapped) break;

		if (dwRead == 0)
		{
			SAFE_DELETE_COverlapped(pOverlapped);
			pIOCPSocket->Close(0x00006);
			pstInfo->bThreadUseFlag = FALSE;
			continue;
		}

		switch (pOverlapped->GetOperationMode())
		{
		case COverlapped::EN_OPERATION_MODE_READ:
		{
			pIOCPSocket->OnReceive((char*)pOverlapped->GetWSABUFPtr()->buf, dwRead, (ENServerType)m_nServerType,
				inet_ntoa(pOverlapped->Getsocketaddr_inPtr()->sin_addr), ntohs(pOverlapped->Getsocketaddr_inPtr()->sin_port), pstInfo);
			pIOCPSocket->SetRecvOperationFlag(FALSE);
			pIOCPSocket->Read();
		}
		break;
		case COverlapped::EN_OPERATION_MODE_WRITE:
		{
			if (dwRead < pOverlapped->GetWSABUFPtr()->len)
			{
				g_pGlobal->WriteSystemLogEX(true, "CIOCP::Worker 패킷이 전송이 모두 안됨, wsaBuf.len[%d] dwRead[%d]\r\n",
					pOverlapped->GetWSABUFPtr()->len, dwRead);

				util::del(pOverlapped);
				pIOCPSocket->Close(0x10008);
			}
			else
			{
				// 2013-03-13 by hskim, 웹 캐시 상점 - RawData 전송 기능 추가
				if (TRUE == pOverlapped->GetSessionEnd())
				{
					util::del(pOverlapped);
					pIOCPSocket->Close(0x10010);
				}
				// end 2013-03-13 by hskim, 웹 캐시 상점 - RawData 전송 기능 추가
				else
				{
					pIOCPSocket->OnWrite(pOverlapped, dwRead);
					pIOCPSocket->Write(TRUE);
				}
			}
		}
		break;
		default:
		{
		}
		}
		pstInfo->bThreadUseFlag = FALSE;
	}
	pstInfo->bThreadUseFlag = FALSE;

	return 2;
}

SThreadInfo *CIOCP::CheckIOCPThread(DWORD i_dwThreadIdToExclude)
{
	return m_IOCPThreadCheck.CheckThreadInfo(i_dwThreadIdToExclude);
}

void CIOCP::SendMessageToMonitor(BYTE *pData, int nDataSize)
{
	m_MonitorIOCPSocketPtrVector.lock();

	for (auto x : m_MonitorIOCPSocketPtrVector)

		if (x->m_PeerSocketType == ST_MONITOR_SERVER) x->SendAddData(pData, nDataSize);

	m_MonitorIOCPSocketPtrVector.unlock();
}

void CIOCP::SendMessageToAdminTool(BYTE *pData, int nDataSize)
{
	m_MonitorIOCPSocketPtrVector.lock();

	for (auto x : m_MonitorIOCPSocketPtrVector)
		
		if (x->m_PeerSocketType == ST_ADMIN_TOOL) x->SendAddData(pData, nDataSize);

	m_MonitorIOCPSocketPtrVector.unlock();
}

BOOL CIOCP::InsertMonitorIOCPSocketPtr(CIOCPSocket *i_pSocket)
{
	m_MonitorIOCPSocketPtrVector.lock();
	m_MonitorIOCPSocketPtrVector.push_back(i_pSocket);
	m_MonitorIOCPSocketPtrVector.unlock();

	return TRUE;
}

BOOL CIOCP::DeleteMonitorIOCPSocketPtr(CIOCPSocket *i_pSocket)
{
	m_MonitorIOCPSocketPtrVector.lock();
	
	for (auto itr = m_MonitorIOCPSocketPtrVector.begin(); itr != m_MonitorIOCPSocketPtrVector.end(); ++itr)
	{
		if (i_pSocket == *itr)
		{
			m_MonitorIOCPSocketPtrVector.erase(itr);
			break;
		}
	}
	m_MonitorIOCPSocketPtrVector.unlock();

	return TRUE;
}
