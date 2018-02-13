#pragma once

enum : BYTE
{
	ITEMKIND_AUTOMATIC = 0,		// 오토매틱류(1-1형)
	ITEMKIND_VULCAN = 1,		// 발칸류(1-1형)
	ITEMKIND_DUALIST = 2,		// 듀얼리스트류(1-1형)	// 2005-08-01 by hblee : GRENADE -> DUALIST 으로 변경.
	ITEMKIND_CANNON = 3,		// 캐논류(1-1형)
	ITEMKIND_RIFLE = 4,			// 라이플류(1-1형)
	ITEMKIND_GATLING = 5,		// 개틀링류(1-1형)
	ITEMKIND_LAUNCHER = 6,		// 런처류(1-1형)
	ITEMKIND_MASSDRIVE = 7,		// 메스드라이브류(1-1형)

	ITEMKIND_ROCKET = 8,		// 로켓류(2-1형)
	ITEMKIND_MISSILE = 9,		// 미사일류(2-1형)
	ITEMKIND_BUNDLE = 10,		// 번들류(2-1형)
	ITEMKIND_MINE = 11,			// 마인류(2-1형)

	ITEMKIND_SHIELD = 12,	// 쉴드류(2-2형)
	ITEMKIND_DUMMY = 13,	// 더미류(2-2형)
	ITEMKIND_FIXER = 14,	// 픽서류(2-2형)
	ITEMKIND_DECOY = 15,	// 디코이류(2-2형)

	ITEMKIND_DEFENSE = 16,	// ARMOR
	ITEMKIND_SUPPORT = 17,	// ENGINE
	ITEMKIND_ENERGY = 18,	// CONSUMABLE KITS
	ITEMKIND_INGOT = 19,	// METALS
	ITEMKIND_CARD = 20,		// HOLOGRAMS, CONTOURS, KILLMARKS, BOXES
	ITEMKIND_ENCHANT = 21,	// ENCHANT CARDS
	ITEMKIND_TANK = 22,		// 탱크(EP 탱크 등)
	ITEMKIND_BULLET = 23,	// AMMUNITION
	ITEMKIND_QUEST = 24,	// QUEST ITEMS
	ITEMKIND_RADAR = 25,	// RADARS
	ITEMKIND_COMPUTER = 26,	// CPUS
	ITEMKIND_GAMBLE = 27,	// FIX GAMBLES
	ITEMKIND_PREVENTION_DELETE_ITEM = 28,	// EPROTS
	ITEMKIND_BLASTER = 29,	// 2005-08-01 by hblee : 블래스터류 아이템 추가.
	ITEMKIND_RAILGUN = 30,	// 2005-08-01 by hblee : 레일건류 아이템 추가.
	ITEMKIND_ACCESSORY_UNLIMITED = 31,	// ACCESSORY INLIMITED EQUIPPED
	ITEMKIND_ACCESSORY_TIMELIMIT = 32,	// TIME LIMITED ACCESSORY
	ITEMKIND_INFLUENCE_BUFF = 33,	// 2009-01-05 by dhjin, 미션마스터 - 편대 버프 아이템 추가 - 실제로 ITEMKIND_BUFF 라는 의미로 변경, // 2006-04-21 by cmkwon, 세력 버프, 현재는 세력버프 아이템
	ITEMKIND_INFLUENCE_GAMEEVENT = 34,	// 2006-04-21 by cmkwon, 세력 게임이벤트
	ITEMKIND_RANDOMBOX = 35,	// 2006-08-10 by cmkwon, 랜덤 박스 - 판도라 박스
	ITEMKIND_MARK = 36,	// 2006-08-21 by cmkwon, 마크 - 이펙트 아이템
	ITEMKIND_SKILL_SUPPORT_ITEM = 37,	// LIKE TIME LIMITED ACCESSORIES
	ITEMKIND_PET_ITEM = 38,	// 2010-06-15 by shcho&hslee 펫시스템 - 펫 아이템.
	ITEMKIND_GENERAL_ITEM_END = 39,	// General Item의 끝을 표시함, 실재로 사용 안 됨

	ITEMKIND_ALL_ITEM = 40,	// 모든 아이템, ReqItemKind 확인할 필요 없음
	ITEMKIND_ALL_WEAPON = 43,	// 모든 무기
	ITEMKIND_PRIMARY_WEAPON_ALL = 44,	// 1형 무기
	ITEMKIND_PRIMARY_WEAPON_1 = 45,	// 1-1형 무기
	ITEMKIND_PRIMARY_WEAPON_2 = 46,	// 1-2형 무기
	ITEMKIND_SECONDARY_WEAPON_ALL = 47,	// 2형 무기
	ITEMKIND_SECONDARY_WEAPON_1 = 48,	// 2-1형 공격 무기
	ITEMKIND_SECONDARY_WEAPON_2 = 49,	// 2-2형 방어 무기

	ITEMKIND_SKILL_ATTACK = 50,	// 공격 스킬
	ITEMKIND_SKILL_DEFENSE = 51,	// 방어 스킬
	ITEMKIND_SKILL_SUPPORT = 52,	// 컨트롤 스킬 + 보조 스킬
	ITEMKIND_SKILL_ATTRIBUTE = 53,	// 속성 스킬

	ITEMKIND_COLOR_ITEM = 60,	// 아머 아이템의 색상 아이템, // 2005-12-06 by cmkwon

	ITEMKIND_FOR_MON_PRIMARY = 100,	// 1형 몬스터용 아이템
	ITEMKIND_FOR_MON_GUN = 101,	// 몬스터 기관포류(1-1형)
	ITEMKIND_FOR_MON_BEAM = 102,	// 몬스터 빔류(1-2형)
	ITEMKIND_FOR_MON_ALLATTACK = 103,	// 몬스터 전체 공격
	ITEMKIND_FOR_MON_SKILL = 104,	// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 몬스터 스킬 아이템 Kind 추가
	ITEMKIND_FOR_MON_RANGE_ATTACK = 105,	// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 몬스터 범위 공격 
	ITEMKIND_FOR_MON_SECONDARY = 150,	// 2형 몬스터용 아이템
	ITEMKIND_FOR_MON_ROCKET = 151,	// 몬스터 로켓(2-1형)
	ITEMKIND_FOR_MON_MISSILE = 152,	// 몬스터 미사일류(2-1형)
	ITEMKIND_FOR_MON_BUNDLE = 153,	// 몬스터 번들류(2-1형)
	ITEMKIND_FOR_MON_MINE = 154,	// 몬스터 마인류(2-1형)
	ITEMKIND_FOR_MON_SHIELD = 155,	// 몬스터 쉴드류(2-2형)
	ITEMKIND_FOR_MON_DUMMY = 156,	// 몬스터 더미류(2-2형)
	ITEMKIND_FOR_MON_FIXER = 157,	// 몬스터 픽서류(2-2형)
	ITEMKIND_FOR_MON_DECOY = 158,	// 몬스터 디코이류(2-2형)
	ITEMKIND_FOR_MON_FIRE = 159,	// 몬스터 파이어류
	ITEMKIND_FOR_MON_OBJBEAM = 160,	// 몬스터 충돌가능 빔류 e 직선 빔과 충돌할 경우 데미지 발생
	ITEMKIND_FOR_MON_STRAIGHTBOOM = 161,	// 몬스터 직진 폭탄류 e 일정한 방향성을 가지고 무기가 발사되며 그 오브젝트에 맞을 경우 데미지 발생 (패턴으로 가능 예상, 검토바람)
	ITEMKIND_FOR_MON_MAIN_ARMAMENT = 162,	// 몬스터 주포 공격류 (인피니티 3차 모선전) - // 2011-03-28 by hskim, 인피니티 3차 - 몬스터 멀티 타겟팅 기능 추가 - 메인 주포로 설정된 경우 일정 거리가 벗어나도 발사되는 이펙트를 볼 수 있음

	ITEMKIND_PET_SOCKET_ITEM = 249,	// 펫의 장착 소켓류 아이템, // 2011-09-01 by hskim, 파트너 시스템 2차 - 값이 변경될시에는 SQL 저장 프로시저 arena_CopyDBInfo 및 arena_CopyDBInfo_Infinity 같이 수정하세요 (쿼리에 249 번호 박혀 있음)

	ITEMKIND_GENERAL_ITEM_2ND_END = ITEMKIND_PET_SOCKET_ITEM,		// General Item 2차의 끝을 표시함
	ITEMKIND_GENERAL_ITEM_2ND_START = ITEMKIND_PET_SOCKET_ITEM,		// General Item 2차의 시작을 표시함

	ITEMKIND_UNKNOWN = 255,
	ITEMKIND_ALL_ITEM_END = ITEMKIND_UNKNOWN,
};


constexpr bool IS_GENERAL_ITEM(BYTE x)
{
	return (x >= ITEMKIND_AUTOMATIC && x <= ITEMKIND_GENERAL_ITEM_END) ||
		(x >= ITEMKIND_GENERAL_ITEM_2ND_END && x <= ITEMKIND_GENERAL_ITEM_2ND_START);
}
constexpr bool IS_WEAPON(BYTE x) { return x >= ITEMKIND_AUTOMATIC && x <= ITEMKIND_DECOY; }
constexpr bool IS_PRIMARY_WEAPON(BYTE x) { return x >= ITEMKIND_AUTOMATIC && x <= ITEMKIND_MASSDRIVE; }
constexpr bool IS_PRIMARY_WEAPON_1(BYTE x) { return x >= ITEMKIND_AUTOMATIC && x <= ITEMKIND_MASSDRIVE; }	// 1-1형 무기인가?

constexpr bool IS_PRIMARY_WEAPON_2(BYTE x) { return false; }		// 2005-09-27 by cmkwon, 1-2형 무기는 없다

constexpr bool IS_SECONDARY_WEAPON(BYTE x) { return x >= ITEMKIND_ROCKET && x <= ITEMKIND_DECOY; }

constexpr bool IS_SECONDARY_WEAPON_1(BYTE x) { return x >= ITEMKIND_ROCKET && x <= ITEMKIND_MINE; }

constexpr bool IS_SECONDARY_WEAPON_2(BYTE x) { return x >= ITEMKIND_SHIELD && x <= ITEMKIND_DECOY; }

constexpr bool IS_SKILL_ITEM(BYTE x) { return x >= ITEMKIND_SKILL_ATTACK && x <= ITEMKIND_SKILL_ATTRIBUTE; }

constexpr bool IS_MONSTER_ITEM(BYTE x) { return x >= ITEMKIND_FOR_MON_PRIMARY && x <= ITEMKIND_FOR_MON_MAIN_ARMAMENT; } // 2011-05-13 by hskim, 인피니티 3차
constexpr bool IS_PRIMARY_WEAPON_MONSTER(BYTE x) { return x >= ITEMKIND_FOR_MON_PRIMARY && x <= ITEMKIND_FOR_MON_RANGE_ATTACK; }		// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 몬스터 스킬 아이템 Kind 추가
constexpr bool IS_SECONDARY_WEAPON_MONSTER(BYTE x) { return x >= ITEMKIND_FOR_MON_SECONDARY && x <= ITEMKIND_FOR_MON_MAIN_ARMAMENT; } // 2011-05-13 by hskim, 인피니티 3차
constexpr bool IS_VALID_ITEMKIND(BYTE x) { return x >= ITEMKIND_AUTOMATIC && x <= ITEMKIND_ALL_ITEM_END; }		// Itemkind 값 확인 - // 2012-12-14 by hskim, 메모리핵 자동 블럭 기능 구현


constexpr auto COUNT_BUNCH_COUNTABLE_ITEM_UNIT = 10;

constexpr bool IS_COUNTABLE_ITEM(BYTE _ITEM_KIND)
{
	return

		_ITEM_KIND == ITEMKIND_ENERGY ||
		_ITEM_KIND == ITEMKIND_INGOT ||
		_ITEM_KIND == ITEMKIND_CARD ||
		_ITEM_KIND == ITEMKIND_ENCHANT ||
		_ITEM_KIND == ITEMKIND_BULLET ||
		_ITEM_KIND == ITEMKIND_QUEST ||
		_ITEM_KIND == ITEMKIND_GAMBLE ||
		_ITEM_KIND == ITEMKIND_PREVENTION_DELETE_ITEM ||
		_ITEM_KIND == ITEMKIND_INFLUENCE_BUFF ||
		_ITEM_KIND == ITEMKIND_INFLUENCE_GAMEEVENT;
}
constexpr bool IS_SPECIAL_COUNTABLE_ITEM(BYTE _ITEM_KIND)
{
	return

		_ITEM_KIND == ITEMKIND_CARD ||
		_ITEM_KIND == ITEMKIND_ENCHANT ||
		_ITEM_KIND == ITEMKIND_GAMBLE ||
		_ITEM_KIND == ITEMKIND_PREVENTION_DELETE_ITEM;
}

constexpr bool COMPARE_ITEMKIND(BYTE _REQ_ITEM_KIND, BYTE _TARGET_KIND_VAR)
{
	return ((_REQ_ITEM_KIND == ITEMKIND_ALL_ITEM) || (_REQ_ITEM_KIND == _TARGET_KIND_VAR)
		|| ((_REQ_ITEM_KIND == ITEMKIND_ALL_WEAPON) && IS_WEAPON(_TARGET_KIND_VAR))
		|| ((_REQ_ITEM_KIND == ITEMKIND_PRIMARY_WEAPON_ALL) && IS_PRIMARY_WEAPON(_TARGET_KIND_VAR))
		|| ((_REQ_ITEM_KIND == ITEMKIND_PRIMARY_WEAPON_1) && IS_PRIMARY_WEAPON_1(_TARGET_KIND_VAR))
		|| ((_REQ_ITEM_KIND == ITEMKIND_PRIMARY_WEAPON_2) && IS_PRIMARY_WEAPON_2(_TARGET_KIND_VAR))
		|| ((_REQ_ITEM_KIND == ITEMKIND_SECONDARY_WEAPON_ALL) && IS_SECONDARY_WEAPON(_TARGET_KIND_VAR))
		|| ((_REQ_ITEM_KIND == ITEMKIND_SECONDARY_WEAPON_1) && IS_SECONDARY_WEAPON_1(_TARGET_KIND_VAR))
		|| ((_REQ_ITEM_KIND == ITEMKIND_DEFENSE) && (ITEMKIND_DEFENSE == _TARGET_KIND_VAR))
		|| ((_REQ_ITEM_KIND == ITEMKIND_SECONDARY_WEAPON_2) && IS_SECONDARY_WEAPON_2(_TARGET_KIND_VAR)));
}

constexpr bool IS_CHARGABLE_ITEM(BYTE x)
{
	return
		IS_WEAPON(x) ||
		x == ITEMKIND_TANK ||
		x == ITEMKIND_ACCESSORY_UNLIMITED ||
		x == ITEMKIND_ACCESSORY_TIMELIMIT;
}

// 2007-08-29 by cmkwon, 조합시 아머도 무기와 같이 레어/인챈트 정보 유지하기 - 
constexpr bool IS_ITEMKIND_REMAIN_RARE8ENCHANT_AS_MIX(BYTE x)
{
	return IS_WEAPON(x) || x == ITEMKIND_DEFENSE;
}

// 2009-02-17 by cmkwon, 인챈트 할 수 있는 아이템 카인드 수정 - 무기, 아머, 레이더, 엔진
constexpr bool IS_ENCHANT_TARGET_ITEMKIND(BYTE _IT_) { return (IS_WEAPON(_IT_) || ITEMKIND_DEFENSE == (_IT_) || ITEMKIND_RADAR == (_IT_) || ITEMKIND_SUPPORT == (_IT_) || ITEMKIND_ACCESSORY_UNLIMITED == (_IT_)); }	// 2009-09-09 ~ 2010-02-10 by dhjin, 인피니티 - 발동류장착아이템

// 2009-09-09 ~ 2010-02-10 by dhjin, 인피니티 - 발동류장착아이템																																																						// 2010-04-20 by cmkwon, 신규 러키 머신 구현 - 아머(ITEMKIND_DEFENSE)도 추가
constexpr bool IS_RARE_TARGET_ITEMKIND(BYTE _IT_)
{
	return (IS_WEAPON(_IT_) || ITEMKIND_DEFENSE == (_IT_));
}	

// 2009-08-26 by cmkwon, 그래픽 리소스 변경 시스템 구현 - 
// - 외형 변경 가능 Kind 총 5종, 1형/2형 무기, 마크, 아머, 레이더, 파트너
// - 아이템의 이펙트 변경 가능 Kind 총 2종, 1형/2형 무기
constexpr bool IS_ENABLE_CHANGE_ShapeItemNum(BYTE _ITEM_KIND) { return (IS_WEAPON(_ITEM_KIND) || ITEMKIND_MARK == (_ITEM_KIND) || ITEMKIND_DEFENSE == (_ITEM_KIND) || ITEMKIND_RADAR == (_ITEM_KIND) || ITEMKIND_PET_ITEM == (_ITEM_KIND)); }		// 2012-12-03 by hskim, 파트너 기간제 외형 변경
constexpr bool IS_ENABLE_CHANGE_EffectItemNum(BYTE _ITEM_KIND) { return (IS_WEAPON(_ITEM_KIND)); }
