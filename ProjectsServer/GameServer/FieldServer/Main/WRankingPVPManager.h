// RankingPVPManager.h: interface for the CWRankingPVPManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RANKINGPVPMANAGER_H__11686A71_CB69_404A_AD02_8757000ED08A__INCLUDED_)
#define AFX_RANKINGPVPMANAGER_H__11686A71_CB69_404A_AD02_8757000ED08A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "WRankingPVP.h"

///////////////////////////////////////////////////////////////////////////////
/// \class		
///
/// \brief		
/// \author		cmkwon
/// \version	
/// \date		2009-02-18 ~ 2009-02-18
/// \warning	
///////////////////////////////////////////////////////////////////////////////
class CFieldIOCPSocket;
class CWRankingPVPManager  
{
public:
	CWRankingPVPManager();
	~CWRankingPVPManager();

	BOOL InitEachWRankingManager(mtvectSWRK_PVP_FOR_DB *i_pvectAllRankingList);
	void ClearEachWRankingManager(void);
	CWRankingPVP *GetWRankingListByUnitMask(INT i_nUnitMask);
	
	int SendLevelRankerList(INT i_nUnitMask, BYTE i_byRankingScope, CFieldIOCPSocket *i_pToFISoc, int i_nStartRanking, int i_nReqSendCnt=SIZE_MAX_REQUEST_COUNT);
	
	mtvectSWRK_PVP		m_vectAllRankerList;
	CWRankingPVP		m_arrWRankingofEachUnitKind[5];

};

#endif // !defined(AFX_RANKINGPVPMANAGER_H__11686A71_CB69_404A_AD02_8757000ED08A__INCLUDED_)
