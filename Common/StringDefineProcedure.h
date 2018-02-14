
// 2008-08-22 by cmkwon, MySQL 통합(Procedure 뽑아내기) - 
// #include "StringDefineProcedure.h"
#ifndef _STRING_DEFINE_PROCEDURE_H_
#define _STRING_DEFINE_PROCEDURE_H_

#include "LocalizationDefineCommon.h"		

// 2008-08-20 by cmkwon, MySQL 관련 소스 통합 - 
//#define DB_SERVER_MYSQL

///////////////////////////////////////////////////////////////////////////////
// 2008-08-22 by cmkwon, MySQL 통합(Procedure 뽑아내기) - 
#define PROCEDURE_080822_0001 (UCHAR*)"{call dbo.atum_InsertLeaderItem(?,?,?)}"
#define PROCEDURE_080822_0002 (UCHAR*)"{call dbo.atum_UpdateCityLeaderByLeaderSet(?,?)}"
#define PROCEDURE_080822_0003 (UCHAR*)"{call dbo.atum_admin_GetAllDeletedCharacter(?)}"
#define PROCEDURE_080822_0004 (UCHAR*)"{call dbo.atum_LiveDeletedCharacter(?,?)}"
#define PROCEDURE_080822_0005 (UCHAR*)"{call dbo.atum_admin_DeleteDeletedCharacter(?,?)}"

#define PROCEDURE_080822_0006 (UCHAR*)"{call dbo.atum_DeleteLeaderItem}"
#define PROCEDURE_080822_0007 (UCHAR*)"{call dbo.atum_deleteAllWarpableUserList}"
#define PROCEDURE_080822_0008 (UCHAR*)"{call dbo.atum_InitCityLeaderForInflLeader}"
#define PROCEDURE_080822_0009 (UCHAR*)"{call dbo.atum_InitVoterList}"
#define PROCEDURE_080822_0010 (UCHAR*)"{call dbo.atum_InitLeaderCandidate}"

#define PROCEDURE_080822_0011 (UCHAR*)"{call dbo.atum_log_InitMSWar}"
#define PROCEDURE_080822_0012 (UCHAR*)"{call dbo.atum_log_InitSPWar}"
#define PROCEDURE_080822_0013 (UCHAR*)"{call dbo.atum_LoadPollDate}"
#define PROCEDURE_080822_0014 (UCHAR*)"{call dbo.atum_UpdatePollDate(?,?,?,?,?)}"
#define PROCEDURE_080822_0015 (UCHAR*)"{call dbo.atum_LoadLeaderCandidate}"
							 
#define PROCEDURE_080822_0016 (UCHAR*)"{call dbo.atum_log_select_integration (?)}"
#define PROCEDURE_080822_0017 (UCHAR*)"{call dbo.atum_LoadOutPostInfo}"
#define PROCEDURE_080822_0018 (UCHAR*)"{call dbo.atum_LoadAllStrategyPointSummonInfo}"
#define PROCEDURE_080822_0019 (UCHAR*)"{call dbo.atum_LoadStrategyPointSummonRange}"
#define PROCEDURE_080822_0020 (UCHAR*)"{call dbo.atum_LoadStrategyPointSummonRangeTimeInfo}"

#define PROCEDURE_080822_0021 (UCHAR*)"{call dbo.atum_UpdateStrategyPointSummonRange(?,?,?)}"
#define PROCEDURE_080822_0022 (UCHAR*)"{call dbo.atum_LoadStrategyPointNotSummonTime}"
#define PROCEDURE_080822_0023 (UCHAR*)"{call dbo.atum_UpdateStrategyPointNotSummonTime(?,?,?,?,? ,?,?,?,?,? ,?,?,?,?,? ,?,?,?,?,? ,?)}"
#define PROCEDURE_080822_0024 (UCHAR*)"{call dbo.atum_InitStrategyPointSummonRange(?,?,?)}"
#define PROCEDURE_080822_0025 (UCHAR*)"{call dbo.atum_admin_InsertStoreItemCountable_DB(?, ?, ?)}"
							 
#define PROCEDURE_080822_0026 (UCHAR*)"{call dbo.atum_admin_GetCharacterByCharacterName(?)}"
#define PROCEDURE_080822_0027 (UCHAR*)"{call dbo.atum_load_ti_MysteryItemDrop}"		// 2008-11-04 by dhjin, 럭키머신
#define PROCEDURE_080822_0028 (UCHAR*)"{call dbo.atum_log_insert_user_game_start_end(?,?,?,?,?, ?,?,?,?,?, ?,?,?)}"
#define PROCEDURE_080822_0029 (UCHAR*)"{call dbo.atum_log_insert_item_trade(?, ?, ?, ?, ?, ?, ?, ?, ?)}"
#define PROCEDURE_080822_0030 (UCHAR*)"{call dbo.Insert_atum_log_Live_Deleted_Character(?, ?, ?, ?, ?)}"

#define PROCEDURE_080822_0031 (UCHAR*)"{call dbo.atum_GetAttachedItems(?)}"
#define PROCEDURE_080822_0032 (UCHAR*)"{call dbo.atum_LoadArenaInfo}"
#define PROCEDURE_080822_0033 (UCHAR*)"{call dbo.atum_LoadArenaMapInfo}"
#define PROCEDURE_080822_0034 (UCHAR*)"{call dbo.atum_LoadTutorialInfo}"
// 2008-08-22 by cmkwon, MySQL 통합(Procedure 뽑아내기) - PROCEDURE_080822_0017로 대체 #define PROCEDURE_080822_0035 (UCHAR*)"{call atum_LoadOutPostInfo}"
							 
#define PROCEDURE_080822_0036 (UCHAR*)"{call dbo.atum_LoadOutPostNextWarTime}"
#define PROCEDURE_080822_0037 (UCHAR*)"{call dbo.atum_LoadCityLeaderInfo}"
#define PROCEDURE_080822_0038 (UCHAR*)"{call dbo.atum_LoadActionByLevel}"
// 2008-08-22 by cmkwon, MySQL 통합(Procedure 뽑아내기) - PROCEDURE_080822_0013 으로 대체 #define PROCEDURE_080822_0039 (UCHAR*)"{call atum_LoadPollDate}"
#define PROCEDURE_080822_0040 (UCHAR*)"{call dbo.atum_LoadServerGroupInfo}"

#define PROCEDURE_080822_0041 (UCHAR*)"{call dbo.atum_UpdateServerGroupInfo(?,?,?)}"
#define PROCEDURE_080822_0042 (UCHAR*)"{call dbo.atum_GetCharacterNumByName(?)}"
#define PROCEDURE_080822_0043 (UCHAR*)"{call dbo.atum_CreateCharacter(?,?,?,?,?, ?,?,?,?,?, ?,?,?,?,?, ?,?,?,?,?,  ?,?,?,?,?, ?,?,?,?,?, ?,?,?,?,?, ?,?,?,?,?,  ?,?,?,?,?, ?,?,?)}"
// 2008-08-22 by cmkwon, MySQL 통합(Procedure 뽑아내기) - PROCEDURE_080822_0042로 대체 #define PROCEDURE_080822_0044 (UCHAR*)"{call atum_GetCharacterNumByName(?)}"
#define PROCEDURE_080822_0045 (UCHAR*)"{call dbo.atum_DeleteCharacter(?)}"
							 
#define PROCEDURE_080822_0046 (UCHAR*)"{call dbo.atum_GetAccountUniqueNumber(?)}"
#define PROCEDURE_080822_0047 (UCHAR*)"{call dbo.atum_GetCharacter(?, ?)}"
#define PROCEDURE_080822_0048 (UCHAR*)"{call dbo.atum_GetAllCharacterInfoByID(?)}"
#define PROCEDURE_080822_0049 (UCHAR*)"{call dbo.atum_GetAccountCashStoreItem(?)}"
#define PROCEDURE_080822_0050 (UCHAR*)"{call dbo.atum_SaveCharacterCriticalData(?,?,?,?,?,  ?,?,?,?,?,  ?,?,?,?,?, ?,?,?,?,?, ?,?,?,?,?, ?,?)}"

#define PROCEDURE_080822_0051 (UCHAR*)"{call dbo.atum_ChangeUnitKind(?, ?)}"
#define PROCEDURE_080822_0052 (UCHAR*)"{call dbo.atum_ChangeStat(	?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)}"
#define PROCEDURE_080822_0053 (UCHAR*)"{call dbo.atum_ChangeBonusStatPoint(?, ?)}"
#define PROCEDURE_080822_0054 (UCHAR*)"{call dbo.atum_ChangeGuild(?, ?)}"
#define PROCEDURE_080822_0055 (UCHAR*)"{call dbo.atum_ChangeExp(?, ?)}"
							 
#define PROCEDURE_080822_0056 (UCHAR*)"{call dbo.atum_ChangeLevel(?, ?, ?)}"
#define PROCEDURE_080822_0057 (UCHAR*)"{call dbo.atum_ChangeBodyCondition(?, ?)}"
#define PROCEDURE_080822_0058 (UCHAR*)"{call dbo.atum_ChangeInfluenceType(?,?,?,?)}"
#define PROCEDURE_080822_0059 (UCHAR*)"{call dbo.atum_ChangeStatus(?, ?)}"
#define PROCEDURE_080822_0060 (UCHAR*)"{call dbo.atum_ChangePKPoint(?, ?, ?, ?)}"

#define PROCEDURE_080822_0061 (UCHAR*)"{call dbo.atum_ChangeRacingPoint(?, ?)}"
#define PROCEDURE_080822_0062 (UCHAR*)"{call dbo.atum_UpdateTotalPlayTime(?, ?)}"
#define PROCEDURE_080822_0063 (UCHAR*)"{call dbo.atum_UpdateLastStartedTime(?)}"
#define PROCEDURE_080822_0064 (UCHAR*)"{call dbo.atum_ChangeHPDPSPEP(?, ?, ?, ?, ?, ?, ?, ?, ?)}"
#define PROCEDURE_080822_0065 (UCHAR*)"{call dbo.atum_ChangeCurrentHPDPSPEP(?, ?, ?, ?, ?)}"
							 
#define PROCEDURE_080822_0066 (UCHAR*)"{call dbo.atum_ChangeMapChannel(?, ?, ?)}"
#define PROCEDURE_080822_0067 (UCHAR*)"{call dbo.atum_ChangePetInfo(?, ?, ?)}"
#define PROCEDURE_080822_0068 (UCHAR*)"{call dbo.atum_ChangePosition(?, ?, ?, ?)}"
#define PROCEDURE_080822_0069 (UCHAR*)"{call dbo.atum_UpdateItemPossess(?, ?)}"
#define PROCEDURE_080822_0070 (UCHAR*)"{call dbo.atum_UpdateItemStorage(?, ?, ?)}"

#define PROCEDURE_080822_0071 (UCHAR*)"{call dbo.atum_GetEventItemList(?,?,?)}"		// 2009-11-19 by cmkwon, 아이템 이벤트에 Account/Character 체크 기능 추가 - 인자 1개 추가
#define PROCEDURE_080822_0072 (UCHAR*)"{call dbo.atum_insert_Log_ItemEvent(?, ?, ?, ?)}"
#define PROCEDURE_080822_0073 (UCHAR*)"{call dbo.atum_UseCouponNumber(?, ?, ?)}"
#define PROCEDURE_080822_0074 (UCHAR*)"{call dbo.atum_Get_GuildStore(?, ?)}"
#define PROCEDURE_080822_0075 (UCHAR*)"{call dbo.atum_GetEnchantBYItemUID(?)}"
							 
#define PROCEDURE_080822_0076 (UCHAR*)"{call dbo.atum_Update_Guild_Store(?, ?, ?, ?, ?)}"

// 2009-08-25 by cmkwon, 사용하지 않는 필드 제거(td_Store.ScarcityNumber) - 인자 1개 제거
// 2009-08-26 by cmkwon, 그래픽 리소스 변경 시스템 구현 - td_Store.ShapeItemNum 필드 추가, 인자 1개 추가됨
#define PROCEDURE_080822_0077 (UCHAR*)"{call dbo.atum_InsertStoreItem(?,?,?,?,?, ?,?,?,?,?, ?,?,?,?)}"

#define PROCEDURE_080822_0078 (UCHAR*)"{call dbo.atum_Delete_Guild_Store(?, ?, ?)}"
#define PROCEDURE_080822_0079 (UCHAR*)"{call dbo.atum_AllDelete_Guild_Store(?, ?)}"
#define PROCEDURE_080822_0080 (UCHAR*)"{call dbo.atum_getCharacterUIDByCharacterName(?, ?)}"

#define PROCEDURE_080822_0081 (UCHAR*)"{call dbo.atum_insert2WarpableUserList(?, ?)}"
#define PROCEDURE_080822_0082 (UCHAR*)"{call dbo.atum_deleteWarpableUser(?, ?)}"
#define PROCEDURE_080822_0083 (UCHAR*)"{call dbo.atum_Update_ConnectingServerGroupID_Of_Account(?, ?)}"
// 2008-08-22 by cmkwon, MySQL 통합(Procedure 뽑아내기) - PROCEDURE_080822_0077로 대체 #define PROCEDURE_080822_0084 (UCHAR*)"{call atum_InsertStoreItem(?,?,?,?,?, ?,?,?,?,?, ?,?,?,?)}"
#define PROCEDURE_080822_0085 (UCHAR*)"{call dbo.atum_Delete_Letter_By_LetterPassedRangeDay(?,?)}"
							 
#define PROCEDURE_080822_0086 (UCHAR*)"{call dbo.atum_Get_Letter(?,?)}"
#define PROCEDURE_080822_0087 (UCHAR*)"{call dbo.atum_Read_Letter(?,?)}"
#define PROCEDURE_080822_0088 (UCHAR*)"{call dbo.atum_Delete_Letter(?,?)}"
#define PROCEDURE_080822_0089 (UCHAR*)"{call dbo.atum_Get_AllLetter(?,?,?)}"
#define PROCEDURE_080822_0090 (UCHAR*)"{call dbo.atum_Insert_ReadAllLetter_ByRead(?,?)}"

#define PROCEDURE_080822_0091 (UCHAR*)"{call dbo.atum_Delete_ReadAllLetter(?,?)}"
#define PROCEDURE_080822_0092 (UCHAR*)"{call dbo.atum_Delete_AllLetter(?)}"
#define PROCEDURE_080822_0093 (UCHAR*)"{call dbo.atum_log_Get_GuildStore(?)}"
#define PROCEDURE_080822_0094 (UCHAR*)"{call dbo.atum_GetStoreItem(?, ?, ?)}"
// 2008-08-22 by cmkwon, MySQL 통합(Procedure 뽑아내기) - PROCEDURE_080822_0075로 대체 #define PROCEDURE_080822_0095 (UCHAR*)"{call atum_GetEnchantBYItemUID(?)}"
							 
#define PROCEDURE_080822_0096 (UCHAR*)"{call dbo.atum_GetStoreItemSkill(?, ?)}"
#define PROCEDURE_080822_0097 (UCHAR*)"{call dbo.atum_GetAllEnchant(?, ?)}"
#define PROCEDURE_080822_0098 (UCHAR*)"{call dbo.atum_DeleteStoreItem(?, ?)}"
// 2008-08-22 by cmkwon, MySQL 통합(Procedure 뽑아내기) - PROCEDURE_080822_0077로 대체 #define PROCEDURE_080822_0099 (UCHAR*)"{call atum_InsertStoreItem(?,?,?,?,?, ?,?,?,?,?, ?,?,?,?)}"
#define PROCEDURE_080822_0100 (UCHAR*)"{call dbo.atum_InsertStoreItemSkill(?, ?, ?, ?)}"

///////////////////////////////////////////////////////////////////////////////
// 2008-08-22 by cmkwon, MySQL 통합(Procedure 뽑아내기) - 101
#define PROCEDURE_080822_0101 (UCHAR*)"{call dbo.atum_UpdateStoreItemSkillUseTime(?, ?)}"
#define PROCEDURE_080822_0102 (UCHAR*)"{call dbo.atum_UpdateItemCount(?, ?)}"
#define PROCEDURE_080822_0103 (UCHAR*)"{call dbo.atum_UpdateItemNum(?, ?)}"
#define PROCEDURE_080822_0104 (UCHAR*)"{call dbo.atum_UpdateEndurance(?, ?)}"
#define PROCEDURE_080822_0105 (UCHAR*)"{call dbo.atum_StoreUpdateColorCode(?, ?)}"

#define PROCEDURE_080822_0106 (UCHAR*)"{call dbo.atum_UpdateItemUsingTimeStamp(?, ?)}"
#define PROCEDURE_080822_0107 (UCHAR*)"{call dbo.atum_UpdateItemRareFix(?, ?, ?)}"
#define PROCEDURE_080822_0108 (UCHAR*)"{call dbo.atum_insertDefaultItems(?)}"
#define PROCEDURE_080822_0109 (UCHAR*)"{call dbo.atum_UpdateItemWindowList(?, ?, ?)}"
#define PROCEDURE_080822_0110 (UCHAR*)"{call dbo.atum_ChangeWindowPosition(?, ?, ?, ?, ?, ?, ?, ?)}"

#define PROCEDURE_080822_0111 (UCHAR*)"{call dbo.atum_TradeMoveItem(?, ?, ?, ?)}"
#define PROCEDURE_080822_0112 (UCHAR*)"{call dbo.atum_InsertEnchant(?, ?, ?)}"
#define PROCEDURE_080822_0113 (UCHAR*)"{call dbo.atum_DeleteAllEnchant(?)}"
#define PROCEDURE_080822_0114 (UCHAR*)"{call dbo.atum_GetAllQuest(?)}"
#define PROCEDURE_080822_0115 (UCHAR*)"{call dbo.atum_GetAllQuestMonsterCount(?)}"
							 
#define PROCEDURE_080822_0116 (UCHAR*)"{call dbo.atum_InsertQuest(?, ?, ?, ?, ?, ?)}"
#define PROCEDURE_080822_0117 (UCHAR*)"{call dbo.atum_UpdateQuestState(?, ?, ?, ?)}"
#define PROCEDURE_080822_0118 (UCHAR*)"{call dbo.atum_DeleteQuest(?, ?)}"
#define PROCEDURE_080822_0119 (UCHAR*)"{call dbo.atum_UpdateQuestMonsterCount(?, ?, ?, ?)}"
#define PROCEDURE_080822_0120 (UCHAR*)"{call dbo.atum_DeleteQuestMonsterCount(?, ?)}"

#define PROCEDURE_080822_0121 (UCHAR*)"{call dbo.atum_AuctionCancelItem(?, ?, ?)}"
#define PROCEDURE_080822_0122 (UCHAR*)"{call dbo.atum_insert_AccountCashStoreItem(?, ?, ?, ?)}"
#define PROCEDURE_080822_0123 (UCHAR*)"{call dbo.atum_update_AccountCashStoreItem(?, ?, ?, ?)}"
#define PROCEDURE_080822_0124 (UCHAR*)"{call dbo.atum_delete_AccountCashStoreItem(?)}"
#define PROCEDURE_080822_0125 (UCHAR*)"{call dbo.atum_LoadHappyHourEvent}"
							 
#define PROCEDURE_080822_0126 (UCHAR*)"{call dbo.atum_UpdateHappyHourEventDate(?, ?, ?)}"
#define PROCEDURE_080822_0127 (UCHAR*)"{call dbo.atum_UpdateInfluenceWarData(?,?,?)}"
#define PROCEDURE_080822_0128 (UCHAR*)"{call dbo.atum_UpdateOwnerOfConflictArea(?,?)}"
#define PROCEDURE_080822_0129 (UCHAR*)"{call dbo.atum_Update_SubLeader(?,?,?,?)}"
#define PROCEDURE_080822_0130 (UCHAR*)"{call dbo.jamboree_init_jamboree_db}"

#define PROCEDURE_080822_0131 (UCHAR*)"{call dbo.jamboree_copy_character_to_jamboree_db(?,?,?,?,?)}"
#define PROCEDURE_080822_0132 (UCHAR*)"{call dbo.atum_Load_ItemEvent}"
#define PROCEDURE_080822_0133 (UCHAR*)"{call dbo.atum_GetPCBangIPList}"

// 2012-06-21 by jhseol, 아레나 추가개발part2 - 아레나 전용 퀵슬롯
#define PROCEDURE_080822_0134 (UCHAR*)"{call dbo.atum_GetQuickSlot(?,?,?)}"		// 기존 프로시저에 파라미터 값 1게 추가
#define PROCEDURE_080822_0135 (UCHAR*)"{call dbo.atum_delete_QuickSlot(?,?,?,?)}"		// 기존 프로시저에 파라미터 값 1게 추가
#define PROCEDURE_080822_0136 (UCHAR*)"{call dbo.atum_Update_QuickSlot(?,?,?,?,?, ?,?,?,?,? ,?,?,?,?)}"		// 기존 프로시저에 파라미터 값 1게 추가
// end 2012-06-21 by jhseol, 아레나 추가개발part2 - 아레나 전용 퀵슬롯

#define PROCEDURE_080822_0137 (UCHAR*)"{call dbo.atum_UpdateStrategyPointSummonInfoByMapIndex}"
#define PROCEDURE_080822_0138 (UCHAR*)"{call dbo.atum_LoadStrategyPointSummonInfo}"
#define PROCEDURE_080822_0139 (UCHAR*)"{call dbo.atum_UpdateStrategyPointSummonInfoBySummon(?,?)}"
#define PROCEDURE_080822_0140 (UCHAR*)"{call dbo.atum_UpdateStrategyPointSummonInfoBySummonTime(?,?,?)}"

#define PROCEDURE_080822_0141 (UCHAR*)"{call dbo.atum_UpdateStrategyPointSummonInfoByNewSummonRange}"
// 2008-08-22 by cmkwon, MySQL 통합(Procedure 뽑아내기) - PROCEDURE_080822_0020로 대체 #define PROCEDURE_080822_0142 (UCHAR*)"{call atum_LoadStrategyPointSummonRangeTimeInfo}"
// 2008-08-22 by cmkwon, MySQL 통합(Procedure 뽑아내기) - PROCEDURE_080822_0022로 대체 #define PROCEDURE_080822_0143 (UCHAR*)"{call atum_LoadStrategyPointNotSummonTime}"
#define PROCEDURE_080822_0144 (UCHAR*)"{call dbo.atum_UpdateCharacterWarPoint(?,?,?,?)}"
#define PROCEDURE_080822_0145 (UCHAR*)"{call dbo.atum_UpdateCharacterArenaResult(?,?,?,?,?,?)}"
							 
#define PROCEDURE_080822_0146 (UCHAR*)"{call dbo.atum_UpdateCharacterArenaDisConnect(?,?,?)}"
#define PROCEDURE_080822_0147 (UCHAR*)"{call dbo.atum_LoadTutorialComplete(?)}"
#define PROCEDURE_080822_0148 (UCHAR*)"{call dbo.atum_InsertTutorialComplete(?,?)}"
#define PROCEDURE_080822_0149 (UCHAR*)"{call dbo.atum_SetNPCPossessionOutPost(?,?,?)}"
#define PROCEDURE_080822_0150 (UCHAR*)"{call dbo.atum_SetGuildPossessionOutPost(?,?,?,?,?)}"

#define PROCEDURE_080822_0151 (UCHAR*)"{call dbo.atum_SetOutPostNextWarTime(?,?,?,?)}"
#define PROCEDURE_080822_0152 (UCHAR*)"{call dbo.atum_InitExpediencyFund(?,?,?,?,?,?)}"
#define PROCEDURE_080822_0153 (UCHAR*)"{call dbo.atum_SetExpediencyFund(?,?,?)}"
#define PROCEDURE_080822_0154 (UCHAR*)"{call dbo.atum_SetExpediencyFundPayBack(?,?,?,?)}"
#define PROCEDURE_080822_0155 (UCHAR*)"{call dbo.atum_SetExpediencyFundRate(?,?,?,?)}"
							 
#define PROCEDURE_080822_0156 (UCHAR*)"{call dbo.atum_DeleteCityLeaderInfo(?,?,?)}"
#define PROCEDURE_080822_0157 (UCHAR*)"{call dbo.atum_RegNotice(?,?,?)}"
#define PROCEDURE_080822_0158 (UCHAR*)"{call dbo.atum_ModifyNotice(?,?,?)}"
#define PROCEDURE_080822_0159 (UCHAR*)"{call dbo.atum_UpdateSecondaryPassword(?,?)}"
#define PROCEDURE_080822_0160 (UCHAR*)"{call dbo.atum_LoadVoterList}"

// 2008-08-22 by cmkwon, MySQL 통합(Procedure 뽑아내기) - PROCEDURE_080822_0015로 대체 #define PROCEDURE_080822_0161 (UCHAR*)"{call atum_LoadLeaderCandidate}"
#define PROCEDURE_080822_0162 (UCHAR*)"{call dbo.atum_SelectLeaderCandidateInfoByRealTimeVariable(?,?)}"
#define PROCEDURE_080822_0163 (UCHAR*)"{call dbo.atum_InsertLeaderCandidate(?,?,?,?,?, ?,?,?,?)}"
#define PROCEDURE_080822_0164 (UCHAR*)"{call dbo.atum_UpdateLeaderDeleteCandidate(?)}"
#define PROCEDURE_080822_0165 (UCHAR*)"{call dbo.atum_UpdateLeaderPollCount(?,?,?)}"
							 
#define PROCEDURE_080822_0166 (UCHAR*)"{call dbo.atum_InsertVoterList(?,?,?)}"
#define PROCEDURE_080822_0167 (UCHAR*)"{call dbo.atum_GetGiveTargetCharacter(?)}"
#define PROCEDURE_080822_0168 (UCHAR*)"{call dbo.atum_UpdatePilotFace(?, ?)}"
#define PROCEDURE_080822_0169 (UCHAR*)"{call dbo.atum_InsertNotifyMsg(?,?,?,?,?)}"
#define PROCEDURE_080822_0170 (UCHAR*)"{call dbo.atum_GetNotifyMsg(?)}"

#define PROCEDURE_080822_0171 (UCHAR*)"{call dbo.atum_DeleteNotifyMsg(?)}"
#define PROCEDURE_080822_0172 (UCHAR*)"{call dbo.atum_GetGuildMark(?)}"
#define PROCEDURE_080822_0173 (UCHAR*)"{call dbo.arena_updateCharacterInfo(?,?,?)}"
#define PROCEDURE_080822_0174 (UCHAR*)"{call dbo.arena_GetCharacter(?, ?)}"
//#define PROCEDURE_080822_0175 (UCHAR*)"{call dbo.arena_CopyDBInfo(?,?,?)}"
#define PROCEDURE_080822_0175 (UCHAR*)"{call dbo.arena_CopyDBInfo(?,?,?,?,?)}"	// 2012-06-08 by jhseol, 아레나 추가개발part2 - 케릭터 : 타입 복사 파람 추가
							 
// 2008-08-22 by cmkwon, MySQL 통합(Procedure 뽑아내기) - PROCEDURE_080822_0174로 대체 #define PROCEDURE_080822_0176 (UCHAR*)"{call arena_GetCharacter(?, ?)}"
#define PROCEDURE_080822_0177 (UCHAR*)"{call dbo.atum_log_Get_MSWar}"
#define PROCEDURE_080822_0178 (UCHAR*)"{call dbo.atum_log_Get_SPWar}"
#define PROCEDURE_080822_0179 (UCHAR*)"{call dbo.atum_Update_MSWarOptionType(?,?)}"
// 2008-08-22 by cmkwon, MySQL 통합(Procedure 뽑아내기) - PROCEDURE_080822_0041로 대체 #define PROCEDURE_080822_0180 (UCHAR*)"{call atum_UpdateServerGroupInfo(?,?,?,?)}"

#define PROCEDURE_080822_0181 (UCHAR*)"{call dbo.atum_GetNumOfCharacter(?)}"
#define PROCEDURE_080822_0182 (UCHAR*)"{call dbo.atum_GetUserInfo(?)}"
#define PROCEDURE_080822_0183 (UCHAR*)"{call dbo.atum_Job_DeleteCharacter}"
#define PROCEDURE_080822_0184 (UCHAR*)"{call dbo.atum_Guild_Update_TotalFameRankByJob}"
#define PROCEDURE_080822_0185 (UCHAR*)"{call dbo.atum_Guild_Update_MonthlyFameRankByJob}"
							 
#define PROCEDURE_080822_0186 (UCHAR*)"{call dbo.atum_CityWar_LoadingOccupyInfo(?)}"
#define PROCEDURE_080822_0187 (UCHAR*)"{call dbo.atum_CityWar_CheckGuildAndGuildMaster(?, ?)}"
#define PROCEDURE_080822_0188 (UCHAR*)"{call dbo.atum_CityWar_LoaingCityWarGuildList(?, ?)}"
#define PROCEDURE_080822_0189 (UCHAR*)"{call dbo.atum_CityWar_StoreOccupyInfo(?, ?, ?, ?, ?, ?, ?, ?, ?, ?)}"
#define PROCEDURE_080822_0190 (UCHAR*)"{call dbo.atum_CityWar_StoreTexPercent(?, ?)}"

#define PROCEDURE_080822_0191 (UCHAR*)"{call dbo.atum_CityWar_StoreWarTime(?, ?, ?)}"
#define PROCEDURE_080822_0192 (UCHAR*)"{call dbo.atum_CityWar_StoreSumOfTex(?, ?)}"
#define PROCEDURE_080822_0193 (UCHAR*)"{call dbo.atum_CityWar_StoreBriefing(?, ?)}"
#define PROCEDURE_080822_0194 (UCHAR*)"{call dbo.atum_Coupon_LoadCouponByNumber(?)}"
#define PROCEDURE_080822_0195 (UCHAR*)"{call dbo.atum_Coupon_UseCoupon(?, ?, ?)}"
							 
#define PROCEDURE_080822_0196 (UCHAR*)"{call dbo.Billing_Get_Point(?)}"
#define PROCEDURE_080822_0197 (UCHAR*)"{call dbo.Billing_Buy_Item(?,?,?,?,?, ?,?)}"
#define PROCEDURE_080822_0198 (UCHAR*)"{call dbo.Billing_Gift_Item(?,?,?,?,?, ?,?,?,?,?)}"
#define PROCEDURE_080822_0199 (UCHAR*)"{call dbo.SP_UserCashSearch(?,?,?,?)}"
#define PROCEDURE_080822_0200 (UCHAR*)"{call dbo.SP_GameCashUpdate(?,?,?,?,?, ?,?,?,?,?, ?)}"

///////////////////////////////////////////////////////////////////////////////
// 2008-08-22 by cmkwon, MySQL 통합(Procedure 뽑아내기) - 201
#define PROCEDURE_080822_0201 (UCHAR*)"{call dbo.SP_GameCashUpdateGift(?,?,?,?,?, ?,?,?,?,?, ?,?,?,?)}"
#define PROCEDURE_080822_0202 (UCHAR*)"{call dbo.atum_GetBuildingNPC(?)}"
#define PROCEDURE_080822_0203 (UCHAR*)"{call dbo.atum_GetAllBuildingIndex(?)}"
#define PROCEDURE_080822_0204 (UCHAR*)"{call dbo.atum_GetShopItems(?)}"
#define PROCEDURE_080822_0205 (UCHAR*)"{call dbo.atum_loadWarpableUserList(?)}"
						   
#define PROCEDURE_080822_0206 (UCHAR*)"{call dbo.atum_Cash_ChangeCharacterName(?, ?, ?)}"
#define PROCEDURE_080822_0207 (UCHAR*)"{call dbo.atum_AuthChatLogin(?, ?)}"
#define PROCEDURE_080822_0208 (UCHAR*)"{call dbo.atum_GetChatCharacterInfoByName(?, ?)}"
#define PROCEDURE_080822_0209 (UCHAR*)"{call dbo.atum_SaveLastPartyID(?, ?)}"
#define PROCEDURE_080822_0210 (UCHAR*)"{call dbo.atum_CreateGuild(?, ?, ?)}"
						   
#define PROCEDURE_080822_0211 (UCHAR*)"{call dbo.atum_Get_Guild_Master_CashStoreInfo(?) }"
#define PROCEDURE_080822_0212 (UCHAR*)"{call dbo.atum_InsertGuildMember(?, ?, ?)}"
#define PROCEDURE_080822_0213 (UCHAR*)"{call dbo.atum_DeleteSelfIntroduction(?)}"
// 2008-08-22 by cmkwon, MySQL 통합(Procedure 뽑아내기) - PROCEDURE_080822_0212로 대체 #define PROCEDURE_080822_0214 (UCHAR*)"{call atum_InsertGuildMember(?, ?, ?)}"
// 2008-08-22 by cmkwon, MySQL 통합(Procedure 뽑아내기) - PROCEDURE_080822_0213로 대체 #define PROCEDURE_080822_0215 (UCHAR*)"{call dbo.atum_DeleteSelfIntroduction(?)}"
								
// 2008-08-22 by cmkwon, MySQL 통합(Procedure 뽑아내기) - PROCEDURE_080822_0212로 대체 #define PROCEDURE_080822_0216 (UCHAR*)"{call dbo.atum_InsertGuildMember(?, ?, ?)}"
// 2008-08-22 by cmkwon, MySQL 통합(Procedure 뽑아내기) - PROCEDURE_080822_0213로 대체 #define PROCEDURE_080822_0217 (UCHAR*)"{call dbo.atum_DeleteSelfIntroduction(?)}"
#define PROCEDURE_080822_0218 (UCHAR*)"{call dbo.atum_DeleteGuildMember(?)}"
// 2008-08-22 by cmkwon, MySQL 통합(Procedure 뽑아내기) - PROCEDURE_080822_0218로 대체 #define PROCEDURE_080822_0219 (UCHAR*)"{call atum_DeleteGuildMember(?)}"
#define PROCEDURE_080822_0220 (UCHAR*)"{call dbo.atum_SetDismemberDate(?)}"
						   
#define PROCEDURE_080822_0221 (UCHAR*)"{call dbo.atum_Guild_UpdateMemberCapacity(?, ?, ?)}"
#define PROCEDURE_080822_0222 (UCHAR*)"{call dbo.atum_CancelDismember(?)}"
#define PROCEDURE_080822_0223 (UCHAR*)"{call dbo.atum_ChangeGuildName(?, ?)}"
#define PROCEDURE_080822_0224 (UCHAR*)"{call dbo.atum_SetGuildMark(?, ?, ?, ?)}"
#define PROCEDURE_080822_0225 (UCHAR*)"{call dbo.atum_ChangeGuildRank(?, ?)}"
								
#define PROCEDURE_080822_0226 (UCHAR*)"{call dbo.atum_DismemberGuild(?)}"
#define PROCEDURE_080822_0227 (UCHAR*)"{call dbo.atum_GetGuildInfoByUniqueNumber(?) }"
// 2008-08-22 by cmkwon, MySQL 통합(Procedure 뽑아내기) - PROCEDURE_080822_0211로 대체 #define PROCEDURE_080822_0228 (UCHAR*)"{call dbo.atum_Get_Guild_Master_CashStoreInfo(?) }"
#define PROCEDURE_080822_0229 (UCHAR*)"{call dbo.atum_GetAllGuildMembers(?) }"
#define PROCEDURE_080822_0230 (UCHAR*)"{call dbo.atum_GetGuildRank(?, ?) }"
						   
#define PROCEDURE_080822_0231 (UCHAR*)"{call dbo.atum_DeleteGuildIntroduction_By_PassedRangeDay(?)}"
#define PROCEDURE_080822_0232 (UCHAR*)"{call dbo.atum_GetGuildIntroduction(?) }"
#define PROCEDURE_080822_0233 (UCHAR*)"{call dbo.atum_SaveGuildWarPoint(?, ?, ?)}"
#define PROCEDURE_080822_0234 (UCHAR*)"{call dbo.atum_GuildAddGuildFame(?,?,?) }"
// 2008-08-22 by cmkwon, MySQL 통합(Procedure 뽑아내기) - PROCEDURE_080822_0211로 대체 #define PROCEDURE_080822_0235 (UCHAR*)"{call dbo.atum_Get_Guild_Master_CashStoreInfo(?) }"
								
#define PROCEDURE_080822_0236 (UCHAR*)"{call dbo.atum_UpdateGuildNotice(?,?) }"
#define PROCEDURE_080822_0237 (UCHAR*)"{call dbo.atum_GetGuildApplicant(?) }"
// 2008-08-22 by cmkwon, MySQL 통합(Procedure 뽑아내기) - PROCEDURE_080822_0232로 대체 #define PROCEDURE_080822_0238 (UCHAR*)"{call dbo.atum_GetGuildIntroduction(?) }"
#define PROCEDURE_080822_0239 (UCHAR*)"{call dbo.atum_DeleteGuildIntroduction(?) }"
#define PROCEDURE_080822_0240 (UCHAR*)"{call dbo.atum_GetSelfIntroduction(?) }"
						   
// 2008-08-22 by cmkwon, MySQL 통합(Procedure 뽑아내기) - PROCEDURE_080822_0231로 대체 #define PROCEDURE_080822_0241 (UCHAR*)"{call dbo.atum_DeleteGuildIntroduction_By_PassedRangeDay(?)}"
#define PROCEDURE_080822_0242 (UCHAR*)"{call dbo.atum_SearchGuildIntroduction(?) }"
#define PROCEDURE_080822_0243 (UCHAR*)"{call dbo.atum_UpdateGuildIntroduction(?, ?, ?)}"
#define PROCEDURE_080822_0244 (UCHAR*)"{call dbo.atum_FriendInsertFriend(?,?,?,?)}"
#define PROCEDURE_080822_0245 (UCHAR*)"{call dbo.atum_FriendDeleteFriend(?, ?, ?)}"
								
#define PROCEDURE_080822_0246 (UCHAR*)"{call dbo.atum_FriendLoadFriends(?)}"

#define PROCEDURE_080822_0247 (UCHAR*)"{call dbo.atum_log_insert_connection(?,?,?,?,?,?,?)}"

#define PROCEDURE_080822_0248 (UCHAR*)"{call dbo.atum_log_insert_user_level_stat(?, ?, ?, ?, ?, ?, ?, ?, ?)}"
#define PROCEDURE_080822_0249 (UCHAR*)"{call dbo.atum_log_insert_exp(?, ?, ?, ?, ?, ?, ?, ?)}"
// 2008-08-22 by cmkwon, MySQL 통합(Procedure 뽑아내기) - PROCEDURE_080822_0028로 대체 #define PROCEDURE_080822_0250 (UCHAR*)"{call atum_log_insert_user_game_start_end(?,?,?,?,?, ?,?,?,?,?, ?,?,?)}"
						   
#define PROCEDURE_080822_0251 (UCHAR*)"{call dbo.atum_log_insert_user_quest_pk_dead(?, ?, ?, ?, ?, ?, ?)}"
#define PROCEDURE_080822_0252 (UCHAR*)"{call dbo.atum_log_insert_user_warp(?, ?, ?, ?, ?, ?, ?, ?, ?, ?)}"
#define PROCEDURE_080822_0253 (UCHAR*)"{call dbo.atum_log_insert_user_speed_hack(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)}"
#define PROCEDURE_080822_0254 (UCHAR*)"{call dbo.atum_log_insert_user_memory_hack(?,?,?,?,?, ?,?,?,?,?, ?,?,?)}"
#define PROCEDURE_080822_0255 (UCHAR*)"{call dbo.atum_log_insert_item_charac_use(?, ?, ?, ?, ?, ?, ?)}"
								
#define PROCEDURE_080822_0256 (UCHAR*)"{call dbo.atum_log_insert_item_get_throw(?, ?, ?, ?, ?, ?, ?)}"
// 2008-08-22 by cmkwon, MySQL 통합(Procedure 뽑아내기) - PROCEDURE_080822_0029로 대체 #define PROCEDURE_080822_0257 (UCHAR*)"{call atum_log_insert_item_trade(?, ?, ?, ?, ?, ?, ?, ?, ?)}"
#define PROCEDURE_080822_0258 (UCHAR*)"{call dbo.atum_log_Insert_GuildStore(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)}"
#define PROCEDURE_080822_0259 (UCHAR*)"{call dbo.atum_log_insert_item_buy_sell(?, ?, ?, ?, ?, ?, ?, ?)}"
#define PROCEDURE_080822_0260 (UCHAR*)"{call dbo.atum_log_insert_server_map(?, ?, ?, ?, ?)}"
						   
#define PROCEDURE_080822_0261 (UCHAR*)"{call dbo.atum_log_insert_monster_boss(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)}"
#define PROCEDURE_080822_0262 (UCHAR*)"{call dbo.PB_INIT}"
#define PROCEDURE_080822_0263 (UCHAR*)"{call dbo.atum_log_Insert_MSWar(?,?,?,?,? ,?)}"
#define PROCEDURE_080822_0264 (UCHAR*)"{call dbo.atum_log_Insert_SPWar(?,?,?,?,?)}"
#define PROCEDURE_080822_0265 (UCHAR*)"{call dbo.atum_PreServerGetAccountInfo(?)}"
#define PROCEDURE_080822_0265_BETA (UCHAR*)"{call dbo.atum_BetaPreServerGetAccountInfo(?)}"
								
#define PROCEDURE_080822_0266 (UCHAR*)"{call dbo.atum_PreServerUpdateLastLoginDate(?)}"
#define PROCEDURE_080822_0267 (UCHAR*)"{call dbo.atum_Insert_BlockedAccounts(?,?,?,?,?, ?,?)}"
#define PROCEDURE_080822_0268 (UCHAR*)"{call dbo.atum_Delete_BlockedAccounts(?)}"

#define PROCEDURE_080822_0269 (UCHAR*)"{call dbo.atum_log_insert_total_user(?, ?, ?)}"			// 2010-11 by dhjin, 아라리오 채널링 로그인.

#define PROCEDURE_080822_0270 (UCHAR*)"{call dbo.atum_log_insert_block_unblock(?,?,?,?,?, ?,?,?)}"
						   
#define PROCEDURE_080822_0271 (UCHAR*)"{call dbo.atum_PreServerExtAuthLogin(?, ?, ?, ?, ?)}"
#define PROCEDURE_080822_0272 (UCHAR*)"{call dbo.atum_GetLastGamePlayData(?)}"
#define PROCEDURE_080822_0273 (UCHAR*)"{call dbo.G_AUTHENTICATE(?, ?, ?, ?)}"
#define PROCEDURE_080822_0274 (UCHAR*)"{call dbo.atum_ExternalAuthentication2(?, ?, ?, ?)}"
#define PROCEDURE_080822_0275 (UCHAR*)"{call dbo.atum_ExternalAuthenticationChina(?, ?)}"
								
#define PROCEDURE_080822_0276 (UCHAR*)"{call dbo.atum_Load_BlockAccounts}"
#define PROCEDURE_080822_0277 (UCHAR*)"{call dbo.atum_Send_Letter(?,?,?,?,? ,?)}"
#define PROCEDURE_080822_0278 (UCHAR*)"{call dbo.atum_Send_AllLetter_ByLeader(?,?,?,?,? ,?)}"
// 2008-10-13 by dhjin, 여단장 위임 가능 레벨 체크 추가 - 아래와 같이 수정
//#define PROCEDURE_080822_0279 (UCHAR*)"{call dbo.atum_UpdateGuildCommander(?,?,?) }"
#define PROCEDURE_080822_0279 (UCHAR*)"{call dbo.atum_UpdateGuildCommander(?,?,?,?) }"
#define PROCEDURE_080822_0280 (UCHAR*)"{call dbo.atum_UpdateSelfIntroduction(?, ?, ?)}"

#define	PROCEDURE_080822_0282 (UCHAR*)"{call dbo.Billing_Add_Points(?,?)}"

#define PROCEDURE_080822_0281 (UCHAR*)"{call Usp_Do_Login(?,?)}"
#define PROCEDURE_080822_0283 (UCHAR*)"{call dbo.atum_Load_BlockedMACs}"
#define PROCEDURE_080822_0284 (UCHAR*)"{call dbo.atum_Insert_BlockedMAC}"
#define PROCEDURE_080822_0285 (UCHAR*)"{call dbo.atum_Delete_BlockedMAC}"

///////////////////////////////////////////////////////////////////////////////
// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
#define PROCEDURE_080827_0001 (UCHAR*)"{call dbo.atum_PROCEDURE_080827_0001(?)}"
#define PROCEDURE_080827_0002 (UCHAR*)"{call dbo.atum_PROCEDURE_080827_0002(?,?,?,?)}"
#define PROCEDURE_080827_0003 (UCHAR*)"{call dbo.atum_PROCEDURE_080827_0003}"
#define PROCEDURE_080827_0004 (UCHAR*)"{call dbo.atum_PROCEDURE_080827_0004(?,?)}"
#define PROCEDURE_080827_0005 (UCHAR*)"{call dbo.atum_PROCEDURE_080827_0005(?,?)}"

#define PROCEDURE_080827_0006 (UCHAR*)"{call dbo.atum_PROCEDURE_080827_0006(?,?)}"
#define PROCEDURE_080827_0007 (UCHAR*)"{call dbo.atum_PROCEDURE_080827_0007}"
#define PROCEDURE_080827_0008 (UCHAR*)"{call dbo.atum_PROCEDURE_080827_0008}"
#define PROCEDURE_080827_0009 (UCHAR*)"{call dbo.atum_PROCEDURE_080827_0009}"
#define PROCEDURE_080827_0010 (UCHAR*)"{call dbo.atum_PROCEDURE_080827_0010(?,?)}"

#define PROCEDURE_080827_0014 (UCHAR*)"{call dbo.atum_PROCEDURE_080827_0014(?)}"
#define PROCEDURE_080827_0015 (UCHAR*)"{call dbo.atum_PROCEDURE_080827_0015(?,?,?,?,?)}"

#define PROCEDURE_080827_0017 (UCHAR*)"{call dbo.atum_PROCEDURE_080827_0017(?,?)}"
#define PROCEDURE_080827_0019 (UCHAR*)"{call dbo.atum_PROCEDURE_080827_0019(?)}"
#define PROCEDURE_080827_0020 (UCHAR*)"{call dbo.atum_PROCEDURE_080827_0020(?,?)}"

#define PROCEDURE_080827_0021 (UCHAR*)"{call dbo.atum_PROCEDURE_080827_0021(?)}"
#define PROCEDURE_080827_0022 (UCHAR*)"{call dbo.atum_PROCEDURE_080827_0022(?)}"
#define PROCEDURE_080827_0023 (UCHAR*)"{call dbo.atum_PROCEDURE_080827_0023(?,?,?)}"
#define PROCEDURE_080827_0024 (UCHAR*)"{call dbo.atum_PROCEDURE_080827_0024(?,?)}"
#define PROCEDURE_080827_0025 (UCHAR*)"{call dbo.atum_PROCEDURE_080827_0025}"

#define PROCEDURE_080827_0026 (UCHAR*)"{call dbo.atum_PROCEDURE_080827_0026(?,?,?,?,?, ?,?,?,?,?, ?,?,?)}"
#define PROCEDURE_080827_0027 (UCHAR*)"{call dbo.atum_PROCEDURE_080827_0027(?,?,?,?,?, ?,?,?,?,?, ?,?,?,?)}"
#define PROCEDURE_080827_0028 (UCHAR*)"{call dbo.atum_PROCEDURE_080827_0028(?,?,?,?)}"
#define PROCEDURE_080827_0029 (UCHAR*)"{call dbo.atum_PROCEDURE_080827_0029}"
#define PROCEDURE_080827_0030 (UCHAR*)"{call dbo.atum_PROCEDURE_080827_0030(?,?,?)}"

#define PROCEDURE_080827_0031 (UCHAR*)"{call dbo.atum_PROCEDURE_080827_0031(?)}"
#define PROCEDURE_080827_0032 (UCHAR*)"{call dbo.atum_PROCEDURE_080827_0032(?,?)}"
#define PROCEDURE_080827_0033 (UCHAR*)"{call dbo.atum_PROCEDURE_080827_0033(?)}"
#define PROCEDURE_080827_0034 (UCHAR*)"{call dbo.atum_PROCEDURE_080827_0034(?)}"

// 2009-08-26 by cmkwon, 그래픽 리소스 변경 시스템 구현 - ShapeItemNum 필드 추가, 프로시저 인자 추가
#define PROCEDURE_080827_0035 (UCHAR*)"{call dbo.atum_PROCEDURE_080827_0035(?,?,?,?,?, ?)}"

#define PROCEDURE_080827_0036 (UCHAR*)"{call dbo.atum_PROCEDURE_080827_0036(?,?,?)}"
#define PROCEDURE_080827_0037 (UCHAR*)"{call dbo.atum_PROCEDURE_080827_0037(?,?,?,?)}"

#define PROCEDURE_080827_0041 (UCHAR*)"{call dbo.atum_PROCEDURE_080827_0041(?)}"
#define PROCEDURE_080827_0042 (UCHAR*)"{call dbo.atum_PROCEDURE_080827_0042(?)}"
#define PROCEDURE_080827_0043 (UCHAR*)"{call dbo.atum_PROCEDURE_080827_0043(?)}"
#define PROCEDURE_080827_0044 (UCHAR*)"{call dbo.atum_PROCEDURE_080827_0044(?)}"
#define PROCEDURE_080827_0045 (UCHAR*)"{call dbo.atum_PROCEDURE_080827_0045(?,?,?)}"

#define PROCEDURE_080827_0046 (UCHAR*)"{call dbo.atum_PROCEDURE_080827_0046(?)}"
#define PROCEDURE_080827_0047 (UCHAR*)"{call dbo.atum_PROCEDURE_080827_0047(?)}"
#define PROCEDURE_080827_0048 (UCHAR*)"{call dbo.atum_PROCEDURE_080827_0048(?,?,?,?,?, ?,?,?,?,?, ?,?,?,?,?, ?,?,?,?,?, ?,?,?,?,?, ?,?,?,?,?, ?,?,?,?)}"
#define PROCEDURE_080827_0049 (UCHAR*)"{call dbo.atum_PROCEDURE_080827_0049(?)}"
#define PROCEDURE_080827_0050 (UCHAR*)"{call dbo.atum_PROCEDURE_080827_0050(?,?)}"

#define PROCEDURE_080827_0051 (UCHAR*)"{call dbo.atum_PROCEDURE_080827_0051(?,?,?,?,?, ?,?,?,?,?, ?)}"
#define PROCEDURE_080827_0052 (UCHAR*)"{call dbo.atum_PROCEDURE_080827_0052(?,?,?,?,?, ?,?,?,?,?, ?,?)}"
#define PROCEDURE_080827_0053 (UCHAR*)"{call dbo.atum_PROCEDURE_080827_0053(?)}"
#define PROCEDURE_080827_0054 (UCHAR*)"{call dbo.atum_PROCEDURE_080827_0054(?)}"
#define PROCEDURE_080827_0055 (UCHAR*)"{call dbo.atum_PROCEDURE_080827_0055(?,?)}"

#define PROCEDURE_080827_0056 (UCHAR*)"{call dbo.atum_PROCEDURE_080827_0056}"
#define PROCEDURE_080827_0057 (UCHAR*)"{call dbo.atum_PROCEDURE_080827_0057}"
#define PROCEDURE_080827_0058 (UCHAR*)"{call dbo.atum_PROCEDURE_080827_0058}"
#define PROCEDURE_080827_0059 (UCHAR*)"{call dbo.atum_PROCEDURE_080827_0059}"
#define PROCEDURE_080827_0060 (UCHAR*)"{call dbo.atum_PROCEDURE_080827_0060}"

#define PROCEDURE_080827_0061 (UCHAR*)"{call dbo.atum_PROCEDURE_080827_0061}"
#define PROCEDURE_080827_0062 (UCHAR*)"{call dbo.atum_PROCEDURE_080827_0062}"
#define PROCEDURE_080827_0063 (UCHAR*)"{call dbo.atum_PROCEDURE_080827_0063}"
#define PROCEDURE_080827_0064 (UCHAR*)"{call dbo.atum_PROCEDURE_080827_0064}"
#define PROCEDURE_080827_0065 (UCHAR*)"{call dbo.atum_PROCEDURE_080827_0065}"

//#define PROCEDURE_080827_0066 (UCHAR*)"{call dbo.atum_PROCEDURE_080827_0066}"
#define PROCEDURE_080827_0067 (UCHAR*)"{call dbo.atum_PROCEDURE_080827_0067(?,?,?,?,?, ?,?,?,?,?, ?,?,?)}"
#define PROCEDURE_080827_0068 (UCHAR*)"{call dbo.atum_PROCEDURE_080827_0068}"
#define PROCEDURE_080827_0069 (UCHAR*)"{call dbo.atum_PROCEDURE_080827_0069}"
#define PROCEDURE_080827_0070 (UCHAR*)"{call dbo.atum_PROCEDURE_080827_0070}"

#define PROCEDURE_080827_0071 (UCHAR*)"{call dbo.atum_PROCEDURE_080827_0071(?)}"
#define PROCEDURE_080827_0072 (UCHAR*)"{call dbo.atum_PROCEDURE_080827_0072}"
#define PROCEDURE_080827_0073 (UCHAR*)"{call dbo.atum_PROCEDURE_080827_0073}"
#define PROCEDURE_080827_0074 (UCHAR*)"{call dbo.atum_PROCEDURE_080827_0074(?,?,?)}"
#define PROCEDURE_080827_0075 (UCHAR*)"{call dbo.atum_PROCEDURE_080827_0075}"

#define PROCEDURE_080827_0076 (UCHAR*)"{call dbo.atum_PROCEDURE_080827_0076(?,?,?,?)}"
#define PROCEDURE_080827_0077 (UCHAR*)"{call dbo.atum_PROCEDURE_080827_0077(?)}"
#define PROCEDURE_080827_0078 (UCHAR*)"{call dbo.atum_PROCEDURE_080827_0078(?)}"
#define PROCEDURE_080827_0079 (UCHAR*)"{call dbo.atum_PROCEDURE_080827_0079(?)}"
#define PROCEDURE_080827_0080 (UCHAR*)"{call dbo.atum_PROCEDURE_080827_0080(?)}"

#define PROCEDURE_080827_0081 (UCHAR*)"{call dbo.atum_PROCEDURE_080827_0081(?)}"
#define PROCEDURE_080827_0082 (UCHAR*)"{call dbo.atum_PROCEDURE_080827_0082(?)}"
#define PROCEDURE_080827_0083 (UCHAR*)"{call dbo.atum_PROCEDURE_080827_0083(?)}"
#define PROCEDURE_080827_0084 (UCHAR*)"{call dbo.atum_PROCEDURE_080827_0084(?)}"

#define PROCEDURE_080827_0087 (UCHAR*)"{call dbo.atum_PROCEDURE_080827_0087}"

#define PROCEDURE_080827_0120 (UCHAR*)"{call dbo.atum_PROCEDURE_080827_0120(?,?,?)}"

#define PROCEDURE_080827_0122 (UCHAR*)"{call dbo.atum_PROCEDURE_080827_0122(?)}"
#define PROCEDURE_080827_0125 (UCHAR*)"{call dbo.atum_PROCEDURE_080827_0125}"

#define PROCEDURE_080827_0129 (UCHAR*)"{call dbo.atum_PROCEDURE_080827_0129}"

#define PROCEDURE_080827_0133 (UCHAR*)"{call dbo.atum_PROCEDURE_080827_0133}"
#define PROCEDURE_080827_0134 (UCHAR*)"{call dbo.atum_PROCEDURE_080827_0134(?,?,?)}"
#define PROCEDURE_080827_0135 (UCHAR*)"{call dbo.atum_PROCEDURE_080827_0135(?,?)}"

#define PROCEDURE_080827_0136 (UCHAR*)"{call dbo.atum_PROCEDURE_080827_0136(?,?)}"
#define PROCEDURE_080827_0137 (UCHAR*)"{call dbo.atum_PROCEDURE_080827_0137(?)}"
#define PROCEDURE_080827_0138 (UCHAR*)"{call dbo.atum_PROCEDURE_080827_0138(?)}"

#define PROCEDURE_080827_0141 (UCHAR*)"{call dbo.atum_PROCEDURE_080827_0141}"
#define PROCEDURE_080827_0142 (UCHAR*)"{call dbo.atum_PROCEDURE_080827_0142}"
#define PROCEDURE_080827_0143 (UCHAR*)"{call dbo.atum_PROCEDURE_080827_0143}"
#define PROCEDURE_080827_0144 (UCHAR*)"{call dbo.atum_PROCEDURE_080827_0144}"
#define PROCEDURE_080827_0145 (UCHAR*)"{call dbo.atum_PROCEDURE_080827_0145}"

#define PROCEDURE_080827_0155 (UCHAR*)"{call dbo.atum_PROCEDURE_080827_0155}"
#define PROCEDURE_080827_0160 (UCHAR*)"{call dbo.atum_PROCEDURE_080827_0160}"

#define PROCEDURE_080827_0165 (UCHAR*)"{call dbo.atum_PROCEDURE_080827_0165}"
#define PROCEDURE_080827_0168 (UCHAR*)"{call dbo.atum_PROCEDURE_080827_0168}"

#define PROCEDURE_080827_0171 (UCHAR*)"{call dbo.atum_PROCEDURE_080827_0171}"
#define PROCEDURE_080827_0175 (UCHAR*)"{call dbo.atum_PROCEDURE_080827_0175}"

#define PROCEDURE_080827_0178 (UCHAR*)"{call dbo.atum_PROCEDURE_080827_0178}"
#define PROCEDURE_080827_0180 (UCHAR*)"{call dbo.atum_PROCEDURE_080827_0180(?,?)}"

#define PROCEDURE_080827_0183 (UCHAR*)"{call dbo.atum_PROCEDURE_080827_0183(?)}"

#define PROCEDURE_080827_0186 (UCHAR*)"{call dbo.atum_PROCEDURE_080827_0186(?)}"
#define PROCEDURE_080827_0187 (UCHAR*)"{call dbo.atum_PROCEDURE_080827_0187(?)}"
#define PROCEDURE_080827_0188 (UCHAR*)"{call dbo.atum_PROCEDURE_080827_0188(?)}"

#define PROCEDURE_080827_0195 (UCHAR*)"{call dbo.atum_PROCEDURE_080827_0195}"
#define PROCEDURE_080827_0198 (UCHAR*)"{call dbo.atum_PROCEDURE_080827_0198(?,?)}"
#define PROCEDURE_080827_0199 (UCHAR*)"{call dbo.atum_PROCEDURE_080827_0199(?,?)}"
#define PROCEDURE_080827_0200 (UCHAR*)"{call dbo.atum_PROCEDURE_080827_0200(?,?)}"


#define PROCEDURE_080827_0201 (UCHAR*)"{call dbo.atum_PROCEDURE_080827_0201}"
#define PROCEDURE_080827_0202 (UCHAR*)"{call dbo.atum_PROCEDURE_080827_0202(?)}"
#define PROCEDURE_080827_0203 (UCHAR*)"{call dbo.atum_PROCEDURE_080827_0203(?,?)}"

#define PROCEDURE_080827_0204 (UCHAR*)"{call dbo.atum_PROCEDURE_080827_0204}"
#define PROCEDURE_080827_0205 (UCHAR*)"{call dbo.atum_PROCEDURE_080827_0205(?)}"
#define PROCEDURE_080827_0206 (UCHAR*)"{call dbo.atum_PROCEDURE_080827_0206(?,?,?)}"

#define PROCEDURE_080827_0207 (UCHAR*)"{call dbo.atum_PROCEDURE_080827_0207}"
#define PROCEDURE_080827_0208 (UCHAR*)"{call dbo.atum_PROCEDURE_080827_0208(?)}"
#define PROCEDURE_080827_0209 (UCHAR*)"{call dbo.atum_PROCEDURE_080827_0209(?,?,?)}"

#define PROCEDURE_080827_0210 (UCHAR*)"{call dbo.atum_PROCEDURE_080827_0210}"
#define PROCEDURE_080827_0211 (UCHAR*)"{call dbo.atum_PROCEDURE_080827_0211(?)}"
#define PROCEDURE_080827_0212 (UCHAR*)"{call dbo.atum_PROCEDURE_080827_0212(?,?,?)}"

#define PROCEDURE_080827_0213 (UCHAR*)"{call dbo.atum_PROCEDURE_080827_0213}"
#define PROCEDURE_080827_0214 (UCHAR*)"{call dbo.atum_PROCEDURE_080827_0214(?)}"
#define PROCEDURE_080827_0215 (UCHAR*)"{call dbo.atum_PROCEDURE_080827_0215(?,?,?)}"

#define PROCEDURE_080827_0216 (UCHAR*)"{call dbo.atum_PROCEDURE_080827_0216(?,?,?,?)}"
#define PROCEDURE_080827_0217 (UCHAR*)"{call dbo.atum_PROCEDURE_080827_0217(?,?,?,?)}"

#define PROCEDURE_080827_0218 (UCHAR*)"{call dbo.atum_PROCEDURE_080827_0218}"
#define PROCEDURE_080827_0219 (UCHAR*)"{call dbo.atum_PROCEDURE_080827_0219(?)}"
#define PROCEDURE_080827_0220 (UCHAR*)"{call dbo.atum_PROCEDURE_080827_0220}"

#define PROCEDURE_080827_0221 (UCHAR*)"{call dbo.atum_PROCEDURE_080827_0221(?)}"
#define PROCEDURE_080827_0222 (UCHAR*)"{call dbo.atum_PROCEDURE_080827_0222(?,?)}"

#define PROCEDURE_080827_0223 (UCHAR*)"{call dbo.atum_PROCEDURE_080827_0223(?,?)}"

#define PROCEDURE_080827_0224 (UCHAR*)"{call dbo.atum_PROCEDURE_080827_0224}"
#define PROCEDURE_080827_0225 (UCHAR*)"{call dbo.atum_PROCEDURE_080827_0225(?)}"
#define PROCEDURE_080827_0226 (UCHAR*)"{call dbo.atum_PROCEDURE_080827_0226}"

#define PROCEDURE_080827_0227 (UCHAR*)"{call dbo.atum_PROCEDURE_080827_0227}"
#define PROCEDURE_080827_0228 (UCHAR*)"{call dbo.atum_PROCEDURE_080827_0228(?)}"
#define PROCEDURE_080827_0229 (UCHAR*)"{call dbo.atum_PROCEDURE_080827_0229}"

#define PROCEDURE_080827_0236 (UCHAR*)"{call dbo.atum_PROCEDURE_080827_0236}"
#define PROCEDURE_080827_0237 (UCHAR*)"{call dbo.atum_PROCEDURE_080827_0237(?)}"
#define PROCEDURE_080827_0238 (UCHAR*)"{call dbo.atum_PROCEDURE_080827_0238(?,?)}"
#define PROCEDURE_080827_0239 (UCHAR*)"{call dbo.atum_PROCEDURE_080827_0239(?,?)}"

#define PROCEDURE_080827_0240 (UCHAR*)"{call dbo.atum_PROCEDURE_080827_0240(?)}"
#define PROCEDURE_080827_0241 (UCHAR*)"{call dbo.atum_PROCEDURE_080827_0241(?)}"
#define PROCEDURE_080827_0242 (UCHAR*)"{call dbo.atum_PROCEDURE_080827_0242(?)}"
#define PROCEDURE_080827_0243 (UCHAR*)"{call dbo.atum_PROCEDURE_080827_0243}"

#define PROCEDURE_080827_0244 (UCHAR*)"{call dbo.atum_PROCEDURE_080827_0244}"

#define PROCEDURE_080827_0246 (UCHAR*)"{call dbo.atum_PROCEDURE_080827_0246}"
#define PROCEDURE_080827_0247 (UCHAR*)"{call dbo.atum_PROCEDURE_080827_0247}"

#define PROCEDURE_080827_0248 (UCHAR*)"{call dbo.atum_PROCEDURE_080827_0248(?)}"

#define PROCEDURE_080827_0249 (UCHAR*)"{call dbo.atum_PROCEDURE_080827_0249(?)}"
#define PROCEDURE_080827_0250 (UCHAR*)"{call dbo.atum_PROCEDURE_080827_0250(?)}"
#define PROCEDURE_080827_0251 (UCHAR*)"{call dbo.atum_PROCEDURE_080827_0251(?)}"
#define PROCEDURE_080827_0252 (UCHAR*)"{call dbo.atum_PROCEDURE_080827_0252(?)}"
#define PROCEDURE_080827_0253 (UCHAR*)"{call dbo.atum_PROCEDURE_080827_0253(?)}"
#define PROCEDURE_080827_0254 (UCHAR*)"{call dbo.atum_PROCEDURE_080827_0254(?)}"
#define PROCEDURE_080827_0255 (UCHAR*)"{call dbo.atum_PROCEDURE_080827_0255(?)}"
#define PROCEDURE_080827_0256 (UCHAR*)"{call dbo.atum_PROCEDURE_080827_0256(?,?,?)}"

#define PROCEDURE_080827_0257 (UCHAR*)"{call dbo.atum_PROCEDURE_080827_0257}"

#define PROCEDURE_080827_0258 (UCHAR*)"{call dbo.atum_PROCEDURE_080827_0258(?)}"

#define PROCEDURE_080827_0259 (UCHAR*)"{call dbo.atum_PROCEDURE_080827_0259(?)}"
#define PROCEDURE_080827_0260 (UCHAR*)"{call dbo.atum_PROCEDURE_080827_0260(?,?,?,?)}"
#define PROCEDURE_080827_0261 (UCHAR*)"{call dbo.atum_PROCEDURE_080827_0261(?)}"
#define PROCEDURE_080827_0262 (UCHAR*)"{call dbo.atum_PROCEDURE_080827_0262(?,?,?,?)}"
#define PROCEDURE_080827_0263 (UCHAR*)"{call dbo.atum_PROCEDURE_080827_0263(?,?,?)}"
#define PROCEDURE_080827_0264 (UCHAR*)"{call dbo.atum_PROCEDURE_080827_0264(?,?,?,?,?,?,?,?,?)}"
#define PROCEDURE_080827_0265 (UCHAR*)"{call dbo.atum_PROCEDURE_080827_0265(?,?,?,?,?,?,?,?,?)}"

#ifdef S_ITEM_EVENT_RETURN_USER_BCKIM				// 2013-02-28 by bckim, 복귀유져 버프추가
#define PROCEDURE_080827_0266 (UCHAR*)"{call dbo.atum_PROCEDURE_080827_0266(?,?,?,?,?,?,?,?,?,?, ?,?,?,?,?,?,?,?,?,? ,?)}"	
#define PROCEDURE_080827_0267 (UCHAR*)"{call dbo.atum_PROCEDURE_080827_0267(?)}"
#define PROCEDURE_080827_0268 (UCHAR*)"{call dbo.atum_PROCEDURE_080827_0268(?,?,?,?,?,?,?,?,?,?, ?,?,?,?,?,?,?,?,?,?, ?,?)}"
#define PROCEDURE_080827_0269 (UCHAR*)"{call dbo.atum_PROCEDURE_080827_0269(?,?,?,?,?,?,?,?,?)}"
#else 
#define PROCEDURE_080827_0266 (UCHAR*)"{call dbo.atum_PROCEDURE_080827_0266(?,?,?,?,?,?,?,?,?,?, ?,?,?,?,?,?,?,?,?)}"			// 2009-11-19 by cmkwon, 아이템 이벤트에 Account/Character 체크 기능 추가 - 인자 1개 추가 // 2010-06-16 by jskim, 레벨별 이벤트 체크 기능 - // start 2011-08-25 by shcho, 횟수별 아이템 지급기능 구현 -
#define PROCEDURE_080827_0267 (UCHAR*)"{call dbo.atum_PROCEDURE_080827_0267(?)}"
#define PROCEDURE_080827_0268 (UCHAR*)"{call dbo.atum_PROCEDURE_080827_0268(?,?,?,?,?,?,?,?,?,?, ?,?,?,?,?,?,?,?,?,?)}"		// 2009-11-19 by cmkwon, 아이템 이벤트에 Account/Character 체크 기능 추가 - 인자 1개 추가 // 2010-06-16 by jskim, 레벨별 이벤트 체크 기능 -	// start 2011-08-25 by shcho, 횟수별 아이템 지급기능 구현 -
#define PROCEDURE_080827_0269 (UCHAR*)"{call dbo.atum_PROCEDURE_080827_0269(?,?,?,?,?,?,?,?,?)}"
#endif

//////////////////////////////////////////////////////////////////////////
// 2008-09-09 by dhjin, 러시아 빌링 관련 -
#define PROCEDURE_080909_0270 (UCHAR*)"{call dbo.Usp_Check_Balance(?,?)}"
#define PROCEDURE_080909_0271 (UCHAR*)"{call dbo.Usp_Sell_Item(?,?,?,?,?,?,?,?)}"

// 2008-11-04 by dhjin, 럭키머신
#define PROCEDURE_081104_0272 (UCHAR*)"{call dbo.atum_Update_LuckyItem_DropCount(?,?,?)}"
#define PROCEDURE_081104_0273 (UCHAR*)"{call dbo.atum_Update_LuckyItem_STARTTIME(?,?)}"
#define PROCEDURE_081110_0274 (UCHAR*)"{call dbo.atum_load_ti_LuckyMachine}"
#define PROCEDURE_081125_0275 (UCHAR*)"{call dbo.atum_Update_LuckyItem_DropCount_Reset(?)}"
#define PROCEDURE_081125_0276 (UCHAR*)"{call dbo.atum_ValidCheck_td_MysteryItemDropCount}"
#define PROCEDURE_081126_0277 (UCHAR*)"{call dbo.atum_load_ti_MysteryItemDropByOmi}"
#define PROCEDURE_081201_0278 (UCHAR*)"{call dbo.atum_Update_LuckyItem_DropCount_AllReset_ByAdminTool}"
#define PROCEDURE_081201_0279 (UCHAR*)"{call dbo.atum_Update_LuckyItem_DropCount_ServerGroupIDReset_ByAdminTool(?)}"

///////////////////////////////////////////////////////////////////////////////
// 2009-01-14 by cmkwon, 운영자 자동 공지 시스템 구현 - 
#define PROCEDURE_090115_0280 (UCHAR*)"{call dbo.atum_Load_AdminAutoNoticeInfo}"
#define PROCEDURE_090115_0281 (UCHAR*)"{call dbo.atum_Load_AdminAutoNoticeString}"
#define PROCEDURE_090115_0282 (UCHAR*)"{call dbo.atum_Update_AdminAutoNoticeInfo(?,?,?,?)}"
#define PROCEDURE_090115_0283 (UCHAR*)"{call dbo.atum_Insert_AdminAutoNoticeString(?,?,?)}"

//////////////////////////////////////////////////////////////////////////
// 2009-01-12 by dhjin, 선전 포고
#define PROCEDURE_090114_0284 (UCHAR*)"{call dbo.atum_Load_DeclarationOfWarInfo}"
#define PROCEDURE_090114_0285 (UCHAR*)"{call dbo.atum_Load_DeclarationOfWarForbidTimeInfo}"
#define PROCEDURE_090114_0286 (UCHAR*)"{call dbo.atum_UpdateStartDeclarationOfWar(?,?,?,?,?)}"
#define PROCEDURE_090114_0287 (UCHAR*)"{call dbo.atum_UpdateEndDeclarationOfWar(?,?,?,?)}"
#define PROCEDURE_090114_0288 (UCHAR*)"{call dbo.atum_UpdateMSWarStartTime(?,?,?,?,?)}"
#define PROCEDURE_090114_0289 (UCHAR*)"{call dbo.atum_UpdateStepDeclarationOfWarByAdminTool(?,?)}"
#define PROCEDURE_090114_0290 (UCHAR*)"{call dbo.atum_UpdateSelectCountDeclarationOfWarByAdminTool(?,?,?)}"
#define PROCEDURE_090114_0291 (UCHAR*)"{call dbo.atum_UpdateForbidTimeDeclarationOfWarByAdminTool(?,?,?)}"
#define PROCEDURE_090114_0292 (UCHAR*)"{call dbo.atum_ResetDeclarationOfWarByAdminTool}"
#define PROCEDURE_090114_0293 (UCHAR*)"{call dbo.atum_UpdateMSWarStartDeclarationOfWarByAdminTool(?,?,?)}"

///////////////////////////////////////////////////////////////////////////////
// 2009-01-28 by cmkwon, 캐쉬?수정(추천탭,신상품 추가) - 프로시저 2개 추가
#define PROCEDURE_090129_0295 (UCHAR*)"{call dbo.atum_Load_CashShopBit}"
// 2010-01-26 by cmkwon, 캐쉬 아이템 한정판매 시스템 구현 - 인자 1개 추가
#define PROCEDURE_090129_0296 (UCHAR*)"{call dbo.atum_Update_CashShopBit(?,?,?)}"


///////////////////////////////////////////////////////////////////////////////
// 2009-02-12 by cmkwon, EP3-3 월드랭킹시스템 구현 - 
#define PROCEDURE_090219_0300 (UCHAR*)"{call dbo.atum_wrk_load_selfServiceInfo8ServerGroupName(?)}"
#define PROCEDURE_090219_0301 (UCHAR*)"{call dbo.atum_wrk_delete_selfAllLevelRanking(?,?)}"
#define PROCEDURE_090219_0302 (UCHAR*)"{call dbo.atum_wrk_arrange_selfAllLevelRanking(?,?,?)}"
#define PROCEDURE_090219_0303 (UCHAR*)"{call dbo.atum_wrk_get_LevelRanking(?,?)}"
#define PROCEDURE_090219_0304 (UCHAR*)"{call dbo.atum_wrk_delete_selfAllFameRanking(?,?)}"
#define PROCEDURE_090219_0305 (UCHAR*)"{call dbo.atum_wrk_arrange_selfAllFameRanking(?,?,?)}"
#define PROCEDURE_090219_0306 (UCHAR*)"{call dbo.atum_wrk_get_FameRanking(?,?)}"
#define PROCEDURE_090219_0307 (UCHAR*)"{call dbo.atum_wrk_delete_selfAllPVPRanking(?,?)}"
#define PROCEDURE_090219_0308 (UCHAR*)"{call dbo.atum_wrk_arrange_selfAllPVPRanking(?,?,?)}"
#define PROCEDURE_090219_0309 (UCHAR*)"{call dbo.atum_wrk_get_PVPRanking(?,?)}"
#define PROCEDURE_090219_0310 (UCHAR*)"{call dbo.atum_wrk_get_allServiceInfo}"
#define PROCEDURE_090219_0311 (UCHAR*)"{call dbo.atum_wrk_updateNickName(?,?)}"
#define PROCEDURE_090219_0312 (UCHAR*)"{call dbo.atum_wrk_load_selfServiceInfo}"
#define PROCEDURE_090219_0313 (UCHAR*)"{call dbo.atum_wrkR_updateService_to_WRankingServer(?,?,?,?)}"
#define PROCEDURE_090219_0314 (UCHAR*)"{call dbo.atum_wrk_update_selfServiceInfo(?,?,?,?)}"
#define PROCEDURE_090219_0315 (UCHAR*)"{call dbo.atum_wrkR_deleteService_from_WRankingServer(?)}"
#define PROCEDURE_090219_0316 (UCHAR*)"{call dbo.atum_wrk_delete_selfServiceInfo}"
#define PROCEDURE_090219_0317 (UCHAR*)"{call dbo.atum_wrk_load_ServerGroupInfo_forWRK}"
#define PROCEDURE_090219_0318 (UCHAR*)"{call dbo.atum_wrk_update_ServerGroupInfo_forWRK(?,?)}"
#define PROCEDURE_090219_0319 (UCHAR*)"{call dbo.atum_wrk_insert_ServiceInfo(?,?,?,?)}"
#define PROCEDURE_090219_0320 (UCHAR*)"{call dbo.atum_wrkR_loadAllService_from_WRankingServer}"
#define PROCEDURE_090219_0321 (UCHAR*)"{call dbo.atum_wrk_delete_AllServiceList}"
#define PROCEDURE_090219_0322 (UCHAR*)"{call dbo.atum_wrkR_delete_LevelRankingList_from_WRankingServer(?)}"
#define PROCEDURE_090219_0323 (UCHAR*)"{call dbo.atum_wrkR_delete_FameRankingList_from_WRankingServer(?)}"
#define PROCEDURE_090219_0324 (UCHAR*)"{call dbo.atum_wrkR_delete_PVPRankingList_from_WRankingServer(?)}"
#define PROCEDURE_090219_0325 (UCHAR*)"{call dbo.atum_wrkR_insert_LevelRanker_to_WRankingServer(?,?,?,?,?, ?,?,?,?,?, ?)}"
#define PROCEDURE_090219_0326 (UCHAR*)"{call dbo.atum_wrkR_insert_FameRanker_to_WRankingServer(?,?,?,?,?, ?,?,?,?,?)}"
#define PROCEDURE_090219_0327 (UCHAR*)"{call dbo.atum_wrkR_insert_PVPRanker_to_WRankingServer(?,?,?,?,?, ?,?,?,?,?, ?,?)}"
#define PROCEDURE_090219_0328 (UCHAR*)"{call dbo.atum_wrkR_get_LevelRanking_from_WRankingServer}"
#define PROCEDURE_090219_0329 (UCHAR*)"{call dbo.atum_wrkR_get_FameRanking_from_WRankingServer}"
#define PROCEDURE_090219_0330 (UCHAR*)"{call dbo.atum_wrkR_get_PVPRanking_from_WRankingServer}"
#define PROCEDURE_090219_0331 (UCHAR*)"{call dbo.atum_wrk_delete_otherAllLevelRanking(?)}"
#define PROCEDURE_090219_0332 (UCHAR*)"{call dbo.atum_wrk_delete_otherAllFameRanking(?)}"
#define PROCEDURE_090219_0333 (UCHAR*)"{call dbo.atum_wrk_delete_otherAllPVPRanking(?)}"
#define PROCEDURE_090219_0334 (UCHAR*)"{call dbo.atum_wrk_insert_LevelRanker(?,?,?,?,?, ?,?,?,?,?, ?)}"
#define PROCEDURE_090219_0335 (UCHAR*)"{call dbo.atum_wrk_insert_FameRanker(?,?,?,?,?, ?,?,?,?,?)}"
#define PROCEDURE_090219_0336 (UCHAR*)"{call dbo.atum_wrk_insert_PVPRanker(?,?,?,?,?, ?,?,?,?,?, ?,?)}"
#define PROCEDURE_090219_0337 (UCHAR*)"{call dbo.atum_wrk_delete_ServerGroupInfo_forWRK}"
#define PROCEDURE_090219_0338 (UCHAR*)"{call dbo.atum_wrk_get_SelfLevelRanking(?)}"
#define PROCEDURE_090219_0339 (UCHAR*)"{call dbo.atum_wrk_get_SelfFameRanking(?)}"
#define PROCEDURE_090219_0340 (UCHAR*)"{call dbo.atum_wrk_get_SelfPVPRanking(?)}"

// 2009-03-03 by dhjin, 럭키머신 수정안
#define PROCEDURE_090303_0341 (UCHAR*)"{call dbo.atum_load_ti_LuckyMachineByOmi}"



///////////////////////////////////////////////////////////////////////////////
// 2009-03-31 by cmkwon, 세력초기화 시스템 구현 - 
#define PROCEDURE_090401_0342 (UCHAR*)"{call dbo.atum_Load_QuestMatching}"
#define PROCEDURE_090401_0343 (UCHAR*)"{call dbo.atum_Load_ItemMatching}"
#define PROCEDURE_090401_0344 (UCHAR*)"{call dbo.atum_Init_InfluenceType_AllCharacter}"
#define PROCEDURE_090401_0345 (UCHAR*)"{call dbo.atum_Init_InfluenceType_ByAccountUID(?)}"
#define PROCEDURE_090401_0346 (UCHAR*)"{call dbo.atum_ChangeItemWithItemMatching(?,?)}"
#define PROCEDURE_090401_0347 (UCHAR*)"{call dbo.atum_check_enableChangeInfluence(?)}"


///////////////////////////////////////////////////////////////////////////////
// 2009-06-25 by cmkwon, 베트남 VTC 빌링 시스템 수정 - 
#define PROCEDURE_090625_0348 (UCHAR*)"{? = call dbo.SP_Account_GetTotalVcoin(?,?,?)}"
#define PROCEDURE_090625_0349 (UCHAR*)"{? = call dbo.SP_SCO_BuyItem(?,?,?,?,?,?,?)}"
										

///////////////////////////////////////////////////////////////////////////////
// 2009-07-09 by jwlee, 게임 정보 테이블 데이터 DB에 밀어넣기 툴 구현 -
#define PROCEDURE_090709_0350 (UCHAR*)"{call dbo.atum_import_ti_item(?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)}"	// 2009-09-09 ~ 2010-02-10 by dhjin, 인피니티 - 발동류장착아이템

// 2010-04-09 by cmkwon, 인피2차 추가 수정(단계별 보상 추가) - 필드 1개추가(DropType)
#define PROCEDURE_090709_0351 (UCHAR*)"{call dbo.atum_import_ti_monsteritem(?,?,?,?,?,?,?,?)}"

#define PROCEDURE_090709_0352 (UCHAR*)"{call dbo.atum_import_ti_rareiteminfo(?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)}"
#define PROCEDURE_090709_0353 (UCHAR*)"{call dbo.atum_import_ti_shop(?,?,?)}"
#define PROCEDURE_090709_0354 (UCHAR*)"{call dbo.atum_import_ti_citytargetwarpmap(?,?,?,?,?,?)}"
#define PROCEDURE_090709_0355 (UCHAR*)"{call dbo.atum_import_ti_itemmixinginfo(?,?,?,?,?)}"		// 2013-05-06 by hskim, 아이템 미리 보기 (속성 값 추가) - // 2012-02-20 by hskim, DB 정규화 - ItemMixingInfo
#define PROCEDURE_090709_0356 (UCHAR*)"{call dbo.atum_import_ti_enchantinfo(?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)}"
#define PROCEDURE_090709_0357 (UCHAR*)"{call dbo.atum_import_ti_strategypointsummonmapindex(?,?,?)}"
//#define PROCEDURE_090709_0358 (UCHAR*)"{call dbo.atum_import_ti_arenainfo(?,?,?,?,?,?)}"
#define PROCEDURE_090709_0358 (UCHAR*)"{call dbo.atum_import_ti_arenainfo(?,?,?,?,?,?,?,?)}"	// 2012-05-23 by jhseol, 아레나 추가개발 - 최소 보상 워포 필드 추가
// 2012-09-14 by jhseol, 아레나 추가개발 part3 - 아레나 지급 아이템 추가 (아레나 전용 탄약 재충전 카드)
#ifdef _ARENA_ADDITIONAL_SUPPLY_ITEMS
#define PROCEDURE_090709_0359 (UCHAR*)"{call dbo.atum_import_ti_arenamapinfo(?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)}"	// 4번째 지급 아이템 추가
#else
#define PROCEDURE_090709_0359 (UCHAR*)"{call dbo.atum_import_ti_arenamapinfo(?,?,?,?,?,?,?,?,?,?,?)}"
#endif
// end 2012-09-14 by jhseol, 아레나 추가개발 part3 - 아레나 지급 아이템 추가 (아레나 전용 탄약 재충전 카드)
#define PROCEDURE_090709_0360 (UCHAR*)"{call dbo.atum_import_ti_mysteryitemdrop(?,?,?,?,?,?,?,?,?,?,?,?,?)}"
#define PROCEDURE_090709_0361 (UCHAR*)"{call dbo.atum_import_ti_influencewar(?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)}"
#define PROCEDURE_090709_0362 (UCHAR*)"{call dbo.atum_import_ti_outpostnextwartime(?)}"
#define PROCEDURE_090709_0363 (UCHAR*)"{call dbo.atum_import_ti_actionbylevel(?,?)}"
#define PROCEDURE_090709_0364 (UCHAR*)"{call dbo.atum_import_ti_mapinfo(?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)}"	// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 
#define PROCEDURE_090709_0365 (UCHAR*)"{call dbo.atum_import_ti_buildingnpc(?,?,?,?,?,?,?,?,?)}"

// 2010-04-14 by cmkwon, 인피2차 몬스터 랜텀 타겟 변경 처리 - ChangeTargetTime 필드 추가
#define PROCEDURE_090709_0366 (UCHAR*)"{call dbo.atum_import_ti_monster(?,?,?,?,?, ?,?,?,?,?, ?,?,?,?,?, ?,?,?,?,?, ?,?,?,?,?, ?,?,?,?,?, ?,?,?,?,?, ?,?,?,?,?, ?,?,?,?,?, ?,?,?,?,?, ?,?,?,?,?, ?,?,?)}"	// 2010-03-31 by dhjin, 인피니티(기지방어) - // 2009-09-09 ~ 2010 by dhjin, 인피니티 - // 2011-05-23 by hskim, 인피니티 3차 - 웨이포인트 구현 - WayPointPattern 필드 추가

#define PROCEDURE_090709_0367 (UCHAR*)"{call dbo.atum_import_ti_tutorialinfo(?,?,?,?,?,?,?)}"
#define PROCEDURE_090709_0368 (UCHAR*)"{call dbo.atum_import_ti_mapobject(?,?,?,?,?,?,?,?,?,?)}"
#define PROCEDURE_090709_0369 (UCHAR*)"{call dbo.atum_import_ti_luckymachine(?,?,?,?,?,?,?,?,?,?)}"
#define PROCEDURE_090709_0370 (UCHAR*)"{call dbo.atum_import_ti_questmatching(?,?)}"
#define PROCEDURE_090709_0371 (UCHAR*)"{call dbo.atum_import_ti_itemmatching(?,?)}"
#define PROCEDURE_090709_0372 (UCHAR*)"{call dbo.atum_import_ti_item_info(?,?,?,?,?,?,?,?)}"

#define PROCEDURE_090709_0373 (UCHAR*)"{call dbo.atum_delete_ti_item}"
#define PROCEDURE_090709_0374 (UCHAR*)"{call dbo.atum_delete_ti_monsteritem}"
#define PROCEDURE_090709_0375 (UCHAR*)"{call dbo.atum_delete_ti_rareiteminfo}"
#define PROCEDURE_090709_0376 (UCHAR*)"{call dbo.atum_delete_ti_shop}"
#define PROCEDURE_090709_0377 (UCHAR*)"{call dbo.atum_delete_ti_citytargetwarpmap}"
#define PROCEDURE_090709_0378 (UCHAR*)"{call dbo.atum_delete_ti_itemmixinginfo}"
#define PROCEDURE_090709_0379 (UCHAR*)"{call dbo.atum_delete_ti_enchantinfo}"
#define PROCEDURE_090709_0380 (UCHAR*)"{call dbo.atum_delete_ti_strategyoointsummonmapindex}"
#define PROCEDURE_090709_0381 (UCHAR*)"{call dbo.atum_delete_ti_arenainfo}"
#define PROCEDURE_090709_0382 (UCHAR*)"{call dbo.atum_delete_ti_arenamapinfo}"
#define PROCEDURE_090709_0383 (UCHAR*)"{call dbo.atum_delete_ti_mysteryitemdrop}"
#define PROCEDURE_090709_0384 (UCHAR*)"{call dbo.atum_delete_ti_influencewar}"
#define PROCEDURE_090709_0385 (UCHAR*)"{call dbo.atum_delete_ti_outpostnextwartime}"
#define PROCEDURE_090709_0386 (UCHAR*)"{call dbo.atum_delete_ti_actionbylevel}"
#define PROCEDURE_090709_0387 (UCHAR*)"{call dbo.atum_delete_ti_mapinfo}"
#define PROCEDURE_090709_0388 (UCHAR*)"{call dbo.atum_delete_ti_buildingnpc}"
#define PROCEDURE_090709_0389 (UCHAR*)"{call dbo.atum_delete_ti_monster}"
#define PROCEDURE_090709_0390 (UCHAR*)"{call dbo.atum_delete_ti_tutorialinfo}"
#define PROCEDURE_090709_0391 (UCHAR*)"{call dbo.atum_delete_ti_mapobject}"
#define PROCEDURE_090709_0392 (UCHAR*)"{call dbo.atum_delete_ti_luckymachine}"
#define PROCEDURE_090709_0393 (UCHAR*)"{call dbo.atum_delete_ti_questmatching}"
#define PROCEDURE_090709_0394 (UCHAR*)"{call dbo.atum_delete_ti_itemmatching}"
#define PROCEDURE_090709_0395 (UCHAR*)"{call dbo.atum_delete_ti_item_info}"
			

// 2009-07-07 by cmkwon, 로드 제너레이터 문제 해결 - 
// 2009-08-03 by cmkwon, 소스 통합으로 정의 변경 - 기존(PROCEDURE_090707_0350)
#define PROCEDURE_090707_0396 (UCHAR*)"{call dbo.atum_getAccountUID_byAccountName(?)}"


///////////////////////////////////////////////////////////////////////////////
// 2009-07-20 by cmkwon, 예당 PCBang 체크 시스템 수정 - 
// 2009-08-03 by cmkwon, 소스 통합으로 정의 변경 - 기존(PROCEDURE_090707_0351)
#define PROCEDURE_090720_0397 (UCHAR*)"{call dbo.usp_select_ipAddress(?)}"


///////////////////////////////////////////////////////////////////////////////
// 2009-08-26 by cmkwon, 그래픽 리소스 변경 시스템 구현 - 
#define PROCEDURE_090826_0398 (UCHAR*)"{call dbo.atum_Update_ShapeItemNum(?,?)}"


///////////////////////////////////////////////////////////////////////////////
// 2009-09-16 by cmkwon, 세력 초기화시 어뷰징 방지 구현 - 
#define PROCEDURE_090918_0400 (UCHAR*)"{call dbo.atum_Rearrange_InfluenceRate(?)}"
#define PROCEDURE_090918_0401 (UCHAR*)"{call dbo.atum_Load_InfluenceRate}"
#define PROCEDURE_090918_0402 (UCHAR*)"{call dbo.atum_Update_AllowablePercent(?,?,?)}"
#define PROCEDURE_090918_0403 (UCHAR*)"{call dbo.atum_Delete_InfluenceRate(?,?)}"


///////////////////////////////////////////////////////////////////////////////
// 2009-09-16 by cmkwon, 세력 초기화시 어뷰징 방지 구현 - ti_InfluenceRate 테이블 추가, 
#define PROCEDURE_090922_0404 (UCHAR*)"{call dbo.atum_import_ti_InfluenceRate(?,?)}"
#define PROCEDURE_090922_0405 (UCHAR*)"{call dbo.atum_delete_ti_InfluenceRate}"

///////////////////////////////////////////////////////////////////////////////
// 2009-10-12 by cmkwon, 프리스카 제거 방안 적용 - 
#define PROCEDURE_091013_0406 (UCHAR*)"{call dbo.atum_change_StartCityMapIndex(?,?)}"


///////////////////////////////////////////////////////////////////////////////
// 2010-01-14 by cmkwon, 월드랭킹시스템 버그 수정(서비스삭제,서버그룹삭제) - 
#define PROCEDURE_100114_0410 (UCHAR*)"{call dbo.atum_wrkR_delete_AllRankingList_from_WRankingServer(?,?)}"
#define PROCEDURE_100114_0411 (UCHAR*)"{call dbo.atum_wrkR_delete_AllRankingList_from_LRankingServer(?,?)}"


////////////////////////////////////////////////////////////////////////////////
// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 
#define PROCEDURE_090909_0500 (UCHAR*)"{call dbo.atum_Load_HPAction}"
#define PROCEDURE_090909_0501 (UCHAR*)"{call dbo.atum_Load_Cinema}"
#define PROCEDURE_090909_0502 (UCHAR*)"{call dbo.atum_Load_InfinityMode}"
#define PROCEDURE_090909_0503 (UCHAR*)"{call dbo.atum_Load_InfinityMonster}"
#define PROCEDURE_090909_0504 (UCHAR*)"{call dbo.atum_Load_InfinityImpute(?,?)}"
#define PROCEDURE_090909_0505 (UCHAR*)"{call dbo.atum_Insert_InfinityImpute(?,?,?,?)}"
#define PROCEDURE_090909_0506 (UCHAR*)"{call dbo.atum_Update_InfinityImpute(?,?,?)}"
#define PROCEDURE_090909_0507 (UCHAR*)"{call dbo.atum_Reset_InfinityImpute(?)}"
#define PROCEDURE_090909_0508 (UCHAR*)"{call dbo.atum_Load_Revision}"
#define PROCEDURE_090909_0509 (UCHAR*)"{call dbo.arena_CopyDBInfo_Infinity(?,?,?)}"
#define PROCEDURE_090909_0510 (UCHAR*)"{call dbo.atum_InfinityFinInsertItem(?,?,?,?,?, ?,?,?,?,?, ?,?,?,?,?,?)}"	// 2009-09-09 ~ 2010-02-10 by dhjin, 인피니티 - 발동류장착아이템
#define PROCEDURE_090909_0511 (UCHAR*)"{call dbo.atum_InfinityFinUpdateItem(?,?,?,?,?,?)}"			// 2009-09-09 ~ 2010-02-10 by dhjin, 인피니티 - 발동류장착아이템
#define PROCEDURE_090909_0512 (UCHAR*)"{call dbo.atum_CharacterSaveDataInfinityFin(?,?,?,?,?, ?,?,?,?,?, ?,?,?,?,?, ?,?,?,?,?,?)}"	// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 팅긴 유저 재접속 처리
#define PROCEDURE_090909_0513 (UCHAR*)"{call dbo.atum_Insert_Log_Infinity(?,?,?,?,?, ?,?,?)}"	// 2010-06-25 by shcho, 인피니티 관련로그 찍기 - 종료시 최대 경험치 추가로 인자 추가	
#define PROCEDURE_090909_0514 (UCHAR*)"{call dbo.atum_delete_ti_HPAction}"
#define PROCEDURE_090909_0515 (UCHAR*)"{call dbo.atum_import_ti_HPAction(?,?,?,?,?, ?,?,?,?,?, ?,?,?,?)}"
#define PROCEDURE_090909_0516 (UCHAR*)"{call dbo.atum_delete_ti_Cinema}"

// 2010-03-29 by cmkwon, 인피2차 시네마에 효과음 파일 설정 추가 - SoundFileName 필드 추가
#define PROCEDURE_090909_0517 (UCHAR*)"{call dbo.atum_import_ti_Cinema(?,?,?,?,?, ?,?,?,?,?, ?,?,?,?,?, ?,?,?,?,?, ?,?,?,?,?, ?,?)}"  // 2010-03-31 by dhjin, 인피니티(기지방어) - // 2011-03-09 by hskim, 시네마 퀘스트 인덱스 설정 추가 - // 2011-04-28 by hskim, 인피니티 3차 - 시네마 관련 기능 추가 - 해당 맵채널 특정 몬스터 삭제 기능 추가

#define PROCEDURE_090909_0518 (UCHAR*)"{call dbo.atum_delete_ti_InfinityMonster}"
#define PROCEDURE_090909_0519 (UCHAR*)"{call dbo.atum_import_ti_InfinityMonster(?,?,?,?,?)}"
#define PROCEDURE_090909_0520 (UCHAR*)"{call dbo.atum_delete_ti_Revision}"
#define PROCEDURE_090909_0521 (UCHAR*)"{call dbo.atum_import_ti_Revision(?,?,?,?,?, ?,?)}"
#define PROCEDURE_090909_0522 (UCHAR*)"{call dbo.atum_delete_ti_InfinityMode}"
#define PROCEDURE_090909_0523 (UCHAR*)"{call dbo.atum_import_ti_InfinityMode(?,?,?,?,?, ?,?,?,?,?, ?,?,?,?, ?)}"		// 2011-06-14 by hskim, 인피니티 3차 - 패널티 기능 추가 (HP 및 시간 동시 지원을 위해) // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 사망시 패널티 추가
#define PROCEDURE_090909_0524 (UCHAR*)"{call dbo.atum_Reset_InfinityImputeByServerStart(?,?)}" 
#define PROCEDURE_090909_0525 (UCHAR*)"{call dbo.atum_delete_ti_Tender}"
#define PROCEDURE_090909_0526 (UCHAR*)"{call dbo.atum_import_ti_Tender(?,?)}" 
#define PROCEDURE_090909_0527 (UCHAR*)"{call dbo.atum_Load_Tender}"
#define PROCEDURE_090909_0528 (UCHAR*)"{call dbo.atum_delete_ti_InfinityShop}"
#define PROCEDURE_090909_0529 (UCHAR*)"{call dbo.atum_import_ti_InfinityShop(?,?,?,?,?, ?,?,?,?,?, ?,?,?)}" 
#define PROCEDURE_090909_0530 (UCHAR*)"{call dbo.atum_Load_InfinityShop}"
#define PROCEDURE_090909_0531 (UCHAR*)"{call dbo.atum_Reset_All_InfinityImpute}"					// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 귀속 정보 리셋
#define PROCEDURE_090909_0532 (UCHAR*)"{call dbo.atum_Reset_Character_InfinityImpute(?)}"			// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 귀속 정보 리셋


///////////////////////////////////////////////////////////////////////////////
// 2010-01-18 by cmkwon, 아이템 사용시 Parameter 중복 체크 시스템 구현 - 
#define PROCEDURE_100118_0540 (UCHAR*)"{call dbo.atum_get_ItemParamOverlap}"
#define PROCEDURE_100118_0541 (UCHAR*)"{call dbo.atum_import_ti_ItemParamOverlap(?,?)}"
#define PROCEDURE_100118_0542 (UCHAR*)"{call dbo.atum_delete_ti_ItemParamOverlap}"

///////////////////////////////////////////////////////////////////////////////
// 2010-01-26 by cmkwon, 캐쉬 아이템 한정판매 시스템 구현 - 
#define PROCEDURE_100127_0545 (UCHAR*)"{call dbo.atum_check_Buy_LimitedEdtion_shopItem(?)}"

#define PROCEDURE_100210_0550 (UCHAR*)"{call dbo.atum_delete_ti_InvokingWearItemDestParamNum}"		// 2009-09-09 ~ 2010-02-10 by dhjin, 인피니티 - 발동류장착아이템
#define PROCEDURE_100210_0551 (UCHAR*)"{call dbo.atum_import_ti_InvokingWearItemDestParamNum(?,?,?,?)}"	// 2009-09-09 ~ 2010-02-10 by dhjin, 인피니티 - 발동류장착아이템
#define PROCEDURE_100210_0552 (UCHAR*)"{call dbo.atum_Load_InvokingWearItemDestParamNum}"			// 2009-09-09 ~ 2010-02-10 by dhjin, 인피니티 - 발동류장착아이템
#define PROCEDURE_100210_0553 (UCHAR*)"{call dbo.atum_delete_ti_InvokingWearItemDestParamNumByUse}"			// 2009-09-09 ~ 2010-02-10 by dhjin, 인피니티 - 발동류장착아이템
#define PROCEDURE_100210_0554 (UCHAR*)"{call dbo.atum_import_ti_InvokingWearItemDestParamNumByUse(?,?,?,?)}"	// 2009-09-09 ~ 2010-02-10 by dhjin, 인피니티 - 발동류장착아이템
#define PROCEDURE_100210_0555 (UCHAR*)"{call dbo.atum_Load_InvokingWearItemDestParamNumByUse}"				// 2009-09-09 ~ 2010-02-10 by dhjin, 인피니티 - 발동류장착아이템
#define PROCEDURE_100210_0556 (UCHAR*)"{call dbo.atum_UpdateItemCoolingTimeStamp(?,?)}"				// 2009-09-09 ~ 2010-02-10 by dhjin, 인피니티 - 발동류장착아이템

///////////////////////////////////////////////////////////////////////////////
// 2010-04-06 by cmkwon, 인피2차 추가 수정 - 
#define PROCEDURE_100406_0560 (UCHAR*)"{call dbo.atum_InfinityUpdateUserMapInfo(?,?,?)}"

///////////////////////////////////////////////////////////////////////////////
// 2010-05-24 by shcho, 인피니티 난이도 조절 -
#define PROCEDURE_100506_0561 (UCHAR*)"{call dbo.atum_Load_InfinityDifficult}"
#define PROCEDURE_100506_0562 (UCHAR*)"{call dbo.atum_Load_InfinityDifficultMonster}"	// 2010-05-24 by shcho, 인피니티 난이도 조절 -
#define PROCEDURE_100524_0563 (UCHAR*)"{call dbo.atum_delete_ti_LevelAdjustment}"		// 2010-05-24 by shcho, 인피니티 난이도 조절 - 인피니티 난이도 조정에 관한 툴 업데이트
#define PROCEDURE_100524_0564 (UCHAR*)"{call dbo.atum_import_ti_LevelAdjustment(?,?,?,?,?,?)}"		// 2010-05-24 by shcho, 인피니티 난이도 조절 - 인피니티 난이도 조정에 관한 툴 업데이트
#define PROCEDURE_100524_0565 (UCHAR*)"{call dbo.atum_delete_ti_LevelAdjustmentReward}"	// 2010-05-24 by shcho, 인피니티 난이도 조절 - 인피니티 난이도 조정에 관한 툴 업데이트
#define PROCEDURE_100524_0566 (UCHAR*)"{call dbo.atum_import_ti_LevelAdjustmentReward(?,?,?,?)}"	// 2010-05-24 by shcho, 인피니티 난이도 조절 - 인피니티 난이도 조정에 관한 툴 업데이트

// 2010-08-05 by dhjin, 버닝맵 -
#define PROCEDURE_100805_0566 (UCHAR*)"{call dbo.atum_Load_BurningMap}"
#define PROCEDURE_100805_0567 (UCHAR*)"{call dbo.atum_delete_ti_BurningMap}"
#define PROCEDURE_100805_0568 (UCHAR*)"{call dbo.atum_import_ti_BurningMap(?,?,?,?,?)}"

///////////////////////////////////////////////////////////////////////////////
// 2010-06-15 by shcho&hslee 펫시스템 - 기본정보
#define PROCEDURE_100615_0567 (UCHAR*)"{call dbo.atum_Insert_td_pet(?,?,?,?,?, ?,?,?,?,?, ?,?,?,?,?, ?,?,?,?,? ,?,?,?,?,?)}" // 2011-08-17 by hskim, 파트너 시스템 2차 - 자료 구조 결정 - // 2010-06-15 by shcho&hslee 펫시스템 - 유저가 가진 펫 정보 테이블에 추가(서버내부 저장)
#define PROCEDURE_100615_0568 (UCHAR*)"{call dbo.atum_Insert_ti_pet(?,?,?,?,?, ?,?)}" // 2011-08-17 by hskim, 파트너 시스템 2차 - 자료 구조 결정 - // 2010-06-15 by shcho&hslee 펫시스템 - 기본 정보 펫 테이블 정보 저장 프로시저 (엑셀밀기)
#define PROCEDURE_100615_0569 (UCHAR*)"{call dbo.atum_Load_ti_pet}" // 2010-06-15 by shcho&hslee 펫시스템 - 펫 기본 정보 테이블 가져오는 프로시저(로드)
#define PROCEDURE_100615_0570 (UCHAR*)"{call dbo.atum_Load_ti_petLevel(?)}" // 2010-06-15 by shcho&hslee 펫시스템 - 펫 경험치 테이블 가져오는 프로시저(로드)
#define PROCEDURE_100615_0571 (UCHAR*)"{call dbo.atum_InsertStorePetItem(?,?,?,?,?, ?,?,?,?,?, ?,?,?,?)}" // 2011-08-17 by hskim, 파트너 시스템 2차 - 자료 구조 결정 - // 2010-06-15 by shcho&hslee 펫시스템 - 개인이 가진 팻정보 테이블을 가져오는 프로시저(로드)
#define PROCEDURE_100615_0574 (UCHAR*)"{call dbo.atum_Insert_ti_petLevel(?,?,?,?,?, ?,?,?,?)}"  // 2011-08-17 by hskim, 파트너 시스템 2차 - 자료 구조 결정 - // 2010-06-15 by shcho&hslee 펫시스템 - 기본 레벨 정보 펫 테이블 정보 저장 프로시저 (엑셀밀기)

///////////////////////////////////////////////////////////////////////////////
// 2010-06-15 by shcho&hslee 펫시스템 - 컨텐츠 관련
#define PROCEDURE_100615_0572 (UCHAR*)"{call dbo.atum_Update_PetRenameOnce(?,?,?)}" // 2010-06-15 by shcho&hslee 펫시스템 - 펫 이름 변경 프로시저 1회
#define PROCEDURE_100615_0573 (UCHAR*)"{call dbo.atum_Load_td_petInfo(?)}" // 2010-06-15 by shcho&hslee 펫시스템 - 소유 팻 정보 가져오기 팻UID는 아이템UID와 동일
#define PROCEDURE_100615_0575 (UCHAR*)"{call dbo.atum_Delete_ti_pet}"		// 2010-06-15 by shcho&hslee 펫시스템 - 팻 기본 정보 엑셀 밀기(테이블 삭제)
#define PROCEDURE_100615_0576 (UCHAR*)"{call dbo.atum_Delete_ti_petLevel}"	// 2010-06-15 by shcho&hslee 펫시스템 - 팻 레벨 정보 엑셀 밀기(테이블 삭제)
#define PROCEDURE_100615_0577 (UCHAR*)"{call dbo.atum_Load_ti_OperatorAction}" // 2010-06-15 by shcho&hslee 펫시스템 - omi 추가 작업. - 펫 오퍼레이터 정보.
#define PROCEDURE_100615_0578 (UCHAR*)"{call dbo.atum_import_ti_OperatorAction(?,?,?,?,?, ?)}" // 2010-06-15 by shcho&hslee 펫시스템 - DB 밀기 추가 작업 펫 오퍼레이터 정보.
#define PROCEDURE_100615_0579 (UCHAR*)"{call dbo.atum_Delete_ti_OperatorAction}" // 2010-06-15 by shcho&hslee 펫시스템 - DB 밀기 추가 작업 펫 오퍼레이터 정보.
//#define PROCEDURE_100615_0572 (UCHAR*)"{call dbo.Update_Petmaster_changed(?,?,?,?,?,?)}" // 2010-06-15 by shcho&hslee 펫시스템 - 아이템 교환시 소유자 변경

///////////////////////////////////////////////////////////////////////////////
// 2010-08-31 by shcho&jskim 아이템용해 시스템 - 용해 아이템 관련.
#define PROCEDURE_100831_0001 (UCHAR*)"{call dbo.atum_Load_ti_DissolutionItem}" // Load
#define PROCEDURE_100831_0002 (UCHAR*)"{call dbo.atum_Delete_ti_DissolutionItem}" // Delete
#define PROCEDURE_100831_0003 (UCHAR*)"{call dbo.atum_insert_ti_DissolutionItem(?,?,?,?,?, ?,?,?,?,?, ?,?,?)}" // Insert

///////////////////////////////////////////////////////////////////////////////
// 2011-03-17 by hskim, 인피니티 3차 - 몬스터 멀티 타겟팅 기능 추가
#define PROCEDURE_110317_0001 (UCHAR*)"{call dbo.atum_Load_ti_MonsterMultiTarget}" // Load
#define PROCEDURE_110317_0002 (UCHAR*)"{call dbo.atum_Delete_ti_MonsterMultiTarget}" // Delete
#define PROCEDURE_110317_0003 (UCHAR*)"{call dbo.atum_Insert_ti_MonsterMultiTarget(?,?,?,?,?)}" // Insert

///////////////////////////////////////////////////////////////////////////////
// 2011-05-23 by hskim, 인피니티 3차 - 웨이포인트 구현
#define PROCEDURE_110529_0001 (UCHAR*)"{call dbo.atum_Load_ti_WayPointMove}"					// Load
#define PROCEDURE_110529_0002 (UCHAR*)"{call dbo.atum_Delete_ti_WayPointMove}"					// Delete
#define PROCEDURE_110529_0003 (UCHAR*)"{call dbo.atum_Insert_ti_WayPointMove(?,?)}"				// Insert

#define PROCEDURE_110529_0004 (UCHAR*)"{call dbo.atum_Load_ti_WayPoint}"						// Load
#define PROCEDURE_110529_0005 (UCHAR*)"{call dbo.atum_Delete_ti_WayPoint}"						// Delete
#define PROCEDURE_110529_0006 (UCHAR*)"{call dbo.atum_Insert_ti_WayPoint(?,?,?,?,?, ?)}"		// Insert

///////////////////////////////////////////////////////////////////////////////
// 2010-06-25 by shcho, 인피니티 관련로그 찍기 - 습득 아이템 정보 DB저장
#define PROCEDURE_100914_0001 (UCHAR*)"{call dbo.atum_Insert_Log_Infinity_User_Get_TenderItem(?,?,?,?,?, ?)}" // Insert
///////////////////////////////////////////////////////////////////////////////
// 2010-06-01 by shcho, GLogDB 관련 -
#define PROCEDURE_GLOG_100601_0001 (UCHAR*)"{call dbo.GLog_BuyCashItem_Insert(?,?,?,?,?, ?,?,?,?)}"
#define PROCEDURE_GLOG_100601_0002 (UCHAR*)"{call dbo.GLog_CharacterPlayTime_Insert(?,?,?,?,?, ?,?,?,?,?, ?,?,?,?)}" 
#define PROCEDURE_GLOG_100601_0003 (UCHAR*)"{call dbo.GLog_ConnectTotalUserCount_Insert(?,?,?,?,?)}"
#define PROCEDURE_GLOG_100601_0004 (UCHAR*)"{call dbo.GLog_ConnectZoneUserCount_Insert(?,?,?,?,?,?)}"
#define PROCEDURE_GLOG_100601_0005 (UCHAR*)"{call dbo.GLog_EventParticipationRate_Insert(?,?,?,?,?,?,?,?)}"
#define PROCEDURE_GLOG_100601_0006 (UCHAR*)"{call dbo.atum_InsertGlogAccount(?,?,?)}"

///////////////////////////////////////////////////////////////////////////////
// 2011-01-26 by hskim, 인증 서버 구현
#define PROCEDURE_AUTHENTICATION_100701_0001 (UCHAR*)"{call dbo.authentication_server_select_account(?)}"
#define PROCEDURE_AUTHENTICATION_100701_0002 (UCHAR*)"{call dbo.authentication_server_insert_log(?,?,?,?,?, ?,?,?,?,?, ?,?,?,?)}"

///////////////////////////////////////////////////////////////////////////////
// 2011-08-22 by hskim, 파트너 시스템 2차 - 기능 구현
#define PROCEDURE_110824_0001 (UCHAR*)"{call dbo.atum_Update_PetName(?,?)}"
#define PROCEDURE_110824_0002 (UCHAR*)"{call dbo.atum_Update_PetExpRatio(?,?)}"
#define PROCEDURE_110824_0003 (UCHAR*)"{call dbo.atum_Update_PetLevel(?,?)}"
#define PROCEDURE_110824_0004 (UCHAR*)"{call dbo.atum_Update_PetExp(?,?)}"

#define PROCEDURE_110824_0005 (UCHAR*)"{call dbo.atum_Update_PetSocket(?,?,?,?,? ,?,?)}"
#define PROCEDURE_110824_0006 (UCHAR*)"{call dbo.atum_Update_PetKitSlot(?,?,?,?,? ,?,?)}"
#define PROCEDURE_110824_0007 (UCHAR*)"{call dbo.atum_Update_PetAutoSkillSlot(?,?,?,?,? ,?,?)}"

#define PROCEDURE_110824_0008 (UCHAR*)"{call dbo.atum_Update_PetInfo(?,?,?,?,? ,?,?,?,?,? ,?,?,?,?,? ,?,?,?)}"

#define PROCEDURE_110824_0009 (UCHAR*)"{call dbo.atum_Update_PetSocketOwner(?,?)}"		// 2012-01-30 by hskim, 파트너 시스템 2차 - 개인 창고 이동
#define PROCEDURE_110824_0010 (UCHAR*)"{call dbo.atum_GetStoreItemOne(?)}"				// 2012-01-30 by hskim, 파트너 시스템 2차 - 개인 창고 이동

#define PROCEDURE_110824_0011 (UCHAR*)"{call dbo.atum_Upgrade_Pet(?,?)}"						// 2015-06-22 Future, upgrading Pets with sockets

// start 2011-07-15 by shcho - 캐쉬아이템 체크 시스템 구현
#define PROCEDURE_CASHCHECK_110715_0001		 (UCHAR*)"{call dbo.atum_CashItemCheck}"
// end 2011-07-15 by shcho - 캐쉬아이템 체크 시스템 구현

// 2011-08-25 by shcho, 횟수별 아이템 지급기능 구현
#define PROCEDURE_LOGINITEMEVENT_110825_0001	(UCHAR*)"{call dbo.atum_InsertLoginItemEvent(?,?,?)}" // atum2_db_account에 프로시저 있음
#define PROCEDURE_LOGINITEMEVENT_110825_0002	(UCHAR*)"{?= call dbo.atum_CheckLoginItemEvent(?,?)}" // 리턴 값으로 카운트 체크 
#define PROCEDURE_LOGINITEMEVENT_110825_0003	(UCHAR*)"{call dbo.atum_EndLoginEvent(?)}"

///////////////////////////////////////////////////////////////////////////////
// 2011-10-10 by hskim, EP4 [트리거 시스템] - 화산재 / 모래 폭풍
#define PROCEDURE_111010_0001 (UCHAR*)"{call dbo.atum_Load_ti_MapBuff}"
#define PROCEDURE_111010_0002 (UCHAR*)"{call dbo.atum_Delete_ti_MapBuff}"
#define PROCEDURE_111010_0003 (UCHAR*)"{call dbo.atum_Insert_ti_MapBuff(?,?)}"

///////////////////////////////////////////////////////////////////////////////
// 2011-10-28 by hskim, EP4 [트리거 시스템] - 크리스탈 시스템
#define PROCEDURE_111028_0001 (UCHAR*)"{call dbo.atum_Load_ti_MapTrigger}"
#define PROCEDURE_111028_0002 (UCHAR*)"{call dbo.atum_Delete_ti_MapTrigger}"
#define PROCEDURE_111028_0003 (UCHAR*)"{call dbo.atum_Insert_ti_MapTrigger(?,?,?,?)}"

#define PROCEDURE_111028_0004 (UCHAR*)"{call dbo.atum_Load_ti_TriggerFunctionCrystal}"
#define PROCEDURE_111028_0005 (UCHAR*)"{call dbo.atum_Delete_ti_TriggerFunctionCrystal}"
#define PROCEDURE_111028_0006 (UCHAR*)"{call dbo.atum_Insert_ti_TriggerFunctionCrystal(?,?,?,?)}"

#define PROCEDURE_111028_0007 (UCHAR*)"{call dbo.atum_Load_ti_TriggerCrystalGroup}"
#define PROCEDURE_111028_0008 (UCHAR*)"{call dbo.atum_Delete_ti_TriggerCrystalGroup}"
#define PROCEDURE_111028_0009 (UCHAR*)"{call dbo.atum_Insert_ti_TriggerCrystalGroup(?,?,?)}"

#define PROCEDURE_111028_0010 (UCHAR*)"{call dbo.atum_Load_ti_TriggerCrystalDestroyGroup}"
#define PROCEDURE_111028_0011 (UCHAR*)"{call dbo.atum_Delete_ti_TriggerCrystalDestroyGroup}"
#define PROCEDURE_111028_0012 (UCHAR*)"{call dbo.atum_Insert_ti_TriggerCrystalDestroyGroup(?,?,?)}"

///////////////////////////////////////////////////////////////////////////////
// 2011-11-07 by hskim, EP4 [트리거 시스템] - NCG 거점전 시스템
#define PROCEDURE_111107_0001 (UCHAR*)"{call dbo.atum_Load_ti_TriggerFunctionNGCInflWar}"
#define PROCEDURE_111107_0002 (UCHAR*)"{call dbo.atum_Delete_ti_TriggerFunctionNGCInflWar}"
#define PROCEDURE_111107_0003 (UCHAR*)"{call dbo.atum_Insert_ti_TriggerFunctionNGCInflWar(?,?,?,?,?, ?,?,?)}"

#define PROCEDURE_111107_0004 (UCHAR*)"{call dbo.atum_Load_ti_TriggerNGCInflWarMonsterGroup}"
#define PROCEDURE_111107_0005 (UCHAR*)"{call dbo.atum_Delete_ti_TriggerNGCInflWarMonsterGroup}"
#define PROCEDURE_111107_0006 (UCHAR*)"{call dbo.atum_Insert_ti_TriggerNGCInflWarMonsterGroup(?,?,?,?)}"

///////////////////////////////////////////////////////////////////////////////
// 2011-12-21 by hskim, EP4 [동영상 1회 재생]
#define PROCEDURE_111221_0001 (UCHAR*)"{call dbo.atum_Load_ti_SystemEvent(?)}"
#define PROCEDURE_111222_0002 (UCHAR*)"{call dbo.atum_Update_AccountLastGameEndDate(?)}"

///////////////////////////////////////////////////////////////////////////////
// 2011-12-12 by hskim, GLog 2차
#define PROCEDURE_GLOG_2ND_111212_0001 (UCHAR*)"{call dbo.GLog_TB_USER_Insert(?,?,?,?,?)}"
#define PROCEDURE_GLOG_2ND_111212_0002 (UCHAR*)"{call dbo.GLog_TB_CONNECT_USER_Insert(?,?,?)}" 
#define PROCEDURE_GLOG_2ND_111212_0003 (UCHAR*)"{call dbo.GLog_TB_ITEM_BUY_Insert(?,?,?,?,?, ?,?,?,?,?, ?,?)}"
#define PROCEDURE_GLOG_2ND_111212_0004 (UCHAR*)"{call dbo.GLog_TB_CONCURRENT_USER_Insert(?,?,?,?,?)}"
#define PROCEDURE_GLOG_2ND_111212_0005 (UCHAR*)"{call dbo.GLog_TB_USER_PLAYTIME_Insert(?,?,?,?,?, ?,?,?,?,?, ?)}"
#define PROCEDURE_GLOG_2ND_111212_0006 (UCHAR*)"{call dbo.GLog_TB_USER_LocalUserConnectInfo_NewCount_Inc(?)}"
#define PROCEDURE_GLOG_2ND_111212_0007 (UCHAR*)"{call dbo.GLog_TB_USER_LocalUserConnectInfo_DelCount_Inc(?)}"

///////////////////////////////////////////////////////////////////////////////
// 2012-01-16 by hskim, 통계 - 화패
#define PROCEDURE_120116_0001 (UCHAR*)"{call dbo.atum_log_Insert_StatisticsMoney(?,?,?,?,?, ?,?,?,?)}"

///////////////////////////////////////////////////////////////////////////////
// 2012-02-27 by jhseol, 통계 - 화패 검색 프로시저
#define PROCEDURE_120227_0001 (UCHAR*)"{call dbo.atum_log_Search_StatisticsMoney(?,?)}"

///////////////////////////////////////////////////////////////////////////////
// 2012-02-20 by hskim, DB 정규화 - ItemMixingInfo
#define PROCEDURE_120220_0001 (UCHAR*)"{call dbo.atum_import_ti_itemmixingelement(?,?,?)}"
#define PROCEDURE_120220_0002 (UCHAR*)"{call dbo.atum_delete_ti_itemmixingelement}"
#define PROCEDURE_120220_0003 (UCHAR*)"{call dbo.atum_load_ti_itemmixingelement}"

///////////////////////////////////////////////////////////////////////////////
// 2012-03-13 by hskim, 현지화 관련 DB 구조 변경
#define PROCEDURE_120313_0001 (UCHAR*)"{call dbo.atum_Load_ti_OverlapItem}"
#define PROCEDURE_120313_0002 (UCHAR*)"{call dbo.atum_Delete_ti_OverlapItem}"
#define PROCEDURE_120313_0003 (UCHAR*)"{call dbo.atum_Insert_ti_OverlapItem(?,?,?,?)}"

///////////////////////////////////////////////////////////////////////////////
// 2012-03-15 by jhseol, 파트너 - 파트너 정보 변경사항 Log저장 프로시저
#define PROCEDURE_120315_0001 (UCHAR*)"{call dbo.atum_log_Insert_pet_info_updata(?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)}"

///////////////////////////////////////////////////////////////////////////////
// 2012-04-12 by jhseol, 아레나 추가개발 - 보상 : td_CharacterArena 관련 프로시저
#define PROCEDURE_120412_0001 (UCHAR*)"{call dbo.atum_arena_CharacterArena_Insert(?,?)}"
#define PROCEDURE_120412_0002 (UCHAR*)"{call dbo.atum_arena_CharacterArena_Get(?,?)}"
#define PROCEDURE_120412_0003 (UCHAR*)"{call dbo.atum_arena_CharacterArena_Updata(?,?,?,?)}"
// end 2012-04-12 by jhseol, 아레나 추가개발 - 보상 : td_CharacterArena 관련 프로시저

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 2012-08-27 by hskim, 베트남 인첸트 핵 관련 추가 보안 처리 (코드상의 문제점은 발견되지 않았으나 만일의 경우에는 로그를 남긴다) (동기화 DB 접속을 시도해서 성능 저하가 있음 : 필요없다고 판단되면 삭제 진행)
#define PROCEDURE_120827_0003 (UCHAR*)"{call dbo.atum_GetEnchantCountByItemUID(?)}"


///////////////////////////////////////////////////////////////////////////////

// we don't have those

// start 2012-10-08 by khkim, GLog
//#define PROCEDURE_GLOG_3ND_121011_0001 (UCHAR*)"{call dbo.GLog_ImportAcconutConnect(?,?,?,?)}" 
//#define PROCEDURE_GLOG_3ND_121011_0002 (UCHAR*)"{call dbo.GLog_ImportCharData(?,?,?,?,?,?,?,?,?)}"
//#define PROCEDURE_GLOG_3ND_121011_0003 (UCHAR*)"{call dbo.GLog_ImportItemLog(?,?,?,?,?,?,?)}"
//#define PROCEDURE_GLOG_3ND_121011_0004 (UCHAR*)"{call dbo.GLog_ImportItemStateLog(?,?,?,?,?,?)}"
//#define PROCEDURE_GLOG_3ND_121011_0005 (UCHAR*)"{call dbo.GLog_ImportServerLog(?,?)}"
// end 2012-10-08 by khkim, GLog

///////////////////////////////////////////////////////////////////////////////
// 2012-10-07 by hskim, 휴식 경험치
#define PROCEDURE_121008_0001 (UCHAR*)"{call dbo.atum_Get_AddExp_RestExperienceCount(?)}"
#define PROCEDURE_121008_0002 (UCHAR*)"{call dbo.atum_Set_AddExp_RestExperienceCount(?, ?)}"

///////////////////////////////////////////////////////////////////////////////
// 2015-06-11 Future, Additional KillCount
#define PROCEDURE_121008_0003 (UCHAR*)"{call dbo.atum_Get_AddKills_RestKillCount(?)}"
#define PROCEDURE_121008_0004 (UCHAR*)"{call dbo.atum_Set_AddKills_RestKillCount(?, ?)}"

///////////////////////////////////////////////////////////////////////////////
// 2012-10-10 by hskim, 기간제 속성 구현 (기간제 외형)
#define PROCEDURE_121010_0001 (UCHAR*)"{call dbo.atum_Get_FixedTerm(?, ?)}"
#define PROCEDURE_121010_0002 (UCHAR*)"{call dbo.atum_Insert_FixedTerm(?, ?, ?, ?, ?)}"
#define PROCEDURE_121010_0003 (UCHAR*)"{call dbo.atum_Delete_FixedTerm(?, ?)}"

///////////////////////////////////////////////////////////////////////////////
// 2012-11-13 by jhseol, 전쟁 시스템 리뉴얼 - 거점전
#define PROCEDURE_121113_0001 (UCHAR*)"{call dbo.atum_LoadRenewalStrategyPointSummonTime}"
#define PROCEDURE_121113_0002 (UCHAR*)"{call dbo.atum_UpdateRenewalStrategyPointSummonTime(?,?,?,?,?)}"
#define PROCEDURE_121113_0003 (UCHAR*)"{call dbo.atum_UpdateRenewalStrategyPointSummonInfo(?,?,?,?)}"
#define PROCEDURE_121113_0004 (UCHAR*)"{call dbo.atum_LoadRenewalStrategyPointMapInfluenceInfo}"
// end 2012-11-13 by jhseol, 전쟁 시스템 리뉴얼 - 거점전

// 2012-11-21 by bckim, 운영툴기능추가, 인챈트아이템제거기능
#define PROCEDURE_121121_0001 (UCHAR*)"{call dbo.atum_PROCEDURE_121121_001(?, ?, ?)}"
#define PROCEDURE_121121_0002 (UCHAR*)"{call dbo.atum_log_insert_item_Enchant_Change_By_Admin(?, ?, ?, ?, ?, ?, ?, ? )}"
// 2012-11-21 by bckim, 운영툴기능추가, 인챈트아이템제거기능. End

// 2012-12-20 by bckim, 세력초기화시 길드창고아이템이 길드장에게 옮겨지는 아이템 로그
#define PROCEDURE_121220_0001 (UCHAR*)"{call dbo.atum_LoadInitializedGuildList}"
#define PROCEDURE_121220_0002 (UCHAR*)"{call dbo.atum_Delete_LoadInitializedGuildList}"
// 2012-12-20 by bckim, 세력초기화시 길드창고아이템이 길드장에게 옮겨지는 아이템 로그. End

// 2013-01-16 by jhseol, 지도자 권한 박탈 시 지도자 관련정보 초기화
#define PROCEDURE_130116_0001 (UCHAR*)"{call dbo.atum_InitCityLeader(?,?)}"

///////////////////////////////////////////////////////////////////////////////
// 2013-01-21 by jhseol, NGC 전전기지 트리거 시스템
#define PROCEDURE_130121_0001 (UCHAR*)"{call dbo.atum_InsertTriggerFunctionNGCOutPost(?,?,?,?,?,?,?,?)}"
#define PROCEDURE_130121_0002 (UCHAR*)"{call dbo.atum_InsertTriggerOutPosKill(?,?,?,?)}"
#define PROCEDURE_130121_0003 (UCHAR*)"{call dbo.atum_LoadTriggerFunctionNGCOutPost}"
#define PROCEDURE_130121_0004 (UCHAR*)"{call dbo.atum_LoadTriggerOutPosKill}"
#define PROCEDURE_130121_0005 (UCHAR*)"{call dbo.atum_DeleteTriggerFunctionNGCOutPost}"
#define PROCEDURE_130121_0006 (UCHAR*)"{call dbo.atum_DeleteTriggerOutPosKill}"
#define PROCEDURE_130121_0007 (UCHAR*)"{call dbo.atum_InsertTriggerCrystalBuff(?,?)}"
#define PROCEDURE_130121_0008 (UCHAR*)"{call dbo.atum_LoadTriggerCrystalBuff}"
#define PROCEDURE_130121_0009 (UCHAR*)"{call dbo.atum_DeleteTriggerCrystalBuff}"

// 2013-02-05 by bckim, 캐시샵 추천탭 정렬순서부여
#define PROCEDURE_130204_0001 (UCHAR*)"{call dbo.atum_Load_Realignment_CashShop}"			
#define PROCEDURE_130204_0002 (UCHAR*)"{call dbo.atum_Update_Realignment_CashShop(?,?)}"	
// End. 2013-02-05 by bckim, 캐시샵 추천탭 정렬순서부여

///////////////////////////////////////////////////////////////////////////////
// 2013-02-28 by bckim, 복귀유져 버프추가
#define PROCEDURE_130228_0001 (UCHAR*)"{call dbo.atum_update_Log_ItemEvent_fixed_period(?, ?, ?, ?)}"
///////////////////////////////////////////////////////////////////////////////
// 2013-03-13 by hskim, 웹 캐시 상점
#define PROCEDURE_130319_0001 (UCHAR*)"{call dbo.atum_insertStoreItem_FromXMLRPC(?,?,?,?,?,?,?,?)}"
#define PROCEDURE_130319_0002 (UCHAR*)"{call dbo.atum_GetCharacterInfo_FromXMLRPC(?,?,?)}"
#define PROCEDURE_130319_0003 (UCHAR*)"{call dbo.atum_DeleteItem_FromXMLRPC(?,?,?,?)}"

///////////////////////////////////////////////////////////////////////////////
// 2013-03-29 by jhseol, 아이템 이벤트 - 매월 첫 결재시 선물지급
#define PROCEDURE_130329_0001 (UCHAR*)"{call dbo.atum_GetCashBuyDateByAccountUID(?)}"
#define PROCEDURE_130329_0002 (UCHAR*)"{call dbo.atum_GetCashBuyDateByCharacterUID(?)}"
#define PROCEDURE_130329_0003 (UCHAR*)"{call dbo.atum_InsertCashBuyDate(?,?,?)}"

///////////////////////////////////////////////////////////////////////////////
// 2013-03-29 by jhseol, 아이템 이벤트 - 서브타입 추가구조로 변경
#define PROCEDURE_130329_0004 (UCHAR*)"{call dbo.atum_InsertItemEventSubType(?,?,?,?,?,?,?,?)}"
#define PROCEDURE_130329_0005 (UCHAR*)"{call dbo.atum_GetItemEventSubType(?)}"

///////////////////////////////////////////////////////////////////////////////
// 2013-04-18 by jhseol,bckim 이달의 아머
#define PROCEDURE_130503_0001 (UCHAR*)"{call dbo.atum_InsertMonthlyArmorEvent(?,?,?,?,?,?)}"
#define PROCEDURE_130503_0002_1 (UCHAR*)"{call dbo.atum_GetMonthlyArmorEventByTool}"
#define PROCEDURE_130503_0002_2 (UCHAR*)"{call dbo.atum_GetMonthlyArmorEventByServer}"
#define PROCEDURE_130503_0003 (UCHAR*)"{call dbo.atum_DeleteMonthlyArmorEvent(?)}"

#define PROCEDURE_130503_0004 (UCHAR*)"{call dbo.atum_GetMonthlyArmorList(?,?)}"
#define PROCEDURE_130503_0005 (UCHAR*)"{call dbo.atum_GetMonthlyOptionList(?)}"
// end 2013-04-18 by jhseol,bckim 이달의 아머 - 이벤트 로드 프로시저 선언

// 2013-05-09 by hskim, 세력 포인트 개선
#define PROCEDURE_130510_0001 (UCHAR*)"{call dbo.atum_UpdateInfluenceConsecutiveVictorites(?,?,?)}"
// end 2013-05-09 by hskim, 세력 포인트 개선

// 2013-05-20 by hskim, [보안 시스템] 비정상적인 방법으로 아이템 추가 방지
#define PROCEDURE_130520_0001 (UCHAR*)"{call dbo.atum_InsertStoreExtension(?,?)}"
#define PROCEDURE_130520_0002 (UCHAR*)"{call dbo.atum_GetStoreExtension(?)}"
#define PROCEDURE_130520_0003 (UCHAR*)"{call dbo.atum_GetTemporarySystemInfomation(?)}"
// end 2013-05-20 by hskim, [보안 시스템] 비정상적인 방법으로 아이템 추가 방지


// 2013-05-31 by jhseol,bckim 아머 컬렉션 - 프로시저 추가
#define PROCEDURE_130531_0001 (UCHAR*)"{call dbo.atum_GetCollectionList(?,?)}"
#define PROCEDURE_130531_0002 (UCHAR*)"{call dbo.atum_UpdateCollectionInfo(?,?,?,?,?, ?,?)}"
#define PROCEDURE_130531_0003 (UCHAR*)"{call dbo.atum_Get_ShapeStatLevel(?)}"
#define PROCEDURE_130531_0004 (UCHAR*)"{call dbo.atum_Insert_ShapeStatLevel(?,?,?,?)}"
// end 2013-05-31 by jhseol,bckim 아머 컬렉션 - 프로시저 추가

#define PROCEDURE_130702_0001 (UCHAR*)"{call dbo.atum_log_Insert_Try_Enchant_Info(?,?,?,?,?, ?,?,?,?)}"		// 2013-07-01 by bckim, 10인챈트 미만 하이퍼카드처리, 로그추가, 운영툴 보완

// 2013-07-26 by jhseol, 타 계정 세력변경
#define PROCEDURE_130726_0001 (UCHAR*)"{call dbo.atum_CheckInfluenceType_ByAccountUID(?,?)}"
#define PROCEDURE_130726_0002 (UCHAR*)"{call dbo.atum_ChangeInfluence_ItemNum(?,?)}"
#define PROCEDURE_130726_0003 (UCHAR*)"{call dbo.atum_getAccountName_byAccountUID(?)}"
// end 2013-07-26 by jhseol, 타 계정 세력변경

#define PROCEDURE_130727_0001 (UCHAR*)"{call dbo.atum_GetMultipleIpStatusByAccountName(?)}"
#define PROCEDURE_130727_0002 (UCHAR*)"{call dbo.atum_Nation_Transfer(?,?)}"
#define PROCEDURE_130727_0003 (UCHAR*)"{call dbo.atum_GetReduceAvailableNationTransfers(?)}"

#endif // END - #ifndef _STRING_DEFINE_PROCEDURE_H_

