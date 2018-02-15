//Copyright[2002] MasangSoft
///////////////////////////////////
// 아이템의 캐릭터 파라이터 - DES_XXX
#pragma once
enum : DestParam_t
{
	DES_NULL = 0,		// 대상 파라미터가 없는 경우 사용
	DES_ATTACK_PART = 1,		// 공격 파트
	DES_DEFENSE_PART = 2,		// 방어 파트
	DES_FUEL_PART = 3,		// 연료 파트
	DES_SOUL_PART = 4,		// 감응 파트
	DES_SHIELD_PART = 5,		// 쉴드 파트
	DES_DODGE_PART = 6,		// 회피 파트
	DES_ALL_PART = 106,		// 모든 스텟	// 2013-05-31 by jhseol,bckim 아머 컬렉션 - 모든 스텟 DES_ALL_PART 추가
	DES_BODYCONDITION = 7,		// 몸상태
	DES_ENDURANCE_01 = 8,		// 내구도 01
	DES_ENDURANCE_02 = 9,		// 내구도 02
	DES_CHARGING_01 = 10,		// 장탄수 01
	DES_CHARGING_02 = 11,		// 장탄수 02
	DES_PROPENSITY = 12,		// 성향
	DES_HP = 13,		// 히트포인트, MAX HP를 +VALUE만큼 올려줌
	DES_DP = 89,		// 쉴드(DEFENSE)포인트
	DES_SP = 14,		// 소울포인트
	DES_EP = 15,		// 엔진포인트
	DES_SPRECOVERY = 16,		// 소울포인트회복력
	DES_HPRECOVERY = 17,		// 에너지포인트회복력
	DES_MINATTACK_01 = 18,		// (*) 최소 공격력 01
	DES_MAXATTACK_01 = 71,		// (*) 최대 공격력 01
	DES_MINATTACK_02 = 19,		// (*) 최소 공격력 02
	DES_MAXATTACK_02 = 72,		// (*) 최대 공격력 02
	DES_ATTACKPROBABILITY_01 = 20,		// 공격확률 01
	DES_ATTACKPROBABILITY_02 = 21,		// 공격확률 02
	DES_DEFENSE_01 = 22,		// (*) 방어력 01 (데미지 감소 확률) 20040622 추가
	DES_DEFENSE_02 = 23,		// (*) 방어력 02 (데미지 감소 확률) 20040622 추가
	DES_DEFENSEPROBABILITY_01 = 24,		// 방어확률 01 (회피 확률)
	DES_DEFENSEPROBABILITY_02 = 25,		// 방어확률 02 (회피 확률)
	DES_SKILLPROBABILITY_01 = 26,		// 스킬공격확률 01
	DES_SKILLPROBABILITY_02 = 64,		// 스킬공격확률 02
	DES_FACTION_01 = 79,		// 속성 01, check: 추가됨
	DES_FACTION_02 = 80,		// 속성 02, check: 추가됨
	DES_FACTIONRESISTANCE_01 = 27,		// 속성저항력 01
	DES_FACTIONRESISTANCE_02 = 65,		// 속성저항력 02
	DES_SPEED = 28,		// (*) 이동속도, FIXER 등의 속도 감소량
	DES_TRANSPORT = 29,		// 운반력
	DES_MATERIAL = 30,		// 재질
	DES_REATTACKTIME_01 = 31,		// (*) 리어택타임 01 (- 증가)
	DES_REATTACKTIME_02 = 32,		// (*) 리어택타임 02 (- 증가)
	DES_ABRASIONRATE_01 = 33,		// 마모율 01
	DES_ABRASIONRATE_02 = 34,		// 마모율 02
	DES_RANGE_01 = 35,		// (*) 유효거리 01
	DES_RANGE_02 = 36,		// (*) 유효거리 02
	DES_RANGEANGLE_01 = 37,		// 유효각도 01
	DES_RANGEANGLE_02 = 38,		// 유효각도 02
	DES_MULTITAGET_01 = 39,		// 멀티타겟 01
	DES_MULTITAGET_02 = 66,		// 멀티타겟 02
	DES_EXPLOSIONRANGE_01 = 40,		// 폭발반경 01
	DES_EXPLOSIONRANGE_02 = 67,		// 폭발반경 02
	DES_UNIT = 41,		// 유닛의 종류 (28 ~ 29이 같이 쓰여 유닛마다의 보정값으로 사용됨)
	DES_REVISION = 42,		// 유닛의 보정값 (28 ~ 29이 같이 쓰여 유닛마다의 보정값으로 사용됨)
	DES_FACTIONPROBABILITY_01 = 43,		// 속성에 대한 방어확률 01
	DES_FACTIONPROBABILITY_02 = 68,		// 속성에 대한 방어확률 02
	DES_SHOTNUM_01 = 44,		// 일점사 당 발사 수 01
	DES_SHOTNUM_02 = 69,		// 일점사 당 발사 수 02
	DES_MULTINUM_01 = 45,		// 동시 발사 수 01
	DES_MULTINUM_02 = 70,		// 동시 발사 수 02
	DES_ATTACKTIME_01 = 46,		// 처음 공격 시의 타임 01 (- 증가)
	DES_ATTACKTIME_02 = 47,		// 처음 공격 시의 타임 02 (- 증가)
	DES_TIME_01 = 48,		// (*) 지속 시간 01, check: + -> *
	DES_TIME_02 = 49,		// (*) 지속 시간 02, check: + -> *
	DES_OVERHITTIME_01 = 73,		// (*) 오버힛회복시간 01, check: 추가됨
	DES_OVERHITTIME_02 = 74,		// (*) 오버힛회복시간 02, check: 추가됨
	DES_UNITKIND = 50,		// 기체 업그레이드시 해당 기체
	DES_ITEMKIND = 51,		// 아이템의 종류(ITEMKIND_XXX)
	DES_SUMMON = 52,		// 몬스터 소환
	DES_GRADUAL_HP_UP = 53,		// 에너지류, 현재 HP를 일정 시간(ITEM.Time에 저장)동안 점진적으로 VALUE만큼 올려줌, 중간에 부스터 켜거나 공격 받으면 cancel
	DES_GRADUAL_DP_UP = 81,		// 에너지류, 현재 DP를 일정 시간(ITEM.Time에 저장)동안 점진적으로 VALUE만큼 올려줌, 중간에 부스터 켜거나 공격 받으면 cancel, check: 추가됨
	DES_GRADUAL_SP_UP = 54,		// 에너지류, 현재 SP를 일정 시간(ITEM.Time에 저장)동안 점진적으로 VALUE만큼 올려줌, 중간에 뭐(?)들어오면 cancel
	DES_GRADUAL_EP_UP = 55,		// 에너지류, 현재 EP를 일정 시간(ITEM.Time에 저장)동안 점진적으로 VALUE만큼 올려줌, 중간에 뭐(?)들어오면 cancel
	DES_IN_TIME_HP_UP = 56,		// 에너지류, 현재 HP를 일정 시간(ITEM.Time에 저장)이 지난 후 VALUE만큼 올려줌
	DES_IN_TIME_DP_UP = 82,		// 에너지류, 현재 DP를 일정 시간(ITEM.Time에 저장)이 지난 후 VALUE만큼 올려줌, check: 추가됨
	DES_IN_TIME_SP_UP = 57,		// 에너지류, 현재 SP를 일정 시간(ITEM.Time에 저장)이 지난 후 VALUE만큼 올려줌
	DES_IN_TIME_EP_UP = 58,		// 에너지류, 현재 EP를 일정 시간(ITEM.Time에 저장)이 지난 후 VALUE만큼 올려줌
	DES_IMMEDIATE_HP_UP = 59,		// 에너지류, 현재 HP를 즉시 VALUE만큼 올려줌
	DES_IMMEDIATE_DP_UP = 83,		// 에너지류, 현재 DP를 즉시 VALUE만큼 올려줌, check: 추가됨
	DES_IMMEDIATE_SP_UP = 60,		// 에너지류, 현재 SP를 즉시 VALUE만큼 올려줌
	DES_IMMEDIATE_EP_UP = 61,		// 에너지류, 현재 EP를 즉시 VALUE만큼 올려줌
	DES_GROUNDMODE = 62,		// 폭격모드
	DES_SIEGEMODE = 63,		// 시즈모드
	DES_WEIGHT_01 = 75,		// (*) 무게 01, check: 추가됨
	DES_WEIGHT_02 = 76,		// (*) 무게 02, check: 추가됨
	DES_BULLET_01 = 77,		// (*) 1형 총알 01, 2005-11-02 by cmkwon 빔소모초알량을 변경함
	DES_BULLET_02 = 78,		// (*) 2형 총알 02, 2005-11-02 by cmkwon 빔소모초알량을 변경함
	DES_PRIMARY_WEAPON = 84,		// 1형 무기, 범용빔
	DES_SECONDARY_WEAPON = 85,		// 2형 무기, 범용
	DES_ALL_WEAPON = 86,		// 1,2형 무기 모두, 범용
	DES_CRITICALHITRATE_01 = 87,		// (*)크리티컬 확률 20040622 추가
	DES_CRITICALHITRATE_02 = 88,		// (*)크리티컬 확률 20040622 추가
	DES_WARP = 90,		// 도시워프 아이템용
	DES_REACTION_RANGE = 91,		// ITEM의 ReactionRange 변경
	DES_RARE_FIX_NONE = 92,		// 접두사, 접미사 모두 없음, ITEMKIND_GAMEBLE의 DestParam1에 설정
	DES_RARE_FIX_PREFIX = 93,		// 접두사, ITEMKIND_GAMEBLE의 DestParam1에 설정
	DES_RARE_FIX_SUFFIX = 94,		// 접미사, ITEMKIND_GAMEBLE의 DestParam1에 설정
	DES_RARE_FIX_BOTH = 95,		// 접두사, 접미사 모두 포함, ITEMKIND_GAMEBLE의 DestParam1에 설정
	DES_REQ_MIN_LEVEL = 96,		// 아이템장착 요구 MinLevel을 낮춘다
	DES_REQ_MAX_LEVEL = 97,		// 아이템장착 요구 MaxLevel을 낮춘다
	DES_WARP_OUTPOST = 98,		// 2007-09-05 by dhjin, 전진기지 도시워프 아이템용
	DES_CHAT_BLOCK = 99,		// 2008-12-30 by cmkwon, 지도자 채팅 제한 카드 구현 - 

	DES_CASH_STAT_ALL_INITIALIZE = 100,		// 유료 모든 스탯 초기화 아이템
	//DES_CASH_STAT_HALF_INITIALIZE = 101,		// (구현 미확정)유료 50% 스탯 초기화 아이템
	DES_CASH_STAT_PART_INITIALIZE = 102,		// 유료 부분 스탯 초기화 아이템
	DES_RARE_FIX_PREFIX_INITIALIZE = 103,		// 접두사 초기화, ITEMKIND_GAMEBLE의 DestParam1에 설정
	DES_RARE_FIX_SUFFIX_INITIALIZE = 104,		// 접미사 초기화, ITEMKIND_GAMEBLE의 DestParam1에 설정
	DES_ENCHANT_INITIALIZE = 105,		// 2007-04-03 by cmkwon, 인챈트 초기화, ITEMKIND_ENCHANT의 DestParam1에 설정
	//											106		// 모든 스텟(DES_ALL_PART)에서 사용	// 2013-05-31 by jhseol,bckim 아머 컬렉션 - 모든 스텟 DES_ALL_PART 추가
	DES_CASH_STEALTH = 108,		// 스텔스 아이템
	DES_CASH_HP_AND_DP_UP = 109,		// HP and DP UP 아이템
	DES_CASH_GUILD_ALL_MEMBERS_SUMMON = 110,		// 모든 여단원 소환 - 여단장만 사용가능
	DES_CASH_GUILD_MEMBER_SUMMON = 111,		// 여단원 1명 소환 - 여단장만 사용가능
	DES_CASH_NORMAL_RESTORE = 112,		// 일반 부활 카드 - 사
	DES_CASH_SUPER_RESTORE = 113,		// 슈퍼 부활 카드
	DES_CASH_GUILD = 114,		// 일반/고급 여단 카드
	DES_CASH_MONSTER_SUMMON = 115,		// 몬스터 소환 카드
	DES_CASH_CHANGE_CHARACTERNAME = 116,		// 캐릭터명 변경 카드
	DES_CASH_SKILL_INITIALIZE = 117,		// 스킬 초기화 카드
	DES_CASH_CHANGE_PILOTFACE = 118,		// 얼굴 변경 카드
	DES_CASH_CRACKER_CARD = 119,		// 종합 폭죽 카드


	///////////////////////////////////////////////////////////////////////////////
	// 2005-11-21 by cmkwon, 새로 추가된 DesParam
	DES_SKILL_REDUCE_SHIELD_DAMAGE = 121,		// 쉴드에 가는 데미지를 줄여준다.
	DES_SKILL_INVINCIBLE = 122,		// 무적 상태
	DES_SKILL_BIG_BOOM = 123,		// 		미구현 - 자신의 HP를 모두 소모하여 주변에게 데미지를 입히고, 자신은 폭발한다.
	DES_SKILL_HALLUCINATION = 124,		// 체프 사출.
	DES_SKILL_RANGEDOWN_01 = 125,		// 		미구현 - 적의 기본무기 사거리를 줄인다.
	DES_SKILL_RANGEDOWN_02 = 126,		// 		미구현 - 적의 고급 무기 사거리를 줄인다.
	DES_SKILL_PROHIBITION_01 = 127,		// 		미구현 - 적의 기본 무기 사용을 금지시킨다.
	DES_SKILL_PROHIBITION_02 = 128,		// 		미구현 - 적의 고급 무기 사용을 금지시킨다.
	DES_ATTACK_RANGE_01 = 129,		// 레이다의 기본 무기 조준 거리를 증가시킨다.(*)
	DES_ATTACK_RANGE_02 = 130,		// 레이다의 고급 무기 조준 거리를 증가시킨다.(*)
	DES_INVISIBLE = 131,		//	스텔스 모드로서 적에게 보이지 않으며, 레이다에도 잡히지 않는다. 자신도 공격이 되지 않는다.
	// 2005-12-02 by cmkwon, 154, 155, 156으로 나눔DES_SKILL_HYPERMOVING = 132,		// 유닛의 전체 이동속도가 좋아지고, 부스터 사용이 되지 않는다.(*)
	DES_SKILL_DEFENSE_01 = 133,		// 		미구현 - 적의 기본방어력을 떨어뜨린다.(*)
	DES_SKILL_DEFENSE_02 = 134,		// 		미구현 - 적의 고급방어력을 떨어뜨린다.(*)
	DES_SKILL_FREEZINGBODY = 135,		// 		미구현 - 적의 HP,쉴드 회복을 불가능하게 만든다. 수리킷,쉴드킷,스킬회복(전체)이 안된다.
	DES_SKILL_REVERSECONTROL = 136,		// 2010-03-31 by dhjin, 인피니티(기지방어) -	// 		미구현 - 적의 움직임을 반대로 움직이게 한다.
	DES_SKILL_GRADUAL_HPSHIELD_DOWN = 137,		// 		미구현 - 적의 HP,쉴드를 점차 감소시킨다.
	DES_SKILL_SLOWMOVING = 138,		// 적의 움직임을 둔하게 한다.(최대이동 속도, 부스터 속도, 이동 각도, 부스터 이동 각도)(*)
	DES_SKILL_BOOSTEROFF = 139,		// 		미구현 - 적의 부스터 사용을 금지시킨다.
	DES_SKILL_COLLISIONDAMAGE_DOWN = 140,		// 배경 오브젝트,바닥 충돌시 데미지를 감소시킨다.
	//DES_SKILL_CAMOUFLAGE = 141,		// 		미구현 - 위장하여 적의 시야에 보이지 않는다. 타겟에는 잡힌다.
	DES_SKILL_RANDOMTELEPORT = 142,		// 		미구현 - 같은 맵에서 랜덤하게 이동한다.
	DES_SKILL_ATTACK_ALL = 143,		// 		미구현 - 전체 공격 스킬
	DES_SKILL_SCANNING = 144,		// Invisible 상태의 유저를 볼 수 있도록 한다
	DES_SKILL_REVERSEENGINE = 145,		// 엔진이 후진이 가능하게 한다.
	DES_SKILL_LOCKDOWN = 146,		// 		미구현 - 상대방을 일시 정지시킨다.(전진,좌우선회,부스터 정지, 단, 공격기능은 가능)
	DES_SKILL_STEALSP = 147,		// 		미구현 - 상대방의 SP를 일정량 P아온다.
	DES_SKILL_SMARTSP = 148,		// 자신이 사용하는 모든 스킬의 사용량을 줄여준다.(*)
	DES_SKILL_SUMMON_FORMATION_MEMBER = 149,		// 편대원 중 1명을 소환한다.
	DES_SKILL_CANCEL_MAGIC = 150,		// 		미구현 - 적의 버프 스킬을 모두 해제시킨다.
	DES_SKILL_RANDOM_CANCEL = 151,		// 		미구현 - 타겟의 디버프 스킬을 하나 랜덤하게 해제한다.
	DES_SKILL_STOPMAGIC = 152,		// 적의 스킬을 일정시간 사용금지시킨다. - 2011-10-28 by hskim, EP4 [트리거 시스템] - 크리스탈 시스템
	DES_SKILL_CANCELALL = 153,		// 		미구현 - 대상의 버프 스킬을 모두 해제한다.
	DES_SKILL_REACTIONSPEED = 154,		// (*)하이퍼 무빙(반응속도)
	DES_SKILL_ENGINEANGLE = 155,		// (*)하이퍼 무빙(선회각)
	DES_SKILL_ENGINEBOOSTERANGLE = 156,		// (*)하이퍼 무빙(부스터 선회각)

	// 2006-03-30 by cmkwon
	DES_DROP_EXP = 157,		// 경험치, 프리미엄과 중첩 불가
	DES_DROP_SPI = 158,		// SPI, 프리미엄과 중첩 불가
	DES_DROP_ITEM = 159,		// 아이템 드랍율, 프리미엄과 중첩 불가
	DES_HP_REPAIR_RATE_FLIGHTING = 160,		// 비행시 HP 회복율
	DES_DP_REPAIR_RATE = 161,		// DP 회복율
	DES_SP_REPAIR_RATE = 162,		// SP 회복율

	// 2006-07-26 by cmkwon
	DES_BAZAAR_SELL = 163,		// 2006-07-26 by cmkwon, 개인 판매 상점
	DES_BAZAAR_BUY = 164,		// 2006-07-26 by cmkwon, 개인 구매 상점

	// 2006-08-14 by dhjin
	DES_KILLMARK_EXP = 165,		// 2006-08-14 by dhjin, 킬마크 경험치

	// 2006-10-11 by cmkwon
	DES_HYPER_BOOSTER = 166,		// 2006-10-11 by cmkwon, 부스터 게이지 떨어지지 않음

	// 2006-11-17 by dhjin, 2차 스킬로 인해 추가된 사항
	DES_SKILL_CHAFF_HP = 167,		// 2006-11-17 by dhjin, 체프의 HP
	DES_SKILL_AIR_BOMBING = 168,		// 2006-11-17 by dhjin, 공중 폭격
	DES_SKILL_NO_WARNING = 169,		// 2006-11-17 by dhjin, 타기어에 스킬 사용시, 해당기어는 워닝음과 레이더 점멸이 울리지 않게 된다
	DES_SKILL_ROLLING_TIME = 170,		// 2006-11-17 by dhjin, 30초동안 롤링 재사용 시간이 없다.
	DES_SKILL_FULL_RECOVERY = 171,		// 2006-11-17 by dhjin, 기체의 HP, 쉴드, 연료, SP를 완전회복한다.
	DES_SKILL_CAMOUFLAGE = 172,		// 2006-11-17 by dhjin, A기어의 위장 스킬
	DES_SKILL_BARRIER = 173,		// 2006-11-17 by dhjin, A기어의 고급무기에 대한 무적 스킬
	DES_SKILL_HYPERSHOT = 174,		// 2006-11-17 by dhjin, A기어의 기본무기 차징샷, 스플래쉬 데미지는 Range
	DES_SKILL_SHIELD_PARALYZE = 175,		// 2006-11-17 by dhjin, A기어의 쉴드마비 스킬, 상대기어의 쉴드 회복율을 0으로 만든다.

	DES_WARHEAD_SPEED = 176,		// 2007-06-11 by cmkwon, 탄두의 속도
	DES_CHAT_ALL_INFLUENCE = 177,		// 2007-08-09 by cmkwon, 모든 세력에 채팅 전송하기 - desparam 추가, 유료아이템

	// 2008-09-22 by dhjin, 신규 인첸트
	DES_ENGINE_BOOSTER_TIME_UP = 178,		// 2008-09-22 by dhjin, 부스터 시간 증가
	DES_ENGINE_MAX_SPEED_UP = 179,		// 2008-09-22 by dhjin, 엔진 일반속도(최대) 증가
	DES_ENGINE_MIN_SPEED_UP = 180,		// 2008-09-22 by dhjin, 엔진 일반속도(최소) 증가
	DES_ENGINE_BOOSTER_SPEED_UP = 181,		// 2008-09-22 by dhjin, 엔진 부스터속도 증가
	DES_ENGINE_GROUND_SPEED_UP = 182,		// 2008-09-22 by dhjin, 엔진 지상속도 증가
	DES_RADAR_OBJECT_DETECT_RANGE = 183,		// 2008-09-22 by dhjin, 레이더 물체 감지 반경
	DES_PIERCE_UP_01 = 184,		// 2008-09-22 by dhjin, 기본무기 피어스율 증가 카드
	DES_PIERCE_UP_02 = 185,		// 2008-09-22 by dhjin, 고급무기 피어스율 증가 카드
	DES_ENGINE_ANGLE_UP = 186,		// 2008-09-30 by dhjin, 엔진 회전각 증가 카드
	DES_ENGINE_BOOSTERANGLE_UP = 187,		// 2008-09-30 by dhjin, 엔진 부스터 회전각 증가 카드

	// 2009-01-05 by dhjin, 미션마스터 - 편대 버프 아이템 추가 
	DES_ITEM_BUFF_INFLUENCE = 188,
	DES_ITEM_BUFF_PARTY = 189,

	// 2009-01-19 by dhjin, 인첸트 확률 증가, 10인첸 파방 카드 - 인첸트 확률 증가 카드
	DES_ENCHANT_PREVENTION_DELETE_USE_ENCHANT = 190,		// 사용 하기 위한 아이템의 보유인챈트 카운트 설정
	DES_ENCHANT_PREVENTION_DELETE_SAVE_ENCHANT = 191,		// 인챈트 실패시 남기는 인챈트 카운트 설정
	DES_ENCHANT_INCREASE_PROBABILITY = 192,		// 인챈트 카드 확률 증가분 설정

	// 2009-08-03 by cmkwon, EP3-4 편대 대형 스킬 구현 - DES_ 추가
	DES_SKILL_DAMAGE_DISTRIBUTION = 193,		// 데미지를 편대원에게 분산(분배) 처리 한다.

	// 2009-08-26 by cmkwon, 그래픽 리소스 변경 시스템 구현 - 
	DES_SHAPE_ITEM = 194,		// 조합시 소스 아이템의 ShapeItemNum을 변경 할 Item, LinkItem을 사용		(Value => KIND_ITEM_FIX_MIXING_WEAPON / KIND_ITEM_FIX_MIXING_DEFENSE)		// 2012-02-20 by hskim, DB 정규화 - ItemMixingInfo
	DES_EFFECT_ITEM = 195,		// 조합시 소스 아이템의 EffectItemNum을 변경 할 Item, 해당 ItemNum을 사용	(Value => KIND_ITEM_FIX_MIXING_WEAPON / KIND_ITEM_FIX_MIXING_DEFENSE)		// 2012-02-20 by hskim, DB 정규화 - ItemMixingInfo

	// 2009-09-23 by cmkwon, 필드창고 캐쉬 아이템 구현 - 
	DES_FIELD_STORE = 196,		// 필드창고

	// 2009-10-01 by cmkwon, 그래픽 리소스 변경 관련 초기화 기능 구현 - 
	DES_INIT_SHAPE_ITEM = 197,		// 조합시 소스 아이템의 ShapeItemNum을 초기화 할 Item		(Value => KIND_ITEM_FIX_MIXING_WEAPON / KIND_ITEM_FIX_MIXING_DEFENSE)		// 2012-02-20 by hskim, DB 정규화 - ItemMixingInfo
	DES_INIT_EFFECT_ITEM = 198,		// 조합시 소스 아이템의 EffectItemNum을 초기화 할 Item		(Value => KIND_ITEM_FIX_MIXING_WEAPON / KIND_ITEM_FIX_MIXING_DEFENSE)		// 2012-02-20 by hskim, DB 정규화 - ItemMixingInfo

	// 2009-11-02 by cmkwon, 캐쉬(인벤/창고 확장) 아이템 추가 구현 - DES_XXX 추가
	DES_INCREASE_INVENTORY_SPACE = 199,		// 캐릭터 인벤토리 증가
	DES_INCREASE_STORE_SPACE = 200,		// 캐릭터 창고 증가

	// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 
	DES_ITEM_RESISTANCE = 201,		// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 저항 아이템 사용 체크 
	DES_ITEM_ADDATTACK = 202,		// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 절대값 타격치 아이템 사용 체크 
	DES_ITEM_IGNOREDEFENCE = 203,		// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 방어력 무시 아이템 사용 체크
	DES_ITEM_IGNOREAVOID = 204,		// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 회피력 무시 아이템 사용 체크
	DES_ITEM_REDUCEDAMAGE = 205,		// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 데미지 절대값 감소 아이템 사용 체크
	DES_ITEM_ADDATTACK_SEC = 206,		// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 절대값 타격치 아이템 고급무기용(기무와 분류)
	DES_ITEM_ONCE_RESISTANCE = 207,		// 2009-09-09 ~ 2010-01-19 by dhjin, 인피니티 - 한 번만 저항하고 없어지는 저항 아이템 추가
	DES_SKILL_MON_SILENCE_PRIMARY = 210,		// 몬스터 사일런스 스킬 (1형무기 적용)
	DES_SKILL_MON_SILENCE_SECOND = 211,		// 몬스터 사일런스 스킬 (2형무기 적용)
	DES_SKILL_MON_FREEZE_HP = 212,		// 몬스터 프리즈 스킬 HP 회복 불가  
	DES_SKILL_MON_FREEZE_DP = 213,		// 몬스터 프리즈 스킬 DP 회복 불가
	DES_SKILL_MON_FREEZE_SP = 214,		// 몬스터 프리즈 스킬 SP 회복 불가
	DES_SKILL_MON_HOLD = 215,		// 몬스터 홀드 스킬
	DES_SKILL_MON_STEALING = 216,		// 몬스터 스틸링 스킬 
	DES_SKILL_MON_DRAIN = 217,		// 몬스터 드레인 스킬
	DES_SKILL_RELEASE = 218,		// M기어 릴리즈 스킬
	DES_SKILL_MON_SILENCE_SKILL = 219,		// 몬스터 사일런스 스킬 (스킬 적용)

	// 2009-09-09 ~ 2010-02-10 by dhjin, 인피니티 - 발동류장착아이템
	DES_PAIR_DRAIN_1_RATE = 220,		// 드레인 발동 확률
	DES_PAIR_DRAIN_2_HP_DP_UP_RATE = 221,		// 드레인 회복량 HP->DP 회복 (데미지의 %)
	DES_ANTI_DRAIN_RATE = 222,		// 드레인 발동 저항 확률
	DES_PAIR_REFLECTION_1_RATE = 223,		// 데미지 반사 확률
	DES_PAIR_REFLECTION_2_DAMAGE_RATE = 224,		// 반사 데미지량 %
	DES_ANTI_REFLECTION_RATE = 225,		// 데미지 반사 저항 확률

	// 2010-03-31 by dhjin, 인피니티(기지방어) - 
	DES_BLIND = 226,
	DES_SUPERINTEND = 227,
	DES_IMMEDIATE_HP_OR_DP_UP = 228,		// HP를 먼저 채우고 그 이후에 DP를 채운다.
	DES_HIT_INVOKE_SKILL = 229,		// 명중시 해당 데스파람값에 스킬을 시전한다.
	DES_TIME_BOMB = 230,

	// 2010-03-18 by cmkwon, 몬스터변신 구현 - 
	DES_TRANSFORM_TO_MONSTER = 231,		// ParamValue에 MonsterUnitKind 입력

	// 2010-03-23 by cmkwon, 인피니티 입장 캐쉬 아이템 구현 - 1회만 추가 입장 가능
	DES_INFINITY_REENTRY_TICKET = 232,		// 현재는 ParamValue는 사용하지 않음

	// 2010-04-05 by cmkwon, 몬스터변신관련 해제 카드 구현 - 
	DES_TRANSFORM_TO_GEAR = 233,		// 현재는 ParamValue는 사용하지 않음.

	// 2010-05-18 by cmkwon, WarPoint 증가 아이템 구현(일본요청) - 
	DES_PLUS_WARPOINT_RATE = 234,		// ParamValue: WarPoint 증가 Rate

	// 2010-06-01 by shcho, PC방 권한 획득(캐쉬) 아이템 - 
	DES_PCROOM_USE_CARD = 235,		//PC방 플래그를 TRUE로 변환 시킨다.

	// 2010-08-26 by shcho&jsKim, 밤 아이템 구현 -
	DES_MAX_SP_UP = 236,		// SP의 최대치를 증가시킨다.

	// 2010-08-27 by shcho&&jskim, WARPOINT 증가 아이템 구현
	DES_WAR_POINT_UP = 237,		// WARPOINT 증가

	// 2010-08-27 by shcho&&jskim, WARPOINT 증가 아이템 구현
	DES_DONATE_POINT_UP = 389,		// WARPOINT 증가 //are u here?yes

	// 2010-11-30 by shcho, 레어아이템 드랍확률 증가 아이템 구현
	DES_RARE_ITEM_DROP_RATE = 238,		// 레어 아이템 드랍확률 증가

	// 2010-12-21 by jskim, 마을 이동 속도 증가 아이템 구현
	DES_RARE_ITEM_PARTNER_SPEED = 239,		// 마을 이동 스피드 증가

	// 2010-12-21 by jskim, 파트너 데미지 증가 아이템 구현
	DES_RARE_ITEM_PARTNER_DAMAGE = 240,		// 파트너 데미지 증가

	// 2010-12-21 by jskim, HP, DP 키트 적용량 증가 아이템 구현
	DES_RARE_ITEM_HPDP = 241,		// HP DP 키트 적용량

	// 2011-05-02 by hskim, 인피니티 3차 - 시네마 관련 기능 추가 - 특정 좌표에 소환
	DES_SUMMON_POSITION_X = 242,		// 소환시 위치 변경 (상대값)
	DES_SUMMON_POSITION_Y = 243,		// 소환시 위치 변경 (상대값)
	DES_SUMMON_POSITION_Z = 244,		// 소환시 위치 변경 (상대값)

	// 2011-10-18 by hskim, EP4 [트리거 시스템] - 화산재 / 모래 폭풍
	DES_MAPBUFF_RANDOM_ADD_REATTACKTIME = 245,		// 맵버프 추가 시간 적용
	DES_MAPBUFF_RANDOM_ADD_TIME = 246,		// 맵버프 추가 시간 적용
	DES_ENCHANT_ONCE_APPLY = 247,		// 2011-10-19 by hskim, EP4 [Free to play] - 10회 인첸트 아이템 기능 구현
	DES_GAMBLE_RARE_ITEM_FIX = 248,		// 2011-10-20 by hskim, EP4 [Free to play] - 접두/접미 고정 옵션

	DES_GAMBLE_RARE_ITEM_FIX_STD = 249,
	DES_GAMBLE_RARE_ITEM_FIX_ADV = 250,

	// 2013-05-07 by jhseol,bckim 버프 패널티
	DES_BUFF_PENALTY_RATIO = 251,		// 패널티 비율 (1.0f = 100%)
	DES_BUFF_PENALTY_APPLIED_UNIT_KIND = 252,		// 패널티 적용 기어 (ReqUnitKind)

	// 2013-05-09 by hskim, 세력 포인트 개선
	DES_SKILLTYPE_CONSECUTIVE_VICTORIES = 253,		// 세력 포인트 개선 - 연승의 버프류 종류
	DES_SKILLTYPE_TRUN_AROUND = 254,		// 세력 포인트 개선 - 역전의 버프류 종류

	// 2015-08-02 by killburne
	DES_SKILLTYPE_OUTPOST_BUFF = 255,

	// 2013-05-09 by hskim, 세력 포인트 개선
	DES_SKILL_BUFF_MON_ATTACK_POWER = 300,		// 몬스터 공격시 - 공격력 증가 : Value 증가 %
	DES_SKILL_BUFF_MON_ATTACK_PROBABILITY = 301,		// 몬스터 공격시 - 공격력 확율 : Value 증가 %
	DES_SKILL_BUFF_MON_ATTACK_PIERCE = 302,		// 몬스터 공격시 - 피어스 증가 : Value 증가 %
	DES_SKILL_BUFF_MON_DEFENCE = 303,		// 몬스터 방어시 - 방어력 증가 : Value 증가 %
	DES_SKILL_BUFF_MON_DEFENCE_AVOID = 304,		// 몬스터 방어시 - 회피력 증가 : Value 증가 %
	DES_SKILL_BUFF_PVP_ATTACK_POWER = 305,		// PVP - 공격력 증가 : Value 증가 %
	DES_SKILL_BUFF_PVP_ATTACK_PROBABILITY = 306,		// PVP - 명중률 증가 : Value 증가 %		// 2013-08-01 by jhseol, 역전의 버프 리뉴얼
	DES_SKILL_BUFF_PVP_ATTACK_PIERCE = 307,		// PVP - 피어스 증가 : Value 증가 %		// 2013-08-01 by jhseol, 역전의 버프 리뉴얼
	DES_SKILL_BUFF_PVP_DEFENCE = 308,		// PVP - 방어력 증가 : Value 증가 %		// 2013-08-01 by jhseol, 역전의 버프 리뉴얼
	DES_SKILL_BUFF_PVP_DEFENCE_PROBABILITY = 309,		// PVP - 회피력 증가 : Value 증가 %		// 2013-08-01 by jhseol, 역전의 버프 리뉴얼


	DES_SKILL_SKILL_CANCEL_IMMUNE = 311,		// 2015-10-20 Future, Purify Immune Skills

	// 2013-05-31 by jhseol,bckim 아머 컬렉션 - COLLECTION INDEX DesParam 등록 (390~399 : 10개 예약)
	DES_COLLECTION_ARMOR_INDEX = 390,		// 아머 컬렉션에서 사용될 인덱스

	// 2013-02-28 by bckim, 복귀유져 버프추가
	DES_PLUS_WARPOINT_RATE_FOR_RETURN_USER = 499,		// ParamValue: WarPoint 증가 Rate ( 다회성 ) // 1회성 DES_PLUS_WARPOINT_RATE
	DES_BUFF_TO_RETURN_USER = 500,		// 2013-02-28 by bckim, 복귀유져 버프추가

	// 2012-10-10 by hskim, 기간제 속성 구현 (기간제 외형)
	DES_FIXED_TERM_SHAPE_TIME = 501,		// 기간제 외형 적용 시간 (초)
	DES_FIXED_TERM_ITEM_TIME = 502,		// 기간제 아이템 적용 시간 (초)

	DES_WEB_DELETE_ITEM = 503,		// 2013-03-13 by hskim, 웹 캐시 상점 - 웹 인터페이스로 에서 삭제 가능한 아이템

	DES_INGAME_SHOW = 504,		// 인게임 조합식에 표시할지 여부 // 2013-07-02 by bhsohn 인게임 조합식 Show추가 처리

	// 2015-12-17 Future, Nation Change Card
	DES_CASH_CHANGE_NATION = 505,		
	DES_CASH_CHANGE_NATION_REQ_FAME = 506,

	// 신 버프류 아이템 예약 ( 600 ~ 699 : 100개)  
	// 2013-04-18 by jhseol,bckim 이달의 아머 - 추가 DesParam 선언
	DES_OPTION_ITEM_DEFAULT_DESPARAM = 600,		// 이달의 아머에 적용될 옵션 아이템인지 구분할 DesParam

	// 파트너 소켓 아이템류 예약 (23000 ~ 23899 : 900 개 )
	DES_PET_SOCKET_ITEM_AUTOKIT = 23000,		// 파트너 시스템 소켓류 - 자동 키트, 필수적으로 DestParameter 0번에 설정 해야함
	DES_PET_SOCKET_ITEM_AUTOSKILL = 23001,		// 파트너 시스템 소켓류 - 자동 스킬, 필수적으로 DestParameter 0번에 설정 해야함
	DES_PET_SOCKET_ITEM_SPEED = 23002,		// 파트너 시스템 소켓류 - 자동 스킬, 필수적으로 DestParameter 0번에 설정 해야함

	// 파트너 자동 키트 아이템류 예약 (23900 ~ 23909 : 10 개)
	DES_PET_SLOT_ITEM_AUTOKIT_HP = 23900,		// 자동 키트 HP		=> VALUE : 등급 1 ~ 100 까지
	DES_PET_SLOT_ITEM_AUTOKIT_SHIELD = 23901,		// 자동 키트 Shield	=> VALUE : 등급 1 ~ 100 까지
	DES_PET_SLOT_ITEM_AUTOKIT_SP = 23902,		// 자동 키트 SP		=> VALUE : 등급 1 ~ 100 까지

	// 파트너 자동 스킬 아이템류 예약 (23910 ~ 23919 : 10 개)
	DES_PET_SLOT_ITEM_AUTOSKILL_AGEAR = 23910,		// 자동 스킬 A 기어
	DES_PET_SLOT_ITEM_AUTOSKILL_BGEAR = 23911,		// 자동 스킬 B 기어
	DES_PET_SLOT_ITEM_AUTOSKILL_IGEAR = 23912,		// 자동 스킬 I 기어
	DES_PET_SLOT_ITEM_AUTOSKILL_MGEAR = 23913,		// 자동 스킬 M 기어
};
