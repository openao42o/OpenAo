// QuestMatching.cpp: implementation of the CQuestMatching class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "QuestMatching.h"
#include "FieldIOCPSocket.h"
#include "FieldGlobal.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2009-03-31 by cmkwon, 세력초기화 시스템 구현 - 
/// \author		cmkwon
/// \date		2009-04-01 ~ 2009-04-01
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
CQuestMatching::CQuestMatching()
{

}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2009-03-31 by cmkwon, 세력초기화 시스템 구현 - 
/// \author		cmkwon
/// \date		2009-04-01 ~ 2009-04-01
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
CQuestMatching::~CQuestMatching()
{

}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2009-03-31 by cmkwon, 세력초기화 시스템 구현 - 
/// \author		cmkwon
/// \date		2009-04-01 ~ 2009-04-01
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CQuestMatching::LoadQuestMatchingAndItemMatching(CODBCStatement *i_pODBCStmt)
{
	m_vectQuestMatchingList.clear();

	/************************************************************************
	--------------------------------------------------------------------------------
	-- PROCEDURE NAME	: atum_Load_QuestMaching
	-- DESC				: // 2009-03-31 by cmkwon, 세력초기화 시스템 구현 - 
	--						
	--------------------------------------------------------------------------------
	CREATE PROCEDURE dbo.atum_Load_QuestMatching
	AS
		SELECT QuestIndexForBCU, QuestIndexForANI
		FROM atum2_db_account.dbo.ti_QuestMatching WITH(NOLOCK)
	GO
	************************************************************************/
	RETCODE	ret = SQLExecDirect(i_pODBCStmt->m_hstmt, PROCEDURE_090401_0342, SQL_NTS);
	if(SQL_SUCCESS != ret && SQL_SUCCESS_WITH_INFO != ret && SQL_NO_DATA != ret)
	{
		i_pODBCStmt->ProcessLogMessagesForStmt(PROCEDURE_090401_0342);
		i_pODBCStmt->FreeStatement();
		return FALSE;
	}

	SQUEST_MATCHING tmQuestMatching;
	SQLINTEGER	arrCB[3] = {SQL_NTS, SQL_NTS, SQL_NTS};
	SQLBindCol(i_pODBCStmt->m_hstmt, 1, SQL_C_LONG, &tmQuestMatching.QuestIndexForBCU, 0,			&arrCB[1]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 2, SQL_C_LONG, &tmQuestMatching.QuestIndexForANI, 0,			&arrCB[2]);
	while (TRUE)
	{
		util::zero(&tmQuestMatching, sizeof(tmQuestMatching));
		ret = SQLFetch(i_pODBCStmt->m_hstmt);
		if(SQL_SUCCESS != ret && SQL_SUCCESS_WITH_INFO != ret)
		{
			break;
		}
		m_vectQuestMatchingList.push_back(tmQuestMatching);
	}
	i_pODBCStmt->FreeStatement();


	m_vectItemMatchingList.clear();

	/************************************************************************
	--------------------------------------------------------------------------------
	-- PROCEDURE NAME	: atum_Load_ItemMaching
	-- DESC				: // 2009-03-31 by cmkwon, 세력초기화 시스템 구현 - 
	--						
	--------------------------------------------------------------------------------
	CREATE PROCEDURE dbo.atum_Load_ItemMatching
	AS
		SELECT ItemNumForBCU, ItemNumForANI
		FROM atum2_db_account.dbo.ti_ItemMatching WITH(NOLOCK)
	GO
	************************************************************************/
	ret = SQLExecDirect(i_pODBCStmt->m_hstmt, PROCEDURE_090401_0343, SQL_NTS);
	if(SQL_SUCCESS != ret && SQL_SUCCESS_WITH_INFO != ret && SQL_NO_DATA != ret)
	{
		i_pODBCStmt->ProcessLogMessagesForStmt(PROCEDURE_090401_0343);
		i_pODBCStmt->FreeStatement();
		return FALSE;
	}

	SITEM_MATCHING	tmItemMatching;
	arrCB[1] = arrCB[2] = SQL_NTS;
	SQLBindCol(i_pODBCStmt->m_hstmt, 1, SQL_C_LONG, &tmItemMatching.ItemNumForBCU, 0,			&arrCB[1]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 2, SQL_C_LONG, &tmItemMatching.ItemNumForANI, 0,			&arrCB[2]);
	while (TRUE)
	{
		util::zero(&tmItemMatching, sizeof(tmItemMatching));
		ret = SQLFetch(i_pODBCStmt->m_hstmt);
		if(SQL_SUCCESS != ret && SQL_SUCCESS_WITH_INFO != ret)
		{
			break;
		}
		m_vectItemMatchingList.push_back(tmItemMatching);
	}
	i_pODBCStmt->FreeStatement();
	
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2009-03-31 by cmkwon, 세력초기화 시스템 구현 - 
/// \author		cmkwon
/// \date		2009-04-02 ~ 2009-04-02
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CQuestMatching::CheckQuestMatchingAndItemMatching(CFieldIOCP *i_pFIOCP)
{
	BOOL bWillRet = TRUE;
	vectSQUEST_MATCHING::iterator	itr(m_vectQuestMatchingList.begin());
	for(; itr != m_vectQuestMatchingList.end(); itr++)
	{
		SQUEST_MATCHING *pQuestMatching = &*itr;
		if(pQuestMatching->QuestIndexForBCU == pQuestMatching->QuestIndexForANI)
		{
			bWillRet = FALSE;
			g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ERROR] CQuestMatching::CheckQuestMatchingAndItemMatching# BCUQuestIndex(%d) ANIQuestIndex(%d), same both QuestIndex \r\n"
				, pQuestMatching->QuestIndexForBCU, pQuestMatching->QuestIndexForANI);
		}

		if(0 != pQuestMatching->QuestIndexForBCU)
		{
			CQuest *pQuestInfo = i_pFIOCP->GetQuestInfo(pQuestMatching->QuestIndexForBCU);
			if(NULL == pQuestInfo)
			{
				bWillRet = FALSE;
				g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ERROR] CQuestMatching::CheckQuestMatchingAndItemMatching# BCUQuestIndex(%d) ANIQuestIndex(%d), don't have BCUQuestInfo \r\n"
					, pQuestMatching->QuestIndexForBCU, pQuestMatching->QuestIndexForANI);
			}
			else if(INFLUENCE_TYPE_VCN != pQuestInfo->ReqInfluenceType)
			{
				bWillRet = FALSE;
				g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ERROR] CQuestMatching::CheckQuestMatchingAndItemMatching# BCUQuestIndex(%d) ANIQuestIndex(%d), BCUQuest influence type not match, QuestInfluenceTypeMask(%d) \r\n"
					, pQuestMatching->QuestIndexForBCU, pQuestMatching->QuestIndexForANI, pQuestInfo->ReqInfluenceType);
			}
		}
		if(0 != pQuestMatching->QuestIndexForANI)
		{
			CQuest *pQuestInfo = i_pFIOCP->GetQuestInfo(pQuestMatching->QuestIndexForANI);
			if(NULL == pQuestInfo)
			{
				bWillRet = FALSE;
				g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ERROR] CQuestMatching::CheckQuestMatchingAndItemMatching# BCUQuestIndex(%d) ANIQuestIndex(%d), don't have ANIQuestInfo \r\n"
					, pQuestMatching->QuestIndexForBCU, pQuestMatching->QuestIndexForANI);
			}
			else if(INFLUENCE_TYPE_ANI != pQuestInfo->ReqInfluenceType)
			{
				bWillRet = FALSE;
				g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ERROR] CQuestMatching::CheckQuestMatchingAndItemMatching# BCUQuestIndex(%d) ANIQuestIndex(%d), ANIQuest influece type not match, QuestInfluenceTypeMask(%d) \r\n"
					, pQuestMatching->QuestIndexForBCU, pQuestMatching->QuestIndexForANI, pQuestInfo->ReqInfluenceType);
			}
		}
	}

	vectSITEM_MATCHING::iterator	itrIM(m_vectItemMatchingList.begin());
	for(; itrIM != m_vectItemMatchingList.end(); itrIM++)
	{
		SITEM_MATCHING *pItemMatch = &*itrIM;
		if(NULL == i_pFIOCP->GetItemInfo(pItemMatch->ItemNumForBCU))
		{
			bWillRet	= FALSE;
			g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ERROR] CQuestMatching::CheckQuestMatchingAndItemMatching# BCUItemNum(%d) ANIItemNum(%d), don't have BCUItemInfo \r\n"
				, pItemMatch->ItemNumForBCU, pItemMatch->ItemNumForANI);
		}
		if(NULL == i_pFIOCP->GetItemInfo(pItemMatch->ItemNumForANI))
		{
			bWillRet	= FALSE;
			g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ERROR] CQuestMatching::CheckQuestMatchingAndItemMatching# BCUItemNum(%d) ANIItemNum(%d), don't have ANIItemInfo \r\n"
				, pItemMatch->ItemNumForBCU, pItemMatch->ItemNumForANI);
		}
	}

	return bWillRet;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2009-03-31 by cmkwon, 세력초기화 시스템 구현 - 
/// \author		cmkwon
/// \date		2009-04-01 ~ 2009-04-01
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CQuestMatching::IsEnableSelectInfluence(CFieldIOCPSocket *i_pFISoc)
{
	if(IsHaveInfluenceQuest(INFLUENCE_TYPE_VCN, i_pFISoc)
		&& IsHaveInfluenceQuest(INFLUENCE_TYPE_ANI, i_pFISoc))
	{// 2009-04-01 by cmkwon, 두 세력 퀘스트를 모두 가지고 있다
		return FALSE;
	}

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2009-03-31 by cmkwon, 세력초기화 시스템 구현 - 
/// \author		cmkwon
/// \date		2009-04-01 ~ 2009-04-01
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CQuestMatching::IsHaveInfluenceQuest(BYTE i_byInflTy, CFieldIOCPSocket *i_pFISoc)
{
	if(FALSE == i_pFISoc->IsValidCharacter(FALSE))
	{
		return FALSE;
	}

	if(INFLUENCE_TYPE_VCN == i_byInflTy)
	{
		vectSQUEST_MATCHING::iterator	itr(m_vectQuestMatchingList.begin());
		for(; itr != m_vectQuestMatchingList.end(); itr++)
		{
			SQUEST_MATCHING *pQuestMatching = &*itr;
			if(0 != pQuestMatching->QuestIndexForBCU
				&& i_pFISoc->GetFieldCharacterQuestPtrByQuestIndex(pQuestMatching->QuestIndexForBCU))
			{
				return TRUE;
			}
		}
	}
	else if(INFLUENCE_TYPE_ANI == i_byInflTy)
	{
		vectSQUEST_MATCHING::iterator	itr(m_vectQuestMatchingList.begin());
		for(; itr != m_vectQuestMatchingList.end(); itr++)
		{
			SQUEST_MATCHING *pQuestMatching = &*itr;
			if(0 != pQuestMatching->QuestIndexForANI
				&& i_pFISoc->GetFieldCharacterQuestPtrByQuestIndex(pQuestMatching->QuestIndexForANI))
			{
				return TRUE;
			}
		}
	}
	else
	{
		return FALSE;
	}

	return FALSE;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2009-03-31 by cmkwon, 세력초기화 시스템 구현 - 
/// \author		cmkwon
/// \date		2009-04-01 ~ 2009-04-01
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CQuestMatching::IsChangeInfluence(BYTE i_byTargetInfluTy, CFieldIOCPSocket *i_pFISoc, vectSQUEST_MATCH_CHANGE *o_pQuestMatchList, vectSITEM_MATCH_CHANGE *o_pItemMatchList)
{
	o_pQuestMatchList->clear();
	o_pQuestMatchList->reserve(m_vectQuestMatchingList.size());
	o_pItemMatchList->reserve(m_vectItemMatchingList.size());

	if(FALSE == this->IsEnableSelectInfluence(i_pFISoc))
	{
		return FALSE;
	}

	if(i_byTargetInfluTy == INFLUENCE_TYPE_VCN)
	{
		if(this->IsHaveInfluenceQuest(INFLUENCE_TYPE_ANI, i_pFISoc))
		{
			vectSQUEST_MATCHING::iterator	itr(m_vectQuestMatchingList.begin());
			for(; itr != m_vectQuestMatchingList.end(); itr++)
			{
				SQUEST_MATCHING *pQuestMatching = &*itr;
				
				// change influence from ANI to BCU
				SQUEST_MATCH_CHANGE tmQMatch;
				tmQMatch.FromQuestIndex			= pQuestMatching->QuestIndexForANI;
				tmQMatch.ToQuestIndex			= pQuestMatching->QuestIndexForBCU;
				o_pQuestMatchList->push_back(tmQMatch);
			}

			vectSITEM_MATCHING::iterator	itrIM(m_vectItemMatchingList.begin());
			for(; itrIM != m_vectItemMatchingList.end(); itrIM++)
			{
				SITEM_MATCH_CHANGE tmIMatch;
				tmIMatch.FromItemNum			= itrIM->ItemNumForANI;
				tmIMatch.ToItemNum				= itrIM->ItemNumForBCU;
				o_pItemMatchList->push_back(tmIMatch);
			}
			return TRUE;
		}
	}
	else if(i_byTargetInfluTy == INFLUENCE_TYPE_ANI)
	{
		if(this->IsHaveInfluenceQuest(INFLUENCE_TYPE_VCN, i_pFISoc))
		{
			vectSQUEST_MATCHING::iterator	itr(m_vectQuestMatchingList.begin());
			for(; itr != m_vectQuestMatchingList.end(); itr++)
			{
				SQUEST_MATCHING *pQuestMatching = &*itr;
				
				// change influence from BUC to ANI
				SQUEST_MATCH_CHANGE tmQMatch;				
				tmQMatch.FromQuestIndex			= pQuestMatching->QuestIndexForBCU;
				tmQMatch.ToQuestIndex			= pQuestMatching->QuestIndexForANI;
				o_pQuestMatchList->push_back(tmQMatch);
			}

			vectSITEM_MATCHING::iterator	itrIM(m_vectItemMatchingList.begin());
			for(; itrIM != m_vectItemMatchingList.end(); itrIM++)
			{
				SITEM_MATCH_CHANGE tmIMatch;
				tmIMatch.FromItemNum			= itrIM->ItemNumForBCU;
				tmIMatch.ToItemNum				= itrIM->ItemNumForANI;
				o_pItemMatchList->push_back(tmIMatch);
			}
			return TRUE;
		}
	}

	return FALSE;
}