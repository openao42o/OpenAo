// ArenaFieldWinSocket.h: interface for the CArenaFieldWinSocket class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ARENAFIELDWINSOCKET_H__078F259B_A907_4803_AE97_42DA4107846C__INCLUDED_)
#define AFX_ARENAFIELDWINSOCKET_H__078F259B_A907_4803_AE97_42DA4107846C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "WinSocket.h"

#define	WM_FIELD_ASYNC_EVENT			(WM_USER_BASE + 7)
#define	WM_FIELD_PACKET_NOTIFY			(WM_USER_BASE + 8)

class CFieldIOCP;
class CFieldIOCPSocket;
class CArenaFieldWinSocket : public CWinSocket 
{
public:
	CArenaFieldWinSocket(HWND hWnd, UINT wmSock, UINT wmNotify);
	virtual ~CArenaFieldWinSocket();

	BOOL OnCloseSocket(int nErrorCode);
	BOOL OnConnect(int nErrorCode);

	void SetFieldIOCP(CFieldIOCP *pFieldIOCP);
	BOOL CheckInvalidCharacter(CFieldIOCPSocket * i_pFISoc, UID32_t i_AccountUID, UID32_t i_CharacterUID);

	ProcessResult Process_FtoA_AUTH_CHARACTER(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FtoA_ARENA_STATE_CHANGE(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FtoA_ARENA_TEAM_MATCHING(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FtoA_ARENA_SERVER_PAY(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FtoA_ARENA_CHARACTER_PAY(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FtoA_ARENA_CHARACTER_DISCONNECT(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FtoA_ARENA_CHARACTERARENAINFO_UPDATA(const char* pPacket, int nLength, int &nBytesUsed);	// 2012-04-12 by jhseol, 아레나 추가개발 - 보상 : AS에서 받은 CharacterArena 정보 업데이트
	ProcessResult Process_FtoA_INFINITY_START(const char* pPacket, int nLength, int &nBytesUsed);				// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 시작 MainFieldServer로 알림, AFS -> MFS
	ProcessResult Process_FtoA_INFINITY_IMPUTE(const char* pPacket, int nLength, int &nBytesUsed);				// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 MainFieldServer로 귀속 정보 전송, AFS -> MFS
	ProcessResult Process_FtoA_INFINITY_UPDATE_USER_INFO(const char* pPacket, int nLength, int &nBytesUsed);			// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 MainFieldServer로 유저 동기화 위해 전송, AFS -> MFS
	ProcessResult Process_FtoA_INFINITY_UPDATE_ALL_ITEM_COUNT(const char* pPacket, int nLength, int &nBytesUsed);		// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 MainFieldServer로 아이템 동기화 위해 전송, AFS -> MFS
	ProcessResult Process_FtoA_INFINITY_DELETE_ITEM(const char* pPacket, int nLength, int &nBytesUsed);				// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 MainFieldServer로 아이템 동기화 위해 전송, AFS -> MFS
	ProcessResult Process_FtoA_INFINITY_UPDATE_ITEM	(const char* pPacket, int nLength, int &nBytesUsed);			// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 MainFieldServer로 아이템 동기화 위해 전송, AFS -> MFS
	ProcessResult Process_FtoA_INFINITY_INSERT_ITEM(const char* pPacket, int nLength, int &nBytesUsed);				// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 MainFieldServer로 아이템 동기화 위해 전송, AFS -> MFS
	ProcessResult Process_FtoA_INFINITY_LOG(const char* pPacket, int nLength, int &nBytesUsed);						// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 MainFieldServer로 로그 전송, AFS -> MFS
	ProcessResult Process_FtoA_INFINITY_USING_TIME_LIMIT_ITEM(const char* pPacket, int nLength, int &nBytesUsed);						// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 MainFieldServer로 로그 전송, AFS -> MFS
	ProcessResult Process_FtoA_INFINITY_STATE_CHANGE(const char* pPacket, int nLength, int &nBytesUsed);		// 2009-09-09 ~ 2010-01-20 by dhjin, 인피니티 - 인피 상태 값을 메인서버로 전송한다. 인피 상태 체크
	ProcessResult Process_FtoA_INFINITY_START_CHECK(const char* pPacket, int nLength, int &nBytesUsed);		// 2010-03-23 by cmkwon, 인피니티 입장 캐쉬 아이템 구현 - 
	ProcessResult Process_FtoA_INFINITY_UPDATE_USER_MAP_INFO(const char* pPacket, int nLength, int &nBytesUsed);	// 2010-04-06 by cmkwon, 인피2차 추가 수정 - 
	ProcessResult Process_FtoA_INFINITY_TENDERITEM_LOG(const char* pPacket, int nLength, int &nBytesUsed);			// 2010-06-25 by shcho, 인피니티 관련로그 찍기 - 습득 아이템 정보 DB저장		
	ProcessResult Process_FtoA_INFINITY_COME_BACK_POSTWORK(const char* pPacket, int nLength, int &nBytesUsed);		// 2012-01-16 by hskim, 통계 - 화패 인피 MainFieldServer로 돌아가기 위한 후 작업 처리 명령, AFS -> MFS
	ProcessResult Process_FtoA_INFINITY_COME_BACK_PREWORK(const char* pPacket, int nLength, int &nBytesUsed);		// 2011-09-29 by hskim, 파트너 시스템 2차 - 인피 MainFieldServer로 돌아가기 위한 사전 작업 처리 명령, AFS -> MFS
	ProcessResult Process_FtoA_INFINITY_UPDATE_ITEM_PET(const char* pPacket, int nLength, int &nBytesUsed);			// 2011-09-30 by hskim, 파트너 시스템 2차 - 인피 MainFieldServer로 파트너 아이템 동기화 위해 전송, AFS -> MFS

	// error
	ProcessResult Process_ERROR(const char* pPacket, int nLength, int &nBytesUsed);

public:
	virtual BOOL OnRecvdPacket(LPSTR pPacket, int nLength, BYTE nSeq);

public:
	CFieldIOCP		*m_pFieldIOCP20;
};

#endif // !defined(AFX_ARENAFIELDWINSOCKET_H__078F259B_A907_4803_AE97_42DA4107846C__INCLUDED_)
