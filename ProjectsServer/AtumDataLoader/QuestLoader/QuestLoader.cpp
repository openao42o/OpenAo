
/*  A Bison parser, made from QuestLoader.y
 by  GNU Bison version 1.25
  */

#define YYBISON 1  /* Identify Bison output.  */

#define alloca

#define yyparse QuestLoaderparse
#define yylex QuestLoaderlex
#define yyerror QuestLoadererror
#define yylval QuestLoaderlval
#define yychar QuestLoaderchar
#define yydebug QuestLoaderdebug
#define yynerrs QuestLoadernerrs
#define	QUEST_TOK	258
#define	BASIC_INFO_TOK	259
#define	QUEST_INDEX_TOK	260
#define	QUEST_NAME_TOK	261
#define	QUEST_TYPE_TOK	262
#define	QUEST_START_TYPE_TOK	263
#define	QUEST_END_TYPE_TOK	264
#define	NPC_INDEX_TOK	265
#define	NPC_NAME_TOK	266
#define	PRE_TALK_TOK	267
#define	FAIL_TALK_TOK	268
#define	SUCCESS_TALK_TOK	269
#define	NEXT_QUEST_INDEX_TOK	270
#define	START_REQ_TOK	271
#define	S_REQ_UNIT_KIND_TOK	272
#define	S_REQ_LEVEL_TOK	273
#define	S_REQ_ATTACK_PART_TOK	274
#define	S_REQ_ENDURANCE_PART_TOK	275
#define	S_REQ_FUEL_PART_TOK	276
#define	S_REQ_SOUL_PART_TOK	277
#define	S_REQ_DEFENSE_PART_TOK	278
#define	S_REQ_DODGE_PART_TOK	279
#define	S_REQ_ITEM_TOK	280
#define	S_REQ_QUEST_INDEX_TOK	281
#define	START_ACTION_TOK	282
#define	S_PAY_ITEM_TOK	283
#define	S_DEL_ITEM_TOK	284
#define	COMPLETION_REQ_TOK	285
#define	C_REQ_MEET_NPC_INDEX_TOK	286
#define	C_REQ_ITEM_TOK	287
#define	COMPLETION_ACTION_TOK	288
#define	C_PAY_BONUS_STAT_TOK	289
#define	C_PAY_BONUS_SKILL_POINT_TOK	290
#define	C_PAY_FAME_TOK	291
#define	C_PAY_STATUS_TOK	292
#define	C_PAY_LEVEL_TOK	293
#define	C_PAY_EXPERIENCE_TOK	294
#define	C_PAY_WARP_MAP_INDEX_TOK	295
#define	C_DEL_ITEM_TOK	296
#define	C_PAY_ITEM_TOK	297
#define	FROM_TOK	298
#define	MONSTER_TOK	299
#define	ALL_TOK	300
#define	GT_TOK	301
#define	LT_TOK	302
#define	GE_TOK	303
#define	LE_TOK	304
#define	EQUAL_TOK	305
#define	USER_VARIABLE_TOK	306
#define	INTEGER_TOK	307
#define	WORD_TOK	308
#define	QSTRING_TOK	309

#line 2 "QuestLoader.y"

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

#line 20 "QuestLoader.y"
typedef union
{
	int				integer;
	char			*string;
	ITEM_W_COUNT	item_w_count;
	INT_RANGE		integer_range;
} YYSTYPE;
#include <stdio.h>

#ifndef __cplusplus
#ifndef __STDC__
#define const
#endif
#endif



#define	YYFINAL		234
#define	YYFLAG		32768
#define	YYNTBASE	67

#define YYTRANSLATE(x) ((unsigned)(x) <= 309 ? yytranslate[x] : 79)

static const char yytranslate[] = {     0,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,    59,     2,    62,
    63,    58,    56,     2,    55,     2,    57,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,    61,     2,
    60,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,    64,     2,    65,    66,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     1,     2,     3,     4,     5,
     6,     7,     8,     9,    10,    11,    12,    13,    14,    15,
    16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
    26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
    36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
    46,    47,    48,    49,    50,    51,    52,    53,    54
};

#if YYDEBUG != 0
static const short yyprhs[] = {     0,
     0,     2,     5,     7,     9,    14,    19,    24,    33,    35,
    38,    44,    49,    55,    60,    66,    71,    77,    82,    88,
    93,    95,    98,   103,   108,   113,   118,   123,   128,   133,
   138,   143,   148,   153,   158,   163,   168,   173,   178,   183,
   188,   193,   198,   203,   208,   213,   218,   226,   231,   236,
   241,   246,   251,   256,   261,   266,   271,   275,   281,   286,
   291,   295,   299,   303,   307,   311,   315,   317,   319,   321,
   323,   328
};

static const short yyrhs[] = {    68,
     0,    67,    68,     0,    69,     0,    70,     0,    51,    60,
    52,    61,     0,    51,    60,    54,    61,     0,    51,    60,
    51,    61,     0,     3,    62,    52,    63,    64,    71,    65,
    61,     0,    72,     0,    71,    72,     0,     4,    64,    73,
    65,    61,     0,     4,    64,    65,    61,     0,    16,    64,
    73,    65,    61,     0,    16,    64,    65,    61,     0,    27,
    64,    73,    65,    61,     0,    27,    64,    65,    61,     0,
    30,    64,    73,    65,    61,     0,    30,    64,    65,    61,
     0,    33,    64,    73,    65,    61,     0,    33,    64,    65,
    61,     0,    74,     0,    73,    74,     0,     5,    60,    76,
    61,     0,     6,    60,    77,    61,     0,     7,    60,    76,
    61,     0,     8,    60,    76,    61,     0,     9,    60,    76,
    61,     0,    10,    60,    76,    61,     0,    11,    60,    77,
    61,     0,    12,    60,    77,    61,     0,    13,    60,    77,
    61,     0,    14,    60,    77,    61,     0,    15,    60,    76,
    61,     0,    17,    60,    76,    61,     0,    18,    60,    75,
    61,     0,    19,    60,    75,    61,     0,    20,    60,    75,
    61,     0,    21,    60,    75,    61,     0,    22,    60,    75,
    61,     0,    23,    60,    75,    61,     0,    24,    60,    75,
    61,     0,    25,    60,    78,    61,     0,    26,    60,    76,
    61,     0,    28,    60,    78,    61,     0,    29,    60,    78,
    61,     0,    31,    60,    76,    61,     0,    32,    60,    78,
    43,    44,    76,    61,     0,    34,    60,    76,    61,     0,
    35,    60,    76,    61,     0,    36,    60,    76,    61,     0,
    37,    60,    76,    61,     0,    38,    60,    76,    61,     0,
    39,    60,    76,    61,     0,    40,    60,    76,    61,     0,
    41,    60,    78,    61,     0,    42,    60,    78,    61,     0,
    62,    76,    63,     0,    62,    76,    66,    76,    63,     0,
    62,    76,    66,    63,     0,    62,    66,    76,    63,     0,
    76,    56,    76,     0,    76,    55,    76,     0,    76,    58,
    76,     0,    76,    57,    76,     0,    76,    59,    76,     0,
    62,    76,    63,     0,    52,     0,    51,     0,    54,     0,
    51,     0,    76,    62,    76,    63,     0,    76,    62,    45,
    63,     0
};

#endif

#if YYDEBUG != 0
static const short yyrline[] = { 0,
   105,   107,   111,   113,   117,   125,   134,   158,   169,   171,
   175,   180,   182,   184,   186,   188,   190,   192,   194,   196,
   200,   202,   206,   212,   219,   224,   229,   234,   239,   246,
   253,   260,   267,   272,   278,   283,   288,   293,   298,   303,
   308,   313,   318,   323,   329,   334,   340,   345,   351,   356,
   361,   366,   371,   376,   381,   386,   393,   399,   405,   411,
   419,   421,   423,   425,   433,   435,   437,   439,   443,   445,
   449,   455
};
#endif


#if YYDEBUG != 0 || defined (YYERROR_VERBOSE)

static const char * const yytname[] = {   "$","error","$undefined.","QUEST_TOK",
"BASIC_INFO_TOK","QUEST_INDEX_TOK","QUEST_NAME_TOK","QUEST_TYPE_TOK","QUEST_START_TYPE_TOK",
"QUEST_END_TYPE_TOK","NPC_INDEX_TOK","NPC_NAME_TOK","PRE_TALK_TOK","FAIL_TALK_TOK",
"SUCCESS_TALK_TOK","NEXT_QUEST_INDEX_TOK","START_REQ_TOK","S_REQ_UNIT_KIND_TOK",
"S_REQ_LEVEL_TOK","S_REQ_ATTACK_PART_TOK","S_REQ_ENDURANCE_PART_TOK","S_REQ_FUEL_PART_TOK",
"S_REQ_SOUL_PART_TOK","S_REQ_DEFENSE_PART_TOK","S_REQ_DODGE_PART_TOK","S_REQ_ITEM_TOK",
"S_REQ_QUEST_INDEX_TOK","START_ACTION_TOK","S_PAY_ITEM_TOK","S_DEL_ITEM_TOK",
"COMPLETION_REQ_TOK","C_REQ_MEET_NPC_INDEX_TOK","C_REQ_ITEM_TOK","COMPLETION_ACTION_TOK",
"C_PAY_BONUS_STAT_TOK","C_PAY_BONUS_SKILL_POINT_TOK","C_PAY_FAME_TOK","C_PAY_STATUS_TOK",
"C_PAY_LEVEL_TOK","C_PAY_EXPERIENCE_TOK","C_PAY_WARP_MAP_INDEX_TOK","C_DEL_ITEM_TOK",
"C_PAY_ITEM_TOK","FROM_TOK","MONSTER_TOK","ALL_TOK","GT_TOK","LT_TOK","GE_TOK",
"LE_TOK","EQUAL_TOK","USER_VARIABLE_TOK","INTEGER_TOK","WORD_TOK","QSTRING_TOK",
"'-'","'+'","'/'","'*'","'&'","'='","';'","'('","')'","'{'","'}'","'~'","command_list",
"command","user_variable_definition","quest_definition","quest_content_list",
"quest_content","statement_list","statement","integer_range_value","integer_expression",
"string_expression","item_w_count_value", NULL
};
#endif

static const short yyr1[] = {     0,
    67,    67,    68,    68,    69,    69,    69,    70,    71,    71,
    72,    72,    72,    72,    72,    72,    72,    72,    72,    72,
    73,    73,    74,    74,    74,    74,    74,    74,    74,    74,
    74,    74,    74,    74,    74,    74,    74,    74,    74,    74,
    74,    74,    74,    74,    74,    74,    74,    74,    74,    74,
    74,    74,    74,    74,    74,    74,    75,    75,    75,    75,
    76,    76,    76,    76,    76,    76,    76,    76,    77,    77,
    78,    78
};

static const short yyr2[] = {     0,
     1,     2,     1,     1,     4,     4,     4,     8,     1,     2,
     5,     4,     5,     4,     5,     4,     5,     4,     5,     4,
     1,     2,     4,     4,     4,     4,     4,     4,     4,     4,
     4,     4,     4,     4,     4,     4,     4,     4,     4,     4,
     4,     4,     4,     4,     4,     4,     7,     4,     4,     4,
     4,     4,     4,     4,     4,     4,     3,     5,     4,     4,
     3,     3,     3,     3,     3,     3,     1,     1,     1,     1,
     4,     4
};

static const short yydefact[] = {     0,
     0,     0,     0,     1,     3,     4,     0,     0,     2,     0,
     0,     0,     0,     0,     7,     5,     6,     0,     0,     0,
     0,     0,     0,     0,     9,     0,     0,     0,     0,     0,
     0,    10,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,    21,     0,
     0,     0,     0,     0,     0,     0,     0,     8,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,    12,     0,    22,    14,     0,    16,     0,    18,
     0,    20,     0,    68,    67,     0,     0,    70,    69,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,    11,    13,    15,    17,    19,     0,
     0,     0,     0,     0,     0,    23,    24,    25,    26,    27,
    28,    29,    30,    31,    32,    33,    34,     0,     0,    35,
    36,    37,    38,    39,    40,    41,     0,    42,    43,    44,
    45,    46,     0,    48,    49,    50,    51,    52,    53,    54,
    55,    56,    66,    62,    61,    64,    63,    65,     0,    57,
     0,     0,     0,     0,    60,    59,     0,    72,    71,     0,
    58,    47,     0,     0
};

static const short yydefgoto[] = {     3,
     4,     5,     6,    24,    25,    68,    69,   142,   149,   130,
   150
};

static const short yypact[] = {    -2,
   -56,   -52,    12,-32768,-32768,-32768,   -15,    24,-32768,   -20,
     3,     8,    19,     9,-32768,-32768,-32768,    18,    25,    26,
    43,    44,    46,     5,-32768,   115,   153,   191,   229,   267,
    55,-32768,   -14,    57,    71,    88,   109,   123,   126,   147,
   161,   164,   185,   199,   202,   223,   237,   240,   261,   275,
   278,   299,   313,   316,   337,   351,   354,   375,   389,   392,
   413,   427,   430,   446,   447,   455,   444,   305,-32768,   453,
   343,   467,   381,   474,   419,   481,   457,-32768,    20,   -18,
    20,    20,    20,    20,   -18,   -18,   -18,   -18,    20,    20,
   487,   487,   487,   487,   487,   487,   487,    20,    20,    20,
    20,    20,    20,    20,    20,    20,    20,    20,    20,    20,
    20,    20,-32768,   495,-32768,-32768,   502,-32768,   509,-32768,
   516,-32768,   523,-32768,-32768,    20,   468,-32768,-32768,   530,
   475,   482,   489,   496,   537,   544,   551,   558,   503,   510,
   -12,   565,   572,   579,   587,   588,   589,   590,    29,   591,
   517,   592,   593,   524,    63,   531,   538,   545,   552,   559,
   566,   573,   594,   595,-32768,-32768,-32768,-32768,-32768,    42,
    20,    20,    20,    20,    20,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,    20,     2,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,   -38,-32768,-32768,-32768,
-32768,-32768,   472,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,    45,    45,   598,   598,-32768,    56,-32768,
     4,   596,   445,    20,-32768,-32768,   454,-32768,-32768,   580,
-32768,-32768,   658,-32768
};

static const short yypgoto[] = {-32768,
   657,-32768,-32768,-32768,   637,   -11,     6,   550,   -79,   433,
   -59
};


#define	YYLAST		661


static const short yytable[] = {   127,
     1,   131,   132,   133,   134,     7,   222,     8,    19,   139,
   140,   233,   124,   125,     1,    71,    73,    75,    77,   151,
    20,    19,   154,   126,   156,   157,   158,   159,   160,   161,
   162,    21,   128,    20,    22,   129,    10,    23,   124,   125,
   152,   153,    14,   155,    21,    79,   170,    22,     2,   126,
    23,   163,   164,   188,   124,   125,   171,   172,   173,   174,
   175,   189,     2,    15,   220,   126,   226,   221,    16,    31,
   124,   125,    18,   115,    11,    12,   115,    13,   115,    17,
   115,   126,   115,   171,   172,   173,   174,   175,    26,    27,
   197,   214,   215,   216,   217,   218,   171,   172,   173,   174,
   175,   173,   174,   175,   213,   203,    28,    29,   219,    30,
   171,   172,   173,   174,   175,    78,    80,   223,   225,    33,
    34,    35,    36,    37,    38,    39,    40,    41,    42,    43,
    81,    44,    45,    46,    47,    48,    49,    50,    51,    52,
    53,   227,    54,    55,   230,    56,    57,    82,    58,    59,
    60,    61,    62,    63,    64,    65,    66,    33,    34,    35,
    36,    37,    38,    39,    40,    41,    42,    43,    83,    44,
    45,    46,    47,    48,    49,    50,    51,    52,    53,    67,
    54,    55,    84,    56,    57,    85,    58,    59,    60,    61,
    62,    63,    64,    65,    66,    33,    34,    35,    36,    37,
    38,    39,    40,    41,    42,    43,    86,    44,    45,    46,
    47,    48,    49,    50,    51,    52,    53,    70,    54,    55,
    87,    56,    57,    88,    58,    59,    60,    61,    62,    63,
    64,    65,    66,    33,    34,    35,    36,    37,    38,    39,
    40,    41,    42,    43,    89,    44,    45,    46,    47,    48,
    49,    50,    51,    52,    53,    72,    54,    55,    90,    56,
    57,    91,    58,    59,    60,    61,    62,    63,    64,    65,
    66,    33,    34,    35,    36,    37,    38,    39,    40,    41,
    42,    43,    92,    44,    45,    46,    47,    48,    49,    50,
    51,    52,    53,    74,    54,    55,    93,    56,    57,    94,
    58,    59,    60,    61,    62,    63,    64,    65,    66,    33,
    34,    35,    36,    37,    38,    39,    40,    41,    42,    43,
    95,    44,    45,    46,    47,    48,    49,    50,    51,    52,
    53,    76,    54,    55,    96,    56,    57,    97,    58,    59,
    60,    61,    62,    63,    64,    65,    66,    33,    34,    35,
    36,    37,    38,    39,    40,    41,    42,    43,    98,    44,
    45,    46,    47,    48,    49,    50,    51,    52,    53,   114,
    54,    55,    99,    56,    57,   100,    58,    59,    60,    61,
    62,    63,    64,    65,    66,    33,    34,    35,    36,    37,
    38,    39,    40,    41,    42,    43,   101,    44,    45,    46,
    47,    48,    49,    50,    51,    52,    53,   117,    54,    55,
   102,    56,    57,   103,    58,    59,    60,    61,    62,    63,
    64,    65,    66,    33,    34,    35,    36,    37,    38,    39,
    40,    41,    42,    43,   104,    44,    45,    46,    47,    48,
    49,    50,    51,    52,    53,   119,    54,    55,   105,    56,
    57,   106,    58,    59,    60,    61,    62,    63,    64,    65,
    66,    33,    34,    35,    36,    37,    38,    39,    40,    41,
    42,    43,   107,    44,    45,    46,    47,    48,    49,    50,
    51,    52,    53,   121,    54,    55,   108,    56,    57,   109,
    58,    59,    60,    61,    62,    63,    64,    65,    66,   171,
   172,   173,   174,   175,   113,   110,   111,   229,   171,   172,
   173,   174,   175,   116,   112,   224,   231,   135,   136,   137,
   138,   123,   171,   172,   173,   174,   175,   118,   176,   171,
   172,   173,   174,   175,   120,   178,   171,   172,   173,   174,
   175,   122,   179,   171,   172,   173,   174,   175,   141,   180,
   171,   172,   173,   174,   175,   165,   181,   171,   172,   173,
   174,   175,   166,   186,   171,   172,   173,   174,   175,   167,
   187,   171,   172,   173,   174,   175,   168,   199,   171,   172,
   173,   174,   175,   169,   202,   171,   172,   173,   174,   175,
   177,   204,   171,   172,   173,   174,   175,   182,   205,   171,
   172,   173,   174,   175,   183,   206,   171,   172,   173,   174,
   175,   184,   207,   171,   172,   173,   174,   175,   185,   208,
   171,   172,   173,   174,   175,   190,   209,   171,   172,   173,
   174,   175,   191,   210,   171,   172,   173,   174,   175,   192,
   232,   143,   144,   145,   146,   147,   148,   193,   194,   195,
   196,   198,   200,   201,   211,   212,   175,   234,   228,     9,
    32
};

static const short yycheck[] = {    79,
     3,    81,    82,    83,    84,    62,    45,    60,     4,    89,
    90,     0,    51,    52,     3,    27,    28,    29,    30,    99,
    16,     4,   102,    62,   104,   105,   106,   107,   108,   109,
   110,    27,    51,    16,    30,    54,    52,    33,    51,    52,
   100,   101,    63,   103,    27,    60,   126,    30,    51,    62,
    33,   111,   112,    66,    51,    52,    55,    56,    57,    58,
    59,   141,    51,    61,    63,    62,    63,    66,    61,    65,
    51,    52,    64,    68,    51,    52,    71,    54,    73,    61,
    75,    62,    77,    55,    56,    57,    58,    59,    64,    64,
    62,   171,   172,   173,   174,   175,    55,    56,    57,    58,
    59,    57,    58,    59,    63,    43,    64,    64,   188,    64,
    55,    56,    57,    58,    59,    61,    60,   197,    63,     5,
     6,     7,     8,     9,    10,    11,    12,    13,    14,    15,
    60,    17,    18,    19,    20,    21,    22,    23,    24,    25,
    26,   221,    28,    29,   224,    31,    32,    60,    34,    35,
    36,    37,    38,    39,    40,    41,    42,     5,     6,     7,
     8,     9,    10,    11,    12,    13,    14,    15,    60,    17,
    18,    19,    20,    21,    22,    23,    24,    25,    26,    65,
    28,    29,    60,    31,    32,    60,    34,    35,    36,    37,
    38,    39,    40,    41,    42,     5,     6,     7,     8,     9,
    10,    11,    12,    13,    14,    15,    60,    17,    18,    19,
    20,    21,    22,    23,    24,    25,    26,    65,    28,    29,
    60,    31,    32,    60,    34,    35,    36,    37,    38,    39,
    40,    41,    42,     5,     6,     7,     8,     9,    10,    11,
    12,    13,    14,    15,    60,    17,    18,    19,    20,    21,
    22,    23,    24,    25,    26,    65,    28,    29,    60,    31,
    32,    60,    34,    35,    36,    37,    38,    39,    40,    41,
    42,     5,     6,     7,     8,     9,    10,    11,    12,    13,
    14,    15,    60,    17,    18,    19,    20,    21,    22,    23,
    24,    25,    26,    65,    28,    29,    60,    31,    32,    60,
    34,    35,    36,    37,    38,    39,    40,    41,    42,     5,
     6,     7,     8,     9,    10,    11,    12,    13,    14,    15,
    60,    17,    18,    19,    20,    21,    22,    23,    24,    25,
    26,    65,    28,    29,    60,    31,    32,    60,    34,    35,
    36,    37,    38,    39,    40,    41,    42,     5,     6,     7,
     8,     9,    10,    11,    12,    13,    14,    15,    60,    17,
    18,    19,    20,    21,    22,    23,    24,    25,    26,    65,
    28,    29,    60,    31,    32,    60,    34,    35,    36,    37,
    38,    39,    40,    41,    42,     5,     6,     7,     8,     9,
    10,    11,    12,    13,    14,    15,    60,    17,    18,    19,
    20,    21,    22,    23,    24,    25,    26,    65,    28,    29,
    60,    31,    32,    60,    34,    35,    36,    37,    38,    39,
    40,    41,    42,     5,     6,     7,     8,     9,    10,    11,
    12,    13,    14,    15,    60,    17,    18,    19,    20,    21,
    22,    23,    24,    25,    26,    65,    28,    29,    60,    31,
    32,    60,    34,    35,    36,    37,    38,    39,    40,    41,
    42,     5,     6,     7,     8,     9,    10,    11,    12,    13,
    14,    15,    60,    17,    18,    19,    20,    21,    22,    23,
    24,    25,    26,    65,    28,    29,    60,    31,    32,    60,
    34,    35,    36,    37,    38,    39,    40,    41,    42,    55,
    56,    57,    58,    59,    61,    60,    60,    63,    55,    56,
    57,    58,    59,    61,    60,    44,    63,    85,    86,    87,
    88,    65,    55,    56,    57,    58,    59,    61,    61,    55,
    56,    57,    58,    59,    61,    61,    55,    56,    57,    58,
    59,    61,    61,    55,    56,    57,    58,    59,    62,    61,
    55,    56,    57,    58,    59,    61,    61,    55,    56,    57,
    58,    59,    61,    61,    55,    56,    57,    58,    59,    61,
    61,    55,    56,    57,    58,    59,    61,    61,    55,    56,
    57,    58,    59,    61,    61,    55,    56,    57,    58,    59,
    61,    61,    55,    56,    57,    58,    59,    61,    61,    55,
    56,    57,    58,    59,    61,    61,    55,    56,    57,    58,
    59,    61,    61,    55,    56,    57,    58,    59,    61,    61,
    55,    56,    57,    58,    59,    61,    61,    55,    56,    57,
    58,    59,    61,    61,    55,    56,    57,    58,    59,    61,
    61,    92,    93,    94,    95,    96,    97,    61,    61,    61,
    61,    61,    61,    61,    61,    61,    59,     0,    63,     3,
    24
};
/* -*-C-*-  Note some compilers choke on comments on `#line' lines.  */

/* Skeleton output parser for bison,
   Copyright (C) 1984, 1989, 1990 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.  */

/* As a special exception, when this file is copied by Bison into a
   Bison output file, you may use that output file without restriction.
   This special exception was added by the Free Software Foundation
   in version 1.24 of Bison.  */

#ifndef alloca
#ifdef __GNUC__
#define alloca __builtin_alloca
#else /* not GNU C.  */
#if (!defined (__STDC__) && defined (sparc)) || defined (__sparc__) || defined (__sparc) || defined (__sgi)
#include <alloca.h>
#else /* not sparc */
#if defined (MSDOS) && !defined (__TURBOC__)
#include <malloc.h>
#else /* not MSDOS, or __TURBOC__ */
#if defined(_AIX)
#include <malloc.h>
 #pragma alloca
#else /* not MSDOS, __TURBOC__, or _AIX */
#ifdef __hpux
#ifdef __cplusplus
extern "C" {
void *alloca (unsigned int);
};
#else /* not __cplusplus */
void *alloca ();
#endif /* not __cplusplus */
#endif /* __hpux */
#endif /* not _AIX */
#endif /* not MSDOS, or __TURBOC__ */
#endif /* not sparc.  */
#endif /* not GNU C.  */
#endif /* alloca not defined.  */

/* This is the parser code that is written into each bison parser
  when the %semantic_parser declaration is not specified in the grammar.
  It was written by Richard Stallman by simplifying the hairy parser
  used when %semantic_parser is specified.  */

/* Note: there must be only one dollar sign in this file.
   It is replaced by the list of actions, each action
   as one case of the switch.  */

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		-2
#define YYEOF		0
#define YYACCEPT	return(0)
#define YYABORT 	return(1)
#define YYERROR		goto yyerrlab1
/* Like YYERROR except do call yyerror.
   This remains here temporarily to ease the
   transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */
#define YYFAIL		goto yyerrlab
#define YYRECOVERING()  (!!yyerrstatus)
#define YYBACKUP(token, value) \
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    { yychar = (token), yylval = (value);			\
      yychar1 = YYTRANSLATE (yychar);				\
      YYPOPSTACK;						\
      goto yybackup;						\
    }								\
  else								\
    { yyerror ("syntax error: cannot back up"); YYERROR; }	\
while (0)

#define YYTERROR	1
#define YYERRCODE	256

#ifndef YYPURE
#define YYLEX		yylex()
#endif

#ifdef YYPURE
#ifdef YYLSP_NEEDED
#ifdef YYLEX_PARAM
#define YYLEX		yylex(&yylval, &yylloc, YYLEX_PARAM)
#else
#define YYLEX		yylex(&yylval, &yylloc)
#endif
#else /* not YYLSP_NEEDED */
#ifdef YYLEX_PARAM
#define YYLEX		yylex(&yylval, YYLEX_PARAM)
#else
#define YYLEX		yylex(&yylval)
#endif
#endif /* not YYLSP_NEEDED */
#endif

/* If nonreentrant, generate the variables here */

#ifndef YYPURE

int	yychar;			/*  the lookahead symbol		*/
YYSTYPE	yylval;			/*  the semantic value of the		*/
				/*  lookahead symbol			*/

#ifdef YYLSP_NEEDED
YYLTYPE yylloc;			/*  location data for the lookahead	*/
				/*  symbol				*/
#endif

int yynerrs;			/*  number of parse errors so far       */
#endif  /* not YYPURE */

#if YYDEBUG != 0
int yydebug;			/*  nonzero means print parse trace	*/
/* Since this is uninitialized, it does not stop multiple parsers
   from coexisting.  */
#endif

/*  YYINITDEPTH indicates the initial size of the parser's stacks	*/

#ifndef	YYINITDEPTH
#define YYINITDEPTH 200
#endif

/*  YYMAXDEPTH is the maximum size the stacks can grow to
    (effective only if the built-in stack extension method is used).  */

#if YYMAXDEPTH == 0
#undef YYMAXDEPTH
#endif

#ifndef YYMAXDEPTH
#define YYMAXDEPTH 10000
#endif

/* Prevent warning if -Wstrict-prototypes.  */
#ifdef __GNUC__
int yyparse (void);
#endif

#if __GNUC__ > 1		/* GNU C and GNU C++ define this.  */
#define __yy_memcpy(TO,FROM,COUNT)	__builtin_memcpy(TO,FROM,COUNT)
#else				/* not GNU C or C++ */
#ifndef __cplusplus

/* This is the most reliable way to avoid incompatibilities
   in available built-in functions on various systems.  */
static void
__yy_memcpy (to, from, count)
     char *to;
     char *from;
     int count;
{
  register char *f = from;
  register char *t = to;
  register int i = count;

  while (i-- > 0)
    *t++ = *f++;
}

#else /* __cplusplus */

/* This is the most reliable way to avoid incompatibilities
   in available built-in functions on various systems.  */
static void
__yy_memcpy (char *to, char *from, int count)
{
  register char *f = from;
  register char *t = to;
  register int i = count;

  while (i-- > 0)
    *t++ = *f++;
}

#endif
#endif


/* The user can define YYPARSE_PARAM as the name of an argument to be passed
   into yyparse.  The argument should have type void *.
   It should actually point to an object.
   Grammar actions can access the variable by casting it
   to the proper pointer type.  */

#ifdef YYPARSE_PARAM
#ifdef __cplusplus
#define YYPARSE_PARAM_ARG void *YYPARSE_PARAM
#define YYPARSE_PARAM_DECL
#else /* not __cplusplus */
#define YYPARSE_PARAM_ARG YYPARSE_PARAM
#define YYPARSE_PARAM_DECL void *YYPARSE_PARAM;
#endif /* not __cplusplus */
#else /* not YYPARSE_PARAM */
#define YYPARSE_PARAM_ARG
#define YYPARSE_PARAM_DECL
#endif /* not YYPARSE_PARAM */

int
yyparse(YYPARSE_PARAM_ARG)
     YYPARSE_PARAM_DECL
{
  register int yystate;
  register int yyn;
  register short *yyssp;
  register YYSTYPE *yyvsp;
  int yyerrstatus;	/*  number of tokens to shift before error messages enabled */
  int yychar1 = 0;		/*  lookahead token as an internal (translated) token number */

  short	yyssa[YYINITDEPTH];	/*  the state stack			*/
  YYSTYPE yyvsa[YYINITDEPTH];	/*  the semantic value stack		*/

  short *yyss = yyssa;		/*  refer to the stacks thru separate pointers */
  YYSTYPE *yyvs = yyvsa;	/*  to allow yyoverflow to reallocate them elsewhere */

#ifdef YYLSP_NEEDED
  YYLTYPE yylsa[YYINITDEPTH];	/*  the location stack			*/
  YYLTYPE *yyls = yylsa;
  YYLTYPE *yylsp;

#define YYPOPSTACK   (yyvsp--, yyssp--, yylsp--)
#else
#define YYPOPSTACK   (yyvsp--, yyssp--)
#endif

  int yystacksize = YYINITDEPTH;

#ifdef YYPURE
  int yychar;
  YYSTYPE yylval;
  int yynerrs;
#ifdef YYLSP_NEEDED
  YYLTYPE yylloc;
#endif
#endif

  YYSTYPE yyval;		/*  the variable used to return		*/
				/*  semantic values from the action	*/
				/*  routines				*/

  int yylen;

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Starting parse\n");
#endif

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss - 1;
  yyvsp = yyvs;
#ifdef YYLSP_NEEDED
  yylsp = yyls;
#endif

/* Push a new state, which is found in  yystate  .  */
/* In all cases, when you get here, the value and location stacks
   have just been pushed. so pushing a state here evens the stacks.  */
yynewstate:

  *++yyssp = yystate;

  if (yyssp >= yyss + yystacksize - 1)
    {
      /* Give user a chance to reallocate the stack */
      /* Use copies of these so that the &'s don't force the real ones into memory. */
      YYSTYPE *yyvs1 = yyvs;
      short *yyss1 = yyss;
#ifdef YYLSP_NEEDED
      YYLTYPE *yyls1 = yyls;
#endif

      /* Get the current used size of the three stacks, in elements.  */
      int size = yyssp - yyss + 1;

#ifdef yyoverflow
      /* Each stack pointer address is followed by the size of
	 the data in use in that stack, in bytes.  */
#ifdef YYLSP_NEEDED
      /* This used to be a conditional around just the two extra args,
	 but that might be undefined if yyoverflow is a macro.  */
      yyoverflow("parser stack overflow",
		 &yyss1, size * sizeof (*yyssp),
		 &yyvs1, size * sizeof (*yyvsp),
		 &yyls1, size * sizeof (*yylsp),
		 &yystacksize);
#else
      yyoverflow("parser stack overflow",
		 &yyss1, size * sizeof (*yyssp),
		 &yyvs1, size * sizeof (*yyvsp),
		 &yystacksize);
#endif

      yyss = yyss1; yyvs = yyvs1;
#ifdef YYLSP_NEEDED
      yyls = yyls1;
#endif
#else /* no yyoverflow */
      /* Extend the stack our own way.  */
      if (yystacksize >= YYMAXDEPTH)
	{
	  yyerror("parser stack overflow");
	  return 2;
	}
      yystacksize *= 2;
      if (yystacksize > YYMAXDEPTH)
	yystacksize = YYMAXDEPTH;
      yyss = (short *) alloca (yystacksize * sizeof (*yyssp));
      __yy_memcpy ((char *)yyss, (char *)yyss1, size * sizeof (*yyssp));
      yyvs = (YYSTYPE *) alloca (yystacksize * sizeof (*yyvsp));
      __yy_memcpy ((char *)yyvs, (char *)yyvs1, size * sizeof (*yyvsp));
#ifdef YYLSP_NEEDED
      yyls = (YYLTYPE *) alloca (yystacksize * sizeof (*yylsp));
      __yy_memcpy ((char *)yyls, (char *)yyls1, size * sizeof (*yylsp));
#endif
#endif /* no yyoverflow */

      yyssp = yyss + size - 1;
      yyvsp = yyvs + size - 1;
#ifdef YYLSP_NEEDED
      yylsp = yyls + size - 1;
#endif

#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Stack size increased to %d\n", yystacksize);
#endif

      if (yyssp >= yyss + yystacksize - 1)
	YYABORT;
    }

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Entering state %d\n", yystate);
#endif

  goto yybackup;
 yybackup:

/* Do appropriate processing given the current state.  */
/* Read a lookahead token if we need one and don't already have one.  */
/* yyresume: */

  /* First try to decide what to do without reference to lookahead token.  */

  yyn = yypact[yystate];
  if (yyn == YYFLAG)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* yychar is either YYEMPTY or YYEOF
     or a valid token in external form.  */

  if (yychar == YYEMPTY)
    {
#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Reading a token: ");
#endif
      yychar = YYLEX;
    }

  /* Convert token to internal form (in yychar1) for indexing tables with */

  if (yychar <= 0)		/* This means end of input. */
    {
      yychar1 = 0;
      yychar = YYEOF;		/* Don't call YYLEX any more */

#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Now at end of input.\n");
#endif
    }
  else
    {
      yychar1 = YYTRANSLATE(yychar);

#if YYDEBUG != 0
      if (yydebug)
	{
	  fprintf (stderr, "Next token is %d (%s", yychar, yytname[yychar1]);
	  /* Give the individual parser a way to print the precise meaning
	     of a token, for further debugging info.  */
#ifdef YYPRINT
	  YYPRINT (stderr, yychar, yylval);
#endif
	  fprintf (stderr, ")\n");
	}
#endif
    }

  yyn += yychar1;
  if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != yychar1)
    goto yydefault;

  yyn = yytable[yyn];

  /* yyn is what to do for this token type in this state.
     Negative => reduce, -yyn is rule number.
     Positive => shift, yyn is new state.
       New state is final state => don't bother to shift,
       just return success.
     0, or most negative number => error.  */

  if (yyn < 0)
    {
      if (yyn == YYFLAG)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }
  else if (yyn == 0)
    goto yyerrlab;

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Shift the lookahead token.  */

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Shifting token %d (%s), ", yychar, yytname[yychar1]);
#endif

  /* Discard the token being shifted unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  *++yyvsp = yylval;
#ifdef YYLSP_NEEDED
  *++yylsp = yylloc;
#endif

  /* count tokens shifted since error; after three, turn off error status.  */
  if (yyerrstatus) yyerrstatus--;

  yystate = yyn;
  goto yynewstate;

/* Do the default action for the current state.  */
yydefault:

  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;

/* Do a reduction.  yyn is the number of a rule to reduce with.  */
yyreduce:
  yylen = yyr2[yyn];
  if (yylen > 0)
    yyval = yyvsp[1-yylen]; /* implement default value of the action */

#if YYDEBUG != 0
  if (yydebug)
    {
      int i;

      fprintf (stderr, "Reducing via rule %d (line %d), ",
	       yyn, yyrline[yyn]);

      /* Print the symbols being reduced, and their result.  */
      for (i = yyprhs[yyn]; yyrhs[i] > 0; i++)
	fprintf (stderr, "%s ", yytname[yyrhs[i]]);
      fprintf (stderr, " -> %s\n", yytname[yyr1[yyn]]);
    }
#endif


  switch (yyn) {

case 5:
#line 119 "QuestLoader.y"
{
			g_user_var_quest.AddUserVariableInteger(yyvsp[-3].string, yyvsp[-1].integer);
			_loader_print("user variable %s = %d\n", yyvsp[-3].string, yyvsp[-1].integer);

			delete yyvsp[-3].string;
		;
    break;}
case 6:
#line 127 "QuestLoader.y"
{
			g_user_var_quest.AddUserVariableString(yyvsp[-3].string, yyvsp[-1].string);
			_loader_print("user variable %s = %s\n", yyvsp[-3].string, yyvsp[-1].string);

			delete yyvsp[-3].string;
			delete yyvsp[-1].string;
		;
    break;}
case 7:
#line 136 "QuestLoader.y"
{
			USER_VAR_ELEMENT *pElem = g_user_var_quest.GetUserVariable(yyvsp[-1].string);
			if (pElem == NULL)
			{
				// error
				QuestLoadererror("User Variable NOT Defined");
			}

			if (pElem->Type == ET_INTEGER)
			{
				_loader_print("user variable %s = %d\n", yyvsp[-3].string, pElem->IntValue);
			}
			else if (pElem->Type == ET_STRING)
			{
				_loader_print("user variable %s = %s\n", yyvsp[-3].string, pElem->StringValue);
			}

			delete yyvsp[-3].string;
			delete yyvsp[-1].string;
		;
    break;}
case 8:
#line 160 "QuestLoader.y"
{
			g_pQuestInfoMap->insertEZ(yyvsp[-5].integer, g_quest_for_loading.Clone());
			_loader_print("quest %d defined...\n", yyvsp[-5].integer);

			// reset
			g_quest_for_loading.Reset();
		;
    break;}
case 11:
#line 177 "QuestLoader.y"
{
			// _loader_print("%s's BASIC_INFO Parsed\n", "rr");
		;
    break;}
case 23:
#line 209 "QuestLoader.y"
{
			g_quest_for_loading.QuestIndex = yyvsp[-1].integer;
		;
    break;}
case 24:
#line 214 "QuestLoader.y"
{
			strncpy(g_quest_for_loading.QuestName, yyvsp[-1].string, SIZE_MAX_QUEST_NAME);

			delete yyvsp[-1].string;
		;
    break;}
case 25:
#line 221 "QuestLoader.y"
{
			g_quest_for_loading.QuestType = yyvsp[-1].integer;
		;
    break;}
case 26:
#line 226 "QuestLoader.y"
{
			g_quest_for_loading.QuestStartType = yyvsp[-1].integer;
		;
    break;}
case 27:
#line 231 "QuestLoader.y"
{
			g_quest_for_loading.QuestEndType = yyvsp[-1].integer;
		;
    break;}
case 28:
#line 236 "QuestLoader.y"
{
			g_quest_for_loading.NPCIndex = yyvsp[-1].integer;
		;
    break;}
case 29:
#line 241 "QuestLoader.y"
{
			strncpy(g_quest_for_loading.NPCName, yyvsp[-1].string, SIZE_MAX_NPC_NAME);

			delete yyvsp[-1].string;
		;
    break;}
case 30:
#line 248 "QuestLoader.y"
{
			strncpy(g_quest_for_loading.PreTalk, yyvsp[-1].string, SIZE_MAX_NPC_TALK_STRING);

			delete yyvsp[-1].string;
		;
    break;}
case 31:
#line 255 "QuestLoader.y"
{
			strncpy(g_quest_for_loading.FailTalk, yyvsp[-1].string, SIZE_MAX_NPC_TALK_STRING);

			delete yyvsp[-1].string;
		;
    break;}
case 32:
#line 262 "QuestLoader.y"
{
			strncpy(g_quest_for_loading.SuccessTalk, yyvsp[-1].string, SIZE_MAX_NPC_TALK_STRING);

			delete yyvsp[-1].string;
		;
    break;}
case 33:
#line 269 "QuestLoader.y"
{
			g_quest_for_loading.NextQuestIndex = yyvsp[-1].integer;
		;
    break;}
case 34:
#line 275 "QuestLoader.y"
{
			g_quest_for_loading.ReqUnitKind = yyvsp[-1].integer;
		;
    break;}
case 35:
#line 280 "QuestLoader.y"
{
			g_quest_for_loading.ReqLevel = yyvsp[-1].integer_range;
		;
    break;}
case 36:
#line 285 "QuestLoader.y"
{
			g_quest_for_loading.ReqAttackPart = yyvsp[-1].integer_range;
		;
    break;}
case 37:
#line 290 "QuestLoader.y"
{
			g_quest_for_loading.ReqEndurancePart = yyvsp[-1].integer_range;
		;
    break;}
case 38:
#line 295 "QuestLoader.y"
{
			g_quest_for_loading.ReqFuelPart = yyvsp[-1].integer_range;
		;
    break;}
case 39:
#line 300 "QuestLoader.y"
{
			g_quest_for_loading.ReqSoulPart = yyvsp[-1].integer_range;
		;
    break;}
case 40:
#line 305 "QuestLoader.y"
{
			g_quest_for_loading.ReqDefensePart = yyvsp[-1].integer_range;
		;
    break;}
case 41:
#line 310 "QuestLoader.y"
{
			g_quest_for_loading.ReqDodgePart = yyvsp[-1].integer_range;
		;
    break;}
case 42:
#line 315 "QuestLoader.y"
{
			g_quest_for_loading.ReqItemVector.push_back(yyvsp[-1].item_w_count);
		;
    break;}
case 43:
#line 320 "QuestLoader.y"
{
			g_quest_for_loading.ReqQuestIndexVector.push_back(yyvsp[-1].integer);
		;
    break;}
case 44:
#line 326 "QuestLoader.y"
{
			g_quest_for_loading.StartPayItemVector.push_back(yyvsp[-1].item_w_count);
		;
    break;}
case 45:
#line 331 "QuestLoader.y"
{
			g_quest_for_loading.StartDelItemVector.push_back(yyvsp[-1].item_w_count);
		;
    break;}
case 46:
#line 337 "QuestLoader.y"
{
			g_quest_for_loading.TargetMeetNPCIndex = yyvsp[-1].integer;
		;
    break;}
case 47:
#line 342 "QuestLoader.y"
{
			g_quest_for_loading.TargetItem = yyvsp[-4].item_w_count;
		;
    break;}
case 48:
#line 348 "QuestLoader.y"
{
			g_quest_for_loading.EndPayBonusStat = yyvsp[-1].integer;
		;
    break;}
case 49:
#line 353 "QuestLoader.y"
{
			g_quest_for_loading.EndPayBonusSkillPoint = yyvsp[-1].integer;
		;
    break;}
case 50:
#line 358 "QuestLoader.y"
{
			g_quest_for_loading.EndPayFame = yyvsp[-1].integer;
		;
    break;}
case 51:
#line 363 "QuestLoader.y"
{
			g_quest_for_loading.EndPayStatus = yyvsp[-1].integer;
		;
    break;}
case 52:
#line 368 "QuestLoader.y"
{
			g_quest_for_loading.EndPayLevel = yyvsp[-1].integer;
		;
    break;}
case 53:
#line 373 "QuestLoader.y"
{
			g_quest_for_loading.EndPayExperience = yyvsp[-1].integer;
		;
    break;}
case 54:
#line 378 "QuestLoader.y"
{
			g_quest_for_loading.EndPayWarpMapIndex = yyvsp[-1].integer;
		;
    break;}
case 55:
#line 383 "QuestLoader.y"
{
			g_quest_for_loading.EndDeleteItemVector.push_back(yyvsp[-1].item_w_count);
		;
    break;}
case 56:
#line 388 "QuestLoader.y"
{
			g_quest_for_loading.EndPayItemVector.push_back(yyvsp[-1].item_w_count);
		;
    break;}
case 57:
#line 395 "QuestLoader.y"
{
			yyval.integer_range.Min = yyvsp[-1].integer;
			yyval.integer_range.Max = yyvsp[-1].integer;
		;
    break;}
case 58:
#line 401 "QuestLoader.y"
{
			yyval.integer_range.Min = yyvsp[-3].integer;
			yyval.integer_range.Max = yyvsp[-1].integer;
		;
    break;}
case 59:
#line 407 "QuestLoader.y"
{
			yyval.integer_range.Min = yyvsp[-2].integer;
			yyval.integer_range.Max = 0x7FFFFFFF;
		;
    break;}
case 60:
#line 413 "QuestLoader.y"
{
			yyval.integer_range.Min = 0;
			yyval.integer_range.Max = yyvsp[-1].integer;
		;
    break;}
case 61:
#line 420 "QuestLoader.y"
{ yyval.integer = yyvsp[-2].integer + yyvsp[0].integer; ;
    break;}
case 62:
#line 422 "QuestLoader.y"
{ yyval.integer = yyvsp[-2].integer - yyvsp[0].integer; ;
    break;}
case 63:
#line 424 "QuestLoader.y"
{ yyval.integer = yyvsp[-2].integer * yyvsp[0].integer; ;
    break;}
case 64:
#line 427 "QuestLoader.y"
{
				if (yyvsp[0].integer == 0.0)
					QuestLoadererror("divided by zero");
				else
					yyval.integer = yyvsp[-2].integer / yyvsp[0].integer;
			;
    break;}
case 65:
#line 434 "QuestLoader.y"
{ yyval.integer = yyvsp[-2].integer & yyvsp[0].integer; ;
    break;}
case 66:
#line 436 "QuestLoader.y"
{ yyval.integer = yyvsp[-1].integer; ;
    break;}
case 67:
#line 438 "QuestLoader.y"
{ yyval.integer = yyvsp[0].integer; ;
    break;}
case 68:
#line 440 "QuestLoader.y"
{ yyval.integer = g_user_var_quest.GetUserVariable(yyvsp[0].string)->IntValue; ;
    break;}
case 69:
#line 444 "QuestLoader.y"
{ yyval.string = strdup(yyvsp[0].string); ;
    break;}
case 70:
#line 446 "QuestLoader.y"
{ yyval.string = strdup(g_user_var_quest.GetUserVariable(yyvsp[0].string)->StringValue); ;
    break;}
case 71:
#line 451 "QuestLoader.y"
{
			yyval.item_w_count.ItemNum = yyvsp[-3].integer;
			yyval.item_w_count.Count = yyvsp[-1].integer;
		;
    break;}
case 72:
#line 457 "QuestLoader.y"
{
			yyval.item_w_count.ItemNum = yyvsp[-3].integer;
			yyval.item_w_count.Count = -1;
		;
    break;}
}
   /* the action file gets copied in in place of this dollarsign */

  yyvsp -= yylen;
  yyssp -= yylen;
#ifdef YYLSP_NEEDED
  yylsp -= yylen;
#endif

#if YYDEBUG != 0
  if (yydebug)
    {
      short *ssp1 = yyss - 1;
      fprintf (stderr, "state stack now");
      while (ssp1 != yyssp)
	fprintf (stderr, " %d", *++ssp1);
      fprintf (stderr, "\n");
    }
#endif

  *++yyvsp = yyval;

#ifdef YYLSP_NEEDED
  yylsp++;
  if (yylen == 0)
    {
      yylsp->first_line = yylloc.first_line;
      yylsp->first_column = yylloc.first_column;
      yylsp->last_line = (yylsp-1)->last_line;
      yylsp->last_column = (yylsp-1)->last_column;
      yylsp->text = 0;
    }
  else
    {
      yylsp->last_line = (yylsp+yylen-1)->last_line;
      yylsp->last_column = (yylsp+yylen-1)->last_column;
    }
#endif

  /* Now "shift" the result of the reduction.
     Determine what state that goes to,
     based on the state we popped back to
     and the rule number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTBASE] + *yyssp;
  if (yystate >= 0 && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTBASE];

  goto yynewstate;

yyerrlab:   /* here on detecting error */

  if (! yyerrstatus)
    /* If not already recovering from an error, report this error.  */
    {
      ++yynerrs;

#ifdef YYERROR_VERBOSE
      yyn = yypact[yystate];

      if (yyn > YYFLAG && yyn < YYLAST)
	{
	  int size = 0;
	  char *msg;
	  int x, count;

	  count = 0;
	  /* Start X at -yyn if nec to avoid negative indexes in yycheck.  */
	  for (x = (yyn < 0 ? -yyn : 0);
	       x < (sizeof(yytname) / sizeof(char *)); x++)
	    if (yycheck[x + yyn] == x)
	      size += strlen(yytname[x]) + 15, count++;
	  msg = (char *) malloc(size + 15);
	  if (msg != 0)
	    {
	      strcpy(msg, "parse error");

	      if (count < 5)
		{
		  count = 0;
		  for (x = (yyn < 0 ? -yyn : 0);
		       x < (sizeof(yytname) / sizeof(char *)); x++)
		    if (yycheck[x + yyn] == x)
		      {
			strcat(msg, count == 0 ? ", expecting `" : " or `");
			strcat(msg, yytname[x]);
			strcat(msg, "'");
			count++;
		      }
		}
	      yyerror(msg);
	      free(msg);
	    }
	  else
	    yyerror ("parse error; also virtual memory exceeded");
	}
      else
#endif /* YYERROR_VERBOSE */
	yyerror("parse error");
    }

  goto yyerrlab1;
yyerrlab1:   /* here on error raised explicitly by an action */

  if (yyerrstatus == 3)
    {
      /* if just tried and failed to reuse lookahead token after an error, discard it.  */

      /* return failure if at end of input */
      if (yychar == YYEOF)
	YYABORT;

#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Discarding token %d (%s).\n", yychar, yytname[yychar1]);
#endif

      yychar = YYEMPTY;
    }

  /* Else will try to reuse lookahead token
     after shifting the error token.  */

  yyerrstatus = 3;		/* Each real token shifted decrements this */

  goto yyerrhandle;

yyerrdefault:  /* current state does not do anything special for the error token. */

#if 0
  /* This is wrong; only states that explicitly want error tokens
     should shift them.  */
  yyn = yydefact[yystate];  /* If its default is to accept any token, ok.  Otherwise pop it.*/
  if (yyn) goto yydefault;
#endif

yyerrpop:   /* pop the current state because it cannot handle the error token */

  if (yyssp == yyss) YYABORT;
  yyvsp--;
  yystate = *--yyssp;
#ifdef YYLSP_NEEDED
  yylsp--;
#endif

#if YYDEBUG != 0
  if (yydebug)
    {
      short *ssp1 = yyss - 1;
      fprintf (stderr, "Error: state stack now");
      while (ssp1 != yyssp)
	fprintf (stderr, " %d", *++ssp1);
      fprintf (stderr, "\n");
    }
#endif

yyerrhandle:

  yyn = yypact[yystate];
  if (yyn == YYFLAG)
    goto yyerrdefault;

  yyn += YYTERROR;
  if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != YYTERROR)
    goto yyerrdefault;

  yyn = yytable[yyn];
  if (yyn < 0)
    {
      if (yyn == YYFLAG)
	goto yyerrpop;
      yyn = -yyn;
      goto yyreduce;
    }
  else if (yyn == 0)
    goto yyerrpop;

  if (yyn == YYFINAL)
    YYACCEPT;

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Shifting error token, ");
#endif

  *++yyvsp = yylval;
#ifdef YYLSP_NEEDED
  *++yylsp = yylloc;
#endif

  yystate = yyn;
  goto yynewstate;
}
#line 462 "QuestLoader.y"


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
