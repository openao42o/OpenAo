// INFArenaCreate.cpp: implementation of the CINFArenaCreate class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "AtumApplication.h"
#include "INFArenaCreate.h"
#include "INFCitySupply.h"
#include "INFCityBase.h"
#include "INFScrollbar.h"
#include "INFImage.h"
#include "D3dHanFont.h"
#include "ItemInfo.h"
#include "ShuttleChild.h"
#include "CharacterChild.h"				// 2005-07-21 by ispark
#include "FieldWinSocket.h"
#include "GameDataLast.h"
#include "StoreData.h"
#include "WeaponItemInfo.h"
#include "INFWindow.h"
#include "INFGameMain.h"
#include "INFItemInfo.h"
#include "RangeTime.h"
#include "INFInven.h"
#include "INFIcon.h"
#include "AtumDatabase.h"
#include "dxutil.h"
#include "Chat.h"
#include "Interface.h"
#include "INFGameArena.h"

#include "INFImageEx.h" 								// 2011. 10. 10 by jskim UI시스템 변경
#include "INFGroupImage.h"
#include "INFGroupManager.h"


#define MAX_PASSWORD									4

#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경

// 2012-05-29 by mspark, 아레나 통합(전달)
#ifdef C_ARENA_EX_1ST_MSPARK
#define ARENA_CREATE_BUTTON_X							158		// 2012-04-13 by mspark, 아레나 UI 작업 - 기존 95에서 158로 수정
#define ARENA_CREATE_BUTTON_Y							270		// 2012-04-13 by mspark, 아레나 UI 작업 - 기존 113에서 270으로 수정

#define ARENA_CANCEL_BUTTON_X							158		// 2012-04-13 by mspark, 아레나 UI 작업 - 기존 150에서 158로 수정
#define ARENA_CANCEL_BUTTON_Y							ARENA_CREATE_BUTTON_Y

#define ARENA_BASE_BUTTON_W								38 
#define ARENA_BASE_BUTTON_H								17

#define ARENA_CREATE_COUNT_X							180		// 2012-04-13 by mspark, 아레나 UI 작업 - 기존 91에서 180으로 수정
#define ARENA_CREATE_COUNT_Y							54		// 2012-04-13 by mspark, 아레나 UI 작업 - 기존 37에서 54로 수정
#define ARENA_CREATE_COUNT_W							110		// 2012-03-29 by mspark, 아레나 생성창 인원 콤보박스 버튼 클릭 가능 범위 수정 - 기존 93에서 110으로 변경
#define ARENA_CREATE_COUNT_H							15
#define ARENA_CREATE_COUNT_COMBO_X						200
#define ARENA_CREATE_COUNT_COMBO_Y						55
#define ARENA_CREATE_COUNT_FONT_X						237		// 2012-04-13 by mspark, 아레나 UI 작업 - 기존 146에서 237으로 수정
#define ARENA_CREATE_COUNT_FONT_Y						53		// 2012-04-13 by mspark, 아레나 UI 작업 - 기존 36에서 53으로 수정

#define ARENA_CREATE_MODE_X								180		// 2012-04-13 by mspark, 아레나 UI 작업 - 기존 91에서 180으로 수정
#define ARENA_CREATE_MODE_Y								82		// 2012-04-13 by mspark, 아레나 UI 작업 - 기존 59에서 81로 수정
#define ARENA_CREATE_MODE_W								156
#define ARENA_CREATE_MODE_H								15
#define ARENA_CREATE_MODE_COMBO_X						226
#define ARENA_CREATE_MODE_COMBO_Y						83
#define ARENA_CREATE_MODE_FONT_X						235		// 2012-04-13 by mspark, 아레나 UI 작업 - 기존 144에서 235로 수정
#define ARENA_CREATE_MODE_FONT_Y						83		// 2012-04-13 by mspark, 아레나 UI 작업 - 기존 61에서 83로 수정

#define ARENA_CREATE_PASS_X								181		// 2012-04-13 by mspark, 아레나 UI 작업 - 기존 89에서 181으로 수정
#define ARENA_CREATE_PASS_Y								110		// 2012-04-13 by mspark, 아레나 UI 작업 - 기존 80에서 110으로 수정
#define ARENA_CREATE_PASS_W								82		// 2012-04-13 by mspark, 아레나 UI 작업 - 기존 85에서 82로 수정
#define ARENA_CREATE_PASS_H								15
#define ARENA_CREATE_PASS_FONT_X						183		// 2012-04-13 by mspark, 아레나 UI 작업 - 기존 91에서 183으로 수정
#define ARENA_CREATE_PASS_FONT_Y						109		// 2012-04-13 by mspark, 아레나 UI 작업 - 기존 81에서 109로 수정

// 2012-04-13 by mspark, 아레나 UI 작업
#define ARENA_CREATE_MAP_IMAGE_X						60
#define ARENA_CREATE_MAP_IMAGE_Y						175
#define ARENA_CREATE_MAP_IMAGE_W						86
#define ARENA_CREATE_MAP_IMAGE_H						62
#define ARENA_CREATE_MAP_IMAGE_TAB						22

#define ARENA_MAP_COUNT									2

#define ARENA_MAP_01_NUMBER				9101
#define ARENA_MAP_02_NUMBER				9102
#define ARENA_MAP_RANDOM_NUMBER			9100

#define ARENA_MAP_01_INDEX				0
#define ARENA_MAP_02_INDEX				1
#define ARENA_MAP_RANDOM_INDEX			2
// end 2012-04-13 by mspark, 아레나 UI 작업

#else		

#define ARENA_CREATE_BUTTON_X							95
#define ARENA_CREATE_BUTTON_Y							113

#define ARENA_CANCEL_BUTTON_X							150
#define ARENA_CANCEL_BUTTON_Y							ARENA_CREATE_BUTTON_Y

#define ARENA_BASE_BUTTON_W								38 
#define ARENA_BASE_BUTTON_H								17

#define ARENA_CREATE_COUNT_X							91	
#define ARENA_CREATE_COUNT_Y							37
#define ARENA_CREATE_COUNT_W							93
#define ARENA_CREATE_COUNT_H							15
#define ARENA_CREATE_COUNT_COMBO_X						200
#define ARENA_CREATE_COUNT_COMBO_Y						55
#define ARENA_CREATE_COUNT_FONT_X						146
#define ARENA_CREATE_COUNT_FONT_Y						36

#define ARENA_CREATE_MODE_X								91
#define ARENA_CREATE_MODE_Y								59
#define ARENA_CREATE_MODE_W								156
#define ARENA_CREATE_MODE_H								15
#define ARENA_CREATE_MODE_COMBO_X						226
#define ARENA_CREATE_MODE_COMBO_Y						83
#define ARENA_CREATE_MODE_FONT_X						144
#define ARENA_CREATE_MODE_FONT_Y						61

#define ARENA_CREATE_PASS_X								89
#define ARENA_CREATE_PASS_Y								80
#define ARENA_CREATE_PASS_W								85
#define ARENA_CREATE_PASS_H								15
#define ARENA_CREATE_PASS_FONT_X						91
#define ARENA_CREATE_PASS_FONT_Y						81

#define ARENA_MAP_01_NUMBER								9101	// 2012-04-13 by mspark, 아레나 UI 작업

#endif
// end 2012-05-29 by mspark, 아레나 통합(전달)

#else		
#define ARENA_CREATE_BUTTON_X							136
#define ARENA_CREATE_BUTTON_Y							168

#define ARENA_CANCEL_BUTTON_X							179
#define ARENA_CANCEL_BUTTON_Y							ARENA_CREATE_BUTTON_Y

#define ARENA_BASE_BUTTON_W								38 
#define ARENA_BASE_BUTTON_H								17

#define ARENA_CREATE_COUNT_X							100	
#define ARENA_CREATE_COUNT_Y							54
#define ARENA_CREATE_COUNT_W							32
#define ARENA_CREATE_COUNT_H							15
#define ARENA_CREATE_COUNT_COMBO_X						119
#define ARENA_CREATE_COUNT_COMBO_Y						55
#define ARENA_CREATE_COUNT_FONT_X						117
#define ARENA_CREATE_COUNT_FONT_Y						53

#define ARENA_CREATE_MODE_X								100
#define ARENA_CREATE_MODE_Y								82
#define ARENA_CREATE_MODE_W								102
#define ARENA_CREATE_MODE_H								15
#define ARENA_CREATE_MODE_COMBO_X						188
#define ARENA_CREATE_MODE_COMBO_Y						83
#define ARENA_CREATE_MODE_FONT_X						147
#define ARENA_CREATE_MODE_FONT_Y						81

#define ARENA_CREATE_PASS_X								100
#define ARENA_CREATE_PASS_Y								110
#define ARENA_CREATE_PASS_W								85
#define ARENA_CREATE_PASS_H								15
#define ARENA_CREATE_PASS_FONT_X						106
#define ARENA_CREATE_PASS_FONT_Y						110

#define ARENA_MAP_01_NUMBER								9101	// 2012-04-13 by mspark, 아레나 UI 작업

#endif



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CINFArenaCreate::CINFArenaCreate(CAtumNode* pParent, BUILDINGNPC* pBuilding)
{
	m_pParent = pParent;	// CGameMain*
	m_pGameData = NULL;
	m_pBuildingInfo = pBuilding;

	m_bShow = FALSE;
	m_nStateCancelB = BUTTON_STATE_NORMAL;
	m_nStateCreateB = BUTTON_STATE_NORMAL;
//	m_nStateCbArrCount = BUTTON_STATE_NORMAL;
//	m_nStateCbArrMode = BUTTON_STATE_NORMAL;
	m_ntempMode = -1;

	m_nStartX = m_nStartY = 0;

	// 2012-04-13 by mspark, 아레나 UI 작업
	m_pImgArenaMap01 = NULL;
	m_pImgArenaMap02 = NULL;
	m_pImgArenaMapRandom = NULL;
	m_pImgArenaMapSelect = NULL;
	m_pOkCancelImage = NULL;
	m_pImgFadeBG = NULL;

	m_nOkImgMinPos.x = 0;
	m_nOkImgMinPos.y = 0;
	m_nCancelImgMinPos.x = 0;
	m_nCancelImgMinPos.y = 0;
	m_nOkCancelBaseImgMaxPos.x = 0;
	m_nOkCancelBaseImgMaxPos.y = 0;

	m_nArenaMapNum = 0;
	m_nArenaMapImgTabNum = 0;
	// end 2012-04-13 by mspark, 아레나 UI 작업
}
CINFArenaCreate::CINFArenaCreate()
{
	m_bShow = FALSE;
	m_nStateCancelB = BUTTON_STATE_NORMAL;
	m_nStateCreateB = BUTTON_STATE_NORMAL;
//	m_nStateCbArrCount = BUTTON_STATE_NORMAL;
//	m_nStateCbArrMode = BUTTON_STATE_NORMAL;

	m_nStartX = m_nStartY = 0;
}

CINFArenaCreate::~CINFArenaCreate()
{

}
HRESULT CINFArenaCreate::InitDeviceObjects()
{
	int i;
	DataHeader	* pDataHeader;
	char buf[64];
	for(i = 0 ; i < BUTTON_STATE_NUMBER ; i++)
	{
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경

// 2012-05-29 by mspark, 아레나 통합(전달)
#ifdef C_ARENA_EX_1ST_MSPARK
		wsprintf(buf,"okb0%d",i);		// 2012-04-13 by mspark, 아레나 UI 작업
#else				 
		wsprintf(buf,"oks0%d",i);
#endif
// end 2012-05-29 by mspark, 아레나 통합(전달)

#else				 
		wsprintf(buf,"scre%d",i);
#endif

		pDataHeader = FindResource(buf);
		m_pImgCreateB[i] = new CINFImageEx;
		m_pImgCreateB[i]->InitDeviceObjects( pDataHeader );

//		wsprintf(buf,"comarr%d",i);
//		pDataHeader = FindResource(buf);
//		m_pImgCbArr[i] = new CINFImage;
//		m_pImgCbArr[i]->InitDeviceObjects( pDataHeader->m_pData, pDataHeader->m_DataSize );
//
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경

// 2012-05-29 by mspark, 아레나 통합(전달)
#ifdef C_ARENA_EX_1ST_MSPARK
		wsprintf(buf,"canb0%d",i);		// 2012-04-13 by mspark, 아레나 UI 작업
#else			   
		wsprintf(buf,"cans0%d",i);
#endif
// end 2012-05-29 by mspark, 아레나 통합(전달)

#else			   
		wsprintf(buf,"shmcan0%d",i);
#endif

		pDataHeader = FindResource(buf);
		m_pImgCancelB[i] = new CINFImageEx;
		m_pImgCancelB[i]->InitDeviceObjects( pDataHeader );

	}

	m_pFontCount = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9, D3DFONT_ZENABLE, FALSE,128,32);
	m_pFontCount->InitDeviceObjects(g_pD3dDev);
	m_pFontMode = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9, D3DFONT_ZENABLE, FALSE,128,32);
	m_pFontMode->InitDeviceObjects(g_pD3dDev);
	m_pFontPass = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9, D3DFONT_ZENABLE, FALSE,128,32);
	m_pFontPass->InitDeviceObjects(g_pD3dDev);

	pDataHeader = FindResource("arecreate");
	m_pImgAreCreate = new CINFImageEx;
	m_pImgAreCreate->InitDeviceObjects( pDataHeader );

#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
     pDataHeader = FindResource("c_ccbk");								 
#else
	pDataHeader = FindResource("cbcount");
#endif
	


	m_pImgCbCount = new CINFImageEx;
	m_pImgCbCount->InitDeviceObjects( pDataHeader );

	pDataHeader = FindResource("cbamode");
	m_pImgCbAmode = new CINFImageEx;
	m_pImgCbAmode->InitDeviceObjects( pDataHeader );

#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경

    pDataHeader = FindResource("cbarena");
	m_pImgCbCountBk	 = new CINFImageEx;
	m_pImgCbCountBk->InitDeviceObjects( pDataHeader );

	pDataHeader = FindResource("cbarenaba");
	m_pImgCbAmodeBk = new CINFImageEx;
	m_pImgCbAmodeBk->InitDeviceObjects( pDataHeader );

	{
		DataHeader* pDataHeader = g_pGameMain->m_GruopImagemanager->FindResource( "are_cre" );
		m_pRenewArenaCreateBackImage = g_pGameMain->m_GruopImagemanager->GetGroupImage( pDataHeader );
		m_pRenewArenaCreateBackImage->InitDeviceObjects( g_pD3dApp->m_pImageList );
		m_pRenewArenaCreateBackImage->RestoreDeviceObjects();		
	}

	// 2012-04-13 by mspark, 아레나 UI 작업
	pDataHeader = FindResource("aremap01");
	m_pImgArenaMap01 = new CINFImageEx;
	m_pImgArenaMap01->InitDeviceObjects( pDataHeader );

	pDataHeader = FindResource("aremap02");
	m_pImgArenaMap02 = new CINFImageEx;
	m_pImgArenaMap02->InitDeviceObjects( pDataHeader );

	pDataHeader = FindResource("are_random");
	m_pImgArenaMapRandom = new CINFImageEx;
	m_pImgArenaMapRandom->InitDeviceObjects( pDataHeader );

	pDataHeader = FindResource("aremapsel");
	m_pImgArenaMapSelect = new CINFImageEx;
	m_pImgArenaMapSelect->InitDeviceObjects( pDataHeader );

	{
		DataHeader* pDataHeader = g_pGameMain->m_GruopImagemanager->FindResource( "ok_cancel" );
		m_pOkCancelImage = g_pGameMain->m_GruopImagemanager->GetGroupImage( pDataHeader );
		m_pOkCancelImage->InitDeviceObjects( g_pD3dApp->m_pImageList );
		m_pOkCancelImage->RestoreDeviceObjects();		
	}

	pDataHeader = FindResource("LM_inven");
	m_pImgFadeBG = new CINFImageEx;
	m_pImgFadeBG->InitDeviceObjects( pDataHeader );

	m_nOkImgMinPos.x = m_pOkCancelImage->GetFindControlTargetofMinPos("okb00").x;
	m_nOkImgMinPos.y = m_pOkCancelImage->GetFindControlTargetofMinPos("okb00").y;
	m_nCancelImgMinPos.x = m_pOkCancelImage->GetFindControlTargetofMinPos("canb00").x;
	m_nCancelImgMinPos.y = m_pOkCancelImage->GetFindControlTargetofMinPos("canb00").y;
	m_nOkCancelBaseImgMaxPos.x = m_pOkCancelImage->GetFindControlTargetofMaxPos("okb00").x;
	m_nOkCancelBaseImgMaxPos.y = m_pOkCancelImage->GetFindControlTargetofMaxPos("okb00").y;
	// end 2012-04-13 by mspark, 아레나 UI 작업

#endif
	

	
	return S_OK;


}
HRESULT CINFArenaCreate::RestoreDeviceObjects()
{
	int i;
	for(i = 0; i < BUTTON_STATE_NUMBER; i++)
	{
		m_pImgCreateB[i]->RestoreDeviceObjects();
//		m_pImgCbArr[i]->RestoreDeviceObjects();
		m_pImgCancelB[i]->RestoreDeviceObjects();
	}
	m_pFontCount->RestoreDeviceObjects();
	m_pFontMode->RestoreDeviceObjects();
	m_pFontPass->RestoreDeviceObjects();

	m_pImgAreCreate->RestoreDeviceObjects();
	m_pImgCbCount->RestoreDeviceObjects();
	m_pImgCbAmode->RestoreDeviceObjects();
		  
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
    m_pRenewArenaCreateBackImage->RestoreDeviceObjects();
	m_pImgCbCountBk->RestoreDeviceObjects();		
	m_pImgCbAmodeBk->RestoreDeviceObjects();

	// 2012-04-13 by mspark, 아레나 UI 작업
	m_pImgArenaMap01->RestoreDeviceObjects();
	m_pImgArenaMap02->RestoreDeviceObjects();
	m_pImgArenaMapRandom->RestoreDeviceObjects();
	m_pImgArenaMapSelect->RestoreDeviceObjects();
	m_pOkCancelImage->RestoreDeviceObjects();
	m_pImgFadeBG->RestoreDeviceObjects();
	// end 2012-04-13 by mspark, 아레나 UI 작업

#endif
	return S_OK;
}
HRESULT CINFArenaCreate::DeleteDeviceObjects()
{
	int i;
	DbgOutA("Arena Create DeleteDeviceObjects()\n");
	for(i = 0; i < BUTTON_STATE_NUMBER; i++)
	{
		m_pImgCreateB[i]->DeleteDeviceObjects();
//		m_pImgCbArr[i]->DeleteDeviceObjects();
		m_pImgCancelB[i]->DeleteDeviceObjects();
		util::del(m_pImgCreateB[i]);
//		util::del(m_pImgCbArr[i]);
		util::del(m_pImgCancelB[i]);
	}
	m_pFontCount->DeleteDeviceObjects();
	m_pFontMode->DeleteDeviceObjects();
	m_pFontPass->DeleteDeviceObjects();

	m_pImgAreCreate->DeleteDeviceObjects();
	m_pImgCbCount->DeleteDeviceObjects();
	m_pImgCbAmode->DeleteDeviceObjects();

	util::del(m_pImgAreCreate);
	util::del(m_pImgCbCount);
	util::del(m_pImgCbCount);

#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
    m_pRenewArenaCreateBackImage->DeleteDeviceObjects();
	m_pImgCbCountBk->DeleteDeviceObjects();		
	m_pImgCbAmodeBk->DeleteDeviceObjects();

    util::del(m_pRenewArenaCreateBackImage);
	util::del(m_pImgCbCountBk);
	util::del(m_pImgCbAmodeBk);

	// 2012-04-13 by mspark, 아레나 UI 작업
	m_pImgArenaMap01->DeleteDeviceObjects();
	m_pImgArenaMap02->DeleteDeviceObjects();
	m_pImgArenaMapRandom->DeleteDeviceObjects();
	m_pImgArenaMapSelect->DeleteDeviceObjects();
	m_pOkCancelImage->DeleteDeviceObjects();
	m_pImgFadeBG->DeleteDeviceObjects();

	util::del(m_pImgArenaMap01);
	util::del(m_pImgArenaMap02);
	util::del(m_pImgArenaMapRandom);
	util::del(m_pImgArenaMapSelect);
	util::del(m_pOkCancelImage);
	util::del(m_pImgFadeBG);
	// end 2012-04-13 by mspark, 아레나 UI 작업

#endif
    

	return S_OK;
}
HRESULT CINFArenaCreate::InvalidateDeviceObjects()
{
	int i;
	for(i = 0; i < BUTTON_STATE_NUMBER; i++)
	{
		m_pImgCreateB[i]->InvalidateDeviceObjects();
//		m_pImgCbArr[i]->InvalidateDeviceObjects();
		m_pImgCancelB[i]->InvalidateDeviceObjects();
	}
	m_pFontCount->InvalidateDeviceObjects();
	m_pFontMode->InvalidateDeviceObjects();
	m_pFontPass->InvalidateDeviceObjects();

	m_pImgAreCreate->InvalidateDeviceObjects();
	m_pImgCbCount->InvalidateDeviceObjects();
	m_pImgCbAmode->InvalidateDeviceObjects();
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
    m_pRenewArenaCreateBackImage->InvalidateDeviceObjects();
	m_pImgCbCountBk->InvalidateDeviceObjects();		
	m_pImgCbAmodeBk->InvalidateDeviceObjects();

	// 2012-04-13 by mspark, 아레나 UI 작업
	m_pImgArenaMap01->InvalidateDeviceObjects();
	m_pImgArenaMap02->InvalidateDeviceObjects();
	m_pImgArenaMapRandom->InvalidateDeviceObjects();
	m_pImgArenaMapSelect->InvalidateDeviceObjects();
	m_pOkCancelImage->InvalidateDeviceObjects();
	m_pImgFadeBG->InvalidateDeviceObjects();
	// end 2012-04-13 by mspark, 아레나 UI 작업

#endif  




	return S_OK;
}
void CINFArenaCreate::Render()
{
	char ctemp[64];
	int i;
	SIZE size;
	if(m_bShow)
	{
// 2012-05-29 by mspark, 아레나 통합(전달)
#ifdef C_ARENA_EX_1ST_MSPARK
		// 2012-04-13 by mspark, 아레나 UI 작업
		m_pImgFadeBG->Move(0, 0);
		m_pImgFadeBG->SetScale((INT)g_pD3dApp->GetBackBufferDesc().Width,(INT)g_pD3dApp->GetBackBufferDesc().Height);
		m_pImgFadeBG->Render();
		// end 2012-04-13 by mspark, 아레나 UI 작업
#endif
// end 2012-05-29 by mspark, 아레나 통합(전달)

#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
		m_pRenewArenaCreateBackImage->Move(m_nStartX,m_nStartY);
		m_pRenewArenaCreateBackImage->Render();
#else		
		m_pImgAreCreate->Move(m_nStartX,m_nStartY);
		m_pImgAreCreate->Render();
#endif

// 2012-05-29 by mspark, 아레나 통합(전달)
#ifdef C_ARENA_EX_1ST_MSPARK
		// 2012-04-13 by mspark, 아레나 UI 작업
		m_pImgArenaMap01->Move(m_nStartX + ARENA_CREATE_MAP_IMAGE_X, m_nStartY + ARENA_CREATE_MAP_IMAGE_Y);
		m_pImgArenaMap01->Render();

		m_pImgArenaMap02->Move(m_nStartX + ARENA_CREATE_MAP_IMAGE_X + ARENA_CREATE_MAP_IMAGE_W + ARENA_CREATE_MAP_IMAGE_TAB, m_nStartY + ARENA_CREATE_MAP_IMAGE_Y);
		m_pImgArenaMap02->Render();

		m_pImgArenaMapRandom->Move(m_nStartX + ARENA_CREATE_MAP_IMAGE_X + (ARENA_CREATE_MAP_IMAGE_W*2) + (ARENA_CREATE_MAP_IMAGE_TAB*2), m_nStartY + ARENA_CREATE_MAP_IMAGE_Y);
		m_pImgArenaMapRandom->Render();

		m_pImgArenaMapSelect->Move(m_nStartX + ARENA_CREATE_MAP_IMAGE_X + ( (ARENA_CREATE_MAP_IMAGE_W * m_nArenaMapImgTabNum) + ARENA_CREATE_MAP_IMAGE_TAB * m_nArenaMapImgTabNum ), m_nStartY + ARENA_CREATE_MAP_IMAGE_Y);
		m_pImgArenaMapSelect->Render();

		m_pImgCreateB[m_nStateCreateB]->Move(m_nStartX + ARENA_CREATE_BUTTON_X + m_nOkImgMinPos.x, m_nStartY + ARENA_CREATE_BUTTON_Y + m_nOkImgMinPos.y);
		m_pImgCreateB[m_nStateCreateB]->Render();

		m_pImgCancelB[m_nStateCancelB]->Move(m_nStartX + ARENA_CREATE_BUTTON_X + m_nCancelImgMinPos.x, m_nStartY + ARENA_CANCEL_BUTTON_Y + m_nCancelImgMinPos.y);
		m_pImgCancelB[m_nStateCancelB]->Render();
		// end 2012-04-13 by mspark, 아레나 UI 작업
#else
		m_pImgCreateB[m_nStateCreateB]->Move(m_nStartX + ARENA_CREATE_BUTTON_X, m_nStartY + ARENA_CREATE_BUTTON_Y);
		m_pImgCreateB[m_nStateCreateB]->Render();

		m_pImgCancelB[m_nStateCancelB]->Move(m_nStartX + ARENA_CANCEL_BUTTON_X, m_nStartY + ARENA_CANCEL_BUTTON_Y);
		m_pImgCancelB[m_nStateCancelB]->Render();
#endif
// end 2012-05-29 by mspark, 아레나 통합(전달)

//		m_pImgCbArr[m_nStateCbArrCount]->Move(m_nStartX + ARENA_CREATE_COUNT_COMBO_X,m_nStartY + ARENA_CREATE_COUNT_COMBO_Y);
//		m_pImgCbArr[m_nStateCbArrCount]->Render();
//
//		m_pImgCbArr[m_nStateCbArrMode]->Move(m_nStartX + ARENA_CREATE_MODE_COMBO_X,m_nStartY + ARENA_CREATE_MODE_COMBO_Y);
//		m_pImgCbArr[m_nStateCbArrMode]->Render();

		wsprintf(ctemp,"%d",m_nCount);
		size = m_pFontCount->GetStringSize(ctemp);
		m_pFontCount->DrawText(m_nStartX + ARENA_CREATE_COUNT_FONT_X - size.cx,m_nStartY + ARENA_CREATE_COUNT_FONT_Y,GUI_FONT_COLOR_W,ctemp);

		if(m_nSelectMode == ARENAMODE_DEATHMATCH)
			wsprintf(ctemp,"%s",STRMSG_C_070427_0100);
		else
			wsprintf(ctemp,"%s",STRMSG_C_070427_0101);

		size = m_pFontCount->GetStringSize(ctemp);

		m_pFontMode->DrawText(m_nStartX + ARENA_CREATE_MODE_FONT_X - (size.cx/2),m_nStartY + ARENA_CREATE_MODE_FONT_Y-2,GUI_FONT_COLOR_W,ctemp);	// 2012-03-29 by mspark, 아레나 생성 창에서 모드표시 부분 위치 수정 - y값 -2 추가
					
#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
		m_pImgCbCountBk->Move(m_nStartX + ARENA_CREATE_COUNT_X, m_nStartY + ARENA_CREATE_COUNT_Y);
		m_pImgCbCountBk->Render();

		m_pImgCbAmodeBk->Move(m_nStartX + ARENA_CREATE_MODE_X, m_nStartY + ARENA_CREATE_MODE_Y);
		m_pImgCbAmodeBk->Render();

#endif

		if(m_bCbCount)
		{// 인원 콤보 박스 열기.
// 2012-05-29 by mspark, 아레나 통합(전달)
#ifdef C_ARENA_EX_1ST_MSPARK
			for(i = 0; i < m_vecPersons.size()/ARENA_MAP_COUNT; i++)	// 2012-04-13 by mspark, 아레나 UI 작업 - 기존 m_vecPersons.size()를 m_vecPersons.size()/ARENA_MAP_COUNT로 변경(아레나 맵 갯수로 나눔)
#else
			for(i = 0; i < m_vecPersons.size(); i++)
#endif
// end 2012-05-29 by mspark, 아레나 통합(전달)
			{
				
				// 콤보 배경 이미지
				m_pImgCbCount->Move(m_nStartX + ARENA_CREATE_COUNT_X, 
					m_nStartY + ARENA_CREATE_COUNT_Y + ARENA_CREATE_COUNT_H +(ARENA_CREATE_COUNT_H * i));
				m_pImgCbCount->Render();
				
				// 인원 숫자.
				wsprintf(ctemp,"%d",m_vecPersons[i]);
				size = m_pFontCount->GetStringSize(ctemp);
				m_pFontCount->DrawText(m_nStartX + ARENA_CREATE_COUNT_FONT_X - size.cx,
					m_nStartY + ARENA_CREATE_COUNT_FONT_Y + ARENA_CREATE_COUNT_H + (ARENA_CREATE_COUNT_H * i),
					GUI_FONT_COLOR_Y,ctemp);
			}

		}
		if(m_bCbMode)
		{// 모드 콤보 박스 열기.
			for(i = 0; i < m_nMode; i++)
			{
				m_pImgCbAmode->Move(m_nStartX + ARENA_CREATE_MODE_X-1,
					m_nStartY + ARENA_CREATE_MODE_Y + ARENA_CREATE_MODE_H + (ARENA_CREATE_MODE_H * i)+1);		// 2012-03-29 by mspark, 아레나 생성 창에서 모드표시 부분 위치 수정 - x좌표 값 -1, y좌표 값 +1 추가
				m_pImgCbAmode->Render();

				if((i+1) == ARENAMODE_DEATHMATCH)
					wsprintf(ctemp,"%s",STRMSG_C_070427_0100);
				else if((i+1) == ARENAMODE_ROUND)
					wsprintf(ctemp,"%s",STRMSG_C_070427_0101);
				size = m_pFontCount->GetStringSize(ctemp);
				m_pFontMode->DrawText(m_nStartX + ARENA_CREATE_MODE_FONT_X - (size.cx/2),
					m_nStartY + ARENA_CREATE_MODE_FONT_Y + ARENA_CREATE_MODE_H + (ARENA_CREATE_MODE_H * i),
					GUI_FONT_COLOR_Y,ctemp);

			}

		}
		if(m_bTxtPass)
		{// 비밀번호 입력중일경우.
			wsprintf(ctemp,"%s_",m_cTeamPW);
			m_pFontPass->DrawText(m_nStartX + ARENA_CREATE_PASS_FONT_X,
				m_nStartY + ARENA_CREATE_PASS_FONT_Y,
				GUI_FONT_COLOR_W,ctemp);
		}else
		{
			if(strlen(m_cTeamPW))
			{
				wsprintf(ctemp,"%s",m_cTeamPW);
					m_pFontPass->DrawText(m_nStartX + ARENA_CREATE_PASS_FONT_X,
						m_nStartY + ARENA_CREATE_PASS_FONT_Y,
						GUI_FONT_COLOR_W,ctemp);


			}
		}
	}
}
void CINFArenaCreate::Tick()
{
	
}
int CINFArenaCreate::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if(m_bShow)
	{
		switch(uMsg)
		{
			case WM_MOUSEMOVE:
				{
					POINT pt;
					pt.x = LOWORD(lParam);
					pt.y = HIWORD(lParam);
					CheckMouseReverse(&pt);

// 2012-05-29 by mspark, 아레나 통합(전달)
#ifdef C_ARENA_EX_1ST_MSPARK
					if(pt.y > ARENA_CREATE_BUTTON_Y + m_nStartY + m_nOkImgMinPos.y &&
						pt.y < ARENA_CREATE_BUTTON_Y + m_nStartY + m_nOkImgMinPos.y + m_nOkCancelBaseImgMaxPos.y)
					{
						if(pt.x > ARENA_CREATE_BUTTON_X + m_nStartX + m_nOkImgMinPos.x &&
							pt.x < ARENA_CREATE_BUTTON_X + m_nStartX + m_nOkImgMinPos.x + m_nOkCancelBaseImgMaxPos.x)
						{
							m_nStateCreateB = BUTTON_STATE_UP;
						}
						else
						{
							m_nStateCreateB = BUTTON_STATE_NORMAL;
						}
						if(pt.x > ARENA_CANCEL_BUTTON_X + m_nStartX + m_nCancelImgMinPos.x &&
							pt.x < ARENA_CANCEL_BUTTON_X + m_nStartX + m_nCancelImgMinPos.x + m_nOkCancelBaseImgMaxPos.x)
						{
							m_nStateCancelB = BUTTON_STATE_UP;
						}
						else
						{
							m_nStateCancelB = BUTTON_STATE_NORMAL;
						}

					}else
					{
						m_nStateCreateB = BUTTON_STATE_NORMAL;
						m_nStateCancelB = BUTTON_STATE_NORMAL;
					}
#else
					if(pt.y > ARENA_CREATE_BUTTON_Y + m_nStartY &&
						pt.y < ARENA_CREATE_BUTTON_Y + ARENA_BASE_BUTTON_H + m_nStartY)
					{
						if(pt.x > ARENA_CREATE_BUTTON_X + m_nStartX &&
							pt.x < ARENA_CREATE_BUTTON_X + ARENA_BASE_BUTTON_W + m_nStartX)
						{
							m_nStateCreateB = BUTTON_STATE_UP;
						}
						else
						{
							m_nStateCreateB = BUTTON_STATE_NORMAL;
						}
						if(pt.x > ARENA_CANCEL_BUTTON_X + m_nStartX &&
							pt.x < ARENA_CANCEL_BUTTON_X + ARENA_BASE_BUTTON_W + m_nStartX)
						{
							m_nStateCancelB = BUTTON_STATE_UP;
						}
						else
						{
							m_nStateCancelB = BUTTON_STATE_NORMAL;
						}

					}else
					{
						m_nStateCreateB = BUTTON_STATE_NORMAL;
						m_nStateCancelB = BUTTON_STATE_NORMAL;
					}
#endif
// end 2012-05-29 by mspark, 아레나 통합(전달)

//					// 콤보 박스 처리(인원).
//					if(pt.x > ARENA_CREATE_COUNT_X + m_nStartX &&
//						pt.x < ARENA_CREATE_COUNT_X + ARENA_CREATE_COUNT_W + m_nStartX &&
//						pt.y > ARENA_CREATE_COUNT_Y + m_nStartY &&
//						pt.y < ARENA_CREATE_COUNT_Y + ARENA_CREATE_COUNT_H + m_nStartY)
//					{
//						m_nStateCbArrCount = BUTTON_STATE_UP;
//					}
//					else
//					{
//						m_nStateCbArrCount = BUTTON_STATE_NORMAL;
//					}
//					// 콤보 박스 처리(모드).
//					if(pt.x > ARENA_CREATE_MODE_X + m_nStartX &&
//						pt.x < ARENA_CREATE_MODE_X + ARENA_CREATE_MODE_W + m_nStartX &&
//						pt.y > ARENA_CREATE_MODE_Y + m_nStartY &&
//						pt.y < ARENA_CREATE_MODE_Y + ARENA_CREATE_MODE_H + m_nStartY)
//					{
//						m_nStateCbArrMode = BUTTON_STATE_UP;
//					}
//					else
//					{
//						m_nStateCbArrMode = BUTTON_STATE_NORMAL;
//					}

// 2012-05-29 by mspark, 아레나 통합(전달)
#ifdef C_ARENA_EX_1ST_MSPARK
					// 2012-04-13 by mspark, 아레나 UI 작업
					char buf[64] = { 0, };	

					if(pt.y > ARENA_CREATE_MAP_IMAGE_Y + m_nStartY &&
						pt.y < ARENA_CREATE_MAP_IMAGE_Y + ARENA_CREATE_MAP_IMAGE_H + m_nStartY)
					{
						if(pt.x > ARENA_CREATE_MAP_IMAGE_X + m_nStartX &&
							pt.x < ARENA_CREATE_MAP_IMAGE_X + ARENA_CREATE_MAP_IMAGE_W + m_nStartX)
						{
							wsprintf( buf, "%s", STRMSG_C_120413_0001);
							g_pGameMain->SetToolTip( pt.x, pt.y, buf);
							return INF_MSGPROC_BREAK;
						}
						
						if(pt.x > ARENA_CREATE_MAP_IMAGE_X + ARENA_CREATE_MAP_IMAGE_W + ARENA_CREATE_MAP_IMAGE_TAB + m_nStartX &&
							pt.x < ARENA_CREATE_MAP_IMAGE_X + (ARENA_CREATE_MAP_IMAGE_W * 2) + ARENA_CREATE_MAP_IMAGE_TAB + m_nStartX)
						{
							wsprintf( buf, "%s", STRMSG_C_120413_0002);
							g_pGameMain->SetToolTip( pt.x, pt.y, buf);
							return INF_MSGPROC_BREAK;
						}
						
						if(pt.x > ARENA_CREATE_MAP_IMAGE_X + (ARENA_CREATE_MAP_IMAGE_W * 2) + (ARENA_CREATE_MAP_IMAGE_TAB * 2) + m_nStartX &&
							pt.x < ARENA_CREATE_MAP_IMAGE_X + (ARENA_CREATE_MAP_IMAGE_W * 3) + (ARENA_CREATE_MAP_IMAGE_TAB * 2) + m_nStartX)
						{
							wsprintf( buf, "%s", STRMSG_C_120413_0003);
							g_pGameMain->SetToolTip( pt.x, pt.y, buf);
							return INF_MSGPROC_BREAK;
						}						
					}
					// end 2012-04-13 by mspark, 아레나 UI 작업
#endif
// end 2012-05-29 by mspark, 아레나 통합(전달)

				}
				break;
			case WM_LBUTTONDOWN:
				{
					POINT pt;
					pt.x = LOWORD(lParam);
					pt.y = HIWORD(lParam);
					CheckMouseReverse(&pt);
					// 콤보 박스가 활성화 되어있을대는 제일 우선적으로 처리한다.
					if(m_bCbCount &&
						pt.x > ARENA_CREATE_COUNT_X + m_nStartX &&
						pt.x < ARENA_CREATE_COUNT_X + ARENA_CREATE_COUNT_W + m_nStartX )
//						pt.y > ARENA_CREATE_COUNT_Y + m_nStartY &&
//						pt.y < ARENA_CREATE_COUNT_Y + ARENA_CREATE_COUNT_H + m_nStartY)
					{
						int nTempSelectCount = (pt.y - (ARENA_CREATE_COUNT_Y + ARENA_CREATE_COUNT_H + m_nStartY)) / ARENA_CREATE_COUNT_H;

						if(nTempSelectCount < 0 
							|| nTempSelectCount > m_vecPersons.size())
						{
						}
						else
						{
							m_nSelectCount = nTempSelectCount;
							m_nCount = m_vecPersons[m_nSelectCount];
						}
						m_bCbCount = FALSE;
						
						break;
					}
				

					if(m_bCbMode &&
						pt.x > ARENA_CREATE_MODE_X + m_nStartX &&
						pt.x < ARENA_CREATE_MODE_X + ARENA_CREATE_MODE_W + m_nStartX )
//						pt.y > ARENA_CREATE_MODE_Y + m_nStartY &&
//						pt.y < ARENA_CREATE_MODE_Y + ARENA_CREATE_MODE_H + m_nStartY)
					{
						int nTempSelectMode = (pt.y - (ARENA_CREATE_MODE_Y +ARENA_CREATE_MODE_H + m_nStartY)) / ARENA_CREATE_MODE_H;
						if(nTempSelectMode < 0 
							|| nTempSelectMode >= m_nMode)			
						{
						}
						else
						{
							m_nSelectMode = nTempSelectMode + 1;			// 1 데스 매치 2 라운드 이다 0은 없기때문에 1을 더해준다.
						}
						m_bCbMode = FALSE;
						break;

					}
				
					//--------------------------------------------------------------------------//
					

// 2012-05-29 by mspark, 아레나 통합(전달)
#ifdef C_ARENA_EX_1ST_MSPARK
					// 2012-04-13 by mspark, 아레나 UI 작업
					if(pt.y > ARENA_CREATE_MAP_IMAGE_Y + m_nStartY &&
						pt.y < ARENA_CREATE_MAP_IMAGE_Y + ARENA_CREATE_MAP_IMAGE_H + m_nStartY)
					{
						if(pt.x > ARENA_CREATE_MAP_IMAGE_X + m_nStartX &&
							pt.x < ARENA_CREATE_MAP_IMAGE_X + ARENA_CREATE_MAP_IMAGE_W + m_nStartX)
						{
							m_nArenaMapImgTabNum = ARENA_MAP_01_INDEX;
						}
						
						if(pt.x > ARENA_CREATE_MAP_IMAGE_X + ARENA_CREATE_MAP_IMAGE_W + ARENA_CREATE_MAP_IMAGE_TAB + m_nStartX &&
							pt.x < ARENA_CREATE_MAP_IMAGE_X + (ARENA_CREATE_MAP_IMAGE_W * 2) + ARENA_CREATE_MAP_IMAGE_TAB + m_nStartX)
						{
							m_nArenaMapImgTabNum = ARENA_MAP_02_INDEX;
						}
						
						if(pt.x > ARENA_CREATE_MAP_IMAGE_X + (ARENA_CREATE_MAP_IMAGE_W * 2) + (ARENA_CREATE_MAP_IMAGE_TAB * 2) + m_nStartX &&
							pt.x < ARENA_CREATE_MAP_IMAGE_X + (ARENA_CREATE_MAP_IMAGE_W * 3) + (ARENA_CREATE_MAP_IMAGE_TAB * 2) + m_nStartX)
						{
							m_nArenaMapImgTabNum = ARENA_MAP_RANDOM_INDEX;
						}						
					}
					// end 2012-04-13 by mspark, 아레나 UI 작업
#endif
// end 2012-05-29 by mspark, 아레나 통합(전달)

// 2012-05-29 by mspark, 아레나 통합(전달)
#ifdef C_ARENA_EX_1ST_MSPARK
					if(pt.y > ARENA_CREATE_BUTTON_Y + m_nStartY + m_nOkImgMinPos.y &&
						pt.y < ARENA_CREATE_BUTTON_Y + m_nStartY + m_nOkImgMinPos.y + m_nOkCancelBaseImgMaxPos.y)
					{
						if(pt.x > ARENA_CREATE_BUTTON_X + m_nStartX + m_nOkImgMinPos.x &&
							pt.x < ARENA_CREATE_BUTTON_X + m_nStartX + m_nOkImgMinPos.x + m_nOkCancelBaseImgMaxPos.x)
						{
							m_nStateCreateB = BUTTON_STATE_DOWN;
						}
						else
						{
							m_nStateCreateB = BUTTON_STATE_NORMAL;
						}
						if(pt.x > ARENA_CANCEL_BUTTON_X + m_nStartX + m_nCancelImgMinPos.x &&
							pt.x < ARENA_CANCEL_BUTTON_X + m_nStartX + m_nCancelImgMinPos.x + m_nOkCancelBaseImgMaxPos.x)
						{
							m_nStateCancelB = BUTTON_STATE_DOWN;
						}
						else
						{
							m_nStateCancelB = BUTTON_STATE_NORMAL;
						}

					}else
					{
						m_nStateCreateB = BUTTON_STATE_NORMAL;
						m_nStateCancelB = BUTTON_STATE_NORMAL;
					}
#else
					if(pt.y > ARENA_CREATE_BUTTON_Y + m_nStartY &&
						pt.y < ARENA_CREATE_BUTTON_Y + ARENA_BASE_BUTTON_H + m_nStartY)
					{
						if(pt.x > ARENA_CREATE_BUTTON_X + m_nStartX &&
							pt.x < ARENA_CREATE_BUTTON_X + ARENA_BASE_BUTTON_W + m_nStartX)
						{
							m_nStateCreateB = BUTTON_STATE_DOWN;
						}
						else
						{
							m_nStateCreateB = BUTTON_STATE_NORMAL;
						}
						if(pt.x > ARENA_CANCEL_BUTTON_X + m_nStartX &&
							pt.x < ARENA_CANCEL_BUTTON_X + ARENA_BASE_BUTTON_W + m_nStartX)
						{
							m_nStateCancelB = BUTTON_STATE_DOWN;
						}
						else
						{
							m_nStateCancelB = BUTTON_STATE_NORMAL;
						}

					}else
					{
						m_nStateCreateB = BUTTON_STATE_NORMAL;
						m_nStateCancelB = BUTTON_STATE_NORMAL;
					}
#endif
// end 2012-05-29 by mspark, 아레나 통합(전달)

					// 콤보 박스 처리(인원).
					if(!m_bCbMode &&
						pt.x > ARENA_CREATE_COUNT_X + m_nStartX &&
						pt.x < ARENA_CREATE_COUNT_X + ARENA_CREATE_COUNT_W + m_nStartX &&
						pt.y > ARENA_CREATE_COUNT_Y + m_nStartY &&
						pt.y < ARENA_CREATE_COUNT_Y + ARENA_CREATE_COUNT_H + m_nStartY)
					{
//						m_nStateCbArrCount = BUTTON_STATE_DOWN;
						m_bCbCount = TRUE;
					}
					else
					{
//						m_nStateCbArrCount = BUTTON_STATE_NORMAL;
						m_bCbCount = FALSE;
					}
					// 콤보 박스 처리(모드).
					if(!m_bCbCount &&
						pt.x > ARENA_CREATE_MODE_X + m_nStartX &&
						pt.x < ARENA_CREATE_MODE_X + ARENA_CREATE_MODE_W + m_nStartX &&
						pt.y > ARENA_CREATE_MODE_Y + m_nStartY &&
						pt.y < ARENA_CREATE_MODE_Y + ARENA_CREATE_MODE_H + m_nStartY)
					{
//						m_nStateCbArrMode = BUTTON_STATE_DOWN;
						m_bCbMode = TRUE;
					}
					else
					{
//						m_nStateCbArrMode = BUTTON_STATE_NORMAL;
						m_bCbMode = FALSE;
					}



				}
				break;
			case WM_LBUTTONUP:
				{
					POINT pt;
					pt.x = LOWORD(lParam);
					pt.y = HIWORD(lParam);
					CheckMouseReverse(&pt);
// 2012-05-29 by mspark, 아레나 통합(전달)
#ifdef C_ARENA_EX_1ST_MSPARK
					if(pt.y > ARENA_CREATE_BUTTON_Y + m_nStartY + m_nOkImgMinPos.y &&
						pt.y < ARENA_CREATE_BUTTON_Y + m_nStartY + m_nOkImgMinPos.y + m_nOkCancelBaseImgMaxPos.y)
					{
						if(pt.x > ARENA_CREATE_BUTTON_X + m_nStartX + m_nOkImgMinPos.x &&
							pt.x < ARENA_CREATE_BUTTON_X + m_nStartX + m_nOkImgMinPos.x + m_nOkCancelBaseImgMaxPos.x)
						{
							m_nStateCreateB = BUTTON_STATE_UP;
							SendCreateTeam();
							HideCreateWindow();
						}
						else
						{
							m_nStateCreateB = BUTTON_STATE_NORMAL;
						}
						if(pt.x > ARENA_CANCEL_BUTTON_X + m_nStartX + m_nCancelImgMinPos.x &&
							pt.x < ARENA_CANCEL_BUTTON_X + m_nStartX + m_nCancelImgMinPos.x+ m_nOkCancelBaseImgMaxPos.x)
						{
							m_nStateCancelB = BUTTON_STATE_UP;
							HideCreateWindow();
						}
						else
						{
							m_nStateCancelB = BUTTON_STATE_NORMAL;
						}

					}else
					{
						m_nStateCreateB = BUTTON_STATE_NORMAL;
						m_nStateCancelB = BUTTON_STATE_NORMAL;
					}
#else
					if(pt.y > ARENA_CREATE_BUTTON_Y + m_nStartY &&
						pt.y < ARENA_CREATE_BUTTON_Y + ARENA_BASE_BUTTON_H + m_nStartY)
					{
						if(pt.x > ARENA_CREATE_BUTTON_X + m_nStartX &&
							pt.x < ARENA_CREATE_BUTTON_X + ARENA_BASE_BUTTON_W + m_nStartX)
						{
							m_nStateCreateB = BUTTON_STATE_UP;
							SendCreateTeam();
							HideCreateWindow();
						}
						else
						{
							m_nStateCreateB = BUTTON_STATE_NORMAL;
						}
						if(pt.x > ARENA_CANCEL_BUTTON_X + m_nStartX &&
							pt.x < ARENA_CANCEL_BUTTON_X + ARENA_BASE_BUTTON_W + m_nStartX)
						{
							m_nStateCancelB = BUTTON_STATE_UP;
							HideCreateWindow();
						}
						else
						{
							m_nStateCancelB = BUTTON_STATE_NORMAL;
						}

					}else
					{
						m_nStateCreateB = BUTTON_STATE_NORMAL;
						m_nStateCancelB = BUTTON_STATE_NORMAL;
					}
#endif
// end 2012-05-29 by mspark, 아레나 통합(전달)

					// 비밀번호 
					if(pt.x > ARENA_CREATE_PASS_X + m_nStartX &&
						pt.x < ARENA_CREATE_PASS_X + m_nStartX + ARENA_CREATE_PASS_W &&
						pt.y > ARENA_CREATE_PASS_Y + m_nStartY &&
						pt.y < ARENA_CREATE_PASS_Y + m_nStartY + ARENA_CREATE_PASS_H)
					{
						m_bTxtPass = TRUE;
					}
					else
					{
						m_bTxtPass = FALSE;
					}
//					// 콤보 박스 처리(인원).
//					if(pt.x > ARENA_CREATE_COUNT_X + m_nStartX &&
//						pt.x < ARENA_CREATE_COUNT_X + ARENA_CREATE_COUNT_W + m_nStartX &&
//						pt.y > ARENA_CREATE_COUNT_Y + m_nStartY &&
//						pt.y < ARENA_CREATE_COUNT_Y + ARENA_CREATE_COUNT_H + m_nStartY)
//					{
//						m_nStateCbArrCount = BUTTON_STATE_UP;
//					}
//					else
//					{
//						m_nStateCbArrCount = BUTTON_STATE_NORMAL;
//					}
//					// 콤보 박스 처리(모드).
//					if(pt.x > ARENA_CREATE_MODE_X + m_nStartX &&
//						pt.x < ARENA_CREATE_MODE_X + ARENA_CREATE_MODE_W + m_nStartX &&
//						pt.y > ARENA_CREATE_MODE_Y + m_nStartY &&
//						pt.y < ARENA_CREATE_MODE_Y + ARENA_CREATE_MODE_H + m_nStartY)
//					{
//						m_nStateCbArrMode = BUTTON_STATE_UP;
//					}
//					else
//					{
//						m_nStateCbArrMode = BUTTON_STATE_NORMAL;
//					}
				}
				break;
				// 2009-01-30 by bhsohn 상점 숫자 입력 방식 변경
// 			case WM_CHAR:
// 				{
// 					if(m_bTxtPass)
// 					{
// 						char code = wParam;
// 						int num = strlen(m_cTeamPW);
// 						if(code == VK_BACK)
// 						{
// 							if(num > 0)
// 								m_cTeamPW[num-1] = NULL;
// 						}
// 						if(num > MAX_PASSWORD)
// 						{
// 							break;
// 						}
// 						if(code >= '0' && code <= '9' )
// 						{
// 							m_cTeamPW[num] = code;
// 							m_cTeamPW[num+1] = NULL;
// 						}
// 						
// 					}
// 				}
// 				break;
			// 2012-12-10 by jhjang 아레나 암호 영문도 가능하게 입력 수정 
#ifdef 	C_ARENA_PASSWORD_ALPHABET
			case WM_CHAR:
				{
					if(m_bTxtPass)
					{
						DWORD dwCode = lParam;
						
						int num = strlen(m_cTeamPW);
						if(0x000e0001 == dwCode)
						{
							if(num > 0)
								m_cTeamPW[num-1] = NULL;
						}
						if(num > MAX_PASSWORD)
						{
							break;
						}
						int cCurPassword = g_pGameMain->GetKeyDownToNumberAndAlphabet(wParam);
						if(cCurPassword != -1 )					
						{
							m_cTeamPW[num] = cCurPassword;
							m_cTeamPW[num+1] = NULL;
							
						}						
					}
				}
				break;

#else
			case WM_KEYDOWN:
				{
					if(m_bTxtPass)
					{
						DWORD dwCode = lParam;

						int num = strlen(m_cTeamPW);
						if(0x000e0001 == dwCode)
						{
							if(num > 0)
								m_cTeamPW[num-1] = NULL;
						}
						if(num > MAX_PASSWORD)
						{
							break;
						}
						int nNumber = g_pGameMain->GetKeyDownToNumber(dwCode);
						if(nNumber != -1 )					
						{
							m_cTeamPW[num] = '0'+nNumber;
							m_cTeamPW[num+1] = NULL;

						}						
					}
				}
				break;
			// end 2009-01-30 by bhsohn 상점 숫자 입력 방식 변경
#endif
			// end 2012-12-10 by jhjang 아레나 암호 영문도 가능하게 입력 수정 
		}
	}

	return INF_MSGPROC_NORMAL;
}

VOID CINFArenaCreate::SetPos(UINT x, UINT y)
{
	m_nStartX = x;
	m_nStartY = y;
}

VOID CINFArenaCreate::AddAllPersons(SARENA_REQUEST_CREATE_TEAMINFO *pTeamInfo)
{
	if(pTeamInfo->ArenaMode != m_ntempMode)
	{
		m_nMode++;
		m_ntempMode = pTeamInfo->ArenaMode;
	}
	m_vecAllPersons.push_back((*pTeamInfo));
	
}

VOID CINFArenaCreate::ClearAllPersons()
{
	m_ntempMode = -1;
	m_vecAllPersons.clear();
}
VOID CINFArenaCreate::SetPersons(BYTE BMode)
{
	ClearPersons();
	vector<SARENA_REQUEST_CREATE_TEAMINFO>::iterator it = m_vecAllPersons.begin();
	while(it != m_vecAllPersons.end())
	{
		if(it->ArenaMode == BMode)
			m_vecPersons.push_back(it->ReqMemberNum);
		it++;
	}
	m_nCount = m_vecPersons[m_nSelectCount];
}

VOID CINFArenaCreate::ClearPersons()
{
	m_vecPersons.clear();
}
VOID CINFArenaCreate::ChangeArenaMode(BYTE BMode)
{
	SetPersons(BMode);
	m_nSelectMode = (UINT)BMode;
}
VOID CINFArenaCreate::ShowCreateWindow()
{
	m_bShow = TRUE;
	m_nSelectCount = 0;
	memset(m_cTeamPW,0x00,SIZE_MAX_TEAM_PW);
	m_nSelectMode = ARENAMODE_DEATHMATCH;

}
VOID CINFArenaCreate::HideCreateWindow()
{
	m_bShow = FALSE;
	ClearPersons();
	memset(m_cTeamPW,0x00,SIZE_MAX_TEAM_PW);
}
VOID CINFArenaCreate::SendCreateTeam()
{
	// 2008-03-07 by bhsohn 세력전시, 아레나 이용 못하게 변경
	if(TRUE == g_pD3dApp->IsDoingInfluenceWar())
	{
		g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_070427_0104,COLOR_ERROR); //"세력전이 진행 중이므로 참가할 수 없습니다."
		return;
	}
	// end 2008-03-07 by bhsohn 세력전시, 아레나 이용 못하게 변경
	// 2008-03-10 by bhsohn 아레나 관련 버그 수정
	if(TRUE == g_pInterface->IsArenaGameState(ARENA_GAME_STATE_TEAMINFO))
	{
		g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_080225_0207,COLOR_ERROR);//"이미 아레나 대기방에 참여중입니다."
		return;	
	}
	// end 2008-03-10 by bhsohn 아레나 관련 버그 수정

	MSG_FC_ARENA_CREATE_TEAM sMsg;
	memset(&sMsg,0x00,sizeof(MSG_FC_ARENA_CREATE_TEAM));
	sMsg.ArenaMode = m_nSelectMode;
	// 2007-11-22 by bhsohn 아레나 통합서버
	//sMsg.ClientIndex = g_pShuttleChild->m_myShuttleInfo.ClientIndex;
	CHARACTER*	pArenaCharacterInfo = g_pD3dApp->GetArenaCharacterInfo();
	sMsg.ClientIndex = pArenaCharacterInfo->ClientIndex;

	sMsg.TeamSize = m_nCount;
	//비밀번호 처리.
	int size = strlen(m_cTeamPW);
	if(size > 0)
	{
		sMsg.TeamLock = TRUE;
		//memcpy(sMsg.TeamPW,m_cTeamPW,size);
		strncpy(sMsg.TeamPW, m_cTeamPW, SIZE_MAX_TEAM_PW);
	}
	else
	{
		sMsg.TeamLock = FALSE;
	}

// 2012-05-29 by mspark, 아레나 통합(전달)
#ifdef C_ARENA_EX_1ST_MSPARK
	// 2012-04-13 by mspark, 아레나 UI 작업
	if(m_nArenaMapImgTabNum == ARENA_MAP_01_INDEX)
	{
		m_nArenaMapNum = ARENA_MAP_01_NUMBER;
	}
	else if(m_nArenaMapImgTabNum == ARENA_MAP_02_INDEX)
	{
		m_nArenaMapNum = ARENA_MAP_02_NUMBER;
	}
	else if(m_nArenaMapImgTabNum == ARENA_MAP_RANDOM_INDEX)
	{
		m_nArenaMapNum = ARENA_MAP_RANDOM_NUMBER;
	}

	sMsg.ArenaMap = m_nArenaMapNum;

#else

	sMsg.ArenaMap = ARENA_MAP_01_NUMBER;
	// end 2012-04-13 by mspark, 아레나 UI 작업
#endif
// end 2012-05-29 by mspark, 아레나 통합(전달)

	g_pFieldWinSocket->SendMsg(T_FC_ARENA_CREATE_TEAM,(char*)&sMsg,sizeof(sMsg));
}
