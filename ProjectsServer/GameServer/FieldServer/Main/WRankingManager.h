// WRankingManager.h: interface for the CWRankingManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WRANKINGMANAGER_H__7D411396_CA9C_415D_A147_5B43FB004164__INCLUDED_)
#define AFX_WRANKINGMANAGER_H__7D411396_CA9C_415D_A147_5B43FB004164__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "WRankingLevelManager.h"
#include "WRankingFameManager.h"
#include "WRankingPvpManager.h"
#include "WRankingServiceManager.h"

class CWRankingManager  
{
public:
	CWRankingManager();
	~CWRankingManager();
	
	int SendServiceListW(CFieldIOCPSocket *i_pToFISoc);
	int SendRankerListByRankingType(BYTE i_byRankingTy, INT i_nUnitMask, BYTE i_byRankingScope, CFieldIOCPSocket *i_pToFISoc, int i_nStartRanking, int i_nReqSendCnt=SIZE_MAX_REQUEST_COUNT);
	
	CWRankingLevelManager		m_LevelRankManager;
	CWRankingFameManager		m_FameRankManager;
	CWRankingPVPManager			m_PVPRankManager;
	CWRankingServiceManager		m_ServiceManager;
};

#endif // !defined(AFX_WRANKINGMANAGER_H__7D411396_CA9C_415D_A147_5B43FB004164__INCLUDED_)
