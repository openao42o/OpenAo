// ClientParty.h: interface for the CClientParty class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CLIENTPARTY_H__DB818FDB_228D_4091_B3DF_73DF6410F56E__INCLUDED_)
#define AFX_CLIENTPARTY_H__DB818FDB_228D_4091_B3DF_73DF6410F56E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "stdafx.h"
#include "AtumDatabase.h"		// 2013-03-13 by bhsohn 스킬 번호 구조 수정

class CEnemyData;

// 2009. 09. 21 by ckPark 인피니티 필드 몬스터 스킬 구현
struct sDebuffDotInfo
{
	INT		nSkillItemNum;	// 적용된 도트, 디버프 아이템 넘버
	INT		nSourceIndex;	// 도트, 디버프 소스 인덱스

	sDebuffDotInfo( ITEM* pSkillItem )
	{
		nSkillItemNum	= pSkillItem->ItemNum;
		// 2013-03-13 by bhsohn 스킬 번호 구조 수정
//		nSourceIndex	= SKILL_BASE_NUM( pSkillItem->SourceIndex );
		nSourceIndex	= SKILL_BASE_SOURCEINDEX( pSkillItem->SourceIndex );
		// END 2013-03-13 by bhsohn 스킬 번호 구조 수정
	}
};
// end 2009. 09. 21 by ckPark 인피니티 필드 몬스터 스킬 구현

struct PARTYENEMYINFO
{
	LONGLONG		m_nUniqueNumber;		// 편대원의 UniqueNumber
	PARTY_TYPE		m_bPartyType;
	BOOL			m_bFormationFlight;		// 편대 비행중인 파티원인가?
	BYTE			m_bFormationFlyingType;
	BYTE			m_bFormationFlyingPositionType;
	IM_PARTY_MEMBER_INFO m_ImPartyMemberInfo;
	CEnemyData*		m_pEnemyData;
	BOOL			m_bSpeakingRender;
	EN_CHECK_TYPE	VoipType;
	BOOL			m_bUserLogOn;		// 2008-12-02 by bhsohn 편대원 온오프라인 상태체크

	// 2009. 09. 21 by ckPark 인피니티 필드 몬스터 스킬 구현
	std::vector<sDebuffDotInfo> m_vecDebuffDotInfo;
	// end 2009. 09. 21 by ckPark 인피니티 필드 몬스터 스킬 구현
};

typedef struct 
{
	LONGLONG		m_nUniqueNumber;
	D3DXVECTOR3		m_vPos;
	D3DXVECTOR3		m_vVel;
	D3DXVECTOR3		m_vSide;
	D3DXVECTOR3		m_vUp;
} MASTERINFO;

typedef struct
{
	ClientIndex_t	ClientIndex;
	UID32_t			CharacterUniqueNumber;
} OTHERPARTYINFO;

class CClientParty  
{
public:
	CClientParty();
	virtual ~CClientParty();
	void			Tick(float fElapsedTime) const;	// 현재 사용안함
	static constexpr bool	GetNtoNCallState() { return false; } // m_bNtoNCallState; }
	BOOL			IsParty() const { return m_bParty; }
	BOOL			IsFormationFlight() const { return m_bFormationFlight; }	// 편대비행중인가?
	BOOL			IsFormationFlightMove();							// 편대비행 이동을 해야 하는가?
	void			SetMasterInfo();
	MASTERINFO		GetMasterInfo();
	void			SetPartyInfo(PARTYINFO infoParty) { memcpy((char*)&m_infoParty,(char*)&infoParty,sizeof(PARTYINFO)); }
	PARTYINFO		GetPartyInfo() const { return m_infoParty; }
	void			SetFormationFlight() { m_bFormationFlight = TRUE; }
	void			ReleaseFormationFlight() { m_bFormationFlight = FALSE; }
	void			FormationFlightDismember();											// 편대비행을 해체함. 
	void			FormationFlightClear();												// 편대비행 관련 초기화
	PARTYENEMYINFO*	PartyFindMemberByClientIndex(ClientIndex_t nClientIndex);			// 파티원 정보를 얻음
	PARTYENEMYINFO*	PartyFindMemberByUniqueNumber(LONGLONG nUniqueNumber);				// 파티원 정보를 얻음
	PARTYENEMYINFO* PartyFindMemberByUnitName(char* strName);							// 파티원 정보를 얻음
	PARTYENEMYINFO*	FormationFlightFindMemberByClientIndex(ClientIndex_t nClientIndex);	// 편대원 정보를 얻음
	PARTYENEMYINFO*	FormationFlightFindMemberByUniqueNumber(LONGLONG nUniqueNumber);	// 편대원 정보를 얻음

// 편대대형 관련 처리 함수들
	void	FormationFlightMoveForShuttleChild();
	void	FormationFlightMoveForEnemy();

// 편대전 관련 함수
	BOOL	IsFVFEnemy(UID32_t nUniqueNum);

// 파티 관련 프로토콜 받았을 때 처리 함수들(I->C)
	void	IPartyPutMember(MSG_IC_PARTY_PUT_MEMBER* pMsg);												// 파티원 한명을 추가하였을 때					MSG_IC_PARTY_PUT_MEMBER			
	void	IPartyAcceptInviteOk(MSG_IC_PARTY_ACCEPT_INVITE_OK* pMsg);									// 파티 초대를 수락했을 때						MSG_IC_PARTY_ACCEPT_INVITE_OK
	void	IPartyBanMemberOk(MSG_IC_PARTY_BAN_MEMBER_OK* pMsg);										// 파티원 한명을 추방하였을 때					MSG_IC_PARTY_BAN_MEMBER_OK
	void	IPartyLeaveOk(MSG_IC_PARTY_LEAVE_OK* pMsg);													// 파티원 한명이 나갔을 때						MSG_IC_PARTY_LEAVE_OK
	void	IPartyDismemberOk(MSG_IC_PARTY_DISMEMBER_OK* pMsg);											// 파티가 해산되었을 때							MSG_IC_PARTY_DISMEMBER_OK
	void	IPartyPutAllMember(MSG_IC_PARTY_PUT_ALL_MEMBER* pMsg);										// 모든 파티원 리스트를 받았을 때				MSG_IC_PARTY_PUT_ALL_MEMBER
	void	IPartyPutLastPartyInfo(MSG_IC_PARTY_PUT_LAST_PARTY_INFO* pMsg);								// 기존의 파티가 있을 때						MSG_IC_PARTY_PUT_LAST_PARTY_INFO
	void	IPartyTransferMasterOk(MSG_IC_PARTY_TRANSFER_MASTER_OK* pMsg);								// 파티 마스터가 변경되었을 때					MSG_IC_PARTY_TRANSFER_MASTER_OK
	void	IPartyMemberInvalidated(MSG_IC_PARTY_MEMBER_INVALIDATED* pMsg);								// 잘못된 파티원이 추가되었을 때?				MSG_IC_PARTY_MEMBER_INVALIDATED
	void	IPartyMemberRejoined(MSG_IC_PARTY_MEMBER_REJOINED* pMsg);									// 파티원이 다시 접속했을 때?					MSG_IC_PARTY_MEMBER_REJOINED
	void	IPartyUpdateMemberInfoMapname(MSG_IC_PARTY_UPDATE_MEMBER_INFO_MAPNAME* pMsg);				// ?											MSG_IC_PARTY_UPDATE_MEMBER_INFO_MAPNAME
	void	IPartyUpdateItemPos(MSG_IC_PARTY_UPDATE_ITEM_POS* pMsg);

	// 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템
	void	IPartyGetAutoPartyInfoOK( MSG_IC_PARTY_GET_AUTO_PARTY_INFO_OK* pMsg );
	// end 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템

// 파티 관련 프로토콜 받았을 때 처리 함수들(F->C)
	void	FPartyBattleEnd(MSG_FC_PARTY_BATTLE_END* pMsg);												// 파티전이 끝났을 때							MSG_FC_PARTY_BATTLE_END
	void	FPartyBattleReady(MSG_FC_PARTY_BATTLE_START* pMsg);											// 파티전이 시작되었을 때						MSG_FC_PARTY_BATTLE_READY
	void	FPartyCreateOk(MSG_FC_PARTY_CREATE_OK* pMsg);												// 파티를 생성했을 때							MSG_FC_PARTY_CREATE_OK
	void	FPartyPutItemOther(MSG_FC_PARTY_PUT_ITEM_OTHER* pMsg);										// 파티원이 아이템을 획득했을 때				MSG_FC_PARTY_PUT_ITEM_OTHER
	void	FPartyPutMember(MSG_FC_PARTY_PUT_MEMBER* pMsg);												// 파티원이 추가됐을 때	? 위에는?				MSG_FC_PARTY_PUT_MEMBER
	void	FPartyRejectInviteOk(MSG_FC_PARTY_REJECT_INVITE_OK* pMsg);									// 파티 초대를 거절했을 때						MSG_FC_PARTY_REJECT_INVITE_OK
	void	FPartyRequestInviteQuestion(MSG_FC_PARTY_REQUEST_INVITE_QUESTION* pMsg);					// ?											MSG_FC_PARTY_REQUEST_INVITE_QUESTION
	void	FPartyUpdateMemberInfoAll(MSG_FC_PARTY_UPDATE_MEMBER_INFO_ALL* pMsg);						// 파티원 한명의 모든 정보를 받았을 때			MSG_FC_PARTY_UPDATE_MEMBER_INFO_ALL
	void	FPartyUpdateMemberInfoBodyCondition(MSG_FC_PARTY_UPDATE_MEMBER_INFO_BODYCONDITION* pMsg);	// 파티원 한명의 바디컨디션을 받았을 때			MSG_FC_PARTY_UPDATE_MEMBER_INFO_BODYCONDITION
	void	FPartyUpdateMemberInfoCurrentDP(MSG_FC_PARTY_UPDATE_MEMBER_INFO_CURRENT_DP* pMsg);			//												MSG_FC_PARTY_UPDATE_MEMBER_INFO_CURRENT_DP
	void	FPartyUpdateMemberInfoCurrentEP(MSG_FC_PARTY_UPDATE_MEMBER_INFO_CURRENT_EP* pMsg);			//												MSG_FC_PARTY_UPDATE_MEMBER_INFO_CURRENT_EP
	void	FPartyUpdateMemberInfoCurrentHP(MSG_FC_PARTY_UPDATE_MEMBER_INFO_CURRENT_HP* pMsg);			//												MSG_FC_PARTY_UPDATE_MEMBER_INFO_CURRENT_HP
	void	FPartyUpdateMemberInfoCurrentSP(MSG_FC_PARTY_UPDATE_MEMBER_INFO_CURRENT_SP* pMsg);			//												MSG_FC_PARTY_UPDATE_MEMBER_INFO_CURRENT_SP
	void	FPartyUpdateMemberInfoDP(MSG_FC_PARTY_UPDATE_MEMBER_INFO_DP* pMsg);							//												MSG_FC_PARTY_UPDATE_MEMBER_INFO_DP
	void	FPartyUpdateMemberInfoEP(MSG_FC_PARTY_UPDATE_MEMBER_INFO_EP* pMsg);							//												MSG_FC_PARTY_UPDATE_MEMBER_INFO_EP
	void	FPartyUpdateMemberInfoHP(MSG_FC_PARTY_UPDATE_MEMBER_INFO_HP* pMsg);							//												MSG_FC_PARTY_UPDATE_MEMBER_INFO_HP
	void	FPartyUpdateMemberInfoSP(MSG_FC_PARTY_UPDATE_MEMBER_INFO_SP* pMsg);							//												MSG_FC_PARTY_UPDATE_MEMBER_INFO_SP
	void	FPartyUpdateMemberInfoLevel(MSG_FC_PARTY_UPDATE_MEMBER_INFO_LEVEL* pMsg);					// 파티원 한명의 레벨정보를 받았을 때			MSG_FC_PARTY_UPDATE_MEMBER_INFO_LEVEL
	// 2009. 09. 21 by ckPark 인피니티 필드 몬스터 스킬 구현
	void	FPartyUpdateMemberInfoDebuffDotInfo( MSG_FC_CHARACTER_DEBUFF_DOT_INFO_OK* pMsg );
	void	FPartyUpdateMemberInfoDebuffDotApply( MSG_FC_CHARACTER_DEBUFF_DOT_APPLYING* pMsg );
	void	FPartyUpdateMemberInfoDebuffDotRelease( MSG_FC_CHARACTER_DEBUFF_DOT_RELEASE* pMsg );
	// end 2009. 09. 21 by ckPark 인피니티 필드 몬스터 스킬 구현
	// 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템
	void	FPartyAutoCreateOK( MSG_FC_PARTY_AUTO_CREATED* pMsg );
	// end 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템

// 편대비행 관련 프로토콜 받았을 때 처리 함수들(I->C)
	void	IPartyAllFlightPosition(MSG_IC_PARTY_ALL_FLIGHT_POSITION* pMsg);							// 모든 편대원의 편대비행 포지션을 받을 때		MSG_IC_PARTY_ALL_FLIGHT_POSITION
	void	IPartyGetFlightPosition(MSG_IC_PARTY_GET_FLIGHT_POSITION* pMsg);							// 편대원이 추가되었을 때						MSG_IC_PARTY_GET_FLIGHT_POSITION
	void	IPartyCancelFlightPosition(MSG_IC_PARTY_CANCEL_FLIGHT_POSITION* pMsg);						// 편대원이 나갔을 때							MSG_IC_PARTY_CANCEL_FLIGHT_POSITION
	void	IPartyChangeFlightFormationOk(MSG_IC_PARTY_CHANGE_FLIGHT_FORMATION_OK* pMsg);				// 편대비행 타입이 변경되었을 때				MSG_IC_PARTY_CHANGE_FLIGHT_FORMATION_OK
	void	IPartyChangeFlightPosition(MSG_IC_PARTY_CHANGE_FLIGHT_POSITION* pMsg);						// ?											MSG_IC_PARTY_CHANGE_FLIGHT_POSITION

//	프로토콜 보낼 때 처리 함수들
	void	FSendPartyRequestPartyWarp();																// 파티 워프를 서버로 요청
	void	FSendPartyRequestPartyObjectEvent(CObjectChild * pObj);										// 파티 오브젝트 이벤트를 서버로 요청
	void	ISendPartyChangeFlightFormation(int nFormationFlightType);									// 편대비행 포메이션 타입 변경을 서버로 요청
	void	FSendEventSelectChannelWithParty(MSG_FC_EVENT_SELECT_CHANNEL msgEventSelectChannel);															// 편대워프 이벤트시 편대원과함께 서버로 요청      
	void	FSendPartyRequestPartyWarpWithMapName(char* pName);	
	void	ISendPartyGetFlightPosition();
	void	FSendPartyAcceptInvite(PartyID_t nPartyRequestID);
	void	FSendPartyRejectInvite(PartyID_t nPartyRequestID);
	void	ISendPartyLeave() const;
	void	ISendPartyBanMember(LONGLONG nUniqueNumber) const;
	void	ISendPartyTransferMaster(LONGLONG nUniqueNumber) const;
	static void	ISendPartyCreate();
	static void	FSendPartyRequestInvite(const char* pName);
	// 2008-02-27 by bhsohn 통합 아레나 수정
	static void	ISendPartyLeaveArena();


	// 2008-06-03 by bhsohn EP3 편대 관련 처리
	void			SetSPartyInfo(SPARTY_INFO *pSPartyInfo);
	SPARTY_INFO*	GetSPartyInfo() { return &m_stPartyInfo; }
	void			FSendPartyJoinFree(PartyID_t	nPartyRequestID, char* pPass);
	void			ChagePartyFormation(int nFormation);
	void			SetFirstPartySetupInfo(BOOL bFirstPartySetupInfo);
	BOOL			GetFirstPartySetupInfo();

	// 2008-06-17 by bhsohn 편대 관련 처리
	void			RefreshFormationType();						// 과거 편대대형으로 갱신
	void			TempPartyFormation(int nFormation);		// 스킬을 사용하여 편대 형태 변경
	int				GetOldFormationFlyingType();			// 과거 편대가있었다.

	// 2008-10-20 by bhsohn VOIP 버그 수정
	static constexpr EN_CHECK_TYPE GetVoiceTypePartyMaster() { return CHECK_TYPE_VOIP_NONE; }

	// 2008-12-16 by dgwoo 미션 마스터 관련 함수.
	void			SetMissionMasterPartyID(PartyID_t i_nID);
	PartyID_t		GetMissionMasterPartyID();

	// 2012-10-12 by jhjang 해피 아워 경험치 보너스 시스템 리뉴얼
	INT		GetInSameMapPartyMemberCount();
	// end 2012-10-12 by jhjang 해피 아워 경험치 보너스 시스템 리뉴얼

private:
// 파티 관련
	void			PartyCreate(PartyID_t PartyID);											// 파티 생성
	void			PartyDeleteAll();														// 모든 파티 정보를 삭제
	void			PartyInsertMember(IM_PARTY_MEMBER_INFO& ImPartyMemberInfo);								// 파티원 추가
	void			PartyDeleteMember(LONGLONG nUniqueNumber,char* pName);					// 파티원 삭제
	void			PartyChangeMaster(LONGLONG nNewMasterUniqueNumber,LONGLONG nOldMasterUniqueNumber,char* pName);			// 마스터 변경

// 편대비행 관련
	void			FormationFlightInsertMember(LONGLONG nUniqueNumber);				// 편대원이 추가되었을 때
	void			FormationFlightDeleteMember(LONGLONG nUniqueNumber);				// 편대원이 나갔을 때
	void			FormationFlightChangeType(BYTE nFormationFlightType);				// 편대비행 타입이 변경되었을 때
	BOOL			FormationFlightSortPosition();										// 편대원들의 포지션을 정렬한다.

// 편대대형 관련
	void	FormationFlightForShuttleChildType1();
	void	FormationFlightForShuttleChildType2();
	void	FormationFlightForShuttleChildType3();
	void	FormationFlightForShuttleChildType4();
	void	FormationFlightForShuttleChildType5();
	void	FormationFlightForShuttleChildType6();
	void	FormationFlightForShuttleChildType7();
	void	FormationFlightForShuttleChildType8();
	void	FormationFlightForEnemyType1(PARTYENEMYINFO* pEnemyData);
	void	FormationFlightForEnemyType2(PARTYENEMYINFO* pEnemyData);
	void	FormationFlightForEnemyType3(PARTYENEMYINFO* pEnemyData);
	void	FormationFlightForEnemyType4(PARTYENEMYINFO* pEnemyData);
	void	FormationFlightForEnemyType5(PARTYENEMYINFO* pEnemyData);
	void	FormationFlightForEnemyType6(PARTYENEMYINFO* pEnemyData);
	void	FormationFlightForEnemyType7(PARTYENEMYINFO* pEnemyData);
	void	FormationFlightForEnemyType8(PARTYENEMYINFO* pEnemyData);
	void	FormationFlightForEnemySetPos(PARTYENEMYINFO* pEnemyData) const;

public:
	vector<PARTYENEMYINFO*>		m_vecPartyEnemyInfo;		// 나와 편대중인 EnemyList
	vector<PEER_PARTY_MEMBER>	m_vecPeerPartyEnemyInfo;	// 우리 편대와 전투중인 다른 파티원 List
private:
	BOOL		m_bParty;								// 내가 파티중인가?
	BOOL		m_bFormationFlight;						// 내가 편대비행중인가?
	PARTYINFO	m_infoParty;							// 나의 파티정보
	MASTERINFO	m_infoMaster;							// 편대 마스터 정보
	
	// 2016-07-19 panoskj disabled this													
	//BOOL		m_bNtoNCallState;						// 편대 통화 중?

	// 2008-06-03 by bhsohn EP3 편대 관련 처리
	SPARTY_INFO			m_stPartyInfo;

	int				m_nOldFormationFlyingType;
	BOOL			m_bFirstPartySetupInfo;

	// 2008-12-16 by dgwoo 미션마스터 임시 파티 번호.
	PartyID_t		m_nTempMMasterPartyID;
};

#endif // !defined(AFX_CLIENTPARTY_H__DB818FDB_228D_4091_B3DF_73DF6410F56E__INCLUDED_)
