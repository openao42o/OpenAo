
%{
#include <stdlib.h>
#include <string.h>
#include "AtumDataLoader.h"
#include "QuestLoader.cpp.h"
#undef YY_INPUT
//#define YY_INPUT(buf,result,max_size) \
//    { \
//    int c = getchar(); \
//    result = (c == EOF) ? YY_NULL : (buf[0] = c, 1); \
//    }

extern int g_line_number_quest;
%}

%%
quest					return QUEST_TOK;

	/* BASIC_INFO 관련 */
BASIC_INFO				{
							_loader_print("	BASIC_INFO_TOK\n");
							return BASIC_INFO_TOK;
						}
QUEST_INDEX				return QUEST_INDEX_TOK;
QUEST_NAME				return QUEST_NAME_TOK;
QUEST_TYPE				return QUEST_TYPE_TOK;
QUEST_START_TYPE		return QUEST_START_TYPE_TOK;
QUEST_END_TYPE			return QUEST_END_TYPE_TOK;
NPC_INDEX				return NPC_INDEX_TOK;
NPC_NAME				return NPC_NAME_TOK;
PRE_TALK				return PRE_TALK_TOK;
FAIL_TALK				return FAIL_TALK_TOK;
SUCCESS_TALK			return SUCCESS_TALK_TOK;
NEXT_QUEST_INDEX		return NEXT_QUEST_INDEX_TOK;

	/* START_REQ 관련 */
START_REQ				{
							_loader_print("	START_REQ_TOK\n");
							return START_REQ_TOK;
						}
S_REQ_UNIT_KIND			return S_REQ_UNIT_KIND_TOK;
S_REQ_LEVEL				return S_REQ_LEVEL_TOK;
S_REQ_ATTACK_PART		return S_REQ_ATTACK_PART_TOK;
S_REQ_ENDURANCE_PART	return S_REQ_ENDURANCE_PART_TOK;
S_REQ_FUEL_PART			return S_REQ_FUEL_PART_TOK;
S_REQ_SOUL_PART			return S_REQ_SOUL_PART_TOK;
S_REQ_DEFENSE_PART		return S_REQ_DEFENSE_PART_TOK;
S_REQ_DODGE_PART		return S_REQ_DODGE_PART_TOK;
S_REQ_ITEM				return S_REQ_ITEM_TOK;
S_REQ_QUEST_INDEX		return S_REQ_QUEST_INDEX_TOK;

	/* START_ACTION  관련 */
START_ACTION			{
							_loader_print("	START_ACTION_TOK\n");
							return START_ACTION_TOK;
						}
S_PAY_ITEM				return S_PAY_ITEM_TOK;
S_DEL_ITEM				return S_DEL_ITEM_TOK;

	/* COMPLETION_REQ 관련 */
COMPLETION_REQ			{
							_loader_print("	COMPLETION_REQ_TOK\n");
							return COMPLETION_REQ_TOK;
						}
C_REQ_MEET_NPC_INDEX	return C_REQ_MEET_NPC_INDEX_TOK;
C_REQ_ITEM				return C_REQ_ITEM_TOK;

	/* COMPLETION_ACTION 관련 */
COMPLETION_ACTION		{
							_loader_print("	COMPLETION_ACTION_TOK\n");
							return COMPLETION_ACTION_TOK;
						}
C_PAY_BONUS_STAT		return C_PAY_BONUS_STAT_TOK;
C_PAY_BONUS_SKILL_POINT	return C_PAY_BONUS_SKILL_POINT_TOK;
C_PAY_FAME				return C_PAY_FAME_TOK;
C_PAY_STATUS			return C_PAY_STATUS_TOK;
C_PAY_LEVEL				return C_PAY_LEVEL_TOK;
C_PAY_EXPERIENCE		return C_PAY_EXPERIENCE_TOK;
C_PAY_WARP_MAP_INDEX	return C_PAY_WARP_MAP_INDEX_TOK;
C_DEL_ITEM				return C_DEL_ITEM_TOK;
C_PAY_ITEM				return C_PAY_ITEM_TOK;

	/* 범용 Token */
FROM					return FROM_TOK;
MONSTER					return MONSTER_TOK;
ALL						return ALL_TOK;

">"						return GT_TOK;
"<"						return LT_TOK;
">="					return GE_TOK;
"<="					return LE_TOK;
"=="					return EQUAL_TOK;
\$[\_a-zA-Z]+			{
							QuestLoaderlval.string = strdup(QuestLoadertext);
							_loader_print("	USER_VARIABLE_TOK: %s\n", QuestLoaderlval.string);
							return USER_VARIABLE_TOK;
						}
[\+-]?[0-9]+			{
							QuestLoaderlval.integer = atoi(QuestLoadertext);
							_loader_print("	INTEGER_TOK: %d\n", QuestLoaderlval.integer);
							return INTEGER_TOK;
						}
[a-zA-Z][\_a-zA-Z0-9]*	{
							QuestLoaderlval.string = strdup(QuestLoadertext);
							_loader_print("	WORD_TOK: %s\n", QuestLoaderlval.string);
							return WORD_TOK;
						}
'([^'\n]|'')*'			{
							QuestLoaderlval.string = ++QuestLoadertext;
							QuestLoaderlval.string[strlen(QuestLoaderlval.string)-1] = '\0';
							QuestLoaderlval.string = strdup(QuestLoaderlval.string);
							_loader_print("	QSTRING_TOK: %s\n", QuestLoaderlval.string);
							return QSTRING_TOK;
						}
\n                      { /* ignore EOL */;	g_line_number_quest++; }
#.*$					{ /* ignore comment */; /*_loader_print("	comment line %d\n", g_line_number_quest);*/ }
[ \t]+					/* ignore whitespace */;
.						return QuestLoadertext[0];
%%
