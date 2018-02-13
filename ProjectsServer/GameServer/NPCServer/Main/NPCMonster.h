// NPCMonster.h: interface for the CNPCMonster class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NPCMONSTER_H__17A3BE4F_8D53_496C_8631_94D066B1AEC7__INCLUDED_)
#define AFX_NPCMONSTER_H__17A3BE4F_8D53_496C_8631_94D066B1AEC7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Monster.h"
#include "HPAction.h"		// 2009-09-09 ~ 2010 by dhjin, 인피니티 -
#include "NPCSkillManager.h"	// 2010-03-31 by dhjin, 인피니티(기지방어) -

typedef struct
{
	ClientIndex_t	AttackIndex;
	ITEM			*pAttackITEM;
	DWORD			dwExpireTick;			// 2005-12-30 by cmkwon, 종료 Tick으로 변경
	UINT			AttackedItemIndex;
} AttackedItemInfo;
typedef mt_vector<AttackedItemInfo>		mtvectorAttackedItemInfo;

typedef struct
{
	ClientIndex_t	m_TargetIndex;						// 공격중인 인덱스
	INT				WeaponIndex;
	MONSTER_ITEM	*pAttackMonsterItem;
	DWORD			m_dwShootedTick;					// 몬스터가 아이템을 사용한 시각	
} ShootedSecondaryItemInfo;
typedef mt_vector<ShootedSecondaryItemInfo*>		mtvectorShootedSecondaryItemInfoPtr;

typedef struct _MOVEFLAGS
{
	SHORT	MovableFlag:2;						// 이동 가능 Flag
	SHORT	FBFlag:2;							// FrontAndBack Flag
	SHORT	FBDirect:2;							// FrontAndBack Direction
	SHORT	LRDirect:2;							// LeftAndRight	Direction
	SHORT	UDDirect:2;							// UpAndDown Direction
	SHORT	SpeedDirect:2;						// Speed Up or Down
} MOVEFLAGS;

typedef struct _MOVEINFO: public MOVEFLAGS
{	
	BYTE 	FBChangeBaseCount;					// FrontAndBack
	BYTE 	FBChangeCount;						// FrontAndBack
	BYTE	FBChangeRandomCount;
	BYTE	FBCurrentCount;						//

	BYTE 	LRChangeBaseCount;					// LeftAndRight
	BYTE 	LRChangeCount;						// LeftAndRight
	BYTE	LRChangeRandomCount;
	BYTE	LRCurrentCount;						// 
	BYTE	LRMoveDirection;					// 방향 이동 최소값
	BYTE	LRMoveDirectionRandomCount;			// 뱡향 이동 최대값	
	
	BYTE 	LRSubChangeBaseCount;				// LeftAndRight
	BYTE 	LRSubChangeCount;					// LeftAndRight
	BYTE 	LRSubChangeRandomCount;				//
	BYTE	LRSubCurrentCount;					//	
	BYTE	LRSubMoveDirection;

	BYTE 	UDChangeBaseCount;					// UpAndDown
	BYTE 	UDChangeCount;						// UpAndDown
	BYTE 	UDChangeRandomCount;				// UpAndDown
	BYTE	UDCurrentCount;						//
	BYTE	UDMoveDirection;
	BYTE	UDMoveDirectionRandomCount;

	BYTE	UDSubChangeBaseCount;
	BYTE	UDSubChangeCount;
	BYTE	UDSubChangeRandomCount;
	BYTE	UDSubChangePercent;
	BYTE	UDSubCurrentCount;
	BYTE	UDSubMoveDirection;
	
	SHORT	MoveCount;
} MOVEINFO;

#define GET_STRING_VECTOR3(pVec3Pos)	GNPCGetStringVector3(pVec3Pos, string())
const char * GNPCGetStringVector3(D3DXVECTOR3 *i_pVec3Pos, string &str);


//////////////////////////////////////////////////////////////////////
// Local Global Function
//////////////////////////////////////////////////////////////////////
int GNPCRotateTargetVectorHorizontal(D3DXVECTOR3 *pDestTargetVector3, const D3DXVECTOR3 *pSourTargetVector3, int enDirection, int nRandomRange);
int GNPCRotateTargetVectorVertical(D3DXVECTOR3 *pDestTargetVector3, const D3DXVECTOR3 *pSourTargetVector3, int enDirection, int nRandomRange);
D3DXMATRIX GNPCGetMaxTargetVector(D3DXVECTOR3 *pCurTarget, D3DXVECTOR3 *pNextTarget, float fMaxRotationAngle);

///////////////////////////////////////////////////////////////////////////////
/// \class		CNPCMonster
///
/// \brief		NPC Server 에서 사용하는 몬스터 클래스
/// \author		cmkwon
/// \version	
/// \date		2004-03-25 ~ 2004-03-25
/// \warning	
///////////////////////////////////////////////////////////////////////////////
// start 2011-05-23 by hskim, 인피니티 3차 - 웨이포인트 구현
class CWayPointManager;
// end 2011-05-23 by hskim, 인피니티 3차 - 웨이포인트 구현

class CNPCMapChannel;
class CNPCMapProject;
class CLIENT_INFO;
class CNPCMonster : public CMonster  
{
	friend CNPCMapChannel;
	friend CNPCMapProject;

	typedef struct
	{
		BYTE	EnforceTargetVector:1;				// 강제 타겟 벡터 변경이 설정 중인지의 플래그
		BYTE	MoveTargetVectorSetAttack:1;		// 공격시 MoveTargetVector이 타겟을 향하도록 처리가 되었는지의 플래그
	} ST_NPCMONSTER_FLAG;
	
public:
	CNPCMonster();
	virtual ~CNPCMonster();

	///////////////////////////////////////////////////////////////////////////
	// Property
	void SetTargetIndex(ClientIndex_t i_nTargetIdx);
	void SetMoveState(SHORT i_MoveState);
	void SetMoveTargetVector(D3DXVECTOR3 * i_pVector3);
	void SetTargetVector(D3DXVECTOR3 * i_pVector3, BOOL i_bUnconditionalFlag=FALSE);
	void SetCurrentSpeed(float i_fSpeed);
	float GetCurrentSpeed(void);
	void SetTimeSetEnforceTargetVector(DWORD i_dwCurTick);
	void SetTimeEnforceTargetVectorTerm(USHORT i_usTerm);
	void SetTimeQuickTurnTerm(USHORT i_usRandTerm);
	void SetSpeedDirectMoveInfo(SHORT i_sSpeedDirect);
	void SetCurrentMonsterForm(SHORT i_sMonForm);
	void SetCurrentMovePattern(SHORT i_sMovePat);
	void SetUsingMonsterItem(MONSTER_ITEM *i_pMonItem);
	void SetCurrentTick(DWORD i_dwCurTick);

	// NPCFlag
	void SetNPCMonsterFlagEnforceTargetVector(BOOL i_bFlag);
	void SetNPCMonsterFlagMoveTargetVectorSetAttack(BOOL i_bFlag);


	///////////////////////////////////////////////////////////////////////////
	// Method
	// 생성 관련
	void CreateNPCMonster(MONSTER_INFO * i_pMonInfo, D3DXVECTOR3 *i_pCreatePos, DWORD i_dwCurTick, int i_CreateInfoIndex
		, BYTE i_byMonTargetType, int i_nTargetTypeData, ClientIndex_t i_cltIdxForTargetType, int i_nMapMonsterVisibleDiameter
		, int i_nTargetIndex=0, BYTE i_byObjectMonsterType=EVENT_TYPE_NO_OBJECT_MONSTER, D3DXVECTOR3 *i_pUnitVec3ObjMonsterTarget=NULL
		, MONSTER_BALANCE_DATA * i_pMonInfoBalance = NULL );
	void InitializeMonster(void);
	void SetMonsterMoveInfo(void);

	// 이동 관련
	float GetSpeed(void);
	float GetQuickSpeed(void);
	void UpdateCurrentSpeed(void);
	void ResetLastAttackTime(DWORD i_dwCurTick);
	void ResetAllLastReAttackTime(DWORD i_dwCurTick);	
	void ResetCurrentShotNumCount(void);
	void UpdatePositionVector(float i_fTimeRate);
	void UpdateEnforceTargetVector(void);
	BOOL CheckMoveRange(void);
	void UpdateMoveInfoAllCurrentCount(void);
	void UpdateMoveInfoAttack(D3DXVECTOR3 *i_pVec3Pos, D3DXVECTOR3 * i_pUnitVec3Tar);
	void SetEnforceTargetVector(D3DXVECTOR3 * i_pVector3, float i_fSpeed, SHORT i_MoveState);
	void ResetPartyVariable(void);
	void ChangeBodyCondition(BodyCond_t * pBodyCon);
	void ResetAttackBodyCondition(void);
		
	// 공격 관련
	int GetFirstTargetIndexfromAttackedInfo(void);
	void SelectTargetIndex(int nTargetIdx = 0);
	void LockVectorAttackedItemInfo(void);
	void UnlockVectorAttackedItemInfo(void);
	void InsertAttackedItemInfo(AttackedItemInfo * i_pItemInfo);
	BOOL SelectUsingMonsterItem(void);
	BOOL CheckEnableSearchTarget(void);

	// virtual
	virtual void ResetMonster(void);
	virtual void InserttoAttackedInfoList(ActionInfo *i_pAttackedInfo);
	virtual void DeleteAttackedInfowithIndex(int i_AttackIndex = 0);


	static void GetRandomPlaneUnitVector(D3DXVECTOR3 * o_pVec3Tar);
	static void GetRandomVector(D3DXVECTOR3 * o_pVec3Tar);

	///////////////////////////////////////////////////////////////////////////////
	// 2008-04-16 by cmkwon, 몬스터 사망 시 몬스터 소환 이벤트 시스템 구현 - 
	mtvectSSUMMON_EVENT_MONSTER		m_summonEventMonsterListAfterDead;
	void SetSummonEventMonsterListAfterDead(mtvectSSUMMON_EVENT_MONSTER *i_pSummonEvMonList);
	BOOL CheckSummonEventMonsterListAfterDead(SSUMMON_EVENT_MONSTER *o_pSummonEventMonster);

	////////////////////////////////////////////////////////////////////////////////
	// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 
	void SetUserPositionVector(D3DXVECTOR3 * i_pUserPos, D3DXVECTOR3 * i_pUserTarget);
	BOOL SetCurrentMapChannel(CNPCMapChannel * i_pCurrentMapChannel);

	// 2009-12-11 by cmkwon, 데미지 어그로로 타겟을 변경하는 몬스터 구현 - 
	BOOL IsChangeableTarget(void);

	///////////////////////////////////////////////////////////////////////////////
	// 2010-01-06 by cmkwon, 몬스터 공격시 타겟 공격 가능 체크 추가(타겟변경) - 
	DWORD						m_dwTimeCheckValidTarget;			// 1초마다 타겟 유효성 체크를 위한 시간
	DWORD						m_dwTimeCheckedLastValidTarget;		// 마지막 유효한 타겟 체크 시간
	DWORD GetTimeCheckValidTarget(void);
	void SetTimeCheckValidTarget(DWORD i_dwTick);
	DWORD GetTimeCheckedLastValidTarget(void);
	void SetTimeCheckedLastValidTarget(DWORD i_dwTick);

	///////////////////////////////////////////////////////////////////////////////
	// 2010-03-16 by cmkwon, 인피2차 MtoM, MtoC 타겟 변경 관련 수정 - 
	DWORD						m_dwTimeLastCheckChangeTarget;
	DWORD GetTimeLastCheckChangeTarget(void);
	void SetTimeLastCheckChangeTarget(DWORD i_dwTick);

	///////////////////////////////////////////////////////////////////////////////
	// 2010-03-31 by dhjin, 인피니티(기지방어) -
	void CheckExpireSkill();			// 2010-03-31 by dhjin, 인피니티(기지방어) - 
	void GetSkillTargets(vector<ClientIndex_t> *o_pVectorClientIndex, ClientIndex_t i_TargetIdx);	// 2010-03-31 by dhjin, 인피니티(기지방어) - 스킬 타겟 타입으로 대상 인덱스 가져오기
	void UseSkill(ClientIndex_t i_TargetIdx);			// 2010-03-31 by dhjin, 인피니티(기지방어) - 스킬 사용!!
	BOOL UseOneSkillETC(ClientIndex_t i_TargetIdx);			// 2010-03-31 by dhjin, 인피니티(기지방어) - 예외 처리가 필요한 한번 사용 스킬 
	void UseSkillClick(vectClientIndex_t * i_pTargetIdxList);		// 2010-03-31 by dhjin, 인피니티(기지방어) - 클릭형 
	void UseSkillTimeLimit(vectClientIndex_t * i_pTargetIdxList);	// 2010-03-31 by dhjin, 인피니티(기지방어) - 타임리밋형

	// start 2011-05-23 by hskim, 인피니티 3차 - 웨이포인트 구현
	CWayPointManager *GetWayPointManager();
	// end 2011-05-23 by hskim, 인피니티 3차 - 웨이포인트 구현

protected:
	// NPC Monster에만 필요한 Monster DB정보
	SHORT						CurrentMonsterForm;					// Current Monster 형태, NPC Server에서는 변화 한다.
	SHORT						CurrentMovePattern;					// Current MovePattern
	SHORT						SendMoveRange;						// Move 전송 Range, 지름
	
	// CNPCMonster에서 사용하는 모든 Flag
	ST_NPCMONSTER_FLAG			m_FlagNPCMonster;

	// 생성 정보
	USHORT						m_dwIndexCreatedMonsterData;		// 몬스터가 생성된 영역 정보 인덱스
	DWORD						m_dwTimeCreated;					// 몬스터가 생성된 Tick
	DWORD						m_dwTimeDeath;						// 2005-12-17 by cmkwon, 몬스터가 폭발된 Tick
	
	// 이동 관련
	SHORT						m_enMoveState;						// 몬스터의 이동 상태 변수(MSS_NORMAL, MSS_NORMAL_MAP_IMPACT, MSS_NORMAL_MONSTER_IMPACT, ...)
	MOVEINFO					m_MoveInfo;							//
	float						m_CurrentSpeed;						// 몬스터의 현재 Speed
	USHORT						m_usSpeedPercent;					// 몬스터의 Speed 백분율(2-2형 공격을 당했을때 사용하기 위한것)
	D3DXVECTOR3					m_BeforePosition;					//
	D3DXVECTOR3					m_MoveTargetVector;					// 지상 몬스터(점프형, 두더지형)와 콥터형 몬스터의 실제 이동을 위한 타겟벡터변수
	D3DXVECTOR3					m_CreatedPosition;					//
	DWORD						m_dwTimeLastMoved;					// 몬스터의 마지막 이동 시간
//	DWORD						m_dwTimeDead;						// 2008-04-16 by cmkwon, 몬스터 사망 시 몬스터 소환 이벤트 시스템 구현 - 사용하지 않는 변수임, // 몬스터가 폭발한 시간		
	int							m_nTimeGapChangeMonsterForm;		// 2가지 형태의 MonsterForm을 가진몬스터가 사용
	DWORD						m_dwLastRealMoveTime;				// 몬스터가 이동시 주위에 캐릭터거 없으면 이동을 하지 않는다. 캐릭터거 있을때 이동한 마지막 이동시간

	// 직진형 모스터 이동 관련
	DWORD						m_dwAttackMinDistance;				// 직진형 몬스터가 공격시 캐릭과 가까워지는 최소 거리, 이 거리 보다 가까워지면 QuickTurn이동함
	
	// 빠르게 이동 관련
	D3DXVECTOR3					m_EnforceTargetVector;				// 몬스터가 다음에 최대한 맞추어야할 타겟벡터
	DWORD						m_dwTimeSetEnforceTargetVector;		// EnforceTargetVector이 설정된 시간
	USHORT						m_usTimeEnforceTargetVectorTerm;	// EnforceTargetVector이 지속될 시간
	USHORT						m_usTimeQuickTurnTerm;				// 콥터형이 몬스터가 공격시 QuickTurn을 재 사용하기 위한 시간

	// start 2011-05-23 by hskim, 인피니티 3차 - 웨이포인트 구현
	INT							m_nWayPointNowStep;					// 현재 웨이포인트 위치
	INT							m_nWayPointDirection;				// 현재 웨이포인트 진행 방향 (정방형 +1, 역방향 -1)
	// end 2011-05-23 by hskim, 인피니티 3차 - 웨이포인트 구현

	// 공격 처리 루틴 관련
	DWORD						m_dwTimeLastAttackRoutine;			// 공격 루틴에 들어간 마지막 시간

	// 공격 관련	
	mtvectorAttackedItemInfo	m_mtvectorAttackedItemInfo;			// 몬스터가 공격당했을때 Item 정보
	MONSTER_ITEM				*m_pUsingMonsterItem;				// 몬스터가 현재 사용중이 아이템의 포인터
	DWORD						m_dwTimeChangeUsingMonsterItem;		// 몬스터가 사용 아이템을 바꾼 시간
	DWORD						m_dwCurrentTick;					
	DWORD						m_dwTimeMonsterLastAttack;			// 몬스터가 아이템으로 공격한 마지막 시간
// 2009-12-11 by cmkwon, 데미지 어그로로 타겟을 변경하는 몬스터 구현 - CMonster로 이동
//	DWORD						m_dwSetTargetIndexLastTick;			// 몬스터가 TargetIndex를 설정한 마지막 시간

	
	int							m_nCurrentShotNumCount;
	DWORD						m_ArrLastReattackTime[ARRAY_SIZE_MONSTER_ITEM];
	DWORD						m_dwLastAttackTime;

	// 몬스터 파티 관련
	ClientIndex_t				m_nPartyManagerIndex;				// 몬스터 파티장 Index(자신과 같으면 자신이 파티장이며 0이 초기값으로 파티 상태가 아님)
	BYTE						m_byPartyMemberCounts;				// 파티원 카운트 - 자신을 뺀 나머지 파티원들의 카운트, 파티원 카운트가 0이면 파티를 해제한다
	BYTE						m_byPartyFormationIndex;			// 파티원 인덱스

	// 기타
	DWORD						m_dwLastHPRecoveryTime;				// 몬스터 HP Recovery Time
// 20040528 cmkwon, 2형 무기는 NPC에서 발사만 하고 관리는 하지 않는다, 이동이나 타격은 Client에서 처리한다.
//	mtvectorShootedSecondaryItemInfoPtr	m_mtvectorShootedSecondaryItemInfoPtr;		// 몬스터가 발사한 2형 무기 정보, ItemKind에 따라 life Time을 처리하기 위해, 이동은 클라이언트에서

	DWORD						m_dwSearchTargetLastTick;			// 2007-06-26 by cmkwon, 몬스터 타겟 검색 시간 수정 - 마지막 검색 시간 

	/////////////////////////////////////////////////////////////////////////////
	// 2009-09-09 ~ 2010 by dhjin, 인피니티 -
	CHPAction					m_HPAction;						// 2009-09-09 ~ 2010 by dhjin, 인피니티 - HP에 따른 행동
	ItemIdx_t					m_BarrierUseItemArrayIdx;			// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 배리어 스킬 배열 인덱스
	CNPCMapChannel	*			m_CurrentNPCMapChannel;				
	CNPCSkillManager			m_SkillManager;					// 2010-03-31 by dhjin, 인피니티(기지방어) -
};

#endif // !defined(AFX_NPCMONSTER_H__17A3BE4F_8D53_496C_8631_94D066B1AEC7__INCLUDED_)
