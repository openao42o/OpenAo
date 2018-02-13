/* A Bison parser, made by GNU Bison 2.7.  */

/* Bison interface for Yacc-like parsers in C
   
      Copyright (C) 1984, 1989-1990, 2000-2012 Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.
   
   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

#ifndef YY_QUESTLOADER_QUESTLOADER_HPP_INCLUDED
# define YY_QUESTLOADER_QUESTLOADER_HPP_INCLUDED
/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int QuestLoaderdebug;
#endif

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     QUEST_TOK = 258,
     BASIC_INFO_TOK = 259,
     QUEST_INDEX_TOK = 260,
     QUEST_NAME_TOK = 261,
     QUEST_EPISODE_TYPE_TOK = 262,
     QUEST_PARTYTYPE_TOK = 263,
     QUEST_KIND_TOK = 264,
     QUEST_TYPE_TOK = 265,
     QUEST_START_TYPE_TOK = 266,
     QUEST_END_TYPE_TOK = 267,
     QUEST_PARTY_MEMBERS_TOK = 268,
     QUEST_NPC_INFO_TOK = 269,
     NPC_INDEX_TOK = 270,
     NPC_NAME_TOK = 271,
     PRE_TALK_TOK = 272,
     MID_TALK_TOK = 273,
     FAIL_TALK_TOK = 274,
     SUCCESS_TALK_TOK = 275,
     DESCRIPTION_TOK = 276,
     EVENT_MAP_TOK = 277,
     NEXT_QUEST_INDEX_TOK = 278,
     PARTY_QUEST_MEMBER_COUNT_TOK = 279,
     DROP_ITEM_TO_ALL_ATTACK_CHARACTER_TOK = 280,
     TIME_LIMIT_IN_MINUTES_TOK = 281,
     DISCARDABLE_QUEST_TOK = 282,
     BI_QUEST_ATTRI_IS_CITYWAR_TOK = 283,
     BI_QUEST_ATTRI_NEVER_COMPLETION_BY_USER_TOK = 284,
     DROP_ITEM_TO_ALL_PARTY_MEMBERS_TOK = 285,
     START_REQ_TOK = 286,
     S_REQ_UNIT_KIND_TOK = 287,
     S_REQ_LEVEL_TOK = 288,
     S_REQ_ATTACK_PART_TOK = 289,
     S_REQ_DEFENSE_PART_TOK = 290,
     S_REQ_FUEL_PART_TOK = 291,
     S_REQ_SOUL_PART_TOK = 292,
     S_REQ_SHIELD_PART_TOK = 293,
     S_REQ_DODGE_PART_TOK = 294,
     S_REQ_ITEM_TOK = 295,
     S_REQ_QUEST_INDEX_TOK = 296,
     S_REQ_RECV_QUEST_INDEX_TOK = 297,
     S_REQ_GUILD_TOK = 298,
     S_REQ_INFLUENCE_TYPE_TOK = 299,
     S_REQ_START_AREA_TOK = 300,
     START_ACTION_TOK = 301,
     S_PAY_ITEM_TOK = 302,
     S_DEL_ITEM_TOK = 303,
     S_START_MAP_INDEX_TOK = 304,
     S_START_PARTY_WARP_FLAG_TOK = 305,
     COMPLETION_REQ_TOK = 306,
     C_REQ_MEET_NPC_INFO_TOK = 307,
     C_REQ_ITEM_TOK = 308,
     C_REQ_MEET_OBJ_INDEX_TOK = 309,
     C_REQ_DEAD_BY_MONSTER_TOK = 310,
     C_REQ_MEET_MAP_AREA_TOK = 311,
     C_REQ_MONSTER_TOK = 312,
     C_REQ_MIX_ITEM_TARGET_TOK = 313,
     COMPLETION_ACTION_TOK = 314,
     C_PAY_BONUS_STAT_TOK = 315,
     C_PAY_BONUS_SKILL_POINT_TOK = 316,
     C_PAY_FAME_TOK = 317,
     C_PAY_STATUS_TOK = 318,
     C_PAY_LEVEL_TOK = 319,
     C_PAY_EXPERIENCE_TOK = 320,
     C_PAY_WARP_MAP_INDEX_TOK = 321,
     C_DEL_ITEM_TOK = 322,
     C_PAY_ITEM_TOK = 323,
     C_PAY_ITEM_MISSIONMASTER_TOK = 324,
     C_RANDOM_PAY_ITEM_TOK = 325,
     C_MIX_ITEM_TOK = 326,
     C_PAY_PROPENSITY_TOK = 327,
     C_CINEMA_NUM_TOK = 328,
     C_PAY_INFLUENCE_TYPE_TOK = 329,
     FROM_TOK = 330,
     TO_TOK = 331,
     MONSTER_TOK = 332,
     CHARACTER_TOK = 333,
     ALL_TOK = 334,
     ATTACH_TOK = 335,
     AREA_TOK = 336,
     X_TOK = 337,
     Y_TOK = 338,
     R_TOK = 339,
     PER_TOK = 340,
     PROB_100_TOK = 341,
     PROB_1K_TOK = 342,
     PROB_10K_TOK = 343,
     PROB_100K_TOK = 344,
     PROB_256_TOK = 345,
     AND_TOK = 346,
     OR_TOK = 347,
     TRUE_TOK = 348,
     FALSE_TOK = 349,
     R_ARROW_TOK = 350,
     GT_TOK = 351,
     LT_TOK = 352,
     GE_TOK = 353,
     LE_TOK = 354,
     EQUAL_TOK = 355,
     USER_VARIABLE_TOK = 356,
     INTEGER_TOK = 357,
     WORD_TOK = 358,
     QSTRING_TOK = 359
   };
#endif


#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{
/* Line 2058 of yacc.c  */
#line 34 "QuestLoader.y"

	int					BOOLEAN;
	int					integer;
	char				string2048[2048];
	MONSTER_W_COUNT		monster_w_count;
	ITEM_W_COUNT		item_w_count;
	ITEM_W_COUNT_PROB	item_w_count_prob;
	QUEST_PAY_ITEM		quest_pay_item;
	INT_RANGE			integer_range;
	MAP_AREA			map_area;


/* Line 2058 of yacc.c  */
#line 174 "QuestLoader.hpp"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE QuestLoaderlval;

#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int QuestLoaderparse (void *YYPARSE_PARAM);
#else
int QuestLoaderparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int QuestLoaderparse (void);
#else
int QuestLoaderparse ();
#endif
#endif /* ! YYPARSE_PARAM */

#endif /* !YY_QUESTLOADER_QUESTLOADER_HPP_INCLUDED  */
