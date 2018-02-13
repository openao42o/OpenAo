
// 2008-07-02 by cmkwon
// #include "StringDefineQuery.h"
#ifndef _STRING_DEFINE_QUERY_H_
#define _STRING_DEFINE_QUERY_H_

// 2008-08-20 by cmkwon, MySQL 관련 소스 통합 - 
//#define DB_SERVER_MYSQL


///////////////////////////////////////////////////////////////////////////////
// 2008-07-02 by cmkwon, MySQL 지원 구현 -  

// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
// #define QUERY_080702_0001		"select AccountName, Password, AccountType,Sex, BirthYear, RegisteredDate, LastLoginDate,IsBlocked, ChattingBlocked, SecondaryPassword from atum2_db_account.dbo.td_account WITH (NOLOCK) where accountname = \'%s\';"
// #define QUERY_080702_0002		"update atum2_db_account.dbo.td_account set AccountType = %d, Password = \'%s\', SecondaryPassword = \'%s\' where accountname = \'%s\';"
// #define QUERY_080702_0003		"SELECT AccountName, BlockedType, StartDate,	EndDate, AdminAccountName, BlockedReason, BlockedReasonForOnlyAdmin FROM td_blockedaccounts WITH (NOLOCK);"
// #define QUERY_080702_0004		"UPDATE td_influencewardata SET InflLeaderCharacterUID = %d WHERE InfluenceType = %d;"
// #define QUERY_080702_0005		"UPDATE td_influencewardata SET InflSub1LeaderCharacterUID = %d WHERE InfluenceType = %d;"
// #define QUERY_080702_0006		"UPDATE td_influencewardata SET InflSub2LeaderCharacterUID = %d WHERE InfluenceType = %d;"
// #define QUERY_080702_0007		"SELECT wd.InfluenceType, wd.MGameServerID, wd.WartimeStage, wd.ContributionPoint, wd.InflLeaderCharacterUID, c.CharacterName, wd.InflSub1LeaderCharacterUID, c1.CharacterName, wd.InflSub2LeaderCharacterUID, c2.CharacterName \
// 									FROM td_influencewardata wd  LEFT OUTER JOIN td_character c \
// 									ON wd.InflLeaderCharacterUID = c.uniquenumber	\
// 									LEFT OUTER JOIN td_character c1	\
// 									ON wd.InflSub1LeaderCharacterUID = c1.uniquenumber \
// 									LEFT OUTER JOIN td_character c2 \
// 									ON wd.InflSub2LeaderCharacterUID = c2.uniquenumber;"
// 
// #define QUERY_080702_0008		"UPDATE td_influencewardata SET WartimeStage = 0, ContributionPoint = 0, InflLeaderCharacterUID = 0, OwnerOfConflictArea = 0, InflSub1LeaderCharacterUID = 0, InflSub2LeaderCharacterUID = 0, MSWarOptionType = 510;"
// #define QUERY_080702_0009		"UPDATE td_guild SET MonthlyFame = 0;"

#define QUERY_080702_0010_1_A	"select top %d "
#define QUERY_080702_0010_1_B	"select "
#define QUERY_080702_0010_2		" l.accountname, l.Time, i.LogStringKO, l.IPAddress, l.ServerName, l.MACAddress"	// 2015-09-23 Future, added logging of MAC Address
#define QUERY_080702_0010_3		" from atum_log_type_info i, "
#define QUERY_080702_0010_4		" l WITH (NOLOCK) where i.logtype = l.logtype "
#define QUERY_080702_0010_5		" and l.logtype = %d "
#define QUERY_080702_0010_6		" and l.accountname = \'%s\' "
#define QUERY_080702_0010_7		" and l.time > \'%s\' and l.time < \'%s\' "
#define QUERY_080702_0010_8		" and CAST(CAST(substring(IPAddress, 1, 1) AS INT) AS VARCHAR(3)) + '.' + CAST(CAST(substring(IPAddress, 2, 1) AS INT) AS VARCHAR(3)) + '.' + CAST(CAST(substring(IPAddress, 3, 1) AS INT) AS VARCHAR(3)) + '.' + CAST(CAST(substring(IPAddress, 4, 1) AS INT) AS VARCHAR(3)) like \'%s\'"
#define QUERY_080702_0010_8_1	" and l.MACAddress = \'%s\'"	// 2015-09-23 Future, added logging of MAC Address
#define QUERY_080702_0010_9		" order by l.time desc "
#define QUERY_080702_0010_10_A	" LIMIT 0,%d;"		// 2008-07-08 by cmkwon, MySQL 지원 구현 - Only for MySql
#define QUERY_080702_0010_10_B	" ;"					// 2008-07-08 by cmkwon, MySQL 지원 구현 - Only for MySql

#define QUERY_080702_0011_1_A	"select top %d "
#define QUERY_080702_0011_1_B	"select "
#define QUERY_080702_0011_2		" l.* "
#define QUERY_080702_0011_3		" from atum_log_type_info i, "
#define QUERY_080702_0011_4		" l WITH (NOLOCK) where i.logtype = l.logtype "
#define QUERY_080702_0011_5		" and l.logtype = %d "
#define QUERY_080702_0011_6		" and l.AccountName = \'%s\' "
#define QUERY_080702_0011_7		" and l.CharacterName = \'%s\' "
#define QUERY_080702_0011_8		" and l.time > \'%s\' and l.time < \'%s\' "
#define QUERY_080702_0011_9		" and l.GuildName = \'%s\' "
#define QUERY_080702_0011_10	" and l.GuildUID = %d "
#define QUERY_080702_0011_11	" and l.ItemNum = %d "
#define QUERY_080702_0011_12	" and l.ItemUID = %d "
#define QUERY_080702_0011_13	" order by time desc "
#define QUERY_080702_0011_14_A	" LIMIT 0,%d;"		// 2008-07-08 by cmkwon, MySQL 지원 구현 - Only for MySql
#define QUERY_080702_0011_14_B	" ;"				// 2008-07-08 by cmkwon, MySQL 지원 구현 - Only for MySql

#define QUERY_080702_0012_1_A	"select top %d "
#define QUERY_080702_0012_1_B	"select "
#define QUERY_080702_0012_2		" c.accountname, c.charactername, l.Time, i.LogStringKO, ii.ItemNum, ii.ItemName, l.ItemUniqueNumber, "
#define QUERY_080702_0012_3		" from atum_log_type_info i, "
#define QUERY_080702_0012_4		" l, atum2_db_account.dbo.ti_Item ii, td_character c WITH (NOLOCK) where i.logtype = l.logtype and l.ItemNum = ii.ItemNum and c.uniquenumber = l.characteruniquenumber "
#define QUERY_080702_0012_5		" and l.logtype = %d "
#define QUERY_080702_0012_6_A	" and c.accountname = \'%s\' "
#define QUERY_080702_0012_6_B	" and (c.accountname = \'%s\' or c.accountname = \'%s\') "
#define QUERY_080702_0012_7_A	" and ((c.charactername = \'%s\' and l.PeerCharacterName = \'%s\') or (c.charactername = \'%s\' and l.PeerCharacterName = \'%s\')) "
#define QUERY_080702_0012_7_B	" and c.charactername = \'%s\' "
#define QUERY_080702_0012_8		" and l.time > \'%s\' and l.time < \'%s\' "
#define QUERY_080702_0012_9		" and l.ItemNum = %d "
#define QUERY_080702_0012_10	" and l.ItemUniqueNumber = %d "
#define QUERY_080702_0012_11	" order by l.time desc "
#define QUERY_080702_0012_12_A	" LIMIT 0,%d;"		// 2008-07-08 by cmkwon, MySQL 지원 구현 - Only for MySql
#define QUERY_080702_0012_12_B	" ;"				// 2008-07-08 by cmkwon, MySQL 지원 구현 - Only for MySql

#define QUERY_080702_0013_1_A	"select top %d "
#define QUERY_080702_0013_1_B	"select "
#define QUERY_080702_0013_2_A	" l.accountname, l.charactername, l.Time, i.LogStringKO, l.MapIndex, l.Position_X, l.Position_Y, l.Position_Z, "
#define QUERY_080702_0013_2_B	" c.accountname, c.charactername, l.Time, i.LogStringKO, l.MapIndex, l.Position_X, l.Position_Y, l.Position_Z, "
#define QUERY_080702_0013_3		" from atum_log_type_info i, "
#define QUERY_080702_0013_4_A	" l WITH (NOLOCK) where i.logtype = l.logtype "
#define QUERY_080702_0013_4_B	" l, td_character c WITH (NOLOCK) where i.logtype = l.logtype and c.uniquenumber = l.characteruniquenumber "
#define QUERY_080702_0013_5		" and l.logtype = %d "
#define QUERY_080702_0013_6_A	" and l.accountname = \'%s\' "
#define QUERY_080702_0013_6_B	" and c.accountname = \'%s\' "
#define QUERY_080702_0013_7_A	" and l.charactername = \'%s\' "
#define QUERY_080702_0013_7_B	" and c.charactername = \'%s\' "
#define QUERY_080702_0013_8		" and l.time > \'%s\' and l.time < \'%s\' "
#define QUERY_080702_0013_9		" order by l.time desc "
#define QUERY_080702_0013_10_A	" LIMIT 0,%d;"		// 2008-07-08 by cmkwon, MySQL 지원 구현 - Only for MySql
#define QUERY_080702_0013_10_B	" ;"				// 2008-07-08 by cmkwon, MySQL 지원 구현 - Only for MySql

// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
// #define QUERY_080702_0014		"select UniqueNumber, ItemNum, CreatedTime,	UpdatedTime, ExpireTime from td_accountcashstore WITH (NOLOCK) where AccountUniqueNumber = %d;"
// #define QUERY_080702_0015		"INSERT INTO td_accountcashstore(AccountUniqueNumber,ItemNum,CreatedTime,UpdatedTime,ExpireTime) VALUES (%u, %d, \'%s\', \'%s\', \'%s\');"
// #define QUERY_080702_0016		"select * from td_accountblock where EndTime > GetDate();"
// #define QUERY_080702_0017		"SELECT GuildUniqueNumber, GuildName, GuildCommanderUniqueNumber, GuildMemberCapacity, GuildState, GuildDismemberDate, GuildMarkVersion, GuildMarkSize, WarWinPoint, WarLossPoint, TotalFame, MonthlyFame, GuildMarkImage, GuildMarkState, GuildMemberCardCashPrice FROM td_guild WITH (NOLOCK) WHERE GuildName = \'%s\';"
// #define QUERY_080702_0018		"SELECT GuildUniqueNumber, GuildName, GuildCommanderUniqueNumber, GuildMemberCapacity, GuildState, GuildDismemberDate, GuildMarkVersion, GuildMarkSize, WarWinPoint, WarLossPoint, TotalFame, MonthlyFame, GuildMarkImage, GuildMarkState, GuildMemberCardCashPrice FROM td_guild WITH (NOLOCK) WHERE GuildUniqueNumber = %d;"
// #define QUERY_080702_0019		"SELECT c.AccountName, c.CharacterName, dbo.atum_GetGearString(c.unitkind), c.InfluenceType, \
// 									c.level, c.propensity, c.PKWinPoint, c.PKLossPoint, gm.GuildRank \
// 									FROM td_character c, td_guildmember gm WITH (NOLOCK) \
// 									WHERE gm.GuildUniqueNumber = %d and c.uniqueNumber = gm.CharacterUniqueNumber;"
// #define QUERY_080702_0020		"SELECT s.UniqueNumber, i.ItemNum, s.PrefixCodeNum, s.SuffixCodeNum, s.ItemStorage, s.Wear \
// 									, s.ItemWindowIndex, s.CurrentCount, s.ColorCode, s.possess, s.CreatedTime \
// 									FROM td_store s, atum2_db_account.dbo.ti_Item i WITH (NOLOCK) WHERE s.Possess = %d AND s.ItemStorage = %d AND s.ItemNum = i.ItemNum;"
// #define QUERY_080702_0021		"select COUNT(*) from td_enchant WITH (NOLOCK) where TargetItemUniqueNumber = %I64d;"
// #define QUERY_080702_0022		"SELECT GuildUniqueNumber FROM td_guild WITH (NOLOCK) WHERE GuildName = \'%s\';"
// #define QUERY_080702_0023		"UPDATE td_guild SET GuildName = \'%s\', GuildMarkState= %d WHERE GuildUniqueNumber = %d;"
// #define QUERY_080702_0024		"UPDATE td_character SET GuildName = \'%s\' WHERE GuildUniqueNumber = %d;"
// #define QUERY_080702_0025		"SELECT uniquenumber, ServerGroupID, dayofweek, starttime, endtime, exprate, spirate, exprepairrate, dropitemrate, droprareitemrate, WarPointRate, InfluenceType, MinLevel, MaxLevel FROM atum2_db_account.dbo.ti_HappyHourEvent WITH (NOLOCK) order by ServerGroupID, InfluenceType, dayofweek;"
// #define QUERY_080702_0026		"INSERT atum2_db_account.dbo.ti_HappyHourEvent(ServerGroupID, dayofweek, starttime, endtime, exprate, spirate, exprepairrate, dropitemrate, droprareitemrate, WarPointRate, InfluenceType, MinLevel, MaxLevel) values( %d, %d, '%s', '%s', %6.4f, %6.4f, %6.4f, %6.4f, %6.4f, %6.4f, %d, %d, %d);"
// #define QUERY_080702_0027		"UPDATE atum2_db_account.dbo.ti_HappyHourEvent SET ServerGroupID= %d, dayofweek= %d, starttime = '%s', endtime = '%s', exprate = %6.2f, spirate = %6.2f, exprepairrate = %6.2f, dropitemrate = %6.2f, droprareitemrate = %6.2f , WarPointRate = %6.2f, InfluenceType = %d, MinLevel = %d, MaxLevel = %d where uniquenumber= %d;"
// #define QUERY_080702_0028		"SELECT UniqueNumber FROM atum2_db_account.dbo.ti_HappyHourEvent WITH(NOLOCK) WHERE InfluenceType = %d AND DayOfWeek = %d;"
// #define QUERY_080702_0029		"SELECT * FROM atum2_db_account.dbo.ti_itemevent WITH (NOLOCK) order by StartTime;"
// #define QUERY_080702_0030		"SELECT ItemEventUID FROM atum2_db_account.dbo.ti_itemevent WHERE ItemEventUID <> %d AND (ItemEventType = %d OR ItemEventType = %d);"
// 
// #define QUERY_080702_0031		"select * from td_blockedaccounts WITH (NOLOCK) where accountname = \'%s\';"
// #define QUERY_080702_0032		"select * from td_store s, td_character c WITH (NOLOCK) where ItemNum = %d and s.possess = c.uniquenumber and c.charactername like \'%s\';"
// #define QUERY_080702_0033		"delete from td_store where uniquenumber = %I64d;"
// #define QUERY_080702_0034		"delete from td_enchant where TargetItemUniqueNumber = %I64d;"
// #define QUERY_080702_0035		"update td_store set CurrentCount = %d, ColorCode = %d, PrefixCodeNum = %d, SuffixCodeNum = %d where UniqueNumber = %I64d;"
// 
// #define QUERY_080702_0036		"insert td_enchant(TargetItemUniqueNumber, TargetItemNum, EnchantItemNum) Values(%I64d, %d, %d);"
// #define QUERY_080702_0037		"select Accountname, AccountUniqueNumber, CharacterName from td_character WITH (NOLOCK) where accountname = \'%s\' AND 0 = Race&%d;"
// #define QUERY_080702_0038		"select Accountname, AccountUniqueNumber, CharacterName from td_character WITH (NOLOCK) where accountuniquenumber = %s AND 0 = Race&%d;"
// #define QUERY_080702_0039		"select Accountname, AccountUniqueNumber, CharacterName from td_character WITH (NOLOCK) where accountname in (select accountname from td_character where charactername = \'%s\') AND 0 = Race&%d;"
// #define QUERY_080702_0040		"select Accountname, AccountUniqueNumber, CharacterName from td_character WITH (NOLOCK) where accountname in (select accountname from td_character where uniquenumber = %s) AND 0 = Race&%d;"
// 
// #define QUERY_080702_0041		"select accounttype from atum2_db_account.dbo.td_account WITH (NOLOCK) WHERE AccountName like \'%s\';"
// #define QUERY_080702_0042		"SELECT TOP 1 c.CharacterName, c.UniqueNumber FROM td_character c, td_influencewardata iw WITH (NOLOCK) WHERE iw.InfluenceType = %d and iw.InflLeaderCharacterUID = c.UniqueNumber;"
// #define QUERY_080702_0043		"SELECT TOP 1 c.CharacterName, c.UniqueNumber FROM td_character c, td_influencewardata iw WITH (NOLOCK) WHERE iw.InfluenceType = %d and iw.InflSub1LeaderCharacterUID = c.UniqueNumber;"
// #define QUERY_080702_0044		"SELECT TOP 1 c.CharacterName, c.UniqueNumber FROM td_character c, td_influencewardata iw WITH (NOLOCK) WHERE iw.InfluenceType = %d and iw.InflSub2LeaderCharacterUID = c.UniqueNumber;"
// #define QUERY_080702_0045		"select s.UniqueNumber, i.ItemNum, s.PrefixCodeNum, s.SuffixCodeNum, s.ItemStorage, s.Wear, s.ItemWindowIndex, s.CurrentCount, s.ColorCode, s.possess, s.CreatedTime from td_store s, td_character c, atum2_db_account.dbo.ti_Item i WITH (NOLOCK) \
// 									where c.charactername = '%s'and i.ItemNum = s.ItemNum and c.accountuniquenumber = s.accountuniquenumber \
// 									and (c.uniquenumber = s.possess OR s.possess = 0 OR s.possess = -1) and (s.ItemStorage = %d OR s.ItemStorage= %d);"
// 
// #define QUERY_080702_0046		"select COUNT(*) from td_enchant WITH (NOLOCK) where TargetItemUniqueNumber = %I64d;"
// #define QUERY_080702_0047		"SELECT uniqueNumber FROM td_character WITH (NOLOCK) WHERE CharacterName = \'%s\';"
// #define QUERY_080702_0048		"update td_character set CharacterName = \'%s\', MapIndex = %d, ChannelIndex = %d, Position_X = %f, Position_Z = %f, Race = %d, InfluenceType = %d, Level = %d, Experience = %I64f, CurrentHP = %f, HP = %d, CurrentDP = %f, DP = %d, CurrentSP = %d, \
// 									SP = %d, CurrentEP = %f, EP = %d, AttackPart = %d, DefensePart = %d, FuelPart = %d, SoulPart = %d, ShieldPart = %d, DodgePart = %d, BonusStat = %d, BonusStatPoint = %d, Propensity = %d, RacingPoint = %d, WarPoint = %d, CumulativeWarPoint = %d, ArenaWin = %d, ArenaLose = %d, ArenaDisConnect = %d, PCBangTotalPlayTime = %I64d \
// 									where uniqueNumber = %d;"
// #define QUERY_080702_0049		"SELECT ItemNum FROM atum2_db_account.dbo.ti_Item WITH (NOLOCK) where ItemNum = %d;"

#define QUERY_080702_0050_1		"select top %d "
#define QUERY_080702_0050_2		" i.LogStringKO, l.Time, l.AccountName, l.BlockedType, l.StartDate, l.EndDate, l.AdminAccountName, l.BlockedReason, l.BlockedReasonForOnlyAdmin"
#define QUERY_080702_0050_3		" from atum_log_type_info i, "
#define QUERY_080702_0050_4		" l WITH (NOLOCK) where i.logtype = l.logtype "
#define QUERY_080702_0050_5		" and l.logtype = %d "
#define QUERY_080702_0050_6		" and l.accountname = \'%s\' "
#define QUERY_080702_0050_7		" and l.BlockedType = %d"
#define QUERY_080702_0050_8		" and l.AdminAccountName = \'%s\' "
#define QUERY_080702_0050_9		" AND l.Time >= '%s' AND l.Time <= '%s' "
#define QUERY_080702_0050_10	" order by l.time desc "
#define QUERY_080702_0050_11	" LIMIT 0,%d ;"		// 2008-07-08 by cmkwon, MySQL 지원 구현 - Only for MySQL

// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
// #define QUERY_080702_0051		"INSERT INTO atum2_db_account.dbo.ti_EventMonster(ServerGroupID,StartDateTime,EndDateTime,SummonerMapIndex,SummonerReqMinLevel,SummonerReqMaxLevel,SummonerExceptMonster,SummonMonsterNum,SummonMonsterCount,SummonDelayTime,SummonProbability) VALUES(%d, '%s', '%s', %d, %d, %d, %d, %d, %d, %d, %d);"
// #define QUERY_080702_0052		"UPDATE atum2_db_account.dbo.ti_EventMonster SET ServerGroupID= %d, StartDateTime= '%s', EndDateTime= '%s', SummonerMapIndex= %d, SummonerReqMinLevel= %d, SummonerReqMaxLevel= %d, SummonerExceptMonster= %d, SummonMonsterNum= %d, SummonMonsterCount= %d, SummonDelayTime= %d, SummonProbability= %d WHERE EventMonsterUID = %d;"
// #define QUERY_080702_0053		"DELETE atum2_db_account.dbo.ti_EventMonster WHERE EventMonsterUID = %d;"
// #define QUERY_080702_0054		"SELECT GuildUniqueNumber, GuildName, GuildMarkImage, GuildMarkState FROM td_guild WITH (NOLOCK) WHERE GuildMarkState = %d;"
// #define QUERY_080702_0055		"UPDATE td_guild SET GuildMarkState = %d WHERE GuildUniqueNumber = %u;"
// 
// #define QUERY_080702_0056		"SELECT * FROM atum2_db_account.dbo.ti_Item WITH (NOLOCK);"
// #define QUERY_080702_0057		"SELECT * FROM atum2_db_account.dbo.ti_EnchantInfo WITH (NOLOCK);"
// #define QUERY_080702_0058		"SELECT * FROM atum2_db_account.dbo.ti_ItemMixingInfo WITH (NOLOCK);"
// #define QUERY_080702_0059		"SELECT * FROM atum2_db_account.dbo.ti_RareItemInfo WITH (NOLOCK);"
// #define QUERY_080702_0060		"SELECT Code, Alpha, Collision, CollisionForServer, RenderIndex, ObjectRenderType FROM atum2_db_account.dbo.ti_MapObject WITH (NOLOCK);"
// 
// #define QUERY_080702_0061		"SELECT * FROM ti_BuildingNPC WITH (NOLOCK);"
// #define QUERY_080702_0062		"SELECT UniqueNumber, MonsterName, Level, Experience, HP, Race, Speed, MonsterItem01, MonsterItem02, MonsterItem03, MonsterItem04, MonsterItem05, ControlSkill, SizeForClient, Faction, \
// 									MonsterForm, AttackPattern, MovePattern, Belligerence, Defense, DefenceProbability, AlphaBlending, AttackObject, AttackRange, HPRecoveryTime, HPRecoveryValue, RenderIndex, ScaleValue, TextureIndex, SourceIndex, QuickTurnAngle, \
// 									QuickSpeed, QuickTurnTerm, MPOption, MPOptionParam1, MPOptionParam2, Range, TurnAngle, MonsterHabit, ClickEvent FROM atum2_db_account.dbo.ti_Monster WITH (NOLOCK);"
// 
// #define QUERY_080702_0063		"SELECT UniqueNumber, MonsterName, Level, Experience, HP, Race, Speed, MonsterItem01, MonsterItem02, MonsterItem03, MonsterItem04, MonsterItem05, ControlSkill, SizeForServer, Faction, \
// 									MonsterForm, AttackPattern, MovePattern, Belligerence, Defense, DefenceProbability, AlphaBlending, AttackObject, AttackRange, HPRecoveryTime, HPRecoveryValue, RenderIndex, ScaleValue, TextureIndex, SourceIndex, QuickTurnAngle, \
// 									QuickSpeed, QuickTurnTerm, MPOption, MPOptionParam1, MPOptionParam2, Range, TurnAngle, MonsterHabit, ClickEvent FROM atum2_db_account.dbo.ti_Monster WITH (NOLOCK);"
// 
// #define QUERY_080702_0064		"SELECT * FROM atum2_db_account.dbo.ti_MapInfo WITH (NOLOCK);"

// 2008-09-12 by cmkwon, 한국 Yedang_Kor PCBang 리로드 버그 수정 - 쿼리로 해야 함.
//#define QUERY_080702_0065		"Select TempIp.UID, TempIp.ST_IP, TempIp.ED_IP, TempBr.User_Id,	\
// 									TempBr.Branch_Name, TempBr.Branch_RegNO, TempBr.Branch_Tel, \
// 									TempBr.ZipCode, TempBr.Addr_Sido, TempBr.Addr_SiGugun, TempBr.Addr_Dong, \
// 									TempBr.Addr_Detail, TempBr.User_Level \
// 									From 	(Select MAX(BSeq) As UID, ST_IP, MAX(ED_IP)As ED_IP \
// 									From pc_mem_branch_ip \
// 									where ST_IP <> ' ' AND ST_IP NOT LIKE '%..%' \
// 									Group by ST_IP \
// 									) As TempIp  \
// 									Inner Join \
// 									(select TempBranch.BSeq, TempBranch.User_Id,	\
// 									TempBranch.Branch_Name, 	TempBranch.Branch_RegNO,	 TempBranch.Branch_Tel, \
// 									TempBranch.ZipCode,		 TempBranch.Addr_Sido, 		TempBranch.Addr_SiGugun,	 TempBranch.Addr_Dong, \
// 									TempBranch.Addr_Detail, 		TempMember.User_Level  \
// 									from \
// 									(select * from pc_mem_branch) As TempBranch Inner join	\
// 									(select User_Id, User_Level  from pc_member Where  pc_member.User_Level >= 3) As TempMember	\
// 									On TempBranch.User_Id = TempMember.User_Id) As TempBr \
// 									On TempIp.UID = TempBr.BSeq;"
 
// #define QUERY_080702_0066		"DELETE atum2_db_account.dbo.td_pcbang;"
//#define QUERY_080702_0067		"INSERT INTO atum2_db_account.dbo.td_pcbang VALUES(?,?,?,?,?, ?,?,?,?,?, ?,?,?);"

// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
// #define QUERY_080702_0068		"SELECT InfluenceType, InflLeaderCharacterUID FROM td_influencewardata WITH (NOLOCK) WHERE MGameServerID = %d;"
// #define QUERY_080702_0069		"SELECT InfluenceType, InflSub1LeaderCharacterUID FROM td_influencewardata WITH (NOLOCK) WHERE MGameServerID = %d;"
// #define QUERY_080702_0070		"SELECT InfluenceType, InflSub2LeaderCharacterUID FROM td_influencewardata WITH (NOLOCK) WHERE MGameServerID = %d;"
// 
// #define QUERY_080702_0071		"SELECT InfluenceType, COUNT(*) FROM td_character WITH (NOLOCK) WHERE LastStartedTime > DATEADD(day, %d, GETDATE()) GROUP BY InfluenceType;"
// #define QUERY_080702_0072		"SELECT * FROM atum2_db_account.dbo.ti_InfluenceWar WITH (NOLOCK);"
// #define QUERY_080702_0073		"SELECT EventMonsterUID, ServerGroupID, StartDateTime, EndDateTime, SummonerMapIndex, SummonerReqMinLevel, SummonerReqMaxLevel, SummonerExceptMonster, SummonMonsterNum, SummonMonsterCount, SummonDelayTime, SummonProbability FROM atum2_db_account.dbo.ti_EventMonster WITH (NOLOCK);"
// #define QUERY_080702_0074		"select accounttype from atum2_db_account.dbo.td_account WITH (NOLOCK) where accountname like \'%s\' and (password like \'%s\' OR password like \'%s\');"
// #define QUERY_080702_0075		"SELECT wd.InfluenceType, wd.MGameServerID, wd.WartimeStage, wd.ContributionPoint, wd.InflLeaderCharacterUID, c.CharacterName, wd.OwnerOfConflictArea, wd.InflSub1LeaderCharacterUID, c1.CharacterName, wd.InflSub2LeaderCharacterUID, c2.CharacterName, wd.MSWarOptionType \
// 									FROM td_influencewardata wd  LEFT OUTER JOIN td_character c \
// 									ON wd.InflLeaderCharacterUID = c.uniquenumber	\
// 									LEFT OUTER JOIN td_character c1	\
// 									ON wd.InflSub1LeaderCharacterUID = c1.uniquenumber \
// 									LEFT OUTER JOIN td_character c2 \
// 									ON wd.InflSub2LeaderCharacterUID = c2.uniquenumber;"
// 
// #define QUERY_080702_0076		"INSERT INTO td_influencewardata VALUES (%d, %d, %d, %d, 0, 0, 0, 0, 510);"
// #define QUERY_080702_0077		"SELECT g.GuildUniqueNumber FROM td_guild g WITH (NOLOCK), td_character c WITH (NOLOCK) WHERE c.UniqueNumber = %d AND g.GuildUniqueNumber = c.GuildUniqueNumber AND g.GuildCommanderUniqueNumber = c.UniqueNumber;"
// #define QUERY_080702_0078		"SELECT g.GuildMemberCapacity FROM td_guild g, td_character c WITH (NOLOCK) WHERE c.UniqueNumber = %d AND g.GuildUniqueNumber = c.GuildUniqueNumber AND g.GuildCommanderUniqueNumber = c.UniqueNumber;"
// #define QUERY_080702_0079		"SELECT * FROM td_guild WITH (NOLOCK) WHERE GuildUniqueNumber = %d;"
// #define QUERY_080702_0080		"SELECT CharacterUniqueNumber FROM td_guildmember WITH (NOLOCK) WHERE GuildUniqueNumber = %d;"
// 
// #define QUERY_080702_0081		"SELECT GuildCommanderUniqueNumber FROM td_guild WITH (NOLOCK) WHERE GuildUniqueNumber = %d;"
// #define QUERY_080702_0082		"SELECT GuildUniqueNumber FROM td_character	 WHERE UniqueNumber = %u;"
// #define QUERY_080702_0083		"SELECT GuildState FROM td_guild WITH (NOLOCK) WHERE GuildUniqueNumber = %d;"
// #define QUERY_080702_0084		"SELECT CharacterName FROM td_character WITH (NOLOCK) WHERE UniqueNumber = (SELECT GuildCommanderUniqueNumber FROM td_guild WITH (NOLOCK) WHERE GuildUniqueNumber = %d);"
// #define QUERY_080702_0085		"SELECT o.MapIndex, g.GuildUniqueNumber, g.GuildName FROM td_MapOwnership o, td_guild g WITH (NOLOCK) WHERE o.MapIndex = %d AND g.GuildUniqueNumber = o.OwnerGuildUniqueNumber;"
// 
// #define QUERY_080702_0086		"SELECT g.GuildName FROM td_GuildWar gw, td_guild g WITH (NOLOCK) WHERE g.GuildUniqueNumber = gw.DefenderGuildUniqueNumber AND g.GuildUniqueNumber = %d;"
// #define QUERY_080702_0087		"UPDATE atum2_db_account.dbo.td_account SET ConnectingServerGroupID = 0;"
#define QUERY_080702_0088		"delete atum2_db_account.dbo.ti_Item where ItemNum = %s;"
#define QUERY_080702_0089		"Update atum2_db_account.dbo.ti_Item set kind = %s, ItemName = '%s', abilitymin = %s, abilitymax = %s, reqrace = %s, reqattackpart = %s, reqdefensepart = %s, ReqFuelPart = %s, reqsoulpart = %s, reqshieldpart = %s, reqdodgepart = %s, requnitkind = %s, reqminlevel = %s, reqmaxlevel = %s, weight = %s, range = %s, position = %s, scarcity = %s, mintradequantity = %s, price = %s, cashprice = %s, [time] = %s, rangeangle = %s, boosterangle = %s, ItemAttribute = %s, [description] = '%s', SourceIndex = %s where ItemNum = %s;"
#define QUERY_080702_0090		"Insert into atum2_db_account.dbo.ti_Item(ItemNum, kind, ItemName, abilitymin, abilitymax, reqrace, reqattackpart, reqdefensepart, ReqFuelPart, reqsoulpart, reqshieldpart, reqdodgepart, requnitkind, reqminlevel, reqmaxlevel, weight, range, position, scarcity, mintradequantity, price, cashprice, [time], rangeangle, boosterangle, ItemAttribute, [description], SourceIndex) values ( %s, %s, '%s', %s, %s,%s, %s,%s, %s,%s, %s,%s, %s,%s, %s,%s, %s,%s, %s,%s,%s, %s,%s,%s,%s, %s, '%s', %s);"

#define QUERY_080702_0091		"select ItemNum, kind, ItemName, abilitymin, abilitymax, reqrace, reqattackpart, reqdefensepart, ReqFuelPart, reqsoulpart, reqshieldpart, reqdodgepart, requnitkind, reqminlevel, reqmaxlevel, weight, range, position, scarcity, mintradequantity, price, cashprice, [time], rangeangle, boosterangle, ItemAttribute, description, SourceIndex from atum2_db_account.dbo.ti_Item where kind = %d;"
#define QUERY_080702_0091_B		"select ItemNum, kind, ItemName, abilitymin, abilitymax, reqrace, reqattackpart, reqdefensepart, ReqFuelPart, reqsoulpart, reqshieldpart, reqdodgepart, requnitkind, reqminlevel, reqmaxlevel, weight, range, position, scarcity, mintradequantity, [time], rangeangle, boosterangle, ItemAttribute, description, SourceIndex from atum2_db_account.dbo.ti_Item where kind = %d;"

#define QUERY_080702_0092_1_A	"select top %d "
#define QUERY_080702_0092_1_B	"select "
#define QUERY_080702_0092_2		" l.accountname, l.Time, i.LogStringKO, l.IPAddress, l.ServerName"
#define QUERY_080702_0092_3		" from atum_log_type_info i, "
#define QUERY_080702_0092_4		" l WITH (NOLOCK) where i.logtype = l.logtype "
#define QUERY_080702_0092_5		" and l.logtype = %d "
#define QUERY_080702_0092_6		" and l.accountname = \'%s\' "
#define QUERY_080702_0092_7		" and l.time > \'%s\' and l.time < \'%s\' "
#define QUERY_080702_0092_8		" and CAST(CAST(substring(IPAddress, 1, 1) AS INT) AS VARCHAR(3)) + '.' + CAST(CAST(substring(IPAddress, 2, 1) AS INT) AS VARCHAR(3)) + '.' + CAST(CAST(substring(IPAddress, 3, 1) AS INT) AS VARCHAR(3)) + '.' + CAST(CAST(substring(IPAddress, 4, 1) AS INT) AS VARCHAR(3)) like \'%s\'"
#define QUERY_080702_0092_9		" order by l.time desc "
#define QUERY_080702_0092_10_A	" LIMIT 0,%d;"		// 2008-07-08 by cmkwon, MySQL 지원 구현 - Only for MySql
#define QUERY_080702_0092_10_B	" ;"				// 2008-07-08 by cmkwon, MySQL 지원 구현 - Only for MySql

#define QUERY_080702_0093_1_A	"select top %d "
#define QUERY_080702_0093_1_B	"select "
#define QUERY_080702_0093_2		" l.* "
#define QUERY_080702_0093_3		" from atum_log_type_info i, "
#define QUERY_080702_0093_4		" l WITH (NOLOCK) where i.logtype = l.logtype "
#define QUERY_080702_0093_5		" and l.logtype = %d "
#define QUERY_080702_0093_6		" and l.AccountName = \'%s\' "
#define QUERY_080702_0093_7		" and l.CharacterName = \'%s\' "
#define QUERY_080702_0093_8		" and l.time > \'%s\' and l.time < \'%s\' "
#define QUERY_080702_0093_9		" and l.GuildName = \'%s\' "
#define QUERY_080702_0093_10	" and l.GuildUID = %d "
#define QUERY_080702_0093_11	" and l.ItemNum = %d "
#define QUERY_080702_0093_12	" and l.ItemUID = %d "
#define QUERY_080702_0093_13	" order by time desc "
#define QUERY_080702_0093_14_A	" LIMIT 0,%d;"		// 2008-07-08 by cmkwon, MySQL 지원 구현 - Only for MySql
#define QUERY_080702_0093_14_B	" ;"				// 2008-07-08 by cmkwon, MySQL 지원 구현 - Only for MySql

#define QUERY_080702_0094_1_A	"select top %d "
#define QUERY_080702_0094_1_B	"select "
#define QUERY_080702_0094_2		" c.accountname, c.charactername, l.Time, i.LogStringKO, ii.ItemNum, ii.ItemName, l.ItemUniqueNumber, "
#define QUERY_080702_0094_3		" from atum_log_type_info i, "
#define QUERY_080702_0094_4		" l, atum2_db_account.dbo.ti_Item ii, td_character c WITH (NOLOCK) where i.logtype = l.logtype and l.ItemNum = ii.ItemNum and c.uniquenumber = l.characteruniquenumber "
#define QUERY_080702_0094_5		" and l.logtype = %d "
#define QUERY_080702_0094_6_A	" and c.accountname = \'%s\' "
#define QUERY_080702_0094_6_B	" and (c.accountname = \'%s\' or c.accountname = \'%s\') "
#define QUERY_080702_0094_7_A	" and ((c.charactername = \'%s\' and l.PeerCharacterName = \'%s\') or (c.charactername = \'%s\' and l.PeerCharacterName = \'%s\')) "
#define QUERY_080702_0094_7_B	" and c.charactername = \'%s\' "
#define QUERY_080702_0094_8		" and l.time > \'%s\' and l.time < \'%s\' "
#define QUERY_080702_0094_9		" and l.ItemNum = %d "
#define QUERY_080702_0094_10	" and l.ItemUniqueNumber = %d "
#define QUERY_080702_0094_11	" order by l.time desc "
#define QUERY_080702_0094_12_A	" LIMIT 0,%d;"		// 2008-07-08 by cmkwon, MySQL 지원 구현 - Only for MySql
#define QUERY_080702_0094_12_B	" ;"				// 2008-07-08 by cmkwon, MySQL 지원 구현 - Only for MySql

#define QUERY_080702_0095_1_A	"select top %d "
#define QUERY_080702_0095_1_B	"select "
#define QUERY_080702_0095_2_A	" l.accountname, l.charactername, l.Time, i.LogStringKO, l.MapIndex, l.Position_X, l.Position_Y, l.Position_Z, "
#define QUERY_080702_0095_2_B	" c.accountname, c.charactername, l.Time, i.LogStringKO, l.MapIndex, l.Position_X, l.Position_Y, l.Position_Z, "
#define QUERY_080702_0095_3		" from atum_log_type_info i, "
#define QUERY_080702_0095_4_A	" l WITH (NOLOCK) where i.logtype = l.logtype "
#define QUERY_080702_0095_4_B	" l, td_character c WITH (NOLOCK) where i.logtype = l.logtype and c.uniquenumber = l.characteruniquenumber "
#define QUERY_080702_0095_5		" and l.logtype = %d "
#define QUERY_080702_0095_6_A	" and l.accountname = \'%s\' "
#define QUERY_080702_0095_6_B	" and c.accountname = \'%s\' "
#define QUERY_080702_0095_7_A	" and l.charactername = \'%s\' "
#define QUERY_080702_0095_7_B	" and c.charactername = \'%s\' "
#define QUERY_080702_0095_8		" and l.time > \'%s\' and l.time < \'%s\' "
#define QUERY_080702_0095_9		" order by l.time desc "
#define QUERY_080702_0095_10_A	" LIMIT 0,%d;"		// 2008-07-08 by cmkwon, MySQL 지원 구현 - Only for MySql
#define QUERY_080702_0095_10_B	" ;"				// 2008-07-08 by cmkwon, MySQL 지원 구현 - Only for MySql

#define QUERY_080702_0096		"select MapIndex, RenderMapIndex, MapName, MapInfluenceType, CityMapIndex, CityWarQuestIndex, CityWarQuestNPCIndex, CityWarQuestMonsterUID, MaxUserCount, MaxGroundHeight, MinGroundHeight, WaterHeight, UserVisibleDistance, MonsterVisibleDistance, QuestIndexForWarp, FrontPositionDistance, AutoRecoveryFlag, DefaultMapIndex, CityWarpTargetPositionX, CityWarpTargetPositionY, CityWarpTargetPositionZ, ClientMaxAltitudeHeight, DayFogColor, NightFogColor, DayFogStartDistance, DayFogEndDistance, NightFogStartDistance, NightFogEndDistance, DaySunDirectionX, DaySunDirectionY, DaySunDirectionZ, NightSunDirectionX, NightSunDirectionY, NightSunDirectionZ, WaterType, PKZoneFlag, TileRenderingFlag, SkyRenderingFlag, SunRenderingFlag, FogRenderingFlag from atum2_db_account.dbo.ti_MapInfo;"
#define QUERY_080702_0097		"delete atum2_db_account.dbo.ti_MapInfo where MapIndex = %s;"
#define QUERY_080702_0098		"Update atum2_db_account.dbo.ti_MapInfo set RenderMapIndex = %s, MapName = '%s', MapInfluenceType = %s, CityMapIndex = %s, CityWarQuestIndex = %s, CityWarQuestNPCIndex = %s, CityWarQuestMonsterUID = %s, MaxUserCount = %s, MaxGroundHeight = %s, MinGroundHeight = %s, WaterHeight = %s, UserVisibleDistance = %s, MonsterVisibleDistance = %s, QuestIndexForWarp = %s, FrontPositionDistance = %s, AutoRecoveryFlag = %s, DefaultMapIndex = %s, CityWarpTargetPositionX = %s, CityWarpTargetPositionY = %s, CityWarpTargetPositionZ = %s, ClientMaxAltitudeHeight = %s, DayFogColor = %s, NightFogColor = %s, DayFogStartDistance = %s, DayFogEndDistance = %s, NightFogStartDistance = %s, NightFogEndDistance = %s, DaySunDirectionX = %s, DaySunDirectionY = %s, DaySunDirectionZ = %s, NightSunDirectionX = %s, NightSunDirectionY = %s, NightSunDirectionZ = %s, WaterType = %s, PKZoneFlag = %s, TileRenderingFlag = %s, SkyRenderingFlag = %s, SunRenderingFlag = %s, FogRenderingFlag = %s where MapIndex = %s;"
#define QUERY_080702_0099		"Insert into atum2_db_account.dbo.ti_MapInfo(MapIndex, RenderMapIndex, MapName, MapInfluenceType, CityMapIndex, CityWarQuestIndex, CityWarQuestNPCIndex, CityWarQuestMonsterUID, MaxUserCount, MaxGroundHeight, MinGroundHeight, WaterHeight, UserVisibleDistance, MonsterVisibleDistance, QuestIndexForWarp, FrontPositionDistance, AutoRecoveryFlag, DefaultMapIndex, CityWarpTargetPositionX, CityWarpTargetPositionY, CityWarpTargetPositionZ, ClientMaxAltitudeHeight, DayFogColor, NightFogColor, DayFogStartDistance, DayFogEndDistance, NightFogStartDistance, NightFogEndDistance, DaySunDirectionX, DaySunDirectionY, DaySunDirectionZ, NightSunDirectionX, NightSunDirectionY, NightSunDirectionZ, WaterType, PKZoneFlag, TileRenderingFlag, SkyRenderingFlag, SunRenderingFlag, FogRenderingFlag) values ( %s, %s, '%s', %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s);"

// 2009-04-21 by cmkwon, ITEM에 DesParam 필드 개수 8개로 늘리기 - 현재 사용하지 않으므로 주석 처리
//#define QUERY_080702_0100		"select ItemNum, ItemName, AbilityMin, AbilityMax, ReqRace, ReqMinLevel, Hitrate, SpeedPenalty, Range, Charging, RangeAngle, Shotnum, Multinum, Attacktime, ReqSP, Summonmonster, Orbittype, CameraPattern, Kind, SourceIndex, DestParameter1, ParameterValue1, DestParameter2, ParameterValue2, DestParameter3, ParameterValue3, DestParameter4, ParameterValue4 from atum2_db_account.dbo.ti_Item where ItemNum like '75%%';"
//
//#define QUERY_080702_0101		"select ItemNum, ItemName, AbilityMin, AbilityMax, ReqRace, ReqMinLevel, Hitrate, SpeedPenalty, Range, Charging, RangeAngle, Shotnum, Multinum, Attacktime, ReqSP, Summonmonster, Orbittype, CameraPattern, Kind, SourceIndex, DestParameter1, ParameterValue1, DestParameter2, ParameterValue2, DestParameter3, ParameterValue3, DestParameter4, ParameterValue4 from atum2_db_account.dbo.ti_Item where ItemNum like '75%%' and ItemNum in (%d, %d, %d, %d, %d, %d);"
//#define QUERY_080702_0102		"delete atum2_db_account.dbo.ti_Item where ItemNum = %s;"
//#define QUERY_080702_0103		"Update atum2_db_account.dbo.ti_Item set ItemName='%s', AbilityMin=%s, AbilityMax=%s, ReqRace=%s, ReqMinLevel=%s, Hitrate=%s, SpeedPenalty=%s, Range=%s, Charging=%s, RangeAngle=%s, Shotnum=%s, Multinum=%s, Attacktime=%s, ReqSP=%s, Summonmonster=%s, Orbittype=%s, Kind=%s, CameraPattern=%s, SourceIndex = %s, DestParameter1 = %s, ParameterValue1 = %s, DestParameter2 = %s, ParameterValue2 = %s, DestParameter3 = %s, ParameterValue3 = %s, DestParameter4 = %s, ParameterValue4 = %s where ItemNum = %s;"
//#define QUERY_080702_0104		"Insert into atum2_db_account.dbo.ti_Item(ItemNum, ItemName, AbilityMin, AbilityMax, ReqRace, ReqMinLevel, Hitrate, SpeedPenalty, Range, Charging, RangeAngle, Shotnum, Multinum, Attacktime, ReqSP, Summonmonster, Orbittype, Kind, CameraPattern, SourceIndex, DestParameter1, ParameterValue1, DestParameter2, ParameterValue2, DestParameter3, ParameterValue3, DestParameter4, ParameterValue4) values ( %s, '%s', %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s);"
//#define QUERY_080702_0105		"select ItemNum from atum2_db_account.dbo.ti_Item where ItemNum like '75%%';"
//
//#define QUERY_080702_0106		"select ItemNum, ItemName, AbilityMin, AbilityMax, ReqRace, ReqMinLevel, Hitrate, SpeedPenalty, Range, Charging, RangeAngle, Shotnum, Multinum, Attacktime, ReqSP, Summonmonster, Orbittype, CameraPattern, Kind, SourceIndex, DestParameter1, ParameterValue1, DestParameter2, ParameterValue2, DestParameter3, ParameterValue3, DestParameter4, ParameterValue4 from atum2_db_account.dbo.ti_Item where ItemNum like '75%%' and ItemNum in (%s);"
//#define QUERY_080702_0107		"select ItemNum, ItemName, AbilityMin, AbilityMax, ReqRace, ReqMinLevel, Hitrate, SpeedPenalty, Range, Charging, RangeAngle, Shotnum, Multinum, Attacktime, ReqSP, Summonmonster, Orbittype, Kind, SourceIndex, DestParameter1, ParameterValue1, DestParameter2, ParameterValue2, DestParameter3, ParameterValue3, DestParameter4, ParameterValue4 from atum2_db_account.dbo.ti_Item where ItemNum like '75%%' and ItemNum in (%s);"
//#define QUERY_080702_0108		"select ItemNum, ItemName, AbilityMin, AbilityMax, ReqRace, ReqMinLevel, Hitrate, SpeedPenalty, Range, Charging, RangeAngle, Shotnum, Multinum, Attacktime, ReqSP, Summonmonster, Orbittype, Kind, SourceIndex, DestParameter1, ParameterValue1, DestParameter2, ParameterValue2, DestParameter3, ParameterValue3, DestParameter4, ParameterValue4 from atum2_db_account.dbo.ti_Item where ItemNum like '75%%' and ItemNum in (%s);"
//#define QUERY_080702_0109		"select ItemNum, ItemName, AbilityMin, AbilityMax, ReqRace, ReqMinLevel, Hitrate, SpeedPenalty, Range, Charging, RangeAngle, Shotnum, Multinum, Attacktime, ReqSP, Summonmonster, Orbittype, Kind, SourceIndex, DestParameter1, ParameterValue1, DestParameter2, ParameterValue2, DestParameter3, ParameterValue3, DestParameter4, ParameterValue4 from atum2_db_account.dbo.ti_Item where ItemNum like '75%%' and ItemNum in (%s);"
//#define QUERY_080702_0110		"select ItemNum, ItemName, AbilityMin, AbilityMax, ReqRace, ReqMinLevel, Hitrate, SpeedPenalty, Range, Charging, RangeAngle, Shotnum, Multinum, Attacktime, ReqSP, Summonmonster, Orbittype, Kind, SourceIndex, DestParameter1, ParameterValue1, DestParameter2, ParameterValue2, DestParameter3, ParameterValue3, DestParameter4, ParameterValue4 from atum2_db_account.dbo.ti_Item where ItemNum like '75%%' and ItemNum in (%s);"

#define QUERY_080702_0111		"select UniqueNumber, MonsterName, Level, Experience, HP, Race, Speed, MonsterItem01, MonsterItem02, MonsterItem03, MonsterItem04, MonsterItem05, SizeForServer, SizeForClient, MonsterForm, AttackPattern, MovePattern, Belligerence, Defense, DefenceProbability, AlphaBlending, AttackObject, AttackRange, HPRecoveryTime, HPRecoveryValue, ScaleValue, TextureIndex, SourceIndex, QuickTurnAngle, QuickSpeed, QuickTurnTerm, MPOption, MPOptionParam1, MPOptionParam2, Range, TurnAngle  from atum2_db_account.dbo.ti_Monster;"
#define QUERY_080702_0112		"delete atum2_db_account.dbo.ti_Monster where uniquenumber = %s;"
#define QUERY_080702_0113		"Update atum2_db_account.dbo.ti_Monster set MonsterName= '%s', Level= %s, Experience= %s, HP= %s, Race= %s, Speed= %s, MonsterItem01= %s, MonsterItem02= %s, MonsterItem03= %s, MonsterItem04= %s, MonsterItem05= %s, SizeForServer= %s, SizeForClient= %s, MonsterForm= %s, AttackPattern= %s, MovePattern= %s, Belligerence= %s, Defense= %s, DefenceProbability= %s, AlphaBlending= %s, AttackObject= %s, AttackRange= %s, HPRecoveryTime= %s, HPRecoveryValue= %s, ScaleValue= %s, TextureIndex= %s, SourceIndex= %s, QuickTurnAngle= %s, QuickSpeed= %s, QuickTurnTerm= %s, MPOption= %s, MPOptionParam1= %s, MPOptionParam2= %s, Range= %s, TurnAngle= %s where uniquenumber = %s;"
#define QUERY_080702_0114		"Insert into atum2_db_account.dbo.ti_Monster(UniqueNumber, MonsterName, Level, Experience, HP, Race, Speed, MonsterItem01, MonsterItem02, MonsterItem03, MonsterItem04, MonsterItem05, SizeForServer, SizeForClient, MonsterForm, AttackPattern, MovePattern, Belligerence, Defense, DefenceProbability, AlphaBlending, AttackObject, AttackRange, HPRecoveryTime, HPRecoveryValue, ScaleValue, TextureIndex, SourceIndex, QuickTurnAngle, QuickSpeed, QuickTurnTerm, MPOption, MPOptionParam1, MPOptionParam2, Range, TurnAngle) values ( %s, '%s', %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s);"
#define QUERY_080702_0115		"select MonsterUniqueNumber ,ItemNum ,MinCount ,MaxCount ,Probability ,PrefixProbability ,SuffixProbability  from atum2_db_account.dbo.ti_MonsterItem;"

#define QUERY_080702_0116		"select MonsterUniqueNumber ,ItemNum ,MinCount ,MaxCount ,Probability ,PrefixProbability ,SuffixProbability  from atum2_db_account.dbo.ti_MonsterItem where monsteruniquenumber = %d;"
#define QUERY_080702_0117		"delete atum2_db_account.dbo.ti_MonsterItem where MonsterUniqueNumber = %s;"
#define QUERY_080702_0118		"Update atum2_db_account.dbo.ti_MonsterItem set ItemNum =%s,MinCount=%s ,MaxCount=%s ,Probability=%s ,PrefixProbability=%s ,SuffixProbability=%s where monsteruniquenumber = %s;"
#define QUERY_080702_0119		"Insert into atum2_db_account.dbo.ti_MonsterItem(MonsterUniqueNumber ,ItemNum ,MinCount ,MaxCount ,Probability ,PrefixProbability ,SuffixProbability) values ( %s, %s, %s, %s, %s, %s, %s);"

// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
// #define QUERY_080702_0120		"select accounttype from atum2_db_account.dbo.td_account where accountname = '%s' and password = '%s';"
// 
// #define QUERY_080702_0121		"select accounttype from atum2_db_account.dbo.td_account where accountname = '%s' and password = '%s';"

#define QUERY_080702_0122		"drop view dbo.game_log_start_end;"
#define QUERY_080702_0123		"Create View dbo.game_log_start_end \
									as \
									select Time, LogType, CharacterUniqueNumber, AccountName, CharacterName, TotalPlayTime, PlayTime, PCBangUID \
									from dbo.atum_backup_log_user_game_start_end WITH (NOLOCK) \
									where Time >= '%s' and Time <= '%s' \
									union \
									select Time, LogType, CharacterUniqueNumber, AccountName, CharacterName, TotalPlayTime, PlayTime, PCBangUID \
									from dbo.atum_log_user_game_start_end WITH (NOLOCK) \
									where Time >= '%s' and Time <= '%s' "
#define QUERY_080702_0124		"Create View dbo.game_log_start_end \
									as \
									select Time, LogType, CharacterUniqueNumber, AccountName, CharacterName, TotalPlayTime, PCBangPlayTime AS PlayTime, PCBangUID \
									from dbo.atum_backup_log_user_game_start_end WITH (NOLOCK) \
									where Time >= '%s' and Time <= '%s' \
									and PCBangUID IN \
									(select UID from atum2_db_account.dbo.td_pcbang WITH (NOLOCK) \
									where Addr_Sido like '%%%s%%' and Addr_SiGuGun like '%%%s%%' and Branch_Name like '%%%s%%') \
									union \
									select Time, LogType, CharacterUniqueNumber, AccountName, CharacterName, TotalPlayTime, PCBangPlayTime AS PlayTime, PCBangUID \
									from dbo.atum_log_user_game_start_end WITH (NOLOCK) \
									where Time >= '%s' and Time <= '%s' \
									and PCBangUID IN \
									(select UID from atum2_db_account.dbo.td_pcbang WITH (NOLOCK) \
									where Addr_Sido like '%%%s%%' and Addr_SiGuGun like '%%%s%%' and Branch_Name like '%%%s%%') "
// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
// #define QUERY_080702_0125		"select a.AccountName, a.CharacterName, c.Level, a.START_TIME, b.END_TIME, b.PlayTime, b.PCBangUID, c.CreatedTime \
// 									from \
// 									(select CharacterUniqueNumber, AccountName, CharacterName, Time as START_TIME, TotalPlayTime \
// 									from dbo.game_log_start_end \
// 									where LogType = 19) as a, \
// 									(select CharacterUniqueNumber, AccountName, CharacterName, Time as END_TIME, TotalPlayTime, PlayTime, PCBangUID \
// 									from dbo.game_log_start_end \
// 									where LogType = 20) as b, \
// 									dbo.td_character as c \
// 									where 0 = c.Race&%d AND a.CharacterUniqueNumber = b.CharacterUniqueNumber  \
// 									and (b.TotalPlayTime - a.TotalPlayTime) = PlayTime  \
// 									and a.CharacterUniqueNumber = c.UniqueNumber \
// 									and a.START_TIME < b.END_TIME \
// 									order by  a.START_TIME;"

#define QUERY_080702_0126		"use atum2_db_%d;"
#define QUERY_080702_0127		"drop view view_pcbangplaytime;"
#define QUERY_080702_0128		"Create View dbo.view_pcbangplaytime \
									as \
									select sum(PCBangPlayTime) AS TotalPCBangPlayTime, PCBangUID  \
									from dbo.atum_log_user_game_start_end WITH (NOLOCK) where LogType = 20 and Time >= '%s' and Time <= '%s' \
									group by PCBangUID having PCBangUID IS NOT NULL \
									UNION \
									select sum(PCBangPlayTime) AS TotalPCBangPlayTime, PCBangUID  \
									from dbo.atum_backup_log_user_game_start_end WITH (NOLOCK) where LogType = 20 and Time >= '%s' and Time <= '%s'\
									group by PCBangUID having PCBangUID IS NOT NULL "

// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
// #define QUERY_080702_0129		"select TotalTime.PCBangUID, PCBang.Branch_Name, TotalTime.TotalPCBangPlayTime, \
// 									PCBang.Branch_Tel, PCBang.ZipCode, PCBang.Addr_Sido, PCBang.Addr_SiGuGun, \
// 									PCBang.Addr_Dong, PCBang.Addr_Detail, PCBang.User_Level \
// 									from \
// 									dbo.view_pcbangplaytime AS TotalTime INNER JOIN atum2_db_account.dbo.td_pcbang AS PCBang \
// 									on TotalTime.PCBangUID = PCBang.UID;"

#define QUERY_080702_0130		"use atum2_db_%d;"

#define QUERY_080702_0131		"drop view dbo.log_pcbangtotalplaytimebyeachday;"
#define QUERY_080702_0132		"Create View dbo.log_pcbangtotalplaytimebyeachday	\
									as \
									select Time, PCBangUID , PCBangPlayTime \
									from dbo.atum_backup_log_user_game_start_end WITH (NOLOCK) \
									where LogType = 20 and Time >= '%s' and Time <= '%s' \
									and PCBangUID IN \
									(select UID from atum2_db_account.dbo.td_pcbang WITH (NOLOCK)) \
									UNION \
									select Time, PCBangUID , PCBangPlayTime \
									from dbo.atum_log_user_game_start_end WITH (NOLOCK) \
									where LogType = 20 and Time >= '%s' and Time <= '%s' \
									and PCBangUID IN \
									(select UID from atum2_db_account.dbo.td_pcbang WITH (NOLOCK)) "

// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
// #define QUERY_080702_0133		"Select Time, PCBangUID, PCBangPlayTime From dbo.log_pcbangtotalplaytimebyeachday WITH (NOLOCK);"
// #define QUERY_080702_0134		"SELECT TOP %d accountname, charactername, dbo.atum_GetGearString(unitkind), InfluenceType, level, WarPoint, CumulativeWarPoint, ArenaWin, ArenaLose, ArenaDisConnect \
// 									FROM td_character WITH (NOLOCK) \
// 									WHERE 0 = Race&%d AND race < 128 \
// 									ORDER BY %s DESC;"
// #define QUERY_080702_0135		"SELECT dbo.atum_getonlydate(RegisteredDate), count(*) \
// 									FROM atum2_db_account.dbo.td_account WITH (NOLOCK) \
// 									WHERE RegisteredDate >= '%s' and RegisteredDate <= '%s' and accounttype = 0 \
// 									GROUP BY dbo.atum_getonlydate(RegisteredDate) \
// 									ORDER BY dbo.atum_getonlydate(RegisteredDate);"
// 
// #define QUERY_080702_0136		"SELECT dbo.atum_getonlydate(RegisteredDate), count(*) \
// 									FROM atum2_db_account.dbo.td_account WITH (NOLOCK) \
// 									WHERE RegisteredDate >= '%s' and RegisteredDate <= '%s' and accounttype = 0 \
// 									and AccountUniqueNumber in (select DISTINCT a.accountuniquenumber from atum2_db_account.dbo.td_account a, td_character c where a.RegisteredDate >= '%s' and a.RegisteredDate <='%s' and a.AccountuniqueNumber = c.AccountUniqueNumber) \
// 									GROUP BY dbo.atum_getonlydate(RegisteredDate) \
// 									ORDER BY dbo.atum_getonlydate(RegisteredDate);"
// #define QUERY_080702_0137		"select count(*) from atum2_db_account.dbo.td_account WITH (NOLOCK) where lastlogindate < dateadd(ww, -2, getdate());"
// #define QUERY_080702_0138		"select birthyear, count(*) from atum2_db_account.dbo.td_account WITH (NOLOCK) where registereddate >= '%s' group by birthyear order by birthyear;"

#define QUERY_080702_0139		"DROP VIEW log_server_map;"
#define QUERY_080702_0140		"CREATE VIEW log_server_map AS \
									SELECT time, clientCount \
									FROM atum_backup_log_server_map WITH (NOLOCK) \
									WHERE MapIndex = 0 and time > '%s' and time < '%s' \
									UNION ALL \
									SELECT time, clientCount \
									FROM atum_log_server_map WITH (NOLOCK) \
									WHERE MapIndex = 0 and time > '%s' and time < '%s' "

// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
// #define QUERY_080702_0141		"SELECT time, clientCount FROM log_server_map ORDER BY time DESC;"
// #define QUERY_080702_0142		"SELECT TOP 1 (AccountUniqueNumber) FROM atum2_db_account.dbo.td_account WITH (NOLOCK) ORDER BY AccountUniqueNumber DESC;"
// #define QUERY_080702_0143		"SELECT COUNT(*) FROM atum2_db_account.dbo.td_account WITH (NOLOCK);"
// #define QUERY_080702_0144		"SELECT ((SELECT TOP 1 (AccountUniqueNumber) FROM atum2_db_account.dbo.td_account WITH (NOLOCK) ORDER BY AccountUniqueNumber DESC) - COUNT(*)) FROM atum2_db_account.dbo.td_account;"
// #define QUERY_080702_0145		"SELECT (SELECT TOP 1 (AccountUniqueNumber) FROM atum2_db_account.dbo.td_account WITH (NOLOCK) ORDER BY AccountUniqueNumber DESC) - ((SELECT TOP 1 (AccountUniqueNumber) FROM atum2_db_account.dbo.td_account ORDER BY AccountUniqueNumber DESC) - COUNT(*))/datediff(dd, min(registereddate), getdate()) AS '현 잠정 회원수' FROM atum2_db_account.dbo.td_account;"

#define QUERY_080702_0146		"DROP VIEW user_count_timeline;"
#define QUERY_080702_0147		"DROP VIEW tm_user_count_timeline;"
#define QUERY_080702_0148		"CREATE VIEW tm_user_count_timeline AS \
									SELECT * FROM atum_backup_log_server_map WITH (NOLOCK) where MapIndex = 0 and time > '%s' and time < '%s' \
									UNION ALL \
									SELECT * FROM atum_log_server_map WITH (NOLOCK) where MapIndex = 0 and time > '%s' and time < '%s' "
#define QUERY_080702_0149		"create view user_count_timeline as \
									select time, dbo.atum_GetOnlyDate(time) as date, CAST(DATENAME(yy, time)+'-'+DATENAME(mm, time)+'-'+DATENAME(dd, time)+' '+DATENAME(hh, time)+':00' as datetime) as datehour,	DATEPART(hh, time) as hour,	sum(clientcount) as clientcount	from tm_user_count_timeline where MapIndex = 0 and time > '%s' and time < '%s'	group by time "
#define QUERY_080702_0150_1		"select time, clientcount from user_count_timeline where time > '%s' and time < '%s'"
#define QUERY_080702_0150_2_A	"order by clientcount desc;"
#define QUERY_080702_0150_2_B	"order by time desc;"

#define QUERY_080702_0151		"drop view user_count_timeline ;"
#define QUERY_080702_0152		"drop view tm_user_count_timeline ;"
#define QUERY_080702_0153		"CREATE VIEW tm_user_count_timeline AS \
									SELECT * FROM atum_backup_log_server_map NOLOCK where MapIndex = 0 and time > '%s' and time < '%s' \
									UNION ALL \
									SELECT * FROM atum_log_server_map NOLOCK where MapIndex = 0 and time > '%s' and time < '%s' "
#define QUERY_080702_0154		"CREATE VIEW user_count_timeline	as	select	time, dbo.atum_GetOnlyDate(time) as date, CAST(DATENAME(yy, time)+'-'+DATENAME(mm, time)+'-'+DATENAME(dd, time)+' '+DATENAME(hh, time)+':00' as datetime) as datehour,	DATEPART(hh, time) as hour,	sum(clientcount) as clientcount	from tm_user_count_timeline where MapIndex = 0 and time > '%s' and time < '%s'	group by time "

// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
//#define QUERY_080702_0155		"select hour, max(clientcount) as maxcount, min(clientcount) as mincount,avg(clientcount) as avgcount from user_count_timeline group by hour order by hour ;"

#define QUERY_080702_0156		"drop view user_count_timeline;"
#define QUERY_080702_0157		"drop view tm_user_count_timeline;"
#define QUERY_080702_0158		"CREATE VIEW tm_user_count_timeline AS \
									SELECT * FROM atum_backup_log_server_map WITH (NOLOCK) where MapIndex = 0 and time > '%s' and time < '%s' \
									UNION ALL \
									SELECT * FROM atum_log_server_map WITH (NOLOCK) where MapIndex = 0 and time > '%s' and time < '%s' "
#define QUERY_080702_0159		"create view user_count_timeline	as	select	time, dbo.atum_GetOnlyDate(time) as date, CAST(DATENAME(yy, time)+'-'+DATENAME(mm, time)+'-'+DATENAME(dd, time)+' '+DATENAME(hh, time)+':00' as datetime) as datehour,	DATEPART(hh, time) as hour,	sum(clientcount) as clientcount	from tm_user_count_timeline where MapIndex = 0 and time > '%s' and time < '%s'	group by time "

// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
//#define QUERY_080702_0160		"select date, max(clientcount) as maxcount, avg(clientcount) as avgcount, min(clientcount) as mincount from user_count_timeline group by date	order by date;"

#define QUERY_080702_0161		"drop view user_count_timeline;"
#define QUERY_080702_0162		"drop view tm_user_count_timeline;"
#define QUERY_080702_0163		"CREATE VIEW tm_user_count_timeline AS \
									SELECT * FROM atum_backup_log_server_map WITH (NOLOCK) where MapIndex = 0 and time > '%s' and time < '%s' \
									UNION ALL \
									SELECT * FROM atum_log_server_map WITH (NOLOCK) where MapIndex = 0 and time > '%s' and time < '%s' "
#define QUERY_080702_0164		"create view user_count_timeline	as	select	time, dbo.atum_GetOnlyDate(time) as date, CAST(DATENAME(yy, time)+'-'+DATENAME(mm, time)+'-'+DATENAME(dd, time)+' '+DATENAME(hh, time)+':00' as datetime) as datehour,	DATEPART(hh, time) as hour,	sum(clientcount) as clientcount	from tm_user_count_timeline where MapIndex = 0 and time > '%s' and time < '%s'	group by time "

// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
//#define QUERY_080702_0165		"select datehour, max(clientcount) as maxcount, min(clientcount) as mincount, avg(clientcount) as avgcount from user_count_timeline group by datehour	order by datehour desc;"

#define QUERY_080702_0166		"drop view view_levelranking;"
#define QUERY_080702_0167_1		"CREATE VIEW view_levelranking AS \
									SELECT TOP %d c.accountname, c.charactername, dbo.atum_GetGearString(c.unitkind) as unit, c.InfluenceType, c.level, c2.maxexp, c.TotalPlayTime, c2.numofcharacter \
									FROM td_character c, (SELECT accountuniquenumber, max(experience) as maxexp, count(*) as numofcharacter	FROM td_character WITH (NOLOCK) \
									WHERE 0 = Race&%d AND race < 128 and charactername not like 'loadtest%%'	group by accountuniquenumber) c2, atum2_db_account.dbo.td_account a WITH (NOLOCK) \
									WHERE 0 = c.Race&%d AND c.race < 128 AND c.accountuniquenumber = c2.accountuniquenumber and c.experience >= c2.maxexp \
									AND (c2.maxexp != 0 OR charactername in (select max(charactername) FROM td_character group by accountuniquenumber)) \
									AND a.accountuniquenumber = c.accountuniquenumber "
// 2008-08-28 by cmkwon, MySQL 관련 날짜시간 관련 오류 수정 - 
//#define QUERY_080702_0167_2_A	" %s AND a.LastLoginDate >= '%04d%02d%02d %02d:%02d:%02d' ORDER BY c2.maxexp desc "
#define QUERY_080702_0167_2_A	" %s AND a.LastLoginDate >= '%s' ORDER BY c2.maxexp desc "

#define QUERY_080702_0167_2_B	" %s ORDER BY c2.maxexp desc "
#define QUERY_080702_0167_3		" LIMIT 0,%d ;"		// 2008-07-08 by cmkwon, MySQL 지원 구현 - Only for MySQL

// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
//#define QUERY_080702_0168		"SELECT * FROM view_levelranking;"

#define QUERY_080702_0169		"drop view view_levelranking;"

#define QUERY_080702_0170_1		"CREATE VIEW view_levelranking AS \
									SELECT TOP %d c.accountname, c.charactername, dbo.atum_GetGearString(c.unitkind) as unit, c.InfluenceType, c.level, c2.maxexp, c.TotalPlayTime, c2.numofcharacter \
									FROM td_character c, (SELECT accountuniquenumber, max(experience) as maxexp, count(*) as numofcharacter	FROM td_character WITH (NOLOCK) \
									WHERE 0 = Race&%d AND race < 128 and charactername not like 'loadtest%%' group by accountuniquenumber) c2, atum2_db_account.dbo.td_account a WITH (NOLOCK) \
									WHERE 0 = c.Race&%d AND c.race < 128 and c.accountuniquenumber = c2.accountuniquenumber and c.experience >= c2.maxexp \
									AND (c2.maxexp != 0 OR charactername in (select max(charactername) FROM td_character group by accountuniquenumber)) "

// 2008-08-28 by cmkwon, MySQL 관련 날짜시간 관련 오류 수정 - 
//#define QUERY_080702_0170_2		" %s AND c.CreatedTime >= '%04d%02d%02d %02d:%02d:%02d' AND c.CreatedTime <= '%04d%02d%02d %02d:%02d:%02d' ORDER BY c2.maxexp desc "
#define QUERY_080702_0170_2		" %s AND c.CreatedTime >= '%s' AND c.CreatedTime <= '%s' ORDER BY c2.maxexp desc "

#define QUERY_080702_0170_3		" LIMIT 0,%d ;"		// 2008-07-08 by cmkwon, MySQL 지원 구현 - Only for MySQL

// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
//#define QUERY_080702_0171		"SELECT * FROM view_levelranking;"

#define QUERY_080702_0172		"drop view view_levelranking;"

#define QUERY_080702_0174_1		"CREATE VIEW view_levelranking AS \
									SELECT c.InfluenceType, c.level, COUNT(c.level) AS UserCnt\
									FROM td_character c, atum2_db_account.dbo.td_account a WITH (NOLOCK) \
									WHERE 0 = c.Race&%d AND c.Race < 128 AND a.accountuniquenumber = c.accountuniquenumber "

// 2008-08-28 by cmkwon, MySQL 관련 날짜시간 관련 오류 수정 - 
//#define QUERY_080702_0174_2_A	" %s AND a.LastLoginDate >= '%04d%02d%02d %02d:%02d:%02d' GROUP BY c.InfluenceType, c.level "
#define QUERY_080702_0174_2_A	" %s AND a.LastLoginDate >= '%s' GROUP BY c.InfluenceType, c.level "

#define QUERY_080702_0174_2_B	" %s GROUP BY c.InfluenceType, c.level "

// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
//#define QUERY_080702_0175		"SELECT * FROM view_levelranking ORDER BY InfluenceType, Level desc;"

#define QUERY_080702_0176		"drop view view_levelranking;"

#define QUERY_080702_0177_1		"CREATE VIEW view_levelranking AS \
									SELECT c.InfluenceType, c.level, COUNT(c.level) AS UserCnt\
									FROM td_character c, atum2_db_account.dbo.td_account a WITH (NOLOCK) \
									WHERE 0 = c.Race&%d AND c.Race < 128 AND a.accountuniquenumber = c.accountuniquenumber "

// 2008-08-28 by cmkwon, MySQL 관련 날짜시간 관련 오류 수정 - 
//#define QUERY_080702_0177_2		" %s AND c.CreatedTime >= '%04d%02d%02d %02d:%02d:%02d' AND c.CreatedTime <= '%04d%02d%02d %02d:%02d:%02d' GROUP BY c.InfluenceType, c.level "
#define QUERY_080702_0177_2		" %s AND c.CreatedTime >= '%s' AND c.CreatedTime <= '%s' GROUP BY c.InfluenceType, c.level "

// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
//#define QUERY_080702_0178		"SELECT * FROM view_levelranking ORDER BY InfluenceType, Level desc;"

#define QUERY_080702_0179		"use atum2_db_%d;"

// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
//#define QUERY_080702_0180		"select i.ItemNum, count(i.ItemNum) as Sellcount, count(i.ItemNum)*i.CashPrice as SellCash \
//									from atum_log_item_buy_sell il, ti_Item i WITH (NOLOCK) where il.ItemNum = i.ItemNum and 0 <> i.ItemAttribute & 65536 and il.time >= '%s' and il.time <= '%s' \
//									group by i.ItemNum, i.CashPrice;"

#define QUERY_080702_0181		"DROP VIEW view_itemcount;"
#define QUERY_080702_0182		"CREATE VIEW view_itemcount AS \
									SELECT TOP 100 s.accountuniqueNumber, SUM(CAST(s.CurrentCount AS FLOAT)) as Count \
									FROM td_store s, atum2_db_account.dbo.td_account a WITH (NOLOCK) \
									WHERE s.ItemNum = %d AND s.accountuniquenumber = a.accountuniquenumber AND a.accountType < 128 \
									GROUP BY s.accountuniqueNumber, s.ItemNum \
									ORDER BY SUM(CAST(s.CurrentCount AS FLOAT)) DESC "

// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
// #define QUERY_080702_0183		"SELECT c.accountname, c.charactername, dbo.atum_GetGearString(c.unitkind) as unit, c.InfluenceType, c.level, tm.Count \
// 									FROM td_character c, (SELECT accountuniquenumber, max(experience) as maxexp, count(*) as numofcharacter	FROM td_character \
// 									WHERE 0 = Race&%d AND race < 128 and charactername not like 'loadtest%%'	group by accountuniquenumber) c2, atum2_db_account.dbo.td_account a, view_itemcount tm \
// 									WHERE 0 = c.Race&%d AND c.Race < 128 AND c.accountuniquenumber = c2.accountuniquenumber and c.experience >= c2.maxexp AND (c.AccountUniqueNumber in (select AccountUniqueNumber from view_itemcount)) \
// 									AND a.accountuniquenumber = c.accountuniquenumber and tm.AccountUniqueNumber = c.accountuniquenumber \
// 									ORDER BY tm.Count desc;"

#define QUERY_080702_0184		"DROP VIEW view_itemcount;"
#define QUERY_080702_0185		"CREATE VIEW view_itemcount AS \
									SELECT TOP 100 s.accountuniqueNumber, count(s.ItemNum) as Count \
									FROM td_store s, atum2_db_account.dbo.td_account a WITH (NOLOCK) \
									WHERE s.ItemNum = %d AND s.accountuniquenumber = a.accountuniquenumber AND a.accountType < 128 \
									GROUP BY s.accountuniqueNumber, s.ItemNum \
									ORDER BY count(*) DESC "

// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
// #define QUERY_080702_0186		"SELECT c.accountname, c.charactername, dbo.atum_GetGearString(c.unitkind) as unit, c.InfluenceType, c.level, tm.Count \
// 									FROM td_character c, (SELECT accountuniquenumber, max(experience) as maxexp, count(*) as numofcharacter	FROM td_character \
// 									WHERE 0 = Race&%d AND race < 128 and charactername not like 'loadtest%%'	group by accountuniquenumber) c2, atum2_db_account.dbo.td_account a, view_itemcount tm \
// 									WHERE 0 = c.Race&%d AND c.Race < 128 and c.accountuniquenumber = c2.accountuniquenumber and c.experience >= c2.maxexp AND (c.AccountUniqueNumber in (select AccountUniqueNumber from view_itemcount)) \
// 									AND a.accountuniquenumber = c.accountuniquenumber and tm.AccountUniqueNumber = c.accountuniquenumber \
// 									ORDER BY tm.Count desc;"
// #define QUERY_080702_0187		"SELECT SUM(CAST(CurrentCount AS FLOAT)) \
// 									FROM td_store s, atum2_db_account.dbo.td_account a WITH (NOLOCK)\
// 									WHERE s.ItemNum= %d and s.AccountUniqueNumber = a.AccountUniqueNumber and a.AccountType < 128 \
// 									GROUP by s.ItemNum;"
// #define QUERY_080702_0188		"SELECT COUNT(s.ItemNum) \
// 									FROM td_store s, atum2_db_account.dbo.td_account a WITH (NOLOCK)\
// 									WHERE s.ItemNum= %d and s.AccountUniqueNumber = a.AccountUniqueNumber and a.AccountType < 128 \
// 									GROUP by s.ItemNum;"

#define QUERY_080702_0189		"SELECT TOP %d c.AccountName, c.CharacterName, dbo.atum_GetGearString(c.unitkind), c.InfluenceType, c.level, s.ItemNum, i.ReqMinLevel, s.PrefixCodeNum, s.SuffixCodeNum \
									FROM td_store s, td_character c, atum2_db_account.dbo.ti_Item i WITH (NOLOCK) \
									WHERE s.Possess = c.UniqueNumber AND s.ItemNum = i.ItemNum and 0 = c.Race&%d AND c.Race < 128 \
									AND s.PrefixCodeNum = %d AND s.SuffixCodeNum = %d \
									ORDER BY i.ReqMinLevel DESC;"
									
#define QUERY_080702_0190		"SELECT TOP %d c.AccountName, c.CharacterName, dbo.atum_GetGearString(c.unitkind), c.InfluenceType, c.level, s.ItemNum, i.ReqMinLevel, s.PrefixCodeNum, s.SuffixCodeNum \
									FROM td_store s, td_character c, atum2_db_account.dbo.ti_Item i WITH (NOLOCK) \
									WHERE s.Possess = c.UniqueNumber AND s.ItemNum = i.ItemNum and 0 = c.Race&%d AND c.Race < 128 \
									AND (%d <> 0 and s.PrefixCodeNum = %d OR %d <> 0 AND s.SuffixCodeNum = %d) \
									ORDER BY i.ReqMinLevel DESC;"

#define QUERY_080702_0191		"SELECT COUNT(*) \
									FROM td_store s, atum2_db_account.dbo.td_account a WITH (NOLOCK)\
									WHERE s.AccountUniqueNumber = a.AccountUniqueNumber and a.AccountType < 128 AND s.PrefixCodeNum = %d AND s.SuffixCodeNum = %d;"

#define QUERY_080702_0192		"SELECT COUNT(*) \
									FROM td_store s, atum2_db_account.dbo.td_account a WITH (NOLOCK)\
									WHERE s.AccountUniqueNumber = a.AccountUniqueNumber and a.AccountType < 128 AND (%d <> 0 and s.PrefixCodeNum = %d OR %d <>0 and s.SuffixCodeNum = %d);"

#define QUERY_080702_0193		"drop view moneyitemtable;"
#define QUERY_080702_0194		"CREATE VIEW moneyitemtable AS \
									SELECT TOP %d s.Possess, SUM(CAST(s.CurrentCount AS FLOAT)) AS TotalCount FROM td_store s, td_character c WITH (NOLOCK) \
									WHERE s.ItemNum=%d and s.Possess = c.uniqueNumber and 0 = c.Race&%d AND c.Race < 128 GROUP BY s.Possess ORDER BY SUM(CAST(s.CurrentCount AS FLOAT)) DESC "

// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
// #define QUERY_080702_0195		"select c.AccountName, c.charactername, dbo.atum_GetGearString(c.unitkind), c.InfluenceType, c.level, c.TotalPlayTime, m.TotalCount from moneyitemtable m, td_character c WITH (NOLOCK) \
// 									where c.uniquenumber = m.possess and c.race < 128 order by m.TotalCount desc;"

#define QUERY_080702_0196		"drop view moneyitemtable;"
#define QUERY_080702_0197		"CREATE VIEW moneyitemtable AS \
									SELECT TOP %d s.AccountUniqueNumber, SUM(CAST(s.CurrentCount AS FLOAT)) AS TotalCount \
									FROM td_store s, atum2_db_account.dbo.td_account a WITH (NOLOCK) \
									WHERE s.ItemNum=%d and s.AccountUniqueNumber = a.AccountUniqueNumber and a.AccountType < 128 \
									GROUP BY s.AccountUniqueNumber \
									ORDER BY SUM(CAST(s.CurrentCount AS FLOAT)) DESC "

// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
// #define QUERY_080702_0198		"SELECT c.accountname, c.charactername, dbo.atum_GetGearString(c.unitkind) as unit, c.InfluenceType, c.level, tmM.TotalCount \
// 									FROM td_character c, (SELECT accountuniquenumber, max(experience) as maxexp, count(*) as numofcharacter	FROM td_character \
// 									WHERE 0 = Race&%d AND race < 128 and charactername not like 'loadtest%%'	group by accountuniquenumber) c2, atum2_db_account.dbo.td_account a, moneyitemtable tmM \
// 									WHERE 0 = c.Race&%d AND c.Race < 128 AND c.accountuniquenumber = c2.accountuniquenumber and c.experience >= c2.maxexp AND (c.AccountUniqueNumber in (select AccountUniqueNumber from moneyitemtable)) \
// 									AND a.accountuniquenumber = c.accountuniquenumber and tmM.AccountUniqueNumber = c.accountuniquenumber \
// 									ORDER BY tmM.TotalCount desc;"
// #define QUERY_080702_0199		"SELECT SUM(CAST(CurrentCount AS FLOAT)) \
// 									FROM td_store s, td_character c WITH (NOLOCK)\
// 									WHERE s.ItemNum= %d and s.Possess = c.uniqueNumber and 0 = c.Race&%d AND c.Race < 128 \
// 									GROUP by s.ItemNum;"									
// 
// #define QUERY_080702_0200		"SELECT TOP %d c.accountname, c.charactername, dbo.atum_GetGearString(c.unitkind), c.InfluenceType, c.level, c.PKWinPoint*10 + c.PKLossPoint, c.PKWinPoint, c.PKLossPoint \
// 									FROM td_character c WITH (NOLOCK) \
// 									WHERE 0 = c.Race&%d AND c.race < 128 \
// 									ORDER BY c.PKWinPoint*10 + c.PKLossPoint DESC;"
// 
// #define QUERY_080702_0201		"DROP VIEW temp_view;"
#define QUERY_080702_0202		"CREATE VIEW temp_view AS \
									SELECT * FROM  atum_backup_log_user_quest_pk_dead WITH (NOLOCK) WHERE logtype = 22 and time > '%s' and time < '%s' \
									UNION ALL \
									SELECT * FROM  atum_log_user_quest_pk_dead WITH (NOLOCK) WHERE logtype = 22 and time > '%s' and time < '%s' "

// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
// #define QUERY_080702_0203		"select date, count(*) from (select time, dbo.atum_getonlydate(time) as date	from temp_view l where logtype = 22 and time > '%s' and time < '%s') ll group by date	order by date;"
// #define QUERY_080702_0204		"DROP VIEW temp_view;"
#define QUERY_080702_0205		"CREATE VIEW temp_view AS \
									SELECT * FROM  atum_backup_log_user_quest_pk_dead WITH (NOLOCK) WHERE logtype = 22 and time > '%s' and time < '%s' \
									UNION ALL \
									SELECT * FROM  atum_log_user_quest_pk_dead WITH (NOLOCK) WHERE logtype = 22 and time > '%s' and time < '%s' "

// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
// #define QUERY_080702_0206		"select time, c1.charactername, c1.level,  c2.charactername, c2.level, l.MapIndex, dbo.atum_getonlydate(time) \
// 									from temp_view l, td_character c1, td_character c2 where logtype = 22 and time > '%s' and time < '%s' \
// 									and l.characteruniquenumber = c1.uniquenumber and l.param1 = c2.uniquenumber AND 0 = c1.Race&%d\
// 									order by time;"
// #define QUERY_080702_0207		"DROP VIEW temp_view;"
#define QUERY_080702_0208		"CREATE VIEW temp_view AS \
									SELECT * FROM  atum_backup_log_user_quest_pk_dead WITH (NOLOCK) WHERE (logtype = 22 or logtype = 23) and time > '%s' and time < '%s' \
									UNION ALL \
									SELECT * FROM  atum_log_user_quest_pk_dead WITH (NOLOCK) WHERE (logtype = 22 or logtype = 23) and time > '%s' and time < '%s' "

// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
// #define QUERY_080702_0209		"select time, c1.charactername, c1.level,  c2.charactername, c2.level, l.MapIndex, dbo.atum_getonlydate(time) \
// 									from temp_view l, td_character c1, td_character c2 where (logtype = 22 or logtype = 23) and time > '%s' and time < '%s' \
// 									and l.characteruniquenumber = c1.uniquenumber and l.param1 = c2.uniquenumber AND 0 = c1.Race&%d\
// 									order by time;"
// #define QUERY_080702_0210		"DROP VIEW temp_view;"

#define QUERY_080702_0211		"CREATE VIEW temp_view AS \
									SELECT * FROM  atum_backup_log_user_quest_pk_dead WITH (NOLOCK) WHERE (logtype = 22 or logtype = 23) and time > '%s' and time < '%s' \
									UNION ALL \
									SELECT * FROM  atum_log_user_quest_pk_dead WITH (NOLOCK) WHERE (logtype = 22 or logtype = 23) and time > '%s' and time < '%s' "
									
// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
// #define QUERY_080702_0212		"select c1.charactername, (select level from td_character where charactername = c1.charactername) as level, count(*) as count \
// 									from temp_view l, td_character c1, td_character c2 where (logtype = 22 or logtype = 23) and time > '%s' and time < '%s' \
// 									and l.characteruniquenumber = c1.uniquenumber and c1.uniquenumber = c2.uniquenumber AND 0 = c1.Race&%d\
// 									group by c1.charactername \
// 									order by count desc;"
// #define QUERY_080702_0213		"DROP VIEW temp_view;"
#define QUERY_080702_0214		"CREATE VIEW temp_view AS \
									SELECT * FROM  atum_backup_log_user_quest_pk_dead WITH (NOLOCK) WHERE logtype = 21 and time > '%s' and time < '%s' \
									UNION ALL \
									SELECT * FROM  atum_log_user_quest_pk_dead WITH (NOLOCK) WHERE logtype = 21 and time > '%s' and time < '%s' "

// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
// #define QUERY_080702_0215		"select date, count(*) from (select time, dbo.atum_getonlydate(time) as date	from temp_view l	where logtype = 21 and time > '%s' and time < '%s') ll group by date order by date;"
// 
// #define QUERY_080702_0216		"SELECT c.AccountName, c.CharacterName, dbo.atum_GetGearString(c.unitkind), c.InfluenceType, c.level, q.QuestTimeStamp \
// 									FROM td_character c, td_characterquest q WITH (NOLOCK) \
// 									WHERE q.QuestIndex = %d and q.QuestState = 2 and q.QuestTimeStamp > \'%s\' and q.QuestTimeStamp < \'%s\' and q.CharacterUniqueNumber = c.uniqueNumber AND 0 = c.Race&%d\
// 									ORDER BY q.QuestTimeStamp;"
// #define QUERY_080702_0217		"SELECT c.AccountName, c.CharacterName, dbo.atum_GetGearString(c.unitkind), c.InfluenceType, c.level, q.QuestTimeStamp \
// 									FROM td_character c, td_characterquest q WITH (NOLOCK) \
// 									WHERE q.QuestIndex = %d and q.QuestState = 1 and q.QuestTimeStamp > \'%s\' and q.QuestTimeStamp < \'%s\' and q.CharacterUniqueNumber = c.uniqueNumber AND 0 = c.Race&%d\
// 									ORDER BY q.QuestTimeStamp;"
// #define QUERY_080702_0218		"DROP VIEW temp_view;"
#define QUERY_080702_0219		"CREATE VIEW temp_view AS \
									SELECT * FROM  atum_backup_log_monster_boss WITH (NOLOCK) WHERE time > '%s' and time < '%s' \
									UNION ALL \
									SELECT * FROM  atum_log_monster_boss WITH (NOLOCK) WHERE time > '%s' and time < '%s' "

// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
// #define QUERY_080702_0220		"SELECT l.MonsterUnitKind, l.MapIndex, l.ChannelIndex, l.CreatedTime, l.DeadTime, l.AttackUser1, l.AttackUser2, l.AttackUser3, l.AttackUser4, l.AttackUser5 \
// 									, l.AttackUser6, l.AttackUser7, l.AttackUser8, l.AttackUser9, l.AttackUser10, l.DropItemList \
// 									FROM temp_view l\
// 									ORDER BY l.Time DESC "
// 
// #define QUERY_080702_0221		"SELECT dbo.atum_GetGearString(unitkind), InfluenceType, Level \
// 									FROM td_character WITH (NOLOCK) \
// 									WHERE CharacterName = '%s';"
// #define QUERY_080702_0222		"SELECT TOP %d g.GuildName, g.GuildUniqueNumber, g.TotalFame, g.MonthlyFame, c.AccountName, c.CharacterName, dbo.atum_GetGearString(c.unitkind), c.InfluenceType, c.level \
// 									FROM td_character c, td_guild g WITH (NOLOCK) \
// 									WHERE g.GuildCommanderUniqueNumber = c.UniqueNumber AND 0 = c.Race&%d \
// 									ORDER BY g.MonthlyFame DESC;"
// #define QUERY_080702_0223		"SELECT TOP %d c.accountname, c.charactername, dbo.atum_GetGearString(c.unitkind), c.InfluenceType, c.level, c.GuildName, c.Propensity, c.TotalPlayTime \
// 									FROM td_character c WITH (NOLOCK) \
// 									WHERE 0 = c.Race&%d AND c.race < 128 \
// 									ORDER BY c.Propensity DESC;"
// #define QUERY_080702_0224		"DROP VIEW tm_user_connection;"

// 2009-02-10 by cmkwon, 세력 유저 카운트 관련 수정 - 아래에 QUERY_090209_0300처럼 수정함.
// #define QUERY_080702_0225		"CREATE VIEW tm_user_connection AS \
// 									SELECT DISTINCT c.AccountUniqueNumber, c.SelectableInfluenceMask \
// 									FROM atum2_db_account.dbo.atum_log_connection l WITH (NOLOCK) inner join td_character c WITH (NOLOCK) \
// 									ON c.accountuniquenumber = l.accountuniquenumber \
// 									WHERE time > '%s' and time < '%s' \
// 									UNION (SELECT DISTINCT c.AccountUniqueNumber, c.SelectableInfluenceMask \
// 									FROM atum2_db_account.dbo.atum_log_connection l WITH (NOLOCK) INNER join td_character c \
// 									ON c.accountuniquenumber = l.accountuniquenumber \
// 									WHERE time > '%s' and time < '%s') "

// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
// #define QUERY_080702_0226		"SELECT SelectableInfluenceMask, COUNT(SelectableInfluenceMask) \
// 									FROM tm_user_connection \
// 									GROUP BY SelectableInfluenceMask;"
// #define QUERY_080702_0227		"DROP VIEW tm_user_connection;"

// 2009-02-10 by cmkwon, 세력 유저 카운트 관련 수정 - 아래에 QUERY_090209_0301처럼 수정함.
// #define QUERY_080702_0228		"CREATE VIEW tm_user_connection AS \
// 									SELECT dbo.atum_getonlydate(Time) AS conntedDate, c.AccountUniqueNumber, c.SelectableInfluenceMask \
// 									FROM atum2_db_account.dbo.atum_log_connection l WITH(NOLOCK) INNER JOIN td_character c ON c.AccountUniqueNumber = l.AccountUniqueNumber \
// 									WHERE Time >= '%s' and Time <= '%s' \
// 									GROUP BY dbo.atum_getonlydate(Time), c.AccountUniqueNumber, c.SelectableInfluenceMask \
// 									UNION (SELECT dbo.atum_getonlydate(Time) AS conntedDate, c.AccountUniqueNumber, c.SelectableInfluenceMask \
// 									FROM atum2_db_account.dbo.atum_backup_log_connection l WITH(NOLOCK) INNER JOIN td_character c ON c.AccountUniqueNumber = l.AccountUniqueNumber \
// 									WHERE Time >= '%s' and Time <= '%s' \
// 									GROUP BY dbo.atum_getonlydate(Time), c.AccountUniqueNumber, c.SelectableInfluenceMask) "

// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
// #define QUERY_080702_0229		"SELECT l.conntedDate, l.SelectableInfluenceMask, COUNT(l.AccountUniqueNumber) \
// 									FROM tm_user_connection l WITH(NOLOCK) \
// 									GROUP BY l.conntedDate, l.SelectableInfluenceMask \
// 									ORDER BY l.conntedDate;"
#define QUERY_080702_0230		"select ItemNum, kind, ItemName, abilitymin, abilitymax, rangeangle, reqrace, reqattackpart, reqdefensepart, ReqFuelPart, reqsoulpart, reqshieldpart, reqdodgepart, requnitkind, reqminlevel, reqmaxlevel, ReqItemKind, weight, hitrate, defense, range, position, abrasionrate, charging, mintradequantity, price, cashprice, reattacktime, [time], LinkItem, multitarget, explosionrange, reactionrange, shotnum, multinum, attacktime, ItemAttribute, description, caliber, orbittype, camerapattern, SourceIndex from atum2_db_account.dbo.ti_Item WITH (NOLOCK) where kind >= %d and kind <= %d;"

#define QUERY_080702_0231		"delete atum2_db_account.dbo.ti_Item where ItemNum = %s;"
#define QUERY_080702_0232		"Update atum2_db_account.dbo.ti_Item set kind = %s, ItemName = '%s', abilitymin = %s, abilitymax = %s, rangeangle = %s, reqrace = %s, reqattackpart = %s, reqdefensepart = %s, ReqFuelPart = %s, reqsoulpart = %s, reqshieldpart = %s, reqdodgepart = %s, requnitkind = %s, reqminlevel = %s, reqmaxlevel = %s, ReqItemKind = %s, weight = %s, hitrate = %s, defense = %s, range = %s, position = %s, abrasionrate = %s, charging = %s, mintradequantity = %s, price = %s, cashprice = %s, reattacktime = %s, [time] = %s, LinkItem = %s, multitarget = %s, explosionrange = %s, reactionrange = %s, shotnum = %s, multinum = %s, attacktime = %s, ItemAttribute = %s, [description] = '%s', caliber = %s, orbittype = %s, camerapattern = %s, SourceIndex = %s where ItemNum = %s;"
#define QUERY_080702_0233		"Insert into atum2_db_account.dbo.ti_Item(ItemNum, kind, ItemName, abilitymin, abilitymax, rangeangle, reqrace, reqattackpart, reqdefensepart, ReqFuelPart, reqsoulpart, reqshieldpart, reqdodgepart, requnitkind, reqminlevel, reqmaxlevel, ReqItemKind, weight, hitrate, defense, range, position, abrasionrate, charging, mintradequantity, price, cashprice, reattacktime, [time], LinkItem, multitarget, explosionrange, reactionrange, shotnum, multinum, attacktime, ItemAttribute, [description], caliber, orbittype, camerapattern, SourceIndex) values ( %s, %s, '%s', %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, '%s', %s, %s, %s, %s);"
#define QUERY_080702_0234		"select ItemNum from atum2_db_account.dbo.ti_Item WITH (NOLOCK) where kind = %d;"

// 2009-04-21 by cmkwon, ITEM에 DesParam 필드 개수 8개로 늘리기 - 현재 사용하지 않으므로 주석 처리
//#define QUERY_080702_0235		"select ItemNum, ItemName, AbilityMin, AbilityMax, ReqRace, ReqMinLevel, Hitrate, SpeedPenalty, Range, Charging, RangeAngle, Shotnum, Multinum, Attacktime, ReqSP, Summonmonster, Orbittype, Kind, SourceIndex, DestParameter1, ParameterValue1, DestParameter2, ParameterValue2, DestParameter3, ParameterValue3, DestParameter4, ParameterValue4 from atum2_db_account.dbo.ti_Item where ItemNum like '75%%' and ItemNum in (%s);"
								
// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
// #define QUERY_080702_0236		"SELECT a.AccountName, a.Password as TempPassword, ak.Password as OrgPassword \
// 									FROM atum2_db_account.dbo.td_account a, td_accountbackup ak \
// 									WHERE a.accountname = ak.accountname;"
// #define QUERY_080702_0237		"INSERT INTO AccountBackup SELECT AccountName, Password FROM atum2_db_account.dbo.td_account WHERE AccountName like \'%s\';"
// #define QUERY_080702_0238		"UPDATE atum2_db_account.dbo.td_account SET Password = \'%s\' WHERE AccountName like \'%s\';"
// #define QUERY_080702_0239		"UPDATE atum2_db_account.dbo.td_account SET Password = (SELECT Password FROM td_accountbackup WHERE AccountName like \'%s\') WHERE AccountName LIKE \'%s\';"
// #define QUERY_080702_0240		"DELETE FROM td_accountbackup WHERE AccountName like \'%s\';"
// 
// #define QUERY_080702_0241		"INSERT INTO BlockedAccounts VALUES(\'%s\');"
// #define QUERY_080702_0242		"DELETE FROM atum2_db_account.dbo.td_blockedaccounts WHERE AccountName = \'%s\';"
// #define QUERY_080702_0243		"SELECT * FROM atum2_db_account.dbo.td_blockedaccounts WITH (NOLOCK);"
// #define QUERY_080702_0244		"SELECT Code, Alpha, Collision, CollisionForServer, RenderIndex, ObjectRenderType FROM atum2_db_account.dbo.ti_MapObject WITH (NOLOCK);"
// #define QUERY_080702_0245		"SELECT AuctionPrice, AuctionAccountUID, AuctionCharacterUID FROM td_Auction NOLOCK WHERE AuctionItemUID = %d ;"
// 
// #define QUERY_080702_0246		"SELECT * FROM atum2_db_account.dbo.ti_Item WITH (NOLOCK);"
// #define QUERY_080702_0247		"SELECT * FROM atum2_db_account.dbo.ti_MonsterItem WITH (NOLOCK);"
// #define QUERY_080702_0248		"SELECT * FROM atum2_db_account.dbo.ti_CityTargetWarpMap WITH (NOLOCK) WHERE MapIndex = %d;"
// #define QUERY_080702_0249		"SELECT Password FROM atum2_db_account.dbo.td_account WITH (NOLOCK) WHERE AccountName LIKE '%s' AND IsBlocked = 0;"
// #define QUERY_080702_0250		"SELECT GuildName FROM td_guild WITH (NOLOCK) WHERE GuildName LIKE \'%s\';"
// 
// #define QUERY_080702_0251		"SELECT GuildName FROM td_guild WITH (NOLOCK) WHERE GuildCommanderUniqueNumber = %d;"
// #define QUERY_080702_0252		"SELECT GuildDismemberDate FROM td_guild WITH (NOLOCK) WHERE GuildUniqueNumber = %d;"
// #define QUERY_080702_0253		"SELECT GuildName FROM td_guild WITH (NOLOCK) WHERE GuildName LIKE \'%s\';"
// #define QUERY_080702_0254		"UPDATE td_character SET GuildName = \'\', GuildUniqueNumber = 0 WHERE uniqueNumber = %d;"
// #define QUERY_080702_0255		"SELECT CharacterName, Race, InfluenceType FROM td_character WITH (NOLOCK) WHERE CharacterName = \'%s\';"
// 
// #define QUERY_080702_0256		"SELECT CharacterName FROM td_friendlist WITH (NOLOCK) WHERE CharacterName LIKE \'%s\' AND FriendName LIKE \'%s\' AND Type = %d;"
// #define QUERY_080702_0257		"SELECT * FROM atum2_db_account.dbo.ti_Item WITH (NOLOCK);"
// #define QUERY_080702_0258		"SELECT AccountUniqueNumber, AccountType, Password FROM atum2_db_account.dbo.td_account WITH (NOLOCK) WHERE AccountName = \'%s\';"
// 									
// #define QUERY_080702_0259		"DELETE FROM atum2_db_account.dbo.td_accountcashstore WHERE AccountUniqueNumber = %u;"
// #define QUERY_080702_0260		"UPDATE atum2_db_account.dbo.td_accountcashstore SET ItemNum = %d, UpdatedTime = \'%s\', ExpireTime = \'%s\' WHERE AccountUniqueNumber = %u;"
// 
// #define QUERY_080702_0261		"DELETE atum2_db_account.dbo.ti_HappyHourEvent WHERE uniquenumber = %d;"
// #define QUERY_080702_0262		"INSERT atum2_db_account.dbo.ti_HappyHourEvent(ServerGroupID, dayofweek, starttime, endtime, exprate, spirate, exprepairrate, dropitemrate, droprareitemrate, WarPointRate, InfluenceType, MinLevel, MaxLevel) values( 0, %d, '%s', '%s', 0, 0, 0, 0, 0, 0, %d, 1, 100);"
// #define QUERY_080702_0263		"UPDATE atum2_db_account.dbo.ti_HappyHourEvent SET starttime = '%s', endtime = '%s' where UniqueNumber= %d;"
// #define QUERY_080702_0264		"INSERT atum2_db_account.dbo.ti_HappyHourEvent(ServerGroupID, dayofweek, starttime, endtime, exprate, spirate, exprepairrate, dropitemrate, droprareitemrate, WarPointRate, MinLevel, MaxLevel) VALUES( 0, %d, '%s', '%s', %6.4f, %6.4f, %6.4f, %6.4f, %6.4f, %6.4f, 1, 100);"
// #define QUERY_080702_0265		"UPDATE atum2_db_account.dbo.ti_HappyHourEvent SET starttime= '%s', endtime= '%s', exprate= %6.4f, spirate= %6.4f, exprepairrate= %6.4f, dropitemrate= %6.4f, droprareitemrate= %6.4f, WarPointRate= %6.4f WHERE UniqueNumber= %d;"
// 
// #define QUERY_080702_0266		"INSERT into atum2_db_account.dbo.ti_itemevent(ItemEventType,OnlyPCBang,InfluenceType,ItemNum,Count,StartTime,EndTime,MemberShip,ReqLevel,NewMember,UnitKind,PrefixCodeNum,SuffixCodeNum,UseLastGameEndDate,LastGameEndDate) values( %d, %d, %d, %d, %d,'%s', '%s', %d, %d, %d, %d, %d, %d, %d, '%s');"
// #define QUERY_080702_0267		"DELETE atum2_db_account.dbo.ti_itemevent WHERE ItemEventUID = %d;"
// #define QUERY_080702_0268		"UPDATE atum2_db_account.dbo.ti_itemevent SET ItemEventType = %d, OnlyPCBang = %d, InfluenceType = %d, ItemNum = %d, Count = %d, StartTime = '%s', EndTime = '%s', MemberShip = %d, ReqLevel = %d, NewMember = %d, UnitKind = %d, PrefixCodeNum = %d, SuffixCodeNum = %d, UseLastGameEndDate=%d, LastGameEndDate= '%s' WHERE ItemEventUID = %d;"
// #define QUERY_080702_0269		"UPDATE dbo.td_outpostinfo \
// 									SET OutPostNextWarTime = '%s', OutPostNextWarTimeSet = %d, OutPostGetInfl = %d, OutPostGuildUID = %d \
// 									WHERE OutPostMapIndex = %d AND OutPostCityMapIndex = %d \
// 									UPDATE dbo.td_CityLeaderInfo \
// 									SET Influence = %d, CharacterUID = (SELECT GuildCommanderUniqueNumber FROM dbo.td_guild WHERE GuildUniqueNumber = %d) \
// 									WHERE MapIndex = %d;"

									
///////////////////////////////////////////////////////////////////////////////
// 2008-08-25 by cmkwon, MySQL 소스 통합(빠진 쿼리 정리) - 
#define QUERY_080825_0001 "CAST(l.CurrentCount AS VARCHAR(10)) + 'counts'"
#define QUERY_080825_0002 "CAST(l.ChangeCount AS VARCHAR(10)) + 'counts Pick up, ' + CAST(l.CurrentCount AS VARCHAR(10)) + 'counts'"
#define QUERY_080825_0003 "'Gave ' + CAST(l.ChangeCount AS VARCHAR(10)) + 'counts ' + 'to ''' + l.PeerCharacterName + ''', ' + CAST(l.CurrentCount AS VARCHAR(10)) + 'counts'"
#define QUERY_080825_0004 "'Received ' + CAST(l.ChangeCount AS VARCHAR(10)) + 'counts ' + 'from ''' + l.PeerCharacterName + ''', ' + CAST(l.CurrentCount AS VARCHAR(10)) + 'counts'"
#define QUERY_080825_0005 "'Throw away ' + CAST(l.ChangeCount AS VARCHAR(10)) + 'counts, ' + CAST(l.CurrentCount AS VARCHAR(10)) + 'counts'"

#define QUERY_080825_0006 "'Buy ' + CAST(l.ChangeCount AS VARCHAR(10)) + 'counts, Remained SPI: ' + CAST(l.RemainedMoney AS VARCHAR(10))"
#define QUERY_080825_0007 "'Sell ' + CAST(l.ChangeCount AS VARCHAR(10)) + 'counts, Remained SPI: ' + CAST(l.RemainedMoney AS VARCHAR(10))"
#define QUERY_080825_0008 "'Send ' + CAST(l.ChangeCount AS VARCHAR(10)) + 'counts ' + 'to ''' + l.PeerCharacterName + ''', ' + CAST(l.CurrentCount AS VARCHAR(10)) + 'counts'"
#define QUERY_080825_0009 "'Received ' + CAST(l.ChangeCount AS VARCHAR(10)) + 'counts ' + 'from ''' + l.PeerCharacterName + ''', ' + CAST(l.CurrentCount AS VARCHAR(10)) + 'counts'"
#define QUERY_080825_0010 "'Change EXP: ' + CAST(l.Param1 AS VARCHAR(15)) + ', Total EXP: ' + CAST(l.Param2 AS VARCHAR(15))"

// 2012-11-21 by bckim, 운영툴기능추가, 인챈트아이템제거기능
#define QUERY_130708_0001 "'''' + l.PeerCharacterName + '''worked ['+CAST(l.EnchantCardNumber AS VARCHAR(10)) + '] : ' + CAST(l.ChangeEnchantCount AS VARCHAR(10)) + ' Count Enchanted,  Current :  ' + CAST(l.ResultEnchantCount AS VARCHAR(10)) + ' Enchant'"
#define QUERY_130708_0002 "'''' + l.PeerCharacterName + '''worked ['+CAST(l.EnchantCardNumber AS VARCHAR(10)) + '] : ' + CAST(l.ChangeEnchantCount AS VARCHAR(10)) + ' Count DisEnchanted,  Current : ' + CAST(l.ResultEnchantCount AS VARCHAR(10)) + ' Enchant'"
// 2012-11-21 by bckim, 운영툴기능추가, 인챈트아이템제거기능. End

#define QUERY_130708_0003 " '['+l.ResultType+']' + l.CardList"		// 2013-07-01 by bckim, 10인챈트 미만 하이퍼카드처리, 로그추가, 운영툴 보완


///////////////////////////////////////////////////////////////////////////////
// 2008-09-01 by cmkwon, StringDefineQuery.h로 추출 - 
#define QUERY_080901_0001 "atum2_db_account.dbo.atum_log_connection"
#define QUERY_080901_0002 "atum2_db_account.dbo.atum_backup_log_connection"
#define QUERY_080901_0003 "atum2_db_account.dbo.atum_log_blockedAccount"
// 2009-01-02 by cmkwon, 계정 Unblock 로그 검색 버그 수정 -  
//#define QUERY_080901_0004 "atum2_db_account.dbo.atum_backup_log_blockedAccount"
#define QUERY_080901_0004 "atum2_db_account.dbo.atum_log_blockedAccount"	// 2009-01-02 by cmkwon, 계정 Unblock 로그 검색 버그 수정 -  

// 2009-03-30 by cmkwon, AdminTool 유저 카운트 버그 수정(로그로 검색 해야 함) - 아래와 같이 로그로부터 검색 해야함.
// // 2009-02-10 by cmkwon, 세력 유저 카운트 관련 수정 - QUERY_080702_0225, QUERY_080702_0228을  주석 처리하고 아래처럼 변경함. 기존 계정의 로그인 정보로 검색 하던것을 캐릭터 최종 접속시간 검색으로 수정
// #define QUERY_090209_0300		"CREATE VIEW tm_user_connection AS \
// 									SELECT DISTINCT AccountUniqueNumber, SelectableInfluenceMask \
// 									FROM td_character WITH (NOLOCK) \
// 									WHERE (Race & 0x4000 = 0) AND LastStartedTime > '%s' AND LastStartedTime < '%s' "
// #define QUERY_090209_0301		"CREATE VIEW tm_user_connection AS \
// 									SELECT DISTINCT dbo.atum_getonlydate(LastStartedTime) AS conntedDate, AccountUniqueNumber, SelectableInfluenceMask \
// 									FROM td_character WITH (NOLOCK) \
// 									WHERE (Race & 0x4000 = 0) AND LastStartedTime > '%s' AND LastStartedTime < '%s' "

// 2009-03-30 by cmkwon, AdminTool 유저 카운트 버그 수정(로그로 검색 해야 함) - 아래와 같이 로그로부터 검색 해야함.
#define QUERY_090330_0302		"CREATE VIEW tm_user_connection AS \
									SELECT DISTINCT c.AccountUniqueNumber, c.SelectableInfluenceMask \
									FROM dbo.atum_log_user_game_start_end l WITH (NOLOCK) inner join td_character c WITH(NOLOCK) ON c.accountuniquenumber = l.accountuniquenumber \
									WHERE (Race & 0x4000 = 0) AND Time >= '%s' and Time <= '%s' \
									UNION (SELECT DISTINCT c.AccountUniqueNumber, c.SelectableInfluenceMask \
											FROM dbo.atum_backup_log_user_game_start_end l WITH (NOLOCK) INNER join td_character c WITH(NOLOCK) ON c.accountuniquenumber = l.accountuniquenumber \
											WHERE (Race & 0x4000 = 0) AND Time >= '%s' and Time <= '%s') "
#define QUERY_090330_0303		"CREATE VIEW tm_user_connection AS \
									SELECT dbo.atum_getonlydate(Time) AS conntedDate, c.AccountUniqueNumber, c.SelectableInfluenceMask \
									FROM dbo.atum_log_user_game_start_end l WITH(NOLOCK) INNER JOIN td_character c ON c.AccountUniqueNumber = l.AccountUniqueNumber \
									WHERE (Race & 0x4000 = 0) AND Time >= '%s' and Time <= '%s' \
									GROUP BY dbo.atum_getonlydate(Time), c.AccountUniqueNumber, c.SelectableInfluenceMask \
									UNION (SELECT dbo.atum_getonlydate(Time) AS conntedDate, c.AccountUniqueNumber, c.SelectableInfluenceMask \
											FROM dbo.atum_backup_log_user_game_start_end l WITH(NOLOCK) INNER JOIN td_character c ON c.AccountUniqueNumber = l.AccountUniqueNumber \
											WHERE (Race & 0x4000 = 0) AND Time >= '%s' and Time <= '%s' \
											GROUP BY dbo.atum_getonlydate(Time), c.AccountUniqueNumber, c.SelectableInfluenceMask) "

#endif // END - #ifndef _STRING_DEFINE_QUERY_H_

