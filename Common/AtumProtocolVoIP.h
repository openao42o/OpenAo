#if 0 // ndef _ATUM_PROTOCOL_VOIP_H_
#define _ATUM_PROTOCOL_VOIP_H_

#include "AtumProtocol.h"
#include "VoIPError.h"


// AtumProtocol.h에 정의 되어있음
//#define T0_ATUMVOIP						0x60				// 음성 통화를 위해
	#define T1_IC_VOIP_GET_ACCOUNTUNIQUENUMBER				0x00
	#define T1_IC_VOIP_GET_ACCOUNTUNIQUENUMBER_OK			0x01
	#define T1_IC_VOIP_1to1_DIAL							0x10
	#define T1_IC_VOIP_1to1_DIAL_OK							0x11
	#define T1_IC_VOIP_1to1_END								0x12

	#define T1_IC_VOIP_NtoN_CREATEROOM_REQUEST				0x80	
	#define T1_IC_VOIP_NtoN_CREATEROOM_REQUEST_OK			0x81	
	#define T1_IC_VOIP_NtoN_INVITE_ALL						0x82	
	#define T1_IC_VOIP_NtoN_INVITE_USER						0x83
	#define T1_IC_VOIP_NtoN_INVITE_YOU						0x84
	#define T1_IC_VOIP_NtoN_GOOUT_ALL						0x85
	
	#define T1_IC_VOIP_ERROR								0xFF

///////////////////////////////////////////////////////////////////////////////
//
	#define T_IC_VOIP_GET_ACCOUNTUNIQUENUMBER				(MessageType_t)((T0_ATUMVOIP<<8)|T1_IC_VOIP_GET_ACCOUNTUNIQUENUMBER)
	#define T_IC_VOIP_GET_ACCOUNTUNIQUENUMBER_OK			(MessageType_t)((T0_ATUMVOIP<<8)|T1_IC_VOIP_GET_ACCOUNTUNIQUENUMBER_OK)
	#define T_IC_VOIP_1to1_DIAL								(MessageType_t)((T0_ATUMVOIP<<8)|T1_IC_VOIP_1to1_DIAL)
	#define T_IC_VOIP_1to1_DIAL_OK							(MessageType_t)((T0_ATUMVOIP<<8)|T1_IC_VOIP_1to1_DIAL_OK)
	#define T_IC_VOIP_1to1_END								(MessageType_t)((T0_ATUMVOIP<<8)|T1_IC_VOIP_1to1_END)

	#define T_IC_VOIP_NtoN_CREATEROOM_REQUEST				(MessageType_t)((T0_ATUMVOIP<<8)|T1_IC_VOIP_NtoN_CREATEROOM_REQUEST)
	#define T_IC_VOIP_NtoN_CREATEROOM_REQUEST_OK			(MessageType_t)((T0_ATUMVOIP<<8)|T1_IC_VOIP_NtoN_CREATEROOM_REQUEST_OK)
	#define T_IC_VOIP_NtoN_INVITE_ALL						(MessageType_t)((T0_ATUMVOIP<<8)|T1_IC_VOIP_NtoN_INVITE_ALL)
	#define T_IC_VOIP_NtoN_INVITE_USER						(MessageType_t)((T0_ATUMVOIP<<8)|T1_IC_VOIP_NtoN_INVITE_USER)
	#define T_IC_VOIP_NtoN_INVITE_YOU						(MessageType_t)((T0_ATUMVOIP<<8)|T1_IC_VOIP_NtoN_INVITE_YOU)
	#define T_IC_VOIP_NtoN_GOOUT_ALL						(MessageType_t)((T0_ATUMVOIP<<8)|T1_IC_VOIP_NtoN_GOOUT_ALL)

	#define T_IC_VOIP_ERROR									(MessageType_t)((T0_ATUMVOIP<<8)|T1_IC_VOIP_ERROR)



///////////////////////////////////////////////////////////////////////////////
//
	typedef struct
	{
		char			szCalledCharacterName[SIZE_MAX_CHARACTER_NAME];
	} MSG_IC_VOIP_GET_ACCOUNTUNIQUENUMBER;

	typedef struct
	{
		UID32_t			nCalledAccountUniqueNumber;
	} MSG_IC_VOIP_GET_ACCOUNTUNIQUENUMBER_OK;

	typedef struct
	{
		UID32_t			nCallerAccountUniqueNumber;
		UID32_t			nCalledAccountUniqueNumber;
		char			szCallerCharacterName[SIZE_MAX_CHARACTER_NAME];
	} MSG_IC_VOIP_1to1_DIAL;
	typedef struct
	{
		UID32_t			nCallerAccountUniqueNumber;
		UID32_t			nCalledAccountUniqueNumber;
	} MSG_IC_VOIP_1to1_DIAL_OK;
	typedef struct
	{
		UID32_t			nSend2AccountUniqueNumber;
	} MSG_IC_VOIP_1to1_END;

	typedef struct
	{
		BYTE			byIsPartyCall;					// TRUE이면 편대통화, FALSE이면 길드 통화
	} MSG_IC_VOIP_NtoN_CREATEROOM_REQUEST;
	typedef struct
	{
		BYTE			byIsPartyCall;					// TRUE이면 편대통화, FALSE이면 길드 통화
	} MSG_IC_VOIP_NtoN_CREATEROOM_REQUEST_OK;
	typedef struct
	{
		BYTE			byIsPartyCall;					// TRUE이면 편대통화, FALSE이면 길드 통화
	} MSG_IC_VOIP_NtoN_INVITE_ALL;
	typedef struct
	{
		BYTE			byIsPartyCall;					// TRUE이면 편대통화, FALSE이면 길드 통화
		UID32_t			nCharacterUniqueNumber;
	} MSG_IC_VOIP_NtoN_INVITE_USER;
	typedef struct
	{
		BYTE			byIsPartyCall;					// TRUE이면 편대통화, FALSE이면 길드 통화
	} MSG_IC_VOIP_NtoN_INVITE_YOU;
	typedef struct
	{
		BYTE			byIsPartyCall;
	} MSG_IC_VOIP_NtoN_GOOUT_ALL;

	typedef struct
	{
		UINT			nErrorNum;
		UID32_t			nSend2AccountUniqueNumber;
	} MSG_IC_VOIP_ERROR;

	



#endif
