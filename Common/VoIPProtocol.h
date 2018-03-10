///////////////////////////////////////////////////////////////////////////////
//  VoIPProtocol.h
//			: 편대간 음성 통화에서 서버와 클라이언트이 통신을 위한 프로토콜 헤더 파일
//			  VoIP Distribute Server, VoIP Call Server, VoIP Client에서 사용됨
//
//  Date	: 2004-03-16 by cmkwon
///////////////////////////////////////////////////////////////////////////////

#ifndef _VOIPPROTOCOL_H_
#define _VOIPPROTOCOL_H_

#include "windows.h"
#include "DefineGlobal.h"
#include "SocketHeader.h"
#include "VoIPParam.h"
#include "VoIPError.h"

#ifndef _ATUM_CLIENT

#define VD_SERVER_PORT					15150	// VoIP Distribute Server의 Listening Port
#define VC_SERVER_PORT					15151	// VoIP Call Server의 Listening Port

///////////////////////////////////////////////////////////////////////////////
// MessageType T0 (대분류)
//	DC : DistributeServer CallServer
//	SD : SCVoIPClient DistributeServer
//	SC : SCVoIPClient CallServer
///////////////////////////////////////////////////////////////////////////////
	#define T0_DC_CONNECT					0x01
	#define T0_SD_CONNECT					0x02
	#define T0_SC_CONNECT					0x03

	#define T0_DC_ROOM						0x10
	#define T0_SD_ROOM						0x11
	#define T0_SC_ROOM						0x12

	#define T0_SC_TOKEN						0x20
	#define T0_SC_RTP						0x21


	#define T0_DM_MONITOR					0x31	// monitor protocol
	#define T0_CM_MONITOR					0x32	// monitor protocol

	#define T0_SD_ADMIN						0x36	// admin protocol
	#define T0_SC_ADMIN						0x37	// admin protocol

	#define T0_DC							0x40	// SendErrorMessage등에 Type으로 사용하기 위해
	#define T0_VD							0x41	// SendErrorMessage등에 Type으로 사용하기 위해
	#define T0_VC							0x42	// SendErrorMessage등에 Type으로 사용하기 위해

	#define T0_NA							0xD0	// INVALID PROTOCOL, NOT AVAILABLE

	#define T0_SD_STRING					0xFD	// debugging protocol
	#define T0_SC_STRING					0xFE

	#define T0_ERROR						0xFF	// error protocol





///////////////////////////////////////////////////////////////////////////////
// MessageType T1 (소분류)
///////////////////////////////////////////////////////////////////////////////
	// T0_DC_CONNECT
	#define T1_C2D_CONNECT							0x00	// No body
	#define T1_D2C_CONNECT_OK						0x01	// No body
	#define T1_C2D_SOCKET_ALIVE						0x02	// No body
	#define T1_C2D_REGISTER_SELF					0x03
	#define T1_D2C_REGISTER_SELF_OK					0x04	// No body

	// T0_SD_CONNECT
	#define T1_S2D_CONNECT							0x00	// No body
	#define T1_D2S_CONNECT_OK						0x01	// No body
	#define T1_S2D_SOCKET_ALIVE						0x02	// No body

	// T0_SC_CONNECT
	#define T1_S2C_CONNECT							0x00	// No body
	#define T1_C2S_CONNECT_OK						0x01	// No body
	#define T1_S2C_SOCKET_ALIVE						0x02	// No body



	///////////////////////////////////////////////////////////////////////////////
	// T0_DC_ROOM
	#define T1_C2D_VCSERVER_STATUS_INFO				0x00
	#define T1_C2D_CREATE_ROOM_NOTIFY				0x01
	#define T1_D2C_CREATE_ROOM_NOTIFY_OK			0x02
	#define T1_C2D_DELETE_ROOM_NOTIFY				0x03

	// T0_SD_ROOM
	#define T1_S2D_GET_VCSERVER_CONNECT_INFO		0x00
	#define T1_D2S_GET_VCSERVER_CONNECT_INFO_OK		0x11

	// T0_SC_ROOM
	#define T1_S2C_CREATE_ROOM						0x00
	#define T1_C2S_CREATE_ROOM_OK					0x01
	#define T1_S2C_JOIN_ROOM						0x02
	#define T1_C2S_JOIN_ROOM_OK						0x03
	#define T1_C2S_JOINED_USERLIST					0x04
	#define T1_C2S_JOIN_USER						0x05
	#define T1_S2C_GOOUT_ROOM						0x06		// No body
	#define T1_C2S_GOOUT_USER						0x07


	///////////////////////////////////////////////////////////////////////////////
	// T0_SC_TOKEN
	#define T1_S2C_REQUEST_TOKEN					0x00
	#define T1_C2S_REQUEST_TOKEN_OK					0x01
	#define T1_C2S_LOST_TOKEN						0x02		// 2005-10-05 by cmkwon
	#define T1_S2C_CHANGE_TALKING_STATE				0x03		// 2008-10-15 by cmkwon, 말하기 기능 수정 - 말하기 상태 변경 전송
	#define T1_C2S_CHANGE_TALKING_STATE_OK			0x04		// 2008-10-15 by cmkwon, 말하기 기능 수정 - 말하기 상태 변경 전송


	///////////////////////////////////////////////////////////////////////////////
	// T0_SC_RTP
	#define T1_S2C_RTP_PACKET						0x00
	#define T1_C2S_RTP_PACKET						0x01

	

	///////////////////////////////////////////////////////////////////////////////
	// T0_ERROR
	#define T1_ERROR								0x00
	


///////////////////////////////////////////////////////////////////////////////
// MessageType
///////////////////////////////////////////////////////////////////////////////
	// T0_DC_CONNECT
	#define T_C2D_CONNECT							(MessageType_t)((T0_DC_CONNECT<<8)|T1_C2D_CONNECT)
	#define T_D2C_CONNECT_OK						(MessageType_t)((T0_DC_CONNECT<<8)|T1_D2C_CONNECT_OK)
	#define T_C2D_SOCKET_ALIVE						(MessageType_t)((T0_DC_CONNECT<<8)|T1_C2D_SOCKET_ALIVE)
	#define T_C2D_REGISTER_SELF						(MessageType_t)((T0_DC_CONNECT<<8)|T1_C2D_REGISTER_SELF)
	#define T_D2C_REGISTER_SELF_OK					(MessageType_t)((T0_DC_CONNECT<<8)|T1_D2C_REGISTER_SELF_OK)

	// T0_SD_CONNECT
	#define T_S2D_CONNECT							(MessageType_t)((T0_SD_CONNECT<<8)|T1_S2D_CONNECT)
	#define T_D2S_CONNECT_OK						(MessageType_t)((T0_SD_CONNECT<<8)|T1_D2S_CONNECT_OK)
	#define T_S2D_SOCKET_ALIVE						(MessageType_t)((T0_SD_CONNECT<<8)|T1_S2D_SOCKET_ALIVE)

	// T0_SC_CONNECT
	#define T_S2C_CONNECT							(MessageType_t)((T0_SC_CONNECT<<8)|T1_S2C_CONNECT)
	#define T_C2S_CONNECT_OK						(MessageType_t)((T0_SC_CONNECT<<8)|T1_C2S_CONNECT_OK)
	#define T_S2C_SOCKET_ALIVE						(MessageType_t)((T0_SC_CONNECT<<8)|T1_S2C_SOCKET_ALIVE)



	///////////////////////////////////////////////////////////////////////////////
	// T0_DC_ROOM
	#define T_C2D_VCSERVER_STATUS_INFO				(MessageType_t)((T0_DC_ROOM<<8)|T1_C2D_VCSERVER_STATUS_INFO)
	#define T_C2D_CREATE_ROOM_NOTIFY				(MessageType_t)((T0_DC_ROOM<<8)|T1_C2D_CREATE_ROOM_NOTIFY)
	#define T_D2C_CREATE_ROOM_NOTIFY_OK				(MessageType_t)((T0_DC_ROOM<<8)|T1_D2C_CREATE_ROOM_NOTIFY_OK)
	#define T_C2D_DELETE_ROOM_NOTIFY				(MessageType_t)((T0_DC_ROOM<<8)|T1_C2D_DELETE_ROOM_NOTIFY)

	// T0_SD_ROOM
	#define T_S2D_GET_VCSERVER_CONNECT_INFO			(MessageType_t)((T0_SD_ROOM<<8)|T1_S2D_GET_VCSERVER_CONNECT_INFO)
	#define T_D2S_GET_VCSERVER_CONNECT_INFO_OK		(MessageType_t)((T0_SD_ROOM<<8)|T1_D2S_GET_VCSERVER_CONNECT_INFO_OK)

	// T0_SC_ROOM
	#define T_S2C_CREATE_ROOM						(MessageType_t)((T0_SC_ROOM<<8)|T1_S2C_CREATE_ROOM)
	#define T_C2S_CREATE_ROOM_OK					(MessageType_t)((T0_SC_ROOM<<8)|T1_C2S_CREATE_ROOM_OK)
	#define T_S2C_JOIN_ROOM							(MessageType_t)((T0_SC_ROOM<<8)|T1_S2C_JOIN_ROOM)
	#define T_C2S_JOIN_ROOM_OK						(MessageType_t)((T0_SC_ROOM<<8)|T1_C2S_JOIN_ROOM_OK)
	#define T_C2S_JOINED_USERLIST					(MessageType_t)((T0_SC_ROOM<<8)|T1_C2S_JOINED_USERLIST)
	#define T_C2S_JOIN_USER							(MessageType_t)((T0_SC_ROOM<<8)|T1_C2S_JOIN_USER)
	#define T_S2C_GOOUT_ROOM						(MessageType_t)((T0_SC_ROOM<<8)|T1_S2C_GOOUT_ROOM)
	#define T_C2S_GOOUT_USER						(MessageType_t)((T0_SC_ROOM<<8)|T1_C2S_GOOUT_USER)


	///////////////////////////////////////////////////////////////////////////////
	// T0_SC_TOKEN
	#define T_S2C_REQUEST_TOKEN						(MessageType_t)((T0_SC_TOKEN<<8)|T1_S2C_REQUEST_TOKEN)
	#define T_C2S_REQUEST_TOKEN_OK					(MessageType_t)((T0_SC_TOKEN<<8)|T1_C2S_REQUEST_TOKEN_OK)
	#define T_C2S_LOST_TOKEN						(MessageType_t)((T0_SC_TOKEN<<8)|T1_C2S_LOST_TOKEN)		// 2005-10-05 by cmkwon
	#define T_S2C_CHANGE_TALKING_STATE				(MessageType_t)((T0_SC_TOKEN<<8)|T1_S2C_CHANGE_TALKING_STATE)	// 2008-10-15 by cmkwon, 말하기 기능 수정 - 말하기 상태 변경 전송
	#define T_C2S_CHANGE_TALKING_STATE_OK			(MessageType_t)((T0_SC_TOKEN<<8)|T1_C2S_CHANGE_TALKING_STATE_OK)	// 2008-10-15 by cmkwon, 말하기 기능 수정 - 말하기 상태 변경 전송
	

	///////////////////////////////////////////////////////////////////////////////
	// T0_SC_RTP
	#define T_S2C_RTP_PACKET						(MessageType_t)((T0_SC_RTP<<8)|T1_S2C_RTP_PACKET)
	#define T_C2S_RTP_PACKET						(MessageType_t)((T0_SC_RTP<<8)|T1_C2S_RTP_PACKET)



	///////////////////////////////////////////////////////////////////////////////
	// T0_ERROR
	#define T_ERROR							(MessageType_t)((T0_ERROR<<8)|T1_ERROR)


///////////////////////////////////////////////////////////////////////////////
// Structures for MessageType
///////////////////////////////////////////////////////////////////////////////
	// T0_DC_CONNECT
	typedef struct
	{
		ServerID_t	vcServerID;
		DWORD		dwMaxRoomCounts;
		DWORD		dwMaxUserCounts;
		DWORD		dwMaxBandwidth;
	} MSG_C2D_REGISTER_SELF;

	
	// T0_SD_CONNECT

	
	// T0_SC_CONNECT



	///////////////////////////////////////////////////////////////////////////////
	// T0_DC_ROOM
	typedef struct
	{
		DWORD			dwCurRoomCounts;
		DWORD			dwCurUserCounts;
		DWORD			dwCurBandwidth;
	} MSG_C2D_VCSERVER_STATUS_INFO;
	
	typedef struct
	{
		char			szUserName[SIZE_MAX_USER_NAME];
		ClientIndex_t	ClientIndex;
		int				nConcurrentTalkingCount;	// 2005-10-04 by cmkwon, 동시 말하기 가능 카운트
		RoomID_t		CreateRoomID;
	} MSG_C2D_CREATE_ROOM_NOTIFY;

	typedef struct
	{
		char			szUserName[SIZE_MAX_USER_NAME];
		ClientIndex_t	ClientIndex;
		int				nConcurrentTalkingCount;	// 2005-10-04 by cmkwon, 동시 말하기 가능 카운트
		RoomID_t		CreateRoomID;
	} MSG_D2C_CREATE_ROOM_NOTIFY_OK;	

	typedef struct
	{
		RoomID_t		DeleteRoomID;
	} MSG_C2D_DELETE_ROOM_NOTIFY;
	
	// T0_SD_ROOM
	typedef struct
	{
		RoomID_t		connectRoomID;
		BYTE			bCreateRoomFlag;
	} MSG_S2D_GET_VCSERVER_CONNECT_INFO;

	typedef struct
	{
		ServerID_t		connectVCServerID;
		BYTE			bCreateRoomFlag;
	} MSG_D2S_GET_VCSERVER_CONNECT_INFO_OK;


	// T0_SC_ROOM
	typedef struct
	{
		char			szUserName[SIZE_MAX_USER_NAME];
		int				nConcurrentTalkingCount;	// 2005-10-04 by cmkwon, 동시 말하기 카운트
		RoomID_t		createRoomID;
	} MSG_S2C_CREATE_ROOM;

	typedef struct
	{
		BYTE			nChannelIndex;
	} MSG_C2S_CREATE_ROOM_OK;

	typedef struct
	{
		char			szUserName[SIZE_MAX_USER_NAME];
		RoomID_t		joinRoomID;
	} MSG_S2C_JOIN_ROOM;

	typedef struct
	{
		BYTE			nChannelIndex;
	} MSG_C2S_JOIN_ROOM_OK;

	typedef struct
	{
		char			szUserName[SIZE_MAX_USER_NAME];
		BYTE			nChannelIndex;
		BYTE			bIsTalking;			// 2008-10-15 by cmkwon, 말하기 기능 수정 - 
	} JoinedUserInfo;
	typedef struct
	{
		BYTE			byUserCounts;
		ARRAY_(JoinedUserInfo);
	} MSG_C2S_JOINED_USERLIST;

	typedef struct
	{
		char			szUserName[SIZE_MAX_USER_NAME];
		BYTE			nChannelIndex;
		BYTE			bIsTalking;			// 2008-10-15 by cmkwon, 말하기 기능 수정 - 
	} MSG_C2S_JOIN_USER;

	typedef struct
	{
		char			szUserName[SIZE_MAX_USER_NAME];
		BYTE			nChannelIndex;
	} MSG_C2S_GOOUT_USER;

	///////////////////////////////////////////////////////////////////////////////
	// T0_SC_TOKEN
	typedef struct
	{		
		BYTE			byIsTalk;
	} MSG_S2C_REQUEST_TOKEN;

	typedef struct
	{
		BYTE			byIsTalk;
		BYTE			nChannelIndex;
		char			szUserName[SIZE_MAX_USER_NAME];
	} MSG_C2S_REQUEST_TOKEN_OK;

	struct MSG_C2S_LOST_TOKEN
	{
		BYTE			nChannelIndex;
		char			szUserName[SIZE_MAX_USER_NAME];
	};

	// 2008-10-15 by cmkwon, 말하기 기능 수정 - 
	struct MSG_S2C_CHANGE_TALKING_STATE	
	{
		TALKING_STATE_t talkingState;
		BYTE			nChannelIndex;
		char			szUserName[SIZE_MAX_USER_NAME];
	};

	// 2008-10-15 by cmkwon, 말하기 기능 수정 - 
	typedef MSG_S2C_CHANGE_TALKING_STATE MSG_C2S_CHANGE_TALKING_STATE_OK;



	///////////////////////////////////////////////////////////////////////////////
	// T0_SC_RTP
	typedef struct
	{
		BYTE			byMediaType;
		USHORT			usRTPLength;
		ARRAY_(BYTE);
	} MSG_S2C_RTP_PACKET;

	typedef struct
	{
		DWORD			dwChannelIndex;
		BYTE			byMediaType;
		USHORT			usRTPLength;
		ARRAY_(BYTE);
	} MSG_C2S_RTP_PACKET;


	///////////////////////////////////////////////////////////////////////////////
	// T0_ERROR
	// 2015-05-16 Future, macro redifiniton & Voip not used
	//typedef struct 
	//{
	//	MessageType_t	MsgType;			// 에러가 일어났을 때 처리중이었던 Message Type
	//	Error_t			ErrorCode;			// 2 bytes
	//	BOOL			CloseConnection;	// Client에서 연결을 끊을지에 대한 여부
	//	INT				ErrParam1;			// general-purpose err parameter 1
	//	INT				ErrParam2;			// general-purpose err parameter 2
	//} MSG_ERROR;

#endif// _ATUM_CLIENT_endif


///////////////////////////////////////////////////////////////////////////////////
// SCDialer1.ocx
	typedef struct
	{
		char szCharacterName[SIZE_MAX_CHARACTER_NAME];
		BOOL EnableTalking;			// 2008-10-15 by cmkwon, 말하기 기능 수정 - 
	} MSG_EV_JOIN_USER;

	typedef struct
	{
		char szCharacterName[SIZE_MAX_CHARACTER_NAME];
	} MSG_EV_GOOUT_USER;

	typedef struct
	{
		char szCharacterName[SIZE_MAX_CHARACTER_NAME];
		BOOL bTalkFlag;
	} MSG_EV_TALK_USER;

	typedef struct
	{
		char szCharacterName[SIZE_MAX_CHARACTER_NAME];
		int		nTakingState;	// TALKING_STATE_XXX
	} MSG_EV_CHANGED_TALKING_STATE;		// 2008-10-15 by cmkwon, 말하기 기능 수정 - 
	
	typedef struct
	{
		UINT		nAccountUniqueNumber;
		char		szCharacterName[SIZE_MAX_CHARACTER_NAME];
	} MSG_MT_REGISTER;

	typedef struct
	{
		UINT		nCalleeAccountUniqueNumber;
	} MSG_MT_DIAL;


	typedef struct
	{
		UINT		nAccountUniqueNumber;
		int			nConcurrentTalkingCount;		// 동시 말하기 가능 카운트, 2005-10-04 by cmkwon
		char		szCharacterName[SIZE_MAX_CHARACTER_NAME];
		LONGLONG	llPartyID;
	} MSG_MT_PARTY_CREATE_ROOM;

	typedef struct
	{
		UINT		nAccountUniqueNumber;
		int			nConcurrentTalkingCount;		// 동시 말하기 가능 카운트, 2005-10-04 by cmkwon
		char		szCharacterName[SIZE_MAX_CHARACTER_NAME];
		UINT		uiGuildUniqueNumber;
	} MSG_MT_GUILD_CREATE_ROOM;

	typedef struct
	{
		UINT		nAccountUniqueNumber;
		char		szCharacterName[SIZE_MAX_CHARACTER_NAME];
		LONGLONG	llPartyID;
	} MSG_MT_PARTY_JOIN_ROOM;
	typedef struct
	{
		UINT		nAccountUniqueNumber;
		char		szCharacterName[SIZE_MAX_CHARACTER_NAME];
		UINT		uiGuildUniqueNumber;
	} MSG_MT_GUILD_JOIN_ROOM;
	
// SCDialer1.ocx_end
///////////////////////////////////////////////////////////////////////////////
#endif	// _VOIPPROTOCOL_H_endif