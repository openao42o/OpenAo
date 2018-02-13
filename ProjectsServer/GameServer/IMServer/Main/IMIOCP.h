// IMIOCP.h: interface for the CIMIOCP class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IMIOCP_H__89992FF9_ADF7_4FFF_A639_F9BC79F85619__INCLUDED_)
#define AFX_IMIOCP_H__89992FF9_ADF7_4FFF_A639_F9BC79F85619__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "IOCP.h"
#include "ServerGroup.h"
#include "Guild.h"
#include "IMTickManager.h"
#include "ChatBlockManager.h"
//#include "MersenneTwister.h"					// 2008-06-04 by dhjin, EP3 랜덤 함수 추가
#include "ChatRoomManager.h"					// 2008-06-09 by dhjin, EP3 채팅방 - 

///////////////////////////////////////////////////////////////////////////////
// Party(편대 정보)
struct BelongedPartyInfo	// 소속된 편대의 정보
{
	UID32_t		CharacterUniqueNumber;		// 해당 캐릭의 파티 정보
	UID32_t		PartyMasterUniqueNumber;	// 파티(장) 고유 번호
	TimeUnit_t	PartyCreationTime;			// 파티 생성 시간
};

class CAtumIMDBManager;
class CIMIOCPSocket;
class CPreIMWinSocket;
class CIMParty;
class CChatRoomManager;						// 2008-06-09 by dhjin, EP3 채팅방 - 
class CANoticeManager;				// 2009-01-14 by cmkwon, 운영자 자동 공지 시스템 구현 - 
//class CSecurityManager;						// 2011-06-22 by hskim, 사설 서버 방지

class CIMIOCP : public CIOCP
{
public:
	CIMIOCP(int nPort, char *szLocalIP = "127.0.0.1");
	virtual ~CIMIOCP();

public:
	CAtumIMDBManager				*m_pAtumIMDBManager;			// DB manager
	CIMTickManager					*m_pIMTickManager;				// Tick manager
	//CSecurityManager				*m_pSecurityManager;			// 2011-06-22 by hskim, 사설 서버 방지
	mt_map<string, CIMIOCPSocket*>	m_mAccountName;					// AccountName이 키로 이루어진 Client Map
	mt_map<UID32_t, CIMIOCPSocket*>	m_MapAccountUniqueNumber;
	mt_map<string, CIMIOCPSocket*>	m_mCharacterName;				// CharacterName이 키로 이루어진 Client Map
	mt_map<UID32_t, CIMIOCPSocket*>	m_MapChactarterUniqueNumber;
	CPreIMWinSocket					*m_pPreWinSocket;
public:
	CIMIOCPSocket* GetIMIOCPSocket(int idx);
	CIMIOCPSocket* GetIMIOCPSocketByCharacterUID(UID32_t i_characterUID);
	CIMIOCPSocket* GetIMIOCPSocketByCharacterName(char *i_szCharacterName);
	CIMIOCPSocket* GetIMIOCPSocketByAccountUID(UID32_t i_accountUID);
	CHARACTER* GetCharacterByArrayIndex(int idx);
	BOOL IsConnectedUser(char* AccountName, CIMIOCPSocket **ppSock);
	BOOL SendMessageToFieldServer(BYTE *buffer, int size);

	///////////////////////////////////////////////////////////////////////////
	// 관리자용 소켓 리스트 관련 함수
	void SendMessageToAllAdmin(BYTE *buffer, int size);
	void SendString128ToAllAdmin(BYTE i_byString128Ty, char *i_szString);		// 2007-11-19 by cmkwon, 진정시스템 업데이트 - 
	BOOL AddToAdminCharacter(UID32_t AdminCharacterUniqueNumber);
	BOOL DeleteFromAdminCharacter(UID32_t AdminCharacterUniqueNumber);

	///////////////////////////////////////////////////////////////////////////
	// 공지 관련
	void SendMessageToAllClients(BYTE *buffer, int size);
//	void SendChatMsgToAllClients(BitFlag8_t i_chatType, BYTE *i_pChat, int i_nSize, BYTE i_byInfluenceMask=INFLUENCE_TYPE_ALL_MASK);
	void SendChatMsgToAllClients(BitFlag16_t i_chatType, BYTE *i_pChat, int i_nSize, BYTE i_byInfluenceMask=INFLUENCE_TYPE_ALL_MASK);	// 2008-05-15 by dhjin, EP3 - 채팅 시스템 변경
	int SendMsg2MultiUser(vectCIMIOCPSocketPtr *i_pvectCIISockPtrList, BYTE *i_pbyBuf, int i_nLen);
	int SendMsg2MultiUser(vectClientIndex_t *i_pvectClientIdxList, BYTE *i_pbyBuf, int i_nLen);
//	int SendChatMsg2MultiUser(vectCIMIOCPSocketPtr *i_pvectCIISockPtrList, BYTE *i_pbyBuf, int i_nLen, BitFlag8_t i_chatType);
//	int SendChatMsg2MultiUser(vectClientIndex_t *i_pvectClientIdxList, BYTE *i_pbyBuf, int i_nLen, BitFlag8_t i_chatType);
	int SendChatMsg2MultiUser(vectCIMIOCPSocketPtr *i_pvectCIISockPtrList, BYTE *i_pbyBuf, int i_nLen, BitFlag16_t i_chatType);		// 2008-05-15 by dhjin, EP3 - 채팅 시스템 변경
	int SendChatMsg2MultiUser(vectClientIndex_t *i_pvectClientIdxList, BYTE *i_pbyBuf, int i_nLen, BitFlag16_t i_chatType);			// 2008-05-15 by dhjin, EP3 - 채팅 시스템 변경

	// 2009-01-14 by cmkwon, 운영자 자동 공지 시스템 구현 - 
	int SendString128ToAllClients(BYTE i_byStr128Ty, char *i_szString, BOOL i_bWriteLog=FALSE, BYTE i_byInfluenceMask=INFLUENCE_TYPE_ALL_MASK);
	
	///////////////////////////////////////////////////////////////////////////
	// 길드
	BOOL OnGuildLoading(CGuild *i_pGuild, CIMIOCPSocket *i_pIISock);
	CGuild* GetGuildByName(const char *i_szGuildName);
	void DismemberAllPendingGuild(ATUM_DATE_TIME *pCurrentDateTime);
// 2004-10-20 by cmkwon, 사용하지 않던 함수임
//	void ReadyGuildWar(UID32_t DefenderGuildUID, UID32_t ChallengerGuildUID, INT ReadyTimeInSeconds);
// 2004-10-20 by cmkwon, 카운트 방식 수정
//	void StartGuildWar(UID32_t DefenderGuildUID, UID32_t ChallengerGuildUID);
	void SendMsgToGuildMembersByGuildUID(UID32_t i_uidGuild, BYTE *buffer, int size, UID32_t CharacterUniqueNumberToExclude = INVALID_UNIQUE_NUMBER);
	void GuildWarCheck(UID32_t i_uidGuildUID, MAP_CHANNEL_INDEX i_mapChannIdxCurrent, UID32_t i_uidAttackerGuildUID=INVALID_GUILD_UID);
	void GuildWarEnd(USHORT i_usWarEndType, UID32_t i_uidGuildUID, DWORD i_dwTimerUniqueNumber=0);
	void GuildSetGuildFame(UID32_t i_guildUID, int i_nTotalFame, int i_nMonthlyFame);
	int WarpGulid(char *i_szGuildName, MAP_CHANNEL_INDEX i_wapMapChann, D3DXVECTOR3 i_vWarpPosition);
	BOOL IsGuildCommanderW(UID32_t i_guildUID, UID32_t i_charUID);	// 2007-10-06 by cmkwon, 여단장 체크


	void SetFieldServerSocket(CIMIOCPSocket *i_pFieldServerSocket){	m_pFieldServerSocket = i_pFieldServerSocket;};

	///////////////////////////////////////////////////////////////////////////
	// 편대관련
	void InsertParty(CIMParty *i_pParty);
	CIMParty *FindParty(PartyID_t i_partyID);
	void DeleteParty(PartyID_t i_partyID);
	void AdjustDeletedParties(void);				// 지워진 파티를 정리한다

	//////////////////////////////////////////////////////////////////////////
	// 2006-12-08 by dhjin, 리더 셋팅
	void SetInflAllLeader(CHARACTER *o_m_character);

	// 2006-05-08 by cmkwon, callGM 시스템
	int CallGMInsertRequest(UID32_t i_characterUID, BYTE i_byInflTy, char *i_szCharName, char *i_szCallDes);	// 2007-11-19 by cmkwon, 진정시스템 업데이트 - 인자추가(, BYTE i_byInflTy)
	void CallGMOnRequest(int i_nCallGMRequestCount);
	BOOL CallGMMake_MSG_IC_ADMIN_CALLGM_VIEW_OK(MSG_IC_ADMIN_CALLGM_VIEW_OK *o_pCallGMViewOK, int i_nReqCount);
	BOOL CallGMMake_MSG_IC_ADMIN_CALLGM_BRING_OK(MSG_IC_ADMIN_CALLGM_BRING_OK *o_pCallGMBringOK, int i_nReqCount);

	///////////////////////////////////////////////////////////////////////////////
	// 2007-10-06 by cmkwon, 부지도자 2명의 호칭을 다르게 설정 -
	BOOL CheckInfluenceLeader(BYTE i_byInflTy, UID32_t i_uidCharacterUID);
	BOOL CheckInfluenceSubLeader(BYTE i_byInflTy, UID32_t i_uidCharacterUID);
	BOOL CheckInfluenceSubLeader1(BYTE i_byInflTy, UID32_t i_uidCharacterUID);
	BOOL CheckInfluenceSubLeader2(BYTE i_byInflTy, UID32_t i_uidCharacterUID);

	///////////////////////////////////////////////////////////////////////////////
	// 2007-11-19 by cmkwon, 진정시스템 업데이트 -
	bool IsOnCallGMSystem() const;
	void SetCallGMSystemTime(ATUM_DATE_TIME *i_patStartTime, ATUM_DATE_TIME *i_patEndTime);

	void OnDoMinutelyWorkIMIOCP(ATUM_DATE_TIME *pDateTime);		// 2007-11-19 by cmkwon, 진정시스템 업데이트 -

	// 2009-01-14 by cmkwon, 운영자 자동 공지 시스템 구현 - 
	void OnDoSecondlyWorkIMIOCP(ATUM_DATE_TIME *pDateTime);

	//////////////////////////////////////////////////////////////////////////
	// 2008-05-21 by dhjin, EP3 - 여단 수정 사항
	void UpdateGuildCommander(UID32_t i_GuildUID, UID32_t i_Commander, UID32_t i_NewCommander);	// 2008-05-21 by dhjin, EP3 - 여단 수정 사항 - 여단장 위임
	BOOL CheckGuildLoaded(CGuild *i_pGuild, CIMIOCPSocket *i_pIISock);							// 2008-05-30 by dhjin, EP3 - 여단 수정 사항 - 여단이 로딩되어 있는지 체크
	void SetGuildMemberShipW(UID32_t i_GuildUID, BOOL i_bMemberShip, ATUM_DATE_TIME * i_pMemberShipDate = NULL);		// 2008-06-20 by dhjin, EP3 - 여단 수정 사항 - 여단장 맴버쉽 정보 저장

	static int GetRandInt32(UINT i_uiMin, UINT i_uiMax) { return RANDI(i_uiMin, i_uiMax); }				// 2008-06-04 by dhjin, EP3 랜덤 함수 추가

public:
	virtual BOOL IOCPInit(void);
	virtual void IOCPClean(void);
	virtual SThreadInfo *CheckIOCPThread(DWORD i_dwThreadIdToExclude);

public:
	///////////////////////////////////////////////////////////////////////////
	// Party
	mt_map<PartyID_t, CIMParty*>		m_MapParties;
	mt_vector<CIMParty *>				m_mtvectorDeletedParties;
	
	CUID32Generator						m_UID32GenPartyID;		// PartyID용 UID32 Generator

	///////////////////////////////////////////////////////////////////////////
	// Guild
	mt_map<UID32_t, CGuild*>				m_MapGuild;				// main guild data

	///////////////////////////////////////////////////////////////////////////
	// 서버 다운 확인용 숫자
	USHORT			m_nServerDownVerifyNumber;

	///////////////////////////////////////////////////////////////////////////
	// 관리자용 소켓 리스트
	mtsetUID32		m_setAdminCharacterUniqueNumbers;

	UINT								m_uiIMAccumulatedTotalUserCounts;
	UINT								m_uiIMMaxTotalUserCounts;

	///////////////////////////////////////////////////////////////////////////
	// FieldServer가 연결된 IMIOCPSocket
	CIMIOCPSocket						*m_pFieldServerSocket;
	FieldServerInfo						m_FieldServerInfo;

	///////////////////////////////////////////////////////////////////////////////
	// 채금 관리
	CChatBlockManager					m_chatBlockManager;

	CChatBlockManager					m_chatBlockManagerByLeader;		// 2008-12-30 by cmkwon, 지도자 채팅 제한 카드 구현 - 


	///////////////////////////////////////////////////////////////////////////////
	// 2005-11-17 by cmkwon, 명령어 리스트
	vectString							m_vectstringCommandList;				// 관리자 명령어 리스트
	vectString							m_vectstringGameMasterCommandList;		// 2007-11-08 by cmkwon, GM 명령어 정리 - 운영자 명령어 리스트
	vectString							m_vectstringCommandListforUser;			// 일반유저 명령어 리스트

	///////////////////////////////////////////////////////////////////////////////
	// 2006-04-20 by cmkwon, 세력지도자 CharacterUID
	UID32_t								m_VCNLeaderCharacterUID;
	UID32_t								m_ANILeaderCharacterUID;
	UID32_t								m_VCNSub1LeaderCharacterUID;
	UID32_t								m_ANISub1LeaderCharacterUID;
	UID32_t								m_VCNSub2LeaderCharacterUID;
	UID32_t								m_ANISub2LeaderCharacterUID;

	// 2006-05-08 by cmkwon, CallGM 시스템
	mtvectSCALLGM					m_mtvectSCallGMList;			// 2006-05-08 by cmkwon

	///////////////////////////////////////////////////////////////////////////////
	// 2007-11-19 by cmkwon, 진정시스템 업데이트 - 시작, 종료 시간 변수 추가
	BOOL							m_bOnCallGMSystem;		
	ATUM_DATE_TIME					m_atCallGMStartTime;
	ATUM_DATE_TIME					m_atCallGMEndTime;


	BOOL							m_bArenaServerCheck;				// 2008-02-28 by dhjin, 아레나 통합 - 아레나 서버인지 체크 비트 0:False, 1:TRUE

	//MTRand							m_mtRandManager;	// 2008-06-04 by dhjin, EP3 랜덤 함수 추가
	mt_lock							m_lockRandManager;	// 2008-06-04 by dhjin, EP3 랜덤 함수 추가

	//////////////////////////////////////////////////////////////////////////
	// 2008-06-09 by dhjin, EP3 채팅방 - 
	CChatRoomManager				m_ChatRoomManager;	// 2008-06-09 by dhjin, EP3 채팅방 - 채팅방 관리자

	///////////////////////////////////////////////////////////////////////////////
	// 2009-01-14 by cmkwon, 운영자 자동 공지 시스템 구현 - 
	CANoticeManager *				m_pANoticeManager;	// 2009-01-14 by cmkwon, 운영자 자동 공지 시스템 구현 - 

	BOOL							m_bEnableCNC;
};

#endif // !defined(AFX_IMIOCP_H__89992FF9_ADF7_4FFF_A639_F9BC79F85619__INCLUDED_)
