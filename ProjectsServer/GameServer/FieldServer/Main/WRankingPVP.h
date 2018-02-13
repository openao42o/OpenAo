// RankingPVP.h: interface for the CWRankingPVP class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RANKINGPVP_H__23935829_1A0F_417B_853F_DFDF447DBED0__INCLUDED_)
#define AFX_RANKINGPVP_H__23935829_1A0F_417B_853F_DFDF447DBED0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CFieldIOCPSocket;
class CWRankingPVP  
{
public:
	CWRankingPVP();
	~CWRankingPVP();

	BOOL InitWRanking(int i_unitKindMask, mtvectSWRK_PVP *i_pvectAllRankingList);
	void ClearRanking(void);
	
	int SendRankerList(BYTE i_byRankingScope, CFieldIOCPSocket *i_pToFISoc, int i_nStartRanking, int i_nReqSendCnt=SIZE_MAX_REQUEST_COUNT);
	
	INT						m_UnitKind;				
	mtvectSWRK_PVPPtr		m_vectRankingList[2];
};

#endif // !defined(AFX_RANKINGPVP_H__23935829_1A0F_417B_853F_DFDF447DBED0__INCLUDED_)
