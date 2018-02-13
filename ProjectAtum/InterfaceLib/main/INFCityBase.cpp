// INFCityBase.cpp: implementation of the CINFCityBase class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "INFCityBase.h"
#include "AtumApplication.h"
#include "INFCityShop.h"
#include "QuestData.h"
#include "INFCityMission.h"
#include "INFCityWarp.h"
#include "INFImage.h"
#include "D3DHanFont.h"
#include "GameDataLast.h"
#include "FieldWinSocket.h"
#include "ShuttleChild.h"
#include "CharacterChild.h"				// 2005-07-21 by ispark
#include "INFGameMain.h"
#include "AtumDatabase.h"
#include "INFWindow.h"
#include "SceneData.h"
#include "RangeTime.h"
#include "INFInven.h"
#include "INFScrollBar.h"
#include "GameDataLast.h"
#include "INFCityLab.h"
#include "INFCityStore.h"
#include "AtumSound.h"
#include "INFCitySkillShop.h"
#include "INFLuckyMachine.h"
// 2008-09-22 by bhsohn EP3 캐릭터 창
//#include "INFCharacterInfo.h"
#include "INFCharacterInfoExtend.h"
#include "Interface.h"
#include "INFTrade.h"
// 2007-11-22 by bhsohn 아레나 통합서버
#include "IMSocketManager.h"
//#include "IMSocket.h"

#include "StoreData.h"
#include "Chat.h"
#include "INFCityAuction.h"
#include "INFCityOccupy.h"
#include "INFCityCashShop.h"
#include "dxutil.h"
#include "INFCitySupply.h"				// 2005-10-24 by ispark
#include "INFGameMainChat.h"			// 2006-06-01 by ispark
#include "INFCityBazaar.h"				// 2006-07-25 by ispark
#include "INFSelect.h"
#include "INFCityArena.h"				// 2007-04-24 by dgwoo
#include "INFCityWarPointShop.h"
#include "INFCityOutPost.h"
#include "INFCityLeader.h"
#include "INFMotherShipManager.h"

// 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템
#include "INFCityInfinityField.h"
#include "INFCityInfinityShop.h"
// end 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템
// 2010. 04. 21 by jskim 신규 럭키 머신 구현
#include "INFOptionMachine.h"
//end 2010. 04. 21 by jskim 신규 럭키 머신 구현
// 2010-08-31 by shcho&&jskim, 용해 시스템 구현
#include "INFDissolution.h"
// end 2010-08-31 by shcho&&jskim, 용해 시스템 구현
#include "INFImageEx.h"		              // 2011. 10. 10 by jskim UI시스템 변경
#include "INFGroupImage.h"
#include "INFGroupManager.h"
// 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현
#include "INFToolTip.h"
// end 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현

#include "INFCityInflBuff.h"	// 2013-05-07 by bhsohn 세력포인트 개선안 시스템

#define SCROLL_START_X				(CITY_BASE_NPC_BOX_START_X + 703)
#define SCROLL_START_Y				(CITY_BASE_NPC_BOX_START_Y + 6 )

#define SCROLL_LINE_LENGTH			65
#define CITY_BASE_NPC_TEXT_SIZE_X	675

#define IS_EXEC_INVENTORY(x)	(x == BUILDINGKIND_1ST_WEAPONSHOP		||			\
								 x == BUILDINGKIND_2ND_WEAPONSHOP		||			\
								 x == BUILDINGKIND_PUBLICOFFICE			||			\
								 x == BUILDINGKIND_LABORATORY			||			\
								 x == BUILDINGKIND_FACTORY				||			\
								 x == BUILDINGKIND_AUCTION				||			\
								 x == BUILDINGKIND_STORE				||			\
								 x == BUILDINGKIND_TUNING_COLOR			||			\
								 x == BUILDINGKIND_WARPOINT_SHOP		||			\
								 x == BUILDINGKIND_PARTSSHOP			||			\
								 x == BUILDINGKIND_GEARSHOP				||			\
								 x == BUILDINGKIND_CARDSHOP				||			\
								 x == BUILDINGKIND_TRIGGER_CRYSTAL)					//2011-12-08 by jhahn EP4 트리거 상점 UI 변경
								 // 2012-02-29 by mspark, 상점 타이틀 넣기 - 파츠, 기어, 카드상점 추가

//								 x == BUILDINGKIND_CITY_OCCUPY_INFO		||			\
//								 x == BUILDINGKIND_REFINERY				||			\

 
// 2012-02-29 by mspark, 상점 타이틀 넣기
#define		TITLE_SHOP_START_X		(CITY_BASE_NPC_BOX_START_X + 427)
#define		TITLE_SHOP_START_Y		(CITY_BASE_NPC_BOX_START_Y - 450)
	
#define		TITLE_LAP_START_X		(CITY_BASE_NPC_BOX_START_X + 480)
#define		TITLE_LAP_START_Y		(CITY_BASE_NPC_BOX_START_Y - 450)
// end 2012-02-29 by mspark, 상점 타이틀 넣기  

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CINFCityBase::CINFCityBase(CAtumNode* pParent)
{
	FLOG( "CINFCityBase::CINFCityBase(CAtumNode* pParent)" );
	m_pParent = pParent;
	int i;
	for(i=0;i<9;i++)
	{
		m_pNPCBoxImage[i] = NULL;
	}
	m_pDownBoxImage[0]= NULL;
	m_pDownBoxImage[1]= NULL;
	m_pUpBoxImage[0] = NULL;
	m_pUpBoxImage[1] = NULL;
	memset( m_pNPCButtonImage, 0x00, NPC_BUTTON_NUMBER*CITY_BUTTON_NUMBER*sizeof(DWORD));
	for(i=0;i<3;i++)
	{
		m_nNPCButtonState[i] = BUTTON_STATE_NORMAL;
	}
	for(i=0;i<CITY_BUTTON_NUMBER;i++)
	{
		m_pOutDoorButtonImage[i] = NULL;
	}
	m_nOutDoorButtonState = BUTTON_STATE_NORMAL;
	for(i = 0 ; i < CITY_BUTTON_NUMBER ; i++)
	{
		m_pImgCreateB[i] = NULL;
		m_pImgAppB[i] = NULL;
		m_pImgQuickAppB[i] = NULL;
	}
	m_nCreateB				= BUTTON_STATE_NORMAL;
	m_nAppB					= BUTTON_STATE_NORMAL;
	m_nQuickAppB			= BUTTON_STATE_NORMAL;
	m_bRestored = FALSE;
	m_pCurrentBuildingNPC = NULL;
	m_nCurrentBuildingNPCState = 0;
	m_nCurrentEnterBuildingIndex = -1;
//	memset( m_strNPCTalk, 0x00, CITY_NPCTALK_LINE_STRING_NUMBER*CITY_NPCTALK_SIZE);
	m_pMission = NULL;
	for(i=0;i<CITY_NPCTALK_LINE_FONT_NUMBER;i++)
	{
		m_pFontNPCTalk[i] = NULL;
	}
	m_nNextEnterBuilding = -1;


	m_pDataHeader = NULL;
	m_pNPCImage = NULL;
	m_pNPCScroll = NULL;
	m_fCurrentShowTimeTime = 0.0f;
	memset(m_strGuildMasterGr, 0x00, 512);

	// 2005-02-21 by jschoi
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	m_pRenewTirShopImage = NULL;							// 2011-12-08 by jhahn EP4 트리거 상점 UI 변경
	m_pRenewShopImage = NULL;
	m_pRenewShopRefinery = NULL;
	m_pRenewShopSkill = NULL;
	m_pRenewInfinityImage = NULL;
	m_pRenewCityStore = NULL;	
#else
	m_pWebTitleBarImage = NULL;
#endif

	// 2012-02-29 by mspark, 상점 타이틀 넣기
	m_pImgTitleWeapon = NULL;
	m_pImgTitleParts = NULL;
	m_pImgTitleGear = NULL;
	m_pImgTitleCard = NULL;
	m_pImgTitleWarPoint = NULL;
	m_pImgTitleLab = NULL;
	m_pImgTitleFactory = NULL;
	// end 2012-02-29 by mspark, 상점 타이틀 넣기

	// 2008-03-27 by bhsohn 미션성공창이 떠있는 상태에서는 상점이 안열리게 변경
	m_bForceClose = FALSE;	// 창 강제 종료

	// 2012-10-31 by jhjang 포인터 초기화 코드 추가
	m_pRenewCityWarPointStore = NULL;
	// end 2012-10-31 by jhjang 포인터 초기화 코드 추가
}

CINFCityBase::~CINFCityBase()
{
	FLOG( "CINFCityBase::~CINFCityBase()" );
	int i;
	for(i=0;i<9;i++)
	{
		util::del(m_pNPCBoxImage[i]);
	}
	for(i=0;i<4;i++)
	{
		util::del( m_pOutDoorButtonImage[i] );
	}
	for(i=0;i<NPC_BUTTON_NUMBER;i++)
	{
		for(int j=0;j<CITY_BUTTON_NUMBER;j++)
		{
			util::del( m_pNPCButtonImage[i][j] );
		}
	}
	util::del(m_pDownBoxImage[0]);
	util::del(m_pDownBoxImage[1]);
	util::del(m_pUpBoxImage[0]);
	util::del(m_pUpBoxImage[1]);
	util::del(m_pMission);
	for(i=0;i<CITY_NPCTALK_LINE_FONT_NUMBER;i++)
	{
		util::del(m_pFontNPCTalk[i]);
	}

	util::del(m_pDataHeader);
	util::del(m_pNPCImage);

	// 2005-02-21 by jschoi
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	util::del(m_pRenewTirShopImage);						//2011-12-08 by jhahn EP4 트리거 상점 UI 변경
    util::del( m_pRenewShopRefinery );
	util::del( m_pRenewInfinityImage );
	util::del( m_pRenewCityStore );	
	util::del( m_pRenewShopSkill );
	util::del( m_pRenewShopImage );
#else
	util::del(m_pWebTitleBarImage);
#endif
	util::del(m_pRenewShopImage);
	
	// 2012-02-29 by mspark, 상점 타이틀 넣기
	util::del(m_pImgTitleWeapon);
	util::del(m_pImgTitleParts);
	util::del(m_pImgTitleGear);
	util::del(m_pImgTitleCard);
	util::del(m_pImgTitleWarPoint);
	util::del(m_pImgTitleLab);
	util::del(m_pImgTitleFactory);
	// end 2012-02-29 by mspark, 상점 타이틀 넣기
	
}

HRESULT CINFCityBase::InitDeviceObjects()
{
	FLOG( "CINFCityBase::InitDeviceObjects()" );
	DataHeader* pDataHeader = NULL;
	int i;
	for(i=0;i<3;i++)
	{
		for(int j=0;j<3;j++)
		{
			char buf[12];
			wsprintf(buf, "sh_npc%d%d",i,j);
			m_pNPCBoxImage[i*3+j] = new CINFImageEx;
			pDataHeader = FindResource(buf);
			m_pNPCBoxImage[i*3+j]->InitDeviceObjects(pDataHeader );
		}
	}
// 2006-06-08 by ispark, 쓰지 않는 버튼이므로 주석 처리 했다. 아직 지우지 말것
//	for(i=0;i<CITY_BUTTON_NUMBER;i++)
//	{
//		char buf[12];
//		wsprintf(buf, "shbtn99%d", i);
//		m_pOutDoorButtonImage[i] = new CINFImage;
//		pDataHeader = FindResource( buf );
//		m_pOutDoorButtonImage[i]->InitDeviceObjects(pDataHeader->m_pData, pDataHeader->m_DataSize);
//	}
	m_pUpBoxImage[0] = new CINFImageEx;
	pDataHeader = FindResource("sh_up0");//1x2
	m_pUpBoxImage[0]->InitDeviceObjects(pDataHeader );
	m_pUpBoxImage[1] = new CINFImageEx;
	pDataHeader = FindResource("sh_up1");//1x1
	m_pUpBoxImage[1]->InitDeviceObjects(pDataHeader );

	m_pDownBoxImage[0] = new CINFImageEx;
	pDataHeader = FindResource("sh_base0");//1x2
	m_pDownBoxImage[0]->InitDeviceObjects(pDataHeader );
	m_pDownBoxImage[1] = new CINFImageEx;
	pDataHeader = FindResource("sh_base1");//1x1
	m_pDownBoxImage[1]->InitDeviceObjects(pDataHeader );

	for(i=0;i<NPC_BUTTON_NUMBER;i++)
	{
		for(int j=0;j<CITY_BUTTON_NUMBER;j++)
		{
			char buf[12];
			wsprintf(buf, "shnpc%02d%d",i,j);
			m_pNPCButtonImage[i][j] = new CINFImageEx;
			pDataHeader = FindResource(buf);
			m_pNPCButtonImage[i][j]->InitDeviceObjects(pDataHeader );
		}
	}
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	m_pNPCBoxImageEP = new CINFImageEx;	
	pDataHeader = FindResource("shop_int");
	m_pNPCBoxImageEP->InitDeviceObjects(pDataHeader );											  
#endif

	for(i = 0 ; i < CITY_BUTTON_NUMBER ; i++)
	{
		char buf[12];
		wsprintf(buf, "createb%d",i);
		m_pImgCreateB[i] = new CINFImageEx;
		pDataHeader = FindResource(buf);
		m_pImgCreateB[i]->InitDeviceObjects(pDataHeader );

		wsprintf(buf,"appb%d",i);
		m_pImgAppB[i] = new CINFImageEx;
		pDataHeader = FindResource(buf);
		m_pImgAppB[i]->InitDeviceObjects(pDataHeader );

		wsprintf(buf,"hurrytm%d",i);
		m_pImgQuickAppB[i] = new CINFImageEx;
		pDataHeader = FindResource(buf);
		m_pImgQuickAppB[i]->InitDeviceObjects(pDataHeader );

		wsprintf(buf,"canb%d",i);
		m_pImgCancelB[i] = new CINFImageEx;
		pDataHeader = FindResource(buf);
		m_pImgCancelB[i]->InitDeviceObjects(pDataHeader );
	}

	for(i=0;i<CITY_NPCTALK_LINE_FONT_NUMBER;i++)
	{
		m_pFontNPCTalk[i] = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9, D3DFONT_ZENABLE,  FALSE,1024,32);
		m_pFontNPCTalk[i]->InitDeviceObjects(g_pD3dDev);
	}
	// BUTTON_GROUP 초기화 : 게임 수행중에 받기때문에 Init과 Restore는 따로 하지 않는다.
//	CMapCityGroupIterator it = m_mapButtonGroup.begin();
//	while(it != m_mapButtonGroup.end())
//	{
//		for(int i=0;i<CITY_BUTTON_NUMBER;i++)
//		{
//			if((it->second)->m_pButton[i])
//			{
//				(it->second)->m_pButton[i]->InvalidateDeviceObjects();
//				(it->second)->m_pButton[i]->DeleteDeviceObjects();
//				util::del((it->second)->m_pButton[i]);
//			}
//		}
//		util::del(it->second);
//		it++;
//	}
	vector<GUI_BUILDINGNPC*>::iterator it = m_vecButtonGroup.begin();
	while(it != m_vecButtonGroup.end())
	{
		for(int i=0;i<CITY_BUTTON_NUMBER;i++)
		{
			if((*it)->m_pButton[i])
			{
				(*it)->m_pButton[i]->InvalidateDeviceObjects();
				(*it)->m_pButton[i]->DeleteDeviceObjects();
				util::del((*it)->m_pButton[i]);
			}
		}
		util::del(*it);
		it++;
	}
//	m_mapButtonGroup.clear();
	m_vecButtonGroup.clear();
	m_mapButtonState.clear();
	CMapCityShopIterator itShop = m_mapCityShop.begin();
	while(itShop != m_mapCityShop.end())
	{
		itShop->second->InvalidateDeviceObjects();
		itShop->second->DeleteDeviceObjects();
		util::del(itShop->second);
		itShop++;
	}
	m_mapCityShop.clear();
	m_pMission = new CINFCityMission(this);
	m_pMission->SetGameData(g_pGameMain->m_pGameData);
	m_pMission->InitDeviceObjects();


	m_pNPCScroll = new CINFScrollBar(this,
								SCROLL_START_X, 
								SCROLL_START_Y, 
								SCROLL_LINE_LENGTH,
								CITY_NPCTALK_LINE_FONT_NUMBER);
	m_pNPCScroll->SetGameData( m_pGameData );
	m_pNPCScroll->InitDeviceObjects();

	// 2005-02-21 by jschoi
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	if( m_pRenewShopImage == NULL )
	{
		pDataHeader = g_pGameMain->m_GruopImagemanager->FindResource( "shopBG" );
		m_pRenewShopImage = g_pGameMain->m_GruopImagemanager->GetGroupImage( pDataHeader );
		m_pRenewShopImage->InitDeviceObjects( g_pD3dApp->m_pImageList );
	}	
//2011-12-08 by jhahn EP4 트리거 상점 UI 변경
	if( m_pRenewTirShopImage == NULL )
	{
		pDataHeader = g_pGameMain->m_GruopImagemanager->FindResource("tri_sh");
		m_pRenewTirShopImage = g_pGameMain->m_GruopImagemanager->GetGroupImage( pDataHeader );		
		m_pRenewTirShopImage->InitDeviceObjects( g_pD3dApp->m_pImageList );
		

	}
//end 2011-12-08 by jhahn EP4 트리거 상점 UI 변경	
	if( m_pRenewInfinityImage == NULL )
	{
		pDataHeader = g_pGameMain->m_GruopImagemanager->FindResource( "if_shop" );
		m_pRenewInfinityImage = g_pGameMain->m_GruopImagemanager->GetGroupImage( pDataHeader );
		m_pRenewInfinityImage->InitDeviceObjects( g_pD3dApp->m_pImageList );
	}

	if( m_pRenewShopRefinery == NULL )
	{
		pDataHeader = g_pGameMain->m_GruopImagemanager->FindResource( "fueling" );
		m_pRenewShopRefinery = g_pGameMain->m_GruopImagemanager->GetGroupImage( pDataHeader );
		m_pRenewShopRefinery->InitDeviceObjects( g_pD3dApp->m_pImageList );
	}
	
	if( m_pRenewShopSkill == NULL )
	{
		pDataHeader = g_pGameMain->m_GruopImagemanager->FindResource( "skill_sh" );
		m_pRenewShopSkill = g_pGameMain->m_GruopImagemanager->GetGroupImage( pDataHeader );
		m_pRenewShopSkill->InitDeviceObjects( g_pD3dApp->m_pImageList );
	}

	if( m_pRenewCityStore == NULL )
	{
		pDataHeader = g_pGameMain->m_GruopImagemanager->FindResource( "Ware_sh" );
		m_pRenewCityStore = g_pGameMain->m_GruopImagemanager->GetGroupImage( pDataHeader );
		m_pRenewCityStore->InitDeviceObjects( g_pD3dApp->m_pImageList );	
	}
	
	if( m_pRenewCityWarPointStore == NULL )
	{
		pDataHeader = g_pGameMain->m_GruopImagemanager->FindResource( "warpoin_sh" );
		m_pRenewCityWarPointStore = g_pGameMain->m_GruopImagemanager->GetGroupImage( pDataHeader );
		m_pRenewCityWarPointStore->InitDeviceObjects( g_pD3dApp->m_pImageList );	
	}
#else
	m_pWebTitleBarImage = new CINFImageEx;
	pDataHeader = FindResource("webbar");
	m_pWebTitleBarImage->InitDeviceObjects( pDataHeader );
	
	m_pRenewShopImage = new CINFImageEx;
	pDataHeader = FindResource("renewsh");
 	m_pRenewShopImage->InitDeviceObjects( pDataHeader );
#endif

	// 2012-02-29 by mspark, 상점 타이틀 넣기
	m_pImgTitleWeapon = new CINFImageEx;
	pDataHeader = FindResource("weapon_ti");
	if(m_pImgTitleWeapon)
	{
		m_pImgTitleWeapon->InitDeviceObjects(pDataHeader);
	}	
	
	m_pImgTitleParts = new CINFImageEx;
	pDataHeader = FindResource("parts_ti");
	if(m_pImgTitleParts)
	{
		m_pImgTitleParts->InitDeviceObjects(pDataHeader);
	}	

	m_pImgTitleGear = new CINFImageEx;
	pDataHeader = FindResource("gear_ti");
	if(m_pImgTitleGear)
	{
		m_pImgTitleGear->InitDeviceObjects(pDataHeader);
	}	
	
	m_pImgTitleCard = new CINFImageEx;
	pDataHeader = FindResource("card_ti");
	if(m_pImgTitleCard)
	{
		m_pImgTitleCard->InitDeviceObjects(pDataHeader);
	}	

	m_pImgTitleWarPoint = new CINFImageEx;
	pDataHeader = FindResource("point_ti");
	if(m_pImgTitleWarPoint)
	{
		m_pImgTitleWarPoint->InitDeviceObjects(pDataHeader);
	}	

	m_pImgTitleLab = new CINFImageEx;
	pDataHeader = FindResource("lab_ti");
	if(m_pImgTitleLab)
	{
		m_pImgTitleLab->InitDeviceObjects(pDataHeader);
	}	
	
	m_pImgTitleFactory = new CINFImageEx;
	pDataHeader = FindResource("fac_ti");
	if(m_pImgTitleFactory)
	{
		m_pImgTitleFactory->InitDeviceObjects(pDataHeader);
	}	
	// end 2012-02-29 by mspark, 상점 타이틀 넣기

	return S_OK;
}
HRESULT CINFCityBase::RestoreDeviceObjects()
{
	FLOG( "CINFCityBase::RestoreDeviceObjects()" );
	if(!m_bRestored)
	{
		int i;
		for(i=0;i<9;i++)
		{
			m_pNPCBoxImage[i]->RestoreDeviceObjects();
		}
// 2006-06-08 by ispark
//		for(i=0;i<CITY_BUTTON_NUMBER;i++)
//		{
//			m_pOutDoorButtonImage[i]->RestoreDeviceObjects();
//		}
		m_pUpBoxImage[0]->RestoreDeviceObjects();
		m_pUpBoxImage[1]->RestoreDeviceObjects();
		m_pDownBoxImage[0]->RestoreDeviceObjects();
		m_pDownBoxImage[1]->RestoreDeviceObjects();
		for(i=0;i<NPC_BUTTON_NUMBER;i++)
		{
			for(int j=0;j<CITY_BUTTON_NUMBER;j++)
			{
				m_pNPCButtonImage[i][j]->RestoreDeviceObjects();
			}
		}
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
		m_pNPCBoxImageEP->RestoreDeviceObjects();												  
#endif

		for(i = 0 ; i < CITY_BUTTON_NUMBER ; i++)
		{
			m_pImgCreateB[i]->RestoreDeviceObjects();
			m_pImgAppB[i]->RestoreDeviceObjects();
			m_pImgCancelB[i]->RestoreDeviceObjects();
			m_pImgQuickAppB[i]->RestoreDeviceObjects();
		}

		for(i=0;i<CITY_NPCTALK_LINE_FONT_NUMBER;i++)
		{
			m_pFontNPCTalk[i]->RestoreDeviceObjects();
		}
		// ALT_TAB과 같이 나갔다 들어오는 경우에 처리된다.
		vector<GUI_BUILDINGNPC*>::iterator it = m_vecButtonGroup.begin();
		while(it != m_vecButtonGroup.end())
		{
			for(int i=0;i<CITY_BUTTON_NUMBER;i++)
			{
				if((*it)->m_pButton[i])
				{
					(*it)->m_pButton[i]->RestoreDeviceObjects();
				}
			}
			it++;
		}
//		CMapCityGroupIterator it = m_mapButtonGroup.begin();
//		while(it != m_mapButtonGroup.end())
//		{
//			for(int i=0;i<CITY_BUTTON_NUMBER;i++)
//			{
//				if((it->second)->m_pButton[i])
//				{
//					(it->second)->m_pButton[i]->RestoreDeviceObjects();
//				}
//			}
//			it++;
//		}
		CMapCityShopIterator itShop = m_mapCityShop.begin();
		while(itShop != m_mapCityShop.end())
		{
			itShop->second->RestoreDeviceObjects();
			itShop++;
		}
		m_pMission->RestoreDeviceObjects();


		m_pNPCScroll->RestoreDeviceObjects();
		m_pNPCScroll->SetWheelRect(CITY_BASE_NPC_TEXT_START_X, 
			CITY_BASE_NPC_TEXT_START_Y,
			CITY_BASE_NPC_TEXT_START_X+CITY_BASE_NPC_TEXT_SIZE_X,
			CITY_BASE_NPC_TEXT_START_Y+CITY_BASE_NPC_TEXT_LINE_INTERVAL*CITY_NPCTALK_LINE_FONT_NUMBER);



		if(m_pNPCImage)
		{
			m_pNPCImage->RestoreDeviceObjects();
		}

		// 2005-02-21 by jschoi
#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
		if(m_pWebTitleBarImage)
		{
			m_pWebTitleBarImage->RestoreDeviceObjects();
		}
		
#endif

		// city fog setting
//		g_pScene->SetFogLevel(CITY_FOG_COLOR, CITY_FOG_START, CITY_FOG_END );
//		g_pScene->SetFogLevel(GetFogColor(g_pShuttleChild->m_myShuttleInfo.MapChannelIndex.MapIndex,TRUE), CITY_FOG_START, CITY_FOG_END );

		m_bRestored = TRUE;
	}
	

	if(m_pRenewShopImage)
	{
		m_pRenewShopImage->RestoreDeviceObjects();
	}
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	if(m_pRenewShopRefinery)
	{
		m_pRenewShopRefinery->RestoreDeviceObjects();
	}

	if(m_pRenewShopSkill)
	{
		m_pRenewShopSkill->RestoreDeviceObjects();
	}

	//2011-12-08 by jhahn EP4 트리거 상점 UI 변경	
	
	if(m_pRenewTirShopImage)
	{
		m_pRenewTirShopImage->RestoreDeviceObjects();
	}	
//end 2011-12-08 by jhahn EP4 트리거 상점 UI 변경
	if(m_pRenewInfinityImage)
	{
		m_pRenewInfinityImage->RestoreDeviceObjects();
	}
	if( m_pRenewCityStore )
	{
		m_pRenewCityStore->RestoreDeviceObjects();
	}
	if( m_pRenewCityWarPointStore )
	{
		m_pRenewCityWarPointStore->RestoreDeviceObjects();
	}
#endif
	
	// 2012-02-29 by mspark, 상점 타이틀 넣기
	if(m_pImgTitleWeapon)
	{
		m_pImgTitleWeapon->RestoreDeviceObjects();
		m_pImgTitleWeapon->Move(TITLE_SHOP_START_X, TITLE_SHOP_START_Y);	 
	}	
	
	if(m_pImgTitleParts)
	{
		m_pImgTitleParts->RestoreDeviceObjects();
		m_pImgTitleParts->Move(TITLE_SHOP_START_X, TITLE_SHOP_START_Y);	 
	}	

	if(m_pImgTitleGear)
	{
		m_pImgTitleGear->RestoreDeviceObjects();
		m_pImgTitleGear->Move(TITLE_SHOP_START_X, TITLE_SHOP_START_Y);	 
	}	
	
	if(m_pImgTitleCard)
	{
		m_pImgTitleCard->RestoreDeviceObjects();
		m_pImgTitleCard->Move(TITLE_SHOP_START_X, TITLE_SHOP_START_Y);	 
	}	

	if(m_pImgTitleWarPoint)
	{
		m_pImgTitleWarPoint->RestoreDeviceObjects();
		m_pImgTitleWarPoint->Move(TITLE_SHOP_START_X, TITLE_SHOP_START_Y);	 
	}	

	if(m_pImgTitleLab)
	{
		m_pImgTitleLab->RestoreDeviceObjects();
		m_pImgTitleLab->Move(TITLE_LAP_START_X, TITLE_LAP_START_Y);	 
	}	
	
	if(m_pImgTitleFactory)
	{
		m_pImgTitleFactory->RestoreDeviceObjects();
		m_pImgTitleFactory->Move(TITLE_LAP_START_X, TITLE_LAP_START_Y);	 
	}	
	// end 2012-02-29 by mspark, 상점 타이틀 넣기
	
	return S_OK;
}

HRESULT CINFCityBase::InvalidateDeviceObjects()
{
	FLOG( "CINFCityBase::InvalidateDeviceObjects()" );
	if(m_bRestored)
	{
		int i;
		for(i=0;i<9;i++)
		{
			m_pNPCBoxImage[i]->InvalidateDeviceObjects();
		}
// 2006-06-08 by ispark	
//		for(i=0;i<CITY_BUTTON_NUMBER;i++)
//		{
//			m_pOutDoorButtonImage[i]->InvalidateDeviceObjects();
//		}
		m_pUpBoxImage[0]->InvalidateDeviceObjects();
		m_pUpBoxImage[1]->InvalidateDeviceObjects();
		m_pDownBoxImage[0]->InvalidateDeviceObjects();
		m_pDownBoxImage[1]->InvalidateDeviceObjects();
		for(i=0;i<NPC_BUTTON_NUMBER;i++)
		{
			for(int j=0;j<CITY_BUTTON_NUMBER;j++)
			{
				m_pNPCButtonImage[i][j]->InvalidateDeviceObjects();
			}
		}

#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
		m_pNPCBoxImageEP->InvalidateDeviceObjects();											  
#endif
		for(i = 0 ;i < CITY_BUTTON_NUMBER ; i++)
		{
			m_pImgCreateB[i]->InvalidateDeviceObjects();
			m_pImgAppB[i]->InvalidateDeviceObjects();
			m_pImgCancelB[i]->InvalidateDeviceObjects();
			m_pImgQuickAppB[i]->InvalidateDeviceObjects();
		}

		for(i=0;i<CITY_NPCTALK_LINE_FONT_NUMBER;i++)
		{
			m_pFontNPCTalk[i]->InvalidateDeviceObjects();
		}

		vector<GUI_BUILDINGNPC*>::iterator it = m_vecButtonGroup.begin();
		while(it != m_vecButtonGroup.end())
		{
			for(int i=0;i<CITY_BUTTON_NUMBER;i++)
			{
				if((*it)->m_pButton[i])
				{
					(*it)->m_pButton[i]->InvalidateDeviceObjects();
				}
			}
			it++;
		}
//		CMapCityGroupIterator it = m_mapButtonGroup.begin();
//		while(it != m_mapButtonGroup.end())
//		{
//			for(int i=0;i<CITY_BUTTON_NUMBER;i++)
//			{
//				if((it->second)->m_pButton[i])
//				{
//					(it->second)->m_pButton[i]->InvalidateDeviceObjects();
//				}
//			}
//			it++;
//		}
		CMapCityShopIterator itShop = m_mapCityShop.begin();
		while(itShop != m_mapCityShop.end())
		{
			itShop->second->InvalidateDeviceObjects();
			itShop++;
		}
		m_pMission->InvalidateDeviceObjects();


		m_pNPCScroll->InvalidateDeviceObjects();


		if(m_pNPCImage)
		{
			m_pNPCImage->InvalidateDeviceObjects();
		}


		// 2005-02-21 by jschoi
#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
		if(m_pWebTitleBarImage)
		{
			m_pWebTitleBarImage->InvalidateDeviceObjects();
		}
	
		
#endif
		m_bRestored = FALSE;
	}
	if(m_pRenewShopImage)
	{
		m_pRenewShopImage->InvalidateDeviceObjects();
	}
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	if(m_pRenewShopRefinery)
	{
		m_pRenewShopRefinery->InvalidateDeviceObjects();
	}

	if(m_pRenewShopSkill)
	{
		m_pRenewShopSkill->InvalidateDeviceObjects();
	}																							  
	//2011-12-08 by jhahn EP4 트리거 상점 UI 변경
	if(m_pRenewTirShopImage)
	{
		m_pRenewTirShopImage->InvalidateDeviceObjects();
	}
	//end 2011-12-08 by jhahn EP4 트리거 상점 UI 변경
	if(m_pRenewInfinityImage)
	{
		m_pRenewInfinityImage->InvalidateDeviceObjects();
	}
	if(m_pRenewCityStore)
	{
		m_pRenewCityStore->InvalidateDeviceObjects();
	}
	if( m_pRenewCityWarPointStore )
	{
		m_pRenewCityWarPointStore->InvalidateDeviceObjects();
	}

#endif

	// 2012-02-29 by mspark, 상점 타이틀 넣기
	if(m_pImgTitleWeapon)
	{
		m_pImgTitleWeapon->InvalidateDeviceObjects();
	}	
	
	if(m_pImgTitleParts)
	{
		m_pImgTitleParts->InvalidateDeviceObjects();
	}	

	if(m_pImgTitleGear)
	{
		m_pImgTitleGear->InvalidateDeviceObjects();
	}	
	
	if(m_pImgTitleCard)
	{
		m_pImgTitleCard->InvalidateDeviceObjects();
	}	

	if(m_pImgTitleWarPoint)
	{
		m_pImgTitleWarPoint->InvalidateDeviceObjects();
	}

	if(m_pImgTitleLab)
	{
		m_pImgTitleLab->InvalidateDeviceObjects();
	}	
	
	if(m_pImgTitleFactory)
	{
		m_pImgTitleFactory->InvalidateDeviceObjects();
	}	
	// end 2012-02-29 by mspark, 상점 타이틀 넣기

	return S_OK;
}

HRESULT CINFCityBase::DeleteDeviceObjects()
{
	FLOG( "CINFCityBase::DeleteDeviceObjects()" );
	int i;
	for(i=0;i<9;i++)
	{
		m_pNPCBoxImage[i]->DeleteDeviceObjects();
		util::del(m_pNPCBoxImage[i] ) ;
	}
// 2006-06-08 by ispark
//	for(i=0;i<CITY_BUTTON_NUMBER;i++)
//	{
//		m_pOutDoorButtonImage[i]->DeleteDeviceObjects();
//		util::del(m_pOutDoorButtonImage[i] ) ;
//	}
	m_pUpBoxImage[0]->DeleteDeviceObjects();
	util::del(m_pUpBoxImage[0] ) ;
	m_pUpBoxImage[1]->DeleteDeviceObjects();
	util::del(m_pUpBoxImage[1] ) ;
	m_pDownBoxImage[0]->DeleteDeviceObjects();
	util::del(m_pDownBoxImage[0] ) ;
	m_pDownBoxImage[1]->DeleteDeviceObjects();
	util::del(m_pDownBoxImage[1] ) ;
	for(i=0;i<NPC_BUTTON_NUMBER;i++)
	{
		for(int j=0;j<CITY_BUTTON_NUMBER;j++)
		{
			m_pNPCButtonImage[i][j]->DeleteDeviceObjects();
			util::del( m_pNPCButtonImage[i][j] ) ;
		}
	}
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	m_pNPCBoxImageEP->DeleteDeviceObjects();
	util::del( m_pNPCBoxImageEP) ;
#endif
	
	for(i = 0 ; i < CITY_BUTTON_NUMBER ; i++)
	{
		m_pImgCreateB[i]->DeleteDeviceObjects();
		m_pImgAppB[i]->DeleteDeviceObjects();
		m_pImgCancelB[i]->DeleteDeviceObjects();
		m_pImgQuickAppB[i]->DeleteDeviceObjects();
		util::del(m_pImgCreateB[i]);
		util::del(m_pImgAppB[i]);
		util::del(m_pImgCancelB[i]);
		util::del(m_pImgQuickAppB[i]);

	}
	for(i=0;i<CITY_NPCTALK_LINE_FONT_NUMBER;i++)
	{
		m_pFontNPCTalk[i]->DeleteDeviceObjects();
		util::del(m_pFontNPCTalk[i] ) ;
	}

	vector<GUI_BUILDINGNPC*>::iterator it = m_vecButtonGroup.begin();
	while(it != m_vecButtonGroup.end())
	{
		for(int i=0;i<CITY_BUTTON_NUMBER;i++)
		{
			if((*it)->m_pButton[i])
			{
				(*it)->m_pButton[i]->DeleteDeviceObjects();
				util::del((*it)->m_pButton[i]);
			}
		}
		util::del(*it);
		it++;
	}
	m_vecButtonGroup.clear();
//	CMapCityGroupIterator it = m_mapButtonGroup.begin();
//	while(it != m_mapButtonGroup.end())
//	{
//		for(int i=0;i<CITY_BUTTON_NUMBER;i++)
//		{
//			if((it->second)->m_pButton[i])
//			{
//				(it->second)->m_pButton[i]->DeleteDeviceObjects();
//				util::del((it->second)->m_pButton[i]);
//			}
//		}
//		util::del(it->second);
//		it++;
//	}
//	m_mapButtonGroup.clear();
	m_mapButtonState.clear();

	CMapCityShopIterator itShop = m_mapCityShop.begin();
	while(itShop != m_mapCityShop.end())
	{
		itShop->second->DeleteDeviceObjects();
		util::del(itShop->second);
		itShop++;
	}
	m_mapCityShop.clear();
	m_pMission->DeleteDeviceObjects();
	util::del(m_pMission ) ;

	m_pNPCScroll->DeleteDeviceObjects();
	util::del(m_pNPCScroll);

	if(m_pNPCImage)
	{
		m_pNPCImage->DeleteDeviceObjects();
		util::del(m_pNPCImage ) ;
	}

	// 2005-02-21 by jschoi
#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	if(m_pWebTitleBarImage)
	{
		m_pWebTitleBarImage->DeleteDeviceObjects();
		util::del(m_pWebTitleBarImage);
	}

	
#endif
	
	if(m_pRenewShopImage)
	{
		m_pRenewShopImage->DeleteDeviceObjects();
		util::del(m_pRenewShopImage);
	}
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
//2011-12-08 by jhahn EP4 트리거 상점 UI 변경	
	if(m_pRenewTirShopImage)
	{
		m_pRenewTirShopImage->DeleteDeviceObjects();
		util::del(m_pRenewTirShopImage);
 	}
//end 2011-12-08 by jhahn EP4 트리거 상점 UI 변경

	if(m_pRenewInfinityImage)
	{
		m_pRenewInfinityImage->DeleteDeviceObjects();
		util::del(m_pRenewInfinityImage);
	}
	if(m_pRenewShopRefinery)
	{
		m_pRenewShopRefinery->DeleteDeviceObjects();
		util::del(m_pRenewShopRefinery);
	}

	if(m_pRenewShopSkill)
	{
		m_pRenewShopSkill->DeleteDeviceObjects();
		util::del(m_pRenewShopSkill);
	}
	//2011-12-08 by jhahn EP4 트리거 상점 UI 변경	
	if(m_pRenewTirShopImage)
	{
		m_pRenewTirShopImage->DeleteDeviceObjects();
		util::del(m_pRenewTirShopImage);
	}
	//end 2011-12-08 by jhahn EP4 트리거 상점 UI 변경
	if(m_pRenewCityStore)
	{
		m_pRenewCityStore->DeleteDeviceObjects();
		util::del(m_pRenewCityStore);
	}
	if( m_pRenewCityWarPointStore )
	{
		m_pRenewCityWarPointStore->DeleteDeviceObjects();
		util::del(m_pRenewCityWarPointStore);
	}

#endif

	// 2012-02-29 by mspark, 상점 타이틀 넣기
	if(m_pImgTitleWeapon)
	{
		m_pImgTitleWeapon->DeleteDeviceObjects();
		util::del(m_pImgTitleWeapon);
	}
	
	if(m_pImgTitleParts)
	{
		m_pImgTitleParts->DeleteDeviceObjects();
		util::del(m_pImgTitleParts);
	}

	if(m_pImgTitleGear)
	{
		m_pImgTitleGear->DeleteDeviceObjects();
		util::del(m_pImgTitleGear);
	}
	
	if(m_pImgTitleCard)
	{
		m_pImgTitleCard->DeleteDeviceObjects();
		util::del(m_pImgTitleCard);
	}

	if(m_pImgTitleWarPoint)
	{
		m_pImgTitleWarPoint->DeleteDeviceObjects();
		util::del(m_pImgTitleWarPoint);
	}

	if(m_pImgTitleLab)
	{
		m_pImgTitleLab->DeleteDeviceObjects();
		util::del(m_pImgTitleLab);
	}
	
	if(m_pImgTitleFactory)
	{
		m_pImgTitleFactory->DeleteDeviceObjects();
		util::del(m_pImgTitleFactory);
	}
	// end 2012-02-29 by mspark, 상점 타이틀 넣기

	return S_OK;
}

void CINFCityBase::Tick()
{
	FLOG( "CINFCityBase::Tick()" );
	if(m_pCurrentBuildingNPC == NULL)
	{
		return;
	}
	CMapCityShopIterator it = m_mapCityShop.find(m_pCurrentBuildingNPC->buildingInfo.BuildingKind);
	if(it != m_mapCityShop.end())
	{
		switch(m_pCurrentBuildingNPC->buildingInfo.BuildingKind)
		{
		case BUILDINGKIND_CASH_SHOP:
			{
				if(
					g_pD3dApp->m_dwGameState == _CITY ||
					g_pD3dApp->m_dwGameState == _SHOP ||
					g_pD3dApp->m_dwGameState == _GAME)
				{
					it->second->Tick();
				}
			}
			break;
// 2008-11-04 by dgwoo LuckyMachine기획 추가.
		case BUILDINGKIND_LUCKY:
			{
				if(
					g_pD3dApp->m_dwGameState == _CITY ||
					g_pD3dApp->m_dwGameState == _SHOP ||
					g_pD3dApp->m_dwGameState == _GAME)
				{
					it->second->Tick();
				}
			}
			break;
		// 2010. 04. 21 by jskim 신규 럭키 머신 구현
		case BUILDINGKIND_LUCKY_OPTION_MACHINE:
			{
				if(
					g_pD3dApp->m_dwGameState == _CITY ||
					g_pD3dApp->m_dwGameState == _SHOP ||
					g_pD3dApp->m_dwGameState == _GAME)
				{
					it->second->Tick();
				}
			}
			break;
		//end 2010. 04. 21 by jskim 신규 럭키 머신 구현
		// 2010-08-31 by shcho&&jskim, 용해 시스템 구현
		case BUILDINGKIND_DISSOLUTION:
			{
				if(
					g_pD3dApp->m_dwGameState == _CITY ||
					g_pD3dApp->m_dwGameState == _SHOP ||
					g_pD3dApp->m_dwGameState == _GAME)
				{
					it->second->Tick();
				}
			}
			break;
		// end 2010-08-31 by shcho&&jskim, 용해 시스템 구현
		}
	}
	// 2008-03-27 by bhsohn 미션성공창이 떠있는 상태에서는 상점이 안열리게 변경
		// 2008-03-27 by bhsohn 미션성공창이 떠있는 상태에서는 상점이 안열리게 변경	
	if(!g_pD3dApp->IsPossibleShopOpen() && m_bForceClose)
	{
		m_bForceClose = FALSE;
		g_pGameMain->CloseCurrentEnterBuilding();
	}	
	// end 2008-03-27 by bhsohn 미션성공창이 떠있는 상태에서는 상점이 안열리게 변경
}

#define NPC_SHOW_TIME	1.0f
void CINFCityBase::Render()
{
	FLOG( "CINFCityBase::Render()" );
	if(g_pD3dApp->m_dwGameState == _CITY)
	{
		//RenderUpBox();
		//RenderDownBox();
		//////////////////화면 하단 버튼 렌더링//////////////////////////////////////
		vector<GUI_BUILDINGNPC*>::iterator it = m_vecButtonGroup.begin();
		int i=0;
		while(it != m_vecButtonGroup.end())
		{
			CMapButtonStateIterator itState = m_mapButtonState.find(i);
			if(itState == m_mapButtonState.end())
			{
				m_mapButtonState[i] = BUTTON_STATE_DISABLE;
			}
			if(m_mapButtonState[i] != BUTTON_STATE_DISABLE)
			{
				(*it)->m_pButton[m_mapButtonState[i]]->
					Move(CITY_BASE_DOWN_BUTTON_START_X(m_vecButtonGroup.size()) + CITY_BASE_NPC_BUTTON_POS_X_INTERVAL*i,
						 CITY_BASE_DOWN_BUTTON_START_Y);
				(*it)->m_pButton[m_mapButtonState[i]]->Render();
			}
			i++;
			it++;
		}
		///////////////////화면 하단 '나가기'버튼 렌더링 //////////////////////////////
		// 2006-06-08 by ispark
//		m_pOutDoorButtonImage[m_nOutDoorButtonState]->Move(CITY_BASE_DOWN_OUTDOOR_BUTTON_START_X,
//														   CITY_BASE_DOWN_BUTTON_START_Y);
//		m_pOutDoorButtonImage[m_nOutDoorButtonState]->Render();
	}
	// _CITY, _SHOP
	////////////////// 화면 중하단 NPC 박스 렌더링 ////////////////////////////////

	if( m_pCurrentBuildingNPC && 
		strlen(m_pCurrentBuildingNPC->buildingInfo.BuildingName)>0)// m_pCurrentBulidingNPC->buildingInfo.BuildingName는 서버정보가 왔는지 확인하는 용도이다.
	{
		// 유료 아이템 상점은 NPC 와 버튼들은 안보인다
		if(m_pCurrentBuildingNPC->buildingInfo.BuildingKind != BUILDINGKIND_CASH_SHOP &&
			// 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템
			// 인피니티 npc는 버튼들이 안보인다
			m_pCurrentBuildingNPC->buildingInfo.BuildingKind != BUILDINGKIND_INFINITY &&
			// end 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템
			m_pCurrentBuildingNPC->buildingInfo.BuildingKind != BUILDINGKIND_TUTORIAL)
		{
// 2005-12-06 by ispark, NPC임시 삭제
//			if(m_pNPCImage)
//			{
//				D3DCOLOR dwColor = (D3DCOLOR)((int)(0xFF*m_fCurrentShowTimeTime) << 24 | 0x00FFFFFF);
//				m_pNPCImage->SetColor(dwColor);
//				m_pNPCImage->Move(CITY_BASE_NPC_IMG_START_X, CITY_BASE_NPC_IMG_START_Y);
//				m_pNPCImage->Render();
//				m_fCurrentShowTimeTime += g_pD3dApp->GetElapsedTime();
//				if(m_fCurrentShowTimeTime > NPC_SHOW_TIME)
//				{
//					m_fCurrentShowTimeTime = NPC_SHOW_TIME;
//				}
//			}
			// RenderNPCBox
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
			RenderNPCBox(CITY_BASE_NPC_BOX_START_X, CITY_BASE_NPC_BOX_START_Y - 525, 			  
#else								  
			RenderNPCBox(CITY_BASE_NPC_BOX_START_X, CITY_BASE_NPC_BOX_START_Y, 
#endif
				CITY_BASE_NPC_BOX_SIZE_X, CITY_BASE_NPC_BOX_SIZE_Y);
			RenderNPCButton(m_pCurrentBuildingNPC);
			// RenderText
			int nNPCScrollIndex = m_pNPCScroll->GetCurrentScrollIndex();
			if(strlen(m_strGuildMasterGr)>0 && m_pCurrentBuildingNPC->buildingInfo.BuildingKind == BUILDINGKIND_CITY_OCCUPY_INFO)
			{
				SetNPCTalkInfo( m_strGuildMasterGr );

			}
			for(int i=0;i<CITY_NPCTALK_LINE_FONT_NUMBER;i++)
			{
				if( i+nNPCScrollIndex < m_vecNPCTalk.size() )
				{
					m_pFontNPCTalk[i]->DrawText(CITY_BASE_NPC_TEXT_START_X, 
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
						(CITY_BASE_NPC_TEXT_START_Y - 525 )+CITY_BASE_NPC_TEXT_LINE_INTERVAL*i, 
#else
						CITY_BASE_NPC_TEXT_START_Y+CITY_BASE_NPC_TEXT_LINE_INTERVAL*i, 
#endif
						GUI_FONT_COLOR, (LPTSTR)(LPCTSTR)m_vecNPCTalk[i+nNPCScrollIndex].c_str(), 0L);
				}
				else
				{
					break;
				}
			}
			if(m_vecNPCTalk.size() > CITY_NPCTALK_LINE_FONT_NUMBER)
			{
				m_pNPCScroll->Render();
			}
		}
		////////////////// 화면 중단 SHOP, MISSION 박스 렌더링 ////////////////////////
		switch(m_nCurrentBuildingNPCState)
		{
		case BUILDINGNPC_RENDER_TALKBOX:
			break;
		case BUILDINGNPC_RENDER_ARENA:
			{
#ifdef C_EPSODE4_UI_CHANGE_JSKIM
#else
				m_pImgCreateB[m_nCreateB]->Move(CITY_BASE_NPC_CREATE_BUTTON_X,CITY_BASE_NPC_CREATE_BUTTON_Y);
				m_pImgCreateB[m_nCreateB]->Render();
				m_pImgAppB[m_nAppB]->Move(CITY_BASE_NPC_APP_BUTTON_X,CITY_BASE_NPC_APP_BUTTON_Y);
				m_pImgAppB[m_nAppB]->Render();
				m_pImgCancelB[m_nCancelB]->Move(CITY_BASE_NPC_CANCEL_BUTTON_X,CITY_BASE_NPC_CANCEL_BUTTON_Y);
				m_pImgCancelB[m_nCancelB]->Render();
				m_pImgQuickAppB[m_nQuickAppB]->Move(CITY_BASE_NPC_QUICKAPP_BUTTON_X,CITY_BASE_NPC_QUICKAPP_BUTTON_Y);
				m_pImgQuickAppB[m_nQuickAppB]->Render();
#endif
			}// RenderBuildingNPCShop(); 들어가서 렌더해야한다.
		case BUILDINGNPC_RENDER_SHOP:
		case BUILDINGNPC_RENDER_INFLUENCE:
		case BUILDINGNPC_RENDER_WARPOINT:
		case BUILDINGNPC_RENDER_LUCKYMACHINE:// 2008-11-04 by dgwoo LuckyMachine기획 추가.
		case BUILDINGNPC_RENDER_OPTIONMACHINE: // 2010. 04. 21 by jskim 신규 럭키 머신 구현
		case BUILDINGKIND_RENDER_DISSOLUTION:		// 2010-08-31 by shcho&&jskim, 용해 시스템 구현
  	    // 2012-02-03 by jskim 팩토리 더블클릭 구현 
		case BUILDINGNPC_RENDER_LABORATORY:
		case BUILDINGNPC_RENDER_FACTORY:
   	    // end 2012-02-03 by jskim 팩토리 더블클릭 구현 
			{
				RenderBuildingNPCShop();
			}
			break;
		case BUILDINGNPC_RENDER_MISSION:
			{
				g_pGameMain->m_pInfWindow->RenderCenterWindow(CITY_BASE_NPC_BOX_START_X,
							 CITY_BASE_NPC_BOX_START_Y - CITY_BASE_NPC_MISSION_SIZE_Y,
							 CITY_BASE_NPC_MISSION_SIZE_X, 
							 CITY_BASE_NPC_MISSION_SIZE_Y,FALSE);
				m_pMission->Render();
			}
			break;
		case BUILDINGNPC_RENDER_OCCUPY:
			{
				RenderBuildingNPCShop();
			}
			break;
		case BUILDINGNPC_RENDER_CASH_SHOP:
			{
				// 2009-02-24 by bhsohn 캐쉬샵 충전 웹페이지 추가
// 				if(g_pD3dApp->GetWeb() == FALSE)
// 				{
// 					RenderBuildingNPCShop();
// 				}
// 				else
// 				{
// 					const int nBarHeight = 18;
// 					const int nBarStartX = WEB_START_X;
// 					const int nBarStartY = WEB_START_Y - nBarHeight;
// 					m_pWebTitleBarImage->Move(nBarStartX,nBarStartY);
// 					m_pWebTitleBarImage->Render();
// 				}
				RenderBuildingNPCShop();
				// end 2009-02-24 by bhsohn 캐쉬샵 충전 웹페이지 추가
			}
			break;

		// 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템
		case BUILDINGNPC_RENDER_INFINITYFIELD:
			{
				RenderBuildingKindCityInfinityField();
			}
			break;

		case BUILDINGNPC_RENDER_INFINITYSHOP:
			{
				RenderBuildingKindCItyInfinityShop();
			}
			break;
		//end  2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템
			// 2013-05-07 by bhsohn 세력포인트 개선안 시스템
		case BUILDINGNPC_RENDER_INFLBUFFSHOP:
			{
				RenderBuildingKindCItyInflBuffShop();
			}
			break;
			// END 2013-05-07 by bhsohn 세력포인트 개선안 시스템
		case BUILDINGNPC_RENDER_MSGBOX:
			{

			}
			break;
		}

	}
}

void CINFCityBase::RenderNPCButton(GUI_BUILDINGNPC* pCity)// RenderNPCButton , 0 : CITY_NPC_BUTTON_MAIN, 1 : MISSION, 2 : CLOSE
{
// 2006-05-19 by ispark, 상점 왼쪽 하단 버튼 삭제
	// 첫번째 버튼, 고유 버튼
//	if(IS_ITEM_SHOP_TYPE(pCity->buildingInfo.BuildingKind))
//	{
//		m_pNPCButtonImage[NPC_BUTTON_ITEM_SHOP][m_nNPCButtonState[0]]->Move(CITY_BASE_NPC_BUTTON_START_X, CITY_BASE_NPC_BUTTON_START_Y);
//		m_pNPCButtonImage[NPC_BUTTON_ITEM_SHOP][m_nNPCButtonState[0]]->Render();
//	}
//	else if(IS_STORE_SHOP_TYPE(pCity->buildingInfo.BuildingKind))
//	{
//		m_pNPCButtonImage[NPC_BUTTON_STORE_SHOP][m_nNPCButtonState[0]]->Move(CITY_BASE_NPC_BUTTON_START_X, CITY_BASE_NPC_BUTTON_START_Y);
//		m_pNPCButtonImage[NPC_BUTTON_STORE_SHOP][m_nNPCButtonState[0]]->Render();
//	}
//	else if(IS_WARP_SHOP_TYPE(pCity->buildingInfo.BuildingKind))
//	{
//		m_pNPCButtonImage[NPC_BUTTON_WARP][m_nNPCButtonState[0]]->Move(CITY_BASE_NPC_BUTTON_START_X, CITY_BASE_NPC_BUTTON_START_Y);
//		m_pNPCButtonImage[NPC_BUTTON_WARP][m_nNPCButtonState[0]]->Render();
//	}
//	else if(IS_MISSION_SHOP_TYPE(pCity->buildingInfo.BuildingKind))
//	{
//		m_pNPCButtonImage[NPC_BUTTON_MISSION][m_nNPCButtonState[0]]->Move(CITY_BASE_NPC_BUTTON_START_X, CITY_BASE_NPC_BUTTON_START_Y);
//		m_pNPCButtonImage[NPC_BUTTON_MISSION][m_nNPCButtonState[0]]->Render();
//	}
//	else if(IS_RACING_SHOP_TYPE(pCity->buildingInfo.BuildingKind))
//	{
//		m_pNPCButtonImage[NPC_BUTTON_RACING_SHOP][m_nNPCButtonState[0]]->Move(CITY_BASE_NPC_BUTTON_START_X, CITY_BASE_NPC_BUTTON_START_Y);
//		m_pNPCButtonImage[NPC_BUTTON_RACING_SHOP][m_nNPCButtonState[0]]->Render();
//	}
//	else if(IS_ACTIONHOUSE_SHOP_TYPE(pCity->buildingInfo.BuildingKind))
//	{
//		m_pNPCButtonImage[NPC_BUTTON_ACTIONHOUSE][m_nNPCButtonState[0]]->Move(CITY_BASE_NPC_BUTTON_START_X, CITY_BASE_NPC_BUTTON_START_Y);
//		m_pNPCButtonImage[NPC_BUTTON_ACTIONHOUSE][m_nNPCButtonState[0]]->Render();
//	}
//	else if(IS_LABORATORY_SHOP_TYPE(pCity->buildingInfo.BuildingKind))
//	{
//		m_pNPCButtonImage[NPC_BUTTON_LABORATORY][m_nNPCButtonState[0]]->Move(CITY_BASE_NPC_BUTTON_START_X, CITY_BASE_NPC_BUTTON_START_Y);
//		m_pNPCButtonImage[NPC_BUTTON_LABORATORY][m_nNPCButtonState[0]]->Render();
//	}
//	else if(IS_FACTORY_SHOP_TYPE(pCity->buildingInfo.BuildingKind))
//	{
//		m_pNPCButtonImage[NPC_BUTTON_FACTORY][m_nNPCButtonState[0]]->Move(CITY_BASE_NPC_BUTTON_START_X, CITY_BASE_NPC_BUTTON_START_Y);
//		m_pNPCButtonImage[NPC_BUTTON_FACTORY][m_nNPCButtonState[0]]->Render();
//	}
//	else if(IS_SKILL_SHOP_TYPE(pCity->buildingInfo.BuildingKind))
//	{
//		m_pNPCButtonImage[NPC_BUTTON_ITEM_SHOP][m_nNPCButtonState[0]]->Move(CITY_BASE_NPC_BUTTON_START_X, CITY_BASE_NPC_BUTTON_START_Y);
//		m_pNPCButtonImage[NPC_BUTTON_ITEM_SHOP][m_nNPCButtonState[0]]->Render();
//	}
//	else if(IS_AUCTION_SHOP_TYPE(pCity->buildingInfo.BuildingKind))
//	{
//		m_pNPCButtonImage[NPC_BUTTON_ITEM_SHOP][m_nNPCButtonState[0]]->Move(CITY_BASE_NPC_BUTTON_START_X, CITY_BASE_NPC_BUTTON_START_Y);
//		m_pNPCButtonImage[NPC_BUTTON_ITEM_SHOP][m_nNPCButtonState[0]]->Render();
//	}
//	else if(IS_CITY_OCCUPY_INFO_TYPE(pCity->buildingInfo.BuildingKind))
//	{
//		m_pNPCButtonImage[NPC_BUTTON_OCCUPY_INFO][m_nNPCButtonState[0]]->Move(CITY_BASE_NPC_BUTTON_START_X, CITY_BASE_NPC_BUTTON_START_Y);
//		m_pNPCButtonImage[NPC_BUTTON_OCCUPY_INFO][m_nNPCButtonState[0]]->Render();
//	}

	// 두번째 버튼, 미션 상점이 아니면서 미션과 연관된 상점
//	if( !IS_MISSION_SHOP_TYPE(pCity->buildingInfo.BuildingKind) && m_pMission->IsExistMission() )
//	{
		// 2005-12-22 by ispark
//		m_pNPCButtonImage[NPC_BUTTON_MISSION][m_nNPCButtonState[1]]->Move(CITY_BASE_NPC_BUTTON_START_X+CITY_BASE_NPC_BUTTON_POS_X_INTERVAL,
//													 CITY_BASE_NPC_BUTTON_START_Y);
//		m_pNPCButtonImage[NPC_BUTTON_MISSION][m_nNPCButtonState[1]]->Render();
//	}
	// 세번째 버튼, 클로즈
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	m_pNPCButtonImage[NPC_BUTTON_CLOSE][m_nCancelB]->Move(CITY_BASE_NPC_CLOSE_BUTTON_START_X, CITY_BASE_NPC_CLOSE_BUTTON_START_Y - 583);
	m_pNPCButtonImage[NPC_BUTTON_CLOSE][m_nCancelB]->Render();
#else
	m_pNPCButtonImage[NPC_BUTTON_CLOSE][m_nNPCButtonState[2]]->Move(CITY_BASE_NPC_CLOSE_BUTTON_START_X, CITY_BASE_NPC_CLOSE_BUTTON_START_Y);
	m_pNPCButtonImage[NPC_BUTTON_CLOSE][m_nNPCButtonState[2]]->Render();
#endif

}

void CINFCityBase::RenderBuildingNPCShop()
{

	CMapCityShopIterator it = m_mapCityShop.find(m_pCurrentBuildingNPC->buildingInfo.BuildingKind);
	if(it != m_mapCityShop.end())
	{
		switch(m_pCurrentBuildingNPC->buildingInfo.BuildingKind)
		{
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
		case BUILDINGKIND_TRIGGER_CRYSTAL:
			{
				if( /*g_pShuttleChild->m_myShuttleInfo.Level >= 3 &&*/ 
					g_pD3dApp->m_dwGameState == _CITY ||
					g_pD3dApp->m_dwGameState == _SHOP)
				{
					RenderBuildingKindKristalTriggerShop(it->second);  //2011-12-08 by jhahn EP4 트리거 상점 UI 변경
				}
			}
			break;
#endif

		case BUILDINGKIND_1ST_WEAPONSHOP:
			{
				if( /*g_pShuttleChild->m_myShuttleInfo.Level >= 3 &&*/ 
					g_pD3dApp->m_dwGameState == _CITY ||
					g_pD3dApp->m_dwGameState == _SHOP)
				{
					RenderBuildingKind1stWeaponShop(it->second);
				}
			}
			break;
		case BUILDINGKIND_2ND_WEAPONSHOP:
			{
				if(/*g_pShuttleChild->m_myShuttleInfo.Level >= 3 &&*/
					g_pD3dApp->m_dwGameState == _CITY ||
					g_pD3dApp->m_dwGameState == _SHOP)
				{
					RenderBuildingKind2ndWeaponShop(it->second);
				}
			}
			break;
		case BUILDINGKIND_COMPONENTSHOP:
			{
				if(/*g_pShuttleChild->m_myShuttleInfo.Level >= 2 &&*/ 
					g_pD3dApp->m_dwGameState == _CITY ||
					g_pD3dApp->m_dwGameState == _SHOP)
				{
					RenderBuildingKindComponentShop(it->second);
				}
			}
			break;
		case BUILDINGKIND_PETSHOP:
			{
				if( (g_pShuttleChild->m_myShuttleInfo.Level >= 10 && g_pD3dApp->m_dwGameState == _CITY) || 
					g_pD3dApp->m_dwGameState == _SHOP)
				{
					RenderBuildingKindPetShop(it->second);
				}
			}
			break;
		case BUILDINGKIND_ANTIQUESHOP:
			{
				if( (g_pShuttleChild->m_myShuttleInfo.Level >= 10 && g_pD3dApp->m_dwGameState == _CITY) || 
					g_pD3dApp->m_dwGameState == _SHOP)
				{
					RenderBuildingKindAntiqueShop(it->second);
				}
			}
			break;
		case BUILDINGKIND_CITYWARP:
			{
				if((g_pShuttleChild->m_myShuttleInfo.Level >= 10 && g_pD3dApp->m_dwGameState == _CITY) || 
					g_pD3dApp->m_dwGameState == _SHOP)
				{
					RenderBuildingKindCityWarp(it->second);
				}
			}
			break;
		case BUILDINGKIND_TUTORIAL:
			{
			}
			break;
		case BUILDINGKIND_PUBLICOFFICE:
			{
				if(/*g_pShuttleChild->m_myShuttleInfo.Level >= 2 &&*/ 
					g_pD3dApp->m_dwGameState == _CITY ||
					g_pD3dApp->m_dwGameState == _SHOP)
				{
					RenderBuildingKindPublicOffice(it->second);
				}
			}
			break;
		case BUILDINGKIND_AUCTION:
			{
				if(/*g_pShuttleChild->m_myShuttleInfo.Level >= 2 &&*/ 
					g_pD3dApp->m_dwGameState == _CITY ||
					g_pD3dApp->m_dwGameState == _SHOP)
				{
					RenderBuildingKindAuctionHouse(it->second);
				}
			}
			break;
		case BUILDINGKIND_CITY_OCCUPY_INFO:
			{
				if(
					g_pD3dApp->m_dwGameState == _CITY ||
					g_pD3dApp->m_dwGameState == _SHOP)
				{
					RenderBuildingKindCityOccupyInfo(it->second);
				}
			}
			break;
		case BUILDINGKIND_RACETRACK:
			{
				if((g_pShuttleChild->m_myShuttleInfo.Level >= 10 && g_pD3dApp->m_dwGameState == _CITY) || 
					g_pD3dApp->m_dwGameState == _SHOP)
				{
					RenderBuildingKindRaceTrack(it->second);
				}
			}
			break;
		case BUILDINGKIND_REFINERY:
			{
				if(/*g_pShuttleChild->m_myShuttleInfo.Level >= 2 ||*/
					g_pD3dApp->m_dwGameState == _CITY ||
					g_pD3dApp->m_dwGameState == _SHOP)
				{
					RenderBuildingKindRefinery(it->second);
				}
			}
			break;
		case BUILDINGKIND_LABORATORY:
			{
				if((g_pShuttleChild->m_myShuttleInfo.Level >= 3 && g_pD3dApp->m_dwGameState == _CITY) || 
					g_pD3dApp->m_dwGameState == _SHOP)
				{
					RenderBuildingKindLaboratory(it->second);
				}
			}
			break;
		case BUILDINGKIND_FACTORY:
			{
				if((g_pShuttleChild->m_myShuttleInfo.Level >= 10 && g_pD3dApp->m_dwGameState == _CITY) || 
					g_pD3dApp->m_dwGameState == _SHOP)
				{
					RenderBuildingKindFactory(it->second);
				}
			}
			break;
		case BUILDINGKIND_MILITARYACADEMY:
			{
				if((g_pShuttleChild->m_myShuttleInfo.Level >= 10 && g_pD3dApp->m_dwGameState == _CITY) || 
					g_pD3dApp->m_dwGameState == _SHOP)
				{
					RenderBuildingKindMilitaryAcademy(it->second);
				}
			}
			break;
		case BUILDINGKIND_STORE:
			{
				if((g_pShuttleChild->m_myShuttleInfo.Level >= 8 && g_pD3dApp->m_dwGameState == _CITY) || 
					g_pD3dApp->m_dwGameState == _SHOP)
				{
					RenderBuildingKindStore(it->second);
				}
			}
			break;
		case BUILDINGKIND_OUTDOOR:
			{
				if(/*g_pShuttleChild->m_myShuttleInfo.Level >= 2 &&*/ 
					g_pD3dApp->m_dwGameState == _CITY ||
					g_pD3dApp->m_dwGameState == _SHOP)
				{
				RenderBuildingKindOutDoor(it->second);
			}
			}
			break;
		case BUILDINGKIND_SKILL_SHOP:
			{
				if((g_pShuttleChild->m_myShuttleInfo.Level >= 5 && g_pD3dApp->m_dwGameState == _CITY) || 
					g_pD3dApp->m_dwGameState == _SHOP)
				{
					RenderBuildingKindSkillShop(it->second);
				}
			}
			break;
		case BUILDINGKIND_CASH_SHOP:
			{
				if(
					g_pD3dApp->m_dwGameState == _CITY ||
					g_pD3dApp->m_dwGameState == _SHOP ||
					g_pD3dApp->m_dwGameState == _GAME)
				{
					RenderBuildingKindCityCashShop(it->second);
				}
			}
			break;
		case BUILDINGKIND_TUNING_COLOR:
			{
				if(	g_pD3dApp->m_dwGameState == _CITY ||
					g_pD3dApp->m_dwGameState == _SHOP)
				{
					RenderBuildingKindCityColorShop(it->second);
				}
			}
			break;
		// 2008-11-04 by dgwoo LuckyMachine기획 추가.
		case BUILDINGKIND_LUCKY:
			{
				if(g_pD3dApp->m_dwGameState == _SHOP 
					|| g_pD3dApp->m_dwGameState == _CITY )
				{
					RenderBuildingKindCityLuckyMachin(it->second);
				}
			}
			break;
		// 2008-11-04 by dgwoo LuckyMachine기획 추가.
		// 2010. 04. 21 by jskim 신규 럭키 머신 구현
		case BUILDINGKIND_LUCKY_OPTION_MACHINE:
			{
				if(g_pD3dApp->m_dwGameState == _SHOP 
					|| g_pD3dApp->m_dwGameState == _CITY )
				{
					RenderBuildingKindCityOptionMachin(it->second);
				}
			}
			break;
		//end 2010. 04. 21 by jskim 신규 럭키 머신 구현
		// 2010-08-31 by shcho&&jskim, 용해 시스템 구현
		case BUILDINGKIND_DISSOLUTION:
			{
				if(g_pD3dApp->m_dwGameState == _SHOP 
					|| g_pD3dApp->m_dwGameState == _CITY )
				{
					RenderBuildingKindCityDissolution(it->second);
				}
			}
			break;
		// end 2010-08-31 by shcho&&jskim, 용해 시스템 구현

	
		case BUILDINGKIND_ARENA:
			{
				if(g_pD3dApp->m_dwGameState == _SHOP 
					|| g_pD3dApp->m_dwGameState == _CITY )
				{
					RenderBuildingKindCityArena(it->second);
				}
			}
			break;
		case BUILDINGKIND_WARPOINT_SHOP:
			{
				if(g_pD3dApp->m_dwGameState == _SHOP 
					|| g_pD3dApp->m_dwGameState == _CITY )
				{
					RenderBuildingKindCityWarPointShop(it->second);
				}

			}
			break;
		case BUILDINGKIND_CITYLEADER_OUTPOST:
			{
				if(g_pD3dApp->m_dwGameState == _SHOP 
					|| g_pD3dApp->m_dwGameState == _CITY )
				{
					RenderBuildingKindCityOutPost(it->second);
				}

			}
			break;
		case BUILDINGKIND_CITYLEADER_LEADER:
			{
				if(g_pD3dApp->m_dwGameState == _SHOP 
					|| g_pD3dApp->m_dwGameState == _CITY )
				{
					RenderBuildingKindCityLeader(it->second);
				}

			}
			break;
		// 2012-02-29 by mspark, 상점 타이틀 넣기
		case BUILDINGKIND_PARTSSHOP:
			{
				if( /*g_pShuttleChild->m_myShuttleInfo.Level >= 3 &&*/ 
					g_pD3dApp->m_dwGameState == _CITY ||
					g_pD3dApp->m_dwGameState == _SHOP)
				{
					RenderBuildingKindPartsShop(it->second);
				}
			}
			break;
		case BUILDINGKIND_GEARSHOP:
			{
				if( /*g_pShuttleChild->m_myShuttleInfo.Level >= 3 &&*/ 
					g_pD3dApp->m_dwGameState == _CITY ||
					g_pD3dApp->m_dwGameState == _SHOP)
				{
					RenderBuildingKindGearShop(it->second);
				}
			}
			break;
		case BUILDINGKIND_CARDSHOP:
			{
				if( /*g_pShuttleChild->m_myShuttleInfo.Level >= 3 &&*/ 
					g_pD3dApp->m_dwGameState == _CITY ||
					g_pD3dApp->m_dwGameState == _SHOP)
				{
					RenderBuildingKindCardShop(it->second);
				}
			}
			break;
		// end 2012-02-29 by mspark, 상점 타이틀 넣기

		}
	}

}
//2011-12-08 by jhahn EP4 트리거 상점 UI 변경
void CINFCityBase::RenderBuildingKindKristalTriggerShop(CINFBase* pShop)
{
	

#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경

	//	g_pGameMain->m_pInfWindow->RenderCenterWindow(CITY_BASE_NPC_BOX_START_X,
	//				 CITY_BASE_NPC_BOX_START_Y - SIZE_NORMAL_WINDOW_Y,
	//				 SIZE_BIG_WINDOW_X, 
	//				 SIZE_NORMAL_WINDOW_Y,FALSE);
	m_pRenewTirShopImage->Move(CITY_BASE_NPC_BOX_START_X+426, CITY_BASE_NPC_BOX_START_Y - SIZE_NORMAL_WINDOW_Y);
	m_pRenewTirShopImage->Render();
	
	pShop->Render();
	g_pGameMain->m_pInven->RenderOnCityBase();

	
#endif

}
//end 2011-12-08 by jhahn EP4 트리거 상점 UI 변경
void CINFCityBase::RenderBuildingKind1stWeaponShop(CINFBase* pShop)
{
//	g_pGameMain->m_pInfWindow->RenderCenterWindow(CITY_BASE_NPC_BOX_START_X,
//				 CITY_BASE_NPC_BOX_START_Y - SIZE_NORMAL_WINDOW_Y,
//				 SIZE_BIG_WINDOW_X, 
//				 SIZE_NORMAL_WINDOW_Y,FALSE);
	m_pRenewShopImage->Move(CITY_BASE_NPC_BOX_START_X+426, CITY_BASE_NPC_BOX_START_Y - SIZE_NORMAL_WINDOW_Y);
	m_pRenewShopImage->Render();

	pShop->Render();
	g_pGameMain->m_pInven->RenderOnCityBase();

	// 2012-02-29 by mspark, 상점 타이틀 넣기
	if(m_pImgTitleWeapon)
	{
		m_pImgTitleWeapon->Render();
	}
	// end 2012-02-29 by mspark, 상점 타이틀 넣기
}
void CINFCityBase::RenderBuildingKind2ndWeaponShop(CINFBase* pShop)
{
//	g_pGameMain->m_pInfWindow->RenderCenterWindow(CITY_BASE_NPC_BOX_START_X,
//				 CITY_BASE_NPC_BOX_START_Y - SIZE_NORMAL_WINDOW_Y,
//				 SIZE_BIG_WINDOW_X, 
//				 SIZE_NORMAL_WINDOW_Y,FALSE);
	m_pRenewShopImage->Move(CITY_BASE_NPC_BOX_START_X+426, CITY_BASE_NPC_BOX_START_Y - SIZE_NORMAL_WINDOW_Y);
	m_pRenewShopImage->Render();

	pShop->Render();
	g_pGameMain->m_pInven->RenderOnCityBase();

	// 2012-02-29 by mspark, 상점 타이틀 넣기
	if(m_pImgTitleWeapon)
	{
		m_pImgTitleWeapon->Render();
	}
	// end 2012-02-29 by mspark, 상점 타이틀 넣기
}
void CINFCityBase::RenderBuildingKindComponentShop(CINFBase* pShop)
{
//	g_pGameMain->m_pInfWindow->RenderCenterWindow(CITY_BASE_NPC_BOX_START_X,
//				 CITY_BASE_NPC_BOX_START_Y - SIZE_NORMAL_WINDOW_Y,
//				 SIZE_BIG_WINDOW_X, 
//				 SIZE_NORMAL_WINDOW_Y,FALSE);
	m_pRenewShopImage->Move(CITY_BASE_NPC_BOX_START_X+426, CITY_BASE_NPC_BOX_START_Y - SIZE_NORMAL_WINDOW_Y);
	m_pRenewShopImage->Render();

	pShop->Render();
	g_pGameMain->m_pInven->RenderOnCityBase();
}
void CINFCityBase::RenderBuildingKindPetShop(CINFBase* pShop)
{
}
void CINFCityBase::RenderBuildingKindAntiqueShop(CINFBase* pShop)
{
}

void CINFCityBase::RenderBuildingKindCityWarp(CINFBase* pShop)//워프
{
#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	g_pGameMain->m_pInfWindow->RenderCenterWindow(CITY_BASE_NPC_BOX_START_X,
				 CITY_BASE_NPC_BOX_START_Y - CITY_BASE_WARP_SIZE_Y,
				 CITY_BASE_WARP_SIZE_X, 
				 CITY_BASE_WARP_SIZE_Y,FALSE);
#endif
	pShop->Render();
}
void CINFCityBase::RenderBuildingKindPublicOffice(CINFBase* pShop)
{
//	g_pGameMain->m_pInfWindow->RenderCenterWindow(CITY_BASE_NPC_BOX_START_X,
//				 CITY_BASE_NPC_BOX_START_Y - SIZE_NORMAL_WINDOW_Y,
//				 SIZE_BIG_WINDOW_X, 
//				 SIZE_NORMAL_WINDOW_Y,FALSE);
	m_pRenewShopImage->Move(CITY_BASE_NPC_BOX_START_X+426, CITY_BASE_NPC_BOX_START_Y - SIZE_NORMAL_WINDOW_Y);
	m_pRenewShopImage->Render();

	pShop->Render();
	g_pGameMain->m_pInven->RenderOnCityBase();
}
void CINFCityBase::RenderBuildingKindAuctionHouse(CINFBase* pShop)
{
//	g_pGameMain->m_pInfWindow->RenderCenterWindow(CITY_BASE_NPC_BOX_START_X,
//				 CITY_BASE_NPC_BOX_START_Y - SIZE_NORMAL_WINDOW_Y,
//				 SIZE_BIG_WINDOW_X, 
//				 SIZE_NORMAL_WINDOW_Y,FALSE);
	m_pRenewShopImage->Move(CITY_BASE_NPC_BOX_START_X+426, CITY_BASE_NPC_BOX_START_Y - SIZE_NORMAL_WINDOW_Y);
	m_pRenewShopImage->Render();

	pShop->Render();
	g_pGameMain->m_pInven->RenderOnCityBase();
}
void CINFCityBase::RenderBuildingKindRaceTrack(CINFBase* pShop)
{
}
void CINFCityBase::RenderBuildingKindRefinery(CINFBase* pShop)
{
	// 2005-11-07 by ispark
	// 수리상점 -> 보급상점으로 변경
#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	g_pGameMain->m_pInfWindow->RenderCenterWindow(CITY_BASE_NPC_BOX_START_X,
				 CITY_BASE_NPC_BOX_START_Y - SIZE_SUPPLY_WINDOW_Y,
				 SIZE_SUPPLY_WINDOW_X, 
				 SIZE_SUPPLY_WINDOW_Y,FALSE);
#else
	m_pRenewShopRefinery->Move(CITY_BASE_NPC_BOX_START_X, CITY_BASE_NPC_BOX_START_Y - SIZE_NORMAL_WINDOW_Y);
	m_pRenewShopRefinery->Render();
#endif
	pShop->Render();
//	g_pGameMain->m_pInven->RenderOnCityBase();
}
void CINFCityBase::RenderBuildingKindLaboratory(CINFBase* pShop)
{
//	g_pGameMain->m_pInfWindow->RenderCenterWindow(CITY_BASE_NPC_BOX_START_X,
//				 CITY_BASE_NPC_BOX_START_Y - SIZE_NORMAL_WINDOW_Y,
//				 SIZE_BIG_WINDOW_X, 
//				 SIZE_NORMAL_WINDOW_Y,FALSE);
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
#else
	m_pRenewShopImage->Move(CITY_BASE_NPC_BOX_START_X+426, CITY_BASE_NPC_BOX_START_Y - SIZE_NORMAL_WINDOW_Y);
	m_pRenewShopImage->Render();
#endif
	

	pShop->Render();
	g_pGameMain->m_pInven->RenderOnCityBase();

	// 2012-02-29 by mspark, 상점 타이틀 넣기
	if(m_pImgTitleLab)
	{
		m_pImgTitleLab->Render();
	}
	// end 2012-02-29 by mspark, 상점 타이틀 넣기
}
void CINFCityBase::RenderBuildingKindFactory(CINFBase* pShop)
{
//	g_pGameMain->m_pInfWindow->RenderCenterWindow(CITY_BASE_NPC_BOX_START_X,
//				 CITY_BASE_NPC_BOX_START_Y - SIZE_NORMAL_WINDOW_Y,
//				 SIZE_BIG_WINDOW_X, 
//				 SIZE_NORMAL_WINDOW_Y,FALSE);
#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	m_pRenewShopImage->Move(CITY_BASE_NPC_BOX_START_X+426, CITY_BASE_NPC_BOX_START_Y - SIZE_NORMAL_WINDOW_Y);
	m_pRenewShopImage->Render();
#endif

	pShop->Render();
	g_pGameMain->m_pInven->RenderOnCityBase();

	// 2013-03-18 by bhsohn 팩토리 조합 추가
#ifdef C_INGAME_MIX_ITEM
#else
	// 2012-02-29 by mspark, 상점 타이틀 넣기
	if(m_pImgTitleFactory)
	{
		m_pImgTitleFactory->Render();
	}
#endif
	// end 2012-02-29 by mspark, 상점 타이틀 넣기
	// END 2013-03-18 by bhsohn 팩토리 조합 추가
}
void CINFCityBase::RenderBuildingKindMilitaryAcademy(CINFBase* pShop)
{
}
void CINFCityBase::RenderBuildingKindStore(CINFBase* pShop)
{
//	g_pGameMain->m_pInfWindow->RenderCenterWindow(CITY_BASE_NPC_BOX_START_X,
//				 CITY_BASE_NPC_BOX_START_Y - SIZE_NORMAL_WINDOW_Y,
//				 SIZE_BIG_WINDOW_X, 
//				 SIZE_NORMAL_WINDOW_Y,FALSE);
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	m_pRenewCityStore->Move(CITY_BASE_NPC_BOX_START_X+426, CITY_BASE_NPC_BOX_START_Y - SIZE_NORMAL_WINDOW_Y);
	m_pRenewCityStore->Render();
#else
	m_pRenewShopImage->Move(CITY_BASE_NPC_BOX_START_X+426, CITY_BASE_NPC_BOX_START_Y - SIZE_NORMAL_WINDOW_Y);
	m_pRenewShopImage->Render();
#endif
	
	pShop->Render();
	g_pGameMain->m_pInven->RenderOnCityBase();
}
void CINFCityBase::RenderBuildingKindOutDoor(CINFBase* pShop)
{
	m_pRenewShopImage->Move(CITY_BASE_NPC_BOX_START_X+426, CITY_BASE_NPC_BOX_START_Y - SIZE_NORMAL_WINDOW_Y);
	m_pRenewShopImage->Render();
}
void CINFCityBase::RenderBuildingKindSkillShop(CINFBase* pShop)
{
#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	g_pGameMain->m_pInfWindow->RenderCenterWindow(CITY_BASE_NPC_BOX_START_X,
				 CITY_BASE_NPC_BOX_START_Y - SIZE_NORMAL_WINDOW_Y,
				 SIZE_SKILL_SHOP_WINDOW_X,	// 2008-08-22 by bhsohn EP3 인벤토리 처리
				 SIZE_NORMAL_WINDOW_Y,FALSE);	
#endif
	//g_pGameMain->m_pCharacterInfo->RenderSkillInShop();// 2008-08-22 by bhsohn EP3 인벤토리 처리
	//g_pGameMain->m_pInven->RenderSpi( SKILL_SHOP_SPI_START_X, SKILL_SHOP_SPI_START_Y );
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	if(m_pRenewShopSkill)
	{
		m_pRenewShopSkill->Move( CITY_BASE_NPC_BOX_START_X, CITY_BASE_NPC_BOX_START_Y - SIZE_NORMAL_WINDOW_Y );
		m_pRenewShopSkill->Render();
	}
#endif
	pShop->Render();
	g_pGameMain->m_pCharacterInfo->RenderSkillInShop();// 2008-08-22 by bhsohn EP3 인벤토리 처리
	g_pGameMain->m_pInven->RenderSpi( SKILL_SHOP_SPI_START_X, SKILL_SHOP_SPI_START_Y );
}
void CINFCityBase::RenderBuildingKindCityOccupyInfo(CINFBase* pShop)
{
	g_pGameMain->m_pInfWindow->RenderCenterWindow(CITY_BASE_NPC_BOX_START_X,
				 CITY_BASE_NPC_BOX_START_Y - SIZE_NORMAL_WINDOW_Y,
				 SIZE_BIG_WINDOW_X, 
				 SIZE_NORMAL_WINDOW_Y,FALSE);
	pShop->Render();
//	g_pGameMain->m_pInven->RenderOnCityBase();
}
void CINFCityBase::RenderBuildingKindCityCashShop(CINFBase* pShop)
{
//	g_pGameMain->m_pInfWindow->RenderCenterWindow(CITY_BASE_NPC_BOX_START_X,
//			 CITY_BASE_NPC_BOX_START_Y - SIZE_SUPPLY_WINDOW_Y,
//			 SIZE_SUPPLY_WINDOW_X, 
//			 SIZE_SUPPLY_WINDOW_Y,FALSE);
	pShop->Render();
}

void CINFCityBase::RenderBuildingKindCityColorShop(CINFBase* pShop)
{
#ifdef _REWORKED_COLORSHOP
	m_pRenewCityWarPointStore->Move(CITY_BASE_NPC_BOX_START_X + 426, CITY_BASE_NPC_BOX_START_Y - SIZE_NORMAL_WINDOW_Y);
	m_pRenewCityWarPointStore->Render();
#else
	m_pRenewShopImage->Move(CITY_BASE_NPC_BOX_START_X + 426, CITY_BASE_NPC_BOX_START_Y - SIZE_NORMAL_WINDOW_Y);
	m_pRenewShopImage->Render();
#endif

	pShop->Render();
	g_pGameMain->m_pInven->RenderOnCityBase();	
}
// 2008-11-04 by dgwoo LuckyMachine기획 추가.
void CINFCityBase::RenderBuildingKindCityLuckyMachin(CINFBase* pShop)
{
	// 2009-03-04 by bhsohn 럭키 머신 수정안
// 	m_pRenewShopImage->Move(CITY_BASE_NPC_BOX_START_X+426, CITY_BASE_NPC_BOX_START_Y - SIZE_NORMAL_WINDOW_Y);
// 	m_pRenewShopImage->Render();
	// end 2009-03-04 by bhsohn 럭키 머신 수정안
	pShop->Render();
	
}

// 2010. 04. 21 by jskim 신규 럭키 머신 구현
void CINFCityBase::RenderBuildingKindCityOptionMachin(CINFBase* pShop)
{
	pShop->Render();	
}
//end 2010. 04. 21 by jskim 신규 럭키 머신 구현

// 2010-08-31 by shcho&&jskim, 용해 시스템 구현
void CINFCityBase::RenderBuildingKindCityDissolution(CINFBase* pShop)
{
#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	g_pGameMain->m_pInfWindow->RenderCenterWindow(CITY_BASE_NPC_BOX_START_X + SIZE_RNORMAL_WINDOW_X,
			 CITY_BASE_NPC_BOX_START_Y - SIZE_CITYLEADER_WINDOWR_Y,
			 SIZE_RIGNT_WINDOW_X,
			 SIZE_CITYLEADER_WINDOWR_Y,FALSE);
#endif
	pShop->Render();
}
// end 2010-08-31 by shcho&&jskim, 용해 시스템 구현
// 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템
// 인피니티 입장상점
void CINFCityBase::RenderBuildingKindCityInfinityField( void )
{
	CMapCityShopIterator it = m_mapCityShop.find(m_pCurrentBuildingNPC->buildingInfo.BuildingKind);
	if(it != m_mapCityShop.end())
	{
		it->second->Render();
	}
}

// 인피니티 판매 상점
void CINFCityBase::RenderBuildingKindCItyInfinityShop( void )
{
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	m_pRenewInfinityImage->Move(CITY_BASE_NPC_BOX_START_X+426, CITY_BASE_NPC_BOX_START_Y - SIZE_NORMAL_WINDOW_Y);
	m_pRenewInfinityImage->Render();
#else
	m_pRenewShopImage->Move(CITY_BASE_NPC_BOX_START_X+426, CITY_BASE_NPC_BOX_START_Y - SIZE_NORMAL_WINDOW_Y);
	m_pRenewShopImage->Render();
#endif
	
	CMapCityShopIterator it = m_mapCityShop.find(m_pCurrentBuildingNPC->buildingInfo.BuildingKind);
	if(it != m_mapCityShop.end())
	{
		it->second->Render();
	}
}
// end 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템

// 2013-05-07 by bhsohn 세력포인트 개선안 시스템
void CINFCityBase::RenderBuildingKindCItyInflBuffShop()
{	
#ifdef C_UPGRADE_INFLUENCE_POINT_BHSOHN
	CMapCityShopIterator it = m_mapCityShop.find(m_pCurrentBuildingNPC->buildingInfo.BuildingKind);
	if(it != m_mapCityShop.end())
	{
		it->second->Render();
	}
#endif
}

// 2008-11-04 by dgwoo LuckyMachine기획 추가.
// 2007-04-23 by dgwoo 아레나.
void CINFCityBase::RenderBuildingKindCityArena(CINFBase* pShop)
{
#ifndef C_EPSODE4_UI_CHANGE_JSKIM
	g_pGameMain->m_pInfWindow->RenderCenterWindow(CITY_BASE_NPC_BOX_START_X,
			 CITY_BASE_NPC_BOX_START_Y - SIZE_ARENA_WINDOW_Y,
			 SIZE_ARENA_WINDOW_X, 
			 SIZE_ARENA_WINDOW_Y,FALSE);
#endif
 	
	pShop->Render();
}
void CINFCityBase::RenderBuildingKindCityWarPointShop(CINFBase* pShop)
{
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	m_pRenewCityWarPointStore->Move(CITY_BASE_NPC_BOX_START_X+426, CITY_BASE_NPC_BOX_START_Y - SIZE_NORMAL_WINDOW_Y);
	m_pRenewCityWarPointStore->Render();
#else
	m_pRenewShopImage->Move(CITY_BASE_NPC_BOX_START_X+426, CITY_BASE_NPC_BOX_START_Y - SIZE_NORMAL_WINDOW_Y);
	m_pRenewShopImage->Render();
#endif

	pShop->Render();
	g_pGameMain->m_pInven->RenderOnCityBase();

	// 2012-02-29 by mspark, 상점 타이틀 넣기
	if(m_pImgTitleWarPoint)
	{
		m_pImgTitleWarPoint->Render();
	}
	// end 2012-02-29 by mspark, 상점 타이틀 넣기
}
void CINFCityBase::RenderBuildingKindCityLeader(CINFBase* pShop)
{
#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	g_pGameMain->m_pInfWindow->RenderCenterWindow(CITY_BASE_NPC_BOX_START_X,
			 CITY_BASE_NPC_BOX_START_Y - SIZE_CITYLEADER_WINDOWL_Y,
			 SIZE_CITYLEADER_WINDOWL_X, 
			 SIZE_CITYLEADER_WINDOWL_Y,FALSE);		
	// 2007-12-05 by dgwoo 전쟁 정보 인터페이스 추가.
	CINFCityLeader * pCityLeader = (CINFCityLeader*)pShop;
	if(LEADER_STATE_WARINFO == pCityLeader->GetLeaderState())
	{
		g_pGameMain->m_pInfWindow->RenderCenterWindow(CITY_BASE_NPC_BOX_START_X + SIZE_CITYLEADER_WINDOWL_X,
				 CITY_BASE_NPC_BOX_START_Y - SIZE_CITYLEADER_WINDOWR_Y,
				 SIZE_CITYLEADER_WINDOWR_WARINFO_X, 
				 SIZE_CITYLEADER_WINDOWR_Y,FALSE);
	}else
	{
		g_pGameMain->m_pInfWindow->RenderCenterWindow(CITY_BASE_NPC_BOX_START_X + SIZE_CITYLEADER_WINDOWL_X,
				 CITY_BASE_NPC_BOX_START_Y - SIZE_CITYLEADER_WINDOWR_Y,
				 SIZE_CITYLEADER_WINDOWR_X, 
				 SIZE_CITYLEADER_WINDOWR_Y,FALSE);
	}
#endif

	pShop->Render();
}
void CINFCityBase::RenderBuildingKindCityOutPost(CINFBase* pShop)
{
#ifdef C_EPSODE4_UI_CHANGE_JSKIM
#else
	g_pGameMain->m_pInfWindow->RenderCenterWindow(CITY_BASE_NPC_BOX_START_X,
			 CITY_BASE_NPC_BOX_START_Y - SIZE_CITYLEADER_WINDOWL_Y,
			 SIZE_CITYLEADER_WINDOWL_X, 
			 SIZE_CITYLEADER_WINDOWL_Y,FALSE);		
	// 2007-12-05 by dgwoo 전쟁 정보 인터페이스 추가.
	CINFCityOutPost* pCityOutPost = (CINFCityOutPost*)pShop;
	if(OUTPOST_STATE_WARINFO == pCityOutPost->GetOutPostState())
	{
		g_pGameMain->m_pInfWindow->RenderCenterWindow(CITY_BASE_NPC_BOX_START_X + SIZE_CITYLEADER_WINDOWL_X,
				 CITY_BASE_NPC_BOX_START_Y - SIZE_CITYLEADER_WINDOWR_Y,
				 SIZE_CITYLEADER_WINDOWR_WARINFO_X, 
				 SIZE_CITYLEADER_WINDOWR_Y,FALSE);

	}else
	{
		g_pGameMain->m_pInfWindow->RenderCenterWindow(CITY_BASE_NPC_BOX_START_X + SIZE_CITYLEADER_WINDOWL_X,
				 CITY_BASE_NPC_BOX_START_Y - SIZE_CITYLEADER_WINDOWR_Y,
				 SIZE_CITYLEADER_WINDOWR_X, 
				 SIZE_CITYLEADER_WINDOWR_Y,FALSE);
	}
#endif
	pShop->Render();
}

// 2012-02-29 by mspark, 상점 타이틀 넣기
void CINFCityBase::RenderBuildingKindPartsShop(CINFBase* pShop)
{
	m_pRenewShopImage->Move(CITY_BASE_NPC_BOX_START_X+426, CITY_BASE_NPC_BOX_START_Y - SIZE_NORMAL_WINDOW_Y);
	m_pRenewShopImage->Render();
	
	pShop->Render();
	g_pGameMain->m_pInven->RenderOnCityBase();

	// 2012-02-29 by mspark, 상점 타이틀 넣기
	if(m_pImgTitleParts)
	{
		m_pImgTitleParts->Render();
	}
	// end 2012-02-29 by mspark, 상점 타이틀 넣기
}

void CINFCityBase::RenderBuildingKindGearShop(CINFBase* pShop)
{
	m_pRenewShopImage->Move(CITY_BASE_NPC_BOX_START_X+426, CITY_BASE_NPC_BOX_START_Y - SIZE_NORMAL_WINDOW_Y);
	m_pRenewShopImage->Render();
	
	pShop->Render();
	g_pGameMain->m_pInven->RenderOnCityBase();

	// 2012-02-29 by mspark, 상점 타이틀 넣기
	if(m_pImgTitleGear)
	{
		m_pImgTitleGear->Render();
	}
	// end 2012-02-29 by mspark, 상점 타이틀 넣기
}

void CINFCityBase::RenderBuildingKindCardShop(CINFBase* pShop)
{
	m_pRenewShopImage->Move(CITY_BASE_NPC_BOX_START_X+426, CITY_BASE_NPC_BOX_START_Y - SIZE_NORMAL_WINDOW_Y);
	m_pRenewShopImage->Render();
	
	pShop->Render();
	g_pGameMain->m_pInven->RenderOnCityBase();

	// 2012-02-29 by mspark, 상점 타이틀 넣기
	if(m_pImgTitleCard)
	{
		m_pImgTitleCard->Render();
	}
	// end 2012-02-29 by mspark, 상점 타이틀 넣기
}
// end 2012-02-29 by mspark, 상점 타이틀 넣기

void CINFCityBase::RenderUpBox()
{
	FLOG( "CINFCityBase::RenderUpBox()" );
	m_pUpBoxImage[1]->Move(0, 0);
	m_pUpBoxImage[1]->SetScale(g_pD3dApp->GetBackBufferDesc().Width, CITY_BASE_UP_BOX_HEIGHT-2);
	m_pUpBoxImage[1]->Render();
	m_pUpBoxImage[0]->Move(0, CITY_BASE_UP_BOX_HEIGHT-2);
	m_pUpBoxImage[0]->SetScale(g_pD3dApp->GetBackBufferDesc().Width, 1);
	m_pUpBoxImage[0]->Render();
}

void CINFCityBase::RenderDownBox()
{
	FLOG( "CINFCityBase::RenderDownBox()" );
	m_pDownBoxImage[0]->Move( 0, CITY_BASE_DOWN_BOX_START_Y );
	m_pDownBoxImage[0]->SetScale(g_pD3dApp->GetBackBufferDesc().Width, 1);
	m_pDownBoxImage[0]->Render();
	m_pDownBoxImage[1]->Move( 0, CITY_BASE_DOWN_BOX_START_Y+2 );
	m_pDownBoxImage[1]->SetScale(g_pD3dApp->GetBackBufferDesc().Width, CITY_BASE_DOWN_BOX_HEIGHT-2);
	m_pDownBoxImage[1]->Render();
}

#define EDGE_SIZE		6
void CINFCityBase::RenderNPCBox(int x, int y, int cx, int cy)
{
	FLOG( "CINFCityBase::RenderNPCBox(int x, int y, int cx, int cy)" );
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	m_pNPCBoxImageEP->Move(x,y);
	m_pNPCBoxImageEP->Render();
// 	m_pNPCBoxImage[0]->Move(x,y);
// 	m_pNPCBoxImage[0]->Render();
// 	m_pNPCBoxImage[1]->Move(x+EDGE_SIZE, y);
// 	m_pNPCBoxImage[1]->SetScale(cx-EDGE_SIZE*2, 1);
// 	m_pNPCBoxImage[1]->Render();
// 	m_pNPCBoxImage[2]->Move(x+cx-EDGE_SIZE,y);
// 	m_pNPCBoxImage[2]->Render();
// 
// 	m_pNPCBoxImage[3]->Move(x, y+EDGE_SIZE);
// 	m_pNPCBoxImage[3]->SetScale(1, cy-EDGE_SIZE*2);
// 	m_pNPCBoxImage[3]->Render();
// 	m_pNPCBoxImage[4]->Move(x+EDGE_SIZE, y+EDGE_SIZE);
// 	m_pNPCBoxImage[4]->SetScale(cx-EDGE_SIZE*2,cy-EDGE_SIZE*2);
// 	m_pNPCBoxImage[4]->Render();
// 	m_pNPCBoxImage[5]->Move(x+cx-EDGE_SIZE, y+EDGE_SIZE);
// 	m_pNPCBoxImage[5]->SetScale(1, cy-EDGE_SIZE*2);
// 	m_pNPCBoxImage[5]->Render();
// 
// 	m_pNPCBoxImage[6]->Move(x,y+cy-EDGE_SIZE);
// 	m_pNPCBoxImage[6]->Render();
// 	m_pNPCBoxImage[7]->Move(x+EDGE_SIZE,y+cy-EDGE_SIZE);
// 	m_pNPCBoxImage[7]->SetScale(cx-EDGE_SIZE*2, 1);
// 	m_pNPCBoxImage[7]->Render();
// 	m_pNPCBoxImage[8]->Move(x+cx-EDGE_SIZE,y+cy-EDGE_SIZE);
// 	m_pNPCBoxImage[8]->Render();							  
#else	   
	m_pNPCBoxImage[0]->Move(x,y);
	m_pNPCBoxImage[0]->Render();
	m_pNPCBoxImage[1]->Move(x+EDGE_SIZE, y);
	m_pNPCBoxImage[1]->SetScale(cx-EDGE_SIZE*2, 1);
	m_pNPCBoxImage[1]->Render();
	m_pNPCBoxImage[2]->Move(x+cx-EDGE_SIZE,y);
	m_pNPCBoxImage[2]->Render();

	m_pNPCBoxImage[3]->Move(x, y+EDGE_SIZE);
	m_pNPCBoxImage[3]->SetScale(1, cy-EDGE_SIZE*2);
	m_pNPCBoxImage[3]->Render();
	m_pNPCBoxImage[4]->Move(x+EDGE_SIZE, y+EDGE_SIZE);
	m_pNPCBoxImage[4]->SetScale(cx-EDGE_SIZE*2,cy-EDGE_SIZE*2);
	m_pNPCBoxImage[4]->Render();
	m_pNPCBoxImage[5]->Move(x+cx-EDGE_SIZE, y+EDGE_SIZE);
	m_pNPCBoxImage[5]->SetScale(1, cy-EDGE_SIZE*2);
	m_pNPCBoxImage[5]->Render();

	m_pNPCBoxImage[6]->Move(x,y+cy-EDGE_SIZE);
	m_pNPCBoxImage[6]->Render();
	m_pNPCBoxImage[7]->Move(x+EDGE_SIZE,y+cy-EDGE_SIZE);
	m_pNPCBoxImage[7]->SetScale(cx-EDGE_SIZE*2, 1);
	m_pNPCBoxImage[7]->Render();
	m_pNPCBoxImage[8]->Move(x+cx-EDGE_SIZE,y+cy-EDGE_SIZE);
	m_pNPCBoxImage[8]->Render();
#endif
	
}
GUI_BUILDINGNPC* CINFCityBase::FindBuildingNPC(int nBuildingIndex)
{
	vector<GUI_BUILDINGNPC*>::iterator it = m_vecButtonGroup.begin();
	while( it != m_vecButtonGroup.end() )
	{
		if( (*it)->buildingInfo.BuildingIndex == nBuildingIndex )
		{
			return (*it);
		}
		it++;
	}
	return NULL;
}
void CINFCityBase::AddBuildingIndexList(CITY_BUILDING_INFO_4_EXCHANGE &info)
{
//	GUI_BUILDINGNPC* pBuilding = NULL;
//	CMapCityGroupIterator it = m_mapButtonGroup.find(info.BuildingIndex);
	GUI_BUILDINGNPC* pBuilding = FindBuildingNPC( info.BuildingIndex );
//	if( it == m_mapButtonGroup.end() )
	if( pBuilding == NULL )
	{
		pBuilding = new GUI_BUILDINGNPC;
		memset(&pBuilding->buildingInfo, 0x00, sizeof(BUILDINGNPC));
		pBuilding->buildingInfo.BuildingIndex = info.BuildingIndex;
		pBuilding->buildingInfo.BuildingKind = info.BuildingKind;
		/*for(int i=0;i<CITY_BUTTON_NUMBER;i++)
		{
			pBuilding->m_pButton[i] = new CINFImageEx;
			char buf[32];
			wsprintf(buf, "shbtn%02d%d", info.BuildingKind, i);
			DataHeader* pDataHeader = FindResource(buf);
			if(pDataHeader)
			{
				pBuilding->m_pButton[i]->InitDeviceObjects( pDataHeader );
				pBuilding->m_pButton[i]->RestoreDeviceObjects();
			}
			else
			{
				DBGOUT("Shop Button is Non Existent.     index:%d, kind:%d\n", info.BuildingIndex, info.BuildingKind);
			}
		}*/
//		m_mapButtonGroup[info.BuildingIndex] = pBuilding;
		m_vecButtonGroup.push_back( pBuilding );
	}
#ifdef _DEBUG
	else
	{
		DBGOUT("CINFCityBase::AddBuildingIndexList, Already being CITY_BUILDING_INFO_4_EXCHANGE is arrived.(idnex:%d,kind:%d)\n", info.BuildingIndex, info.BuildingKind);
	}
#endif // _DEBUG_endif
}

void CINFCityBase::SetDetailBuildingInfo(BUILDINGNPC &buildingNPC)
{
//	CMapCityGroupIterator it = m_mapButtonGroup.find(buildingNPC.BuildingIndex);
	GUI_BUILDINGNPC* pBuilding = FindBuildingNPC( buildingNPC.BuildingIndex );
//	if( it != m_mapButtonGroup.end())
	if( pBuilding != NULL )
	{
//		memcpy(&((it->second)->buildingInfo), &buildingNPC, sizeof(BUILDINGNPC));
		memcpy(&(pBuilding->buildingInfo), &buildingNPC, sizeof(BUILDINGNPC));
		SetNPCTalkInfo( buildingNPC.GreetingTalk);
	}
#ifdef _DEBUG
	else
	{
		DBGOUT("CINFCityBase::SetDetailBuildingInfo, Non existent BUILDINGNPC is Arrived.(idnex:%d,kind:%d)\n", 
			buildingNPC.BuildingIndex, buildingNPC.BuildingKind);
	}
#endif // _DEBUG_endif
}

int CINFCityBase::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if(m_pCurrentBuildingNPC)
	{
		if(m_nCurrentBuildingNPCState != BUILDINGNPC_RENDER_CASH_SHOP)
		{
			if(m_pNPCScroll)
			{
				if( m_vecNPCTalk.size() > CITY_NPCTALK_LINE_FONT_NUMBER &&
					m_pNPCScroll->WndProc(uMsg, wParam, lParam) == INF_MSGPROC_BREAK)
				{
					return INF_MSGPROC_BREAK;
				}
			}
		}
		switch(m_nCurrentBuildingNPCState)
		{
		case BUILDINGNPC_RENDER_MSGBOX:
			{

			}
			break;
		case BUILDINGNPC_RENDER_TALKBOX:
			{
				CloseAllMessageBox();				
			}
			break;
		case BUILDINGNPC_RENDER_SHOP:
			{
				CMapCityShopIterator it = m_mapCityShop.find(m_pCurrentBuildingNPC->buildingInfo.BuildingKind);
				if(it != m_mapCityShop.end())
				{
					if( it->second->WndProc(uMsg, wParam, lParam) == INF_MSGPROC_BREAK )
						return INF_MSGPROC_BREAK;
					
					// in item shop
					if( IS_EXEC_INVENTORY(m_pCurrentBuildingNPC->buildingInfo.BuildingKind) &&
						g_pGameMain->m_pInven->WndProcOnCityBase( uMsg, wParam, lParam ) == INF_MSGPROC_BREAK )
					{
						return INF_MSGPROC_BREAK;
					}
					// in skill shop
					else if(m_pCurrentBuildingNPC->buildingInfo.BuildingKind == BUILDINGKIND_SKILL_SHOP &&
						g_pGameMain->m_pCharacterInfo->WndProcSkillInShop( uMsg, wParam, lParam ) == INF_MSGPROC_BREAK )
					{
						return INF_MSGPROC_BREAK;
					}
				}
			}
			break;
		case BUILDINGNPC_RENDER_MISSION:
			{
				if( m_pMission->WndProc(uMsg, wParam, lParam) == INF_MSGPROC_BREAK )
					return INF_MSGPROC_BREAK;
			}
			break;
		case BUILDINGNPC_RENDER_OCCUPY:
			{
				if(m_mapCityShop[BUILDINGKIND_CITY_OCCUPY_INFO]->WndProc(uMsg, wParam, lParam) == INF_MSGPROC_BREAK )
					return INF_MSGPROC_BREAK;		
			}
			break;
		case BUILDINGNPC_RENDER_CASH_SHOP:
			{
				// 2009-02-24 by bhsohn 캐쉬샵 충전 웹페이지 추가
// 				if(g_pD3dApp->GetWeb() == FALSE)
// 				{
// 					if(m_mapCityShop[BUILDINGKIND_CASH_SHOP]->WndProc(uMsg, wParam, lParam) == INF_MSGPROC_BREAK )
// 						return INF_MSGPROC_BREAK;		
// 				}
// 				else
// 				{
// 					if(WebWndProc(uMsg, wParam, lParam) == INF_MSGPROC_BREAK )
// 						return INF_MSGPROC_BREAK;
// 				}
				if(m_mapCityShop[BUILDINGKIND_CASH_SHOP]->WndProc(uMsg, wParam, lParam) == INF_MSGPROC_BREAK )
				{
					return INF_MSGPROC_BREAK;		
				}
				// end 2009-02-24 by bhsohn 캐쉬샵 충전 웹페이지 추가
			}
			break;
			
			// 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템
		// 인피니티 입장 상점
		case BUILDINGNPC_RENDER_INFINITYFIELD:
			{
				if( m_mapCityShop[BUILDINGKIND_INFINITY]->WndProc( uMsg, wParam, lParam ) == INF_MSGPROC_BREAK )
					return INF_MSGPROC_BREAK;
			}
			break;
		// 인피니티 판매 상점
		case BUILDINGNPC_RENDER_INFINITYSHOP:
			{
				if( m_mapCityShop[BUILDINGKIND_INFINITY_SHOP]->WndProc( uMsg, wParam, lParam ) == INF_MSGPROC_BREAK )
					return INF_MSGPROC_BREAK;
			}
			break;
			// end 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템
			// 2013-05-07 by bhsohn 세력포인트 개선안 시스템
		case BUILDINGNPC_RENDER_INFLBUFFSHOP:
			{
#ifdef C_UPGRADE_INFLUENCE_POINT_BHSOHN			
				if( m_mapCityShop[BUILDINGKIND_INFLBUFF_SHOP]->WndProc( uMsg, wParam, lParam ) == INF_MSGPROC_BREAK )
					return INF_MSGPROC_BREAK;
#endif
			}
			break;
			// END 2013-05-07 by bhsohn 세력포인트 개선안 시스템

		case BUILDINGNPC_RENDER_ARENA:
			{
				if(m_mapCityShop[BUILDINGKIND_ARENA]->WndProc(uMsg, wParam, lParam) == INF_MSGPROC_BREAK )
					return INF_MSGPROC_BREAK;
			}
			break;
		case BUILDINGNPC_RENDER_WARPOINT:
			{
				if(m_mapCityShop[BUILDINGKIND_WARPOINT_SHOP]->WndProc(uMsg, wParam, lParam) == INF_MSGPROC_BREAK )
					return INF_MSGPROC_BREAK;
				if(g_pGameMain->m_pInven->WndProcOnCityBase( uMsg, wParam, lParam ) == INF_MSGPROC_BREAK)
					return INF_MSGPROC_BREAK;
			}
			break;
		case BUILDINGNPC_RENDER_INFLUENCE:
			{
				if(m_pCurrentBuildingNPC->buildingInfo.BuildingKind == BUILDINGKIND_CITYLEADER_OUTPOST)
				{
					if(m_mapCityShop[BUILDINGKIND_CITYLEADER_OUTPOST]->WndProc(uMsg,wParam,lParam) == INF_MSGPROC_BREAK)
						return INF_MSGPROC_BREAK;
				}
				if(m_pCurrentBuildingNPC->buildingInfo.BuildingKind == BUILDINGKIND_CITYLEADER_LEADER)
				{
					if(m_mapCityShop[BUILDINGKIND_CITYLEADER_LEADER]->WndProc(uMsg,wParam,lParam) == INF_MSGPROC_BREAK)
						return INF_MSGPROC_BREAK;
				}
			}
			break;
		// 2008-11-04 by dgwoo LuckyMachine기획 추가.
		case BUILDINGNPC_RENDER_LUCKYMACHINE:
			{
				if(m_pCurrentBuildingNPC->buildingInfo.BuildingKind == BUILDINGKIND_LUCKY)
				{
					if(m_mapCityShop[BUILDINGKIND_LUCKY]->WndProc(uMsg,wParam,lParam) == INF_MSGPROC_BREAK)
						return INF_MSGPROC_BREAK;
				}
			}
			break;
		// 2010. 04. 21 by jskim 신규 럭키 머신 구현
		case BUILDINGNPC_RENDER_OPTIONMACHINE:
			{
				if(m_pCurrentBuildingNPC->buildingInfo.BuildingKind == BUILDINGKIND_LUCKY_OPTION_MACHINE)
				{
					if(m_mapCityShop[BUILDINGKIND_LUCKY_OPTION_MACHINE]->WndProc(uMsg,wParam,lParam) == INF_MSGPROC_BREAK)
						return INF_MSGPROC_BREAK;
				}
			}
			break;
		//end 2010. 04. 21 by jskim 신규 럭키 머신 구현
		// 2010-08-31 by shcho&&jskim, 용해 시스템 구현
		case BUILDINGKIND_RENDER_DISSOLUTION:
			{
				if(m_pCurrentBuildingNPC->buildingInfo.BuildingKind == BUILDINGKIND_DISSOLUTION)
				{
					if(m_mapCityShop[BUILDINGKIND_DISSOLUTION]->WndProc(uMsg,wParam,lParam) == INF_MSGPROC_BREAK)
						return INF_MSGPROC_BREAK;
				}
			}
			break;
		// end 2010-08-31 by shcho&&jskim, 용해 시스템 구현
		case BUILDINGNPC_RENDER_FACTORY:
			{
				if(m_pCurrentBuildingNPC->buildingInfo.BuildingKind == BUILDINGKIND_FACTORY)
				{
					if(m_mapCityShop[BUILDINGKIND_FACTORY]->WndProc(uMsg,wParam,lParam) == INF_MSGPROC_BREAK)
						return INF_MSGPROC_BREAK;
				}
			}
			break;
		case BUILDINGNPC_RENDER_LABORATORY:
			{
				if(m_pCurrentBuildingNPC->buildingInfo.BuildingKind == BUILDINGKIND_LABORATORY)
				{
					if(m_mapCityShop[BUILDINGKIND_LABORATORY]->WndProc(uMsg,wParam,lParam) == INF_MSGPROC_BREAK)
						return INF_MSGPROC_BREAK;
				}
			}
			break;
		}
	}
	
	if(m_pCurrentBuildingNPC != NULL && 
		m_pCurrentBuildingNPC->buildingInfo.BuildingKind == BUILDINGKIND_CASH_SHOP &&
		g_pD3dApp->m_dwGameState != _CITY)
	{
		if(WM_KEYDOWN == uMsg && wParam == VK_ESCAPE && m_pCurrentBuildingNPC)
		{
			OnCityNPCButtonDown(CITY_NPC_BUTTON_CLOSE);
			g_pGameMain->SetItemInfo( 0, 0, 0, 0 );
			return INF_MSGPROC_BREAK;
		}
		
		g_pGameMain->SetItemInfo( 0, 0, 0, 0 );
		
		return INF_MSGPROC_NORMAL;
	}	
		
	switch(uMsg)
	{
	case WM_KEYDOWN:
		{
			if(wParam == VK_ESCAPE && m_pCurrentBuildingNPC)
			{
				OnCityNPCButtonDown(CITY_NPC_BUTTON_CLOSE);
			}
			// 2009-02-03 by bhsohn 장착 아이템 비교 툴팁
			//g_pGameMain->SetItemInfo( 0, 0, 0, 0 );
			BOOL bSetTooltip = TRUE;
			if((VK_SHIFT == wParam) && g_pGameMain && g_pGameMain->m_pInven && g_pGameMain->m_pInven->IsShowInven())
			{
				// Shit를 눌렀다.
				bSetTooltip = FALSE;
			}
			if(bSetTooltip)
			{
				g_pGameMain->SetItemInfo( 0, 0, 0, 0 );
			}
			// end 2009-02-03 by bhsohn 장착 아이템 비교 툴팁

			
		}
		break;
	case WM_MOUSEMOVE:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);
			int nStartX = CITY_BASE_DOWN_BUTTON_START_X(m_mapButtonState.size());
			int nStartY = CITY_BASE_DOWN_BUTTON_START_Y;
			if(pt.y > nStartY && pt.y < nStartY + CITY_BASE_BUTTON_SIZE_Y)
			{
				for(int i=0;i<m_mapButtonState.size();i++)
				{
					if(m_mapButtonState[i] != BUTTON_STATE_DISABLE)
					{
//						if(pt.x > nStartX + CITY_BASE_BUTTON_SIZE_X*i && 
//							pt.x < nStartX + CITY_BASE_BUTTON_SIZE_X*(i+1))
						if(pt.x > nStartX + CITY_BASE_NPC_BUTTON_POS_X_INTERVAL*i && 
							pt.x < nStartX + CITY_BASE_NPC_BUTTON_POS_X_INTERVAL*i+CITY_BASE_BUTTON_SIZE_X)
						{
							if( m_mapButtonState[i] != BUTTON_STATE_DOWN)
								m_mapButtonState[i] = BUTTON_STATE_UP;
						}
						else 
						{
							m_mapButtonState[i] = BUTTON_STATE_NORMAL;
						}
					}
				}
// 2006-06-12 by ispark, 옛날 엠게임 코드
//				if( pt.x > CITY_BASE_DOWN_OUTDOOR_BUTTON_START_X && 
//					pt.x < CITY_BASE_DOWN_OUTDOOR_BUTTON_START_X+CITY_BASE_BUTTON_SIZE_X)
//				{
//					if(m_nOutDoorButtonState != BUTTON_STATE_DOWN)
//						m_nOutDoorButtonState = BUTTON_STATE_UP;
//				}
//				else 
//				{
//					m_nOutDoorButtonState = BUTTON_STATE_NORMAL;
//				}
			}
			else
			{
				for(int i=0;i<m_mapButtonState.size();i++)
				{
					if(m_mapButtonState[i] != BUTTON_STATE_DISABLE)
					{
						m_mapButtonState[i] = BUTTON_STATE_NORMAL;
					}
				}
				m_nOutDoorButtonState = BUTTON_STATE_NORMAL;
			}
			if(m_pCurrentBuildingNPC)
			{
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
				m_nCancelB = BUTTON_STATE_NORMAL;
				if( pt.y > CITY_BASE_NPC_BUTTON_START_Y - 580 && 
					pt.y < CITY_BASE_NPC_BUTTON_START_Y - 580 + CITY_BASE_BUTTON_SIZE_Y)
				{
					{
						if(pt.x > CITY_BASE_NPC_CANCEL_BUTTON_X &&
							pt.x < CITY_BASE_NPC_CANCEL_BUTTON_X + CITY_BASE_BUTTON_SIZE_X)
						{
							m_nCancelB = BUTTON_STATE_UP;
						}
						else
						{
							m_nCancelB = BUTTON_STATE_NORMAL;
						}
					}
				}
#else
				if( pt.y > CITY_BASE_NPC_BUTTON_START_Y && 
					pt.y < CITY_BASE_NPC_BUTTON_START_Y + CITY_BASE_BUTTON_SIZE_Y)

				{
					if( pt.x > CITY_BASE_NPC_BUTTON_START_X && 
						pt.x < CITY_BASE_NPC_BUTTON_START_X+CITY_BASE_BUTTON_SIZE_X)
					{
						if(m_nNPCButtonState[0] != BUTTON_STATE_DOWN)
							m_nNPCButtonState[0] = BUTTON_STATE_UP;
					}
					else
					{
						m_nNPCButtonState[0] = BUTTON_STATE_NORMAL;
					}
					if( !IS_MISSION_SHOP_TYPE(m_pCurrentBuildingNPC->buildingInfo.BuildingKind) && m_pMission->IsExistMission() )
					{
						if( pt.x > CITY_BASE_NPC_BUTTON_START_X+CITY_BASE_NPC_BUTTON_POS_X_INTERVAL && 
							pt.x < CITY_BASE_NPC_BUTTON_START_X+CITY_BASE_NPC_BUTTON_POS_X_INTERVAL+CITY_BASE_BUTTON_SIZE_X)
						{
							if(m_nNPCButtonState[1] != BUTTON_STATE_DOWN)
								m_nNPCButtonState[1] = BUTTON_STATE_UP;
						}
						else 
						{
							m_nNPCButtonState[1] = BUTTON_STATE_NORMAL;
						}
					}
					if(pt.x > CITY_BASE_NPC_CREATE_BUTTON_X &&
						pt.x < CITY_BASE_NPC_CREATE_BUTTON_X+CITY_BASE_BUTTON_SIZE_X)
					{
						m_nCreateB = BUTTON_STATE_UP;
					}
					else
					{
						m_nCreateB = BUTTON_STATE_NORMAL;
					}


					if( pt.x > CITY_BASE_NPC_CLOSE_BUTTON_START_X && 
						pt.x < CITY_BASE_NPC_CLOSE_BUTTON_START_X+CITY_BASE_BUTTON_SIZE_X)
					{
						if(m_nNPCButtonState[2] != BUTTON_STATE_DOWN)
							m_nNPCButtonState[2] = BUTTON_STATE_UP;
					}
					else 
					{
						m_nNPCButtonState[2] = BUTTON_STATE_NORMAL;
					}
				}
				else
				{
					m_nNPCButtonState[0] = BUTTON_STATE_NORMAL;
					m_nNPCButtonState[1] = BUTTON_STATE_NORMAL;
					m_nNPCButtonState[2] = BUTTON_STATE_NORMAL;
					m_nCreateB = BUTTON_STATE_NORMAL;
				}
				if(pt.x > CITY_BASE_NPC_QUICKAPP_BUTTON_X
					&& pt.x < CITY_BASE_NPC_QUICKAPP_BUTTON_X + CITY_BASE_NPC_QUICKAPP_BUTTON_W
					&& pt.y > CITY_BASE_NPC_QUICKAPP_BUTTON_Y
					&& pt.y < CITY_BASE_NPC_QUICKAPP_BUTTON_Y + CITY_BASE_NPC_QUICKAPP_BUTTON_H)
				{
					m_nQuickAppB = BUTTON_STATE_UP;
				}
				else
				{
					m_nQuickAppB = BUTTON_STATE_NORMAL;
				}

				if( pt.y > CITY_BASE_NPC_APP_BUTTON_Y && 
					pt.y < CITY_BASE_NPC_APP_BUTTON_Y + CITY_BASE_BUTTON_SIZE_Y)
				{
					if(pt.x > CITY_BASE_NPC_APP_BUTTON_X &&
						pt.x < CITY_BASE_NPC_APP_BUTTON_X + CITY_BASE_BUTTON_SIZE_X)
					{
						m_nAppB = BUTTON_STATE_UP;
					}
					else
					{
						m_nAppB = BUTTON_STATE_NORMAL;
					}
					if(pt.x > CITY_BASE_NPC_CANCEL_BUTTON_X &&
						pt.x < CITY_BASE_NPC_CANCEL_BUTTON_X + CITY_BASE_BUTTON_SIZE_X)
					{
						m_nCancelB = BUTTON_STATE_UP;
					}
					else
					{
						m_nCancelB = BUTTON_STATE_NORMAL;
					}
				}
				else
				{
					m_nAppB = BUTTON_STATE_NORMAL;
					m_nCancelB = BUTTON_STATE_NORMAL;
				}
#endif
			}
		}

		break;
	case WM_LBUTTONDOWN:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);
			int nStartX = CITY_BASE_DOWN_BUTTON_START_X(m_mapButtonState.size());
			int nStartY = CITY_BASE_DOWN_BUTTON_START_Y;
			if(pt.y > nStartY && pt.y < nStartY + CITY_BASE_BUTTON_SIZE_Y)
			{
				for(int i=0;i<m_mapButtonState.size();i++)
				{
					if(m_mapButtonState[i] != BUTTON_STATE_DISABLE)
					{
//						if(pt.x > nStartX + CITY_BASE_BUTTON_SIZE_X*i && 
//							pt.x < nStartX + CITY_BASE_BUTTON_SIZE_X*(i+1))
						if(pt.x > nStartX + CITY_BASE_NPC_BUTTON_POS_X_INTERVAL*i && 
							pt.x < nStartX + CITY_BASE_NPC_BUTTON_POS_X_INTERVAL*i+CITY_BASE_BUTTON_SIZE_X)
						{
							m_mapButtonState[i] = BUTTON_STATE_DOWN;
							g_pD3dApp->m_pSound->PlayD3DSound(SOUND_BUTTON_OK, D3DXVECTOR3(0,0,0), FALSE);
						}
						else 
						{
							m_mapButtonState[i] = BUTTON_STATE_NORMAL;
						}
					}
				}
// 2006-06-12 by ispark, 옛날 엠게임 코드
//				if( pt.x > CITY_BASE_DOWN_OUTDOOR_BUTTON_START_X && 
//					pt.x < CITY_BASE_DOWN_OUTDOOR_BUTTON_START_X+CITY_BASE_BUTTON_SIZE_X)
//				{
//					m_nOutDoorButtonState = BUTTON_STATE_DOWN;
//					g_pD3dApp->m_pSound->PlayD3DSound(SOUND_BUTTON_OK, D3DXVECTOR3(0,0,0), FALSE);
//				}
//				else 
//				{
//					m_nOutDoorButtonState = BUTTON_STATE_NORMAL;
//				}
			}
			if(m_pCurrentBuildingNPC)
			{
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
				if( pt.y > CITY_BASE_NPC_BUTTON_START_Y - 583 && 
					pt.y < CITY_BASE_NPC_BUTTON_START_Y - 583 + CITY_BASE_BUTTON_SIZE_Y)
				{
				
					{
						if(pt.x > CITY_BASE_NPC_CANCEL_BUTTON_X &&
							pt.x < CITY_BASE_NPC_CANCEL_BUTTON_X + CITY_BASE_BUTTON_SIZE_X)
						{
							m_nCancelB = BUTTON_STATE_DOWN;
						}
						else
						{
							m_nCancelB = BUTTON_STATE_NORMAL;
						}
					}
				}
#else
				if( pt.y > CITY_BASE_NPC_BUTTON_START_Y && 
					pt.y < CITY_BASE_NPC_BUTTON_START_Y + CITY_BASE_BUTTON_SIZE_Y)

				{
					if( pt.x > CITY_BASE_NPC_BUTTON_START_X && 
						pt.x < CITY_BASE_NPC_BUTTON_START_X+CITY_BASE_BUTTON_SIZE_X)
					{
//						m_nNPCButtonState[0] = BUTTON_STATE_DOWN;
//						g_pD3dApp->m_pSound->PlayD3DSound(SOUND_BUTTON_OK, D3DXVECTOR3(0,0,0), FALSE);
					}
					else
					{
						m_nNPCButtonState[0] = BUTTON_STATE_NORMAL;
					}
					if(pt.x > CITY_BASE_NPC_CREATE_BUTTON_X &&
						pt.x < CITY_BASE_NPC_CREATE_BUTTON_X+CITY_BASE_BUTTON_SIZE_X)
					{
						m_nCreateB = BUTTON_STATE_DOWN;
					}
					else
					{
						m_nCreateB = BUTTON_STATE_NORMAL;
					}
					// 2005-12-22 by ispark, EP1 미션 버튼 삭제
//					if( !IS_MISSION_SHOP_TYPE(m_pCurrentBuildingNPC->buildingInfo.BuildingKind) && m_pMission->IsExistMission() )
//					{
//						if( pt.x > CITY_BASE_NPC_BUTTON_START_X+CITY_BASE_NPC_BUTTON_POS_X_INTERVAL && 
//							pt.x < CITY_BASE_NPC_BUTTON_START_X+CITY_BASE_NPC_BUTTON_POS_X_INTERVAL+CITY_BASE_BUTTON_SIZE_X)
//						{
//							m_nNPCButtonState[1] = BUTTON_STATE_DOWN;
//							g_pD3dApp->m_pSound->PlayD3DSound(SOUND_BUTTON_OK, D3DXVECTOR3(0,0,0), FALSE);
//						}
//						else
//						{
//							m_nNPCButtonState[1] = BUTTON_STATE_NORMAL;
//						}
//					}
					if( pt.x > CITY_BASE_NPC_CLOSE_BUTTON_START_X && 
						pt.x < CITY_BASE_NPC_CLOSE_BUTTON_START_X+CITY_BASE_BUTTON_SIZE_X)
					{
						m_nNPCButtonState[2] = BUTTON_STATE_DOWN;
						g_pD3dApp->m_pSound->PlayD3DSound(SOUND_BUTTON_OK, D3DXVECTOR3(0,0,0), FALSE);
						CloseAllMessageBox();						
					}
					else
					{
						m_nNPCButtonState[2] = BUTTON_STATE_NORMAL;
					}
				}
				if(pt.x > CITY_BASE_NPC_QUICKAPP_BUTTON_X
					&& pt.x < CITY_BASE_NPC_QUICKAPP_BUTTON_X + CITY_BASE_NPC_QUICKAPP_BUTTON_W
					&& pt.y > CITY_BASE_NPC_QUICKAPP_BUTTON_Y
					&& pt.y < CITY_BASE_NPC_QUICKAPP_BUTTON_Y + CITY_BASE_NPC_QUICKAPP_BUTTON_H)
				{
					m_nQuickAppB = BUTTON_STATE_DOWN;
				}
				else
				{
					m_nQuickAppB = BUTTON_STATE_NORMAL;
				}

				if( pt.y > CITY_BASE_NPC_APP_BUTTON_Y && 
					pt.y < CITY_BASE_NPC_APP_BUTTON_Y + CITY_BASE_BUTTON_SIZE_Y)
				{
					if(pt.x > CITY_BASE_NPC_APP_BUTTON_X &&
						pt.x < CITY_BASE_NPC_APP_BUTTON_X + CITY_BASE_BUTTON_SIZE_X)
					{
						m_nAppB = BUTTON_STATE_DOWN;
					}
					else
					{
						m_nAppB = BUTTON_STATE_NORMAL;
					}
					if(pt.x > CITY_BASE_NPC_CANCEL_BUTTON_X &&
						pt.x < CITY_BASE_NPC_CANCEL_BUTTON_X + CITY_BASE_BUTTON_SIZE_X)
					{
						m_nCancelB = BUTTON_STATE_DOWN;
					}
					else
					{
						m_nCancelB = BUTTON_STATE_NORMAL;
					}
				}
#endif
			}
			
		}
		break;
	case WM_LBUTTONUP:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);
			int nStartX = CITY_BASE_DOWN_BUTTON_START_X(m_mapButtonState.size());
			int nStartY = CITY_BASE_DOWN_BUTTON_START_Y;
			if(pt.y > nStartY && pt.y < nStartY + CITY_BASE_BUTTON_SIZE_Y)
			{
				for(int i=0;i<m_mapButtonState.size();i++)
				{
					if(m_mapButtonState[i] != BUTTON_STATE_DISABLE)
					{
//						if(pt.x > nStartX + CITY_BASE_BUTTON_SIZE_X*i && 
//							pt.x < nStartX + CITY_BASE_BUTTON_SIZE_X*(i+1))
						if(pt.x > nStartX + CITY_BASE_NPC_BUTTON_POS_X_INTERVAL*i && 
							pt.x < nStartX + CITY_BASE_NPC_BUTTON_POS_X_INTERVAL*i+CITY_BASE_BUTTON_SIZE_X)
						{
							if(m_mapButtonState[i] == BUTTON_STATE_DOWN)
							{
								// 2007-08-09 by dgwoo 옛날 엠게임 코드
								//OnCityBaseButtonDown(i);
							}
							m_mapButtonState[i] = BUTTON_STATE_UP;
						}
						else 
						{
							m_mapButtonState[i] = BUTTON_STATE_NORMAL;
						}
					}
				}
// 2006-06-12 by ispark, 옛날 엠게임 코드
//				if( pt.x > CITY_BASE_DOWN_OUTDOOR_BUTTON_START_X && 
//					pt.x < CITY_BASE_DOWN_OUTDOOR_BUTTON_START_X+CITY_BASE_BUTTON_SIZE_X)
//				{
//					if(m_nOutDoorButtonState == BUTTON_STATE_DOWN)
//					{
//						OnCityBaseButtonDown(BUILDINGKIND_OUTDOOR);
//					}
//					m_nOutDoorButtonState = BUTTON_STATE_UP;
//				}
//				else 
//				{
//					m_nOutDoorButtonState = BUTTON_STATE_NORMAL;
//				}
			}
			
			// 캐쉬 상점 아래 처리없음 
			if(BUILDINGNPC_RENDER_CASH_SHOP == m_nCurrentBuildingNPCState)
				break;
			if(m_pCurrentBuildingNPC)
			{
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
				if( pt.y > CITY_BASE_NPC_BUTTON_START_Y - 583 && 
					pt.y < CITY_BASE_NPC_BUTTON_START_Y - 583 + CITY_BASE_BUTTON_SIZE_Y)
				{
				
					{
						if(pt.x > CITY_BASE_NPC_CANCEL_BUTTON_X &&
							pt.x < CITY_BASE_NPC_CANCEL_BUTTON_X + CITY_BASE_BUTTON_SIZE_X)
						{
							if(m_nCancelB == BUTTON_STATE_DOWN)
							{
								OnCityNPCButtonDown(CITY_NPC_BUTTON_CLOSE);
							}
							m_nCancelB = BUTTON_STATE_UP;
						}
					}

				}
#else
				if( pt.y > CITY_BASE_NPC_BUTTON_START_Y && 
					pt.y < CITY_BASE_NPC_BUTTON_START_Y + CITY_BASE_BUTTON_SIZE_Y)

				{
					if( pt.x > CITY_BASE_NPC_BUTTON_START_X && 
						pt.x < CITY_BASE_NPC_BUTTON_START_X+CITY_BASE_BUTTON_SIZE_X)
					{
						if(m_nNPCButtonState[0] == BUTTON_STATE_DOWN)
						{
							OnCityNPCButtonDown(CITY_NPC_BUTTON_MAIN);
						}
						m_nNPCButtonState[0] = BUTTON_STATE_UP;
					}
					else
					{
						m_nNPCButtonState[0] = BUTTON_STATE_NORMAL;
					}
					if( !IS_MISSION_SHOP_TYPE(m_pCurrentBuildingNPC->buildingInfo.BuildingKind) && m_pMission->IsExistMission() )
					{
						if( pt.x > CITY_BASE_NPC_BUTTON_START_X+CITY_BASE_NPC_BUTTON_POS_X_INTERVAL && 
							pt.x < CITY_BASE_NPC_BUTTON_START_X+CITY_BASE_NPC_BUTTON_POS_X_INTERVAL+CITY_BASE_BUTTON_SIZE_X)
						{
							if(m_nNPCButtonState[1] == BUTTON_STATE_DOWN)
							{
								OnCityNPCButtonDown(CITY_NPC_BUTTON_MISSION);
							}
							m_nNPCButtonState[1] = BUTTON_STATE_UP;
						}
						else
						{
							m_nNPCButtonState[1] = BUTTON_STATE_NORMAL;
						}
					}
					if( pt.x > CITY_BASE_NPC_CLOSE_BUTTON_START_X && 
						pt.x < CITY_BASE_NPC_CLOSE_BUTTON_START_X+CITY_BASE_BUTTON_SIZE_X)
					{
						if(m_nNPCButtonState[2] == BUTTON_STATE_DOWN)
						{
							OnCityNPCButtonDown(CITY_NPC_BUTTON_CLOSE);
						}
						m_nNPCButtonState[2] = BUTTON_STATE_UP;
					}
					else
					{
						m_nNPCButtonState[2] = BUTTON_STATE_NORMAL;
					}
					if(pt.x > CITY_BASE_NPC_CREATE_BUTTON_X &&
						pt.x < CITY_BASE_NPC_CREATE_BUTTON_X + CITY_BASE_BUTTON_SIZE_X)
					{
						if(m_nCreateB == BUTTON_STATE_DOWN)
						{
							OnCityNPCButtonDown(CITY_NPC_BUTTON_CREATE);
						}
						m_nCreateB = BUTTON_STATE_UP;
					}
				}
				if(pt.x > CITY_BASE_NPC_QUICKAPP_BUTTON_X
					&& pt.x < CITY_BASE_NPC_QUICKAPP_BUTTON_X + CITY_BASE_NPC_QUICKAPP_BUTTON_W
					&& pt.y > CITY_BASE_NPC_QUICKAPP_BUTTON_Y
					&& pt.y < CITY_BASE_NPC_QUICKAPP_BUTTON_Y + CITY_BASE_NPC_QUICKAPP_BUTTON_H)
				{
					if(m_nQuickAppB = BUTTON_STATE_DOWN)
					{
						OnCityNPCButtonDown(CITY_NPC_BUTTON_QUICK);
					}
					m_nQuickAppB = BUTTON_STATE_UP;
				}

				if(pt.y > CITY_BASE_NPC_APP_BUTTON_Y &&
					pt.y < CITY_BASE_NPC_APP_BUTTON_Y + CITY_BASE_BUTTON_SIZE_Y)
				{
					if(pt.x > CITY_BASE_NPC_APP_BUTTON_X &&
						pt.x < CITY_BASE_NPC_APP_BUTTON_X + CITY_BASE_BUTTON_SIZE_X)
					{
						if(m_nAppB == BUTTON_STATE_DOWN)
						{
							OnCityNPCButtonDown(CITY_NPC_BUTTON_APP);
						}
						m_nAppB = BUTTON_STATE_UP;
					}
					if(pt.x > CITY_BASE_NPC_CANCEL_BUTTON_X &&
						pt.x < CITY_BASE_NPC_CANCEL_BUTTON_X + CITY_BASE_BUTTON_SIZE_X)
					{
						if(m_nCancelB == BUTTON_STATE_DOWN)
						{
							OnCityNPCButtonDown(CITY_NPC_BUTTON_CANCEL);
						}
						m_nCancelB = BUTTON_STATE_UP;
					}
				}
#endif
			}
		}
		break;
	}
	return INF_MSGPROC_NORMAL;
}

void CINFCityBase::SetNPCTalkInfo( char* strNPCTalk, int nType)
{
	FLOG( "CINFItemInfo::SetDesc( int nParameterIndex, char* strNPCTalk )" );
//	memset(m_strNPCTalk, 0x00, CITY_NPCTALK_SIZE*CITY_NPCTALK_LINE_STRING_NUMBER);

	if(m_pCurrentBuildingNPC != NULL && m_pCurrentBuildingNPC->buildingInfo.BuildingKind == BUILDINGKIND_CITY_OCCUPY_INFO)
	{
		if(strlen(m_strGuildMasterGr)>0)
			strNPCTalk = m_strGuildMasterGr;
	}
	
	char szBuffer[CITY_NPCTALK_SIZE];
	memset( szBuffer, 0x00, CITY_NPCTALK_SIZE );	
	
	if(!nType)
	{
		m_vecNPCTalk.clear();
	}
	else
	{
		char buftemp[64];
		memset(buftemp, 0x00, 64);
		memcpy( szBuffer, buftemp, sizeof(buftemp));
		m_vecNPCTalk.push_back( (string)szBuffer );
		sprintf(buftemp,"\\g%s\\g", STRMSG_C_CITY_0001);//"[미션목표]"
		memcpy( szBuffer, buftemp, sizeof(buftemp));
		m_vecNPCTalk.push_back( (string)szBuffer );
	}
	if(strlen(strNPCTalk) == 0)
	{
		DBGOUT("strNPCTalk is Null.\n");
		return;
	}

//#if defined(LANGUAGE_ENGLISH) || defined(LANGUAGE_VIETNAM)|| defined(LANGUAGE_THAI)// 2008-04-30 by bhsohn 태국 버전 추가
//	::StringCullingUserData3(strNPCTalk, _SHOP, CITY_NPCTALK_SIZE, &m_vecNPCTalk);
//#else
	::StringCullingUserData(strNPCTalk, CITY_NPCTALK_SIZE, &m_vecNPCTalk);
//#endif
// 2005-08-04 by ydkim, 스트링 컬링 함수 교체
//	int i = 0;
//	int nPoint = 0;
//	int nCheckPoint = 0;
//	int nBreakPoint = 0;
//	int nLine = 0;
//	int nLineLength = 0;
//	if(strlen(strNPCTalk) > CITY_NPCTALK_SIZE+CITY_NPCTALK_SIZE/2 ||
//		strlen(strNPCTalk) < CITY_NPCTALK_SIZE/3*2)
//	{
//		nLineLength = CITY_NPCTALK_SIZE-1;
//	}
//	else 
//	{
//		nLineLength = strlen(strNPCTalk)/3 * 2 - 1;
//	}
//	while(TRUE)
//	{
//		if(strNPCTalk[i] == ' ' || strNPCTalk[i] == '.' || strNPCTalk[i] == '!' || strNPCTalk[i] == NULL)
//		{
//			//if(nPoint >= CITY_NPCTALK_SIZE-1)
//			if(nPoint >= nLineLength)
//			{
////				if(nLine >= CITY_NPCTALK_LINE_STRING_NUMBER)
////				{
////					DBGOUT("CINFCityBase::SetNPCTalkInfo(),1 NPC대화가 너무 깁니다.제작자에게 줄이라고 하십시요.\n");
////					return;
////				}
////				memcpy(m_strNPCTalk[nLine], strNPCTalk + nCheckPoint, nBreakPoint+1);
//				memcpy( szBuffer, strNPCTalk + nCheckPoint, nBreakPoint+1);
//				m_vecNPCTalk.push_back( (string)szBuffer );
//				memset( szBuffer, 0x00, CITY_NPCTALK_SIZE );
//				
//				nPoint -= nBreakPoint;
//				nCheckPoint += nBreakPoint+1;
//				nBreakPoint = nPoint-1;
//				nLine ++;
//				i++;
//				continue;
//			}
//			if(strNPCTalk[i] == NULL)
//			{
////				if(nLine >= CITY_NPCTALK_LINE_STRING_NUMBER)
////				{
////					DBGOUT("CINFCityBase::SetNPCTalkInfo(),2 NPC대화가 너무 깁니다.제작자에게 줄이라고 하십시요.\n");
////					return;
////				}
////				memcpy(m_strNPCTalk[nLine], strNPCTalk + nCheckPoint, nPoint);
//				memcpy( szBuffer, strNPCTalk + nCheckPoint, nPoint);
//				m_vecNPCTalk.push_back( (string)szBuffer );
//				memset( szBuffer, 0x00, CITY_NPCTALK_SIZE );
//				break;
//			}
//			nBreakPoint = nPoint;
//		}// end_if(strNPCTalk[i] == ' ' || strNPCTalk[i] == '.' || strNPCTalk[i] == '!' || strNPCTalk[i] == NULL)
//		else if ()
//		{
//		}
//
//		i++;
//		nPoint++;
//	}
	m_pNPCScroll->Reset();
	m_pNPCScroll->SetNumberOfData( m_vecNPCTalk.size() );
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			CINFCityBase::OnCityBaseButtonDown(int nButton)
/// \brief		화면 하단에 있는 버튼 클릭
/// \author		dhkwon
/// \date		2004-04-13 ~ 2004-04-13
/// \warning	m_pCurrentBuildingNPC 설정
///				예전 엠게임 코드로 사용하지 않음.
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFCityBase::OnCityBaseButtonDown(int nButton)
{
	if(nButton == BUILDINGKIND_OUTDOOR)
	{
		if(g_pD3dApp->m_dwGameState == _CITY)
		{
			if( g_pStoreData->FindItemInInventoryByWindowPos( POS_REAR ) == NULL )
			{
				g_pD3dApp->m_pChat->CreateChatChild( STRMSG_C_CITY_0002, COLOR_ERROR );//"엔진이 없으면 출격 할 수 없습니다."
				return;
			}
			DBGOUT("CITY_NPC_BUTTON_CLOSE\n");
			SendLeaveEnterBuilding( m_nCurrentEnterBuildingIndex, -1 );
			m_pCurrentBuildingNPC = NULL;
			CloseAllMessageBox();
			g_pGameMain->m_pInfWindow->AddMsgBox( STRMSG_C_CITY_0003, _Q_OUTDOOR_WARP );//"격납고를 나갑니다."
		}
	}
	else
	{
//		CMapCityGroupIterator it = m_mapButtonGroup.begin();
//		for(int i=0;i<nButton;i++)
//		{
//			ASSERT_ASSERT(it != m_mapButtonGroup.end());
//			it++;
//		}
		vector<GUI_BUILDINGNPC*>::iterator it = m_vecButtonGroup.begin();
		for(int i=0;i<nButton;i++)
		{
			ASSERT_ASSERT(it != m_vecButtonGroup.end());
			it++;
		}
//		if(it != m_mapButtonGroup.end())
		if(it != m_vecButtonGroup.end())
		{
//			if(m_pCurrentBuildingNPC != it->second)
			if(m_pCurrentBuildingNPC != (*it))
			{
//				BUILDINGNPC* pInfo = g_pDatabase->GetServerBuildingNPCInfo((it->second)->buildingInfo.BuildingIndex);
				BUILDINGNPC* pInfo = g_pDatabase->GetServerBuildingNPCInfo((*it)->buildingInfo.BuildingIndex);
				if(pInfo)
				{
//					memcpy(&((it->second)->buildingInfo), pInfo, sizeof(BUILDINGNPC));
					memcpy(&((*it)->buildingInfo), pInfo, sizeof(BUILDINGNPC));
				}
//				m_pCurrentBuildingNPC =it->second;
				m_pCurrentBuildingNPC = *it;

				// NPC 캐쉬상점은 없으므로 안보여준다 바로 상점으로 들어간다
				if(m_pCurrentBuildingNPC->buildingInfo.BuildingKind != BUILDINGKIND_CASH_SHOP)
				{
					m_nCurrentBuildingNPCState = BUILDINGNPC_RENDER_TALKBOX;
					SetNPCTalkInfo( m_pCurrentBuildingNPC->buildingInfo.GreetingTalk);
					m_pMission->Reset();
					m_pNPCScroll->Reset();
					g_pQuestData->GetQuestList(m_pCurrentBuildingNPC->buildingInfo.NPCIndex);
					if(m_pNPCImage)
					{
						m_pNPCImage->InvalidateDeviceObjects();
						m_pNPCImage->DeleteDeviceObjects();
					}
					m_pNPCImage = LoadNPCImage(m_pCurrentBuildingNPC->buildingInfo.NPCIndex);
					m_fCurrentShowTimeTime = 0;
				}
				else
				{
					OnCityNPCButtonDown(CITY_NPC_BUTTON_MAIN);
				}

				g_pGameMain->RightWindowShow(FALSE,g_pGameMain->m_nRightWindowInfo);
				g_pGameMain->LeftWindowShow(FALSE,g_pGameMain->m_nLeftWindowInfo);
			}
		}
		else
		{
			m_pCurrentBuildingNPC = NULL;
			DBGOUT("ERROR CINFCityBase::OnCityBaseButtonDown(int nButton) unable Shop Button .\n");
			SetNPCTalkInfo( NULL );
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			CINFCityBase::OnCityNPCButtonDown(int nButton)
/// \brief		NPC 박스에 있는 버튼 클릭
/// \author		dhkwon
/// \date		2004-04-13 ~ 2004-04-13
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFCityBase::OnCityNPCButtonDown(int nButton)
{
	switch(nButton)
	{
	case CITY_NPC_BUTTON_MAIN:
		{
			ASSERT_ASSERT(m_pCurrentBuildingNPC);
			OnCityNPCMainMenu(m_pCurrentBuildingNPC);
		}
		break;
	case CITY_NPC_BUTTON_MISSION: // OnCityNPCMainMenu에도 같은 코드 존재
		{
			// ENTER BUIDLING
			SendLeaveEnterBuilding(m_nCurrentEnterBuildingIndex, m_pCurrentBuildingNPC->buildingInfo.BuildingIndex);
//			m_pMission->Reset();
//			g_pQuestData->GetQuestList(m_pCurrentBuildingNPC->buildingInfo.NPCIndex);
			m_nCurrentBuildingNPCState = BUILDINGNPC_RENDER_MISSION;
		}
		break;
	case CITY_NPC_BUTTON_CLOSE:
		{
			// 2008-12-01 by dgwoo 룰렛을 돌리는중에는 창을 닫을수 없음.
			if(BUILDINGNPC_RENDER_LUCKYMACHINE == m_nCurrentBuildingNPCState)
			{
				CMapCityShopIterator it = m_mapCityShop.find(BUILDINGKIND_LUCKY);
				if(it != m_mapCityShop.end())
				{
					if(!((CINFLuckyMachine*)it->second)->IsCloseLucky())
					{
						g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_081202_0103, COLOR_SYSTEM);
						return;
					}
				}
				
			}
			// 2010. 04. 21 by jskim 신규 럭키 머신 구현
			if(BUILDINGNPC_RENDER_OPTIONMACHINE == m_nCurrentBuildingNPCState)
			{
				CMapCityShopIterator it = m_mapCityShop.find(BUILDINGKIND_LUCKY_OPTION_MACHINE);
				if(it != m_mapCityShop.end())
				{
 					if(!((CINFOptionMachine*)it->second)->IsCloseOptionMachine())
 					{
 						g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_100421_0411, COLOR_SYSTEM);
 						return;
 					}
				}				
			}
			//end 2010. 04. 21 by jskim 신규 럭키 머신 구현
			// 2010-08-31 by shcho&&jskim, 용해 시스템 구현
			if(BUILDINGKIND_RENDER_DISSOLUTION == m_nCurrentBuildingNPCState)
			{
				CMapCityShopIterator it = m_mapCityShop.find(BUILDINGKIND_DISSOLUTION);
				if(it != m_mapCityShop.end())
				{
					if(!((CINFDissolution*)it->second)->IsCloseDissolution())
					{
						g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_100901_0401, COLOR_SYSTEM); //"아이템 용해 중에는 인터페이스를 닫을 수 없습니다."
						return;
					}
				}		
			}
			// end 2010-08-31 by shcho&&jskim, 용해 시스템 구현
			// 2007-02-12 by bhsohn Item 다중 선택 처리
			g_pD3dApp->DelMultiItemList(TRUE);
			// end 2007-02-12 by bhsohn Item 다중 선택 처리
			
			// 2008-08-22 by bhsohn EP3 인벤토리 처리
			g_pGameMain->m_pInven->ShowInven(NULL, NULL, FALSE, TRUE);
			// end 2008-08-22 by bhsohn EP3 인벤토리 처리

			// 2005-11-11 by ispark
			// 현재 상점 닫을 수 있는 함수로 만들었음
//			DBGOUT("CITY_NPC_BUTTON_CLOSE\n");
//			SendLeaveEnterBuilding( m_nCurrentEnterBuildingIndex, -1 );
//			m_pCurrentBuildingNPC = NULL;

			// 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템

			// 인피니티 필드 npc 닫기 전에 인피니티와 관련된 데이터 제거
			if( m_nCurrentBuildingNPCState == BUILDINGNPC_RENDER_INFINITYFIELD )
			{
				CMapCityShopIterator it = m_mapCityShop.find(BUILDINGKIND_INFINITY);
				if(it != m_mapCityShop.end())
					((CINFCityInfinityField*)it->second)->Reset();
			}

			// end 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템

			CloseCurrentEnterBuilding();
			// 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현
			g_pInterface->m_pToolTip->m_bToolTipState = FALSE;
			// end 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현			
		}
		break;
	case CITY_NPC_BUTTON_CREATE:
		{
			if(BUILDINGNPC_RENDER_ARENA == m_nCurrentBuildingNPCState)
			{
				CMapCityShopIterator it = m_mapCityShop.find(BUILDINGKIND_ARENA);
				if(it != m_mapCityShop.end())
				{
					((CINFCityArena*)it->second)->CreateRoom();
				}

			}

		}
		break;
	case CITY_NPC_BUTTON_APP:
		{
			if(BUILDINGNPC_RENDER_ARENA == m_nCurrentBuildingNPCState)
			{
				CMapCityShopIterator it = m_mapCityShop.find(BUILDINGKIND_ARENA);
				if(it != m_mapCityShop.end())
				{
					((CINFCityArena*)it->second)->App();
				}
			}

		}
		break;
	case CITY_NPC_BUTTON_CANCEL:
		{
			if(BUILDINGNPC_RENDER_ARENA == m_nCurrentBuildingNPCState)
			{
				CMapCityShopIterator it = m_mapCityShop.find(BUILDINGKIND_ARENA);
				if(it != m_mapCityShop.end())
				{
					((CINFCityArena*)it->second)->AppCancel();
				}
			}

		}
		break;
	case CITY_NPC_BUTTON_QUICK:
		{
			if(BUILDINGNPC_RENDER_ARENA == m_nCurrentBuildingNPCState)
			{
				CMapCityShopIterator it = m_mapCityShop.find(BUILDINGKIND_ARENA);
				if(it != m_mapCityShop.end())
				{
					((CINFCityArena*)it->second)->QuickApp();
				}
			}

		}
		break;
	}
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			OnCityNPCMainMenu(GUI_BUILDINGNPC* pBuilding)
/// \brief		각 상점별로 고유 메뉴 처리
/// \author		dhkwon
/// \date		2004-06-25 ~ 2004-06-25
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFCityBase::OnCityNPCMainMenu(GUI_BUILDINGNPC* pBuilding)
{
	CMapCityShopIterator it = m_mapCityShop.find(pBuilding->buildingInfo.BuildingKind);
	
	/////////////////////////////////////////////////////////////////////////
	// 설정 초기화 2005-04-15 by ydkim
	CMapCityShopIterator it2 = m_mapCityShop.find(BUILDINGKIND_LABORATORY);
	if(it2 != m_mapCityShop.end())
	{
		((CINFCityLab*)it2->second)->InitData();	
		// 2008-03-14 by bhsohn 조합식 개선안
		((CINFCityLab*)it2->second)->OnOpenInfWnd();
	}
	CMapCityShopIterator it3 = m_mapCityShop.find(BUILDINGKIND_FACTORY);
	if(it3 != m_mapCityShop.end()) 
	{
		((CINFCityLab*)it3->second)->InitData();
		// 2008-03-14 by bhsohn 조합식 개선안
		((CINFCityLab*)it3->second)->OnOpenInfWnd();
	}
	//
	/////////////////////////////////////////////////////////////////////////

	// 2007-02-12 by bhsohn Item 다중 선택 처리
	// 다중 선택 항목 초기화
	g_pD3dApp->DelMultiItemList(TRUE);
	// end 2007-02-12 by bhsohn Item 다중 선택 처리

	switch(pBuilding->buildingInfo.BuildingKind)
	{
//2011-12-08 by jhahn EP4 트리거 상점 UI 변경
	case BUILDINGKIND_TRIGGER_CRYSTAL:
		{
			if(it == m_mapCityShop.end())
			{
				BUILDINGNPC* pInfo = g_pDatabase->GetServerBuildingNPCInfo(pBuilding->buildingInfo.BuildingIndex);
				CINFCityShop* pCityShop = new CINFCityShop(g_pGameMain, pInfo);
				pCityShop->SetGameData( g_pGameMain->m_pGameData );
				pCityShop->InitDeviceObjects();
				pCityShop->RestoreDeviceObjects();
				m_mapCityShop[pBuilding->buildingInfo.BuildingKind] = pCityShop;
			}
			SendLeaveEnterBuilding(m_nCurrentEnterBuildingIndex, pBuilding->buildingInfo.BuildingIndex);
			m_nCurrentBuildingNPCState = BUILDINGNPC_RENDER_SHOP;

		}
		break;								 
//end 2011-12-08 by jhahn EP4 트리거 상점 UI 변경
	case BUILDINGKIND_1ST_WEAPONSHOP:
	case BUILDINGKIND_2ND_WEAPONSHOP:
	case BUILDINGKIND_COMPONENTSHOP:
	case BUILDINGKIND_PETSHOP:
	case BUILDINGKIND_ANTIQUESHOP:
	case BUILDINGKIND_TUNING_COLOR:
	case BUILDINGKIND_PARTSSHOP: // 2012-02-29 by mspark, 상점 타이틀 넣기 - 파츠상점
	case BUILDINGKIND_GEARSHOP: // 2012-02-29 by mspark, 상점 타이틀 넣기 - 기어상점
	case BUILDINGKIND_CARDSHOP: // 2012-02-29 by mspark, 상점 타이틀 넣기 - 카드상점
//	case BUILDINGKIND_REFINERY:				// 2005-11-07 by ispark 수리 상점 -> 보급 상점
		{
            if(it == m_mapCityShop.end())
			{
				BUILDINGNPC* pInfo = g_pDatabase->GetServerBuildingNPCInfo(pBuilding->buildingInfo.BuildingIndex);
				CINFCityShop* pCityShop = new CINFCityShop(g_pGameMain, pInfo);
				pCityShop->SetGameData( g_pGameMain->m_pGameData );
				pCityShop->InitDeviceObjects();
				pCityShop->RestoreDeviceObjects();
				m_mapCityShop[pBuilding->buildingInfo.BuildingKind] = pCityShop;
			}
			SendLeaveEnterBuilding(m_nCurrentEnterBuildingIndex, pBuilding->buildingInfo.BuildingIndex);
			m_nCurrentBuildingNPCState = BUILDINGNPC_RENDER_SHOP;
		}
		break;
	case BUILDINGKIND_CITYWARP:
		{
			SendLeaveEnterBuilding(m_nCurrentEnterBuildingIndex, pBuilding->buildingInfo.BuildingIndex);
			if(it == m_mapCityShop.end())
			{
				CINFCityWarp* pCityWarp = new CINFCityWarp(g_pGameMain);				
				pCityWarp->SetGameData( g_pGameMain->m_pGameData );
				pCityWarp->InitDeviceObjects();
				pCityWarp->RestoreDeviceObjects();
				m_mapCityShop[pBuilding->buildingInfo.BuildingKind] = pCityWarp;
				// 워프 리스트는 한번만 요청한다.
				g_pD3dApp->SendFieldSocketGetShopWarpTargetMapList( g_pShuttleChild->m_myShuttleInfo.MapChannelIndex );
			}
			m_nCurrentBuildingNPCState = BUILDINGNPC_RENDER_SHOP;
		}
		break;
	case BUILDINGKIND_TUTORIAL:
		{
			g_pGameMain->m_pInfWindow->AddMsgBox( STRMSG_C_070716_0100, _Q_TUTORIAL_START);		//"튜토리얼 모드로 이동하시겠습니까?"
			m_nCurrentBuildingNPCState = BUILDINGNPC_RENDER_MSGBOX;
			
		}
		break;
	case BUILDINGKIND_PUBLICOFFICE:
	case BUILDINGKIND_MILITARYACADEMY:	// OnCityNPCButtonDown에도 같은 코드 존재
		{
			SendLeaveEnterBuilding(m_nCurrentEnterBuildingIndex, pBuilding->buildingInfo.BuildingIndex);
			m_pMission->Reset();
			g_pQuestData->GetQuestList(m_pCurrentBuildingNPC->buildingInfo.NPCIndex);
//			if(m_pCurrentBuildingNPC)
//			{	// 해결은 어디다가 넣을 것인가?
//				g_pQuestData->MakeCompleteProgressMission(m_pCurrentBuildingNPC->buildingInfo.NPCIndex);
//				m_pMission->Reset();
//				g_pQuestData->GetQuestList(m_pCurrentBuildingNPC->buildingInfo.NPCIndex);
//			}
			// 미션의 리셋은 ENTERBUILDING에서 수행한다.

			m_nCurrentBuildingNPCState = BUILDINGNPC_RENDER_MISSION;
		}
		break;
	case BUILDINGKIND_AUCTION:
		{
			SendLeaveEnterBuilding(m_nCurrentEnterBuildingIndex, pBuilding->buildingInfo.BuildingIndex);
			if(it == m_mapCityShop.end())
			{
				BUILDINGNPC* pInfo = g_pDatabase->GetServerBuildingNPCInfo(pBuilding->buildingInfo.BuildingIndex);				
				CINFCityAuction* pCityAuction = new CINFCityAuction(g_pGameMain, pInfo);
				pCityAuction->SetGameData( g_pGameMain->m_pGameData );
				pCityAuction->InitDeviceObjects();
				pCityAuction->RestoreDeviceObjects();
				m_mapCityShop[pBuilding->buildingInfo.BuildingKind] = pCityAuction;
			}
			m_nCurrentBuildingNPCState = BUILDINGNPC_RENDER_SHOP;
		}
		break;
	case BUILDINGKIND_CITY_OCCUPY_INFO:
		{
			SendLeaveEnterBuilding(m_nCurrentEnterBuildingIndex, pBuilding->buildingInfo.BuildingIndex);
			if(it == m_mapCityShop.end())
			{
				BUILDINGNPC* pInfo = g_pDatabase->GetServerBuildingNPCInfo(pBuilding->buildingInfo.BuildingIndex);				
				CINFCityOccupy* pCityOccupy = new CINFCityOccupy(g_pGameMain, pInfo);
				pCityOccupy->SetGameData( g_pGameMain->m_pGameData );
				pCityOccupy->InitDeviceObjects();
				pCityOccupy->RestoreDeviceObjects();
				m_mapCityShop[pBuilding->buildingInfo.BuildingKind] = pCityOccupy;
			}
			g_pFieldWinSocket->SendMsg( T_FC_CITYWAR_GET_OCCUPYINFO, NULL, 0 );
			m_nCurrentBuildingNPCState = BUILDINGNPC_RENDER_OCCUPY;
		}
		break;
	case BUILDINGKIND_RACETRACK:
		{
		}
		break;
	case BUILDINGKIND_STORE:
		{
			SendLeaveEnterBuilding(m_nCurrentEnterBuildingIndex, pBuilding->buildingInfo.BuildingIndex);
			if(it == m_mapCityShop.end())
			{
				BUILDINGNPC* pInfo = g_pDatabase->GetServerBuildingNPCInfo(pBuilding->buildingInfo.BuildingIndex);				
				CINFCityStore* pCityStore = new CINFCityStore(g_pGameMain, pInfo);
				pCityStore->SetGameData( g_pGameMain->m_pGameData );
				pCityStore->InitDeviceObjects();
				pCityStore->RestoreDeviceObjects();
				m_mapCityShop[pBuilding->buildingInfo.BuildingKind] = pCityStore;
			}
			m_nCurrentBuildingNPCState = BUILDINGNPC_RENDER_SHOP;
		}
		break;
	case BUILDINGKIND_LABORATORY:
		{
			SendLeaveEnterBuilding(m_nCurrentEnterBuildingIndex, pBuilding->buildingInfo.BuildingIndex);
			if(it == m_mapCityShop.end())
			{
				BUILDINGNPC* pInfo = g_pDatabase->GetServerBuildingNPCInfo(pBuilding->buildingInfo.BuildingIndex);				
				CINFCityLab* pCityLab = new CINFCityLab(g_pGameMain, pInfo);
				pCityLab->SetGameData( g_pGameMain->m_pGameData );
				pCityLab->InitDeviceObjects();
				pCityLab->RestoreDeviceObjects();
				m_mapCityShop[pBuilding->buildingInfo.BuildingKind] = pCityLab;
			}
			
			m_nCurrentBuildingNPCState = BUILDINGNPC_RENDER_LABORATORY;
		}
		break;
	case BUILDINGKIND_FACTORY:
		{
			SendLeaveEnterBuilding(m_nCurrentEnterBuildingIndex, pBuilding->buildingInfo.BuildingIndex);
			if(it == m_mapCityShop.end())
			{
				BUILDINGNPC* pInfo = g_pDatabase->GetServerBuildingNPCInfo(pBuilding->buildingInfo.BuildingIndex);				
				CINFCityLab* pCityLab = new CINFCityLab(g_pGameMain, pInfo);
				pCityLab->SetGameData( g_pGameMain->m_pGameData );
				pCityLab->InitDeviceObjects();
				pCityLab->RestoreDeviceObjects();
				m_mapCityShop[pBuilding->buildingInfo.BuildingKind] = pCityLab;
			}
			
			m_nCurrentBuildingNPCState = BUILDINGNPC_RENDER_FACTORY;
		}
		break;
	case BUILDINGKIND_SKILL_SHOP:
		{
			SendLeaveEnterBuilding(m_nCurrentEnterBuildingIndex, pBuilding->buildingInfo.BuildingIndex);
			if(it == m_mapCityShop.end())
			{
				BUILDINGNPC* pInfo = g_pDatabase->GetServerBuildingNPCInfo(pBuilding->buildingInfo.BuildingIndex);				
				CINFCitySkillShop* pCitySkillShop = new CINFCitySkillShop(g_pGameMain, pInfo);
				pCitySkillShop->SetGameData( g_pGameMain->m_pGameData );
				pCitySkillShop->InitDeviceObjects();
				pCitySkillShop->RestoreDeviceObjects();
				m_mapCityShop[pBuilding->buildingInfo.BuildingKind] = pCitySkillShop;
			}
			m_nCurrentBuildingNPCState = BUILDINGNPC_RENDER_SHOP;
		}
		break;
	// 2008-11-04 by dgwoo LuckyMachine기획 추가.
    case BUILDINGKIND_LUCKY:
		{
			SendLeaveEnterBuilding(m_nCurrentEnterBuildingIndex, pBuilding->buildingInfo.BuildingIndex);
			if(it == m_mapCityShop.end())
			{
				BUILDINGNPC* pInfo = g_pDatabase->GetServerBuildingNPCInfo(pBuilding->buildingInfo.BuildingIndex);				
				CINFLuckyMachine* pLuckyMachine = new CINFLuckyMachine(g_pGameMain, pInfo);
				pLuckyMachine->SetGameData( g_pGameMain->m_pGameData );
				pLuckyMachine->InitDeviceObjects();
				pLuckyMachine->RestoreDeviceObjects();
				m_mapCityShop[pBuilding->buildingInfo.BuildingKind] = pLuckyMachine;
			}
			m_nCurrentBuildingNPCState = BUILDINGNPC_RENDER_LUCKYMACHINE;
			CMapCityShopIterator it2 = m_mapCityShop.find(BUILDINGKIND_LUCKY);
			((CINFLuckyMachine*)it2->second)->ResetData();

			// 2009-03-04 by bhsohn 럭키 머신 수정안
			((CINFLuckyMachine*)it2->second)->EnterMachineShop(pBuilding->buildingInfo.BuildingIndex);
			// end 2009-03-04 by bhsohn 럭키 머신 수정안

		}
		break;
	// 2010. 04. 21 by jskim 신규 럭키 머신 구현
	case BUILDINGKIND_LUCKY_OPTION_MACHINE:
		{
			SendLeaveEnterBuilding(m_nCurrentEnterBuildingIndex, pBuilding->buildingInfo.BuildingIndex);
			if(it == m_mapCityShop.end())
			{
				BUILDINGNPC* pInfo = g_pDatabase->GetServerBuildingNPCInfo(pBuilding->buildingInfo.BuildingIndex);				
				CINFOptionMachine* pOptionMachine = new CINFOptionMachine(g_pGameMain, pInfo);
				pOptionMachine->SetGameData( g_pGameMain->m_pGameData );
				pOptionMachine->InitDeviceObjects();
				pOptionMachine->RestoreDeviceObjects();
				m_mapCityShop[pBuilding->buildingInfo.BuildingKind] = pOptionMachine;
			}
			m_nCurrentBuildingNPCState = BUILDINGNPC_RENDER_OPTIONMACHINE;
			CMapCityShopIterator it2 = m_mapCityShop.find(BUILDINGKIND_LUCKY_OPTION_MACHINE);
			((CINFOptionMachine*)it2->second)->ResetInfo();			
		}
		break;
	//end 2010. 04. 21 by jskim 신규 럭키 머신 구현
	// 2010-08-31 by shcho&&jskim, 용해 시스템 구현
	case BUILDINGKIND_DISSOLUTION:
		{
			SendLeaveEnterBuilding(m_nCurrentEnterBuildingIndex, pBuilding->buildingInfo.BuildingIndex);
			if(it == m_mapCityShop.end())
			{
				BUILDINGNPC* pInfo = g_pDatabase->GetServerBuildingNPCInfo(pBuilding->buildingInfo.BuildingIndex);				
				CINFDissolution* pDissolution = new CINFDissolution(g_pGameMain, pInfo);
				pDissolution->SetGameData( g_pGameMain->m_pGameData );
				pDissolution->InitDeviceObjects();
				pDissolution->RestoreDeviceObjects();
				m_mapCityShop[pBuilding->buildingInfo.BuildingKind] = pDissolution;
			}
			m_nCurrentBuildingNPCState = BUILDINGKIND_RENDER_DISSOLUTION;
			CMapCityShopIterator it2 = m_mapCityShop.find(BUILDINGKIND_DISSOLUTION);
			((CINFDissolution*)it2->second)->ResetInfo();	
		}
		break;
	// end 2010-08-31 by shcho&&jskim, 용해 시스템 구현
	case BUILDINGKIND_CASH_SHOP:
		{
			SendLeaveEnterBuilding(m_nCurrentEnterBuildingIndex, pBuilding->buildingInfo.BuildingIndex);
			if(it == m_mapCityShop.end())
			{
				BUILDINGNPC* pInfo = g_pDatabase->GetServerBuildingNPCInfo(pBuilding->buildingInfo.BuildingIndex);

				// 2012-11-23 by jhjang 게임포지 웹 상점 통합 작업
				#ifdef C_WEB_CASHSHOP
					CINFCityCashShop* pCityShop = new CINFCityWebCashShop(g_pGameMain, pInfo);
				#else
					CINFCityCashShop* pCityShop = new CINFCityCashShop(g_pGameMain, pInfo);
				#endif
				// end 2012-11-23 by jhjang 게임포지 웹 상점 통합 작업

				pCityShop->SetGameData( g_pGameMain->m_pGameData );
				pCityShop->InitDeviceObjects();
				pCityShop->RestoreDeviceObjects();
				m_mapCityShop[pBuilding->buildingInfo.BuildingKind] = pCityShop;
			}
			CMapCityShopIterator it2 = m_mapCityShop.find(BUILDINGKIND_CASH_SHOP);
			((CINFCityCashShop*)it2->second)->m_bMoveShop			= FALSE;
			((CINFCityCashShop*)it2->second)->m_nCurrentSelectTab	= 0;
			((CINFCityCashShop*)it2->second)->m_nShopMovePosX		= 0;
			((CINFCityCashShop*)it2->second)->m_nShopMovePosY		= 0;
			
			g_pFieldWinSocket->SendMsg( T_FC_CHARACTER_GET_CASH_MONEY_COUNT, NULL, 0 );
			m_nCurrentBuildingNPCState = BUILDINGNPC_RENDER_CASH_SHOP;
		}
		break;
	case BUILDINGKIND_REFINERY:
		{
			SendLeaveEnterBuilding(m_nCurrentEnterBuildingIndex, pBuilding->buildingInfo.BuildingIndex);
			if(it == m_mapCityShop.end())
			{
				BUILDINGNPC* pInfo = g_pDatabase->GetServerBuildingNPCInfo(pBuilding->buildingInfo.BuildingIndex);
				CINFCitySupply* pCitySupply = new CINFCitySupply(g_pGameMain, pInfo);				
				pCitySupply->SetGameData( g_pGameMain->m_pGameData );
				pCitySupply->InitDeviceObjects();
				pCitySupply->RestoreDeviceObjects();
				m_mapCityShop[pBuilding->buildingInfo.BuildingKind] = pCitySupply;
			}

			CMapCityShopIterator it2 = m_mapCityShop.find(BUILDINGKIND_REFINERY);
			((CINFCitySupply*)it2->second)->InitItemCheck();
			// 2007-05-23 by bhsohn 아레나 끝나고 상점 떳을떄의 문제 처리
			//((CINFCitySupply*)it2->second)->InitData();
			((CINFCitySupply*)it2->second)->InitData(TRUE);
			m_nCurrentBuildingNPCState = BUILDINGNPC_RENDER_SHOP;
		}
		break;
	case BUILDINGKIND_ARENA:
		{
			SendLeaveEnterBuilding(m_nCurrentEnterBuildingIndex, pBuilding->buildingInfo.BuildingIndex);
			CINFCityArena* pCityArena = NULL;
			if(it == m_mapCityShop.end())
			{
				BUILDINGNPC* pInfo = g_pDatabase->GetServerBuildingNPCInfo(pBuilding->buildingInfo.BuildingIndex);
				pCityArena = new CINFCityArena(g_pGameMain, pInfo,g_pGameMain->m_pGameData);				
				//pCityArena->SetGameData( g_pGameMain->m_pGameData );
				pCityArena->InitDeviceObjects();
				pCityArena->RestoreDeviceObjects();
				m_mapCityShop[pBuilding->buildingInfo.BuildingKind] = pCityArena;
			}
			// 처음 방 목록을 다시 받을때.
//			m_mapCityShop[pBuilding->buildingInfo.BuildingKind]->ReFresh();
			pCityArena = (CINFCityArena*)FindBuildingShop(BUILDINGKIND_ARENA);
			// 2007-11-20 by bhsohn 맵로딩 하는 방식 변경
			//pCityArena->ReFresh();
			// 아레나 상점에 연결 
			OnClickArenaShop();

			CMapCityShopIterator it = m_mapCityShop.find(BUILDINGKIND_ARENA);
			((CINFCityArena*)it->second)->Init();
			m_nCurrentBuildingNPCState = BUILDINGNPC_RENDER_ARENA;
		}
		break;
	case BUILDINGKIND_WARPOINT_SHOP:
		{
			SendLeaveEnterBuilding(m_nCurrentEnterBuildingIndex, pBuilding->buildingInfo.BuildingIndex);
			CINFCityWarPointShop* pCityWarPoint = NULL;
			if(it == m_mapCityShop.end())
			{
				BUILDINGNPC* pInfo = g_pDatabase->GetServerBuildingNPCInfo(pBuilding->buildingInfo.BuildingIndex);
				pCityWarPoint = new CINFCityWarPointShop(g_pGameMain, pInfo,g_pGameMain->m_pGameData);				
				pCityWarPoint->InitDeviceObjects();
				pCityWarPoint->RestoreDeviceObjects();
				m_mapCityShop[pBuilding->buildingInfo.BuildingKind] = pCityWarPoint;
			}
			m_nCurrentBuildingNPCState = BUILDINGNPC_RENDER_WARPOINT;
		}
		break;		
		// 2007-09-05 by bhsohn 전진 기지전
	case BUILDINGKIND_CITYLEADER_LEADER:
		{
			SendLeaveEnterBuilding(m_nCurrentEnterBuildingIndex, pBuilding->buildingInfo.BuildingIndex);
			if(it == m_mapCityShop.end())
			{
				BUILDINGNPC* pInfo = g_pDatabase->GetServerBuildingNPCInfo(pBuilding->buildingInfo.BuildingIndex);				
				CINFCityLeader* pCityOutPost = new CINFCityLeader(g_pGameMain, pInfo);
				pCityOutPost->SetGameData( g_pGameMain->m_pGameData );
				pCityOutPost->InitDeviceObjects();
				pCityOutPost->RestoreDeviceObjects();
				m_mapCityShop[pBuilding->buildingInfo.BuildingKind] = pCityOutPost;
			}
			else
			{
				// 객체가 있으면 공지 사항 버튼 클릭으로 인식해서 띠움				
				((CINFCityLeader*)it->second)->ReFresh();
			}
			m_nCurrentBuildingNPCState = BUILDINGNPC_RENDER_INFLUENCE;
		}
		break;
	case BUILDINGKIND_CITYLEADER_OUTPOST:
		{
			SendLeaveEnterBuilding(m_nCurrentEnterBuildingIndex, pBuilding->buildingInfo.BuildingIndex);
			if(it == m_mapCityShop.end())
			{
				BUILDINGNPC* pInfo = g_pDatabase->GetServerBuildingNPCInfo(pBuilding->buildingInfo.BuildingIndex);				
				CINFCityOutPost* pCityOutPost = new CINFCityOutPost(g_pGameMain, pInfo);
				pCityOutPost->SetGameData( g_pGameMain->m_pGameData );
				pCityOutPost->InitDeviceObjects();
				pCityOutPost->RestoreDeviceObjects();
				m_mapCityShop[pBuilding->buildingInfo.BuildingKind] = pCityOutPost;
			}
			else
			{
				// 객체가 있으면 공지 사항 버튼 클릭으로 인식해서 띠움				
				((CINFCityOutPost*)it->second)->ReFresh();
			}
			m_nCurrentBuildingNPCState = BUILDINGNPC_RENDER_INFLUENCE;
		}
		break;
		// end 2007-09-05 by bhsohn 전진 기지전

		// 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템
		// 인피니티 입장 상점
	case	BUILDINGKIND_INFINITY:
		{
			SendLeaveEnterBuilding(m_nCurrentEnterBuildingIndex, pBuilding->buildingInfo.BuildingIndex);
			CINFCityInfinityField* pCityInfinityField = NULL;
			if(it == m_mapCityShop.end())
			{
				BUILDINGNPC* pInfo = g_pDatabase->GetServerBuildingNPCInfo(pBuilding->buildingInfo.BuildingIndex);
				pCityInfinityField = new CINFCityInfinityField();
				pCityInfinityField->SetGameData( g_pGameMain->m_pGameData);
				pCityInfinityField->InitDeviceObjects();
				pCityInfinityField->RestoreDeviceObjects();
				m_mapCityShop[pBuilding->buildingInfo.BuildingKind] = pCityInfinityField;
			}

			OnClickInfinityFieldShop();

			m_nCurrentBuildingNPCState = BUILDINGNPC_RENDER_INFINITYFIELD;
		}
		break;

		// 인피니티 구매 상점
	case	BUILDINGKIND_INFINITY_SHOP:
		{
			SendLeaveEnterBuilding(m_nCurrentEnterBuildingIndex, pBuilding->buildingInfo.BuildingIndex);

			CINFCityInfinityShop*	pCityInfinityShop = NULL;
			if(it == m_mapCityShop.end())
			{
				BUILDINGNPC* pInfo = g_pDatabase->GetServerBuildingNPCInfo(pBuilding->buildingInfo.BuildingIndex);
				pCityInfinityShop = new CINFCityInfinityShop( g_pGameMain );
				pCityInfinityShop->SetGameData( g_pGameMain->m_pGameData);
				pCityInfinityShop->InitDeviceObjects();
				pCityInfinityShop->RestoreDeviceObjects();
				m_mapCityShop[pBuilding->buildingInfo.BuildingKind] = pCityInfinityShop;
			}
			
			m_nCurrentBuildingNPCState = BUILDINGNPC_RENDER_INFINITYSHOP;
		}
		break;
		// end 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템
		// 2013-05-07 by bhsohn 세력포인트 개선안 시스템
	case BUILDINGKIND_INFLBUFF_SHOP:
		{
			SendLeaveEnterBuilding(m_nCurrentEnterBuildingIndex, pBuilding->buildingInfo.BuildingIndex);

#ifdef C_UPGRADE_INFLUENCE_POINT_BHSOHN			
			CINFCityInflBuff*	pCityInfinityShop = NULL;
			if(it == m_mapCityShop.end())
			{
				BUILDINGNPC* pInfo = g_pDatabase->GetServerBuildingNPCInfo(pBuilding->buildingInfo.BuildingIndex);
				pCityInfinityShop = new CINFCityInflBuff( g_pGameMain );
				pCityInfinityShop->SetGameData( g_pGameMain->m_pGameData);
				pCityInfinityShop->InitDeviceObjects();
				pCityInfinityShop->RestoreDeviceObjects();
				m_mapCityShop[pBuilding->buildingInfo.BuildingKind] = pCityInfinityShop;
			}
#endif
			
			m_nCurrentBuildingNPCState = BUILDINGNPC_RENDER_INFLBUFFSHOP;
		}
		break;
		// END 2013-05-07 by bhsohn 세력포인트 개선안 시스템

	}
}

void CINFCityBase::ResetCityMission()
{
	ASSERT_ASSERT(m_pMission);
	m_pMission->Reset();
}


void CINFCityBase::AddCityMission(CQuest* pQuest)
{
	ASSERT_ASSERT(m_pMission);
	m_pMission->AddMission( pQuest );
}

CINFCityWarp* CINFCityBase::GetCityWarp()
{
	CMapCityShopIterator it = m_mapCityShop.find(BUILDINGKIND_CITYWARP);
	if(it != m_mapCityShop.end())
	{
		return (CINFCityWarp*)it->second;
	}
	return NULL;

}

void CINFCityBase::SetAllCityButtonState(int nState)
{
	
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			CINFCityBase::RecvEventEnterBuilding(MSG_FC_EVENT_ENTER_BUILDING* pMsg)
/// \brief		필드 상점 들어감
/// \author		dhkwon
/// \date		2004-08-03 ~ 2004-08-03
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFCityBase::RecvEventEnterBuilding(MSG_FC_EVENT_ENTER_BUILDING* pMsg)
{	
	// 2004-11-17 by ydkim 필드상점으로 들어갔을때 좌우 창을 닫는다 
//	g_pGameMain->RightWindowShow(FALSE,g_pGameMain->m_nRightWindowInfo);
//	g_pGameMain->LeftWindowShow(FALSE,g_pGameMain->m_nLeftWindowInfo);
	// 2005-11-29 by ispark, 모든 창 닫는다.
	// 2007-07-20 by bhsohn 브리핑룸 수정
	//g_pGameMain->InitShowWindow();

	m_nCurrentEnterBuildingIndex = pMsg->BuildingNPCInfo.BuildingIndex;
	GUI_BUILDINGNPC* pBuilding = FindBuildingNPC(pMsg->BuildingNPCInfo.BuildingIndex);	
	if(pBuilding == NULL || strlen(pBuilding->buildingInfo.NPCName)<=0)
	{
		pBuilding = new GUI_BUILDINGNPC;
		memset(pBuilding, 0x00, sizeof(GUI_BUILDINGNPC));
		memcpy(&pBuilding->buildingInfo, &pMsg->BuildingNPCInfo, sizeof(BUILDINGNPC));
		m_vecButtonGroup.push_back( pBuilding );
		m_pCurrentBuildingNPC = pBuilding;
	}
	else
	{
		m_pCurrentBuildingNPC = pBuilding;
	}
	// 2007-07-20 by bhsohn 브리핑룸 수정	
	if(OnlyBuildShopEvent(pMsg->BuildingNPCInfo.BuildingKind))
	{
		g_pD3dApp->m_bRequestEnable = TRUE;			// 메세지 응답 완료
		return;
	}	
	g_pD3dApp->ChangeGameState(_SHOP);
	g_pGameMain->InitShowWindow();
	// end 2007-07-20 by bhsohn 브리핑룸 수정

	m_nCurrentBuildingNPCState = BUILDINGNPC_RENDER_TALKBOX;
	SetNPCTalkInfo( m_pCurrentBuildingNPC->buildingInfo.GreetingTalk);
	m_pMission->Reset();
	m_pNPCScroll->Reset();
//	g_pQuestData->GetQuestList(m_pCurrentBuildingNPC->buildingInfo.NPCIndex);	// 2005-12-26 by ispark
	if(m_pNPCImage)
	{
		m_pNPCImage->InvalidateDeviceObjects();
		m_pNPCImage->DeleteDeviceObjects();
	}
	m_pNPCImage = LoadNPCImage(m_pCurrentBuildingNPC->buildingInfo.NPCIndex);
	m_fCurrentShowTimeTime = 0;
	MSG_FC_EVENT_ENTER_BUILDING_OK sMsg;
	sMsg.BuildingIndex = m_pCurrentBuildingNPC->buildingInfo.BuildingIndex;
	sMsg.SendShopItemList = FALSE;
	OnCityNPCMainMenu(m_pCurrentBuildingNPC);
	g_pD3dApp->m_bRequestEnable = TRUE;			// 2006-06-21 by ispark, 메세지 응답 완료
	CMapCityShopIterator it2 = m_mapCityShop.find(m_pCurrentBuildingNPC->buildingInfo.BuildingKind);
	
	if(it2 != m_mapCityShop.end()) // 2005-11-10 by ispark 죽지 않게 수정
	{
		if(IS_CASH_SHOP_TYPE(m_pCurrentBuildingNPC->buildingInfo.BuildingKind))
		{
			if(it2 != m_mapCityShop.end())
			{
				if(((CINFCityCashShop*)it2->second)->IsInputItemDone() == FALSE)
				{
					((CINFCityCashShop*)it2->second)->InitData();
					sMsg.SendShopItemList = TRUE;
				}
			}
		}
		else if(IS_ITEM_SHOP_TYPE(m_pCurrentBuildingNPC->buildingInfo.BuildingKind))
		{
			if(it2 != m_mapCityShop.end())
			{
				if(IS_TUNING_COLOR_SHOP(m_pCurrentBuildingNPC->buildingInfo.BuildingKind))
				{
					((CINFCityShop*)it2->second)->InputColorShopItem();
					((CINFCityShop*)it2->second)->m_pBuildingInfo = g_pDatabase->GetServerBuildingNPCInfo(pBuilding->buildingInfo.BuildingIndex);
				}
				else if(((CINFCityShop*)it2->second)->IsInputItemDone() == FALSE)
				{
					((CINFCityShop*)it2->second)->InitData();
					sMsg.SendShopItemList = TRUE;
				    ((CINFCityShop*)it2->second)->m_pBuildingInfo = g_pDatabase->GetServerBuildingNPCInfo(pBuilding->buildingInfo.BuildingIndex);
				}
			}
		}
		else if(IS_SKILL_SHOP_TYPE(m_pCurrentBuildingNPC->buildingInfo.BuildingKind))
		{
			if(it2 != m_mapCityShop.end())
			{
				if(((CINFCitySkillShop*)it2->second)->IsInputItemDone() == FALSE)
				{
					sMsg.SendShopItemList = TRUE;
				}
			}
		}
		else if(IS_WARPOINT_SHOP_TYPE(m_pCurrentBuildingNPC->buildingInfo.BuildingKind))
		{
			if(it2 != m_mapCityShop.end())
			{
				if(((CINFCityWarPointShop*)it2->second)->IsInputItemDone() == FALSE)
				{
					((CINFCityWarPointShop*)it2->second)->InitData();
					sMsg.SendShopItemList = TRUE;
				}
			}
		}
		// 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템
		else if( m_pCurrentBuildingNPC->buildingInfo.BuildingKind == BUILDINGKIND_INFINITY_SHOP )
		{
			if( it2 != m_mapCityShop.end() )
			{
				if(((CINFCityInfinityShop*)it2->second)->GetItemInputDone() == FALSE)
				{
					((CINFCityInfinityShop*)it2->second)->InitData();
					sMsg.SendShopItemList = TRUE;
				}
			}
		}
		// end 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템


		// 2006-02-08 by ispark, 도시점령전 세금에서 세력 세금으로 변경
//		(it2->second)->SetTex(pMsg->fCityWarTexRate);
		(it2->second)->SetTex(pMsg->fInflDistributionTexPercent);
		g_pFieldWinSocket->SendMsg(T_FC_EVENT_ENTER_BUILDING_OK, (char*)&sMsg,sizeof(sMsg));
		
		g_pD3dApp->m_fRequestEnableTime = REQUEST_ENABLE_INIT_TIME;

		//////////////////////////////////////////////////////////////////////////
		// 2005-11-11 by ispark
		// 상점 OK를 보낸후 서버에게 요청해야 할 사항

		if(IS_STORE_SHOP_TYPE(m_pCurrentBuildingNPC->buildingInfo.BuildingKind))
		{
			((CINFCityStore*)it2->second)->InitData();

			// 2005-03-07 by jschoi
			g_pD3dApp->m_bRequestEnable = FALSE;

			MSG_FC_STORE_GET_ITEM sMsg;
			sMsg.AccountUniqueNumber = g_pShuttleChild->m_myShuttleInfo.AccountUniqueNumber;
			sMsg.bSendToClient = TRUE;
			// 2006-09-14 by dgwoo 현재 캐릭터의 넘버를 서버에 넘겨준다.
			sMsg.PossessCharacter = g_pShuttleChild->m_myShuttleInfo.CharacterUniqueNumber;
			sMsg.ItemStorage0 = ITEM_IN_STORE;
			g_pFieldWinSocket->SendMsg( T_FC_STORE_GET_ITEM, (char*)&sMsg, sizeof(sMsg) );
		}			
		// 2008-08-22 by bhsohn EP3 인벤토리 처리

		// 2009-03-04 by bhsohn 럭키 머신 수정안
		//g_pGameMain->ShowShopInven(m_pCurrentBuildingNPC->buildingInfo.BuildingKind);
		BOOL bShowInven =FALSE;
		if(BUILDINGKIND_LUCKY == m_pCurrentBuildingNPC->buildingInfo.BuildingKind)
		{
			CMapCityShopIterator itLucky = m_mapCityShop.find(BUILDINGKIND_LUCKY);
			if(itLucky != m_mapCityShop.end()) 
			{
				if(((CINFLuckyMachine*)itLucky->second)->IsInfluenceOneShopMode())
				{
					bShowInven = TRUE;
				}
			}
		}
		// 2010. 04. 21 by jskim 신규 럭키 머신 구현
		if(BUILDINGKIND_LUCKY_OPTION_MACHINE == m_pCurrentBuildingNPC->buildingInfo.BuildingKind)
		{
			CMapCityShopIterator itOptionMachine = m_mapCityShop.find(BUILDINGKIND_LUCKY_OPTION_MACHINE);
			if(itOptionMachine != m_mapCityShop.end()) 
			{
				bShowInven = TRUE;
			}
		}
		//end 2010. 04. 21 by jskim 신규 럭키 머신 구현
		// 2010-08-31 by shcho&&jskim, 용해 시스템 구현
		if(BUILDINGKIND_DISSOLUTION == m_pCurrentBuildingNPC->buildingInfo.BuildingKind)
		{
			CMapCityShopIterator itDissolution = m_mapCityShop.find(BUILDINGKIND_DISSOLUTION);
			if(itDissolution != m_mapCityShop.end()) 
			{
				bShowInven = TRUE;
			}
		}
		// end 2010-08-31 by shcho&&jskim, 용해 시스템 구현

		POINT ptInvenPos = g_pGameMain->ShowShopInven(m_pCurrentBuildingNPC->buildingInfo.BuildingKind, bShowInven); // 2013-03-18 by bhsohn 팩토리 조합 추가
		// end 2009-03-04 by bhsohn 럭키 머신 수정안		
		// end 2008-08-22 by bhsohn EP3 인벤토리 처리
#ifdef C_INGAME_MIX_ITEM
		// 2013-03-18 by bhsohn 팩토리 조합 추가
		if(BUILDINGKIND_FACTORY == m_pCurrentBuildingNPC->buildingInfo.BuildingKind)
		{
			CMapCityShopIterator itShop = m_mapCityShop.find(BUILDINGKIND_FACTORY);			
			if(itShop != m_mapCityShop.end())
			{				
				ptInvenPos.x += 395;
				((CINFCityLab*)itShop->second)->ShowItemMixWnd(TRUE, &ptInvenPos);
			}
		}		
		// END 2013-03-18 by bhsohn 팩토리 조합 추가
#endif
		
	}
	
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			CINFCityBase::RecvEventEnterBuilding(MSG_FC_EVENT_ENTER_BUILDING* pMsg)
/// \brief		필드 상점 들어감
/// \author		dhkwon
/// \date		2004-08-03 ~ 2004-08-03
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CINFCityBase::OnlyBuildShopEvent(int BuildingKind)
{
	switch(BuildingKind)
	{
	case BUILDINGKIND_BRIEFING_ROOM:
		{			
			OnCityNPCButtonDown(CITY_NPC_BUTTON_CLOSE);
			g_pGameMain->OnClickBriefingRoom();			
		}
		break;
	case BUILDINGKIND_TUTORIAL:
		{
			OnCityNPCButtonDown(CITY_NPC_BUTTON_CLOSE);
			g_pGameMain->m_pInfWindow->AddMsgBox( STRMSG_C_070716_0100, _Q_TUTORIAL_START);		//"튜토리얼 모드로 이동하시겠습니까?"
			m_nCurrentBuildingNPCState = BUILDINGNPC_RENDER_MSGBOX;			
		}
		break;
		// 2009-02-13 by bhsohn 월드 랭킹 시스템
	case BUILDINGKIND_WORLDRANKING:
		{	
			OnCityNPCButtonDown(CITY_NPC_BUTTON_CLOSE);
			g_pGameMain->OnClickWorldRankBoard();			
		}
		break;
		// end 2009-02-13 by bhsohn 월드 랭킹 시스템	
	default:
		{
			return FALSE;
		}
	}
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			CINFCityBase::RecvCityRequestEnterBuildingOk(INT nBuildingIndex, float fTexRate)
/// \brief		서버에서 ENTER_BUILDING 승락
/// \author		dhkwon
/// \date		2004-06-25 ~ 2004-06-25
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFCityBase::RecvCityRequestEnterBuildingOk(INT nBuildingIndex, float fTexRate)
{
	m_nCurrentEnterBuildingIndex = nBuildingIndex;
	GUI_BUILDINGNPC* pBuilding = FindBuildingNPC( nBuildingIndex );
	if( pBuilding != NULL )
	{
		int nKind = pBuilding->buildingInfo.BuildingKind;
		CMapCityShopIterator it2 = m_mapCityShop.find(nKind);
		if(it2 != m_mapCityShop.end())
		{
			// 2006-02-08 by ispark, 도시점령전 세금에서 세력 세금으로 변경
			(it2->second)->SetTex(fTexRate);
			if(IS_CASH_SHOP_TYPE(nKind))
			{
				if(!((CINFCityCashShop*)it2->second)->IsInputItemDone())
				{
					DBGOUT("RecvCityRequestEnterBuildingOk, Shop(%s) Request ItemList Information\n",pBuilding->buildingInfo.BuildingName);
					((CINFCityCashShop*)it2->second)->InitData();
					MSG_FC_SHOP_GET_SHOP_ITEM_LIST sMsg;
					sMsg.BuildingIndex = nBuildingIndex;
					g_pFieldWinSocket->SendMsg( T_FC_SHOP_GET_SHOP_ITEM_LIST, (char*)&sMsg, sizeof(sMsg) );
				}
				((CINFCityCashShop*)it2->second)->m_bMoveShop = FALSE;
			}
			else if(IS_ITEM_SHOP_TYPE(nKind))
			{
				if(!((CINFCityShop*)it2->second)->IsInputItemDone())
				{
					DBGOUT("RecvCityRequestEnterBuildingOk, Shop(%s) Request ItemList Information\n",pBuilding->buildingInfo.BuildingName);
					((CINFCityShop*)it2->second)->InitData();
					MSG_FC_SHOP_GET_SHOP_ITEM_LIST sMsg;
					sMsg.BuildingIndex = nBuildingIndex;
					g_pFieldWinSocket->SendMsg( T_FC_SHOP_GET_SHOP_ITEM_LIST, (char*)&sMsg, sizeof(sMsg) );
				}
			}
			else if(IS_STORE_SHOP_TYPE(nKind))
			{
				((CINFCityStore*)it2->second)->InitData();

				// 2005-03-07 by jschoi
				g_pD3dApp->m_bRequestEnable = FALSE;

				MSG_FC_STORE_GET_ITEM sMsg;
				sMsg.AccountUniqueNumber = g_pShuttleChild->m_myShuttleInfo.AccountUniqueNumber;
				sMsg.bSendToClient = TRUE;
				sMsg.PossessCharacter = g_pShuttleChild->m_myShuttleInfo.CharacterUniqueNumber;
				sMsg.ItemStorage0 = ITEM_IN_STORE;
				g_pFieldWinSocket->SendMsg( T_FC_STORE_GET_ITEM, (char*)&sMsg, sizeof(sMsg) );
			}
			else if(IS_SKILL_SHOP_TYPE(nKind))
			{
				if(!((CINFCitySkillShop*)it2->second)->IsInputItemDone())
				{
					DBGOUT("RecvCityRequestEnterBuildingOk, Skill Shop(%s) Request ItemList Information\n",pBuilding->buildingInfo.BuildingName);
					((CINFCitySkillShop*)it2->second)->InitData();
					MSG_FC_SHOP_GET_SHOP_ITEM_LIST sMsg;
					sMsg.BuildingIndex = nBuildingIndex;
					g_pFieldWinSocket->SendMsg( T_FC_SHOP_GET_SHOP_ITEM_LIST, (char*)&sMsg, sizeof(sMsg) );
				}
			}
			else if(IS_AUCTION_SHOP_TYPE(nKind))
			{
				if(!((CINFCityAuction*)it2->second)->IsInputItemDone())
				{
					DBGOUT("RecvCityRequestEnterBuildingOk, Auction Shop(%s) Request ItemList Information\n",pBuilding->buildingInfo.BuildingName);
					((CINFCityAuction*)it2->second)->InitData();
					
					((CINFCityAuction*)it2->second)->m_nCurrentTab = 0;					
					((CINFCityAuction*)it2->second)->m_vecAuctionItem[((CINFCityAuction*)it2->second)->m_nCurrentTab].clear();
					((CINFCityAuction*)it2->second)->AuctionItemInfoView(((CINFCityAuction*)it2->second)->m_nCurrentTab);
				}
			}
			else if(IS_WARPOINT_SHOP_TYPE(nKind))
			{
				if(!((CINFCityWarPointShop*)it2->second)->IsInputItemDone())
				{
					DBGOUT("RecvCityRequestEnterBuildingOk, Shop(%s) Request ItemList Information\n",pBuilding->buildingInfo.BuildingName);
					((CINFCityWarPointShop*)it2->second)->InitData();
					MSG_FC_SHOP_GET_SHOP_ITEM_LIST sMsg;
					sMsg.BuildingIndex = nBuildingIndex;
					g_pFieldWinSocket->SendMsg( T_FC_SHOP_GET_SHOP_ITEM_LIST, (char*)&sMsg, sizeof(sMsg) );
				}
			}
			else if(IS_OUTPOST_SHOP_TYPE(nKind))
			{
			}
			// 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템
			// 인피니티 구매 상점 아이템 목록 요청
			else if( nKind == BUILDINGKIND_INFINITY_SHOP )
			{
				if(!((CINFCityInfinityShop*)it2->second)->GetItemInputDone())
				{
					((CINFCityInfinityShop*)it2->second)->InitData();
					MSG_FC_SHOP_GET_SHOP_ITEM_LIST sMsg;
					sMsg.BuildingIndex = nBuildingIndex;
					g_pFieldWinSocket->SendMsg( T_FC_SHOP_GET_SHOP_ITEM_LIST, (char*)&sMsg, sizeof(sMsg) );
				}
			}
			// end 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템
		}
		else
		{
			DBGOUT("ERROR : CINFCityBase::RecvCityRequestEnterBuildingOk(%d)\n", nBuildingIndex);
		}
	}
}


void CINFCityBase::RecvLeaveBuildingOk()
{	
	m_nCurrentEnterBuildingIndex = -1;
	if(m_nNextEnterBuilding != -1)
	{
		SendLeaveEnterBuilding( -1, m_nNextEnterBuilding );
		m_nNextEnterBuilding = -1;
	}
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			CINFCityBase::SendLeaveEnterBuilding(int nLeaveBuildingIndex, int nBuildingIndex)
/// \brief		LEAVE_BUILDING 후에 ENTER_BUILDING을 보낸다.
/// \author		dhkwon
/// \date		2004-06-25 ~ 2004-06-25
/// \warning	LEAVE_BUIDING -> LEAVE_BUILDING_OK -> ENTER_BUILDING -> ENTER_BUILDING_OK
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFCityBase::SendLeaveEnterBuilding(int nLeaveBuildingIndex, int nBuildingIndex)
{
	// 상점관련 인벤토리
	// 2007-07-23 by dgwoo 인벤을 연상태에서 상점을 클릭시 인벤이 작게 보인다.
	//g_pGameMain->m_pInfWindow->m_nLeftWindowInfo = 0;
	g_pShuttleChild->SetRenderInven(FALSE);
	if(m_pCurrentBuildingNPC)
	{
		g_pInterface->SetLeaveShopKind(m_pCurrentBuildingNPC->buildingInfo.BuildingKind);
	}

	if( nLeaveBuildingIndex == nBuildingIndex )
	{
		return;
	}
	if( nLeaveBuildingIndex != -1 )
	{
		DBGOUT("SEND--------------MSG_FC_EVENT_LEAVE_BUILDING(%d)\n",nLeaveBuildingIndex);
		MSG_FC_EVENT_LEAVE_BUILDING sMsg;
		sMsg.BuildingIndex = nLeaveBuildingIndex;
		g_pFieldWinSocket->SendMsg( T_FC_EVENT_LEAVE_BUILDING, (char*)&sMsg, sizeof(sMsg) );
		m_nNextEnterBuilding = nBuildingIndex;
//		if(CASH_SHOP_BUILDING_INDEX != nBuildingIndex && g_pD3dApp->m_dwGameState != _GAME &&
//			g_pD3dApp->m_dwGameState != _CITY)		


		// 2005-02-25 by ydkim
		nBuildingIndex = -1;
//		if( g_pD3dApp->m_dwGameState != _SHOP )
//		{
//			nBuildingIndex = -1;
//		}
	}
	if( nLeaveBuildingIndex != nBuildingIndex &&
		nBuildingIndex != -1)
	{
		DBGOUT("SEND--------------MSG_FC_CITY_REQUEST_ENTER_BUILDING(%d)\n",nBuildingIndex);
		MSG_FC_CITY_REQUEST_ENTER_BUILDING sMsg;
		sMsg.BuildingIndex = nBuildingIndex;
		g_pFieldWinSocket->SendMsg( T_FC_CITY_REQUEST_ENTER_BUILDING, (char*)&sMsg, sizeof(sMsg));

		// 캐쉬 상점은 여기서 만든다
//		if(nBuildingIndex == CASH_SHOP_BUILDING_INDEX)
//		{
//			GUI_BUILDINGNPC* pBuilding = FindBuildingNPC(CASH_SHOP_BUILDING_INDEX);
//			if(pBuilding)
//			{
//				m_pCurrentBuildingNPC = pBuilding;
//				OnCityNPCMainMenu(m_pCurrentBuildingNPC);				
//			}
//		}
	}
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			CINFCityBase::SendEventRequestWarpToOutdoor()
/// \brief		도시에서 외곽으로 출격
/// \author		dhkwon
/// \date		2004-07-02 ~ 2004-07-02
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFCityBase::SendEventRequestWarpToOutdoor()
{
	GUI_BUILDINGNPC* pBuilding = GetCurrentBuildingNPC();
	if(pBuilding)
	{
		SendLeaveEnterBuilding( pBuilding->buildingInfo.BuildingIndex, -1 );
	}
	MSG_FC_EVENT_REQUEST_WARP sMsg;
	sMsg.ClientIndex = g_pShuttleChild->m_myShuttleInfo.ClientIndex;
	g_pFieldWinSocket->SendMsg(T_FC_EVENT_REQUEST_WARP, (char*)&sMsg, sizeof(sMsg));
	DBGOUT("FieldSocket : Request Warp. MSG_FC_EVENT_REQUEST_WARP\n");
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			CINFImage* CINFCityBase::LoadNPCImage(int nNPCIndex)
/// \brief		shopnpc.tex에서 NPC 로딩
/// \author		dhkwon
/// \date		2004-07-03 ~ 2004-07-03
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
CINFImageEx* CINFCityBase::LoadNPCImage(int nNPCIndex)
{
	util::del(m_pDataHeader);
	CGameData gameData;
	gameData.SetFile( ".\\Res-Tex\\shopnpc.tex", FALSE, NULL, 0, FALSE );
	char szName[32];
	wsprintf(szName, "%04d", nNPCIndex);
	m_pDataHeader = gameData.FindFromFile(szName);
	if(m_pDataHeader == NULL)
	{
		DBGOUT("NPC(%d) Can't Find NPC file(shopnpc.tex).", nNPCIndex);
		return NULL;
	}
	CINFImageEx *pImage = new CINFImageEx;
	pImage->InitDeviceObjects( m_pDataHeader );
	pImage->RestoreDeviceObjects();
	return pImage;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			CINFBase* CINFCityBase::FindBuildingShop(int nBuildingKind)
/// \brief		상점 종류를 이용해 상점 포인터를 찾는다.
/// \author		dhkwon
/// \date		2004-07-03 ~ 2004-07-03
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
CINFBase* CINFCityBase::FindBuildingShop(int nBuildingKind)
{
	CMapCityShopIterator it = m_mapCityShop.find(nBuildingKind);
	if(it != m_mapCityShop.end())
	{
		return it->second;
	}
	return NULL;
}

void CINFCityBase::SetButtonEnableByLevel(int nLevel)
{
	int index = 0;
//	CMapCityGroupIterator it = m_mapButtonGroup.begin();
	vector<GUI_BUILDINGNPC*>::iterator it = m_vecButtonGroup.begin();
//	while( it != m_mapButtonGroup.end())
	while( it != m_vecButtonGroup.end())
	{
		m_mapButtonState[index] = BUTTON_STATE_NORMAL;
		switch((*it)->buildingInfo.BuildingKind)
		{
//		case BUILDINGKIND_1ST_WEAPONSHOP:
//			{
//				if(nLevel < 3)
//				{
//					m_mapButtonState[index] = BUTTON_STATE_DISABLE;
//				}
//			}
//			break;
//		case BUILDINGKIND_2ND_WEAPONSHOP:
//			{
//				if(nLevel < 3)
//				{
//					m_mapButtonState[index] = BUTTON_STATE_DISABLE;
//				}
//			}
//			break;
//		case BUILDINGKIND_COMPONENTSHOP:
//			{
//				if(nLevel < 2)
//				{
//					m_mapButtonState[index] = BUTTON_STATE_DISABLE;
//				}
//			}
//			break;
		case BUILDINGKIND_PETSHOP:
			{
				if(g_pShuttleChild->m_myShuttleInfo.Level < 10)
				{
					m_mapButtonState[index] = BUTTON_STATE_DISABLE;
				}
			}
			break;
		case BUILDINGKIND_ANTIQUESHOP:
			{
				if(nLevel < 10)
				{
					m_mapButtonState[index] = BUTTON_STATE_DISABLE;
				}
			}
			break;
		case BUILDINGKIND_CITYWARP:
			{
				if(nLevel < 10)
				{
					m_mapButtonState[index] = BUTTON_STATE_DISABLE;
				}
			}
			break;
		case BUILDINGKIND_TUTORIAL:
			{
				if(IS_NORMAL_INFLUENCE_TYPE(g_pShuttleChild->GetMyShuttleInfo().InfluenceType))
				{

				}
			}
			break;
//		case BUILDINGKIND_PUBLICOFFICE:
//			{
//			}
//			break;
//		case BUILDINGKIND_ACTIONHOUSE:
//			{
//			}
//			break;
		case BUILDINGKIND_RACETRACK:
			{
				if(nLevel < 10)
				{
					m_mapButtonState[index] = BUTTON_STATE_DISABLE;
				}
			}
			break;
//		case BUILDINGKIND_REFINERY:
//			{
//				if(nLevel < 2)
//				{
//					m_mapButtonState[index] = BUTTON_STATE_DISABLE;
//				}
//			}
//			break;
		case BUILDINGKIND_LABORATORY:
			{
				if(nLevel < 3)
				{
					m_mapButtonState[index] = BUTTON_STATE_DISABLE;
				}
			}
			break;
		case BUILDINGKIND_FACTORY:
			{
				if(nLevel < 10)
				{
					m_mapButtonState[index] = BUTTON_STATE_DISABLE;
				}
			}
			break;
		case BUILDINGKIND_MILITARYACADEMY:
			{
				if(nLevel < 10)
				{
					m_mapButtonState[index] = BUTTON_STATE_DISABLE;
				}
			}
			break;
		case BUILDINGKIND_STORE:
			{
				if(nLevel < 8)
				{
					m_mapButtonState[index] = BUTTON_STATE_DISABLE;
				}
			}
			break;
//		case BUILDINGKIND_OUTDOOR:
//			{
//			}
//			break;
		case BUILDINGKIND_SKILL_SHOP:
			{
				if(nLevel < 5)
				{
					m_mapButtonState[index] = BUTTON_STATE_DISABLE;
				}
			}
			break;
		case BUILDINGKIND_AUCTION:
			{
				if(nLevel < 10)
				{
					m_mapButtonState[index] = BUTTON_STATE_DISABLE;
				}
			}
			break;
		case BUILDINGKIND_CITY_OCCUPY_INFO:
			{
				// 조건 추가
			}
			break;
		case BUILDINGKIND_CASH_SHOP:
			{
				// 조건 추가
			}
			break;
		case BUILDINGKIND_ARENA:
			{
				// 조건 추가.
			}
			break;
		case BUILDINGKIND_WARPOINT_SHOP:
			{
				// 조건 추가.
			}
			break;
		}
		index++;
		it++;
	}
}

void CINFCityBase::CloseAllMessageBox()
{	
	vector<CINFMessageBox*>::iterator it = g_pGameMain->m_pInfWindow->m_vecMessageBox.begin();
	while(it != g_pGameMain->m_pInfWindow->m_vecMessageBox.end())
	{
		CINFMessageBox* pMsgBox = *it;
		// 메세지 박스 지우기(교환)
		if(_Q_TRADE_REQUEST == pMsgBox->m_nMsgType)
		{
			g_pGameMain->m_pTrade->SendTradeReject();
		}

		// 2016-07-19 panoskj removed these
		//  1:1 통화 지우기
		//if(_Q_VOIP_OK == pMsgBox->m_nMsgType)
		//{
		//	MSG_IC_VOIP_ERROR sMsg;
		//	sMsg.nErrorNum = VOIPERR_1to1_REJECT;
		//	sMsg.nSend2AccountUniqueNumber = g_pD3dApp->m_VOIPState.nCalledAccountUniqueNumber;
		//	g_pIMSocket->SendMsg( T_IC_VOIP_ERROR, (char*)&sMsg, sizeof(sMsg) );
		//	// State 초기화
		//	//memset(&g_pD3dApp->m_VOIPState, 0x00, sizeof(g_pD3dApp->m_VOIPState));
		//	g_pD3dApp->m_VOIPState.nVOIPState = _VOIP_NOTLOGIN;
		//	g_pD3dApp->m_VOIPState.bCaller = FALSE;
		//}

		//  1:1 대결 지우기
		if(_Q_PK_REQUEST == pMsgBox->m_nMsgType)
		{
			MSG_FC_BATTLE_REJECT_REQUEST_P2P_PK sMsg;
			memset(&sMsg,0x00,sizeof(sMsg));
			char buffer[SIZE_MAX_PACKET];
			int nType = T_FC_BATTLE_REJECT_REQUEST_P2P_PK;
			sMsg.SourceClientIndex = pMsgBox->m_dwData;
			memcpy(buffer, &nType, SIZE_FIELD_TYPE_HEADER);
			memcpy(buffer+SIZE_FIELD_TYPE_HEADER, &sMsg, sizeof(sMsg));
			g_pD3dApp->m_pFieldWinSocket->Write(buffer, SIZE_FIELD_TYPE_HEADER + sizeof(sMsg));
			pMsgBox->m_dwData = 0;
		}
		// 게임 종료, 로그오프시		
		if(_Q_QUIT_MESSAGE == pMsgBox->m_nMsgType)
		{
			g_pGameMain->m_pSystem->m_bQuitGameMessage = FALSE;
			g_pGameMain->m_pSystem->m_fTickSecTime = 0.0f;
			g_pGameMain->m_pSystem->m_fLastTickSecTime = 0.0f;

			// 2012-12-21 by bhsohn 베트남 게임 종료 안되는 문제 해결
			g_pD3dApp->SetClientLogOff(FALSE);
			// END 2012-12-21 by bhsohn 베트남 게임 종료 안되는 문제 해결
			
			if(TRUE == g_pGameMain->m_pSystem->m_bLogOffGameMessage)
				g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_CITY_0004, COLOR_SYSTEM);//"로그오프가 취소 되었습니다."
			else					
				g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_CITY_0005, COLOR_SYSTEM);//"게임종료가 취소 되었습니다."
			
			g_pGameMain->m_pSystem->m_bLogOffGameMessage = FALSE;
		}

		// 2005-05-12 by ydkim - 죽었을때 도시 귀환
		if(	_Q_DEAD_RETURN == pMsgBox->m_nMsgType ||
			_Q_USE_SUPER_RESTORE == pMsgBox->m_nMsgType ||
			_Q_USE_NORMAL_RESTORE == pMsgBox->m_nMsgType)
		{
			g_pShuttleChild->m_bIsCameraPattern = FALSE;
			if(g_pShuttleChild->m_bAttackMode == _SIEGE)			// 시즈모드시 죽으면 일반상태로 변경 
			{
				g_pShuttleChild->m_pEngine->ChangeBodyCondition(BODYCON_SIEGE_OFF_MASK);
				g_pShuttleChild->m_bAttackMode = _AIR;
			}
			
			// 2010. 03. 25 by ckPark 사망시 5초이상 추락하면 부활메세지 추가
			//if(COMPARE_BODYCON_BIT(g_pShuttleChild->GetCurrentBodyCondition(),BODYCON_DEAD_MASK|BODYCON_EXPLOSION_MASK) )
			if( g_pShuttleChild->m_dwState == _FALLING ||
				COMPARE_BODYCON_BIT(g_pShuttleChild->GetCurrentBodyCondition(), BODYCON_DEAD_MASK|BODYCON_EXPLOSION_MASK) )
			// end 2010. 03. 25 by ckPark 사망시 5초이상 추락하면 부활메세지 추가
			{
				MSG_FC_CHARACTER_DEAD_GAMESTART sMsg;
				memset(&sMsg,0x00,sizeof(sMsg));
				char buffer[SIZE_MAX_PACKET];
				int nType = T_FC_CHARACTER_DEAD_GAMESTART;
				sMsg.ClientIndex = g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.ClientIndex;
				sMsg.CharacterUniqueNumber = g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.CharacterUniqueNumber;
				memcpy(buffer, &nType, SIZE_FIELD_TYPE_HEADER);
				memcpy(buffer+SIZE_FIELD_TYPE_HEADER, &sMsg, sizeof(sMsg));
				g_pD3dApp->m_pFieldWinSocket->Write(buffer, SIZE_FIELD_TYPE_HEADER + sizeof(sMsg));
			}
		}		
		// 2006-06-01 by ispark
		if(_Q_CASHITEM_SHOP_OPEN == pMsgBox->m_nMsgType)
		{
			// 캐쉬 메세지 창이 열려있고 지금 닫는 중이라면 메세지를 지우지 않는다.
			if(g_pGameMain->m_pChat->m_bCashShopOpen == TRUE)
			{
				it++;
				continue;
			}
		}

		// 2006-07-24 by ispark, 세력소환 메세지는 지우지 않는다.
		if(_Q_EVENT_CALL_WARP_REQUEST == pMsgBox->m_nMsgType)
		{
			it++;
			continue;
		}
		if(_Q_ARENA_WARP == pMsgBox->m_nMsgType)
		{
			it++;
			continue;
		}
		if(_Q_ARENA_RESTART == pMsgBox->m_nMsgType)
		{
			it++;
			continue;
		}
		// 2007-11-12 by bhsohn 콜오브 히어로 창도 닫지 않는다
		if(_Q_SKILL_CALLOFHERO == pMsgBox->m_nMsgType)
		{
			it++;
			continue;
		}
		// end 2007-11-12 by bhsohn 콜오브 히어로 창도 닫지 않는다.
		// 2008-10-20 by bhsohn VOIP 버그 수정
		// VOIP참가 창은 닫지 안는다.
		if(_Q_JOIN_PARTY_VOIP == pMsgBox->m_nMsgType)
		{
			// 파티 VOIP창
			it++;
			continue;
		}
		if(_Q_JOIN_GUILD_VOIP == pMsgBox->m_nMsgType)
		{
			// 길드 VOIP창
			it++;
			continue;
		}
		// end 2008-10-20 by bhsohn VOIP 버그 수정

		it = g_pGameMain->m_pInfWindow->m_vecMessageBox.erase(it);		
		pMsgBox->InvalidateDeviceObjects();
		pMsgBox->DeleteDeviceObjects();
		util::del(pMsgBox);
//		continue;
//		
//		it++;
	}

	CMapCityShopIterator itr = m_mapCityShop.find(BUILDINGKIND_LABORATORY);
	if(itr != m_mapCityShop.end())
	{
		((CINFCityLab*)itr->second)->m_pSelectItem = NULL;
	}

	itr = m_mapCityShop.find(BUILDINGKIND_FACTORY);
	if(itr != m_mapCityShop.end())
	{
		((CINFCityLab*)itr->second)->m_pSelectItem = NULL;
	}

	g_pD3dApp->m_bChatMode = FALSE;
				
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			CINFCityBase::RecvRequestRepair()
/// \brief		보급상점 데이타를 재 갱신하기 위한것
/// \author		ispark
/// \date		2005-11-08
///////////////////////////////////////////////////////////////////////////////
void CINFCityBase::RecvRequestRepair(BOOL bSupply)
{
	if(m_pCurrentBuildingNPC &&
		m_pCurrentBuildingNPC->buildingInfo.BuildingKind == BUILDINGKIND_REFINERY) // 2005-12-10 by ispark
	{
		CMapCityShopIterator it = m_mapCityShop.find(m_pCurrentBuildingNPC->buildingInfo.BuildingKind);
		if(it != g_pInterface->m_pCityBase->m_mapCityShop.end())
		{
			((CINFCitySupply*)it->second)->InitData(bSupply);
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			CINFCityBase::CloseCurrentEnterBuilding()
/// \brief		현재 떠 있는 상점 닫기
/// \author		ispark
/// \date		2005-11-11 ~ 2005-11-11
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFCityBase::CloseCurrentEnterBuilding(BOOL bUserClose/*=TRUE*/)	// 2013-03-22 by bhsohn 트리거 상점 닫혔을때 현재 보고 있는 상점 닫고 수정
{
	DBGOUT("CITY_NPC_BUTTON_CLOSE\n");
	/////////////////////////////////////////////////////////////////////////
	// 설정 초기화 2005-04-15 by ydkim
	POINT ptIcon;
	ptIcon.x = 0;
	ptIcon.y = 0;
	g_pGameMain->SetSelectItem(NULL, ptIcon, 0);	

	g_pGameMain->SetItemInfo( 0, 0, 0, 0, 0, TRUE); // 2013-03-22 by bhsohn 트리거 상점 닫혔을때 현재 보고 있는 상점 닫고 수정

	CMapCityShopIterator it2 = m_mapCityShop.find(BUILDINGKIND_LABORATORY);
	if(it2 != m_mapCityShop.end())	
	{
		((CINFCityLab*)it2->second)->InitData();	
		// 2008-03-14 by bhsohn 조합식 개선안
		((CINFCityLab*)it2->second)->OnCloseInfWnd();
	}
	CMapCityShopIterator it3 = m_mapCityShop.find(BUILDINGKIND_FACTORY);
	if(it3 != m_mapCityShop.end()) 
	{
		((CINFCityLab*)it3->second)->InitData();
		// 2008-03-14 by bhsohn 조합식 개선안
		((CINFCityLab*)it3->second)->OnCloseInfWnd();
	}

	// 2007-09-05 by bhsohn 전진 기지전
	CMapCityShopIterator itCityLeader = m_mapCityShop.find(BUILDINGKIND_CITYLEADER_LEADER);
	if(itCityLeader != m_mapCityShop.end()) 
	{

		((CINFCityLeader*)itCityLeader->second)->OnCloseInfWnd();
	}
	CMapCityShopIterator itCityOutPost = m_mapCityShop.find(BUILDINGKIND_CITYLEADER_OUTPOST);
	if(itCityOutPost != m_mapCityShop.end()) 
	{
		((CINFCityOutPost*)itCityOutPost->second)->OnCloseInfWnd();
	}
	// end 2007-09-05 by bhsohn 전진 기지전	

	// 2010. 04. 21 by jskim 신규 럭키 머신 구현
	CMapCityShopIterator itOptionMachine = m_mapCityShop.find(BUILDINGKIND_LUCKY_OPTION_MACHINE);
	if(itOptionMachine != m_mapCityShop.end()) 
	{
		((CINFOptionMachine*)itOptionMachine->second)->OnCloseInfWnd();
	}
	//end 2010. 04. 21 by jskim 신규 럭키 머신 구현
	// 2010-08-31 by shcho&&jskim, 용해 시스템 구현
	CMapCityShopIterator itDissolution = m_mapCityShop.find(BUILDINGKIND_DISSOLUTION);
	if(itDissolution != m_mapCityShop.end()) 
	{
		((CINFDissolution*)itDissolution->second)->OnCloseInfWnd();
	}
	// end 2010-08-31 by shcho&&jskim, 용해 시스템 구현
// 2008-11-04 by dgwoo LuckyMachine기획 추가.
	CMapCityShopIterator itLucky = m_mapCityShop.find(BUILDINGKIND_LUCKY);
	if(itLucky != m_mapCityShop.end()) 
	{
		((CINFLuckyMachine*)itLucky->second)->OnCloseInfWnd();
	}


	// 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템

	// 2010. 03. 03 by ckPark 인피니티 필드 2차(입장 UI변경)

// 	CMapCityShopIterator itInfinity = m_mapCityShop.find(BUILDINGKIND_INFINITY);
// 	if(itInfinity != m_mapCityShop.end())
// 	{
// 		((CINFCityInfinityField*)itInfinity->second)->DisableChatControl();
// 		((CINFCityInfinityField*)itInfinity->second)->ClearChatList();
// 	}

	// Popup Class로 옮김
	CMapCityShopIterator itInfinityShop = m_mapCityShop.find(BUILDINGKIND_INFINITY_SHOP);
	if(itInfinityShop != m_mapCityShop.end())
		((CINFCityInfinityShop*)itInfinityShop->second)->InitData();

	// end 2010. 03. 03 by ckPark 인피니티 필드 2차(입장 UI변경)		

	// end 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템


	// 2013-03-22 by bhsohn 트리거 상점 닫혔을때 현재 보고 있는 상점 닫고 수정
	if(bUserClose)
	{
		// 2008-04-15 by dgwoo 모선전 옵션창이 활성화되어있을 경우 옵션창 먼저 닫는다.
		if(g_pGameMain->GetINFMotherShipManager()->GetMotherShipOptionAct())
		{
			g_pGameMain->GetINFMotherShipManager()->HideWindowOption();
			return;
		}
	}
	
	//
	/////////////////////////////////////////////////////////////////////////
	SendLeaveEnterBuilding( m_nCurrentEnterBuildingIndex, -1 );
	m_pCurrentBuildingNPC = NULL;

	// 2007-11-20 by bhsohn 창고 연 상태에서 편대 들어왔을시, 랜더링 크기가 잘못되어 있는 현상 처리
	// 초기화 
	g_pGameMain->m_pInfWindow->m_nLeftWindowInfo = 0;
	// end 2007-11-20 by bhsohn 창고 연 상태에서 편대 들어왔을시, 랜더링 크기가 잘못되어 있는 현상 처리
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CINFCityBase::SetCityGuildMark(UID32_t i_nGuildUID)
/// \brief		마을 인터페이스 관련 여단 마크를 갱신시킨다.
/// \author		dgwoo
/// \date		2007-12-07 ~ 2007-12-07
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CINFCityBase::SetCityGuildMark(UID32_t i_nGuildUID)
{

	CMapCityShopIterator itCity = m_mapCityShop.find(BUILDINGKIND_CITYLEADER_OUTPOST);
	if(itCity != m_mapCityShop.end())
	{
		((CINFCityOutPost*)itCity->second)->SetGuildMark(i_nGuildUID);
	}
	itCity = m_mapCityShop.find(BUILDINGKIND_CITYLEADER_LEADER);
	if(itCity != m_mapCityShop.end())
	{
		((CINFCityLeader*)itCity->second)->SetGuildMark(i_nGuildUID);
	}

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2007-11-22 by bhsohn 아레나 통합서버
/// \date		2007-12-14 ~ 2007-12-14
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFCityBase::OnClickArenaShop()
{
	g_pD3dApp->OnClickArenaShop();
}

// 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템
void	CINFCityBase::OnClickInfinityFieldShop( void )
{
	// 인피니티 복귀 메세지가 있을 경우
	// 복귀 부분 초기화
	if( g_pGameMain->m_pInfWindow->IsExistMsgBox( _Q_RETURN_TO_PROGRESS_INFINITY ) )
	{
		g_pD3dApp->m_bInfinityRestart	= FALSE;

		g_pGameMain->m_pInfWindow->DeleteMsgBox( _Q_RETURN_TO_PROGRESS_INFINITY );
	}

	g_pD3dApp->OnClickInfinityField();
}
// end 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2008-03-27 by bhsohn 미션성공창이 떠있는 상태에서는 상점이 안열리게 변경
/// \date		2008-03-27 ~ 2008-03-27
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFCityBase::SetForceCloseShop(BOOL bForceClose)
{
	m_bForceClose = bForceClose;	
}