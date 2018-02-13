// Skill.h: interface for the CSkill class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SKILL_H__5A26A431_9BF7_4616_8ADB_BBBEF7A4D820__INCLUDED_)
#define AFX_SKILL_H__5A26A431_9BF7_4616_8ADB_BBBEF7A4D820__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CSkillInfo;
class CAppEffectData;
class CUnitData;
class CSkill  
{
	friend class CINFSkill;
public:
	CSkill();
	virtual ~CSkill();

	void		Tick(float fElaspedTime);
	CSkillInfo*	PutSkill(ITEM_SKILL* pItemSkill);		// 스킬 추가
	void		DeleteSkill(LONGLONG UniqueNumber);		// 스킬 삭제
	void		UseSkill(CSkillInfo* pItemSkill);		// 스킬 사용
	void		ReleaseSkill(CSkillInfo* pItemSkill);	// 스킬 사용 해체
	void		CancelSkill(int nSkillItemNum, BOOL i_bEnemyUsingSkillCancel);	
	void		CancelSkillByBaseNum(int nSkillItemNum, BOOL i_bEnemyUsingSkillCancel);	
	
	ITEM*		FindItem(int nSkillItemNumber);			// SkillItemNumber 로 부터 ITEM* 를 찾아주는 함수
	CSkillInfo* FindItemSkill(int nSkillItemNumber);	// SkillItemNumber 로 부터 ITEM_SKILL* 를 찾아주는 함수
	CSkillInfo* FindUsingSkillInfo(int nSkillItemNum);	// 현재 사용중인 스킬
	CSkillInfo* FindEnemyUsingSkillForMeInfo(int nSkillItemNum);	//현재 적이 나에게 사용한 스킬.
	CSkillInfo* FindEnemyUsingSkillForMeInfoByBaseNum(int nSkillItemNum);	//현재 적이 나에게 사용한 스킬.
	CSkillInfo* FindUsingSkillInfoByBaseNum(int nSkillItemNum);	// 현재 사용중인 스킬중 같은 BASENUM을 같은 스킬
	CSkillInfo* FindUsingSkillExistInfoByBaseNum(int nSkillItemNum);		//현재 발동중인 스킬.
	int			FindItemSkillLevelByBaseNum(int nSkillBaseNum); // 0 : 존재하지 않는 스킬
	void		DisableSkillEffect(CUnitData* pUnit, int nEffectNum);

	BOOL		DeleteUsingSkillInfo(int nSkillItemNum);
	BOOL		DeleteUsingSkillInfoByBaseNum(int nSkillItemNum);
	BOOL		DeleteEnemyUsingSkillInfo(int nSkillItemNum);
	BOOL		DeleteEnemyUsingSkillInfoByBaseNum(int nSkillItemNum);

	// 2007-02-07 by dgwoo 스캔 스킬관련.
	BOOL		ISScanSkill()								{return m_bScan;}
	
	// 2007-04-19 by bhsohn 서치아이 아이템 추가
	//void		SetScanSkill(BOOL bScan)					{m_bScan = bScan;}
//	float		GetScanRange()								{return m_fScanRange;}
//	void		SetScanRange(float ScanRange)				{m_fScanRange = ScanRange;}	
	void		SetScanSkill(int nIdx, BOOL bScan);
	float		GetScanRange(int nIdx);
	void		SetScanRange(int nIdx, float ScanRange);

	void		SetScanPosition(int nIdx, D3DXVECTOR3	vScanPosition);
	BOOL		IsCanSeeInvisible(D3DXVECTOR3 vPos);						// 인비지블 대상을 볼수 있는지 판단

	
	


	char*		GetSkillName(int nSkillItemNumber);			// SkillItemNumber 로 스킬의 이름을 얻어옴
	void		PutSkillFromWearItem(BYTE nItemKind);	// 아이템 교체로 nImteKind에 관련된 스킬 활성화(패시브)
	BOOL		GetSmartSpeedState() { return m_bSmartSpeed; }
	void		SetSmartSpeedState(BOOL bSmartSpeed) { m_bSmartSpeed = bSmartSpeed;	}
	int			GetSkillPatternType() { return m_nSkillPatternType; }
	void		SetSkillPatternType(int nSkillPatternType) { m_nSkillPatternType = nSkillPatternType; }

	// 2009. 09. 21 by ckPark 인피니티 필드 몬스터 스킬 구현
	void		SetMonsterHoldState( const BOOL bHold) { m_bMonsterHold = bHold; }
	BOOL		GetMonsterHoldState( void ) { return m_bMonsterHold; }
	// end 2009. 09. 21 by ckPark 인피니티 필드 몬스터 스킬 구현

	void		DeleteSkillFromWearItem(BYTE nItemKind);// 아이템 교체로 nItemKind에 관련된 스킬 해제
	void		ReleaseAllUsingSkillWithOutPermanent();	// Permanent만 제외하고 사용중인 모든 스킬을 해제한다.
	void		ReleaseAllUsingToggleSkill();			// 사용중인 모든 Toggle 스킬을 해제한다.
	void		ReleaseGroundSkill();					// 사용중인 모든 지상에서 사용되는 스킬을 해제한다.
	void		ReleaseAllChargingSkill();				// 사용중인 모든 Charging 스킬을 해제한다.
	void		ReleseSkill(int nType, INT nAttackSkillItemNum = 0);	// 스킬을 해제한다.

	BOOL		IsSatisfyReqItemKind(BYTE nReqItemKind, BYTE nItemKind);

	void		SendPrepareUseSkill(CSkillInfo* pItemInfo);	// 서버로 스킬이 발동됨을 알려준다.
	void		SendPrepareCancelSkill();				// 서버로 발동된 스킬이 취소됨을 알려준다.
	void		SetChargingShotState(int nChargingShotState) { m_nChargingShotState = nChargingShotState; }
	int			GetChargingShotState() { return m_nChargingShotState; }

	void		CheckChargingShotSkill();				// 현재 사용중인 차징샷 스킬 번호를 리턴한다.
	UINT		GetChargingShotTypeSkillNum();			// 2006-12-06 by ispark, 챠징샷 타입 스킬 넘버 가져오기
	void		SetPrimarySkill(CSkillInfo* pSkillInfo) { m_pPriSkillInfo = pSkillInfo; }	// 현재 사용중인 1형무기 Skill
	CSkillInfo*	GetPrimarySkill() { return m_pPriSkillInfo; }								// 현재 사용중인 1형무기 Skill
	void		SetSecondarySkill(CSkillInfo* pSkillInfo) { m_pSecSkillInfo = pSkillInfo; }	// 현재 사용중인 2형무기 Skill
	CSkillInfo*	GetSecondarySkill() { return m_pSecSkillInfo; }								// 현재 사용중인 2형무기 Skill

	void			SetSkillTargetState(BOOL bSkillTargetState) { m_bSkillTargetState = bSkillTargetState; }
	BOOL			GetSkillTargetState() { return m_bSkillTargetState; }
	void			SetTargetIndex(ClientIndex_t nTargetIndex) { m_nTargetIndex = nTargetIndex; }
	ClientIndex_t	GetTargetIndex() { return m_nTargetIndex; }
	void			SetCharacterUID(UID32_t nCharacterUID) { m_nCharacterUID = nCharacterUID; }
	UID32_t			GetCharacterUID() { return m_nCharacterUID; }

	BOOL		FindTargetForSkill();					// 마우스 포인터를 기준으로 가장 가까운 내 주변에 Enemy를 구한다.(클릭)
	void		ReleaseAllPrepareTargetOneSkill();		// 발동중인 모든 TARGET TYPE ONE 스킬을 해제한다.
	void		ReleasePrepareTargetOneSkill(int nSkillNumber);			//타겟형 스킬을 삭제하고 초기화한다.
	void		SafeDeleteSkillInfo(CSkillInfo* pSkillInfo);	
	void		EraseUsingSkill(CSkillInfo* pSkillInfo, BOOL bEnemyDelete = FALSE);

	// 2013-03-27 by bhsohn DestParam 자료형 수정
//	BOOL		FindDestParamUseSkill(ITEM * pItem, BYTE byDestParameter);
	BOOL		FindDestParamUseSkill(ITEM * pItem, DestParam_t byDestParameter);
	// END 2013-03-27 by bhsohn DestParam 자료형 수정
	void		ReleaseAllUsingControlSkill(int i_nExcludeSkillNum);

	void		SetSkillConfirmData(MSG_FC_SKILL_CONFIRM_USE* pMsg);
	void		UseSkillConfirm(int i);
	BOOL		IsSkillOverBooster();
	CSkillInfo*	CheckConfirmSkillUse();

	CSkillInfo* FindItemEnemySkillInfo(int nSkillItemNumber);	// SkillItemNumber 로 부터 ITEM_SKILL* 를 찾아주는 함수

	// 2007-04-05 by bhsohn 빅붐 버그 처리
	INT			GetPreSkillItemNum();
	void		InitPreSkillItemNum();

	// 2007-10-22 by bhsohn 타켓형 스킬 오류에 대한 처리
	void		ReleasePrePareSkill();

	BOOL		IsAttackPossibility();


	// 2009-03-30 by bhsohn 차징샷 이펙트 버그 수정
	BOOL		IsSkillUse(INT	ItemNum);
	// end 2009-03-30 by bhsohn 차징샷 이펙트 버그 수정

	// 2008-08-27 by bhsohn HP/DP동시에 차는 아이템 추가
	BOOL		IsShieldParalyze();

	
	// 2009. 09. 21 by ckPark 인피니티 필드 몬스터 스킬 구현

	BOOL		IsExistDesParamSKill( BYTE nDesParam );

	// end 2009. 09. 21 by ckPark 인피니티 필드 몬스터 스킬 구현


	// 2008-10-23 by bhsohn 자기 자신한테 힐 단축키 추가
	BOOL		IsTargetToMyShuttle(BYTE	i_bySkillTargetType);
	BOOL		FindTargetForSkill_ToMe();	 
	
	// 2008. 12. 23 by ckPark 스킬 사용 지연 사용 지연
	void		SendConfirmSkillUse(CSkillInfo* pSkillInfo, UID32_t TargetCharacterUniqueNumber);

	// 2008-12-26 by bhsohn 엠기어 편대 버그 수정
	void		EraseLowLVUsingSkill(ClientIndex_t	i_AttackIndex, INT i_nSkillItemNum);

	// 2008-12-29 by bhsohn 사망 시 버프 스킬 유지 시스템
	void		ReleaseStatDEAD_AllUsingSkillWithOutPermanent();

	// 2013-05-07 by bhsohn 세력포인트 개선안 시스템
	void		DisableTimeLimitSkill(INT ItemNum);

private:
	int			FindSkillItemNum(LONGLONG UniqueNumber);// UniqueNumber 로 부터 SkillItemNumber 를 찾아주는 함수
	BOOL		IsSingleSkill(int nSkillItemNum);		// nSkillItemNum 에 해당하는 스킬이 컨드롤 스킬,시즈,폭격 스킬인가?			
	BOOL		IsExistSingleSkill();					// 컨트롤 스킬,시즈,폭격은 Single스킬이다.
	BOOL		IsExistTargetOneSkill(CSkillInfo *pItemSkill);				// 타켓을 클릭해야 발동하는 스킬이 발동중인가?
	BOOL		FindListTargetForSkill(POINT pt);		// 편대 리스트에서 스킬 발동
	BOOL		IsExistMultyNGround();					// 멀티타겟이나 지상폭격을 쓰는지 확인
	BOOL		IsUseSkill(CSkillInfo* pItemSkill);
	BOOL		IsUseEventMap(CSkillInfo* pItemSkill);	// 이벤트 맵에서 사용 가능한 스킬인가?
	BOOL		IsSkillPossibility();					// 스킬을 사용가능한가?
	BOOL		IsSkillPossibilitySlience();		  // 2011-10110 by jhahn EP4 침묵 스킬 추가
	BOOL		IsGroundSkill(CSkillInfo *pItemSkill);	// 지상에서 사용하는 스킬이냐?
	BOOL		IsCancelSkill(CSkillInfo* pSkillInfo);		// 정화 스킬 사용시 릴리즈 스킬인가?

	// 2008. 12. 23 by ckPark 스킬 사용 지연 사용 지연
	//void		SendConfirmSkillUse(CSkillInfo* pSkillInfo, UID32_t TargetCharacterUniqueNumber);


	// 2007-05-23 by bhsohn 스킬 안나가는 문제 처리
	void		InitTargetState();

	// 2008-12-29 by bhsohn 사망 시 버프 스킬 유지 시스템
	BOOL	IsReleaseSkill_StatDEAD(CSkillInfo* i_pSkillInfo);

public:
	map<int, CSkillInfo*>	m_mapSkill;					// 사용 가능한 모든 스킬 (key:ItemNum, value:skill info)
	map<int, CSkillInfo*>	m_mapEnemySkillInfo;		// 다른 사람이 사용한 스킬(key:ItemNum, value:skill info)
	vector<CSkillInfo*>		m_vecUsingSkill;			// 나 자신에게 사용한 스킬.
	vector<CSkillInfo*>		m_vecEnemyUsingSkillForMe;	// 다른 유저가 나한테 쓴 스킬.
	// 2007-04-19 by bhsohn 서치아이 아이템 추가
	//D3DXVECTOR3				m_vScanPosition;			// 스캔한 중심 좌표.
	

private:
	BOOL					m_bSmartSpeed;				// SmartSpeed 스킬을 사용중인가?
	// 2009. 09. 21 by ckPark 인피니티 필드 몬스터 스킬 구현
	BOOL					m_bMonsterHold;
	// end 2009. 09. 21 by ckPark 인피니티 필드 몬스터 스킬 구현
	int						m_nSkillPatternType;		// 현재 사용중인 패턴 타입
	int						m_nChargingShotState;		// 차징샷의 상태
	BOOL					m_bScan;					// 현재 스캔버프스킬을 받고 있는 상태인가?
	//float					m_fScanRange;
	CSkillInfo*				m_pPriSkillInfo;			// 현재 사용중인 1형무기 Skill 을 저장하기위한 임시변수
	CSkillInfo*				m_pSecSkillInfo;			// 현재 사용중인 2형무기 Skill 을 저장하기위한 임시변수
	BOOL					m_bSkillTargetState;		// 현재 마우스 포인터 타켓 클릭 상태
	ClientIndex_t			m_nTargetIndex;				// 타켓이 있는 스킬인 경우 TargetIndex;
	UID32_t					m_nCharacterUID;			// 클라이언트 인덱스가 유효하지 않을 때 유니크넘버를 입력
	MSG_FC_SKILL_CONFIRM_USE m_stSkillConfirm;

	// 2007-04-05 by bhsohn 빅붐 버그 처리
	INT						m_nPreSkillItemNum;			// 사용을 시도중인 기술

	// 2007-04-19 by bhsohn 서치아이 아이템 추가
	INT						m_nScanState;
	D3DXVECTOR3				m_vScanPosition[MAX_SCAN];			// 스캔한 중심 좌표.
	float					m_fScanRange[MAX_SCAN];				// 스캔 범위 

	// 2009. 04. 06 by ckPark 콜오브히어로시 다른 타겟팅 스킬 못쓰는 문제
public:
	MSG_FC_SKILL_CONFIRM_USE GetSkillConfirm( void ) { return m_stSkillConfirm; }
	// end 2009. 04. 06 by ckPark 콜오브히어로시 다른 타겟팅 스킬 못쓰는 문제
};

#endif // !defined(AFX_SKILL_H__5A26A431_9BF7_4616_8ADB_BBBEF7A4D820__INCLUDED_)
