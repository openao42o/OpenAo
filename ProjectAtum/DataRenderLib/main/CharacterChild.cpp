// CharacterChild.cpp: implementation of the CCharacterChild class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CharacterChild.h"
#include "ShuttleChild.h"
#include "AtumApplication.h"
#include "AtumSound.h"
#include "EnemyData.h"
#include "ChatMoveData.h"
#include "AtumApplication.h"
#include "FieldWinSocket.h"
#include "Chat.h"
#include "Cinema.h"								// 2005-07-25 by ispark
#include "Background.h"
#include "SceneData.h"
#include "Camera.h"
#include "Weapon.h"
#include "INFGameMain.h"						// 2005-07-25 by ispark
#include "Skill.h"								// 2005-08-02 by ispark
#include "AtumDatabase.h"
#include "WeaponShieldData.h"
#include "CharacterRender.h"					// 2005-07-21 by ispark
#include "Frustum.h"
#include "ObjectChild.h"
#include "ClientParty.h"
#include "ObjRender.h"
#include "SkillEffect.h"
#include "dxutil.h"
#include "Interface.h"
#include "INFCityBase.h"
#include "INFTarget.h"
#include "INFMissionInfo.h"
#include "PkNormalTimer.h"						// 2005-08-02 by ispark
#include "INFCommunity.h"
#include "INFCommunityGuild.h"
#include "EffectRender.h"
#include "StoreData.h"	
#include "CustomOptimizer.h"

// 2010. 03. 03 by ckPark ÀÎÇÇ´ÏÆ¼ ÇÊµå 2Â÷(ÀÔÀå UIº¯°æ)
#include "INFCityInfinityField.h"
#include "INFGameArena.h"
// end 2010. 03. 03 by ckPark ÀÎÇÇ´ÏÆ¼ ÇÊµå 2Â÷(ÀÔÀå UIº¯°æ)

// 2011-07-22 by hsson ˜?Áßº¹ Á¢¼ÓÀ¸·Î ÀÎÃ¦Æ® µÇ´ø ¹ö±× ¼öÁ¤
#include "INFInvenExtend.h"
// end 2011-07-22 by hsson ˜?Áßº¹ Á¢¼ÓÀ¸·Î ÀÎÃ¦Æ® µÇ´ø ¹ö±× ¼öÁ¤


#include "PetManager.h"			// 2010-06-15 by shcho&hslee Æê½Ã½ºÅÛ - Æê µ¥ÀÌÅÍ¸¦ °¡Á®¿À´Â ÇÔ¼ö
#include "INFItemInfo.h"
#include "WeaponItemInfo.h"



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
#define CHARACTER_HEIGHT				7.087f

// 2009-02-05 by bhsohn Ä«¸Þ¶ó ±úÁö´Â Çö»ó Ã³¸®
// #define CHARACTER_CAMERA_DISTANCE		30.0f	// ÃÊ±â°Å¸®
// #define CHARACTER_CAMERA_HEIGHT			30.0f	// ÃÊ±â³ôÀÌ
// #define CHARACTER_CAMERA_WHEEL_DISTANCE 6.0f	// ÈÙ °£°Ý
// end 2009-02-05 by bhsohn Ä«¸Þ¶ó ±úÁö´Â Çö»ó Ã³¸®

#define CHARACTER_WALK					10.0f	// °È±â
#define CHARACTER_MAN_RUN				35.7f	// ³²ÀÚ ¶Ù±â 46->25->30->33		// 2006-11-02 by ispark, ´Ù¸¥°ÍÀ¸·Î ¹Ù²Þ
#define CHARACTER_WOMAN_RUN				33.0f	// ¿©ÀÚ ¶Ù±â 46->25->30->33		// 2006-11-02 by ispark, ´Ù¸¥°ÍÀ¸·Î ¹Ù²Þ

#define CHARACTER_SHOP_SERCH_DISTANCE	50.0f	// ¼¥°Ë»ö

// 2007-07-27 by bhsohn ÇÁ¸®½ºÄ« ¸Ê¸¸ »óÁ¡ ÀÎ½Ä ¹üÀ§ Áõ°¡
#define CHARACTER_SHOP_SERCH_FREESKA_DISTANCE	100.0f	// ¼¥°Ë»ö

#define CHARACTER_PICKING_LENGTH		10000.0f;// Picking Çã¿ë °Å¸®
//////////////////////////////////////////////////////////////////////////

CCharacterChild::CCharacterChild()
{
	D3DXMatrixIdentity(&m_mMatrix);
	D3DXMatrixIdentity(&m_mPickMatrix);

	g_pCharacterChild = this;
	m_pCharacterInfo = nullptr;
	m_pPickingInfo = nullptr;					// 2005-07-26 by ispark Picking ÀÌÆåÆ®
	m_pCharacterRender = g_pD3dApp->m_pCharacterRender;

	m_dwState = _INITIALIZING;
//	m_dwPartType = _SHUTTLE;
	m_bRButtonState = FALSE;
	m_bPickMove = FALSE;

	m_pVBShadow = nullptr;

	m_fCurrentTime = 0.0f;
	m_fCharacterSpeed = 0.0f;
	m_fCharacterAddSpeed = .0f;
	m_hyBodyCondition = 0;
	m_nUnitNum = 0;
	m_pSelectObject = nullptr;					// 2006-04-08 by ispark
	m_pShopObject = nullptr;
	m_bEnterShopState = FALSE;

	m_fOldLength = 0.0f;					// 2005-07-29 by ispark
	m_bMouseMove = FALSE;					// 2005-08-16 by ispark
	m_bMouseWheel = FALSE;					// 2005-08-17 by ispark

	m_bCharacterRender = TRUE;
	m_nStartEventType = 0;

	m_bStartPositionEvent = FALSE;
	m_pSelBazaarShop = nullptr;					// 2006-07-29 by ispark
	m_fCharacterWarpErrTime = 0.0f;				// 2006-08-07 by ispark

	m_pMoveChatShop = nullptr;

//	m_fCharacterMoveRate = 0.0f;			// 2006-11-02 by ispark

	// 2010-11-29 by jskim, Æê ÀåÂø½Ã ÀÌµ¿¼Óµµ Áõ°¡
	m_bSetSpeed = FALSE;
	m_bPetWearItem = FALSE;
	// end 2010-11-29 by jskim, Æê ÀåÂø½Ã ÀÌµ¿¼Óµµ Áõ°¡

	// 2012-06-25 by jhahn, Æê ÀåÂø½Ã ÀÌµ¿¼Óµµ Áõ°¡ ±â´ÉÃß°¡
	PetoptionSpeed	= NULL;
	//end 2012-06-25 by jhahn, Æê ÀåÂø½Ã ÀÌµ¿¼Óµµ Áõ°¡ ±â´ÉÃß°¡

	// 2013-06-10 by bhsohn ¼¼·ÂÆ÷ÀÎÆ® °³¼±¾È - ¹öÇÁ 'ÄÉ¸¯ÅÍÀÌµ¿¼Óµµ' °³¼±
	m_fAdminModeSpeed = 0.0f;
	// END 2013-06-10 by bhsohn ¼¼·ÂÆ÷ÀÎÆ® °³¼±¾È - ¹öÇÁ 'ÄÉ¸¯ÅÍÀÌµ¿¼Óµµ' °³¼±
}

CCharacterChild::~CCharacterChild()
{
	SAFE_RELEASE(m_pVBShadow);
	util::del(m_pMoveChatShop);
}

HRESULT CCharacterChild::InitDeviceObjects()
{
	SAFE_RELEASE(m_pVBShadow);
	if(m_pCharacterInfo)
	{
		m_pCharacterInfo->InitDeviceObjects();
		m_pCharacterInfo->DeleteDeviceObjects();
		util::del(m_pCharacterInfo);
	}
	if(m_pPickingInfo)
	{
		m_pPickingInfo->InvalidateDeviceObjects();
		m_pPickingInfo->DeleteDeviceObjects();
		util::del(m_pPickingInfo);
	}
	if(m_pMoveChatShop)
		m_pMoveChatShop->InitDeviceObjects();
	return S_OK;
}

HRESULT CCharacterChild::RestoreDeviceObjects()
{
	RestoreShadow();
	if(m_pMoveChatShop)
		m_pMoveChatShop->RestoreDeviceObjects();
	return S_OK;
}

HRESULT CCharacterChild::InvalidateDeviceObjects()
{
	SAFE_RELEASE(m_pVBShadow);
	if(m_pMoveChatShop)
		m_pMoveChatShop->InvalidateDeviceObjects();
	return S_OK;
}

HRESULT CCharacterChild::DeleteDeviceObjects()
{
	SAFE_RELEASE(m_pVBShadow);
//	if(m_pCharacterInfo)
//		m_pCharacterInfo->DeleteDeviceObjects();
//	if(m_pPickingInfo)
//		m_pPickingInfo->DeleteDeviceObjects();
	if(m_pCharacterInfo)
	{
		m_pCharacterInfo->InvalidateDeviceObjects();
		m_pCharacterInfo->DeleteDeviceObjects();
		util::del(m_pCharacterInfo);
	}
	if(m_pPickingInfo)
	{
		m_pPickingInfo->InvalidateDeviceObjects();
		m_pPickingInfo->DeleteDeviceObjects();
		util::del(m_pPickingInfo);
	}
	if(m_pMoveChatShop)
	{
		m_pMoveChatShop->DeleteDeviceObjects();
		util::del(m_pMoveChatShop);
	}
	return S_OK;
}
// 2010-06-08 by dgwoo, Æê½Ã½ºÅÛ Ãß°¡. 
void CCharacterChild::SetPartner(char* i_szPartner)
{
	//int nPartner = atoi(i_szPartner);
	//CreateWearItem( WEAR_ITEM_KIND_PET, nPartner, FALSE);
}
// 2010-06-08 by dgwoo, Æê½Ã½ºÅÛ Ãß°¡. 
void CCharacterChild::Render()
{
	FLOG( "CShuttleChild::Render()" );
//	if(g_pCamera->GetCamType() == CAMERA_TYPE_NORMAL ||
//		m_bIsCameraPattern == TRUE)
	// 2005-08-30 by ispark	
//	g_pShuttleChild->RenderMirror();

	if(m_bCharacterRender)
	{
		g_pD3dDev->SetRenderState( D3DRS_SPECULARENABLE, FALSE );

//		g_pD3dDev->LightEnable( 1, FALSE );
//		g_pD3dDev->LightEnable( 2, FALSE );
//		g_pD3dDev->LightEnable( 3, FALSE );
//		g_pD3dDev->SetRenderState( D3DRS_FOGENABLE,  IsFogEnableMap(g_pShuttleChild->m_myShuttleInfo.MapChannelIndex.MapIndex) );
		g_pD3dDev->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE);
		g_pD3dDev->SetRenderState( D3DRS_ZENABLE,  TRUE );
		g_pD3dDev->SetRenderState( D3DRS_LIGHTING, TRUE );
		
//		g_pD3dDev->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_ONE );
//		g_pD3dDev->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );
		g_pD3dDev->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
		g_pD3dDev->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
		g_pD3dDev->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
		g_pD3dDev->SetRenderState( D3DRS_ALPHATESTENABLE,  TRUE );
		g_pD3dDev->SetRenderState( D3DRS_ALPHAFUNC,  D3DCMP_GREATER);
		g_pD3dDev->SetRenderState( D3DRS_ALPHAREF,  0x00000088 );
	

		
//		g_pD3dDev->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE );
		
		m_pCharacterRender->Render();	
		g_pD3dDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);		
	}
}

void CCharacterChild::Tick(float fElapsedTime)
{
	//////////////////////////////////////////////////////////////////////////
	// CShuttleChild->Tick()Áß¿¡¼­ ²À ÇÊ¿äÇÑ ÇÔ¼öµé... 
	// Ä³¸¯ÅÍÀÏ¶§µµ µ¹¾Æ¾ß ÇÏ´Â ÇÔ¼öµé
	// 2005-08-02 by ispark

	// ÆÄÆ¼ Tick
	if(g_pShuttleChild->m_pClientParty->IsParty())
	{
		g_pShuttleChild->m_pClientParty->Tick(fElapsedTime);
	}
	
	g_pShuttleChild->m_pPkNormalTimer->Tick(fElapsedTime);

	g_pShuttleChild->UnitInfoBarSmoothRepair(fElapsedTime);
	g_pShuttleChild->UsedItemTick(fElapsedTime);
//	g_pShuttleChild->m_pSkill->Tick(fElapsedTime);			// 2005-08-08 by ispark
	// Å¬¶óÀÌ¾ðÆ® Å¸ÀÌ¸Ó °è»ê
	g_pShuttleChild->m_timeProcess.Tick(fElapsedTime);
	g_pShuttleChild->TickBurn(fElapsedTime);				// 2005-08-04 by ispark
	g_pShuttleChild->m_pSkill->Tick(fElapsedTime);			// 2005-12-01 by ispark

	//////////////////////////////////////////////////////////////////////////
	// ÇöÀç »óÁ¡ ÀÌ¿ëÁßÀÌ°¡? ÀÌ¿ëÁßÀÌ¸é Picking ¹«½Ã
	// 2006-06-08 by ispark, »óÁ¡ Ã¼Å© º¯°æ
	m_bEnterShopState = FALSE;
//	GUI_BUILDINGNPC* pTempBase = g_pInterface->m_pCityBase->GetCurrentBuildingNPC();
//	if(pTempBase)
//	{
//		if( m_pShopObject )
//		{
//			m_bPickMove = FALSE;
//			m_bEnterShopState = TRUE;
//			m_dwState = _STAND;
//			m_pCharacterInfo->ChangeBodyCondition(BODYCON_CHARACTER_MODE_STOP);
////			float fDistance = D3DXVec3Length(&(m_vPos - m_pShopObject->m_vPos));
////			if(fDistance>CHARACTER_SHOP_SERCH_DISTANCE)
////			{
////				m_bPickMove = TRUE;
////				g_pInterface->m_pCityBase->SendLeaveEnterBuilding( g_pInterface->m_pCityBase-> GetCurrentEnterBuildingIndex(), -1 );
////			}
//		}		
//	}
	// 2006-06-13 by ispark, Ä³¸¯ÅÍ »óÅÂ°¡ _NCITYIN ¾Æ´Ï¾î¾ß ÇÑ´Ù. Ä³¸¯ÅÍ°¡ Ã³À½¿¡ ÇÑ¹øÀº _NCITYIN Ã³¸®ÇØ¾ß ÇÑ´Ù.
	if(g_pD3dApp->m_dwGameState == _SHOP && m_dwState != _NCITYIN)
	{
		m_bPickMove = FALSE;
		m_bEnterShopState = TRUE;
		m_dwState = _STAND;
		m_pCharacterInfo->ChangeBodyCondition(BODYCON_CHARACTER_MODE_STOP);
		 // 2010-06-08 by dgwoo, Æê½Ã½ºÅÛ Ãß°¡. 
		//ChangeWearItemBodyConditionAllProcess(BODYCON_CHARACTER_MODE_STOP);
		 // 2010-06-08 by dgwoo, Æê½Ã½ºÅÛ Ãß°¡. 
// 		g_pShuttleChild->ChangeWearItemBodyCondition(WEAR_ITEM_KIND_ATTACHMENT, BODYCON_CHARACTER_MODE_STOP);
// 		g_pShuttleChild->ChangeWearItemBodyCondition(WEAR_ITEM_KIND_ACCESSORIES, BODYCON_CHARACTER_MODE_STOP);
// 		g_pShuttleChild->ChangeWearItemBodyCondition(WEAR_ITEM_KIND_WINGIN, BODYCON_CHARACTER_MODE_STOP);
  		// 2010-06-15 by shcho&hslee Æê½Ã½ºÅÛ - ¸¶À» ÀÌµ¿ Ã³¸®
		g_pShuttleChild->ChangeWearItemBodyCondition(WEAR_ITEM_KIND_ACCESSORY_UNLIMITED, BODYCON_CHARACTER_MODE_STOP);
		g_pShuttleChild->ChangeWearItemBodyCondition(WEAR_ITEM_KIND_ACCESSORY_TIME_LIMIT, BODYCON_CHARACTER_MODE_STOP);
		g_pShuttleChild->ChangeWearItemBodyCondition(WEAR_ITEM_KIND_WINGIN, BODYCON_CHARACTER_MODE_STOP);

	}
	// 2007-06-12 by bhsohn ¹Ì¼Ç ÀÎÅÍÆäÀÌ½º ¼öÁ¤¾È
	//g_pGameMain->m_pMissionInfo->SetEnterShop(m_bEnterShopState);
	g_pGameMain->SetMissionEnterShop(m_bEnterShopState);

	//////////////////////////////////////////////////////////////////////////
	// Ä³¸¯ÅÍ ÀÌº¥Æ® ¿ÀºêÁ§Æ®
	CheckEventTypeObject(fElapsedTime);

	//////////////////////////////////////////////////////////////////////////
	// Ä³¸¯ÅÍ ÀÌµ¿
	CheckState();
	Move_Character(fElapsedTime);

	CheckShadowPos();
	CheckAniTime(fElapsedTime);
	g_pInterface->m_pTarget->SetMouseType(MOUSE_TYPE_0);

	// ½ºÅ©¸°»óÀÇ ÁÂÇ¥¸¦ ±¸ÇÑ´Ù.
	// 2005-07-29 by ispark
	// Ä³¸¯ÅÍ Å° ³ôÀÌ·Î ¼öÁ¤
	D3DXVECTOR3 vPos = m_vPos;
	vPos.y += GetCharacterHeight(m_nUnitNum) + 1.0f;

	//////////////////////////////////////////////////////////////////////////
	// Ä³¸¯ÅÍ Ä«¸Þ¶ó ¼ÂÆÃ
	if(g_pShuttleChild->m_bIsCameraPattern != TRUE)
		CameraMoveTick();

//	DBGOUT("m_vPos %f,  %f,  %f\n",m_vPos.x,m_vPos.y,m_vPos.z);
	g_pD3dApp->CalcObjectSourceScreenCoords(vPos, g_pD3dApp->GetBackBufferDesc().Width, g_pD3dApp->GetBackBufferDesc().Height, 
		g_pShuttleChild->m_nObjScreenX, g_pShuttleChild->m_nObjScreenY, g_pShuttleChild->m_nObjScreenW);
	g_pShuttleChild->m_pMoveChat->Tick();

	// 2006-09-04 by ispark, °³ÀÎ»óÁ¡ ¿ÀÇÂ ¸»Ç³¼±
	if(m_pMoveChatShop)
	{
		m_pMoveChatShop->Tick();
	}
}

void CCharacterChild::CheckAniTime(float fElapsedTime)
{
	FLOG( "CShuttleChild::CheckAniTime()" );

	m_pCharacterInfo->Tick(fElapsedTime);
	m_pCharacterInfo->SetAllBodyConditionMatrix(m_mMatrix);
	m_fCurrentTime = m_pCharacterInfo->GetCurrentCharacterAnimationTime();

	m_pPickingInfo->Tick(fElapsedTime);
//	sprintf( g_pD3dApp->m_strDebug, "0x%016I64x %.3f",GetCurrentBodyCondition(), m_fCurrentTime );
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			CShuttleChild::Move_Character(float fElapsedTime)
/// \brief		Ä³¸¯ÅÍ ÀÌµ¿
/// \author		ispark
/// \date		2005-07-13 ~ 2005-07-13
/// \warning	
///
/// \param		
/// \return		void
///////////////////////////////////////////////////////////////////////////////
void CCharacterChild::Move_Character(float fElapsedTime)
{
	D3DXVec3Normalize(&m_vVel,&m_vVel);
	D3DXVec3Normalize(&m_vUp,&m_vUp);
	D3DXVec3Cross(&m_vSideVel,&m_vUp,&m_vVel);

	//////////////////////////////////////////////////////////////////////////
	// Ä³¸¯ÅÍ ÀÌµ¿
	// Picking¿¡ ÀÇÇÑ Ã³¸®
	D3DXVECTOR3 vPos = m_vPos;
	vPos.y += CHARACTER_HEIGHT;			// Ãæµ¹°Ë»ç·Î ÀÎÇÑ ³ôÀÌ °ªÀ» ¿Ã¸°´Ù.

	const auto& myShuttleInfo = g_pShuttleChild->GetMyShuttleInfo();
	
	// 2010-11-29 by jskim, Æê ÀåÂø½Ã ÀÌµ¿¼Óµµ Áõ°¡
	//if( COMPARE_RACE( myShuttleInfo.Race,RACE_OPERATION|RACE_GAMEMASTER) )
	// end 2010-11-29 by jskim, Æê ÀåÂø½Ã ÀÌµ¿¼Óµµ Áõ°¡
	//{	// °ü¸®ÀÚ ¸ðµåÀÇ ÀÌµ¿ ¼Óµµ º¯°æ Ã³¸®.
		float fTempSpeed = m_fCharacterAddSpeed;
		
		const float a_fMinAddSpeed = .0f;
		const float a_fMaxAddSpeed = 150.0f;
		
	// 2010-11-29 by jskim, Æê ÀåÂø½Ã ÀÌµ¿¼Óµµ Áõ°¡
	//if ( GetAsyncKeyState ( VK_ADD ) )				// Å°ÆÐµå '+'·Î ¼Óµµ Áõ°¡.
	//	m_fCharacterAddSpeed += 1.0f;
	//else if ( GetAsyncKeyState ( VK_SUBTRACT ) )	// Å°ÆÐµå '-'·Î ¼Óµµ °¨¼Ò.
	//	m_fCharacterAddSpeed -= 1.0f;
	//else if ( GetAsyncKeyState ( VK_DECIMAL ) )
	//	m_fCharacterAddSpeed += 50.0f;
	//else if ( GetAsyncKeyState ( VK_DIVIDE ) )		// Å°¹èµå '/'·Î ÃÊ±âÈ­.
	//	m_fCharacterAddSpeed = .0f;
		if (COMPARE_RACE(myShuttleInfo.Race, RACE_OPERATION | RACE_GAMEMASTER | RACE_MONITOR))
	{
		// 2013-06-10 by bhsohn ¼¼·ÂÆ÷ÀÎÆ® °³¼±¾È - ¹öÇÁ 'ÄÉ¸¯ÅÍÀÌµ¿¼Óµµ' °³¼±
		if ( GetAsyncKeyState ( VK_ADD ) )				// Å°ÆÐµå '+'·Î ¼Óµµ Áõ°¡.
			m_fAdminModeSpeed += 1.0f;
		else if ( GetAsyncKeyState ( VK_SUBTRACT ) )	// Å°ÆÐµå '-'·Î ¼Óµµ °¨¼Ò.
			m_fAdminModeSpeed -= 1.0f;
		else if ( GetAsyncKeyState ( VK_DECIMAL ) )
			m_fAdminModeSpeed += 50.0f;
		else if ( GetAsyncKeyState ( VK_DIVIDE ) )		// Å°¹èµå '/'·Î ÃÊ±âÈ­.
			m_fAdminModeSpeed = .0f;				
		// END 2013-06-10 by bhsohn ¼¼·ÂÆ÷ÀÎÆ® °³¼±¾È - ¹öÇÁ 'ÄÉ¸¯ÅÍÀÌµ¿¼Óµµ' °³¼±
	}
	// end end 2010-11-29 by jskim, Æê ÀåÂø½Ã ÀÌµ¿¼Óµµ Áõ°¡

	// 2013-06-10 by bhsohn ¼¼·ÂÆ÷ÀÎÆ® °³¼±¾È - ¹öÇÁ 'ÄÉ¸¯ÅÍÀÌµ¿¼Óµµ' °³¼±
	{
		float fcharSpeed = GetParamFactor_DesParam( g_pShuttleChild->m_paramFactor, DES_RARE_ITEM_PARTNER_SPEED );
		if(0 != fcharSpeed)
		{
			m_fCharacterAddSpeed = GetCharacterSteps( g_pShuttleChild->m_myShuttleInfo.PilotFace ) * fcharSpeed;			
		}
		else
		{
			m_fCharacterAddSpeed = 0.0f;			
		}
		// todo : here is walking speed
#ifdef ARESBETA
		m_fCharacterAddSpeed = GetCharacterSteps(g_pShuttleChild->m_myShuttleInfo.PilotFace) * 1.5f;
#endif

		if(m_fAdminModeSpeed > 0.0f)
		{
			m_fCharacterAddSpeed += m_fAdminModeSpeed;
		}
	}
	// END 2013-06-10 by bhsohn ¼¼·ÂÆ÷ÀÎÆ® °³¼±¾È - ¹öÇÁ 'ÄÉ¸¯ÅÍÀÌµ¿¼Óµµ' °³¼±
		
	m_fCharacterAddSpeed = max ( m_fCharacterAddSpeed , a_fMinAddSpeed );
	m_fCharacterAddSpeed = min ( m_fCharacterAddSpeed , a_fMaxAddSpeed );
				
	// 2013-06-10 by bhsohn ¼¼·ÂÆ÷ÀÎÆ® °³¼±¾È - ¹öÇÁ 'ÄÉ¸¯ÅÍÀÌµ¿¼Óµµ' °³¼±
	// 2010-11-29 by jskim, Æê ÀåÂø½Ã ÀÌµ¿¼Óµµ Áõ°¡
// 	if( m_bPetWearItem )
// 	{
// 		if( m_bSetSpeed == FALSE )
// 		{
// 		// 2010-12-21 by jskim, ¸¶À» ÀÌµ¿ ¼Óµµ Áõ°¡ ¾ÆÀÌÅÛ ±¸Çö
// 			//m_fCharacterAddSpeed += GetCharacterSteps( g_pShuttleChild->m_myShuttleInfo.PilotFace ) * 0.1f;
// 			float fcharSpeed = GetParamFactor_DesParam( g_pShuttleChild->m_paramFactor, DES_RARE_ITEM_PARTNER_SPEED );
// 			m_fCharacterAddSpeed += GetCharacterSteps( g_pShuttleChild->m_myShuttleInfo.PilotFace ) * fcharSpeed;			
// 
// 		// end 2010-12-21 by jskim, ¸¶À» ÀÌµ¿ ¼Óµµ Áõ°¡ ¾ÆÀÌÅÛ ±¸Çö
// 			m_bSetSpeed = TRUE;
// 		}
// 	}
// 	else
// 	{
// 		m_fCharacterAddSpeed = 0.0f;
// 		m_bPetWearItem = FALSE;
// 	}
	// end 2010-11-29 by jskim, Æê ÀåÂø½Ã ÀÌµ¿¼Óµµ Áõ°¡	
	// END 2013-06-10 by bhsohn ¼¼·ÂÆ÷ÀÎÆ® °³¼±¾È - ¹öÇÁ 'ÄÉ¸¯ÅÍÀÌµ¿¼Óµµ' °³¼±

	// 2013-06-10 by bhsohn ¼¼·ÂÆ÷ÀÎÆ® °³¼±¾È - ¹öÇÁ 'ÄÉ¸¯ÅÍÀÌµ¿¼Óµµ' °³¼±
// 2012-06-25 by jhahn, Æê ÀåÂø½Ã ÀÌµ¿¼Óµµ Áõ°¡ ±â´ÉÃß°¡
#ifdef S_OPTION_PARTNER__JHAHN		
// 	tPET_CURRENTINFO *pPetCurrentInfo = NULL;
// 	tPET_LEVEL_DATA *psPetLevelDataPrev = NULL;
// 	INVEN_DISPLAY_INFO *pInvenDisplayInfo = g_pGameMain->m_pInven->rtnPtr_AttachmentItemInfo ( POS_PET );
// 	BOOL runSpeed = FALSE;
// 	
// 			
// 		if ( pInvenDisplayInfo )
// 		{
// 			pPetCurrentInfo = g_pShuttleChild->GetPetManager()->GetPtr_PetCurrentData ( pInvenDisplayInfo->pItem->UniqueNumber );
// 			psPetLevelDataPrev = g_pDatabase->GetPtr_PetLevelData ( pPetCurrentInfo->PetIndex , pPetCurrentInfo->PetLevel );			
// 			CItemInfo* pAllItem = g_pStoreData->FindItemInInventoryByWindowPos( POS_PET ); 	
// 
// 			for(int i =0 ; i<6 ;i++)
// 			{
// 				CItemInfo* SoketItem = g_pStoreData->FindItemInInventoryByUniqueNumber(pPetCurrentInfo->PetSocketItemUID[i] ); 
// 				
//  				if(SoketItem == NULL)
// 					continue;
// 				
// 				else if( SoketItem->GetItemInfo() && SoketItem->GetItemInfo()->ArrDestParameter[0] == DES_PET_SOCKET_ITEM_SPEED )
//  				{
//  					runSpeed = TRUE;
// 					break;
//  				}	
// 				else
// 					runSpeed = FALSE;
// 			}
// 			if (runSpeed == FALSE)
// 			{
// 				PetoptionSpeed = NULL;
// 			}
// 			
// 			if ((pPetCurrentInfo->PetLevel != PetoptionSpeed) && runSpeed)
// 			{
// 				int TempLevel;
// 				if (pPetCurrentInfo->PetLevel / 5 <= 0)
// 				{
// 					TempLevel = 1 ;						
// 				}
// 				else 
// 				{
// 					TempLevel = pPetCurrentInfo->PetLevel;
// 				}
// 				m_fCharacterAddSpeed = (((GetCharacterSteps( g_pShuttleChild->m_myShuttleInfo.PilotFace ) * 0.1f ) + (pPetCurrentInfo->PetLevel/5 ) * 2.1f) );		
// 				PetoptionSpeed = pPetCurrentInfo->PetLevel;
// 			}
// 		
// 		}
// 		else
// 		{
// 
// 			PetoptionSpeed = NULL;
// 			runSpeed = FALSE;
// 		}
		// END 2013-06-10 by bhsohn ¼¼·ÂÆ÷ÀÎÆ® °³¼±¾È - ¹öÇÁ 'ÄÉ¸¯ÅÍÀÌµ¿¼Óµµ' °³¼±
#endif
//end 2012-06-25 by jhahn, Æê ÀåÂø½Ã ÀÌµ¿¼Óµµ Áõ°¡ ±â´ÉÃß°¡
	
	
	g_cCustomOptimizer.FastMove( m_fCharacterAddSpeed );

	if ( fTempSpeed != m_fCharacterAddSpeed )
	{
		// 2010-11-29 by jskim, Æê ÀåÂø½Ã ÀÌµ¿¼Óµµ Áõ°¡
// 		if ( g_pD3dApp->m_pChat )
// 		{
// 			char szTemp[256] = {0, };
// 			sprintf ( szTemp , "Change Character Add MoveSpeed : (%.2f)" , m_fCharacterAddSpeed );
// 			g_pD3dApp->m_pChat->CreateChatChild ( szTemp , COLOR_SYSTEM );
// 		}
		if (COMPARE_RACE(myShuttleInfo.Race, RACE_OPERATION | RACE_GAMEMASTER | RACE_MONITOR))
		{
			if ( g_pD3dApp->m_pChat )
			{
				char szTemp[256] = {0, };
				sprintf ( szTemp , "Change Character Add MoveSpeed : (%.2f)" , m_fCharacterAddSpeed );
				
				g_pD3dApp->m_pChat->CreateChatChild ( szTemp , COLOR_SYSTEM );
			}
		}
		// end 2010-11-29 by jskim, Æê ÀåÂø½Ã ÀÌµ¿¼Óµµ Áõ°¡
			CBodyConditionInfo *pcGetBodyCondition = m_pCharacterInfo->FindBodyConditionInfo ( BODYCON_CHARACTER_MODE_RUN );

			if ( pcGetBodyCondition )
			{

				float fSetFrame = 1.0f + ( 2.0f * ( m_fCharacterAddSpeed / a_fMaxAddSpeed ) );


				pcGetBodyCondition->SetFrameSpeed ( fSetFrame );
			}
	}

	if(m_dwState == _STAND)
	{
		m_fCharacterSpeed = 0.0f;
	}
	if(m_bPickMove)
	{
		D3DXMATRIX  matTemp;
		double dRadian = 0.0f;
		float dot = 0.0f;
		D3DXVECTOR3 vCrossVec;
		D3DXVECTOR3 vNewDir = m_vPickDir;

		D3DXVec3Cross(&vCrossVec, &m_vVel, &vNewDir);
		D3DXVec3Normalize(&vNewDir, &vNewDir);
		dot = D3DXVec3Dot(&m_vVel, &vNewDir);

		if(m_dwState != _RUN)
		{
			if(1.0f > dot)
			{
				if(0.0f > vCrossVec.y)
				{
					//¿À¸¥ÂÊ È¸Àü  
					dRadian -= ACOS(dot);
				}
				else	
				{
					// ¿ÞÂÊ È¸Àü
					dRadian += ACOS(dot);
				}

				D3DXMatrixRotationY(&matTemp, (float)dRadian);
				D3DXVec3TransformCoord(&m_vVel, &m_vVel, &matTemp);
			}
			// ÀÏ´Ü Å×½ºÆ®·Î ¶Ù´Â Çàµ¿À» ÁØ´Ù.
			// 2005-07-29 by ispark
			// Picking Çã¿ë °Å¸®¸¦ »õ·Î ¼ÂÆÃ
			m_fOldLength = CHARACTER_PICKING_LENGTH;
			m_dwState = _RUN;
			// 2005-07-29 by ispark
			// ¶Ù±â ¹Ùµð ÄÁµð¼ÇÀ» WndProc¿¡¼­ ¿©±â·Î ¿Å°å´Ù.
			m_pCharacterInfo->ChangeBodyCondition(BODYCON_CHARACTER_MODE_RUN);
			// 2010-06-08 by dgwoo, Æê½Ã½ºÅÛ Ãß°¡. ¾Æ·¡ÀÇ ÇÔ¼ö·Î ÅëÇÕ. 
			ChangeWearItemBodyConditionAllProcess(BODYCON_CHARACTER_MODE_RUN);
// 			g_pShuttleChild->ChangeWearItemBodyCondition(WEAR_ITEM_KIND_ATTACHMENT, BODYCON_CHARACTER_MODE_RUN);
// 			g_pShuttleChild->ChangeWearItemBodyCondition(WEAR_ITEM_KIND_ACCESSORIES, BODYCON_CHARACTER_MODE_RUN);
// 			g_pShuttleChild->ChangeWearItemBodyCondition(WEAR_ITEM_KIND_WINGIN, BODYCON_CHARACTER_MODE_RUN);
			// 2010-06-08 by dgwoo, Æê½Ã½ºÅÛ Ãß°¡.
		}
		// Æê °ü·Ã - Ä³¸¯ÅÍ ÀÌµ¿ (¸¶¿ì½º ¿ì Å¬¸¯)½Ã Tick() Áö¿¬µÇ´Â ¹®Á¦ ¼öÁ¤. 2010. 10. 08. by jskim.
		if(m_dwState == _RUN)
		{
			D3DXVECTOR3 vMovingVec;
			vMovingVec = m_vPicking - m_vPos;
			FLOAT lenght = D3DXVec3Length(&vMovingVec);

			// 2005-07-29 by ispark
			// º¸Á¤ °Å¸® ¼öÁ¤ 5.0f -> 10.0f
			if(10.0f >= lenght)
			{
				// ÇÑ¹ø´õ È¸Àü °Ë»çÈÄ °Å¸® 0.5¿¡¼­ Á¤Áö ½ÃÅ²´Ù.
				if(1.0f > dot && 0.5f < lenght)
				{
					if(0.0f > vCrossVec.y)
					{
						//¿À¸¥ÂÊ È¸Àü  
						dRadian -= ACOS(dot);
					}
					else	
					{
						// ¿ÞÂÊ È¸Àü
						dRadian += ACOS(dot);
					}

					D3DXMatrixRotationY(&matTemp, (float)dRadian);
					D3DXVec3TransformCoord(&m_vVel, &m_vVel, &matTemp);
				}
			}
			
			if((0.5f > lenght) || (m_fOldLength < lenght))
				{
//					g_pShuttleChild->SendFieldSocketChangeBodyCondition(g_pShuttleChild->m_myShuttleInfo.ClientIndex, BODYCON_CHRACTER_MODE_STOP);
					// 2010-06-08 by dgwoo, Æê½Ã½ºÅÛ Ãß°¡. 
					m_pCharacterInfo->ChangeBodyCondition(BODYCON_CHARACTER_MODE_STOP);
					// 2010-06-15 by shcho&hslee Æê½Ã½ºÅÛ - ¸¶À» ÀÌµ¿ Ã³¸®
					//ChangeWearItemBodyConditionAllProcess(BODYCON_CHARACTER_MODE_STOP);
					g_pShuttleChild->ChangeWearItemBodyCondition(WEAR_ITEM_KIND_ACCESSORY_UNLIMITED, BODYCON_CHARACTER_MODE_STOP);
					g_pShuttleChild->ChangeWearItemBodyCondition(WEAR_ITEM_KIND_ACCESSORY_TIME_LIMIT, BODYCON_CHARACTER_MODE_STOP);
					g_pShuttleChild->ChangeWearItemBodyCondition(WEAR_ITEM_KIND_WINGIN, BODYCON_CHARACTER_MODE_STOP);
// 					g_pShuttleChild->ChangeWearItemBodyCondition(WEAR_ITEM_KIND_ATTACHMENT, BODYCON_CHARACTER_MODE_STOP);
// 					g_pShuttleChild->ChangeWearItemBodyCondition(WEAR_ITEM_KIND_ACCESSORIES, BODYCON_CHARACTER_MODE_STOP);
// 					g_pShuttleChild->ChangeWearItemBodyCondition(WEAR_ITEM_KIND_WINGIN, BODYCON_CHARACTER_MODE_STOP);
					// 2010-06-08 by dgwoo, Æê½Ã½ºÅÛ Ãß°¡. 
					m_dwState = _STAND;
					m_bPickMove = FALSE;
//					m_vPos = m_vPicking;
				}
			else
			{
				// Ä³¸¯ÅÍ´Â  Picking¶§¸¸ ¿òÁ÷ÀÎ´Ù.
				CheckMoveRate(fElapsedTime);
				m_vPos += (m_vNextPos - m_vPos) * fElapsedTime;
				m_fOldLength = lenght;
			}
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// ¿ÀºêÁ§Æ® Ãæµ¹
	// ¿©±â¼­ ¿ÀºêÁ§Æ®´Â »óÁ¡ ³»ºÎÀÌ´Ù.
	// Ä³¸¯ÅÍ´Â ¿ÀºêÁ§Æ® À§¿¡ Á¸ÀçÇÑ´Ù´Â Á¤ÀÇ·Î ½ÃÀÛÇÑ´Ù. ¿Àºê ³ôÀÌ »êÃâ
	// 2006-07-14 by ispark, Ä³¸¯ÅÍ ¿öÇÁ Áß¿¡´Â ¿ÀºêÁ§Æ® Ãæµ¹ °Ë»ç¸¦ ¾ÈÇÑ´Ù.
	if(m_dwState == _WARP)
	{
		// ¿öÇÁ Áß¿¡ ÀÌµ¿
		m_vPos += m_vMoveVel * fElapsedTime;
	}
	else if(!m_bStartPositionEvent)						// 2006-07-20 by ispark, ½ÃÀÛ À§Ä¡°¡ ÀÌº¥Æ® À§Ä¡°¡ ¾Æ´Ï¶ó¸é
	{
		float fTempHeight = 0.0f;
		float fObjHeight = -DEFAULT_COLLISION_DISTANCE;
		
		COLLISION_RESULT GroundCollResult;
		D3DXMATRIX matTemp;
		D3DXVECTOR3 vSide(0, 0, 1), vUp(0, 1, 0);
		D3DXMatrixLookAtLH(&matTemp,&vPos,&(vPos - vUp),&vSide);
	//	fTempHeight = g_pScene->m_pObjectRender->CheckCollMesh(matTemp,vPos).fDist - CHARACTER_HEIGHT;
		GroundCollResult = g_pScene->m_pObjectRender->CheckCollMesh(matTemp,vPos);

		fTempHeight = GroundCollResult.fDist - CHARACTER_HEIGHT;

		if(GroundCollResult.vNormalVector.y > 0.7f)
		{
			if(fTempHeight<m_vPos.y)
				m_vPos.y += -fTempHeight;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// ÀÌµ¿ ¹üÀ§ Á¦ÇÑ
	// ´ÙÀ½ °è»êÇÑ ¿ÀºêÁ§Æ®¿ÍÀÇ °Å¸®·Î Çã¿ë ¹üÀ§¸¦ °è»êÇÑ´Ù.
	// _NCITYIN Ã³À½ »óÁ¡¿¡ µé¾î¿Ã¶§´Â ÀÌµ¿ ¹üÀ§ Á¦ÇÑ °Ë»ç¸¦ ¾ÈÇÏ´Ù.
	// ÇÒ°æ¿ì Ä³¸¯ÅÍ¸¦ ¿ÀºêÁ§Æ® À§¿¡ ¶ç¿ì´Âµ¥ ¹®Á¦°¡ »ý±è
	// ¼­¹ö·Î ºÎÅÍ Y°ªÀ» Á¦¿ÜÇÑ °ª¸¸ ¹Þ°í Å¬¶óÀÌ¾ðÆ®¿¡¼­ Y°ªÀ» °è»ê
	// ±×·¯³ª »óÁ¡ ³ôÀÌ°¡ º¯ÇÒ ¼ö ÀÖ´Â °ü°è·Î ²Ï ³ôÀº °ªÀ¸·Î ¼³Á¤
	// Ä³¸¯ÅÍ°¡ ³ªÅ¸³ª´Â À§Ä¡¿¡´Â À§ÂÊÀ¸·Î ¿ÀºêÁ§Æ®°¡ ¾ø´Ù´Â Á¸ÀçÇÏ¿¡ ¼³Á¤
	// 2006-07-14 by ispark, Ä³¸¯ÅÍ ¿öÇÁ Áß¿¡´Â ¿ÀºêÁ§Æ® Ãæµ¹ °Ë»ç¸¦ ¾ÈÇÑ´Ù.
	if(m_dwState != _NCITYIN && m_dwState != _WARP)
	{
		float fCollResult;
		COLLISION_RESULT CollResult;
		D3DXMATRIX matArray[2];
		D3DXVECTOR3 vCollSide,vVel,vFrontPos, vDownPos;
		
		vFrontPos = vPos;			// »ç¶÷ Å°³ôÀÌ¸¦ »ý°¢ÇÏÀÚ
		vDownPos = vPos;			// Àýº®¸¦ °è»êÇÏ±â À§ÇØ¼­ ³» À§Ä¡ º¸´Ù ´õ ¾ÕÂÊ ÁÂÇ¥ ÀÔ·Â
		vDownPos += m_vVel * 5;

		D3DXVec3Cross(&vCollSide,&m_vUp,&m_vVel);
		
		D3DXMatrixLookAtLH(&matArray[0],&vFrontPos,&(vFrontPos + m_vVel),&m_vUp);		// ¾Õ
		D3DXMatrixLookAtLH(&matArray[1],&vDownPos,&(vDownPos - m_vUp),&vCollSide);		// ¾Æ·¡
		
		float fFrontMove = m_fCharacterSpeed*fElapsedTime;
		float size[2] = {7 + fFrontMove, CHARACTER_HEIGHT + 4.0f};	// À¯´Ö Å©±â	¾Õ, ¾Æ·¡
		
		// ¿ÀºêÁ§Æ®¿Í Ãæµ¹ Ã³¸® 2004.06.18 jschoi
		
		if(	/*m_bCollObjectCheck && */
			g_pScene &&
			g_pScene->m_byMapType != MAP_TYPE_TUTORIAL &&
			g_pScene->m_pObjectRender)
		{
			// Á¤¸é
			CollResult = g_pScene->m_pObjectRender->CheckCollMesh(matArray[0], vPos);	
			// 2005-07-29 by ispark
			// ¿ÀºêÁ§Æ®¾È¿¡¼­ ³ª¿ÀÁö ¸øÇÏ´Â ¹ö±× ¿¹¿Ü Ã³¸®(³ª¿À°Ô ÇÔ)
//			DBGOUT("¹æÇâ %f %f %f\n", m_vVel.x, m_vVel.y, m_vVel.z);
//			DBGOUT("³ë¸» %.2f %.2f %.2f\n", CollResult.vNormalVector.x, CollResult.vNormalVector.y, CollResult.vNormalVector.z);
//			DBGOUT("°Å¸® %f\n", CollResult.fDist);
			float dot = D3DXVec3Dot(&m_vVel, &CollResult.vNormalVector);
			if(0.0f >= dot)
			{
//				DBGOUT("¹æÇâ %f %f %f\n", m_vVel.x, m_vVel.y, m_vVel.z);
//				DBGOUT("³ë¸» %f %f %f\n", CollResult.vNormalVector.x, CollResult.vNormalVector.y, CollResult.vNormalVector.z);
//				if(CollResult.fDist < size[0] && m_dwState != _STAND)
				// 2005-08-04 by ispark
				// °æ»ç °¢µµ·Î °è»ê
				if((CollResult.fDist < size[0] && CollResult.vNormalVector.y <= 0.7f) && m_dwState != _STAND)
				{
					// size[0] ¾È¿¡ ÀÖÀ¸¸é Ãæµ¹
					m_vPos = vPos;
					m_vPos.y -= CHARACTER_HEIGHT;

					m_dwState = _STAND;
					m_bPickMove = FALSE;
//					g_pShuttleChild->SendFieldSocketChangeBodyCondition(g_pShuttleChild->m_myShuttleInfo.ClientIndex, BODYCON_CHRACTER_MODE_STOP);
					m_pCharacterInfo->ChangeBodyCondition(BODYCON_CHARACTER_MODE_STOP);
					// 2010-06-08 by dgwoo, Æê½Ã½ºÅÛ Ãß°¡. ¾Æ·¡ÀÇ ÇÔ¼ö·Î ÅëÇÕ. 
					ChangeWearItemBodyConditionAllProcess(BODYCON_CHARACTER_MODE_STOP);
					
// 					g_pShuttleChild->ChangeWearItemBodyCondition(WEAR_ITEM_KIND_ATTACHMENT, BODYCON_CHARACTER_MODE_STOP);
// 					g_pShuttleChild->ChangeWearItemBodyCondition(WEAR_ITEM_KIND_ACCESSORIES, BODYCON_CHARACTER_MODE_STOP);
// 					g_pShuttleChild->ChangeWearItemBodyCondition(WEAR_ITEM_KIND_WINGIN, BODYCON_CHARACTER_MODE_STOP);
					// 2010-06-08 by dgwoo, Æê½Ã½ºÅÛ Ãß°¡. 
				}
			}

			// Àýº®
			fCollResult = g_pScene->m_pObjectRender->CheckCollMesh(matArray[1], m_vPos).fDist - CHARACTER_HEIGHT;
			if(fCollResult > size[1] && m_dwState != _STAND)
			{
				// size[1] Çã¿ë ¼öÄ¡º¸´Ù Å©¸é Á¤Áö
				m_vPos = vPos;
				m_vPos.y -= CHARACTER_HEIGHT;

				m_dwState = _STAND;
				m_bPickMove = FALSE;
//				g_pShuttleChild->SendFieldSocketChangeBodyCondition(g_pShuttleChild->m_myShuttleInfo.ClientIndex, BODYCON_CHRACTER_MODE_STOP);
				m_pCharacterInfo->ChangeBodyCondition(BODYCON_CHARACTER_MODE_STOP);
				// 2010-06-08 by dgwoo, Æê½Ã½ºÅÛ Ãß°¡. ¾Æ·¡ÀÇ ÇÔ¼ö·Î ÅëÇÕ. 
				ChangeWearItemBodyConditionAllProcess(BODYCON_CHARACTER_MODE_STOP);
// 				g_pShuttleChild->ChangeWearItemBodyCondition(WEAR_ITEM_KIND_ATTACHMENT, BODYCON_CHARACTER_MODE_STOP);
// 				g_pShuttleChild->ChangeWearItemBodyCondition(WEAR_ITEM_KIND_ACCESSORIES, BODYCON_CHARACTER_MODE_STOP);
// 				g_pShuttleChild->ChangeWearItemBodyCondition(WEAR_ITEM_KIND_WINGIN, BODYCON_CHARACTER_MODE_STOP);
				// 2010-06-08 by dgwoo, Æê½Ã½ºÅÛ Ãß°¡.
			}
			else if(m_vPos.y < vPos.y - CHARACTER_HEIGHT - 5.0f)		// Áö±Ý ¶³¾îÁö´Â À§Ä¡ÀÎ°¡
			{															// 5.0fÀº °æ»ç¸éÀÌ ¾Æ´Ñ°ÍÀ» È®ÀÎÇÏ±â À§ÇØ¼­
				// Çã¿ë ¼öÄ¡ÀÌ¸é ºÎµå·´°Ô ¶³¾îÁø´Ù.
				// 2005-08-04 by ispark
				// ¾ÕÂÊ ¹æÇâÀº Á¶±Ý ¿òÁ÷ÀÎ´Ù.
				D3DXVECTOR3 vVel = m_vNextPos - vPos;
				D3DXVec3Normalize(&vVel, &vVel);
				m_vPos = vPos + (vVel) * fElapsedTime;
				m_vPos.y = vPos.y - CHARACTER_HEIGHT - (CHARACTER_HEIGHT * fElapsedTime * 6.0f);
			}
		}
	}
	else if(m_dwState == _NCITYIN)
	{
		// Move_Character()¿¡¼­ Ã³À½À¸·Î ½ÃÀÛÇØ¾ß ÇÏ´Â ºÎºÐ
		// _NCITYIN ÀÌ¶ó¸é _STAND·Î ¹Ù²Û´Ù. 
		// _NCITYIN ÇÑ¹ø¸¸ »ç¿ë
		m_dwState = _STAND;
		m_bPickMove = FALSE;
		m_bStartPositionEvent = FALSE;
		// Ä«¸Þ¶ó ÃÊ±â ¼ÂÆÃ
		// InitCharacterData()¿¡¼­ ÇÏÁö ¾Ê´Â ÀÌÀ¯´Â Âø·úÈÄ Ä«¸Þ¶ó À§Ä¡¿¡ ¿ÀÂ÷°¡ »ý±è
		g_pCamera->SetCamDistance(CHARACTER_CAMERA_DISTANCE);		// °Å¸®
		g_pD3dApp->SetCamPosInit();
	}

	//////////////////////////////////////////////////////////////////////////
	// ÀÌº¥Æ® Áö¿ª Ã¼Å© - Shop
	int nCheckX = ((int)m_vPos.x)/TILE_SIZE;
	int nCheckZ = ((int)m_vPos.z)/TILE_SIZE;
	if((g_pGround->m_pTileInfo[nCheckX*g_pGround->m_projectInfo.sYSize + nCheckZ].bEventType != EVENT_TYPE_ENTER_BUILDING &&
		g_pGround->m_pTileInfo[nCheckX*g_pGround->m_projectInfo.sYSize + nCheckZ].bEventType != EVENT_TYPE_ENTER_BUILDING_BAZAAR) &&
		m_dwState != _STAND)
	{
		m_vPos = vPos;
		m_vPos.y -= CHARACTER_HEIGHT;
		
		m_dwState = _STAND;
		m_bPickMove = FALSE;
//		g_pShuttleChild->SendFieldSocketChangeBodyCondition(g_pShuttleChild->m_myShuttleInfo.ClientIndex, BODYCON_CHRACTER_MODE_STOP);
		m_pCharacterInfo->ChangeBodyCondition(BODYCON_CHARACTER_MODE_STOP);
		// 2010-06-08 by dgwoo, Æê½Ã½ºÅÛ Ãß°¡. ¾Æ·¡ÀÇ ÇÔ¼ö·Î ÅëÇÕ. 
		ChangeWearItemBodyConditionAllProcess(BODYCON_CHARACTER_MODE_STOP);
		//g_pShuttleChild->ChangeWearItemBodyCondition(WEAR_ITEM_KIND_ATTACHMENT, BODYCON_CHARACTER_MODE_STOP);
		//g_pShuttleChild->ChangeWearItemBodyCondition(WEAR_ITEM_KIND_ACCESSORIES, BODYCON_CHARACTER_MODE_STOP);
		//g_pShuttleChild->ChangeWearItemBodyCondition(WEAR_ITEM_KIND_WINGIN, BODYCON_CHARACTER_MODE_STOP);
		// 2010-06-08 by dgwoo, Æê½Ã½ºÅÛ Ãß°¡.

	}

	m_bBazaarEventPos = FALSE;
	if(g_pGround->m_pTileInfo[nCheckX*g_pGround->m_projectInfo.sYSize + nCheckZ].bEventType == EVENT_TYPE_ENTER_BUILDING_BAZAAR)
	{
		m_bBazaarEventPos = TRUE;
	}
	//////////////////////////////////////////////////////////////////////////
	SetMatrix_Move_Character(fElapsedTime);
	//////////////////////////////////////////////////////////////////////////
	// ShuttleChild¿Í °°ÀÌ ÀÔ·ÂÇØ¾ß ÇÑ´Ù.
	g_pShuttleChild->m_vPos = m_vNextPos = m_vPos;
	g_pShuttleChild->m_vVel = m_vVel;
	g_pShuttleChild->m_vUp = m_vUp;
	g_pShuttleChild->m_vSideVel;

	// 2005-07-28 by ispark
	// Move ÆÐÅ¶À» º¸³¾¶§ Up º¤ÅÍ¸¦ Metrix¿¡¼­ °¡Á®¿Â´Ù. 
	g_pShuttleChild->SetMatrix_Move_Ground(fElapsedTime);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			CShuttleChild::SetMatrix_Move_Character(float fElapsedTime)
/// \brief		Ä³¸¯ÅÍ ¸ÞÆ®¸¯½º ¼ÂÆÃ
/// \author		ispark
/// \date		2005-07-13 ~ 2005-07-13
/// \warning	
///
/// \param		
/// \return		void
///////////////////////////////////////////////////////////////////////////////
void CCharacterChild::SetMatrix_Move_Character(float fElapsedTime)
{
	D3DXVec3Normalize(&m_vUp,&m_vUp);
	D3DXVec3Normalize(&m_vVel,&m_vVel);
	D3DXVec3Cross(&m_vSideVel,&m_vUp,&m_vVel);
	D3DXVec3Cross(&m_vVel,&m_vSideVel,&m_vUp);

	D3DXMatrixLookAtRH(&m_mMatrix, &m_vPos, &(m_vPos + m_vVel), &m_vUp);
	D3DXMatrixInverse(&m_mMatrix, nullptr, &m_mMatrix );

	D3DXMATRIX matTemp;
	D3DXMatrixScaling(&matTemp, CHARACTER_SCALE, CHARACTER_SCALE, CHARACTER_SCALE);
	m_mMatrix = matTemp * m_mMatrix;
}

void CCharacterChild::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	FLOG( "CCharacterChild::WndProc()" );
	// 2006-07-14 by ispark, ¿öÇÁ½Ã ÇÁ·Î½ÃÀú ±ÝÁö
	if(m_dwState == _WARP)
		return;
	
	float fElapsedTime = g_pD3dApp->GetElapsedTime();

	if(!g_pD3dApp->m_bChatMode)// && m_bMoveInputType == 0)
	{
		switch(uMsg)
		{
		case WM_KEYDOWN:
			{
				switch(lParam)
				{
				case WM_KEYDOWN_LPARAM_Z:
					{
//						if( g_pShuttleChild->m_pClientParty->GetNtoNCallState() ||
//							g_pGameMain->m_pCommunity->GetGuild()->GetNtoNCallState())
//						{
//							// 2008-10-14 by bhsohn VOIP°³¼±							
////							COPYDATASTRUCT	copyData;
////							memset(&copyData, 0x00, sizeof(COPYDATASTRUCT));
////							AV_MSG_WM_COPYDATA voIP(_NtoNCall, AV_MT_TALK, 0);
////							copyData.dwData = (DWORD)voIP;
////							copyData.lpData = NULL;
////							copyData.cbData = 0;
////							::SendMessage(g_pD3dApp->m_VOIPState.m_hWndAtumVoIPClient, WM_COPYDATA, (WPARAM)g_pD3dApp->GetHwnd(), (LPARAM)&copyData);
//							//g_pGameMain->ChangeVOIPMode();
//						}
					}
					break;
				}

//				switch(wParam)
//				{
//				case VK_ADD:
//					{
//						m_fCharacterMoveRate += 0.1f;
//					}
//					break;
//				case VK_SUBTRACT:
//					{
//						m_fCharacterMoveRate -= 0.1f;
//					}
//					break;
//				}
			}
		case WM_KEYUP:
			{
				switch(lParam)
				{
				case WM_KEYUP_LPARAM_Z:
					{
						// 2008-10-14 by bhsohn VOIP°³¼±							
//						if( g_pShuttleChild->m_pClientParty->GetNtoNCallState() == TRUE ||
//							g_pGameMain->m_pCommunity->GetGuild()->GetNtoNCallState() == TRUE)
//						{
//							COPYDATASTRUCT	copyData;
//							memset(&copyData, 0x00, sizeof(COPYDATASTRUCT));
//							AV_MSG_WM_COPYDATA voIP(_NtoNCall, AV_MT_TALK_CANCEL, 0);
//							copyData.dwData = (DWORD)voIP;
//							copyData.lpData = NULL;
//							copyData.cbData = 0;
//							::SendMessage(g_pD3dApp->m_VOIPState.m_hWndAtumVoIPClient, WM_COPYDATA, (WPARAM)g_pD3dApp->GetHwnd(), (LPARAM)&copyData);
//						}
						// end 2008-10-14 by bhsohn VOIP°³¼±							
					}
				}
				break;
			}
		case WM_MBUTTONDOWN:
			{				
			}
			break;
		case WM_MBUTTONUP:
			{
			}
			break;
		case WM_MOUSEWHEEL:
			{
				// 2005-08-18 by ispark
				// Ä³¸¯ÅÍ Ä«¸Þ¶ó ÈÙ
				m_bMouseWheel = TRUE;
				float nDist = g_pCamera->GetCamDistance();
				float fCollDist = g_pCamera->GetCollDistance();			// 2005-08-19 by ispark

				if((int)wParam > 0)
				{
					// 2005-08-19 by ispark
					// Ãæµ¹ °Å¸® ÀÖÀ» ¶§ Ä«¸Þ¶ó µÚ·Î °¡±â °è»ê ¾ÈÇÔ
					if(fCollDist != 0 && fCollDist < nDist)
					{
						break;
					}

					float fDistMax = CHARACTER_CAMERA_DISTANCE_MAX;					
#ifdef _DEBUG
					fDistMax = 600.0f;
#endif
					if(nDist + CHARACTER_CAMERA_WHEEL_DISTANCE > fDistMax)
					{
						nDist = fDistMax;
						g_pCamera->SetCamDistance(nDist);
					}
					else
					{
						g_pCamera->SetCamDistance(nDist + CHARACTER_CAMERA_WHEEL_DISTANCE);
					}
				}
				else
				{
					// 2005-08-19 by ispark
					// °¡±îÀÌ °¥ ¶§¸¸ Ãæµ¹ °Å¸® Àû¿ë(½ÇÁ¦ °Å¸® »èÁ¦)
					if(fCollDist != 0 && fCollDist < nDist)
					{
						if(fCollDist < CHARACTER_CAMERA_DISTANCE_MIN)
						{
							g_pCamera->SetCamDistance(fCollDist);
							break;
						}

						nDist = fCollDist;
					}

					if(nDist - CHARACTER_CAMERA_WHEEL_DISTANCE < CHARACTER_CAMERA_DISTANCE_MIN)
					{
						nDist = CHARACTER_CAMERA_DISTANCE_MIN;
						g_pCamera->SetCamDistance(nDist);
					}
					else
					{
						g_pCamera->SetCamDistance(nDist - CHARACTER_CAMERA_WHEEL_DISTANCE);
					}
				}
			}
			break;
		case WM_LBUTTONDOWN:
			{
				POINT pt;
				GetCursorPos(&pt);
				ScreenToClient(g_pD3dApp->GetHwnd(), &pt);
				CheckMouseReverse(&pt);
	
				// Pick
				// 2005-08-25 by ispark, ÆË¾÷ ¸Þ´º ¶ç¿ì»óÅÂ¶ó¸é
				// 2006-06-20 by ispark, ¼­¹ö·Î ºÎÅÍ ÀÀ´äÀ» ±â´Ù¸®´Â ÁßÀÌ¶ó¸é Ä³¸¯ÅÍ°¡ ¿òÁ÷ÀÌÁö ¸øÇÏ°Ô ÇÑ´Ù.
				// (ÀÎÅÍÆäÀÌ½º Ã¢À» ´©¸£¸é PickingÀÌ µÇ±â ¶§¹®ÀÌ´Ù.) 
				// 2007-12-17 by bhsohn ¾ÆÀÌÅÛ Á¤º¸°¡ ´Ù ¾È¿À¸é °ÔÀÓ ´ë±â»óÅÂ·Î
				//if(g_pGameMain->m_bMenuLock || g_pD3dApp->m_bRequestEnable == FALSE)
				if(g_pGameMain->m_bMenuLock || g_pD3dApp->IsLockMode() == FALSE)
					break;

				// 2005-09-14 by ispark
				// ±âº»ÀÎÅÍÆäÀÌ½º ¹ØÁÙ Å¬¸¯À» ¸·´Â´Ù.
				if(g_pInterface->m_bShowInterface && 
					(pt.x >= 0 && pt.x <= g_pD3dApp->GetBackBufferDesc().Width) &&
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI½Ã½ºÅÛ º¯°æ
					(pt.y >= g_pD3dApp->GetBackBufferDesc().Height - 70) || // #define CHATBOX_FIELD_SHOWCHATBOX_INIT_HEIGHT			70 -> INFGameMainChat¿¡ ÀÖÀ½
#else				 
					(pt.y >= g_pD3dApp->GetBackBufferDesc().Height - 39) ||
#endif
					((pt.y >= g_pD3dApp->GetBackBufferDesc().Height - 51) && 
					(pt.x >= g_pD3dApp->GetBackBufferDesc().Width / 2 - 117 &&
					pt.x <= g_pD3dApp->GetBackBufferDesc().Width / 2 + 117)))
				{
					break;
				}
					
				m_dwState = _STAND;
				
				COLLISION_RESULT Coll_Result_Pick;
				Coll_Result_Pick = g_pScene->m_pObjectRender->CheckPickMesh(pt.x, pt.y);

				// 2007-07-27 by bhsohn ÇÁ¸®½ºÄ« ¸Ê¸¸ »óÁ¡ ÀÎ½Ä ¹üÀ§ Áõ°¡
				float fSearchLength = CHARACTER_SHOP_SERCH_DISTANCE;
				// 2009. 10. 14 by jskim ÇÁ¸®½ºÄ« Á¦°Å 
// 				if(IS_NORMAL_CITY_MAP_INDEX(g_pShuttleChild->GetMyShuttleInfo().MapChannelIndex.MapIndex))
// 				{
// 					fSearchLength = CHARACTER_SHOP_SERCH_FREESKA_DISTANCE;
// 				}
				//end 2009. 10. 14 by jskim ÇÁ¸®½ºÄ« Á¦°Å 
				//if(!CheckEnterShopNPC(&pt, CHARACTER_SHOP_SERCH_DISTANCE, TRUE))
				if(!CheckEnterShopNPC(&pt, fSearchLength, TRUE))
				{
					// PickingÀ» ¹Ù´ÚÀ¸·Î Á¦ÇÑ
	//				DBGOUT("º¤ÅÍ %.2f %.2f %.2f\n", Coll_Result_Pick.vPicking.x, Coll_Result_Pick.vPicking.y, Coll_Result_Pick.vPicking.z);
	//				DBGOUT("³ë¸» %.2f %.2f %.2f\n", Coll_Result_Pick.vNormalVector.x, Coll_Result_Pick.vNormalVector.y, Coll_Result_Pick.vNormalVector.z);
					if(Coll_Result_Pick.vNormalVector.y > 0.7f)
					{
						m_bPickMove = TRUE;
						m_vPicking = Coll_Result_Pick.vPicking;
						D3DXVECTOR3 vPosTemp = m_vPos;
						D3DXVECTOR3 vPickTemp = m_vPicking;
						// Picking À§Ä¡ ¼ÂÆÃ
						SetMatrix_Picking(m_vPicking, Coll_Result_Pick.vNormalVector);
						m_pPickingInfo->SetSingleBodyConditionMatrix(BODYCON_LANDED_MASK, m_mPickMatrix);
						
						vPosTemp.y = vPickTemp.y = 0.0f;
						m_vPickDir = vPickTemp - vPosTemp;
					}
				}
			}
			break;
		case WM_LBUTTONUP:
			{
			}
			break;
		case WM_RBUTTONDOWN:
			{
				if(g_pCamera->GetCamType() == CAMERA_TYPE_NORMAL)
				{
					g_pCamera->m_bIsCamControl = TRUE;
					POINT pt;
					pt.x = LOWORD(lParam);
					pt.y = HIWORD(lParam);
					CheckMouseReverse(&pt);

					m_pOldMousePoint = pt;
				}
				
				m_bRButtonState = TRUE;
			}
			break;
		case WM_RBUTTONUP:
			{
				// Ä«¸Þ¶ó ÀüÈ¯ ¸ðµå ÇØÁ¦
				if(g_pD3dApp->m_pCamera->m_bIsCamControl)
				{
					g_pD3dApp->m_pCamera->m_bIsCamControl = FALSE;
				}
				m_bRButtonState = FALSE;
			}
			break;
		case WM_MOUSEMOVE:
			{
				POINT pt;
				pt.x = LOWORD(lParam);
				pt.y = HIWORD(lParam);
				CheckMouseReverse(&pt);

				// 2007-07-27 by bhsohn ÇÁ¸®½ºÄ« ¸Ê¸¸ »óÁ¡ ÀÎ½Ä ¹üÀ§ Áõ°¡
				//CheckEnterShopNPC(&pt, CHARACTER_SHOP_SERCH_DISTANCE);
				float fSearchLength = CHARACTER_SHOP_SERCH_DISTANCE;
				// 2009. 10. 14 by jskim ÇÁ¸®½ºÄ« Á¦°Å 
				//if(IS_NORMAL_CITY_MAP_INDEX(g_pShuttleChild->GetMyShuttleInfo().MapChannelIndex.MapIndex))
				//{
				//	fSearchLength = CHARACTER_SHOP_SERCH_FREESKA_DISTANCE;
				//}				
				//end 2009. 10. 14 by jskim ÇÁ¸®½ºÄ« Á¦°Å  
				CheckEnterShopNPC(&pt, fSearchLength);
				
				// end 2007-07-27 by bhsohn ÇÁ¸®½ºÄ« ¸Ê¸¸ »óÁ¡ ÀÎ½Ä ¹üÀ§ Áõ°¡

				

				m_bMouseMove = TRUE;

//				if(g_pCamera->GetCamType() == CAMERA_TYPE_NORMAL)
//				{
//					if( g_pCamera->GetCamType() == CAMERA_TYPE_NORMAL &&
//						g_pD3dApp->m_pCamera->m_bIsCamControl && 
//						m_bRButtonState)
//					{
//						float fDirY = 0.0f, fDirX = 0.0f;
//						fDirY = m_pOldMousePoint.y - pt.y;
//						fDirX = m_pOldMousePoint.x - pt.x;
//						if(fDirY != 0)
//						{
//							// Ä³¸¯ÅÍ Ä«¸Þ¶ó È¸Àü½Ã ¿ÀºêÁ§Æ®¸¦ Åë°ú¸¦ ÇÏ´Âµ¥ ±×ÀÌÀ¯´Â ¸¶¿ì½º ÀÌµ¿ÀÌ Ä¿¼­ ±×·¸´Ù.
//							// ±×·¡¼­ Á¦ÇÑÀ» µÎ¾ú´Ù.
//							if(20 < fDirY)
//								fDirY = 20;
//							if(-20 > fDirY)
//								fDirY = -20;
//						}
//						g_pCamera->SetAngleY(fDirY);
//						
//						if(fDirX != 0)
//						{
//							if(20 < fDirX)
//								fDirX = 20;
//							if(-20 > fDirX)
//								fDirX = -20;					
//						}
//						g_pCamera->SetAngleX(fDirX);
//					}
//				}
//				m_pOldMousePoint = pt;
			}
			break;
		}

	}
}

void CCharacterChild::CheckShadowPos()
{
	// ¿©±â¼­ ¼öÄ¡´Â ±×¸²ÀÚ¸¦ »Ñ¸± »çÀÌÁî¸¦ ¶æÇÑ´Ù. ¿øÁ¡¿¡¼­ ´ë°¢¼± ¹æÇâÀ¸·Î °Å¸®ÀÌ´Ù.
	m_vShadowPos[0] = m_vPos + CHARACTER_SHADOW_SCALE * (m_vVel - m_vSideVel);
	m_vShadowPos[1] = m_vPos + CHARACTER_SHADOW_SCALE * (m_vVel + m_vSideVel);
	m_vShadowPos[2] = m_vPos - CHARACTER_SHADOW_SCALE * (m_vVel - m_vSideVel);
	m_vShadowPos[3] = m_vPos - CHARACTER_SHADOW_SCALE * (m_vVel + m_vSideVel);

	for(int i = 0 ; i < 4 ; i++)
	{
		m_vShadowPos[i].y += CHARACTER_HEIGHT;
		float fTempHeight = 0.0f;
		float fObjHeight = -DEFAULT_COLLISION_DISTANCE;

		D3DXMATRIX matTemp;
		D3DXVECTOR3 vSide(0, 0, 1), vUp(0, 1, 0);
		D3DXMatrixLookAtLH(&matTemp,&m_vShadowPos[i],&(m_vShadowPos[i] - vUp),&vSide);
		fTempHeight = g_pScene->m_pObjectRender->CheckCollMesh(matTemp,m_vShadowPos[i]).fDist - CHARACTER_HEIGHT;

		if(fTempHeight <= 1.0f && fTempHeight >= -1.0f)
		{
			m_vShadowPos[i].y += -fTempHeight;
		}
		else
		{
			if(fTempHeight > 0) 
			{
				m_vShadowPos[i].y += -fTempHeight;
			}
			else
			{
				float fRealObjectHeight = m_vShadowPos[i].y - fTempHeight;
				if( fRealObjectHeight > m_vShadowPos[i].y )
				{
					m_vShadowPos[i].y = fRealObjectHeight;
				}
			}
		}

		// ±×¸²ÀÚ´Â Z¹öÆÛ¸¦ »ç¿ëÇÏ¹Ç·Î ±×¸²ÀÚ ³ôÀÌ¸¦ 0.1f¸¸Å­ ¿Ã¸°´Ù.
		if(g_pD3dApp->m_nDephbias == -1)
		{
			m_vShadowPos[i].y -= (CHARACTER_HEIGHT - 0.1f);	
		}
		else
		{
			m_vShadowPos[i].y -= CHARACTER_HEIGHT;
		}
	}

	SPRITEVERTEX* pV;
	m_pVBShadow->Lock( 0, 0, (void**)&pV,	0 );
	pV[0].p = m_vShadowPos[3]; 
	pV[1].p = m_vShadowPos[0]; 
	pV[2].p = m_vShadowPos[2]; 
	pV[3].p = m_vShadowPos[1];
	m_pVBShadow->Unlock();
}

void CCharacterChild::CheckMoveRate(float fElapsedTime)
{
	if(m_dwState == _WALK)
	{
		m_fCharacterSpeed = CHARACTER_WALK;
	}
	else if(m_dwState == _RUN)
	{
//		// 2006-02-09 by ispark, ³²ÀÚ, ¿©ÀÚ ¿òÁ÷ÀÓ ¼Óµµ ´Ù¸§
//		if(g_pShuttleChild->m_myShuttleInfo.PilotFace < 100)
//		{
//			m_fCharacterSpeed = CHARACTER_WOMAN_RUN;		
//		}
//		else
//		{
//			m_fCharacterSpeed = CHARACTER_MAN_RUN;		
//		}
		// 2006-11-02 by ispark, Ä³¸¯ÅÍ º°·Î ¿òÁ÷ÀÓÀ¸·Î º¯°æ
		m_fCharacterSpeed = GetCharacterSteps(g_pShuttleChild->m_myShuttleInfo.PilotFace);

		m_fCharacterSpeed += m_fCharacterAddSpeed;

// 		m_fCharacterSpeed = GetCharacterSteps(g_pShuttleChild->m_myShuttleInfo.PilotFace) + m_fCharacterMoveRate;
	}

	m_vNextPos += m_vVel * m_fCharacterSpeed;
}

void CCharacterChild::InitEffect()
{
	FLOG( "CCharacterChild::InitEffect()" );
	char strFileName[16];
//	CItemInfo* pItem = m_pStoreData->FindItemInInventoryByWindowPos( POS_CENTER );
//	// 2005-07-20 by ispark
//	// Ä³¸¯ÅÍ ·»´õ¸µ ¹øÈ£¸¦ ÀÔ·ÂÇÑ´Ù. ¿©±â¿¡ ÀÌÆåÆ® Æ÷ÇÔ
////	ChangeUnitCharacterInfo((pItem?pItem->ItemNum:0), g_pShuttleChild->m_myShuttleInfo.PilotFace, GetCurrentBodyCondition(), FALSE);
	int nTemp = ::GetUnitNum(0, 0, g_pShuttleChild->m_myShuttleInfo.PilotFace, TRUE);

//	nTemp = 20000100;
	if(nTemp != m_nUnitNum)
	{
		m_nUnitNum = nTemp;
		wsprintf( strFileName, "%08d", m_nUnitNum );
		LoadCharacterEffect(&m_pCharacterInfo, strFileName);
		if(m_pCharacterInfo)
		{
			m_pCharacterInfo->SetAllBodyConditionMatrix(m_mMatrix);
			m_pCharacterInfo->SetCharacterAnimationBodyConditionMask(BODYCON_HUMAN_ANIMATION_TIME);
			m_pCharacterInfo->SetBodyCondition(m_hyBodyCondition);
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// Picking ÀÌÆåÆ®6
	wsprintf(strFileName, "%08d", PICKING_OBJECT_NUMBER);
	LoadCharacterEffect(&m_pPickingInfo, strFileName);
	if(m_pPickingInfo)
	{
		m_pPickingInfo->SetAllBodyConditionMatrix(m_mPickMatrix);
		m_pPickingInfo->SetBodyCondition(BODYCON_LANDED_MASK);
	}
}

void CCharacterChild::InitCharacterData()
{
	D3DXVECTOR3 vTempPos;
	m_vUp = D3DXVECTOR3(0, 1, 0);
	vTempPos.x = g_pGround->m_projectInfo.sXSize*TILE_SIZE/2.0f+RAND100();
	vTempPos.y = m_vPos.y;
	vTempPos.z = g_pGround->m_projectInfo.sYSize*TILE_SIZE/2.0f+RAND100();
	D3DXVec3Normalize(&m_vVel,&(vTempPos-m_vPos));
	// 2007-07-04 by dgwoo ÇÁ¸®½ºÄ« ¸Ê¿¡¼­¸¸ ¹Ù¶óº¸´Â ¹æÇâÀ» ¹Ý´ë ¹æÇâÀ¸·Î º¯°æ.
	// 2009. 10. 14 by jskim ÇÁ¸®½ºÄ« Á¦°Å 
// 	if(IS_NORMAL_CITY_MAP_INDEX(g_pShuttleChild->GetMyShuttleInfo().MapChannelIndex.MapIndex))
// 	{
// 		m_vVel = -m_vVel;
// 	}
	//end 2009. 10. 14 by jskim ÇÁ¸®½ºÄ« Á¦°Å 
	DBGOUT("CCharacterChild,m_vPos(%.2f, %.2f, %.2f)\n", m_vPos.x, m_vPos.y, m_vPos.z);
	DBGOUT("CCharacterChild,m_vVel(%.2f, %.2f, %.2f)\n", m_vVel.x, m_vVel.y, m_vVel.z);
	D3DXVec3Normalize(&m_vVel,&m_vVel);
    D3DXVec3Cross(&m_vSideVel,&m_vUp,&m_vVel);        

	m_dwState = _NCITYIN;
	g_pShuttleChild->ChangeUnitState( _LANDED );
	m_bPickMove = FALSE;										// Ã³À½¿¡´Â Picking »óÅÂ ¾Æ´Ô
	m_bCharacterRender = TRUE;
	m_bBazaarEventPos = FALSE;
		
	InitEffect();
	g_pShuttleChild->InitItemPoint();									// 2006-07-21 by ispark
	g_pD3dApp->m_pSound->StopD3DSound( SOUND_GROUND_MOVING_A_GEAR );	// 2005-08-19 by ispark
	//////////////////////////////////////////////////////////////////////////
	// Ä³¸¯ÅÍ ³ôÀÌ ¼ÂÆÃ
//	D3DXVECTOR3 vPos = m_vPos;
//	vPos.y += CHARACTER_HEIGHT;

	// ÃÊ±â¿¡´Â ¼­ ÀÖ´Â ¾Ö´Ï¸ÞÀÌ¼Ç
//	g_pShuttleChild->SendFieldSocketChangeBodyCondition(g_pShuttleChild->m_myShuttleInfo.ClientIndex, BODYCON_CHARACTER_MODE_STOP);
	m_pCharacterInfo->ChangeBodyCondition(BODYCON_CHARACTER_MODE_STOP);
	// 2010-06-08 by dgwoo, Æê½Ã½ºÅÛ Ãß°¡. ¾Æ·¡ÀÇ ÇÔ¼ö·Î ÅëÇÕ. 
	ChangeWearItemBodyConditionAllProcess(BODYCON_CHARACTER_MODE_STOP);
	//g_pShuttleChild->ChangeWearItemBodyCondition(WEAR_ITEM_KIND_ATTACHMENT, BODYCON_CHARACTER_MODE_STOP);
	//g_pShuttleChild->ChangeWearItemBodyCondition(WEAR_ITEM_KIND_ACCESSORIES, BODYCON_CHARACTER_MODE_STOP);
	//g_pShuttleChild->ChangeWearItemBodyCondition(WEAR_ITEM_KIND_WINGIN, BODYCON_CHARACTER_MODE_STOP);
	// 2010-06-08 by dgwoo, Æê½Ã½ºÅÛ Ãß°¡.

	// ÃÊ±âÈ­
//	float fTempHeight = 0.0f;
//	float fObjHeight = -DEFAULT_COLLISION_DISTANCE;
//	
//	D3DXMATRIX matTemp;
//	D3DXVECTOR3 vSide(0, 0, 1), vUp(0, 1, 0);
//	D3DXMatrixLookAtLH(&matTemp,&vPos,&(vPos - vUp),&vSide);
//	fTempHeight = g_pScene->m_pObjectRender->CheckCollMesh(matTemp,vPos).fDist - CHARACTER_HEIGHT;
//	
//	if(fTempHeight<m_vPos.y)
//	m_vPos.y += -fTempHeight;

	m_vNextPos = m_vPos;
	//////////////////////////////////////////////////////////////////////////
	// CShuttleChild ÃÊ±âÈ­
	// Âø·úÀü ½ÃÁ¡ÀÌ 1ÀÎÄªÀÌ¿´À¸¸é 3ÀÎÄªÀ¸·Î º¯È¯ 
	if(g_pD3dApp->m_pCamera->GetCamType() == CAMERA_TYPE_FPS)
	{
		g_pD3dApp->m_pCamera->SetCamType(CAMERA_TYPE_NORMAL);
		g_pShuttleChild->m_bLandingOldCamtypeFPS = TRUE;
	}
	
	g_pShuttleChild->m_vPos = m_vPos;
	g_pShuttleChild->m_vVel = m_vVel;

	g_pShuttleChild->InitCharacterToShuttleData();				// 2005-11-10 by ispark

	//////////////////////////////////////////////////////////////////////////
	// ±âÅ¸
	g_pInterface->m_pGameMain->InitShowWindow();				// 2005-08-05 by ispark

	// 2008-06-20 by bhsohn EP3 ¿É¼Ç°ü·Ã Ã³¸®
	{
		//g_pInterface->m_bShowInterface = TRUE;						// 2005-08-30 by ispark	
		//BOOL bShowInter = !g_pD3dApp->IsOptionEtc(OPTION_RADIO_INTERFACE_HIDE);
		//g_pInterface->SetShowInterface(bShowInter);		
	}

	// ÇÙ °Ë»ç¸¦ À§ÇÑ ÁÂÇ¥ ÃÊ±âÈ­
	g_pD3dApp->m_vShuttleOldPos = m_vPos;


//	// 2006-02-09 by ispark, ³²ÀÚ, ¿©ÀÚ ¿òÁ÷ÀÓ ¼Óµµ ´Ù¸§
//	if(g_pShuttleChild->m_myShuttleInfo.PilotFace < 100)
//	{
//		m_fCharacterSpeed = CHARACTER_WOMAN_RUN;		
//	}
//	else
//	{
//		m_fCharacterSpeed = CHARACTER_MAN_RUN;		
//	}
	// 2006-11-02 by ispark, Ä³¸¯ÅÍ º°·Î ¿òÁ÷ÀÓÀ¸·Î º¯°æ
	m_fCharacterSpeed = GetCharacterSteps(g_pShuttleChild->m_myShuttleInfo.PilotFace);

	m_nStartEventType = EVENT_TYPE_NOEVENT;
	m_nSelBazaarType = 0;
	m_ShopEnemyClientIndex = 0;
	m_fCharacterWarpErrTime	= 0.0f;
}

void CCharacterChild::LoadCharacterEffect(CCharacterInfo** pCharacterInfo, char *strFileName)
{
	if((*pCharacterInfo))
	{
		(*pCharacterInfo)->InvalidateDeviceObjects();
		(*pCharacterInfo)->DeleteDeviceObjects();
		util::del(*pCharacterInfo);
	}
	(*pCharacterInfo) = new CCharacterInfo();
	if((*pCharacterInfo)->Load(strFileName))
	{
		(*pCharacterInfo)->InitDeviceObjects();
		(*pCharacterInfo)->RestoreDeviceObjects();
	}
	else
	{
		util::del(*pCharacterInfo);
		DBGOUT("WARNING!! [EFFECT ERROR] Can't Find Effect File : %s \n",strFileName);
	}
}

HRESULT CCharacterChild::RestoreShadow()
{
	DWORD	dwColor = 0xFFFFFFFF;
	if( FAILED( g_pD3dDev->CreateVertexBuffer( 4*sizeof( SPRITEVERTEX ),
		0, D3DFVF_SPRITEVERTEX, D3DPOOL_MANAGED, &m_pVBShadow,NULL ) ) )
	{
		g_pD3dApp->CheckError(CLIENT_ERR_SHUTTLE_EFFECTRESTORE);
		return E_FAIL;
	}
	SPRITEVERTEX* v;
	m_pVBShadow->Lock( 0, 0, (void**)&v, 0 );
	v[0].p = D3DXVECTOR3(0,0,0);  v[0].color=dwColor;	v[0].tu=0; v[0].tv=1;	
	v[1].p = D3DXVECTOR3(0,0,0);  v[1].color=dwColor;	v[1].tu=0; v[1].tv=0;	
	v[2].p = D3DXVECTOR3(0,0,0);  v[2].color=dwColor;	v[2].tu=1; v[2].tv=1;	
	v[3].p = D3DXVECTOR3(0,0,0);  v[3].color=dwColor;	v[3].tu=1; v[3].tv=0;	
	m_pVBShadow->Unlock();

	return S_OK;
}

void CCharacterChild::FineObjectTakeOff()
{
	if(m_bEnterShopState == TRUE)
		return;
	CObjectChild * pCurrentObjEvent = g_pScene->FindEventObjectByTypeAndPosition(EVENT_TYPE_TAKEOFF_PATTERN_START, m_vPos, 10240.0f);
	
	if(pCurrentObjEvent)
	{
		CObjectChild* pObj = g_pScene->FindObjectByIndex(pCurrentObjEvent->m_sEventIndexFrom);
		
		if(pObj)
		{
		    m_vPos = pObj->m_vPos;
			// ±â¾î ¸ðµå º¯È¯
			g_pD3dApp->m_bCharacter = FALSE;
			g_pInterface->m_pGameMain->InitWarp();
			g_pShuttleChild->m_vPos = m_vPos;
			g_pShuttleChild->InitShuttleData(TRUE);
			g_pShuttleChild->m_vPos.y = m_vPos.y;
			g_pShuttleChild->m_bWarpLink = TRUE;
		}
	}
	
}

void CCharacterChild::CheckState()
{
	if(g_pShuttleChild->m_dwState == _WARP)
	{
		EVENT_POINT UnitPoint, CameraPoint;
		BOOL bResult = FALSE;
//		if(g_pShuttleChild->m_pCinemaUnitPattern != NULL)
//		{
//			bResult = g_pShuttleChild->m_pCinemaUnitPattern->SkillTick();
//			UnitPoint = g_pShuttleChild->m_pCinemaUnitPattern->GetCurrentCinemaPoint();
//			m_vPos = UnitPoint.vPosition;				// À§Ä¡
//			m_vVel = UnitPoint.vDirection;				// ¹æÇâ
//			m_vUp = UnitPoint.vUpVector;
//			m_vTargetPosition = UnitPoint.vTarget;			
//		}

		if(g_pShuttleChild->m_bIsCameraPattern == TRUE)
		{
			bResult = g_pShuttleChild->m_pCinemaCamera->SkillTick();
			D3DXVECTOR3 vCameraPos, vCameraVel, vCameraUp;
			CameraPoint = g_pShuttleChild->m_pCinemaCamera->GetCurrentCinemaPoint();
			vCameraPos = CameraPoint.vPosition;
			vCameraVel = m_vPos - CameraPoint.vPosition;
			vCameraUp = CameraPoint.vUpVector;
			g_pCamera->Tick(vCameraPos,vCameraVel,vCameraUp);
			
		}
//		m_fEventCheckTime = 2.0f;
		if(bResult)	// Ä«¸Þ¶ó ÆÐÅÏÀÌ ³¡³ª´ø ¾Æ´Ï¸é À¯´Ö ÆÐÅÏÀÌ ³¡³ª¸é Á¾·áÇÑ´Ù.
		{
			switch(g_pShuttleChild->m_nEventType)
			{
			case EVENT_GAME_END:
				{
					g_pShuttleChild->SetSelectCharacterItem();
					g_pD3dApp->SendFieldSocketCharacterGameEnd(FALSE);	// 2015-09-26 Future, standardized Game closing
				}
				break;
			}
//			m_bEventReady = FALSE; WARP_OK¿¡¼­ ÇÑ´Ù.
			
		}
	}
}

void CCharacterChild::SetMatrix_Picking(D3DXVECTOR3 vPos, D3DXVECTOR3 vNorPos)
{
	// ¿ÀºêÁ§Æ® ÁÂÇ¥°¡ Z¹æÇâÀ¸·Î 90µµ È¸Àü µÇ¾î ÀÖ´Â »óÅÂÀÌ´Ù.
	// ÀÌºÎºÐÀº Åø¿¡¼­ ¹®Á¦°¡ ÀÖ´Ù.
	// ¿©±â¼­ vNorPos¸¦ 90µµ È¸Àü ½ÃÅ² °ÍÀÌ vDir(¹æÇâ) °ªÀÌ µÈ´Ù.
//	D3DXMATRIX matTemp;
//	D3DXVECTOR3 vDir;
//	D3DXMatrixRotationZ(&matTemp, D3DXToRadian(90));
//	D3DXVec3TransformCoord(&vDir, &vNorPos, &matTemp);

	// LookAtRH¿¡¼­ Up°ªÀ» xÃà ±âÁØÀ¸·Î ÁÖ¾ú´Ù.
	vPos.y += 0.3f;
	D3DXMatrixLookAtRH(&m_mPickMatrix, &vPos, &(vPos + vNorPos), &D3DXVECTOR3(1, 0, 0));
	D3DXMatrixInverse(&m_mPickMatrix, nullptr, &m_mPickMatrix );
}

BOOL CCharacterChild::CheckEnterShopNPC(POINT *pt, float fLength, BOOL nFlag)
{
	int nShopIndex = 0;
	BOOL bCheckShopObj = FALSE;

	// 2005-12-26 by ispark
	// °Å·¡ÁßÀÌ¸é »óÁ¡À» ¿­Áö ¸øÇÏ°Ô ÇÑ´Ù.
	if(g_pGameMain->m_nLeftWindowInfo == LEFT_WINDOW_TRANS)
		return FALSE;
	
	// 2005-11-11 by ispark
	// ¸®ÅÏÀÌ TRUEÀÌ¸é »óÁ¡ »ç¿ë
	if(g_pD3dApp->m_dwGameState == _SHOP)
		return TRUE;

	D3DXVECTOR2 vPos1 = D3DXVECTOR2(pt->x,pt->y);
	COLLISION_OBJ_RESULT stObject;
	CObjectChild * NPCtempObj = nullptr;
	m_pSelectObject = nullptr;

	stObject = g_pScene->m_pObjectRender->GetPickObject(pt->x, pt->y);
	
	if(stObject.pObject)
	{
		float fDistanceScene = D3DXVec3Length(&(m_vPos - stObject.stCollision.vPicking));
//		DBGOUT("ObjNum %d, Pick EventType %d(dis = %f)\n", stObject.pObject->m_nCode, stObject.pObject->m_bEventType, fDistanceScene);
		if(stObject.pObject->m_bEventType == EVENT_TYPE_OBJ_BUILDING_NPC && fDistanceScene<100.0f)
		{
//			m_nSelectObjectIndex = tempObj->m_pObjectInfo->RenderIndex; // ¼º°ø	
			m_pSelectObject = stObject.pObject; // ¼º°ø	
			
//			float fDistance = D3DXVec3Length(&(m_vPos - tempObj->m_vPos)) - tempObj->m_pObjMesh->m_fRadius;
			float fDistance = D3DXVec3Length(&(m_vPos - stObject.stCollision.vPicking));
			if( fDistance<fLength && nFlag)
			{
				nShopIndex = stObject.pObject->m_sEventIndexTo;
#ifdef CUSTOM_OPTIMIZER_HSSON
				g_cCustomOptimizer.m_nMyInfiShopIndex = stObject.pObject->m_sEventIndexTo;
#endif
			}

			bCheckShopObj = TRUE;
		}
	}

	// 2006-09-14 by ispark, µÎ¹øÂ° °Ë»ç
	NPCtempObj = g_pScene->m_pObjectRender->GetPickObjectShopNPC(pt->x, pt->y);
	if(NPCtempObj && bCheckShopObj == FALSE)
	{
		
		float fDistanceScene = D3DXVec3Length(&(m_vPos - NPCtempObj->m_vPos));
		if(NPCtempObj->m_bEventType == EVENT_TYPE_OBJ_BUILDING_NPC && fDistanceScene<10.0f)	// 2012-10-10 by mspark, ´Ù¸¥ ÃþÀÇ °Ç¹° Å¬¸¯µÇ´Â ¹®Á¦ ¼öÁ¤ - ±âÁ¸ 100.0f¿¡¼­ 10.0f·Î º¯°æ
		{
			m_pSelectObject = NPCtempObj; // ¼º°ø	
			
			float fDistance = D3DXVec3Length(&(m_vPos - stObject.stCollision.vPicking));
			if( fDistance<fLength && nFlag)
			{
				nShopIndex = NPCtempObj->m_sEventIndexTo;
			}
		}
	}

	// 2008-03-27 by bhsohn ¹Ì¼Ç¼º°øÃ¢ÀÌ ¶°ÀÖ´Â »óÅÂ¿¡¼­´Â »óÁ¡ÀÌ ¾È¿­¸®°Ô º¯°æ
	if(nShopIndex && !g_pD3dApp->IsPossibleShopOpen())
	{
		return TRUE;
	}
	// end 2008-03-27 by bhsohn ¹Ì¼Ç¼º°øÃ¢ÀÌ ¶°ÀÖ´Â »óÅÂ¿¡¼­´Â »óÁ¡ÀÌ ¾È¿­¸®°Ô º¯°æ


	// 2010. 03. 03 by ckPark ÀÎÇÇ´ÏÆ¼ ÇÊµå 2Â÷(ÀÔÀå UIº¯°æ)
	if( nShopIndex )
	{
		BUILDINGNPC* pBuilding = g_pDatabase->GetServerBuildingNPCInfo( nShopIndex );
		if( pBuilding )
		{
			// ÀÎÇÇ´ÏÆ¼ ÆË¾÷Ã¢ÀÌ ¶°ÀÖÀ» °æ¿ì ÀÎÇÇ´ÏÆ¼ »óÁ¡Å¬¸¯ÀÌ Áßº¹µÇÁö ¾Êµµ·Ï ÇÑ´Ù
			if( pBuilding->BuildingKind == BUILDINGKIND_INFINITY )
			{
				if( g_pD3dApp->m_pInterface->m_pInfinityPopup 
					&& g_pD3dApp->m_pInterface->m_pInfinityPopup->IsShowWnd() )
				{
					return TRUE;
				}

				if( g_pD3dApp->m_pInterface->IsArenaGameState( ARENA_GAME_STATE_TEAMINFO ) )
				{
					return TRUE;
				}
			}

			if( pBuilding->BuildingKind == BUILDINGKIND_ARENA )
			{
				if( g_pD3dApp->m_pInterface->m_pInfinityPopup 
					&& g_pD3dApp->m_pInterface->m_pInfinityPopup->IsShowWnd() )
				{
					return TRUE;
				}
			}
		}
	}
	// end 2010. 03. 03 by ckPark ÀÎÇÇ´ÏÆ¼ ÇÊµå 2Â÷(ÀÔÀå UIº¯°æ)

	// 2013-03-21 by bhsohn Ã¢°í ÁÖÀ§¿¡ °³ÀÎ»óÁ¡ Ä³¸¯ÅÍ ¼±ÅÃ ½Ã °³ÀÎ»óÁ¡ÀÌ ¿¬°áµÇÁö ¾Ê°í Ã¢°í°¡ ¿ÀÇÂµÇ´Â Çö»ó.
#ifdef FREEWAR_
	if (!IsInfluenceCharacter(g_pShuttleChild->m_myShuttleInfo.InfluenceType, INFLUENCE_TYPE_NORMAL, 0, g_pShuttleChild->m_myShuttleInfo.MapChannelIndex.MapIndex))
#else
	if(!IsInfluenceCharacter(g_pShuttleChild->m_myShuttleInfo.InfluenceType,INFLUENCE_TYPE_NORMAL,0))
#endif
	{			
		//ÇöÀç ÀÚ½ÅÀÇ ¼¼·ÂÀÌ ÀÏ¹Ý±ºÀÌ¸é °³ÀÎ»óÁ¡À» ºñÈ°¼ºÈ­ ÇÑ´Ù.
		// Ä³¸¯ÅÍ Ã¼Å©
		CEnemyData* pEnemy = g_pScene->GetEnemyCharaterID(vPos1);
		m_pSelBazaarShop = nullptr;
		if(pEnemy)
		{
			float fDistanceScene = D3DXVec3Length(&(m_vPos - pEnemy->m_vPos));
			
			if(fDistanceScene <= REQUERED_BAZAAR_DISTANCE)
			{
				return g_pInterface->SendBazaarRequestItemList(pEnemy, nFlag);
			}
		}
	}
	// END 2013-03-21 by bhsohn Ã¢°í ÁÖÀ§¿¡ °³ÀÎ»óÁ¡ Ä³¸¯ÅÍ ¼±ÅÃ ½Ã °³ÀÎ»óÁ¡ÀÌ ¿¬°áµÇÁö ¾Ê°í Ã¢°í°¡ ¿ÀÇÂµÇ´Â Çö»ó.


	// 2006-09-14 by ispark, »óÁ¡ ¸Þ¼¼Áö
//*--------------------------------------------------------------------------*//
	if(nShopIndex)
	{
		// 2011-07-22 by hsson ˜?Áßº¹ Á¢¼ÓÀ¸·Î ÀÎÃ¦Æ® µÇ´ø ¹ö±× ¼öÁ¤
		// ¾ÆÀÌÅÛÀ» »ç¿ëÇØ¼­ »óÁ¡À» ¿­¾úÀ¸¸é ÀÏÁ¤½Ã°£ »óÁ¡À» ¸ø ¿­°Ô ÇÑ´Ù.
		if( g_pD3dApp->m_pInterface->m_pGameMain->m_pInven->m_bShopConcurrent + CINFInvenExtend::SHOP_CONCURRENT_DELAY < timeGetTime() )
		{
			g_pD3dApp->m_pInterface->m_pGameMain->m_pInven->m_bShopConcurrent = timeGetTime();

			m_pShopObject = stObject.pObject;
			
			MSG_FC_EVENT_CHARACTERMODE_ENTER_BUILDING sMsg;
			sMsg.nBuildingIndex0 = nShopIndex;
			//		sMsg.SendShopItemList = FALSE;
			g_pFieldWinSocket->SendMsg( T_FC_EVENT_CHARACTERMODE_ENTER_BUILDING, (char*)&sMsg, sizeof(sMsg) );				
			g_pD3dApp->m_bRequestEnable = FALSE;			// 20 06-06-21 by ispark, ¸Þ¼¼Áö ÀÀ´äÀ» ±â´Ù¸°´Ù.
			return TRUE;
		}
		// end 2011-07-22 by hsson ˜?Áßº¹ Á¢¼ÓÀ¸·Î ÀÎÃ¦Æ® µÇ´ø ¹ö±× ¼öÁ¤
	}
	
//*--------------------------------------------------------------------------*//

	// 2013-03-21 by bhsohn Ã¢°í ÁÖÀ§¿¡ °³ÀÎ»óÁ¡ Ä³¸¯ÅÍ ¼±ÅÃ ½Ã °³ÀÎ»óÁ¡ÀÌ ¿¬°áµÇÁö ¾Ê°í Ã¢°í°¡ ¿ÀÇÂµÇ´Â Çö»ó.
	// 2006-08-07 by dgwoo ÇöÀç ÀÚ½ÅÀÇ ¼¼·ÂÀÌ ÀÏ¹Ý±ºÀÌ¸é °³ÀÎ»óÁ¡À» ºñÈ°¼ºÈ­ ÇÑ´Ù.
// 	if(IsInfluenceCharacter(g_pShuttleChild->m_myShuttleInfo.InfluenceType,INFLUENCE_TYPE_NORMAL,0))
// 		return FALSE;
// 
// 	// 2006-07-29 by ispark, Ä³¸¯ÅÍ Ã¼Å©
// 	CEnemyData* pEnemy = g_pScene->GetEnemyCharaterID(vPos1);
// 	m_pSelBazaarShop = NULL;
// 	if(pEnemy)
// 	{
// 		float fDistanceScene = D3DXVec3Length(&(m_vPos - pEnemy->m_vPos));
// 
// 		if(fDistanceScene <= REQUERED_BAZAAR_DISTANCE)
// 		{
// 			return g_pInterface->SendBazaarRequestItemList(pEnemy, nFlag);
// 		}
// 	}
	// END 2013-03-21 by bhsohn Ã¢°í ÁÖÀ§¿¡ °³ÀÎ»óÁ¡ Ä³¸¯ÅÍ ¼±ÅÃ ½Ã °³ÀÎ»óÁ¡ÀÌ ¿¬°áµÇÁö ¾Ê°í Ã¢°í°¡ ¿ÀÇÂµÇ´Â Çö»ó.

	return FALSE;
}

CObjectChild *CCharacterChild::GetEventObject(float fDistance)
{
	CObjectChild * pObjectRes = nullptr;
	vectorCObjectChildPtr::iterator itObj = g_pScene->m_vectorRangeObjectPtrList.begin();
	while(itObj != g_pScene->m_vectorRangeObjectPtrList.end())
	{
		CObjectChild * pObject = *itObj;
		if(pObject->m_bEventType == EVENT_TYPE_OBJ_BUILDING_NPC)
		{
			float fLengthTemp = D3DXVec3Length(&(m_vPos - pObject->m_vPos));
			if(fLengthTemp > fDistance)
			{
				itObj++;
				continue;
			}
			
			if(pObjectRes == nullptr)
			{
				pObjectRes = pObject;
			}

			float fLengthTemp1 = D3DXVec3Length(&(m_vPos - pObject->m_vPos));
			float fLengthTemp2 = D3DXVec3Length(&(m_vPos - pObjectRes->m_vPos));
			
			if(fLengthTemp2>fLengthTemp1)
			{
				pObjectRes = pObject;
			}
			
		}
		itObj++;
	}

	return pObjectRes;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			CCharacterChild::CameraMoveTick()
/// \brief		Ä³¸¯ÅÍ ¸¶¿ì½º ¿òÁ÷ÀÓ Tick()
/// \author		ispark
/// \date		2005-08-16 ~ 2005-08-16
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CCharacterChild::CameraMoveTick()
{
	float fDirY = 0.0f, fDirX = 0.0f;
	POINT pt;

	m_bCharacterRender = TRUE;

	GetCursorPos(&pt);
	ScreenToClient(g_pD3dApp->GetHwnd(), &pt);

	if(m_bRButtonState && m_bMouseMove)
	{
//		CheckMouseReverse(&pt);
		// ÀÏ¹Ý, ¿Ü°û¿¡¼­ ¸¶¿ì½º ¿òÁ÷ÀÓÀÏ ¶§
		if(pt.y <= 0)
			fDirY = 10.0f;
		else if(pt.y >= g_pD3dApp->GetBackBufferDesc().Height-1)
			fDirY = -10.0f;
		else
			fDirY = m_pOldMousePoint.y - pt.y;
		
		if(pt.x <= 0)
			fDirX = -10.0f;
		else if(pt.x >= g_pD3dApp->GetBackBufferDesc().Width-1)
			fDirX = 10.0f;
		else
			fDirX = pt.x - m_pOldMousePoint.x;
		
		m_pOldMousePoint = pt;
	}
	else if(m_bRButtonState)
	{
		// ¿Ü°ûÃ³¸® ¹®Á¦ ¿À¸¥ÂÊ ¹öÆ°´­·¶À» ¶§¸¸ Ã³¸®, ¸¶¿ì½º ¿òÁ÷ÀÓÀÌ ¾øÀ½
		if(pt.y <= 0)
			fDirY = 10.0f;
		else if(pt.y >= g_pD3dApp->GetBackBufferDesc().Height-1)
			fDirY = -10.0f;

		if(pt.x <= 0)
			fDirX = -10.0f;
		else if(pt.x >= g_pD3dApp->GetBackBufferDesc().Width-1)
			fDirX = 10.0f;
	}
	
	g_pCamera->SetAngleX(fDirX);
	g_pCamera->SetAngleY(fDirY);

	// ÀÌº¥Æ® ¿ÀºêÁ§Æ®¿À ÀÎÇØ Ä«¸Þ¶ó°¡ ¿òÁ÷ÀÌÁö¸¸ È¸ÀüÀ» ÇÏ¸ç ¾ÈµÅ´Â »óÈ²
	BOOL bWarp = (m_nStartEventType != EVENT_TYPE_NOEVENT) ? TRUE : FALSE;

	if(m_bPickMove == TRUE || m_bMouseWheel == TRUE || bWarp)
		g_pCamera->SetCamMove(m_bRButtonState, m_bMouseWheel, bWarp);

	// Ä«¸Þ¶ó °Å¸®°¡ ÃÖ¼ÒÇã¿ëÄ¡º¸´Ù ÀÛÀ¸¸é Ä³¸¯ÅÍ ·»´õ¸µ ¾ÈÇÔ
	float fCollDist = g_pCamera->GetCollDistance();
	if(fCollDist != 0 && 
		fCollDist < CHARACTER_CAMERA_DISTANCE_MIN)
		m_bCharacterRender = FALSE;

	m_bMouseMove = FALSE;
	m_bMouseWheel = FALSE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CCharacterChild::CheckEventTypeObject(float fElapsedTime)
/// \brief		Ä³¸¯ÅÍ Àü¿ë ÀÌº¥Æ® ¿ÀºêÁ§Æ® Ã¼Å©
/// \author		ispark
/// \date		2006-07-14 ~ 2006-07-14
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CCharacterChild::CheckEventTypeObject(float fElapsedTime)
{
	if(g_pScene->m_pObjectRender)
	{
		CObjectChild* pCurrentObjEvent = (CObjectChild *)g_pScene->m_pObjectRender->CheckCollMeshEvent(40.0f);
		
		if(!pCurrentObjEvent)
		{
			return;
		}

		// ½ÃÀÛÀÌ¶ó¸é
		if(m_dwState == _NCITYIN)
		{
			m_bStartPositionEvent = TRUE;
			return;
		}

		// Å¸ÀÓ Ã¼Å©
		// ¿¡·¯ Å¸ÀÓ(10ÃÊ)À» ÃÊ°ú Çß´ÂÁö
		m_fCharacterWarpErrTime -= fElapsedTime;
		if(m_fCharacterWarpErrTime > 0.0f)
		{
			return;
		}
		else
		{
			m_fCharacterWarpErrTime = 0.0f;
		}

		float fEventLength = D3DXVec3Length(&D3DXVECTOR3(pCurrentObjEvent->m_vPos - m_vPos));
		
		switch(pCurrentObjEvent->m_bEventType)
		{
		case EVENT_TYPE_CHARACTERMODE_WARP:
			{
				if(m_nStartEventType == EVENT_TYPE_CHARACTERMODE_WARP_TARGET)
				{
					// ¿öÇÁ ½ÃÀÛ
					// ÀÌº¥Æ®°¡ Æ¯Á¤ °Å¸® ¾È¿¡ µå´ÂÁö
					if(fEventLength > 15.0f)
					{
						break;
					}

//					m_dwState = _STAND;
					m_bPickMove = FALSE;

					m_vMoveVel = D3DXVECTOR3(0, 0, 0);
					m_fCharacterWarpErrTime = 10.0f;
					SendRequestEventObjectWarpIn(pCurrentObjEvent);
					m_nStartEventType = EVENT_TYPE_NOEVENT;
				}
				else if(m_nStartEventType == EVENT_TYPE_NOEVENT)
				{
					// Å¸°Ù ÀÌº¥Æ® ¿ÀºêÁ§Æ® ¹æÇâÀ» Ã£´Â´Ù.
					SetMoveVelByEventObject(EVENT_TYPE_CHARACTERMODE_WARP_TARGET, 500.0f);
					m_nStartEventType = EVENT_TYPE_CHARACTERMODE_WARP;
					m_dwState = _WARP;

					m_pCharacterInfo->ChangeBodyCondition(BODYCON_CHARACTER_MODE_STOP);
					// 2010-06-08 by dgwoo, Æê½Ã½ºÅÛ Ãß°¡. ¾Æ·¡ÀÇ ÇÔ¼ö·Î ÅëÇÕ. 
					ChangeWearItemBodyConditionAllProcess(BODYCON_CHARACTER_MODE_STOP);
// 					g_pShuttleChild->ChangeWearItemBodyCondition(WEAR_ITEM_KIND_ATTACHMENT, BODYCON_CHARACTER_MODE_STOP);
// 					g_pShuttleChild->ChangeWearItemBodyCondition(WEAR_ITEM_KIND_ACCESSORIES, BODYCON_CHARACTER_MODE_STOP);
// 					g_pShuttleChild->ChangeWearItemBodyCondition(WEAR_ITEM_KIND_WINGIN, BODYCON_CHARACTER_MODE_STOP);
					// 2010-06-08 by dgwoo, Æê½Ã½ºÅÛ Ãß°¡.
				}
			}
			break;
		case EVENT_TYPE_CHARACTERMODE_WARP_TARGET:
			{
				if(m_nStartEventType == EVENT_TYPE_CHARACTERMODE_WARP)
				{
					// ¿öÇÁ ³¡
					// ÀÌº¥Æ® ¿ÀºêÁ§Æ® ³ôÀÌº¸´Ù ³ô¾Æ¾ß ÇÑ´Ù.
					if(pCurrentObjEvent->m_vPos.y > m_vPos.y)
					{
						break;
					}

					m_dwState = _STAND;
					m_nStartEventType = -1;								// ÀÌº¥Æ® Å¸ÀÔ ¹«½Ã
				}
				else if(m_nStartEventType == EVENT_TYPE_NOEVENT)
				{
					// ÀÌº¥Æ®°¡ Æ¯Á¤ °Å¸® ¾È¿¡ µå´ÂÁö
					if(fEventLength > 10.0f)
					{
						break;
					}

					// ¿öÇÁ ÀÌº¥Æ® ¿ÀºêÁ§Æ® ¹æÇâÀ» Ã£´Â´Ù.
					SetMoveVelByEventObject(EVENT_TYPE_CHARACTERMODE_WARP, 500.0f);
					m_nStartEventType = EVENT_TYPE_CHARACTERMODE_WARP_TARGET;
					m_dwState = _WARP;
					m_bPickMove = FALSE;

					m_pCharacterInfo->ChangeBodyCondition(BODYCON_CHARACTER_MODE_STOP);
					// 2010-06-08 by dgwoo, Æê½Ã½ºÅÛ Ãß°¡. ¾Æ·¡ÀÇ ÇÔ¼ö·Î ÅëÇÕ. 
					ChangeWearItemBodyConditionAllProcess(BODYCON_CHARACTER_MODE_STOP);
					//g_pShuttleChild->ChangeWearItemBodyCondition(WEAR_ITEM_KIND_ATTACHMENT, BODYCON_CHARACTER_MODE_STOP);
					//g_pShuttleChild->ChangeWearItemBodyCondition(WEAR_ITEM_KIND_ACCESSORIES, BODYCON_CHARACTER_MODE_STOP);
					//g_pShuttleChild->ChangeWearItemBodyCondition(WEAR_ITEM_KIND_WINGIN, BODYCON_CHARACTER_MODE_STOP);
					// 2010-06-08 by dgwoo, Æê½Ã½ºÅÛ Ãß°¡.
				}
				else if(m_nStartEventType == -1)
				{
					// ¿öÇÁ¸¦ ÇÑ ÈÄ ÀÏÁ¤ ¹üÀ§¸¦ ³Ñ¾î°¡±â Àü±îÁö´Â ÀÌº¥Æ® ¿ÀºêÁ§Æ®¸¦ ¹«½ÃÇÑ´Ù.
					float fEventLength = D3DXVec3Length(&D3DXVECTOR3(pCurrentObjEvent->m_vPos - m_vPos));
					if(fEventLength > 30.0f)
					{
						m_nStartEventType = EVENT_TYPE_NOEVENT;
					}
				}
			}
			break;
			// 2007-12-14 by dgwoo Ä³¸¯ÅÍ ¸ðµå¿¡¼­ ¹Ù·Î ¿öÇÁ Å»¼öÀÖµµ·Ï ÀÌº¥Æ® Ãß°¡.
		case EVENT_TYPE_CHARACTERMODE_DIRECTLY_WARP:
			{
				m_bPickMove = FALSE;
				m_vMoveVel = D3DXVECTOR3(0, 0, 0);
				m_fCharacterWarpErrTime = 10.0f;
				SendRequestEventObjectWarpIn(pCurrentObjEvent);
				m_nStartEventType = EVENT_TYPE_NOEVENT;
			}
			break;
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CCharacterChild::SendRequestEventObjectWarpIn(CObjectChild *pObj)
/// \brief		Ä³¸¯ÅÍ ¿öÇÁ ÀÎ
/// \author		ispark
/// \date		2006-07-19 ~ 2006-07-19
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CCharacterChild::SendRequestEventObjectWarpIn(CObjectChild *pObj)
{
	MSG_FC_EVENT_REQUEST_OBJECT_EVENT sMsg;
	sMsg.ObjectType = pObj->m_nCode;
	sMsg.ObjectPosition = pObj->m_vPos;
	g_pFieldWinSocket->SendMsg( T_FC_EVENT_REQUEST_OBJECT_EVENT, (char*)&sMsg, sizeof(sMsg) );
	DBGOUT("Character Request Warp : MSG_FC_EVENT_REQUEST_OBJECT_EVENT(%d)\n",pObj->m_nCode);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			D3DXVECTOR3 CCharacterChild::SetMoveVelByEventObject(BYTE bObjectType, float fDist)
/// \brief		Æ¯Á¤ ÀÌº¥Æ® ¿ÀºêÁ§Æ® ¹æÇâÀ» Á¤ÇÑ´Ù.
/// \author		ispark
/// \date		2006-07-20 ~ 2006-07-20
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
D3DXVECTOR3 CCharacterChild::SetMoveVelByEventObject(BYTE bObjectType, float fDist)
{
	// ÀÌº¥Æ® ¿ÀºêÁ§Æ®¸¦ Ã£´Â´Ù.
	CObjectChild * pNextTargetObjEvent = g_pScene->FindEventObjectByTypeAndPosition(bObjectType, m_vPos, fDist);
	// ÀÌµ¿ ¹æÇâÀ» °áÁ¤ÇÑ´Ù.
	m_vMoveVel = pNextTargetObjEvent->m_vPos - m_vPos;
	m_vMoveVel.x = m_vMoveVel.z = 0.0f;
	D3DXVec3Normalize(&m_vMoveVel, &m_vMoveVel);

	m_vMoveVel.y *= CHARACTER_MAN_RUN;
	return m_vMoveVel;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CCharacterChild::DeleteChatMoveShop()
/// \brief		
/// \author		ispark
/// \date		2006-09-04 ~ 2006-09-04
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CCharacterChild::DeleteChatMoveShop()
{
	if(m_pMoveChatShop)
	{
		m_pMoveChatShop->InvalidateDeviceObjects();
		m_pMoveChatShop->DeleteDeviceObjects();
		util::del(m_pMoveChatShop);
	}
}
// 2010-06-08 by dgwoo, Æê½Ã½ºÅÛ Ãß°¡. ¾Æ·¡ÀÇ ÇÔ¼ö·Î ÅëÇÕ. 
void CCharacterChild::ChangeWearItemBodyConditionAllProcess(BodyCond_t i_nBodyCondition)
{
	g_pShuttleChild->ChangeWearItemBodyCondition(WEAR_ITEM_KIND_ACCESSORY_UNLIMITED, i_nBodyCondition);
	g_pShuttleChild->ChangeWearItemBodyCondition(WEAR_ITEM_KIND_ACCESSORY_TIME_LIMIT, i_nBodyCondition);
	g_pShuttleChild->ChangeWearItemBodyCondition(WEAR_ITEM_KIND_WINGIN, i_nBodyCondition);
	g_pShuttleChild->ChangeWearItemBodyCondition(WEAR_ITEM_KIND_PET, i_nBodyCondition);
}
// 2010-06-08 by dgwoo, Æê½Ã½ºÅÛ Ãß°¡. ¾Æ·¡ÀÇ ÇÔ¼ö·Î ÅëÇÕ. 