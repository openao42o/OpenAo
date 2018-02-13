// LogIOCPSocket.cpp: implementation of the CLogIOCPSocket class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
// 2008-11-21 by cmkwon, LogServer를 MFC에서 Win32 프로젝트로 변경 - 
//#include "LogServer.h"
#include "LogIOCPSocket.h"
#include "LogIOCP.h"
#include "LogGlobal.h"
#include "AtumDBLogWriter.h"
#include "AtumError.h"
#include "AtumLogDBManager.h"

// 2008-11-21 by cmkwon, LogServer를 MFC에서 Win32 프로젝트로 변경 - 
//#ifdef _DEBUG
//#undef THIS_FILE
//static char THIS_FILE[]=__FILE__;
//#define new DEBUG_NEW
//#endif

// macro	DECLARE_MESSAGE_AND_CHECK_SIZE_LOG - 로그 서버의 메시지 교환에서만 사용
// brief	CXXXIOCPSocket::Process_XXX() 함수 내에서 초기에 메세지 선언 및 사이즈 확인 매크로
// author	kelovon
// date		2004-06-02
// warning	CXXXIOCPSocket::Process_XXX() 함수 내에서만 사용해야 함!
#define DECLARE_MESSAGE_AND_CHECK_SIZE_LOG(__PPACKET, __NLENGTH, __NBYTESUSED, __MSGTYPE, __MSGSTRUCT, __PMESSAGEVAR)	\
	INT			__NRECVTYPESIZE	= 0;									\
	__MSGSTRUCT	*__PMESSAGEVAR	= NULL;									\
	__NRECVTYPESIZE = sizeof(__MSGSTRUCT);								\
	if (__NLENGTH - __NBYTESUSED < __NRECVTYPESIZE)						\
	{																	\
		SendErrorMessage(__MSGTYPE, ERR_PROTOCOL_INVALID_FIELD_DATA);	\
		return FALSE;													\
	}																	\
	__PMESSAGEVAR = (__MSGSTRUCT*)(__PPACKET+__NBYTESUSED);				\
	__NBYTESUSED += __NRECVTYPESIZE;


CLogIOCP		*CLogIOCPSocket::ms_pLogIOCP = NULL;


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLogIOCPSocket::CLogIOCPSocket()
{
// 2006-02-10 by cmkwon
//	m_ODBCStmt.Init((char*)g_pGlobal->GetODBCDSN(), (char*)g_pGlobal->GetODBCUID(), (char*)g_pGlobal->GetODBCPASSWORD());
}

CLogIOCPSocket::~CLogIOCPSocket()
{
	m_ODBCStmt.Clean();
}


void CLogIOCPSocket::OnConnect(void)
{
	char szSystemLog[256];
	sprintf(szSystemLog, "Socket Connect SocketIndex[%3d] PeerIP[%15s] Port[%4d]\r\n",
		this->GetClientArrayIndex(), m_szPeerIP, m_nPeerPort);
	g_pLogGlobal->WriteSystemLog(szSystemLog);
	DBGOUT(szSystemLog);

	// 2006-02-10 by cmkwon
	CIOCPSocket::OnConnect();
}

void CLogIOCPSocket::OnClose(int reason)
{
	// 2009-04-20 by cmkwon, 시스템 로그 추가 - 
	//DbgOut("\n AtumLogServer Index[%d] Client[%s] Closed ==> reason %d[%08X]\r\n", this->GetClientArrayIndex(), m_szPeerIP, reason, reason);
	g_pLogGlobal->WriteSystemLogEX(TRUE, "  Socket closed ! Index[%d] Client[%s] Closed ==> reason %d[0x%08X]\r\n", this->GetClientArrayIndex(), m_szPeerIP, reason, reason);

	CIOCPSocket::OnClose(reason);
	
	// 2006-02-10 by cmkwon 
	m_ODBCStmt.Clean(); 
}

BOOL CLogIOCPSocket::OnRecvdPacket(const char* pPacket, int nLength,
											BYTE nSeq, char* pPeerIP, int nPeerPort, SThreadInfo *i_pThreadInfo)
{
	int						nBytesUsed		= 0;
	MessageType_t			nRecvType		= 0;
	MessageType_t			nPrevType		= 0;
	int						nRecvTypeSize	= 0;
	ProcessResult			procRes			= RES_BREAK;

	//////////////////////////////////////////////////////////////////////
	// UDP 처리 루틴
	if(m_bUDPFlag == TRUE)
	{
		return FALSE;
	}

	//////////////////////////////////////////////////////////////////////
	// TCP
	while(this->IsUsing() && nBytesUsed < nLength)
	{
		nRecvType = *(MessageType_t*)(pPacket+nBytesUsed);
		nBytesUsed += SIZE_FIELD_TYPE_HEADER;

		procRes = RES_RETURN_TRUE;
// 2006-05-19 by cmkwon, 로그 저장 시스템 수정함
// 		if(FALSE == m_ODBCStmt.IsReadyToUse())
// 		{
// 			m_ODBCStmt.Init((char*)g_pGlobal->GetODBCDSN(), (char*)g_pGlobal->GetODBCUID(), (char*)g_pGlobal->GetODBCPASSWORD());
// 		}

		switch(nRecvType)
		{
		case T_FL_LOG_LOGIN:				Process_FL_LOG_LOGIN(pPacket, nLength, nBytesUsed);						break;
		case T_FL_LOG_LOGOUT:				Process_FL_LOG_LOGOUT(pPacket, nLength, nBytesUsed);					break;
		case T_FL_LOG_LEVEL:				Process_FL_LOG_LEVEL(pPacket, nLength, nBytesUsed);						break;
		case T_FL_LOG_EXP:					Process_FL_LOG_EXP(pPacket, nLength, nBytesUsed);						break;
		case T_FL_LOG_STAT:					Process_FL_LOG_STAT(pPacket, nLength, nBytesUsed);						break;
		case T_FL_LOG_GAME_START:			Process_FL_LOG_GAME_START(pPacket, nLength, nBytesUsed);				break;
		case T_FL_LOG_GAME_END:				Process_FL_LOG_GAME_END(pPacket, nLength, nBytesUsed);					break;
		case T_FL_LOG_QUEST_COMPLETION:		Process_FL_LOG_QUEST_COMPLETION(pPacket, nLength, nBytesUsed);			break;
		case T_FL_LOG_QUEST_COMPENSATION:	Process_FL_LOG_QUEST_COMPENSATION(pPacket, nLength, nBytesUsed);		break;
		case T_FL_LOG_PKWIN:				Process_FL_LOG_PKWIN(pPacket, nLength, nBytesUsed);						break;
		case T_FL_LOG_PKLOSS:				Process_FL_LOG_PKLOSS(pPacket, nLength, nBytesUsed);					break;
		case T_FL_LOG_DEAD:					Process_FL_LOG_DEAD(pPacket, nLength, nBytesUsed);						break;
		case T_FL_LOG_WARP:					Process_FL_LOG_WARP(pPacket, nLength, nBytesUsed);						break;
		case T_FL_LOG_SPEED_HACK:			Process_FL_LOG_SPEED_HACK(pPacket, nLength, nBytesUsed);				break;
		case T_FL_LOG_CREATE_CHARACTER:		Process_FL_LOG_CREATE_CHARACTER(pPacket, nLength, nBytesUsed);			break;
		case T_FL_LOG_DELETE_CHARACTER:		Process_FL_LOG_DELETE_CHARACTER(pPacket, nLength, nBytesUsed);			break;
		case T_FL_LOG_MEMORY_HACK:			Process_FL_LOG_MEMORY_HACK(pPacket, nLength, nBytesUsed);				break;
		case T_FL_LOG_PvEWIN:				Process_FL_LOG_PvEWIN(pPacket, nLength, nBytesUsed);					break;
		case T_FL_LOG_PvELOSS:				Process_FL_LOG_PvELOSS(pPacket, nLength, nBytesUsed);					break;
		
		case T_FL_LOG_ITEM_CHARACTER_ITEM:	Process_FL_LOG_ITEM_CHARACTER_ITEM(pPacket, nLength, nBytesUsed);		break;
		case T_FL_LOG_ITEM_GET_ITEM:		Process_FL_LOG_ITEM_GET_ITEM(pPacket, nLength, nBytesUsed);				break;
		case T_FL_LOG_ITEM_ADD_ITEM_BY_COMMAND:	Process_FL_LOG_ITEM_ADD_ITEM_BY_COMMAND(pPacket, nLength, nBytesUsed);		break;
		case T_FL_LOG_ITEM_INSERT_TO_STORE:	Process_FL_LOG_ITEM_INSERT_TO_STORE(pPacket, nLength, nBytesUsed);		break;
		case T_FL_LOG_ITEM_DRAW_FROM_STORE:	Process_FL_LOG_ITEM_DRAW_FROM_STORE(pPacket, nLength, nBytesUsed);		break;
		case T_FL_LOG_ITEM_TRADE_SEND:		Process_FL_LOG_ITEM_TRADE_SEND(pPacket, nLength, nBytesUsed);			break;
		case T_FL_LOG_ITEM_TRADE_RECV:		Process_FL_LOG_ITEM_TRADE_RECV(pPacket, nLength, nBytesUsed);			break;
		case T_FL_LOG_ITEM_THROWAWAY_ITEM:	Process_FL_LOG_ITEM_THROWAWAY_ITEM(pPacket, nLength, nBytesUsed);		break;
		case T_FL_LOG_ITEM_TRY_ENCHANT_INFO:Process_FL_LOG_ITEM_TRY_ENCHANT_INFO(pPacket, nLength, nBytesUsed);		break;	// 2013-07-01 by bckim, 10인챈트 미만 하이퍼카드처리, 로그추가, 운영툴 보완
		case T_FL_LOG_ITEM_DISSOLUTION_ITEM:Process_FL_LOG_ITEM_ITEM_DISSOLUTION_ITEM(pPacket, nLength, nBytesUsed);		break;	// 2013-08-05 by bckim, 용해 대상아이템 로그 추가
		case T_FL_LOG_ITEM_BUY_ITEM:		Process_FL_LOG_ITEM_BUY_ITEM(pPacket, nLength, nBytesUsed);				break;
		case T_FL_LOG_ITEM_BUY_CASH_PRICE:	Process_FL_LOG_ITEM_BUY_CASH_PRICE(pPacket, nLength, nBytesUsed);		break;	// 2012-10-16 by bckim, 캐쉬아이템 지불정보 Log 추가
		case T_FL_LOG_ITEM_CHARACTER_ITEM_GUILDSTORE_RESULT:	Process_FL_LOG_ITEM_CHARACTER_ITEM_GUILDSTORE_RESULT(pPacket, nLength, nBytesUsed);		break;	// 2012-12-17 by bckim, 길드창고 아이템 이동 관련 추가로그 
		case T_FL_LOG_ITEM_INITIALIZED_GUILD_STORE:	Process_FL_LOG_ITEM_INITIALIZED_GUILD_STORE(pPacket, nLength, nBytesUsed);		break;	// 2012-12-20 by bckim, 세력초기화시 길드창고아이템이 길드장에게 옮겨지는 아이템 로그
		case T_FL_LOG_ITEM_GET_WITH_SAME_ACCOUNT:		Process_FL_LOG_ITEM_GET_WITH_SAME_ACCOUNT(pPacket, nLength, nBytesUsed);	break;	// 2012-12-27 by bckim, 동일계정, 각각의 케릭터간의 아이템 이동시 로그추가
		
		case T_FL_LOG_ITEM_SELL_ITEM:		Process_FL_LOG_ITEM_SELL_ITEM(pPacket, nLength, nBytesUsed);			break;
		case T_FL_LOG_ITEM_USE_ITEM:		Process_FL_LOG_ITEM_USE_ITEM(pPacket, nLength, nBytesUsed);				break;
		case T_FL_LOG_ITEM_USE_ENERGY:		Process_FL_LOG_ITEM_USE_ENERGY(pPacket, nLength, nBytesUsed);			break;
		case T_FL_LOG_ITEM_USE_ENCHANT:		Process_FL_LOG_ITEM_USE_ENCHANT(pPacket, nLength, nBytesUsed);			break;
		case T_FL_LOG_ITEM_STORE_ITEM:		Process_FL_LOG_ITEM_STORE_ITEM(pPacket, nLength, nBytesUsed);			break;
		case T_FL_LOG_ITEM_USE_MIX:			Process_FL_LOG_ITEM_USE_MIX(pPacket, nLength, nBytesUsed);				break;
		case T_FL_LOG_ITEM_USE_CHANGE_CHARACTER_NAME_ORIGIN:	Process_FL_LOG_ITEM_USE_CHANGE_CHARACTER_NAME_ORIGIN(pPacket, nLength, nBytesUsed);	break;
		case T_FL_LOG_ITEM_BAZAAR_SEND:		Process_FL_LOG_ITEM_BAZAAR_SEND(pPacket, nLength, nBytesUsed);	break;
		case T_FL_LOG_ITEM_BAZAAR_RECV:		Process_FL_LOG_ITEM_BAZAAR_RECV(pPacket, nLength, nBytesUsed);	break;
		case T_FL_LOG_ITEM_INSERT_TO_GUILDSTORE:			Process_FL_LOG_ITEM_INSERT_TO_GUILDSTORE(pPacket, nLength, nBytesUsed);	break;
		case T_FL_LOG_ITEM_DRAW_FROM_GUILDSTORE:			Process_FL_LOG_ITEM_DRAW_FROM_GUILDSTORE(pPacket, nLength, nBytesUsed);	break;
		case T_FL_LOG_ITEM_SPI:				Process_FL_LOG_ITEM_SPI(pPacket, nLength, nBytesUsed);	break;				// 2006-10-27 by cmkwon
		case T_FL_LOG_ITEM_GET_ITEM_BY_ITEMEVENT:	Process_FL_LOG_ITEM_GET_ITEM_BY_ITEMEVENT(pPacket, nLength, nBytesUsed);	break;		// 2007-01-11 by cmkwon
		case T_FL_LOG_WARPOINT:				Process_FL_LOG_WARPOINT(pPacket, nLength, nBytesUsed);	break;				// 2007-05-15 by dhjin
		case T_FL_LOG_ARENA_TEAM_MATCH:		Process_FL_LOG_ARENA_TEAM_MATCH(pPacket, nLength, nBytesUsed);		break;				// 2007-06-11 by dhjin
		case T_FL_LOG_TUTORIAL_COMPLETION:	Process_FL_LOG_TUTORIAL_COMPLETION(pPacket, nLength, nBytesUsed);		break;				// 2007-07-06 by dhjin
		case T_FL_LOG_OUTPOST_START:		Process_FL_LOG_OUTPOST_START(pPacket, nLength, nBytesUsed);		break;				// 2007-10-02 by dhjin
		case T_FL_LOG_OUTPOST_END:			Process_FL_LOG_OUTPOST_END(pPacket, nLength, nBytesUsed);		break;				// 2007-10-02 by dhjin
		case T_FL_LOG_EXPEDIENCYFUND:		Process_FL_LOG_EXPEDIENCYFUND(pPacket, nLength, nBytesUsed);		break;				// 2007-10-02 by dhjin

		case T_FL_LOG_SERVER_INFO_MAP:		Process_FL_LOG_SERVER_INFO_MAP(pPacket, nLength, nBytesUsed);			break;
		case T_FL_LOG_SERVER_INFO_TOTAL:	Process_FL_LOG_SERVER_INFO_TOTAL(pPacket, nLength, nBytesUsed);			break;
		case T_FL_LOG_HACKING_LOG:			Process_FL_LOG_HACKING_LOG(pPacket, nLength, nBytesUsed);			break;
		case T_FL_LOG_MONSTER_BOSS:			Process_FL_LOG_MONSTER_BOSS(pPacket, nLength, nBytesUsed);				break;
		case T_FL_LOG_START_FIELD_SERVER:	Process_FL_LOG_START_FIELD_SERVER(pPacket, nLength, nBytesUsed);		break;
		
		case T_FL_LOG_INFLWAR_START:				Process_FL_LOG_INFLWAR_START(pPacket, nLength, nBytesUsed);					break;	// 2007-10-16 by cmkwon, 로그 추가 -
		case T_FL_LOG_INFLWAR_END:					Process_FL_LOG_INFLWAR_END(pPacket, nLength, nBytesUsed);					break;	// 2007-10-16 by cmkwon, 로그 추가 -
		case T_FL_LOG_OUTPOSTWAR_RESET_START:		Process_FL_LOG_OUTPOSTWAR_RESET_START(pPacket, nLength, nBytesUsed);		break;	// 2007-10-16 by cmkwon, 로그 추가 -
		case T_FL_LOG_OUTPOSTWAR_RESET_DESTROY:		Process_FL_LOG_OUTPOSTWAR_RESET_DESTROY(pPacket, nLength, nBytesUsed);		break;	// 2007-10-16 by cmkwon, 로그 추가 -
		case T_FL_LOG_OUTPOSTWAR_RESET_COMPLETE:	Process_FL_LOG_OUTPOSTWAR_RESET_COMPLETE(pPacket, nLength, nBytesUsed);		break;	// 2007-10-16 by cmkwon, 로그 추가 -
		case T_FL_LOG_OUTPOSTWAR_PROTECTOR_DESTROY:	Process_FL_LOG_OUTPOSTWAR_PROTECTOR_DESTROY(pPacket, nLength, nBytesUsed);	break;	// 2007-10-16 by cmkwon, 로그 추가 -
		case T_FL_LOG_OUTPOSTWAR_SET_NEXTTIME:		Process_FL_LOG_OUTPOSTWAR_SET_NEXTTIME(pPacket, nLength, nBytesUsed);		break;	// 2007-10-16 by cmkwon, 로그 추가 -

		case T_FL_LOG_POLL_VOTE:					Process_FL_LOG_POLL_VOTE(pPacket, nLength, nBytesUsed);		break;	// 2007-11-09 by dhjin
		case T_FL_LOG_POLL_DELETE_LEADERCANDIDATE:	Process_FL_LOG_POLL_DELETE_LEADERCANDIDATE(pPacket, nLength, nBytesUsed);		break;	// 2007-11-09 by dhjin
		case T_FL_LOG_DISMEMBER_GUILD:				Process_FL_LOG_DISMEMBER_GUILD(pPacket, nLength, nBytesUsed);		break;	// 2007-11-09 by dhjin
		case T_FL_LOG_NOTIFY_MSG_DELETE:			Process_FL_LOG_NOTIFY_MSG_DELETE(pPacket, nLength, nBytesUsed);		break;	// 2007-11-28 by cmkwon, 통지시스템 구현 -

		case T_FL_LOG_ITEM_ATTACH_ITEM:				Process_FL_LOG_ITEM_ATTACH_ITEM(pPacket, nLength, nBytesUsed);		break;	// 2008-01-23 by cmkwon, S_F, S_L: 장착/장착해제 게임 로그에 추가 - 
		case T_FL_LOG_ITEM_DETACH_ITEM:				Process_FL_LOG_ITEM_DETACH_ITEM(pPacket, nLength, nBytesUsed);		break;	// 2008-01-23 by cmkwon, S_F, S_L: 장착/장착해제 게임 로그에 추가 - 
		case T_FL_LOG_ITEM_DELETE:					Process_FL_LOG_ITEM_DELETE(pPacket, nLength, nBytesUsed);			break;	// 2008-01-23 by cmkwon, S_F, S_L: 장착/장착해제 게임 로그에 추가 - 
		case T_FL_LOG_USE_COUPON:					Process_FL_LOG_USE_COUPON(pPacket, nLength, nBytesUsed);			break;	// 2008-01-23 by cmkwon, S_F, S_L: 쿠폰 사용 게임 로그에 추가 - 

		//////////////////////////////////////////////////////////////////////////
		// 2008-04-01 by dhjin, 모선전, 거점전 정보창 기획안 - 
		case T_FL_LOG_MS_WAR:						Process_FL_LOG_MS_WAR(pPacket, nLength, nBytesUsed);			break;	
		case T_FL_LOG_SP_WAR:						Process_FL_LOG_SP_WAR(pPacket, nLength, nBytesUsed);			break;	
		
		// 2008-12-23 by dhjin, 전쟁 보상 추가안		
		case T_FL_LOG_WAR_CONTRIBUTION:				Process_FL_LOG_WAR_CONTRIBUTION(pPacket, nLength, nBytesUsed);			break;				
		case T_FL_LOG_WAR_CONTRIBUTION_GEAR:		Process_FL_LOG_WAR_CONTRIBUTION_GEAR(pPacket, nLength, nBytesUsed);			break;				

		case T_FL_LOG_LUCKY_DROP:					Process_FL_LOG_LUCKY_DROP(pPacket, nLength, nBytesUsed);				break;	// 2009-03-31 by dhjin, 럭키머신 로그
		case T_FL_LOG_INFINITY_START:				Process_FL_LOG_INFINITY_START(pPacket, nLength, nBytesUsed);			break;		// 193 - // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 게임로그 추가, 시작 관련 정보 
		case T_FL_LOG_INFINITY_START_ITEM:			Process_FL_LOG_INFINITY_START_ITEM(pPacket, nLength, nBytesUsed);		break;		// 194 - // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 게임로그 추가, 시작시 아이템 복사 정보
		case T_FL_LOG_INFINITY_CINEMA:				Process_FL_LOG_INFINITY_CINEMA(pPacket, nLength, nBytesUsed);			break;		// 195 - // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 게임로그 추가, 연출 로그
		case T_FL_LOG_MONSTER_SKILL:				Process_FL_LOG_MONSTER_SKILL(pPacket, nLength, nBytesUsed);				break;		// 196 - // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 게임로그 추가, 몬스터 스킬 사용시
		case T_FL_LOG_HPACTION_TALK:				Process_FL_LOG_HPACTION_TALK(pPacket, nLength, nBytesUsed);				break;		// 197 - // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 게임로그 추가, 몬스터 대화 사용시
		case T_FL_LOG_INFINITY_FIN:					Process_FL_LOG_INFINITY_FIN(pPacket, nLength, nBytesUsed);				break;		// 198 - // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 게임로그 추가, 종료 관련 정보 
		case T_FL_LOG_INFINITY_FIN_ITEM:			Process_FL_LOG_INFINITY_FIN_ITEM(pPacket, nLength, nBytesUsed);			break;		// 199 - // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 게임로그 추가, 종료시 아이템 복사 정보
		case T_FL_LOG_INFINITY_FIN_ALIVE_KEY_MONSTER:	Process_FL_LOG_INFINITY_FIN_ALIVE_KEY_MONSTER(pPacket, nLength, nBytesUsed);	break;		// 200 - // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 게임로그 추가, 종료시 살아있는 몬스터 정보
		case T_FL_LOG_INFINITY_LEAVE_ITEM:			Process_FL_LOG_INFINITY_LEAVE_ITEM(pPacket, nLength, nBytesUsed);	break;			// 201 - // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 게임로그 추가, 인피 탈퇴시 아이템 복사 정보

		case T_FL_LOG_EVENT_PARTICIPATION_RATE:		Process_FL_LOG_EVENT_PARTICIPATION_RATE(pPacket, nLength, nBytesUsed);	break;		// 68 -  // 2010-06-01 by shcho, GLogDB 관련 -

		case T_FL_LOG_PET_LEVEL:					Process_FL_LOG_PET_LEVEL(pPacket, nLength, nBytesUsed);	break;						// 213 - // 2011-09-15 by hskim, 파트너 시스템 2차 - 파트너 레벨 변경

		case T_FL_LOG_CONNECT_USER_NEW:				Process_FL_LOG_CONNECT_USER_NEW(pPacket, nLength, nBytesUsed);		break;			// 214 - // 2012-01-08 by hskim, GLog 2차 - 유저 신규 접속
		case T_FL_LOG_CONNECT_USER_LOGIN:			Process_FL_LOG_CONNECT_USER_LOGIN(pPacket, nLength, nBytesUsed);	break;			// 215 - // 2012-01-08 by hskim, GLog 2차 - 유저 접속

		case T_FL_LOG_STATISTICS_MONEY:				Process_FL_LOG_STATISTICS_MONEY(pPacket, nLength, nBytesUsed);		break;			// 216 - // 2012-01-16 by hskim, 통계 - 화패

		case T_FL_LOG_FIXED_TERM_SHAPE_START:		Process_FL_LOG_FIXED_TERM_SHAPE_START(pPacket, nLength, nBytesUsed);		break;			// 217 - // 2012-10-10 by hskim, 기간제 속성 구현 (기간제 외형)
		case T_FL_LOG_FIXED_TERM_SHAPE_END:			Process_FL_LOG_FIXED_TERM_SHAPE_END(pPacket, nLength, nBytesUsed);		break;			// 218 - // 2012-10-10 by hskim, 기간제 속성 구현 (기간제 외형)
				
		// start 2012-11-21 by khkim, GLog (수정) 
		case T_FL_LOG_ACCOUNTCONNECT:				Process_FL_LOG_ACCOUNTCONNECT(pPacket, nLength, nBytesUsed);		break;			
		case T_FL_LOG_CHARDATA:						Process_FL_LOG_CHARDATA(pPacket, nLength, nBytesUsed);				break;  
		case T_FL_LOG_ITEM:							Process_FL_LOG_ITEM(pPacket, nLength, nBytesUsed);					break;
		case T_FL_LOG_ITEMSTATE:					Process_FL_LOG_ITEMSTATE(pPacket, nLength, nBytesUsed);				break;
		case T_FL_LOG_SERVER:						Process_FL_LOG_SERVER(pPacket, nLength, nBytesUsed);				break;
		// end 2012-11-21 by khkim, GLog

		case T_FL_LOG_COLLECTION_STATE:				Process_FL_LOG_COLLECTION_STATE(pPacket, nLength, nBytesUsed);			break;			// 226 - // 2013-05-31 by jhseol,bckim 아머 컬렉션 - GameLog 추가
		case T_FL_LOG_COLLECTION_SHAPE_CHANGE:		Process_FL_LOG_COLLECTION_SHAPE_CHANGE(pPacket, nLength, nBytesUsed);	break;			// 227 - // 2013-05-31 by jhseol,bckim 아머 컬렉션 - GameLog 추가
		case T_FL_LOG_ACCOUNT_INFL_CHANGE_BY_ADMIN:	Process_FL_LOG_ACCOUNT_INFL_CHANGE_BY_ADMIN(pPacket, nLength, nBytesUsed);	break;		// 228 - // 2013-07-26 by jhseol, 타 계정 세력변경 - GameLog 추가
		
		default:
			{
				// Protocol Error 처리(Close 처리함)
				// - Client로 부터 받은 Field Type이 유효하지 않다
				// Error Code : ERR_PROTOCOL_INVALID_PROTOCOL_TYPE

				char	szSystemLog[256];
				sprintf(szSystemLog, "[Error] CLogIOCPSocket::OnRecvdPacketFieldServer1 invalid protocol type, RecvType[%s(%#04X)] PrevType[%s(%#04X)]\r\n",
								GetProtocolTypeString(nRecvType), nRecvType,
								GetProtocolTypeString(nPrevType), nPrevType								);
				g_pLogGlobal->WriteSystemLog(szSystemLog);
				DBGOUT(szSystemLog);
				return TRUE;
			}
		}

		switch(procRes)
		{
		case RES_RETURN_FALSE:
//		case RES_PACKET_ERROR:
			return TRUE;
		}

		nPrevType = nRecvType;
	}

	return TRUE;
}

BOOL CLogIOCPSocket::Process_FL_LOG_LOGIN(const char* pPacket, int nLength, int &nBytesUsed)
{
	DECLARE_MESSAGE_AND_CHECK_SIZE_LOG(pPacket, nLength, nBytesUsed, T_FL_LOG_LOGIN,
											MSG_FL_LOG_LOGIN, msgLogin);
	///////////////////////////////////////////////////////////////////////////////
	// 2006-05-23 by cmkwon, 로그를 시스템 로그로 남긴다.
	char szGameLog[1024];
	ms_pLogIOCP->WriteGameLog(msgLogin->GetWriteLogString(T1_FL_LOG_LOGIN, szGameLog));
// 2006-09-14 by cmkwon, DB에 저장
//	return TRUE;

// 2006-05-19 by cmkwon
// 	CAtumDBLogWriter::InsertLog_Connection(&m_ODBCStmt, T1<T0::_FL_LOG>::_\1,
// 		msgLogin->IPAddress, msgLogin->AccountUniqueNumber, msgLogin->AccountName, msgLogin->ServerGroupName);
	MSG_FL_LOG_LOGIN *pMsg = new MSG_FL_LOG_LOGIN;
	*pMsg = *msgLogin;
	ms_pLogIOCP->m_pAtumLogDBManager->MakeAndEnqueueQuery(QT_FL_LOG_LOGIN, NULL, pMsg, NULL, T1_FL_LOG_LOGIN);
	return TRUE;
}

BOOL CLogIOCPSocket::Process_FL_LOG_LOGOUT(const char* pPacket, int nLength, int &nBytesUsed)
{
	DECLARE_MESSAGE_AND_CHECK_SIZE_LOG(pPacket, nLength, nBytesUsed, T_FL_LOG_LOGOUT,
											MSG_FL_LOG_LOGOUT, msgLogout);
	///////////////////////////////////////////////////////////////////////////////
	// 2006-05-23 by cmkwon, 로그를 시스템 로그로 남긴다.
	char szGameLog[1024];
	ms_pLogIOCP->WriteGameLog(msgLogout->GetWriteLogString(T1_FL_LOG_LOGOUT, szGameLog));
// 2006-09-14 by cmkwon, DB에 저장
//	return TRUE;

// 2006-05-19 by cmkwon
// 	CAtumDBLogWriter::InsertLog_Connection(&m_ODBCStmt, T1_FL_LOG_LOGOUT,
// 		msgLogout->IPAddress, msgLogout->AccountUniqueNumber, msgLogout->AccountName, msgLogout->ServerGroupName);
	MSG_FL_LOG_LOGOUT *pMsg = new MSG_FL_LOG_LOGOUT;
	*pMsg = *msgLogout;
	ms_pLogIOCP->m_pAtumLogDBManager->MakeAndEnqueueQuery(QT_FL_LOG_LOGOUT, NULL, pMsg, NULL, T1_FL_LOG_LOGOUT);
	return TRUE;
}

BOOL CLogIOCPSocket::Process_FL_LOG_LEVEL(const char* pPacket, int nLength, int &nBytesUsed)
{
	DECLARE_MESSAGE_AND_CHECK_SIZE_LOG(pPacket, nLength, nBytesUsed, T_FL_LOG_LEVEL,
											MSG_FL_LOG_LEVEL, msgLevel);
	///////////////////////////////////////////////////////////////////////////////
	// 2006-05-23 by cmkwon, 로그를 시스템 로그로 남긴다.
	char szGameLog[1024];
	ms_pLogIOCP->WriteGameLog(msgLevel->GetWriteLogString(T1_FL_LOG_LEVEL, szGameLog));
	// 2013-01-04 by jhseol, DB 풀로그 남기기
#ifdef S_DB_FULL_LOG_JHSEOL
#else
	return TRUE;
#endif	// #ifdef S_DB_FULL_LOG_JHSEOL
	// end 2013-01-04 by jhseol, DB 풀로그 남기기

// 2006-05-19 by cmkwon
// 	CAtumDBLogWriter::InsertLog_User_Level_Stat(&m_ODBCStmt, T1_FL_LOG_LEVEL,
// 		msgLevel, msgLevel->FromLevel, msgLevel->ToLevel, msgLevel->TotalPlayTime);
	MSG_FL_LOG_LEVEL *pMsg = new MSG_FL_LOG_LEVEL;
	*pMsg = *msgLevel;
	ms_pLogIOCP->m_pAtumLogDBManager->MakeAndEnqueueQuery(QT_FL_LOG_LEVEL, NULL, pMsg, NULL, T1_FL_LOG_LEVEL);
	return TRUE;
}

BOOL CLogIOCPSocket::Process_FL_LOG_EXP(const char* pPacket, int nLength, int &nBytesUsed)
{
	DECLARE_MESSAGE_AND_CHECK_SIZE_LOG(pPacket, nLength, nBytesUsed, T_FL_LOG_EXP,
											MSG_FL_LOG_EXP, msgExp);
	///////////////////////////////////////////////////////////////////////////////
	// 2006-05-23 by cmkwon, 로그를 시스템 로그로 남긴다.
	char szGameLog[1024];
	ms_pLogIOCP->WriteGameLog(msgExp->GetWriteLogString(T1_FL_LOG_EXP, szGameLog));
	// 2013-01-04 by jhseol, DB 풀로그 남기기
#ifdef S_DB_FULL_LOG_JHSEOL
#else
	return TRUE;
#endif	// #ifdef S_DB_FULL_LOG_JHSEOL
	// end 2013-01-04 by jhseol, DB 풀로그 남기기

// 2006-05-19 by cmkwon
// 	CAtumDBLogWriter::InsertLog_User_Exp(&m_ODBCStmt, T1_FL_LOG_EXP,
// 		msgExp, msgExp->AcquiredExperience, msgExp->CharacterExperiece);
	MSG_FL_LOG_EXP *pMsg = new MSG_FL_LOG_EXP;
	*pMsg = *msgExp;
	ms_pLogIOCP->m_pAtumLogDBManager->MakeAndEnqueueQuery(QT_FL_LOG_EXP, NULL, pMsg, NULL, T1_FL_LOG_EXP);
	return TRUE;
}

BOOL CLogIOCPSocket::Process_FL_LOG_STAT(const char* pPacket, int nLength, int &nBytesUsed)
{
	DECLARE_MESSAGE_AND_CHECK_SIZE_LOG(pPacket, nLength, nBytesUsed, T_FL_LOG_STAT,
											MSG_FL_LOG_STAT, msgStat);
	///////////////////////////////////////////////////////////////////////////////
	// 2006-05-23 by cmkwon, 로그를 시스템 로그로 남긴다.
	char szGameLog[1024];
	ms_pLogIOCP->WriteGameLog(msgStat->GetWriteLogString(T1_FL_LOG_STAT, szGameLog));
	// 2013-01-04 by jhseol, DB 풀로그 남기기
#ifdef S_DB_FULL_LOG_JHSEOL
#else
	return TRUE;
#endif	// #ifdef S_DB_FULL_LOG_JHSEOL
	// end 2013-01-04 by jhseol, DB 풀로그 남기기

// 2006-05-19 by cmkwon
// 	CAtumDBLogWriter::InsertLog_User_Level_Stat(&m_ODBCStmt, T1_FL_LOG_STAT,
// 		msgStat, msgStat->StatKind, msgStat->RemainedBonusStat, 0);
	MSG_FL_LOG_STAT *pMsg = new MSG_FL_LOG_STAT;
	*pMsg = *msgStat;
	ms_pLogIOCP->m_pAtumLogDBManager->MakeAndEnqueueQuery(QT_FL_LOG_STAT, NULL, pMsg, NULL, T1_FL_LOG_STAT);

	return TRUE;
}

BOOL CLogIOCPSocket::Process_FL_LOG_GAME_START(const char* pPacket, int nLength, int &nBytesUsed)
{
	DECLARE_MESSAGE_AND_CHECK_SIZE_LOG(pPacket, nLength, nBytesUsed, T_FL_LOG_GAME_START,
											MSG_FL_LOG_GAME_START, msgGameStart);
	///////////////////////////////////////////////////////////////////////////////
	// 2006-05-23 by cmkwon, 로그를 시스템 로그로 남긴다.
	char szGameLog[1024];
	ms_pLogIOCP->WriteGameLog(msgGameStart->GetWriteLogString(T1_FL_LOG_GAME_START, szGameLog));

// 2006-08-04 by dhjin, 게임 체류 시간 로그를 남긴다.
//	return TRUE; 

// 2006-05-19 by cmkwon
// 	CAtumDBLogWriter::InsertLog_User_Game_Start_End(&m_ODBCStmt, T1_FL_LOG_GAME_START,
// 		msgGameStart, msgGameStart->CharacterName, msgGameStart->AccountUniqueNumber, msgGameStart->AccountName, 0, msgGameStart->TotalPlayTime);

	MSG_FL_LOG_GAME_START *pMsg = new MSG_FL_LOG_GAME_START;
	*pMsg = *msgGameStart;
	ms_pLogIOCP->m_pAtumLogDBManager->MakeAndEnqueueQuery(QT_FL_LOG_GAME_START, NULL, pMsg, NULL, T1_FL_LOG_GAME_START);
	return TRUE;
}

BOOL CLogIOCPSocket::Process_FL_LOG_GAME_END(const char* pPacket, int nLength, int &nBytesUsed)
{
	DECLARE_MESSAGE_AND_CHECK_SIZE_LOG(pPacket, nLength, nBytesUsed, T_FL_LOG_GAME_END,
											MSG_FL_LOG_GAME_END, msgGameEnd);

	///////////////////////////////////////////////////////////////////////////////
	// 2006-05-23 by cmkwon, 로그를 시스템 로그로 남긴다.
	char szGameLog[1024];
	ms_pLogIOCP->WriteGameLog(msgGameEnd->GetWriteLogString(T1_FL_LOG_GAME_END, szGameLog));

// 2006-08-04 by dhjin, 게임 체류 시간 로그를 남긴다.
//  return TRUE;

// 2006-05-19 by cmkwon
// 	CAtumDBLogWriter::InsertLog_User_Game_Start_End(&m_ODBCStmt, T1_FL_LOG_GAME_END,
// 		msgGameEnd, msgGameEnd->CharacterName, msgGameEnd->AccountUniqueNumber, msgGameEnd->AccountName, msgGameEnd->PlayTime, msgGameEnd->TotalPlayTime);

	MSG_FL_LOG_GAME_END *pMsg = new MSG_FL_LOG_GAME_END;
	*pMsg = *msgGameEnd;
	ms_pLogIOCP->m_pAtumLogDBManager->MakeAndEnqueueQuery(QT_FL_LOG_GAME_END, NULL, pMsg, NULL, T1_FL_LOG_GAME_END);

#ifdef S_GLOG_HSSON
	// 2010-06-01 by shcho, GLogDB 관련 -
	MSG_FL_LOG_GAME_END *pGLogMsg = new MSG_FL_LOG_GAME_END;
	*pGLogMsg = *msgGameEnd;
	ms_pLogIOCP->m_pGLogDBManager->MakeAndEnqueueQuery(QT_FL_GLOG_CHARACTER_PLAY_TIME, NULL, pGLogMsg, NULL);
#endif // S_GLOG_HSSON

	// start 2011-12-12 by hskim, GLog 2차
#ifdef S_GLOG_2ND_HSKIM
	MSG_FL_LOG_GAME_END *pGLog2ndMsg = new MSG_FL_LOG_GAME_END;
	*pGLog2ndMsg = *msgGameEnd;
	ms_pLogIOCP->m_pGLogDBManager->MakeAndEnqueueQuery(QT_FL_GLOG_TB_USER_PLAYTIME, NULL, pGLog2ndMsg, NULL);
#endif
	// end 2011-12-12 by hskim, GLog 2차

	return TRUE;
}

BOOL CLogIOCPSocket::Process_FL_LOG_CREATE_CHARACTER(const char* pPacket, int nLength, int &nBytesUsed)
{
	DECLARE_MESSAGE_AND_CHECK_SIZE_LOG(pPacket, nLength, nBytesUsed, T_FL_LOG_CREATE_CHARACTER,
											MSG_FL_LOG_CREATE_CHARACTER, msgCreateCharacter);
	///////////////////////////////////////////////////////////////////////////////
	// 2006-05-23 by cmkwon, 로그를 시스템 로그로 남긴다.
	char szGameLog[1024];
	ms_pLogIOCP->WriteGameLog(msgCreateCharacter->GetWriteLogString(T1_FL_LOG_CREATE_CHARACTER, szGameLog));
	// 2013-01-04 by jhseol, DB 풀로그 남기기
#ifdef S_DB_FULL_LOG_JHSEOL
#else
	return TRUE;
#endif	// #ifdef S_DB_FULL_LOG_JHSEOL
	// end 2013-01-04 by jhseol, DB 풀로그 남기기

// 2006-05-19 by cmkwon
// 	CAtumDBLogWriter::InsertLog_User_Game_Start_End(&m_ODBCStmt, T1_FL_LOG_CREATE_CHARACTER,
// 		msgCreateCharacter, msgCreateCharacter->CharacterName, msgCreateCharacter->AccountUniqueNumber,
// 		msgCreateCharacter->AccountName, msgCreateCharacter->UnitKind, 1);
	MSG_FL_LOG_CREATE_CHARACTER *pMsg = new MSG_FL_LOG_CREATE_CHARACTER;
	*pMsg = *msgCreateCharacter;

	ms_pLogIOCP->m_pAtumLogDBManager->MakeAndEnqueueQuery(QT_FL_LOG_CREATE_CHARACTER, NULL, pMsg, NULL, T1_FL_LOG_CREATE_CHARACTER);
	return TRUE;
}

BOOL CLogIOCPSocket::Process_FL_LOG_DELETE_CHARACTER(const char* pPacket, int nLength, int &nBytesUsed)
{
	DECLARE_MESSAGE_AND_CHECK_SIZE_LOG(pPacket, nLength, nBytesUsed, T_FL_LOG_DELETE_CHARACTER,
											MSG_FL_LOG_DELETE_CHARACTER, msgDeleteCharacter);

	///////////////////////////////////////////////////////////////////////////////
	// 2006-05-23 by cmkwon, 로그를 시스템 로그로 남긴다.
	char szGameLog[1024];
	ms_pLogIOCP->WriteGameLog(msgDeleteCharacter->GetWriteLogString(T1_FL_LOG_DELETE_CHARACTER, szGameLog));

	// start 2011-12-12 by hskim, GLog 2차
#ifdef S_GLOG_2ND_HSKIM
	QPARAM_GLOG_USER_INFO_COUNT_INC *pGLog2ndMsg = new QPARAM_GLOG_USER_INFO_COUNT_INC;
	util::strncpy(pGLog2ndMsg->szAccountName, msgDeleteCharacter->AccountName, SIZE_MAX_ACCOUNT_NAME);
	ms_pLogIOCP->m_pGLogDBManager->MakeAndEnqueueQuery(QT_FL_GLOG_TB_USER_LocalUserConnectInfo_DelCountInc, NULL, pGLog2ndMsg, NULL);
#endif
	// end 2011-12-12 by hskim, GLog 2차

	// 2013-01-04 by jhseol, DB 풀로그 남기기
#ifdef S_DB_FULL_LOG_JHSEOL
#else
	return TRUE;
#endif	// #ifdef S_DB_FULL_LOG_JHSEOL
	// end 2013-01-04 by jhseol, DB 풀로그 남기기

// 2006-05-19 by cmkwon
// 	CAtumDBLogWriter::InsertLog_User_Game_Start_End(&m_ODBCStmt, T1_FL_LOG_DELETE_CHARACTER,
// 		msgDeleteCharacter, msgDeleteCharacter->CharacterName, msgDeleteCharacter->AccountUniqueNumber,
// 		msgDeleteCharacter->AccountName, msgDeleteCharacter->UnitKind, msgDeleteCharacter->Level);
	MSG_FL_LOG_DELETE_CHARACTER *pMsg = new MSG_FL_LOG_DELETE_CHARACTER;
	*pMsg = *msgDeleteCharacter;
	ms_pLogIOCP->m_pAtumLogDBManager->MakeAndEnqueueQuery(QT_FL_LOG_DELETE_CHARACTER, NULL, pMsg, NULL, T1_FL_LOG_DELETE_CHARACTER);

	return TRUE;
}


BOOL CLogIOCPSocket::Process_FL_LOG_QUEST_COMPLETION(const char* pPacket, int nLength, int &nBytesUsed)
{
	DECLARE_MESSAGE_AND_CHECK_SIZE_LOG(pPacket, nLength, nBytesUsed, T_FL_LOG_QUEST_COMPLETION,
											MSG_FL_LOG_QUEST_COMPLETION, msgQuest);
	///////////////////////////////////////////////////////////////////////////////
	// 2006-05-23 by cmkwon, 로그를 시스템 로그로 남긴다.
	char szGameLog[1024];
	ms_pLogIOCP->WriteGameLog(msgQuest->GetWriteLogString(T1_FL_LOG_QUEST_COMPLETION, szGameLog));
	// 2013-01-04 by jhseol, DB 풀로그 남기기
#ifdef S_DB_FULL_LOG_JHSEOL
#else
	return TRUE;
#endif	// #ifdef S_DB_FULL_LOG_JHSEOL
	// end 2013-01-04 by jhseol, DB 풀로그 남기기

// 2006-05-19 by cmkwon
// 	CAtumDBLogWriter::InsertLog_User_Quest_PK_Dead(&m_ODBCStmt, T1_FL_LOG_QUEST_COMPLETION,
// 		msgQuest, msgQuest->QuestIndex);
	MSG_FL_LOG_QUEST_COMPLETION *pMsg = new MSG_FL_LOG_QUEST_COMPLETION;
	*pMsg = *msgQuest;
	ms_pLogIOCP->m_pAtumLogDBManager->MakeAndEnqueueQuery(QT_FL_LOG_QUEST_COMPLETION, NULL, pMsg, NULL, T1_FL_LOG_QUEST_COMPLETION);
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CLogIOCPSocket::Process_FL_LOG_QUEST_COMPENSATION(const char* pPacket, int nLength, int &nBytesUsed)
/// \brief		퀘스트 완료 보상 게임 로그에 남기기
/// \author		dhjin
/// \date		2007-01-16 ~ 2007-01-16
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CLogIOCPSocket::Process_FL_LOG_QUEST_COMPENSATION(const char* pPacket, int nLength, int &nBytesUsed)
{
	DECLARE_MESSAGE_AND_CHECK_SIZE_LOG(pPacket, nLength, nBytesUsed, T_FL_LOG_QUEST_COMPENSATION,
											MSG_FL_LOG_QUEST_COMPENSATION, msgQuestCompensation);
	nBytesUsed += msgQuestCompensation->LenOfCompensation;

	///////////////////////////////////////////////////////////////////////////////
	// 2006-05-23 by cmkwon, 로그를 시스템 로그로 남긴다.
	char szGameLog[2048];
	char szQuestCompensationItem[1024];		
	util::zero(szQuestCompensationItem, 1024);
	if(0 < msgQuestCompensation->LenOfCompensation)
	{
		util::strncpy(szQuestCompensationItem, (char*)msgQuestCompensation + sizeof(MSG_FL_LOG_QUEST_COMPENSATION), 500);
	}
	ms_pLogIOCP->WriteGameLog(msgQuestCompensation->GetWriteLogString(T1_FL_LOG_QUEST_COMPENSATION, szGameLog, szQuestCompensationItem));

	return TRUE;
}

BOOL CLogIOCPSocket::Process_FL_LOG_PKWIN(const char* pPacket, int nLength, int &nBytesUsed)
{
	DECLARE_MESSAGE_AND_CHECK_SIZE_LOG(pPacket, nLength, nBytesUsed, T_FL_LOG_PKWIN,
											MSG_FL_LOG_PKWIN, msgPKWin);
	///////////////////////////////////////////////////////////////////////////////
	// 2006-05-23 by cmkwon, 로그를 시스템 로그로 남긴다.
	char szGameLog[1024];
	ms_pLogIOCP->WriteGameLog(msgPKWin->GetWriteLogString(T1_FL_LOG_PKWIN, szGameLog));
	// 2013-01-04 by jhseol, DB 풀로그 남기기
#ifdef S_DB_FULL_LOG_JHSEOL
#else
	return TRUE;
#endif	// #ifdef S_DB_FULL_LOG_JHSEOL
	// end 2013-01-04 by jhseol, DB 풀로그 남기기

// 2006-05-19 by cmkwon
// 	CAtumDBLogWriter::InsertLog_User_Quest_PK_Dead(&m_ODBCStmt, T1_FL_LOG_PKWIN,
// 		msgPKWin, msgPKWin->PeerCharacterUniqueNumber);
	MSG_FL_LOG_PKWIN *pMsg = new MSG_FL_LOG_PKWIN;
	*pMsg = *msgPKWin;
	ms_pLogIOCP->m_pAtumLogDBManager->MakeAndEnqueueQuery(QT_FL_LOG_PKWIN, NULL, pMsg, NULL, T1_FL_LOG_PKWIN);
	return TRUE;
}

BOOL CLogIOCPSocket::Process_FL_LOG_PKLOSS(const char* pPacket, int nLength, int &nBytesUsed)
{
	DECLARE_MESSAGE_AND_CHECK_SIZE_LOG(pPacket, nLength, nBytesUsed, T_FL_LOG_PKLOSS,
											MSG_FL_LOG_PKLOSS, msgPKLoss);
	///////////////////////////////////////////////////////////////////////////////
	// 2006-05-23 by cmkwon, 로그를 시스템 로그로 남긴다.
	char szGameLog[1024];
	ms_pLogIOCP->WriteGameLog(msgPKLoss->GetWriteLogString(T1_FL_LOG_PKLOSS, szGameLog));
	// 2013-01-04 by jhseol, DB 풀로그 남기기
#ifdef S_DB_FULL_LOG_JHSEOL
#else
	return TRUE;
#endif	// #ifdef S_DB_FULL_LOG_JHSEOL
	// end 2013-01-04 by jhseol, DB 풀로그 남기기

// 2006-05-19 by cmkwon
// 	CAtumDBLogWriter::InsertLog_User_Quest_PK_Dead(&m_ODBCStmt, T1_FL_LOG_PKLOSS,
// 		msgPKLoss, msgPKLoss->PeerCharacterUniqueNumber);
	MSG_FL_LOG_PKLOSS *pMsg = new MSG_FL_LOG_PKLOSS;
	*pMsg = *msgPKLoss;
	ms_pLogIOCP->m_pAtumLogDBManager->MakeAndEnqueueQuery(QT_FL_LOG_PKLOSS, NULL, pMsg, NULL, T1_FL_LOG_PKLOSS);
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CLogIOCPSocket::Process_FL_LOG_PvEWIN(const char* pPacket, int nLength, int &nBytesUsed)
/// \brief		
/// \author		cmkwon
/// \date		2006-03-06 ~ 2006-03-06
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CLogIOCPSocket::Process_FL_LOG_PvEWIN(const char* pPacket, int nLength, int &nBytesUsed)
{
	DECLARE_MESSAGE_AND_CHECK_SIZE_LOG(pPacket, nLength, nBytesUsed, T_FL_LOG_PvEWIN,
											MSG_FL_LOG_PvEWIN, msgPKWin);
	///////////////////////////////////////////////////////////////////////////////
	// 2006-05-23 by cmkwon, 로그를 시스템 로그로 남긴다.
	char szGameLog[1024];
	ms_pLogIOCP->WriteGameLog(msgPKWin->GetWriteLogString(T1_FL_LOG_PvEWIN, szGameLog));
	// 2013-01-04 by jhseol, DB 풀로그 남기기
#ifdef S_DB_FULL_LOG_JHSEOL
#else
	return TRUE;
#endif	// #ifdef S_DB_FULL_LOG_JHSEOL
	// end 2013-01-04 by jhseol, DB 풀로그 남기기

// 2006-05-19 by cmkwon
// 	CAtumDBLogWriter::InsertLog_User_Quest_PK_Dead(&m_ODBCStmt, T1_FL_LOG_PvEWIN,
// 		msgPKWin, msgPKWin->PeerCharacterUniqueNumber);
	MSG_FL_LOG_PvEWIN *pMsg = new MSG_FL_LOG_PvEWIN;
	*pMsg = *msgPKWin;
	ms_pLogIOCP->m_pAtumLogDBManager->MakeAndEnqueueQuery(QT_FL_LOG_PvEWIN, NULL, pMsg, NULL, T1_FL_LOG_PvEWIN);
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CLogIOCPSocket::Process_FL_LOG_PvELOSS(const char* pPacket, int nLength, int &nBytesUsed)
/// \brief		
/// \author		cmkwon
/// \date		2006-03-06 ~ 2006-03-06
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CLogIOCPSocket::Process_FL_LOG_PvELOSS(const char* pPacket, int nLength, int &nBytesUsed)
{
	DECLARE_MESSAGE_AND_CHECK_SIZE_LOG(pPacket, nLength, nBytesUsed, T_FL_LOG_PvELOSS,
											MSG_FL_LOG_PvELOSS, msgPKLoss);
	///////////////////////////////////////////////////////////////////////////////
	// 2006-05-23 by cmkwon, 로그를 시스템 로그로 남긴다.
	char szGameLog[1024];
	ms_pLogIOCP->WriteGameLog(msgPKLoss->GetWriteLogString(T1_FL_LOG_PvELOSS, szGameLog));
	// 2013-01-04 by jhseol, DB 풀로그 남기기
#ifdef S_DB_FULL_LOG_JHSEOL
#else
	return TRUE;
#endif	// #ifdef S_DB_FULL_LOG_JHSEOL
	// end 2013-01-04 by jhseol, DB 풀로그 남기기

// 2006-05-19 by cmkwon 
// 	CAtumDBLogWriter::InsertLog_User_Quest_PK_Dead(&m_ODBCStmt, T1_FL_LOG_PvELOSS,
//		msgPKLoss, msgPKLoss->PeerCharacterUniqueNumber);
	MSG_FL_LOG_PvELOSS *pMsg = new MSG_FL_LOG_PvELOSS;
	*pMsg = *msgPKLoss;
	ms_pLogIOCP->m_pAtumLogDBManager->MakeAndEnqueueQuery(QT_FL_LOG_PvELOSS, NULL, pMsg, NULL, T1_FL_LOG_PvELOSS);
	return TRUE;
}
BOOL CLogIOCPSocket::Process_FL_LOG_DEAD(const char* pPacket, int nLength, int &nBytesUsed)
{
	DECLARE_MESSAGE_AND_CHECK_SIZE_LOG(pPacket, nLength, nBytesUsed, T_FL_LOG_DEAD,
											MSG_FL_LOG_DEAD, msgDead);
	///////////////////////////////////////////////////////////////////////////////
	// 2006-05-23 by cmkwon, 로그를 시스템 로그로 남긴다.
	char szGameLog[1024];
	ms_pLogIOCP->WriteGameLog(msgDead->GetWriteLogString(T1_FL_LOG_DEAD, szGameLog));
	// 2013-01-04 by jhseol, DB 풀로그 남기기
#ifdef S_DB_FULL_LOG_JHSEOL
#else
	return TRUE;
#endif	// #ifdef S_DB_FULL_LOG_JHSEOL
	// end 2013-01-04 by jhseol, DB 풀로그 남기기

// 2006-05-19 by cmkwon
// 	CAtumDBLogWriter::InsertLog_User_Quest_PK_Dead(&m_ODBCStmt, T1_FL_LOG_DEAD,
// 		msgDead, msgDead->DamageKind);
	MSG_FL_LOG_DEAD *pMsg = new MSG_FL_LOG_DEAD;
	*pMsg = *msgDead;
	ms_pLogIOCP->m_pAtumLogDBManager->MakeAndEnqueueQuery(QT_FL_LOG_DEAD, NULL, pMsg, NULL, T1_FL_LOG_DEAD);
	return TRUE;
}

BOOL CLogIOCPSocket::Process_FL_LOG_WARP(const char* pPacket, int nLength, int &nBytesUsed)
{
	DECLARE_MESSAGE_AND_CHECK_SIZE_LOG(pPacket, nLength, nBytesUsed, T_FL_LOG_WARP,
											MSG_FL_LOG_WARP, msgWarp);
	///////////////////////////////////////////////////////////////////////////////
	// 2006-05-23 by cmkwon, 로그를 시스템 로그로 남긴다.
	char szGameLog[1024];
	ms_pLogIOCP->WriteGameLog(msgWarp->GetWriteLogString(T1_FL_LOG_WARP, szGameLog));
	// 2013-01-04 by jhseol, DB 풀로그 남기기
#ifdef S_DB_FULL_LOG_JHSEOL
#else
	return TRUE;
#endif	// #ifdef S_DB_FULL_LOG_JHSEOL
	// end 2013-01-04 by jhseol, DB 풀로그 남기기

// 2006-05-19 by cmkwon
// 	CAtumDBLogWriter::InsertLog_User_WARP(&m_ODBCStmt, T1_FL_LOG_WARP,
// 		msgWarp, msgWarp->TargetMapIndex, msgWarp->TargetPosition);
	MSG_FL_LOG_WARP *pMsg = new MSG_FL_LOG_WARP;
	*pMsg = *msgWarp;
	ms_pLogIOCP->m_pAtumLogDBManager->MakeAndEnqueueQuery(QT_FL_LOG_WARP, NULL, pMsg, NULL, T1_FL_LOG_WARP);
	return TRUE;
}

BOOL CLogIOCPSocket::Process_FL_LOG_ITEM_CHARACTER_ITEM(const char* pPacket, int nLength, int &nBytesUsed)
{
	DECLARE_MESSAGE_AND_CHECK_SIZE_LOG(pPacket, nLength, nBytesUsed, T_FL_LOG_ITEM_CHARACTER_ITEM,
											MSG_FL_LOG_ITEM_CHARACTER_ITEM, msgCharac);
	///////////////////////////////////////////////////////////////////////////////
	// 2006-05-23 by cmkwon, 로그를 시스템 로그로 남긴다.
	char szGameLog[1024];
	ms_pLogIOCP->WriteGameLog(msgCharac->GetWriteLogString(T1_FL_LOG_ITEM_CHARACTER_ITEM, szGameLog));
	// 2013-01-04 by jhseol, DB 풀로그 남기기
#ifdef S_DB_FULL_LOG_JHSEOL
#else
	return TRUE;
#endif	// #ifdef S_DB_FULL_LOG_JHSEOL
	// end 2013-01-04 by jhseol, DB 풀로그 남기기

// 2006-05-19 by cmkwon
// 	CAtumDBLogWriter::InsertLog_Item_Charac_Use(&m_ODBCStmt, T1_FL_LOG_ITEM_CHARACTER_ITEM,
// 		msgCharac, &msgCharac->Item4Log);
	MSG_FL_LOG_ITEM_CHARACTER_ITEM *pMsg = new MSG_FL_LOG_ITEM_CHARACTER_ITEM;
	*pMsg = *msgCharac;
	ms_pLogIOCP->m_pAtumLogDBManager->MakeAndEnqueueQuery(QT_FL_LOG_ITEM_CHARACTER_ITEM, NULL, pMsg, NULL, T1_FL_LOG_ITEM_CHARACTER_ITEM);
	return TRUE;
}

// 2012-12-17 by bckim, 길드창고 아이템 이동 관련 추가로그 
BOOL CLogIOCPSocket::Process_FL_LOG_ITEM_CHARACTER_ITEM_GUILDSTORE_RESULT(const char* pPacket, int nLength, int &nBytesUsed)
{
	DECLARE_MESSAGE_AND_CHECK_SIZE_LOG(pPacket, nLength, nBytesUsed, T_FL_LOG_ITEM_CHARACTER_ITEM_GUILDSTORE_RESULT,
		MSG_FL_LOG_ITEM_CHARACTER_ITEM_GUILDSTORE_RESULT, msgCharac);

	char szGameLog[1024];
	ms_pLogIOCP->WriteGameLog(msgCharac->GetWriteLogString(T1_FL_LOG_ITEM_CHARACTER_ITEM_GUILDSTORE_RESULT, szGameLog));
	return TRUE;
}
// 2012-12-17 by bckim, 길드창고 아이템 이동 관련 추가로그.End


BOOL CLogIOCPSocket::Process_FL_LOG_ITEM_STORE_ITEM(const char* pPacket, int nLength, int &nBytesUsed)
{
	DECLARE_MESSAGE_AND_CHECK_SIZE_LOG(pPacket, nLength, nBytesUsed, T_FL_LOG_ITEM_STORE_ITEM,
											MSG_FL_LOG_ITEM_STORE_ITEM, msgStoreItem);
	///////////////////////////////////////////////////////////////////////////////
	// 2006-05-23 by cmkwon, 로그를 시스템 로그로 남긴다.
	char szGameLog[1024];
	ms_pLogIOCP->WriteGameLog(msgStoreItem->GetWriteLogString(T1_FL_LOG_ITEM_STORE_ITEM, szGameLog));
	// 2013-01-04 by jhseol, DB 풀로그 남기기
#ifdef S_DB_FULL_LOG_JHSEOL
#else
	return TRUE;
#endif	// #ifdef S_DB_FULL_LOG_JHSEOL
	// end 2013-01-04 by jhseol, DB 풀로그 남기기

// 2006-05-19 by cmkwon
// 	CAtumDBLogWriter::InsertLog_Item_Charac_Use(&m_ODBCStmt, T1_FL_LOG_ITEM_STORE_ITEM,
// 		msgStoreItem, &msgStoreItem->Item4Log);
	MSG_FL_LOG_ITEM_STORE_ITEM *pMsg = new MSG_FL_LOG_ITEM_STORE_ITEM;
	*pMsg = *msgStoreItem;
	ms_pLogIOCP->m_pAtumLogDBManager->MakeAndEnqueueQuery(QT_FL_LOG_ITEM_STORE_ITEM, NULL, pMsg, NULL, T1_FL_LOG_ITEM_STORE_ITEM);

	return TRUE;
}

BOOL CLogIOCPSocket::Process_FL_LOG_ITEM_USE_MIX(const char* pPacket, int nLength, int &nBytesUsed)
{
	DECLARE_MESSAGE_AND_CHECK_SIZE_LOG(pPacket, nLength, nBytesUsed, T_FL_LOG_ITEM_USE_MIX,
											MSG_FL_LOG_ITEM_USE_MIX, pRLogMix);
	///////////////////////////////////////////////////////////////////////////////
	// 2006-05-23 by cmkwon, 로그를 시스템 로그로 남긴다.
	char szGameLog[1024];
	ms_pLogIOCP->WriteGameLog(pRLogMix->GetWriteLogString(T1_FL_LOG_ITEM_USE_MIX, szGameLog));
	// 2013-01-04 by jhseol, DB 풀로그 남기기
#ifdef S_DB_FULL_LOG_JHSEOL
#else
	return TRUE;
#endif	// #ifdef S_DB_FULL_LOG_JHSEOL
	// end 2013-01-04 by jhseol, DB 풀로그 남기기

// 2006-05-19 by cmkwon
// 	CAtumDBLogWriter::InsertLog_Item_Charac_Use(&m_ODBCStmt, T1_FL_LOG_ITEM_USE_MIX,
// 		pRLogMix, &pRLogMix->Item4Log);
	MSG_FL_LOG_ITEM_USE_MIX *pMsg = new MSG_FL_LOG_ITEM_USE_MIX;
	*pMsg = *pRLogMix;
	ms_pLogIOCP->m_pAtumLogDBManager->MakeAndEnqueueQuery(QT_FL_LOG_ITEM_USE_MIX, NULL, pMsg, NULL, T1_FL_LOG_ITEM_USE_MIX);
	return TRUE;
}
BOOL CLogIOCPSocket::Process_FL_LOG_ITEM_USE_CHANGE_CHARACTER_NAME_ORIGIN(const char* pPacket, int nLength, int &nBytesUsed)
{
	DECLARE_MESSAGE_AND_CHECK_SIZE_LOG(pPacket, nLength, nBytesUsed, T_FL_LOG_ITEM_USE_CHANGE_CHARACTER_NAME_ORIGIN,
											MSG_FL_LOG_ITEM_USE_CHANGE_CHARACTER_NAME_ORIGIN, pRLogChangeCharName);
	///////////////////////////////////////////////////////////////////////////////
	// 2006-05-23 by cmkwon, 로그를 시스템 로그로 남긴다.
	char szGameLog[1024];
	ms_pLogIOCP->WriteGameLog(pRLogChangeCharName->GetWriteLogString(T1_FL_LOG_ITEM_USE_CHANGE_CHARACTER_NAME_ORIGIN, szGameLog));
	// 2013-01-04 by jhseol, DB 풀로그 남기기
#ifdef S_DB_FULL_LOG_JHSEOL
#else
	return TRUE;
#endif	// #ifdef S_DB_FULL_LOG_JHSEOL
	// end 2013-01-04 by jhseol, DB 풀로그 남기기

// 2006-05-19 by cmkwon
// 	CAtumDBLogWriter::InsertLog_Item_Trade(&m_ODBCStmt, T1_FL_LOG_ITEM_USE_CHANGE_CHARACTER_NAME_ORIGIN,
// 		pRLogChangeCharName, &pRLogChangeCharName->Item4Log, 0, pRLogChangeCharName->szOriginCharName, pRLogChangeCharName->CharacterUniqueNumber);
// 	CAtumDBLogWriter::InsertLog_Item_Trade(&m_ODBCStmt, T1_FL_LOG_ITEM_USE_CHANGE_CHARACTER_NAME_NEW,
// 		pRLogChangeCharName, &pRLogChangeCharName->Item4Log, 0, pRLogChangeCharName->szNewCharName, pRLogChangeCharName->CharacterUniqueNumber);
	MSG_FL_LOG_ITEM_USE_CHANGE_CHARACTER_NAME_ORIGIN *pMsg = new MSG_FL_LOG_ITEM_USE_CHANGE_CHARACTER_NAME_ORIGIN;
	*pMsg = *pRLogChangeCharName;
	ms_pLogIOCP->m_pAtumLogDBManager->MakeAndEnqueueQuery(QT_FL_LOG_ITEM_USE_CHANGE_CHARACTER_NAME_ORIGIN, NULL, pMsg, NULL, T1_FL_LOG_ITEM_USE_CHANGE_CHARACTER_NAME_ORIGIN);
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CLogIOCPSocket::Process_FL_LOG_ITEM_BAZAAR_SEND(const char* pPacket, int nLength, int &nBytesUsed)
/// \brief		
/// \author		cmkwon
/// \date		2006-07-27 ~ 2006-07-27
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CLogIOCPSocket::Process_FL_LOG_ITEM_BAZAAR_SEND(const char* pPacket, int nLength, int &nBytesUsed)
{
	DECLARE_MESSAGE_AND_CHECK_SIZE_LOG(pPacket, nLength, nBytesUsed, T_FL_LOG_ITEM_BAZAAR_SEND,
											MSG_FL_LOG_ITEM_BAZAAR_SEND, pRMsg);
	///////////////////////////////////////////////////////////////////////////////
	// 2006-05-23 by cmkwon, 로그를 시스템 로그로 남긴다.
	char szGameLog[1024];
	ms_pLogIOCP->WriteGameLog(pRMsg->GetWriteLogString(T1_FL_LOG_ITEM_BAZAAR_SEND, szGameLog));

	///////////////////////////////////////////////////////////////////////////////
	// 2006-07-27 by cmkwon, DB에 저장
	MSG_FL_LOG_ITEM_TRADE_SEND *pMsg = new MSG_FL_LOG_ITEM_TRADE_SEND;
	*pMsg = *pRMsg;
	ms_pLogIOCP->m_pAtumLogDBManager->MakeAndEnqueueQuery(QT_FL_LOG_ITEM_TRADE_SEND, NULL, pMsg, NULL, T1_FL_LOG_ITEM_BAZAAR_SEND);

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CLogIOCPSocket::Process_FL_LOG_ITEM_BAZAAR_RECV(const char* pPacket, int nLength, int &nBytesUsed)
/// \brief		
/// \author		cmkwon
/// \date		2006-07-27 ~ 2006-07-27
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CLogIOCPSocket::Process_FL_LOG_ITEM_BAZAAR_RECV(const char* pPacket, int nLength, int &nBytesUsed)
{
	DECLARE_MESSAGE_AND_CHECK_SIZE_LOG(pPacket, nLength, nBytesUsed, T_FL_LOG_ITEM_BAZAAR_RECV,
											MSG_FL_LOG_ITEM_BAZAAR_RECV, pRMsg);
	///////////////////////////////////////////////////////////////////////////////
	// 2006-05-23 by cmkwon, 로그를 시스템 로그로 남긴다.
	char szGameLog[1024];
	ms_pLogIOCP->WriteGameLog(pRMsg->GetWriteLogString(T1_FL_LOG_ITEM_BAZAAR_RECV, szGameLog));

	///////////////////////////////////////////////////////////////////////////////
	// 2006-07-27 by cmkwon, DB에 저장
	MSG_FL_LOG_ITEM_TRADE_RECV *pMsg = new MSG_FL_LOG_ITEM_TRADE_RECV;
	*pMsg = *pRMsg;
	ms_pLogIOCP->m_pAtumLogDBManager->MakeAndEnqueueQuery(QT_FL_LOG_ITEM_TRADE_RECV, NULL, pMsg, NULL, T1_FL_LOG_ITEM_BAZAAR_RECV);

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CLogIOCPSocket::Process_FL_LOG_ITEM_INSERT_TO_GUILDSTORE(const char* pPacket, int nLength, int &nBytesUsed)
/// \brief		
/// \author		dhjin
/// \date		2006-09-27 ~ 2006-09-27
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CLogIOCPSocket::Process_FL_LOG_ITEM_INSERT_TO_GUILDSTORE(const char* pPacket, int nLength, int &nBytesUsed)
{
	DECLARE_MESSAGE_AND_CHECK_SIZE_LOG(pPacket, nLength, nBytesUsed, T_FL_LOG_ITEM_INSERT_TO_GUILDSTORE,
											MSG_FL_LOG_GUILD_STORE, pRMsg);

	///////////////////////////////////////////////////////////////////////////////
	// 2006-09-28 by cmkwon, 로그를 시스템 로그로 남긴다.
	char szGameLog[1024];
	ms_pLogIOCP->WriteGameLog(pRMsg->GetWriteLogString(T1_FL_LOG_ITEM_INSERT_TO_GUILDSTORE, szGameLog));
	
	MSG_FL_LOG_GUILD_STORE *pMsg = new MSG_FL_LOG_GUILD_STORE;
	*pMsg = *pRMsg;
	ms_pLogIOCP->m_pAtumLogDBManager->MakeAndEnqueueQuery(QT_FL_LOG_GUILD_STORE, NULL, pMsg, NULL, T1_FL_LOG_ITEM_INSERT_TO_GUILDSTORE);

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL Process_FL_LOG_ITEM_DRAW_FROM_GUILDSTORE(const char* pPacket, int nLength, int &nBytesUsed);
/// \brief		
/// \author		dhjin
/// \date		2006-09-27 ~ 2006-09-27
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CLogIOCPSocket::Process_FL_LOG_ITEM_DRAW_FROM_GUILDSTORE(const char* pPacket, int nLength, int &nBytesUsed)
{
	DECLARE_MESSAGE_AND_CHECK_SIZE_LOG(pPacket, nLength, nBytesUsed, T_FL_LOG_ITEM_DRAW_FROM_GUILDSTORE,
											MSG_FL_LOG_GUILD_STORE, pRMsg);

	///////////////////////////////////////////////////////////////////////////////
	// 2006-09-28 by cmkwon, 로그를 시스템 로그로 남긴다.
	char szGameLog[1024];
	ms_pLogIOCP->WriteGameLog(pRMsg->GetWriteLogString(T1_FL_LOG_ITEM_DRAW_FROM_GUILDSTORE, szGameLog));

	MSG_FL_LOG_GUILD_STORE *pMsg = new MSG_FL_LOG_GUILD_STORE;
	*pMsg = *pRMsg;
	ms_pLogIOCP->m_pAtumLogDBManager->MakeAndEnqueueQuery(QT_FL_LOG_GUILD_STORE, NULL, pMsg, NULL, T1_FL_LOG_ITEM_DRAW_FROM_GUILDSTORE);

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CLogIOCPSocket::Process_FL_LOG_ITEM_SPI(const char* pPacket, int nLength, int &nBytesUsed)
/// \brief		
/// \author		cmkwon
/// \date		2006-10-27 ~ 2006-10-27
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CLogIOCPSocket::Process_FL_LOG_ITEM_SPI(const char* pPacket, int nLength, int &nBytesUsed)
{
	DECLARE_MESSAGE_AND_CHECK_SIZE_LOG(pPacket, nLength, nBytesUsed, T_FL_LOG_ITEM_SPI,
											MSG_FL_LOG_ITEM_SPI, pRMsg);

	///////////////////////////////////////////////////////////////////////////////
	// 2006-10-27 by cmkwon, 로그를 시스템 로그로 남긴다.
	char szGameLog[1024];
	ms_pLogIOCP->WriteGameLog(pRMsg->GetWriteLogString(T1_FL_LOG_ITEM_SPI, szGameLog));

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CLogIOCPSocket::Process_FL_LOG_ITEM_GET_ITEM_BY_ITEMEVENT(const char* pPacket, int nLength, int &nBytesUsed)
/// \brief		
/// \author		cmkwon
/// \date		2007-01-11 ~ 2007-01-11
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CLogIOCPSocket::Process_FL_LOG_ITEM_GET_ITEM_BY_ITEMEVENT(const char* pPacket, int nLength, int &nBytesUsed)
{
	DECLARE_MESSAGE_AND_CHECK_SIZE_LOG(pPacket, nLength, nBytesUsed, T_FL_LOG_ITEM_GET_ITEM_BY_ITEMEVENT,
											MSG_FL_LOG_ITEM_GET_ITEM_BY_ITEMEVENT, pRMsg);

	///////////////////////////////////////////////////////////////////////////////
	// 2007-01-11 by cmkwon, 로그를 시스템 로그로 남긴다.
	char szGameLog[1024];
	ms_pLogIOCP->WriteGameLog(pRMsg->GetWriteLogString(T1_FL_LOG_ITEM_GET_ITEM_BY_ITEMEVENT, szGameLog));

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CLogIOCPSocket::Process_FL_LOG_WARPOINT(const char* pPacket, int nLength, int &nBytesUsed)	
/// \brief		
/// \author		dhjin
/// \date		2007-05-15 ~ 2007-05-15
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CLogIOCPSocket::Process_FL_LOG_WARPOINT(const char* pPacket, int nLength, int &nBytesUsed)
{
	DECLARE_MESSAGE_AND_CHECK_SIZE_LOG(pPacket, nLength, nBytesUsed, T_FL_LOG_WARPOINT,
											MSG_FL_LOG_WARPOINT, msgGet);

	char szGameLog[1024];
	ms_pLogIOCP->WriteGameLog(msgGet->GetWriteLogString(T1_FL_LOG_WARPOINT, szGameLog));
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CLogIOCPSocket::Process_FL_LOG_ARENA_TEAM_MATCH(const char* pPacket, int nLength, int &nBytesUsed)
/// \brief		
/// \author		dhjin
/// \date		2007-06-11 ~ 2007-06-11
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CLogIOCPSocket::Process_FL_LOG_ARENA_TEAM_MATCH(const char* pPacket, int nLength, int &nBytesUsed)
{
	DECLARE_MESSAGE_AND_CHECK_SIZE_LOG(pPacket, nLength, nBytesUsed, T_FL_LOG_ARENA_TEAM_MATCH,
											MSG_FL_LOG_ARENA_TEAM_MATCH, msgGet);

	char szGameLog[1024];
	ms_pLogIOCP->WriteGameLog(msgGet->GetWriteLogString(T1_FL_LOG_ARENA_TEAM_MATCH, szGameLog));
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CLogIOCPSocket::Process_FL_LOG_TUTORIAL_COMPLETION(const char* pPacket, int nLength, int &nBytesUsed)
/// \brief		
/// \author		dhjin
/// \date		2007-07-06 ~ 2007-07-06
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CLogIOCPSocket::Process_FL_LOG_TUTORIAL_COMPLETION(const char* pPacket, int nLength, int &nBytesUsed)
{
	DECLARE_MESSAGE_AND_CHECK_SIZE_LOG(pPacket, nLength, nBytesUsed, T_FL_LOG_TUTORIAL_COMPLETION,
											MSG_FL_LOG_TUTORIAL_COMPLETION, msgGet);

	char szGameLog[1024];
	ms_pLogIOCP->WriteGameLog(msgGet->GetWriteLogString(T1_FL_LOG_TUTORIAL_COMPLETION, szGameLog));
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CLogIOCPSocket::Process_FL_LOG_OUTPOST_START(const char* pPacket, int nLength, int &nBytesUsed)
/// \brief		
/// \author		dhjin
/// \date		2007-10-02 ~ 2007-10-02
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CLogIOCPSocket::Process_FL_LOG_OUTPOST_START(const char* pPacket, int nLength, int &nBytesUsed)
{
	DECLARE_MESSAGE_AND_CHECK_SIZE_LOG(pPacket, nLength, nBytesUsed, T_FL_LOG_OUTPOST_START,
											MSG_FL_LOG_OUTPOST_START, msgGet);

	char szGameLog[1024];
	ms_pLogIOCP->WriteGameLog(msgGet->GetWriteLogString(T1_FL_LOG_OUTPOST_START, szGameLog));
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CLogIOCPSocket::Process_FL_LOG_OUTPOST_END(const char* pPacket, int nLength, int &nBytesUsed)
/// \brief		
/// \author		dhjin
/// \date		2007-10-02 ~ 2007-10-02
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CLogIOCPSocket::Process_FL_LOG_OUTPOST_END(const char* pPacket, int nLength, int &nBytesUsed)
{
	DECLARE_MESSAGE_AND_CHECK_SIZE_LOG(pPacket, nLength, nBytesUsed, T_FL_LOG_OUTPOST_END,
											MSG_FL_LOG_OUTPOST_END, msgGet);

	char szGameLog[1024];
	ms_pLogIOCP->WriteGameLog(msgGet->GetWriteLogString(T1_FL_LOG_OUTPOST_END, szGameLog));
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CLogIOCPSocket::Process_FL_LOG_EXPEDIENCYFUND(const char* pPacket, int nLength, int &nBytesUsed)
/// \brief		
/// \author		dhjin
/// \date		2007-10-02 ~ 2007-10-02
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CLogIOCPSocket::Process_FL_LOG_EXPEDIENCYFUND(const char* pPacket, int nLength, int &nBytesUsed)
{
	DECLARE_MESSAGE_AND_CHECK_SIZE_LOG(pPacket, nLength, nBytesUsed, T_FL_LOG_EXPEDIENCYFUND,
											MSG_FL_LOG_EXPEDIENCYFUND, msgGet);

	char szGameLog[1024];
	ms_pLogIOCP->WriteGameLog(msgGet->GetWriteLogString(T1_FL_LOG_EXPEDIENCYFUND, szGameLog));
	return TRUE;
}


BOOL CLogIOCPSocket::Process_FL_LOG_ITEM_GET_ITEM(const char* pPacket, int nLength, int &nBytesUsed)
{
	DECLARE_MESSAGE_AND_CHECK_SIZE_LOG(pPacket, nLength, nBytesUsed, T_FL_LOG_ITEM_GET_ITEM,
											MSG_FL_LOG_ITEM_GET_ITEM, msgGet);
	///////////////////////////////////////////////////////////////////////////////
	// 2006-05-23 by cmkwon, 로그를 시스템 로그로 남긴다.
	char szGameLog[1024];
	ms_pLogIOCP->WriteGameLog(msgGet->GetWriteLogString(T1_FL_LOG_ITEM_GET_ITEM, szGameLog));
	// 2013-01-04 by jhseol, DB 풀로그 남기기
#ifdef S_DB_FULL_LOG_JHSEOL
#else
	return TRUE;
#endif	// #ifdef S_DB_FULL_LOG_JHSEOL
	// end 2013-01-04 by jhseol, DB 풀로그 남기기

// 2006-05-19 by cmkwon
// 	CAtumDBLogWriter::InsertLog_Item_Get_Throw(&m_ODBCStmt, T1_FL_LOG_ITEM_GET_ITEM,
// 		msgGet, &msgGet->Item4Log, msgGet->ChangeCount);
	MSG_FL_LOG_ITEM_GET_ITEM *pMsg = new MSG_FL_LOG_ITEM_GET_ITEM;
	*pMsg = *msgGet;
	ms_pLogIOCP->m_pAtumLogDBManager->MakeAndEnqueueQuery(QT_FL_LOG_ITEM_GET_ITEM, NULL, pMsg, NULL, T1_FL_LOG_ITEM_GET_ITEM);
	return TRUE;
}

// 2013-08-05 by bckim, 용해 대상아이템 로그 추가
BOOL CLogIOCPSocket::Process_FL_LOG_ITEM_ITEM_DISSOLUTION_ITEM(const char* pPacket, int nLength, int &nBytesUsed)
{
	DECLARE_MESSAGE_AND_CHECK_SIZE_LOG(pPacket, nLength, nBytesUsed, T_FL_LOG_ITEM_DISSOLUTION_ITEM,
		MSG_FL_LOG_ITEM_DISSOLUTION_ITEM, msgGet);

	char szGameLog[1024];
	ms_pLogIOCP->WriteGameLog(msgGet->GetWriteLogString(T1_FL_LOG_ITEM_DISSOLUTION_ITEM, szGameLog));
	return TRUE;
}
// End. 2013-08-05 by bckim, 용해 대상아이템 로그 추가


BOOL CLogIOCPSocket::Process_FL_LOG_ITEM_ADD_ITEM_BY_COMMAND(const char* pPacket, int nLength, int &nBytesUsed)
{
	DECLARE_MESSAGE_AND_CHECK_SIZE_LOG(pPacket, nLength, nBytesUsed, T_FL_LOG_ITEM_ADD_ITEM_BY_COMMAND,
											MSG_FL_LOG_ITEM_ADD_ITEM_BY_COMMAND, msgAddByCmd);
	///////////////////////////////////////////////////////////////////////////////
	// 2006-05-23 by cmkwon, 로그를 시스템 로그로 남긴다.
	char szGameLog[1024];
	ms_pLogIOCP->WriteGameLog(msgAddByCmd->GetWriteLogString(T1_FL_LOG_ITEM_ADD_ITEM_BY_COMMAND, szGameLog));
	// 2013-01-04 by jhseol, DB 풀로그 남기기
#ifdef S_DB_FULL_LOG_JHSEOL
#else
	return TRUE;
#endif	// #ifdef S_DB_FULL_LOG_JHSEOL
	// end 2013-01-04 by jhseol, DB 풀로그 남기기

// 2006-05-19 by cmkwon
// 	CAtumDBLogWriter::InsertLog_Item_Get_Throw(&m_ODBCStmt, T1_FL_LOG_ITEM_ADD_ITEM_BY_COMMAND,
// 		msgAddByCmd, &msgAddByCmd->Item4Log, msgAddByCmd->ChangeCount);
	MSG_FL_LOG_ITEM_ADD_ITEM_BY_COMMAND *pMsg = new MSG_FL_LOG_ITEM_ADD_ITEM_BY_COMMAND;
	*pMsg = *msgAddByCmd;
	ms_pLogIOCP->m_pAtumLogDBManager->MakeAndEnqueueQuery(QT_FL_LOG_ITEM_ADD_ITEM_BY_COMMAND, NULL, pMsg, NULL, T1_FL_LOG_ITEM_ADD_ITEM_BY_COMMAND);
	return TRUE;
}

BOOL CLogIOCPSocket::Process_FL_LOG_ITEM_INSERT_TO_STORE(const char* pPacket, int nLength, int &nBytesUsed)
{
	DECLARE_MESSAGE_AND_CHECK_SIZE_LOG(pPacket, nLength, nBytesUsed, T_FL_LOG_ITEM_INSERT_TO_STORE,
											MSG_FL_LOG_ITEM_INSERT_TO_STORE, msgInsertStore);
	///////////////////////////////////////////////////////////////////////////////
	// 2006-05-23 by cmkwon, 로그를 시스템 로그로 남긴다.
	char szGameLog[1024];
	ms_pLogIOCP->WriteGameLog(msgInsertStore->GetWriteLogString(T1_FL_LOG_ITEM_INSERT_TO_STORE, szGameLog));
	// 2013-01-04 by jhseol, DB 풀로그 남기기
#ifdef S_DB_FULL_LOG_JHSEOL
#else
	return TRUE;
#endif	// #ifdef S_DB_FULL_LOG_JHSEOL
	// end 2013-01-04 by jhseol, DB 풀로그 남기기

// 2006-05-19 by cmkwon
// 	CAtumDBLogWriter::InsertLog_Item_Get_Throw(&m_ODBCStmt, T1_FL_LOG_ITEM_INSERT_TO_STORE,
// 		msgInsertStore, &msgInsertStore->Item4Log, msgInsertStore->ChangeCount);
	MSG_FL_LOG_ITEM_INSERT_TO_STORE *pMsg = new MSG_FL_LOG_ITEM_INSERT_TO_STORE;
	*pMsg = *msgInsertStore;
	ms_pLogIOCP->m_pAtumLogDBManager->MakeAndEnqueueQuery(QT_FL_LOG_ITEM_INSERT_TO_STORE, NULL, pMsg, NULL, T1_FL_LOG_ITEM_INSERT_TO_STORE);
	return TRUE;
}

// 2012-12-27 by bckim, 동일계정, 각각의 케릭터간의 아이템 이동시 로그추가
BOOL CLogIOCPSocket::Process_FL_LOG_ITEM_GET_WITH_SAME_ACCOUNT(const char* pPacket, int nLength, int &nBytesUsed)
{
	DECLARE_MESSAGE_AND_CHECK_SIZE_LOG(pPacket, nLength, nBytesUsed, T_FL_LOG_ITEM_GET_WITH_SAME_ACCOUNT,
		MSG_FL_LOG_ITEM_GET_WITH_SAME_ACCOUNT, msgInsertStore);

	char szGameLog[1024];
	ms_pLogIOCP->WriteGameLog(msgInsertStore->GetWriteLogString(T1_FL_LOG_ITEM_GET_WITH_SAME_ACCOUNT, szGameLog));
	return TRUE;
}
// 2012-12-27 by bckim, 동일계정, 각각의 케릭터간의 아이템 이동시 로그추가.End

BOOL CLogIOCPSocket::Process_FL_LOG_ITEM_DRAW_FROM_STORE(const char* pPacket, int nLength, int &nBytesUsed)
{
	DECLARE_MESSAGE_AND_CHECK_SIZE_LOG(pPacket, nLength, nBytesUsed, T_FL_LOG_ITEM_DRAW_FROM_STORE,
											MSG_FL_LOG_ITEM_DRAW_FROM_STORE, msgDrawStore);
	///////////////////////////////////////////////////////////////////////////////
	// 2006-05-23 by cmkwon, 로그를 시스템 로그로 남긴다.
	char szGameLog[1024];
	ms_pLogIOCP->WriteGameLog(msgDrawStore->GetWriteLogString(T1_FL_LOG_ITEM_DRAW_FROM_STORE, szGameLog));
	// 2013-01-04 by jhseol, DB 풀로그 남기기
#ifdef S_DB_FULL_LOG_JHSEOL
#else
	return TRUE;
#endif	// #ifdef S_DB_FULL_LOG_JHSEOL
	// end 2013-01-04 by jhseol, DB 풀로그 남기기

// 2006-05-19 by cmkwon
// 	CAtumDBLogWriter::InsertLog_Item_Get_Throw(&m_ODBCStmt, T1_FL_LOG_ITEM_DRAW_FROM_STORE,
// 		msgDrawStore, &msgDrawStore->Item4Log, msgDrawStore->ChangeCount);
	MSG_FL_LOG_ITEM_DRAW_FROM_STORE *pMsg = new MSG_FL_LOG_ITEM_DRAW_FROM_STORE;
	*pMsg = *msgDrawStore;
	ms_pLogIOCP->m_pAtumLogDBManager->MakeAndEnqueueQuery(QT_FL_LOG_ITEM_DRAW_FROM_STORE, NULL, pMsg, NULL, T1_FL_LOG_ITEM_DRAW_FROM_STORE);
	return TRUE;
}

BOOL CLogIOCPSocket::Process_FL_LOG_ITEM_TRADE_SEND(const char* pPacket, int nLength, int &nBytesUsed)
{
	DECLARE_MESSAGE_AND_CHECK_SIZE_LOG(pPacket, nLength, nBytesUsed, T_FL_LOG_ITEM_TRADE_SEND,
											MSG_FL_LOG_ITEM_TRADE_SEND, msgTradeSend);

	// 2007-11-13 by cmkwon, 선물하기 기능 추가 - 선물하기 체크
	// 2007-11-29 by cmkwon, 카운터블아이템 합쳐지는 게임 로그 남기기 - 
	//int nLogType = (FALSE == msgTradeSend->bGiftItem) ? T1_FL_LOG_ITEM_TRADE_SEND : T1_FL_LOG_ITEM_GIFT_SEND;
	auto nLogType = T1_FL_LOG_ITEM_TRADE_SEND;

	if (msgTradeSend->bGiftItem) nLogType = T1_FL_LOG_ITEM_GIFT_SEND;
	else if (msgTradeSend->bRearrangeItem) nLogType = T1_FL_LOG_ITEM_REARRANGE_DELETE_ITEM;

	///////////////////////////////////////////////////////////////////////////////
	// 2006-05-23 by cmkwon, 로그를 시스템 로그로 남긴다.
	char szGameLog[1024];
	ms_pLogIOCP->WriteGameLog(msgTradeSend->GetWriteLogString(nLogType, szGameLog));
// 2006-05-23 by cmkwon, 거래는 시스템로그와 DB 모두 저장한다.	return TRUE;

// 2006-05-19 by cmkwon
// 	CAtumDBLogWriter::InsertLog_Item_Trade(&m_ODBCStmt, T1_FL_LOG_ITEM_TRADE_SEND,
// 		msgTradeSend, &msgTradeSend->Item4Log, msgTradeSend->ChangeCount, msgTradeSend->PeerCharacterName, msgTradeSend->PeerCharacterUniqueNumber);
	MSG_FL_LOG_ITEM_TRADE_SEND *pMsg = new MSG_FL_LOG_ITEM_TRADE_SEND;
	*pMsg = *msgTradeSend;
	ms_pLogIOCP->m_pAtumLogDBManager->MakeAndEnqueueQuery(QT_FL_LOG_ITEM_TRADE_SEND, NULL, pMsg, NULL, nLogType);
	return TRUE;
}

BOOL CLogIOCPSocket::Process_FL_LOG_ITEM_TRADE_RECV(const char* pPacket, int nLength, int &nBytesUsed)
{
	DECLARE_MESSAGE_AND_CHECK_SIZE_LOG(pPacket, nLength, nBytesUsed, T_FL_LOG_ITEM_TRADE_RECV,
											MSG_FL_LOG_ITEM_TRADE_RECV, msgTradeRecv);

	// 2007-11-13 by cmkwon, 선물하기 기능 추가 - 선물하기 체크
	// 2007-11-29 by cmkwon, 카운터블아이템 합쳐지는 게임 로그 남기기 - 
	//int nLogType = (FALSE == msgTradeRecv->bGiftItem) ? T1_FL_LOG_ITEM_TRADE_RECV : T1_FL_LOG_ITEM_GIFT_RECV;
	auto nLogType = T1_FL_LOG_ITEM_TRADE_RECV;
	if (msgTradeRecv->bGiftItem) nLogType = T1_FL_LOG_ITEM_GIFT_RECV;
	else if (msgTradeRecv->bRearrangeItem) nLogType = T1_FL_LOG_ITEM_REARRANGE_ITEM;

	///////////////////////////////////////////////////////////////////////////////
	// 2006-05-23 by cmkwon, 로그를 시스템 로그로 남긴다.
	char szGameLog[1024];
	ms_pLogIOCP->WriteGameLog(msgTradeRecv->GetWriteLogString(nLogType, szGameLog));
// 2006-05-23 by cmkwon, 거래는 시스템로그와 DB 모두 저장한다.	return TRUE;

// 2006-05-19 by cmkwon
// 	CAtumDBLogWriter::InsertLog_Item_Trade(&m_ODBCStmt, T1_FL_LOG_ITEM_TRADE_RECV,
// 		msgTradeRecv, &msgTradeRecv->Item4Log, msgTradeRecv->ChangeCount, msgTradeRecv->PeerCharacterName, msgTradeRecv->PeerCharacterUniqueNumber);
	MSG_FL_LOG_ITEM_TRADE_RECV *pMsg = new MSG_FL_LOG_ITEM_TRADE_RECV;
	*pMsg = *msgTradeRecv;
	ms_pLogIOCP->m_pAtumLogDBManager->MakeAndEnqueueQuery(QT_FL_LOG_ITEM_TRADE_RECV, NULL, pMsg, NULL, nLogType);

	return TRUE;
}

BOOL CLogIOCPSocket::Process_FL_LOG_ITEM_THROWAWAY_ITEM(const char* pPacket, int nLength, int &nBytesUsed)
{
	DECLARE_MESSAGE_AND_CHECK_SIZE_LOG(pPacket, nLength, nBytesUsed, T_FL_LOG_ITEM_THROWAWAY_ITEM,
											MSG_FL_LOG_ITEM_THROWAWAY_ITEM, msgThrow);

	// 2007-06-05 by cmkwon, 인챈트 ItemNum 리스트 추가한것임
	if(nLength-nBytesUsed < msgThrow->EnchatItemNumCount*sizeof(INT))
	{
		SendErrorMessage(T_FL_LOG_ITEM_THROWAWAY_ITEM, ERR_PROTOCOL_INVALID_FIELD_DATA);
		return FALSE;
	}
	nBytesUsed += msgThrow->EnchatItemNumCount*sizeof(INT);

	///////////////////////////////////////////////////////////////////////////////
	// 2006-05-23 by cmkwon, 로그를 시스템 로그로 남긴다.

	// 2007-06-05 by cmkwon, 인챈트 리스트도 저장한다
	int nStrLenth = 0;
	char szEnchantLog[1024];		util::zero(szEnchantLog, 1024);
	if(0 < msgThrow->EnchatItemNumCount && SIZE_MAX_ENCHANT_PER_ITEM >= msgThrow->EnchatItemNumCount)
	{
		int *pEnchantItemNum = (int*)( ((BYTE*)msgThrow) + sizeof(MSG_FL_LOG_ITEM_THROWAWAY_ITEM));
		for(int i=0; i < msgThrow->EnchatItemNumCount; i++)
		{
			nStrLenth = strlen(szEnchantLog);
			// 2008-01-29 by cmkwon, S_L: 상점에 판매, 버리기시에 인챈트 로그 버그 수정 
			//sprintf(&szEnchantLog[nStrLenth], "|%d", *pEnchantItemNum);
			sprintf(&szEnchantLog[nStrLenth], "|%d", pEnchantItemNum[i]);
		}
		nStrLenth = strlen(szEnchantLog);
		strcat(&szEnchantLog[nStrLenth], "\r\n");
	}
	else
	{
		strcat(&szEnchantLog[nStrLenth], "\r\n");
	}

	char szGameLog[1024];
	msgThrow->GetWriteLogString(T1_FL_LOG_ITEM_THROWAWAY_ITEM, szGameLog);
	nStrLenth = strlen(szGameLog);
	util::strncpy(&szGameLog[nStrLenth], szEnchantLog, min(strlen(szEnchantLog)+1, 1024-nStrLenth));
	ms_pLogIOCP->WriteGameLog(szGameLog);
	// 2013-01-04 by jhseol, DB 풀로그 남기기
#ifdef S_DB_FULL_LOG_JHSEOL
#else
	return TRUE;
#endif	// #ifdef S_DB_FULL_LOG_JHSEOL
	// end 2013-01-04 by jhseol, DB 풀로그 남기기

// 2006-05-19 by cmkwon
// 	CAtumDBLogWriter::InsertLog_Item_Get_Throw(&m_ODBCStmt, T1_FL_LOG_ITEM_THROWAWAY_ITEM,
// 		msgThrow, &msgThrow->Item4Log, msgThrow->ChangeCount);
	MSG_FL_LOG_ITEM_THROWAWAY_ITEM *pMsg = new MSG_FL_LOG_ITEM_THROWAWAY_ITEM;
	*pMsg = *msgThrow;
	ms_pLogIOCP->m_pAtumLogDBManager->MakeAndEnqueueQuery(QT_FL_LOG_ITEM_THROWAWAY_ITEM, NULL, pMsg, NULL, T1_FL_LOG_ITEM_THROWAWAY_ITEM);
	return TRUE;
}

// 2013-07-01 by bckim, 10인챈트 미만 하이퍼카드처리, 로그추가, 운영툴 보완
BOOL CLogIOCPSocket::Process_FL_LOG_ITEM_TRY_ENCHANT_INFO(const char* pPacket, int nLength, int &nBytesUsed)
{
	DECLARE_MESSAGE_AND_CHECK_SIZE_LOG(pPacket, nLength, nBytesUsed, T_FL_LOG_ITEM_TRY_ENCHANT_INFO,
		MSG_FL_LOG_ITEM_TRY_ENCHANT_INFO, msgTryEnchant);
	
	if(nLength-nBytesUsed < msgTryEnchant->EnchantCount*sizeof(INT))
	{
		SendErrorMessage(T_FL_LOG_ITEM_TRY_ENCHANT_INFO, ERR_PROTOCOL_INVALID_FIELD_DATA);
		return FALSE;
	}
	nBytesUsed += msgTryEnchant->EnchantCount*sizeof(INT);
	
	int nStrLenth = 0;
	char szEnchantLog[1024];		util::zero(szEnchantLog, 1024);
	if(0 < msgTryEnchant->EnchantCount && SIZE_MAX_ENCHANT_PER_ITEM >= msgTryEnchant->EnchantCount)
	{
		int *pEnchantItemNum = (int*)( ((BYTE*)msgTryEnchant) + sizeof(MSG_FL_LOG_ITEM_TRY_ENCHANT_INFO));
		for(int i=0; i < msgTryEnchant->EnchantCount; i++)
		{
			nStrLenth = strlen(szEnchantLog);
			sprintf(&szEnchantLog[nStrLenth], "|%d", pEnchantItemNum[i]);
		}
		nStrLenth = strlen(szEnchantLog);
		strcat(&szEnchantLog[nStrLenth], "\r\n");
	}
	else
	{
		strcat(&szEnchantLog[nStrLenth], "\r\n");
	}
	
	char szGameLog[1024];
	msgTryEnchant->GetWriteLogString(T1_FL_LOG_ITEM_TRY_ENCHANT_INFO, szGameLog);
	nStrLenth = strlen(szGameLog);
	util::strncpy(&szGameLog[nStrLenth], szEnchantLog, min(strlen(szEnchantLog)+1, 1024-nStrLenth));
	ms_pLogIOCP->WriteGameLog(szGameLog);

	MSG_FL_LOG_ITEM_TRY_ENCHANT_INFO_STRING *pLogMsg = new MSG_FL_LOG_ITEM_TRY_ENCHANT_INFO_STRING;
	pLogMsg->TryEnchantInfo = *msgTryEnchant;
	util::strncpy(pLogMsg->szCardList, szEnchantLog, min(strlen(szEnchantLog)+1, 1024-nStrLenth));
	ms_pLogIOCP->m_pAtumLogDBManager->MakeAndEnqueueQuery(QT_FL_LOG_ITEM_TRY_ENCHANT_INFO,NULL,pLogMsg,NULL);

	return TRUE;
}
// End. 2013-07-01 by bckim, 10인챈트 미만 하이퍼카드처리, 로그추가, 운영툴 보완

BOOL CLogIOCPSocket::Process_FL_LOG_ITEM_BUY_ITEM(const char* pPacket, int nLength, int &nBytesUsed)
{
	DECLARE_MESSAGE_AND_CHECK_SIZE_LOG(pPacket, nLength, nBytesUsed, T_FL_LOG_ITEM_BUY_ITEM,
											MSG_FL_LOG_ITEM_BUY_ITEM, msgBuy);
	///////////////////////////////////////////////////////////////////////////////
	// 2006-05-23 by cmkwon, 로그를 시스템 로그로 남긴다.
	char szGameLog[1024];
	ms_pLogIOCP->WriteGameLog(msgBuy->GetWriteLogString(T1_FL_LOG_ITEM_BUY_ITEM, szGameLog));

	// 2007-08-27 by cmkwon, 캐쉬아이템 로그만 DB에 저장해야한다. - 아래와 같이 수정함
	//if(FALSE == msgBuy->IsCashItem)
	if(TRUE != msgBuy->IsCashItem)
	{// 2006-06-21 by cmkwon, 유료아이템이 아니면 파일로그만 남긴다
		return TRUE;
	}

// 2006-05-19 by cmkwon
// 	CAtumDBLogWriter::InsertLog_Item_Buy_Sell(&m_ODBCStmt, T1_FL_LOG_ITEM_BUY_ITEM,
// 		msgBuy, &msgBuy->Item4Log, msgBuy->ChangeCount, msgBuy->RemainedMoney);
	MSG_FL_LOG_ITEM_BUY_ITEM *pMsg = new MSG_FL_LOG_ITEM_BUY_ITEM;
	*pMsg = *msgBuy;
	ms_pLogIOCP->m_pAtumLogDBManager->MakeAndEnqueueQuery(QT_FL_LOG_ITEM_BUY_ITEM, NULL, pMsg, NULL, T1_FL_LOG_ITEM_BUY_ITEM);
	
#ifdef S_GLOG_HSSON
	// 2010-06-01 by shcho, GLogDB 관련 -
	if( TRUE == msgBuy->GLog )
	{
	MSG_FL_LOG_ITEM_BUY_ITEM *pGLogMsg = new MSG_FL_LOG_ITEM_BUY_ITEM;
	*pGLogMsg = *msgBuy;
	ms_pLogIOCP->m_pGLogDBManager->MakeAndEnqueueQuery(QT_FL_GLOG_BUY_CASH_ITEM,NULL,pGLogMsg,NULL);
	}
#endif

	// start 2011-12-12 by hskim, GLog 2차
#ifdef S_GLOG_2ND_HSKIM
	/*
	// YD에서 구매정보는 영업 비밀이라며 거절함
	if( TRUE == msgBuy->GLog )
	{
	MSG_FL_LOG_ITEM_BUY_ITEM *pGLog2ndMsg = new MSG_FL_LOG_ITEM_BUY_ITEM;
	*pGLog2ndMsg = *msgBuy;
	ms_pLogIOCP->m_pGLogDBManager->MakeAndEnqueueQuery(QT_FL_GLOG_TB_ITEM_BUY, NULL, pGLog2ndMsg, NULL);
	}
	*/
#endif
	// end 2011-12-12 by hskim, GLog 2차

	return TRUE;
}


// 2012-10-16 by bckim, 캐쉬아이템 지불정보 Log 추가
BOOL CLogIOCPSocket::Process_FL_LOG_ITEM_BUY_CASH_PRICE(const char* pPacket, int nLength, int &nBytesUsed)
{
	DECLARE_MESSAGE_AND_CHECK_SIZE_LOG(pPacket, nLength, nBytesUsed, T_FL_LOG_ITEM_BUY_CASH_PRICE,
											MSG_FL_LOG_ITEM_BUY_CASH_PRICE, msgBuy);
	char szGameLog[1024];
	ms_pLogIOCP->WriteGameLog(msgBuy->GetWriteLogString(T1_FL_LOG_ITEM_BUY_CASH_PRICE, szGameLog));

	return TRUE;
}
// end 2012-10-16 by bckim, 캐쉬아이템 지불정보 Log 추가

// 2012-12-20 by bckim, 세력초기화시 길드창고아이템이 길드장에게 옮겨지는 아이템 로그
BOOL CLogIOCPSocket::Process_FL_LOG_ITEM_INITIALIZED_GUILD_STORE(const char* pPacket, int nLength, int &nBytesUsed)
{
	DECLARE_MESSAGE_AND_CHECK_SIZE_LOG(pPacket, nLength, nBytesUsed, T_FL_LOG_ITEM_INITIALIZED_GUILD_STORE,
		MSG_FL_LOG_ITEM_INITIALIZED_GUILD_STORE, msgBuy);
	char szGameLog[1024];
	ms_pLogIOCP->WriteGameLog(msgBuy->GetWriteLogString(T1_FL_LOG_ITEM_INITIALIZED_GUILD_STORE, szGameLog));
	
	return TRUE;
}
// 2012-12-20 by bckim, 세력초기화시 길드창고아이템이 길드장에게 옮겨지는 아이템 로그.End


BOOL CLogIOCPSocket::Process_FL_LOG_ITEM_SELL_ITEM(const char* pPacket, int nLength, int &nBytesUsed)
{
	DECLARE_MESSAGE_AND_CHECK_SIZE_LOG(pPacket, nLength, nBytesUsed, T_FL_LOG_ITEM_SELL_ITEM,
											MSG_FL_LOG_ITEM_SELL_ITEM, msgSell);

	// 2007-07-20 by cmkwon, 판매아이템 로그 추가 - 인챈트 리스트 받은것 처리하기
	if(nLength-nBytesUsed < msgSell->EnchatItemNumCount*sizeof(INT))
	{
		SendErrorMessage(T_FL_LOG_ITEM_SELL_ITEM, ERR_PROTOCOL_INVALID_FIELD_DATA);
		return FALSE;
	}
	nBytesUsed += msgSell->EnchatItemNumCount*sizeof(INT);

	///////////////////////////////////////////////////////////////////////////////
	// 2006-05-23 by cmkwon, 로그를 시스템 로그로 남긴다.
// 2007-07-20 by cmkwon, 판매아이템 로그 추가 - 아래와 같이 수정(접두/접미, 인챈트정보 게임로그에 저장하기)
//	char szGameLog[1024];
//	ms_pLogIOCP->WriteGameLog(msgSell->GetWriteLogString(T1_FL_LOG_ITEM_SELL_ITEM, szGameLog));
	// 2007-07-20 by cmkwon, 판매아이템 로그 추가 - 인챈트 리스트 정보 게임로그에 저장하기
	int nStrLenth = 0;
	char szEnchantLog[1024];		util::zero(szEnchantLog, 1024);
	if(0 < msgSell->EnchatItemNumCount && SIZE_MAX_ENCHANT_PER_ITEM >= msgSell->EnchatItemNumCount)
	{
		int *pEnchantItemNum = (int*)( ((BYTE*)msgSell) + sizeof(MSG_FL_LOG_ITEM_SELL_ITEM));
		for(int i=0; i < msgSell->EnchatItemNumCount; i++)
		{
			nStrLenth = strlen(szEnchantLog);
			// 2008-01-29 by cmkwon, S_L: 상점에 판매, 버리기시에 인챈트 로그 버그 수정 
			//sprintf(&szEnchantLog[nStrLenth], "|%d", *pEnchantItemNum);
			sprintf(&szEnchantLog[nStrLenth], "|%d", pEnchantItemNum[i]);
		}
		nStrLenth = strlen(szEnchantLog);
		strcat(&szEnchantLog[nStrLenth], "\r\n");
	}
	else
	{
		strcat(&szEnchantLog[nStrLenth], "\r\n");
	}

	char szGameLog[1024];
	msgSell->GetWriteLogString(T1_FL_LOG_ITEM_SELL_ITEM, szGameLog);
	nStrLenth = strlen(szGameLog);
	util::strncpy(&szGameLog[nStrLenth], szEnchantLog, min(strlen(szEnchantLog)+1, 1024-nStrLenth));
	ms_pLogIOCP->WriteGameLog(szGameLog);
	// 2013-01-04 by jhseol, DB 풀로그 남기기
#ifdef S_DB_FULL_LOG_JHSEOL
#else
	return TRUE;
#endif	// #ifdef S_DB_FULL_LOG_JHSEOL
	// end 2013-01-04 by jhseol, DB 풀로그 남기기

// 2006-05-19 by cmkwon
// 	CAtumDBLogWriter::InsertLog_Item_Buy_Sell(&m_ODBCStmt, T1_FL_LOG_ITEM_SELL_ITEM,
// 		msgSell, &msgSell->Item4Log, msgSell->ChangeCount, msgSell->RemainedMoney);
	MSG_FL_LOG_ITEM_SELL_ITEM *pMsg = new MSG_FL_LOG_ITEM_SELL_ITEM;
	*pMsg = *msgSell;
	ms_pLogIOCP->m_pAtumLogDBManager->MakeAndEnqueueQuery(QT_FL_LOG_ITEM_SELL_ITEM, NULL, pMsg, NULL, T1_FL_LOG_ITEM_SELL_ITEM);

	return TRUE;
}

BOOL CLogIOCPSocket::Process_FL_LOG_ITEM_USE_ITEM(const char* pPacket, int nLength, int &nBytesUsed)
{
	DECLARE_MESSAGE_AND_CHECK_SIZE_LOG(pPacket, nLength, nBytesUsed, T_FL_LOG_ITEM_USE_ITEM,
											MSG_FL_LOG_ITEM_USE_ITEM, msgUse);
	///////////////////////////////////////////////////////////////////////////////
	// 2006-05-23 by cmkwon, 로그를 시스템 로그로 남긴다.
	char szGameLog[1024];
	ms_pLogIOCP->WriteGameLog(msgUse->GetWriteLogString(T1_FL_LOG_ITEM_USE_ITEM, szGameLog));
	// 2013-01-04 by jhseol, DB 풀로그 남기기
#ifdef S_DB_FULL_LOG_JHSEOL
#else
	return TRUE;
#endif	// #ifdef S_DB_FULL_LOG_JHSEOL
	// end 2013-01-04 by jhseol, DB 풀로그 남기기

// 2006-05-19 by cmkwon
// 	CAtumDBLogWriter::InsertLog_Item_Charac_Use(&m_ODBCStmt, T1_FL_LOG_ITEM_USE_ITEM,
// 		msgUse, &msgUse->Item4Log);
	MSG_FL_LOG_ITEM_USE_ITEM *pMsg = new MSG_FL_LOG_ITEM_USE_ITEM;
	*pMsg = *msgUse;
	ms_pLogIOCP->m_pAtumLogDBManager->MakeAndEnqueueQuery(QT_FL_LOG_ITEM_USE_ITEM, NULL, pMsg, NULL, T1_FL_LOG_ITEM_USE_ITEM);

	return TRUE;
}

BOOL CLogIOCPSocket::Process_FL_LOG_ITEM_USE_ENERGY(const char* pPacket, int nLength, int &nBytesUsed)
{
	DECLARE_MESSAGE_AND_CHECK_SIZE_LOG(pPacket, nLength, nBytesUsed, T_FL_LOG_ITEM_USE_ENERGY,
											MSG_FL_LOG_ITEM_USE_ENERGY, msgEnergy);
	///////////////////////////////////////////////////////////////////////////////
	// 2006-05-23 by cmkwon, 로그를 시스템 로그로 남긴다.
	char szGameLog[1024];
	ms_pLogIOCP->WriteGameLog(msgEnergy->GetWriteLogString(T1_FL_LOG_ITEM_USE_ENERGY, szGameLog));
	// 2013-01-04 by jhseol, DB 풀로그 남기기
#ifdef S_DB_FULL_LOG_JHSEOL
#else
	return TRUE;
#endif	// #ifdef S_DB_FULL_LOG_JHSEOL
	// end 2013-01-04 by jhseol, DB 풀로그 남기기

// 2006-05-19 by cmkwon
// 	CAtumDBLogWriter::InsertLog_Item_Charac_Use(&m_ODBCStmt, T1_FL_LOG_ITEM_USE_ENERGY,
// 		msgEnergy, &msgEnergy->Item4Log);
	MSG_FL_LOG_ITEM_USE_ENERGY *pMsg = new MSG_FL_LOG_ITEM_USE_ENERGY;
	*pMsg = *msgEnergy;
	ms_pLogIOCP->m_pAtumLogDBManager->MakeAndEnqueueQuery(QT_FL_LOG_ITEM_USE_ENERGY, NULL, pMsg, NULL, T1_FL_LOG_ITEM_USE_ENERGY);

	return TRUE;
}

BOOL CLogIOCPSocket::Process_FL_LOG_ITEM_USE_ENCHANT(const char* pPacket, int nLength, int &nBytesUsed)
{
	DECLARE_MESSAGE_AND_CHECK_SIZE_LOG(pPacket, nLength, nBytesUsed, T_FL_LOG_ITEM_USE_ENCHANT,
											MSG_FL_LOG_ITEM_USE_ENCHANT, msgEnchant);
	///////////////////////////////////////////////////////////////////////////////
	// 2006-05-23 by cmkwon, 로그를 시스템 로그로 남긴다.
	char szGameLog[1024];
	ms_pLogIOCP->WriteGameLog(msgEnchant->GetWriteLogString(T1_FL_LOG_ITEM_USE_ENCHANT, szGameLog));
	// 2013-01-04 by jhseol, DB 풀로그 남기기
#ifdef S_DB_FULL_LOG_JHSEOL
#else
	return TRUE;
#endif	// #ifdef S_DB_FULL_LOG_JHSEOL
	// end 2013-01-04 by jhseol, DB 풀로그 남기기

// 2006-05-19 by cmkwon
// 	CAtumDBLogWriter::InsertLog_Item_Charac_Use(&m_ODBCStmt, T1_FL_LOG_ITEM_USE_ENCHANT,
// 		msgEnchant, &msgEnchant->Item4Log, msgEnchant->EnchantLogType);
	MSG_FL_LOG_ITEM_USE_ENCHANT *pMsg = new MSG_FL_LOG_ITEM_USE_ENCHANT;
	*pMsg = *msgEnchant;
	ms_pLogIOCP->m_pAtumLogDBManager->MakeAndEnqueueQuery(QT_FL_LOG_ITEM_USE_ENCHANT, NULL, pMsg, NULL, T1_FL_LOG_ITEM_USE_ENCHANT);
	return TRUE;
}

BOOL CLogIOCPSocket::Process_FL_LOG_SERVER_INFO_MAP(const char* pPacket, int nLength, int &nBytesUsed)
{
	DECLARE_MESSAGE_AND_CHECK_SIZE_LOG(pPacket, nLength, nBytesUsed, T_FL_LOG_SERVER_INFO_MAP,
											MSG_FL_LOG_SERVER_INFO_MAP, msgServerMap);

	///////////////////////////////////////////////////////////////////////////////
	// 2009-04-29 by cmkwon, 동접 관련 로그를 게임 파일 로그로 저장하기 - 
	char szGameLog[1024];
	ms_pLogIOCP->WriteGameLog(msgServerMap->GetWriteLogString(T1_FL_LOG_SERVER_INFO_MAP, szGameLog));
///////////////////////////////////////////////////////////////////////////////
// 2010-06-01 by shcho, GLogDB 관련 - 밑과 같이 주석 처리
// 	return TRUE;	// 2009-04-29 by cmkwon, 동접 관련 로그를 게임 파일 로그로 저장하기 - 맵별 로그는 DB에 저장하지 않는다
// 
// // 2006-05-19 by cmkwon
// // CAtumDBLogWriter::InsertLog_Server_Map(&m_ODBCStmt, T1_FL_LOG_SERVER_INFO_MAP,
// // msgServerMap->MapIndex, msgServerMap->ChannelIndex, msgServerMap->ClientCount, msgServerMap->MonsterCount);
	// 2013-01-04 by jhseol, DB 풀로그 남기기
#ifdef S_DB_FULL_LOG_JHSEOL
	MSG_FL_LOG_SERVER_INFO_MAP *pMsg = new MSG_FL_LOG_SERVER_INFO_MAP;
	*pMsg = *msgServerMap;
	ms_pLogIOCP->m_pAtumLogDBManager->MakeAndEnqueueQuery(QT_FL_LOG_SERVER_INFO_MAP, NULL, pMsg, NULL, T1_FL_LOG_SERVER_INFO_MAP);
#else
#endif	// #ifdef S_DB_FULL_LOG_JHSEOL
	// end 2013-01-04 by jhseol, DB 풀로그 남기기

#ifdef S_GLOG_HSSON
	MSG_FL_LOG_SERVER_INFO_MAP *pGLogMsg = new MSG_FL_LOG_SERVER_INFO_MAP;
	*pGLogMsg = *msgServerMap;
	ms_pLogIOCP->m_pGLogDBManager->MakeAndEnqueueQuery(QT_FL_GLOG_CONNECT_ZONE_USER_COUNT, NULL, pGLogMsg, NULL);
#endif

	return TRUE;
}

BOOL CLogIOCPSocket::Process_FL_LOG_SERVER_INFO_TOTAL(const char* pPacket, int nLength, int &nBytesUsed)
{
	DECLARE_MESSAGE_AND_CHECK_SIZE_LOG(pPacket, nLength, nBytesUsed, T_FL_LOG_SERVER_INFO_TOTAL,
											MSG_FL_LOG_SERVER_INFO_TOTAL, msgServerTotal);

	///////////////////////////////////////////////////////////////////////////////
	// 2009-04-29 by cmkwon, 동접 관련 로그를 게임 파일 로그로 저장하기 - 
	char szGameLog[1024];
	ms_pLogIOCP->WriteGameLog(msgServerTotal->GetWriteLogString(T1_FL_LOG_SERVER_INFO_TOTAL, szGameLog));

// 2006-05-19 by cmkwon
// 	CAtumDBLogWriter::InsertLog_Server_Map(&m_ODBCStmt, T1_FL_LOG_SERVER_INFO_TOTAL,
// 		0, 0, msgServerTotal->ClientCount, msgServerTotal->MonsterCount);
	MSG_FL_LOG_SERVER_INFO_TOTAL *pMsg = new MSG_FL_LOG_SERVER_INFO_TOTAL;
	*pMsg = *msgServerTotal;
	ms_pLogIOCP->m_pAtumLogDBManager->MakeAndEnqueueQuery(QT_FL_LOG_SERVER_INFO_TOTAL, NULL, pMsg, NULL, T1_FL_LOG_SERVER_INFO_TOTAL);

#ifdef S_GLOG_HSSON
	// 2010-06-01 by shcho, GLogDB 관련 -
	if(TRUE == msgServerTotal->bGlogUpdate)
	{
		MSG_FL_LOG_SERVER_INFO_TOTAL *pGLogMsg = new MSG_FL_LOG_SERVER_INFO_TOTAL;
		*pGLogMsg = *msgServerTotal;
		ms_pLogIOCP->m_pGLogDBManager->MakeAndEnqueueQuery(QT_FL_GLOG_CONNECT_TOTAL_USER_COUNT, NULL, pGLogMsg, NULL);
	}
#endif // S_GLOG_HSSON

	// start 2011-12-12 by hskim, GLog 2차
#ifdef S_GLOG_2ND_HSKIM
	if( TRUE == msgServerTotal->bGlogUpdate )
	{
		MSG_FL_LOG_SERVER_INFO_TOTAL *pGLog2ndMsg = new MSG_FL_LOG_SERVER_INFO_TOTAL;
		*pGLog2ndMsg = *msgServerTotal;
		ms_pLogIOCP->m_pGLogDBManager->MakeAndEnqueueQuery(QT_FL_GLOG_TB_CONCURRENT_USER, NULL, pGLog2ndMsg, NULL);
	}
#endif
	// end 2011-12-12 by hskim, GLog 2차

	return TRUE;
}

BOOL CLogIOCPSocket::Process_FL_LOG_HACKING_LOG(const char* pPacket, int nLength, int &nBytesUsed)
{
	DECLARE_MESSAGE_AND_CHECK_SIZE_LOG(pPacket, nLength, nBytesUsed, T_FL_LOG_HACKING_LOG,
											MSG_FL_LOG_HACKING_LOG, pRHacking);
	///////////////////////////////////////////////////////////////////////////////
	// 2006-05-23 by cmkwon, 로그를 시스템 로그로 남긴다.
	char szGameLog[1024];
	ms_pLogIOCP->WriteGameLog(pRHacking->GetWriteLogString(T1_FL_LOG_HACKING_LOG, szGameLog));
	return TRUE;
}

BOOL CLogIOCPSocket::Process_FL_LOG_SPEED_HACK(const char* pPacket, int nLength, int &nBytesUsed)
{
	DECLARE_MESSAGE_AND_CHECK_SIZE_LOG(pPacket, nLength, nBytesUsed, T_FL_LOG_SPEED_HACK,
											MSG_FL_LOG_SPEED_HACK, msgSpeedHack);
// 2006-05-19 by cmkwon
//	CAtumDBLogWriter::InsertLog_Speed_Hack(&m_ODBCStmt, msgSpeedHack);
	MSG_FL_LOG_SPEED_HACK *pMsg = new MSG_FL_LOG_SPEED_HACK;
	*pMsg = *msgSpeedHack;
	ms_pLogIOCP->m_pAtumLogDBManager->MakeAndEnqueueQuery(QT_FL_LOG_SPEED_HACK, NULL, pMsg, NULL, T1_FL_LOG_SPEED_HACK);
	return TRUE;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CLogIOCPSocket::Process_FL_LOG_MEMORY_HACK(const char* pPacket, int nLength, int &nBytesUsed)
/// \brief		
/// \author		cmkwon
/// \date		2005-12-20 ~ 2005-12-20
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CLogIOCPSocket::Process_FL_LOG_MEMORY_HACK(const char* pPacket, int nLength, int &nBytesUsed)
{
	DECLARE_MESSAGE_AND_CHECK_SIZE_LOG(pPacket, nLength, nBytesUsed, T_FL_LOG_MEMORY_HACK,
											MSG_FL_LOG_MEMORY_HACK, pRMsg);

	///////////////////////////////////////////////////////////////////////////////
	// 2007-04-10 by cmkwon, 로그를 시스템 로그로 남긴다.
	char szGameLog[1024];
	ms_pLogIOCP->WriteGameLog(pRMsg->GetWriteLogString(T1_FL_LOG_MEMORY_HACK, szGameLog));

// 2006-05-19 by cmkwon
//	CAtumDBLogWriter::InsertLog_Memory_Hack(&m_ODBCStmt, pRMsg);
	MSG_FL_LOG_MEMORY_HACK *pMsg = new MSG_FL_LOG_MEMORY_HACK;
	*pMsg = *pRMsg;
	ms_pLogIOCP->m_pAtumLogDBManager->MakeAndEnqueueQuery(QT_FL_LOG_MEMORY_HACK, NULL, pMsg, NULL, T1_FL_LOG_MEMORY_HACK);
	return TRUE;
}

BOOL CLogIOCPSocket::Process_FL_LOG_MONSTER_BOSS(const char* pPacket, int nLength, int &nBytesUsed)
{
	DECLARE_MESSAGE_AND_CHECK_SIZE_LOG(pPacket, nLength, nBytesUsed, T_FL_LOG_MONSTER_BOSS,
											MSG_FL_LOG_MONSTER_BOSS, msgMonsterBoss);
	nBytesUsed += msgMonsterBoss->LenOfDropItemList;

// 2006-05-19 by cmkwon, 기존 코드에 버그 있음, 다시 살릴경우 수정 필요함
// 	char szItems[200];
// 	strncpy(szItems, pPacket+MSG_SIZE(MSG_FL_LOG_MONSTER_BOSS), 200);
// 	szItems[199] = '\0';
// 	CAtumDBLogWriter::InsertLog_Monster_Boss(&m_ODBCStmt, msgMonsterBoss, szItems);

	///////////////////////////////////////////////////////////////////////////////
	// 2006-05-23 by cmkwon, 로그를 시스템 로그로 남긴다.
	char szGameLog[1024];
	char szDroItem[1024];		util::zero(szDroItem, 1024);
	if(0 < msgMonsterBoss->LenOfDropItemList)
	{
		util::strncpy(szDroItem, (char*)msgMonsterBoss + sizeof(MSG_FL_LOG_MONSTER_BOSS), 200);
	}
	ms_pLogIOCP->WriteGameLog(msgMonsterBoss->GetWriteLogString(T1_FL_LOG_MONSTER_BOSS, szGameLog, szDroItem));
	// 2013-01-04 by jhseol, DB 풀로그 남기기
#ifdef S_DB_FULL_LOG_JHSEOL
#else
	return TRUE;
#endif	// #ifdef S_DB_FULL_LOG_JHSEOL
	// end 2013-01-04 by jhseol, DB 풀로그 남기기


	msgMonsterBoss->LenOfDropItemList = min(199, msgMonsterBoss->LenOfDropItemList);
	MSG_FL_LOG_MONSTER_BOSS *pMsg	= new MSG_FL_LOG_MONSTER_BOSS;	
	*pMsg = *msgMonsterBoss;
	
	char *pParam = NULL;
	if(0 < msgMonsterBoss->LenOfDropItemList)
	{
		pParam	= new char[msgMonsterBoss->LenOfDropItemList+1];
		util::zero(pParam, msgMonsterBoss->LenOfDropItemList+1);
		memcpy(pParam, (char*)msgMonsterBoss + sizeof(MSG_FL_LOG_MONSTER_BOSS), msgMonsterBoss->LenOfDropItemList);
	}
	ms_pLogIOCP->m_pAtumLogDBManager->MakeAndEnqueueQuery(QT_FL_LOG_MONSTER_BOSS, NULL, pMsg, pParam, T1_FL_LOG_MONSTER_BOSS);
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
// 2008-04-01 by dhjin, 모선전, 거점전 정보창 기획안 - 
BOOL CLogIOCPSocket::Process_FL_LOG_MS_WAR(const char* pPacket, int nLength, int &nBytesUsed)
{
	DECLARE_MESSAGE_AND_CHECK_SIZE_LOG(pPacket, nLength, nBytesUsed, T_FL_LOG_MS_WAR,
											MSG_FL_LOG_MS_WAR, pRMsg);
	MSG_FL_LOG_MS_WAR *pMsg = new MSG_FL_LOG_MS_WAR;
	*pMsg = *pRMsg;
	
	ms_pLogIOCP->m_pAtumLogDBManager->MakeAndEnqueueQuery(QT_FL_LOG_MS_WAR, NULL, pMsg, NULL, T1_FL_LOG_MS_WAR);
	return TRUE;
}

BOOL CLogIOCPSocket::Process_FL_LOG_SP_WAR(const char* pPacket, int nLength, int &nBytesUsed)
{
	DECLARE_MESSAGE_AND_CHECK_SIZE_LOG(pPacket, nLength, nBytesUsed, T_FL_LOG_SP_WAR,
											MSG_FL_LOG_SP_WAR, pRMsg);
	MSG_FL_LOG_SP_WAR *pMsg = new MSG_FL_LOG_SP_WAR;
	*pMsg = *pRMsg;
		
	ms_pLogIOCP->m_pAtumLogDBManager->MakeAndEnqueueQuery(QT_FL_LOG_SP_WAR, NULL, pMsg, NULL, T1_FL_LOG_SP_WAR);
	return TRUE;	
}

BOOL CLogIOCPSocket::Process_FL_LOG_WAR_CONTRIBUTION(const char* pPacket, int nLength, int &nBytesUsed)
{
	DECLARE_MESSAGE_AND_CHECK_SIZE_LOG(pPacket, nLength, nBytesUsed, T_FL_LOG_WAR_CONTRIBUTION,
											MSG_FL_LOG_WAR_CONTRIBUTION, pRMsg);
	///////////////////////////////////////////////////////////////////////////////
	// 2007-10-16 by cmkwon, 로그 추가 - 로그를 시스템 로그로 남긴다.
	char szGameLog[1024];
	ms_pLogIOCP->WriteGameLog(pRMsg->GetWriteLogString(T1_FL_LOG_WAR_CONTRIBUTION, szGameLog));
	
	return TRUE;	
}

BOOL CLogIOCPSocket::Process_FL_LOG_WAR_CONTRIBUTION_GEAR(const char* pPacket, int nLength, int &nBytesUsed)
{
	DECLARE_MESSAGE_AND_CHECK_SIZE_LOG(pPacket, nLength, nBytesUsed, T_FL_LOG_WAR_CONTRIBUTION_GEAR,
											MSG_FL_LOG_WAR_CONTRIBUTION_GEAR, pRMsg);
	///////////////////////////////////////////////////////////////////////////////
	// 2007-10-16 by cmkwon, 로그 추가 - 로그를 시스템 로그로 남긴다.
	char szGameLog[1024];
	ms_pLogIOCP->WriteGameLog(pRMsg->GetWriteLogString(T1_FL_LOG_WAR_CONTRIBUTION_GEAR, szGameLog));
	
	return TRUE;	
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CLogIOCPSocket::Process_FL_LOG_LUCKY_DROP(const char* pPacket, int nLength, int &nBytesUsed)
/// \brief		// 2009-03-31 by dhjin, 럭키머신 로그
/// \author		dhjin
/// \date		2009-03-31 ~ 2009-03-31
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CLogIOCPSocket::Process_FL_LOG_LUCKY_DROP(const char* pPacket, int nLength, int &nBytesUsed)
{
	DECLARE_MESSAGE_AND_CHECK_SIZE_LOG(pPacket, nLength, nBytesUsed, T_FL_LOG_LUCKY_DROP,
											MSG_FL_LOG_LUCKY_DROP, pRMsg);
	///////////////////////////////////////////////////////////////////////////////
	// 2007-10-16 by cmkwon, 로그 추가 - 로그를 시스템 로그로 남긴다.
	char szGameLog[1024];
	ms_pLogIOCP->WriteGameLog(pRMsg->GetWriteLogString(T1_FL_LOG_LUCKY_DROP, szGameLog));
	
	return TRUE;	
}

BOOL CLogIOCPSocket::Process_FL_LOG_START_FIELD_SERVER(const char* pPacket, int nLength, int &nBytesUsed)
{
	// No Body
	///////////////////////////////////////////////////////////////////////////////
	// 2006-05-23 by cmkwon, 로그를 시스템 로그로 남긴다.

	// 2009-04-20 by cmkwon, 시스템 로그 추가 - 
	g_pLogGlobal->WriteSystemLogEX(TRUE, "  [Notify] OnReceived T_FL_LOG_START_FIELD_SERVER ! \r\n");
	// 2013-01-04 by jhseol, DB 풀로그 남기기
#ifdef S_DB_FULL_LOG_JHSEOL
#else
	return TRUE;
#endif	// #ifdef S_DB_FULL_LOG_JHSEOL
	// end 2013-01-04 by jhseol, DB 풀로그 남기기


// 2006-05-19 by cmkwon
//	CAtumDBLogWriter::MGame_PB_Init(&m_ODBCStmt);

	ms_pLogIOCP->m_pAtumLogDBManager->MakeAndEnqueueQuery(QT_FL_LOG_START_FIELD_SERVER, NULL, NULL, NULL, T1_FL_LOG_START_FIELD_SERVER);
	return TRUE;
}


BOOL CLogIOCPSocket::Process_FL_LOG_INFLWAR_START(const char* pPacket, int nLength, int &nBytesUsed)
{
	DECLARE_MESSAGE_AND_CHECK_SIZE_LOG(pPacket, nLength, nBytesUsed, T_FL_LOG_INFLWAR_START,
											MSG_FL_LOG_INFLWAR_START, pRMsg);
	///////////////////////////////////////////////////////////////////////////////
	// 2007-10-16 by cmkwon, 로그 추가 - 로그를 시스템 로그로 남긴다.
	char szGameLog[1024];
	ms_pLogIOCP->WriteGameLog(pRMsg->GetWriteLogString(T1_FL_LOG_INFLWAR_START, szGameLog));
	return TRUE;
}
BOOL CLogIOCPSocket::Process_FL_LOG_INFLWAR_END(const char* pPacket, int nLength, int &nBytesUsed)
{
	DECLARE_MESSAGE_AND_CHECK_SIZE_LOG(pPacket, nLength, nBytesUsed, T_FL_LOG_INFLWAR_END,
											MSG_FL_LOG_INFLWAR_END, pRMsg);
	///////////////////////////////////////////////////////////////////////////////
	// 2007-10-16 by cmkwon, 로그 추가 - 로그를 시스템 로그로 남긴다.
	char szGameLog[1024];
	ms_pLogIOCP->WriteGameLog(pRMsg->GetWriteLogString(T1_FL_LOG_INFLWAR_END, szGameLog));
	return TRUE;
}
BOOL CLogIOCPSocket::Process_FL_LOG_OUTPOSTWAR_RESET_START(const char* pPacket, int nLength, int &nBytesUsed)
{
	DECLARE_MESSAGE_AND_CHECK_SIZE_LOG(pPacket, nLength, nBytesUsed, T_FL_LOG_OUTPOSTWAR_RESET_START,
											MSG_FL_LOG_OUTPOSTWAR_RESET_START, pRMsg);
	///////////////////////////////////////////////////////////////////////////////
	// 2007-10-16 by cmkwon, 로그 추가 - 로그를 시스템 로그로 남긴다.
	char szGameLog[1024];
	ms_pLogIOCP->WriteGameLog(pRMsg->GetWriteLogString(T1_FL_LOG_OUTPOSTWAR_RESET_START, szGameLog));
	return TRUE;
}
BOOL CLogIOCPSocket::Process_FL_LOG_OUTPOSTWAR_RESET_DESTROY(const char* pPacket, int nLength, int &nBytesUsed)
{
	DECLARE_MESSAGE_AND_CHECK_SIZE_LOG(pPacket, nLength, nBytesUsed, T_FL_LOG_OUTPOSTWAR_RESET_DESTROY,
											MSG_FL_LOG_OUTPOSTWAR_RESET_DESTROY, pRMsg);
	///////////////////////////////////////////////////////////////////////////////
	// 2007-10-16 by cmkwon, 로그 추가 - 로그를 시스템 로그로 남긴다.
	char szGameLog[1024];
	ms_pLogIOCP->WriteGameLog(pRMsg->GetWriteLogString(T1_FL_LOG_OUTPOSTWAR_RESET_DESTROY, szGameLog));
	return TRUE;
}
BOOL CLogIOCPSocket::Process_FL_LOG_OUTPOSTWAR_RESET_COMPLETE(const char* pPacket, int nLength, int &nBytesUsed)
{
	DECLARE_MESSAGE_AND_CHECK_SIZE_LOG(pPacket, nLength, nBytesUsed, T_FL_LOG_OUTPOSTWAR_RESET_COMPLETE,
											MSG_FL_LOG_OUTPOSTWAR_RESET_COMPLETE, pRMsg);
	///////////////////////////////////////////////////////////////////////////////
	// 2007-10-16 by cmkwon, 로그 추가 - 로그를 시스템 로그로 남긴다.
	char szGameLog[1024];
	ms_pLogIOCP->WriteGameLog(pRMsg->GetWriteLogString(T1_FL_LOG_OUTPOSTWAR_RESET_COMPLETE, szGameLog));
	return TRUE;
}
BOOL CLogIOCPSocket::Process_FL_LOG_OUTPOSTWAR_PROTECTOR_DESTROY(const char* pPacket, int nLength, int &nBytesUsed)
{
	DECLARE_MESSAGE_AND_CHECK_SIZE_LOG(pPacket, nLength, nBytesUsed, T_FL_LOG_OUTPOSTWAR_PROTECTOR_DESTROY,
											MSG_FL_LOG_OUTPOSTWAR_PROTECTOR_DESTROY, pRMsg);
	///////////////////////////////////////////////////////////////////////////////
	// 2007-10-16 by cmkwon, 로그 추가 - 로그를 시스템 로그로 남긴다.
	char szGameLog[1024];
	pRMsg->GetWriteLogString(T1_FL_LOG_OUTPOSTWAR_PROTECTOR_DESTROY, szGameLog);
	
	char szDamageList[1024] = {0x00, };
	for(int i=0; i < SIZE_MAX_OUTPOSTWAR_DAMAGELIST; i++)
	{
		if(FALSE == IS_VALID_UNIQUE_NUMBER(pRMsg->ArrGuildUID[i]))
		{// 2007-10-16 by cmkwon, GuildUID가 유효한지 체크
			break;
		}
		int nLen = strlen(szDamageList);
		if(nLen > 512)
		{// 2007-10-16 by cmkwon, 데미지 로그가 길면 break
			break;
		}
		sprintf(&szDamageList[nLen], "|%u|%5.1f", pRMsg->ArrGuildUID[i], pRMsg->ArrDamage[i]);
	}
	strncat(szGameLog, szDamageList, 1024);
	strncat(szGameLog, "\r\n", 1024);

	ms_pLogIOCP->WriteGameLog(szGameLog);
	return TRUE;
}
BOOL CLogIOCPSocket::Process_FL_LOG_OUTPOSTWAR_SET_NEXTTIME(const char* pPacket, int nLength, int &nBytesUsed)
{
	DECLARE_MESSAGE_AND_CHECK_SIZE_LOG(pPacket, nLength, nBytesUsed, T_FL_LOG_OUTPOSTWAR_SET_NEXTTIME,
											MSG_FL_LOG_OUTPOSTWAR_SET_NEXTTIME, pRMsg);
	///////////////////////////////////////////////////////////////////////////////
	// 2007-10-16 by cmkwon, 로그 추가 - 로그를 시스템 로그로 남긴다.
	char szGameLog[1024];
	ms_pLogIOCP->WriteGameLog(pRMsg->GetWriteLogString(T1_FL_LOG_OUTPOSTWAR_SET_NEXTTIME, szGameLog));
	return TRUE;
}

BOOL CLogIOCPSocket::Process_FL_LOG_POLL_VOTE(const char* pPacket, int nLength, int &nBytesUsed)
{
	DECLARE_MESSAGE_AND_CHECK_SIZE_LOG(pPacket, nLength, nBytesUsed, T_FL_LOG_POLL_VOTE,
											MSG_FL_LOG_POLL_VOTE, pRMsg);

	char szGameLog[1024];
	ms_pLogIOCP->WriteGameLog(pRMsg->GetWriteLogString(T1_FL_LOG_POLL_VOTE, szGameLog));
	return TRUE;	
}

BOOL CLogIOCPSocket::Process_FL_LOG_POLL_DELETE_LEADERCANDIDATE(const char* pPacket, int nLength, int &nBytesUsed)
{
	DECLARE_MESSAGE_AND_CHECK_SIZE_LOG(pPacket, nLength, nBytesUsed, T_FL_LOG_POLL_DELETE_LEADERCANDIDATE,
											MSG_FL_LOG_POLL_DELETE_LEADERCANDIDATE, pRMsg);

	char szGameLog[1024];
	ms_pLogIOCP->WriteGameLog(pRMsg->GetWriteLogString(T1_FL_LOG_POLL_DELETE_LEADERCANDIDATE, szGameLog));
	return TRUE;
}

BOOL CLogIOCPSocket::Process_FL_LOG_DISMEMBER_GUILD(const char* pPacket, int nLength, int &nBytesUsed)
{
	DECLARE_MESSAGE_AND_CHECK_SIZE_LOG(pPacket, nLength, nBytesUsed, T_FL_LOG_DISMEMBER_GUILD,
											MSG_FL_LOG_DISMEMBER_GUILD, pRMsg);

	char szGameLog[1024];
	ms_pLogIOCP->WriteGameLog(pRMsg->GetWriteLogString(T1_FL_LOG_DISMEMBER_GUILD, szGameLog));
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CLogIOCPSocket::Process_FL_LOG_NOTIFY_MSG_DELETE(const char* pPacket, int nLength, int &nBytesUsed)
/// \brief		
/// \author		cmkwon
/// \date		2007-11-28 ~ 2007-11-28
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CLogIOCPSocket::Process_FL_LOG_NOTIFY_MSG_DELETE(const char* pPacket, int nLength, int &nBytesUsed)
{
	DECLARE_MESSAGE_AND_CHECK_SIZE_LOG(pPacket, nLength, nBytesUsed, T_FL_LOG_NOTIFY_MSG_DELETE,
											MSG_FL_LOG_NOTIFY_MSG_DELETE, pRMsg);

	char szGameLog[1024];
	ms_pLogIOCP->WriteGameLog(pRMsg->GetWriteLogString(T1_FL_LOG_NOTIFY_MSG_DELETE, szGameLog));
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CLogIOCPSocket::Process_FL_LOG_ITEM_ATTACH_ITEM(const char* pPacket, int nLength, int &nBytesUsed)
/// \brief		// 2008-01-23 by cmkwon, S_F, S_L: 장착/장착해제 게임 로그에 추가 - CLogIOCPSocket::Process_FL_LOG_ITEM_ATTACH_ITEM() 추가
/// \author		cmkwon
/// \date		2008-01-23 ~ 2008-01-23
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CLogIOCPSocket::Process_FL_LOG_ITEM_ATTACH_ITEM(const char* pPacket, int nLength, int &nBytesUsed)
{
	DECLARE_MESSAGE_AND_CHECK_SIZE_LOG(pPacket, nLength, nBytesUsed, T_FL_LOG_ITEM_ATTACH_ITEM,
											MSG_FL_LOG_ITEM_ATTACH_ITEM, pRMsg);

	char szGameLog[1024];
	ms_pLogIOCP->WriteGameLog(pRMsg->GetWriteLogString(T1_FL_LOG_ITEM_ATTACH_ITEM, szGameLog));
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CLogIOCPSocket::Process_FL_LOG_ITEM_DETACH_ITEM(const char* pPacket, int nLength, int &nBytesUsed)
/// \brief		// 2008-01-23 by cmkwon, S_F, S_L: 장착/장착해제 게임 로그에 추가 - CLogIOCPSocket::Process_FL_LOG_ITEM_DETACH_ITEM() 추가
/// \author		cmkwon
/// \date		2008-01-23 ~ 2008-01-23
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CLogIOCPSocket::Process_FL_LOG_ITEM_DETACH_ITEM(const char* pPacket, int nLength, int &nBytesUsed)
{
	DECLARE_MESSAGE_AND_CHECK_SIZE_LOG(pPacket, nLength, nBytesUsed, T_FL_LOG_ITEM_DETACH_ITEM,
											MSG_FL_LOG_ITEM_DETACH_ITEM, pRMsg);

	char szGameLog[1024];
	ms_pLogIOCP->WriteGameLog(pRMsg->GetWriteLogString(T1_FL_LOG_ITEM_DETACH_ITEM, szGameLog));
	return TRUE;
}



///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CLogIOCPSocket::Process_FL_LOG_ITEM_DELETE(const char* pPacket, int nLength, int &nBytesUsed)
/// \brief		// 2008-01-23 by cmkwon, S_F, S_L: 장착/장착해제 게임 로그에 추가 - CLogIOCPSocket::Process_FL_LOG_ITEM_DELETED() 추가
/// \author		cmkwon
/// \date		2008-01-23 ~ 2008-01-23
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CLogIOCPSocket::Process_FL_LOG_ITEM_DELETE(const char* pPacket, int nLength, int &nBytesUsed)
{
	DECLARE_MESSAGE_AND_CHECK_SIZE_LOG(pPacket, nLength, nBytesUsed, T_FL_LOG_ITEM_DELETE,
											MSG_FL_LOG_ITEM_DELETE, pRMsg);

	char szGameLog[1024];
	ms_pLogIOCP->WriteGameLog(pRMsg->GetWriteLogString(T1_FL_LOG_ITEM_DELETE, szGameLog));
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CLogIOCPSocket::Process_FL_LOG_USE_COUPON(const char* pPacket, int nLength, int &nBytesUsed)
/// \brief		// 2008-01-23 by cmkwon, S_F, S_L: 쿠폰 사용 게임 로그에 추가 - CLogIOCPSocket::Process_FL_LOG_USE_COUPON() 추가
/// \author		cmkwon
/// \date		2008-01-23 ~ 2008-01-23
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CLogIOCPSocket::Process_FL_LOG_USE_COUPON(const char* pPacket, int nLength, int &nBytesUsed)
{
	DECLARE_MESSAGE_AND_CHECK_SIZE_LOG(pPacket, nLength, nBytesUsed, T_FL_LOG_USE_COUPON,
											MSG_FL_LOG_USE_COUPON, pRMsg);

	char szGameLog[1024];
	ms_pLogIOCP->WriteGameLog(pRMsg->GetWriteLogString(T1_FL_LOG_USE_COUPON, szGameLog));
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CLogIOCPSocket::Process_FL_LOG_INFINITY_START(const char* pPacket, int nLength, int &nBytesUsed) {
/// \brief		인피니티 - 인피 게임로그 추가, 시작 관련 정보 
/// \author		dhjin
/// \date		2009-09-09 ~ 2010
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CLogIOCPSocket::Process_FL_LOG_INFINITY_START(const char* pPacket, int nLength, int &nBytesUsed) {
	DECLARE_MESSAGE_AND_CHECK_SIZE_LOG(pPacket, nLength, nBytesUsed, T_FL_LOG_INFINITY_START,
		MSG_FL_LOG_INFINITY_START, pRMsg);
	
	char szGameLog[1024];
	ms_pLogIOCP->WriteGameLog(pRMsg->GetWriteLogString(T1_FL_LOG_INFINITY_START, szGameLog));
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CLogIOCPSocket::Process_FL_LOG_INFINITY_START_ITEM(const char* pPacket, int nLength, int &nBytesUsed)
/// \brief		인피니티 - 인피 게임로그 추가, 시작시 아이템 복사 정보
/// \author		dhjin
/// \date		2009-09-09 ~ 2010
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CLogIOCPSocket::Process_FL_LOG_INFINITY_START_ITEM(const char* pPacket, int nLength, int &nBytesUsed) {
	DECLARE_MESSAGE_AND_CHECK_SIZE_LOG(pPacket, nLength, nBytesUsed, T_FL_LOG_INFINITY_START_ITEM,
		MSG_FL_LOG_INFINITY_START_ITEM, pRMsg);
	
	char szGameLog[1024];
	ms_pLogIOCP->WriteGameLog(pRMsg->GetWriteLogString(T1_FL_LOG_INFINITY_START_ITEM, szGameLog));
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CLogIOCPSocket::Process_FL_LOG_INFINITY_CINEMA(const char* pPacket, int nLength, int &nBytesUsed)
/// \brief		인피니티 - 인피 게임로그 추가, 연출 로그
/// \author		dhjin
/// \date		2009-09-09 ~ 2010
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CLogIOCPSocket::Process_FL_LOG_INFINITY_CINEMA(const char* pPacket, int nLength, int &nBytesUsed) {
	DECLARE_MESSAGE_AND_CHECK_SIZE_LOG(pPacket, nLength, nBytesUsed, T_FL_LOG_INFINITY_CINEMA,
		MSG_FL_LOG_INFINITY_CINEMA, pRMsg);
	
	char szGameLog[1024];
	ms_pLogIOCP->WriteGameLog(pRMsg->GetWriteLogString(T1_FL_LOG_INFINITY_CINEMA, szGameLog));
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CLogIOCPSocket::Process_FL_LOG_MONSTER_SKILL(const char* pPacket, int nLength, int &nBytesUsed)
/// \brief		인피니티 - 인피 게임로그 추가, 몬스터 스킬 사용시
/// \author		dhjin
/// \date		2009-09-09 ~ 2010
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CLogIOCPSocket::Process_FL_LOG_MONSTER_SKILL(const char* pPacket, int nLength, int &nBytesUsed) {
	DECLARE_MESSAGE_AND_CHECK_SIZE_LOG(pPacket, nLength, nBytesUsed, T_FL_LOG_MONSTER_SKILL,
		MSG_FL_LOG_MONSTER_SKILL, pRMsg);
	
	char szGameLog[1024];
	ms_pLogIOCP->WriteGameLog(pRMsg->GetWriteLogString(T1_FL_LOG_MONSTER_SKILL, szGameLog));
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CLogIOCPSocket::Process_FL_LOG_HPACTION_TALK(const char* pPacket, int nLength, int &nBytesUsed)
/// \brief		인피니티 - 인피 게임로그 추가, 몬스터 대화 사용시
/// \author		dhjin
/// \date		2009-09-09 ~ 2010
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CLogIOCPSocket::Process_FL_LOG_HPACTION_TALK(const char* pPacket, int nLength, int &nBytesUsed) {
	DECLARE_MESSAGE_AND_CHECK_SIZE_LOG(pPacket, nLength, nBytesUsed, T_FL_LOG_HPACTION_TALK,
		MSG_FL_LOG_HPACTION_TALK, pRMsg);
	
	char szGameLog[1024];
	ms_pLogIOCP->WriteGameLog(pRMsg->GetWriteLogString(T1_FL_LOG_HPACTION_TALK, szGameLog));
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CLogIOCPSocket::Process_FL_LOG_INFINITY_FIN(const char* pPacket, int nLength, int &nBytesUsed)
/// \brief		인피니티 - 인피 게임로그 추가, 종료 관련 정보 
/// \author		dhjin
/// \date		2009-09-09 ~ 2010
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CLogIOCPSocket::Process_FL_LOG_INFINITY_FIN(const char* pPacket, int nLength, int &nBytesUsed) {
	DECLARE_MESSAGE_AND_CHECK_SIZE_LOG(pPacket, nLength, nBytesUsed, T_FL_LOG_INFINITY_FIN,
		MSG_FL_LOG_INFINITY_FIN, pRMsg);
	
	char szGameLog[1024];
	ms_pLogIOCP->WriteGameLog(pRMsg->GetWriteLogString(T1_FL_LOG_INFINITY_FIN, szGameLog));
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CLogIOCPSocket::Process_FL_LOG_INFINITY_FIN_ITEM(const char* pPacket, int nLength, int &nBytesUsed)
/// \brief		인피니티 - 인피 게임로그 추가, 종료시 아이템 복사 정보
/// \author		dhjin
/// \date		2009-09-09 ~ 2010
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CLogIOCPSocket::Process_FL_LOG_INFINITY_FIN_ITEM(const char* pPacket, int nLength, int &nBytesUsed) {
	DECLARE_MESSAGE_AND_CHECK_SIZE_LOG(pPacket, nLength, nBytesUsed, T_FL_LOG_INFINITY_FIN_ITEM,
		MSG_FL_LOG_INFINITY_FIN_ITEM, pRMsg);
	
	char szGameLog[1024];
	ms_pLogIOCP->WriteGameLog(pRMsg->GetWriteLogString(T1_FL_LOG_INFINITY_FIN_ITEM, szGameLog));
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CLogIOCPSocket::Process_FL_LOG_INFINITY_FIN_ALIVE_KEY_MONSTER(const char* pPacket, int nLength, int &nBytesUsed)
/// \brief		인피니티 - 인피 게임로그 추가, 종료시 살아있는 몬스터 정보
/// \author		dhjin
/// \date		2009-09-09 ~ 2010
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CLogIOCPSocket::Process_FL_LOG_INFINITY_FIN_ALIVE_KEY_MONSTER(const char* pPacket, int nLength, int &nBytesUsed) {
	DECLARE_MESSAGE_AND_CHECK_SIZE_LOG(pPacket, nLength, nBytesUsed, T_FL_LOG_INFINITY_FIN_ALIVE_KEY_MONSTER,
		MSG_FL_LOG_INFINITY_FIN_ALIVE_KEY_MONSTER, pRMsg);
	
	char szGameLog[1024];
	ms_pLogIOCP->WriteGameLog(pRMsg->GetWriteLogString(T1_FL_LOG_INFINITY_FIN_ALIVE_KEY_MONSTER, szGameLog));
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CLogIOCPSocket::Process_FL_LOG_INFINITY_LEAVE_ITEM(const char* pPacket, int nLength, int &nBytesUsed)
/// \brief		인피니티 - 인피 게임로그 추가, 인피 탈퇴시 아이템 복사 정보
/// \author		dhjin
/// \date		2009-09-09 ~ 2010
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CLogIOCPSocket::Process_FL_LOG_INFINITY_LEAVE_ITEM(const char* pPacket, int nLength, int &nBytesUsed) {
	DECLARE_MESSAGE_AND_CHECK_SIZE_LOG(pPacket, nLength, nBytesUsed, T_FL_LOG_INFINITY_LEAVE_ITEM,
		MSG_FL_LOG_INFINITY_FIN_ITEM, pRMsg);
	
	char szGameLog[1024];
	ms_pLogIOCP->WriteGameLog(pRMsg->GetWriteLogString(T1_FL_LOG_INFINITY_LEAVE_ITEM, szGameLog));
	return TRUE;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CLogIOCPSocket::Process_FL_LOG_EVENT_PARTICIPATION_RATE(const char* pPacket, int nLength, int &nBytesUsed)
/// \brief		GLogDB 관련 -
/// \author		shcho
/// \date		2010-06-01
/// \warning	
///
/// \param		
/// \return		
/////////////////////////////////////////////////////////////////////////////// 
BOOL CLogIOCPSocket::Process_FL_LOG_EVENT_PARTICIPATION_RATE(const char* pPacket, int nLength, int &nBytesUsed) {

#ifdef S_GLOG_HSSON
	DECLARE_MESSAGE_AND_CHECK_SIZE_LOG(pPacket, nLength, nBytesUsed, T_FL_LOG_EVENT_PARTICIPATION_RATE,
		MSG_FL_LOG_EVENT_PARTICIPATION_RATE, pRMsg);

	MSG_FL_LOG_EVENT_PARTICIPATION_RATE *pGLogMsg = new MSG_FL_LOG_EVENT_PARTICIPATION_RATE;
	*pGLogMsg = *pRMsg;
	ms_pLogIOCP->m_pGLogDBManager->MakeAndEnqueueQuery(QT_FL_GLOG_EVENT_PARTICIPATION_RATE,NULL,pGLogMsg,NULL);
#endif // S_GLOG_HSSON

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CLogIOCPSocket::Process_FL_LOG_PET_LEVEL(const char* pPacket, int nLength, int &nBytesUsed)
/// \brief		2011-09-15 by hskim, 파트너 시스템 2차 - 파트너 레벨 변경
/// \author		hskim
/// \date		2011-09-15
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CLogIOCPSocket::Process_FL_LOG_PET_LEVEL(const char* pPacket, int nLength, int &nBytesUsed)
{
	DECLARE_MESSAGE_AND_CHECK_SIZE_LOG(pPacket, nLength, nBytesUsed, T_FL_LOG_PET_LEVEL, MSG_FL_LOG_PET_LEVEL, msgLevel);

	char szGameLog[1024];
	ms_pLogIOCP->WriteGameLog(msgLevel->GetWriteLogString(T1_FL_LOG_PET_LEVEL, szGameLog));

	return TRUE;
}

// start 2012-01-08 by hskim, GLog 2차
BOOL CLogIOCPSocket::Process_FL_LOG_CONNECT_USER_NEW(const char* pPacket, int nLength, int &nBytesUsed)
{
	DECLARE_MESSAGE_AND_CHECK_SIZE_LOG(pPacket, nLength, nBytesUsed, T_FL_LOG_CONNECT_USER_NEW, MSG_FL_LOG_CONNECT_USER_NEW, msgUserNew);

	//char szGameLog[1024];
	//ms_pLogIOCP->WriteGameLog(msgLevel->GetWriteLogString(T1_FL_LOG_CONNECT_USER_NEW, szGameLog));

#ifdef S_GLOG_2ND_HSKIM
	QPARAM_GLOG_CONNECT_USER_ACCOUNT_NAME *pGLog2ndMsg = new QPARAM_GLOG_CONNECT_USER_ACCOUNT_NAME;
	util::strncpy(pGLog2ndMsg->szAccountName, msgUserNew->AccountName, SIZE_MAX_ACCOUNT_NAME);
	ms_pLogIOCP->m_pGLogDBManager->MakeAndEnqueueQuery(QT_FL_GLOG_TB_USER_LocalUserConnectInfo_NewCountInc, NULL, pGLog2ndMsg, NULL);
#endif // S_GLOG_2ND_HSKIM

	return TRUE;
}

BOOL CLogIOCPSocket::Process_FL_LOG_CONNECT_USER_LOGIN(const char* pPacket, int nLength, int &nBytesUsed)
{
	DECLARE_MESSAGE_AND_CHECK_SIZE_LOG(pPacket, nLength, nBytesUsed, T_FL_LOG_CONNECT_USER_LOGIN, MSG_FL_LOG_CONNECT_USER_LOGIN, msgUserLogin);

	//char szGameLog[1024];
	//ms_pLogIOCP->WriteGameLog(msgLevel->GetWriteLogString(T1_FL_LOG_CONNECT_USER_LOGIN, szGameLog));

#ifdef S_GLOG_2ND_HSKIM
	QPARAM_GLOG_CONNECT_USER_ACCOUNT_NAME *pGLog2ndMsg = new QPARAM_GLOG_CONNECT_USER_ACCOUNT_NAME;
	util::strncpy(pGLog2ndMsg->szAccountName, msgUserLogin->AccountName, SIZE_MAX_ACCOUNT_NAME);
	ms_pLogIOCP->m_pGLogDBManager->MakeAndEnqueueQuery(QT_FL_GLOG_TB_CONNECT_USER, NULL, pGLog2ndMsg, NULL);
#endif // S_GLOG_2ND_HSKIM

	return TRUE;
}
// end 2011-12-12 by hskim, GLog 2차

// start 2012-01-16 by hskim, 통계 - 화패
BOOL CLogIOCPSocket::Process_FL_LOG_STATISTICS_MONEY(const char* pPacket, int nLength, int &nBytesUsed)
{
	DECLARE_MESSAGE_AND_CHECK_SIZE_LOG(pPacket, nLength, nBytesUsed, T_FL_LOG_STATISTICS_MONEY, MSG_FL_LOG_STATISTICS_MONEY, msgMoney);

	MSG_FL_LOG_STATISTICS_MONEY *pMoney = new MSG_FL_LOG_STATISTICS_MONEY;
	*pMoney = *msgMoney;
	ms_pLogIOCP->m_pAtumLogDBManager->MakeAndEnqueueQuery(QT_FL_LOG_STATISTICS_MONEY, NULL, pMoney, NULL);
	
	return TRUE;
}
// end 2012-01-16 by hskim, 통계 - 화패

// 2012-10-10 by hskim, 기간제 속성 구현 (기간제 외형)
BOOL CLogIOCPSocket::Process_FL_LOG_FIXED_TERM_SHAPE_START(const char* pPacket, int nLength, int &nBytesUsed) 
{
	DECLARE_MESSAGE_AND_CHECK_SIZE_LOG(pPacket, nLength, nBytesUsed, T_FL_LOG_FIXED_TERM_SHAPE_START,
		MSG_FL_LOG_FIXED_TERM_SHAPE_START, pRMsg);
	
	char szGameLog[1024];
	ms_pLogIOCP->WriteGameLog(pRMsg->GetWriteLogString(T1_FL_LOG_FIXED_TERM_SHAPE_START, szGameLog));

	return TRUE;
}

BOOL CLogIOCPSocket::Process_FL_LOG_FIXED_TERM_SHAPE_END(const char* pPacket, int nLength, int &nBytesUsed) 
{
	DECLARE_MESSAGE_AND_CHECK_SIZE_LOG(pPacket, nLength, nBytesUsed, T_FL_LOG_FIXED_TERM_SHAPE_END,
		MSG_FL_LOG_FIXED_TERM_SHAPE_END, pRMsg);
	
	char szGameLog[1024];
	ms_pLogIOCP->WriteGameLog(pRMsg->GetWriteLogString(T1_FL_LOG_FIXED_TERM_SHAPE_END, szGameLog));

	return TRUE;
}
// end 2012-10-10 by hskim, 기간제 속성 구현 (기간제 외형)

// start 2012-11-21 by khkim, GLog (수정)
BOOL CLogIOCPSocket::Process_FL_LOG_ACCOUNTCONNECT(const char* pPacket, int nLength, int &nBytesUsed)
{
#ifdef S_GLOG_3ND_KHK
	DECLARE_MESSAGE_AND_CHECK_SIZE_LOG(pPacket, nLength, nBytesUsed, T_FL_LOG_ACCOUNTCONNECT, MSG_FL_LOG_ACCOUNTCONNECT, msgAccountConnect);
	MSG_FL_LOG_ACCOUNTCONNECT *pAccountConnect = new MSG_FL_LOG_ACCOUNTCONNECT;
	pAccountConnect->LogType		= msgAccountConnect->LogType;
	pAccountConnect->UserSerial		= msgAccountConnect->UserSerial;
	pAccountConnect->IP				= msgAccountConnect->IP;
	pAccountConnect->GameServerID	= msgAccountConnect->GameServerID;
	ms_pLogIOCP->m_pGLogDBManager->MakeAndEnqueueQuery(QT_FL_LOG_ACCOUNTCONNECT, NULL, pAccountConnect, NULL);
#endif
	return TRUE;
}

BOOL CLogIOCPSocket::Process_FL_LOG_CHARDATA(const char* pPacket,int nLength,int &nBytesUsed)
{
#ifdef S_GLOG_3ND_KHK
	DECLARE_MESSAGE_AND_CHECK_SIZE_LOG(pPacket, nLength, nBytesUsed, T_FL_LOG_CHARDATA, MSG_FL_LOG_CHARDATA, msgCharData);
	MSG_FL_LOG_CHARDATA *pCharData = new MSG_FL_LOG_CHARDATA;
	pCharData->LogType		= msgCharData->LogType;
	pCharData->UserSerial	= (UID64_t)msgCharData->UserSerial;
	pCharData->CharSerial	= (UID64_t)msgCharData->CharSerial;
	pCharData->Class		= (SHORT)msgCharData->Class;
	pCharData->Lv			= (SHORT)msgCharData->Lv;
	pCharData->Exp			= (UID64_t)msgCharData->Exp;
	pCharData->GameMoney	= (UID64_t)msgCharData->GameMoney;
	pCharData->Playtime		= (UID64_t)msgCharData->Playtime;
	pCharData->GameServerID	= (int)msgCharData->GameServerID;


// 2013-01-03 by khkim, GLog (버그 확인용)
#ifdef S_KOR_TEST_GLOG_CHARDATA
	if(pCharData->UserSerial < 0 || pCharData->UserSerial > 7000000 )
	{
		g_pLogGlobal->WriteSystemLogEX(TRUE,"[DEBUG] GLog #100 [%d,%I64d,%I64d,%d,%d,%I64d,%I64d,%I64d,%d]\r\n ", 
			pCharData->LogType,		pCharData->UserSerial,	pCharData->CharSerial,
			pCharData->Class,		pCharData->Lv,			pCharData->Exp, 
			pCharData->GameMoney,	pCharData->Playtime,	pCharData->GameServerID);
	}
#endif 
// end 2013-01-03 by khkim, GLog (버그 확인용)	

	ms_pLogIOCP->m_pGLogDBManager->MakeAndEnqueueQuery(QT_FL_LOG_CHARDATA, NULL, pCharData, NULL);
#endif
	return TRUE;
}

BOOL CLogIOCPSocket::Process_FL_LOG_ITEM(const char* pPacket, int nLength, int &nBytesUsed)
{
#ifdef S_GLOG_3ND_KHK
	DECLARE_MESSAGE_AND_CHECK_SIZE_LOG(pPacket, nLength, nBytesUsed, T_FL_LOG_ITEM, MSG_FL_LOG_ITEM, msgItem);
	MSG_FL_LOG_ITEM *pItem	= new MSG_FL_LOG_ITEM;
	pItem->LogType			= msgItem->LogType;
	pItem->ItemSerial		= msgItem->ItemSerial;
	pItem->Count1			= msgItem->Count1;
	pItem->Count2			= msgItem->Count2;
	pItem->CharSerial		= msgItem->CharSerial;	
	pItem->CharLv			= msgItem->CharLv;
	pItem->GameServerID		= msgItem->GameServerID;
	ms_pLogIOCP->m_pGLogDBManager->MakeAndEnqueueQuery(QT_FL_LOG_ITEM, NULL, pItem, NULL);
#endif
	return TRUE;
}

BOOL CLogIOCPSocket::Process_FL_LOG_ITEMSTATE(const char* pPacket, int nLength, int &nBytesUsed)
{
#ifdef S_GLOG_3ND_KHK
	DECLARE_MESSAGE_AND_CHECK_SIZE_LOG(pPacket, nLength, nBytesUsed, T_FL_LOG_ITEMSTATE, MSG_FL_LOG_ITEMSTATE, msgItemState);
	MSG_FL_LOG_ITEMSTATE *pItemState = new MSG_FL_LOG_ITEMSTATE;
	pItemState->LogType			= msgItemState->LogType;
	pItemState->ItemSerial		= msgItemState->ItemSerial;
	pItemState->Count			= msgItemState->Count;
	pItemState->CharSerial		= msgItemState->CharSerial;
	pItemState->CharLv			= msgItemState->CharLv;
	pItemState->GameServerID	= msgItemState->GameServerID;
	ms_pLogIOCP->m_pGLogDBManager->MakeAndEnqueueQuery(QT_FL_LOG_ITEMSTATE, NULL, pItemState, NULL);
#endif
	return TRUE;
}

BOOL CLogIOCPSocket::Process_FL_LOG_SERVER(const char* pPacket, int nLength, int &nBytesUsed)
{
#ifdef S_GLOG_3ND_KHK
	DECLARE_MESSAGE_AND_CHECK_SIZE_LOG(pPacket, nLength, nBytesUsed, T_FL_LOG_SERVER, MSG_FL_LOG_SERVER, msgServer);
	MSG_FL_LOG_SERVER *pServer = new MSG_FL_LOG_SERVER;
	pServer->LogType		= msgServer->LogType;
	pServer->GameServerID	= msgServer->GameServerID;
	ms_pLogIOCP->m_pGLogDBManager->MakeAndEnqueueQuery(QT_FL_LOG_SERVER, NULL, pServer, NULL);
#endif
	return TRUE;
}
// end 2012-11-21 by khkim, GLog

// 2013-05-31 by jhseol,bckim 아머 컬렉션 - GameLog 추가
BOOL CLogIOCPSocket::Process_FL_LOG_COLLECTION_STATE(const char* pPacket, int nLength, int &nBytesUsed) 
{
	DECLARE_MESSAGE_AND_CHECK_SIZE_LOG(pPacket, nLength, nBytesUsed, T_FL_LOG_COLLECTION_STATE,
		MSG_FL_LOG_COLLECTION_STATE, pRMsg);
	
	char szGameLog[1024];
	ms_pLogIOCP->WriteGameLog(pRMsg->GetWriteLogString(T1_FL_LOG_COLLECTION_STATE, szGameLog));
	
	return TRUE;
}

BOOL CLogIOCPSocket::Process_FL_LOG_COLLECTION_SHAPE_CHANGE(const char* pPacket, int nLength, int &nBytesUsed) 
{
	DECLARE_MESSAGE_AND_CHECK_SIZE_LOG(pPacket, nLength, nBytesUsed, T_FL_LOG_COLLECTION_SHAPE_CHANGE,
		MSG_FL_LOG_COLLECTION_SHAPE_CHANGE, pRMsg);
	
	char szGameLog[1024];
	ms_pLogIOCP->WriteGameLog(pRMsg->GetWriteLogString(T1_FL_LOG_COLLECTION_SHAPE_CHANGE, szGameLog));
	
	return TRUE;
}
// end 2013-05-31 by jhseol,bckim 아머 컬렉션 - GameLog 추가

// 2013-07-26 by jhseol, 타 계정 세력변경 - GameLog 추가
BOOL CLogIOCPSocket::Process_FL_LOG_ACCOUNT_INFL_CHANGE_BY_ADMIN(const char* pPacket, int nLength, int &nBytesUsed) 
{
	DECLARE_MESSAGE_AND_CHECK_SIZE_LOG(pPacket, nLength, nBytesUsed, T_FL_LOG_ACCOUNT_INFL_CHANGE_BY_ADMIN,
		MSG_FL_LOG_ACCOUNT_INFL_CHANGE_BY_ADMIN, pRMsg);
	
	char szGameLog[1024];
	ms_pLogIOCP->WriteGameLog(pRMsg->GetWriteLogString(T1_FL_LOG_ACCOUNT_INFL_CHANGE_BY_ADMIN, szGameLog));
	
	return TRUE;
}
// end 2013-07-26 by jhseol, 타 계정 세력변경 - GameLog 추가