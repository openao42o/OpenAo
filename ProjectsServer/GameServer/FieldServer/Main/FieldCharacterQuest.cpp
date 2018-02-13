// FieldCharacterQuest.cpp: implementation of the CFieldCharacterQuest class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FieldCharacterQuest.h"
#include "Quest.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFieldCharacterQuest::CFieldCharacterQuest(CharacterQuest *i_pCharacterQuest/*=NULL*/, CQuest *i_pQuestInfo/*=NULL*/)
{
	ResetFieldCharacterQuest(i_pCharacterQuest, i_pQuestInfo);
}

CFieldCharacterQuest::~CFieldCharacterQuest()
{
	ResetFieldCharacterQuest();
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			void CFieldCharacterQuest::ResetFieldCharacterQuest(CharacterQuest *i_pCharacterQuest/*=NULL*/, CQuest *i_pQuestInfo/*=NULL*/)
/// \brief		
/// \author		cmkwon
/// \date		2005-10-21 ~ 2005-10-21
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CFieldCharacterQuest::ResetFieldCharacterQuest(CharacterQuest *i_pCharacterQuest/*=NULL*/
													, CQuest *i_pQuestInfo/*=NULL*/)
{
	///////////////////////////////////////////////////////////////////////////////
	// 2005-10-21 by cmkwon,  초기화
	CharacterUniqueNumber	= 0;
	QuestIndex				= 0;
	QuestState				= QUEST_STATE_NONE;
	QuestParam1				= 0;
	QuestTimeStamp.Reset();
	QuestPlayTimeStamp		= 0;

	m_pQuestInfo			= NULL;
	m_vectQuestMonsterCount.clear();


	///////////////////////////////////////////////////////////////////////////////
	// 2005-10-21 by cmkwon, 인자가 유효할 경우
	if(i_pCharacterQuest)
	{
		CharacterUniqueNumber	= i_pCharacterQuest->CharacterUniqueNumber;
		QuestIndex				= i_pCharacterQuest->QuestIndex;
		QuestState				= i_pCharacterQuest->QuestState;
		QuestParam1				= i_pCharacterQuest->QuestParam1;
		QuestTimeStamp			= i_pCharacterQuest->QuestTimeStamp;
		QuestPlayTimeStamp		= i_pCharacterQuest->QuestPlayTimeStamp;

		m_pQuestInfo			= i_pQuestInfo;
	}
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CFieldCharacterQuest::IsEmptyMonsterCount(void)
/// \brief		
/// \author		cmkwon
/// \date		2005-10-25 ~ 2005-10-25
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CFieldCharacterQuest::IsEmptyMonsterCount(void)
{
	if(m_vectQuestMonsterCount.empty())
	{
		return TRUE;
	}

	return FALSE;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CFieldCharacterQuest::IsValidQuestInfomation(INT i_questIndex/*=0*/)
/// \brief		
/// \author		cmkwon
/// \date		2005-10-24 ~ 2005-10-24
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CFieldCharacterQuest::IsValidQuestInfomation(INT i_questIndex/*=0*/)
{
	if(NULL == m_pQuestInfo)
	{
		return FALSE;
	}

	if(i_questIndex
		&& i_questIndex != m_pQuestInfo->QuestIndex)
	{
		return FALSE;
	}

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			CharacterQuestMonsterCount *CFieldCharacterQuest::FindMonsterCountByMonsterUniqueNumber(INT i_monsterUniqueNumber)
/// \brief		
/// \author		cmkwon
/// \date		2005-10-24 ~ 2005-10-24
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
CharacterQuestMonsterCount *CFieldCharacterQuest::FindMonsterCountByMonsterUniqueNumber(INT i_monsterUniqueNumber)
{
	vectCharacterQuestMonsterCount::iterator itr(m_vectQuestMonsterCount.begin());
	for(; itr != m_vectQuestMonsterCount.end(); itr++)
	{
		if(itr->MonsterUniqueNumber == i_monsterUniqueNumber)
		{
			return &*itr;
		}
	}

	return NULL;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CFieldCharacterQuest::InsertMonsterCount(CharacterQuestMonsterCount *i_pMonCount)
/// \brief		
/// \author		cmkwon
/// \date		2005-10-24 ~ 2005-10-24
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CFieldCharacterQuest::InsertMonsterCount(DBCharacterQuestMonsterCount *i_pDBMonCount)
{
	if(FALSE == IsValidQuestInfomation(i_pDBMonCount->QuestIndex))
	{
		return FALSE;
	}

	if(FALSE == m_pQuestInfo->IsExistTargetMonsterVector(i_pDBMonCount->MonsterUniqueNumber))
	{
		return FALSE;
	}

	CharacterQuestMonsterCount *pFCharMon = FindMonsterCountByMonsterUniqueNumber(i_pDBMonCount->MonsterUniqueNumber);
	if(pFCharMon)
	{
		return FALSE;
	}
	
	CharacterQuestMonsterCount tmCharMonCount;
	tmCharMonCount.MonsterUniqueNumber	= i_pDBMonCount->MonsterUniqueNumber;
	tmCharMonCount.Count				= i_pDBMonCount->Count;
	m_vectQuestMonsterCount.push_back(tmCharMonCount);
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			CharacterQuestMonsterCount * CFieldCharacterQuest::AddMonsterCountByMonsterUniqueNumber(INT i_monsterUniqueNumber)
/// \brief		
/// \author		cmkwon
/// \date		2005-10-24 ~ 2005-10-24
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
CharacterQuestMonsterCount * CFieldCharacterQuest::AddMonsterCountByMonsterUniqueNumber(INT i_monsterUniqueNumber)
{
	if(FALSE == IsValidQuestInfomation())
	{
		return NULL;
	}

	if(FALSE == m_pQuestInfo->IsExistTargetMonsterVector(i_monsterUniqueNumber))
	{
		return NULL;
	}

	CharacterQuestMonsterCount *pFCharMon = FindMonsterCountByMonsterUniqueNumber(i_monsterUniqueNumber);
	if(pFCharMon)
	{
		pFCharMon->Count++;
		return pFCharMon;
	}
	
	CharacterQuestMonsterCount tmCharMonCount;
	tmCharMonCount.MonsterUniqueNumber	= i_monsterUniqueNumber;
	tmCharMonCount.Count				= 1;
	m_vectQuestMonsterCount.push_back(tmCharMonCount);
	return FindMonsterCountByMonsterUniqueNumber(i_monsterUniqueNumber);	
}
