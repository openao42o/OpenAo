// LogIOCPSocket.h: interface for the CLogIOCPSocket class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGIOCPSOCKET_H__23324694_E31B_4344_9DF9_F0EB3B36EED2__INCLUDED_)
#define AFX_LOGIOCPSOCKET_H__23324694_E31B_4344_9DF9_F0EB3B36EED2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LogIOCP.h"
#include "AtumDBManager.h"
#include "odbcstatement.h"

class CLogIOCPSocket : public CIOCPSocket
{
	friend CLogIOCP;
public:
	CLogIOCPSocket();
	virtual ~CLogIOCPSocket();

public:
	BOOL OnRecvdPacket(const char* pPacket, int nLength, BYTE nSeq, char* pPeerIP = "", int nPeerPort = 0, SThreadInfo *i_pThreadInfo=NULL) override;
	virtual void OnConnect(void);
	virtual void OnClose(int reason);

	BOOL Process_FL_LOG_LOGIN(const char* pPacket, int nLength, int &nBytesUsed);
	BOOL Process_FL_LOG_LOGOUT(const char* pPacket, int nLength, int &nBytesUsed);
	BOOL Process_FL_LOG_LEVEL(const char* pPacket, int nLength, int &nBytesUsed);
	BOOL Process_FL_LOG_EXP(const char* pPacket, int nLength, int &nBytesUsed);
	BOOL Process_FL_LOG_STAT(const char* pPacket, int nLength, int &nBytesUsed);
	BOOL Process_FL_LOG_GAME_START(const char* pPacket, int nLength, int &nBytesUsed);
	BOOL Process_FL_LOG_GAME_END(const char* pPacket, int nLength, int &nBytesUsed);
	BOOL Process_FL_LOG_QUEST_COMPLETION(const char* pPacket, int nLength, int &nBytesUsed);
	BOOL Process_FL_LOG_QUEST_COMPENSATION(const char* pPacket, int nLength, int &nBytesUsed);
	BOOL Process_FL_LOG_PKWIN(const char* pPacket, int nLength, int &nBytesUsed);
	BOOL Process_FL_LOG_PKLOSS(const char* pPacket, int nLength, int &nBytesUsed);
	BOOL Process_FL_LOG_PvEWIN(const char* pPacket, int nLength, int &nBytesUsed);
	BOOL Process_FL_LOG_PvELOSS(const char* pPacket, int nLength, int &nBytesUsed);
	BOOL Process_FL_LOG_DEAD(const char* pPacket, int nLength, int &nBytesUsed);
	BOOL Process_FL_LOG_WARP(const char* pPacket, int nLength, int &nBytesUsed);
	BOOL Process_FL_LOG_CREATE_CHARACTER(const char* pPacket, int nLength, int &nBytesUsed);
	BOOL Process_FL_LOG_DELETE_CHARACTER(const char* pPacket, int nLength, int &nBytesUsed);

	BOOL Process_FL_LOG_ITEM_CHARACTER_ITEM(const char* pPacket, int nLength, int &nBytesUsed);
	BOOL Process_FL_LOG_ITEM_GET_ITEM(const char* pPacket, int nLength, int &nBytesUsed);
	BOOL Process_FL_LOG_ITEM_ADD_ITEM_BY_COMMAND(const char* pPacket, int nLength, int &nBytesUsed);
	BOOL Process_FL_LOG_ITEM_INSERT_TO_STORE(const char* pPacket, int nLength, int &nBytesUsed);
	BOOL Process_FL_LOG_ITEM_DRAW_FROM_STORE(const char* pPacket, int nLength, int &nBytesUsed);
	BOOL Process_FL_LOG_ITEM_TRADE_SEND(const char* pPacket, int nLength, int &nBytesUsed);
	BOOL Process_FL_LOG_ITEM_TRADE_RECV(const char* pPacket, int nLength, int &nBytesUsed);
	BOOL Process_FL_LOG_ITEM_THROWAWAY_ITEM(const char* pPacket, int nLength, int &nBytesUsed);
	BOOL Process_FL_LOG_ITEM_TRY_ENCHANT_INFO(const char* pPacket, int nLength, int &nBytesUsed);		// 2013-07-01 by bckim, 10인챈트 미만 하이퍼카드처리, 로그추가, 운영툴 보완
	BOOL Process_FL_LOG_ITEM_ITEM_DISSOLUTION_ITEM(const char* pPacket, int nLength, int &nBytesUsed);	// 2013-08-05 by bckim, 용해 대상아이템 로그 추가
	BOOL Process_FL_LOG_ITEM_BUY_ITEM(const char* pPacket, int nLength, int &nBytesUsed);
	BOOL Process_FL_LOG_ITEM_BUY_CASH_PRICE(const char* pPacket, int nLength, int &nBytesUsed);	// 2012-10-16 by bckim, 캐쉬아이템 지불정보 Log 추가
	BOOL Process_FL_LOG_ITEM_CHARACTER_ITEM_GUILDSTORE_RESULT(const char* pPacket, int nLength, int &nBytesUsed);	// 2012-12-17 by bckim, 길드창고 아이템 이동 관련 추가로그 
	BOOL Process_FL_LOG_ITEM_INITIALIZED_GUILD_STORE(const char* pPacket, int nLength, int &nBytesUsed);	// 2012-12-20 by bckim, 세력초기화시 길드창고아이템이 길드장에게 옮겨지는 아이템 로그
	BOOL Process_FL_LOG_ITEM_GET_WITH_SAME_ACCOUNT(const char* pPacket, int nLength, int &nBytesUsed);	// 2012-12-27 by bckim, 동일계정, 각각의 케릭터간의 아이템 이동시 로그추가

	BOOL Process_FL_LOG_ITEM_SELL_ITEM(const char* pPacket, int nLength, int &nBytesUsed);
	BOOL Process_FL_LOG_ITEM_USE_ITEM(const char* pPacket, int nLength, int &nBytesUsed);
	BOOL Process_FL_LOG_ITEM_USE_ENERGY(const char* pPacket, int nLength, int &nBytesUsed);
	BOOL Process_FL_LOG_ITEM_USE_ENCHANT(const char* pPacket, int nLength, int &nBytesUsed);
	BOOL Process_FL_LOG_ITEM_STORE_ITEM(const char* pPacket, int nLength, int &nBytesUsed);
	BOOL Process_FL_LOG_ITEM_USE_MIX(const char* pPacket, int nLength, int &nBytesUsed);
	BOOL Process_FL_LOG_ITEM_USE_CHANGE_CHARACTER_NAME_ORIGIN(const char* pPacket, int nLength, int &nBytesUsed);
	BOOL Process_FL_LOG_ITEM_BAZAAR_SEND(const char* pPacket, int nLength, int &nBytesUsed);
	BOOL Process_FL_LOG_ITEM_BAZAAR_RECV(const char* pPacket, int nLength, int &nBytesUsed);
	BOOL Process_FL_LOG_ITEM_INSERT_TO_GUILDSTORE(const char* pPacket, int nLength, int &nBytesUsed);
	BOOL Process_FL_LOG_ITEM_DRAW_FROM_GUILDSTORE(const char* pPacket, int nLength, int &nBytesUsed);
	BOOL Process_FL_LOG_ITEM_SPI(const char* pPacket, int nLength, int &nBytesUsed);			// 2006-10-27 by cmkwon
	BOOL Process_FL_LOG_ITEM_GET_ITEM_BY_ITEMEVENT(const char* pPacket, int nLength, int &nBytesUsed);	// 2007-01-11 by cmkwon
	BOOL Process_FL_LOG_WARPOINT(const char* pPacket, int nLength, int &nBytesUsed);					// 2007-05-15 by dhjin
	BOOL Process_FL_LOG_ARENA_TEAM_MATCH(const char* pPacket, int nLength, int &nBytesUsed);			// 2007-06-11 by dhjin
	BOOL Process_FL_LOG_TUTORIAL_COMPLETION(const char* pPacket, int nLength, int &nBytesUsed);			// 2007-07-06 by dhjin
	BOOL Process_FL_LOG_OUTPOST_START(const char* pPacket, int nLength, int &nBytesUsed);			// 2007-10-02 by dhjin
	BOOL Process_FL_LOG_OUTPOST_END(const char* pPacket, int nLength, int &nBytesUsed);			// 2007-10-02 by dhjin
	BOOL Process_FL_LOG_EXPEDIENCYFUND(const char* pPacket, int nLength, int &nBytesUsed);			// 2007-10-02 by dhjin

	BOOL Process_FL_LOG_SERVER_INFO_MAP(const char* pPacket, int nLength, int &nBytesUsed);
	BOOL Process_FL_LOG_SERVER_INFO_TOTAL(const char* pPacket, int nLength, int &nBytesUsed);
	BOOL Process_FL_LOG_HACKING_LOG(const char* pPacket, int nLength, int &nBytesUsed);

	BOOL Process_FL_LOG_SPEED_HACK(const char* pPacket, int nLength, int &nBytesUsed);
	BOOL Process_FL_LOG_MEMORY_HACK(const char* pPacket, int nLength, int &nBytesUsed);

	BOOL Process_FL_LOG_MONSTER_BOSS(const char* pPacket, int nLength, int &nBytesUsed);

	//////////////////////////////////////////////////////////////////////////
	// 2008-04-01 by dhjin, 모선전, 거점전 정보창 기획안 - 
	BOOL Process_FL_LOG_MS_WAR(const char* pPacket, int nLength, int &nBytesUsed);
	BOOL Process_FL_LOG_SP_WAR(const char* pPacket, int nLength, int &nBytesUsed);


	BOOL Process_FL_LOG_START_FIELD_SERVER(const char* pPacket, int nLength, int &nBytesUsed);

	BOOL Process_FL_LOG_INFLWAR_START(const char* pPacket, int nLength, int &nBytesUsed);				// 2007-10-16 by cmkwon, 로그 추가 -
	BOOL Process_FL_LOG_INFLWAR_END(const char* pPacket, int nLength, int &nBytesUsed);					// 2007-10-16 by cmkwon, 로그 추가 -
	BOOL Process_FL_LOG_OUTPOSTWAR_RESET_START(const char* pPacket, int nLength, int &nBytesUsed);		// 2007-10-16 by cmkwon, 로그 추가 -
	BOOL Process_FL_LOG_OUTPOSTWAR_RESET_DESTROY(const char* pPacket, int nLength, int &nBytesUsed);	// 2007-10-16 by cmkwon, 로그 추가 -
	BOOL Process_FL_LOG_OUTPOSTWAR_RESET_COMPLETE(const char* pPacket, int nLength, int &nBytesUsed);		// 2007-10-16 by cmkwon, 로그 추가 -
	BOOL Process_FL_LOG_OUTPOSTWAR_PROTECTOR_DESTROY(const char* pPacket, int nLength, int &nBytesUsed);// 2007-10-16 by cmkwon, 로그 추가 -
	BOOL Process_FL_LOG_OUTPOSTWAR_SET_NEXTTIME(const char* pPacket, int nLength, int &nBytesUsed);		// 2007-10-16 by cmkwon, 로그 추가 -

	BOOL Process_FL_LOG_POLL_VOTE(const char* pPacket, int nLength, int &nBytesUsed);					// 2007-11-09 by dhjin
	BOOL Process_FL_LOG_POLL_DELETE_LEADERCANDIDATE(const char* pPacket, int nLength, int &nBytesUsed);	// 2007-11-09 by dhjin
	BOOL Process_FL_LOG_DISMEMBER_GUILD(const char* pPacket, int nLength, int &nBytesUsed);				// 2007-11-09 by dhjin
	BOOL Process_FL_LOG_NOTIFY_MSG_DELETE(const char* pPacket, int nLength, int &nBytesUsed);			// 2007-11-28 by cmkwon, 통지시스템 구현 -
	BOOL Process_FL_LOG_ITEM_ATTACH_ITEM(const char* pPacket, int nLength, int &nBytesUsed);			// 2008-01-23 by cmkwon, S_F, S_L: 장착/장착해제 게임 로그에 추가 - 
	BOOL Process_FL_LOG_ITEM_DETACH_ITEM(const char* pPacket, int nLength, int &nBytesUsed);			// 2008-01-23 by cmkwon, S_F, S_L: 장착/장착해제 게임 로그에 추가 - 
	BOOL Process_FL_LOG_ITEM_DELETE(const char* pPacket, int nLength, int &nBytesUsed);				// 2008-01-23 by cmkwon, S_F, S_L: 장착/장착해제 게임 로그에 추가 - 
	BOOL Process_FL_LOG_USE_COUPON(const char* pPacket, int nLength, int &nBytesUsed);				// 2008-01-23 by cmkwon, S_F, S_L: 쿠폰 사용 게임 로그에 추가 - 
	
	BOOL Process_FL_LOG_WAR_CONTRIBUTION(const char* pPacket, int nLength, int &nBytesUsed);		// 2008-12-29 by dhjin, 전쟁 보상 추가안
	BOOL Process_FL_LOG_WAR_CONTRIBUTION_GEAR(const char* pPacket, int nLength, int &nBytesUsed);		// 2008-12-29 by dhjin, 전쟁 보상 추가안

	BOOL Process_FL_LOG_LUCKY_DROP(const char* pPacket, int nLength, int &nBytesUsed);		// 2009-03-31 by dhjin, 럭키머신 로그
	BOOL Process_FL_LOG_INFINITY_START(const char* pPacket, int nLength, int &nBytesUsed);						// 193 - // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 게임로그 추가, 시작 관련 정보 
	BOOL Process_FL_LOG_INFINITY_START_ITEM(const char* pPacket, int nLength, int &nBytesUsed);					// 194 - // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 게임로그 추가, 시작시 아이템 복사 정보
	BOOL Process_FL_LOG_INFINITY_CINEMA(const char* pPacket, int nLength, int &nBytesUsed);						// 195 - // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 게임로그 추가, 연출 로그
	BOOL Process_FL_LOG_MONSTER_SKILL(const char* pPacket, int nLength, int &nBytesUsed);						// 196 - // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 게임로그 추가, 몬스터 스킬 사용시
	BOOL Process_FL_LOG_HPACTION_TALK(const char* pPacket, int nLength, int &nBytesUsed);						// 197 - // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 게임로그 추가, 몬스터 대화 사용시
	BOOL Process_FL_LOG_INFINITY_FIN(const char* pPacket, int nLength, int &nBytesUsed);						// 198 - // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 게임로그 추가, 종료 관련 정보 
	BOOL Process_FL_LOG_INFINITY_FIN_ITEM(const char* pPacket, int nLength, int &nBytesUsed);					// 199 - // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 게임로그 추가, 종료시 아이템 복사 정보
	BOOL Process_FL_LOG_INFINITY_FIN_ALIVE_KEY_MONSTER(const char* pPacket, int nLength, int &nBytesUsed);		// 200 - // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 게임로그 추가, 종료시 살아있는 몬스터 정보
	BOOL Process_FL_LOG_INFINITY_LEAVE_ITEM(const char* pPacket, int nLength, int &nBytesUsed);					// 201 - // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 게임로그 추가, 인피 탈퇴시 아이템 복사 정보
	BOOL Process_FL_LOG_EVENT_PARTICIPATION_RATE(const char* pPacket, int nLength, int &nBytesUsed);			// 68 -  // 2010-06-01 by shcho, GLogDB 관련 -

	BOOL Process_FL_LOG_PET_LEVEL(const char* pPacket, int nLength, int &nBytesUsed);							// 213 - // 2011-09-15 by hskim, 파트너 시스템 2차 - 파트너 레벨 변경

	BOOL Process_FL_LOG_CONNECT_USER_NEW(const char* pPacket, int nLength, int &nBytesUsed);					// 214 - // 2012-01-08 by hskim, GLog 2차 - 유저 신규 접속
	BOOL Process_FL_LOG_CONNECT_USER_LOGIN(const char* pPacket, int nLength, int &nBytesUsed);					// 215 - // 2012-01-08 by hskim, GLog 2차 - 유저 접속

	BOOL Process_FL_LOG_STATISTICS_MONEY(const char* pPacket, int nLength, int &nBytesUsed);					// 216 - // 2012-01-16 by hskim, 통계 - 화패

	BOOL Process_FL_LOG_FIXED_TERM_SHAPE_START(const char* pPacket, int nLength, int &nBytesUsed);			// 217 - // 2012-10-10 by hskim, 기간제 속성 구현 (기간제 외형)
	BOOL Process_FL_LOG_FIXED_TERM_SHAPE_END(const char* pPacket, int nLength, int &nBytesUsed);				// 218 - // 2012-10-10 by hskim, 기간제 속성 구현 (기간제 외형)
	
	// start 2012-11-21 by khkim, GLog (수정)
	BOOL Process_FL_LOG_ACCOUNTCONNECT(const char* pPacket, int nLength, int &nBytesUsed);
	BOOL Process_FL_LOG_CHARDATA(const char* pPacket, int nLength, int &nBytesUsed);
	BOOL Process_FL_LOG_ITEM(const char* pPacket, int nLength, int &nBytesUsed);
	BOOL Process_FL_LOG_ITEMSTATE(const char* pPacket, int nLength, int &nBytesUsed);
	BOOL Process_FL_LOG_SERVER(const char* pPacket, int nLength, int &nBytesUsed);
	// end 2012-11-21 by khkim, GLog

	BOOL Process_FL_LOG_COLLECTION_STATE(const char* pPacket, int nLength, int &nBytesUsed);				// 226 - // 2013-05-31 by jhseol,bckim 아머 컬렉션 - GameLog 추가
	BOOL Process_FL_LOG_COLLECTION_SHAPE_CHANGE(const char* pPacket, int nLength, int &nBytesUsed);			// 227 - // 2013-05-31 by jhseol,bckim 아머 컬렉션 - GameLog 추가
	BOOL Process_FL_LOG_ACCOUNT_INFL_CHANGE_BY_ADMIN(const char* pPacket, int nLength, int &nBytesUsed);	// 228 - // 2013-07-26 by jhseol, 타 계정 세력변경 - GameLog 추가

private:
	CODBCStatement		m_ODBCStmt;			

	static CLogIOCP		*ms_pLogIOCP;
};

#endif // !defined(AFX_LOGIOCPSOCKET_H__23324694_E31B_4344_9DF9_F0EB3B36EED2__INCLUDED_)
