#pragma once

#include <VMemPool.h>
#include "MapBlock.h"

#define	MON2MON_IMPACT_DISTANCE_GAP							20.0f		// 몬스터간 충돌 처리 간격
#define MONSTER_MIN_ATTACK_DISTANCE							100.0f		// 몬스터가 공격시 최소 유지 거리
#define MONSTER_TARGET_DISTANCE								200.0f		// 공격 성향의 몬스터가 주위에서 Target을 잡는 거리
#define MONSTER_BASIC_ARROUND_MOVABLE_RANGE					500.0f		// 몬스터가 생성지점으로 부터 이동 가능한 기초 거리
#define MONSTER_ANGLE_PI_5									0.0872f		// 5도를 PI로 변환한값
#define MONSTER_MAX_QUICK_TURN_ANGLE						150			// 150도
#define MONSTER_ATTACKOBJ_SAMERACE_MAX_DISTANCE				300			// Attack Object가 동족성
#define MONSTER_MAX_PARTY_DISTANCE							100.0f		// 파티를 생성하기 위한 최대거리, 파티가 소멸되는 거리는 + 50 = 200.0f
#define MONSTER_MAX_PARTY_MEMBER_COUNTS						10			// 몬스터 파티 최대 카운트
#define MONSTER_NOT_MOVE_AFTER_CREATED_TERM_TICK			2000		// 몬스터가 생성되고 움직이지 않는 시간
#define MONSTER_EVENT_MON_NOT_MOVE_AFTER_CREATED_TERM_TICK			10000	// 이벤트 몬스터가 생성되고 움직이지 않는 시간
#define MONSTER_DELETE_AFTER_CREATED_TERM_TICK				10000		// 몬스터가 생성되었지만 FieldServer로 부터 응답을 못받았을경우 삭제하는 시간, 2006-07-25 by cmkwon 5000-->10000
#define MONSTER_MIN_ATTACK_TERM_TICK						100			// 몬스터가 Attack을 위해 최소한 이텀이상이 지나야 다음 공격이 가능함(최소 ShotNum Term)
#define MONSTER_ATTACKED_INFO_DELETE_TERM_TICK				10000		// 몬스터가 Attackedlist에서 Attack이 없을때 지우는 Tick
#define MONSTER_UPDATE_MOVE_TERM_TICK						200			// 몬스터의 이동을 위한 텀
#define MONSTER_MOVPAT_GROUNDTELEPORT_ANIMATION_COUNT		2000/MONSTER_UPDATE_MOVE_TERM_TICK	// 2초 땅속 이동 몬스터가 땅으로 들어가고 나오는데 카운트
#define MONSTER_MAP_IMPACK_HEIGHT_GAP						40.0f		// 몬스터가 이동중 맵과 충돌시 높이차기 이보다 크면 MSS_MAP_IMPACT상태가 된다.
#define MONSTER_COPTER_MAXTARGET_PINPOINT					PI/6		// 35도
#define MONSTER_COPTER_MAXMOVE_PINPOINT						PI/9		// 20도



#define MON_MOVE_RANDOMCOUNT_0						2
#define MON_MOVE_RANDOMCOUNT_1						10
#define MON_MOVE_RANDOMCOUNT_2						20

#define MON_MOVE_LRCOUNT_0							30			// FLYING_RIGHT, LeftRight Counts
#define MON_MOVE_LRCOUNT_1							40			// FLYING_RIGHT, LeftRight Counts
#define MON_MOVE_LRCOUNT_2							50			// FLYING_RIGHT, LeftRight Counts

#define MON_MOVE_FBCOUNT_0							15			// FLYING_RIGHT, FrontBack Counts
#define MON_MOVE_FBCOUNT_1							20			// FLYING_RIGHT, FrontBack Counts
#define MON_MOVE_FBCOUNT_2							25			// FLYING_RIGHT, FrontBack Counts

#define MON_MOVE_UDCOUNT_0							30			// FLYING_RIGHT, UpDown Counts
#define MON_MOVE_UDCOUNT_1							40			// FLYING_RIGHT, UpDown Counts
#define MON_MOVE_UDCOUNT_2							50			// FLYING_RIGHT, UpDown Counts

typedef enum
{
	MS_NULL				= 0,			// Monster가 사용되지 않는 상태
	CS_NULL				= 0,			// 클라언트가 사용되지 않는 상태
	MS_CREATED			= 1,			// Monster가 생성되어 Field Server로 생성 패킷을 보낸상태
	MS_PLAYING			= 2,			// Monster가 비행중인 상태
	CS_GAMESTARTED		= 2,			// 클라이언트가 Field Server로 접속한 상태
	MS_DEAD				= 3,			// Monster가 폭발한 상태
	CS_DEAD				= 3				// 클라이언트가 폭발한 상태
} UNIT_STATE;

typedef enum
{
	MSS_NORMAL					= 0,		// Monster 정상적으로 이동하는 상태
	MSS_MAP_IMPACT				= 1,		// 맵, Object와 충돌 상태
	MSS_RANGE_DISTANCE_IMPACT	= 2,		// 몬스터가 비 공격중일때 이동영역을 벗어났다
	MSS_OUT_OF_ATTACK_RANGE		= 3,		// 처음 공격시 거리가 멀어 빠르게 가까이 갈때
	MSS_QUICK_TURN_GENERAL		= 4,		// 몬스터가 직선이동의 QuickTurn이 사용중이다,
	MSS_QUICK_TURN_SKILL		= 5			// 몬스터가 자체 스킬의 QuickTurn이 사용중이다,
} Unit_Move_State;

typedef enum
{
	MSD_FRONT					=  1,
	MSD_BACK					= -1,
	MSD_LEFT					=  1,
	MSD_RIGHT					= -1,
	MSD_UP						=  1,
	MSD_DOWN					= -1,
	MSD_SPEED_UP				=  1,
	MSD_SPEED_DOWN				= -1,

	MSD_TARGET					=  0,					// Monster의 이동 방향이 Target Vector과 같다
	MSD_LEFT_1					=  1,					// Monster의 이동 방향이 Target Vector의 좌측 15도 방향
	MSD_RIGHT_1					= -1,
	MSD_LEFT_2					=  2,					// Monster의 이동 방향이 Target Vector의 좌측 15도 방향
	MSD_RIGHT_2					= -2,
	MSD_LEFT_3					=  3,					// Monster의 이동 방향이 Target Vector의 좌측 15도 방향
	MSD_RIGHT_3					= -3,
	MSD_LEFT_4					=  4,					// Monster의 이동 방향이 Target Vector의 좌측 15도 방향
	MSD_RIGHT_4					= -4,
	MSD_LEFT_5					=  5,					// Monster의 이동 방향이 Target Vector의 좌측 15도 방향
	MSD_RIGHT_5					= -5,					// Monster의 이동 방향이 Target Vector의 우측 15도 방향
	MSD_LEFT_10					=  10,					// Monster의 이동 방향이 Target Vector의 좌측 15도 방향
	MSD_RIGHT_10				= -10,					// Monster의 이동 방향이 Target Vector의 우측 15도 방향
	MSD_LEFT_15					=  15,					// Monster의 이동 방향이 Target Vector의 좌측 15도 방향
	MSD_RIGHT_15				= -15,					// Monster의 이동 방향이 Target Vector의 우측 15도 방향
	MSD_LEFT_20					=  20,					// Monster의 이동 방향이 Target Vector의 좌측 15도 방향
	MSD_RIGHT_20				= -20,					// Monster의 이동 방향이 Target Vector의 우측 15도 방향
	MSD_LEFT_25					=  25,					// Monster의 이동 방향이 Target Vector의 좌측 15도 방향
	MSD_RIGHT_25				= -25,					// Monster의 이동 방향이 Target Vector의 우측 15도 방향
	MSD_LEFT_30					=  30,					// Monster의 이동 방향이 Target Vector의 좌측 30도 방향
	MSD_RIGHT_30				= -30,					// Monster의 이동 방향이 Target Vector의 우측 30도 방향
	MSD_LEFT_35					=  35,					// Monster의 이동 방향이 Target Vector의 좌측 15도 방향
	MSD_RIGHT_35				= -35,					// Monster의 이동 방향이 Target Vector의 우측 15도 방향
	MSD_LEFT_40					=  40,					// Monster의 이동 방향이 Target Vector의 좌측 15도 방향
	MSD_RIGHT_40				= -40,					// Monster의 이동 방향이 Target Vector의 우측 15도 방향
	MSD_LEFT_45					=  45,					// Monster의 이동 방향이 Target Vector의 좌측 45도 방향
	MSD_RIGHT_45				= -45,					// Monster의 이동 방향이 Target Vector의 우측 45도 방향
	MSD_LEFT_50					=  50,					// Monster의 이동 방향이 Target Vector의 좌측 15도 방향
	MSD_RIGHT_50				= -50,					// Monster의 이동 방향이 Target Vector의 우측 15도 방향
	MSD_LEFT_55					=  55,					// Monster의 이동 방향이 Target Vector의 좌측 15도 방향
	MSD_RIGHT_55				= -55,					// Monster의 이동 방향이 Target Vector의 우측 15도 방향
	MSD_LEFT_60					=  60,					// Monster의 이동 방향이 Target Vector의 좌측 60도 방향
	MSD_RIGHT_60				= -60,					// Monster의 이동 방향이 Target Vector의 우측 60도 방향
	MSD_LEFT_65					=  65,					// Monster의 이동 방향이 Target Vector의 좌측 15도 방향
	MSD_RIGHT_65				= -65,					// Monster의 이동 방향이 Target Vector의 우측 15도 방향
	MSD_LEFT_70					=  70,					// Monster의 이동 방향이 Target Vector의 좌측 15도 방향
	MSD_RIGHT_70				= -70,					// Monster의 이동 방향이 Target Vector의 우측 15도 방향
	MSD_LEFT_75					=  75,					// Monster의 이동 방향이 Target Vector의 좌측 75도 방향
	MSD_RIGHT_75				= -75,					// Monster의 이동 방향이 Target Vector의 우측 75도 방향
	MSD_LEFT_80					=  80,					// Monster의 이동 방향이 Target Vector의 좌측 15도 방향
	MSD_RIGHT_80				= -80,					// Monster의 이동 방향이 Target Vector의 우측 15도 방향
	MSD_LEFT_85					=  85,					// Monster의 이동 방향이 Target Vector의 좌측 15도 방향
	MSD_RIGHT_85				= -85,					// Monster의 이동 방향이 Target Vector의 우측 15도 방향
	MSD_LEFT_90					=  90,					// Monster의 이동 방향이 Target Vector의 좌측 90도 방향
	MSD_RIGHT_90				= -90,					// Monster의 이동 방향이 Target Vector의 우측 90도 방향
	MSD_LEFT_100				=  100,
	MSD_RIGHT_100				= -100,
	MSD_LEFT_110				=  110,
	MSD_RIGHT_110				= -110,
	MSD_LEFT_120				=  120,
	MSD_RIGHT_120				= -120,
	MSD_TARGET_180				=  180,					// Monster의 이동 방향이 Target Vector의 반대 방향

	MSD_UP_1					=  1,
	MSD_DOWN_1					= -1,
	MSD_UP_2					=  2,
	MSD_DOWN_2					= -2,
	MSD_UP_3					=  3,
	MSD_DOWN_3					= -3,
	MSD_UP_4					=  4,
	MSD_DOWN_4					= -4,
	MSD_UP_5					=  5,
	MSD_DOWN_5					= -5,
	MSD_UP_10					=  10,
	MSD_DOWN_10					= -10,
	MSD_UP_15					=  15,
	MSD_DOWN_15					= -15,
	MSD_UP_20					=  20,
	MSD_DOWN_20					= -20,
	MSD_UP_25					=  25,
	MSD_DOWN_25					= -25,
	MSD_UP_30					=  30,
	MSD_DOWN_30					= -30,
	MSD_UP_35					=  35,
	MSD_DOWN_35					= -35,
	MSD_UP_40					=  40,
	MSD_DOWN_40					= -40,
	MSD_UP_45					=  45,
	MSD_DOWN_45					= -45,
	MSD_UP_50					=  50,
	MSD_DOWN_50					= -50,
	MSD_UP_55					=  55,
	MSD_DOWN_55					= -55,
	MSD_UP_60					=  60,
	MSD_DOWN_60					= -60,
	MSD_UP_65					=  65,
	MSD_DOWN_65					= -65,
	MSD_UP_70					=  70,
	MSD_DOWN_70					= -70,
	MSD_UP_75					=  75,
	MSD_DOWN_75					= -75,
	MSD_UP_80					=  80,
	MSD_DOWN_80					= -80,
	MSD_UP_85					=  85,
	MSD_DOWN_85					= -85,
	MSD_UP_90					=  90,
	MSD_DOWN_90					= -90,
} Unit_Move_Direction;

struct ActionInfo
{
	ClientIndex_t	UnitIndex;
	DWORD			dwStartTick;
	DWORD			dwCurTick;
	float			fSumDamage;
	float			fSumAggro;		// 2006-07-21 by cmkwon

	ActionInfo()
		:UnitIndex(0), dwStartTick(0), dwCurTick(0), fSumDamage(0), fSumAggro(0)
	{};
	ActionInfo(ClientIndex_t i_cIdx, DWORD i_dwTick, float i_fDamage, float i_fAggro=0.0f)
		:UnitIndex(i_cIdx), dwStartTick(i_dwTick), dwCurTick(i_dwTick), fSumDamage(i_fDamage), fSumAggro(i_fAggro)
	{
	}

	void * operator new(size_t size)
	{
		return VMemPool::vmObjectNew(size);
	}
	void operator delete(void* p)
	{
		VMemPool::vmObjectDelete(p, sizeof(ActionInfo));
	}
};

typedef mt_vector<ActionInfo*>	mtvectorActionInfoPtr;
typedef mt_vector<ActionInfo>	mtvectActionInfo;
typedef vector<ActionInfo*>		vectorActionInfoPtr;
typedef vector<ActionInfo>		vectActionInfo;


struct ActionInfo_Sort_Object: binary_function<ActionInfo*, ActionInfo*, bool>
{
	bool operator()(ActionInfo *pAInfo1, ActionInfo *pAInfo2)
	{
        return pAInfo1->fSumDamage > pAInfo2->fSumDamage;	// 내림 차순 정렬을 위해 반대로 함
    };
};

////////////////////////////////////////////////////////////////////////////////
//
// 클래스 이름  : listActionInfoPtr_UnitIndex_Find_Object
// 클래스 설명  : vectorActionInfoPtr에서 UnitIndex를 찾는 Function Object이다
//					생성자의 인자는 찾고자하는 UnitIndex를 넣는다.
struct listActionInfoPtr_UnitIndex_Find_Object
{
	listActionInfoPtr_UnitIndex_Find_Object(int index): m_nFindIndex(index){};
	bool operator()(const ActionInfo *pAInfo)
	{
		return pAInfo->UnitIndex == m_nFindIndex;
	}
	int m_nFindIndex;
};

class CMonster;
typedef mt_vector<CMonster*>			mtvectorMonsterPtr;

///////////////////////////////////////////////////////////////////////////////
/// \class		CMonster
///
/// \brief
/// \author		cmkwon
/// \version
/// \date		2004-03-26 ~ 2004-03-26
/// \warning
///////////////////////////////////////////////////////////////////////////////
class CMonster : public MONSTER
{
public:
	CMonster();
	virtual ~CMonster();

	///////////////////////////////////////////////////////////////////////////
	// Property
	void SetMonsterIndex(int i_nMonsterIndex);

	//void SetMonsterInfoPtr(MONSTER_INFO * i_pMonsterInfo);
// 2010. 05. 19 by hsLee 인피니티 필드 2차 난이도 조절. (신호처리 + 몬스터 처리(서버) )
	void SetMonsterInfoPtr(MONSTER_INFO * i_pMonsterInfo , const bool a_bReset_ExtendInfo = true );
	void SetMonsterInfoExtend_AllData ( const MONSTER_BALANCE_DATA *a_pBalanceInfo );
	void SetMonsterInfoExtend_StatParam ( const FLOAT a_fHPPer = 1.0f , const FLOAT a_fDefensePer = 1.0f , const FLOAT a_fEvasionPer = 1.0f , const FLOAT a_fAtkPer = 1.0f );
	void SetMonsterInfoExtend_BonusParam ( const float a_fIncreaseExpRatio = 1.0f , const float a_fIncreaseDropItemProbabilityRatio = 1.0f , const float a_fInCreaseDropItemCountRatio = 1.0f );
// End 2010. 05. 19 by hsLee 인피니티 필드 2차 난이도 조절. (신호처리 + 몬스터 처리(서버) )

	BOOL GetMonsterDeadFlag(void);
	void SetMonsterDeadFlag(BOOL i_bDeadFlag);
	
	///////////////////////////////////////////////////////////////////////////
	// Method
	BOOL CheckDeadBodyCondition(void){		return COMPARE_BODYCON_BIT(BodyCondition, BODYCON_DEAD_MASK);}
	BOOL OnDead(vectorActionInfoPtr *o_pmtvecAttackedInfoPtr);
	ClientIndex_t GetClientIndexForTargetType(void);

	// 2010. 05. 31 by hsLee 인피니티 필드 2차 난이도 조절. (오브젝트 몬스터 밸런스 적용 문제 수정.)
	// 몬스터 경험치 리턴.
	Experience_t rtn_MonsterExperience ( void );
	// End 2010. 05. 31 by hsLee 인피니티 필드 2차 난이도 조절. (오브젝트 몬스터 밸런스 적용 문제 수정.)

	// start 2011-03-21 by hskim, 인피니티 3차 - 몬스터 멀티 타겟팅 기능 추가
	D3DXVECTOR3	GetNearMultiTarget(D3DXVECTOR3 Target,  int *i_pMultiTargetIndex);
	D3DXVECTOR3	GetPointMultiTarget(int i_pMultiTargetIndex);
	BOOL IsMultiTargetMonster();
	// end 2011-03-21 by hskim, 인피니티 3차 - 몬스터 멀티 타겟팅 기능 추가
	
	// virtual
	virtual void ResetMonster(void);
	virtual void InserttoAttackedInfoList(ActionInfo *i_pAttackedInfo);
	virtual void DeleteAttackedInfowithIndex(int i_AttackIndex = 0);

	mtvectorActionInfoPtr *GetAttackedInfoPtrList(void);
	void GetAttackedInfoClientIndexList(vectorClientIndex *o_pvectClientIdxList);

	// 상태 관련
	BYTE						m_enMonsterState;				// 몬스터의 상태 변수(MS_NULL, MS_CREATED, MS_PLAYING, MS_DEAD)

	// 공격 관련
	ClientIndex_t				m_nTargetIndex;					// 몬스터가 공격중인 인덱스
	mtvectorActionInfoPtr		m_mtvectorAttackedInfoPtr;		// 몬스터가 공격 당한 ActionInfo의 포인터 리스트

	///////////////////////////////////////////////////////////////////////////////
	// 2009-12-11 by cmkwon, 데미지 어그로로 타겟을 변경하는 몬스터 구현 - 
	DWORD						m_dwChangedTargetIndexTick;
	void SetChangedTargetIndexTick(DWORD i_dwTick);
	DWORD GetChangedTargetIndexTick(void);
	ClientIndex_t GetMaxDamageUser(void);
	inline ClientIndex_t GetTargetIndex(void) { return m_nTargetIndex; }
	void ResetSumDamageInAttackedInfoList(void);

	BOOL IsValidMonster(BOOL i_bLiveCheck=TRUE);		// 2009-12-11 by cmkwon, 데미지 어그로로 타겟을 변경하는 몬스터 구현 - CFieldMonster 클래스에 있던것을 이곳으로 이동

	// 생성 관련
	BYTE						m_byObjectMonsterType;			// Object Monster Type(0 = No Object Monster, 6 = EVENT_TYPE_OBJECT_MONSTER)
	
	BYTE						m_byMonsterTargetType;			// 몬스터가 타겟으로 잡을 수 있는 타입을 결정
	INT							m_nTargetTypeData;				// 
	mtvectClientIndex_t			m_mtvectClientIdxForTargetType;	// 

	BOOL						m_bDeadFlag;					// 몬스터 폭발 플래그, 2005-09-05 by cmkwon
};