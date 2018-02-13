
%{
#pragma warning(disable:4786)

#ifndef _ATUM_SERVER
#undef DBGOUT
#define DBGOUT (void(0))
#endif // _ATUM_SERVER

#include <iostream.h>
#include "NPCScripts.h"
#define SIZE_MAX_TALK 4096

int							g_line_number;
bool						g_bParseSucceeded;

extern int NpcScriptLoadererror(const char* s);
extern int NpcScriptLoaderlex();

NPCID_t						g_NpcID = 0;
NPCTalk						g_NPCTalk;
NPCScript					g_NPCScript;

ez_map<NPCID_t, NPCScript>	*g_NpcScriptsMap;

void SetParseError(const char* err, char* pStr = NULL)
{
	g_bParseSucceeded = false;
	printf("line %d: Parse Error(%s) String(%s)\n", g_line_number, err, pStr);
	NpcScriptLoadererror(err);
}

int g_line_number_qeust = 0;

%}

%token				INTEGER_TOK
%token				WORD_TOK
%token				SCRIPT_TOK
%token				OPAREN_TOK
%token				EPAREN_TOK
%token				OBRACE_TOK
%token				EBRACE_TOK
%token				SEMICOLON_TOK
%token				QSTRING_TOK
%token				CASE_TOK
%token				DEFAULT_TOK
%token				TALK_TOK
%token				MENU_TOK
%token				LEVEL_TOK
%token				UNITKIND_TOK
%token				ITEM_TOK
%token				QUEST_TOK
%token				AND_TOK
%token				GT_TOK
%token				LT_TOK
%token				GE_TOK
%token				LE_TOK
%token				EQUAL_TOK

%%

commands: command
		|
		commands command
		;


command:
		SCRIPT_TOK INTEGER_TOK scriptcontent SEMICOLON_TOK
		{
			g_NPCScript.NpcID = $2;

			g_NpcScriptsMap->insertEZ(g_NPCScript.NpcID, g_NPCScript);
			printf("Complete script for '%d' found\n", g_NPCScript.NpcID);
			g_NPCScript.reset();
		}
		;

scriptcontent:
		OBRACE_TOK scriptstatements EBRACE_TOK
		;

scriptstatements: scriptstatement
		|
		scriptstatements scriptstatement
		;

scriptstatement:
		CASE_TOK conditions statementcontent SEMICOLON_TOK
		{
			NPCTalk *pNPCTalk = new NPCTalk;
			pNPCTalk->TalkID = g_NPCTalk.TalkID;
			pNPCTalk->TalkString = g_NPCTalk.TalkString;
			pNPCTalk->MenuVector = g_NPCTalk.MenuVector;
			g_NPCTalk.reset();

			if (g_NPCScript.NpcTalkVector.size() != pNPCTalk->TalkID)
			{
				// error
				SetParseError("TalkID 순서가 맞지 않습니다.", pNPCTalk->TalkString);
				break;
			}

			g_NPCScript.NpcTalkVector.push_back(pNPCTalk);
		}
		|
		DEFAULT_TOK statementcontent SEMICOLON_TOK
		{
			NPCTalk *pNPCTalk = new NPCTalk;
			pNPCTalk->TalkID = g_NPCTalk.TalkID;
			pNPCTalk->TalkString = g_NPCTalk.TalkString;
			pNPCTalk->MenuVector = g_NPCTalk.MenuVector;
			g_NPCTalk.reset();

			if (g_NPCScript.NpcTalkVector.size() != pNPCTalk->TalkID)
			{
				// error
				SetParseError("TalkID 순서가 맞지 않습니다.", pNPCTalk->TalkString);
				break;
			}

			g_NPCScript.NpcTalkVector.push_back(pNPCTalk);
		}
		;

statementcontent:
		OBRACE_TOK talk_or_menus EBRACE_TOK
		;

talk_or_menus:
		|
		talk_or_menus talk_or_menu SEMICOLON_TOK
		;

talk_or_menu:
		TALK_TOK INTEGER_TOK QSTRING_TOK
		{
			g_NPCTalk.TalkID = $2;
			int str_len = strlen((char*)$3);
			g_NPCTalk.TalkString = new char[str_len];
			strncpy(g_NPCTalk.TalkString, (char*)$3, str_len-1);
			g_NPCTalk.TalkString[str_len-2] = '\0';
		}
		|
		MENU_TOK INTEGER_TOK QSTRING_TOK
		{
			int str_len = strlen((char*)$3);
			char *pTmpString = new char[str_len];
			strncpy(pTmpString, (char*)$3, str_len-1);
			pTmpString[str_len-2] = '\0';

			if (g_NPCTalk.MenuVector.size() != $2)
			{
				// error
				SetParseError("MenuID 순서가 맞지 않습니다.", pTmpString);
				break;
			}

			g_NPCTalk.MenuVector.push_back(pTmpString);
		}
		;

conditions:
		OPAREN_TOK condition EPAREN_TOK
		;

condition:
		condition AND_TOK condition
		|
		predicate
		;

predicate:
		cond_var comparison WORD_TOK
		|
		cond_var comparison INTEGER_TOK
		|
		cond_var comparison INTEGER_TOK OPAREN_TOK INTEGER_TOK EPAREN_TOK
		;

cond_var:
		LEVEL_TOK
		|
		UNITKIND_TOK
		|
		ITEM_TOK
		|
		QUEST_TOK
		;

comparison:
		GT_TOK
		|
		LT_TOK
		|
		GE_TOK
		|
		LE_TOK
		|
		EQUAL_TOK
		;

%%

extern "C"
{
int
NpcScriptLoaderwrap()
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
NpcScriptLoadererror(const char *s)
{
    cerr << "Syntax error: " << s << " at line: " << g_line_number << endl;
	return 0;
}
