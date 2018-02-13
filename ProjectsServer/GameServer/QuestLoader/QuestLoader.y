
%{
#pragma warning(disable:4786)
#include <iostream>
#include "UserVariable.h"
//#include "windows.h"
#ifdef _ATUM_FIELD_SERVER
	#include "DBGOUT_Field.h"
#endif

#ifndef DBGOUT
#define DBGOUT (void(0))
#endif

#include "Quest.h"
#include "AtumDataLoader.h"

extern int QuestLoadererror(const char* s);
extern int QuestLoaderlex();

int				g_line_number_quest = 1;
CUserVariable	g_user_var_quest;

CQuest			g_quest_for_loading;
QUEST_NPC_INFO  g_quest_npc_info_for_loading;
vector<ITEM_W_COUNT>		g_item_w_count_list_for_loading;
vector<ITEM_W_COUNT_PROB>	g_item_w_count_list_for_random_pay_item;

ez_map<INT, CQuest*>	*g_pQuestInfoMap;
vectCQuestPtr			*g_pvectQuestInfoList;		// 2007-07-09 by cmkwon, 퀘스트 스크립트를 정보를 vector로 받기 위해
%}

%union
{
	int					BOOLEAN;
	int					integer;
	char				string2048[2048];
	MONSTER_W_COUNT		monster_w_count;
	ITEM_W_COUNT		item_w_count;
	ITEM_W_COUNT_PROB	item_w_count_prob;
	QUEST_PAY_ITEM		quest_pay_item;
	INT_RANGE			integer_range;
	MAP_AREA			map_area;
}

%token QUEST_TOK

	/* BASIC_INFO 관련 */
%token BASIC_INFO_TOK
%token QUEST_INDEX_TOK
%token QUEST_NAME_TOK
%token QUEST_EPISODE_TYPE_TOK						/* // 2008-06-16 by cmkwon, EP3 퀘스트 기존 퀘스트와 구별 - */
%token QUEST_PARTYTYPE_TOK						/* // 2008-12-02 by dhjin, 미션마스터 */
%token QUEST_KIND_TOK
%token QUEST_TYPE_TOK
%token QUEST_START_TYPE_TOK
%token QUEST_END_TYPE_TOK
%token QUEST_PARTY_MEMBERS_TOK
%token QUEST_NPC_INFO_TOK
%token NPC_INDEX_TOK
%token NPC_NAME_TOK
%token PRE_TALK_TOK
%token MID_TALK_TOK
%token FAIL_TALK_TOK
%token SUCCESS_TALK_TOK
%token DESCRIPTION_TOK
%token EVENT_MAP_TOK
%token NEXT_QUEST_INDEX_TOK
%token PARTY_QUEST_MEMBER_COUNT_TOK
%token DROP_ITEM_TO_ALL_ATTACK_CHARACTER_TOK
%token TIME_LIMIT_IN_MINUTES_TOK
%token DISCARDABLE_QUEST_TOK
%token BI_QUEST_ATTRI_IS_CITYWAR_TOK
%token BI_QUEST_ATTRI_NEVER_COMPLETION_BY_USER_TOK
%token DROP_ITEM_TO_ALL_PARTY_MEMBERS_TOK		/* // 2009-01-13 by cmkwon, 미션 보상을 파티원 모두에게 주기 명령어 추가 - */

	/* START_REQ 관련 */
%token START_REQ_TOK
%token S_REQ_UNIT_KIND_TOK
%token S_REQ_LEVEL_TOK
%token S_REQ_ATTACK_PART_TOK
%token S_REQ_DEFENSE_PART_TOK
%token S_REQ_FUEL_PART_TOK
%token S_REQ_SOUL_PART_TOK
%token S_REQ_SHIELD_PART_TOK
%token S_REQ_DODGE_PART_TOK
%token S_REQ_ITEM_TOK
%token S_REQ_QUEST_INDEX_TOK
%token S_REQ_RECV_QUEST_INDEX_TOK
%token S_REQ_GUILD_TOK
%token S_REQ_INFLUENCE_TYPE_TOK
%token S_REQ_START_AREA_TOK

	/* START_ACTION  관련 */
%token START_ACTION_TOK
%token S_PAY_ITEM_TOK
%token S_DEL_ITEM_TOK
%token S_START_MAP_INDEX_TOK
%token S_START_PARTY_WARP_FLAG_TOK

	/* COMPLETION_REQ 관련 */
%token COMPLETION_REQ_TOK
%token C_REQ_MEET_NPC_INFO_TOK
%token C_REQ_ITEM_TOK
%token C_REQ_MEET_OBJ_INDEX_TOK
%token C_REQ_DEAD_BY_MONSTER_TOK
%token C_REQ_MEET_MAP_AREA_TOK
%token C_REQ_MONSTER_TOK
%token C_REQ_MIX_ITEM_TARGET_TOK

	/* COMPLETION_ACTION 관련 */
%token COMPLETION_ACTION_TOK
%token C_PAY_BONUS_STAT_TOK
%token C_PAY_BONUS_SKILL_POINT_TOK
%token C_PAY_FAME_TOK
%token C_PAY_STATUS_TOK
%token C_PAY_LEVEL_TOK
%token C_PAY_EXPERIENCE_TOK
%token C_PAY_WARP_MAP_INDEX_TOK
%token C_DEL_ITEM_TOK
%token C_PAY_ITEM_TOK
%token C_PAY_ITEM_MISSIONMASTER_TOK		// 2008-12-02 by dhjin, 미션마스터
%token C_RANDOM_PAY_ITEM_TOK
%token C_MIX_ITEM_TOK
%token C_PAY_PROPENSITY_TOK
%token C_CINEMA_NUM_TOK
%token C_PAY_INFLUENCE_TYPE_TOK

	/* 범용 Token */
%token FROM_TOK
%token TO_TOK
%token MONSTER_TOK
%token CHARACTER_TOK
%token ALL_TOK
%token ATTACH_TOK
%token AREA_TOK
%token X_TOK
%token Y_TOK
%token R_TOK
%token PER_TOK

%token PROB_100_TOK
%token PROB_1K_TOK
%token PROB_10K_TOK
%token PROB_100K_TOK
%token PROB_256_TOK

%token AND_TOK
%token OR_TOK

%token TRUE_TOK
%token FALSE_TOK

%token R_ARROW_TOK
%token GT_TOK
%token LT_TOK
%token GE_TOK
%token LE_TOK
%token EQUAL_TOK

%token <string2048>	USER_VARIABLE_TOK
%token <integer>	INTEGER_TOK
%token <string2048>	WORD_TOK
%token <string2048>	QSTRING_TOK

%left       '-' '+'
%left       '/' '*'
%left       AND_TOK OR_TOK

%type <integer>				integer_expression
%type <string2048>			string_expression
%type <monster_w_count>		monster_w_count_value
%type <item_w_count>		item_w_count_value
%type <item_w_count_prob>	item_w_count_prob_value
%type <quest_pay_item>		quest_pay_item_value
%type <quest_pay_item>		quest_missinmaster_pay_item_value		// 2008-12-02 by dhjin, 미션마스터
%type <integer_range>		integer_range_value
%type <map_area>			map_area_value
%type <integer>				per_item_value
%type <BOOLEAN>				attach_value
%type <integer>				to_unit_kind_value
%type <BOOLEAN>				boolean_value
%type <integer>				prob_value
%%
command_list:
		command
		|
		command_list command
		;

command:
		user_variable_definition
		|
		quest_definition
		;

user_variable_definition:
		USER_VARIABLE_TOK '=' integer_expression ';'
		{
			g_user_var_quest.AddUserVariableInteger($1, $3);
			_loader_print("user variable %s = %d\n", $1, $3);
		}
		|
		USER_VARIABLE_TOK '=' QSTRING_TOK ';'
		{
			g_user_var_quest.AddUserVariableString($1, $3);
			_loader_print("user variable %s = %s\n", $1, $3);
		}
		|
		USER_VARIABLE_TOK '=' USER_VARIABLE_TOK ';'
		{
			USER_VAR_ELEMENT *pElem = g_user_var_quest.GetUserVariable($3);
			if (pElem == NULL)
			{
				// error
				QuestLoadererror("User Variable NOT Defined");
				return -1;
			}

			if (pElem->Type == ET_INTEGER)
			{
				_loader_print("user variable %s = %d\n", $1, pElem->IntValue);
			}
			else if (pElem->Type == ET_STRING)
			{
				_loader_print("user variable %s = %s\n", $1, pElem->StringValue);
			}
		}
		;

quest_definition:
		QUEST_TOK '(' INTEGER_TOK ')' '{' quest_content_list '}' ';'
		{
			// 2008-09-09 by cmkwon, 퀘스트 인덱스 중복 체크 
			if(NULL != g_pQuestInfoMap->findEZ($3))
			{
				// error
				QuestLoadererror("QuestIndex dup");
				return -1;
			}

			// 2007-07-09 by cmkwon, 퀘스트 스크립트를 정보를 vector로 받기 위해 - 아래와 같이 수정
			//g_pQuestInfoMap->insertEZ($3, g_quest_for_loading.Clone());
			CQuest *pQuestInfo = g_quest_for_loading.Clone();
			g_pQuestInfoMap->insertEZ($3, pQuestInfo);
			if(g_pvectQuestInfoList)
			{
				g_pvectQuestInfoList->push_back(pQuestInfo);
			}
			_loader_print("quest %d defined...\n", $3);

			// reset
			g_quest_for_loading.Reset();
		}
		;

quest_content_list:
		quest_content
		|
		quest_content_list quest_content
		;

quest_content:
		BASIC_INFO_TOK '{' statement_list '}' ';'
		{
			// _loader_print("%s's BASIC_INFO Parsed\n", "rr");
		}
		|
		BASIC_INFO_TOK '{' '}' ';'
		|
		START_REQ_TOK '{' statement_list '}' ';'
		|
		START_REQ_TOK '{' '}' ';'
		|
		START_ACTION_TOK '{' statement_list '}' ';'
		|
		START_ACTION_TOK '{' '}' ';'
		|
		COMPLETION_REQ_TOK '{' statement_list '}' ';'
		|
		COMPLETION_REQ_TOK '{' '}' ';'
		|
		COMPLETION_ACTION_TOK '{' statement_list '}' ';'
		|
		COMPLETION_ACTION_TOK '{' '}' ';'
		;

statement_list:
		statement
		|
		statement_list statement
		;

statement:
		/* BASIC_INFO */
		QUEST_INDEX_TOK '=' integer_expression ';'
		{
			g_quest_for_loading.QuestIndex = $3;
		}
		|
		QUEST_NAME_TOK '=' string_expression ';'
		{
#ifdef _ATUM_FIELD_SERVER
			if(SIZE_MAX_QUEST_NAME <= strlen($3))
			{
				DbgOut("QuestName overflow error, QuestIndex(%d) QuestNameLength(%d)\n", g_quest_for_loading.QuestIndex, strlen($3));
			}
#endif
			memset(g_quest_for_loading.QuestName, 0x00, SIZE_MAX_QUEST_NAME);
			strncpy(g_quest_for_loading.QuestName, $3, SIZE_MAX_QUEST_NAME-1);
		}
		|
		QUEST_EPISODE_TYPE_TOK '=' integer_expression ';'
		{
			g_quest_for_loading.QuestEpisodeType = $3;	// 2008-06-16 by cmkwon, EP3 퀘스트 기존 퀘스트와 구별 - 
		}
		|
		QUEST_PARTYTYPE_TOK '=' integer_expression ';'
		{
			g_quest_for_loading.QuestPartyType = $3;        // 2008-12-02 by dhjin, 미션마스터
		}
		|
		QUEST_KIND_TOK '=' integer_expression ';'
		{
			g_quest_for_loading.QuestKind = $3;
		}
		|
		QUEST_TYPE_TOK '=' integer_expression ';'
		{
			g_quest_for_loading.QuestType = $3;
		}
		|
		QUEST_START_TYPE_TOK '=' integer_expression ';'
		{
			g_quest_for_loading.QuestStartType = $3;
		}
		|
		QUEST_END_TYPE_TOK '=' integer_expression ';'
		{
			g_quest_for_loading.QuestEndType = $3;
		}
		|
		QUEST_PARTY_MEMBERS_TOK '=' integer_expression ';'
		{
			g_quest_for_loading.QuestPartyMembers = $3;
		}
		|
		QUEST_NPC_INFO_TOK '{' quest_npc_statement_list '}' ';'
		{
			g_quest_for_loading.QuestNPCInfo = g_quest_npc_info_for_loading;
			g_quest_npc_info_for_loading.Reset();
		}
		|
		DESCRIPTION_TOK '=' string_expression ';'
		{
#ifdef _ATUM_FIELD_SERVER
			if(SIZE_MAX_NPC_TALK_STRING <= strlen($3))
			{
				DbgOut("QuestDescription overflow error, QuestIndex(%d) QuestDescriptionLength(%d)\n", g_quest_for_loading.QuestIndex, strlen($3));
			}
#endif
			memset(g_quest_for_loading.QuestDescription, 0x00, SIZE_MAX_NPC_TALK_STRING);
			strncpy(g_quest_for_loading.QuestDescription, $3, SIZE_MAX_NPC_TALK_STRING-1);
		}
		|
		EVENT_MAP_TOK '=' integer_expression ';'
		{
			g_quest_for_loading.EventMapIndex = $3;
		}
		|
		NEXT_QUEST_INDEX_TOK '=' integer_expression ';'
		{
			g_quest_for_loading.NextQuestIndex = $3;
		}
		|
		PARTY_QUEST_MEMBER_COUNT_TOK '=' integer_expression ';'
		{
			g_quest_for_loading.PartyQuestMemberCount = $3;
		}
		|
		DROP_ITEM_TO_ALL_ATTACK_CHARACTER_TOK '=' boolean_value ';'
		{
			g_quest_for_loading.DropItemToAllAttackCharacter = $3;
		}
		|
		TIME_LIMIT_IN_MINUTES_TOK '=' integer_expression ';'
		{
			g_quest_for_loading.TimeLimitInMinutes = $3;
		}
		|
		DISCARDABLE_QUEST_TOK '=' boolean_value ';'
		{
			g_quest_for_loading.IsDiscardable = $3;
		}
		|
		BI_QUEST_ATTRI_IS_CITYWAR_TOK '=' boolean_value ';'
		{
			g_quest_for_loading.IsCityWar = $3;
		}
		|
		BI_QUEST_ATTRI_NEVER_COMPLETION_BY_USER_TOK '=' boolean_value ';'
		{
			g_quest_for_loading.IsNeverCompletionByUser = $3;
		}
		|
		DROP_ITEM_TO_ALL_PARTY_MEMBERS_TOK '=' boolean_value ';'		/* // 2009-01-13 by cmkwon, 미션 보상을 파티원 모두에게 주기 명령어 추가 -  */
		{
			g_quest_for_loading.IsDropItemToAllPartyMembers = $3;
		}
		|
		/* START_REQ */
		S_REQ_UNIT_KIND_TOK '=' integer_expression ';'
		{
			g_quest_for_loading.ReqUnitKind = $3;
		}
		|
		S_REQ_LEVEL_TOK '=' integer_range_value ';'
		{
			g_quest_for_loading.ReqLevel = $3;
		}
		|
		S_REQ_ATTACK_PART_TOK '=' integer_range_value ';'
		{
			g_quest_for_loading.ReqAttackPart = $3;
		}
		|
		S_REQ_DEFENSE_PART_TOK '=' integer_range_value ';'
		{
			g_quest_for_loading.ReqDefensePart = $3;
		}
		|
		S_REQ_FUEL_PART_TOK '=' integer_range_value ';'
		{
			g_quest_for_loading.ReqFuelPart = $3;
		}
		|
		S_REQ_SOUL_PART_TOK '=' integer_range_value ';'
		{
			g_quest_for_loading.ReqSoulPart = $3;
		}
		|
		S_REQ_SHIELD_PART_TOK '=' integer_range_value ';'
		{
			g_quest_for_loading.ReqShieldPart = $3;
		}
		|
		S_REQ_DODGE_PART_TOK '=' integer_range_value ';'
		{
			g_quest_for_loading.ReqDodgePart = $3;
		}
		|
		S_REQ_ITEM_TOK '=' item_w_count_value ';'
		{
			g_quest_for_loading.ReqItemVector.push_back($3);
		}
		|
		S_REQ_QUEST_INDEX_TOK '=' integer_expression ';'
		{
			g_quest_for_loading.ReqQuestIndexVector.push_back($3);
		}
		|
		S_REQ_RECV_QUEST_INDEX_TOK '=' integer_expression ';'
		{
			g_quest_for_loading.ReqRecvQuestIndexVector.push_back($3);
		}
		|
		S_REQ_GUILD_TOK '=' integer_expression ';'
		{
			g_quest_for_loading.ReqGuild = $3;
		}
		|
		S_REQ_INFLUENCE_TYPE_TOK '=' integer_expression ';'
		{
			g_quest_for_loading.ReqInfluenceType = $3;
		}
		|
		S_REQ_START_AREA_TOK '=' map_area_value ';'
		{
			g_quest_for_loading.ReqStartArea = $3;
		}
		|
		/* START_ACTION */
		S_PAY_ITEM_TOK '=' quest_pay_item_value ';'
		{
			g_quest_for_loading.StartPayItemVector.push_back($3);
		}
		|
		S_DEL_ITEM_TOK '=' item_w_count_value ';'
		{
			g_quest_for_loading.StartDelItemVector.push_back($3);
		}
		|
		S_START_MAP_INDEX_TOK '=' integer_expression ';'
		{
			g_quest_for_loading.StartMapIndex = $3;
		}
		|
		S_START_PARTY_WARP_FLAG_TOK '=' boolean_value ';'
		{
			g_quest_for_loading.StartPartyWarpFlag = $3;
		}
		|
		/* COMPLETION_REQ */
		C_REQ_MEET_NPC_INFO_TOK '{' quest_npc_statement_list '}' ';'
		{
			g_quest_for_loading.TargetMeetNPCInfo = g_quest_npc_info_for_loading;
			g_quest_npc_info_for_loading.Reset();
		}
		|
		C_REQ_MEET_OBJ_INDEX_TOK '=' integer_expression map_area_value ';'
		{
			g_quest_for_loading.TargetMeetObjectIndex = $3;
			g_quest_for_loading.TargetMeetObjectMapArea = $4;
		}
		|
		C_REQ_ITEM_TOK '=' item_w_count_value';'
		{
			g_quest_for_loading.TargetItemVector.push_back($3);
		}
		|
		C_REQ_ITEM_TOK '=' item_w_count_value FROM_TOK MONSTER_TOK '(' integer_expression ',' integer_expression '~' integer_expression ',' integer_expression ')' map_area_value ';'
		{
			///////////////////////////////////////////////////////////////////////////////
			// 2007-03-14 by cmkwon, ItemNum이 0이거나 Count가 0이면 드랍아이템리스트에는 추가하지 않는다. 위치 정보에는 추가한다.
			if(0 < $3.ItemNum
				&& 0 <= $3.Count)
			{
				QUEST_DROP_ITEM tmpQuestDropItem;
				tmpQuestDropItem.MonsterUniqueNumber = $7;
				tmpQuestDropItem.ItemNumber = $3.ItemNum;
				tmpQuestDropItem.MinCount = $9;
				tmpQuestDropItem.MaxCount = $11;
				tmpQuestDropItem.Probability = $13;
				g_quest_for_loading.vecQuestDropItem.push_back(tmpQuestDropItem);
			}

			ITEM_W_COUNT_MAP_AREA tmpTargetMonsterItem;
			tmpTargetMonsterItem.ItemWithCount = $3;
			tmpTargetMonsterItem.MapArea = $15;
			tmpTargetMonsterItem.MonsterUniqueNumber = $7;
			g_quest_for_loading.TargetMonsterItemVector.push_back(tmpTargetMonsterItem);
		}
		|
		C_REQ_ITEM_TOK '=' item_w_count_value FROM_TOK CHARACTER_TOK '(' integer_expression ',' integer_expression ',' integer_expression '~' integer_expression ',' integer_expression ')' map_area_value ';'
		{
			QUEST_DROP_CHARACTER2ITEM tmpQuestDropItem;
			tmpQuestDropItem.InfluenceType0				= $7;
			tmpQuestDropItem.UnitKindMask0				= $9;
			tmpQuestDropItem.Character2Item.ItemNumber	= $3.ItemNum;
			tmpQuestDropItem.Character2Item.MinCount	= $11;
			tmpQuestDropItem.Character2Item.MaxCount	= $13;
			tmpQuestDropItem.Character2Item.Probability = $15;
			tmpQuestDropItem.Character2Item.QuestIndex	= 0;
			g_quest_for_loading.vecQuestDropCharacter2Item.push_back(tmpQuestDropItem);

			ITEM_W_COUNT_MAP_AREA tmpTargetMonsterItem;
			tmpTargetMonsterItem.ItemWithCount = $3;
			tmpTargetMonsterItem.MapArea = $17;
			tmpTargetMonsterItem.MonsterUniqueNumber = 0;
			g_quest_for_loading.TargetMonsterItemVector.push_back(tmpTargetMonsterItem);
		}
		|
		C_REQ_DEAD_BY_MONSTER_TOK '=' integer_expression ';'
		{
			g_quest_for_loading.TargetDeadByMonsterUniquenumber = $3;
		}
		|
		C_REQ_MEET_MAP_AREA_TOK '=' map_area_value ';'
		{
			g_quest_for_loading.TargetMeetMapArea = $3;
		}
		|
		C_REQ_MONSTER_TOK '=' monster_w_count_value';'
		{
			MONSTER_W_COUNT tmMonCnt;
			tmMonCnt.MonsterUniqueNumber	= $3.MonsterUniqueNumber;
			tmMonCnt.Count					= $3.Count;
			tmMonCnt.MapArea.Reset();
			g_quest_for_loading.TargetMonsterVector.push_back(tmMonCnt);
		}
		|
		C_REQ_MONSTER_TOK '=' monster_w_count_value map_area_value';'
		{
			MONSTER_W_COUNT tmMonCnt;
			tmMonCnt.MonsterUniqueNumber	= $3.MonsterUniqueNumber;
			tmMonCnt.Count					= $3.Count;
			tmMonCnt.MapArea				= $4;
			g_quest_for_loading.TargetMonsterVector.push_back(tmMonCnt);
		}
		|
		C_REQ_MIX_ITEM_TARGET_TOK '=' item_w_count_value';'
		{
			g_quest_for_loading.TargetMixItemTarget	= $3;
		}
		|
		/* COMPLETION_ACTION */
		C_PAY_BONUS_STAT_TOK '=' integer_expression ';'
		{
			g_quest_for_loading.EndPayBonusStat = $3;
		}
		|
		C_PAY_BONUS_SKILL_POINT_TOK '=' integer_expression ';'
		{
// 2005-11-15 by cmkwon, 삭제함
//			g_quest_for_loading.EndPayBonusSkillPoint = $3;
		}
		|
		C_PAY_FAME_TOK '=' integer_expression ';'
		{
			g_quest_for_loading.EndPayFame = $3;
		}
		|
		C_PAY_STATUS_TOK '=' integer_expression ';'
		{
			g_quest_for_loading.EndPayStatus = $3;
		}
		|
		C_PAY_LEVEL_TOK '=' integer_expression ';'
		{
			g_quest_for_loading.EndPayLevel = $3;
		}
		|
		C_PAY_EXPERIENCE_TOK '=' integer_expression ';'
		{
			g_quest_for_loading.EndPayExperience = $3;
		}
		|
		C_PAY_WARP_MAP_INDEX_TOK '=' integer_expression ';'
		{
			g_quest_for_loading.EndPayWarpMapIndex = $3;
		}
		|
		C_DEL_ITEM_TOK '='  item_w_count_value ';'
		{
			g_quest_for_loading.EndDeleteItemVector.push_back($3);
		}
		|
		C_PAY_ITEM_TOK '='  quest_pay_item_value ';'
		{
			g_quest_for_loading.EndPayItemVector.push_back($3);
		}
		|
		C_PAY_ITEM_MISSIONMASTER_TOK '='  quest_missinmaster_pay_item_value ';'
		{// 2008-12-02 by dhjin, 미션마스터
			g_quest_for_loading.EndMissionMasterPayItemVector.push_back($3);
		}		
		|
		C_RANDOM_PAY_ITEM_TOK '=' item_w_count_prob_list PER_TOK integer_expression ';'
		{
			// not implemented yet
			QUEST_RANDOM_PAY_ITEM tmpRandomPayItem; tmpRandomPayItem.Reset();
			for (int i = 0; i < g_item_w_count_list_for_random_pay_item.size(); i++)
			{
				if (i >= COUNT_RANDOM_PAY_ITEM)
				{
					break;
				}

				tmpRandomPayItem.ItemWithCountProb[i] = g_item_w_count_list_for_random_pay_item[i];
				tmpRandomPayItem.NumOfItems++;
			}
			tmpRandomPayItem.PerItemNum = $5;

			g_quest_for_loading.EndRandomPayItemVector.push_back(tmpRandomPayItem);

			g_item_w_count_list_for_random_pay_item.clear();
		}
		|
		C_MIX_ITEM_TOK '=' item_w_count_list R_ARROW_TOK integer_expression prob_value ';'
		{
			ITEM_MIXING_INFO tmpMixingInfo;
			tmpMixingInfo.TargetItemNum = $5;
			tmpMixingInfo.MixingProbability = $6;
			tmpMixingInfo.MixingCost = 0;
			memset(tmpMixingInfo.SourceItem, 0x00, COUNT_ITEM_MIXING_SOURCE*sizeof(ITEM_W_COUNT));
			tmpMixingInfo.NumOfSourceItems = 0;
			for (int i = 0; i < g_item_w_count_list_for_loading.size(); i++)
			{
				if (i >= COUNT_ITEM_MIXING_SOURCE)
				{
					break;
				}

				// 할당
				tmpMixingInfo.SourceItem[i] = g_item_w_count_list_for_loading[i];
				tmpMixingInfo.NumOfSourceItems++;

				//printf("%d(%d)\r\n", g_item_w_count_list_for_loading[i].ItemNum, g_item_w_count_list_for_loading[i].Count);
			}

			g_quest_for_loading.EndPayMixItemVector.push_back(tmpMixingInfo);

			g_item_w_count_list_for_loading.clear();
		}
		|
		C_PAY_PROPENSITY_TOK '=' integer_expression ';'
		{
			g_quest_for_loading.EndPayPropensity = $3;
		}
		|
		C_CINEMA_NUM_TOK '=' integer_expression ';'
		{
			g_quest_for_loading.EndCinemaNum = $3;
		}
		|
		C_PAY_INFLUENCE_TYPE_TOK '=' integer_expression ';'
		{
			g_quest_for_loading.EndInfluenceType = $3;
		}
		;

item_w_count_prob_list:
		item_w_count_prob_value
		{
			g_item_w_count_list_for_random_pay_item.push_back($1);
		}
		|
		item_w_count_prob_list OR_TOK item_w_count_prob_value
		{
			g_item_w_count_list_for_random_pay_item.push_back($3);
		}
		;

item_w_count_prob_value:
		integer_expression '(' integer_expression ',' integer_expression ')'
		{
			$$.ItemNum = $1;
			$$.Count = $3;
			$$.Prob100 = $5;
		}
		;

item_w_count_list:
		item_w_count_value
		{
			g_item_w_count_list_for_loading.push_back($1);
		}
		|
		item_w_count_list '+' item_w_count_value
		{
			g_item_w_count_list_for_loading.push_back($3);
		}
		;

map_area_value:
		/* if nothing defined... */
		{
			$$.MapIndex = 0;
			$$.X = -1;
			$$.Y = -1;
			$$.Radius = -1;
		}
		|
		AREA_TOK integer_expression '(' X_TOK integer_expression ',' Y_TOK integer_expression ',' R_TOK integer_expression ')'
		{
			$$.MapIndex = $2;
			$$.X = $5;
			$$.Y = $8;
			$$.Radius = $11;
		}
		|
		AREA_TOK integer_expression
		{
			$$.MapIndex = $2;
			$$.X = -1;
			$$.Y = -1;
			$$.Radius = -1;
		}
		;

integer_range_value:
		'(' integer_expression ')'
		{
			$$.Min = $2;
			$$.Max = $2;
		}
		|
		'(' integer_expression '~' integer_expression ')'
		{
			$$.Min = $2;
			$$.Max = $4;
		}
		|
		'(' integer_expression '~' ')'
		{
			$$.Min = $2;
			$$.Max = MAX_INT_VALUE;
		}
		|
		'(' '~' integer_expression ')'
		{
			$$.Min = 0;
			$$.Max = $3;
		}
		;

integer_expression:
		integer_expression '+' integer_expression	{ $$ = $1 + $3; }
		|
		integer_expression '-' integer_expression	{ $$ = $1 - $3; }
		|
		integer_expression '*' integer_expression	{ $$ = $1 * $3; }
		|
		integer_expression '/' integer_expression
			{
				if ($3 == 0.0)
				{
					QuestLoadererror("divided by zero");
					return -1;
				}
				else
				{
					$$ = $1 / $3;
				}
			}
        |
		integer_expression AND_TOK integer_expression	{ $$ = $1 & $3; }
		|
		integer_expression OR_TOK integer_expression	{ $$ = $1 | $3; }
		|
		'(' integer_expression ')'	{ $$ = $2; }
        |
		INTEGER_TOK			{ $$ = $1; }
		|
		USER_VARIABLE_TOK
			{
				USER_VAR_ELEMENT *pElem = g_user_var_quest.GetUserVariable($1);
				if (pElem == NULL)
				{
					// error
					QuestLoadererror("User Variable NOT Defined");
					return -1;
				}
				if (pElem->Type != ET_INTEGER)
				{
					// error
					QuestLoadererror("User Variable Type NOT Matched");
					return -1;
				}
				$$ = pElem->IntValue;
			}
        ;

string_expression:
		QSTRING_TOK { strncpy($$, $1, 2048); }
		|
		USER_VARIABLE_TOK
			{
				USER_VAR_ELEMENT *pElem = g_user_var_quest.GetUserVariable($1);
				if (pElem == NULL)
				{
					// error
					QuestLoadererror("User Variable NOT Defined");
					return -1;
				}
				if (pElem->Type != ET_STRING)
				{
					// error
					QuestLoadererror("User Variable Type NOT Matched");
					return -1;
				}
				strncpy($$, pElem->StringValue, 2048);
			}
		;

monster_w_count_value:
		integer_expression '(' integer_expression ')'
		{
			$$.MonsterUniqueNumber = $1;
			$$.Count = $3;
		}
		|
		integer_expression '(' ALL_TOK ')'
		{
			$$.MonsterUniqueNumber = $1;
			$$.Count = MAX_INT_VALUE;
		}
		;

item_w_count_value:
		integer_expression '(' integer_expression ')'
		{
			$$.ItemNum = $1;
			$$.Count = $3;
		}
		|
		integer_expression '(' ALL_TOK ')'
		{
			$$.ItemNum = $1;
			$$.Count = MAX_INT_VALUE;
		}
		;

quest_pay_item_value:
		item_w_count_value per_item_value attach_value to_unit_kind_value
		{
			$$.ItemNum = $1.ItemNum;
			$$.Count = $1.Count;
			$$.PerItem = $2;
			$$.ToBeAttached = $3;
			$$.ToUnitKind = $4;
		}
		;

quest_missinmaster_pay_item_value:	// 2008-12-02 by dhjin, 미션마스터
		item_w_count_value per_item_value attach_value to_unit_kind_value
		{
			$$.ItemNum = $1.ItemNum;
			$$.Count = $1.Count;
			$$.PerItem = $2;
			$$.ToBeAttached = $3;
			$$.ToUnitKind = $4;
		}
		;

quest_npc_statement_list:
		quest_npc_statement
		|
		quest_npc_statement_list quest_npc_statement
		;

quest_npc_statement:
		NPC_INDEX_TOK '=' integer_expression ';'
		{
			g_quest_npc_info_for_loading.NPCIndex = $3;
		}
		|
		NPC_NAME_TOK '=' string_expression ';'
		{
#ifdef _ATUM_FIELD_SERVER
			if(SIZE_MAX_NPC_NAME <= strlen($3))
			{
				DbgOut("Quest NPCName overflow error, QuestIndex(%d) NPCNameLength(%d)\n", g_quest_for_loading.QuestIndex, strlen($3));
			}
#endif
			memset(g_quest_npc_info_for_loading.NPCName, 0x00, SIZE_MAX_NPC_NAME);
			strncpy(g_quest_npc_info_for_loading.NPCName, $3, SIZE_MAX_NPC_NAME-1);
		}
		|
		PRE_TALK_TOK '=' string_expression ';'
		{
#ifdef _ATUM_FIELD_SERVER
			if(SIZE_MAX_NPC_TALK_STRING <= strlen($3))
			{
				DbgOut("Quest PreTalk overflow error, QuestIndex(%d) PreTalkLength(%d)\n", g_quest_for_loading.QuestIndex, strlen($3));
			}
#endif
			memset(g_quest_npc_info_for_loading.PreTalk, 0x00, SIZE_MAX_NPC_TALK_STRING);
			strncpy(g_quest_npc_info_for_loading.PreTalk, $3, SIZE_MAX_NPC_TALK_STRING-1);
		}
		|
		MID_TALK_TOK '=' string_expression ';'
		{
#ifdef _ATUM_FIELD_SERVER
			if(SIZE_MAX_NPC_TALK_STRING <= strlen($3))
			{
				DbgOut("Quest MidTalk overflow error, QuestIndex(%d) MidTalkLength(%d)\n", g_quest_for_loading.QuestIndex, strlen($3));
			}
#endif
			memset(g_quest_npc_info_for_loading.MidTalk, 0x00, SIZE_MAX_NPC_TALK_STRING);
			strncpy(g_quest_npc_info_for_loading.MidTalk, $3, SIZE_MAX_NPC_TALK_STRING-1);
		}
		|
		FAIL_TALK_TOK '=' string_expression ';'
		{
#ifdef _ATUM_FIELD_SERVER
			if(SIZE_MAX_NPC_TALK_STRING <= strlen($3))
			{
				DbgOut("Quest FailTalk overflow error, QuestIndex(%d) FailTalkLength(%d)\n", g_quest_for_loading.QuestIndex, strlen($3));
			}
#endif
			memset(g_quest_npc_info_for_loading.FailTalk, 0x00, SIZE_MAX_NPC_TALK_STRING);
			strncpy(g_quest_npc_info_for_loading.FailTalk, $3, SIZE_MAX_NPC_TALK_STRING-1);
		}
		|
		SUCCESS_TALK_TOK '=' string_expression ';'
		{
#ifdef _ATUM_FIELD_SERVER
			if(SIZE_MAX_NPC_TALK_STRING <= strlen($3))
			{
				DbgOut("Quest SuccessTalk overflow error, QuestIndex(%d) SuccessTalkLength(%d)\n", g_quest_for_loading.QuestIndex, strlen($3));
			}
#endif
			memset(g_quest_npc_info_for_loading.SuccessTalk, 0x00, SIZE_MAX_NPC_TALK_STRING);
			strncpy(g_quest_npc_info_for_loading.SuccessTalk, $3, SIZE_MAX_NPC_TALK_STRING-1);
		}
		;

per_item_value:
		/* if nothing defined... */
		{
			$$ = 0;
		}
		|
		PER_TOK integer_expression
		{
			$$ = $2;
		}
		;

attach_value:
		/* if nothing defined... */
		{
			$$ = FALSE;
		}
		|
		ATTACH_TOK
		{
			$$ = TRUE;
		}
		;

to_unit_kind_value:
		/* if nothing defined... */
		{
			$$ = UNITKIND_ALL_MASK;
		}
		|
		TO_TOK integer_expression
		{
			$$ = $2;
		}
		;

boolean_value:
		TRUE_TOK
		{
			$$ = TRUE;
		}
		|
		FALSE_TOK
		{
			$$ = FALSE;
		}
		;

prob_value:
		PROB_100_TOK '(' integer_expression ')'
		{
			$$ = $3;
		}
		|
		PROB_1K_TOK '(' integer_expression ')'
		{
			$$ = $3;
		}
		|
		PROB_10K_TOK '(' integer_expression ')'
		{
			$$ = $3;
		}
		|
		PROB_100K_TOK '(' integer_expression ')'
		{
			$$ = $3;
		}
		|
		PROB_256_TOK '(' integer_expression ')'
		{
			$$ = $3;
		}
		;
%%

/* programs */

/*
 * ???
 */
extern "C"
{
int
QuestLoaderwrap()
{
  return 1;
}
}

/*
 * Report an error situation discovered in a production
 *
 * This does not do anything since we report all error situations through
 * idl_global->err() operations
 */
int
QuestLoadererror(const char *s)
{
    cerr << "Error: " << s << " at line " << g_line_number_quest << endl;
	DBGOUT("Error: %s at line %d\r\n", s, g_line_number_quest);
	system("pause");
	return 0;
}
