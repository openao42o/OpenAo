// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__E2BE8695_6D27_43CA_A49E_13A893C4F3CE__INCLUDED_)
#define AFX_STDAFX_H__E2BE8695_6D27_43CA_A49E_13A893C4F3CE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#define _WIN32_WINNT 0x0501

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#pragma warning(disable:4786)		// 2009-04-21 by cmkwon, ITEM에 DesParam 필드 개수 8개로 늘리기 - 
#include "AtumParam.h"				// 2009-04-21 by cmkwon, ITEM에 DesParam 필드 개수 8개로 늘리기 - 

#include "GlobalGameServer.h" // with "GlobalGameServerForAdminTool.cpp"

typedef struct TI_ITEM {
	int ItemNum;
	unsigned char Kind;
	char ItemName[40];
	float AbilityMin;
	float AbilityMax;
	short ReqRace;
	short ReqAttackPart;
	short ReqDefensePart;
	short ReqFuelPart;
	short ReqSoulPart;
	short ReqShieldPart;
	short ReqDodgePart;
	int ReqUnitKind;
	unsigned char ReqMinLevel;
	unsigned char ReqMaxLevel;
	unsigned char ReqItemKind;
	short Weight;
	unsigned char HitRate;
	unsigned char Defense;
	unsigned char FractionResistance;
	unsigned char NaturalFaction;
	unsigned char SpeedPenalty;
	short Range;
	unsigned char Position;
	unsigned char Scarcity;
	float Endurance;
	unsigned char AbrasionRate;
	short Charging;
	unsigned char Luck;
	short MinTradeQuantity;
#ifdef SHOP_PRICES_PER_BUILDING_NPC
	int SellingPrice;
#else
	int Price;
	int CashPrice;
#endif
	
// 2009-04-21 by cmkwon, ITEM에 DesParam 필드 개수 8개로 늘리기 - 아래와 같이 수정
// 	unsigned char DestParameter1;
// 	float ParameterValue1;
// 	unsigned char DestParameter2;
// 	float ParameterValue2;
// 	unsigned char DestParameter3;
// 	float ParameterValue3;
// 	unsigned char DestParameter4;
// 	float ParameterValue4;
	DestParam_t		ArrDestParameter[SIZE_MAX_DESPARAM_COUNT_IN_ITEM];		// 2011-08-01 by hskim, 파트너 시스템 2차 - 자료형 변경 (DestParameter - 255 -> 32767 지원)	
	float			ArrParameterValue[SIZE_MAX_DESPARAM_COUNT_IN_ITEM];

	int ReAttacktime;
	int Time;
	short RepeatTime;
	short Material;
	short ReqMaterial;
	float RangeAngle;
	unsigned char UpgradeNum;
	int LinkItem;
	unsigned char MultiTarget;
	short ExplosionRange;
	short ReactionRange;
	unsigned char ShotNum;
	unsigned char MultiNum;
	short AttackTime;
	unsigned char ReqSP;
	int SummonMonster;
	int NextSkill;
	unsigned char SkillLevel;
	unsigned char SkillHitRate;
	unsigned char SkillType;
	unsigned char SkillTargetType;
	unsigned char Caliber;
	unsigned char OrbitType;
	int ItemAttribute;
	float BoosterAngle;
	int CameraPattern;
	char Description[200];
	int SourceIndex;
} _TI_ITEM;

typedef struct TI_MONSTER {
	int UniqueNumber;
	char MonsterName[20];
	unsigned char Level;
	long Experience;
	int HP;
	short Race;
	unsigned char Speed;
	int MonsterItem01;
	int MonsterItem02;
	int MonsterItem03;
	int MonsterItem04;
	int MonsterItem05;
	int ControlSkill;
	int SizeForServer;
	int SizeForClient;
	int CameraPattern;
	short MonsterForm;
	unsigned char AttackPattern;
	short MovePattern;
	unsigned char Belligerence;
	unsigned char Defense;
	unsigned char DefenseProbability;
	unsigned char AlphaBlending;
	unsigned char AttackObject;
	short AttackRange;
	short HPRecoveryTime;
	short HPRecoveryValue;
	short ScaleValue;
	unsigned char TextureIndex;
	int SourceIndex;
	unsigned char QuickTurnAngle;
	unsigned char QuickTurnSpeed;
	short QuickTurnTerm;
	unsigned char MPOption;
	unsigned char MPOptionParam1;
	unsigned char MPOptionParam2;
	short Range;
	float TurnAngle;
	unsigned char MonsterHabit;
} _TI_MONSTER;

typedef struct TI_MONSTERITEM {
	int MonsterUniqueNumber;
	int ItemNum;
	int MinCount;
	int MaxCount;
	int Probability;
	unsigned char PrefixProbability;
	unsigned char SuffixProbability;
} _TI_MONSTERITEM;

typedef struct TI_MAPINFO {
	int MapIndex;
	int RenderMapIndex;
	char MapName[30];
	short		MapInfluenceType;			// 2005-12-26 by cmkwon
	int CitiMapIndex;
	int CitiWarQuestIndex;
	int CitiWarQuestNPCIndex;
	int CitiWarQuestMonsterUID;
	short MaxUserCount;
	short MaxGroundHeight;
	short MinGroundHeight;
	short WaterHeight;
	short UserVisibleDistance;
	short MonsterVisibleDistance;
	int QuestIndexForWarp;
	short FrontPositionDistance;
	unsigned char AutoRecoveryFlag;
	int DefaultMapIndex;
	short CitiWarpTargetPositionX;
	short CitiWarpTargetPositionY;
	short CitiWarpTargetPositionZ;
	short ClientMaxAltitudeHeight;
	int DayFogColor;
	int NightFogColor;
	short DayFogStartDistance;
	short DayFogEndDistance;
	short NightFogStartDistance;
	short NightFogEndDistance;
	short DaySunDirectionX;
	short DaySunDirectionY;
	short DaySunDirectionZ;
	short NightSunDirectionX;
	short NightSunDirectionY;
	short NightSunDirectionZ;
	unsigned char WaterType;
	unsigned char PKZoneFlag;
	unsigned char TileRenderingFlag;
	unsigned char SkyRenderingFlag;
	unsigned char SunRenderingFlag;
	unsigned char FogRenderingFlag;
} _TI_MAPINFO;

enum MONSTER_ITEMLIST_TYPE
{
	MONSTER_ITEMLIST_TYPE_ALL_MONSTER		= 0,		// 2005-12-23 by cmkwon, 모든 몬스터의 아이템 리스트
	MONSTER_ITEMLIST_TYPE_ANYMONSTER		= 1			// 2005-12-23 by cmkwon, 특정 몬스터의 아이템 리스트
};



#include "DbgOut_C.h"
#include "QuestLoaderParser.h"
#include "ODBCStatement.h" //For DB Access
#include "GridCtrl/GridCtrl.h" //For GridCtrl
#include "AtumProtocol.h"
#include "AtumSJ.h"
#include "AtumParam.h"
#include "AtumProtocol.h"
#include "AtumError.h"
#include "writelog.h"
#include "MonsterDetailDlg.h"
#include "MonsterDlg.h"
#include "MonsterDropDetailDlg.h"
#include "MonsterDropDlg.h"
#include "MonsterAttackDetailDlg.h"
#include "MonsterAttackDlg.h"
#include "Quest.h"
#include "PreviewDlg.h"
#include "QuestEditorDlg.h"
#include "QuestLoadDlg.h"
#include "LinkDetailDlg.h"
#include "WeaponItemDetail.h"
#include "WeaponItem.h"
#include "EngineItemDetailDlg.h"
#include "EngineItemDlg.h"
#include "MapEditorDetailDlg.h"
#include "MapEditorDlg.h"
#include "StatisticSheetDlg.h"
#include "SystemLogManager.h"			// 2007-11-08 by cmkwon
#include "StringDefineQuery.h"			// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
#include "StringDefineProcedure.h"	// 2008-08-22 by cmkwon, MySQL 통합(Procedure 뽑아내기) - 


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__E2BE8695_6D27_43CA_A49E_13A893C4F3CE__INCLUDED_)
