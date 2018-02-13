#pragma once

//#include "SocketHeader.h"
#include "WinPacket.h"
#include <queue>
#include <winsock2.h>

using namespace std;

// socket HWND에서 받는 메시지(아래 메시지는 재정의해서 필요한 메시지로 바꿀 수 있다.)
#define	WM_USER_BASE				(WM_USER + 222)
#define	WM_ASYNC_EVENT				(WM_USER_BASE + 1)				//
#define	WM_PACKET_NOTIFY			(WM_USER_BASE + 2)				// HWND에 전달되는 메시지
#define WM_ASYNC_EVENT_BY_NPLAY		(WM_USER_BASE + 3)				// 2012-10-05 by jhseol, NPlay pc방 모듈
#define	WM_PACKET_NOTIFY_BY_NPLAY	(WM_USER_BASE + 4)				// 2012-10-05 by jhseol, NPlay pc방 모듈


class CWinSocket
{
public:
	enum
	{
		WS_ERROR = 0x0001,
		WS_CONNECTED = 0x0002,
		WS_ACCEPTED = 0x0003, // 2007-06-21 by cmkwon, 클라이언트의 소스와 통합
		WS_RECEIVED = 0x0004,
		WS_CLOSED = 0x0005
	};

	explicit CWinSocket(ENServerType PeerType, HWND hWnd, UINT wmSock = WM_ASYNC_EVENT, UINT wmNotify = WM_PACKET_NOTIFY, BOOL bEncoding = FALSE, char* szNPlayServerIP_Real = "", char* szNPlayServerIP_BackUp = "");// 2012-10-05 by jhseol, NPlay pc방 모듈 - NPlay 서버 IP 인자 추가
	virtual ~CWinSocket();

protected:
	HWND m_hMsgWnd; // Socket Event와 Notify 메시지 전송 윈도우
	SOCKET m_Socket; // 소켓의 핸들
	UINT m_wmSocket; // HWND에서 Socket 처리 메시지(WSAAsyncSelect함수 메지시)
	UINT m_wmNotify; // HWND에 통보되는 메시지(HWND에 소켓 처리결과 통보)
	BOOL m_bConnected; // 소켓이 remote와 연결되었는지의 플래그
	int m_nLastError; // 마지막 Error code
	int m_nAsyncFlag; // WSAAsyncSelect Event parameter
	BOOL m_bEncodingFlag;

	SOCKADDR_IN m_SockAddrLocal; // local을 위한 소켓 주소 구조체(Bind시에 필요)
	SOCKADDR_IN m_SockAddrRemote; // remote을 위한 소켓 주소 구조체(Accetp와 Connect시에 필요)
	list<CSendPacket*> m_listSendPacket; // Send Packet Pointer list
	CRITICAL_SECTION m_criticalSendList; // Send Packet List 의 동기화를 위해
	char m_RecvBuffer[SIZE_MAX_SOCKET_BUFFER ]; // Receive Buffer
	CRecvPacket m_RecvPacket; //
	queue<char*> m_queueRecvMessage; // Received Message를 저장해 놓을 큐
	CRITICAL_SECTION m_criticalRecvMessageQueue; // Received Message Queue의 동기화를 위해

	BYTE m_byHostSequenceNumber;
	BYTE m_byPeerSequenceNumber;
	BOOL m_bPeerSequenceNumberInitFlag; // 상대방의 받아야할 Sequence Number의 초기화 여부 플래그

	MessageType_t m_msgTyLastPacket; // 2008-02-22 by cmkwon, 마지막 패킷 메시지 타임

	// 2012-10-05 by jhseol, NPlay pc방 모듈
	char m_szNPlayServerIP_Real[SIZE_MAX_IPADDRESS ];
	char m_szNPlayServerIP_BackUp[SIZE_MAX_IPADDRESS ];
	// end 2012-10-05 by jhseol, NPlay pc방 모듈

public:
	int m_nPeerPort; // UDP 통신을 위한 상대방 Port
	char m_szPeerIP[SIZE_MAX_IPADDRESS ]; // TCP : 현재 연결된 Client의 IP Address,  UDP : 통신을 위한 상대방 IP Address
	ENServerType m_PeerSocketType;
	BOOL m_bSendNotify; // Send just one time Notify message 20060514 by dhkwon

protected:
	// Socket Misc. operation
	BOOL CreateSocket();
	inline void SetAsyncFlag(int nMode);
	inline BOOL IsError(int nError = 0);
	// Operations
public:
	inline BOOL AsyncSelect(int nMode);
	inline BOOL AsyncSelect();
	inline SOCKET GetSocketHandle(void);
	BOOL SetOptionWinSocket(int level, int nOption, char* pValue, int nValueSize); // 2007-06-21 by cmkwon, TCP Nagle 알고리즘 해제를 위해

	// 2007-12-21 by cmkwon, CWinSocket 에 블러킹 모드 구현 - 인자추가(BOOL i_bBlockingMode=FALSE)
	BOOL Connect(LPCSTR strPeerIP, int nPort, BOOL i_bBlockingMode = FALSE); // Client로서 동작, Server 로 연결시도
	BOOL Bind(int nPort); // Server로서 동작, 지정된 포트를 열어놓는다. Listen시에 필요
	BOOL Listen(int nBackLog = 1); // Server로서 동작, 접속하려는 Clinet를 기다린다.
	BOOL CloseSocket(int reason = 0); // Socket을 닫는다.

	BOOL Write(LPCSTR pPacket, int nLength, BOOL NPlaySocket = FALSE); // 2012-10-05 by jhseol, NPlay pc방 모듈 - 패킷의 해더 제거 여부인자 추가
	BOOL Write(BYTE* pPacket, int nLength) { return Write(LPCSTR(pPacket), nLength); }
	
	BOOL WriteMessageType(MessageType_t msgType);
	BOOL Read(LPSTR* pPacket, int& nLength);

	template<template<MessageType_t> typename data_t, MessageType_t msgtype>
	bool Write(const data_t<msgtype>& packet) { return this->Write(LPCSTR(&packet), packet.size()); }


	///////////////////////////////////////////////////////////////////////////////
	// 2007-12-21 by cmkwon, CWinSocket 에 블러킹 모드 구현 - 
	BOOL WriteBlockingMode(LPCSTR pPacket, int nLength, int i_nLimiteTimeInSec = -1); // 2007-12-21 by cmkwon, CWinSocket 에 블러킹 모드 구현 - CWinSocket::WriteBlockingMode() 추가
	BOOL ReadBlockingMode(LPSTR o_pBuffer, int nRLength, int i_nLimiteTimeInSec = -1); // 2007-12-21 by cmkwon, CWinSocket 에 블러킹 모드 구현 - CWinSocket::ReadBlockingMode() 추가

	void SendErrorMessage(MessageType_t msgType, Err_t err, int errParam1 = 0, int errParam2 = 0, char* errMsg = NULL, BOOL bCloseConnection = FALSE);

	int GetSendBufferCounts(void)
	{
		return m_listSendPacket.size();
	}

	int GetLastErrorCode(void); //
	BOOL IsConnected(void); //
	BOOL PostNotify(WORD nNotify, WORD nParam = 0, char* pData = NULL);
	BOOL SendNotify(WORD nNotify, WORD nParam = 0, char* pData = NULL);

public:
	// Message Operations
	LONG OnAsyncEvent(LONG lParam); // 실제 발생하는 Socket Event를 연결된 윈도우의 Handle로 불려질 Event handler
	BOOL OnAccept(int nErrorCode); // Accept가 된후에 발생
	BOOL OnReceive(int nErrorCode); // 읽을 데이타가 있을때 발생
	BOOL OnSendReady(int nErrorCode); // 소켓이 데이타 전송할수 있을때 발생
	BOOL OnOutOfBand(int nErrorCode); //

	virtual BOOL OnConnect(int nErrorCode); // 소켓이 연결된후에 발생
	virtual BOOL OnRecvdPacket(LPSTR pPacket, int nLength, BYTE nSeq); // 하나의 패킷이 도착했을 때 발생
	virtual BOOL OnCloseSocket(int nErrorCode); // 소켓이 닫히고 발생

	static BOOL m_bSocketInitFlag;
	static BOOL SocketInit(void);
	static void SocketClean(void);
};

SOCKET CWinSocket::GetSocketHandle(void)
{
	return m_Socket;
}

void CWinSocket::SetAsyncFlag(int nMode)
{
	m_nAsyncFlag = nMode;
}

BOOL CWinSocket::AsyncSelect()
{
	return AsyncSelect(m_nAsyncFlag);
}

BOOL CWinSocket::AsyncSelect(int nMode)
{
	if (INVALID_SOCKET == m_Socket || !IsWindow(m_hMsgWnd))
	{
		return FALSE;
	}
	if (SOCKET_ERROR == WSAAsyncSelect(m_Socket, m_hMsgWnd, nMode ? m_wmSocket : 0, nMode))
	{
		return !IsError();
	}

	return TRUE;
}

BOOL CWinSocket::IsError(int nError)
{
	if (0 == nError)
	{
		nError = WSAGetLastError();
		WSASetLastError(0);
	}
	//	DbgOut("Is Error(%d)\n", nError);
	switch (nError)
	{
	case WSAENOBUFS:
	case WSAEWOULDBLOCK:
	case WSAEFAULT:
	case 0:
		return FALSE;
	case WSAEACCES:
	case WSAEADDRINUSE:
	case WSAEADDRNOTAVAIL:
	case WSAEAFNOSUPPORT:
	case WSAEALREADY:
	case WSAECONNABORTED:
	case WSAECONNREFUSED:
	case WSAECONNRESET:
	case WSAEDESTADDRREQ:
	case WSAEHOSTDOWN:
	case WSAEHOSTUNREACH:
	case WSAEINPROGRESS:
	case WSAEINTR:
	case WSAEINVAL:
	case WSAEISCONN:
	case WSAEMFILE:
	case WSAEMSGSIZE:
	case WSAENETDOWN:
	case WSAENETRESET:
	case WSAENETUNREACH:
	case WSAENOPROTOOPT:
	case WSAENOTCONN:
	case WSAENOTSOCK:
	case WSAEOPNOTSUPP:
	case WSAEPFNOSUPPORT:
	case WSAEPROCLIM:
	case WSAEPROTONOSUPPORT:
	case WSAEPROTOTYPE:
	case WSAESHUTDOWN:
	case WSAESOCKTNOSUPPORT:
	case WSAETIMEDOUT:
	case WSATYPE_NOT_FOUND:
	case WSAHOST_NOT_FOUND:
	case WSA_INVALID_HANDLE:
	case WSA_INVALID_PARAMETER:
	case WSA_IO_INCOMPLETE:
	case WSA_IO_PENDING:
		//case WSA_NOT_ENOUGH_MEMORY:
	case WSANOTINITIALISED:
	case WSANO_DATA:
	case WSANO_RECOVERY:
	case WSAEPROVIDERFAILEDINIT:
	case WSASYSCALLFAILURE:
	case WSAEINVALIDPROCTABLE:
	case WSAEINVALIDPROVIDER:
	case WSASYSNOTREADY:
	case WSATRY_AGAIN:
	case WSAVERNOTSUPPORTED:
	case WSAEDISCON:
	case WSA_OPERATION_ABORTED:
		break;
	default:
		return FALSE;
	}
	m_nLastError = nError;
	CloseSocket(0x10003);
	return TRUE;
}

