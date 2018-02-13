
%{
#pragma warning(disable:4786)

#ifndef _ATUM_SERVER
#undef DBGOUT
#define DBGOUT (void(0))
#endif // _ATUM_SERVER

#include <stdlib.h>
#include "NpcScriptLoader.cpp.h"
//#undef YY_INPUT
//#define YY_INPUT(buf,result,max_size) \
//    { \
//    int c = getchar(); \
//    result = (c == EOF) ? YY_NULL : (buf[0] = c, 1); \
//    }

extern int g_line_number;

%}

LETTER			[A-Za-z]
DIGIT			[0-9]
INTEGER_NUM		[0-9]+
FLOAT_NUM		-?[0-9]*\.[0-9]+
HYPHEN			\-
UNDERSCORE		\_
DOT				\.
DOUBLE_DOT		\.\.
COMMA			\,
COLON			\:
SEMICOLON		;
ALPHANUMERIC	[A-Za-z0-9.,+*()/$-]
WHITE_SPACE		[ \t\r]+
LPAREN			\(
RPAREN			\)
LBRACE			\{
RBRACE			\}
DOUBLE_VAL		([0-9]+|([0-9]*\.[0-9]+)([eE][-+]?[0-9]+)?)
VARIABLE		[A-Za-z]

%%
#.*$					/* ignore comment */;
{INTEGER_NUM}			NpcScriptLoaderlval = atoi(NpcScriptLoadertext); return INTEGER_TOK;
script                  return SCRIPT_TOK;
case					return CASE_TOK;
default					return DEFAULT_TOK;
talk					return TALK_TOK;
menu					return MENU_TOK;
level					return LEVEL_TOK;
unitkind				return UNITKIND_TOK;
item					return ITEM_TOK;
quest					return QUEST_TOK;
"&&"					return AND_TOK;
">"						return GT_TOK;
"<"						return LT_TOK;
">="					return GE_TOK;
"<="					return LE_TOK;
"=="					return EQUAL_TOK;
[a-zA-Z][a-zA-Z0-9]*    NpcScriptLoaderlval = (int)NpcScriptLoadertext; return WORD_TOK;
'([^'\n]|'')*'			{
	NpcScriptLoaderlval = (int)(++NpcScriptLoadertext);
	return QSTRING_TOK;
}
\(                      return OPAREN_TOK;
\)                      return EPAREN_TOK;
\{                      return OBRACE_TOK;
\}                      return EBRACE_TOK;
;                       return SEMICOLON_TOK;
\n                      {
	/* ignore EOL */;
	g_line_number++;
}
[ \t]+                  /* ignore whitespace */;
.						return NpcScriptLoadertext[0];
%%
