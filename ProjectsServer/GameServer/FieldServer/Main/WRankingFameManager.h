// RankingFameManager.h: interface for the CWRankingFameManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RANKINGFAMEMANAGER_H__E004B188_6A17_4203_84A9_81619AB374EA__INCLUDED_)
#define AFX_RANKINGFAMEMANAGER_H__E004B188_6A17_4203_84A9_81619AB374EA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "WRankingFame.h"

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
class CWRankingFameManager  
{
public:
	CWRankingFameManager();
	~CWRankingFameManager();

	BOOL InitEachWRankingManager(mtvectSWRK_FAME_FOR_DB *i_pvectAllRankingList);
	void ClearEachWRankingManager(void);
	CWRankingFame *GetWRankingListByUnitMask(INT i_nUnitMask);
	
	int SendLevelRankerList(INT i_nUnitMask, BYTE i_byRankingScope, CFieldIOCPSocket *i_pToFISoc, int i_nStartRanking, int i_nReqSendCnt=SIZE_MAX_REQUEST_COUNT);
	
	mtvectSWRK_FAME		m_vectAllRankerList;
	CWRankingFame		m_arrWRankingofEachUnitKind[5];
};

#endif // !defined(AFX_RANKINGFAMEMANAGER_H__E004B188_6A17_4203_84A9_81619AB374EA__INCLUDED_)
