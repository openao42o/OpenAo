// UnitData.h: interface for the CUnitData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_UNITDATA_H__2D386C99_DEB3_45E7_A7B5_A88B6CB2666D__INCLUDED_)
#define AFX_UNITDATA_H__2D386C99_DEB3_45E7_A7B5_A88B6CB2666D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AtumData.h"
// 2007-04-02 by bhsohn Move패킷으로 적기 상태 체크
#define	UNIT_STATE_INVISIBLE	0
#define	UNIT_STATE_CHARINGSHOT	1
#define	UNIT_STATE_HYPERSHOT	2
// 2013-01-30 by mspark, A기어 배리어 스킬 워프후에 상대방에게 보이지 않는 문제 수정
#define UNIT_STATE_BARRIER		3
// end 2013-01-30 by mspark, A기어 배리어 스킬 워프후에 상대방에게 보이지 않는 문제 수정
// 2013-02-14 by mspark, M기어 무적 스킬 워프후에 상대방에게 보이지 않는 문제 수정
#define UNIT_STATE_INVINCIBLE	4
// end 2013-02-14 by mspark, M기어 무적 스킬 워프후에 상대방에게 보이지 않는 문제 수정
// 2012-11-29 by mspark, 서치아이 위치가 이동하는 캐릭터의 위치와 동일하도록 수정
#define UNIT_STATE_SEARCHEYE	5
#define UNIT_STATE_SEARCHEYE2	6
// end 2012-11-29 by mspark, 서치아이 위치가 이동하는 캐릭터의 위치와 동일하도록 수정
#define	UNIT_STATE_MAX			7

// 2013-01-30 by mspark, A기어 배리어 스킬 워프후에 상대방에게 보이지 않는 문제 수정
#define	AGEAR_STATE_BARRIER		7821060		// 배리어
// end 2013-01-30 by mspark, A기어 배리어 스킬 워프후에 상대방에게 보이지 않는 문제 수정

// 2013-02-14 by mspark, M기어 무적 스킬 워프후에 상대방에게 보이지 않는 문제 수정
#define	MGEAR_STATE_INVINCIBLE	7811100		// 무적
// end 2013-02-14 by mspark, M기어 무적 스킬 워프후에 상대방에게 보이지 않는 문제 수정

#define	BGEAR_STATE_CHARINGSHOT		7800060		// 차징샷 
#define	AGEAR_STATE_HYPERSHOT		7823060		// 하이퍼샷 

// 2007-05-17 by bhsohn 오브젝트 뒤에 숨었을시 에 대한 처 검사 처리
#define	CHECK_OBJECT_TIME		1.0f

///////////////////////////////////////////////////////////////////////////////
/// \class		CUnitData
///
/// \brief		CShuttleChild, CEnemyData, CMonsterData의 부모로서, 공통부분 처리
/// \author		dhkwon
/// \version	
/// \date		2004-03-20 ~ 2004-03-20
/// \warning	
///////////////////////////////////////////////////////////////////////////////
class CSkillEffect;
class CItemData;
class CPetManager;		// 2010-06-15 by shcho&hslee 펫시스템 - 클래스 추가
class CUnitData : public CAtumData  
{
	friend class CAtumApplication;	// 차후에 지울 것.
	friend class CUnitRender;		// m_pVBShadow 만 쓸것
	friend class CCharacterRender;	// 2005-07-13 by ispark
	friend class CMonsterRender;	// m_pVBShadow 만 쓸것
public:
	CUnitData();
	virtual ~CUnitData();

	int		GetUnitNum() const { return m_nUnitNum; }
	int		GetPilotNum() const { return m_nPilotNum; }			// 2005-07-13 by ispark 파일롯트 넘버
	HRESULT InitDeviceObjects() override;
	HRESULT RestoreDeviceObjects() override;
	HRESULT InvalidateDeviceObjects() override;
	HRESULT DeleteDeviceObjects() override;

	HRESULT	RestoreShadow();		// CUnitRender, CMonsterRender의 RestoreDeviceObjects()에서 호출한다.
	HRESULT	InvalidateShadow();		// CUnitRender, CMonsterRender의 InvalidateDeviceObjects()에서 호출한다.

public:
// 유닛 상태
	void	ChangeUnitState( DWORD dwState );
	// 2010. 03. 18 by jskim 몬스터변신 카드
	//void	ChangeUnitCharacterInfo(int nDefenseItemNum, int nUnitKind, BodyCond_t nBodyCon, BOOL bCharacter);
	void	ChangeUnitCharacterInfo(int nDefenseItemNum, int nUnitKind, BodyCond_t nBodyCon, BOOL bCharacter, BOOL bMonsterTransformer = FALSE);
	//end 2010. 03. 18 by jskim 몬스터변신 카드
// 바디컨디션
	void	ChangeUnitBodyCondition(BodyCond_t hyBodyCondition);
	void	ChangeSingleBodyCondition( BodyCond_t hySingleBodyCondition );
	void	ChangeKeepingBodyCondition( BodyCond_t hyBodyCondition );
	void	TurnSingleBodyCondition(BodyCond_t hySingleBodyCondition, BOOL bSet);
	float	GetCurrentAnimationTime(void);
	float	GetCurrentBodyConditionEndAnimationTime(void);
	BodyCond_t GetCurrentBodyCondition(void);
	void	ResetBodyCondition( BodyCond_t hyBody );
	void	ChangeBodyCondition( BodyCond_t hyBody );
	void	SetFlyBodyCondition( BodyCond_t hyBody );
	void	SetExBodyCondition( BodyCond_t hyBody );

// 무기
	virtual void CreateSecondaryShieldDamage(D3DXVECTOR3 vCollPos) {};
	float	GetAutomaticAttackTime( BYTE nOrbitType );
	
	// 2009. 08. 27 by ckPark 그래픽 리소스 변경 시스템 구현

// 	void	CreatePrimaryWeaponItem(ITEM* pItem);
// 	void	CreateSecondaryWeaponItem(ITEM* pItem);
	void	CreatePrimaryWeaponItem( ITEM* pItem, ITEM* pShapeItem );
	void	CreateSecondaryWeaponItem( ITEM* pItem, ITEM* pShapeItem );

	// end 2009. 08. 27 by ckPark 그래픽 리소스 변경 시스템 구현

	void	CreateWearItem( int nWearItemKind, int nEffectNum, BOOL bCharacter = FALSE);		// 2006-09-20 by ispark
	void	DeleteWearItem( int nWearItemKind );
	int		GetTotalShotNumPerReattackTime(ITEM* pWeaponITEM, CParamFactor *pParamFactor);
	int		GetMultiNum(ITEM* pWeaponITEM, CParamFactor *pParamFactor);

	void	CreateWeaponByFieldServer( MSG_FC_BATTLE_ATTACK_OK* pMsg);

	// 2009. 11. 23 by jskim 리소스 로딩 구조 변경
	// 2009. 08. 27 by ckPark 그래픽 리소스 변경 시스템 구현
	//void	CreateWeapon(ATTACK_DATA& attackData, ITEM* pWeaponITEM);
	//void	CreateWeapon( ATTACK_DATA& attackData, ITEM* pWeaponITEM, ITEM* pEffectItem = NULL );
	// end 2009. 08. 27 by ckPark 그래픽 리소스 변경 시스템 구현
	void	CreateWeapon( ATTACK_DATA& attackData, ITEM* pWeaponITEM, ITEM* pEffectItem = NULL, int LoadingPriority = _NOTHING_STEP);
	//end 2009. 11. 23 by jskim 리소스 로딩 구조 변경

	// 2009. 08. 27 by ckPark 그래픽 리소스 변경 시스템 구현
	//virtual void CheckAttack(ATTACK_DATA &attackData, BOOL bReset, ITEM* pWeaponITEM);
	virtual void CheckAttack(ATTACK_DATA &attackData, BOOL bReset, ITEM* pWeaponITEM = NULL, ITEM* pEffectItem = NULL);
	// end 2009. 08. 27 by ckPark 그래픽 리소스 변경 시스템 구현
	static CAppEffectData * CreateWearItemEffectAtSelectMenu( GUIUnitRenderInfo *pInfo, int nEffectNum, BodyCond_t nBodyCondition );

	void	ResetPrimaryWeaponBodyCondition( BodyCond_t hyBodyCon);
	void	ChangePrimaryWeaponBodyCondition( BodyCond_t hyBodyCon);
	void	ResetSecondaryWeaponBodyCondition( BodyCond_t hyBodyCon, BOOL i_bIsLeft=TRUE);
	void	ChangeSecondaryWeaponBodyCondition( BodyCond_t hyBodyCon);
	void	SetWeaponVelFromServer( D3DXVECTOR3 vWeaponVel );
	// 2010-06-15 by shcho&hslee 펫시스템 - 시즈 모드
	void	ResetPetWeaponBodyCondition( BodyCond_t hyBodyCon, BOOL i_bIsLeft=TRUE);
	void	SetPetVelFromServer( D3DXVECTOR3 vPetLeftVel, D3DXVECTOR3 vPetRightVel );
	// end 2010-06-15 by shcho&hslee 펫시스템 - 시즈 모드

	// 2009. 07. 07 by ckPark 로봇기어 요청사항(롤링, 선택화면, 무기, A기어포대)
	void	ShowWeapon( const BOOL bShow );
	void	CreateRobotAGearWeapon( ITEM* pAGearWeapon, const UINT nUnitType );
	void	DeleteRobotAGearWeapon( void );
	// end 2009. 07. 07 by ckPark 로봇기어 요청사항(롤링, 선택화면, 무기, A기어포대)

// PK
	void	SetPkState( DWORD dwPkFlag, BOOL bSet );
	BOOL	IsPkState( DWORD dwPkFlag );
	BOOL	IsPkEnable() { return (m_dwPkState != PK_NONE ? TRUE : FALSE); } // Enemy : 내가 공격 가능한 Enemy 인가?, ShuttleChild : 전투중
	BOOL	IsPkAttackEnable() { return m_dwPkState != PK_NONE && m_dwPkState != PK_NORMAL; }
	DWORD	GetPkState() { return m_dwPkState; }
	BOOL	GetbPkAttack() { return m_bPkAttack; }
	void	SetbPkAttack(BOOL bAttack) { m_bPkAttack = bAttack; }
	int		GetUnitNumFromCharacter(int nDefenseItemNum, int nUnitKind, BodyCond_t nBodyCon, BOOL bCharacter, int nDegree);

	void	ResetPrimaryWeaponBodyCondition( BodyCond_t hyBodyCon, BOOL bZig);

	void	ChangeWearItemBodyCondition(int nWearItemKind, BodyCond_t hyBodyCondition);	

// 투명
	BYTE	GetUnitAlphaState() { return m_bySkillStateFlag; }
	int		GetUnitAlphaValue() { return m_nAlphaValue; }
	void	SetUnitAlpha(BYTE bySkillState, int nAlphaValue);
	void	InitUnitAlpha();

// 체프
	void	AddChaffData(CAtumNode* pUnitData, MSG_FC_BATTLE_DROP_DUMMY_OK* pMsg);
	void	DelChaffData(UINT nItemIndex);
	void	DelChaffDataAll();
	CItemData* FindChaffDataByFieldIndex ( UINT nFieldIndex );
	void	ChaffDataTick();
	int		GetChaffDataCound();

	void	AddScanObject(CAtumNode* pUnitData,float fCheckTime);
	void	ScanDataTick();

	// 2007-04-02 by bhsohn Move패킷으로 적기 상태 체크
	FLOAT	GetSkillChgTime(int nIdx);
	void	AddSkillChgTime(int nIdx, FLOAT fUnitStateChgCap);
	bool	IsUnitState(int nIdx);
	void	SetUnitSkillState(int nIdx, bool bOnOff);

	// 2007-05-17 by bhsohn 오브젝트 뒤에 숨었을시 에 대한 처 검사 처리
	virtual BOOL	IsCheckObjectShow();
	virtual BOOL	IsCheckObjectColl();
	// 2010. 03. 18 by jskim 몬스터변신 카드
	void SetMonsterTransformer(int MonNum) { m_MonsterTransformer = MonNum; }
	int	 GetMonsterTransformer() { return m_MonsterTransformer; }
	void SetMonsterTransScale( float Scale ) { m_MonsterTransScale = Scale; }
	float GetMonsterTransScale() { return m_MonsterTransScale; }
	void SetMonsterTransPrimarySidePos( D3DXVECTOR3 sidePos ) { m_MonsterTransPrimarySidePos = sidePos; }
	void SetMonsterTransSecondarySidePos( D3DXVECTOR3 sidePos ) { m_MonsterTransSecondarySidePos = sidePos; }
	D3DXVECTOR3	GetMonsterTransPrimarySidePos() { return m_MonsterTransPrimarySidePos; }
	D3DXVECTOR3	GetMonsterTransSecondarySidePos() { return m_MonsterTransSecondarySidePos; }
	//end 2010. 03. 18 by jskim 몬스터변신 카드

	// 2010-06-15 by shcho&hslee 펫시스템 - 펫 공격 처리
	CPetManager*		GetPetManager() { return m_pPetManager; }
	void				SetPetManager(CPetManager* i_PetManager) { m_pPetManager = i_PetManager; }
	void				HidePet( const BOOL bHide );
	//end 2010-06-15 by shcho&hslee 펫시스템 - 펫 공격 처리

	// 2013-03-06 by bhsohn 복귀 유저 시스템
	// 복귀 유저인지 여부	
	virtual void SetRtnGameUser(BYTE byUsingReturnItem) {return ;}
	virtual BYTE GetRtnGameUser() {return 0;}
	// END 2013-03-06 by bhsohn 복귀 유저 시스템

protected:
//장착 아이템
	void	ReloadWearItem();
	void	DeleteWearItemEffect( CAppEffectData * pEffect );
	CAppEffectData * CreateWearItemEffect( int nWearItemKind, int nEffectNum, D3DXVECTOR3 vPos, int nBodyCondition, int nWeaponPositionIndex = 0 );
	void	ResetWearItemBodyCondition( int nWearItemKind, CAppEffectData * pEffect, BodyCond_t hyBodyCon);
	void	DeleteItemPoint();
// 무기
	virtual USHORT GetShiftWeaponBodyconditionByUnitKind();
	void	DelSecondWeapon();

	// 2009. 07. 07 by ckPark 로봇기어 요청사항(롤링, 선택화면, 무기, A기어포대)
public:
	// 2013-01-08 by jhjang 로봇아머에 외형 변경 킷을 사용했을때 문제가 발생하던 부분 수정
	BOOL	IsRobotArmor_Old( void );// 2013-01-08 by jhjang 기존 로봇 아머 체크 함수
	BOOL	IsRobotArmor( void );	// 2013-01-08 by jhjang 외형 변경 관련 로봇 아머 체크 함수
	// end 2013-01-08 by jhjang 로봇아머에 외형 변경 킷을 사용했을때 문제가 발생하던 부분 수정
	// end 2009. 07. 07 by ckPark 로봇기어 요청사항(롤링, 선택화면, 무기, A기어포대)

public:
	CParamFactor			m_paramFactor;			// ParamFactor( Enemy인 경우 1형의 샷넘,리어택타임만 관리)
	LPDIRECT3DVERTEXBUFFER9 m_pVBShadow;			// 그림자 버퍼

	//////////////////////////////////////////////////////////////////////////
	// 스킬
	vector<CItemData *>		m_pChaffData;			// 2006-12-04 by ispark
	CSkillEffect*			m_pSkillEffect;
	int						m_nAlphaValue;			// 알파값
	BYTE					m_bySkillStateFlag;		// 스킬 상태 (위장, 인비지블...)
	ClientIndex_t			m_nTargetIndex;					// Enemy의 타겟 인덱스
	
	// 2009. 11. 23 by jskim 리소스 로딩 구조 변경
	int						m_LoadingPriority;
	//end 2009. 11. 23 by jskim 리소스 로딩 구조 변경

protected:
	int						m_nUnitNum;				// ex> 12030100 ( 12 : degree, 03 : amor, 01 : unitKind )
	int						m_nPilotNum;			// 2005-07-13 by ispark	파일롯트 넘버
	BOOL					m_bCharacter;			// 2005-07-13 by ispark (Unit : FALSE, Character : TRUE)
	DWORD					m_dwPkState;
	BOOL					m_bPkAttack;			// 2005-11-03 by ispark PK 공격 가능한가, m_dwPkState가 Normal이어도 이것이 TRUE이면 락온 된 상태

	// 2007-04-02 by bhsohn Move패킷으로 적기 상태 체크
	BYTE					m_byUnitState;
	FLOAT					m_fUnitStateChgCap[UNIT_STATE_MAX];		// 유닛 상태가 변경되어있었던 시간 

	// 2007-05-17 by bhsohn 오브젝트 뒤에 숨었을시 에 대한 처 검사 처리
	BOOL					m_bCheckObjectEnemyShow;		// 오브젝트 뒤에 숨었을시 에 대한 처 검사 처리
	BOOL					m_bCheckObject;					// 오브젝트 체크
	float					n_fCheckObjectTime;
	// 2010. 03. 18 by jskim 몬스터변신 카드
	int						m_MonsterTransformer;
	D3DXVECTOR3				m_MonsterTransPrimarySidePos;
	D3DXVECTOR3				m_MonsterTransSecondarySidePos;
	float					m_MonsterTransScale;
	//end 2010. 03. 18 by jskim 몬스터변신 카드

	CPetManager*			m_pPetManager;
};

#endif // !defined(AFX_UNITDATA_H__2D386C99_DEB3_45E7_A7B5_A88B6CB2666D__INCLUDED_)
