// LoadExcelFile.cpp: implementation of the CLoadExcelFile class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "atummonitor.h"
#include "LoadExcelFile.h"
#include "io.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


inline double atof_AfterChangeCommaToDot(const char *i_szStr)
{
	char szTemp[128] { };

	strncpy(szTemp, i_szStr, 127);

	for (auto c = szTemp; *c != '\0'; c++) if (*c == ',') *c = '.';

	return atof(szTemp);
}


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// 2009-10-23 by cmkwon, Monitor 엑셀파일 밀어넣기시 체크 추가 - 
SGAME_INFO_TABEL_INFO g_arrGameInfoTableInfo[] =
{
	{TB_NAME_ti_Item						, CB_COUNT_ti_Item							,"ItemNum","Kind","ItemName","AbilityMin","AbilityMax","ReqRace","ReqAttackPart","ReqDefensePart","ReqFuelPart","ReqSoulPart","ReqShieldPart","ReqDodgePart","ReqUnitKind","ReqMinLevel","ReqMaxLevel","ReqItemKind","Weight","HitRate","Defense","FractionResistance","NaturalFaction","SpeedPenalty","Range","Position","Scarcity","Endurance","AbrasionRate","Charging","Luck","MinTradeQuantity","Price","CashPrice","DestParameter1","ParameterValue1","DestParameter2","ParameterValue2","DestParameter3","ParameterValue3","DestParameter4","ParameterValue4","DestParameter5","ParameterValue5","DestParameter6","ParameterValue6","DestParameter7","ParameterValue7","DestParameter8","ParameterValue8","ReAttacktime","Time","RepeatTime","Material","ReqMaterial","RangeAngle","UpgradeNum","LinkItem","MultiTarget","ExplosionRange","ReactionRange","ShotNum","MultiNum","AttackTime","ReqSP","SummonMonster","NextSkill","SkillLevel","SkillHitRate","SkillType","SkillTargetType","Caliber","OrbitType","ItemAttribute","BoosterAngle","CameraPattern","SourceIndex","Description", "EnchantCheckDestParam", "InvokingDestParamID", "InvokingDestParamIDByUse"},	// 2009-09-09 ~ 2010-02-10 by dhjin, 인피니티 - 발동류장착아이템

	// 2010-04-09 by cmkwon, 인피2차 추가 수정(단계별 보상 추가) - 필드 1개추가(DropType)
	{TB_NAME_ti_MonsterItem					, CB_COUNT_ti_MonsterItem					,"MonsterUniqueNumber","ItemNum","MinCount","MaxCount","Probability","PrefixProbability","SuffixProbability","DropType"},

	{TB_NAME_ti_RareItemInfo				, CB_COUNT_ti_RareItemInfo					,"CodeNum","Name","ReqUseType","ReqMinLevel","ReqMaxLevel","ReqItemKind","ReqAttackPart","ReqDefensePart","ReqDodgePart","ReqFuelPart","ReqShieldPart","ReqSoulPart","DesParameter1","ParameterValue1","DesParameter2","ParameterValue2","DesParameter3","ParameterValue3","DesParameter4","ParameterValue4","DesParameter5","ParameterValue5","DesParameter6","ParameterValue6","DesParameter7","ParameterValue7","DesParameter8","ParameterValue8","DesParameter9","ParameterValue9","Probability"},
	{TB_NAME_ti_Shop						, CB_COUNT_ti_Shop							,"ShopOrder","UniqueNumber","ItemNum"},
	{TB_NAME_ti_CityTargetWarpMap			, CB_COUNT_ti_CityTargetWarpMap				,"MapIndex","BuildingIndex","WarpTargetMapIndex","WarpTargetIndex","WarpTargetName","Fee"},
	{TB_NAME_ti_ItemMixingInfo				, CB_COUNT_ti_ItemMixingInfo				,"UniqueID","TargetItemNum","MixingProbability","MixingCost","Visible"},		// 2013-05-06 by hskim, 아이템 미리 보기 (속성 값 추가) - // 2012-02-20 by hskim, DB 정규화 - ItemMixingInfo
	{TB_NAME_ti_ItemMixingElement			, CB_COUNT_ti_ItemMixingElement				,"UniqueID","SourceItemNum","SourceItemCount"},		// 2012-02-20 by hskim, DB 정규화 - ItemMixingInfo
	{TB_NAME_ti_EnchantInfo					, CB_COUNT_ti_EnchantInfo					,"EnchantItemNum","EnchantItemCount","EnchantCost","Prob1","Prob2","Prob3","Prob4","Prob5","Prob6","Prob7","Prob8","Prob9","Prob10","Prob11","Prob12","Prob13","Prob14","Prob15","Prob16","Prob17","Prob18","Prob19","Prob20","Prob21","Prob22","Prob23","Prob24","Prob25","Prob26","Prob27","Prob28","Prob29","Prob30","Prob31","Prob32","Prob33","Prob34","Prob35","Prob36","Prob37","Prob38","Prob39","Prob40"},
	{TB_NAME_ti_StrategyPointSummonMapIndex	, CB_COUNT_ti_StrategyPointSummonMapIndex	,"MapName","MapIndex","StratrgyPiontNum"},
	{TB_NAME_ti_ArenaInfo					, CB_COUNT_ti_ArenaInfo						,"ArenaMode","ReqMemberNum","PayInfluencePointWIN","PayInfluencePointLOSE","PayWarPointWIN","PayWarPointLOSE"},
	// 2012-09-14 by jhseol, 아레나 추가개발 part3 - 아레나 지급 아이템 추가 (아레나 전용 탄약 재충전 카드)
#ifdef _ARENA_ADDITIONAL_SUPPLY_ITEMS
	{ TB_NAME_ti_ArenaMapinfo, CB_COUNT_ti_ArenaMapinfo, "ArenaMapIndex", "ArenaMode", "ReqMemberNum", "PlayLimitedTime", "WINCondition", "LvDSupplyItemNum1", "LvDSupplyItemCount1", "LvDSupplyItemNum2", "LvDSupplyItemCount2", "LvDSupplyItemNum3", "LvDSupplyItemCount3", "LvDSupplyItemNum4", "LvDSupplyItemCount4", "LvDSupplyItemNum5", "LvDSupplyItemCount5" },
#else
	{TB_NAME_ti_ArenaMapinfo                , CB_COUNT_ti_ArenaMapinfo					,"ArenaMapIndex","ArenaMode","ReqMemberNum","PlayLimitedTime","WINCondition","LvDSupplyItemNum1","LvDSupplyItemCount1","LvDSupplyItemNum2","LvDSupplyItemCount2","LvDSupplyItemNum3","LvDSupplyItemCount3"},
#endif
	// end 2012-09-14 by jhseol, 아레나 추가개발 part3 - 아레나 지급 아이템 추가 (아레나 전용 탄약 재충전 카드)
	{TB_NAME_ti_MysteryItemDrop             , CB_COUNT_ti_MysteryItemDrop				,"MysteryItemDropNum","ReqUnitKind","ReqMinLevel","ReqMaxLevel","DropItemNum","MinCount","MaxCount","Probability","PrefixProbability","SuffixProbability","Period","CountPerPeriod","starttime"},
	{TB_NAME_ti_InfluenceWar                , CB_COUNT_ti_InfluenceWar					,"InfluenceType","WartimeStage","ReqContributionPoint","HPRepairRate","DPRepairRate","SPRepairRate","EventTime","EventExperience","EventDropSPI","EventDownExperience","EventDropItem","EventDropRareItem","SupplyItemNum","SupplyItemCount","IsSummonJacoMonster","BossMonsterUnitKind","BossMonsterCount","BossMonsterSummonTerm","BossMonsterSummonMapIndex","BossMonsterSummonPositionX","BossMonsterSummonPositionY","BossMonsterSummonPositionZ","SummonMonsterUnitKind1","SummonCount1","SummonMapIndex1","SummonPositionX1","SummonPositionY1","SummonPositionZ1","SummonMonsterUnitKind2","SummonCount2","SummonMapIndex2","SummonPositionX2","SummonPositionY2","SummonPositionZ2","SummonMonsterUnitKind3","SummonCount3","SummonMapIndex3","SummonPositionX3","SummonPositionY3","SummonPositionZ3","SummonMonsterUnitKind4","SummonCount4","SummonMapIndex4","SummonPositionX4","SummonPositionY4","SummonPositionZ4","SummonMonsterUnitKind5","SummonCount5","SummonMapIndex5","SummonPositionX5","SummonPositionY5","SummonPositionZ5","SupplyItemNum2","SupplyItemCount2","WinBossMonsterNum","LossBossMonsterNum"},
	{TB_NAME_ti_OutPostNextWarTime          , CB_COUNT_ti_OutPostNextWarTime			,"OutPostWarStartTime"},
	{TB_NAME_ti_ActionByLevel               , CB_COUNT_ti_ActionByLevel					,"Level","PollPoint"},
	{TB_NAME_ti_MapInfo                     , CB_COUNT_ti_MapInfo						,"MapIndex","RenderMapIndex","BeforeMapIndex","MapName","MapInfluenceType","CityMapIndex","CityWarQuestIndex","CityWarQuestNPCIndex","CityWarQuestMonsterUID","MaxUserCount","MaxGroundHeight","MinGroundHeight","WaterHeight","UserVisibleDistance","MonsterVisibleDistance","QuestIndexForWarp","FrontPositionDistance","AutoRecoveryFlag","DefaultMapIndex","CityWarpTargetPositionX","CityWarpTargetPositionY","CityWarpTargetPositionZ","ClientMaxAltitudeHeight","DayFogColor","NightFogColor","DayFogStartDistance","DayFogEndDistance","NightFogStartDistance","NightFogEndDistance","DaySunDirectionX","DaySunDirectionY","DaySunDirectionZ","NightSunDirectionX","NightSunDirectionY","NightSunDirectionZ","WaterType","PKZoneFlag","TileRenderingFlag","SkyRenderingFlag","SunRenderingFlag","FogRenderingFlag","VCNWarpObjectIndex","ANIWarpObjectIndex","dat","map","tex","cloud","sky","nsky","bgm","TeleportWarpObjectIndex","BeforeMapIndex2","MapDescription"},	// 2009-09-09 ~ 2010 by dhjin, 인피니티 -
	{TB_NAME_ti_BuildingNPC                 , CB_COUNT_ti_BuildingNPC					,"BuildingIndex","BuildingName","BuildingKind","MapIndex","NPCIndex","NPCName","GreetingTalk","OwnerCityMapIndex","OwnerOrderNumber"},

	// 2010-04-14 by cmkwon, 인피2차 몬스터 랜텀 타겟 변경 처리 - ChangeTargetTime 필드 추가
	{TB_NAME_ti_Monster                     , CB_COUNT_ti_Monster						,"UniqueNumber","MonsterName","Level","Experience","HP","Race","Speed","MonsterItem01","MonsterItem02","MonsterItem03","MonsterItem04","MonsterItem05","MonsterItem06","MonsterItem07","MonsterItem08","MonsterItem09","MonsterItem10","MonsterItem11","MonsterItem12","MonsterItem13","MonsterItem14","MonsterItem15","ControlSkill","SizeForServer","SizeForClient","Faction","MonsterForm","AttackPattern","MovePattern","Belligerence","Defense","DefenceProbability","AlphaBlending","AttackObject","AttackRange","HPRecoveryTime","HPRecoveryValue","RenderIndex","ScaleValue","TextureIndex","SourceIndex","QuickTurnAngle","QuickSpeed","QuickTurnTerm","MPOption","MPOptionParam1","MPOptionParam2","Range","TurnAngle","MonsterHabit","ClickEvent","HPActionIdx","MonsterTarget","ChangeTarget","MonsterTarget2","PortraitFileName","ChangeTargetTime","WayPointPattern"},	// 2010-03-31 by dhjin, 인피니티(기지방어) - // 2009-09-09 ~ 2010 by dhjin, 인피니티 - // 2011-05-23 by hskim, 인피니티 3차 - 웨이포인트 구현

	{TB_NAME_ti_TutorialInfo                , CB_COUNT_ti_TutorialInfo					,"TutorialUID","TutorialPayItem1","TutorialPayItemCount1","TutorialPayItem2","TutorialPayItemCount2","TutorialPayItem3","TutorialPayItemCount3"},
	{TB_NAME_ti_MapObject                   , CB_COUNT_ti_MapObject						,"ObjectName","Code","Alpha","RadiusForServer","RadiusForClient","Collision","CollisionForServer","RenderIndex","ObjectRenderType","ObjectDescription"},
	{TB_NAME_ti_LuckyMachine                , CB_COUNT_ti_LuckyMachine					,"MachineOrder","BuildingIndex","MachineKind","MachineNum","CoinItemNum","SlotNum","SlotProbability","MysteryItemDropNum","SourceIndex","Description"},
	{TB_NAME_ti_QuestMatching               , CB_COUNT_ti_QuestMatching					,"QuestIndexForBCU","QuestIndexForANI"},
	{TB_NAME_ti_ItemMatching                , CB_COUNT_ti_ItemMatching					,"ItemNumForBCU","ItemNumForANI"},
	{TB_NAME_ti_InfluenceRate               , CB_COUNT_ti_InfluenceRate					,"StartLevel","EndLevel"},
	// 2009-09-09 ~ 2010 by dhjin, 인피니티 - Excel -> DB 밀기
	{TB_NAME_ti_HPAction				    , CB_COUNT_ti_HPAction						,"HPActionUID", "HPActionNum", "HPMaxValueRate", "HPMinValueRate", "UseItemArrayIdx", "NextUseItemArrayIdx", "HitRate", "UseCount", "HPTalkCondition", "HPTalkImportance", "PreHPCameraTremble", "PreHPTalk", "HPCameraTremble", "HPTalk"},
	{TB_NAME_ti_Cinema	                    , CB_COUNT_ti_Cinema						,"CinemaNum", "CinemaOrder", "StartTime", "EffectIdx", "EffectPosition_X", "EffectPosition_Y", "EffectPosition_Z", "EffectPlayTime", "EffectPlayCount", "ObjectIdx", "ChangeObjectIdx", "ChangeTime", "CameraTremble", "KeyMonster", "MonsterIdx", "SummonCount", "SummonRandomPos", "ObjectBodyConditionIdx", "BodyCondition", "TipFileName", "SkyFileName", "CinemaTalk", "SoundFileName", "QuestIndex", "MonsterIdxType", "CustomIdx", "UpdateMonsterIdx" },	// 2010-03-29 by cmkwon, 인피2차 시네마에 효과음 파일 설정 추가 - // 2011-03-09 by hskim, 시네마 퀘스트 인덱스 설정 추가 - // 2011-04-28 by hskim, 인피니티 3차 - 시네마 관련 기능 추가 - 해당 맵채널 특정 몬스터 삭제 기능 추가
	{TB_NAME_ti_InfinityMonster	            , CB_COUNT_ti_InfinityMonster				,"InfinityMonsterUID", "InfinityMonsterIdx", "MonsterIdx", "TargetType", "TargetCount"},
	{TB_NAME_ti_Revision	                , CB_COUNT_ti_Revision						,"RevisionNum", "UnitKind", "RevisionLevel", "RevisionHP", "RevisionDP", "RevisionStandardWeapon", "RevisionAdvancedWeapon"},
	{TB_NAME_ti_InfinityMode		        , CB_COUNT_ti_InfinityMode					,"InfinityModeUID", "MapIdx", "ModeTypeNum", "InfinityMonsterIdx", "CinemaNum", "RevisionNum", "ResetCycle", "EntranceCount", "LimitTime", "MinLv", "MaxLv", "MinAdmissionCount", "MaxAdmissionCount", "TimePenaltyValue", "HPPenaltyValue"},	// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 사망시 패널티 추가 // // 2011-06-14 by hskim, 인피니티 3차 - 패널티 기능 추가 (HP 및 시간 동시 지원을 위해)
	{TB_NAME_ti_Tender				        , CB_COUNT_ti_Tender						,"TenderUID", "TenderItemNum"},
	{TB_NAME_ti_InfinityShop		        , CB_COUNT_ti_InfinityShop					,"InfinityShopUID", "BuyItemNum", "BuyItemCount", "TradeItemNum1", "TradeItemCount1", "TradeItemNum2", "TradeItemCount2", "TradeItemNum3", "TradeItemCount3", "TradeItemNum4", "TradeItemCount4", "TradeItemNum5", "TradeItemCount5"},		// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 상점
	{TB_NAME_ti_ItemParamOverlap	        , CB_COUNT_ti_ItemParamOverlap				,"ItemNum", "OverlapIndex"},		// 2010-01-18 by cmkwon, 아이템 사용시 Parameter 중복 체크 시스템 구현 - 
	{TB_NAME_ti_InvokingWearItemDPNum       , CB_COUNT_ti_InvokingWearItemDPNum			,"InvokingDestParamID", "InvokingDestParam", "InvokingDestParamValue", "InvokingEffectIdx"},		// 2009-09-09 ~ 2010-02-10 by dhjin, 인피니티 - 발동류장착아이템
	{TB_NAME_ti_InvokingWearItemDPNumByUse	, CB_COUNT_ti_InvokingWearItemDPNumByUse	,"InvokingDestParamIDByUse", "InvokingDestParamByUse", "InvokingDestParamValueByUse", "InvokingEffectIdxbyUse"},		// 2009-09-09 ~ 2010-02-10 by dhjin, 인피니티 - 발동류장착아이템
	{TB_NAME_ti_LevelAdjustment				, CB_COUNT_ti_LevelAdjustment				,"IncreaseStep","MonsterHP","MonsterDefense","MonsterEvasion","MonsterAttackItem","MonsterPopulation"}, // 2010-05-24 by shcho, 인피니티 난이도 조절 - 인피니티 난이도 조정에 관한 툴 업데이트
	{TB_NAME_ti_LevelAdjustmentReward		, CB_COUNT_ti_LevelAdjustmentReward			,"IncreaseStep","IncreaselncomeEXP","IncreaselncomeDropProbabillity","IncreaselncomeDropCount"}, // 2010-05-24 by shcho, 인피니티 난이도 조절 - 인피니티 난이도 조정에 관한 툴 업데이트
	// 2010-08-05 by dhjin, 버닝맵 - 
	{TB_NAME_ti_BurningMap					, CB_COUNT_ti_BurningMap					,"BuringMapUID","MapIndex","ReqUnitKind","ReqMinLv","ReqMaxLv"},
	//end, 2010-08-05 by dhjin, 버닝맵 - 
	{TB_NAME_ti_Pet							, CB_COUNT_ti_Pet							,"PetIndex","PetName","PetKind","EnableName","EnableLevel","MaxLevel","BaseSocketCount"},   // 2011-08-05 by hskim, 파트너 시스템 2차 - 자료 구조 결정 - // 2010-06-15 by shcho&hslee 펫시스템 - 팻 기본 정보 엑셀 밀기
	{TB_NAME_ti_PetLevel					, CB_COUNT_ti_petLevel						,"PetIndex","Level","NeedExp","UseWeaponIndex","SlotCountSkill","SlotCountSocket","KitLevelHP","KitLevelShield","KitLevelSP"},   // 2011-08-05 by hskim, 파트너 시스템 2차 - 자료 구조 결정 - // 2010-06-15 by shcho&hslee 펫시스템 - 팻 레벨 정보 엑셀 밀기 
	{TB_NAME_ti_OperatorAction				, CB_COUNT_ti_OperatorAction				,"ActionType","Itemnum","FunctionIndex","FunctionValue","SourceIndex","ActionDesc"},
	{TB_NAME_ti_DissolutionItem				, CB_COUNT_ti_DissolutionItem				,"SourceItemnum","ResultItemNum1","ResultItemMinCount1","ResultItemMaxCount1","ResultItemProbabillity1","ResultItemNum2","ResultItemMinCount2","ResultItemMaxCount2","ResultItemProbabillity2","ResultItemNum3","ResultItemMinCount3","ResultItemMaxCount3","ResultItemProbabillity3"},
	{TB_NAME_ti_MonsterMultiTarget			, CB_COUNT_ti_MonsterMultiTarget			,"MonsterIndex", "PointIndex", "X", "Y", "Z"},		// 2011-03-17 by hskim, 인피니티 3차 - 몬스터 멀티 타겟팅 기능 추가
	// start 2011-05-23 by hskim, 인피니티 3차 - 웨이포인트 구현
	{TB_NAME_ti_WayPoint					, CB_COUNT_ti_WayPoint						,"PatternIndex", "SequenceNum", "PostionAttribute", "X", "Y", "Z"},
	{TB_NAME_ti_WayPointMove				, CB_COUNT_ti_WayPointMove					,"PatternIndex", "CompletionAction"},
	// end 2011-05-23 by hskim, 인피니티 3차 - 웨이포인트 구현
	// start 2011-10-10 by hskim, EP4 [트리거 시스템] - 화산재 / 모래 폭풍
	{TB_NAME_ti_MapBuff						, CB_COUNT_ti_MapBuff						,"MapIndex", "ItemNum"},
	// end 2011-10-10 by hskim, EP4 [트리거 시스템] - 화산재 / 모래 폭풍
	// start 2011-10-28 by hskim, EP4 [트리거 시스템] - 크리스탈 시스템
	{TB_NAME_ti_MapTrigger					, CB_COUNT_ti_MapTrigger					,"MapTriggerID", "MapIndex", "MapChannel", "FunctionID"},
	{TB_NAME_ti_TriggerFunctionCrystal		, CB_COUNT_ti_TriggerFunctionCrystal		,"FunctionID", "CrystalGroupID", "PeriodTime", "RandomSequence"},
	{TB_NAME_ti_TriggerCrystalGroup			, CB_COUNT_ti_TriggerCrystalGroup			,"CrystalGroupID", "DestroyGroupID", "EventID"},
	{TB_NAME_ti_TriggerCrystalDestroyGroup	, CB_COUNT_ti_TriggerCrystalDestroyGroup	,"DestroyGroupID", "SequenceNumber", "TargetMonster"},
	// end 2011-10-28 by hskim, EP4 [트리거 시스템] - 크리스탈 시스템
	// start 2011-11-07 by hskim, EP4 [트리거 시스템] - NCG 거점전 시스템
	{TB_NAME_ti_TriggerFunctionNGCInflWar		, CB_COUNT_ti_TriggerFunctionNGCInflWar			,"FunctionID", "NGCInflWarGroupID", "PeriodTime", "BossMonster", "WinInfluenceWP", "LossInfluenceWP", "FirstDamageGuildWP", "FixedTimeType"},
	{TB_NAME_ti_TriggerNGCInflWarMonsterGroup	, CB_COUNT_ti_TriggerNGCInflWarMonsterGroup		,"NGCInflWarGroupID", "Monster", "SummonCount", "SummonRandomPos"},
	// end 2011-11-07 by hskim, EP4 [트리거 시스템] - NCG 거점전 시스템
	// 2013-01-21 by jhseol, NGC 전전기지 트리거 시스템 - 모니터 툴 DB정보 추가
#ifdef S_TRIGGER_OUTPOST_JHSEOL	// - 모니터 툴 DB정보 추가
	{TB_NAME_ti_TriggerFunctionNGCOutPost		, CB_COUNT_ti_TriggerFunctionNGCOutPost			,"FunctionID", "NGCInflWarGroupID", "StandardOutPostMap", "BossMonster", "WinInfluenceWP", "LossInfluenceWP", "NextOutPostMap", "DestroyCrystalcount"},
	{TB_NAME_ti_TriggerOutPostBossKill			, CB_COUNT_ti_TriggerOutPostBossKill			,"BossMonster", "CrystalNum", "RegenMinTimeMinutes", "RegenMaxTimeMinutes"},
	{TB_NAME_ti_TriggerCrystalBuff				, CB_COUNT_ti_TriggercrystalBuff				,"FunctionID", "SkillItemNum"},
#endif	// #ifdef S_TRIGGER_OUTPOST_JHSEOL
	// end 2013-01-21 by jhseol, NGC 전전기지 트리거 시스템 - 모니터 툴 DB정보 추가
	{TB_NAME_ti_OverlapItem						, CB_COUNT_ti_ti_OverlapItem					,"ItemNum", "CashPrice", "Tab", "ItemAttribute"},		// 2012-03-13 by hskim, 현지화 관련 DB 구조 변경
	
	{NULL, 0}
};

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief			// 2009-10-23 by cmkwon, Monitor 엑셀파일 밀어넣기시 체크 추가 - SGAME_INFO_TABEL_INFO::CheckFieldNameList#
///					// 2010-04-20 by cmkwon, SCMonitor 엑셀 파일 밀어넣기 에러 정보 추가 - 
/// \author		cmkwon
/// \date		2009-10-23 ~ 2009-10-23
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL SGAME_INFO_TABEL_INFO::CheckFieldNameList(vectstring *i_pvectFieldNameList, char *o_pszErrTxt)
{
	// 2010-04-20 by cmkwon, SCMonitor 엑셀 파일 밀어넣기 에러 정보 추가 - 
	//*o_ppszErrField = NULL;

	if(m_nCBCount-1 > i_pvectFieldNameList->size())
	{
		sprintf(o_pszErrTxt, "FieldCount not match !, (%d != %d)", m_nCBCount-1, i_pvectFieldNameList->size());
		return FALSE;
	}

	for(int i=0; i < m_nCBCount-1; i++)
	{
		if(0 != stricmp(m_szFieldNames[i], (*i_pvectFieldNameList)[i].c_str()))
		{
			// 2010-04-20 by cmkwon, SCMonitor 엑셀 파일 밀어넣기 에러 정보 추가 - 
			//*o_ppszErrField = m_szFieldNames[i];
			sprintf(o_pszErrTxt, "FieldName not match !, FieldIdx(%d) (%s != %s)", i, m_szFieldNames[i], (*i_pvectFieldNameList)[i].c_str());
			return FALSE;
		}
	}

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2009-10-23 by cmkwon, Monitor 엑셀파일 밀어넣기시 체크 추가 - 
/// \author		cmkwon
/// \date		2009-10-23 ~ 2009-10-23
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
SGAME_INFO_TABEL_INFO *FindGameInfoTableInfoByTableName(char *i_szTableName)
{
	int i=0;
	while(TRUE)
	{
		if(NULL == g_arrGameInfoTableInfo[i].m_szTableName || 0 > g_arrGameInfoTableInfo[i].m_nCBCount)
		{
			return NULL;
		}

		if(0 == stricmp(g_arrGameInfoTableInfo[i].m_szTableName, i_szTableName))
		{
			return &g_arrGameInfoTableInfo[i];
		}

		i++;
	}

	return NULL;
}


// 2009-07-09 by jwlee, 게임 정보 테이블 데이터 DB에 밀어넣기 툴 구현 -
CLoadExcelFile::CLoadExcelFile()
{
	::CoInitialize(NULL);

	vectItem.reserve(10000);
	vectMonsterItem.reserve(10000);
	vectRareItemInfo.reserve(5000);
	vectItemMixingInfo.reserve(60000);
	vectItemMixingElement.reserve(10000);		// 2012-02-20 by hskim, DB 정규화 - ItemMixingInfo
	vectEnchantInfo.reserve(1000);
	vectArenaInfo.reserve(100);
	vectArenaMapInfo.reserve(100);
	vectMysteryItem.reserve(5000);
	vectInfluenceWarInfo.reserve(100);
	vectOutPostInfo.reserve(50);
	vectActoinByLevel.reserve(500);
	vectMapInfo.reserve(1000);
	vectBuildingNpc.reserve(500);
	vectMonsterInfo.reserve(2000);
	vectTutorialInfo.reserve(50);
	vectLuckyMachine.reserve(500);
	vectQuestMatching.reserve(1000);
	vectItemMatching.reserve(500);
	vectItemInfo.reserve(500);
	vectInfluenceRate.reserve(500);		// 2009-09-16 by cmkwon, 세력 초기화시 어뷰징 방지 구현 - ti_InfluenceRate 테이블 추가, 
	////////////////////////////////////////////////////////////////////////////////
	// 2009-09-09 ~ 2010 by dhjin, 인피니티 - Excel -> DB 밀기
	HPActionList.reserve(1000);
	CinemaInfoList.reserve(1000);
	RevisionInfoList.reserve(1000);
	InfinityMonsterInfoList.reserve(1000);
	InfinityModeInfoList.reserve(1000);
	TenderInfoList.reserve(1000);
	InfinityShopInfoList.reserve(1000);			// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 상점

	ItemParamOverlapList.reserve(1000);			// 2010-01-18 by cmkwon, 아이템 사용시 Parameter 중복 체크 시스템 구현 - 

	InvokingItemDPList.reserve(1000);				// 2009-09-09 ~ 2010-02-10 by dhjin, 인피니티 - 발동류장착아이템
	InvokingItemDPByUseList.reserve(1000);		// 2009-09-09 ~ 2010-02-10 by dhjin, 인피니티 - 발동류장착아이템

	LevelAdjustmentList.reserve(1000);			// 2010-05-24 by shcho, 인피니티 난이도 조절 - 인피니티 난이도 조정에 관한 툴 업데이트
	LevelAdjustmentRewardList.reserve(1000);	// 2010-05-24 by shcho, 인피니티 난이도 조절 - 인피니티 난이도 조정에 관한 툴 업데이트
	
	BurningMapList.reserve(100);				// 2010-08-05 by dhjin, 버닝맵 - 

	PetBaseData.reserve(1000);	// 2010-06-15 by shcho&hslee 펫시스템 - 팻 기본 정보 엑셀 밀기
	PetLevelData.reserve(1000);	// 2010-06-15 by shcho&hslee 펫시스템 - 팻 레벨 정보 엑셀 밀기
	OperatorDataList.reserve(1000); // 2010-06-15 by shcho&hslee 펫시스템 - DB 밀기 추가 작업 펫 오퍼레이터 정보.
	DissolutionIteminfo.reserve(1000); // 2010-08-31 by shcho&jskim 아이템용해 시스템 - 용해 아이템 정보.
	MonsterMultiTargetList.reserve(1000); // 2011-03-17 by hskim, 인피니티 3차 - 몬스터 멀티 타겟팅 기능 추가

	// start 2011-05-23 by hskim, 인피니티 3차 - 웨이포인트 구현
	WayPointList.reserve(1000);
	WayPointMoveList.reserve(1000);
	// end 2011-05-23 by hskim, 인피니티 3차 - 웨이포인트 구현

	// start 2011-10-28 by hskim, EP4 [트리거 시스템] - 크리스탈 시스템
	m_vectTriggerMapList.reserve(100);
	m_vectTriggerFunctionCrystalList.reserve(100);
	m_vectTriggerCrystalGroupList.reserve(100);
	m_vectTriggerCrystalDestroyGroupList.reserve(200);
	// end 2011-10-28 by hskim, EP4 [트리거 시스템] - 크리스탈 시스템

	// start 2011-11-07 by hskim, EP4 [트리거 시스템] - NCG 거점전 시스템
	m_vectTriggerFunctionNGCInflWarList.reserve(20);
	m_vectTriggerNGCInflWarMonsterGroupList.reserve(20);	
	// end 2011-11-07 by hskim, EP4 [트리거 시스템] - NCG 거점전 시스템

	// 2013-01-21 by jhseol, NGC 전전기지 트리거 시스템
	m_vectTriggerFunctionNGCOutPostList.reserve(10);
	m_vectTriggerOutPostBossKillInfoList.reserve(50);
	m_vectTriggerCrystalBuff.reserve(50);
	// end 2013-01-21 by jhseol, NGC 전전기지 트리거 시스템

//	m_vectOverlapItemList.reserve(2000);		// 2012-03-13 by hskim, 현지화 관련 DB 구조 변경

	m_vectTableInfo.reserve(50);

	memset(szStrCon, 0x00, sizeof(szStrCon));
	memset(szFileName, 0x00, sizeof(szFileName));
	memset(m_szStrTable, 0x00, sizeof(m_szStrTable));
	memset(m_szStrQuery, 0x00, sizeof(m_szStrTable));

	_ConnectionPtr	m_Connection			= NULL;
	_RecordsetPtr	m_RecordSet				= NULL;
	_RecordsetPtr	m_Sheetname				= NULL;
}

CLoadExcelFile::~CLoadExcelFile()
{
	DeleteExcelData();
	if(m_Connection != NULL){m_Connection->Close();}

	memset(szStrCon, 0x00, sizeof(szStrCon));
	memset(szFileName, 0x00, sizeof(szFileName));
	memset(m_szStrTable, 0x00, sizeof(m_szStrTable));
	memset(m_szStrQuery, 0x00, sizeof(m_szStrQuery));

	m_Connection			= NULL;
	m_RecordSet				= NULL;
	m_Sheetname				= NULL;

	::CoUninitialize();
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CLoadExcelFile::ConnectExcel(char *i_szFileName)
/// \brief		엑셀 파일 연결 
/// \author		jwlee
/// \date		2009-07-09 ~ 2009-07-09
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CLoadExcelFile::ConnectExcel(char *i_szFileName)
{
	if(NULL == i_szFileName){
		return FALSE;
	}

	if(-1 == access(i_szFileName, 6)){
		return FALSE;
	}

	try{
	m_Connection.CreateInstance(__uuidof(Connection));
	m_RecordSet.CreateInstance(__uuidof(Recordset));
	
	util::strncpy(szFileName, i_szFileName, MAX_PATH);

	sprintf(szStrCon, "Provider=Microsoft.ACE.OLEDB.12.0;"
					  "Data Source=%s;"
					  "Extended Properties=\"Excel 12.0;HDR=Yes;\" ", szFileName);
	
	m_Connection->Open(szStrCon, "", "", adConnectUnspecified);
	}catch(_com_error &e){
		e.Error();
		return FALSE;
	}

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CLoadExcelFile::DeleteExcelData()
/// \brief		로드한 데이터 초기화
/// \author		jwlee
/// \date		2009-07-09 ~ 2009-07-09
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CLoadExcelFile::DeleteExcelData()
{
	vectItem.clear();
	vectMonsterItem.clear();
	vectRareItemInfo.clear();
	vectShopInfo.clear();
	vectCityTargetWarpMap.clear();
	vectItemMixingInfo.clear();
	vectItemMixingElement.clear();		// 2012-02-20 by hskim, DB 정규화 - ItemMixingInfo
	vectEnchantInfo.clear();
	vectStrategyPointSummonMapIndex.clear();
	vectArenaInfo.clear();
	vectArenaMapInfo.clear();
	vectMysteryItem.clear();
	vectInfluenceWarInfo.clear();
	vectOutPostInfo.clear();
	vectActoinByLevel.clear();
	vectMapInfo.clear();
	vectBuildingNpc.clear();
	vectMonsterInfo.clear();
	vectTutorialInfo.clear();
	vectMapObject.clear();
	vectLuckyMachine.clear();
	vectQuestMatching.clear();
	vectItemMatching.clear();
	vectItemInfo.clear();
	vectInfluenceRate.clear();		// 2009-09-16 by cmkwon, 세력 초기화시 어뷰징 방지 구현 - ti_InfluenceRate 테이블 추가, 
	////////////////////////////////////////////////////////////////////////////////
	// 2009-09-09 ~ 2010 by dhjin, 인피니티 - Excel -> DB 밀기
	HPActionList.clear();
	CinemaInfoList.clear();
	RevisionInfoList.clear();
	InfinityMonsterInfoList.clear();
	InfinityModeInfoList.clear();	
	TenderInfoList.clear();
	InfinityShopInfoList.clear();			// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 상점

	ItemParamOverlapList.clear();			// 2010-01-18 by cmkwon, 아이템 사용시 Parameter 중복 체크 시스템 구현 - 
	InvokingItemDPList.clear();				// 2009-09-09 ~ 2010-02-10 by dhjin, 인피니티 - 발동류장착아이템
	InvokingItemDPByUseList.clear();		// 2009-09-09 ~ 2010-02-10 by dhjin, 인피니티 - 발동류장착아이템
	LevelAdjustmentList.clear();			// 2010-05-24 by shcho, 인피니티 난이도 조절 - 인피니티 난이도 조정에 관한 툴 업데이트
	LevelAdjustmentRewardList.clear();		// 2010-05-24 by shcho, 인피니티 난이도 조절 - 인피니티 난이도 조정에 관한 툴 업데이트
	
	BurningMapList.clear();				// 2010-08-05 by dhjin, 버닝맵 - 
	
	PetBaseData.clear();					// 2010-06-15 by shcho&hslee 펫시스템 - 팻 기본 정보 엑셀 밀기
	PetLevelData.clear();					// 2010-06-15 by shcho&hslee 펫시스템 - 팻 레벨 정보 엑셀 밀기
	OperatorDataList.clear();				// 2010-06-15 by shcho&hslee 펫시스템 - DB 밀기 추가 작업 펫 오퍼레이터 정보.
	DissolutionIteminfo.clear();			// 2010-08-31 by shcho&jskim 아이템용해 시스템 - 용해 아이템 정보.
	MonsterMultiTargetList.clear();				// 2011-03-17 by hskim, 인피니티 3차 - 몬스터 멀티 타겟팅 기능 추가

	// start 2011-05-23 by hskim, 인피니티 3차 - 웨이포인트 구현
	WayPointList.clear();
	WayPointMoveList.clear();
	// end 2011-05-23 by hskim, 인피니티 3차 - 웨이포인트 구현

	// start 2011-10-28 by hskim, EP4 [트리거 시스템] - 크리스탈 시스템
	m_vectTriggerMapList.clear();
	m_vectTriggerFunctionCrystalList.clear();
	m_vectTriggerCrystalGroupList.clear();
	m_vectTriggerCrystalDestroyGroupList.clear();
	// end 2011-10-28 by hskim, EP4 [트리거 시스템] - 크리스탈 시스템

	// start 2011-11-07 by hskim, EP4 [트리거 시스템] - NCG 거점전 시스템
	m_vectTriggerFunctionNGCInflWarList.clear();
	m_vectTriggerNGCInflWarMonsterGroupList.clear();
	// end 2011-11-07 by hskim, EP4 [트리거 시스템] - NCG 거점전 시스템	

	// 2013-01-21 by jhseol, NGC 전전기지 트리거 시스템
	m_vectTriggerFunctionNGCOutPostList.clear();
	m_vectTriggerOutPostBossKillInfoList.clear();
	m_vectTriggerCrystalBuff.clear();
	// end 2013-01-21 by jhseol, NGC 전전기지 트리거 시스템

//	m_vectOverlapItemList.clear();		// 2012-03-13 by hskim, 현지화 관련 DB 구조 변경
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2009-10-23 by cmkwon, Monitor 엑셀파일 밀어넣기시 체크 추가 - 
/// \author		cmkwon
/// \date		2009-10-23 ~ 2009-10-23
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CLoadExcelFile::GetFieldNameList(vectstring *o_pvectFieldNameList, _RecordsetPtr i_pRecordSet)
{
	o_pvectFieldNameList->clear();

	int nFieldCnt = i_pRecordSet->Fields->GetCount();
	for(int i=0; i < nFieldCnt; i++)
	{
		o_pvectFieldNameList->push_back((char*)(i_pRecordSet->Fields->GetItem((long)i)->Name));
	}
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CLoadExcelFile::LoadTableList()
/// \brief		엑셀 파일 테이블 리스트 작성
/// \author		jwlee
/// \date		2009-07-09 ~ 2009-07-09
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CLoadExcelFile::LoadTableList()
{
	char szTableinfo[MAX_TABLE_NAME];
	int nsize;

	// Loading TableList
	try{
	m_Sheetname = m_Connection->OpenSchema(adSchemaTables);
	m_Sheetname->MoveFirst();
	}catch(_com_error &e){
		e.Description();
		return FALSE;
	}
	m_vectTableInfo.clear();

	while(m_Sheetname->EndOfFile == VARIANT_FALSE)
	{
		if(m_Sheetname->Fields->GetItem((long)2)->GetValue().vt != VT_EMPTY
		&& m_Sheetname->Fields->GetItem((long)2)->GetValue().vt != VT_NULL)
		{
			_bstr_t Data = m_Sheetname->Fields->GetItem((long)2)->GetValue();
			util::strncpy(szTableinfo, (char*)Data, sizeof(szTableinfo));
			nsize = strlen(szTableinfo);
			if(szTableinfo[nsize-1] == '$' && szTableinfo[0] == 't' && szTableinfo[1] == 'i')
			{
				TABLEINFO tempTable;

				try{
					sprintf(m_szStrTable, "[%s]", (char*)Data);
					m_RecordSet->Open(m_szStrTable, _variant_t((IDispatch*)m_Connection), adOpenStatic, adLockReadOnly, adCmdTable);

					sprintf(m_szStrQuery, "select * from [%s]", (char*)Data);
					m_Connection->Execute(m_szStrQuery, NULL, adExecuteNoRecords);

					tempTable.szTableName = Data;
					tempTable.nRecordCount = m_RecordSet->GetRecordCount();

					///////////////////////////////////////////////////////////////////////////////
					// 2009-10-23 by cmkwon, Monitor 엑셀파일 밀어넣기시 체크 추가 - 
					CString csTableName = (char*)Data;
					csTableName.Remove('$');
					SGAME_INFO_TABEL_INFO *pGameInfoTbInfo = FindGameInfoTableInfoByTableName((LPSTR)(LPCSTR)csTableName);
					if(pGameInfoTbInfo)
					{
						vectstring tmFieldNameList;
						GetFieldNameList(&tmFieldNameList, m_RecordSet);

						// 2010-04-20 by cmkwon, SCMonitor 엑셀 파일 밀어넣기 에러 정보 추가 - 
						char errTxt[1024];
						util::zero(errTxt, 1024);
						if(FALSE == pGameInfoTbInfo->CheckFieldNameList(&tmFieldNameList, errTxt))
						{// 2009-10-23 by cmkwon, 필드명과 필드 순서를 체크한다.
							m_RecordSet->Close();
							m_Sheetname->Close();
							char szErrTxt[1024];
							sprintf(szErrTxt, "error check FieldNameList !! TableName(%s) ErrorString(%s)", m_szStrTable, errTxt);
							AfxMessageBox(szErrTxt);
							return FALSE;
						}
						
						m_vectTableInfo.push_back(tempTable);
					}
					m_RecordSet->Close();
				}catch(_com_error &e){
					e.Description();
					return FALSE;
				}
			}
		}
		m_Sheetname->MoveNext();
	}
	m_Sheetname->Close();

	if(0 == m_vectTableInfo.size())
	{
		return FALSE;
	}

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CLoadExcelFile::LoadExcelData(vector<CString> *i_pvectTables)
/// \brief		사용자가 선택한 테이블 데이터를 엑셀 파일에서 로드 
/// \author		jwlee
/// \date		2009-07-09 ~ 2009-07-29
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CLoadExcelFile::LoadExcelData(vector<CString> *i_pvectTables)
{
	// 엑셀 데이터 초기화
	DeleteExcelData();

	// Loading ExcelData
	vector<CString>::iterator itr(i_pvectTables->begin());
	for(; itr != i_pvectTables->end(); itr++)
	{
		CString tmSelectTable;
		tmSelectTable = *itr;
	
		try{
		// Exceute Query
		sprintf(m_szStrTable, "[%s$]", tmSelectTable);
		m_RecordSet->Open(m_szStrTable, _variant_t((IDispatch*)m_Connection), adOpenStatic, adLockReadOnly, adCmdTable);

		sprintf(m_szStrQuery, "select * from [%s$]", tmSelectTable);
		m_Connection->Execute(m_szStrQuery, NULL, adExecuteNoRecords);
		}catch(_com_error &e){
			e.Error();
			return FALSE;
		}

		///////////////////////////////////////////////////////////////////////////////
		// 2009-10-23 by cmkwon, Monitor 엑셀파일 밀어넣기시 체크 추가 - CLoadExcelFile::LoadExcelData#, 테이블명을 #define 한것을 사용함.
		if(stricmp(tmSelectTable, TB_NAME_ti_Item) == 0)
		{
			LoadExcelItem();
		}
		else if(stricmp(tmSelectTable, TB_NAME_ti_MonsterItem) == 0)
		{
			LoadExcelMonsterItem();
		}
		else if(stricmp(tmSelectTable, TB_NAME_ti_RareItemInfo) == 0)
		{
			LoadExcelRareItemInfo();
		}
		else if(stricmp(tmSelectTable, TB_NAME_ti_Shop) == 0)
		{
			LoadExcelShop();
		}
		else if(stricmp(tmSelectTable, TB_NAME_ti_CityTargetWarpMap) == 0)
		{
			LoadExcelCityTargetWarpMap();
		}
		else if(stricmp(tmSelectTable, TB_NAME_ti_ItemMixingInfo) == 0)
		{
			LoadExcelItemMixingInfo();
		}
		// 2012-02-20 by hskim, DB 정규화 - ItemMixingInfo
		else if(stricmp(tmSelectTable, TB_NAME_ti_ItemMixingElement) == 0)
		{
			LoadExcelItemMixingElement();
		}
		// 2012-02-20 by hskim, DB 정규화 - ItemMixingInfo
		else if(stricmp(tmSelectTable, TB_NAME_ti_EnchantInfo) == 0)
		{
			LoadExcelEnchantInfo();
		}
		else if(stricmp(tmSelectTable, TB_NAME_ti_StrategyPointSummonMapIndex) == 0)
		{
			LoadExcelStrategyPointSummonMapIndex();
		}
		else if(stricmp(tmSelectTable, TB_NAME_ti_ArenaInfo) == 0)
		{
			LoadExcelArenaInfo();
		}
		else if(stricmp(tmSelectTable, TB_NAME_ti_ArenaMapinfo) == 0)
		{
			LoadExcelArenaMapinfo();
		}
		else if(stricmp(tmSelectTable, TB_NAME_ti_MysteryItemDrop) == 0)
		{
			LoadExcelMysteryItemDrop();
		}
		else if(stricmp(tmSelectTable, TB_NAME_ti_InfluenceWar) == 0)
		{
			LoadExcelInfluenceWar();
		}
		else if(stricmp(tmSelectTable, TB_NAME_ti_OutPostNextWarTime) == 0)
		{
			LoadExcelOutPostNextWarTime();
		}
		else if(stricmp(tmSelectTable, TB_NAME_ti_ActionByLevel) == 0)
		{
			LoadExcelActionByLevel();
		}
		else if(stricmp(tmSelectTable, TB_NAME_ti_MapInfo) == 0)
		{
			LoadExcelMapInfo();
		}
		else if(stricmp(tmSelectTable, TB_NAME_ti_BuildingNPC) == 0)
		{
			LoadExcelBuildingNPC();
		}
		else if(stricmp(tmSelectTable, TB_NAME_ti_Monster) == 0)
		{
			LoadExcelMonster();
		}
		else if(stricmp(tmSelectTable, TB_NAME_ti_TutorialInfo) == 0)
		{
			LoadExcelTutorialInfo();
		}
		else if(stricmp(tmSelectTable, TB_NAME_ti_MapObject) == 0)
		{
			LoadExcelMapObject();
		}
		else if(stricmp(tmSelectTable, TB_NAME_ti_LuckyMachine) == 0)
		{
			LoadExcelLuckyMachine();
		}
		else if(stricmp(tmSelectTable, TB_NAME_ti_QuestMatching) == 0)
		{
			LoadExcelQuestMatching();
		}
		else if(stricmp(tmSelectTable, TB_NAME_ti_ItemMatching) == 0)
		{
			LoadExcelItemMatching();
		}
// 2009-10-23 by cmkwon, Monitor 엑셀파일 밀어넣기시 체크 추가 - CLoadExcelFile::LoadExcelData#, 사용하지 않으므로 제거 함.
// 		else if(stricmp(tmSelectTable, "ti_Item_Info") == 0)
// 		{
// 			LoadExcelItemInfo();
// 		}
		else if(stricmp(tmSelectTable, TB_NAME_ti_InfluenceRate) == 0)		// 2009-09-16 by cmkwon, 세력 초기화시 어뷰징 방지 구현 - ti_InfluenceRate 테이블 추가, 
		{
			LoadExceInfluenceRate();
		}
		////////////////////////////////////////////////////////////////////////////////
		// 2009-09-09 ~ 2010 by dhjin, 인피니티 - Excel -> DB 밀기
		else if(stricmp(tmSelectTable, TB_NAME_ti_HPAction) == 0)
		{
			LoadExcelHPAction();
		}
		else if(stricmp(tmSelectTable, TB_NAME_ti_Cinema) == 0)
		{
			LoadExcelCinema();
		}
		else if(stricmp(tmSelectTable, TB_NAME_ti_InfinityMonster) == 0)
		{
			LoadExcelInfinityMonster();
		}
		else if(stricmp(tmSelectTable, TB_NAME_ti_Revision) == 0)
		{
			LoadExcelRevision();
		}
		else if(stricmp(tmSelectTable, TB_NAME_ti_InfinityMode) == 0)
		{
			LoadExcelInfinityMode();
		}
		else if(stricmp(tmSelectTable, TB_NAME_ti_Tender) == 0)
		{
			LoadExcelTender();
		}	
		else if(stricmp(tmSelectTable, TB_NAME_ti_InfinityShop) == 0) {
			// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 상점
			LoadExcelInfinityShop();
		}	
		else if(stricmp(tmSelectTable, TB_NAME_ti_ItemParamOverlap) == 0) {
			// 2010-01-18 by cmkwon, 아이템 사용시 Parameter 중복 체크 시스템 구현 - CLoadExcelFile::LoadExcelData#, LoadExcelItemParamOverlap# 호출
			LoadExcelItemParamOverlap();
		}
		else if(stricmp(tmSelectTable, TB_NAME_ti_InvokingWearItemDPNum) == 0)
		{// 2009-09-09 ~ 2010-02-10 by dhjin, 인피니티 - 발동류장착아이템
			LoadExcelInvokingWearItemDPNum();
		}
		else if(stricmp(tmSelectTable, TB_NAME_ti_InvokingWearItemDPNumByUse) == 0)
		{// 2009-09-09 ~ 2010-02-10 by dhjin, 인피니티 - 발동류장착아이템
			LoadExcelInvokingWearItemDPNumByUse();
		}
		else if(stricmp(tmSelectTable, TB_NAME_ti_LevelAdjustment) == 0)
		{// 2010-05-24 by shcho, 인피니티 난이도 조절 - 인피니티 난이도 조정에 관한 툴 업데이트
			LoadExeclLevelAdjustment();
		}
		else if(stricmp(tmSelectTable, TB_NAME_ti_LevelAdjustmentReward) == 0)
		{// 2010-05-24 by shcho, 인피니티 난이도 조절 - 인피니티 난이도 조정에 관한 툴 업데이트
			LoadExeclLevelAdjustmentReward();
		}
		else if(stricmp(tmSelectTable, TB_NAME_ti_BurningMap) == 0)
		{// 2010-08-05 by dhjin, 버닝맵 - 
			LoadExeclBurningMap();
		}
		else if(stricmp(tmSelectTable, TB_NAME_ti_Pet) == 0)
		{// 2010-06-15 by shcho&hslee 펫시스템 - 팻 기본 정보 엑셀 밀기
			LoadExeclPetBaseData();
		}
		else if(stricmp(tmSelectTable, TB_NAME_ti_PetLevel) == 0)
		{// 2010-06-15 by shcho&hslee 펫시스템 - 팻 레벨 정보 엑셀 밀기
			LoadExeclPetLevelData();
		}
		else if(stricmp(tmSelectTable, TB_NAME_ti_OperatorAction) == 0)
		{// 2010-06-15 by shcho&hslee 펫시스템 - DB 밀기 추가 작업 펫 오퍼레이터 정보.
			ExeclLoadOperatorData();
		}
		else if(stricmp(tmSelectTable, TB_NAME_ti_DissolutionItem) == 0)
		{// 2010-08-31 by shcho&jskim 아이템용해 시스템 - 용해 아이템 정보.
			LoadExeclDissolutionItemData();
		}
		else if(stricmp(tmSelectTable, TB_NAME_ti_MonsterMultiTarget) == 0)
		{// 2011-03-17 by hskim, 인피니티 3차 - 몬스터 멀티 타겟팅 기능 추가
			LoadExeclMonsterMultiTargetData();
		}
		// start 2011-05-23 by hskim, 인피니티 3차 - 웨이포인트 구현
		else if(stricmp(tmSelectTable, TB_NAME_ti_WayPoint) == 0)
		{
			LoadExeclWayPointData();
		}
		else if(stricmp(tmSelectTable, TB_NAME_ti_WayPointMove) == 0)
		{
			LoadExeclWayPointMoveData();
		}
		// end 2011-05-23 by hskim, 인피니티 3차 - 웨이포인트 구현
		else if(stricmp(tmSelectTable, TB_NAME_ti_MapBuff) == 0)
		{// 2011-10-10 by hskim, EP4 [트리거 시스템] - 화산재 / 모래 폭풍
			LoadExeclTriggerMapBuffData();
		}
		// start 2011-10-28 by hskim, EP4 [트리거 시스템] - 크리스탈 시스템
		else if(stricmp(tmSelectTable, TB_NAME_ti_MapTrigger) == 0)
		{
			LoadExeclMapTrigger();
		}
		else if(stricmp(tmSelectTable, TB_NAME_ti_TriggerFunctionCrystal) == 0)
		{
			LoadExeclTriggerFunctionCrystal();
		}
		else if(stricmp(tmSelectTable, TB_NAME_ti_TriggerCrystalGroup) == 0)
		{
			LoadExeclTriggerCrystalGroup();
		}
		else if(stricmp(tmSelectTable, TB_NAME_ti_TriggerCrystalDestroyGroup) == 0)
		{
			LoadExeclTriggerCrystalDestroyGroup();
		}
		// end 2011-10-28 by hskim, EP4 [트리거 시스템] - 크리스탈 시스템
		// start 2011-11-07 by hskim, EP4 [트리거 시스템] - NCG 거점전 시스템
		else if(stricmp(tmSelectTable, TB_NAME_ti_TriggerFunctionNGCInflWar) == 0)
		{
			LoadExeclTriggerFunctionNGCInflWar();
		}
		else if(stricmp(tmSelectTable, TB_NAME_ti_TriggerNGCInflWarMonsterGroup) == 0)
		{
			LoadExeclTriggerNGCInflWarMonsterGroup();
		}
		// end 2011-11-07 by hskim, EP4 [트리거 시스템] - NCG 거점전 시스템
		// 2013-01-21 by jhseol, NGC 전전기지 트리거 시스템 - 모니터 툴 엑셀 파일에서 DB정보 가져오기
#ifdef S_TRIGGER_OUTPOST_JHSEOL	// - 모니터 툴 엑셀 파일에서 DB정보 가져오기
		else if(stricmp(tmSelectTable, TB_NAME_ti_TriggerFunctionNGCOutPost) == 0)
		{
			LoadExeclTriggerFunctionNGCOutPostList();
		}
		else if(stricmp(tmSelectTable, TB_NAME_ti_TriggerOutPostBossKill) == 0)
		{
			LoadExeclTriggerOutPostBossKillInfoList();
		}
		else if(stricmp(tmSelectTable, TB_NAME_ti_TriggerCrystalBuff) == 0)
		{
			LoadExeclTriggerCrystalBuffList();
		}
#endif	// #ifdef S_TRIGGER_OUTPOST_JHSEOL
		// end 2013-01-21 by jhseol, NGC 전전기지 트리거 시스템 - 모니터 툴 엑셀 파일에서 DB정보 가져오기
		// start 2012-03-13 by hskim, 현지화 관련 DB 구조 변경
		//else if(stricmp(tmSelectTable, TB_NAME_ti_OverlapItem) == 0)
		//{
		//	LoadExeclOverlapItem();
		//}
		// end 2012-03-13 by hskim, 현지화 관련 DB 구조 변경

		m_RecordSet->Close();
	} // end for
	return TRUE;
}

// 각 테이블 별 로드 함수
void CLoadExcelFile::LoadExcelItem(void)
{
	while(m_RecordSet->EndOfFile == VARIANT_FALSE)
	{
		ITEM tmItem;
		memset(&tmItem, 0x00, sizeof(tmItem));
		for(int i=0; i<m_RecordSet->Fields->GetCount(); ++i)
		{
			if(m_RecordSet->Fields->GetItem((long)i)->GetValue().vt != VT_EMPTY
				&&m_RecordSet->Fields->GetItem((long)i)->GetValue().vt != VT_NULL){
				_bstr_t Data;
				Data = m_RecordSet->Fields->GetItem((long)i)->GetValue();
				switch(i)
				{
					case 0:
						tmItem.ItemNum = atoi((char*)Data);
						break;
					case 1:
						tmItem.Kind = atoi((char*)Data);
						break;
					case 2:
						util::strncpy(tmItem.ItemName, (char*)Data, SIZE_MAX_ITEM_NAME);
						break;
					case 3:
						// 2010-03-04 by cmkwon, atof관련 문제 해결(언어별 소숫점 구분자 변환) - 
						tmItem.AbilityMin = atof_AfterChangeCommaToDot((char*)Data);
						break;
					case 4:
						// 2010-03-04 by cmkwon, atof관련 문제 해결(언어별 소숫점 구분자 변환) - 
						tmItem.AbilityMax = atof_AfterChangeCommaToDot((char*)Data);
						break;
					case 5:
						tmItem.ReqRace = atoi((char*)Data);
						break;
					case 6:
						tmItem.ReqGearStat.AttackPart = atoi((char*)Data);
						break;
					case 7:
						tmItem.ReqGearStat.DefensePart = atoi((char*)Data);
						break;
					case 8:
						tmItem.ReqGearStat.FuelPart = atoi((char*)Data);
						break;
					case 9:
						tmItem.ReqGearStat.SoulPart = atoi((char*)Data);
						break;
					case 10:
						tmItem.ReqGearStat.ShieldPart = atoi((char*)Data);
						break;
					case 11:
						tmItem.ReqGearStat.DodgePart = atoi((char*)Data);
						break;
					case 12:
						tmItem.ReqUnitKind = atoi((char*)Data);
						break;
					case 13:
						tmItem.ReqMinLevel = atoi((char*)Data);
						break;
					case 14:
						tmItem.ReqMaxLevel = atoi((char*)Data);
						break;
					case 15:
						tmItem.ReqItemKind = atoi((char*)Data);
						break;
					case 16:
						tmItem.Weight = atoi((char*)Data);
						break;
					case 17:
						{
							// 2009-10-22 by cmkwon, 게임정보파일 밀어넣기 툴 수정 - HitRate 필드는 
							// 2010-03-04 by cmkwon, atof관련 문제 해결(언어별 소숫점 구분자 변환) - 
							float fTemp = atof_AfterChangeCommaToDot((char*)Data);
							tmItem.HitRate = fTemp;//ceil(fTemp); // 2010-07-19 by dhjin, 확률 수식 변경
						}
						break;
					case 18:
						tmItem.Defense = atoi((char*)Data);
						break;
					case 19:
						tmItem.FractionResistance = atof_AfterChangeCommaToDot((char*)Data);		// 2010-07-19 by dhjin, 확률 수식 변경
						break;
					case 20:
						tmItem.NaturalFaction = atoi((char*)Data);
						break;
					case 21:
						tmItem.SpeedPenalty = atoi((char*)Data);
						break;
					case 22:
						tmItem.Range = atoi((char*)Data);
						break;
					case 23:
						tmItem.Position = atoi((char*)Data);
						break;
					case 24:
						tmItem.Scarcity = atoi((char*)Data);
						break;
					case 25:
						// 2010-03-04 by cmkwon, atof관련 문제 해결(언어별 소숫점 구분자 변환) - 
						tmItem.Endurance = atof_AfterChangeCommaToDot((char*)Data);
						break;
					case 26:
						tmItem.AbrasionRate = atoi((char*)Data);
						break;
					case 27:
						tmItem.Charging = atoi((char*)Data);
						break;
					case 28:
						tmItem.Luck = atoi((char*)Data);
						break;
					case 29:
						tmItem.MinTradeQuantity = atoi((char*)Data);
						break;
#ifndef SHOP_PRICES_PER_BUILDING_NPC
					case 30:
						tmItem.Price = atoi((char*)Data);
						break;
					case 31:
						tmItem.CashPrice = atoi((char*)Data);
						break;
#endif // SHOP_PRICES_PER_BUILDING_NPC
					case 32:
						tmItem.ArrDestParameter[0] = atoi((char*)Data);
						break;
					case 33:
						tmItem.ArrParameterValue[0] = atof_AfterChangeCommaToDot((char*)Data);		// 2010-03-04 by cmkwon, atof관련 문제 해결(언어별 소숫점 구분자 변환) - 
						break;
					case 34:
						tmItem.ArrDestParameter[1] = atoi((char*)Data);
						break;
					case 35:
						tmItem.ArrParameterValue[1] = atof_AfterChangeCommaToDot((char*)Data);		// 2010-03-04 by cmkwon, atof관련 문제 해결(언어별 소숫점 구분자 변환) - 
						break;
					case 36: 
						tmItem.ArrDestParameter[2] = atoi((char*)Data);
						break;
					case 37:
						tmItem.ArrParameterValue[2] = atof_AfterChangeCommaToDot((char*)Data);		// 2010-03-04 by cmkwon, atof관련 문제 해결(언어별 소숫점 구분자 변환) - 
						break;
					case 38:
						tmItem.ArrDestParameter[3] = atoi((char*)Data);
						break;
					case 39:
						tmItem.ArrParameterValue[3] = atof_AfterChangeCommaToDot((char*)Data);		// 2010-03-04 by cmkwon, atof관련 문제 해결(언어별 소숫점 구분자 변환) - 
						break;
					case 40:
						tmItem.ArrDestParameter[4] = atoi((char*)Data);
						break;
					case 41:
						tmItem.ArrParameterValue[4] = atof_AfterChangeCommaToDot((char*)Data);		// 2010-03-04 by cmkwon, atof관련 문제 해결(언어별 소숫점 구분자 변환) - 
						break;
					case 42:
						tmItem.ArrDestParameter[5] = atoi((char*)Data);
						break;
					case 43:
						tmItem.ArrParameterValue[5] = atof_AfterChangeCommaToDot((char*)Data);		// 2010-03-04 by cmkwon, atof관련 문제 해결(언어별 소숫점 구분자 변환) - 
						break;
					case 44:
						tmItem.ArrDestParameter[6] = atoi((char*)Data);
						break;
					case 45:
						tmItem.ArrParameterValue[6] = atof_AfterChangeCommaToDot((char*)Data);		// 2010-03-04 by cmkwon, atof관련 문제 해결(언어별 소숫점 구분자 변환) - 
						break;
					case 46:
						tmItem.ArrDestParameter[7] = atoi((char*)Data);
						break;
					case 47:
						tmItem.ArrParameterValue[7] = atof_AfterChangeCommaToDot((char*)Data);		// 2010-03-04 by cmkwon, atof관련 문제 해결(언어별 소숫점 구분자 변환) - 
						break;
					case 48:
						tmItem.ReAttacktime = atoi((char*)Data);
						break;
					case 49:
						tmItem.Time = atoi((char*)Data);
						break;
					case 50:
						tmItem.RepeatTime = atoi((char*)Data);
						break;
					case 51:
						tmItem.Material = atoi((char*)Data);
						break;
					case 52:
						tmItem.ReqMaterial = atoi((char*)Data);
						break;
					case 53:
						tmItem.RangeAngle = atof_AfterChangeCommaToDot((char*)Data);	// 2010-03-04 by cmkwon, atof관련 문제 해결(언어별 소숫점 구분자 변환) - 
						break;
					case 54:
						tmItem.UpgradeNum = atoi((char*)Data);
						break;
					case 55:
						tmItem.LinkItem = atoi((char*)Data);
						break;
					case 56:
						tmItem.MultiTarget = atoi((char*)Data);
						break;
					case 57:
						tmItem.ExplosionRange = atoi((char*)Data);
						break;
					case 58:
						tmItem.ReactionRange = atoi((char*)Data);
						break;
					case 59:
						tmItem.ShotNum = atoi((char*)Data);
						break;
					case 60:
						tmItem.MultiNum = atoi((char*)Data);
						break;
					case 61:
						tmItem.AttackTime = atoi((char*)Data);
						break;
					case 62:
						tmItem.ReqSP = atoi((char*)Data);
						break;
					case 63:
						tmItem.SummonMonster = atoi((char*)Data);
						break;
					case 64:
						tmItem.NextSkill = atoi((char*)Data);
						break;
					case 65:
						tmItem.SkillLevel = atoi((char*)Data);
						break;
					case 66:
						tmItem.SkillHitRate = atoi((char*)Data);
						break;
					case 67:
						tmItem.SkillType = atoi((char*)Data);
						break;
					case 68:
						tmItem.SkillTargetType = atoi((char*)Data);
						break;
					case 69:
						tmItem.Caliber = atoi((char*)Data);
						break;
					case 70:
						tmItem.OrbitType = atoi((char*)Data);
						break;
					case 71:
						tmItem.ItemAttribute = _atoi64((char*)Data);		// 2009-09-23 by cmkwon, Excel to DB 관련 자료형 버그 수정 - 
						break;
					case 72:
						tmItem.BoosterAngle = atof_AfterChangeCommaToDot((char*)Data);	// 2010-03-04 by cmkwon, atof관련 문제 해결(언어별 소숫점 구분자 변환) - 
						break;
					case 73:
						tmItem.CameraPattern = atoi((char*)Data);
						break;
					case 74:
						tmItem.SourceIndex = atoi((char*)Data);
						break;
					case 75:
						util::strncpy(tmItem.Description, (char*)Data, SIZE_MAX_ITEM_DESCRIPTION);
						break;
					case 76:		// 2009-09-09 ~ 2010-02-10 by dhjin, 인피니티 - 발동류장착아이템
						tmItem.EnchantCheckDestParam = atoi((char*)Data);
						break;
					case 77:		// 2009-09-09 ~ 2010-02-10 by dhjin, 인피니티 - 발동류장착아이템
						tmItem.InvokingDestParamID = atoi((char*)Data);
						break;
					case 78:		// 2009-09-09 ~ 2010-02-10 by dhjin, 인피니티 - 발동류장착아이템
						tmItem.InvokingDestParamIDByUse = atoi((char*)Data);
						break;

				}// end switch
			}// end if
		}// end for
		vectItem.push_back(tmItem);
		m_RecordSet->MoveNext();
	}// end while
}

void CLoadExcelFile::LoadExcelItemInfo(void)
{
	while(m_RecordSet->EndOfFile == VARIANT_FALSE)
	{
			ITEM_INFO tmItemInfo;
			memset(&tmItemInfo, 0x00, sizeof(tmItemInfo));
			for(int i=0; i<m_RecordSet->Fields->GetCount(); ++i)
			{
				if(m_RecordSet->Fields->GetItem((long)i)->GetValue().vt != VT_EMPTY
					&&m_RecordSet->Fields->GetItem((long)i)->GetValue().vt != VT_NULL)
				{
					_bstr_t Data;
					Data = m_RecordSet->Fields->GetItem((long)i)->GetValue();
					switch(i)
					{
						case 0:
							tmItemInfo.IsDisable = atof_AfterChangeCommaToDot((char*)Data);	// 2010-03-04 by cmkwon, atof관련 문제 해결(언어별 소숫점 구분자 변환) - 
							break;
						case 1:
							tmItemInfo.GameUID = atof_AfterChangeCommaToDot((char*)Data);		// 2010-03-04 by cmkwon, atof관련 문제 해결(언어별 소숫점 구분자 변환) - 
							break;
						case 2:
							tmItemInfo.ItemNum = atof_AfterChangeCommaToDot((char*)Data);		// 2010-03-04 by cmkwon, atof관련 문제 해결(언어별 소숫점 구분자 변환) - 
							break;
						case 3:
							util::strncpy(tmItemInfo.ItemName, (char*)Data, SIZE_MAX_ITEM_NAME);
							break;
						case 4:
							tmItemInfo.TabIndex = atof_AfterChangeCommaToDot((char*)Data);		// 2010-03-04 by cmkwon, atof관련 문제 해결(언어별 소숫점 구분자 변환) - 
							break;
						case 5:
							util::strncpy(tmItemInfo.IconFileName, (char*)Data, SIZE_MAX_TABINDEX);
							break;
						case 6:
							tmItemInfo.ItemUnitPrice = atof_AfterChangeCommaToDot((char*)Data);		// 2010-03-04 by cmkwon, atof관련 문제 해결(언어별 소숫점 구분자 변환) - 
							break;
						case 7:
							util::strncpy(tmItemInfo.ItemDescription, (char*)Data, SIZE_MAX_ITEMDESCRIPTION);
							break;
					}// end switch
				}// end if
			}// end for
			vectItemInfo.push_back(tmItemInfo);
			m_RecordSet->MoveNext();
	}// end while
}
void CLoadExcelFile::LoadExcelMonsterItem(void)
{
	while(m_RecordSet->EndOfFile == VARIANT_FALSE)
	{
			MONSTER2ITEM m2i;
			memset(&m2i, 0x00, sizeof(m2i));
			for(int i=0; i<m_RecordSet->Fields->GetCount(); ++i)
			{
				if(m_RecordSet->Fields->GetItem((long)i)->GetValue().vt != VT_EMPTY
					&&m_RecordSet->Fields->GetItem((long)i)->GetValue().vt != VT_NULL)
				{
					_bstr_t Data;
					Data = m_RecordSet->Fields->GetItem((long)i)->GetValue();
					switch(i)
					{
						case 0:
							m2i.MonsterUniqueNumber	= atoi((char*)Data);
							break;
						case 1:
							m2i.ItemNumber			= atoi((char*)Data);
							break;
						case 2:
							m2i.MinCount			= atoi((char*)Data);
							break;
						case 3:
							m2i.MaxCount			= atoi((char*)Data);
							break;
						case 4:
							m2i.Probability			= atoi((char*)Data);
							break;
						case 5:
							m2i.PrefixProbability	= atoi((char*)Data);
							break;
						case 6:
							m2i.SuffixProbability	= atoi((char*)Data);
							break;
						case 7:
							m2i.DropType			= atoi((char*)Data);	// 2010-04-09 by cmkwon, 인피2차 추가 수정(단계별 보상 추가) - 
							break;
					}// end switch
				}// end if
			}// end for
			vectMonsterItem.push_back(m2i);
			m_RecordSet->MoveNext();
	}// end while
}

void CLoadExcelFile::LoadExcelRareItemInfo(void)
{
	while(m_RecordSet->EndOfFile == VARIANT_FALSE)
	{
			RARE_ITEM_INFO rareItemInfo;
			memset(&rareItemInfo, 0x00, sizeof(rareItemInfo));
			for(int i=0; i<m_RecordSet->Fields->GetCount(); ++i)
			{
				if(m_RecordSet->Fields->GetItem((long)i)->GetValue().vt != VT_EMPTY
					&&m_RecordSet->Fields->GetItem((long)i)->GetValue().vt != VT_NULL)
				{
					_bstr_t Data;
					Data = m_RecordSet->Fields->GetItem((long)i)->GetValue();
					switch(i)
					{
						case 0:
							rareItemInfo.CodeNum = atoi((char*)Data);
							break;
						case 1:
							util::strncpy(rareItemInfo.Name,(char*)Data, SIZE_MAX_RARE_FIX_NAME);
							break;
						case 2:
							rareItemInfo.ReqUseType = atoi((char*)Data);
							break;
						case 3:
							rareItemInfo.ReqMinLevel = atoi((char*)Data);
							break;
						case 4:
							rareItemInfo.ReqMaxLevel = atoi((char*)Data);
							break;
						case 5:
							rareItemInfo.ReqItemKind = atoi((char*)Data);
							break;
						case 6:
							rareItemInfo.ReqGearStat.AttackPart = atoi((char*)Data);
							break;
						case 7:
							rareItemInfo.ReqGearStat.DefensePart = atoi((char*)Data);
							break;
						case 8:
							rareItemInfo.ReqGearStat.DodgePart = atoi((char*)Data);
							break;
						case 9:
							rareItemInfo.ReqGearStat.FuelPart = atoi((char*)Data);
							break;
						case 10:
							rareItemInfo.ReqGearStat.ShieldPart = atoi((char*)Data);
							break;
						case 11:
							rareItemInfo.ReqGearStat.SoulPart = atoi((char*)Data);
							break;
						case 12:
							rareItemInfo.DesParameter[0] = atoi((char*)Data);
							break;
						case 13:
							rareItemInfo.ParameterValue[0] = atof_AfterChangeCommaToDot((char*)Data);	// 2010-03-04 by cmkwon, atof관련 문제 해결(언어별 소숫점 구분자 변환) - 
							break;
						case 14:
							rareItemInfo.DesParameter[1] = atoi((char*)Data);
							break;
						case 15:
							rareItemInfo.ParameterValue[1] = atof_AfterChangeCommaToDot((char*)Data);	// 2010-03-04 by cmkwon, atof관련 문제 해결(언어별 소숫점 구분자 변환) - 
							break;
						case 16:
							rareItemInfo.DesParameter[2] = atoi((char*)Data);
							break;
						case 17:
							rareItemInfo.ParameterValue[2] = atof_AfterChangeCommaToDot((char*)Data);	// 2010-03-04 by cmkwon, atof관련 문제 해결(언어별 소숫점 구분자 변환) - 
							break;
						case 18:
							rareItemInfo.DesParameter[3] = atoi((char*)Data);
							break;
						case 19:
							rareItemInfo.ParameterValue[3] = atof_AfterChangeCommaToDot((char*)Data);	// 2010-03-04 by cmkwon, atof관련 문제 해결(언어별 소숫점 구분자 변환) - 
							break;
						case 20:
							rareItemInfo.DesParameter[4] = atoi((char*)Data);
							break;
						case 21:
							rareItemInfo.ParameterValue[4] = atof_AfterChangeCommaToDot((char*)Data);	// 2010-03-04 by cmkwon, atof관련 문제 해결(언어별 소숫점 구분자 변환) - 
							break;
						case 22:
							rareItemInfo.DesParameter[5] = atoi((char*)Data);
							break;
						case 23:
							rareItemInfo.ParameterValue[5] = atof_AfterChangeCommaToDot((char*)Data);	// 2010-03-04 by cmkwon, atof관련 문제 해결(언어별 소숫점 구분자 변환) - 
							break;
						case 24:
							rareItemInfo.DesParameter[6] = atoi((char*)Data);
							break;
						case 25:
							rareItemInfo.ParameterValue[6] = atof_AfterChangeCommaToDot((char*)Data);	// 2010-03-04 by cmkwon, atof관련 문제 해결(언어별 소숫점 구분자 변환) - 
							break;
						case 26:
							rareItemInfo.DesParameter[7] = atoi((char*)Data);
							break;
						case 27:
							rareItemInfo.ParameterValue[7] = atof_AfterChangeCommaToDot((char*)Data);	// 2010-03-04 by cmkwon, atof관련 문제 해결(언어별 소숫점 구분자 변환) - 
							break;
						case 28:
							rareItemInfo.DesParameter[8] = atoi((char*)Data);
							break;
						case 29:
							rareItemInfo.ParameterValue[8] = atof_AfterChangeCommaToDot((char*)Data);	// 2010-03-04 by cmkwon, atof관련 문제 해결(언어별 소숫점 구분자 변환) - 
							break;
						case 30:
							rareItemInfo.Probability = atoi((char*)Data);
							break;
					}// end switch
				}// end if
			}// end for
			vectRareItemInfo.push_back(rareItemInfo);
			m_RecordSet->MoveNext();
	}// end while
}

void CLoadExcelFile::LoadExcelShop(void)
{
	while(m_RecordSet->EndOfFile == VARIANT_FALSE)
	{
		SHOP_INFO tmShopInfo;
		memset(&tmShopInfo, 0x00, sizeof(tmShopInfo));
		for(int i=0; i<m_RecordSet->Fields->GetCount(); ++i)
		{
			if(m_RecordSet->Fields->GetItem((long)i)->GetValue().vt != VT_EMPTY
				&&m_RecordSet->Fields->GetItem((long)i)->GetValue().vt != VT_NULL)
			{
				_bstr_t Data;
				Data = m_RecordSet->Fields->GetItem((long)i)->GetValue();
				switch(i)
				{
					case 0:
						tmShopInfo.ShopOrder = atoi((char*)Data);
						break;
					case 1:
						tmShopInfo.UniqueNumber = atoi((char*)Data);
						break;
					case 2:
						tmShopInfo.ItemNum = atoi((char*)Data);
						break;
				}// end switch
			}// end if
		}// end for
		vectShopInfo.push_back(tmShopInfo);
		m_RecordSet->MoveNext();
	}// end while
}

void CLoadExcelFile::LoadExcelCityTargetWarpMap(void)
{
	while(m_RecordSet->EndOfFile == VARIANT_FALSE)
	{
		CITY_TARGET_WARPMAP tmTargetWarp;
		memset(&tmTargetWarp, 0x00, sizeof(tmTargetWarp));
		for(int i=0; i<m_RecordSet->Fields->GetCount(); ++i)
		{
			if(m_RecordSet->Fields->GetItem((long)i)->GetValue().vt != VT_EMPTY
				&&m_RecordSet->Fields->GetItem((long)i)->GetValue().vt != VT_NULL)
			{
				_bstr_t Data;
				Data = m_RecordSet->Fields->GetItem((long)i)->GetValue();
				switch(i)
				{
					case 0:
						tmTargetWarp.MapIndex = atoi((char*)Data);
						break;
					case 1:
						tmTargetWarp.BuildingIndex = atoi((char*)Data);
						break;
					case 2:
						tmTargetWarp.WarpTargetMapIndex = atoi((char*)Data);
						break;
					case 3:
						tmTargetWarp.WarpTargetIndex = atoi((char*)Data);
						break;
					case 4:
						util::strncpy(tmTargetWarp.WarpTargetName, (char*)Data, SIZE_MAX_WARP_TARGET_NAME);
						break;
					case 5:
						tmTargetWarp.Fee = atoi((char*)Data);
						break;
				}// end switch
			}// end if
		}// end for
		vectCityTargetWarpMap.push_back(tmTargetWarp);
		m_RecordSet->MoveNext();
	}// end while
}

void CLoadExcelFile::LoadExcelItemMixingInfo(void)
{
	while(m_RecordSet->EndOfFile == VARIANT_FALSE)
	{
			ITEM_MIXING_INFO_TOOL retMixingInfo;		// start 2012-02-20 by hskim, DB 정규화 - ItemMixingInfo
			memset(&retMixingInfo, 0x00, sizeof(retMixingInfo));
			for(int i=0; i<m_RecordSet->Fields->GetCount(); ++i)
			{
				if(m_RecordSet->Fields->GetItem((long)i)->GetValue().vt != VT_EMPTY
					&&m_RecordSet->Fields->GetItem((long)i)->GetValue().vt != VT_NULL)
				{
					_bstr_t Data;
					Data = m_RecordSet->Fields->GetItem((long)i)->GetValue();
					switch(i)
					{
						// start 2012-02-20 by hskim, DB 정규화 - ItemMixingInfo

						// 기존 
						/*
						case 0:
							retMixingInfo.TargetItemNum = atoi((char*)Data);
							break;
						case 1:
							retMixingInfo.MixingProbability = atoi((char*)Data);
							break;
						case 2:
							retMixingInfo.MixingCost = atoi((char*)Data);
							break;
						case 3:
							retMixingInfo.SourceItem[0].ItemNum = atoi((char*)Data);
							break;
						case 4:
							retMixingInfo.SourceItem[0].Count = atoi((char*)Data);
							break;
						case 5:
							retMixingInfo.SourceItem[1].ItemNum = atoi((char*)Data);
							break;
						case 6:
							retMixingInfo.SourceItem[1].Count = atoi((char*)Data);
							break;
						case 7:
							retMixingInfo.SourceItem[2].ItemNum = atoi((char*)Data);
							break;
						case 8:
							retMixingInfo.SourceItem[2].Count = atoi((char*)Data);
							break;
						case 9:
							retMixingInfo.SourceItem[3].ItemNum = atoi((char*)Data);
							break;
						case 10:
							retMixingInfo.SourceItem[3].Count = atoi((char*)Data);
							break;
						case 11:
							retMixingInfo.SourceItem[4].ItemNum = atoi((char*)Data);
							break;
						case 12:
							retMixingInfo.SourceItem[4].Count = atoi((char*)Data);
							break;
						*/

						// 수정
						case 0:
							retMixingInfo.UniqueID = atoi((char*)Data);
							break;
						case 1:
							retMixingInfo.TargetItemNum = atoi((char*)Data);
							break;
						case 2:
							retMixingInfo.MixingProbability = atoi((char*)Data);
							break;
						case 3:
							retMixingInfo.MixingCost = atoi((char*)Data);
							break;
						// 2013-05-06 by hskim, 아이템 미리 보기 (속성 값 추가)
						case 4:
							retMixingInfo.Visible = atoi((char*)Data);
						// end 2013-05-06 by hskim, 아이템 미리 보기 (속성 값 추가)
							break;
						// end 2012-02-20 by hskim, DB 정규화 - ItemMixingInfo
					}// end switch
				}// end if
			}// end for
			vectItemMixingInfo.push_back(retMixingInfo);
			m_RecordSet->MoveNext();
	}// end while
}

// start 2012-02-20 by hskim, DB 정규화 - ItemMixingInfo
void CLoadExcelFile::LoadExcelItemMixingElement(void)
{
	while(m_RecordSet->EndOfFile == VARIANT_FALSE)
	{
			ITEM_MIXING_ELEMENT_TOOL retMixingInfo;
			memset(&retMixingInfo, 0x00, sizeof(retMixingInfo));
			for(int i=0; i<m_RecordSet->Fields->GetCount(); ++i)
			{
				if(m_RecordSet->Fields->GetItem((long)i)->GetValue().vt != VT_EMPTY
					&&m_RecordSet->Fields->GetItem((long)i)->GetValue().vt != VT_NULL)
				{
					_bstr_t Data;
					Data = m_RecordSet->Fields->GetItem((long)i)->GetValue();
					switch(i)
					{
						case 0:
							retMixingInfo.UniqueID = atoi((char*)Data);
							break;
						case 1:
							retMixingInfo.SourceItem.ItemNum = atoi((char*)Data);
							break;
						case 2:
							retMixingInfo.SourceItem.Count = atoi((char*)Data);
							break;
					}// end switch
				}// end if
			}// end for
			vectItemMixingElement.push_back(retMixingInfo);
			m_RecordSet->MoveNext();
	}// end while
}
// end 2012-02-20 by hskim, DB 정규화 - ItemMixingInfo

void CLoadExcelFile::LoadExcelEnchantInfo(void)
{
	while(m_RecordSet->EndOfFile == VARIANT_FALSE)
	{
			ENCHANT_INFO retEnchantInfo;
			memset(&retEnchantInfo, 0x00, sizeof(retEnchantInfo));
			for(int i=0; i<m_RecordSet->Fields->GetCount(); ++i)
			{
				if(m_RecordSet->Fields->GetItem((long)i)->GetValue().vt != VT_EMPTY
					&&m_RecordSet->Fields->GetItem((long)i)->GetValue().vt != VT_NULL)
				{
					_bstr_t Data;
					Data = m_RecordSet->Fields->GetItem((long)i)->GetValue();
					switch(i)
					{
						case 0:
							retEnchantInfo.EnchantItemNum = atoi((char*)Data);
							break;
						case 1:
							retEnchantInfo.EnchantItemCount = atoi((char*)Data);
							break;
						case 2:
							retEnchantInfo.EnchantCost = atoi((char*)Data);
							break;
						case 3:
							retEnchantInfo.ProbabilityPerLevel[0] = atoi((char*)Data);
							break;
						case 4:
							retEnchantInfo.ProbabilityPerLevel[1] = atoi((char*)Data);
							break;
						case 5:
							retEnchantInfo.ProbabilityPerLevel[2] = atoi((char*)Data);
							break;
						case 6:
							retEnchantInfo.ProbabilityPerLevel[3] = atoi((char*)Data);
							break;
						case 7:
							retEnchantInfo.ProbabilityPerLevel[4] = atoi((char*)Data);
							break;
						case 8:
							retEnchantInfo.ProbabilityPerLevel[5] = atoi((char*)Data);
							break;
						case 9:
							retEnchantInfo.ProbabilityPerLevel[6] = atoi((char*)Data);
							break;
						case 10:
							retEnchantInfo.ProbabilityPerLevel[7] = atoi((char*)Data);
							break;
						case 11:
							retEnchantInfo.ProbabilityPerLevel[8] = atoi((char*)Data);
							break;
						case 12:
							retEnchantInfo.ProbabilityPerLevel[9] = atoi((char*)Data);
							break;
						case 13:
							retEnchantInfo.ProbabilityPerLevel[10] = atoi((char*)Data);
							break;
						case 14:
							retEnchantInfo.ProbabilityPerLevel[11] = atoi((char*)Data);
							break;
						case 15:
							retEnchantInfo.ProbabilityPerLevel[12] = atoi((char*)Data);
							break;
						case 16:
							retEnchantInfo.ProbabilityPerLevel[13] = atoi((char*)Data);
							break;
						case 17:
							retEnchantInfo.ProbabilityPerLevel[14] = atoi((char*)Data);
							break;
						case 18:
							retEnchantInfo.ProbabilityPerLevel[15] = atoi((char*)Data);
							break;
						case 19:
							retEnchantInfo.ProbabilityPerLevel[16] = atoi((char*)Data);
							break;
						case 20:
							retEnchantInfo.ProbabilityPerLevel[17] = atoi((char*)Data);
							break;
						case 21:
							retEnchantInfo.ProbabilityPerLevel[18] = atoi((char*)Data);
							break;
						case 22:
							retEnchantInfo.ProbabilityPerLevel[19] = atoi((char*)Data);
							break;
						case 23:
							retEnchantInfo.ProbabilityPerLevel[20] = atoi((char*)Data);
							break;
						case 24:
							retEnchantInfo.ProbabilityPerLevel[21] = atoi((char*)Data);
							break;
						case 25:
							retEnchantInfo.ProbabilityPerLevel[22] = atoi((char*)Data);
							break;
						case 26:
							retEnchantInfo.ProbabilityPerLevel[23] = atoi((char*)Data);
							break;
						case 27:
							retEnchantInfo.ProbabilityPerLevel[24] = atoi((char*)Data);
							break;
						case 28:
							retEnchantInfo.ProbabilityPerLevel[25] = atoi((char*)Data);
							break;
						case 29:
							retEnchantInfo.ProbabilityPerLevel[26] = atoi((char*)Data);
							break;
						case 30:
							retEnchantInfo.ProbabilityPerLevel[27] = atoi((char*)Data);
							break;
						case 31:
							retEnchantInfo.ProbabilityPerLevel[28] = atoi((char*)Data);
							break;
						case 32:
							retEnchantInfo.ProbabilityPerLevel[29] = atoi((char*)Data);
							break;
						case 33:
							retEnchantInfo.ProbabilityPerLevel[30] = atoi((char*)Data);
							break;
						case 34:
							retEnchantInfo.ProbabilityPerLevel[31] = atoi((char*)Data);
							break;
						case 35:
							retEnchantInfo.ProbabilityPerLevel[32] = atoi((char*)Data);
							break;
						case 36:
							retEnchantInfo.ProbabilityPerLevel[33] = atoi((char*)Data);
							break;
						case 37:
							retEnchantInfo.ProbabilityPerLevel[34] = atoi((char*)Data);
							break;
						case 38:
							retEnchantInfo.ProbabilityPerLevel[35] = atoi((char*)Data);
							break;
						case 39:
							retEnchantInfo.ProbabilityPerLevel[36] = atoi((char*)Data);
							break;
						case 40:
							retEnchantInfo.ProbabilityPerLevel[37] = atoi((char*)Data);
							break;
						case 41:
							retEnchantInfo.ProbabilityPerLevel[38] = atoi((char*)Data);
							break;
						case 42:
							retEnchantInfo.ProbabilityPerLevel[39] = atoi((char*)Data);
							break;
					}// end switch
				}// end if
			}// end for
			vectEnchantInfo.push_back(retEnchantInfo);
			m_RecordSet->MoveNext();
	}// end while
}

void CLoadExcelFile::LoadExcelStrategyPointSummonMapIndex(void)
{
	while(m_RecordSet->EndOfFile == VARIANT_FALSE)
	{
			STRATEGYPOINT_SUMMON_MAPINDEX tmStrategyPoint;
			memset(&tmStrategyPoint, 0x00, sizeof(tmStrategyPoint));
			for(int i=0; i<m_RecordSet->Fields->GetCount(); ++i)
			{
				if(m_RecordSet->Fields->GetItem((long)i)->GetValue().vt != VT_EMPTY
					&&m_RecordSet->Fields->GetItem((long)i)->GetValue().vt != VT_NULL)
				{
					_bstr_t Data;
					Data = m_RecordSet->Fields->GetItem((long)i)->GetValue();
					switch(i)
					{
						case 0:
							util::strncpy(tmStrategyPoint.MapName, (char*)Data, SIZE_MAX_MAP_NAME);
							break;
						case 1:
							tmStrategyPoint.MapIndex = atoi((char*)Data);
							break;
						case 2:
							tmStrategyPoint.StratrgyPiontNum = atoi((char*)Data);
							break;
					}// end switch
				}// end if
			}// end for
			vectStrategyPointSummonMapIndex.push_back(tmStrategyPoint);
			m_RecordSet->MoveNext();
	}// end while
}

void CLoadExcelFile::LoadExcelArenaInfo(void)
{
	while(m_RecordSet->EndOfFile == VARIANT_FALSE)
	{
			SDBARENA_INFO ArenaInfo;
			memset(&ArenaInfo, 0x00, sizeof(ArenaInfo));
			for(int i=0; i<m_RecordSet->Fields->GetCount(); ++i)
			{
				if(m_RecordSet->Fields->GetItem((long)i)->GetValue().vt != VT_EMPTY
					&&m_RecordSet->Fields->GetItem((long)i)->GetValue().vt != VT_NULL)
				{
					_bstr_t Data;
					Data = m_RecordSet->Fields->GetItem((long)i)->GetValue();
					switch(i)
					{
						case 0:
							ArenaInfo.ArenaMode = atoi((char*)Data);
							break;
						case 1:
							ArenaInfo.ReqMemberNum = atoi((char*)Data);
							break;
						case 2:
							ArenaInfo.PayInfluencePointWIN = atoi((char*)Data);
							break;
						case 3:
							ArenaInfo.PayInfluencePointLOSE = atoi((char*)Data);
							break;
						case 4:
							ArenaInfo.PayWarPointWIN = atoi((char*)Data);
							break;
						case 5:
							ArenaInfo.PayWarPointLOSE = atoi((char*)Data);
							break;
						//////////////////////////////////////////////////////////////////////////////
						// 2012-05-23 by jhseol, 아레나 추가개발 - 최소 보상 워포 필드 추가
						case 6:
							ArenaInfo.DefaultPayWarPointWIN = atoi((char*)Data);
							break;
						case 7:
							ArenaInfo.DefaultPayWarPointLOSE = atoi((char*)Data);
							break;
						// end 2012-05-23 by jhseol, 아레나 추가개발 - 최소 보상 워포 필드 추가
					}// end switch
				}// end if
			}// end for
			vectArenaInfo.push_back(ArenaInfo);
			m_RecordSet->MoveNext();
	}// end while
}

void CLoadExcelFile::LoadExcelArenaMapinfo(void)
{
	while(m_RecordSet->EndOfFile == VARIANT_FALSE)
	{
			SDBARENA_MAPINFO ArenaMapInfo;
			memset(&ArenaMapInfo, 0x00, sizeof(ArenaMapInfo));
			for(int i=0; i<m_RecordSet->Fields->GetCount(); ++i)
			{
				if(m_RecordSet->Fields->GetItem((long)i)->GetValue().vt != VT_EMPTY
					&&m_RecordSet->Fields->GetItem((long)i)->GetValue().vt != VT_NULL)
				{
					_bstr_t Data;
					Data = m_RecordSet->Fields->GetItem((long)i)->GetValue();
					switch(i)
					{
						case 0:
							ArenaMapInfo.ArenaMapIndex = atoi((char*)Data);
							break;
						case 1:
							ArenaMapInfo.ArenaMode = atoi((char*)Data);
							break;
						case 2:
							ArenaMapInfo.ReqMemberNum = atoi((char*)Data);
							break;
						case 3:
							ArenaMapInfo.PlayLimitedTime = atoi((char*)Data);
							break;
						case 4:
							ArenaMapInfo.WINCondition = atoi((char*)Data);
							break;
						case 5:
							ArenaMapInfo.LvDSupplyItemNum1 = atoi((char*)Data);
							break;
						case 6:
							ArenaMapInfo.LvDSupplyItemCount1 = atoi((char*)Data);
							break;
						case 7:
							ArenaMapInfo.LvDSupplyItemNum2 = atoi((char*)Data);
							break;
						case 8:
							ArenaMapInfo.LvDSupplyItemCount2 = atoi((char*)Data);
							break;
						case 9:
							ArenaMapInfo.LvDSupplyItemNum3 = atoi((char*)Data);
							break;
						case 10:
							ArenaMapInfo.LvDSupplyItemCount3 = atoi((char*)Data);
							break;
							// 2012-09-14 by jhseol, 아레나 추가개발 part3 - 아레나 지급 아이템 추가 (아레나 전용 탄약 재충전 카드)
#ifdef _ARENA_ADDITIONAL_SUPPLY_ITEMS
						case 11:
							ArenaMapInfo.LvDSupplyItemNum4 = atoi((char*)Data);
							break;
						case 12:
							ArenaMapInfo.LvDSupplyItemCount4 = atoi((char*)Data);
							break;
						case 13:
							ArenaMapInfo.LvDSupplyItemNum5 = atoi((char*)Data);
							break;
						case 14:
							ArenaMapInfo.LvDSupplyItemCount5 = atoi((char*)Data);
							break;
#endif
							// end 2012-09-14 by jhseol, 아레나 추가개발 part3 - 아레나 지급 아이템 추가 (아레나 전용 탄약 재충전 카드)
					}// end switch
				}// end if
			}// end for
			vectArenaMapInfo.push_back(ArenaMapInfo);
			m_RecordSet->MoveNext();
	}// end while
}

void CLoadExcelFile::LoadExcelMysteryItemDrop(void)
{
	while(m_RecordSet->EndOfFile == VARIANT_FALSE)
	{
			MYSTERY_ITEM_DROP MysteryItemDrop;
			memset(&MysteryItemDrop, 0x00, sizeof(MysteryItemDrop));
			for(int i=0; i<m_RecordSet->Fields->GetCount(); ++i)
			{
				if(m_RecordSet->Fields->GetItem((long)i)->GetValue().vt != VT_EMPTY
					&&m_RecordSet->Fields->GetItem((long)i)->GetValue().vt != VT_NULL)
				{
					_bstr_t Data;
					Data = m_RecordSet->Fields->GetItem((long)i)->GetValue();
					switch(i)
					{
						case 0:
							MysteryItemDrop.MysteryItemDropNum = atoi((char*)Data);
							break;
						case 1:
							MysteryItemDrop.ReqUnitKind = atoi((char*)Data);
							break;
						case 2:
							MysteryItemDrop.ReqMinLevel = atoi((char*)Data);
							break;
						case 3:
							MysteryItemDrop.ReqMaxLevel = atoi((char*)Data);
							break;
						case 4:
							MysteryItemDrop.DropItemNum = atoi((char*)Data);
							break;
						case 5:
							MysteryItemDrop.MinCount = atoi((char*)Data);
							break;
						case 6:
							MysteryItemDrop.MaxCount = atoi((char*)Data);
							break;
						case 7:
							MysteryItemDrop.Probability = atoi((char*)Data);
							break;
						case 8:
							MysteryItemDrop.PrefixProbability = atoi((char*)Data);
							break;
						case 9:
							MysteryItemDrop.SuffixProbability = atoi((char*)Data);
							break;
						case 10:
							MysteryItemDrop.Period = atoi((char*)Data);
							break;
						case 11:
							MysteryItemDrop.CountPerPeriod = atoi((char*)Data);
							break;
						case 12:
							char szTime[SIZE_MAX_SQL_DATETIME_STRING];

							_variant_t szTemp(Data);		// _variant_t 형 변환
							COleDateTime tmTime(szTemp);	// COleDateTime으로 시간 변환
							sprintf(szTime, "%4d-%2d-%2d %2d:%2d", tmTime.GetYear(), tmTime.GetMonth(), tmTime.GetDay(), tmTime.GetHour(), tmTime.GetMinute());
							MysteryItemDrop.Starttime.SetDateTime(szTime);
							break;
					}// end switch
				}// end if
			}// end for
			vectMysteryItem.push_back(MysteryItemDrop);
			m_RecordSet->MoveNext();
	}// end while
}

void CLoadExcelFile::LoadExcelInfluenceWar(void)
{
	while(m_RecordSet->EndOfFile == VARIANT_FALSE)
	{
			SDB_INFLUENCE_WAR_INFO InfluenceWar;
			memset(&InfluenceWar, 0x00, sizeof(InfluenceWar));
			for(int i=0; i<m_RecordSet->Fields->GetCount(); ++i)
			{
				if(m_RecordSet->Fields->GetItem((long)i)->GetValue().vt != VT_EMPTY
					&&m_RecordSet->Fields->GetItem((long)i)->GetValue().vt != VT_NULL)
				{
					_bstr_t Data;
					Data = m_RecordSet->Fields->GetItem((long)i)->GetValue();
					switch(i)
					{
						case 0:
							InfluenceWar.InfluenceType = atoi((char*)Data);
							break;
						case 1:
							InfluenceWar.WartimeStage = atoi((char*)Data);
							break;
						case 2:
							InfluenceWar.ReqContributionPoint = atoi((char*)Data);
							break;
						case 3:
							InfluenceWar.HPRepairRate = atof_AfterChangeCommaToDot((char*)Data);	// 2010-03-04 by cmkwon, atof관련 문제 해결(언어별 소숫점 구분자 변환) - 
							break;
						case 4:
							InfluenceWar.DPRepairRate = atof_AfterChangeCommaToDot((char*)Data);	// 2010-03-04 by cmkwon, atof관련 문제 해결(언어별 소숫점 구분자 변환) - 
							break;
						case 5:
							InfluenceWar.SPRepairRate = atof_AfterChangeCommaToDot((char*)Data);	// 2010-03-04 by cmkwon, atof관련 문제 해결(언어별 소숫점 구분자 변환) - 
							break;
						case 6:
							InfluenceWar.EventTime = atoi((char*)Data);
							break;
						case 7:
							InfluenceWar.EventExperience = atof_AfterChangeCommaToDot((char*)Data);	// 2010-03-04 by cmkwon, atof관련 문제 해결(언어별 소숫점 구분자 변환) - 
							break;
						case 8:
							InfluenceWar.EventDropSPI = atof_AfterChangeCommaToDot((char*)Data);	// 2010-03-04 by cmkwon, atof관련 문제 해결(언어별 소숫점 구분자 변환) - 
							break;
						case 9:
							InfluenceWar.EventDownExperience = atof_AfterChangeCommaToDot((char*)Data);	// 2010-03-04 by cmkwon, atof관련 문제 해결(언어별 소숫점 구분자 변환) - 
							break;
						case 10:
							InfluenceWar.EventDropItem = atof_AfterChangeCommaToDot((char*)Data);	// 2010-03-04 by cmkwon, atof관련 문제 해결(언어별 소숫점 구분자 변환) - 
							break;
						case 11:
							InfluenceWar.EventDropRareItem = atof_AfterChangeCommaToDot((char*)Data);	// 2010-03-04 by cmkwon, atof관련 문제 해결(언어별 소숫점 구분자 변환) - 
							break;
						case 12:
							InfluenceWar.SupplyItemNum = atoi((char*)Data);
							break;
						case 13:
							InfluenceWar.SupplyItemCount = atoi((char*)Data);
							break;
						case 14:
							InfluenceWar.IsSummonJacoMonster = atoi((char*)Data);
							break;
						case 15:
							InfluenceWar.BossMonsterUnitKind = atoi((char*)Data);
							break;
						case 16:
							InfluenceWar.BossMonsterCount = atoi((char*)Data);
							break;
						case 17:
							InfluenceWar.BossMonsterSummonTerm = atoi((char*)Data);
							break;
						case 18:
							InfluenceWar.BossMonsterSummonMapIndex = atoi((char*)Data);
							break;
						case 19:
							InfluenceWar.BossMonsterSummonPositionX = atoi((char*)Data);
							break;
						case 20:
							InfluenceWar.BossMonsterSummonPositionY = atoi((char*)Data);
							break;
						case 21:
							InfluenceWar.BossMonsterSummonPositionZ = atoi((char*)Data);
							break;
						case 22:
							InfluenceWar.SummonMonsterUnitKind[0] = atoi((char*)Data);
							break;
						case 23:
							InfluenceWar.SummonCount[0] = atoi((char*)Data);
							break;
						case 24:
							InfluenceWar.SummonMapIndex[0] = atoi((char*)Data);
							break;
						case 25:
							InfluenceWar.SummonPositionX[0] = atoi((char*)Data);
							break;
						case 26:
							InfluenceWar.SummonPositionY[0] = atoi((char*)Data);
							break;
						case 27:
							InfluenceWar.SummonPositionZ[0] = atoi((char*)Data);
							break;
						case 28:
							InfluenceWar.SummonMonsterUnitKind[1] = atoi((char*)Data);
							break;
						case 29:
							InfluenceWar.SummonCount[1] = atoi((char*)Data);
							break;
						case 30:
							InfluenceWar.SummonMapIndex[1] = atoi((char*)Data);
							break;
						case 31:
							InfluenceWar.SummonPositionX[1] = atoi((char*)Data);
							break;
						case 32:
							InfluenceWar.SummonPositionY[1] = atoi((char*)Data);
							break;
						case 33:
							InfluenceWar.SummonPositionZ[1] = atoi((char*)Data);
							break;
						case 34:
							InfluenceWar.SummonMonsterUnitKind[2] = atoi((char*)Data);
							break;
						case 35:
							InfluenceWar.SummonCount[2] = atoi((char*)Data);
							break;
						case 36:
							InfluenceWar.SummonMapIndex[2] = atoi((char*)Data);
							break;
						case 37:
							InfluenceWar.SummonPositionX[2] = atoi((char*)Data);
							break;
						case 38:
							InfluenceWar.SummonPositionY[2] = atoi((char*)Data);
							break;
						case 39:
							InfluenceWar.SummonPositionZ[2] = atoi((char*)Data);
							break;
						case 40:
							InfluenceWar.SummonMonsterUnitKind[3] = atoi((char*)Data);
							break;
						case 41:
							InfluenceWar.SummonCount[3] = atoi((char*)Data);
							break;
						case 42:
							InfluenceWar.SummonMapIndex[3] = atoi((char*)Data);
							break;
						case 43:
							InfluenceWar.SummonPositionX[3] = atoi((char*)Data);
							break;
						case 44:
							InfluenceWar.SummonPositionY[3] = atoi((char*)Data);
							break;
						case 45:
							InfluenceWar.SummonPositionZ[3] = atoi((char*)Data);
							break;
						case 46:
							InfluenceWar.SummonMonsterUnitKind[4] = atoi((char*)Data);
							break;
						case 47:
							InfluenceWar.SummonCount[4] = atoi((char*)Data);
							break;
						case 48:
							InfluenceWar.SummonMapIndex[4] = atoi((char*)Data);
							break;
						case 49:
							InfluenceWar.SummonPositionX[4] = atoi((char*)Data);
							break;
						case 50:
							InfluenceWar.SummonPositionY[4] = atoi((char*)Data);
							break;
						case 51:
							InfluenceWar.SummonPositionZ[4] = atoi((char*)Data);
							break;
						case 52:
							InfluenceWar.SupplyItemNum2 = atoi((char*)Data);
							break;
						case 53:
							InfluenceWar.SupplyItemCount2 = atoi((char*)Data);
							break;
						case 54:
							InfluenceWar.WinBossMonsterUnitKind = atoi((char*)Data);
							break;
						case 55:
							InfluenceWar.LossBossMonsterUnitKind = atoi((char*)Data);
							break;
					}// end switch
				}// end if
			}// end for
			vectInfluenceWarInfo.push_back(InfluenceWar);
			m_RecordSet->MoveNext();
	}// end while
}

void CLoadExcelFile::LoadExcelOutPostNextWarTime(void)
{
	while(m_RecordSet->EndOfFile == VARIANT_FALSE)
	{
			SOUTPOST_INFO tmOutPostNextWarTime;
			memset(&tmOutPostNextWarTime, 0x00, sizeof(tmOutPostNextWarTime));
			for(int i=0; i<m_RecordSet->Fields->GetCount(); ++i)
			{
				if(m_RecordSet->Fields->GetItem((long)i)->GetValue().vt != VT_EMPTY
					&&m_RecordSet->Fields->GetItem((long)i)->GetValue().vt != VT_NULL)
				{
					_bstr_t Data;
					Data = m_RecordSet->Fields->GetItem((long)i)->GetValue();
					switch(i)
					{
						case 0:
							char szTime[SIZE_MAX_SQL_DATETIME_STRING];

							_variant_t szTemp(Data);		// _variant_t 형 변환
							COleDateTime tmTime(szTemp);	// COleDateTime으로 시간 변환
							sprintf(szTime, "%4d-%2d-%2d %2d:%2d", tmTime.GetYear(), tmTime.GetMonth(), tmTime.GetDay(), tmTime.GetHour(), tmTime.GetMinute());
										
							tmOutPostNextWarTime.OutPostNextWarTime.SetDateTime(szTime);
							break;
					}// end switch
				}// end if
			}// end for
			vectOutPostInfo.push_back(tmOutPostNextWarTime);
			m_RecordSet->MoveNext();
	}// end while
}

void CLoadExcelFile::LoadExcelActionByLevel(void)
{
	while(m_RecordSet->EndOfFile == VARIANT_FALSE)
	{
			SACTION_BY_LEVEL_DB ActionByLevel;
			memset(&ActionByLevel, 0x00, sizeof(ActionByLevel));
			for(int i=0; i<m_RecordSet->Fields->GetCount(); ++i)
			{
				if(m_RecordSet->Fields->GetItem((long)i)->GetValue().vt != VT_EMPTY
					&&m_RecordSet->Fields->GetItem((long)i)->GetValue().vt != VT_NULL)
				{
					_bstr_t Data;
					Data = m_RecordSet->Fields->GetItem((long)i)->GetValue();
					switch(i)
					{
						case 0:
							ActionByLevel.Level = atoi((char*)Data);
							break;
						case 1:
							ActionByLevel.PollPoint = atoi((char*)Data);
							break;
					}// end switch
				}// end if
			}// end for
			vectActoinByLevel.push_back(ActionByLevel);
			m_RecordSet->MoveNext();
	}// end while
}

void CLoadExcelFile::LoadExcelMapInfo(void)
{
	while(m_RecordSet->EndOfFile == VARIANT_FALSE)
	{
			MAP_INFO MapInfo;
			memset(&MapInfo, 0x00, sizeof(MapInfo));
			for(int i=0; i<m_RecordSet->Fields->GetCount(); ++i)
			{
				if(m_RecordSet->Fields->GetItem((long)i)->GetValue().vt != VT_EMPTY
					&&m_RecordSet->Fields->GetItem((long)i)->GetValue().vt != VT_NULL)
				{
					_bstr_t Data;
					Data = m_RecordSet->Fields->GetItem((long)i)->GetValue();
					switch(i)
					{
						case 0:
							MapInfo.MapIndex = atoi((char*)Data);
							break;
						case 1:
							MapInfo.RenderMapIndex = atoi((char*)Data);
							break;
						case 2:
							MapInfo.BeforeMapIndex = atoi((char*)Data);
							break;
						case 3:
							util::strncpy(MapInfo.MapName,(char*)Data, SIZE_MAX_MAP_NAME);
							break;
						case 4:
							MapInfo.MapInfluenceType = atoi((char*)Data);
							break;
						case 5:
							MapInfo.CityMapIndex = atoi((char*)Data);
							break;
						case 6:
							MapInfo.CityWarQuestIndex = atoi((char*)Data);
							break;
						case 7:
							MapInfo.CityWarQuestNPCIndex = atoi((char*)Data);
							break;
						case 8:
							MapInfo.CityWarQuestMonsterUID = atoi((char*)Data);
							break;
						case 9:
							MapInfo.MaxUserCount = atoi((char*)Data);
							break;
						case 10:
							MapInfo.MaxGroundHeight = atoi((char*)Data);
							break;
						case 11:
							MapInfo.MinGroundHeight = atoi((char*)Data);
							break;
						case 12:
							MapInfo.WaterHeight = atoi((char*)Data);
							break;
						case 13:
							MapInfo.UserVisibleDistance = atoi((char*)Data);
							break;
						case 14:
							MapInfo.MonsterVisibleDistance = atoi((char*)Data);
							break;
						case 15:
							MapInfo.QuestIndexForWarp = atoi((char*)Data);
							break;
						case 16:
							MapInfo.FrontPositionDistance = atoi((char*)Data);
							break;
						case 17:
							MapInfo.AutoRecoveryFlag = atoi((char*)Data);
							break;
						case 18:
							MapInfo.DefaultMapIndex = atoi((char*)Data);
							break;
						case 19:
							MapInfo.CityWarpTargetPosition.x = atoi((char*)Data);
							break;
						case 20:
							MapInfo.CityWarpTargetPosition.y = atoi((char*)Data);
							break;
						case 21:
							MapInfo.CityWarpTargetPosition.z = atoi((char*)Data);
							break;
						case 22:
							MapInfo.ClientMaxAltitudeHeight = atoi((char*)Data);
							break;
						case 23:
							MapInfo.DayFogColor = atoi((char*)Data);
							break;
						case 24:
							MapInfo.NightFogColor = atoi((char*)Data);
							break;
						case 25:
							MapInfo.DayFogStartDistance = atoi((char*)Data);
							break;
						case 26:
							MapInfo.DayFogEndDistance = atoi((char*)Data);
							break;
						case 27:
							MapInfo.NightFogStartDistance = atoi((char*)Data);
							break;
						case 28:
							MapInfo.NightFogEndDistance = atoi((char*)Data);
							break;
						case 29:
							MapInfo.DaySunDirection.x = atoi((char*)Data);
							break;
						case 30:
							MapInfo.DaySunDirection.y = atoi((char*)Data);
							break;
						case 31:
							MapInfo.DaySunDirection.z = atoi((char*)Data);
							break;
						case 32:
							MapInfo.NightSunDirection.x = atoi((char*)Data);
							break;
						case 33:
							MapInfo.NightSunDirection.y = atoi((char*)Data);
							break;
						case 34:
							MapInfo.NightSunDirection.z = atoi((char*)Data);
							break;
						case 35:
							MapInfo.WaterType = atoi((char*)Data);
							break;
						case 36:
							MapInfo.PKZoneFlag = atoi((char*)Data);
							break;
						case 37:
							MapInfo.TileRenderingFlag = atoi((char*)Data);
							break;
						case 38:
							MapInfo.SkyRenderingFlag = atoi((char*)Data);
							break;
						case 39:
							MapInfo.SunRenderingFlag = atoi((char*)Data);
							break;
						case 40:
							MapInfo.FogRenderingFlag = atoi((char*)Data);
							break;
						case 41:
							MapInfo.VCNWarpObjectIndex = atoi((char*)Data);
							break;
						case 42:
							MapInfo.ANIWarpObjectIndex = atoi((char*)Data);
							break;
						case 43:
							MapInfo.Dat = atoi((char*)Data);
							break;
						case 44:
							MapInfo.Map = atoi((char*)Data);
							break;
						case 45:
							MapInfo.Tex = atoi((char*)Data);
							break;
						case 46:
							MapInfo.Cloud = atoi((char*)Data);
							break;
						case 47:
							MapInfo.Sky = atoi((char*)Data);
							break;
						case 48:
							MapInfo.Nsky = atoi((char*)Data);
							break;
						case 49:
							MapInfo.Bgm = atoi((char*)Data);
							break;
						case 50:
							MapInfo.TeleportWarpObjectIndex = atoi((char*)Data);
							break;
						case 51:
							MapInfo.BeforeMapIndex2 = atoi((char*)Data);
							break;
						case 52:	// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 
							util::strncpy(MapInfo.MapDescription,(char*)Data, SIZE_MAX_MAP_DESCRIPTION);
							break;
					}// end switch
				}// end if
			}// end for
			vectMapInfo.push_back(MapInfo);
			m_RecordSet->MoveNext();
	}// end while
}

void CLoadExcelFile::LoadExcelBuildingNPC(void)
{
	while(m_RecordSet->EndOfFile == VARIANT_FALSE)
	{
			BUILDINGNPC BuildingNpc;
			memset(&BuildingNpc, 0x00, sizeof(BuildingNpc));
			for(int i=0; i<m_RecordSet->Fields->GetCount(); ++i)
			{
				if(m_RecordSet->Fields->GetItem((long)i)->GetValue().vt != VT_EMPTY
					&&m_RecordSet->Fields->GetItem((long)i)->GetValue().vt != VT_NULL)
				{
					_bstr_t Data;
					Data = m_RecordSet->Fields->GetItem((long)i)->GetValue();
					switch(i)
					{
						case 0:
							BuildingNpc.BuildingIndex = atoi((char*)Data);
							break;
						case 1:
							util::strncpy(BuildingNpc.BuildingName, (char*)Data, SIZE_MAX_BUILDING_NAME);
							break;
						case 2:
							BuildingNpc.BuildingKind = atoi((char*)Data);
							break;
						case 3:
							BuildingNpc.MapIndex = atoi((char*)Data);
							break;
						case 4:
							BuildingNpc.NPCIndex = atoi((char*)Data);
							break;
						case 5:
							util::strncpy(BuildingNpc.NPCName, (char*)Data, SIZE_MAX_NPC_NAME);
							break;
						case 6:
							util::strncpy(BuildingNpc.GreetingTalk, (char*)Data, SIZE_MAX_BUILDING_NPC_TALK_STRING);
							break;
						case 7:
							BuildingNpc.OwnerCityMapIndex =  atoi((char*)Data);
							break;
						case 8:
							BuildingNpc.OwnerOrderNumber = atoi((char*)Data);
							break;
					}// end switch
				}// end if
			}// end for
			vectBuildingNpc.push_back(BuildingNpc);
			m_RecordSet->MoveNext();
	}// end while
}

void CLoadExcelFile::LoadExcelMonster(void)
{
	while(m_RecordSet->EndOfFile == VARIANT_FALSE)
	{
			MONSTER_DATA MonsterInfo;
			memset(&MonsterInfo, 0x00, sizeof(MonsterInfo));
			for(int i=0; i<m_RecordSet->Fields->GetCount(); ++i)
			{
				if(m_RecordSet->Fields->GetItem((long)i)->GetValue().vt != VT_EMPTY
					&&m_RecordSet->Fields->GetItem((long)i)->GetValue().vt != VT_NULL)
				{
					_bstr_t Data;
					Data = m_RecordSet->Fields->GetItem((long)i)->GetValue();
					switch(i)
					{
						case 0:
							MonsterInfo.MonsterUnitKind = atoi((char*)Data);
							break;
						case 1:
							util::strncpy(MonsterInfo.MonsterName, (char*)Data, SIZE_MAX_MONSTER_NAME);
							break;
						case 2:
							MonsterInfo.Level = atoi((char*)Data);
							break;
						case 3:
							MonsterInfo.Experience = _atoi64((char*)Data);		// 2009-09-23 by cmkwon, Excel to DB 관련 자료형 버그 수정 - 
							break;
						case 4:
							MonsterInfo.MonsterHP = atoi((char*)Data);
							break;
						case 5:
							MonsterInfo.Race = atoi((char*)Data);
							break;
						case 6:
							MonsterInfo.Speed = atoi((char*)Data);
							break;
						case 7:
							MonsterInfo.MonsterItem[0] = atoi((char*)Data);
							break;
						case 8:
							MonsterInfo.MonsterItem[1] = atoi((char*)Data);
							break;
						case 9:
							MonsterInfo.MonsterItem[2] = atoi((char*)Data);
							break;
						case 10:
							MonsterInfo.MonsterItem[3] = atoi((char*)Data);
							break;
						case 11:
							MonsterInfo.MonsterItem[4] = atoi((char*)Data);
							break;
						case 12:		// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 
							MonsterInfo.MonsterItem[5] = atoi((char*)Data);
							break;
						case 13:		// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 
							MonsterInfo.MonsterItem[6] = atoi((char*)Data);
							break;
						case 14:		// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 
							MonsterInfo.MonsterItem[7] = atoi((char*)Data);
							break;
						case 15:	// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 
							MonsterInfo.MonsterItem[8] = atoi((char*)Data);
							break;
						case 16:	// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 
							MonsterInfo.MonsterItem[9] = atoi((char*)Data);
							break;
						case 17:		// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 
							MonsterInfo.MonsterItem[10] = atoi((char*)Data);
							break;
						case 18:		// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 
							MonsterInfo.MonsterItem[11] = atoi((char*)Data);
							break;
						case 19:		// 2009-09-09 ~ 2010 by dhjin, 인피니티 -  
							MonsterInfo.MonsterItem[12] = atoi((char*)Data);
							break;
						case 20:	// 2009-09-09 ~ 2010 by dhjin, 인피니티 -  
							MonsterInfo.MonsterItem[13] = atoi((char*)Data);
							break;
						case 21:	// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 
							MonsterInfo.MonsterItem[14] = atoi((char*)Data);
							break;
						case 22:
							MonsterInfo.ControlSkil = atoi((char*)Data);
							break;
						case 23:
							MonsterInfo.SizeForServer = atoi((char*)Data);
							break;
						case 24:
							MonsterInfo.SizeForClient = atoi((char*)Data);
							break;
						case 25:
							MonsterInfo.Faction = atoi((char*)Data);
							break;
						case 26:
							MonsterInfo.MonsterForm = atoi((char*)Data);
							break;
						case 27:
							MonsterInfo.AttackPattern = atoi((char*)Data);
							break;
						case 28:
							MonsterInfo.MovePattern = atoi((char*)Data);
							break;
						case 29:
							MonsterInfo.Belligerence = atoi((char*)Data);
							break;
						case 30:
							MonsterInfo.Defense = atof_AfterChangeCommaToDot((char*)Data);			// 2010-07-19 by dhjin, 확률 수식 변경
							break;
						case 31:
							MonsterInfo.DefenseProbability = atof_AfterChangeCommaToDot((char*)Data);		// 2010-07-19 by dhjin, 확률 수식 변경
							break;
						case 32:
							MonsterInfo.AlphaBlending = atoi((char*)Data);
							break;
						case 33:
							MonsterInfo.AttackObject = atoi((char*)Data);
							break;
						case 34:
							MonsterInfo.AttackRange = atoi((char*)Data);
							break;
						case 35:
							MonsterInfo.HPRecoveryTime = atoi((char*)Data);
							break;
						case 36:
							MonsterInfo.HPRecoveryValue = atoi((char*)Data);
							break;
						case 37:
							MonsterInfo.RenderIndex = atoi((char*)Data);
							break;
						case 38:
							MonsterInfo.ScaleValue = atof_AfterChangeCommaToDot((char*)Data);	// 2010-03-04 by cmkwon, atof관련 문제 해결(언어별 소숫점 구분자 변환) - 
							break;
						case 39:
							MonsterInfo.TextureIndex = atoi((char*)Data);
							break;
						case 40:
							MonsterInfo.SourceIndex = atoi((char*)Data);
							break;
						case 41:
							MonsterInfo.QuickTurnAngle = atoi((char*)Data);
							break;
						case 42:
							MonsterInfo.QuickSpeed = atoi((char*)Data);
							break;
						case 43:
							MonsterInfo.QuickTurnTerm = atoi((char*)Data);
							break;
						case 44:
							MonsterInfo.MPOption = _atoi64((char*)Data);	// 2010-01-11 by cmkwon, 몬스터 MPOption 64bit로 변경 - 기존(atoi)
							break;
						case 45:
							MonsterInfo.MPOptionParam1 = atoi((char*)Data);
							break;
						case 46:
							MonsterInfo.MPOptionParam2 = atoi((char*)Data);
							break;
						case 47:
							MonsterInfo.Range = atoi((char*)Data);
							break;
						case 48:
							MonsterInfo.TurnAngle = atof_AfterChangeCommaToDot((char*)Data);	// 2010-03-04 by cmkwon, atof관련 문제 해결(언어별 소숫점 구분자 변환) - 
							break;
						case 49:
							MonsterInfo.MonsterHabit = atoi((char*)Data);
							break;
						case 50:
							MonsterInfo.ClickEvent = atoi((char*)Data);
							break;
						case 51:		// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 
							MonsterInfo.HPActionIdx = atoi((char*)Data);
							break;
						case 52:		// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 
							MonsterInfo.MonsterTarget = atoi((char*)Data);
							break;
						case 53:		// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 
							MonsterInfo.ChangeTarget = atoi((char*)Data);
							break;
						case 54:		// 2010-03-31 by dhjin, 인피니티(기지방어) -
							MonsterInfo.MonsterTarget2 = atoi((char*)Data);
							break;
						case 55:		// 2010-03-31 by dhjin, 인피니티(기지방어) -
							util::strncpy(MonsterInfo.PortraitFileName, (char*)Data, SIZE_MAX_FILE_NAME);
							break;
						case 56:		// 2010-04-14 by cmkwon, 인피2차 몬스터 랜텀 타겟 변경 처리 - ChangeTargetTime 필드 추가
							MonsterInfo.ChangeTargetTime	= atoi((char*)Data);
							break;
						case 57:		// 2011-05-23 by hskim, 인피니티 3차 - 웨이포인트 구현
							MonsterInfo.WayPointPattern		= atoi((char*)Data);
							break;
					}// end switch
				}// end if
			}// end for
			vectMonsterInfo.push_back(MonsterInfo);
			m_RecordSet->MoveNext();
	}// end while
}

void CLoadExcelFile::LoadExcelTutorialInfo(void)
{
	while(m_RecordSet->EndOfFile == VARIANT_FALSE)
	{
			STUTORIAL_INFO TutorialInfo;
			memset(&TutorialInfo, 0x00, sizeof(TutorialInfo));
			for(int i=0; i<m_RecordSet->Fields->GetCount(); ++i)
			{
				if(m_RecordSet->Fields->GetItem((long)i)->GetValue().vt != VT_EMPTY
					&&m_RecordSet->Fields->GetItem((long)i)->GetValue().vt != VT_NULL)
				{
					_bstr_t Data;
					Data = m_RecordSet->Fields->GetItem((long)i)->GetValue();
					switch(i)
					{
						case 0:
							TutorialInfo.TutorialUID = atoi((char*)Data);
							break;
						case 1:
							TutorialInfo.TutorialPayItem1 = atoi((char*)Data);
							break;
						case 2:
							TutorialInfo.TutorialPayItemCount1 = atoi((char*)Data);
							break;
						case 3:
							TutorialInfo.TutorialPayItem2 = atoi((char*)Data);
							break;
						case 4:
							TutorialInfo.TutorialPayItemCount2 = atoi((char*)Data);
							break;
						case 5:
							TutorialInfo.TutorialPayItem3 = atoi((char*)Data);
							break;
						case 6:
							TutorialInfo.TutorialPayItemCount3 = atoi((char*)Data);
							break;
					}// end switch
				}// end if
			}// end for
			vectTutorialInfo.push_back(TutorialInfo);
			m_RecordSet->MoveNext();
	}// end while
}

void CLoadExcelFile::LoadExcelMapObject(void)
{
	while(m_RecordSet->EndOfFile == VARIANT_FALSE)
	{
			MAPOBJECT tmMapObject;
			memset(&tmMapObject, 0x00, sizeof(tmMapObject));
			for(int i=0; i<m_RecordSet->Fields->GetCount(); ++i)
			{
				if(m_RecordSet->Fields->GetItem((long)i)->GetValue().vt != VT_EMPTY
					&&m_RecordSet->Fields->GetItem((long)i)->GetValue().vt != VT_NULL)
				{
					_bstr_t Data;
					Data = m_RecordSet->Fields->GetItem((long)i)->GetValue();
					switch(i)
					{
						case 0:
							util::strncpy(tmMapObject.ObjectName, (char*)Data, SIZE_MAX_OBJECT_NAME);
							break;
						case 1:
							tmMapObject.Code = atoi((char*)Data);
							break;
						case 2:
							tmMapObject.Alpha = atoi((char*)Data);
							break;
						case 3:
							tmMapObject.RadiusForServer = atoi((char*)Data);
							break;
						case 4:
							tmMapObject.RadiusForClient = atoi((char*)Data);
							break;
						case 5:
							tmMapObject.Collision = atoi((char*)Data);
							break;
						case 6:
							tmMapObject.CollisionForServer = atoi((char*)Data);
							break;
						case 7:
							tmMapObject.RenderIndex =  atoi((char*)Data);
							break;
						case 8:
							tmMapObject.ObjectRenderType = atoi((char*)Data);
							break;
						case 9:
							util::strncpy(tmMapObject.ObjectDescription, (char*)Data, SIZE_MAX_OBJECT_DESCRIPTION);
							break;
					}// end switch
				}// end if
			}// end for
			vectMapObject.push_back(tmMapObject);
			m_RecordSet->MoveNext();
	}// end while
}

void CLoadExcelFile::LoadExcelLuckyMachine()
{
	while(m_RecordSet->EndOfFile == VARIANT_FALSE)
	{
			LUCKY_MACHINE tmLuckyMachine;
			memset(&tmLuckyMachine, 0x00, sizeof(tmLuckyMachine));
			for(int i=0; i<m_RecordSet->Fields->GetCount(); ++i)
			{
				if(m_RecordSet->Fields->GetItem((long)i)->GetValue().vt != VT_EMPTY
					&&m_RecordSet->Fields->GetItem((long)i)->GetValue().vt != VT_NULL)
				{
					_bstr_t Data;
					Data = m_RecordSet->Fields->GetItem((long)i)->GetValue();
					switch(i)
					{
						case 0:
							tmLuckyMachine.order =  atoi((char*)Data);
							break;
						case 1:
							tmLuckyMachine.BuildingInx = atoi((char*)Data);
							break;
						case 2:
							tmLuckyMachine.MachineKind = atoi((char*)Data);
							break;
						case 3:
							tmLuckyMachine.MachineNum = atoi((char*)Data);
							break;
						case 4:
							tmLuckyMachine.CoinItemNum = atoi((char*)Data);
							break;
						case 5:
							tmLuckyMachine.SlotNum = atoi((char*)Data);
							break;
						case 6:
							tmLuckyMachine.SlotProbability = atoi((char*)Data);
							break;
						case 7:
							tmLuckyMachine.MysteryItemDropNum =  atoi((char*)Data);
							break;
						case 8:
							tmLuckyMachine.SourceIndex = atoi((char*)Data);
							break;
						case 9:
							util::strncpy(tmLuckyMachine.szDescription, (char*)Data, SIZE_MAX_LUCKYMACHINE_DESCRIPTION);
							break;
					}// end switch
				}// end if
			}// end for
			vectLuckyMachine.push_back(tmLuckyMachine);
			m_RecordSet->MoveNext();
	}// end while
}

void CLoadExcelFile::LoadExcelQuestMatching()
{
	while(m_RecordSet->EndOfFile == VARIANT_FALSE)
	{
			SQUEST_MATCHING tmQuestMatching;
			memset(&tmQuestMatching, 0x00, sizeof(tmQuestMatching));
			for(int i=0; i<m_RecordSet->Fields->GetCount(); ++i)
			{
				if(m_RecordSet->Fields->GetItem((long)i)->GetValue().vt != VT_EMPTY
					&&m_RecordSet->Fields->GetItem((long)i)->GetValue().vt != VT_NULL)
				{
					_bstr_t Data;
					Data = m_RecordSet->Fields->GetItem((long)i)->GetValue();
					switch(i)
					{
						case 0:
							tmQuestMatching.QuestIndexForBCU =  atoi((char*)Data);
							break;
						case 1:
							tmQuestMatching.QuestIndexForANI = atoi((char*)Data);
							break;
					}// end switch
				}// end if
			}// end for
			vectQuestMatching.push_back(tmQuestMatching);
			m_RecordSet->MoveNext();
	}// end while
}

void CLoadExcelFile::LoadExcelItemMatching()
{
	while(m_RecordSet->EndOfFile == VARIANT_FALSE)
	{
			SITEM_MATCHING tmItemMatching;
			memset(&tmItemMatching, 0x00, sizeof(tmItemMatching));
			for(int i=0; i<m_RecordSet->Fields->GetCount(); ++i)
			{
				if(m_RecordSet->Fields->GetItem((long)i)->GetValue().vt != VT_EMPTY
					&&m_RecordSet->Fields->GetItem((long)i)->GetValue().vt != VT_NULL)
				{
					_bstr_t Data;
					Data = m_RecordSet->Fields->GetItem((long)i)->GetValue();
					switch(i)
					{
						case 0:
							tmItemMatching.ItemNumForBCU =  atoi((char*)Data);
							break;
						case 1:
							tmItemMatching.ItemNumForANI = atoi((char*)Data);
							break;
					}// end switch
				}// end if
			}// end for
			vectItemMatching.push_back(tmItemMatching);
			m_RecordSet->MoveNext();
	}// end while
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2009-09-16 by cmkwon, 세력 초기화시 어뷰징 방지 구현 - ti_InfluenceRate 테이블 추가, 
/// \author		cmkwon
/// \date		2009-09-23 ~ 2009-09-23
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CLoadExcelFile::LoadExceInfluenceRate(void)
{
	while(m_RecordSet->EndOfFile == VARIANT_FALSE)
	{
			STI_INFLUENCERATE tmInfluenceRate;
			memset(&tmInfluenceRate, 0x00, sizeof(tmInfluenceRate));
			for(int i=0; i<m_RecordSet->Fields->GetCount(); ++i)
			{
				if(m_RecordSet->Fields->GetItem((long)i)->GetValue().vt != VT_EMPTY
					&&m_RecordSet->Fields->GetItem((long)i)->GetValue().vt != VT_NULL)
				{
					_bstr_t Data;
					Data = m_RecordSet->Fields->GetItem((long)i)->GetValue();
					switch(i)
					{
						case 0:
							tmInfluenceRate.StartLevel =  atoi((char*)Data);
							break;
						case 1:
							tmInfluenceRate.EndLevel = atoi((char*)Data);
							break;
					}// end switch
				}// end if
			}// end for
			vectInfluenceRate.push_back(tmInfluenceRate);
			m_RecordSet->MoveNext();
	}// end while
}

////////////////////////////////////////////////////////////////////////////////
// 2009-09-09 ~ 2010 by dhjin, 인피니티 - Excel -> DB 밀기
void CLoadExcelFile::LoadExcelHPAction(void) {
	while(m_RecordSet->EndOfFile == VARIANT_FALSE) {
		HPACTION tmHPAction;
		memset(&tmHPAction, 0x00, sizeof(HPACTION));
		for(int i=0; i<m_RecordSet->Fields->GetCount(); ++i) {
			if(m_RecordSet->Fields->GetItem((long)i)->GetValue().vt != VT_EMPTY
				&&m_RecordSet->Fields->GetItem((long)i)->GetValue().vt != VT_NULL) {
				_bstr_t Data;
				Data = m_RecordSet->Fields->GetItem((long)i)->GetValue();
				switch(i) {
				case 0:
					tmHPAction.HPActionUID =  atoi((char*)Data);
					break;
				case 1:
					tmHPAction.HPActionNum = atoi((char*)Data);
					break;
				case 2:
					tmHPAction.HPMaxValueRate = atoi((char*)Data);
					break;
				case 3:
					tmHPAction.HPMinValueRate = atoi((char*)Data);
					break;
				case 4:
					tmHPAction.UseItemArrayIdx = atoi((char*)Data);
					break;
				case 5:
					tmHPAction.NextUseItemArrayIdx = atoi((char*)Data);
					break;
				case 6:
					tmHPAction.HitRate = atoi((char*)Data);
					break;
				case 7:
					tmHPAction.UseCount =  atoi((char*)Data);
					break;
				case 8:
					tmHPAction.HPTalkCondition = (eHPACTION_TALK_CONDITION)atoi((char*)Data);
					break;
				case 9:
					tmHPAction.HPTalkImportance = (eHPACTION_TALK_IMPORTANCE)atoi((char*)Data);
					break;
				case 10:
					tmHPAction.PreHPCameraTremble =  atoi((char*)Data);
					break;
				case 11:
					util::strncpy(tmHPAction.PreHPTalk, (char*)Data, SIZE_MAX_HPTALK_DESCRIPTION);
					break;
				case 12:
					tmHPAction.HPCameraTremble =  atoi((char*)Data);
					break;
				case 13:
					util::strncpy(tmHPAction.HPTalk, (char*)Data, SIZE_MAX_HPTALK_DESCRIPTION);
					break;
				}// end switch
			}// end if
		}// end for
		HPActionList.push_back(tmHPAction);
		m_RecordSet->MoveNext();
	}// end while
}

void CLoadExcelFile::LoadExcelCinema(void) {
	while(m_RecordSet->EndOfFile == VARIANT_FALSE) {
		CINEMAINFO tmCinemaInfo;
		memset(&tmCinemaInfo, 0x00, sizeof(CINEMAINFO));
		for(int i=0; i<m_RecordSet->Fields->GetCount(); ++i) {
			if(m_RecordSet->Fields->GetItem((long)i)->GetValue().vt != VT_EMPTY
				&&m_RecordSet->Fields->GetItem((long)i)->GetValue().vt != VT_NULL) {
				_bstr_t Data;
				Data = m_RecordSet->Fields->GetItem((long)i)->GetValue();
				switch(i) {
				case 0:
					tmCinemaInfo.CinemaNum =  atoi((char*)Data);
					break;
				case 1:
					tmCinemaInfo.CinemaOrder = atoi((char*)Data);
					break;
				case 2:
					tmCinemaInfo.StartTime = atoi((char*)Data);
					break;
				case 3:
					tmCinemaInfo.EffectIdx = atoi((char*)Data);
					break;
				case 4:
					tmCinemaInfo.EffectPosition_X = atof_AfterChangeCommaToDot((char*)Data);	// 2010-03-04 by cmkwon, atof관련 문제 해결(언어별 소숫점 구분자 변환) - 
					break;
				case 5:
					tmCinemaInfo.EffectPosition_Y = atof_AfterChangeCommaToDot((char*)Data);	// 2010-03-04 by cmkwon, atof관련 문제 해결(언어별 소숫점 구분자 변환) - 
					break;
				case 6:
					tmCinemaInfo.EffectPosition_Z = atof_AfterChangeCommaToDot((char*)Data);	// 2010-03-04 by cmkwon, atof관련 문제 해결(언어별 소숫점 구분자 변환) - 
					break;
				case 7:
					tmCinemaInfo.EffectPlayTime =  atoi((char*)Data);
					break;
				case 8:
					tmCinemaInfo.EffectPlayCount = atoi((char*)Data);
					break;
				case 9:
					tmCinemaInfo.ObjectIdx = atoi((char*)Data);
					break;
				case 10:
					tmCinemaInfo.ChangeObjectIdx = atoi((char*)Data);
					break;
				case 11:
					tmCinemaInfo.ChangeTime = atoi((char*)Data);
					break;
				case 12:
					tmCinemaInfo.CameraTremble = atoi((char*)Data);
					break;
				case 13:
					// start 2011-03-17 by hskim, 인피니티 3차
					tmCinemaInfo.KeyMonster = (KeyMonster_t)atoi((char*)Data);
					// end 2011-05-13 by hskim, 인피니티 3차
					break;
				case 14:
					tmCinemaInfo.MonsterIdx = atoi((char*)Data);
					break;
				case 15:
					tmCinemaInfo.SummonCount = atoi((char*)Data);
					break;
				case 16:
					tmCinemaInfo.SummonRandomPos = atoi((char*)Data);
					break;
				case 17:
					tmCinemaInfo.ObjectBodyConditionIdx = atoi((char*)Data);
					break;
				case 18:
					tmCinemaInfo.BodyCondition = _atoi64((char*)Data);
					break;
				case 19:
					util::strncpy(tmCinemaInfo.TipFileName, (char*)Data, SIZE_MAX_FILE_NAME);
					break;
				case 20:
					util::strncpy(tmCinemaInfo.SkyFileName, (char*)Data, SIZE_MAX_FILE_NAME);
					break;	
				case 21:
					util::strncpy(tmCinemaInfo.CinemaTalk, (char*)Data, SIZE_MAX_CINEMATALK_DESCRIPTION);
					break;
				case 22:		// 2010-03-29 by cmkwon, 인피2차 시네마에 효과음 파일 설정 추가 - 
					util::strncpy(tmCinemaInfo.SoundFileName, (char*)Data, SIZE_MAX_FILE_NAME);
					break;
                // start 2011-04-28 by hskim, 인피니티 3차 - 시네마 관련 기능 추가 - 해당 맵채널 특정 몬스터 삭제 기능 추가
				case 23:		// 시네마 퀘스트 인덱스 설정 추가
					tmCinemaInfo.QuestIndex = atoi((char*)Data);
					break;
				case 24:
					tmCinemaInfo.MonsterIdxType = (eCINEMA_MONSTER_INDEX_TYPE)atoi((char*)Data);
					break;
				case 25:
					tmCinemaInfo.CustomIdx = atoi((char*)Data);
					break;
				case 26:
					tmCinemaInfo.UpdateMonsterIdx = atoi((char*)Data);
					break;
                // end 2011-04-28 by hskim, 인피니티 3차 - 시네마 관련 기능 추가 - 해당 맵채널 특정 몬스터 삭제 기능 추가
				}// end switch
			}// end if
		}// end for
		CinemaInfoList.push_back(tmCinemaInfo);
		m_RecordSet->MoveNext();
	}// end while
}

void CLoadExcelFile::LoadExcelInfinityMonster(void) {
	while(m_RecordSet->EndOfFile == VARIANT_FALSE) {
		INFINITY_MONSTERINFO tmInfinityMonsterInfo;
		memset(&tmInfinityMonsterInfo, 0x00, sizeof(INFINITY_MONSTERINFO));
		for(int i=0; i<m_RecordSet->Fields->GetCount(); ++i) {
			if(m_RecordSet->Fields->GetItem((long)i)->GetValue().vt != VT_EMPTY
				&&m_RecordSet->Fields->GetItem((long)i)->GetValue().vt != VT_NULL) {
				_bstr_t Data;
				Data = m_RecordSet->Fields->GetItem((long)i)->GetValue();
				switch(i) {
				case 0:
					tmInfinityMonsterInfo.InfinityMonsterUID =  atoi((char*)Data);
					break;
				case 1:
					tmInfinityMonsterInfo.InfinityMonsterIdx = atoi((char*)Data);
					break;
				case 2:
					tmInfinityMonsterInfo.MonsterIdx = atoi((char*)Data);
					break;
				case 3:
					tmInfinityMonsterInfo.TargetType = atoi((char*)Data);
					break;
				case 4:
					tmInfinityMonsterInfo.TargetCount = atoi((char*)Data);
					break;
				}// end switch
			}// end if
		}// end for
		InfinityMonsterInfoList.push_back(tmInfinityMonsterInfo);
		m_RecordSet->MoveNext();
	}// end while
}

void CLoadExcelFile::LoadExcelRevision(void) {
	while(m_RecordSet->EndOfFile == VARIANT_FALSE) {
		REVISIONINFO		tmRevisionInfo;
		memset(&tmRevisionInfo, 0x00, sizeof(REVISIONINFO));
		for(int i=0; i<m_RecordSet->Fields->GetCount(); ++i) {
			if(m_RecordSet->Fields->GetItem((long)i)->GetValue().vt != VT_EMPTY
				&&m_RecordSet->Fields->GetItem((long)i)->GetValue().vt != VT_NULL) {
				_bstr_t Data;
				Data = m_RecordSet->Fields->GetItem((long)i)->GetValue();
				switch(i) {
				case 0:
					tmRevisionInfo.RevisionNum =  atoi((char*)Data);
					break;
				case 1:
					tmRevisionInfo.UnitKind = atoi((char*)Data);
					break;
				case 2:
					tmRevisionInfo.RevisionLevel = atoi((char*)Data);
					break;
				case 3:
					tmRevisionInfo.RevisionHP = atoi((char*)Data);
					break;
				case 4:
					tmRevisionInfo.RevisionDP = atoi((char*)Data);
					break;
				case 5:
					tmRevisionInfo.RevisionStandardWeapon = atoi((char*)Data);
					break;
				case 6:
					tmRevisionInfo.RevisionAdvancedWeapon = atoi((char*)Data);
					break;
				}// end switch
			}// end if
		}// end for
		RevisionInfoList.push_back(tmRevisionInfo);
		m_RecordSet->MoveNext();
	}// end while
}

void CLoadExcelFile::LoadExcelInfinityMode(void) {
	while(m_RecordSet->EndOfFile == VARIANT_FALSE) {
		INFINITY_MODEINFO		tmInfinityModeInfo;
		memset(&tmInfinityModeInfo, 0x00, sizeof(INFINITY_MODEINFO));
		for(int i=0; i<m_RecordSet->Fields->GetCount(); ++i) {
			if(m_RecordSet->Fields->GetItem((long)i)->GetValue().vt != VT_EMPTY
				&&m_RecordSet->Fields->GetItem((long)i)->GetValue().vt != VT_NULL) {
				_bstr_t Data;
				Data = m_RecordSet->Fields->GetItem((long)i)->GetValue();
				switch(i) {
				case 0:
					tmInfinityModeInfo.InfinityModeUID =  atoi((char*)Data);
					break;
				case 1:
					tmInfinityModeInfo.MapIdx = atoi((char*)Data);
					break;
				case 2:
					tmInfinityModeInfo.ModeTypeNum = (eINFINITY_MODE)atoi((char*)Data);
					break;
				case 3:
					tmInfinityModeInfo.InfinityMonsterIdx = atoi((char*)Data);
					break;
				case 4:
					tmInfinityModeInfo.CinemaNum = atoi((char*)Data);
					break;
				case 5:
					tmInfinityModeInfo.RevisionNum = atoi((char*)Data);
					break;
				case 6:
					tmInfinityModeInfo.ResetCycle = atoi((char*)Data);
					break;
				case 7:
					tmInfinityModeInfo.EntranceCount =  atoi((char*)Data);
					break;
				case 8:
					tmInfinityModeInfo.LimitTime = atoi((char*)Data);
					break;
				case 9:
					tmInfinityModeInfo.MinLv = atoi((char*)Data);
					break;
				case 10:
					tmInfinityModeInfo.MaxLv = atoi((char*)Data);
					break;
				case 11:
					tmInfinityModeInfo.MinAdmissionCount = atoi((char*)Data);
					break;
				case 12:
					tmInfinityModeInfo.MaxAdmissionCount = atoi((char*)Data);
					break;
				case 13:	// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 사망시 패널티 추가
					tmInfinityModeInfo.TimePenaltyValue = atoi((char*)Data);
					break;	
				case 14:	// 2011-06-14 by hskim, 인피니티 3차 - 패널티 기능 추가 (HP 및 시간 동시 지원을 위해)
					tmInfinityModeInfo.HPPenaltyValue = atoi((char*)Data);
					break;	
				}// end switch
			}// end if
		}// end for
		InfinityModeInfoList.push_back(tmInfinityModeInfo);
		m_RecordSet->MoveNext();
	}// end while
}

void CLoadExcelFile::LoadExcelTender(void) {
	while(m_RecordSet->EndOfFile == VARIANT_FALSE) {
		TENDER_INFO tmTenderInfo;
		memset(&tmTenderInfo, 0x00, sizeof(TENDER_INFO));
		for(int i=0; i<m_RecordSet->Fields->GetCount(); ++i) {
			if(m_RecordSet->Fields->GetItem((long)i)->GetValue().vt != VT_EMPTY
				&&m_RecordSet->Fields->GetItem((long)i)->GetValue().vt != VT_NULL) {
				_bstr_t Data;
				Data = m_RecordSet->Fields->GetItem((long)i)->GetValue();
				switch(i) {
				case 0:
					tmTenderInfo.TenderUID =  atoi((char*)Data);
					break;
				case 1:
					tmTenderInfo.TenderItemNum = atoi((char*)Data);
					break;
				}// end switch
			}// end if
		}// end for
		TenderInfoList.push_back(tmTenderInfo);
		m_RecordSet->MoveNext();
	}// end while
}

// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 상점
void CLoadExcelFile::LoadExcelInfinityShop(void) {
	while(m_RecordSet->EndOfFile == VARIANT_FALSE) {
		INFINITY_SHOP_INFO tmInfinityShopInfo;
		memset(&tmInfinityShopInfo, 0x00, sizeof(INFINITY_SHOP_INFO));
		for(int i=0; i<m_RecordSet->Fields->GetCount(); ++i) {
			if(m_RecordSet->Fields->GetItem((long)i)->GetValue().vt != VT_EMPTY
				&&m_RecordSet->Fields->GetItem((long)i)->GetValue().vt != VT_NULL) {
				_bstr_t Data;
				Data = m_RecordSet->Fields->GetItem((long)i)->GetValue();
				switch(i) {
				case 0:
					tmInfinityShopInfo.InfinityShopUID =  atoi((char*)Data);
					break;
				case 1:
					tmInfinityShopInfo.BuyItemNum =  atoi((char*)Data);
					break;
				case 2:
					tmInfinityShopInfo.BuyItemCount =  atoi((char*)Data);
					break;
				case 3:
					tmInfinityShopInfo.TradeItemNum1 = atoi((char*)Data);
					break;
				case 4:
					tmInfinityShopInfo.TradeItemCount1 = atoi((char*)Data);
					break;
				case 5:
					tmInfinityShopInfo.TradeItemNum2 = atoi((char*)Data);
					break;
				case 6:
					tmInfinityShopInfo.TradeItemCount2 = atoi((char*)Data);
					break;
				case 7:
					tmInfinityShopInfo.TradeItemNum3 = atoi((char*)Data);
					break;
				case 8:
					tmInfinityShopInfo.TradeItemCount3 = atoi((char*)Data);
					break;
				case 9:
					tmInfinityShopInfo.TradeItemNum4 = atoi((char*)Data);
					break;
				case 10:
					tmInfinityShopInfo.TradeItemCount4 = atoi((char*)Data);
					break;
				case 11:
					tmInfinityShopInfo.TradeItemNum5 = atoi((char*)Data);
					break;
				case 12:
					tmInfinityShopInfo.TradeItemCount5 = atoi((char*)Data);
					break;
				}// end switch
			}// end if
		}// end for
		InfinityShopInfoList.push_back(tmInfinityShopInfo);
		m_RecordSet->MoveNext();
	}// end while
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2010-01-18 by cmkwon, 아이템 사용시 Parameter 중복 체크 시스템 구현 - 
/// \author		cmkwon
/// \date		2010-01-18 ~ 2010-01-18
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CLoadExcelFile::LoadExcelItemParamOverlap(void)
{
	while(m_RecordSet->EndOfFile == VARIANT_FALSE)
	{
		SITEM_PARAM_OVERLAP tmParmOverlap;
		util::zero(&tmParmOverlap, sizeof(tmParmOverlap));

		for(int i=0; i<m_RecordSet->Fields->GetCount(); ++i)
		{
			if(m_RecordSet->Fields->GetItem((long)i)->GetValue().vt != VT_EMPTY
				&&m_RecordSet->Fields->GetItem((long)i)->GetValue().vt != VT_NULL)
			{
				_bstr_t Data;
				Data = m_RecordSet->Fields->GetItem((long)i)->GetValue();
				switch(i) {
				case 0:
					tmParmOverlap.ItemNum			= atoi((char*)Data);
					break;
				case 1:
					tmParmOverlap.OverlapIndex		= atoi((char*)Data);
					break;				
				}// end switch
			}// end if
		} // END - for(int i=0; i<m_RecordSet->Fields->GetCount(); ++i)

		ItemParamOverlapList.push_back(tmParmOverlap);
		m_RecordSet->MoveNext();

	} // END - while(m_RecordSet->EndOfFile == VARIANT_FALSE)
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CLoadExcelFile::LoadExcelInvokingWearItemDPNum(void)
/// \brief		인피니티 - 발동류장착아이템
/// \author		dhjin
/// \date		2009-09-09 ~ 2010-02-10
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CLoadExcelFile::LoadExcelInvokingWearItemDPNum(void) {
	while(m_RecordSet->EndOfFile == VARIANT_FALSE)
	{
		INVOKING_WEAR_ITEM_DESTPARAM InvokingItemDP;
		util::zero(&InvokingItemDP, sizeof(INVOKING_WEAR_ITEM_DESTPARAM));
		
		for(int i=0; i<m_RecordSet->Fields->GetCount(); ++i)
		{
			if(m_RecordSet->Fields->GetItem((long)i)->GetValue().vt != VT_EMPTY
				&&m_RecordSet->Fields->GetItem((long)i)->GetValue().vt != VT_NULL)
			{
				_bstr_t Data;
				Data = m_RecordSet->Fields->GetItem((long)i)->GetValue();
				switch(i) {
				case 0:
					InvokingItemDP.InvokingDestParamID			= atoi((char*)Data);
					break;
				case 1:
					InvokingItemDP.InvokingDestParam		= atoi((char*)Data);
					break;
				case 2:
					InvokingItemDP.InvokingDestParamValue		= atof_AfterChangeCommaToDot((char*)Data);	// 2010-03-04 by cmkwon, atof관련 문제 해결(언어별 소숫점 구분자 변환) - 
					break;
				case 3:
					InvokingItemDP.InvokingEffectIdx		= atoi((char*)Data);
					break;
				}// end switch
			}// end if
		} // END - for(int i=0; i<m_RecordSet->Fields->GetCount(); ++i)
		
		InvokingItemDPList.push_back(InvokingItemDP);
		m_RecordSet->MoveNext();
		
	} // END - while(m_RecordSet->EndOfFile == VARIANT_FALSE)
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CLoadExcelFile::LoadExcelInvokingWearItemDPNumByUse(void)
/// \brief		인피니티 - 발동류장착아이템
/// \author		dhjin
/// \date		2009-09-09 ~ 2010-02-10
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CLoadExcelFile::LoadExcelInvokingWearItemDPNumByUse(void) {
	while(m_RecordSet->EndOfFile == VARIANT_FALSE)
	{
		INVOKING_WEAR_ITEM_DESTPARAM InvokingItemDP;
		util::zero(&InvokingItemDP, sizeof(INVOKING_WEAR_ITEM_DESTPARAM));
		
		for(int i=0; i<m_RecordSet->Fields->GetCount(); ++i)
		{
			if(m_RecordSet->Fields->GetItem((long)i)->GetValue().vt != VT_EMPTY
				&&m_RecordSet->Fields->GetItem((long)i)->GetValue().vt != VT_NULL)
			{
				_bstr_t Data;
				Data = m_RecordSet->Fields->GetItem((long)i)->GetValue();
				switch(i) {
				case 0:
					InvokingItemDP.InvokingDestParamID			= atoi((char*)Data);
					break;
				case 1:
					InvokingItemDP.InvokingDestParam		= atoi((char*)Data);
					break;
				case 2:
					InvokingItemDP.InvokingDestParamValue		= atof_AfterChangeCommaToDot((char*)Data);	// 2010-03-04 by cmkwon, atof관련 문제 해결(언어별 소숫점 구분자 변환) - 
					break;
				case 3:
					InvokingItemDP.InvokingEffectIdx		= atoi((char*)Data);
					break;
				}// end switch
			}// end if
		} // END - for(int i=0; i<m_RecordSet->Fields->GetCount(); ++i)
		
		InvokingItemDPByUseList.push_back(InvokingItemDP);
		m_RecordSet->MoveNext();
		
	} // END - while(m_RecordSet->EndOfFile == VARIANT_FALSE)
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			int CAtumDBHelper::SaveLevelAdjustmentList(CODBCStatement *i_pODBCStmt, vectorInfinity_DifficultyMonsterListInfo *InvokingItemDPByUseList, HWND hWnd);
///				int CAtumDBHelper::SaveLevelAdjustmentRewardList(CODBCStatement *i_pODBCStmt, vectorInfinity_DifficultyInfo *InvokingItemDPByUseList, HWND hWnd);
/// \brief		인피니티 난이도 조절
/// \author		shcho
/// \date		// 2010-05-24 by shcho
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CLoadExcelFile::LoadExeclLevelAdjustment(void)			// 2010-05-24 by shcho, 인피니티 난이도 조절 - 인피니티 난이도 조정에 관한 툴 업데이트 
{
	while(m_RecordSet->EndOfFile == VARIANT_FALSE)
	{
		INFINITY_DIFFICULTY_MONSTER_SETTING_INFO  tempList;
		util::zero(&tempList, sizeof(INFINITY_DIFFICULTY_MONSTER_SETTING_INFO ));
		
		for(int i=0; i<m_RecordSet->Fields->GetCount(); ++i)
		{
			if(m_RecordSet->Fields->GetItem((long)i)->GetValue().vt != VT_EMPTY
				&&m_RecordSet->Fields->GetItem((long)i)->GetValue().vt != VT_NULL)
			{
				_bstr_t Data;
				Data = m_RecordSet->Fields->GetItem((long)i)->GetValue();
				switch(i) 
				{
					case 0:
						tempList.iIncreaseStep		= atoi((char*)Data);
						break;
					case 1:
						tempList.iMonsterHP			= atoi((char*)Data);
						break;
					case 2:
						tempList.iMonsterDefense		= atoi((char*)Data);
						break;
					case 3:
						tempList.iMonsterEvasion		= atoi((char*)Data);
						break;
					case 4:
						tempList.iMonsterAttackItem	= atoi((char*)Data);
						break;
					case 5:
						tempList.iMonsterPopulation	= atoi((char*)Data);
						break;
				}// end switch
			}// end if
		} // END - for(int i=0; i<m_RecordSet->Fields->GetCount(); ++i)
		
		LevelAdjustmentList.push_back(tempList);
		m_RecordSet->MoveNext();
		
	} // END - while(m_RecordSet->EndOfFile == VARIANT_FALSE)
}
void CLoadExcelFile::LoadExeclLevelAdjustmentReward(void)		// 2010-05-24 by shcho, 인피니티 난이도 조절 - 인피니티 난이도 조정에 관한 툴 업데이트
{
	while(m_RecordSet->EndOfFile == VARIANT_FALSE)
	{
		INFINITY_DIFFICULTY_BONUS_INFO tempList;
		util::zero(&tempList, sizeof(INFINITY_DIFFICULTY_BONUS_INFO));
		
		for(int i=0; i<m_RecordSet->Fields->GetCount(); ++i)
		{
			if(m_RecordSet->Fields->GetItem((long)i)->GetValue().vt != VT_EMPTY
				&&m_RecordSet->Fields->GetItem((long)i)->GetValue().vt != VT_NULL)
			{
				_bstr_t Data;
				Data = m_RecordSet->Fields->GetItem((long)i)->GetValue();
				switch(i) 
				{
					case 0:
						tempList.iIncreaseStep			= atoi((char*)Data);
						break;
					case 1:
						tempList.iExp					= atoi((char*)Data);
						break;
					case 2:
						tempList.iDrop_Item_Persent		= atoi((char*)Data);
						break;
					case 3:
						tempList.iItem_EA_persent		= atoi((char*)Data);
						break;
				}// end switch
			}// end if
		} // END - for(int i=0; i<m_RecordSet->Fields->GetCount(); ++i)
		
		LevelAdjustmentRewardList.push_back(tempList);
		m_RecordSet->MoveNext();
		
	} // END - while(m_RecordSet->EndOfFile == VARIANT_FALSE)
}

// 2010-08-05 by dhjin, 버닝맵 - 
void CLoadExcelFile::LoadExeclBurningMap(void)
{
	while(m_RecordSet->EndOfFile == VARIANT_FALSE)
	{
		BURNING_MAP_INFO tempList;
		util::zero(&tempList, sizeof(BURNING_MAP_INFO));
		
		for(int i=0; i<m_RecordSet->Fields->GetCount(); ++i)
		{
			if(m_RecordSet->Fields->GetItem((long)i)->GetValue().vt != VT_EMPTY
				&&m_RecordSet->Fields->GetItem((long)i)->GetValue().vt != VT_NULL)
			{
				_bstr_t Data;
				Data = m_RecordSet->Fields->GetItem((long)i)->GetValue();
				switch(i) 
				{
				case 0:
					tempList.BurningMapUID		= atoi((char*)Data);
					break;
				case 1:
					tempList.MapIndex			= atoi((char*)Data);
					break;
				case 2:
					tempList.ReqUnitKind		= atoi((char*)Data);
					break;
				case 3:
					tempList.ReqMinLv			= atoi((char*)Data);
					break;
				case 4:
					tempList.ReqMaxLv			= atoi((char*)Data);
					break;
				}// end switch
			}// end if
		} // END - for(int i=0; i<m_RecordSet->Fields->GetCount(); ++i)
		
		BurningMapList.push_back(tempList);
		m_RecordSet->MoveNext();
		
	} // END - while(m_RecordSet->EndOfFile == VARIANT_FALSE)
}

void CLoadExcelFile::LoadExeclPetBaseData(void)		// 2010-06-15 by shcho&hslee 펫시스템 - 팻 기본 정보 엑셀 밀기
{
	while(m_RecordSet->EndOfFile == VARIANT_FALSE)
	{
		// vectorPetBaseData 사용
		// 순서 UniqueNumber,PetIndex,PetKind,DefaultPetName,EnableName,EnableLevel  6개
		tPET_BASEDATA tempBastDataList;
		util::zero(&tempBastDataList, sizeof(tPET_BASEDATA));
		
		for(int i=0; i<m_RecordSet->Fields->GetCount(); ++i)
		{
			if(m_RecordSet->Fields->GetItem((long)i)->GetValue().vt != VT_EMPTY
				&&m_RecordSet->Fields->GetItem((long)i)->GetValue().vt != VT_NULL)
			{
				_bstr_t Data;
				Data = m_RecordSet->Fields->GetItem((long)i)->GetValue();
				switch(i) 
				{

				///////////////////////////////////////////////////////////////////////////////////////
				// start 2011-08-17 by hskim, 파트너 시스템 2차 - 자료 구조 결정
				//
				// 기존 
				//

// 				case 0:
// 					tempBastDataList.UniqueNumber	= atoi((char*)Data);
// 					break;
// 				case 1:
// 					tempBastDataList.PetIndex		= atoi((char*)Data);
// 					break;
// 				case 2:
// 					tempBastDataList.PetKind		= atoi((char*)Data);
// 					break;
// 				case 3:
// 					util::strncpy(tempBastDataList.szPetName, (char*)Data, SIZE_MAX_PET_NAME);
// 					break;
// 				case 4:
// 					tempBastDataList.EnableName		= atoi((char*)Data);
// 					break;
// 				case 5:
// 					tempBastDataList.EnableLevel	= atoi((char*)Data);
// 					break;

				/////////
				// 수정
					
				case 0:
					tempBastDataList.PetIndex		= atoi((char*)Data);
					break;
				case 1:
					util::strncpy(tempBastDataList.PetName, (char*)Data, SIZE_MAX_PET_NAME);
					break;
				case 2:
					tempBastDataList.PetKind		= atoi((char*)Data);
					break;
				case 3:
					tempBastDataList.EnableName		= atoi((char*)Data);
					break;
				case 4:
					tempBastDataList.EnableLevel	= atoi((char*)Data);
					break;
				case 5:
					tempBastDataList.MaxLevel			= atoi((char*)Data);
					break;
				case 6:
					tempBastDataList.BaseSocketCount	= atoi((char*)Data);
					break;

				// end 2011-08-17 by hskim, 파트너 시스템 2차 - 자료 구조 결정
				///////////////////////////////////////////////////////////////////////////////////////
				}// end switch
			}// end if
		} // END - for(int i=0; i<m_RecordSet->Fields->GetCount(); ++i)
		
		PetBaseData.push_back(tempBastDataList);
		m_RecordSet->MoveNext();
		
	} // END - while(m_RecordSet->EndOfFile == VARIANT_FALSE)
}
void CLoadExcelFile::LoadExeclPetLevelData(void)		// 2010-06-15 by shcho&hslee 펫시스템 - 팻 레벨 정보 엑셀 밀기
{
	while(m_RecordSet->EndOfFile == VARIANT_FALSE)
	{
		// vectorPetLevelData 사용
		// 순서 UniqueNumber,PetLevel,PetEXP,Stamina,CitySourceIndex,FieldSourceIndex,
		//		Petskillslot1,Petskillslot2,Petskillslot3,Petskillslot4,Petskillslot5,Petskillslot6    12개
		tPET_LEVEL_DATA tempList;
		util::zero(&tempList, sizeof(tPET_LEVEL_DATA));
		
		for(int i=0; i<m_RecordSet->Fields->GetCount(); ++i)
		{
			if(m_RecordSet->Fields->GetItem((long)i)->GetValue().vt != VT_EMPTY
				&&m_RecordSet->Fields->GetItem((long)i)->GetValue().vt != VT_NULL)
			{
				_bstr_t Data;
				Data = m_RecordSet->Fields->GetItem((long)i)->GetValue();
				switch(i) 
				{

				///////////////////////////////////////////////////////////////////////////////////////
				// start 2011-08-17 by hskim, 파트너 시스템 2차 - 자료 구조 결정
				//
				// 기존 
				//

// 				case 0:
// 					tempList.UniqueNumber			= atoi((char*)Data);
// 					break;
// 				case 1:
// 					tempList.Level					= atoi((char*)Data);
// 					break;
// 				case 2:
// 					tempList.NeedExp				= atof_AfterChangeCommaToDot((char*)Data);
// 					break;
// 				case 3:
// 					tempList.Stamina				= atof_AfterChangeCommaToDot((char*)Data);
// 					break;
// 				case 4:
// 					tempList.CitySourceIndex		= atoi((char*)Data);
// 					break;
// 				case 5:
// 					tempList.FieldSourceIndex		= atoi((char*)Data);
// 					break;
// 				case 6:
// 					tempList.PetAttackIndex			= atoi((char*)Data);
// 					break;
// 				case 7:
// 					tempList.PetSkillItemIndex[0]	= atoi((char*)Data);
// 					break;
// 				case 8:
// 					tempList.PetSkillItemIndex[1]	= atoi((char*)Data);
// 					break;
// 				case 9:
// 					tempList.PetSkillItemIndex[2]	= atoi((char*)Data);
// 					break;
// 				case 10:
// 					tempList.PetSkillItemIndex[3]	= atoi((char*)Data);
// 					break;
// 				case 11:
// 					tempList.PetSkillItemIndex[4]	= atoi((char*)Data);
// 					break;
// 				case 12:
// 					tempList.PetSkillItemIndex[5]	= atoi((char*)Data);
// 					break;

				/////////
				// 수정
				
				case 0:
					tempList.PetIndex			= atoi((char*)Data);
					break;
				case 1:
					tempList.Level					= atoi((char*)Data);
					break;
				case 2:
					tempList.NeedExp				= atof_AfterChangeCommaToDot((char*)Data);
					break;
				case 3:
					tempList.UseWeaponIndex		= atoi((char*)Data);
					break;
				case 4:
					tempList.SlotCountSkill		= atoi((char*)Data);
					break;
				case 5:
					tempList.SlotCountSocket	= atoi((char*)Data);
					break;
				case 6:
					tempList.KitLevelHP			= atoi((char*)Data);
					break;
				case 7:
					tempList.KitLevelShield		= atoi((char*)Data);
					break;
				case 8:
					tempList.KitLevelSP			= atoi((char*)Data);
					break;

				// end 2011-08-17 by hskim, 파트너 시스템 2차 - 자료 구조 결정
				///////////////////////////////////////////////////////////////////////////////////////
				}// end switch
			}// end if
		} // END - for(int i=0; i<m_RecordSet->Fields->GetCount(); ++i)
		
		PetLevelData.push_back(tempList);
		m_RecordSet->MoveNext();
		
	} // END - while(m_RecordSet->EndOfFile == VARIANT_FALSE)
}

void CLoadExcelFile::ExeclLoadOperatorData(void)		// 2010-06-15 by shcho&hslee 펫시스템 - DB 밀기 추가 작업 펫 오퍼레이터 정보.
{
	while(m_RecordSet->EndOfFile == VARIANT_FALSE)
	{
		OperatorInfo tempList;
		util::zero(&tempList, sizeof(OperatorInfo));
		
		for(int i=0; i<m_RecordSet->Fields->GetCount(); ++i)
		{
			if(m_RecordSet->Fields->GetItem((long)i)->GetValue().vt != VT_EMPTY
				&&m_RecordSet->Fields->GetItem((long)i)->GetValue().vt != VT_NULL)
			{
				_bstr_t Data;
				Data = m_RecordSet->Fields->GetItem((long)i)->GetValue();
				switch(i) 
				{
				case 0:
					tempList.ActionType		= atoi((char*)Data);
					break;
				case 1:
					tempList.Itemnum		= atoi((char*)Data);
					break;
				case 2:
					tempList.FunctionIndex	= atoi((char*)Data);
					break;
				case 3:
					tempList.FunctionValue	= atof_AfterChangeCommaToDot((char*)Data);
					break;
				case 4:
					tempList.SourceIndex	= atoi((char*)Data);
					break;
				case 5:
					util::strncpy(tempList.ActionDesc, (char*)Data, 128);
					break;
				}// end switch
			}// end if
		} // END - for(int i=0; i<m_RecordSet->Fields->GetCount(); ++i)
		
		OperatorDataList.push_back(tempList);
		m_RecordSet->MoveNext();
		
	} // END - while(m_RecordSet->EndOfFile == VARIANT_FALSE)
}

// 2010-08-31 by shcho&jskim 아이템용해 시스템 - 용해 아이템 정보.
void CLoadExcelFile::LoadExeclDissolutionItemData(void)
{
	while(m_RecordSet->EndOfFile == VARIANT_FALSE)
	{
		tDissolutionItemInfo tempList;
		util::zero(&tempList, sizeof(tDissolutionItemInfo));
		
		for(int i=0; i<m_RecordSet->Fields->GetCount(); ++i)
		{
			if(m_RecordSet->Fields->GetItem((long)i)->GetValue().vt != VT_EMPTY
				&&m_RecordSet->Fields->GetItem((long)i)->GetValue().vt != VT_NULL)
			{
				_bstr_t Data;
				Data = m_RecordSet->Fields->GetItem((long)i)->GetValue();

				switch(i) 
				{
				case 0:
					tempList.SourceItemnum				= atoi((char*)Data);
					break;
				case 1:
					tempList.ResultItemNum[0]			= atoi((char*)Data);
					break;
				case 2:
					tempList.ResultItemMinCount[0]		= atoi((char*)Data);
					break;
				case 3:
					tempList.ResultItemMaxCount[0]		= atoi((char*)Data);
					break;
				case 4:
					tempList.ResultItemProbabillity[0]	= atoi((char*)Data);
					break;
				case 5:
					tempList.ResultItemNum[1]			= atoi((char*)Data);
					break;
				case 6:
					tempList.ResultItemMinCount[1]		= atoi((char*)Data);
					break;
				case 7:
					tempList.ResultItemMaxCount[1]		= atoi((char*)Data);
					break;
				case 8:
					tempList.ResultItemProbabillity[1]	= atoi((char*)Data);
					break;
				case 9:
					tempList.ResultItemNum[2]			= atoi((char*)Data);
					break;
				case 10:
					tempList.ResultItemMinCount[2]		= atoi((char*)Data);
					break;
				case 11:
					tempList.ResultItemMaxCount[2]		= atoi((char*)Data);
					break;
				case 12:
					tempList.ResultItemProbabillity[2]	= atoi((char*)Data);
					break;
				}// end switch
			}// end if
		} // END - for(int i=0; i<m_RecordSet->Fields->GetCount(); ++i)
		
		DissolutionIteminfo.push_back(tempList);
		m_RecordSet->MoveNext();
		
	} // END - while(m_RecordSet->EndOfFile == VARIANT_FALSE)
}
// END 2010-08-31 by shcho&jskim 아이템용해 시스템 - 용해 아이템 정보.

// start 2011-03-17 by hskim, 인피니티 3차 - 몬스터 멀티 타겟팅 기능 추가
void CLoadExcelFile::LoadExeclMonsterMultiTargetData(void)
{
	while(m_RecordSet->EndOfFile == VARIANT_FALSE)
	{
		MONSTER_MULTI_TARGET tempData;
		util::zero(&tempData, sizeof(MONSTER_MULTI_TARGET));
		
		for(int i=0; i<m_RecordSet->Fields->GetCount(); ++i)
		{
			if(m_RecordSet->Fields->GetItem((long)i)->GetValue().vt != VT_EMPTY
				&&m_RecordSet->Fields->GetItem((long)i)->GetValue().vt != VT_NULL)
			{
				_bstr_t Data;
				Data = m_RecordSet->Fields->GetItem((long)i)->GetValue();
				
				switch(i) 
				{
				case 0:
					tempData.MonsterIndex				= atoi((char*)Data);
					break;
				case 1:
					tempData.PointIndex					= atoi((char*)Data);
					break;
				case 2:
					tempData.TargetPosition.x			= atof_AfterChangeCommaToDot((char*)Data);
					break;
				case 3:
					tempData.TargetPosition.y			= atof_AfterChangeCommaToDot((char*)Data);
					break;
				case 4:
					tempData.TargetPosition.z			= atof_AfterChangeCommaToDot((char*)Data);
					break;
				}// end switch
			}// end if
		} // END - for(int i=0; i<m_RecordSet->Fields->GetCount(); ++i)
		
		MonsterMultiTargetList.push_back(tempData);
		m_RecordSet->MoveNext();
		
	} // END - while(m_RecordSet->EndOfFile == VARIANT_FALSE)
}
// end 2011-03-17 by hskim, 인피니티 3차 - 몬스터 멀티 타겟팅 기능 추가

// start 2011-05-23 by hskim, 인피니티 3차 - 웨이포인트 구현
void CLoadExcelFile::LoadExeclWayPointData(void)
{ 
	while(m_RecordSet->EndOfFile == VARIANT_FALSE)
	{
		SWAY_POINT tempData;
		util::zero(&tempData, sizeof(SWAY_POINT));
		
		for(int i=0; i<m_RecordSet->Fields->GetCount(); ++i)
		{
			if(m_RecordSet->Fields->GetItem((long)i)->GetValue().vt != VT_EMPTY
				&&m_RecordSet->Fields->GetItem((long)i)->GetValue().vt != VT_NULL)
			{
				_bstr_t Data;
				Data = m_RecordSet->Fields->GetItem((long)i)->GetValue();
				
				switch(i) 
				{
				case 0:
					tempData.dwPatternIndex				= atoi((char*)Data);
					break;
				case 1:
					tempData.dwSequenceNum				= atoi((char*)Data);
					break;
				case 2:
					tempData.byPostionAttribute			= atoi((char*)Data);
					break;
				case 3:
					tempData.vPoint.x					= atof_AfterChangeCommaToDot((char*)Data);
					break;
				case 4:
					tempData.vPoint.y					= atof_AfterChangeCommaToDot((char*)Data);
					break;
				case 5:
					tempData.vPoint.z					= atof_AfterChangeCommaToDot((char*)Data);
					break;
				}// end switch
			}// end if
		} // END - for(int i=0; i<m_RecordSet->Fields->GetCount(); ++i)
		
		WayPointList.push_back(tempData);
		m_RecordSet->MoveNext();
		
	} // END - while(m_RecordSet->EndOfFile == VARIANT_FALSE)
}

void CLoadExcelFile::LoadExeclWayPointMoveData(void)
{ 
	while(m_RecordSet->EndOfFile == VARIANT_FALSE)
	{
		SWAY_POINT_MOVE tempData;
		util::zero(&tempData, sizeof(SWAY_POINT_MOVE));
		
		for(int i=0; i<m_RecordSet->Fields->GetCount(); ++i)
		{
			if(m_RecordSet->Fields->GetItem((long)i)->GetValue().vt != VT_EMPTY
				&&m_RecordSet->Fields->GetItem((long)i)->GetValue().vt != VT_NULL)
			{
				_bstr_t Data;
				Data = m_RecordSet->Fields->GetItem((long)i)->GetValue();
				
				switch(i) 
				{
				case 0:
					tempData.dwIndex				= atoi((char*)Data);
					break;
				case 1:
					tempData.byCompletionAction		= atoi((char*)Data);
					break;
				}// end switch
			}// end if
		} // END - for(int i=0; i<m_RecordSet->Fields->GetCount(); ++i)

		WayPointMoveList.push_back(tempData);
		m_RecordSet->MoveNext();
		
	} // END - while(m_RecordSet->EndOfFile == VARIANT_FALSE)
}
// end 2011-05-23 by hskim, 인피니티 3차 - 웨이포인트 구현

// start 2011-10-10 by hskim, EP4 [트리거 시스템] - 화산재 / 모래 폭풍
void CLoadExcelFile::LoadExeclTriggerMapBuffData(void)
{ 
	while(m_RecordSet->EndOfFile == VARIANT_FALSE)
	{
		STRIGGER_MAP_BUFF tempData;
		util::zero(&tempData, sizeof(STRIGGER_MAP_BUFF));

		for(int i=0; i<m_RecordSet->Fields->GetCount(); ++i)
		{
			if(m_RecordSet->Fields->GetItem((long)i)->GetValue().vt != VT_EMPTY
				&&m_RecordSet->Fields->GetItem((long)i)->GetValue().vt != VT_NULL)
			{
				_bstr_t Data;
				Data = m_RecordSet->Fields->GetItem((long)i)->GetValue();
				
				switch(i) 
				{
				case 0:
					tempData.MapIndex			= atoi((char*)Data);
					break;
				case 1:
					tempData.ItemNum			= atoi((char*)Data);
					break;
				}// end switch
			}// end if
		} // END - for(int i=0; i<m_RecordSet->Fields->GetCount(); ++i)
		
		TriggerMapBuffList.push_back(tempData);
		m_RecordSet->MoveNext();
		
	} // END - while(m_RecordSet->EndOfFile == VARIANT_FALSE)
}
// end 2011-10-10 by hskim, EP4 [트리거 시스템] - 화산재 / 모래 폭풍

// start 2011-10-28 by hskim, EP4 [트리거 시스템] - 크리스탈 시스템
void CLoadExcelFile::LoadExeclMapTrigger(void)
{ 
	while(m_RecordSet->EndOfFile == VARIANT_FALSE)
	{
		STRIGGER_MAP tempData;
		util::zero(&tempData, sizeof(STRIGGER_MAP));

		for(int i=0; i<m_RecordSet->Fields->GetCount(); ++i)
		{
			if(m_RecordSet->Fields->GetItem((long)i)->GetValue().vt != VT_EMPTY
				&&m_RecordSet->Fields->GetItem((long)i)->GetValue().vt != VT_NULL)
			{
				_bstr_t Data;
				Data = m_RecordSet->Fields->GetItem((long)i)->GetValue();
				
				switch(i) 
				{
				case 0:
					tempData.MapTriggerID		= atoi((char*)Data);
					break;
				case 1:
					tempData.MapIndex			= atoi((char*)Data);
					break;
				case 2:
					tempData.MapChannel			= atoi((char*)Data);
					break;
				case 3:
					tempData.FunctionID			= atoi((char*)Data);
					break;
				}// end switch
			}// end if
		} // END - for(int i=0; i<m_RecordSet->Fields->GetCount(); ++i)
		
		m_vectTriggerMapList.push_back(tempData);
		m_RecordSet->MoveNext();
		
	} // END - while(m_RecordSet->EndOfFile == VARIANT_FALSE)
}

void CLoadExcelFile::LoadExeclTriggerFunctionCrystal(void)
{ 
	while(m_RecordSet->EndOfFile == VARIANT_FALSE)
	{
		STRIGGER_FUNCTION_CRYSTAL tempData;
		util::zero(&tempData, sizeof(STRIGGER_FUNCTION_CRYSTAL));

		for(int i=0; i<m_RecordSet->Fields->GetCount(); ++i)
		{
			if(m_RecordSet->Fields->GetItem((long)i)->GetValue().vt != VT_EMPTY
				&&m_RecordSet->Fields->GetItem((long)i)->GetValue().vt != VT_NULL)
			{
				_bstr_t Data;
				Data = m_RecordSet->Fields->GetItem((long)i)->GetValue();
				
				switch(i) 
				{
				case 0:
					tempData.FunctionID			= atoi((char*)Data);
					break;
				case 1:
					tempData.CrystalGroupID		= atoi((char*)Data);
					break;
				case 2:
					tempData.PeriodTime			= atoi((char*)Data);
					break;
				case 3:
					tempData.RandomSequence		= atoi((char*)Data);
					break;
				}// end switch
			}// end if
		} // END - for(int i=0; i<m_RecordSet->Fields->GetCount(); ++i)
		
		m_vectTriggerFunctionCrystalList.push_back(tempData);
		m_RecordSet->MoveNext();
		
	} // END - while(m_RecordSet->EndOfFile == VARIANT_FALSE)
}

void CLoadExcelFile::LoadExeclTriggerCrystalGroup(void)
{ 
	while(m_RecordSet->EndOfFile == VARIANT_FALSE)
	{
		STRIGGER_CRYSTAL_GROUP tempData;
		util::zero(&tempData, sizeof(STRIGGER_CRYSTAL_GROUP));

		for(int i=0; i<m_RecordSet->Fields->GetCount(); ++i)
		{
			if(m_RecordSet->Fields->GetItem((long)i)->GetValue().vt != VT_EMPTY
				&&m_RecordSet->Fields->GetItem((long)i)->GetValue().vt != VT_NULL)
			{
				_bstr_t Data;
				Data = m_RecordSet->Fields->GetItem((long)i)->GetValue();
				
				switch(i) 
				{
				case 0:
					tempData.CrystalGroupID		= atoi((char*)Data);
					break;
				case 1:
					tempData.DestroyGroupID		= atoi((char*)Data);
					break;
				case 2:
					tempData.EventID			= atoi((char*)Data);
					break;
				}// end switch
			}// end if
		} // END - for(int i=0; i<m_RecordSet->Fields->GetCount(); ++i)
		
		m_vectTriggerCrystalGroupList.push_back(tempData);
		m_RecordSet->MoveNext();
		
	} // END - while(m_RecordSet->EndOfFile == VARIANT_FALSE)
}

void CLoadExcelFile::LoadExeclTriggerCrystalDestroyGroup(void)
{ 
	while(m_RecordSet->EndOfFile == VARIANT_FALSE)
	{
		STRIGGER_CRYSTAL_DESTROY_GROUP tempData;
		util::zero(&tempData, sizeof(STRIGGER_CRYSTAL_DESTROY_GROUP));

		for(int i=0; i<m_RecordSet->Fields->GetCount(); ++i)
		{
			if(m_RecordSet->Fields->GetItem((long)i)->GetValue().vt != VT_EMPTY
				&&m_RecordSet->Fields->GetItem((long)i)->GetValue().vt != VT_NULL)
			{
				_bstr_t Data;
				Data = m_RecordSet->Fields->GetItem((long)i)->GetValue();
				
				switch(i) 
				{
				case 0:
					tempData.DestroyGroupID		= atoi((char*)Data);
					break;
				case 1:
					tempData.SequenceNumber		= atoi((char*)Data);
					break;
				case 2:
					tempData.TargetMonster		= atoi((char*)Data);
					break;
				}// end switch
			}// end if
		} // END - for(int i=0; i<m_RecordSet->Fields->GetCount(); ++i)
		
		m_vectTriggerCrystalDestroyGroupList.push_back(tempData);
		m_RecordSet->MoveNext();
		
	} // END - while(m_RecordSet->EndOfFile == VARIANT_FALSE)
}
// end 2011-10-28 by hskim, EP4 [트리거 시스템] - 크리스탈 시스템

// start 2011-11-07 by hskim, EP4 [트리거 시스템] - NCG 거점전 시스템
void CLoadExcelFile::LoadExeclTriggerFunctionNGCInflWar(void)
{ 
	while(m_RecordSet->EndOfFile == VARIANT_FALSE)
	{
		STRIGGER_FUNCTION_NGC_INFLWAR tempData;
		util::zero(&tempData, sizeof(STRIGGER_FUNCTION_NGC_INFLWAR));

		for(int i=0; i<m_RecordSet->Fields->GetCount(); ++i)
		{
			if(m_RecordSet->Fields->GetItem((long)i)->GetValue().vt != VT_EMPTY
				&&m_RecordSet->Fields->GetItem((long)i)->GetValue().vt != VT_NULL)
			{
				_bstr_t Data;
				Data = m_RecordSet->Fields->GetItem((long)i)->GetValue();

				switch(i) 
				{
				case 0:
					tempData.FunctionID			= atoi((char*)Data);
					break;
				case 1:
					tempData.NGCInflWarGroupID	= atoi((char*)Data);
					break;
				case 2:
					tempData.PeriodTime			= atoi((char*)Data);
					break;
				case 3:
					tempData.BossMonster		= atoi((char*)Data);
					break;
				case 4:
					tempData.WinInfluenceWP		= atoi((char*)Data);
					break;
				case 5:
					tempData.LossInfluenceWP	= atoi((char*)Data);
					break;
				case 6:
					tempData.FirstDamageGuildWP	= atoi((char*)Data);
					break;
				case 7:
					tempData.FixedTimeType	= atoi((char*)Data);
					break;
				}// end switch
			}// end if
		} // END - for(int i=0; i<m_RecordSet->Fields->GetCount(); ++i)

		m_vectTriggerFunctionNGCInflWarList.push_back(tempData);
		m_RecordSet->MoveNext();

	} // END - while(m_RecordSet->EndOfFile == VARIANT_FALSE)
}

void CLoadExcelFile::LoadExeclTriggerNGCInflWarMonsterGroup(void)
{ 
	while(m_RecordSet->EndOfFile == VARIANT_FALSE)
	{
		STRIGGER_NGC_INFLWAR_MONSTER_GROUP tempData;
		util::zero(&tempData, sizeof(STRIGGER_NGC_INFLWAR_MONSTER_GROUP));

		for(int i=0; i<m_RecordSet->Fields->GetCount(); ++i)
		{
			if(m_RecordSet->Fields->GetItem((long)i)->GetValue().vt != VT_EMPTY
				&&m_RecordSet->Fields->GetItem((long)i)->GetValue().vt != VT_NULL)
			{
				_bstr_t Data;
				Data = m_RecordSet->Fields->GetItem((long)i)->GetValue();

				switch(i) 
				{
				case 0:
					tempData.NGCInflWarGroupID	= atoi((char*)Data);
					break;
				case 1:
					tempData.Monster			= atoi((char*)Data);
					break;
				case 2:
					tempData.SummonCount		= atoi((char*)Data);
					break;
				case 3:
					tempData.SummonRandomPos	= atoi((char*)Data);
					break;
				}// end switch
			}// end if
		} // END - for(int i=0; i<m_RecordSet->Fields->GetCount(); ++i)

		m_vectTriggerNGCInflWarMonsterGroupList.push_back(tempData);
		m_RecordSet->MoveNext();

	} // END - while(m_RecordSet->EndOfFile == VARIANT_FALSE)
}
// end 2011-11-07 by hskim, EP4 [트리거 시스템] - NCG 거점전 시스템

// 2013-01-21 by jhseol, NGC 전전기지 트리거 시스템
void CLoadExcelFile::LoadExeclTriggerFunctionNGCOutPostList(void)
{
	while(m_RecordSet->EndOfFile == VARIANT_FALSE)
	{
		STRIGGER_FUNCTION_NGC_OUTPOST tempData;
		util::zero(&tempData, sizeof(STRIGGER_FUNCTION_NGC_OUTPOST));
		
		for(int i=0; i<m_RecordSet->Fields->GetCount(); ++i)
		{
			if(m_RecordSet->Fields->GetItem((long)i)->GetValue().vt != VT_EMPTY
				&&m_RecordSet->Fields->GetItem((long)i)->GetValue().vt != VT_NULL)
			{
				_bstr_t Data;
				Data = m_RecordSet->Fields->GetItem((long)i)->GetValue();
				
				switch(i) 
				{
				case 0:
					tempData.FunctionID				= atoi((char*)Data);
					break;
				case 1:
					tempData.SummonMonsterGroupID	= atoi((char*)Data);
					break;
				case 2:
					tempData.StandardOutPostMap		= atoi((char*)Data);
					break;
				case 3:
					tempData.BossMonster			= atoi((char*)Data);
					break;
				case 4:
					tempData.WinInfluenceWP			= atoi((char*)Data);
					break;
				case 5:
					tempData.LossInfluenceWP		= atoi((char*)Data);
					break;
				case 6:
					tempData.NextOutPostMap			= atoi((char*)Data);
					break;
				case 7:
					tempData.DestroyCrystalcount	= atoi((char*)Data);
					break;
				}// end switch
			}// end if
		} // END - for(int i=0; i<m_RecordSet->Fields->GetCount(); ++i)
		
		m_vectTriggerFunctionNGCOutPostList.push_back(tempData);
		m_RecordSet->MoveNext();
		
	} // END - while(m_RecordSet->EndOfFile == VARIANT_FALSE)
}

void CLoadExcelFile::LoadExeclTriggerOutPostBossKillInfoList(void)
{ 
	while(m_RecordSet->EndOfFile == VARIANT_FALSE)
	{
		STRIGGER_TRIGGER_OUTPOST_BOSS_KILL_INFO tempData;
		util::zero(&tempData, sizeof(STRIGGER_TRIGGER_OUTPOST_BOSS_KILL_INFO));
		
		for(int i=0; i<m_RecordSet->Fields->GetCount(); ++i)
		{
			if(m_RecordSet->Fields->GetItem((long)i)->GetValue().vt != VT_EMPTY
				&&m_RecordSet->Fields->GetItem((long)i)->GetValue().vt != VT_NULL)
			{
				_bstr_t Data;
				Data = m_RecordSet->Fields->GetItem((long)i)->GetValue();
				
				switch(i) 
				{
				case 0:
					tempData.BossMonster			= atoi((char*)Data);
					break;
				case 1:
					tempData.CrystalNum				= atoi((char*)Data);
					break;
				case 2:
					tempData.RegenMinTimeMinutes	= atoi((char*)Data);
					break;
				case 3:
					tempData.RegenMaxTimeMinutes	= atoi((char*)Data);
					break;
				}// end switch
			}// end if
		} // END - for(int i=0; i<m_RecordSet->Fields->GetCount(); ++i)
		
		m_vectTriggerOutPostBossKillInfoList.push_back(tempData);
		m_RecordSet->MoveNext();
		
	} // END - while(m_RecordSet->EndOfFile == VARIANT_FALSE)
}

void CLoadExcelFile::LoadExeclTriggerCrystalBuffList(void)
{ 
	while(m_RecordSet->EndOfFile == VARIANT_FALSE)
	{
		STRIGGER_CRYSTAL_BUFF tempData;
		util::zero(&tempData, sizeof(STRIGGER_CRYSTAL_BUFF));
		
		for(int i=0; i<m_RecordSet->Fields->GetCount(); ++i)
		{
			if(m_RecordSet->Fields->GetItem((long)i)->GetValue().vt != VT_EMPTY
				&&m_RecordSet->Fields->GetItem((long)i)->GetValue().vt != VT_NULL)
			{
				_bstr_t Data;
				Data = m_RecordSet->Fields->GetItem((long)i)->GetValue();
				
				switch(i) 
				{
				case 0:
					tempData.FunctionID			= atoi((char*)Data);
					break;
				case 1:
					tempData.SkillItemNum		= atoi((char*)Data);
					break;
				}// end switch
			}// end if
		} // END - for(int i=0; i<m_RecordSet->Fields->GetCount(); ++i)
		
		m_vectTriggerCrystalBuff.push_back(tempData);
		m_RecordSet->MoveNext();
		
	} // END - while(m_RecordSet->EndOfFile == VARIANT_FALSE)
}
// end 2013-01-21 by jhseol, NGC 전전기지 트리거 시스템