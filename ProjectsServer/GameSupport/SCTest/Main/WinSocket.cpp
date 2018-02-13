// WinSocket.cpp: implementation of the CWinSocket class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "WinSocket.h"
//#include "AtumProtocol.h"
#ifndef util::del
#define util::del(p) { if(NULL != p){ delete (p); (p) = NULL;}}
#endif

#ifndef util::del_array
#define util::del_array(p) { if(NULL != p){ delete[] (p); (p) = NULL;}}
#endif

#ifndef DBGOUT
#define DBGOUT (void(0))
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWinSocket::CWinSocket(HWND hWnd, UINT wmSock, UINT wmNotify, BOOL bEncoding)
{
	m_hMsgWnd			= hWnd;
	m_Socket			= INVALID_SOCKET;
	m_bConnected		= FALSE;
	m_nLastError		= 0;
	m_wmSocket			= wmSock;
	m_wmNotify			= wmNotify;
	m_bEncodingFlag		= bEncoding;
	m_bPeerSequenceNumberInitFlag = FALSE;
	m_PeerSocketType	= ST_INVALID_TYPE;
	memset(m_szPeerIP, 0x00, SIZE_MAX_IPADDRESS);

	InitializeCriticalSection(&m_criticalSendList);
	InitializeCriticalSection(&m_criticalRecvMessageQueue);

	// 자체 생성
	m_nAsyncFlag = 0;
	m_bSendNotify = FALSE;

	CreateSocket();
}

CWinSocket::~CWinSocket()
{
	CloseSocket(0x00000);
	EnterCriticalSection(&m_criticalSendList);
	CSendPacket *pSendPacket = NULL;
	while(m_listSendPacket.empty() == false)
	{
		pSendPacket = m_listSendPacket.front();
		util::del(pSendPacket);
		m_listSendPacket.pop_front();
	}
	LeaveCriticalSection(&m_criticalSendList);
	DeleteCriticalSection(&m_criticalSendList);

	EnterCriticalSection(&m_criticalRecvMessageQueue);
	char *pData = NULL;
	while(m_queueRecvMessage.empty() == false)
	{
		pData = m_queueRecvMessage.front();
		util::del_array(pData);
		m_queueRecvMessage.pop();
	}
	LeaveCriticalSection(&m_criticalRecvMessageQueue);
	DeleteCriticalSection(&m_criticalRecvMessageQueue);
}


int CWinSocket::GetLastErrorCode(void)
{
	return m_nLastError;
}

BOOL CWinSocket::IsConnected()
{
	return	m_bConnected;
}

BOOL CWinSocket::CreateSocket()
{
	if(!IsWindow(m_hMsgWnd)){ return FALSE;}		// Socket Event와 Notify에 사용할 윈도우 핸들이 유효한지 체크

	if(INVALID_SOCKET == m_Socket)
	{
		m_Socket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_IP, NULL, 0, 0);
		if(INVALID_SOCKET == m_Socket){ return !IsError();}
	}
	return	TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CWinSocket::SetOptionWinSocket(int level, int nOption, char* pValue, int nValueSize)
/// \brief		// 2007-06-21 by cmkwon, TCP Nagle 알고리즘 해제를 위해
/// \author		cmkwon
/// \date		2007-06-21 ~ 2007-06-21
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CWinSocket::SetOptionWinSocket(int level, int nOption, char* pValue, int nValueSize)
{
	if (INVALID_SOCKET == m_Socket){			return FALSE;}
    return	!setsockopt(m_Socket, level, nOption, pValue, nValueSize);
}

BOOL CWinSocket::Connect(LPCSTR strPeerIP, int nPort)
{
	if (INVALID_SOCKET == m_Socket && !CreateSocket()){ return FALSE;}

	m_byHostSequenceNumber = (BYTE)(GetTickCount()%SEQNO_VAR_C);								// 초기값을 SEQNO_VAR_C 값보다 작게 설정
	m_bPeerSequenceNumberInitFlag = FALSE;
	if(m_byHostSequenceNumber == 116)
	{
		m_byHostSequenceNumber++;
	}
	m_SockAddrRemote.sin_port = htons(nPort);
	DWORD dwPeerAddress = inet_addr(strPeerIP);
	if (INADDR_NONE != dwPeerAddress)
	{
	    m_SockAddrRemote.sin_addr.s_addr = dwPeerAddress;
		m_SockAddrRemote.sin_family = AF_INET;

		if(!AsyncSelect(FD_CONNECT) ||	SOCKET_ERROR != connect(m_Socket, (SOCKADDR *)&m_SockAddrRemote, sizeof(m_SockAddrRemote)))
		{
			return !IsError();
		}
		WSASetLastError(0);
		return	TRUE;
	}

	return FALSE;
}

BOOL CWinSocket::Bind(int nPort)
{
	if (INVALID_SOCKET == m_Socket && !CreateSocket()){ return FALSE;}
    int	nLen = sizeof(m_SockAddrLocal);

	m_SockAddrLocal.sin_port = htons(nPort);
    m_SockAddrLocal.sin_family = AF_INET;
    m_SockAddrLocal.sin_addr.s_addr = INADDR_ANY;

	if (SOCKET_ERROR == bind(m_Socket, (LPSOCKADDR)&m_SockAddrLocal, nLen))
	{
		return !IsError();
	}

	return TRUE;
}

BOOL CWinSocket::Listen(int nBackLog)
{
	if (INVALID_SOCKET == m_Socket && !CreateSocket()){ return FALSE;}
	if (!AsyncSelect(FD_ACCEPT) || SOCKET_ERROR == listen(m_Socket, nBackLog))
	{
		return !IsError();
	}
	return	TRUE;
}

BOOL CWinSocket::CloseSocket(int reason)
{
	m_bConnected = FALSE;
	m_RecvPacket.Init();
	EnterCriticalSection(&m_criticalSendList);
	CSendPacket *pSendPacket = NULL;
	while(m_listSendPacket.empty() == false)
	{
		pSendPacket = m_listSendPacket.front();
		util::del(pSendPacket);
		m_listSendPacket.pop_front();
	}
	LeaveCriticalSection(&m_criticalSendList);

	EnterCriticalSection(&m_criticalRecvMessageQueue);
	char *pData = NULL;
	while(m_queueRecvMessage.empty() == false)
	{
		pData = m_queueRecvMessage.front();
		util::del_array(pData);
		m_queueRecvMessage.pop();
	}
	LeaveCriticalSection(&m_criticalRecvMessageQueue);

	if (INVALID_SOCKET != m_Socket)
	{
		DWORD	dw = 1L;
		if (GetLastError() == 0)
		{
			AsyncSelect(0);
		}
		ioctlsocket(m_Socket, FIONBIO, &dw);
		closesocket(m_Socket);
		m_Socket = INVALID_SOCKET;
	}

	m_bPeerSequenceNumberInitFlag = FALSE;
	SetAsyncFlag(0);

	return	TRUE;
}

BOOL CWinSocket::PostNotify(WORD nNotify, WORD nParam, char* pData)
{
	if(m_wmNotify)
	{
		PostMessage(m_hMsgWnd, m_wmNotify, MAKEWPARAM(nNotify, nParam), (LPARAM)pData);
	}
	return TRUE;
}

BOOL CWinSocket::SendNotify(WORD nNotify, WORD nParam, char* pData)
{
	if(m_wmNotify)
	{
		SendMessage(m_hMsgWnd, m_wmNotify, MAKEWPARAM(nNotify, nParam), (LPARAM)pData);
	}
	return TRUE;
}

LONG CWinSocket::OnAsyncEvent(LONG lParam)
{
	int nErrorCode = WSAGETSELECTERROR(lParam);
	switch (WSAGETSELECTEVENT(lParam))
	{
	case FD_CONNECT:
		OnConnect(nErrorCode);
		break;
	case FD_ACCEPT:
		OnAccept(nErrorCode);
		break;
	case FD_CLOSE:
		OnCloseSocket(nErrorCode);
		break;
	case FD_OOB:
		OnOutOfBand(nErrorCode);
		break;
	case FD_READ:
		OnReceive(nErrorCode);
		break;
	case FD_WRITE:
		OnSendReady(nErrorCode);
	}
	return	0;
}

BOOL CWinSocket::OnConnect(int nErrorCode)
{
	if(INVALID_SOCKET == m_Socket || (nErrorCode && IsError(nErrorCode)) == TRUE)
	{
		SendNotify(WS_CONNECTED, FALSE, (char*)this);
		return FALSE;
	}

	// 2007-06-21 by cmkwon, TCP Nagle 알고리즘 해제를 위해 추가
	BOOL bOption = TRUE;
	this->SetOptionWinSocket(IPPROTO_TCP, TCP_NODELAY, (char*)&bOption, sizeof(BOOL));

	SetAsyncFlag(FD_READ | FD_WRITE | FD_OOB | FD_CLOSE);
	AsyncSelect();											// Socket Event를 설정한다.

	m_bConnected = TRUE;

	strncpy(m_szPeerIP, inet_ntoa(m_SockAddrRemote.sin_addr), SIZE_MAX_IPADDRESS);
	m_nPeerPort = ntohs(m_SockAddrRemote.sin_port);

	// cmkwon Socket이 연결된후 처리 코드 삽입 부분
	// App에 Connected 정보를 Post한다.
	SendNotify(WS_CONNECTED, TRUE, (char*)this);
	return	TRUE;
}

BOOL CWinSocket::OnAccept(int nErrorCode)
{
	if(IsError(nErrorCode) == TRUE || INVALID_SOCKET == m_Socket){ return FALSE;}

	// cmkwon Socket이 Accept 된후에 처리 코드 삽입 부분
	// App에 Accepted 정보를 Send한다.
	SendNotify(WS_ACCEPTED, 0, (char*)this);		// 2007-06-21 by cmkwon, 클라이언트의 소스와 통합
	return	TRUE;
}

BOOL CWinSocket::OnCloseSocket(int nErrorCode)
{
	CloseSocket(0x00001);

	m_PeerSocketType	= ST_INVALID_TYPE;

	// cmkwon Socket이 Close 된후에 처리 코드 삽입 부분
	// App에 Closed 정보를 Send한다.
	SendNotify(WS_CLOSED, 0, (char*)this);
	return	TRUE;
}

BOOL CWinSocket::OnOutOfBand(int nErrorCode)
{
	return !IsError(nErrorCode);
}

BOOL CWinSocket::OnReceive(int nErrorCode)
{
	WSABUF	wsaBuf;
	DWORD	nBytesRecvd		= 0;
	DWORD	nBytesTotalUsed	= 0;
	int		nBytesUsed		= 0;
	DWORD	dwFlag			= 0;
	int		nRet			= 0;

	if(INVALID_SOCKET == m_Socket || (nErrorCode && IsError(nErrorCode) == TRUE)){ return FALSE;}
	wsaBuf.buf = m_RecvBuffer;
	wsaBuf.len = SIZE_MAX_SOCKET_BUFFER;

	nRet = WSARecv(m_Socket, &wsaBuf, 1, &nBytesRecvd, &dwFlag, NULL, NULL);
	if(SOCKET_ERROR == nRet)
	{
		int nErr = WSAGetLastError();
		WSASetLastError(0);
		if(IsError(nErr))
		{
			return FALSE;
		}
		AsyncSelect();
		return TRUE;
	}

	while(nBytesRecvd > 0)
	{
		nRet = m_RecvPacket.AddTail(m_RecvBuffer + nBytesTotalUsed, nBytesRecvd, &nBytesUsed);
		if(nRet < 0)					// size 헤더가 SIZE_MAX_PACKET(1500 bytes)보다 크다, 오류 발생 소켓을 닫는다.
		{

			CloseSocket(0x10000);
			return FALSE;
		}
		nBytesTotalUsed += nBytesUsed;
		nBytesRecvd -= nBytesUsed;
		nRet = FALSE;
		if(m_RecvPacket.RecvCompleted() == TRUE)
		{
			if(m_RecvPacket.IsPacketEncoded() == FALSE
				|| m_RecvPacket.IsValidPacket() == TRUE)
			{
// 2005-09-07 by cmkwon, 테스트
//				MessageType_t ty = *(MessageType_t*)m_RecvPacket.GetPacket();
//				DBGOUT("		Recv : %s(0x%X)\n", GetProtocolTypeString(ty), ty);
				
				nRet = OnRecvdPacket(m_RecvPacket.GetPacket(), m_RecvPacket.GetLength(), m_RecvPacket.GetSequenceNumber());
			}
			else
			{	// Encoded Packet이지만 유효하지 않은 패킷이다.
				CloseSocket(0x10001);

				char	szSystemLog[256];
				sprintf(szSystemLog, "[Error] CWinSocket::OnReceive invalid packet !!\r\n");
				//GWriteSystemLog(szSystemLog);
				DBGOUT(szSystemLog);
				return FALSE;
			}

			if(nRet == FALSE)
			{	// Protocol Error
				CloseSocket(0x10002);

				char	szSystemLog[256];
				sprintf(szSystemLog, "[Error] CWinSocket::OnReceive Error!!\r\n");
				//GWriteSystemLog(szSystemLog);
				DBGOUT(szSystemLog);
				return FALSE;
			}
			m_RecvPacket.Init();
		}
	}
	return TRUE;
}

int CWinSocket::OnRecvdPacket(LPSTR pPacket, int nLength, BYTE nSeq)
{
	return FALSE;
}

BOOL CWinSocket::OnSendReady(int nErrorCode)
{
	WSABUF	wsaBuf;
	DWORD	sendBytes	= 0;
	DWORD	dwFlag		= 0;
	int		nRet		= 0;
	CSendPacket *pSendPacket = NULL;

	if (nErrorCode && IsError(nErrorCode) == TRUE){ return FALSE;}

	EnterCriticalSection(&m_criticalSendList);
	while(m_listSendPacket.empty() == false)
	{
		pSendPacket = NULL;
		pSendPacket = *m_listSendPacket.begin();
		wsaBuf.len = pSendPacket->GetLength();
		wsaBuf.buf = pSendPacket->GetPacket();
		nRet = WSASend(m_Socket, &wsaBuf, 1, &sendBytes, dwFlag, NULL, NULL);
//		int nType = 0;
//		memcpy(&nType, pSendPacket->GetPacket()+SIZE_PACKET_HEADER,SIZE_FIELD_TYPE_HEADER);
//		DbgOut("!!!! WSASend reqLen(%4d) sendedLen(%4d) Type(%s)\n", wsaBuf.len, sendBytes, GetProtocolTypeString(nType));
		if(0 != nRet)
		{
//			DbgOut("!!!! WSASend Error Type(%s) nRet(%d)\n", GetProtocolTypeString(nType), nRet);
			int nErr = WSAGetLastError();
			WSASetLastError(0);
			if(IsError(nErr) == TRUE)
			{
//				DbgOut("!!!! IsError\n");
				LeaveCriticalSection(&m_criticalSendList);
				return FALSE;
			}

			break;
		}
		else
		{
			util::del (pSendPacket);
			m_listSendPacket.pop_front();
		}
	}
	LeaveCriticalSection(&m_criticalSendList);

	return	TRUE;
}

int CWinSocket::Read(LPSTR *pPacket, int &nLength)
{
	*pPacket	= NULL;
	nLength		= 0;						// 의미 없음
	EnterCriticalSection(&m_criticalRecvMessageQueue);
	if(m_queueRecvMessage.empty() == true)
	{
		LeaveCriticalSection(&m_criticalRecvMessageQueue);
		return nLength;
	}
	*pPacket = m_queueRecvMessage.front();
	m_queueRecvMessage.pop();
	if(m_queueRecvMessage.empty() == true)
	{
		m_bSendNotify = FALSE;
	}
	LeaveCriticalSection(&m_criticalRecvMessageQueue);
	return TRUE;
}

BOOL CWinSocket::WriteMessageType(MessageType_t msgType)
{
	return Write((LPCSTR)&msgType, SIZE_FIELD_TYPE_HEADER);
}

BOOL CWinSocket::Write(LPCSTR pPacket, int nLength)
{
	WSABUF	wsaBuf;
	DWORD	sendBytes	= 0;
	DWORD	dwFlag		= 0;
	int		nRet		= 0;
	int		tmpSeq;
	CSendPacket *pSendPacket = NULL;

	if(INVALID_SOCKET == m_Socket || !m_bConnected){ return FALSE;}
// 2005-09-07 by cmkwon, 테스트
//	MessageType_t ty = *(MessageType_t*)pPacket;
//	DBGOUT("	Send : %s(0x%X)\n", GetProtocolTypeString(ty), ty);
	pSendPacket = new CSendPacket((char*)pPacket, nLength, m_bEncodingFlag, m_byHostSequenceNumber);
	tmpSeq = (m_byHostSequenceNumber + SEQNO_VAR_A) * SEQNO_VAR_B;
	if(tmpSeq > SEQNO_VAR_C)
	{
		tmpSeq = tmpSeq % SEQNO_VAR_C;
	}
	m_byHostSequenceNumber = ++tmpSeq;
	EnterCriticalSection(&m_criticalSendList);
	if(m_listSendPacket.empty() == true)
	{
		wsaBuf.len = pSendPacket->GetLength();
		wsaBuf.buf = pSendPacket->GetPacket();
		nRet = WSASend(m_Socket, &wsaBuf, 1, &sendBytes, dwFlag, NULL, NULL);
//		int nType = 0;
//		memcpy(&nType, pPacket,SIZE_FIELD_TYPE_HEADER);
//		DbgOut("WSASend reqLen(%4d) sendedLen(%4d) nLength(%4d) Type(%s)\n", wsaBuf.len, sendBytes, nLength, GetProtocolTypeString(nType));
		if(0 != nRet)
		{
//			DbgOut("WSASend Error Type(%s) nRet(%d)\n", GetProtocolTypeString(nType), nRet);
			int nErr = WSAGetLastError();
			WSASetLastError(0);
			if(IsError(nErr) == TRUE)			// 에러가 발생하면 소켓이 닫히게 됨, 그러므로 전송하려면 CSendPacket은 delete해야함
			{
				util::del(pSendPacket);
				LeaveCriticalSection(&m_criticalSendList);
				return FALSE;
			}
			else
			{
				m_listSendPacket.push_back(pSendPacket);
				AsyncSelect();
			}
		}
		else
		{
			util::del(pSendPacket);
		}
		LeaveCriticalSection(&m_criticalSendList);
		return TRUE;
	}
//	else
//	{
//		int nType = 0;
//		memcpy(&nType, pPacket,SIZE_FIELD_TYPE_HEADER);
//		DbgOut("NOT Send(m_listSendPacket == Not Empty Type(%s)\n", GetProtocolTypeString(nType));
//	}
	m_listSendPacket.push_back(pSendPacket);
	LeaveCriticalSection(&m_criticalSendList);

	return OnSendReady(0);
}


BOOL CWinSocket::m_bSocketInitFlag = FALSE;
BOOL CWinSocket::SocketInit(void)
{
	if(m_bSocketInitFlag)
		return m_bSocketInitFlag;

	srand( (unsigned)timeGetTime() );			// Random Number를 위해서
	// todo : deprecate
	random::init();
	WSADATA wsaData;

	WORD wVersionRequested = MAKEWORD(2, 2);
	int nResult = WSAStartup(wVersionRequested, &wsaData);
	if (nResult != 0)
		return FALSE;

	if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2)
	{
		WSACleanup();
		return FALSE;
	}

	m_bSocketInitFlag = TRUE;
	return m_bSocketInitFlag;
}
void CWinSocket::SocketClean(void)
{
	if(!m_bSocketInitFlag)
		return;

	WSACleanup();
}

// error 메세지를 전송
void CWinSocket::SendErrorMessage(MessageType_t msgType, Err_t err, int errParam1, int errParam2, char* errMsg, BOOL bCloseConnection)
{
//	MSG_ERROR	*pMsgError;
//	char		SendBuf[SIZE_MAX_PACKET];
//
//	char szSystemLog[256];
//	sprintf(szSystemLog, "  SendErrorMsg to %15s(%4d) ==> %s(%#04x) AT %s Param1(%d) Param2(%d) Msg(%s)\r\n",
//		m_szPeerIP, m_nPeerPort,
//		GetErrorString(err), err, GetProtocolTypeString(msgType), errParam1, errParam2, errMsg);
//	DBGOUT(szSystemLog);
//
//	*(MessageType_t*)SendBuf = T_ERROR;
//	pMsgError = (MSG_ERROR*)(SendBuf+SIZE_FIELD_TYPE_HEADER);
//	pMsgError->CloseConnection = bCloseConnection;
//	pMsgError->ErrorCode = err;
//	pMsgError->MsgType = msgType;
//	pMsgError->ErrParam1 = errParam1;
//	pMsgError->ErrParam2 = errParam2;
//	if (errMsg == NULL)
//	{
//		pMsgError->StringLength = 0;
//	}
//	else
//	{
//		pMsgError->StringLength = (USHORT)(strlen(errMsg) + 1);
//		strncpy((char*)SendBuf + MSG_SIZE(MSG_ERROR), errMsg, SIZE_MAX_ERROR_STRING);
//	}
//	Write(SendBuf, MSG_SIZE(MSG_ERROR) + pMsgError->StringLength);
}