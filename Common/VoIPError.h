#ifndef _VOIP_ERROR_H_
#define _VOIP_ERROR_H_

///////////////////////////////////////////////////////////////////////////////
// 
	#define VOIPERR_PROTOCOL_INVAILD_LENGTH					0x0001		// 

///////////////////////////////////////////////////////////////////////////////
// 1:1 통화시 오류
	#define VOIPERR_1to1_NO_SUCH_CHARACTER					0x1001		// 
	#define VOIPERR_1to1_NO_SUCH_ACCOUNTUNIQUENUMBER		0x1002
	#define VOIPERR_1to1_CANNOT_CALL						0x1003
	#define VOIPERR_1to1_REJECT								0x1004

///////////////////////////////////////////////////////////////////////////////
// N:N 통화시 오류
	#define VOIPERR_NtoN_NO_SUCH_WELLSUITED_VCSERVER		0x2001		// 적당한 VCServer가 없다, 등록된 VCServer가 없거나 접속가능한 VCServer가 없다
	#define VOIPERR_NtoN_ALREADY_REGISTER_VCSERVER			0x2002		// 이미 VCServer가 등록되었음
	#define VOIPERR_NtoN_ALREADY_EXISTS_ROOMID				0x2003		// 이미 등록된 RoomID임
	#define VOIPERR_NtoN_NO_SUCH_ROOMID						0x2004		// RoomID를 가진 Room이 없음
	#define VOIPERR_NtoN_NO_SUCH_ROOM						0x2005		// Room이 없음
	#define VOIPERR_NtoN_ALREADY_EXISTS_ROOM				0x2006		// 이미 Room이 있음
	#define VOIPERR_NtoN_FAIL_REQUEST_TOKEN					0x2007		// 말하기 권한 요청 실패
	#define VOIPERR_NtoN_LOST_TOKEN							0x2008		// 말하기 권한 뺏김
	#define VOIPERR_NtoN_CANNOT_TALKING						0x2009		// 말하기 권한 뺏김

	


#endif// _VOIP_ERROR_H_endif