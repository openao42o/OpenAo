#pragma once

// 아이템의 속성, ITEM_ATTR_XXX
enum
{
	ITEM_ATTR_AUTO_PICK_UP = 0x00000001,					// 1, 몬스터가 터지면 자동으로 먹기(바닥에 안 떨어짐)
	ITEM_ATTR_NO_TRANSFER = 0x00000002,						// 2, 창고 이동, 버리기, 상점에서 구입, 상점에 판매, 유저간 거래 불가
	ITEM_ATTR_QUEST_ITEM = 0x00000004,						// 4, 퀘스트 아이템, 보통의 경우는 떨어지지 않고, 퀘스트 수행중이며 조건에 만족할 때만 떨어짐, check: deprecated, ITEMKIND_QUEST를 사용, 20040714, kelovon
	ITEM_ATTR_TIME_LIMITE = 0x00000008,						// 8, 시간 제한 아이템, 아이템 사용시 사용 시간 제한이 있다
	ITEM_ATTR_KILL_MARK_ITEM = 0x00000010,					// 16, 세력전 킬마크 아이템(창고이동/버리기/상점에판매 가능, 유저간거래/상점에서구입 불가), 2006-02-09 by cmkwon
	ITEM_ATTR_SKILL_SUPPORT_ITEM = 0x00000020,				// 32, 스킬 보조 아이템, 2006-09-29 by cmkwon 추가 됨
	ITEM_ATTR_DELETED_TIME_LIMITE = 0x00000040,				// 64, 아이템이 생성 후 유효 시간(ti_item.Endurance)이 경과하면 자동으로 삭제되는 아이템, 2006-09-29 by cmkwon
	ITEM_ATTR_ACCOUNT_POSSESSION = 0x00000080,				// 128, 계정소유아이템(개인창고이동/버리기/상점에판매/상점에서구입 가능, 유저간거래/여단창고이동 불가), // 2007-09-17 by cmkwon, 예당 캐쉬상점 캐쉬아이템 거래 불가 기능 추가 - 

	ITEM_ATTR_ARENA_ITEM = 0x00000100,						// 256, 아레나에서만 사용 가능한 아이템, 2007-06-01 by dhjin
	ITEM_ATTR_SUPER_GAMBLE_ITEM = 0x00000200,				// 512, // 2007-11-07 by cmkwon, 갬블/슈갬 붙는 시스템 수정 - 슈퍼갬블슈퍼갬블 옵션이 붙을 갬블카드아이템
	ITEM_ATTR_PACKAGE_ITEM = 0x00000400,					// 1024, // 2008-08-26 by cmkwon, ItemAttribute 추가 - 패키지(Package) 아이템, 현재는 미스테리캡슐만 적용됨
	ITEM_ATTR_FOR_ONLY_GAME_MASTER = 0x00000800,			// 2048, // 2008-08-26 by cmkwon, ItemAttribute 추가 - GM용 아이템, RACE_GAMEMASTER 만 사용 가능한 아이템

	ITEM_ATTR_UNIQUE_ITEM = 0x00001000,						// 4096, 유니크 아이템, 인챈트/갬블 불가, 2005-11-21 by cmkwon
	ITEM_ATTR_BAZAAR_ITEM = 0x00002000,						// 8192, 2006-07-26 by cmkwon, 개인상점에 구입/판매 가능 아이템
	ITEM_ATTR_LEGEND_ITEM = 0x00004000,						// 16384, // 2007-08-22 by cmkwon, 레전드 아이템, 기본은 유니크아이템 속성이며 조합시 타겟 아이템이 레전드 이면 인챈트가 유지된다
	ITEM_ATTR_FORBID_ARENA_COPY_ITEM = 0x00008000,			// 32768, // 2008-01-07 by dhjin, 아레나 통합 - atum2_db_n => atum2_db_arena 로 td_store를 복사하면 안되는 아이템

	ITEM_ATTR_CASH_ITEM = 0x00010000,						// 65536, 유료화 아이템
	ITEM_ATTR_CASH_ITEM_PREMIUM_CARD = 0x00020000,			// 131072, // 2008-08-26 by cmkwon, ItemAttribute 추가 - 멤버쉽(프리미엄) 아이템,  

	ITEM_ATTR_LUCKY_ITEM = 0x00040000,						// 262144, // 2008-11-04 by dhjin, 럭키머신
	ITEM_ATTR_DELETED_TIME_LIMITE_AFTER_USED = 0x00080000,	// 524288, // 2008-11-26 by cmkwon, 사용 후 시간(절대시간) 제한 아이템 구현 - 추가 함

	ITEM_ATTR_MISSION_MASTER = 0x00100000,					// 1048576, // 2008-12-15 by dhjin, 미션마스터
	ITEM_ATTR_ROBOT_ARMOR = 0x00200000,						// 2097152, // 2009-07-07 by cmkwon, 로봇 아머 관련 처리 추가 - 로봇아머 아이템 속성 추가

	ITEM_ATTR_CANNOT_USE_INFINITY = 0x00400000,				// 4194304, // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피니티에서 사용 불가 아이템
	ITEM_ATTR_ONLY_USE_INFINITY = 0x00800000,				// 8388608, // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피니티에서만 사용 가능 아이템
	ITEM_ATTR_CHECK_SKILL_RESISTANCE_STATE = 0x01000000,	// 16777216, // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 레지스탕스 키트 적용 여부를 체크해야되는 아이템

	ITEM_ATTR_NOT_STORE_SAVE = 0x02000000,					// 33554432, // 2011-06-07 by shcho, 베트남 요청 창고에만 저장 불가 속성 구현
	ITEM_ATTR_WAREHOUSE_SHARE_BANNED = 0x04000000,			// 67108864, // 2012-01-14 by hskim, 파트너 시스템 2차 - 개인 창고 공유 금지 추가 (ITEM_ATTR_ACCOUNT_POSSESSION 같은 속성과 같이 사용해야함)
	ITEM_ATTR_HYPER_GAMBLE_ITEM = 0x08000000,				// 134217728, // 2012-12-27 by hskim, 하이퍼 접두/점미 시스템 구현

	ITEM_ATTR_TRASH_ONLY = 0x10000000,						// 268435456, // 2015-06-13 Future, new Trading Option (only trashing allowed)
};