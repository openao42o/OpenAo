/*******************************************************************************************
  ATUM Character, Monster, Object, Texture define

  예> 
      RC_C_D_BT01				13001200
        -RC : Resource			 -13 : type(캐릭터(01),몬스터(02),오브젝트(03),GUI(04), TileTex(05) ,Effect(06)등)
		-C : Character			 -0012 : object 종류
		-D : Deca				 -00 : X파일의 Texture파일(-ff : X파일의 Shadow파일)
							
		-BT01 : 종류

	  RC_G_INIT_BACKGROUND
		-RC : Resource
		-G : GUI
		-INIT : Init state
		-BACKGROUND : 세부 종류

  작성자 : 권동혁
  작성일 : 2002.10.30

  작성자 : 이진하
  작성일 : 2003.07.30

********************************************************************************************/


#ifndef __ATUM_OBJECTDEFINE_H__
#define __ATUM_OBJECTDEFINE_H__

#include <array>

struct SPRITEVERTEX
{
    D3DXVECTOR3 p;
    DWORD       color;
    FLOAT       tu, tv;
};
// FVF 플래그
#define D3DFVF_SPRITEVERTEX (D3DFVF_XYZ|D3DFVF_DIFFUSE|D3DFVF_TEX1)

// Map
#define RC_MAP_WORKSPACE		"ms.wok"

// Effect
#define RC_EFF_COLL_01					1100001
#define RC_EFF_GROUND_01				1100002
#define RC_EFF_GROUND_02				1100003
#define RC_EFF_HEAL_01					1100004
#define RC_EFF_LEVELUP_DT				1100005
#define RC_EFF_LEVELUP_BT				1100006
#define RC_EFF_LEVELUP_OT				1100007
#define RC_EFF_LEVELUP_ST				1100008
#define RC_EFF_CLOUD					1100009	
#define RC_EFF_WARP_ZONE				1100010	
#define RC_EFF_OTHER_WARP				1100011	
#define RC_EFF_BOOSTER					1100012	
#define RC_EFF_SELECT					1100013
//#define RC_EFF_HELP00					1100013	//	조작법
//#define RC_EFF_HELP01					1100014	//	착륙
#define RC_EFF_HELP02					1100015	//	공격
#define RC_EFF_HELP03					1100016	//	상점
#define RC_EFF_HELP04					1100017	//	물건사고 팔기
#define RC_EFF_HELP05					1100018	//	기능키

#define RC_EFF_USEITEM_REPAIR_HP		1100022	//	아이템 수리 이펙트	체력(일반 아이템 - A~일반)
#define RC_EFF_USEITEM_REPAIR_DP		1100073	//	아이템 수리 이펙트	쉴드(일반 아이템 - A~일반)
#define RC_EFF_USEITEM_REPAIR_HPS		1100056	//	아이템 수리 이펙트	체력(일반 아이템 - S)
#define RC_EFF_USEITEM_REPAIR_HPA		1100058	//	아이템 수리 이펙트	체력(일반 아이템 - A+)
#define RC_EFF_USEITEM_REPAIR_DPS		1100057	//	아이템 수리 이펙트	쉴드(일반 아이템 - S)
#define RC_EFF_USEITEM_REPAIR_DPA		1100059	//	아이템 수리 이펙트	쉴드(일반 아이템 - A+)
#define RC_EFF_USEITEM_REPAIR_SPS		7002840	//	아이템 수리 이펙트	스킬(일반 아이템 - A)
#define RC_EFF_USEITEM_REPAIR_SPA		7002850	//	아이템 수리 이펙트	스킬(일반 아이템 - B)
#define RC_EFF_USEITEM_REPAIR_SP		7002860	//	아이템 수리 이펙트	스킬(일반 아이템 - C)

#define RC_EFF_USEITEM_REPAIR_EP		1100055 //  아이템 수리 이펙트
#define RC_EFF_BGEAR_TRACE				1100074	//	B기어 잔상 이펙트
#define RC_EFF_IGEAR_TRACE				1100075	//	I기어 잔상 이펙트
#define RC_EFF_MGEAR_TRACE				1100076	//	M기어 잔상 이펙트
#define RC_EFF_AGEAR_TRACE				1100077	//	A기어 잔상 이펙트
#define RC_EFF_DUMMY_BACK_POSITION		"00000000.eff"	//	좌표만 가진 더미 뒷좌표이펙트(A기어 1형무기만 가짐)
#define RC_EFF_DUMMY_POSITION			"00000001.eff"	//	좌표만 가진 더미 이펙트 
#define RC_EFF_LANDING_TAKEOFF			1100014	//	이착륙 이펙트
// 1형
#define RC_EFF_LOCKON_AUTOMATIC			1100056
#define RC_EFF_LOCKON_VULCAN			1100020	
#define RC_EFF_LOCKON_GRENADE			1100057
#define RC_EFF_LOCKOND_CANNON			1100058
#define RC_EFF_LOCKON_RIFLE				1100059
#define RC_EFF_LOCKON_GATLING			1100060
#define RC_EFF_LOCKON_LAUNCHER			1100061
#define RC_EFF_LOCKON_MASSDRIVE			1100062
// 2형
#define RC_EFF_LOCKON_ROCKET			1100063
#define RC_EFF_LOCKON_MISSILE			1100021	
#define RC_EFF_LOCKON_BUNDLE			1100064
#define RC_EFF_LOCKON_MINE				1100065
#define RC_EFF_LOCKON_SHIELD			1100066
#define RC_EFF_LOCKON_DUMMY				1100067
#define RC_EFF_LOCKON_FIXER				1100068
#define RC_EFF_LOCKON_DECOY				1100069


#define RC_EFF_SNOW						1100040	//	눈

#define RC_EFF_IGEAR_UPGRADE			1100042 //	- I-Gear Upgrade
#define RC_EFF_AGEAR_UPGRADE			1100043 //	- A-Gear Upgrade
#define RC_EFF_MGEAR_UPGRADE			1100044 //	- M-Gear Upgrade
#define RC_EFF_BGEAR_UPGRADE			1100045 //	- B-Gear Upgrade

#define RC_EFF_TARGET_ARROW				1100070	//	타겟 방향 화살표
//ysw 9_19
#define RC_EFF_GAGE_BOX					1100019
#define RC_EFF_INTRO					1100023

#define	RC_EFF_UNIT_ARROW				1100026 
#define RC_EFF_FIRST_TARGET				1100028
#define	RC_EFF_SECOND_TARGET			1100027 

#define RC_EFF_SPD_NUM					1100029
#define RC_EFF_CURRENTBURN_NUM			1100030
#define RC_EFF_TOTALBURN_NUM			1100031
#define RC_EFF_CURRENTHP_NUM			1100032
#define RC_EFF_TOTALHP_NUM				1100033
#define RC_EFF_CURRENTUTC_NUM			1100034
#define RC_EFF_TOTALUTC_NUM				1100035
#define RC_EFF_CURRENTFUEL_NUM			1100036
#define RC_EFF_TOTALFUEL_NUM			1100037
#define RC_EFF_EXP_NUM					1100038
#define RC_EFF_EXP_MIDDLE_GAGE			1100039

#define RC_EFF_SUN						1100047
#define RC_EFF_SHIELD_HIT				1100050	// 쉴드 데미지 시
#define RC_EFF_SHIELD_HIT2				7900790 // 쉴드 데미지 시

#define RC_EFF_QUEST_MAP_POSITION		01100079

#define RC_EFF_AGEAR_FIRE				1100072	// 임시A-Gear 용 발사 화이어 이펙트
#define RC_EFF_GET_SKILL				1100078

#define RC_EFF_CHAFF_SKILL				1100080		//체프 스킬 이펙트.
#define RC_EFF_SCAN_SKILL				1100081		//스캔 스킬 이펙트.
// 2007-04-19 by bhsohn 서치아이 아이템 추가
#define RC_EFF_SCAN_ITEM				1100082		//스캔 스킬 이펙트.
#define RC_EFF_SCAN_ITEM2				1100086		//스캔 스킬 이펙트.
// 2008-12-26 by bhsohn 럭키머신 이펙트 추가
#define RC_EFF_LUCKY_SUCCESS			1100083		//럭키머신 이펙트
// end 2008-12-26 by bhsohn 럭키머신 이펙트 추가

// 2009. 07. 07 by ckPark 로봇기어 요청사항(롤링, 선택화면, 무기, A기어포대)
#define RC_EFF_ROBOTAMOR_FIRE_EFFECT	1100084		// 로봇아머용 1형 발사 이펙트
// end 2009. 07. 07 by ckPark 로봇기어 요청사항(롤링, 선택화면, 무기, A기어포대)

#define RC_EFF_BOOSTER_00000		"06010000"

// 2006-01-12 by ispark, 1형, 2형 무기 모듈
#define RC_EFF_PRIMARY_RIGHT_POSITION	"00000000.eff"
#define RC_EFF_PRIMARY_LEFT_POSITION	"00000001.eff"
#define RC_EFF_SECONDARY_RIGHT_POSITION	"00000002.eff"
#define RC_EFF_SECONDARY_LEFT_POSITION	"00000003.eff"

// 2010-06-15 by shcho&hslee 펫시스템 - 펫 무기 생성
#define RC_EFF_PET_RIGHT_POSITION	"00000000.eff"
#define RC_EFF_PET_LEFT_POSITION	"00000001.eff"
// end 2010-06-15 by shcho&hslee 펫시스템 - 펫 무기 생성

#define RC_EFF_PET_LEVEL_CHANGE		1100085				 //2011-10-06 by jhahn 파트너 성장형 시스템
// 2011-03-21 by jhAhn 마크시스템 마크 위치 생성
#define RC_EFF_MARK_POSITION		"00000004.eff"
// end 2011-03-21 by jhAhn 마크시스템 마크 위치 생성

// 2012-10-13 by jhjang 해피 아워 경험치 보너스 시스템 리뉴얼
#define RC_EFF_KILL_COUNT_MAX		1100087
// end 2012-10-13 by jhjang 해피 아워 경험치 보너스 시스템 리뉴얼

// 2006-10-11 by ispark, 캡슐 이펙트
#define RC_EFF_POWER_ENERGY_CAPSULE		7800110
#define RC_EFF_POWER_SHIELD_CAPSULE		7810100
#define RC_EFF_POWER_SP_CAPSULE			7813010
#define RC_EFF_ADRENALIN_CAPSULE		7812020
#define RC_EFF_BRAVE_CAPSULE			7831030
#define RC_EFF_HYPER_BOOSTER_CAPSULE	7813030

// Texture
#define RC_TEX_GUI_LOADING_BACK		"back"
#define RC_TEX_GUI_LOADING_BAR		"bar"

// 2012-11-29 by mspark, 서치아이 위치가 이동하는 캐릭터의 위치와 동일하도록 수정
#define RC_EFF_SEARCHEYE_SHUTTLE_POSITION	1100088
#define RC_EFF_SEARCHEYE_SHUTTLE_POSITION2	1100089

#define RC_ITEM_SEARCHEYE_NUM				7004680
#define RC_ITEM_SEARCHEYE2_NUM				7037070
// end 2012-11-29 by mspark, 서치아이 위치가 이동하는 캐릭터의 위치와 동일하도록 수정

#define AGEAR_SKILL_BASENUM_BARIAL		    	7821060	// 시간  	- 베리어	// 2012-12-17 by bhsohn A기어 배리어 이펙트 워프후 사라지는 오류 처리

#define FILTER_ENCODESTRING "ud830pwkldlkv[]f\20jdmvld;"

//채팅 Color	B|G|R
#define COLOR_CHARACTER_ID00		0xFFFFFF				// white
#define COLOR_CHARACTER_ID01		0x0000FF				// red
#define COLOR_CHARACTER_ID02		0x00FF00				// green
#define COLOR_MONEY					0x66CCFF				// light orange
#define COLOR_CHAT_ALL				0x00FF99				// light green
#define COLOR_CHAT_REGION			0xFEFEFE				// white
#define COLOR_CHAT_MAP				0x66FF33				// light green
#define COLOR_CHAT_PTOP				0xCC99CC				// greyish pink
#define COLOR_CHAT_GUILD			0xFF00FF				// magenta
#define COLOR_CHAT_CASH				0x000099				// dark red
#define COLOR_CHAT_CHATROOM			0xCCCCFF				// red-grey
#define COLOR_CHAT_WAR				0X0000CC				// was 0X2222EA, red
#define COLOR_CHAT_CNC				0x999933				// dark cyan				
#define COLOR_CHAT_SELL				0x0099FF				// orange

#define COLOR_MONSTER_ID00			0xFF0000				// blue
#define COLOR_MONSTER_ID01			0xFF9899				// light blue
#define COLOR_MONSTER_ID02			0xFDFDFD				// white
#define COLOR_MONSTER_ID03			0x9999FF				// light red
#define COLOR_MONSTER_ID04			0x0000FF				// red
#define COLOR_HELP					0x66FFFF				// light yellow
#define COLOR_RED					0x0000FF				// red
#define COLOR_SKILL_USE				0x00FF00				// green
#define COLOR_SKILL_CANCEL			0xCCCCCC				// grey
#define COLOR_ARENA					0xFF9999				// light blue
#define COLOR_PARTY					0x99FF99				// light green
#define COLOR_TRADE					0x99CC33				// slightly dark cyan
#define COLOR_ITEM					0x00FFFF				// yellow
#define COLOR_INFLUENCE				0xFFFF00				// cyan

#define COLOR_SYSTEM				0xFFCCFF				// light pink
#define COLOR_ERROR					0x00FFFF				// yellow
#define COLOR_DEBUG					0x00FFFF				// yellow

#define BOOSTER_NORMAL_SPEEDRATE	1.0f
#define BOOSTER_FAST_SPEEDRATE		2.0f
#define BOOSTER_KEY_CHECK_TIME		0.2f
#define BOOSTER_START_CHECK_RATE	0.9f
#define BOOSTER_END_CHECK_RATE		0.0f


// TODO: reference additional headers your program requires here
enum ERRER_TYPE { 
	APP_ERR_MAPLOAD,
	CLIENT_ERR_MESHLOAD,
	CLIENT_ERR_TEXINIT, 
	CLIENT_ERR_SHUTTLE_MESHLORD,
	CLIENT_ERR_SHUTTLE_EFFECTINIT,
	CLIENT_ERR_SHUTTLE_EFFECTRESTORE,
	CLIENT_ERR_WEAPON_TEXINIT,
	CLIENT_ERR_WEAPON_VBRESTORE
};
enum State { 
	_INITIALIZING=-1, 
	_NORMAL,
	_WARP, 
	_FALLING,						//추락되고있는 상태.
	_FALLEN,						//추락된상태.
	_LANDING,
	_LANDED,
	_TAKINGOFF,
	_EXPLODING,						//폭발중.
	_EXPLODED,						//폭발
	_CLOSE,
	_NCITYIN,
	_STAND,
	_WALK,
	_RUN,
	_FUELALLIN,						//연료가 없어서 추락하는 상태.
	_AUTODESTROYED
};

enum GameState { 
	_MOVIE,
	_LOGO,
	_INIT,
	_SELECT,
	_CREATE,
	_OPTION,
	_MAPLOAD,
	_GAME,
	_SHOP,
	_CITY, // 도시
	_SCRIPT, // 성우 스크립트
	_WAITING,
	_GAMEOVER,
	_COMPLETE, 
	_PROLOGUE , 
	_ENDGAME, 
	_ENDCLIENT,
	_SELECTCHANNEL, 
	_BOSSMONSTERSUMMONREADY,
	_BOSSMONSTERSUMMON,
	_SERVER_DOWN,			// 2013-07-05 by bhsohn 서버종료시, 경고음 시스템
	_MOVIEPLAYER
};
enum PartType {	_ADMIN,_SHUTTLE, _ENEMY, _MONSTER,				// Shuttle 및 Enemy, Monster
				_ITEMFIELD,										// 필드에 떨어진 아이템
//				_CYCLOPSE,_ARROW,_HAMMERHEAD,					// Missile 류 ( Shuttle )
				_MISSILE,										// Missile 류 ( Shuttle )
//				_FATBIRD,_BALLISTAR,_MOSQUITO,					// Rocket 류 ( Shuttle )
				_ROCKET,										// Rocket 류 ( Shuttle )
				_BUNDLE,										// Bundle 류 ( Shuttle )
				_MINE, //_SPOREMINE,_AXISMINE,_OCTOPUSMINE,		// Mine 류 ( Shuttle )
				_SHIELD,										// Shield 류 (Shuttle )
				_DUMMY,											// Dummy 류
				_FIXER,											// Fixer 류
				_LAY,											// 고정형 아이템 (Shuttle : 스캔오브젝트)
				_WMHONIAN, _WMAMMON, _WMNATING,_WMMONSTERMISSILE,_WMWATCHER,_WMFLYSEED,_WMOVERWATCHER,_WMSCOUT,_WMSPOTTIE,_WMTETINO,_WMBALDRE,
				_WMFRIGG,_WMTHOR,_WMFENRIR,_WMLUCANUS,_WMLOKI,_WMCURSESTORM,
				_FASTDATA,_FIREDATA
};
// Party Type
enum PARTY_TYPE { _NOPARTY,_PARTYMASTER,_PARTYMEMBER,_PARTYOTHERMAPMASTER,_PARTYOTHERMAPMEMBER,_PARTYRESTARTMEMBER};//,_ATTACKENEMY };
// _NOPARTY:파티가 아님,_PARTYMASTER:파티장,_PARTYMEMBER:파티원,_ATTACKENEMY:결투 상대

//#define DIRECTINPUT_VERSION         0x0800

enum ATTACK_MODE{_AIR,_GROUND,_SIEGE,_AIRBOMBING};



// Enemy Type
#define ENEMYDATA_ENEMYLIST					0x00
#define ENEMYDATA_TARGETLIST				0x01
#define ENEMYDATA_TARGET					0x02
// Monster Type 
#define MONSTERDATA_MONSTERLIST				0x03
#define MONSTERDATA_MONSTERTARGETLIST		0x04
#define MONSTERDATA_MONSTERTARGET			0x05

// Chat
#define WM_IM_ASYNC		(WM_USER_BASE + 770)
#define WM_IM_NOTIFY	(WM_USER_BASE + 771)

//#define REQUEST_LOCKON_TIME			1.0f

#define TILE_SIZE							40.0f

#define MAP_BLOCK_SIZE						640
#define SIZE_OBJECT_VISIBLERECT				(MAP_BLOCK_SIZE*3)
#define CHECK_OBJECT_RADIUS					320

struct TWO_BLOCK_INDEXES
{
	short sMinX;
	short sMinZ;
	short sMaxX;
	short sMaxZ;
};


#define	TEXTILE_NUM							500
#define SHUTTLE_NUM							16*4
#define MONSTER_NUM							100
//#define OBJECT_FRONT_NUM					120
//#define OBJECT_BACK_NUM						38
//#define OBJECT_NUM							(OBJECT_FRONT_NUM + OBJECT_BACK_NUM + 100)
#define OBJECT_NUM							300

#define SHUTTLE_TARGET_MOUSE_CIRCLE_RADIUS	20.0f
#define SHUTTLE_ATTACK_CENTER_RADIUS		30.0f
#define SHUTTLE_MAX_RENDER_DISTANCE			1020.0f			// 
#define SHUTTLE_MAX_UPVEC_RATE				0.8f			//	Up Vector 의 기울임 최대 값
#define SHUTTLE_MAX_SIDERATE				1.5f			//	좌우 회전 최대 값
#define SHUTTLE_MAX_UPDOWNRATE				1.9f			//	아래위 기울임 최대 값
#define SHUTTLE_LRUPRECOVERY_RATE			1.5f			//	좌우 Up Vec의 기울임 회복 속도 조절
#define SHUTTLE_LRUPLEAN_RATE				1.0f			// 좌우 Up Vec의 기울임 속도 조절

#define SHADOW_MAX_DISTANCE					500.0f

//#define MAX_SHUTTLE_MOVE_TYPE				11
//#define MAX_CAMERA_TYPE						2

#define MAX_SOUND_ITEM						100
#define MAX_SOUND_GROUND_BIRD				10				//	1~~~000번대
#define MAX_SOUND_GROUND_WORM				10				//	1~~~100번대
#define MAX_SOUND_GROUND_ANIMAL				10				//	1~~~200번대
#define MAX_SOUND_GROUND_WATER	 			20				//	1~~~300번대
#define MAX_SOUND_GROUND_WIND				10				//	1~~~400번대
#define MAX_SOUND_GROUND_EXPLODE			5				//	1~~~500번대
#define MAX_SOUND_GROUND_MACHINE			5				//	1~~~600번대
#define MAX_SOUND_GROUND_HUMAN				10				//	1~~~700번대
#define MAX_SOUND_GROUND					(MAX_SOUND_GROUND_BIRD + MAX_SOUND_GROUND_WORM + MAX_SOUND_GROUND_ANIMAL + MAX_SOUND_GROUND_WATER + MAX_SOUND_GROUND_WIND + MAX_SOUND_GROUND_EXPLODE + MAX_SOUND_GROUND_MACHINE + MAX_SOUND_GROUND_HUMAN)
#define MAX_SOUND_GUI						100
#define MAX_SOUND_VOICE						100
#define MAX_SOUND_MONSTER					100
#define MAX_SOUND_TOTAL						(MAX_SOUND_ITEM + MAX_SOUND_GROUND + MAX_SOUND_GUI + MAX_SOUND_VOICE + MAX_SOUND_MONSTER)



#define MAX_CHAT_PTOP_CONT					10

#define MAX_TRADE_ITEM_NUMBER				12

#define MAX_SNOW_AMOUNT						1
#define SNOW_CHECKTIME						0.1f

#define MAX_PARTY_NUMBER			10


// by dhkwon, GAGE BOX BODYCONDITION
#define GAGEBOX_NORMAL			0x000000000000007E		// 게이지 박스,버튼들 노말
#define GAGEBOX_INFO_NORMAL		0x0000000000000004		// info button up
#define GAGEBOX_INVEN_NORMAL	0x0000000000000008		// inventory button up
#define GAGEBOX_PARTY_NORMAL	0x0000000000000010		// party button up
#define GAGEBOX_TRANS_NORMAL	0x0000000000000020		// trans button up
#define GAGEBOX_OPTION_NORMAL	0x0000000000000040		// option button up
#define GAGEBOX_INFO_UP			0x0000000000000080		// info button up
#define GAGEBOX_INVEN_UP		0x0000000000000100		// inventory button up
#define GAGEBOX_PARTY_UP		0x0000000000000200		// party button up
#define GAGEBOX_TRANS_UP		0x0000000000000400		// trans button up
#define GAGEBOX_OPTION_UP		0x0000000000000800		// option button up
#define GAGEBOX_INFO_DOWN		0x0000000000100000		// info button down
#define GAGEBOX_INVEN_DOWN		0x0000000000002000		// invetory button down
#define GAGEBOX_PARTY_DOWN		0x0000000000004000		// party button down
#define GAGEBOX_TRANS_DOWN		0x0000000000008000		// trans button down
#define GAGEBOX_OPTION_DOWN		0x0000000000010000		// option button down
#define GAGEBOX_AUTO_MODE_ON	0x0000000000200000		// auto mode
#define GAGEBOX_AUTO_MODE_OFF	0x0000000000400000
#define GAGEBOX_ACTIVE_MODE_ON	0x0000000000800000		// active mode
#define GAGEBOX_ACTIVE_MODE_OFF	0x0000000001000000

// messagebox
enum { _Q_PARTY_CREATE,				// 편대 생성
	   _Q_PARTY_INVITE,				// 편대에 합류하겠습니까?"
	   _Q_PARTY_BAN_MEMBER,			// 편대원 추방
	   _Q_PARTY_SECEDE,				// 편대 탈퇴
	   _Q_PARTY_LEAVE,				// 없음
	   _Q_PARTY_FORMATION,			// 없음
	   _Q_PARTY_TRANSFER_MASTER,	// 편대장 위임
	   _Q_PARTY_INVITE_USER,		// 누구를 편대에 초대하겠습니까?
	   _Q_PARTY_INVITE_USER_OK,		// 사용 안함
	   _Q_GUILD_CREATE,				// 비행단 생성
	   _Q_GUILD_MARK,				// 비행단 문장 파일이름 ?
	   _Q_GUILD_INVITE,				// 비행단 초대( C -> IM )
	   _Q_GUILD_INVITE_QUESTION,	// 비행단 초대 물어봄( IM -> C )
	   _Q_GUILD_BAN_MEMBER,			// 추방
	   _Q_GUILD_LEAVE,				// 탈퇴
	   _Q_GUILD_DISBAND,			// 비행단 해체 ?
	   _Q_GUILD_CANCEL,				// 비행단 해체 취소 ?
	   _Q_TRADE_INPUT_NAME,			// 거래할 유닛은 ?
	   _Q_TRADE_REQUEST,			// ~와 거래하시겠습니까?
	   _Q_TRADE_OK,					// 거래 완료하시겠습니까?
	   _Q_TRADE_CANCEL,				// 거래 취소 ?
	   _Q_WARF_OK,					
	   _Q_SHOP_SELL_ENERGY,			// 몇개를 파시겠습니까?
	   _Q_SHOP_SELL_ITEM,			// 아이템을 파시겠습니까?
	   _Q_AUCTION_INPUT_ITEM,		// 아이템을 경매에 올리시겠습니까?
	   _Q_AUCTION_DELETE_ITEM,		// 등록한 아이템을 삭제하시겠습니까?
	   _Q_ITEM_DELETE,				// 아이템 1개를 지우겠습니까?
	   _Q_ITEM_DELETE_NUM,			// 아이템 몇개를 지우겠습니까?
	   _Q_TRADE_ITEM_NUMBER,		// 몇개(얼마)를 올리겠습니까?
	   _Q_TRADE_COMPLETE_OK,		// 사용안함
	   _Q_VOIP_OK,					// ~와 음성교신을 하시겠습니까?
	   _Q_VOIP_NAME,				// 음성교신할 아이디를 입력하시요
	   _Q_DEAD_RETURN,				// 죽은 후 도시로 이동
	   _Q_PK_REQUEST,				// ~와 1:1 전투를 하시겠습니까?
	   _Q_PK_LOSE_REQUEST,			// 항복하시겠습니까?
	   _Q_PK_LOSE_REQUEST_OK,		// 항복을 받아들이겠습니까?
	   _Q_USE_ENCHANT_ITEM_CARD,	// 인챈트 아이템 더블클릭(인챈트할 아이템을 클릭하시오)
	   _Q_REQUEST_PARTY_BATTLE,		// 파티전투를 하시겠습니까?
	   _Q_SELECT_DELETE,			// 캐릭터를 지우시겠습니까?
	   _MESSAGE,					// 일반 메시지
	   _MESSAGE_ERROR_CREATE,		// 캐릭터 생성 시에 나타나는 메시지(존재하는 이름, 잘못된 이름, 이름을 입력하세요)
	   _MESSAGE_ERROR_NETWORK,		// 네트웍 접속 에러(기존 CGUITextBox대신 사용)
	   _Q_BATTLE_NAME,				// 1:1 대결할 유닛?
	   _Q_OUTDOOR_WARP,				// 외곽으로 워프?
	   _Q_LAB_ITEM_NUMBER,			// 몇개를 올리시겠습니까?
	   _Q_REQUEST_GUILD_WAR,		// 여단전를 하시겠습니까?
	   _Q_STORE_PUT_COUNTABLE_ITEM,	// 창고에 몇개 맡기시겠습니까?
	   _Q_INPUT_AUCTION_PRICE,		// 경매에 올릴 가격은 얼마로 하시겠습니까?
	   _Q_STORE_PUT_ITEM,			// 창고에 아이템을 맡기시겠습니까?
	   _Q_STORE_PUSH_ITEM,			// 창고에서 몇개의 아이템을 찾으시겠습니까?
	   _Q_QUEST_DISCARD,			// 미션 포기?
	   _Q_PARTYMASTER_VOIP,			// 편대간 통화 생성
	   _Q_JOIN_PARTY_VOIP,			// 편대간 통화 참여 ?
	   _Q_USE_HELP_SYSTEM,			// 초보 도우미 이용?
	   _Q_GUILDMASTER_VOIP,			// 여단 통화 생성
	   _Q_JOIN_GUILD_VOIP,			// 여단 통화 참여 ?
	   _Q_PUT_ITEM_SPI,				// 창고에 얼마를 맞기시겠습니까?
	   _Q_GET_ITEM_SPI,				// 창고에서 얼마를 찾겠습니까?
	   _Q_ADD_FRIEND,				// 친구추가
       _Q_DELETE_FRIEND,			// 친구를 삭제하시겠습니까?
       _Q_ADD_REJECT,				// 거부추가
       _Q_DELETE_REJECT,			// 거부목록에서 삭제하시겠습니까?
	   _Q_ITEM_DELETE_SKILL,		// 스킬을 삭제하시겠습니까?
	   _Q_GUILD_VOIP_END,			// 여단통화를 종료 하시겠습니까?
	   _Q_PARTY_VOIP_END,			// 파티통화를 종료 하시겠습니까?
	   _Q_VOIP_USING,				// 이미 음성통화를 사용하시고 있습니다.	   
	   _Q_TEX_SET,					// 세금 설정하기
	   _Q_DATE_WAR,					// 다음방어전 날짜 시간 설정
	   _Q_STORE_USE,				// 여단원 외 창고사용을 금지하시겠습니까?	   
	   _Q_GREATING_SET,				// 여단장 초기 인사말 설정
	   _Q_GET_TEX,					// 세금을 가져오시겠습니까?
	   _Q_CASHITEM_BUY,				// 캐쉬아이템을 구입하시겠습니까?
	   _Q_CASHITEM_SHOP_OPEN,		// 캐쉬상점을 사용하시겠습니까?
	   _Q_INPUT_SERIAL_NUM,			// 시리얼 넘버 입력
	   _Q_SUPER_RESTORE,			// 슈퍼 부활 카드
	   _Q_MEMBER_SUMMON,			// 여단원 한 명 소환 카드
	   _Q_USE_SUPER_RESTORE,		// 슈퍼 부활 카드를 사용하시겠습니까?
	   _Q_USE_NORMAL_RESTORE,		// 일반 부활 카드를 사용하시겠습니까?
	   _Q_MP3_DIRECTORY,			// MP3 디렉토리 입력
	   _Q_MP3_PLAYLIST_DEL,
       _Q_PART_STAT_RESET,			// 스텟 초기화(파트)
	   _Q_QUIT_MESSAGE,				// 종료 메세지 박스
	   _Q_USEITEM_NAME_CHANGE,
	   _Q_UNIT_STOP,				// 유닛 정지 하시겠습니까?
	   _Q_AGEAR_FUEL_ALLIN,			// 연료가 소모되어 도시로 워프됩니다.
	   _Q_MISSION_START,			// ~ 미션을 시작하시겠습니까?
	   _Q_VOIP_ONE_NAME,			// 통화 할 유저 아이디를 입력하세요.
	   _Q_VOIP_MENU,				// 메뉴를 선택하세요.
	   _Q_SKILL_CALLOFHERO,			// 콜오브히어로를 쓰셨습니다. 허락하시겠습니까?
	   _Q_FUELALLIN_DEAD_RETURN,	// 격납고로 이동합니다.
	   _Q_INFLUENCEMAP_DEAD_RETURN,	// 다른 세력맵에서 죽었을 때 자신의 세력의 도시맵으로 이동
	   _Q_EVENT_CALL_WARP_REQUEST,	// 이벤트맵 
	   _Q_BAZAAR_OPEN_SHOP_END,		// 상점(개인)을 닫으시겠습니까?
	   _Q_BAZAAR_INVALID_SHOP_END,	// 상점이 종료 되었습니다.
	   _Q_BAZAAR_SELL_OK,			// (방문자)아이템을 구매하시겠습니까?
	   _Q_BAZAAR_BUY_OK,			// (방문자)아이템을 판매하시겠습니까?
	   _Q_USEITEM_KILL_MARK,		// 킬마크를 EXP로 전화
	   _Q_QUEST_REQUEST_PARTY_WARP,	// 퀘스트 참여 확인 요청 메세지
	   _Q_SHOP_MULTI_SELL_ITEM,		// 아이템들을 파시겠습니까?// 2007-02-12 by bhsohn Item 다중 선택 처리
	   _Q_STORE_MULTI_PUT_ITEM,		// 아이템들을 맡기 겠습니까?// 2007-02-12 by bhsohn Item 다중 선택 처리
	   _Q_STORE_MULTI_GET_ITEM,	    // 선택된 모든 아이템을 가져오시겠습니까?.?// 2007-02-12 by bhsohn Item 다중 선택 처리
       _Q_UPDATE_SUBLEADER_1,		// %s를(을) 부지도자1로 선출하시겠습니까?
	   _Q_UPDATE_SUBLEADER_2,		// %s를(을) 부지도자1로 선출하시겠습니까?
	   _Q_ARENA_PASSWORD,			// 비밀번호를 입력하십시요.
	   _Q_ARENA_WARP,				// 아레나로 이동하시겠습니까?
	   _Q_ARENA_RESTART,			// 시작지점으로 이동합니다.
	   _Q_TUTORIAL_START,			// 튜토리얼 모드로 이동하시겠습니까?
	   _Q_USEITEM_SPEAKER_MSG,		// Message
	   _Q_INFLUENCE_WAR_EXPENCE_OK_MSG,		// 환급받을 금액을 입력하세요.
	   _Q_SECOND_PASS_CANCEL_MSG,		// 환급받을 금액을 입력하세요.
	   _Q_POLL_CANDIDATE_VOTE,			// %s 후보에게 투표하시겠습니까?
	   _Q_POLL_CANDIDATE_DELETE,			// "후보에서 탈퇴하시면, 다음 선거에 출마 가능합니다. 탈퇴하시겠습니까?"
	   _Q_GIFT_CHARACTER_TEXT,			// "아이템을 선물할 캐리터 명을 입력하세요."
	   _Q_GIFT_ITEM,						// "%s 아이템을 선물 하시겠습니까?"
	   _Q_GIFT_ITEM_RING_IN,				// %s님이 %s아이템 %d개를 선물 하였습니다. 창고에서 확인하세요.
	   _Q_COUPON,						// "쿠폰 관련 메시지.
	   // 2008-04-04 by bhsohn Ep3 커뮤니티 창
	   _Q_GUILD_GIVE_SUBLEADER,			// "\\y%s\\y님을 부여단장으로 임명하시겠습니까?"
	   _Q_GUILD_FALL_SUBLEADER,			// "부여단장 직위를 공석으로 비워두시겠습니까?"
	   _Q_GUILD_CHARGE_LEADER,			// "\\y%s\\y님에게 여단장을 위임하시겠습니까?"	   
	   _Q_GUILD_REQUEST,				// "\\y%s\\y여단에 가입 신청 하시겠습니까?"
	   _Q_GUILD_REQUEST_CANCEL,			// "\\y%s\\y여단에 가입 신청을 취소하시겠습니까?"
	   _Q_PARTY_JOIN_PASSWORD,			// 비밀번호를 입력하십시요.
	   // end 2008-04-04 by bhsohn Ep3 커뮤니티 창	   
	   _Q_CHATROOM_INVITE,				// "초대할 유닛을 입력하시오."
	   _Q_CHATROOM_INVITE_FROM,			// "%s님이 채팅에 초대하셨습니다."
	   _Q_CHATROOM_OUT,					//"채팅방에서 나가시겠습니까?"
	   _Q_CHATROOM_PASSWORD,			//"비밀번호를 입력해주세요."
	   // 2008-09-17 by dgwoo 편대 여단전투 버튼 기획안 
	   _Q_PARTY_BATTLE,					//"상대편 편대장 캐릭터 명을 입력하세요"
	   _Q_GUILD_BATTLE,					//"상대편 여단장 캐릭터 명을 입력하세요"
	   // 2008-11-13 by bhsohn 조이스틱 작업
	   _Q_JOYSTICK_SETUP,				// 조이스틱 옵션 파일
	   // end 2008-11-13 by bhsohn 조이스틱 작업
// 2008-12-09 by dgwoo 미션마스터.
	   _Q_MISSION_MASTER_CANCEL,			// ""미션 마스터 등록을 해제 하시겠습니까?"
	   _Q_MISSION_MASTER_HELPER,			//"%s님의 미션 도움을 요청 하였습니다."
// end 2008-12-09 by dgwoo 미션마스터.
	// 2008-12-30 by bhsohn 지도자 채팅 제한 카드 기획안
		_Q_USEITEM_LEADER_BAN_CHAT,
		// 2009-01-12 by bhsohn 친구 등록 당하는 유저 메시지창 기능 추가
		_Q_FRIEND_LIST_INSERT,

		// 2009. 01. 12 by ckPark 선전 포고 시스템
		_Q_WAR_DECLARE_GIVE_UP,				//"다음주 공격 모선전을 포기하시겠습니까?"
		_Q_WAR_DECLARE,						//"%d년 %d월 %02d시 %02d분에 %s 세력에게 선전포고를 하시겠습니까?"
		// end 2009. 01. 12 by ckPark 선전 포고 시스템
		_Q_GUILD_CREATE_CONFIRM,				// 비행단 생성 확인창 // 2009-02-02 by bhsohn 여단 생성시 확인창 추가
		// 2009-02-13 by bhsohn 월드 랭킹 시스템
		_Q_CREATE_NICKNAME_OKCANCEL,	// 닉네임 생성 확인창
		_Q_CREATE_NICKNAME,				// 닉네임 생성창		
		// end 2009-02-13 by bhsohn 월드 랭킹 시스템
		// 2009-04-08 by bhsohn 다중 선택시, 유니크 및 인챈트 된 아이템 판매시 경고 메시지 출력 기획 문서(K0000174)
		_Q_MULTI_SEL_WARNING,		// "%s 아이템은 \\y유니크\\y 아이템입니다. 정말로 선택 하시겠습니까?"
		// end 2009-04-08 by bhsohn 다중 선택시, 유니크 및 인챈트 된 아이템 판매시 경고 메시지 출력 기획 문서(K0000174)

		// 2009. 10. 08 by ckPark 필드 창고 아이템 중복 불가 처리
		_Q_ALREADY_HAVE_ITEM,		// "같은 종류 아이템의 사용기간이 만료 되지 않아 아이템을 사용할 수 없습니다. 사용기간이 만료된 후 사용해 주세요."
		// end 2009. 10. 08 by ckPark 필드 창고 아이템 중복 불가 처리

		// 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템
		_Q_INFINITY_GIVE_UP,		// "인피니티 필드의 진행을 중지 하고 도시로 귀환 하시겠습니까?"
		_Q_REVIVE_WAIT_5SECOND,		// "\\r%d초\\r 후에 부활이 가능합니다."
		_Q_BUY_INFINITYSHOP_ITEM,	// "아이템을 구매 하시겠습니까?"
		_Q_INFINITY_RETURN_TO_CITY,	// "인피니티 필드를 종료하고 도시로 귀환 하시겠습니까?"
		_Q_RETURN_TO_PROGRESS_INFINITY,// 비정상 종료가 되었습니다. 진행중인 인피니티 필드로 이동 하시겠습니까?
		// end 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템
		// 2010. 06. 07 by jskim 파트너 시스템 구현 - 파트너 정보창
		_Q_PARTNER_NEW_NAME_CHANGE,
		_Q_PARTNER_NAME_OKCANCLE,
		//end 2010. 06. 07 by jskim 파트너 시스템 구현 - 파트너 정보창
		// 2010. 06. 18 by jskim 인챈트 경고 메시지 추가
		_Q_ENCHANT_PREVENTION,
		//end 2010. 06. 18 by jskim 인챈트 경고 메시지 추가
		_Q_DIS_BOOM,			 // 2010-08-31 by shcho&&jskim, 용해 시스템 구현

		_Q_SETTING_SOCKET_OKCANCLE,		  //2011-10-06 by jhahn 파트너 성장형 시스템

		_Q_INVEN_ITEM_SORT,		// 2012-08-13 by mspark, 인벤토리 자동 정렬 확인창 추가 작업

		_Q_APPEARANCE_CHANGE_PREVENTION,		// 2012-10-30 by mspark, 외형 변경 경고 메시지 추가

		_Q_INCHANT_ITEM_DELETE,				// 2013-04-23 by ssjung 중요 아이템 삭제시 2차 경고(질문) 팝업창 구현

		_Q_AUTO_KICK,

		_Q_USEITEM_NATION_CHANGE,			// 2015-12-17 Future, Nation Change Card
		
};
// 인터페이스 MsgProc 리턴값
#define INF_MSGPROC_CLOSE	0	// 이 값을 리턴한 놈을 지운다.
#define INF_MSGPROC_NORMAL	1	// 정상적으로 처리를 계속한다.
#define INF_MSGPROC_BREAK	2	// 메시지를 빼낸다.즉, 아래 Msg Proc에서는 현재 msg를 수행하지 않는다.

#define TIME_HOUR		4
#define TIME_MINUTE		60
#define TIME_SECOND		60
#define TIME_DAY_CHANGE	300.0f


#define MAX_BEGIN_QUEST_COUNT	70	// 초보 모드(튜토리얼) 퀘스트 수


typedef struct _VERTEXINFO
{
    D3DXVECTOR3 pos;
	D3DXVECTOR3 nor;
} VERTEXINFO;

typedef struct _TILEINFOCLIENT // tile type
{
	INT			useTexNumber;		// 사용텍스쳐의 인덱스
	// 2005-04-06 by jschoi 물타입 추가
	DWORD		dwWaterType;		// useWater대신 물타입으로 변경
//	BOOL		useWater;			// Water 사용
	INT			waterHeight;		// Water 높이
	SHORT		waterTexNumber;		// Water Texture
	BYTE		bMove;				// Move Type
	BOOL		bEnableLand;		// 착륙가능
	BYTE		bEventType;			// 이벤트 타입 1:(워프 입구) 2:(워프 출구) 3:상점
} TILEINFOCLIENT;


typedef struct _OBJECTINFOCLIENT
{
	DWORD		dwObjType;			// Object 타입
	INT			nObjCount;
	D3DXVECTOR3 vPos;
	D3DXVECTOR3 vVel;
	D3DXVECTOR3 vUp;
	DWORD		dwObjectMonsterUniqueNumber;	// 2004-11-27 by jschoi with cmkwon
	BYTE		bObjectTexIndex;
	UINT		nObjectSrcIndex;
	BYTE		bBodyCondition;
	BOOL		bIsEvent;
	BYTE		bEventType;
	short		sEventIndexFrom;
	short		sEventIndexTo;
	short		sEventIndex3;
	char		strEventParam[40];
} OBJECTINFOCLIENT;

// 2012-05-02 by isshin 맵툴 개선 오브젝트 스케일 정보
typedef struct _OBJECTSCALEINFO
{
	D3DXVECTOR3 vObjScale;	
} OBJECTSCALEINFO;
// end 2012-05-02 by isshin 맵툴 개선 오브젝트 스케일 정보

typedef struct _MAP_DATA
{
	FLOAT			fTileSize;
	int				nVertexNumber;
	int				nTileInfoNumber;
	int				nObjectNumber;
}MapInfo;

typedef struct _WORKSPACE
{
	char workspaceName[40];
	int numberOfProject;
} WORKSPACE;

#ifndef _ATUM_SERVER
typedef struct _PROJECTINFO
{
	int			useTileSetIndex;
	char		strProjectName[40];
	char		strFieldIP[16];
	short		sFieldPort;
	short		sFieldPortUDP;
	char		strNPCIP[16];
	short		sNPCPort;
	short		sNPCPortUDP;
	short		sXSize;
	short		sYSize;
	float		fHeightMax;
	float		fHeightMin;
	float		fWaterHeight;
	float		fDiffuseR1;
	float		fDiffuseG1;
	float		fDiffuseB1;
	float		fAmbientR1;
	float		fAmbientG1;
	float		fAmbientB1;
	float		fDiffuseR2;
	float		fDiffuseG2;
	float		fDiffuseB2;
	float		fAmbientR2;
	float		fAmbientG2;
	float		fAmbientB2;
} PROJECTINFO;
#endif	// !_ATUM_SERVER

struct GROUNDVERTEX
{
    D3DXVECTOR3 p;
    D3DXVECTOR3 n;
    FLOAT       tu, tv;

	// 2008. 12. 11 by ckPark 물렌더링
	// 범프 맵 텍스쳐 좌표로 사용
	// FLOAT       tu2, tv2;				// 삭제하고 WATERBUMPVERTEX 새로 만듬
	// end 2008. 12. 11 by ckPark 물렌더링
};


// 2008. 12. 11 by ckPark 물렌더링
struct WATERBUMPVERTEX
{
	D3DXVECTOR3 p;
    D3DXVECTOR3 n;
    FLOAT       tu, tv;
	FLOAT       tu2, tv2;		// 범퍼 맵 텍스쳐 좌표로 사용
};
// end 2008. 12. 11 by ckPark 물렌더링


typedef struct _WATERINFO
{
	BOOL		useWater;			// Water 사용
	INT			waterHeight;		// Water 높이
	SHORT		waterTexNumber;		// Water Texture
} WATERINFO;

typedef struct _NOSPRITE
{
    D3DXVECTOR3 p;
    DWORD       color;
} NOSPRITE;


struct FOGVERTEX
{
	D3DXVECTOR3 p;
	D3DXVECTOR3 n;
	FLOAT       tu, tv;
};

typedef struct _SKILLITEMCHECKTIME
{
	BOOL	bReady;
	FLOAT	fCurrentTime;
}SKILLITEMCHECKTIME;

typedef struct _EFFECTINFO
{
	char		strEffectName[40];
	INT			nType;
	BOOL		bRegion;
	BOOL		bLoop;
	FLOAT		fReTime;
	FLOAT		fCheckTime;
	D3DXVECTOR3 vPos;
} EFFECTINFO;

typedef struct _EFFECTINFOCLIENT
{
	INT			nType;
	BOOL		bRegion;
	BOOL		bLoop;
	FLOAT		fReTime;
	D3DXVECTOR3 vPos;
	FLOAT		fCheckTime;
} EFFECTINFOCLIENT;

typedef struct _PARTYINFO
{
	PartyID_t	PartyID;
	PARTY_TYPE	bPartyType;
	BYTE		bFormationFlyingType;
	BYTE		bFormationFlyingPositionType;
	UINT		nMasterUniqueNumber;	// 파티(장)의 번호, added!
}PARTYINFO;

struct ENEMYINFO
{
	MEX_OTHER_CHARACTER_INFO CharacterInfo;
	CHARACTER_RENDER_INFO	 CharacterRenderInfo;			// 장착아이템의 정보
	MAP_CHANNEL_INDEX		 MapChannelIndex;
	SHORT			HP;
	float			CurrentHP;
	SHORT			DP;
	float			CurrentDP;
	SHORT			SP;
	SHORT			CurrentSP;
	SHORT			EP;
	SHORT			CurrentEP;
	BYTE			Level;
};

struct GUIUnitRenderInfo
{
	D3DXMATRIX	matrix;
	UINT		rendertype;	// 유닛카인드 --> 아머 포함
};

struct MONSTERINFO
{
	char		strRegionName[40];
	UINT		nMonType;				//	몬스터 타입(인덱스)
	short		sStartx;				//	영역의 시작좌표 X
	short		sStartz;				//	영역의 시작좌표 Y
	short		sEndx;					//	영역의 끝좌표 X
	short		sEndz;					//	영역의 끝좌표 Y
	short		sMaxMon;				//	
	short		sResNum;				//
	short		sResTime;				//
	BYTE		bMonType;				// 0:보스 1:대형 2:중형 3:소형
};

struct INVEN_DISPLAY_INFO
{
	char IconName[20];
	char Name[50];
	ITEM_BASE* pItem;//ITEM_BASE로 한 이유는 ITEM_GENERAL과 ITEM_SKILL을 포함하기 위해서..
};

// 상점에서 레벨로 아이템 소팅시에 사용
struct CompareLevel
{
	template<typename T>
		bool operator()(const T *pPtr1, const T *pPtr2)
	{
		ITEM* pITEM1 = g_pDatabase->GetServerItemInfo(pPtr1->ItemNum);
		ITEM* pITEM2 = g_pDatabase->GetServerItemInfo(pPtr2->ItemNum);
		if( pITEM1 == NULL || pITEM2 == NULL )
		{
			return false;
		}
		if(pITEM1->ReqMinLevel < pITEM2->ReqMinLevel)
		{
			return true;
		}
		return false;
	}
};

struct STAGE_EFFECT_DATA
{
	INT			*	pWarpObject;
	INT				nMissionIndex;

	STAGE_EFFECT_DATA()
	{
		pWarpObject		= NULL;
		nMissionIndex	= -1;
	}
};


#define D3DFVF_TILEVERTEX (D3DFVF_XYZ|D3DFVF_DIFFUSE|D3DFVF_TEX1)
#define D3DFVF_FOGVERTEX (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_TEX1)
#define D3DFVF_NOSPRITE (D3DFVF_XYZ|D3DFVF_DIFFUSE)


// 2008. 12. 11 by ckPark 물렌더링
//#define D3DFVF_GROUNDVERTEX (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_TEX2)
#define D3DFVF_GROUNDVERTEX (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_TEX1)

#define D3DFVF_WATERBUMPVERTEX		(D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_TEX2)
// end 2008. 12. 11 by ckPark 물렌더링




#define MOUSE_TYPE_0	0	// 마우스 일반
#define MOUSE_TYPE_1	1	// 방향 지시
#define MOUSE_TYPE_2	2	// 마우스 빨간색
#define MOUSE_TYPE_3	3	// 마우스 파란색
#define MOUSE_TYPE_4	4	// 마우스 빨간색 회전
#define MOUSE_TYPE_5	5	// 마우스 파란색 회전

#define MOUSE_TYPE_6	6	// 왼쪽 위 오른쪽 아래 화살표
#define MOUSE_TYPE_7	7	// 왼쪽 아래 오른쪽 위 화살표
#define MOUSE_TYPE_8	8	// 왼쪽 오른쪽 화살표
#define MOUSE_TYPE_9	9	// 위 아래 화살표
#define MOUSE_TYPE_10	10	// 십자 화살표

#define MOUSE_TYPE_11	11	// 스킬 타겟 마우스

// 키보드 메시지 LPARAM 값
#define WM_KEYDOWN_LPARAM_Q				0x00100001
#define WM_KEYDOWN_LPARAM_R				0x00130001
#define WM_KEYUP_LPARAM_R				0xc0130001
#define WM_KEYDOWN_LPARAM_F				0x00210001
#define WM_KEYUP_LPARAM_F				0xc0210001
#define WM_KEYDOWN_LPARAM_V				0x002f0001
#define WM_KEYDOWN_LPARAM_M				0x00320001
#define WM_KEYDOWN_LPARAM_PLUS			0x000d0001
#define WM_KEYDOWN_LPARAM_KEYPAD_PLUS	0x004e0001
#define WM_KEYUP_LPARAM_KEYPAD_PLUS		0xc04e0001
#define WM_KEYDOWN_LPARAM_MINUS			0x000c0001
#define WM_KEYDOWN_LPARAM_KEYPAD_MINUS	0x004a0001
#define WM_KEYDOWN_LPARAM_I				0x00170001
#define WM_KEYDOWN_LPARAM_N				0x00310001
#define WM_KEYDOWN_LPARAM_E				0x00120001
#define WM_KEYUP_LPARAM_E				0xc0120001
#define WM_KEYDOWN_LPARAM_Z				0x002c0001
#define WM_KEYUP_LPARAM_Z				0xc02c0001
#define WM_KEYDOWN_LPARAM_T				0x00140001
#define WM_KEYDOWN_LPARAM_B				0x00300001
#define WM_KEYDOWN_LPARAM_P				0x00190001
#define WM_KEYDOWN_LPARAM_A				0x001e0001
#define WM_KEYUP_LPARAM_A				0xc01e0001
#define WM_KEYDOWN_LPARAM_D				0x00200001
#define WM_KEYUP_LPARAM_D				0xc0200001
#define WM_KEYDOWN_LPARAM_L				0x00260001
#define WM_KEYDOWN_LPARAM_K				0x00250001
#define WM_KEYDOWN_LPARAM_FLOW			0x00290001	// `키 // 2008-10-23 by bhsohn 자기 자신한테 힐 단축키 추가


#define DEFAULT_OBJECT_MONSTER_OBJECT	90000000
#define IS_OBJECT_MONSTER_OBJECT_CODE(x)		(( (int)((x)/DEFAULT_OBJECT_MONSTER_OBJECT) == 1 ) ? TRUE:FALSE)

#define WARP_GATE_OBJECT_NUM			3003500
#define WARP_GATE_OBJECT_NUM_2			3151300					// 2008-06-24 by dgwoo 워프 게이트 추가.
#define WARP_GATE_OBJECT_NUM_3			3204600					// 2012-06-13 by jhahn 워프 게이트 추가2.
// 2007-07-10 by dgwoo 아래와 같이 튜토리얼 게이트 변경.
#define TUTORIAL_GATE					3107400
#define TUTORIAL_LAND_GATE				3107500
//#define TUTORIAL_GATE					3024500
#define ARENA_MAP_01_DOOR_OBJECT				3099100
#define ARENA_MAP_02_DOOR_OBJECT				3207800		// 2012-05-29 by mspark, 아레나 2번 맵(라그나로크) 문


#define ZERO_MEMORY(x)					memset( x, 0x00, sizeof(x));	// 변수 초기화 관련

#define SKILL_OBJECT_ALPHA_NONE				255
#define SKILL_OBJECT_ALPHA_DEFAULT			135
#define SKILL_OBJECT_ALPHA_OTHER_INFLUENCE	0

// 2007-04-19 by bhsohn 서치아이 아이템 추가
// 스캔 관련 특성들
#define SCAN_SKILL						0
#define SCAN_ITEM						1
// 2012-11-29 by mspark, 서치아이 위치가 이동하는 캐릭터의 위치와 동일하도록 수정
#define SCAN_ITEM2						2
//#define MAX_SCAN						2
#define MAX_SCAN						3
// end 2012-11-29 by mspark, 서치아이 위치가 이동하는 캐릭터의 위치와 동일하도록 수정

// 2007-09-06 by dgwoo 전진기지 상태
#define OUTPOST_STATE_BEFORE				1	
#define OUTPOST_STATE_WAR_START				2
#define OUTPOST_STATE_WAR_END				3
#define OUTPOST_STATE_PROTECT_DESTROY		4
#define OUTPOST_STATE_RESET_START			5
#define OUTPOST_STATE_RESET_DESTROY			6
#define OUTPOST_STATE_RESET_SUCCESS			7

// 모선전 진형
#define	MOTHERSHIP_MAX_INFLUENCE					2	// 최대 진영수 
#define	MOTHERSHIP_INFLUENCE_VCN					0	// 바이제니유
#define	MOTHERSHIP_INFLUENCE_ANI					1	// 알링턴

// 2007-11-22 by bhsohn 아레나 통합서버
// 아레나 현재 진행상황
#define	ARENA_STATE_NONE					0	// 일반 게임 상태
#define	ARENA_STATE_CONNECTING_FS			1	// 아레나 필드 서버 연결중
#define	ARENA_STATE_ARENA_WAIT				2	// 아레나 태기상태
#define	ARENA_STATE_ARENA_MAIN_TO_ARENA		3	// 아레나 허용하고 서버로 부터 응답을 기다리는 상태
#define	ARENA_STATE_ARENA_LOAD_GAME_INFO	4	// 아레나 맵에 이동해서 관련 정보들 로드 
#define	ARENA_STATE_ARENA_GAMING			5	// 아레나 게임중 
#define	ARENA_STATE_ARENA_ARENA_TO_MAIN		6	// 아레나 끝나고 메인서버로 이동

// 2008-02-15 by bhsohn 네트워크 속도 체크
typedef struct
{
	int nMaxRepeatCnt;			// 최대 반복수
	int nCurruntRepeatCnt;		// 현재 반복수
	DWORD dwLastCheckTime;		// 마지막 체크 타임
	DWORD dwSumCheckTime;		// 시간 간격 추가
} structNetCheckInfo;

// 2008-02-25 by bhsohn 아레나 유저 정보 서버도 표시하게 변경
#define SIZE_MAX_ARENA_FULL_NAME	(SIZE_MAX_CHARACTER_NAME+SIZE_MAX_SERVER_NAME+1)

// 2008-05-19 by dgwoo 채팅 메뉴 리스트 인덱스.
#define CHAT_TAB_SPEAKER						0							// 스피커.	
#define CHAT_TAB_ALLUSER						1							// 모든 유저에게 채팅.
#define CHAT_TAB_NORMAL							2							// 일반 채팅
#define CHAT_TAB_WAR							3							// 전쟁 채팅
#define CHAT_TAB_TRADE							4							// 거래 채팅
#define CHAT_TAB_MAP							5							// 맵 채팅
#define CHAT_TAB_GUILD							6							// 여단 채팅
#define CHAT_TAB_PARTY							7							// 편대 채팅
#define CHAT_TAB_ARENA							8							// 아레나 채팅  
#define CHAT_TAB_CHATROOM						9							// 채팅방
#define CHAT_TAB_SYSTEM							10							// 시스템 메시지
#define CHAT_TAB_HELPDESK						11							// 도움말
#define CHAT_TAB_CNC							12							// 2015-07-19 Future, adding Cross Nation Chat
#define CHAT_TAB_INFLUENCE_LEADER				13							// 세력 지도자
#define CHAT_TAB_SOUND					        14							// 음성 채팅
#define CHAT_TAB_PTOP							15							// 귓속말
#define CHAT_TAB_CASH							16							// 유료 채팅
#define CHAT_TAB_GAMEMASTER						17							// 게임 마스터.
#define CHAT_TAB_NUMBER_ALL						18							// 채팅종류 수(전체)

// 2008-04-04 by bhsohn Ep3 커뮤니티 창
struct structReadLetterItemInfo
{
	UID64_t   LetterUID;	
	char chSendUser[SIZE_MAX_ARENA_FULL_NAME];					//보낸 유저
	char chLetterTitle[SIZE_MAX_ARENA_FULL_NAME];				// 편지 제목	
	ATUM_DATE_TIME	atimeMail;									// 받은 메일 등록일자
	BOOL bReadMail;												// 읽었던 메일이냐?
	BOOL bAllMail;												// 전체 메일이냐?
};


// 여단 관리
struct structGuildInfo
{
	char	MemberName[SIZE_MAX_CHARACTER_NAME];		// 유저명
	UID32_t	MemberUniqueNumber;
	
	USHORT	UnitKind;						// 유닛의 종류
	int		nUnitIdx;						// 유닛순서
	char	chUnitKind[SIZE_MAX_CHARACTER_NAME];		// 유닛의 종류

	char 	chLevel[SIZE_MAX_CHARACTER_NAME];							//
	BYTE	GuildRank;				// GUILD_RANK_XXX
	BYTE	GuildOrderIdx;			// 길드 우선순위
	BYTE	IsUseVOIP;				// 1: TRUE, 0: FALSE
	BYTE	IsOnline;				// 1: TRUE, 0: FALSE
	BYTE	IsUseSpeakeingVOIP;		// 말하고 있는지 여부 // 2008-10-14 by bhsohn VOIP개선
};

// end 2008-04-04 by bhsohn Ep3 커뮤니티 창

// 2008-06-16 by bhsohn 태국 게임 종료후 웹페이지 뜨우게 함
#define CLOSE_WEBSTIE		_T("http://ace.winner.co.th/ads/promotion.asp")

// 2008-06-20 by bhsohn EP3 옵션관련 처리
// 각각의 셋업정보
#define		OPTION_RADIO_MAN_TO_MAN			0	// 1:1전투
#define		OPTION_RADIO_FORMATIONWAR		1	// 편대전투
#define		OPTION_RADIO_GUILD_WAR			2	// 여단전투
#define		OPTION_RADIO_WHISPER			3	// 귓속말
#define		OPTION_RADIO_FORMATION_INVITE	4	// 편대초대
#define		OPTION_RADIO_GUILD_INVITE		5	// 여단초대
#define		OPTION_RADIO_EXCHANGE			6	// 교환
#define		OPTION_RADIO_INTERFACE_POS		7	// 인터페이스 위치 저장
#define		OPTION_RADIO_INTERFACE_HIDE		8	// 인터페이스 모두 감추기
#define		OPTION_RADIO_FRIEND_ID_HIDE		9	// 같은 세력 아이디 감추기
#define		OPTION_RADIO_ENEMY_ID_HIDE		10	// 다른 세력 아이디 감추기
#define		OPTION_RADIO_MOUSE_LOCK			11	// 마우스 가두기(윈도우 모드시)
#define		OPTION_RADIO_CHAT_FILTER		12	// 욕펄터
#define		MAX_OPTION_RADIO				13	

enum
{
	VEDIO_OPTION_OPERATOR,
	VEDIO_OPTION_INTERFACE_POS,
	VEDIO_OPTION_INTERFACE_HIDE,
	VEDIO_OPTION_PET_OP_SYS,
#ifdef _WARRIOR_ADDITIONAL_INFO
	VEDIO_OPTION_WSW_ADD_AIMINFO,
#endif
#ifdef _SHOW_LEADER_INFO
	VEDIO_OPTION_SHOW_GUILDNAME,
#endif
	MAX_VEDIO_OPTION_RADIO
};

enum
{
	SOUND_OPTION_MUSIC,
	MAX_SOUND_OPTION_RADIO
};

enum
{
	ETC_OPTION_MAN_TO_MAN,
	ETC_FORMATIONWAR,
	ETC_GUILD_WAR,
	ETC_WHISPER,
	ETC_FORMATION_INVITE,
	ETC_GUILD_INVITE,
	ETC_EXCHANGE,
	ETC_FRIEND_ID_HIDE,
	ETC_ENEMY_ID_HIDE,
	ETC_MOUSE_LOCK,
	ETC_CHAT_FILTER,
	ETC_SHOW_MOUSE,
	ETC_HELP,
	ETC_MAINTAIN,
	ETC_MOUSE_REVERSE_TB,
	MAX_ETC_OPTION_RADIO
};

typedef struct 
{
	float	fRateX;
	float	fRateY;
	float	fRateWidth;
	float	fRateHeight;
	float	fGameWidth;
	float	fGameHeight;
}stcuctRateRect;

typedef struct 
{
	BOOL		bRadioInfo[MAX_OPTION_RADIO];			// 라디오 버튼 정보
	stcuctRateRect	stRateRect;

	// 2009. 02. 03 by ckPark 채팅창 인터페이스 크기 저장
	stcuctRateRect	stChatRect;
	// end 2009. 02. 03 by ckPark 채팅창 인터페이스 크기 저장
}sOPTION_ETC;
// end 2008-06-20 by bhsohn EP3 옵션관련 처리

// 2008-07-10 by bhsohn 타켓힐이 안되는 문제 해결
#define FRAME_START_Y		250//85
#define PARTY_FRAME_SIZE_X			130
#define PARTY_FRAME_SIZE_Y			34
// end 2008-07-10 by bhsohn 타켓힐이 안되는 문제 해결


// 2009. 09. 21 by ckPark 인피니티 필드 몬스터 스킬 구현
#define PARTY_FRAME_DEBUFF_ICON_SIZE	15
#define PARTY_FRAME_DEBUFF_START_X		(PARTY_FRAME_SIZE_X + 5)
#define PARTY_FRAME_DEBUFF_Y			(PARTY_FRAME_SIZE_X + 5)
#define PARTY_FRAME_DEBUFF_LINE_PER_ICON 5
// end 2009. 09. 21 by ckPark 인피니티 필드 몬스터 스킬 구현


// 2008-08-19 by bhsohn 세력 마크 시스템 추가
// 마크 오브젝트
#define MARK_INFLUENCE_VCU_OBJECT		7014740		// 바이제니유
#define MARK_INFLUENCE_ANI_OBJECT		7014770		// 알링턴
// end 2008-08-19 by bhsohn 세력 마크 시스템 추가

// 2008-09-26 by bhsohn 신규 인첸트 처리
typedef struct 
{
	POINT				ptItemIvenPos;
	POINT				ptEqIvenPos;
	BOOL				bItemSetPos;		// 위치를 지정했냐?
	BOOL				bEqSetPos;		// 위치를 지정했냐?
	BOOL				bShowEqWnd;		// 일반 인벤장비창
	BOOL				bShowEqShopWnd;	// 상점 장비창
	POINT				ptEqIvenShopPos;	 
	BOOL				bEqShopSetPos;
} structInvenPosInfo;

// end 2008-09-26 by bhsohn 신규 인첸트 처리

// 2008-10-15 by bhsohn 리소스 메모리 보호 기능 추가
#define NOT_HAVE_OBJECT					3157700
#define INTERFCE_NOT_HAVE_TEXTURE		"xxx"
// end 2008-10-15 by bhsohn 리소스 메모리 보호 기능 추가

// 2008-10-16 by bhsohn 전진기지 모선전, 거점전 정보창
struct structMotherShipInfo
{
	ATUM_DATE_TIME SummonTime;					// 2008-08-19 by bhsohn 세력전, 모선전 정보 소환 시간으로 정렬
	char chAttackInfluence[64];							// 공격 세력
	char chWinInfluence[64];							// 승리 세력
	char chMotherShipName[SIZE_MAX_MONSTER_NAME];		// 공격 세력 모선
	char chWarPoint[64];								// 세력포인트
	char chTimeCap[512];								// 세력전 진행 시간	
};

struct structWarPointInfo
{
	ATUM_DATE_TIME SummonTime;					// 2008-08-19 by bhsohn 세력전, 모선전 정보 소환 시간으로 정렬
	char chAttackInfluence[64];							// 공격 세력
	char chWinInfluence[64];							// 승리 세력
	char MapName[SIZE_MAX_MAP_NAME];					// 거점전 맵이름	
	char chTimeCap[512];								// 세력전 진행 시간	
};
// end 2008-10-16 by bhsohn 전진기지 모선전, 거점전 정보창

// 2008-11-13 by bhsohn 조이스틱 작업
// 조이스틱 기능모음 옵션
#define	JOSTICK_OPTION_MOUSE_UP					0		// 마우스(상)
#define	JOSTICK_OPTION_MOUSE_DOWN				1		// 마우스(하)
#define	JOSTICK_OPTION_MOUSE_LEFT				2		// 마우스(좌)
#define	JOSTICK_OPTION_MOUSE_RIGHT				3		// 마우스(우)
#define	JOSTICK_OPTION_UNIT_GO					4		// go
#define	JOSTICK_OPTION_UNIT_MIN_SPEED			5		// 최소 속도로 's'키
#define	JOSTICK_OPTION_UNIT_LEFT				6		// Left
#define	JOSTICK_OPTION_UNIT_RIGHT				7		// Right
#define	JOSTICK_OPTION_TARGET_ME				8		// 자신 타켓
#define	JOSTICK_OPTION_WEAPON_1					9		// 1형무기
#define	JOSTICK_OPTION_WEAPON_2					10		// 2형무기
#define	JOSTICK_OPTION_TARGET_LOCKON			11		// 타켓 락온
#define	JOSTICK_OPTION_BOSTER					12		// 부스터
#define	JOSTICK_OPTION_UNITSTOP					13		// 유닛정지
#define	JOSTICK_OPTION_TAKEOFF					14		// 이착륙
#define	JOSTICK_OPTION_BACKVIEW					15		// 뺵�?
#define	JOSTICK_OPTION_JOIN_FORMATION			16		// 편대 대형 참여
#define	JOSTICK_OPTION_SLOT_CHANGE				17		// 스킬 슬롯 변경 
#define	JOSTICK_OPTION_SLOT_0					18		// 1
#define	JOSTICK_OPTION_SLOT_1					19		// 2
#define	JOSTICK_OPTION_SLOT_2					20		// 3
#define	JOSTICK_OPTION_SLOT_3					21		// 4
#define	JOSTICK_OPTION_SLOT_4					22		// 5
#define	JOSTICK_OPTION_SLOT_5					23		// 6
#define	JOSTICK_OPTION_SLOT_6					24		// 7
#define	JOSTICK_OPTION_SLOT_7					25		// 8
#define	JOSTICK_OPTION_SLOT_8					26		// 9
#define	JOSTICK_OPTION_SLOT_9					27		// 10
// 2009-01-16 by bhsohn 조이스틱 추가 승인/취소 추가
//#define	JOSTICK_OPTION_SLOT_EMPTY				28		// empty
//#define	MAX_JOSTICK_OPTION						30		
#define	JOSTICK_OPTION_OK						28		// 
#define	JOSTICK_OPTION_CANCEL					29		// 
#define	MAX_JOSTICK_OPTION						40		
// end 2009-01-16 by bhsohn 조이스틱 추가 승인/취소 추가
struct structJoystickKey
{
	int nKeyIdx;					// 키
	int nKeyValue;					// 키값
};

typedef struct 
{		
	BOOL		bUseJoystick;				// 조이스틱 사용 여부
	char		chJoySelJoy[MAX_PATH];		// 선택 한 조이스틱 장치
	BOOL		bUseFeedJoyStick;			// 진동 사용 여부
	int			nJoysticSense;				// 감도 조절
	char		chJoyKeyFile[MAX_PATH];		// 키 파일명 조절	
		
} structJoyStickOptionInfo;

// 최대 슬롯 수
#define		MAX_JOSTICK_OPTION_SLOT				5
// 최대 행동 등록수
#define		MAX_JOSTICK_OPTION_BEHAVIOR			10

struct structJoystickSetupKeyTable
{
	int nKeySetup;					// 키
	int nBehavior[MAX_JOSTICK_OPTION_SLOT];					// 행동 테이블
};
// end 2008-11-13 by bhsohn 조이스틱 작업

// 2009-02-05 by bhsohn 카메라 깨지는 현상 처리
#define CHARACTER_CAMERA_DISTANCE		30.0f	// 초기거리
#define CHARACTER_CAMERA_HEIGHT			30.0f	// 초기높이
#define CHARACTER_CAMERA_WHEEL_DISTANCE 6.0f	// 휠 간격
// end 2009-02-05 by bhsohn 카메라 깨지는 현상 처리

// 2009-02-13 by bhsohn 월드 랭킹 시스템
#define	INFLUENCE_NAME_ANI			"ANI"			
#define	INFLUENCE_NAME_BCU			"BCU"	

#define	WORLD_RANKING_LEVEL			"LEVEL"			
#define	WORLD_RANKING_FAME			"FAME"
#define	WORLD_RANKING_PVP			"PVP"
// end 2009-02-13 by bhsohn 월드 랭킹 시스템

#endif
