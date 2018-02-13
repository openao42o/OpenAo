// WeaponShieldData.h: interface for the CWeaponShieldData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WEAPONSHIELDDATA_H__9BFEEA51_C5AD_4BD6_85D0_4DFD2FA0C197__INCLUDED_)
#define AFX_WEAPONSHIELDDATA_H__9BFEEA51_C5AD_4BD6_85D0_4DFD2FA0C197__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Weapon.h"

///////////////////////////////////////////////////////////////////////////////
/// \class		CWeaponShieldData
///
/// \brief		쉴드 아이템 처리
/// \author		dhkwon
/// \version	
/// \date		2004-05-14 ~ 2004-05-14
/// \warning	
///////////////////////////////////////////////////////////////////////////////

class CAtumData;
class CUnitData;
class CWeaponShieldData : public CWeapon  
{
public:
	CWeaponShieldData(CAtumData* pAttackter, int nItemNum, D3DXVECTOR3 vTarget);
	virtual ~CWeaponShieldData();
	virtual void Tick();
};

#endif // !defined(AFX_WEAPONSHIELDDATA_H__9BFEEA51_C5AD_4BD6_85D0_4DFD2FA0C197__INCLUDED_)
