//Copyright[2002] MasangSoft
#include "stdafx.h"
#include "atumparam.h"
#include "atumprotocol.h"
#include "atumerror.h"

const char* GetProtocolTypeString(MessageType_t msgType)
{
    switch(msgType)
    {
    case T_PC_DEFAULT_UPDATE_LAUNCHER_VERSION:            return "T_PC_DEFAULT_UPDATE_LAUNCHER_VERSION";
    case T_PC_DEFAULT_UPDATE_LAUNCHER_UPDATE_INFO:        return "T_PC_DEFAULT_UPDATE_LAUNCHER_UPDATE_INFO";
    case T_PC_DEFAULT_UPDATE_LAUNCHER_VERSION_OK:        return "T_PC_DEFAULT_UPDATE_LAUNCHER_VERSION_OK";
    case T_PC_DEFAULT_NEW_UPDATE_LAUNCHER_VERSION:        return "T_PC_DEFAULT_NEW_UPDATE_LAUNCHER_VERSION";        // 2007-01-08 by cmkwon
    case T_PC_DEFAULT_NEW_UPDATE_LAUNCHER_UPDATE_INFO:    return "T_PC_DEFAULT_NEW_UPDATE_LAUNCHER_UPDATE_INFO";    // 2007-01-08 by cmkwon

    case T_PC_CONNECT_CLOSE:                            return "T_PC_CONNECT_CLOSE";
    case T_PC_CONNECT_ALIVE:                            return "T_PC_CONNECT_ALIVE";
    case T_PC_CONNECT_VERSION:                            return "T_PC_CONNECT_VERSION";
    case T_PC_CONNECT_UPDATE_INFO:                        return "T_PC_CONNECT_UPDATE_INFO";
    case T_PC_CONNECT_VERSION_OK:                        return "T_PC_CONNECT_VERSION_OK";
    case T_PC_CONNECT_REINSTALL_CLIENT:                    return "T_PC_CONNECT_REINSTALL_CLIENT";
    case T_PC_CONNECT_LOGIN:                            return "T_PC_CONNECT_LOGIN";
    case T_PC_CONNECT_LOGIN_OK:                            return "T_PC_CONNECT_LOGIN_OK";
    case T_PC_CONNECT_SINGLE_FILE_VERSION_CHECK:        return "T_PC_CONNECT_SINGLE_FILE_VERSION_CHECK";
    case T_PC_CONNECT_SINGLE_FILE_VERSION_CHECK_OK:        return "T_PC_CONNECT_SINGLE_FILE_VERSION_CHECK_OK";
    case T_PC_CONNECT_SINGLE_FILE_UPDATE_INFO:            return "T_PC_CONNECT_SINGLE_FILE_UPDATE_INFO";
    case T_PC_CONNECT_GET_SERVER_GROUP_LIST:            return "T_PC_CONNECT_GET_SERVER_GROUP_LIST";
    case T_PC_CONNECT_GET_SERVER_GROUP_LIST_OK:            return "T_PC_CONNECT_GET_SERVER_GROUP_LIST_OK";
    case T_PC_CONNECT_GET_GAME_SERVER_GROUP_LIST:        return "T_PC_CONNECT_GET_GAME_SERVER_GROUP_LIST";        // 2007-05-02 by cmkwon
    case T_PC_CONNECT_GET_GAME_SERVER_GROUP_LIST_OK:    return "T_PC_CONNECT_GET_GAME_SERVER_GROUP_LIST_OK";    // 2007-05-02 by cmkwon
    case T_PC_CONNECT_NETWORK_CHECK:                    return "T_PC_CONNECT_NETWORK_CHECK";    // 2007-06-18 by cmkwon, 네트워크 상태 체크
    case T_PC_CONNECT_NETWORK_CHECK_OK:                    return "T_PC_CONNECT_NETWORK_CHECK_OK";    // 2007-06-18 by cmkwon, 네트워크 상태 체크
    case T_PC_CONNECT_GET_NEW_GAME_SERVER_GROUP_LIST:        return "T_PC_CONNECT_GET_NEW_GAME_SERVER_GROUP_LIST";        // 2007-09-05 by cmkwon, EXE_1에 로그인 서버 선택 인터페이스 수정 -
    case T_PC_CONNECT_GET_NEW_GAME_SERVER_GROUP_LIST_OK:    return "T_PC_CONNECT_GET_NEW_GAME_SERVER_GROUP_LIST_OK";    // 2007-09-05 by cmkwon, EXE_1에 로그인 서버 선택 인터페이스 수정 -

    case T_PC_CONNECT_LOGIN_BLOCKED:                    return "T_PC_CONNECT_LOGIN_BLOCKED";
    case T_PC_CONNECT_MAC_BLOCKED:                        return "T_PC_CONNECT_MAC_BLOCKED";

    case T_FC_CONNECT_CLOSE:                            return "T_FC_CONNECT_CLOSE";
    case T_FC_CONNECT_ALIVE:                            return "T_FC_CONNECT_ALIVE";
    case T_FC_CONNECT_LOGIN:                            return "T_FC_CONNECT_LOGIN";
    case T_FC_CONNECT_LOGIN_OK:                            return "T_FC_CONNECT_LOGIN_OK";
    case T_FC_CONNECT_SYNC_TIME:                        return "T_FC_CONNECT_SYNC_TIME";
    case T_FC_CONNECT_NOTIFY_SERVER_SHUTDOWN:            return "T_FC_CONNECT_NOTIFY_SERVER_SHUTDOWN";

    case T_FC_CONNECT_NETWORK_CHECK:                    return "T_FC_CONNECT_NETWORK_CHECK";        // 2008-02-15 by cmkwon, Client<->FieldServer 간 네트워크 상태 체크 - 
    case T_FC_CONNECT_NETWORK_CHECK_OK:                    return "T_FC_CONNECT_NETWORK_CHECK_OK";        // 2008-02-15 by cmkwon, Client<->FieldServer 간 네트워크 상태 체크 - 



    case T_FC_CONNECT_ARENASERVER_INFO:                    return "T_FC_CONNECT_ARENASERVER_INFO";            // 2007-12-28 by dhjin, 아레나 통합 - 
    case T_FC_CONNECT_ARENASERVER_LOGIN:                return "T_FC_CONNECT_ARENASERVER_LOGIN";            // 2007-12-28 by dhjin, 아레나 통합 - 
    case T_FC_CONNECT_ARENASERVER_LOGIN_OK:                return "T_FC_CONNECT_ARENASERVER_LOGIN_OK";            // 2007-12-28 by dhjin, 아레나 통합 - 
    case T_FC_CONNECT_ARENASERVER_SSERVER_GROUP_FOR_CLIENT:                return "T_FC_CONNECT_ARENASERVER_SSERVER_GROUP_FOR_CLIENT";            // 2008-02-25 by dhjin, 아레나 통합 - 
    case T_FC_CONNECT_ARENASERVER_TO_IMSERVER:                return "T_FC_CONNECT_ARENASERVER_TO_IMSERVER";            // 2008-03-03 by dhjin, 아레나 통합 - 
    // start 2011-11-03 by shcho, yedang 셧다운제 구현 - 
    case T_FC_SHUTDOWNMINS_USER_ALTER:                    return "T_FC_SHUTDOWNMINS_USER_ALTER";        // 10분 마다 알림
    case T_FC_SHUTDOWNMINS_USER_ENDGAME:                return "T_FC_SHUTDOWNMINS_USER_ENDGAME";    // 종료 전송
    case ERR_SHUTDOWNMINORS_USER:                        return "ERR_SHUTDOWNMINORS_USER";            // 16세 이하 런처에서 막음 에러처리
    // end 2011-11-03 by shcho, yedang 셧다운제 구현 - 

    case T_FP_CONNECT_CLOSE:                            return "T_FP_CONNECT_CLOSE";
    case T_FP_CONNECT_ALIVE:                            return "T_FP_CONNECT_ALIVE";
    case T_FP_CONNECT_AUTH_USER:                        return "T_FP_CONNECT_AUTH_USER";
    case T_FP_CONNECT_AUTH_USER_OK:                        return "T_FP_CONNECT_AUTH_USER_OK";
    case T_FP_CONNECT_FIELD_CONNECT:                    return "T_FP_CONNECT_FIELD_CONNECT";
    case T_FP_CONNECT_FIELD_CONNECT_OK:                    return "T_FP_CONNECT_FIELD_CONNECT_OK";
    case T_FP_CONNECT_NOTIFY_CLOSE:                        return "T_FP_CONNECT_NOTIFY_CLOSE";
    case T_FP_CONNECT_NOTIFY_FIELDSERVER_CHANGE:        return "T_FP_CONNECT_NOTIFY_FIELDSERVER_CHANGE";
    case T_FP_CONNECT_NOTIFY_FIELDSERVER_CHANGE_OK:        return "T_FP_CONNECT_NOTIFY_FIELDSERVER_CHANGE_OK";
    case T_FP_CONNECT_PREPARE_SHUTDOWN:                    return "T_FP_CONNECT_PREPARE_SHUTDOWN";
    case T_FP_CONNECT_UPDATE_DBSERVER_GROUP:            return "T_FP_CONNECT_UPDATE_DBSERVER_GROUP";        // 2008-04-29 by cmkwon, 서버군 정보 DB에 추가(신규 계정 캐릭터 생성 제한 시스템추가) - 
    case T_FP_CONNECT_CHECK_CONNECTABLE_ACCOUNT:        return "T_FP_CONNECT_CHECK_CONNECTABLE_ACCOUNT";        // 2008-04-29 by cmkwon, 서버군 정보 DB에 추가(신규 계정 캐릭터 생성 제한 시스템추가) - 
    case T_FP_CONNECT_CHECK_CONNECTABLE_ACCOUNT_OK:        return "T_FP_CONNECT_CHECK_CONNECTABLE_ACCOUNT_OK";    // 2008-04-29 by cmkwon, 서버군 정보 DB에 추가(신규 계정 캐릭터 생성 제한 시스템추가) - 
    case T_FP_ADMIN_RELOAD_WORLDRANKING:                return "T_FP_ADMIN_RELOAD_WORLDRANKING";        // 2009-02-12 by cmkwon, EP3-3 월드랭킹시스템 구현 - 
    case T_FP_ADMIN_RELOAD_INFLUENCERATE:                return "T_FP_ADMIN_RELOAD_INFLUENCERATE";        // 2009-09-16 by cmkwon, 세력 초기화시 어뷰징 방지 구현 - 

    case T_IP_CONNECT_CLOSE:                            return "T_IP_CONNECT_CLOSE";
    case T_IP_CONNECT_ALIVE:                            return "T_IP_CONNECT_ALIVE";
    case T_IP_CONNECT_IM_CONNECT:                        return "T_IP_CONNECT_IM_CONNECT";
    case T_IP_CONNECT_IM_CONNECT_OK:                    return "T_IP_CONNECT_IM_CONNECT_OK";
    case T_IP_GET_SERVER_GROUP_INFO:                    return "T_IP_GET_SERVER_GROUP_INFO";
    case T_IP_GET_SERVER_GROUP_INFO_ACK:                return "T_IP_GET_SERVER_GROUP_INFO_ACK";
    case T_IP_ADMIN_PETITION_SET_PERIOD:                return "T_IP_ADMIN_PETITION_SET_PERIOD";        // 2007-11-19 by cmkwon, 진정시스템 업데이트 - 
    case T_IP_ADMIN_RELOAD_ADMIN_NOTICE_SYSTEM:            return "T_IP_ADMIN_RELOAD_ADMIN_NOTICE_SYSTEM";        // 2009-01-14 by cmkwon, 운영자 자동 공지 시스템 구현 - 
        
    case T_IC_CONNECT_CLOSE:                            return "T_IC_CONNECT_CLOSE";
    case T_IC_CONNECT_ALIVE:                            return "T_IC_CONNECT_ALIVE";
    case T_IC_CONNECT_LOGIN:                            return "T_IC_CONNECT_LOGIN";
    case T_IC_CONNECT_LOGIN_OK:                            return "T_IC_CONNECT_LOGIN_OK";
    case T_IC_CONNECT_FM_TO_IM_OK:                        return "T_IC_CONNECT_FM_TO_IM_OK";        // 2008-03-03 by dhjin, 아레나 통합 - 

    case T_FI_CONNECT:                                    return "T_FI_CONNECT";
    case T_FI_CONNECT_OK:                                return "T_FI_CONNECT_OK";
    case T_FI_CONNECT_CLOSE:                            return "T_FI_CONNECT_CLOSE";
    case T_FI_CONNECT_ALIVE:                            return "T_FI_CONNECT_ALIVE";
    case T_FI_CONNECT_NOTIFY_FIELDSERVER_IP:            return "T_FI_CONNECT_NOTIFY_FIELDSERVER_IP";
    case T_FI_CONNECT_NOTIFY_GAMEEND:                    return "T_FI_CONNECT_NOTIFY_GAMEEND";
    case T_FI_CONNECT_NOTIFY_DEAD:                        return "T_FI_CONNECT_NOTIFY_DEAD";
    case T_FI_GET_FIELD_USER_COUNTS:                    return "T_FI_GET_FIELD_USER_COUNTS";
    case T_FI_GET_FIELD_USER_COUNTS_ACK:                return "T_FI_GET_FIELD_USER_COUNTS_ACK";
    case T_FI_CONNECT_NOTIFY_GAMESTART:                    return "T_FI_CONNECT_NOTIFY_GAMESTART";
    case T_FI_CONNECT_NOTIFY_DEAD_GAMESTART:            return "T_FI_CONNECT_NOTIFY_DEAD_GAMESTART";
    case T_FI_CONNECT_PREPARE_SHUTDOWN:                    return "T_FI_CONNECT_PREPARE_SHUTDOWN";

    case T_PM_CONNECT:                                    return "T_PM_CONNECT";
    case T_PM_CONNECT_OK:                                return "T_PM_CONNECT_OK";
    case T_PM_CONNECT_CLOSE:                            return "T_PM_CONNECT_CLOSE";
    case T_PM_CONNECT_ALIVE:                            return "T_PM_CONNECT_ALIVE";
    case T_FM_CONNECT:                                    return "T_FM_CONNECT";
    case T_FM_CONNECT_OK:                                return "T_FM_CONNECT_OK";
    case T_FM_CONNECT_CLOSE:                            return "T_FM_CONNECT_CLOSE";
    case T_FM_CONNECT_ALIVE:                            return "T_FM_CONNECT_ALIVE";
    case T_FN_CONNECT_CLOSE:                            return "T_FN_CONNECT_CLOSE";
    case T_FN_CONNECT_ALIVE:                            return "T_FN_CONNECT_ALIVE";
    case T_FN_CONNECT_INCREASE_CHANNEL:                    return "T_FN_CONNECT_INCREASE_CHANNEL";
    case T_FN_CONNECT_SET_CHANNEL_STATE:                return "T_FN_CONNECT_SET_CHANNEL_STATE";
    case T_LM_CONNECT:                                    return "T_LM_CONNECT";
    case T_LM_CONNECT_OK:                                return "T_LM_CONNECT_OK";
    case T_LM_CONNECT_CLOSE:                            return "T_LM_CONNECT_CLOSE";
    case T_LM_CONNECT_ALIVE:                            return "T_LM_CONNECT_ALIVE";
    case T_IM_CONNECT:                                    return "T_IM_CONNECT";
    case T_IM_CONNECT_OK:                                return "T_IM_CONNECT_OK";
    case T_IM_CONNECT_CLOSE:                            return "T_IM_CONNECT_CLOSE";
    case T_IM_CONNECT_ALIVE:                            return "T_IM_CONNECT_ALIVE";
    case T_NM_CONNECT:                                    return "T_NM_CONNECT";
    case T_NM_CONNECT_OK:                                return "T_NM_CONNECT_OK";
    case T_NM_CONNECT_CLOSE:                            return "T_NM_CONNECT_CLOSE";
    case T_NM_CONNECT_ALIVE:                            return "T_NM_CONNECT_ALIVE";
    case T_PL_CONNECT:                                    return "T_PL_CONNECT";
    case T_PL_CONNECT_OK:                                return "T_PL_CONNECT_OK";
    case T_PL_CONNECT_CLOSE:                            return "T_PL_CONNECT_CLOSE";
    case T_PL_CONNECT_ALIVE:                            return "T_PL_CONNECT_ALIVE";
    case T_IL_CONNECT:                                    return "T_IL_CONNECT";
    case T_IL_CONNECT_OK:                                return "T_IL_CONNECT_OK";
    case T_IL_CONNECT_CLOSE:                            return "T_IL_CONNECT_CLOSE";
    case T_IL_CONNECT_ALIVE:                            return "T_IL_CONNECT_ALIVE";
    case T_FL_CONNECT:                                    return "T_FL_CONNECT";
    case T_FL_CONNECT_OK:                                return "T_FL_CONNECT_OK";
    case T_FL_CONNECT_CLOSE:                            return "T_FL_CONNECT_CLOSE";
    case T_FL_CONNECT_ALIVE:                            return "T_FL_CONNECT_ALIVE";
    case T_NL_CONNECT:                                    return "T_NL_CONNECT";
    case T_NL_CONNECT_OK:                                return "T_NL_CONNECT_OK";
    case T_NL_CONNECT_CLOSE:                            return "T_NL_CONNECT_CLOSE";
    case T_NL_CONNECT_ALIVE:                            return "T_NL_CONNECT_ALIVE";

    case T_PP_CONNECT:                                    return "T_PP_CONNECT";            // 2008-02-22 by cmkwon, ServerPreServer->MasangPreServer 로 서비스 정보 전송 시스템 추가 - 
    case T_PP_CONNECT_OK:                                return "T_PP_CONNECT_OK";        // 2008-02-22 by cmkwon, ServerPreServer->MasangPreServer 로 서비스 정보 전송 시스템 추가 - 
    case T_PP_CONNECT_DO_CLOSE:                            return "T_PP_CONNECT_DO_CLOSE";    // 2008-02-22 by cmkwon, ServerPreServer->MasangPreServer 로 서비스 정보 전송 시스템 추가 - 

    case T_FI_EVENT_NOTIFY_WARP:                        return "T_FI_EVENT_NOTIFY_WARP";
// 2005-07-27 by cmkwon, 다른 필드서버로의 워프는 없으므로 삭제함
//    case T_FI_EVENT_NOTIFY_WARP_OK:                        return "T_FI_EVENT_NOTIFY_WARP_OK";
    case T_FI_EVENT_CHAT_BLOCK:                            return "T_FI_EVENT_CHAT_BLOCK";        // 2008-12-30 by cmkwon, 지도자 채팅 제한 카드 구현 - 


    case T_IC_CHAT_ALL:                                    return "T_IC_CHAT_ALL";
    case T_IC_CHAT_MAP:                                    return "T_IC_CHAT_MAP";
    case T_IC_CHAT_REGION:                                return "T_IC_CHAT_REGION";
    case T_IC_CHAT_PTOP:                                return "T_IC_CHAT_PTOP";
    case T_IC_CHAT_PARTY:                                return "T_IC_CHAT_PARTY";
    case T_IC_CHAT_GUILD:                                return "T_IC_CHAT_GUILD";
    case T_IC_CHAT_GET_GUILD:                            return "T_IC_CHAT_GET_GUILD";
    case T_IC_CHAT_GET_GUILD_OK:                        return "T_IC_CHAT_GET_GUILD_OK";
    case T_IC_CHAT_CHANGE_GUILD:                        return "T_IC_CHAT_CHANGE_GUILD";
    case T_IC_CHAT_CHANGE_GUILD_OK:                        return "T_IC_CHAT_CHANGE_GUILD_OK";
    case T_IC_CHAT_CHANGE_PARTY:                        return "T_IC_CHAT_CHANGE_PARTY";
    case T_IC_CHAT_CHANGE_PARTY_OK:                        return "T_IC_CHAT_CHANGE_PARTY_OK";
    case T_IC_CHAT_CHANGE_CHAT_FLAG:                    return "T_IC_CHAT_CHANGE_CHAT_FLAG";
    case T_IC_CHAT_POSITION:                            return "T_IC_CHAT_POSITION";
    case T_IC_CHAT_BLOCK:                                return "T_IC_CHAT_BLOCK";
    case T_IC_CHAT_GET_BLOCK:                            return "T_IC_CHAT_GET_BLOCK";
    case T_IC_CHAT_GET_BLOCK_OK:                        return "T_IC_CHAT_GET_BLOCK_OK";
    case T_IC_CHAT_BLOCK_YOU:                            return "T_IC_CHAT_BLOCK_YOU";
    case T_IC_CHAT_FRIENDLIST_AND_REJECTLIST_LOADING:    return "T_IC_CHAT_FRIENDLIST_AND_REJECTLIST_LOADING";
    case T_IC_CHAT_FRIENDLIST_LOADING_OK:                return "T_IC_CHAT_FRIENDLIST_LOADING_OK";
    case T_IC_CHAT_FRIENDLIST_INSERT:                    return "T_IC_CHAT_FRIENDLIST_INSERT";
    case T_IC_CHAT_FRIENDLIST_INSERT_OK:                return "T_IC_CHAT_FRIENDLIST_INSERT_OK";
    case T_IC_CHAT_FRIENDLIST_INSERT_NOTIFY:            return "T_IC_CHAT_FRIENDLIST_INSERT_NOTIFY";    // 2009-01-13 by cmkwon, 친구 등록시 상대방에게 알림 시스템 적용 - 

    case T_IC_CHAT_FRIENDLIST_DELETE:                    return "T_IC_CHAT_FRIENDLIST_DELETE";
    case T_IC_CHAT_FRIENDLIST_DELETE_OK:                return "T_IC_CHAT_FRIENDLIST_DELETE_OK";
    case T_IC_CHAT_FRIENDLIST_REFRESH:                    return "T_IC_CHAT_FRIENDLIST_REFRESH";
    case T_IC_CHAT_FRIENDLIST_REFRESH_OK:                return "T_IC_CHAT_FRIENDLIST_REFRESH_OK";
    case T_IC_CHAT_REJECTLIST_LOADING_OK:                return "T_IC_CHAT_REJECTLIST_LOADING_OK";
    case T_IC_CHAT_REJECTLIST_INSERT:                    return "T_IC_CHAT_REJECTLIST_INSERT";
    case T_IC_CHAT_REJECTLIST_INSERT_OK:                return "T_IC_CHAT_REJECTLIST_INSERT_OK";
    case T_IC_CHAT_REJECTLIST_DELETE:                    return "T_IC_CHAT_REJECTLIST_DELETE";
    case T_IC_CHAT_REJECTLIST_DELETE_OK:                return "T_IC_CHAT_REJECTLIST_DELETE_OK";
    case T_IC_CHAT_SELL_ALL:                            return "T_IC_CHAT_SELL_ALL";
    case T_IC_CHAT_CASH_ALL:                            return "T_IC_CHAT_CASH_ALL";
    case T_IC_CHAT_INFLUENCE_ALL:                        return "T_IC_CHAT_INFLUENCE_ALL";
    case T_IC_CHAT_ARENA:                                return "T_IC_CHAT_ARENA";
    case T_IC_CHAT_WAR:                                    return "T_IC_CHAT_WAR";            // 2008-05-19 by dhjin, EP3 - 채팅 시스템 변경, 전쟁 채팅
    case T_IC_CHAT_CHATROOM:                            return "T_IC_CHAT_CHATROOM";    // 2008-06-18 by dhjin, EP3 채팅방 - 
    case T_IC_CHAT_INFINITY:                            return "T_IC_CHAT_INFINITY";    // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 채팅
        
    case T_FI_CHAT_MAP:                                    return "T_FI_CHAT_MAP";
    case T_FI_CHAT_REGION:                                return "T_FI_CHAT_REGION";
    case T_FI_CHAT_CHANGE_CHAT_FLAG:                    return "T_FI_CHAT_CHANGE_CHAT_FLAG";
    case T_FI_CHAT_CASH_ALL:                            return "T_FI_CHAT_CASH_ALL";
    case T_FI_CHAT_ARENA:                                return "T_FI_CHAT_ARENA";
    case T_FI_CHAT_OUTPOST_GUILD:                        return "T_FI_CHAT_OUTPOST_GUILD";    // 2007-10-06 by cmkwon
    case T_FI_CHAT_INFINITY:                            return "T_FI_CHAT_INFINITY";    // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 채팅
    case T_FI_CHAT_CNC:                                    return "T_FI_CHAT_CNC";    // 2015-07-20 Future, CnC costs money

    case T_FI_CHAT_ONLINE_EACHOTHER_FRIEND_COUNT_OK:    return "T_FI_CHAT_ONLINE_EACHOTHER_FRIEND_COUNT_OK";
    case T_IC_CHAT_ONLINE_EACHOTHER_FRIEND_COUNT:        return "T_IC_CHAT_ONLINE_EACHOTHER_FRIEND_COUNT";        // 2008-07-11 by dhjin, EP3 친구목록 -

    case T_FC_CHAT_MAP:                                    return "T_FC_CHAT_MAP";
    case T_FC_CHAT_REGION:                                return "T_FC_CHAT_REGION";
    case T_FC_CHAT_CASH_ALL:                            return "T_FC_CHAT_CASH_ALL";
    case T_FC_CHAT_ARENA:                                return "T_FC_CHAT_ARENA";
    case T_FC_CHAT_ALL_INFLUENCE:                        return "T_FC_CHAT_ALL_INFLUENCE";    // 2007-08-09 by cmkwon, 모든 세력에 채팅 전송하기 -
    case T_FC_CHAT_OUTPOST_GUILD:                        return "T_FC_CHAT_OUTPOST_GUILD";    // 2007-10-06 by cmkwon, 전진 기지 소유한 여단장 세력 채팅 가능
    case T_FC_CHAT_INFINITY:                            return "T_FC_CHAT_INFINITY";    // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 채팅
    case T_FC_CHAT_CNC:                                    return "T_FC_CHAT_CNC";

    case T_FC_CHARACTER_CREATE:                            return "T_FC_CHARACTER_CREATE";
    case T_FC_CHARACTER_CREATE_OK:                        return "T_FC_CHARACTER_CREATE_OK";
    case T_FC_CHARACTER_DELETE:                            return "T_FC_CHARACTER_DELETE";
    case T_FC_CHARACTER_DELETE_OK:                        return "T_FC_CHARACTER_DELETE_OK";
    case T_FC_CHARACTER_GET_CHARACTER:                    return "T_FC_CHARACTER_GET_CHARACTER";
    case T_FC_CHARACTER_GET_CHARACTER_OK:                return "T_FC_CHARACTER_GET_CHARACTER_OK";
    case T_FC_CHARACTER_GAMESTART:                        return "T_FC_CHARACTER_GAMESTART";
    case T_FC_CHARACTER_GAMESTART_OK:                    return "T_FC_CHARACTER_GAMESTART_OK";
    case T_FC_CHARACTER_CONNECT_GAMESTART:                return "T_FC_CHARACTER_CONNECT_GAMESTART";
    case T_FC_CHARACTER_CONNECT_GAMESTART_OK:            return "T_FC_CHARACTER_CONNECT_GAMESTART_OK";
    case T_FC_CHARACTER_GAMEEND:                        return "T_FC_CHARACTER_GAMEEND";
    case T_FC_CHARACTER_GAMEEND_OK:                        return "T_FC_CHARACTER_GAMEEND_OK";
    case T_FC_CHARACTER_REPAIR:                            return "T_FC_CHARACTER_REPAIR";
    case T_FC_CHARACTER_REPAIR_OK:                        return "T_FC_CHARACTER_REPAIR_OK";
    case T_FC_CHARACTER_REPAIR_ERR:                        return "T_FC_CHARACTER_REPAIR_ERR";
    case T_FC_CHARACTER_DOCKING:                        return "T_FC_CHARACTER_DOCKING";
    case T_FC_CHARACTER_UNDOCKING:                        return "T_FC_CHARACTER_UNDOCKING";
    case T_FC_CHARACTER_DOCKING_ERR:                    return "T_FC_CHARACTER_DOCKING_ERR";
    case T_FC_CHARACTER_GET_OTHER_INFO:                    return "T_FC_CHARACTER_GET_OTHER_INFO";
    case T_FC_CHARACTER_GET_OTHER_INFO_OK:                return "T_FC_CHARACTER_GET_OTHER_INFO_OK";
    case T_FC_CHARACTER_GET_MONSTER_INFO_OK:            return "T_FC_CHARACTER_GET_MONSTER_INFO_OK";
    case T_FC_CHARACTER_CHANGE_UNITKIND:                return "T_FC_CHARACTER_CHANGE_UNITKIND";
    case T_FC_CHARACTER_CHANGE_STAT:                    return "T_FC_CHARACTER_CHANGE_STAT";
    case T_FC_CHARACTER_CHANGE_TOTALGEAR_STAT:            return "T_FC_CHARACTER_CHANGE_TOTALGEAR_STAT";
    case T_FC_CHARACTER_CHANGE_FRIEND:                    return "T_FC_CHARACTER_CHANGE_FRIEND";
    case T_FC_CHARACTER_CHANGE_EXP:                        return "T_FC_CHARACTER_CHANGE_EXP";
    case T_FC_CHARACTER_CHANGE_BODYCONDITION:            return "T_FC_CHARACTER_CHANGE_BODYCONDITION";
    case T_FC_CHARACTER_CHANGE_BODYCONDITION_SKILL:        return "T_FC_CHARACTER_CHANGE_BODYCONDITION_SKILL";
    case T_FC_CHARACTER_CHANGE_INFLUENCE_TYPE:            return "T_FC_CHARACTER_CHANGE_INFLUENCE_TYPE";
    case T_FC_CHARACTER_CHANGE_STATUS:                    return "T_FC_CHARACTER_CHANGE_STATUS";
    case T_FC_CHARACTER_CHANGE_PKPOINT:                    return "T_FC_CHARACTER_CHANGE_PKPOINT";
    case T_FC_CHARACTER_CHANGE_HPDPSPEP:                return "T_FC_CHARACTER_CHANGE_HPDPSPEP";
    case T_FC_CHARACTER_CHANGE_CURRENTHPDPSPEP:            return "T_FC_CHARACTER_CHANGE_CURRENTHPDPSPEP";
    case T_FC_CHARACTER_CHANGE_CURRENTHP:                return "T_FC_CHARACTER_CHANGE_CURRENTHP";
    case T_FC_CHARACTER_CHANGE_CURRENTDP:                return "T_FC_CHARACTER_CHANGE_CURRENTDP";
    case T_FC_CHARACTER_CHANGE_CURRENTSP:                return "T_FC_CHARACTER_CHANGE_CURRENTSP";
    case T_FC_CHARACTER_CHANGE_CURRENTEP:                return "T_FC_CHARACTER_CHANGE_CURRENTEP";
    case T_FC_CHARACTER_CHANGE_MAPNAME:                    return "T_FC_CHARACTER_CHANGE_MAPNAME";
    case T_FC_CHARACTER_CHANGE_PETINFO:                    return "T_FC_CHARACTER_CHANGE_PETINFO";
    case T_FC_CHARACTER_CHANGE_POSITION:                return "T_FC_CHARACTER_CHANGE_POSITION";
    case T_FC_CHARACTER_CHANGE_LEVEL:                    return "T_FC_CHARACTER_CHANGE_LEVEL";
    case T_FC_CHARACTER_USE_BONUSSTAT:                    return "T_FC_CHARACTER_USE_BONUSSTAT";
    case T_FC_CHARACTER_USE_BONUSSTAT_OK:                return "T_FC_CHARACTER_USE_BONUSSTAT_OK";        // 2006-09-18 by cmkwon
    case T_FC_CHARACTER_DEAD_NOTIFY:                    return "T_FC_CHARACTER_DEAD_NOTIFY";
    case T_FC_CHARACTER_GET_ACCOUNTUNIQUENUMBER:        return "T_FC_CHARACTER_GET_ACCOUNTUNIQUENUMBER";
    case T_FC_CHARACTER_GET_ACCOUNTUNIQUENUMBER_OK:        return "T_FC_CHARACTER_GET_ACCOUNTUNIQUENUMBER_OK";
    case T_FC_CHARACTER_APPLY_COLLISION_DAMAGE:            return "T_FC_CHARACTER_APPLY_COLLISION_DAMAGE";
    case T_FC_CHARACTER_GET_OTHER_MOVE:                    return "T_FC_CHARACTER_GET_OTHER_MOVE";
    case T_FC_CHARACTER_DELETE_OTHER_INFO:                return "T_FC_CHARACTER_DELETE_OTHER_INFO";
    case T_FC_CHARACTER_DEAD_GAMESTART:                    return "T_FC_CHARACTER_DEAD_GAMESTART";
    case T_FC_CHARACTER_OTHER_REVIVED:                    return "T_FC_CHARACTER_OTHER_REVIVED";
    case T_FC_CHARACTER_GET_OTHER_RENDER_INFO:            return "T_FC_CHARACTER_GET_OTHER_RENDER_INFO";
    case T_FC_CHARACTER_GET_OTHER_RENDER_INFO_OK:        return "T_FC_CHARACTER_GET_OTHER_RENDER_INFO_OK";
    case T_FC_CHARACTER_CHANGE_BODYCONDITION_ALL:        return "T_FC_CHARACTER_CHANGE_BODYCONDITION_ALL";
    case T_FC_CHARACTER_CHANGE_PROPENSITY:            return "T_FC_CHARACTER_CHANGE_PROPENSITY";

    case T_FC_CHARACTER_SHOW_EFFECT:                    return "T_FC_CHARACTER_SHOW_EFFECT";
    case T_FC_CHARACTER_SHOW_EFFECT_OK:                    return "T_FC_CHARACTER_SHOW_EFFECT_OK";
    case T_FC_CHARACTER_GET_OTHER_PARAMFACTOR:            return "T_FC_CHARACTER_GET_OTHER_PARAMFACTOR";
    case T_FC_CHARACTER_GET_OTHER_PARAMFACTOR_OK:        return "T_FC_CHARACTER_GET_OTHER_PARAMFACTOR_OK";
    case T_FC_CHARACTER_SEND_PARAMFACTOR_IN_RANGE:        return "T_FC_CHARACTER_SEND_PARAMFACTOR_IN_RANGE";
    case T_FC_CHARACTER_SPEED_HACK_USER:                return "T_FC_CHARACTER_SPEED_HACK_USER";
    case T_FC_CHARACTER_CHANGE_CHARACTER_MENT:            return "T_FC_CHARACTER_CHANGE_CHARACTER_MENT";    
    case T_FC_CHARACTER_GET_CASH_MONEY_COUNT:            return "T_FC_CHARACTER_GET_CASH_MONEY_COUNT";    
    case T_FC_CHARACTER_GET_CASH_MONEY_COUNT_OK:        return "T_FC_CHARACTER_GET_CASH_MONEY_COUNT_OK";    
    case T_FC_CHARACTER_CASH_PREMIUM_CARD_INFO:            return "T_FC_CHARACTER_CASH_PREMIUM_CARD_INFO";    
    case T_FC_CHARACTER_TUTORIAL_SKIP:                    return "T_FC_CHARACTER_TUTORIAL_SKIP";    
    case T_FC_CHARACTER_TUTORIAL_SKIP_OK:                return "T_FC_CHARACTER_TUTORIAL_SKIP_OK";
    // 2005-07-26 by hblee : 착륙장에서 캐릭터 모드 변환.
    case T_FC_CHARACTER_CHANGE_CHARACTER_MODE:            return "T_FC_CHARACTER_CHANGE_CHARACTER_MODE";
    case T_FC_CHARACTER_CHANGE_CHARACTER_MODE_OK:        return "T_FC_CHARACTER_CHANGE_CHARACTER_MODE_OK";
    case T_FC_CHARACTER_FALLING_BY_FUEL_ALLIN:            return "T_FC_CHARACTER_FALLING_BY_FUEL_ALLIN";
    case T_FC_CHARACTER_WARP_BY_AGEAR_LANDING_FUEL_ALLIN:        return "T_FC_CHARACTER_WARP_BY_AGEAR_LANDING_FUEL_ALLIN";
    case T_FC_CHARACTER_GET_REAL_WEAPON_INFO:            return "T_FC_CHARACTER_GET_REAL_WEAPON_INFO";
    case T_FC_CHARACTER_GET_REAL_WEAPON_INFO_OK:        return "T_FC_CHARACTER_GET_REAL_WEAPON_INFO_OK";
    case T_FC_CHARACTER_GET_REAL_ENGINE_INFO:            return "T_FC_CHARACTER_GET_REAL_ENGINE_INFO";
    case T_FC_CHARACTER_GET_REAL_ENGINE_INFO_OK:        return "T_FC_CHARACTER_GET_REAL_ENGINE_INFO_OK";
    case T_FC_CHARACTER_GET_REAL_TOTAL_WEIGHT:            return "T_FC_CHARACTER_GET_REAL_TOTAL_WEIGHT";
    case T_FC_CHARACTER_GET_REAL_TOTAL_WEIGHT_OK:        return "T_FC_CHARACTER_GET_REAL_TOTAL_WEIGHT_OK";
    case T_FC_CHARACTER_MEMORY_HACK_USER:                return "T_FC_CHARACTER_MEMORY_HACK_USER";
    case T_FC_CHARACTER_UPDATE_SUBLEADER:                return "T_FC_CHARACTER_UPDATE_SUBLEADER";
    case T_FC_CHARACTER_UPDATE_SUBLEADER_OK:            return "T_FC_CHARACTER_UPDATE_SUBLEADER_OK";

    case T_FC_CHARACTER_OBSERVER_START:                    return "T_FC_CHARACTER_OBSERVER_START";
    case T_FC_CHARACTER_OBSERVER_START_OK:                return "T_FC_CHARACTER_OBSERVER_START_OK";
    case T_FC_CHARACTER_OBSERVER_END:                    return "T_FC_CHARACTER_OBSERVER_END";
    case T_FC_CHARACTER_OBSERVER_END_OK:                return "T_FC_CHARACTER_OBSERVER_END_OK";
    case T_FC_CHARACTER_OBSERVER_INFO:                    return "T_FC_CHARACTER_OBSERVER_INFO";
    case T_FC_CHARACTER_OBSERVER_REG:                    return "T_FC_CHARACTER_OBSERVER_REG";
    case T_FC_CHARACTER_SHOW_MAP_EFFECT:                return "T_FC_CHARACTER_SHOW_MAP_EFFECT";        // 2007-04-20 by cmkwon
    case T_FC_CHARACTER_SHOW_MAP_EFFECT_OK:                return "T_FC_CHARACTER_SHOW_MAP_EFFECT_OK";        // 2007-04-20 by cmkwon
    case T_FC_CHARACTER_PAY_WARPOINT:                    return "T_FC_CHARACTER_PAY_WARPOINT";            // 2007-05-16 by dhjin
    case T_FC_CHARACTER_WATCH_INFO:                        return "T_FC_CHARACTER_WATCH_INFO";                // 2007-06-19 by dhjin
    // 2008-01-10 by dhjin,  아레나 통합 - 
    case T_FC_CHARACTER_GAMESTART_FROM_ARENA_TO_MAINSERVER:            return "T_FC_CHARACTER_GAMESTART_FROM_ARENA_TO_MAINSERVER";
    case T_FC_CHARACTER_READY_GAMESTART_FROM_ARENA_TO_MAINSERVER:            return "T_FC_CHARACTER_READY_GAMESTART_FROM_ARENA_TO_MAINSERVER";
    case T_FC_CHARACTER_READY_GAMESTART_FROM_ARENA_TO_MAINSERVER_OK:        return "T_FC_CHARACTER_READY_GAMESTART_FROM_ARENA_TO_MAINSERVER_OK";
    case T_FC_CHARACTER_GET_USER_INFO:                    return "T_FC_CHARACTER_GET_USER_INFO";                    // C->F, // 2008-06-23 by dhjin, EP3 유저정보옵션 -
    case T_FC_CHARACTER_GET_USER_INFO_OK:                return "T_FC_CHARACTER_GET_USER_INFO_OK";                // F->C, // 2008-06-23 by dhjin, EP3 유저정보옵션 -
    case T_FC_CHARACTER_CHANGE_INFO_OPTION_SECRET:        return "T_FC_CHARACTER_CHANGE_INFO_OPTION_SECRET";        // C->F, // 2008-06-23 by dhjin, EP3 유저정보옵션 -
    case T_FC_CHARACTER_CHANGE_INFO_OPTION_SECRET_OK:    return "T_FC_CHARACTER_CHANGE_INFO_OPTION_SECRET_OK";    // F->C, // 2008-06-23 by dhjin, EP3 유저정보옵션 -
    case T_FC_CHARACTER_CHANGE_NICKNAME:                return "T_FC_CHARACTER_CHANGE_NICKNAME";        // C->F, // 2009-02-12 by cmkwon, EP3-3 월드랭킹시스템 구현 - 
    case T_FC_CHARACTER_CHANGE_NICKNAME_OK:                return "T_FC_CHARACTER_CHANGE_NICKNAME_OK";        // F->C, // 2009-02-12 by cmkwon, EP3-3 월드랭킹시스템 구현 - 
    case T_FC_CHARACTER_CHANGE_START_CITY_MAPINDEX:                return "T_FC_CHARACTER_CHANGE_START_CITY_MAPINDEX";        // 2009-10-12 by cmkwon, 프리스카 제거 방안 적용 - 
    case T_FC_CHARACTER_CHANGE_START_CITY_MAPINDEX_OK:            return "T_FC_CHARACTER_CHANGE_START_CITY_MAPINDEX_OK";    // 2009-10-12 by cmkwon, 프리스카 제거 방안 적용 - 
    case T_FC_CHARACTER_CHANGE_ADDED_INVENTORY_COUNT:            return "T_FC_CHARACTER_CHANGE_ADDED_INVENTORY_COUNT";    // 2009-11-02 by cmkwon, 캐쉬(인벤/창고 확장) 아이템 추가 구현 - 
    //////////////////////////////////////////////////////////////////
    // 2012-06-05 by jhseol, 아템미리보기 - 클라요청 패킷 추가
    case T_FC_CHARACTER_GET_USER_ITEM_INFO:                        return "T_FC_CHARACTER_GET_USER_ITEM_INFO";        // 2012-06-05 by jhseol, 아템미리보기 - 살펴볼 케릭터 UID
    case T_FC_CHARACTER_GET_USER_ITEM_INFO_OK:                    return "T_FC_CHARACTER_GET_USER_ITEM_INFO_OK";    // 2012-06-05 by jhseol, 아템미리보기 - 살펴본 케릭터의 아템정보
    case T_FC_CHARACTER_GET_USER_ITEM_INFO_OK_DONE:                return "T_FC_CHARACTER_GET_USER_ITEM_INFO_OK_DONE";    // 2012-06-05 by jhseol, 아템미리보기 - 살펴본 케릭터의 아템정보
    case T_FC_CHARACTER_GET_USER_ITEM_INFO_SECRET:                return "T_FC_CHARACTER_GET_USER_ITEM_INFO_SECRET";        // 2012-07-12 by isshin 아템미리보기 On / Off 기능 - 공개 여부
    case T_FC_CHARACTER_GET_USER_ITEM_INFO_SECRET_OK:            return "T_FC_CHARACTER_GET_USER_ITEM_INFO_SECRET_OK";    // 2012-07-12 by isshin 아템미리보기 On / Off 기능 - 공개 여부

    case T_FC_CHARACTER_DEBUFF_DOT_INFO:                return "T_FC_CHARACTER_DEBUFF_DOT_INFO";        // 2009-09-09 ~ 2010 by dhjin, 인피니티 - C -> F
    case T_FC_CHARACTER_DEBUFF_DOT_INFO_OK:                return "T_FC_CHARACTER_DEBUFF_DOT_INFO_OK";        // 2009-09-09 ~ 2010 by dhjin, 인피니티 - F -> C
    case T_FC_CHARACTER_DEBUFF_DOT_APPLYING:            return "T_FC_CHARACTER_DEBUFF_DOT_APPLYING";        // 2009-09-09 ~ 2010 by dhjin, 인피니티 - F -> C
    case T_FC_CHARACTER_DEBUFF_DOT_RELEASE:                return "T_FC_CHARACTER_DEBUFF_DOT_RELEASE";        // 2009-09-09 ~ 2010 by dhjin, 인피니티 - F -> C

    // 2012-10-12 by jhjang 해피 아워 경험치 보너스 시스템 리뉴얼
    case T_FC_CHARACTER_KILLCOUNT:                        return "T_FC_CHARACTER_KILLCOUNT";
    case T_FC_CHARACTER_RESTCOUNT:                        return "T_FC_CHARACTER_RESTCOUNT";
    case T_FC_CHARACTER_BONUS_EXP_RATE:                    return "T_FC_CHARACTER_BONUS_EXP_RATE";
    // end 2012-10-12 by jhjang 해피 아워 경험치 보너스 시스템 리뉴얼
    
    case T_FN_CHARACTER_CHANGE_UNITKIND:                return "T_FN_CHARACTER_CHANGE_UNITKIND";
    case T_FN_CHARACTER_CHANGE_BODYCONDITION:            return "T_FN_CHARACTER_CHANGE_BODYCONDITION";
    case T_FN_CHARACTER_CHANGE_HPDPSPEP:                return "T_FN_CHARACTER_CHANGE_HPDPSPEP";
    case T_FN_CHARACTER_CHANGE_CURRENTHPDPSPEP:            return "T_FN_CHARACTER_CHANGE_CURRENTHPDPSPEP";
    case T_FN_CHARACTER_CHANGE_MAPNAME:                    return "T_FN_CHARACTER_CHANGE_MAPNAME";
    case T_FN_CHARACTER_CHANGE_POSITION:                return "T_FN_CHARACTER_CHANGE_POSITION";
    case T_FN_CHARACTER_CHANGE_STEALTHSTATE:            return "T_FN_CHARACTER_CHANGE_STEALTHSTATE";
    case T_FN_CHARACTER_CHANGE_CHARACTER_MODE_OK:                    return "T_FN_CHARACTER_CHANGE_CHARACTER_MODE_OK";
    case T_FN_CHARACTER_CHANGE_INFLUENCE_TYPE:                return "T_FN_CHARACTER_CHANGE_INFLUENCE_TYPE";
    case T_FN_CHARACTER_CHANGE_INVISIBLE:                return "T_FN_CHARACTER_CHANGE_INVISIBLE";

    case T_FC_MOVE:                                        return "T_FC_MOVE";
    case T_FC_MOVE_OK:                                    return "T_FC_MOVE_OK";
    case T_FC_MOVE_LOCKON:                                return "T_FC_MOVE_LOCKON";
    case T_FC_MOVE_LOCKON_OK:                            return "T_FC_MOVE_LOCKON_OK";
    case T_FC_MOVE_UNLOCKON:                            return "T_FC_MOVE_UNLOCKON";
    case T_FC_MOVE_UNLOCKON_OK:                            return "T_FC_MOVE_UNLOCKON_OK";
    case T_FC_MOVE_LANDING:                                return "T_FC_MOVE_LANDING";
    case T_FC_MOVE_LANDING_OK:                            return "T_FC_MOVE_LANDING_OK";
    case T_FC_MOVE_LANDING_DONE:                        return "T_FC_MOVE_LANDING_DONE";
    case T_FC_MOVE_TAKEOFF:                                return "T_FC_MOVE_TAKEOFF";
    case T_FC_MOVE_TAKEOFF_OK:                            return "T_FC_MOVE_TAKEOFF_OK";
    case T_FC_MISSILE_MOVE_OK:                            return "T_FC_MISSILE_MOVE_OK";
    case T_FC_MOVE_TARGET:                                return "T_FC_MOVE_TARGET";
    case T_FC_MOVE_WEAPON_VEL:                            return "T_FC_MOVE_WEAPON_VEL";
    case T_FC_MOVE_WEAPON_VEL_OK:                        return "T_FC_MOVE_WEAPON_VEL_OK";
    case T_FC_MOVE_ROLLING:                                return "T_FC_MOVE_ROLLING";
    case T_FC_MOVE_ROLLING_OK:                            return "T_FC_MOVE_ROLLING_OK";
    case T_FC_MOVE_HACKSHIELD_GuidReqMsg:                return "T_FC_MOVE_HACKSHIELD_GuidReqMsg";
    case T_FC_MOVE_HACKSHIELD_GuidAckMsg:                return "T_FC_MOVE_HACKSHIELD_GuidAckMsg";
    case T_FC_MOVE_HACKSHIELD_CRCReqMsg:                return "T_FC_MOVE_HACKSHIELD_CRCReqMsg";
    case T_FC_MOVE_HACKSHIELD_CRCAckMsg:                return "T_FC_MOVE_HACKSHIELD_CRCAckMsg";
    case T_FC_MOVE_HACKSHIELD_HACKING_CLIENT:            return "T_FC_MOVE_HACKSHIELD_HACKING_CLIENT";

    case T_FN_MONSTER_MOVE:                                return "T_FN_MONSTER_MOVE";
    case T_FN_MOVE_OK:                                    return "T_FN_MOVE_OK";
    case T_FN_MOVE_LOCKON:                                return "T_FN_MOVE_LOCKON";
    case T_FN_MOVE_LOCKON_OK:                            return "T_FN_MOVE_LOCKON_OK";
    case T_FN_MOVE_LOCKON_AVOID:                        return "T_FN_MOVE_LOCKON_AVOID";
    case T_FN_MOVE_LOCKON_AVOID_OK:                        return "T_FN_MOVE_LOCKON_AVOID_OK";
    case T_FN_MISSILE_MOVE:                                return "T_FN_MISSILE_MOVE";
    case T_FN_MONSTER_HPRECOVERY:                        return "T_FN_MONSTER_HPRECOVERY";
    case T_FN_MONSTER_HIDE:                                return "T_FN_MONSTER_HIDE";
    case T_FN_MONSTER_SHOW:                                return "T_FN_MONSTER_SHOW";
    case T_FC_BATTLE_ATTACK:                            return "T_FC_BATTLE_ATTACK";
    case T_FC_BATTLE_ATTACK_OK:                            return "T_FC_BATTLE_ATTACK_OK";
    case T_FC_BATTLE_ATTACK_FIND:                        return "T_FC_BATTLE_ATTACK_FIND";
    case T_FC_BATTLE_ATTACK_FIND_OK:                    return "T_FC_BATTLE_ATTACK_FIND_OK";
    case T_FC_BATTLE_DROP_MINE:                            return "T_FC_BATTLE_DROP_MINE";
    case T_FC_BATTLE_DROP_MINE_OK:                        return "T_FC_BATTLE_DROP_MINE_OK";
    case T_FC_BATTLE_MINE_ATTACK:                        return "T_FC_BATTLE_MINE_ATTACK";
    case T_FC_BATTLE_MINE_ATTACK_OK:                    return "T_FC_BATTLE_MINE_ATTACK_OK";
    case T_FC_BATTLE_MINE_ATTACK_FIND:                    return "T_FC_BATTLE_MINE_ATTACK_FIND";
    case T_FC_BATTLE_MINE_ATTACK_FIND_OK:                return "T_FC_BATTLE_MINE_ATTACK_FIND_OK";
    case T_FC_BATTLE_REQUEST_PK:                        return "T_FC_BATTLE_REQUEST_PK";
    case T_FC_BATTLE_REQUEST_PK_OK:                        return "T_FC_BATTLE_REQUEST_PK_OK";
    case T_FC_BATTLE_CANCEL_PK:                            return "T_FC_BATTLE_CANCEL_PK";
    case T_FC_BATTLE_REQUEST_P2P_PK:                    return "T_FC_BATTLE_REQUEST_P2P_PK";
    case T_FC_BATTLE_REQUEST_P2P_PK_OK:                    return "T_FC_BATTLE_REQUEST_P2P_PK_OK";
    case T_FC_BATTLE_ACCEPT_REQUEST_P2P_PK:                return "T_FC_BATTLE_ACCEPT_REQUEST_P2P_PK";
    case T_FC_BATTLE_ACCEPT_REQUEST_P2P_PK_OK:            return "T_FC_BATTLE_ACCEPT_REQUEST_P2P_PK_OK";
    case T_FC_BATTLE_REJECT_REQUEST_P2P_PK:                return "T_FC_BATTLE_REJECT_REQUEST_P2P_PK";
    case T_FC_BATTLE_REJECT_REQUEST_P2P_PK_OK:            return "T_FC_BATTLE_REJECT_REQUEST_P2P_PK_OK";
    case T_FC_BATTLE_SURRENDER_P2P_PK:                    return "T_FC_BATTLE_SURRENDER_P2P_PK";
    case T_FC_BATTLE_SURRENDER_P2P_PK_OK:                return "T_FC_BATTLE_SURRENDER_P2P_PK_OK";
    case T_FC_BATTLE_ACCEPT_SURRENDER_P2P_PK:            return "T_FC_BATTLE_ACCEPT_SURRENDER_P2P_PK";
    case T_FC_BATTLE_REJECT_SURRENDER_P2P_PK:            return "T_FC_BATTLE_REJECT_SURRENDER_P2P_PK";
    case T_FC_BATTLE_REJECT_SURRENDER_P2P_PK_OK:        return "T_FC_BATTLE_REJECT_SURRENDER_P2P_PK_OK";
    case T_FC_BATTLE_END_P2P_PK:                        return "T_FC_BATTLE_END_P2P_PK";
    case T_FC_BATTLE_ATTACK_EXPLODE_ITEM:                return "T_FC_BATTLE_ATTACK_EXPLODE_ITEM";
    case T_FC_BATTLE_ATTACK_HIDE_ITEM:                    return "T_FC_BATTLE_ATTACK_HIDE_ITEM";
    case T_FC_BATTLE_ATTACK_EXPLODE_ITEM_W_KIND:        return "T_FC_BATTLE_ATTACK_EXPLODE_ITEM_W_KIND";
    case T_FC_BATTLE_ATTACK_HIDE_ITEM_W_KIND:            return "T_FC_BATTLE_ATTACK_HIDE_ITEM_W_KIND";
    case T_FC_BATTLE_TOGGLE_SHIELD:                        return "T_FC_BATTLE_TOGGLE_SHIELD";
    case T_FC_BATTLE_TOGGLE_SHIELD_RESULT:                return "T_FC_BATTLE_TOGGLE_SHIELD_RESULT";
    case T_FC_BATTLE_DROP_DUMMY:                        return "T_FC_BATTLE_DROP_DUMMY";
    case T_FC_BATTLE_DROP_DUMMY_OK:                        return "T_FC_BATTLE_DROP_DUMMY_OK";
    case T_FC_BATTLE_DROP_FIXER:                        return "T_FC_BATTLE_DROP_FIXER";
    case T_FC_BATTLE_DROP_FIXER_OK:                        return "T_FC_BATTLE_DROP_FIXER_OK";
    case T_FC_BATTLE_PRI_BULLET_RELOADED:                return "T_FC_BATTLE_PRI_BULLET_RELOADED";
    case T_FC_BATTLE_SEC_BULLET_RELOADED:                return "T_FC_BATTLE_SEC_BULLET_RELOADED";
    case T_FC_BATTLE_SHIELD_DAMAGE:                        return "T_FC_BATTLE_SHIELD_DAMAGE";
    case T_FC_BATTLE_TOGGLE_DECOY:                        return "T_FC_BATTLE_TOGGLE_DECOY";
    case T_FC_BATTLE_TOGGLE_DECOY_OK:                    return "T_FC_BATTLE_TOGGLE_DECOY_OK";
    case T_FC_BATTLE_SHOW_DAMAGE:                        return "T_FC_BATTLE_SHOW_DAMAGE";
    case T_FC_BATTLE_ATTACK_EVASION:                    return "T_FC_BATTLE_ATTACK_EVASION";
    case T_FC_BATTLE_ATTACK_EVASION_OK:                    return "T_FC_BATTLE_ATTACK_EVASION_OK";
    case T_FC_BATTLE_DELETE_DUMMY_OK:                    return "T_FC_BATTLE_DELETE_DUMMY_OK";        // 2006-12-04 by dhjin
    case T_FC_BATTLE_EXPLODE_DUMMY_OK:                    return "T_FC_BATTLE_EXPLODE_DUMMY_OK";        // 2006-12-04 by dhjin
    case T_FC_BATTLE_MONSTER_BARRIER_USING:                return "T_FC_BATTLE_MONSTER_BARRIER_USING";        // 2009-09-09 ~ 2010 by dhjin, 인피니티 - F -> C
    case T_FC_BATTLE_MONSTER_BARRIER_USE:                return "T_FC_BATTLE_MONSTER_BARRIER_USE";        // 2009-09-09 ~ 2010 by dhjin, 인피니티 - F -> C(n)

    case T_FN_BATTLE_ATTACK_PRIMARY:                    return "T_FN_BATTLE_ATTACK_PRIMARY";
    case T_FN_BATTLE_ATTACK_RESULT_PRIMARY:                return "T_FN_BATTLE_ATTACK_RESULT_PRIMARY";
    case T_FN_BATTLE_ATTACK_SECONDARY:                    return "T_FN_BATTLE_ATTACK_SECONDARY";
    case T_FN_BATTLE_ATTACK_RESULT_SECONDARY:            return "T_FN_BATTLE_ATTACK_RESULT_SECONDARY";
    case T_FN_BATTLE_ATTACK_FIND:                        return "T_FN_BATTLE_ATTACK_FIND";
    case T_FN_BATTLE_ATTACK_FIND_RESULT:                return "T_FN_BATTLE_ATTACK_FIND_RESULT";
    case T_FN_BATTLE_SET_ATTACK_CHARACTER:                return "T_FN_BATTLE_SET_ATTACK_CHARACTER";
    case T_FN_BATTLE_DROP_FIXER:                        return "T_FN_BATTLE_DROP_FIXER";
    case T_FN_BATTLE_DROP_FIXER_OK:                        return "T_FN_BATTLE_DROP_FIXER_OK";
    case T_FN_BATTLE_ATTACK_HIDE_ITEM_W_KIND:            return "T_FN_BATTLE_ATTACK_HIDE_ITEM_W_KIND";
    case T_FN_BATTLE_ATTACK_SKILL:                        return "T_FN_BATTLE_ATTACK_SKILL";                // 2009-09-09 ~ 2010 by dhjin, 인피니티 - N -> F
    case T_FC_PARTY_CREATE_OK:                            return "T_FC_PARTY_CREATE_OK";
    case T_FC_PARTY_REQUEST_INVITE:                        return "T_FC_PARTY_REQUEST_INVITE";
    case T_FC_PARTY_REQUEST_INVITE_QUESTION:            return "T_FC_PARTY_REQUEST_INVITE_QUESTION";
    case T_FC_PARTY_ACCEPT_INVITE:                        return "T_FC_PARTY_ACCEPT_INVITE";
    case T_FC_PARTY_REJECT_INVITE:                        return "T_FC_PARTY_REJECT_INVITE";
    case T_FC_PARTY_REJECT_INVITE_OK:                    return "T_FC_PARTY_REJECT_INVITE_OK";
    case T_FC_PARTY_GET_MEMBER:                            return "T_FC_PARTY_GET_MEMBER";
    case T_FC_PARTY_PUT_MEMBER:                            return "T_FC_PARTY_PUT_MEMBER";
    case T_FC_PARTY_GET_ALL_MEMBER:                        return "T_FC_PARTY_GET_ALL_MEMBER";
    case T_FC_PARTY_PUT_ALL_MEMBER:                        return "T_FC_PARTY_PUT_ALL_MEMBER";
    case T_FC_PARTY_UPDATE_MEMBER_INFO_ALL:                return "T_FC_PARTY_UPDATE_MEMBER_INFO_ALL";
    case T_FC_PARTY_UPDATE_MEMBER_INFO_LEVEL:            return "T_FC_PARTY_UPDATE_MEMBER_INFO_LEVEL";
    case T_FC_PARTY_UPDATE_MEMBER_INFO_HP:                return "T_FC_PARTY_UPDATE_MEMBER_INFO_HP";
    case T_FC_PARTY_UPDATE_MEMBER_INFO_CURRENT_HP:        return "T_FC_PARTY_UPDATE_MEMBER_INFO_CURRENT_HP";
    case T_FC_PARTY_UPDATE_MEMBER_INFO_DP:                return "T_FC_PARTY_UPDATE_MEMBER_INFO_DP";
    case T_FC_PARTY_UPDATE_MEMBER_INFO_CURRENT_DP:        return "T_FC_PARTY_UPDATE_MEMBER_INFO_CURRENT_DP";
    case T_FC_PARTY_UPDATE_MEMBER_INFO_SP:                return "T_FC_PARTY_UPDATE_MEMBER_INFO_SP";
    case T_FC_PARTY_UPDATE_MEMBER_INFO_CURRENT_SP:        return "T_FC_PARTY_UPDATE_MEMBER_INFO_CURRENT_SP";
    case T_FC_PARTY_UPDATE_MEMBER_INFO_EP:                return "T_FC_PARTY_UPDATE_MEMBER_INFO_EP";
    case T_FC_PARTY_UPDATE_MEMBER_INFO_CURRENT_EP:        return "T_FC_PARTY_UPDATE_MEMBER_INFO_CURRENT_EP";
    case T_FC_PARTY_UPDATE_MEMBER_INFO_BODYCONDITION:    return "T_FC_PARTY_UPDATE_MEMBER_INFO_BODYCONDITION";
    case T_FC_PARTY_REQUEST_PARTY_WARP:                    return "T_FC_PARTY_REQUEST_PARTY_WARP";
    case T_FC_PARTY_REQUEST_PARTY_WARP_WITH_MAP_NAME:    return "T_FC_PARTY_REQUEST_PARTY_WARP_WITH_MAP_NAME";
    case T_FC_PARTY_REQUEST_PARTY_OBJECT_EVENT:            return "T_FC_PARTY_REQUEST_PARTY_OBJECT_EVENT";
    case T_FC_PARTY_GET_OTHER_MOVE:                        return "T_FC_PARTY_GET_OTHER_MOVE";
    case T_FC_PARTY_BATTLE_START:                        return "T_FC_PARTY_BATTLE_START";
    case T_FC_PARTY_BATTLE_END:                            return "T_FC_PARTY_BATTLE_END";
    case T_FC_PARTY_PUT_ITEM_OTHER:                        return "T_FC_PARTY_PUT_ITEM_OTHER";
    case T_FI_PARTY_CREATE_OK:                            return "T_FI_PARTY_CREATE_OK";
    case T_FI_PARTY_ACCEPT_INVITE_OK:                    return "T_FI_PARTY_ACCEPT_INVITE_OK";
    case T_FI_PARTY_BAN_MEMBER_OK:                        return "T_FI_PARTY_BAN_MEMBER_OK";
    case T_FI_PARTY_LEAVE_OK:                            return "T_FI_PARTY_LEAVE_OK";
    case T_FI_PARTY_TRANSFER_MASTER_OK:                    return "T_FI_PARTY_TRANSFER_MASTER_OK";
    case T_FI_PARTY_DISMEMBER_OK:                        return "T_FI_PARTY_DISMEMBER_OK";
    case T_FI_PARTY_CHANGE_FLIGHT_FORMATION_OK:            return "T_FI_PARTY_CHANGE_FLIGHT_FORMATION_OK";
    case T_FI_PARTY_CHANGE_FLIGHT_POSITION:                return "T_FI_PARTY_CHANGE_FLIGHT_POSITION";
    case T_FI_PARTY_CANCEL_FLIGHT_POSITION:                return "T_FI_PARTY_CANCEL_FLIGHT_POSITION";
    case T_FI_PARTY_NOTIFY_BATTLE_PARTY:                return "T_FI_PARTY_NOTIFY_BATTLE_PARTY";
    case T_FI_PARTY_NOTIFY_BATTLE_PARTY_OK:                return "T_FI_PARTY_NOTIFY_BATTLE_PARTY_OK";
    case T_FI_PARTY_ADD_MEMBER:                            return "T_FI_PARTY_ADD_MEMBER";
    case T_FI_PARTY_DELETE_MEMBER:                        return "T_FI_PARTY_DELETE_MEMBER";
    case T_FI_PARTY_UPDATE_ITEM_POS:                    return "T_FI_PARTY_UPDATE_ITEM_POS";
    case T_FI_PARTY_ALL_FLIGHT_POSITION:                return "T_FI_PARTY_ALL_FLIGHT_POSITION";
    case T_FI_PARTY_UPDATE_PARTY_INFO:                    return "T_FI_PARTY_UPDATE_PARTY_INFO";
    case T_FI_PARTY_CHANGE_EXP_DISTRIBUTE_TYPE:            return "T_FI_PARTY_CHANGE_EXP_DISTRIBUTE_TYPE";        // 2008-06-04 by dhjin, EP3 편대 수정 - 경험치 분배 방식 변경 
    case T_FI_PARTY_CHANGE_ITEM_DISTRIBUTE_TYPE:        return "T_FI_PARTY_CHANGE_ITEM_DISTRIBUTE_TYPE";    // 2008-06-04 by dhjin, EP3 편대 수정 - 아이템 분배 방식 변경
    case T_FI_PARTY_CHANGE_FORMATION_SKILL:                return "T_FI_PARTY_CHANGE_FORMATION_SKILL";            // 2009-08-03 by cmkwon, EP3-4 편대 대형 스킬 구현 - 
    case T_FI_PARTY_ARENA_TEAM_NUM:                        return "T_FI_PARTY_ARENA_TEAM_NUM";                    // 2012-04-12 by jhseol, 아레나 추가개발 - 자동파티 : 같은 팀인지 체크할 팀번호 설정

    case T_IC_PARTY_CREATE:                                return "T_IC_PARTY_CREATE";
    case T_IC_PARTY_ACCEPT_INVITE_OK:                    return "T_IC_PARTY_ACCEPT_INVITE_OK";
    case T_IC_PARTY_GET_MEMBER:                            return "T_IC_PARTY_GET_MEMBER";
    case T_IC_PARTY_PUT_MEMBER:                            return "T_IC_PARTY_PUT_MEMBER";
    case T_IC_PARTY_GET_ALL_MEMBER:                        return "T_IC_PARTY_GET_ALL_MEMBER";
    case T_IC_PARTY_PUT_ALL_MEMBER:                        return "T_IC_PARTY_PUT_ALL_MEMBER";
    case T_IC_PARTY_BAN_MEMBER:                            return "T_IC_PARTY_BAN_MEMBER";
    case T_IC_PARTY_BAN_MEMBER_OK:                        return "T_IC_PARTY_BAN_MEMBER_OK";
    case T_IC_PARTY_LEAVE:                                return "T_IC_PARTY_LEAVE";
    case T_IC_PARTY_LEAVE_OK:                            return "T_IC_PARTY_LEAVE_OK";
    case T_IC_PARTY_TRANSFER_MASTER:                    return "T_IC_PARTY_TRANSFER_MASTER";
    case T_IC_PARTY_TRANSFER_MASTER_OK:                    return "T_IC_PARTY_TRANSFER_MASTER_OK";
    case T_IC_PARTY_DISMEMBER:                            return "T_IC_PARTY_DISMEMBER";
    case T_IC_PARTY_DISMEMBER_OK:                        return "T_IC_PARTY_DISMEMBER_OK";
    case T_IC_PARTY_CHANGE_FLIGHT_FORMATION:            return "T_IC_PARTY_CHANGE_FLIGHT_FORMATION";
    case T_IC_PARTY_CHANGE_FLIGHT_FORMATION_OK:            return "T_IC_PARTY_CHANGE_FLIGHT_FORMATION_OK";
    case T_IC_PARTY_GET_FLIGHT_POSITION:                return "T_IC_PARTY_GET_FLIGHT_POSITION";
    case T_IC_PARTY_CHANGE_FLIGHT_POSITION:                return "T_IC_PARTY_CHANGE_FLIGHT_POSITION";
    case T_IC_PARTY_CANCEL_FLIGHT_POSITION:                return "T_IC_PARTY_CANCEL_FLIGHT_POSITION";
    case T_IC_PARTY_PUT_LAST_PARTY_INFO:                return "T_IC_PARTY_PUT_LAST_PARTY_INFO";
    case T_IC_PARTY_UPDATE_MEMBER_INFO_MAPNAME:            return "T_IC_PARTY_UPDATE_MEMBER_INFO_MAPNAME";
    case T_IC_PARTY_MEMBER_INVALIDATED:                    return "T_IC_PARTY_MEMBER_INVALIDATED";
    case T_IC_PARTY_MEMBER_REJOINED:                    return "T_IC_PARTY_MEMBER_REJOINED";
    case T_IC_PARTY_UPDATE_ITEM_POS:                    return "T_IC_PARTY_UPDATE_ITEM_POS";
    case T_IC_PARTY_ALL_FLIGHT_POSITION:                return "T_IC_PARTY_ALL_FLIGHT_POSITION";
    case T_IC_PARTY_REQUEST_PARTYINFO_FROM_A_TO_M:        return "T_IC_PARTY_REQUEST_PARTYINFO_FROM_A_TO_M";    // 2008-02-28 by dhjin, 아레나 통합 - 
    case T_IC_PARTY_LEAVE_FROM_M_TO_A:                    return "T_IC_PARTY_LEAVE_FROM_M_TO_A";                // 2008-02-28 by dhjin, 아레나 통합 -
    case T_IC_PARTY_LEAVE_FROM_A_TO_M:                    return "T_IC_PARTY_LEAVE_FROM_A_TO_M";                // 2008-02-28 by dhjin, 아레나 통합 -
    case T_IC_PARTY_LIST_INFO:                            return "T_IC_PARTY_LIST_INFO";                        // C -> I, 2008-06-02 by dhjin, EP3 편대 수정 - 편대 정보 리스트 요청
    case T_IC_PARTY_LIST_INFO_OK:                        return "T_IC_PARTY_LIST_INFO_OK";                    // I -> C, 2008-06-02 by dhjin, EP3 편대 수정 - 편대 정보 리스트 요청 OK
    case T_IC_PARTY_JOIN_FREE:                            return "T_IC_PARTY_JOIN_FREE";                        // C -> I, 2008-06-03 by dhjin, EP3 편대 수정 - 편대 자유 참여
    case T_IC_PARTY_JOIN_FREE_OK:                        return "T_IC_PARTY_JOIN_FREE_OK";                    // I -> C, 2008-06-03 by dhjin, EP3 편대 수정 - 편대 자유 참여 OK
    case T_IC_PARTY_CHANGE_INFO:                        return "T_IC_PARTY_CHANGE_INFO";                    // C -> I, 2008-06-04 by dhjin, EP3 편대 수정 - 편대 정보 수정
    case T_IC_PARTY_CHANGE_INFO_OK:                        return "T_IC_PARTY_CHANGE_INFO_OK";                    // I -> C, 2008-06-04 by dhjin, EP3 편대 수정 - 편대 정보 수정 OK
    case T_IC_PARTY_RECOMMENDATION_MEMBER:                return "T_IC_PARTY_RECOMMENDATION_MEMBER";            // C -> I, 2008-06-04 by dhjin, EP3 편대 수정 - 추천 케릭터 요청
    case T_IC_PARTY_RECOMMENDATION_MEMBER_OK:            return "T_IC_PARTY_RECOMMENDATION_MEMBER_OK";        // I -> C, 2008-06-04 by dhjin, EP3 편대 수정 - 추천 케릭터 요청 OK
    case T_IC_PARTY_INFO:                                return "T_IC_PARTY_INFO";                            // I -> C, 2008-06-10 by dhjin, EP3 편대 수정 - 편대 정보 전송
    case T_FC_PARTY_REQUEST_INVITE_QUESTION_THE_ARENA:    return "T_FC_PARTY_REQUEST_INVITE_QUESTION_THE_ARENA";    // 2012-04-12 by jhseol, 아레나 추가개발 - 자동파티 : 아레나 방 입장시 자동으로 파티를 생성할때 사용하는 패킷
    case T_FC_PARTY_AUTO_CREATED_THE_ARENA:                return "T_FC_PARTY_AUTO_CREATED_THE_ARENA";            // 2012-04-12 by jhseol, 아레나 추가개발 - 자동파티 : 아레나 방 입장시 자동으로 파티를 생성할때 사용하는 패킷
    
    case T_FI_CHARACTER_DELETE_CHARACTER:                return "T_FI_CHARACTER_DELETE_CHARACTER";
    case T_FI_CHARACTER_CHANGE_LEVEL:                    return "T_FI_CHARACTER_CHANGE_LEVEL";
    case T_FI_CHARACTER_UPDATE_GUILD_INFO:                return "T_FI_CHARACTER_UPDATE_GUILD_INFO";
    case T_FI_CHARACTER_UPDATE_MAP_CHANNEL:                return "T_FI_CHARACTER_UPDATE_MAP_CHANNEL";
    case T_FI_CHARACTER_CHANGE_INFLUENCE_TYPE:                return "T_FI_CHARACTER_CHANGE_INFLUENCE_TYPE";
    case T_FI_UPDATE_SUBLEADER:                            return "T_FI_UPDATE_SUBLEADER";
    case T_FI_CREATE_GUILD_BY_SUBLEADER:                return "T_FI_CREATE_GUILD_BY_SUBLEADER";
        
    case T_FC_MONSTER_CREATED:                            return "T_FC_MONSTER_CREATED";
    case T_FC_MONSTER_MOVE_OK:                            return "T_FC_MONSTER_MOVE_OK";
    case T_FC_MONSTER_HIDE:                                return "T_FC_MONSTER_HIDE";
    case T_FC_MONSTER_SHOW:                                return "T_FC_MONSTER_SHOW";
    case T_FC_MONSTER_CHANGE_HP:                        return "T_FC_MONSTER_CHANGE_HP";
    case T_FC_MONSTER_CHANGE_BODYCONDITION:                return "T_FC_MONSTER_CHANGE_BODYCONDITION";
    case T_FC_MONSTER_SKILL_USE_SKILL:                    return "T_FC_MONSTER_SKILL_USE_SKILL";
    case T_FC_MONSTER_SKILL_END_SKILL:                    return "T_FC_MONSTER_SKILL_END_SKILL";
    case T_FC_MONSTER_SUMMON_MONSTER:                    return "T_FC_MONSTER_SUMMON_MONSTER";
    case T_FC_MONSTER_TUTORIAL_MONSTER_DEAD_NOTIFY:        return "T_FC_MONSTER_TUTORIAL_MONSTER_DEAD_NOTIFY";
    case T_FC_MONSTER_TUTORIAL_MONSTER_DELETE:            return "T_FC_MONSTER_TUTORIAL_MONSTER_DELETE";
    case T_FC_MONSTER_CHANGE_INDEX:                        return "T_FC_MONSTER_CHANGE_INDEX";        // 2011-05-17 by hskim, 인피니티 3차 - 시네마 몬스터 교체 기능

    case T_FC_MONSTER_CINEMA_DELETE_NOTIFY:                return "T_FC_MONSTER_CINEMA_DELETE_NOTIFY";        // 2011-05-30 by hskim, 인피니티 3차 - 몬스터 삭제 클라이언트 반영

    case T_FC_TRIGGER_MAP_BUFF_SKILL:                    return "T_FC_TRIGGER_MAP_BUFF_SKILL";            // 2011-10-18 by hskim, EP4 [트리거 시스템] - 화산재 / 모래 폭풍
    case T_FC_SET_LIMITTIME:                            return "T_FC_SET_LIMITTIME";                    // 2011-10-28 by hskim, EP4 [트리거 시스템] - 크리스탈 시스템

    case T_FN_MAPPROJECT_START:                            return "T_FN_MAPPROJECT_START";
    case T_FN_MAPPROJECT_START_OK:                        return "T_FN_MAPPROJECT_START_OK";
    case T_FN_MAPPROJECT_START_OK_ACK:                    return "T_FN_MAPPROJECT_START_OK_ACK";
    case T_FN_MONSTER_CREATE:                            return "T_FN_MONSTER_CREATE";
    case T_FN_MONSTER_CREATE_OK:                        return "T_FN_MONSTER_CREATE_OK";
    case T_FN_MONSTER_DELETE:                            return "T_FN_MONSTER_DELETE";
    case T_FN_CLIENT_GAMESTART_OK:                        return "T_FN_CLIENT_GAMESTART_OK";
    case T_FN_CLIENT_GAMEEND_OK:                        return "T_FN_CLIENT_GAMEEND_OK";
    case T_FN_GET_CHARACTER_INFO:                        return "T_FN_GET_CHARACTER_INFO";
    case T_FN_GET_CHARACTER_INFO_OK:                    return "T_FN_GET_CHARACTER_INFO_OK";
    case T_FN_ADMIN_SUMMON_MONSTER:                        return "T_FN_ADMIN_SUMMON_MONSTER";
    case T_FN_MONSTER_CHANGE_HP:                        return "T_FN_MONSTER_CHANGE_HP";
    case T_FN_MONSTER_CHANGE_BODYCONDITION:                return "T_FN_MONSTER_CHANGE_BODYCONDITION";
    case T_FN_MONSTER_SKILL_USE_SKILL:                    return "T_FN_MONSTER_SKILL_USE_SKILL";
    case T_FN_MONSTER_SKILL_END_SKILL:                    return "T_FN_MONSTER_SKILL_END_SKILL";
    case T_FN_MONSTER_AUTO_DESTROYED:                    return "T_FN_MONSTER_AUTO_DESTROYED";
    case T_FN_MONSTER_STRATEGYPOINT_INIT:                return "T_FN_MONSTER_STRATEGYPOINT_INIT";        // 2006-11-20 by cmkwon
    case T_FN_MONSTER_STRATEGYPOINT_SUMMON:                return "T_FN_MONSTER_STRATEGYPOINT_SUMMON";        // 2007-03-02 by dhjin
    case T_FN_MONSTER_OUTPOST_INIT:                        return "T_FN_MONSTER_OUTPOST_INIT";                // 2007-08-24 by dhjin
    case T_FN_MONSTER_OUTPOST_RESET_SUMMON:                return "T_FN_MONSTER_OUTPOST_RESET_SUMMON";            // 2007-08-24 by dhjin
    case T_FN_MONSTER_CREATE_IN_MAPCHANNEL_BYVALUE:        return "T_FN_MONSTER_CREATE_IN_MAPCHANNEL_BYVALUE";            // 2007-08-29 by dhjin
    case T_FN_MONSTER_TELEPORT_SUMMON:                    return "T_FN_MONSTER_TELEPORT_SUMMON";            // 2007-08-24 by dhjin

    case T_FN_NPCSERVER_START:                            return "T_FN_NPCSERVER_START";
    case T_FN_NPCSERVER_START_OK:                        return "T_FN_NPCSERVER_START_OK";
    case T_FN_NPCSERVER_SUMMON_JACO_MONSTER:            return "T_FN_NPCSERVER_SUMMON_JACO_MONSTER";
    case T_FN_NPCSERVER_DELETE_MONSTER_IN_MAPCHANNEL:    return "T_FN_NPCSERVER_DELETE_MONSTER_IN_MAPCHANNEL";    // 2007-08-22 by cmkwon, 해당 맵채널 몬스터 모두 삭제하기 기능 추가

    case T_FN_MONSTER_CHANGE_OK:                        return "T_FN_MONSTER_CHANGE_OK";        // 2011-05-11 by hskim, 인피니티 3차 - 시네마 관련 기능 추가 - 해당 맵채널 특정 몬스터 변경 기능 추가
    case T_FN_NPCSERVER_CINEMA_MONSTER_DESTROY:            return "T_FN_NPCSERVER_CINEMA_MONSTER_DESTROY";        // 2011-04-28 by hskim, 인피니티 3차 - 시네마 관련 기능 추가 - 해당 맵채널 특정 몬스터 삭제 기능 추가
    case T_FN_NPCSERVER_CINEMA_MONSTER_CHANGE:            return "T_FN_NPCSERVER_CINEMA_MONSTER_CHANGE";        // 2011-05-11 by hskim, 인피니티 3차 - 시네마 관련 기능 추가 - 해당 맵채널 특정 몬스터 변경 기능 추가
    case T_FN_NPCSERVER_CINEMA_MONSTER_REGEN:            return "T_FN_NPCSERVER_CINEMA_MONSTER_REGEN";        // 2011-06-02 by hskim, 인피니티 3차 - 스텝 6 - 주기적 소환 기능 제작

    case T_FC_INFINITY_SET_LIMITTIME:                    return "T_FC_INFINITY_SET_LIMITTIME";        // 2011-05-30 by hskim, 인피니티 3차 - 플레이 시간 재설정 기능 

    case T_FC_EVENT_WARP:                                return "T_FC_EVENT_WARP";
    case T_FC_EVENT_WARP_SAME_MAP:                        return "T_FC_EVENT_WARP_SAME_MAP";
    case T_FC_EVENT_WARP_SAME_MAP_DONE:                    return "T_FC_EVENT_WARP_SAME_MAP_DONE";
    case T_FC_EVENT_WARP_SAME_FIELD_SERVER:                return "T_FC_EVENT_WARP_SAME_FIELD_SERVER";
    case T_FC_EVENT_WARP_SAME_FIELD_SERVER_DONE:        return "T_FC_EVENT_WARP_SAME_FIELD_SERVER_DONE";
    case T_FC_EVENT_OTHER_WARPED:                        return "T_FC_EVENT_OTHER_WARPED";
    case T_FC_EVENT_WARP_CONNECT_OK:                    return "T_FC_EVENT_WARP_CONNECT_OK";
    case T_FC_EVENT_ENTER_BUILDING:                        return "T_FC_EVENT_ENTER_BUILDING";
    case T_FC_EVENT_ENTER_BUILDING_OK:                    return "T_FC_EVENT_ENTER_BUILDING_OK";
    case T_FC_EVENT_LEAVE_BUILDING:                        return "T_FC_EVENT_LEAVE_BUILDING";
    case T_FC_EVENT_LEAVE_BUILDING_OK:                    return "T_FC_EVENT_LEAVE_BUILDING_OK";
    case T_FC_EVENT_REQUEST_WARP:                        return "T_FC_EVENT_REQUEST_WARP";
    case T_FC_EVENT_REJECT_WARP:                        return "T_FC_EVENT_REJECT_WARP";
    case T_FC_EVENT_REQUEST_OBJECT_EVENT:                return "T_FC_EVENT_REQUEST_OBJECT_EVENT";
    case T_FC_EVENT_CHANGE_WEATHER:                        return "T_FC_EVENT_CHANGE_WEATHER";
    case T_FC_EVENT_SUGGEST_CHANNELS:                    return "T_FC_EVENT_SUGGEST_CHANNELS";
    case T_FC_EVENT_SELECT_CHANNEL:                        return "T_FC_EVENT_SELECT_CHANNEL";
    case T_FC_EVENT_SELECT_CHANNEL_WITH_PARTY:            return "T_FC_EVENT_SELECT_CHANNEL_WITH_PARTY";
    case T_FC_EVENT_REQUEST_RACING_WARP:                return "T_FC_EVENT_REQUEST_RACING_WARP";
    case T_FC_EVENT_GET_SHOP_WARP_TARGET_MAP_LIST:        return "T_FC_EVENT_GET_SHOP_WARP_TARGET_MAP_LIST";
    case T_FC_EVENT_GET_SHOP_WARP_TARGET_MAP_LIST_OK:    return "T_FC_EVENT_GET_SHOP_WARP_TARGET_MAP_LIST_OK";
    case T_FC_EVENT_REQUEST_SHOP_WARP:                    return "T_FC_EVENT_REQUEST_SHOP_WARP";
    case T_FC_EVENT_CHARACTERMODE_ENTER_BUILDING:        return "T_FC_EVENT_CHARACTERMODE_ENTER_BUILDING";
    case T_FC_EVENT_CALL_WARP_EVENT_REQUEST:            return "T_FC_EVENT_CALL_WARP_EVENT_REQUEST";
    case T_FC_EVENT_CALL_WARP_EVENT_REQUEST_ACK:        return "T_FC_EVENT_CALL_WARP_EVENT_REQUEST_ACK";
    case T_FC_EVENT_CLICK_TELEPORT:                        return "T_FC_EVENT_CLICK_TELEPORT";
    case T_FC_EVENT_CLICK_TELEPORT_OK:                    return "T_FC_EVENT_CLICK_TELEPORT_OK";            // 2008-04-22 by dhjin, 모선전 정보 표시 기획안 - 텔레포트 빌딩 완료 시간
    case T_FC_EVENT_TELEPORT_BUILDCOMPLETE:                return "T_FC_EVENT_TELEPORT_BUILDCOMPLETE";        
    case T_FC_EVENT_TELEPORT_DESTROY:                    return "T_FC_EVENT_TELEPORT_DESTROY";

    case T_FC_EVENT_NOTIFY_MSG_GET:                        return "T_FC_EVENT_NOTIFY_MSG_GET";        // 2007-11-28 by cmkwon, 통지시스템 구현 -
    case T_FC_EVENT_NOTIFY_MSG_GET_OK:                    return "T_FC_EVENT_NOTIFY_MSG_GET_OK";    // 2007-11-28 by cmkwon, 통지시스템 구현 -
    case T_FC_EVENT_NOTIFY_MSG_DELETE:                    return "T_FC_EVENT_NOTIFY_MSG_DELETE";    // 2007-11-28 by cmkwon, 통지시스템 구현 -

    case T_FC_EVENT_COUPON_EVENT_INFO:                    return "T_FC_EVENT_COUPON_EVENT_INFO";            // 2008-01-10 by cmkwon, 아이템 이벤트 시스템에 신 쿠폰 시스템 추가 - 
    case T_FC_EVENT_COUPON_EVENT_USE_COUPON:            return "T_FC_EVENT_COUPON_EVENT_USE_COUPON";    // 2008-01-10 by cmkwon, 아이템 이벤트 시스템에 신 쿠폰 시스템 추가 - 
    case T_FC_EVENT_COUPON_EVENT_USE_COUPON_OK:            return "T_FC_EVENT_COUPON_EVENT_USE_COUPON_OK";    // 2008-01-10 by cmkwon, 아이템 이벤트 시스템에 신 쿠폰 시스템 추가 - 

    case T_FN_EVENT_WARP:                                return "T_FN_EVENT_WARP";
    case T_FN_EVENT_OTHER_WARPED:                        return "T_FN_EVENT_OTHER_WARPED";
    case T_FN_EVENT_WARP_CONNECT_OK:                    return "T_FN_EVENT_WARP_CONNECT_OK";
// 2005-07-27 by cmkwon, 다른 필드서버로의 워프는 없으므로 삭제함
//    case T_FP_EVENT_NOTIFY_WARP:                        return "T_FP_EVENT_NOTIFY_WARP";
//    case T_FP_EVENT_NOTIFY_WARP_OK:                        return "T_FP_EVENT_NOTIFY_WARP_OK";
    case T_FP_EVENT_ENTER_SELECT_SCREEN:                return "T_FP_EVENT_ENTER_SELECT_SCREEN";
    case T_FP_EVENT_GAME_STARTED:                        return "T_FP_EVENT_GAME_STARTED";
    case T_FP_EVENT_MAP_CHANGED:                        return "T_FP_EVENT_MAP_CHANGED";
    case T_FP_EVENT_RELOAD_HAPPYEV:                        return "T_FP_EVENT_RELOAD_HAPPYEV";
    case T_FP_EVENT_RELOAD_ITEMEV:                        return "T_FP_EVENT_RELOAD_ITEMEV";

    case T_FP_EVENT_UPDATE_PCBANGLIST:                    return "T_FP_EVENT_UPDATE_PCBANGLIST";
    case T_FP_EVENT_UPDATE_STRATEGYPOINT_NOTSUMMONTIM:    return "T_FP_EVENT_UPDATE_STRATEGYPOINT_NOTSUMMONTIM";

    case T_FP_CASH_CHANGE_CHARACTERNAME:                return "T_FP_CASH_CHANGE_CHARACTERNAME";

    case T_FP_ADMIN_BLOCKACCOUNT:                        return "T_FP_ADMIN_BLOCKACCOUNT";        // 2008-01-31 by cmkwon, 계정 블럭/해제 명령어로 가능한 시스템 구현 - 
    case T_FP_ADMIN_BLOCKACCOUNT_OK:                    return "T_FP_ADMIN_BLOCKACCOUNT_OK";    // 2008-01-31 by cmkwon, 계정 블럭/해제 명령어로 가능한 시스템 구현 - 
    case T_FP_ADMIN_UNBLOCKACCOUNT:                        return "T_FP_ADMIN_UNBLOCKACCOUNT";        // 2008-01-31 by cmkwon, 계정 블럭/해제 명령어로 가능한 시스템 구현 - 
    case T_FP_ADMIN_UNBLOCKACCOUNT_OK:                    return "T_FP_ADMIN_UNBLOCKACCOUNT_OK";    // 2008-01-31 by cmkwon, 계정 블럭/해제 명령어로 가능한 시스템 구현 - 
        
    case T_FP_ADMIN_STRATRGYPOINT_INFO_CHANGE:            return "T_FP_ADMIN_STRATRGYPOINT_INFO_CHANGE";    // 2012-11-13 by jhseol, 전쟁 시스템 리뉴얼 - 거점전

    case T_FP_MONITOR_RELOAD_VERSION_INFO_OK:            return "T_FP_MONITOR_RELOAD_VERSION_INFO_OK";        // 2008-09-08 by cmkwon, SCMonitor에서 ReloadVersionInfo시에 일부 체크섬파일(.\Res-Tex\*.*)도 리로드하기 - 

    case T_FC_STORE_GET_ITEM:                            return "T_FC_STORE_GET_ITEM";
    case T_FC_STORE_PUT_ITEM_HEADER:                    return "T_FC_STORE_PUT_ITEM_HEADER";
    case T_FC_STORE_PUT_ITEM:                            return "T_FC_STORE_PUT_ITEM";
    case T_FC_STORE_PUT_ITEM_DONE:                        return "T_FC_STORE_PUT_ITEM_DONE";
    case T_FC_STORE_MOVE_ITEM:                            return "T_FC_STORE_MOVE_ITEM";
    case T_FC_STORE_MOVE_ITEM_OK:                        return "T_FC_STORE_MOVE_ITEM_OK";
    case T_FC_STORE_INSERT_ITEM:                        return "T_FC_STORE_INSERT_ITEM";
    case T_FC_STORE_DELETE_ITEM:                        return "T_FC_STORE_DELETE_ITEM";
    case T_FC_STORE_UPDATE_ITEM_COUNT:                    return "T_FC_STORE_UPDATE_ITEM_COUNT";
    case T_FC_STORE_UPDATE_ENDURANCE:                    return "T_FC_STORE_UPDATE_ENDURANCE";
    case T_FC_STORE_UPDATE_POSSESS:                        return "T_FC_STORE_UPDATE_POSSESS";
    case T_FC_STORE_UPDATE_RARE_FIX:                    return "T_FC_STORE_UPDATE_RARE_FIX";
    case T_FC_STORE_INSERT_USINGITEM:                    return "T_FC_STORE_INSERT_USINGITEM";
    case T_FC_STORE_DELETE_USINGITEM:                    return "T_FC_STORE_DELETE_USINGITEM";
    case T_FC_STORE_UPDATE_USINGITEM:                    return "T_FC_STORE_UPDATE_USINGITEM";
    case T_FC_STORE_EXPIRE_USINGITEM:                    return "T_FC_STORE_EXPIRE_USINGITEM";        // 2006-10-11 by cmkwon
    case T_FC_STORE_UPDATE_ITEMSTORAGE:                    return "T_FC_STORE_UPDATE_ITEMSTORAGE";        // 2009-03-16 by cmkwon, 빠진것 추가
    case T_FC_STORE_UPDATE_ITEMNUM:                        return "T_FC_STORE_UPDATE_ITEMNUM";            // 2009-03-16 by cmkwon, 빠진것 추가
    case T_FC_STORE_REQUEST_QUICKSLOT:                    return "T_FC_STORE_REQUEST_QUICKSLOT";        // 2009-03-16 by cmkwon, 빠진것 추가
    case T_FC_STORE_REQUEST_QUICKSLOT_OK:                return "T_FC_STORE_REQUEST_QUICKSLOT_OK";    // 2009-03-16 by cmkwon, 빠진것 추가
    case T_FC_STORE_SAVE_QUICKSLOT:                        return "T_FC_STORE_SAVE_QUICKSLOT";            // 2009-03-16 by cmkwon, 빠진것 추가
    case T_FC_STORE_UPDATE_ARMOR_ITEM_EVENT_INFO:        return "T_FC_STORE_UPDATE_ARMOR_ITEM_EVENT_INFO";    // 2013-04-18 by jhseol,bckim 이달의 아머 - 이달의 아머 변경 사항 전달 패킷
        
    case T_FC_STORE_LOG_GUILD_ITEM:                        return "T_FC_STORE_LOG_GUILD_ITEM";
    case T_FC_STORE_LOG_GUILD_ITEM_OK:                    return "T_FC_STORE_LOG_GUILD_ITEM_OK";
    case T_FC_STORE_LOG_GUILD_ITEM_OK_HEADER:            return "T_FC_STORE_LOG_GUILD_ITEM_OK_HEADER";
    case T_FC_STORE_LOG_GUILD_ITEM_OK_DONE:                return "T_FC_STORE_LOG_GUILD_ITEM_OK_DONE";

    case T_FC_ITEM_SHOW_ITEM:                            return "T_FC_ITEM_SHOW_ITEM";
    case T_FC_ITEM_HIDE_ITEM:                            return "T_FC_ITEM_HIDE_ITEM";
    case T_FC_ITEM_GET_ITEM:                            return "T_FC_ITEM_GET_ITEM";
    case T_FC_ITEM_GET_ITEM_OK:                            return "T_FC_ITEM_GET_ITEM_OK";
    case T_FC_ITEM_GET_ITEM_FAIL:                        return "T_FC_ITEM_GET_ITEM_FAIL";
    case T_FC_ITEM_PUT_ITEM:                            return "T_FC_ITEM_PUT_ITEM";
// 2005-11-15 by cmkwon, 제거함
//    case T_FC_ITEM_BONUSSKILLPOINT:                        return "T_FC_ITEM_BONUSSKILLPOINT";
//    case T_FC_ITEM_BONUSSKILLPOINT_OK:                    return "T_FC_ITEM_BONUSSKILLPOINT_OK";
    case T_FC_ITEM_CHANGE_WINDOW_POSITION:                return "T_FC_ITEM_CHANGE_WINDOW_POSITION";
    case T_FC_ITEM_CHANGE_WINDOW_POSITION_OK:            return "T_FC_ITEM_CHANGE_WINDOW_POSITION_OK";
    case T_FC_ITEM_UPDATE_WINDOW_ITEM_LIST:                return "T_FC_ITEM_UPDATE_WINDOW_ITEM_LIST";
    case T_FC_ITEM_THROW_AWAY_ITEM:                        return "T_FC_ITEM_THROW_AWAY_ITEM";
    case T_FC_ITEM_THROW_AWAY_ITEM_OK:                    return "T_FC_ITEM_THROW_AWAY_ITEM_OK";
    case T_FC_ITEM_USE_ENERGY:                            return "T_FC_ITEM_USE_ENERGY";
    case T_FC_ITEM_USE_ENERGY_OK:                        return "T_FC_ITEM_USE_ENERGY_OK";
    case T_FC_ITEM_USE_ITEM:                            return "T_FC_ITEM_USE_ITEM";
    case T_FC_ITEM_USE_ITEM_OK:                            return "T_FC_ITEM_USE_ITEM_OK";
    case T_FC_ITEM_DELETE_ITEM_ADMIN:                    return "T_FC_ITEM_DELETE_ITEM_ADMIN";
    case T_FC_ITEM_RELOAD_ITEM_INFO:                    return "T_FC_ITEM_RELOAD_ITEM_INFO";
    case T_FC_ITEM_USE_ENCHANT:                            return "T_FC_ITEM_USE_ENCHANT";
    case T_FC_ITEM_USE_ENCHANT_OK:                        return "T_FC_ITEM_USE_ENCHANT_OK";
    case T_FC_ITEM_PUT_ENCHANT_HEADER:                    return "T_FC_ITEM_PUT_ENCHANT_HEADER";
    case T_FC_ITEM_PUT_ENCHANT:                            return "T_FC_ITEM_PUT_ENCHANT";
    case T_FC_ITEM_PUT_ENCHANT_DONE:                    return "T_FC_ITEM_PUT_ENCHANT_DONE";
    case T_FC_ITEM_DELETE_DROP_ITEM:                    return "T_FC_ITEM_DELETE_DROP_ITEM";
    case T_FC_ITEM_UPDATE_ITEM_POS:                        return "T_FC_ITEM_UPDATE_ITEM_POS";
    case T_FC_ITEM_MIX_ITEMS:                            return "T_FC_ITEM_MIX_ITEMS";
    case T_FC_ITEM_MIX_ITEMS_RESULT:                    return "T_FC_ITEM_MIX_ITEMS_RESULT";
    case T_FC_ITEM_USE_CARDITEM_GUILDSUMMON:            return "T_FC_ITEM_USE_CARDITEM_GUILDSUMMON";
    case T_FC_ITEM_USE_CARDITEM_GUILDSUMMON_NOTIFY:        return "T_FC_ITEM_USE_CARDITEM_GUILDSUMMON_NOTIFY";
    case T_FC_ITEM_USE_CARDITEM_RESTORE:                return "T_FC_ITEM_USE_CARDITEM_RESTORE";
    case T_FC_ITEM_USE_CARDITEM_RESTORE_NOTIFY:            return "T_FC_ITEM_USE_CARDITEM_RESTORE_NOTIFY";
    case T_FC_ITEM_USE_CARDITEM_GUILD:                    return "T_FC_ITEM_USE_CARDITEM_GUILD";
    case T_FC_ITEM_USE_CARDITEM_GUILD_NOTIFY:            return "T_FC_ITEM_USE_CARDITEM_GUILD_NOTIFY";
    case T_FC_ITEM_USE_CARDITEM_MONSTERSUMMON:            return "T_FC_ITEM_USE_CARDITEM_MONSTERSUMMON";
    case T_FC_ITEM_USE_CARDITEM_MONSTERSUMMON_NOTIFY:    return "T_FC_ITEM_USE_CARDITEM_MONSTERSUMMON_NOTIFY";
    case T_FC_ITEM_USE_CARDITEM_CHANGECHARACTERNAME:    return "T_FC_ITEM_USE_CARDITEM_CHANGECHARACTERNAME";
    case T_FC_ITEM_USE_CARDITEM_CHANGECHARACTERNAME_NOTIFY:        return "T_FC_ITEM_USE_CARDITEM_CHANGECHARACTERNAME_NOTIFY";
    case T_FC_ITEM_USE_CARDITEM_SKILLINITIALIZE:        return "T_FC_ITEM_USE_CARDITEM_SKILLINITIALIZE";
    case T_FC_ITEM_USE_CARDITEM_SKILLINITIALIZE_NOTIFY:        return "T_FC_ITEM_USE_CARDITEM_SKILLINITIALIZE_NOTIFY";
    case T_FC_ITEM_USE_CARDITEM_CHANGEPILOTFACE:        return "T_FC_ITEM_USE_CARDITEM_CHANGEPILOTFACE";
    case T_FC_ITEM_USE_CARDITEM_CHANGEPILOTFACE_NOTIFY:        return "T_FC_ITEM_USE_CARDITEM_CHANGEPILOTFACE_NOTIFY";
    case T_FC_ITEM_USE_INFLUENCE_BUFF:                    return "T_FC_ITEM_USE_INFLUENCE_BUFF";
    case T_FC_ITEM_USE_INFLUENCE_BUFF_OK:                return "T_FC_ITEM_USE_INFLUENCE_BUFF_OK";
    case T_FC_ITEM_USE_INFLUENCE_GAMEEVENT:                return "T_FC_ITEM_USE_INFLUENCE_GAMEEVENT";
    case T_FC_ITEM_USE_INFLUENCE_GAMEEVENT_OK:            return "T_FC_ITEM_USE_INFLUENCE_GAMEEVENT_OK";
    case T_FC_ITEM_USE_RANDOMBOX:                        return "T_FC_ITEM_USE_RANDOMBOX";
    case T_FC_ITEM_USE_RANDOMBOX_OK:                    return "T_FC_ITEM_USE_RANDOMBOX_OK";
    case T_FC_ITEM_USE_SKILL_SUPPORT_ITEM:                return "T_FC_ITEM_USE_SKILL_SUPPORT_ITEM";
    case T_FC_ITEM_USE_SKILL_SUPPORT_ITEM_OK:            return "T_FC_ITEM_USE_SKILL_SUPPORT_ITEM_OK";
    case T_FC_ITEM_USE_RANDOMBOX_OK_DONE:                return "T_FC_ITEM_USE_RANDOMBOX_OK_DONE";            // 2008-08-26 by cmkwon, ItemAttribute 추가 - 패키지(Package) 아이템
    case T_FC_ITEM_USE_LUCKY_ITEM:                        return "T_FC_ITEM_USE_LUCKY_ITEM";                    // C->F, 2008-11-04 by dhjin, 럭키머신
    case T_FC_ITEM_USE_LUCKY_ITEM_OK:                    return "T_FC_ITEM_USE_LUCKY_ITEM_OK";                // F->C, 2008-11-04 by dhjin, 럭키머신
    case T_FC_ITEM_CHANGED_SHAPEITEMNUM:                return "T_FC_ITEM_CHANGED_SHAPEITEMNUM";            // 2009-08-26 by cmkwon, 그래픽 리소스 변경 시스템 구현 - 
    case T_FC_ITEM_CHANGED_EFFECTITEMNUM:                return "T_FC_ITEM_CHANGED_EFFECTITEMNUM";            // 2009-08-26 by cmkwon, 그래픽 리소스 변경 시스템 구현 - 
    case T_FC_ITEM_USE_PET_SOCKET_ITEM:                    return "T_FC_ITEM_USE_PET_SOCKET_ITEM";                // C->F, // 2011-09-20 by hskim, 파트너 시스템 2차 - 소켓 아이템 사용 (토글)
    case T_FC_ITEM_USE_CARDITEM_CHANGE_NATION_REQUEST:    return "T_FC_ITEM_USE_CARDITEM_CHANGE_NATION_REQUEST";    // 2015-12-17 Future, Nation Change Card
    case T_FC_ITEM_USE_CARDITEM_CHANGE_NATION_CONFIRM:    return "T_FC_ITEM_USE_CARDITEM_CHANGE_NATION_CONFIRM";    // 2015-12-17 Future, Nation Change Card


    case T_FC_ITEM_USE_PET_SOCKET_ITEM_OK:                return "T_FC_ITEM_USE_PET_SOCKET_ITEM_OK";            // F->C, // 2011-09-20 by hskim, 파트너 시스템 2차 - 소켓 아이템 사용 결과 (토글)
    case T_FC_ITEM_CANCEL_PET_SOCKET_ITEM:                return "T_FC_ITEM_CANCEL_PET_SOCKET_ITEM";            // C->F, // 2011-09-20 by hskim, 파트너 시스템 2차 - 소켓 아이템 사용 중지 (토글)
    case T_FC_ITEM_CANCEL_PET_SOCKET_ITEM_OK:            return "T_FC_ITEM_CANCEL_PET_SOCKET_ITEM_OK";        // F->C, // 2011-09-20 by hskim, 파트너 시스템 2차 - 소켓 아이템 사용 중지 결과 (토글)
    case T_FC_ITEM_NOTIFY_WINDOW_POSITION:                return "T_FC_ITEM_NOTIFY_WINDOW_POSITION";            // F->C, // 2011-09-20 by hskim, 파트너 시스템 2차

    case T_FC_SHOP_PUT_ITEM_HEADER:                        return "T_FC_SHOP_PUT_ITEM_HEADER";
    case T_FC_SHOP_PUT_ITEM:                            return "T_FC_SHOP_PUT_ITEM";
    case T_FC_SHOP_PUT_ITEM_DONE:                        return "T_FC_SHOP_PUT_ITEM_DONE";
    case T_FC_SHOP_GET_ITEMINFO:                        return "T_FC_SHOP_GET_ITEMINFO";
    case T_FC_SHOP_GET_ITEMINFO_OK:                        return "T_FC_SHOP_GET_ITEMINFO_OK";
    case T_FC_SHOP_BUY_ITEM:                            return "T_FC_SHOP_BUY_ITEM";
    case T_FC_SHOP_BUY_ITEM_UPDATE:                        return "T_FC_SHOP_BUY_ITEM_UPDATE";                    // F->C, // 2011-12-26 by hskim, EP4 [트리거 시스템] - 크리스탈 시스템
    case T_FC_SHOP_BUY_ITEM_OK:                            return "T_FC_SHOP_BUY_ITEM_OK";
    case T_FC_SHOP_SELL_ITEM:                            return "T_FC_SHOP_SELL_ITEM";
    case T_FC_SHOP_SELL_ITEM_OK:                        return "T_FC_SHOP_SELL_ITEM_OK";
    case T_FC_SHOP_GET_USED_ITEM_PRICE:                    return "T_FC_SHOP_GET_USED_ITEM_PRICE";
    case T_FC_SHOP_GET_USED_ITEM_PRICE_OK:                return "T_FC_SHOP_GET_USED_ITEM_PRICE_OK";
    case T_FC_SHOP_GET_SHOP_ITEM_LIST:                    return "T_FC_SHOP_GET_SHOP_ITEM_LIST";
    case T_FC_SHOP_REQUEST_REPAIR:                        return "T_FC_SHOP_REQUEST_REPAIR";
    case T_FC_SHOP_REQUEST_REPAIR_OK:                    return "T_FC_SHOP_REQUEST_REPAIR_OK";
    case T_FC_SHOP_BUY_CASH_ITEM:                        return "T_FC_SHOP_BUY_CASH_ITEM";
    case T_FC_SHOP_BUY_CASH_ITEM_OK:                    return "T_FC_SHOP_BUY_CASH_ITEM_OK";            // 2007-11-13 by cmkwon, 선물하기 기능 추가 -
    case T_FC_SHOP_BUY_COLOR_ITEM:                        return "T_FC_SHOP_BUY_COLOR_ITEM";
    case T_FC_SHOP_BUY_COLOR_ITEM_OK:                    return "T_FC_SHOP_BUY_COLOR_ITEM_OK";
    case T_FC_SHOP_BUY_WARPOINT_ITEM_OK:                return "T_FC_SHOP_BUY_WARPOINT_ITEM_OK";        // 2007-06-13 by dhjin
    case T_FC_SHOP_BUY_WARPOINT_ITEM:                    return "T_FC_SHOP_BUY_WARPOINT_ITEM";            // 2007-06-13 by dhjin    

    case T_FC_SHOP_CLOSE:                                return "T_FC_SHOP_CLOSE";                        // 2011-10-28 by hskim, EP4 [트리거 시스템] - 크리스탈 시스템
    
    case T_FC_SHOP_INFLUENCE_BUFF:                        return "T_FC_SHOP_INFLUENCE_BUFF";                // 2013-05-09 by hskim, 세력 포인트 개선
    case T_FC_SHOP_INFLUENCE_BUFF_OK:                    return "T_FC_SHOP_INFLUENCE_BUFF_OK";            // 2013-05-09 by hskim, 세력 포인트 개선

    case T_FC_SHOP_CHECK_GIVE_TARGET:                    return "T_FC_SHOP_CHECK_GIVE_TARGET";            // 2007-11-13 by cmkwon, 선물하기 기능 추가 -
    case T_FC_SHOP_CHECK_GIVE_TARGET_OK:                return "T_FC_SHOP_CHECK_GIVE_TARGET_OK";        // 2007-11-13 by cmkwon, 선물하기 기능 추가 -

    case T_FC_TRADE_REQUEST_TRADE:                        return "T_FC_TRADE_REQUEST_TRADE";
    case T_FC_TRADE_REQUEST_TRADE_OK:                    return "T_FC_TRADE_REQUEST_TRADE_OK";
    case T_FC_TRADE_CANCEL_REQUEST:                        return "T_FC_TRADE_CANCEL_REQUEST";
    case T_FC_TRADE_CANCEL_REQUEST_OK:                    return "T_FC_TRADE_CANCEL_REQUEST_OK";
    case T_FC_TRADE_ACCEPT_TRADE:                        return "T_FC_TRADE_ACCEPT_TRADE";
    case T_FC_TRADE_ACCEPT_TRADE_OK:                    return "T_FC_TRADE_ACCEPT_TRADE_OK";
    case T_FC_TRADE_REJECT_TRADE:                        return "T_FC_TRADE_REJECT_TRADE";
    case T_FC_TRADE_REJECT_TRADE_OK:                    return "T_FC_TRADE_REJECT_TRADE_OK";
    case T_FC_TRADE_REJECT_TRADING:                        return "T_FC_TRADE_REJECT_TRADING";
    case T_FC_TRADE_SHOW_TRADE_WINDOW:                    return "T_FC_TRADE_SHOW_TRADE_WINDOW";
    case T_FC_TRADE_TRANS_ITEM:                            return "T_FC_TRADE_TRANS_ITEM";
    case T_FC_TRADE_TRANS_ITEM_OK:                        return "T_FC_TRADE_TRANS_ITEM_OK";
    case T_FC_TRADE_SEE_ITEM:                            return "T_FC_TRADE_SEE_ITEM";
    case T_FC_TRADE_SEE_ITEM_OK:                        return "T_FC_TRADE_SEE_ITEM_OK";
    case T_FC_TRADE_OK_TRADE:                            return "T_FC_TRADE_OK_TRADE";
    case T_FC_TRADE_OK_TRADE_OK:                        return "T_FC_TRADE_OK_TRADE_OK";
    case T_FC_TRADE_CANCEL_TRADE:                        return "T_FC_TRADE_CANCEL_TRADE";
    case T_FC_TRADE_CANCEL_TRADE_OK:                    return "T_FC_TRADE_CANCEL_TRADE_OK";
    case T_FC_TRADE_INSERT_ITEM:                        return "T_FC_TRADE_INSERT_ITEM";
    case T_FC_TRADE_DELETE_ITEM:                        return "T_FC_TRADE_DELETE_ITEM";
    case T_FC_TRADE_UPDATE_ITEM_COUNT:                    return "T_FC_TRADE_UPDATE_ITEM_COUNT";
    case T_FC_TRADE_OK_TRADE_NOTIFY:                    return "T_FC_TRADE_OK_TRADE_NOTIFY";        // 2008-11-21 by cmkwon, 거래 승인 확인 시스템 구현 - 
    case T_FC_TRADE_SEE_PET_DATA:                        return "T_FC_TRADE_SEE_PET_DATA";            // 2010-06-15 by shcho&hslee 펫시스템 - 거래 시 펫 정보 전송
    case T_FC_TRADE_DELETE_CURRENT_PET_DATA:            return "T_FC_TRADE_DELETE_CURRENT_PET_DATA";
    case T_FC_TRADE_INSERT_CURRENT_PET_DATA:            return "T_FC_TRADE_INSERT_CURRENT_PET_DATA";

    case T_FC_COUNTDOWN_START:                            return "T_FC_COUNTDOWN_START";
    case T_FC_COUNTDOWN_DONE:                            return "T_FC_COUNTDOWN_DONE";
    case T_FC_OBJECT_CHANGE_BODYCONDITION:                return "T_FC_OBJECT_CHANGE_BODYCONDITION";
    case T_FC_OBJECT_CHANGE_BODYCONDITION_OK:            return "T_FC_OBJECT_CHANGE_BODYCONDITION_OK";
    case T_FC_OBJECT_CHANGE                    :            return "T_FC_OBJECT_CHANGE";                // 2011-11-07 by hskim, EP4 [트리거 시스템] - 크리스탈 시스템
    case T_FC_AUCTION_REGISTER_ITEM:                    return "T_FC_AUCTION_REGISTER_ITEM";
    case T_FC_AUCTION_REGISTER_ITEM_OK:                    return "T_FC_AUCTION_REGISTER_ITEM_OK";
    case T_FC_AUCTION_CANCEL_REGISTER:                    return "T_FC_AUCTION_CANCEL_REGISTER";
    case T_FC_AUCTION_CANCEL_REGISTER_OK:                return "T_FC_AUCTION_CANCEL_REGISTER_OK";
    case T_FC_AUCTION_BUY_ITEM:                            return "T_FC_AUCTION_BUY_ITEM";
    case T_FC_AUCTION_BUY_ITEM_OK:                        return "T_FC_AUCTION_BUY_ITEM_OK";
    case T_FC_AUCTION_GET_ITEM_LIST:                    return "T_FC_AUCTION_GET_ITEM_LIST";
    case T_FC_AUCTION_INSERT_ITEM:                        return "T_FC_AUCTION_INSERT_ITEM";
    case T_FC_AUCTION_PUT_ENCHANT:                        return "T_FC_AUCTION_PUT_ENCHANT";
    case T_FC_GUILD_GET_MAP_OWNER_INFO:                    return "T_FC_GUILD_GET_MAP_OWNER_INFO";
    case T_FC_GUILD_GET_MAP_OWNER_INFO_OK:                return "T_FC_GUILD_GET_MAP_OWNER_INFO_OK";
    case T_FC_GUILD_REQUEST_GUILD_WAR:                    return "T_FC_GUILD_REQUEST_GUILD_WAR";
    case T_FC_GUILD_REQUEST_GUILD_WAR_RESULT:            return "T_FC_GUILD_REQUEST_GUILD_WAR_RESULT";
    case T_FC_GUILD_GET_CHALLENGER_GUILD:                return "T_FC_GUILD_GET_CHALLENGER_GUILD";
    case T_FC_GUILD_GET_CHALLENGER_GUILD_OK:            return "T_FC_GUILD_GET_CHALLENGER_GUILD_OK";
    case T_FC_GUILD_GET_WAR_INFO:                        return "T_FC_GUILD_GET_WAR_INFO";
    case T_FC_GUILD_GET_WAR_INFO_OK:                    return "T_FC_GUILD_GET_WAR_INFO_OK";
    case T_FC_GUILD_SUMMON_MEMBER:                        return "T_FC_GUILD_SUMMON_MEMBER";
    case T_FC_GUILD_DISMEMBER:                            return "T_FC_GUILD_DISMEMBER";

    case T_FI_GUILD_NOTIFY_START_WAR:                    return "T_FI_GUILD_NOTIFY_START_WAR";
    case T_FI_GUILD_NOTIFY_END_WAR:                        return "T_FI_GUILD_NOTIFY_END_WAR";
    case T_FI_GUILD_DELETE_GUILD:                        return "T_FI_GUILD_DELETE_GUILD";
    case T_FI_GUILD_REG_DELETE_GUILD:                    return "T_FI_GUILD_REG_DELETE_GUILD";
    case T_FI_GUILD_DISMEMBER:                            return "T_FI_GUILD_DISMEMBER";
    case T_FI_GUILD_OUTPOST:                            return "T_FI_GUILD_OUTPOST";    // F->I, // 2008-05-20 by dhjin, EP3 - 여단 수정 사항 - 전진기지 관련
    // 2012-10-12 by jhjang 해피 아워 경험치 보너스 시스템 리뉴얼
    case T_FI_GUILD_BONUS_EXP_RATE:                        return "T_FI_GUILD_BOUNUEXP_RATE";    // F->I, // 2012-10-12 by jhjang 여단 경험치 비율 전송
    // end 2012-10-12 by jhjang 해피 아워 경험치 보너스 시스템 리뉴얼

    case T_IC_GUILD_CREATE:                                return "T_IC_GUILD_CREATE";
    case T_IC_GUILD_CREATE_OK:                            return "T_IC_GUILD_CREATE_OK";
    case T_IC_GUILD_GET_GUILD_INFO:                        return "T_IC_GUILD_GET_GUILD_INFO";
    case T_IC_GUILD_GET_GUILD_INFO_OK:                    return "T_IC_GUILD_GET_GUILD_INFO_OK";
    case T_IC_GUILD_GET_OTHER_GUILD_INFO:                return "T_IC_GUILD_GET_OTHER_GUILD_INFO";
    case T_IC_GUILD_GET_OTHER_GUILD_INFO_OK:            return "T_IC_GUILD_GET_OTHER_GUILD_INFO_OK";
    case T_IC_GUILD_REQUEST_INVITE:                        return "T_IC_GUILD_REQUEST_INVITE";
    case T_IC_GUILD_REQUEST_INVITE_QUESTION:            return "T_IC_GUILD_REQUEST_INVITE_QUESTION";
    case T_IC_GUILD_ACCEPT_INVITE:                        return "T_IC_GUILD_ACCEPT_INVITE";
    case T_IC_GUILD_ACCEPT_INVITE_OK:                    return "T_IC_GUILD_ACCEPT_INVITE_OK";
    case T_IC_GUILD_REJECT_INVITE:                        return "T_IC_GUILD_REJECT_INVITE";
    case T_IC_GUILD_REJECT_INVITE_OK:                    return "T_IC_GUILD_REJECT_INVITE_OK";
    case T_IC_GUILD_BAN_MEMBER:                            return "T_IC_GUILD_BAN_MEMBER";
    case T_IC_GUILD_BAN_MEMBER_OK:                        return "T_IC_GUILD_BAN_MEMBER_OK";
    case T_IC_GUILD_LEAVE:                                return "T_IC_GUILD_LEAVE";
    case T_IC_GUILD_LEAVE_OK:                            return "T_IC_GUILD_LEAVE_OK";
//    case T_IC_GUILD_DISMEMBER:                            return "T_IC_GUILD_DISMEMBER";
    case T_IC_GUILD_DISMEMBER_OK:                        return "T_IC_GUILD_DISMEMBER_OK";
    case T_IC_GUILD_SET_MEMBER_STATE:                    return "T_IC_GUILD_SET_MEMBER_STATE";
    case T_IC_GUILD_CANCEL_DISMEMBER:                    return "T_IC_GUILD_CANCEL_DISMEMBER";
    case T_IC_GUILD_CANCEL_DISMEMBER_OK:                return "T_IC_GUILD_CANCEL_DISMEMBER_OK";
    case T_IC_GUILD_GET_DISMEMBER_DATE:                    return "T_IC_GUILD_GET_DISMEMBER_DATE";
    case T_IC_GUILD_GET_DISMEMBER_DATE_OK:                return "T_IC_GUILD_GET_DISMEMBER_DATE_OK";
    case T_IC_GUILD_CHANGE_GUILD_NAME:                    return "T_IC_GUILD_CHANGE_GUILD_NAME";
    case T_IC_GUILD_CHANGE_GUILD_NAME_OK:                return "T_IC_GUILD_CHANGE_GUILD_NAME_OK";
    case T_IC_GUILD_GET_GUILD_MARK:                        return "T_IC_GUILD_GET_GUILD_MARK";
    case T_IC_GUILD_GET_GUILD_MARK_OK:                    return "T_IC_GUILD_GET_GUILD_MARK_OK";
    case T_IC_GUILD_SET_GUILD_MARK:                        return "T_IC_GUILD_SET_GUILD_MARK";
    case T_IC_GUILD_SET_GUILD_MARK_OK:                    return "T_IC_GUILD_SET_GUILD_MARK_OK";
    case T_IC_GUILD_SET_RANK:                            return "T_IC_GUILD_SET_RANK";
    case T_IC_GUILD_SET_RANK_OK:                        return "T_IC_GUILD_SET_RANK_OK";
    case T_IC_GUILD_CHANGE_GUILD_STATE:                    return "T_IC_GUILD_CHANGE_GUILD_STATE";
    case T_IC_GUILD_LOADING_GUILD_DONE:                    return "T_IC_GUILD_LOADING_GUILD_DONE";
    case T_IC_GUILD_WAR_READY:                            return "T_IC_GUILD_WAR_READY";
    case T_IC_GUILD_START_WAR:                            return "T_IC_GUILD_START_WAR";
    case T_IC_GUILD_END_WAR:                            return "T_IC_GUILD_END_WAR";
    case T_IC_GUILD_UPDATE_WAR_POINT:                    return "T_IC_GUILD_UPDATE_WAR_POINT";
    case T_IC_GUILD_SURRENDER_GUILD_WAR:                return "T_IC_GUILD_SURRENDER_GUILD_WAR";
    case T_IC_GUILD_CHANGE_MEMBER_CAPACITY:                return "T_IC_GUILD_CHANGE_MEMBER_CAPACITY";
    case T_IC_GUILD_GET_GUILD_MEMBER_LIST_OK:            return "T_IC_GUILD_GET_GUILD_MEMBER_LIST_OK";
    case T_IC_GUILD_END_WAR_ADMIN_NOTIFY:                return "T_IC_GUILD_END_WAR_ADMIN_NOTIFY";
    case T_IC_GUILD_MEMBER_LEVEL_UP:                    return "T_IC_GUILD_MEMBER_LEVEL_UP";        // 2008-05-20 by dhjin, EP3 - 여단 수정 사항 - 여단원 레벨업 관련
    case T_IC_GUILD_NEW_COMMANDER:                        return "T_IC_GUILD_NEW_COMMANDER";        // 2008-05-20 by dhjin, EP3 - 여단 수정 사항 - 여단장 위임
    case T_IC_GUILD_NOTICE_WRITE:                        return "T_IC_GUILD_NOTICE_WRITE";        // 2008-05-20 by dhjin, EP3 - 여단 수정 사항 - 여단 공지
    case T_IC_GUILD_NOTICE_WRITE_OK:                    return "T_IC_GUILD_NOTICE_WRITE_OK";        // 2008-05-20 by dhjin, EP3 - 여단 수정 사항 - 여단 공지
    case T_IC_GUILD_GET_APPLICANT:                        return "T_IC_GUILD_GET_APPLICANT";                // C->I, // 2008-05-27 by dhjin, EP3 - 여단 수정 사항 - 여단 지원자 관리
    case T_IC_GUILD_GET_APPLICANT_OK_HEADER:            return "T_IC_GUILD_GET_APPLICANT_OK_HEADER";    // I->C, // 2008-05-27 by dhjin, EP3 - 여단 수정 사항 - 여단 지원자 관리 OK
    case T_IC_GUILD_GET_APPLICANT_OK:                    return "T_IC_GUILD_GET_APPLICANT_OK";            // I->C, // 2008-05-27 by dhjin, EP3 - 여단 수정 사항 - 여단 지원자 관리 OK
    case T_IC_GUILD_GET_APPLICANT_OK_DONE:                return "T_IC_GUILD_GET_APPLICANT_OK_DONE";        // I->C, // 2008-05-27 by dhjin, EP3 - 여단 수정 사항 - 여단 지원자 관리 OK
    case T_IC_GUILD_GET_INTRODUCTION:                    return "T_IC_GUILD_GET_INTRODUCTION";            // C->I, // 2008-05-27 by dhjin, EP3 - 여단 수정 사항 - 여단 소개
    case T_IC_GUILD_GET_INTRODUCTION_OK:                return "T_IC_GUILD_GET_INTRODUCTION_OK";        // I->C, // 2008-05-27 by dhjin, EP3 - 여단 수정 사항 - 여단 소개 ok
    case T_IC_GUILD_GET_SELF_INTRODUCTION:                return "T_IC_GUILD_GET_SELF_INTRODUCTION";        // C->I, // 2008-05-27 by dhjin, EP3 - 여단 수정 사항 - 여단 지원자 소개서 
    case T_IC_GUILD_GET_SELF_INTRODUCTION_OK:            return "T_IC_GUILD_GET_SELF_INTRODUCTION_OK";    // I->C, // 2008-05-27 by dhjin, EP3 - 여단 수정 사항 - 여단 지원자 소개서 OK
    case T_IC_GUILD_SEARCH_INTRODUCTION:                return "T_IC_GUILD_SEARCH_INTRODUCTION";            // C->I, // 2008-05-27 by dhjin, EP3 - 여단 수정 사항 - 여단 소개 검색 
    case T_IC_GUILD_SEARCH_INTRODUCTION_OK_HEADER:        return "T_IC_GUILD_SEARCH_INTRODUCTION_OK_HEADER";    // I->C, // 2008-05-27 by dhjin, EP3 - 여단 수정 사항 - 여단 소개 검색 OK
    case T_IC_GUILD_SEARCH_INTRODUCTION_OK:                return "T_IC_GUILD_SEARCH_INTRODUCTION_OK";            // I->C, // 2008-05-27 by dhjin, EP3 - 여단 수정 사항 - 여단 소개 검색 OK
    case T_IC_GUILD_SEARCH_INTRODUCTION_OK_DONE:        return "T_IC_GUILD_SEARCH_INTRODUCTION_OK_DONE";        // I->C, // 2008-05-27 by dhjin, EP3 - 여단 수정 사항 - 여단 소개 검색 OK
    case T_IC_GUILD_UPDATE_INTRODUCTION:                return "T_IC_GUILD_UPDATE_INTRODUCTION";            // C->I, // 2008-05-27 by dhjin, EP3 - 여단 수정 사항 - 여단 소개 작성 
    case T_IC_GUILD_UPDATE_INTRODUCTION_OK:                return "T_IC_GUILD_UPDATE_INTRODUCTION_OK";            // I->C, // 2008-05-27 by dhjin, EP3 - 여단 수정 사항 - 여단 소개 작성 OK
    case T_IC_GUILD_DELETE_INTRODUCTION:                return "T_IC_GUILD_DELETE_INTRODUCTION";            // C->I, // 2008-05-27 by dhjin, EP3 - 여단 수정 사항 - 여단 소개 지우기  
    case T_IC_GUILD_DELETE_INTRODUCTION_OK:                return "T_IC_GUILD_DELETE_INTRODUCTION_OK";            // I->C, // 2008-05-27 by dhjin, EP3 - 여단 수정 사항 - 여단 소개 지우기 OK
    case T_IC_GUILD_UPDATE_SELFINTRODUCTION:            return "T_IC_GUILD_UPDATE_SELFINTRODUCTION";        // C->I, // 2008-05-27 by dhjin, EP3 - 여단 수정 사항 - 자기 소개 작성 
    case T_IC_GUILD_UPDATE_SELFINTRODUCTION_OK:            return "T_IC_GUILD_UPDATE_SELFINTRODUCTION_OK";        // I->C, // 2008-05-27 by dhjin, EP3 - 여단 수정 사항 - 자기 소개 작성 OK
    case T_IC_GUILD_DELETE_SELFINTRODUCTION:            return "T_IC_GUILD_DELETE_SELFINTRODUCTION";        // C->I, // 2008-05-27 by dhjin, EP3 - 여단 수정 사항 - 자기 소개 지우기  
    case T_IC_GUILD_DELETE_SELFINTRODUCTION_OK:            return "T_IC_GUILD_DELETE_SELFINTRODUCTION_OK";        // I->C, // 2008-05-27 by dhjin, EP3 - 여단 수정 사항 - 자기 소개 지우기 OK
    case T_IC_GUILD_CHANGE_FAME_RANK:                    return "T_IC_GUILD_CHANGE_FAME_RANK";                // I->C, // 2008-06-10 by dhjin, EP3 - 여단 수정 사항 - 여단 명성 변경
    case T_IC_GUILD_APPLICANT_INVITE:                    return "T_IC_GUILD_APPLICANT_INVITE";                // C->I, // 2008-06-12 by dhjin, EP3 - 여단 수정 사항 - 지원자 관리에서 여단원 초대
    case T_IC_GUILD_APPLICANT_INVITE_OK:                return "T_IC_GUILD_APPLICANT_INVITE_OK";            // I->C, // 2008-06-12 by dhjin, EP3 - 여단 수정 사항 - 지원자 관리에서 여단원 초대 OK
    case T_IC_GUILD_APPLICANT_REJECT_INVITE:            return "T_IC_GUILD_APPLICANT_REJECT_INVITE";        // C->I, // 2008-06-12 by dhjin, EP3 - 여단 수정 사항 - 지원자 관리에서 여단원 초대 거부 
    case T_IC_GUILD_APPLICANT_REJECT_INVITE_OK:            return "T_IC_GUILD_APPLICANT_REJECT_INVITE_OK";        // I->C, // 2008-06-12 by dhjin, EP3 - 여단 수정 사항 - 지원자 관리에서 여단원 초대 거부 OK
    case T_IC_GUILD_CHANGE_MEMBERSHIP:                    return "T_IC_GUILD_CHANGE_MEMBERSHIP";                // I->C, // 2008-06-20 by dhjin, EP3 - 여단 수정 사항 - 여단장 맴버쉽 정보 전송

    case T_FC_SKILL_USE_SKILLPOINT:                        return "T_FC_SKILL_USE_SKILLPOINT";
    case T_FC_SKILL_USE_SKILLPOINT_OK:                    return "T_FC_SKILL_USE_SKILLPOINT_OK";
    case T_FC_SKILL_SETUP_SKILL:                        return "T_FC_SKILL_SETUP_SKILL";
    case T_FC_SKILL_SETUP_SKILL_OK_HEADER:                return "T_FC_SKILL_SETUP_SKILL_OK_HEADER";
    case T_FC_SKILL_SETUP_SKILL_OK:                        return "T_FC_SKILL_SETUP_SKILL_OK";
    case T_FC_SKILL_SETUP_SKILL_OK_DONE:                return "T_FC_SKILL_SETUP_SKILL_OK_DONE";
    case T_FC_SKILL_USE_SKILL:                            return "T_FC_SKILL_USE_SKILL";
    case T_FC_SKILL_USE_SKILL_OK:                        return "T_FC_SKILL_USE_SKILL_OK";
    case T_FC_SKILL_CANCEL_SKILL:                        return "T_FC_SKILL_CANCEL_SKILL";
    case T_FC_SKILL_CANCEL_SKILL_OK:                    return "T_FC_SKILL_CANCEL_SKILL_OK";
    case T_FC_SKILL_INVALIDATE_SKILL:                    return "T_FC_SKILL_INVALIDATE_SKILL";
    case T_FC_SKILL_PREPARE_USE:                        return "T_FC_SKILL_PREPARE_USE";
    case T_FC_SKILL_PREPARE_USE_OK:                        return "T_FC_SKILL_PREPARE_USE_OK";
    case T_FC_SKILL_CANCEL_PREPARE:                        return "T_FC_SKILL_CANCEL_PREPARE";
    case T_FC_SKILL_CANCEL_PREPARE_OK:                    return "T_FC_SKILL_CANCEL_PREPARE_OK";
    case T_FC_SKILL_CONFIRM_USE:                        return "T_FC_SKILL_CONFIRM_USE";
    case T_FC_SKILL_CONFIRM_USE_ACK:                    return "T_FC_SKILL_CONFIRM_USE_ACK";

    case T_FN_SKILL_USE_SKILL:                            return "T_FN_SKILL_USE_SKILL";
    case T_FN_SKILL_USE_SKILL_OK:                        return "T_FN_SKILL_USE_SKILL_OK";
    case T_FC_QUEST_REQUEST_START:                        return "T_FC_QUEST_REQUEST_START";
    case T_FC_QUEST_REQUEST_START_RESULT:                return "T_FC_QUEST_REQUEST_START_RESULT";
    case T_FC_QUEST_ACCEPT_QUEST:                        return "T_FC_QUEST_ACCEPT_QUEST";
    case T_FC_QUEST_CANCEL_QUEST:                        return "T_FC_QUEST_CANCEL_QUEST";
    case T_FC_QUEST_REQUEST_SUCCESS:                    return "T_FC_QUEST_REQUEST_SUCCESS";
    case T_FC_QUEST_REQUEST_SUCCESS_RESULT:                return "T_FC_QUEST_REQUEST_SUCCESS_RESULT";
    case T_FC_QUEST_PUT_ALL_QUEST_HEADER:                return "T_FC_QUEST_PUT_ALL_QUEST_HEADER";
    case T_FC_QUEST_PUT_ALL_QUEST:                        return "T_FC_QUEST_PUT_ALL_QUEST";
    case T_FC_QUEST_PUT_ALL_QUEST_DONE:                    return "T_FC_QUEST_PUT_ALL_QUEST_DONE";
    case T_FC_QUEST_DISCARD_QUEST:                        return "T_FC_QUEST_DISCARD_QUEST";
    case T_FC_QUEST_DISCARD_QUEST_OK:                    return "T_FC_QUEST_DISCARD_QUEST_OK";
    case T_FC_QUEST_MOVE_QUEST_MAP:                        return "T_FC_QUEST_MOVE_QUEST_MAP";
    case T_FC_QUEST_PUT_ALL_QUEST_MONSTER_COUNT_HEADER:    return "T_FC_QUEST_PUT_ALL_QUEST_MONSTER_COUNT_HEADER";
    case T_FC_QUEST_PUT_ALL_QUEST_MONSTER_COUNT:        return "T_FC_QUEST_PUT_ALL_QUEST_MONSTER_COUNT";
    case T_FC_QUEST_PUT_ALL_QUEST_MONSTER_COUNT_DONE:    return "T_FC_QUEST_PUT_ALL_QUEST_MONSTER_COUNT_DONE";
    case T_FC_QUEST_UPDATE_MONSTER_COUNT:                return "T_FC_QUEST_UPDATE_MONSTER_COUNT";
    case T_FC_QUEST_REQUEST_SUCCESS_CHECK:                return "T_FC_QUEST_REQUEST_SUCCESS_CHECK";
    case T_FC_QUEST_REQUEST_SUCCESS_CHECK_RESULT:        return "T_FC_QUEST_REQUEST_SUCCESS_CHECK_RESULT";
    case T_FC_QUEST_REQUEST_PARTY_WARP:                    return "T_FC_QUEST_REQUEST_PARTY_WARP";                // 2006-10-16 by cmkwon
    case T_FC_QUEST_REQUEST_PARTY_WARP_ACK:                return "T_FC_QUEST_REQUEST_PARTY_WARP_ACK";            // 2006-10-16 by cmkwon

    case T_FC_INFO_GET_MONSTER_INFO:                    return "T_FC_INFO_GET_MONSTER_INFO";
    case T_FC_INFO_GET_MONSTER_INFO_OK:                    return "T_FC_INFO_GET_MONSTER_INFO_OK";
    case T_FC_INFO_GET_MAPOBJECT_INFO:                    return "T_FC_INFO_GET_MAPOBJECT_INFO";
    case T_FC_INFO_GET_MAPOBJECT_INFO_OK:                return "T_FC_INFO_GET_MAPOBJECT_INFO_OK";
    case T_FC_INFO_GET_ITEM_INFO:                        return "T_FC_INFO_GET_ITEM_INFO";
    case T_FC_INFO_GET_ITEM_INFO_OK:                    return "T_FC_INFO_GET_ITEM_INFO_OK";
    case T_FC_INFO_GET_RARE_ITEM_INFO:                    return "T_FC_INFO_GET_RARE_ITEM_INFO";
    case T_FC_INFO_GET_RARE_ITEM_INFO_OK:                return "T_FC_INFO_GET_RARE_ITEM_INFO_OK";
    case T_FC_INFO_GET_BUILDINGNPC_INFO:                return "T_FC_INFO_GET_BUILDINGNPC_INFO";
    case T_FC_INFO_GET_BUILDINGNPC_INFO_OK:                return "T_FC_INFO_GET_BUILDINGNPC_INFO_OK";
    case T_FC_INFO_GET_SIMPLE_ITEM_INFO:                return "T_FC_INFO_GET_SIMPLE_ITEM_INFO";
    case T_FC_INFO_GET_SIMPLE_ITEM_INFO_OK:                return "T_FC_INFO_GET_SIMPLE_ITEM_INFO_OK";
    case T_FC_INFO_GET_ENCHANT_COST:                    return "T_FC_INFO_GET_ENCHANT_COST";
    case T_FC_INFO_GET_ENCHANT_COST_OK:                    return "T_FC_INFO_GET_ENCHANT_COST_OK";
    case T_FC_INFO_GET_CURRENT_MAP_INFO:                return "T_FC_INFO_GET_CURRENT_MAP_INFO";
    case T_FC_INFO_GET_CURRENT_MAP_INFO_OK:                return "T_FC_INFO_GET_CURRENT_MAP_INFO_OK";
    case T_FC_INFO_GET_HAPPY_HOUR_EVENT_INFO_OK:        return "T_FC_INFO_GET_HAPPY_HOUR_EVENT_INFO_OK";
    case T_FC_INFO_GET_GAME_EVENT_INFO_OK:                return "T_FC_INFO_GET_GAME_EVENT_INFO_OK";
    case T_FC_INFO_GET_SERVER_DATE_TIME:                return "T_FC_INFO_GET_SERVER_DATE_TIME";
    case T_FC_INFO_GET_SERVER_DATE_TIME_OK:                return "T_FC_INFO_GET_SERVER_DATE_TIME_OK";
    case T_FC_INFO_GET_HAPPY_HOUR_EVENT_INFO:            return "T_FC_INFO_GET_HAPPY_HOUR_EVENT_INFO";    // 2007-10-30 by cmkwon, 세력별 해피아워 이벤트 구현 - C->F
    case T_FC_INFO_GET_HAPPY_HOUR_EVENT_INFO_BY_LEVEL:    return "T_FC_INFO_GET_HAPPY_HOUR_EVENT_INFO_BY_LEVEL";    // 2008-03-14 by dhjin, Level별 해피아워 이벤트 구현 -
    case T_FC_INFO_CHECK_RESOBJ_CHECKSUM:                return "T_FC_INFO_CHECK_RESOBJ_CHECKSUM";        // 2007-05-28 by cmkwon

    case T_FC_REQUEST_REQUEST:                            return "T_FC_REQUEST_REQUEST";
    case T_FC_REQUEST_REQUEST_OK:                        return "T_FC_REQUEST_REQUEST_OK";
    case T_FC_REQUEST_ACCEPT_REQUEST:                    return "T_FC_REQUEST_ACCEPT_REQUEST";
    case T_FC_REQUEST_ACCEPT_REQUEST_OK:                return "T_FC_REQUEST_ACCEPT_REQUEST_OK";
    case T_FC_REQUEST_REJECT_REQUEST:                    return "T_FC_REQUEST_REJECT_REQUEST";
    case T_FC_REQUEST_REJECT_REQUEST_OK:                return "T_FC_REQUEST_REJECT_REQUEST_OK";
    case T_FC_REQUEST_CANCEL_REQUEST:                    return "T_FC_REQUEST_CANCEL_REQUEST";
    
    case T_FC_CITY_GET_BUILDING_LIST:                                return "T_FC_CITY_GET_BUILDING_LIST";
    case T_FC_CITY_GET_BUILDING_LIST_OK:                            return "T_FC_CITY_GET_BUILDING_LIST_OK";
    case T_FC_CITY_REQUEST_ENTER_BUILDING:                            return "T_FC_CITY_REQUEST_ENTER_BUILDING";
    case T_FC_CITY_REQUEST_ENTER_BUILDING_OK:                        return "T_FC_CITY_REQUEST_ENTER_BUILDING_OK";
    case T_FC_CITY_CHECK_WARP_STATE:                                return "T_FC_CITY_CHECK_WARP_STATE";
    case T_FC_CITY_CHECK_WARP_STATE_OK:                                return "T_FC_CITY_CHECK_WARP_STATE_OK";
    case T_FC_CITY_POLL_REQUEST_LEADER_CANDIDATE_LIST:                return "T_FC_CITY_POLL_REQUEST_LEADER_CANDIDATE_LIST";
    case T_FC_CITY_POLL_REQUEST_LEADER_CANDIDATE_LIST_OK_HEADER:    return "T_FC_CITY_POLL_REQUEST_LEADER_CANDIDATE_LIST_OK_HEADER";
    case T_FC_CITY_POLL_REQUEST_LEADER_CANDIDATE_LIST_OK:            return "T_FC_CITY_POLL_REQUEST_LEADER_CANDIDATE_LIST_OK";
    case T_FC_CITY_POLL_REQUEST_LEADER_CANDIDATE_LIST_OK_DONE:        return "T_FC_CITY_POLL_REQUEST_LEADER_CANDIDATE_LIST_OK_DONE";
    case T_FC_CITY_POLL_REQUEST_LEADER_CANDIDATE_INFO:                return "T_FC_CITY_POLL_REQUEST_LEADER_CANDIDATE_INFO";
    case T_FC_CITY_POLL_REQUEST_LEADER_CANDIDATE_INFO_OK:            return "T_FC_CITY_POLL_REQUEST_LEADER_CANDIDATE_INFO_OK";
    case T_FC_CITY_POLL_REQUEST_LEADER_CANDIDATE_INFO_OK_GUILDMARK:    return "T_FC_CITY_POLL_REQUEST_LEADER_CANDIDATE_INFO_OK_GUILDMARK";
    case T_FC_CITY_POLL_REG_LEADER_CANDIDATE:                        return "T_FC_CITY_POLL_REG_LEADER_CANDIDATE";
    case T_FC_CITY_POLL_REG_LEADER_CANDIDATE_OK:                    return "T_FC_CITY_POLL_REG_LEADER_CANDIDATE_OK";
    case T_FC_CITY_POLL_DELETE_LEADER_CANDIDATE:                    return "T_FC_CITY_POLL_DELETE_LEADER_CANDIDATE";
    case T_FC_CITY_POLL_DELETE_LEADER_CANDIDATE_OK:                    return "T_FC_CITY_POLL_DELETE_LEADER_CANDIDATE_OK";
    case T_FC_CITY_POLL_VOTE:                                        return "T_FC_CITY_POLL_VOTE";
    case T_FC_CITY_POLL_VOTE_OK:                                    return "T_FC_CITY_POLL_VOTE_OK";
    case T_FC_CITY_POLL_REQUEST_POLL_DATE:                            return "T_FC_CITY_POLL_REQUEST_POLL_DATE";
    case T_FC_CITY_POLL_REQUEST_POLL_DATE_OK:                        return "T_FC_CITY_POLL_REQUEST_POLL_DATE_OK";
    case T_FC_CITY_POLL_LEADER_ELECTION_INFO:                        return "T_FC_CITY_POLL_LEADER_ELECTION_INFO";
    case T_FC_CITY_WARINFO_INFLUENCE:                                return "T_FC_CITY_WARINFO_INFLUENCE";
    case T_FC_CITY_WARINFO_INFLUENCE_OK:                            return "T_FC_CITY_WARINFO_INFLUENCE_OK";
    case T_FC_CITY_WARINFO_OUTPOST:                                    return "T_FC_CITY_WARINFO_OUTPOST";
    case T_FC_CITY_WARINFO_OUTPOST_OK:                                return "T_FC_CITY_WARINFO_OUTPOST_OK";


    case T_FC_TIMER_START_TIMER:                        return "T_FC_TIMER_START_TIMER";
    case T_FC_TIMER_STOP_TIMER:                            return "T_FC_TIMER_STOP_TIMER";
    case T_FC_TIMER_UPDATE_TIMER:                        return "T_FC_TIMER_UPDATE_TIMER";
    case T_FC_TIMER_PAUSE_TIMER:                        return "T_FC_TIMER_PAUSE_TIMER";
    case T_FC_TIMER_CONTINUE_TIMER:                        return "T_FC_TIMER_CONTINUE_TIMER";
    case T_FC_TIMER_TIMEOUT:                            return "T_FC_TIMER_TIMEOUT";
    case T_FC_CLIENT_REPORT:                            return "T_FC_CLIENT_REPORT";
    case T_FC_STRING_128:                                return "T_FC_STRING_128";
    case T_IC_STRING_128:                                return "T_IC_STRING_128";
    case T_FI_ADMIN_GET_CHARACTER_INFO:                    return "T_FI_ADMIN_GET_CHARACTER_INFO";
    case T_FI_ADMIN_GET_CHARACTER_INFO_OK:                return "T_FI_ADMIN_GET_CHARACTER_INFO_OK";
    case T_FI_ADMIN_CALL_CHARACTER:                        return "T_FI_ADMIN_CALL_CHARACTER";
    case T_FI_ADMIN_MOVETO_CHARACTER:                    return "T_FI_ADMIN_MOVETO_CHARACTER";
    case T_FI_ADMIN_PRINT_DEBUG_MSG:                    return "T_FI_ADMIN_PRINT_DEBUG_MSG";
    case T_FI_ADMIN_CHANGE_WEATHER:                        return "T_FI_ADMIN_CHANGE_WEATHER";
    case T_IC_ADMIN_CALL_CHARACTER:                        return "T_IC_ADMIN_CALL_CHARACTER";
    case T_IC_ADMIN_GET_SERVER_STAT:                    return "T_IC_ADMIN_GET_SERVER_STAT";
    case T_IC_ADMIN_GET_SERVER_STAT_OK:                    return "T_IC_ADMIN_GET_SERVER_STAT_OK";
    case T_IC_ADMIN_CALL_GUILD:                            return "T_IC_ADMIN_CALL_GUILD";
    case T_IC_ADMIN_CALLGM_INFO_OK:                        return "T_IC_ADMIN_CALLGM_INFO_OK";
    case T_IC_ADMIN_CALLGM_VIEW_OK:                        return "T_IC_ADMIN_CALLGM_VIEW_OK";
    case T_IC_ADMIN_CALLGM_BRING_OK:                    return "T_IC_ADMIN_CALLGM_BRING_OK";

    case T_FC_ADMIN_GET_SERVER_STAT:                    return "T_FC_ADMIN_GET_SERVER_STAT";
    case T_FC_ADMIN_GET_SERVER_STAT_OK:                    return "T_FC_ADMIN_GET_SERVER_STAT_OK";
        
    case T_IC_COUNTDOWN_START:                            return "T_IC_COUNTDOWN_START";
    case T_IC_COUNTDOWN_DONE:                            return "T_IC_COUNTDOWN_DONE";

    //case T_IC_VOIP_SET:                                    return "T_IC_VOIP_SET";                    // C -> I, 2008-06-17 by dhjin, EP3 VOIP - 설정 
    //case T_IC_VOIP_SET_OK:                                return "T_IC_VOIP_SET_OK";                // I -> C, 2008-06-17 by dhjin, EP3 VOIP - 설정 정보 전송

    // T0_IC_CHATROOM
    case T_IC_CHATROOM_CREATE:                            return "T_IC_CHATROOM_CREATE";                    // C -> I, 2008-06-16 by dhjin, EP3 채팅방 - 채팅방 생성
    case T_IC_CHATROOM_CREATE_OK:                        return "T_IC_CHATROOM_CREATE_OK";                // I -> C, 2008-06-16 by dhjin, EP3 채팅방 - 채팅방 생성 OK
    case T_IC_CHATROOM_LIST_INFO:                        return "T_IC_CHATROOM_LIST_INFO";                // C -> I, 2008-06-16 by dhjin, EP3 채팅방 - 채팅방 목록 가져오기
    case T_IC_CHATROOM_LIST_INFO_OK:                    return "T_IC_CHATROOM_LIST_INFO_OK";            // I -> C, 2008-06-16 by dhjin, EP3 채팅방 - 채팅방 목록 가져오기 OK
    case T_IC_CHATROOM_REQUEST_INVITE:                    return "T_IC_CHATROOM_REQUEST_INVITE";            // C -> I, 2008-06-16 by dhjin, EP3 채팅방 - 채팅방 초대 요청 
    case T_IC_CHATROOM_REQUEST_INVITE_QUESTION:            return "T_IC_CHATROOM_REQUEST_INVITE_QUESTION";    // I -> C, 2008-06-16 by dhjin, EP3 채팅방 - 채팅방 초대 대상에게 전송
    case T_IC_CHATROOM_JOIN:                            return "T_IC_CHATROOM_JOIN";                    // C -> I, 2008-06-16 by dhjin, EP3 채팅방 - 채팅방 참여
    case T_IC_CHATROOM_JOIN_OK:                            return "T_IC_CHATROOM_JOIN_OK";                    // I -> C, 2008-06-16 by dhjin, EP3 채팅방 - 채팅방 참여 OK
    case T_IC_CHATROOM_ACCEPT_INVITE:                    return "T_IC_CHATROOM_ACCEPT_INVITE";            // C -> I, 2008-06-16 by dhjin, EP3 채팅방 - 채팅방 초대 수락
    case T_IC_CHATROOM_ACCEPT_INVITE_OK:                return "T_IC_CHATROOM_ACCEPT_INVITE_OK";        // I -> C, 2008-06-16 by dhjin, EP3 채팅방 - 채팅방 초대 수락 OK
    case T_IC_CHATROOM_REJECT_INVITE:                    return "T_IC_CHATROOM_REJECT_INVITE";            // C -> I, 2008-06-16 by dhjin, EP3 채팅방 - 채팅방 초대 거절
    case T_IC_CHATROOM_REJECT_INVITE_OK:                return "T_IC_CHATROOM_REJECT_INVITE_OK";        // I -> C, 2008-06-16 by dhjin, EP3 채팅방 - 채팅방 초대 거절 OK
    case T_IC_CHATROOM_LEAVE:                            return "T_IC_CHATROOM_LEAVE";                    // C -> I, 2008-06-16 by dhjin, EP3 채팅방 - 채팅방 나가기
    case T_IC_CHATROOM_LEAVE_OK:                        return "T_IC_CHATROOM_LEAVE_OK";                // I -> C, 2008-06-16 by dhjin, EP3 채팅방 - 채팅방 나가기 OK
    case T_IC_CHATROOM_BAN:                                return "T_IC_CHATROOM_BAN";                        // C -> I, 2008-06-16 by dhjin, EP3 채팅방 - 채팅방 추방
    case T_IC_CHATROOM_BAN_OK:                            return "T_IC_CHATROOM_BAN_OK";                    // I -> C, 2008-06-16 by dhjin, EP3 채팅방 - 채팅방 추방 OK
    case T_IC_CHATROOM_CHANGE_NAME:                        return "T_IC_CHATROOM_CHANGE_NAME";                // C -> I, 2008-06-16 by dhjin, EP3 채팅방 - 채팅방 이름 변경
    case T_IC_CHATROOM_CHANGE_NAME_OK:                    return "T_IC_CHATROOM_CHANGE_NAME_OK";            // I -> C, 2008-06-16 by dhjin, EP3 채팅방 - 채팅방 이름 변경 OK
    case T_IC_CHATROOM_CHANGE_MASTER:                    return "T_IC_CHATROOM_CHANGE_MASTER";            // C -> I, 2008-06-16 by dhjin, EP3 채팅방 - 채팅방 방장 변경
    case T_IC_CHATROOM_CHANGE_MASTER_OK:                return "T_IC_CHATROOM_CHANGE_MASTER_OK";        // I -> C, 2008-06-16 by dhjin, EP3 채팅방 - 채팅방 방장 변경 OK
    case T_IC_CHATROOM_CHANGE_LOCK_PW:                    return "T_IC_CHATROOM_CHANGE_LOCK_PW";            // C -> I, 2008-06-16 by dhjin, EP3 채팅방 - 채팅방 비밀번호 변경
    case T_IC_CHATROOM_CHANGE_LOCK_PW_OK:                return "T_IC_CHATROOM_CHANGE_LOCK_PW_OK";        // I -> C, 2008-06-16 by dhjin, EP3 채팅방 - 채팅방 비밀번호 변경 OK
    case T_IC_CHATROOM_CHANGE_MAX_MEMBER:                return "T_IC_CHATROOM_CHANGE_MAX_MEMBER";        // C -> I, 2008-06-16 by dhjin, EP3 채팅방 - 채팅방 인원수 변경
    case T_IC_CHATROOM_CHANGE_MAX_MEMBER_OK:            return "T_IC_CHATROOM_CHANGE_MAX_MEMBER_OK";    // I -> C, 2008-06-16 by dhjin, EP3 채팅방 - 채팅방 인원수 변경 OK
    case T_IC_CHATROOM_MEMBER_INFO:                        return "T_IC_CHATROOM_MEMBER_INFO";                // I -> C, 2008-06-25 by dhjin, EP3 채팅방 - 채팅방 맴버 정보 전송
    case T_IC_CHATROOM_MEMBER_INFO_OK:                    return "T_IC_CHATROOM_MEMBER_INFO_OK";            // I -> C, 2008-06-25 by dhjin, EP3 채팅방 - 채팅방 맴버 정보 전송 OK
    case T_IC_CHATROOM_OTHER_MEMBER_INFO:                return "T_IC_CHATROOM_OTHER_MEMBER_INFO";        // I -> C, 2008-06-25 by dhjin, EP3 채팅방 - 채팅방 다른 맴버 정보 전송
    case T_IC_CHATROOM_OTHER_MEMBER_INFO_OK:            return "T_IC_CHATROOM_OTHER_MEMBER_INFO_OK";    // I -> C, 2008-06-25 by dhjin, EP3 채팅방 - 채팅방 다른 맴버 정보 전송 OK
        
    case T_FC_CITYWAR_START_WAR:                        return "T_FC_CITYWAR_START_WAR";
    case T_FC_CITYWAR_MONSTER_CREATED:                    return "T_FC_CITYWAR_MONSTER_CREATED";
    case T_FC_CITYWAR_MONSTER_DEAD:                        return "T_FC_CITYWAR_MONSTER_DEAD";
    case T_FC_CITYWAR_END_WAR:                            return "T_FC_CITYWAR_END_WAR";
    case T_FC_CITYWAR_GET_OCCUPYINFO:                    return "T_FC_CITYWAR_GET_OCCUPYINFO";
    case T_FC_CITYWAR_GET_OCCUPYINFO_OK:                return "T_FC_CITYWAR_GET_OCCUPYINFO_OK";
    case T_FC_CITYWAR_SET_SETTINGTIME:                    return "T_FC_CITYWAR_SET_SETTINGTIME";
    case T_FC_CITYWAR_SET_TEX:                            return "T_FC_CITYWAR_SET_TEX";
    case T_FC_CITYWAR_SET_BRIEFING:                        return "T_FC_CITYWAR_SET_BRIEFING";
    case T_FC_CITYWAR_BRING_SUMOFTEX:                    return "T_FC_CITYWAR_BRING_SUMOFTEX";
    case T_FC_CITYWAR_BRING_SUMOFTEX_OK:                return "T_FC_CITYWAR_BRING_SUMOFTEX_OK";

    case T_FC_WAR_NOTIFY_INVASION:                        return "T_FC_WAR_NOTIFY_INVASION";
    case T_FC_WAR_NOTIFY_INFLUENCE_MONSTER_DEAD:        return "T_FC_WAR_NOTIFY_INFLUENCE_MONSTER_DEAD";
    case T_FC_WAR_NOTIFY_INFLUENCE_MONSTER_INVASION:    return "T_FC_WAR_NOTIFY_INFLUENCE_MONSTER_INVASION";
    case T_FC_WAR_NOTIFY_INFLUENCE_MONSTER_AUTO_DESTROYED:    return "T_FC_WAR_NOTIFY_INFLUENCE_MONSTER_AUTO_DESTROYED";
    case T_FC_WAR_BOSS_MONSTER_SUMMON_DATA:                return "T_FC_WAR_BOSS_MONSTER_SUMMON_DATA";
    case T_FC_WAR_JACO_MONSTER_SUMMON:                    return "T_FC_WAR_JACO_MONSTER_SUMMON";
    case T_FC_WAR_STRATEGYPOINT_MONSTER_SUMMON:            return "T_FC_WAR_STRATEGYPOINT_MONSTER_SUMMON";

    case T_FC_WAR_SIGN_BOARD_INSERT_STRING:                return "T_FC_WAR_SIGN_BOARD_INSERT_STRING";
    case T_FC_WAR_SIGN_BOARD_DELETE_STRING:                return "T_FC_WAR_SIGN_BOARD_DELETE_STRING";
    case T_FC_WAR_REQ_SIGN_BOARD_STRING_LIST:            return "T_FC_WAR_REQ_SIGN_BOARD_STRING_LIST";
    case T_FC_WAR_REQ_SIGN_BOARD_STRING_LIST_OK:        return "T_FC_WAR_REQ_SIGN_BOARD_STRING_LIST_OK";
    case T_FC_WAR_UPDATE_CONTRIBUTION_POINT_OK:            return "T_FC_WAR_UPDATE_CONTRIBUTION_POINT_OK";
    case T_FC_WAR_INFLUENCE_DATA:                        return "T_FC_WAR_INFLUENCE_DATA";
    case T_FC_WAR_MONSTER_CREATED:                        return "T_FC_WAR_MONSTER_CREATED";
    case T_FC_WAR_MONSTER_AUTO_DESTROYED:                return "T_FC_WAR_MONSTER_AUTO_DESTROYED";
    case T_FC_WAR_MONSTER_DEAD:                            return "T_FC_WAR_MONSTER_DEAD";
    case T_FC_WAR_BOSS_CONTRIBUTION_GUILD:                return "T_FC_WAR_BOSS_CONTRIBUTION_GUILD";    // 2008-12-29 by dhjin, 전쟁 보상 추가안, F->C(n)
    case T_FC_WAR_INFLUENCE_CONSECUTIVE_VICTORITES:        return "T_FC_WAR_INFLUENCE_CONSECUTIVE_VICTORITES";        // 2013-05-09 by hskim, 세력 포인트 개선
    case T_FC_WAR_INFLUENCE_CONSECUTIVE_POINT:            return "T_FC_WAR_INFLUENCE_CONSECUTIVE_POINT";            // 2013-08-01 by jhseol, 역전의 버프 리뉴얼 - 세력포인트 정보 보내주기

    case T_FC_BAZAAR_CUSTOMER_INFO_OK:                    return "T_FC_BAZAAR_CUSTOMER_INFO_OK";
    case T_FC_BAZAAR_INFO_OK                :            return "T_FC_BAZAAR_INFO_OK";
    case T_FC_BAZAAR_SELL_PUT_ITEM            :            return "T_FC_BAZAAR_SELL_PUT_ITEM";
    case T_FC_BAZAAR_SELL_PUT_ITEM_OK        :            return "T_FC_BAZAAR_SELL_PUT_ITEM_OK";
    case T_FC_BAZAAR_SELL_CANCEL_ITEM        :            return "T_FC_BAZAAR_SELL_CANCEL_ITEM";
    case T_FC_BAZAAR_SELL_CANCEL_ITEM_OK    :            return "T_FC_BAZAAR_SELL_CANCEL_ITEM_OK";
    case T_FC_BAZAAR_SELL_START                :            return "T_FC_BAZAAR_SELL_START";
    case T_FC_BAZAAR_SELL_START_OK            :            return "T_FC_BAZAAR_SELL_START_OK";
    case T_FC_BAZAAR_SELL_REQUEST_ITEMLIST    :            return "T_FC_BAZAAR_SELL_REQUEST_ITEMLIST";
    case T_FC_BAZAAR_SELL_REQUEST_ITEMLIST_OK:            return "T_FC_BAZAAR_SELL_REQUEST_ITEMLIST_OK";
    case T_FC_BAZAAR_SELL_ITEM_ENCHANTLIST_OK:            return "T_FC_BAZAAR_SELL_ITEM_ENCHANTLIST_OK";
    case T_FC_BAZAAR_SELL_BUY_ITEM            :            return "T_FC_BAZAAR_SELL_BUY_ITEM";
    case T_FC_BAZAAR_SELL_BUY_ITEM_OK        :            return "T_FC_BAZAAR_SELL_BUY_ITEM_OK";

    case T_FC_BAZAAR_BUY_PUT_ITEM            :            return "T_FC_BAZAAR_BUY_PUT_ITEM";
    case T_FC_BAZAAR_BUY_PUT_ITEM_OK            :        return "T_FC_BAZAAR_BUY_PUT_ITEM_OK";
    case T_FC_BAZAAR_BUY_CANCEL_ITEM            :        return "T_FC_BAZAAR_BUY_CANCEL_ITEM";
    case T_FC_BAZAAR_BUY_CANCEL_ITEM_OK        :            return "T_FC_BAZAAR_BUY_CANCEL_ITEM_OK";
    case T_FC_BAZAAR_BUY_START                :            return "T_FC_BAZAAR_BUY_START";
    case T_FC_BAZAAR_BUY_START_OK            :            return "T_FC_BAZAAR_BUY_START_OK";
    case T_FC_BAZAAR_BUY_REQUEST_ITEMLIST    :            return "T_FC_BAZAAR_BUY_REQUEST_ITEMLIST";
    case T_FC_BAZAAR_BUY_REQUEST_ITEMLIST_OK    :        return "T_FC_BAZAAR_BUY_REQUEST_ITEMLIST_OK";
    case T_FC_BAZAAR_BUY_SELL_ITEM            :            return "T_FC_BAZAAR_BUY_SELL_ITEM";
    case T_FC_BAZAAR_BUY_SELL_ITEM_OK        :            return "T_FC_BAZAAR_BUY_SELL_ITEM_OK";
    case T_FI_GUILD_RANK_OK        :                        return "T_FI_GUILD_RANK_OK";
    case T_FI_GUILD_DELETE_INFO_OK        :                return "T_FI_GUILD_DELETE_INFO_OK";


    case T_PRE_IOCP:                                    return "T_PRE_IOCP";
    case T_PRE_DB:                                        return "T_PRE_DB";
    case T_IM_IOCP:                                        return "T_IM_IOCP";
    case T_IM_DB:                                        return "T_IM_DB";
    case T_FIELD_IOCP:                                    return "T_FIELD_IOCP";
    case T_FIELD_DB:                                    return "T_FIELD_DB";
    case T_NPC_IOCP:                                    return "T_NPC_IOCP";
    case T_NPC_DB:                                        return "T_NPC_DB";
    case T_TIMER:                                        return "T_TIMER";
    case T_DB:                                            return "T_DB";
    case T_NA:                                            return "T_NA";
    case T_ERROR:                                        return "T_ERROR";
    case T_FC_RACING_RACINGLIST_REQUEST:                return "T_FC_RACING_RACINGLIST_REQUEST";
    case T_FC_RACING_RACINGLIST_REQUEST_ACK:            return "T_FC_RACING_RACINGLIST_REQUEST_ACK";
    case T_FC_RACING_RACINGINFO_REQUEST:                return "T_FC_RACING_RACINGINFO_REQUEST";
    case T_FC_RACING_RACINGINFO_REQUEST_ACK:            return "T_FC_RACING_RACINGINFO_REQUEST_ACK";
    case T_FC_RACING_RACINGNOTIFY:                        return "T_FC_RACING_RACINGNOTIFY";
    case T_FC_RACING_JOIN_ENABLE:                        return "T_FC_RACING_JOIN_ENABLE";
    case T_FC_RACING_JOIN_REQUEST:                        return "T_FC_RACING_JOIN_REQUEST";
    case T_FC_RACING_JOIN_REQUEST_ACK:                    return "T_FC_RACING_JOIN_REQUEST_ACK";
    case T_FC_RACING_COUNTDOWN:                            return "T_FC_RACING_COUNTDOWN";
    case T_FC_RACING_CHECKPOINT_CHECK:                    return "T_FC_RACING_CHECKPOINT_CHECK";
    case T_FC_RACING_CHECKPOINT_CHECK_ACK:                return "T_FC_RACING_CHECKPOINT_CHECK_ACK";
    case T_FC_RACING_FINISHED:                            return "T_FC_RACING_FINISHED";
    case T_FC_RACING_OTHER_FINISHED:                    return "T_FC_RACING_OTHER_FINISHED";
    case T_FC_RACING_FINALRANKING:                        return "T_FC_RACING_FINALRANKING";
    case T_FL_LOG_LOGIN:                                return "T_FL_LOG_LOGIN";
    case T_FL_LOG_LOGOUT:                                return "T_FL_LOG_LOGOUT";
    case T_FL_LOG_LEVEL:                                return "T_FL_LOG_LEVEL";
    case T_FL_LOG_EXP:                                    return "T_FL_LOG_EXP";
    case T_FL_LOG_STAT:                                    return "T_FL_LOG_STAT";
    case T_FL_LOG_GAME_START:                            return "T_FL_LOG_GAME_START";
    case T_FL_LOG_GAME_END:                                return "T_FL_LOG_GAME_END";
    case T_FL_LOG_QUEST_COMPLETION:                        return "T_FL_LOG_QUEST_COMPLETION";
    case T_FL_LOG_QUEST_COMPENSATION:                    return "T_FL_LOG_QUEST_COMPENSATION";
    case T_FL_LOG_PKWIN:                                return "T_FL_LOG_PKWIN";
    case T_FL_LOG_PKLOSS:                                return "T_FL_LOG_PKLOSS";
    case T_FL_LOG_DEAD:                                    return "T_FL_LOG_DEAD";
    case T_FL_LOG_WARP:                                    return "T_FL_LOG_WARP";
    case T_FL_LOG_SPEED_HACK:                            return "T_FL_LOG_SPEED_HACK";
    case T_FL_LOG_CREATE_CHARACTER:                        return "T_FL_LOG_CREATE_CHARACTER";
    case T_FL_LOG_DELETE_CHARACTER:                        return "T_FL_LOG_DELETE_CHARACTER";
    case T_FL_LOG_MEMORY_HACK:                            return "T_FL_LOG_MEMORY_HACK";
    case T_FL_LOG_PvEWIN:                                return "T_FL_LOG_PvEWIN";
    case T_FL_LOG_PvELOSS:                                return "T_FL_LOG_PvELOSS";

    // 2012-12-17 by bckim, 길드창고 아이템 이동 관련 추가로그 
    case T_FL_LOG_ITEM_CHARACTER_ITEM_GUILDSTORE_RESULT:                    return "T_FL_LOG_ITEM_CHARACTER_ITEM_GUILDSTORE_RESULT";
    // 2012-12-17 by bckim, 길드창고 아이템 이동 관련 추가로그.End 
    case T_FL_LOG_ITEM_CHARACTER_ITEM:                    return "T_FL_LOG_ITEM_CHARACTER_ITEM";
    case T_FL_LOG_ITEM_GET_ITEM:                        return "T_FL_LOG_ITEM_GET_ITEM";
    case T_FL_LOG_ITEM_TRADE_SEND:                        return "T_FL_LOG_ITEM_TRADE_SEND";
    case T_FL_LOG_ITEM_TRADE_RECV:                        return "T_FL_LOG_ITEM_TRADE_RECV";
    case T_FL_LOG_ITEM_THROWAWAY_ITEM:                    return "T_FL_LOG_ITEM_THROWAWAY_ITEM";
    case T_FL_LOG_ITEM_BUY_ITEM:                        return "T_FL_LOG_ITEM_BUY_ITEM";
    case T_FL_LOG_ITEM_SELL_ITEM:                        return "T_FL_LOG_ITEM_SELL_ITEM";
    case T_FL_LOG_ITEM_USE_ITEM:                        return "T_FL_LOG_ITEM_USE_ITEM";
    case T_FL_LOG_ITEM_USE_ENERGY:                        return "T_FL_LOG_ITEM_USE_ENERGY";
    case T_FL_LOG_ITEM_USE_ENCHANT:                        return "T_FL_LOG_ITEM_USE_ENCHANT";
    case T_FL_LOG_ITEM_ADD_ITEM_BY_ADMIN:                return "T_FL_LOG_ITEM_ADD_ITEM_BY_ADMIN";
    case T_FL_LOG_ITEM_DEL_ITEM_BY_ADMIN:                return "T_FL_LOG_ITEM_DEL_ITEM_BY_ADMIN";
    case T_FL_LOG_ITEM_ADD_ITEM_BY_COMMAND:                return "T_FL_LOG_ITEM_ADD_ITEM_BY_COMMAND";
    case T_FL_LOG_ITEM_INSERT_TO_STORE:                    return "T_FL_LOG_ITEM_INSERT_TO_STORE";
    case T_FL_LOG_ITEM_DRAW_FROM_STORE:                    return "T_FL_LOG_ITEM_DRAW_FROM_STORE";
    case T_FL_LOG_ITEM_STORE_ITEM:                        return "T_FL_LOG_ITEM_STORE_ITEM";
    case T_FL_LOG_ITEM_USE_MIX:                            return "T_FL_LOG_ITEM_USE_MIX";
    case T_FL_LOG_ITEM_USE_CHANGE_CHARACTER_NAME_ORIGIN:    return "T_FL_LOG_ITEM_USE_CHANGE_CHARACTER_NAME_ORIGIN";
    case T_FL_LOG_ITEM_BAZAAR_SEND:                        return "T_FL_LOG_ITEM_BAZAAR_SEND";
    case T_FL_LOG_ITEM_BAZAAR_RECV:                        return "T_FL_LOG_ITEM_BAZAAR_RECV";
    case T_FL_LOG_ITEM_INSERT_TO_GUILDSTORE:            return "T_FL_LOG_ITEM_INSERT_TO_GUILDSTORE";
    case T_FL_LOG_ITEM_DRAW_FROM_GUILDSTORE:            return "T_FL_LOG_ITEM_DRAW_FROM_GUILDSTORE";
    case T_FL_LOG_ITEM_SPI:                                return "T_FL_LOG_ITEM_SPI";            // 2006-10-27 by cmkwon
    case T_FL_LOG_ITEM_GET_ITEM_BY_ITEMEVENT:            return "T_FL_LOG_ITEM_GET_ITEM_BY_ITEMEVENT";    // 2007-01-11 by cmkwon
    case T_FL_LOG_WARPOINT:                                return "T_FL_LOG_WARPOINT";                        // 2007-05-15 by dhjin

    case T_FL_LOG_ARENA_TEAM_MATCH:                        return "T_FL_LOG_ARENA_TEAM_MATCH";                // 2007-06-11 by dhjin
    case T_FL_LOG_TUTORIAL_COMPLETION:                    return "T_FL_LOG_TUTORIAL_COMPLETION";                // 2007-07-06 by dhjin
    case T_FL_LOG_OUTPOST_START:                        return "T_FL_LOG_OUTPOST_START";                // 2007-10-02 by dhjin
    case T_FL_LOG_OUTPOST_END:                            return "T_FL_LOG_OUTPOST_END";                // 2007-10-02 by dhjin
    case T_FL_LOG_EXPEDIENCYFUND:                        return "T_FL_LOG_EXPEDIENCYFUND";                // 2007-10-02 by dhjin

    case T_FL_LOG_SERVER_INFO_MAP:                        return "T_FL_LOG_SERVER_INFO_MAP";
    case T_FL_LOG_SERVER_INFO_TOTAL:                    return "T_FL_LOG_SERVER_INFO_TOTAL";
    case T_FL_LOG_HACKING_LOG:                            return "T_FL_LOG_HACKING_LOG";

    case T_FL_LOG_MONSTER_BOSS:                            return "T_FL_LOG_MONSTER_BOSS";
    case T_FL_LOG_MS_WAR:                                return "T_FL_LOG_MS_WAR";            // 2008-04-01 by dhjin, 모선전, 거점전 정보창 기획안 - 
    case T_FL_LOG_SP_WAR:                                return "T_FL_LOG_SP_WAR";            // 2008-04-01 by dhjin, 모선전, 거점전 정보창 기획안 - 
    case T_FL_LOG_WAR_CONTRIBUTION:                        return "T_FL_LOG_WAR_CONTRIBUTION";            // 2008-12-23 by dhjin, 전쟁 보상 추가안
    case T_FL_LOG_WAR_CONTRIBUTION_GEAR:                return "T_FL_LOG_WAR_CONTRIBUTION_GEAR";            // 2008-12-23 by dhjin, 전쟁 보상 추가안

    case T_FL_LOG_INFLWAR_START:                        return "T_FL_LOG_INFLWAR_START";                    // 2007-10-16 by cmkwon, 로그 추가 -
    case T_FL_LOG_INFLWAR_END:                            return "T_FL_LOG_INFLWAR_END";                        // 2007-10-16 by cmkwon, 로그 추가 -
    case T_FL_LOG_OUTPOSTWAR_RESET_START:                return "T_FL_LOG_OUTPOSTWAR_RESET_START";            // 2007-10-16 by cmkwon, 로그 추가 -
    case T_FL_LOG_OUTPOSTWAR_RESET_DESTROY:                return "T_FL_LOG_OUTPOSTWAR_RESET_DESTROY";            // 2007-10-16 by cmkwon, 로그 추가 -
    case T_FL_LOG_OUTPOSTWAR_RESET_COMPLETE:            return "T_FL_LOG_OUTPOSTWAR_RESET_COMPLETE";        // 2007-10-16 by cmkwon, 로그 추가 -
    case T_FL_LOG_OUTPOSTWAR_PROTECTOR_DESTROY:            return "T_FL_LOG_OUTPOSTWAR_PROTECTOR_DESTROY";        // 2007-10-16 by cmkwon, 로그 추가 -
    case T_FL_LOG_OUTPOSTWAR_SET_NEXTTIME:                return "T_FL_LOG_OUTPOSTWAR_SET_NEXTTIME";            // 2007-10-16 by cmkwon, 로그 추가 -

    case T_FL_LOG_POLL_VOTE:                            return "T_FL_LOG_POLL_VOTE";                        // 2007-11-08 by dhjin
    case T_FL_LOG_POLL_DELETE_LEADERCANDIDATE:            return "T_FL_LOG_POLL_DELETE_LEADERCANDIDATE";        // 2007-11-08 by dhjin

    case T_FL_LOG_DISMEMBER_GUILD:                        return "T_FL_LOG_DISMEMBER_GUILD";        // 2007-11-08 by dhjin
    case T_FL_LOG_NOTIFY_MSG_DELETE:                    return "T_FL_LOG_NOTIFY_MSG_DELETE";    // 2008-01-23 by cmkwon
    case T_FL_LOG_USE_COUPON:                            return "T_FL_LOG_USE_COUPON";            // 2008-01-23 by cmkwon, S_F, S_L: 쿠폰 사용 게임 로그에 추가 - 

    case T_FL_LOG_ITEM_ATTACH_ITEM:                        return "T_FL_LOG_ITEM_ATTACH_ITEM";        // 2008-01-23 by cmkwon, S_F, S_L: 장착/장착해제 게임 로그에 추가 - 
    case T_FL_LOG_ITEM_DETACH_ITEM:                        return "T_FL_LOG_ITEM_DETACH_ITEM";        // 2008-01-23 by cmkwon, S_F, S_L: 장착/장착해제 게임 로그에 추가 - 
    case T_FL_LOG_ITEM_DELETE:                            return "T_FL_LOG_ITEM_DELETE";            // 2008-01-23 by cmkwon, S_F, S_L: 장착/장착해제 게임 로그에 추가 - 

    case T_FL_LOG_LUCKY_DROP:                            return "T_FL_LOG_LUCKY_DROP";            // 2009-03-31 by dhjin, 럭키머신 로그
    case T_FL_LOG_INFINITY_START:                        return "T_FL_LOG_INFINITY_START";        // 193 - // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 게임로그 추가, 시작 관련 정보 
    case T_FL_LOG_INFINITY_START_ITEM:                    return "T_FL_LOG_INFINITY_START_ITEM";    // 194 - // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 게임로그 추가, 시작시 아이템 복사 정보
    case T_FL_LOG_INFINITY_CINEMA:                        return "T_FL_LOG_INFINITY_CINEMA";        // 195 - // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 게임로그 추가, 연출 로그
    case T_FL_LOG_MONSTER_SKILL:                        return "T_FL_LOG_MONSTER_SKILL";        // 196 - // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 게임로그 추가, 몬스터 스킬 사용시
    case T_FL_LOG_HPACTION_TALK:                        return "T_FL_LOG_HPACTION_TALK";        // 197 - // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 게임로그 추가, 몬스터 대화 사용시
    case T_FL_LOG_INFINITY_FIN:                            return "T_FL_LOG_INFINITY_FIN";            // 198 - // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 게임로그 추가, 종료 관련 정보 
    case T_FL_LOG_INFINITY_FIN_ITEM:                    return "T_FL_LOG_INFINITY_FIN_ITEM";    // 199 - // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 게임로그 추가, 종료시 아이템 복사 정보
    case T_FL_LOG_INFINITY_FIN_ALIVE_KEY_MONSTER:        return "T_FL_LOG_INFINITY_FIN_ALIVE_KEY_MONSTER";    // 200 - // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 게임로그 추가, 종료시 살아있는 몬스터 정보
    case T_FL_LOG_INFINITY_LEAVE_ITEM:                    return "T_FL_LOG_INFINITY_LEAVE_ITEM";    // 201 - // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 게임로그 추가, 인피 탈퇴시 아이템 복사 정보
    
    case T_FL_LOG_FIXED_TERM_SHAPE_START:                return "T_FL_LOG_FIXED_TERM_SHAPE_START";        // 217 - // 2012-10-10 by hskim, 기간제 속성 구현 (기간제 외형)
    case T_FL_LOG_FIXED_TERM_SHAPE_END:                    return "T_FL_LOG_FIXED_TERM_SHAPE_END";            // 218 - // 2012-10-10 by hskim, 기간제 속성 구현 (기간제 외형)

    case T_FL_LOG_START_FIELD_SERVER:                    return "T_FL_LOG_START_FIELD_SERVER";
    
    case T_FM_GET_NUM_CLIENTS:                            return "T_FM_GET_NUM_CLIENTS";
    case T_FM_GET_NUM_CLIENTS_OK:                        return "T_FM_GET_NUM_CLIENTS_OK";
    case T_FM_SHUTDOWN:                                    return "T_FM_SHUTDOWN";
    case T_FM_SHUTDOWN_OK:                                return "T_FM_SHUTDOWN_OK";
    case T_FM_PING:                                        return "T_FM_PING";
    case T_FM_PING_OK:                                    return "T_FM_PING_OK";
    case T_FM_PRINTMAP:                                    return "T_FM_PRINTMAP";
    case T_FM_SAVE_MAPBLOCK_INFO:                        return "T_FM_SAVE_MAPBLOCK_INFO";
    case T_FM_PRINT_DEBUG_MSG:                            return "T_FM_PRINT_DEBUG_MSG";
    case T_FM_SEND_CHANNEL_INFO:                        return "T_FM_SEND_CHANNEL_INFO";
    case T_FM_CHANGE_BANDWIDTH_FLAG:                    return "T_FM_CHANGE_BANDWIDTH_FLAG";
    case T_FM_CHANGE_BANDWIDTH_FLAG_OK:                    return "T_FM_CHANGE_BANDWIDTH_FLAG_OK";
    case T_FM_SET_MSG_PRINT_LEVEL:                        return "T_FM_SET_MSG_PRINT_LEVEL";
    case T_FM_GET_CHARACTER_INFO:                        return "T_FM_GET_CHARACTER_INFO";
    case T_FM_GET_CHARACTER_INFO_OK:                    return "T_FM_GET_CHARACTER_INFO_OK";
    case T_FM_GET_MAP_USER_COUNTS:                        return "T_FM_GET_MAP_USER_COUNTS";
    case T_FM_GET_MAP_USER_COUNTS_ACK:                    return "T_FM_GET_MAP_USER_COUNTS_ACK";
    case T_FM_RELOAD_ITEM_INFO:                            return "T_FM_RELOAD_ITEM_INFO";
    case T_FM_RELOAD_ITEM_INFO_OK:                        return "T_FM_RELOAD_ITEM_INFO_OK";
    case T_FM_RESET_NPC_SERVER:                            return "T_FM_RESET_NPC_SERVER";
    case T_FM_RESET_NPC_SERVER_OK:                        return "T_FM_RESET_NPC_SERVER_OK";
    case T_FM_SERVER_THREAD_STATUS:                        return "T_FM_SERVER_THREAD_STATUS";
    case T_FM_SET_SERVICE_STATE:                        return "T_FM_SET_SERVICE_STATE";
    case T_FM_SET_SERVICE_STATE_OK:                        return "T_FM_SET_SERVICE_STATE_OK";
    case T_FM_RELOAD_SERVER_INFO:                        return "T_FM_RELOAD_SERVER_INFO";
    case T_FM_RELOAD_SERVER_INFO_OK:                    return "T_FM_RELOAD_SERVER_INFO_OK";
    case T_FM_INCREASE_CHANNEL:                            return "T_FM_INCREASE_CHANNEL";
    case T_FM_INCREASE_CHANNEL_OK:                        return "T_FM_INCREASE_CHANNEL_OK";
    case T_FM_SET_CHANNEL_STATE:                        return "T_FM_SET_CHANNEL_STATE";
    case T_FM_SET_CHANNEL_STATE_OK:                        return "T_FM_SET_CHANNEL_STATE_OK";
    case T_PM_GET_NUM_CLIENTS:                            return "T_PM_GET_NUM_CLIENTS";
    case T_PM_GET_NUM_CLIENTS_OK:                        return "T_PM_GET_NUM_CLIENTS_OK";
    case T_PM_SHUTDOWN:                                    return "T_PM_SHUTDOWN";
    case T_PM_SHUTDOWN_OK:                                return "T_PM_SHUTDOWN_OK";
    case T_PM_CLOSE:                                    return "T_PM_CLOSE";
    case T_PM_CLOSE_OK:                                    return "T_PM_CLOSE_OK";
    case T_PM_PING:                                        return "T_PM_PING";
    case T_PM_PING_OK:                                    return "T_PM_PING_OK";
    case T_PM_RELOAD_VERSION_INFO:                        return "T_PM_RELOAD_VERSION_INFO";
    case T_PM_RELOAD_VERSION_INFO_OK:                    return "T_PM_RELOAD_VERSION_INFO_OK";
    case T_PM_CHANGE_BANDWIDTH_FLAG:                    return "T_PM_CHANGE_BANDWIDTH_FLAG";
    case T_PM_CHANGE_BANDWIDTH_FLAG_OK:                    return "T_PM_CHANGE_BANDWIDTH_FLAG_OK";
    case T_PM_SET_MSG_PRINT_LEVEL:                        return "T_PM_SET_MSG_PRINT_LEVEL";
    case T_PM_PAUSE_SERVICE:                            return "T_PM_PAUSE_SERVICE";
    case T_PM_PAUSE_SERVICE_OK:                            return "T_PM_PAUSE_SERVICE_OK";
    case T_PM_START_SERVICE:                            return "T_PM_START_SERVICE";
    case T_PM_START_SERVICE_OK:                            return "T_PM_START_SERVICE_OK";
    case T_PM_GET_SERVER_GROUP_INFO:                    return "T_PM_GET_SERVER_GROUP_INFO";
    case T_PM_GET_SERVER_GROUP_INFO_ACK:                return "T_PM_GET_SERVER_GROUP_INFO_ACK";
    case T_PM_SET_LIMIT_GROUP_USER_COUNTS:                return "T_PM_SET_LIMIT_GROUP_USER_COUNTS";
    case T_PM_SERVER_THREAD_STATUS:                        return "T_PM_SERVER_THREAD_STATUS";
    case T_PM_RELOAD_BLOCKED_ACCOUNTS:                    return "T_PM_RELOAD_BLOCKED_ACCOUNTS";
    case T_PM_RELOAD_BLOCKED_ACCOUNTS_OK:                return "T_PM_RELOAD_BLOCKED_ACCOUNTS_OK";
    case T_PM_CHANGE_ENABLE_SERVER_GROUP:                return "T_PM_CHANGE_ENABLE_SERVER_GROUP";
    case T_PM_PREPARE_SHUTDOWN:                            return "T_PM_PREPARE_SHUTDOWN";
    case T_PM_AUTO_UPDATE_HTTP_SERVER_SETTING:            return "T_PM_AUTO_UPDATE_HTTP_SERVER_SETTING";
    case T_PM_RELOAD_VERSION_INFO_HEADER:                return "T_PM_RELOAD_VERSION_INFO_HEADER";            // 2007-01-09 by cmkwon
    case T_PM_RELOAD_VERSION_INFO_LIST:                    return "T_PM_RELOAD_VERSION_INFO_LIST";                // 2007-01-09 by cmkwon
    case T_PM_RELOAD_VERSION_INFO_DONE:                    return "T_PM_RELOAD_VERSION_INFO_DONE";                // 2007-01-09 by cmkwon

    case T_NM_GET_NUM_MONSTERS:                            return "T_NM_GET_NUM_MONSTERS";
    case T_NM_GET_NUM_MONSTERS_OK:                        return "T_NM_GET_NUM_MONSTERS_OK";
    case T_NM_SHUTDOWN:                                    return "T_NM_SHUTDOWN";
    case T_NM_SHUTDOWN_OK:                                return "T_NM_SHUTDOWN_OK";
    case T_NM_PING:                                        return "T_NM_PING";
    case T_NM_PING_OK:                                    return "T_NM_PING_OK";
    case T_NM_PRINTMAP:                                    return "T_NM_PRINTMAP";
    case T_NM_SAVE_MAPBLOCK_INFO:                        return "T_NM_SAVE_MAPBLOCK_INFO";
    case T_NM_SEND_CHANNEL_INFO:                        return "T_NM_SEND_CHANNEL_INFO";
    case T_NM_CHANGE_BANDWIDTH_FLAG:                    return "T_NM_CHANGE_BANDWIDTH_FLAG";
    case T_NM_CHANGE_BANDWIDTH_FLAG_OK:                    return "T_NM_CHANGE_BANDWIDTH_FLAG_OK";
    case T_NM_SET_MSG_PRINT_LEVEL:                        return "T_NM_SET_MSG_PRINT_LEVEL";
    case T_NM_GET_MAP_USER_COUNTS:                        return "T_NM_GET_MAP_USER_COUNTS";
    case T_NM_GET_MAP_USER_COUNTS_ACK:                    return "T_NM_GET_MAP_USER_COUNTS_ACK";
    case T_NM_SET_LIMIT_MONSTER_COUNT:                    return "T_NM_SET_LIMIT_MONSTER_COUNT";
    case T_NM_SET_LIMIT_MONSTER_COUNT_ACK:                return "T_NM_SET_LIMIT_MONSTER_COUNT_ACK";
    case T_NM_SERVER_THREAD_STATUS:                        return "T_NM_SERVER_THREAD_STATUS";
    case T_NM_SET_CHANNEL_STATE_OK:                        return "T_NM_SET_CHANNEL_STATE_OK";
    case T_IM_GET_NUM_CLIENTS:                            return "T_IM_GET_NUM_CLIENTS";
    case T_IM_GET_NUM_CLIENTS_OK:                        return "T_IM_GET_NUM_CLIENTS_OK";
    case T_IM_SHUTDOWN:                                    return "T_IM_SHUTDOWN";
    case T_IM_SHUTDOWN_OK:                                return "T_IM_SHUTDOWN_OK";
    case T_IM_PING:                                        return "T_IM_PING";
    case T_IM_PING_OK:                                    return "T_IM_PING_OK";
    case T_IM_PRINTMAP:                                    return "T_IM_PRINTMAP";
    case T_IM_PRINT_DEBUG_MSG:                            return "T_IM_PRINT_DEBUG_MSG";
    case T_IM_CHANGE_BANDWIDTH_FLAG:                    return "T_IM_CHANGE_BANDWIDTH_FLAG";
    case T_IM_CHANGE_BANDWIDTH_FLAG_OK:                    return "T_IM_CHANGE_BANDWIDTH_FLAG_OK";
    case T_IM_SET_MSG_PRINT_LEVEL:                        return "T_IM_SET_MSG_PRINT_LEVEL";
    case T_IM_GET_TOTAL_USER_COUNTS:                    return "T_IM_GET_TOTAL_USER_COUNTS";
    case T_IM_GET_TOTAL_USER_COUNTS_ACK:                return "T_IM_GET_TOTAL_USER_COUNTS_ACK";
    case T_IM_SERVER_THREAD_STATUS:                        return "T_IM_SERVER_THREAD_STATUS";
    case T_PA_ADMIN_CONNECT:                            return "T_PA_ADMIN_CONNECT";
    case T_PA_ADMIN_CONNECT_OK:                            return "T_PA_ADMIN_CONNECT_OK";
    case T_PA_ADMIN_CHANGE_SERVER_STATE:                return "T_PA_ADMIN_CHANGE_SERVER_STATE";
    case T_PA_ADMIN_GET_ACCOUNT_INFO:                    return "T_PA_ADMIN_GET_ACCOUNT_INFO";
    case T_PA_ADMIN_GET_ACCOUNT_INFO_OK:                return "T_PA_ADMIN_GET_ACCOUNT_INFO_OK";
    case T_PA_ADMIN_DISCONNECT_USER:                    return "T_PA_ADMIN_DISCONNECT_USER";
    case T_PA_ADMIN_DISCONNECT_USER_OK:                    return "T_PA_ADMIN_DISCONNECT_USER_OK";
    case T_PA_ADMIN_BLOCK_ACCOUNT:                        return "T_PA_ADMIN_BLOCK_ACCOUNT";
    case T_PA_ADMIN_BLOCK_ACCOUNT_OK:                    return "T_PA_ADMIN_BLOCK_ACCOUNT_OK";
    case T_PA_ADMIN_UNBLOCK_ACCOUNT:                    return "T_PA_ADMIN_UNBLOCK_ACCOUNT";
    case T_PA_ADMIN_UNBLOCK_ACCOUNT_OK:                    return "T_PA_ADMIN_UNBLOCK_ACCOUNT_OK";
    case T_PA_ADMIN_GET_SERVER_INFO:                    return "T_PA_ADMIN_GET_SERVER_INFO";
    case T_PA_ADMIN_GET_SERVER_INFO_OK:                    return "T_PA_ADMIN_GET_SERVER_INFO_OK";
    case T_PA_ADMIN_GET_ACCOUNT_LIST:                    return "T_PA_ADMIN_GET_ACCOUNT_LIST";
    case T_PA_ADMIN_GET_ACCOUNT_LIST_OK:                return "T_PA_ADMIN_GET_ACCOUNT_LIST_OK";
    case T_PA_ADMIN_RELOAD_HAPPYEV:                        return "T_PA_ADMIN_RELOAD_HAPPYEV";
    case T_PA_ADMIN_RELOAD_ITEMEV:                        return "T_PA_ADMIN_RELOAD_ITEMEV";          // 2006-08-31 by dhjin, ItemEvent Reload관련
    case T_PA_ADMIN_UPDATE_PCBANGLIST:                    return "T_PA_ADMIN_UPDATE_PCBANGLIST";        // 2007-01-22 by dhjin    
    case T_PA_ADMIN_UPDATE_STRATEGYPOINT_NOTSUMMONTIME:    return "T_PA_ADMIN_UPDATE_STRATEGYPOINT_NOTSUMMONTIME";        // 2007-03-06 by dhjin
    case T_PA_ADMIN_PETITION_SET_PERIOD:                return "T_PA_ADMIN_PETITION_SET_PERIOD";        // 2007-11-19 by cmkwon, 진정시스템 업데이트 - 
    case T_PA_ADMIN_SET_DBSERVER_GROUP:                    return "T_PA_ADMIN_SET_DBSERVER_GROUP";        // 2008-04-29 by cmkwon, 서버군 정보 DB에 추가(신규 계정 캐릭터 생성 제한 시스템추가) - 
    case T_PA_ADMIN_SET_DBSERVER_GROUP_OK:                return "T_PA_ADMIN_SET_DBSERVER_GROUP_OK";        // 2008-04-29 by cmkwon, 서버군 정보 DB에 추가(신규 계정 캐릭터 생성 제한 시스템추가) - 
    case T_PA_ADMIN_RELOAD_ADMIN_NOTICE_SYSTEM:            return "T_PA_ADMIN_RELOAD_ADMIN_NOTICE_SYSTEM";        // 2009-01-14 by cmkwon, 운영자 자동 공지 시스템 구현 - 
    case T_PA_ADMIN_RELOAD_WORLDRANKING:                return "T_PA_ADMIN_RELOAD_WORLDRANKING";            // 2009-02-12 by cmkwon, EP3-3 월드랭킹시스템 구현 - 
    case T_PA_ADMIN_RELOAD_INFLUENCERATE:                return "T_PA_ADMIN_RELOAD_INFLUENCERATE";            // 2009-09-16 by cmkwon, 세력 초기화시 어뷰징 방지 구현 - 
    case T_PA_ADMIN_LOG_ACCOUNT_ACCOUNTTYPE_CHANGE:            return "T_PA_ADMIN_LOG_ACCOUNT_ACCOUNTTYPE_CHANGE";            // 2012-06-07 by hskim, 관리자 계정 보안 강화 - 계정 권한 변경시 로그 남김
    case T_PA_ADMIN_LOG_CHARACTER_RACEACCOUNTTYPE_CHANGE:    return "T_PA_ADMIN_LOG_CHARACTER_RACEACCOUNTTYPE_CHANGE";    // 2012-06-07 by hskim, 관리자 계정 보안 강화 - 계정 권한 변경시 로그 남김

    case T_PA_ADMIN_STRATRGYPOINT_INFO_CHANGE:            return "T_PA_ADMIN_STRATRGYPOINT_INFO_CHANGE";        // 2012-11-13 by jhseol, 전쟁 시스템 리뉴얼 - 거점전

    case T_IA_ADMIN_CONNECT:                            return "T_IA_ADMIN_CONNECT";
    case T_IA_ADMIN_CONNECT_OK:                            return "T_IA_ADMIN_CONNECT_OK";
    case T_IA_ADMIN_CHANGE_SERVER_STATE:                return "T_IA_ADMIN_CHANGE_SERVER_STATE";
    case T_IA_ADMIN_GET_CHARACTER:                        return "T_IA_ADMIN_GET_CHARACTER";
    case T_IA_ADMIN_GET_CHARACTER_OK:                    return "T_IA_ADMIN_GET_CHARACTER_OK";
    case T_IA_ADMIN_CHAT_ALL:                            return "T_IA_ADMIN_CHAT_ALL";
    case T_IA_ADMIN_GET_SERVER_INFO:                    return "T_IA_ADMIN_GET_SERVER_INFO";
    case T_IA_ADMIN_GET_SERVER_INFO_OK:                    return "T_IA_ADMIN_GET_SERVER_INFO_OK";
    case T_FA_ADMIN_CONNECT:                            return "T_FA_ADMIN_CONNECT";
    case T_FA_ADMIN_CONNECT_OK:                            return "T_FA_ADMIN_CONNECT_OK";
    case T_FA_ADMIN_CHANGE_SERVER_STATE:                return "T_FA_ADMIN_CHANGE_SERVER_STATE";

    case T_FC_ARENA_PLAY_CHARACTER_TYPE:                return "T_FC_ARENA_PLAY_CHARACTER_TYPE";    // 2012-06-08 by jhseol, 아레나 추가개발part2 - 케릭터 : 타입 복사
    case T_FC_ARENA_REQUEST_TEAM:                        return "T_FC_ARENA_REQUEST_TEAM";
    case T_FC_ARENA_REQUEST_TEAM_OK:                    return "T_FC_ARENA_REQUEST_TEAM_OK";
    case T_FC_ARENA_CREATE_TEAM:                        return "T_FC_ARENA_CREATE_TEAM";    
    case T_FC_ARENA_CREATE_TEAM_OK:                        return "T_FC_ARENA_CREATE_TEAM_OK";
    case T_FC_ARENA_ENTER_TEAM:                            return "T_FC_ARENA_ENTER_TEAM";        
    case T_FC_ARENA_ENTER_TEAM_OK:                        return "T_FC_ARENA_ENTER_TEAM_OK";
    case T_FC_ARENA_TEAM_MEMBER_LIST:                    return "T_FC_ARENA_TEAM_MEMBER_LIST";
    case T_FC_ARENA_REAVE_TEAM:                            return "T_FC_ARENA_REAVE_TEAM";        
    case T_FC_ARENA_REAVE_TEAM_OK:                        return "T_FC_ARENA_REAVE_TEAM_OK";
    case T_FC_ARENA_TEAM_READY:                            return "T_FC_ARENA_TEAM_READY";
    case T_FC_ARENA_TEAM_READY_FINISH:                    return "T_FC_ARENA_TEAM_READY_FINISH";
    case T_FC_ARENA_TEAM_READY_FINISH_CANCEL:            return "T_FC_ARENA_TEAM_READY_FINISH_CANCEL";
    case T_FC_ARENA_TEAM_MATCHING:                        return "T_FC_ARENA_TEAM_MATCHING";
    case T_FC_ARENA_ENTER_ROOM:                            return "T_FC_ARENA_ENTER_ROOM";
    case T_FC_ARENA_ENTER_ROOM_WARP:                    return "T_FC_ARENA_ENTER_ROOM_WARP";
    case T_FC_ARENA_ROOM_WAR_START:                        return "T_FC_ARENA_ROOM_WAR_START";
    case T_FC_ARENA_ROOM_WAR_INFO:                        return "T_FC_ARENA_ROOM_WAR_INFO";
    case T_FC_ARENA_ROOM_WAR_FINISH_HEADER:                return "T_FC_ARENA_ROOM_WAR_FINISH_HEADER";
    case T_FC_ARENA_ROOM_WAR_FINISH:                    return "T_FC_ARENA_ROOM_WAR_FINISH";
    case T_FC_ARENA_ROOM_WAR_FINISH_DONE:                return "T_FC_ARENA_ROOM_WAR_FINISH_DONE";
    case T_FC_ARENA_ROOM_WAR_FINISH_DRAW:                return "T_FC_ARENA_ROOM_WAR_FINISH_DRAW";
    case T_FC_ARENA_FINISH_WARP:                        return "T_FC_ARENA_FINISH_WARP";        
    case T_FC_ARENA_FINISH_WARP_OK:                        return "T_FC_ARENA_FINISH_WARP_OK";
    case T_FC_ARENA_REQUEST_CREATE_TEAMINFO:            return "T_FC_ARENA_REQUEST_CREATE_TEAMINFO";
    case T_FC_ARENA_REQUEST_CREATE_TEAMINFO_OK:            return "T_FC_ARENA_REQUEST_CREATE_TEAMINFO_OK";
    case T_FC_ARENA_BEFORE_ROOM_WAR_FINISH:                return "T_FC_ARENA_BEFORE_ROOM_WAR_FINISH";
    case T_FC_ARENA_REQUEST_OTHER_TEAM_INFO:            return "T_FC_ARENA_REQUEST_OTHER_TEAM_INFO";
    case T_FC_ARENA_REQUEST_OTHER_TEAM_INFO_OK:            return "T_FC_ARENA_REQUEST_OTHER_TEAM_INFO_OK";
    case T_FC_ARENA_GM_COMMAND_INFO_HEADER:                return "T_FC_ARENA_GM_COMMAND_INFO_HEADER";
    case T_FC_ARENA_GM_COMMAND_INFO:                    return "T_FC_ARENA_GM_COMMAND_INFO";
    case T_FC_ARENA_GM_COMMAND_INFO_DONE:                return "T_FC_ARENA_GM_COMMAND_INFO_DONE";
    case T_FC_ARENA_QUICK_ENTER_TEAM:                    return "T_FC_ARENA_QUICK_ENTER_TEAM";
    case T_FC_ARENA_WATCH_READY:                        return "T_FC_ARENA_WATCH_READY";
    case T_FC_ARENA_WATCH_START:                        return "T_FC_ARENA_WATCH_START";
    case T_FC_ARENA_WATCH_END:                            return "T_FC_ARENA_WATCH_END";
    case T_FC_ARENA_WATCH_REMOVE_USER:                    return "T_FC_ARENA_WATCH_REMOVE_USER";
    // 2008-01-08 by dhjin, 아레나 통합 - 
    case T_FC_ARENA_POSSIBLE_GAMESTART:                    return "T_FC_ARENA_POSSIBLE_GAMESTART";
    case T_FC_ARENA_CHARACTER_GAMESTART:                return "T_FC_ARENA_CHARACTER_GAMESTART";
    case T_FC_ARENA_USE_CITYWARPITEM:                    return "T_FC_ARENA_USE_CITYWARPITEM";
    
    case T_FC_TUTORIAL_START:                            return "T_FC_TUTORIAL_START";
    case T_FC_TUTORIAL_START_OK:                        return "T_FC_TUTORIAL_START_OK";
    case T_FC_TUTORIAL_COMPLETE:                        return "T_FC_TUTORIAL_COMPLETE";
    case T_FC_TUTORIAL_COMPLETE_OK:                        return "T_FC_TUTORIAL_COMPLETE_OK";
    case T_FC_TUTORIAL_END:                                return "T_FC_TUTORIAL_END";
    case T_FC_TUTORIAL_END_OK:                            return "T_FC_TUTORIAL_END_OK";
    case T_FC_TUTORIAL_WARP:                            return "T_FC_TUTORIAL_WARP";
    case T_FC_OUTPOST_WAR_BEFORE:                        return "T_FC_OUTPOST_WAR_BEFORE";
    case T_FC_OUTPOST_WAR_START:                        return "T_FC_OUTPOST_WAR_START";
    case T_FC_OUTPOST_PROTECTOR_DESTROY:                return "T_FC_OUTPOST_PROTECTOR_DESTROY";
//    case T_FC_OUTPOST_RESET_CHECK_START:                return "T_FC_OUTPOST_RESET_CHECK_START";
//    case T_FC_OUTPOST_RESET_CHECK_START_OK:                return "T_FC_OUTPOST_RESET_CHECK_START_OK";
    case T_FC_OUTPOST_RESET_START:                        return "T_FC_OUTPOST_RESET_START";
    case T_FC_OUTPOST_RESET_START_OK:                    return "T_FC_OUTPOST_RESET_START_OK";
    case T_FC_OUTPOST_RESET_DESTROY:                    return "T_FC_OUTPOST_RESET_DESTROY";
    case T_FC_OUTPOST_RESET_SUCCESS:                    return "T_FC_OUTPOST_RESET_SUCCESS";
    case T_FC_OUTPOST_WAR_END:                            return "T_FC_OUTPOST_WAR_END";
    case T_FC_OUTPOST_NEXTWAR_INFO_REQUEST:                return "T_FC_OUTPOST_NEXTWAR_INFO_REQUEST";
    case T_FC_OUTPOST_NEXTWAR_INFO_REQUEST_OK:            return "T_FC_OUTPOST_NEXTWAR_INFO_REQUEST_OK";
    case T_FC_OUTPOST_NEXTWAR_SET_TIME:                    return "T_FC_OUTPOST_NEXTWAR_SET_TIME";
    case T_FC_OUTPOST_NEXTWAR_SET_TIME_OK:                return "T_FC_OUTPOST_NEXTWAR_SET_TIME_OK";
    case T_FC_OUTPOST_WAR_INFO:                            return "T_FC_OUTPOST_WAR_INFO";
    case T_FC_INFO_NOTICE_REQUEST:                        return "T_FC_INFO_NOTICE_REQUEST";
    case T_FC_INFO_NOTICE_REQUEST_OK:                    return "T_FC_INFO_NOTICE_REQUEST_OK";
    case T_FC_INFO_NOTICE_REG:                            return "T_FC_INFO_NOTICE_REG";
    case T_FC_INFO_NOTICE_REG_OK:                        return "T_FC_INFO_NOTICE_REG_OK";
    case T_FC_INFO_NOTICE_MODIFY:                        return "T_FC_INFO_NOTICE_MODIFY";
    case T_FC_INFO_NOTICE_MODIFY_OK:                    return "T_FC_INFO_NOTICE_MODIFY_OK";
    case T_FC_INFO_EXPEDIENCYFUND_REQUEST:                return "T_FC_INFO_EXPEDIENCYFUND_REQUEST";
    case T_FC_INFO_EXPEDIENCYFUND_REQUEST_OK:            return "T_FC_INFO_EXPEDIENCYFUND_REQUEST_OK";
    case T_FC_INFO_EXPEDIENCYFUND_PAYBACK:                return "T_FC_INFO_EXPEDIENCYFUND_PAYBACK";
    case T_FC_INFO_EXPEDIENCYFUND_PAYBACK_OK:            return "T_FC_INFO_EXPEDIENCYFUND_PAYBACK_OK";
    case T_FC_INFO_SECONDARYPASSWORD_UPDATE_PASSWORD:        return "T_FC_INFO_SECONDARYPASSWORD_UPDATE_PASSWORD";        // 2007-09-12 by cmkwon, 베트남 2차패스워드 구현 -
    case T_FC_INFO_SECONDARYPASSWORD_UPDATE_PASSWORD_OK:    return "T_FC_INFO_SECONDARYPASSWORD_UPDATE_PASSWORD_OK";    // 2007-09-12 by cmkwon, 베트남 2차패스워드 구현 -
    case T_FC_INFO_SECONDARYPASSWORD_CHECK_PASSWORD:        return "T_FC_INFO_SECONDARYPASSWORD_CHECK_PASSWORD";        // 2007-09-12 by cmkwon, 베트남 2차패스워드 구현 -
    case T_FC_INFO_SECONDARYPASSWORD_CHECK_PASSWORD_OK:        return "T_FC_INFO_SECONDARYPASSWORD_CHECK_PASSWORD_OK";        // 2007-09-12 by cmkwon, 베트남 2차패스워드 구현 -
    case T_FC_INFO_SECONDARYPASSWORD_LOCK:                    return "T_FC_INFO_SECONDARYPASSWORD_LOCK";                    // 2007-09-12 by cmkwon, 베트남 2차패스워드 구현 -
    case T_FC_INFO_SECONDARYPASSWORD_LOCK_OK:                return "T_FC_INFO_SECONDARYPASSWORD_LOCK_OK";                // 2007-09-12 by cmkwon, 베트남 2차패스워드 구현 -
    case T_FC_INFO_GET_GUILDMARK:                        return "T_FC_INFO_GET_GUILDMARK";                    // 2007-12-07 by dhjin, 여단 마크 C->F
    case T_FC_INFO_GET_GUILDMARK_OK:                    return "T_FC_INFO_GET_GUILDMARK_OK";                // 2007-12-07 by dhjin, 여단 마크 F->C
    case T_FtoA_MFSINFO:                                return "T_FtoA_MFSINFO";                            // 2007-12-27 by dhjin, 아레나 통합 -     
    case T_FtoA_MFSINFO_OK:                                return "T_FtoA_MFSINFO_OK";                            // 2007-12-27 by dhjin, 아레나 통합 - 
    case T_FtoA_ALIVE:                                    return "T_FtoA_ALIVE";                                // 2007-12-27 by dhjin, 아레나 통합 - 
    case T_FtoA_AUTH_CHARACTER:                            return "T_FtoA_AUTH_CHARACTER";                        // 2007-12-27 by dhjin, 아레나 통합 - 
    case T_FtoA_AUTH_CHARACTER_OK:                        return "T_FtoA_AUTH_CHARACTER_OK";                    // 2007-12-27 by dhjin, 아레나 통합 -     
    case T_FtoA_ARENA_STATE_CHANGE:                        return "T_FtoA_ARENA_STATE_CHANGE";                    // 2008-01-03 by dhjin, 아레나 통합 - 
    case T_FtoA_ARENA_TEAM_MATCHING:                    return "T_FtoA_ARENA_TEAM_MATCHING";                // 2008-01-04 by dhjin, 아레나 통합 -     
    case T_FtoA_ARENA_TEAM_MATCHING_OK:                    return "T_FtoA_ARENA_TEAM_MATCHING_OK";                // 2008-01-04 by dhjin, 아레나 통합 - 
    case T_FtoA_ARENA_SERVER_PAY:                        return "T_FtoA_ARENA_SERVER_PAY";                    // 2008-01-10 by dhjin, 아레나 통합 - 
    case T_FtoA_ARENA_CHARACTER_PAY:                    return "T_FtoA_ARENA_CHARACTER_PAY";                    // 2008-01-10 by dhjin, 아레나 통합 - 
    case T_FtoA_ARENA_CHARACTER_DISCONNECT:                return "T_FtoA_ARENA_CHARACTER_DISCONNECT";                // 2008-01-15 by dhjin, 아레나 통합 - 
    case T_FtoA_ARENA_CHARACTERARENAINFO_UPDATA:        return "T_FtoA_ARENA_CHARACTERARENAINFO_UPDATA";        // 2012-04-12 by jhseol, 아레나 추가개발 - 보상 : AFS -> MFS CharacterArena UUpdata 요청 패킷
    case T_FtoA_ARENA_CHARACTER_RESET_RELOAD:            return "T_FtoA_ARENA_CHARACTER_RESET_RELOAD";        // 2012-12-18 by jhseol, 아레나 캐릭터 리셋 후 캐릭터 정보 다시 로딩하기.
    case T_FC_INFO_MSWARINFO_DISPLAY:                    return "T_FC_INFO_MSWARINFO_DISPLAY";                // 2008-03-27 by dhjin, 모선전 정보 표시 기획안 - 
    case T_FC_INFO_MSWARINFO_DISPLAY_OPTION:            return "T_FC_INFO_MSWARINFO_DISPLAY_OPTION";                // 2008-03-27 by dhjin, 모선전 정보 표시 기획안 - 
    case T_FC_INFO_MSWARINFO_DISPLAY_OPTION_OK:            return "T_FC_INFO_MSWARINFO_DISPLAY_OK";                // 2008-03-27 by dhjin, 모선전 정보 표시 기획안 - 
    case T_FC_INFO_MSWARINFO_RESULT:                    return "T_FC_INFO_MSWARINFO_RESULT";                    // 2008-04-02 by dhjin, 모선전, 거점전 정보창 기획안 - 
    case T_FC_INFO_MSWARINFO_RESULT_OK:                    return "T_FC_INFO_MSWARINFO_RESULT_OK";                    // 2008-04-02 by dhjin, 모선전, 거점전 정보창 기획안 - 
    case T_FC_INFO_STRATEGICPOINTINFO_DISPLAY_LIST:        return "T_FC_INFO_STRATEGICPOINTINFO_DISPLAY_LIST";        // 2015-05-01 by killburne
    case T_FC_INFO_STRATEGICPOINTINFO_DISPLAY:            return "T_FC_INFO_STRATEGICPOINTINFO_DISPLAY";            // 2015-05-01 by killburne
    case T_FC_INFO_SPWARINFO_RESULT:                    return "T_FC_INFO_SPWARINFO_RESULT";                    // 2008-04-02 by dhjin, 모선전, 거점전 정보창 기획안 - 
    case T_FC_INFO_SPWARINFO_RESULT_OK_HEADER:            return "T_FC_INFO_SPWARINFO_RESULT_OK_HEADER";            // 2008-04-02 by dhjin, 모선전, 거점전 정보창 기획안 - 
    case T_FC_INFO_SPWARINFO_RESULT_OK:                    return "T_FC_INFO_SPWARINFO_RESULT_OK";                    // 2008-04-02 by dhjin, 모선전, 거점전 정보창 기획안 - 
    case T_FC_INFO_SPWARINFO_RESULT_OK_DONE:            return "T_FC_INFO_SPWARINFO_RESULT_OK_DONE";            // 2008-04-02 by dhjin, 모선전, 거점전 정보창 기획안 - 
    case T_FC_CHAT_LETTER_REQUEST_TITLE:                return "T_FC_CHAT_LETTER_REQUEST_TITLE";                // 2008-04-29 by dhjin, EP3 편지 시스템 - 편지 리스트 요청 C->F
    case T_FC_CHAT_LETTER_REQUEST_TITLE_OK_HEADER:        return "T_FC_CHAT_LETTER_REQUEST_TITLE_OK_HEADER";        // 2008-04-29 by dhjin, EP3 편지 시스템 - 편지 리스트 전송 F->C
    case T_FC_CHAT_LETTER_REQUEST_TITLE_OK:                return "T_FC_CHAT_LETTER_REQUEST_TITLE_OK";                // 2008-04-29 by dhjin, EP3 편지 시스템 - 편지 리스트 전송 F->C
    case T_FC_CHAT_LETTER_REQUEST_TITLE_OK_DONE:        return "T_FC_CHAT_LETTER_REQUEST_TITLE_OK_DONE";        // 2008-04-29 by dhjin, EP3 편지 시스템 - 편지 리스트 전송 F->C
    case T_FC_CHAT_LETTER_READ:                            return "T_FC_CHAT_LETTER_READ";                            // 2008-04-29 by dhjin, EP3 편지 시스템 - 편지 읽기 요청 C->F
    case T_FC_CHAT_LETTER_READ_OK:                        return "T_FC_CHAT_LETTER_READ_OK";                        // 2008-04-29 by dhjin, EP3 편지 시스템 - 편지 읽기 완료 F->C
    case T_FC_CHAT_LETTER_DELETE:                        return "T_FC_CHAT_LETTER_DELETE";                        // 2008-04-29 by dhjin, EP3 편지 시스템 - 편지 지우기 요청 C->F
    case T_FC_CHAT_LETTER_DELETE_OK:                    return "T_FC_CHAT_LETTER_DELETE_OK";                    // 2008-04-29 by dhjin, EP3 편지 시스템 - 편지 지우기 완료 F->C
    case T_FC_CHAT_LETTER_SEND:                            return "T_FC_CHAT_LETTER_SEND";                            // 2008-05-08 by dhjin, EP3 편지 시스템 - 편지 전송 C->F
    case T_FC_CHAT_LETTER_SEND_OK:                        return "T_FC_CHAT_LETTER_SEND_OK";                        // 2008-05-08 by dhjin, EP3 편지 시스템 - 편지 전송 완료 F->C 
    case T_FC_CHAT_LETTER_RECEIVE:                        return "T_FC_CHAT_LETTER_RECEIVE";                        // 2008-05-08 by dhjin, EP3 편지 시스템 - 편지 전송 받음 F->C
    case T_FC_CHAT_ALLLETTER_REQUEST_TITLE:                return "T_FC_CHAT_ALLLETTER_REQUEST_TITLE";                // 2008-05-09 by dhjin, EP3 편지 시스템 - 전체 편지 리스트 요청 C->F
    case T_FC_CHAT_ALLLETTER_REQUEST_TITLE_OK_HEADER:    return "T_FC_CHAT_ALLLETTER_REQUEST_TITLE_OK_HEADER";    // 2008-05-09 by dhjin, EP3 편지 시스템 - 전체 편지 리스트 전송 F->C
    case T_FC_CHAT_ALLLETTER_REQUEST_TITLE_OK:            return "T_FC_CHAT_ALLLETTER_REQUEST_TITLE_OK";            // 2008-05-09 by dhjin, EP3 편지 시스템 - 전체 편지 리스트 전송 F->C
    case T_FC_CHAT_ALLLETTER_REQUEST_TITLE_OK_DONE:        return "T_FC_CHAT_ALLLETTER_REQUEST_TITLE_OK_DONE";        // 2008-05-09 by dhjin, EP3 편지 시스템 - 전체 편지 리스트 전송 F->C
    case T_FC_CHAT_ALLLETTER_READ:                        return "T_FC_CHAT_ALLLETTER_READ";                        // 2008-05-09 by dhjin, EP3 편지 시스템 - 전체 편지 읽기 요청 C->F
    case T_FC_CHAT_ALLLETTER_READ_OK:                    return "T_FC_CHAT_ALLLETTER_READ_OK";                    // 2008-05-09 by dhjin, EP3 편지 시스템 - 전체 편지 읽기 완료 F->C
    case T_FC_CHAT_ALLLETTER_DELETE:                    return "T_FC_CHAT_ALLLETTER_DELETE";                    // 2008-05-09 by dhjin, EP3 편지 시스템 - 전체 편지 지우기 요청 C->F
    case T_FC_CHAT_ALLLETTER_DELETE_OK:                    return "T_FC_CHAT_ALLLETTER_DELETE_OK";                // 2008-05-09 by dhjin, EP3 편지 시스템 - 전체 편지 지우기 완료 F->C
    case T_FC_CHAT_ALLLETTER_SEND:                        return "T_FC_CHAT_ALLLETTER_SEND";                    // 2008-05-09 by dhjin, EP3 편지 시스템 - 전체 편지 전송 C->F
    case T_FC_CHAT_ALLLETTER_SEND_OK:                    return "T_FC_CHAT_ALLLETTER_SEND_OK";                    // 2008-05-09 by dhjin, EP3 편지 시스템 - 전체 편지 전송 완료 F->C 
    case T_FC_CHAT_ALLLETTER_RECEIVE:                    return "T_FC_CHAT_ALLLETTER_RECEIVE";                    // 2008-05-09 by dhjin, EP3 편지 시스템 - 전체 편지 전송 받음 F->C
    case T_FC_EVENT_INFLUENCEMARK:                        return "T_FC_EVENT_INFLUENCEMARK";                        // 2008-08-18 by dhjin, 세력마크이벤트 F->C
    case T_FC_EVENT_INFLUENCEMARKEND:                    return "T_FC_EVENT_INFLUENCEMARKEND";                    // 2008-08-18 by dhjin, 세력마크이벤트 F->C
    case T_FC_QUEST_REQUEST_MISSIONMASTER_HELP:            return "T_FC_QUEST_REQUEST_MISSIONMASTER_HELP";            // 2008-12-02 by dhjin, C -> F, 미션마스터 요청
    case T_FC_QUEST_REQUEST_MISSIONMASTER_HELP_INVITE:    return "T_FC_QUEST_REQUEST_MISSIONMASTER_HELP_INVITE";    // 2008-12-02 by dhjin, F -> C(n), 랜덤으로 뽑힌 미션마스터에게 요청
    case T_FC_QUEST_MISSIONMASTER_HELP_INVITE:            return "T_FC_QUEST_MISSIONMASTER_HELP_INVITE";            // 2008-12-02 by dhjin, C -> F, 미션마스터 요청 승락 
    case T_FC_QUEST_MISSIONMASTER_HELP_INVITE_OK:        return "T_FC_QUEST_MISSIONMASTER_HELP_INVITE_OK";        // 2008-12-02 by dhjin, F -> C, 미션마스터 요청 승락
    case T_FC_QUEST_MISSIONMASTER_HELP_REJECT:            return "T_FC_QUEST_MISSIONMASTER_HELP_REJECT";            // 2008-12-02 by dhjin, C -> F, 미션마스터 요청 거절 
    case T_FC_QUEST_MISSIONMASTER_HELP_REJECT_OK:        return "T_FC_QUEST_MISSIONMASTER_HELP_REJECT_OK";        // 2008-12-02 by dhjin, F -> C, 미션마스터 요청 거절
    case T_FC_QUEST_INSERT_QUEST:                        return "T_FC_QUEST_INSERT_QUEST";                        // 2009-03-31 by cmkwon, 세력초기화 시스템 구현 - 
    case T_FC_QUEST_DELETE_QUEST:                        return "T_FC_QUEST_DELETE_QUEST";                        // 2009-03-31 by cmkwon, 세력초기화 시스템 구현 - 

    case T_FC_INFO_DECLARATION_MSWAR_INFO:                return "T_FC_INFO_DECLARATION_MSWAR_INFO";                // 2009-01-12 by dhjin, 선전 포고 - 선전포고 정보 요청 C->F
    case T_FC_INFO_DECLARATION_MSWAR_INFO_OK:            return "T_FC_INFO_DECLARATION_MSWAR_INFO_OK";            // 2009-01-12 by dhjin, 선전 포고 - 선전포고 정보 전송 F->C
    case T_FC_INFO_DECLARATION_MSWAR_SET:                return "T_FC_INFO_DECLARATION_MSWAR_SET";                // 2009-01-12 by dhjin, 선전 포고 - 선전포고 시간 및 포기 설정 C->F
    case T_FI_INFO_DECLARATION_MSWAR_SET_OK:            return "T_FI_INFO_DECLARATION_MSWAR_SET_OK";            // 2009-01-12 by dhjin, 선전 포고 - 선전포고 시간 및 포기 설정 F->I
    case T_IC_INFO_DECLARATION_MSWAR_SET_OK:            return "T_IC_INFO_DECLARATION_MSWAR_SET_OK";            // 2009-01-12 by dhjin, 선전 포고 - 선전포고 시간 및 포기 설정 I->C

    case T_FC_INFO_WRK_GET_SERVICE_INFO:                return "T_FC_INFO_WRK_GET_SERVICE_INFO";                // 2009-02-12 by cmkwon, EP3-3 월드랭킹시스템 구현 - 
    case T_FC_INFO_WRK_GET_SERVICE_INFO_OK:                return "T_FC_INFO_WRK_GET_SERVICE_INFO_OK";                // 2009-02-12 by cmkwon, EP3-3 월드랭킹시스템 구현 - 
    case T_FC_INFO_WRK_GET_SERVICE_INFO_OK_IMAGE:        return "T_FC_INFO_WRK_GET_SERVICE_INFO_OK_IMAGE";        // 2009-02-12 by cmkwon, EP3-3 월드랭킹시스템 구현 - 
    case T_FC_INFO_WRK_GET_SERVICE_INFO_OK_DONE:        return "T_FC_INFO_WRK_GET_SERVICE_INFO_OK_DONE";        // 2009-02-12 by cmkwon, EP3-3 월드랭킹시스템 구현 - 
    case T_FC_INFO_WRK_GET_RANKER_LIST:                    return "T_FC_INFO_WRK_GET_RANKER_LIST";                    // 2009-02-12 by cmkwon, EP3-3 월드랭킹시스템 구현 - 
    case T_FC_INFO_WRK_GET_LEVEL_RANKER_LIST_OK:        return "T_FC_INFO_WRK_GET_LEVEL_RANKER_LIST_OK";        // 2009-02-12 by cmkwon, EP3-3 월드랭킹시스템 구현 - 
    case T_FC_INFO_WRK_GET_FAME_RANKER_LIST_OK:            return "T_FC_INFO_WRK_GET_FAME_RANKER_LIST_OK";            // 2009-02-12 by cmkwon, EP3-3 월드랭킹시스템 구현 - 
    case T_FC_INFO_WRK_GET_PVP_RANKER_LIST_OK:            return "T_FC_INFO_WRK_GET_PVP_RANKER_LIST_OK";            // 2009-02-12 by cmkwon, EP3-3 월드랭킹시스템 구현 - 
    case T_FC_INFO_WRK_GET_SELF_RANKING:                return "T_FC_INFO_WRK_GET_SELF_RANKING";                // 2009-02-12 by cmkwon, EP3-3 월드랭킹시스템 구현 - 
    case T_FC_INFO_WRK_GET_SELF_RANKING_OK:                return "T_FC_INFO_WRK_GET_SELF_RANKING_OK";                // 2009-02-12 by cmkwon, EP3-3 월드랭킹시스템 구현 - 

    ////////////////////////////////////////////////////////////////////////////////
    // 2009-09-09 ~ 2010 by dhjin, 인피니티 -
    case T_FN_MONSTER_HPTALK:                            return "T_FN_MONSTER_HPTALK";                            // 2009-09-09 ~ 2010 by dhjin, 인피니티 - , N->F
    case T_FC_MONSTER_HPTALK:                            return "T_FC_MONSTER_HPTALK";                            // 2009-09-09 ~ 2010 by dhjin, 인피니티 - , F->C
    case T_FC_MONSTER_SKILL:                            return "T_FC_MONSTER_SKILL";                            // 2009-09-09 ~ 2010 by dhjin, 인피니티 - , F->C
    case T_FC_INFINITY_MODE_LIST:                        return "T_FC_INFINITY_MODE_LIST";                        // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 리스트 요청, C -> F
    case T_FC_INFINITY_MODE_LIST_OK:                    return "T_FC_INFINITY_MODE_LIST_OK";                    // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 리스트 요청, F -> C
    case T_FC_INFINITY_READY_LIST:                        return "T_FC_INFINITY_READY_LIST";                        // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 리스트 요청, C -> F
    case T_FC_INFINITY_READY_LIST_OK:                    return "T_FC_INFINITY_READY_LIST_OK";                    // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 리스트 요청, F -> C
    case T_FC_INFINITY_CREATE:                            return "T_FC_INFINITY_CREATE";                            // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 생성 요청, C -> F
    case T_FC_INFINITY_CREATE_OK:                        return "T_FC_INFINITY_CREATE_OK";                        // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 생성 요청, F -> C
    case T_FC_INFINITY_JOIN:                            return "T_FC_INFINITY_JOIN";                            // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 가입 요청, C -> F
    case T_FC_INFINITY_JOIN_REQUEST_MASTERUSER:            return "T_FC_INFINITY_JOIN_REQUEST_MASTERUSER";            // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 방장에거 가입자 승인 여부 요청, F -> C
    case T_FC_INFINITY_JOIN_REQUEST_MASTERUSER_OK:        return "T_FC_INFINITY_JOIN_REQUEST_MASTERUSER_OK";        // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 방장에거 가입자 승인 여부 요청, C -> F
    case T_FC_INFINITY_JOIN_OK:                            return "T_FC_INFINITY_JOIN_OK";                            // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 가입 요청, F -> C
    case T_FC_INFINITY_MEMBER_INFO_LIST:                return "T_FC_INFINITY_MEMBER_INFO_LIST";                // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 맴버 정보, C -> F
    case T_FC_INFINITY_MEMBER_INFO_LIST_OK:                return "T_FC_INFINITY_MEMBER_INFO_LIST_OK";                // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 맴버 정보, F -> C
    case T_FC_INFINITY_CHANGE_MASTER:                    return "T_FC_INFINITY_CHANGE_MASTER";                    // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 마스터 변경, C -> F
    case T_FC_INFINITY_CHANGE_MASTER_OK:                return "T_FC_INFINITY_CHANGE_MASTER_OK";                // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 마스터 변경, F -> C
    case T_FC_INFINITY_LEAVE:                            return "T_FC_INFINITY_LEAVE";                            // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 탈퇴, C -> F
    case T_FC_INFINITY_LEAVE_OK:                        return "T_FC_INFINITY_LEAVE_OK";                        // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 탈퇴, F -> C
    case T_FC_INFINITY_BAN:                                return "T_FC_INFINITY_BAN";                                // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 추방, C -> F
    case T_FC_INFINITY_BAN_OK:                            return "T_FC_INFINITY_BAN_OK";                            // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 추방, F -> C
    case T_FtoA_INFINITY_IMPUTE_LIST:                    return "T_FtoA_INFINITY_IMPUTE_LIST";                    // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 귀속 정보 전송, MFS -> AFS
    case T_FC_INFINITY_READY:                            return "T_FC_INFINITY_READY";                            // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 래디 버튼 누름, C -> F
    case T_FC_INFINITY_READY_OK:                        return "T_FC_INFINITY_READY_OK";                        // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 래디 버튼 누름, F -> C
    case T_FC_INFINITY_READY_CANCEL:                    return "T_FC_INFINITY_READY_CANCEL";                    // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 래디취소 버튼 누름, C -> F
    case T_FC_INFINITY_READY_CANCEL_OK:                    return "T_FC_INFINITY_READY_CANCEL_OK";                    // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 래디취소 버튼 누름, F -> C
    case T_FC_INFINITY_START:                            return "T_FC_INFINITY_START";                            // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 시작 버튼 누름(인피 시작), C -> F
    case T_FtoA_INFINITY_START:                            return "T_FtoA_INFINITY_START";                            // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 시작 MainFieldServer로 알림, AFS -> MFS
    case T_FtoA_INFINITY_START_OK:                        return "T_FtoA_INFINITY_START_OK";                        // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 시작 MainFieldSever준비 완료 전송, MFS -> AFS
    case T_FC_INFINITY_MAP_LOADING:                        return "T_FC_INFINITY_MAP_LOADING";                        // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 맵 로딩 상태임을 알림, F -> C
    case T_FC_INFINITY_MAP_LOADED:                        return "T_FC_INFINITY_MAP_LOADED";                        // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 맵 로딩 완료, C -> F
    case T_FtoA_INFINITY_IMPUTE:                        return "T_FtoA_INFINITY_IMPUTE";                        // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 MainFieldServer로 귀속 정보 전송, AFS -> MFS
    case T_FC_INFINITY_ENTER:                            return "T_FC_INFINITY_ENTER";                            // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 맵 입장 허가, F -> C
    case T_FC_INFINITY_CINEMA:                            return "T_FC_INFINITY_CINEMA";                            // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 시네마 정보 전송, F -> C
    case T_FN_NPCSERVER_CINEMA_MONSTER_CREATE:            return "T_FN_NPCSERVER_CINEMA_MONSTER_CREATE";            // 2010-03-31 by dhjin, 인피니티(기지방어) -    // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 키 몬스터 생성, F -> N
    case T_FC_INFINITY_SUCCESS_FIN:                        return "T_FC_INFINITY_SUCCESS_FIN";                        // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 성공 종료, F -> C
    case T_FC_INFINITY_FAIL_FIN:                        return "T_FC_INFINITY_FAIL_FIN";                        // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 실패 종료, F -> C
    case T_FC_INFINITY_FIN_OK:                            return "T_FC_INFINITY_FIN_OK";                            // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 종료 확인 후 마을(Main서버로 돌아감), C -> F
    case T_FC_INFINITY_READY_FINISH_MAINSVR_START:        return "T_FC_INFINITY_READY_FINISH_MAINSVR_START";        // 2009-09-09 ~ 2010 by dhjin, 인피니티 - Main서버로 돌아갈 준비가 완료 됨, F -> C
    case T_FC_INFINITY_MOVIE:                            return "T_FC_INFINITY_MOVIE";                            // 2011-05-17 by hskim, 인피니티 3차 - 시네마 연출, F -> C
    case T_FtoA_INFINITY_UPDATE_ALL_ITEM_COUNT:            return "T_FtoA_INFINITY_UPDATE_ALL_ITEM_COUNT";            // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 MainFieldServer로 아이템 동기화 위해 전송, AFS -> MFS
    case T_FtoA_INFINITY_INSERT_ITEM_HEADER:            return "T_FtoA_INFINITY_INSERT_ITEM_HEADER";            // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 MainFieldServer로 아이템 동기화 위해 전송, AFS -> MFS
    case T_FtoA_INFINITY_INSERT_ITEM:                    return "T_FtoA_INFINITY_INSERT_ITEM";                    // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 MainFieldServer로 아이템 동기화 위해 전송, AFS -> MFS
    case T_FtoA_INFINITY_INSERT_ITEM_DONE:                return "T_FtoA_INFINITY_INSERT_ITEM_DONE";                // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 MainFieldServer로 아이템 동기화 위해 전송, AFS -> MFS
    case T_FtoA_INFINITY_UPDATE_ITEM_HEADER:            return "T_FtoA_INFINITY_UPDATE_ITEM_HEADER";            // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 MainFieldServer로 아이템 동기화 위해 전송, AFS -> MFS
    case T_FtoA_INFINITY_UPDATE_ITEM:                    return "T_FtoA_INFINITY_UPDATE_ITEM";                    // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 MainFieldServer로 아이템 동기화 위해 전송, AFS -> MFS
    case T_FtoA_INFINITY_UPDATE_ITEM_DONE:                return "T_FtoA_INFINITY_UPDATE_ITEM_DONE";                // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 MainFieldServer로 아이템 동기화 위해 전송, AFS -> MFS
    case T_FtoA_INFINITY_DELETE_ITEM:                    return "T_FtoA_INFINITY_DELETE_ITEM";                    // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 MainFieldServer로 아이템 동기화 위해 전송, AFS -> MFS
    case T_FtoA_INFINITY_UPDATE_USER_INFO:                return "T_FtoA_INFINITY_UPDATE_USER_INFO";                // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 MainFieldServer로 유저 동기화 위해 전송, AFS -> MFS
    case T_FtoA_INFINITY_READY_FINISH_MAINSVR_START:    return "T_FtoA_INFINITY_READY_FINISH_MAINSVR_START";    // 2009-09-09 ~ 2010 by dhjin, 인피니티 - Main서버로 돌아갈 준비가 완료 됨, MFS -> AFS
    case T_FtoA_INFINITY_LOG:                            return "T_FtoA_INFINITY_LOG";                            // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 MainFieldServer로 로그 전송, AFS -> MFS
    case T_FtoA_INFINITY_USING_TIME_LIMIT_ITEM:            return "T_FtoA_INFINITY_USING_TIME_LIMIT_ITEM";            // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 MainFieldServer로 로그 전송, AFS -> MFS
    case T_FN_NPCSERVER_NEW_CHANGE_OBJECT:                return "T_FN_NPCSERVER_NEW_CHANGE_OBJECT";                    // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 변경 오브젝트를 위해!!!! 
    case T_FN_NPCSERVER_RESET_CHANGE_OBJECT:            return "T_FN_NPCSERVER_RESET_CHANGE_OBJECT";                // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 변경 오브젝트를 위해!!!! 
    case T_FC_INFINITY_TENDER_DROPITEM_INFO:            return "T_FC_INFINITY_TENDER_DROPITEM_INFO";            // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 몬스터에게서 드랍된 Tender 아이템, F -> C
    case T_FC_INFINITY_TENDER_START:                    return "T_FC_INFINITY_TENDER_START";                    // 2009-09-09 ~ 2010 by dhjin, 인피니티 - Tender 시작, F -> C
    case T_FC_INFINITY_TENDER_PUT_IN_TENDER:            return "T_FC_INFINITY_TENDER_PUT_IN_TENDER";            // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 유저 Tender 버튼 누름, C -> F
    case T_FC_INFINITY_TENDER_PUT_IN_TENDER_OK:            return "T_FC_INFINITY_TENDER_PUT_IN_TENDER_OK";            // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 유저 Tender 버튼 누름 결과, F -> C
    case T_FC_INFINITY_TENDER_RESULT:                    return "T_FC_INFINITY_TENDER_RESULT";                    // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 입찰자 결과, F -> C
    case T_FC_INFINITY_TENDER_RESULT_TIMEOVER:            return "T_FC_INFINITY_TENDER_TIMEOVER";                    // 2009-09-09 ~ 2010 by dhjin, 인피니티 - Tender 시간이 지났지만 입찰자가 없다, F -> C
    case T_FC_INFINITY_TENDER_RESULT_ALLGIVEUP:            return "T_FC_INFINITY_TENDER_ALLGIVEUP";                // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 입찰자 모두 포기, F -> C
    case T_FC_INFO_APPLY_RESISTANCE_ITEM:                return "T_FC_INFO_APPLY_RESISTANCE_ITEM";                // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 저항 아이템 적용 정보 전송, F -> C(n)
    case T_FC_SHOP_INFINITY_ITEM_HEADER:                return "T_FC_SHOP_INFINITY_ITEM_HEADER";                // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 상점
    case T_FC_SHOP_INFINITY_ITEM:                        return "T_FC_SHOP_INFINITY_ITEM";                        // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 상점
    case T_FC_SHOP_INFINITY_ITEM_DONE:                    return "T_FC_SHOP_INFINITY_ITEM_DONE";                    // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 상점
    case T_FC_SHOP_INFINITY_BUY_ITEM:                    return "T_FC_SHOP_INFINITY_BUY_ITEM";                    // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 상점, 구매
    case T_FC_INFINITY_CHANGE_LIMITTIME:                return "T_FC_INFINITY_CHANGE_LIMITTIME";                // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 사망시 패널티 추가, F -> C
    case T_FC_INFINITY_JOIN_CANCEL:                        return "T_FC_INFINITY_JOIN_CANCEL";                        // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 입장 취소
    case T_FC_INFINITY_JOIN_CANCEL_REQUEST_MASTERUSER:    return "T_FC_INFINITY_JOIN_CANCEL_REQUEST_MASTERUSER";    // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 입장 취소
    case T_FC_INFINITY_REQUEST_RESTART_BY_DISCONNECT:    return "T_FC_INFINITY_REQUEST_RESTART_BY_DISCONNECT";            // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 팅긴 유저 재접속 처리
    case T_FC_INFINITY_RESTART_BY_DISCONNECT:            return "T_FC_INFINITY_RESTART_BY_DISCONNECT";            // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 팅긴 유저 재접속 처리
    case T_FC_INFINITY_MAP_LOADED_RESTART_BY_DISCONNECT:            return "T_FC_INFINITY_MAP_LOADED_RESTART_BY_DISCONNECT";            // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 팅긴 유저 재접속 처리
    case T_FC_INFINITY_DELETED_CINEMA_HEADER:            return "T_FC_INFINITY_DELETED_CINEMA_HEADER";            // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 팅긴 유저 재접속 처리
    case T_FC_INFINITY_DELETED_CINEMA:                    return "T_FC_INFINITY_DELETED_CINEMA";            // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 팅긴 유저 재접속 처리
    case T_FC_INFINITY_DELETED_CINEMA_DONE:                return "T_FC_INFINITY_DELETED_CINEMA_DONE";            // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 팅긴 유저 재접속 처리
    case T_FC_INFINITY_ENTER_BY_DISCONNECT:                return "T_FC_INFINITY_ENTER_BY_DISCONNECT";            // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 팅긴 유저 재접속 처리
    case T_FtoA_INFINITY_STATE_CHANGE:                    return "T_FtoA_INFINITY_STATE_CHANGE";            // 2009-09-09 ~ 2010-01-20 by dhjin, 인피니티 - 인피 상태 값을 메인서버로 전송한다. 인피 상태 체크
    case T_FI_PARTY_AUTO_CREATE:                        return "T_FI_PARTY_AUTO_CREATE";                    // 2009-09-09 ~ 2010-01-26 by dhjin, 인피니티 - 자동 편대 생성 요청, F -> I
    case T_FI_PARTY_AUTO_CREATE_OK:                        return "T_FI_PARTY_AUTO_CREATE_OK";                // 2009-09-09 ~ 2010-01-26 by dhjin, 인피니티 - 자동 편대 생성 완료, I -> F
    case T_FC_PARTY_AUTO_CREATED:                        return "T_FC_PARTY_AUTO_CREATED";                    // 2009-09-09 ~ 2010-01-26 by dhjin, 인피니티 - 자동 편대 생성 완료 알림, F -> C
    case T_IC_PARTY_GET_AUTO_PARTY_INFO:                return "T_IC_PARTY_GET_AUTO_PARTY_INFO";            // 2009-09-09 ~ 2010-01-26 by dhjin, 인피니티 - 자동 편대 정보 요청, C -> I
    case T_IC_PARTY_GET_AUTO_PARTY_INFO_OK:                return "T_IC_PARTY_GET_AUTO_PARTY_INFO_OK";        // 2009-09-09 ~ 2010-01-26 by dhjin, 인피니티 - 자동 편대 정보 전송, I -> C
    case T_FI_PARTY_DISCONNECT_LEAVE_OK:                return "T_FI_PARTY_DISCONNECT_LEAVE_OK";        // 2009-09-09 ~ 2010-01-27 by dhjin, 인피니티 - 강제 종료 유저 파티에서 탈퇴처리!
    case T_FC_ITEM_USE_INVOKING_WEAR_ITEM:                return "T_FC_ITEM_USE_INVOKING_WEAR_ITEM";        // 2009-09-09 ~ 2010-02-10 by dhjin, 인피니티 - 발동류장착아이템 사용, C->F
    case T_FC_ITEM_EXPIRE_TIME_INVOKING_WEAR_ITEM:        return "T_FC_ITEM_EXPIRE_TIME_INVOKING_WEAR_ITEM";        // 2009-09-09 ~ 2010-02-10 by dhjin, 인피니티 - 발동류장착아이템 지속 시간 완료, C->F
    case T_FC_INFO_APPLY_DESTPARAM:                        return "T_FC_INFO_APPLY_DESTPARAM";                // 2009-09-09 ~ 2010-02-10 by dhjin, 인피니티 - 발동류장착아이템, DestParam 적용 유저 정보 전송
    case T_FC_INFO_APPLY_DESTPARAM_LIST:                return "T_FC_INFO_APPLY_DESTPARAM_LIST";                // 2009-09-09 ~ 2010-02-10 by dhjin, 인피니티 - 발동류장착아이템, DestParam 적용 유저 정보 전송
    case T_FC_ITEM_END_COOLINGTIME_ITEM:                return "T_FC_ITEM_END_COOLINGTIME_ITEM";        // 2009-09-09 ~ 2010-02-10 by dhjin, 인피니티 - 발동류장착아이템, 쿨타임 종료 C->F
    case T_FC_ITEM_END_COOLINGTIME_ITEM_OK:                return "T_FC_ITEM_END_COOLINGTIME_ITEM_OK";        // 2009-09-09 ~ 2010-02-10 by dhjin, 인피니티 - 발동류장착아이템, 쿨타임 종료 F->C
    case T_FC_ITEM_GET_COOLINGTIME_INFO:                return "T_FC_ITEM_GET_COOLINGTIME_INFO";        // 2009-09-09 ~ 2010-02-10 by dhjin, 인피니티 - 발동류장착아이템, 쿨타임 정보 요청 C->F
    case T_FC_ITEM_GET_COOLINGTIME_INFO_OK:                return "T_FC_ITEM_GET_COOLINGTIME_INFO_OK";        // 2009-09-09 ~ 2010-02-10 by dhjin, 인피니티 - 발동류장착아이템, 쿨타임 정보 요청 F->C
    case T_FC_ITEM_USE_INVOKING_WEAR_ITEM_BUFF:            return "T_FC_ITEM_USE_INVOKING_WEAR_ITEM_BUFF";        // 2009-09-09 ~ 2010-02-10 by dhjin, 인피니티 - 발동류장착아이템, 버프 올리세요 F->C
    case T_FC_INFINITY_CHANGE_ALIVE_FOR_GAMECLEAR_MONSTERHP:    return "T_FC_INFINITY_CHANGE_ALIVE_FOR_GAMECLEAR_MONSTERHP";        // 2010-03-31 by dhjin, 인피니티(기지방어) - 인피 사망시 패널티, F -> C
    case T_FC_MONSTER_CREATED_ALIVE_FOR_GAMECLEAR:        return "T_FC_MONSTER_CREATED_ALIVE_FOR_GAMECLEAR";        // 2010-03-31 by dhjin, 인피니티(기지방어) -

    case T_FC_ITEM_UPDATE_TRANSFORMER_OK:                return "T_FC_ITEM_UPDATE_TRANSFORMER_OK";            // 2010-03-18 by cmkwon, 몬스터변신 구현 - 
    case T_FI_PARTY_UPDATE_ITEM_TRANSFORMER_OK:            return "T_FI_PARTY_UPDATE_ITEM_TRANSFORMER_OK";        // 2010-03-18 by cmkwon, 몬스터변신 구현 - 
    case T_IC_PARTY_UPDATE_ITEM_TRANSFORMER_OK:            return "T_IC_PARTY_UPDATE_ITEM_TRANSFORMER_OK";        // 2010-03-18 by cmkwon, 몬스터변신 구현 - 

    case T_FN_BATTLE_ATTACK_SKILL_CANCEL:                return "T_FN_BATTLE_ATTACK_SKILL_CANCEL";        // 2010-03-31 by dhjin, 인피니티(기지방어) -
    case T_FC_MONSTER_SKILL_CANCEL:                        return "T_FC_MONSTER_SKILL_CANCEL";        // 2010-03-31 by dhjin, 인피니티(기지방어) -
    case T_FtoA_UPDATE_ITEM_NOTI:                        return "T_FtoA_UPDATE_ITEM_NOTI";        //    MFS -> AFS // 2010-03-31 by dhjin, 인피니티 입장 캐쉬 아이템 구현 - 

    case T_FtoA_INFINITY_UPDATE_USER_MAP_INFO:            return "T_FtoA_INFINITY_UPDATE_USER_MAP_INFO";        // 2010-04-06 by cmkwon, 인피2차 추가 수정 - 
    
    //case T_FC_INFINITY_DIFFICULTY_LIST:                return "T_FC_INFINITY_DIFFICULTY_LIST";        // 2010-05-04 by shcho, 인피니티 난이도 조절 -
    case T_FC_INFINITY_DIFFICULTY_LIST_OK:                return "T_FC_INFINITY_DIFFICULTY_LIST_OK";    // 2010-05-04 by shcho, 인피니티 난이도 조절 -

    case T_FC_INFINITY_CHANGE_DIFFICULTY_LEVEL:            return "T_FC_INFINITY_CHANGE_DIFFICULTY_LEVEL";            // C -> AFS // 2010-05-24 by hsLee, 난이도 변경 요청.
    case T_FC_INFINITY_CHANGE_DIFFICULTY_LEVEL_OK :        return "T_FC_INFINITY_CHANGE_DIFFICULTY_LEVEL_OK";        // AFS -> C // 2010-05-24 by hsLee, 난이도 변경 요청 결과.

    case T_FC_INFINITY_READY_CANCEL_ALL_OK :            return "T_FC_INFINITY_READY_CANCEL_ALL_OK";                // AFS -> C // 2010-05-31 by hsLee, 모든 래디 취소. (난이도 변경시).

    case T_FC_INFINITY_SKIP_ENDING_CINEMA :                return "T_FC_INFINITY_SKIP_ENDING_CINEMA";                // C -> F // 인피니티 거점 방어 엔딩 시네마 연출 스킵 요청. 2010. 07. 27 by hsLee.
    case T_FC_INFINITY_SKIP_ENDING_CINEMA_OK :            return "T_FC_INFINITY_SKIP_ENDING_CINEMA_OK";            // F -> C // 인피니티 거점 방어 엔딩 시네마 연출 스킵 결과. 2010. 07. 27 by hsLee.

    case T_FC_ITEM_HOMEPREMIUM_INFO :                    return "T_FC_ITEM_HOMEPREMIUM_INFO";                // F->C, // 2010-06-01 by shcho, PC방 권한 획득(캐쉬) 아이템 -

    // 2010-06-15 by shcho&hslee 펫시스템 - 클라이언트 패킷 전송 커맨드 추가
    case T_FC_ITEM_PET_HEADER:                            return "T_FC_ITEM_PET_HEADER";                
    case T_FC_ITEM_PET:                                    return "T_FC_ITEM_PET";
    case T_FC_ITEM_PET_BASEDATA_OK:                        return "T_FC_ITEM_PET_BASEDATA_OK";
    //case T_FC_ITEM_PET_SKILLDATA_OK:                    return "T_FC_ITEM_PET_SKILLDATA_OK";        // 2011-08-30 by hskim, 파트너 시스템 2차
    //case T_FC_ITEM_PET_SOCKETDATA_OK:                    return "T_FC_ITEM_PET_SOCKETDATA_OK";        // 2011-08-30 by hskim, 파트너 시스템 2차
    case T_FC_ITEM_PET_DONE:                            return "T_FC_ITEM_PET_DONE";
    
    case T_IP_AUTHENTICATION_SHUTDOWN:                    return "T_IP_AUTHENTICATION_SHUTDOWN";        // 2011-06-22 by hskim, 사설 서버 방지
    case T_FP_AUTHENTICATION_SHUTDOWN:                    return "T_FP_AUTHENTICATION_SHUTDOWN";        // 2011-06-22 by hskim, 사설 서버 방지    

    case T_IP_SELECTIVE_SHUTDOWN_NOTICE:                return "T_IP_SELECTIVE_SHUTDOWN_NOTICE";        // 2012-07-11 by hskim, 선택적 셧다운

    /////////////////////////////////////////////////////////////
    // start 2011-08-22 by hskim, 파트너 시스템 2차 - 기능 구현

    case T_FC_ITEM_PET_SET_NAME:                        return "T_FC_ITEM_PET_SET_NAME";                    // C -> F, 2011-08-22 by hskim, 파트너 시스템 2차 - 펫 이름 변경 요청
    case T_FC_ITEM_PET_SET_NAME_OK:                        return "T_FC_ITEM_PET_SET_NAME_OK";                    // F -> C, 2011-08-22 by hskim, 파트너 시스템 2차 - 펫 이름 변경 요청 결과
    case T_FC_ITEM_PET_SET_EXP_RATIO:                    return "T_FC_ITEM_PET_SET_EXP_RATIO";                // C -> F, 2011-08-22 by hskim, 파트너 시스템 2차 - 펫 경험치 획득 비율 변경 요청
    case T_FC_ITEM_PET_SET_EXP_RATIO_OK:                return "T_FC_ITEM_PET_SET_EXP_RATIO_OK";            // F -> C, 2011-08-22 by hskim, 파트너 시스템 2차 - 펫 경험치 획득 비율 변경 결과
    case T_FC_ITEM_PET_CHANGE_LEVEL:                    return "T_FC_ITEM_PET_CHANGE_LEVEL";                // F -> C, 2011-08-22 by hskim, 파트너 시스템 2차 - 펫 레벨업 알림
    case T_FC_ITEM_PET_CHANGE_EXP:                        return "T_FC_ITEM_PET_CHANGE_EXP";                    // F -> C, 2011-08-22 by hskim, 파트너 시스템 2차 - 펫 경험치 획득 알림

    case T_FC_ITEM_PET_SET_SOCKET:                        return "T_FC_ITEM_PET_SET_SOCKET";                    // C -> F, 2011-08-30 by hskim, 파트너 시스템 2차 - 소켓 아이템 설정
    case T_FC_ITEM_PET_SET_SOCKET_OK:                    return "T_FC_ITEM_PET_SET_SOCKET_OK";                // F -> C, 2011-08-30 by hskim, 파트너 시스템 2차 - 소켓 아이템 설정 결과
    case T_FC_ITEM_PET_SET_KIT_SLOT:                    return "T_FC_ITEM_PET_SET_KIT_SLOT";                // C -> F, 2011-08-30 by hskim, 파트너 시스템 2차 - 키트 슬롯 설정
    case T_FC_ITEM_PET_SET_KIT_SLOT_OK:                    return "T_FC_ITEM_PET_SET_KIT_SLOT_OK";                // F -> C, 2011-08-30 by hskim, 파트너 시스템 2차 - 키트 슬롯 설정 결과
    case T_FC_ITEM_PET_SET_AUTOSKILL_SLOT:                return "T_FC_ITEM_PET_SET_AUTOSKILL_SLOT";            // C -> F, 2011-08-30 by hskim, 파트너 시스템 2차 - 오토 스킬 슬롯 설정
    case T_FC_ITEM_PET_SET_AUTOSKILL_SLOT_OK:            return "T_FC_ITEM_PET_SET_AUTOSKILL_SLOT_OK";        // F -> C, 2011-08-30 by hskim, 파트너 시스템 2차 - 오토 스킬 슬롯 설정        

    case T_FtoA_INFINITY_COME_BACK_PREWORK:                return "T_FtoA_INFINITY_COME_BACK_PREWORK";            //  AMF - >MFS // 2011-09-29 by hskim, 파트너 시스템 2차 - 인피 MainFieldServer로 돌아가기 위한 사전 작업 처리 명령
    case T_FtoA_INFINITY_UPDATE_ITEM_PET:                return "T_FtoA_INFINITY_UPDATE_ITEM_PET";            //    AFS -> MFS // 2011-09-30 by hskim, 파트너 시스템 2차 - 인피 MainFieldServer로 파트너 아이템 동기화 위해 전송
    case T_FtoA_INFINITY_COME_BACK_POSTWORK:            return "T_FtoA_INFINITY_COME_BACK_POSTWORK";        //  AMF - >MFS  // 2011-09-29 by hskim, 파트너 시스템 2차 - 인피 MainFieldServer로 돌아가기 위한 후 작업 처리 명령

    // end 2011-08-22 by hskim, 파트너 시스템 2차 - 기능 구현
    /////////////////////////////////////////////////////////////


#ifdef S_WEB_CASHSHOP_SERVER_MODULE_HSKIM
    case T_FC_SHOP_AUTH_KEY:                            return "T_FC_SHOP_AUTH_KEY";        // 2013-03-13 by hskim, 웹 캐시 상점
#endif

    // 2013-05-31 by jhseol,bckim 아머 컬렉션 - 패킷 추가 : 메세지
    case T_FC_COLLECTION_INFO:                            return "T_FC_COLLECTION_INFO";
    case T_FC_COLLECTION_SHAPE_CHANGE:                    return "T_FC_COLLECTION_SHAPE_CHANGE";
    case T_FC_COLLECTION_MONTHLY_AROMOR_EVENT_INIT:        return "T_FC_COLLECTION_MONTHLY_AROMOR_EVENT_INIT";
    case T_FC_COLLECTION_MONTHLY_AROMOR_EVENT_INFO:        return "T_FC_COLLECTION_MONTHLY_AROMOR_EVENT_INFO";
    // end 2013-05-31 by jhseol,bckim 아머 컬렉션 - 패킷 추가 : 메세지
    }
    return "NO SUCH MESSAGE TYPE";
}

const char* GetGameLogTypeString(T1<T0::FL_LOG> logType)
{

    switch(logType)
    {

    // DEVX NOT IMPLEMENTED YET
    //case T_FA_ADMIN_CHANGE_SERVER_STATE:                return "T_FA_ADMIN_CHANGE_SERVER_STATE";

    case T1_FL_LOG_LOGIN:                                return "T1_FL_LOG_LOGIN";
    case T1_FL_LOG_LOGOUT:                                return "T1_FL_LOG_LOGOUT";
    case T1_FL_LOG_LEVEL:                                return "T1_FL_LOG_LEVEL";
    case T1_FL_LOG_EXP:                                    return "T1_FL_LOG_EXP";
    case T1_FL_LOG_STAT:                                return "T1_FL_LOG_STAT";
    case T1_FL_LOG_GAME_START:                            return "T1_FL_LOG_GAME_START";
    case T1_FL_LOG_GAME_END:                            return "T1_FL_LOG_GAME_END";
    case T1_FL_LOG_QUEST_COMPLETION:                    return "T1_FL_LOG_QUEST_COMPLETION";
    case T1_FL_LOG_QUEST_COMPENSATION:                    return "T1_FL_LOG_QUEST_COMPENSATION";

    case T1_FL_LOG_PKWIN:                                return "T1_FL_LOG_PKWIN";
    case T1_FL_LOG_PKLOSS:                                return "T1_FL_LOG_PKLOSS";
    case T1_FL_LOG_DEAD:                                return "T1_FL_LOG_DEAD";
    case T1_FL_LOG_WARP:                                return "T1_FL_LOG_WARP";
    case T1_FL_LOG_SPEED_HACK:                            return "T1_FL_LOG_SPEED_HACK";
    case T1_FL_LOG_CREATE_CHARACTER:                    return "T1_FL_LOG_CREATE_CHARACTER";
    case T1_FL_LOG_DELETE_CHARACTER:                    return "T1_FL_LOG_DELETE_CHARACTER";
    case T1_FL_LOG_MEMORY_HACK:                            return "T1_FL_LOG_MEMORY_HACK";
    case T1_FL_LOG_PvEWIN:                                return "T1_FL_LOG_PvEWIN";
    case T1_FL_LOG_PvELOSS:                                return "T1_FL_LOG_PvELOSS";
    case T1_FL_LOG_ITEM_CHARACTER_ITEM:                    return "T1_FL_LOG_ITEM_CHARACTER_ITEM";
    case T1_FL_LOG_ITEM_GET_ITEM:                        return "T1_FL_LOG_ITEM_GET_ITEM";
    case T1_FL_LOG_ITEM_TRADE_SEND:                        return "T1_FL_LOG_ITEM_TRADE_SEND";
    case T1_FL_LOG_ITEM_TRADE_RECV:                        return "T1_FL_LOG_ITEM_TRADE_RECV";
    case T1_FL_LOG_ITEM_THROWAWAY_ITEM:                    return "T1_FL_LOG_ITEM_THROWAWAY_ITEM";
    case T1_FL_LOG_ITEM_BUY_ITEM:                        return "T1_FL_LOG_ITEM_BUY_ITEM";

    
    case T1_FL_LOG_ITEM_BUY_CASH_PRICE:                        return "T1_FL_LOG_ITEM_BUY_CASH_PRICE";    
    case T1_FL_LOG_ITEM_CHARACTER_ITEM_GUILDSTORE_RESULT:    return "T1_FL_LOG_ITEM_CHARACTER_ITEM_GUILDSTORE_RESULT";    
    case T1_FL_LOG_ITEM_INITIALIZED_GUILD_STORE:            return "T1_FL_LOG_ITEM_INITIALIZED_GUILD_STORE";    
    case T1_FL_LOG_ITEM_GET_WITH_SAME_ACCOUNT:                return "T1_FL_LOG_ITEM_GET_WITH_SAME_ACCOUNT";    
    
    case T1_FL_LOG_ITEM_SELL_ITEM:                            return "T1_FL_LOG_ITEM_SELL_ITEM";
    case T1_FL_LOG_ITEM_USE_ITEM:                            return "T1_FL_LOG_ITEM_USE_ITEM";
    case T1_FL_LOG_ITEM_USE_ENERGY:                            return "T1_FL_LOG_ITEM_USE_ENERGY";
    case T1_FL_LOG_ITEM_USE_ENCHANT:                        return "T1_FL_LOG_ITEM_USE_ENCHANT";
    case T1_FL_LOG_ITEM_ADD_ITEM_BY_ADMIN:                    return "T1_FL_LOG_ITEM_ADD_ITEM_BY_ADMIN";
    case T1_FL_LOG_ITEM_DEL_ITEM_BY_ADMIN:                    return "T1_FL_LOG_ITEM_DEL_ITEM_BY_ADMIN";
    case T1_FL_LOG_ITEM_ADD_ITEM_BY_COMMAND:                return "T1_FL_LOG_ITEM_ADD_ITEM_BY_COMMAND";
    case T1_FL_LOG_ITEM_INSERT_TO_STORE:                    return "T1_FL_LOG_ITEM_INSERT_TO_STORE";
    case T1_FL_LOG_ITEM_DRAW_FROM_STORE:                    return "T1_FL_LOG_ITEM_DRAW_FROM_STORE";
    case T1_FL_LOG_ITEM_STORE_ITEM:                            return "T1_FL_LOG_ITEM_STORE_ITEM";
    case T1_FL_LOG_ITEM_USE_MIX:                            return "T1_FL_LOG_ITEM_USE_MIX";
    case T1_FL_LOG_ITEM_USE_CHANGE_CHARACTER_NAME_ORIGIN:    return "T1_FL_LOG_ITEM_USE_CHANGE_CHARACTER_NAME_ORIGIN";
    case T1_FL_LOG_ITEM_USE_CHANGE_CHARACTER_NAME_NEW:        return "T1_FL_LOG_ITEM_USE_CHANGE_CHARACTER_NAME_NEW";
    case T1_FL_LOG_ITEM_BAZAAR_SEND:                        return "T1_FL_LOG_ITEM_BAZAAR_SEND";
    case T1_FL_LOG_ITEM_BAZAAR_RECV:                        return "T1_FL_LOG_ITEM_BAZAAR_RECV";
    case T1_FL_LOG_ITEM_INSERT_TO_GUILDSTORE:                return "T1_FL_LOG_ITEM_INSERT_TO_GUILDSTORE";        
    case T1_FL_LOG_ITEM_DRAW_FROM_GUILDSTORE:                return "T1_FL_LOG_ITEM_DRAW_FROM_GUILDSTORE";        
    case T1_FL_LOG_ITEM_SPI:                                return "T1_FL_LOG_ITEM_SPI";                        
    case T1_FL_LOG_ITEM_GET_ITEM_BY_ITEMEVENT:                return "T1_FL_LOG_ITEM_GET_ITEM_BY_ITEMEVENT";        
    case T1_FL_LOG_WARPOINT:                                return "T1_FL_LOG_WARPOINT";                        

    case T1_FL_LOG_ARENA_TEAM_MATCH:                        return "T1_FL_LOG_ARENA_TEAM_MATCH";                
    case T1_FL_LOG_TUTORIAL_COMPLETION:                        return "T1_FL_LOG_TUTORIAL_COMPLETION";                

    case T1_FL_LOG_OUTPOST_START:                            return "T1_FL_LOG_OUTPOST_START";                    
    case T1_FL_LOG_OUTPOST_END:                                return "T1_FL_LOG_OUTPOST_END";                        
    case T1_FL_LOG_EXPEDIENCYFUND:                            return "T1_FL_LOG_EXPEDIENCYFUND";                    

    case T1_FL_LOG_SERVER_INFO_MAP:                            return "T1_FL_LOG_SERVER_INFO_MAP";
    case T1_FL_LOG_SERVER_INFO_TOTAL:                        return "T1_FL_LOG_SERVER_INFO_TOTAL";
    case T1_FL_LOG_HACKING_LOG:                                return "T1_FL_LOG_HACKING_LOG";

    case T1_FL_LOG_MONSTER_BOSS:                            return "T1_FL_LOG_MONSTER_BOSS";
    case T1_FL_LOG_START_FIELD_SERVER:                        return "T1_FL_LOG_START_FIELD_SERVER";

    case T1_FL_LOG_SERVER_INTEGRAION:                        return "T1_FL_LOG_SERVER_INTEGRAION";            
    case T1_FL_LOG_SERVER_INTEGRAION_GUILD_STORE_ITEMS:        return "T1_FL_LOG_SERVER_INTEGRAION_GUILD_STORE_ITEMS";    
    case T1_FL_LOG_SERVER_INTEGRAION_ONLY_CASH_STORE_ITEMS:    return "T1_FL_LOG_SERVER_INTEGRAION_ONLY_CASH_STORE_ITEMS";    

    case T1_FL_LOG_INFLWAR_START:                        return "T1_FL_LOG_INFLWAR_START";                    
    case T1_FL_LOG_INFLWAR_END:                            return "T1_FL_LOG_INFLWAR_END";                        
    case T1_FL_LOG_OUTPOSTWAR_RESET_START:                return "T1_FL_LOG_OUTPOSTWAR_RESET_START";            
    case T1_FL_LOG_OUTPOSTWAR_RESET_DESTROY:            return "T1_FL_LOG_OUTPOSTWAR_RESET_DESTROY";        
    case T1_FL_LOG_OUTPOSTWAR_RESET_COMPLETE:            return "T1_FL_LOG_OUTPOSTWAR_RESET_COMPLETE";        
    case T1_FL_LOG_OUTPOSTWAR_PROTECTOR_DESTROY:        return "T1_FL_LOG_OUTPOSTWAR_PROTECTOR_DESTROY";    
    case T1_FL_LOG_OUTPOSTWAR_SET_NEXTTIME:                return "T1_FL_LOG_OUTPOSTWAR_SET_NEXTTIME";            


    case T1_FL_LOG_POLL_VOTE:                            return "T1_FL_LOG_POLL_VOTE";
    case T1_FL_LOG_POLL_DELETE_LEADERCANDIDATE:            return "T1_FL_LOG_POLL_DELETE_LEADERCANDIDATE";

    case T1_FL_LOG_DISMEMBER_GUILD:                        return "T1_FL_LOG_DISMEMBER_GUILD";
    case T1_FL_LOG_NOTIFY_MSG_DELETE:                    return "T1_FL_LOG_NOTIFY_MSG_DELETE";        
    case T1_FL_LOG_USE_COUPON:                            return "T1_FL_LOG_USE_COUPON";                
        
    case T1_FL_LOG_ITEM_GIFT_SEND:                        return "T1_FL_LOG_ITEM_GIFT_SEND";            
    case T1_FL_LOG_ITEM_GIFT_RECV:                        return "T1_FL_LOG_ITEM_GIFT_RECV";            
    case T1_FL_LOG_ITEM_REARRANGE_DELETE_ITEM:            return "T1_FL_LOG_ITEM_REARRANGE_DELETE_ITEM";    
    case T1_FL_LOG_ITEM_REARRANGE_ITEM:                    return "T1_FL_LOG_ITEM_REARRANGE_ITEM";            
    case T1_FL_LOG_ITEM_ATTACH_ITEM:                    return "T1_FL_LOG_ITEM_ATTACH_ITEM";            
    case T1_FL_LOG_ITEM_DETACH_ITEM:                    return "T1_FL_LOG_ITEM_DETACH_ITEM";            
    case T1_FL_LOG_ITEM_DELETE:                            return "T1_FL_LOG_ITEM_DELETE";                    

    case T1_FL_LOG_LUCKY_DROP:                            return "T1_FL_LOG_LUCKY_DROP";                    

    case T1_FL_LOG_BLOCK_ACCOUNT:                        return "T1_FL_LOG_BLOCK_ACCOUNT";                
    case T1_FL_LOG_UNBLOCK_ACCOUNT:                        return "T1_FL_LOG_UNBLOCK_ACCOUNT";                

    case T1_FL_LOG_WAR_CONTRIBUTION:                    return "T1_FL_LOG_WAR_CONTRIBUTION";            
    case T1_FL_LOG_WAR_CONTRIBUTION_GEAR:                return "T1_FL_LOG_WAR_CONTRIBUTION_GEAR";        
    
    case T1_FL_LOG_INFINITY_START:                        return "T1_FL_LOG_INFINITY_START";    
    case T1_FL_LOG_INFINITY_START_ITEM:                    return "T1_FL_LOG_INFINITY_START_ITEM";    
    case T1_FL_LOG_INFINITY_CINEMA:                        return "T1_FL_LOG_INFINITY_CINEMA";    
    case T1_FL_LOG_MONSTER_SKILL:                        return "T1_FL_LOG_MONSTER_SKILL";    
    case T1_FL_LOG_HPACTION_TALK:                        return "T1_FL_LOG_HPACTION_TALK";    
    case T1_FL_LOG_INFINITY_FIN:                        return "T1_FL_LOG_INFINITY_FIN";    
    case T1_FL_LOG_INFINITY_FIN_ITEM:                    return "T1_FL_LOG_INFINITY_FIN_ITEM";    
    case T1_FL_LOG_INFINITY_FIN_ALIVE_KEY_MONSTER:        return "T1_FL_LOG_INFINITY_FIN_ALIVE_KEY_MONSTER";    
    case T1_FL_LOG_INFINITY_LEAVE_ITEM:                    return "T1_FL_LOG_INFINITY_LEAVE_ITEM";    
    case T1_FL_LOG_PET_LEVEL:                            return "T1_FL_LOG_PET_LEVEL"; 
    case T1_FL_LOG_CONNECT_USER_NEW:                    return "T1_FL_LOG_CONNECT_USER_NEW";    
    case T1_FL_LOG_CONNECT_USER_LOGIN:                    return "T1_FL_LOG_CONNECT_USER_LOGIN";    

    case T1_FL_LOG_STATISTICS_MONEY:                    return "T1_FL_LOG_STATISTICS_MONEY";        

    case T1_FL_LOG_FIXED_TERM_SHAPE_START:                return "T1_FL_LOG_FIXED_TERM_SHAPE_START";    
    case T1_FL_LOG_FIXED_TERM_SHAPE_END:                return "T1_FL_LOG_FIXED_TERM_SHAPE_END";        

    case T1_FL_LOG_COLLECTION_STATE:                    return "T1_FL_LOG_COLLECTION_STATE";            
    case T1_FL_LOG_COLLECTION_SHAPE_CHANGE:                return "T1_FL_LOG_COLLECTION_SHAPE_CHANGE";        
    case T1_FL_LOG_ACCOUNT_INFL_CHANGE_BY_ADMIN:        return "T1_FL_LOG_ACCOUNT_INFL_CHANGE_BY_ADMIN";
    case T1_FL_LOG_ITEM_TRY_ENCHANT_INFO:                return "T1_FL_LOG_ITEM_TRY_ENCHANT_INFO";        

    case T1_FL_LOG_ITEM_DISSOLUTION_ITEM:                return "T1_FL_LOG_ITEM_DISSOLUTION_ITEM";
    
    
    case T1_FL_LOG_LIVE_DELETED_CHARACTER:                return "T1_FL_LOG_LIVE_DELETED_CHARACTER";
    case T1_FL_LOG_EVENT_PARTICIPATION_RATE:            return "T1_FL_LOG_EVENT_PARTICIPATION_RATE";
    case T1_FL_LOG_ITEM_ENCHANT_BY_ADMIN:                return "T1_FL_LOG_ITEM_ENCHANT_BY_ADMIN";
    case T1_FL_LOG_ITEM_DISENCHANT_BY_ADMIN:            return "T1_FL_LOG_ITEM_DISENCHANT_BY_ADMIN";
    case T1_FL_LOG_MS_WAR:                                return "T1_FL_LOG_MS_WAR";
    case T1_FL_LOG_SP_WAR:                                return "T1_FL_LOG_SP_WAR";
    case T1_FL_LOG_ACCOUNTCONNECT:                        return "T1_FL_LOG_ACCOUNTCONNECT";
    case T1_FL_LOG_CHARDATA:                            return "T1_FL_LOG_CHARDATA";
    case T1_FL_LOG_ITEM:                                return "T1_FL_LOG_ITEM";
    case T1_FL_LOG_ITEMSTATE:                            return "T1_FL_LOG_ITEMSTATE";
    case T1_FL_LOG_SERVER:                                return "T1_FL_LOG_SERVER";

    default:                                            return "UNKNOWN_GAME_LOG_TYPE";
    }
}


///////////////////////////////////////////////////////////////////////////////
/// \fn            const char* GetItemUpdateTypeString(BYTE i_byItemUpdateTy)
/// \brief        
/// \author        cmkwon
/// \date        2006-10-27 ~ 2006-10-27
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
const char* GetItemUpdateTypeString(BYTE i_byItemUpdateTy)
{
    switch(i_byItemUpdateTy)
    {
    case IUT_GENERAL:                        return "IUT_GENERAL";
    case IUT_DROP_ITEM:                        return "IUT_DROP_ITEM";
    case IUT_MIXING:                        return "IUT_MIXING";
    case IUT_TRADE:                            return "IUT_TRADE";
    case IUT_SHOP:                            return "IUT_SHOP";
    case IUT_QUEST:                            return "IUT_QUEST";
    case IUT_ADMIN:                            return "IUT_ADMIN";
    case IUT_BULLET:                        return "IUT_BULLET";
    case IUT_SKILL:                            return "IUT_SKILL";
    case IUT_LOADING:                        return "IUT_LOADING";
    case IUT_AUCTION:                        return "IUT_AUCTION";
    case IUT_ENCHANT:                        return "IUT_ENCHANT";
    case IUT_USE_ITEM:                        return "IUT_USE_ITEM";
    case IUT_MGAME_EVENT:                    return "IUT_MGAME_EVENT";
    case IUT_USE_ENERGY:                    return "IUT_USE_ENERGY";
    case IUT_EXPIRE_CARD_ITEM:                return "IUT_EXPIRE_CARD_ITEM";
    case IUT_PENALTY_ON_DEAD:                return "IUT_PENALTY_ON_DEAD";
    case IUT_PENALTY_AGEAR_FUEL_ALLIN:        return "IUT_PENALTY_AGEAR_FUEL_ALLIN";
    case IUT_INFLUENCEWAR_KILLER_BONUS:        return "IUT_INFLUENCEWAR_KILLER_BONUS";
    case IUT_BONUS_ITEM:                    return "IUT_BONUS_ITEM";
    case IUT_BAZAAR_SELL:                    return "IUT_BAZAAR_SELL";
    case IUT_BAZAAR_BUY:                    return "IUT_BAZAAR_BUY";
    case IUT_RANDOMBOX:                        return "IUT_RANDOMBOX";
    case IUT_GIVEEVENTITEM:                    return "IUT_GIVEEVENTITEM";
    case IUT_GUILD_STORE:                    return "IUT_GUILD_STORE";
    case IUT_EXPIRE_ITEM:                    return "IUT_EXPIRE_ITEM";
    case IUT_STORE:                            return "IUT_STORE";
    case IUT_STORE_FEE:                        return "IUT_STORE_FEE";
    case IUT_ARENA_ITEM:                    return "IUT_ARENA_ITEM";
    case IUT_TUTORIAL_PAY_ITEM:                return "IUT_TUTORIAL_PAY_ITEM";
    case IUT_EXPEDIENCYFUND_PAYBACK:        return "IUT_EXPEDIENCYFUND_PAYBACK";
    case IUT_GIVEEVENTITEM_COUPONEVENT:        return "IUT_GIVEEVENTITEM_COUPONEVENT";        // 2008-01-10 by cmkwon, 아이템 이벤트 시스템에 신 쿠폰 시스템 추가 - 
    case IUT_DEPENDENCY_ITEM:                return "IUT_DEPENDENCY_ITEM";        // 2011-09-20 by hskim, 파트너 시스템 2차
    case IUT_WARP:                            return "IUT_WARP";                            // 2011-10-28 by hskim, EP4 [트리거 시스템] - 크리스탈 시스템 - 워프 입장 조건에 의한 아이템 삭제
    case IUT_DISSOLUTION_ITEM:                return "IUT_DISSOLUTION_ITEM";        // 2013-08-05 by bckim, 용해 대상아이템 로그 추가
    }

    return "UNKNOWN_ITEM_UPDATE_TYPE";
}


///////////////////////////////////////////////////////////////////////////////
/// \fn            const char* GetItemDeleteTypeString(BYTE i_byItemDeleteTy)
/// \brief        // 2008-01-23 by cmkwon, S_F, S_L: 장착/장착해제 게임 로그에 추가 - GetItemDeleteTypeString() 추가
/// \author        cmkwon
/// \date        2008-01-23 ~ 2008-01-23
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
const char* GetItemDeleteTypeString(BYTE i_byItemDeleteTy)
{
    switch(i_byItemDeleteTy)
    {
    case IDT_GENERAL:                        return "IDT_GENERAL";
    case IDT_EXPIRE_TIME:                    return "IDT_EXPIRE_TIME";
    case IDT_INVALID_ITEMNUM:                return "IDT_INVALID_ITEMNUM";        // 2009-11-20 by cmkwon, 소유 아이템 로딩시 유효하지 않은 아이템 처리 수정 - 
    case IDT_COUNTABLEITEM_ZERO:            return "IDT_COUNTABLEITEM_ZERO";    // 2009-11-20 by cmkwon, 소유 아이템 로딩시 유효하지 않은 아이템 처리 수정 - 
    }

    return "UNKNOWN_ITEM_DELETE_TYPE";
}

const char* GetErrorString(Err_t err)
{
    switch(err)
    {
    case ERR_NO_ERROR:                                    return "ERR_NO_ERROR";
    case ERR_COMMON_UNKNOWN_ERROR:                        return "ERR_COMMON_UNKNOWN_ERROR";
    case ERR_COMMON_LOGIN_FAILED:                        return "ERR_COMMON_LOGIN_FAILED";
    case ERR_COMMON_NO_SUCH_CHARACTER:                    return "ERR_COMMON_NO_SUCH_CHARACTER";
    case ERR_COMMON_CHARACTER_ALREADY_EXISTS:            return "ERR_COMMON_CHARACTER_ALREADY_EXISTS";
    case ERR_COMMON_TOO_LONG_PACKET:                    return "ERR_COMMON_TOO_LONG_PACKET";
    case ERR_COMMON_SERVICE_TEMPORARILY_PAUSED:            return "ERR_COMMON_SERVICE_TEMPORARILY_PAUSED";
    case ERR_COMMON_INVALID_CLIENT_VERSION:                return "ERR_COMMON_INVALID_CLIENT_VERSION";
    case ERR_COMMON_NOT_ALLOWED_TO_MYSELF:                return "ERR_COMMON_NOT_ALLOWED_TO_MYSELF";
    case ERR_COMMON_INVALID_BUILDINGKIND:                return "ERR_COMMON_INVALID_BUILDINGKIND";
    case ERR_COMMON_NO_SUCH_BUILDINGKIND:                return "ERR_COMMON_NO_SUCH_BUILDINGKIND";
    case ERR_COMMON_BAD_NETWORK:                        return "ERR_COMMON_BAD_NETWORK";
    case ERR_COMMON_CONNECTING_GAME:                    return "ERR_COMMON_CONNECTING_GAME";        // 2008-11-26 by cmkwon, 대만 Netpower_Tpe 웹에서 아이템 추가 프로시저 추가 - 
    case ERR_COMMON_DISABLE_CONTENT:                    return "ERR_COMMON_DISABLE_CONTENT";        // 2011-10-05 by hskim, 파트너 시스템 2차 - 컨텐츠 OFF 에러 메시지 용

    case ERR_DB_ACCOUNT_ALREAY_EXISTS:                    return "ERR_DB_ACCOUNT_ALREAY_EXISTS";
    case ERR_DB_ACCOUNT_CHARACTER_NO_MATCH:                return "ERR_DB_ACCOUNT_CHARACTER_NO_MATCH";
    case ERR_DB_INTEGRITY_CONSTRAINT_VIOLATION:            return "ERR_DB_INTEGRITY_CONSTRAINT_VIOLATION";
    case ERR_DB_INVALID_PARAMETER:                        return "ERR_DB_INVALID_PARAMETER";
    case ERR_DB_INVALID_UNIQUE_NUMBER:                    return "ERR_DB_INVALID_UNIQUE_NUMBER";
    case ERR_DB_NO_SUCH_ACCOUNT:                        return "ERR_DB_NO_SUCH_ACCOUNT";
    case ERR_DB_NUMERIC_VALUE_OUT_OF_RANGE:                return "ERR_DB_NUMERIC_VALUE_OUT_OF_RANGE";
    case ERR_DB_STRING_TOO_LONG:                        return "ERR_DB_STRING_TOO_LONG";
    case ERR_DB_NO_SUCH_STORE_ITEM:                        return "ERR_DB_NO_SUCH_STORE_ITEM";
    case ERR_DB_CANNOT_INSERT_DEFAULT_ITEMS:            return "ERR_DB_CANNOT_INSERT_DEFAULT_ITEMS";
    case ERR_DB_CONNECTION_ERROR:                        return "ERR_DB_CONNECTION_ERROR";
    case ERR_DB_EXECUTION_FAILED:                        return "ERR_DB_EXECUTION_FAILED";
    case ERR_DB_NO_SUCH_DATA:                            return "ERR_DB_NO_SUCH_DATA";        // 2008-06-12 by dhjin, EP3 데이타 값 없을 경우 에러 추가
    case ERR_DB_INSERT_QUERY_ERROR:                        return "ERR_DB_INSERT_QUERY_ERROR";        // 2008-11-26 by cmkwon, 대만 Netpower_Tpe 웹에서 아이템 추가 프로시저 추가 - 

    case ERR_PROTOCOL_INVALID_PROTOCOL_TYPE:            return "ERR_PROTOCOL_INVALID_PROTOCOL_TYPE";
    case ERR_PROTOCOL_INVALID_FIELD_DATA:                return "ERR_PROTOCOL_INVALID_FIELD_DATA";
    case ERR_PROTOCOL_INVALID_ACCOUNT_UNIQUENUMBER:        return "ERR_PROTOCOL_INVALID_ACCOUNT_UNIQUENUMBER";
    case ERR_PROTOCOL_INVALID_CHARACTER_UNIQUENUMBER:    return "ERR_PROTOCOL_INVALID_CHARACTER_UNIQUENUMBER";
    case ERR_PROTOCOL_INVALID_UNITKIND:                    return "ERR_PROTOCOL_INVALID_UNITKIND";
    case ERR_PROTOCOL_INVALID_CLIENTINDEX:                return "ERR_PROTOCOL_INVALID_CLIENTINDEX";
    case ERR_PROTOCOL_INVALID_PACKET:                    return "ERR_PROTOCOL_INVALID_PACKET";
    case ERR_PROTOCOL_INVALID_PACKET_SIZE:                return "ERR_PROTOCOL_INVALID_PACKET_SIZE";
    case ERR_PROTOCOL_INVALID_SEQUENCE_NUMBER:            return "ERR_PROTOCOL_INVALID_SEQUENCE_NUMBER";
    case ERR_PROTOCOL_INVALID_PEER:                        return "ERR_PROTOCOL_INVALID_PEER";
    case ERR_PROTOCOL_INVALID_STAT_VALUE_OR_KIND:        return "ERR_PROTOCOL_INVALID_STAT_VALUE_OR_KIND";
    case ERR_PROTOCOL_EMPTY_ACCOUNTNAME:                return "ERR_PROTOCOL_EMPTY_ACCOUNTNAME";
    case ERR_PROTOCOL_DUPLICATE_LOGIN:                    return "ERR_PROTOCOL_DUPLICATE_LOGIN";
    case ERR_PROTOCOL_NOT_LOGINED:                        return "ERR_PROTOCOL_NOT_LOGINED";
    case ERR_PROTOCOL_INVALID_SERVER_GROUP_NAME:        return "ERR_PROTOCOL_INVALID_SERVER_GROUP_NAME";
    case ERR_PROTOCOL_LIMIT_GROUP_USER_COUNT:            return "ERR_PROTOCOL_LIMIT_GROUP_USER_COUNT";
    case ERR_PROTOCOL_ACCOUNT_BLOCKED:                    return "ERR_PROTOCOL_ACCOUNT_BLOCKED";
    case ERR_PROTOCOL_INVALID_TARGET_INDEX:                return "ERR_PROTOCOL_INVALID_TARGET_INDEX";
    case ERR_PROTOCOL_INVALID_GETCHARACTER:                return "ERR_PROTOCOL_INVALID_GETCHARACTER";
    case ERR_PROTOCOL_INVALID_CHARACTER_STATE:                return "ERR_PROTOCOL_INVALID_CHARACTER_STATE";

    case ERR_PROTOCOL_ALREADY_MAX_CHARACTER:            return "ERR_PROTOCOL_ALREADY_MAX_CHARACTER";
    case ERR_PROTOCOL_EMPTY_CHARACTERNAME:                return "ERR_PROTOCOL_EMPTY_CHARACTERNAME";
    case ERR_PROTOCOL_STATE_MINUSVALUE:                    return "ERR_PROTOCOL_STATE_MINUSVALUE";
    case ERR_PROTOCOL_STATE_SUMISNOTBONUS:                return "ERR_PROTOCOL_STATE_SUMISNOTBONUS";
    case ERR_PROTOCOL_INVALID_GAMESTART:                return "ERR_PROTOCOL_INVALID_GAMESTART";
    case ERR_PROTOCOL_INVALID_SOCKET_FORNPC:            return "ERR_PROTOCOL_INVALID_SOCKET_FORNPC";
    case ERR_PROTOCOL_INVALID_CONNECT_GAMESTART:        return "ERR_PROTOCOL_INVALID_CONNECT_GAMESTART";
    case ERR_PROTOCOL_INVALID_CONNECT_WARP:                return "ERR_PROTOCOL_INVALID_CONNECT_WARP";
    case ERR_PROTOCOL_INVALID_POSITION:                    return "ERR_PROTOCOL_INVALID_POSITION";
    case ERR_PROTOCOL_INVALID_GAMEEND:                    return "ERR_PROTOCOL_INVALID_GAMEEND";
    case ERR_PROTOCOL_GAMESTARTROUTINE_FAILED:            return "ERR_PROTOCOL_GAMESTARTROUTINE_FAILED";
    case ERR_PROTOCOL_INVALID_CHARACTER_NAME:            return "ERR_PROTOCOL_INVALID_CHARACTER_NAME";
    case ERR_PROTOCOL_INVALID_PEER_CHARACTER:            return "ERR_PROTOCOL_INVALID_PEER_CHARACTER";
    case ERR_PROTOCOL_INVALID_AUTOSTAT_TYPE:            return "ERR_PROTOCOL_INVALID_AUTOSTAT_TYPE";
    case ERR_PROTOCOL_NOT_TUTORIAL_MAP:                    return "ERR_PROTOCOL_NOT_TUTORIAL_MAP";
    case ERR_PROTOCOL_RACE_PERMISSION_DENIED:            return "ERR_PROTOCOL_RACE_PERMISSION_DENIED";
    case ERR_PROTOCOL_NOT_ENOUGH_ELAPSE_TIME:            return "ERR_PROTOCOL_NOT_ENOUGH_ELAPSE_TIME";
    case ERR_PROTOCOL_INVALID_CHARACTERNAME:            return "ERR_PROTOCOL_INVALID_CHARACTERNAME";
    case ERR_PROTOCOL_CHARACTER_MODE_NOT_MATCHED:        return "ERR_PROTOCOL_CHARACTER_MODE_NOT_MATCHED";
    case ERR_PROTOCOL_UNKNOWN_CHARACTER_ACTION_TYPE:    return "ERR_PROTOCOL_UNKNOWN_CHARACTER_ACTION_TYPE";
    case ERR_PROTOCOL_BUILDING_KIND_NOT_MATCHED:        return "ERR_PROTOCOL_BUILDING_KIND_NOT_MATCHED";

    case ERR_PROTOCOL_INVALID_BODYCONDITION:            return "ERR_PROTOCOL_INVALID_BODYCONDITION";
    case ERR_PROTOCOL_INVALID_MAP_EVENT_INFO:            return "ERR_PROTOCOL_INVALID_MAP_EVENT_INFO";
    case ERR_PROTOCOL_MAP_ALREADY_SERVED_BY_FEILD_SERVER:        return "ERR_PROTOCOL_MAP_ALREADY_SERVED_BY_FEILD_SERVER";
    case ERR_PROTOCOL_NO_SUCH_FIELD_SERVER:                return "ERR_PROTOCOL_NO_SUCH_FIELD_SERVER";
    case ERR_PROTOCOL_NO_SUCH_MAP_SERVED:                return "ERR_PROTOCOL_NO_SUCH_MAP_SERVED";
    case ERR_PROTOCOL_NO_SUCH_CHANNEL:                    return "ERR_PROTOCOL_NO_SUCH_CHANNEL";
    case ERR_PROTOCOL_MAP_CHANNEL_NOT_MATCHED:            return "ERR_PROTOCOL_MAP_CHANNEL_NOT_MATCHED";
    case ERR_PROTOCOL_CANNOT_WARP:                        return "ERR_PROTOCOL_CANNOT_WARP";
    case ERR_PROTOCOL_CANNOT_WARP_INVALID_STATE:        return "ERR_PROTOCOL_CANNOT_WARP_INVALID_STATE";
    case ERR_PROTOCOL_DOING_WARP:        return "ERR_PROTOCOL_DOING_WARP";

    case ERR_PROTOCOL_INVALID_FIELDSERVER_CLIENT_STATE:        return "ERR_PROTOCOL_INVALID_FIELDSERVER_CLIENT_STATE";
    case ERR_PROTOCOL_INVALID_PRESERVER_CLIENT_STATE:    return "ERR_PROTOCOL_INVALID_PRESERVER_CLIENT_STATE";
    case ERR_PROTOCOL_NO_SUCH_SERVER_GROUP:                return "ERR_PROTOCOL_NO_SUCH_SERVER_GROUP";
    case ERR_PROTOCOL_FIELD_SERVER_ALREADY_REGISTERD:    return "ERR_PROTOCOL_FIELD_SERVER_ALREADY_REGISTERD";
    case ERR_PROTOCOL_FIELD_SERVER_ID_NOT_MATCHED:        return "ERR_PROTOCOL_FIELD_SERVER_ID_NOT_MATCHED";
    case ERR_PROTOCOL_ALL_FIELD_SERVER_NOT_ALIVE:        return "ERR_PROTOCOL_ALL_FIELD_SERVER_NOT_ALIVE";
    case ERR_PROTOCOL_NO_SUCH_SHOP:                        return "ERR_PROTOCOL_NO_SUCH_SHOP";
    case ERR_PROTOCOL_NO_SUCH_SHOP_ITEM:                return "ERR_PROTOCOL_NO_SUCH_SHOP_ITEM";
    case ERR_PROTOCOL_NOT_ENOUGH_MONEY:                    return "ERR_PROTOCOL_NOT_ENOUGH_MONEY";
    case ERR_PROTOCOL_ITEM_KIND_NOT_MATCHED:            return "ERR_PROTOCOL_ITEM_KIND_NOT_MATCHED";
    case ERR_PROTOCOL_NO_SUCH_ITEM:                        return "ERR_PROTOCOL_NO_SUCH_ITEM";
    case ERR_PROTOCOL_ITEM_AMOUNT_EXCEED:                return "ERR_PROTOCOL_ITEM_AMOUNT_EXCEED";
    case ERR_PROTOCOL_CLIENT_IP_NOT_MATCHED:            return "ERR_PROTOCOL_CLIENT_IP_NOT_MATCHED";
    case ERR_PROTOCOL_IMSERVER_ALREADY_CONNECTED:        return "ERR_PROTOCOL_IMSERVER_ALREADY_CONNECTED";
    case ERR_PROTOCOL_GENERAL_ITEM_ERROR:                return "ERR_PROTOCOL_GENERAL_ITEM_ERROR";
    case ERR_PROTOCOL_IM_SERVER_NOT_ALIVE:                return "ERR_PROTOCOL_IM_SERVER_NOT_ALIVE";
    case ERR_PROTOCOL_PARTY_MASTER_NOT_MATCHED:            return "ERR_PROTOCOL_PARTY_MASTER_NOT_MATCHED";
    case ERR_PROTOCOL_NO_SUCH_PARTY_MEMBER:                return "ERR_PROTOCOL_NO_SUCH_PARTY_MEMBER";
    case ERR_PROTOCOL_PARTY_PERMISSION_DENIED:            return "ERR_PROTOCOL_PARTY_PERMISSION_DENIED";
    case ERR_PROTOCOL_PARTY_MEMBER_ALREADY_EXISTS:        return "ERR_PROTOCOL_PARTY_MEMBER_ALREADY_EXISTS";
    case ERR_PROTOCOL_NO_SUCH_PARTY:                    return "ERR_PROTOCOL_NO_SUCH_PARTY";
    case ERR_PROTOCOL_CANNOT_INVITE_USER:                return "ERR_PROTOCOL_CANNOT_INVITE_USER";
    case ERR_PROTOCOL_GET_MEMBER_FAIL:                    return "ERR_PROTOCOL_GET_MEMBER_FAIL";
    case ERR_PROTOCOL_PARTY_ID_NOT_MATCHED:                return "ERR_PROTOCOL_PARTY_ID_NOT_MATCHED";
    case ERR_PROTOCOL_ALREADY_MAX_PARTY_MEMBER:            return "ERR_PROTOCOL_ALREADY_MAX_PARTY_MEMBER";
    case ERR_PROTOCOL_ALREADY_PARTY_BATTLE:                return "ERR_PROTOCOL_ALREADY_PARTY_BATTLE";
    case ERR_PROTOCOL_ALREADY_REQUESTING_PARTY_BATTLE:    return "ERR_PROTOCOL_ALREADY_REQUESTING_PARTY_BATTLE";
    case ERR_PROTOCOL_DOING_PARTY_BATTLE:                return "ERR_PROTOCOL_DOING_PARTY_BATTLE";
    case ERR_PROTOCOL_IMPOSSIBLE_IN_PARTY:                return "ERR_PROTOCOL_IMPOSSIBLE_IN_PARTY";

    case ERR_PROTOCOL_NOT_ENOUGH_PARTY_MEMBER:            return "ERR_PROTOCOL_NOT_ENOUGH_PARTY_MEMBER";
    case ERR_PROTOCOL_GUILD_NAME_ALREADY_EXISTS:        return "ERR_PROTOCOL_GUILD_NAME_ALREADY_EXISTS";
    case ERR_PROTOCOL_GUILD_MEMBER_ALREADY_EXISTS:        return "ERR_PROTOCOL_GUILD_MEMBER_ALREADY_EXISTS";
    case ERR_PROTOCOL_GUILD_PERMISSION_DENIED:            return "ERR_PROTOCOL_GUILD_PERMISSION_DENIED";
    case ERR_PROTOCOL_NOT_HAVE_GUILD:                    return "ERR_PROTOCOL_NOT_HAVE_GUILD";
    case ERR_PROTOCOL_GUILD_NOT_MATCHED:                return "ERR_PROTOCOL_GUILD_NOT_MATCHED";
    case ERR_PROTOCOL_NO_SUCH_GUILD_MEMBER:                return "ERR_PROTOCOL_NO_SUCH_GUILD_MEMBER";
    case ERR_PROTOCOL_NOT_HAVE_PARTY:                    return "ERR_PROTOCOL_NOT_HAVE_PARTY";
    case ERR_PROTOCOL_NO_SUCH_GUILD:                    return "ERR_PROTOCOL_NO_SUCH_GUILD";
    case ERR_PROTOCOL_NOT_GUILD_MASTER:                    return "ERR_PROTOCOL_NOT_GUILD_MASTER";
    case ERR_PROTOCOL_GUILD_CARD_ALREADY_USING:            return "ERR_PROTOCOL_GUILD_CARD_ALREADY_USING";
    case ERR_PROTOCOL_INVLID_GUILDNAME:                    return "ERR_PROTOCOL_INVLID_GUILDNAME";
    case ERR_PROTOCOL_NOT_GET_GUILDSTORE_MEMBER:        return "ERR_PROTOCOL_NOT_GET_GUILDSTORE_MEMBER";
    case ERR_PROTOCOL_DOING_GUILDWAR_BATTLE:            return "ERR_PROTOCOL_DOING_GUILDWAR_BATTLE";
    case ERR_PROTOCOL_INVLID_GuildMarkSize:                return "ERR_PROTOCOL_INVLID_GuildMarkSize";            // 2007-08-02 by cmkwon, 여단 마크 심사 시스템 구현 - 여단마크 사이즈 오류
    case ERR_PROTOCOL_OVER_MEMBERCOUNT:                    return "ERR_PROTOCOL_OVER_MEMBERCOUNT";        // 2008-05-27 by dhjin, EP3 여단 수정 사항 - 여단원 증가 캐쉬 아이템, 최대 여단원 수 넘김

    case ERR_PROTOCOL_TOO_FAR_TO_TRADE:                    return "ERR_PROTOCOL_TOO_FAR_TO_TRADE";
    case ERR_PROTOCOL_ALREADY_TRADING:                    return "ERR_PROTOCOL_ALREADY_TRADING";
    case ERR_PROTOCOL_PEER_TRADER_NOT_MATCHED:            return "ERR_PROTOCOL_PEER_TRADER_NOT_MATCHED";
    case ERR_PROTOCOL_TRADE_NOT_STARTED:                return "ERR_PROTOCOL_TRADE_NOT_STARTED";
    case ERR_PROTOCOL_INVALID_ITEMNUM:                    return "ERR_PROTOCOL_INVALID_ITEMNUM";
    case ERR_PROTOCOL_INVALID_ITEM_COUNT:                return "ERR_PROTOCOL_INVALID_ITEM_COUNT";
    case ERR_PROTOCOL_TRADE_ERROR:                        return "ERR_PROTOCOL_TRADE_ERROR";
    case ERR_PROTOCOL_INVALID_ITEM_KIND:                return "ERR_PROTOCOL_INVALID_ITEM_KIND";
    case ERR_PROTOCOL_MIN_TRADE_QUANTITY_ERROR:            return "ERR_PROTOCOL_MIN_TRADE_QUANTITY_ERROR";
    case ERR_PROTOCOL_ALREADY_TRADE_ITEM:                return "ERR_PROTOCOL_ALREADY_TRADE_ITEM";
    case ERR_PROTOCOL_NO_SUCH_SKILL:                    return "ERR_PROTOCOL_NO_SUCH_SKILL";
    case ERR_PROTOCOL_CANNOT_USE_SKILL:                    return "ERR_PROTOCOL_CANNOT_USE_SKILL";
    case ERR_PROTOCOL_INVALID_SKILLTYPE:                return "ERR_PROTOCOL_INVALID_SKILLTYPE";
    case ERR_PROTOCOL_NOT_ENOUGH_SP:                    return "ERR_PROTOCOL_NOT_ENOUGH_SP";
    case ERR_PROTOCOL_NO_SUCH_TIME_LIMIT_EVENT_IN_BUCKET:        return "ERR_PROTOCOL_NO_SUCH_TIME_LIMIT_EVENT_IN_BUCKET";
    case ERR_PROTOCOL_NO_SUCH_TIMER_EVENT_TYPE:            return "ERR_PROTOCOL_NO_SUCH_TIMER_EVENT_TYPE";
    case ERR_PROTOCOL_ALREADY_MAX_ENCHANT_PER_ITEM:        return "ERR_PROTOCOL_ALREADY_MAX_ENCHANT_PER_ITEM";
    case ERR_PROTOCOL_ALREADY_HAVE_RARE_FIX:            return "ERR_PROTOCOL_ALREADY_HAVE_RARE_FIX";
    case ERR_PROTOCOL_ALREADY_INITIALIZE_RARE_FIX:        return "ERR_PROTOCOL_ALREADY_INITIALIZE_RARE_FIX";
    case ERR_PROTOCOL_CANNOT_USE_ITEMKIND_PREVENTION_DELETE_ITEM:    return "ERR_PROTOCOL_CANNOT_USE_ITEMKIND_PREVENTION_DELETE_ITEM";
    case ERR_PROTOCOL_CANNOT_USE_ITEMKIND_PREVENTION_DELETE_ITEM_BY_STARTCOUNT:    return "ERR_PROTOCOL_CANNOT_USE_ITEMKIND_PREVENTION_DELETE_ITEM_BY_STARTCOUNT";    // 2009-01-19 by dhjin, 인첸트 확률 증가, 10인첸 파방 카드

    case ERR_PROTOCOL_NOT_ENOUGH_EP:                    return "ERR_PROTOCOL_NOT_ENOUGH_EP";
    case ERR_PROTOCOL_TOO_LONG_DISTANCE:                return "ERR_PROTOCOL_TOO_LONG_DISTANCE";
    case ERR_PROTOCOL_ALREADY_P2P_PK:                    return "ERR_PROTOCOL_ALREADY_P2P_PK";
    case ERR_PROTOCOL_TOO_FAR_TO_P2P_PK:                return "ERR_PROTOCOL_TOO_FAR_TO_P2P_PK";
    case ERR_PROTOCOL_HIGH_LEVEL_GAP_TO_P2P_PK:            return "ERR_PROTOCOL_HIGH_LEVEL_GAP_TO_P2P_PK";
    case ERR_PROTOCOL_NOT_DOING_P2P_PK:                    return "ERR_PROTOCOL_NOT_DOING_P2P_PK";
    case ERR_PROTOCOL_NOT_ALLOWED_ATTACK:                return "ERR_PROTOCOL_NOT_ALLOWED_ATTACK";
    case ERR_PROTOCOL_CANNOT_USE_ITEM:                    return "ERR_PROTOCOL_CANNOT_USE_ITEM";
    case ERR_PROTOCOL_ALREADY_HAVE_ITEM:                return "ERR_PROTOCOL_ALREADY_HAVE_ITEM";
    case ERR_PROTOCOL_ALREADY_HAVE_SKILL:                return "ERR_PROTOCOL_ALREADY_HAVE_SKILL";
    case ERR_PROTOCOL_INVALID_ITEM_WINDOW_POSITION:        return "ERR_PROTOCOL_INVALID_ITEM_WINDOW_POSITION";
    case ERR_PROTOCOL_ALREADY_ITEM_ACTIVATED:            return "ERR_PROTOCOL_ALREADY_ITEM_ACTIVATED";
    case ERR_PROTOCOL_NO_SUCH_DES_PARAM:                return "ERR_PROTOCOL_NO_SUCH_DES_PARAM";
    case ERR_PROTOCOL_NOT_ENOUGH_BULLET:                return "ERR_PROTOCOL_NOT_ENOUGH_BULLET";
    case ERR_PROTOCOL_ITEM_CANNOT_TRANSFER:                return "ERR_PROTOCOL_ITEM_CANNOT_TRANSFER";
    case ERR_PROTOCOL_NO_SUCH_MIXING_INFO:                return "ERR_PROTOCOL_NO_SUCH_MIXING_INFO";
    case ERR_PROTOCOL_ITEM_OVER_WEIGHT:                    return "ERR_PROTOCOL_ITEM_OVER_WEIGHT";
    case ERR_PROTOCOL_DO_NOT_HAVE_LINK_ITEM:            return "ERR_PROTOCOL_DO_NOT_HAVE_LINK_ITEM";
    case ERR_PROTOCOL_INVENTORY_FULL_ITEM:                return "ERR_PROTOCOL_INVENTORY_FULL_ITEM";
    case ERR_PROTOCOL_STORE_FULL_ITEM:                    return "ERR_PROTOCOL_STORE_FULL_ITEM";
    case ERR_PROTOCOL_ALREADY_USE_ITEM:                    return "ERR_PROTOCOL_ALREADY_USE_ITEM";
    case ERR_PROTOCOL_ALREADY_USE_OTHER_ITEM:            return "ERR_PROTOCOL_ALREADY_USE_OTHER_ITEM";
    case ERR_PROTOCOL_STAT_ERROR_STATE:                    return "ERR_PROTOCOL_STAT_ERROR_STATE";
    case ERR_PROTOCOL_STAT_INITIALIZE_STATE:            return "ERR_PROTOCOL_STAT_INITIALIZE_STATE";
    case ERR_PROTOCOL_AREADY_USING_HP_UP_ITEM:            return "ERR_PROTOCOL_AREADY_USING_HP_UP_ITEM";
    case ERR_PROTOCOL_AREADY_USING_DP_UP_ITEM:            return "ERR_PROTOCOL_AREADY_USING_DP_UP_ITEM";
    case ERR_PROTOCOL_AREADY_USING_SP_UP_ITEM:            return "ERR_PROTOCOL_AREADY_USING_SP_UP_ITEM";
    case ERR_PROTOCOL_AREADY_USING_EP_UP_ITEM:            return "ERR_PROTOCOL_AREADY_USING_EP_UP_ITEM";
    case ERR_PROTOCOL_AREADY_FULL_HP:                    return "ERR_PROTOCOL_AREADY_FULL_HP";
    case ERR_PROTOCOL_AREADY_FULL_DP:                    return "ERR_PROTOCOL_AREADY_FULL_DP";
    case ERR_PROTOCOL_AREADY_FULL_SP:                    return "ERR_PROTOCOL_AREADY_FULL_SP";
    case ERR_PROTOCOL_AREADY_FULL_EP:                    return "ERR_PROTOCOL_AREADY_FULL_EP";
    case ERR_PROTOCOL_CANNOT_USEITEM_IN_PARTY:            return "ERR_PROTOCOL_CANNOT_USEITEM_IN_PARTY";
    case ERR_PROTOCOL_CANNOT_IN_CITY_MAP_CHANNEL:        return "ERR_PROTOCOL_CANNOT_IN_CITY_MAP_CHANNEL";
    case ERR_PROTOCOL_CHANNEL_USER_OVERFLOW:            return "ERR_PROTOCOL_CHANNEL_USER_OVERFLOW";
    case ERR_PROTOCOL_NO_SUCH_EVENT_AREA_INDEX:            return "ERR_PROTOCOL_NO_SUCH_EVENT_AREA_INDEX";
    case ERR_PROTOCOL_MAX_ITEM_COUNTS_OVER:                return "ERR_PROTOCOL_MAX_ITEM_COUNTS_OVER";
    case ERR_PROTOCOL_NOTIFY_MAX_ITEM_COUNTS_OVER:        return "ERR_PROTOCOL_NOTIFY_MAX_ITEM_COUNTS_OVER";
    case ERR_PROTOCOL_CANNOT_UNWEAR_ARMOR:                return "ERR_PROTOCOL_CANNOT_UNWEAR_ARMOR";
    case ERR_PROTOCOL_NO_SUCH_BULLET_ITEM:                return "ERR_PROTOCOL_NO_SUCH_BULLET_ITEM";
    case ERR_PROTOCOL_CANNOT_ENCHANT_ITEM:                return "ERR_PROTOCOL_CANNOT_ENCHANT_ITEM";
    case ERR_PROTOCOL_BAZAAR_NO_SUCH_ITEM:                return "ERR_PROTOCOL_BAZAAR_NO_SUCH_ITEM";
    case ERR_PROTOCOL_GUILD_STORE_FULL_ITEM:            return "ERR_PROTOCOL_GUILD_STORE_FULL_ITEM";
    case ERR_PROTOCOL_CANNOT_USE_ITEM_IN_ARENA:            return "ERR_PROTOCOL_CANNOT_USE_ITEM_IN_ARENA";
    case ERR_PROTOCOL_INVALID_MIXING_INFO:                return "ERR_PROTOCOL_INVALID_MIXING_INFO";        // 2009-10-01 by cmkwon, 그래픽 리소스 변경 관련 초기화 기능 구현 - 
    case ERR_PROTOCOL_NOT_WEARING_STATE:                return "ERR_PROTOCOL_NOT_WEARING_STATE";            // 2009-09-09 ~ 2010-02-10 by dhjin, 인피니티 - 발동류장착아이템, 장착 상태가 아니다. 
    case ERR_PROTOCOL_INVALID_INVOKING_TYPE:            return "ERR_PROTOCOL_INVALID_INVOKING_TYPE";        // 2009-09-09 ~ 2010-02-10 by dhjin, 인피니티 - 발동류장착아이템, 발동 타잎 오류
    case ERR_PROTOCOL_FAIL_INVOKING_BY_RATE:            return "ERR_PROTOCOL_FAIL_INVOKING_BY_RATE";        // 2009-09-09 ~ 2010-02-10 by dhjin, 인피니티 - 발동류장착아이템, 발동 미스
    case ERR_PROTOCOL_FAIL_INVOKING_BY_REQITEMNUM:        return "ERR_PROTOCOL_FAIL_INVOKING_BY_REQITEMNUM";    // 2009-09-09 ~ 2010-02-10 by dhjin, 인피니티 - 발동류장착아이템, 발동 조건 값 오류
    case ERR_PROTOCOL_COOLING_TIME_INVOKING_ITEM:        return "ERR_PROTOCOL_COOLING_TIME_INVOKING_ITEM";    // 2009-09-09 ~ 2010-02-10 by dhjin, 인피니티 - 발동류장착아이템, 발동류 아이템 쿨타임중
    case ERR_PROTOCOL_CANNOT_RARE_ITEM:                    return "ERR_PROTOCOL_CANNOT_RARE_ITEM";    // 2009-09-09 ~ 2010-02-10 by dhjin, 인피니티 - 발동류장착아이템, 발동류 아이템 쿨타임중
    case ERR_PROTOCOL_NOT_ACTIVATED_ITEM:                return "ERR_PROTOCOL_NOT_ACTIVATED_ITEM";            // 2011-09-20 by hskim, 파트너 시스템 2차 - 활성화 되지 않은 아이템
    case ERR_PROTOCOL_CANNOT_ATTACHED_ITEM:                return "ERR_PROTOCOL_CANNOT_ATTACHED_ITEM";            // 2011-09-20 by hskim, 파트너 시스템 2차 - 장착되지 않은 아이템
    case ERR_PROTOCOL_CANNOT_ENCHANT_MORE_THEN_10:            return "ERR_PROTOCOL_CANNOT_ENCHANT_MORE_THEN_10";        // 2011-10-19 by hskim, EP4 [Free to play] - 10 회 인첸트 아이템 기능 구현 - 10 인챈트 이상 아이템은 강화할 수 없음
    case ERR_PROTOCOL_CANNOT_ENCHANT_DIFFERENCE_ATTRIBUTE:    return "ERR_PROTOCOL_CANNOT_ENCHANT_DIFFERENCE_ATTRIBUTE";        // 2011-10-19 by hskim, EP4 [Free to play] - 10 회 인첸트 아이템 기능 구현 - 인챈트 속성이 틀려 인챈트 할 수 없음
    case ERR_PROTOCOL_CANNOT_ENCHANT_LACK_REQUIREMENT:        return "ERR_PROTOCOL_CANNOT_ENCHANT_LACK_REQUIREMENT";            // 2011-10-19 by hskim, EP4 [Free to play] - 접두/접미 고정 옵션 - 요구조건에 부적함
    case ERR_PROTOCOL_REQ_EXPERIENCE_NOT_MATCHED:        return "ERR_PROTOCOL_REQ_EXPERIENCE_NOT_MATCHED";
    case ERR_PROTOCOL_REQ_RACE_NOT_MATCHED:                return "ERR_PROTOCOL_REQ_RACE_NOT_MATCHED";
    case ERR_PROTOCOL_REQ_ATTACK_PART_NOT_MATCHED:        return "ERR_PROTOCOL_REQ_ATTACK_PART_NOT_MATCHED";
    case ERR_PROTOCOL_REQ_DEFENSE_PART_NOT_MATCHED:        return "ERR_PROTOCOL_REQ_DEFENSE_PART_NOT_MATCHED";
    case ERR_PROTOCOL_REQ_FUEL_PART_NOT_MATCHED:        return "ERR_PROTOCOL_REQ_FUEL_PART_NOT_MATCHED";
    case ERR_PROTOCOL_REQ_SOUL_PART_NOT_MATCHED:        return "ERR_PROTOCOL_REQ_SOUL_PART_NOT_MATCHED";
    case ERR_PROTOCOL_REQ_SHIELD_PART_NOT_MATCHED:        return "ERR_PROTOCOL_REQ_SHIELD_PART_NOT_MATCHED";
    case ERR_PROTOCOL_REQ_DODGE_PART_NOT_MATCHED:        return "ERR_PROTOCOL_REQ_DODGE_PART_NOT_MATCHED";
    case ERR_PROTOCOL_REQ_UNITKIND_NOT_MATCHED:            return "ERR_PROTOCOL_REQ_UNITKIND_NOT_MATCHED";
    case ERR_PROTOCOL_REQ_LEVEL_NOT_MATCHED:            return "ERR_PROTOCOL_REQ_LEVEL_NOT_MATCHED";
    case ERR_PROTOCOL_REQ_MINLEVEL_NOT_MATCHED:            return "ERR_PROTOCOL_REQ_MINLEVEL_NOT_MATCHED";
    case ERR_PROTOCOL_REQ_MAXLEVEL_NOT_MATCHED:            return "ERR_PROTOCOL_REQ_MAXLEVEL_NOT_MATCHED";
    case ERR_PROTOCOL_REQ_ITEM_NOT_MATCHED:                return "ERR_PROTOCOL_REQ_ITEM_NOT_MATCHED";
    case ERR_PROTOCOL_REQ_QUEST_NOT_MATCHED:            return "ERR_PROTOCOL_REQ_QUEST_NOT_MATCHED";
    case ERR_PROTOCOL_REQ_NUM_OF_CHARACTER_NOT_MATCHED:        return "ERR_PROTOCOL_REQ_NUM_OF_CHARACTER_NOT_MATCHED";
    case ERR_PROTOCOL_TOO_FAR_TO_DO:                    return "ERR_PROTOCOL_TOO_FAR_TO_DO";
    case ERR_PROTOCOL_NO_SUCH_REQUEST_TYPE:                return "ERR_PROTOCOL_NO_SUCH_REQUEST_TYPE";
    case ERR_PROTOCOL_LOW_PROBABILITY:                    return "ERR_PROTOCOL_LOW_PROBABILITY";
    case ERR_PROTOCOL_REQ_NOT_CITY_MAP_CHANNEL:            return "ERR_PROTOCOL_REQ_NOT_CITY_MAP_CHANNEL";
    case ERR_PROTOCOL_REQ_MUST_RELEASE_ALL_ITEM:        return "ERR_PROTOCOL_REQ_MUST_RELEASE_ALL_ITEM";
    case ERR_PROTOCOL_REQ_WARP_REQUIREMENTS_NOT_MATCHED:    return "ERR_PROTOCOL_REQ_WARP_REQUIREMENTS_NOT_MATCHED";
    case ERR_PROTOCOL_REQ_INFLUENCE_NOT_MATCHED:        return "ERR_PROTOCOL_REQ_INFLUENCE_NOT_MATCHED";
    case ERR_PROTOCOL_REQ_MAP_OBJECT_NOT_MATCHED:        return "ERR_PROTOCOL_REQ_MAP_WARP_TARGET_NOT_MATCHED";
    case ERR_PROTOCOL_REQ_MAP_AREA_NOT_MATCHED:            return "ERR_PROTOCOL_REQ_MAP_AREA_NOT_MATCHED";
    case ERR_PROTOCOL_REQ_MONSTER_NOT_MATCHED:            return "ERR_PROTOCOL_REQ_MONSTER_NOT_MATCHED";
    case ERR_PROTOCOL_REQ_PARTYMEMBERS_NOT_MATCHED:        return "ERR_PROTOCOL_REQ_PARTYMEMBERS_NOT_MATCHED";
    case ERR_PROTOCOL_REQ_PARTYMEMBERS_LEVEL_NOT_MATCHED:    return "ERR_PROTOCOL_REQ_PARTYMEMBERS_LEVEL_NOT_MATCHED";
    case ERR_PROTOCOL_REQ_PARTYMEMBERS_SOMEONE_CANNOT_WARP:    return "ERR_PROTOCOL_REQ_PARTYMEMBERS_SOMEONE_CANNOT_WARP";
    case ERR_PROTOCOL_REQ_PARTYMEMBERS_SOMEONE_NOT_COMPLETION:    return "ERR_PROTOCOL_REQ_PARTYMEMBERS_SOMEONE_NOT_COMPLETION";
    case ERR_PROTOCOL_REQ_MIX_ITEM_NOT_MATCHED:            return "ERR_PROTOCOL_REQ_MIX_ITEM_NOT_MATCHED";        // 2007-04-02 by cmkwon
    case ERR_PROTOCOL_REQ_PW_NOT_MATCHED:                return "ERR_PROTOCOL_REQ_PW_NOT_MATCHED";        // 2008-06-03 by dhjin, EP3 편대 수정 - PW오류
    case ERR_PROTOCOL_NOT_WARP_BURNING_MAP:                return "ERR_PROTOCOL_NOT_WARP_BURNING_MAP";        // 2010-11-24 by shcho, 콜오브 히어로 사용시 버닝타임 맵이면 소환불가 처리

    case ERR_PROTOCOL_NO_SUCH_CHARACTER_QUEST:            return "ERR_PROTOCOL_NO_SUCH_CHARACTER_QUEST";
    case ERR_PROTOCOL_QUEST_ALREADY_COMPLETED:            return "ERR_PROTOCOL_QUEST_ALREADY_COMPLETED";
    case ERR_PROTOCOL_QUEST_NOT_IN_PROGRESS:            return "ERR_PROTOCOL_QUEST_NOT_IN_PROGRESS";
    case ERR_PROTOCOL_QUEST_RESULT_PROCESS_FAILED:        return "ERR_PROTOCOL_QUEST_RESULT_PROCESS_FAILED";
    case ERR_PROTOCOL_QUEST_GENERAL_ERROR:                return "ERR_PROTOCOL_QUEST_GENERAL_ERROR";
    case ERR_PROTOCOL_TIME_LIMIT_ERROR:                    return "ERR_PROTOCOL_TIME_LIMIT_ERROR";
    case ERR_PROTOCOL_REQ_NOT_GUILD_MASTER:                return "ERR_PROTOCOL_REQ_NOT_GUILD_MASTER";
    case ERR_PROTOCOL_QUEST_ALREADY_EXIST_CITYWAR:        return "ERR_PROTOCOL_QUEST_ALREADY_EXIST_CITYWAR";
    case ERR_PROTOCOL_QUEST_NEVER_COMPLITION_BY_USER:    return "ERR_PROTOCOL_QUEST_NEVER_COMPLITION_BY_USER";
    case ERR_PROTOCOL_QUEST_INVALID_CITYWAR_QUEST_INDEX:    return "ERR_PROTOCOL_QUEST_INVALID_CITYWAR_QUEST_INDEX";
    case ERR_PROTOCOL_QUEST_CANNOT_CITYWAR_QUEST:        return "ERR_PROTOCOL_QUEST_CANNOT_CITYWAR_QUEST";
    case ERR_PROTOCOL_QUEST_CITYWAR_MGAMESERVERID_NOT_MATCHED:        return "ERR_PROTOCOL_QUEST_CITYWAR_MGAMESERVERID_NOT_MATCHED";
    case ERR_PROTOCOL_QUEST_COUPON_INVALID_NUMBER:        return "ERR_PROTOCOL_QUEST_COUPON_INVALID_NUMBER";
    case ERR_PROTOCOL_QUEST_COUPON_ALREADY_USED:        return "ERR_PROTOCOL_QUEST_COUPON_ALREADY_USED";
    case ERR_PROTOCOL_QUEST_COUPON_EXPIRED:                return "ERR_PROTOCOL_QUEST_COUPON_EXPIRED";
    case ERR_PROTOCOL_QUEST_COUPON_USE_ERROR:            return "ERR_PROTOCOL_QUEST_COUPON_USE_ERROR";
    case ERR_PROTOCOL_QUEST_NO_LEVEL_UP:                return "ERR_PROTOCOL_QUEST_NO_LEVEL_UP";
    case ERR_PROTOCOL_QUEST_IS_IN_PROGRESS:                return "ERR_PROTOCOL_QUEST_IS_IN_PROGRESS";

    case ERR_PROTOCOL_QUEST_SERVICE_PAUSED:                return "ERR_PROTOCOL_QUEST_SERVICE_PAUSED";

    case ERR_PROTOCOL_NO_SUCH_MONSTER_INFO:                return "ERR_PROTOCOL_NO_SUCH_MONSTER_INFO";
    case ERR_PROTOCOL_NO_SUCH_MAPOBJECT_INFO:            return "ERR_PROTOCOL_NO_SUCH_MAPOBJECT_INFO";
    case ERR_PROTOCOL_NO_SUCH_QUEST_INFO:                return "ERR_PROTOCOL_NO_SUCH_QUEST_INFO";
    case ERR_PROTOCOL_NO_SUCH_ITEM_INFO:                return "ERR_PROTOCOL_NO_SUCH_ITEM_INFO";

    case ERR_PROTOCOL_SELECTIVE_SHUTDOWN_NOT_ALLOWED_TIME:        return "ERR_PROTOCOL_SELECTIVE_SHUTDOWN_NOT_ALLOWED_TIME";        // 2012-07-11 by hskim, 선택적 셧다운 - 현재 시간에는 게임을 플레이 할수 없음
    case ERR_PROTOCOL_SELECTIVE_SHUTDOWN_APPLY_LOGOUT:            return "ERR_PROTOCOL_SELECTIVE_SHUTDOWN_APPLY_LOGOUT";            // 2012-07-11 by hskim, 선택적 셧다운 - 선택적 셧다운 적용

    case ERR_CHAT_CHARACTER_NAME_NOT_MATCHED:            return "ERR_CHAT_CHARACTER_NAME_NOT_MATCHED";
    case ERR_CHAT_NOT_ALLOWED_STRING:                    return "ERR_CHAT_NOT_ALLOWED_STRING";
    case ERR_CHAT_PERMISSION_DENIED:                    return "ERR_CHAT_PERMISSION_DENIED";
    case ERR_CHAT_INVALID_COMMAND:                        return "ERR_CHAT_INVALID_COMMAND";
    case ERR_CHAT_CHAT_NOT_TRANSFERED:                    return "ERR_CHAT_CHAT_NOT_TRANSFERED";
    case ERR_CHAT_CHAT_BLOCKED:                            return "ERR_CHAT_CHAT_BLOCKED";
    case ERR_FRIEND_INVALID_CHARACTER_NAME:                return "ERR_FRIEND_INVALID_CHARACTER_NAME";
    case ERR_FRIEND_REGISTERED_PEER_REJECTLIST:            return "ERR_FRIEND_REGISTERED_PEER_REJECTLIST";
    case ERR_FRIEND_ALREADY_REGISTERED:                    return "ERR_FRIEND_ALREADY_REGISTERED";
    case ERR_FRIEND_ALREADY_MAX_LIST:                    return "ERR_FRIEND_ALREADY_MAX_LIST";
    case ERR_FRIEND_NO_SUCH_CHARACTER_IN_LIST:            return "ERR_FRIEND_NO_SUCH_CHARACTER_IN_LIST";
    case ERR_COUNTDOWN_INVALID_COUNTDOWN_TYPE:            return "ERR_COUNTDOWN_INVALID_COUNTDOWN_TYPE";
    case ERR_COUNTDOWN_NOT_ENOUGH_TIME:                    return "ERR_COUNTDOWN_NOT_ENOUGH_TIME";
    case ERR_INTERVAL_SYSTEM_SECOND:                    return "ERR_INTERVAL_SYSTEM_SECOND";            // 2008-08-18 by dhjin, 1초 텀 시스템 기획안

    case ERR_CITYWAR_NO_SUCH_CITYWAR_INFO:                return "ERR_CITYWAR_NO_SUCH_CITYWAR_INFO";
    case ERR_CITYWAR_NOT_MATCHED_CITY_MAP_INDEX:        return "ERR_CITYWAR_NOT_MATCHED_CITY_MAP_INDEX";
    case ERR_CITYWAR_SETOCCUPYINFO_PERMISSION_DENIED:    return "ERR_CITYWAR_SETOCCUPYINFO_PERMISSION_DENIED";
    case ERR_CITYWAR_SETOCCUPYINFO_IMPOSSIBLE_STATE:    return "ERR_CITYWAR_SETOCCUPYINFO_IMPOSSIBLE_STATE";
    case ERR_CITYWAR_SETOCCUPYINFO_IMPOSSIBLE_TIME:        return "ERR_CITYWAR_SETOCCUPYINFO_IMPOSSIBLE_TIME";
    case ERR_CITYWAR_SETOCCUPYINFO_INVALID_WARTIME:        return "ERR_CITYWAR_SETOCCUPYINFO_INVALID_WARTIME";

    case ERR_CASH_mgQueryMall_ERROR:                    return "ERR_CASH_mgQueryMall_ERROR";
    case ERR_CASH_mgQueryMall_RESULT_CANNOT_PAYMENT:    return "ERR_CASH_mgQueryMall_RESULT_CANNOT_PAYMENT";
    case ERR_CASH_mgQueryMall_RESULT_NO_ACCOUNT:        return "ERR_CASH_mgQueryMall_RESULT_NO_ACCOUNT";
    case ERR_CASH_NOT_CHARGE_USER:                        return "ERR_CASH_NOT_CHARGE_USER";
    case ERR_CASH_INVALID_EXT_ACCOUNTID_NUM:            return "ERR_CASH_INVALID_EXT_ACCOUNTID_NUM";
    case ERR_CASH_INVALID_ITEM_INFORMATION:                return "ERR_CASH_INVALID_ITEM_INFORMATION";
    case ERR_CASH_NORMAL_ERROR:                            return "ERR_CASH_NORMAL_ERROR";
    case ERR_CASH_CASH_SHOP_NO_SERVICE:                    return "ERR_CASH_CASH_SHOP_NO_SERVICE";
    case ERR_CASH_LIBRARY_INIT_ERROR:                    return "ERR_CASH_LIBRARY_INIT_ERROR";        // 2007-08-22 by cmkwon, 중국 Yetime 빌링 라이브러리 적용 -
    case ERR_CASH_INVALID_GIVE_TARGET_ACCOUNT:            return "ERR_CASH_INVALID_GIVE_TARGET_ACCOUNT";        // 2009-08-18 by cmkwon, 예당 캐쉬?블럭 계정 체크 추가 - 

    case ERR_INVALID_CHARACTER:                            return "ERR_INVALID_CHARACTER";
    case ERR_INVALID_PEER_CHARACTER:                    return "ERR_INVALID_PEER_CHARACTER";
    case ERR_WARPING:                                    return "ERR_WARPING";
    case ERR_IN_PARTY_BATTLE:                            return "ERR_IN_PARTY_BATTLE";
    case ERR_REQ_WARP_COMPLETIONQUEST_NOT_MATCHED:        return "ERR_REQ_WARP_COMPLETIONQUEST_NOT_MATCHED";
    case ERR_REQ_CHOOSE_INFLUENCE_TYPE:                    return "ERR_REQ_CHOOSE_INFLUENCE_TYPE";
    case ERR_CANNOT_WARP_CONNFLICT_AREA:                return "ERR_CANNOT_WARP_INFLICT_AREA";
    case ERR_CANNOT_WARP_POSSESSED_CONNFLICT_AREA:        return "ERR_CANNOT_WARP_POSSESSED_CONNFLICT_AREA";
    case ERR_CANNOT_WARP_BY_TRADING:                    return "ERR_CANNOT_WARP_BY_TRADING";
    case ERR_CANNOT_WARP_BY_SHOPPING:                    return "ERR_CANNOT_WARP_BY_SHOPPING";
    case ERR_CANNOT_ATTACHED_USING_PREMIUM_CARD:        return "ERR_CANNOT_ATTACHED_USING_PREMIUM_CARD";
    case ERR_CANNOT_BUY_PREMIUM_CARD_USING_SPEC_ITEM:    return "ERR_CANNOT_BUY_PREMIUM_CARD_USING_SPEC_ITEM";
    case ERR_EXPIRED_ITEM:                                return "ERR_EXPIRED_ITEM";
    case ERR_FAILURE_USE_RESTORE_ITEM:                    return "ERR_FAILURE_USE_RESTORE_ITEM";
    case ERR_DOING_GAMEEVENT:                            return "ERR_DOING_GAMEEVENT";
    case ERR_INVALID_GAMEUSER:                            return "ERR_INVALID_GAMEUSER";
    case ERR_INVALID_CALL_WARP_EVENT_ID:                return "ERR_INVALID_CALL_WARP_EVENT_ID";
    case ERR_TIMEOVER_CALL_WARP_EVENT:                    return "ERR_TIMEOVER_CALL_WARP_EVENT";
    case ERR_CANNOT_PUT_ITEM:                            return "ERR_CANNOT_PUT_ITEM";
    case ERR_OVER_COUNT:                                return "ERR_OVER_COUNT";
    case ERR_INVALID_BAZAAR:                            return "ERR_INVALID_BAZAAR";
    case ERR_USING_BAZAAR:                                return "ERR_USING_BAZAAR";
    case ERR_INVISIBLE_STATE:                            return "ERR_INVISIBLE_STATE";
    case ERR_DOING_SERVER_SHUTDOWN:                        return "ERR_DOING_SERVER_SHUTDOWN";
    case ERR_NO_SEARCH_CHARACTER:                        return "ERR_NO_SEARCH_CHARACTER";
    case ERR_NO_MEMBERSHIP_USER:                        return "ERR_NO_MEMBERSHIP_USER";
    case ERR_ALREADY_OPEN_OTHER_STORE:                    return "ERR_ALREADY_OPEN_OTHER_STORE";        // 2006-09-24 by cmkwon
    case ERR_ALREADY_LOADING_GUILD_STORE:                return "ERR_ALREADY_LOADING_GUILD_STORE";    // 2006-09-25 by cmkwon
    case ERR_NO_SUCH_GUILD_STORE:                        return "ERR_NO_SUCH_GUILD_STORE";            // 2006-09-25 by cmkwon
    case ERR_PERMISSION_DENIED:                            return "ERR_PERMISSION_DENIED";                // 2006-09-27 by cmkwon
    case ERR_INVALID_EXCUTE_PARAMETER_COUNT:            return "ERR_INVALID_EXCUTE_PARAMETER_COUNT";        // 2006-10-02 by cmkwon
    case ERR_INVALID_EXCUTE_PARAMETER:                    return "ERR_INVALID_EXCUTE_PARAMETER";                // 2006-10-02 by cmkwon
    case ERR_NOT_RANK_DRAW_GUILD_STORE:                    return "ERR_NOT_RANK_DRAW_GUILD_STORE";                // 2006-10-09 by dhjin
    case ERR_CANNOT_BOOSTER:                            return "ERR_CANNOT_BOOSTER";                // 2006-10-09 by dhjin
    case ERR_NOT_INVISIBLE_STATE:                        return "ERR_NOT_INVISIBLE_STATE";            // 2007-04-02 by cmkwon
    case ERR_JAMBOREE_NO_SUCH_CHARACTER:                return "ERR_JAMBOREE_NO_SUCH_CHARACTER";        // 2007-04-09 by cmkwon
    case ERR_JAMBOREE_FAIL_CREATE_CHARACTER:            return "ERR_JAMBOREE_FAIL_CREATE_CHARACTER";    // 2007-04-09 by cmkwon
    case ERR_BOSS_STRATEGYPOINT_STATE_SUMMON:            return "ERR_BOSS_STRATEGYPOINT_STATE_SUMMON";    // 2007-04-26 by dhjin
    case ERR_CANNOT_WARP_TO_CITY:                        return "ERR_CANNOT_WARP_TO_CITY";                // 2007-05-22 by cmkwon
    case ERR_INVALID_WEAPON_INDEX:                        return "ERR_INVALID_WEAPON_INDEX";                // 2007-06-08 by cmkwon
    case ERR_INVALID_TUTORIAL:                            return "ERR_INVALID_TUTORIAL";
    case ERR_CANNOT_USE_SPEAKER_ITEM:                    return "ERR_CANNOT_USE_SPEAKER_ITEM";            // 2007-08-24 by cmkwon, 스피커아이템 사용 가능/금지 설정 기능 추가
    case ERR_CONFERENCEROOM_PERMISSION_DENIED:            return "ERR_CONFERENCEROOM_PERMISSION_DENIED";    // 2007-08-30 by cmkwon, 회의룸 시스템 구현 - 워프 불가 에러 추가
    case ERR_CANNOT_WARP_TO_OUTPOST:                    return "ERR_CANNOT_WARP_TO_OUTPOST";            // 2007-08-28 by dhjin, 전진기지 맵으로 워프 불가

    case ERR_CANNOT_WARP_TO_OUTPOST_CITY_MAP:            return "ERR_CANNOT_WARP_TO_OUTPOST_CITY_MAP";            // 2007-08-28 by dhjin, 전진기지 맵으로 워프 불가
    case ERR_CANNOT_SEARCH_OUTPOST:                        return "ERR_CANNOT_SEARCH_OUTPOST";            // 2007-08-28 by dhjin, 전진기지 맵으로 워프 불가
    case ERR_ALREADY_SET_OUTPOST_NEXTWARTIME:            return "ERR_ALREADY_SET_OUTPOST_NEXTWARTIME";            // 2007-08-28 by dhjin, 전진기지 맵으로 워프 불가
    case ERR_CANNOT_SEARCH_OUTPOST_NEXTWARTIME:            return "ERR_CANNOT_SEARCH_OUTPOST_NEXTWARTIME";            // 2007-08-28 by dhjin, 전진기지 맵으로 워프 불가

    
    case ERR_ARENA_NO_SUCH_TEAM:                        return "ERR_ARENA_NO_SUCH_TEAM";                // 2007-04-23 by dhjin
    case ERR_ARENA_CREATE_TEAM:                            return "ERR_ARENA_CREATE_TEAM";                    // 2007-04-23 by dhjin, 
    case ERR_ARENA_NOT_MATCH_MODE:                        return "ERR_ARENA_NOT_MATCH_MODE";                // 2007-04-23 by dhjin, 
    case ERR_ARENA_NOT_MATCH_LEVEL:                        return "ERR_ARENA_NOT_MATCH_LEVEL";                // 2007-04-23 by dhjin, 
    case ERR_ARENA_NOT_MATCH_PW:                        return "ERR_ARENA_NOT_MATCH_PW";                // 2007-04-23 by dhjin, 
    case ERR_ARENA_FULL_TEAMLIST:                        return "ERR_ARENA_FULL_TEAMLIST";                // 2007-04-23 by dhjin, 
    case ERR_ARENA_STATE:                                return "ERR_ARENA_STATE";                        // 2007-04-23 by dhjin, 
    case ERR_ARENA_STATE_TEAM_READY:                    return "ERR_ARENA_STATE_TEAM_READY";            // 2007-06-05 by dhjin
    case ERR_ARENA_STATE_TEAM_WARING:                    return "ERR_ARENA_STATE_TEAM_WARING";            // 2007-06-05 by dhjin
    case ERR_ARENA_BLOCKED:                                return "ERR_ARENA_BLOCKED";                        // 2007-07-11 by cmkwon, 아레나블럭 시스템 구현

    case ERR_NO_SEARCH_CITYLEADER_INFO:                    return "ERR_NO_SEARCH_CITYLEADER_INFO";            // 2007-09-13 by cmkwon
    case ERR_REQ_NOT_LEADER_CANDIDATE:                    return "ERR_REQ_NOT_LEADER_CANDIDATE";            // 2007-10-30 by dhjin
    case ERR_REQ_NOT_POLLDATE_APPLICATION:                return "ERR_REQ_NOT_POLLDATE_APPLICATION";        // 2007-10-30 by dhjin
    case ERR_FAIL_REG_LEADER_CANDIDATE_BY_DB:            return "ERR_FAIL_REG_LEADER_CANDIDATE_BY_DB";    // 2007-10-30 by dhjin
    case ERR_FAIL_REG_LEADER_CANDIDATE_BY_REQ:            return "ERR_FAIL_REG_LEADER_CANDIDATE_BY_REQ";    // 2007-10-30 by dhjin
    case ERR_REQ_NOT_POLLDATE_VOTE:                        return "ERR_REQ_NOT_POLLDATE_VOTE";                // 2007-10-30 by dhjin
    case ERR_REQ_NOT_VOTE:                                return "ERR_REQ_NOT_VOTE";                        // 2007-10-31 by dhjin
    case ERR_ALREADY_VOTE:                                return "ERR_ALREADY_VOTE";                        // 2007-10-31 by dhjin
    case ERR_INVALID_LEADER_CANDIDATE:                    return "ERR_INVALID_LEADER_CANDIDATE";            // 2007-10-31 by dhjin
    case ERR_ALREADY_LEADER_CANDIDATE:                    return "ERR_ALREADY_LEADER_CANDIDATE";            // 2007-11-01 by dhjin

    case ERR_ALREADY_DELETE_LEADER_CANDIDATE:            return "ERR_ALREADY_DELETE_LEADER_CANDIDATE";    // 2007-11-01 by dhjin
    case ERR_REQ_NOT_POLLDATE_ALL_RANGE:                return "ERR_REQ_NOT_POLLDATE_ALL_RANGE";        // 2007-11-20 by dhjin
    case ERR_REQ_REG_LEADER_CANDIDATE:                    return "ERR_REQ_REG_LEADER_CANDIDATE";        // 2007-11-20 by dhjin

    case ERR_CANNOT_WARP_DURING_MSWAR:                    return "ERR_CANNOT_WARP_DURING_MSWAR";            // 2012-07-02 by hskim, 모선전 개선 - 모선전 진행시 스톤즈 루인 진입 불가

    case ERR_WARING_OUTPOST_TO_OWNMAP:                    return "ERR_WARING_OUTPOST_TO_OWNMAP";            // 2007-11-01 by dhjin
    case ERR_DELETE_CHARACTER_GUILDCOMMANDER:            return "ERR_DELETE_CHARACTER_GUILDCOMMANDER";    // 2008-10-20 by dhjin, 여단장은 케릭터 삭제 불가로 변경

    case ERR_SECPASS_NOT_USE_SYSTEM:                    return "ERR_SECPASS_NOT_USE_SYSTEM";            // 2007-09-13 by cmkwon
    case ERR_SECPASS_PASSWORD_NOT_SETTING:                return "ERR_SECPASS_PASSWORD_NOT_SETTING";        // 2007-09-13 by cmkwon
    case ERR_SECPASS_ALREADY_LOCK:                        return "ERR_SECPASS_ALREADY_LOCK";                // 2007-09-13 by cmkwon
    case ERR_SECPASS_ALREADY_UNLOCK:                    return "ERR_SECPASS_ALREADY_UNLOCK";            // 2007-09-13 by cmkwon
    case ERR_SECPASS_PASSWORD_NOT_MATCHED:                return "ERR_SECPASS_PASSWORD_NOT_MATCHED";        // 2007-09-13 by cmkwon
    case ERR_SECPASS_NEW_PASSWORD_NOT_SETTING:            return "ERR_SECPASS_NEW_PASSWORD_NOT_SETTING";    // 2007-09-13 by cmkwon
    case ERR_SECPASS_LOCKED:                            return "ERR_SECPASS_LOCKED";                    // 2007-09-13 by cmkwon

    case ERR_LUCKYITEM_INSERT_BUT_COINITEM_DELETE:        return "ERR_LUCKYITEM_INSERT_BUT_COINITEM_DELETE";                // 2008-11-10 by dhjin, 럭키머신 아이템이 하나라도 지급되었지만 시스템에 오류가 있어 정상지급이 되지는 않았을 경우
        
    case ERR_INVALID_ARMOR_ITEM:                        return "ERR_INVALID_ARMOR_ITEM";
    case ERR_ITEM_TRADING:                                return "ERR_ITEM_TRADING";
    case ERR_USING_SHOP:                                return "ERR_USING_SHOP";
    case ERR_NO_SUCH_WEAPON_ITEM:                        return "ERR_NO_SUCH_WEAPON_ITEM";                // 2007-08-07 by cmkwon, 1형/2형 무기 총알 충전 아이템 구현 - 에러코드 추가
    case ERR_ALREADY_FULL_BULLET:                        return "ERR_ALREADY_FULL_BULLET";                // 2007-08-07 by cmkwon, 1형/2형 무기 총알 충전 아이템 구현 - 에러코드 추가
    case ERR_INVALID_ITEMEVENT_UID:                        return "ERR_INVALID_ITEMEVENT_UID";                // 2008-01-10 by cmkwon, 아이템 이벤트 시스템에 신 쿠폰 시스템 추가 - 
    case ERR_NOT_ACCOUNT_BLOCKED:                        return "ERR_NOT_ACCOUNT_BLOCKED";
    case ERR_SOLD_OUT_SHOP_ITEM:                        return "ERR_SOLD_OUT_SHOP_ITEM";                // 2010-01-26 by cmkwon, 캐쉬 아이템 한정판매 시스템 구현 - 
    case ERR_NO_SUCH_ENCHANT_INFO:                        return "ERR_NO_SUCH_ENCHANT_INFO";                // 2010-04-20 by cmkwon, 신규 러키 머신 구현 - 

    case ERR_PROTOCOL_SUBLEADER_SET_FAIL:                return "ERR_PROTOCOL_SUBLEADER_SET_FAIL";
    case ERR_PROTOCOL_SUBLEADER_SET_ALEADY:                return "ERR_PROTOCOL_SUBLEADER_SET_ALEADY";

    case ERR_CANNOT_USE_GUILD_STORE:                    return "ERR_CANNOT_USE_GUILD_STORE";            // 2009-09-23 by cmkwon, 필드창고 캐쉬 아이템 구현 - 
    case ERR_CANNOT_USE_STORE:                            return "ERR_CANNOT_USE_STORE";                    // 2009-09-23 by cmkwon, 필드창고 캐쉬 아이템 구현 - 
    case ERR_CANNOT_USE_FIELD_STORE:                    return "ERR_CANNOT_USE_FIELD_STORE";            // 2009-09-23 by cmkwon, 필드창고 캐쉬 아이템 구현 - 
    case ERR_ALREADY_USING_BUILDING:                    return "ERR_ALREADY_USING_BUILDING";            // 2009-09-23 by cmkwon, 필드창고 캐쉬 아이템 구현 - 
    case ERR_NO_SUCH_BUILDINGINDEX:                        return "ERR_NO_SUCH_BUILDINGINDEX";                // 2009-09-23 by cmkwon, 필드창고 캐쉬 아이템 구현 - 
    case ERR_CANNOT_BUY_ITEM:                            return "ERR_CANNOT_BUY_ITEM";                    // 2009-09-23 by cmkwon, 필드창고 캐쉬 아이템 구현 - 
    case ERR_ALREADY_USE_ITEMPARAM:                        return "ERR_ALREADY_USE_ITEMPARAM";                // 2010-01-18 by cmkwon, 아이템 사용시 Parameter 중복 체크 시스템 구현 - 
    case ERR_INSUFFICIENT_NUMBER_SHOP_ITEM:                return "ERR_INSUFFICIENT_NUMBER_SHOP_ITEM";        // 2011-10-28 by hskim, EP4 [트리거 시스템] - 크리스탈 시스템 - 구매할수 있는 수량이 부족함
    case ERR_NOT_OWNER_SHOP:                            return "ERR_NOT_OWNER_SHOP";                    // 2011-10-28 by hskim, EP4 [트리거 시스템] - 크리스탈 시스템 - 소유 상점이 아님

    case ERR_MAP_CHECKSUM_NOT_MATCHED:                    return "ERR_MAP_CHECKSUM_NOT_MATCHED";        // 2007-04-06 by cmkwon
    case ERR_REQ_WARP_ONE_TIME_LIMITED:                    return "ERR_REQ_WARP_ONE_TIME_LIMITED";            // 2011-10-28 by hskim, EP4 [트리거 시스템] - 크리스탈 시스템 - 1번만 입장 할 수있음
    case ERR_REQ_WARP_GUILD_NOT_MATCHED:                return "ERR_REQ_WARP_GUILD_NOT_MATCHED";        // 2011-10-28 by hskim, EP4 [트리거 시스템] - 크리스탈 시스템 - 현재 입장할 수 있는 길드가 아님
    case ERR_REQ_WARP_MAXIMUM_USER_LIMITED:                return "ERR_REQ_WARP_MAXIMUM_USER_LIMITED";        // 2011-10-28 by hskim, EP4 [트리거 시스템] - 크리스탈 시스템 - 최대 접속할 수 있는 인원을 넘었음
    case ERR_REQ_WARP_NO_QUEST_ITEM:                    return "ERR_REQ_WARP_NO_QUEST_ITEM";            // 2011-10-28 by hskim, EP4 [트리거 시스템] - 크리스탈 시스템 - 입장에 필요한 퀘스트 아이템이 없음
    case ERR_REQ_WARP_GAME_START_NOT_ALLOWED_USER:        return "ERR_REQ_WARP_GAME_START_NOT_ALLOWED_USER";        // 2011-10-28 by hskim, EP4 [트리거 시스템] - 크리스탈 시스템 - 게임 시작시에 맵에 입장이 허용된 유저가 아님
    case ERR_REQ_WARP_INFLUENCE_NOT_MATCHED:            return "ERR_REQ_WARP_INFLUENCE_NOT_MATCHED";        // 2011-10-28 by hskim, EP4 [트리거 시스템] - 크리스탈 시스템 - 현재 입장할 수 있는 세력이 아님

    case ERR_REQ_CHOOSE_START_CIIY_MAP_INDEX:            return "ERR_REQ_CHOOSE_START_CIIY_MAP_INDEX";    // 2009-10-12 by cmkwon, 프리스카 제거 방안 적용 - 

    case ERR_INVALID_ARENASERVER:                        return "ERR_INVALID_ARENASERVER";            // 2008-03-07 by dhjin, 아레나 통합 - 
    case ERR_NO_SUCH_FIELDSERVER:                        return "ERR_NO_SUCH_FIELDSERVER";            // 2008-03-07 by dhjin, 아레나 통합 - 
    case ERR_INVALID_MAINSERVER_CHARACTER:                return "ERR_INVALID_MAINSERVER_CHARACTER";    // 2008-03-07 by dhjin, 아레나 통합 - 
    case ERR_INVALID_ARENASERVER_CHARACTER:                return "ERR_INVALID_ARENASERVER_CHARACTER";    // 2008-03-07 by dhjin, 아레나 통합 -     
    case ERR_CANNOT_USE_ARENASERVER:                    return "ERR_CANNOT_USE_ARENASERVER";        // 2008-03-07 by dhjin, 아레나 통합 - 
    case ERR_NOT_CONNECT_TO_ARENASERVER:                return "ERR_NOT_CONNECT_TO_ARENASERVER";    // 2008-03-07 by dhjin, 아레나 통합 - 

    case ERR_NO_SUCH_LETTER:                            return "ERR_NO_SUCH_LETTER";                // 2008-04-30 by dhjin, EP3 편지 시스템 - 편지를 찾지 못하였다.

    case ERR_ALREADY_REG:                                return "ERR_ALREADY_REG";                    // 2008-04-30 by dhjin, EP3 - 여단 수정 사항 - 자기 소개가 등록이 되어있다.
    case ERR_CANNOT_USE_NEW_COMMANDER_BY_CITYLEADER:    return "ERR_CANNOT_USE_NEW_COMMANDER_BY_CITYLEADER";        // 2008-06-19 by dhjin, EP3 - 여단 수정 사항 - 지도자나 전진기지 여단장은 여단장 위임 불가 
    case ERR_CANNOT_USE_NEW_COMMANDER_BY_POLL:            return "ERR_CANNOT_USE_NEW_COMMANDER_BY_POLL";                // 2008-06-19 by dhjin, EP3 - 여단 수정 사항 - 선거에 출마한 사람은 여단장 위임 불가
    case ERR_POSSESS_OUTPOST:                            return "ERR_POSSESS_OUTPOST";                            // 2009-06-30 by cmkwon, 여단 해체시 체크 수정 - 

    case ERR_INVALID_DATA_FROM_CLIENT:                    return "ERR_INVALID_DATA_FROM_CLIENT";            // 2008-06-17 by dhjin, EP3 채팅방 - 클라이언트로부터 받은 잘못된 데이타
    case ERR_CHATROOM_NO_SUCH_CREATE_NUM:                return"ERR_CHATROOM_NO_SUCH_CREATE_NUM";        // 2008-06-17 by dhjin, EP3 채팅방 - 생성 방 번호가 없다
    case ERR_CHATROOM_NO_SUCH:                            return"ERR_CHATROOM_NO_SUCH";                    // 2008-06-17 by dhjin, EP3 채팅방 - 채팅방이 없다.
    case ERR_CHATROOM_ALREADY_USING:                    return"ERR_CHATROOM_ALREADY_USING";                // 2008-06-17 by dhjin, EP3 채팅방 - 이미 채팅방 이용중이다.
    case ERR_CHATROOM_PERMISSION_DENIED:                return"ERR_CHATROOM_PERMISSION_DENIED";            // 2008-06-17 by dhjin, EP3 채팅방 - 권한이 없다.
    case ERR_CHATROOM_FULL_MEMBER:                        return"ERR_CHATROOM_FULL_MEMBER";                // 2008-06-17 by dhjin, EP3 채팅방 - 채팅방 인원이 만땅이다.
    case ERR_CHATROOM_NO_SUCH_NEW_MASTER:                return"ERR_CHATROOM_NO_SUCH_NEW_MASTER";        // 2008-06-17 by dhjin, EP3 채팅방 - 새로운 방장을 찾지 못했다.
    case ERR_NOT_MATCH_PW:                                return"ERR_NOT_MATCH_PW";                        // 2008-07-15 by dhjin, EP3 채팅방 - 암호 불일치.
    case ERR_NO_SUCH_DECLARATION_MSWAR_INFO:            return"ERR_NO_SUCH_DECLARATION_MSWAR_INFO";        // 2009-01-12 by dhjin, 선전 포고 - 선전 포고 정보 못찾음
    case ERR_NO_MORE_DECLARATION_MSWAR_SET:                return"ERR_NO_MORE_DECLARATION_MSWAR_SET";        // 2009-01-12 by dhjin, 선전 포고 - 선전 포고 시간을 더 이상 설정 할 수 없다.
    case ERR_INVALID_DECLARATION_MSWAR_TIME:            return"ERR_INVALID_DECLARATION_MSWAR_TIME";        // 2009-01-12 by dhjin, 선전 포고 - 선전 포고 시간 설정이 잘못됨.

    case ERR_CANNOT_CHOOSE_INFLUENCE_TYPE:                return"ERR_CANNOT_CHOOSE_INFLUENCE_TYPE";        // 2009-03-31 by cmkwon, 세력초기화 시스템 구현 - 

    case ERR_CANNOT_CONNECT_INTERNET:                    return "ERR_CANNOT_CONNECT_INTERNET";
    case ERR_CANNOT_CONNECT_AUTO_UPDATE_SERVER:            return "ERR_CANNOT_CONNECT_AUTO_UPDATE_SERVER";
    case ERR_LOCAL_FILE_CREATE_FAIL:                    return "ERR_LOCAL_FILE_CREATE_FAIL";
    case ERR_AUTO_UPDATE_SERVER_PERMISSION_DENIED:        return "ERR_AUTO_UPDATE_SERVER_PERMISSION_DENIED";
    case ERR_UPDATE_FILE_NOT_FOUND:                        return "ERR_UPDATE_FILE_NOT_FOUND";
    case ERR_UPDATE_FILE_DOWNLOADING_FAIL:                return "ERR_UPDATE_FILE_DOWNLOADING_FAIL";
    case ERR_UPDATE_FILE_DOWNLOADING_CANCEL:            return "ERR_UPDATE_FILE_DOWNLOADING_CANCEL";
    case ERR_FUNC_HttpOpenRequest:                        return "ERR_FUNC_HttpOpenRequest";
    case ERR_FUNC_HttpSendRequest:                        return "ERR_FUNC_HttpSendRequest";
    case ERR_FUNC_HttpQueryInfo:                        return "ERR_FUNC_HttpQueryInfo";
    case ERR_VERSIONINFO_FILE_NOT_FOUND:                return "ERR_VERSIONINFO_FILE_NOT_FOUND";        // 2007-05-09 by cmkwon
    case ERR_VERSIONINFO_FILE_CAN_NOT_CREATE:            return "ERR_VERSIONINFO_FILE_CAN_NOT_CREATE";        // 2007-05-09 by cmkwon
    case ERR_NOT_ALLOWED_IP:                            return "ERR_NOT_ALLOWED_IP";        // 2007-10-19 by cmkwon, AllowedIP 시스템 변경 -

    ////////////////////////////////////////////////////////////////////////////////
    // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 
    case ERR_DEBUFF_SKILL_APPLYING_SLIENCE:                return "ERR_DEBUFF_SKILL_APPLYING_SLIENCE";                // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 사일런스 디버프가 적용중이다.
    case ERR_RELEASE_SKILL_NOT_FIND:                    return "ERR_RELEASE_SKILL_NOT_FIND";                    // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 해제할 스킬을 찾을수가 없다.
    case ERR_DEBUFF_SKILL_APPLYING_NOT_HP_RECOVERY:        return "ERR_DEBUFF_SKILL_APPLYING_NOT_HP_RECOVERY";        // 2009-09-09 ~ 2010 by dhjin, 인피니티 - HP회복 금지
    case ERR_DEBUFF_SKILL_APPLYING_NOT_DP_RECOVERY:        return "ERR_DEBUFF_SKILL_APPLYING_NOT_DP_RECOVERY";        // 2009-09-09 ~ 2010 by dhjin, 인피니티 - DP회복 금지
    case ERR_DEBUFF_SKILL_APPLYING_NOT_SP_RECOVERY:        return "ERR_DEBUFF_SKILL_APPLYING_NOT_SP_RECOVERY";        // 2009-09-09 ~ 2010 by dhjin, 인피니티 - SP회복 금지
    case ERR_INFINITY_NO_SUCH_MODE_LIST:                return "ERR_INFINITY_NO_SUCH_MODE_LIST";                // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피니티 리스트를 찾지 못했을 경우
    case ERR_INFINITY_NO_SUCH_READY_LIST:                return "ERR_INFINITY_NO_SUCH_READY_LIST";                // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피니티 대기 리스트를 찾지 못했을 경우
    case ERR_INFINITY_OVER_ENTRANCECOUNT:                return "ERR_INFINITY_OVER_ENTRANCECOUNT";                // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피니티 입장 가능한 수를 넘겼다.
    case ERR_INFINITY_CREATE_FAIL:                        return "ERR_INFINITY_CREATE_FAIL";                        // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피니티 생성 실패
    case ERR_INFINITY_JOIN_FAIL_SOCKET:                    return "ERR_INFINITY_JOIN_FAIL_SOCKET";                    // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피니티 가입 실패 - 소켓 NULL값
    case ERR_INFINITY_MAX_ADMISSIONCNT:                    return "ERR_INFINITY_MAX_ADMISSIONCNT";                // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피니티 가입 실패 - 최대 인원
    case ERR_INFINITY_MISMATCH_LEVEL:                    return "ERR_INFINITY_MISMATCH_LEVEL";                    // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피니티 가입 실패 - 레벨
    case ERR_INFINITY_MODE:                                return "ERR_INFINITY_MODE";                                // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 잘못된 모드
    case ERR_INFINITY_CREATEUID:                        return "ERR_INFINITY_CREATEUID";                        // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 잘못된 생성 번호
    case ERR_INFINITY_NO_SUCH_MASTER_USER:                return "ERR_INFINITY_NO_SUCH_MASTER_USER";                // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피니티 가입 실패 - 인피 마스터
    case ERR_INFINITY_JOIN_FAIL_MASTERUSER_REJECT:        return "ERR_INFINITY_JOIN_FAIL_MASTERUSER_REJECT";        // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피니티 가입 실패 - 생성 번호
    case ERR_INFINITY_NO_SUCH_MEMBER_LIST:                return "ERR_INFINITY_NO_SUCH_MEMBER_LIST";                // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피니티 유저 리스트를 찾지 못했을 경우
    case ERR_INFINITY_STATE:                            return "ERR_INFINITY_STATE";                            // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피니티 잘못된 상태값
    case ERR_INFINITY_SAME_MASTER:                        return "ERR_INFINITY_SAME_MASTER";                        // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 동일 인물!
    case ERR_INFINITY_MASTER:                            return "ERR_INFINITY_MASTER";                            // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 마스터가 아니다.
    case ERR_INFINITY_CREATE_MAPCHANNEL:                return "ERR_INFINITY_CREATE_MAPCHANNEL";                // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 맵 생성 중 채널 생성 오류
    case ERR_INFINITY_NOT_ALL_READY:                    return "ERR_INFINITY_NOT_ALL_READY";                    // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 모든 유저가 래디 상태가 아니다.
    case ERR_INFINITY_CANNOT_USE_ITEM:                    return "ERR_INFINITY_CANNOT_USE_ITEM";                    // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피니티에서 사용 불가
    case ERR_INFINITY_MIN_ADMISSIONCNT:                    return "ERR_INFINITY_MIN_ADMISSIONCNT";                    // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 이용 최소 인원 오류
    case ERR_INFINITY_TENDER_PROCESSING:                return "ERR_INFINITY_TENDER_PROCESSING";                // 2009-09-09 ~ 2010 by dhjin, 인피니티 - Tender중이다.
    case ERR_INFINITY_NOT_PROCESSING_TENDERITEM:        return "ERR_INFINITY_NOT_PROCESSING_TENDERITEM";        // 2009-09-09 ~ 2010 by dhjin, 인피니티 - Tender진행중인 아이템이 아니다.
    case ERR_INFINITY_ONLY_USE_ITEM:                    return "ERR_INFINITY_ONLY_USE_ITEM";                    // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피니티에서만 사용 가능
    case ERR_INFINITY_CANNOT_SUCH_TEAM:                    return "ERR_INFINITY_CANNOT_SUCH_TEAM";                    // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 팅긴 유저 재접속 처리, 존재하는 팀이 없다.
    case ERR_INFINITY_NULL_VALUE:                        return "ERR_INFINITY_NULL_VALUE";                        // 2009-09-09 ~ 2010-01 by dhjin, 인피니티 - 소스 체크, NULL값 체크
    case ERR_INFINITY_CANNOT_ALL_START:                    return "ERR_INFINITY_CANNOT_ALL_START";                    // 2010-04-06 by cmkwon, 인피2차 추가 수정 - 
    case ERR_INFINITY_TENDER_ALREADY_CHOICED:            return "ERR_INFINITY_TENDER_ALREADY_CHOICED";            // 2010-04-09 by cmkwon, 인피2차 추가 수정(단계별 보상 추가) - 

    case ERR_INFINITY_DIFFICLUTY_NOT_FIND :                return "ERR_INFINITY_DIFFICLUTY_NOT_FIND";                // 2010-05-04 by shcho, 인피니티 난이도 조절 - 난이도를 찾지 못했을 때
    case ERR_INFINITY_DIFFICULTY_LEVEL_SAME :            return "ERR_INFINITY_DIFFICULTY_LEVEL_SAME";            // 2010. 05. 19 by hsLee 인피니티 필드 2차 난이도 조절. (신호처리 + 몬스터 처리(서버) ) - 같은 난이도로 변경 요청 오류.
    case ERR_INFINITY_DIFFICULTY_LEVEL_INVALID :        return "ERR_INFINITY_DIFFICULTY_LEVEL_INVALID";            // 2010. 05. 19 by hsLee 인피니티 필드 2차 난이도 조절. (신호처리 + 몬스터 처리(서버) ) - 유효하지 않은 난이도 변경값 요청 오류.

    case ERR_INFINITY_MISMATCH_CREATEUID :                return "ERR_INFINITY_MISMATCH_CREATEUID";                // 인피니티 - 방 생성번호 불일치.    // 2010. 07. 27 by hsLee 인피니티 2차 거점 방어 시네마 연출 스킵 처리.
    case ERR_INFINITY_PLAYING_STATE :                    return "ERR_INFINITY_PLAYING_STATE";                    // 인피니티 - 게임 진행 중이 아닌 경우. ( STATE - PLAYING ~ 엔딩 ) // 2010. 07. 27 by hsLee 인피니티 2차 거점 방어 시네마 연출 스킵 처리.
    case ERR_INFINITY_ALWAYS_SKIP_ENDINGCINEMA :        return "ERR_INFINITY_ALWAYS_SKIP_ENDINGCINEMA";            // 인피니티 - 이미 연출 스킵 활성화 되어 있음. // 2010. 07. 27 by hsLee 인피니티 2차 거점 방어 시네마 연출 스킵 처리.

    case ERR_DEBUFF_SKILL_APPLYING_STOPMAGIC :            return "ERR_DEBUFF_SKILL_APPLYING_STOPMAGIC";            // 2011-10-28 by hskim, EP4 [트리거 시스템] - 스킬 추가 : 침묵 당해 스킬을 사용할수 없습니다.

    case ERR_KNOWN_ERROR:                                return "ERR_KNOWN_ERROR";            // 2010-04-26 by cmkwon, 러시아 Innva 인증/빌링 시스템 변경 - 
    case ERR_INVALID_HANDLE:                            return "ERR_INVALID_HANDLE";            // 2010-04-26 by cmkwon, 러시아 Innva 인증/빌링 시스템 변경 - 

    ///////////////////////////////////////////////////////////////////////////////
    // 2011-01-26 by hskim, 인증 서버 구현

    case ERR_PROTOCOL_INVALID_AUTHENTICATION_SERVER_LOGIN_DATA:            return "ERR_PROTOCOL_INVALID_AUTHENTICATION_SERVER_LOGIN_DATA"; // 2011-01-26 by hskim, 인증 서버 구현 - PreServer 로 부터 받은 Data Size가 Authentication Packet Type에 따른 Data Size보다 작음
    case ERR_PROTOCOL_INVALID_AUTHENTICATION_SERVER_LOGIN_DB:            return "ERR_PROTOCOL_INVALID_AUTHENTICATION_SERVER_LOGIN_DB";    // 2011-01-26 by hskim, 인증 서버 구현 - PreServer 로 부터 받은데이타를 사용해 인증절차를 거쳤지만 중간에 정상적으로 처리되지 않았다

    /////////////////////////////////////////////////////////////
    // 2011-08-22 by hskim, 파트너 시스템 2차

    case ERR_PET_NUMERIC_VALUE_OUT_OF_RANGE:            return "ERR_PET_NUMERIC_VALUE_OUT_OF_RANGE";        // 2011-09-15 by hskim, 파트너 시스템 2차 - 값이 범위를 벗어남
    case ERR_PET_NOT_EQUIPPED:                            return "ERR_PET_NOT_EQUIPPED";                        // 2011-09-15 by hskim, 파트너 시스템 2차 - 펫이 장착되어 있지 않다
    case ERR_PET_EMPTY_NAME:                            return "ERR_PET_EMPTY_NAME";                        // 2011-09-15 by hskim, 파트너 시스템 2차 - 이름이 비여 있음
    case ERR_PET_NOT_ALLOW_LEVEL_UP:                    return "ERR_PET_NOT_ALLOW_LEVEL_UP";                // 2011-09-15 by hskim, 파트너 시스템 2차 - 레벨업 허용 안함
    case ERR_PET_NOT_ALLOW_CHANGE_NAME:                    return "ERR_PET_NOT_ALLOW_CHANGE_NAME";                // 2011-09-15 by hskim, 파트너 시스템 2차 - 이름 변경 허용 안함
    case ERR_PET_NOT_ALLOW_SOCKET_INDEX:                return "ERR_PET_NOT_ALLOW_SOCKET_INDEX";            // 2011-09-15 by hskim, 파트너 시스템 2차 - 허용안되는 소켓 인덱스
    case ERR_PET_ALREADY_USE_SOCKET:                    return "ERR_PET_ALREADY_USE_SOCKET";                // 2011-09-15 by hskim, 파트너 시스템 2차 - 이미 사용된 소켓
    case ERR_PET_INVALID_SOCKET:                        return "ERR_PET_INVALID_SOCKET";                    // 2011-09-15 by hskim, 파트너 시스템 2차 - 잘못된 소켓
    case ERR_PET_NO_SUCH_INFO:                            return "ERR_PET_NO_SUCH_INFO";                        // 2011-09-15 by hskim, 파트너 시스템 2차 - 펫관련 정보를 찾을수 없음
    case ERR_PET_INVALID_AUTOSKILL:                        return "ERR_PET_INVALID_AUTOSKILL";                    // 2011-09-15 by hskim, 파트너 시스템 2차 - 잘못된 자동 스킬
    case ERR_PET_INVALID_AUTOKIT:                        return "ERR_PET_INVALID_AUTOKIT";                    // 2011-09-15 by hskim, 파트너 시스템 2차 - 잘못된 오트 스킬
    case ERR_PET_NOT_MATCH_SOCKET:                        return "ERR_PET_NOT_MATCH_SOCKET";                    // 2011-09-15 by hskim, 파트너 시스템 2차 - 소켓아이템이 서버정보와 일치하지 않는다

    case ERR_SECURITY_NOT_CHECK_VERSION:                return "ERR_SECURITY_NOT_CHECK_VERSION";
    case ERR_SECURITY_HACKING_GUID:                        return "ERR_SECURITY_HACKING_GUID";
    case ERR_SECURITY_HACKING_CRC:                        return "ERR_SECURITY_HACKING_CRC";
    case ERR_SECURITY_HACKING_CLIENT:                    return "ERR_SECURITY_HACKING_CLIENT";
    case ERR_SECURITY_ANTICPX_INVALID_HANDLE:            return "ERR_SECURITY_ANTICPX_INVALID_HANDLE";    // 2008-04-03 by cmkwon, 핵쉴드 서버 연동 시스템 수정 - 
    case ERR_SECURITY_CREATE_CLIENT_FAIL:                return "ERR_SECURITY_CREATE_CLIENT_FAIL";        // 2008-04-03 by cmkwon, 핵쉴드 서버 연동 시스템 수정 - 

    case ERR_AHNHS_ENGINE_DETECT_GAME_HACK:                return "ERR_AHNHS_ENGINE_DETECT_GAME_HACK";
    case ERR_AHNHS_ACTAPC_DETECT_AUTOMOUSE:                return "ERR_AHNHS_ACTAPC_DETECT_AUTOMOUSE";
    case ERR_AHNHS_ACTAPC_DETECT_ALREADYHOOKED:            return "ERR_AHNHS_ACTAPC_DETECT_ALREADYHOOKED";
    case ERR_AHNHS_ACTAPC_DETECT_SPEEDHACK:                return "ERR_AHNHS_ACTAPC_DETECT_SPEEDHACK";
    case ERR_AHNHS_ACTAPC_DETECT_SPEEDHACK_APP:            return "ERR_AHNHS_ACTAPC_DETECT_SPEEDHACK_APP";
    case ERR_AHNHS_ACTAPC_DETECT_KDTRACE:                return "ERR_AHNHS_ACTAPC_DETECT_KDTRACE";
    case ERR_AHNHS_ACTAPC_DETECT_KDTRACE_CHANGED:        return "ERR_AHNHS_ACTAPC_DETECT_KDTRACE_CHANGED";
    case ERR_AHNHS_ACTAPC_DETECT_DRIVERFAILED:            return "ERR_AHNHS_ACTAPC_DETECT_DRIVERFAILED";
    case ERR_AHNHS_ACTAPC_DETECT_HOOKFUNCTION:            return "ERR_AHNHS_ACTAPC_DETECT_HOOKFUNCTION";
    case ERR_AHNHS_ACTAPC_DETECT_MESSAGEHOOK:            return "ERR_AHNHS_ACTAPC_DETECT_MESSAGEHOOK";
    case ERR_AHNHS_ACTAPC_DETECT_MODULE_CHANGE:            return "ERR_AHNHS_ACTAPC_DETECT_MODULE_CHANGE";
    case ERR_AHNHS_ACTAPC_DETECT_ABNORMAL_MEMORY_ACCESS:    return "ERR_AHNHS_ACTAPC_DETECT_ABNORMAL_MEMORY_ACCESS";    // 2007-04-09 by cmkwon

    case ERR_AHNHS_ACTAPC_DETECT_ENGINEFAILED:                return "ERR_AHNHS_ACTAPC_DETECT_ENGINEFAILED";    // 2008-03-24 by cmkwon, 핵쉴드 2.0 적용 - AHNHS_ACTAPC_DETECT_ENGINEFAILED 추가됨
    case ERR_AHNHS_ACTAPC_DETECT_AUTOMACRO:                        return "ERR_AHNHS_ACTAPC_DETECT_AUTOMACRO";                        // 2009-07-17 by cmkwon, 핵쉴드 자체 종료 처리 에러타입추가(클라이언트만 사용) - 
    case ERR_AHNHS_ACTAPC_DETECT_CODEMISMATCH:                    return "ERR_AHNHS_ACTAPC_DETECT_CODEMISMATCH";                    // 2009-07-17 by cmkwon, 핵쉴드 자체 종료 처리 에러타입추가(클라이언트만 사용) - 
    case ERR_AHNHS_ACTAPC_DETECT_ABNORMAL_HACKSHIELD_STATUS:    return "ERR_AHNHS_ACTAPC_DETECT_ABNORMAL_HACKSHIELD_STATUS";    // 2009-07-17 by cmkwon, 핵쉴드 자체 종료 처리 에러타입추가(클라이언트만 사용) - 
    case ERR_AHNHS_ACTAPC_DETECT_LMP_FAILED:                    return "ERR_AHNHS_ACTAPC_DETECT_LMP_FAILED";                    // 2009-07-17 by cmkwon, 핵쉴드 자체 종료 처리 에러타입추가(클라이언트만 사용) - 

    case ERR_CASH_PREMIUM_CARD_INVALID_ITEMNUM:            return "ERR_CASH_PREMIUM_CARD_INVALID_ITEMNUM";
    case ERR_CASH_PREMIUM_CARD_ALREADY_USING:            return "ERR_CASH_PREMIUM_CARD_ALREADY_USING";
    case ERR_CASH_PREMIUM_CARD_NOT_MATCHED:                return "ERR_CASH_PREMIUM_CARD_NOT_MATCHED";
//    case ERR_CASH_PREMIUM_CARD_NOT_YET_BUY_STATE:        return "ERR_CASH_PREMIUM_CARD_NOT_YET_BUY_STATE";
    case ERR_CASH_PREMIUM_CARD_NOT_YET_BUY_STATE:        return "ERR_CASH_PREMIUM_CARD_NOT_YET_BUY_STATE";
    case ERR_NOT_DISSOLVED_ITEM:                        return "ERR_NOT_DISSOLVED_ITEM";
    case ERR_NOT_INSERT_DISSOLVED_ITEMS:                return "ERR_NOT_INSERT_DISSOLVED_ITEMS";
    case ERR_CANNOT_DO_GIFT_UNDER_LEVEL:                return "ERR_CANNOT_DO_GIFT_UNDER_LEVEL";    // 2013-07-08 by bckim, 캐나다요청 레벨70 미만은 선물하기 불가
    
    // 2012-08-17 by jhseol, 마상 BPSoft 빌링시스템
    case ERR_CASH_BPSOFT_COM_LIBRARY_INIT:                return "ERR_CASH_BPSOFT_COM_LIBRARY_INIT";
    case ERR_CASH_BPSOFT_CREATION_OBJECK:                return "ERR_CASH_BPSOFT_CREATION_OBJECK";
    case ERR_CASH_BPSOFT_METHOD_CALL:                    return "ERR_CASH_BPSOFT_METHOD_CALL";
    case ERR_CASH_BPSOFT_EXCEPTION:                        return "ERR_CASH_BPSOFT_EXCEPTION";
    // end 2012-08-17 by jhseol, 마상 BPSoft 빌링시스템

    case ERR_PROTOCOL_MULTIPLE_IP_CONNECTION:            return "ERR_PROTOCOL_MULTIPLE_IP_CONNECTION";    // 2015-11-24 Future, Restrict Multiple IP Access

    // 2015-12-17 Future, new error codes for the Nation Change Card
    case ERR_IS_LEADER:                                    return "ERR_IS_LEADER";
    case ERR_NOT_ENOUGH_FAME:                            return "ERR_NOT_ENOUGH_FAME";
    case ERR_INVALID_INFLUENCE:                            return "ERR_INVALID_INFLUENCE";
    case ERR_UNFAIR_BALANCE:                            return "ERR_UNFAIR_BALANCE";
    case ERR_IS_GUILD_MEMBER:                            return "ERR_IS_GUILD_MEMBER";

    // 2016-01-04 Future, User is having a warp restriction
    case ERR_WARP_RESTRICTED:                            return "ERR_WARP_RESTRICTED";

    }

    return "UNKNOWN_ERROR";
}


///////////////////////////////////////////////////////////////////////////////
/// \fn            char *GET_LANGUAGE_TYPE_STRING(int i_nLangTy)
/// \brief        // 2008-04-25 by cmkwon, 지원 언어에 독일어 추가 - GET_LANGUAGE_TYPE_STRING() 추가
/// \author        cmkwon
/// \date        2008-04-25 ~ 2008-04-25
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
//char *GET_LANGUAGE_TYPE_STRING(int i_nLangTy)
//{
//    // 2008-04-25 by cmkwon, 지원 언어/서비스 추가시 꼭 추가 되어야 하는 사항 - [언어-필수] 추가 언어 스트링으로 가져오기 함수에 추가
//
//    switch(i_nLangTy)
//    {
//    case LANGUAGE_TYPE_DEFAULT:            return "LANGUAGE_TYPE_DEFAULT (earlier LANGUAGE_TYPE_KOREAN)";    // 2015-05-17 Future, renamed Korean to Default
//    case LANGUAGE_TYPE_ENGLISH:            return "LANGUAGE_TYPE_ENGLISH";
//    case LANGUAGE_TYPE_VIETNAMESE:        return "LANGUAGE_TYPE_VIETNAMESE";
//    }
//    return "LANGUAGE_TYPE_UNKNOWN";
//}

char *Get_EVENT_TYPE_String(EventType_t i_evType)
{
    switch(i_evType)
    {
        GET_CASERETURN_STRING_BY_TYPE(EVENT_TYPE_NOEVENT);
        GET_CASERETURN_STRING_BY_TYPE(EVENT_TYPE_WARP);
        GET_CASERETURN_STRING_BY_TYPE(EVENT_TYPE_WARP_TARGET);
        GET_CASERETURN_STRING_BY_TYPE(EVENT_TYPE_ENTER_BUILDING);
        GET_CASERETURN_STRING_BY_TYPE(EVENT_TYPE_LANDING);
        GET_CASERETURN_STRING_BY_TYPE(EVENT_TYPE_RACING_CHECK_POTINT);
        GET_CASERETURN_STRING_BY_TYPE(EVENT_TYPE_OBJECT_MONSTER);
        GET_CASERETURN_STRING_BY_TYPE(EVENT_TYPE_PATTERN_POINT);
        GET_CASERETURN_STRING_BY_TYPE(EVENT_TYPE_OBJ_BUILDING_NPC);
        GET_CASERETURN_STRING_BY_TYPE(EVENT_TYPE_LANDING_PATTERN_START);
        GET_CASERETURN_STRING_BY_TYPE(EVENT_TYPE_LANDING_PATTERN_END);
        GET_CASERETURN_STRING_BY_TYPE(EVENT_TYPE_TAKEOFF_PATTERN_START);
        GET_CASERETURN_STRING_BY_TYPE(EVENT_TYPE_TAKEOFF_PATTERN_END);
        GET_CASERETURN_STRING_BY_TYPE(EVENT_TYPE_OBJ_ENTER_BUILDING);
//        GET_CASERETURN_STRING_BY_TYPE(EVENT_TYPE_OBJ_LEAVE_BUILDING);
    }
    
    return "UNKNOWN_EVENT_TYPE";
}

char * GetStringBLOCKED_TYPE(int i_blocktype)
{
    switch(i_blocktype)
    {
    case T_BLOCKEDACCOUNT_UNKNOWN:                return "Unknown";                // 알수 없음
    case T_BLOCKEDACCOUNT_NORMAL:                return "Normal";                // 특별한 사유 없음, 임시로
    case T_BLOCKEDACCOUNT_MONEY_RELATED:        return "Related Money";            // 
    case T_BLOCKEDACCOUNT_ITEM_RELATED:            return "Related Item";            // 
    case T_BLOCKEDACCOUNT_SPEEDHACK_RELATED:    return "Related SpeedHack";        //  
    case T_BLOCKEDACCOUNT_CHAT_RELATED:            return "Related Chatting";        // 
    case T_BLOCKEDACCOUNT_CHAT_GAMEBUG:            return "Related GameBug";        // 
    case T_BLOCKEDACCOUNT_MEMORYHACK_AUTOBLOCK:    return "Related AutoBlock - MemHack";        // 2012-12-14 by hskim, 메모리핵 자동 블럭 기능 구현
    case T_BLOCKEDACCOUNT_SPEEDHACK_AUTOBLOCK:    return "Related AutoBlock - SpdHack";        // 2013-01-29 by hskim, 스피드핵 자동 블럭 기능 구현
    }
    return "Unknown";
}

///////////////////////////////////////////////////////////////////////////////
/// \fn            int GetBlockTypeBYBlockTypeString(char *i_szTyString)
/// \brief        // 2008-01-30 by cmkwon, 계정 블럭 로그 남기기 구현 - GetBlockTypeBYBlockTypeString() 추가
/// \author        cmkwon
/// \date        2008-01-30 ~ 2008-01-30
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
int GetBlockTypeBYBlockTypeString(char *i_szTyString)
{
    if(0 == stricmp(i_szTyString, "Unknown"))
    {
        return T_BLOCKEDACCOUNT_UNKNOWN;
    }

    if(0 == stricmp(i_szTyString, "Normal"))
    {
        return T_BLOCKEDACCOUNT_NORMAL;
    }
    if(0 == stricmp(i_szTyString, "Related Money"))
    {
        return T_BLOCKEDACCOUNT_MONEY_RELATED;
    }
    if(0 == stricmp(i_szTyString, "Related Item"))
    {
        return T_BLOCKEDACCOUNT_ITEM_RELATED;
    }
    if(0 == stricmp(i_szTyString, "Related SpeedHack"))
    {
        return T_BLOCKEDACCOUNT_SPEEDHACK_RELATED;
    }
    if(0 == stricmp(i_szTyString, "Related Chatting"))
    {
        return T_BLOCKEDACCOUNT_CHAT_RELATED;
    }
    if(0 == stricmp(i_szTyString, "Related GameBug"))
    {
        return T_BLOCKEDACCOUNT_CHAT_GAMEBUG;
    }
    // 2012-12-14 by hskim, 메모리핵 자동 블럭 기능 구현
    if(0 == stricmp(i_szTyString, "Related AutoBlock - MemHack"))
    {
        return T_BLOCKEDACCOUNT_MEMORYHACK_AUTOBLOCK;
    }
    // end 2012-12-14 by hskim, 메모리핵 자동 블럭 기능 구현
    // 2013-01-29 by hskim, 스피드핵 자동 블럭 기능 구현
    if(0 == stricmp(i_szTyString, "Related AutoBlock - SpdHack"))
    {
        return T_BLOCKEDACCOUNT_SPEEDHACK_AUTOBLOCK;
    }
    // end 2013-01-29 by hskim, 스피드핵 자동 블럭 기능 구현
    
    return T_BLOCKEDACCOUNT_UNKNOWN;
}
///////////////////////////////////////////////////////////////////////////////
/// \fn            char *GetStringGuildMarkState(BYTE i_byGuildMarkState, BOOL i_bForUser/*=FALSE*/)
/// \brief        
/// \author        cmkwon
/// \date        2007-08-03 ~ 2007-08-03
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
const char *GetStringGuildMarkState(BYTE i_byGuildMarkState, bool i_bForUser/*=FALSE*/)
{
    if (!i_bForUser)
    {
        switch(i_byGuildMarkState)
        {
        case GUILD_MARK_STATE_NONE:                    return "GUILD_MARK_STATE_NONE";
        case GUILD_MARK_STATE_WAITING_PERMISSION:    return "GUILD_MARK_STATE_WAITING_PERMISSION";
        case GUILD_MARK_STATE_NORMAL:                return "GUILD_MARK_STATE_NORMAL";
        default:                                    return "GUILD_MARK_STATE_UNKNWON";
        }
    }

    switch(i_byGuildMarkState)
    {
    case GUILD_MARK_STATE_NONE:                    return STRMSG_070802_0004;
    case GUILD_MARK_STATE_WAITING_PERMISSION:    return STRMSG_070802_0005;
    case GUILD_MARK_STATE_NORMAL:                return STRMSG_070802_0006;
    default:                                    return STRMSG_070802_0007;
    }
    
}

///////////////////////////////////////////////////////////////////////////////
/// \fn            
/// \brief        // 2010-04-20 by cmkwon, 신규 러키 머신 구현 - 
/// \author        cmkwon
/// \date        2010-04-21 ~ 2010-04-21
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
char *GetEnchatLogTypeString(int i_nEnchantLogTy)
{
    switch(i_nEnchantLogTy)
    {
    case ENCHANT_LOGTYPE_ENCHANT_FAIL                :                return "ENCHANT_LOGTYPE_ENCHANT_FAIL";
    case ENCHANT_LOGTYPE_ENCHANT_SUCCESS            :                return "ENCHANT_LOGTYPE_ENCHANT_SUCCESS";
    case ENCHANT_LOGTYPE_ENCHANT_INIT_FAIL            :                return "ENCHANT_LOGTYPE_ENCHANT_INIT_FAIL";
    case ENCHANT_LOGTYPE_ENCHANT_INIT_SUCCESS        :                return "ENCHANT_LOGTYPE_ENCHANT_INIT_SUCCESS";
    case ENCHANT_LOGTYPE_GAMBLE_PREFIX_FAIL            :                return "ENCHANT_LOGTYPE_GAMBLE_PREFIX_FAIL";
    case ENCHANT_LOGTYPE_GAMBLE_PREFIX_SUCCESS        :                return "ENCHANT_LOGTYPE_GAMBLE_PREFIX_SUCCESS";
    case ENCHANT_LOGTYPE_GAMBLE_SUFFIX_FAIL            :                return "ENCHANT_LOGTYPE_GAMBLE_SUFFIX_FAIL";
    case ENCHANT_LOGTYPE_GAMBLE_SUFFIX_SUCCESS        :                return "ENCHANT_LOGTYPE_GAMBLE_SUFFIX_SUCCESS";
    case ENCHANT_LOGTYPE_GAMBLE_PREFIX_INIT_FAIL    :                return "ENCHANT_LOGTYPE_GAMBLE_PREFIX_INIT_FAIL";
    case ENCHANT_LOGTYPE_GAMBLE_PREFIX_INIT_SUCCESS    :                return "ENCHANT_LOGTYPE_GAMBLE_PREFIX_INIT_SUCCESS";
    case ENCHANT_LOGTYPE_GAMBLE_SUFFIX_INIT_FAIL    :                return "ENCHANT_LOGTYPE_GAMBLE_SUFFIX_INIT_FAIL";
    case ENCHANT_LOGTYPE_GAMBLE_SUFFIX_INIT_SUCCESS    :                return "ENCHANT_LOGTYPE_GAMBLE_SUFFIX_INIT_SUCCESS";
    }

    return "ENCHANT_LOGTYPE_UNKNOWN";
}

// 2013-07-01 by bckim, 10인챈트 미만 하이퍼카드처리, 로그추가, 운영툴 보완
char *GetTryEnchantSuccessTypeString(int i_nSuccessType)
{
    switch(i_nSuccessType)
    {
        case TRY_ENCHANT_FAIL                :                return "FAIL";
        case TRY_ENCHANT_SUCCESS            :                return "SUCCESS";
        case TRY_ENCHANT_PREVENT_5            :                return "PREVENT_5";
        case TRY_ENCHANT_PREVENT_10            :                return "PREVENT_10";
    }    
    return "TRY_ENCHANT_LOGTYPE_UNKNOWN";
}
// End. 2013-07-01 by bckim, 10인챈트 미만 하이퍼카드처리, 로그추가, 운영툴 보완


int GetEnchantLogType(ITEM *i_pEnchantItemInfo, bool i_bSuccessFlag)
{
    if(ITEMKIND_ENCHANT == i_pEnchantItemInfo->Kind)
    {
        if(i_pEnchantItemInfo->IsExistDesParam(DES_ENCHANT_INITIALIZE))
        {
            return (FALSE == i_bSuccessFlag) ? ENCHANT_LOGTYPE_ENCHANT_INIT_FAIL : ENCHANT_LOGTYPE_ENCHANT_INIT_SUCCESS;
        }
        return (FALSE == i_bSuccessFlag) ? ENCHANT_LOGTYPE_ENCHANT_FAIL : ENCHANT_LOGTYPE_ENCHANT_SUCCESS;
    }

    if(i_pEnchantItemInfo->IsExistDesParam(DES_RARE_FIX_PREFIX_INITIALIZE))
    {
        return (FALSE == i_bSuccessFlag) ? ENCHANT_LOGTYPE_GAMBLE_PREFIX_INIT_FAIL : ENCHANT_LOGTYPE_GAMBLE_PREFIX_INIT_SUCCESS;
    }
    if(i_pEnchantItemInfo->IsExistDesParam(DES_RARE_FIX_SUFFIX_INITIALIZE))
    {
        return (FALSE == i_bSuccessFlag) ? ENCHANT_LOGTYPE_GAMBLE_SUFFIX_INIT_FAIL : ENCHANT_LOGTYPE_GAMBLE_SUFFIX_INIT_SUCCESS;
    }

    if(i_pEnchantItemInfo->IsExistDesParam(DES_RARE_FIX_PREFIX))
    {
        return (FALSE == i_bSuccessFlag) ? ENCHANT_LOGTYPE_GAMBLE_PREFIX_FAIL : ENCHANT_LOGTYPE_GAMBLE_PREFIX_SUCCESS;
    }
    if(i_pEnchantItemInfo->IsExistDesParam(DES_RARE_FIX_SUFFIX))
    {
        return (FALSE == i_bSuccessFlag) ? ENCHANT_LOGTYPE_GAMBLE_SUFFIX_FAIL : ENCHANT_LOGTYPE_GAMBLE_SUFFIX_SUCCESS;
    }

    return ENCHANT_LOGTYPE_UNKNOWN;
}

// 2013-03-13 by hskim, 웹 캐시 상점
char* GetErrorXMLRPCString(int err)
{
    // 아래 부분 구현이 마무리되면 영어로 번역 할 예정

    switch(err)
    {
    /*
    case XMLRPC_ERROR_INTERNAL_SERVER                :                return "서버 내부 에러";
    case XMLRPC_ERROR_INVALID_SERVER_ID                :                return "서버 ID 유효하지 않음";
    case XMLRPC_ERROR_DISABLE_SERVER                :                return "서버 처리 불가 상태";
    case XMLRPC_ERROR_DISABLE_REQUEST                :                return "서버 해당 요청 처리 불가 상태";
    case XMLRPC_ERROR_INVALID_USER_IP                :                return "유저 IP 주소가 유효하지 않음";
    case XMLRPC_ERROR_NOT_MATCHED_USER_INFO            :                return "계정과 캐릭터 정보가 같지 않음";
    case XMLRPC_ERROR_INVALID_ACCOUNTUID            :                return "계정 정보가 유효하지 않음";
    case XMLRPC_ERROR_INVALID_CHARACTERUID            :                return "캐릭터 정보가 유효하지 않음";
    case XMLRPC_ERROR_INVALID_ITEMNUM                :                return "아이템 번호가 유효하지 않음";
    case XMLRPC_ERROR_OUT_OF_RANGE_ITEMCOUNT        :                return "아이템 수의 범위가 유효하지 않음";
    case XMLRPC_ERROR_INVALID_AUTHENTICATION_KEY    :                return "구매 상점 인증키가 유효하지 않음";
    case XMLRPC_ERROR_NOT_ACCEPTED_USER                :                return "해당 유저의 요청 처리 불가";
    case XMLRPC_ERROR_INVALID_ITEM_STORAGE            :                return "아이템 위치가 유효하지 않음";
    case XMLRPC_ERROR_NEED_MORE_CASH                :                return "구매에 필요한 캐시 부족";
    case XMLRPC_ERROR_BILLING_ERROR                    :                return "빌링 연동 에러";
    case XMLRPC_ERROR_INVALID_GET_CHARACTERUID        :                return "선물 받을 계정 정보가 유효하지 않음";
    case XMLRPC_ERROR_NOT_FOUND_BILLING_ITEM_LIST    :                return "빌링 목록에 없는 아이템 (구매 불가)";
    case XMLRPC_ERROR_UNKNOWN_ERROR                    :                return "알수 없는 에러";
    case XMLRPC_ERROR_NOT_FOUND_RPC_METHOD            :                return "RPC 메소드를 찾을 수 없음";
    case XMLRPC_ERROR_NOT_MATCHED_PARAMETER_TYPE    :                return "RPC 메소드의 파라미터 형식이 틀림";
    case XMLRPC_ERROR_INVALID_XML                    :                return "XML 형식이 틀림";
    */

    case XMLRPC_ERROR_INTERNAL_SERVER                :                return "XMLRPC_ERROR_INTERNAL_SERVER";
    case XMLRPC_ERROR_INVALID_SERVER_ID                :                return "XMLRPC_ERROR_INVALID_SERVER_ID";
    case XMLRPC_ERROR_DISABLE_SERVER                :                return "XMLRPC_ERROR_DISABLE_SERVER";
    case XMLRPC_ERROR_DISABLE_REQUEST                :                return "XMLRPC_ERROR_DISABLE_REQUEST";
    case XMLRPC_ERROR_INVALID_USER_IP                :                return "XMLRPC_ERROR_INVALID_USER_IP";
    case XMLRPC_ERROR_NOT_MATCHED_USER_INFO            :                return "XMLRPC_ERROR_NOT_MATCHED_USER_INFO";
    case XMLRPC_ERROR_INVALID_ACCOUNTUID            :                return "XMLRPC_ERROR_INVALID_ACCOUNTUID";
    case XMLRPC_ERROR_INVALID_CHARACTERUID            :                return "XMLRPC_ERROR_INVALID_CHARACTERUID";
    case XMLRPC_ERROR_INVALID_ITEMNUM                :                return "XMLRPC_ERROR_INVALID_ITEMNUM";
    case XMLRPC_ERROR_OUT_OF_RANGE_ITEMCOUNT        :                return "XMLRPC_ERROR_OUT_OF_RANGE_ITEMCOUNT";
    case XMLRPC_ERROR_INVALID_AUTHENTICATION_KEY    :                return "XMLRPC_ERROR_INVALID_AUTHENTICATION_KEY";
    case XMLRPC_ERROR_NOT_ACCEPTED_USER                :                return "XMLRPC_ERROR_NOT_ACCEPTED_USER";
    case XMLRPC_ERROR_INVALID_ITEM_STORAGE            :                return "XMLRPC_ERROR_INVALID_ITEM_STORAGE";
    case XMLRPC_ERROR_NEED_MORE_CASH                :                return "XMLRPC_ERROR_NEED_MORE_CASH";
    case XMLRPC_ERROR_BILLING_ERROR                    :                return "XMLRPC_ERROR_BILLING_ERROR";
    case XMLRPC_ERROR_INVALID_GET_CHARACTERUID        :                return "XMLRPC_ERROR_INVALID_GET_CHARACTERUID";
    case XMLRPC_ERROR_NOT_FOUND_BILLING_ITEM_LIST    :                return "XMLRPC_ERROR_NOT_FOUND_BILLING_ITEM_LIST";
    case XMLRPC_ERROR_UNKNOWN_ERROR                    :                return "XMLRPC_ERROR_UNKNOWN_ERROR";
    case XMLRPC_ERROR_NOT_FOUND_RPC_METHOD            :                return "XMLRPC_ERROR_NOT_FOUND_RPC_METHOD";
    case XMLRPC_ERROR_NOT_MATCHED_PARAMETER_TYPE    :                return "XMLRPC_ERROR_NOT_MATCHED_PARAMETER_TYPE";
    case XMLRPC_ERROR_INVALID_XML                    :                return "XMLRPC_ERROR_INVALID_XML";

    default:
        break;
    }

    return "XMLRPC_ERROR_UNKNOWN";
}
// end 2013-03-13 by hskim, 웹 캐시 상점