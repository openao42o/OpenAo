// IMWinSocket.h: interface for the CIMWinSocket class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IMWINSOCKET_H__CE9F7F73_C6DF_4E82_A0FE_5DB8F40B2E69__INCLUDED_)
#define AFX_IMWINSOCKET_H__CE9F7F73_C6DF_4E82_A0FE_5DB8F40B2E69__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "WinSocket.h"

#define	WM_IM_ASYNC_EVENT				(WM_USER + 5)				//
#define	WM_IM_PACKET_NOTIFY				(WM_USER + 6)				// HWND에 전달되는 메시지

class CFieldIOCP;

class CIMWinSocket : public CWinSocket
{
public:
	CIMWinSocket(HWND hWnd, UINT wmSock, UINT wmNotify);
	virtual ~CIMWinSocket();

	BOOL OnCloseSocket(int nErrorCode);
	BOOL OnConnect(int nErrorCode);

	void SetFieldIOCP(CFieldIOCP *pFieldIOCP);

	// Message 처리 함수
	// 2007-08-27 by cmkwon, 서버다운준비 명령어 추가(SCAdminTool에서 SCMonitor의 PrepareShutdown을 진행 할 수 있게)
	ProcessResult Process_FI_CONNECT_PREPARE_SHUTDOWN(const char* pPacket, int nLength, int &nBytesUsed);


	// party
	ProcessResult Process_FI_PARTY_CREATE_OK(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FI_PARTY_ACCEPT_INVITE_OK(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FI_PARTY_LEAVE_OK(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FI_PARTY_TRANSFER_MASTER_OK(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FI_PARTY_BAN_MEMBER_OK(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FI_PARTY_DISMEMBER_OK(const char* pPacket, int nLength, int &nBytesUsed);

	ProcessResult Process_FI_PARTY_ADD_MEMBER(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FI_PARTY_DELETE_MEMBER(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FI_PARTY_UPDATE_PARTY_INFO(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FI_PARTY_CHANGE_EXP_DISTRIBUTE_TYPE(const char* pPacket, int nLength, int &nBytesUsed);		// 2008-06-04 by dhjin, EP3 편대 수정 - 경험치 분배 방식 변경 
	ProcessResult Process_FI_PARTY_CHANGE_ITEM_DISTRIBUTE_TYPE(const char* pPacket, int nLength, int &nBytesUsed);		// 2008-06-04 by dhjin, EP3 편대 수정 - 아이템 분배 방식 변경
	ProcessResult Process_FI_PARTY_CHANGE_FORMATION_SKILL(const char* pPacket, int nLength, int &nBytesUsed);		// 2009-08-03 by cmkwon, EP3-4 편대 대형 스킬 구현 - 
	ProcessResult Process_FI_PARTY_AUTO_CREATE_OK(const char* pPacket, int nLength, int &nBytesUsed);				// 2009-09-09 ~ 2010-01-26 by dhjin, 인피니티 - 자동 편대 
	ProcessResult Process_FI_PARTY_DISCONNECT_LEAVE_OK(const char* pPacket, int nLength, int &nBytesUsed);				// 2009-09-09 ~ 2010-01-27 by dhjin, 인피니티 - 강제 종료 유저 파티에서 탈퇴처리!
	
	// chat
	ProcessResult Process_FI_CHAT_MAP(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FI_CHAT_REGION(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FI_CHAT_CHANGE_CHAT_FLAG(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FI_CHAT_CASH_ALL(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FI_CHAT_ARENA(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FI_CHAT_ONLINE_EACHOTHER_FRIEND_COUNT_OK(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FI_CHAT_OUTPOST_GUILD(const char* pPacket, int nLength, int &nBytesUsed);		// 2007-10-06 by cmkwon, 전진 기지 소유한 여단장 세력 채팅 가능 -
	ProcessResult Process_FI_CHAT_INFINITY(const char* pPacket, int nLength, int &nBytesUsed);		// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 채팅
	ProcessResult Process_FI_CHAT_CNC(const char* pPacket, int nLength, int &nBytesUsed);		// 2015-07-20 Future, CnC costs money

	// admin
	ProcessResult Process_FI_ADMIN_GET_CHARACTER_INFO(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FI_ADMIN_CALL_CHARACTER(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FI_ADMIN_MOVETO_CHARACTER(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FI_ADMIN_CHANGE_WEATHER(const char* pPacket, int nLength, int &nBytesUsed);

	// character
	ProcessResult Process_FI_CHARACTER_UPDATE_GUILD_INFO(const char* pPacket, int nLength, int &nBytesUsed);

// 2004-05-24 cmkwon, 삭제함
//	// monitor
//	ProcessResult Process_FI_GET_FIELD_USER_COUNTS(const char* pPacket, int nLength, int &nBytesUsed);

	// 여단전
	ProcessResult Process_FI_GUILD_NOTIFY_END_WAR(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FI_GUILD_REG_DELETE_GUILD(const char* pPacket, int nLength, int &nBytesUsed);

	// event
	ProcessResult Process_FI_EVENT_NOTIFY_WARP_OK(const char* pPacket, int nLength, int &nBytesUsed);
// 2004-12-16 by cmkwon, 다른 필드서버로의 워프는 없으므로 삭제함
//	ProcessResult Process_FI_EVENT_GET_WARP_INFO_OK(const char* pPacket, int nLength, int &nBytesUsed);

	ProcessResult Process_FI_GUILD_RANK_OK(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FI_GUILD_DELETE_INFO_OK(const char* pPacket, int nLength, int &nBytesUsed);	// 2006-09-29 by dhjin, 길드 삭제 시 PROCEDURE atum_AllDelete_Guild_Store 실행
	
	ProcessResult Process_FI_CREATE_GUILD_BY_SUBLEADER(const char* pPacket, int nLength, int &nBytesUsed);	// 2007-10-06 by dhjin

	// 2012-10-17 by jhjang 해피 아워 경험치 보너스 시스템 리뉴얼
	ProcessResult Process_FI_GUILD_BOUNUEXP_RATE(const char* pPacket, int nLength, int &nBytesUsed);
	// end 2012-10-17 by jhjang 해피 아워 경험치 보너스 시스템 리뉴얼

	// error
	ProcessResult Process_ERROR(const char* pPacket, int nLength, int &nBytesUsed);

public:
	virtual BOOL OnRecvdPacket(LPSTR pPacket, int nLength, BYTE nSeq);

public:
	CFieldIOCP		*m_pFieldIOCP4;
};

#endif // !defined(AFX_IMWINSOCKET_H__CE9F7F73_C6DF_4E82_A0FE_5DB8F40B2E69__INCLUDED_)
