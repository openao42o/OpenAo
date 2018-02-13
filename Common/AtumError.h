#ifndef _ATUM_ERROR_H_
#define _ATUM_ERROR_H_

///////////////////////////////////////////////////////////////////////////////
// ERROR CODES
///////////////////////////////////////////////////////////////////////////////

/*
	- Common Error (0x2000 ~ 0x20FF): 공통으로 사용될 수 있는 에러들로 일단은 비워둡니다.
	- DB Error (0x2100 ~ 0x21FF)
	- Server Error (0x2200 ~ 0x22FF)
	- Protocol Error (0x2300 ~ 0x23FF)
	- Network Error (0x2400 ~ 0x24FF)
	- ClientSocket Error (0x2500 ~ 0x25FF)
	- ClientDirectX Error (0x2600 ~ 0x26FF)
	- ClientUser Error (0x2700 ~ 0x27FF)
	- Chatting Error (0x2800 ~ 0x28FF) 
*/

#ifndef	ERR_NO_ERROR
#define ERR_NO_ERROR								0x0000
#endif

/////////////////////////////////////
// Common Error (0x2000 ~ 0x20FF)
#define ERR_COMMON_UNKNOWN_ERROR						0x2000
#define ERR_COMMON_LOGIN_FAILED							0x2001
#define ERR_COMMON_NO_SUCH_CHARACTER					0x2002
#define ERR_COMMON_CHARACTER_ALREADY_EXISTS				0x2003
#define ERR_COMMON_TOO_LONG_PACKET						0x2004
#define ERR_COMMON_SERVICE_TEMPORARILY_PAUSED			0x2005
#define ERR_COMMON_INVALID_CLIENT_VERSION				0x2006
#define ERR_COMMON_NOT_ALLOWED_TO_MYSELF				0x2007		// 자기 자신에게는 안 됨
#define ERR_COMMON_INVALID_BUILDINGKIND					0x2008
#define ERR_COMMON_NO_SUCH_BUILDINGKIND					0x2009
#define ERR_COMMON_BAD_NETWORK							0x200A
#define ERR_COMMON_CONNECTING_GAME						0x200B		// 2008-11-26 by cmkwon, 대만 Netpower_Tpe 웹에서 아이템 추가 프로시저 추가 - 게임에 접속 중이다.
#define ERR_COMMON_DISABLE_CONTENT						0x200C		// 2011-10-05 by hskim, 파트너 시스템 2차 - 컨텐츠 OFF 에러 메시지 용

#define ERR_COMMON_SOCKET_CLOSED						0x20F0

/////////////////////////////////////
// DB Error (0x2100 ~ 0x21FF)
#define ERR_DB_ACCOUNT_ALREAY_EXISTS					0x2100		// 해당 아이디가 이미 존재(pk integrity constraint), 23000
#define ERR_DB_ACCOUNT_CHARACTER_NO_MATCH				0x2101		// 캐릭터와 account가 일치하지 않음(다른 사람의 캐릭터), SQL_NO_DATA
//#define ERR_DB_AUTH_FAILD								0x2102		// 아이디가 존재하지 않거나 패스워드가 틀린 경우, SQL_NO_DATA
#define ERR_DB_INTEGRITY_CONSTRAINT_VIOLATION			0x2103		// integrity constraint violation, 23000
#define ERR_DB_INVALID_PARAMETER						0x2104		// 파라미터 값 이상, 22018
#define ERR_DB_INVALID_UNIQUE_NUMBER					0x2105		// 캐릭터의 unique number가 존재하지 않음, SQL_NO_DATA
#define ERR_DB_NO_SUCH_ACCOUNT							0x2106		// 해당 이름의 account가 없음, SQL_NO_DATA
//#define ERR_DB_NO_SUCH_CHARACTER						0x2107		// 해당 이름의 캐릭터가 없음, SQL_NO_DATA
#define ERR_DB_NUMERIC_VALUE_OUT_OF_RANGE				0x2108		// 숫자값이 범위를 벗어남, 22003
#define ERR_DB_STRING_TOO_LONG							0x2109		// string이 schema의 길이보다 길 때, 22001
// item 관련 에러
#define ERR_DB_NO_SUCH_STORE_ITEM						0x210A		// DB에 해당 아이템이 존재하지 않음
#define ERR_DB_CANNOT_INSERT_DEFAULT_ITEMS				0x210B		// 기본 아이템 삽입 실패
#define ERR_DB_CONNECTION_ERROR							0x210C		// DB와의 연결이 실패
#define ERR_DB_EXECUTION_FAILED							0x210D		// SQLExecDirect() 실패
#define ERR_DB_NO_SUCH_DATA								0x210E		// 2008-06-12 by dhjin, EP3 데이타 값 없을 경우 에러 추가
#define ERR_DB_INSERT_QUERY_ERROR						0x210F		// 2008-11-26 by cmkwon, 대만 Netpower_Tpe 웹에서 아이템 추가 프로시저 추가 - 

/////////////////////////////////////
// PROTOCOL Error (0x2100 ~ 0x21FF)
#define ERR_PROTOCOL_INVALID_PROTOCOL_TYPE				0x2400		// Client로 부터 받은 Protocol Type이 유효하지 않음
#define ERR_PROTOCOL_INVALID_FIELD_DATA					0x2401		// Client로 부터 받은 Data Size가 Field Type에 따른 Data Size보다 작음
#define ERR_PROTOCOL_INVALID_ACCOUNT_UNIQUENUMBER		0x2402		// AccountUniqueNumber가 유효하지 않음
#define ERR_PROTOCOL_INVALID_CHARACTER_UNIQUENUMBER		0x2403		// CharacterUniqueNumber가 유효하지 않음
#define ERR_PROTOCOL_INVALID_UNITKIND					0x2404		// Unit Kind가 유효하지 않음
#define ERR_PROTOCOL_INVALID_CLIENTINDEX				0x2405		// 자신 또는 Target의 ClientIndex가 유효하지 않음
#define ERR_PROTOCOL_INVALID_PACKET						0x2406		// 받은 패킷이 유효하지 않음
#define ERR_PROTOCOL_INVALID_PACKET_SIZE				0x2407		// 받은 패킷의 Size가 유효하지 않음
#define ERR_PROTOCOL_INVALID_SEQUENCE_NUMBER			0x2408		// 받은 패킷의 Sequence Number가 유효하지 않음
#define ERR_PROTOCOL_INVALID_PEER						0x2409		// UDP로 받은 패킷의 IP와 Port가 서버에 설정된 Client의 IP, Port와 같지 않다
#define ERR_PROTOCOL_INVALID_STAT_VALUE_OR_KIND			0x240A		// 보너스 포인트가 없거나 Stat의 종류가 유효하지 않다
#define ERR_PROTOCOL_EMPTY_ACCOUNTNAME					0x2410		// AccountName이 비어 있음
#define ERR_PROTOCOL_DUPLICATE_LOGIN					0x2411		// 이중 로그인
#define ERR_PROTOCOL_NOT_LOGINED						0x2412		// Login 하지 않았음
#define ERR_PROTOCOL_INVALID_SERVER_GROUP_NAME			0x2413		// Server Group Name이 유효하지 않음
#define ERR_PROTOCOL_LIMIT_GROUP_USER_COUNT				0x2414		// Limit Server Group User Count에 걸림
#define ERR_PROTOCOL_ACCOUNT_BLOCKED					0x2415		// 계정 압류
#define ERR_PROTOCOL_INVALID_TARGET_INDEX				0x2416		// Target Index 가 유효하지 않음
#define ERR_PROTOCOL_LIMIT_STAT_POINT					0x2417		// 최대 스탯 포인트임
#define ERR_PROTOCOL_INVALID_GETCHARACTER				0x2418		// 유효하지 않은 GetCharacter 메시지
#define ERR_PROTOCOL_INVALID_CHARACTER_STATE			0x2419		// Character가 유효하지 않음

#define ERR_PROTOCOL_ALREADY_MAX_CHARACTER				0x2420		// 더이상 Character를 만들수 없음, 최대 3(SIZE_MAX_NUM_CHARACTER)
#define ERR_PROTOCOL_EMPTY_CHARACTERNAME				0x2421		// CharacterName이 비어있음
#define ERR_PROTOCOL_STATE_MINUSVALUE					0x2422		// 받은 State 값중에 마이너스 값이 있음
#define ERR_PROTOCOL_STATE_SUMISNOTBONUS				0x2423		// 받은 State 세팅값의 합이 [20]이 아님
#define ERR_PROTOCOL_INVALID_GAMESTART					0x2424		// 유효하지 않은 Game Start 메시지
#define ERR_PROTOCOL_INVALID_SOCKET_FORNPC				0x2425		// NPC 서버를 위한 IOCPSocket이 유효 하지 않다.
#define ERR_PROTOCOL_INVALID_CONNECT_GAMESTART			0x2426		// 유효하지 않은 Connect Game Start 메시지
#define ERR_PROTOCOL_INVALID_CONNECT_WARP				0x2427		// 유효하지 않은 Warp Connect 메시지
#define ERR_PROTOCOL_INVALID_POSITION					0x2428		// 클라이언트의 Position이 유효하지 않다.
#define ERR_PROTOCOL_INVALID_GAMEEND					0x2429		// GameEnd가 유효 하지 않음
#define ERR_PROTOCOL_GAMESTARTROUTINE_FAILED			0x242A		// GameStartRoutine() 수행 실패
#define ERR_PROTOCOL_INVALID_CHARACTER_NAME				0x242B		// 잘못된 캐릭터 이름
#define ERR_PROTOCOL_INVALID_PEER_CHARACTER				0x242C		// 상대방이 유효하지 않습니다.
#define ERR_PROTOCOL_INVALID_AUTOSTAT_TYPE				0x242D		// 자동분배스탯타입이 유효하지 않음
#define ERR_PROTOCOL_NOT_TUTORIAL_MAP					0x242E		// 튜토리얼 맵이 아니다
#define ERR_PROTOCOL_RACE_PERMISSION_DENIED				0x242F		// 권한이 없음
#define ERR_PROTOCOL_NOT_ENOUGH_ELAPSE_TIME				0x2430		// 재사용 시간이 경과하지 않음
#define ERR_PROTOCOL_INVALID_CHARACTERNAME				0x2431		// CharacterName이 유효하지 않음
#define ERR_PROTOCOL_CHARACTER_MODE_NOT_MATCHED			0x2432		// CharacterMode가 동일하지 않음
#define ERR_PROTOCOL_UNKNOWN_CHARACTER_ACTION_TYPE		0x2433		// 알수 없는 CharacterActionType
#define ERR_PROTOCOL_BUILDING_KIND_NOT_MATCHED			0x2434		// BuildingKind가 동일하지 않음

#define ERR_PROTOCOL_INVALID_BODYCONDITION				0x2440		// body condition이 잘 못 됨(ex: warp시에 warping이 아님)
#define ERR_PROTOCOL_INVALID_MAP_EVENT_INFO				0x2441		// map의 event_info 구조체의 정보가 잘 못 되었을 때
#define ERR_PROTOCOL_MAP_ALREADY_SERVED_BY_FEILD_SERVER	0x2442		// 다른 field server가 이미 map을 등록하였음
#define ERR_PROTOCOL_NO_SUCH_FIELD_SERVER				0x2443		// 해당 field server가 없음
#define ERR_PROTOCOL_NO_SUCH_MAP_SERVED					0x2444		// 해당 맵이 등록되어 있지 않음
#define ERR_PROTOCOL_NO_SUCH_CHANNEL					0x2445		// 해당 채널이 없음
#define ERR_PROTOCOL_MAP_CHANNEL_NOT_MATCHED			0x2446		// 맵채널이 일치하지 않음
#define ERR_PROTOCOL_CANNOT_WARP						0x2447		// WarpToMap()함수 처리 중 에러를 리턴하여 워프 불가
#define ERR_PROTOCOL_CANNOT_WARP_INVALID_STATE			0x2448		// 유효하지 않은 상태(죽은 상태...)이기 때문에 워프 불가
#define ERR_PROTOCOL_DOING_WARP							0x2449		// 워프중

#define ERR_PROTOCOL_INVALID_FIELDSERVER_CLIENT_STATE	0x2450		// Field Server 내에서의 client state가 잘못됨
#define ERR_PROTOCOL_INVALID_PRESERVER_CLIENT_STATE		0x2451		// Pre Server내에서의 client state가 잘못됨
#define ERR_PROTOCOL_NO_SUCH_SERVER_GROUP				0x2452		// Field server가 보낸 서버군이 없음
#define ERR_PROTOCOL_FIELD_SERVER_ALREADY_REGISTERD		0x2453		// 해당 field server가 이미 등록되었음
#define ERR_PROTOCOL_FIELD_SERVER_ID_NOT_MATCHED		0x2454		// PreServer에 저장된 (client가 접속해야 할)FieldServer의 ID가, 질의를 요청하는 FieldServer의 ID와 다름(AUTH_USER 등에서...)
#define ERR_PROTOCOL_ALL_FIELD_SERVER_NOT_ALIVE			0x2455		// 모든 field server가 다 죽었음 =.=
#define ERR_PROTOCOL_NO_SUCH_SHOP						0x2456		// 해당 샾이 없음
#define ERR_PROTOCOL_NO_SUCH_SHOP_ITEM					0x2457		// 샾에서 해당 아이템을 팔지 않음
#define ERR_PROTOCOL_NOT_ENOUGH_MONEY					0x2458		// 돈 부족, 아이템 구매 불가
#define ERR_PROTOCOL_ITEM_KIND_NOT_MATCHED				0x2459		//
#define ERR_PROTOCOL_NO_SUCH_ITEM						0x245A		// 해당 아이템이 없음
#define ERR_PROTOCOL_ITEM_AMOUNT_EXCEED					0x245B		// 가진 개수보다 아이템 수가 많음
#define ERR_PROTOCOL_CLIENT_IP_NOT_MATCHED				0x245C		//
#define ERR_PROTOCOL_IMSERVER_ALREADY_CONNECTED			0x245D		// pre server에 해당 그룹의 im server가 이미 연결되어 있음
#define ERR_PROTOCOL_GENERAL_ITEM_ERROR					0x245E		// 일반 아이템 에러
#define ERR_PROTOCOL_IM_SERVER_NOT_ALIVE				0x245F		// IM Server가 실행되어있지 않음
// 2007-10-12 by dhjin, 번호 다 찼음
//////////////////////////////////////////////////////////////////////////

// 파티 관련 에러
#define ERR_PROTOCOL_PARTY_MASTER_NOT_MATCHED			0x2460		// 파티장 불일치
#define ERR_PROTOCOL_NO_SUCH_PARTY_MEMBER				0x2461		// 해당 파티원 없음
#define ERR_PROTOCOL_PARTY_PERMISSION_DENIED			0x2462		// 권한이 없음
#define ERR_PROTOCOL_PARTY_MEMBER_ALREADY_EXISTS		0x2463		// 이미 가입했음
#define ERR_PROTOCOL_NO_SUCH_PARTY						0x2464		// 해당 파티가 없음
#define ERR_PROTOCOL_CANNOT_INVITE_USER					0x2465		// 초대할 수 없음
#define ERR_PROTOCOL_GET_MEMBER_FAIL					0x2466		// 파티원 정보를 읽을 수 없음(게임중이 아니거나 다른 맵에 있음)
#define ERR_PROTOCOL_PARTY_ID_NOT_MATCHED				0x2467		// 파티 아이디 불일치
#define ERR_PROTOCOL_ALREADY_MAX_PARTY_MEMBER			0x2468		// 파티 정원 초과
#define ERR_PROTOCOL_ALREADY_PARTY_BATTLE				0x2469		// 이미 파티전 중임
#define ERR_PROTOCOL_ALREADY_REQUESTING_PARTY_BATTLE	0x246A		// 이미 파티전 요청중임
#define ERR_PROTOCOL_DOING_PARTY_BATTLE					0x246B		// 파티전 중임
#define ERR_PROTOCOL_IMPOSSIBLE_IN_PARTY				0x246C		// 파티시에는 불가

// 길드 관련 에러
#define ERR_PROTOCOL_NOT_ENOUGH_PARTY_MEMBER			0x2470		// 길드를 만들기 위한 최소한 파티원 수가 부족함
#define ERR_PROTOCOL_GUILD_NAME_ALREADY_EXISTS			0x2471		// 같은 이름의 길드가 존재함
#define ERR_PROTOCOL_GUILD_MEMBER_ALREADY_EXISTS		0x2472		// 이미 가입했음
#define ERR_PROTOCOL_GUILD_PERMISSION_DENIED			0x2473		// 권한이 없음
#define ERR_PROTOCOL_NOT_HAVE_GUILD						0x2474		// 길드에 가입하지 않았음
#define ERR_PROTOCOL_GUILD_NOT_MATCHED					0x2475		// 속한 길드가 일치하지 않음
#define ERR_PROTOCOL_NO_SUCH_GUILD_MEMBER				0x2476		// 해당 파티원 없음
#define ERR_PROTOCOL_NOT_HAVE_PARTY						0x2477		// 파티에 가입하지 않았음
#define ERR_PROTOCOL_NO_SUCH_GUILD						0x2478		// 해당 길드가 없음
#define ERR_PROTOCOL_NOT_GUILD_MASTER					0x2479		// 길드장이 아니다
#define ERR_PROTOCOL_GUILD_CARD_ALREADY_USING			0x247A		// 여단카드가 이미 사용중임
#define ERR_PROTOCOL_INVLID_GUILDNAME					0x247B		// 길드명이 유효하지 않다, 2006-03-06 by cmkwon
#define ERR_PROTOCOL_NOT_GET_GUILDSTORE_MEMBER			0x247C		// 2006-09-25 by dhjin, 여단 창고 이용 가능한 유저가 아니다 대대장 이상 사용가능
#define ERR_PROTOCOL_DOING_GUILDWAR_BATTLE				0x247D		// 여단전 중임
#define ERR_PROTOCOL_INVLID_GuildMarkSize				0x247E		// 2007-08-02 by cmkwon, 여단 마크 심사 시스템 구현 - 여단마크 사이즈 오류
#define ERR_PROTOCOL_OVER_MEMBERCOUNT					0x247F		// 2008-05-27 by dhjin, EP3 여단 수정 사항 - 여단원 증가 캐쉬 아이템, 최대 여단원 수 넘김

// Trade 관련 error
#define ERR_PROTOCOL_TOO_FAR_TO_TRADE					0x2480		// 두 캐릭의 거리가 멀어서 거래가 성사 안 됨
#define ERR_PROTOCOL_ALREADY_TRADING					0x2481		// 이미 거래 중임
#define ERR_PROTOCOL_PEER_TRADER_NOT_MATCHED			0x2482		// 거래 상대가 틀림
#define ERR_PROTOCOL_TRADE_NOT_STARTED					0x2483		// 거래가 시작되지 않았음
#define ERR_PROTOCOL_INVALID_ITEMNUM					0x2484		// ItemNum이 잘못되었음
#define ERR_PROTOCOL_INVALID_ITEM_COUNT					0x2485		// Item 개수가 invalid함, 예) energy류가 아닌데 거래 수량이 여러개이다
#define ERR_PROTOCOL_TRADE_ERROR						0x2486		// 일반 거래 오류
#define ERR_PROTOCOL_INVALID_ITEM_KIND					0x2487		// Item Kind가 잘못되었음
#define ERR_PROTOCOL_MIN_TRADE_QUANTITY_ERROR			0x2488		// 최소 거래 단위 수량 에러
#define ERR_PROTOCOL_ALREADY_TRADE_ITEM					0x2489		// 이미 올려진 아이템이다(넌카운터블 아이템)

// Skill 관련 error
#define ERR_PROTOCOL_NO_SUCH_SKILL						0x2490		// 스킬이 존재하지 않음
#define ERR_PROTOCOL_CANNOT_USE_SKILL					0x2491		// 스킬을 사용할 수 없음, 예) level 부족, 공격 무지 미장전
#define ERR_PROTOCOL_INVALID_SKILLTYPE					0x2492		// 스킬 타입이 잘 못 되었음
#define ERR_PROTOCOL_NOT_ENOUGH_SP						0x2493		// SP 부족(스킬 사용 불가)

// Timer 관련 error
#define ERR_PROTOCOL_NO_SUCH_TIME_LIMIT_EVENT_IN_BUCKET	0x24A0		// 해당 이벤트가 TIMER_EVENT_BUCKET 안에 없음
#define ERR_PROTOCOL_NO_SUCH_TIMER_EVENT_TYPE			0x24A1		// 해당 이벤트 type이 안에 없음

// ENCHANT 관련 에러
#define ERR_PROTOCOL_ALREADY_MAX_ENCHANT_PER_ITEM		0x24A2		// 최대 인챈트 개수 초과
#define ERR_PROTOCOL_ALREADY_HAVE_RARE_FIX				0x24A3		// 이미 Rare Fix를 가진 아이템임
#define ERR_PROTOCOL_ALREADY_INITIALIZE_RARE_FIX		0x24A4		// 이미 Rare Fix를 가진 아이템임
#define ERR_PROTOCOL_CANNOT_USE_ITEMKIND_PREVENTION_DELETE_ITEM		0x24A5	// 파방(파괴방지) 사용 할 수 없다
#define ERR_PROTOCOL_CANNOT_USE_ITEMKIND_PREVENTION_DELETE_ITEM_BY_STARTCOUNT		0x24A6	// 2009-01-19 by dhjin, 인첸트 확률 증가, 10인첸 파방 카드 - 사용 가능한 인첸트 수가 아니다.

// 공격 & 움직임 관련 에러
#define ERR_PROTOCOL_NOT_ENOUGH_EP						0x24B0		// EP 부족(부스터 작동 불가, beam류 발사 불가)
#define ERR_PROTOCOL_TOO_LONG_DISTANCE					0x24B1		// 거리가 너무 멀다(ex. SIZE_VISIBLERECT_X이상임)
#define ERR_PROTOCOL_ALREADY_P2P_PK						0x24B2		// 이미 일대일 PK 중임
#define ERR_PROTOCOL_TOO_FAR_TO_P2P_PK					0x24B3		// 너무 멀어서 일대일 PK 불가
#define ERR_PROTOCOL_HIGH_LEVEL_GAP_TO_P2P_PK			0x24B4		// 레벨 차가 많이 나서 PK 불가
#define ERR_PROTOCOL_NOT_DOING_P2P_PK					0x24B5		// PK 중이 아님
#define ERR_PROTOCOL_NOT_ALLOWED_ATTACK					0x24B6		// 공격 불가

#define ERR_PROTOCOL_MULTIPLE_IP_CONNECTION				0x24C0		// 2015-11-24 Future, Restrict Multiple IP Access

// Item 관련 에러
#define ERR_PROTOCOL_CANNOT_USE_ITEM							0x2600		// 아이템 사용 불가
#define ERR_PROTOCOL_ALREADY_HAVE_ITEM							0x2601		// 이미 아이템을 가지고 있습니다
#define ERR_PROTOCOL_ALREADY_HAVE_SKILL							0x2602		// 이미 스킬을 가지고 있습니다
#define ERR_PROTOCOL_INVALID_ITEM_WINDOW_POSITION				0x2603		// 잘못된 Item Window Position
#define ERR_PROTOCOL_ALREADY_ITEM_ACTIVATED						0x2604		// 이미 아이템이 수행 중(dummy류 등)
#define ERR_PROTOCOL_NO_SUCH_DES_PARAM							0x2605		// DES_XXX이 잘못되었음
#define ERR_PROTOCOL_NOT_ENOUGH_BULLET							0x2606		// 총알(탄두)가 부족함
#define ERR_PROTOCOL_ITEM_CANNOT_TRANSFER						0x2607		// 아이템을 이동할 수 없습니다.
#define ERR_PROTOCOL_NO_SUCH_MIXING_INFO						0x2608		// 아이템 조합 정보가 없습
#define ERR_PROTOCOL_ITEM_OVER_WEIGHT							0x2609		// 적재량 초과
#define ERR_PROTOCOL_DO_NOT_HAVE_LINK_ITEM						0x260A		// 링크 아이템을 가지고 있지 않음
#define ERR_PROTOCOL_INVENTORY_FULL_ITEM						0x260B		// 인벤토리가 다 참
#define ERR_PROTOCOL_STORE_FULL_ITEM							0x260C		// 창고가 다 참
#define ERR_PROTOCOL_ALREADY_USE_ITEM							0x260D		// 이미 사용 중인 아이템이다
#define ERR_PROTOCOL_ALREADY_USE_OTHER_ITEM						0x260E		// 이미 다른 사용 중인 아이템이 있다(같은 ItemNum)
#define ERR_PROTOCOL_STAT_ERROR_STATE							0x260F		// 현재 스탯이 오류 상태임
#define ERR_PROTOCOL_STAT_INITIALIZE_STATE						0x2610		// 스탯이 초기화 상태임
#define ERR_PROTOCOL_AREADY_USING_HP_UP_ITEM					0x2611		// 이미 HP UP 아이템을 사용중임
#define ERR_PROTOCOL_AREADY_USING_DP_UP_ITEM					0x2612		// 이미 DP UP 아이템을 사용중임
#define ERR_PROTOCOL_AREADY_USING_SP_UP_ITEM					0x2613		// 이미 SP UP 아이템을 사용중임
#define ERR_PROTOCOL_AREADY_USING_EP_UP_ITEM					0x2614		// 이미 EP UP 아이템을 사용중임
#define ERR_PROTOCOL_AREADY_FULL_HP								0x2615		// 이미 HP 가 만땅임
#define ERR_PROTOCOL_AREADY_FULL_DP								0x2616		// 이미 DP 가 만땅임
#define ERR_PROTOCOL_AREADY_FULL_SP								0x2617		// 이미 SP 가 만땅임
#define ERR_PROTOCOL_AREADY_FULL_EP								0x2618		// 이미 EP 가 만땅임
#define ERR_PROTOCOL_CANNOT_USEITEM_IN_PARTY					0x2619		// 파티 참여중에는 아이템 사용 불가
#define ERR_PROTOCOL_CANNOT_IN_CITY_MAP_CHANNEL					0x261A		// 도시맵에서는 사용 불가
#define ERR_PROTOCOL_CHANNEL_USER_OVERFLOW						0x261B		// 채널 사용자가 많다.
#define ERR_PROTOCOL_NO_SUCH_EVENT_AREA_INDEX					0x261C		// 지정한 Event Area Index를 가진 이벤트를 찾을수 없다.
#define ERR_PROTOCOL_MAX_ITEM_COUNTS_OVER						0x261D		// 카운터블 아이템의 최대 카운트 오류(MAX_ITEM_COUNTS - 20억개)
#define ERR_PROTOCOL_NOTIFY_MAX_ITEM_COUNTS_OVER				0x261E		// 카운터블 아이템의 최대 카운트 오류가 발생 할 수 있음을 알림
#define ERR_PROTOCOL_CANNOT_UNWEAR_ARMOR						0x261F		// 2005-10-31 by cmkwon, 아머는 장착 해제 할 수 없다, 교체만 가능하다
#define ERR_PROTOCOL_NO_SUCH_BULLET_ITEM						0x2620		// 2005-11-02 by cmkwon, 총알 아이템이 없다
#define ERR_PROTOCOL_CANNOT_ENCHANT_ITEM						0x2621		// 2005-11-21 by cmkwon, 인챈트/갬블을 할 수 없는 아이템
#define ERR_PROTOCOL_BAZAAR_NO_SUCH_ITEM						0x2622		// 2006-08-02 by dhjin, 개인상점시 찾을 수 없는 아이템일때
#define ERR_PROTOCOL_GUILD_STORE_FULL_ITEM						0x2623		// 2006-09-23 by dhjin, 길드 창고 다 참
#define ERR_PROTOCOL_CANNOT_USE_ITEM_IN_ARENA					0x2624		// 2007-06-01 by dhjin, 아레나 맵에서 사용 불가 아이템
#define ERR_PROTOCOL_INVALID_MIXING_INFO						0x2625		// 2009-10-01 by cmkwon, 그래픽 리소스 변경 관련 초기화 기능 구현 - 
#define ERR_PROTOCOL_NOT_WEARING_STATE							0x2626		// 2009-09-09 ~ 2010-02-10 by dhjin, 인피니티 - 발동류장착아이템, 장착 상태가 아니다. 
#define ERR_PROTOCOL_INVALID_INVOKING_TYPE						0x2627		// 2009-09-09 ~ 2010-02-10 by dhjin, 인피니티 - 발동류장착아이템, 발동 타잎 오류
#define ERR_PROTOCOL_FAIL_INVOKING_BY_RATE						0x2628		// 2009-09-09 ~ 2010-02-10 by dhjin, 인피니티 - 발동류장착아이템, 발동 미스
#define ERR_PROTOCOL_FAIL_INVOKING_BY_REQITEMNUM				0x2629		// 2009-09-09 ~ 2010-02-10 by dhjin, 인피니티 - 발동류장착아이템, 발동 조건 값 오류
#define ERR_PROTOCOL_COOLING_TIME_INVOKING_ITEM					0x262A		// 2009-09-09 ~ 2010-02-10 by dhjin, 인피니티 - 발동류장착아이템, 발동류 아이템 쿨타임중
#define ERR_PROTOCOL_CANNOT_RARE_ITEM							0x262B		// 2009-09-09 ~ 2010-02-10 by dhjin, 인피니티 - 발동류장착아이템
#define ERR_PROTOCOL_NOT_ACTIVATED_ITEM							0x262C		// 2011-09-20 by hskim, 파트너 시스템 2차 - 활성화 되지 않은 아이템
#define ERR_PROTOCOL_CANNOT_ATTACHED_ITEM						0x262D		// 2011-09-20 by hskim, 파트너 시스템 2차 - 장착되지 않은 아이템

#define ERR_PROTOCOL_CANNOT_ENCHANT_MORE_THEN_10				0x262E		// 2011-10-19 by hskim, EP4 [Free to play] - 10 회 인첸트 아이템 기능 구현 - 10 인챈트 이상 아이템은 강화할 수 없음
#define ERR_PROTOCOL_CANNOT_ENCHANT_DIFFERENCE_ATTRIBUTE		0x262F		// 2011-10-19 by hskim, EP4 [Free to play] - 10 회 인첸트 아이템 기능 구현 - 인챈트 속성이 틀려 인챈트 할 수 없음
#define ERR_PROTOCOL_CANNOT_ENCHANT_LACK_REQUIREMENT			0x2630		// 2011-10-19 by hskim, EP4 [Free to play] - 접두/접미 고정 옵션 - 요구 조건에 부적함

// 요구 조건 관련 에러			
#define ERR_PROTOCOL_REQ_EXPERIENCE_NOT_MATCHED					0x2640
#define ERR_PROTOCOL_REQ_RACE_NOT_MATCHED						0x2641		//
#define ERR_PROTOCOL_REQ_ATTACK_PART_NOT_MATCHED				0x2642		//
#define ERR_PROTOCOL_REQ_DEFENSE_PART_NOT_MATCHED				0x2643		//
#define ERR_PROTOCOL_REQ_FUEL_PART_NOT_MATCHED					0x2644		//
#define ERR_PROTOCOL_REQ_SOUL_PART_NOT_MATCHED					0x2645		//
#define ERR_PROTOCOL_REQ_SHIELD_PART_NOT_MATCHED				0x2646		//
#define ERR_PROTOCOL_REQ_DODGE_PART_NOT_MATCHED					0x2647		//
#define ERR_PROTOCOL_REQ_UNITKIND_NOT_MATCHED					0x2648		//
#define ERR_PROTOCOL_REQ_LEVEL_NOT_MATCHED						0x2649		//
#define ERR_PROTOCOL_REQ_MINLEVEL_NOT_MATCHED					0x264A		// 최소 레벨보다 레벨이 낮다
#define ERR_PROTOCOL_REQ_MAXLEVEL_NOT_MATCHED					0x264B		// 최대 레벨보다 레벨이 높다
#define ERR_PROTOCOL_REQ_ITEM_NOT_MATCHED						0x264C		//
#define ERR_PROTOCOL_REQ_QUEST_NOT_MATCHED						0x264D		//
#define ERR_PROTOCOL_REQ_NUM_OF_CHARACTER_NOT_MATCHED			0x264E		//
#define ERR_PROTOCOL_TOO_FAR_TO_DO								0x264F		// 뭔가를 하기에 거리가 너무 멀다
#define ERR_PROTOCOL_NO_SUCH_REQUEST_TYPE						0x2650		// 해당 REQUEST_TYPE 이 없다
#define ERR_PROTOCOL_LOW_PROBABILITY							0x2651		// 확률 부족으로 인한 에러
#define ERR_PROTOCOL_REQ_NOT_CITY_MAP_CHANNEL					0x2652		// 격납고(도시맵)가 아니다
#define ERR_PROTOCOL_REQ_MUST_RELEASE_ALL_ITEM					0x2653		// 반드시 모든 아이템을 장착 해제 해야함
#define ERR_PROTOCOL_REQ_WARP_REQUIREMENTS_NOT_MATCHED			0x2654		// 워프 요구 조건 에러
#define ERR_PROTOCOL_REQ_INFLUENCE_NOT_MATCHED					0x2655		// 세력
#define ERR_PROTOCOL_REQ_MAP_OBJECT_NOT_MATCHED					0x2656		// 지정한 Map Object와 다르다
#define ERR_PROTOCOL_REQ_MAP_AREA_NOT_MATCHED					0x2657		// 지정한 Map Area와 다르다, 2005-08-29 by cmkwon
#define ERR_PROTOCOL_REQ_MONSTER_NOT_MATCHED					0x2658		// 요구 몬스터 카운트와 다르다
#define ERR_PROTOCOL_REQ_PARTYMEMBERS_NOT_MATCHED				0x2659		// 파티가 없거나 요구 파티원수와 다르다.
#define ERR_PROTOCOL_REQ_PARTYMEMBERS_LEVEL_NOT_MATCHED			0x265A		// 최고레벨 파티원과 최저레벨 차가 10이상 이다
#define ERR_PROTOCOL_REQ_PARTYMEMBERS_SOMEONE_CANNOT_WARP		0x265B		// 파티원중 누군가 워프 할 수 없는 상태이다
#define ERR_PROTOCOL_REQ_PARTYMEMBERS_SOMEONE_NOT_COMPLETION	0x265C		// 파티원중 누군가 퀘스트 완료 상태가 아니다.
#define ERR_PROTOCOL_REQ_MIX_ITEM_NOT_MATCHED					0x265D		// 2007-04-02 by cmkwon, 퀘스트 완료 조건에 조합 결과물이 필요함
#define ERR_PROTOCOL_REQ_PW_NOT_MATCHED							0x265E		// 2008-06-03 by dhjin, EP3 편대 수정 - PW오류
#define ERR_PROTOCOL_NOT_WARP_BURNING_MAP						0x265F		// 2010-11-24 by shcho, 콜오브 히어로 사용시 버닝타임 맵이면 소환불가 처리

// Quest 관련 에러
#define ERR_PROTOCOL_NO_SUCH_CHARACTER_QUEST					0x2700
#define ERR_PROTOCOL_QUEST_ALREADY_COMPLETED					0x2701		// 이미 수행 완료된 quest임
#define ERR_PROTOCOL_QUEST_NOT_IN_PROGRESS						0x2702		// 진행중인 퀘스트가 아님
#define ERR_PROTOCOL_QUEST_RESULT_PROCESS_FAILED				0x2703		// 퀘스트 결과 처리 실패
#define ERR_PROTOCOL_QUEST_GENERAL_ERROR						0x2704		// 퀘스트 일반 에러
#define ERR_PROTOCOL_TIME_LIMIT_ERROR							0x2705		// 시간 제한으로 인한 에러
#define ERR_PROTOCOL_REQ_NOT_GUILD_MASTER						0x2706		// 길드장만 가능한 퀘스트이다
#define ERR_PROTOCOL_QUEST_ALREADY_EXIST_CITYWAR				0x2707		// 이미 다른 도시점령전 퀘스트가 진행중이다
#define ERR_PROTOCOL_QUEST_NEVER_COMPLITION_BY_USER				0x2708		// 유저에 의해서 퀘스트가 완료불가능
#define ERR_PROTOCOL_QUEST_INVALID_CITYWAR_QUEST_INDEX			0x2709		// 퀘스트 인덱스가 유효하지 않다
#define ERR_PROTOCOL_QUEST_CANNOT_CITYWAR_QUEST					0x270A		// 도시점령전 퀘스트를 받을수 없다(점령전이 진행중인 상태)
#define ERR_PROTOCOL_QUEST_CITYWAR_MGAMESERVERID_NOT_MATCHED	0x270B		// 도시점령전 퀘스트의 취소는 받은 서버에서만 가능하다
#define ERR_PROTOCOL_QUEST_COUPON_INVALID_NUMBER				0x2710		// 쿠폰번호가 DB에 존재하지 않거나
#define ERR_PROTOCOL_QUEST_COUPON_ALREADY_USED					0x2711		// 이미 사용된 쿠폰
#define ERR_PROTOCOL_QUEST_COUPON_EXPIRED						0x2712		// 쿠폰 유효기간 경과
#define ERR_PROTOCOL_QUEST_COUPON_USE_ERROR						0x2713		// 쿠폰 사용 Error
#define ERR_PROTOCOL_QUEST_NO_LEVEL_UP							0x2714		// 레벨업이 되지 않음
#define ERR_PROTOCOL_QUEST_IS_IN_PROGRESS						0x2715		// 퀘스트가 진행중임
#define ERR_PROTOCOL_QUEST_IMPOSSIBLE_GIVEUP					0x2716		// 포기 할 수 없는 퀘스트.

// 서비스 관련 에러
#define ERR_PROTOCOL_QUEST_SERVICE_PAUSED						0x2740		// 퀘스트 기능 일시 정지됨

// GET_INFO 관련 에러
#define ERR_PROTOCOL_NO_SUCH_MONSTER_INFO						0x2780
#define ERR_PROTOCOL_NO_SUCH_MAPOBJECT_INFO						0x2781
#define ERR_PROTOCOL_NO_SUCH_QUEST_INFO							0x2782
#define ERR_PROTOCOL_NO_SUCH_ITEM_INFO							0x2783

#define ERR_PROTOCOL_SELECTIVE_SHUTDOWN_NOT_ALLOWED_TIME		0x2790		// 2012-07-11 by hskim, 선택적 셧다운 - 현재 시간에는 게임을 플레이 할수 없음
#define ERR_PROTOCOL_SELECTIVE_SHUTDOWN_APPLY_LOGOUT			0x2791		// 2012-07-11 by hskim, 선택적 셧다운 - 선택적 셧다운 적용

///////////////////////////////////////////////////////////////////////////////
// Chatting Error (0x2800 ~ 0x28FF)
#define ERR_CHAT_CHARACTER_NAME_NOT_MATCHED				0x2800		// 캐릭터 이름 불일치
#define ERR_CHAT_NOT_ALLOWED_STRING						0x2801		// 안 좋은 단어가 사용됨
#define ERR_CHAT_PERMISSION_DENIED						0x2802		// 해당 채팅 권한 부족
#define ERR_CHAT_INVALID_COMMAND						0x2803		// 해당 채팅 권한 부족
#define ERR_CHAT_CHAT_NOT_TRANSFERED					0x2804		// 해당 채팅이 전달되지 않았습니다
#define ERR_CHAT_CHAT_BLOCKED							0x2805		// 채팅금지 상태
	

// 친구, 거부 목록 관련 에러
#define ERR_FRIEND_INVALID_CHARACTER_NAME				0x2900		// 캐릭터이름이 비어있거나 유효하지 않다
#define ERR_FRIEND_REGISTERED_PEER_REJECTLIST			0x2901		// 상대방 거부리스트에 등록되어 있음
#define ERR_FRIEND_ALREADY_REGISTERED					0x2902		// 이미 리스트(친구, 거부)에 등록되어있음
#define ERR_FRIEND_ALREADY_MAX_LIST						0x2903		// 이미 리스트 최대인원이 등록되어있음
#define ERR_FRIEND_NO_SUCH_CHARACTER_IN_LIST			0x2904		// 리스트에서 캐릭터이름이 없음
	
// Countdown 관련 에러	
#define ERR_COUNTDOWN_INVALID_COUNTDOWN_TYPE			0x2910		// Countdown Type이 유효하지 않음
#define ERR_COUNTDOWN_NOT_ENOUGH_TIME					0x2911		// 시간이 경과 되지 않았다
#define ERR_INTERVAL_SYSTEM_SECOND						0x2912		// 2008-08-18 by dhjin, 1초 텀 시스템 기획안

// 도시점령전 관련
#define ERR_CITYWAR_NO_SUCH_CITYWAR_INFO				0x2A00		// 도시점령전 관련 정보가 없다
#define ERR_CITYWAR_NOT_MATCHED_CITY_MAP_INDEX			0x2A01		// 도시점령전 도시 맵이 아니다
#define ERR_CITYWAR_SETOCCUPYINFO_PERMISSION_DENIED		0x2A02		// 도시점령전 설정 권한 부족
#define ERR_CITYWAR_SETOCCUPYINFO_IMPOSSIBLE_STATE		0x2A03		// 도시점령전 설정 불가능 상태이다(Nomal 상태가 아니다)
#define ERR_CITYWAR_SETOCCUPYINFO_IMPOSSIBLE_TIME		0x2A04		// 현재 시간이 도시점령전 설정 불가능 시간이다
#define ERR_CITYWAR_SETOCCUPYINFO_INVALID_WARTIME		0x2A05		// 도시점령전 설정 시간이 유효하지 않다

// MGameMoney/MGameCash 관련
#define ERR_CASH_mgQueryMall_ERROR						0x3000		// mgQueryMall에서 false를 리턴함
#define ERR_CASH_mgQueryMall_RESULT_CANNOT_PAYMENT		0x3001		// mgQueryMall에서 결과값 에러(출금실패)
#define ERR_CASH_mgQueryMall_RESULT_NO_ACCOUNT			0x3002		// mgQueryMall에서 결과값 에러(계좌 존재하지 않음)
#define ERR_CASH_NOT_CHARGE_USER						0x3003		// 2006-06-01 by cmkwon, 유료 사용자가 아니다
#define ERR_CASH_INVALID_EXT_ACCOUNTID_NUM				0x3004		// 2006-06-02 by cmkwon
#define ERR_CASH_INVALID_ITEM_INFORMATION				0x3005		// 2006-06-05 by cmkwon
#define ERR_CASH_NORMAL_ERROR							0x3006		// 2006-06-05 by cmkwon
#define ERR_CASH_CASH_SHOP_NO_SERVICE					0x3007		// 2007-01-10 by cmkwon, 유료 상점 서비스 안함
#define ERR_CASH_LIBRARY_INIT_ERROR						0x3008		// 2007-08-22 by cmkwon, 중국 Yetime 빌링 라이브러리 적용 - 에러 추가, 라이브러리 초기화 에러
#define ERR_CASH_INVALID_GIVE_TARGET_ACCOUNT			0x3009		// 2009-08-18 by cmkwon, 예당 캐쉬샾 블럭 계정 체크 추가 - 계정 블럭 상태


///////////////////////////////////////////////////////////////////////////////
// 2005-12-02 by cmkwon
#define ERR_INVALID_CHARACTER							0x4000		// Character가 유효하지 않다(접속종료, 죽은)
#define ERR_INVALID_PEER_CHARACTER						0x4001		// 나 아닌 다른 Character가 유효하지 않다(접속종료, 죽은)
#define ERR_WARPING										0x4002		// 워프중
#define ERR_IN_PARTY_BATTLE								0x4003		// 파티전중
#define ERR_REQ_WARP_COMPLETIONQUEST_NOT_MATCHED		0x4004		// 워프 요건 오류, 필요 퀘스트를 완료해야 한다
#define ERR_REQ_CHOOSE_INFLUENCE_TYPE					0x4005		// 세력 선택이 필요하다
#define ERR_CANNOT_WARP_CONNFLICT_AREA					0x4006		// 분쟁 지역으로 워프 불가,// 2006-02-14 by cmkwon
#define ERR_CANNOT_WARP_POSSESSED_CONNFLICT_AREA		0x4007		// 점유된 분쟁 지역으로 워프 불가,// 2006-02-14 by cmkwon
#define ERR_CANNOT_WARP_BY_TRADING						0x4008		// 거래중이므로 워프 불가, 2006-03-23 by cmkwon
#define ERR_CANNOT_WARP_BY_SHOPPING						0x4009		// 상점 이용중이므로 워프 불가, 2006-03-23 by cmkwon
#define ERR_CANNOT_ATTACHED_USING_PREMIUM_CARD			0x400A		// 2006-03-30 by cmkwon, 이 아이템은 프리미엄 카드 사용 중에는 장착 할 수 없다
#define ERR_CANNOT_BUY_PREMIUM_CARD_USING_SPEC_ITEM		0x400B		// 2006-03-30 by cmkwon, 특정 아이템 사용 중 에는 프리미엄 카드를 구입 할 수 없다
#define ERR_EXPIRED_ITEM								0x400C		// 2006-03-31 by cmkwon, 사용 시간이 경과된 아이템이다
#define ERR_FAILURE_USE_RESTORE_ITEM					0x400D		// 2006-04-10 by cmkwon, 부활 아이템(천사의 가호/날개/깃털) 사용 실패 - 확률로 실패
#define ERR_DOING_GAMEEVENT								0x400E		// 2006-04-21 by cmkwon, 이미 게임이벤트 진행중
#define ERR_INVALID_GAMEUSER							0x400F		// 
#define ERR_INVALID_CALL_WARP_EVENT_ID					0x4010		// 2006-07-21 by cmkwon 
#define ERR_TIMEOVER_CALL_WARP_EVENT					0x4011		// 2006-07-21 by cmkwon 
#define ERR_CANNOT_PUT_ITEM								0x4012		// 2006-07-26 by cmkwon
#define ERR_OVER_COUNT									0x4013		// 2006-07-26 by cmkwon
#define ERR_INVALID_BAZAAR								0x4014		// 2006-07-26 by cmkwon
#define ERR_USING_BAZAAR								0x4015		// 2006-07-26 by cmkwon
#define ERR_INVISIBLE_STATE								0x4016		// 2006-08-03 by cmkwon, 캐릭터가 투명 상태이다
#define ERR_DOING_SERVER_SHUTDOWN						0x4017		// 2006-08-04 by cmkwon, 서버 다운 진행상태
#define ERR_NO_SEARCH_CHARACTER							0x4018		// 2006-09-15 by dhjin, 캐릭터를 찾지 못했을때
#define ERR_NO_MEMBERSHIP_USER							0x4019		// 2006-09-15 by dhjin, 맴버쉽 유저가 아닐때
#define ERR_ALREADY_OPEN_OTHER_STORE					0x401A		// 2006-09-15 by cmkwon, 다른 창고가 열려 있다
#define ERR_ALREADY_LOADING_GUILD_STORE					0x401B		// 2006-09-15 by cmkwon, 여단 창고 아이템 로딩중
#define ERR_NO_SUCH_GUILD_STORE							0x401C		// 2006-09-25 by cmkwon, 여단 UID로 여단 창고가 없음
#define ERR_PERMISSION_DENIED							0x401D		// 2006-09-27 by cmkwon, 접근 권한이 없습니다.
#define ERR_INVALID_EXCUTE_PARAMETER_COUNT				0x401E		// 2006-10-02 by cmkwon, 실행 인자 개수 오류
#define ERR_INVALID_EXCUTE_PARAMETER					0x401F		// 2006-10-02 by cmkwon, 실행 인자 오류
#define ERR_NOT_RANK_DRAW_GUILD_STORE					0x4020		// 2006-10-09 by dhjin, 대대원 이상만 찾을 수 있다.
#define ERR_CANNOT_BOOSTER								0x4021		// 2006-10-13 by cmkwon, 부스터 사용 불가
#define ERR_NOT_INVISIBLE_STATE							0x4022		// 2007-04-02 by cmkwon, 캐릭터가 투명 상태가 아니다
#define ERR_JAMBOREE_NO_SUCH_CHARACTER					0x4023		// 2007-04-09 by cmkwon, 대회 서버에 캐릭터가 없다, 접속 불가
#define ERR_JAMBOREE_FAIL_CREATE_CHARACTER				0x4024		// 2007-04-09 by cmkwon, 대회 서버에 캐릭터 생성 불가
#define ERR_BOSS_STRATEGYPOINT_STATE_SUMMON				0x4025		// 2007-04-26 by dhjin, 전함이나 전략포인트가 소환되어 있는 상태				
#define ERR_CANNOT_WARP_TO_CITY							0x4026		// 2007-05-22 by cmkwon, 도시맵으로 워프 불가(다른 세력의 도시맵)
#define ERR_INVALID_WEAPON_INDEX						0x4027		// 2007-06-08 by cmkwon, 2형 탄두 인덱스 오류
#define ERR_INVALID_TUTORIAL							0x4028		// 2007-07-23 by dhjin, Tutorial 정보를 찾을 수 없다.
#define ERR_CANNOT_USE_SPEAKER_ITEM						0x4029		// 2007-08-24 by cmkwon, 스피커아이템 사용 가능/금지 설정 기능 추가 - 에러추가
#define ERR_CANNOT_WARP_TO_OUTPOST						0x402A		// 2007-08-28 by dhjin, 전진기지 맵으로 워프 불가
#define ERR_CANNOT_SEARCH_OUTPOST						0x402B		// 2007-08-28 by dhjin, 전진기지 찾지 못했을 때
#define ERR_ALREADY_SET_OUTPOST_NEXTWARTIME				0x402C		// 2007-08-28 by dhjin, 전진기지 시간 설정이 이미 되어있다.
#define ERR_CANNOT_SEARCH_OUTPOST_NEXTWARTIME			0x402D		// 2007-08-28 by dhjin, 전진기지 시간 설정을 찾지 못했을 때
#define ERR_CONFERENCEROOM_PERMISSION_DENIED			0x402E		// 2007-08-30 by cmkwon, 회의룸 시스템 구현 - 워프 불가 에러 추가
#define ERR_CANNOT_WARP_TO_OUTPOST_CITY_MAP				0x402F		// 2007-09-12 by dhjin, 전진기지 도시 맵으로 워프 불가


#define ERR_ARENA_NO_SUCH_TEAM							0x4030		// 2007-04-23 by dhjin, 아레나 조건에 맞는 팀을 찾을 수 없다
#define ERR_ARENA_CREATE_TEAM							0x4031		// 2007-04-23 by dhjin, 아레나 팀 생성 실패
#define ERR_ARENA_NOT_MATCH_MODE						0x4032		// 2007-04-23 by dhjin, 아레나 팀 모드가 틀리다
#define ERR_ARENA_NOT_MATCH_LEVEL						0x4033		// 2007-04-23 by dhjin, 아레나 팀 레벨이 틀리다
#define ERR_ARENA_NOT_MATCH_PW							0x4034		// 2007-04-23 by dhjin, 아레나 팀 패스워가 틀리다
#define ERR_ARENA_FULL_TEAMLIST							0x4035		// 2007-04-23 by dhjin, 아레나 팀이 꽉 차있다
#define ERR_ARENA_STATE									0x4036		// 2007-04-23 by dhjin, 아레나 팀 상태여서 오류 일때
#define ERR_ARENA_STATE_TEAM_READY						0x4037		// 2007-06-05 by dhjin, 아레나 팀 매칭중이어서 팀에 참가 할 수 없을때
#define ERR_ARENA_STATE_TEAM_WARING						0x4038		// 2007-04-23 by dhjin, 아레나 팀 진행중이어서 팀에 참가 할 수 없을때
#define ERR_ARENA_BLOCKED								0x4039		// 2007-07-11 by cmkwon, 아레나블럭 시스템 구현 - 

#define ERR_NO_SEARCH_CITYLEADER_INFO					0x4040		// 2007-08-22 by dhjin, CityLeader 정보를 찾지 못했을 때
#define ERR_REQ_NOT_LEADER_CANDIDATE					0x4041		// 2007-10-30 by dhjin, 지도자 후보 등록 조건에 적합하지 않다.
#define ERR_REQ_NOT_POLLDATE_APPLICATION				0x4042		// 2007-10-30 by dhjin, 지도자 후보 등록 기간이 아니다.
#define ERR_FAIL_REG_LEADER_CANDIDATE_BY_DB				0x4043		// 2007-10-30 by dhjin, 지도자 후보 등록에 실패 DB
#define ERR_FAIL_REG_LEADER_CANDIDATE_BY_REQ			0x4044		// 2007-10-30 by dhjin, 지도자 후보 등록에 요구 조건을 만족하지 못해 실패
#define ERR_REQ_NOT_POLLDATE_VOTE						0x4045		// 2007-10-30 by dhjin, 투표 기간이 아니다.
#define ERR_REQ_NOT_VOTE								0x4046		// 2007-10-31 by dhjin, 투표 조건에 적합하지 않다.
#define ERR_ALREADY_VOTE								0x4047		// 2007-10-31 by dhjin, 이미 투표한 유저이다.
#define ERR_INVALID_LEADER_CANDIDATE					0x4048		// 2007-10-31 by dhjin, 유효한 지도자 후보가 아니다.
#define ERR_ALREADY_LEADER_CANDIDATE					0x4049		// 2007-11-01 by dhjin, 이미 지도자 후보이다.
#define ERR_ALREADY_DELETE_LEADER_CANDIDATE				0x404A		// 2007-11-01 by dhjin, 이미 탈퇴한 지도자 후보이거나 탈퇴가 불가능한 유저이다.
#define ERR_REQ_NOT_POLLDATE_ALL_RANGE					0x404B		// 2007-11-20 by dhjin, 선거 기간이 아니다.
#define ERR_REQ_REG_LEADER_CANDIDATE					0x404C		// 2007-11-20 by dhjin, 지도자 후보에 등록이 되어있다.

#define ERR_WARING_OUTPOST_TO_OWNMAP					0x404D		// 2007-11-09 by dhjin, 전진기지가 진행 중이다.

#define ERR_DELETE_CHARACTER_GUILDCOMMANDER				0x404E		// 2008-10-20 by dhjin, 여단장은 케릭터 삭제 불가로 변경

///////////////////////////////////////////////////////////////////////////////
// 2007-09-12 by cmkwon, 베트남 2차패스워드 구현 - 2차패스워드 시스템 에러 추가
#define ERR_SECPASS_NOT_USE_SYSTEM						0x4050		// 2007-09-12 by cmkwon, 2차패스워드 시스템을 사용하지 않는다
#define ERR_SECPASS_PASSWORD_NOT_SETTING				0x4051		// 2007-09-12 by cmkwon, 2차패스워드가 설정되어 있지 않음
#define ERR_SECPASS_ALREADY_LOCK						0x4052		// 2007-09-12 by cmkwon, 이미 Lock 상태임
#define ERR_SECPASS_ALREADY_UNLOCK						0x4053		// 2007-09-12 by cmkwon, 이미 Unlock 상태임
#define ERR_SECPASS_PASSWORD_NOT_MATCHED				0x4054		// 2007-09-12 by cmkwon, 이미 Unlock 상태임
#define ERR_SECPASS_NEW_PASSWORD_NOT_SETTING			0x4055		// 2007-09-12 by cmkwon, 신규 패스워드가 설정되어 있지 않음
#define ERR_SECPASS_LOCKED								0x4056		// 2007-09-12 by cmkwon, 2차패스워드가 Lock 상태임

//////////////////////////////////////////////////////////////////////////
// 2008-11-10 by dhjin. 럭키머신
#define ERR_LUCKYITEM_INSERT_BUT_COINITEM_DELETE		0x4060		// 2008-11-10 by dhjin, 럭키머신 아이템이 하나라도 지급되었지만 시스템에 오류가 있어 정상지급이 되지는 않았을 경우

#define ERR_CANNOT_WARP_DURING_MSWAR					0x4061		// 2012-07-02 by hskim, 모선전 개선 - 모선전 진행시 스톤즈 루인 진입 불가

///////////////////////////////////////////////////////////////////////////////
// 아이템 관련
#define ERR_INVALID_ARMOR_ITEM							0x4100		// 아머 아이템이 유효하지 않다(없거나 ItemKind 오류)
#define ERR_ITEM_TRADING								0x4101		// 2005-12-26 by cmkwon, 거래중입니다. (상점이용 불가, 창고 이용 불가, ...)
#define ERR_USING_SHOP									0x4102		// 2006-05-11 by cmkwon, 상점 이용중(거래 불가)
#define ERR_NO_SUCH_WEAPON_ITEM							0x4103		// 2007-08-07 by cmkwon, 1형/2형 무기 총알 충전 아이템 구현 - 에러코드 추가
#define ERR_ALREADY_FULL_BULLET							0x4104		// 2007-08-07 by cmkwon, 1형/2형 무기 총알 충전 아이템 구현 - 에러코드 추가
#define ERR_INVALID_ITEMEVENT_UID						0x4105		// 2008-01-10 by cmkwon, 아이템 이벤트 시스템에 신 쿠폰 시스템 추가 - 
#define ERR_NOT_ACCOUNT_BLOCKED							0x4106		// 2008-01-31 by cmkwon, 계정 블럭/해제 명령어로 가능한 시스템 구현 - 블럭 상태가 아니다
#define ERR_SOLD_OUT_SHOP_ITEM							0x4107		// 2010-01-26 by cmkwon, 캐쉬 아이템 한정판매 시스템 구현 - 
#define ERR_NO_SUCH_ENCHANT_INFO						0x4108		// 2010-04-20 by cmkwon, 신규 러키 머신 구현 - 
#define ERR_NOT_DISSOLVED_ITEM							0x4109		// 2010-08-31 by shcho&jskim 아이템용해 시스템 - 용해 시스템 패킷 처리
#define ERR_CANNOT_DO_GIFT_UNDER_LEVEL					0x4119		// 2013-07-08 by bckim, 캐나다요청 레벨70 미만은 선물하기 불가
#define ERR_NOT_INSERT_DISSOLVED_ITEMS					0x4128		// 2010-08-31 by shcho&jskim 아이템용해 시스템 - 용해 시스템 패킷 처리

// 2007-10-12 by dhjin, 부지도자 설정 관련 오류
#define ERR_PROTOCOL_SUBLEADER_SET_FAIL					0x4110		// 2007-10-06 by dhjin, 부지도자 설정 실패 
#define ERR_PROTOCOL_SUBLEADER_SET_ALEADY				0x4111		// 2007-10-06 by dhjin, 부지도자가 이미 설정 되어있음

#define ERR_CANNOT_USE_GUILD_STORE						0x4120		// 2009-09-23 by cmkwon, 필드창고 캐쉬 아이템 구현 - 여단창고 사용 불가
#define ERR_CANNOT_USE_STORE							0x4121		// 2009-09-23 by cmkwon, 필드창고 캐쉬 아이템 구현 - 창고 사용 불가
#define ERR_CANNOT_USE_FIELD_STORE						0x4122		// 2009-09-23 by cmkwon, 필드창고 캐쉬 아이템 구현 - 필드창고 사용 불가
#define ERR_ALREADY_USING_BUILDING						0x4123		// 2009-09-23 by cmkwon, 필드창고 캐쉬 아이템 구현 - 상점 이용 중
#define ERR_NO_SUCH_BUILDINGINDEX						0x4124		// 2009-09-23 by cmkwon, 필드창고 캐쉬 아이템 구현 - 
#define ERR_CANNOT_BUY_ITEM								0x4125		// 2009-09-23 by cmkwon, 필드창고 캐쉬 아이템 구현 - 
#define ERR_ALREADY_USE_ITEMPARAM						0x4126		// 2010-01-18 by cmkwon, 아이템 사용시 Parameter 중복 체크 시스템 구현 - 
#define ERR_INSUFFICIENT_NUMBER_SHOP_ITEM				0x4129		// 2011-10-28 by hskim, EP4 [트리거 시스템] - 크리스탈 시스템 - 구매할수 있는 수량이 부족함
#define ERR_NOT_OWNER_SHOP								0x4130		// 2011-10-28 by hskim, EP4 [트리거 시스템] - 크리스탈 시스템 - 소유 상점이 아님

///////////////////////////////////////////////////////////////////////////////
// 맵 관련
#define ERR_MAP_CHECKSUM_NOT_MATCHED					0x4200		// 2007-04-06 by cmkwon, 
#define ERR_REQ_WARP_ONE_TIME_LIMITED					0x4201		// 2011-10-28 by hskim, EP4 [트리거 시스템] - 크리스탈 시스템 - 1번만 입장 할 수있음
#define ERR_REQ_WARP_GUILD_NOT_MATCHED					0x4202		// 2011-10-28 by hskim, EP4 [트리거 시스템] - 크리스탈 시스템 - 현재 입장할 수 있는 길드가 아님
#define ERR_REQ_WARP_MAXIMUM_USER_LIMITED				0x4203		// 2011-10-28 by hskim, EP4 [트리거 시스템] - 크리스탈 시스템 - 최대 접속할 수 있는 인원을 넘었음
#define ERR_REQ_WARP_NO_QUEST_ITEM						0x4204		// 2011-10-28 by hskim, EP4 [트리거 시스템] - 크리스탈 시스템 - 입장에 필요한 퀘스트 아이템이 없음
#define ERR_REQ_WARP_GAME_START_NOT_ALLOWED_USER		0x4205		// 2011-10-28 by hskim, EP4 [트리거 시스템] - 크리스탈 시스템 - 게임 시작시에 맵에 입장이 허용된 유저가 아님
#define ERR_REQ_WARP_INFLUENCE_NOT_MATCHED				0x4206		// 2011-10-28 by hskim, EP4 [트리거 시스템] - 크리스탈 시스템 - 현재 입장할 수 있는 세력이 아님

///////////////////////////////////////////////////////////////////////////////
// 캐릭터 관련
#define ERR_REQ_CHOOSE_START_CIIY_MAP_INDEX				0x4250		// 2009-10-12 by cmkwon, 프리스카 제거 방안 적용 - 일반세력의 시작 도시맵 설정이 필요하다


//////////////////////////////////////////////////////////////////////////
// 2007-12-28 by dhjin, 아레나 통합 - 
#define ERR_INVALID_ARENASERVER							0x4300		// 2007-12-28 by dhjin, 아레나 서버가 아니다.
#define ERR_NO_SUCH_FIELDSERVER							0x4301		// 2007-12-28 by dhjin, 필드 서버를 찾을 수 없다.
#define ERR_INVALID_MAINSERVER_CHARACTER				0x4302		// 2007-12-28 by dhjin, 유효하지 않은 서버 유저이다.
#define ERR_INVALID_ARENASERVER_CHARACTER				0x4303		// 2007-12-28 by dhjin, 유효하지 않은 아레나 유저이다.
#define ERR_CANNOT_USE_ARENASERVER						0x4304		// 2008-02-26 by dhjin, 아레나 서버에서는 사용 불가이다.
#define ERR_NOT_CONNECT_TO_ARENASERVER					0x4305		// 2008-03-07 by dhjin, 아레나 서버와 연결이 되어 있지 않다.	

//////////////////////////////////////////////////////////////////////////
// 2008-04-30 by dhjin, EP3 편지 시스템 -
#define ERR_NO_SUCH_LETTER								0x4310		// 2008-04-30 by dhjin, EP3 편지 시스템 - 편지를 찾지 못하였다.

//////////////////////////////////////////////////////////////////////////
// 2008-04-30 by dhjin, EP3 - 여단 수정 사항
#define ERR_ALREADY_REG									0x4320		// 2008-04-30 by dhjin, EP3 - 여단 수정 사항 - 자기 소개가 등록이 되어있다.
#define ERR_CANNOT_USE_NEW_COMMANDER_BY_CITYLEADER      0x4321		// 2008-06-19 by dhjin, EP3 - 여단 수정 사항 - 지도자나 전진기지 여단장은 여단장 위임 불가 
#define ERR_CANNOT_USE_NEW_COMMANDER_BY_POLL		    0x4322		// 2008-06-19 by dhjin, EP3 - 여단 수정 사항 - 선거에 출마한 사람은 여단장 위임 불가
#define ERR_POSSESS_OUTPOST							    0x4323		// 2009-06-30 by cmkwon, 여단 해체시 체크 수정 - 전진 기지를 소유하고 있다.

//////////////////////////////////////////////////////////////////////////
// 2008-06-17 by dhjin, EP3 채팅방 -
#define ERR_INVALID_DATA_FROM_CLIENT					0x4330		// 2008-06-17 by dhjin, EP3 채팅방 - 클라이언트로부터 받은 잘못된 데이타
#define ERR_CHATROOM_NO_SUCH_CREATE_NUM					0x4331		// 2008-06-17 by dhjin, EP3 채팅방 - 생성 방 번호가 없다
#define ERR_CHATROOM_NO_SUCH							0x4332		// 2008-06-17 by dhjin, EP3 채팅방 - 채팅방이 없다.
#define ERR_CHATROOM_ALREADY_USING						0x4333		// 2008-06-17 by dhjin, EP3 채팅방 - 이미 채팅방 이용중이다.
#define ERR_CHATROOM_PERMISSION_DENIED					0x4334		// 2008-06-17 by dhjin, EP3 채팅방 - 권한이 없다.
#define ERR_CHATROOM_FULL_MEMBER						0x4335		// 2008-06-17 by dhjin, EP3 채팅방 - 채팅방 인원이 만땅이다.
#define ERR_CHATROOM_NO_SUCH_NEW_MASTER					0x4336		// 2008-06-17 by dhjin, EP3 채팅방 - 새로운 방장을 찾지 못했다.
#define ERR_NOT_MATCH_PW								0x4337		// 2008-07-15 by dhjin, EP3 채팅방 - 암호 불일치.

//////////////////////////////////////////////////////////////////////////
// 2009-01-12 by dhjin, 선전 포고
#define ERR_NO_SUCH_DECLARATION_MSWAR_INFO				0x4340		// 2009-01-12 by dhjin, 선전 포고 - 선전 포고 정보 못찾음
#define ERR_NO_MORE_DECLARATION_MSWAR_SET				0x4341		// 2009-01-12 by dhjin, 선전 포고 - 선전 포고 시간을 더 이상 설정 할 수 없다.
#define ERR_INVALID_DECLARATION_MSWAR_TIME				0x4342		// 2009-01-12 by dhjin, 선전 포고 - 선전 포고 시간 설정이 잘못됨.

#define ERR_CANNOT_CHOOSE_INFLUENCE_TYPE				0x4350		// 2009-03-31 by cmkwon, 세력초기화 시스템 구현 - 해당 세력을 선택 할 수 업다


///////////////////////////////////////////////////////////////////////////////
// 2007-01-05 by cmkwon, 자동 업데이트 관련
#define ERR_CANNOT_CONNECT_INTERNET						0x5000		// 2007-01-05 by cmkwon, 인터넷 연결 안됨
#define ERR_CANNOT_CONNECT_AUTO_UPDATE_SERVER			0x5001		// 2007-01-05 by cmkwon, 자동업데이트 서버에 연결 할 수 없다
#define ERR_LOCAL_FILE_CREATE_FAIL						0x5002		// 2007-01-05 by cmkwon, 로컬파일 생성 오류
#define ERR_AUTO_UPDATE_SERVER_PERMISSION_DENIED		0x5003		// 2007-01-05 by cmkwon, 자동업데이트 서버 접근 권한 오류
#define ERR_UPDATE_FILE_NOT_FOUND						0x5004		// 2007-01-05 by cmkwon, 업데이트 파일을 찾을 수 없다
#define ERR_UPDATE_FILE_DOWNLOADING_FAIL				0x5005		// 2007-01-05 by cmkwon, 업데이트 파일 다운로딩 실패
#define ERR_UPDATE_FILE_DOWNLOADING_CANCEL				0x5006		// 2007-01-05 by cmkwon, 업데이트 파일 다운로딩 취소
#define ERR_FUNC_HttpOpenRequest						0x5007		// 2007-01-05 by cmkwon, HttpOpenRequest 함수에서 에러를 리턴함
#define ERR_FUNC_HttpSendRequest						0x5008		// 2007-01-05 by cmkwon, HttpSendRequest 함수에서 에러를 리턴함
#define ERR_FUNC_HttpQueryInfo							0x5009		// 2007-01-05 by cmkwon, HttpQueryInfo 함수에서 에러를 리턴함
#define ERR_VERSIONINFO_FILE_NOT_FOUND					0x500A		// 2007-05-09 by cmkwon, 
#define ERR_VERSIONINFO_FILE_CAN_NOT_CREATE				0x500B		// 2007-05-09 by cmkwon, 
#define ERR_NOT_ALLOWED_IP								0x500C		// 2007-10-19 by cmkwon, AllowedIP 시스템 변경 - 접속 허가된 IP가 아니다

////////////////////////////////////////////////////////////////////////////////
// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 
#define ERR_DEBUFF_SKILL_APPLYING_SLIENCE				0x6000		// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 사일런스 디버프가 적용중이다.
#define ERR_RELEASE_SKILL_NOT_FIND						0x6001		// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 해제할 스킬을 찾을수가 없다.
#define ERR_DEBUFF_SKILL_APPLYING_NOT_HP_RECOVERY		0x6002		// 2009-09-09 ~ 2010 by dhjin, 인피니티 - HP회복 금지
#define ERR_DEBUFF_SKILL_APPLYING_NOT_DP_RECOVERY		0x6003		// 2009-09-09 ~ 2010 by dhjin, 인피니티 - DP회복 금지
#define ERR_DEBUFF_SKILL_APPLYING_NOT_SP_RECOVERY		0x6004		// 2009-09-09 ~ 2010 by dhjin, 인피니티 - SP회복 금지
#define ERR_INFINITY_NO_SUCH_MODE_LIST					0x6005		// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피니티 리스트를 찾지 못했을 경우
#define ERR_INFINITY_NO_SUCH_READY_LIST					0x6006		// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피니티 대기 리스트를 찾지 못했을 경우
#define ERR_INFINITY_OVER_ENTRANCECOUNT					0x6007		// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피니티 입장 가능한 수를 넘겼다.
#define ERR_INFINITY_CREATE_FAIL						0x6008		// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피니티 생성 실패
#define ERR_INFINITY_JOIN_FAIL_SOCKET					0x6009		// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피니티 가입 실패 - 소켓 NULL값
#define ERR_INFINITY_MAX_ADMISSIONCNT					0x600A		// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피니티 가입 실패 - 최대 인원
#define ERR_INFINITY_MISMATCH_LEVEL						0x600B		// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피니티 - 레벨 오류
#define ERR_INFINITY_JOIN_FAIL_ENTRANCECOUNT			0x600C		// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피니티 가입 실패 - 입장 수 제한
#define ERR_INFINITY_MODE								0x600D		// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 잘못된 모드
#define ERR_INFINITY_CREATEUID							0x600E		// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 잘못된 생성 번호
#define ERR_INFINITY_NO_SUCH_MASTER_USER				0x600F		// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피니티 가입 실패 - 인피 마스터
#define ERR_INFINITY_JOIN_FAIL_MASTERUSER_REJECT		0x6010		// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피니티 가입 실패 - 생성 번호
#define ERR_INFINITY_NO_SUCH_MEMBER_LIST				0x6011		// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피니티 유저 리스트를 찾지 못했을 경우
#define ERR_INFINITY_STATE								0x6012		// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피니티 잘못된 상태값
#define ERR_INFINITY_SAME_MASTER						0x6013		// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 동일 인물!
#define ERR_INFINITY_MASTER								0x6014		// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 마스터가 아니다.
#define ERR_INFINITY_CREATE_MAPCHANNEL					0x6015		// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 맵 생성 중 채널 생성 오류
#define ERR_INFINITY_NOT_ALL_READY						0x6016		// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 모든 유저가 래디 상태가 아니다.
#define ERR_INFINITY_CANNOT_USE_ITEM					0x6017		// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피니티에서 사용 불가
#define ERR_INFINITY_MIN_ADMISSIONCNT					0x6018		// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 이용 최소 인원 오류
#define ERR_INFINITY_TENDER_PROCESSING					0x6019		// 2009-09-09 ~ 2010 by dhjin, 인피니티 - Tender중이다.
#define ERR_INFINITY_NOT_PROCESSING_TENDERITEM			0x601A		// 2009-09-09 ~ 2010 by dhjin, 인피니티 - Tender진행중인 아이템이 아니다.
#define ERR_INFINITY_ONLY_USE_ITEM						0x601B		// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피니티에서만 사용 가능
#define ERR_INFINITY_CANNOT_SUCH_TEAM					0x601C		// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 팅긴 유저 재접속 처리, 존재하는 팀이 없다.
#define ERR_INFINITY_NULL_VALUE							0x601D		// 2009-09-09 ~ 2010-01 by dhjin, 인피니티 - 소스 체크, NULL값 관련
#define ERR_INFINITY_CANNOT_ALL_START					0x601E		// 2010-04-06 by cmkwon, 인피2차 추가 수정 - 
#define ERR_INFINITY_TENDER_ALREADY_CHOICED				0x601F		// 2010-04-09 by cmkwon, 인피2차 추가 수정(단계별 보상 추가) - 이미 선택한 유저
#define ERR_INFINITY_DIFFICLUTY_NOT_FIND				0x6020		// // 2010-05-04 by shcho, 인피니티 난이도 조절 - 난이도를 찾지 못했을 때
#define ERR_INFINITY_DIFFICULTY_LEVEL_SAME				0x6021		// 2010. 05. 19 by hsLee 인피니티 필드 2차 난이도 조절. (신호처리 + 몬스터 처리(서버) ) - 같은 난이도로 변경 요청 오류.
#define ERR_INFINITY_DIFFICULTY_LEVEL_INVALID			0x6022		// 2010. 05. 19 by hsLee 인피니티 필드 2차 난이도 조절. (신호처리 + 몬스터 처리(서버) ) - 유효하지 않은 난이도 변경값 요청 오류.

#define ERR_INFINITY_MISMATCH_CREATEUID					0x6023		// 인피니티 - 방 생성번호 불일치.	// 2010. 07. 27 by hsLee 인피니티 2차 거점 방어 시네마 연출 스킵 처리.
#define ERR_INFINITY_PLAYING_STATE						0x6024		// 인피니티 - 게임 진행 중이 아닌 경우. ( STATE - PLAYING ~ 엔딩 ) // 2010. 07. 27 by hsLee 인피니티 2차 거점 방어 시네마 연출 스킵 처리.
#define ERR_INFINITY_ALWAYS_SKIP_ENDINGCINEMA			0x6025		// 인피니티 - 이미 연출 스킵 활성화 되어 있음. // 2010. 07. 27 by hsLee 인피니티 2차 거점 방어 시네마 연출 스킵 처리.

#define ERR_DEBUFF_SKILL_APPLYING_STOPMAGIC				0x6026		// 2011-10-28 by hskim, EP4 [트리거 시스템] - 스킬 추가 : 침묵 당해 스킬을 사용할수 없습니다.

#define ERR_KNOWN_ERROR									0x6100		// 2010-04-26 by cmkwon, 러시아 Innva 인증/빌링 시스템 변경 - 
#define ERR_INVALID_HANDLE								0x6101		// 2010-04-26 by cmkwon, 러시아 Innva 인증/빌링 시스템 변경 - 

///////////////////////////////////////////////////////////////////////////////
// 2011-01-26 by hskim, 인증 서버 구현

#define ERR_PROTOCOL_INVALID_AUTHENTICATION_SERVER_LOGIN_DATA		0x7001		// 2011-01-26 by hskim, 인증 서버 구현 - PreServer 로 부터 받은 Data Size가 Authentication Packet Type에 따른 Data Size보다 작음
#define ERR_PROTOCOL_INVALID_AUTHENTICATION_SERVER_LOGIN_DB			0x7002		// 2011-01-26 by hskim, 인증 서버 구현 - PreServer 로 부터 받은데이타를 사용해 인증절차를 거쳤지만 중간에 정상적으로 처리되지 않았다

///////////////////////////////////////////////////////////////////////////////
// 2011-08-22 by hskim, 파트너 시스템 2차

#define ERR_PET_NUMERIC_VALUE_OUT_OF_RANGE				0x7101		// 2011-09-15 by hskim, 파트너 시스템 2차 - 값이 범위를 벗어남
#define ERR_PET_NOT_EQUIPPED							0x7102		// 2011-09-15 by hskim, 파트너 시스템 2차 - 펫이 장착되어 있지 않다
#define ERR_PET_EMPTY_NAME								0x7103		// 2011-09-15 by hskim, 파트너 시스템 2차 - 이름이 비여 있음
#define ERR_PET_NOT_ALLOW_LEVEL_UP						0x7104		// 2011-09-15 by hskim, 파트너 시스템 2차 - 레벨업 허용 안함
#define ERR_PET_NOT_ALLOW_CHANGE_NAME					0x7105		// 2011-09-15 by hskim, 파트너 시스템 2차 - 이름 변경 허용 안함
#define ERR_PET_NOT_ALLOW_SOCKET_INDEX					0x7106		// 2011-09-15 by hskim, 파트너 시스템 2차 - 허용안되는 소켓 인덱스
#define ERR_PET_ALREADY_USE_SOCKET						0x7107		// 2011-09-15 by hskim, 파트너 시스템 2차 - 이미 사용된 소켓
#define ERR_PET_INVALID_SOCKET							0x7108		// 2011-09-15 by hskim, 파트너 시스템 2차 - 잘못된 소켓
#define ERR_PET_NO_SUCH_INFO							0x7109		// 2011-09-15 by hskim, 파트너 시스템 2차 - 펫관련 정보를 찾을수 없음
#define ERR_PET_INVALID_AUTOSKILL						0x710a		// 2011-09-15 by hskim, 파트너 시스템 2차 - 잘못된 자동 스킬
#define ERR_PET_INVALID_AUTOKIT							0x710b		// 2011-09-15 by hskim, 파트너 시스템 2차 - 잘못된 오트 스킬
#define ERR_PET_NOT_MATCH_SOCKET						0x710c		// 2011-09-15 by hskim, 파트너 시스템 2차 - 소켓아이템이 서버정보와 일치하지 않는다

#define ERR_IS_LEADER									0x8000		// 2015-12-17 Future, Leaders cant use a nation change card
#define	ERR_NOT_ENOUGH_FAME								0x8001		// 2015-12-17 Future, Too low fame for usage
#define ERR_INVALID_INFLUENCE							0x8002		// 2015-12-17 Future, the user belongs to an invalid influence
#define ERR_UNFAIR_BALANCE								0x8003		// 2015-12-17 Future, error due to nation inbalance
#define	ERR_IS_GUILD_MEMBER								0x8004		// 2015-12-17 Future, Guild Members cant use a nation change card
#define	ERR_MAX_TRANSFER								0x8005		// 2015-12-17 Future, Guild Members cant use a nation change card
#define ERR_WARP_RESTRICTED								0x8005		// 2016-01-04 Future, User is having a warp restriction

///////////////////////////////////////////////////////////////////////////////
// 보안 관련
#define ERR_SECURITY_NOT_CHECK_VERSION					0xE000		// 
#define ERR_SECURITY_HACKING_GUID						0xE001		// 
#define ERR_SECURITY_HACKING_CRC						0xE002		// 
#define ERR_SECURITY_HACKING_CLIENT						0xE003		// 
#define ERR_SECURITY_ANTICPX_INVALID_HANDLE				0xE004		// 2008-04-03 by cmkwon, 핵쉴드 서버 연동 시스템 수정 - 
#define ERR_SECURITY_CREATE_CLIENT_FAIL					0xE005		// 2008-04-03 by cmkwon, 핵쉴드 서버 연동 시스템 수정 - 

#define ERR_AHNHS_ENGINE_DETECT_GAME_HACK				0xE100
#define ERR_AHNHS_ACTAPC_DETECT_AUTOMOUSE				0xE101
#define ERR_AHNHS_ACTAPC_DETECT_ALREADYHOOKED			0xE102
#define ERR_AHNHS_ACTAPC_DETECT_SPEEDHACK				0xE103
#define ERR_AHNHS_ACTAPC_DETECT_SPEEDHACK_APP			0xE104
#define ERR_AHNHS_ACTAPC_DETECT_KDTRACE					0xE105
#define ERR_AHNHS_ACTAPC_DETECT_KDTRACE_CHANGED			0xE106
#define ERR_AHNHS_ACTAPC_DETECT_DRIVERFAILED			0xE107
#define ERR_AHNHS_ACTAPC_DETECT_HOOKFUNCTION			0xE108
#define ERR_AHNHS_ACTAPC_DETECT_MESSAGEHOOK				0xE109
#define ERR_AHNHS_ACTAPC_DETECT_MODULE_CHANGE			0xE10A
#define ERR_AHNHS_ACTAPC_DETECT_ABNORMAL_MEMORY_ACCESS	0xE10B		// 2007-04-09 by cmkwon
#define ERR_AHNHS_ACTAPC_DETECT_ENGINEFAILED			0xE10C		// 2008-03-24 by cmkwon, 핵쉴드 2.0 적용 - AHNHS_ACTAPC_DETECT_ENGINEFAILED 추가됨
#define ERR_AHNHS_ACTAPC_DETECT_AUTOMACRO					0xE10D		// 2009-07-17 by cmkwon, 핵쉴드 자체 종료 처리 에러타입추가(클라이언트만 사용) - 
#define ERR_AHNHS_ACTAPC_DETECT_CODEMISMATCH				0xE10E		// 2009-07-17 by cmkwon, 핵쉴드 자체 종료 처리 에러타입추가(클라이언트만 사용) - 
#define ERR_AHNHS_ACTAPC_DETECT_ABNORMAL_HACKSHIELD_STATUS	0xE10F		// 2009-07-17 by cmkwon, 핵쉴드 자체 종료 처리 에러타입추가(클라이언트만 사용) - 
#define ERR_AHNHS_ACTAPC_DETECT_LMP_FAILED					0xE110		// 2009-07-17 by cmkwon, 핵쉴드 자체 종료 처리 에러타입추가(클라이언트만 사용) - 





///////////////////////////////////////////////////////////////////////////////
// 유료화 아이템 관련
#define ERR_CASH_PREMIUM_CARD_INVALID_ITEMNUM			0xF000		// 프리미엄(일반, 슈퍼, 업그레이드) 카드 ItemNum이 아니다
#define ERR_CASH_PREMIUM_CARD_ALREADY_USING				0xF001		// 프리미엄(일반, 슈퍼, 업그레이드) 카드가 이미 다른 처리중임
#define ERR_CASH_PREMIUM_CARD_NOT_MATCHED				0xF002		// 프리미엄(일반, 슈퍼, 업그레이드) 카드가 다른 종류임
#define ERR_CASH_PREMIUM_CARD_NOT_YET_BUY_STATE			0xF003		// 프리미엄(일반, 슈퍼, 업그레이드) 카드가 구입할수 있는 상태가 아니다

// start 2011-11-03 by shcho, yedang 셧다운제 구현
///////////////////////////////////////////////////////////////////////////////
// 예당 셧다운제 
#define ERR_SHUTDOWNMINORS_USER							0xF004		// 16세 미만의 유저임으로 런처에서 1차로 막는다.
// end 2011-11-03 by shcho, yedang 셧다운제 구현

// 2012-08-17 by jhseol, 마상 BPSoft 빌링시스템
#define ERR_CASH_BPSOFT_COM_LIBRARY_INIT				0xF011		// COM 라이브러리 초기화 실패
#define ERR_CASH_BPSOFT_CREATION_OBJECK					0xF012		// COM 객체 생성 실패
#define ERR_CASH_BPSOFT_METHOD_CALL						0xF013		// 함수 호출 실패
#define ERR_CASH_BPSOFT_EXCEPTION						0xF014		// COM 라이브러리 예외사항 에러
// end 2012-08-17 by jhseol, 마상 BPSoft 빌링시스템

///////////////////////////////////////////////////////////////////////////////
// String Conversion Functions
const char* GetErrorString(Err_t err);

// 2013-03-13 by hskim, 웹 캐시 상점
/////////////////////////////////////////////////////////////////////////////////
// XMLRPC ERROR

// 아래 정의 수정시는 atum_insertStoreItem_FromXMLRPC 프로시져도 같이 수정 해야함

#define XMLRPC_ERROR_INTERNAL_SERVER					-1			// 서버 내부 에러
#define XMLRPC_ERROR_INVALID_SERVER_ID					-2			// 서버 ID 유효하지 않음
#define XMLRPC_ERROR_DISABLE_SERVER						-3			// 서버 처리 불가 상태
#define XMLRPC_ERROR_DISABLE_REQUEST					-4			// 서버 해당 요청 처리 불가 상태
#define XMLRPC_ERROR_INVALID_USER_IP					-5			// 유저 IP 주소가 유효하지 않음
#define XMLRPC_ERROR_NOT_MATCHED_USER_INFO				-6			// 계정과 캐릭터 정보가 같지 않음
#define XMLRPC_ERROR_INVALID_ACCOUNTUID					-7			// 계정 정보가 유효하지 않음
#define XMLRPC_ERROR_INVALID_CHARACTERUID				-8			// 캐릭터 정보가 유효하지 않음
#define XMLRPC_ERROR_INVALID_ITEMNUM					-9			// 아이템 번호가 유효하지 않음
#define XMLRPC_ERROR_OUT_OF_RANGE_ITEMCOUNT				-10			// 아이템 수의 범위가 유효하지 않음
#define XMLRPC_ERROR_INVALID_AUTHENTICATION_KEY			-11			// 구매 상점 인증키가 유효하지 않음
#define XMLRPC_ERROR_NOT_ACCEPTED_USER					-12			// 해당 유저의 요청 처리 불가
#define XMLRPC_ERROR_INVALID_ITEM_STORAGE				-13			// 아이템 위치가 유효하지 않음
#define XMLRPC_ERROR_NEED_MORE_CASH						-16			// 구매에 필요한 캐시 부족
#define XMLRPC_ERROR_BILLING_ERROR						-17			// 빌링 연동 에러
#define XMLRPC_ERROR_INVALID_GET_CHARACTERUID			-18			// 선물 받을 계정 정보가 유효하지 않음
#define XMLRPC_ERROR_NOT_FOUND_BILLING_ITEM_LIST		-19			// 빌링 목록에 없는 아이템 (구매 불가)
#define XMLRPC_ERROR_NOT_ACCEPTED_ITEMNUM				-20			// 허용하지 않는 아이템 (추가 / 삭제)
#define XMLRPC_ERROR_SUPPORT_ONLY_COUNTABLE_ITEM		-21			// 카운터블 아이템만 지원
#define XMLRPC_ERROR_NOT_ENOUGH_ITEM_COUNT				-22			// 아이템 수 부족
#define XMLRPC_ERROR_CASH_SHOP_NO_SERVICE				-23			// 캐시샵을 서비스 하지 않음
#define XMLRPC_ERROR_INVALID_PEER_CHARACTER				-24			// 대상 캐릭터 정보가 유효하지 않음
#define XMLRPC_ERROR_DISABLE_WEBCASH_SHOP				-25			// 웹 캐시샵 사용 안함 (프로토콜)
#define XMLRPC_ERROR_UNKNOWN_ERROR						-10000		// 알수 없는 에러
#define XMLRPC_ERROR_NOT_FOUND_RPC_METHOD				-10001		// RPC 메소드를 찾을 수 없음
#define XMLRPC_ERROR_NOT_MATCHED_PARAMETER_TYPE			-10002		// RPC 메소드의 파라미터 형식이 틀림
#define XMLRPC_ERROR_INVALID_XML						-10003		// XML 형식이 틀림

char* GetErrorXMLRPCString(int err);

// end 2013-03-13 by hskim, 웹 캐시 상점

#endif // _ATUM_ERROR_H_