// WRankingLevelManager.h: interface for the CWRankingLevelManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WRANKINGLEVELMANAGER_H__605A0563_CAFC_4870_A8C6_4C0A2EBBD7ED__INCLUDED_)
#define AFX_WRANKINGLEVELMANAGER_H__605A0563_CAFC_4870_A8C6_4C0A2EBBD7ED__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "WRankingLevel.h"

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
class CFieldIOCPSocket;
class CWRankingLevelManager  
{
public:
	CWRankingLevelManager();
	~CWRankingLevelManager();

	BOOL InitEachWRankingManager(mtvectSWRK_LEVEL_FOR_DB *i_pvectAllRankingList);
	void ClearEachWRankingManager(void);
	CWRankingLevel *GetWRankingListByUnitMask(INT i_nUnitMask);

	int SendLevelRankerList(INT i_nUnitMask, BYTE i_byRankingScope, CFieldIOCPSocket *i_pToFISoc, int i_nStartRanking, int i_nReqSendCnt=SIZE_MAX_REQUEST_COUNT);

	mtvectSWRK_LEVEL	m_vectAllRankerList;
	CWRankingLevel		m_arrWRankingofEachUnitKind[5];
};

#endif // !defined(AFX_WRANKINGLEVELMANAGER_H__605A0563_CAFC_4870_A8C6_4C0A2EBBD7ED__INCLUDED_)
