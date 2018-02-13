// WeaponFireData.h: interface for the CWeaponFireData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WEAPONFIREDATA_H__F480BD02_6FE7_4955_84D6_906C6147F36D__INCLUDED_)
#define AFX_WEAPONFIREDATA_H__F480BD02_6FE7_4955_84D6_906C6147F36D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Weapon.h"

class CAtumData;
class CWeaponFireData : public CWeapon  
{
public:
	CWeaponFireData(CAtumData * pAttack,int wtType);
	virtual ~CWeaponFireData();
	virtual void Tick();

};

#endif // !defined(AFX_WEAPONFIREDATA_H__F480BD02_6FE7_4955_84D6_906C6147F36D__INCLUDED_)
