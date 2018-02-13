// Quest.h: interface for the CQuest class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_QUEST_H__C9FFDA46_B3F8_4FBF_AA50_4547696C4FEF__INCLUDED_)
#define AFX_QUEST_H__C9FFDA46_B3F8_4FBF_AA50_4547696C4FEF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define QUEST_USER_DEFINED_VARIABLES_FILE_NAME	"q_000000.cfg"

#define WIN32_LEAN_AND_MEAN // so that windows.h won't load winsock.h

#include "mt_stl.h"
#include "AtumParam.h"
#include "AtumDataLoader.h"
#include "QuestLoaderParser.h"

#define LAST_SCENARIO_QUEST_NEXT_QUEST_INDEX			-1			// 2006-08-07 by cmkwon, 시나리오 미션일 경우 NextQuestIndex가 -1이면 마지막 퀘스트임



struct QUEST_DROP_ITEM
{
	INT				MonsterUniqueNumber;	// 몬스터 종류
	INT				ItemNumber;				// 아이템고유넘버
	INT				MinCount;				// 생성할 아이템의 최소 개수
	INT				MaxCount;				// 생성할 아이템의 최대 개수, 만약 개수가 정해져 있다면 MinCount = MixCount
	INT				Probability;			// 생성 확률: 0 ~ 10000, Prob10K_t
};
struct QUEST_DROP_CHARACTER2ITEM
{
	BYTE			InfluenceType0;			// 세력 타입으로
	USHORT			UnitKindMask0;			// 유닛의 종류
	CHARACTER2ITEM	Character2Item;
};


class CQuest  
{
public:
	CQuest();

	void Reset();
	CQuest *Clone();
	BOOL IsExistTargetMonsterVector(INT i_monUniqueNumber);
	BOOL IsPartyWarp(void);
	BOOL IsPartyQuest(void);
	BOOL IsFirstScenarioQuest(BYTE i_byInflTy);

	
	BOOL IsEndTypeTimeOver(void);			// 2007-12-06 by cmkwon, 시간제한 퀘스트 서버에서 시간 체크하게 수정 - CQuest::IsEndTypeTimeOver() 추가
	BOOL IsCompleteEndTypeTimeOver(int i_nSpentTimeInSec);	// 2007-12-06 by cmkwon, 시간제한 퀘스트 서버에서 시간 체크하게 수정 - CQuest::IsCompleteEndTypeTimeOver() 추가



public:
	// Basic Info
	INT					QuestIndex;
	char				QuestName[SIZE_MAX_QUEST_NAME];
	BYTE				QuestEpisodeType;				// 2008-06-16 by cmkwon, EP3 퀘스트 기존 퀘스트와 구별 - QUEST_EPISODE_TYPE_XXX
	BYTE				QuestPartyType;			// 2008-12-02 by dhjin, 미션마스터		
	BYTE				QuestKind;			// 일반, 콜렉션, 시나리오
	BYTE				QuestType;			// 1회성, 연속성: QUEST_TYPE_SINGLE, QUEST_TYPE_SERIES
	BYTE				QuestStartType;		// QUEST가 발생하는 시점에 따른 분류: QUEST_START_TYPE_XXX
	BYTE				QuestEndType;		// QUEST가 종료 방식에 따른 분류: QUEST_END_TYPE_XXX
	BYTE				QuestPartyMembers;	// 2006-03-24 by cmkwon, 파티퀘스트 여부, 파티원수, 유효값:0 ~ SIZE_MAX_PARTY_MEMBER(6)
	QUEST_NPC_INFO		QuestNPCInfo;		// 퀘스트를 주는 NPC 정보 및 Talk
	char				QuestDescription[SIZE_MAX_NPC_TALK_STRING];	// 퀘스트에 대한 설명
	MapIndex_t			EventMapIndex;					// 퀘스트가 일어나는 맵, ex)해당 맵에서 몬스터를 잡을 때만 아이템이 떨이진다.
	INT					NextQuestIndex;					// 다음 퀘스트 번호, QuestType이 연속성일 때만 의미 가짐
	INT					PartyQuestMemberCount;			// 1 이상이면 파티 퀘스트임, 해당 인원만큼 파티를 맺고 사냥을 해야 아이템이 떨어짐.
	BOOL				DropItemToAllAttackCharacter;	// 한번이라도 데미지를 준 모든 캐릭터에게 Quest Drop Item을 주는 지 여부, 디폴트는 최대 데미지를 준 캐릭에게만 아이템을 줌.
	INT					TimeLimitInMinutes;				// 퀘스트 종료 타입이 QUEST_END_TYPE_TIME_OVER일때는 경과후 퀘스트 완료 가능, 그외에는 시간 제한(분단위)
	BOOL				IsDiscardable;					// 퀘스트 시작 시 YES 혹은 NO가 있으며 해당하는 조건에 따라 내용이 달라지는 조건(YES 퀘스트를 수행함, NO 그냥 넘김), 기본값은 FALSE
	BOOL				IsCityWar;						// 도시점령전 퀘스트 인가? - 도시점령전 퀘스트는 받을때 QuestParam1에 서버군 ID를 저장한다.
	BOOL				IsNeverCompletionByUser;		// 유저의 요청으로 퀘스트가 완료되지 않는다
	BOOL				IsDropItemToAllPartyMembers;	// 2009-01-13 by cmkwon, 미션 보상을 파티원 모두에게 주기 명령어 추가 - CQuest에 멤버 변수 추가

	// Start Requirements
	USHORT					ReqUnitKind;
	INT_RANGE				ReqLevel;			// 필요 레벨 하한
	Experience_t			ReqExperience;		// check: 나중에 필요하면 사용하기, 지금은 사용하지 않고 있음! 20040422, kelovon
	INT_RANGE				ReqAttackPart;
	INT_RANGE				ReqDefensePart;
	INT_RANGE				ReqFuelPart;
	INT_RANGE				ReqSoulPart;
	INT_RANGE				ReqShieldPart;
	INT_RANGE				ReqDodgePart;
	vector<INT>				ReqQuestIndexVector;
	vector<INT>				ReqRecvQuestIndexVector;
	vector<ITEM_W_COUNT>	ReqItemVector;
	INT						ReqGuild;
	BYTE					ReqInfluenceType;				// 세력 타입 마스크		
	MAP_AREA				ReqStartArea;			// 2005-08-22 by cmkwon, 미션 시작 조건에 특정 좌표 설정

	// Start Action
	vector<QUEST_PAY_ITEM>	StartPayItemVector;		// 시작 시 주는 아이템
	vector<ITEM_W_COUNT>	StartDelItemVector;		// 시작 시 지우는 아이템
	MapIndex_t				StartMapIndex;			// 시작시 워프될 MapIndex
	BOOL					StartPartyWarpFlag;		// StartMapIndex가 유효 할 때 파티원 모두 워프하는지의 플래그(TRUE:모든 파티원 미션맵으로 이동, FALSE자신만 미션맵으로 이동)

	// Completion Requirements
	vector<ITEM_W_COUNT>			TargetItemVector;			// 퀘스트 완료를 위해 필요한 아이템 리스트
	vector<ITEM_W_COUNT_MAP_AREA>	TargetMonsterItemVector;	// 몬스터 사냥 퀘스트에서 잡았을 때 떨어지는 아이템 및 사냥 범위, 0이면 무시
	QUEST_NPC_INFO					TargetMeetNPCInfo;			// NPC를 만나면 해결되는 퀘스트에서, 만나야 할 NPC, 0이면 무시
	INT								TargetMeetObjectIndex;		// object를 만나면 해결되는 퀘스트에서, 만나야 할 object, 0이면 무시
	MAP_AREA						TargetMeetObjectMapArea;	// object를 만나면 해결되는 퀘스트에서, 만나야 할 object의 위치, 2005-07-07 by cmkwon, 워프타겟을 만나면 해결되는 퀘스트에서, 만나야 할 워프타겟인덱스 - 서버에서 자동으로 완료 처리됨
	INT								TargetDeadByMonsterUniquenumber;	// 설정된 몬스터에 의해 죽을때 퀘스트 해결 조건
	MAP_AREA						TargetMeetMapArea;			// 2005-08-29 by cmkwon, 미션 종료 조건에 특정 좌표 설정
	vectMONSTER_W_COUNT				TargetMonsterVector;		// 2005-10-19 by cmkwon, 퀘스트 완료를 위해 사냥해야하는 몬스터 카운트 리스트
	ITEM_W_COUNT					TargetMixItemTarget;		// 2007-04-02 by cmkwon, 미션 종료 조건에 조합을 위해 추가함 변수로
																// 조합 결과물 필요 아이템이며, 이 아이템이 설정되어 있으면 다른 필요아이템 체크는 필요 없다
	
	// Completion Action
	vector<ITEM_W_COUNT>			EndDeleteItemVector;	// 퀘스트 수행 시 없어지는 아이템
	vector<QUEST_PAY_ITEM>			EndPayItemVector;		// 퀘스트 수행 시 주어지는 아이템
	vector<QUEST_PAY_ITEM>			EndMissionMasterPayItemVector;		// 2008-12-02 by dhjin, 미션마스터 - 퀘스트 수행 시 미션마스터에게 주어지는 아이템	
	vector<QUEST_RANDOM_PAY_ITEM>	EndRandomPayItemVector;	// 퀘스트 수행 시 RANDOM하게 주어지는 아이템
	vector<ITEM_MIXING_INFO>		EndPayMixItemVector;	// 퀘스트 수행 시 조합에 의해 주어지는 아이템
	BYTE							EndPayBonusStat;
// 2005-11-15 by cmkwon, 삭제함
//	BYTE							EndPayBonusSkillPoint;
	INT								EndPayFame;			// 명성이 높아짐
	INT								EndPayStatus;		// 신분 상승함
	BYTE							EndPayLevel;		// 레벨 상승량(+1, +2 등)
	INT								EndPayExperience;	// 경험치 상승량(+100, +200 등)
	MapIndex_t						EndPayWarpMapIndex;	// 워프할 맵
	INT								EndPayPropensity;	// 신분 상승 보너스
	INT								EndCinemaNum;		// CinemaNum
	INT								EndInfluenceType;	// 0이 아니면 세력을 변경한다.

	// Quest용 Drop Item List, 로딩 후 CFieldIOCP::m_mapMonster2Item에 넣어 주어야 함
	vector<QUEST_DROP_ITEM>			vecQuestDropItem;
	vector<QUEST_DROP_CHARACTER2ITEM>	vecQuestDropCharacter2Item;		// 2006-03-02 by cmkwon, 세력전관

	// 기타 추가 정보
public:
	float							StartPayItemWeight;	// FieldServer에서 퀘스트 로딩 후 계산해서 할당해야 함
	float							EndPayItemWeight;	// FieldServer에서 퀘스트 로딩 후 계산해서 할당해야 함
	float							EndMissionMasterPayItemWeight; // 2008-12-02 by dhjin, 미션마스터 - // FieldServer에서 퀘스트 로딩 후 계산해서 할당해야 함
};
typedef vector<CQuest*>		vectCQuestPtr;		// 2007-07-09 by cmkwon, 퀘스트 스크립트를 정보를 vector로 받기 위해 - 

///////////////////////////////////////////////////////////////////////////////
// 2008-06-16 by cmkwon, EP3 퀘스트 기존 퀘스트와 구별 - 
// QuestEpisodeType
#define QUEST_EPISODE_TYPE_1AND2			(BYTE)0		// Episode1 ~ 2
#define QUEST_EPISODE_TYPE_3				(BYTE)1		// Episode3
#define QUEST_INFINITY_TYPE_3				(BYTE)2		// Infinity3		// 2011-03-07 by hskim 인피니티 3차 Quest 시작 알림 기능
#define QUEST_EPISODE_TYPE_4				(BYTE)3		// Episode4			// 2011-11-04 by hskim, EP4 [퀘스트 시스템]

//////////////////////////////////////////////////////////////////////////
// 2008-12-08 by dhjin, 미션마스터
#define QUEST_PARTYTYPE_NONE				(BYTE)0			
#define QUEST_PARTYTYPE_PARTY				(BYTE)1			

// QuestKind
#define QUEST_KIND_NORMAL					(BYTE)0		// 일반 Quest
#define QUEST_KIND_SIDESTORY				(BYTE)1		// 번외 미션 ,// 2006-03-15 by cmkwon, QUEST_KIND_COLLECTION에서 변경한것
#define QUEST_KIND_SCENARIO					(BYTE)2		// 시나리오 Quest

// QuestType
#define QUEST_TYPE_SINGLE					(BYTE)0		// 일회성 Quest
#define QUEST_TYPE_SERIES					(BYTE)1		// 연속성 Quest
#define QUEST_TYPE_REPEATABLE				(BYTE)2		// 무한 반복 Quest

// QuestStartType
#define QUEST_START_TYPE_LEVEL_CHANGE		(BYTE)0		// Level 변화 시 일어날 수 있는 Quest
#define QUEST_START_TYPE_EXP_CHANGE			(BYTE)1		// EXP 변화 시 일어날 수 있는 Quest
#define QUEST_START_TYPE_MEET_NPC			(BYTE)2		// 특정 NPC를 만났을 때 일어날 수 있는 Quest
#define QUEST_START_TYPE_ENTER_BUILDING		(BYTE)3		// 특정 건물에 들어갔을 때 일어날 수 있는 Quest
#define QUEST_START_TYPE_ITEM_CLICK			(BYTE)4		// 아이템을 클릭 시 일어날 수 있는 QUEST
#define QUEST_START_TYPE_COUPON				(BYTE)5		// 쿠폰 퀘스트 - 무한 반복, 시작시 쿠폰번호 체크 후 완료가 바로 진행됨

// QuestEndType
#define QUEST_END_TYPE_IMMEDIATE			(BYTE)0		// 시작과 함께 바로 끝나는 Quest, 클라이언트는 서버의 Quest 종료 메시지를 받을 때 까지 기다리면 됨
#define QUEST_END_TYPE_REQESTED_BY_CLIENT	(BYTE)1		// 클라이언트의 요청과 그에 대한 서버의 검사에 의해 끝나는 Quest
#define QUEST_END_TYPE_LEVEL_UP				(BYTE)2		// 레벨업시에 퀘스트가 종료되는 타입, QuestParam1에 현재 레벨 저장됨 - 다른 종료 조건 체크는 없음, 도시점령전과 겹칠수 없음
#define QUEST_END_TYPE_TIME_OVER			(BYTE)3		// 일정시간(TimeLimitInMinutes) 경과시 퀘스트가 종료되는 타입 - 다른 종료 조건 체크는 없음
#define QUEST_END_TYPE_SELECT_INFLUENCE		(BYTE)4		// 2005-08-17 by cmkwon, 세력 선택 미션으로 QuestResult에 선택한 세력이 들어온다.

struct CharacterQuest{
	UID32_t			CharacterUniqueNumber;
	INT				QuestIndex;
	BYTE			QuestState;				// QUEST_STATE_XXX
	INT				QuestParam1;			// 도시점령전미션(도시점령전서버군 인덱스), 종료 타입이 QUEST_END_TYPE_LEVEL_UP인 미션(현재 레벨) <== 겹쳐서 사용되면 안된다.
	ATUM_DATE_TIME	QuestTimeStamp;			// 퀘스트 시작 시간
	LONGLONG		QuestPlayTimeStamp;		// 퀘스트 시작 시간(PlayTime)
};							// 캐릭터의 퀘스트 진행 상황

struct DBCharacterQuestMonsterCount			// 2005-10-19 by cmkwon
{
	UID32_t			CharacterUniqueNumber;
	INT				QuestIndex;
	INT				MonsterUniqueNumber;
	INT				Count;
};

struct CharacterQuestMonsterCount			// 2005-10-19 by cmkwon
{
	INT				MonsterUniqueNumber;
	INT				Count;
};
typedef vector<CharacterQuestMonsterCount>	vectCharacterQuestMonsterCount;


// QusetState
#define QUEST_STATE_NONE			(BYTE)0		// 시작하지 않은 상태
#define QUEST_STATE_IN_PROGRESS		(BYTE)1		// 진행중
#define QUEST_STATE_COMPLETED		(BYTE)2		// 완료

#endif // !defined(AFX_QUEST_H__C9FFDA46_B3F8_4FBF_AA50_4547696C4FEF__INCLUDED_)
