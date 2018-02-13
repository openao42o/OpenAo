// WeaponFastData.h: interface for the CWeaponFastData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WEAPONFASTDATA_H__51B7BBE4_E0D4_4A92_82EA_CC3BC47A7EC5__INCLUDED_)
#define AFX_WEAPONFASTDATA_H__51B7BBE4_E0D4_4A92_82EA_CC3BC47A7EC5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Weapon.h"

class CAtumData;
struct ATTACK_DATA;
struct ITEM;

class CWeaponFastData : public CWeapon  
{
public:

	// 2009. 11. 23 by jskim 리소스 로딩 구조 변경
	// 2009. 08. 27 by ckPark 그래픽 리소스 변경 시스템 구현
	//CWeaponFastData(CAtumData * pAttack, ITEM * pWeaponITEM, ATTACK_DATA & attackData);
	//CWeaponFastData( CAtumData* pAttack, ITEM* pWeaponITEM, ATTACK_DATA& attackData, ITEM* pEffectItem = NULL );
	// end 2009. 08. 27 by ckPark 그래픽 리소스 변경 시스템 구현
	CWeaponFastData( CAtumData* pAttack, ITEM* pWeaponITEM, ATTACK_DATA& attackData, ITEM* pEffectItem = NULL, int LoadingPriority = _NOTHING_STEP);
	//end 2009. 11. 23 by jskim 리소스 로딩 구조 변경


//	CWeaponFastData(CAtumData * pAttack,CAtumData * pTarget,int type,MSG_FC_BATTLE_ATTACK_RESULT_PRIMARY* pMsg);//int wtType,D3DXVECTOR3 vPos);
//	CWeaponFastData(CAtumData * pAttack,CAtumData * pTarget,MSG_FC_BATTLE_ATTACK_RESULT_PRIMARY* pMsg);//int wtType,D3DXVECTOR3 vPos);
//	CWeaponFastData(CAtumData * pAttack,CAtumData * pTarget,int type,MSG_FC_BATTLE_ATTACK_ITEM_RESULT_PRIMARY* pMsg);//int wtType,D3DXVECTOR3 vPos);
//	CWeaponFastData(CAtumData * pAttack,CAtumData * pTarget,MSG_FC_BATTLE_ATTACK_ITEM_RESULT_PRIMARY* pMsg);//int wtType,D3DXVECTOR3 vPos);
	
	virtual ~CWeaponFastData();
	virtual void Tick();
	void CheckWeaponCollision(D3DXVECTOR3 vDistance, float fMovingDistance);

	FLOAT				m_fTargetDistance;			// 최초 셋팅된 타겟 위치와의 거리(충돌 체크를 위해서 미리 구해놓는다)
	INT					m_nItemIndex;
	BYTE				m_bFireType;				// 0: 좌 1: 우 2: 중
	UINT				m_nWeaponItemNumber;		// 무기 타입(각 무기종류,마인드 컨트롤종류)
	BOOL				m_bNeedCheckCollision;		// 충돌 체크가 필요한가?

	D3DXMATRIX			m_matColl;
};

#endif // !defined(AFX_WEAPONFASTDATA_H__51B7BBE4_E0D4_4A92_82EA_CC3BC47A7EC5__INCLUDED_)
