// QuestMatching.h: interface for the CQuestMatching class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_QUESTMATCHING_H__78E1A339_B7F6_4662_AD63_34733BD966BF__INCLUDED_)
#define AFX_QUESTMATCHING_H__78E1A339_B7F6_4662_AD63_34733BD966BF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


///////////////////////////////////////////////////////////////////////////////
/// \class		CQuestMatching
///
/// \brief		// 2009-03-31 by cmkwon, 세력초기화 시스템 구현 - 
/// \author		cmkwon
/// \version	
/// \date		2009-03-31 ~ 2009-03-31
/// \warning	
///////////////////////////////////////////////////////////////////////////////
class CFieldIOCPSocket;
class CODBCStatement;
class CFieldIOCP;
class CQuestMatching
{
public:
	CQuestMatching();
	~CQuestMatching();

	BOOL LoadQuestMatchingAndItemMatching(CODBCStatement *i_pODBCStmt);
	BOOL CheckQuestMatchingAndItemMatching(CFieldIOCP *i_pFIOCP);
	BOOL IsEnableSelectInfluence(CFieldIOCPSocket *i_pFISoc);
	BOOL IsHaveInfluenceQuest(BYTE i_byInflTy, CFieldIOCPSocket *i_pFISoc);
	BOOL IsChangeInfluence(BYTE i_byTargetInfluTy, CFieldIOCPSocket *i_pFISoc, vectSQUEST_MATCH_CHANGE *o_pQuestMatchList, vectSITEM_MATCH_CHANGE *o_pItemMatchList);
	

	vectSQUEST_MATCHING		m_vectQuestMatchingList;		// 2009-03-31 by cmkwon, 세력초기화 시스템 구현 - 
	vectSITEM_MATCHING		m_vectItemMatchingList;			// 2009-03-31 by cmkwon, 세력초기화 시스템 구현 - 
};

#endif // !defined(AFX_QUESTMATCHING_H__78E1A339_B7F6_4662_AD63_34733BD966BF__INCLUDED_)
