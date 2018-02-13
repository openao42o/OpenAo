// WRankingManager.cpp: implementation of the CWRankingManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "WRankingManager.h"
#include "FieldGlobal.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWRankingManager::CWRankingManager()
{

}

CWRankingManager::~CWRankingManager()
{

}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		cmkwon
/// \date		2009-02-19 ~ 2009-02-19
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int CWRankingManager::SendServiceListW(CFieldIOCPSocket *i_pToFISoc)
{
	if(g_pFieldGlobal->IsArenaServer()
		|| g_pFieldGlobal->GetIsJamboreeServer())
	{
		return 0;
	}

	return m_ServiceManager.SendServiceList(i_pToFISoc);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2009-02-12 by cmkwon, EP3-3 월드랭킹시스템 구현 - 
/// \author		cmkwon
/// \date		2009-02-18 ~ 2009-02-18
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int CWRankingManager::SendRankerListByRankingType(BYTE i_byRankingTy, INT i_nUnitMask, BYTE i_byRankingScope, CFieldIOCPSocket *i_pToFISoc, int i_nStartRanking, int i_nReqSendCnt/*=SIZE_MAX_REQUEST_COUNT*/)
{
	if(g_pFieldGlobal->IsArenaServer()
		|| g_pFieldGlobal->GetIsJamboreeServer())
	{
		return 0;
	}

	switch(i_byRankingTy)
	{
	case RANKING_TYPE_LEVEL:
		{
			return m_LevelRankManager.SendLevelRankerList(i_nUnitMask, i_byRankingScope, i_pToFISoc, i_nStartRanking, i_nReqSendCnt);
		}
		break;
	case RANKING_TYPE_FAME:
		{
			return m_FameRankManager.SendLevelRankerList(i_nUnitMask, i_byRankingScope, i_pToFISoc, i_nStartRanking, i_nReqSendCnt);
		}
		break;
	case RANKING_TYPE_PVP:
		{
			return m_PVPRankManager.SendLevelRankerList(i_nUnitMask, i_byRankingScope, i_pToFISoc, i_nStartRanking, i_nReqSendCnt);
		}
		break;
	}

	g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ERROR] CWRankingManager::SendRankerListByRankingType# invalid RankingType(%d) !!", i_byRankingTy);
	return 0;
}