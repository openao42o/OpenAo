// RankingFame.h: interface for the CWRankingFame class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RANKINGFAME_H__D799E51E_824C_419B_8FB3_9EE245F69CA3__INCLUDED_)
#define AFX_RANKINGFAME_H__D799E51E_824C_419B_8FB3_9EE245F69CA3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CFieldIOCPSocket;
class CWRankingFame  
{
public:
	CWRankingFame();
	~CWRankingFame();

	BOOL InitWRanking(int i_unitKindMask, mtvectSWRK_FAME *i_pvectAllRankingList);
	void ClearRanking(void);
	
	int SendRankerList(BYTE i_byRankingScope, CFieldIOCPSocket *i_pToFISoc, int i_nStartRanking, int i_nReqSendCnt=SIZE_MAX_REQUEST_COUNT);
	
	INT						m_UnitKind;				
	mtvectSWRK_FAMEPtr		m_vectRankingList[2];
};

#endif // !defined(AFX_RANKINGFAME_H__D799E51E_824C_419B_8FB3_9EE245F69CA3__INCLUDED_)
