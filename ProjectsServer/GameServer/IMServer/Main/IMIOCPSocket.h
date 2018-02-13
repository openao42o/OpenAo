// IMIOCPSocket.h: interface for the CIMIOCPSocket class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IMIOCPSOCKET_H__9C62870A_F7ED_49A2_9583_956845931320__INCLUDED_)
#define AFX_IMIOCPSOCKET_H__9C62870A_F7ED_49A2_9583_956845931320__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "IOCP.h"
#include "AtumProtocol.h"
#include "PreIMWinsocket.h"
#include "Guild.h"
#include "CountdownChecker.h"
#include "ChatRoomManager.h"				// 2008-06-16 by dhjin, EP3 채팅방 -
#include "ChatRoom.h"						// 2008-06-16 by dhjin, EP3 채팅방 -

typedef mt_vector<FRIENDINFO>			mtvectorFRIENDINFO;

class CIMIOCP;
class CIMIOCPSocket;
class CIMParty;

class CIMIOCPSocket : public CIOCPSocket
{
	friend class CPreIMWinSocket;
public:
	CIMIOCPSocket();
	virtual ~CIMIOCPSocket();

public:
	CHARACTER					m_character;
	BOOL						m_bGameEndRoutineFlag;
	CMTCriticalSection			m_mtCritSecForGameEndRoutine;
	INT							m_nLoginType;
	CIMIOCPSocket				*m_pCurrentFieldServerSocket;		// 게임 사용자가 접속해있는 필드 서버에 대한 포인터
	CIMParty					*m_pIMParty;
	MSG_FI_CASH_PREMIUM_CARD_INFO m_MembershipInfo;					// 2006-09-15 by dhjin, 맴버쉽 상태 정보

	// 길드 관련
	UID32_t						m_uidGuildUniqueNumber;

	// 2008-05-15 by dhjin, EP3 - 채팅 시스템 변경
//	BitFlag8_t					m_enableChatFlag8;
	BitFlag32_t					m_enableChatFlag32;
	SERVER_ID					m_FieldServerID;
	mtvectorFRIENDINFO			m_mtvectorFriendList;
	mtvectorFRIENDINFO			m_mtvectorRejectList;
	int							m_nEachOtherFriendCnts;				// 2006-07-19 by cmkwon, 서로 등록한 친구 카운트

	// 카운트다운체크 관련
	CCountdownChecker			m_countdownChecker;

	EN_CHECK_TYPE				m_VoipType;							// 2008-06-17 by dhjin, EP3 VOIP -

	BOOL						m_bChatRoomUseing;					// 2008-06-17 by dhjin, EP3 채팅방 - 채팅방 참여 중인지에 대한 값
	INT							m_nChatRoomNum;						// 2008-06-18 by dhjin, EP3 채팅방 - 채팅방 번호	

	static CIMIOCP				*ms_pIMIOCP;

	INT							m_nTeamNum;							// 2012-04-12 by jhseol, 아레나 추가개발 : 자동파티 - 팀 번호
	BOOL						m_bArenaIMServerCheck;				// 2012-04-12 by jhseol, 아레나 추가개발 - 자동파티 : 아레나 서버인지 체크

public:		
	inline const CHARACTER* GetCharacter(void);
	BOOL IsValidCharacter(void);

	BOOL SetChatCharacter(CHARACTER *pCharacter);
	BOOL FilterChattingString(char *chatString);

	void GameStartRoutineIM(MSG_FI_CONNECT_NOTIFY_GAMESTART *i_FINotifyGameStart, CIMIOCPSocket *i_pF2IISock);
	void GameEndRoutineIM(void);
	void CharacterDeadRoutine(UID32_t i_uidAttackerGuildUID);

	void SetGuildUniqueNumber(UID32_t i_uidGuild){			m_uidGuildUniqueNumber = i_uidGuild;
															this->m_character.GuildUniqueNumber	= i_uidGuild;		// 2008-06-20 by dhjin, EP3 - 여단 수정 사항 - 
													};

	void SetCurrentFieldServerSocket(CIMIOCPSocket *i_pFServerSocket){		m_pCurrentFieldServerSocket = i_pFServerSocket;};

	BOOL SendMsgToCurrentFieldServerSocket(BYTE *i_pData, int i_nLength);
	void SendOnlineEachOtherFriendCntsToFieldServer(void);

	CIMParty *GetIMParty(void);
	void SetIMParty(CIMParty *i_pParty);

	BOOL IsExistPremiumCard();						// 2006-09-15 by dhjin, 맴버쉽 유저인지 체크
	// 디버그용, MessageType을 클라이언트에게 보낸다
	BOOL				m_bSendMessgeTypeDBG;

	//////////////////////////////////////////////////////////////////////////
	// 2008-06-02 by dhjin, EP3 편대 수정
	void SendPartyListInfo(PartyID_t i_nStartNum);		// 2008-06-02 by dhjin, EP3 편대 수정 - 편대 정보 리스트
	INT	 MakeMSG_IC_PARTY_LIST_INFO_OK(SPARTY_LIST_INFO * o_pPartyListInfo, PartyID_t i_nStartNum, INT * o_nTotalCount);			// 2008-06-02 by dhjin, EP3 편대 수정 - 편대 정보 리스트	

	//void SendVoipType(EN_CHECK_TYPE i_NewVoipType);			// 2008-06-17 by dhjin, EP3 VOIP -

	//////////////////////////////////////////////////////////////////////////
	// 2008-06-17 by dhjin, EP3 채팅방 -
	void SendChatRoomListInfo(INT i_nStartNum);		// 2008-06-17 by dhjin, EP3 채팅방 -
	INT	 MakeMSG_IC_CHATROOM_LIST_INFO_OK(SCHATROOM_LIST_INFO * o_pChatRoomListInfo, INT i_nStartNum, INT * o_nTotalCount);		// 2008-06-17 by dhjin, EP3 채팅방 -
	
	//////////////////////////////////////////////////////////////////////////
	// 2009-03-05 by cmkwon, 통합 아레나 이용시 친구 리스트 보너스 경험치 관련 버그 수정 - 
	BOOL				m_bFirstTimeLoadFriendList;			

	///////////////////////////////////////////////////////////////////////////////
	// 2009-08-03 by cmkwon, EP3-4 편대 대형 스킬 구현 - 
	void SendFieldServPartyChangeFormationSkill(UID32_t i_PartyMasterCharcUID, BOOL i_bOnFlag, BYTE i_byFormationTy=FLIGHT_FORM_NONE);

public:
	virtual BOOL OnRecvdPacket(const char* pPacket, int nLength, BYTE nSeq, char* pPeerIP = "", int nPeerPort = 0, SThreadInfo *i_pThreadInfo=NULL);
	virtual void OnConnect(void);
	virtual void OnClose(int reason = 0);

	void SendString128(BYTE string128_type, char* pFormat, ...);
//	BOOL SendChatMessage(BitFlag8_t i_CHAT_TYPE, BYTE *i_pChatData, int i_nSize, BYTE i_byInfluenceMask=INFLUENCE_TYPE_ALL_MASK);
	BOOL SendChatMessage(BitFlag16_t i_CHAT_TYPE, BYTE *i_pChatData, int i_nSize, BYTE i_byInfluenceMask=INFLUENCE_TYPE_ALL_MASK);		// 2008-05-15 by dhjin, EP3 - 채팅 시스템 변경

	void PRINT_DEBUG_MESSAGE(char*& msg);
	mt_lock	PRINT_DEBUG_MESSAGE_LOCK;

	template<typename... args_t>
	void DBGOUT2(BitFlag8_t bType, char* pFormat, args_t... args)
	{
		if (bType == DO2_NONE) return;

		char szLogMsg[1024];

		sprintf(szLogMsg, pFormat, args...);

		if (COMPARE_BIT_FLAG(bType, DO2_DBGOUT)) DBGOUT(szLogMsg);
		if (COMPARE_BIT_FLAG(bType, DO2_SYSLOG)) g_pGlobal->WriteSystemLog(szLogMsg, TRUE);
		if (COMPARE_BIT_FLAG(bType, DO2_STR128)) SendString128(STRING_128_DEBUG_L3, szLogMsg);
	}

	// response function : by DB
	BOOL ResAuthChatLogin(CHARACTER *pCharacter, int nLoginType, int nErr = 0 );

	// 2009-03-05 by cmkwon, 통합 아레나 이용시 친구 리스트 보너스 경험치 관련 버그 수정 - ResLoadingFriendsAndRejects() 함수에 인자추가, BOOL i_bFirstTimeLoadFriendList
	BOOL ResLoadingFriendsAndRejects(mtvectorFRIENDINFO *i_pvectorFriendList, mtvectorFRIENDINFO *i_pvectorRejectList, BOOL i_bFirstTimeLoadFriendList);
// 2008-10-24 by cmkwon, 친구리스트 전송 버그 수정 - 아래의 함수로 변경함
//	BOOL MakeMsg_MSG_IC_CHAT_FRIENDLIST_LOADING_OK(MSG_IC_CHAT_FRIENDLIST_LOADING_OK *io_pFriendListAll, FRIENDINFO *io_pFriendInfo, int i_nMaxCount, BOOL i_bAllFlag=FALSE, CIMIOCPSocket **o_ArrOnlineIISocketPtr=NULL);
//	BOOL MakeMsg_MSG_IC_CHAT_REJECTLIST_LOADING_OK(MSG_IC_CHAT_REJECTLIST_LOADING_OK *io_pRejectListAll, FRIENDINFO *io_pFriendInfo, int i_nMaxCount);
	BOOL SendFriendListLoadingOK(CIMIOCPSocket **o_ArrOnlineIISocketPtr=NULL);	// 2008-10-24 by cmkwon, 친구리스트 전송 버그 수정 - 
	BOOL SendFriendListRefreshOK(void);			// 2008-10-24 by cmkwon, 친구리스트 전송 버그 수정 - 
	BOOL SendRejectListLoadingOK(void);			// 2008-10-24 by cmkwon, 친구리스트 전송 버그 수정 - 
	
	FRIENDINFO *FindFriendInfoByCharacterName(char *i_szCharacterName);
	mtvectorFRIENDINFO::iterator FindFriendInfoByCharacterNameItr(char *i_szCharacterName);
	BOOL IsExistFriendList(char *i_szCharacterName);
	BOOL IncreaseOnlineEachOtherFriendCnts(char *i_szOnlineCharacterName);
	BOOL DecreaseOnlineEachOtherFriendCnts(char *i_szOfflineCharacterName);
	BOOL InsertToFriendList(FRIENDINFO *i_pFriendInfo, BOOL i_bEachOtherFriend=FALSE);
	BOOL DeleteFromFriendList(char *i_szCharacterName, BOOL i_bEachOtherFriend=FALSE);
	FRIENDINFO *IsCharacterNameFromRejectList(char *i_szCharacterName);
	mtvectorFRIENDINFO::iterator IsCharacterNameFromRejectListItr(char *i_szCharacterName);
	BOOL InsertToRejectList(FRIENDINFO *i_pFriendInfo);
	BOOL DeleteFromRejectList(char *i_szCharacterName);
	void SetEachOtherFriendCnts(int i_nFriendCnts);
	
	void ChangedCharacterInfluenceType(BYTE i_byInfluenceTy);

	// 2007-10-06 by dhjin, 부지도자가 여단 생성시 필드서버로 정보 전송
	void SendFieldCreateGuildBySubLeader();

	// Client - Chating
	ProcessResult Process_IC_CONNECT_LOGIN(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_IC_CHAT_ALL(const char* pPacket, int nLength, int &nBytesUsed, SThreadInfo *i_pThreadInfo);
	ProcessResult Process_IC_CHAT_MAP(const char* pPacket, int nLength, int &nBytesUsed, SThreadInfo *i_pThreadInfo);
	ProcessResult Process_IC_CHAT_REGION(const char* pPacket, int nLength, int &nBytesUsed, SThreadInfo *i_pThreadInfo);
	ProcessResult Process_IC_CHAT_GUILD(const char* pPacket, int nLength, int &nBytesUsed, SThreadInfo *i_pThreadInfo);
	ProcessResult Process_IC_CHAT_PARTY(const char* pPacket, int nLength, int &nBytesUsed, SThreadInfo *i_pThreadInfo);
	ProcessResult Process_IC_CHAT_PTOP(const char* pPacket, int nLength, int &nBytesUsed, SThreadInfo *i_pThreadInfo);
	ProcessResult Process_IC_CHAT_CHANGE_CHAT_FLAG(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_IC_CHAT_SELL_ALL(const char* pPacket, int nLength, int &nBytesUsed, SThreadInfo *i_pThreadInfo);
	ProcessResult Process_IC_CHAT_CASH_ALL(const char* pPacket, int nLength, int &nBytesUsed, SThreadInfo *i_pThreadInfo);
	ProcessResult Process_IC_CHAT_INFLUENCE_ALL(const char* pPacket, int nLength, int &nBytesUsed, SThreadInfo *i_pThreadInfo);
	ProcessResult Process_IC_CHAT_ARENA(const char* pPacket, int nLength, int &nBytesUsed, SThreadInfo *i_pThreadInfo);
	ProcessResult Process_IC_CHAT_WAR(const char* pPacket, int nLength, int &nBytesUsed, SThreadInfo *i_pThreadInfo);		// 2008-05-19 by dhjin, EP3 - 채팅 시스템 변경, 전쟁 채팅
	ProcessResult Process_IC_CHAT_CNC(const char* pPacket, int nLength, int &nBytesUsed, SThreadInfo *i_pThreadInfo);
	ProcessResult Process_IC_CHAT_CHATROOM(const char* pPacket, int nLength, int &nBytesUsed, SThreadInfo *i_pThreadInfo);		// 2008-06-18 by dhjin, EP3 채팅방 - 
	ProcessResult Process_IC_CHAT_INFINITY(const char* pPacket, int nLength, int &nBytesUsed, SThreadInfo *i_pThreadInfo);		// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 채팅

	ProcessResult Process_IC_CHAT_FRIENDLIST_AND_REJECTLIST_LOADING(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_IC_CHAT_FRIENDLIST_INSERT(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_IC_CHAT_FRIENDLIST_DELETE(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_IC_CHAT_FRIENDLIST_REFRESH(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_IC_CHAT_REJECTLIST_INSERT(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_IC_CHAT_REJECTLIST_DELETE(const char* pPacket, int nLength, int &nBytesUsed);


	BOOL SendToFieldServerChatBitFlag(void);

	// Client - Party
	ProcessResult Process_IC_PARTY_CREATE(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_IC_PARTY_GET_MEMBER(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_IC_PARTY_GET_ALL_MEMBER(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_IC_PARTY_LEAVE(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_IC_PARTY_TRANSFER_MASTER(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_IC_PARTY_BAN_MEMBER(const char* pPacket, int nLength, int &nBytesUsed);

// 2009-08-03 by cmkwon, EP3-4 편대 대형 스킬 구현 - 사용하지 않는 프로토콜임
//	ProcessResult Process_IC_PARTY_DISMEMBER(const char* pPacket, int nLength, int &nBytesUsed);

	ProcessResult Process_IC_PARTY_CHANGE_FLIGHT_FORMATION(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_IC_PARTY_GET_FLIGHT_POSITION(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_IC_PARTY_CHANGE_FLIGHT_POSITION(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_IC_PARTY_CANCEL_FLIGHT_POSITION(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_IC_PARTY_ALL_FLIGHT_POSITION(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_IC_PARTY_REQUEST_PARTYINFO_FROM_A_TO_M(const char* pPacket, int nLength, int &nBytesUsed);	// 2008-02-28 by dhjin, 아레나 통합 - 
	ProcessResult Process_IC_PARTY_LEAVE_FROM_M_TO_A(const char* pPacket, int nLength, int &nBytesUsed);				// 2008-02-28 by dhjin, 아레나 통합 -	
	ProcessResult Process_IC_PARTY_LEAVE_FROM_A_TO_M(const char* pPacket, int nLength, int &nBytesUsed);				// 2008-02-28 by dhjin, 아레나 통합 -	
	ProcessResult Process_IC_PARTY_LIST_INFO(const char* pPacket, int nLength, int &nBytesUsed);				// 2008-06-02 by dhjin, EP3 편대 수정 - 편대 정보 리스트 요청	
	ProcessResult Process_IC_PARTY_JOIN_FREE(const char* pPacket, int nLength, int &nBytesUsed);				// 2008-06-02 by dhjin, EP3 편대 수정 - 편대 자유 참여
	ProcessResult Process_IC_PARTY_CHANGE_INFO(const char* pPacket, int nLength, int &nBytesUsed);				// 2008-06-02 by dhjin, EP3 편대 수정 - 편대 자유 참여
	ProcessResult Process_IC_PARTY_RECOMMENDATION_MEMBER(const char* pPacket, int nLength, int &nBytesUsed);	// 2008-06-04 by dhjin, EP3 편대 수정 - 추천 케릭터 요청
	ProcessResult Process_IC_PARTY_GET_AUTO_PARTY_INFO(const char* pPacket, int nLength, int &nBytesUsed);		// 2009-09-09 ~ 2010-01-26 by dhjin, 인피니티 - 자동 편대

	// Field Server - Party
	ProcessResult Process_FI_PARTY_ACCEPT_INVITE_OK(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FI_PARTY_ARENA_TEAM_NUM(const char* pPacket, int nLength, int &nBytesUsed);	// 2012-04-12 by jhseol, 아레나 추가개발 - 자동파티 : 같은 팀인지 체크할 팀번호 설정
	ProcessResult Process_FI_PARTY_UPDATE_ITEM_POS(const char* pPacket, int nLength, int &nBytesUsed);
//	ProcessResult Process_FI_PARTY_GET_PARTY_INFO(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FI_PARTY_AUTO_CREATE(const char* pPacket, int nLength, int &nBytesUsed);		// 2009-09-09 ~ 2010-01-26 by dhjin, 인피니티 - 자동 편대
	ProcessResult Process_FI_PARTY_UPDATE_ITEM_TRANSFORMER_OK(const char* pPacket, int nLength, int &nBytesUsed);	// 2010-03-18 by cmkwon, 몬스터변신 구현 - 

	// Party 관련
	BOOL ProcessPartyLeaveNoLock( CIMParty *i_pIMParty , const bool a_bMoveToArena = false );			// 파티에서 탈퇴
	BOOL ProcessPartyLeaveArenaEndNoLock(CIMParty *i_pIMParty);		// 2008-02-28 by dhjin, 아레나 통합 - 아레나 서버에서 경기 종료후 파티 관련 처리
	BOOL ProcessPartyGameEndNoLock(CIMParty *i_pIMParty);		// 파타중 게임 종료
	BOOL ProcessPartyFromMainServerToArenaServerNoLock(CIMParty *i_pIMParty);		// 2008-02-28 by dhjin, 아레나 통합 - 본 서버에서 아레나 서버로 이동 시 파티 처리 사항
	BOOL SendMsgPartyTransferMasterInfoNoLock(CIMParty *i_pIMParty, UINT OldMaster, UINT NewMaster);
	void ProcessPartyDismember(PartyID_t i_partyID);

	// Client - Guild
	ProcessResult Process_IC_GUILD_CREATE(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_IC_GUILD_GET_GUILD_INFO(const char* pPacket, int nLength, int &nBytesUsed);
//	ProcessResult Process_IC_GUILD_GET_MEMBER(const char* pPacket, int nLength, int &nBytesUsed);	// check: T_IC_GUILD_GET_GUILD_INFO로 통합, 필요하면 살림, 20040520, kelovon
	ProcessResult Process_IC_GUILD_REQUEST_INVITE(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_IC_GUILD_ACCEPT_INVITE(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_IC_GUILD_REJECT_INVITE(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_IC_GUILD_LEAVE(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_IC_GUILD_BAN_MEMBER(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_IC_GUILD_DISMEMBER(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_IC_GUILD_CANCEL_DISMEMBER(const char *pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_IC_GUILD_GET_DISMEMBER_DATE(const char *pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_IC_GUILD_CHANGE_GUILD_NAME(const char *pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_IC_GUILD_GET_GUILD_MARK(const char *pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_IC_GUILD_SET_GUILD_MARK(const char *pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_IC_GUILD_SET_RANK(const char *pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_IC_GUILD_GET_OTHER_GUILD_INFO(const char *pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_IC_GUILD_SURRENDER_GUILD_WAR(const char *pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_IC_GUILD_NEW_COMMANDER(const char *pPacket, int nLength, int &nBytesUsed);			// 2008-05-20 by dhjin, EP3 - 여단 수정 사항 - 여단장 위임
	ProcessResult Process_IC_GUILD_NOTICE_WRITE(const char *pPacket, int nLength, int &nBytesUsed);			// 2008-05-20 by dhjin, EP3 - 여단 수정 사항 - 여단 공지
	ProcessResult Process_IC_GUILD_GET_APPLICANT(const char *pPacket, int nLength, int &nBytesUsed);		// 2008-05-27 by dhjin, EP3 - 여단 수정 사항 - 지원자 관리
	ProcessResult Process_IC_GUILD_GET_INTRODUCTION(const char *pPacket, int nLength, int &nBytesUsed);		// 2008-05-27 by dhjin, EP3 - 여단 수정 사항 - 여단 소개
	ProcessResult Process_IC_GUILD_GET_SELF_INTRODUCTION(const char *pPacket, int nLength, int &nBytesUsed);		// 2008-05-27 by dhjin, EP3 - 여단 수정 사항 - 여단 지원자 소개서 
	ProcessResult Process_IC_GUILD_SEARCH_INTRODUCTION(const char *pPacket, int nLength, int &nBytesUsed);		// 2008-05-27 by dhjin, EP3 - 여단 수정 사항 - 여단 소개 검색
	ProcessResult Process_IC_GUILD_UPDATE_INTRODUCTION(const char *pPacket, int nLength, int &nBytesUsed);						// 2008-05-27 by dhjin, EP3 - 여단 수정 사항 - 여단 소개 작성 
	ProcessResult Process_IC_GUILD_DELETE_INTRODUCTION(const char *pPacket, int nLength, int &nBytesUsed);		// 2008-05-27 by dhjin, EP3 - 여단 수정 사항 - 여단 소개 지우기
	ProcessResult Process_IC_GUILD_UPDATE_SELFINTRODUCTION(const char *pPacket, int nLength, int &nBytesUsed);		// 2008-05-27 by dhjin, EP3 - 여단 수정 사항 - 자기 소개 작성 
	ProcessResult Process_IC_GUILD_DELETE_SELFINTRODUCTION(const char *pPacket, int nLength, int &nBytesUsed);		// 2008-05-27 by dhjin, EP3 - 여단 수정 사항 - 자기 소개 지우기  
	ProcessResult Process_IC_GUILD_APPLICANT_INVITE(const char *pPacket, int nLength, int &nBytesUsed);				// 2008-06-12 by dhjin, EP3 - 여단 수정 사항 - 지원자 관리에서 여단원 초대
	ProcessResult Process_IC_GUILD_APPLICANT_REJECT_INVITE(const char *pPacket, int nLength, int &nBytesUsed);		// 2008-06-12 by dhjin, EP3 - 여단 수정 사항 - 지원자 관리에서 여단원 초대 거부 
	
	// 2006-09-29 by dhjin, CheckDelete추가 1->이면 PROCEDURE atum_AllDelete_Guild_Store 실행
	BOOL SendGuildInfoToFieldServer(UID32_t i_nGuildUniqueNumber, const char *i_szGuildName); 

	// Client - Admin
	ProcessResult Process_IC_ADMIN_CALL_CHARACTER(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_IC_ADMIN_CALL_GUILD(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_IC_ADMIN_GET_SERVER_STAT(const char* pPacket, int nLength, int &nBytesUsed);

	// Client - Countdown 관련
	ProcessResult Process_IC_COUNTDOWN_DONE(const char* pPacket, int nLength, int &nBytesUsed);
	
	//ProcessResult Process_IC_VOIP_SET(const char* pPacket, int nLength, int &nBytesUsed);					// 2008-06-17 by dhjin, EP3 VOIP - 설정

	// T0_IC_CHATROOM
	ProcessResult Process_IC_CHATROOM_CREATE(const char* pPacket, int nLength, int &nBytesUsed);			//2008-06-16 by dhjin, EP3 채팅방 - 채팅방 생성
	ProcessResult Process_IC_CHATROOM_LIST_INFO(const char* pPacket, int nLength, int &nBytesUsed);			//2008-06-16 by dhjin, EP3 채팅방 - 채팅방 목록 가져오기		
	ProcessResult Process_IC_CHATROOM_REQUEST_INVITE(const char* pPacket, int nLength, int &nBytesUsed);	//2008-06-16 by dhjin, EP3 채팅방 - 채팅방 초대 요청 	
	ProcessResult Process_IC_CHATROOM_JOIN(const char* pPacket, int nLength, int &nBytesUsed);				//2008-06-16 by dhjin, EP3 채팅방 - 채팅방 참여	
	ProcessResult Process_IC_CHATROOM_ACCEPT_INVITE(const char* pPacket, int nLength, int &nBytesUsed);		//2008-06-16 by dhjin, EP3 채팅방 - 채팅방 초대 수락	
	ProcessResult Process_IC_CHATROOM_REJECT_INVITE(const char* pPacket, int nLength, int &nBytesUsed);		//2008-06-16 by dhjin, EP3 채팅방 - 채팅방 초대 거절	
	ProcessResult Process_IC_CHATROOM_LEAVE(const char* pPacket, int nLength, int &nBytesUsed);				//2008-06-16 by dhjin, EP3 채팅방 - 채팅방 나가기	
	ProcessResult Process_IC_CHATROOM_BAN(const char* pPacket, int nLength, int &nBytesUsed);				//2008-06-16 by dhjin, EP3 채팅방 - 채팅방 추방	
	ProcessResult Process_IC_CHATROOM_CHANGE_NAME(const char* pPacket, int nLength, int &nBytesUsed);		//2008-06-16 by dhjin, EP3 채팅방 - 채팅방 이름 변경	
	ProcessResult Process_IC_CHATROOM_CHANGE_MASTER(const char* pPacket, int nLength, int &nBytesUsed);		//2008-06-16 by dhjin, EP3 채팅방 - 채팅방 방장 변경	
	ProcessResult Process_IC_CHATROOM_CHANGE_LOCK_PW(const char* pPacket, int nLength, int &nBytesUsed);	//2008-06-16 by dhjin, EP3 채팅방 - 채팅방 비밀번호 변경	
	ProcessResult Process_IC_CHATROOM_CHANGE_MAX_MEMBER(const char* pPacket, int nLength, int &nBytesUsed);	//2008-06-16 by dhjin, EP3 채팅방 - 채팅방 인원수 변경	
	ProcessResult Process_IC_CHATROOM_OTHER_MEMBER_INFO(const char* pPacket, int nLength, int &nBytesUsed);		//2008-06-25 by dhjin, EP3 채팅방 - 채팅방 다른 맴버 정보 전송

	// Field Server
	ProcessResult Process_FI_CONNECT(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FI_CONNECT_NOTIFY_FIELDSERVER_IP(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FI_CONNECT_NOTIFY_GAMEEND(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FI_CONNECT_NOTIFY_DEAD(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FI_ADMIN_GET_CHARACTER_INFO_OK(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FI_CONNECT_NOTIFY_GAMESTART(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FI_CONNECT_NOTIFY_DEAD_GAMESTART(const char* pPacket, int nLength, int &nBytesUsed);

	ProcessResult Process_FI_CHARACTER_DELETE_CHARACTER(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FI_CHARACTER_CHANGE_LEVEL(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FI_CHARACTER_UPDATE_MAP_CHANNEL(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FI_CHARACTER_CHANGE_INFLUENCE_TYPE(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FI_UPDATE_SUBLEADER(const char* pPacket, int nLength, int &nBytesUsed);					// 2007-02-14 by dhjin, 명령어로 부지도자 설정.

	ProcessResult Process_FI_GUILD_NOTIFY_START_WAR(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FI_GUILD_DELETE_GUILD(const char* pPacket, int nLength, int &nBytesUsed);
// 2004-12-09 by cmkwon, 여단전의 종료는 IMServer에서만 발생한다. 그러므로 알림은 IMServer -> Field로 발생한다
//	ProcessResult Process_FI_GUILD_NOTIFY_END_WAR(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FI_GUILD_ADD_GUILD_FAME(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FI_GUILD_OUTPOST(const char* pPacket, int nLength, int &nBytesUsed);			// 2008-05-20 by dhjin, EP3 - 여단 수정 사항 - 전진기지 관련

	// Field Server - 다른 필드 서버로의 워프를 처리함
	ProcessResult Process_FI_EVENT_NOTIFY_WARP(const char* pPacket, int nLength, int &nBytesUsed);
// 2004-10-29 by cmkwon, FieldServer는 하나로 픽스한다
//	ProcessResult Process_FI_EVENT_GET_WARP_INFO(const char* pPacket, int nLength, int &nBytesUsed);	// 다른 필드 서버로의 워프인 경우 정보 전달 위해
	ProcessResult Process_FI_EVENT_CHAT_BLOCK(const char* pPacket, int nLength, int &nBytesUsed);		// 2008-12-30 by cmkwon, 지도자 채팅 제한 카드 구현 - 

	// T0_FI_CASH
	ProcessResult Process_FI_CASH_USING_GUILD(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FI_CASH_USING_CHANGE_CHARACTERNAME(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FI_CASH_PREMIUM_CARD_INFO(const char* pPacket, int nLength, int &nBytesUsed);

	// Monitor Server
	ProcessResult Process_IM_PRINT_DEBUG_MSG(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_IM_GET_TOTAL_USER_COUNTS(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FI_GET_FIELD_USER_COUNTS_ACK(const char* pPacket, int nLength, int &nBytesUsed);

	ProcessResult Process_FI_INFO_DECLARATION_MSWAR_SET_OK(const char* pPacket, int nLength, int &nBytesUsed);	// 2009-01-12 by dhjin, 선전 포고 - 선전포고 시간 및 포기 설정 F->I

	ProcessResult Process_FI_CHAT_CNC(const char* pPacket, int nLength, int& nBytesUsed);	// 2015-07-20 Future, CnC costs money

	// Send Error
	void SendErrorMessage(MessageType_t msgType, Err_t err, int errParam1 = 0, int errParam2 = 0, char* errMsg = NULL, BOOL bCloseConnection = FALSE);

	// Admin용 함수, TRUE이면 Field Server로 넘김, FALSE이면 IM에서 끝냄
	BOOL CheckCommandExecutability(USHORT i_nAllowedRace, BOOL i_bAllowAdminTool = FALSE);
	BOOL HandleAdminCommands(char *cmd);

	void SendCNC(MSG_IC_CHAT_CNC* msgCnC);

#ifdef _DEBUG
	inline BOOL SendAddData(BYTE* pData, int nSize)  // check: EVENT 관련 MSG 못 보내는 등의 문제 있음! 좋은 방법 찾아야 함.
	{
		MessageType_t msgType = *(MessageType_t*)pData;

		if (m_PeerSocketType == ST_CLIENT_TYPE
			&& m_bSendMessgeTypeDBG && T0_IC_STRING != HIBYTE(msgType)
			&& !IS_ALIVE_MSG(msgType) && T0_IC_ADMIN != HIBYTE(msgType)
		)
		{
			SendString128(STRING_128_USER_NOTICE, "[I->C]%s", GetProtocolTypeString(msgType));
		}

		return CIOCPSocket::SendAddData(pData, nSize);
	}
#endif
};

typedef vector<CIMIOCPSocket*>		vectCIMIOCPSocketPtr;			// 2007-01-31 by cmkwon

const CHARACTER* CIMIOCPSocket::GetCharacter(void)
{
	return &m_character;
}

#endif // !defined(AFX_IMIOCPSOCKET_H__9C62870A_F7ED_49A2_9583_956845931320__INCLUDED_)
