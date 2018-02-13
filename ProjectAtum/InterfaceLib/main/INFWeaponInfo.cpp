// INFWeaponInfo.cpp: implementation of the CINFWeaponInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "INFWeaponInfo.h"
#include "INFGameMain.h"
#include "AtumApplication.h"
#include "StoreData.h"
#include "INFWindow.h"
#include "ShuttleChild.h"
#include "CharacterChild.h"				// 2005-07-21 by ispark
#include "ItemInfo.h"
#include "RangeTime.h"
#include "INFInven.h"
#include "INFImage.h"
#include "GameDataLast.h"
#include "WeaponItemInfo.h"
#include "dxutil.h"
#include "INFImageList.h"				// 2011. 10. 10 by jskim UI시스템 변경
#include "INFImageEx.h"					// 2011. 10. 10 by jskim UI시스템 변경

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#define LEFT_WEAPON_NUMBER g_pShuttleChild->m_pPrimaryWeapon->GetItemGeneral()->CurrentCount// 음수인 경우 연료 사용중(USE FUEL)
#define RIGHT_WEAPON_NUMBER g_pShuttleChild->m_pSecondaryWeapon->GetItemGeneral()->CurrentCount

#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
#define FIRST_WEAPON_START_X	x
#define FIRST_WEAPON_START_Y	y + 22
#define FIRST_WEAPON_FONT_X		x + 17 //(FIRST_WEAPON_START_X + 55)
#define FIRST_WEAPON_FONT_Y		y + 8  //(FIRST_WEAPON_START_Y + 10)

#define SECOND_WEAPON_START_X	x
#define SECOND_WEAPON_START_Y	y + 22
#define SECOND_WEAPON_FONT_X	x + 11//SECOND_WEAPON_START_X + 55)
#define SECOND_WEAPON_FONT_Y	y + 8//SECOND_WEAPON_START_Y + 10)

#define FIRST_WEAPON_EMPTY_X	FIRST_WEAPON_START_X - 37
#define FIRST_WEAPON_EMPTY_Y	FIRST_WEAPON_START_Y + 5 
#define SECOND_WEAPON_EMPTY_X	SECOND_WEAPON_START_X - 37
#define SECOND_WEAPON_EMPTY_Y	SECOND_WEAPON_START_Y + 5
// end 2011. 10. 10 by jskim UI시스템 변경														  
#else 
#define FIRST_WEAPON_START_X	23
#define FIRST_WEAPON_START_Y	178
#define FIRST_WEAPON_GAGE_X		(FIRST_WEAPON_START_X + 37)
#define FIRST_WEAPON_GAGE_Y		(FIRST_WEAPON_START_Y + 3)
#define FIRST_WEAPON_FONT_X		(FIRST_WEAPON_START_X + 55)
#define FIRST_WEAPON_FONT_Y		(FIRST_WEAPON_START_Y + 10)

#define SECOND_WEAPON_START_X	23
#define SECOND_WEAPON_START_Y	207
#define SECOND_WEAPON_GAGE_X	(SECOND_WEAPON_START_X + 37)
#define SECOND_WEAPON_GAGE_Y	(SECOND_WEAPON_START_Y + 3)
#define SECOND_WEAPON_FONT_X	(SECOND_WEAPON_START_X + 55)
#define SECOND_WEAPON_FONT_Y	(SECOND_WEAPON_START_Y + 10)

#endif

#define LEFT_WEAPON_NUMBER_X	5
#define LEFT_WEAPON_NUMBER_Y	57
#define WEAPON_NUMBER_SIZE		10
// 2007-10-15 by bhsohn 총알 아이템 추가 처리
//#define RIGHT_WEAPON_NUMBER_X	(g_pD3dApp->GetBackBufferDesc().Width - 45) // 45 = WEAPON_NUMBER_SIZE*4(4자리숫자) + LEFT_WEAPON_NUMBER_X
#define RIGHT_WEAPON_NUMBER_X	(g_pD3dApp->GetBackBufferDesc().Width - 52) // 50 = WEAPON_NUMBER_SIZE*5(5자리숫자) + LEFT_WEAPON_NUMBER_X

#define LEFT_FUEL_X				1
#define LEFT_FUEL_Y				59

CINFWeaponInfo::CINFWeaponInfo(CAtumNode* pParent)
{
	FLOG( "CINFWeaponInfo(CAtumNode* pParent)" );
	m_pStImage = NULL ;
	m_pStWeaponImage = NULL;
	m_pNdImage = NULL ;
	m_pNdWeaponImage = NULL;
	m_pStOverHeatBar = NULL;
	m_pNdOverHeatBar = NULL;
	m_pEmptyWeaponImage = NULL;
	m_pStFuelGageBar = NULL;
	m_pStFuelUnderGageBar = NULL;
//	m_pNdFuelGageBar = NULL;
//	m_pNdFuelUnderGageBar = NULL;
	m_pReloadImage = NULL;
	m_pOverHeatImage = NULL;
	m_pBulletEmptyImage = NULL;

#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	m_pBulletEmptyImage1 = NULL;			// 2011. 10. 10 by jskim UI시스템 변경
	m_pBulletEmptyImage2 = NULL;			// 2011. 10. 10 by jskim UI시스템 변경
    m_pBulletLowImage1 = NULL;				// 2011. 10. 10 by jskim UI시스템 변경
	m_bFlash = FALSE;						// 2011. 10. 10 by jskim UI시스템 변경
#endif
	m_pBulletLowImage = NULL;



	m_nLeftWeaponInfoPosX = 0 ;
	m_nRightWeaponInfoPosX = g_pD3dApp->GetBackBufferDesc().Width - SIZE_WEAPON_X;

	m_pParent = pParent;
	memset(m_pWeaponNumberImage, 0x00, sizeof(INT)*10);
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	memset(m_pWeaponWarningNumberImage, 0x00, sizeof(INT)*10);									  
#endif
}

CINFWeaponInfo::~CINFWeaponInfo()
{
	FLOG( "~CINFWeaponInfo()" );
	util::del(m_pStImage) ;
	util::del(m_pStWeaponImage);
	util::del(m_pNdImage) ;
	util::del(m_pNdWeaponImage);
	util::del(m_pStOverHeatBar) ;
	util::del(m_pNdOverHeatBar);
	util::del(m_pEmptyWeaponImage);
	util::del(m_pWeaponData);
	util::del(m_pStFuelGageBar);
	util::del(m_pStFuelUnderGageBar);
//	util::del(m_pNdFuelGageBar);
//	util::del(m_pNdFuelUnderGageBar);
	util::del(m_pReloadImage);
	util::del(m_pOverHeatImage);
	util::del(m_pBulletEmptyImage);
    util::del(m_pBulletLowImage);				// 2011. 10. 10 by jskim UI시스템 변경
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	util::del(m_pBulletEmptyImage1);			// 2011. 10. 10 by jskim UI시스템 변경
	util::del(m_pBulletEmptyImage2);			// 2011. 10. 10 by jskim UI시스템 변경	
	util::del(m_pBulletLowImage1);			// 2011. 10. 10 by jskim UI시스템 변경			  
#endif

	for(int i=0;i<10;i++)
	{
		util::del(m_pWeaponNumberImage[i]);
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
		util::del(m_pWeaponWarningNumberImage[i]);			// 2011. 10. 10 by jskim UI시스템 변경
#endif
	}
}

HRESULT CINFWeaponInfo::InitDeviceObjects()
{
	FLOG( "CINFWeaponInfo::InitDeviceObjects()" );
	char strPath[256];
	g_pD3dApp->LoadPath( strPath, IDS_DIRECTORY_TEXTURE, "weapon.tex");
	m_pWeaponData = new CGameData;
	m_pWeaponData->SetFile(strPath, FALSE, NULL, 0);

	m_pStImage = new CINFImageEx;			// 2011. 10. 10 by jskim UI시스템 변경
	DataHeader	* pDataHeader = FindResource("1sta");
	m_pStImage->InitDeviceObjects( pDataHeader ) ;

	m_pNdImage = new CINFImageEx;			// 2011. 10. 10 by jskim UI시스템 변경
	pDataHeader = FindResource("2nda");
	m_pNdImage->InitDeviceObjects( pDataHeader ) ;

	m_pStOverHeatBar = new CINFImageEx;			// 2011. 10. 10 by jskim UI시스템 변경
	pDataHeader = FindResource("1stgage");
	m_pStOverHeatBar->InitDeviceObjects( pDataHeader ) ;

	m_pNdOverHeatBar = new CINFImageEx;			// 2011. 10. 10 by jskim UI시스템 변경
	pDataHeader = FindResource("2ndgage");
	m_pNdOverHeatBar->InitDeviceObjects( pDataHeader ) ;

	m_pEmptyWeaponImage = new CINFImageEx;			// 2011. 10. 10 by jskim UI시스템 변경
	pDataHeader = FindResource("weslot");
	m_pEmptyWeaponImage->InitDeviceObjects( pDataHeader ) ;

	m_pStFuelGageBar = new CINFImageEx;				// 2011. 10. 10 by jskim UI시스템 변경
	pDataHeader = FindResource("beam1_g");
	m_pStFuelGageBar->InitDeviceObjects( pDataHeader ) ;

	m_pStFuelUnderGageBar = new CINFImageEx;			// 2011. 10. 10 by jskim UI시스템 변경
	pDataHeader = FindResource("beam1_b");
	m_pStFuelUnderGageBar->InitDeviceObjects( pDataHeader ) ;

/*	m_pNdFuelGageBar = new CINFImage;
	pDataHeader = FindResource("beam2_g");
	m_pNdFuelGageBar->InitDeviceObjects(pDataHeader->m_pData,pDataHeader->m_DataSize) ;

	m_pNdFuelUnderGageBar = new CINFImage;
	pDataHeader = FindResource("beam2_b");
	m_pNdFuelUnderGageBar->InitDeviceObjects(pDataHeader->m_pData,pDataHeader->m_DataSize) ;
*/
	m_pReloadImage = new CINFImageEx;				// 2011. 10. 10 by jskim UI시스템 변경
	pDataHeader = FindResource("reload");
	m_pReloadImage->InitDeviceObjects( pDataHeader ) ;
							
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	m_pOverHeatImage = new CINFImageEx;				// 2011. 10. 10 by jskim UI시스템 변경
	pDataHeader = FindResource("weover_1");
	m_pOverHeatImage->InitDeviceObjects( pDataHeader ) ;
	// 2011. 10. 10 by jskim UI시스템 변경
	m_pBulletEmptyImage = new CINFImageEx;			
	pDataHeader = FindResource("welow_1");
	m_pBulletEmptyImage->InitDeviceObjects( pDataHeader ) ;

	m_pBulletEmptyImage1 = new CINFImageEx;
	pDataHeader = FindResource("welow_11");
	m_pBulletEmptyImage1->InitDeviceObjects( pDataHeader ) ;

	m_pBulletEmptyImage2 = new CINFImageEx;
	pDataHeader = FindResource("welow_21");
	m_pBulletEmptyImage2->InitDeviceObjects( pDataHeader ) ;

	m_pBulletLowImage = new CINFImageEx;
	pDataHeader = FindResource("welow_1");
	m_pBulletLowImage->InitDeviceObjects( pDataHeader ) ;

	m_pBulletLowImage1 = new CINFImageEx;
	pDataHeader = FindResource("welow_2");
	m_pBulletLowImage1->InitDeviceObjects( pDataHeader ) ;	   
#else	 
	m_pOverHeatImage = new CINFImageEx;
	pDataHeader = FindResource("weover");
	m_pOverHeatImage->InitDeviceObjects(pDataHeader);
	m_pBulletEmptyImage = new CINFImageEx;
	pDataHeader = FindResource("weempty");
	m_pBulletEmptyImage->InitDeviceObjects(pDataHeader) ;
	m_pBulletLowImage = new CINFImageEx;
	pDataHeader = FindResource("welow");
	m_pBulletLowImage->InitDeviceObjects(pDataHeader) ;
#endif

	for(int i=0;i<10;i++)
	{
		char buf[16];
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
		wsprintf(buf, "BR%d", i);
		m_pWeaponWarningNumberImage[i] = new CINFImageEx;
		pDataHeader = FindResource(buf);
		m_pWeaponWarningNumberImage[i]->InitDeviceObjects(  pDataHeader );						  
#endif
		wsprintf(buf, "B%d", i);
		m_pWeaponNumberImage[i] = new CINFImageEx;		// 2011. 10. 10 by jskim UI시스템 변경
		pDataHeader = FindResource(buf);
		m_pWeaponNumberImage[i]->InitDeviceObjects(  pDataHeader ) ;
	}

	return S_OK ;
}

HRESULT CINFWeaponInfo::RestoreDeviceObjects()
{
	FLOG( "CINFWeaponInfo::RestoreDeviceObjects()" );
	m_nRightWeaponInfoPosX = g_pD3dApp->GetBackBufferDesc().Width - SIZE_WEAPON_X;
	if(m_pStImage )
		m_pStImage->RestoreDeviceObjects();
	if(m_pNdImage )
		m_pNdImage->RestoreDeviceObjects();
	if(m_pStOverHeatBar )
		m_pStOverHeatBar->RestoreDeviceObjects();
	if(m_pNdOverHeatBar )
		m_pNdOverHeatBar->RestoreDeviceObjects();
	if(m_pEmptyWeaponImage )
		m_pEmptyWeaponImage->RestoreDeviceObjects();
	if(m_pStFuelGageBar )
		m_pStFuelGageBar->RestoreDeviceObjects();
	if(m_pStFuelUnderGageBar )
		m_pStFuelUnderGageBar->RestoreDeviceObjects();
/*	if(m_pNdFuelGageBar )
		m_pNdFuelGageBar->RestoreDeviceObjects();
	if(m_pNdFuelUnderGageBar )
		m_pNdFuelUnderGageBar->RestoreDeviceObjects();
*/	if(m_pReloadImage )
		m_pReloadImage->RestoreDeviceObjects();
	if(m_pOverHeatImage )
		m_pOverHeatImage->RestoreDeviceObjects();
	if(m_pBulletEmptyImage )
		m_pBulletEmptyImage->RestoreDeviceObjects();
	if(m_pBulletLowImage )
		m_pBulletLowImage->RestoreDeviceObjects();

#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	if(m_pBulletEmptyImage1 )		// 2011. 10. 10 by jskim UI시스템 변경
		m_pBulletEmptyImage1->RestoreDeviceObjects();
	if(m_pBulletEmptyImage2 )		// 2011. 10. 10 by jskim UI시스템 변경
		m_pBulletEmptyImage2->RestoreDeviceObjects();

	if(m_pBulletLowImage1 )			// 2011. 10. 10 by jskim UI시스템 변경
		m_pBulletLowImage1->RestoreDeviceObjects();
#endif
	for(int i=0;i<10;i++)
	{
		m_pWeaponNumberImage[i]->RestoreDeviceObjects();

#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
		m_pWeaponWarningNumberImage[i]->RestoreDeviceObjects();									  
#endif
	}
	ChangeWeapon(1);
	ChangeWeapon(2);
	return S_OK ;
}

void CINFWeaponInfo::ChangeWeapon(int nWeapon)// nWeapon == 1 then 1형 무기, 2 then 2형 무기
{
	FLOG( "CINFWeaponInfo::ChangeWeapon(int nWeapon)" );

	if(nWeapon == 1)
	{
//		map<int, ITEM_GENERAL*>::iterator it = g_pStoreData->m_mapItemInInventory.find(POS_PROWOUT);// FIRST WEAPON
		CItemInfo *pItemInfo = g_pStoreData->FindItemInInventoryByWindowPos(POS_PROWOUT);
//		if(it != g_pStoreData->m_mapItemInInventory.end())
		if(pItemInfo)
		{
//			ITEM_GENERAL* pItem = it->second;
			char buf[16];
//			wsprintf(buf, "%08d",pItemInfo->ItemNum);	// 2005-08-23 by ispark
			wsprintf(buf, "%08d",pItemInfo->ItemInfo->SourceIndex);
			DataHeader	*  pHeader = m_pWeaponData->Find(buf);
			if(m_pStWeaponImage)
			{
				m_pStWeaponImage->InvalidateDeviceObjects();
				m_pStWeaponImage->DeleteDeviceObjects();
				util::del(m_pStWeaponImage);
			}
			if(pHeader)
			{
				m_pStWeaponImage = new CINFImageEx();		// 2011. 10. 10 by jskim UI시스템 변경
				m_pStWeaponImage->InitDeviceObjects( pHeader  );
				m_pStWeaponImage->RestoreDeviceObjects();
			}
		}
		else
		{
			if(m_pStWeaponImage)
			{
				m_pStWeaponImage->InvalidateDeviceObjects();
				m_pStWeaponImage->DeleteDeviceObjects();
				util::del(m_pStWeaponImage);
			}
		}
	}
	else if(nWeapon == 2)
	{
//		map<int, ITEM_GENERAL*>::iterator it = g_pStoreData->m_mapItemInInventory.find(POS_WINGOUT);// SECOND WEAPON
		CItemInfo *pItemInfo = g_pStoreData->FindItemInInventoryByWindowPos(POS_WINGOUT);
//		if(it != g_pStoreData->m_mapItemInInventory.end())
		if(pItemInfo)
		{
//			ITEM_GENERAL* pItem = it->second;
			char buf[16];
//			wsprintf(buf, "%08d",pItemInfo->ItemNum);	// 2005-08-23 by ispark
			wsprintf(buf, "%08d",pItemInfo->ItemInfo->SourceIndex);
			DataHeader	*  pHeader = m_pWeaponData->Find(buf);
			if(m_pNdWeaponImage)
			{
				m_pNdWeaponImage->InvalidateDeviceObjects();
				m_pNdWeaponImage->DeleteDeviceObjects();
				util::del(m_pNdWeaponImage);
			}
			if(pHeader)
			{
				m_pNdWeaponImage = new CINFImageEx();			// 2011. 10. 10 by jskim UI시스템 변경
				m_pNdWeaponImage->InitDeviceObjects( pHeader );
				m_pNdWeaponImage->RestoreDeviceObjects();
			}
		}
		else
		{
			if(m_pNdWeaponImage)
			{
				m_pNdWeaponImage->InvalidateDeviceObjects();
				m_pNdWeaponImage->DeleteDeviceObjects();
				util::del(m_pNdWeaponImage);
			}
		}
	}

}

HRESULT CINFWeaponInfo::DeleteDeviceObjects()
{
	FLOG( "CINFWeaponInfo::DeleteDeviceObjects()" );
	if(m_pStImage )
		m_pStImage->DeleteDeviceObjects();
	util::del(m_pStImage ) ;

	if(m_pStWeaponImage )
		m_pStWeaponImage->DeleteDeviceObjects() ;
	util::del(m_pStWeaponImage );

	if(m_pNdImage )
		m_pNdImage->DeleteDeviceObjects();
	util::del(m_pNdImage ) ;

	if(m_pNdWeaponImage )
		m_pNdWeaponImage->DeleteDeviceObjects() ;
	util::del(m_pNdWeaponImage );

	if(m_pStOverHeatBar )
		m_pStOverHeatBar->DeleteDeviceObjects();
	util::del(m_pStOverHeatBar ) ;

	if(m_pNdOverHeatBar )
		m_pNdOverHeatBar->DeleteDeviceObjects() ;
	util::del(m_pNdOverHeatBar );

	if(m_pEmptyWeaponImage )
		m_pEmptyWeaponImage->DeleteDeviceObjects() ;
	util::del(m_pEmptyWeaponImage );

	if(m_pStFuelGageBar )
		m_pStFuelGageBar->DeleteDeviceObjects();
	util::del(m_pStFuelGageBar );
	if(m_pStFuelUnderGageBar )
		m_pStFuelUnderGageBar->DeleteDeviceObjects();
	util::del(m_pStFuelUnderGageBar );
/*	if(m_pNdFuelGageBar )
		m_pNdFuelGageBar->DeleteDeviceObjects();
	util::del(m_pNdFuelGageBar );
	if(m_pNdFuelUnderGageBar )
		m_pNdFuelUnderGageBar->DeleteDeviceObjects();
	util::del(m_pNdFuelUnderGageBar );
*/	if(m_pReloadImage )
		m_pReloadImage->DeleteDeviceObjects();
	util::del(m_pReloadImage );
	if(m_pOverHeatImage )
		m_pOverHeatImage->DeleteDeviceObjects();
	util::del(m_pOverHeatImage );
	if(m_pBulletEmptyImage )
		m_pBulletEmptyImage->DeleteDeviceObjects();
	util::del(m_pBulletEmptyImage );
	if(m_pBulletLowImage )
		m_pBulletLowImage->DeleteDeviceObjects();
	util::del(m_pBulletLowImage );

#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	if(m_pBulletEmptyImage1 )		// 2011. 10. 10 by jskim UI시스템 변경
		m_pBulletEmptyImage1->DeleteDeviceObjects();
	util::del(m_pBulletEmptyImage1 );
	if(m_pBulletEmptyImage2 )		// 2011. 10. 10 by jskim UI시스템 변경
		m_pBulletEmptyImage2->DeleteDeviceObjects();
	util::del(m_pBulletEmptyImage2 );		// 2011. 10. 10 by jskim UI시스템 변경

	if(m_pBulletLowImage1 )		
		m_pBulletLowImage1->DeleteDeviceObjects();
	util::del(m_pBulletLowImage1 );		// 2011. 10. 10 by jskim UI시스템 변경
#endif
	for(int i=0;i<10;i++)
	{
		m_pWeaponNumberImage[i]->DeleteDeviceObjects();
		util::del(m_pWeaponNumberImage[i] );

#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
		m_pWeaponWarningNumberImage[i]->DeleteDeviceObjects();		// 2011. 10. 10 by jskim UI시스템 변경
		util::del(m_pWeaponWarningNumberImage[i] );											  
#endif
	}
	return S_OK ;
}


HRESULT CINFWeaponInfo::InvalidateDeviceObjects()
{
	FLOG( "CINFWeaponInfo::InvalidateDeviceObjects()" );
	if(m_pStImage )
		m_pStImage->InvalidateDeviceObjects() ;
	if(m_pStWeaponImage )
		m_pStWeaponImage->InvalidateDeviceObjects() ;
	if(m_pNdImage )
		m_pNdImage->InvalidateDeviceObjects() ;
	if(m_pNdWeaponImage )
		m_pNdWeaponImage->InvalidateDeviceObjects() ;
	if(m_pStOverHeatBar )
		m_pStOverHeatBar->InvalidateDeviceObjects() ;
	if(m_pNdOverHeatBar )
		m_pNdOverHeatBar->InvalidateDeviceObjects() ;
	if(m_pEmptyWeaponImage )
		m_pEmptyWeaponImage->InvalidateDeviceObjects() ;
	if(m_pStFuelGageBar )
		m_pStFuelGageBar->InvalidateDeviceObjects();
	if(m_pStFuelUnderGageBar )
		m_pStFuelUnderGageBar->InvalidateDeviceObjects();
/*	if(m_pNdFuelGageBar )
		m_pNdFuelGageBar->InvalidateDeviceObjects();
	if(m_pNdFuelUnderGageBar )
		m_pNdFuelUnderGageBar->InvalidateDeviceObjects();
*/	if(m_pReloadImage )
		m_pReloadImage->InvalidateDeviceObjects();
	if(m_pOverHeatImage )
		m_pOverHeatImage->InvalidateDeviceObjects();
	if(m_pBulletEmptyImage )
		m_pBulletEmptyImage->InvalidateDeviceObjects();
	if(m_pBulletLowImage )
		m_pBulletLowImage->InvalidateDeviceObjects();

#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	if(m_pBulletEmptyImage1 )		// 2011. 10. 10 by jskim UI시스템 변경
		m_pBulletEmptyImage1->InvalidateDeviceObjects();
	if(m_pBulletEmptyImage2 )		// 2011. 10. 10 by jskim UI시스템 변경
		m_pBulletEmptyImage2->InvalidateDeviceObjects();
	if(m_pBulletLowImage1 )			// 2011. 10. 10 by jskim UI시스템 변경
		m_pBulletLowImage1->InvalidateDeviceObjects();
#endif

	for(int i=0;i<10;i++)
	{
		m_pWeaponNumberImage[i]->InvalidateDeviceObjects();
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
		m_pWeaponWarningNumberImage[i]->InvalidateDeviceObjects();								  
#endif
	}

	return S_OK ;
}


void CINFWeaponInfo::Tick()
{
	FLOG( "CINFWeaponInfo::Tick()" );
	if(m_fFlashTime < FLASH_TIME)
	{
		m_fFlashTime += g_pD3dApp->GetCheckElapsedTime();
	}
	else
	{
		m_fFlashTime = 0;
		m_bFlash = !m_bFlash;
	}


}
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
#ifdef _WARRIOR_ADDITIONAL_INFO
#define SWTARGET_MOUSE_SCALE		((float)g_pD3dApp->GetBackBufferDesc().Width/1024.0f)
#define SWFIRE_ANGLE_START_X			(g_pD3dApp->GetBackBufferDesc().Width/2 - 45*SWTARGET_MOUSE_SCALE)
#define SWFIRE_ANGLE_START_Y			(g_pD3dApp->GetBackBufferDesc().Height/2 - 45*SWTARGET_MOUSE_SCALE + 45)
#define WSW_DECREASE_X		(70 * (SWTARGET_MOUSE_SCALE*0.6))
#define WSW_DECREASE_Y		-(70 * (SWTARGET_MOUSE_SCALE*0.6))
#define WSW_INCREASE_X		(135 * (SWTARGET_MOUSE_SCALE*0.6))
#define WSW_INCREASE_Y		-(70 * (SWTARGET_MOUSE_SCALE*0.6))
#define SW_TRANSPARENCY					150
#endif
void CINFWeaponInfo::RenderLeftWeapon(float x, float y)		// 2011. 10. 10 by jskim UI시스템 변경
#else						 
void CINFWeaponInfo::RenderLeftWeapon()
#endif
{
	int decreaseWSWX = 0;
#ifdef _WARRIOR_ADDITIONAL_INFO
	if ((float)g_pD3dApp->GetBackBufferDesc().Width < 2000 && (float)g_pD3dApp->GetBackBufferDesc().Width > 1700)
	{
		decreaseWSWX = (65 - 15 * (SWTARGET_MOUSE_SCALE*0.6));
	}
	if ((float)g_pD3dApp->GetBackBufferDesc().Width < 1500 && (float)g_pD3dApp->GetBackBufferDesc().Width > 1300)
	{
		decreaseWSWX = (75 - 15 * (SWTARGET_MOUSE_SCALE*0.6));
	}
	else if ((float)g_pD3dApp->GetBackBufferDesc().Width < 1300 && (float)g_pD3dApp->GetBackBufferDesc().Width > 1200)
	{
		decreaseWSWX = (85 - 15 * (SWTARGET_MOUSE_SCALE*0.6));
	}
	else if ((float)g_pD3dApp->GetBackBufferDesc().Width < 1200)
	{
		decreaseWSWX = (110 - 15 * (SWTARGET_MOUSE_SCALE*0.6));
	}
	else
	{
		decreaseWSWX = (70 - 15 * (SWTARGET_MOUSE_SCALE*0.6));
	}
#endif

	FLOG( "CINFWeaponInfo::RenderLeftWeapon()" );
 	int nWindowPosY = g_pGameMain->m_nLeftWindowY;

	// 상점에서는 정보를 보이지 않는다
	if(g_pD3dApp->m_dwGameState == _SHOP)
		return;
	// 2007-03-27 by bhsohn 옵저버 모드시 인터페이스 숨김
	if(g_pShuttleChild->IsObserverMode())
	{
		return;
	}
	// end 2007-03-27 by bhsohn 옵저버 모드시 인터페이스 숨김

//	if(m_pStImage )
//	{
//		m_pStImage->Move(m_nLeftWeaponInfoPosX, nWindowPosY);
//		m_pStImage->Render() ;
//	}
//	if(m_pStOverHeatBar)
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	if(g_pShuttleChild->m_pPrimaryWeapon)
	{
		m_pStOverHeatBar->Move(FIRST_WEAPON_START_X - 7, FIRST_WEAPON_START_Y - 20);
		m_pStOverHeatBar->SetScale(1.0f, 1.0f);
		m_pStOverHeatBar->SetRect(0, 0, m_pStOverHeatBar->GetImgSize().x * g_pShuttleChild->m_pPrimaryWeapon->GetOverHeatRate(), OVERHEAT_HEIGHT);
		m_pStOverHeatBar->SetTransparency(250);
		m_pStOverHeatBar->Render();
#ifdef _WARRIOR_ADDITIONAL_INFO
		if (g_pSOption->sWSWAddAIMInfo)
		{
			if (g_pGameMain->ShowOnScreenDisplay())
			{
				m_pStOverHeatBar->Move(SWFIRE_ANGLE_START_X - decreaseWSWX, SWFIRE_ANGLE_START_Y - WSW_DECREASE_Y + 20);
				m_pStOverHeatBar->SetScale(0.7f, 1.0f);
				m_pStOverHeatBar->SetRect(0, 0, m_pStOverHeatBar->GetImgSize().x * g_pShuttleChild->m_pPrimaryWeapon->GetOverHeatRate(), OVERHEAT_HEIGHT);
				m_pStOverHeatBar->SetTransparency(SW_TRANSPARENCY);
				m_pStOverHeatBar->Render();
			}
		}
#endif
	}
	if(g_pShuttleChild && g_pShuttleChild->m_pPrimaryWeapon)
	{
//		if(g_pD3dApp->m_pShuttleChild && g_pD3dApp->m_pShuttleChild->m_pItemPrimary )
		if(g_pShuttleChild && g_pShuttleChild->m_pPrimaryWeapon )
		{
			if(IS_PRIMARY_WEAPON_1(g_pShuttleChild->m_pPrimaryWeapon->GetRealItemInfo()->Kind))
			{
				if(LEFT_WEAPON_NUMBER == 0)
				{
#ifdef _WARRIOR_ADDITIONAL_INFO
					if (g_pSOption->sWSWAddAIMInfo)
					{
						if (g_pGameMain->ShowOnScreenDisplay())
						{
							RenderWarningWeaponNumber(SWFIRE_ANGLE_START_X - decreaseWSWX, SWFIRE_ANGLE_START_Y - WSW_DECREASE_Y, LEFT_WEAPON_NUMBER, 1);
						}
					}
#endif
					m_pBulletEmptyImage->Move( FIRST_WEAPON_START_X - 37, FIRST_WEAPON_START_Y + 5 - m_pBulletEmptyImage->GetImgSize().y );
					m_pBulletEmptyImage->Render();
					m_pBulletEmptyImage1->Move( FIRST_WEAPON_START_X - 34, FIRST_WEAPON_START_Y + 5  - m_pBulletEmptyImage->GetImgSize().y );
					m_pBulletEmptyImage1->Render();
#ifdef _WARRIOR_ADDITIONAL_INFO
					RenderWarningWeaponNumber(FIRST_WEAPON_FONT_X, FIRST_WEAPON_FONT_Y, LEFT_WEAPON_NUMBER, 0);
#else
					RenderWarningWeaponNumber(FIRST_WEAPON_FONT_X, FIRST_WEAPON_FONT_Y, LEFT_WEAPON_NUMBER);
#endif
				}
				else if( g_pShuttleChild->m_pPrimaryWeapon->IsOverHeat()) 
				{
#ifdef _WARRIOR_ADDITIONAL_INFO
					if (g_pSOption->sWSWAddAIMInfo)
					{
						if (g_pGameMain->ShowOnScreenDisplay())
						{
							if (m_bFlash)
								RenderWarningWeaponNumber(SWFIRE_ANGLE_START_X - decreaseWSWX, SWFIRE_ANGLE_START_Y - WSW_DECREASE_Y, LEFT_WEAPON_NUMBER, 1);
						}
					}
#endif
					m_pOverHeatImage->Move( FIRST_WEAPON_START_X - 37, FIRST_WEAPON_START_Y + 5 - m_pBulletEmptyImage->GetImgSize().y );
					m_pOverHeatImage->Render();
					if( m_bFlash )
#ifdef _WARRIOR_ADDITIONAL_INFO
						RenderWarningWeaponNumber(FIRST_WEAPON_FONT_X, FIRST_WEAPON_FONT_Y, LEFT_WEAPON_NUMBER, 0);
#else
						RenderWarningWeaponNumber(FIRST_WEAPON_FONT_X, FIRST_WEAPON_FONT_Y, LEFT_WEAPON_NUMBER);
#endif
				}
				else if(LEFT_WEAPON_NUMBER < 100 && m_bFlash)//과열상태인 경우
				{
#ifdef _WARRIOR_ADDITIONAL_INFO
					if (g_pSOption->sWSWAddAIMInfo)
					{
						if (g_pGameMain->ShowOnScreenDisplay())
						{
							RenderWarningWeaponNumber(SWFIRE_ANGLE_START_X - decreaseWSWX, SWFIRE_ANGLE_START_Y - WSW_DECREASE_Y, LEFT_WEAPON_NUMBER, 1);
						}
					}
#endif
					m_pBulletLowImage->Move( FIRST_WEAPON_START_X - 37, FIRST_WEAPON_START_Y + 5 - m_pBulletEmptyImage->GetImgSize().y  );
					m_pBulletLowImage->Render();					
#ifdef _WARRIOR_ADDITIONAL_INFO
					RenderWarningWeaponNumber(FIRST_WEAPON_FONT_X, FIRST_WEAPON_FONT_Y, LEFT_WEAPON_NUMBER, 0);
#else
					RenderWarningWeaponNumber(FIRST_WEAPON_FONT_X, FIRST_WEAPON_FONT_Y, LEFT_WEAPON_NUMBER);
#endif
				}
				else
				{
#ifdef _WARRIOR_ADDITIONAL_INFO
					if (g_pSOption->sWSWAddAIMInfo)
					{
						if (g_pGameMain->ShowOnScreenDisplay())
						{

							RenderWeaponNumber(SWFIRE_ANGLE_START_X - decreaseWSWX, SWFIRE_ANGLE_START_Y - WSW_DECREASE_Y, LEFT_WEAPON_NUMBER, 1);
						}
					}
#endif
					// 2006-07-19 by dgwoo m_nLeftWeaponInfoPosX추가 애니메이션에 따라 글씨도 찍히는 위치를 변경해준다.
#ifdef _WARRIOR_ADDITIONAL_INFO
					RenderWeaponNumber(FIRST_WEAPON_FONT_X, FIRST_WEAPON_FONT_Y, LEFT_WEAPON_NUMBER, 0);
#else
					RenderWeaponNumber(FIRST_WEAPON_FONT_X, FIRST_WEAPON_FONT_Y, LEFT_WEAPON_NUMBER);
#endif
				}
			}
			else if(IS_PRIMARY_WEAPON_2(g_pShuttleChild->m_pPrimaryWeapon->GetRealItemInfo()->Kind))// 연료 사용중
			{
				float fRate = g_pShuttleChild->m_myShuttleInfo.CurrentEP/(float)g_pShuttleChild->m_myShuttleInfo.EP;
				RenderWeaponFuel( LEFT_FUEL_X, nWindowPosY, fRate );
				if(fRate == 0)
				{
					m_pBulletEmptyImage->Move(m_nLeftWeaponInfoPosX+3, nWindowPosY + 10);
					m_pBulletEmptyImage->Render();
				}
				else if( fRate < 0.1f)
				{
					m_pBulletLowImage->Move(m_nLeftWeaponInfoPosX+3, nWindowPosY + 10);
					m_pBulletLowImage->Render();
				}
// 2008-10-20 by dgwoo 무기 이미지가 필요없음.
//				else if(g_pShuttleChild->m_pPrimaryWeapon->IsOverHeat() == FALSE)
//				{
//					m_pStWeaponImage->Move(m_nLeftWeaponInfoPosX+3, nWindowPosY + 10);
//					m_pStWeaponImage->Render() ;
//				}
				else//과열상태인 경우
				{
					m_pOverHeatImage->Move(m_nLeftWeaponInfoPosX+3, nWindowPosY + 10);
					m_pOverHeatImage->Render() ;
				}
			}
		}		
	}
	else
	{
		m_pBulletEmptyImage->Move( FIRST_WEAPON_START_X - 37, FIRST_WEAPON_START_Y + 5 - m_pBulletEmptyImage->GetImgSize().y );
		m_pBulletEmptyImage->Render();

//		m_pEmptyWeaponImage->Move(m_nLeftWeaponInfoPosX+3, nWindowPosY + 10);
//		m_pEmptyWeaponImage->Render() ;
	}
#else
	if(g_pShuttleChild->m_pPrimaryWeapon)
	{
// 2008-05-29 by dgwoo 메인 인터페이스 변경 기획.
//		m_pStOverHeatBar->Move(m_nLeftWeaponInfoPosX+LEFT_OVERHEAT_START_X, nWindowPosY+OVERHEAT_START_Y);
////		m_pStOverHeatBar->SetRect(0, 0, OVERHEAT_LENGTH*g_pD3dApp->m_pShuttleChild->m_fPrimaryWeaponActionRate, OVERHEAT_HEIGHT);
//		m_pStOverHeatBar->SetRect(0, 0, OVERHEAT_LENGTH*g_pShuttleChild->m_pPrimaryWeapon->GetOverHeatRate(), OVERHEAT_HEIGHT);
//		m_pStOverHeatBar->Render();
		m_pStOverHeatBar->Move(FIRST_WEAPON_GAGE_X, FIRST_WEAPON_GAGE_Y);
//		m_pStOverHeatBar->SetRect(0, 0, OVERHEAT_LENGTH*g_pD3dApp->m_pShuttleChild->m_fPrimaryWeaponActionRate, OVERHEAT_HEIGHT);
		m_pStOverHeatBar->SetRect(0, 0, OVERHEAT_LENGTH*g_pShuttleChild->m_pPrimaryWeapon->GetOverHeatRate(), OVERHEAT_HEIGHT);
		m_pStOverHeatBar->Render();
	}
	// 2008-10-20 by dgwoo 무기 이미지가 필요없음.
	//if(m_pStWeaponImage )		// 무기를 가지고 있으면
	if(g_pShuttleChild && g_pShuttleChild->m_pPrimaryWeapon)
	{
		
//		if(g_pD3dApp->m_pShuttleChild && g_pD3dApp->m_pShuttleChild->m_pItemPrimary )
		if(g_pShuttleChild && g_pShuttleChild->m_pPrimaryWeapon )
		{
			if(IS_PRIMARY_WEAPON_1(g_pShuttleChild->m_pPrimaryWeapon->GetRealItemInfo()->Kind))
			{
				if(LEFT_WEAPON_NUMBER == 0)
				{
					m_pBulletEmptyImage->Move(FIRST_WEAPON_START_X, FIRST_WEAPON_START_Y);
					m_pBulletEmptyImage->Render();
				}
				else if( g_pShuttleChild->m_pPrimaryWeapon->IsOverHeat())
				{
					m_pOverHeatImage->Move(FIRST_WEAPON_START_X, FIRST_WEAPON_START_Y);
					m_pOverHeatImage->Render();
				}
				else if(LEFT_WEAPON_NUMBER < 100 && m_bFlash)//과열상태인 경우
				{
					m_pBulletLowImage->Move(FIRST_WEAPON_START_X, FIRST_WEAPON_START_Y);
					m_pBulletLowImage->Render();
				}
				else
				{
					// 2006-07-19 by dgwoo m_nLeftWeaponInfoPosX추가 애니메이션에 따라 글씨도 찍히는 위치를 변경해준다.
					RenderWeaponNumber(FIRST_WEAPON_FONT_Y, FIRST_WEAPON_FONT_X, LEFT_WEAPON_NUMBER);
				}
			}
			else if(IS_PRIMARY_WEAPON_2(g_pShuttleChild->m_pPrimaryWeapon->GetRealItemInfo()->Kind))// 연료 사용중
			{
				float fRate = g_pShuttleChild->m_myShuttleInfo.CurrentEP/(float)g_pShuttleChild->m_myShuttleInfo.EP;
				RenderWeaponFuel( nWindowPosY, LEFT_FUEL_X, fRate );
				if(fRate == 0)
				{
					m_pBulletEmptyImage->Move(m_nLeftWeaponInfoPosX+3, nWindowPosY + 10);
					m_pBulletEmptyImage->Render();
				}
				else if( fRate < 0.1f)
				{
					m_pBulletLowImage->Move(m_nLeftWeaponInfoPosX+3, nWindowPosY + 10);
					m_pBulletLowImage->Render();
				}
// 2008-10-20 by dgwoo 무기 이미지가 필요없음.
//				else if(g_pShuttleChild->m_pPrimaryWeapon->IsOverHeat() == FALSE)
//				{
//					m_pStWeaponImage->Move(m_nLeftWeaponInfoPosX+3, nWindowPosY + 10);
//					m_pStWeaponImage->Render() ;
//				}
				else//과열상태인 경우
				{
					m_pOverHeatImage->Move(m_nLeftWeaponInfoPosX+3, nWindowPosY + 10);
					m_pOverHeatImage->Render() ;
				}
			}
		}
		
	}
	else
	{
		m_pBulletEmptyImage->Move(FIRST_WEAPON_START_X, FIRST_WEAPON_START_Y);
		m_pBulletEmptyImage->Render();

//		m_pEmptyWeaponImage->Move(m_nLeftWeaponInfoPosX+3, nWindowPosY + 10);
//		m_pEmptyWeaponImage->Render() ;
	}
#endif
}

#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
void CINFWeaponInfo::RenderRightWeapon(float x, float y)			// 2011. 10. 10 by jskim UI시스템 변경
#else 
void CINFWeaponInfo::RenderRightWeapon()
#endif
{
	FLOG("CINFWeaponInfo::RenderRightWeapon()");
	int nWindowPosY = g_pGameMain->m_nRightWindowY;

	// 상점에서는 정보를 보이지 않는다
	if (g_pD3dApp->m_dwGameState == _SHOP)
		return;
	// 2007-03-27 by bhsohn 옵저버 모드시 인터페이스 숨김
	if (g_pShuttleChild->IsObserverMode())
	{
		return;
	}
	// end 2007-03-27 by bhsohn 옵저버 모드시 인터페이스 숨김

	//	if(m_pNdImage )
	//	{
	//		m_pNdImage->Move(m_nRightWeaponInfoPosX, nWindowPosY);
	//		m_pNdImage->Render() ;
	//	}
	//	if(m_pNdOverHeatBar)
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	if (g_pShuttleChild->m_pSecondaryWeapon)
	{
		//		int startX = (1.0f-g_pD3dApp->m_pShuttleChild->m_fSecondaryWeaponActionRate)*OVERHEAT_LENGTH;
		int startX = (g_pShuttleChild->m_pSecondaryWeapon->GetReattackTimeRate())*OVERHEAT_LENGTH;
		if (g_pD3dApp->m_bCharacter)
			startX = OVERHEAT_LENGTH;
		m_pStOverHeatBar->Move(SECOND_WEAPON_START_X - 13, SECOND_WEAPON_START_Y - 20);
		m_pStOverHeatBar->SetScale(1.0f, 1.0f);
		//m_pStOverHeatBar->SetRect(startX, 0, OVERHEAT_LENGTH, OVERHEAT_HEIGHT);
		m_pStOverHeatBar->SetRect(0, 0, startX, OVERHEAT_HEIGHT);
		m_pStOverHeatBar->SetTransparency(250);
		m_pStOverHeatBar->Render();
#ifdef _WARRIOR_ADDITIONAL_INFO
		if (g_pSOption->sWSWAddAIMInfo)
		{
			if (g_pGameMain->ShowOnScreenDisplay())
			{
				m_pStOverHeatBar->Move(SWFIRE_ANGLE_START_X + WSW_INCREASE_X, SWFIRE_ANGLE_START_Y - WSW_INCREASE_Y + 20);
				m_pStOverHeatBar->SetScale(0.7f, 1.0f);
				m_pStOverHeatBar->SetRect(0, 0, startX, OVERHEAT_HEIGHT);
				m_pStOverHeatBar->SetTransparency(SW_TRANSPARENCY);
				m_pStOverHeatBar->Render();
			}
		}
#endif

	}
	// 2008-10-20 by dgwoo 무기 이미지가 필요없음.
	//	if(m_pNdWeaponImage )		// 무기를 가지고 있으면
	if (g_pShuttleChild->m_pSecondaryWeapon)
	{
		if (RIGHT_WEAPON_NUMBER == 0)
		{
#ifdef _WARRIOR_ADDITIONAL_INFO
			if (g_pSOption->sWSWAddAIMInfo)
			{
				if (g_pGameMain->ShowOnScreenDisplay())
				{
					RenderWarningWeaponNumber(SWFIRE_ANGLE_START_X + WSW_INCREASE_X, SWFIRE_ANGLE_START_Y - WSW_INCREASE_Y, RIGHT_WEAPON_NUMBER, 1);
				}
			}
#endif
			m_pBulletLowImage1->Move( SECOND_WEAPON_START_X - 43, SECOND_WEAPON_START_Y + 5 - m_pBulletEmptyImage->GetImgSize().y );
			m_pBulletLowImage1->Render();
			m_pBulletEmptyImage2->Move(SECOND_WEAPON_START_X - 43, SECOND_WEAPON_START_Y + 5 - m_pBulletEmptyImage->GetImgSize().y);
			m_pBulletEmptyImage2->Render();
#ifdef _WARRIOR_ADDITIONAL_INFO
			RenderWarningWeaponNumber(SECOND_WEAPON_FONT_X, SECOND_WEAPON_FONT_Y, RIGHT_WEAPON_NUMBER, 0);
#else
			RenderWarningWeaponNumber(SECOND_WEAPON_FONT_X, SECOND_WEAPON_FONT_Y, RIGHT_WEAPON_NUMBER);
#endif
		}
		else if (RIGHT_WEAPON_NUMBER < 10 && m_bFlash)
		{
#ifdef _WARRIOR_ADDITIONAL_INFO
			if (g_pSOption->sWSWAddAIMInfo)
			{
				if (g_pGameMain->ShowOnScreenDisplay())
				{
					RenderWarningWeaponNumber(SWFIRE_ANGLE_START_X + WSW_INCREASE_X, SWFIRE_ANGLE_START_Y - WSW_INCREASE_Y, RIGHT_WEAPON_NUMBER, 1);
				}
			}
#endif
			m_pBulletLowImage1->Move(SECOND_WEAPON_START_X - 43, SECOND_WEAPON_START_Y + 5 - m_pBulletEmptyImage->GetImgSize().y);
			m_pBulletLowImage1->Render();
#ifdef _WARRIOR_ADDITIONAL_INFO
			RenderWarningWeaponNumber(SECOND_WEAPON_FONT_X, SECOND_WEAPON_FONT_Y, RIGHT_WEAPON_NUMBER, 0);
#else
			RenderWarningWeaponNumber(SECOND_WEAPON_FONT_X, SECOND_WEAPON_FONT_Y, RIGHT_WEAPON_NUMBER);
#endif
			}
		else //if(g_pD3dApp->m_pShuttleChild->m_bSecondaryWeaponAction)
		{
			if (RIGHT_WEAPON_NUMBER >= 0)
			{
#ifdef _WARRIOR_ADDITIONAL_INFO
				if (g_pSOption->sWSWAddAIMInfo)
				{
					if (g_pGameMain->ShowOnScreenDisplay())
					{
						RenderWeaponNumber(SWFIRE_ANGLE_START_X + WSW_INCREASE_X, SWFIRE_ANGLE_START_Y - WSW_INCREASE_Y, RIGHT_WEAPON_NUMBER, 1);
				}
			}
				RenderWeaponNumber(SECOND_WEAPON_FONT_X, SECOND_WEAPON_FONT_Y, RIGHT_WEAPON_NUMBER, 0);
#else
				RenderWeaponNumber(SECOND_WEAPON_FONT_X, SECOND_WEAPON_FONT_Y, RIGHT_WEAPON_NUMBER);
#endif
		}
		}
	}
	else
	{
		m_pBulletEmptyImage2->Move( SECOND_WEAPON_START_X - 43, SECOND_WEAPON_START_Y + 5 - m_pBulletEmptyImage->GetImgSize().y);
		m_pBulletEmptyImage2->Render();
	}
#else				
	if(g_pShuttleChild->m_pSecondaryWeapon)
	{
		//		int startX = (1.0f-g_pD3dApp->m_pShuttleChild->m_fSecondaryWeaponActionRate)*OVERHEAT_LENGTH;
		int startX = (g_pShuttleChild->m_pSecondaryWeapon->GetReattackTimeRate())*OVERHEAT_LENGTH;
		if(g_pD3dApp->m_bCharacter)
			startX = OVERHEAT_LENGTH;
		m_pStOverHeatBar->Move(SECOND_WEAPON_GAGE_X, SECOND_WEAPON_GAGE_Y);
		//m_pStOverHeatBar->SetRect(startX, 0, OVERHEAT_LENGTH, OVERHEAT_HEIGHT);
		m_pStOverHeatBar->SetRect(0, 0, startX, OVERHEAT_HEIGHT);
		m_pStOverHeatBar->Render();
	}
	// 2008-10-20 by dgwoo 무기 이미지가 필요없음.
	//	if(m_pNdWeaponImage )		// 무기를 가지고 있으면
	if(g_pShuttleChild->m_pSecondaryWeapon)
	{

		if(RIGHT_WEAPON_NUMBER == 0)
		{
			m_pBulletEmptyImage->Move(SECOND_WEAPON_START_X, SECOND_WEAPON_START_Y);
			m_pBulletEmptyImage->Render();
		}
		else if( RIGHT_WEAPON_NUMBER < 10 && m_bFlash)
		{
			m_pBulletLowImage->Move(SECOND_WEAPON_START_X, SECOND_WEAPON_START_Y);
			m_pBulletLowImage->Render();
		}
		else //if(g_pD3dApp->m_pShuttleChild->m_bSecondaryWeaponAction)
		{
			if(RIGHT_WEAPON_NUMBER >= 0)
			{
				RenderWeaponNumber( SECOND_WEAPON_FONT_Y, SECOND_WEAPON_FONT_X, RIGHT_WEAPON_NUMBER);
			}
		}
	}
	else
	{
		m_pBulletEmptyImage->Move(SECOND_WEAPON_START_X, SECOND_WEAPON_START_Y);
		m_pBulletEmptyImage->Render();
	}

#endif
}

#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
void CINFWeaponInfo::RenderWeaponFuel( int nWindowPosX, int nWindowPosY, float fRate)			// 2011. 10. 10 by jskim UI시스템 변경
#else 
void CINFWeaponInfo::RenderWeaponFuel( int nWindowPosY, int nWindowPosX, float fRate)

#endif
{

	FLOG( "CINFWeaponInfo::RenderWeaponFuel( int nWindowPosX, int nWindowPosY,  float fRate)" );
	if(fRate>0)
	{
		m_pStFuelUnderGageBar->Move(nWindowPosX, nWindowPosY+LEFT_FUEL_Y);
		m_pStFuelUnderGageBar->Render();
		m_pStFuelGageBar->Move(nWindowPosX, nWindowPosY+LEFT_FUEL_Y);
		m_pStFuelGageBar->SetRect(0, 0, FUEL_GAGE_LENGTH*fRate, FUEL_GAGE_HEIGHT);
		m_pStFuelGageBar->Render();
	}
	else
	{
		m_pReloadImage->Move(nWindowPosX, nWindowPosY+LEFT_WEAPON_NUMBER_Y);
		m_pReloadImage->Render();
	}
}
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
#ifdef _WARRIOR_ADDITIONAL_INFO
void CINFWeaponInfo::RenderWeaponNumber(int nWindowPosX, int nWindowPosY, int nValue, int isTransparent)
#else
void CINFWeaponInfo::RenderWeaponNumber( int nWindowPosX, int nWindowPosY,  int nValue)			// 2011. 10. 10 by jskim UI시스템 변경
#endif
#else		  
void CINFWeaponInfo::RenderWeaponNumber( int nWindowPosY, int nWindowPosX, int nValue)
#endif
{
	

#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	FLOG( "CINFWeaponInfo::RenderWeaponNumber( int nWindowPosX, int nWindowPosY, int nValue)" );  
#else								
	FLOG( "CINFWeaponInfo::RenderWeaponNumber( int nWindowPosY, int nWindowPosX, int nValue)" );
#endif
	if(nValue > 0)
	{
//		for(int i=0;i<5;i++) // 5 : 5자리숫자
//		{
//			int nNum = nValue%10;
//			m_pWeaponNumberImage[nNum]->Move(nWindowPosX+WEAPON_NUMBER_SIZE*(4-i), nWindowPosY+LEFT_WEAPON_NUMBER_Y);
//			m_pWeaponNumberImage[nNum]->Render();
//			nValue /= 10;
//		}
		for(int i=0;i<5;i++) // 5 : 5자리숫자
		{
			int nNum = nValue % 10;

#ifdef _WARRIOR_ADDITIONAL_INFO
			if (isTransparent == 1)
				m_pWeaponNumberImage[nNum]->SetTransparency(SW_TRANSPARENCY);
			else
				m_pWeaponNumberImage[nNum]->SetTransparency(250);
#endif
			
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
			m_pWeaponNumberImage[nNum]->Move(nWindowPosX + ( m_pWeaponNumberImage[nNum]->GetImgSize().x - 3 ) *(4-i), nWindowPosY);
#else					 
			m_pWeaponNumberImage[nNum]->Move(nWindowPosX+WEAPON_NUMBER_SIZE*(4-i), nWindowPosY);
#endif
			m_pWeaponNumberImage[nNum]->Render();
			nValue /= 10;
		}
	}
	else
	{
//		m_pReloadImage->Move(nWindowPosX, nWindowPosY+LEFT_WEAPON_NUMBER_Y);
//		m_pReloadImage->Render() ;
	}
}
 // 2011. 10. 10 by jskim UI시스템 변경
#ifdef C_EPSODE4_UI_CHANGE_JSKIM	
#ifdef _WARRIOR_ADDITIONAL_INFO
void CINFWeaponInfo::RenderWarningWeaponNumber(int nWindowPosX, int nWindowPosY, int nValue, int isTransparent)
#else
void CINFWeaponInfo::RenderWarningWeaponNumber( int nWindowPosX, int nWindowPosY, int nValue)			// 2011. 10. 10 by jskim UI시스템 변경
#endif
{
	FLOG( "CINFWeaponInfo::RenderWarningWeaponNumber( int nWindowPosX, int nWindowPosY, int nValue)" );
	for(int i=0;i<5;i++) // 5 : 5자리숫자
	{
		int nNum = nValue%10;
#ifdef _WARRIOR_ADDITIONAL_INFO
		if (isTransparent == 1)
			m_pWeaponWarningNumberImage[nNum]->SetTransparency(SW_TRANSPARENCY);
		else
			m_pWeaponWarningNumberImage[nNum]->SetTransparency(150);
#endif
		m_pWeaponWarningNumberImage[nNum]->Move(nWindowPosX + ( m_pWeaponNumberImage[nNum]->GetImgSize().x - 3 ) *(4-i), nWindowPosY);
		m_pWeaponWarningNumberImage[nNum]->Render();
		nValue /= 10;
	}
}
// end 2011. 10. 10 by jskim UI시스템 변경
#endif

int CINFWeaponInfo::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	FLOG( "CINFWeaponInfo::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)" );
	return INF_MSGPROC_NORMAL;
}
