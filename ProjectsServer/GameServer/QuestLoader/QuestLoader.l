
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

///////////////////////////////////////////////////////////////////////////////
// 2006-04-18 by cmkwon, Aposterophe(''')가 두개이면 하나를 삭제하는 함수
void DeleteTwoAposterophe(char *io_szString)
{
	int nLen = strlen(io_szString);
	int nInputIndex = 0;
	for(int i=0; i < nLen; i++,nInputIndex++)
	{
		if('\'' == io_szString[i]
			&& i+1 < nLen
			&& '\'' == io_szString[i+1])
		{
			io_szString[nInputIndex] = io_szString[i];
			i++;
			continue;
		}
		io_szString[nInputIndex] = io_szString[i];
	}
	io_szString[nInputIndex] = '\0';
}

%}

%%
quest					return QUEST_TOK;

	/* BASIC_INFO 관련 */
BASIC_INFO				{
							_loader_print("	BASIC_INFO_TOK\n");
							return BASIC_INFO_TOK;
						}
QUEST_INDEX								return QUEST_INDEX_TOK;
QUEST_NAME								return QUEST_NAME_TOK;
QUEST_EPISODE_TYPE						return QUEST_EPISODE_TYPE_TOK;	// 2008-06-16 by cmkwon, EP3 퀘스트 기존 퀘스트와 구별 - 
QUEST_PARTYTYPE						    return QUEST_PARTYTYPE_TOK;	// 2008-12-02 by dhjin, 미션마스터
QUEST_KIND								return QUEST_KIND_TOK;
QUEST_TYPE								return QUEST_TYPE_TOK;
QUEST_START_TYPE						return QUEST_START_TYPE_TOK;
QUEST_END_TYPE							return QUEST_END_TYPE_TOK;
QUEST_PARTY_MEMBERS						return QUEST_PARTY_MEMBERS_TOK;
QUEST_NPC_INFO							return QUEST_NPC_INFO_TOK;
NPC_INDEX								return NPC_INDEX_TOK;
NPC_NAME								return NPC_NAME_TOK;
PRE_TALK								return PRE_TALK_TOK;
MID_TALK								return MID_TALK_TOK;
FAIL_TALK								return FAIL_TALK_TOK;
SUCCESS_TALK							return SUCCESS_TALK_TOK;
DESCRIPTION								return DESCRIPTION_TOK;
EVENT_MAP								return EVENT_MAP_TOK;
NEXT_QUEST_INDEX						return NEXT_QUEST_INDEX_TOK;
PARTY_QUEST_MEMBER_COUNT				return PARTY_QUEST_MEMBER_COUNT_TOK;
DROP_ITEM_TO_ALL_ATTACK_CHARACTER		return DROP_ITEM_TO_ALL_ATTACK_CHARACTER_TOK;
TIME_LIMIT_IN_MINUTES					return TIME_LIMIT_IN_MINUTES_TOK;
DISCARDABLE_QUEST						return DISCARDABLE_QUEST_TOK;
BI_QUEST_ATTRI_IS_CITYWAR				return BI_QUEST_ATTRI_IS_CITYWAR_TOK;
BI_QUEST_ATTRI_NEVER_COMPLETION_BY_USER	return BI_QUEST_ATTRI_NEVER_COMPLETION_BY_USER_TOK;
DROP_ITEM_TO_ALL_PARTY_MEMBERS			return DROP_ITEM_TO_ALL_PARTY_MEMBERS_TOK;	// 2009-01-13 by cmkwon, 미션 보상을 파티원 모두에게 주기 명령어 추가 - 

	/* START_REQ 관련 */
START_REQ				{
							_loader_print("	START_REQ_TOK\n");
							return START_REQ_TOK;
						}
S_REQ_UNIT_KIND						return S_REQ_UNIT_KIND_TOK;
S_REQ_LEVEL							return S_REQ_LEVEL_TOK;
S_REQ_ATTACK_PART					return S_REQ_ATTACK_PART_TOK;
S_REQ_DEFENSE_PART					return S_REQ_DEFENSE_PART_TOK;
S_REQ_FUEL_PART						return S_REQ_FUEL_PART_TOK;
S_REQ_SOUL_PART						return S_REQ_SOUL_PART_TOK;
S_REQ_SHIELD_PART					return S_REQ_SHIELD_PART_TOK;
S_REQ_DODGE_PART					return S_REQ_DODGE_PART_TOK;
S_REQ_ITEM							return S_REQ_ITEM_TOK;
S_REQ_QUEST_INDEX					return S_REQ_QUEST_INDEX_TOK;
S_REQ_RECV_QUEST_INDEX				return S_REQ_RECV_QUEST_INDEX_TOK;
S_REQ_GUILD							return S_REQ_GUILD_TOK;
S_REQ_INFLUENCE_TYPE				return S_REQ_INFLUENCE_TYPE_TOK;
S_REQ_START_AREA					return S_REQ_START_AREA_TOK;

	/* START_ACTION  관련 */
START_ACTION			{
							_loader_print("	START_ACTION_TOK\n");
							return START_ACTION_TOK;
						}
S_PAY_ITEM							return S_PAY_ITEM_TOK;
S_DEL_ITEM							return S_DEL_ITEM_TOK;
S_START_MAP_INDEX					return S_START_MAP_INDEX_TOK;
S_START_PARTY_WARP_FLAG				return S_START_PARTY_WARP_FLAG_TOK;

	/* COMPLETION_REQ 관련 */
COMPLETION_REQ			{
							_loader_print("	COMPLETION_REQ_TOK\n");
							return COMPLETION_REQ_TOK;
						}
C_REQ_MEET_NPC_INFO					return C_REQ_MEET_NPC_INFO_TOK;
C_REQ_ITEM							return C_REQ_ITEM_TOK;
C_REQ_MEET_OBJ_INDEX				return C_REQ_MEET_OBJ_INDEX_TOK;
C_REQ_DEAD_BY_MONSTER				return C_REQ_DEAD_BY_MONSTER_TOK;
C_REQ_MEET_MAP_AREA					return C_REQ_MEET_MAP_AREA_TOK;
C_REQ_MONSTER						return C_REQ_MONSTER_TOK;
C_REQ_MIX_ITEM_TARGET				return C_REQ_MIX_ITEM_TARGET_TOK;

	/* COMPLETION_ACTION 관련 */
COMPLETION_ACTION		{
							_loader_print("	COMPLETION_ACTION_TOK\n");
							return COMPLETION_ACTION_TOK;
						}
C_PAY_BONUS_STAT					return C_PAY_BONUS_STAT_TOK;
C_PAY_BONUS_SKILL_POINT				return C_PAY_BONUS_SKILL_POINT_TOK;
C_PAY_FAME							return C_PAY_FAME_TOK;
C_PAY_STATUS						return C_PAY_STATUS_TOK;
C_PAY_LEVEL							return C_PAY_LEVEL_TOK;
C_PAY_EXPERIENCE					return C_PAY_EXPERIENCE_TOK;
C_PAY_WARP_MAP_INDEX				return C_PAY_WARP_MAP_INDEX_TOK;
C_DEL_ITEM							return C_DEL_ITEM_TOK;
C_PAY_ITEM							return C_PAY_ITEM_TOK;   
C_PAY_ITEM_MISSIONMASTER			return C_PAY_ITEM_MISSIONMASTER_TOK;	// 2008-12-02 by dhjin, 미션마스터
C_RANDOM_PAY_ITEM					return C_RANDOM_PAY_ITEM_TOK;
C_MIX_ITEM							return C_MIX_ITEM_TOK;
C_PAY_PROPENSITY					return C_PAY_PROPENSITY_TOK;
C_CINEMA_NUM						return C_CINEMA_NUM_TOK;
C_PAY_INFLUENCE_TYPE				return C_PAY_INFLUENCE_TYPE_TOK;

	/* 범용 Token */
FROM					return FROM_TOK;
TO						return TO_TOK;
MONSTER					return MONSTER_TOK;
CHARACTER				return CHARACTER_TOK;
ALL						return ALL_TOK;
ATTACH					return ATTACH_TOK;
AREA					return AREA_TOK;
X						return X_TOK;
Y						return Y_TOK;
R						return R_TOK;
PER						return PER_TOK;

PROB_100				return PROB_100_TOK;
PROB_1K					return PROB_1K_TOK;
PROB_10K				return PROB_10K_TOK;
PROB_100K				return PROB_100K_TOK;
PROB_256				return PROB_256_TOK;

AND						return AND_TOK;
OR						return OR_TOK;

TRUE					return TRUE_TOK;
FALSE					return FALSE_TOK;

"->"					return R_ARROW_TOK;
">"						return GT_TOK;
"<"						return LT_TOK;
">="					return GE_TOK;
"<="					return LE_TOK;
"=="					return EQUAL_TOK;
\$[a-zA-Z][\_a-zA-Z0-9]+	{
								strncpy(QuestLoaderlval.string2048, QuestLoadertext, 2048);
								_loader_print("	USER_VARIABLE_TOK: %s\n", QuestLoaderlval.string2048);
								return USER_VARIABLE_TOK;
							}
[\+-]?[0-9]+			{
							QuestLoaderlval.integer = atoi(QuestLoadertext);
							_loader_print("	INTEGER_TOK: %d\n", QuestLoaderlval.integer);
							return INTEGER_TOK;
						}
[a-zA-Z][\_a-zA-Z0-9]*	{
							strncpy(QuestLoaderlval.string2048, QuestLoadertext, 2048);
							_loader_print("	WORD_TOK: %s\n", QuestLoaderlval.string2048);
							return WORD_TOK;
						}
'([^'\n]|'')*'			{
							strncpy(QuestLoaderlval.string2048, QuestLoadertext + 1, 2048);
							QuestLoaderlval.string2048[strlen(QuestLoaderlval.string2048)-1] = '\0';
							DeleteTwoAposterophe(QuestLoaderlval.string2048);
							_loader_print("	QSTRING_TOK: %s\n", QuestLoaderlval.string2048);
							return QSTRING_TOK;
						}
\n                      { /* ignore EOL */;	g_line_number_quest++; }
#.*$					{ /* ignore comment */; /*_loader_print("	comment line %d\n", g_line_number_quest);*/ }
[ \t]+					/* ignore whitespace */;
.						return QuestLoadertext[0];
%%
