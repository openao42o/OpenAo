#ifndef _ATUM_FIELD_PARTY_H_
#define _ATUM_FIELD_PARTY_H_

#include "mt_stl.h"
#include "AtumParam.h"
#include "AtumProtocol.h"
#include "FieldIOCPSocket.h"

class CFieldIOCP;
class CFieldIOCPSocket;
class CFieldMapChannel;
class CFieldPartyBattle;

struct FieldPartyMember
{
	CFieldIOCPSocket	*PM_CharacterSocket;

	FieldPartyMember() : PM_CharacterSocket(NULL) {}
	FieldPartyMember(CFieldIOCPSocket *pSock) : PM_CharacterSocket(pSock) {}
};

class CFieldParty;
class CFieldPartyBattle
{
public:
	CFieldPartyBattle();

	void Reset();

	void InitFieldPartyBattle(CFieldParty *pMyParty, CFieldParty *pPeerParty, CFieldMapChannel *pMapChannel);
	void CleanFieldPartyBattle();

	BOOL IsUsing();

public:
	CFieldParty			*m_pPeerParty;
	mtsetUID32			m_setLiveMember;		// 살아있는 파티원의 CharacterUID
	mtsetUID32			m_setLossWaiters;		// 파티 전 패배 시, 처리를 기다리는 자의 리스트(이미 처리된 파티원은 제외함), CharacterUID
	CFieldMapChannel	*m_pBattleMapChannel;	// 파티전이 일어나는 맵
	INT					m_nAverageLevel;		// 파티전 시작시의 평균 level
	INT					m_nPeerAverageLevel;	// 파티전 시작시의 상대편대 평균 level
};

typedef mt_vector<FieldPartyMember>		mtlistFieldPartyMember;

class CFieldParty
{
public:
	CFieldParty(CFieldIOCP* pFieldIOCP, PartyID_t pid, UID32_t masterUniqueNumber);
	~CFieldParty();

	BOOL InsertMember(CFieldIOCPSocket *pSocket);
	BOOL DeleteByCharacterUniqueNumber(UID32_t characterUniqueNumber, FieldPartyMember& ret);
	UID32_t GetMasterCharacterUniqueNumber();
	BOOL empty();
	BOOL GetMemberByCharacterUniqueNumber(UID32_t characterUniqueNumber, FieldPartyMember& ret);
	BYTE GetLowestMemberLevel(UID32_t CharacterUIDToExclude = INVALID_UNIQUE_NUMBER);
	BYTE GetMaxMemberLevel(UID32_t CharacterUIDToExclude = INVALID_UNIQUE_NUMBER);
	BOOL UpdateMemberInfoAllInMap(CFieldIOCPSocket *pSocket, CFieldMapChannel *pMapChannel);		// 자신의 정보를 다른 파티원에게 전송하고 다른 파티원의 정보를 받아옴. 워프시 call함.
	BYTE GetMaxMemberLevelInSameMap(CFieldMapChannel *i_pFMapChann, BOOL i_bLiveCheck=FALSE);		// 2007-06-27 by cmkwon, 파티원 경험치 분배 방식 시스템 추가 - 함수 추가
	BOOL SetReturnUserBuffSkill(CFieldIOCPSocket *pSocket);						// 2013-02-28 by bckim, 복귀유져 버프추가
	// CFieldIOCPSocket[SIZE_MAX_PARTY_MEMBER]를 param으로 준다.
	// 주의: return받은 pMemberSocketArray를 다 처리할 때 가지 lock을 잡아두어 함
	// 2007-06-26 by cmkwon, 파티 경험치 관련 수정, 인자 수정(기존 - CFieldIOCPSocket **pMemberSocketArray, CFieldMapChannel *pMapChannel, D3DXVECTOR3 *pPos = NULL, float fRadius = 0, BYTE *o_byMaxLevel=NULL)
	int GetMembersInSameMap(CFieldIOCPSocket **o_pArrMemFISock, CFieldMapChannel *i_pFMapChann, BOOL i_bLiveCheck=FALSE, BYTE *o_byMaxLevel=NULL);
	int QuestWarpGetMembers(CFieldIOCPSocket **o_pArrMemFISock, CFieldMapChannel *i_pFMapChann);		// 2008-12-16 by dhjin, 미션마스터 - 퀘스트 시작 시 워프 메세지 받을 유저 뽑기.
	int GetMembersInSameMapToVector(vector<ClientIndex_t> *i_pVectorClientIndex, CFieldMapChannel *pMapChannel, D3DXVECTOR3 *pPos = NULL, float fRadius = 0, ClientIndex_t nClientIndexToExclude = INVALID_CLIENT_INDEX);
// 2007-02-12 by cmkwon, 아래함수로 수정함	BOOL CheckAllMemberEnableWarp(CFieldMapChannel *i_pFMChan, int i_nMapInfluenceTy, BOOL i_bConflictAraeMap, int i_nRequetCompletionQuestIndex);
	static BOOL CheckAllMemberEnableWarp(vectCFieldIOCPSocket *i_pvectFISockList, CFieldMapChannel *i_pFMChan, int i_nMapInfluenceTy, BOOL i_bConflictAraeMap, int i_nRequetCompletionQuestIndex);
	int GetAllMembers(CFieldIOCPSocket *pMemberSocketArray[SIZE_MAX_PARTY_MEMBER]);	// 2007-06-26 by cmkwon, 파티 경험치 관련 수정, 인자 수정
	int GetAllMembers(vectCFieldIOCPSocket *i_pvectFISockList);

	// 같은 맵에 있는 파티원에게만 보낸다.
	BOOL SendMsgToMembersMap(BYTE* buffer, int size, CFieldMapChannel *pMapChannel, UID32_t CharacterUIDToExclude = INVALID_UNIQUE_NUMBER, MSG_FC_COUNTDOWN_START *i_pCountdownStart=NULL);
	
	template<template<MessageType_t> typename msg_t, MessageType_t t>
	void SendMsgToMembersMap(msg_t<t>& msg, CFieldMapChannel *pMapChannel, UID32_t CharacterUIDToExclude = INVALID_UNIQUE_NUMBER, MSG_FC_COUNTDOWN_START *i_pCountdownStart=NULL)
	{
		this->SendMsgToMembersMap(reinterpret_cast<BYTE*>(&msg), msg.size(), pMapChannel, CharacterUIDToExclude, i_pCountdownStart);
	}
	
	////////////////////////////////////////////////////////////////////////////////
	// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 
	void SendMsgToMembers(BYTE* buffer, int size, UID32_t CharacterUIDToExclude = INVALID_UNIQUE_NUMBER);
	void SendMsgToMembersExcludeMap(BYTE* buffer, int size, CFieldMapChannel *pMapChannel, UID32_t CharacterUIDToExclude = INVALID_UNIQUE_NUMBER);

	template<template<MessageType_t> typename msg_t, MessageType_t t>
	void SendMsgToMembers(const msg_t<t>& msg, UID32_t CharacterUIDToExclude = INVALID_UNIQUE_NUMBER)
	{
		this->SendMsgToMembers(reinterpret_cast<const BYTE*>(&msg), msg.size(), CharacterUIDToExclude);
	}

	void Print(CFieldIOCPSocket *pSocket);
	
	BYTE GetExpDistributeType(void);				// 2007-06-27 by cmkwon, 파티원 경험치 분배 방식 시스템 추가 - 함수 추가
	void SetExpDistributeType(BYTE i_byExpDistTy);	// 2007-06-27 by cmkwon, 파티원 경험치 분배 방식 시스템 추가 - 함수 추가
	BYTE GetItemDistributeType(void);					// 2008-06-03 by dhjin, EP3 편대 수정 - 아이템 분배 방식 
	void SetItemDistributeType(BYTE i_byItemDistTy);	// 2008-06-03 by dhjin, EP3 편대 수정 - 아이템 분배 방식 

	CFieldIOCPSocket * GetItemByRandomDistributeType(CFieldMapChannel *i_pFMapChann);		// 2008-06-03 by dhjin, EP3 편대 수정 - 아이템 랜덤 습득 당첨자
	CFieldIOCPSocket * GetItemByOrderDistributeType(CFieldMapChannel *i_pFMapChann, BYTE * o_byPivotPartyMember);		// 2008-06-03 by dhjin, EP3 편대 수정 - 아이템 순차 습득 당첨자
	void IncreasePivotPartyMember(BYTE i_byPivotPartyMember);					// 2008-06-10 by dhjin, EP3 편대 수정 - 아이템 순차 습득 순서 기억 변수 증가

public:
	PartyID_t					m_PartyID;
	UID32_t						m_MasterUniqueNumber;	// 파티장의 CharacterUniqueNumber
	MEX_FIELD_PARTY_INFO		m_FieldPartyInfo;		// IMServer에서 FieldServer로 넘겨주어야 하는 정보
	mtlistFieldPartyMember		m_ListPartyMember;		// 같은 필드 서버에서 관리된는 파티원의 리스트
	CFieldIOCP					*m_pFieldIOCP7;
	PartyID_t					m_PeerBattleRequesterPartyID;	// 파티전 요청자의 파티 아이디, 파티전 요청시부터 파티전 시작 전까지 할당된다.
	PartyID_t					m_PeerBattlePartyID;	// 파티전 시 상대방 파티의 아이디, 파티전 시작 시 할당된다.
	CFieldPartyBattle			m_PartyBattle;			// 파티전 시 사용
	MAP_CHANNEL_INDEX			m_PartyBattleMapChannelIndex;
	DWORD						m_dwPartyDeletedTick;
	BYTE						m_byExpDistributeType;	// 2007-06-27 by cmkwon, 파티원 경험치 분배 방식 시스템 추가 - 경험치 분배 타입(EXP_DISTRIBUTE_TYPE_XXX)
	BYTE						m_byItemDistributeType;	// 2008-06-03 by dhjin, EP3 편대 수정 - 아이템 분배 방식
	BYTE						m_byPivotPartyMember;	// 2008-06-04 by dhjin, EP3 편대 수정 - 아이템 순차 습득 순서 기억 변수
};

#endif // _ATUM_FIELD_PARTY_H_
