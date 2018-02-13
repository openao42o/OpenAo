#pragma once

#include <windef.h>
#if _MSC_VER >= 1310
#include <atltime.h>
#endif //_MSC_VER
#include <assert.h>

#include "ArtpErrorCode.h"

#define IN
#define OUT

#define ARCODE_DC "CM001001"
#define ARCODE_CF "CM001002"

struct artpdef {
	enum {
		AUTH_USERID_LEN = 32,
		AUTH_USERPASSWORD_LEN = 60, // (before, 20)
		AUTH_SESSIONKEY_LEN = 300,		// for later --> make bigger as 500 (before, 150)
		/*
		* DATE : 20081023 
		* SUBJECT : 유저별 제제 확인 기능 추가
		* WRITER :BY KEILOVE
		*/
		AUTH_GAMEID_LEN = 16,
		AUTH_ENDDATE_LEN = 12,
	};
	enum {
		UI_EMAIL_LEN = 100,
		UI_INNERCODE_LEN = 8,
	};
	enum {
		RCS_RESERVED_LEN = 100,
	};
};

//+--------------------------------------------------------------------------------------------------------------------
//! ARTP (ARario Transfer Protocol) ???꾨줈?좎퐳 醫낅쪟
//!
struct ArtpPktIndex {
	enum {
		PING,			//0..
		GHOSTCHECK,
		AUTH,
		REQRET
	};

	enum 
	{
		VALUE0 = REQRET,
		VALUE1,
	};

	enum {
		RCS_INDEX = VALUE1,
		RCS,
	};
};

struct ArtpPktSubindex {
	enum {
		REQRESULT = 0,
		AUTH_AuthInfo = 10,
		AUTH_IssueSessionKey,
		AUTH_RefreshSessionKey,
		AUTH_AuthBySessionKey,
	};

	/*
	* DATE : 20081023 
	* SUBJECT : 유저별 제제 확인 기능 추가
	* WRITER :BY KEILOVE
	*/
	enum {
		AUTH_PenaltyInfo = AUTH_AuthBySessionKey+1,
	};

	enum 
	{
		VALUE0 = AUTH_AuthBySessionKey,
		VALUE1,
	};
	
	enum {
		RCS_INDEX = VALUE1,
		RCS_ERROR, 
		RCS_GETRIOCASH,
		RCS_USERIOCASH,
		RCS_ROLLBACK,
	};

};

// align as 1byte
#include <PshPack1.h>

//+--------------------------------------------------------------------------------------------------------------------
//! ARTP??湲곕낯 援ъ“泥?
//!
typedef struct tARTP_PKT_SIZE {
	USHORT		m_pktSize			;	// sizeof all packet
} ARTP_PKT_SIZE;

typedef struct tARTP_HEADERv1 : public ARTP_PKT_SIZE {
	BYTE		m_hdrProtocolType	;	// 1:binary, 0:text
	USHORT		m_hdrProtoSetNo		;
	USHORT		m_hdrPacketIndex	;
	USHORT		m_hdrPacketSubIndex	;
	BYTE		m_hdrIsEncoded		;
	__int64		m_hdrTime			;
} ARTP_HEADER;

#define SIZEOF_ARTPHEADER		sizeof(ARTP_HEADER)
#define DEFAULT_PAKCET_SIZE		8192

//+--------------------------------------------------------------------------------------------------------------------
//! User information from client
//!
typedef struct tARTP_AUTHINFO : public ARTP_HEADER {
	WCHAR		m_userId[ artpdef::AUTH_USERID_LEN+1 ];
	WCHAR		m_userPassword[ artpdef::AUTH_USERPASSWORD_LEN+1 ];
	long		m_userIndex;
} ARTP_AUTHINFO;

//+--------------------------------------------------------------------------------------------------------------------
//! User information by session key (for server side)
//!
typedef struct tARTP_AUTHINFOBYSESKEY : public ARTP_HEADER {
	char m_sessionKey[ artpdef::AUTH_SESSIONKEY_LEN+1 ];
} ARTP_AUTHINFOBYSESKEY;

//+--------------------------------------------------------------------------------------------------------------------
//! ?몄뀡?ㅻ? 諛쒖넚?섎뒗 ?꾨줈?좎퐳 
//!
typedef struct tARTP_ISSUESESSIONKEY : public ARTP_HEADER {
	char m_sessionKey [ artpdef::AUTH_SESSIONKEY_LEN+1 ];
} ARTP_ISSUESESSIONKEY;

typedef struct tARTP_REFRESHSESSIONKEY : public ARTP_HEADER {
	WCHAR m_userId    [ artpdef::AUTH_USERID_LEN+1     ];
	char  m_sessionKey[ artpdef::AUTH_SESSIONKEY_LEN+1 ];
} ARTP_REFRESHSESSIONKEY;

/*
* DATE : 20081023 
* SUBJECT : 유저별 제제 확인 기능 추가
* WRITER :BY KEILOVE
*/
typedef struct tARTP_PENALTYINFO : public ARTP_HEADER {
	WCHAR		m_userID[ artpdef::AUTH_USERID_LEN+1 ];
	WCHAR		m_gameID[ artpdef::AUTH_GAMEID_LEN+1 ];
} ARTP_PENALTYINFO;

//+--------------------------------------------------------------------------------------------------------------------
//! use at rcs
//! define length
//!
typedef struct IN tARTP_RCS_ROOT : public ARTP_HEADER
{
	enum
	{
		db_member_len = 32,
		db_item_len = 50,
		db_gamecode_len = 8,
		db_servercode_len = 8,
		db_hashcode_len = 32,
		db_paytype_len = 8,
	};

	//enum
	//{
	//	member_len = db_member_len + 1, 
	//	item_len = db_item_len + 1, 
	//	gamecode_len = db_gamecode_len + 1,
	//	servercode_len = db_servercode_len + 1,		
	//	reserved_len = 100,
	//	hashcode_len = db_hashcode_len + 1,		
	//	paytype_len = db_paytype_len + 1,
	//};

	enum
	{
		member_len = db_member_len, 
		item_len = db_item_len, 
		gamecode_len = db_gamecode_len,
		servercode_len = db_servercode_len,		
		reserved_len = 100,
		hashcode_len = db_hashcode_len,		
		paytype_len = db_paytype_len,
	};	

	// user info
	LONG m_usn;
	WCHAR m_memberID[member_len];
} ARTP_RCS_ROOT;

//+--------------------------------------------------------------------------------------------------------------------
//! use at rcs
//! Get RioCash
//!
typedef struct IN tARTP_RCS_GETRIOCASH : public ARTP_RCS_ROOT
{

} ARTP_RCS_GETRIOCASH;

//+--------------------------------------------------------------------------------------------------------------------
//! use at rcs
//! Use RioCash
//!
typedef struct IN tARTP_RCS_USERIOCASH  : public ARTP_RCS_ROOT
{	
	INT m_clientRemainder;
	WCHAR m_itemID[item_len];
	WCHAR m_gameCode[gamecode_len];
	WCHAR m_serverID[servercode_len];	
	LONGLONG m_characterID;	
	WCHAR m_reserved[reserved_len];	
} ARTP_RCS_USERIOCASH;

//+--------------------------------------------------------------------------------------------------------------------
//! use at rcs
//! roll back
//!
typedef struct IN tARTP_RCS_ROLLBACK  : public ARTP_RCS_ROOT
{	
	WCHAR m_hashcode[hashcode_len];
} ARTP_RCS_ROLLBACK;

//+--------------------------------------------------------------------------------------------------------------------
//! use at rcs
//! Result from server
//!
typedef struct OUT tARTP_RCS_RESULT: public ARTP_RCS_ROOT
{
	// result
	UINT m_result;
	UINT m_subResult;
} ARTP_RCS_RESULT;

//+--------------------------------------------------------------------------------------------------------------------
//! use at rcs
//! Result from server
//!
typedef struct OUT tARTP_RET_RCS_GETRIOCASH : public ARTP_RCS_RESULT
{	
	INT m_cash;	
} ARTP_RET_RCS_GETRIOCASH;

//+--------------------------------------------------------------------------------------------------------------------
//! use at rcs
//! Result from server
//!
typedef struct OUT tARTP_RET_RCS_USERIOCASH : public ARTP_RCS_RESULT
{		
	INT m_remainder;
	WCHAR m_itemID[item_len];	
	WCHAR m_hashcode[hashcode_len];
} ARTP_RET_RCS_USERIOCASH;

//+--------------------------------------------------------------------------------------------------------------------
//! use at rcs
//! Result from rollback
//!
typedef struct OUT tARTP_RET_RCS_ROLLBACK : public ARTP_RCS_RESULT
{
	INT m_cash;
	WCHAR m_hashcode[hashcode_len];
} ARTP_RET_RCS_ROLLBACK;

//+--------------------------------------------------------------------------------------------------------------------
//! client濡?蹂대궡吏??Result???⑦궥
//!
#define ARTP_DEFINE_RETCLASS( myClassName, parentClassName )	\
	typedef struct t##myClassName : public parentClassName {	\
		LONG m_result;											\
	} myClassName

ARTP_DEFINE_RETCLASS( ARTP_REQRESULT, ARTP_HEADER );
ARTP_DEFINE_RETCLASS( ARTP_REQRESULT_SESSIONKEY, ARTP_ISSUESESSIONKEY );
ARTP_DEFINE_RETCLASS( ARTP_REQRESULT_REFRESHSESSIONKEY, ARTP_REFRESHSESSIONKEY );
ARTP_DEFINE_RETCLASS( ARTP_REQRESULT_AUTHINFOBYSESKEY, ARTP_AUTHINFOBYSESKEY );

typedef struct tARTP_REQRESULT_AUTHINFO : public ARTP_AUTHINFO {
	char m_sessionKey[ artpdef::AUTH_SESSIONKEY_LEN+1 ];
	LONG m_result;
} ARTP_REQRESULT_AUTHINFO;

/*
* DATE : 20081023 
* SUBJECT : 유저별 제제 확인 기능 추가
* WRITER :BY KEILOVE
*/
typedef struct tARTP_REQRESULT_PENALTYINFO : public ARTP_PENALTYINFO {	
	LONG m_result;
	int m_penalty;
	TCHAR m_endDate[artpdef::AUTH_ENDDATE_LEN];
} ARTP_REQRESULT_PENALTYINFO;


//typedef struct tARTP_REQRESULT : public ARTP_HEADER {
//	LONG		m_result;
//} ARTP_REQRESULT;
//
//typedef struct tARTP_REQRESULT_SESSIONKEY : public ARTP_ISSUESESSIONKEY {
//	LONG		m_result;
//} ARTP_REQRESULT_SESSIONKEY;
//
//typedef struct tARTP_REQRESULT_REFRESHSESSIONKEY : public ARTP_REFRESHSESSIONKEY {
//	LONG		m_result;
//} ARTP_REQRESULT_REFRESHSESSIONKEY;

#include <PopPack.h>

//+--------------------------------------------------------------------------------------------------------------------
//! Helper utility
//!
inline bool makeARTPHeader( OUT ARTP_HEADER& hdr_, IN USHORT pktIdx_, IN USHORT pktSubIdx_, BYTE isEncoded_ = 0, IN BYTE type_ = 1 )
{
	hdr_.m_pktSize				= 0					;
	hdr_.m_hdrProtocolType		= type_				;
	hdr_.m_hdrProtoSetNo		= 0					; // 0 is default, this var reserved for later(future)
	hdr_.m_hdrPacketIndex		= pktIdx_			;
	hdr_.m_hdrPacketSubIndex	= pktSubIdx_		;
	hdr_.m_hdrIsEncoded			= isEncoded_		;
#if _MSC_VER >= 1310 // if .net2003 or more higher version
	hdr_.m_hdrTime				= CTime::GetCurrentTime().GetTime()	;
#else
	hdr_.m_hdrTime				= time(0);
#endif //_MSC_VER
	return true;
}

inline void makeRCSResultHeader( ARTP_RCS_ROOT& in, ARTP_RCS_RESULT& out )
{
	// set request info
	out.m_usn = in.m_usn;
	wmemcpy(out.m_memberID, in.m_memberID, ARTP_RCS_ROOT::member_len);

	// set default result code
	out.m_result = ARTPRET_OK;
	out.m_subResult = ARTPRET_OK;
}