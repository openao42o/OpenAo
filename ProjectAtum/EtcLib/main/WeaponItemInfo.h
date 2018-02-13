// WeaponItemInfo.h: interface for the CWeaponItemInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WEAPONITEMINFO_H__103FED3F_02AF_4D41_9EC6_B76719B779E3__INCLUDED_)
#define AFX_WEAPONITEMINFO_H__103FED3F_02AF_4D41_9EC6_B76719B779E3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ItemInfo.h"

class CItemInfo;
class CWeaponMineData;
class CParamFactor;
class CAtumData;
class CItemData;
class CUnitData;
struct MINE_DATA
{
	vector<CWeaponMineData *>	m_vecMine;					// 현재 발사된 마인
	int							m_nMaxMineNumber;			// 최대 마인 숫자
};
// 2006-02-01 by ispark, 타겟 거리 비교해서 타겟 정렬 리스트

// 2011. 03. 08 by jskim 인피3차 구현 - 넌 타겟 시스템
struct TARGET_DATA
{
	int nTargetIndex;
	int nTargetMultiIndex;
};
// end 2011. 03. 08 by jskim 인피3차 구현 - 넌 타겟 시스템

struct stTargetList
{
	int		nTargetIndex;
	float	fTargetLength;
	// 2011. 03. 08 by jskim 인피3차 구현
	int		nMultiIndex;   
    // end 2011. 03. 08 by jskim 인피3차 구현

	stTargetList()
	{
		nTargetIndex = 0;
		fTargetLength = 0.0f;
        // 2011. 03. 08 by jskim 인피3차 구현
		nMultiIndex	=	0;					 
        // end 2011. 03. 08 by jskim 인피3차 구현
	}
};

struct sort_List
{
	bool operator()(stTargetList p1, stTargetList p2)
	{
		 return p1.fTargetLength < p2.fTargetLength; 
	}
};

///////////////////////////////////////////////////////////////////////////////
/// \class		CWeaponItemInfo
///
/// \brief		장착 무기에 대한 사용
/// \author		dhkwon
/// \version	
/// \date		2004-07-20 ~ 2004-07-20
/// \warning	ShuttleChild만 뽑는다. UnitData는 안뽑는다.
///////////////////////////////////////////////////////////////////////////////
class CWeaponItemInfo  
{
public:
	CWeaponItemInfo(CItemInfo* pItemInfo, CParamFactor * pParamFactor, float fOverHeatTime=0, BOOL bOverHeat=FALSE);
	virtual ~CWeaponItemInfo();

	void	SetAttackMode(BYTE nAttackMode) { m_nAttackMode = nAttackMode; }
	BYTE	GetAttackMode() { return m_nAttackMode; }
//	void	SetServerSyncReattackOk(BOOL bOK);
	float	GetOverHeatRate() {	return m_fOverHeatCheckTime/CAtumSJ::GetOverheatTime(m_pItemInfo->GetRealItemInfo(),m_pCharacterParamFactor); }
	float	GetReattackTimeRate() { return m_fReattackCheckTime/CAtumSJ::GetShotCountReattackTime(m_pItemInfo->GetRealItemInfo(),m_pCharacterParamFactor); }
	float	GetOverHeatTime() { return CAtumSJ::GetOverheatTime(m_pItemInfo->GetRealItemInfo(),m_pCharacterParamFactor); }
	float	GetOverHeatCheckTime() { return m_fOverHeatCheckTime; }
	BOOL	IsOverHeat() { return m_bOverHeat; }
	void	Tick(float fElapsedTime, BOOL bUse);
	void	TickSecondaryWeapon2(float fElapsedTime, BOOL bUse);
	void	TickNormalWeapon(float fElapsedTime, BOOL bUse);
	// 2010-06-15 by shcho&hslee 펫시스템 - 펫 공격 처리
	void	TickPetWeapon(float fElapsedTime, BOOL bUse);
	//end 2010-06-15 by shcho&hslee 펫시스템 - 펫 공격 처리	

	//float	CheckPrimaryInNet(CUnitData* pTarget);
	float	CheckPrimaryInNet(CUnitData* pTarget, BOOL bEqualTarget2Enemy=FALSE);
	// 2011. 03. 08 by jskim 인피3차 구현 - 넌 타겟 시스템
    float	CheckPrimaryMultiInNet(CUnitData* pTarget, BOOL bEqualTarget2Enemy = FALSE, int nMultiIndex = NULL);
	BOOL	CheckMultiInNet( CUnitData* pTarget, int nMultiIndex );
	// end 2011. 03. 08 by jskim 인피3차 구현 - 넌 타겟 시스템

	BOOL	CheckSecondaryInNet(CUnitData* pTarget);
	void	SetWeaponCount(INT nCount);
	void	BulletReloaded(MSG_FC_BATTLE_PRI_BULLET_RELOADED* pMsg);
	ITEM_GENERAL* GetItemGeneral() { return m_pItemInfo; }
	ITEM*	GetRealItemInfo() { return m_pItemInfo->GetRealItemInfo(); } // 아이템 진짜 정보(인챈트,레어 정보 들어감)
	DWORD	GetAttackTick() { return m_dwLastAttackTick; }

	void	AddMine(CWeaponMineData *pMine);
	void	DeleteMine(CWeaponMineData *pMine);

	void	UsePrimary();
	void	UseSecondary();

	// 2010-06-15 by shcho&hslee 펫시스템 - 펫 무기 생성
	void	UsePetAttack();
	// end 2010-06-15 by shcho&hslee 펫시스템 - 펫 무기 생성
	void	SetUseToggleWeapon(BOOL bUseToggleWeapon) { m_bUseToggleWeapon = bUseToggleWeapon; }

	void	SetReattackCheckTime(float fReattackCheckTime) { m_fReattackCheckTime = fReattackCheckTime; }
	void	SetAttackCount(int nAttackCount) { m_nAttackCount = nAttackCount; }
	BOOL	IsUsingToggleWeapon() { return m_bUseToggleWeapon; }
	void	ReleaseAllUsingToggleWeapon();			// 모든 사용중인 토글 무기를 해제한다.

	void	DelTargetSecondary();					// 2006-09-08 by ispark, 2형 무기 타겟 제거
	void	DelTargetIndex(int nTargetIndex);

	// 2008-09-22 by bhsohn EP3 캐릭터 창
	CItemInfo *	GetClassItemInfo();
	float GetStatPierce();	
	float GetPrimaryEnchantPierce();	
	float GetSecondaryEnchantPierce();
	float GetTotalPierce(BOOL bIsPrimary);

	// 2009-02-16 by bhsohn 접두, 접무 피어스 율 안보이는 현상 처리
	float GetPreSufFixItemInfoRate(int nDesParameter);
	// end 2009-02-16 by bhsohn 접두, 접무 피어스 율 안보이는 현상 처리


protected:
	void	Use();									// 무기를 사용한다.
	// 2011. 03. 08 by jskim 인피3차 구현 - 넌 타겟 시스템
	//void	SendBattleAttackPrimary(int nTargetIndex, int nItemIndex, D3DXVECTOR3 vTargetPos);
	//void	SendBattleAttackSecondary(int nTargetIndex, int nItemIndex, D3DXVECTOR3 vTargetPos, D3DXVECTOR3 vFirePos);
	void	SendBattleAttackPrimary(int nTargetIndex, int nItemIndex, D3DXVECTOR3 vTargetPos, int nMultiIndex = 0);
	void	SendBattleAttackSecondary(int nTargetIndex, int nItemIndex, D3DXVECTOR3 vTargetPos, D3DXVECTOR3 vFirePos, int nMultiIndex = 0);
	void	SendBattleAttackPet(int nTargetIndex, int nItemIndex, D3DXVECTOR3 vTargetPos, int nMultiIndex = 0);
	// end 2011. 03. 08 by jskim 인피3차 구현 - 넌 타겟 시스템
	// 2010-06-15 by shcho&hslee 펫시스템 - 펫 무기 생성
	//void	SendBattleAttackPet(int nTargetIndex, int nItemIndex, D3DXVECTOR3 vTargetPos);
	// end 2010-06-15 by shcho&hslee 펫시스템 - 펫 무기 생성

//	void	SendBattleDropBundle();
	void	SendBattleDropMine();
	void	SendUseShield();
	void	SendUseDecoy();
	void	SendUseDummy();
	void	SendUseFixer();
	void	SendUseMissileAndRocket();

	void	SortTargetList();
	
	// 2007-05-16 by bhsohn 1형무기 타켓 처리
	void	PrimarySortTargetList();

public:
// 무기관련 추가 프로토콜 나중에 주석 지울것..jschoi
	void	SendBattleAttack(MEX_TARGET_INFO TargetInfo,UINT nSkillNum, D3DXVECTOR3 FirePosition);




	void	CheckInNet();		// 화망에 들어온 타겟 리스트 설정 (multitarget, range, angle)
	void	ResetWeaponBodyCondition(BodyCond_t hyBody);

	BOOL	IsExistBullet();	// 총알이 장전되어 있는가?
	
	// 2011. 03. 08 by jskim 인피3차 구현 - 넌 타겟 시스템
	//void	PutTargetIndex( int nRemainedTarget, int index );
	void	PutTargetIndex( int nRemainedTarget, int index, int MultiIndex = 0 );
	// end 2011. 03. 08 by jskim 인피3차 구현 - 넌 타겟 시스템

	BOOL	CheckWarpGateZone(D3DXVECTOR3 vPos);	// 가까운 거리에 워프게이트가 없는가?

public:
	// 2011. 03. 08 by jskim 인피3차 구현 - 넌 타겟 시스템
	//vector<INT>		m_vecTargetIndex;				// ClientIndex : 10000>=0 ? monster : enemy
	vector<TARGET_DATA>		m_vecTargetIndexData;				// ClientIndex : 10000>=0 ? monster : enemy
	// end 2011. 03. 08 by jskim 인피3차 구현 - 넌 타겟 시스템
	MINE_DATA *		m_pMineData;


protected:
	CItemInfo *		m_pItemInfo;
	CParamFactor *	m_pCharacterParamFactor;		// 캐릭터의 ParamFactor

	BOOL			m_bOverHeat;//m_bWeaponAction;				// 과열 상태
	FLOAT			m_fOverHeatCheckTime;//m_fWeaponActionCheckTime;	// 과열 체크 타임
	FLOAT			m_fReattackCheckTime;//m_fWeaponRACheckTime;		// 리어택 체크 타임
	FLOAT			m_fPrepareCheckTime;			// 발동 준비 체크 타임
	FLOAT			m_fAttackCheckTime;				// 공격 시간 체크 타임
	int				m_nAttackCount;					// 공격 가능한 총알 수
	BYTE			m_nAttackMode;					// 공격 타입(0 : 공중 공격 모드  1 : 지상 공격 모드)


	DWORD			m_dwLastAttackTick;
//	BYTE			m_nServerSyncShotNum;			// 총알의 싱크를 맞추기위해 서버로 부터 확인 통보를 받고 발사하게 된다.
	vector<int>		m_vecSoundCheck;				// 2형 무기 사운드 체크용
	BOOL			m_bUseToggleWeapon;				// 2-2형 토글형 무기의 사용 여부
//	float			m_fAutoSyncReattackOkTime;		// 시간이 지나면 자동으로 reattacktime * 2 만큼 세팅

	vector<stTargetList> m_vecTargetOrderList;		// 타겟 거리순 리스트

	// 2007-05-16 by bhsohn 1형무기 타켓 처리
	deque<stTargetList> m_vecTmpTargetOrderList;		// 1형무기 정렬을 위한 임시 버퍼

	//2011-10-06 by jhahn 파트너 성장형 시스템
	BOOL HpCharge;
	BOOL ShieldCharge;
	BOOL SpCharge;							  
	//end 2011-10-06 by jhahn 파트너 성장형 시스템
};

#endif // !defined(AFX_WEAPONITEMINFO_H__103FED3F_02AF_4D41_9EC6_B76719B779E3__INCLUDED_)
