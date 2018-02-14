#ifndef _LOCALIZE_SETTING_H_
#define _LOCALIZE_SETTING_H_



// 공지사항 특수 문자
#if defined( LANGUAGE_ITALIA )
	#define	CHAT_ALL			'<'		// 공지채팅
	#define	CHAT_GUILD			'@'		// 길드채팅
	#define	CHAT_ARENA			'%'		// 아레나채팅
	#define	CHAT_WAR			'^'		// 전쟁채팅
	#define	CHAT_PARTY			'#'		// 편대채팅
	#define	CHAT_MAP			'!'		// 맵전체채팅
	#define	CHAT_SELL_ALL		'$'		// 매매채팅
	#define	CHAT_CHATROOM		'&'		// 유료맵전체채팅
	#define	CHAT_PTOP			'"'		// 귓속말
	#define	CHAT_INFLUENCE_ALL	'*'		// 지도자공지
	#define	CHAT_COMMAND		'/'		// 게임명령어
	#define CHAT_CNC			'<'
#else
	#define	CHAT_ALL			'~'		// 공지채팅
	#define	CHAT_GUILD			'@'		// 길드채팅
	#define	CHAT_ARENA			'%'		// 아레나채팅
	#define	CHAT_WAR			'^'		// 전쟁채팅
	#define	CHAT_PARTY			'#'		// 편대채팅
	#define	CHAT_MAP			'!'		// 맵전체채팅
	#define	CHAT_SELL_ALL		'$'		// 매매채팅
	#define	CHAT_CHATROOM		'&'		// 유료맵전체채팅
	#define	CHAT_PTOP			'"'		// 귓속말
	#define	CHAT_INFLUENCE_ALL	'*'		// 지도자공지
	#define	CHAT_COMMAND		'/'		// 게임명령어
	#define CHAT_CNC			'<'
#endif

#endif