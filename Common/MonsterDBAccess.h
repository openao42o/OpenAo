#pragma once

#define MAXDBMSGBUFLEN   255

//#include <sql.h>
//#include <sqlext.h>
#include "AtumParam.h"

typedef enum
{
	MONSTER_LOAD_TYPE_SIZE_FOR_SERVER = 0,
	MONSTER_LOAD_TYPE_SIZE_FOR_CLIENT = 1
} MonsterLoadType;

class CLocalization;
class CMonsterDBAccess
{
public:
	CMonsterDBAccess();
	~CMonsterDBAccess();

	BOOL GetAllMonsters(ez_map<int, MONSTER_INFO> &mapMonster, ez_map<INT, ITEM*> *pMapItemInfo, MonsterLoadType loadType);
	static bool MonsterMoveTypeCheck(const MONSTER_INFO *pMon);
	static bool CheckMPOption(MONSTER_INFO *pMonInfo);

private:
	SQLHENV		henv;
	SQLHDBC		hdbc;
	SQLHSTMT	hstmt;
};
