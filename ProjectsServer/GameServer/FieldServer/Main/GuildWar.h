#ifndef _GUILD_WAR_H_
#define _GUILD_WAR_H_

#include "mt_stl.h"
#include "ODBCStatement.h"

class CFieldIOCPSocket;

typedef vector<UID32_t>		vectorUID32;

///////////////////////////////////////////////////////////////////////////////
// SGuildForGuildWar
///////////////////////////////////////////////////////////////////////////////
struct SGuildForGuildWar
{
	UID32_t			uidGuildUID;					// GuildUID
	vectorUID32		vectorUID32CharacterUID;		// 길드 멤버의 CharacterUID vector

	BOOL IsGuildMember(UID32_t i_uidCharacterUID);
};

///////////////////////////////////////////////////////////////////////////////
// CGuildWar
///////////////////////////////////////////////////////////////////////////////
class CGuildWarMananger;
class CGuildWar
{
	friend CGuildWarMananger;
public:
	CGuildWar(CGuildWarMananger *i_pGuildWarManager);

	MAP_CHANNEL_INDEX GetMapChannelIndex(void);
	SGuildForGuildWar *GetPeerGuildForGuildWar(UID32_t i_GuildUID);
	SGuildForGuildWar *GetDefenderGuildPtr(void);
	SGuildForGuildWar *GetChallengerGuildPtr(void);
	
	void PrintGuildWar(void);

protected:
	SGuildForGuildWar		m_DefenderGuild;	// 수성 길드	
	SGuildForGuildWar		m_ChallengerGuild;	// 도전 길드
	MAP_CHANNEL_INDEX		m_MapChannelIndex;
	CGuildWarMananger		*m_pGuildWarManager;
};

class CFieldIOCP;

typedef mt_map<UID32_t, CGuildWar*>		mtmapUID32GuildWar;
///////////////////////////////////////////////////////////////////////////////
// CGuildWarMananger
///////////////////////////////////////////////////////////////////////////////
class CGuildWarMananger
{
public:
	CGuildWarMananger();
	virtual ~CGuildWarMananger();

	mtmapUID32GuildWar *GetmtmapGuildWarPtr(void){		return &m_mtmapGuildWar;};
	void LockGuildWarManager(void){			m_mtmapGuildWar.lock();}
	void UnlockGuildWarManager(void){		m_mtmapGuildWar.unlock();}
	// 2008-07-11 by cmkwon, MySQL 지원을 위해 수정(IP,Port 로 DB 서버 연결) - 아래와 같이 수정 함
	//BOOL InitGuildWarMananger(CFieldIOCP *i_pFieldIOCP, UCHAR *i_szDSN, UCHAR *i_szUID, UCHAR *i_szPASSWORD);
	BOOL InitGuildWarMananger(CFieldIOCP *i_pFieldIOCP, const char *i_szServIP, int i_nServPort, const char *i_szDatabaseName, char *i_szUID, char *i_szPassword, HWND i_hWnd);
	void CleanGuildWarMananger();

	BOOL CheckGuildCommander(UID32_t i_CharacterUniqueNumber, UID32_t *o_pGuildUID=NULL);
	BOOL CheckGuildCommanderAndGetMemberCapacity(UID32_t i_CharacterUniqueNumber, int *o_pGuildMemberCapacity);
	BOOL MakeGuildForGuildWar(SGuildForGuildWar *o_pGuildForGuildWar, UID32_t i_uidGuildUID);

	CGuildWar* FindGuildWarPtr(UID32_t i_nGuildUID);
	CGuildWar* PopGuildWarPtr(UID32_t i_nGuildUID);

	BOOL StartGuildWar(UID32_t i_uidDefenderGuildUID, UID32_t i_uidChallengerGuildUID, MAP_CHANNEL_INDEX *i_pMapChannelIdx);
	BOOL EndGuildWar(UID32_t i_nGuildUID, BOOL i_bSurrender = FALSE);

	void PrintGuildWar(UID32_t i_nGuildUID);

	UID32_t GetGuildCommanderUID(UID32_t i_nGuildUID);	// 2007-08-27 by dhjin, 여단 UID로 여단장 UID 얻어오기
	UID32_t GetLeaderGuildUID(UID32_t i_LeaderUniqueNumber);	// 2007-09-07 by dhjin, 지도자 UID로 지도자 여단 UID가져오기
	BOOL CheckGuildStateDisMemberReady(UID32_t i_nGuildUID);	// 2007-11-09 by dhjin, 여단 해체 대기 중인지 체크	
	void GetGuildCommanderName(UID32_t i_nGuildCommanderUID, char * o_pGuildCommanderName);	// 2007-12-03 by dhjin, 여단 UID로 여단장 이름 얻어오기

private:
	mtmapUID32GuildWar		m_mtmapGuildWar;
	CODBCStatement			m_ODBCStmt1;

public:
	CFieldIOCP				*m_pFieldIOCP5;
};

#endif // _GUILD_WAR_H_
