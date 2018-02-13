// INFCharacterInfo.cpp: implementation of the CINFCharacterInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "INFCharacterInfo.h"
#include "AtumApplication.h"
#include "INFGameMain.h"
#include "QuestData.h"
#include "D3DHanFont.h"
#include "ShuttleChild.h"
#include "CharacterChild.h"				// 2005-07-21 by ispark
#include "FieldWinSocket.h"
#include "INFIcon.h"
#include "AtumDatabase.h"
#include "Interface.h"
#include "INFPilotFace.h"
#include "INFWindow.h"
#include "RangeTime.h"
#include "INFInven.h"
#include "INFImage.h"
#include "GameDataLast.h"
#include "AtumSJ.h"
#include "Skill.h"
#include "WeaponItemInfo.h"
#include "StoreData.h"
#include "SkillInfo.h"
#include "INFScrollBar.h"
#include "INFCityBase.h"
#include "Chat.h"
#include "INFSkill.h"
#include "INFCommunity.h"
#include "INFCommunityGuild.h"
#include "INFSelect.h"
#include "dxutil.h"
#include "INFCityBazaar.h"
#include "INFImageBtn.h"		// 2008-06-20 by bhsohn EP3 옵션관련 처리
#include "AtumSound.h"			// 2008-06-20 by bhsohn EP3 옵션관련 처리
#include "INFImageEx.h"	        // 2011. 10. 10 by jskim UI시스템 변경

#define INFO_ID_START_X		59
//#define INFO_HP_START_X		56
#define INFO_START_X		20
#define INFO_START_Y		69

#define INFO_INTERVAL		18

#define CHAR_UTC_START_X		(POS_RIGHT_WINDOW_X+25)
#define CHAR_UTC_START_Y		(nWindowPosY+91)
#define CHAR_UTC_ICON_SIZE		30
#define CHAR_UTC_INTERVAL_X		43
#define CHAR_UTC_INTERVAL_Y		32

#define UTC_SCROLL_BUTTON_START_X			POS_RIGHT_WINDOW_X+191
#define UTC_SCROLL_BAR_SIZE_X				11
#define UTC_SCROLL_BAR_SIZE_Y				30
//#define UTC_MAX_SCROLL_NUMBER				max(max(g_pD3dApp->m_pShuttleChild->m_mapSkillAttack.size(),g_pD3dApp->m_pShuttleChild->m_mapSkillDefense.size()),max(g_pD3dApp->m_pShuttleChild->m_mapSkillSupport.size(),g_pD3dApp->m_pShuttleChild->m_mapSkillAttribute.size()))
#define UTC_MAX_SCROLL_NUMBER				GetUtcMaxScrollNumber()
#define UTC_SCROLL_BUTTON_UP_START_Y		71
#define UTC_SCROLL_BUTTON_DOWN_START_Y		247
#define UTC_SCROLL_BAR_START_Y				85
#define UTC_SCROLL_BAR_END_Y				215
#define UTC_SCROLL_INTERVAL					(UTC_MAX_SCROLL_NUMBER==0 ? 0:((UTC_SCROLL_BAR_END_Y-UTC_SCROLL_BAR_START_Y)/UTC_MAX_SCROLL_NUMBER))

#define CHARACTER_FACE_START_X				17
#define CHARACTER_FACE_START_Y				171
#define CHARACTER_FACE_SIZE_X				66
#define CHARACTER_FACE_SIZE_Y				86
#define GUILD_START_X						87
#define GENERAL_START_X						87
#define PROPENSITY_START_X					87

#define AMMO_INTERVAL						18
#define AMMO_DEFENSE_START_X				20

#define C_QUEST_START_X						(POS_RIGHT_WINDOW_X+19)
#define C_QUEST_START_Y						70
#define C_QUEST_SIZE_X						171
#define C_QUEST_SIZE_Y						103
//#define C_QUEST_STATE_START_X				136
//#define C_QUEST_STATE_START_Y				70
#define C_QUEST_INTERVAL					17
#define C_QUEST_DESC_START_X				(POS_RIGHT_WINDOW_X+20)
#define C_QUEST_DESC_START_Y				178
#define C_QUEST_DESC_INTERVAL				15
#define C_QUEST_DESC_SIZE_X					171
#define C_QUEST_DESC_SIZE_Y					78


#define QUEST_LIST_SCROLL_START_X		(POS_RIGHT_WINDOW_X+198)
#define QUEST_LIST_SCROLL_START_Y		84
#define QUEST_LIST_SCROLL_LINE_LENGTH	76
#define QUEST_DESC_SCROLL_START_X		(POS_RIGHT_WINDOW_X+198)
#define QUEST_DESC_SCROLL_START_Y		185
#define QUEST_DESC_SCROLL_LINE_LENGTH	61

//#define CHAR_SCROLL_BAR_SIZE_X					11
//#define CHAR_SCROLL_BAR_SIZE_Y					30

//#define CHAR_QUEST_LIST_SCROLL_BAR_START_X		192
//#define CHAR_QUEST_LIST_SCROLL_BAR_START_Y		83
//#define CHAR_QUEST_LIST_SCROLL_BAR_LENGTH		(76-CHAR_SCROLL_BAR_SIZE_Y)
//#define CHAR_QUEST_LIST_SCROLL_BAR_INTERVAL(count)		(count==0 ? 0:(CHAR_QUEST_LIST_SCROLL_BAR_LENGTH/count))

//#define CHAR_QUEST_DESC_SCROLL_BAR_START_X		192
//#define CHAR_QUEST_DESC_SCROLL_BAR_START_Y		185
//#define CHAR_QUEST_DESC_SCROLL_BAR_LENGTH		(61-30)
//#define CHAR_QUEST_DESC_SCROLL_BAR_INTERVAL(count)		(count==0 ? 0:(CHAR_QUEST_DESC_SCROLL_BAR_LENGTH/count))
#define SHOP_START_X				CITY_BASE_NPC_BOX_START_X
#define SHOP_START_Y				(CITY_BASE_NPC_BOX_START_Y - SIZE_NORMAL_WINDOW_Y)
// 2008-08-22 by bhsohn EP3 인벤토리 처리
//#define SHOP_SKILL_BACK_START_X		(SHOP_START_X+26)
//#define SHOP_SKILL_BACK_START_Y		(SHOP_START_Y+34)
//#define SHOP_SKILL_START_X			(SHOP_SKILL_BACK_START_X+6)
//#define SHOP_SKILL_START_Y			(SHOP_SKILL_BACK_START_Y+29)
//#define SHOP_SKILL_INTERVAL_X		43
//#define SHOP_SKILL_INTERVAL_Y		32
//#define SHOP_SKILL_SPI_START_X		(SHOP_SKILL_BACK_START_X+26)
//#define SHOP_SKILL_SPI_START_Y		(SHOP_SKILL_BACK_START_Y+238)
//#define SHOP_SKILL_SCROLL_START_X	(SHOP_SKILL_BACK_START_X+174)
//#define SHOP_SKILL_SCROLL_START_Y	(SHOP_SKILL_BACK_START_Y+8)
//#define SHOP_SKILL_LENGTH			181
//#define SHOP_SKILL_BACK_SIZE_X		166
//#define SHOP_SKILL_BACK_SIZE_Y		181
#define SHOP_SKILL_BACK_START_X		(SHOP_START_X)
#define SHOP_SKILL_BACK_START_Y		(SHOP_START_Y)
#define SHOP_SKILL_START_X			(SHOP_SKILL_BACK_START_X+20)
#define SHOP_SKILL_START_Y			(SHOP_SKILL_BACK_START_Y+57)
#define SHOP_SKILL_INTERVAL_X		116
#define SHOP_SKILL_INTERVAL_Y		35
#define SHOP_SKILL_SPI_START_X		(SHOP_SKILL_BACK_START_X+26)
#define SHOP_SKILL_SPI_START_Y		(SHOP_SKILL_BACK_START_Y+238)
#define SHOP_SKILL_SCROLL_START_X	(SHOP_SKILL_BACK_START_X+489)
#define SHOP_SKILL_SCROLL_START_Y	(SHOP_SKILL_BACK_START_Y+58)
#define SHOP_SKILL_LENGTH			181
#define SHOP_SKILL_BACK_SIZE_X		500
#define SHOP_SKILL_BACK_SIZE_Y		181
// end 2008-08-22 by bhsohn EP3 인벤토리 처리

#define SKILL_DROP_SIZE				22

// 29/03/2006 by ispark, 언어에 따라 위치 수정
#ifdef LANGUAGE_ENGLISH
#define INFO_LEVEL_START_X			18//13
#define INFO_LEVEL_START_Y			25//26
#define INFO_ID_START_Y				24//26
#define OTHER_INFO_START_Y			184//186

#define GUILD_START_Y						69
#define GENERAL_START_Y						87
#define PROPENSITY_START_Y					105

#define AMMO_START_X				80//71
#define AMMO_START_Y				68//70

#define STATINFO_START_X0			55//50
#define STATINFO_START_X1			160//155

#define CHARACTER_AMMO_TEXT_X1		26
#define CHARACTER_AMMO_TEXT_X2		61

#define SHOWSTATINFO_Y1				186
#define SHOWSTATINFO_Y2				205
#define SHOWSTATINFO_Y3				223
#define SHOWSTATINFO_Y4				241
#endif

#ifdef LANGUAGE_VIETNAM
#define INFO_LEVEL_START_X			18//13
#define INFO_LEVEL_START_Y			25//26
#define INFO_ID_START_Y				24//26
#define OTHER_INFO_START_Y			184//186

#define GUILD_START_Y						69
#define GENERAL_START_Y						87
#define PROPENSITY_START_Y					105

#define AMMO_START_X				118//80//71
#define AMMO_START_Y				68//70

#define STATINFO_START_X0			55//50
#define STATINFO_START_X1			160//155

#define CHARACTER_AMMO_TEXT_X1		13//26
#define CHARACTER_AMMO_TEXT_X2		99//61

#define SHOWSTATINFO_Y1				186
#define SHOWSTATINFO_Y2				205
#define SHOWSTATINFO_Y3				223
#define SHOWSTATINFO_Y4				241
#endif
// end 2008-12-04 by bhsohn Japan Working

// 2007-05-15 by bhsohn 기어 스탯 관련 처리
// 오버스탯 글씨 간격
#define	OVER_STAT_CAP0_X			(13)
#define	OVER_STAT_CAP1_X			(24)
#define	OVER_STAT_CAPX				30

// 2007-06-07 by dgwoo 아레나 정보창.
#define ARENA_GAP_H							18

#define ARENA_SCORE_X						(POS_RIGHT_WINDOW_X + 194)
#define ARENA_SCORE_Y						(nWindowPosY + 68)

#define ARENA_SCORE_PERCENTAGE_X			(POS_RIGHT_WINDOW_X + 194)
#define ARENA_SCORE_PERCENTAGE_Y			(ARENA_SCORE_Y + ARENA_GAP_H)

#define ARENA_COMPULSION_END_X				(POS_RIGHT_WINDOW_X + 194)
#define ARENA_COMPULSION_END_Y				(ARENA_SCORE_PERCENTAGE_Y + ARENA_GAP_H)

// 누적 포인트
#define ARENA_COMMULATION_POINT_X			(POS_RIGHT_WINDOW_X + 194)
#define ARENA_COMMULATION_POINT_Y			(ARENA_COMPULSION_END_Y + ARENA_GAP_H + ARENA_GAP_H)

// 가용 포인트
#define ARENA_FUSIBLE_POINT_X				(POS_RIGHT_WINDOW_X + 194)
#define ARENA_FUSIBLE_POINT_Y				(ARENA_COMMULATION_POINT_Y + ARENA_GAP_H)


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CINFCharacterInfo::CINFCharacterInfo(CAtumNode* pParent)
{
	FLOG( "CINFCharacterInfo(CAtumNode* pParent)" );
	m_pAmmo = NULL;
	m_pStatImage = NULL;
	m_pInfo = NULL;
	m_pInfoLog = NULL;
	m_pId = NULL;
	m_pUtc = NULL;
	m_pUtcScr = NULL;
	m_pImgArenaTab = NULL;
//	m_pScrollBar = NULL;
	m_pQuestSelect = NULL;
	m_pSkillShopBack = NULL;

	m_pParent = pParent;
	m_nCharacterInfoType = CHARACTER_INFO;
	m_nStatType = STAT_BUTTON_UP ;

	int i;
	for(i=0 ; i<5 ; i++)
	{
		m_CharacInfoFont[i] = NULL ;
	}
	for(i=0 ; i<7 ; i++)
	{
		m_StatInfoFont[i] = NULL ;
	}
	
	// 2007-05-15 by bhsohn 기어 스탯 관련 처리
	m_StatOverInfoFont = NULL;
	m_StatInfoBoldFont = NULL;

	for(i=0 ; i<3 ; i++)
	{
		m_pStat[i] = NULL ;
	}
	m_pFontName = NULL;
	m_pFontLevel = NULL;
	m_pFontGuild = NULL;
	m_pFontGeneral = NULL;
	m_pFontPropensity = NULL;
	m_pFontAutoStat = NULL;
	m_pFontStatDec = NULL;
	for(i=0 ; i<CHARACTER_INTERFACE_FONT_NUM ; i++)
	{
		m_pFontAmmoWeapon[i] = NULL ;
	}

	m_nCurrentUTCScroll = 0;
	memset(m_pUTCDisplayInfo, 0x00, sizeof(INVEN_DISPLAY_INFO)*CHAR_UTC_TYPE_NUMBER*CHAR_UTC_SLOT_NUMBER);
	m_pSelectIcon = NULL;
	m_pSelectSkill = NULL;
	m_bUTCScrollLock = FALSE;
	m_ptMouse.x = 0;
	m_ptMouse.y = 0;

	for(i=0;i<C_QUEST_LIST_NUM;i++)
	{
		m_pFontQuestList[i] = NULL;
//		m_pFontQuestState[i] = NULL;
	}
	for(i=0;i<C_QUEST_DESC_NUM;i++)
	{
		m_pFontQuestDesc[i] = NULL;
	}
	for(i=0; i<CHAR_UTC_SLOT_NUMBER; i++)
	{
		m_pFontItemNum[i] = NULL;
	}
//	m_nCurrentQuestListScroll = 0;
//	m_nCurrentSelectQuestList = 0;
//	m_bQuestListLock = FALSE;
//	m_nCurrentQuestDescScroll = 0;
//	m_nCurrentSelectQuestDesc = 0;
//	m_bQuestDescLock = FALSE;
	m_bRestored = FALSE;
	m_bInvalidated = FALSE;
	m_nRenderMoveIconIntervalWidth  = 0;
	m_nRenderMoveIconIntervalHeight = 0;
	m_pQuestListScrollBar = NULL;
	m_pQuestDescScrollBar = NULL;
	m_pShopSkillScrollBar = NULL;
	m_nOldQuestSelectDataIndex = 0;

	m_pDeleteSkill = NULL;
	m_nMissionProCount = 0;
	m_bLbuttonDown = FALSE;	
//	m_bToolTipStat = FALSE;						// 2005-07-04 by ispark 자동 스탯 분배 삭제

	// 2007-05-15 by bhsohn 기어 스탯 관련 처리
	int nTmpCnt=0;
	for(nTmpCnt=0;nTmpCnt < MAX_STAT_SHUTTLEINFO;nTmpCnt++)
	{
		m_bStatShow[nTmpCnt] = TRUE;
		m_nShowOverStat[nTmpCnt] = 0;
	}
	// end 2007-05-15 by bhsohn 기어 스탯 관련 처리

	// 2008-06-20 by bhsohn EP3 옵션관련 처리
	m_pOpenBtn = NULL;	
	// end 2008-06-20 by bhsohn EP3 옵션관련 처리
}

CINFCharacterInfo::~CINFCharacterInfo()
{
	// 2008-06-20 by bhsohn EP3 옵션관련 처리
	util::del(m_pOpenBtn);	
	// end 2008-06-20 by bhsohn EP3 옵션관련 처리
	
	FLOG( "~CINFCharacterInfo()" );
	util::del(m_pAmmo );
	util::del(m_pStatImage ) ;
	util::del(m_pInfo);
	util::del(m_pInfoLog);
	util::del(m_pId);
	util::del(m_pUtc);
	util::del(m_pUtcScr);
	util::del(m_pSkillShopBack);
	util::del(m_pImgArenaTab);
//	util::del(m_pScrollBar);
	util::del(m_pQuestListScrollBar);
	util::del(m_pQuestDescScrollBar);
	util::del(m_pShopSkillScrollBar);
	util::del(m_pQuestSelect);

	util::del(m_pFontName);
	util::del(m_pFontLevel);
	util::del(m_pFontGuild);
	util::del(m_pFontGeneral);
	util::del(m_pFontPropensity);
	util::del(m_pFontAutoStat);
	util::del(m_pFontStatDec);
	int i;
	for(i=0 ; i<5 ; i++)
	{
		util::del(m_CharacInfoFont[i] ) ;
	}
	for(i=0 ; i<7 ; i++)
	{
		util::del(m_StatInfoFont[i] ) ;
	}
	
	// 2007-05-15 by bhsohn 기어 스탯 관련 처리
	util::del(m_StatOverInfoFont);
	util::del(m_StatInfoBoldFont);	

	for(i=0 ; i<3 ; i++)
	{
		util::del(m_pStat[i] );
	}
	for(i=0;i<C_QUEST_LIST_NUM;i++)
	{
		util::del(m_pFontQuestList[i]);
//		util::del(m_pFontQuestState[i]);
	}
	for(i=0;i<C_QUEST_DESC_NUM;i++)
	{
		util::del(m_pFontQuestDesc[i]);
	}
	for(i=0 ; i<CHARACTER_INTERFACE_FONT_NUM ; i++)
	{
		util::del(m_pFontAmmoWeapon[i]);
	}
	for(i=0; i<CHAR_UTC_SLOT_NUMBER; i++)
	{
		util::del(m_pFontItemNum[i]);
	}


}

HRESULT CINFCharacterInfo::InitDeviceObjects()
{
	FLOG( "CINFCharacterInfo::InitDeviceObjects()" );
	//인포
	DataHeader	* pDataHeader ;

	m_pAmmo = new CINFImageEx;
	pDataHeader = FindResource("ammo");
	m_pAmmo->InitDeviceObjects(pDataHeader ) ;

		m_pStatImage = new CINFImageEx;
	pDataHeader = FindResource("stat");
	m_pStatImage->InitDeviceObjects(pDataHeader ) ;

		m_pInfo = new CINFImageEx;
	pDataHeader = FindResource("info");
	m_pInfo->InitDeviceObjects(pDataHeader ) ;

		m_pInfoLog = new CINFImageEx;
	pDataHeader = FindResource("in_log");
	m_pInfoLog->InitDeviceObjects(pDataHeader ) ;

		m_pId = new CINFImageEx;
	pDataHeader = FindResource("ID");
	m_pId->InitDeviceObjects(pDataHeader ) ;

		m_pUtc = new CINFImageEx;
	pDataHeader = FindResource("utc");
	m_pUtc->InitDeviceObjects(pDataHeader ) ;

		m_pUtcScr = new CINFImageEx;
	pDataHeader = FindResource("c_scrlb");
	m_pUtcScr->InitDeviceObjects(pDataHeader ) ;

		m_pSkillShopBack = new CINFImageEx;
	pDataHeader = FindResource("shskill");
	m_pSkillShopBack->InitDeviceObjects(pDataHeader ) ;

		m_pImgArenaTab = new CINFImageEx;
	pDataHeader = FindResource("arena_t");
	m_pImgArenaTab->InitDeviceObjects(pDataHeader ) ;

		m_pQuestSelect = new CINFImageEx;
	pDataHeader = FindResource("q_select");
	m_pQuestSelect->InitDeviceObjects(pDataHeader ) ;

	int i;
	for(i=0 ; i<5 ; i++)
	{
		m_CharacInfoFont[i] = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9, D3DFONT_ZENABLE,  FALSE,256,32);
		m_CharacInfoFont[i]->InitDeviceObjects(g_pD3dDev) ;
	}
	for(i=0 ; i<7 ; i++)
	{
		m_StatInfoFont[i] = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),10, D3DFONT_ZENABLE,  FALSE,256,32);
		m_StatInfoFont[i]->InitDeviceObjects(g_pD3dDev) ;
	}

	// 2007-05-15 by bhsohn 기어 스탯 관련 처리
	m_StatOverInfoFont = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9, D3DFONT_ZENABLE,  FALSE,256,32);
	m_StatOverInfoFont->InitDeviceObjects(g_pD3dDev) ;	

	m_StatInfoBoldFont = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9, D3DFONT_ZENABLE|D3DFONT_BOLD,  FALSE,256,32);
	m_StatInfoBoldFont->InitDeviceObjects(g_pD3dDev) ;	
	

	for(i=0 ; i<3 ; i++)
	{
		m_pStat[i] = new CINFImageEx;
		char Buf[30] ;
		wsprintf(Buf, "stat0%d", i+1) ;
 		pDataHeader = FindResource(Buf);
		m_pStat[i]->InitDeviceObjects( pDataHeader );
	}
	m_pFontName = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),10, D3DFONT_ZENABLE,  FALSE,256,32);
	m_pFontName->InitDeviceObjects(g_pD3dDev) ;

	m_pFontLevel = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),10, D3DFONT_ZENABLE,  FALSE,256,32);
	m_pFontLevel->InitDeviceObjects(g_pD3dDev) ;

	m_pFontGuild = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9, D3DFONT_ZENABLE,  FALSE,256,32);
	m_pFontGuild->InitDeviceObjects(g_pD3dDev) ;
	m_pFontGeneral = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9, D3DFONT_ZENABLE,  FALSE,256,32);
	m_pFontGeneral->InitDeviceObjects(g_pD3dDev) ;
	m_pFontPropensity = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9, D3DFONT_ZENABLE,  FALSE,256,32);
	m_pFontPropensity->InitDeviceObjects(g_pD3dDev) ;
	m_pFontAutoStat = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9, D3DFONT_ZENABLE,  FALSE,256,32);
	m_pFontAutoStat->InitDeviceObjects(g_pD3dDev) ;
	m_pFontStatDec = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9, D3DFONT_ZENABLE,  FALSE,512,32);
	m_pFontStatDec->InitDeviceObjects(g_pD3dDev) ;

	for(i=0;i<C_QUEST_LIST_NUM;i++)
	{
		m_pFontQuestList[i] = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9, D3DFONT_ZENABLE,  FALSE,256,32);
		m_pFontQuestList[i]->InitDeviceObjects(g_pD3dDev) ;
//		m_pFontQuestState[i] = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9, D3DFONT_ZENABLE,  FALSE,128,32);
//		m_pFontQuestState[i]->InitDeviceObjects(g_pD3dDev) ;
	}
	for(i=0;i<C_QUEST_DESC_NUM;i++)
	{
		m_pFontQuestDesc[i] = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9, D3DFONT_ZENABLE,  FALSE,256,32);
		m_pFontQuestDesc[i]->InitDeviceObjects(g_pD3dDev) ;
	}
	for(i=0 ; i<CHARACTER_INTERFACE_FONT_NUM ; i++)
	{
		m_pFontAmmoWeapon[i] = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9, D3DFONT_ZENABLE,  FALSE,256,32);
		m_pFontAmmoWeapon[i]->InitDeviceObjects(g_pD3dDev) ;
	}
	for(i=0; i<CHAR_UTC_SLOT_NUMBER; i++)
	{
		m_pFontItemNum[i] = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9, D3DFONT_ZENABLE,  TRUE,256,32);
		m_pFontItemNum[i]->InitDeviceObjects(g_pD3dDev);
	}
	
	m_pQuestListScrollBar = new CINFScrollBar(this,
								QUEST_LIST_SCROLL_START_X-3, 
								g_pGameMain->m_nRightWindowY+QUEST_LIST_SCROLL_START_Y+1, 
								QUEST_LIST_SCROLL_LINE_LENGTH,
								C_QUEST_LIST_NUM);
	m_pQuestListScrollBar->SetGameData( m_pGameData );
	m_pQuestListScrollBar->InitDeviceObjects();
	m_pQuestDescScrollBar = new CINFScrollBar(this,
								QUEST_DESC_SCROLL_START_X, 
								g_pGameMain->m_nRightWindowY+QUEST_DESC_SCROLL_START_Y, 
								QUEST_DESC_SCROLL_LINE_LENGTH,
								C_QUEST_DESC_NUM);
	m_pQuestDescScrollBar->SetGameData( m_pGameData );
	m_pQuestDescScrollBar->InitDeviceObjects();
	m_pShopSkillScrollBar = new CINFScrollBar(this,
								SHOP_SKILL_SCROLL_START_X, 
								SHOP_SKILL_SCROLL_START_Y, 
								SHOP_SKILL_LENGTH,
								CHAR_UTC_SLOT_NUMBER);
	m_pShopSkillScrollBar->SetGameData( m_pGameData );
	m_pShopSkillScrollBar->InitDeviceObjects();
	m_pShopSkillScrollBar->SetWheelRect(SHOP_SKILL_BACK_START_X, 
		SHOP_SKILL_BACK_START_Y,
		SHOP_SKILL_BACK_START_X+SHOP_SKILL_BACK_SIZE_X,
		SHOP_SKILL_BACK_START_Y+SHOP_SKILL_BACK_SIZE_Y);

	// 2008-06-20 by bhsohn EP3 옵션관련 처리
	{
		// 닫기
		char szUpBtn[30], szDownBtn[30], szSelBtn[30], szDisBtn[30];
		wsprintf(szUpBtn, "uopbtn3");
		wsprintf(szDownBtn, "uopbtn1");
		wsprintf(szSelBtn, "uopbtn0");
		wsprintf(szDisBtn, "uopbtn2");
		if(NULL == m_pOpenBtn)
		{
			m_pOpenBtn = new CINFImageBtn;
		}
		m_pOpenBtn->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn);		
	}
	// end 2008-06-20 by bhsohn EP3 옵션관련 처리

	return S_OK ;
}



HRESULT CINFCharacterInfo::RestoreDeviceObjects()
{
	FLOG( "CINFCharacterInfo::RestoreDeviceObjects()" );
	m_pAmmo->RestoreDeviceObjects();
	m_pStatImage->RestoreDeviceObjects();

	m_pInfo->RestoreDeviceObjects();
	m_pInfoLog->RestoreDeviceObjects();
	m_pId->RestoreDeviceObjects();
	m_pUtc->RestoreDeviceObjects();
	m_pUtcScr->RestoreDeviceObjects();
	m_pSkillShopBack->RestoreDeviceObjects();
	m_pImgArenaTab->RestoreDeviceObjects();
//	m_pScrollBar->RestoreDeviceObjects();
	m_pFontName->RestoreDeviceObjects();
	m_pFontLevel->RestoreDeviceObjects();
	m_pFontGuild->RestoreDeviceObjects();
	m_pFontGeneral->RestoreDeviceObjects();
	m_pFontPropensity->RestoreDeviceObjects();
	m_pFontAutoStat->RestoreDeviceObjects();
	m_pFontStatDec->RestoreDeviceObjects();
	m_pQuestListScrollBar->RestoreDeviceObjects();
	m_pQuestDescScrollBar->RestoreDeviceObjects();
	m_pQuestSelect->RestoreDeviceObjects();
	m_pShopSkillScrollBar->RestoreDeviceObjects();
	int i;
	for(i=0 ; i<CHARACTER_INTERFACE_FONT_NUM ; i++)
	{
		m_pFontAmmoWeapon[i]->RestoreDeviceObjects();
	}

	for( i=0 ; i<5 ; i++)
	{
		m_CharacInfoFont[i]->RestoreDeviceObjects() ;
	}
	for(i=0 ; i<7 ; i++)
	{
		m_StatInfoFont[i]->RestoreDeviceObjects() ;
	}
	// 2007-05-15 by bhsohn 기어 스탯 관련 처리	
	m_StatOverInfoFont->RestoreDeviceObjects() ;
	m_StatInfoBoldFont->RestoreDeviceObjects() ;

	for(i=0 ; i<3 ; i++)
	{
		m_pStat[i]->RestoreDeviceObjects() ;
	}
	for(i=0;i<C_QUEST_LIST_NUM;i++)
	{
		m_pFontQuestList[i]->RestoreDeviceObjects() ;
//		m_pFontQuestState[i]->RestoreDeviceObjects() ;
	}
	for(i=0;i<C_QUEST_DESC_NUM;i++)
	{
		m_pFontQuestDesc[i]->RestoreDeviceObjects() ;
	}
	for(i=0; i<CHAR_UTC_SLOT_NUMBER; i++)
	{
		m_pFontItemNum[i]->RestoreDeviceObjects() ;
	}
	// 2008-06-20 by bhsohn EP3 옵션관련 처리
	if(m_pOpenBtn)
	{
		m_pOpenBtn->RestoreDeviceObjects();						
	}
	// end 2008-06-20 by bhsohn EP3 옵션관련 처리
	m_bRestored = TRUE;
	return S_OK ;
}

HRESULT CINFCharacterInfo::DeleteDeviceObjects()
{
	FLOG( "CINFCharacterInfo::DeleteDeviceObjects()" );
	m_pAmmo->DeleteDeviceObjects();
	util::del(m_pAmmo );
	m_pStatImage->DeleteDeviceObjects();
	util::del(m_pStatImage );

	m_pInfo->DeleteDeviceObjects();
	util::del(m_pInfo );
	m_pInfoLog->DeleteDeviceObjects();
	util::del(m_pInfoLog );
	m_pId->DeleteDeviceObjects();
	util::del(m_pId );
	m_pUtc->DeleteDeviceObjects();
	util::del(m_pUtc );
	m_pUtcScr->DeleteDeviceObjects();
	util::del(m_pUtcScr );
	m_pSkillShopBack->DeleteDeviceObjects();
	util::del(m_pSkillShopBack );
	m_pImgArenaTab->DeleteDeviceObjects();
	util::del(m_pImgArenaTab );
//	m_pScrollBar->DeleteDeviceObjects();
//	util::del(m_pScrollBar );
	m_pFontName->DeleteDeviceObjects();
	util::del(m_pFontName );
	m_pFontLevel->DeleteDeviceObjects();
	util::del(m_pFontLevel );
	m_pFontGuild->DeleteDeviceObjects();
	util::del(m_pFontGuild );
	m_pFontGeneral->DeleteDeviceObjects();
	util::del(m_pFontGeneral );
	m_pFontPropensity->DeleteDeviceObjects();
	util::del(m_pFontPropensity );
	m_pFontAutoStat->DeleteDeviceObjects();
	util::del(m_pFontAutoStat );
	m_pFontStatDec->DeleteDeviceObjects();
	util::del(m_pFontStatDec );
	m_pQuestListScrollBar->DeleteDeviceObjects();
	util::del(m_pQuestListScrollBar );
	m_pQuestDescScrollBar->DeleteDeviceObjects();
	util::del(m_pQuestDescScrollBar );
	m_pQuestSelect->DeleteDeviceObjects();
	util::del(m_pQuestSelect );
	m_pShopSkillScrollBar->DeleteDeviceObjects();
	util::del(m_pShopSkillScrollBar );
	int i;
	for(i=0 ; i<CHARACTER_INTERFACE_FONT_NUM ; i++)
	{
		m_pFontAmmoWeapon[i]->DeleteDeviceObjects();
		util::del(m_pFontAmmoWeapon[i] );
	}

	for( i=0 ; i<5 ; i++)
	{
		if(m_CharacInfoFont[i])
		{
			m_CharacInfoFont[i]->DeleteDeviceObjects() ;
			util::del(m_CharacInfoFont[i] );
		}
	}
	for(i=0 ; i<7 ; i++)
	{
		if(m_StatInfoFont[i])
		{
			m_StatInfoFont[i]->DeleteDeviceObjects() ;
			util::del(m_StatInfoFont[i] );
		}
	}
	// 2007-05-15 by bhsohn 기어 스탯 관련 처리	
	if(m_StatOverInfoFont)
	{
		m_StatOverInfoFont->DeleteDeviceObjects() ;
		util::del(m_StatOverInfoFont );
	}		
	if(m_StatInfoBoldFont)
	{
		m_StatInfoBoldFont->DeleteDeviceObjects() ;
		util::del(m_StatInfoBoldFont );
	}	

	for(i=0 ; i<3 ; i++)
	{
		m_pStat[i]->DeleteDeviceObjects() ;
		util::del(m_pStat[i] );
	}
	for(i=0;i<C_QUEST_LIST_NUM;i++)
	{
		m_pFontQuestList[i]->DeleteDeviceObjects() ;
		util::del(m_pFontQuestList[i] );
//		m_pFontQuestState[i]->DeleteDeviceObjects() ;
//		util::del(m_pFontQuestState[i] );
	}
	for(i=0;i<C_QUEST_DESC_NUM;i++)
	{
		m_pFontQuestDesc[i]->DeleteDeviceObjects() ;
		util::del(m_pFontQuestDesc[i] );
	}
	for(i=0; i<CHAR_UTC_SLOT_NUMBER; i++)
	{
		m_pFontItemNum[i]->DeleteDeviceObjects() ;
		util::del(m_pFontItemNum[i] );
	}
	// 2008-06-20 by bhsohn EP3 옵션관련 처리
	if(m_pOpenBtn)
	{		
		m_pOpenBtn->DeleteDeviceObjects();
		util::del(m_pOpenBtn);
	}	
	// end 2008-06-20 by bhsohn EP3 옵션관련 처리

	m_bInvalidated = FALSE;

	return S_OK ;
}


HRESULT CINFCharacterInfo::InvalidateDeviceObjects()
{
	FLOG( "CINFCharacterInfo::InvalidateDeviceObjects()" );
	m_pAmmo->InvalidateDeviceObjects();
	m_pStatImage->InvalidateDeviceObjects();
	
	m_pInfo->InvalidateDeviceObjects();
	m_pInfoLog->InvalidateDeviceObjects();
	m_pId->InvalidateDeviceObjects();
	m_pUtc->InvalidateDeviceObjects();
	m_pUtcScr->InvalidateDeviceObjects();
	m_pSkillShopBack->InvalidateDeviceObjects();
	m_pImgArenaTab->InvalidateDeviceObjects();
//	m_pScrollBar->InvalidateDeviceObjects();
	m_pQuestListScrollBar->InvalidateDeviceObjects();
	m_pQuestDescScrollBar->InvalidateDeviceObjects();
	m_pQuestSelect->InvalidateDeviceObjects();
	m_pShopSkillScrollBar->InvalidateDeviceObjects();

	int i;
	for(i=0 ; i<5 ; i++)
	{
		m_CharacInfoFont[i]->InvalidateDeviceObjects() ;
	}
	for(i=0 ; i<7 ; i++)
	{
		m_StatInfoFont[i]->InvalidateDeviceObjects() ;
	}
	// 2007-05-15 by bhsohn 기어 스탯 관련 처리		
	m_StatOverInfoFont->InvalidateDeviceObjects() ;
	m_StatInfoBoldFont->InvalidateDeviceObjects() ;

	for(i=0 ; i<3 ; i++)
	{
		m_pStat[i]->InvalidateDeviceObjects() ;
	}

	m_pFontName->InvalidateDeviceObjects();
	m_pFontLevel->InvalidateDeviceObjects();
	m_pFontGuild->InvalidateDeviceObjects();
	m_pFontGeneral->InvalidateDeviceObjects();
	m_pFontPropensity->InvalidateDeviceObjects();
	m_pFontAutoStat->InvalidateDeviceObjects();
	m_pFontStatDec->InvalidateDeviceObjects();
	for(i=0;i<C_QUEST_LIST_NUM;i++)
	{
		m_pFontQuestList[i]->InvalidateDeviceObjects() ;
//		m_pFontQuestState[i]->InvalidateDeviceObjects() ;
	}
	for(i=0;i<C_QUEST_DESC_NUM;i++)
	{
		m_pFontQuestDesc[i]->InvalidateDeviceObjects() ;
	}
	for(i=0 ; i<CHARACTER_INTERFACE_FONT_NUM ; i++)
	{
		m_pFontAmmoWeapon[i]->InvalidateDeviceObjects();
	}
	for(i=0; i<CHAR_UTC_SLOT_NUMBER; i++)
	{
		m_pFontItemNum[i]->InvalidateDeviceObjects() ;
	}
	// 2008-06-20 by bhsohn EP3 옵션관련 처리
	if(m_pOpenBtn)
	{		
		m_pOpenBtn->InvalidateDeviceObjects();		
	}
	// end 2008-06-20 by bhsohn EP3 옵션관련 처리
	
	m_bRestored = FALSE;
	m_bInvalidated = TRUE;

	return S_OK ;
}


void CINFCharacterInfo::Tick()
{
	FLOG( "CINFCharacterInfo::Tick()" );
//	if(m_nCharacterInfoType == CHARACTER_QUEST)
//	{
//		if(m_pQuestListScrollBar)
//		{
//			m_pQuestListScrollBar->SetNumberOfData( g_pQuestData->GetProgressQuestNum() + g_pQuestData->m_vecQuest.size());
//			m_pQuestListScrollBar->SetScrollLinePos( QUEST_LIST_SCROLL_START_X, g_pGameMain->m_nRightWindowY+QUEST_LIST_SCROLL_START_Y);
//			m_pQuestListScrollBar->SetWheelRect(C_QUEST_START_X, 
//				g_pGameMain->m_nRightWindowY+C_QUEST_START_Y,
//				C_QUEST_START_X+C_QUEST_SIZE_X,
//				g_pGameMain->m_nRightWindowY+C_QUEST_START_Y+C_QUEST_SIZE_Y);
//			if(m_nOldQuestSelectDataIndex != m_pQuestListScrollBar->GetCurrentSelectDataIndex())
//			{
//				m_nOldQuestSelectDataIndex = m_pQuestListScrollBar->GetCurrentSelectDataIndex();
//				SetQuestDesc(m_nOldQuestSelectDataIndex);
//			}
//		}
//		if(m_pQuestDescScrollBar)
//		{
//			//m_pQuestDescScrollBar->SetNumberOfData( m_vecQuestDescription.size() );
//			m_pQuestDescScrollBar->SetScrollLinePos( QUEST_DESC_SCROLL_START_X, g_pGameMain->m_nRightWindowY+QUEST_DESC_SCROLL_START_Y);
//			m_pQuestDescScrollBar->SetWheelRect(C_QUEST_DESC_START_X, 
//				g_pGameMain->m_nRightWindowY+C_QUEST_DESC_START_Y,
//				C_QUEST_DESC_START_X+C_QUEST_DESC_SIZE_X,
//				g_pGameMain->m_nRightWindowY+C_QUEST_DESC_START_Y+C_QUEST_DESC_SIZE_Y);
//		}
//	}

	// 2007-05-15 by bhsohn 기어 스탯 관련 처리
	// 상태 정보값 얻어옴.
	GetStatShowInfo();
	// end 2007-05-15 by bhsohn 기어 스탯 관련 처리
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			CINFCharacterInfo::RenderSkillInShop()
/// \brief		상점에서 왼쪽 창에 내 스킬 렌더링
/// \author		dhkwon
/// \date		2004-10-18 ~ 2004-10-18
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFCharacterInfo::RenderSkillInShop()
{
	// 2008-08-22 by bhsohn EP3 인벤토리 처리
//	if(m_pSkillShopBack )
//	{
//		m_pSkillShopBack->Move(SHOP_SKILL_BACK_START_X,SHOP_SKILL_BACK_START_Y);
//		m_pSkillShopBack->Render() ;
//	}
	// end 2008-08-22 by bhsohn EP3 인벤토리 처리
	CINFIcon* pIcon = ((CINFGameMain*)m_pParent)->m_pIcon;
	for(int i=0;i<CHAR_UTC_TYPE_NUMBER;i++)
	{
		for(int j=0;j<CHAR_UTC_SLOT_NUMBER;j++)
		{
			if(m_pUTCDisplayInfo[i][j].pItem)
			{
				// 2005-11-28 by ispark, SourceIndex로 변경, 스킬은 SKILL_BASE_NUM으로 한다.
				char buf[64];
				// 2013-03-13 by bhsohn 스킬 번호 구조 수정
//				wsprintf(buf, "%08d", SKILL_BASE_NUM(m_pUTCDisplayInfo[i][j].pItem->ItemInfo->SourceIndex));
				wsprintf(buf, "%08d", SKILL_BASE_SOURCEINDEX(m_pUTCDisplayInfo[i][j].pItem->ItemInfo->SourceIndex));
				pIcon->SetIcon(buf,
//					SHOP_SKILL_START_X+CHAR_UTC_INTERVAL_X*i+1, 
//					SHOP_SKILL_START_Y+CHAR_UTC_INTERVAL_Y*j+1, 
					SHOP_SKILL_START_X+SHOP_SKILL_INTERVAL_X*i+1, 
					SHOP_SKILL_START_Y+SHOP_SKILL_INTERVAL_Y*j+1, 
					1.0f);
				pIcon->Render();
				
				if(m_pUTCDisplayInfo[i][j].pItem->ItemInfo->SkillLevel > 0)
				{
					char buf[128];

					// 2008-08-22 by bhsohn EP3 인벤토리 처리
//					wsprintf(buf, "%d",m_pUTCDisplayInfo[i][j].pItem->ItemInfo->SkillLevel);
//					m_pFontItemNum[i]->DrawText(SHOP_SKILL_START_X+SHOP_SKILL_INTERVAL_X*i+21,
//										SHOP_SKILL_START_Y+SHOP_SKILL_INTERVAL_Y*j+14,
//						GUI_FONT_COLOR_Y,buf, 0L);
					
					{
						// 스킬레벨
						//"LV%d"
						wsprintf(buf, STRMSG_C_080922_0202, m_pUTCDisplayInfo[i][j].pItem->ItemInfo->SkillLevel);
						m_pFontItemNum[i]->DrawText(SHOP_SKILL_START_X+SHOP_SKILL_INTERVAL_X*i+21+20,
							SHOP_SKILL_START_Y+SHOP_SKILL_INTERVAL_Y*j,
							GUI_FONT_COLOR,buf, 0L);
					}
					{
						// 스킬이름
						strncpy(buf, m_pUTCDisplayInfo[i][j].pItem->ItemInfo->ItemName, SIZE_MAX_ITEM_NAME);
						m_pFontItemNum[i]->DrawText(SHOP_SKILL_START_X+SHOP_SKILL_INTERVAL_X*i+21+20,
							SHOP_SKILL_START_Y+SHOP_SKILL_INTERVAL_Y*j+14,
							GUI_FONT_COLOR,buf, 0L);
					}

					// end 2008-08-22 by bhsohn EP3 인벤토리 처리

//#if defined(LANGUAGE_ENGLISH) || defined(LANGUAGE_VIETNAM)|| defined(LANGUAGE_THAI)// 2008-04-30 by bhsohn 태국 버전 추가
//					wsprintf(buf, "%d",m_pUTCDisplayInfo[i][j].pItem->ItemInfo->SkillLevel);
//					m_pFontItemNum[i]->DrawText(SHOP_SKILL_START_X+SHOP_SKILL_INTERVAL_X*i+21,
//										SHOP_SKILL_START_Y+SHOP_SKILL_INTERVAL_Y*j+14,
//						GUI_FONT_COLOR_Y,buf, 0L);
//#else
//					wsprintf(buf, "%d",m_pUTCDisplayInfo[i][j].pItem->ItemInfo->SkillLevel);
//					m_pFontItemNum[i]->DrawText(SHOP_SKILL_START_X+SHOP_SKILL_INTERVAL_X*i+21,SHOP_SKILL_START_Y+CHAR_UTC_INTERVAL_Y*j+14,
//						GUI_FONT_COLOR_Y,buf, 0L);

//					wsprintf(buf, "%4.d",m_pUTCDisplayInfo[i][j].pItem->ItemInfo->SkillLevel);
//					m_pFontItemNum[i]->DrawText(SHOP_SKILL_START_X+CHAR_UTC_INTERVAL_X*i+2,SHOP_SKILL_START_Y+CHAR_UTC_INTERVAL_Y*j+15,
//						GUI_FONT_COLOR_Y,buf, 0L);
//#endif
				}
			}
		}
	}
	if(m_pSelectIcon)
	{
		POINT ptCursor;
		GetCursorPos( &ptCursor );
		ScreenToClient( g_pD3dApp->GetHwnd(), &ptCursor );
		CheckMouseReverse(&ptCursor);

		m_pSelectIcon->Move(ptCursor.x-m_nRenderMoveIconIntervalWidth, ptCursor.y-m_nRenderMoveIconIntervalHeight);
		m_pSelectIcon->Render();

	}
	m_pShopSkillScrollBar->Render();
}

void CINFCharacterInfo::Render()
{
	FLOG( "CINFCharacterInfo::Render()" );
	int nWindowPosY = g_pGameMain->m_nRightWindowY;
//	int nWindowPosY = POS_RIGHT_WINDOW_Y;
//	if(nWindowPosY < SIZE_ROLL_Y )
//		nWindowPosY = SIZE_ROLL_Y;
//	if(nWindowPosY > RIGHT_WINDOW_MAX_Y-SIZE_NORMAL_WINDOW_Y)
//		nWindowPosY = RIGHT_WINDOW_MAX_Y-SIZE_NORMAL_WINDOW_Y;
	if(m_pInfoLog )
	{
		m_pInfoLog->Move(POS_RIGHT_WINDOW_X+8, nWindowPosY + 5);
		m_pInfoLog->Render() ;
	}
	if(m_pId )
	{
		m_pId->Move(POS_RIGHT_WINDOW_X+12, nWindowPosY + 25);
		m_pId->Render() ;
	}
	char buff[MAX_PATH];

#if defined(LANGUAGE_ENGLISH) || defined(LANGUAGE_VIETNAM)|| defined(LANGUAGE_THAI) // 2008-04-30 by bhsohn 태국 버전 추가
	wsprintf(buff, "%d", g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.Level);
#else
	wsprintf(buff, "%2d", g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.Level);
#endif
	m_pFontLevel->DrawText(POS_RIGHT_WINDOW_X+INFO_LEVEL_START_X,
		nWindowPosY+ INFO_LEVEL_START_Y, GUI_FONT_COLOR, buff, 0 );

	// 2007-11-22 by bhsohn 아레나 통합서버
//	m_pFontName->DrawText(POS_RIGHT_WINDOW_X+INFO_ID_START_X,
//		nWindowPosY+ INFO_ID_START_Y, GUI_FONT_COLOR, g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.CharacterName, 0 );
	char szCharacterName[SIZE_MAX_ARENA_FULL_NAME];
	memset(szCharacterName, 0x00, SIZE_MAX_ARENA_FULL_NAME);
	strncpy(szCharacterName, g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.CharacterName, SIZE_MAX_CHARACTER_NAME);
	g_pD3dApp->ConevertArenaRenderUserName(g_pD3dApp->GetArenaState(), szCharacterName);
	m_pFontName->DrawText(POS_RIGHT_WINDOW_X+INFO_ID_START_X,
		nWindowPosY+ INFO_ID_START_Y, GUI_FONT_COLOR, szCharacterName, 0 );


	switch(m_nCharacterInfoType)
	{
	case CHARACTER_INFO:
		{
			if(m_pInfo )
			{
				m_pInfo->Move(POS_RIGHT_WINDOW_X+12,nWindowPosY + 48);
				m_pInfo->Render() ;
			}
			if(m_pStatImage )
			{
				m_pStatImage->Move(POS_RIGHT_WINDOW_X+12,nWindowPosY + 166);
				m_pStatImage->Render() ;
			}

			//
			CINFImage* pFace = ((CInterface*)m_pParent->m_pParent)->m_pPilotFace->FindPilotImage(g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.PilotFace);
			if(pFace)
			{
				pFace->Move( POS_RIGHT_WINDOW_X+INFO_START_X-3,nWindowPosY+INFO_START_Y );
				pFace->Render();
			}
			char buf[MAX_PATH];
			if( g_pShuttleChild->m_myShuttleInfo.GuildUniqueNumber != 0 )
			{
				wsprintf(buf, STRMSG_C_INTERFACE_0005, g_pShuttleChild->m_myShuttleInfo.GuildName );//"소속:%s"
				m_pFontGuild->DrawText( POS_RIGHT_WINDOW_X+GUILD_START_X, nWindowPosY + GUILD_START_Y, GUI_FONT_COLOR, buf, 0 );
			}
			wsprintf(buf, STRMSG_C_INTERFACE_0006, (LPTSTR)CAtumSJ::GetStatusNameKO(g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.Status) );//"신분:%s"
			m_pFontGeneral->DrawText( 
				POS_RIGHT_WINDOW_X+GENERAL_START_X,nWindowPosY + GENERAL_START_Y, GUI_FONT_COLOR, buf,0 );
			
			wsprintf(buf,STRMSG_C_INTERFACE_0007,g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.Propensity);//"명성:%d"
			m_pFontPropensity->DrawText( POS_RIGHT_WINDOW_X+PROPENSITY_START_X,nWindowPosY + PROPENSITY_START_Y, GUI_FONT_COLOR, buf,0 );

			wsprintf(buf, STRMSG_C_INTERFACE_0008, g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.PKWinPoint,g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.PKLossPoint);//"개인전:%3d승 %3d패"
			m_pFontGuild->DrawText( POS_RIGHT_WINDOW_X+GUILD_START_X, nWindowPosY + PROPENSITY_START_Y+18, GUI_FONT_COLOR, buf, 0 );
	
			// 길드 전
			if( g_pShuttleChild->m_myShuttleInfo.GuildUniqueNumber != 0 )
			{
				MSG_IC_GUILD_GET_GUILD_INFO_OK *pInfo = g_pGameMain->m_pCommunity->GetGuild()->GetGuildInfo();
				if(pInfo)
				{
					wsprintf(buf, STRMSG_C_INTERFACE_0009, pInfo->WarWinPoint, pInfo->WarLossPoint);//"길드전:%3d승 %3d패"
					m_pFontGuild->DrawText( POS_RIGHT_WINDOW_X+GUILD_START_X, nWindowPosY + PROPENSITY_START_Y+36, GUI_FONT_COLOR, buf, 0 );
				}
			}
			// 자동 스텟관련
			// 2005-07-04 by ispark 
			// 자동 스탯 분배 삭제
//			m_pFontAutoStat->DrawText(POS_RIGHT_WINDOW_X+GUILD_START_X-32, nWindowPosY + PROPENSITY_START_Y+64,
//				GUI_FONT_COLOR, g_pSelect->m_strAutoStat);
			
			// 자동 스텟 툴팁 설명
//			if(TRUE == m_bToolTipStat){
//				RenderStatDescToolTip(g_pShuttleChild->m_myShuttleInfo.AutoStatType, POS_RIGHT_WINDOW_X+GUILD_START_X-92,
//					nWindowPosY + PROPENSITY_START_Y+44);
//			}

			ShowStatInfo() ;

			// 2008-06-20 by bhsohn EP3 옵션관련 처리
			if(m_pOpenBtn)
			{				
				int nPosX = POS_RIGHT_WINDOW_X+143;
				int	nPosY = nWindowPosY + 162;
				
				m_pOpenBtn->SetBtnPosition(nPosX, nPosY);	
				m_pOpenBtn->Render();			
			}
			// end 2008-06-20 by bhsohn EP3 옵션관련 처리
		}
		break;

	case CHARACTER_AMMO:
		{
//			char buf[MAX_PATH];

			if(m_pAmmo )
			{
				m_pAmmo->Move(POS_RIGHT_WINDOW_X+12,nWindowPosY + 48);
				m_pAmmo->Render() ;
			}
//			wsprintf(buff, "속    도 : %3.d ms ~ %3.d ms", (int) g_pShuttleChild->m_fRealShuttleSpeedMin, (int) g_pShuttleChild->m_fRealShuttleSpeedMax );
//			m_CharacInfoFont[0]->DrawText(POS_RIGHT_WINDOW_X+INFO_START_X, nWindowPosY+OTHER_INFO_START_Y,GUI_FONT_COLOR, buff, 0 );

//			wsprintf(buff, "최대가속 : %3.d ms", (int) g_pShuttleChild->m_fRealShuttleSpeedBoosterOn );
//			m_CharacInfoFont[1]->DrawText(POS_RIGHT_WINDOW_X+INFO_START_X, nWindowPosY+OTHER_INFO_START_Y+INFO_INTERVAL,GUI_FONT_COLOR, buff, 0 );

//			wsprintf(buff, "적 재 량 : %d%%(%dKg/%dKg)", 
//				(int) (g_pStoreData->GetTotalWeight() / CAtumSJ::GetTransport(&g_pShuttleChild->m_myShuttleInfo) *100.0f ), 
//				(int) g_pStoreData->GetTotalWeight(), (int)CAtumSJ::GetTransport(&g_pShuttleChild->m_myShuttleInfo) );
//			m_CharacInfoFont[3]->DrawText(POS_RIGHT_WINDOW_X+INFO_START_X, nWindowPosY+OTHER_INFO_START_Y+INFO_INTERVAL*2,GUI_FONT_COLOR, buff, 0 );

//			sprintf(buff,  "경 험 치 : %.2f%%", ((float)((int)(g_pD3dApp->m_pShuttleChild->m_fExpPer*100)))/100 );
//			m_CharacInfoFont[4]->DrawText(POS_RIGHT_WINDOW_X+INFO_START_X, nWindowPosY+OTHER_INFO_START_Y+INFO_INTERVAL*3,GUI_FONT_COLOR, buff, 0 );

//			CItemInfo * pItemInfo = g_pStoreData->FindItemInInventoryByWindowPos( POS_CENTER );
//			sprintf( buff, "방어력:%.1f%%,%.1f%% 회피력:%.1f%%,%.1f", 
//				((float)CAtumSJ::GetDefenseC( &g_pShuttleChild->m_myShuttleInfo, &g_pShuttleChild->m_paramFactor, TRUE ) / PROB256_MAX_VALUE)*100.0f,
//				((float)CAtumSJ::GetDefenseC( &g_pShuttleChild->m_myShuttleInfo, &g_pShuttleChild->m_paramFactor, FALSE ) / PROB256_MAX_VALUE)*100.0f,
//				(CAtumSJ::GetDefenseProbabilityC( &g_pShuttleChild->m_myShuttleInfo, (pItemInfo==NULL?NULL:pItemInfo->GetRealItemInfo()), &g_pShuttleChild->m_paramFactor, TRUE ) / PROB256_MAX_VALUE)*100.0f,
//				(CAtumSJ::GetDefenseProbabilityC( &g_pShuttleChild->m_myShuttleInfo, (pItemInfo==NULL?NULL:pItemInfo->GetRealItemInfo()), &g_pShuttleChild->m_paramFactor, FALSE ) / PROB256_MAX_VALUE)*100.0f);
//			m_pFontAmmoWeapon[4]->DrawText( POS_RIGHT_WINDOW_X+AMMO_DEFENSE_START_X, nWindowPosY + AMMO_START_Y+AMMO_INTERVAL*4, GUI_FONT_COLOR, buff, 0L );

			CItemInfo * pItemInfo = g_pStoreData->FindItemInInventoryByWindowPos( POS_CENTER );

			//sprintf( buff, STRMSG_C_INTERFACE_0010, ((float)CAtumSJ::GetDefenseC( &g_pShuttleChild->m_myShuttleInfo, &g_pShuttleChild->m_paramFactor, TRUE ) / PROB256_MAX_VALUE)*100.0f );//"방어력 기본무기 : %.2f%%"
			// 2010-07-28 by dgwoo 확률 수식 변경 (255 => 100%)
			sprintf( buff, STRMSG_C_INTERFACE_0010, (float)CAtumSJ::GetDefenseC( &g_pShuttleChild->m_myShuttleInfo, &g_pShuttleChild->m_paramFactor, TRUE ));//"방어력 기본무기 : %.2f%%"
			m_CharacInfoFont[0]->DrawText(POS_RIGHT_WINDOW_X+INFO_START_X, nWindowPosY+OTHER_INFO_START_Y,GUI_FONT_COLOR, buff, 0 );

			//sprintf( buff, STRMSG_C_INTERFACE_0011, ((float)CAtumSJ::GetDefenseC( &g_pShuttleChild->m_myShuttleInfo, &g_pShuttleChild->m_paramFactor, FALSE ) / PROB256_MAX_VALUE)*100.0f );//"       고급무기 : %.2f%%"
			// 2010-07-28 by dgwoo 확률 수식 변경 (255 => 100%)
			sprintf( buff, STRMSG_C_INTERFACE_0011, (float)CAtumSJ::GetDefenseC( &g_pShuttleChild->m_myShuttleInfo, &g_pShuttleChild->m_paramFactor, FALSE ));//"       고급무기 : %.2f%%"
			m_CharacInfoFont[1]->DrawText(POS_RIGHT_WINDOW_X+INFO_START_X+CHARACTER_AMMO_TEXT_X1, nWindowPosY+OTHER_INFO_START_Y+INFO_INTERVAL,GUI_FONT_COLOR, buff, 0 );

			sprintf( buff, STRMSG_C_INTERFACE_0012, //"회피력 기본무기 : %.2f%%"
				//(CAtumSJ::GetDefenseProbabilityC( &g_pShuttleChild->m_myShuttleInfo, (pItemInfo==NULL?NULL:pItemInfo->GetRealItemInfo()), &g_pShuttleChild->m_paramFactor, TRUE ) / PROB256_MAX_VALUE)*100.0f );
				// 2010-07-28 by dgwoo 확률 수식 변경 (255 => 100%)
				CAtumSJ::GetDefenseProbabilityC( &g_pShuttleChild->m_myShuttleInfo, (pItemInfo==NULL?NULL:pItemInfo->GetRealItemInfo()), &g_pShuttleChild->m_paramFactor, TRUE ) );
			m_CharacInfoFont[3]->DrawText(POS_RIGHT_WINDOW_X+INFO_START_X, nWindowPosY+OTHER_INFO_START_Y+INFO_INTERVAL*2,GUI_FONT_COLOR, buff, 0 );

			sprintf( buff, STRMSG_C_INTERFACE_0011, //"       고급무기 : %.2f%%"
				//(CAtumSJ::GetDefenseProbabilityC( &g_pShuttleChild->m_myShuttleInfo, (pItemInfo==NULL?NULL:pItemInfo->GetRealItemInfo()), &g_pShuttleChild->m_paramFactor, FALSE ) / PROB256_MAX_VALUE)*100.0f );
				// 2010-07-28 by dgwoo 확률 수식 변경 (255 => 100%)
				CAtumSJ::GetDefenseProbabilityC( &g_pShuttleChild->m_myShuttleInfo, (pItemInfo==NULL?NULL:pItemInfo->GetRealItemInfo()), &g_pShuttleChild->m_paramFactor, FALSE ) );
			m_CharacInfoFont[4]->DrawText(POS_RIGHT_WINDOW_X+INFO_START_X+CHARACTER_AMMO_TEXT_X1, nWindowPosY+OTHER_INFO_START_Y+INFO_INTERVAL*3,GUI_FONT_COLOR, buff, 0 );
			
			if(g_pShuttleChild->m_pPrimaryWeapon)
			{
				// 2010. 02. 19 by ckPark 100레벨미만일시 300스탯초과량이 적용되는 버그 수정

// 				sprintf( buff, STRMSG_C_INTERFACE_0013,//"공격확율 : %.2f"
// 						 (CAtumSJ::GetAttackProbabilityC(g_pShuttleChild->m_pPrimaryWeapon->GetRealItemInfo(), &g_pShuttleChild->m_paramFactor, TRUE, g_pShuttleChild->m_myShuttleInfo.TotalGearStat.AttackPart )*100)/255);	// 캐릭터의 공격 확률

				sprintf( buff, STRMSG_C_INTERFACE_0013,//"공격확율 : %.2f"
						// 2010-07-28 by dgwoo 확률 수식 변경 (255 => 100%)
						 //(CAtumSJ::GetAttackProbabilityC(g_pShuttleChild->m_pPrimaryWeapon->GetRealItemInfo(), &g_pShuttleChild->m_paramFactor, TRUE, g_pShuttleChild->m_myShuttleInfo.TotalGearStat.AttackPart, g_pShuttleChild->m_myShuttleInfo.Level )*100)/255);	// 캐릭터의 공격 확률
						 CAtumSJ::GetAttackProbabilityC(g_pShuttleChild->m_pPrimaryWeapon->GetRealItemInfo(), &g_pShuttleChild->m_paramFactor, TRUE, g_pShuttleChild->m_myShuttleInfo.TotalGearStat.AttackPart, g_pShuttleChild->m_myShuttleInfo.Level ));	// 캐릭터의 공격 확률

				// end 2010. 02. 19 by ckPark 100레벨미만일시 300스탯초과량이 적용되는 버그 수정

				m_pFontAmmoWeapon[0]->DrawText( POS_RIGHT_WINDOW_X+AMMO_START_X, nWindowPosY + AMMO_START_Y, GUI_FONT_COLOR, buff, 0L );

				sprintf( buff, STRMSG_C_INTERFACE_0014, CAtumSJ::GetMinAttackPerSecondC(&g_pShuttleChild->m_myShuttleInfo, //"초당공격력 : %.2f ~ %.2f"
					g_pShuttleChild->m_pPrimaryWeapon->GetRealItemInfo(), &g_pShuttleChild->m_paramFactor, TRUE ), 
					CAtumSJ::GetMaxAttackPerSecondC(&g_pShuttleChild->m_myShuttleInfo, 
					g_pShuttleChild->m_pPrimaryWeapon->GetRealItemInfo(), &g_pShuttleChild->m_paramFactor, TRUE ) );
				m_pFontAmmoWeapon[1]->DrawText( POS_RIGHT_WINDOW_X+AMMO_START_X-CHARACTER_AMMO_TEXT_X2, nWindowPosY + AMMO_START_Y+AMMO_INTERVAL, GUI_FONT_COLOR, buff, 0L );				
			}
			if(g_pShuttleChild->m_pSecondaryWeapon)
			{
				// 2010. 02. 19 by ckPark 100레벨미만일시 300스탯초과량이 적용되는 버그 수정

// 				sprintf( buff, STRMSG_C_INTERFACE_0013,//"공격확율 : %.2f"
// 				(CAtumSJ::GetAttackProbabilityC(g_pShuttleChild->m_pSecondaryWeapon->GetRealItemInfo(),
// 				&g_pShuttleChild->m_paramFactor, FALSE, g_pShuttleChild->m_myShuttleInfo.TotalGearStat.AttackPart)*100)/255 );	// 캐릭터의 공격 확률

				sprintf( buff, STRMSG_C_INTERFACE_0013,//"공격확율 : %.2f"
						// 2010-07-28 by dgwoo 확률 수식 변경 (255 => 100%)
						// (CAtumSJ::GetAttackProbabilityC(g_pShuttleChild->m_pSecondaryWeapon->GetRealItemInfo(),
						 //&g_pShuttleChild->m_paramFactor, FALSE, g_pShuttleChild->m_myShuttleInfo.TotalGearStat.AttackPart, g_pShuttleChild->m_myShuttleInfo.Level)*100)/255 );	// 캐릭터의 공격 확률
						 CAtumSJ::GetAttackProbabilityC(g_pShuttleChild->m_pSecondaryWeapon->GetRealItemInfo(),
						 &g_pShuttleChild->m_paramFactor, FALSE, g_pShuttleChild->m_myShuttleInfo.TotalGearStat.AttackPart, g_pShuttleChild->m_myShuttleInfo.Level));	// 캐릭터의 공격 확률

				// end 2010. 02. 19 by ckPark 100레벨미만일시 300스탯초과량이 적용되는 버그 수정

				m_pFontAmmoWeapon[2]->DrawText( POS_RIGHT_WINDOW_X+AMMO_START_X, nWindowPosY + AMMO_START_Y+AMMO_INTERVAL*2, GUI_FONT_COLOR, buff, 0L );

				sprintf( buff, STRMSG_C_INTERFACE_0014, CAtumSJ::GetMinAttackPerSecondC(&g_pShuttleChild->m_myShuttleInfo, 
					g_pShuttleChild->m_pSecondaryWeapon->GetRealItemInfo(), &g_pShuttleChild->m_paramFactor, FALSE ), 
					CAtumSJ::GetMaxAttackPerSecondC(&g_pShuttleChild->m_myShuttleInfo, 
					g_pShuttleChild->m_pSecondaryWeapon->GetRealItemInfo(), &g_pShuttleChild->m_paramFactor, FALSE ) );
				m_pFontAmmoWeapon[3]->DrawText( POS_RIGHT_WINDOW_X+AMMO_START_X-CHARACTER_AMMO_TEXT_X2, nWindowPosY + AMMO_START_Y+AMMO_INTERVAL*3, GUI_FONT_COLOR, buff, 0L );				
			}
			wsprintf(buff, STRMSG_C_INTERFACE_0015, //"적 재 량 : %d%%(%dKg/%dKg)"
				(int) (g_pStoreData->GetTotalWeight() / CAtumSJ::GetTransport(&g_pShuttleChild->m_myShuttleInfo) *100.0f ), 
				(int) g_pStoreData->GetTotalWeight(), (int)CAtumSJ::GetTransport(&g_pShuttleChild->m_myShuttleInfo) );
			m_pFontAmmoWeapon[4]->DrawText( POS_RIGHT_WINDOW_X+AMMO_DEFENSE_START_X, nWindowPosY + AMMO_START_Y+AMMO_INTERVAL*4, GUI_FONT_COLOR, buff, 0L );
			
			// 2008-09-22 by bhsohn EP3 캐릭터 창
//			sprintf(buff, STRMSG_C_070116_0100, //"피어스율 : %.2f%%"
//				(float)(CAtumSJ::GetAttackC_PierceAttackProbability((int)g_pShuttleChild->m_myShuttleInfo.TotalGearStat.AttackPart)) / (float)PROB256_MAX_VALUE *100.0f);
//			m_pFontAmmoWeapon[5]->DrawText(POS_RIGHT_WINDOW_X+AMMO_DEFENSE_START_X,nWindowPosY + AMMO_START_Y+AMMO_INTERVAL*5, GUI_FONT_COLOR, buff, 0L);
		}
		break;
	case CHARACTER_UTC:
		{
			if(m_pUtc )
			{
				m_pUtc->Move(POS_RIGHT_WINDOW_X+12,nWindowPosY + 48);
				m_pUtc->Render() ;
			}
			CINFIcon* pIcon = ((CINFGameMain*)m_pParent)->m_pIcon;
			for(int i=0;i<CHAR_UTC_TYPE_NUMBER;i++)
			{
				for(int j=0;j<CHAR_UTC_SLOT_NUMBER;j++)
				{
					if(m_pUTCDisplayInfo[i][j].pItem)
					{
						// 2005-11-28 by ispark, SourceIndex로 변경, 스킬은 SKILL_BASE_NUM으로 한다.
						char buf[64];
						// 2013-03-13 by bhsohn 스킬 번호 구조 수정
//						wsprintf(buf, "%08d", SKILL_BASE_NUM(m_pUTCDisplayInfo[i][j].pItem->ItemInfo->SourceIndex));
						wsprintf(buf, "%08d", SKILL_BASE_SOURCEINDEX(m_pUTCDisplayInfo[i][j].pItem->ItemInfo->SourceIndex));
						pIcon->SetIcon(buf,
							CHAR_UTC_START_X+CHAR_UTC_INTERVAL_X*i+1, CHAR_UTC_START_Y+CHAR_UTC_INTERVAL_Y*j+1, 1.0f);
						pIcon->Render();
						
//						if( IS_COUNTABLE_ITEM(m_pUTCDisplayInfo[i][j].pItem->Kind) )
//						{
//							CItemInfo* pItemInfo = g_pStoreData->FindItemInInventoryByUniqueNumber( 
//								m_pUTCDisplayInfo[i][j].pItem->UniqueNumber );
//							if( pItemInfo->CurrentCount > 1 )
//							{
								// 갯수를 보여준다.
								if(m_pUTCDisplayInfo[i][j].pItem->ItemInfo->SkillLevel > 0)
								{
									char buf[128];
#if defined(LANGUAGE_ENGLISH) || defined(LANGUAGE_VIETNAM)|| defined(LANGUAGE_THAI)// 2008-04-30 by bhsohn 태국 버전 추가
									wsprintf(buf, "%d",m_pUTCDisplayInfo[i][j].pItem->ItemInfo->SkillLevel);
									m_pFontItemNum[i]->DrawText(CHAR_UTC_START_X+CHAR_UTC_INTERVAL_X*i+21,CHAR_UTC_START_Y+CHAR_UTC_INTERVAL_Y*j+14,
										GUI_FONT_COLOR_Y,buf, 0L);
#else
									wsprintf(buf, "%d",m_pUTCDisplayInfo[i][j].pItem->ItemInfo->SkillLevel);
									m_pFontItemNum[i]->DrawText(CHAR_UTC_START_X+CHAR_UTC_INTERVAL_X*i+21,CHAR_UTC_START_Y+CHAR_UTC_INTERVAL_Y*j+14,
										GUI_FONT_COLOR_Y,buf, 0L);
#endif
								}
//							}
//						}
					}
				}
			}
// 2006-07-27 by ispark
//			if(m_pSelectIcon)
//			{
//				POINT ptCursor;
//				GetCursorPos( &ptCursor );
//				ScreenToClient( g_pD3dApp->GetHwnd(), &ptCursor );
//				CheckMouseReverse(&ptCursor);				
//
//				m_pSelectIcon->Move(ptCursor.x-m_nRenderMoveIconIntervalWidth, ptCursor.y-m_nRenderMoveIconIntervalHeight);
//				m_pSelectIcon->Render();
//
//			}
			m_pUtcScr->Move(UTC_SCROLL_BUTTON_START_X,
				nWindowPosY+UTC_SCROLL_BAR_START_Y+UTC_SCROLL_INTERVAL*m_nCurrentUTCScroll);
		m_pUtcScr->Render() ;

		}
		break;


	case CHARACTER_ARENA:
		{
			m_pImgArenaTab->Move(POS_RIGHT_WINDOW_X+12,nWindowPosY + 48);
			m_pImgArenaTab->Render();
			SIZE size;
			const auto& ShuttleInfo = g_pShuttleChild->GetMyShuttleInfo();
			wsprintf(buff,STRMSG_C_070607_0108,ShuttleInfo.ArenaWin,ShuttleInfo.ArenaLose);
			size = m_pFontGeneral->GetStringSize(buff);
			m_pFontGeneral->DrawText(ARENA_SCORE_X - size.cx,ARENA_SCORE_Y,GUI_FONT_COLOR,buff);
			UINT nPer = 0;
			if((ShuttleInfo.ArenaWin+ShuttleInfo.ArenaLose) > 0)
			{
				FLOAT fPer = (FLOAT)ShuttleInfo.ArenaWin/(FLOAT)(ShuttleInfo.ArenaWin+ShuttleInfo.ArenaLose);
				nPer = fPer*100;
			}
			wsprintf(buff,"%d%%",nPer);
			size = m_pFontGeneral->GetStringSize(buff);
			m_pFontGeneral->DrawText(ARENA_SCORE_PERCENTAGE_X - size.cx,ARENA_SCORE_PERCENTAGE_Y,GUI_FONT_COLOR,buff);

			wsprintf(buff,STRMSG_C_070607_0107,ShuttleInfo.ArenaDisConnect);
			size = m_pFontGeneral->GetStringSize(buff);
			m_pFontGeneral->DrawText(ARENA_COMPULSION_END_X - size.cx,ARENA_COMPULSION_END_Y,GUI_FONT_COLOR,buff);

			char buf[MAX_PATH];
			wsprintf(buf,"%d",ShuttleInfo.CumulativeWarPoint);
			MakeCurrencySeparator(buff,buf,3,',');
			size = m_pFontGeneral->GetStringSize(buff);
			m_pFontGeneral->DrawText(ARENA_COMMULATION_POINT_X - size.cx,ARENA_COMMULATION_POINT_Y,GUI_FONT_COLOR,buff);

			wsprintf(buf,"%d",ShuttleInfo.WarPoint);
			MakeCurrencySeparator(buff,buf,3,',');
			size = m_pFontGeneral->GetStringSize(buff);
			m_pFontGeneral->DrawText(ARENA_FUSIBLE_POINT_X - size.cx,ARENA_FUSIBLE_POINT_Y,GUI_FONT_COLOR,buff);


		}
		break;
	}
}

void CINFCharacterInfo::SetAllUTCInfo()
{
	FLOG( "CINFCharacterInfo::SetAllUTCInfo()" );

	m_pShopSkillScrollBar->SetNumberOfData( GetUtcMaxScrollNumber() );
	char strIconName[64];
	
	memset(m_pUTCDisplayInfo, 0x00, sizeof(INVEN_DISPLAY_INFO)*CHAR_UTC_TYPE_NUMBER*CHAR_UTC_SLOT_NUMBER);

//	map<int, ITEM_SKILL*>::iterator itSkill = g_pShuttleChild->m_pSkill->m_mapEnableSkill.begin();
	map<int, CSkillInfo*>::iterator itSkillinfo = g_pShuttleChild->m_pSkill->m_mapSkill.begin();
	while(itSkillinfo != g_pShuttleChild->m_pSkill->m_mapSkill.end() )
	{
		CSkillInfo* pSkillInfo = itSkillinfo->second;

		// 2013-03-19 by bhsohn 스킬 번호 구조 수정
// 		int nTemp = pSkillInfo->ItemNum / 1000;
// 		int nWindowNum = (pSkillInfo->ItemNum  % nTemp) / 10 -1;
// 		int nSkillType = pSkillInfo->ItemNum /1000 - (pSkillInfo->ItemNum /10000)*10;
		int nBaseNum = max(1, SKILL_BASE_NUM(pSkillInfo->ItemNum));
		int nTemp = max(1, nBaseNum / 1000);
		int nWindowNum = (nBaseNum  % nTemp) / 10 -1;
		int nSkillType = min((CHAR_UTC_TYPE_NUMBER-1), SKILL_KIND_INDEX(pSkillInfo->ItemNum));
		//END 2013-03-19 by bhsohn 스킬 번호 구조 수정

		if(pSkillInfo->ItemInfo && nWindowNum >= m_nCurrentUTCScroll && nWindowNum < m_nCurrentUTCScroll+CHAR_UTC_SLOT_NUMBER)
		{
//			wsprintf(strIconName,"%08d",SKILL_BASE_NUM(pSkillInfo->ItemNum));	// 2005-08-23 by ispark
			// 2013-03-13 by bhsohn 스킬 번호 구조 수정
//			wsprintf(strIconName,"%08d",SKILL_BASE_NUM(pSkillInfo->ItemInfo->SourceIndex));
			wsprintf(strIconName,"%08d",SKILL_BASE_SOURCEINDEX(pSkillInfo->ItemInfo->SourceIndex));
			strcpy(m_pUTCDisplayInfo[nSkillType][nWindowNum-m_nCurrentUTCScroll].IconName, strIconName);
			if(pSkillInfo)
			{
				strcpy(m_pUTCDisplayInfo[nSkillType][nWindowNum-m_nCurrentUTCScroll].Name, pSkillInfo->ItemInfo->ItemName);
			}
			else
			{
				memset(m_pUTCDisplayInfo[nSkillType][nWindowNum-m_nCurrentUTCScroll].Name,0x00, sizeof(m_pUTCDisplayInfo[nSkillType][nWindowNum-m_nCurrentUTCScroll].Name));
			}
			m_pUTCDisplayInfo[nSkillType][nWindowNum-m_nCurrentUTCScroll].pItem = pSkillInfo;
		}
		itSkillinfo++;
	}

}

int CINFCharacterInfo::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	FLOG( "CINFCharacterInfo::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)" );
	switch(uMsg)
	{
	case WM_MOUSEMOVE:
		{
//			POINT pt = MAKEPOINT(lParam);

		}
		break;
	case WM_LBUTTONDOWN:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);
			int nWindowPosY = g_pGameMain->m_nRightWindowY;

			if(pt.y>nWindowPosY + 48 && pt.y<nWindowPosY + 62)
			{
				if(pt.x>POS_RIGHT_WINDOW_X+12 &&
				   pt.x<POS_RIGHT_WINDOW_X+51 )
				{
					m_nCharacterInfoType = CHARACTER_INFO;
				}
				if(pt.x>POS_RIGHT_WINDOW_X+52 &&
				   pt.x<POS_RIGHT_WINDOW_X+103 )
				{
					m_nCharacterInfoType = CHARACTER_AMMO;
				}
				if(pt.x>POS_RIGHT_WINDOW_X+104 &&
				   pt.x<POS_RIGHT_WINDOW_X+138 )
				{
					m_nCharacterInfoType = CHARACTER_UTC;
				}
				if(pt.x>POS_RIGHT_WINDOW_X+139 &&
				   pt.x<POS_RIGHT_WINDOW_X+193 )
				{
					m_nCharacterInfoType = CHARACTER_ARENA;
				}
			}

			// 2005-08-05 by ispark
			// AMMO 창안에 마우스 클릭시 무효
			if(pt.y>nWindowPosY &&
				pt.y<nWindowPosY+SIZE_NORMAL_WINDOW_Y &&
				pt.x>POS_RIGHT_WINDOW_X &&
				pt.x<POS_RIGHT_WINDOW_X + SIZE_NORMAL_WINDOW_X &&
				m_nCharacterInfoType == CHARACTER_AMMO)
			{
				g_pInterface->SetWindowOrder(WNDLeftRightWindow);
			}

		}
		break;
	case WM_LBUTTONUP:
		{
//			POINT pt = MAKEPOINT(lParam);

		}
		break;
	}
	switch(m_nCharacterInfoType)
	{
	case CHARACTER_INFO:
		{
			return InfoWndProc( uMsg, wParam, lParam);
		}
		break;
	case CHARACTER_AMMO:
		{
			return AmmoWndProc( uMsg, wParam, lParam);
			
		}
		break;
	case CHARACTER_UTC:
		{
			return UTCWndProc(uMsg, wParam, lParam);
		}
		break;
	case CHARACTER_ARENA:
		{
			return ArenaWndProc(uMsg,wParam,lParam);
		}
		break;
	}
	return INF_MSGPROC_NORMAL;
}


int CINFCharacterInfo::GetCharacterQuestCount()
{
	FLOG( "CINFCharacterInfo::GetCharacterQuestCount()" );
	// 2008-09-22 by bhsohn EP3 캐릭터 창
//	map<int, MEX_QUEST_INFO*>::iterator it = g_pQuestData->m_mapCharacterQuest.begin();
//	int count = 0;
//	while(it != g_pQuestData->m_mapCharacterQuest.end())
//	{
//		if(it->second->QuestState == QUEST_STATE_IN_PROGRESS)
//		{
//			count++;
//		}
//		it++;
//	}
	int count = 0;
	return count;
}

int CINFCharacterInfo::QuestWndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	FLOG( "CINFCharacterInfo::QuestWndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)" );
	if(m_pQuestListScrollBar)
	{
		if(m_pQuestListScrollBar->WndProc(uMsg, wParam, lParam) == INF_MSGPROC_BREAK)
		{
			return INF_MSGPROC_BREAK;
		}
	}
	if(m_pQuestDescScrollBar)
	{
		if(m_pQuestDescScrollBar->WndProc(uMsg, wParam, lParam) == INF_MSGPROC_BREAK)
		{
			return INF_MSGPROC_BREAK;
		}
	}
	switch(uMsg)
	{
	case WM_MOUSEMOVE:
		{
//			POINT pt = MAKEPOINT(lParam);
//			if(m_bQuestListLock)
//			{
//				int nMove = pt.y - m_ptMouse.y;
//				int count = GetCharacterQuestCount();
//				if(count > 0 && abs(nMove) > CHAR_QUEST_LIST_SCROLL_BAR_INTERVAL(count))
//				{
//					m_nCurrentQuestListScroll += nMove / CHAR_QUEST_LIST_SCROLL_BAR_INTERVAL(count);
//					if(m_nCurrentQuestListScroll < 0)
//						m_nCurrentQuestListScroll = 0;
//					if(m_nCurrentQuestListScroll > count)
//						m_nCurrentQuestListScroll = count;
//					m_ptMouse = pt;
//				}
//			}
//			if(m_bQuestDescLock)////// 구현해야 한다.
//			{
//				int nMove = pt.y - m_ptMouse.y;
//				int count = 4;// line number
//				if(abs(nMove) > CHAR_QUEST_DESC_SCROLL_BAR_INTERVAL(count))
//				{
//					m_nCurrentQuestDescScroll += nMove / CHAR_QUEST_DESC_SCROLL_BAR_INTERVAL(count);
//					if(m_nCurrentQuestDescScroll < 0)
//						m_nCurrentQuestDescScroll = 0;
//					if(m_nCurrentQuestDescScroll > count)
//						m_nCurrentQuestDescScroll = count;
//					m_ptMouse = pt;
//				}
//			}
		}
		break;
	case WM_LBUTTONDOWN:
		{
//			POINT pt = MAKEPOINT(lParam);
//			int nWindowPosY = g_pGameMain->m_nRightWindowY;
//			int count = GetCharacterQuestCount();
//			if( pt.x > POS_RIGHT_WINDOW_X+CHAR_QUEST_LIST_SCROLL_BAR_START_X &&
//				pt.x < POS_RIGHT_WINDOW_X+CHAR_QUEST_LIST_SCROLL_BAR_START_X+CHAR_SCROLL_BAR_SIZE_X &&
//				pt.y > nWindowPosY+CHAR_QUEST_LIST_SCROLL_BAR_START_Y+CHAR_QUEST_LIST_SCROLL_BAR_INTERVAL(count)*m_nCurrentQuestListScroll &&
//				pt.y < nWindowPosY+CHAR_QUEST_LIST_SCROLL_BAR_START_Y+CHAR_QUEST_LIST_SCROLL_BAR_INTERVAL(count)*m_nCurrentQuestListScroll+CHAR_SCROLL_BAR_SIZE_Y )
//			{
//				m_bQuestListLock = TRUE;
//				m_ptMouse = pt;
//			}
		}
		break;
	case WM_LBUTTONUP:
		{
//			m_bQuestListLock = FALSE;
		}
		break;
//#define CHAR_QUEST_LINE_SIZE_X			169
//#define CHAR_QUEST_LINE_SIZE_Y			16
//#define	CHAR_QUEST_LINE_START_X			19
//#define	CHAR_QUEST_LINE_START_Y			70

	case WM_LBUTTONDBLCLK:
		{
/*			POINT pt = MAKEPOINT(lParam);
			int nWindowPosY = g_pGameMain->m_nRightWindowY;
			if( g_pQuestData->m_mapCharacterQuest.size()>0 && 
				pt.x>POS_RIGHT_WINDOW_X+CHAR_QUEST_LINE_START_X && 
				pt.x<POS_RIGHT_WINDOW_X+CHAR_QUEST_LINE_START_X+CHAR_QUEST_LINE_SIZE_X)
			{
				int line = (pt.y-nWindowPosY-CHAR_QUEST_LINE_START_Y)/CHARACTER_QUEST_INTERVAL;
				if(line >=0 && line < 6 && pt.y>nWindowPosY+CHAR_QUEST_LINE_START_Y)
				{
					map<int, MEX_QUEST_INFO*>::iterator it = g_pQuestData->m_mapCharacterQuest.begin();
					int i = -1;
					while(it != g_pQuestData->m_mapCharacterQuest.end() )
					{
						if(it->second->QuestState == QUEST_STATE_IN_PROGRESS)
						{
							i++;
							if(line == i)
								break;
						}
						it++;
					}
					if(it == g_pQuestData->m_mapCharacterQuest.end() )
						return INF_MSGPROC_NORMAL;
					MEX_QUEST_INFO* pInfo = it->second;
					map<int, CQuest*>::iterator it2 = g_pQuestData->m_mapQuest.find(pInfo->QuestIndex);
					if(it2 != g_pQuestData->m_mapQuest.end())
					{
						if(it2->second->QuestEndType == QUEST_END_TYPE_IMMEDIATE)//클릭시 즉시 종료 아이템
						{
							MSG_FC_QUEST_ACCEPT_QUEST sMsg;
							memset(&sMsg,0x00,sizeof(sMsg));
							char buffer[SIZE_MAX_PACKET];
							sMsg.QuestIndex = pInfo->QuestIndex;
							int nType = T_FC_QUEST_ACCEPT_QUEST;
							memcpy(buffer, &nType, SIZE_FIELD_TYPE_HEADER);
							memcpy(buffer+SIZE_FIELD_TYPE_HEADER, &sMsg, sizeof(sMsg));
							g_pD3dApp->m_pFieldWinSocket->Write(buffer, SIZE_FIELD_TYPE_HEADER + sizeof(sMsg));
						}
						else if(it2->second->QuestEndType == QUEST_END_TYPE_REQESTED_BY_CLIENT)
						{// 진행중인 클라이언트 퀘스트는 중간 대화를 보여준다.
							g_pGameMain->m_pMission->SetQuest(it2->second,pInfo);
						}
					}
				}
			}
*/		}
		break;
	}
	return INF_MSGPROC_NORMAL;
}

int CINFCharacterInfo::WndProcSkillInShop(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int bReturn = m_pShopSkillScrollBar->WndProc( uMsg, wParam, lParam );
	if( m_nCurrentUTCScroll != m_pShopSkillScrollBar->GetCurrentScrollIndex())
	{
		m_nCurrentUTCScroll = m_pShopSkillScrollBar->GetCurrentScrollIndex();
		SetAllUTCInfo();
	}
	if( bReturn == INF_MSGPROC_BREAK )
	{
		return INF_MSGPROC_BREAK;
	}

	switch(uMsg)
	{
	case WM_MOUSEWHEEL:
		{
		}
		break;
	case WM_MOUSEMOVE:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);
			if( pt.x>SHOP_SKILL_START_X && 
				pt.x<SHOP_SKILL_START_X+SHOP_SKILL_BACK_SIZE_X &&
				pt.y>SHOP_SKILL_START_Y && 
				pt.y<SHOP_SKILL_START_Y+SHOP_SKILL_BACK_SIZE_Y)
			{
				int i = ( pt.x - SHOP_SKILL_START_X ) / SHOP_SKILL_INTERVAL_X;
				if( i>=0 && 
					i<CHAR_UTC_TYPE_NUMBER &&
					pt.x<SHOP_SKILL_START_X+SHOP_SKILL_INTERVAL_X*i+CHAR_UTC_ICON_SIZE)
				{
					int j = (pt.y - SHOP_SKILL_START_Y) / SHOP_SKILL_INTERVAL_Y;
					if( j >=0 && 
						j<CHAR_UTC_SLOT_NUMBER &&
						pt.y<SHOP_SKILL_START_Y+SHOP_SKILL_INTERVAL_Y*j+CHAR_UTC_ICON_SIZE &&
						m_pUTCDisplayInfo[i][j].pItem)
					{
						((CINFGameMain*)m_pParent)->SetItemInfo(m_pUTCDisplayInfo[i][j].pItem->UniqueNumber, 
																m_pUTCDisplayInfo[i][j].pItem->ItemNum, pt.x, pt.y);
					}
					else
						((CINFGameMain*)m_pParent)->SetItemInfo(0,0,0,0);
				}
				else
					((CINFGameMain*)m_pParent)->SetItemInfo(0,0,0,0);
			}
//			else
//			{
//				((CINFGameMain*)m_pParent)->SetItemInfo(0,0,0,0);
//			}
		}
		break;
	case WM_LBUTTONDOWN:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);
			if( pt.x>SHOP_SKILL_START_X && 
				pt.x<SHOP_SKILL_START_X+SHOP_SKILL_BACK_SIZE_X &&
				pt.y>SHOP_SKILL_START_Y && 
				pt.y<SHOP_SKILL_START_Y+SHOP_SKILL_BACK_SIZE_Y)
			{
				int i = ( pt.x - SHOP_SKILL_START_X ) / SHOP_SKILL_INTERVAL_X;
				if( i>=0 && 
					i<CHAR_UTC_TYPE_NUMBER &&
					pt.x<SHOP_SKILL_START_X+SHOP_SKILL_INTERVAL_X*i+CHAR_UTC_ICON_SIZE)
				{
					int j = (pt.y - SHOP_SKILL_START_Y) / SHOP_SKILL_INTERVAL_Y;
					if( j >=0 && 
						j<CHAR_UTC_SLOT_NUMBER &&
						pt.y<SHOP_SKILL_START_Y+SHOP_SKILL_INTERVAL_Y*j+CHAR_UTC_ICON_SIZE )
					{

						m_nRenderMoveIconIntervalWidth =  pt.x - (SHOP_SKILL_START_X+SHOP_SKILL_INTERVAL_X*i);
						m_nRenderMoveIconIntervalHeight = pt.y - (SHOP_SKILL_START_Y+SHOP_SKILL_INTERVAL_Y*j);

						if(	m_pUTCDisplayInfo[i][j].pItem &&
							m_pUTCDisplayInfo[i][j].pItem->ItemInfo->SkillType != SKILLTYPE_PERMANENT)	
						{
							SetSelectUTC( &m_pUTCDisplayInfo[i][j] );
						}
					}
				}
			}
		}
		break;
	case WM_LBUTTONUP:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);

			if( m_pSelectIcon &&
				m_pSelectSkill &&
				pt.x>SKILL_SHOP_SPI_START_X+1 && 
				pt.x<SKILL_SHOP_SPI_START_X+1+SKILL_DROP_SIZE &&
				pt.y>SKILL_SHOP_SPI_START_Y+1 && 
				pt.y<SKILL_SHOP_SPI_START_Y+1+SKILL_DROP_SIZE)
			{
				if( !((CINFGameMain*)m_pParent)->m_pInfWindow->IsExistMsgBox(_Q_ITEM_DELETE) &&
					!((CINFGameMain*)m_pParent)->m_pInfWindow->IsExistMsgBox(_Q_ITEM_DELETE_NUM))
				{
					g_pD3dApp->m_pChat->CreateChatChild(STRERR_C_SKILL_0012,COLOR_SKILL_USE);//"스킬 삭제는 아직 지원되지 않습니다."
//					char buf[256];
//					ITEM *pITEM = g_pDatabase->GetServerItemInfo(m_pSelectSkill->ItemNum);
//					if(pITEM)
//					{
//						m_pDeleteSkill = m_pSelectSkill;
//						wsprintf(buf, "스킬 %s 를(을)  삭제하시겠습니까?", pITEM->ItemName);
//						g_pGameMain->m_pInfWindow->AddMsgBox(buf, _Q_ITEM_DELETE_SKILL);
//					}
				}
			}
			SetSelectUTC(NULL);
			((CINFGameMain*)m_pParent)->SetItemInfo(0,0, 0,0);
		}
		break;

	}
	return INF_MSGPROC_NORMAL;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			CINFCharacterInfo::SendDeleteSelectItemSkill()
/// \brief		아직 작동되지 않음
/// \author		dhkwon
/// \date		2004-10-18 ~ 2004-10-18
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFCharacterInfo::SendDeleteSelectItemSkill()
{
	ASSERT_ASSERT(m_pDeleteSkill);
	MSG_FC_ITEM_THROW_AWAY_ITEM sMsg;
	sMsg.Amount = 1;
	sMsg.ClientIndex = g_pShuttleChild->m_myShuttleInfo.ClientIndex;
	sMsg.ItemUniqueNumber = m_pDeleteSkill->UniqueNumber;
	g_pFieldWinSocket->SendMsg( T_FC_ITEM_THROW_AWAY_ITEM, (char*)&sMsg, sizeof(sMsg) );
	m_pDeleteSkill = NULL;
}

int CINFCharacterInfo::UTCWndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	FLOG( "CINFCharacterInfo::UTCWndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)" );
	switch(uMsg)
	{
	case WM_MOUSEWHEEL:
		{
			POINT pt;
			GetCursorPos(&pt);
			ScreenToClient( g_pD3dApp->GetHwnd(), &pt );
			CheckMouseReverse(&pt);

			int nWindowPosY = g_pGameMain->m_nRightWindowY;
			if( pt.x>CHAR_UTC_START_X && pt.x<CHAR_UTC_START_X+CHAR_UTC_INTERVAL_X*CHAR_UTC_TYPE_NUMBER &&
				pt.y>CHAR_UTC_START_Y && pt.y<CHAR_UTC_START_Y+CHAR_UTC_INTERVAL_Y*CHAR_UTC_SLOT_NUMBER)
			{

				int nTemp = m_nCurrentUTCScroll;

				if((int)wParam > 0)
				{
					m_nCurrentUTCScroll--;
					if(m_nCurrentUTCScroll<0)
						m_nCurrentUTCScroll = 0;
					else if(m_nCurrentUTCScroll>GetUtcMaxScrollNumber())
						m_nCurrentUTCScroll = GetUtcMaxScrollNumber();
				}
				else
				{
					m_nCurrentUTCScroll++;
					if(m_nCurrentUTCScroll<0)
						m_nCurrentUTCScroll = 0;
					else if(m_nCurrentUTCScroll>GetUtcMaxScrollNumber())
						m_nCurrentUTCScroll = GetUtcMaxScrollNumber();
				}

				if(nTemp != m_nCurrentUTCScroll)
						SetAllUTCInfo();
				return INF_MSGPROC_BREAK;
			}
		}
		break;
	case WM_MOUSEMOVE:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);
			m_bLbuttonDown = FALSE;
			
			int nWindowPosY = g_pGameMain->m_nRightWindowY;
//			int nWindowPosY = POS_RIGHT_WINDOW_Y;
//			if(nWindowPosY < SIZE_ROLL_Y )
//				nWindowPosY = SIZE_ROLL_Y;
//			if(nWindowPosY > RIGHT_WINDOW_MAX_Y-SIZE_NORMAL_WINDOW_Y)
//				nWindowPosY = RIGHT_WINDOW_MAX_Y-SIZE_NORMAL_WINDOW_Y;
			if(m_bUTCScrollLock)
			{
				int nMove = (int)(pt.y-m_ptMouse.y);
				if(abs(nMove)>UTC_SCROLL_INTERVAL)
				{
					int nTemp = m_nCurrentUTCScroll;
					m_ptMouse = pt;
					int max = UTC_SCROLL_INTERVAL;
					if(max>0)
					{
						m_nCurrentUTCScroll += nMove/max;
						if(m_nCurrentUTCScroll<0)
							m_nCurrentUTCScroll = 0;
						else if(m_nCurrentUTCScroll>GetUtcMaxScrollNumber())
							m_nCurrentUTCScroll = GetUtcMaxScrollNumber();
						if(nTemp != m_nCurrentUTCScroll)
							SetAllUTCInfo();
					}
				}
			}
			if( pt.x>CHAR_UTC_START_X-20 && pt.x<CHAR_UTC_START_X+CHAR_UTC_INTERVAL_X*CHAR_UTC_TYPE_NUMBER+20 &&
				pt.y>CHAR_UTC_START_Y-20 && pt.y<CHAR_UTC_START_Y+CHAR_UTC_INTERVAL_Y*CHAR_UTC_SLOT_NUMBER+20)
			{
				int i = ( pt.x - CHAR_UTC_START_X ) / CHAR_UTC_INTERVAL_X;
				if( i>=0 && 
					i<CHAR_UTC_TYPE_NUMBER &&
					pt.x<CHAR_UTC_START_X+CHAR_UTC_INTERVAL_X*i+CHAR_UTC_ICON_SIZE)
				{
					int j = (pt.y - CHAR_UTC_START_Y) / CHAR_UTC_INTERVAL_Y;
					if( j >=0 && 
						j<CHAR_UTC_SLOT_NUMBER &&
						pt.y<CHAR_UTC_START_Y+CHAR_UTC_INTERVAL_Y*j+CHAR_UTC_ICON_SIZE &&
						m_pUTCDisplayInfo[i][j].pItem)
					{
						//((CINFGameMain*)m_pParent)->SetToolTip(pt.x, pt.y, m_pUTCDisplayInfo[i][j].Name );
						((CINFGameMain*)m_pParent)->SetItemInfo(m_pUTCDisplayInfo[i][j].pItem->UniqueNumber, 
																m_pUTCDisplayInfo[i][j].pItem->ItemNum, pt.x, pt.y);
					}
					else
						((CINFGameMain*)m_pParent)->SetItemInfo(0,0,0,0);
						//((CINFGameMain*)m_pParent)->SetToolTip(0,0,NULL);
				}
				else
					((CINFGameMain*)m_pParent)->SetItemInfo(0,0,0,0);
					//((CINFGameMain*)m_pParent)->SetToolTip(0,0,NULL);
			}
			else
			{
				((CINFGameMain*)m_pParent)->SetItemInfo(0,0,0,0);
			}
				//((CINFGameMain*)m_pParent)->SetToolTip(0,0,NULL);
		}
		break;
	case WM_LBUTTONDOWN:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);
			int nWindowPosY = g_pGameMain->m_nRightWindowY;
//			int nWindowPosY = POS_RIGHT_WINDOW_Y;
//			if(nWindowPosY < SIZE_ROLL_Y )
//				nWindowPosY = SIZE_ROLL_Y;
//			if(nWindowPosY > RIGHT_WINDOW_MAX_Y-SIZE_NORMAL_WINDOW_Y)
//				nWindowPosY = RIGHT_WINDOW_MAX_Y-SIZE_NORMAL_WINDOW_Y;
			if( pt.x>CHAR_UTC_START_X && pt.x<CHAR_UTC_START_X+CHAR_UTC_INTERVAL_X*CHAR_UTC_TYPE_NUMBER &&
				pt.y>CHAR_UTC_START_Y && pt.y<CHAR_UTC_START_Y+CHAR_UTC_INTERVAL_Y*CHAR_UTC_SLOT_NUMBER)
			{
				int i = ( pt.x - CHAR_UTC_START_X ) / CHAR_UTC_INTERVAL_X;
				if( i>=0 && i<CHAR_UTC_TYPE_NUMBER &&
					pt.x<CHAR_UTC_START_X+CHAR_UTC_INTERVAL_X*i+CHAR_UTC_ICON_SIZE)
				{
					int j = (pt.y - CHAR_UTC_START_Y) / CHAR_UTC_INTERVAL_Y;
					if( j >=0 && j<CHAR_UTC_SLOT_NUMBER &&
						pt.y<CHAR_UTC_START_Y+CHAR_UTC_INTERVAL_Y*j+CHAR_UTC_ICON_SIZE)
					{

						m_nRenderMoveIconIntervalWidth =  pt.x - (CHAR_UTC_START_X+CHAR_UTC_INTERVAL_X*i);
						m_nRenderMoveIconIntervalHeight = pt.y - (CHAR_UTC_START_Y+CHAR_UTC_INTERVAL_Y*j);

						if(	m_pUTCDisplayInfo[i][j].pItem &&
							m_pUTCDisplayInfo[i][j].pItem->ItemInfo->SkillType != SKILLTYPE_PERMANENT)	
						{
							SetSelectUTC( &m_pUTCDisplayInfo[i][j] );
							m_bLbuttonDown = TRUE;
							//((CINFGameMain*)m_pParent)->SetItemInfo(m_pUTCDisplayInfo[i][j].pItem->UniqueNumber, m_pUTCDisplayInfo[i][j].pItem->ItemNum, pt.x, pt.y);
							return INF_MSGPROC_BREAK;
						}
					}
				}
			}
			if( pt.x>UTC_SCROLL_BUTTON_START_X && pt.x<UTC_SCROLL_BUTTON_START_X+UTC_SCROLL_BAR_SIZE_X &&
				pt.y>nWindowPosY+UTC_SCROLL_BAR_START_Y+m_nCurrentUTCScroll*UTC_SCROLL_INTERVAL && pt.y<nWindowPosY+UTC_SCROLL_BAR_START_Y+m_nCurrentUTCScroll*UTC_SCROLL_INTERVAL+UTC_SCROLL_BAR_SIZE_Y)
			{
				m_bUTCScrollLock =	TRUE;
				m_ptMouse = pt;

			}

			// 2005-08-02 by ispark
			// 창안에 마우스 클릭시 무효
			if(pt.y>nWindowPosY &&
				pt.y<nWindowPosY+SIZE_NORMAL_WINDOW_Y &&
				pt.x>POS_RIGHT_WINDOW_X &&
				pt.x<POS_RIGHT_WINDOW_X + SIZE_NORMAL_WINDOW_X)
			{
				return INF_MSGPROC_BREAK;
			}

			// 2006-07-27 by ispark, 선택 아이템을 가지고 있다면
			if(((CINFGameMain*)m_pParent)->m_stSelectItem.pSelectItem && 
				((CINFGameMain*)m_pParent)->m_stSelectItem.bySelectType == ITEM_SKILL_POS)
			{
				return INF_MSGPROC_BREAK;
			}
		}
		break;
	case WM_LBUTTONUP:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);
			//			POINT pt = MAKEPOINT(lParam);
			if(m_bLbuttonDown)
			{
				m_bLbuttonDown = FALSE;
			}
			else
			{
				// 2005-12-01 by ispark, 퀵슬롯에서 LBUTTONUP일 때
				if(((CINFGameMain*)m_pParent)->m_pQuickSlot->LButtonUpQuickSlot(pt))
				{
					// 퀵슬롯에서는 SelectItem 삭제 안함
					break;
				}
				else if(((CINFGameMain*)m_pParent)->m_stSelectItem.bySelectType == ITEM_SKILL_POS)
				{
					SetSelectUTC(NULL);
				}
			}

			((CINFGameMain*)m_pParent)->SetItemInfo(0,0, 0,0);

			m_bUTCScrollLock = FALSE;
		}
		break;
	case WM_LBUTTONDBLCLK:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);
			int nWindowPosY = g_pGameMain->m_nRightWindowY;
			if( pt.x>CHAR_UTC_START_X && pt.x<CHAR_UTC_START_X+CHAR_UTC_INTERVAL_X*CHAR_UTC_TYPE_NUMBER &&
				pt.y>CHAR_UTC_START_Y && pt.y<CHAR_UTC_START_Y+CHAR_UTC_INTERVAL_Y*CHAR_UTC_SLOT_NUMBER)
			{
				int i = ( pt.x - CHAR_UTC_START_X ) / CHAR_UTC_INTERVAL_X;
				if( i>=0 && i<CHAR_UTC_TYPE_NUMBER &&
					pt.x<CHAR_UTC_START_X+CHAR_UTC_INTERVAL_X*i+CHAR_UTC_ICON_SIZE)
				{
					int j = (pt.y - CHAR_UTC_START_Y) / CHAR_UTC_INTERVAL_Y;
					if( j >=0 && j<CHAR_UTC_SLOT_NUMBER &&
						pt.y<CHAR_UTC_START_Y+CHAR_UTC_INTERVAL_Y*j+CHAR_UTC_ICON_SIZE)
					{
						m_nRenderMoveIconIntervalWidth =  pt.x - (CHAR_UTC_START_X+CHAR_UTC_INTERVAL_X*i);
						m_nRenderMoveIconIntervalHeight = pt.y - (CHAR_UTC_START_Y+CHAR_UTC_INTERVAL_Y*j);

						if(	m_pUTCDisplayInfo[i][j].pItem &&
							m_pUTCDisplayInfo[i][j].pItem->ItemInfo->SkillType != SKILLTYPE_PERMANENT)	
						{
							SendUseSkill(m_pUTCDisplayInfo[i][j].pItem);	
						}
					}
				}
			}
			if( pt.x>UTC_SCROLL_BUTTON_START_X && pt.x<UTC_SCROLL_BUTTON_START_X+UTC_SCROLL_BAR_SIZE_X &&
				pt.y>nWindowPosY+UTC_SCROLL_BAR_START_Y+m_nCurrentUTCScroll*UTC_SCROLL_INTERVAL && pt.y<nWindowPosY+UTC_SCROLL_BAR_START_Y+m_nCurrentUTCScroll*UTC_SCROLL_INTERVAL+UTC_SCROLL_BAR_SIZE_Y)
			{
//				m_bUTCScrollLock =	TRUE;
				m_ptMouse = pt;
			}

		}
		break;
	}
	return INF_MSGPROC_NORMAL;
}
int CINFCharacterInfo::ArenaWndProc(UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_LBUTTONDOWN:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);
			int nWindowPosY = g_pGameMain->m_nRightWindowY;

			if(pt.y>nWindowPosY &&
				pt.y<nWindowPosY+SIZE_NORMAL_WINDOW_Y &&
				pt.x>POS_RIGHT_WINDOW_X &&
				pt.x<POS_RIGHT_WINDOW_X + SIZE_NORMAL_WINDOW_X)
			{
				return INF_MSGPROC_BREAK;
			}
		}
		break;
	}
	return INF_MSGPROC_NORMAL;

}
void CINFCharacterInfo::SetSelectUTC( INVEN_DISPLAY_INFO* pDisplayInfo )
{
	FLOG( "CINFCharacterInfo::SetSelectUTC( INVEN_DISPLAY_INFO* pDisplayInfo )" );
	POINT ptIcon;
	ptIcon.x = m_nRenderMoveIconIntervalWidth;
	ptIcon.y = m_nRenderMoveIconIntervalHeight;

	((CINFGameMain*)m_pParent)->SetSelectItem(pDisplayInfo, ptIcon, ITEM_SKILL_POS);

//	if(pDisplayInfo)
//	{
//		m_pSelectIcon = ((CINFGameMain*)m_pParent)->m_pIcon->FindIcon(pDisplayInfo->IconName);
//		m_pSelectSkill = (ITEM_SKILL*)pDisplayInfo->pItem;
//		g_pGameMain->m_pQuickSlot->m_pSelectItem = (ITEM_BASE*)m_pSelectSkill;
//	}
//	else
//	{
//		m_pSelectIcon = NULL;
//		m_pSelectSkill = NULL;
//	}

}


///////////////////////////////////////////////////////////////////////////////
/// \fn			SendUseSkill( ITEM_BASE* pSkill )
/// \brief		스킬을 사용한다.
/// \author		jschoi
/// \date		2004-10-04 ~ 2004-10-04
/// \warning	사용한 스킬이 토글형인 경우 토클 해제하는 부분이 있다.
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFCharacterInfo::SendUseSkill( ITEM_BASE* pSkill )
{
	FLOG( "CINFCharacterInfo::SendUseSkill( ITEM_BASE* pSkill )" );
	// 2006-07-28 by ispark, 캐릭터용 스킬
	if(g_pD3dApp->m_bCharacter &&
		!IS_BAZAAR_SKILL(pSkill->ItemInfo))
	{
		// 캐릭터용 스킬 아님
		return;
	}

	// 2006-07-30 by ispark, 스킬 종료시 메시지 박스로 허가 해야하는 스킬을 체크한다.
	if(!CheckUseSkillApprovalEnd(pSkill))
	{
		return;
	}

	if( pSkill->ItemInfo->SkillType == SKILLTYPE_PERMANENT )
	{
		ASSERT_ASSERT(pSkill->ItemInfo->SkillType != SKILLTYPE_PERMANENT);
		return;
	}

	BOOL bUse = TRUE;
	if(	pSkill->ItemInfo->SkillType == SKILLTYPE_TOGGLE )
	{
		CSkillInfo* pSkillInfo = g_pShuttleChild->m_pSkill->FindUsingSkillInfo(pSkill->ItemNum);
		if(pSkillInfo != NULL)
		{
//			if(	pSkillInfo->GetSkillState() == SKILL_STATE_WAIT_REATTACK ||
//				pSkillInfo->GetSkillState() == SKILL_STATE_RELEASE )
//			{
//				return;
//			}
			pSkillInfo->ChangeSkillState(SKILL_STATE_WAIT_REATTACK);
			g_pGameMain->m_pInfSkill->DeleteSkillInfo(pSkillInfo);
			g_pShuttleChild->m_pSkill->EraseUsingSkill(pSkillInfo);
			bUse = FALSE;
		}
	}

	if(bUse)
	{
		g_pShuttleChild->m_pSkill->UseSkill( (CSkillInfo*)pSkill );
	}
	else
	{
//		g_pShuttleChild->m_pSkill->ReleaseAllUsingToggleSkill();
		
//		(*itSkillInfo)->ChangeSkillState(SKILL_STATE_WAIT_REATTACK);
//		g_pGameMain->m_pInfSkill->DeleteSkillInfo(*itSkillInfo);
//		SafeDeleteSkillInfo(*itSkillInfo);
//		m_vecUsingSkill.erase(itSkillInfo);	


//		MSG_FC_SKILL_CANCEL_SKILL sMsg;
//		memset(&sMsg,0x00,sizeof(sMsg));
//		char buffer[SIZE_MAX_PACKET];
//		int nType = T_FC_SKILL_CANCEL_SKILL;
//		sMsg.SkillItemUniqueNumber = pSkill->UniqueNumber;
//		memcpy(buffer, &nType, SIZE_FIELD_TYPE_HEADER);
//		memcpy(buffer+SIZE_FIELD_TYPE_HEADER, &sMsg, sizeof(sMsg));
//		g_pD3dApp->m_pFieldWinSocket->Write(buffer, SIZE_FIELD_TYPE_HEADER + sizeof(sMsg));
		}

//	if(g_pD3dApp->m_pShuttleChild->m_bIsAir == TRUE && pSkill->ItemInfo->SkillType == 3)		// 공중에서 시즈모드를 사용할때 사용금지]
//	{
//	}
//	else if( pSkill->ItemInfo->SkillType == 3 ||				// 시즈 모드 이거나
//		(g_pShuttleChild->m_dwState == _NORMAL					// 일반상테에서.. 
//		&& g_pShuttleChild->m_nEventType != EVENT_GAME_END) )	// 스킬 사용 제한 - jschoi 
//	{
//		if(bUse)
//		{
//			g_pShuttleChild->m_pSkill->UseSkill( (CSkillInfo*)pSkill );
//			//		MSG_FC_SKILL_USE_SKILL sMsg;
//			//		memset(&sMsg,0x00,sizeof(sMsg));
//			//		char buffer[SIZE_MAX_PACKET];
//			//		int nType = T_FC_SKILL_USE_SKILL;
//			//		sMsg.SkillItemUniqueNumber = pSkill->UniqueNumber;
//			//		// target ???
//			//		sMsg.AttackIndex = g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.ClientIndex;
//			//		sMsg.TargetIndex = 0;
//			//		memcpy(buffer, &nType, SIZE_FIELD_TYPE_HEADER);
//			//		memcpy(buffer+SIZE_FIELD_TYPE_HEADER, &sMsg, sizeof(sMsg));
//			//		g_pD3dApp->m_pFieldWinSocket->Write(buffer, SIZE_FIELD_TYPE_HEADER + sizeof(sMsg));
//		}
//		else
//		{
//			MSG_FC_SKILL_CANCEL_SKILL sMsg;
//			memset(&sMsg,0x00,sizeof(sMsg));
//			char buffer[SIZE_MAX_PACKET];
//			int nType = T_FC_SKILL_CANCEL_SKILL;
//			sMsg.SkillItemUniqueNumber = pSkill->UniqueNumber;
//			memcpy(buffer, &nType, SIZE_FIELD_TYPE_HEADER);
//			memcpy(buffer+SIZE_FIELD_TYPE_HEADER, &sMsg, sizeof(sMsg));
//			g_pD3dApp->m_pFieldWinSocket->Write(buffer, SIZE_FIELD_TYPE_HEADER + sizeof(sMsg));
//		}
//	}
	
}

#define STAT_TOOLTIP_LEFT_START_X			18
#define STAT_TOOLTIP_RIGHT_START_X			123
#define STAT_TOOLTIP_ATTACK_START_Y			188
#define STAT_TOOLTIP_ENDURANCE_START_Y		188
#define STAT_TOOLTIP_FUEL_START_Y			206
#define STAT_TOOLTIP_SOUL_START_Y			206
#define STAT_TOOLTIP_DODGE_START_Y			224
#define STAT_TOOLTIP_DEFENSE_START_Y		224
#define STAT_TOOLTIP_SIZE_X					64
#define STAT_TOOLTIP_SIZE_Y					13

int CINFCharacterInfo::InfoWndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	FLOG( "CINFCharacterInfo::InfoWndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)" );
	switch(uMsg)
	{
	case WM_MOUSEMOVE:
		{
			int nWindowPosY = g_pGameMain->m_nRightWindowY;
//			m_bToolTipStat = FALSE;					// 2005-07-04 by ispark 자동 스탯 분배 삭제

			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);
			((CINFGameMain*)m_pParent)->SetToolTip(0,0,NULL);

			// 2008-06-20 by bhsohn EP3 옵션관련 처리
			m_pOpenBtn->OnMouseMove(pt);			
			// end 2008-06-20 by bhsohn EP3 옵션관련 처리
			
			// 2007-05-15 by bhsohn 기어 스탯 관련 처리
			BOOL bOverStat[MAX_STAT_SHUTTLEINFO];
			int nCnt = 0;
			for(nCnt = 0;nCnt < MAX_STAT_SHUTTLEINFO; nCnt++)
			{
				bOverStat[nCnt] = (m_nShowOverStat[nCnt] > 0);
			}			
				

			if( pt.y > nWindowPosY+STAT_TOOLTIP_ATTACK_START_Y &&
				pt.y < nWindowPosY+STAT_TOOLTIP_ATTACK_START_Y+STAT_TOOLTIP_SIZE_Y)
			{				
				if( pt.x > POS_RIGHT_WINDOW_X+STAT_TOOLTIP_LEFT_START_X &&
					pt.x < POS_RIGHT_WINDOW_X+STAT_TOOLTIP_LEFT_START_X+STAT_TOOLTIP_SIZE_X
					&& (FALSE == bOverStat[STAT_SHUTTLEINFO_ATTACK]))
				{
					g_pGameMain->SetToolTip( pt.x, pt.y, STRMSG_C_TOOLTIP_0001);//"공격시 데미지를 증가시켜 줍니다."
					return INF_MSGPROC_BREAK;
				}
				// 2007-05-15 by bhsohn 기어 스탯 관련 처리
				else if( pt.x > POS_RIGHT_WINDOW_X+STAT_TOOLTIP_LEFT_START_X &&
					pt.x < POS_RIGHT_WINDOW_X+STAT_TOOLTIP_LEFT_START_X+STAT_TOOLTIP_SIZE_X-OVER_STAT_CAP0_X
					&& (TRUE == bOverStat[STAT_SHUTTLEINFO_ATTACK]))
				{
					g_pGameMain->SetToolTip( pt.x, pt.y, STRMSG_C_TOOLTIP_0001);//"공격시 데미지를 증가시켜 줍니다."
					return INF_MSGPROC_BREAK;
				}
				else if( pt.x > POS_RIGHT_WINDOW_X+STAT_TOOLTIP_LEFT_START_X+STAT_TOOLTIP_SIZE_X &&
					pt.x < POS_RIGHT_WINDOW_X+STAT_TOOLTIP_LEFT_START_X+STAT_TOOLTIP_SIZE_X+STAT_TOOLTIP_SIZE_Y
					&& (FALSE == bOverStat[STAT_SHUTTLEINFO_ATTACK]))
				{
					char buf[64];
					const GEAR_STAT_CHANGE_VALUE* pStat = CAtumSJ::GetGearStatChangeValueStruct( g_pShuttleChild->m_myShuttleInfo.UnitKind );
					
					if(pStat && (g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.BonusStat>0))
					{
						sprintf( buf, STRMSG_C_TOOLTIP_0002, pStat->GearStat1.AttackPart);//"\\y공격 스탯 증가량: %d"
						g_pGameMain->SetToolTip( pt.x, pt.y, buf);
						return INF_MSGPROC_BREAK;
					}					
				}
				// 2007-05-15 by bhsohn 기어 스탯 관련 처리
				else if( pt.x > POS_RIGHT_WINDOW_X+STAT_TOOLTIP_LEFT_START_X+STAT_TOOLTIP_SIZE_X - OVER_STAT_CAP0_X &&
					pt.x < POS_RIGHT_WINDOW_X+STAT_TOOLTIP_LEFT_START_X+STAT_TOOLTIP_SIZE_X+STAT_TOOLTIP_SIZE_Y
					&& (TRUE == bOverStat[STAT_SHUTTLEINFO_ATTACK]))
				{				
					// 초과된 스탯
					char buf[64];
					sprintf( buf, STRMSG_C_070516_0201);//"\\y초과된 스탯"
					g_pGameMain->SetToolTip( pt.x, pt.y, buf);
					return INF_MSGPROC_BREAK;
				}
				else if( pt.x > POS_RIGHT_WINDOW_X+STAT_TOOLTIP_RIGHT_START_X &&
				pt.x < POS_RIGHT_WINDOW_X+STAT_TOOLTIP_RIGHT_START_X+STAT_TOOLTIP_SIZE_X
				&& (FALSE == bOverStat[STAT_SHUTTLEINFO_DEFENCE]))
				{
					g_pGameMain->SetToolTip( pt.x, pt.y, STRMSG_C_TOOLTIP_0003);//"맞는 데미지를 감소시켜 줍니다."
					return INF_MSGPROC_BREAK;
				}
				else if( pt.x > POS_RIGHT_WINDOW_X+STAT_TOOLTIP_RIGHT_START_X &&
				pt.x < POS_RIGHT_WINDOW_X+STAT_TOOLTIP_RIGHT_START_X+STAT_TOOLTIP_SIZE_X- OVER_STAT_CAP1_X
				&& (TRUE == bOverStat[STAT_SHUTTLEINFO_DEFENCE]))
				{
					g_pGameMain->SetToolTip( pt.x, pt.y, STRMSG_C_TOOLTIP_0003);//"맞는 데미지를 감소시켜 줍니다."
					return INF_MSGPROC_BREAK;
				}
				else if( pt.x > POS_RIGHT_WINDOW_X+STAT_TOOLTIP_RIGHT_START_X+STAT_TOOLTIP_SIZE_X &&
				pt.x < POS_RIGHT_WINDOW_X+STAT_TOOLTIP_RIGHT_START_X+STAT_TOOLTIP_SIZE_X+STAT_TOOLTIP_SIZE_Y 
					&& (FALSE == bOverStat[STAT_SHUTTLEINFO_DEFENCE]))
				{
					char buf[64];
					const GEAR_STAT_CHANGE_VALUE* pStat = CAtumSJ::GetGearStatChangeValueStruct( g_pShuttleChild->m_myShuttleInfo.UnitKind );

					// 2007-05-15 by bhsohn 기어 스탯 관련 처리
					if(pStat && (g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.BonusStat>0))
					{
						sprintf( buf, STRMSG_C_TOOLTIP_0004, pStat->GearStat1.DefensePart);//내구->방어//"\\y방어 스탯 증가량: %d"
						g_pGameMain->SetToolTip( pt.x, pt.y, buf);
						return INF_MSGPROC_BREAK;
					}					
				}
				else if( pt.x > POS_RIGHT_WINDOW_X+STAT_TOOLTIP_RIGHT_START_X+STAT_TOOLTIP_SIZE_X - OVER_STAT_CAP1_X &&
				pt.x < POS_RIGHT_WINDOW_X+STAT_TOOLTIP_RIGHT_START_X+STAT_TOOLTIP_SIZE_X+STAT_TOOLTIP_SIZE_Y 
					&& (TRUE == bOverStat[STAT_SHUTTLEINFO_DEFENCE]))
				{
					// 초과된 스탯
					char buf[64];
					sprintf( buf, STRMSG_C_070516_0201);//"\\y초과된 스탯"
					g_pGameMain->SetToolTip( pt.x, pt.y, buf);
					return INF_MSGPROC_BREAK;
				}
			}
			else if( pt.y > nWindowPosY+STAT_TOOLTIP_FUEL_START_Y &&
				pt.y < nWindowPosY+STAT_TOOLTIP_FUEL_START_Y+STAT_TOOLTIP_SIZE_Y)
			{
				if( pt.x > POS_RIGHT_WINDOW_X+STAT_TOOLTIP_LEFT_START_X &&
					pt.x < POS_RIGHT_WINDOW_X+STAT_TOOLTIP_LEFT_START_X+STAT_TOOLTIP_SIZE_X 
					&& (FALSE == bOverStat[STAT_SHUTTLEINFO_FUEL]))
				{
					g_pGameMain->SetToolTip( pt.x, pt.y, STRMSG_C_TOOLTIP_0005);//"연료의 양을 증가시켜 줍니다."
					return INF_MSGPROC_BREAK;
				}
				// 2007-05-15 by bhsohn 기어 스탯 관련 처리
				else if( pt.x > POS_RIGHT_WINDOW_X+STAT_TOOLTIP_LEFT_START_X &&
					pt.x < POS_RIGHT_WINDOW_X+STAT_TOOLTIP_LEFT_START_X+STAT_TOOLTIP_SIZE_X -OVER_STAT_CAP0_X
					&& (TRUE == bOverStat[STAT_SHUTTLEINFO_FUEL]))
				{
					g_pGameMain->SetToolTip( pt.x, pt.y, STRMSG_C_TOOLTIP_0005);//"연료의 양을 증가시켜 줍니다."
					return INF_MSGPROC_BREAK;
				}
				else if( pt.x > POS_RIGHT_WINDOW_X+STAT_TOOLTIP_LEFT_START_X+STAT_TOOLTIP_SIZE_X &&
					pt.x < POS_RIGHT_WINDOW_X+STAT_TOOLTIP_LEFT_START_X+STAT_TOOLTIP_SIZE_X+STAT_TOOLTIP_SIZE_Y 
					&& (FALSE == bOverStat[STAT_SHUTTLEINFO_FUEL]))
				{
					char buf[64];
					const GEAR_STAT_CHANGE_VALUE* pStat = CAtumSJ::GetGearStatChangeValueStruct( g_pShuttleChild->m_myShuttleInfo.UnitKind );					
					if(pStat && (g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.BonusStat>0))
					{
						sprintf( buf, STRMSG_C_TOOLTIP_0006, pStat->GearStat1.FuelPart);//"\\y연료 스탯 증가량: %d"
						g_pGameMain->SetToolTip( pt.x, pt.y, buf);
						return INF_MSGPROC_BREAK;
					}					
				}
				else if( pt.x > POS_RIGHT_WINDOW_X+STAT_TOOLTIP_LEFT_START_X+STAT_TOOLTIP_SIZE_X - OVER_STAT_CAP0_X &&
					pt.x < POS_RIGHT_WINDOW_X+STAT_TOOLTIP_LEFT_START_X+STAT_TOOLTIP_SIZE_X+STAT_TOOLTIP_SIZE_Y 
					&& (TRUE == bOverStat[STAT_SHUTTLEINFO_FUEL]))
				{
					// 초과된 스탯
					char buf[64];
					sprintf( buf, STRMSG_C_070516_0201);//"\\y초과된 스탯"
					g_pGameMain->SetToolTip( pt.x, pt.y, buf);
					return INF_MSGPROC_BREAK;
				}
				else if( pt.x > POS_RIGHT_WINDOW_X+STAT_TOOLTIP_RIGHT_START_X &&
					pt.x < POS_RIGHT_WINDOW_X+STAT_TOOLTIP_RIGHT_START_X+STAT_TOOLTIP_SIZE_X 
					&& (FALSE == bOverStat[STAT_SHUTTLEINFO_SOUL]))
				{
					g_pGameMain->SetToolTip( pt.x, pt.y, STRMSG_C_TOOLTIP_0007);//"스킬을 사용할 수 있는 스킬 포인트를 증가시켜 줍니다."
					return INF_MSGPROC_BREAK;
				}
				// 2007-05-15 by bhsohn 기어 스탯 관련 처리
				else if( pt.x > POS_RIGHT_WINDOW_X+STAT_TOOLTIP_RIGHT_START_X &&
					pt.x < POS_RIGHT_WINDOW_X+STAT_TOOLTIP_RIGHT_START_X+STAT_TOOLTIP_SIZE_X -OVER_STAT_CAP1_X
					&& (TRUE == bOverStat[STAT_SHUTTLEINFO_SOUL]))
				{
					g_pGameMain->SetToolTip( pt.x, pt.y, STRMSG_C_TOOLTIP_0007);//"스킬을 사용할 수 있는 스킬 포인트를 증가시켜 줍니다."
					return INF_MSGPROC_BREAK;
				}
				else if( pt.x > POS_RIGHT_WINDOW_X+STAT_TOOLTIP_RIGHT_START_X+STAT_TOOLTIP_SIZE_X &&
				pt.x < POS_RIGHT_WINDOW_X+STAT_TOOLTIP_RIGHT_START_X+STAT_TOOLTIP_SIZE_X+STAT_TOOLTIP_SIZE_Y 
				&& (FALSE == bOverStat[STAT_SHUTTLEINFO_SOUL]))
				{
					char buf[64];
					const GEAR_STAT_CHANGE_VALUE* pStat = CAtumSJ::GetGearStatChangeValueStruct( g_pShuttleChild->m_myShuttleInfo.UnitKind );

					// 2007-05-15 by bhsohn 기어 스탯 관련 처리
					if(pStat && (g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.BonusStat>0))
					{
						sprintf( buf, STRMSG_C_TOOLTIP_0008, pStat->GearStat1.SoulPart);//"\\y정신 스탯 증가량: %d"
						g_pGameMain->SetToolTip( pt.x, pt.y, buf);
						return INF_MSGPROC_BREAK;
					}					
				}
				else if( pt.x > POS_RIGHT_WINDOW_X+STAT_TOOLTIP_RIGHT_START_X+STAT_TOOLTIP_SIZE_X - OVER_STAT_CAP1_X &&
				pt.x < POS_RIGHT_WINDOW_X+STAT_TOOLTIP_RIGHT_START_X+STAT_TOOLTIP_SIZE_X+STAT_TOOLTIP_SIZE_Y 
				&& (TRUE == bOverStat[STAT_SHUTTLEINFO_SOUL]))
				{
					// 초과된 스탯
					char buf[64];
					sprintf( buf, STRMSG_C_070516_0201);//"\\y초과된 스탯"
					g_pGameMain->SetToolTip( pt.x, pt.y, buf);
					return INF_MSGPROC_BREAK;
				}
			}
			else if( pt.y > nWindowPosY+STAT_TOOLTIP_DODGE_START_Y &&
				pt.y < nWindowPosY+STAT_TOOLTIP_DODGE_START_Y+STAT_TOOLTIP_SIZE_Y)
			{
				if( pt.x > POS_RIGHT_WINDOW_X+STAT_TOOLTIP_LEFT_START_X &&
					pt.x < POS_RIGHT_WINDOW_X+STAT_TOOLTIP_LEFT_START_X+STAT_TOOLTIP_SIZE_X
					&& (FALSE == bOverStat[STAT_SHUTTLEINFO_DODGE]))
				{
					g_pGameMain->SetToolTip( pt.x, pt.y, STRMSG_C_TOOLTIP_0009);//"공격에 대한 회피를 증가시켜 줍니다."
					return INF_MSGPROC_BREAK;
				}
				// 2007-05-15 by bhsohn 기어 스탯 관련 처리
				else if( pt.x > POS_RIGHT_WINDOW_X+STAT_TOOLTIP_LEFT_START_X &&
					pt.x < POS_RIGHT_WINDOW_X+STAT_TOOLTIP_LEFT_START_X+STAT_TOOLTIP_SIZE_X - OVER_STAT_CAP0_X 
					&& (TRUE == bOverStat[STAT_SHUTTLEINFO_DODGE]))
				{
					g_pGameMain->SetToolTip( pt.x, pt.y, STRMSG_C_TOOLTIP_0009);//"공격에 대한 회피를 증가시켜 줍니다."
					return INF_MSGPROC_BREAK;
				}
				else if( pt.x > POS_RIGHT_WINDOW_X+STAT_TOOLTIP_LEFT_START_X+STAT_TOOLTIP_SIZE_X &&
					pt.x < POS_RIGHT_WINDOW_X+STAT_TOOLTIP_LEFT_START_X+STAT_TOOLTIP_SIZE_X+STAT_TOOLTIP_SIZE_Y
					&& (FALSE == bOverStat[STAT_SHUTTLEINFO_DODGE]))
				{ 
					char buf[64];
					const GEAR_STAT_CHANGE_VALUE* pStat = CAtumSJ::GetGearStatChangeValueStruct( g_pShuttleChild->m_myShuttleInfo.UnitKind );
					
					if(pStat && (g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.BonusStat>0))
					{
						sprintf( buf, STRMSG_C_TOOLTIP_0010, pStat->GearStat1.DodgePart);//"\\y회피 스탯 증가량: %d"
						g_pGameMain->SetToolTip( pt.x, pt.y, buf);
						return INF_MSGPROC_BREAK;
					}				
				}
				else if( pt.x > POS_RIGHT_WINDOW_X+STAT_TOOLTIP_LEFT_START_X+STAT_TOOLTIP_SIZE_X - OVER_STAT_CAP0_X &&
					pt.x < POS_RIGHT_WINDOW_X+STAT_TOOLTIP_LEFT_START_X+STAT_TOOLTIP_SIZE_X+STAT_TOOLTIP_SIZE_Y
					&& (TRUE == bOverStat[STAT_SHUTTLEINFO_DODGE]))
				{
					// 초과된 스탯
					char buf[64];
					sprintf( buf, STRMSG_C_070516_0201);//"\\y초과된 스탯"
					g_pGameMain->SetToolTip( pt.x, pt.y, buf);
					return INF_MSGPROC_BREAK;
				}
				else if( pt.x > POS_RIGHT_WINDOW_X+STAT_TOOLTIP_RIGHT_START_X &&
				pt.x < POS_RIGHT_WINDOW_X+STAT_TOOLTIP_RIGHT_START_X+STAT_TOOLTIP_SIZE_X 
				&& (FALSE == bOverStat[STAT_SHUTTLEINFO_SHIELD]))
				{
					g_pGameMain->SetToolTip( pt.x, pt.y, STRMSG_C_TOOLTIP_0011);//"쉴드의 양을 증가시켜 줍니다."
					return INF_MSGPROC_BREAK;
				}
				else if( pt.x > POS_RIGHT_WINDOW_X+STAT_TOOLTIP_RIGHT_START_X &&
				pt.x < POS_RIGHT_WINDOW_X+STAT_TOOLTIP_RIGHT_START_X+STAT_TOOLTIP_SIZE_X - OVER_STAT_CAP1_X
				&& (TRUE == bOverStat[STAT_SHUTTLEINFO_SHIELD]))
				{
					g_pGameMain->SetToolTip( pt.x, pt.y, STRMSG_C_TOOLTIP_0011);//"쉴드의 양을 증가시켜 줍니다."
					return INF_MSGPROC_BREAK;
				}
				else if( pt.x > POS_RIGHT_WINDOW_X+STAT_TOOLTIP_RIGHT_START_X+STAT_TOOLTIP_SIZE_X &&
				pt.x < POS_RIGHT_WINDOW_X+STAT_TOOLTIP_RIGHT_START_X+STAT_TOOLTIP_SIZE_X+STAT_TOOLTIP_SIZE_Y 
				&& (FALSE == bOverStat[STAT_SHUTTLEINFO_SHIELD]))
				{
					char buf[64];
					const GEAR_STAT_CHANGE_VALUE* pStat = CAtumSJ::GetGearStatChangeValueStruct( g_pShuttleChild->m_myShuttleInfo.UnitKind );
					
					if(pStat && (g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.BonusStat>0))
					{
						sprintf( buf, STRMSG_C_TOOLTIP_0012, pStat->GearStat1.ShieldPart); //방어->쉴드//"\\y쉴드 스탯 증가량: %d"
						g_pGameMain->SetToolTip( pt.x, pt.y, buf);
						return INF_MSGPROC_BREAK;
					}					
				}
				else if( pt.x > POS_RIGHT_WINDOW_X+STAT_TOOLTIP_RIGHT_START_X+STAT_TOOLTIP_SIZE_X - OVER_STAT_CAP1_X&&
				pt.x < POS_RIGHT_WINDOW_X+STAT_TOOLTIP_RIGHT_START_X+STAT_TOOLTIP_SIZE_X+STAT_TOOLTIP_SIZE_Y 
				&& (TRUE == bOverStat[STAT_SHUTTLEINFO_SHIELD]))
				{
					// 초과된 스탯
					char buf[64];
					sprintf( buf, STRMSG_C_070516_0201);//"\\y초과된 스탯"
					g_pGameMain->SetToolTip( pt.x, pt.y, buf);
					return INF_MSGPROC_BREAK;
				}
			}
			// 2005-07-04 by ispark
			// 자동 스탯 분배 삭제
//			else if( pt.y > nWindowPosY + PROPENSITY_START_Y+64 &&
//					 pt.y < nWindowPosY + PROPENSITY_START_Y+84 &&
//					 pt.x > POS_RIGHT_WINDOW_X+GUILD_START_X-32 &&
//					 pt.x < POS_RIGHT_WINDOW_X+GUILD_START_X+10)
//			{
//				m_bToolTipStat = TRUE;
//			}
/*			else if( pt.y > nWindowPosY+STAT_TOOLTIP_ENDURANCE_START_Y &&
				pt.y < nWindowPosY+STAT_TOOLTIP_ENDURANCE_START_Y+STAT_TOOLTIP_SIZE_Y)
			{
			}
			else if( pt.y > nWindowPosY+STAT_TOOLTIP_SOUL_START_Y &&
				pt.y < nWindowPosY+STAT_TOOLTIP_SOUL_START_Y+STAT_TOOLTIP_SIZE_Y)
			{
			}
			else if( pt.y > nWindowPosY+STAT_TOOLTIP_DEFENSE_START_Y &&
				pt.y < nWindowPosY+STAT_TOOLTIP_DEFENSE_START_Y+STAT_TOOLTIP_SIZE_Y)
			{
			}
*/
		}
		break;
	case WM_LBUTTONDOWN:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);
			int nWindowPosY = g_pGameMain->m_nRightWindowY;
//			int nWindowPosY = POS_RIGHT_WINDOW_Y;
//			if(nWindowPosY < SIZE_ROLL_Y )
//				nWindowPosY = SIZE_ROLL_Y;
//			if(nWindowPosY > RIGHT_WINDOW_MAX_Y-SIZE_NORMAL_WINDOW_Y)
//				nWindowPosY = RIGHT_WINDOW_MAX_Y-SIZE_NORMAL_WINDOW_Y;
			if(g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.BonusStat > 0)
			{
				for(int j=0 ; j<3 ; j++)
				{
					for(int i=0 ; i<2 ;i++)
					{
						if( pt.x>(POS_RIGHT_WINDOW_X+82)+(i*105)&&
							pt.x<(POS_RIGHT_WINDOW_X+82)+(i*105)+10&&
							pt.y>nWindowPosY+(188+j*18) && pt.y<nWindowPosY+(188+j*18)+10 )
						{
							// 2007-05-15 by bhsohn 기어 스탯 관련 처리
							int nPreButtonState, nPreStatType;
							nPreButtonState = m_nButtonState;
							nPreStatType = m_nStatType;
							// end 2007-05-15 by bhsohn 기어 스탯 관련 처리
							
							m_nButtonState = STAT_BUTTON_DOWN ;
							m_nStatType = (j*2)+i ;
							if(g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.BonusStat>0)
							{
								// 2007-05-15 by bhsohn 기어 스탯 관련 처리
								int nShowBtn = -1;

								MSG_FC_CHARACTER_USE_BONUSSTAT sMsg;
								memset(&sMsg,0x00,sizeof(sMsg));
								char buffer[SIZE_MAX_PACKET];
								sMsg.ClientIndex = g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.ClientIndex;
								switch(m_nStatType)
								{
								case STAT_ATTACK:
									{
										sMsg.KindOfStat = STAT_ATTACK_PART;
										nShowBtn= STAT_SHUTTLEINFO_ATTACK;
									}
									break;
								case STAT_DURABILITY:
									{
										sMsg.KindOfStat = STAT_DEFENSE_PART;
										nShowBtn= STAT_SHUTTLEINFO_DEFENCE;
									}
									break;
								case STAT_FUEL:
									{
										sMsg.KindOfStat = STAT_FUEL_PART;
										nShowBtn= STAT_SHUTTLEINFO_FUEL;
									}
									break;
								case STAT_INDUCTION:
									{
										sMsg.KindOfStat = STAT_SOUL_PART;
										nShowBtn= STAT_SHUTTLEINFO_SOUL;
									}
									break;
								case STAT_AVOID:
									{
										sMsg.KindOfStat = STAT_DODGE_PART;
										nShowBtn= STAT_SHUTTLEINFO_DODGE;
									}
									break;
								case STAT_KALISMA:
									{
										sMsg.KindOfStat = STAT_SHIELD_PART;
										nShowBtn= STAT_SHUTTLEINFO_SHIELD;
									}
									break;
								}
								// 2007-05-15 by bhsohn 기어 스탯 관련 처리
								BOOL bSendPacket = FALSE;
								if(nShowBtn >= 0)
								{
									if(m_bStatShow[nShowBtn])
									{
										int nType = T_FC_CHARACTER_USE_BONUSSTAT;
										memcpy(buffer, &nType, SIZE_FIELD_TYPE_HEADER);
										memcpy(buffer+SIZE_FIELD_TYPE_HEADER, &sMsg, sizeof(sMsg));
										g_pD3dApp->m_pFieldWinSocket->Write(buffer, SIZE_FIELD_TYPE_HEADER + sizeof(sMsg));
										g_pD3dApp->m_bRequestEnable = FALSE;			// 2006-09-18 by ispark, 메세지 응답을 기다린다.

										bSendPacket = TRUE;
									}									
								}
								if(FALSE == bSendPacket)
								{
									// 못 찾았다.
									m_nButtonState = nPreButtonState;
									m_nStatType = nPreStatType;
								}
							}
						}
					}
				}
			}
			// 2008-06-20 by bhsohn EP3 옵션관련 처리
			{
				if(TRUE == m_pOpenBtn->OnLButtonDown(pt))
				{					
					return  INF_MSGPROC_BREAK;
				}		
			}
			// end 2008-06-20 by bhsohn EP3 옵션관련 처리

			// 2005-08-02 by ispark
			// 창안에 마우스 클릭시 무효
			if(pt.y>nWindowPosY &&
				pt.y<nWindowPosY+SIZE_NORMAL_WINDOW_Y &&
				pt.x>POS_RIGHT_WINDOW_X &&
				pt.x<POS_RIGHT_WINDOW_X + SIZE_NORMAL_WINDOW_X)
			{
				return INF_MSGPROC_BREAK;
			}

		}
		break;
	case WM_LBUTTONUP:
		{
//			POINT pt = MAKEPOINT(lParam);
			m_nButtonState = STAT_BUTTON_UP ;
			m_nStatType = STAT_NONE ;

			// 2008-06-20 by bhsohn EP3 옵션관련 처리
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);

			{
				if(TRUE == m_pOpenBtn->OnLButtonUp(pt))
				{			
					// 버튼위에 마우스가 있다.					
					g_pGameMain->m_pCommunity->OnPopupUserOpenSetup(TRUE);
					g_pD3dApp->m_pSound->PlayD3DSound(SOUND_SELECT_BUTTON, D3DXVECTOR3(0,0,0), FALSE);											
					return  INF_MSGPROC_BREAK;				
				}
			}
			// end 2008-06-20 by bhsohn EP3 옵션관련 처리

		}
		break;
	}
	return INF_MSGPROC_NORMAL;
}

void CINFCharacterInfo::ShowStatInfo()
{
	FLOG( "CINFCharacterInfo::ShowStatInfo()" );
	int nWindowPosY = g_pGameMain->m_nRightWindowY;
//	int nWindowPosY = POS_RIGHT_WINDOW_Y;
//	if(nWindowPosY < SIZE_ROLL_Y )
//		nWindowPosY = SIZE_ROLL_Y;
//	if(nWindowPosY > RIGHT_WINDOW_MAX_Y-SIZE_NORMAL_WINDOW_Y)
//		nWindowPosY = RIGHT_WINDOW_MAX_Y-SIZE_NORMAL_WINDOW_Y;
	//stat

	char	buff[30] ;
	CShuttleChild* pShuttle = g_pD3dApp->m_pShuttleChild;

	// 2007-05-15 by bhsohn 기어 스탯 관련 처리
	if(pShuttle)
	{
		// X좌표
		FLOAT fAllPosX[MAX_STAT_SHUTTLEINFO] =
		{
				POS_RIGHT_WINDOW_X+STATINFO_START_X0,		// 공격
				POS_RIGHT_WINDOW_X+STATINFO_START_X1,		// 방어
				POS_RIGHT_WINDOW_X+STATINFO_START_X0,		// 연료
				POS_RIGHT_WINDOW_X+STATINFO_START_X1,		// 정신
				POS_RIGHT_WINDOW_X+STATINFO_START_X0,		// 회피
				POS_RIGHT_WINDOW_X+STATINFO_START_X1// 쉴드
		};
		// Y좌표
		FLOAT fAllPosY[MAX_STAT_SHUTTLEINFO] =
		{
				nWindowPosY+ SHOWSTATINFO_Y1,				// 공격
				nWindowPosY+ SHOWSTATINFO_Y1,				// 방어	
				nWindowPosY+ SHOWSTATINFO_Y2,				// 연료
				nWindowPosY+ SHOWSTATINFO_Y2,				// 정신
				nWindowPosY+ SHOWSTATINFO_Y3,				// 회피
				nWindowPosY+ SHOWSTATINFO_Y3				// 쉴드
		};	

		FLOAT fOverStatPosX[MAX_STAT_SHUTTLEINFO] =
		{
				POS_RIGHT_WINDOW_X+STATINFO_START_X0-OVER_STAT_CAP0_X,		// 공격
				POS_RIGHT_WINDOW_X+STATINFO_START_X1-OVER_STAT_CAP1_X,		// 방어
				POS_RIGHT_WINDOW_X+STATINFO_START_X0-OVER_STAT_CAP0_X,		// 연료
				POS_RIGHT_WINDOW_X+STATINFO_START_X1-OVER_STAT_CAP1_X,		// 정신
				POS_RIGHT_WINDOW_X+STATINFO_START_X0-OVER_STAT_CAP0_X,		// 회피
				POS_RIGHT_WINDOW_X+STATINFO_START_X1-OVER_STAT_CAP1_X		// 쉴드
		};
		FLOAT fPosX, fPosY;		
		char szOvrbuff[256] ;
		wsprintf(szOvrbuff, "");

		fPosX = fPosY = 0;
		
		// 공격 
		{			
			fPosX = fAllPosX[STAT_SHUTTLEINFO_ATTACK];
			fPosY = fAllPosY[STAT_SHUTTLEINFO_ATTACK];		
			if(m_nShowOverStat[STAT_SHUTTLEINFO_ATTACK] > 0)
			{
				fPosX = fOverStatPosX[STAT_SHUTTLEINFO_ATTACK];
				wsprintf(szOvrbuff, "(+%d)", m_nShowOverStat[STAT_SHUTTLEINFO_ATTACK]);
				if(m_StatOverInfoFont)
				{				
					// 빨간색으로 그린다. 
					m_StatOverInfoFont->DrawText(fPosX+OVER_STAT_CAPX, fPosY, GUI_FONT_COLOR_R, szOvrbuff, 0 );
				}

				// 두꺼운 글씨로 그려준다.
				wsprintf(buff, "\\e%4d", (int)pShuttle->m_myShuttleInfo.TotalGearStat.AttackPart);//+(int)pShuttle->m_paramFactor.pfp_ATTACK_PART) ;
				if(m_StatInfoBoldFont)
				{
					m_StatInfoBoldFont->DrawText(fPosX, fPosY,GUI_FONT_COLOR, buff, 0 );
				}
				
			}
			else if(m_nShowOverStat[STAT_SHUTTLEINFO_ATTACK] == 0)
			{
				// 두꺼운 글씨로 그려준다.
				wsprintf(buff, "\\e%4d", (int)pShuttle->m_myShuttleInfo.TotalGearStat.AttackPart);//+(int)pShuttle->m_paramFactor.pfp_ATTACK_PART) ;
				if(m_StatInfoBoldFont)
				{
					m_StatInfoBoldFont->DrawText(fPosX, fPosY,GUI_FONT_COLOR, buff, 0 );
				}
				
			}
			else
			{
				wsprintf(buff, "%4d", (int)pShuttle->m_myShuttleInfo.TotalGearStat.AttackPart);//+(int)pShuttle->m_paramFactor.pfp_ATTACK_PART) ;
				if(m_StatInfoFont[0])
				{
					m_StatInfoFont[0]->DrawText(fPosX, fPosY,GUI_FONT_COLOR, buff, 0 );
				}
			}			
		}

		// 방어
		{			
			fPosX = fAllPosX[STAT_SHUTTLEINFO_DEFENCE];
			fPosY = fAllPosY[STAT_SHUTTLEINFO_DEFENCE];		
			if(m_nShowOverStat[STAT_SHUTTLEINFO_DEFENCE] > 0)
			{
				fPosX = fOverStatPosX[STAT_SHUTTLEINFO_DEFENCE];
				wsprintf(szOvrbuff, "(+%d)", m_nShowOverStat[STAT_SHUTTLEINFO_DEFENCE]);
				if(m_StatOverInfoFont)
				{				
					// 빨간색으로 그린다. 
					m_StatOverInfoFont->DrawText(fPosX+OVER_STAT_CAPX, fPosY, GUI_FONT_COLOR_R, szOvrbuff, 0 );
				}
				wsprintf(buff, "\\e%4d", (int)g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.TotalGearStat.DefensePart);//+(int)pShuttle->m_paramFactor.pfp_DEFENSE_PART) ;
				if(m_StatInfoBoldFont)
				{
					m_StatInfoBoldFont->DrawText(fPosX, fPosY,GUI_FONT_COLOR, buff, 0 );
				}
			}
			else if(m_nShowOverStat[STAT_SHUTTLEINFO_DEFENCE] == 0)
			{
				wsprintf(buff, "\\e%4d", (int)g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.TotalGearStat.DefensePart);//+(int)pShuttle->m_paramFactor.pfp_DEFENSE_PART) ;
				if(m_StatInfoBoldFont)
				{
					m_StatInfoBoldFont->DrawText(fPosX, fPosY,GUI_FONT_COLOR, buff, 0 );
				}
			}
			else
			{
				wsprintf(buff, "%4d", (int)g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.TotalGearStat.DefensePart);//+(int)pShuttle->m_paramFactor.pfp_DEFENSE_PART) ;
				if(m_StatInfoFont[1])
				{
					m_StatInfoFont[1]->DrawText(fPosX, fPosY,GUI_FONT_COLOR, buff, 0 );
				}
			}			
		}

		// 연료
		{			
			fPosX = fAllPosX[STAT_SHUTTLEINFO_FUEL];
			fPosY = fAllPosY[STAT_SHUTTLEINFO_FUEL];		
			if(m_nShowOverStat[STAT_SHUTTLEINFO_FUEL] > 0)
			{
				fPosX = fOverStatPosX[STAT_SHUTTLEINFO_FUEL];
				wsprintf(szOvrbuff, "(+%d)", m_nShowOverStat[STAT_SHUTTLEINFO_FUEL]);
				if(m_StatOverInfoFont)
				{				
					// 빨간색으로 그린다. 
					m_StatOverInfoFont->DrawText(fPosX+OVER_STAT_CAPX, fPosY, GUI_FONT_COLOR_R, szOvrbuff, 0 );
				}
				wsprintf(buff, "\\e%4d", (int)g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.TotalGearStat.FuelPart);//+(int)pShuttle->m_paramFactor.pfp_FUEL_PART) ;
				if(m_StatInfoBoldFont)
				{
					m_StatInfoBoldFont->DrawText(fPosX, fPosY,GUI_FONT_COLOR, buff, 0 );
				}
			}
			else if(m_nShowOverStat[STAT_SHUTTLEINFO_FUEL] == 0)
			{
				wsprintf(buff, "\\e%4d", (int)g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.TotalGearStat.FuelPart);//+(int)pShuttle->m_paramFactor.pfp_FUEL_PART) ;
				if(m_StatInfoBoldFont)
				{
					m_StatInfoBoldFont->DrawText(fPosX, fPosY,GUI_FONT_COLOR, buff, 0 );
				}
			}
			else
			{
				wsprintf(buff, "%4d", (int)g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.TotalGearStat.FuelPart);//+(int)pShuttle->m_paramFactor.pfp_FUEL_PART) ;
				if(m_StatInfoFont[2])
				{
					m_StatInfoFont[2]->DrawText(fPosX, fPosY,GUI_FONT_COLOR, buff, 0 );
				}
			}
			
		}

		// 정신
		{			
			fPosX = fAllPosX[STAT_SHUTTLEINFO_SOUL];
			fPosY = fAllPosY[STAT_SHUTTLEINFO_SOUL];		
			if(m_nShowOverStat[STAT_SHUTTLEINFO_SOUL] > 0)
			{
				fPosX = fOverStatPosX[STAT_SHUTTLEINFO_SOUL];
				wsprintf(szOvrbuff, "(+%d)", m_nShowOverStat[STAT_SHUTTLEINFO_SOUL]);
				if(m_StatOverInfoFont)
				{				
					// 빨간색으로 그린다. 
					m_StatOverInfoFont->DrawText(fPosX+OVER_STAT_CAPX, fPosY, GUI_FONT_COLOR_R, szOvrbuff, 0 );
				}
				wsprintf(buff, "\\e%4d", (int)g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.TotalGearStat.SoulPart);//+(int)pShuttle->m_paramFactor.pfp_SOUL_PART) ;
				if(m_StatInfoBoldFont)
				{
					m_StatInfoBoldFont->DrawText(fPosX, fPosY,GUI_FONT_COLOR, buff, 0 );
				}
			}
			else if(m_nShowOverStat[STAT_SHUTTLEINFO_SOUL] == 0)
			{
				wsprintf(buff, "\\e%4d", (int)g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.TotalGearStat.SoulPart);//+(int)pShuttle->m_paramFactor.pfp_SOUL_PART) ;
				if(m_StatInfoBoldFont)
				{
					m_StatInfoBoldFont->DrawText(fPosX, fPosY,GUI_FONT_COLOR, buff, 0 );
				}

			}
			else
			{
				wsprintf(buff, "%4d", (int)g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.TotalGearStat.SoulPart);//+(int)pShuttle->m_paramFactor.pfp_SOUL_PART) ;
				if(m_StatInfoFont[3])
				{
					m_StatInfoFont[3]->DrawText(fPosX, fPosY,GUI_FONT_COLOR, buff, 0 );
				}
			}			
		}

		// 회피
		{			
			fPosX = fAllPosX[STAT_SHUTTLEINFO_DODGE];
			fPosY = fAllPosY[STAT_SHUTTLEINFO_DODGE];		
			if(m_nShowOverStat[STAT_SHUTTLEINFO_DODGE] > 0)
			{
				fPosX = fOverStatPosX[STAT_SHUTTLEINFO_DODGE];
				wsprintf(szOvrbuff, "(+%d)", m_nShowOverStat[STAT_SHUTTLEINFO_DODGE]);
				if(m_StatOverInfoFont)
				{				
					// 빨간색으로 그린다. 
					m_StatOverInfoFont->DrawText(fPosX+OVER_STAT_CAPX, fPosY, GUI_FONT_COLOR_R, szOvrbuff, 0 );
				}
				wsprintf(buff, "\\e%4d", (int)g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.TotalGearStat.DodgePart);//+(int)pShuttle->m_paramFactor.pfp_DODGE_PART) ;
				if(m_StatInfoBoldFont)
				{
					m_StatInfoBoldFont->DrawText(fPosX, fPosY,GUI_FONT_COLOR, buff, 0 );
				}
			}
			else if(m_nShowOverStat[STAT_SHUTTLEINFO_DODGE] == 0)
			{
				wsprintf(buff, "\\e%4d", (int)g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.TotalGearStat.DodgePart);//+(int)pShuttle->m_paramFactor.pfp_DODGE_PART) ;
				if(m_StatInfoBoldFont)
				{
					m_StatInfoBoldFont->DrawText(fPosX, fPosY,GUI_FONT_COLOR, buff, 0 );
				}
			}
			else
			{
				wsprintf(buff, "%4d", (int)g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.TotalGearStat.DodgePart);//+(int)pShuttle->m_paramFactor.pfp_DODGE_PART) ;
				if(m_StatInfoFont[4])
				{
					m_StatInfoFont[4]->DrawText(fPosX, fPosY,GUI_FONT_COLOR, buff, 0 );
				}
			}
			
		}

		// 쉴드
		{			
			fPosX = fAllPosX[STAT_SHUTTLEINFO_SHIELD];
			fPosY = fAllPosY[STAT_SHUTTLEINFO_SHIELD];		
			if(m_nShowOverStat[STAT_SHUTTLEINFO_SHIELD] > 0)
			{
				fPosX = fOverStatPosX[STAT_SHUTTLEINFO_SHIELD];
				wsprintf(szOvrbuff, "(+%d)", m_nShowOverStat[STAT_SHUTTLEINFO_SHIELD]);
				if(m_StatOverInfoFont)
				{				
					// 빨간색으로 그린다. 
					m_StatOverInfoFont->DrawText(fPosX+OVER_STAT_CAPX, fPosY, GUI_FONT_COLOR_R, szOvrbuff, 0 );
				}
				wsprintf(buff, "\\e%4d", (int)g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.TotalGearStat.ShieldPart);//+(int)pShuttle->m_paramFactor.pfp_SHIELD_PART) ;
				if(m_StatInfoBoldFont)
				{
					m_StatInfoBoldFont->DrawText(fPosX, fPosY,GUI_FONT_COLOR, buff, 0 );
				}
			}
			else if(m_nShowOverStat[STAT_SHUTTLEINFO_SHIELD] == 0)
			{
				wsprintf(buff, "\\e%4d", (int)g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.TotalGearStat.ShieldPart);//+(int)pShuttle->m_paramFactor.pfp_SHIELD_PART) ;
				if(m_StatInfoBoldFont)
				{
					m_StatInfoBoldFont->DrawText(fPosX, fPosY,GUI_FONT_COLOR, buff, 0 );
				}
			}
			else
			{
				wsprintf(buff, "%4d", (int)g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.TotalGearStat.ShieldPart);//+(int)pShuttle->m_paramFactor.pfp_SHIELD_PART) ;
				if(m_StatInfoFont[5])
				{
					m_StatInfoFont[5]->DrawText(fPosX, fPosY,GUI_FONT_COLOR, buff, 0 );
				}
			}			
		}
		
		wsprintf(buff, "%4d", (int)g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.BonusStat) ;
		if(m_StatInfoFont[6])
			m_StatInfoFont[6]->DrawText(POS_RIGHT_WINDOW_X+STATINFO_START_X0, nWindowPosY+ SHOWSTATINFO_Y4,
			GUI_FONT_COLOR, buff, 0 );	
		
	}	
	// end 2007-05-15 by bhsohn 기어 스탯 관련 처리
	
	if(g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.BonusStat>0)
	{
		// 2007-05-15 by bhsohn 기어 스탯 관련 처리
		int nPos = 0;
				
		for(int j=0 ; j<3 ; j++)
		{
			for(int i=0 ;i<2 ;i++)
			{
				// 2007-05-15 by bhsohn 기어 스탯 관련 처리
//				m_pStat[STAT_BUTTON_UP]->Move((POS_RIGHT_WINDOW_X+82)+(i*105), nWindowPosY+(188+j*18));
//				m_pStat[STAT_BUTTON_UP]->Render() ;
				if(nPos >= MAX_STAT_SHUTTLEINFO)
				{
					continue;
				}
				if(m_bStatShow[nPos])
				{
					m_pStat[STAT_BUTTON_UP]->Move((POS_RIGHT_WINDOW_X+82)+(i*105), nWindowPosY+(188+j*18));
					m_pStat[STAT_BUTTON_UP]->Render() ;
				}				
				nPos++;
			}
		}

		if(m_nButtonState == STAT_BUTTON_DOWN )
		{
			// 2007-05-15 by bhsohn 기어 스탯 관련 처리
			if(m_nStatType == STAT_ATTACK && m_bStatShow[STAT_SHUTTLEINFO_ATTACK])
			{
				m_pStat[STAT_BUTTON_DOWN]->Move((POS_RIGHT_WINDOW_X+82), nWindowPosY+(188));
				m_pStat[STAT_BUTTON_DOWN]->Render() ;
			}
			if(m_nStatType == STAT_DURABILITY && m_bStatShow[STAT_SHUTTLEINFO_DEFENCE])
			{
				m_pStat[STAT_BUTTON_DOWN]->Move((POS_RIGHT_WINDOW_X+187), nWindowPosY+(188));
				m_pStat[STAT_BUTTON_DOWN]->Render() ;
			}
			if(m_nStatType == STAT_FUEL&& m_bStatShow[STAT_SHUTTLEINFO_FUEL])
			{
				m_pStat[STAT_BUTTON_DOWN]->Move((POS_RIGHT_WINDOW_X+82), nWindowPosY+206);
				m_pStat[STAT_BUTTON_DOWN]->Render() ;
			}
			if(m_nStatType == STAT_INDUCTION&& m_bStatShow[STAT_SHUTTLEINFO_SOUL])
			{
				m_pStat[STAT_BUTTON_DOWN]->Move((POS_RIGHT_WINDOW_X+187), nWindowPosY+206);
				m_pStat[STAT_BUTTON_DOWN]->Render() ;
			}
			
			if(m_nStatType == STAT_AVOID&& m_bStatShow[STAT_SHUTTLEINFO_DODGE])
			{
				m_pStat[STAT_BUTTON_DOWN]->Move((POS_RIGHT_WINDOW_X+82), nWindowPosY+224);
				m_pStat[STAT_BUTTON_DOWN]->Render() ;
			}
			
			if(m_nStatType == STAT_KALISMA&& m_bStatShow[STAT_SHUTTLEINFO_SHIELD])
			{
				m_pStat[STAT_BUTTON_DOWN]->Move((POS_RIGHT_WINDOW_X+187), nWindowPosY+224);
				m_pStat[STAT_BUTTON_DOWN]->Render() ;
			}
			// end 2007-05-15 by bhsohn 기어 스탯 관련 처리
		}
	}
}

int CINFCharacterInfo::GetUtcMaxScrollNumber()
{
	int Utc[4]={0,0,0,0};
//	map<int,ITEM_SKILL*>::iterator itSkill = g_pShuttleChild->m_pSkill->m_mapSkill.begin();
	map<int,CSkillInfo*>::iterator itSkillInfo = g_pShuttleChild->m_pSkill->m_mapSkill.begin();
	while(itSkillInfo != g_pShuttleChild->m_pSkill->m_mapSkill.end())
	{
		// 2013-03-19 by bhsohn 스킬 번호 구조 수정
// 		int nTemp = itSkillInfo->second->ItemNum / 1000;
// 		int nWindowNum = (itSkillInfo->second->ItemNum  % nTemp) / 10 -1;
		int nBaseNum = max(1, SKILL_BASE_NUM(itSkillInfo->second->ItemNum));
		int nTemp = max(1, nBaseNum / 1000);
		int nWindowNum = (nBaseNum  % nTemp) / 10 -1;		
		// END 2013-03-19 by bhsohn 스킬 번호 구조 수정
		// 2013-03-13 by bhsohn 스킬 번호 구조 수정
//		Utc[SKILL_KIND(itSkillInfo->second->ItemNum)] = max(Utc[SKILL_KIND(itSkillInfo->second->ItemNum)], nWindowNum) ;
		Utc[SKILL_KIND_INDEX(itSkillInfo->second->ItemNum)] = max(Utc[SKILL_KIND_INDEX(itSkillInfo->second->ItemNum)], nWindowNum) ;
		itSkillInfo++;
	}
	return max(max(Utc[0],Utc[1]),max(Utc[2],Utc[3]));
}

#define C_QUEST_DESC_TEXT_NUM		28
void CINFCharacterInfo::SetQuestDesc(int nQuestWindowDataIndex)
{
	// 2008-09-22 by bhsohn EP3 캐릭터 창
//	if(nQuestWindowDataIndex < 0)
//	{
//		m_pQuestListScrollBar->Reset();
//		m_pQuestDescScrollBar->Reset();
//		m_vecQuestDescription.clear();
//		return;
//	}
//	m_pQuestDescScrollBar->Reset();
//	m_vecQuestDescription.clear();
//	int index =0;
//	map<int, MEX_QUEST_INFO*>::iterator it = g_pQuestData->m_mapCharacterQuest.begin();
//	while(it != g_pQuestData->m_mapCharacterQuest.end() )
//	{
//		if(it->second->QuestState == QUEST_STATE_IN_PROGRESS)
//		{
//			if(index == nQuestWindowDataIndex)
//			{
//				break;
//			}
//			index ++;
//		}
//		it++;
//	}
//	if(it == g_pQuestData->m_mapCharacterQuest.end() )
//	{
//		return;
//	}
//	CQuest* pQuest = g_pQuestData->FindQuest( it->second->QuestIndex );
//	if(pQuest == NULL)
//	{
//		DBGOUT("CINFCharacterInfo::SetQuestDesc(%d) Can't Find Quest.\n", nQuestWindowDataIndex);
//		return;
//	}
//	if(strlen(pQuest->QuestDescription) == 0)
//	{
//		DBGOUT("CINFCharacterInfo::SetQuestDesc(%d) Quest (%d) description is empty.\n", nQuestWindowDataIndex, pQuest->QuestIndex);
//		return;
//	}
//	int i = 0;
//	int nPoint = 0;
//	int nCheckPoint = 0;
//	int nBreakPoint = 0;
//	int nLine = 0;
//	int nLineLength = C_QUEST_DESC_TEXT_NUM;
////	if(strlen(strNPCTalk) > CITY_NPCTALK_SIZE+CITY_NPCTALK_SIZE/2 ||
////		strlen(strNPCTalk) < CITY_NPCTALK_SIZE/3*2)
////	{
////		nLineLength = CITY_NPCTALK_SIZE-1;
////	}
////	else 
////	{
////		nLineLength = strlen(strNPCTalk)/3 * 2 - 1;
////	}
//	char szBuffer[MAX_PATH];
//	memset( szBuffer, 0x00, MAX_PATH );
//	while(TRUE)
//	{
//		if(pQuest->QuestDescription[i] == ' ' || 
//			pQuest->QuestDescription[i] == '.' || 
//			pQuest->QuestDescription[i] == '!' || 
//			pQuest->QuestDescription[i] == NULL)
//		{
//			if(nPoint >= nLineLength)
//			{
//				memcpy( szBuffer, pQuest->QuestDescription + nCheckPoint, nBreakPoint+1);
//				m_vecQuestDescription.push_back( (string)szBuffer );
//				memset( szBuffer, 0x00, MAX_PATH );
//				
//				nPoint -= nBreakPoint;
//				nCheckPoint += nBreakPoint+1;
//				nBreakPoint = nPoint-1;
//				nLine ++;
//				i++;
//				continue;
//			}
//			if(pQuest->QuestDescription[i] == NULL)
//			{
//				memcpy( szBuffer, pQuest->QuestDescription + nCheckPoint, nPoint);
//				m_vecQuestDescription.push_back( (string)szBuffer );
//				memset( szBuffer, 0x00, MAX_PATH );
//				break;
//			}
//			nBreakPoint = nPoint;
//		}
//		i++;
//		nPoint++;
//	}
//	if(pQuest->IsCityWar)
//	{
//		wsprintf( szBuffer, STRMSG_C_QUEST_0009, it->second->szCityWarServerGroupName );//"서버:%s"
//		m_vecQuestDescription.push_back( (string)szBuffer );
//	}
//	m_pQuestDescScrollBar->SetNumberOfData( m_vecQuestDescription.size() );
}
void CINFCharacterInfo::RenderNonQuest(int nWindowPosY, int nLine)
{
	// 퀘스트 벡터 업데이트
	g_pQuestData->NotifyStartQuest();
	int mScrollIndex	= m_pQuestListScrollBar->GetCurrentScrollIndex() - m_nMissionProCount;
	
	vector<Quest_Data*>::iterator it = g_pQuestData->m_vecQuest.begin();
	while(it != g_pQuestData->m_vecQuest.end())
	{		
		Quest_Data* pInfo = (*it);
		if(mScrollIndex <= 0 && nLine < C_QUEST_LIST_NUM)
		{
			m_pFontQuestList[nLine]->DrawText(C_QUEST_START_X,
				nWindowPosY+C_QUEST_START_Y+C_QUEST_INTERVAL*nLine+1,
				RGB(128,128,255),
				pInfo->szMissionName);
			/////////////////////////////////////////////////////////////////////////////////////////////
			// 설명
			if(m_pQuestListScrollBar->GetCurrentSelectWindowIndex() == nLine)
			{
				char buffChar[1024];
				memset(buffChar, 0x00, 1024);
				
				m_pFontQuestDesc[0]->DrawText(C_QUEST_DESC_START_X+1,
					nWindowPosY+C_QUEST_DESC_START_Y+0*C_QUEST_DESC_INTERVAL+1,
					GUI_FONT_COLOR_Y,
					STRMSG_C_QUEST_0010 );//"미션정보"

				CQuest* pQuest = g_pQuestData->FindQuest( pInfo->nIndex );
				if(pQuest->ReqLevel.Max > 100)
				{
					wsprintf(buffChar, STRMSG_C_QUEST_0011, pQuest->ReqLevel.Min);//"제한레벨: %d ~"
				}
				else
				{
					wsprintf(buffChar, STRMSG_C_QUEST_0012,pQuest->ReqLevel.Min, pQuest->ReqLevel.Max);//"제한레벨: %d ~ %d "
				}				
				m_pFontQuestDesc[2]->DrawText(C_QUEST_DESC_START_X+1,
					nWindowPosY+C_QUEST_DESC_START_Y+2*C_QUEST_DESC_INTERVAL+1,
					GUI_FONT_COLOR,
					buffChar );

				memset(buffChar, 0x00, 1024);
				wsprintf(buffChar, STRMSG_C_QUEST_0013, pInfo->szMAPName);//"맵:%s"
				m_pFontQuestDesc[3]->DrawText(C_QUEST_DESC_START_X+1,
					nWindowPosY+C_QUEST_DESC_START_Y+3*C_QUEST_DESC_INTERVAL+1,
					GUI_FONT_COLOR,
					buffChar );

				memset(buffChar, 0x00, 1024);
				wsprintf(buffChar, "NPC: %s", pInfo->szNPCName);
				m_pFontQuestDesc[4]->DrawText(C_QUEST_DESC_START_X+1,
					nWindowPosY+C_QUEST_DESC_START_Y+4*C_QUEST_DESC_INTERVAL+1,
					GUI_FONT_COLOR,
					buffChar );				
			}
			//
			///////////////////////////////////////////////////////////////////////////////////////////////	

			nLine++;			
		}		
		mScrollIndex--;
		it++;
	}
}	
		
void CINFCharacterInfo::RenderStatDescToolTip(int nType, int x, int y)
{
	char buff[512];
	ZERO_MEMORY(buff);
	
	switch(nType) {
	case AUTOSTAT_TYPE_FREESTYLE:			wsprintf(buff, STRMSG_C_TOOLTIP_0021);	break;// "원하는 형태로 조절가능한 스탯 형태"
	case AUTOSTAT_TYPE_BGEAR_ATTACK:		wsprintf(buff, STRMSG_C_TOOLTIP_0022);	break;// "공격력을 강화한 스탯 형태"
	case AUTOSTAT_TYPE_BGEAR_MULTI:			wsprintf(buff, STRMSG_C_TOOLTIP_0023);	break;// "다양한 멀티기능 스탯 형태"
	case AUTOSTAT_TYPE_IGEAR_ATTACK:		wsprintf(buff, STRMSG_C_TOOLTIP_0022);	break;// "공격력을 강화한 스탯 형태"
	case AUTOSTAT_TYPE_IGEAR_DODGE:			wsprintf(buff, STRMSG_C_TOOLTIP_0024);	break;// "회피력을 강화한 스탯 형태"
	case AUTOSTAT_TYPE_AGEAR_ATTACK:		wsprintf(buff, STRMSG_C_TOOLTIP_0022);	break;// "공격력을 강화한 스탯 형태"
	case AUTOSTAT_TYPE_AGEAR_SHIELD:		wsprintf(buff, STRMSG_C_TOOLTIP_0025);	break;// "생존력을 강화한 스탯 형태"
	case AUTOSTAT_TYPE_MGEAR_DEFENSE:		wsprintf(buff, STRMSG_C_TOOLTIP_0026);	break;// "방어력을 강화한 스탯 형태"
	case AUTOSTAT_TYPE_MGEAR_SUPPORT:		wsprintf(buff, STRMSG_C_TOOLTIP_0027);	break;// "편대의 지원능력을 강화한 스탯 형태"
	}

	g_pGameMain->RenderPopUpWindowImage(x, y, strlen(buff)*6.5, 1);
	m_pFontStatDec->DrawText(x, y, GUI_FONT_COLOR,buff);
}

int CINFCharacterInfo::AmmoWndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_LBUTTONDOWN:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);
			int nWindowPosY = g_pGameMain->m_nRightWindowY;

			if(pt.y>nWindowPosY &&
				pt.y<nWindowPosY+SIZE_NORMAL_WINDOW_Y &&
				pt.x>POS_RIGHT_WINDOW_X &&
				pt.x<POS_RIGHT_WINDOW_X + SIZE_NORMAL_WINDOW_X)
			{
				return INF_MSGPROC_BREAK;
			}
		}
		break;
	}
	return INF_MSGPROC_NORMAL;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CINFCharacterInfo::CheckUseSkillApprovalEnd(ITEM_BASE* pSkill)
/// \brief		사용중인 스킬 종료 허가 체크
/// \author		ispark
/// \date		2006-07-30 ~ 2006-07-30
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CINFCharacterInfo::CheckUseSkillApprovalEnd(ITEM_BASE* pSkill)
{
	CSkillInfo* pSkillInfo = g_pShuttleChild->m_pSkill->FindUsingSkillInfo(pSkill->ItemNum);
	if(pSkillInfo && pSkillInfo->GetbApprovalEnd() == FALSE)
	{
		// 스킬을 사용한 상점 오픈형
		if(g_pInterface->m_pBazaarShop &&
			pSkill->UniqueNumber == pSkillInfo->UniqueNumber &&
			IS_BAZAAR_SKILL(pSkillInfo->ItemInfo))
		{
			// 2006-08-01 by ispark, 시작한 상점이면
			if(g_pInterface->m_pBazaarShop->GetbBazaarShopStart())
			{
				char msgBox[256] = {0,};
				if(g_pInterface->m_pBazaarShop->GetbBazaarShopType() == 1)
				{
					wsprintf(msgBox, STRMSG_C_060730_0000, STRMSG_C_060730_0002);		// "%s 상점을 종료하시겠습니까?"
				}	
				else if(g_pInterface->m_pBazaarShop->GetbBazaarShopType() == 2)
				{
					wsprintf(msgBox, STRMSG_C_060730_0000, STRMSG_C_060730_0003);		// "%s 상점을 종료하시겠습니까?"
				}
				g_pGameMain->m_pInfWindow->AddMsgBox(msgBox, _Q_BAZAAR_OPEN_SHOP_END, (DWORD)pSkillInfo);

				return FALSE;
			}
		}
	}

	return TRUE;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		보일지 말지에 대한 결정
/// \author		// 2007-05-15 by bhsohn 기어 스탯 관련 처리
/// \date		2007-05-15 ~ 2007-05-15
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFCharacterInfo::GetStatShowInfo()
{	
	//(m_bStatShow, m_nShowOverStat)
	// 전체 스탯 
	GEAR_STAT stMyShuttleFullStatInfo = g_pD3dApp->m_pShuttleChild->GetMyShuttleFullStatInfo();	

	// 공격
	m_bStatShow[STAT_SHUTTLEINFO_ATTACK] = TRUE;
	m_nShowOverStat[STAT_SHUTTLEINFO_ATTACK] = stMyShuttleFullStatInfo.AttackPart - COUNT_MAX_STAT_POINT;		
	if(stMyShuttleFullStatInfo.AttackPart >= COUNT_MAX_STAT_POINT)
	{
		m_bStatShow[STAT_SHUTTLEINFO_ATTACK] = FALSE;				
	}	

	// 방어
	m_bStatShow[STAT_SHUTTLEINFO_DEFENCE] = TRUE;
	m_nShowOverStat[STAT_SHUTTLEINFO_DEFENCE] = stMyShuttleFullStatInfo.DefensePart - COUNT_MAX_STAT_POINT;		
	if(stMyShuttleFullStatInfo.DefensePart >= COUNT_MAX_STAT_POINT)
	{
		m_bStatShow[STAT_SHUTTLEINFO_DEFENCE] = FALSE;		
	}	

	// 연료
	m_bStatShow[STAT_SHUTTLEINFO_FUEL] = TRUE;
	m_nShowOverStat[STAT_SHUTTLEINFO_FUEL] = stMyShuttleFullStatInfo.FuelPart - COUNT_MAX_STAT_POINT;		
	if(stMyShuttleFullStatInfo.FuelPart >= COUNT_MAX_STAT_POINT)
	{
		m_bStatShow[STAT_SHUTTLEINFO_FUEL] = FALSE;		
	}	

	// 정신
	m_bStatShow[STAT_SHUTTLEINFO_SOUL] = TRUE;
	m_nShowOverStat[STAT_SHUTTLEINFO_SOUL] = stMyShuttleFullStatInfo.SoulPart - COUNT_MAX_STAT_POINT;		
	if(stMyShuttleFullStatInfo.SoulPart >= COUNT_MAX_STAT_POINT)
	{
		m_bStatShow[STAT_SHUTTLEINFO_SOUL] = FALSE;	
	}	

	// 회피
	m_bStatShow[STAT_SHUTTLEINFO_DODGE] = TRUE;
	m_nShowOverStat[STAT_SHUTTLEINFO_DODGE] = stMyShuttleFullStatInfo.DodgePart - COUNT_MAX_STAT_POINT;
	if(stMyShuttleFullStatInfo.DodgePart >= COUNT_MAX_STAT_POINT)
	{
		m_bStatShow[STAT_SHUTTLEINFO_DODGE] = FALSE;		
	}	
	
	// 쉴드
	m_bStatShow[STAT_SHUTTLEINFO_SHIELD] = TRUE;
	m_nShowOverStat[STAT_SHUTTLEINFO_SHIELD] = stMyShuttleFullStatInfo.ShieldPart - COUNT_MAX_STAT_POINT;		
	if(stMyShuttleFullStatInfo.ShieldPart >= COUNT_MAX_STAT_POINT)
	{
		m_bStatShow[STAT_SHUTTLEINFO_SHIELD] = FALSE;		
	}		


}
