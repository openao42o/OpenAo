// FieldCharacterQuest.h: interface for the CFieldCharacterQuest class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FIELDCHARACTERQUEST_H__14FEDEE8_53E9_467C_8A61_8E95062CD2E7__INCLUDED_)
#define AFX_FIELDCHARACTERQUEST_H__14FEDEE8_53E9_467C_8A61_8E95062CD2E7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Quest.h"

class CQuest;
class CFieldCharacterQuest : public CharacterQuest  
{
public:
	friend class CFieldIOCPSocket;

public:
	CFieldCharacterQuest(CharacterQuest *i_pCharacterQuest=NULL, CQuest *i_pQuestInfo=NULL);
	~CFieldCharacterQuest();

	CQuest* GetQuestInfo() const { return m_pQuestInfo; }

	void ResetFieldCharacterQuest(CharacterQuest *i_pCharacterQuest=NULL, CQuest *i_pQuestInfo=NULL);

	BOOL IsEmptyMonsterCount(void);
	BOOL IsValidQuestInfomation(INT i_questIndex=0);
	CharacterQuestMonsterCount *FindMonsterCountByMonsterUniqueNumber(INT i_monsterUniqueNumber);
	BOOL InsertMonsterCount(DBCharacterQuestMonsterCount *i_pDBMonCount);
	CharacterQuestMonsterCount *AddMonsterCountByMonsterUniqueNumber(INT i_monsterUniqueNumber);
protected:
	CQuest							*m_pQuestInfo;
	vectCharacterQuestMonsterCount	m_vectQuestMonsterCount;
};

#endif // !defined(AFX_FIELDCHARACTERQUEST_H__14FEDEE8_53E9_467C_8A61_8E95062CD2E7__INCLUDED_)
