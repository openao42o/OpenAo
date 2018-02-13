// INFMissionTreeInfo.cpp: implementation of the CINFMissionTreeInfo class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "AtumApplication.h"
#include "AtumParam.h"// 2008-12-09 by dgwoo 미션마스터.
#include "ShuttleChild.h"
#include "INFImage.h"
#include "GameDataLast.h"
#include "INFGameMain.h"
#include "D3DHanFont.h"
#include "AtumSound.h"
#include "QuestData.h"
#include "StoreData.h"
#include "ItemInfo.h"
#include "Interface.h"
#include "INFMissionMain.h"
#include "ClientParty.h"
#include "AtumDatabase.h"		// 2013-03-13 by bhsohn 스킬 번호 구조 수정

#include "INFMissionTreeInfo.h"
// 2008-12-09 by dgwoo 미션마스터.
#include "INFCommunity.h"
#include "INFcommuPartyCre.h"
#include "Chat.h"

// 2011. 10. 10 by jskim UI시스템 변경
#include "INFGroupManager.h"
#include "INFGroupImage.h"
#include "INFImageEx.h"
// end 2011. 10. 10 by jskim UI시스템 변경

////////////// 라디오 버튼 위치 ////////////// 
// 시나리오 미션
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
#define		TAB_MISSION_ALL_POS_X			142
#define		TAB_MISSION_ALL_POS_Y			413//638 // 2008-12-09 by dgwoo 미션마스터.

// 시나리오 미션
#define		TAB_MISSION_COMPLETE_POS_X		89
#define		TAB_MISSION_COMPLETE_POS_Y		614//638 // 2008-12-09 by dgwoo 미션마스터.

// 번ㅎ외 미션
#define		TAB_MISSION_NOTCOMPLETE_POS_X			177
#define		TAB_MISSION_NOTCOMPLETE_POS_Y			614//638 // 2008-12-09 by dgwoo 미션마스터.

#define		TAB_MISSION_CAP_X				5

////////////// 리스트 컨트롤 위치 ////////////// 
#define		LISTCTRL_MAIN_LEN				3
#define		LISTCTRL_POS_X					28
#define		LISTCTRL_POS_Y					86
#define		LISTCTRL_WIDTH					180
#define		LISTCTRL_HEIGHT					335
#define		LISTCTRL_SUB_POS_X				18
#define		LISTCTRL_SUB_POS_Y				5
#define		LISTCTRL_ONEPAGE_ITEMLEN		16

// 에디트 박스 
#define		EDITBOX_ONEPAGE_ITEMLEN			9
#define		EDITBOX_POS_X					243
#define		EDITBOX_POS_Y					40
#define		EDITBOX_WIDTH					430
#define		EDITBOX_HEIGHT					220
#define		EDITBOX_SCROLL_POS_X			447
#define		EDITBOX_SCROLL_POS_Y			47
#define		EDITBOX_SCROLL_WIDTH			11
#define		EDITBOX_SCROLL_HEIGHT			168
#define		EDITBOX_NORMALCAP_Y				20
#define		EDITBOX_HEADERCAP_X				10
#define		EDITBOX_HEADERCAP_Y				28

// 미션 보상 
#define		MISSION_COMPEN_X				263
#define		MISSION_COMPEN_Y				268
#define		MISSION_COMPEN_CAP_X			52
#define		MISSION_TOOLTIB_GAP_Y			32
#define		MISSION_TOOLTIB_Y				30
#define		MISSION_COMPEN_ONEITEM_WIDTH	27		// 한개의 아이템  
#define		MISSION_COMPEN_ONEITEM_HEIGHT	27


// 색상 설정
#define TREEMISSION_SELECT_COLOR				(RGB(194, 155, 0))
#define TREEMISSION_PROGRESS_COLOR				(RGB(59, 177, 255))
#define TREEMISSION_COMPLETE_COLOR				(RGB(215, 215, 45))
#define TREEMISSION_IMPOSIBLE_COLOR				(RGB(184, 36, 36))
#define TREEMISSION_POSIBLE_COLOR				(RGB(255, 255, 255))

// 에러 설정 정보 관련
#define TREEMISSION_ERR_NONE					0
#define TREEMISSION_ERR_LEVEL					100
#define TREEMISSION_ERR_STAT_AT					101
#define TREEMISSION_ERR_STAT_DE					102
#define TREEMISSION_ERR_STAT_FU					103
#define TREEMISSION_ERR_STAT_SO					104
#define TREEMISSION_ERR_STAT_SH					105
#define TREEMISSION_ERR_STAT_DO					106
#define TREEMISSION_ERR_ITEM					107
#define TREEMISSION_ERR_FLUENCE					108
#define TREEMISSION_ERR_EXPERIENCE				109
#define TREEMISSION_ERR_QUEST					110
#define TREEMISSION_ERR_HIDEQUEST				111
#define TREEMISSION_ERR_POSENDQUEST				112

// 에러 설정 정보 관련
#define TREEMISSION_ERR_NONE					0
#define TREEMISSION_ERR_LEVEL					100
#define TREEMISSION_ERR_STAT_AT					101
#define TREEMISSION_ERR_STAT_DE					102
#define TREEMISSION_ERR_STAT_FU					103
#define TREEMISSION_ERR_STAT_SO					104
#define TREEMISSION_ERR_STAT_SH					105
#define TREEMISSION_ERR_STAT_DO					106
#define TREEMISSION_ERR_ITEM					107
#define TREEMISSION_ERR_FLUENCE					108
#define TREEMISSION_ERR_EXPERIENCE				109
#define TREEMISSION_ERR_QUEST					110
#define TREEMISSION_ERR_HIDEQUEST				111
#define TREEMISSION_ERR_POSENDQUEST				112

// 미션 시작 버튼
#define	MISSION_START_POS_X						240
#define	MISSION_START_POS_Y						490

// 미션 맵으로 이동 버튼 
#define	MISSION_GO_POS_X						580
#define	MISSION_GO_POS_Y						400

#define MISSION_BUTTON_SIZE_X					115
#define MISSION_BUTTON_SIZE_Y					28

// 맵 버튼
#define	MAP_POS_X								253
#define	MAP_POS_Y								347
#define	MAP_POS_WIDTH							300
#define	MAP_POS_HEIGHT							92

// 닫기 창
#define	CLOSE_POS_X								708
#define	CLOSE_POS_Y								6

// 애니창
#define	ANI_POS_X								44
#define ANI_POS_Y								41
		

// SPI ID
#define	ITEM_NUM_SPI							7000022

// 2007-07-30 by bhsohn 일반, 번외 퀘스트 재정렬
#define	QUEST_RESORT_LEN						2

// 2008-12-02 by dgwoo 미션 마스터 시스템.
// 도움 요청 버튼.
#define MMASTER_HELPER_X					497
#define MMASTER_HELPER_Y					400

#define MMASTER_HELPER_MX					623
#define MMASTER_HELPER_MY					400

// 편대 생성 & 도움 요청 버튼.
#define MMASTER_HELPER_PARTY_X					438
#define MMASTER_HELPER_PARTY_Y					400

#define MMASTER_HELPER_PARTY_MX					12
#define MMASTER_HELPER_PARTY_MY					400

// 미션 마스터 등록 해제.
#define MMASTER_REG_X					12
#define MMASTER_REG_Y					452

#define MMASTER_BTN_DOWN_GAP			10.0f
// 2008-12-02 by dgwoo 미션 마스터 시스템.
#else
#define		TAB_MISSION_ALL_POS_X			26
#define		TAB_MISSION_ALL_POS_Y			614//638 // 2008-12-09 by dgwoo 미션마스터.

// 시나리오 미션
#define		TAB_MISSION_COMPLETE_POS_X		89
#define		TAB_MISSION_COMPLETE_POS_Y		614//638 // 2008-12-09 by dgwoo 미션마스터.

// 번ㅎ외 미션
#define		TAB_MISSION_NOTCOMPLETE_POS_X			177
#define		TAB_MISSION_NOTCOMPLETE_POS_Y			614//638 // 2008-12-09 by dgwoo 미션마스터.

#define		TAB_MISSION_CAP_X				5

////////////// 리스트 컨트롤 위치 ////////////// 
#define		LISTCTRL_MAIN_LEN				3
#define		LISTCTRL_POS_X					35
#define		LISTCTRL_POS_Y					252
#define		LISTCTRL_WIDTH					220
#define		LISTCTRL_HEIGHT					360
#define		LISTCTRL_SUB_POS_X				21
#define		LISTCTRL_SUB_POS_Y				5
#define		LISTCTRL_ONEPAGE_ITEMLEN		18

// 에디트 박스 
#define		EDITBOX_ONEPAGE_ITEMLEN			15
#define		EDITBOX_POS_X					300
#define		EDITBOX_POS_Y					52
#define		EDITBOX_WIDTH					460
#define		EDITBOX_HEIGHT					287
#define		EDITBOX_SCROLL_POS_X			492
#define		EDITBOX_SCROLL_POS_Y			58
#define		EDITBOX_SCROLL_WIDTH			460
#define		EDITBOX_SCROLL_HEIGHT			488
#define		EDITBOX_NORMALCAP_Y				20
#define		EDITBOX_HEADERCAP_X				10
#define		EDITBOX_HEADERCAP_Y				38

// 미션 보상 
#define		MISSION_COMPEN_X				303
#define		MISSION_COMPEN_Y				445
#define		MISSION_COMPEN_CAP_X			52
#define		MISSION_TOOLTIB_GAP_Y			32
#define		MISSION_TOOLTIB_Y				30
#define		MISSION_COMPEN_ONEITEM_WIDTH	27		// 한개의 아이템  
#define		MISSION_COMPEN_ONEITEM_HEIGHT	27


// 색상 설정
#define TREEMISSION_SELECT_COLOR				(RGB(194, 155, 0))
#define TREEMISSION_PROGRESS_COLOR				(RGB(59, 177, 255))
#define TREEMISSION_COMPLETE_COLOR				(RGB(215, 215, 45))
#define TREEMISSION_IMPOSIBLE_COLOR				(RGB(184, 36, 36))
#define TREEMISSION_POSIBLE_COLOR				(RGB(255, 255, 255))

// 에러 설정 정보 관련
#define TREEMISSION_ERR_NONE					0
#define TREEMISSION_ERR_LEVEL					100
#define TREEMISSION_ERR_STAT_AT					101
#define TREEMISSION_ERR_STAT_DE					102
#define TREEMISSION_ERR_STAT_FU					103
#define TREEMISSION_ERR_STAT_SO					104
#define TREEMISSION_ERR_STAT_SH					105
#define TREEMISSION_ERR_STAT_DO					106
#define TREEMISSION_ERR_ITEM					107
#define TREEMISSION_ERR_FLUENCE					108
#define TREEMISSION_ERR_EXPERIENCE				109
#define TREEMISSION_ERR_QUEST					110
#define TREEMISSION_ERR_HIDEQUEST				111
#define TREEMISSION_ERR_POSENDQUEST				112

// 에러 설정 정보 관련
#define TREEMISSION_ERR_NONE					0
#define TREEMISSION_ERR_LEVEL					100
#define TREEMISSION_ERR_STAT_AT					101
#define TREEMISSION_ERR_STAT_DE					102
#define TREEMISSION_ERR_STAT_FU					103
#define TREEMISSION_ERR_STAT_SO					104
#define TREEMISSION_ERR_STAT_SH					105
#define TREEMISSION_ERR_STAT_DO					106
#define TREEMISSION_ERR_ITEM					107
#define TREEMISSION_ERR_FLUENCE					108
#define TREEMISSION_ERR_EXPERIENCE				109
#define TREEMISSION_ERR_QUEST					110
#define TREEMISSION_ERR_HIDEQUEST				111
#define TREEMISSION_ERR_POSENDQUEST				112

// 미션 시작 버튼
#define	MISSION_START_POS_X						705
#define	MISSION_START_POS_Y						400

// 미션 맵으로 이동 버튼 
#define	MISSION_GO_POS_X						580
#define	MISSION_GO_POS_Y						400



// 맵 버튼
#define	MAP_POS_X								296
#define	MAP_POS_Y								521
#define	MAP_POS_WIDTH							505
#define	MAP_POS_HEIGHT							90

// 닫기 창
#define	CLOSE_POS_X								792
#define	CLOSE_POS_Y								10

// 애니창
#define	ANI_POS_X								42
#define ANI_POS_Y								41
		

// SPI ID
#define	ITEM_NUM_SPI							7000022

// 2007-07-30 by bhsohn 일반, 번외 퀘스트 재정렬
#define	QUEST_RESORT_LEN						2

// 2008-12-02 by dgwoo 미션 마스터 시스템.
// 도움 요청 버튼.
#define MMASTER_HELPER_X					497
#define MMASTER_HELPER_Y					400

#define MMASTER_HELPER_MX					623
#define MMASTER_HELPER_MY					400

// 편대 생성 & 도움 요청 버튼.
#define MMASTER_HELPER_PARTY_X					438
#define MMASTER_HELPER_PARTY_Y					400

#define MMASTER_HELPER_PARTY_MX					564
#define MMASTER_HELPER_PARTY_MY					400

// 미션 마스터 등록 해제.
#define MMASTER_REG_X					38
#define MMASTER_REG_Y					630

#define MMASTER_BTN_DOWN_GAP			10.0f
// 2008-12-02 by dgwoo 미션 마스터 시스템.

#endif

// 격추수로 정렬
struct Rank2SQuestIdx_Sort_Order: binary_function<structQuestInfo, structQuestInfo, bool>
{
	bool operator()(structQuestInfo pArenaItemInfo1, structQuestInfo pArenaItemInfo2)
	{
        if(pArenaItemInfo1.nOrderIdx < pArenaItemInfo2.nOrderIdx)
		{
			return TRUE;
		}		
		return FALSE;
    };
};

// 2012-11-29 by bhsohn 혼돈의 사막 퀘스트 안나오는 버그수정
// 레벨순으로 정렬
struct Rank2SQuestIdx_Sort_Level: binary_function<structQuestInfo, structQuestInfo, bool>
{
	bool operator()(structQuestInfo pArenaItemInfo1, structQuestInfo pArenaItemInfo2)
	{
        if(pArenaItemInfo1.nMinLevel < pArenaItemInfo2.nMinLevel)
		{
			return TRUE;
		}		
		return FALSE;
    };
};
// END 2012-11-29 by bhsohn 혼돈의 사막 퀘스트 안나오는 버그수정


#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
#define TREEMISSION_MAX_DESC_STRING				430//35
#else		  
#define TREEMISSION_MAX_DESC_STRING				460//35
#endif
#define TREEMISSION_TITLE_MAX_DESC_STRING		700//35
#if defined(LANGUAGE_ENGLISH) || defined(LANGUAGE_VIETNAM)|| defined(LANGUAGE_THAI)// 2008-04-30 by bhsohn 태국 버전 추가
	#define STRING_CULL ::StringCullingUserData_ToBlank
#else
	#define STRING_CULL ::StringCullingUserDataEx	
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CINFMissionTreeInfo::CINFMissionTreeInfo(CINFMissionMain* i_pParent)
{
	m_pParentMissionMain = i_pParent;
	
	int nCnt = 0;
	m_pMissionListCtrl = NULL;
	m_pINFImageListBox = NULL;
	m_pMissionBk = NULL;	
	for(nCnt = 0;nCnt < MAX_TAB_MISSION;nCnt++)
	{
		m_pMissionMode[nCnt] = NULL;
	}
	m_nSelMission = TAB_MISSION_NOTCOMPLETE;
	
	m_fBackPosX = m_fBackPosY = 0;
	int nMainId = 0;
	for(nMainId = 0;nMainId < MAX_TAB_MISSION_MODE_EP;nMainId++)
	{
		for(nCnt = 0;nCnt < MAX_TAB_MISSION_MODE;nCnt++)
		{
			m_vecQuestInfo[nMainId][nCnt].clear();	
		}
	}
	m_vecErrPart.clear();
	m_vecErrQuestName.clear();	
	m_vecErritem.clear();	

	// 할 수 있는 미션 정보
	m_vecMissionName.clear();	
	m_vecPossibleDesc.clear();			// 미션 설명

	m_vecCompensationItemName.clear();	// 미션 보상아이템 명칭
	ZERO_MEMORY(m_nCompensation);
	ZERO_MEMORY(m_bCompenToolTib);
	
	m_pCompensation = NULL;

	m_pStartMissionBtn = NULL;
	m_pGoMissionBtn= NULL;
	m_pGiveupMissionBtn = NULL;
// 2008-12-09 by dgwoo 미션마스터.
	m_pBtnHelper			= NULL;
	m_pBtnPartyHelper		= NULL;
	m_pBtnMissionMaster		= NULL;
	m_pBtnMissionUnMaster	= NULL;	  
	m_fMMBtnTime = 0.0f;				  
	m_nMissionMasterQuestIdx = 0;

	m_pCloseBtn = NULL;
	
	m_dwSelQuestIndex = 0;		

	m_pINFMissionMapViewCtrl = NULL;
#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	m_pINFAniCtrl = NULL;
#endif

	m_fBackWidth = m_fBackHeight =1;
	
}

CINFMissionTreeInfo::~CINFMissionTreeInfo()
{	
	if(m_pCloseBtn)
	{
		m_pCloseBtn->DeleteDeviceObjects();	
		util::del(m_pCloseBtn);
	}
	if(m_pGiveupMissionBtn)
	{
		m_pGiveupMissionBtn->DeleteDeviceObjects();	
		util::del(m_pGiveupMissionBtn);
	}
// 2008-12-09 by dgwoo 미션마스터.
	if(m_pBtnHelper)
	{
		m_pBtnHelper->DeleteDeviceObjects();
		util::del(m_pBtnHelper);
	}
	if(m_pBtnPartyHelper)
	{
		m_pBtnPartyHelper->DeleteDeviceObjects();
		util::del(m_pBtnPartyHelper);
	}
	if(m_pBtnMissionMaster)
	{
		m_pBtnMissionMaster->DeleteDeviceObjects();
		util::del(m_pBtnMissionMaster);
	}
	if(m_pBtnMissionUnMaster)
	{
		m_pBtnMissionUnMaster->DeleteDeviceObjects();
		util::del(m_pBtnMissionUnMaster);
	}

	if(m_pGoMissionBtn)
	{
		m_pGoMissionBtn->DeleteDeviceObjects();	
		util::del(m_pGoMissionBtn);
	}
	if(m_pStartMissionBtn)
	{
		m_pStartMissionBtn->DeleteDeviceObjects();	
		util::del(m_pStartMissionBtn);
	}
	if(m_pMissionBk)
	{
		m_pMissionBk->DeleteDeviceObjects();	
		util::del(m_pMissionBk);
	}
	if(m_pINFMissionMapViewCtrl)
	{
		m_pINFMissionMapViewCtrl->DeleteDeviceObjects();
		util::del(m_pINFMissionMapViewCtrl);
	}
	
#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경	
	if(m_pINFAniCtrl)
	{
		m_pINFAniCtrl->DeleteDeviceObjects();
		util::del(m_pINFAniCtrl);
	}
#endif

	if(m_pMissionListCtrl)
	{
		m_pMissionListCtrl->DeleteDeviceObjects();
		util::del(m_pMissionListCtrl);
	}
	if(m_pINFImageListBox )
	{
		m_pINFImageListBox ->DeleteDeviceObjects();
		util::del(m_pINFImageListBox );
	}
	
	int nCnt = 0;
	for(nCnt = 0;nCnt < MAX_TAB_MISSION;nCnt++)
	{		
		if(m_pMissionMode[nCnt])
		{
			m_pMissionMode[nCnt]->DeleteDeviceObjects();	
			util::del(m_pMissionMode[nCnt]);
		}
	}
	if(m_pCompensation)
	{
		m_pCompensation->DeleteDeviceObjects();
		util::del(m_pCompensation);
	}
// 2008-12-09 by dgwoo 미션마스터.
	if(m_pBtnHelper)
	{
		m_pBtnHelper->DeleteDeviceObjects();
		util::del(m_pBtnHelper);
	}
	if(m_pBtnMissionMaster)
	{
		m_pBtnMissionMaster->DeleteDeviceObjects();
		util::del(m_pBtnMissionMaster);
	}
	if(m_pBtnMissionUnMaster)
	{
		m_pBtnMissionUnMaster->DeleteDeviceObjects();
		util::del(m_pBtnMissionUnMaster);
	}
	if(m_pBtnPartyHelper)
	{
		m_pBtnPartyHelper->DeleteDeviceObjects();
		util::del(m_pBtnPartyHelper);
	}
	
}


HRESULT CINFMissionTreeInfo::InitDeviceObjects()
{	
	DataHeader	* pDataHeader = NULL;
	char buf[30] ;
	int nCnt = 0;
	memset(buf, 0x00, 30);

	// 배경
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	{
		if(NULL == m_pMissionBk)
		{
			pDataHeader = g_pGameMain->m_GruopImagemanager->FindResource("MISSION");	
			m_pMissionBk = g_pGameMain->m_GruopImagemanager->GetGroupImage( pDataHeader );
			m_pMissionBk->InitDeviceObjects( g_pD3dApp->m_pImageList );
			m_pMissionBk->RestoreDeviceObjects();
 		}
 	}
#else
	{
		if(NULL == m_pMissionBk)
		{
			m_pMissionBk = new CINFImageEx;
		}
		wsprintf(buf,"m_bak");
		pDataHeader = g_pGameMain->FindResource(buf);
		m_pMissionBk->InitDeviceObjects(pDataHeader);		
	}
#endif

	// 라디오 버튼
	{
		char chRadioOff[30], chRadioOn[30];
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경		
		wsprintf(chRadioOff, "Mallbtn_1");
		wsprintf(chRadioOn, "Mallbtn_2");
		
		if(NULL == m_pMissionMode[0])
		{		
			m_pMissionMode[0] = new CINFImageRadioBtn;
		}				
		m_pMissionMode[0]->InitDeviceObjects(chRadioOff, chRadioOn);

		wsprintf(chRadioOff, "Mcombtn_1");
		wsprintf(chRadioOn, "Mcombtn_2");
		
		if(NULL == m_pMissionMode[1])
		{		
			m_pMissionMode[1] = new CINFImageRadioBtn;
		}				
		m_pMissionMode[1]->InitDeviceObjects(chRadioOff, chRadioOn);

		wsprintf(chRadioOff, "Mmisbtn_1");
		wsprintf(chRadioOn, "Mmisbtn_2");
		
		if(NULL == m_pMissionMode[2])
		{		
			m_pMissionMode[2] = new CINFImageRadioBtn;
		}				
		m_pMissionMode[2]->InitDeviceObjects(chRadioOff, chRadioOn);
#else 
		wsprintf(chRadioOff, "radio_b");
		wsprintf(chRadioOn, "radio_a");
		
		for(nCnt = 0;nCnt < MAX_TAB_MISSION;nCnt++)
		{		
			if(NULL == m_pMissionMode[nCnt])
			{		
				m_pMissionMode[nCnt] = new CINFImageRadioBtn;
			}				
			m_pMissionMode[nCnt]->InitDeviceObjects(chRadioOff, chRadioOn);
		}
#endif
		// 라디오 버튼 초기화
		SelectRadioBtn(m_nSelMission);
	}

	// 리스트 컨트롤 
	{		
		char chPlus[30], chMinus[30], chItem[30], chSel[30];
		wsprintf(chPlus, "m_plus");
		wsprintf(chMinus, "m_minus");
		wsprintf(chItem, "misradn");
		wsprintf(chSel, "m_selbox");

		if(NULL == m_pMissionListCtrl)
		{
			m_pMissionListCtrl = new CINFImageListCtrl;			
		}
		m_pMissionListCtrl->InitDeviceObjects(LISTCTRL_ONEPAGE_ITEMLEN);
		m_pMissionListCtrl->InitDeviceEtc(chPlus, chMinus, chItem, chSel);		
	}	

	// 맵 뷰
	//
	{
		if(NULL == m_pINFMissionMapViewCtrl)
		{
			m_pINFMissionMapViewCtrl = new CINFMissionMapViewCtrl;			
		}
		m_pINFMissionMapViewCtrl->InitDeviceObjects();		
	}
	
	// 애니메이션 컨트롤
#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	{
		if(NULL == m_pINFAniCtrl)
		{
			m_pINFAniCtrl = new CINFMissionAniCtrl;
		}
		m_pINFAniCtrl->InitDeviceObjects();		
	}
#endif
	// 에디트 박스
	{
		if(NULL == m_pINFImageListBox)
		{
			m_pINFImageListBox = new CINFImageListBox;
		}
		DWORD dwFlagList[EDITBOX_ONEPAGE_ITEMLEN] = {0,};
		int nFontSize[EDITBOX_ONEPAGE_ITEMLEN] = {0,};
		for(nCnt = 0;nCnt < EDITBOX_ONEPAGE_ITEMLEN;nCnt++)
		{
			if(0 == nCnt)
			{
				dwFlagList[nCnt] = D3DFONT_ZENABLE|D3DFONT_BOLD;
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
				nFontSize[nCnt] = 12;															  
#else 
				nFontSize[nCnt] = 17;
#endif
			}
			else
			{
				dwFlagList[nCnt] = D3DFONT_ZENABLE;
				nFontSize[nCnt] = 9;
			}
		}
		m_pINFImageListBox->InitDeviceObjects(EDITBOX_ONEPAGE_ITEMLEN, dwFlagList, nFontSize);
	}

	// 미션 완료 툴팁
	{
		m_pCompensation = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9, D3DFONT_ZENABLE, TRUE,1024,32);
		m_pCompensation->InitDeviceObjects(g_pD3dDev) ;
	}

	// 미션 시작 버튼
	{
		char szUpBtn[30], szDownBtn[30], szSelBtn[30], szDisBtn[30];
		wsprintf(szUpBtn, "mmistarn");
		wsprintf(szDownBtn, "mmistars");
		wsprintf(szSelBtn, "mmistaro");
		wsprintf(szDisBtn, "mmistanot");
		if(NULL == m_pStartMissionBtn)
		{
			m_pStartMissionBtn = new CINFImageBtn;
		}
		m_pStartMissionBtn->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn);
		
	}
	
	// 미션맵으로 이동
	{
		char szUpBtn[30], szDownBtn[30], szSelBtn[30], szDisBtn[30];
		wsprintf(szUpBtn, "mmgdflt");
		wsprintf(szDownBtn, "mmgpsh");
		wsprintf(szSelBtn, "mmgov");
		wsprintf(szDisBtn, "mmgdis");
		if(NULL == m_pGoMissionBtn)
		{
			m_pGoMissionBtn = new CINFImageBtn;
		}
		m_pGoMissionBtn->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn);		
	}
	
	//미션 포기
	{
		char szUpBtn[30], szDownBtn[30], szSelBtn[30], szDisBtn[30];
		wsprintf(szUpBtn, "mmisg_c");
		wsprintf(szDownBtn, "mmisg_s");
		wsprintf(szSelBtn, "mmisg_o");
		wsprintf(szDisBtn, "mmisgnot");
		if(NULL == m_pGiveupMissionBtn)
		{
			m_pGiveupMissionBtn = new CINFImageBtn;
		}
		m_pGiveupMissionBtn->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn);		
	}
// 2008-12-09 by dgwoo 미션마스터.
	{
		if(NULL == m_pBtnHelper)
		{
			m_pBtnHelper = new CINFImageBtn;
		}
		m_pBtnHelper->InitDeviceObjects("MMH");
		if(NULL == m_pBtnPartyHelper)
		{
			m_pBtnPartyHelper = new CINFImageBtn;
		}
		m_pBtnPartyHelper->InitDeviceObjects("MMPH");
		if(NULL == m_pBtnMissionMaster)
		{
			m_pBtnMissionMaster = new CINFImageBtn;
		}
		m_pBtnMissionMaster->InitDeviceObjects("MMregB");
		if(NULL == m_pBtnMissionUnMaster)
		{
			m_pBtnMissionUnMaster = new CINFImageBtn;
		}
		m_pBtnMissionUnMaster->InitDeviceObjects("MMregBU");
	}


	// 닫기 창
	//
	{
		char szUpBtn[30], szDownBtn[30], szSelBtn[30], szDisBtn[30];
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
		wsprintf(szUpBtn, "close");
		wsprintf(szDownBtn, "close");
		wsprintf(szSelBtn, "close");
		wsprintf(szDisBtn, "close");
#else 
		wsprintf(szUpBtn, "xclose");
		wsprintf(szDownBtn, "xclose");
		wsprintf(szSelBtn, "xclose");
		wsprintf(szDisBtn, "xclose");

#endif
		if(NULL == m_pCloseBtn)
		{
			m_pCloseBtn = new CINFImageBtn;
		}
		m_pCloseBtn->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn);		
	}
	return S_OK ;
}
void CINFMissionTreeInfo::LoadListItem()
{
	//m_nSelMission
	int nSelMission = m_nSelMission;
	// 리스트 컨트롤에 전체 아이템을 지운다.
	m_pMissionListCtrl->ResetContent();

	// 퀘스트 정보 로드
	LoadQuest(m_nSelMission);
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	char chEpTitle[MAX_TAB_MISSION_MODE_EP][32] =
	{
		"m_ep2bk",
		"m_ep3bk",

		"m_ep4bk"
	};																							  
#else												   
	char chEpTitle[MAX_TAB_MISSION_MODE_EP][32] =
	{
		"m_ep2bk",
		"m_ep3bk"
	};
#endif

	
	int nMainId = 0;
	for(nMainId = 0;nMainId < MAX_TAB_MISSION_MODE_EP;nMainId++)
	{
		// EP타이틀		
		int nMainIdx = TAB_MISSION_MODE_SCEN + (MAX_TAB_MISSION_MODE*nMainId);
		m_pMissionListCtrl->InserItem(nMainId, nMainIdx, chEpTitle[nMainId], TRUE);

		// 시나리오 미션
		AddScenItem(nMainId);
		// 일반 미션
		AddNormalItem(nMainId);
		// 번외 미션
		AddOutItem(nMainId);	
	}
	
	/*
	int nSubId = 1;
	// 시나리오 미션	
	//////////////////////////////////////////////////////////////////////////	
	// 세력별 퀘스트 분류후 완료 퀘스트를 가져온다
	// 세력이 틀려도 완료된 퀘스트는 화면에 표시한다	
	// 같은 세력의 미션을 찾는 부분
	vector<int> vecQuestInflue;
	vecQuestInflue = g_pQuestData->GetRaceQuestIndex(g_pShuttleChild->GetMyShuttleInfo().InfluenceType);
	
	*/
}
// 퀘스트 정보 로드
void CINFMissionTreeInfo::LoadQuest(int nSelMission)
{
	// 완료된 퀘스트 정보 얻어오기
	int nCnt = 0;
	int nMainId = 0;
	for(nMainId = 0;nMainId < MAX_TAB_MISSION_MODE_EP;nMainId++)
	{
		for(nCnt = 0;nCnt < MAX_TAB_MISSION_MODE;nCnt++)
		{
			m_vecQuestInfo[nMainId][nCnt].clear();
		}
	}

	// 모든 미션 인덱스 넘버.
	vector<int>		vecQuestIndex;	
	vecQuestIndex.clear();
	
	// 완료된 퀘스트 리스트 얻어오기
	m_pParentMissionMain->LoadQuestList(&vecQuestIndex);

	// 항목별 퀘스트 
	LoadQuestInfo(nSelMission, &vecQuestIndex);	

	// 2007-07-30 by bhsohn 일반, 번외 퀘스트 재정렬	
	for(nMainId = 0;nMainId < MAX_TAB_MISSION_MODE_EP;nMainId++)
	{
		SortQuestInfo(&m_vecQuestInfo[nMainId][TAB_MISSION_MODE_NORMAL]);
		SortQuestInfo_SideMision(&m_vecQuestInfo[nMainId][TAB_MISSION_MODE_OUT]);
		
		// 2012-11-29 by bhsohn 혼돈의 사막 퀘스트 안나오는 버그수정
		SortQuestInfo_Scen(&m_vecQuestInfo[nMainId][TAB_MISSION_MODE_SCEN]);
		// END 2012-11-29 by bhsohn 혼돈의 사막 퀘스트 안나오는 버그수정
	}
	
	// 일반, 번외 퀘스트 재정령
	ResortLoadQuest();
	// end 2007-07-30 by bhsohn 일반, 번외 퀘스트 재정렬	
}

void CINFMissionTreeInfo::ResortLoadQuest()
{
	int nSortQuest[QUEST_RESORT_LEN]= 
	{
		TAB_MISSION_MODE_NORMAL,
		TAB_MISSION_MODE_OUT
	};		
	
	int nProgressQuestIdx = 0;
	// 현재 진행중인 퀘스트는 지우지 않는다.
	CQuest* pProgressMission = g_pQuestData->GetProgressMission();	
	if(pProgressMission)
	{
		nProgressQuestIdx = pProgressMission->QuestIndex;
	}

	// 	
	int nMainId = 0;
	for(nMainId = 0;nMainId < MAX_TAB_MISSION_MODE_EP;nMainId++)
	{
		int nCnt = 0;
		BOOL bOverLevel = FALSE;
		const auto& myShuttle = g_pShuttleChild->GetMyShuttleInfo();
		int nLimitLevel = myShuttle.Level;

		for(nCnt = 0;nCnt < QUEST_RESORT_LEN; nCnt++)
		{		
			bOverLevel = FALSE;
			vector<structQuestInfo>::iterator it = m_vecQuestInfo[nMainId][nSortQuest[nCnt]].begin();
			while(it != m_vecQuestInfo[nMainId][nSortQuest[nCnt]].end())
			{
				structQuestInfo	 *pQuestinfo = &*it;			
				if((FALSE == bOverLevel )&& (pQuestinfo->nMinLevel > myShuttle.Level))
				{
					nLimitLevel = pQuestinfo->nMinLevel;
					bOverLevel = TRUE;				
				}
				// 현재 진행중인 퀘스트냐?
				BOOL bProgressQuest = FALSE;
				if((0 != nProgressQuestIdx) && (nProgressQuestIdx == pQuestinfo->nQuestIndex))
				{
					bProgressQuest = TRUE;
				}
				if(bOverLevel && (nLimitLevel < pQuestinfo->nMinLevel) && (FALSE == bProgressQuest))
				{
					it = m_vecQuestInfo[nMainId][nSortQuest[nCnt]].erase(it);
					continue;
				}
				it++;			
			}
		}
	}
}

void CINFMissionTreeInfo::LoadQuestInfo(int nSelMission, vector<int>* pVecQuestIndex)
{
	CQuest* pQuest = NULL;
	
	BOOL bShowAll, bShowComplete;
	bShowAll = bShowComplete = FALSE;
	GetMissionShow(nSelMission,  &bShowAll, &bShowComplete);

	int nQuestIdx = 0;
	
	vector<int>::iterator it = pVecQuestIndex->begin();

	int nTabKind = -1;
#ifdef C_EPSODE4_UI_CHANGE_JSKIM
	BOOL bProgressMission[MAX_TAB_MISSION_MODE_EP] = {FALSE, FALSE, FALSE};
	int bNextScenShow[MAX_TAB_MISSION_MODE_EP] = {TRUE, TRUE, TRUE};
#else
	BOOL bProgressMission[MAX_TAB_MISSION_MODE_EP] = {FALSE, FALSE};
	int bNextScenShow[MAX_TAB_MISSION_MODE_EP] = {TRUE, TRUE};
#endif
	

	while(it != pVecQuestIndex->end())
	{		
		pQuest = g_pQuestData->FindQuest(*it);
		if(NULL == pQuest)
		{
			it++;
			continue;
		}

		int nEpMainId =-1;		

		structQuestInfo struQuestInfo;			
		memset(&struQuestInfo, 0x00, sizeof(structQuestInfo));
		
		struQuestInfo.bComplete		= g_pQuestData->IsQuestCompleted(pQuest->QuestIndex);
		struQuestInfo.nQuestIndex	= pQuest->QuestIndex;			
		struQuestInfo.nQuestKind	= pQuest->QuestKind;
		struQuestInfo.nMinLevel		= pQuest->ReqLevel.Min;
		struQuestInfo.nMaxLevel		= pQuest->ReqLevel.Max;
		struQuestInfo.QuestEpisodeType = pQuest->QuestEpisodeType;

		// EP3여부 판단
		{
			if(QUEST_EPISODE_TYPE_3 == struQuestInfo.QuestEpisodeType)
			{
				nEpMainId = TAB_MISSION_MODE_EP3;
			}
 // 2011. 10. 10 by jskim UI시스템 변경
		#ifdef C_EPSODE4_UI_CHANGE_JSKIM
			else if (QUEST_EPISODE_TYPE_4 == struQuestInfo.QuestEpisodeType)     
			{
				nEpMainId = TAB_MISSION_MODE_EP4;
			}
		#endif
 // end 2011. 10. 10 by jskim UI시스템 변경
			else
			{
				nEpMainId = TAB_MISSION_MODE_EP2;
			}
		}
		if(nEpMainId == -1)
		{
			// 어떤 에피소드도 아니다.
			continue;
		}
		
		bProgressMission[nEpMainId] = TRUE;		
		if(FALSE == bShowAll && (bShowComplete != struQuestInfo.bComplete))
		{
			// 미완료 미션
			it++;
			continue;		
		}		
		
		if(g_pQuestData->IsQuestProgress(pQuest->QuestIndex))
		{
			struQuestInfo.dwColor	=  TREEMISSION_PROGRESS_COLOR;
			wsprintf(struQuestInfo.szItemTxt, STRMSG_C_050805_0002,pQuest->QuestName); // "%s [미해결]"
		}
		else if(struQuestInfo.bComplete)
		{
			struQuestInfo.dwColor	=  TREEMISSION_COMPLETE_COLOR;
			wsprintf(struQuestInfo.szItemTxt, STRMSG_C_050805_0003,pQuest->QuestName);
		}
		else
		{
			struQuestInfo.dwColor	=  TREEMISSION_IMPOSIBLE_COLOR;
			bProgressMission[nEpMainId] = CheckProgressMission(pQuest, &m_vecErrPart, &m_vecErrQuestName, &m_vecErritem);
			if( TRUE == bProgressMission[nEpMainId])
			{
				struQuestInfo.dwColor	=  TREEMISSION_POSIBLE_COLOR;					
			}			
						
			wsprintf(struQuestInfo.szItemTxt, "%s",pQuest->QuestName);
		}
		
		// 2012-11-29 by bhsohn 혼돈의 사막 퀘스트 안나오는 버그수정
		// 시나리오 미션은 완료 한것만 보이기 
		// 시나리오 미션은 마지막 다음 미션 1개 더 보여준다. 
// 		if((FALSE == bProgressMission[nEpMainId]) 
// 			&& (QUEST_KIND_SCENARIO == struQuestInfo.nQuestKind))
// 		{
// 			if(bNextScenShow[nEpMainId] == TRUE)
// 			{
// 				bNextScenShow[nEpMainId] = FALSE;
// 			}
// 			else
// 			{
// 				it++;
// 				continue;
// 			}			
// 		}	
		// END 2012-11-29 by bhsohn 혼돈의 사막 퀘스트 안나오는 버그수정

		nQuestIdx = -1;
		if(QUEST_KIND_SCENARIO == struQuestInfo.nQuestKind)
		{
			nQuestIdx = TAB_MISSION_MODE_SCEN;
		}
		else if(QUEST_KIND_NORMAL == struQuestInfo.nQuestKind)
		{
			nQuestIdx = TAB_MISSION_MODE_NORMAL;
		}
		else if(QUEST_KIND_SIDESTORY == struQuestInfo.nQuestKind)
		{
			nQuestIdx = TAB_MISSION_MODE_OUT;
		}
		// 2008-06-18 by bhsohn EP3 퀘스트 추가				

		if(-1 != nQuestIdx && -1 != nEpMainId)
		{
			m_vecQuestInfo[nEpMainId][nQuestIdx].push_back(struQuestInfo);
		}	
		// end 2008-06-18 by bhsohn EP3 퀘스트 추가
		
		it++;
	}
}
void CINFMissionTreeInfo::GetMissionShow(int nSelMission, BOOL *o_bAllShow,BOOL *o_bShowComplete)
{
	switch(nSelMission)
	{
	case TAB_MISSION_ALL:
		{
			(*o_bAllShow) = TRUE;
			(*o_bShowComplete) = FALSE;
		}
		break;
	case TAB_MISSION_COMPLETE:
		{
			(*o_bShowComplete) = TRUE;
			(*o_bAllShow) = FALSE;
		}
		break;
	case TAB_MISSION_NOTCOMPLETE:
		{				
			(*o_bAllShow) = (*o_bShowComplete) = FALSE;
		}
		break;
	}
	
}



int CINFMissionTreeInfo::CheckProgressMission(CQuest *pQuestInfo, 
												vector<int>* o_vecErrPart, 
												vector<string>*  o_vecErrQuestName,
												vector<string>*  o_vecErritem)
{
	BOOL err = TRUE;
	
	// 에러 초기화
	o_vecErrPart->clear();
	o_vecErrQuestName->clear();
	o_vecErritem->clear();

	if (FALSE == COMPARE_INFLUENCE(g_pShuttleChild->GetMyShuttleInfo().InfluenceType, pQuestInfo->ReqUnitKind))
	{// 가능 세력 체크
		o_vecErrPart->push_back(TREEMISSION_ERR_FLUENCE);
		err = FALSE;
	}

	if (!util::in_range(pQuestInfo->ReqAttackPart.Min, g_pShuttleChild->GetMyShuttleInfo().TotalGearStat.AttackPart, pQuestInfo->ReqAttackPart.Max))
	{
		o_vecErrPart->push_back(TREEMISSION_ERR_STAT_AT);
		err = FALSE;
	}

	if (!util::in_range(pQuestInfo->ReqDefensePart.Min, g_pShuttleChild->GetMyShuttleInfo().TotalGearStat.DefensePart, pQuestInfo->ReqDefensePart.Max))
	{
		o_vecErrPart->push_back(TREEMISSION_ERR_STAT_DE);
		err = FALSE;
	}

	if (!util::in_range(pQuestInfo->ReqFuelPart.Min, g_pShuttleChild->GetMyShuttleInfo().TotalGearStat.FuelPart, pQuestInfo->ReqFuelPart.Max))
	{
		o_vecErrPart->push_back(TREEMISSION_ERR_STAT_FU);
		err = FALSE;
	}

	if (!util::in_range(pQuestInfo->ReqSoulPart.Min, g_pShuttleChild->GetMyShuttleInfo().TotalGearStat.SoulPart, pQuestInfo->ReqSoulPart.Max))
	{
		o_vecErrPart->push_back(TREEMISSION_ERR_STAT_SO);
		err = FALSE;
	}

	if (!util::in_range(pQuestInfo->ReqShieldPart.Min, g_pShuttleChild->GetMyShuttleInfo().TotalGearStat.ShieldPart, pQuestInfo->ReqShieldPart.Max))
	{
		o_vecErrPart->push_back(TREEMISSION_ERR_STAT_SH);
		err = FALSE;
	}

	if (!util::in_range(pQuestInfo->ReqDodgePart.Min, g_pShuttleChild->GetMyShuttleInfo().TotalGearStat.DodgePart, pQuestInfo->ReqDodgePart.Max))
	{
		o_vecErrPart->push_back(TREEMISSION_ERR_STAT_DO);
		err = FALSE;
	}

	if (!util::in_range(pQuestInfo->ReqLevel.Min, g_pShuttleChild->GetMyShuttleInfo().Level, pQuestInfo->ReqLevel.Max))	// 필요레벨
	{
		o_vecErrPart->push_back(TREEMISSION_ERR_LEVEL);
		err = FALSE;
	}

	if (pQuestInfo->ReqExperience > g_pShuttleChild->GetMyShuttleInfo().Experience)	// 필요 경험치
	{
		o_vecErrPart->push_back(TREEMISSION_ERR_EXPERIENCE);
		err = FALSE;
	}

	int X = (int)(g_pD3dApp->m_pShuttleChild->m_vPos.x);
	int Z = (int)(g_pD3dApp->m_pShuttleChild->m_vPos.z);
	if (pQuestInfo->ReqStartArea.MapIndex && 
		FALSE == pQuestInfo->ReqStartArea.IsPositionInArea(g_pShuttleChild->GetMyShuttleInfo().MapChannelIndex.MapIndex, X,Z))	// 히든 퀘스트
	{
		o_vecErrPart->push_back(TREEMISSION_ERR_HIDEQUEST);
		err = FALSE;
	}
	
//	// 미션 완료 지역 체크
//	if (pQuestInfo->TargetMeetMapArea.MapIndex &&
//		FALSE == pQuestInfo->TargetMeetMapArea.IsPositionInArea(g_pShuttleChild->GetMyShuttleInfo().MapChannelIndex.MapIndex, X,Z))	// 히든 퀘스트
//	{
//		o_vecErrPart->push_back(MISSION_ERR_POSENDQUEST);
//		err = FALSE;
//	}

	int i = 0;
	BOOL bReqItem = FALSE;
	// 필요 아이템 확인
	for (i = 0; i < pQuestInfo->ReqItemVector.size(); i++)
	{
		ITEM_W_COUNT tmpItemWCount = pQuestInfo->ReqItemVector[i];

		CItemInfo *pItem = g_pStoreData->FindItemInInventoryByItemNum(tmpItemWCount.ItemNum);
		if (pItem == NULL || (IS_GENERAL_ITEM(pItem->Kind) && pItem->CurrentCount < tmpItemWCount.Count))
		{
			o_vecErritem->push_back(pItem->ItemInfo->ItemName);
			bReqItem = TRUE;			
			err = FALSE;
		}
	}
	if(bReqItem == TRUE)	{
		o_vecErrPart->push_back(TREEMISSION_ERR_ITEM);
	}

//	// 필요 몬스터 아이템 확인
//	bReqItem = FALSE;
//	for (i = 0; i < pQuestInfo->TargetMonsterItemVector.size(); i++)
//	{
//		if (pQuestInfo->TargetMonsterItemVector[i].ItemWithCount.ItemNum != 0)
//		{
//			CItemInfo *pTargetMonsterItem = g_pStoreData->FindItemInInventoryByItemNum(pQuestInfo->TargetMonsterItemVector[i].ItemWithCount.ItemNum);
//			
//			// Count가 0이면 꼭 있지 않아도 되는 아이템임
//			if (pQuestInfo->TargetMonsterItemVector[i].ItemWithCount.Count == 0)
//			{
//				continue;
//			}
//			
//			// 해당 아이템을 가지지 않은 경우
//			if (pTargetMonsterItem == NULL)
//			{
//				err = FALSE;
//				bReqItem = TRUE;
//				break;
//			}
//			
//			// 해당 아이템의 개수가 부족한 경우
//			if (IS_COUNTABLE_ITEM(pTargetMonsterItem->ItemInfo->Kind)
//				&& pTargetMonsterItem->CurrentCount < pQuestInfo->TargetMonsterItemVector[i].ItemWithCount.Count)
//			{
//				err = FALSE;
//				bReqItem = TRUE;
//				break;
//			}
//		}
//	}
//	if(bReqItem == TRUE)	{
//		o_vecErrPart->push_back(TREEMISSION_ERR_MONSTER_ITEM);
//	}

	// 필요 quest 확인
	BOOL	bReqQuest = FALSE;
	for (i = 0; i < pQuestInfo->ReqQuestIndexVector.size(); i++)
	{
		MEX_QUEST_INFO* pCharacterQuest = g_pQuestData->FindCharacterQuest(pQuestInfo->ReqQuestIndexVector[i]);
		if(pCharacterQuest == NULL || pCharacterQuest->QuestState != QUEST_STATE_COMPLETED)
		{
			if(pQuestInfo)
			{
				CQuest* pQuest = NULL;
				vector<INT>::iterator itIndex = pQuestInfo->ReqQuestIndexVector.begin();
				while(itIndex != pQuestInfo->ReqQuestIndexVector.end())
				{
					pQuest = g_pQuestData->FindQuest(*itIndex);
					if(pQuest){
						o_vecErrQuestName->push_back( pQuest->QuestName );
					}
					itIndex++;
				}
				
			}
			bReqQuest = TRUE;
			err = FALSE;
		}
	}
	if(bReqQuest == TRUE) {
		o_vecErrPart->push_back(TREEMISSION_ERR_QUEST);
	}

	return err;
}
int CINFMissionTreeInfo::GetQuestKind_TO_Tab(BYTE QuestKind)
{
	int nQuestTab=-1;
	switch(QuestKind)
	{
	case QUEST_KIND_NORMAL:			// 일반 Quest
		{
			nQuestTab = TAB_MISSION_COMPLETE;			
		}
		break;
	case QUEST_KIND_SIDESTORY:		// 번외 미션 
		{
			nQuestTab = TAB_MISSION_NOTCOMPLETE;						
		}
		break;
	case QUEST_KIND_SCENARIO:		// 시나리오 Quest
		{
			nQuestTab = TAB_MISSION_ALL;
		}
		break;
	}
	return nQuestTab;
}
// 시나리오 미션 추가
void CINFMissionTreeInfo::AddScenItem(int nMainId)
{
	char chMissionMode[30];
	wsprintf(chMissionMode, "m_scen");	
	int nSubId = 1;
	int nMainIdx = TAB_MISSION_MODE_SCEN + (MAX_TAB_MISSION_MODE*nMainId);
	
	// 헤더 아이템 
	m_pMissionListCtrl->InserItem(nMainId, nMainIdx, chMissionMode, FALSE);
	
	{
		vector<structQuestInfo>::iterator it = m_vecQuestInfo[nMainId][TAB_MISSION_MODE_SCEN].begin();
		while(it != m_vecQuestInfo[nMainId][TAB_MISSION_MODE_SCEN].end())
		{
			structQuestInfo	 stQuestinfo = (*it);
			if(QUEST_KIND_SCENARIO == stQuestinfo.nQuestKind)
			{
				// 2008-06-18 by bhsohn EP3 퀘스트 추가
				(*it).nMainIdx = nMainIdx;
				(*it).nSubIdx = nSubId;

				m_pMissionListCtrl->InserSubItem(nMainId, (*it).nMainIdx, nSubId, stQuestinfo.szItemTxt, stQuestinfo.dwColor);
				nSubId++;
			}
			it++;	
		}	
	}
	
	
}
// 일반 미션
void CINFMissionTreeInfo::AddNormalItem(int nMainId)
{
	char chMissionMode[30];
	wsprintf(chMissionMode, "m_nomal");	
	int nSubId = 1;
	int nMainIdx = TAB_MISSION_MODE_NORMAL + (MAX_TAB_MISSION_MODE*nMainId);

	m_pMissionListCtrl->InserItem(nMainId, nMainIdx, chMissionMode, FALSE);	
	
	{
		vector<structQuestInfo>::iterator it = m_vecQuestInfo[nMainId][TAB_MISSION_MODE_NORMAL].begin();
		while(it != m_vecQuestInfo[nMainId][TAB_MISSION_MODE_NORMAL].end())
		{
			structQuestInfo	 stQuestinfo = (*it);
			if(QUEST_KIND_NORMAL == stQuestinfo.nQuestKind)
			{
				// 2008-06-18 by bhsohn EP3 퀘스트 추가
				(*it).nMainIdx = nMainIdx;								
				(*it).nSubIdx = nSubId;
				
				m_pMissionListCtrl->InserSubItem(nMainId, (*it).nMainIdx, nSubId, stQuestinfo.szItemTxt, stQuestinfo.dwColor);
				nSubId++;
			}
			it++;	
		}
	}
}
void CINFMissionTreeInfo::SortQuestInfo(vector<structQuestInfo>* i_vecQuestInfo)
{
	// 인덱스 만들기 
	vector<structQuestInfo>::iterator it = i_vecQuestInfo->begin();
	while(it != i_vecQuestInfo->end())
	{
		structQuestInfo stQuestInfo = (*it);
		(*it).nOrderIdx = ConvertOrderIdx(stQuestInfo.szItemTxt);		
		it++;
	}

	// 정렬
	sort(i_vecQuestInfo->begin(), i_vecQuestInfo->end(), Rank2SQuestIdx_Sort_Order());	
}


int CINFMissionTreeInfo::ConvertOrderIdx(char* pItemTxt)
{	
	char chBuffTmp[512];
	memset(chBuffTmp, 0x00, 512);
	int nCnt = 0;	
	for(nCnt = 0;nCnt < strlen(pItemTxt);nCnt++)
	{
		if(pItemTxt[nCnt] == '.')
		{				
			break;
		}
		chBuffTmp[nCnt] = pItemTxt[nCnt];
	}
	int nValue = atoi(chBuffTmp);	

	return nValue;

}

// 번외 미션
void CINFMissionTreeInfo::AddOutItem(int nMainId)
{
	char chMissionMode[30];
	wsprintf(chMissionMode, "m_out");	
	int nSubId = 1;
	
	int nMainIdx = TAB_MISSION_MODE_OUT + (MAX_TAB_MISSION_MODE*nMainId);
	m_pMissionListCtrl->InserItem(nMainId, nMainIdx, chMissionMode, FALSE);	
	
	{
		vector<structQuestInfo>::iterator it = m_vecQuestInfo[nMainId][TAB_MISSION_MODE_OUT].begin();
		while(it != m_vecQuestInfo[nMainId][TAB_MISSION_MODE_OUT].end())
		{
			structQuestInfo	 stQuestinfo = (*it);
			if(QUEST_KIND_SIDESTORY == stQuestinfo.nQuestKind)
			{
				// 2008-06-18 by bhsohn EP3 퀘스트 추가
				(*it).nMainIdx = nMainIdx;												
				(*it).nSubIdx = nSubId;
				//2011-10-27 by jhahn 인피3차 퀘스트 창 수정
				if (stQuestinfo.QuestEpisodeType != QUEST_INFINITY_TYPE_3) 	
				{
				m_pMissionListCtrl->InserSubItem(nMainId, (*it).nMainIdx, nSubId, stQuestinfo.szItemTxt, stQuestinfo.dwColor);
				nSubId++;
				}
				//end 2011-10-27 by jhahn 인피3차 퀘스트 창 수정
			}
			it++;	
		}	
	}
}

//번외 미션 정렬 방식
void CINFMissionTreeInfo::SortQuestInfo_SideMision(vector<structQuestInfo>* i_vecQuestInfo)
{
	BOOL bFind = FALSE;
	int nOrderIdx = 1;
	CHARACTER myShuttleInfo = g_pShuttleChild->GetMyShuttleInfo();
	vector<int> vecQuestIndex = g_pQuestData->GetRaceQuestIndex_ToVector(myShuttleInfo.InfluenceType);

	vector<int>::iterator itVector = vecQuestIndex.begin();
	while(itVector != vecQuestIndex.end())
	{
		int nFindQuestIdx = (*itVector);
		bFind = SortSideMision(nFindQuestIdx, nOrderIdx, i_vecQuestInfo);
		if(bFind)		
		{
			nOrderIdx++;
		}
		itVector++;
	}
	// 정렬
	sort(i_vecQuestInfo->begin(), i_vecQuestInfo->end(), Rank2SQuestIdx_Sort_Order());	
}
BOOL CINFMissionTreeInfo::SortSideMision(int nFindQuestIdx, int nOrderIdx, vector<structQuestInfo>* i_vecQuestInfo)
{
	vector<structQuestInfo>::iterator it = i_vecQuestInfo->begin();
	while(it != i_vecQuestInfo->end())
	{
		structQuestInfo stQuestInfo = (*it);
		if(nFindQuestIdx == stQuestInfo.nQuestIndex)
		{				
			(*it).nOrderIdx = nOrderIdx;
			
			return TRUE;
		}
		it++;
	}
	return FALSE;
	
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2012-11-29 by bhsohn 혼돈의 사막 퀘스트 안나오는 버그수정
/// \date		2012-11-29 ~ 2012-11-29
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFMissionTreeInfo::SortQuestInfo_Scen(vector<structQuestInfo>* i_vecQuestInfo)
{	
	// 정렬
	std::sort(i_vecQuestInfo->begin(), i_vecQuestInfo->end(), Rank2SQuestIdx_Sort_Level());	

	// 임시 버퍼에 넣자
	vector<structQuestInfo>	vecTmpQuest;	
	{
		vecTmpQuest.clear();		
		vector<structQuestInfo>::iterator it = i_vecQuestInfo->begin();
		while(it != i_vecQuestInfo->end())
		{
			vecTmpQuest.push_back(*it);
			it++;
		}
	}
	{
		i_vecQuestInfo->clear();	// 다시 넣기 위해 초기화
		int bNextScenShow = TRUE;
		vector<structQuestInfo>::iterator it = vecTmpQuest.begin();
		const auto& myShuttle = g_pShuttleChild->GetMyShuttleInfo();
		while(it != vecTmpQuest.end())
		{
			structQuestInfo struQuestInfo = (*it);			

			if(myShuttle.Level > struQuestInfo.nMaxLevel)
			{
				it++;
				continue;
			}

			if(TREEMISSION_IMPOSIBLE_COLOR == struQuestInfo.dwColor)
			{				
				// 현재 불가능한 퀘스트
				if(bNextScenShow == TRUE)
				{
					bNextScenShow = FALSE;
				}
				else
				{
					it++;
					continue;
				}		
			}

			i_vecQuestInfo->push_back(struQuestInfo);
			it++;
		}
	}

// 	{
// 		// Debug용
// 		vector<structQuestInfo>::iterator it = i_vecQuestInfo->begin();
// 		while(it != i_vecQuestInfo->end())
// 		{
// 			structQuestInfo struTmp = (*it);
// 			CQuest* pQuest = g_pQuestData->FindQuest(struTmp.nQuestIndex);
// 			if(pQuest)
// 			{
// 				DBGOUT("[%d] [%s]  \n", struTmp.nQuestIndex, pQuest->QuestName);
// 			}
// 			else
// 			{
// 				DBGOUT("ERROR QuestIdx Error[%d] \n", struTmp.nQuestIndex);
// 			}
// 			it++;
// 		}
// 	}
	
}



HRESULT CINFMissionTreeInfo::RestoreDeviceObjects()
{
	int nCnt = 0;	
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	POINT ptBk;
	// 배경 이미지 
	{	
		m_pMissionBk->RestoreDeviceObjects();		
		ptBk.x = m_pMissionBk->GetMaxPos().x - m_pMissionBk->GetMinPos().x;
		ptBk.y = m_pMissionBk->GetMaxPos().y - m_pMissionBk->GetMinPos().y;
 
 		m_fBackWidth	= ptBk.x;
 		m_fBackHeight	= ptBk.y;
		
		if(g_pD3dApp->GetBackBufferDesc().Width > ptBk.x)
		{
			m_fBackPosX = (g_pD3dApp->GetBackBufferDesc().Width/2)-(ptBk.x/2);
		}
		if(g_pD3dApp->GetBackBufferDesc().Height > ptBk.y)
		{
			m_fBackPosY = (g_pD3dApp->GetBackBufferDesc().Height/2)-(ptBk.y/2);
		}	
	}
	// 리스트 컨트롤 
	{		
		m_pMissionListCtrl->RestoreDeviceObjects();
		m_pMissionListCtrl->SetListCtrlPos(m_fBackPosX+LISTCTRL_POS_X, m_fBackPosY+LISTCTRL_POS_Y, 
											LISTCTRL_SUB_POS_X,LISTCTRL_SUB_POS_Y,
											186, -10,
											LISTCTRL_WIDTH, LISTCTRL_HEIGHT);		
		// 아이템 정렬
		//m_pMissionListCtrl->SortListCtrlItem();
	}
 
	
	{
		// 이미지 버튼 
		float fModeX[MAX_TAB_MISSION] = 
		{
			m_fBackPosX + TAB_MISSION_ALL_POS_X, 
			m_fBackPosX + TAB_MISSION_ALL_POS_X + 27, 
			m_fBackPosX + TAB_MISSION_ALL_POS_X + 53
		};
		
		float fModeY[MAX_TAB_MISSION] = 
		{
			m_fBackPosY + TAB_MISSION_ALL_POS_Y, 
			m_fBackPosY + TAB_MISSION_ALL_POS_Y, 
			m_fBackPosY + TAB_MISSION_ALL_POS_Y
		};
		float fWidth[MAX_TAB_MISSION];
		
		fWidth[TAB_MISSION_ALL]	= fModeX[TAB_MISSION_COMPLETE] - fModeX[TAB_MISSION_ALL];
		fWidth[TAB_MISSION_COMPLETE]	= fModeX[TAB_MISSION_NOTCOMPLETE] - fModeX[TAB_MISSION_COMPLETE];
		fWidth[TAB_MISSION_NOTCOMPLETE]		= (m_fBackPosX + 256) - fModeX[TAB_MISSION_NOTCOMPLETE];
		
		for(nCnt = 0;nCnt < MAX_TAB_MISSION;nCnt++)
		{		
			m_pMissionMode[nCnt]->RestoreDeviceObjects();
			m_pMissionMode[nCnt]->SetPosition(fModeX[nCnt], fModeY[nCnt], TAB_MISSION_ALL_SIZE );		
		}
	}

	// 에디트 박스
	{
		float fPosX = m_fBackPosX + EDITBOX_POS_X;
		float fPosY = m_fBackPosY + EDITBOX_POS_Y;		
		DWORD dwColor = GUI_FONT_COLOR;//GUI_FONT_COLOR_YM;

		m_pINFImageListBox->RestoreDeviceObjects();	
		// 스크롤 위치 갱신
		m_pINFImageListBox->SetScrollPos(fPosX, fPosY, 
											EDITBOX_WIDTH, EDITBOX_HEIGHT,
											EDITBOX_SCROLL_POS_X, EDITBOX_SCROLL_POS_Y,
											EDITBOX_SCROLL_WIDTH, EDITBOX_SCROLL_HEIGHT);

		for(nCnt = 0;nCnt < EDITBOX_ONEPAGE_ITEMLEN;nCnt++)
		{					
			m_pINFImageListBox->SetPosition(nCnt, fPosX, fPosY, dwColor);
			fPosY += EDITBOX_NORMALCAP_Y;
			if(0 == nCnt)
			{
				fPosX += EDITBOX_HEADERCAP_X;
				fPosY += EDITBOX_HEADERCAP_Y;
			}
			dwColor = GUI_FONT_COLOR_W;
		}		
	}
	m_pCompensation->RestoreDeviceObjects();	

	{
		//편대 & 도움요청 버튼
		float fPosX = m_fBackPosX + MMASTER_HELPER_PARTY_MX;
		float fPosY = m_fBackPosY + MISSION_START_POS_Y;

		m_pBtnPartyHelper->RestoreDeviceObjects();
		m_pBtnPartyHelper->SetBtnPosition(fPosX,fPosY);
	

	}
	// 도움요청 버튼
	{
		float fPosX = m_fBackPosX + MISSION_START_POS_X;
		float fPosY = m_fBackPosY + MISSION_START_POS_Y;

		m_pBtnHelper->RestoreDeviceObjects();
		m_pBtnHelper->SetBtnPosition(fPosX, fPosY);

	}
	// 미션 시작 버튼 
	{
		float fPosX = m_fBackPosX + MISSION_START_POS_X +((  MISSION_BUTTON_SIZE_X + 3.0f ) * 3.0f);
		float fPosY = m_fBackPosY + MISSION_START_POS_Y ;

		m_pStartMissionBtn->RestoreDeviceObjects();		
		m_pStartMissionBtn->SetBtnPosition(fPosX, fPosY);
	}

	// 미션맵으로 이동
	{
		float fPosX = m_fBackPosX + ( MISSION_START_POS_X + MISSION_BUTTON_SIZE_X + 3 );
		float fPosY = m_fBackPosY + MISSION_START_POS_Y;		

		m_pGoMissionBtn->RestoreDeviceObjects();		
		m_pGoMissionBtn->SetBtnPosition(fPosX, fPosY); 
	}

	//미션 포기
	{
		float fPosX = m_fBackPosX + MISSION_START_POS_X +((  MISSION_BUTTON_SIZE_X + 3.0f ) * 2.0f);
		float fPosY = m_fBackPosY + MISSION_START_POS_Y;		

		m_pGiveupMissionBtn->RestoreDeviceObjects();		
		m_pGiveupMissionBtn->SetBtnPosition(fPosX, fPosY);
	}
// 2008-12-09 by dgwoo 미션마스터.
	{// 미션 마스터.
		float fPosX = m_fBackPosX + MMASTER_REG_X;
		float fPosY = m_fBackPosY + MMASTER_REG_Y;
		m_pBtnMissionMaster->RestoreDeviceObjects();
		m_pBtnMissionMaster->SetBtnPosition( fPosX, fPosY );
		m_pBtnMissionUnMaster->RestoreDeviceObjects();
		m_pBtnMissionUnMaster->SetBtnPosition( fPosX, fPosY );
	}
	
	// 닫기 창
	{		
		float fPosX = m_fBackPosX + CLOSE_POS_X;
		float fPosY = m_fBackPosY + CLOSE_POS_Y;		

		m_pCloseBtn->RestoreDeviceObjects();		
		m_pCloseBtn->SetBtnPosition(fPosX, fPosY);
	}

	// 맵 뷰
	{		
		float fPosX = m_fBackPosX + MAP_POS_X;
		float fPosY = m_fBackPosY + MAP_POS_Y;		

		m_pINFMissionMapViewCtrl->RestoreDeviceObjects();
		m_pINFMissionMapViewCtrl->SetWindowPos(fPosX, fPosY, MAP_POS_WIDTH, MAP_POS_HEIGHT);
	}

	//애니메이션 컨트롤	
// 	{		
// 		float fPosX = m_fBackPosX + ANI_POS_X;
// 		float fPosY = m_fBackPosY + ANI_POS_Y;		
// 
// 		m_pINFAniCtrl->RestoreDeviceObjects();
// 		m_pINFAniCtrl->SetWindowPos(fPosX, fPosY, 1, 1);
// 	}
#else 
	// 배경 이미지 
	{	
		m_pMissionBk->RestoreDeviceObjects();		
		POINT ptBk = m_pMissionBk->GetImgSize();

		m_fBackWidth	= ptBk.x;
		m_fBackHeight	= ptBk.y;
		
		if(g_pD3dApp->GetBackBufferDesc().Width > ptBk.x)
		{
			m_fBackPosX = (g_pD3dApp->GetBackBufferDesc().Width/2)-(ptBk.x/2);
		}
		if(g_pD3dApp->GetBackBufferDesc().Height > ptBk.y)
		{
			m_fBackPosY = (g_pD3dApp->GetBackBufferDesc().Height/2)-(ptBk.y/2);
		}	
	}

	// 리스트 컨트롤 
	{		
		m_pMissionListCtrl->RestoreDeviceObjects();
		m_pMissionListCtrl->SetListCtrlPos(m_fBackPosX+LISTCTRL_POS_X, m_fBackPosY+LISTCTRL_POS_Y, 
											LISTCTRL_SUB_POS_X,LISTCTRL_SUB_POS_Y,
											229, -34,
											LISTCTRL_WIDTH, LISTCTRL_HEIGHT);		
		// 아이템 정렬
		//m_pMissionListCtrl->SortListCtrlItem();
	}

	
	{
		// 이미지 버튼 
		float fModeX[MAX_TAB_MISSION] = 
		{
			m_fBackPosX + TAB_MISSION_ALL_POS_X, 
			m_fBackPosX + TAB_MISSION_COMPLETE_POS_X, 
			m_fBackPosX + TAB_MISSION_NOTCOMPLETE_POS_X
		};
		
		float fModeY[MAX_TAB_MISSION] = 
		{
			m_fBackPosY + TAB_MISSION_ALL_POS_Y, 
			m_fBackPosY + TAB_MISSION_COMPLETE_POS_Y, 
			m_fBackPosY + TAB_MISSION_NOTCOMPLETE_POS_Y
		};
		float fWidth[MAX_TAB_MISSION];
		
		fWidth[TAB_MISSION_ALL]	= fModeX[TAB_MISSION_COMPLETE] - fModeX[TAB_MISSION_ALL];
		fWidth[TAB_MISSION_COMPLETE]	= fModeX[TAB_MISSION_NOTCOMPLETE] - fModeX[TAB_MISSION_COMPLETE];
		fWidth[TAB_MISSION_NOTCOMPLETE]		= (m_fBackPosX + 256) - fModeX[TAB_MISSION_NOTCOMPLETE];
		
		for(nCnt = 0;nCnt < MAX_TAB_MISSION;nCnt++)
		{		
			m_pMissionMode[nCnt]->RestoreDeviceObjects();
			m_pMissionMode[nCnt]->SetPosition(fModeX[nCnt], fModeY[nCnt], fWidth[nCnt]);		
		}
	}

	// 에디트 박스
	{
		float fPosX = m_fBackPosX + EDITBOX_POS_X;
		float fPosY = m_fBackPosY + EDITBOX_POS_Y;		
		DWORD dwColor = GUI_FONT_COLOR_YM;

		m_pINFImageListBox->RestoreDeviceObjects();	
		// 스크롤 위치 갱신
		m_pINFImageListBox->SetScrollPos(fPosX, fPosY, 
											EDITBOX_WIDTH, EDITBOX_HEIGHT,
											EDITBOX_SCROLL_POS_X, EDITBOX_SCROLL_POS_Y,
											EDITBOX_SCROLL_WIDTH, EDITBOX_SCROLL_HEIGHT);

		for(nCnt = 0;nCnt < EDITBOX_ONEPAGE_ITEMLEN;nCnt++)
		{					
			m_pINFImageListBox->SetPosition(nCnt, fPosX, fPosY, dwColor);
			fPosY += EDITBOX_NORMALCAP_Y;
			if(0 == nCnt)
			{
				fPosX += EDITBOX_HEADERCAP_X;
				fPosY += EDITBOX_HEADERCAP_Y;
			}
			dwColor = GUI_FONT_COLOR_W;
		}		
	}

	m_pCompensation->RestoreDeviceObjects();	

	// 미션 시작 버튼 
	{
		float fPosX = m_fBackPosX + MISSION_START_POS_X;
		float fPosY = m_fBackPosY + MISSION_START_POS_Y;		

		m_pStartMissionBtn->RestoreDeviceObjects();		
		m_pStartMissionBtn->SetBtnPosition(fPosX, fPosY);
	}

	// 미션맵으로 이동
	{
		float fPosX = m_fBackPosX + MISSION_GO_POS_X;
		float fPosY = m_fBackPosY + MISSION_GO_POS_Y;		

		m_pGoMissionBtn->RestoreDeviceObjects();		
		m_pGoMissionBtn->SetBtnPosition(fPosX, fPosY);
	}

	//미션 포기
	{
		float fPosX = m_fBackPosX + MISSION_START_POS_X;
		float fPosY = m_fBackPosY + MISSION_START_POS_Y;		

		m_pGiveupMissionBtn->RestoreDeviceObjects();		
		m_pGiveupMissionBtn->SetBtnPosition(fPosX, fPosY);
	}
// 2008-12-09 by dgwoo 미션마스터.
	{// 미션 마스터.
		m_pBtnHelper->RestoreDeviceObjects();
		m_pBtnMissionMaster->RestoreDeviceObjects();
		m_pBtnMissionMaster->SetBtnPosition(m_fBackPosX+MMASTER_REG_X,m_fBackPosY+MMASTER_REG_Y);
		m_pBtnMissionUnMaster->RestoreDeviceObjects();
		m_pBtnMissionUnMaster->SetBtnPosition(m_fBackPosX+MMASTER_REG_X,m_fBackPosY+MMASTER_REG_Y);
		m_pBtnPartyHelper->RestoreDeviceObjects();
	}
	
	// 닫기 창
	{		
		float fPosX = m_fBackPosX + CLOSE_POS_X;
		float fPosY = m_fBackPosY + CLOSE_POS_Y;		

		m_pCloseBtn->RestoreDeviceObjects();		
		m_pCloseBtn->SetBtnPosition(fPosX, fPosY);
	}

	// 맵 뷰
	{		
		float fPosX = m_fBackPosX + MAP_POS_X;
		float fPosY = m_fBackPosY + MAP_POS_Y;		

		m_pINFMissionMapViewCtrl->RestoreDeviceObjects();
		m_pINFMissionMapViewCtrl->SetWindowPos(fPosX, fPosY, MAP_POS_WIDTH, MAP_POS_HEIGHT);
	}

	//애니메이션 컨트롤	
	{		
		float fPosX = m_fBackPosX + ANI_POS_X;
		float fPosY = m_fBackPosY + ANI_POS_Y;		

		m_pINFAniCtrl->RestoreDeviceObjects();
		m_pINFAniCtrl->SetWindowPos(fPosX, fPosY, 1, 1);
	}
#endif
	// 정보들 초기화 
	InitBtn();
	
		
	return S_OK ;
}

HRESULT CINFMissionTreeInfo::DeleteDeviceObjects()
{
	{
		m_pMissionBk->DeleteDeviceObjects();	
		util::del(m_pMissionBk);		
	}

	{
		m_pMissionListCtrl->DeleteDeviceObjects();	
		util::del(m_pMissionListCtrl);		
	}
	{
		m_pINFMissionMapViewCtrl->DeleteDeviceObjects();	
		util::del(m_pINFMissionMapViewCtrl);		
	}
#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	{
		m_pINFAniCtrl->DeleteDeviceObjects();	
		util::del(m_pINFAniCtrl);		
	}
#endif

	{
		m_pINFImageListBox->DeleteDeviceObjects();	
		util::del(m_pINFImageListBox);		
	}

	int nCnt = 0;
	// 이미지 버튼 
	for(nCnt = 0;nCnt < MAX_TAB_MISSION;nCnt++)
	{
		m_pMissionMode[nCnt]->DeleteDeviceObjects();	
		util::del(m_pMissionMode[nCnt]);		
	}
	m_pCompensation->DeleteDeviceObjects();
	util::del(m_pCompensation);	
	
	// 미션 시작 
	{
		m_pStartMissionBtn->DeleteDeviceObjects();	
		util::del(m_pStartMissionBtn);
	}
	// 미션맵으로 이동	
	{
		m_pGoMissionBtn->DeleteDeviceObjects();	
		util::del(m_pGoMissionBtn);
	}
	
	// 미션 포기
	{
		m_pGiveupMissionBtn->DeleteDeviceObjects();	
		util::del(m_pGiveupMissionBtn);
	}
// 2008-12-09 by dgwoo 미션마스터.
	{
		m_pBtnHelper->DeleteDeviceObjects();
		util::del(m_pBtnHelper);
		m_pBtnMissionMaster->DeleteDeviceObjects();
		util::del(m_pBtnMissionMaster);
		m_pBtnMissionUnMaster->DeleteDeviceObjects();
		util::del(m_pBtnMissionUnMaster);
		m_pBtnPartyHelper->DeleteDeviceObjects();
		util::del(m_pBtnPartyHelper);
	}
	
	// 닫기 창
	{
		m_pCloseBtn->DeleteDeviceObjects();	
		util::del(m_pCloseBtn);
	}
	return S_OK ;
}

HRESULT CINFMissionTreeInfo::InvalidateDeviceObjects()
{
	m_pINFMissionMapViewCtrl->InvalidateDeviceObjects();
	m_pMissionBk->InvalidateDeviceObjects();
	m_pMissionListCtrl->InvalidateDeviceObjects();
	m_pINFImageListBox->InvalidateDeviceObjects();
	m_pCompensation->InvalidateDeviceObjects();
	m_pStartMissionBtn->InvalidateDeviceObjects();		
	m_pGoMissionBtn->InvalidateDeviceObjects();		
	m_pGiveupMissionBtn->InvalidateDeviceObjects();		
// 2008-12-09 by dgwoo 미션마스터.
	m_pBtnHelper->InvalidateDeviceObjects();
	m_pBtnMissionMaster->InvalidateDeviceObjects();
	m_pBtnMissionUnMaster->InvalidateDeviceObjects();
	m_pBtnPartyHelper->InvalidateDeviceObjects();
	m_pCloseBtn->InvalidateDeviceObjects();		
#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	m_pINFAniCtrl->InvalidateDeviceObjects();		
#endif
	
	
	int nCnt = 0;
	// 이미지 버튼 
	for(nCnt = 0;nCnt < MAX_TAB_MISSION;nCnt++)
	{
		m_pMissionMode[nCnt]->InvalidateDeviceObjects();
	}	
	return S_OK ;
}

void CINFMissionTreeInfo::Render()
{	
	// 배경 랜더링	
	{
		m_pMissionBk->Move(m_fBackPosX,m_fBackPosY);
		m_pMissionBk->Render();
	}	
	// 리스트 컨트롤
	{
		m_pMissionListCtrl->Render();
	}
	
	// 에디트 박스
	{
		m_pINFImageListBox->Render();
	}

	// 미션 시작 버튼 	
	{
		m_pStartMissionBtn->Render();
	}
	
	// 미션맵으도 이동	
	{
		m_pGoMissionBtn->Render();
	}
	// 미션 포기
	{
		m_pGiveupMissionBtn->Render();
	}
	{
		m_pCloseBtn->Render();
	}
	
	
	// 버튼 정보 랜더링 
	RednerMissionMode();

	// 보상아이템 렌더링
	RenderCompensationItem();

	// 맵뷰
	{
		m_pINFMissionMapViewCtrl->Render();
	}
	// 애니메이션 컨트롤
	{
#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
		m_pINFAniCtrl->Render();
#endif
	}
// 2008-12-09 by dgwoo 미션마스터.
	// 미션 마스터.
	{
		m_pBtnMissionMaster->Render();
		m_pBtnMissionUnMaster->Render();
		m_pBtnHelper->Render();
		m_pBtnPartyHelper->Render();
	}

}

void CINFMissionTreeInfo::RednerMissionMode()
{		
	int nCnt = 0;
	for(nCnt =0; nCnt < MAX_TAB_MISSION; nCnt++)
	{		
		m_pMissionMode[nCnt]->Render();
	}	
}

void CINFMissionTreeInfo::RenderCompensationItem()
{	
	FLOG( "CINFMissionInfo::RenderCompensationItem()" );

	int nCount = 0;
	int nItemGabX = m_fBackPosX+MISSION_COMPEN_X;
	int nItemGabY = m_fBackPosY+MISSION_COMPEN_Y;
	char buf[64];
	ZERO_MEMORY(buf);
	CINFImageEx* pSelectIcon;		// 2011. 10. 10 by jskim UI시스템 변경
	USHORT nUnitKind;
	vector<QUEST_PAY_ITEM>::iterator it = m_vecCompensationItemName.begin();
	while(it != m_vecCompensationItemName.end())
	{
		if(nCount>=MAX_TAB_COMPENS)
			break;		
		if(it->ItemNum)
		{			
			ITEM * p2 = g_pGameMain->GetServerItemInfo(it->ItemNum);
			if(p2 == NULL)
				break;
			nUnitKind = it->ToUnitKind;						
			if((nUnitKind == UNITKIND_ALL_MASK)
				|| (0 == nUnitKind )
				|| (CompareBitFlag( nUnitKind, g_pShuttleChild->GetMyShuttleInfo().UnitKind )))
			{
				if(IS_SKILL_ITEM(p2->Kind))
				{
					// 2013-03-13 by bhsohn 스킬 번호 구조 수정
//					wsprintf(buf, "%08d", SKILL_BASE_NUM(p2->SourceIndex));
					wsprintf(buf, "%08d", SKILL_BASE_SOURCEINDEX(p2->SourceIndex));
				}
				else
				{
					wsprintf(buf, "%08d", p2->SourceIndex);
				}
				pSelectIcon = g_pGameMain->FindIcon(buf);
				pSelectIcon->Move(nItemGabX, nItemGabY);
				pSelectIcon->Render();
				
				if(m_bCompenToolTib[nCount] == TRUE && p2->ItemNum == ITEM_NUM_SPI)
				{	// 스피일경우만 표시
					ZERO_MEMORY(buf);				
					wsprintf(buf, "SPI:%d", (int)it->Count);
					g_pGameMain->RenderPopUpWindowImage(nItemGabX, nItemGabY+MISSION_TOOLTIB_GAP_Y,strlen(buf)*6.5, 1 );
					m_pCompensation->DrawText(nItemGabX, nItemGabY+MISSION_TOOLTIB_Y, GUI_FONT_COLOR_YM, buf);
				}
				else if(m_bCompenToolTib[nCount] == TRUE)
				{	// 그외의 경우 아이템 정보 표시
					if(p2) 
					{
						g_pGameMain->SetItemInfo(0, p2->ItemNum, nItemGabX, nItemGabY+MISSION_TOOLTIB_GAP_Y, it->Count); 
					}
					else 
					{
						g_pGameMain->SetItemInfo( 0, 0, 0, 0); 
					}
				}
				
				ZERO_MEMORY(buf);
				nItemGabX += MISSION_COMPEN_CAP_X;			// 아이콘 넒이
				nCount++;
			}
		}		
		it++;
	}
		
	for(int i=0; i<MAX_TAB_COMPENS; i++)
	{
		if(m_nCompensation[i] == 0)
			continue;
		
		char bufName[MAX_TAB_COMPENS][256];
		ZERO_MEMORY(bufName);

		switch(i)
		{
		case TAB_COMPENS_EXP:
			wsprintf(buf, "%08d", ICON_EXP_ITEMNUM);
			strcpy(bufName[TAB_COMPENS_EXP], STRMSG_C_050819_0001);	//"경험치:"
			break;
		case TAB_COMPENS_LEV:
			wsprintf(buf, "%08d", ICON_LEV_ITEMNUM);
			strcpy(bufName[TAB_COMPENS_LEV], STRMSG_C_050819_0002);	//"레벨:"
			break;
		case TAB_COMPENS_PRO:
			wsprintf(buf, "%08d", ICON_PRO_ITEMNUM);
			strcpy(bufName[TAB_COMPENS_PRO], STRMSG_C_050819_0003);	//"명성치:"
			break;
		case TAB_COMPENS_BUN:
			wsprintf(buf, "%08d", ICON_BUN_ITEMNUM);
			strcpy(bufName[TAB_COMPENS_BUN], STRMSG_C_050819_0004); //"보너스스탯:"
			break;
		}
		pSelectIcon = g_pGameMain->FindIcon(buf);
		pSelectIcon->Move(nItemGabX, nItemGabY);
		pSelectIcon->Render();		
		
		if( nCount < MAX_TAB_COMPENS )
		{
			if(m_bCompenToolTib[nCount] == TRUE)
			{
				ZERO_MEMORY(buf);
				wsprintf(buf, "%s%d", bufName[i], m_nCompensation[i]);
				g_pGameMain->RenderPopUpWindowImage(nItemGabX, nItemGabY+MISSION_TOOLTIB_GAP_Y,strlen(buf)*6.5, 1 );
				m_pCompensation->DrawText(nItemGabX, nItemGabY+MISSION_TOOLTIB_Y, GUI_FONT_COLOR_YM, buf);
			}
		}

		nItemGabX += MISSION_COMPEN_CAP_X;
		nCount++;
	}	
}

void CINFMissionTreeInfo::Tick()
{
#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	m_pINFAniCtrl->Tick();	
#endif
// 2008-12-09 by dgwoo 미션마스터.
	if(0 > m_fMMBtnTime)
		m_fMMBtnTime = MMASTER_BTN_DOWN_GAP;
	m_fMMBtnTime += g_pD3dApp->GetElapsedTime();

#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	if(g_pQuestData->GetProgressMission())
	{
		if( m_pStartMissionBtn->GetBtnState() != BTN_STATUS_DISABLE )
		{
			m_pStartMissionBtn->EnableBtn(FALSE);
		}
		if( m_pGoMissionBtn->GetBtnState() == BTN_STATUS_DISABLE )
		{
			m_pGoMissionBtn->EnableBtn(TRUE);
		}
		if( m_pGiveupMissionBtn->GetBtnState() == BTN_STATUS_DISABLE )
		{
			m_pGiveupMissionBtn->EnableBtn(TRUE);
		}
	}
	else
	{
		if( m_pStartMissionBtn->GetBtnState() == BTN_STATUS_DISABLE )
		{
			m_pStartMissionBtn->EnableBtn(TRUE);
		}
		if( m_pGoMissionBtn->GetBtnState() != BTN_STATUS_DISABLE )
		{
			m_pGoMissionBtn->EnableBtn(FALSE);
		}
		if( m_pGiveupMissionBtn->GetBtnState() != BTN_STATUS_DISABLE )
		{
			m_pGiveupMissionBtn->EnableBtn(FALSE);
		}
	}
#endif

	// 2013-03-26 by bhsohn [게임포지] 케릭터 일때만 Mission맵으로 이동 버튼 활성화
//#ifdef C_SHOW_MISSIONTOMAP
	if(g_pD3dApp->IsMyShuttleCharacter() == TRUE)
	{
		if( BTN_STATUS_DISABLE  == m_pGoMissionBtn->GetBtnState() )
		{
			m_pGoMissionBtn->EnableBtn(TRUE);
		}		
	}
	else
	{
		if( BTN_STATUS_DISABLE != m_pGoMissionBtn->GetBtnState() )
		{
			m_pGoMissionBtn->EnableBtn(FALSE);
		}
	}
//#endif
// END 2013-03-26 by bhsohn [게임포지] 케릭터 일때만 Mission맵으로 이동 버튼 활성화
}

int CINFMissionTreeInfo::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{	
	switch(uMsg)
	{
	case WM_MOUSEWHEEL:
		{
			return OnMouseWheel(wParam, lParam);
		}
		break;
	case WM_LBUTTONDOWN:
		{
			return OnLButtonDown(wParam, lParam);			
		}
		break;
	case WM_MOUSEMOVE:
		{
			return OnMouseMove(wParam, lParam);
		}
	case WM_LBUTTONUP:
		{
			return OnLButtonUp(wParam, lParam);
		}
	}
	return INF_MSGPROC_NORMAL;	
}

void CINFMissionTreeInfo::SelectRadioBtn(int nSelRadioBtn)
{
	m_nSelMission = nSelRadioBtn;
	BOOL bSelRadio[MAX_TAB_MISSION];
	int nCnt = 0;
	for(nCnt = 0;nCnt < MAX_TAB_MISSION; nCnt++)
	{
		bSelRadio[nCnt] = FALSE;
	}
	switch(m_nSelMission)
	{
	case TAB_MISSION_ALL:
		{
			bSelRadio[TAB_MISSION_ALL] = TRUE;
		}
		break;
	case TAB_MISSION_COMPLETE:
		{
			bSelRadio[TAB_MISSION_COMPLETE] = TRUE;
		}
		break;
	case TAB_MISSION_NOTCOMPLETE:
		{
			bSelRadio[TAB_MISSION_NOTCOMPLETE] = TRUE;
		}
		break;
	}
	for(nCnt = 0;nCnt < MAX_TAB_MISSION; nCnt++)
	{
		m_pMissionMode[nCnt]->SetRadioBtn(bSelRadio[nCnt]);
	}	
}

void CINFMissionTreeInfo::OnRadioButtonDown(int nRadioBtn)
{
	SelectRadioBtn(nRadioBtn);
	LoadListItem();
	m_pMissionListCtrl->RestoreItemDeviceObjects();
	m_pMissionListCtrl->UpdateItemPos();
}


int CINFMissionTreeInfo::OnLButtonDown(WPARAM wParam, LPARAM lParam)
{
	int nCnt = 0;
	POINT pt;
	pt.x = LOWORD(lParam);
	pt.y = HIWORD(lParam);
	CheckMouseReverse(&pt);

	// 라디오 버튼 체크
	for(nCnt = 0;nCnt <MAX_TAB_MISSION;nCnt++)
	{
		if(TRUE == m_pMissionMode[nCnt]->OnLButtonDown(pt))
		{
			OnRadioButtonDown(nCnt);
			return INF_MSGPROC_BREAK;
		}		
	}	

	// 리스트 컨트롤 체크
	int nQuestIndex = 0;
	int nSelMainIdx, nSelSubIdx;
	nSelMainIdx = nSelSubIdx = -1;
	{		
		if(TRUE == m_pMissionListCtrl->OnLButtonDown(pt, &nSelMainIdx, &nSelSubIdx))
		{		
			nQuestIndex = GetListCtrlSel_TO_QuestIndex(nSelMainIdx, nSelSubIdx);
			OnSelectMission(nQuestIndex);
			return INF_MSGPROC_BREAK;
		}
	}	

	// 리스트 박스
	{
		if(TRUE == m_pINFImageListBox->OnLButtonDown(pt))
		{			
			return INF_MSGPROC_BREAK;
		}
	}

	// 미션 시작 버튼 	
	{
		if(TRUE == m_pStartMissionBtn->OnLButtonDown(pt))
		{
			// 버튼위에 마우스가 있다.
			return  INF_MSGPROC_BREAK;
		}		
	}
	
	// 미션맵으로 이동		
	{
		if(TRUE == m_pGoMissionBtn->OnLButtonDown(pt))
		{
			// 버튼위에 마우스가 있다.
			return  INF_MSGPROC_BREAK;
		}		
	}

	// 미션 포기 버튼
	{
		if(TRUE == m_pGiveupMissionBtn->OnLButtonDown(pt))
		{			
			// 버튼위에 마우스가 있다.
			return  INF_MSGPROC_BREAK;
		}		
	}
// 2008-12-09 by dgwoo 미션마스터.
	// 미션 마스터.
	{
		if(TRUE == m_pBtnHelper->OnLButtonDown(pt))
		{
			return INF_MSGPROC_BREAK;
		}
		if(TRUE == m_pBtnPartyHelper->OnLButtonDown(pt))
		{
			
			return INF_MSGPROC_BREAK;
		}
		if(TRUE == m_pBtnMissionMaster->OnLButtonDown(pt))
		{
			
			return INF_MSGPROC_BREAK;
		}
		if(TRUE == m_pBtnMissionUnMaster->OnLButtonDown(pt))
		{
			
			return INF_MSGPROC_BREAK;
		}
	}
	
	// 닫기 창
	{
		if(TRUE == m_pCloseBtn->OnLButtonDown(pt))
		{
			// 버튼위에 마우스가 있다.
			return  INF_MSGPROC_BREAK;
		}		
	}
	// 맵정보 창
	{		
		if(TRUE == m_pINFMissionMapViewCtrl->OnLButtonDown(pt))
		{
			// 버튼위에 마우스가 있다.
			return  INF_MSGPROC_BREAK;
		}		
	}
	// 내 윈도우 영역이면 Return Break를 해줘야 케릭이동이 안된다. 
	{
		if((m_fBackPosX <= pt.x) &&(pt.x <= (m_fBackPosX+m_fBackWidth)))		
		{
			if((m_fBackPosX <= pt.y) &&( pt.y <= (m_fBackPosY + m_fBackHeight) ))
			{
				g_pInterface->SetWindowOrder(WNDMissionInfo);
				return INF_MSGPROC_BREAK;
			}
		}
	}

	return INF_MSGPROC_NORMAL;	
}
int CINFMissionTreeInfo::OnLButtonUp(WPARAM wParam, LPARAM lParam)
{
	POINT pt;
	pt.x = LOWORD(lParam);
	pt.y = HIWORD(lParam);
	CheckMouseReverse(&pt);
	// 리스트 컨트롤 체크
	{		
		if(TRUE == m_pMissionListCtrl->OnLButtonUp(pt))
		{			
			return INF_MSGPROC_BREAK;
		}
	}
	// 리스트 박스
	{
		if(TRUE == m_pINFImageListBox->OnLButtonUp(pt))
		{			
			return INF_MSGPROC_BREAK;
		}
	}

	// 미션 시작	
	{
		if(TRUE == m_pStartMissionBtn->OnLButtonUp(pt))
		{
			m_pParentMissionMain->MissionStart(m_dwSelQuestIndex);
			// 버튼 클릭 
			g_pD3dApp->m_pSound->PlayD3DSound(SOUND_SELECT_BUTTON, D3DXVECTOR3(0,0,0), FALSE);			
			return  INF_MSGPROC_BREAK;
		}
	}
	
	// 미션맵으로 이동	
	{
		if(TRUE == m_pGoMissionBtn->OnLButtonUp(pt))
		{
			OnClickGoMissionMap(m_dwSelQuestIndex);
			// 버튼 클릭 
			g_pD3dApp->m_pSound->PlayD3DSound(SOUND_SELECT_BUTTON, D3DXVECTOR3(0,0,0), FALSE);			
			return  INF_MSGPROC_BREAK;
		}
	}
	
	// 미션 포기 버튼
	{
		if(TRUE == m_pGiveupMissionBtn->OnLButtonUp(pt))
		{				
			OnClickGiveupMission(m_dwSelQuestIndex);
			// 버튼 클릭 
			g_pD3dApp->m_pSound->PlayD3DSound(SOUND_SELECT_BUTTON, D3DXVECTOR3(0,0,0), FALSE);			
			return  INF_MSGPROC_BREAK;
		}
	}
	// 2008-12-16 by dgwoo 미션 마스터.
	{
		if(TRUE == m_pBtnHelper->OnLButtonUp(pt))
		{
			MissionMasterHelpButton();
			return INF_MSGPROC_BREAK;
		}
		if(TRUE == m_pBtnPartyHelper->OnLButtonUp(pt))
		{
			MissionMasterPartyButton();
			return INF_MSGPROC_BREAK;
		}
		if(TRUE == m_pBtnMissionMaster->OnLButtonUp(pt))
		{
			MissionMasterButton();
			return INF_MSGPROC_BREAK;
		}
		if(TRUE == m_pBtnMissionUnMaster->OnLButtonUp(pt))
		{
			MissionMasterButton();
			return INF_MSGPROC_BREAK;
		}
	}
	
	// 닫기 창
	{
		if(TRUE == m_pCloseBtn->OnLButtonUp(pt))
		{					
			// 버튼 클릭 			
			m_pParentMissionMain->ShowTreeWnd(FALSE, TRUE);
			return  INF_MSGPROC_BREAK;
		}
	}
	// 맵정보 창
	{		
		if(TRUE == m_pINFMissionMapViewCtrl->OnLButtonUp(pt))
		{				
			return  INF_MSGPROC_BREAK;
		}
	}

	// 내 윈도우 영역이면 Return Break를 해줘야 케릭이동이 안된다. 
	{
		if((m_fBackPosX <= pt.x) &&(pt.x <= (m_fBackPosX+m_fBackWidth)))		
		{
			if((m_fBackPosX <= pt.y) &&( pt.y <= (m_fBackPosY + m_fBackHeight) ))
			{
				return INF_MSGPROC_BREAK;
			}
		}
	}

	
	return INF_MSGPROC_NORMAL;
}
// 2008-12-09 by dgwoo 미션마스터.
void CINFMissionTreeInfo::MissionMasterButton()
{// 미션 마스터 버튼 클릭.
	if(IsMissionMasterBtnAct())
	{// 미션 마스터 등록이 되어있는 상태라면.
		g_pGameMain->AddMsgBox(STRMSG_C_081202_0104,_Q_MISSION_MASTER_CANCEL);//"미션 마스터 등록을 해제 하시겠습니까?"				
	}
	else
	{// 미션 마스터로 등록이 되어있지 않은 상태.
		m_pParentMissionMain->SetMissionMasterWindowShow(TRUE);
	}
}
// 2008-12-09 by dgwoo 미션마스터.
BOOL CINFMissionTreeInfo::IsMissionMasterBtnAct()
{
	if(g_pShuttleChild->GetMyShuttleInfo().SecretInfoOption & USER_INFO_OPTION_MISSIONMASTER)
		return TRUE;
	return FALSE;
}
// 2008-12-09 by dgwoo 미션마스터.
void CINFMissionTreeInfo::MissionMasterHelpButton()
{
	if(MMASTER_BTN_DOWN_GAP < m_fMMBtnTime)
	{
		g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_081219_0102,COLOR_SYSTEM);
		CINFCommuPartyCre*	pPartyCre = g_pGameMain->m_pCommunity->GetPartyCreate();	// 편대생성
		MSG_FC_QUEST_REQUEST_MISSIONMASTER_HELP sMsg;
		sMsg.QuestIdx = m_dwSelQuestIndex;
		g_pFieldWinSocket->SendMsg(T_FC_QUEST_REQUEST_MISSIONMASTER_HELP,(char*)&sMsg,sizeof(MSG_FC_QUEST_REQUEST_MISSIONMASTER_HELP));
		pPartyCre->SetMissionMaster(FALSE);
		m_fMMBtnTime = 0;
	}
}
// 2008-12-09 by dgwoo 미션마스터.
void CINFMissionTreeInfo::MissionMasterPartyButton()
{
	if(g_pShuttleChild->GetPartyInfo().bPartyType == _NOPARTY)
	{
		CQuest * pQuest = g_pQuestData->FindQuest(m_dwSelQuestIndex);
		if(pQuest == NULL)
		{
			return;
		}
		g_pGameMain->m_pCommunity->GetPartyCreate()->MissionMasterParty(pQuest->QuestName);
		m_nMissionMasterQuestIdx = pQuest->QuestIndex;
		g_pGameMain->AddMsgBox(STRMSG_C_PARTY_0023, _Q_PARTY_CREATE);
	}
}
int CINFMissionTreeInfo::OnMouseMove(WPARAM wParam, LPARAM lParam)
{
	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(g_pD3dApp->GetHwnd(), &pt);
	CheckMouseReverse(&pt);

	// 미션시작버튼 
	{
		m_pStartMissionBtn->OnMouseMove(pt);	
	}
	
	// 미션맵으로 이동
	{
		m_pGoMissionBtn->OnMouseMove(pt);		
	}

	// 미션 포기 버튼
	{
		m_pGiveupMissionBtn->OnMouseMove(pt);		
	}
// 2008-12-09 by dgwoo 미션마스터.
	{
		if(pt.x > m_fBackPosX+MMASTER_REG_X &&
			pt.x < m_fBackPosX+MMASTER_REG_X + m_pBtnMissionMaster->GetImgSize().x && 
			pt.y > m_fBackPosY+MMASTER_REG_Y &&
			pt.y < m_fBackPosY+MMASTER_REG_Y + m_pBtnMissionMaster->GetImgSize().y)
		{
			g_pGameMain->SetToolTip(pt.x, pt.y,STRMSG_C_081219_0103);
			return INF_MSGPROC_BREAK;
		}
		else
		{
			g_pGameMain->SetToolTip(0,0,NULL);
		}

		m_pBtnHelper->OnMouseMove(pt);
		m_pBtnPartyHelper->OnMouseMove(pt);
	}
	// 닫기 창
	{
		m_pCloseBtn->OnMouseMove(pt);		
	}


	// 리스트 컨트롤 
	{		
		if(TRUE == m_pMissionListCtrl->OnMouseMove(pt))
		{			
			return INF_MSGPROC_BREAK;
		}
	}

	// 리스트 박스 
	{		
		if(TRUE == m_pINFImageListBox->OnMouseMove(pt))
		{			
			return INF_MSGPROC_BREAK;
		}
	}
	

	// 미션 보상 툴팁
	{
		if(TRUE == IsMouseMoveMissoinComp(pt))
		{
			return INF_MSGPROC_BREAK;
		}
	}

	// 맵 정보
	{		
		if(TRUE == m_pINFMissionMapViewCtrl->OnMouseMove(pt))
		{			
			return INF_MSGPROC_BREAK;
		}
	}

	// 내 윈도우 영역이면 Return Break를 해줘야 케릭이동이 안된다. 
	{
		if((m_fBackPosX <= pt.x) &&(pt.x <= (m_fBackPosX+m_fBackWidth)))		
		{
			if((m_fBackPosX <= pt.y) &&( pt.y <= (m_fBackPosY + m_fBackHeight) ))
			{
				return INF_MSGPROC_BREAK;
			}
		}
	}

	
	return INF_MSGPROC_NORMAL;	
}

BOOL CINFMissionTreeInfo::IsMouseMoveMissoinComp(POINT pt)
{

	int nItemGabX = m_fBackPosX+MISSION_COMPEN_X;
	int nItemGabY = m_fBackPosY+MISSION_COMPEN_Y;

	// 보상아이템 툴팁설정.
	g_pGameMain->SetItemInfo( 0, 0, 0, 0);	
	ZERO_MEMORY(m_bCompenToolTib);
	
	if(pt.y>nItemGabY && pt.y<(nItemGabY+MISSION_COMPEN_ONEITEM_HEIGHT))
	{
		for(int i=0;i<MAX_TAB_COMPENS;i++)
		{
			if(pt.x>nItemGabX+(i*MISSION_COMPEN_CAP_X) 
				&& pt.x<(nItemGabX+MISSION_COMPEN_ONEITEM_WIDTH+(i*MISSION_COMPEN_CAP_X)))
			{
				m_bCompenToolTib[i] = TRUE;
				return TRUE;
			}
		}
	}
	
	return FALSE;
}

int CINFMissionTreeInfo::OnMouseWheel(WPARAM wParam, LPARAM lParam)
{
	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(g_pD3dApp->GetHwnd(), &pt);
	CheckMouseReverse(&pt);
	int nCnt = 0;
	BOOL bClick = FALSE;	
	
	// 미션 리스트 컨트롤
	{
		bClick = m_pMissionListCtrl->OnMouseWheel(pt, wParam, lParam);
		if(bClick)		
		{		
			return INF_MSGPROC_BREAK;
		}
	}

	// 에디트 박스
	{
		bClick = m_pINFImageListBox->OnMouseWheel(pt, wParam, lParam);
		if(bClick)		
		{		
			return INF_MSGPROC_BREAK;
		}
	}

	// 맵 정보 창
	{		
		bClick = m_pINFMissionMapViewCtrl->OnMouseWheel(pt, wParam, lParam);
		if(bClick)		
		{		
			return INF_MSGPROC_BREAK;
		}
	}
	// 내 윈도우 영역이면 Return Break를 해줘야 케릭이동이 안된다. 
	{
		if((m_fBackPosX <= pt.x) &&(pt.x <= (m_fBackPosX+m_fBackWidth)))		
		{
			if((m_fBackPosX <= pt.y) &&( pt.y <= (m_fBackPosY + m_fBackHeight) ))
			{
				return INF_MSGPROC_BREAK;
			}
		}
	}

	return INF_MSGPROC_NORMAL;	
}

int	CINFMissionTreeInfo::GetListCtrlSel_TO_QuestIndex(int nSelMainIdx, int nSelSubIdx)
{
	int nQuestIndex = -1;
	if((-1 == nSelMainIdx) || (-1 == nSelSubIdx))
	{
		return nQuestIndex;
	}
	int nCnt =0;
	int nMainId = 0;
	for(nMainId = 0;nMainId < MAX_TAB_MISSION_MODE_EP;nMainId++)
	{
		for(nCnt =0;nCnt <MAX_TAB_MISSION_MODE;nCnt++)
		{
			vector<structQuestInfo>::iterator it = m_vecQuestInfo[nMainId][nCnt].begin();
			while(it != m_vecQuestInfo[nMainId][nCnt].end())
			{
				structQuestInfo	 stQuestinfo = (*it);
				if((nSelMainIdx == stQuestinfo.nMainIdx) && (nSelSubIdx == stQuestinfo.nSubIdx))
				{
					return stQuestinfo.nQuestIndex;
				}
				
				it++;
			}
		}
	}

	return nQuestIndex;

}

// 퀘스트 선택 
void CINFMissionTreeInfo::OnSelectMission(int nQuestIndex)
{	
	if(nQuestIndex < 0)
	{
		// 에러 퀘스트
		return;
	}
	
	// 미션 정보 초기화
	InitMissionInfo(FALSE);
	
	CQuest* pQuest = g_pQuestData->FindQuest(nQuestIndex);
	if(NULL == pQuest)
	{
		return;
	}

	BOOL bErrorQuest = FALSE;
	CQuest* pQuestErr = g_pQuestData->GetProgressMission();
	// 에러 퀘스트
	if(CheckProgressMission(pQuest, &m_vecErrPart, &m_vecErrQuestName, &m_vecErritem) == FALSE)
	{
		if(pQuestErr && pQuestErr->QuestIndex == pQuest->QuestIndex)
		{
		}
		else
		{
			bErrorQuest = TRUE;
			// 에러 스트링 관련 처리
			SetErrorString(pQuest, &m_vecPossibleDesc, &m_vecErrPart, &m_vecErrQuestName, &m_vecErritem);
			
		}
	}

	// 퀘스트 이름 설정
	// 2007-10-04 by bhsohn 베트남 글씨 넘어가는 문제 해결
	//m_pParentMissionMain->SetQuestNameString(pQuest, &m_vecMissionName, m_pINFImageListBox, TREEMISSION_MAX_DESC_STRING);	
	m_pParentMissionMain->SetQuestNameString(pQuest, &m_vecMissionName, m_pINFImageListBox, TREEMISSION_TITLE_MAX_DESC_STRING);	
	

	
#ifdef _DEBUG
	// 보상
	SetQuestPay(pQuest, &m_vecCompensationItemName, m_nCompensation);	
	
	if(FALSE == bErrorQuest)
	{
		// 퀘스트 관련되서 에러가 없을때만 에러를 찍는다. 
		// 미션결과
		m_pParentMissionMain->SetQuestResult(pQuest, &m_vecPossibleDesc, m_pINFImageListBox, TREEMISSION_MAX_DESC_STRING);
	}	

	m_pParentMissionMain->SetGoalInfo(pQuest, &m_vecPossibleDesc);
	
	m_pINFMissionMapViewCtrl->UpdateMapInfo(pQuest, pQuest->StartMapIndex);		
#else
	if(FALSE == bErrorQuest)
	{
		// 보상
		SetQuestPay(pQuest, &m_vecCompensationItemName, m_nCompensation);

		// 퀘스트 관련되서 에러가 없을때만 에러를 찍는다. 
		// 미션결과
		m_pParentMissionMain->SetQuestResult(pQuest, &m_vecPossibleDesc, m_pINFImageListBox, TREEMISSION_MAX_DESC_STRING);

		m_pParentMissionMain->SetGoalInfo(pQuest, &m_vecPossibleDesc);

		m_pINFMissionMapViewCtrl->UpdateMapInfo(pQuest, pQuest->StartMapIndex);		
	}
#endif
#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	m_pINFAniCtrl->InitAnimation(FALSE);
	
	// 애니메이션 시작
	m_pINFAniCtrl->StartAnimation(bErrorQuest);
#endif

	m_dwSelQuestIndex = nQuestIndex;	

	// 에디트 박스에 업데이트 
	m_pINFImageListBox->SetTitleText(m_vecMissionName);
	m_pINFImageListBox->SetEditText(m_vecPossibleDesc);

	// 스크롤 바 위치 갱신 
	m_pINFImageListBox->UpdateScrollPos();		

	// 버튼 Show 여부 
	BOOL bShowStartBtn,  bShowGoMission, bShowGiveup;
	bShowStartBtn = bShowGoMission = bShowGiveup = FALSE;
	
	// 진행중인 미션이 없다.
	if(NULL == pQuestErr
		&&(FALSE == g_pQuestData->IsQuestCompleted(nQuestIndex)))
	{
		bShowStartBtn = TRUE;		
	}
	else if(pQuestErr 
		&& (pQuestErr->QuestIndex == pQuest->QuestIndex))
	{		
		bShowGiveup = TRUE;
		if(TRUE == g_pD3dApp->IsMyShuttleCharacter()
			&& (pQuest->StartMapIndex > 0))
		{
			//마을이다.
			bShowGoMission = TRUE;
		}		
	}

	if(pQuest->IsPartyQuest()
		&& g_pShuttleChild->GetPartyInfo().bPartyType != _PARTYMASTER)
	{
		bShowGiveup = bShowStartBtn = bShowGoMission = FALSE;
	}
	else if(TRUE == bErrorQuest)
	{
		// 에러 퀘스트
		bShowGiveup = bShowStartBtn = bShowGoMission = FALSE;
	}
#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경	
	m_pStartMissionBtn->ShowWindow(bShowStartBtn);
	m_pGoMissionBtn->ShowWindow(bShowGoMission);
	m_pGiveupMissionBtn->ShowWindow(bShowGiveup);	
#endif
	
	// 2008-12-09 by dgwool 미션 마스터.
	BOOL bHelper,bPartyHelper;
	bHelper = bPartyHelper = FALSE;
	if(pQuest->QuestPartyType == QUEST_PARTYTYPE_PARTY 
		&& (FALSE == bErrorQuest)) // 2008-01-06 by bhsohn 편대 미션 버그 수정
	{
		// 레벨이 안되는 미션은 버튼이 보여면 안된다.
		if(g_pShuttleChild->GetPartyInfo().bPartyType == _NOPARTY)
		{// 파티중이 아니라면.
			bPartyHelper = TRUE;
		}
		else
		{
			bHelper = TRUE;
		}
	}
#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경	
	if(bShowGoMission)
	{
		m_pBtnHelper->SetBtnPosition(m_fBackPosX+MMASTER_HELPER_X,m_fBackPosY+MMASTER_HELPER_Y);
		m_pBtnPartyHelper->SetBtnPosition(m_fBackPosX+MMASTER_HELPER_PARTY_X,m_fBackPosY+MMASTER_HELPER_PARTY_Y);
	}
	else
	{
		m_pBtnHelper->SetBtnPosition(m_fBackPosX+MMASTER_HELPER_MX,m_fBackPosY+MMASTER_HELPER_MY);
		m_pBtnPartyHelper->SetBtnPosition(m_fBackPosX+MMASTER_HELPER_PARTY_MX,m_fBackPosY+MMASTER_HELPER_PARTY_MY);
	}
#endif
	
	// 2010. 02. 22 by ckPark 완료된 미션에 대해서 미션마스터 버튼이 보이는 버그 수정
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
 	m_pBtnHelper->EnableBtn(bHelper);
 	m_pBtnPartyHelper->EnableBtn(bPartyHelper);													  
#endif
	// 2010. 06. 29 by jskim 완료된 미션에 대해서 미션마스터 버튼이 보이는 버그 수정
	//if( !g_pQuestData->IsQuestCompleted(nQuestIndex) )
	// 	{
	// 		m_pBtnHelper->ShowWindow( bHelper );
	// 		m_pBtnPartyHelper->ShowWindow( bPartyHelper );
	// 	}
	// 	else
	// 	{
	// 		m_pBtnHelper->ShowWindow( FALSE );
	// 		m_pBtnPartyHelper->ShowWindow( FALSE );
	// 	}
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	if( TRUE == g_pQuestData->IsQuestCompleted(nQuestIndex) || NULL != g_pD3dApp->SerchMissionCondition() )
	{		  
 		m_pBtnHelper->EnableBtn( FALSE );
 		m_pBtnPartyHelper->EnableBtn( FALSE );
 		m_pGiveupMissionBtn->EnableBtn( FALSE );
	}
	else
	{
 		m_pBtnHelper->EnableBtn( bHelper );
 		m_pBtnPartyHelper->EnableBtn( bPartyHelper );
	}
#else		  
	if( TRUE == g_pQuestData->IsQuestCompleted(nQuestIndex) || NULL != g_pD3dApp->SerchMissionCondition() )
	{
		m_pBtnHelper->ShowWindow( FALSE );
		m_pBtnPartyHelper->ShowWindow( FALSE );
		m_pGiveupMissionBtn->ShowWindow( FALSE );
	}
	else
	{
		m_pBtnHelper->ShowWindow( bHelper );
		m_pBtnPartyHelper->ShowWindow( bPartyHelper );
	}
#endif
	//end 2010. 06. 29 by jskim 완료된 미션에 대해서 미션마스터 버튼이 보이는 버그 수정
	// end 2010. 02. 22 by ckPark 완료된 미션에 대해서 미션마스터 버튼이 보이는 버그 수정
}


void CINFMissionTreeInfo::SetQuestPay(CQuest* pQuest, vector<QUEST_PAY_ITEM>	*o_vecCompensationItemName, int *o_nCompensation)
{	
	vector<QUEST_PAY_ITEM>::iterator it = pQuest->EndPayItemVector.begin();
	for(; it != pQuest->EndPayItemVector.end(); it++)
	{
		int nCount = 0;
		QUEST_PAY_ITEM* pPayItem = &*it;
		vector<QUEST_PAY_ITEM>::iterator it2 = m_vecCompensationItemName.begin();

		o_vecCompensationItemName->push_back(*pPayItem);		
	}

	if(pQuest->EndPayPropensity != 0)
	{ // 명성
		o_nCompensation[TAB_COMPENS_PRO] = pQuest->EndPayPropensity;
	}
	if(pQuest->EndPayLevel != 0)
	{ // 레벨
		o_nCompensation[TAB_COMPENS_LEV] = pQuest->EndPayLevel;
	}
	if(pQuest->EndPayExperience != 0)
	{ // 경험치
		o_nCompensation[TAB_COMPENS_EXP] = pQuest->EndPayExperience;
	}
	if(pQuest->EndPayBonusStat != 0)
	{ // 보너스 스탯
		o_nCompensation[TAB_COMPENS_BUN] = pQuest->EndPayBonusStat;
	}
	
}



void CINFMissionTreeInfo::SetErrorString(CQuest* i_pQuest, 
											vector<string>* o_vecPossibleDesc,
											vector<int>* o_vecErrPart, 
											vector<string>*  o_vecErrQuestName,
											vector<string>* o_vecErritem)
{
	BOOL bErrorString = FALSE;
	// 에러 설명 부분 (진행할수 없는 미션)
	vector<int>::iterator itErr = o_vecErrPart->begin();
	while(itErr != o_vecErrPart->end())
	{		
		switch(*itErr)
		{
		case TREEMISSION_ERR_NONE:
			break;
		case TREEMISSION_ERR_LEVEL:
			{					
				// 2007-01-22 by bhsohn 미션 인터 페이스 관련 수정안
				//o_vecPossibleDesc->push_back(STRMSG_C_050825_0002); //"레벨이 부족합니다."
				char buf[512];
				ZERO_MEMORY(buf);						
				wsprintf(buf, STRMSG_C_070627_0204, i_pQuest->ReqLevel.Min);		//"%d레벨 부터 가능한 미션 입니다."
				//o_vecPossibleDesc->push_back(buf); 
				m_pINFImageListBox->SetStringCull(1, buf, o_vecPossibleDesc, TREEMISSION_MAX_DESC_STRING);
			}
			break;
		case TREEMISSION_ERR_STAT_AT:
			o_vecPossibleDesc->push_back(STRMSG_C_050825_0003); //"스탯중 공격파트가 부족합니다."
			break;
		case TREEMISSION_ERR_STAT_DE:
			o_vecPossibleDesc->push_back(STRMSG_C_050825_0004); //"스탯중 방어파트가 부족합니다."
			break;
		case TREEMISSION_ERR_STAT_FU:
			o_vecPossibleDesc->push_back(STRMSG_C_050825_0005); //"스탯중 연료파트가 부족합니다."
			break;
		case TREEMISSION_ERR_STAT_SO:
			o_vecPossibleDesc->push_back(STRMSG_C_050825_0006); //"스탯중 정신파트가 부족합니다."
			break;
		case TREEMISSION_ERR_STAT_SH:
			o_vecPossibleDesc->push_back(STRMSG_C_050825_0007); //"스탯중 쉴드파트가 부족합니다."
			break;
		case TREEMISSION_ERR_STAT_DO:
			o_vecPossibleDesc->push_back(STRMSG_C_050825_0008); // "스탯중 회피파트가 부족합니다."
			break;
		case TREEMISSION_ERR_ITEM:
			{
				char buf[512];
				ZERO_MEMORY(buf);
				strcpy(buf, STRMSG_C_050825_0009); //"아이템 \\r"
				vector<string>::iterator itItem = o_vecErritem->begin();
				while(itItem != o_vecErritem->end())
				{
					strcat(buf, (char*)itItem->c_str());
					strcat(buf, " ");
					itItem++;						
				}
				strcat(buf, STRMSG_C_050825_0010); //"\\r가(이) 필요합니다."
				//STRING_CULL(buf, TREEMISSION_MAX_DESC_STRING, o_vecPossibleDesc, m_pMissionFontRight[1]);
				m_pINFImageListBox->SetStringCull(1, buf, o_vecPossibleDesc, TREEMISSION_MAX_DESC_STRING);
			}
			break;
		case TREEMISSION_ERR_FLUENCE:
			o_vecPossibleDesc->push_back(STRMSG_C_050825_0011); //"세력이 맞지않습니다."
			break;
		case TREEMISSION_ERR_EXPERIENCE:
			o_vecPossibleDesc->push_back(STRMSG_C_050825_0012); //"경험치가 부족합니다"
			break;
		case TREEMISSION_ERR_QUEST:
			{
				char buf[512];
				ZERO_MEMORY(buf);
				strcpy(buf, STRMSG_C_070627_0205); //"미션 \\r"
				vector<string>::iterator itItem = o_vecErrQuestName->begin();
				while(itItem != o_vecErrQuestName->end())
				{
					strcat(buf, (char*)itItem->c_str());
					strcat(buf, " ");
					itItem++;						
				}
				strcat(buf, STRMSG_C_070627_0206); //"\\r을(를) 완료해야 진행 할 수 있습니다."
				//STRING_CULL(buf, TREEMISSION_MAX_DESC_STRING, o_vecPossibleDesc, m_pMissionFontRight[1]);
				m_pINFImageListBox->SetStringCull(1, buf, o_vecPossibleDesc, TREEMISSION_MAX_DESC_STRING);
			}
			break;
		case TREEMISSION_ERR_HIDEQUEST:
			{
				o_vecPossibleDesc->push_back(STRMSG_C_050825_0015); //"숨겨진 미션"
				o_vecPossibleDesc->push_back(STRMSG_C_050825_0016); //"\\r특정 조건\\r을 완료 하세요"
			}
			break;
		}
		itErr++;
	}	
}

// 미션맵으로 이동
void CINFMissionTreeInfo::OnClickGoMissionMap(DWORD i_dwSelQuestIndex)
{
	CQuest* pProgressMission = g_pQuestData->GetProgressMission();
	if(NULL == pProgressMission)
	{
		return;
	}
	CQuest* pQuest = g_pQuestData->FindQuest(i_dwSelQuestIndex);
	if(NULL == pQuest)
	{
		return;
	}
	if(pProgressMission->QuestIndex != pQuest->QuestIndex)
	{
		return;
	}

	// 파티 퀘스트 관련 처리 
	BOOL bPartyQuest = ((!pQuest->IsPartyQuest())
						|| (pQuest->IsPartyQuest() && g_pShuttleChild->GetPartyInfo().bPartyType == _PARTYMASTER));

	if(FALSE == bPartyQuest)
	{
		return;
	}
	
	if(FALSE == g_pD3dApp->IsMyShuttleCharacter())
	{
		// 캐릭터 모드가 아니면 이동 안함.
		return;
	}
	// 미션 맵으로 이동
	g_pGameMain->GoWarpMapChange(pProgressMission->QuestIndex);	

	// 2008-06-17 by bhsohn 편대 관련 처리
	// 미션 도우미시 강제로 자유 비행으로
	if(g_pShuttleChild->m_pClientParty 
		&& g_pShuttleChild->m_pClientParty->GetPartyInfo().bPartyType == _PARTYMASTER 
		&& g_pShuttleChild->m_pClientParty->IsFormationFlight())
	{
		g_pShuttleChild->m_pClientParty->TempPartyFormation(FLIGHT_FORM_NONE);
	}
	// end 2008-06-17 by bhsohn 편대 관련 처리
}



void CINFMissionTreeInfo::SetMissionEnterShop(BOOL bEnterShopState)
{

}

int CINFMissionTreeInfo::HideMissionCheckProgressMission(CQuest*	 i_pHidePosQuest)
{
	if(NULL == i_pHidePosQuest)
	{
		return FALSE;
	}
	m_vecMissionName.clear();	
	m_vecPossibleDesc.clear();			// 미션 설명	
	m_vecCompensationItemName.clear();	// 미션 보상아이템 명칭

	return CheckProgressMission(i_pHidePosQuest, &m_vecErrPart, &m_vecErrQuestName, &m_vecErritem);
}

void CINFMissionTreeInfo::RefreshMission(BOOL bClick, INT QuestIndex)
{
	InitBtn();
	
	// 진형정보
#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	m_pINFAniCtrl->SetMAniInfluence(g_pGameMain->GetMyShuttleInfo().InfluenceType);
#endif

	// 선택한 미션
	OnRadioButtonDown(m_nSelMission);
	if(0 != QuestIndex)
	{
		OnSelectListCtrl(QuestIndex);
		OnSelectMission(QuestIndex);	
	}
	else if(TRUE == bClick)
	{	
		// 클릭으로 들어옴.
		InitMissionInfo(bClick);			
	}
	
}

// 미션 정보 초기화
void CINFMissionTreeInfo::InitMissionInfo(BOOL bFirstPopup)
{
	m_dwSelQuestIndex = 0;
	m_vecMissionName.clear();	
	m_vecPossibleDesc.clear();			// 미션 설명	
	m_vecCompensationItemName.clear();	// 미션 보상아이템 명칭
	ZERO_MEMORY(m_nCompensation);
	m_pINFMissionMapViewCtrl->UpdateMapInfo(NULL, 0);
	m_pINFImageListBox->ResetContent();	// 리스트 박스 정보 초기화 

#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	m_pStartMissionBtn->ShowWindow(FALSE);
	m_pGoMissionBtn->ShowWindow(FALSE);
	m_pGiveupMissionBtn->ShowWindow(FALSE);	
#endif
	UpdateMissionMasterButton();

// end 2008-12-09 by dgwoo 미션마스터.
	// 클릭으로 들어오면 초기화 아니면 초기화 하지안음
#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	m_pINFAniCtrl->InitAnimation(bFirstPopup);
#endif
}
void CINFMissionTreeInfo::UpdateMissionMasterButton()
{
// 2008-12-09 by dgwoo 미션마스터.
	// 미션 마스터 버튼 셋팅.
	if(g_pQuestData->GetMissionMasterQuestCompletedCount())
	{
		m_pBtnMissionMaster->EnableBtn(TRUE);
		m_pBtnMissionUnMaster->EnableBtn(TRUE);
	}
	else
	{
		m_pBtnMissionMaster->EnableBtn(FALSE);
		m_pBtnMissionUnMaster->EnableBtn(FALSE);
	}
	if(IsMissionMasterBtnAct())
	{
		m_pBtnMissionMaster->ShowWindow(FALSE);
		m_pBtnMissionUnMaster->ShowWindow(TRUE);
	}
	else
	{
		m_pBtnMissionMaster->ShowWindow(TRUE);
		m_pBtnMissionUnMaster->ShowWindow(FALSE);
	}							  
	CQuest *pQuest = NULL;
	pQuest = g_pQuestData->FindQuest(m_dwSelQuestIndex);
	if(pQuest == NULL)
		return;
	BOOL bHelper,bPartyHelper;
	bHelper = bPartyHelper = FALSE;

	// 2008-01-06 by bhsohn 편대 미션 버그 수정
	BOOL bErrorQuest = FALSE;
	CQuest* pQuestIng = g_pQuestData->GetProgressMission();
	// 에러 퀘스트
	if(CheckProgressMission(pQuest, &m_vecErrPart, &m_vecErrQuestName, &m_vecErritem) == FALSE)
	{
		if(pQuestIng && pQuestIng->QuestIndex == pQuest->QuestIndex)
		{
		}
		else
		{
			bErrorQuest = TRUE;		
		}
	}
	// end 2008-01-06 by bhsohn 편대 미션 버그 수정

	if(pQuest->QuestPartyType == QUEST_PARTYTYPE_PARTY
		&& (FALSE == bErrorQuest)) // 2008-01-06 by bhsohn 편대 미션 버그 수정
	{
		if(g_pShuttleChild->GetPartyInfo().bPartyType == _NOPARTY)
		{// 파티중이 아니라면.
			bPartyHelper = TRUE;
		}
		else
		{
			bHelper = TRUE;
		}
	}
//	if(bShowGoMission)
//	{
//		m_pBtnHelper->SetBtnPosition(m_fBackPosX+MMASTER_HELPER_X,m_fBackPosY+MMASTER_HELPER_Y);
//		m_pBtnPartyHelper->SetBtnPosition(m_fBackPosX+MMASTER_HELPER_PARTY_X,m_fBackPosY+MMASTER_HELPER_PARTY_Y);
//	}
//	else
//	{
//		m_pBtnHelper->SetBtnPosition(m_fBackPosX+MMASTER_HELPER_MX,m_fBackPosY+MMASTER_HELPER_MY);
//		m_pBtnPartyHelper->SetBtnPosition(m_fBackPosX+MMASTER_HELPER_PARTY_MX,m_fBackPosY+MMASTER_HELPER_PARTY_MY);
//	}
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	m_pBtnHelper->EnableBtn(bHelper);
	m_pBtnPartyHelper->EnableBtn(bPartyHelper);													  
#else 
	m_pBtnHelper->ShowWindow(bHelper);
	m_pBtnPartyHelper->ShowWindow(bPartyHelper);
#endif


}
void CINFMissionTreeInfo::OnSelectListCtrl(INT nSelQuestIndex)
{
	int nSelEpIdx, nMainIdx, nSubIdx;
	nSelEpIdx = nMainIdx = nSubIdx = -1;

	int nCnt =0;
	int nMainId = 0;
	BOOL bBreak = FALSE;
	for(nMainId = 0;nMainId < MAX_TAB_MISSION_MODE_EP;nMainId++)
	{
		if(bBreak)
		{
			break;
		}
		for(nCnt =0;nCnt <MAX_TAB_MISSION_MODE;nCnt++)
		{
			vector<structQuestInfo>::iterator it = m_vecQuestInfo[nMainId][nCnt].begin();
			while(it != m_vecQuestInfo[nMainId][nCnt].end())
			{
				structQuestInfo	 stQuestinfo = (*it);
				if(nSelQuestIndex == stQuestinfo.nQuestIndex)
				{
					nMainIdx = stQuestinfo.nMainIdx;
					nSubIdx = stQuestinfo.nSubIdx;
					nSelEpIdx = nMainId;
					bBreak = TRUE;
					break;
				}
				it++;
			}
		}
	}
	
	if(-1 == nMainIdx || -1 == nSubIdx )
	{
		return;
	}
	m_pMissionListCtrl->SetSelPoint(nMainIdx, nSubIdx);
	m_pMissionListCtrl->PopupItem(TRUE, nMainIdx, FALSE, nSelEpIdx);
	m_pMissionListCtrl->ShowTitleAllEpId(TRUE, nSelEpIdx);	// 서브 타이틀을 보여준다.
}
void CINFMissionTreeInfo::OnClickGiveupMission(DWORD dwSelQuestIndex)
{
	CQuest* pSelQuest = g_pQuestData->FindQuest(dwSelQuestIndex);
	CQuest* pProgressMission = g_pQuestData->GetProgressMission();
	if(NULL == pSelQuest )
	{
		g_pGameMain->AddMsgBox(STRMSG_C_060720_0002, _MESSAGE);
		return;
	}
	if(NULL == pProgressMission )
	{
		g_pGameMain->AddMsgBox(STRMSG_C_060720_0002, _MESSAGE);
		return;
	}
	if(pSelQuest->QuestIndex != pProgressMission->QuestIndex)
	{
		g_pGameMain->AddMsgBox(STRMSG_C_060720_0002, _MESSAGE);
		return;
	}

	// 시나리오 미션이면서 포기불가능한 미션
	if((pSelQuest->QuestKind == QUEST_KIND_SCENARIO) && (pSelQuest->IsDiscardable == 0))
	{
		g_pGameMain->AddMsgBox(STRMSG_C_060720_0001, _MESSAGE);
		return;
	}

	g_pInterface->SetWindowOrder(WNDMissionInfo);
	g_pGameMain->AddMsgBox(STRMSG_C_QUEST_0015,_Q_QUEST_DISCARD,
							dwSelQuestIndex);//"해당 미션을 포기하시겠습니까?."					
}

void CINFMissionTreeInfo::SelectNextMission(INT nNextQuestIndex)
{	
	int nMainId = 0;	
	int nMainIdx, nSubIdx, nSelEpIdx;
	BOOL bBreak = FALSE;
	nSelEpIdx = nMainIdx = nSubIdx = -1;
	for(nMainId = 0;nMainId < MAX_TAB_MISSION_MODE_EP;nMainId++)	
	{			
		if(bBreak )
		{
			break;
		}
		vector<structQuestInfo>::iterator it = m_vecQuestInfo[nMainId][TAB_MISSION_MODE_SCEN].begin();		
		while(it != m_vecQuestInfo[nMainId][TAB_MISSION_MODE_SCEN].end())
		{
			structQuestInfo	 stQuestinfo = (*it);
			if(nNextQuestIndex == stQuestinfo.nQuestIndex)
			{
				nMainIdx = stQuestinfo.nMainIdx;
				nSubIdx = stQuestinfo.nSubIdx;
				nSelEpIdx = nMainId;
				bBreak = TRUE;
				break;
			}		
			it++;
		}
	}
	
	if(-1 == nMainIdx || -1 == nSubIdx)
	{
		return;
	}

	m_pMissionListCtrl->SetSelPoint(nMainIdx, nSubIdx);	
	OnSelectMission(nNextQuestIndex);
	m_pMissionListCtrl->PopupItem(TRUE, nMainIdx, FALSE, nSelEpIdx);
	m_pMissionListCtrl->ShowTitleAllEpId(TRUE, nSelEpIdx);	// 서브 타이틀을 보여준다.

}

void CINFMissionTreeInfo::InitBtn()
{
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	m_pStartMissionBtn->ShowWindow(TRUE);
	m_pGoMissionBtn->ShowWindow(TRUE);
	m_pGiveupMissionBtn->ShowWindow(TRUE);	
// 2008-12-09 by dgwoo 미션마스터.
	m_pBtnHelper->ShowWindow(TRUE);
	m_pBtnPartyHelper->ShowWindow(TRUE);
	m_pBtnHelper->EnableBtn(FALSE);
	m_pBtnPartyHelper->EnableBtn(FALSE);
	//m_pINFAniCtrl->InitAnimation(FALSE);
#else 
	m_pStartMissionBtn->ShowWindow(FALSE);
	m_pGoMissionBtn->ShowWindow(FALSE);
	m_pGiveupMissionBtn->ShowWindow(FALSE);	
// 2008-12-09 by dgwoo 미션마스터.
	m_pBtnHelper->ShowWindow(FALSE);
	m_pBtnPartyHelper->ShowWindow(FALSE);
	m_pINFAniCtrl->InitAnimation(FALSE);
#endif
}
