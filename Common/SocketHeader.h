#pragma once

#include "CompileUtils.h"

#include <WinSock2.h>
#include <Mmsystem.h>

#pragma comment(lib, "Ws2_32.lib")

#ifdef SIZE_MAX_IPADDRESS
#undef SIZE_MAX_IPADDRESS
#endif
#ifdef SIZE_MAX_PACKET
#undef SIZE_MAX_PACKET
#endif

constexpr auto SIZE_MAX_IPADDRESS = 16;		// string IP Address
constexpr auto SIZE_TCP_LAN_HEADER = 58;	// 58 Bytes = 18(Datalink) + 20(IP) + 20(TCP)
constexpr auto SIZE_UDP_LAN_HEADER = 46;	// 46 Bytes = 18(Datalink) + 20(IP) + 8(UDP)

static_assert(sizeof(unsigned short) == 2, "The following code assumes unsigned short is 2 bytes long.");

// Packet Header 관련 define

using MessageType_t = unsigned short;		// 패킷 타입을 위해
using Err_t = unsigned short;				// 에러 타입을 위해

constexpr auto SIZE_PACKET_HEADER = 4;		// 각 패킷간 사이즈 헤더 사이즈
constexpr auto SIZE_BODY_LENGTH = 2;		// 각 패킷간 사이즈 헤더 사이즈
constexpr auto SIZE_ENCODE_FLAG = 1;		// 암호화 정보 flag 사이즈
constexpr auto SIZE_CHECKSUM = 1;			// checksum의 byte 수
constexpr auto SIZE_SEQ_NUM = 1;			// sequence number의 byte 수
constexpr auto SIZE_MAX_DUMMY_DATA = 3;		// dummy data의 최대 길이
constexpr auto SIZE_FIELD_TYPE_HEADER = sizeof(MessageType_t);		// 하나의 패킷은 여러개의 필드로 구성된다. 각 필드의 타입 헤더 사이즈
constexpr auto SIZE_MAX_PACKET = 1492;		// 패킷 최대 사이즈(여러개의 메세지가 하나의 패킷으로 전송될수 있음)
constexpr auto SIZE_MAX_SOCKET_BUFFER = 1500;	// Receve Buffer 최대 사이즈

constexpr auto COUNT_MAX_SOCKET_SESSION = 1600;			// 서버에서 지원할 최대 접속 클라이언트수
constexpr auto SIZE_MAX_PRESERVER_SESSION = 1500;		// Pre Server에서 지원할 최대 접속 클라이언트수, COUNT_MAX_SOCKET_SESSION 이하
constexpr auto SIZE_MAX_FIELDSERVER_SESSION = 1500;		// Field Server에서 지원할 최대 접속 클라이언트수, COUNT_MAX_SOCKET_SESSION 이하
constexpr auto SIZE_MAX_IMSERVER_SESSION = 1500;		// IM Server에서 지원할 최대 접속 클라이언트수, COUNT_MAX_SOCKET_SESSION 이하
constexpr auto SIZE_MAX_AUTHSERVER_SESSION = 200;		// Authentication Server에서 지원할 최대 접속 클라이언트수, COUNT_MAX_SOCKET_SESSION 이하 // 2011-01-26 by hskim, 인증 서버 구현
														// 2010-12-03 by jskim 변경 UDP 채널수 170 -> 200으로 변경

constexpr auto SIZE_MAX_FIELDWEBSERVER_SESSION = 400;	// 2013-03-13 by hskim, 웹 캐시 상점 - Field Web Server에서 지원할 최대 접속 클라이언트수, COUNT_MAX_SOCKET_SESSION 이하

constexpr auto SIZE_MAX_UDP_CHANNEL_SESSION = 200;									// 2008-08-29 by cmkwon, FieldServ<->NPCServ 가 UDP 통신 할 채널수 - 서비스맵보다 많아야 한다.													
constexpr auto SIZE_MAX_NPCSERVER_SESSION = SIZE_MAX_UDP_CHANNEL_SESSION + 10;		// NPC Server에서 지원할 최대맵의 수, COUNT_MAX_SOCKET_SESSION 이하
constexpr auto SIZE_MAX_LOGSERVER_SESSION = 100;									// Log 서버에서 지원할 최대 접속 서버수, COUNT_MAX_SOCKET_SESSION 이하

template<typename _MSG_STRUCT> constexpr auto _msg_size() { return SIZE_FIELD_TYPE_HEADER + sizeof(_MSG_STRUCT); }

template<MessageType_t _MSG_TYPE, typename buffer_t> void _set_msg_type(buffer_t* _BUFFER)
{
	*(MessageType_t*)(_BUFFER) = _MSG_TYPE;
}

template<typename _MSG_STRUCT, MessageType_t _MSG_TYPE, typename buffer_t> auto _init_msg(buffer_t* _BUFFER)
{
	static_assert(sizeof(buffer_t) == 1, "_BUFFER must be a pointer to a single byte.");

	memset(_BUFFER, 0x00, _msg_size<_MSG_STRUCT>());
	if (_MSG_TYPE != 0) _set_msg_type<_MSG_TYPE>(_BUFFER);
	return (_MSG_STRUCT*)(_BUFFER + SIZE_FIELD_TYPE_HEADER /*sizeof(MessageType_t)*/);
}

#define MSG_SIZE(_MSG_STRUCT) _msg_size<_MSG_STRUCT>()
#define SET_MSG_TYPE(_BUFFER, _MSG_TYPE) _set_msg_type<_MSG_TYPE>(_BUFFER);

///////////////////////////////////////////////////////////////////////////////
// Declare Variable, Set Message Buffer & Type
//		_MSG_STRUCT	: 메세지 구조체 타입
//		_VAR_NAME	: 변수 이름
//		_BUFFER		: 버퍼
//		_MSG_TYPE	: 메세지 타입
///////////////////////////////////////////////////////////////////////////////
#define INIT_MSG(_MSG_STRUCT, _MSG_TYPE, _VAR_NAME, _BUFFER)				\
	auto _VAR_NAME = _init_msg<_MSG_STRUCT, _MSG_TYPE>(_BUFFER);

#define INIT_MSG_WITH_BUFFER(_MSG_STRUCT, _MSG_TYPE, _VAR_NAME, _BUFFER)	\
	BYTE _BUFFER[SIZE_MAX_PACKET];											\
	auto _VAR_NAME = _init_msg<_MSG_STRUCT, _MSG_TYPE>(_BUFFER);

#define INIT_MSG_OF_SIZE(_MSG_STRUCT, _MSG_TYPE, _VAR_NAME, _BUFFER)		\
	BYTE _BUFFER[MSG_SIZE(_MSG_STRUCT)];									\
	auto _VAR_NAME = _init_msg<_MSG_STRUCT, _MSG_TYPE>(_BUFFER);

///////////////////////////////////////////////////////////////////////////////
// Encode/Decode Protocol
/*/////////////////////////////////////////////////////////////////////////////

<------------------- 4B --------------->
-----------------------------------------------------------------------
| size | encode_flag +   N    | seq. # | MSGs ... | checksum |  dummy  |
| (2B) |   (1bit)    + (7bit) |  (1B)  |          |   (1B)   |  (0~3B) |
-----------------------------------------------------------------------
<- XOR -><------- XOR --------->

* encoding rule 1
- Sequence Number 생성
(1)
첫 SeqNumber := Random 함수로 생성된 Number (0~127)

(2)
NewSeqNumber := ( SeqNumber + A ) * B
if ( NewSeqNumber > C ) {
NextSeqNumber := NewSeqNumber % C
} else {
NextSeqNumber := SeqNumber
}

* encoding rule 2
- Dummy Data 생성 및 packet 뒤부분에 추가
Dummy Data의 길이는 0~3B = (seq. #) % 4, 내용은 임의로 결정

* encoding rule 3
- checksum 생성 및 추가

* encoding rule 4
- XOR_DATA[128]을 미리 생성해둠.
- (seq. #)는 XOR_DATA[N-1]과 xor 연산을 함
- (MSGs + checksum)는 XOR_DATA[N]부터 XOR_DATA[127]까지와 xor 연산을 함
XOR_DATA[127] 다음에는 XOR_DATA[0]부터 다시 시작함
- N 값은 효율성을 위해 항상 4의 배수, N값의 범위는 4, 8, ... , 124

* invalid packet의 조건들
- sequence number가 맞지 않음
- checksum이 맞지 않음
- dummy data의 길이가 맞지 않으면, checksum이 틀리거나, MSG가 맞지 않게 됨
- XOR_DATA와 연산을 한 후의 MSGs가 protocol과 맞지 않음

/////////////////////////////////////////////////////////////////////////////*/

constexpr auto ENCODE_MASK = 0x80;	// 10000000
constexpr auto XOR_N_MASK = 0x7F;	// 01111111

constexpr auto SEQNO_VAR_A = 1;
constexpr auto SEQNO_VAR_B = 2;
constexpr auto SEQNO_VAR_C = 119;	// MAX Sequence Number

constexpr char XOR_ENCODE_BYTES[] = "fewoiroqbfweotui29854f09qwe0213hrf0a89wq0re902149dujaosdjfapwetu2fadq1234fsacdfzdxczfsdgbhtrytrgw563fwsjkpqertgvxhteertw3512ga\0";
constexpr auto SIZE_XOR_ENCODE_BYTES = carrlen(XOR_ENCODE_BYTES);

// sequence number는 connection당 하나를 관리해야 하기 때문에, IOCPSocket에서 관리한다.
struct DECODING_INFO
{
	BYTE nSeqNumber;
	BYTE nXOR_N;
	BOOL bIsPacketEncoded;	// default: FALSE
	BOOL bIsVaildPacket;		// default: FALSE
};

enum ENServerType : DWORD
{
	ST_ALL = -1,			// 2005-12-27 by cmkwon
	ST_NORMAL_SERVER = 0,
	ST_PRE_SERVER = 1,
	ST_IM_SERVER = 2,
	ST_LOG_SERVER = 3,
	ST_MONITOR_SERVER = 4,
	ST_FIELD_SERVER = 5,
	ST_NPC_SERVER = 6,
	ST_CLIENT_TYPE = 7,
	ST_ADMIN_TOOL = 8,
	ST_INVALID_TYPE = 9,
	//ST_AUTHENTICATION_SERVER = 10,		// 2011-01-26 by hskim, 인증 서버 구현
	//ST_FIELD_WEB_SERVER = 11				// 2013-03-13 by hskim, 웹 캐시 상점
};

enum ProcessResult
{
	RES_BREAK = 0,	// 경미한 에러들. 연결을 끊지 않는다.
	RES_PACKET_ERROR = 1,	// 패킷이 손상된 경우. 남은 packet을 처리하지 않고 바로 리턴한다. 서버간 연결에만 사용.
	RES_RETURN_TRUE = 2,	// 정상
	RES_RETURN_FALSE = 3,		// 연결을 끊어야하는 경우. 프로토콜 에러 등
	RES_PACKET_NA = 4,
	RES_ACCESS_DENIED = 5
};

// 디버그용 - For PrintExchangeMsg()
enum
{
	RECV_TYPE = 0,
	SEND_TYPE = 1
};

enum : BYTE
{
	PRINTLEVEL_ALL = 0,
	PRINTLEVEL_WO_ALIVE = 1,
	PRINTLEVEL_WO_MOVE_BATTLE = 2,
	PRINTLEVEL_A_FEW = 3,
	PRINTLEVEL_FEW = 4,
	PRINTLEVEL_NO_MSG = 5
};

// Socket 관련 Global Variable
extern BYTE g_exchangeMsgPrintLevel;	// check: MSG를 print하는 level을 조정하기 위해(AtumMonitor로 조절 가능), by kelovon

// Socket 관련 Global Function
const char *GGetENServerTypeString(ENServerType st);

void GSetexchangeMsgPrintLevel(BYTE i_byPrintLevel);
BYTE GGetexchangeMsgPrintLevel();

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
char* GGetIPByName(char *i_strHostName, char *o_strIPAddr);

///////////////////////////////////////////////////////////////////////////////
// 2006-04-11 by cmkwon

enum
{
	IP_TYPE_AUTOPRIVATE = 1,
	IP_TYPE_PUBLIC = 2,
	IP_TYPE_PRIVATE = 3
};

BOOL GGetLocalIP(char *o_szLocalIP, int nLocalIPType = IP_TYPE_AUTOPRIVATE);
BOOL _GGetLocalIP(BYTE *ip0, BYTE *ip1, BYTE *ip2, BYTE *ip3, int nLocalIPType);