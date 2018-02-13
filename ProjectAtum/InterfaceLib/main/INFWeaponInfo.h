// INFWeaponInfo.h: interface for the CINFWeaponInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFWEAPONINFO_H__E615F872_AE06_4C2C_AEE8_890BB13F7420__INCLUDED_)
#define AFX_INFWEAPONINFO_H__E615F872_AE06_4C2C_AEE8_890BB13F7420__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INFBase.h"

//#define LEFT_OVERHEAT_START_X	60
//#define RIGHT_OVERHEAT_START_X	33
#define OVERHEAT_START_Y		3
#define OVERHEAT_LENGTH			90
#define OVERHEAT_HEIGHT			4
#define FUEL_GAGE_LENGTH		62
#define FUEL_GAGE_HEIGHT		7

#define FLASH_TIME				0.5f


class CGameData;
class CINFImage;
class CINFImageEx;		// 2011. 10. 10 by jskim UI시스템 변경
class CINFWeaponInfo : public CINFBase  
{
public:
	CINFWeaponInfo(CAtumNode* pParent);
	virtual ~CINFWeaponInfo();

	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	void RenderLeftWeapon(float x, float y);		// 2011. 10. 10 by jskim UI시스템 변경
	void RenderRightWeapon(float x, float y);		// 2011. 10. 10 by jskim UI시스템 변경
	void ChangeWeapon(int nWeapon);
#ifdef _WARRIOR_ADDITIONAL_INFO
	void RenderWeaponNumber( int nWindowPosX, int nWindowPosY, int nValue, int isTransparent);
	void RenderWarningWeaponNumber( int nWindowPosX, int nWindowPosY, int nValue, int isTransparent);
#else
	void RenderWeaponNumber( int nWindowPosX, int nWindowPosY, int nValue);			// 2011. 10. 10 by jskim UI시스템 변경
	void RenderWarningWeaponNumber( int nWindowPosX, int nWindowPosY, int nValue);		// 2011. 10. 10 by jskim UI시스템 변경
#endif
	void RenderWeaponFuel( int nWindowPosX, int nWindowPosY, float fRate);		// 2011. 10. 10 by jskim UI시스템 변경
#else
	void RenderLeftWeapon();
	void RenderRightWeapon();
	void ChangeWeapon(int nWeapon);
	void RenderWeaponNumber( int nWindowPosY, int nWindowPosX, int nValue);
	void RenderWeaponFuel( int nWindowPosY, int nWindowPosX, float fRate);
#endif
	virtual void Tick();
	virtual int WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
public:
	CGameData		*	m_pWeaponData ;
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	// 2011. 10. 10 by jskim UI시스템 변경
	CINFImageEx*	m_pStImage;
	CINFImageEx*	m_pStWeaponImage;
	CINFImageEx*	m_pNdImage;
	CINFImageEx*	m_pNdWeaponImage;
	CINFImageEx*	m_pStOverHeatBar;
	CINFImageEx*	m_pNdOverHeatBar;
	CINFImageEx*	m_pEmptyWeaponImage;
	CINFImageEx*	m_pWeaponNumberImage[10];
	CINFImageEx*	m_pWeaponWarningNumberImage[10];
	CINFImageEx*	m_pStFuelGageBar;
	CINFImageEx*	m_pStFuelUnderGageBar;
//	CINFImageEx*	m_pNdFuelGageBar;
//	CINFImageEx*	m_pNdFuelUnderGageBar;
	CINFImageEx*	m_pReloadImage;
	CINFImageEx*	m_pOverHeatImage;
	CINFImageEx*	m_pBulletEmptyImage;
	CINFImageEx*	m_pBulletEmptyImage1;
	CINFImageEx*	m_pBulletEmptyImage2;
	CINFImageEx*	m_pBulletLowImage;
	CINFImageEx*	m_pBulletLowImage1;
	// end 2011. 10. 10 by jskim UI시스템 변경													  
#else 
 	CINFImageEx*	m_pStImage;
	CINFImageEx*	m_pStWeaponImage;
	CINFImageEx*	m_pNdImage;
	CINFImageEx*	m_pNdWeaponImage;
	CINFImageEx*	m_pStOverHeatBar;
	CINFImageEx*	m_pNdOverHeatBar;
	CINFImageEx*	m_pEmptyWeaponImage;
	CINFImageEx*	m_pWeaponNumberImage[10];
	CINFImageEx*	m_pStFuelGageBar;
	CINFImageEx*	m_pStFuelUnderGageBar;
//	CINFImageEx*	m_pNdFuelGageBar;
//	CINFImageEx*	m_pNdFuelUnderGageBar;
	CINFImageEx*	m_pReloadImage;
	CINFImageEx*	m_pOverHeatImage;
	CINFImageEx*	m_pBulletEmptyImage;
	CINFImageEx*	m_pBulletLowImage;
#endif
	int			m_nLeftWeaponInfoPosX ;
	int			m_nRightWeaponInfoPosX ;
	float		m_fFlashTime;
	BOOL		m_bFlash;

};

#endif // !defined(AFX_INFWEAPONINFO_H__E615F872_AE06_4C2C_AEE8_890BB13F7420__INCLUDED_)
