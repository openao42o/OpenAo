
%{
#include <iostream.h>
#include "UserVariable.h"

#include "Quest.h"
#include "AtumDataLoader.h"

extern int QuestLoadererror(const char* s);
extern int QuestLoaderlex();

int				g_line_number_quest = 1;
CUserVariable	g_user_var_quest;

CQuest			g_quest_for_loading;

ez_map<INT, CQuest*>	*g_pQuestInfoMap;
%}

%union
{
	int				integer;
	char			*string;
	ITEM_W_COUNT	item_w_count;
	INT_RANGE		integer_range;
}

%token QUEST_TOK

	/* BASIC_INFO 관련 */
%token BASIC_INFO_TOK
%token QUEST_INDEX_TOK
%token QUEST_NAME_TOK
%token QUEST_TYPE_TOK
%token QUEST_START_TYPE_TOK
%token QUEST_END_TYPE_TOK
%token NPC_INDEX_TOK
%token NPC_NAME_TOK
%token PRE_TALK_TOK
%token FAIL_TALK_TOK
%token SUCCESS_TALK_TOK

%token NEXT_QUEST_INDEX_TOK

	/* START_REQ 관련 */
%token START_REQ_TOK
%token S_REQ_UNIT_KIND_TOK
%token S_REQ_LEVEL_TOK
%token S_REQ_ATTACK_PART_TOK
%token S_REQ_ENDURANCE_PART_TOK
%token S_REQ_FUEL_PART_TOK
%token S_REQ_SOUL_PART_TOK
%token S_REQ_DEFENSE_PART_TOK
%token S_REQ_DODGE_PART_TOK
%token S_REQ_ITEM_TOK
%token S_REQ_QUEST_INDEX_TOK

	/* START_ACTION  관련 */
%token START_ACTION_TOK
%token S_PAY_ITEM_TOK
%token S_DEL_ITEM_TOK

	/* COMPLETION_REQ 관련 */
%token COMPLETION_REQ_TOK
%token C_REQ_MEET_NPC_INDEX_TOK
%token C_REQ_ITEM_TOK

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

	/* 범용 Token */
%token FROM_TOK
%token MONSTER_TOK
%token ALL_TOK

%token GT_TOK
%token LT_TOK
%token GE_TOK
%token LE_TOK
%token EQUAL_TOK

%token <string>		USER_VARIABLE_TOK
%token <integer>	INTEGER_TOK
%token <string>		WORD_TOK
%token <string>		QSTRING_TOK

%left       '-' '+'
%left       '/' '*'
%left       '&'

%type <integer>			integer_expression
%type <string>			string_expression
%type <item_w_count>	item_w_count_value
%type <integer_range>	integer_range_value
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
		USER_VARIABLE_TOK '=' INTEGER_TOK ';'
		{
			g_user_var_quest.AddUserVariableInteger($1, $3);
			_loader_print("user variable %s = %d\n", $1, $3);

			delete $1;
		}
		|
		USER_VARIABLE_TOK '=' QSTRING_TOK ';'
		{
			g_user_var_quest.AddUserVariableString($1, $3);
			_loader_print("user variable %s = %s\n", $1, $3);

			delete $1;
			delete $3;
		}
		|
		USER_VARIABLE_TOK '=' USER_VARIABLE_TOK ';'
		{
			USER_VAR_ELEMENT *pElem = g_user_var_quest.GetUserVariable($3);
			if (pElem == NULL)
			{
				// error
				QuestLoadererror("User Variable NOT Defined");
			}

			if (pElem->Type == ET_INTEGER)
			{
				_loader_print("user variable %s = %d\n", $1, pElem->IntValue);
			}
			else if (pElem->Type == ET_STRING)
			{
				_loader_print("user variable %s = %s\n", $1, pElem->StringValue);
			}

			delete $1;
			delete $3;
		}
		;

quest_definition:
		QUEST_TOK '(' INTEGER_TOK ')' '{' quest_content_list '}' ';'
		{
			g_pQuestInfoMap->insertEZ($3, g_quest_for_loading.Clone());
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
			strncpy(g_quest_for_loading.QuestName, $3, SIZE_MAX_QUEST_NAME);

			delete $3;
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
		NPC_INDEX_TOK '=' integer_expression ';'
		{
			g_quest_for_loading.NPCIndex = $3;
		}
		|
		NPC_NAME_TOK '=' string_expression ';'
		{
			strncpy(g_quest_for_loading.NPCName, $3, SIZE_MAX_NPC_NAME);

			delete $3;
		}
		|
		PRE_TALK_TOK '=' string_expression ';'
		{
			strncpy(g_quest_for_loading.PreTalk, $3, SIZE_MAX_NPC_TALK_STRING);

			delete $3;
		}
		|
		FAIL_TALK_TOK '=' string_expression ';'
		{
			strncpy(g_quest_for_loading.FailTalk, $3, SIZE_MAX_NPC_TALK_STRING);

			delete $3;
		}
		|
		SUCCESS_TALK_TOK '=' string_expression ';'
		{
			strncpy(g_quest_for_loading.SuccessTalk, $3, SIZE_MAX_NPC_TALK_STRING);

			delete $3;
		}
		|
		NEXT_QUEST_INDEX_TOK '=' integer_expression ';'
		{
			g_quest_for_loading.NextQuestIndex = $3;
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
		S_REQ_ENDURANCE_PART_TOK '=' integer_range_value ';'
		{
			g_quest_for_loading.ReqEndurancePart = $3;
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
		S_REQ_DEFENSE_PART_TOK '=' integer_range_value ';'
		{
			g_quest_for_loading.ReqDefensePart = $3;
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
		/* START_ACTION */
		S_PAY_ITEM_TOK '=' item_w_count_value ';'
		{
			g_quest_for_loading.StartPayItemVector.push_back($3);
		}
		|
		S_DEL_ITEM_TOK '=' item_w_count_value ';'
		{
			g_quest_for_loading.StartDelItemVector.push_back($3);
		}
		|
		/* COMPLETION_REQ */
		C_REQ_MEET_NPC_INDEX_TOK '=' integer_expression ';'
		{
			g_quest_for_loading.TargetMeetNPCIndex = $3;
		}
		|
		C_REQ_ITEM_TOK '=' item_w_count_value FROM_TOK MONSTER_TOK integer_expression';'
		{
			g_quest_for_loading.TargetItem = $3;
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
			g_quest_for_loading.EndPayBonusSkillPoint = $3;
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
		C_PAY_ITEM_TOK '='  item_w_count_value ';'
		{
			g_quest_for_loading.EndPayItemVector.push_back($3);
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
			$$.Max = 0x7FFFFFFF;
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
					QuestLoadererror("divided by zero");
				else
					$$ = $1 / $3;
			}
        |
		integer_expression '&' integer_expression	{ $$ = $1 & $3; }
		|
		'(' integer_expression ')'	{ $$ = $2; }
        |
		INTEGER_TOK			{ $$ = $1; }
		|
		USER_VARIABLE_TOK	{ $$ = g_user_var_quest.GetUserVariable($1)->IntValue; }
        ;

string_expression:
		QSTRING_TOK { $$ = strdup($1); }
		|
		USER_VARIABLE_TOK { $$ = strdup(g_user_var_quest.GetUserVariable($1)->StringValue); }
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
			$$.Count = -1;
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
	exit(0);
	return 0;
}
