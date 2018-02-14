//Copyright[2002] MasangSoft
#pragma once

// It's ok
#define ARTPRET_OK						 0

// About Packet
#define ARTPRET_PKT_NODATA				-1		// packet struct has no data.
#define ARTPRET_INVALID_PACKET			-2		// invalid(unexpected) packet
#define ARTPRET_INVALID_SESSIONKEY		-3		// can not disassembly sessionkey

// about auth
//! simple memo of auth result
//!  0 : success
//! -1 : multiple log-in
//! -2 : can't found user info 
//! -3 : password is wrong
//! -4 : unknown reason
#define ARTPRET_AUTH_MULTIPLELOGIN		-10		// multiple login
#define ARTPRET_AUTH_NOUSERINFO			-11		// no user info
#define ARTPRET_AUTH_WRONGPASSWORD		-12		// wrong password
#define ARTPRET_AUTH_UNKNOWNREASON		-13		// as words, unknown error (out of expected)
#define ARTPRET_AUTH_INTERNALERROR		-14		// internal error. cause will be various
#define ARTPRET_AUTH_SESKEYNOTISSUED	-15		// failed to issue new session key
#define ARTPRET_AUTH_SESKEYTOOOLD		-16		// session key looks old one
#define ARTPRET_AUTH_SESKEYNOTREFRESHED	-17		// failed to refresh session key

#define ARTPRET_EVT_NOENTERPERMISSION	-20		// No Permission to entering to game channel


#define ARTPRET_NOCALLBACK				-50
#define ARTPRET_BUFFEROVERFLOW			-51

#define ARTPRET_SKT_CONNFAIL			-100
#define ARTPRET_SKT_SENDFAIL			-101
#define ARTPRET_SKT_RECVFAIL			-102
#define ARTPRET_SKT_NODATATOSEND		-103
#define ARTPRET_SKT_UNKNOWN				-199

inline bool isSocketError( int errCode_ )
{
	return (errCode_ <= ARTPRET_SKT_CONNFAIL && errCode_ >= ARTPRET_SKT_UNKNOWN);
}

#define ARTPRET_GAME_STARTINGFAIL			-200
#define ARTPERT_GAME_INVALIDMAINWND			-201
#define ARTPRET_GAME_CREATEPROCESSFAILED	-202
#define ARTPRET_GAME_NOGAMEPROCESSFOUND		-203
#define ARTPRET_GAME_NOGAMEWNDFOUND			-204
#define ARTPRET_GAME_NOTENOUGHTPARAMETER	-205

// about gss
#define ARTPRET_GSS_NOGAMEID			-10		// no game id
#define ARTPRET_GSS_UNKNOWNREASON		-11		// as words, unknown error (out of expected)
#define ARTPRET_GSS_INTERNALERROR		-12		// internal error. cause will be various
#define ARTPRET_GSS_RETRY				-13		// internal error. cause will be various

#define GAME_SERVICE_ON					1
#define GAME_SERVICE_OFF				0

// about rcs
#define ARTPRET_RCS_UNKNOWNREASON		-11		// as words, unknown error (out of expected)
#define ARTPRET_RCS_INTERNALERROR		-12		// internal error. cause will be various
#define ARTPRET_RCS_RETRY				-13		// plz, retry...
#define ARTPRET_RCS_FAILQUERY			-14		// db error
#define ARTPRET_RCS_FAILDBCONNECTION	-17		// db error

// DB ERROR
#define ARTPRET_DB_SESSIONERROR			-200	
#define ARTPRET_DB_OPENERROR			-201	
// SP ERROR
#define ARTPRET_SP_INTERNALERROR		-400
// SP RETURN VALUE
#define SP_OK							0
#define SP_NOUSER						11
#define SP_NOHASHCODE					12
#define SP_NOMONEY						71
#define SP_HACKING						81

