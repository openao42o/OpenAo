#ifndef _MAP_OWNERSHIP_H_
#define _MAP_OWNERSHIP_H_

#include "mt_stl.h"
#include "ODBCStatement.h"

class CFieldIOCP;

///////////////////////////////////////////////////////////////////////////////
// MAP_OWNERSHIP
///////////////////////////////////////////////////////////////////////////////
struct MAP_OWNERSHIP
{
	MapIndex_t			MapIndex;
	UID32_t				OwnerGuildUniqueNumber;
	char				OwnerGuildName[SIZE_MAX_GUILD_NAME];

	MAP_OWNERSHIP();
	void Reset();
};

///////////////////////////////////////////////////////////////////////////////
// CMapOwnershipManager
///////////////////////////////////////////////////////////////////////////////
class CMapOwnershipManager
{
public:
	CMapOwnershipManager();
	virtual ~CMapOwnershipManager();

	// 2008-07-11 by cmkwon, MySQL 지원을 위해 수정(IP,Port 로 DB 서버 연결) - 아래와 같이 수정 함
	//BOOL InitMapOwnershipManager(CFieldIOCP *i_pFieldIOCP, UCHAR *i_szDSN, UCHAR *i_szUID, UCHAR *i_szPASSWORD);
	BOOL InitMapOwnershipManager(CFieldIOCP *i_pFieldIOCP, char *i_szServIP, int i_nServPort, char *i_szDatabaseName, char *i_szUID, char *i_szPassword, HWND i_hWnd);
	void CleanMapOwnershipManager();

//	BOOL LoadMapOwnership();
	BOOL GetMapOwnership(MapIndex_t i_nMapIndex, MAP_OWNERSHIP *o_pMapOwnership);
	BOOL GetGuildWarChallengers(UID32_t i_nGuildUniqueNumber, vector<string> *o_pChallengers);

	// 길드전 신청, 취소
// 2008-07-24 by cmkwon, MySQL 포팅 관련(OUTPUT 제거) - 사용하지 않는 함수, 주석 처리 함.
//	INT InsertRequestForGuildWar(MapIndex_t i_nMapIndex, char *i_szDefenderGuildName, char *i_szChallengerGuildName);

	BOOL CancelRequestForGuildWar(char *i_szDefenderGuildName, char *i_szChallengerGuildName);

private:
//	mt_map<MapIndex_t, MAP_OWNERSHIP>	m_mapMapOwership;
	CODBCStatement						m_ODBCStmt5;
	CFieldIOCP							*m_pFieldIOCP3;
};

#endif // _MAP_OWNERSHIP_H_
