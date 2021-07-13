//Copyright[2002] MasangSoft
///////////////////////////////////////////////////////////////////////////////
//  IOCPSocket.cpp :
//
//  Date    : 2004-03-17 by cmkwon
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <timeapi.h>
#include "IOCPSocket.h"
#include "IOCP.h"
#include "MTAutoCriticalSection.h"

CIOCP* CIOCPSocket::ms_pIOCP = nullptr;                            // IOCP 인스턴스 포인터

CIOCPSocket::CIOCPSocket() :
    m_nClientArrayIndex { },
    m_hSocket { INVALID_SOCKET },
    m_bUsing { FALSE },
    m_bFlagDelayClose { FALSE },
    m_dwCountClientCheck { 0 },
    m_nCloseReasonCode { 0 },
    m_bRecvOperationFlag { FALSE },
    m_ovRecvBuf { COverlapped::EN_OPERATION_MODE_READ, COverlapped::EN_OVERLAPPED_TYPE_STATIC },
    m_nMaxWriteBufCounts { 0 },
    m_nCurrentWriteBufCounts { 0 },
    m_nBeforMaxWriteBufCountsAtCheckTime { 0 },
    m_dwTickLastCheckTimeNetwork { 0 },
    m_dwBadNetworkContinueTime { 0 },
    m_mtlistWriteBuf { },
    m_dwCountOverlappedSending { 0 },
    m_byHostSequenceNumber { },
    m_byPeerSequenceNumber { },
    m_bPeerSequenceNumberInitFlag { FALSE },
    m_bUDPFlag { FALSE },
    m_bPeerUDPReady { FALSE },
    m_nOpenedUDPPort { 0 },
    m_nPeerPort { 0 },
    m_szPeerIP { },
    m_TrafficInfo { },
    m_crtlTrafficInfo {  },
    m_bMustClose { FALSE },
    m_LastRecvedMsgType { 0 },
    m_vectSendedOverlappedInfoList { },
    m_PeerSocketType { ST_INVALID_TYPE }
{
    InitializeCriticalSection(&m_crtlTrafficInfo);
}

CIOCPSocket::~CIOCPSocket()
{
    if (IsUsing()) Close(0x00000);

    m_mtlistWriteBuf.lock();
    for (auto ptr : m_mtlistWriteBuf) delete ptr;
    m_mtlistWriteBuf.clear();
    m_mtlistWriteBuf.unlock();

    DeleteCriticalSection(&m_crtlTrafficInfo);
}

// Listener로 부터 생성된 소켓을 CIOCPSocket 인스턴스와 연결한다.
BOOL CIOCPSocket::SetSocket(SOCKET s)
{
    if (m_bUsing || m_bUDPFlag) return false;
    
    m_hSocket = s;
    m_bUsing = true;

    return true;
}

int CIOCPSocket::Read()
{
    if (!m_bUsing || m_hSocket == INVALID_SOCKET || m_bRecvOperationFlag) return 0;

    DWORD    dwRecvBytes = 0;
    DWORD    dwFlag = 0;

    m_bRecvOperationFlag = TRUE;                    // Overlapped 동작 플래그를 TRUE로 변경
    m_ovRecvBuf.ResetOverlapped();

    auto nRet = m_bUDPFlag ?

        WSARecvFrom(m_hSocket, m_ovRecvBuf.GetWSABUFPtr(), 1, &dwRecvBytes, &dwFlag,
        (sockaddr*)m_ovRecvBuf.Getsocketaddr_inPtr(), m_ovRecvBuf.GetSizesocketaddr_inPtr(), &m_ovRecvBuf, nullptr) :

        WSARecv(m_hSocket, m_ovRecvBuf.GetWSABUFPtr(), 1, &dwRecvBytes, &dwFlag, &m_ovRecvBuf, nullptr);

    if (nRet != 0)
    {
        int err = GetLastError();
        SetLastError(0);
        if (IsError(err))
        {
            m_bRecvOperationFlag = false;
            Close(0x10000);
            char szError[512];
            sprintf(szError, "CIOCPSocket::Read WSARecv(), LastError[%d] Index[%d]\r\n", err, m_nClientArrayIndex);
            g_pGlobal->WriteSystemLog(szError);
            DBGOUT(szError);
            return 0;
        }
    }

    return dwRecvBytes;
}

int CIOCPSocket::Write(BOOL bWaitFlag)
{
    DWORD dwSendBytes = 0;
    DWORD dwFlag = 0;
    int nRet;

    if (!IsUsing() || m_mtlistWriteBuf.empty()) return 0;

    if (m_dwCountOverlappedSending > 3) return 0;

    m_mtlistWriteBuf.lock();    // 2008-03-25 by cmkwon, bWaitFlag 와 상관없이 무조건 lock() 을 잡아야 할 것 같음

    if (m_mtlistWriteBuf.empty())
    {
        m_mtlistWriteBuf.unlock();
        return 0;
    }

    COverlapped* pOver = *m_mtlistWriteBuf.begin();
    m_mtlistWriteBuf.pop_front();

    ///////////////////////////////////////////////////////////////////////////////
    // 2008-03-13 by cmkwon, 대규모 전쟁시 클라이언트 팅기는거 수정 - pOver 관련 정보를 삭제한다.
    DeleteSendedOverlappedInfo(pOver);

    ///////////////////////////////////////////////////////////////////////
    // TCP 전송
    if (!m_bUDPFlag)
    {
        if (COverlapped::ENDataType::EN_DATA_TYPE_ACEONLINE == pOver->GetDataType())        // 2013-03-13 by hskim, 웹 캐시 상점 - RawData 전송 기능 추가
        {
            pOver->EncodePacket(m_byHostSequenceNumber);

            int tmpSeq;
            tmpSeq = (m_byHostSequenceNumber + SEQNO_VAR_A) * SEQNO_VAR_B;
            if (tmpSeq > SEQNO_VAR_C)
            {
                tmpSeq = tmpSeq % SEQNO_VAR_C;
            }
            m_byHostSequenceNumber = ++tmpSeq;
        }

        InterlockedIncrement((LPLONG)&m_dwCountOverlappedSending);
        nRet = WSASend(m_hSocket, pOver->GetWSABUFPtr(), 1, &dwSendBytes, dwFlag, pOver, NULL);
        if (0 != nRet)
        {
            // 10054 An existing connection was forcibly closed by the remote host. WSAECONNRESET
            // 10054
            auto err = WSAGetLastError();
            WSASetLastError(0);
            if (IsError(err) == TRUE)
            {
                util::del(pOver);
                m_mtlistWriteBuf.unlock();
                InterlockedDecrement((LPLONG)&m_dwCountOverlappedSending);
                this->Close(0x10001);

                char    szError[1024];
                sprintf(szError, "CIOCPSocket::Write1 WSASend(), LastError[%d] Index[%d]\r\n"
                    , err, m_nClientArrayIndex);
                g_pGlobal->WriteSystemLog(szError);
                DBGOUT(szError);
                return FALSE;
            }
        }
        m_mtlistWriteBuf.unlock();
        return dwSendBytes;
    }

    ///////////////////////////////////////////////////////////////////////
    // UDP 전송
    if (strncmp(m_szPeerIP, "", SIZE_MAX_IPADDRESS) == 0
        || 0 == m_nPeerPort
        || this->GetPeerUDPReady() == FALSE)
    {
        util::del(pOver);
        m_mtlistWriteBuf.unlock();
        return FALSE;
    }

    InterlockedIncrement((LPLONG)&m_dwCountOverlappedSending);
    pOver->Getsocketaddr_inPtr()->sin_family = AF_INET;
    pOver->Getsocketaddr_inPtr()->sin_addr.s_addr = inet_addr(m_szPeerIP);
    pOver->Getsocketaddr_inPtr()->sin_port = htons(m_nPeerPort);
    nRet = WSASendTo(m_hSocket, pOver->GetWSABUFPtr(), 1, &dwSendBytes, dwFlag, (sockaddr*)pOver->Getsocketaddr_inPtr()
        , pOver->GetSizesocketaddr_in(), pOver, NULL);

    if (0 != nRet)
    {
        // 10054
        int err = WSAGetLastError();
        WSASetLastError(0);
        if (IsError(err) == TRUE)
        {
            util::del(pOver);
            m_mtlistWriteBuf.unlock();
            InterlockedDecrement((LPLONG)&m_dwCountOverlappedSending);
            this->Close(0x10002);

            char    szError[1024];
            sprintf(szError, "CIOCPSocket::Write2 WSASend(), LastError[%d] Index[%d]\r\n"
                , err, m_nClientArrayIndex);
            g_pGlobal->WriteSystemLog(szError);
            DBGOUT(szError);
            return FALSE;
        }
    }
    m_mtlistWriteBuf.unlock();
    return TRUE;
}

// 전송이 완료된 COverlapped 구조체의 포인터를 인자로 받는다. TCP와 UDP에서 다른 방식으로 처리
void CIOCPSocket::OnWrite(COverlapped *pOverlapped, int nWriteBytes)
{
    InterlockedDecrement((LPLONG)&m_dwCountOverlappedSending);
    delete pOverlapped;

    if (ms_pIOCP->GetFlagCalcTrafficInfo())
    {
        LockTrafficInfo();
        m_TrafficInfo.dwBytesSend += nWriteBytes;
        m_TrafficInfo.dwCountSendPacket++;
        UnlockTrafficInfo();
    }
}

// 2008-03-13 by cmkwon, 대규모 전쟁시 클라이언트 팅기는거 수정 - CIOCPSocket::CheckNetworkState() 추가
void CIOCPSocket::CheckNetworkState()
{
    auto dwCur = timeGetTime();

    if (dwCur - m_dwTickLastCheckTimeNetwork < 3000) return;

    if (m_nCurrentWriteBufCounts > 30)
    {
        m_dwBadNetworkContinueTime += dwCur - m_dwTickLastCheckTimeNetwork;
        if (m_nBeforMaxWriteBufCountsAtCheckTime < m_nMaxWriteBufCounts || 50 < m_nCurrentWriteBufCounts)
        {
            // 2004-11-13 by cmkwon, m_dwTickLastCheckTimeNetwork를 Update하고 호출해야한다(그렇지 않은면 루프가 발생한다)
            m_dwTickLastCheckTimeNetwork = dwCur;
            this->SendNetworkErrorMessage(m_nCurrentWriteBufCounts, m_dwBadNetworkContinueTime);
        }
    }
    else m_dwBadNetworkContinueTime = 0;

    m_nBeforMaxWriteBufCountsAtCheckTime = m_nMaxWriteBufCounts;
    m_dwTickLastCheckTimeNetwork = dwCur;
}

void CIOCPSocket::Close(int reason/*=0*/, BOOL bDelayCloseFlag/*=FALSE*/, DWORD i_dwSleepTime/*=0*/)
{
    if (i_dwSleepTime > 0)
    {
        i_dwSleepTime = min(10000, i_dwSleepTime);
        Sleep(i_dwSleepTime);
    }

    CMTAutoCriticalSection mtAuto(m_mtCritSecForClose.GetCriticalSectionPtr());
    if (!m_bUsing) return;

    m_bUsing = false;
    m_bFlagDelayClose = false;
    m_dwCountClientCheck = 0;

    if (m_hSocket != INVALID_SOCKET)
    {
        shutdown(m_hSocket, SD_SEND);
        closesocket(m_hSocket);
    }

    m_hSocket = INVALID_SOCKET;

    OnClose(reason);

    ///////////////////////////////////////////////////////////////////////////
    // 전송을 위한 list를 초기화 한다.
    m_nMaxWriteBufCounts = 0;
    m_nCurrentWriteBufCounts = 0;
    m_mtlistWriteBuf.lock();
    for (auto ptr : m_mtlistWriteBuf) delete ptr;
    m_mtlistWriteBuf.clear();
    m_vectSendedOverlappedInfoList.clear();        // 2008-03-13 by cmkwon, 대규모 전쟁시 클라이언트 팅기는거 수정 - 
    m_mtlistWriteBuf.unlock();
    m_bRecvOperationFlag = FALSE;
    m_ovRecvBuf.ResetOverlapped();
    m_RecvPacket.Init();
    //m_RecvHTTPPacket.Init();        // 2013-03-13 by hskim, 웹 캐시 상점

    if (ms_pIOCP->GetFlagCalcTrafficInfo())
    {
        LockTrafficInfo();
        if (m_bUDPFlag == TRUE) ms_pIOCP->UpdateUDPTrafficInfo(&m_TrafficInfo);
        else ms_pIOCP->UpdateTCPTrafficInfo(&m_TrafficInfo);
        memset(&m_TrafficInfo, 0x00, sizeof(STrafficInfo));
        UnlockTrafficInfo();
    }

    m_bUDPFlag = FALSE;
    SetPeerUDPReady(FALSE);
    m_nOpenedUDPPort = 0;
    m_nPeerPort = 0;
    memset(m_szPeerIP, 0x00, sizeof(m_szPeerIP));
    m_bPeerSequenceNumberInitFlag = FALSE;
    m_bMustClose = FALSE;            // 2007-03-12 by cmkwon

    ms_pIOCP->PushClientIndex(m_nClientArrayIndex);
}


BOOL CIOCPSocket::SendAddData(const BYTE* pData, int nSize, EN_PACKET_PRIORITY i_enPacketPriority, BOOL bRawSend, BOOL bSessionEnd)    
{
    if (!m_bUsing || m_bFlagDelayClose || (m_bUDPFlag && !GetPeerUDPReady()) || m_bMustClose)

        return false;

    if (ms_pIOCP->GetIOCPNetworkState() > i_enPacketPriority) return true;


    {
        mt_auto_lock auto_lock { m_mtlistWriteBuf };

        auto bEncoding = !m_bUDPFlag && !bRawSend;

        auto DataType = bRawSend ? COverlapped::ENDataType::EN_DATA_TYPE_RAW : COverlapped::ENDataType::EN_DATA_TYPE_ACEONLINE;

        SSendedOverlappedInfo tmSendedOverInfo;

        util::zero(tmSendedOverInfo);

        if (m_mtlistWriteBuf.empty())
        {
            auto pOver = new COverlapped { COverlapped::EN_OPERATION_MODE_WRITE, COverlapped::EN_OVERLAPPED_TYPE_DYNAMIC, bEncoding, DataType };

            if (!pOver || !pOver->AddData(&tmSendedOverInfo, pData, nSize))
            {
                util::del(pOver);
                return false;
            }

            pOver->SetSessionEnd(bSessionEnd);

            m_mtlistWriteBuf.push_back(pOver);
        }
        else
        {
            auto pOver = m_mtlistWriteBuf.back();

            if (!pOver->AddData(&tmSendedOverInfo, pData, nSize))
            {
                pOver = new COverlapped { COverlapped::EN_OPERATION_MODE_WRITE, COverlapped::EN_OVERLAPPED_TYPE_DYNAMIC, bEncoding, DataType };

                if (!pOver || !pOver->AddData(&tmSendedOverInfo, pData, nSize))
                {
                    util::del(pOver);
                    return false;
                }

                pOver->SetSessionEnd(bSessionEnd);

                m_mtlistWriteBuf.push_back(pOver);
            }
        }

        if (!bRawSend) OnSendAddData(&tmSendedOverInfo, nSize, m_mtlistWriteBuf.size());

        m_nCurrentWriteBufCounts = m_mtlistWriteBuf.size() + m_dwCountOverlappedSending;
        m_nMaxWriteBufCounts = max(m_nMaxWriteBufCounts, m_nCurrentWriteBufCounts);
    }

    CheckNetworkState();

    return true;
}

BOOL CIOCPSocket::SendAddLongData(PBYTE pData, int nSize)
{
    for (auto offset = 0; offset < nSize;) // btw there is currently a proposal for "while (statement; expression)" and "if (statement; expression)"
    {
        auto size = min(nSize - offset, SIZE_MAX_PACKET);

        if (!SendAddData(pData + offset, size)) return false;
        
        offset += size;
    }

    return true;
}

void CIOCPSocket::DeleteSendedOverlappedInfo(COverlapped *i_pOverlapped)
{
    if (m_vectSendedOverlappedInfoList.empty()) return;

    auto itr = m_vectSendedOverlappedInfoList.begin();
    while (itr != m_vectSendedOverlappedInfoList.end())
    {
        if (itr->pOverlapped == i_pOverlapped)
        {
            itr = m_vectSendedOverlappedInfoList.erase(itr);
            continue;
        }
        ++itr;
    }
}

void CIOCPSocket::OnReceive(char* pBlock, int length, ENServerType ServerType, char* pPeerIP, int nPeerPort, SThreadInfo *i_pThreadInfo)
{
    if (!IsUsing() || m_bFlagDelayClose) return;

    if (ms_pIOCP->GetFlagCalcTrafficInfo())
    {
        LockTrafficInfo();
        m_TrafficInfo.dwBytesRecv += length;
        m_TrafficInfo.dwCountRecvPacket++;
        UnlockTrafficInfo();
    }

    int        nBytesRecvd = length;
    int        nBytesTotalUsed = 0;
    int        nBytesUsed = 0;
    BYTE    bySeq = 0;

    if (m_bUDPFlag)
    {
        bySeq = *PBYTE(pBlock + SIZE_BODY_LENGTH + SIZE_ENCODE_FLAG);
        
        // Masang don't use the return value for UDP
        OnRecvdPacket(pBlock + SIZE_PACKET_HEADER, length - SIZE_PACKET_HEADER, bySeq, pPeerIP, nPeerPort, i_pThreadInfo);

        return;
    }

    while (IsUsing() && !m_bFlagDelayClose && nBytesRecvd > 0)
    {
        {
            auto bRet = m_RecvPacket.AddTail(pBlock + nBytesTotalUsed, nBytesRecvd, &nBytesUsed);
            
            if (bRet < 0)
            {
                Close(0x01000);
                return;
            }
        }

        nBytesTotalUsed += nBytesUsed;
        nBytesRecvd -= nBytesUsed;
        
        
        if (m_RecvPacket.RecvCompleted())
        {
            bool bRet;

            if (!m_RecvPacket.IsPacketEncoded())
            {
                char    szSystemLog1[256];
                sprintf(szSystemLog1, "[TEMP] Not Encoded Packet IP:[%s] command[%4d]\r\n", pPeerIP, *(DWORD*)m_RecvPacket.GetPacket());
                DBGOUT(szSystemLog1);

                bRet = OnRecvdPacket(m_RecvPacket.GetPacket(), m_RecvPacket.GetLength(), m_byPeerSequenceNumber, NULL, 0, i_pThreadInfo);
            }

            else if (m_RecvPacket.IsValidPacket())
                
                bRet = OnRecvdPacket(m_RecvPacket.GetPacket(), m_RecvPacket.GetLength(), m_RecvPacket.GetSequenceNumber(), NULL, 0, i_pThreadInfo);

            else
            {
                // Encoded Packet not valid
                Close(0x10001, TRUE);

                server::log(true, "[Error] CIOCPSocket::OnReceive invalid packet, SocketIndex[%d]\r\n", m_nClientArrayIndex);
                
                return;
            }

            if (bRet == FALSE)
            {
                // close connection
                //Close(0x10001, TRUE);

                server::log(true, "[Error] CIOCPSocket::OnReceive OnRecvdPacket! Closing Connection!, SocketIndex[%d]\r\n", m_nClientArrayIndex);

                return;
            }

            m_RecvPacket.Init();
        }
    }
}

void CIOCPSocket::OnConnect()
{
    m_nMaxWriteBufCounts = 0;
    m_nCurrentWriteBufCounts = 0;
    m_nBeforMaxWriteBufCountsAtCheckTime = 0;
    m_dwBadNetworkContinueTime = 0;
    m_dwTickLastCheckTimeNetwork = 0;

    m_dwCountOverlappedSending = 0;
    m_RecvPacket.Init();
    //m_RecvHTTPPacket.Init();        // 2013-03-13 by hskim, 웹 캐시 상점
    m_dwCountClientCheck = 0;
    m_byHostSequenceNumber = RANDI(SEQNO_VAR_C);                                // TCP 초기값을 SEQNO_VAR_C 값보다 작게 설정
    if (m_byHostSequenceNumber == 116) m_byHostSequenceNumber++;
    m_bPeerSequenceNumberInitFlag = FALSE;

    m_LastRecvedMsgType = 0;        // 2008-03-06 by cmkwon, IOCPSocket 에 마지막 메시지 타입 체크 시스템 추가 - 

    mt_auto_lock autolock { m_mtlistWriteBuf };
    m_vectSendedOverlappedInfoList.clear();
}

void CIOCPSocket::SetPeerAddress(char* pIP, int nPort)
{
    util::strncpy(m_szPeerIP, pIP, SIZE_MAX_IPADDRESS);
    m_nPeerPort = nPort;
}


bool CIOCPSocket::IsError(int errCode)
{
    if (errCode == 0)
    {
        errCode = WSAGetLastError();
        WSASetLastError(0);
    }

    switch (errCode)
    {
    case 0:
    case WSA_IO_INCOMPLETE:            // 996L Overlapped I/O event is not in a signaled state.
    case WSA_IO_PENDING:            // 997L Overlapped I/O operation is in progress.
        break;
    case WSA_INVALID_HANDLE:        // 6L The handle is invalid.
    case WSA_NOT_ENOUGH_MEMORY:        // 8L Not enough storage is available to process this command.
    case ERROR_NETNAME_DELETED:        // 64L The specified network name is no longer available.
    case WSA_INVALID_PARAMETER:        // 87L The parameter is incorrect.
    case WSA_OPERATION_ABORTED:        // 995L The I/O operation has been aborted because of either a thread exit or an application request.
    case WSAEINTR:                    // 10004 A blocking operation was interrupted by a call to WSACancelBlockingCall.
    case WSAEBADF:                    // 10009 The file handle supplied is not valid.
    case WSAEACCES:                    // 10013 An attempt was made to access a socket in a way forbidden by its access permissions.
    case WSAEFAULT:                    // 10014 The system detected an invalid pointer address in attempting to use a pointer argument in a call.
    case WSAEINVAL:                    // 10022 An invalid argument was supplied.
    case WSAEMFILE:                    // 10024 Too many open sockets.
    case WSAEWOULDBLOCK:            // 10035 A non-blocking socket operation could not be completed immediately.
    case WSAEINPROGRESS:            // 10036 A blocking operation is currently executing.
    case WSAEALREADY:                // 10037 An operation was attempted on a non-blocking socket that already had an operation in progress.
    case WSAENOTSOCK:                // 10038 An operation was attempted on something that is not a socket.
    case WSAEDESTADDRREQ:            // 10039 A required address was omitted from an operation on a socket.
    case WSAEMSGSIZE:                // 10040 A message sent on a datagram socket was larger than the internal message buffer or some other network limit, or the buffer used to receive a datagram into was smaller than the datagram itself.
    case WSAEPROTOTYPE:                // 10041 A protocol was specified in the socket function call that does not support the semantics of the socket type requested.
    case WSAENOPROTOOPT:            // 10042 An unknown, invalid, or unsupported option or level was specified in a getsockopt or setsockopt call.
    case WSAEPROTONOSUPPORT:        // 10043 The requested protocol has not been configured into the system, or no implementation for it exists.
    case WSAESOCKTNOSUPPORT:        // 10044 The support for the specified socket type does not exist in this address family.
    case WSAEOPNOTSUPP:                // 10045 The attempted operation is not supported for the type of object referenced.
    case WSAEPFNOSUPPORT:            // 10046 The protocol family has not been configured into the system or no implementation for it exists.
    case WSAEAFNOSUPPORT:            // 10047 An address incompatible with the requested protocol was used.
    case WSAEADDRINUSE:                // 10048 Only one usage of each socket address (protocol/network address/port) is normally permitted.
    case WSAEADDRNOTAVAIL:            // 10049 The requested address is not valid in its context.
    case WSAENETDOWN:                // 10050 A socket operation encountered a dead network.
    case WSAENETUNREACH:            // 10051 A socket operation was attempted to an unreachable network.
    case WSAENETRESET:                // 10052 The connection has been broken due to keep-alive activity detecting a failure while the operation was in progress.
    case WSAECONNABORTED:            // 10053 An established connection was aborted by the software in your host machine.
    case WSAECONNRESET:                // 10054 An existing connection was forcibly closed by the remote host.
    case WSAENOBUFS:                // 10055 An operation on a socket could not be performed because the system lacked sufficient buffer space or because a queue was full.
    case WSAEISCONN:                // 10056 A connect request was made on an already connected socket.
    case WSAENOTCONN:                // 10057 A request to send or receive data was disallowed because the socket is not connected and (when sending on a datagram socket using a sendto call) no address was supplied.
    case WSAESHUTDOWN:                // 10058 A request to send or receive data was disallowed because the socket had already been shut down in that direction with a previous shutdown call.
    case WSAETOOMANYREFS:            // 10059 Too many references to some kernel object.
    case WSAETIMEDOUT:                // 10060 A connection attempt failed because the connected party did not properly respond after a period of time, or established connection failed because connected host has failed to respond.
    case WSAECONNREFUSED:            // 10061 No connection could be made because the target machine actively refused it.
    case WSAELOOP:                    // 10062 Cannot translate name.
    case WSAENAMETOOLONG:            // 10063 Name component or name was too long.
    case WSAEHOSTDOWN:                // 10064 A socket operation failed because the destination host was down.
    case WSAEHOSTUNREACH:            // 10065 A socket operation was attempted to an unreachable host.
    case WSAENOTEMPTY:                // 10066 Cannot remove a directory that is not empty.
    case WSAEPROCLIM:                // 10067 A Windows Sockets implementation may have a limit on the number of applications that may use it simultaneously.
    case WSAEUSERS:                    // 10068 Ran out of quota.
    case WSAEDQUOT:                    // 10069 Ran out of disk quota.
    case WSAESTALE:                    // 10070 File handle reference is no longer available.
    case WSAEREMOTE:                // 10071 Item is not available locally.
    case WSASYSNOTREADY:            // 10091 WSAStartup cannot function at this time because the underlying system it uses to provide network services is currently unavailable.
    case WSAVERNOTSUPPORTED:        // 10092 The Windows Sockets version requested is not supported.
    case WSANOTINITIALISED:            // 10093 Either the application has not called WSAStartup, or WSAStartup failed.
    case WSAEDISCON:                // 10101 Returned by WSARecv or WSARecvFrom to indicate the remote party has initiated a graceful shutdown sequence.
    case WSAENOMORE:                // 10102 No more results can be returned by WSALookupServiceNext.
    case WSAECANCELLED:                // 10103 A call to WSALookupServiceEnd was made while this call was still processing. The call has been canceled.
    case WSAEINVALIDPROCTABLE:        // 10104 The procedure call table is invalid.
    case WSAEINVALIDPROVIDER:        // 10105 The requested service provider is invalid.
    case WSAEPROVIDERFAILEDINIT:    // 10106 The requested service provider could not be loaded or initialized.
    case WSASYSCALLFAILURE:            // 10107 A system call that should never fail has failed.
    case WSASERVICE_NOT_FOUND:        // 10108 No such service is known. The service cannot be found in the specified name space.
    case WSATYPE_NOT_FOUND:            // 10109 The specified class was not found.
    case WSA_E_NO_MORE:                // 10110 No more results can be returned by WSALookupServiceNext.
    case WSA_E_CANCELLED:            // 10111 A call to WSALookupServiceEnd was made while this call was still processing. The call has been canceled.
    case WSAEREFUSED:                // 10112 A database query failed because it was actively refused.
    case WSAHOST_NOT_FOUND:            // 11001 No such host is known.
    case WSATRY_AGAIN:                // 11002 This is usually a temporary error during hostname resolution and means that the local server did not receive a response from an authoritative server.
    case WSANO_RECOVERY:            // 11003 A non-recoverable error occurred during a database lookup.
    case WSANO_DATA:                // 11004 The requested name is valid, but no data of the requested type was found.
        return TRUE;
    }
    return FALSE;
}

BOOL CIOCPSocket::OpenUDPPort(int nPort, int nRetryCount)
{
    if (m_bUsing || m_hSocket != INVALID_SOCKET) return false;

    m_hSocket = WSASocket(AF_INET, SOCK_DGRAM, IPPROTO_IP, NULL, 0, WSA_FLAG_OVERLAPPED);
    
    if (m_hSocket == INVALID_SOCKET) return false;

    m_dwCountOverlappedSending = 0;

    do
    {
        SOCKADDR_IN udp_addr;
        util::zero(udp_addr);
        udp_addr.sin_port = htons(u_short(nPort));
        udp_addr.sin_family = AF_INET;
        udp_addr.sin_addr.s_addr = INADDR_ANY;

        if (SOCKET_ERROR == ::bind(m_hSocket, LPSOCKADDR(&udp_addr), sizeof(udp_addr))) nPort += 1;

        else
        {
            m_bUsing = true;
            m_bUDPFlag = true;

            m_nOpenedUDPPort = nPort;

            return true; // 2016-08-12 panoskj
        }

        nRetryCount--;
	} while (nRetryCount >= 0);


    closesocket(m_hSocket);

    m_hSocket = INVALID_SOCKET;

    return false;
}


void CIOCPSocket::ClientCheck()
{
    m_dwCountClientCheck++;
    if (m_dwCountClientCheck > 5) Close(0x10007);
}


void CIOCPSocket::OnRecvdAlivePacket()
{
    if (!m_bFlagDelayClose) m_dwCountClientCheck = 0;
}
