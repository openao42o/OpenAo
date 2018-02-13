#pragma once

// 2017-01-06 panoskj
// defines MapInfuelnceType types as an enum
// and constexpr helper functions

// 2005-12-26 by cmkwon
// 비트 플래그 설정 아님 - ti_MapInfo테이블의 MapInfluenceType 필드 맵 세력 타입 설정
enum MapInfuelnceType_t : USHORT
{
	MAP_INFLUENCE_NEUTRALITY = 0,			// 중립 영역
	MAP_INFLUENCE_NON_DISPUTE = 400,		// 유저간 비분쟁 영역 // 2012-01-17 by hskim, EP4 [비분쟁 지역] - 유저간 공격 금지 구역
	MAP_INFLUENCE_HELL = 500,				// 지옥 영역 (죽으면 Backmap 으로 이동 및 초기 입장 불가 설정) // 2011-10-28 by hskim, EP4 [트리거 시스템] - 크리스탈 시스템
	MAP_INFLUENCE_VCN_CITY = 1000,			// 바이제니유 도시맵
	MAP_INFLUENCE_VCN_START = 1001,			// 바이제니유 시작맵
	MAP_INFLUENCE_VCN_MSWAR = 1700,			// 바이제니유 - 모선전시에 튕기는 맵 - // 2012-12-05 by hskim, 모선전 시작시에 상대 세력 도시맵으로 워프
	MAP_INFLUENCE_VCN_BASE = 1701,			// 바이제니유 - 모선전시에 튕기는 맵 + 일반 상황에서 마을 귀환 - // 2012-12-05 by hskim, 모선전 시작시에 상대 세력 도시맵으로 워프
	MAP_INFLUENCE_VCN_LAST = 1999,			// 바이제니유 마지막맵
	MAP_INFLUENCE_ANI_CITY = 2000,			// 알링턴 도시맵
	MAP_INFLUENCE_ANI_START = 2001,			// 알링턴 시작맵
	MAP_INFLUENCE_ANI_MSWAR = 2700,			// 알링턴 - 모선전시에 튕기는 맵 - // 2012-12-05 by hskim, 모선전 시작시에 상대 세력 도시맵으로 워프
	MAP_INFLUENCE_ANI_BASE = 2701,			// 알링턴 - 모선전시에 튕기는 맵 + 일반 상황에서 마을 귀환 - // 2012-12-05 by hskim, 모선전 시작시에 상대 세력 도시맵으로 워프
	MAP_INFLUENCE_ANI_LAST = 2999,			// 알링턴 마지막맵
	MAP_INFLUENCE_NORMAL_CITY = 3000,		// 2007-05-22 by cmkwon, 일반군도시맵
	MAP_INFLUENCE_INFINITY = 5000,			// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 
	MAP_INFLUENCE_INFINITY_LAST = 5999,		// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 
	MAP_INFLUENCE_OUTPOST = 6000,			// 2007-08-17 by dhjin, 전진기지 도시 맵 
	MAP_INFLUENCE_OUTPOST_START = 6001,		// 2007-08-17 by dhjin, 전진기지 맵 
	MAP_INFLUENCE_OUTPOST_LAST = 6999,		// 2007-08-17 by dhjin, 전진기지 맵
	MAP_INFLUENCE_OUTPOST_TRIGGER = 4303,	// 2013-01-21 by jhseol, NGC 전전기지 트리거 시스템 - 트리거 전진기지 맵 추가
	MAP_INFLUENCE_ARENA = 7000,				// 2007-05-04 by dhjin, ARENA 맵
	MAP_INFLUENCE_ARENA_LAST = 7999,		// 2007-05-04 by dhjin, ARENA 맵
	MAP_INFLUENCE_EVENT_CITY = 8000,		// 이벤트 도시맵(의미없음)
	MAP_INFLUENCE_EVENT_START = 8001,		// 이벤트 시작맵, 2006-07-18 by cmkwon
	MAP_INFLUENCE_EVENT_LAST = 8999,		// 이벤트 마지막맵, 2006-07-18 by cmkwon
	MAP_INFLUENCE_CONFLICT_AREA_CITY = 9000,	// 분쟁 지역 시작맵(의미없음), // 2006-02-09 by cmkwon
	MAP_INFLUENCE_CONFLICT_AREA_START = 9001,	// 분쟁 지역 시작맵, // 2006-02-09 by cmkwon
	MAP_INFLUENCE_CONFLICT_AREA_LAST = 9999,	// 분쟁 지역 마지막맵, // 2006-02-09 by cmkwon
};

// 유저간 비분쟁 영역 // 2012-01-17 by hskim, EP4 [비분쟁 지역] - 유저간 공격 금지 구역
constexpr bool IS_MAP_INFLUENCE_NON_DISPUTE(USHORT mapInfl) {
	return  mapInfl == MAP_INFLUENCE_NON_DISPUTE; 
}
// 지옥 영역 (죽으면 Backmap 으로 이동) // 2011-10-28 by hskim, EP4 [트리거 시스템] - 크리스탈 시스템
constexpr bool IS_MAP_INFLUENCE_HELL(USHORT mapInfl) {
	return mapInfl == MAP_INFLUENCE_HELL;
}
// 바이제니유 세력 맵(1000 ~ 1999)인가?
constexpr bool IS_MAP_INFLUENCE_VCN(USHORT mapInfl) {
	return mapInfl >= MAP_INFLUENCE_VCN_CITY && mapInfl <= MAP_INFLUENCE_VCN_LAST;
}
// 알링턴 세력 맵(2000 ~ 2999)인가?
constexpr bool IS_MAP_INFLUENCE_ANI(USHORT mapInfl) {
	return mapInfl >= MAP_INFLUENCE_ANI_CITY && mapInfl <= MAP_INFLUENCE_ANI_LAST;
}
// 2007-05-22 by cmkwon, 일반군도시맵(3000)인가?
constexpr bool IS_MAP_INFLUENCE_NORMAL(USHORT mapInfl) {
	return mapInfl == MAP_INFLUENCE_NORMAL_CITY;
}
// 분쟁 지역 맵(9000 ~ 9999)인가?
constexpr bool IS_MAP_INFLUENCE_CONFLICT_AREA(USHORT mapInfl) {
	return mapInfl >= MAP_INFLUENCE_CONFLICT_AREA_CITY && mapInfl <= MAP_INFLUENCE_CONFLICT_AREA_LAST;
}
// 이벤트맵(8000 ~ 8999)인가?
constexpr bool IS_MAP_INFLUENCE_EVENT_AREA(USHORT mapInfl) {
	return mapInfl >= MAP_INFLUENCE_EVENT_CITY && mapInfl <= MAP_INFLUENCE_EVENT_LAST;
}
// 아레나 맵(7000 ~ 7999)인가?
constexpr bool IS_MAP_INFLUENCE_ARENA(USHORT mapInfl) {
	return mapInfl >= MAP_INFLUENCE_ARENA && mapInfl <= MAP_INFLUENCE_ARENA_LAST;
}

// 전진기지 맵(6000 ~ 6999)인가?	// 2013-01-21 by jhseol, NGC 전전기지 트리거 시스템 - 트리거 전진기지 맵 추가	
constexpr bool IS_MAP_INFLUENCE_OUTPOST(USHORT mapInfl) {
	return (mapInfl >= MAP_INFLUENCE_OUTPOST_START && mapInfl <= MAP_INFLUENCE_OUTPOST_LAST) || mapInfl == MAP_INFLUENCE_OUTPOST_TRIGGER;
}	
// 2009-09-09 ~ 2010 by dhjin, 인피니티 -
constexpr bool IS_MAP_INFLUENCE_INFINITY(USHORT mapInfl) {
	return mapInfl >= MAP_INFLUENCE_INFINITY && mapInfl <= MAP_INFLUENCE_INFINITY_LAST;
}	 
// 2012-12-05 by hskim, 모선전 시작시에 상대 세력 도시맵으로 워프
constexpr bool IS_MAP_INFLUENCE_MSWAR(USHORT mapInfl) {
	return mapInfl == MAP_INFLUENCE_VCN_MSWAR
	|| mapInfl == MAP_INFLUENCE_ANI_MSWAR
	|| mapInfl == MAP_INFLUENCE_VCN_BASE
	|| mapInfl == MAP_INFLUENCE_ANI_BASE;
}	
// 2012-12-05 by hskim, 모선전 시작시에 상대 세력 도시맵으로 워프
constexpr bool IS_MAP_INFLUENCE_MSWAR_ANI(USHORT mapInfl) {
	return mapInfl == MAP_INFLUENCE_ANI_MSWAR || mapInfl == MAP_INFLUENCE_ANI_BASE;
}
// 2012-12-05 by hskim, 모선전 시작시에 상대 세력 도시맵으로 워프
constexpr bool IS_MAP_INFLUENCE_MSWAR_VCN(USHORT mapInfl) {
	return mapInfl == MAP_INFLUENCE_VCN_MSWAR || mapInfl == MAP_INFLUENCE_VCN_BASE;
}		

constexpr bool IS_MAP_INFLUENCE_CITY(USHORT mapInfl) {
	return mapInfl == MAP_INFLUENCE_VCN_CITY || mapInfl == MAP_INFLUENCE_ANI_CITY || mapInfl == MAP_INFLUENCE_NORMAL_CITY;
}

// 2008-12-23 by dhjin, 전쟁 보상 추가안
constexpr bool IS_MAP_INFLUENCE_WAR(USHORT mapInfl) {
	return IS_MAP_INFLUENCE_VCN(mapInfl) || IS_MAP_INFLUENCE_ANI(mapInfl) || IS_MAP_INFLUENCE_OUTPOST(mapInfl);
}
