// WeaponAllAttackData.h: interface for the CWeaponAllAttackData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WEAPONALLATTACKDATA_H__B778727D_7CCE_4F40_8464_FEB126E54B49__INCLUDED_)
#define AFX_WEAPONALLATTACKDATA_H__B778727D_7CCE_4F40_8464_FEB126E54B49__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Weapon.h"
class CUnitData;
class CCharacterInfo;

///////////////////////////////////////////////////////////////////////////////
/// \class		CWeaponAllAttackData
///
/// \brief		전체 공격 이펙트 처리(공격자만 있고, 타겟이 없다. 타겟은 서버에서 결과로 줌)
///				ITEM.Range : 폭발반경(폭발 시 데미지의 영향이 미치는 반경)
///				ITEM.RangeAngle : 발사 전방 각도
///				ITEM.
///				속도 : MAX_WEAPON_SPEED
/// \author		dhkwon
/// \version	
/// \date		2004-06-24 ~ 2004-06-24
/// \warning	
///////////////////////////////////////////////////////////////////////////////
class CWeaponAllAttackData : public CWeapon
{
public:

	// 2009. 11. 23 by jskim 리소스 로딩 구조 변경
	// 2009. 08. 27 by ckPark 그래픽 리소스 변경 시스템 구현
	//CWeaponAllAttackData(CAtumData * pAttackter, CAtumData * pTarget, BodyCond_t nBodyCondition, UINT nItemNum);
	//CWeaponAllAttackData( CAtumData * pAttackter, CAtumData * pTarget, BodyCond_t nBodyCondition, UINT nItemNum, ITEM* pEffectItem = NULL );
	CWeaponAllAttackData( CAtumData * pAttackter, CAtumData * pTarget, BodyCond_t nBodyCondition, UINT nItemNum, ITEM* pEffectItem = NULL ,int LoadingPriority = _NOTHING_STEP);
	// end 2009. 08. 27 by ckPark 그래픽 리소스 변경 시스템 구현	
	//end 2009. 11. 23 by jskim 리소스 로딩 구조 변경

	virtual ~CWeaponAllAttackData();
	virtual void Tick();
protected:
	void CreateHitEffect(CAtumData* pTarget);

protected:
//	D3DXMATRIX			m_mMatrix;
//	CUnitData *			m_pAttacker;
//	CAtumNode *			m_pTarget;
//	ITEM		*		m_pITEM;
//	BodyCond_t			m_hyBodyCondition;
//	CCharacterInfo*		m_pCharacterInfo;
//	D3DXVECTOR3			m_vPos;						// 이펙트의 위치

	// 2009. 08. 27 by ckPark 그래픽 리소스 변경 시스템 구현
	ITEM*	m_pEffectItem;
	// end 2009. 08. 27 by ckPark 그래픽 리소스 변경 시스템 구현
};

#endif // !defined(AFX_WEAPONALLATTACKDATA_H__B778727D_7CCE_4F40_8464_FEB126E54B49__INCLUDED_)
