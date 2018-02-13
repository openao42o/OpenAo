#ifndef __ATUM_IM_DB_MANAGER_H__
#define __ATUM_IM_DB_MANAGER_H__

#include "AtumDBManager.h"

class CIMIOCP;
class CIMIOCPSocket;

class CAtumIMDBManager : public CAtumDBManager
{
public:
	struct MSG_DB_GUILD_CREATE
	{
		char		szGuildName[SIZE_MAX_GUILD_NAME];				// 여단이름
		UID32_t		uidCommanderCharacter;							// 여단장 CharacterUniqueNumber
		int			nMemberCounts;									// 여단원 카운트
		UID32_t		arrMemberCharacterUID[SIZE_MAX_PARTY_MEMBER];	// 여단원 CharacterUniqueNumber(여단장 포함)
		BYTE		arrGuildRank[SIZE_MAX_PARTY_MEMBER];
	};
	
public:
	CAtumIMDBManager(CIMIOCP *pIMIOCP);

	BOOL ProcessServerQuery(DB_QUERY dbquery, SQLHSTMT &hstmt, SQLHSTMT &hstmt_mc, SQLHSTMT &hstmt_extAuth, SQLHSTMT &hstmt_GLog);		// 2013-06-20 by jhseol,bckim GLog 보완

	void QP_AuthChatLogin(DB_QUERY q, SQLHSTMT hstmt);
	void QP_SaveLastPartyID(DB_QUERY q, SQLHSTMT hstmt);
	void QP_GuildCreate(DB_QUERY q, SQLHSTMT hstmt);
	void QP_GuildAddMember(DB_QUERY q, SQLHSTMT hstmt);
	void QP_GuildAddOffMember(DB_QUERY q, SQLHSTMT hstmt);			// 2008-06-12 by dhjin, EP3 - 여단 수정 사항 - 오프유저 여단 가입
	void QP_GuildLeaveMember(DB_QUERY q, SQLHSTMT hstmt);
	void QP_GuildBanMember(DB_QUERY q, SQLHSTMT hstmt);
	void QP_GuildUpdateMemberCapacity(DB_QUERY q, SQLHSTMT hstmt);
	void QP_GuildLoadGuild(DB_QUERY q, SQLHSTMT hstmt);
	void QP_GuildDismember(DB_QUERY q, SQLHSTMT hstmt);
	void QP_GuildCancelDismember(DB_QUERY q, SQLHSTMT hstmt);
	void QP_GuildChangeGuildName(DB_QUERY q, SQLHSTMT hstmt);
	void QP_GuildSetGuildMark(DB_QUERY q, SQLHSTMT hstmt);
// 2007-08-02 by cmkwon, 여단 마크 심사 시스템 구현 - 사용하지 안는 함수임
//	void QP_GuildGetGuildMark(DB_QUERY q, SQLHSTMT hstmt);
	void QP_GuildSetRank(DB_QUERY q, SQLHSTMT hstmt);
	void QP_GuildDeleteGuild(DB_QUERY q, SQLHSTMT hstmt);
	void QP_GuildSaveGuildWarPoint(DB_QUERY q, SQLHSTMT hstmt);
	void QP_GuildDeleteGuildUIDOfCharacter(DB_QUERY q, SQLHSTMT hstmt);
	void QP_GuildAddGuildFame(DB_QUERY q, SQLHSTMT hstmt);
	void QP_GuildUpdateCommander(DB_QUERY q, SQLHSTMT hstmt);		// 2008-05-20 by dhjin, EP3 - 여단 수정 사항 - 여단장 위임
	void QP_GuildNotice(DB_QUERY q, SQLHSTMT hstmt);				// 2008-05-20 by dhjin, EP3 - 여단 수정 사항 - 여단 공지
	void QP_GuildGetApplicant(DB_QUERY q, SQLHSTMT hstmt);			// 2008-05-27 by dhjin, EP3 - 여단 수정 사항 - 여단 소개
	void QP_GuildGetIntroduction(DB_QUERY q, SQLHSTMT hstmt);			// 2008-05-27 by dhjin, EP3 - 여단 수정 사항 - 여단 소개
	void QP_GuildDeleteIntroduction(DB_QUERY q, SQLHSTMT hstmt);			// 2008-05-27 by dhjin, EP3 - 여단 수정 사항 - 여단 소개
	void QP_GetSelfIntroduction(DB_QUERY q, SQLHSTMT hstmt);			// 2008-05-27 by dhjin, EP3 - 여단 수정 사항 - 여단 지원자 소개서
	void QP_GuildSearchIntroduction(DB_QUERY q, SQLHSTMT hstmt);			// 2008-05-27 by dhjin, EP3 - 여단 수정 사항 - 여단 소개 검색
	void QP_GuildUpdateIntroduction(DB_QUERY q, SQLHSTMT hstmt);		// 2008-05-27 by dhjin,	EP3 - 여단 수정 사항 - 여단 소개 작성
	void QP_GuildUpdateSelfIntroduction(DB_QUERY q, SQLHSTMT hstmt);	// 2008-05-27 by dhjin, EP3 - 여단 수정 사항 - 자기 소개 작성 
	void QP_GuildDeleteSelfIntroduction(DB_QUERY q, SQLHSTMT hstmt);	// 2008-05-27 by dhjin, EP3 - 여단 수정 사항 - 자기 소개 지우기 
	void QP_GuildDeleteSelfIntroductionOffUser(DB_QUERY q, SQLHSTMT hstmt);	// 2008-06-13 by dhjin, EP3 - 여단 수정 사항 - 자기 소개 지우기 
	void QP_InsertNotifyMsg(DB_QUERY q, SQLHSTMT hstmt);	// 2008-06-13 by dhjin, EP3 - 여단 수정 사항 - 통지 메세지 보내기

	void QP_FriendInsertFriend(DB_QUERY q, SQLHSTMT hstmt);
	void QP_FriendDeleteFriend(DB_QUERY q, SQLHSTMT hstmt);
	void QP_FriendLoadFriends(DB_QUERY q, SQLHSTMT hstmt);

	void QP_ReloadAdminAutoNotice(DB_QUERY q, SQLHSTMT hstmt);		// 2009-01-14 by cmkwon, 운영자 자동 공지 시스템 구현 - 

	///////////////////////////////////////////////////////////////////////////////
	// 2008-12-01 by cmkwon, 쿼리별로 스레드를 만들어 처리하는 시스템 구축 - 
	virtual BOOL ProcessDynamicServerQuery(DB_QUERY *i_qQuery, CODBCStatement *i_pODBC);

private:
	void ProcessLogMessagesIM(SQLSMALLINT plm_handle_type, SQLHANDLE &plm_handle, char *logstring, int ConnInd, CIOCPSocket *pIOCPSocket);

public:
	CIMIOCP	*m_pIMIOCP5;
};

struct QPARAM_SAVELASTPARTYID
{
	UID32_t		CharacterUniqueNumber;
	PartyID_t	LastPartyID;
};

struct QPARAM_CHANGE_GUILD_NAME
{
	UID32_t		GuildUniqueNumber;
	char		NewGuildName[SIZE_MAX_GUILD_NAME];
};

//////////////////////////////////////////////////////////////////////////
// 2008-05-28 by dhjin, EP3 - 여단 수정 사항
struct QPARAM_GUILD_NOTICE
{
	char		Notice[SIZE_MAX_NOTICE];
};

struct QPARAM_GUILD_UPDATE_INTRODUCTION
{
	char			GuildIntroduction[SIZE_MAX_NOTICE];
	ATUM_DATE_TIME	WriteDate;
};

struct QPARAM_GUILD_UPDATE_SELFINTRODUCTION
{
	UID32_t		TargetGuildUID;
	char		SelfIntroduction[SIZE_MAX_NOTICE];
};

#endif
