// IMParty.h: interface for the CIMParty class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IMPARTY_H__851E1E96_0EBA_43A7_A37A_CC68D4255EE9__INCLUDED_)
#define AFX_IMPARTY_H__851E1E96_0EBA_43A7_A37A_CC68D4255EE9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AtumProtocol.h"
#include "MTCriticalSection.h"


class CIMIOCP;
class CIMIOCPSocket;
class CIMParty;

struct IMPartyMember
{
public:
	IMPartyMember();
	IMPartyMember(CIMParty *i_pParty);
	IMPartyMember(char *i_szCharName, UID32_t i_CharacterUID, CIMIOCPSocket *i_pIISocket, CIMParty *i_pParty);
	void ResetIMPartyMember(void);

	char			PM_CharacterName[SIZE_MAX_CHARACTER_NAME];	// 게임에서 나가있는 파티원의 이름을 알려주기 위해
	UID32_t			PM_CharacterUniqueNumber;					// 게임에서 나가있는 파티원도 유지해준다
	CIMIOCPSocket	*PM_pIMIOCPSocket;							// 현재 파티원이 사용중인 CIMIOCPSocket의 포인터, NULL이면 온라인상태가 아니다
	CIMParty		*PM_pIMParty;

	BOOL IsUsingMember(void);

	BOOL IsOnlineMember(void);									// 게임중인 파티원인지 확인
	void SetOnlineMember(CIMIOCPSocket *pCharacterSocket);		// 게임 종료 후 다시 들어왔을 때, CHECK_REJOIN를 받으면 수행함
	void SetOfflineMember( void );	// 비정상적으로 게임종료되었을 때
};

class CIMParty
{
public:
	CIMParty(CIMIOCP* pIMIOCP, UID32_t CharacterUniqueNumber, DWORD i_dwCreateTick, UID32_t UniqueNumber, SPARTY_INFO * i_pSPartyInfo); 	// 2008-06-02 by dhjin, EP3 편대 수정 - 편대 정보 );
	~CIMParty();
	
	BOOL IsValidIMParty(void);
	BOOL IsExistMember(UID32_t i_uidCharacter, BOOL i_bIsOnline=TRUE);
	
	BOOL PushBackMember(IMPartyMember &partyMember);
	BOOL LeaveMember(UID32_t i_uidCharacter);
	BOOL TransferMaster(UID32_t i_uidNewMasterCharacter=INVALID_UNIQUE_NUMBER, BOOL i_bResetFlag=FALSE, BOOL i_bSetOffline=FALSE);
	
	char *GetMasterCharacterName(void);
	UID32_t GetMasterCharacterUniqueNumber(void);
	CIMIOCPSocket *GetMasterIMIOCPSocket(void);

	IMPartyMember* GetMemberPtrByCharacterUniqueNumber(UID32_t characterUniqueNumber);
	BYTE GetLowestMemberLevel(UINT CharacterToExclude = INVALID_UNIQUE_NUMBER);

	BOOL SendMsgToMembers(BYTE* buffer, int size, UINT CharacterToExclude=INVALID_UNIQUE_NUMBER, BOOL i_bResetIMIOCPSocketParty=FALSE);
//	BOOL SendChatMsgToMembers(BYTE* buffer, int size, BitFlag8_t i_bitFlag8);
	BOOL SendChatMsgToMembers(BYTE* buffer, int size, BitFlag16_t i_bitFlag16);			// 2008-05-15 by dhjin, EP3 - 채팅 시스템 변경
	
	BOOL SetOfflineMemberW(UID32_t characterUniqueNumber );	// 파티중 게임종료되었을 때

	int GetMemberCount(BOOL bOnlyValidMember = TRUE);

	void LockIMParty(void){			m_mtcsParty.Enter();}
	void UnlockIMParty(void){		m_mtcsParty.Leave();}

	void MakeMsg_IC_PARTY_PUT_ALL_MEMBER(UINT *o_pnMemberCounts, IM_PARTY_MEMBER_INFO *o_pIMPMemberInfo, UID32_t i_uidCharacterToExclude);
	int MakeMsg_DB_GUILD_CREATE(int *o_pnMemberCounts, UID32_t o_arrUIDMember[]);

	//////////////////////////////////////////////////////////////////////////
	// 2008-06-04 by dhjin, EP3 편대 수정
	void SetPartyInfo(SPARTY_INFO * i_pPartyInfo);		// 2008-06-04 by dhjin, EP3 편대 수정 - 편대 정보 수정
	void SendRecommendationMember(CIMIOCPSocket * i_pIISocket);	// 2008-06-04 by dhjin, EP3 편대 수정 - 추천 케릭터 요청
	INT  MakeMSG_IC_PARTY_RECOMMENDATION_MEMBER_OK(SRECOMMENDATION_MEMBER_INFO * o_pRecommendationMember, CIMIOCPSocket * i_pPartyMasterSocket, INT i_nMinLevel, INT i_nMaxLevel);	// 2008-11-18 by dhjin, 파티장이 정상적으로 존재하는지 체크. 게임포지 IM서버 다운 현상 관련
	
	BOOL CheckVoipUse();				// 2008-06-17 by dhjin, EP3 VOIP - 사용중인지 체크
	BOOL m_bArenaIMServerCheck;			// 2012-04-12 by jhseol, 아레나 추가개발 - 자동파티 : 아레나 서버인지 체크

public:
	CMTCriticalSection		m_mtcsParty;
	IMPartyMember			m_arrPartyMember[SIZE_MAX_PARTY_MEMBER];
	
	PartyID_t				m_PartyID;
	DWORD					m_PartyCreationTick;		// 파티 생성 시간, timeGetTime()으로 할당함
	DWORD					m_PartyDeletedTick;			// 파티 삭제 시간, timeGetTime()으로 할당함
	IMPartyMember			*m_pMasterIMPartyMember;	// 파티장의 IMPartyMember

	static CIMIOCP			*ms_pIMIOCP2;

	SPARTY_INFO				m_SPartyInfo;				// 2008-06-02 by dhjin, EP3 편대 수정 - 편대 정보 
};

inline IM_PARTY_MEMBER_INFO& IM_PARTY_MEMBER_INFO::operator=(const IMPartyMember& rhs)
{
	memset(this, 0, sizeof(IM_PARTY_MEMBER_INFO));
	this->CharacterUniqueNumber	= rhs.PM_CharacterUniqueNumber;
	util::strncpy(this->CharacterName, rhs.PM_CharacterName, SIZE_MAX_CHARACTER_NAME);
	this->MapChannelIndex.Invalidate();
	return *this;
}

#endif // !defined(AFX_IMPARTY_H__851E1E96_0EBA_43A7_A37A_CC68D4255EE9__INCLUDED_)
