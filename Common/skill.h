/*

#ifndef __ATUM_SKILL_H__
#define __ATUM_SKILL_H__

#include "NPCScripts.h"
#include "AtumParam.h"
#include "AtumTime.h"
#include <list>
#include <vector>

struct MapPosition {
	SERVER_ID		FieldServerID;
	char			*MapName[SIZE_MAX_MAP_NAME];
	D3DXVECTOR3		Position;
};

// Quest Element의 종류
typedef enum
{
	MOVE = 0,
	TRANSPORT_ITEM,
	INTERRUPT_TRANSPORTATION
} QE_TYPE;

struct QUEST_ELEMENT {
	UID32_t			ElementUniqueNumber;
	QE_TYPE			Type;
};

struct QUEST_ELEMENT_MOVE : QUEST_ELEMENT {
	int				TimeLimit;
	NPCID_t			DestNpcID;
};

struct QUEST_ELEMENT_TRANSPORT_ITEM {
	AtumTime		StartTime;
	int				TimeLimit;
	//MapPosition		StartPosition;
	//MapPosition		DestinationPosition;
	NPCID_t			DestNpcID;
	ITEM			*pMoveItem;
};

struct QUEST_ELEMENT_INTERRUPT_TRANSPORTATION {
	AtumTime		StartTime;
	int				TimeLimit;
	//MapPosition		StartPosition;
	//MapPosition		DestinationPosition;
	ITEM			*pMoveItem;
};

struct QUEST {
	UID32_t					QuestUniqueNumber;
	vector<QUEST_ELEMENT*>	VectorQuestElements;
	int						NumOfEvents;	// ASSERT(vectorQuestElements.size() == NumOfEvents)
};

typedef enum
{
	QS_RECEIVED		= 0,
	QS_EXECUTING	= 1,
	QS_COMPLETED	= 2
} QUEST_STATUS;

// 사용자의 quest 완료 여부를 저장하기 위한 구조체
struct USER_QUEST
{
	UID32_t			QuestUniqueNumber;
	QUEST_STATUS	QuestStatus;
	int				ProcessStep;		// 진행 단계
};

// 스킬의 습득 조건을 위한 구조체
struct SKILL_REQUIREMENT
{
	ITEM_SKILL		*pItemSkill;
	int				RequiredLevel;
	list<QUEST*>	ListRequiredQuests;
	list<ITEM*>		ListRequiredItems;		// 나노카드류
};

struct SKILL_TIME_LIMIT
{
	ITEM_SKILL		*pItemSkill;
	int				StartTimeStamp;			// milli-seconds
};

#endif	// __ATUM_SKILL_H__
*/
