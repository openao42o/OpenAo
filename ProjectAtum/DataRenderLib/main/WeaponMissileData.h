// WeaponMissileData.h: interface for the CWeaponMissileData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WEAPONMISSILEDATA_H__ABC87E4F_0E3A_4332_8E31_E024CE50F46E__INCLUDED_)
#define AFX_WEAPONMISSILEDATA_H__ABC87E4F_0E3A_4332_8E31_E024CE50F46E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "WSlowData.h"
#include "Chat.h"

class CItemData;
class CAtumData;
class CCinema;
struct ITEM;
struct ATTACK_DATA;

class CWeaponMissileData : public CWSlowData  
{
public:
	// 2009. 11. 23 by jskim 리소스 로딩 구조 변경
	// 2009. 08. 27 by ckPark 그래픽 리소스 변경 시스템 구현
	//CWeaponMissileData(CAtumData * pAttack, ITEM * pWeaponITEM, ATTACK_DATA & attackData);
	//CWeaponMissileData( CAtumData * pAttack, ITEM * pWeaponITEM, ATTACK_DATA & attackData, ITEM* pEffectItem = NULL  );
	// end 2009. 08. 27 by ckPark 그래픽 리소스 변경 시스템 구현
	CWeaponMissileData( CAtumData * pAttack, ITEM * pWeaponITEM, ATTACK_DATA & attackData, ITEM* pEffectItem = NULL , int LoadingPriority = _NOTHING_STEP);
	//end 2009. 11. 23 by jskim 리소스 로딩 구조 변경

//	CWeaponMissileData(CAtumData * pAttack,CAtumData * pTarget,MSG_FC_BATTLE_ATTACK_RESULT_SECONDARY* pMsg);//float fDist,int nWeaponIndex, int nClientIndex,D3DXVECTOR3 vTargetPos);
//	CWeaponMissileData(CAtumData * pAttack,CAtumData * pTarget,MSG_FC_BATTLE_ATTACK_ITEM_RESULT_SECONDARY* pMsg);//float fDist,int nWeaponIndex, int nClientIndex,D3DXVECTOR3 vTargetPos);

	
	virtual ~CWeaponMissileData();
	void Tick() override;
	virtual void SkillTick();
	void InitData();

	void CheckWeaponCollision(CItemData *pTargetItem,float fMovingDistance);

protected:
	void CheckTargetWarning();
	void CheckTargetByBomb(float fMovingDistance);
	void CheckTargetState();

public:
	INT					m_nTargetIndex;
	INT					m_nTargetItemFieldIndex;
	// 2005-07-19 by ispark
	INT					m_nTargetMe;
	CCinema		*		m_pCinema;
	float				m_fFireTime;
	// 2007-06-12 by dgwoo 폭발 반경(공폭시 : 미사일이 폭반에 의해 미리 터진다.
	//								 지폭시 : 땅에 터질경우 폭반안에 있으면 데미지를 입는다.)
	float				m_fExplosionRange;			

	// 2009. 08. 27 by ckPark 그래픽 리소스 변경 시스템 구현
	ITEM*				m_pEffectItem;
	// end 2009. 08. 27 by ckPark 그래픽 리소스 변경 시스템 구현
	// 2009. 11. 23 by jskim 리소스 로딩 구조 변경
	int					m_LoadingPriority;
	//end 2009. 11. 23 by jskim 리소스 로딩 구조 변경
	// 2011. 03. 08 by jskim 인피3차 구현 - 넌 타겟 시스템
	int m_nMultiTargetIndex;
	// end 2011. 03. 08 by jskim 인피3차 구현 - 넌 타겟 시스템	 

#ifdef COLLCHECKLAG
	float m_fLastCollisionDistance;
	D3DXVECTOR3 m_vLastVelocity;
#endif
};

#endif // !defined(AFX_WEAPONMISSILEDATA_H__ABC87E4F_0E3A_4332_8E31_E024CE50F46E__INCLUDED_)
