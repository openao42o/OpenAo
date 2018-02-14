// Copyright[2002] MasangSoft
#ifndef COMMON_ATUMDEFINE_H_
#define COMMON_ATUMDEFINE_H_


#include "ObjectDefine.h"
#include "Contents.h"


#define OPTION_DEFAULT_SOUNDVOLUME			-2000	// 소리 
#define OPTION_DEFAULT_STAGE_EFFECT_SOUNDVOLUME	-1000	// 소리 
#define OPTION_DEFAULT_MUSICVOLUME			8//-2000	// 음악
#define OPTION_DEFAULT_GAMMA				0		// 감마
#define OPTION_DEFAULT_LOWQUALITY			0		// 최소사향
#define OPTION_DEFAULT_HELPDESK				1		// 도움말
#define OPTION_DEFAULT_REVERSE_LEFT			0		// 좌우 반전
#define OPTION_DEFAULT_REVERSE_UP			0		// 상하 반전
#define OPTION_DEFAULT_AUTO_BALACE			1		// 자동 수평 조정
#define OPTION_DEFAULT_PET_OP				1		// 메뉴 사용시 자동 수평유지
#define OPTION_DEFAULT_MENU_USE				1		// 2010. 10. 26 by jskim 펫 오퍼레이터 옵션 처리
#ifdef _SHOW_GUILD_NAME
#define OPTION_DEFAULT_SHOW_GUILDNAME		1
#endif

#define OPTION_DEFAULT_MP3_USE				1		// 뮤직 플레이어




#define MAX_OPTION_VALUE						9



#define MIP_FILTER_DISTANCE						60




#define MAP_TYPE_CITY_UNIT_POS					D3DXVECTOR3(2400,19,2400)




#define HELPDESK_STATE_DEFAULT					0			// 렌더링하지않는다
#define HELPDESK_STATE_NEW						1			// 새로운 캐릭터를 만들었을때 도움말
#define HELPDESK_STATE_CITY						2			// 도시 내에서 도움말
#define HELPDESK_STATE_MISSION					3			// 미션 진행시 도움말
#define HELPDESK_STATE_FIELD					4			// 필드 내에서 도움말



#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
// 캐쉬상점관련 렌더링 위치
#define CHAT_MENU_CASH_START_W		75 
#define CHAT_MENU_CASH_START_H		57
#define CHAT_MENU_CASH_START_X		(g_pD3dApp->GetBackBufferDesc().Width-CHAT_MENU_CASH_START_W)
#define CHAT_MENU_CASH_START_Y		(g_pD3dApp->GetBackBufferDesc().Height-CHAT_MENU_CASH_START_H-84)
#else
// 캐쉬상점관련 렌더링 위치
#define CHAT_MENU_CASH_START_W		65
#define CHAT_MENU_CASH_START_H		19
#define CHAT_MENU_CASH_START_X		(g_pD3dApp->GetBackBufferDesc().Width-CHAT_MENU_CASH_START_W)
#define CHAT_MENU_CASH_START_Y		(g_pD3dApp->GetBackBufferDesc().Height-CHAT_MENU_CASH_START_H-42)
#endif



// a기어 1형 무기 회전 중심좌표
//#define A_GEAR_PRIMARY_PORT_POSITION		D3DXVECTOR3(0.0f,0.662f,5.765f)
#define A_GEAR_PRIMARY_PORT_POSITION		D3DXVECTOR3(0.0f,0.0f,2.843f)


// 2009. 07. 07 by ckPark 로봇기어 요청사항(롤링, 선택화면, 무기, A기어포대)
#define A_GEAR_ROBOT_PRIMARY_WEAPON_ITEM_NUM	7023980
// end 2009. 07. 07 by ckPark 로봇기어 요청사항(롤링, 선택화면, 무기, A기어포대)




// sound code
#define SOUND_GET_ITEM						0
#define SOUND_WEAPON_RELOAD					7
#define SOUND_SEC_BULLET_RELOADED			9
#define SOUND_FLYING_M_A_GEAR				364//32
#define SOUND_HIGH_BOOSTER_START			360//33
#define SOUND_TAKINGOFF						34
#define SOUND_TAKEINGOFF_IN_CITY			35
#define SOUND_LOCKON_PRIMARY				36
#define SOUND_STAT_UP						38
#define SOUND_WEAR_WEAPON					39
#define SOUND_WARNING						40
#define SOUND_INPUT_CHAT					41
#define SOUND_SEND_CHAT						45
#define SOUND_SELECT_UNIT_CLICK				54
#define SOUND_SELECT_BUTTON					55
#define SOUND_SELECT_UNIT_STOP				56
#define SOUND_SELECT_ROTATE					57
#define SOUND_OFF_WEAPON					58
#define SOUND_MOVING_ON_WATER				60
#define SOUND_FLYING_B_I_GEAR				362//64
#define SOUND_LANDING						67
#define SOUND_LANDING_IN_CITY				68
#define SOUND_WARNING_HP					72
#define SOUND_BUTTON_MGAME					73
#define SOUND_BUTTON_OK						74
#define SOUND_BUTTON_CANCEL					75
#define SOUND_LOCKON_SECONDARY				76
#define SOUND_OPEN_MENU						80
#define SOUND_OPEN_MESSAGE_BOX				81
#define SOUND_WARP							82
#define SOUND_USEITEM_REPAIR_HP				83
#define SOUND_USEITEM_REPAIR_DP				84
#define SOUND_CLOSE_MENU					88
#define SOUND_CLOSE_MESSAGE_BOX				89
#define SOUND_GROUND_MOVING_A_GEAR			91
#define SOUND_LANDING_DONE					96
#define SOUND_MISSILE_WARNNING				97			// 2005-07-08 by ispark // 미사일 경고
#define SOUND_LUCKY_COIN					98
#define SOUND_LUCKY_SLOT					99
#define SOUND_WARP_2						2006023		// 2008-06-24 by dgwoo #define SOUND_WARP	82  워프 사운드 추가.
#define	SOUND_DISSOLUTION					400031      // 2011-03-08 by jhahn 용해 사운드

#define SOUND_HIGH_BOOSTER					361//1033
#define SOUND_HIGH_BOOSTER_END				363//2033
#define SOUND_LOW_BOOSTER_START				360//3033
#define SOUND_LOW_BOOSTER					361//4033
#define SOUND_LOW_BOOSTER_END				365//5033

#define SOUND_PVP_COUNT						300000
#define SOUND_PVP_START						300002

// 2006-10-11 by ispark, 캡슐 사운드
#define SOUND_POWER_ENERGY_CAPSULE			400001
#define SOUND_POWER_SHIELD_CAPSULE			400500
#define SOUND_POWER_SP_CAPSULE				22
#define SOUND_ADRENALIN_CAPSULE				400037
#define SOUND_BRAVE_CAPSULE					400504
#define SOUND_HYPER_BOOSTER_CAPSULE			400545



//#define IS_CLIENT_SET_AUTOMATIC_TIMER(x)	x == ITEMKIND_AUTOMATIC ||			\
//											x == ITEMKIND_GRENADE ||			\
//											x == ITEMKIND_RIFLE ||				\
//											x == ITEMKIND_LAUNCHER
#define IS_CLIENT_SET_AUTOMATIC_TIMER(x)	((x >= ORBIT_SINGLE_FIRE_200 && x <= ORBIT_SINGLE_FIRE_350 )||			\
											(x >= ORBIT_TWIN_FIRE_200 && x <= ORBIT_TWIN_FIRE_350)||				\
											(x >= ORBIT_STRAIGHT_ROCKET_250 && x <= ORBIT_STRAIGHT_MISSILE_300)||	\
											(x >= ORBIT_SINGLE_FIRE_100 && x <= ORBIT_TWIN_FIRE_150))



#define IS_OBJECTMONSTER_BY_MONSTERFORM(x)	((x) == FORM_OBJECT_STOP ||			\
											(x) == FORM_OBJECT_PLANE_ROTATE ||	\
											(x) == FORM_OBJECT_CANNON)








// 공격 가능 Enemy Flag 정의
#define PK_NONE		0x00000000
#define PK_PVP		0x00000001
#define PK_FVF		0x00000002
#define PK_GVG		0x00000004
#define PK_WAR		0x00000008
#define PK_NORMAL	0x00000010	// 강제 PK
#define PK_ALL		0xFFFFFFFF



#define IS_WEAPON_ENABLESTATE(state,booster)	state == _NORMAL &&											\
												booster != BOOSTER_TYPE_AIRBREAK	&&						\
												!g_pGameMain->m_pTrade->m_bTrading &&						\
												!g_pD3dApp->m_bChatMode &&									\
												g_pGameMain->m_nLeftWindowInfo == LEFT_WINDOW_WEAPON &&		\
												g_pGameMain->m_nRightWindowInfo == RIGHT_WINDOW_WEAPON &&	\
												g_pShuttleChild->m_bRollUsed == FALSE &&					\
												g_pShuttleChild->m_bUnitStop == FALSE &&					\
												g_pShuttleChild->m_bPenaltyUnitStop == FALSE






#define BOOSTER_OVERHEAT_REPAIR_TIME			10.0f	// 10초




// 마인 최대 개수
#define MAX_MINE_NUM				100






#define WATER_TEXTURE_COUNT			40




// 2004-10-13 by jschoi
// 스킬 상태 
#define SKILL_STATE_READY			0
#define SKILL_STATE_WAITING_PREPARE	1
#define SKILL_STATE_PREPARE			2
#define SKILL_STATE_WAITING			3
#define SKILL_STATE_USING			4
#define SKILL_STATE_WAIT_REATTACK	5
#define SKILL_STATE_RELEASE			6



// 2005-11-22 by ispark, 서버에서 정의
//// 스킬 관련 정의
//#define	SIEGE_MODE			7820050	// 2005-11-18 by ispark 7820040 -> 7820050
//#define SIEGE_DEFENSE_MODE	7821040	// 2005-11-18 by ispark 7821030 -> 7821040
//#define GROUND_ATTACK_MODE	7800040
//#define SMART_SPEED			7812020
//#define GROUND_ACCELERATOR	7822010	// 2005-11-18 by ispark 7822020 -> 7822010
//#define FLASH_ACCELERATOR	7822030


// 일정 고도 이상 상승시 상수
#define ALTITUDE_APPLY_MIN			600.0f
#define ALTITUDE_APPLY_MAX			1600.0f
#define ALTITUDE_DEST_RATE			0.8f
#define ALTITUDE_SAPCE_ALTITUDE		800.0f






// 1형 무기, 전체 공격 속도
#define MAX_WEAPON_SPEED	3000.0f











// Object Type
#define OBJECT_NORMAL			0		// 거리 렌더컬링검사,절두체 컬링검사, EX:일반 오브젝트(나무)
#define OBJECT_BIG				1		// 거리에 상관없이 무조건 렌더링,절두체 컬링만 검사, EX:워프게이트,착륙장
#define OBJECT_BIG_CULLED		2		// 렌더컬링*(렌더링 영역에서 반지름 검사), EX:식빵
#define OBJECT_BIG_NO_COLLISION	3		// 렌더컬링*(렌더링 영역에서 반지름 검사), 충돌검사 안함, EX:미역
#define OBJECT_BIG_EFFECT		4		// 거리 렌더컬링만 검사.( 이펙트를 위한 오브젝트 ), EX:폭포이펙트,화산이펙트
#define OBJECT_MONSTER			7		// 렌더링 안하고, 충돌검사 안하는 오브젝트, EX:몬스터 오브젝트
#define OBJECT_MONSTER_OBJECT	8		// 오브젝트 몬스터 사망시 대체되는 오브젝트 타입 , EX:코어 오브젝트
#define OBJECT_WATER			9		// 물 타일 오브젝트(착륙불가,미니맵 렌더링 불가,물위를 지날때 물방울 오브젝트 발생,깊이 버퍼 기록안함)
// Object Type에 추가
#define OBJECT_BIG_CONTOUR		10		// 렌더컬링*(렌더링 영역에서 반지름 검사), 미니맵에서 렌더링하지 않음, 충돌 검사 하지 않음, EX:맵 외곽 오브젝트
#define OBJECT_SHOP_NPC			11		// 상점 NPC 캐릭터 (상점 검색 방식이 영역으로 설정(NO Picking))


















#define	UNIT_STATE_MOVE(x)		(x == _NORMAL ||		\
								x == _EXPLODING ||		\
								x == _FALLING ||		\
								x == _LANDING ||		\
								x == _TAKINGOFF)

#define UNIT_STATE_NO_MOVE(x)	(x == _LANDED ||		\
								x == _FALLEN ||			\
								x == _EXPLODED)



// 2006-01-18 by ispark
#define UNIT_STATE_DEAD(x)		(x == _FALLING ||		\
								x == _FALLEN)











#define STAGE_EFFECT_MASANGSOFT			0
#define STAGE_EFFECT_MGAME				1
#define STAGE_EFFECT_GAME_LOGO			2
#define STAGE_EFFECT_GAME_WAITING		3














// 시네마 패턴 타입
#define PATTERN_SOUND					0
#define PATTERN_UNIT					1
#define PATTERN_MONSTER					2
#define PATTERN_OBJECT					3
#define PATTERN_INTERFACE				4
#define PATTERN_TILE					5
#define PATTERN_EFFECT					6
#define PATTERN_ITEM					7
#define PATTERN_CAMERA					8

#define PATTERN_DEFAULT					"99999999"

#define PATTERN_CAMERA_DEFAULT			8000000		// 디폴트 카메라
#define PATTERN_CAMERA_COBRA			8000001		// 유닛 코브라 스킬 카메라
#define PATTERN_CAMERA_FALLING			8000002
#define PATTERN_CAMERA_WARP_OUT			8000003		// 워프 나올때
#define PATTERN_CAMERA_WARP_IN			8000004		// 워프 들어갈때
#define PATTERN_CAMERA_CITY_OUT			8000005		// 사용 안함
#define PATTERN_CAMERA_CITY_IN			8000006		// 사용 안함
#define PATTERN_CAMERA_GAME_START		8000007
#define PATTERN_CAMERA_GAME_END			8000008
#define PATTERN_CAMERA_LANDING			8000009
#define PATTERN_CAMERA_AIRPORT_IN		8700000
#define PATTERN_CAMERA_AIRPORT_OUT		5020001		// 2006-11-01 by dgwoo //8800000
#define PATTERN_CAMERA_AIRPORT_NORMALCITY_OUT		5020002		// 2007-06-13 by bhsohn 아카데미맵 시네마 추가
#define PATTERN_CAMERA_FAIL_INFINITY	5020004		// 인피니티 실패 카매라 패턴
#define PATTERN_CAMERA_SUCCESS_INFINITY	5020005		// 인피니티 성공 카매라 패턴
#define PATTERN_CAMERA_FAIL_INFINITYTHIRD		5020006			// 2011-06-23 by jhahn 인피3차 시네마 적용 카메라 구현
#define PATTERN_CAMERA_SUCCESS_INFINITYTHIRD	5020007			// 2011-06-23 by jhahn 인피3차 시네마 적용 카메라 구현


#define PATTERN_UNIT_FALLING			1000001
#define PATTERN_UNIT_WARP_OUT			1000002
#define PATTERN_UNIT_WARP_IN			1000003
#define PATTERN_UNIT_CITY_OUT			1000004
#define PATTERN_UNIT_CITY_IN			1000005
#define PATTERN_UNIT_GAME_START			1000006		// 사용 안함
#define PATTERN_UNIT_GAME_END			1000007		// 사용 안함
#define PATTERN_UNIT_AIRPORT_IN			1700000
#define PATTERN_UNIT_AIRPORT_OUT		1800000



// 충돌 타입
#define COLL_NONE		-1
#define COLL_FRONT		0
#define	COLL_BACK		1
#define	COLL_LEFT		2
#define	COLL_RIGHT		3
#define	COLL_UP			4
#define	COLL_DOWN		5
#define	COLL_WHOLE		6







// 몬스터 스킬 상태
#define MONSTER_SKILL_ON				1
#define MONSTER_SKILL_OFF				2
#define MONSTER_WAIT_END_SKILL			3








// 더미 데이타 좌표
#define DUMMY_POSITION_1			D3DXVECTOR3( 0.0f, -3.75f, -19.31f )
#define DUMMY_POSITION_2			D3DXVECTOR3( 19.31f, -3.75f, 11.80f )
#define DUMMY_POSITION_3			D3DXVECTOR3( -19.31f, -3.75f, 11.80f )
#define DUMMY_POSITION_4			D3DXVECTOR3( 0.0f, 9.7f, 11.80f )
#define DUMMY_POSITION_5			D3DXVECTOR3( 0.0f, -3.75f, 24.52f )













// 디폴트 곡률(시네마 에디터)
#define DEFAULT_CURVATURE					1.0f




// 빅 오브젝트 분류 기준 크기
#define BIG_OBJECT_SIZE		100


// 빅 몬스터 분류 기준 크기
#define BIG_MONSTER_SIZE	150.0f


#define GUI_FONT_COLOR						RGB(222, 222, 222)		// 기본 색상(흰색)
#ifdef C_EPSODE4_UI_CHANGE_JSKIM
#define GUI_SELECT_FONT_COLOR				RGB(161, 220, 252)			// 선택 색상(검은색)
#else
#define GUI_SELECT_FONT_COLOR				RGB(1, 1, 1)			// 선택 색상(검은색)
#endif
#define GUI_FONT_COLOR_YM					RGB(255, 168, 0)		// 기본 색상(주황색)
#define GUI_FONT_COLOR_BM					RGB(178, 190, 255)		// 기본 색상(연한하늘색)
#define GUI_FONT_COLOR_Y					RGB(255, 255, 0)		// 기본 색상(노란색)
#define GUI_FONT_COLOR_R					RGB(255, 0, 0)			// 기본 색상(빨간색)
#define GUI_FONT_COLOR_G					RGB(128, 128, 128)		// 기본 색상(회색색)
#define GUI_FONT_COLOR_B					RGB(0, 0, 255)			// 기본 색상(파란색)
#define GUI_FONT_COLOR_W					RGB(255, 255, 255)		// 기본 색상(흰색)
#define GUI_FONT_COLOR_GR					RGB(211, 211, 211)		// 기본 색상(회색)
#define GUI_FONT_COLOR_C					RGB(0, 255, 255)		// Cyan



#define NAME_COLOR_NORMAL					RGB(222, 222, 222)		// 기본 이름 색상
#define NAME_COLOR_ATTACKER					RGB(255, 50, 50)		// 도시점령전 공격측 이름 색상
#define NAME_COLOR_DEFENSER					RGB(50, 50, 255)		// 도시점령전 방어측 이름 색상
#define NAME_COLOR_SAFEJONE					RGB(255, 255, 0)			// 2011-11-03 by jhahn EP4 중립지역 아이디 색 변경





// 유닛의 상태
#define ATTACK_AVAILABLE_STATE(X)	(X == _NORMAL || X == _LANDING || X == _TAKINGOFF || X == _LANDED) 
#define MOVE_AVAILABLE_STATE(X)		(X == _NORMAL || X == _LANDING || X == _TAKINGOFF || X == _LANDED) 












typedef DWORD InfUnitState_t;
// Inf Unit Warning
#define INF_UW_DANGER_MASK			(InfUnitState_t)0x00000001		// 기체 손상 위험
#define INF_UW_OVERHEAT_MASK		(InfUnitState_t)0x00000002		// 부스터 과열
#define INF_UW_LOWFUEL_MASK			(InfUnitState_t)0x00000004		// 연료 부족
#define INF_UW_FULLITEM_MASK		(InfUnitState_t)0x00000008		// 아이템 초과(예비)
#define INF_UW_TOOHIGH_MASK			(InfUnitState_t)0x00000010		// 고도 제한
#define INF_UW_LOWSHIELD_MASK		(InfUnitState_t)0x00000020		// 쉴드 부족
#define INF_UW_FULLITEM1_MASK		(InfUnitState_t)0x00000040		// 아이템 초과(70%)
#define INF_UW_FULLITEM2_MASK		(InfUnitState_t)0x00000080		// 아이템 초과(80%)
#define INF_UW_FULLITEM3_MASK		(InfUnitState_t)0x00000100		// 아이템 초과(90%)

// Inf(인터페이스) Unit State
#define INF_US_TAKEOFF_MASK			(InfUnitState_t)0x00010000		// 이륙된 상태(Take Off ) - 다시착륙-0(ins)
#define INF_US_STOP_MASK			(InfUnitState_t)0x00020000		// 유닛 공중에 정지됨 - 해제 -전진키 W
#define INF_US_LOCK_MASK			(InfUnitState_t)0x00040000		// 움직일수 없는 상태(Unit Lock) - R
#define INF_US_FORM_MASK			(InfUnitState_t)0x00080000		// formation 편대비행중( 합류키 f2)
#define INF_US_LAND_MASK			(InfUnitState_t)0x00100000		// 유닛 Landing, 착륙중 - 다시 이륙 W















// 버튼중 양면을 뛰는 버튼일 경우.[2][2]
#define BUTTON_BOTH_STATE_NORMAL		0
#define BUTTON_BOTH_STATE_UP			1
#define BUTTON_BOTH_STATE_NUMBER		2

// 인터페이스 버튼 상태
#define BUTTON_STATE_UP			0
#define BUTTON_STATE_DOWN		1
#define BUTTON_STATE_DISABLE	2
#define BUTTON_STATE_NORMAL		3

#define BUTTON_STATE_NUMBER		4

// 래디오 버튼.
#define	RADIOBUTTON_STATE_NORMAL	0
#define RADIOBUTTON_STATE_SELECT	1
#define RADIOBUTTON_STATE_NUMBER	2

// 게임중 이미지 타잎에 따른 define
#define TOOLTIP_IMGTYPE_NORMAL					0
#define TOOLTIP_IMGTYPE_TELEPORT_SUC			1			// 텔레포트가 설치 되었습니다.
#define TOOLTIP_IMGTYPE_TELEPORT_DESTROY		2			// 텔레포트가 파괴 되었습니다.





// 데미지를 많이 입었을 경우 바디컨디션 세팅을 위해 사용(유닛, 몬스터)
#define CHARACTER_DAMAGE_STATE_05		0.25f
#define CHARACTER_DAMAGE_STATE_10		0.35f
#define CHARACTER_DAMAGE_STATE_15		0.40f





// UNIT_STATE : 연료 부족
#define CHARACTER_LOWFUEL_STATE			0.1f





// 쉴드 타격 이펙트 발동시 무기와 유닛과의 거리
#define SIZE_OF_SHIELD_EFFECT						15.0f
// 1형 무기 공격시 유닛에서 랜덤하게 폭발하는 거리
#define SIZE_OF_PRIMARY_ATTACK_RANDOM_DISTANCE		10








// shield bodycondition
#define BODYCON_SHIELD_ON_MASK			BODYCON_FIRE_MASK
#define BODYCON_SHIELD_OFF_MASK			BODYCON_HIT_MASK
#define BODYCON_SHIELD_ING_MASK			BODYCON_BULLET_MASK
// charging shot bodycondition
#define BODYCON_CHARGING_MASK			BODYCON_LANDED_MASK		// 2
#define BODYCON_USE_SKILL_MASK			BODYCON_FLY_MASK		// 1 , USE_SKILL과 같이 사용한다.
#define BODYCON_TARGET_USE_SKILL_MASK	BODYCON_DEAD_MASK		// 3 , 스킬을 맞은 이펙트 바디컨디션








//카메라 타입
#define CAMERA_TYPE_NORMAL			0//3인칭
#define CAMERA_TYPE_FPS				1//1인칭








// 날씨에 따른 포그 거리
#define WEATHER_DEFAULT_FOG_START	50.0f
#define WEATHER_DEFAULT_FOG_END		640.0f
#define WEATHER_SUNNY_FOG_START		1900.0f//4000.0f
#define WEATHER_SUNNY_FOG_END		2500.0f//6300.0f
#define WEATHER_RAINY_FOG_START		100.0f
#define WEATHER_RAINY_FOG_END		400.0f
#define WEATHER_SNOWY_FOG_START		1000.0f//0.0f			// 2006-12-11 by ispark
#define WEATHER_SNOWY_FOG_END		1800.0f//600.0f			// 2006-12-11 by ispark
#define WEATHER_CLOUDY_FOG_START	300.0f
#define WEATHER_CLOUDY_FOG_END		2250.0f
#define WEATHER_FOGGY_FOG_START		0.0f
#define WEATHER_FOGGY_FOG_END		200.0f




#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
#define SHOP_ITEM_SLOT_NUMBER		8															  
#else 
#define SHOP_ITEM_SLOT_NUMBER		5
#endif
#define SHOP_ITEM_TAB_NUMBER		4






#define IS_ITEM_SHOP_TYPE(x)		(x == BUILDINGKIND_1ST_WEAPONSHOP ||	\
									x == BUILDINGKIND_2ND_WEAPONSHOP ||		\
									x == BUILDINGKIND_COMPONENTSHOP ||		\
									x == BUILDINGKIND_PETSHOP ||			\
									x == BUILDINGKIND_ANTIQUESHOP ||		\
									x == BUILDINGKIND_REFINERY	||			\
									x == BUILDINGKIND_TUNING_COLOR	||		\
									x == BUILDINGKIND_PARTSSHOP	||		\
									x == BUILDINGKIND_GEARSHOP	||		\
									x == BUILDINGKIND_CARDSHOP	||		\
									x == BUILDINGKIND_TRIGGER_CRYSTAL)
									// 2012-02-29 by mspark, 상점 타이틀 넣기 - 파츠상점, 기어상점, 카드상점 추가
									
#define IS_STORE_SHOP_TYPE(x)		(x == BUILDINGKIND_STORE)					// 창고
#define IS_WARP_SHOP_TYPE(x)		(x == BUILDINGKIND_CITYWARP)					// 개인 격납고, 워프
#define IS_MISSION_SHOP_TYPE(x)		(x == BUILDINGKIND_PUBLICOFFICE ||		\
									x == BUILDINGKIND_MILITARYACADEMY)			// 사관학교파트
#define IS_RACING_SHOP_TYPE(x)		(x == BUILDINGKIND_RACETRACK)				// 레이싱파트
#define IS_ACTIONHOUSE_SHOP_TYPE(x)	(x == BUILDINGKIND_ACTIONHOUSE)				// 경매파트
#define IS_LABORATORY_SHOP_TYPE(x)	(x == BUILDINGKIND_LABORATORY)				// 연구소(인챈트)
#define IS_FACTORY_SHOP_TYPE(x)		(x == BUILDINGKIND_FACTORY)					// 팩토리(조합)
#define IS_SKILL_SHOP_TYPE(x)		(x == BUILDINGKIND_SKILL_SHOP)				// 스킬상점
#define IS_AUCTION_SHOP_TYPE(x)		(x == BUILDINGKIND_AUCTION)					// 경매
#define IS_CITY_OCCUPY_INFO_TYPE(x)	(x == BUILDINGKIND_CITY_OCCUPY_INFO)		// 도시 관리
#define IS_TUNING_COLOR_SHOP(x)		(x == BUILDINGKIND_TUNING_COLOR)			// 칼라 상점
#define IS_CASH_SHOP_TYPE(x)		(x == BUILDINGKIND_CASH_SHOP)				// 캐쉬 상점
#define IS_WARPOINT_SHOP_TYPE(x)	(x == BUILDINGKIND_WARPOINT_SHOP)			// 아레나 
#define IS_OUTPOST_SHOP_TYPE(x)		(x == BUILDINGKIND_CITYLEADER_LEADER ||	\
									 x == BUILDINGKIND_CITYLEADER_OUTPOST)			// 아레나 


// 상점 버튼 종류 추가(서버에서 쓰이지 않음)
#define BUILDINGKIND_OUTDOOR					99	//나가기



// HELP 데이타 보여주는 레벨
#define SHOW_TEXT_HELP_LEVEL		6



// 워프 게이트 바디 컨디션
#define BODYCON_WARP_MASK					BODYCON_DEAD_MASK

// 2007-04-18 by dgwoo 아레나 맵에서 문 상태.
#define BODYCON_DOOR_CLOSE_MASK				BODYCON_DEAD_MASK			// 문이 닫혀있는 상태.
#define BODYCON_DOOR_OPENING_MASK			BODYCON_LANDING_MASK		// 문이 열리고 있는 상태.
#define BODYCON_DOOR_OPEN_MASK				BODYCON_LANDED_MASK			// 문이 열려진 상태.
#define BODYCON_DOOR_CLOSING_MASK			BODYCON_NOT_USED1_MASK		// 문이 닫히고 있는 상태.


// 장착 무기 바디 컨디션, UnitData.cpp
#define	WEAPON_BODYCON_LOADING				BODYCON_FLY_MASK		// 무기 장착시(예비동작)
#define WEAPON_BODYCON_FIRE_1				BODYCON_LANDED_MASK		// 유닛에서 무기가 발사되는 상태(1번)
#define WEAPON_BODYCON_FIRE_2				BODYCON_DEAD_MASK		// 무기발사상태(2번), 지그재그시 사용
#define WEAPON_BODYCON_RELOAD				BODYCON_NOT_USED1_MASK	// 재장전






// 장착된 쉴드,디코이류 아이템 바디 컨디션
/*
#define	WEAPON_BODYCON_NORMAL				BODYCON_FLY_MASK		// 평상시(쉴드 발동하기 전)
#define WEAPON_BODYCON_START				BODYCON_LANDED_MASK		// 시전-->자동으로 사용 상태로 넘어감
//#define WEAPON_BODYCON_ING				BODYCON_DEAD_MASK		// 사용 상태(쉴드 발동)필요없음
#define WEAPON_BODYCON_END					BODYCON_NOT_USED1_MASK	// 시전 종료(쉴드 발동 종료-->자동으로 NORMAL로 넘어감)
*/
#define TOGGLE_WEAPON_BODYCON_NORMAL		(BodyCond_t)0x0000000000000002	// 평상시
#define TOGGLE_WEAPON_BODYCON_USE			(BodyCond_t)0x0000000000000004	// 시전-->USING과 동시에 사용
#define TOGGLE_WEAPON_BODYCON_USING			(BodyCond_t)0x0000000000000008	// 사용 상태
#define TOGGLE_WEAPON_BODYCON_END			(BodyCond_t)0x0000000000000010	// 사용 종료-->NORMAL과 동시에 사용
#define TOGGLE_WEAPON_BODYCON_RELOAD		(BodyCond_t)0x0000000000000020	// 재장전



// 장착 일반 아이템 바디 컨디션(이펙트 추가 예정, 2004-03-23 by dhkwon)
#define WEAR_ITEM_BODYCON_NORMAL			BODYCON_FLY_MASK








// 무기의 바디컨디션이 유닛카인드별로 SHIFT됨에 따라 설정
#define UNIT_KIND_SHIFT_DEFAULT				0	// 기본(몬스터에 사용)
#define UNIT_KIND_SHIFT_B_GEAR				0	// B기어
#define UNIT_KIND_SHIFT_M_GEAR				10	// M기어
#define UNIT_KIND_SHIFT_A_GEAR				20	// A기어
#define UNIT_KIND_SHIFT_I_GEAR				30	// I기어







// 거리 관련 상수
#define RANGE_OF_VISION						1000.0f//750.0f	// 화면에 보이는 시야
#define GET_INFO_DISTANCE_OF_2D				30.0f	// 화면상에서 클릭했을 경우 정보를 가져오는 거리
#define GET_ITEM_FROM_FIELD_DISTANCE		200.0f	// 필드 아이템을 습득하는 거리
#define GET_ITEM_NUMBER_IN_TICK				2		// 한번에 습득할 수 있는 아이템 수
#define GET_ITEM_IN_TIME					1.0f	// 2006-07-18 by ispark, 한번에 습득할 수 있는 키 딜레이 타임
#define DETETE_FIELD_ITEM_DISTANCE			750.0f	// 필드 아이템이 지워지는 거리
#define SUN_DISTANCE						8000.0f	// 태양, 달의 거리
#define DEFAULT_COLLISION_DISTANCE			10000.0f // 충돌 검사 가능한 최대거리
#define SEND_FC_TARGET_DISTANCE				400.0f	// MSG_FC_MOVE_TARGET 보내는 두번째 거리
#define REMOVE_ENEMY_DISTANCE				1000.0f//700.0f	// Enemy를 지우는 거리


// marked as todo so that i can find it more easily
// todo : here are wear_item_kind related definitions

enum
{
	WEAR_ITEM_KIND_WEAPON_11,
	WEAR_ITEM_KIND_WEAPON_12,
	WEAR_ITEM_KIND_WEAPON_21,
	WEAR_ITEM_KIND_WEAPON_22,
	WEAR_ITEM_KIND_PROW,
	WEAR_ITEM_KIND_CENTER,
	WEAR_ITEM_KIND_ACCESSORY_UNLIMITED,
	WEAR_ITEM_KIND_ENGINE,
	WEAR_ITEM_KIND_ACCESSORY_TIME_LIMIT,
	WEAR_ITEM_KIND_WINGIN,
	WEAR_ITEM_KIND_PET,
};

enum
{
	EFFECT_WEAR_WEAPON_1 = 100000,		// 아이템 번호에서 이값을 더하면 이펙트 번호가 된다.
	EFFECT_WEAR_WEAPON_2 = 700000,		// 아이템 번호에서 이값을 빼면 이펙트 번호가 된다.
	EFFECT_WEAR_WEPAON_11_DEFAULT = 12100000,
	EFFECT_WEAR_WEPAON_12_DEFAULT = 12200000,
	EFFECT_WEAR_WEPAON_21_DEFAULT = 12300000,
	EFFECT_WEAR_WEPAON_22_DEFAULT = 12400000,
};

// todo : documentation on EFFECT_NUMBER_WEAR
// Documentation :
// Here is the part that finds the effects that will render the weapons on the the gears (that is the "contour")

template<typename item_t>
int EFFECT_NUMBER_WEAR_WEAPON_11(item_t* p1, int p2)
{
	if (p1) return p1->SourceIndex + EFFECT_WEAR_WEAPON_1;
	
	return EFFECT_WEAR_WEPAON_11_DEFAULT + p2 * 100;
}

template<typename item_t>
int EFFECT_NUMBER_WEAR_WEAPON_12(item_t* p1, int p2)
{
	if (p1) return p1->SourceIndex + EFFECT_WEAR_WEAPON_1;
	
	return EFFECT_WEAR_WEPAON_12_DEFAULT + p2 * 100;
}

template<typename item_t>
int EFFECT_NUMBER_WEAR_WEAPON_21(item_t* p1, int p2)
{
	if (p1) return p1->SourceIndex - EFFECT_WEAR_WEAPON_2;
	
	return EFFECT_WEAR_WEPAON_21_DEFAULT + p2 * 100;
}

template<typename item_t>
int EFFECT_NUMBER_WEAR_WEAPON_22(item_t* p1, int p2)
{
	if (p1) return p1->SourceIndex - EFFECT_WEAR_WEAPON_2;
	
	return EFFECT_WEAR_WEPAON_22_DEFAULT + p2 * 100;
}

constexpr int EFFECT_NUMBER_WEAR_RADAR(int x) { return x + 500000; }

constexpr int SELECT_MENU_DEFAULT_WEAR_WEAPON_11(int x) { return EFFECT_WEAR_WEPAON_11_DEFAULT + x * 100; }
constexpr int SELECT_MENU_DEFAULT_WEAR_WEAPON_12(int x) { return EFFECT_WEAR_WEPAON_12_DEFAULT + x * 100; }
constexpr int SELECT_MENU_DEFAULT_WEAR_WEAPON_21(int x) { return EFFECT_WEAR_WEPAON_21_DEFAULT + x * 100; }
constexpr int SELECT_MENU_DEFAULT_WEAR_WEAPON_22(int x) { return EFFECT_WEAR_WEPAON_22_DEFAULT + x * 100; }

// 엔진은 유닛카인드별 SHIFT 규칙을 따르지 않는다.
// why ????
#define GET_SHIFT_BODYCON_OF_UNITKIND_EXCEPT_ENGINE		(nWearItemKind == WEAR_ITEM_KIND_ENGINE || nWearItemKind == WEAR_ITEM_KIND_PET ) ? UNIT_KIND_SHIFT_DEFAULT : GetShiftWeaponBodyconditionByUnitKind()









// IM 서버 진행 상황, CINFMapLoad, AtumApplication
#define IM_SERVER_STATE_NOT_LOGIN			0
#define IM_SERVER_STATE_TRY_LOGIN			1
#define IM_SERVER_STATE_SUCCESS_LOGIN		2
#define IM_SERVER_STATE_READY_FOR_GAMESTART	3








// 기어 이름 짓는 규칙
// 1째 짜리 : 1
// 2째 자리 : 폴리곤 단계(low:0, middle:1, high:2)
// 5째 자리 기어 구분( B:0, M:1, A:2, I:3 )
// 6째 자리 업그레이드 ( 1형 : 1 , 2형 : 2, 3형 : 3, 4형 : 4 )
#define UNIT_EFFECT_INDEX(x,y,z)		(10000000+1000000*x+1000*y+100*z)// x : 폴리곤 단계, y : 기어, z : 업그레이드

// 유닛의 애니메이션을 결정하는 바디컨디션
//#define BODYCON_BOOSTER_EX_STATE_CLEAR_MASK	(BodyCond_t)(BODYCON_BOOSTER1_MASK | BODYCON_BOOSTER2_MASK | BODYCON_BOOSTER3_MASK | BODYCON_BOOSTER4_MASK | BODYCON_BOOSTER5_MASK)

// 2009. 07. 07 by ckPark 로봇기어 요청사항(롤링, 선택화면, 무기, A기어포대)
// BODYCON_ROLLING_MASK 롤링시 애니메이션 추가

// 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템
// BODYCON_CREATION_MASK 추가
#define BODYCON_CHARACTER_ANIMATION_TIME	(BodyCond_t)(BODYCON_BOOSTER_EX_STATE_CLEAR_MASK |	\
											 BODYCON_LANDING_MASK |								\
											 BODYCON_LANDED_MASK |								\
											 BODYCON_DEAD_MASK |								\
											 BODYCON_NOT_USED1_MASK	|							\
											 BODYCON_ROLLING_MASK)
#define BODYCON_MONSTER_ANIMATION_TIME		(BodyCond_t)(BODYCON_BOOSTER_EX_STATE_CLEAR_MASK |	\
											 BODYCON_LANDING_MASK |								\
											 BODYCON_LANDED_MASK |								\
											 BODYCON_EXPLOSION_MASK |							\
											 BODYCON_MON_ATTACKALL_MASK|						\
 											 BODYCON_MON_AUTODESTROYED_MASK|					\
											 BODYCON_MON_BUILD_MASK|							\
											 BODYCON_CREATION_MASK)
//											 BODYCON_NOT_USED1_MASK)
// 부스터 껐을때 사용하는 바디컨디션(SetFlyBodyCondition)
#define BODYCON_BOOSTER_OFF_MASK			0


// 2009. 07. 07 by ckPark 로봇기어 요청사항(롤링, 선택화면, 무기, A기어포대)
// BODYCON_ROLLING_MASK 롤링시 애니메이션 추가
// 엔진 바디컨디션 마스크(CUnitData: ChangeBodyCondition,ResetBodyCondition)
#define BODYCON_ENGINE_STATE_MASK			(BODYCON_BOOSTER_EX_STATE_CLEAR_MASK |				\
											BODYCON_LANDING_MASK |								\
											BODYCON_LANDED_MASK	|								\
											BODYCON_TAKEOFF_MASK |								\
											BODYCON_SIEGE_OFF_MASK |							\
											BODYCON_SIEGE_ON_MASK |								\
											BODYCON_ROLLING_MASK)
// 유닛 바디컨디션 중 엔진으로 갈때 BODYCON_LANDED_MASK 로 바뀌는 것
#define BODYCON_UNIT_TO_ENGINE_LANDED		(BODYCON_EXPLOSION_MASK	| BODYCON_DEAD_MASK)

// 클라이언트는 다음의 바디컨디션은 하나씩만 사용한다.
#define BODYCON_CLIENT_SINGLE				(BODYCON_EX_STATE_CLEAR_MASK|						\
											BODYCON_BOOSTER_EX_STATE_CLEAR_MASK|				\
											BODYCON_EXPLOSION_MASK|								\
											BODYCON_TAKEOFF_MASK|								\
											BODYCON_SIEGE_ON_MASK|								\
											BODYCON_SIEGE_OFF_MASK)
// 몬스터는 다음의 바디컨디션은 하나씩만 사용한다.
#define BODYCON_MONSTER_SINGLE				(BODYCON_EX_STATE_CLEAR_MASK|						\
											BODYCON_BOOSTER_EX_STATE_CLEAR_MASK|				\
											BODYCON_EXPLOSION_MASK|								\
											BODYCON_CREATION_MASK|								\
											BODYCON_LANDING_MASK|								\
											BODYCON_LANDED_MASK|								\
											BODYCON_MON_AUTODESTROYED_MASK)
// 아레나 문.
#define BODYCON_ARENA_DOOR_STATE			(BODYCON_DOOR_CLOSE_MASK|							\
											BODYCON_DOOR_OPEN_MASK|								\
											BODYCON_DOOR_CLOSING_MASK|							\
											BODYCON_DOOR_OPENING_MASK)

// 워프 바디.
#define BODYCON_WARP_STATE					(BODYCON_LANDED_MASK|BODYCON_DEAD_MASK)

template<typename var_t, typename mask_t>
void SET_CLIENT_BODYCON_BIT(var_t VAR, mask_t MASK)
{
	CLEAR_BODYCON_BIT(VAR, (BODYCON_EX_STATE_CLEAR_MASK|BODYCON_BOOSTER_EX_STATE_CLEAR_MASK));
	VAR |= MASK;
}

// IME 관련 설정 (후보리스트 보이는 위치)
#define SET_MAIN_CHAT_CANDIDATE				0			// 채팅 부분 
#define SET_WHISPER_CHAT_CANDIDATE			1			// 귓말창 부분 
#define SET_MESSAGEBOX_CANDIDATE			2			// 메세지 박스 부분
#define SET_FAQ_CANDIDATE					3			// FAQ 부분
#define SET_CREATEMENU_CANDIDATE			4			// 생성창 부분
#define SET_DELETEMENU_CANDIDATE			5			// 캐릭터 지우기

#define CHAR_INF_CANDIDATE					10			// 후보창 리스트는 최대 10개로 한다






// 맵 타입 넘버
#define MAP_TYPE_TUTORIAL		0
#define MAP_TYPE_CITY			1		// 1000 ~
//
#define MAP_TYPE_NORMAL_FIELD	3		// 3000 ~
#define MAP_TYPE_SPECIAL_FIELD	4		// 4000 ~
#define MAP_TYPE_QUEST			5		// 5000 ~







#define BOOSTER_TYPE_BACK		-2//후진 부스터
#define BOOSTER_TYPE_AIRBREAK	-1//공중 부양
#define BOOSTER_TYPE_STOP		0//정지
#define BOOSTER_TYPE_MIN		1//기본 부스터
#define BOOSTER_TYPE_MAX		2//가속 부스터

#define HYPER_BOOSTER_NORMAL	-1// 하이퍼 부스터 사용 안함
#define HYPER_BOOSTER_MAX		1 // 하이퍼 부스터 사용


// 2005-02-21 by jschoi - 웹 관련
// 2009-02-24 by bhsohn 캐쉬샵 충전 웹페이지 추가
// #define WEB_WIDTH		742		// 720
// #define WEB_HEIGHT		344		// 338
#define WEB_WIDTH		660		// 720
#define WEB_HEIGHT		550		// 338// 2009-03-03 by bhsohn 일본웹페이지 크기 변경
// end 2009-02-24 by bhsohn 캐쉬샵 충전 웹페이지 추가
#define WEB_START_X		((g_pD3dApp->GetBackBufferDesc().Width-WEB_WIDTH)/2)
#define WEB_START_Y		((g_pD3dApp->GetBackBufferDesc().Height-WEB_HEIGHT)/2)



// 2005-03-03 by jschoi - RequestEnable
#define REQUEST_ENABLE_INIT_TIME	-4.0f//-1.0f	// 2006-09-22 by ispark, 응답을 기다리는 타임(화면에 찍을 준비 타임)
#define REQUEST_ENABLE_TIME			1.0f			// 2006-08-11 by ispark
#define REQUEST_ENABLE_BOX_WIDTH	255
#define REQUEST_ENABLE_BOX_HEIGHT	35


// 2005-03-08 by jschoi  - Check Gate Distance
#define CHECK_WARP_GATE_DISTANCE	500.0f


// 2005-03-25 by jschoi - 타겟팅 DEFINE
#define OLD_TARGETTING	0
#define NEW_TARGETTING	1
#define OLD2_TARGETTING	2

// // 2005-04-19 by jschoi - TUTORIAL
enum{NONEBOX,SKIPBOX,IMAGEBOX,IMAGEVIEW};
enum{ALPHA_WAVE,ALPHA_DELAY_WAVE,ALPHA_FADE_IN,ALPHA_FADE_OUT};

// 2005-07-07 by ydkim Mission 관련
#define MISSION_RUN_SUCCESS		0	// 미션 명령 실행 성공
#define MISSION_RUN_RENDER_ERR	1	// 미션창 실행 상태가 아니다
#define MISSION_RUN_ERROR		2	// 기타 에러 관련

// 2005-07-07 by ispark
// 롤링 관련 정의문
#define ROLLING_USE_TIME	2.0f

// 미션 메세지 보내기( 이미지 렌더링 후 )
#define SENDMESSAGE_MISSION_COMPLETE		1
#define SENDMESSAGE_MISSION_CANCER			2
#define SET_MISSION_RESAULT_IMG_X			((g_pD3dApp->GetBackBufferDesc().Width/2)-142)
#define SET_MISSION_RESAULT_IMG_Y			((g_pD3dApp->GetBackBufferDesc().Height/3)-24)

// 2005-07-27 by ispark
// Picking 오브젝트
#define PICKING_OBJECT_NUMBER		1100015

// 캐릭터 바디 컨디션
#define BODYCON_HUMAN_ANIMATION_TIME	(BODYCON_CHARACTER_MODE_STOP|BODYCON_CHARACTER_MODE_WALK|BODYCON_CHARACTER_MODE_RUN)

// 캐릭터 관련 정의문
#define CHARACTER_SCALE				1.5f			// 캐릭터 스케일
#define CHARACTER_SHADOW_SCALE		6.0f			// 캐릭터 그림자 스케일(대각선 반지름 크기이다.

// 2005-08-08 by ydkim 글러벌 이미지 관련 정의
typedef enum 
{
	MISSION_SUC				=					0,		// 미션 성공
	MISSION_FAI				=					1,		// 미션 실패
	MISSION_OUT				=					2,
	OUT_ZONE				=					3,

	GLOBAL_IMAGE_MAX_COUNT	=					4		// 최대 이미지 수
} IMAGE_LIST;

#define ICON_EXP_ITEMNUM						 7011210	//       39     아이콘_경험치
#define ICON_LEV_ITEMNUM						 7011220	//       39     아이콘_레벨
#define ICON_BUN_ITEMNUM						 7011230	//       39     아이콘_보너스스텟
#define ICON_PRO_ITEMNUM						 7011240	//       39     아이콘_명성

#define CHARACTER_CAMERA_DISTANCE_MIN	10.0f	// 최소 거리
#define CHARACTER_CAMERA_DISTANCE_MAX	100.0f	// 최대 거리

//enum{ VOICE_NONE,VOICE_ONE, VOICE_PARTY, VOICE_GUILD };			// 음성 채팅 종류

// 2005-10-07 by ispark
// 튜토리얼 리스트
enum { L0, L1,L2, L3, L4, L5, L6, L7, L8, L9,L10};


// 2005-10-27 ydkim 윈도우 창 우선순위관련 처리
// 명칭 지정은 함수 명칭 이름앞에 WND를 붙이는 것으로 한다
// ex)WND********
enum WNDWINDOWSINDEX
{
	WNDGameGlobalImage,		// 글로벌 이미지
	WNDInfWindow,			// Infwindow
	WNDGameMain,			// GameMain
	WNDSysWnd,				// 시스템 창
	WNDBazaarShop,			// 개인상점
	WNDCityBase,			// 도시
	WNDMissionInfo,			// 미션 처리
	WNDMiniMap,				// 미니맵 처리
	WNDMp3Player,			// MP3 플레이어
	WNDInfSkill,			// 스킬 인포
	WNDRenderHelp,			// Help
	WNDGameMainFaq,			// FAQ
	//WNDCommunityVOIP,		// VOIP
	WNDQSlotIconFlag,		// QSlot
	WNDItemInfo,			// 아이템 인포
	WNDMenuList,			// 메뉴리스트
	WNDMainChatStartMenu,	// 메인챗 메뉴리스트
	WNDLeftRightWindow,		// 인벤, 캐릭, 파티 등
	WNDOpJoystick,				// 조이스틱 옵션창 // 2008-11-13 by bhsohn 조이스틱 작업
	WNDCommunityWnd,				// 커뮤니터 창	// 2008-04-04 by bhsohn Ep3 커뮤니티 창
	WNDInvenWnd,				// 인벤 창	// 2008-08-22 by bhsohn EP3 인벤토리 처리
	WNDCharacterWnd,			// 캐릭터 창	// 2008-09-22 by bhsohn EP3 캐릭터 창
	//WNDVoiceWnd,				// 음성
	WNDMacroWnd,				// 매크로 창	
	// 2009. 01. 12 by ckPark 선전 포고 시스템
	WNDWarDecalre,				// 선전포고 포기창
	// end 2009. 01. 12 by ckPark 선전 포고 시스템
	WNDWorldRank,				// 월드 랭킹 창 // 2009-02-13 by bhsohn 월드 랭킹 시스템
	WNDItemMix,				// 인게임조합식	// // 2013-02-20 by bhsohn 인게임 조합창 처리
	WNDChangeCharacter,		// 케릭터 변경카드 // 2013-04-05 by bhsohn 케릭터 변경 창 윈도우 순서 정렬 처리
	WNDArmorCollection,				// 아머 컬렉션	// 2013-05-28 by bhsohn 아머 컬렉션 시스템
	WNDIndexEnd				// 행열의 마지막
};

#define DOUBLEKEY_TIME			0.24f		// 더블키 체크용 시간
#define SHUTTLE_ROTATION_DEFAULT_X		-0.65f	// 인벤토리 셔틀의 기본 방향
#define SHUTTLE_ROTATION_DEFAULT_Z		-0.55f	// 인벤토리 셔틀의 기본 방향

#define ENEMY_WEAPON_INDEX_DUMMY		10	// 2012-06-25 by isshin 아템미리보기 - 자신의 웨폰 인덱스와 구분 짓기 위한 더미값

//////////////////////////////////////////////////////////////////////////

#define INVEN_RESOLUTION_1				800
#define INVEN_RESOLUTION_2				1024
#define INVEN_RESOLUTION_3				1280
#define INVEN_RESOLUTION_3_1			1440			// 2008-02-11 by dgwoo 해상도 추가.(1440 * 900)
#define INVEN_RESOLUTION_4				1600
// 2009. 10. 19 by ckPark 해상도 추가(1680x1050, 1920x1080, 1920x1200)
#define INVEN_RESOLUTION_5				1680
#define	INVEN_RESOLUTION_6				1920
// end 2009. 10. 19 by ckPark 해상도 추가(1680x1050, 1920x1080, 1920x1200)
#define INVEN_RESOLUTION_W				792
#define INVEN_RESOLUTION_WINDOW_1024	1018			// 2007-07-14 by dgwoo
#define INVEN_RESOLUTION_WINDOW_1280	1274			// 2007-07-14 by dgwoo
#define INVEN_RESOLUTION_WINDOW_1600	1594			// 2007-07-14 by dgwoo

// 2007-12-26 by dgwoo 해상도의 Height값.
#define RESOLUTION_FULL_H_720				720
#define RESOLUTION_FULL_H_768				768
#define RESOLUTION_FULL_H_800				800
#define RESOLUTION_FULL_H_900				900
#define RESOLUTION_FULL_H_960				960
// 2009. 10. 19 by ckPark 해상도 추가(1680x1050, 1920x1080, 1920x1200)
#define RESOLUTION_FULL_H_1050				1050
#define	RESOLUTION_FULL_H_1080				1080
// end 2009. 10. 19 by ckPark 해상도 추가(1680x1050, 1920x1080, 1920x1200)
#define RESOLUTION_FULL_H_1024				1024
#define RESOLUTION_FULL_H_1200				1200



// 2006-01-16 by ispark
// 무기 포지션 인덱스
#define WEAPON_POSITION_1_1_1			1
#define WEAPON_POSITION_1_1_2			2
#define WEAPON_POSITION_2_1_1			3
#define WEAPON_POSITION_2_1_2			4
#define ENGINE_POSITION					5


// 2006-03-16 by ispark
// 입력 언어
enum
{
	INPUT_LANGUAGE_KOR = 1,
	INPUT_LANGUAGE_ENG,
	INPUT_LANGUAGE_VIET_TELEX,
	INPUT_LANGUAGE_VIET_VNI,
	INPUT_LANGUAGE_CHINA,
	INPUT_LANGUAGE_END
};

enum : char
{
	RED_FONT = 'r',
	GREEN_FONT = 'g',
	BLUE_FONT = 'b',
	YELLOW_FONT = 'y',
	CYAN_FONT = 'c',
	MAGENTA_FONT = 'm',
	WHITE_FONT = 'w',
	ENCHANT_FONT = 'e',
	GRAY_FONT = 'q',
	DARKBLUE_FONT = 'a',
	//2015-03-02 by St0rmy added new colors
	OLIVE_FONT = 'o',
	PINK_FONT = 'p',
	LIGHTBLUE_FONT = 'l',
	DARKGREY_FONT = 'd',
	BLACK_FONT = 'z',
};

constexpr D3DCOLOR HFONT_ARGB(BYTE a, BYTE r, BYTE g, BYTE b)
{
	return D3DCOLOR(((a & 0xff) << 24) | ((b & 0xff) << 16) | ((g & 0xff) << 8) | (r & 0xff));
}
    
enum { SPECIAL_STRING = 1 };

// 2006-07-26 by ispark, 아이템 위치
// 2013-04-08 by bhsohn 인게임 조합창 추가 구현
//enum {ITEM_NOT_POS, ITEM_INVEN_POS, ITEM_SKILL_POS, ITEM_QSLOT_POS, ITEM_STORE_POS, ITEM_LAB_POS, ITEM_BAZAAR_POS};
enum { ITEM_NOT_POS, ITEM_INVEN_POS, ITEM_SKILL_POS, ITEM_QSLOT_POS, ITEM_STORE_POS, ITEM_LAB_POS, ITEM_BAZAAR_POS, ITEM_MIXWND_POS };

#define GAME_SHUTDOWN_TIME					30.0f
#define SIZE_MAX_BAZAAR_FULL_NAME			42

// 2006-08-22 by ispark, 저레벨 도움말 박스 
enum { LOW_LEVEL_STAT_HELP, LOW_LEVEL_MISSION_HELP };
#define HELP_SPECIAL_COUNT			2

// 2006-09-07 by ispark, 스테이지 이펙트 끝나고 처리 인덱스
// 2007-10-01 by bhsohn 처음 튜토리얼 종료시 프롤로그 안나오는 현상 처리
enum { NOT_ACTION, TUTORIAL_SKIP, TUTORIAL_COMPLETE, MISSION_PRE, TUTORIAL_CHART_END };

// 2006-09-19 by ispark, 기어 상태 리스트
enum { DONOTKNOW = -1, FLIGHT, BREAKDOWN };

// 2006-11-21 by ispark, 스킬 처리에 따른 인덱스
enum { CL_SKILL_NONE, CL_SKILL_INVISIBLE, CL_SKILL_CAMOUFLAGE };
#ifdef EP1_CHARACTER
#define CHARACTER_MALE_COUNT		8
#define CHARACTER_FEMALE_COUNT		8
#else
#define CHARACTER_MALE_COUNT		7
#define CHARACTER_FEMALE_COUNT		7
#endif

// 2009. 11. 23 by jskim 리소스 로딩 구조 변경
enum
{
	_MOST_SIGNIFICANT_PRIORITY,
	_MY_CHARACTER_PRIORITY,
	_MESH_PRIORITY,
	_BULLET_PRIORITY,
	_NOTHING_PRIORITY,		// 단계별 시스템 사용 
	_NOTHING_STEP   // 기존 시스템 사용
};

enum
{
	_RESOURCE_LOADING_START,
	_RESOURCE_LOADING_STEP1,
	_RESOURCE_LOADING_STEP2,
	_RESOURCE_LOADING_STEP3,
	_RESOURCE_LOADING_STEP4,
	_RESOURCE_LOADING_STEP5,
	_RESOURCE_LOADING_STEP6,
	_RESOURCE_LOADING_END
};

enum
{
	_EFFECT_TYPE,
	_CHARACTER_TYPE,
	_OBJECT_TYPE,
	_MONSTER_TYPE,
	_UNIT_TYPE,
};
//end 2009. 11. 23 by jskim 리소스 로딩 구조 변경

// 2010-06-15 by shcho&hslee 펫시스템 - 펫 기본 좌표
#define PET_DEFULT_POS_X 18
#define PET_DEFULT_POS_Y 1.5
#define PET_DEFULT_POS_Z 8

// 2011-03-21 by jhAhn 마크시스템 마크 위치 생성
#define MARK_DEFULT_POS_X 0
#define MARK_DEFULT_POS_Y 0
#define MARK_DEFULT_POS_Z 0
// end 2011-03-21 by jhAhn 마크시스템 마크 위치 생성

#define PET_CITY_DEFULT_POS_X 6
#define PET_CITY_DEFULT_POS_Y 15
#define PET_CITY_DEFULT_POS_Z 0

#define PET_CITY_GAP_POS_X	  2	 
#define PET_CITY_GAP_POS_Y	  5

enum 
{
	OP_PET = 1,		
};

// 2010. 10. 05 by jskim 맵로딩 구조 변경
enum
{
	_MAP_LOADING_START,
	_MAP_LOADING_STEP1,
	_MAP_LOADING_STEP2,
	_MAP_LOADING_STEP3,
	_MAP_LOADING_STEP4,
	_MAP_LOADING_STEP5,
	_MAP_LOADING_END
};
// end 2010. 10. 05 by jskim 맵로딩 구조 변경
enum
{
	GROUP_TYPE,
	IMAGE_TYPE,
	GR_IMGAE_TYPE,
	CONTROL_TYPE
};

enum
{
	UVANI_TYPE,
	SCALE_TYPE
};

enum
{
	UP_TYPE,
	CLICK_TYPE,
	OVER_TYPE,
	DISABLE_TYPE,
	ALL_TYPE
};

enum
{
	LEFT_TOP_LINEUP,
	LEFT_BOTTOM_LINEUP,
	RIGHT_TOP_LINEUP,
	RIGHT_BOTTOM_LINEUP,
	CENTER_TOP_LINEUP,
	CENTER_BOTTOM_LINEUP
};
#endif // COMMON_ATUMDEFINE_H_
