// NPCMapWorkspace.h: interface for the CNPCMapWorkspace class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NPCMAPWORKSPACE_H__4BE47F35_1EF2_4B61_8794_3B657D76F4BE__INCLUDED_)
#define AFX_NPCMAPWORKSPACE_H__4BE47F35_1EF2_4B61_8794_3B657D76F4BE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "IOCP.h"
#include "MapWorkspace.h"
#include "AtumProtocol.h"

class CNPCMapChannel;
class CNPCMapProject;		// 2010-03-18 by cmkwon, 소환 이벤트 몬스터 관련 문제 수정 - 
class CNPCMapWorkspace : public CMapWorkspace
{
public:
	CNPCMapWorkspace(CIOCP* pIOCP, int nSizeBlockX, int nSizeBlockZ);
	virtual ~CNPCMapWorkspace();
	
	virtual CMapProject * CreateChildMapProject(PROJECTINFO * i_pProjectInfo);

// 2008-09-10 by cmkwon, 통합아레나 맵설정(map.cfg) 관련 수정 - 
//	virtual	BOOL LoadAllProject(const char* pFileName);
	virtual	BOOL LoadAllProject(const char* pFileName, BOOL i_bIsArenaServer, vectMapIndexList *i_pvectArenaMapIndexList);

	CNPCMapChannel *GetNPCMapChannelByMapChannelIndex(MAP_CHANNEL_INDEX i_mapChannIdx);

	// 2006-11-21 by cmkwon
	void AddMessageMSG_FN_MONSTER_STRATEGYPOINT_INIT(MSG_FN_MONSTER_STRATEGYPOINT_INIT *pMsg);
	void AddMessageMSG_FN_MONSTER_STRATEGYPOINT_SUMMON(MapIndex_t i_MapIndex);

	// 2010-03-18 by cmkwon, 소환 이벤트 몬스터 관련 문제 수정 - 
	CNPCMapProject *GetNPCMapProject(MapIndex_t i_mapIdx);

protected:

};

#endif // !defined(AFX_NPCMAPWORKSPACE_H__4BE47F35_1EF2_4B61_8794_3B657D76F4BE__INCLUDED_)
