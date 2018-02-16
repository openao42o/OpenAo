//Copyright[2002] MasangSoft
#pragma once

#include "mt_stl.h"
#include "WinPacket.h"
#include "Overlapped.h"
#include "ThreadCheck.h"
#include "MTCriticalSection.h"

#include "ProcList.h"

enum EN_NETWORK_STATE
{
    EN_NETWORK_STATE_WORST = 0,
    EN_NETWORK_STATE_BAD = 1,
    EN_NETWORK_STATE_NORMAL = 2,
    EN_NETWORK_STATE_GOOD = 3,
    EN_NETWORK_STATE_BEST = 4                // Network State 가 가장 좋은 상태
};

enum EN_PACKET_PRIORITY
{
    EN_PACKET_PRIORITY_LOWEST = 0,
    EN_PACKET_PRIORITY_LOW = 1,
    EN_PACKET_PRIORITY_NORMAL = 2,
    EN_PACKET_PRIORITY_HIGH = 3,
    EN_PACKET_PRIORITY_HIGHEST = 4                // 가장 높은 우선순위
};

struct STrafficInfo
{
    DWORD    dwBytesSend;
    DWORD    dwBytesRecv;
    DWORD    dwCountSendPacket;
    DWORD    dwCountRecvPacket;
    DWORD    dwTimeGapSecond;

    void Reset(DWORD currentTick)
    {
        dwBytesSend = 0;
        dwBytesRecv = 0;
        dwCountSendPacket = 0;
        dwCountRecvPacket = 0;

        dwTimeGapSecond = currentTick;
    }

    STrafficInfo& operator+=(STrafficInfo& value)
    {
        dwBytesSend += value.dwBytesSend;
        dwBytesRecv += value.dwBytesRecv;

        dwCountSendPacket += value.dwCountSendPacket;
        dwCountRecvPacket += value.dwCountRecvPacket;

        return *this;
    }
};

typedef mt_list<COverlapped*>    mtlistCOverlappedPtr;

///////////////////////////////////////////////////////////////////////////////
/// \class        CIOCPSocket
///
/// \brief        IOCPSocket 서버 소켓 최상위 클래스
/// \author        cmkwon
/// \version
/// \date        2004-03-17 ~ 2004-03-17
/// \warning
///////////////////////////////////////////////////////////////////////////////
class CIOCPSocket
{
    friend class CIOCP;
    friend class CFieldIOCP;
    friend class CNPCIOCP;

public:

    template<typename socket_t, T0 t0> ProcessResult CallHandlerT1(MessageType_t msgtype, const char* data, int size, int& offset)
    {
        using thisproclist = metaseries256::transform<proclist::get_handlersT1<socket_t, t0>>;

        auto handler = thisproclist::values[msgtype & 0xFF];

        if (handler) return handler(static_cast<socket_t*>(this), data, size, offset);

        return RES_PACKET_NA;
    }

    template<typename socket_t> ProcessResult CallHandlerT0(MessageType_t msgtype, const char* data, int size, int& offset)
    {
        using thisproclist = metaseries256::transform<proclist::get_handlersT0<socket_t>>;

        auto handler = thisproclist::values[msgtype >> 8];

        if (handler) return handler(static_cast<socket_t*>(this), msgtype, data, size, offset);

        return RES_PACKET_NA;
    }


    CIOCPSocket();

    virtual ~CIOCPSocket();

    ///////////////////////////////////////////////////////////////////////////
    // Property
    void SetClientArrayIndex(int i_nIndex) { m_nClientArrayIndex = i_nIndex; }
    int GetClientArrayIndex() const { return m_nClientArrayIndex; }
    BOOL SetSocket(SOCKET s);
    void SetRecvOperationFlag(BOOL bFlag) { m_bRecvOperationFlag = bFlag; }
    const char* GetPeerIP() const { return m_szPeerIP; };        // 연결된 상대방 IP Address를 리턴
    int GetPeerPort() const { return m_nPeerPort; };        // 연결된 상대방 Port를 리턴
    int GetPeerIP4() const { return inet_addr(m_szPeerIP); }    // 2009-11-04 by cmkwon, 태국 게임가드 Apex로 변경 - 연결된 상대방 IP Address를 4바이트로 리턴

    void InitIOCPSocket(int nIdx) { m_nClientArrayIndex = nIdx; }        // IOCPSocket 생성 후 필히 호출 해야함
    bool IsUsing() const { return m_bUsing; }                                    // CIOCPSocket의 인스턴스가 사용 중인지의 여부를 리턴
    void SetPeerAddress(char* pIP, int nPort);            // TCP:연결된 상대방 IP와 Port를 설정,  UDP:UDP 통신을 할 Client의 IP와 Port를 설정한다.
    void ClientCheck();
    void LockTrafficInfo() { EnterCriticalSection(&m_crtlTrafficInfo); }
    void UnlockTrafficInfo() { LeaveCriticalSection(&m_crtlTrafficInfo); }
    void OnRecvdAlivePacket();
    BOOL OpenUDPPort(int nPort, int nRetryCount = 0);

    void SetPeerUDPReady(bool bReadyFlag) { m_bPeerUDPReady = bReadyFlag; }
    bool GetPeerUDPReady() const { return m_bPeerUDPReady; }


    int Read();                                                // 소켓으로부터 데이타를 받기위해 버퍼를 준비, m_ovRecvBuf를 가지고 내부에서 처리
    int Write(BOOL bWaitFlag);                                            // 소켓으로 데이타를 전송한다. m_listWriteBuf 가지고 내부에서 처리
    void OnWrite(COverlapped *pOverlapped, int nWriteBytes);    // 전송이 완료된 COverlapped 구조체의 포인터를 인자로 받는다. TCP와 UDP에서 다른 방식으로 처리

    // 2008-03-13 by cmkwon, 대규모 전쟁시 클라이언트 팅기는거 수정 - 
    void CheckNetworkState();


    //int ReWrite(char * pData, int nLength);                    // 전송한 데이타가 모두 전송되지 않은경우 남은 데이타를 재전송한다. m_listWriteBuf 가지고 내부에서 처리
    void Close(int reason = 0, BOOL bDelayCloseFlag = FALSE, DWORD i_dwSleepTime = 0);    // 소켓을 닫는다.
    BOOL SendAddData(const BYTE* pData, int nSize, EN_PACKET_PRIORITY i_enPacketPriority = EN_PACKET_PRIORITY_HIGHEST, BOOL bRawSend = FALSE, BOOL bSessionEnd = FALSE);    // 2013-03-13 by hskim, 웹 캐시 상점 - RawData 전송 기능 추가 - // 소켓에 전송할 데이타를 설정한다.
    BOOL SendAddLongData(PBYTE pData, int nSize);                // 소켓에 전송할 데이타를 설정한다. (nSize > SIZE_MAX_PACKET)
    BOOL SendAddMessageType(MessageType_t msgType) { return SendAddData(PBYTE(&msgType), SIZE_FIELD_TYPE_HEADER); }                // Message Type만 전송한다.
    BOOL SendAddData(char* pData, int nSize, EN_PACKET_PRIORITY i_enPacketPriority = EN_PACKET_PRIORITY_HIGHEST) { return SendAddData((BYTE*)pData, nSize, i_enPacketPriority); }
    BOOL SendAddLongData(char* pData, int nSize) { return SendAddLongData(PBYTE(pData), nSize); }    // char* 전송용, 소켓에 전송할 데이타를 설정한다. (nSize > SIZE_MAX_PACKET)
    BOOL SendAddRawData(BYTE* pData, int nSize, BOOL bSessionEnd = FALSE) { return SendAddData(pData, nSize, EN_PACKET_PRIORITY_HIGHEST, TRUE, bSessionEnd); }    // 2013-03-13 by hskim, 웹 캐시 상점 - RawData 전송 기능 추가


    template<template<MessageType_t> typename data_t, MessageType_t msgtype>
    bool SendAddData(const data_t<msgtype>& packet) { return this->SendAddData(reinterpret_cast<const BYTE*>(&packet), packet.size()); }

    ////////////////////////////////////////////////////////////////////////////
    // virtual멤버함수
    // Data Receive 관련
    void OnReceive(char* pBlock, int length, ENServerType ServerType = ST_NORMAL_SERVER, char* pPeerIP = "", int nPeerPort = 0, SThreadInfo *i_pThreadInfo = NULL);
    
    
    // Return values matter only for TCP connections
    // true, packets were processed fine, keep socket alive
    // false, fetal error, close the socket!
    virtual BOOL OnRecvdPacket(const char* pPacket, int nLength, BYTE nSeq, char* pPeerIP = "", int nPeerPort = 0, SThreadInfo *i_pThreadInfo = NULL) = 0;

    // 접속 관련
    virtual void OnConnect();
    virtual void OnClose(int reason) { }

    // Error 관련
    virtual BOOL OnError(int errCode) { return false; }
    virtual void SendErrorMessage(MessageType_t msgType, Err_t err, int errParam1 = 0, int errParam2 = 0, const char* errMsg = NULL, BOOL bCloseConnection = FALSE) { }
    virtual void SendNetworkErrorMessage(int i_nWriteBufCounts, int i_nBadNetworkContinueTime) { }

    // 2008-03-13 by cmkwon, 대규모 전쟁시 클라이언트 팅기는거 수정 - 
    //virtual BOOL PreSendAddData(const BYTE *i_pbyData, int i_nSize, int i_nWriteBufCnts, vectSSendedOverlappedInfo *i_pSendedOverInfoList) { return true; }
    virtual void OnSendAddData(SSendedOverlappedInfo *i_pSendedOverInfo, int i_nSize, int i_nWriteBufCnts) { }

    // 2008-03-13 by cmkwon, 대규모 전쟁시 클라이언트 팅기는거 수정 - 
    void AddSendedOverlappedInfo(SSendedOverlappedInfo *i_pSendedOverInfo) { m_vectSendedOverlappedInfoList.push_back(*i_pSendedOverInfo); }
    void DeleteSendedOverlappedInfo(COverlapped *i_pOverlapped);

    static bool IsError(int errCode = 0);                        // 소켓 동작시 발생한 오류가 Error인지 여부를 리턴

protected:
    int                        m_nClientArrayIndex;                // IOCP에서 사용하는 Client 배열 인덱스
    SOCKET                    m_hSocket;                            // 소켓의 Handle
    bool                    m_bUsing;                            // IOCPSocket 객체가 사용 중인지의 플래그
    bool                    m_bFlagDelayClose;                    // 클라이언트에게 Close()를 실행하도록 메세지를 보냈다는 플래그로 일정시간동안 Close하지 안으면 강제 종료
    DWORD                    m_dwCountClientCheck;                // Alive Check를 위한 멤버 변수
    int                        m_nCloseReasonCode;                    // 종료시 종료 코드
    CMTCriticalSection        m_mtCritSecForClose;

    BOOL                    m_bRecvOperationFlag;                // Receive를 위한 버퍼 준비가 1번만 발생하도록 하기 위해(실제로는 필요가 없지만 잘못된 사용을 위해 처리
    COverlapped                m_ovRecvBuf;                        // Receive를 위한 Overlapped(static) 구조체 변수
    CRecvPacket                m_RecvPacket;                        // Received 데이타를 패킷단위로 나누기 위한 변수
    //CRecvHTTPPacket            m_RecvHTTPPacket;                // 2016 DevX    // 2013-03-13 by hskim, 웹 캐시 상점 - Received HTTP 데이타를 헤더와 바디로 나누기 위한 변수

    int                        m_nMaxWriteBufCounts;                // 최대 writeBuf counts
    int                        m_nCurrentWriteBufCounts;            // 현재 writeBuf counts
    int                        m_nBeforMaxWriteBufCountsAtCheckTime;    //
    DWORD                    m_dwTickLastCheckTimeNetwork;        // 1초에 한번 체크할 것임
    DWORD                    m_dwBadNetworkContinueTime;            // 상태가 좋지 않은 네트워크 지속 시간


    mtlistCOverlappedPtr    m_mtlistWriteBuf;                    // 전송하려는 Overlapped 구조체의 포인터를 저장할 list
    DWORD                    m_dwCountOverlappedSending;            // 소켓으로 보내지고 있는 COverlapped의 카운트

    BYTE                    m_byHostSequenceNumber;
    BYTE                    m_byPeerSequenceNumber;
    bool                    m_bPeerSequenceNumberInitFlag;        // 상대방의 받아야할 Sequence Number의 초기화 여부 플래그

    bool                    m_bUDPFlag;                            // ICOPSocket의 TCP와 UDP의 구분을 위한 플래그
    bool                    m_bPeerUDPReady;                    // UDP로 사용시 상대방의 포트가 준비 되어있는지의 플래그
    int                        m_nOpenedUDPPort;                    // Open된 UDP Port 번호
    int                        m_nPeerPort;                        // UDP 통신을 위한 상대방 Port
    char                    m_szPeerIP[SIZE_MAX_IPADDRESS];        // TCP : 현재 연결된 Client의 IP Address,  UDP : 통신을 위한 상대방 IP Address

    STrafficInfo            m_TrafficInfo;
    CRITICAL_SECTION        m_crtlTrafficInfo;

    bool                    m_bMustClose;                        // 2007-03-12 by cmkwon, 종료될 소켓 플레그

    MessageType_t            m_LastRecvedMsgType;                // 2008-03-06 by cmkwon, IOCPSocket 에 마지막 메시지 타입 체크 시스템 추가 - 

    vectSSendedOverlappedInfo    m_vectSendedOverlappedInfoList;    // 2008-03-13 by cmkwon, 대규모 전쟁시 클라이언트 팅기는거 수정 - 

    static CIOCP*            ms_pIOCP;                            // IOCP 인스턴스 포인터

public:
    ENServerType            m_PeerSocketType;
};

using mtvectorIOCPSocket = mt_vector<CIOCPSocket*>;


// macro    DECLARE_MESSAGE_AND_CHECK_SIZE
// brief    CXXXIOCPSocket::Process_XXX() 함수 내에서 초기에 메세지 선언 및 사이즈 확인 매크로
// author    kelovon
// date        2004-04-09
// warning    CXXXIOCPSocket::Process_XXX() 함수 내에서만 사용해야 함!
// 2009-06-12 by cmkwon, 수정함 - 에러 정보 추가
#define DECLARE_MESSAGE_AND_CHECK_SIZE(__PPACKET, __NLENGTH, __NBYTESUSED, __MSGTYPE, __MSGSTRUCT, __PMESSAGEVAR)    \
    INT            __NRECVTYPESIZE    = sizeof(__MSGSTRUCT);                    \
    __MSGSTRUCT    *__PMESSAGEVAR    = nullptr;                                \
    if (__NLENGTH - __NBYTESUSED < __NRECVTYPESIZE)                        \
    {                                                                    \
        SendErrorMessage(__MSGTYPE, ERR_PROTOCOL_INVALID_FIELD_DATA, __NLENGTH - __NBYTESUSED, __NRECVTYPESIZE);    \
        return RES_RETURN_FALSE;                                        \
    }                                                                    \
    __PMESSAGEVAR = (__MSGSTRUCT*)(__PPACKET+__NBYTESUSED);                \
    __NBYTESUSED += __NRECVTYPESIZE;

// macro    DECLARE_MESSAGE_AND_CHECK_SIZE_SERVER - 서버간의 메시지 교환에서만 사용
// brief    CXXXIOCPSocket::Process_XXX() 함수 내에서 초기에 메세지 선언 및 사이즈 확인 매크로
// author    kelovon
// date        2004-06-02
// warning    CXXXIOCPSocket::Process_XXX() 함수 내에서만 사용해야 함!
#define DECLARE_MESSAGE_AND_CHECK_SIZE_SERVER(__PPACKET, __NLENGTH, __NBYTESUSED, __MSGTYPE, __MSGSTRUCT, __PMESSAGEVAR)    \
    INT            __NRECVTYPESIZE    = 0;                                    \
    __MSGSTRUCT    *__PMESSAGEVAR    = NULL;                                    \
    __NRECVTYPESIZE = sizeof(__MSGSTRUCT);                                \
    if (__NLENGTH - __NBYTESUSED < __NRECVTYPESIZE)                        \
    {                                                                    \
        SendErrorMessage(__MSGTYPE, ERR_PROTOCOL_INVALID_FIELD_DATA);    \
        return RES_PACKET_ERROR;                                        \
    }                                                                    \
    __PMESSAGEVAR = (__MSGSTRUCT*)(__PPACKET+__NBYTESUSED);                \
    __NBYTESUSED += __NRECVTYPESIZE;
