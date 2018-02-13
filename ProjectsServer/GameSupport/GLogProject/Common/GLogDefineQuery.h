#pragma once

#import "c:\Program Files\Common Files\System\ADO\msado15.dll" no_namespace rename("EOF", "EndOfFile")


///////////////////////////////////////////////////////////////////////////////
// DB 테이블 추가시 추가되어야 되는곳!!!!!!!!!!!!!!!!!!!
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// 2010-05-24 by dhjin - 
#define	QDB_GLOG_ACCOUNT_SELECT		L"GLog_Account_Select"			/* 	SELECT TOP 1000 [UID], [AccountName], [GamePublisher], [GameName], [GameServerIP], [Birthday], [Sex], [RegDate] FROM GLog.dbo.GLog_Account */
#define	QDB_GLOG_ACCOUNT_INSERT		L"GLog_Account_Insert"			/** INSERT INTO GLog.dbo.GLog_Account([AccountName], [GamePublisher], [GameName], [GameServerIP], [Birthday], [Sex], [RegDate]) VALUES(@i_AccountName, @i_GamePublisher, @i_GameName, @i_GameServerIP, @i_Birthday, @i_Sex, @i_RegDate) **/
#define	QDB_GLOG_ACCOUNT_DELETE		L"GLog_Account_Delete"			/** DELETE FROM GLog.dbo.GLog_Account WHERE [UID] = @i_UID **/

#define	QDB_GLOG_BUY_CASH_ITEM_SELECT	L"GLog_BuyCashItem_Select"			/* 	SELECT TOP 1000 [UID], [AccountName], [GamePublisher], [GameName], [CharacterName], [CharacterLevel], [ItemName], [CashCost], [ItemCount], [BuyDate] FROM GLog.dbo.GLog_BuyCashItem */
#define	QDB_GLOG_BUY_CASH_ITEM_INSERT	L"GLog_BuyCashItem_Insert"			/** INSERT INTO GLog.dbo.GLog_BuyCashItem([AccountName], [GamePublisher], [GameName], [CharacterName], [CharacterLevel], [ItemName], [CashCost], [ItemCount], [BuyDate]) VALUES(@i_AccountName, @i_GamePublisher, @i_GameName, @i_CharacterName, @i_CharacterLevel, @i_ItemName, @i_CashCost, @i_ItemCount, @i_BuyDate) **/
#define	QDB_GLOG_BUY_CASH_ITEM_DELETE	L"GLog_BuyCashItem_Delete"			/** DELETE FROM GLog.dbo.GLog_BuyCashItem WHERE [UID] = @i_UID **/

#define	QDB_GLOG_CHARACTER_PLAY_TIME_SELECT		L"GLog_CharacterPlayTime_Select"			/* 	SELECT TOP 1000 [UID], [AccountName], [GamePublisher], [GameName], [GameServerName], [CharacterName], [LoginDate], [LogoutDate], [TotalPlayTime], [PlayExp], [TotalExp], [Race], [Class], [Level], [MostStayedInZoneName] FROM GLog.dbo.GLog_CharacterPlayTime */
#define	QDB_GLOG_CHARACTER_PLAY_TIME_INSERT		L"GLog_CharacterPlayTime_Insert"			/** INSERT INTO GLog.dbo.GLog_CharacterPlayTime([AccountName], [GamePublisher], [GameName], [GameServerName], [CharacterName], [LoginDate], [LogoutDate], [TotalPlayTime], [PlayExp], [TotalExp], [Race], [Class], [Level], [MostStayedInZoneName]) VALUES(@i_AccountName, @i_GamePublisher, @i_GameName, @i_GameServerName, @i_CharacterName, @i_LoginDate, @i_LogoutDate,	@i_TotalPlayTime, @i_PlayExp, @i_TotalExp, @i_Race, @i_Class, @i_Level, @i_MostStayedInZoneName) **/
#define	QDB_GLOG_CHARACTER_PLAY_TIME_DELETE		L"GLog_CharacterPlayTime_Delete"			/** DELETE FROM GLog.dbo.GLog_CharacterPlayTime WHERE [UID] = @i_UID **/

#define	QDB_GLOG_CONNECT_TOTAL_USER_COUNT_SELECT		L"GLog_ConnectTotalUserCount_Select"			/* 	SELECT TOP 1000 [UID], [Date],	[GamePublisher], [GameName], [GameServerName], [TotalUserCount] FROM GLog.dbo.GLog_ConnectTotalUserCount */
#define	QDB_GLOG_CONNECT_TOTAL_USER_COUNT_INSERT		L"GLog_ConnectTotalUserCount_Insert"			/** INSERT INTO GLog.dbo.GLog_ConnectTotalUserCount([Date],	[GamePublisher], [GameName], [GameServerName], [TotalUserCount]) VALUES(@i_Date, @i_GamePublisher, @i_GameName, @i_GameServerName, @i_TotalUserCount) **/
#define	QDB_GLOG_CONNECT_TOTAL_USER_COUNT_DELETE		L"GLog_ConnectTotalUserCount_Delete"			/** DELETE FROM GLog.dbo.GLog_ConnectTotalUserCount WHERE [UID] = @i_UID **/

#define	QDB_GLOG_CONNECT_ZONE_USER_COUNT_SELECT		L"GLog_ConnectZoneUserCount_Select"			/* 	SELECT TOP 1000 [UID], [Date],	[GamePublisher], [GameName], [GameServerName], [ZoneName], [ZoneUserCount] FROM GLog.dbo.GLog_ConnectZoneUserCount */
#define	QDB_GLOG_CONNECT_ZONE_USER_COUNT_INSERT		L"GLog_ConnectZoneUserCount_Insert"			/** INSERT INTO GLog.dbo.GLog_ConnectZoneUserCount([Date],	[GamePublisher], [GameName], [GameServerName], [ZoneName], [ZoneUserCount]) VALUES(@i_Date,	@i_GamePublisher, @i_GameName, @i_GameServerName, @i_ZoneName, @i_ZoneUserCount) **/
#define	QDB_GLOG_CONNECT_ZONE_USER_COUNT_DELETE		L"GLog_ConnectZoneUserCount_Delete"			/** DELETE FROM GLog.dbo.GLog_ConnectZoneUserCount WHERE [UID] = @i_UID **/

#define	QDB_GLOG_EVENT_SELECT		L"GLog_Event_Select"			/* 	SELECT TOP 1000 [UID], [GamePublisher], [GameName], [GameServerName], [StartDate], [EndDate], [Description] FROM GLog.dbo.GLog_Event */
#define	QDB_GLOG_EVENT_INSERT		L"GLog_Event_Insert"			/** INSERT INTO GLog.dbo.GLog_Event([GamePublisher], [GameName], [GameServerName], [StartDate], [EndDate], [Description]) VALUES(@i_GamePublisher, @i_GameName, @i_GameServerName, @i_StartDate, @i_EndDate, @i_Description) **/
#define	QDB_GLOG_EVENT_DELETE		L"GLog_Event_Delete"			/** DELETE FROM GLog.dbo.GLog_Event WHERE [UID] = @i_UID **/

#define	QDB_GLOG_EVENT_PARTICIPATION_RATE_SELECT		L"GLog_EventParticipationRate_Select"			/* 	SELECT TOP 1000 [UID], [GamePublisher], [GameName], [GameServerName], [StartDate], [EndDate], [ParticipationCount], [ParticipationRate], [Description] FROM GLog.dbo.GLog_EventParticipationRate */
#define	QDB_GLOG_EVENT_PARTICIPATION_RATE_INSERT		L"GLog_EventParticipationRate_Insert"			/** INSERT INTO GLog.dbo.GLog_EventParticipationRate([GamePublisher], [GameName], [GameServerName], [StartDate], [EndDate], [ParticipationCount], [ParticipationRate], [Description]) VALUES(@i_GamePublisher, @i_GameName, @i_GameServerName, @i_StartDate, @i_EndDate, @i_ParticipationCount, @i_ParticipationRate, @i_Description) **/
#define	QDB_GLOG_EVENT_PARTICIPATION_RATE_DELETE		L"GLog_EventParticipationRate_Delete"			/** DELETE FROM GLog.dbo.GLog_EventParticipationRate WHERE [UID] = @i_UID **/

// start 2011-12-12 by hskim, GLog 2차
#define	QDB_GLOG_TB_USER_SELECT				L"GLog_TB_USER_Select"
#define	QDB_GLOG_TB_USER_INSERT				L"usp_GLog_UPDATE_TB_USER"
#define	QDB_GLOG_TB_USER_DELETE				L"GLog_TB_USER_Delete"

#define	QDB_GLOG_TB_CONNECT_USER_SELECT		L"GLog_TB_CONNECT_USER_Select"
#define	QDB_GLOG_TB_CONNECT_USER_INSERT		L"usp_GLog_INSERT_TB_CONNECT_USER"
#define	QDB_GLOG_TB_CONNECT_USER_DELETE		L"GLog_TB_CONNECT_USER_Delete"

#define	QDB_GLOG_TB_ITEM_BUY_USER_SELECT	L"GLog_TB_ITEM_BUY_Select"
#define	QDB_GLOG_TB_ITEM_BUY_USER_INSERT	L"usp_GLog_INSERT_TB_ITEM_BUY"
#define	QDB_GLOG_TB_ITEM_BUY_USER_DELETE	L"GLog_TB_ITEM_BUY_Delete"

#define	QDB_GLOG_TB_CONCURRENT_USER_SELECT	L"GLog_TB_CONCURRENT_USER_Select"
#define	QDB_GLOG_TB_CONCURRENT_USER_INSERT	L"usp_GLog_INSERT_TB_CONCURRENT_USER"
#define	QDB_GLOG_TB_CONCURRENT_USER_DELETE	L"GLog_TB_CONCURRENT_USER_Delete"

#define	QDB_GLOG_TB_USER_PLAYTIME_SELECT	L"GLog_TB_USER_PLAYTIME_Select"
#define	QDB_GLOG_TB_USER_PLAYTIME_INSERT	L"usp_GLog_INSERT_TB_USER_PLAYTIME"
#define	QDB_GLOG_TB_USER_PLAYTIME_DELETE	L"GLog_TB_USER_PLAYTIME_Delete"
// end 2011-12-12 by hskim, GLog 2차

// start 2012-10-08 by bckim, GLog 3차
//GLog_AccountConnect
#define QDB_GLOG_ACCOUNTCONNECT_SELECT		L"GLog_GetAccountConnect"
//#define QDB_GLOG_ACCOUNTCONNECT_INSERT		L"GLog_ImportAcconutConnect"
#define QDB_GLOG_ACCOUNTCONNECT_INSERT		L"usp_GLOG_INSERT_GLOG_ACCOUNT_CONNECT"
#define QDB_GLOG_ACCOUNTCONNECT_DELETE		L"GLog_DeleteAccountConnect"
// GLogDataAccountConnect.cpp

//GLog_CharData
#define QDB_GLOG_CHARDATA_SELECT		L"GLog_GetCharData"
//#define QDB_GLOG_CHARDATA_INSERT		L"GLog_ImportCharData"
#define QDB_GLOG_CHARDATA_INSERT		L"usp_GLOG_INSERT_GLOG_CHARDATA"
#define QDB_GLOG_CHARDATA_DELETE		L"GLog_DeleteCharData"
// GLogDataCharData.cpp

//GLog_ItemLog
#define QDB_GLOG_ITEMLOG_SELECT			L"GLog_GetItemLog"
//#define QDB_GLOG_ITEMLOG_INSERT		L"GLog_ImportItemLog"
#define QDB_GLOG_ITEMLOG_INSERT			L"usp_GLOG_INSERT_GLOG_ITEM_LOG"
#define QDB_GLOG_ITEMLOG_DELETE			L"GLog_DeleteItemLog"
// GLogDataItemLog.cpp

//GLog_ItemStateLog
#define QDB_GLOG_ITEMSTATELOG_SELECT		L"GLog_GetItemStateLog"
//#define QDB_GLOG_ITEMSTATELOG_INSERT		L"GLog_ImportItemStateLog"
#define QDB_GLOG_ITEMSTATELOG_INSERT		L"usp_GLOG_INSERT_GLOG_ITEM_STATE_LOG"
#define QDB_GLOG_ITEMSTATELOG_DELETE		L"GLog_DeleteItemStateLog"
// GLogDataItemStateLog.cpp

//GLog_ServerLog
#define QDB_GLOG_SERVERLOG_SELECT			L"GLog_GetServerLog"
//#define QDB_GLOG_SERVERLOG_INSERT			L"GLog_ImportServerLog"
#define QDB_GLOG_SERVERLOG_INSERT			L"usp_GLOG_INSERT_GLOG_SERVER_LOG"
#define QDB_GLOG_SERVERLOG_DELETE			L"GLog_DeleteServerLog"
// GLogDataServerLog.cpp

// end 2012-10-08 by bckim, GLog 3차