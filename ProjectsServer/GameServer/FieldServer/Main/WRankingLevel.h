// WRankingLevel.h: interface for the CWRankingLevel class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WRANKINGLEVEL_H__EEB39B0D_4569_4D9B_9234_71F1C7769B88__INCLUDED_)
#define AFX_WRANKINGLEVEL_H__EEB39B0D_4569_4D9B_9234_71F1C7769B88__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


///////////////////////////////////////////////////////////////////////////////
/// \class		
///
/// \brief		// 2009-02-12 by cmkwon, EP3-3 월드랭킹시스템 구현 - 
/// \author		cmkwon
/// \version	
/// \date		2009-02-18 ~ 2009-02-18
/// \warning	
///////////////////////////////////////////////////////////////////////////////
class CFieldIOCPSocket;
class CWRankingLevel  
{
public:
	CWRankingLevel();
	~CWRankingLevel();

	BOOL InitWRanking(int i_unitKindMask, mtvectSWRK_LEVEL *i_pvectAllRankingList);
	void ClearRanking(void);

	int SendRankerList(BYTE i_byRankingScope, CFieldIOCPSocket *i_pToFISoc, int i_nStartRanking, int i_nReqSendCnt=SIZE_MAX_REQUEST_COUNT);

	INT						m_UnitKind;				
	mtvectSWRK_LEVELPtr		m_vectRankingList[2];
};

#endif // !defined(AFX_WRANKINGLEVEL_H__EEB39B0D_4569_4D9B_9234_71F1C7769B88__INCLUDED_)
