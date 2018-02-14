//Copyright[2002] MasangSoft
#pragma once

#ifndef ARAUTH4CLI_DLLDECLARE
#	define ARAUTH4CLI_DLLDECLARE	__declspec(dllexport) 
#else
#	define ARAUTH4CLI_DLLDECLARE	__declspec(dllimport) 
#endif //ARAUTH4CLI_DLLDECLARE

#if _MSC_VER >= 1310 // if .net2003 or more higher version
#include <atlstr.h>
#endif
#include "ArtpErrorCode.h"
#include "ArtpProtocol.h"
#include "AuthFunctor.h"

#define USERID_LEN	40
#define USERPWD_LEN 22
#define TICKET_LEN	255
#define URL_LEN		1024 * 2

#include <PshPack1.h>

struct ArGameInfo
{
	WCHAR gameId [ MAX_PATH + 1 ];			// game id of arario inside
	WCHAR developerId [ MAX_PATH + 1 ];			// game studio title
	WCHAR gameName [ MAX_PATH + 1 ];			// original game title
	WCHAR defChannelId [ MAX_PATH + 1 ];			// default channel id for auto-entering
	bool isAlpha;
};

struct ArGameStartInfo
{
	WCHAR		userId [ USERID_LEN + 1 ]; // user id
	DWORD		userIndex						;
	char		ticket [ TICKET_LEN + 1 ]; // ticket to auth (or being auth.ed)
	WCHAR		downloadUrl	[ URL_LEN + 1 ]; // download url to download patch file
	HWND		launcherHwnd; // Window handle of Arario Launcher
	bool		isFullScreen; // Is full screen mode
	ArGameInfo	optGameInfo; // optional game information
};

#include <PopPack.h>

extern "C" {

	//!====================================================================================================================
	//! @brief	try login on Arario Auth server by asynchronous method.
	//!			because of that, result of login can't know right after using this func.
	//!			To know result, register function pointer with other function "SetResultCallbackFunction()"
	//!
#if _MSC_VER >= 1310 // if .net2003 or more higher version
	ARAUTH4CLI_DLLDECLARE int  __cdecl AraAuthLoginB( LPCWSTR userId_, LPCWSTR password_, CStringA& sesKey_, DWORD waitTime_ = 5000 );
#else
	ARAUTH4CLI_DLLDECLARE int  __cdecl AraAuthLoginB( LPCWSTR userId_, LPCWSTR password_, char* sesKey_, DWORD waitTime_ = 5000 );
#endif //_MSC_VER

	//!====================================================================================================================
	//! @brief	try login on Arario Auth server by synchronous method.
	//!			user can know the result of auth with return value of func.
	//!
	ARAUTH4CLI_DLLDECLARE void __cdecl AraAuthLoginNB( LPCWSTR userId_, LPCWSTR password_ );

	//!====================================================================================================================
	//! @brief	try login on Arario Auth server by synchronous method.
	//!			user can know the result of auth with return value of func.
	//!			this function made for server side auth process.
	//!
	ARAUTH4CLI_DLLDECLARE int __cdecl AraAuthLoginWithSesKeyB( LPCSTR oldSesKey_, LPSTR newSesKey_ );

	//!====================================================================================================================
	//! @brief	try login on Arario Auth server by asynchronous method.
	//!			user can know the result of auth with return value of func.
	//!			this function made for server side auth process.
	//!
	ARAUTH4CLI_DLLDECLARE int __cdecl AraAuthLoginWithSesKeyNB( LPCSTR oldSesKey_ );

	/*
	* DATE : 20081027 
	* SUBJECT : 유저별 제제 확인 기능 추가
	* WRITER :BY KEILOVE
	*/
	ARAUTH4CLI_DLLDECLARE int __cdecl GetPenaltyInfoB( LPCWSTR userId_, LPCWSTR gameId_, int& penalty_, WCHAR* endDate );	

	//!====================================================================================================================
	//! @brief	update session key
	//!
	ARAUTH4CLI_DLLDECLARE int  __cdecl UpdateSessionKey( LPCWSTR userId_, LPCSTR oldSesKey_, LPSTR newSesKey_ );
	
	//!====================================================================================================================
	//! @brief	set callback function of auth process.
	//!
	ARAUTH4CLI_DLLDECLARE void __cdecl SetResultCallbackFunction( IFunctor* pCallBackFunc_ );

	//!====================================================================================================================
	//! @brief	try getting game parameter from begin started second application (like patcher, game launcher)
	//!			result can know by return value.
	//!
	ARAUTH4CLI_DLLDECLARE bool __cdecl GetGameParameter( ArGameStartInfo* pGameStartInfo_ );
	
	//!====================================================================================================================
	//! @brief	set ini path	
	//!
	ARAUTH4CLI_DLLDECLARE void __cdecl SetAuthInfoPath( LPCWSTR _path );
}
