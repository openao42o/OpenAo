// 2005-04-28 by cmkwon

// #include "StringDefineCommon.h"
#ifndef _STRING_DEFINE_COMMON_H_
#define _STRING_DEFINE_COMMON_H_

///////////////////////////////////////////////////////////////////////////////
// 1 - Command handled at the FieldServer
	#define STRCMD_CS_COMMAND_GET_WARPOINT_ALL_USER		"/wpalluser"
	#define STRCMD_CS_COMMAND_GET_RND_DGA_TIMES			"/getrndtimes"
	#define STRCMD_CS_COMMAND_MENT_0					"/ment"
	#define STRCMD_CS_COMMAND_MENT_1					"/ment"
	#define STRCMD_CS_COMMAND_MENT_2					"/ment"
	#define STRCMD_CS_COMMAND_MENT_HELP					"format: /ment [|String] - Setting character's ment.  Leave blank string to cancel current ment."
	#define STRCMD_CS_COMMAND_MOVE						"/move"
	#define STRCMD_CS_COMMAND_MOVE_1					"/go"
	#define STRCMD_CS_COMMAND_MOVE_HELP					"format: /move [MapIndex] [|ChannelIndex] - Moves to the specific map & channel"
	#define STRCMD_CS_COMMAND_COORDINATE				"/coor"
	#define STRCMD_CS_COMMAND_COORDINATE_1				"/coordinate"
	#define STRCMD_CS_COMMAND_COORDINATE_HELP			"format: /coordinate [X] [Y] - Moves to the specific coordinates of the map"
	#define STRCMD_CS_COMMAND_LIST						"/list"
	#define STRCMD_CS_COMMAND_LIST_1					"/list"
	#define STRCMD_CS_COMMAND_LIST_HELP					"format: /list - Lists the users in the present map (maximum of 20 users)"
	#define STRCMD_CS_COMMAND_USERSEND					"/senduser"
	#define STRCMD_CS_COMMAND_USERSEND_1				"/senduser"
	#define STRCMD_CS_COMMAND_USERSEND_HELP				"format: /senduser [character name] [map name] - Moves the character into the specified map"
	#define STRCMD_CS_COMMAND_INFObyNAME				"/info"
	#define STRCMD_CS_COMMAND_INFObyNAME_1				"/info"
	#define STRCMD_CS_COMMAND_INFObyNAME_HELP			"format: /info [monster name|item name] - Shows the information about monsters or items included in the string"
	#define STRCMD_CS_COMMAND_QUESTINFO					"/quest"
	#define STRCMD_CS_COMMAND_QUESTINFO_1				"/quest"
	#define STRCMD_CS_COMMAND_QUESTINFO_HELP			"format: /quest - Shows characters quest information"
	#define STRCMD_CS_COMMAND_QUESTDEL					"/delQuest"
	#define STRCMD_CS_COMMAND_QUESTDEL_1				"/delQuest"
	#define STRCMD_CS_COMMAND_QUESTDEL_HELP				"format: /delQuest [quest no.]"
	#define STRCMD_CS_COMMAND_ITEMINFObyKIND			"/itemKind"
	#define STRCMD_CS_COMMAND_ITEMINFObyKIND_1			"/itemKind"
	#define STRCMD_CS_COMMAND_ITEMINFObyKIND_HELP		"format: /itemKind [|item kind(0~53)] - Shows the specific types of item"
	#define STRCMD_CS_COMMAND_ITEMINSERTbyKIND			"/insertItemKind"
	#define STRCMD_CS_COMMAND_ITEMINSERTbyKIND_1		"/insertItemKind"
	#define STRCMD_CS_COMMAND_ITEMINSERTbyKIND_HELP		"format: /insertItemKind [item kind(0~53)] -  Additional specific types of items"
	#define STRCMD_CS_COMMAND_ITEMINSERTbyITEMNUMRANGE		"/insertItemNumRange"
	#define STRCMD_CS_COMMAND_ITEMINSERTbyITEMNUMRANGE_1	"/insertItemNumRange"
	#define STRCMD_CS_COMMAND_ITEMINSERTbyITEMNUMRANGE_HELP	"format: /insertItemNumRange [from itemnum] ~ [to itemnum] - Additional specific items"
	#define STRCMD_CS_COMMAND_STATINIT					"/initStat"
	#define STRCMD_CS_COMMAND_STATINIT_1				"/initStatus"
	#define STRCMD_CS_COMMAND_STATINIT_2				"/initStatus"
	#define STRCMD_CS_COMMAND_STATINIT_HELP				"format: /initStatus - Initiates the entire status"
	#define STRCMD_CS_COMMAND_PARTYINFO					"/partyInfo"
	#define STRCMD_CS_COMMAND_PARTYINFO_1				"/partyInfo"
	#define STRCMD_CS_COMMAND_PARTYINFO_HELP			"format: /partyInfo - Shows party information"
	#define STRCMD_CS_COMMAND_GAMETIME					"/Time"
	#define STRCMD_CS_COMMAND_GAMETIME_1				"/Time"
	#define STRCMD_CS_COMMAND_GAMETIME_HELP				"format: /Time [|additional time(0~23)] - Changes present time (individual time is changed only)"
	#define STRCMD_CS_COMMAND_STRINGLEVEL_0				"/string"
	#define STRCMD_CS_COMMAND_STRINGLEVEL_1				"/string"
	#define STRCMD_CS_COMMAND_STRINGLEVEL_2				"/string"
	#define STRCMD_CS_COMMAND_STRINGLEVEL_HELP			"format: /string [0~5] - Decides levels shown by debug message"
	#define STRCMD_CS_COMMAND_MONSUMMON					"/summon"
	#define STRCMD_CS_COMMAND_MONSUMMON_1				"/summon"
	#define STRCMD_CS_COMMAND_MONSUMMON_HELP			"format: /summon [monster number|monster name] [# of monsters] - Brings monster ('_'is used if the monster's name includes a space in between)"
	#define STRCMD_CS_COMMAND_SKILLALL					"/allSkill"
	#define STRCMD_CS_COMMAND_SKILLALL_1				"/allSkill"
	#define STRCMD_CS_COMMAND_SKILLALL_HELP				"format: /allSkill [level] - Inserts all the matching skills"
	#define STRCMD_CS_COMMAND_ITEMINSERTALL				"/allItem"
	#define STRCMD_CS_COMMAND_ITEMINSERTALL_1			"/allItem"
	#define STRCMD_CS_COMMAND_ITEMINSERTALL_HELP		"format: /allItem - Inserts all the specific items excluding skills and countable items"
	#define STRCMD_CS_COMMAND_ITEMINSERTWEAPON			"/allWeapon"
	#define STRCMD_CS_COMMAND_ITEMINSERTWEAPON_1		"/allWeapon"
	#define STRCMD_CS_COMMAND_ITEMINSERTWEAPON_HELP		"format: /allWeapon - Additional entire weapons suitable for the Gear"
	#define STRCMD_CS_COMMAND_ITEMDELALL_0				"/delAllItem"
	#define STRCMD_CS_COMMAND_ITEMDELALL_1				"/clearInv"
	#define STRCMD_CS_COMMAND_ITEMDELALL_2				"/delAllItem"
	#define STRCMD_CS_COMMAND_ITEMDELALL_HELP			"format: /delAllItem - Eliminates all the unequipped items (excluding skills)"
	#define STRCMD_CS_COMMAND_ITEMINSERTbyITEMNUM		"/item"
	#define STRCMD_CS_COMMAND_ITEMINSERTbyITEMNUM_1		"/item"
	#define STRCMD_CS_COMMAND_ITEMINSERTbyITEMNUM_HELP	"format: /item [item number] [# of items] - Acquires items, treated as 1 item when [# of items] is not inputted"
	#define STRCMD_CS_COMMAND_ITEMDROP					"/dropItem"
	#define STRCMD_CS_COMMAND_ITEMDROP_1				"/dropItem"
	#define STRCMD_CS_COMMAND_ITEMDROP_HELP				"format: /dropItem [item number] [|# of items] - Dropping of items in the field"
	#define STRCMD_CS_COMMAND_USERSINFOTOTAL			"/server"
	#define STRCMD_CS_COMMAND_USERSINFOTOTAL_1			"/server"
	#define STRCMD_CS_COMMAND_USERSINFOTOTAL_HELP		"format: /server - Listing server information"
	#define STRCMD_CS_COMMAND_USERSINFOperMAP			"/serverMap"
	#define STRCMD_CS_COMMAND_USERSINFOperMAP_1			"/serverMap"
	#define STRCMD_CS_COMMAND_USERSINFOperMAP_HELP		"format: /serverMap - Shows information of all the map"
	#define STRCMD_CS_COMMAND_CHANNELINFO				"/channelInfo"
	#define STRCMD_CS_COMMAND_CHANNELINFO_1				"/channelInfo"
	#define STRCMD_CS_COMMAND_CHANNELINFO_HELP			"format: /channelInfo - Shows maps channel information on the present map"
	#define STRCMD_CS_COMMAND_DEBUGPRINTDBG				"/dbg"
	#define STRCMD_CS_COMMAND_DEBUGPRINTDBG_1			"/dbg"
	#define STRCMD_CS_COMMAND_DEBUGPRINTDBG_HELP		"format: /dbg - For test only"
	#define STRCMD_CS_COMMAND_DEBUGSETPARAMF			"/testf"
	#define STRCMD_CS_COMMAND_DEBUGSETPARAMF_HELP		"format: /testf [Param1] [Param2] [Param3]"
	#define STRCMD_CS_COMMAND_BULLETCHARGE				"/bullet"
	#define STRCMD_CS_COMMAND_BULLETCHARGE_1			"/chargeBullet"
	#define STRCMD_CS_COMMAND_BULLETCHARGE_HELP			"format: /chargeBullet [|no. of 1st type bullet] [|no. of 2nd type bullet] - reloading bullets"
	#define STRCMD_CS_COMMAND_REPAIRALL					"/manpi"
	#define STRCMD_CS_COMMAND_REPAIRALL_1				"/repairAll"
	#define STRCMD_CS_COMMAND_REPAIRALL_HELP			"format: /repairAll [|character name] - Healing Energy, Shield, SP, Fuel to 100%, Healing yourself without [character name]"
	#define STRCMD_CS_COMMAND_REPAIRbyPARAM				"/banpi"
	#define STRCMD_CS_COMMAND_REPAIRbyPARAM_1			"/repairParam"
	#define STRCMD_CS_COMMAND_REPAIRbyPARAM_HELP		"format: /repairParam [|decreasing quantity %] [|character name] - Energy, Shield, SP, and Fuel accounts for [decreasing quantity %], 50% if [decreasing quantity %] is not inputted, and if [character name] is not present it stands for the individual's character"
	#define STRCMD_CS_COMMAND_USERNORMALIZE				"/normal"
	#define STRCMD_CS_COMMAND_USERNORMALIZE_1			"/normal"
	#define STRCMD_CS_COMMAND_USERNORMALIZE_HELP		"format: /normal - Change to a normal account from an admin or game master mode"
	#define STRCMD_CS_COMMAND_USERSPECIALIZE			"/specialize"
	#define STRCMD_CS_COMMAND_USERSPECIALIZE_1			"/specialize"
	#define STRCMD_CS_COMMAND_USERSPECIALIZE_HELP		"format: /specialize - Changed from a normal account to a special admin or game master account"
	#define STRCMD_CS_COMMAND_USERINVINCIBILITY			"/godmode"
	#define STRCMD_CS_COMMAND_USERINVINCIBILITY_1		"/invincible"
	#define STRCMD_CS_COMMAND_USERINVINCIBILITY_HELP	"format: /invincible - As a game master or manager mode, no damage is taken"
	#define STRCMD_CS_COMMAND_POWERUP					"/powerUp"
	#define STRCMD_CS_COMMAND_POWERUP_1					"/powerUp"
	#define STRCMD_CS_COMMAND_POWERUP_HELP				"format: /powerUp [Increased offensive power(%%)]"
	#define STRCMD_CS_COMMAND_VARIABLESET				"/setVariable"
	#define STRCMD_CS_COMMAND_VARIABLESET_1				"/setVariable"
	#define STRCMD_CS_COMMAND_VARIABLESET_HELP			"format: /setVariable [variable] - adjusts (normal) variables"
	#define STRCMD_CS_COMMAND_LEVELSET					"/level"
	#define STRCMD_CS_COMMAND_LEVELSET_1				"/level"
	#define STRCMD_CS_COMMAND_LEVELSET_HELP				"format: /level [|level] [|percentage of exp] [|character name] - Adjusts level and percentage of exp. of a character"


	#define STRCMD_CS_COMMAND_PARTNERLEVELSET			"/partnerlevel"
	#define STRCMD_CS_COMMAND_PARTNERLEVELSET_1			"/partnerlevel"
	#define STRCMD_CS_COMMAND_PARTNERLEVELSET_HELP		"format: /partnerlevel [|level] [|percentage of exp] "
	
	
	#define STRCMD_CS_COMMAND_USERINVISIABLE			"/invisible"
	#define STRCMD_CS_COMMAND_USERINVISIABLE_1			"/hide"
	#define STRCMD_CS_COMMAND_USERINVISIABLE_HELP		"format: /invisible -  Invisible from other characters"
	#define STRCMD_CS_COMMAND_DEBUGPRINTMSGF_0			"/messagef"
	#define STRCMD_CS_COMMAND_DEBUGPRINTMSGF_1			"/msgf"
	#define STRCMD_CS_COMMAND_DEBUGPRINTMSGF_HELP		"format: /msgf - For test only"
	#define STRCMD_CS_COMMAND_GAMEEVENT					"/event"
	#define STRCMD_CS_COMMAND_GAMEEVENT_1				"/event"
	#define STRCMD_CS_COMMAND_GAMEEVENT_P1EXP			"exppoint"
	#define STRCMD_CS_COMMAND_GAMEEVENT_P1SPI			"SPI"
	#define STRCMD_CS_COMMAND_GAMEEVENT_P1EXPR			"restoreexppoint"
	#define STRCMD_CS_COMMAND_GAMEEVENT_P1ITEM			"item"
	#define STRCMD_CS_COMMAND_GAMEEVENT_P1RARE			"rareitem"
	#define STRCMD_CS_COMMAND_GAMEEVENT_P2END			"finish"
	#define STRCMD_CS_COMMAND_GAMEEVENT_HELP			"format: /event [exppoint|SPI|restoreexppoint|item|rareitem] [|rate %f|finish] [time(minute)] - event set, cancel"
	#define STRCMD_CS_COMMAND_PREMEUM					"/premium"
	#define STRCMD_CS_COMMAND_PREMEUM_1					"/premium"
	#define STRCMD_CS_COMMAND_PREMEUM_PNORMAL			"standard"
	#define STRCMD_CS_COMMAND_PREMEUM_PSUPER			"super"
	#define STRCMD_CS_COMMAND_PREMEUM_PUPGRADE			"upgrade"
	#define STRCMD_CS_COMMAND_PREMEUM_PEND				"finish"
	#define STRCMD_CS_COMMAND_PREMEUM_HELP				"format: /premium [standard|super|upgrade|finish]"
// 2008-02-14 by cmkwon,   
//	#define STRCMD_CS_COMMAND_CITYWAR					"/citywar"
//	#define STRCMD_CS_COMMAND_CITYWAR_1					"/citywar"
//	#define STRCMD_CS_COMMAND_CITYWAR_PSTART			"start"
//	#define STRCMD_CS_COMMAND_CITYWAR_PEND				"finish"
//	#define STRCMD_CS_COMMAND_CITYWAR_HELP				"format: /citywar [start|finish]"
	#define STRCMD_CS_COMMAND_STEALTH					"/stealth"
	#define STRCMD_CS_COMMAND_STEALTH_1					"/stealth"
	#define STRCMD_CS_COMMAND_STEALTH_HELP				"format: /stealth"
	#define STRCMD_CS_COMMAND_RETURNALL					"/returnAll"
	#define STRCMD_CS_COMMAND_RETURNALL_1				"/returnAll"
	#define STRCMD_CS_COMMAND_RETURNALL_HELP			"format: /returnAll [MapIndex] - All the members in the map are moved to the nation capital"
	#define STRCMD_CS_COMMAND_USERCOUNT					"/count"

// start 2011-10-28 by hskim, EP4 [트리거 시스템] - 크리스탈 시스템
#define STRCMD_CS_COMMAND_RESETTRIGGER				"/resetTrigger"
#define STRCMD_CS_COMMAND_RESETTRIGGER_1			"/resetTrigger"
#define STRCMD_CS_COMMAND_RESETTRIGGER_HELP			"format: /resetTrigger [TriggerNumber] - Off the trigger and default the event."
// end 2011-10-28 by hskim, EP4 [트리거 시스템] - 크리스탈 시스템


// start 2011-06-22 by hskim, 사설 서버 방지
	#define STRCMD_CS_COMMAND_SERVERINFO				"/getserverinfo"		// 서버 정보 보기
// end 2011-06-22 by hskim, 사설 서버 방지
			   
// start 2012-02-13 by hskim, 몬스터 아이템 드랍 테스트 명령어
#define STRCMD_CS_COMMAND_TEST_MONSTER_DROPITEM			"/testmonsterdrop"		// 서버 정보 보기
#define STRCMD_CS_COMMAND_TEST_MONSTER_DROPITEM_1		"/testmonsterdrop"
#define STRCMD_CS_COMMAND_TEST_MONSTER_DROPITEM_HELP	"format: /testmonsterdrop [monster number] [test numbers : (1~100000)] - Monsters that drop items at the show simulation. (A very large load on the server gives an absolute Sacred Shield Do not use)"
// end 2012-02-13 by hskim, 몬스터 아이템 드랍 테스트 명령어

// 2007-10-30 by cmkwon, 세력별 해피아워 이벤트 구현 - 명령어 형식 수정됨 아래에서 다시 정의 함
//	#define STRCMD_CS_COMMAND_HAPPYHOUREVENT			"/happyEvent"
//	#define STRCMD_CS_COMMAND_HAPPYHOUREVENT_1			"/happyEvent"
//	#define STRCMD_CS_COMMAND_HAPPYHOUREVENT_PSTART		"start"
//	#define STRCMD_CS_COMMAND_HAPPYHOUREVENT_PEND		"end"
//	#define STRCMD_CS_COMMAND_HAPPYHOUREVENT_HELP		"format: /happyEvent [start|end] [progress time(unit:min.)]"

// 1_end
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// 2 - Command used in IMServer, some are used with the same command as the above
	#define STRCMD_CS_COMMAND_DEBUGSETPARAMI			"/testi"
	#define STRCMD_CS_COMMAND_DEBUGSETPARAMI_HELP		"format: /testi - IMServer for debug"
	#define STRCMD_CS_COMMAND_WHO						"/who"
	#define STRCMD_CS_COMMAND_WHO_1						"/who"
	#define STRCMD_CS_COMMAND_WHO_HELP					"format: /who [|# of users] - Listing all users presently in the server (unrelated to map)"
	#define STRCMD_CS_COMMAND_REGISTERADMIN				"/registerAdmin"
	#define STRCMD_CS_COMMAND_REGISTERADMIN_1			"/registerAdmin"
	#define STRCMD_CS_COMMAND_REGISTERADMIN_HELP		"format: /registerAdmin - Registers the server to send a message to the admin in the process of an event"
	#define STRCMD_CS_COMMAND_DEBUGPRINTMSGI_0			"/messagei"
	#define STRCMD_CS_COMMAND_DEBUGPRINTMSGI_1			"/msgi"
	#define STRCMD_CS_COMMAND_DEBUGPRINTMSGI_HELP		"format: /msgi - Shows all the protocol that a client and the IM server sends and receives"
	#define STRCMD_CS_COMMAND_SERVERDOWN				"/serverDown"
	#define STRCMD_CS_COMMAND_SERVERDOWN_1				"/serverDown"
	#define STRCMD_CS_COMMAND_SERVERDOWN_HELP			"format: /serverDown [certified no.] - Server shutdown"
	#define STRCMD_CS_COMMAND_WHOAREYOU					"/whoareYou"
	#define STRCMD_CS_COMMAND_WHOAREYOU_1				"/whoareYou"
	#define STRCMD_CS_COMMAND_WHOAREYOU_HELP			"format: /whoareYou [character name]"
	#define STRCMD_CS_COMMAND_GOUSER					"/go"
	#define STRCMD_CS_COMMAND_GOUSER_1					"/go"
	#define STRCMD_CS_COMMAND_GOUSER_HELP				"format: /go [character name] - Moves to the specific position of the character"
	#define STRCMD_CS_COMMAND_COMEON					"/comeon"
	#define STRCMD_CS_COMMAND_COMEON_1					"/comeon"
	#define STRCMD_CS_COMMAND_COMEON_HELP				"format: /comeon [character name] - Summons the specific character"
	#define STRCMD_CS_COMMAND_GUILDCOMEON				"/comeonGuild"
	#define STRCMD_CS_COMMAND_GUILDCOMEON_1				"/comeonGuild"
	#define STRCMD_CS_COMMAND_GUILDCOMEON_HELP			"format: /comeonGuild [guild name] - Summons the whole guild"
	#define STRCMD_CS_COMMAND_GUILDSEND					"/sendGuild"
	#define STRCMD_CS_COMMAND_GUILDSEND_1				"/sendGuild"
	#define STRCMD_CS_COMMAND_GUILDSEND_HELP			"format: /sendGuild [guild name] [map name] - Sends the guild into a specific map"
	#define STRCMD_CS_COMMAND_CHATPTOPFLAG				"/whisperChat"
	#define STRCMD_CS_COMMAND_CHATPTOPFLAG_1			"/whisperChat"
	#define STRCMD_CS_COMMAND_CHATPTOPFLAG_HELP			"format: /whisperChat - Toggles between blocking and allowing whispers"
	#define STRCMD_CS_COMMAND_GUILDINFO					"/guildInfo"
	#define STRCMD_CS_COMMAND_GUILDINFO_1				"/guildInfo"
	#define STRCMD_CS_COMMAND_GUILDINFO_HELP			"format: /guildInfo - Shows guild information"
	#define STRCMD_CS_COMMAND_WEATHERSET				"/weather"
	#define STRCMD_CS_COMMAND_WEATHERSET_1				"/weather"
	#define STRCMD_CS_COMMAND_WEATHERSET_P1NORMAL		"standard"
	#define STRCMD_CS_COMMAND_WEATHERSET_P1FINE			"clear"
	#define STRCMD_CS_COMMAND_WEATHERSET_P1RAIN			"rain"
	#define STRCMD_CS_COMMAND_WEATHERSET_P1SNOW			"snow"
	#define STRCMD_CS_COMMAND_WEATHERSET_P1CLOUDY		"cloudy"
	#define STRCMD_CS_COMMAND_WEATHERSET_P1FOG			"foggy"
	#define STRCMD_CS_COMMAND_WEATHERSET_P2ALL			"whole"
	#define STRCMD_CS_COMMAND_WEATHERSET_P3ON			"on"
	#define STRCMD_CS_COMMAND_WEATHERSET_P3OFF			"off"
	#define STRCMD_CS_COMMAND_WEATHERSET_HELP			"format: /weather [standard|clear|rain|snow|cloudy|foggy] [whole|mapname] [on|off] - Controls the weather"
	#define STRCMD_CS_COMMAND_CHATFORBID				"/mute"
	#define STRCMD_CS_COMMAND_CHATFORBID_1				"/forbidChat"
	#define STRCMD_CS_COMMAND_CHATFORBID_HELP			"format: /forbidChat [character name] [time(min.)] - Prohibiting chat"
	#define STRCMD_CS_COMMAND_CHATFORBIDRELEASE			"/unmute"
	#define STRCMD_CS_COMMAND_CHATFORBIDRELEASE_1		"/releaseChat"
	#define STRCMD_CS_COMMAND_CHATFORBIDRELEASE_HELP	"format: /releaseChat [time(min.)] - Cancelling prohibited chat"
	#define STRCMD_CS_COMMAND_COMMANDLIST_0				"/?"
	#define STRCMD_CS_COMMAND_COMMANDLIST_1				"/help"
	#define STRCMD_CS_COMMAND_COMMANDLIST_2				"/command"
	#define STRCMD_CS_COMMAND_COMMANDLIST_HELP			"format: /? - command list is shown"

	// 2005-07-20 by cmkwon
	#define STRCMD_CS_COMMAND_BONUSSTAT_0				"/BonusStat"
	#define STRCMD_CS_COMMAND_BONUSSTAT_1				"/BonusStat"
	#define STRCMD_CS_COMMAND_BONUSSTAT_2				"/BonusStat"
	#define STRCMD_CS_COMMAND_BONUSSTAT_HELP			"format: /BonusStat [Bonus Counts] [|character name] - BonusStat increase"

// 2_end
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
// 3 - Command used in AtumMonitor, some are used with the same command as the above
	#define STRCMD_CS_COMMAND_PASSWORDSET				"/setPassword"
	#define STRCMD_CS_COMMAND_PASSWORDSET_1				"/setPassword"
	#define STRCMD_CS_COMMAND_PASSWORDSET_HELP			"format: /setPassword [AccountName] [Password]"
	#define STRCMD_CS_COMMAND_PASSWORDROLLBACK			"/rollbackPassword"
	#define STRCMD_CS_COMMAND_PASSWORDROLLBACK_1		"/rollbackPassword"
	#define STRCMD_CS_COMMAND_PASSWORDROLLBACK_HELP		"format: /rollbackPassword [AccountName]"
	#define STRCMD_CS_COMMAND_PASSWORDLIST				"/passwordList"
	#define STRCMD_CS_COMMAND_PASSWORDLIST_1			"/passwordList"
	#define STRCMD_CS_COMMAND_PASSWORDLIST_HELP			"format: /passwordList"
	#define STRCMD_CS_COMMAND_PASSWORDENCRYPT			"/encrypt"
	#define STRCMD_CS_COMMAND_PASSWORDENCRYPT_1			"/encrypt"
	#define STRCMD_CS_COMMAND_PASSWORDENCRYPT_HELP		"format: /encrypt [string that will be encrypted]"
	#define STRCMD_CS_COMMAND_ACCOUNTBLOCK				"/blockAccount"
	#define STRCMD_CS_COMMAND_ACCOUNTBLOCK_1			"/blockAccount"
	#define STRCMD_CS_COMMAND_ACCOUNTBLOCKT_HELP		"format: /blockAccount [AccountName]"
	#define STRCMD_CS_COMMAND_ACCOUNTBLOCKRELEASE		"/releaseAccount"
	#define STRCMD_CS_COMMAND_ACCOUNTBLOCKRELEASE_1		"/releaseAccount"
	#define STRCMD_CS_COMMAND_ACCOUNTBLOCKRELEASE_HELP	"format: /releaseAccount [AccountName]"
	#define STRCMD_CS_COMMAND_ACCOUNTBLOCKLIST			"/blockedList"
	#define STRCMD_CS_COMMAND_ACCOUNTBLOCKLIST_1		"/blockedList"
	#define STRCMD_CS_COMMAND_ACCOUNTBLOCKLIST_HELP		"format: /blockedList"
// 3_end
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
// 4 - CommonGameServer
	#define STRCMD_CS_COMMON_DB_0000 "Only related to Mgame server only!!!!\r\n"
	#define STRCMD_CS_COMMON_DB_0001 "Please enter server login name"
	#define STRCMD_CS_COMMON_DB_0002 "Please enter server login password"
	#define STRCMD_CS_COMMON_DB_0003 "Please enter DB log in name"
	#define STRCMD_CS_COMMON_DB_0004 "Please enter DB log in password"

	#define STRCMD_CS_COMMON_MAP_0000 "Hard coded part: Ignore number 1 warp target at 0101 map, easy way to delete from map editor need to be found and needs to be deleted!\r\n"
	#define STRCMD_CS_COMMON_MAP_0001 "MAP: %04d, m_DefaltWarpTargetIndex: %d\r\n"
	#define STRCMD_CS_COMMON_MAP_0002 "Hard coded part: Ignore number 1 warp target at 0101 map, easy way to delete from map editor need to be found and needs to be deleted!\r\n"
	#define STRCMD_CS_COMMON_MAP_0003 "MAP: %04d, m_DefaltWarpTargetIndex: %d\r\n"
	#define STRCMD_CS_COMMON_MAP_0004 "    ObjMon ==> ObjNum[%8d] EvType[%d] EvIndex[%3d] summon monster[%8d] summon time[%6dsecond], Pos(%4d, %4d, %4d)\r\n"
	#define STRCMD_CS_COMMON_MAP_0005 "[ERROR] ObjectMonster EventParam1 Index overlap Error ==> ObjectNum[%8d] EventType[%d] EventIndex[%3d] summon monster[%8d] summon time[%6dsecond], Pos(%4d, %4d, %4d)\r\n"
	#define STRCMD_CS_COMMON_MAP_0006 "  Tatal Monster Count : [%4d] <== Including object monster\r\n"

	#define STRCMD_CS_COMMON_DOWNLOAD_0000 "Download file error"
	#define STRCMD_CS_COMMON_DOWNLOAD_0001 "File creation error"
	#define STRCMD_CS_COMMON_DOWNLOAD_0002 "Download file read error"

	#define STRCMD_CS_COMMON_DATETIME_0000 "%dday%dhour%dminute%dsecond"

	#define STRCMD_CS_COMMON_RACE_NORMAL		"normal"
	#define STRCMD_CS_COMMON_RACE_BATTALUS		"Vatallus"
	#define STRCMD_CS_COMMON_RACE_DECA			"DECA"
	#define STRCMD_CS_COMMON_RACE_PHILON		"Phillon"
	#define STRCMD_CS_COMMON_RACE_SHARRINE		"Shrine"
	#define STRCMD_CS_COMMON_RACE_MONSTER1		"reserve"
	#define STRCMD_CS_COMMON_RACE_MONSTER2		"reserve"
	#define STRCMD_CS_COMMON_RACE_NPC			"NPC"
	#define STRCMD_CS_COMMON_RACE_OPERATION		"\\cAdministrator\\c"
	#define STRCMD_CS_COMMON_RACE_GAMEMASTER	"\\cGamemaster\\c"
	#define STRCMD_CS_COMMON_RACE_MONITOR		"\\cCommunity Manager\\c"
	#define STRCMD_CS_COMMON_RACE_GUEST			"Guest"
	#define STRCMD_CS_COMMON_RACE_DEMO			"For demo"
	#define STRCMD_CS_COMMON_RACE_ALL			"All race"
	#define STRCMD_CS_COMMON_RACE_UNKNOWN		"Unknown race"

	#define STRCMD_CS_COMMON_MAPNAME_UNKNOWN	"No name"

	#define STRCMD_CS_STATUS_BEGINNER_AIRMAN		"Training Airman"
	#define STRCMD_CS_STATUS_3RD_CLASS_AIRMAN		"3rd Airman"
	#define STRCMD_CS_STATUS_2ND_CLASS_AIRMAN		"2nd Airman"
	#define STRCMD_CS_STATUS_1ST_CLASS_AIRMAN		"1st Airman"
	#define STRCMD_CS_STATUS_3RD_CLASS_WINGMAN		"3rd Wingman"
	#define STRCMD_CS_STATUS_2ND_CLASS_WINGMAN		"2nd Wingman"
	#define STRCMD_CS_STATUS_1ST_CLASS_WINGMAN		"1st Wingman"
	#define STRCMD_CS_STATUS_3RD_CLASS_LEADER		"3rd Leader"
	#define STRCMD_CS_STATUS_2ND_CLASS_LEADER		"2nd Leader"
	#define STRCMD_CS_STATUS_1ST_CLASS_LEADER		"1st Leader"
	#define STRCMD_CS_STATUS_3RD_CLASS_ACE			"3rd Ace"
	#define STRCMD_CS_STATUS_2ND_CLASS_ACE			"2nd Ace"
	#define STRCMD_CS_STATUS_1ST_CLASS_ACE			"1st Ace"
	#define STRCMD_CS_STATUS_COPPER_CLASS_GENERAL	"Air-Commodore"
	#define STRCMD_CS_STATUS_SILVER_CLASS_GENERAL	"Air Vice-Marshal"
	#define STRCMD_CS_STATUS_GOLD_CLASS_GENERAL		"Lieutenant General"
	#define STRCMD_CS_STATUS_MASTER_GENERAL			"General"

	#define STRCMD_CS_ITEMKIND_AUTOMATIC			"Automatic type"
	#define STRCMD_CS_ITEMKIND_VULCAN				"Vulcan type"
	#define STRCMD_CS_ITEMKIND_DUALIST				"Dualist type"		// 2005-08-01 by hblee : changed from GRENADE to DUALIST.
	#define STRCMD_CS_ITEMKIND_CANNON				"Cannon type"
	#define STRCMD_CS_ITEMKIND_RIFLE				"Rifle type"
	#define STRCMD_CS_ITEMKIND_GATLING				"Gatling type"
	#define STRCMD_CS_ITEMKIND_LAUNCHER				"Launcher type"
	#define STRCMD_CS_ITEMKIND_MASSDRIVE			"Mass drive type"
	#define STRCMD_CS_ITEMKIND_ROCKET				"Rocket type"
	#define STRCMD_CS_ITEMKIND_MISSILE				"Missile type"
	#define STRCMD_CS_ITEMKIND_BUNDLE				"Bundle type"

	#define STRCMD_CS_ITEMKIND_MINE					"Mine type"
	#define STRCMD_CS_ITEMKIND_SHIELD				"Shield type"
	#define STRCMD_CS_ITEMKIND_DUMMY				"Dummy type"			
	#define STRCMD_CS_ITEMKIND_FIXER				"Fixer type"
	#define STRCMD_CS_ITEMKIND_DECOY				"Decoy type"
	#define STRCMD_CS_ITEMKIND_DEFENSE				"Armor type"
	#define STRCMD_CS_ITEMKIND_SUPPORT				"Engine type"
	#define STRCMD_CS_ITEMKIND_ENERGY				"Consumable type"
	#define STRCMD_CS_ITEMKIND_INGOT				"Mineral type"
	#define STRCMD_CS_ITEMKIND_CARD					"Standard Card type"
	#define STRCMD_CS_ITEMKIND_ENCHANT				"Enchant Card type"
	#define STRCMD_CS_ITEMKIND_TANK					"Tank type"
	#define STRCMD_CS_ITEMKIND_BULLET				"Bullet type"
	#define STRCMD_CS_ITEMKIND_QUEST				"Quest item type"
	#define STRCMD_CS_ITEMKIND_RADAR				"Radar type"
	#define STRCMD_CS_ITEMKIND_COMPUTER				"Computer type"
	#define STRCMD_CS_ITEMKIND_GAMBLE				"Gamble card type"
	#define STRCMD_CS_ITEMKIND_PREVENTION_DELETE_ITEM	"Enchant Delete Prevention Card type"
	#define STRCMD_CS_ITEMKIND_BLASTER				"Blaster type"	// 2005-08-01 by hblee : Blaster type added.
	#define STRCMD_CS_ITEMKIND_RAILGUN				"Rail gun type"		// 2005-08-01 by hblee : Rail gun type added.
	#define STRCMD_CS_ITEMKIND_ACCESSORY_UNLIMITED	"Unlimited Accessory"		// 2006-03-17 by cmkwon, 사용시간이 <영원>인 액세서리 아이템
	#define STRCMD_CS_ITEMKIND_ACCESSORY_TIMELIMIT	"Time limit Accessory"		// 2006-03-17 by cmkwon, 사용시간에 시간 제한이 있는 액세서리 아이템
	#define STRCMD_CS_ITEMKIND_ALL_WEAPON			"All weapons"
	#define STRCMD_CS_ITEMKIND_PRIMARY_WEAPON_ALL	"Standard Weapon"
	#define STRCMD_CS_ITEMKIND_PRIMARY_WEAPON_1		"Bullet type Standard Weapon"
	#define STRCMD_CS_ITEMKIND_PRIMARY_WEAPON_2		"Fuel type Standard Weapon"
	#define STRCMD_CS_ITEMKIND_SECONDARY_WEAPON_ALL	"Advanced Weapon"
	#define STRCMD_CS_ITEMKIND_SECONDARY_WEAPON_1	"Bullet type Advanced Weapon"
	#define STRCMD_CS_ITEMKIND_SECONDARY_WEAPON_2	"Shield type Advanced Weapon"
	#define STRCMD_CS_ITEMKIND_SKILL_ATTACK			"Attack skill"
	#define STRCMD_CS_ITEMKIND_SKILL_DEFENSE		"Defense skill"
	#define STRCMD_CS_ITEMKIND_SKILL_SUPPORT		"Support skill"
	#define STRCMD_CS_ITEMKIND_SKILL_ATTRIBUTE		"Attribute skill"
	#define STRCMD_CS_ITEMKIND_FOR_MON_PRIMARY		"Item for 1 type monster"
	#define STRCMD_CS_ITEMKIND_FOR_MON_GUN			"Monster machine gun type(1-1type)"
	#define STRCMD_CS_ITEMKIND_FOR_MON_BEAM			"Monster beam type(1-2type)"
	#define STRCMD_CS_ITEMKIND_FOR_MON_ALLATTACK	"Attack all monster"
	#define STRCMD_CS_ITEMKIND_FOR_MON_SECONDARY	"Item for 2 type monster"
	#define STRCMD_CS_ITEMKIND_FOR_MON_ROCKET		"Monster rocket(2-1type)"
	#define STRCMD_CS_ITEMKIND_FOR_MON_MISSILE		"Monster Missile type(2-1type)"
	#define STRCMD_CS_ITEMKIND_FOR_MON_BUNDLE		"Monster Bundle type(2-1type)"
	#define STRCMD_CS_ITEMKIND_FOR_MON_MINE			"Monster Mine type(2-1type)"
	#define STRCMD_CS_ITEMKIND_FOR_MON_SHIELD		"Monster shield type(2-2type)"
	#define STRCMD_CS_ITEMKIND_FOR_MON_DUMMY		"Monster dummy type(2-2type)"
	#define STRCMD_CS_ITEMKIND_FOR_MON_FIXER		"Monster Fixer type(2-2type)"
	#define STRCMD_CS_ITEMKIND_FOR_MON_DECOY		"Monster Decoy type(2-2type)"
	#define STRCMD_CS_ITEMKIND_FOR_MON_FIRE			"Monster Fire type"
	#define STRCMD_CS_ITEMKIND_FOR_MON_OBJBEAM		"Monster crash possible beam type"
	#define STRCMD_CS_ITEMKIND_FOR_MON_STRAIGHTBOOM	"Monster straight bomb type"
	#define STRCMD_CS_ITEMKIND_UNKNOWN				"Unknown item"

	#define STRCMD_CS_UNITKIND_UNKNOWN				"Unknown airframe"

	#define STRCMD_CS_STAT_ATTACK_PART				"Attack"
	#define STRCMD_CS_STAT_DEFENSE_PART				"Defense"
	#define STRCMD_CS_STAT_FUEL_PART				"Fuel"
	#define STRCMD_CS_STAT_SOUL_PART				"Spirit"
	#define STRCMD_CS_STAT_SHIELD_PART				"Shield"
	#define STRCMD_CS_STAT_DODGE_PART				"Agility"
	#define STRCMD_CS_STAT_BONUS					"Bonus stat"
	#define STRCMD_CS_STAT_ALL_PART					"All stat"
	#define STRCMD_CS_STAT_UNKNOWN					"Unknown stat"

	#define STRCMD_CS_AUTOSTAT_TYPE_FREESTYLE		"Free type"
	#define STRCMD_CS_AUTOSTAT_TYPE_BGEAR_ATTACK	"Attack type"
	#define STRCMD_CS_AUTOSTAT_TYPE_BGEAR_MULTI		"Multi-type"	
	#define STRCMD_CS_AUTOSTAT_TYPE_IGEAR_ATTACK	"Attack type"
	#define STRCMD_CS_AUTOSTAT_TYPE_IGEAR_DODGE		"Agility type"
	#define STRCMD_CS_AUTOSTAT_TYPE_AGEAR_ATTACK	"Attack type"
	#define STRCMD_CS_AUTOSTAT_TYPE_AGEAR_SHIELD	"Shield type"
	#define STRCMD_CS_AUTOSTAT_TYPE_MGEAR_DEFENSE	"Defense type"
	#define STRCMD_CS_AUTOSTAT_TYPE_MGEAR_SUPPORT	"Support type"
	#define STRCMD_CS_AUTOSTAT_TYPE_UNKNOWN			"UNKNOWN_AUTOSTAT_TYPE"

// 2007-10-30 by cmkwon, 세력별 해피아워 이벤트 구현 - 아래에서 다시 정의 함
//	#define STRCMD_CS_INFLUENCE_TYPE_NORMAL			"Bygeniou city general army"
//	#define STRCMD_CS_INFLUENCE_TYPE_VCN			"Bygeniou city regular army"
//	#define STRCMD_CS_INFLUENCE_TYPE_ANI			"Arlington city regular army"
//	#define STRCMD_CS_INFLUENCE_TYPE_RRP			"\\gVattalus federation army\\g"

	#define STRCMD_CS_POS_PROW						"Radar location(Top center)"
	#define STRCMD_CS_POS_PROWIN					"Computer(Center left)"
	#define STRCMD_CS_POS_PROWOUT					"Standard Weapon(Top left)"
	#define STRCMD_CS_POS_WINGIN					"Not being used(Center right)"
	#define STRCMD_CS_POS_WINGOUT					"Advanced Weapon(Top right)"
	#define STRCMD_CS_POS_CENTER					"Armor(Center middle)"
	#define STRCMD_CS_POS_REAR						"Engine(Bottom middle)"

	// 2010-06-15 by shcho&hslee 펫시스템
	//#define STRCMD_CS_POS_ATTACHMENT				"부착물(후미 우측-연료탱크|컨테이너계열)"
	#define STRCMD_CS_POS_ACCESSORY_UNLIMITED		"Accessory(Right side of rear-Fueltank|container type)"

	// 2010-06-15 by shcho&hslee 펫시스템
	//#define STRCMD_CS_POS_PET						"사용안함(후미 좌측)"
	#define STRCMD_CS_POS_ACCESSORY_TIME_LIMIT		"Not to use(Left side of rear)"

	#define STRCMD_CS_POS_PET						"Partner"

    #define STRCMD_CS_HIDDEN_ITEM					"Hidden Position"		// 2011-09-20 by hskim, 파트너 시스템 2차 - 숨겨진 아이템
	
	#define STRCMD_CS_POS_INVALID_POSITION			"Pending location"
	#define STRCMD_CS_POS_ITEMWINDOW_OFFSET			"Inventory location"

	// 2005-12-07 by cmkwon
	#define STRCMD_CS_COMMAND_QUESTCOMPLETION_0		"/QuestComplete"
	#define STRCMD_CS_COMMAND_QUESTCOMPLETION_1		"/QuestCom"
	#define STRCMD_CS_COMMAND_QUESTCOMPLETION_HELP	"format: /QuestComplete [|QuestIndex]"

	// 2006-02-08 by cmkwon
	#define STRCMD_CS_COMMAND_INFLDITRIBUTION_0		"/NationRatio"
	#define STRCMD_CS_COMMAND_INFLDITRIBUTION_1		"/InflDist"
	#define STRCMD_CS_COMMAND_INFLDITRIBUTION_HELP	"format: /NationRatio"
	#define STRCMD_CS_COMMAND_CHANGEINFL_0			"/ChangeNation"
	#define STRCMD_CS_COMMAND_CHANGEINFL_1			"/Nation"
	#define STRCMD_CS_COMMAND_CHANGEINFL_HELP		"format: /ChangeNation [|1(Normal)|2(BCU)|4(ANI)]"

	// 2006-03-02 by cmkwon
	#define STRCMD_CS_COMMAND_GOMONSTER_0			"/GoMon"
	#define STRCMD_CS_COMMAND_GOMONSTER_1			"/GoMonster"
	#define STRCMD_CS_COMMAND_GOMONSTER_HELP		"format: /GoMonster [MonsterName|MonsterNumber]"

	//////////////////////////////////////////////////////////////////////////
	// 2008-05-20 by dhjin, EP3 - 여단 수정 사항 - 주석 처리 밑으로 이동
	// 2006-03-07 by cmkwon
//	#define STRCMD_CS_GUILD_RANK_PRIVATE_NULL		"Member"
//	#define STRCMD_CS_GUILD_RANK_COMMANDER			"Flight brigade commander"
//	#define STRCMD_CS_GUILD_RANK_SQUAD_LEADER_1		"1st battalion commander"
//	#define STRCMD_CS_GUILD_RANK_PRIVATE_1			"1st battalion member"
//	#define STRCMD_CS_GUILD_RANK_SQUAD_LEADER_2		"2nd battalion commander"
//	#define STRCMD_CS_GUILD_RANK_PRIVATE_2			"2nd battalion member"
//	#define STRCMD_CS_GUILD_RANK_SQUAD_LEADER_3		"3rd battalion commander"
//	#define STRCMD_CS_GUILD_RANK_PRIVATE_3			"3rd battalion member"
//	#define STRCMD_CS_GUILD_RANK_SQUAD_LEADER_4		"4th battalion commander"
//	#define STRCMD_CS_GUILD_RANK_PRIVATE_4			"4th battalion member"
//	#define STRCMD_CS_GUILD_RANK_SQUAD_LEADER_5		"5th battalion commander"
//	#define STRCMD_CS_GUILD_RANK_PRIVATE_5			"5th battalion member"

	// 2006-04-17 by cmkwon
	#define STRCMD_CS_COMMAND_SIGNBOARD_0			"/Noticeboard"
	#define STRCMD_CS_COMMAND_SIGNBOARD_1			"/Noticeboard"
	#define STRCMD_CS_COMMAND_SIGNBOARD_HELP		"format: /Noticeboard [|Duration(unit:min) [Notice details] - Adds a notice to the public electronic notice board for a given duration."
	#define STRCMD_CS_COMMAND_SIGNBOARD_DEL_0		"/DeleteNoticeboard"
	#define STRCMD_CS_COMMAND_SIGNBOARD_DEL_1		"/DeleteNoticeboard"
	#define STRCMD_CS_COMMAND_SIGNBOARD_DEL_HELP	"format: /DeleteNoticeboard [index of notice to be deleted] - Deletes a certain notice on the electronic notice board."
	
	// 2006-04-20 by cmkwon
	#define STRCMD_CS_COMMON_RACE_INFLUENCE_LEADER	"Influence Leader"
	#define STRCMD_CS_COMMON_RACE_INFLUENCE_SUBLEADER	"Influence Subleader"
	// 2006-04-21 by cmkwon
	#define STRCMD_CS_ITEMKIND_INFLUENCE_BUFF		"Influence Buff"
	#define STRCMD_CS_ITEMKIND_INFLUENCE_GAMEEVENT	"Influence Event"

	// 2006-04-24 by cmkwon
	#define STRCMD_CS_COMMAND_CONPOINT_0			"/ContributionPoint"
	#define STRCMD_CS_COMMAND_CONPOINT_1			"/ContributionPoint"
	#define STRCMD_CS_COMMAND_CONPOINT_HELP			"format: /ContributionPoint [Influence(2:BCU, 4:ANI)] [Increase] - Increase the contribution of a certain nation"

	// 2006-05-08 by cmkwon
	#define STRCMD_CS_COMMAND_CALLGM_0				"/CallGM"
	#define STRCMD_CS_COMMAND_CALLGM_1				"/CallGM"  // Helper
	#define STRCMD_CS_COMMAND_CALLGM_2				"/CallGM"  // Help
	#define STRCMD_CS_COMMAND_CALLGM_HELP			"format: /CallGM  [Details of request] - Request of consultation with GM. - Disabled"
	#define STRCMD_CS_COMMAND_VIEWCALLGM_0			"/ViewCallGM"
	#define STRCMD_CS_COMMAND_VIEWCALLGM_1			"/ViewCallGM"   // See helper
	#define STRCMD_CS_COMMAND_VIEWCALLGM_2			"/ViewCallGM"   // See help
	#define STRCMD_CS_COMMAND_VIEWCALLGM_HELP		"format: /ViewCallGM   [|Number(1~10)] - Check number of consultation request list to GM - Disabled "
	#define STRCMD_CS_COMMAND_BRINGCALLGM_0			"/BringCallGM"
	#define STRCMD_CS_COMMAND_BRINGCALLGM_1			"/BringCallGM"   // Bring helper
	#define STRCMD_CS_COMMAND_BRINGCALLGM_2			"/BringCallGM"   // Bring help
	#define STRCMD_CS_COMMAND_BRINGCALLGM_HELP		"format: /BringCallGM [|Number(1~10)] - Bring the requested number of consultation request list to GM (Deleted from the server)"

	// 2006-07-18 by cmkwon
	#define STRCMD_CS_COMMAND_COMEONINFL_0			"/ComeOnInfl"
	#define STRCMD_CS_COMMAND_COMEONINFL_1			"/ComeOnNation"
	#define STRCMD_CS_COMMAND_COMEONINFL_2			"/ComeOnNation"
// 2008-09-09 by cmkwon, /세력소환 명령어 인자 리스트에 기어타입 추가 - commented
//	#define STRCMD_CS_COMMAND_COMEONINFL_HELP		"format: /ComeOnNation [1(Normal)|2(BCU)|4(ANI)|3|5|6|7] [MAX_USER] [0|Min-level] [0|Max-level] [message to users] - Summons all specified level users in nation with a message."

	// 2006-07-24 by cmkwon
	#define STRCMD_CS_COMMAND_ITEMINMAP_0			"/InsertItemInMap"
	#define STRCMD_CS_COMMAND_ITEMINMAP_1			"/SendItem"
	#define STRCMD_CS_COMMAND_ITEMINMAP_2			"/SendItem"
	#define STRCMD_CS_COMMAND_ITEMINMAP_HELP		"format: /SendItem [1(Normal)|2(BCU)|4(ANI)|3|5|6|7] [Item Number] [# of items] - Gives specific item to all nation users at the current map."

	// 2006-07-28 by cmkwon
	#define STRCMD_CS_ITEMKIND_COLOR_ITEM			"Color Item"

	// 2006-08-03 by cmkwon, 나라별 날짜 표현 방식이 다르다
	// 한국(Korea):		YYYY-MM-DD HH:MM:SS
	// 미국(English):	MM-DD-YYYY HH:MM:SS
	// 베트남(Vietnam):	DD-MM-YYYY HH:MM:SS
	#define NATIONAL_ATUM_DATE_TIME_STRING_FORMAT(Y, M, D, h, m, s)				"%02d-%02d-%04d %02d:%02d:%02d", M, D, Y, h, m, s
	#define NATIONAL_ATUM_DATE_TIME_STRING_FORMAT_EXCLUDE_SECOND(Y, M, D, h, m)	"%02d-%02d-%04d %02d:%02d", M, D, Y, h, m

	// 2006-08-08 by dhjin, 레벨분포
	#define STRCMD_CS_COMMAND_DISTRIBUTIONLEVEL_0		"/LevelDistribution"		// 2006-08-08 by dhjin
	#define STRCMD_CS_COMMAND_DISTRIBUTIONLEVEL_1		"/LevelDist"				// 2006-08-08 by dhjin
	#define STRCMD_CS_COMMAND_DISTRIBUTIONLEVEL_HELP	"format: /LevelDistribution - It shows concurrent users level distribution."	// 2006-08-08 by dhjin


	// 2014-03-18 by bckim, 마켓 ON/OFF
	#define STRCMD_CS_COMMAND_MARKET_ACTIVATION_0		"/market"
	#define STRCMD_CS_COMMAND_MARKET_ACTIVATION_1		"/market"	
	#define STRCMD_CS_COMMAND_MARKET_ACTIVATION_HELP	"format: /market - trade shop ON OFF. "
	// End. 2014-03-18 by bckim, 마켓 ON/OFF

	// 2006-08-10 by cmkwon
	#define STRCMD_CS_ITEMKIND_RANDOMBOX				"Chance Item"

	// 2006-08-21 by cmkwon
	#define STRCMD_CS_ITEMKIND_MARK						"Mark"

	///////////////////////////////////////////////////////////////////////////////
	// 2006-08-24 by cmkwon
	// 클라이언트에서만 사용하는 명령어(Just command for client)
	#define STRCMD_C_COMMAND_COMBAT						"/confront"
	#define STRCMD_C_COMMAND_BATTLE						"/fight"
	#define STRCMD_C_COMMAND_BATTLE_HELP				"format: /fight [CharacterName] - Request PvP to specific character."
	#define STRCMD_C_COMMAND_SURRENDER					"/surrender"
	#define STRCMD_C_COMMAND_SURRENDER_HELP				"format: /surrender [CharacterName] -  Surrenders a PvP battle with the specified character."
	#define STRCMD_C_COMMAND_PARTYBATTLE				"/formfight"
	#define STRCMD_C_COMMAND_PARTYBATTLE_HELP			"format: /formfight [CharacterName] - Request formation PvP to specific character(formation leader). Only usable by the formation leader."
	#define STRCMD_C_COMMAND_PARTYCOMBAT				"/formconfront"
	#define STRCMD_C_COMMAND_PARTYWAR					"/formbattle"
	#define STRCMD_C_COMMAND_GUILDBATTLE				"/brigfight"
	#define STRCMD_C_COMMAND_GUILDCOMBAT				"/brigconfront"
	#define STRCMD_C_COMMAND_GUILDCOMBAT_HELP			"format: /brigconfront [CharacterName] -  Request brigade PvP to specific character(brigade leader). Only usable by the brigade leader."
	#define STRCMD_C_COMMAND_GUILDWAR					"/brigbattle"
	#define STRCMD_C_COMMAND_GUILDSURRENDER				"/brigsurrender"
	#define STRCMD_C_COMMAND_GUILDSURRENDER_HELP		"format: /brigsurrender -  Surrender a brigade PvP battle. Only usable by the brigade leader."
	#define STRCMD_C_COMMAND_NAME						"/name"
	#define STRCMD_C_COMMAND_NAME_HELP					"format: /name [CharacterName] [class(2 ~ 11)] - Change appointment class of specific character. Only usable by the brigade leader. "
	#define STRCMD_C_COMMAND_WARP						"/warp"
	#define STRCMD_C_COMMAND_CANCELSKILL				"/cancelskill"
	#define STRCMD_C_COMMAND_INITCHAT					"/initchat"
	#define STRCMD_C_COMMAND_INITCHAT_HELP				"format: /initchat -  Resets chatting window"
	#define STRCMD_C_COMMAND_REFUSEBATTLE				"/refusefight"
	#define STRCMD_C_COMMAND_REFUSEBATTLE_HELP			"format: /refusefight - On/Off - Toggles PvP denial setting"
	#define STRCMD_C_COMMAND_REFUSETRADE				"/refusetrade"
	#define STRCMD_C_COMMAND_REFUSETRADE_HELP			"format: /refusetrade - On/Off - Toggles trading denial setting"
	#define STRMSG_C_050810_0001						"/CloseWindow"
	#define STRMSG_C_050810_0001_HELP					"format: /Closewindow - Prevents message popups. Popup messages are automatically cancelled."
	#define STRMSG_C_050810_0002						"/OpenWindow"
	#define STRMSG_C_050810_0002_HELP					"format: /Openwindow -  Allows message popups."

// 2006-09-29 by cmkwon
#define STRCMD_CS_ITEMKIND_SKILL_SUPPORT_ITEM			"Support Skill Item"

// 2010-06-15 by shcho&hslee 펫시스템 - 펫 아이템.
#define STRCMD_CS_ITEMKIND_PET_ITEM						"Partner Item"	 
#define STRCMD_CS_ITEMKIND_PET_SOCKET_ITEM				"Partner Socket Item"		// 2011-09-01 by hskim, 파트너 시스템 2차

// 2006-11-17 by cmkwon, 베트남 하루 게임 시간 관련
#define STRCMD_CS_COMMAND_TIMELIMITSYSTEM_0			"/TimeLimitSystem"
#define STRCMD_CS_COMMAND_TIMELIMITSYSTEM_1			"/TimeLimitSystem"
#define STRCMD_CS_COMMAND_TIMELIMITSYSTEM_P2ON		"on"
#define STRCMD_CS_COMMAND_TIMELIMITSYSTEM_P2OFF		"off"
#define STRCMD_CS_COMMAND_TIMELIMITSYSTEM_HELP		"format: /TimeLimitSystem [on|off] - This command is to turn the time limit system turn on/off."
#define STRCMD_CS_COMMAND_PLAYTIME_0				"/PlayTime"
#define STRCMD_CS_COMMAND_PLAYTIME_1				"/PlayTime"
#define STRCMD_CS_COMMAND_PLAYTIME_HELP				"format: /PlayTime - This command shows todays played time."

// 2007-10-06 by cmkwon, 부지도자 2명의 호칭을 다르게 설정 - 아래에 세력별로 다르게 정의함
//// 2006-12-13 by cmkwon
//#define STRCMD_CS_COMMON_INFLUENCE_LEADER			"Leader"
//#define STRCMD_CS_COMMON_INFLUENCE_SUBLEADER		"Subleader"

// 2007-01-08 by dhjin
#define STRCMD_CS_COMMAND_BONUSSTAT_POINT_0			"/BonusStatPoint"
#define STRCMD_CS_COMMAND_BONUSSTAT_POINT_1			"/BonusStatPoint"
#define STRCMD_CS_COMMAND_BONUSSTAT_POINT_2			"/BonusStatPoint"
#define STRCMD_CS_COMMAND_BONUSSTAT_POINT_HELP		"format: /BonusStatPoint [BonusStatPoint Counts] [|character name] - BonusStatPoint update to DB"

// 2007-01-25 by dhjin
#define STRCMD_CS_COMMAND_PCBANGUSERCOUNT_0			"/PCBang"
#define STRCMD_CS_COMMAND_PCBANGUSERCOUNT_1			"/PCBang"
#define STRCMD_CS_COMMAND_PCBANGUSERCOUNT_HELP		"format: /PCBang - PCBang user counts"

// 2007-10-06 by dhjin, 부지도자 선출 방법 변경으로 수정
// 2007-02-13 by dhjin, 부지도자
//#define STRCMD_CS_COMMAND_SUBLEADER_0				"/Subleader"
//#define STRCMD_CS_COMMAND_SUBLEADER_1				"/Subleader"
//#define STRCMD_CS_COMMAND_SUBLEADER_HELP			"format: /Subleader [CharacterName] - Setting subleader"
//#define STRCMD_CS_COMMAND_SUBLEADER_RESULT_ERROR		"Subleader setting has been cancelled."
//#define STRCMD_CS_COMMAND_SUBLEADER_RESULT_0			"No more subleaders can be set."
//#define STRCMD_CS_COMMAND_SUBLEADER_RESULT_1			"%s has become the first subleader."
//#define STRCMD_CS_COMMAND_SUBLEADER_RESULT_2			"%s has become the second subleader."
//#define STRCMD_CS_COMMAND_SUBLEADER_RESULT_10			"%s does not exist."
//#define STRCMD_CS_COMMAND_SUBLEADER_RESULT_20			"%s is already a subleader."

// 2007-02-23 by dhjin, 거점정보
#define STRCMD_CS_COMMAND_STRATEGYPOINTINFO_0			"/sp"
#define STRCMD_CS_COMMAND_STRATEGYPOINTINFO_1			"/sp"
#define STRCMD_CS_COMMAND_STRATEGYPOINTINFO_HELP		"format: /sp - This shows the status of current progress on strategypoint."
#define STRCMD_CS_COMMAND_STRATEGYPOINTINFO_EMPTY		"\\cThere is no strategypoint war in progress.\\c"
#define STRCMD_CS_COMMAND_STRATEGYPOINTINFO_EXIST		"\\pStrategypoint war is in progress.\\p"
#define STRCMD_CS_COMMAND_STRATEGYPOINTINFO_ZONE		"\\cProgress location\\c"
#define STRCMD_CS_COMMAND_STRATEGYPOINTINFO_STARTTIME		"\\mStarting time\\m"

// 2007-03-29 by cmkwon
#define STRCMD_CS_UNITKIND_BGEAR					"B-GEAR"
#define STRCMD_CS_UNITKIND_MGEAR					"M-GEAR"
#define STRCMD_CS_UNITKIND_AGEAR					"A-GEAR"
#define STRCMD_CS_UNITKIND_IGEAR					"I-GEAR"
#define STRCMD_CS_UNITKIND_BGEAR_ALL				"B-GEAR All"
#define STRCMD_CS_UNITKIND_MGEAR_ALL				"M-GEAR All"
#define STRCMD_CS_UNITKIND_AGEAR_ALL				"A-GEAR All"
#define STRCMD_CS_UNITKIND_IGEAR_ALL				"I-GEAR All"
#define STRCMD_CS_UNITKIND_GEAR_ALL					"GEAR All"

// 2007-03-30 by dhjin, 옵저버 모드 유저 등록
#define STRCMD_CS_COMMAND_OBSERVER_REG_START_0  		"/Observerstart"  // 2007-03-30 by dhjin, Client only
#define STRCMD_CS_COMMAND_OBSERVER_REG_START_1  		"/Observerstart"   // 2007-03-30 by dhjin, Client only
#define STRCMD_CS_COMMAND_OBSERVER_REG_END_0  			"/Observerend"   // 2007-03-30 by dhjin, Client only 
#define STRCMD_CS_COMMAND_OBSERVER_REG_END_1  			"/Observerend"   // 2007-03-30 by dhjin, Client only
#define STRCMD_CS_COMMAND_OBSERVER_REG_0   			"/Observer"
#define STRCMD_CS_COMMAND_OBSERVER_REG_1   			"/Observer"
#define STRCMD_CS_COMMAND_OBSERVER_REG_HELP   			"format: /Observer [n] [CharacterName] - CharacterName  save user at n number"

// 2007-04-10 by cmkwon, Jamboree server 군 관련
#define STRCMD_CS_COMMAND_JAMBOREE_INIT_0   			"/InitJamboree"   
#define STRCMD_CS_COMMAND_JAMBOREE_INIT_1   			"/InitJamboree"  
#define STRCMD_CS_COMMAND_JAMBOREE_INIT_HELP  			"format: /InitJamboree [validation number] - Initialize jamboree server DB(atum2_db_20)."
#define STRCMD_CS_COMMAND_JAMBOREE_ENTRANTS_0  			"/EntrantJamboree"
#define STRCMD_CS_COMMAND_JAMBOREE_ENTRANTS_1  			"/EntrantJamboree"  


#define STRCMD_CS_COMMAND_JAMBOREE_ENTRANTS_HELP 		"format: /EntrantJamboree [CharacterName] - Designated character will be duplicated to jamboree server DB(atum2_db_20)."
#define STRCMD_CS_JAMBOREE_PREADD_CHARACTER_NAME_1  "1_"
#define STRCMD_CS_JAMBOREE_PREADD_CHARACTER_NAME_2  "2_"
#define STRCMD_CS_JAMBOREE_PREADD_CHARACTER_NAME_3  "3_"
#define STRCMD_CS_JAMBOREE_PREADD_CHARACTER_NAME_4  "4_"
#define STRCMD_CS_JAMBOREE_PREADD_CHARACTER_NAME_5  "5_"
#define STRCMD_CS_JAMBOREE_PREADD_CHARACTER_NAME_6  "6_"
#define STRCMD_CS_JAMBOREE_PREADD_CHARACTER_NAME_7  "7_"
#define STRCMD_CS_JAMBOREE_PREADD_CHARACTER_NAME_8  "8_"
#define STRCMD_CS_JAMBOREE_PREADD_CHARACTER_NAME_9  "9_"
#define STRCMD_CS_JAMBOREE_PREADD_CHARACTER_NAME_10  "10_"
#define STRCMD_CS_JAMBOREE_PREADD_CHARACTER_NAME_11  "11_"
#define STRCMD_CS_JAMBOREE_PREADD_CHARACTER_NAME_12  "12_"
#define STRCMD_CS_JAMBOREE_PREADD_CHARACTER_NAME_13  "13_"
#define STRCMD_CS_JAMBOREE_PREADD_CHARACTER_NAME_14  "14_"
#define STRCMD_CS_JAMBOREE_PREADD_CHARACTER_NAME_15  "15_"
#define STRCMD_CS_JAMBOREE_PREADD_CHARACTER_NAME_16  "16_"
#define STRCMD_CS_JAMBOREE_PREADD_CHARACTER_NAME_17  "17_"
#define STRCMD_CS_JAMBOREE_PREADD_CHARACTER_NAME_18  "18_"
#define STRCMD_CS_JAMBOREE_PREADD_CHARACTER_NAME_19  "19_"

// 2007-04-17 by dhjin, 레벨 랭크에 관한 등급
#define STRCMD_CS_CHARACTER_12_LEVEL_RANK  "Trainee"
#define STRCMD_CS_CHARACTER_22_LEVEL_RANK  "Junior"
#define STRCMD_CS_CHARACTER_32_LEVEL_RANK  "Airman"
#define STRCMD_CS_CHARACTER_42_LEVEL_RANK  "Wingman"
#define STRCMD_CS_CHARACTER_52_LEVEL_RANK  "Ace"
#define STRCMD_CS_CHARACTER_62_LEVEL_RANK  "Veteran"
#define STRCMD_CS_CHARACTER_72_LEVEL_RANK  "Top Gun"
#define STRCMD_CS_CHARACTER_82_LEVEL_RANK  "Wing Cmdr"
#define STRCMD_CS_CHARACTER_92_LEVEL_RANK  "Hero"

// 2007-05-09 by cmkwon, 
#define STRMSG_VERSION_INFO_FILE_NAME				"VersionInfo.ver"
#define STRMSG_REG_KEY_NAME_LAUNCHER_VERSION		"LauncherVersion"
#define STRMSG_REG_KEY_NAME_CLIENT_VERSION			"ClientVersion"
#define STRMSG_REG_KEY_NAME_DELETE_VERSION			"DeleteVersion"
#define STRMSG_REG_KEY_NAME_ACCOUNT_NAME			"AccountName"
#define STRMSG_REG_KEY_NAME_SERVER_GROUP_NAME		"ServerGroupName"

// 2007-05-23 by dhjin, ARENA 팀 출력 관련 스트링
#define STRMSG_CS_STRING_ARENA_NOT_SEARCH   "Cannot find arena team."
#define STRMSG_CS_COMMAND_ARENA_TEAM_INFO_0   "/ARENA"
#define STRMSG_CS_COMMAND_ARENA_TEAM_INFO_1   "/ARENA"
#define STRMSG_CS_COMMAND_ARENA_TEAM_INFO_HELP  "format: /arena [2(BCU)|4(ANI)]- Shows the present progress of arena."

// 2010. 06. 04 by hsLee ARENA 인피니티 관련. - 
// 2010. 06. 04 by hsLee 인티피니 필드 2차 난이도 조절. (GM 명령어 추가. /nextscene(다음 시네마 씬 호출.) )
#define STRCMD_CS_COMMAND_INFINITY_NEXT_SCENE		"/nextscene"
// End 2010. 06. 04 by hsLee 인티피니 필드 2차 난이도 조절. (GM 명령어 추가. /nextscene(다음 시네마 씬 호출.) )

// 2007-06-15 by dhjin, 관전
#define STRMSG_CS_COMMAND_WATCH_START_INFO_0		"/WatchStart"
#define STRMSG_CS_COMMAND_WATCH_START_INFO_1		"/WatchStart"
#define STRMSG_CS_COMMAND_WATCH_START_INFO_HELP	"format:/WatchStart-Start the watch."
#define STRMSG_CS_COMMAND_WATCH_END_INFO_0			"/WatchEnd"
#define STRMSG_CS_COMMAND_WATCH_END_INFO_1			"/WatchEnd"
#define STRMSG_CS_COMMAND_WATCH_END_INFO_HELP		"format: /WatchEnd ? Ends the watch."

// 2007-06-22 by dhjin, WarPoint 추가
#define STRMSG_CS_COMMAND_WARPOINT_0    "/WarPoint"
#define STRMSG_CS_COMMAND_WARPOINT_1    "/WarPoint"
#define STRMSG_CS_COMMAND_WARPOINT_HELP    "format: /WarPoint [Number 1~1000000] [|Username] ? Adds war points."

// 2007-06-26 by dhjin, 워포인트 이벤트 관련 추가
#define STRCMD_CS_COMMAND_GAMEEVENT_P1WARPOINT		"WarPoint"

// 2007-07-11 by cmkwon, Arena block system materialization - Add command(/forbidAreana, /releaseArena)
#define STRCMD_CS_COMMAND_ARENAFORBID_0    "/forbidArena "
#define STRCMD_CS_COMMAND_ARENAFORBID_1    "/forbidArena"
#define STRCMD_CS_COMMAND_ARENAFORBID_2    "/forbidArena"
#define STRCMD_CS_COMMAND_ARENAFORBID_HELP   "format: /forbidArena [character name] [|Time(minutes)] - Forbid entering Arena"
#define STRCMD_CS_COMMAND_ARENAFORBIDRelease_0  "/releaseArena "
#define STRCMD_CS_COMMAND_ARENAFORBIDRelease_1  "/releaseArena"
#define STRCMD_CS_COMMAND_ARENAFORBIDRelease_2  "/releaseArena"
#define STRCMD_CS_COMMAND_ARENAFORBIDRelease_HELP "format: /releaseArena [character name] - Release prohibition of Arena"

///////////////////////////////////////////////////////////////////////////////
// 2007-08-02 by cmkwon, Brigade mark screening system materialization - added string
#define STRMSG_070802_0001    "Brigade mark has been successfully registered."
#define STRMSG_070802_0002    "Registration will be completed after screening process."
#define STRMSG_070802_0003    "Will you accept %d selected brigade mark?"
#define STRMSG_070802_0004    "No brigade mark status"
#define STRMSG_070802_0005    "Brigade mark in waiting status"
#define STRMSG_070802_0006    "Brigade mark in normal status"
#define STRMSG_070802_0007    "Brigade mark error status"

// 2007-08-24 by cmkwon, 스피커아이템 사용 가능/금지 설정 기능 추가 - 명령어 추가
#define STRCMD_CS_COMMAND_UsableSpeakerItem_0			"/UseSpeaker"
#define STRCMD_CS_COMMAND_UsableSpeakerItem_1			"/UseSpeaker"
#define STRCMD_CS_COMMAND_UsableSpeakerItem_2			"/UseSpeaker"
#define STRCMD_CS_COMMAND_UsableSpeakerItem_P1Able		"Enable"
#define STRCMD_CS_COMMAND_UsableSpeakerItem_P1Forbid	"Forbid"
#define STRCMD_CS_COMMAND_UsableSpeakerItem_HELP		"format: /[UseSpeaker|UseSpeaker|UseSpeaker] [Enable|Forbid] - Enable/Forbid use Speaker item"

// 2007-08-27 by cmkwon, PrepareShutdown command(GM can shutdown game server in SCAdminTool)
#define STRCMD_CS_COMMAND_PrepareShutdown_0				"/PrepareShutdown"
#define STRCMD_CS_COMMAND_PrepareShutdown_1				"/PrepareShutdown"
#define STRCMD_CS_COMMAND_PrepareShutdown_2				"/PrepareShutdown"
#define STRCMD_CS_COMMAND_PrepareShutdown_P1Start		"Start"
#define STRCMD_CS_COMMAND_PrepareShutdown_P1Release		"Release"
#define STRCMD_CS_COMMAND_PrepareShutdown_HELP			"format: /[PrepareShutdown|PrepareShutdown|PrepareShutdown] [Start|Release] - Prepare server shut down, disconnect all users."

// 2007-08-30 by cmkwon, 회의룸 시스템 구현 - 명령어 추가
#define STRCMD_CS_COMMAND_EntrancePermission_0                                     "/EntrancePermission"
#define STRCMD_CS_COMMAND_EntrancePermission_1                                     "/EntrancePermission"
#define STRCMD_CS_COMMAND_EntrancePermission_2                                     "/EntrancePermission"
#define STRCMD_CS_COMMAND_EntrancePermission_HELP                      "format: /[EntrancePermission] [|CharacterName] ? Can only be used by leader. This allows corresponding character to be put on conference room entrance list."
#define STRCMD_CS_COMMAND_EntrancePermissionDeny_0                               "/EntrancePermissionDeny"
#define STRCMD_CS_COMMAND_EntrancePermissionDeny_1                               "/EntrancePermissionDeny"
#define STRCMD_CS_COMMAND_EntrancePermissionDeny_2                               "/EntrancePermissionDeny"
#define STRCMD_CS_COMMAND_EntrancePermissionDeny_HELP                "format: /[ EntrancePermissionDeny] [CharacterName] ? Can only be used by leader. This deletes the corresponding user from conference room entrance list."

// 2007-10-05 by cmkwon, different each nations.
#define STRCMD_071005_0000					"%ddyas %dhour(s) %dminute(s) %dsecond(s)", Day, Hour, Minute, Second  // Day, Hour, Minute, Second is parameter name. No need to translate.
#define STRCMD_071005_0001					"%dYear %dmonth %dday", Year, Month, Day //Year, Month, Day is parameter name. No need to translate.
#define STRCMD_071005_0002					"%dYear %dmonth", Year, Month // Year, Month is parameter name. No need to translate.
#define STRCMD_071005_0003					"%dMonth %dday", Month, Day // Month, Day is parameter name. No need to translate.


// 2007-10-06 by cmkwon, Set name 2 sub-leader each nations
#define STRCMD_CS_COMMON_RACE_INFLUENCE_SUBLEADER_1		"Sub leader 1" // this is common both nations.
#define STRCMD_CS_COMMON_RACE_INFLUENCE_SUBLEADER_2		"Sub leader 2" // this is common both nations.
#define STRCMD_VCN_INFLUENCE_LEADER						"General Commander"
#define STRCMD_VCN_INFLUENCE_SUBLEADER_1				"Deputy Commander"
#define STRCMD_VCN_INFLUENCE_SUBLEADER_2				"Chief of Staff"
#define STRCMD_ANI_INFLUENCE_LEADER						"Chairman"
#define STRCMD_ANI_INFLUENCE_SUBLEADER_1				"Vice-Chairman"
#define STRCMD_ANI_INFLUENCE_SUBLEADER_2				"Strategy Director"
#define STRCMD_OUTPOST_GUILD_MASTER						"Captain %s"

// 2007-10-06 by dhjin, command to set 2 sub-leader
#define STRCMD_CS_COMMAND_SUBLEADER1_0				"/appointment1"
#define STRCMD_CS_COMMAND_SUBLEADER1_1				"/appointment1"
#define STRCMD_CS_COMMAND_SUBLEADER1_HELP			"format: /appointment1 [CharacterName] - BCU : set BCU Nation 1st sub-leader name, ANI : set ANI Nation 1st sub-leader name"
#define STRCMD_CS_COMMAND_SUBLEADER2_0				"/appointment2"
#define STRCMD_CS_COMMAND_SUBLEADER2_1				"/appointment2"
#define STRCMD_CS_COMMAND_SUBLEADER2_HELP			"format: /appointment2 [CharacterName] - BCU : set BCU Nation 2nd sub-leader name, ANI : set ANI Nation 2nd sub-leader name"

// 2007-10-30 by cmkwon, each nation happy hour event system - Command system is changed.
#define STRCMD_CS_COMMAND_HAPPYHOUREVENT				"/HappyEvent"
#define STRCMD_CS_COMMAND_HAPPYHOUREVENT_1			"/HappyEvent"
#define STRCMD_CS_COMMAND_HAPPYHOUREVENT_PSTART	"Start"
#define STRCMD_CS_COMMAND_HAPPYHOUREVENT_PEND		"End"
#define STRCMD_CS_COMMAND_HAPPYHOUREVENT_HELP		"format: /HappyEvent [255(AllNation)|0(NormalNation)|2(BCU)|4(ANI)] [Start|End] [Progress time(Unit:Minute)]"
 
// 2007-10-30 by cmkwon, each nation happy hour event system - Changed with old system.
#define STRCMD_CS_INFLUENCE_TYPE_NORMAL					"\\aNeutral\\a"
#define STRCMD_CS_INFLUENCE_TYPE_VCN					"\\eBygeniou\\e"
#define STRCMD_CS_INFLUENCE_TYPE_ANI					"\\cArlington\\c"
#define STRCMD_CS_INFLUENCE_TYPE_RRP					"\\mNGC\\m"
#define STRCMD_CS_INFLUENCE_TYPE_GM						"\\eStaff-Team\\e"
#define STRCMD_CS_INFLUENCE_TYPE_ALL_MASK				"\\rNemerian\\r"   // 2007-10-30 by cmkwon, each nation happy hour event system - added
 
// 2007-11-05 by cmkwon, WP award command - added
#define STRCMD_CS_COMMAND_WAPPOINTINMAP_0			"/AddWarPointInMap"
#define STRCMD_CS_COMMAND_WAPPOINTINMAP_1			"/WPAddedMap"
#define STRCMD_CS_COMMAND_WAPPOINTINMAP_2			"/WPAddedMap"
#define STRCMD_CS_COMMAND_WAPPOINTINMAP_HELP		"format: /WPAddedMap [1(Normal)|2(BCU)|4(ANI)|3|5|6|7] [AddWarPoint(1~)] - Users of selected nation of present map will be awarded WarPoint."

// 2007-11-19 by cmkwon, CallGM system - new command
#define STRCMD_CS_COMMAND_STARTCALLGM_0			"/StartCallGM"
#define STRCMD_CS_COMMAND_STARTCALLGM_1			"/StartHelper"
#define STRCMD_CS_COMMAND_STARTCALLGM_2			"/StartHelp"
#define STRCMD_CS_COMMAND_STARTCALLGM_HELP		"format: /StartHelper [|time(Unit: Minute)] - Start appeal system"
#define STRCMD_CS_COMMAND_ENDCALLGM_0				"/EndCallGM"
#define STRCMD_CS_COMMAND_ENDCALLGM_1				"/EndHelper"
#define STRCMD_CS_COMMAND_ENDCALLGM_2				"/EndHelp"
#define STRCMD_CS_COMMAND_ENDCALLGM_HELP			"format: /EndHelper - End appeal system"

// 2007-12-27 by cmkwon, 윈도우즈 모드 기능 추가 - STRMSG_REG_KEY_NAME_WINDOWDEGREE_NEW 추가
#define STRMSG_REG_KEY_NAME_WINDOWDEGREE_NEW		"WindowDegreeNew"

// 2008-01-03 by cmkwon, 윈도우모드 상태 저장하기 - 
#define STRMSG_REG_KEY_NAME_WINDOWMODE				"WindowMode"

// 2015-12-16 Future, Vsync setting
#define STRMSG_REG_KEY_NAME_VSYNC					"VSync"

// 2008-01-31 by cmkwon, 계정 블럭/해제 명령어로 가능한 시스템 구현 - 명령어 추가
#define STRCMD_CS_COMMAND_NEWACCOUNTBLOCK_0					"/Block"
#define STRCMD_CS_COMMAND_NEWACCOUNTBLOCK_1					"/Block"
#define STRCMD_CS_COMMAND_NEWACCOUNTBLOCK_2					"/Ban"
#define STRCMD_CS_COMMAND_NEWACCOUNTBLOCK_HELP				"format: /Block [AccountName] [BlockType(1:Normal|2:Related Money|3:Related Item|4:Related SpeedHack|5:Related Chatting|6:Related GameBug)] [Period:Days] [Block Reason for User] / [Block Reason for only Admin]"
#define STRCMD_CS_COMMAND_NEWACCOUNTUNBLOCK_0				"/Unblock"
#define STRCMD_CS_COMMAND_NEWACCOUNTUNBLOCK_1				"/Unblock"
#define STRCMD_CS_COMMAND_NEWACCOUNTUNBLOCK_2				"/Unban"
#define STRCMD_CS_COMMAND_NEWACCOUNTUNBLOCK_HELP			"format: /Unblock [AccountName]"

// 2008-02-20 by cmkwon, 명령어추가(접속중인유저모두에게아이템지급- 
#define STRCMD_CS_COMMAND_ITEMALLUSER_0                                    "/ItemAllUser"
#define STRCMD_CS_COMMAND_ITEMALLUSER_1                                    "/ItemAllUser"
#define STRCMD_CS_COMMAND_ITEMALLUSER_2                                    "/ItemAllUser"
#define STRCMD_CS_COMMAND_ITEMALLUSER_HELP                               "format: /ItemAllUser [1(Normal)|2(BCU)|4(ANI)|255(All)] [Item Number] [# of items] - Logged on user of selected nation will receive the designated item"

// 2008-02-21 by dhjin, 아레나통합- 아레나추가명령어
#define STRCMD_CS_COMMAND_ARENAMOVE_0                                                         "/ArenaMove"
#define STRCMD_CS_COMMAND_ARENAMOVE_1                                                         "/ArenaMove"
#define STRCMD_CS_COMMAND_TEAMARENALEAVE_0                                                  "/TeamArenaLeave"
#define STRCMD_CS_COMMAND_TEAMARENALEAVE_1                                                  "/TeamServerReturn"
#define STRCMD_CS_COMMAND_TEAMARENALEAVE_HELP                                   "format: /TeamArenaLeave [2(BLUE)|4(RED)|6(BLUE AND RED)]"
#define STRCMD_CS_COMMAND_TARGETARENALEAVE_0                                     "/TargetArenaLeave"
#define STRCMD_CS_COMMAND_TARGETARENALEAVE_1                                     "/TargetArenaLeave"
#define STRCMD_CS_COMMAND_TARGETARENALEAVE_HELP                                 "format: /TargetArenaLeave [Charactername]"
#define STRCMD_CS_COMMAND_ARENAEVENT_0                                                         "/ArenaEvent"
#define STRCMD_CS_COMMAND_ARENAEVENT_1                                                         "/ArenaEvent"
#define STRCMD_CS_COMMAND_ARENAEVENT_2                                                         "/ArenaEvent"
#define STRCMD_CS_COMMAND_ARENAEVENT_HELP                                                    "format: /ArenaEvent [RoomNumber]"
#define STRCMD_CS_COMMAND_ARENAEVENTRELEASE_0                                    "/ArenaEventRelease"
#define STRCMD_CS_COMMAND_ARENAEVENTRELEASE_1                                    "/CancelArenaEvent"
#define STRCMD_CS_COMMAND_ARENAEVENTRELEASE_2                                    "/CancelArenaEvent"
#define STRCMD_CS_COMMAND_ARENAEVENTRELEASE_HELP                                "format: /ArenaEventRelease [RoomNumber]"

// 2008-06-03 by cmkwon, AdminTool, DBTool 사용시 아이템 검색시 콤보박스에서 검색 기능 추가(K0000143) - 
#define STRCMD_CS_ITEMKIND_ALL_ITEM							"All Kind"

//////////////////////////////////////////////////////////////////////////
// 2008-05-20 by dhjin, EP3 - 여단 수정 사항	// 2006-03-07 by cmkwon
#define STRCMD_CS_GUILD_RANK_PRIVATE_NULL		"Member"
#define STRCMD_CS_GUILD_RANK_COMMANDER			"Flight Brigade Commander"
#define STRCMD_CS_GUILD_RANK_SUBCOMMANDER		"Deputy Brigade Commander"				// 2008-05-20 by dhjin, EP3 - 여단 수정 사항
#define STRCMD_CS_GUILD_RANK_SQUAD_LEADER_1		"1st Battalion Commander"
#define STRCMD_CS_GUILD_RANK_PRIVATE_1			"1st Battalion Member"
#define STRCMD_CS_GUILD_RANK_SQUAD_LEADER_2		"2nd Battalion Commander"
#define STRCMD_CS_GUILD_RANK_PRIVATE_2			"2nd Battalion Member"
#define STRCMD_CS_GUILD_RANK_SQUAD_LEADER_3		"3rd Battalion Commander"
#define STRCMD_CS_GUILD_RANK_PRIVATE_3			"3rd Battalion Member"
#define STRCMD_CS_GUILD_RANK_SQUAD_LEADER_4		"4th Battalion Commander"
#define STRCMD_CS_GUILD_RANK_PRIVATE_4			"4th Battalion Member"
#define STRCMD_CS_GUILD_RANK_SQUAD_LEADER_5		"5th Battalion Commander"
#define STRCMD_CS_GUILD_RANK_PRIVATE_5			"5th Battalion Member"

//////////////////////////////////////////////////////////////////////////
// 2008-06-19 by dhjin, EP3 - 전장정보
#define STRCMD_COMMAND_WAR_OPTION_0					"/MotherShipInfoOption"
#define STRCMD_COMMAND_WAR_OPTION_1					"/MotherShipInfoOption"

// 2008-08-18 by dhjin, 세력마크이벤트 
#define STRCMD_CS_COMMAND_INFLUENCEMARKEVENT_0				"/influencemarkevent"
#define STRCMD_CS_COMMAND_INFLUENCEMARKEVENT_1				"/influencemarkevent"
#define STRCMD_CS_COMMAND_INFLUENCEMARKEVENT_2				"/influencemarkevent"
#define STRCMD_CS_COMMAND_INFLUENCEMARKEVENT_HELP			"format: /influencemarkevent [nation 2(BCU)|4(ANI)]"
#define STRCMD_CS_COMMAND_INFLUENCEMARKEVENTEND_0			"/influencemarkeventend"
#define STRCMD_CS_COMMAND_INFLUENCEMARKEVENTEND_1			"/influencemarkeventend"
#define STRCMD_CS_COMMAND_INFLUENCEMARKEVENTEND_2			"/influencemarkeventend"
#define STRCMD_CS_COMMAND_INFLUENCEMARKEVENTEND_HELP		"format: /influencemarkeventend"

//////////////////////////////////////////////////////////////////////////
// 2008-08-25 by dhjin, 태국 PC방 IP정보 로딩
#define STRCMD_CS_COMMAND_PCBANGRELOADTIME_0				"/PCBangReloadTime"
#define STRCMD_CS_COMMAND_PCBANGRELOADTIME_1				"/PCBangReloadTime"
#define STRCMD_CS_COMMAND_PCBANGRELOADTIME_HELP				"format: /PCBangreloadtime [Minute] - 10 Min ~ 1440 Min"


// 2008-08-21 by dhjin, 일반, 특수 계정의 부지도자 임명 제한
#define STRMSG_080821_0001				"Cannot appoint selected character with the rank."


// 2008-09-09 by cmkwon, /세력소환 명령어 인자 리스트에 기어타입 추가 - 
#define STRCMD_CS_COMMAND_COMEONINFL_HELP2		"format: /ComeOnInfl [1(Normal)|2(BCU)|4(ANI)|255(All)] [maximum people] [0|minimum level] [0|maximum level] [1(B)|16(M)|256(A)|4096(I)|4369(ALL)] [Message to user] - Request to certain nation, level users to move to your position."

// 2008-09-09 by cmkwon, "/kick" 명령어 추가 - 
#define STRCMD_CS_COMMAND_KICK_0							"/Kick"
#define STRCMD_CS_COMMAND_KICK_1							"/Kick"
#define STRCMD_CS_COMMAND_KICK_HELP							"format: /Kick [CharacterName] - Terminate designated character from the game."


// 2008-09-12 by cmkwon, "/명성" 명령어 추가 - 
#define STRCMD_CS_COMMAND_ADD_FAME_0							"/Fame"
#define STRCMD_CS_COMMAND_ADD_FAME_1							"/Fame"
#define STRCMD_CS_COMMAND_ADD_FAME_HELP							"format: /fame [personal fame] [brigade fame] - Raises personal and brigade fame of character."

// 2008-12-30 by cmkwon, 지도자 채팅 제한 카드 구현 - 
#define STRCMD_CS_COMMAND_CHATFORBIDRELEASE_LEADER_0			"/ReleaseLeaderChatBlock"
#define STRCMD_CS_COMMAND_CHATFORBIDRELEASE_LEADER_1			"/ReleaseLeaderChatBlock"
#define STRCMD_CS_COMMAND_CHATFORBIDRELEASE_LEADER_HELP			"format: /ReleaseLeaderChatBlock [CharacterName] - Leader chat restriction is released."

// 2009-10-12 by cmkwon, 프리스카 제거 방안 적용 - 
#define STRCMD_CS_COMMAND_CHANGE_StartCityMapIndex_0                               "/StartCityMap"
#define STRCMD_CS_COMMAND_CHANGE_StartCityMapIndex_1                               "/StartCity"
#define STRCMD_CS_COMMAND_CHANGE_StartCityMapIndex_HELP                           "format: /StartCity [2001|2002] [|CharacterName] - Set the character`s beginning city to 2001 or 2002 when their nation is initialized."

#define STRCMD_CS_COMMAND_TOGGLE_CNC						"/cnc"
#define STRCMD_CS_COMMAND_TOGGLE_CNC_HELP						"format: /cnc - Toggle the CNC status (\\gEnabled\\g/\\rDisabled\\r)"


///////////////////////////////////////////////////////////////////////////////
// 2010-01-08 by cmkwon, 최대 레벨 상향에 따른 추가 사항(레벨별 계급) - 계급뜻(백부장, 대령, 장군, 총독, 정복자, 수호자, 전설적인)
#define       STRCMD_CS_CHARACTER_96_LEVEL_RANK             "Centurion"
#define       STRCMD_CS_CHARACTER_100_LEVEL_RANK            "Colonel"
#define       STRCMD_CS_CHARACTER_104_LEVEL_RANK            "General"
#define       STRCMD_CS_CHARACTER_108_LEVEL_RANK            "Governer"
#define       STRCMD_CS_CHARACTER_112_LEVEL_RANK            "Conqueror"
#define       STRCMD_CS_CHARACTER_116_LEVEL_RANK            "Guardian"
#define       STRCMD_CS_CHARACTER_120_LEVEL_RANK            "Legendary"
#define       STRCMD_CS_CHARACTER_XX_LEVEL_RANK             "Basic"

#define STRCMD_CS_COMMAND_ENDARENA				"/endarena"				// 2012-09-24 by jhseol, 아레나 종료 명령어 수정

// 2014-05-16 by bckim, 영문 명령어 추가
#define STRCMD_CS_COMMAND_RESET_REVERSAL_BUFF_TIME		"/resetreversalbufftime"
#define STRCMD_CS_COMMAND_RESET_REVERSAL_BUFF_STATE		"/resetreversalbuffstate"
#define STRCMD_CS_COMMAND_RESET_REVERSAL_BUFF_EXECUTE	"/resetreversalbuffexecute"
#define STRCMD_CS_COMMAND_RESET_REVERSAL_BUFF_END		"/resetreversalbuffend"
#define STRCMD_CS_COMMAND_COMPAT_POWER					"/compatpower"
// End. 2014-05-16 by bckim, 영문 명령어 추가

///////////////////////////////////////////////////////////////////////////////
// 2012-11-29 by jhseol, OX이벤트 맵 워프제한 - 명령어 추가
#define STRCMD_CS_COMMAND_MAP_WARP_RESTRICT_0			"/MapWarpRestrict"
#define STRCMD_CS_COMMAND_MAP_WARP_RESTRICT_1			"/맵이동제한"
#define STRCMD_CS_COMMAND_MAP_WARP_RESTRICT_HELP		"format: /맵이동제한 [MapIndex] - 해당 맵의 워프 제한을 설정함"
#define STRCMD_CS_COMMAND_MAP_WARP_RESTRICT_RESULT_0	"[%d]맵의 워프 제한이 설정 되었습니다."
#define STRCMD_CS_COMMAND_MAP_WARP_RESTRICT_RESULT_1	"[%d]맵의 워프 제한이 해제 되었습니다."
#define STRCMD_CS_COMMAND_MAP_WARP_ERROR				"[%d]는 허용하지 않는 맵번호입니다."
// end 2012-11-29 by jhseol, OX이벤트 맵 워프제한 - 명령어 추가

///////////////////////////////////////////////////////////////////////////////
// 2013-07-26 by jhseol, 타 계정 세력변경
#define STRCMD_CS_COMMAND_ACCOUNT_CHANGEINFL_0			"/AccountChangeNation"
#define STRCMD_CS_COMMAND_ACCOUNT_CHANGEINFL_1			"/AccountChangeNation"
#define STRCMD_CS_COMMAND_ACCOUNT_CHANGEINFL_HELP		"format: /AccountChangeNation [accountUID] [2(BCU)|4(ANI)]"

// 2013-11-19 by bckim, 파티원 정보 보기 명령어 추가
#define STRCMD_CS_COMMAND_PARTYMEMBER_INFO_0			"/formationinfo"
#define STRCMD_CS_COMMAND_PARTYMEMBER_INFO_1			"/formationinfo"
#define STRCMD_CS_COMMAND_PARTYMEMBER_INFO_HELP			"format: /formationinfo [CharacterName] - Information on players formation member's (member's must log inside the game)"
// End. 2013-11-19 by bckim, 파티원 정보 보기 명령어 추가

// 2015-11-25 Future, Multiple IP Restriction System
#define STRCMD_CS_COMMAND_MULTI_IP_RESTRICTION_0		"/IPUnique"
#define STRCMD_CS_COMMAND_MULTI_IP_RESTRICTION_1		"/IPBlock"
#define STRCMD_CS_COMMAND_MULTI_IP_RESTRICTION_HELP_0	"format: /IPUnique - Gets the current Multiple IP Restriction Status from the Server"
#define STRCMD_CS_COMMAND_MULTI_IP_RESTRICTION_HELP_1	"format: /IPUnique - [(TRUE|ENABLE|1)|(FALSE|DISABLE|0)]"
#define STRCMD_CS_COMMAND_MULTI_IP_RESTRICTION_HELP_2	"- Sets the current Multiple IP Restriction Status and kicks all double logged users"

// 2016-01-03 Future, leader warp restriction
#define STRCMD_CS_COMMAND_LEADER_FORBID_WARP_0			"/ForbidWarp"
#define STRCMD_CS_COMMAND_LEADER_FORBID_WARP_1			"/RestrictWarp"
#define STRCMD_CS_COMMAND_LEADER_FORBID_WARP_HELP		"format: /ForbidWarp [PlayerName] - Towns the given player and restricts his ability to warp for 30 minutes"
#define STRCMD_CS_COMMAND_LEADER_RELEASE_WARP			"/ReleaseWarp"
#define STRCMD_CS_COMMAND_LEADER_RELEASE_WARP_HELP		"format: /ReleaseWarp [PlayerName] - Releases the warping restriction from a restricted player"

#endif // end_#ifndef _STRING_DEFINE_COMMON_H_





