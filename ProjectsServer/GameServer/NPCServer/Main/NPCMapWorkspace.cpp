// NPCMapWorkspace.cpp: implementation of the CNPCMapWorkspace class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NPCMapWorkspace.h"
#include "MapProject.h"
#include "NPCMapProject.h"
#include "config.h"
#include "NPCGlobal.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNPCMapWorkspace::CNPCMapWorkspace(CIOCP* pIOCP, int nSizeBlockX, int nSizeBlockZ)
:CMapWorkspace(pIOCP, nSizeBlockX, nSizeBlockZ)
{
}

CNPCMapWorkspace::~CNPCMapWorkspace()
{
	
}



//////////////////////////////////////////////////////////////////////
// Method
//////////////////////////////////////////////////////////////////////
CMapProject * CNPCMapWorkspace::CreateChildMapProject(PROJECTINFO * i_pProjectInfo)
{	
// 	static int sTotalCount	= 0;
// 	static int sTotalByte	= 0;
// 	sTotalCount	+= 1;
// 	sTotalByte	+= sizeof(CNPCMapProject) * 1;
// 	g_pNPCGlobal->WriteSystemLogEX(TRUE, "[TEMP] 100408 CNPCMapProject TotalCount(%8d) TotalBytes(%8d) \r\n", sTotalCount, sTotalByte);

	return new CNPCMapProject(this, i_pProjectInfo);
}

// 2008-09-10 by cmkwon, 통합아레나 맵설정(map.cfg) 관련 수정 - 
//BOOL CNPCMapWorkspace::LoadAllProject(const char* pFileName)
BOOL CNPCMapWorkspace::LoadAllProject(const char* pFileName, BOOL i_bIsArenaServer, vectMapIndexList *i_pvectArenaMapIndexList)
{
// 2008-09-10 by cmkwon, 통합아레나 맵설정(map.cfg) 관련 수정 - 
//	BOOL bReted = CMapWorkspace::LoadAllProject(pFileName);
	return CMapWorkspace::LoadAllProject(pFileName, i_bIsArenaServer, i_pvectArenaMapIndexList);
}


CNPCMapChannel *CNPCMapWorkspace::GetNPCMapChannelByMapChannelIndex(MAP_CHANNEL_INDEX i_mapChannIdx)
{
	CNPCMapProject * pNMPro = (CNPCMapProject*)GetMapProjectByMapIndex(i_mapChannIdx.MapIndex);
	if(NULL == pNMPro){				return NULL;}

	return pNMPro->GetNPCMapChannelByIndex(i_mapChannIdx.ChannelIndex);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CNPCMapWorkspace::AddMessageMSG_FN_MONSTER_STRATEGYPOINT_INIT(MSG_FN_MONSTER_STRATEGYPOINT_INIT *pMsg)
/// \brief		
/// \author		cmkwon
/// \date		2006-11-21 ~ 2006-11-21
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CNPCMapWorkspace::AddMessageMSG_FN_MONSTER_STRATEGYPOINT_INIT(MSG_FN_MONSTER_STRATEGYPOINT_INIT *pMsg)
{
	vector<CMapProject*>::iterator itr = m_vectorPtrMapProject.begin();

	for(; itr != m_vectorPtrMapProject.end(); itr++)
	{
		CNPCMapProject *pNMapPro	= (CNPCMapProject*)(*itr);
		if(pMsg->bVCNMapInflTyforInit)
		{
			if(IS_MAP_INFLUENCE_VCN(pNMapPro->m_nMapInfluenceType))
			{
				pNMapPro->NPCOnMonsterStrategyPointInitW(pMsg);
			}
		}
		else
		{
			if(IS_MAP_INFLUENCE_ANI(pNMapPro->m_nMapInfluenceType))
			{
				pNMapPro->NPCOnMonsterStrategyPointInitW(pMsg);
			}
		}		
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CNPCMapWorkspace::AddMessageMSG_FN_MONSTER_STRATEGYPOINT_SUMMON(MapIndex_t i_MapIndex)
/// \brief		
/// \author		dhjin
/// \date		2007-03-02 ~ 2007-03-02
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CNPCMapWorkspace::AddMessageMSG_FN_MONSTER_STRATEGYPOINT_SUMMON(MapIndex_t i_MapIndex)
{
	for (auto pNMapPro : m_vectorPtrMapProject)
	
		if (pNMapPro->m_nMapIndex == i_MapIndex)
		{
			MSG_FN_MONSTER_STRATEGYPOINT_INIT pMsg;
			
			pMsg.ChannelIndex		= 0;
			pMsg.bCreateFlag		= true;

			// 2007-09-19 by cmkwon, 기본값
			// 2007-10-25 by dhjin, 바이제니유 세력 맵 체크 오류로 수정
			pMsg.bVCNMapInflTyforInit = IS_MAP_INFLUENCE_VCN(pNMapPro->m_nMapInfluenceType);	
			
			pMsg.bInfluenceBoss		= false;	// 2007-08-30 by dhjin, 모선전시 전략포인트 소환인지 체크 비트 0 : FASLE, 1 : TRUE
			
			static_cast<CNPCMapProject*>(pNMapPro)->NPCOnMonsterStrategyPointInitW(&pMsg);
		}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2010-03-18 by cmkwon, 소환 이벤트 몬스터 관련 문제 수정 - 
/// \author		cmkwon
/// \date		2010-03-18 ~ 2010-03-18
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
CNPCMapProject *CNPCMapWorkspace::GetNPCMapProject(MapIndex_t i_mapIdx)
{
	// 2010-03-23 by cmkwon, 버그 수정함.
	return (CNPCMapProject*)GetMapProjectByMapIndex(i_mapIdx);
}