// WRankingLevelManager.cpp: implementation of the CWRankingLevelManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "WRankingLevelManager.h"
#include "AtumSJ.h"
#include "FieldGlobal.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWRankingLevelManager::CWRankingLevelManager()
{

}

CWRankingLevelManager::~CWRankingLevelManager()
{

}


///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2009-02-12 by cmkwon, EP3-3 월드랭킹시스템 구현 - 
/// \author		cmkwon
/// \date		2009-02-12 ~ 2009-02-12
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CWRankingLevelManager::InitEachWRankingManager(mtvectSWRK_LEVEL_FOR_DB *i_pvectAllRankingList)
{
	mt_auto_lock mta(&m_vectAllRankerList);
	
	// 2009-02-12 by cmkwon, 기존 랭킹 초기화
	this->ClearEachWRankingManager();
	
	m_vectAllRankerList.reserve(i_pvectAllRankingList->size());
	mtvectSWRK_LEVEL_FOR_DB::iterator itr(i_pvectAllRankingList->begin());
	for(; itr != i_pvectAllRankingList->end(); ++itr)
	{
		SWRK_LEVEL_FOR_DB *pRanker = &*itr;
		m_vectAllRankerList.push_back(*pRanker);		
	}

	for(int i=0; i < 5; i++)
	{
		if(FALSE == m_arrWRankingofEachUnitKind[i].InitWRanking(GetUnitMaskByArrIdxforWRK(i), &m_vectAllRankerList))
		{
			g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ERROR] CWRankingLevelManager::InitEachWRankingManager# %s \r\n", CAtumSJ::GetReqUnitKindString(GetUnitMaskByArrIdxforWRK(i)));
		}
	}
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2009-02-12 by cmkwon, EP3-3 월드랭킹시스템 구현 - 
/// \author		cmkwon
/// \date		2009-02-12 ~ 2009-02-12
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CWRankingLevelManager::ClearEachWRankingManager(void)
{
	mt_auto_lock mta(&m_vectAllRankerList);

	for(int i=0; i < 5; i++)
	{
		m_arrWRankingofEachUnitKind[i].ClearRanking();
	}

	m_vectAllRankerList.clear();
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
CWRankingLevel *CWRankingLevelManager::GetWRankingListByUnitMask(INT i_nUnitMask)
{
	auto idx = GetArrIdxByUnitMaskforWRK(i_nUnitMask);
	
	if(!util::in_range(0, idx, 4)) return nullptr;

	return &(m_arrWRankingofEachUnitKind[idx]);
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
int CWRankingLevelManager::SendLevelRankerList(INT i_nUnitMask, BYTE i_byRankingScope, CFieldIOCPSocket *i_pToFISoc, int i_nStartRanking, int i_nReqSendCnt/*=SIZE_MAX_REQUEST_COUNT*/)
{
	mt_auto_lock mta(&m_vectAllRankerList);
	CWRankingLevel *pRankingList = this->GetWRankingListByUnitMask(i_nUnitMask);
	if(NULL == pRankingList)
	{
		return 0;
	}

	return pRankingList->SendRankerList(i_byRankingScope, i_pToFISoc, i_nStartRanking, i_nReqSendCnt);
}