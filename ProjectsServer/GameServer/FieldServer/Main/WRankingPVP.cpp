// RankingPVP.cpp: implementation of the CWRankingPVP class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "WRankingPVP.h"
#include "FieldIOCPSocket.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWRankingPVP::CWRankingPVP()
{

	m_UnitKind		= 0;
	m_vectRankingList[RANKING_SCOPE_LOCAL].reserve(SIZE_MAX_WRK_COUNT);
	m_vectRankingList[RANKING_SCOPE_WORLD].reserve(SIZE_MAX_WRK_COUNT);
}

CWRankingPVP::~CWRankingPVP()
{

}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		cmkwon
/// \date		2009-02-18 ~ 2009-02-18
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CWRankingPVP::InitWRanking(int i_unitKindMask, mtvectSWRK_PVP *i_pvectAllRankingList)
{
	if(FALSE == COMPARE_BIT_FLAG(i_unitKindMask, UNITKIND_ALL_MASK))
	{
		return FALSE;
	}
	
	if(UNITKIND_ALL_MASK == i_unitKindMask)
	{
		mtvectSWRK_PVP::iterator itr(i_pvectAllRankingList->begin());
		for(;itr != i_pvectAllRankingList->end(); itr++)
		{
			SWRK_PVP *pRanker = &*itr;
			if(IS_VALID_RANKING_SCOPE(pRanker->RankingScope))
			{
				m_vectRankingList[pRanker->RankingScope].push_back(pRanker);
			}
		}
	}
	else
	{
		mtvectSWRK_PVP::iterator itr(i_pvectAllRankingList->begin());
		for(;itr != i_pvectAllRankingList->end(); itr++)
		{
			SWRK_PVP *pRanker = &*itr;
			if(IS_VALID_RANKING_SCOPE(pRanker->RankingScope)
				&& COMPARE_BIT_FLAG(pRanker->UnitKind, i_unitKindMask))
			{
				m_vectRankingList[pRanker->RankingScope].push_back(pRanker);
			}
		}
	}
	
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		cmkwon
/// \date		2009-02-18 ~ 2009-02-18
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CWRankingPVP::ClearRanking(void)
{
	m_vectRankingList[RANKING_SCOPE_LOCAL].clear();
	m_vectRankingList[RANKING_SCOPE_WORLD].clear();
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		cmkwon
/// \date		2009-02-18 ~ 2009-02-18
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int CWRankingPVP::SendRankerList(BYTE i_byRankingScope, CFieldIOCPSocket *i_pToFISoc, int i_nStartRanking, int i_nReqSendCnt/*=SIZE_MAX_REQUEST_COUNT*/)
{
	if(FALSE == IS_VALID_RANKING_SCOPE(i_byRankingScope)
		|| FALSE == i_pToFISoc->IsValidCharacter(FALSE))
	{
		return 0;
	}
	if(i_nStartRanking > m_vectRankingList[i_byRankingScope].size())
	{
		return 0;
	}
	i_nReqSendCnt	= min(SIZE_MAX_REQUEST_COUNT, i_nReqSendCnt);
	
	int nStartIdx	= max(0, i_nStartRanking-1);
	int nEndIdx		= min(m_vectRankingList[i_byRankingScope].size()-1, i_nStartRanking + i_nReqSendCnt - 2);
	
	int nSentCnt	= 0;
	int nSendBytes	= 0;
	
	INIT_MSG_WITH_BUFFER(MSG_FC_INFO_WRK_GET_PVP_RANKER_LIST_OK, T_FC_INFO_WRK_GET_PVP_RANKER_LIST_OK, pRankList, SendBuf);
	pRankList->byStartRank	= nStartIdx + 1;
	pRankList->byRankerCnt	= 0;
	nSendBytes				= MSG_SIZE(MSG_FC_INFO_WRK_GET_PVP_RANKER_LIST_OK);
	SWRK_PVP *pLevel		= (SWRK_PVP*)(SendBuf + nSendBytes);
	for(int i = nStartIdx; i <= nEndIdx; i++)
	{
		if(nSendBytes + sizeof(SWRK_PVP) > SIZE_MAX_PACKET)
		{
			i_pToFISoc->SendAddData(SendBuf, nSendBytes);
			
			pRankList->byStartRank	= i + 1;
			pRankList->byRankerCnt	= 0;
			nSendBytes				= MSG_SIZE(MSG_FC_INFO_WRK_GET_PVP_RANKER_LIST_OK);
		}
		
		pLevel[pRankList->byRankerCnt]	= *(m_vectRankingList[i_byRankingScope][i]);
		pRankList->byRankerCnt++;
		nSendBytes += sizeof(SWRK_PVP);
		nSentCnt++;
	}
	
	if(0 < pRankList->byRankerCnt)
	{
		i_pToFISoc->SendAddData(SendBuf, nSendBytes);
	}
	
	return nSentCnt;
}
