//Copyright[2002] MasangSoft
#ifndef COMMON_ATUMDBHELPER_H_
#define COMMON_ATUMDBHELPER_H_
#include "AtumParam.h"
#include "AtumProtocol.h"
#include "ODBCStatement.h"

class CWayPointManager;        // 2011-05-23 by hskim, 인피니티 3차 - 웨이포인트 구현

struct CHARACTER_DB_EX
{
    SQL_TIMESTAMP_STRUCT    CreatedTime;
    SQL_TIMESTAMP_STRUCT    LastStartedTime;
    SQL_TIMESTAMP_STRUCT    LevelUpTime;            // 2006-12-18 by dhjin, 레벨업 시간

    CHARACTER_DB_EX()
    {
        memset(this, 0x00, sizeof(CHARACTER_DB_EX));
    }
};

class CAtumDBHelper  
{
public:
    // Info Loading
    static int LoadItemInfo(CODBCStatement *i_pODBCStmt, ez_map<INT, ITEM> *o_pMapItemInfo, ez_map<string, ITEM> *o_pMapItemInfoName = NULL);
    static int LoadEnchantInfo(CODBCStatement *i_pODBCStmt, ez_map<INT, ENCHANT_INFO> *o_pMapEnchantInfo);
    static int LoadItemMixingInfo(CODBCStatement *i_pODBCStmt, vector<ITEM_MIXING_INFO> *o_pVectorItemMixingInfo);
    static int LoadRareItemInfo(CODBCStatement *i_pODBCStmt, ez_map<INT, RARE_ITEM_INFO> *o_pMapRareItemInfo,
                                ez_map<string, RARE_ITEM_INFO> *o_pMapRareItemInfoName = NULL,
                                ez_map<BYTE, vector<RARE_ITEM_INFO* > > *o_pMapLevel2RareItemInfo = NULL);
    static int LoadMapObjectInfo(CODBCStatement *i_pODBCStmt, ez_map<int, MAPOBJECTINFO> *o_mapMapObjectInfo);
    static int LoadMapInfo(CODBCStatement *i_pODBCStmt, vectorMAP_INFO *o_mapMapInfo);
    static int LoadMysteryItemDropInfo(mtmapINT2MYSTERY_ITEM_DROP *o_pmmapMysteryItemDropList, CODBCStatement *i_pODBCStmt, ez_map<INT, ITEM*> *i_pemapItemInfoList);        // 2007-05-11 by cmkwon
    static int LoadMysteryItemDropInfoByOmi(mmapINT2MYSTERY_ITEM_DROP *o_pmmapMysteryItemDropList, CODBCStatement *i_pODBCStmt, ez_map<INT, ITEM> *i_pemapItemInfoList);        // 2008-11-04 by dhjin, 럭키머신, omi용
    static int LoadLuckyMachineOmi(CODBCStatement *i_pODBCStmt, vector<LUCKY_MACHINE_OMI> *o_pvectLuckyMachine);            // 2009-03-03 by dhjin, 럭키머신 수정안
    static int LoadInvokingWearItemDestParamNum(CODBCStatement *i_pODBCStmt, InvokingWearItemDestParamList *o_pInvokingWearItemDestParamNumList);            // 2009-09-09 ~ 2010-02-10 by dhjin, 인피니티 - 발동류장착아이템
    static int LoadInvokingWearItemDestParamNumByUse(CODBCStatement *i_pODBCStmt, InvokingWearItemDestParamList *o_pInvokingWearItemDestParamNumByUseList);            // 2009-09-09 ~ 2010-02-10 by dhjin, 인피니티 - 발동류장착아이템
    static int LoadBurningMap(CODBCStatement *i_pODBCStmt, BurningMapInfoList *o_pBurningMapInfo);            // 2010-08-05 by dhjin, 버닝맵 - 

    ///////////////////////////////////////////////////////////////////////////////
    // 2010-06-15 by shcho&hslee 펫시스템 -
    static int DBLoadPetBaseDataInfo(CODBCStatement *i_pODBCStmt, vectorPetDataInfo* o_PetDataInfo );
    static int DBLoadPetLevelDataInfo(CODBCStatement *i_pODBCStmt, vectorPetDataInfo* o_PetDataInfo );

    // 2010-06-15 by shcho&hslee 펫시스템 - omi 추가 작업. - 펫 오퍼레이터 정보.
    static int SaveOperatorData(CODBCStatement *i_pODBCStmt, vectorOperatorInfo* i_OperatorData, HWND hwnd );
    static int DBLoadOperatorDataInfo(CODBCStatement *i_pODBCStmt, vectorOperatorInfo* o_OperatorInfo);
    // END 2010-06-15 by shcho&hslee 펫시스템 - omi 추가 작업. - 펫 오퍼레이터 정보.

    // 2010-08-31 by shcho&jskim 아이템용해 시스템 - 용해 아이템 정보.        
    static int SaveDissolutionItemData(CODBCStatement *i_pODBCStmt, vectorDissolutionItemInfo* i_DissolutionItemInfo,HWND hWnd);
    static int DBLoadDissolutionItemDataInfo(CODBCStatement *i_pODBCStmt, void* o_DissolutionItemInfo, BOOL ToolFlag = TRUE);
    // END 2010-08-31 by shcho&jskim 아이템용해 시스템 - 용해 아이템 정보.        
    // 2011-03-17 by hskim, 인피니티 3차 - 몬스터 멀티 타겟팅 기능 추가
    static int SaveMonsterMultiTargetData(CODBCStatement *i_pODBCStmt, vectorMonsterMultiTarget* i_MonsterMultiTargetInfo,HWND hWnd);
    static int DBLoadMonsterMultiTargetData(CODBCStatement *i_pODBCStmt, void* o_MonsterMultiTargetInfo, BOOL ToolFlag = TRUE);
    // end 2011-03-17 by hskim, 인피니티 3차 - 몬스터 멀티 타겟팅 기능 추가

    // start 2011-05-23 by hskim, 인피니티 3차 - 웨이포인트 구현
    static int SaveWayPointData(CODBCStatement *i_pODBCStmt, vectorWayPoint *i_pWayPoint,HWND hWnd);
    static int DBLoadWayPointDataInfo(CODBCStatement *i_pODBCStmt, void *o_pWayPointManager, BOOL ToolFlag = TRUE);

    static int SaveWayPointMoveData(CODBCStatement *i_pODBCStmt, vectorWayPointMove *i_pWayPointMove,HWND hWnd);
    static int DBLoadWayPointMoveDataInfo(CODBCStatement *i_pODBCStmt, void* o_pWayPointManager, BOOL ToolFlag = TRUE);
    // end 2011-05-23 by hskim, 인피니티 3차 - 웨이포인트 구현

    // start 2011-10-10 by hskim, EP4 [트리거 시스템] - 화산재 / 모래 폭풍
    static int SaveTriggerMapBuff(CODBCStatement *i_pODBCStmt, vectorTriggerMapBuff *i_pTriggerMapBuff, HWND hWnd);
    static int DBLoadTriggerMapBuffInfo(CODBCStatement *i_pODBCStmt, void *i_pTriggerMapBuff, BOOL ToolFlag = TRUE);
    // end 2011-10-10 by hskim, EP4 [트리거 시스템] - 화산재 / 모래 폭풍

    // start 2011-10-28 by hskim, EP4 [트리거 시스템] - 크리스탈 시스템
    static int SaveTriggerMap(CODBCStatement *i_pODBCStmt, vectorTriggerMap *i_pTriggerMap, HWND hWnd);
    static int DBLoadTriggerMapInfo(CODBCStatement *i_pODBCStmt, void *i_pTriggerMap, BOOL ToolFlag = TRUE);
    static int SaveTriggerFunctionCrystal(CODBCStatement *i_pODBCStmt, vectorTriggerFunctionCrystal *i_pTriggerFunctionCrystal, HWND hWnd);
    static int DBLoadTriggerFunctionCrystalInfo(CODBCStatement *i_pODBCStmt, void *i_pTriggerFunctionCrystal, BOOL ToolFlag = TRUE);
    static int SaveTriggerCrystalGroup(CODBCStatement *i_pODBCStmt, vectorTriggerCrystalGroup *i_pTriggerCrystalGroup, HWND hWnd);
    static int DBLoadTriggerCrystalGroupInfo(CODBCStatement *i_pODBCStmt, void *i_pTriggerCrystalGroup, BOOL ToolFlag = TRUE);
    static int SaveTriggerCrystalDestroyGroup(CODBCStatement *i_pODBCStmt, vectorTriggerCrystalDestroyGroup *i_pTriggerCrystalDestroyGroup, HWND hWnd);
    static int DBLoadTriggerCrystalDestroyGroupInfo(CODBCStatement *i_pODBCStmt, void *i_pTriggerCrystalDestroyGroup, BOOL ToolFlag = TRUE);
    // end 2011-10-28 by hskim, EP4 [트리거 시스템] - 크리스탈 시스템

    // start 2011-11-07 by hskim, EP4 [트리거 시스템] - NCG 거점전 시스템
    static int SaveTriggerFunctionNGCInflWar(CODBCStatement *i_pODBCStmt, vectorTriggerFunctionNGCInflWar *i_pTriggerFunctionNGCInflWar, HWND hWnd);
    static int DBLoadTriggerFunctionNGCInflWarInfo(CODBCStatement *i_pODBCStmt, void *i_pTriggerFunctionNGCInflWar, BOOL ToolFlag = TRUE);
    static int SaveTriggerNGCInflWarMonsterGroup(CODBCStatement *i_pODBCStmt, vectorTriggerNGCInflWarMonsterGroup *i_pTriggerNGCInflWarMonsterGroup, HWND hWnd);
    static int DBLoadTriggerNGCInflWarMonsterGroupInfo(CODBCStatement *i_pODBCStmt, void *i_pTriggerNGCInflWarMonsterGroup, BOOL ToolFlag = TRUE);
    // end 2011-11-07 by hskim, EP4 [트리거 시스템] - NCG 거점전 시스템

    // 2013-01-21 by jhseol, NGC 전전기지 트리거 시스템
    static int SaveTriggerFunctionNGCOutPost(CODBCStatement *i_pODBCStmt, vectorTriggerFunctionNGCOutPost *i_pTriggerFunctionNGCOutPost, HWND hWnd);
    static int SaveTriggerOutPostBossKill(CODBCStatement *i_pODBCStmt, vectorTriggerOutPostBossKillInfo *i_pTriggerOutPostBossKillInfo, HWND hWnd);
    static int DBLoadTriggerFunctionNGCOutPostInfo(CODBCStatement *i_pODBCStmt, void *i_pTriggerFunctionNGCOutPost, BOOL ToolFlag = TRUE);
    static int DBLoadTriggerOutPostBossKillInfo(CODBCStatement *i_pODBCStmt, void *i_pTriggerOutPostBossKillInfo, BOOL ToolFlag = TRUE);
    static INT SaveTriggerCrystalBuff(CODBCStatement *i_pODBCStmt, vectorTriggerCrystalBuff *i_pTriggerCrystalBuff, HWND hWnd);
    static int DBLoadTriggerCrystalBuffInfo(CODBCStatement *i_pODBCStmt, void *i_pTriggerCrystalBuffInfo, BOOL ToolFlag = TRUE);
    // end 2013-01-21 by jhseol, NGC 전전기지 트리거 시스템

    // 2011-12-21 by hskim, EP4 [동영상 1회 재생]
    static int DBLoadSystemEventInfo(CODBCStatement *i_pODBCStmt , ATUM_DATE_TIME *i_pEventDateTime, BOOL ToolFlag = TRUE);
    // 2011-12-21 by hskim, EP4 [동영상 1회 재생]

    //static int LoadPCBangInfo(CODBCStatement *i_pODBCStmt, vectorPCBANG_INFO *o_pPCbangInfo, char *o_ErrorString);
    //static int InsertPCBangInfo(CODBCStatement *i_pODBCStmt, vectorPCBANG_INFO *o_pPCbangInfo);

    // 2006-04-20 by cmkwon, 세력리더 CharacterUID 가져오기
    static BOOL LoadInfluenceLeader(UID32_t *o_pVCNLeader, UID32_t *o_pANILeader, CODBCStatement *i_pOdbcStmt);
    // 2006-12-08 by dhjin, 세력부지도자
    static BOOL LoadInfluenceSub1Leader(UID32_t *o_pVCNSub1Leader, UID32_t *o_pANISub1Leader, CODBCStatement *i_pOdbcStmt);
    static BOOL LoadInfluenceSub2Leader(UID32_t *o_pVCNSub2Leader, UID32_t *o_pANISub2Leader, CODBCStatement *i_pOdbcStmt);

    // 2006-02-07 by cmkwon, Influence Type Distribution Loading
    static int LoadEachInflueceTypeCount(int *o_pNormalInfl, int *o_pVCNInfl, int *o_pANIInfl, CODBCStatement *i_pODBCStmt);
    static int LoadInfluenceWarInfo(CODBCStatement *i_pODBCStmt, vectDB_INFLUENCE_WAR_INFO *i_pvectDB_INFLUENCE_WAR_INFO);

    ///////////////////////////////////////////////////////////////////////////////
    // 2008-04-16 by cmkwon, 몬스터 사망 시 몬스터 소환 이벤트 시스템 구현 - 
    static BOOL LoadEventMonster(mtvectSEVENT_MONSTER *o_pEventMonsterList, CODBCStatement *i_pOdbcStmt);

    // 2008-09-10 by cmkwon, 통합아레나 맵설정(map.cfg) 관련 수정 - 
    static BOOL LoadArenaMapList(vectMapIndexList *o_pArenaMapIndexList, CODBCStatement *i_pODBCStmt);

    
    // Client Info Loading
    static int LoadAllBUILDINGNPC(CODBCStatement *i_pODBCStmt, vector<BUILDINGNPC> *o_pVectorBuildingNPC);
    static int Load_MEX_MONSTER_INFO(CODBCStatement *i_pODBCStmt, vector<MEX_MONSTER_INFO> *o_pVectorMonsterInfo);

    // 2008-04-16 by cmkwon, 몬스터 사망 시 몬스터 소환 이벤트 시스템 구현 - 
    static MEX_MONSTER_INFO * FindMEX_MONSTER_INFO(vector<MEX_MONSTER_INFO> *i_pVectorMonsterInfo, int i_nMonsterNum);

    ////////////////////////////////////////////////////////////////////////
    // 2009-09-09 ~ 2010 by dhjin, 인피니티 - HPAction 로딩
    static int  LoadHPActionByDB(CODBCStatement *i_pODBCStmt, ez_map<INT, vectHPAction> * o_pMapHPAction);
    static BOOL DBLoadInfinityModeInfo(CODBCStatement *i_pODBCStmt, vectorInfinityModeInfo * o_pvectInfinityModeInfo, vectMapIndexList *o_pArenaMapIndexList);

    ///////////////////////////////////////////////////////////////////////////////
    // 2009-02-12 by cmkwon, EP3-3 월드랭킹시스템 구현 - 
    static BOOL WRK_LoadSelfServiceInfor8ServerGroupInfo(CODBCStatement *i_pODBCStmt, SWRK_SERVICE_INFO *o_pServiInfo, SWRK_SERVER_GROUP *io_pServGroup) { return false; }

    static constexpr bool WRK_LevelArrangeRanking(CODBCStatement *i_pODBCStmt, SWRK_SERVICE_INFO *i_pServiInfo, SWRK_SERVER_GROUP *i_pServGroup) { return false; }
    static constexpr bool WRK_LevelLoadRanking(CODBCStatement *i_pODBCStmt, INT i_ServiUID, BYTE i_byRankingScope, mtvectSWRK_LEVEL_FOR_DB *o_pvectRankerList) { return false; }
    static constexpr bool WRK_LevelInsertRanking(CODBCStatement *i_pODBCStmt, SWRK_SERVICE_INFO *i_pServiInfo, BYTE i_byRankingScope, mtvectSWRK_LEVEL_FOR_DB *i_pvectRankerList) { return false; }
    static constexpr bool WRK_LevelUpdateRankingToWRankingServer(CODBCStatement *i_pODBCStmt, SWRK_SERVICE_INFO *i_pServiInfo, BYTE i_byRankingScope, mtvectSWRK_LEVEL_FOR_DB *i_pvectRankerList) { return false; }
    static constexpr bool WRK_LevelLoadRankingToWorldRankingServer(CODBCStatement *i_pODBCStmt, INT i_ExcludeServiUID, mtvectSWRK_LEVEL_FOR_DB *o_pvectRankerList) { return false; }

    static constexpr bool WRK_FameArrangeRanking(CODBCStatement *i_pODBCStmt, SWRK_SERVICE_INFO *i_pServiInfo, SWRK_SERVER_GROUP *i_pServGroup) { return false; }
    static constexpr bool WRK_FameLoadRanking(CODBCStatement *i_pODBCStmt, INT i_ServiUID, BYTE i_byRankingScope, mtvectSWRK_FAME_FOR_DB *o_pvectRankerList) { return false; }
    static constexpr bool WRK_FameInsertRanking(CODBCStatement *i_pODBCStmt, SWRK_SERVICE_INFO *i_pServiInfo, BYTE i_byRankingScope, mtvectSWRK_FAME_FOR_DB *i_pvectRankerList) { return false; }
    static constexpr bool WRK_FameUpdateRankingToWRankingServer(CODBCStatement *i_pODBCStmt, SWRK_SERVICE_INFO *i_pServiInfo, BYTE i_byRankingScope, mtvectSWRK_FAME_FOR_DB *i_pvectRankerList) { return false; }
    static constexpr bool WRK_FameLoadRankingToWorldRankingServer(CODBCStatement *i_pODBCStmt, INT i_ExcludeServiUID, mtvectSWRK_FAME_FOR_DB *o_pvectRankerList) { return false; }
    
    static constexpr bool WRK_PVPArrangeRanking(CODBCStatement *i_pODBCStmt, SWRK_SERVICE_INFO *i_pServiInfo, SWRK_SERVER_GROUP *i_pServGroup) { return false; }
    static constexpr bool WRK_PVPLoadRanking(CODBCStatement *i_pODBCStmt, INT i_ServiUID, BYTE i_byRankingScope, mtvectSWRK_PVP_FOR_DB *o_pvectRankerList) { return false; }
    static constexpr bool WRK_PVPInsertRanking(CODBCStatement *i_pODBCStmt, SWRK_SERVICE_INFO *i_pServiInfo, BYTE i_byRankingScope, mtvectSWRK_PVP_FOR_DB *i_pvectRankerList) { return false; }
    static constexpr bool WRK_PVPUpdateRankingToWRankingServer(CODBCStatement *i_pODBCStmt, SWRK_SERVICE_INFO *i_pServiInfo, BYTE i_byRankingScope, mtvectSWRK_PVP_FOR_DB *i_pvectRankerList) { return false; }
    static constexpr bool WRK_PVPLoadRankingToWorldRankingServer(CODBCStatement *i_pODBCStmt, INT i_ExcludeServiUID, mtvectSWRK_PVP_FOR_DB *o_pvectRankerList) { return false; }

    // parameter binding
    static void BindColITEM(SQLHSTMT &hstmt, ITEM &item, SQLINTEGER *cb);
    static void BindColCharacter(SQLHSTMT &hstmt, CHARACTER &character, CHARACTER_DB_EX &characterDBEX, SQLINTEGER *cb);
    static void BindColMONSTER(SQLHSTMT &hstmt, MONSTER_INFO &monster, SQLINTEGER *cb);
    static void BindColMonsterMultiTarget(SQLHSTMT &hstmt, MONSTER_MULTI_TARGET &target, SQLINTEGER *cb);        // 2011-03-21 by hskim, 인피니티 3차 - 몬스터 멀티 타겟팅 기능 추가
    static void BindColRareItemInfo(SQLHSTMT &hstmt, RARE_ITEM_INFO &rareItemInfo, SQLINTEGER *cb);

    // process log messages
    void ProcessLogMessages(SQLSMALLINT plm_handle_type, SQLHANDLE &plm_handle, const char *logstring, BOOL ConnInd);
    // 2009. 10. 27 by jskim 진행률 확인
    static void ProcessLoopMessage(HWND hWnd);
    // 2009. 10. 27 by jskim 진행률 확인
    static void ProcessLogMessagesStatic(SQLSMALLINT plm_handle_type, SQLHANDLE &plm_handle, const char *logstring, BOOL ConnInd);

    // 2009-07-09 by jwlee, 게임 정보 테이블 데이터 DB에 밀어넣기 툴 구현 - 
    // ExcelData to DB Saving..
// 2009. 10. 27 by jskim 진행률 확인
//     static int SaveItem(CODBCStatement *i_pODBCStmt, vector<ITEM> *o_pItem);
//     static int SaveMonsterItem(CODBCStatement *i_pODBCStmt, vector<MONSTER2ITEM> *o_pMonsterItemInfo);
//     static int SaveRareItemInfo(CODBCStatement *i_pODBCStmt, vector<RARE_ITEM_INFO> *o_pRareItemInfo);
//     static int SaveShop(CODBCStatement *i_pODBCStmt, vectSHOP_INFO *o_pShopInfo);
//     static int SaveCityTargetWarpMap(CODBCStatement *i_pODBCStmt, vectCITY_TARGET_WARPMAP *o_pTargetWarp);
//     static int SaveItemMixingInfo(CODBCStatement *i_pODBCStmt, vector<ITEM_MIXING_INFO> *o_pMapItemMixingInfo);
//     static int SaveEnchantInfo(CODBCStatement *i_pODBCStmt, vector<ENCHANT_INFO> *o_pEnchantInfo);
//     static int SaveStrategyPointSummonMapIndex(CODBCStatement *i_pODBCStmt, vectSTRATEGYPOINT_SUMMON_MAPINDEX *o_pStrategyPoint);
//     static int SaveArenaInfo(CODBCStatement *i_pODBCStmt, vectorSDBARENA_INFO *o_pArenaInfo);
//     static int SaveArenaMapinfo(CODBCStatement *i_pODBCStmt, vectorSDBARENA_MAPINFO *o_pArenaMapInfo);
//     static int SaveMysteryItemDrop(CODBCStatement *i_pODBCStmt, vectMYSTERY_ITEM_DROP *o_pMysteryItemDrop);
//     static int SaveInfluenceWar(CODBCStatement *i_pODBCStmt, vectDB_INFLUENCE_WAR_INFO *o_pInfluenceWar);
//     static int SaveOutPostNextWarTime(CODBCStatement *i_pODBCStmt, vectSOutPostInfo *o_pOutPostNextWarTime);
//     static int SaveActionByLevel(CODBCStatement *i_pODBCStmt, vectSACTION_BY_LEVEL_DB *o_pActionByLevel);
//     static int SaveMapInfo(CODBCStatement *i_pODBCStmt, vectorMAP_INFO *o_pMapInfo);
//     static int SaveBuildingNPC(CODBCStatement *i_pODBCStmt, vector<BUILDINGNPC> *o_pBuildingNPC);
//     static int SaveMonster(CODBCStatement *i_pODBCStmt, vectMONSTER_DATA *o_pMonster);
//     static int SaveTutorialInfo(CODBCStatement *i_pODBCStmt, vectSTutorialInfo *o_pTutorialInfo);
//     static int SaveMapObject(CODBCStatement *i_pODBCStmt, vectMAPOBJECT *o_pMapItemInfo);
//     static int SaveLuckyMachine(CODBCStatement *i_pODBCStmt, vectLUCKY_MACHINE *o_pLuckyMachine);
//     static int SaveQuestMatching(CODBCStatement *i_pODBCStmt, vectSQUEST_MATCHING *o_pQuestMatching);
//     static int SaveItemMatching(CODBCStatement *i_pODBCStmt, vectSITEM_MATCHING *o_pItemMatching);
//     static int SaveItemInfo(CODBCStatement *i_pODBCStmt, vectItem_Info *o_pItemInfo);
//     static int SaveInfluenceRate(CODBCStatement *i_pODBCStmt, vectSTI_INFLUENCERATE *o_pInfluenceRate);        // 2009-09-16 by cmkwon, 세력 초기화시 어뷰징 방지 구현 - ti_InfluenceRate 테이블 추가, 

    static int SaveItem(CODBCStatement *i_pODBCStmt, vector<ITEM> *o_pItem, HWND hWnd);
    static int SaveMonsterItem(CODBCStatement *i_pODBCStmt, vector<MONSTER2ITEM> *o_pMonsterItemInfo, HWND hWnd);
    static int SaveRareItemInfo(CODBCStatement *i_pODBCStmt, vector<RARE_ITEM_INFO> *o_pRareItemInfo, HWND hWnd);
    static int SaveShop(CODBCStatement *i_pODBCStmt, vectSHOP_INFO *o_pShopInfo, HWND hWnd);
    static int SaveCityTargetWarpMap(CODBCStatement *i_pODBCStmt, vectCITY_TARGET_WARPMAP *o_pTargetWarp, HWND hWnd);
    static int SaveItemMixingInfo(CODBCStatement *i_pODBCStmt, vector<ITEM_MIXING_INFO_TOOL> *o_pMapItemMixingInfo, HWND hWnd);                // 2012-02-20 by hskim, DB 정규화 - ItemMixingInfo
    static int SaveItemMixingElement(CODBCStatement *i_pODBCStmt, vector<ITEM_MIXING_ELEMENT_TOOL> *o_pMapItemMixingElement, HWND hWnd);    // 2012-02-20 by hskim, DB 정규화 - ItemMixingInfo
    static int SaveEnchantInfo(CODBCStatement *i_pODBCStmt, vector<ENCHANT_INFO> *o_pEnchantInfo, HWND hWnd);
    static int SaveStrategyPointSummonMapIndex(CODBCStatement *i_pODBCStmt, vectSTRATEGYPOINT_SUMMON_MAPINDEX *o_pStrategyPoint, HWND hWnd);
    static int SaveArenaInfo(CODBCStatement *i_pODBCStmt, vectorSDBARENA_INFO *o_pArenaInfo, HWND hWnd);
    static int SaveArenaMapinfo(CODBCStatement *i_pODBCStmt, vectorSDBARENA_MAPINFO *o_pArenaMapInfo, HWND hWnd);
    static int SaveMysteryItemDrop(CODBCStatement *i_pODBCStmt, vectMYSTERY_ITEM_DROP *o_pMysteryItemDrop, HWND hWnd);
    static int SaveInfluenceWar(CODBCStatement *i_pODBCStmt, vectDB_INFLUENCE_WAR_INFO *o_pInfluenceWar, HWND hWnd);
    static int SaveOutPostNextWarTime(CODBCStatement *i_pODBCStmt, vectSOutPostInfo *o_pOutPostNextWarTime, HWND hWnd);
    static int SaveActionByLevel(CODBCStatement *i_pODBCStmt, vectSACTION_BY_LEVEL_DB *o_pActionByLevel, HWND hWnd);
    static int SaveMapInfo(CODBCStatement *i_pODBCStmt, vectorMAP_INFO *o_pMapInfo, HWND hWnd);
    static int SaveBuildingNPC(CODBCStatement *i_pODBCStmt, vector<BUILDINGNPC> *o_pBuildingNPC, HWND hWnd);
    static int SaveMonster(CODBCStatement *i_pODBCStmt, vectMONSTER_DATA *o_pMonster, HWND hWnd);
    static int SaveTutorialInfo(CODBCStatement *i_pODBCStmt, vectSTutorialInfo *o_pTutorialInfo, HWND hWnd);
    static int SaveMapObject(CODBCStatement *i_pODBCStmt, vectMAPOBJECT *o_pMapItemInfo, HWND hWnd);
    static int SaveLuckyMachine(CODBCStatement *i_pODBCStmt, vectLUCKY_MACHINE *o_pLuckyMachine, HWND hWnd);
    static int SaveQuestMatching(CODBCStatement *i_pODBCStmt, vectSQUEST_MATCHING *o_pQuestMatching, HWND hWnd);
    static int SaveItemMatching(CODBCStatement *i_pODBCStmt, vectSITEM_MATCHING *o_pItemMatching, HWND hWnd);
    static int SaveItemInfo(CODBCStatement *i_pODBCStmt, vectItem_Info *o_pItemInfo, HWND hWnd);
    static int SaveInfluenceRate(CODBCStatement *i_pODBCStmt, vectSTI_INFLUENCERATE *o_pInfluenceRate, HWND hWnd);        // 2009-09-16 by cmkwon, 세력 초기화시 어뷰징 방지 구현 - ti_InfluenceRate 테이블 추가, 
    ////////////////////////////////////////////////////////////////////////////////
    // 2009-09-09 ~ 2010 by dhjin, 인피니티 - Excel -> DB 밀기
    static int SaveHPAction(CODBCStatement *i_pODBCStmt, vectHPAction *i_pHPActionList, HWND hWnd);
    static int SaveCinema(CODBCStatement *i_pODBCStmt, vectorCinemaInfo *i_pCinemaInfoList, HWND hWnd);
    static int SaveRevision(CODBCStatement *i_pODBCStmt, vectorRevisionInfo *i_pRevisionInfoList, HWND hWnd);
    static int SaveInfinityMonster(CODBCStatement *i_pODBCStmt, vectorInfinityMonsterInfo *i_pInfinityMonsterInfoList, HWND hWnd);
    static int SaveInfinityMode(CODBCStatement *i_pODBCStmt, vectorInfinityModeInfo *i_pInfinityModeInfoList, HWND hWnd);
    static int SaveTender(CODBCStatement *i_pODBCStmt, vectorTenderInfo *i_pTenderInfoList, HWND hWnd);
    static int SaveInfinityShop(CODBCStatement *i_pODBCStmt, vectorInfinityShopInfo *i_pInfinityShopInfoList, HWND hWnd);        // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 상점
    static int SaveInvokingWearItemDPNum(CODBCStatement *i_pODBCStmt, InvokingWearItemDestParamList *i_pInvokingItemDPList, HWND hWnd);        // 2009-09-09 ~ 2010-02-10 by dhjin, 인피니티 - 발동류장착아이템
    static int SaveInvokingWearItemDPNumByUse(CODBCStatement *i_pODBCStmt, InvokingWearItemDestParamList *i_pInvokingItemDPList, HWND hWnd);        // 2009-09-09 ~ 2010-02-10 by dhjin, 인피니티 - 발동류장착아이템
    //end 2009. 10. 27 by jskim 진행률 확인    

    static int SaveLevelAdjustmentList(CODBCStatement *i_pODBCStmt, vectorInfinity_DifficultyInfo_Monster  *i_InvokingItemDPByUseList, HWND hWnd); // 2010-05-24 by shcho, 인피니티 난이도 조절 - 인피니티 난이도 조정에 관한 툴 업데이트
    static int SaveLevelAdjustmentRewardList(CODBCStatement *i_pODBCStmt, vectorInfinity_DifficultyInfo_Bonus *i_InvokingItemDPByUseList, HWND hWnd); // 2010-05-24 by shcho, 인피니티 난이도 조절 - 인피니티 난이도 조정에 관한 툴 업데이트
    static int SaveBurningMapList(CODBCStatement *i_pODBCStmt, BurningMapInfoList *i_pBurningMapList, HWND hWnd); // 2010-08-05 by dhjin, 버닝맵 - 

    ///////////////////////////////////////////////////////////////////////////////
    // 2010-06-15 by shcho&hslee 펫시스템 - 팻 정보 엑셀 밀기
    static int SavePetBaseData(CODBCStatement *i_pODBCStmt, vectorPetBaseData  *i_InvokingItemDPByUseList, HWND hWnd);
    static int SavePetLevelData(CODBCStatement *i_pODBCStmt, vectorPetLevelData  *i_InvokingItemDPByUseList, HWND hWnd);

    ///////////////////////////////////////////////////////////////////////////////
    // 2010-01-18 by cmkwon, 아이템 사용시 Parameter 중복 체크 시스템 구현 - 
    static BOOL LoadItemParamOverlap(CODBCStatement *i_pODBCStmt, vectSITEM_PARAM_OVERLAP *o_ItemParamOverList);
    static int SaveItemParamOverlap(CODBCStatement *i_pODBCStmt, vectSITEM_PARAM_OVERLAP *i_pItemParamOverlapList, HWND hWnd);

};

#ifdef SHOP_PRICES_PER_BUILDING_NPC
#ifdef _Parameter_ADDITIONAL
    #define CB_COUNT_ITEM                83
#else
    #define CB_COUNT_ITEM                79
#endif
#else    // SHOP_PRICES_PER_BUILDING_NPC
    #define CB_COUNT_ITEM                80    // # of fields + 1    // 2009-09-09 ~ 2010-02-10 by dhjin, 인피니티 - 발동류장착아이템 77 -> 80    // 2009-04-21 by cmkwon, ITEM에 DesParam 필드 개수 8개로 늘리기 - 기존 69
#endif    // SHOP_PRICES_PER_BUILDING_NPC

#define CB_COUNT_CHARACTER            64    // # of fields + 1    // 2009-02-12 by cmkwon, EP3-3 월드랭킹시스템 구현 - 64로수정, // 2008-06-23 by dhjin, EP3 유저정보옵션 - // 2007-06-07 by dhjin
#define CB_COUNT_MONSTER            58    // # of fields + 1    // 2010-04-14 by cmkwon, 인피2차 몬스터 랜텀 타겟 변경 처리 - ChangeTargetTime 필드 추가, // 2010-03-31 by dhjin, 인피니티(기지방어) - 54 -> 56 // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 41 -> 54으로 수정, // 2007-09-05 by dhjin, // 2011-05-23 by hskim, 인피니티 3차 - 웨이포인트 구현 - WayPointPattern 필드 추가
#define CB_COUNT_RARE_ITEM_INFO        32    // # of fields + 1

///////////////////////////////////////////////////////////////////////////////
// 2009-10-23 by cmkwon, Monitor 엑셀파일 밀어넣기시 체크 추가 - CB_COUNT_### of fields + 1
#define SIZE_MAX_TB_FIELD_COUNT                        100
#define CB_COUNT_ti_Item                            CB_COUNT_ITEM
#define CB_COUNT_ti_MonsterItem                        9                // 2010-04-09 by cmkwon, 인피2차 추가 수정(단계별 보상 추가) - 필드 1개추가
#define CB_COUNT_ti_RareItemInfo                       32
#define CB_COUNT_ti_Shop                               4
#define CB_COUNT_ti_CityTargetWarpMap                  7
#define CB_COUNT_ti_ItemMixingInfo                     5                // 2013-05-06 by hskim, 아이템 미리 보기 (속성 값 추가) - // 2012-02-20 by hskim, DB 정규화 - ItemMixingInfo
#define CB_COUNT_ti_ItemMixingElement                3                // 2012-02-20 by hskim, DB 정규화 - ItemMixingInfo
#define CB_COUNT_ti_EnchantInfo                        44
#define CB_COUNT_ti_StrategyPointSummonMapIndex        4
#define CB_COUNT_ti_ArenaInfo                          7
#define CB_COUNT_ti_ArenaMapinfo                       16
#define CB_COUNT_ti_MysteryItemDrop                    14
#define CB_COUNT_ti_InfluenceWar                       57
#define CB_COUNT_ti_OutPostNextWarTime                 2
#define CB_COUNT_ti_ActionByLevel                      3
#define CB_COUNT_ti_MapInfo                            54                        // 2009-09-09 ~ 2010 by dhjin, 인피니티 -     
#define CB_COUNT_ti_BuildingNPC                        10
#define CB_COUNT_ti_Monster                            (CB_COUNT_MONSTER + 1) // 추가 이유: SizeForServer, SizeForClient
#define CB_COUNT_ti_TutorialInfo                       8
#define CB_COUNT_ti_MapObject                          11
#define CB_COUNT_ti_LuckyMachine                       11
#define CB_COUNT_ti_QuestMatching                      3
#define CB_COUNT_ti_ItemMatching                       3
#define CB_COUNT_ti_InfluenceRate                      3
// 2009-09-09 ~ 2010 by dhjin, 인피니티 - Excel -> DB 밀기
#define CB_COUNT_ti_HPAction                          15
#define CB_COUNT_ti_Cinema                              24            // 2010-03-29 by cmkwon, 인피2차 시네마에 효과음 파일 설정 추가 - 기존(23), // 2010-03-31 by dhjin, 인피니티(기지방어) -     
#define CB_COUNT_ti_InfinityMonster                    6
#define CB_COUNT_ti_Revision                          8
#define CB_COUNT_ti_InfinityMode                      15            // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 사망시 패널티 추가
#define CB_COUNT_ti_Tender                              3
#define CB_COUNT_ti_InfinityShop                      14            // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 상점
#define CB_COUNT_ti_ItemParamOverlap                  3            // 2010-01-18 by cmkwon, 아이템 사용시 Parameter 중복 체크 시스템 구현 - 
#define CB_COUNT_ti_InvokingWearItemDPNum              5            // 2009-09-09 ~ 2010-02-10 by dhjin, 인피니티 - 발동류장착아이템 
#define CB_COUNT_ti_InvokingWearItemDPNumByUse      5            // 2009-09-09 ~ 2010-02-10 by dhjin, 인피니티 - 발동류장착아이템 
#define CB_COUNT_ti_LevelAdjustment                    7            // 2010-05-24 by shcho, 인피니티 난이도 조절 - 인피니티 난이도 조정에 관한 툴 업데이트
#define CB_COUNT_ti_LevelAdjustmentReward            5            // 2010-05-24 by shcho, 인피니티 난이도 조절 - 인피니티 난이도 조정에 관한 툴 업데이트
#define CB_COUNT_ti_BurningMap                        6            // 2010-08-05 by dhjin, 버닝맵 - 
#define CB_COUNT_ti_Pet                                7            // 2011-08-05 by hskim, 파트너 시스템 2차 - 자료 구조 결정 - // 2010-06-15 by shcho&hslee 펫시스템 - 팻 기본 정보 엑셀 밀기
#define CB_COUNT_ti_petLevel                        9            // 2011-08-05 by hskim, 파트너 시스템 2차 - 자료 구조 결정 - // 2010-06-15 by shcho&hslee 펫시스템 - 팻 레벨 정보 엑셀 밀기
#define CB_COUNT_ti_OperatorAction                    6            // 2010-06-15 by shcho&hslee 펫시스템 - DB 밀기 추가 작업 펫 오퍼레이터 정보.                
#define CB_COUNT_ti_DissolutionItem                    13            // 2010-08-31 by shcho&jskim 아이템용해 시스템 - 용해 아이템 정보.
#define CB_COUNT_ti_MonsterMultiTarget                5            // 2011-03-17 by hskim, 인피니티 3차 - 몬스터 멀티 타겟팅 기능 추가
// start 2011-05-23 by hskim, 인피니티 3차 - 웨이포인트 구현
#define CB_COUNT_ti_WayPoint                        6
#define CB_COUNT_ti_WayPointMove                    2
// end 2011-05-23 by hskim, 인피니티 3차 - 웨이포인트 구현
#define CB_COUNT_ti_MapBuff                            2            // 2011-10-10 by hskim, EP4 [트리거 시스템] - 화산재 / 모래 폭풍
// start 2011-10-28 by hskim, EP4 [트리거 시스템] - 크리스탈 시스템
#define CB_COUNT_ti_MapTrigger                        4
#define CB_COUNT_ti_TriggerFunctionCrystal            4
#define CB_COUNT_ti_TriggerCrystalGroup                3
#define CB_COUNT_ti_TriggerCrystalDestroyGroup        3
// end 2011-10-28 by hskim, EP4 [트리거 시스템] - 크리스탈 시스템
// start 2011-11-07 by hskim, EP4 [트리거 시스템] - NCG 거점전 시스템
#define CB_COUNT_ti_TriggerFunctionNGCInflWar        8
#define CB_COUNT_ti_TriggerNGCInflWarMonsterGroup    4
// end 2011-11-07 by hskim, EP4 [트리거 시스템] - NCG 거점전 시스템
// 2013-01-21 by jhseol, NGC 전전기지 트리거 시스템
#define CB_COUNT_ti_TriggerFunctionNGCOutPost        8
#define CB_COUNT_ti_TriggerOutPostBossKill            4
#define CB_COUNT_ti_TriggercrystalBuff                2
// end 2013-01-21 by jhseol, NGC 전전기지 트리거 시스템
#define CB_COUNT_ti_ti_OverlapItem                    4            // 2012-03-13 by hskim, 현지화 관련 DB 구조 변경

#define TB_NAME_ti_Item                                "ti_Item"
#define TB_NAME_ti_MonsterItem                        "ti_MonsterItem"
#define TB_NAME_ti_RareItemInfo                       "ti_RareItemInfo"
#define TB_NAME_ti_Shop                               "ti_Shop"
#define TB_NAME_ti_CityTargetWarpMap                  "ti_CityTargetWarpMap"
#define TB_NAME_ti_ItemMixingInfo                     "ti_ItemMixingInfo"
#define TB_NAME_ti_ItemMixingElement                 "ti_ItemMixingElement"        // 2012-02-20 by hskim, DB 정규화 - ItemMixingInfo
#define TB_NAME_ti_EnchantInfo                        "ti_EnchantInfo"
#define TB_NAME_ti_StrategyPointSummonMapIndex        "ti_StrategyPointSummonMapIndex"
#define TB_NAME_ti_ArenaInfo                          "ti_ArenaInfo"
#define TB_NAME_ti_ArenaMapinfo                       "ti_ArenaMapinfo"
#define TB_NAME_ti_MysteryItemDrop                    "ti_MysteryItemDrop"
#define TB_NAME_ti_InfluenceWar                       "ti_InfluenceWar"
#define TB_NAME_ti_OutPostNextWarTime                 "ti_OutPostNextWarTime"
#define TB_NAME_ti_ActionByLevel                      "ti_ActionByLevel"
#define TB_NAME_ti_MapInfo                            "ti_MapInfo"
#define TB_NAME_ti_BuildingNPC                        "ti_BuildingNPC"
#define TB_NAME_ti_Monster                            "ti_Monster"
#define TB_NAME_ti_TutorialInfo                       "ti_TutorialInfo"
#define TB_NAME_ti_MapObject                          "ti_MapObject"
#define TB_NAME_ti_LuckyMachine                       "ti_LuckyMachine"
#define TB_NAME_ti_QuestMatching                      "ti_QuestMatching"
#define TB_NAME_ti_ItemMatching                       "ti_ItemMatching"
#define TB_NAME_ti_InfluenceRate                      "ti_InfluenceRate"
// 2009-09-09 ~ 2010 by dhjin, 인피니티 - Excel -> DB 밀기
#define TB_NAME_ti_HPAction                          "ti_HPAction"
#define TB_NAME_ti_Cinema                              "ti_Cinema"
#define TB_NAME_ti_InfinityMonster                    "ti_InfinityMonster"
#define TB_NAME_ti_Revision                              "ti_Revision"
#define TB_NAME_ti_InfinityMode                        "ti_InfinityMode"
#define TB_NAME_ti_Tender                            "ti_Tender"
#define TB_NAME_ti_InfinityShop                        "ti_InfinityShop"        // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 상점
#define TB_NAME_ti_ItemParamOverlap                    "ti_ItemParamOverlap"        // 2010-01-18 by cmkwon, 아이템 사용시 Parameter 중복 체크 시스템 구현 - 
#define TB_NAME_ti_InvokingWearItemDPNum            "ti_InvokingWearItemDPNum"        // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 상점
#define TB_NAME_ti_InvokingWearItemDPNumByUse        "ti_InvokingWearItemDPNumByUse"        // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 상점
#define TB_NAME_ti_LevelAdjustment                    "ti_LevelAdjustment"        // 2010-05-24 by shcho, 인피니티 난이도 조절 - 인피니티 난이도 조정에 관한 툴 업데이트
#define TB_NAME_ti_LevelAdjustmentReward            "ti_LevelAdjustmentReward"    // 2010-05-24 by shcho, 인피니티 난이도 조절 - 인피니티 난이도 조정에 관한 툴 업데이트
#define TB_NAME_ti_BurningMap                        "ti_BurningMap"        // 2010-08-05 by dhjin, 버닝맵 - 
#define TB_NAME_ti_Pet                                "ti_pet"            // 2010-06-15 by shcho&hslee 펫시스템 - 팻 기본 정보 엑셀 밀기
#define TB_NAME_ti_PetLevel                            "ti_PetLevel"        // 2010-06-15 by shcho&hslee 펫시스템 - 팻 레벨 정보 엑셀 밀기
#define TB_NAME_ti_OperatorAction                    "ti_OperatorAction" // 2010-06-15 by shcho&hslee 펫시스템 - DB 밀기 추가 작업 펫 오퍼레이터 정보.
#define TB_NAME_ti_DissolutionItem                    "ti_DissolutionItem"    // 2010-08-31 by shcho&jskim 아이템용해 시스템 - 용해 아이템 정보.
#define TB_NAME_ti_MonsterMultiTarget                "ti_MonsterMultiTarget"    // 2011-03-17 by hskim, 인피니티 3차 - 몬스터 멀티 타겟팅 기능 추가
#define TB_NAME_ti_WayPoint                            "ti_WayPoint"        // 2011-05-23 by hskim, 인피니티 3차 - 웨이포인트 구현
#define TB_NAME_ti_WayPointMove                        "ti_WayPointMove"    // 2011-05-23 by hskim, 인피니티 3차 - 웨이포인트 구현
#define TB_NAME_ti_MapBuff                            "ti_MapBuff"        // 2011-10-10 by hskim, EP4 [트리거 시스템] - 화산재 / 모래 폭풍
#define TB_NAME_ti_MapTrigger                        "ti_MapTrigger"                    // 2011-10-28 by hskim, EP4 [트리거 시스템] - 크리스탈 시스템
#define TB_NAME_ti_TriggerFunctionCrystal            "ti_TriggerFunctionCrystal"        // 2011-10-28 by hskim, EP4 [트리거 시스템] - 크리스탈 시스템
#define TB_NAME_ti_TriggerCrystalGroup                "ti_TriggerCrystalGroup"        // 2011-10-28 by hskim, EP4 [트리거 시스템] - 크리스탈 시스템
#define TB_NAME_ti_TriggerCrystalDestroyGroup        "ti_TriggerCrystalDestroyGroup"    // 2011-10-28 by hskim, EP4 [트리거 시스템] - 크리스탈 시스템
#define TB_NAME_ti_TriggerFunctionNGCInflWar        "ti_TriggerFunctionNGCInflWar"    // 2011-11-07 by hskim, EP4 [트리거 시스템] - NCG 거점전 시스템
#define TB_NAME_ti_TriggerNGCInflWarMonsterGroup    "ti_TriggerNGCInflWarMonsterGrou"    // 2011-11-07 by hskim, EP4 [트리거 시스템] - NCG 거점전 시스템 - 엑셀 이름 길이 제한 문에 마지막 'p' 삭제
#define TB_NAME_ti_TriggerFunctionNGCOutPost        "ti_TriggerFunctionNGCOutPost"    // 2013-01-21 by jhseol, NGC 전전기지 트리거 시스템
#define TB_NAME_ti_TriggerOutPostBossKill            "ti_TriggerOutPostBossKill"        // 2013-01-21 by jhseol, NGC 전전기지 트리거 시스템
#define TB_NAME_ti_TriggerCrystalBuff                "ti_TriggerCrystalBuff"            // 2013-01-21 by jhseol, NGC 전전기지 트리거 시스템
#define TB_NAME_ti_OverlapItem                        "ti_OverlapItem"        // 2012-03-13 by hskim, 현지화 관련 DB 구조 변경

// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
// // 2008-07-02 by cmkwon, MySQL 지원 구현 - 
// // #define MONSTER_SQL_QUERY_FOR_CLIENT    "SELECT UniqueNumber, MonsterName, Level, Experience, HP, \
// //                                 Race, Speed, MonsterItem01, MonsterItem02, MonsterItem03, \
// //                                 MonsterItem04, MonsterItem05, ControlSkill, SizeForClient, Faction, \
// //                                 MonsterForm, AttackPattern, MovePattern, Belligerence, Defense, \
// //                                 DefenceProbability, AlphaBlending, AttackObject, AttackRange, HPRecoveryTime, HPRecoveryValue, \
// //                                 RenderIndex, ScaleValue, TextureIndex, SourceIndex, QuickTurnAngle, \
// //                                 QuickSpeed, QuickTurnTerm, MPOption, MPOptionParam1, MPOptionParam2, \
// //                                 Range, TurnAngle, MonsterHabit, ClickEvent FROM ti_Monster WITH (NOLOCK)"
// #define MONSTER_SQL_QUERY_FOR_CLIENT    QUERY_080702_0062        
// 
// // 2008-07-02 by cmkwon, MySQL 지원 구현 - 
// // #define MONSTER_SQL_QUERY_FOR_SERVER    "SELECT UniqueNumber, MonsterName, Level, Experience, HP, \
// //                                 Race, Speed, MonsterItem01, MonsterItem02, MonsterItem03, \
// //                                 MonsterItem04, MonsterItem05, ControlSkill, SizeForServer, Faction, \
// //                                 MonsterForm, AttackPattern, MovePattern, Belligerence, Defense, \
// //                                 DefenceProbability, AlphaBlending, AttackObject, AttackRange, HPRecoveryTime, HPRecoveryValue, \
// //                                 RenderIndex, ScaleValue, TextureIndex, SourceIndex, QuickTurnAngle, \
// //                                 QuickSpeed, QuickTurnTerm, MPOption, MPOptionParam1, MPOptionParam2, \
// //                                 Range, TurnAngle, MonsterHabit, ClickEvent FROM ti_Monster WITH (NOLOCK)"
// #define MONSTER_SQL_QUERY_FOR_SERVER    QUERY_080702_0063


#endif // COMMON_ATUMDBHELPER_H_
