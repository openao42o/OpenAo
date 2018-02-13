// INFCommunityGuild.cpp: implementation of the CINFCommunityGuildGuild class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "INFCommunityGuild.h"
#include "AtumApplication.h"
#include "INFImageFile.h"
#include "INFImage.h"
#include "D3DHanFont.h"
#include "INFScrollBar.h"
#include "INFComboBox.h"

// 2007-11-22 by bhsohn 아레나 통합서버
#include "IMSocketManager.h"
//#include "IMSocket.h"

#include "Chat.h"
#include "ShuttleChild.h"
#include "CharacterChild.h"				// 2005-07-21 by ispark
#include "GameDataLast.h"
#include "INFGameMain.h"
#include "AtumDatabase.h"
#include "INFWindow.h"
#include "INFTarget.h"
#include "INFGameMainChat.h"
#include "INFCommunity.h"
#include "ClientParty.h"

#include "EnemyData.h"
#include "SceneData.h"
#include "dxutil.h"

// 2008-04-04 by bhsohn Ep3 커뮤니티 창
#include "INFImageBtn.h"
#include "AtumSound.h"
#include "INFPopupMenu.h"

#include "INFArenaScrollBar.h"
#include "INFImageComboBox.h"
//#include "INFEditBox.h"
#include "INFCurselEditBox.h"	// 2008-10-29 by bhsohn 에디트 박스 형태 변경

#include "INFImageEx.h"								 // 2011. 10. 10 by jskim UI시스템 변경
#include "INFGroupImage.h"
#include "INFGroupManager.h"
#include "Interface.h"
#include "INFToolTip.h"			// 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현

// 2015-11-2, PanKJ FreeWar
#include "FreeWar.h"

//#define SCROLL_START_X						197
//#define SCROLL_START_Y						71
#define SCROLL_START_X						494
#define SCROLL_START_Y						120

//#define SCROLL_BAR_SIZE_X					11
//#define SCROLL_BAR_SIZE_Y					30
#define SCROLL_LINE_LENGTH					154
//#define SCROLL_INTERVAL(count)				(count==0 ? 0:(SCROLL_LINE_LENGTH/count))
#define LIST_BOX_START_X					19
#define LIST_BOX_START_Y					72
#define LIST_BOX_SIZE_X						171
#define LIST_BOX_SIZE_Y						154

#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경

#define COMBO_SIZE_Y						15
#define COMBO_SIZE_X						100

#define GUILD_BTN_START_X					63
#define GUILD_BTN_START_Y					47

#define BACK_START_X						13
#define BACK_START_Y						25
#define GUILD_MARK_START_X					19
#define GUILD_MARK_START_Y					49
#define GUILD_MARK_SIZE_X					24
#define GUILD_MARK_SIZE_Y					12

#define BUTTON_MAKE_START_X					19	// 길드원인 경우 탈퇴 버튼
#define BUTTON_MAKE_START_Y					231
#define BUTTON_LEAVE_START_X				BUTTON_MAKE_START_X	// 탈퇴
#define BUTTON_LEAVE_START_Y				BUTTON_MAKE_START_Y
#define BUTTON_INVITE_START_X				BUTTON_MAKE_START_X	// 초대
#define BUTTON_INVITE_START_Y				BUTTON_MAKE_START_Y
#define BUTTON_CANCEL_START_X				BUTTON_MAKE_START_X	// 초대
#define BUTTON_CANCEL_START_Y				BUTTON_MAKE_START_Y
#define BUTTON_MARK_START_X					56	// 마크
#define BUTTON_MARK_START_Y					BUTTON_MAKE_START_Y
#define BUTTON_BANMEMBER_START_X			93	// 탈퇴
#define BUTTON_BANMEMBER_START_Y			BUTTON_MAKE_START_Y
#define BUTTON_DISBAND_START_X				130 // 해체
#define BUTTON_DISBAND_START_Y				BUTTON_MAKE_START_Y
#define BUTTON_LEAVE_START_X				BUTTON_MAKE_START_X // 탈퇴
#define BUTTON_LEAVE_START_Y				BUTTON_MAKE_START_Y

#define BUTTON_VOIP_START_X					167
#define BUTTON_VOIP_START_Y					BUTTON_MAKE_START_Y
#define VOIP_BUTTON_STATE_NORMAL			0
#define VOIP_BUTTON_STATE_UP				1
#define VOIP_BUTTON_STATE_DOWN				2
#define VOIP_BUTTON_STATE_DISABLE			3
#define VOIP_WIDTH							35
#define VOIP_HEIGHT							16



#define BUTTON_SIZE_X						35
#define BUTTON_SIZE_Y						16
#define BUTTON_MIDDLE_SIZE_X				56
#define BUTTON_BIG_SIZE_X					72


//#define GUILD_NAME_START_X					53
#define GUILD_NAME_START_X					62
#define LIST_BOX_NAME_START_X				190

//#define LIST_BOX_GUILDRANK_START_X			140//138
//#define LIST_BOX_GUILDRANK_START_Y			74//75
#define LIST_BOX_GUILDRANK_START_X			379
#define LIST_BOX_GUILDRANK_START_Y			115

//#define LIST_BOX_ONOFF_START_X				161
#define LIST_BOX_ONOFF_START_X				458
#define LIST_BOX_NUMBER						8
#define LIST_BOX_INTERVAL					18
#define GUILD_MEMBER_ONLINE_COLOR			GUI_FONT_COLOR
#define GUILD_MEMBER_OFFLINE_COLOR			RGB(222, 0, 0)
#define GUI_FONT_COLOR_DISMEMBER_READY		RGB(150, 150, 150)
//#define MARK_START_X						21
//#define MARK_START_Y						53
#define MARK_START_X						24
#define MARK_START_Y						51


// 2006-03-07 by ispark, 언어에 따라 위치 수정
//#if defined(LANGUAGE_ENGLISH) || defined(LANGUAGE_VIETNAM)
//#define LIST_BOX_NAME_START_Y				72//75
//#define LIST_BOX_ONOFF_START_Y				72//75
//#define LIST_GUILD_ALL_Y					50
//#define GUILD_NAME_START_Y					50
//#else
//#define LIST_BOX_NAME_START_Y				72//75
//#define LIST_BOX_ONOFF_START_Y				72//75
//#define LIST_GUILD_ALL_Y					50
//#define GUILD_NAME_START_Y					50
//#endif

#define LIST_BOX_NAME_START_Y				100//75
#define LIST_BOX_ONOFF_START_Y				100//75
#define LIST_GUILD_ALL_Y					50
#define GUILD_NAME_START_Y					48

// 2008-04-04 by bhsohn Ep3 커뮤니티 창
#define SELECT_GUILD_NAME_WIDTH				300		// 길드 선택창

// 메뉴
#define MENU_INFO							0		// 정보
#define MENU_LETTER							1		// 편지
#define MENU_APPOINT						2		// 임명
#define MENU_CHARGE							3		// 위임
#define MENU_PURGE							4		// 추방
#define MAX_MENU_LIST						5

// 기어 명
#define LIST_BOX_GEAR_START_X				287
#define LIST_BOX_GEAR_START_Y				100

// 레벨
#define LIST_BOX_LV_START_X					331
#define LIST_BOX_LV_START_Y					100

// 길드 계급
#define LIST_BOX_RANK_START_X					376
#define LIST_BOX_RANK_START_Y					103

// 음성
#define LIST_BOX_VOIP_START_X					411
#define LIST_BOX_VOIP_START_Y					100

// 여단 공지 사항
#define GUILD_OPENAREA_EDIT_X						26

#define GUILD_OPENAREA_EDIT_Y						210
//#define GUILD_OPENAREA_EDIT_W						135
#define GUILD_OPENAREA_EDIT_W						125
#define GUILD_OPENAREA_FONT_LINE_HEIGHT			15
#define GUILD_OPENAREA_FONT_MOUSE_HEIGHT			120
#define	MAX_GUILD_LINE								8

// 길드 마스터 
#define GUILD_MASTERNAME_START_X					100
#define GUILD_MASTERNAME_START_Y					76

// 부길드 마스터 
#define GUILD_SECONDNAME_START_X					100
#define GUILD_SECONDNAME_START_Y					93

// 전진 기지전 
#define GUILD_OUTPOST_START_X					100
#define GUILD_OUTPOST_START_Y					110

// 전체 여단명성 
#define GUILD_ALL_START_X						28
#define GUILD_ALL_START_Y						150
#define GUILD_ALL_POINT_START_X					80
#define GUILD_ALL_POINT_START_Y					150
#define GUILD_ALL_RANK_START_X					133
#define GUILD_ALL_RANK_START_Y					150
// 월별 여단명성 
#define GUILD_MONTH_START_X						28
#define GUILD_MONTH_START_Y						168
#define GUILD_MONTH_POINT_START_X					80
#define GUILD_MONTH_POINT_START_Y					168
#define GUILD_MONTH_RANK_START_X					133
#define GUILD_MONTH_RANK_START_Y					168	
#else
#define COMBO_SIZE_Y						15
#define COMBO_SIZE_X						100



#define BACK_START_X						13
#define BACK_START_Y						30
#define GUILD_MARK_START_X					19
#define GUILD_MARK_START_Y					49
#define GUILD_MARK_SIZE_X					24
#define GUILD_MARK_SIZE_Y					12

#define BUTTON_MAKE_START_X					19	// 길드원인 경우 탈퇴 버튼
#define BUTTON_MAKE_START_Y					231
#define BUTTON_LEAVE_START_X				BUTTON_MAKE_START_X	// 탈퇴
#define BUTTON_LEAVE_START_Y				BUTTON_MAKE_START_Y
#define BUTTON_INVITE_START_X				BUTTON_MAKE_START_X	// 초대
#define BUTTON_INVITE_START_Y				BUTTON_MAKE_START_Y
#define BUTTON_CANCEL_START_X				BUTTON_MAKE_START_X	// 초대
#define BUTTON_CANCEL_START_Y				BUTTON_MAKE_START_Y
#define BUTTON_MARK_START_X					56	// 마크
#define BUTTON_MARK_START_Y					BUTTON_MAKE_START_Y
#define BUTTON_BANMEMBER_START_X			93	// 탈퇴
#define BUTTON_BANMEMBER_START_Y			BUTTON_MAKE_START_Y
#define BUTTON_DISBAND_START_X				130 // 해체
#define BUTTON_DISBAND_START_Y				BUTTON_MAKE_START_Y
#define BUTTON_LEAVE_START_X				BUTTON_MAKE_START_X // 탈퇴
#define BUTTON_LEAVE_START_Y				BUTTON_MAKE_START_Y

#define BUTTON_VOIP_START_X					167
#define BUTTON_VOIP_START_Y					BUTTON_MAKE_START_Y
#define VOIP_BUTTON_STATE_NORMAL			0
#define VOIP_BUTTON_STATE_UP				1
#define VOIP_BUTTON_STATE_DOWN				2
#define VOIP_BUTTON_STATE_DISABLE			3
#define VOIP_WIDTH							35
#define VOIP_HEIGHT							16



#define BUTTON_SIZE_X						35
#define BUTTON_SIZE_Y						16
#define BUTTON_MIDDLE_SIZE_X				56
#define BUTTON_BIG_SIZE_X					72


//#define GUILD_NAME_START_X					53
#define GUILD_NAME_START_X					64
#define LIST_BOX_NAME_START_X				190

//#define LIST_BOX_GUILDRANK_START_X			140//138
//#define LIST_BOX_GUILDRANK_START_Y			74//75
#define LIST_BOX_GUILDRANK_START_X			379
#define LIST_BOX_GUILDRANK_START_Y			115

//#define LIST_BOX_ONOFF_START_X				161
#define LIST_BOX_ONOFF_START_X				458
#define LIST_BOX_NUMBER						8
#define LIST_BOX_INTERVAL					18
#define GUILD_MEMBER_ONLINE_COLOR			GUI_FONT_COLOR
#define GUILD_MEMBER_OFFLINE_COLOR			RGB(222, 0, 0)
#define GUI_FONT_COLOR_DISMEMBER_READY		RGB(150, 150, 150)
//#define MARK_START_X						21
//#define MARK_START_Y						53
#define MARK_START_X						32
#define MARK_START_Y						57


// 2006-03-07 by ispark, 언어에 따라 위치 수정
//#if defined(LANGUAGE_ENGLISH) || defined(LANGUAGE_VIETNAM)
//#define LIST_BOX_NAME_START_Y				72//75
//#define LIST_BOX_ONOFF_START_Y				72//75
//#define LIST_GUILD_ALL_Y					50
//#define GUILD_NAME_START_Y					50
//#else
//#define LIST_BOX_NAME_START_Y				72//75
//#define LIST_BOX_ONOFF_START_Y				72//75
//#define LIST_GUILD_ALL_Y					50
//#define GUILD_NAME_START_Y					50
//#endif

#define LIST_BOX_NAME_START_Y				114//75
#define LIST_BOX_ONOFF_START_Y				114//75
#define LIST_GUILD_ALL_Y					50
#define GUILD_NAME_START_Y					54

// 2008-04-04 by bhsohn Ep3 커뮤니티 창
#define SELECT_GUILD_NAME_WIDTH				300		// 길드 선택창

// 메뉴
#define MENU_INFO							0		// 정보
#define MENU_LETTER							1		// 편지
#define MENU_APPOINT						2		// 임명
#define MENU_CHARGE							3		// 위임
#define MENU_PURGE							4		// 추방
#define MAX_MENU_LIST						5

// 기어 명
#define LIST_BOX_GEAR_START_X				287
#define LIST_BOX_GEAR_START_Y				114

// 레벨
#define LIST_BOX_LV_START_X					331
#define LIST_BOX_LV_START_Y					114

// 길드 계급
#define LIST_BOX_RANK_START_X					376
#define LIST_BOX_RANK_START_Y					117

// 음성
#define LIST_BOX_VOIP_START_X					411
#define LIST_BOX_VOIP_START_Y					114

// 여단 공지 사항
#define GUILD_OPENAREA_EDIT_X						30
#define GUILD_OPENAREA_EDIT_Y						225
//#define GUILD_OPENAREA_EDIT_W						135
#define GUILD_OPENAREA_EDIT_W						125
#define GUILD_OPENAREA_FONT_LINE_HEIGHT			15
#define GUILD_OPENAREA_FONT_MOUSE_HEIGHT			120
#define	MAX_GUILD_LINE								8

// 길드 마스터 
#define GUILD_MASTERNAME_START_X					102
#define GUILD_MASTERNAME_START_Y					81

// 부길드 마스터 
#define GUILD_SECONDNAME_START_X					102
#define GUILD_SECONDNAME_START_Y					98

// 전진 기지전 
#define GUILD_OUTPOST_START_X					102
#define GUILD_OUTPOST_START_Y					116

// 전체 여단명성 
#define GUILD_ALL_START_X						31
#define GUILD_ALL_START_Y						157
#define GUILD_ALL_POINT_START_X					77
#define GUILD_ALL_POINT_START_Y					157
#define GUILD_ALL_RANK_START_X					130
#define GUILD_ALL_RANK_START_Y					157
// 월별 여단명성 
#define GUILD_MONTH_START_X						31
#define GUILD_MONTH_START_Y						175
#define GUILD_MONTH_POINT_START_X					77
#define GUILD_MONTH_POINT_START_Y					175
#define GUILD_MONTH_RANK_START_X					130
#define GUILD_MONTH_RANK_START_Y					175

#endif

// 아이디 별
struct CompareNameInc
{
	template<typename T>
		bool operator()(const T *pPtr1, const T *pPtr2)
	{		
		if(strcmp(pPtr1->MemberName, pPtr2->MemberName) <0)
		{
			return true;
		}
		return false;
	}
};

struct CompareNameDec
{
	template<typename T>
		bool operator()(const T *pPtr1, const T *pPtr2)
	{		
		if(strcmp(pPtr1->MemberName, pPtr2->MemberName) >0)
		{
			return true;
		}
		return false;
	}
};
// 유닛별
struct CompareGearInc
{
	template<typename T>
		bool operator()(const T *pPtr1, const T *pPtr2)
	{		
		if(pPtr1->nUnitIdx > pPtr2->nUnitIdx)
		{
			return true;
		}
		return false;
	}
};

struct CompareGearDec
{
	template<typename T>
		bool operator()(const T *pPtr1, const T *pPtr2)
	{		
		if(pPtr1->nUnitIdx < pPtr2->nUnitIdx)
		{
			return true;
		}
		return false;
	}
};

// 레벨별
struct CompareLVInc
{
	template<typename T>
		bool operator()(const T *pPtr1, const T *pPtr2)
	{		
		int nLV1 = atoi(pPtr1->chLevel);
		int nLV2 = atoi(pPtr2->chLevel);
		if(nLV1 > nLV2)
		{
			return true;
		}
		return false;
	}
};

struct CompareLVDec
{
	template<typename T>
		bool operator()(const T *pPtr1, const T *pPtr2)
	{		
		int nLV1 = atoi(pPtr1->chLevel);
		int nLV2 = atoi(pPtr2->chLevel);
		if(nLV1 < nLV2)
		{
			return true;
		}
		return false;
	}
};

// 계급별
struct CompareRankInc
{
	template<typename T>
		bool operator()(const T *pPtr1, const T *pPtr2)
	{		
		//if(pPtr1->GuildRank > pPtr2->GuildRank)
		if(pPtr1->GuildOrderIdx > pPtr2->GuildOrderIdx)
		{
			return true;
		}
		return false;
	}
};

struct CompareRankDec
{
	template<typename T>
		bool operator()(const T *pPtr1, const T *pPtr2)
	{		
		if(pPtr1->GuildOrderIdx < pPtr2->GuildOrderIdx)
		{
			return true;
		}
		return false;
	}
};

// 접속여부
struct CompareOnOffInc
{
	template<typename T>
		bool operator()(const T *pPtr1, const T *pPtr2)
	{		
		if(pPtr1->IsOnline > pPtr2->IsOnline)
		{
			return true;
		}
		return false;
	}
};

struct CompareOnOffDec
{
	template<typename T>
		bool operator()(const T *pPtr1, const T *pPtr2)
	{		
		if(pPtr1->IsOnline < pPtr2->IsOnline)
		{
			return true;
		}
		return false;
	}
};


// end 2008-04-04 by bhsohn Ep3 커뮤니티 창

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CINFCommunityGuild::CINFCommunityGuild(CAtumNode* pParent)
{
	m_pParent = pParent;

	m_pBack = NULL;
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	m_pBackControl = NULL;																		  
#endif
	m_pImgSelect = NULL;
	//memset(m_pButton, 0x00, sizeof(int)*BUTTON_NUMBER*BUTTON_STATE_NUMBER);
	int i;
	for(i=0;i<BUTTON_NUMBER;i++)
	{
		m_nButtonState[i] = BUTTON_STATE_NORMAL;
	}

//	for(i=0;i<BUTTON_STATE_VOIP;i++)
//	{
//		m_pGuildVOIPButton[i] = NULL;
//	}

	m_pGuildInfo = NULL;
	m_nGuildRank = 0;
	m_bGuildMember = FALSE;

//	m_nCurrentSelectWindowIndex = -1;
//	m_nCurrentScrollNumber = 0;

	m_pFontGuildMaster = NULL;
	m_pFontGuildAll = NULL;
	memset(m_pFontGuildMember, 0x00, sizeof(DWORD)*MAX_GUILD_NUMBER);
	memset(m_pFontGuildOnline, 0x00, sizeof(DWORD)*MAX_GUILD_NUMBER);
	memset(m_pFontGuildRank, 0x00, sizeof(DWORD)*MAX_GUILD_NUMBER);
	//memset(m_pComboBox, 0x00, sizeof(DWORD)*MAX_GUILD_NUMBER);
	memset(m_pImgGuildRank, 0x00, sizeof(DWORD)*MAX_COMBO_GUILD_RANK);

	//m_pScrollMember = NULL;
	//m_bNtoNCallState = FALSE;
	m_nVOIPButtonState	= VOIP_BUTTON_STATE_NORMAL;
	
	//m_vecGuildMember.clear();

	// 2008-04-04 by bhsohn Ep3 커뮤니티 창
	m_stGuildInfo.clear();

	m_pSpManageBtn = NULL;				// 지원자 관리
	m_pRegisterBtn = NULL;
	
	m_pInviteBtn	= NULL;				// 초대
	m_pMarkBtn		= NULL;				// 마크
	m_pOutBtn		= NULL;				// 탈퇴
	m_pCancelBtn	= NULL;				// 해제
	m_pOpBtn		= NULL;				// 옵션
	m_pCancelQuildCancelBtn = NULL;

	m_pSearchBtn = NULL;				// 여단검색
	m_pGuildBattleBtn = NULL;				// 여단전투 
	m_pOpOpenBtn = NULL;				// 공개 설정
	m_pCrateBtn = NULL ;			// 생성

	for(i =0;i<MAX_GUILD_TAB;i++)
	{
		m_pImageTabBtn[i] = NULL;
		m_bTabState[i] = FALSE;
	}
	m_pScrollMember				= NULL;
	m_pScrollGuildOpen = NULL;
	m_nGuildSelect = -1;			// 선택된 길드원	
	m_SelMemberUniqueNumber = 0;	// 선택된 길드원	

	m_pPopupMenu = NULL;
	m_pImgVOIP = NULL;
	m_bShowRank = FALSE;
	for(i =0;i<MAX_GUILD_NUMBER;i++)
	{
		m_pINFImageComboBox[i] = NULL;
	}

	m_pEditOpenArea = NULL;

	m_bUIGuildMember = FALSE;	// 가입 여부 
	m_nUIGuildRank = 0;	// 계급 : 길드장,대장,대원,무계급(GUILD_RANK_~)

	memset(m_chOpenNotice, 0x00, SIZE_MAX_NOTICE);

	// end 2008-04-04 by bhsohn Ep3 커뮤니티 창

	m_pImgVOIPSpeaking = NULL;
}

CINFCommunityGuild::~CINFCommunityGuild()
{
	// 2008-04-04 by bhsohn Ep3 커뮤니티 창
	int nCnt = 0;
	for(nCnt =0;nCnt<MAX_GUILD_NUMBER;nCnt++)
	{
		util::del(m_pINFImageComboBox[nCnt]);
	}
	// end 2008-04-04 by bhsohn Ep3 커뮤니티 창

	util::del(m_pBack);
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	util::del(m_pBackControl);																  
#endif
	util::del(m_pImgSelect);
	util::del(m_pGuildInfo);
	util::del(m_pFontGuildMaster);
	util::del(m_pFontGuildAll);
//	for(int i=0;i<BUTTON_NUMBER;i++)
//	{
//		for(int j=0;j<BUTTON_STATE_NUMBER;j++)
//		{
//			util::del(m_pButton[i][j]);
//		}
//	}
	int i;
	for( i=0; i<MAX_GUILD_NUMBER; i++)
	{
		util::del(m_pFontGuildMember[i]);
		util::del(m_pFontGuildOnline[i]);
		util::del(m_pFontGuildRank[i]);
		//util::del(m_pComboBox[i]);
	}
	for( i=0; i<MAX_COMBO_GUILD_RANK; i++)
	{
		util::del(m_pImgGuildRank[i]);
	}
	util::del(m_pScrollMember);
	util::del(m_pScrollGuildOpen);

//	for(i=0;i<BUTTON_STATE_VOIP;i++)
//	{
//		util::del(m_pGuildVOIPButton[i]);
//	}

//	CVectorGuildMemberInfoIterator it = m_vecGuildMember.begin();
//	while(it != m_vecGuildMember.end())
//	{
//		util::del(*it);
//		it++;
//	}
//	m_vecGuildMember.clear();

	// 2008-04-04 by bhsohn Ep3 커뮤니티 창		
	DeleteAllGuildInfo();

	util::del(m_pSpManageBtn);			
	util::del(m_pRegisterBtn);				

	util::del(m_pInviteBtn);				// 초대
	util::del(m_pMarkBtn);				// 마크
	util::del(m_pOutBtn);				// 탈퇴
	util::del(m_pCancelBtn);				// 해제
	util::del(m_pCancelQuildCancelBtn);
	util::del(m_pOpBtn);				// 옵션
	util::del(m_pCrateBtn);	// 생성

	util::del(m_pSearchBtn);				// 여단검색
	util::del(m_pGuildBattleBtn);				// 여단전투
	util::del(m_pOpOpenBtn);				// 공개 설정	 
	for(i =0;i<MAX_GUILD_TAB;i++)
	{
		util::del(m_pImageTabBtn[i]);
	}
	util::del(m_pPopupMenu);
	util::del(m_pImgVOIP);
	util::del(m_pImgVOIPSpeaking );
	
	util::del(m_pEditOpenArea);	
	// end 2008-04-04 by bhsohn Ep3 커뮤니티 창
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			CINFCommuinityGuild::IMSocketSendGetGuildInfo(UINT nGuildUniqueNumber)
/// \brief		길드 정보를 요청한다.
///				
/// \author		dhkwon
/// \date		2004-05-20 ~ 2004-05-20
/// \warning	길드창설시(MSG_IC_GUILD_CREATE_OK)
///				길드가입시(MSG_IC_GUILD_ACCEPT_INVITE_OK)
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFCommunityGuild::IMSocketSendGetGuildInfo(UINT nGuildUniqueNumber)
{
	if(!m_pGuildInfo)
	{
		MSG_IC_GUILD_GET_GUILD_INFO sMsg;
		sMsg.GuildUniqueNumber = nGuildUniqueNumber;
		g_pIMSocket->SendMsg(T_IC_GUILD_GET_GUILD_INFO, (char*)&sMsg, sizeof(sMsg));
	}	
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			CINFCommunityGuild::IMSocketSendGuildMark(char* szFileName)
/// \brief		서버로 길드 마크를 보낸다.
/// \author		dhkwon
/// \date		2004-05-25 ~ 2004-05-25
/// \warning	이미지 사이즈가 20X20으로 고정되어야 한다.
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFCommunityGuild::IMSocketSendGuildMark(char* szFileName)
{
	MSG_IC_GUILD_SET_GUILD_MARK sMsg;
	CINFImageFile *pImage = new CINFImageFile;
	pImage->InitDeviceObjects( szFileName, strlen(szFileName));
	if(FAILED(pImage->RestoreDeviceObjects()))
	{
		g_pD3dApp->m_pChat->CreateChatChild( STRERR_C_GUILD_0002, COLOR_CHAT_GUILD );
		util::del(pImage);
		return;
	}
	FILE* fp = NULL;
	if( pImage->GetSrcInfo().Width == 24 && 
		pImage->GetSrcInfo().Height == 12 &&
		(pImage->GetSrcInfo().Format == D3DFMT_A8R8G8B8 ||
		pImage->GetSrcInfo().Format == D3DFMT_R8G8B8 ||
		pImage->GetSrcInfo().Format == D3DFMT_X8R8G8B8) &&
		pImage->GetSrcInfo().ImageFileFormat == D3DXIFF_BMP)
	{
		fp = fopen( szFileName, "rb" );
		if(!fp)
		{
			goto END;
		}
		char pPacket[SIZE_MAX_PACKET];
		memset(pPacket, 0x00, SIZE_MAX_PACKET);
		int nResult = fseek(fp, 0L, SEEK_END);
		if( nResult != 0)
		{
			goto END;
		}
		nResult = ftell(fp);
		fseek(fp, 0, SEEK_SET);
		int nSize = fread( pPacket+sizeof(sMsg), nResult, 1, fp );
		if(nSize != nResult)
		{
//			goto END;
		}
		sMsg.SizeOfGuildMark = nResult;
		memcpy(pPacket, (char*)&sMsg, sizeof(sMsg));
		g_pIMSocket->SendMsg( T_IC_GUILD_SET_GUILD_MARK, pPacket, sizeof(sMsg) + nResult);
		fclose(fp);
		pImage->InvalidateDeviceObjects();
		pImage->DeleteDeviceObjects();
		util::del(pImage);
		return;
	}
END:	// ERROR
	if(fp)	fclose(fp);
	g_pD3dApp->m_pChat->CreateChatChild( STRERR_C_GUILD_0003, COLOR_CHAT_GUILD );
	pImage->InvalidateDeviceObjects();
	pImage->DeleteDeviceObjects();
	util::del(pImage);
	
	// char buf[SIZE_MAX_PACKET];
	//memcpy( sMsg.GuildMark, buf, file size );
	//sMsg.SizeOfGuildMark = sizeof(buf);
	//g_pIMSocket->SendMsg( T_IC_GUILD_SET_GUILD_MARK, (char*)&sMsg, sizeof(sMsg));
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			CINFCommunityGuild::ResetGuildInfo(MSG_IC_GUILD_GET_GUILD_INFO_OK *pInfo)
/// \brief		길드 정보와 길드원 리스트를 지우고, 다시 받는다.
///				길드 해체시에도 사용한다.(MSG_IC_GUILD_DISMEMBER_OK, 인자:NULL)
/// \author		dhkwon
/// \date		2004-05-20 ~ 2004-05-20
/// \warning	AddGuildMember를 통해 길드원을 다시 넣어야 한다.
///				m_nGuildRank는 AddGuildMember에서 세팅한다.
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFCommunityGuild::ResetGuildInfo(MSG_IC_GUILD_GET_GUILD_INFO_OK *pInfo, BOOL bEraseMemberList)
{
	DBGOUT("MSG_IC_GUILD_GET_GUILD_INFO_OK(0x%08X)\n", pInfo);
	util::del(m_pGuildInfo);
	if(bEraseMemberList)
	{
//		CVectorGuildMemberInfoIterator it = m_vecGuildMember.begin();
//		while(it != m_vecGuildMember.end())
//		{
//			util::del(*it);
//			it++;
//		}
//		m_vecGuildMember.clear();
		
		// 2008-04-04 by bhsohn Ep3 커뮤니티 창
		DeleteAllGuildInfo();	// 전체 길드 정보 삭제
		// end 2008-04-04 by bhsohn Ep3 커뮤니티 창
	}	

	if(pInfo)
	{
		m_pGuildInfo = new MSG_IC_GUILD_GET_GUILD_INFO_OK;
		memcpy( m_pGuildInfo, pInfo, sizeof(MSG_IC_GUILD_GET_GUILD_INFO_OK));
		m_bGuildMember = TRUE;

		SetNoticeWrite(m_pGuildInfo->Notice);		
	}
	else
	{		
		SetNoticeWrite(NULL);		

		m_bGuildMember = FALSE;
		g_pShuttleChild->m_myShuttleInfo.GuildUniqueNumber = 0;
		memset( g_pShuttleChild->m_myShuttleInfo.GuildName, 0x00, SIZE_MAX_GUILD_NAME );
	}
	// 2008-04-04 by bhsohn Ep3 커뮤니티 창
	// 길드 관련 버튼들 갱신
	m_bUIGuildMember = m_bGuildMember;
	m_nUIGuildRank = m_nGuildRank;

	RefreshGuildInfo();
	RefreshGuildRankCombo();
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			CINFCommunityGuild::AddGuildMember(MEX_GUILD_MEMBER_INFO* pInfo)
/// \brief		길드원 리스트에 넣음
/// \author		dhkwon
/// \date		2004-05-20 ~ 2004-05-20
/// \warning	최초 접속시 전체 리스트 받을때
///				누군가가 가입했을때
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFCommunityGuild::AddGuildMember(MEX_GUILD_MEMBER_INFO* pInfo)
{
#ifdef _DEBUG
//	CVectorGuildMemberInfoIterator it = m_vecGuildMember.begin();
//	while(it != m_vecGuildMember.end())
//	{
//		if((*it)->MemberUniqueNumber == pInfo->MemberUniqueNumber)
//		{
//			DBGOUT("ERROR: CINFCommunityGuild::AddGuildMember().(%s)\n",pInfo->MemberName);
//			return;
//		}
//		it++;
//	}
#endif
//	MEX_GUILD_MEMBER_INFO* pNewInfo = new MEX_GUILD_MEMBER_INFO;
//	memcpy(pNewInfo, pInfo, sizeof(MEX_GUILD_MEMBER_INFO));
//	m_vecGuildMember.push_back(pNewInfo);

	// 2008-04-04 by bhsohn Ep3 커뮤니티 창
	{
		auto pGuiInfo = new structGuildInfo;		
		memset(pGuiInfo, 0x00 ,sizeof(structGuildInfo));

		strncpy(pGuiInfo->MemberName,	pInfo->MemberName, SIZE_MAX_CHARACTER_NAME);	// 유저명
		pGuiInfo->MemberUniqueNumber	= pInfo->MemberUniqueNumber;				// 길드의 유닛넘
		
		pGuiInfo->UnitKind				= pInfo->UnitKind;							// 유닛의 종류		
		pGuiInfo->nUnitIdx				= ConvertUnitKind_TO_Order(pGuiInfo->UnitKind);	// 유닛인덱스
		reinterpret_cast<CINFCommunity*>(m_pParent)->GetUnitKindString(pGuiInfo->UnitKind, pGuiInfo->chUnitKind);

		wsprintf(pGuiInfo->chLevel, "%d", pInfo->MemberLevel);								// 레벨		

		pGuiInfo->GuildRank				= pInfo->GuildRank;							// GUILD_RANK_XXX
		pGuiInfo->IsUseVOIP = false; // (pInfo->VoipType == CHECK_TYPE_VOIP_NONE) ? FALSE : TRUE;
		//pGuiInfo->IsUseVOIP				= pInfo->VoipType;										// 음성
		pGuiInfo->IsOnline				= pInfo->IsOnline;							// 유저 접속여부

		pGuiInfo->GuildOrderIdx			= GetGuildOrderIdx(pGuiInfo->GuildRank);

		// 2008-10-14 by bhsohn VOIP개선
		pGuiInfo->IsUseSpeakeingVOIP		= FALSE;

		m_stGuildInfo.push_back(pGuiInfo);	
		
		if(m_pScrollMember) m_pScrollMember->SetMaxItem(m_stGuildInfo.size());

		SetGuildMemberSelect(-1); // 선택화면 초기화
		RefreshGuildRankCombo();	// 콤보 박스 갱신		
	}
	// end 2008-04-04 by bhsohn Ep3 커뮤니티 창

	if(pInfo->MemberUniqueNumber == g_pShuttleChild->m_myShuttleInfo.CharacterUniqueNumber)
	{
		m_nGuildRank = pInfo->GuildRank;
	}

	// 2016-07-19 panoskj removing voip
	// 여단 통화 초대
	//if( (m_nGuildRank == GUILD_RANK_COMMANDER || (m_nGuildRank == GUILD_RANK_SUBCOMMANDER)) 
	//	&&m_bNtoNCallState 
	//	&& pInfo->MemberUniqueNumber != g_pShuttleChild->m_myShuttleInfo.CharacterUniqueNumber)
	//{
	//	char buffer[SIZE_MAX_PACKET];
	//	INIT_MSG(MSG_IC_VOIP_NtoN_INVITE_USER, T_IC_VOIP_NtoN_INVITE_USER, pS2Msg, buffer);
	//	pS2Msg->nCharacterUniqueNumber = pInfo->MemberUniqueNumber;
	//	pS2Msg->byIsPartyCall = FALSE;
	//	g_pIMSocket->Write(buffer, MSG_SIZE(MSG_IC_VOIP_NtoN_INVITE_USER));
	//}

}

///////////////////////////////////////////////////////////////////////////////
/// \fn			CINFCommunityGuild::DeleteGuildMember(UINT nUniqueNumber)
/// \brief		길드원 리스트에서 삭제
/// \author		dhkwon
/// \date		2004-05-20 ~ 2004-05-20
/// \warning	길드원이 탈퇴했을때
///				길드원을 추방했을때
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFCommunityGuild::DeleteGuildMember(UINT nUniqueNumber)
{
	// 2008-04-04 by bhsohn Ep3 커뮤니티 창
	{
		vector<structGuildInfo*>::iterator itGuInfo = m_stGuildInfo.begin();
		while(itGuInfo != m_stGuildInfo.end())
		{
			if((*itGuInfo)->MemberUniqueNumber == nUniqueNumber)
			{
				util::del(*itGuInfo);
				m_stGuildInfo.erase(itGuInfo);
				break;
			}			
			itGuInfo++;
		}	
	}
	m_pScrollMember->SetMaxItem((int)m_stGuildInfo.size());
	SetGuildMemberSelect(-1); // 선택화면 초기화
	RefreshGuildRankCombo();	// 콤보 박스 갱신

	// end 2008-04-04 by bhsohn Ep3 커뮤니티 창

//	CVectorGuildMemberInfoIterator it = m_vecGuildMember.begin();
//	while(it != m_vecGuildMember.end())
//	{
//		if((*it)->MemberUniqueNumber == nUniqueNumber)
//		{
//			util::del(*it);
//			m_vecGuildMember.erase(it);
//			return;
//		}
//		it++;
//	}	
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			MEX_GUILD_MEMBER_INFO* CINFCommunityGuild::GetGuildMemberInfo(UNIT nUniqueNumber)
/// \brief		길드 멤버 정보 가져오기
/// \author		dhkwon
/// \date		2004-05-20 ~ 2004-05-20
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
//MEX_GUILD_MEMBER_INFO* CINFCommunityGuild::GetGuildMemberInfo(UINT nUniqueNumber)
//{
//	CVectorGuildMemberInfoIterator it = m_vecGuildMember.begin();
//	while(it != m_vecGuildMember.end())
//	{
//		if((*it)->MemberUniqueNumber == nUniqueNumber)
//		{
//			return *it;
//		}
//		it++;
//	}
//	return NULL;
//}

///////////////////////////////////////////////////////////////////////////////
/// \fn			MEX_GUILD_MEMBER_INFO* CINFCommunityGuild::GetGuildMemberInfo(UNIT nUniqueNumber)
/// \brief		길드 멤버 정보 가져오기
/// \author		dhkwon
/// \date		2004-05-20 ~ 2004-05-20
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
structGuildInfo* CINFCommunityGuild::GetGuildMemberInfo(UINT nUniqueNumber)
{
	vector<structGuildInfo*>::iterator itGuildInfo = m_stGuildInfo.begin();	

	while(itGuildInfo != m_stGuildInfo.end())
	{
		if((*itGuildInfo)->MemberUniqueNumber == nUniqueNumber)
		{
			return *itGuildInfo;
		}
		itGuildInfo++;
	}	
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			CINFCommunityGuild::IMSocketSendGuildSetRank( int nGuildRank, int nIndex)
/// \brief		길드 멤버의 GuildRank를 임명한다.
/// \author		dhkwon
/// \date		2004-05-31 ~ 2004-05-31
/// \warning	nIndex는 위에서부터의 번호이다. 즉 nIndex만큼 it++해야 해당 멤버 정보를 가져올 수 있다.
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CINFCommunityGuild::IMSocketSendGuildSetRank( int nGuildRank, UID32_t	MemberUniqueNumber)
{
	if(nGuildRank == GUILD_RANK_COMMANDER)
	{
		g_pGameMain->m_pInfWindow->AddMsgBox( STRMSG_C_GUILD_0021, _MESSAGE );
		return FALSE;
	}
	MSG_IC_GUILD_SET_RANK sMsg;
	sMsg.GuildRank = nGuildRank;
	
//	CVectorGuildMemberInfoIterator it = m_vecGuildMember.begin();
//	for(int i=0;i<nIndex;i++)
//	{
//		if(it != m_vecGuildMember.end())
//		{
//			it++;
//		}
//		else
//		{
//			DBGOUT("CINFCommunityGuild::IMSocketSendGuildSetRank(%d,%d). \n", nGuildRank, nIndex);
//			return;
//		}
//	}
	sMsg.MemberUniqueNumber = MemberUniqueNumber;
	g_pIMSocket->SendMsg( T_IC_GUILD_SET_RANK, (char*)&sMsg, sizeof(sMsg));
	return TRUE;
}


HRESULT CINFCommunityGuild::InitDeviceObjects()
{	
	// 2008-04-04 by bhsohn Ep3 커뮤니티 창
	//DataHeader *pDataHeader = m_pGameData->Find("glback");
 	DataHeader *pDataHeader;
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	if( m_pBack == NULL )
	{
		DataHeader* pDataHeader = g_pGameMain->m_GruopImagemanager->FindResource("commu_Bri");	
		m_pBack = g_pGameMain->m_GruopImagemanager->GetGroupImage( pDataHeader );
		m_pBack->InitDeviceObjects( g_pD3dApp->m_pImageList );
		m_pBack->RestoreDeviceObjects();
	}

	if( m_pBackControl == NULL )
	{
		DataHeader* pDataHeader = g_pGameMain->m_GruopImagemanager->FindResource("B_btn1");	
		m_pBackControl = g_pGameMain->m_GruopImagemanager->GetGroupImage( pDataHeader );
	}																							  
#else  
    pDataHeader = m_pGameData->Find("commubk2");
	m_pBack = new CINFImageEx;
	m_pBack->InitDeviceObjects( pDataHeader );
#endif
	
	// 2008-04-04 by bhsohn Ep3 커뮤니티 창
	//pDataHeader = m_pGameData->Find("glselect");	
	pDataHeader = m_pGameData->Find("p_sel0");	
	// end 2008-04-04 by bhsohn Ep3 커뮤니티 창

	m_pImgSelect = new CINFImageEx;
	m_pImgSelect->InitDeviceObjects( pDataHeader );
	char chRankBuf[MAX_COMBO_GUILD_RANK][64] =
	{
		"gr00",
		"gr01",
		"gr12",
		"gr02",
		"gr03",
		"gr04",
		"gr05",
		"gr06",
		"gr07",
		"gr08",
		"gr09",
		"gr10",
		"gr11"
	};

	int i;
	for(i=0;i<MAX_COMBO_GUILD_RANK; i++)
	{
//		wsprintf(buf, "gr%02d", i);
//		pDataHeader = m_pGameData->Find(buf);
		pDataHeader = m_pGameData->Find(chRankBuf[i]);
		m_pImgGuildRank[i] = new CINFImageEx;
		m_pImgGuildRank[i]->InitDeviceObjects( pDataHeader );
	}
//	for(i=0;i<BUTTON_STATE_NUMBER; i++)
//	{
//		wsprintf(buf, "glmake%02d", i);
//		pDataHeader = m_pGameData->Find(buf);
//		m_pButton[BUTTON_MAKE][i] = new CINFImage;
//		m_pButton[BUTTON_MAKE][i]->InitDeviceObjects( pDataHeader->m_pData, pDataHeader->m_DataSize );
//		wsprintf(buf, "glsece%02d", i);
//		pDataHeader = m_pGameData->Find(buf);
//		m_pButton[BUTTON_LEAVE][i] = new CINFImage;
//		m_pButton[BUTTON_LEAVE][i]->InitDeviceObjects( pDataHeader->m_pData, pDataHeader->m_DataSize );
//		wsprintf(buf, "glinvi%02d", i);
//		pDataHeader = m_pGameData->Find(buf);
//		m_pButton[BUTTON_INVITE][i] = new CINFImage;
//		m_pButton[BUTTON_INVITE][i]->InitDeviceObjects( pDataHeader->m_pData, pDataHeader->m_DataSize );
//		wsprintf(buf, "glmark%02d", i);
//		pDataHeader = m_pGameData->Find(buf);
//		m_pButton[BUTTON_MARK][i] = new CINFImage;
//		m_pButton[BUTTON_MARK][i]->InitDeviceObjects( pDataHeader->m_pData, pDataHeader->m_DataSize );
//		wsprintf(buf, "glbrea%02d", i);
//		pDataHeader = m_pGameData->Find(buf);
//		m_pButton[BUTTON_DISBAND][i] = new CINFImage;
//		m_pButton[BUTTON_DISBAND][i]->InitDeviceObjects( pDataHeader->m_pData, pDataHeader->m_DataSize );
//		wsprintf(buf, "glban%02d", i);
//		pDataHeader = m_pGameData->Find(buf);
//		m_pButton[BUTTON_BANMEMBER][i] = new CINFImage;
//		m_pButton[BUTTON_BANMEMBER][i]->InitDeviceObjects( pDataHeader->m_pData, pDataHeader->m_DataSize );
//		wsprintf(buf, "glcan%02d", i);
//		pDataHeader = m_pGameData->Find(buf);
//		m_pButton[BUTTON_CANCEL][i] = new CINFImage;
//		m_pButton[BUTTON_CANCEL][i]->InitDeviceObjects( pDataHeader->m_pData, pDataHeader->m_DataSize );
//	}
	// 2008-04-04 by bhsohn Ep3 커뮤니티 창
	// 버튼 생성
	{
		// 지원자 관리
		{	
			char szUpBtn[30], szDownBtn[30], szSelBtn[30], szDisBtn[30];				
			wsprintf(szUpBtn, "p_sup3");
			wsprintf(szDownBtn, "p_sup1");
			wsprintf(szSelBtn, "p_sup0");
			wsprintf(szDisBtn, "p_sup2");
			if(NULL == m_pSpManageBtn)
			{
				m_pSpManageBtn = new CINFImageBtn;
				// 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현
				//m_pSpManageBtn->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn);		
				m_pSpManageBtn->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn,"STRTOOLTIP61");
				// end 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현
			}			
		}		
		
		// 등록
		{	
			char szUpBtn[30], szDownBtn[30], szSelBtn[30], szDisBtn[30];				
			wsprintf(szUpBtn, "Oapp3");
			wsprintf(szDownBtn, "Oapp1");
			wsprintf(szSelBtn, "Oapp0");
			wsprintf(szDisBtn, "Oapp2");
			if(NULL == m_pRegisterBtn)
			{
				m_pRegisterBtn = new CINFImageBtn;
				// 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현
				//m_pRegisterBtn->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn);		
				m_pRegisterBtn->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn,"STRTOOLTIP56");
				// end 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현
			}			
		}
		// 초대
		{	
			char szUpBtn[30], szDownBtn[30], szSelBtn[30], szDisBtn[30];				
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경			
			wsprintf(szUpBtn, "invs03");
			wsprintf(szDownBtn, "invs01");
			wsprintf(szSelBtn, "invs00");
			wsprintf(szDisBtn, "invs02");														  
#else
			wsprintf(szUpBtn, "c_ssbt03");
			wsprintf(szDownBtn, "c_ssbt01");
			wsprintf(szSelBtn, "c_ssbt00");
			wsprintf(szDisBtn, "c_ssbt02");
#endif
			if(NULL == m_pInviteBtn)
			{
				m_pInviteBtn = new CINFImageBtn;
				// 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현
				//m_pInviteBtn->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn);
				m_pInviteBtn->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn,"STRTOOLTIP57");
				// end 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현
			}			
		}
		// 마크
		{	
			char szUpBtn[30], szDownBtn[30], szSelBtn[30], szDisBtn[30];				
			wsprintf(szUpBtn, "p_bt13");
			wsprintf(szDownBtn, "p_bt11");
			wsprintf(szSelBtn, "p_bt10");
			wsprintf(szDisBtn, "p_bt12");
			if(NULL == m_pMarkBtn)
			{
				m_pMarkBtn = new CINFImageBtn;
				// 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현
				m_pMarkBtn->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn,"STRTOOLTIP64");		
				// end 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현
			}			
		}
		// 탈퇴
		{	
			char szUpBtn[30], szDownBtn[30], szSelBtn[30], szDisBtn[30];				
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경				
			wsprintf(szUpBtn, "outs03");
			wsprintf(szDownBtn, "outs01");
			wsprintf(szSelBtn, "outs00");
			wsprintf(szDisBtn, "outs02");														  
#else
			wsprintf(szUpBtn, "c_bt13");
			wsprintf(szDownBtn, "c_bt11");
			wsprintf(szSelBtn, "c_bt10");
			wsprintf(szDisBtn, "c_bt12");
#endif
			if(NULL == m_pOutBtn)
			{
				m_pOutBtn = new CINFImageBtn;
				// 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현
				//m_pOutBtn->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn);
				m_pOutBtn->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn,"STRTOOLTIP59");
				// end 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현
			}			
		}
		// 해제
		{	
			char szUpBtn[30], szDownBtn[30], szSelBtn[30], szDisBtn[30];				
			wsprintf(szUpBtn, "p_bt33");
			wsprintf(szDownBtn, "p_bt31");
			wsprintf(szSelBtn, "p_bt30");
			wsprintf(szDisBtn, "p_bt32");
			if(NULL == m_pCancelBtn)
			{
				m_pCancelBtn = new CINFImageBtn;
				// 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현
				//m_pCancelBtn->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn);
				m_pCancelBtn->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn,"STRTOOLTIP69");
				// 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현
			}			
		}
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경		
		// 해제 취소
		{	
			char szUpBtn[30], szDownBtn[30], szSelBtn[30], szDisBtn[30];				
			wsprintf(szUpBtn, "cans03");
			wsprintf(szDownBtn, "cans01");
			wsprintf(szSelBtn, "cans00");
			wsprintf(szDisBtn, "cans02");
			if(NULL == m_pCancelQuildCancelBtn)
			{
				m_pCancelQuildCancelBtn = new CINFImageBtn;
				// 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현
				m_pCancelQuildCancelBtn->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn,"STRTOOLTIP41");		
				// end 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현
			}			
		}
#else
		// 해제 취소
		{	
			char szUpBtn[30], szDownBtn[30], szSelBtn[30], szDisBtn[30];				
			wsprintf(szUpBtn, "p_bt53");
			wsprintf(szDownBtn, "p_bt51");
			wsprintf(szSelBtn, "p_bt50");
			wsprintf(szDisBtn, "p_bt52");
			if(NULL == m_pCancelQuildCancelBtn)
			{
				m_pCancelQuildCancelBtn = new CINFImageBtn;
				m_pCancelQuildCancelBtn->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn);		
			}			
		}
#endif
		// 옵션
		{	
			char szUpBtn[30], szDownBtn[30], szSelBtn[30], szDisBtn[30];				
			wsprintf(szUpBtn, "p_bt43");
			wsprintf(szDownBtn, "p_bt41");
			wsprintf(szSelBtn, "p_bt40");
			wsprintf(szDisBtn, "p_bt42");
			if(NULL == m_pOpBtn)
			{
				m_pOpBtn = new CINFImageBtn;
				// 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현
				//m_pOpBtn->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn);		
				m_pOpBtn->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn,"STRTOOLTIP70");
				// end 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현
			}			
		}	
		
		// 여단검색
		{	
			char szUpBtn[30], szDownBtn[30], szSelBtn[30], szDisBtn[30];				
			wsprintf(szUpBtn, "p_tbt03");
			wsprintf(szDownBtn, "p_tbt01");
			wsprintf(szSelBtn, "p_tbt00");
			wsprintf(szDisBtn, "p_tbt02");
			if(NULL == m_pSearchBtn)
			{
				m_pSearchBtn = new CINFImageBtn;
				// 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현
				//m_pSearchBtn->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn);		
				m_pSearchBtn->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn,"STRTOOLTIP68");
				// ebd 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현
			}			
		}
		// 여단전투
		{	
			char szBtn[30];				
			wsprintf(szBtn, "p_gwar");
			if(NULL == m_pGuildBattleBtn)
			{
				m_pGuildBattleBtn = new CINFImageBtn;
				// 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현
				//m_pGuildBattleBtn->InitDeviceObjects(szBtn);	
				m_pGuildBattleBtn->InitDeviceObjects(szBtn,"STRTOOLTIP50");	
				// end 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현
			}			
		}
		// 공개 설정	 
		{	
			char szUpBtn[30], szDownBtn[30], szSelBtn[30], szDisBtn[30];				
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경		
			wsprintf(szUpBtn, "uopbtn3");
			wsprintf(szDownBtn, "uopbtn1");
			wsprintf(szSelBtn, "uopbtn0");
			wsprintf(szDisBtn, "uopbtn2");														  
#else
			wsprintf(szUpBtn, "p_tbt13");
			wsprintf(szDownBtn, "p_tbt11");
			wsprintf(szSelBtn, "p_tbt10");
			wsprintf(szDisBtn, "p_tbt12");
#endif
			if(NULL == m_pOpOpenBtn)
			{
				m_pOpOpenBtn = new CINFImageBtn;
				// 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현
				//m_pOpOpenBtn->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn);
				m_pOpOpenBtn->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn,"STRTOOLTIP15");
				// end 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현
			}			
		}

		// 창
		{
			char szUpBtn[30], szDownBtn[30], szSelBtn[30], szDisBtn[30];				
			for(i =0;i<MAX_GUILD_TAB;i++)
			{				
				wsprintf(szUpBtn, "p_li%d3", i);
				wsprintf(szDownBtn, "p_li%d1", i);
				wsprintf(szSelBtn, "p_li%d3", i);
				wsprintf(szDisBtn, "p_li%d3", i);
				if(NULL == m_pImageTabBtn[i])
				{
					m_pImageTabBtn[i] = new CINFImageBtn;
					// 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현
					//m_pImageTabBtn[i]->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn);
					m_pImageTabBtn[i]->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn);		
					// end 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현
				}			
			}
		}

		{			
			// 음성 채팅
			pDataHeader = m_pGameData->Find("p_sel1");				
			
			m_pImgVOIP = new CINFImageEx;
			m_pImgVOIP->InitDeviceObjects( pDataHeader );			
		}
		
		{			
			// 음성 채팅
			pDataHeader = m_pGameData->Find("p_sel2");				
			
			m_pImgVOIPSpeaking = new CINFImageEx;
			m_pImgVOIPSpeaking->InitDeviceObjects( pDataHeader );
		}

		
	}
	
	m_pFontGuildMaster = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()), 9, D3DFONT_ZENABLE, FALSE,128,32);
	m_pFontGuildMaster->InitDeviceObjects(g_pD3dDev);
	m_pFontGuildAll = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()), 9, D3DFONT_ZENABLE, FALSE,128,32);
	m_pFontGuildAll->InitDeviceObjects(g_pD3dDev);
	for( i=0; i<MAX_GUILD_NUMBER; i++)
	{
		m_pFontGuildMember[i] = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()), 9, D3DFONT_ZENABLE, FALSE,128,32);
		m_pFontGuildMember[i]->InitDeviceObjects(g_pD3dDev);
		m_pFontGuildOnline[i] = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()), 9, D3DFONT_ZENABLE, FALSE,128,32);
		m_pFontGuildOnline[i]->InitDeviceObjects(g_pD3dDev);
		m_pFontGuildRank[i] = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()), 9, D3DFONT_ZENABLE, FALSE,128,32);
		m_pFontGuildRank[i]->InitDeviceObjects(g_pD3dDev);
	}
	
	POINT ptBkPos = ((CINFCommunity*)m_pParent)->GetCommunityBkPos();
	int nPosX, nPosY;
	nPosX = ptBkPos.x + SCROLL_START_X;
	nPosY = ptBkPos.y + SCROLL_START_Y;
	
	// 2008-04-04 by bhsohn Ep3 커뮤니티 창
//	m_pScroll = new CINFScrollBar(this,
//								nPosX, 
//								nPosY, 
//								SCROLL_LINE_LENGTH,
//								MAX_GUILD_NUMBER);
//	m_pScroll->SetGameData( m_pGameData );
//	m_pScroll->InitDeviceObjects();
	{
		char  szScBall[30];	
		if(NULL == m_pScrollMember)
		{
			m_pScrollMember = new CINFArenaScrollBar;
		}		
		wsprintf(szScBall,"c_scrlb");
		
		m_pScrollMember->InitDeviceObjects(MAX_GUILD_NUMBER, szScBall);
	}
	
	{
		char  szScBall[30];	
		if(NULL == m_pScrollGuildOpen)
		{
			m_pScrollGuildOpen = new CINFArenaScrollBar;
		}		
		wsprintf(szScBall,"c_scrlb");		
		m_pScrollGuildOpen->InitDeviceObjects(MAX_GUILD_LINE, szScBall);
	}
	// end 2008-04-04 by bhsohn Ep3 커뮤니티 창

//	for( i=0; i<MAX_GUILD_NUMBER; i++)
//	{
//		nPosX = ptBkPos.x + LIST_BOX_GUILDRANK_START_X;
//		nPosY = ptBkPos.y + LIST_BOX_GUILDRANK_START_Y+LIST_BOX_INTERVAL*i;
//
//		m_pComboBox[i] = new CINFComboBox(this, nPosX, 
//						   nPosY, 
//						   COMBO_SIZE_X, 
//						   COMBO_SIZE_Y, 
//						   -1);
//		m_pComboBox[i]->SetGameData(m_pGameData);
//		for(int j=0;j<MAX_COMBO_GUILD_RANK;j++)
//		{
//			m_pComboBox[i]->AddString(GetGuildRankName(j), m_pImgGuildRank[j], j);
//		}
//		m_pComboBox[i]->InitDeviceObjects();
//	}
	{	
		// 생성
		char szUpBtn[30], szDownBtn[30], szSelBtn[30], szDisBtn[30];				
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경		
		wsprintf(szUpBtn, "cres03");
		wsprintf(szDownBtn, "cres01");
		wsprintf(szSelBtn, "cres00");
		wsprintf(szDisBtn, "cres02");															  
#else
		wsprintf(szUpBtn, "createb3");
		wsprintf(szDownBtn, "createb1");
		wsprintf(szSelBtn, "createb0");
		wsprintf(szDisBtn, "createb2");
#endif
		if(NULL == m_pCrateBtn)
		{
			m_pCrateBtn = new CINFImageBtn;
			// 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현
			//m_pCrateBtn->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn);		
			m_pCrateBtn->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn,"STRTOOLTIP47");
			// end 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현
		}		
	}
	{
		char szUpBtn[30], szDownBtn[30], szSelBtn[30], szDisBtn[30];				

		if(NULL == m_pPopupMenu)
		{
			m_pPopupMenu = new CINFPopupMenu;
		}
		
		// 정보 
		wsprintf(szUpBtn, "p_inf03");
		wsprintf(szDownBtn, "p_inf01");
		wsprintf(szSelBtn, "p_inf00");
		wsprintf(szDisBtn, "p_inf02");
		m_pPopupMenu->AddMenu(MENU_INFO, szUpBtn, szDownBtn, szSelBtn, szDisBtn);

		// 편지
		wsprintf(szUpBtn, "p_inf13");
		wsprintf(szDownBtn, "p_inf11");
		wsprintf(szSelBtn, "p_inf10");
		wsprintf(szDisBtn, "p_inf12");
		m_pPopupMenu->AddMenu(MENU_LETTER, szUpBtn, szDownBtn, szSelBtn, szDisBtn);

		// 임명
		wsprintf(szUpBtn, "p_inf23");
		wsprintf(szDownBtn, "p_inf21");
		wsprintf(szSelBtn, "p_inf20");
		wsprintf(szDisBtn, "p_inf22");
		m_pPopupMenu->AddMenu(MENU_APPOINT, szUpBtn, szDownBtn, szSelBtn, szDisBtn);

		// 위임
		wsprintf(szUpBtn, "p_inf33");
		wsprintf(szDownBtn, "p_inf31");
		wsprintf(szSelBtn, "p_inf30");
		wsprintf(szDisBtn, "p_inf32");
		m_pPopupMenu->AddMenu(MENU_CHARGE, szUpBtn, szDownBtn, szSelBtn, szDisBtn);

		// 추방화
		wsprintf(szUpBtn, "p_inf43");
		wsprintf(szDownBtn, "p_inf41");
		wsprintf(szSelBtn, "p_inf40");
		wsprintf(szDisBtn, "p_inf42");
		m_pPopupMenu->AddMenu(MENU_PURGE, szUpBtn, szDownBtn, szSelBtn, szDisBtn);

		// 객체 초기
		m_pPopupMenu->InitDeviceObjects();
		
	}

	{		
		int nCnt= 0;
		POINT ptPos;		
		ptPos.x = ptPos.y = 0;
		
		

		int nMainCnt = 0;
		for(nMainCnt =0;nMainCnt<MAX_GUILD_NUMBER;nMainCnt++)
		{
			if(NULL == m_pINFImageComboBox[nMainCnt])
			{
				ptPos.x = ptBkPos.x + LIST_BOX_RANK_START_X;
				ptPos.y = ptBkPos.y + LIST_BOX_RANK_START_Y + LIST_BOX_INTERVAL*nMainCnt;

				m_pINFImageComboBox[nMainCnt] = new CINFImageComboBox;
				m_pINFImageComboBox[nMainCnt]->SetGameData(m_pGameData);
				m_pINFImageComboBox[nMainCnt]->InitDeviceObjects(ptPos, COMBO_SIZE_X, COMBO_SIZE_Y);	
			}
			
			for(nCnt=0;nCnt<MAX_COMBO_GUILD_RANK;nCnt++)
			{
				BOOL bShowItem = TRUE;
				if(GUILD_RANK_COMMANDER == nCnt)
				{
					// 여단장은 아이템을 안보여준다.
					bShowItem = FALSE;					
				}
				m_pINFImageComboBox[nMainCnt]->AddItem(nCnt, GetGuildRankName(nCnt), m_pImgGuildRank[nCnt], bShowItem);		
			}
		}
	}
	{
		if(NULL == m_pEditOpenArea)
		{
			// 2008-10-29 by bhsohn 에디트 박스 형태 변경
			//m_pEditOpenArea = new CINFEditBox;
			m_pEditOpenArea = new CINFCurselEditBox;
		}
		POINT ptPos = {GUILD_OPENAREA_EDIT_X, GUILD_OPENAREA_EDIT_Y};		
		m_pEditOpenArea->InitDeviceObjects(9, ptPos, GUILD_OPENAREA_EDIT_W, TRUE, GUILD_OPENAREA_FONT_LINE_HEIGHT, 
										FALSE, GUILD_OPENAREA_FONT_MOUSE_HEIGHT);				
		m_pEditOpenArea->SetStringMaxBuff(SIZE_MAX_NOTICE);		
		m_pEditOpenArea->SetOnePageItemCnt(MAX_GUILD_LINE);	// 2008-10-29 by bhsohn 에디트 박스 형태 변경
	}

	// end 2008-04-04 by bhsohn Ep3 커뮤니티 창

	// 음성통화 버튼	
//	for(i=0;i<BUTTON_STATE_VOIP;i++)
//	{
//		wsprintf(buf, "c_voip0%d", i); 
//		m_pGuildVOIPButton[i] = new CINFImage;
//		pDataHeader = FindResource(buf);
//		m_pGuildVOIPButton[i]->InitDeviceObjects(pDataHeader->m_pData,pDataHeader->m_DataSize) ;
//	}

	return S_OK;
}

HRESULT CINFCommunityGuild::RestoreDeviceObjects()
{
	m_pBack->RestoreDeviceObjects();
	m_pImgSelect->RestoreDeviceObjects();
	
//	for(int i=0;i<BUTTON_NUMBER;i++)
//	{
//		for(int j=0;j<BUTTON_STATE_NUMBER; j++)
//		{
//			m_pButton[i][j]->RestoreDeviceObjects();
//		}
//	}
	m_pFontGuildMaster->RestoreDeviceObjects();
	m_pFontGuildAll->RestoreDeviceObjects();
	int i;
	for( i=0; i<MAX_GUILD_NUMBER; i++)
	{
		m_pFontGuildMember[i]->RestoreDeviceObjects();
		m_pFontGuildOnline[i]->RestoreDeviceObjects();
		m_pFontGuildRank[i]->RestoreDeviceObjects();
		//m_pComboBox[i]->RestoreDeviceObjects();
	}
	// 2008-04-04 by bhsohn Ep3 커뮤니티 창
	//m_pScroll->RestoreDeviceObjects();
	POINT ptBkPos = ((CINFCommunity*)m_pParent)->GetCommunityBkPos();
	{
		// 스크롤
		m_pScrollMember->RestoreDeviceObjects();

		
		RECT rcMouseWhell, rcMousePos;
		POINT ptScrollPos = ptBkPos;

		ptScrollPos.x += 489;
		ptScrollPos.y += 116;

		m_pScrollMember->SetPosition(ptScrollPos.x ,ptScrollPos.y,11,192);
		rcMouseWhell.left		= ptScrollPos.x - 300;
		rcMouseWhell.top		= ptScrollPos.y - 30;
		rcMouseWhell.right		= ptScrollPos.x + 30;
		rcMouseWhell.bottom		= ptScrollPos.y + 252;
		m_pScrollMember->SetMouseWhellRect(rcMouseWhell);
		rcMousePos.left			= ptScrollPos.x - 11;
		rcMousePos.top			= ptScrollPos.y ;
		rcMousePos.right		= rcMousePos.left + 32;
		rcMousePos.bottom		= rcMousePos.top + 282;
		m_pScrollMember->SetMouseBallRect(rcMousePos);
	}
	
	{
		// 스크롤
		m_pScrollGuildOpen->RestoreDeviceObjects();

		
		RECT rcMouseWhell, rcMousePos;
		POINT ptScrollPos = ptBkPos;

		ptScrollPos.x += 168;
		ptScrollPos.y += 225;

		m_pScrollGuildOpen->SetPosition(ptScrollPos.x ,ptScrollPos.y,11,88);
		rcMouseWhell.left		= ptScrollPos.x - 300;
		rcMouseWhell.top		= ptScrollPos.y - 30;
		rcMouseWhell.right		= ptScrollPos.x + 30;
		rcMouseWhell.bottom		= ptScrollPos.y + 100;
		m_pScrollGuildOpen->SetMouseWhellRect(rcMouseWhell);
		rcMousePos.left			= ptScrollPos.x - 11;
		rcMousePos.top			= ptScrollPos.y ;
		rcMousePos.right		= rcMousePos.left + 32;
		rcMousePos.bottom		= rcMousePos.top + 110;
		m_pScrollGuildOpen->SetMouseBallRect(rcMousePos);
	}
	// end 2008-04-04 by bhsohn Ep3 커뮤니티 창
	
	for( i=0; i<MAX_COMBO_GUILD_RANK; i++)
	{
		m_pImgGuildRank[i]->RestoreDeviceObjects();
	}

//	for(i=0;i<BUTTON_STATE_VOIP;i++)
//	{
//		m_pGuildVOIPButton[i]->RestoreDeviceObjects();
//	}
	// 2008-04-04 by bhsohn Ep3 커뮤니티 창	
	int nPosX, nPosY;
	nPosX = nPosY = 0;
	if(m_pSpManageBtn)
	{
		m_pSpManageBtn->RestoreDeviceObjects();				
		nPosX = ptBkPos.x + 27;
		nPosY = ptBkPos.y + 361;		
		m_pSpManageBtn->SetBtnPosition(nPosX, nPosY);	
	}	
	if(m_pRegisterBtn)
	{
		m_pRegisterBtn->RestoreDeviceObjects();				
		nPosX = ptBkPos.x + 110;
		nPosY = ptBkPos.y + 361;		
		m_pRegisterBtn->SetBtnPosition(nPosX, nPosY);	
	}

	if(m_pInviteBtn)
	{
		m_pInviteBtn->RestoreDeviceObjects();				
		nPosX = ptBkPos.x + 186;
		nPosY = ptBkPos.y + 361;		
		m_pInviteBtn->SetBtnPosition(nPosX, nPosY);	
	}
	if(m_pMarkBtn)
	{
		m_pMarkBtn->RestoreDeviceObjects();				
		nPosX = ptBkPos.x + 249;
		nPosY = ptBkPos.y + 361;		
		m_pMarkBtn->SetBtnPosition(nPosX, nPosY);	
	}
	if(m_pOutBtn)
	{
		m_pOutBtn->RestoreDeviceObjects();				
		nPosX = ptBkPos.x + 312;
		nPosY = ptBkPos.y + 361;		
		m_pOutBtn->SetBtnPosition(nPosX, nPosY);	
	}
	if(m_pCancelBtn)
	{
		m_pCancelBtn->RestoreDeviceObjects();				
		nPosX = ptBkPos.x + 375;
		nPosY = ptBkPos.y + 361;		
		m_pCancelBtn->SetBtnPosition(nPosX, nPosY);	
	}
	if(m_pCancelQuildCancelBtn)
	{
		m_pCancelQuildCancelBtn->RestoreDeviceObjects();				
		nPosX = ptBkPos.x + 375;
		nPosY = ptBkPos.y + 361;		
		m_pCancelQuildCancelBtn->SetBtnPosition(nPosX, nPosY);	
	}
	if(m_pOpBtn)
	{
		m_pOpBtn->RestoreDeviceObjects();				
		nPosX = ptBkPos.x + 438;
		nPosY = ptBkPos.y + 361;		
		m_pOpBtn->SetBtnPosition(nPosX, nPosY);	
	}
	if(m_pCrateBtn)
	{
		m_pCrateBtn->RestoreDeviceObjects();				
		nPosX = ptBkPos.x + 438;
		nPosY = ptBkPos.y + 361;		
		m_pCrateBtn->SetBtnPosition(nPosX, nPosY);	
	}
	if(m_pSearchBtn)
	{
		m_pSearchBtn->RestoreDeviceObjects();				
		nPosX = ptBkPos.x + 331;
		nPosY = ptBkPos.y + 57;		
		m_pSearchBtn->SetBtnPosition(nPosX, nPosY);	
	}
	if(m_pGuildBattleBtn)
	{
		m_pGuildBattleBtn->RestoreDeviceObjects();				
		nPosX = ptBkPos.x + 331;
		nPosY = ptBkPos.y + 57;		
		m_pGuildBattleBtn->SetBtnPosition(nPosX, nPosY);	
	}
	if(m_pOpOpenBtn)
	{
		m_pOpOpenBtn->RestoreDeviceObjects();				
		nPosX = ptBkPos.x + 415;
		nPosY = ptBkPos.y + 57;		
		m_pOpOpenBtn->SetBtnPosition(nPosX, nPosY);	
	}
	POINT ptPos[MAX_GUILD_TAB] = 
	{
		{187, 88},
		{271, 88},
		{315, 88},
		{359, 88},
		{403, 88},
		{445, 88},
	};
	for(i =0;i<MAX_GUILD_TAB;i++)
	{
		if(m_pImageTabBtn[i])
		{
			m_pImageTabBtn[i]->RestoreDeviceObjects();				
			nPosX = ptBkPos.x + ptPos[i].x;
			nPosY = ptBkPos.y + ptPos[i].y;		
			m_pImageTabBtn[i]->SetBtnPosition(nPosX, nPosY);	
		}
	}

	if(m_pPopupMenu)
	{
		m_pPopupMenu->RestoreDeviceObjects();
	}
	if(m_pImgVOIP)
	{
		m_pImgVOIP->RestoreDeviceObjects();
	}
	if(m_pImgVOIPSpeaking)
	{
		m_pImgVOIPSpeaking->RestoreDeviceObjects();
	}
	int nMainCnt = 0;
	for(nMainCnt =0;nMainCnt<MAX_GUILD_NUMBER;nMainCnt++)
	{
		if(m_pINFImageComboBox[nMainCnt])
		{
			m_pINFImageComboBox[nMainCnt]->RestoreDeviceObjects();
		}
	}

	if(m_pEditOpenArea)
	{
		m_pEditOpenArea->RestoreDeviceObjects();
		nPosX = ptBkPos.x + GUILD_OPENAREA_EDIT_X;
		nPosY = ptBkPos.y + GUILD_OPENAREA_EDIT_Y;		
		m_pEditOpenArea->SetPos(nPosX, nPosY);	
	}
	
	// end 2008-04-04 by bhsohn Ep3 커뮤니티 창
	return S_OK;
}
HRESULT CINFCommunityGuild::InvalidateDeviceObjects()
{
	m_pBack->InvalidateDeviceObjects();
	m_pImgSelect->InvalidateDeviceObjects();
//	for(int i=0;i<BUTTON_NUMBER;i++)
//	{
//		for(int j=0;j<BUTTON_STATE_NUMBER; j++)
//		{
//			m_pButton[i][j]->InvalidateDeviceObjects();
//		}
//	}
	m_pFontGuildMaster->InvalidateDeviceObjects();
	m_pFontGuildAll->InvalidateDeviceObjects();
	int i;
	for( i=0; i<MAX_GUILD_NUMBER; i++)
	{
		m_pFontGuildMember[i]->InvalidateDeviceObjects();
		m_pFontGuildOnline[i]->InvalidateDeviceObjects();
		m_pFontGuildRank[i]->InvalidateDeviceObjects();
		//m_pComboBox[i]->InvalidateDeviceObjects();
	}
	m_pScrollMember->InvalidateDeviceObjects();
	m_pScrollGuildOpen->InvalidateDeviceObjects();
	for( i=0; i<MAX_COMBO_GUILD_RANK; i++)
	{
		m_pImgGuildRank[i]->InvalidateDeviceObjects();
	}

//	for(i=0;i<BUTTON_STATE_VOIP;i++)
//	{
//		m_pGuildVOIPButton[i]->InvalidateDeviceObjects();
//	}
	// 2008-04-04 by bhsohn Ep3 커뮤니티 창
	if(m_pSpManageBtn)
	{
		m_pSpManageBtn->InvalidateDeviceObjects();
	}	
	if(m_pRegisterBtn)
	{
		m_pRegisterBtn->InvalidateDeviceObjects();
	}
	if(m_pInviteBtn)
	{
		m_pInviteBtn->InvalidateDeviceObjects();
	}	
	if(m_pMarkBtn)
	{
		m_pMarkBtn->InvalidateDeviceObjects();
	}	
	if(m_pOutBtn)
	{
		m_pOutBtn->InvalidateDeviceObjects();
	}	
	if(m_pCancelBtn)
	{
		m_pCancelBtn->InvalidateDeviceObjects();
	}	
	if(m_pCancelQuildCancelBtn)
	{
		m_pCancelQuildCancelBtn->InvalidateDeviceObjects();
	}
	if(m_pOpBtn)
	{
		m_pOpBtn->InvalidateDeviceObjects();
	}	
	if(m_pSearchBtn)
	{
		m_pSearchBtn->InvalidateDeviceObjects();
	}	
	if(m_pGuildBattleBtn)
	{
		m_pGuildBattleBtn->InvalidateDeviceObjects();
	}	
	if(m_pOpOpenBtn)
	{
		m_pOpOpenBtn->InvalidateDeviceObjects();
	}	
	for(i =0;i<MAX_GUILD_TAB;i++)
	{
		if(m_pImageTabBtn[i])
		{
			m_pImageTabBtn[i]->InvalidateDeviceObjects();
		}
	}
	if(m_pCrateBtn)
	{
		m_pCrateBtn->InvalidateDeviceObjects();
	}	
	if(m_pPopupMenu)
	{
		m_pPopupMenu->InvalidateDeviceObjects();
	}
	if(m_pImgVOIP)
	{
		m_pImgVOIP->InvalidateDeviceObjects();
	}
	if(m_pImgVOIPSpeaking)
	{
		m_pImgVOIPSpeaking->InvalidateDeviceObjects();
	}
	int nMainCnt = 0;
	for(nMainCnt =0;nMainCnt<MAX_GUILD_NUMBER;nMainCnt++)
	{
		if(m_pINFImageComboBox[nMainCnt])
		{
			m_pINFImageComboBox[nMainCnt]->InvalidateDeviceObjects();
		}
	}
	if(m_pEditOpenArea)
	{
		m_pEditOpenArea->InvalidateDeviceObjects();
	}
	// end 2008-04-04 by bhsohn Ep3 커뮤니티 창
	return S_OK;
}

HRESULT CINFCommunityGuild::DeleteDeviceObjects()
{
	// 2008-04-04 by bhsohn Ep3 커뮤니티 창
	int nMainCnt = 0;
	for(nMainCnt =0;nMainCnt<MAX_GUILD_NUMBER;nMainCnt++)
	{
		if(m_pINFImageComboBox[nMainCnt])
		{
			m_pINFImageComboBox[nMainCnt]->DeleteDeviceObjects();
			util::del(m_pINFImageComboBox[nMainCnt]);
		}
	}
	
	// end 2008-04-04 by bhsohn Ep3 커뮤니티 창
	m_pBack->DeleteDeviceObjects();
	util::del(m_pBack);

#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	m_pBackControl->DeleteDeviceObjects();
	util::del(m_pBackControl);																  
#endif

	m_pImgSelect->DeleteDeviceObjects();
	util::del(m_pImgSelect);
//	for(int i=0;i<BUTTON_NUMBER;i++)
//	{
//		for(int j=0;j<BUTTON_STATE_NUMBER; j++)
//		{
//			m_pButton[i][j]->DeleteDeviceObjects();
//			util::del(m_pButton[i][j]);
//		}
//	}
	m_pFontGuildMaster->DeleteDeviceObjects();
	m_pFontGuildAll->DeleteDeviceObjects();
	util::del(m_pFontGuildMaster);
	util::del(m_pFontGuildAll);
	int i;
	for( i=0; i<MAX_GUILD_NUMBER; i++)
	{
		m_pFontGuildMember[i]->DeleteDeviceObjects();
		m_pFontGuildOnline[i]->DeleteDeviceObjects();
		m_pFontGuildRank[i]->DeleteDeviceObjects();
		util::del(m_pFontGuildMember[i]);
		util::del(m_pFontGuildOnline[i]);
		util::del(m_pFontGuildRank[i]);
//		m_pComboBox[i]->DeleteDeviceObjects();
//		util::del(m_pComboBox[i]);
	}
	m_pScrollMember->DeleteDeviceObjects();
	util::del(m_pScrollMember);

	m_pScrollGuildOpen->DeleteDeviceObjects();
	util::del(m_pScrollGuildOpen);

	for( i=0; i<MAX_COMBO_GUILD_RANK; i++)
	{
		m_pImgGuildRank[i]->DeleteDeviceObjects();
		util::del(m_pImgGuildRank[i]);
	}

//	for(i=0;i<BUTTON_STATE_VOIP;i++)
//	{
//		m_pGuildVOIPButton[i]->DeleteDeviceObjects();
//		util::del(m_pGuildVOIPButton[i]);
//	}

	// 2008-04-04 by bhsohn Ep3 커뮤니티 창
	if(m_pSpManageBtn)
	{		
		m_pSpManageBtn->DeleteDeviceObjects();
		util::del(m_pSpManageBtn);
	}
	if(m_pRegisterBtn)
	{		
		m_pRegisterBtn->DeleteDeviceObjects();
		util::del(m_pRegisterBtn);
	}
	if(m_pInviteBtn)
	{
		m_pInviteBtn->DeleteDeviceObjects();
		util::del(m_pInviteBtn);
	}	
	if(m_pMarkBtn)
	{
		m_pMarkBtn->DeleteDeviceObjects();
		util::del(m_pMarkBtn);
	}	
	if(m_pOutBtn)
	{
		m_pOutBtn->DeleteDeviceObjects();
		util::del(m_pOutBtn);
	}	
	if(m_pCancelBtn)
	{
		m_pCancelBtn->DeleteDeviceObjects();
		util::del(m_pCancelBtn);
	}	
	if(m_pCancelQuildCancelBtn)
	{
		m_pCancelQuildCancelBtn->DeleteDeviceObjects();
		util::del(m_pCancelQuildCancelBtn);
	}
	if(m_pOpBtn)
	{
		m_pOpBtn->DeleteDeviceObjects();
		util::del(m_pOpBtn);
	}	
	if(m_pSearchBtn)
	{		
		m_pSearchBtn->DeleteDeviceObjects();
		util::del(m_pSearchBtn);
	}
	if(m_pGuildBattleBtn)
	{		
		m_pGuildBattleBtn->DeleteDeviceObjects();
		util::del(m_pGuildBattleBtn);
	}
	if(m_pOpOpenBtn)
	{
		m_pOpOpenBtn->DeleteDeviceObjects();
		util::del(m_pOpOpenBtn);
	}	
	for(i =0;i<MAX_GUILD_TAB;i++)
	{
		if(m_pImageTabBtn[i])
		{
			m_pImageTabBtn[i]->DeleteDeviceObjects();
			util::del(m_pImageTabBtn[i]);
		}
	}
	if(m_pCrateBtn)
	{		
		m_pCrateBtn->DeleteDeviceObjects();
		util::del(m_pCrateBtn);
	}	
	if(m_pPopupMenu)
	{
		m_pPopupMenu->DeleteDeviceObjects();
		util::del(m_pPopupMenu);
	}
	if(m_pImgVOIP)
	{
		m_pImgVOIP->DeleteDeviceObjects();
		util::del(m_pImgVOIP);
	}
	if(m_pImgVOIPSpeaking)
	{
		m_pImgVOIPSpeaking->DeleteDeviceObjects();
		util::del(m_pImgVOIPSpeaking);
	}
	if(m_pEditOpenArea)
	{
		m_pEditOpenArea->DeleteDeviceObjects();
		util::del(m_pEditOpenArea);		
	}
	// end 2008-04-04 by bhsohn Ep3 커뮤니티 창
	
	return S_OK;
}

void CINFCommunityGuild::Tick()
{
	//POINT ptBkPos = ((CINFCommunity*)m_pParent)->GetCommunityBkPos();
	
	//if(m_pScrollMember)
//	{
//		m_pScrollMember->SetNumberOfData( m_vecGuildMember.size());
//
//		int nPosX = ptBkPos.x + SCROLL_START_X;
//		int nPosY = ptBkPos.y + SCROLL_START_Y;
//
//		//m_pScrollMember->SetScrollLinePos( SCROLL_START_X, g_pGameMain->m_nLeftWindowY+SCROLL_START_Y);
//		m_pScrollMember->SetScrollLinePos( nPosX, nPosY);
//		m_pScrollMember->SetWheelRect(LIST_BOX_START_X, 
//			g_pGameMain->m_nLeftWindowY+LIST_BOX_START_Y,
//			LIST_BOX_START_X+LIST_BOX_SIZE_X,
//			g_pGameMain->m_nLeftWindowY+LIST_BOX_START_Y+LIST_BOX_SIZE_Y);
//	}
//	for(int i=0; i<MAX_GUILD_NUMBER; i++)
//	{
//		int nPosX = ptBkPos.x + LIST_BOX_GUILDRANK_START_X;
//		int nPosY = ptBkPos.y + LIST_BOX_GUILDRANK_START_Y+LIST_BOX_INTERVAL*i;
//
//		m_pComboBox[i]->SetStartPos( nPosX, nPosY );
//		int nCurrentStartIndex = m_pScrollMember->GetCurrentScrollIndex();
//		if(i+nCurrentStartIndex < m_vecGuildMember.size())
//		{
//			m_pComboBox[i]->SetCurSelData(m_vecGuildMember[i+nCurrentStartIndex]->GuildRank);
//		}
//		else
//		{
//			m_pComboBox[i]->SetCurSelData(-1);
//		}
//	}
	if((m_bUIGuildMember != m_bGuildMember) // 가입 여부 
		|| (m_nUIGuildRank != m_nGuildRank))	
	{
		m_bUIGuildMember = m_bGuildMember;
		m_nUIGuildRank = m_nGuildRank;
		RefreshGuildInfo();
		RefreshGuildRankCombo();
	}	
}


void CINFCommunityGuild::Render(POINT ptPos)
{
	int nWindowPosY = ptPos.y;
	//m_pBack->Move(BACK_START_X, nWindowPosY + BACK_START_Y );
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	m_pBack->Move(ptPos.x, ptPos.y + BACK_START_Y);												  
#else
	m_pBack->Move(ptPos.x, ptPos.y);
#endif
	m_pBack->Render();

	if(m_bGuildMember)
	{
		if(m_nGuildRank == GUILD_RANK_COMMANDER)
		{
			if(m_pGuildInfo->GuildState == GUILD_STATE_NORMAL)
			{
//				m_pButton[BUTTON_INVITE][m_nButtonState[BUTTON_INVITE]]->Move(BUTTON_INVITE_START_X, nWindowPosY + BUTTON_INVITE_START_Y);
//				m_pButton[BUTTON_INVITE][m_nButtonState[BUTTON_INVITE]]->Render();
//				m_pButton[BUTTON_MARK][m_nButtonState[BUTTON_MARK]]->Move(BUTTON_MARK_START_X, nWindowPosY + BUTTON_MARK_START_Y);
//				m_pButton[BUTTON_MARK][m_nButtonState[BUTTON_MARK]]->Render();
//				m_pButton[BUTTON_BANMEMBER][m_nButtonState[BUTTON_BANMEMBER]]->Move(BUTTON_BANMEMBER_START_X, nWindowPosY + BUTTON_BANMEMBER_START_Y);
//				m_pButton[BUTTON_BANMEMBER][m_nButtonState[BUTTON_BANMEMBER]]->Render();
//				m_pButton[BUTTON_DISBAND][m_nButtonState[BUTTON_DISBAND]]->Move(BUTTON_DISBAND_START_X, nWindowPosY + BUTTON_DISBAND_START_Y);
//				m_pButton[BUTTON_DISBAND][m_nButtonState[BUTTON_DISBAND]]->Render();

				// VOIP 버튼
//				m_pGuildVOIPButton[m_nVOIPButtonState]->Move(BUTTON_VOIP_START_X, nWindowPosY + BUTTON_VOIP_START_Y);
//				m_pGuildVOIPButton[m_nVOIPButtonState]->Render();
			}
			else // GUILD_STATE_DISMEMBER_READY
			{
//				m_pButton[BUTTON_CANCEL][m_nButtonState[BUTTON_CANCEL]]->Move(BUTTON_CANCEL_START_X, nWindowPosY + BUTTON_CANCEL_START_Y);
//				m_pButton[BUTTON_CANCEL][m_nButtonState[BUTTON_CANCEL]]->Render();
			}
		}
		else
		{
//			m_pButton[BUTTON_LEAVE][m_nButtonState[BUTTON_LEAVE]]->Move(BUTTON_LEAVE_START_X, nWindowPosY + BUTTON_LEAVE_START_Y);
//			m_pButton[BUTTON_LEAVE][m_nButtonState[BUTTON_LEAVE]]->Render();

			// VOIP 버튼그러
//			m_pGuildVOIPButton[VOIP_BUTTON_STATE_DISABLE]->Move(BUTTON_VOIP_START_X, nWindowPosY + BUTTON_VOIP_START_Y);
//			m_pGuildVOIPButton[VOIP_BUTTON_STATE_DISABLE]->Render();
		}
//		if(m_pScrollMember->GetCurrentSelectWindowIndex() >= 0 && MAX_GUILD_NUMBER > m_pScrollMember->GetCurrentSelectWindowIndex())
//		{
//			m_pImgSelect->Move(LIST_BOX_START_X+1, nWindowPosY + LIST_BOX_START_Y+LIST_BOX_INTERVAL*m_pScrollMember->GetCurrentSelectWindowIndex()+1);
//			m_pImgSelect->Render();
//		}
		RenderGuildMemberList(m_pGuildInfo->GuildState);
	}
	else
	{
//		m_pButton[BUTTON_MAKE][m_nButtonState[BUTTON_MAKE]]->Move(BUTTON_MAKE_START_X, nWindowPosY + BUTTON_MAKE_START_Y);
//		m_pButton[BUTTON_MAKE][m_nButtonState[BUTTON_MAKE]]->Render();
	}
	m_pScrollMember->Render();
	m_pScrollGuildOpen->Render();	// 공지사항 스크롤
	
//	for( int i=0; i<MAX_GUILD_NUMBER; i++)
//	{
//		m_pComboBox[i]->Render(FALSE);
//	}

	// 2008-04-04 by bhsohn Ep3 커뮤니티 창
	// 에디트 박스	
	m_pEditOpenArea->Tick();
	m_pEditOpenArea->Render(m_pScrollGuildOpen->GetScrollStep(), MAX_GUILD_LINE);

	
	m_pSpManageBtn->Render();
	m_pRegisterBtn->Render();
	
	m_pInviteBtn->Render();				// 초대
	m_pMarkBtn->Render();				// 마크
	m_pOutBtn->Render();				// 탈퇴
	m_pCancelBtn->Render();				// 해제
	m_pCancelQuildCancelBtn->Render();

	m_pOpBtn->Render();					// 옵션

	m_pSearchBtn->Render();		// 여단검색
	m_pGuildBattleBtn->Render();		// 여단전투 
	m_pOpOpenBtn->Render();		// 공개 설정
	
	int i =0;
	for(i =0;i<MAX_GUILD_TAB;i++)
	{
		m_pImageTabBtn[i]->Render();
	}
	m_pCrateBtn->Render();

	{
		int nMainCnt = 0;
		int nShowItem = -1;
		for(nMainCnt =0;nMainCnt<MAX_GUILD_NUMBER;nMainCnt++)
		{
			if(m_pINFImageComboBox[nMainCnt]->IsShowItem())
			{
				nShowItem = nMainCnt;
			}
			else
			{
				m_pINFImageComboBox[nMainCnt]->Render();	// 콤보 박스
			}		
		}
		if(nShowItem != -1)
		{
			m_pINFImageComboBox[nShowItem]->Render();	// 콤보 박스		
		}
	}
	m_pPopupMenu->Render();
	
	// end 2008-04-04 by bhsohn Ep3 커뮤니티 창
}

void CINFCommunityGuild::RenderGuildMemberList(BYTE nGuildState)
{
	POINT ptBkPos = ((CINFCommunity*)m_pParent)->GetCommunityBkPos();

	int nWindowPosY = g_pGameMain->m_nLeftWindowY;
//	int nScrollIndex = m_pScrollMember->GetCurrentScrollIndex();
//	int nCurrentSelectWindowIndex = m_pScrollMember->GetCurrentSelectWindowIndex();

	char buf[64];
	memset(buf, 0x00, 64);
	// 2006-09-19 by dgwoo
	//wsprintf(buf, "[%2d/%d]",m_vecGuildMember.size(),m_pGuildInfo->GuildMemberCapacity);
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	int nPosX = ptBkPos.x + 216;
	int nPosY = ptBkPos.y + 49;																	  
#else							
	int nPosX = ptBkPos.x + 214;
	int nPosY = ptBkPos.y + 59;
#endif

	//wsprintf(buf, "[%d/%d]",(int)m_stGuildInfo.size(),CAtumSJ::GetMaxGuildSize((BOOL)g_pD3dApp->GetPrimiumCardInfo()->nCardItemNum1));
	// 2008-06-18 by bhsohn 여단원증가 카드 관련 처리
	//wsprintf(buf, "%d/%d",(int)m_stGuildInfo.size(),CAtumSJ::GetMaxGuildSize((BOOL)g_pD3dApp->GetPrimiumCardInfo()->nCardItemNum1));
	wsprintf(buf, "%d/%d",(int)m_stGuildInfo.size(), GetMaxGuildSize());
	m_pFontGuildAll->DrawText(nPosX, nPosY, GUI_FONT_COLOR, buf);

	COLORREF fontColor;
	if(nGuildState == GUILD_STATE_NORMAL)
	{
		fontColor = GUI_FONT_COLOR;
	}
	else
	{
		fontColor = GUI_FONT_COLOR_DISMEMBER_READY;
	}
	CINFImage* pMark = g_pDatabase->GetGuildMark(m_pGuildInfo->GuildUniqueNumber);
	if(pMark)
	{
		nPosX = ptBkPos.x + MARK_START_X;
		nPosY = ptBkPos.y + MARK_START_Y;

		pMark->Move(nPosX, nPosY);
		pMark->Render();
	}

	// 길드마스터
	nPosX = ptBkPos.x + GUILD_NAME_START_X;
	nPosY = ptBkPos.y + GUILD_NAME_START_Y;
	m_pFontGuildMaster->DrawText(nPosX, nPosY, fontColor, m_pGuildInfo->GuildName, 0L);

	// 길드 마스터, 부길마 표시
	{
		char chGuildMasterName[SIZE_MAX_ARENA_FULL_NAME], chGuildSecondName[SIZE_MAX_ARENA_FULL_NAME];
		char chBuff[128], chBuff1[128];
		memset(chGuildMasterName, 0x00, SIZE_MAX_ARENA_FULL_NAME);
		memset(chGuildSecondName, 0x00, SIZE_MAX_ARENA_FULL_NAME);
		memset(chBuff, 0x00, 128);
		memset(chBuff1, 0x00, 128);
		GetGuidPeopleInfo(chGuildMasterName, chGuildSecondName);

		// 길드 마스터
		nPosX = ptBkPos.x + GUILD_MASTERNAME_START_X;
		nPosY = ptBkPos.y + GUILD_MASTERNAME_START_Y;
		m_pFontGuildMaster->DrawText(nPosX, nPosY, fontColor, chGuildMasterName, 0L);

		// 부길드 마스터
		nPosX = ptBkPos.x + GUILD_SECONDNAME_START_X;
		nPosY = ptBkPos.y + GUILD_SECONDNAME_START_Y;
		m_pFontGuildMaster->DrawText(nPosX, nPosY, fontColor, chGuildSecondName, 0L);

		// 전진 기지 
		if(m_pGuildInfo && 0 != m_pGuildInfo->GuildOutPostCityMapIndex)
		{
			MAP_INFO* pMapInfo = g_pDatabase->GetMapInfo(m_pGuildInfo->GuildOutPostCityMapIndex);
			if(pMapInfo)
			{
				nPosX = ptBkPos.x + GUILD_OUTPOST_START_X;
				nPosY = ptBkPos.y + GUILD_OUTPOST_START_Y;
				m_pFontGuildMaster->DrawText(nPosX, nPosY, fontColor, pMapInfo->MapName, 0L);
			}
		}

		// 전체
		{
			// 제목
			nPosX = ptBkPos.x + GUILD_ALL_START_X;
			nPosY = ptBkPos.y + GUILD_ALL_START_Y;
			wsprintf(chBuff, "%s", STRMSG_C_080611_0200);			
			m_pFontGuildMaster->DrawText(nPosX, nPosY, fontColor, chBuff, 0L);
			// 여단명성
			nPosX = ptBkPos.x + GUILD_ALL_POINT_START_X;
			nPosY = ptBkPos.y + GUILD_ALL_POINT_START_Y;
			wsprintf(chBuff1, "%d", m_pGuildInfo->GuildTotalFame);
			MakeCurrencySeparator(chBuff,chBuff1,3,',');
			m_pFontGuildMaster->DrawText(nPosX, nPosY, fontColor, chBuff, 0L);
			// 여단랭킹
			nPosX = ptBkPos.x + GUILD_ALL_RANK_START_X;
			nPosY = ptBkPos.y + GUILD_ALL_RANK_START_Y;
			wsprintf(chBuff1, "%d", m_pGuildInfo->GuildTotalFameRank);
			MakeCurrencySeparator(chBuff,chBuff1,3,',');
			m_pFontGuildMaster->DrawText(nPosX, nPosY, fontColor, chBuff, 0L);
		}
		// 월별
		{
			// 제목
			nPosX = ptBkPos.x + GUILD_MONTH_START_X;
			nPosY = ptBkPos.y + GUILD_MONTH_START_Y;
			wsprintf(chBuff, "%s", STRMSG_C_080611_0201);			
			m_pFontGuildMaster->DrawText(nPosX, nPosY, fontColor, chBuff, 0L);
			// 여단명성
			nPosX = ptBkPos.x + GUILD_MONTH_POINT_START_X;
			nPosY = ptBkPos.y + GUILD_MONTH_POINT_START_Y;
			wsprintf(chBuff1, "%d", m_pGuildInfo->GuildMonthlyFame);
			MakeCurrencySeparator(chBuff,chBuff1,3,',');
			m_pFontGuildMaster->DrawText(nPosX, nPosY, fontColor, chBuff, 0L);
			// 여단랭킹
			nPosX = ptBkPos.x + GUILD_MONTH_RANK_START_X;
			nPosY = ptBkPos.y + GUILD_MONTH_RANK_START_Y;
			wsprintf(chBuff1, "%d", m_pGuildInfo->GuildMonthlyFameRank);
			MakeCurrencySeparator(chBuff,chBuff1,3,',');
			m_pFontGuildMaster->DrawText(nPosX, nPosY, fontColor, chBuff, 0L);
		}
	}

//	int i=0;
//	CVectorGuildMemberInfoIterator it = m_vecGuildMember.begin();
//	while(it != m_vecGuildMember.end())
//	{
//		if(nScrollIndex==0)
//		{			
//			nPosX = ptBkPos.x + LIST_BOX_NAME_START_X;
//			nPosY = ptBkPos.y + LIST_BOX_NAME_START_Y + LIST_BOX_INTERVAL*i;
//
//			m_pFontGuildMember[i]->DrawText( nPosX, 
//									nPosY, 
//									i == nCurrentSelectWindowIndex ? GUI_SELECT_FONT_COLOR : fontColor,
//									(*it)->MemberName, 0L );
//
//			if(nGuildState == GUILD_STATE_NORMAL)
//			{
//				nPosX = ptBkPos.x + LIST_BOX_ONOFF_START_X;
//				nPosY = ptBkPos.y + LIST_BOX_ONOFF_START_Y + LIST_BOX_INTERVAL*i;
//
//				if((*it)->IsOnline == GUILD_MEMBER_STATE_ONLINE)
//				{
//					m_pFontGuildOnline[i]->DrawText( nPosX,
//										nPosY, 
//										i == nCurrentSelectWindowIndex ? GUI_SELECT_FONT_COLOR : GUILD_MEMBER_ONLINE_COLOR,
//										"ON",0L);
//				}
//				else
//				{
//					m_pFontGuildOnline[i]->DrawText( nPosX,
//										nPosY, 
//										i == nCurrentSelectWindowIndex ? GUI_SELECT_FONT_COLOR : GUILD_MEMBER_OFFLINE_COLOR,
//										"OFF",0L);
//				}
//			}
//			i++;
//			if(i == MAX_GUILD_NUMBER)
//			{
//				return;
//			}
//		}
//		else
//		{
//			nScrollIndex --;
//		}
//		it++;
//	}
	
	// 2008-04-04 by bhsohn Ep3 커뮤니티 창	
	int nCurrentSelectWindowIndex = -1 ;
	if(m_nGuildSelect >= 0)
	{
		nCurrentSelectWindowIndex = m_nGuildSelect - m_pScrollMember->GetScrollStep();

		if(nCurrentSelectWindowIndex >= 0 && nCurrentSelectWindowIndex < MAX_GUILD_NUMBER)
		{
			// 선택된 여단원표시
			nPosX = ptBkPos.x + LIST_BOX_NAME_START_X-2;
			nPosY = ptBkPos.y + LIST_BOX_NAME_START_Y + (LIST_BOX_INTERVAL*nCurrentSelectWindowIndex)-1;
			m_pImgSelect->Move(nPosX, nPosY);
			m_pImgSelect->Render();
		}
		
	}
	// 길드 원들을 랜더링한다.
	vector<structGuildInfo*>::iterator itGuildInfo = m_stGuildInfo.begin();	
	int nLine = 0;
	for(int nCnt = 0;nCnt < m_pScrollMember->GetScrollStep(); nCnt++)
	{
		if(itGuildInfo == m_stGuildInfo.end())
		{
			break;
		}
		itGuildInfo++;

	}
	for(nLine = 0;nLine < MAX_GUILD_NUMBER; nLine++)
	{
		if(itGuildInfo == m_stGuildInfo.end())
		{
			break;
		}

		structGuildInfo* pGuildInfo = (*itGuildInfo);
		nPosX = ptBkPos.x + LIST_BOX_NAME_START_X;
		nPosY = ptBkPos.y + LIST_BOX_NAME_START_Y + (LIST_BOX_INTERVAL*nLine);
		m_pFontGuildMember[nLine]->DrawText( nPosX, 
			nPosY, 
			fontColor,
			pGuildInfo->MemberName, 0L );
		
		if(nGuildState == GUILD_STATE_NORMAL)
		{	
			// 기어종류표시
			{
				nPosX = ptBkPos.x + LIST_BOX_GEAR_START_X;
				nPosY = ptBkPos.y + LIST_BOX_GEAR_START_Y + LIST_BOX_INTERVAL*nLine;
				
				m_pFontGuildOnline[nLine]->DrawText( nPosX,
						nPosY, 
						GUILD_MEMBER_ONLINE_COLOR,
						pGuildInfo->chUnitKind,0L);		
				
			}

			// 레벨
			{
				nPosX = ptBkPos.x + LIST_BOX_LV_START_X;
				nPosY = ptBkPos.y + LIST_BOX_LV_START_Y + LIST_BOX_INTERVAL*nLine;
				
				m_pFontGuildOnline[nLine]->DrawText( nPosX,
						nPosY, 
						GUILD_MEMBER_ONLINE_COLOR,
						pGuildInfo->chLevel,0L);		
				
			}

					
			// 음성 온오프
			{
				if(pGuildInfo->IsUseVOIP)
				{
					if(pGuildInfo->IsUseSpeakeingVOIP)
					{
						nPosX = ptBkPos.x + LIST_BOX_VOIP_START_X;
						nPosY = ptBkPos.y + LIST_BOX_VOIP_START_Y + LIST_BOX_INTERVAL*nLine;
						m_pImgVOIPSpeaking->Move(nPosX, nPosY);
						m_pImgVOIPSpeaking->Render();
					}
					else 
					{
						nPosX = ptBkPos.x + LIST_BOX_VOIP_START_X;
						nPosY = ptBkPos.y + LIST_BOX_VOIP_START_Y + LIST_BOX_INTERVAL*nLine;
						m_pImgVOIP->Move(nPosX, nPosY);
						m_pImgVOIP->Render();
					}					
				}
				
			}

			// 온/오프 여부
			{
				nPosX = ptBkPos.x + LIST_BOX_ONOFF_START_X;
				nPosY = ptBkPos.y + LIST_BOX_ONOFF_START_Y + LIST_BOX_INTERVAL*nLine;
				
				if(pGuildInfo->IsOnline == GUILD_MEMBER_STATE_ONLINE)
				{
					m_pFontGuildOnline[nLine]->DrawText( nPosX,
						nPosY, 
						GUILD_MEMBER_ONLINE_COLOR,
						"ON",0L);
				}
				else
				{
					m_pFontGuildOnline[nLine]->DrawText( nPosX,
						nPosY, 
						GUILD_MEMBER_OFFLINE_COLOR,
						"OFF",0L);
				}
			}	
		}
		itGuildInfo++;
		
	}	
}


int CINFCommunityGuild::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
//	if(m_pScrollMember)
//	{
//		if(m_pScrollMember->WndProc(uMsg, wParam, lParam) == INF_MSGPROC_BREAK)
//		{
//			return INF_MSGPROC_BREAK;
//		}
//	}
//	if(m_nGuildRank == GUILD_RANK_COMMANDER && m_pGuildInfo->GuildState == GUILD_STATE_NORMAL)
//	{
//		for(int i=0; i<MAX_GUILD_NUMBER; i++)
//		{
//			// 2006-10-18 by ispark, 길드장 아닌 경우 판단 소스 수정
//			int nCurrentIndex = m_pScrollMember->GetCurrentScrollIndex() + i;
//			if(m_pComboBox[i]->GetCurSelIndex() != GUILD_RANK_COMMANDER && 
//				nCurrentIndex < m_vecGuildMember.size()) // 길드장이 아닌 경우만 들어간다.
//			{
//				int result = m_pComboBox[i]->WndProc(uMsg, wParam, lParam);
//				if(m_pComboBox[i]->IsSelectChanged())
//				{
//					int nGuildRank = (int)m_pComboBox[i]->GetCurSelData();
//					IMSocketSendGuildSetRank(nGuildRank, nCurrentIndex);
//				}
//				if(result == INF_MSGPROC_BREAK)
//				{
//					return INF_MSGPROC_BREAK;
//				}
//			}
//		}
//	}
	switch(uMsg)
	{
	case WM_MOUSEMOVE:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);
//			int nWindowPosY = g_pGameMain->m_nLeftWindowY;
//			if(GetButtonStateOnMouse(pt, BUTTON_MAKE_START_X, nWindowPosY+BUTTON_MAKE_START_Y, BUTTON_SIZE_X, BUTTON_SIZE_Y))
//			{
//				if( m_nButtonState[BUTTON_MAKE] != BUTTON_STATE_DOWN )
//					m_nButtonState[BUTTON_MAKE] = BUTTON_STATE_UP;
//				if( m_nButtonState[BUTTON_LEAVE] != BUTTON_STATE_DOWN )
//					m_nButtonState[BUTTON_LEAVE] = BUTTON_STATE_UP;
//				if( m_nButtonState[BUTTON_INVITE] != BUTTON_STATE_DOWN )
//					m_nButtonState[BUTTON_INVITE] = BUTTON_STATE_UP;
//				if( m_nButtonState[BUTTON_CANCEL] != BUTTON_STATE_DOWN )
//					m_nButtonState[BUTTON_CANCEL] = BUTTON_STATE_UP;
//				m_nButtonState[BUTTON_MARK] = BUTTON_STATE_NORMAL;
//				m_nButtonState[BUTTON_BANMEMBER] = BUTTON_STATE_NORMAL;
//				m_nButtonState[BUTTON_DISBAND] = BUTTON_STATE_NORMAL;
//			}
//			else if(GetButtonStateOnMouse(pt, BUTTON_MARK_START_X,nWindowPosY+BUTTON_MARK_START_Y, BUTTON_SIZE_X, BUTTON_SIZE_X))
//			{
//				m_nButtonState[BUTTON_MAKE] = BUTTON_STATE_NORMAL;
//				m_nButtonState[BUTTON_LEAVE] = BUTTON_STATE_NORMAL;
//				m_nButtonState[BUTTON_INVITE] = BUTTON_STATE_NORMAL;
//				m_nButtonState[BUTTON_CANCEL] = BUTTON_STATE_NORMAL;
//				if( m_nButtonState[BUTTON_MARK] != BUTTON_STATE_DOWN )
//					m_nButtonState[BUTTON_MARK] = BUTTON_STATE_UP;
//				m_nButtonState[BUTTON_BANMEMBER] = BUTTON_STATE_NORMAL;
//				m_nButtonState[BUTTON_DISBAND] = BUTTON_STATE_NORMAL;
//			}
//			else if(GetButtonStateOnMouse(pt, BUTTON_BANMEMBER_START_X,nWindowPosY+BUTTON_BANMEMBER_START_Y, BUTTON_SIZE_X, BUTTON_SIZE_X))
//			{
//				m_nButtonState[BUTTON_MAKE] = BUTTON_STATE_NORMAL;
//				m_nButtonState[BUTTON_LEAVE] = BUTTON_STATE_NORMAL;
//				m_nButtonState[BUTTON_INVITE] = BUTTON_STATE_NORMAL;
//				m_nButtonState[BUTTON_CANCEL] = BUTTON_STATE_NORMAL;
//				m_nButtonState[BUTTON_MARK] = BUTTON_STATE_NORMAL;
//				if( m_nButtonState[BUTTON_BANMEMBER] != BUTTON_STATE_DOWN )
//					m_nButtonState[BUTTON_BANMEMBER] = BUTTON_STATE_UP;
//				m_nButtonState[BUTTON_DISBAND] = BUTTON_STATE_NORMAL;
//			}
//			else if(GetButtonStateOnMouse(pt, BUTTON_DISBAND_START_X,nWindowPosY+BUTTON_DISBAND_START_Y, BUTTON_SIZE_X, BUTTON_SIZE_X))
//			{
//				m_nButtonState[BUTTON_MAKE] = BUTTON_STATE_NORMAL;
//				m_nButtonState[BUTTON_LEAVE] = BUTTON_STATE_NORMAL;
//				m_nButtonState[BUTTON_INVITE] = BUTTON_STATE_NORMAL;
//				m_nButtonState[BUTTON_CANCEL] = BUTTON_STATE_NORMAL;
//				m_nButtonState[BUTTON_MARK] = BUTTON_STATE_NORMAL;
//				m_nButtonState[BUTTON_BANMEMBER] = BUTTON_STATE_NORMAL;
//				if( m_nButtonState[BUTTON_DISBAND] != BUTTON_STATE_DOWN )
//					m_nButtonState[BUTTON_DISBAND] = BUTTON_STATE_UP;
//			}
//			else
//			{
//				m_nButtonState[BUTTON_MAKE] = BUTTON_STATE_NORMAL;
//				m_nButtonState[BUTTON_LEAVE] = BUTTON_STATE_NORMAL;
//				m_nButtonState[BUTTON_INVITE] = BUTTON_STATE_NORMAL;
//				m_nButtonState[BUTTON_CANCEL] = BUTTON_STATE_NORMAL;
//				m_nButtonState[BUTTON_MARK] = BUTTON_STATE_NORMAL;
//				m_nButtonState[BUTTON_BANMEMBER] = BUTTON_STATE_NORMAL;
//				m_nButtonState[BUTTON_DISBAND] = BUTTON_STATE_NORMAL;
//			}
//
//			// 음성 채팅
//			if(	pt.x > BUTTON_VOIP_START_X && pt.x < BUTTON_VOIP_START_X+VOIP_WIDTH &&
//				pt.y > BUTTON_VOIP_START_Y+nWindowPosY && pt.y < BUTTON_VOIP_START_Y+nWindowPosY+VOIP_HEIGHT)
//			{
//				if(m_nVOIPButtonState != VOIP_BUTTON_STATE_DOWN)
//				{
//					m_nVOIPButtonState = VOIP_BUTTON_STATE_UP;
//				}
//			}
//			else
//			{
//				m_nVOIPButtonState = VOIP_BUTTON_STATE_NORMAL;
//			}
			// 2008-04-04 by bhsohn Ep3 커뮤니티 창
			m_pSpManageBtn->OnMouseMove(pt);
			m_pRegisterBtn->OnMouseMove(pt);
			m_pInviteBtn->OnMouseMove(pt);		// 초대
			m_pMarkBtn->OnMouseMove(pt);		// 마크
			m_pOutBtn->OnMouseMove(pt);			// 추방
			m_pCancelBtn->OnMouseMove(pt);		// 해제
			m_pCancelQuildCancelBtn->OnMouseMove(pt);		
			m_pOpBtn->OnMouseMove(pt);			// 옵션

			m_pSearchBtn->OnMouseMove(pt);		// 여단검색
			m_pGuildBattleBtn->OnMouseMove(pt);		// 여단전투 
			m_pOpOpenBtn->OnMouseMove(pt);		// 공개 설정
			for(int i =0;i<MAX_GUILD_TAB;i++)
			{
				m_pImageTabBtn[i]->OnMouseMove(pt);
			}
			m_pCrateBtn->OnMouseMove(pt);	// 생성
			{
				if(m_pScrollMember->GetMouseMoveMode())
				{
					if(FALSE == m_pScrollMember->IsMouseScrollPos(pt))
					{
						m_pScrollMember->SetMouseMoveMode(FALSE);
					}
					else
					{
						m_pScrollMember->SetScrollPos(pt);						
						RefreshGuildRankCombo();	// 콤보 박스 갱신

						return INF_MSGPROC_BREAK;
					}
				}
			}
			
			{
				if(m_pScrollGuildOpen->GetMouseMoveMode())
				{
					if(FALSE == m_pScrollGuildOpen->IsMouseScrollPos(pt))
					{
						m_pScrollGuildOpen->SetMouseMoveMode(FALSE);
					}
					else
					{
						m_pScrollGuildOpen->SetScrollPos(pt);
						
						return INF_MSGPROC_BREAK;
					}
				}
			}
			{
				int nMainCnt =0;
				BOOL bComboSel = FALSE;
				for(nMainCnt =0;nMainCnt<MAX_GUILD_NUMBER;nMainCnt++)
				{
					bComboSel |= m_pINFImageComboBox[nMainCnt]->OnMouseMove(pt);					
				}
				if(bComboSel)
				{
					return INF_MSGPROC_BREAK;
				}
			}
			m_pPopupMenu->OnMouseMove(pt);
			// end 2008-04-04 by bhsohn Ep3 커뮤니티 창
		}
		break;
	case WM_LBUTTONDOWN:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);
			int nWindowPosY = g_pGameMain->m_nLeftWindowY;
//			if( pt.x > LIST_BOX_START_X &&
//				pt.x < LIST_BOX_START_X + LIST_BOX_SIZE_X &&
//				pt.y > nWindowPosY + LIST_BOX_START_Y &&
//				pt.y < nWindowPosY + LIST_BOX_START_Y + LIST_BOX_INTERVAL*MAX_GUILD_NUMBER )
//			{
//				int i = (pt.y-nWindowPosY-LIST_BOX_START_Y)/LIST_BOX_INTERVAL;
//				if(i>=0 && i<MAX_GUILD_NUMBER)
//				{
//					m_nCurrentSelectWindowIndex = i;
//				}
//				break;
//			}
//			if(GetButtonStateOnMouse(pt, BUTTON_MAKE_START_X, nWindowPosY+BUTTON_MAKE_START_Y, BUTTON_SIZE_X, BUTTON_SIZE_Y))
//			{
//				m_nButtonState[BUTTON_MAKE] = BUTTON_STATE_DOWN;
//				m_nButtonState[BUTTON_LEAVE] = BUTTON_STATE_DOWN;
//				m_nButtonState[BUTTON_INVITE] = BUTTON_STATE_DOWN;
//				m_nButtonState[BUTTON_CANCEL] = BUTTON_STATE_DOWN;
//				m_nButtonState[BUTTON_MARK] = BUTTON_STATE_NORMAL;
//				m_nButtonState[BUTTON_BANMEMBER] = BUTTON_STATE_NORMAL;
//				m_nButtonState[BUTTON_DISBAND] = BUTTON_STATE_NORMAL;
//			}
//			else if(GetButtonStateOnMouse(pt, BUTTON_MARK_START_X,nWindowPosY+BUTTON_MARK_START_Y, BUTTON_SIZE_X, BUTTON_SIZE_X))
//			{
//				m_nButtonState[BUTTON_MAKE] = BUTTON_STATE_NORMAL;
//				m_nButtonState[BUTTON_LEAVE] = BUTTON_STATE_NORMAL;
//				m_nButtonState[BUTTON_INVITE] = BUTTON_STATE_NORMAL;
//				m_nButtonState[BUTTON_CANCEL] = BUTTON_STATE_NORMAL;
//				m_nButtonState[BUTTON_MARK] = BUTTON_STATE_DOWN;
//				m_nButtonState[BUTTON_BANMEMBER] = BUTTON_STATE_NORMAL;
//				m_nButtonState[BUTTON_DISBAND] = BUTTON_STATE_NORMAL;
//			}
//			else if(GetButtonStateOnMouse(pt, BUTTON_BANMEMBER_START_X,nWindowPosY+BUTTON_BANMEMBER_START_Y, BUTTON_SIZE_X, BUTTON_SIZE_X))
//			{
//				m_nButtonState[BUTTON_MAKE] = BUTTON_STATE_NORMAL;
//				m_nButtonState[BUTTON_LEAVE] = BUTTON_STATE_NORMAL;
//				m_nButtonState[BUTTON_INVITE] = BUTTON_STATE_NORMAL;
//				m_nButtonState[BUTTON_CANCEL] = BUTTON_STATE_NORMAL;
//				m_nButtonState[BUTTON_MARK] = BUTTON_STATE_NORMAL;
//				m_nButtonState[BUTTON_BANMEMBER] = BUTTON_STATE_DOWN;
//				m_nButtonState[BUTTON_DISBAND] = BUTTON_STATE_NORMAL;
//			}
//			else if(GetButtonStateOnMouse(pt, BUTTON_DISBAND_START_X,nWindowPosY+BUTTON_DISBAND_START_Y, BUTTON_SIZE_X, BUTTON_SIZE_X))
//			{
//				m_nButtonState[BUTTON_MAKE] = BUTTON_STATE_NORMAL;
//				m_nButtonState[BUTTON_LEAVE] = BUTTON_STATE_NORMAL;
//				m_nButtonState[BUTTON_INVITE] = BUTTON_STATE_NORMAL;
//				m_nButtonState[BUTTON_CANCEL] = BUTTON_STATE_NORMAL;
//				m_nButtonState[BUTTON_MARK] = BUTTON_STATE_NORMAL;
//				m_nButtonState[BUTTON_BANMEMBER] = BUTTON_STATE_NORMAL;
//				m_nButtonState[BUTTON_DISBAND] = BUTTON_STATE_DOWN;
//			}
//			else
//			{
//				m_nButtonState[BUTTON_MAKE] = BUTTON_STATE_NORMAL;
//				m_nButtonState[BUTTON_LEAVE] = BUTTON_STATE_NORMAL;
//				m_nButtonState[BUTTON_INVITE] = BUTTON_STATE_NORMAL;
//				m_nButtonState[BUTTON_CANCEL] = BUTTON_STATE_NORMAL;
//				m_nButtonState[BUTTON_MARK] = BUTTON_STATE_NORMAL;
//				m_nButtonState[BUTTON_BANMEMBER] = BUTTON_STATE_NORMAL;
//				m_nButtonState[BUTTON_DISBAND] = BUTTON_STATE_NORMAL;
//			}
//
//			// 음성 채팅
//			if(	pt.x > BUTTON_VOIP_START_X && pt.x < BUTTON_VOIP_START_X+VOIP_WIDTH &&
//				pt.y > BUTTON_VOIP_START_Y+nWindowPosY && pt.y < BUTTON_VOIP_START_Y+nWindowPosY+VOIP_HEIGHT)
//			{
//				m_nVOIPButtonState = VOIP_BUTTON_STATE_DOWN;				
//			}
//			else
//			{
//				m_nVOIPButtonState = VOIP_BUTTON_STATE_NORMAL;
//			}

			// 2008-04-04 by bhsohn Ep3 커뮤니티 창
			if(((m_nGuildRank == GUILD_RANK_COMMANDER ) || (m_nGuildRank == GUILD_RANK_SUBCOMMANDER ))
				&& (m_pGuildInfo && m_pGuildInfo->GuildState == GUILD_STATE_NORMAL))
			{
				if(TRUE == m_pEditOpenArea->OnLButtonDown(pt))
				{						
					m_pEditOpenArea->EnableEdit(TRUE, TRUE);
					
					// 2008-10-29 by bhsohn 에디트 박스 형태 변경
					m_pEditOpenArea->SetFirstLine();	// 가장 위라인으로 이동
					int nMaxStep = m_pScrollGuildOpen->GetMaxStepCnt();
					m_pScrollGuildOpen->SetMaxItem(nMaxStep);
					// end 2008-10-29 by bhsohn 에디트 박스 형태 변경

					return INF_MSGPROC_BREAK;
				}
				if(m_pEditOpenArea->IsEditMode())
				{
					m_pEditOpenArea->BackupTxtString();
					m_pEditOpenArea->EnableEdit(FALSE, FALSE);
				}

				if(UpdateGuildRankInfo(pt))
				{
					return  INF_MSGPROC_BREAK;
				}
			}		
			{
				//  지원자관리
				if(TRUE == m_pSpManageBtn->OnLButtonDown(pt))
				{
					// 버튼위에 마우스가 있다.
					return  INF_MSGPROC_BREAK;
				}		
			}
			
			{
				//  편지보내기
				if(TRUE == m_pRegisterBtn->OnLButtonDown(pt))
				{
					// 버튼위에 마우스가 있다.
					return  INF_MSGPROC_BREAK;
				}		
			}
			
			{
				// 초대
				if(TRUE == m_pInviteBtn->OnLButtonDown(pt))
				{
					// 버튼위에 마우스가 있다.
					return  INF_MSGPROC_BREAK;
				}		
			}
			
			{
				// 마크
				if(TRUE == m_pMarkBtn->OnLButtonDown(pt))
				{
					// 버튼위에 마우스가 있다.
					return  INF_MSGPROC_BREAK;
				}		
			}

			{
				// 추방
				if(TRUE == m_pOutBtn->OnLButtonDown(pt))
				{
					// 버튼위에 마우스가 있다.
					return  INF_MSGPROC_BREAK;
				}		
			}
			{
				// 해제
				if(TRUE == m_pCancelBtn->OnLButtonDown(pt))
				{
					// 버튼위에 마우스가 있다.
					return  INF_MSGPROC_BREAK;
				}		
			}
			
			{
				// 해제 취소
				if(TRUE == m_pCancelQuildCancelBtn->OnLButtonDown(pt))
				{
					// 버튼위에 마우스가 있다.
					return  INF_MSGPROC_BREAK;
				}		
			}
			{
				// 옵션
				if(TRUE == m_pOpBtn->OnLButtonDown(pt))
				{
					// 버튼위에 마우스가 있다.
					return  INF_MSGPROC_BREAK;
				}		
			}				
			{
				// 여단검색
				if(TRUE == m_pSearchBtn->OnLButtonDown(pt))
				{
					// 버튼위에 마우스가 있다.
					return  INF_MSGPROC_BREAK;
				}		
			}
			{
				// 여단전투 
				if(TRUE == m_pGuildBattleBtn->OnLButtonDown(pt))
				{
					// 버튼위에 마우스가 있다.
					return  INF_MSGPROC_BREAK;
				}		
			}
			{
				// 공개 설정
				if(TRUE == m_pOpOpenBtn->OnLButtonDown(pt))
				{
					// 버튼위에 마우스가 있다.
					return  INF_MSGPROC_BREAK;
				}		
			}				
			{
				for(int i =0;i<MAX_GUILD_TAB;i++)
				{
					if(TRUE == m_pImageTabBtn[i]->OnLButtonDown(pt))
					{
						// 버튼위에 마우스가 있다.
						return  INF_MSGPROC_BREAK;
					}
				}
			}
			{
				// 생성
				if(TRUE == m_pCrateBtn->OnLButtonDown(pt))
				{
					// 버튼위에 마우스가 있다.
					return  INF_MSGPROC_BREAK;
				}		
			}
			{
				BOOL bClick = m_pScrollMember->IsMouseBallPos(pt);
				if(bClick)
				{
					m_pScrollMember->SetMouseMoveMode(TRUE);
					return INF_MSGPROC_BREAK;
				}
			}	
			
			{
				BOOL bClick = m_pScrollGuildOpen->IsMouseBallPos(pt);
				if(bClick)
				{
					m_pScrollGuildOpen->SetMouseMoveMode(TRUE);
					return INF_MSGPROC_BREAK;
				}
			}						
						// 길드 권한 변경
				

			{
				// 메뉴관련 설정
				if(m_pPopupMenu->OnLButtonDown(pt))
				{
					// 버튼위에 마우스가 있다.
					return  INF_MSGPROC_BREAK;
				}
				UpdateGuildMemberSelect(pt, FALSE);
			}			
			// end 2008-04-04 by bhsohn Ep3 커뮤니티 창			
		}
		break;
	case WM_RBUTTONDOWN:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);
			// 2008-04-04 by bhsohn Ep3 커뮤니티 창
			UpdateGuildMemberSelect(pt, TRUE);			
			// end 2008-04-04 by bhsohn Ep3 커뮤니티 창
		}
		break;
	case WM_LBUTTONUP:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);
//			int nWindowPosY = g_pGameMain->m_nLeftWindowY;
//			if(GetButtonStateOnMouse(pt, BUTTON_MAKE_START_X, nWindowPosY+BUTTON_MAKE_START_Y, BUTTON_SIZE_X, BUTTON_SIZE_Y))
//			{
//				if(m_bGuildMember)
//				{
//					if(m_nGuildRank == GUILD_RANK_COMMANDER)
//					{
//						if(m_pGuildInfo->GuildState == GUILD_STATE_NORMAL)
//						{
//							if(m_nButtonState[BUTTON_INVITE] == BUTTON_STATE_DOWN)
//							{
//								OnButtonClicked(BUTTON_INVITE);
//							}
//						}
//						else // GUILD_STATE_DISMEMBER_READY
//						{
//							if(m_nButtonState[BUTTON_CANCEL] == BUTTON_STATE_DOWN)
//							{
//								OnButtonClicked(BUTTON_CANCEL);
//							}
//						}
//					}
//					else
//					{
//						if(m_nButtonState[BUTTON_LEAVE] == BUTTON_STATE_DOWN)
//						{
//							OnButtonClicked(BUTTON_LEAVE);
//						}
//					}
//				}
//				else
//				{
//					if(m_nButtonState[BUTTON_MAKE] == BUTTON_STATE_DOWN)
//					{
//						OnButtonClicked(BUTTON_MAKE);
//					}
//				}
//				m_nButtonState[BUTTON_MAKE] = BUTTON_STATE_UP;
//				m_nButtonState[BUTTON_LEAVE] = BUTTON_STATE_UP;
//				m_nButtonState[BUTTON_INVITE] = BUTTON_STATE_UP;
//				m_nButtonState[BUTTON_MARK] = BUTTON_STATE_NORMAL;
//				m_nButtonState[BUTTON_BANMEMBER] = BUTTON_STATE_NORMAL;
//				m_nButtonState[BUTTON_DISBAND] = BUTTON_STATE_NORMAL;
//			}
//			else if(m_bGuildMember == TRUE &&
//					GetButtonStateOnMouse(pt, BUTTON_MARK_START_X,nWindowPosY+BUTTON_MARK_START_Y, BUTTON_SIZE_X, BUTTON_SIZE_X))
//			{
//				if(m_nButtonState[BUTTON_MARK] == BUTTON_STATE_DOWN)
//				{
//					OnButtonClicked(BUTTON_MARK);
//				}
//				m_nButtonState[BUTTON_MAKE] = BUTTON_STATE_NORMAL;
//				m_nButtonState[BUTTON_LEAVE] = BUTTON_STATE_NORMAL;
//				m_nButtonState[BUTTON_INVITE] = BUTTON_STATE_NORMAL;
//				m_nButtonState[BUTTON_MARK] = BUTTON_STATE_UP;
//				m_nButtonState[BUTTON_BANMEMBER] = BUTTON_STATE_NORMAL;
//				m_nButtonState[BUTTON_DISBAND] = BUTTON_STATE_NORMAL;
//			}
//			else if(m_bGuildMember == TRUE &&
//					GetButtonStateOnMouse(pt, BUTTON_BANMEMBER_START_X,nWindowPosY+BUTTON_BANMEMBER_START_Y, BUTTON_SIZE_X, BUTTON_SIZE_X))
//			{
//				if(m_nButtonState[BUTTON_BANMEMBER] == BUTTON_STATE_DOWN)
//				{
//					OnButtonClicked(BUTTON_BANMEMBER);
//				}
//				m_nButtonState[BUTTON_MAKE] = BUTTON_STATE_NORMAL;
//				m_nButtonState[BUTTON_LEAVE] = BUTTON_STATE_NORMAL;
//				m_nButtonState[BUTTON_INVITE] = BUTTON_STATE_NORMAL;
//				m_nButtonState[BUTTON_MARK] = BUTTON_STATE_NORMAL;
//				m_nButtonState[BUTTON_BANMEMBER] = BUTTON_STATE_UP;
//				m_nButtonState[BUTTON_DISBAND] = BUTTON_STATE_NORMAL;
//			}
//			else if(m_bGuildMember == TRUE &&
//					GetButtonStateOnMouse(pt, BUTTON_DISBAND_START_X,nWindowPosY+BUTTON_DISBAND_START_Y, BUTTON_SIZE_X, BUTTON_SIZE_X))
//			{
//				if(m_nButtonState[BUTTON_DISBAND] == BUTTON_STATE_DOWN)
//				{
//					OnButtonClicked(BUTTON_DISBAND);
//				}
//				m_nButtonState[BUTTON_MAKE] = BUTTON_STATE_NORMAL;
//				m_nButtonState[BUTTON_LEAVE] = BUTTON_STATE_NORMAL;
//				m_nButtonState[BUTTON_INVITE] = BUTTON_STATE_NORMAL;
//				m_nButtonState[BUTTON_MARK] = BUTTON_STATE_NORMAL;
//				m_nButtonState[BUTTON_BANMEMBER] = BUTTON_STATE_NORMAL;
//				m_nButtonState[BUTTON_DISBAND] = BUTTON_STATE_UP;
//			}
//			else
//			{
//				m_nButtonState[BUTTON_MAKE] = BUTTON_STATE_NORMAL;
//				m_nButtonState[BUTTON_LEAVE] = BUTTON_STATE_NORMAL;
//				m_nButtonState[BUTTON_INVITE] = BUTTON_STATE_NORMAL;
//				m_nButtonState[BUTTON_MARK] = BUTTON_STATE_NORMAL;
//				m_nButtonState[BUTTON_BANMEMBER] = BUTTON_STATE_NORMAL;
//				m_nButtonState[BUTTON_DISBAND] = BUTTON_STATE_NORMAL;
//			}
//
//			// 음성 채팅
//			if(	m_nGuildRank == GUILD_RANK_COMMANDER &&
//				pt.x > BUTTON_VOIP_START_X && pt.x < BUTTON_VOIP_START_X+VOIP_WIDTH &&
//				pt.y > BUTTON_VOIP_START_Y+nWindowPosY && pt.y < BUTTON_VOIP_START_Y+nWindowPosY+VOIP_HEIGHT)
//			{
//				OnButtonClicked(BUTTON_VOIP);
//				m_nVOIPButtonState = VOIP_BUTTON_STATE_UP;				
//			}
//			else
//			{
//				m_nVOIPButtonState = VOIP_BUTTON_STATE_NORMAL;
//			}

			// 2008-04-04 by bhsohn Ep3 커뮤니티 창
			{				
				if(TRUE == m_pSpManageBtn->OnLButtonUp(pt))
				{
					((CINFCommunity*)m_pParent)->OnPopupGuildManager();
					g_pD3dApp->m_pSound->PlayD3DSound(SOUND_SELECT_BUTTON, D3DXVECTOR3(0,0,0), FALSE);			
					return  INF_MSGPROC_BREAK;
				}
			}
			
			{				
				if(TRUE == m_pRegisterBtn->OnLButtonUp(pt))
				{
					OnClickRegister();	// 여단 공지 등록
					g_pD3dApp->m_pSound->PlayD3DSound(SOUND_SELECT_BUTTON, D3DXVECTOR3(0,0,0), FALSE);			
					return  INF_MSGPROC_BREAK;
				}
			}

			{				
				if(TRUE == m_pInviteBtn->OnLButtonUp(pt))
				{
					OnButtonClicked(BUTTON_INVITE);	// 초대
					g_pD3dApp->m_pSound->PlayD3DSound(SOUND_SELECT_BUTTON, D3DXVECTOR3(0,0,0), FALSE);			
					return  INF_MSGPROC_BREAK;
				}
			}
			{				
				if(TRUE == m_pMarkBtn->OnLButtonUp(pt))
				{
					OnButtonClicked(BUTTON_MARK);	// 마크
					g_pD3dApp->m_pSound->PlayD3DSound(SOUND_SELECT_BUTTON, D3DXVECTOR3(0,0,0), FALSE);			
					return  INF_MSGPROC_BREAK;
				}
			}
			{				
				if(TRUE == m_pOutBtn->OnLButtonUp(pt))
				{
					OnButtonClicked(BUTTON_LEAVE);	// 탈퇴
					g_pD3dApp->m_pSound->PlayD3DSound(SOUND_SELECT_BUTTON, D3DXVECTOR3(0,0,0), FALSE);			
					return  INF_MSGPROC_BREAK;
				}
			}
			{				
				if(TRUE == m_pCancelBtn->OnLButtonUp(pt))
				{
					OnButtonClicked(BUTTON_DISBAND);	// 해체
					g_pD3dApp->m_pSound->PlayD3DSound(SOUND_SELECT_BUTTON, D3DXVECTOR3(0,0,0), FALSE);			
					return  INF_MSGPROC_BREAK;
				}
			}
			
			{				
				if(TRUE == m_pCancelQuildCancelBtn->OnLButtonUp(pt))
				{
					OnButtonClicked(BUTTON_CANCEL);// 해체취소
					g_pD3dApp->m_pSound->PlayD3DSound(SOUND_SELECT_BUTTON, D3DXVECTOR3(0,0,0), FALSE);			
					return  INF_MSGPROC_BREAK;
				}
			}
			//{				
			//	if(TRUE == m_pOpBtn->OnLButtonUp(pt))
			//	{
			//		//
			//		//TestDB(); // 테스트용
			//		g_pGameMain->m_pChat->ShowVoiceChatControl(TRUE);

			//		g_pD3dApp->m_pSound->PlayD3DSound(SOUND_SELECT_BUTTON, D3DXVECTOR3(0,0,0), FALSE);			
			//		return  INF_MSGPROC_BREAK;
			//	}
			//}			

			{				
				if(TRUE == m_pSearchBtn->OnLButtonUp(pt))
				{
					((CINFCommunity*)m_pParent)->OnPopupGuildSearch();					
					g_pD3dApp->m_pSound->PlayD3DSound(SOUND_SELECT_BUTTON, D3DXVECTOR3(0,0,0), FALSE);			
					return  INF_MSGPROC_BREAK;
				}
			}
			{				
				if(TRUE == m_pGuildBattleBtn->OnLButtonUp(pt))
				{
					g_pGameMain->m_pInfWindow->AddMsgBox(STRMSG_C_080917_0100, _Q_GUILD_BATTLE);
					g_pD3dApp->m_pSound->PlayD3DSound(SOUND_SELECT_BUTTON, D3DXVECTOR3(0,0,0), FALSE);			
					return  INF_MSGPROC_BREAK;
				}
			}

			{				
				if(TRUE == m_pOpOpenBtn->OnLButtonUp(pt))
				{
					((CINFCommunity*)m_pParent)->OnPopupUserOpenSetup(FALSE);
					g_pD3dApp->m_pSound->PlayD3DSound(SOUND_SELECT_BUTTON, D3DXVECTOR3(0,0,0), FALSE);			
					return  INF_MSGPROC_BREAK;
				}
			}
			{
				for(int i =0;i<MAX_GUILD_TAB;i++)
				{
					if(TRUE == m_pImageTabBtn[i]->OnLButtonUp(pt))
					{
						OnClickTab(i);
						// 버튼위에 마우스가 있다.
						g_pD3dApp->m_pSound->PlayD3DSound(SOUND_SELECT_BUTTON, D3DXVECTOR3(0,0,0), FALSE);			
						return  INF_MSGPROC_BREAK;
					}
				}
			}
			{
				// 생성
				if(TRUE == m_pCrateBtn->OnLButtonUp(pt))
				{						
					// 여단 창설
					OnButtonClicked(BUTTON_MAKE);

					g_pD3dApp->m_pSound->PlayD3DSound(SOUND_SELECT_BUTTON, D3DXVECTOR3(0,0,0), FALSE);			
					return  INF_MSGPROC_BREAK;					
				}
			}
			{				
				// 팝업 메뉴
				int nPopupMenuId= m_pPopupMenu->OnLButtonUp(pt);
				if(-1 != nPopupMenuId)
				{	
					OnClickPopupMenu(nPopupMenuId);
					m_pPopupMenu->ShowWindow(FALSE, NULL);
					g_pD3dApp->m_pSound->PlayD3DSound(SOUND_SELECT_BUTTON, D3DXVECTOR3(0,0,0), FALSE);			
					return  INF_MSGPROC_BREAK;					
				}
			}
			{
				BOOL bClick = m_pScrollMember->GetMouseMoveMode();
				if(bClick)
				{
					m_pScrollMember->SetMouseMoveMode(FALSE);
					return INF_MSGPROC_BREAK;
				}				
			}
			
			{
				BOOL bClick = m_pScrollGuildOpen->GetMouseMoveMode();
				if(bClick)
				{
					m_pScrollGuildOpen->SetMouseMoveMode(FALSE);
					return INF_MSGPROC_BREAK;
				}				
			}
			// end 2008-04-04 by bhsohn Ep3 커뮤니티 창
		}
		break;
		// 2008-04-04 by bhsohn Ep3 커뮤니티 창
	case WM_MOUSEWHEEL:
		{
			POINT pt;
			GetCursorPos(&pt);
			ScreenToClient(g_pD3dApp->GetHwnd(), &pt);
			CheckMouseReverse(&pt);
			BOOL bClick = m_pScrollMember->IsMouseWhellPos(pt);
			if(bClick)		
			{			
				m_pScrollMember->OnMouseWheel(wParam, lParam);					
				RefreshGuildRankCombo();	// 콤보 박스 갱신

				return INF_MSGPROC_BREAK;
			}
			
			bClick = m_pScrollGuildOpen->IsMouseWhellPos(pt);
			if(bClick)		
			{			
				m_pScrollGuildOpen->OnMouseWheel(wParam, lParam);

				return INF_MSGPROC_BREAK;
			}
		}
		break;	
	case WM_IME_STARTCOMPOSITION:
//	case WM_IME_NOTIFY:
	case WM_IME_COMPOSITION:
	case WM_INPUTLANGCHANGE:	
	case WM_IME_ENDCOMPOSITION:
	case WM_IME_SETCONTEXT:	
	case WM_CHAR:
	case WM_KEYDOWN:
	// 2010. 04. 12 by ckPark 편지쓰기 관련 에디트 박스 기능 개선
	case WM_KEYUP:
	// end 2010. 04. 12 by ckPark 편지쓰기 관련 에디트 박스 기능 개선
		{
			// 2008-10-29 by bhsohn 에디트 박스 형태 변경
			int nLienCnt = -1;
			int nArrowScroll = 0;
			BOOL bRtn = FALSE;
			BOOL bStrCat =FALSE;
			bRtn = m_pEditOpenArea->WndProc(uMsg, wParam, lParam, m_pScrollGuildOpen->GetScrollStep(), 
				&nLienCnt, &nArrowScroll, &bStrCat);
			
			if(nLienCnt < 0)
			{
				// 아무 처리도 안했다.
				return INF_MSGPROC_NORMAL;
			}			
			int nMaxStep = 0;			
			int nScrollStep = 0;
			switch(uMsg)
			{
			case WM_KEYDOWN:
				{
					switch(wParam)
					{
					case VK_LEFT:	// 왼쪽으로 이동
					case VK_UP:
					case VK_RIGHT:
					case VK_DOWN:
						{
							// 방향키를 눌러서 스크롤 위치 변경 
							if(nLienCnt > MAX_GUILD_LINE 
								&& nArrowScroll !=0)
							{
								nMaxStep = m_pScrollGuildOpen->GetMaxStepCnt();
								nScrollStep = m_pScrollGuildOpen->GetScrollStep() + nArrowScroll;
								nScrollStep = max(0, nScrollStep);						
								nScrollStep = min(nMaxStep, nScrollStep);						
								m_pScrollGuildOpen->SetScrollStep(nScrollStep);
							}
							
							return INF_MSGPROC_NORMAL;
						}
						break;
					case VK_RETURN:
						{
							return INF_MSGPROC_BREAK;
						}
						break;
					}
				}
				break;
			}
			
			nMaxStep = max(nLienCnt, MAX_GUILD_LINE);
			nScrollStep = nMaxStep - MAX_GUILD_LINE;
			if(nScrollStep < 0)
			{
				nScrollStep = 0;
			}						
			if(m_pEditOpenArea->IsLastPos())
			{
				// 가장 마지막 라인이냐?					
				
				// 최대 아이템을 정해주고
				m_pScrollGuildOpen->SetMaxItem(nMaxStep);					
				// 스트링을 뒤에 붙이는 방식이 아니다.
				// 스크롤 위치는 가장 아래
				m_pScrollGuildOpen->SetScrollStep(nScrollStep);								
			}					
			else
			{
				m_pScrollGuildOpen->SetOnlyMaxItem(nMaxStep);
				m_pEditOpenArea->RefreshEndCurselPos();	// 마지막위치 갱신
			}
			
			if(bRtn)
			{
				return INF_MSGPROC_BREAK;
			}					
			
		}
		break;
		// end 2008-04-04 by bhsohn Ep3 커뮤니티 창
	}	
	return INF_MSGPROC_NORMAL;
}

void CINFCommunityGuild::OnButtonClicked(int nButton)
{
	switch(nButton)
	{
	case BUTTON_MAKE:
		{
			if(m_pGuildInfo
				&& m_pGuildInfo->GuildState != GUILD_STATE_NORMAL)
				break;
			// 2008-02-27 by bhsohn 통합 아레나 수정
			if(g_pD3dApp->GetArenaState() == ARENA_STATE_ARENA_GAMING )
			{
				
				// 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템
				if( g_pD3dApp->m_ArenaSocketType == CAtumApplication::ARENASOCKET_ARENA )
					g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_080225_0203,COLOR_ERROR);//"아레나에서는 이용할 수 없습니다."
				else if( g_pD3dApp->m_ArenaSocketType == CAtumApplication::ARENASOCKET_INFINITY )
					g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_091103_0322,COLOR_ERROR);//"\y인피니티 필드에서는 사용 할 수 없습니다\y"
				// end 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템

				break;
			}
			// end 2008-02-27 by bhsohn 통합 아레나 수정

			g_pGameMain->m_pInfWindow->AddMsgBox(STRMSG_C_GUILD_0001, _Q_GUILD_CREATE);
		}
		break;
	case BUTTON_LEAVE:
		{
			// 2006-11-07 by ispark, 일반적으로 다 보내기
			if(m_pGuildInfo == NULL)
				break;
			g_pGameMain->m_pInfWindow->AddMsgBox(STRMSG_C_GUILD_0003, _Q_GUILD_LEAVE, m_pGuildInfo->GuildUniqueNumber);
		}
		break;
	case BUTTON_INVITE:
		{
			if(m_pGuildInfo == NULL ||
				m_pGuildInfo->GuildState != GUILD_STATE_NORMAL)
				break;
			if((m_nGuildRank == GUILD_RANK_COMMANDER)||(m_nGuildRank == GUILD_RANK_SUBCOMMANDER))
			{
				g_pGameMain->m_pInfWindow->AddMsgBox(STRMSG_C_GUILD_0004, _Q_GUILD_INVITE);
			}
		}
		break;
	case BUTTON_MARK:
		{
			if(m_pGuildInfo == NULL ||
				m_pGuildInfo->GuildState != GUILD_STATE_NORMAL)
				break;
			if(m_nGuildRank == GUILD_RANK_COMMANDER)
			{
				g_pGameMain->m_pInfWindow->AddMsgBox(STRMSG_C_GUILD_0018, _Q_GUILD_MARK, (DWORD)this);
			}
		}
		break;
	case BUTTON_BANMEMBER:
		{
			if(m_pGuildInfo == NULL ||
				m_pGuildInfo->GuildState != GUILD_STATE_NORMAL)
				break;
			if(m_nGuildSelect < 0 )
			{
				return;
			}		
			int nCurrentSelectRealIndex = m_nGuildSelect;
			vector<structGuildInfo*>::iterator it = m_stGuildInfo.begin();
			int i = 0;
			while(it != m_stGuildInfo.end())
			{
				if( nCurrentSelectRealIndex == i )
				{
					break;
				}
				i++;
				it++;
			}
			if(it != m_stGuildInfo.end())
			{
				if(((*it)->MemberUniqueNumber != g_pShuttleChild->m_myShuttleInfo.CharacterUniqueNumber)
					&&(GUILD_RANK_COMMANDER != (*it)->GuildRank))	// 2008-07-14 by bhsohn EP3 관련 처리
				{
					char buf[256];
					wsprintf( buf, STRMSG_C_GUILD_0014, (*it)->MemberName);
					g_pGameMain->m_pInfWindow->AddMsgBox(buf, _Q_GUILD_BAN_MEMBER, (*it)->MemberUniqueNumber);
				}
			}
			
		}
		break;
	case BUTTON_DISBAND:
		{
			if(m_pGuildInfo == NULL ||
				m_pGuildInfo->GuildState != GUILD_STATE_NORMAL)
				break;
			if(m_nGuildRank == GUILD_RANK_COMMANDER)
			{
				g_pGameMain->m_pInfWindow->AddMsgBox(STRMSG_C_GUILD_0005, _Q_GUILD_DISBAND, m_pGuildInfo->GuildUniqueNumber);
			}
		}
		break;
	case BUTTON_CANCEL:
		{
			if(m_nGuildRank == GUILD_RANK_COMMANDER)
			{
				g_pGameMain->m_pInfWindow->AddMsgBox(STRMSG_C_GUILD_0006, _Q_GUILD_CANCEL, m_pGuildInfo->GuildUniqueNumber);
			}
		}
		break;
	}

}


///////////////////////////////////////////////////////////////////////////////
/// \fn			CINFCommunityGuild::RenderGuildMark(int x, int y, int nGuildUniqueNumber, int nCharacterUniqueNumber)
/// \brief		길드 마크와 길드 랭크 렌더링
/// \author		dhkwon
/// \date		2004-06-02 ~ 2004-06-02
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFCommunityGuild::RenderGuildMark(int x, int y, int nGuildUniqueNumber, int nCharacterUniqueNumber)
{
	if( nGuildUniqueNumber == g_pShuttleChild->m_myShuttleInfo.GuildUniqueNumber )
	{
		structGuildInfo* pInfo = GetGuildMemberInfo(nCharacterUniqueNumber);
		if( pInfo && pInfo->GuildRank != GUILD_RANK_PRIVATE_NULL )
		{
			RenderGuildRank( x, y, pInfo->GuildRank );
		}	
	}
	CINFImageEx* pMark = ((CINFImageEx*)g_pDatabase->GetGuildMark(nGuildUniqueNumber));
	if (pMark)
	{
		pMark->Move(x - ID_GUILD_MARK_START_X, y - 14);							// 2005-10-17 by ispark
		pMark->Render();
	}
}

void CINFCommunityGuild::RenderGuildRank( int x, int y, int nGuildRank )
{
//	m_pImgGuildRank[nGuildRank]->Move( x - ID_GUILD_MARK_START_X + GUILD_MARK_SIZE_X, y - ID_GUILD_MARK_START_Y-5 );
	m_pImgGuildRank[nGuildRank]->Move( x - ID_GUILD_MARK_START_X + GUILD_MARK_SIZE_X + 1, y - 14 );	// 2006-01-11 by ispark
	m_pImgGuildRank[nGuildRank]->Render();
}

void CINFCommunityGuild::IMSocketSendVoipNtoNInviteUser( UID32_t nMemberUniqueNumber )
{
	// 2016-07-19 panoskj disabled this

	//if( //m_nGuildRank == GUILD_RANK_COMMANDER &&
	//	//m_bNtoNCallState &&
	//	nMemberUniqueNumber != g_pShuttleChild->m_myShuttleInfo.CharacterUniqueNumber)
	//{
	//	char buffer[SIZE_MAX_PACKET];
	//	INIT_MSG(MSG_IC_VOIP_NtoN_INVITE_USER, T_IC_VOIP_NtoN_INVITE_USER, pS2Msg, buffer);
	//	pS2Msg->nCharacterUniqueNumber = nMemberUniqueNumber;
	//	pS2Msg->byIsPartyCall = FALSE;
	//	g_pIMSocket->Write(buffer, MSG_SIZE(MSG_IC_VOIP_NtoN_INVITE_USER));
	//}
}

BOOL CINFCommunityGuild::pGuildQuestionInviteUser(CINFMessageBox* pMsgBox, int x, int y)//유닛을 정확히 찍은 경우 return TRUE
{
	D3DXVECTOR2 vPos1 = D3DXVECTOR2(x,y);
	float fLength = 100.0f;
	CEnemyData * pGuild = NULL;
	CEnemyData * pEnemy = NULL;
	map<INT,CEnemyData *>::iterator itEnemy = g_pD3dApp->m_pScene->m_mapEnemyList.begin();
	while(itEnemy != g_pD3dApp->m_pScene->m_mapEnemyList.end())
	{
		if( itEnemy->second->m_nObjScreenW > 0)
		{
			D3DXVECTOR2 vPos2 = D3DXVECTOR2(itEnemy->second->m_nObjScreenX,itEnemy->second->m_nObjScreenY);
			float fLengthTemp = D3DXVec2Length(&(vPos1-vPos2));
			if(fLengthTemp < 50.0f && fLengthTemp < fLength)
			{
				fLength = fLengthTemp;
				pGuild = itEnemy->second;
			}
		}
		itEnemy++;
	}
	if(pGuild)
	{
		strcpy(pMsgBox->m_strInputMessage,pGuild->m_infoCharacter.CharacterInfo.CharacterName);
		strcat(pMsgBox->m_strInputMessage, "_");
		return TRUE;
	}

	return FALSE;
}

void CINFCommunityGuild::RefreshGuildInfo()
{
	if(NULL == m_pSpManageBtn)
	{
		return;
	}
	
	// 2008-10-29 by bhsohn 에디트 박스 형태 변경
	// 여단 공지사항
	if(m_pGuildInfo)
	{
		SetNoticeWrite(m_chOpenNotice);		
	}
	else
	{
		SetNoticeWrite(NULL);		
	}
	// end 2008-10-29 by bhsohn 에디트 박스 형태 변경
	

	BOOL bGuildMember = m_bGuildMember;
	// 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현
	g_pInterface->m_pToolTip->m_bToolTipState = FALSE;
	// end 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현
	
	if(!bGuildMember)
	{		
		//////////////////// 가입한 길드가 없다. /////////////////////			
		m_pSpManageBtn->ShowWindow(FALSE);		// 지원자 관리
		m_pRegisterBtn->ShowWindow(FALSE);		// 등록
	
		m_pInviteBtn->ShowWindow(FALSE);		// 초대
		m_pMarkBtn->ShowWindow(FALSE);			// 마크
		m_pOutBtn->ShowWindow(FALSE);			// 탈퇴
		m_pCancelBtn->ShowWindow(FALSE);		// 해제
		m_pOpBtn->ShowWindow(FALSE);			// 옵션

		m_pSearchBtn->ShowWindow(TRUE);			// 여단검색
		m_pGuildBattleBtn->ShowWindow(FALSE);			// 여단전투 
		m_pOpOpenBtn->ShowWindow(FALSE);		// 공개 설정
	
		m_pCrateBtn->ShowWindow(TRUE);			// 여단 창설

		m_pCancelQuildCancelBtn->ShowWindow(FALSE);		// 여단 해체 취소
		return;
	}
	
	
	BOOL bInvite, bMark, bDisBand, bLeave, bManage, bRegister, bSearch, bOpOpen,bBattle;
	bInvite = bMark = bDisBand = bLeave = bManage = bRegister = bSearch = bOpOpen = bBattle = FALSE;

	bSearch = TRUE;		
	bLeave  = TRUE;
	if(m_nGuildRank == GUILD_RANK_COMMANDER)
	{
		if(m_pGuildInfo && m_pGuildInfo->GuildState == GUILD_STATE_NORMAL)
		{
			bBattle = bInvite = bMark = bDisBand = bManage = bRegister = bLeave = bOpOpen = TRUE;				
		}		
	}		
	else if(m_nGuildRank == GUILD_RANK_SUBCOMMANDER)
	{
		if(m_pGuildInfo && m_pGuildInfo->GuildState == GUILD_STATE_NORMAL)
		{
			bMark = FALSE;
			bInvite = bManage = bRegister = bLeave = bOpOpen = TRUE;				
		}	
	}
	m_pSpManageBtn->ShowWindow(TRUE);
	m_pSpManageBtn->EnableBtn(bManage);		// 지원자 관리
	
	m_pRegisterBtn->ShowWindow(TRUE);
	m_pRegisterBtn->EnableBtn(bRegister);		// 등록		
	
	m_pInviteBtn->ShowWindow(TRUE);
	m_pInviteBtn->EnableBtn(bInvite);		// 초대
	
	m_pMarkBtn->ShowWindow(TRUE);
	m_pMarkBtn->EnableBtn(bMark);			// 마크		
	
	m_pOutBtn->ShowWindow(TRUE);
	m_pOutBtn->EnableBtn(bLeave);			// 탈퇴			
	
	m_pCrateBtn->ShowWindow(FALSE);
	
	m_pCancelBtn->ShowWindow(TRUE);		// 해제
	
	m_pGuildBattleBtn->EnableBtn(bBattle);		// 여단 전투.
	
	
	if(NULL == m_pGuildInfo)
	{
		m_pCancelBtn->EnableBtn(FALSE);
		m_pCancelBtn->ShowWindow(TRUE);		// 해제

		m_pCancelQuildCancelBtn->EnableBtn(TRUE);
		m_pCancelQuildCancelBtn->ShowWindow(FALSE);		// 해제 취소
	}
	else if(m_pGuildInfo && m_pGuildInfo->GuildState == GUILD_STATE_NORMAL)
	{
		if(m_nGuildRank == GUILD_RANK_COMMANDER)
		{
			m_pCancelBtn->EnableBtn(TRUE);
			m_pCancelBtn->ShowWindow(TRUE);		// 해제
			
			m_pCancelQuildCancelBtn->EnableBtn(TRUE);
			m_pCancelQuildCancelBtn->ShowWindow(FALSE);		// 해제 취소
		}		
		else		
		{
			// 부여단장 및 일반 여단원
			m_pCancelBtn->EnableBtn(FALSE);
			m_pCancelBtn->ShowWindow(TRUE);		// 해제
			
			m_pCancelQuildCancelBtn->EnableBtn(FALSE);
			m_pCancelQuildCancelBtn->ShowWindow(FALSE);		// 해제 취소
		}		
	}	
	else
	{
		m_pCancelBtn->EnableBtn(TRUE);
		m_pCancelBtn->ShowWindow(FALSE);		// 해제

		m_pCancelQuildCancelBtn->EnableBtn(TRUE);
		m_pCancelQuildCancelBtn->ShowWindow(TRUE);		// 해제 취소
	}
	

	m_pOpBtn->EnableBtn(bGuildMember);			// 옵션
	m_pOpBtn->ShowWindow(bGuildMember);

	m_pSearchBtn->ShowWindow(FALSE);		// 여단검색
	m_pGuildBattleBtn->ShowWindow(TRUE);		// 여단검색
	m_pOpOpenBtn->ShowWindow(TRUE);		// 공개 설정

	RefreshGuildRankCombo();	// 콤보 박스 갱신
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		길드원 온/오프 여부
/// \author		// 2008-04-04 by bhsohn Ep3 커뮤니티 창
/// \date		2008-05-23 ~ 2008-05-23
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFCommunityGuild::SetGuildOnOffInfo(UINT nUniqueNumber, BYTE IsOnline)	// 길드원 접속 여부
{
	vector<structGuildInfo*>::iterator itGuInfo = m_stGuildInfo.begin();
	while(itGuInfo != m_stGuildInfo.end())
	{
		if((*itGuInfo)->MemberUniqueNumber == nUniqueNumber)
		{
			(*itGuInfo)->IsOnline = IsOnline;
			RefreshGuildRankCombo();	// 콤보 박스 갱신
			return;
		}			
		itGuInfo++;
	}	
	
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		길드원 레벨 정보
/// \author		// 2008-04-04 by bhsohn Ep3 커뮤니티 창
/// \date		2008-05-23 ~ 2008-05-23
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFCommunityGuild::SetLevelInfo(UID32_t CharacterUID, INT	nLevel)
{
	vector<structGuildInfo*>::iterator itGuInfo = m_stGuildInfo.begin();
	while(itGuInfo != m_stGuildInfo.end())
	{
		if((*itGuInfo)->MemberUniqueNumber == CharacterUID)
		{
			wsprintf((*itGuInfo)->chLevel, "%d", nLevel);
			return;
		}			
		itGuInfo++;
	}	
	
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		길드원 랭킹
/// \author		// 2008-04-04 by bhsohn Ep3 커뮤니티 창
/// \date		2008-05-23 ~ 2008-05-23
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFCommunityGuild::SetGuildDSetRank(UINT nUniqueNumber, BYTE GuildRank)	// 길드원 랭킹
{
	vector<structGuildInfo*>::iterator itGuInfo = m_stGuildInfo.begin();
	while(itGuInfo != m_stGuildInfo.end())
	{
		if((*itGuInfo)->MemberUniqueNumber == nUniqueNumber)
		{
			(*itGuInfo)->GuildRank = GuildRank;
			RefreshGuildRankCombo();	// 콤보 박스 갱신			
			if(nUniqueNumber == g_pShuttleChild->m_myShuttleInfo.CharacterUniqueNumber)
			{
				// 나의 권한이 바뀌었다.
				m_nGuildRank = GuildRank;
			}
			return;
		}			
		itGuInfo++;
	}		
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		Test
/// \author		// 2008-04-04 by bhsohn Ep3 커뮤니티 창
/// \date		2008-05-23 ~ 2008-05-23
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFCommunityGuild::TestDB()
{
//	for(int nCnt =0; nCnt < 60; nCnt++)
//	{
//		structGuildInfo* pGuiInfo = new structGuildInfo;		
//		memset(pGuiInfo, 0x00 ,sizeof(structGuildInfo));
//
//		wsprintf(pGuiInfo->MemberName,	"User%d", nCnt+1);	// 유저명
//		pGuiInfo->MemberUniqueNumber	= nCnt;				// 길드의 유닛넘
//		//pGuiInfo->UnitKind				= pInfo->UnitKind;							// 유닛의 종류
//		pGuiInfo->UnitKind = UNITKIND_BT01;
//		pGuiInfo->nUnitIdx				= ConvertUnitKind_TO_Order(pGuiInfo->UnitKind);	// 유닛인덱스
//		((CINFCommunity*)m_pParent)->GetUnitKindString(pGuiInfo->UnitKind, pGuiInfo->chUnitKind);
//
//		//pGuiInfo->Level					= pInfo->Level;								// 레벨
//		wsprintf(pGuiInfo->chLevel, "%d", 80+nCnt);
//		
//		pGuiInfo->GuildRank				= 1;							// GUILD_RANK_XXX
//		pGuiInfo->IsUseVOIP				= TRUE;							// 음성
//		pGuiInfo->IsOnline				= TRUE;							// 유저 접속여부
//
//		m_stGuildInfo.push_back(pGuiInfo);	
//		m_pScrollMember->SetMaxItem((int)m_stGuildInfo.size());
//		RefreshGuildRankCombo();	// 콤보 박스 갱신
//	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		모든 길드 정보 삭제
/// \author		// 2008-04-04 by bhsohn Ep3 커뮤니티 창
/// \date		2008-05-23 ~ 2008-05-23
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFCommunityGuild::DeleteAllGuildInfo()
{
	vector<structGuildInfo*>::iterator itGuInfo = m_stGuildInfo.begin();
	while(itGuInfo != m_stGuildInfo.end())
	{
		util::del(*itGuInfo);
		itGuInfo++;
	}
	m_stGuildInfo.clear();
	if(m_pScrollMember)
	{
		m_pScrollMember->SetMaxItem((int)m_stGuildInfo.size());
	}
	SetGuildMemberSelect(-1);	// 선택화면 초기화	
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		선택 길드원정보 갱신
/// \author		// 2008-04-04 by bhsohn Ep3 커뮤니티 창
/// \date		2008-05-23 ~ 2008-05-23
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFCommunityGuild::UpdateGuildMemberSelect(POINT pt, BOOL bPopupMenu)
{
	int nSelCursel = -1;
	int nCnt = 0;
	POINT ptBkPos = ((CINFCommunity*)m_pParent)->GetCommunityBkPos();
	
	for(nCnt = 0; nCnt<MAX_GUILD_NUMBER; nCnt++)
	{
		int nCheckPosX = (ptBkPos.x + LIST_BOX_NAME_START_X);
		int nCheckPosY = (ptBkPos.y + LIST_BOX_NAME_START_Y + LIST_BOX_INTERVAL*nCnt);										
		if(pt.x >= nCheckPosX 
			&& pt.x < (nCheckPosX + SELECT_GUILD_NAME_WIDTH)
			&& pt.y >= nCheckPosY
			&& pt.y < (nCheckPosY +LIST_BOX_INTERVAL))
		{
			nSelCursel = nCnt;						
			break;
		}					
	}
	if(nSelCursel>=0)
	{		
		int nGuildSelect = m_pScrollMember->GetScrollStep()+nSelCursel;		
		if( nGuildSelect >= (int)m_stGuildInfo.size())
		{
			SetGuildMemberSelect(-1);			
		}
		else
		{
			SetGuildMemberSelect(nGuildSelect);
		}
	}
	else
	{
		// 아무것도 선택안됨
		SetGuildMemberSelect(-1);
	}

	if(bPopupMenu)
	{
		if(m_nGuildSelect != -1)
		{
			UpdateMenuInfo();
			m_pPopupMenu->ShowWindow(TRUE, &pt);
		}
		else
		{
			m_pPopupMenu->ShowWindow(FALSE, NULL);
		}
	}
	else
	{
		m_pPopupMenu->ShowWindow(FALSE, NULL);
	}
	
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		선택한 메뉴 클릭
/// \author		// 2008-04-04 by bhsohn Ep3 커뮤니티 창
/// \date		2008-05-23 ~ 2008-05-23
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFCommunityGuild::OnClickPopupMenu(int nPopupMenuId)
{
	switch(nPopupMenuId)
	{
	case MENU_INFO:// 정보
		{
			RqCharacterUserInfo();
		}
		break;
	case MENU_LETTER:// 편지
		{
			structGuildInfo* pGuild = GetGuildMemeber(m_nGuildSelect);
			if(pGuild)
			{
				((CINFCommunity*)m_pParent)->OnClickLetterWrite(pGuild->MemberName);
			}			
		}
		break;
	case MENU_APPOINT:// 임명
		{
			OnClickAppoint();
		}
		break;
	case MENU_CHARGE:// 위임
		{
			structGuildInfo* pGuild = GetGuildMemeber(m_nGuildSelect);
			if(pGuild)
			{
				char buf[256];
				wsprintf( buf, STRMSG_C_080527_0202, pGuild->MemberName);
				g_pGameMain->m_pInfWindow->AddMsgBox(buf, _Q_GUILD_CHARGE_LEADER, pGuild->MemberUniqueNumber);
			}
		}
		break;
	case MENU_PURGE:// 추방
		{
			OnButtonClicked(BUTTON_BANMEMBER);			
			SetGuildMemberSelect(-1);			// 선택된 길드원 초기화
					
		}
		break;
	}
	

}

void CINFCommunityGuild::RenderQuildRanking()
{
	if(m_nGuildSelect < 0)
	{
		return;
	}	
}

void CINFCommunityGuild::RefreshGuildRankCombo()
{
	int nCnt = 0;
	int nShowComboCnt = 0;
	if(NULL == m_pGuildInfo || 
		(m_pGuildInfo && m_pGuildInfo->GuildState != GUILD_STATE_NORMAL))
	{
		for(nCnt = nShowComboCnt; nCnt < MAX_GUILD_NUMBER;nCnt++)
		{
			if(m_pINFImageComboBox[nCnt])
			{
				m_pINFImageComboBox[nCnt]->ShowWindow(FALSE, NULL);
			}
		}
		return;
	}	

	vector<structGuildInfo*>::iterator itGuInfo = m_stGuildInfo.begin();
	for(nCnt = 0; nCnt < m_pScrollMember->GetScrollStep();nCnt++)
	{
		if(itGuInfo != m_stGuildInfo.end())
		{			
			itGuInfo++;
		}
	}	
	
	while(itGuInfo != m_stGuildInfo.end())
	{	
		structGuildInfo* pInfo = (*itGuInfo);
		if(NULL == pInfo)
		{
			itGuInfo++;
			continue;
		}
		if(nShowComboCnt >= MAX_GUILD_NUMBER)
		{
			break;
		}
		if(m_pINFImageComboBox[nShowComboCnt])
		{
			m_pINFImageComboBox[nShowComboCnt]->ShowWindow(TRUE, NULL);
			m_pINFImageComboBox[nShowComboCnt]->SetCurSel(pInfo->GuildRank);
		}

		nShowComboCnt++;
		itGuInfo++;
	}
	for(nCnt = nShowComboCnt; nCnt < MAX_GUILD_NUMBER;nCnt++)
	{
		if(m_pINFImageComboBox[nCnt])
		{
			m_pINFImageComboBox[nCnt]->ShowWindow(FALSE, NULL);
		}
	}
}

void CINFCommunityGuild::SetGuildMemberSelect(int nGuildSelect)
{
	AllHideComboItem();
	if(nGuildSelect >= (int)m_stGuildInfo.size())
	{
		return;
	}
	
	m_nGuildSelect = nGuildSelect;
}
void CINFCommunityGuild::OnClickTab(int nTab)
{	
	if(nTab >= MAX_GUILD_TAB)
	{
		return;
	}
	m_bTabState[nTab] ^= TRUE;
	switch(nTab)
	{
	case GUILD_TAB_ID:	// 아이디
		{
			if(m_bTabState[nTab])
			{
				sort(m_stGuildInfo.begin(), m_stGuildInfo.end(), CompareNameInc());						
			}
			else
			{
				sort(m_stGuildInfo.begin(), m_stGuildInfo.end(), CompareNameDec());						
			}
		}
		break;
	case GUILD_TAB_GEAR:// 기어
		{
			if(m_bTabState[nTab])
			{
				sort(m_stGuildInfo.begin(), m_stGuildInfo.end(), CompareGearInc());						
			}
			else
			{
				sort(m_stGuildInfo.begin(), m_stGuildInfo.end(), CompareGearDec());						
			}
			
		}
		break;
	case GUILD_TAB_LEVEL:// 레벨
		{
			if(m_bTabState[nTab])
			{
				sort(m_stGuildInfo.begin(), m_stGuildInfo.end(), CompareLVInc());						
			}
			else
			{
				sort(m_stGuildInfo.begin(), m_stGuildInfo.end(), CompareLVDec());						
			}
		}
		break;
	case GUILD_TAB_RANK:	// 계급
		{
			if(m_bTabState[nTab])
			{
				sort(m_stGuildInfo.begin(), m_stGuildInfo.end(), CompareRankInc());						
			}
			else
			{
				sort(m_stGuildInfo.begin(), m_stGuildInfo.end(), CompareRankDec());						
			}
		}
		break;
	case GUILD_TAB_VOICE:	// 음성
		{
		}
		break;
	case GUILD_TAB_CONNECT:	// 접속
		{
			if(m_bTabState[nTab])
			{
				sort(m_stGuildInfo.begin(), m_stGuildInfo.end(), CompareOnOffInc());						
			}
			else
			{
				sort(m_stGuildInfo.begin(), m_stGuildInfo.end(), CompareOnOffDec());						
			}
		}
		break;
	}	
	SetGuildMemberSelect(-1); // 선택화면 초기화
	RefreshGuildRankCombo();	// 콤보 박스 갱신
}

int CINFCommunityGuild::ConvertUnitKind_TO_Order(USHORT i_nUnitKind)
{
	int nOrder = 0;
	switch(i_nUnitKind)
	{
	case UNITKIND_BT01:
	case UNITKIND_BT02:
	case UNITKIND_BT03:
	case UNITKIND_BT04:
		nOrder = 0;
		break;
	case UNITKIND_DT01:
	case UNITKIND_DT02:
	case UNITKIND_DT03:
	case UNITKIND_DT04:
		nOrder = 1;
		break;
	case UNITKIND_ST01:
	case UNITKIND_ST02:
	case UNITKIND_ST03:
	case UNITKIND_ST04:
		nOrder = 2;
		break;
	case UNITKIND_OT01:
	case UNITKIND_OT02:
	case UNITKIND_OT03:
	case UNITKIND_OT04:
		nOrder = 3;
		break;	
	}	
	return nOrder;	
}

structGuildInfo* CINFCommunityGuild::GetGuildMemeber(int nSelect)
{	
	if(-1 == nSelect )
	{
		return NULL;
	}
	int nMemberCnt = (int)m_stGuildInfo.size();
	if(nSelect >= nMemberCnt)
	{
		return NULL;
	}	
	return m_stGuildInfo[nSelect];
}

void CINFCommunityGuild::CloseGuildWnd()
{
	if(m_pEditOpenArea)
	{
		m_pEditOpenArea->EnableEdit(FALSE, FALSE);
	}

}

void CINFCommunityGuild::OnClickAppoint() // 임명
{
	structGuildInfo* pSelInfo = GetGuildMemeber(m_nGuildSelect);
	if((NULL == pSelInfo )
		||	(pSelInfo && GUILD_RANK_COMMANDER == pSelInfo->GuildRank))
	{
		// 여단장은 임명이 안된다.
		AllHideComboItem();
		return;
	}
	// 2008-07-03 by bhsohn EP3 버그 리포트 수정
	UpdateComboBoxItem();
	// end 2008-07-03 by bhsohn EP3 버그 리포트 수정

	// 길드 권한 변경
	if(((m_nGuildRank == GUILD_RANK_COMMANDER) || (m_nGuildRank == GUILD_RANK_SUBCOMMANDER ))
		&& (m_pGuildInfo && m_pGuildInfo->GuildState == GUILD_STATE_NORMAL))
	{	
		int nCnt = 0;		
		int nScreenPos = m_nGuildSelect - m_pScrollMember->GetScrollStep();		

		for(nCnt = 0; nCnt<MAX_GUILD_NUMBER; nCnt++)
		{
			if(NULL == m_pINFImageComboBox[nCnt])
			{
				continue;
			}
			if(nScreenPos == nCnt)
			{
				m_pINFImageComboBox[nCnt]->ShowItem(TRUE);
			}
			else
			{
				m_pINFImageComboBox[nCnt]->ShowItem(FALSE);
			}
		}	
	}
}

void CINFCommunityGuild::UpdateComboBoxItem()
{
	int nCnt = 0;
	// 모든 아이템들 다시 갱신
	int nMainCnt = 0;
	for(nMainCnt =0;nMainCnt<MAX_GUILD_NUMBER;nMainCnt++)
	{
		for(nCnt=0;nCnt<MAX_COMBO_GUILD_RANK;nCnt++)
		{
			BOOL bShowItem = TRUE;
			if(GUILD_RANK_COMMANDER == nCnt)
			{
				// 여단장은 아이템을 안보여준다.
				bShowItem = FALSE;					
			}
			else if(m_nGuildRank == GUILD_RANK_SUBCOMMANDER )
			{
				// 자신이 부여단장일떄 보여지는 아이템
				switch(nCnt)
				{
				case GUILD_RANK_COMMANDER:
				case GUILD_RANK_SUBCOMMANDER:
					{
						bShowItem = FALSE;					
					}
					break;				
				}			
			}		
			m_pINFImageComboBox[nMainCnt]->UpdateItemInfo(nCnt, bShowItem);		
		}
	}
}

BOOL CINFCommunityGuild::UpdateGuildRankInfo(POINT pt)
{
	int nSelectCombo = -1;
	int nSelectEvent = LBTN_COMBO_NONE;
	
	int nMainCnt = 0;
	for(nMainCnt =0;nMainCnt<MAX_GUILD_NUMBER;nMainCnt++)
	{
		if(!m_pINFImageComboBox[nMainCnt]->IsShowItem())
		{
			// 안보이는 아이템은 그냥 지나간다.
			continue;
		}
		int nTmpSelRealUser = nMainCnt + m_pScrollMember->GetScrollStep();
		if(nTmpSelRealUser >= (int)m_stGuildInfo.size())
		{
			continue;				
		}
		structGuildInfo* pSelInfo = GetGuildMemeber(nTmpSelRealUser);
		if((NULL == pSelInfo) 
			||(pSelInfo && GUILD_RANK_COMMANDER == pSelInfo->GuildRank))
		{
			// 여단장은 클릭 이벤트에서 제외
			continue;
		}
		int nTmpEvent = m_pINFImageComboBox[nMainCnt]->OnLButtonDown(pt);
		if(LBTN_COMBO_NONE != nTmpEvent)
		{
			nSelectEvent = nTmpEvent;
			nSelectCombo = nMainCnt;						
			break;
		}
	}
	if(nSelectCombo == -1)
	{
		return FALSE;
	}
	int nSelRealUser = nSelectCombo + m_pScrollMember->GetScrollStep();
	if(nSelRealUser >= (int)m_stGuildInfo.size())
	{
		return FALSE;				
	}
	structGuildInfo* pSelInfo = GetGuildMemeber(nSelRealUser);
	if((NULL == pSelInfo) 
		|| (pSelInfo && GUILD_RANK_COMMANDER == pSelInfo->GuildRank))
	{
		// 여단장은 변경 불가
		return FALSE;				
	}
	
	if(nSelectCombo != -1)
	{
		switch(nSelectEvent)
		{		
		case LBTN_COMBO_CHANGEITEM:
			{
				for(nMainCnt =0;nMainCnt<MAX_GUILD_NUMBER;nMainCnt++)
				{
					if(nMainCnt != nSelectCombo)
					{
						m_pINFImageComboBox[nMainCnt]->ShowItem(FALSE);
					}
				}
				int nGuildRank = m_pINFImageComboBox[nSelectCombo]->GetCurSel();
				m_pINFImageComboBox[nSelectCombo]->SetCurSel(-1);	// 초기화

				if(!IsPossbleChangeRank(nGuildRank))
				{
					g_pD3dApp->m_pChat->CreateChatChild( STRMSG_C_080716_0201, COLOR_ERROR);
					
					// 길드장들급이 미리있다.
					RefreshGuildRankCombo();
					return FALSE;
				}
				

				if(nGuildRank != -1)
				{
					if(!IMSocketSendGuildSetRank(nGuildRank, pSelInfo->MemberUniqueNumber))
					{
						RefreshGuildRankCombo();
					}
				}
			}
			break;
		}		
		return  TRUE;		
	}
	return  FALSE;
}

void CINFCommunityGuild::AllHideComboItem()
{
	int nCnt = 0;
	for(nCnt = 0; nCnt<MAX_GUILD_NUMBER; nCnt++)
	{	
		if(m_pINFImageComboBox[nCnt])
		{
			m_pINFImageComboBox[nCnt]->ShowItem(FALSE);		
		}
	}	

}

// 여단 공지 등록
void CINFCommunityGuild::OnClickRegister()
{
	BOOL bRtn = TRUE;
	if(m_bGuildMember
		&& ((m_nGuildRank == GUILD_RANK_COMMANDER )||(m_nGuildRank == GUILD_RANK_SUBCOMMANDER))
		&& (m_pGuildInfo && m_pGuildInfo->GuildState == GUILD_STATE_NORMAL))
	{
		bRtn = FALSE;
	}
	if(bRtn)
	{
		return;
	}
	// Lock
	{
		vector<MessageType_t> vecUnLockMsg;
		vecUnLockMsg.clear();
		vecUnLockMsg.push_back(T_IC_GUILD_NOTICE_WRITE);		
		vecUnLockMsg.push_back(T_IC_GUILD_NOTICE_WRITE_OK);		
		// 하나만 매칭 되어도 락이 풀린다. 
		g_pD3dApp->EnterMultiLock(FALSE, T_IC_GUILD_NOTICE_WRITE, vecUnLockMsg, MULTI_LOCK_ONE_MATCHING);
	}

	MSG_IC_GUILD_NOTICE_WRITE sMsg;
	memset(&sMsg, 0x00, sizeof(MSG_IC_GUILD_NOTICE_WRITE));
	m_pEditOpenArea->GetString(sMsg.Notice, SIZE_MAX_NOTICE);		
	g_pIMSocket->SendMsg( T_IC_GUILD_NOTICE_WRITE, (char*)&sMsg, sizeof(sMsg));


	if(m_pEditOpenArea->IsEditMode())
	{
		m_pEditOpenArea->BackupTxtString();		
	}
	m_pEditOpenArea->EnableEdit(FALSE, FALSE);

}

void CINFCommunityGuild::GetGuidPeopleInfo(char* pGuildMasterName, char* pGuildSecondName)
{
	vector<structGuildInfo*>::iterator itGuild = m_stGuildInfo.begin();
	while(itGuild != m_stGuildInfo.end())
	{
		structGuildInfo* pInfo = (*itGuild);
		if(NULL == pInfo)
		{
			itGuild++;
			continue;
		}
		if(GUILD_RANK_COMMANDER == pInfo->GuildRank)
		{
			strncpy(pGuildMasterName, pInfo->MemberName, SIZE_MAX_CHARACTER_NAME);
		}
		else if(GUILD_RANK_SUBCOMMANDER == pInfo->GuildRank)
		{
			strncpy(pGuildSecondName, pInfo->MemberName, SIZE_MAX_CHARACTER_NAME);
		}

		itGuild++;
	}

}
void CINFCommunityGuild::SetNoticeWrite(char* pNotice)
{
	if(NULL == m_pEditOpenArea)
	{
		return;
	}
	if(pNotice)
	{
		strncpy(m_chOpenNotice, pNotice, SIZE_MAX_NOTICE);	
		
	}
	else
	{
		memset(m_chOpenNotice, 0x00, SIZE_MAX_NOTICE);
	}
	// 2008-10-29 by bhsohn 에디트 박스 형태 변경
	//int nLienCnt  = m_pEditOpenArea->SetString(m_chOpenNotice, SIZE_MAX_NOTICE);		
	int nLienCnt  = m_pEditOpenArea->SetString(m_chOpenNotice, SIZE_MAX_NOTICE, 0, TRUE);		// 내용

	m_pEditOpenArea->BackupTxtString();
		
	int nMaxStep = max(nLienCnt, MAX_GUILD_LINE);			
		
	// 최대 아이템을 정해주고
	m_pScrollGuildOpen->SetMaxItem(nMaxStep);
	
	// 2008-10-29 by bhsohn 에디트 박스 형태 변경

	
}

void CINFCommunityGuild::UpdateMenuInfo()
{
	m_pPopupMenu->SetMenuShow(MENU_INFO, TRUE);			// 정보
	m_pPopupMenu->SetMenuShow(MENU_LETTER, TRUE);		// 편지
	switch(m_nGuildRank)
	{
	case GUILD_RANK_COMMANDER:
		{
			m_pPopupMenu->SetMenuShow(MENU_APPOINT, TRUE);		// 임명
			m_pPopupMenu->SetMenuShow(MENU_CHARGE, TRUE);		// 위임
			m_pPopupMenu->SetMenuShow(MENU_PURGE, TRUE);		// 추방
		}
		break;
	case GUILD_RANK_SUBCOMMANDER:
		{
			m_pPopupMenu->SetMenuShow(MENU_APPOINT, TRUE);		// 임명
			m_pPopupMenu->SetMenuShow(MENU_CHARGE, FALSE);		// 위임
			m_pPopupMenu->SetMenuShow(MENU_PURGE, TRUE);		// 추방
		}
		break;
	case GUILD_RANK_SQUAD_LEADER_1:	// 제 1 비행대대장
	case GUILD_RANK_SQUAD_LEADER_2:	// 제 2 비행대대장
	case GUILD_RANK_SQUAD_LEADER_3:	// 제 3 비행대대장
	case GUILD_RANK_SQUAD_LEADER_4:	// 제 4 비행대대장
	case GUILD_RANK_SQUAD_LEADER_5:	// 제 5 비행대대장
		{
			m_pPopupMenu->SetMenuShow(MENU_APPOINT, FALSE);		// 임명
			m_pPopupMenu->SetMenuShow(MENU_CHARGE, FALSE);		// 위임
			m_pPopupMenu->SetMenuShow(MENU_PURGE, FALSE);		// 추방
		}
		break;
	case GUILD_RANK_PRIVATE_1:		// 제 1 비행대대원
	case GUILD_RANK_PRIVATE_2:		// 제 2 비행대대원
	case GUILD_RANK_PRIVATE_3:		// 제 3 비행대대원
	case GUILD_RANK_PRIVATE_4:		// 제 4 비행대대원
	case GUILD_RANK_PRIVATE_5:		// 제 5 비행대대원
	case GUILD_RANK_PRIVATE_NULL:	// 무소속 대대원
		{
			m_pPopupMenu->SetMenuShow(MENU_APPOINT, FALSE);		// 임명
			m_pPopupMenu->SetMenuShow(MENU_CHARGE, FALSE);		// 위임
			m_pPopupMenu->SetMenuShow(MENU_PURGE, FALSE);		// 추방
		}
		break;
	default:
		{
			m_pPopupMenu->SetMenuShow(MENU_APPOINT, FALSE);		// 임명
			m_pPopupMenu->SetMenuShow(MENU_CHARGE, FALSE);		// 위임
			m_pPopupMenu->SetMenuShow(MENU_PURGE, FALSE);		// 추방
		}
		break;
	}

}

int CINFCommunityGuild::GetMaxGuildSize()
{
	if(NULL == m_pGuildInfo)
	{
		return 0;
	}
	ATUM_DATE_TIME time = GetServerDateTime();
	BOOL bMember= FALSE;
	if(m_pGuildInfo->GuildMemberShip)
	{		
		if(time < m_pGuildInfo->GuildMemberShipExpireTime)
		{
			// 길드 멤버쉽 기간이냐?
			bMember= TRUE;
		}
	}
	int nTmpGuidSize = CAtumSJ::GetMaxGuildSize(m_pGuildInfo->GuildMemberCapacity, bMember);	
	int nGuildSize = nTmpGuidSize;
	if(bMember)
	{	
		nGuildSize = min(nTmpGuidSize, SIZE_MAX_GUILD_CAPACITY);		
	}
	else
	{
		nGuildSize = min(m_pGuildInfo->GuildMemberCapacity, SIZE_MAX_GUILD_CAPACITY);		
	}
	return nGuildSize;
}

void CINFCommunityGuild::SetMaxGuildMember(int nMemberCapacity)
{
	if(NULL == m_pGuildInfo)
	{
		return;
	}
	m_pGuildInfo->GuildMemberCapacity = nMemberCapacity;
}

void CINFCommunityGuild::SetGuildChangeMemberShip(MSG_IC_GUILD_CHANGE_MEMBERSHIP* pMsg)
{
	if(NULL == m_pGuildInfo)
	{
		return;
	}
	m_pGuildInfo->GuildMemberShipExpireTime = pMsg->MemberShipExpireTime;
	m_pGuildInfo->GuildMemberShip	= pMsg->MemberShip;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		유저정보 요청
/// \author		// 2008-04-04 by bhsohn Ep3 커뮤니티 창
/// \date		2008-05-23 ~ 2008-05-23
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFCommunityGuild::RqCharacterUserInfo()
{
	structGuildInfo* pGuild = GetGuildMemeber(m_nGuildSelect);
	if(NULL == pGuild)
	{
		return;
	}
	((CINFCommunity*)m_pParent)->RqCharacterUserInfo(pGuild->MemberUniqueNumber);
	
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			CINFCommunityGuild::DeleteGuildMember(UINT nUniqueNumber)
/// \brief		길드원 리스트에서 삭제
/// \author		dhkwon
/// \date		2004-05-20 ~ 2004-05-20
/// \warning	길드원이 탈퇴했을때
///				길드원을 추방했을때
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BYTE CINFCommunityGuild::GetGuildOrderIdx(BYTE byRank)
{
	BYTE	byGuildOrderIdx = byRank;
	switch(byRank)
	{
	case GUILD_RANK_PRIVATE_NULL:
		{
			// 일반 길드원은 가장 순위가 낮다.
			byGuildOrderIdx = GUILD_RANK_PRIVATE_5+1;
		}
		break;
	}
	return	byGuildOrderIdx;

}

void CINFCommunityGuild::UpdateBtnPos()
{
	POINT ptBkPos = ((CINFCommunity*)m_pParent)->GetCommunityBkPos();
	int nPosX, nPosY, i;
	nPosX = nPosY = i = 0;
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	{
		RECT rcMouseWhell, rcMousePos;
		POINT ptScrollPos = ptBkPos;

		ptScrollPos.x += 491;
		ptScrollPos.y += 99;

		m_pScrollMember->SetPosition(ptScrollPos.x ,ptScrollPos.y,11,208);
		rcMouseWhell.left		= ptScrollPos.x - 300;
		rcMouseWhell.top		= ptScrollPos.y - 30;
		rcMouseWhell.right		= ptScrollPos.x + 30;
		rcMouseWhell.bottom		= ptScrollPos.y + 252;
		m_pScrollMember->SetMouseWhellRect(rcMouseWhell);
		rcMousePos.left			= ptScrollPos.x - 11;
		rcMousePos.top			= ptScrollPos.y ;
		rcMousePos.right		= rcMousePos.left + 32;
		rcMousePos.bottom		= rcMousePos.top + 282;
		m_pScrollMember->SetMouseBallRect(rcMousePos);
	}
	{
		RECT rcMouseWhell, rcMousePos;
		POINT ptScrollPos = ptBkPos;

		ptScrollPos.x += 161;
		ptScrollPos.y += 219;

		m_pScrollGuildOpen->SetPosition(ptScrollPos.x ,ptScrollPos.y,11,82);
		rcMouseWhell.left		= ptScrollPos.x - 300;
		rcMouseWhell.top		= ptScrollPos.y - 30;
		rcMouseWhell.right		= ptScrollPos.x + 30;
		rcMouseWhell.bottom		= ptScrollPos.y + 100;
		m_pScrollGuildOpen->SetMouseWhellRect(rcMouseWhell);
		rcMousePos.left			= ptScrollPos.x - 11;
		rcMousePos.top			= ptScrollPos.y ;
		rcMousePos.right		= rcMousePos.left + 32;
		rcMousePos.bottom		= rcMousePos.top + 110;
		m_pScrollGuildOpen->SetMouseBallRect(rcMousePos);
	}
	{
		POINT pPos = m_pBackControl->GetFindControlTargetofMinPos("p_sup0");
		nPosX = ptBkPos.x + GUILD_BTN_START_X + pPos.x;
		nPosY = ptBkPos.y + GUILD_BTN_START_Y + pPos.y;		
		m_pSpManageBtn->SetBtnPosition(nPosX, nPosY);	
	}
	{
		POINT pPos = m_pBackControl->GetFindControlTargetofMinPos("Oapp0");
		nPosX = ptBkPos.x + GUILD_BTN_START_X + pPos.x;
		nPosY = ptBkPos.y + GUILD_BTN_START_Y + pPos.y;		
		m_pRegisterBtn->SetBtnPosition(nPosX, nPosY);
	}

	if(m_pInviteBtn)
	{	
		POINT pPos = m_pBackControl->GetFindControlTargetofMinPos("invs00");
		nPosX = ptBkPos.x + GUILD_BTN_START_X + pPos.x;
		nPosY = ptBkPos.y + GUILD_BTN_START_Y + pPos.y;		
		m_pInviteBtn->SetBtnPosition(nPosX, nPosY);	
	}
	if(m_pMarkBtn)
	{		
		POINT pPos = m_pBackControl->GetFindControlTargetofMinPos("p_bt10");
		nPosX = ptBkPos.x + GUILD_BTN_START_X + pPos.x;
		nPosY = ptBkPos.y + GUILD_BTN_START_Y + pPos.y;		
		m_pMarkBtn->SetBtnPosition(nPosX, nPosY);	
	}
	if(m_pOutBtn)
	{		
		POINT pPos = m_pBackControl->GetFindControlTargetofMinPos("outs00");
		nPosX = ptBkPos.x + GUILD_BTN_START_X + pPos.x;
		nPosY = ptBkPos.y + GUILD_BTN_START_Y + pPos.y;		
		m_pOutBtn->SetBtnPosition(nPosX, nPosY);	
	}
	if(m_pCancelBtn)
	{	
		POINT pPos = m_pBackControl->GetFindControlTargetofMinPos("p_bt30");
		nPosX = ptBkPos.x + GUILD_BTN_START_X + pPos.x;
		nPosY = ptBkPos.y + GUILD_BTN_START_Y + pPos.y;		
		m_pCancelBtn->SetBtnPosition(nPosX, nPosY);	
	}
	if(m_pCancelQuildCancelBtn)
	{
		POINT pPos = m_pBackControl->GetFindControlTargetofMinPos("p_bt30");
		nPosX = ptBkPos.x + GUILD_BTN_START_X + pPos.x;
		nPosY = ptBkPos.y + GUILD_BTN_START_Y + pPos.y;		
		m_pCancelQuildCancelBtn->SetBtnPosition(nPosX, nPosY);	
	}
	if(m_pOpBtn)
	{	
		POINT pPos = m_pBackControl->GetFindControlTargetofMinPos("p_bt40");
		nPosX = ptBkPos.x + GUILD_BTN_START_X + pPos.x;
		nPosY = ptBkPos.y + GUILD_BTN_START_Y + pPos.y;
		m_pOpBtn->SetBtnPosition(nPosX, nPosY);	
	}
	if(m_pCrateBtn)
	{	
		POINT pPos = m_pBackControl->GetFindControlTargetofMinPos("p_bt40");
		nPosX = ptBkPos.x + GUILD_BTN_START_X + pPos.x;
		nPosY = ptBkPos.y + GUILD_BTN_START_Y + pPos.y;		
		m_pCrateBtn->SetBtnPosition(nPosX, nPosY);	
	}
	if(m_pSearchBtn)
	{	
		POINT pPos = m_pBackControl->GetFindControlTargetofMinPos("uopbtn0");
		nPosX = ptBkPos.x + GUILD_BTN_START_X + pPos.x;
		nPosY = ptBkPos.y + GUILD_BTN_START_Y + pPos.y;		
		m_pSearchBtn->SetBtnPosition(nPosX, nPosY);	
	}
	if(m_pGuildBattleBtn)
	{	
		POINT pPos = m_pBackControl->GetFindControlTargetofMinPos("p_gwar0");
		nPosX = ptBkPos.x + GUILD_BTN_START_X + pPos.x;
		nPosY = ptBkPos.y + GUILD_BTN_START_Y + pPos.y;		
		m_pGuildBattleBtn->SetBtnPosition(nPosX, nPosY);	
	}
	if(m_pOpOpenBtn)
	{		
		POINT pPos = m_pBackControl->GetFindControlTargetofMinPos("uopbtn0");
		nPosX = ptBkPos.x + GUILD_BTN_START_X + pPos.x;
		nPosY = ptBkPos.y + GUILD_BTN_START_Y + pPos.y;		
		m_pOpOpenBtn->SetBtnPosition(nPosX, nPosY);	
	}
	if(m_pImageTabBtn[0])
	{
		POINT pPos = m_pBackControl->GetFindControlTargetofMinPos("p_li01");
		nPosX = ptBkPos.x + GUILD_BTN_START_X + pPos.x;
		nPosY = ptBkPos.y + GUILD_BTN_START_Y + pPos.y;		
		m_pImageTabBtn[0]->SetBtnPosition(nPosX, nPosY);	
	}

	if(m_pImageTabBtn[1])
	{
		POINT pPos = m_pBackControl->GetFindControlTargetofMinPos("p_li11");
		nPosX = ptBkPos.x + GUILD_BTN_START_X + pPos.x;
		nPosY = ptBkPos.y + GUILD_BTN_START_Y + pPos.y;		
		m_pImageTabBtn[1]->SetBtnPosition(nPosX, nPosY);	
	}

	if(m_pImageTabBtn[2])
	{
		POINT pPos = m_pBackControl->GetFindControlTargetofMinPos("p_li21");
		nPosX = ptBkPos.x + GUILD_BTN_START_X + pPos.x;
		nPosY = ptBkPos.y + GUILD_BTN_START_Y + pPos.y;		
		m_pImageTabBtn[2]->SetBtnPosition(nPosX, nPosY);	
	}

	if(m_pImageTabBtn[3])
	{
		POINT pPos = m_pBackControl->GetFindControlTargetofMinPos("p_li31");
		nPosX = ptBkPos.x + GUILD_BTN_START_X + pPos.x;
		nPosY = ptBkPos.y + GUILD_BTN_START_Y + pPos.y;		
		m_pImageTabBtn[3]->SetBtnPosition(nPosX, nPosY);	
	}

	if(m_pImageTabBtn[4])
	{
		POINT pPos = m_pBackControl->GetFindControlTargetofMinPos("p_li41");
		nPosX = ptBkPos.x + GUILD_BTN_START_X + pPos.x;
		nPosY = ptBkPos.y + GUILD_BTN_START_Y + pPos.y;
		m_pImageTabBtn[4]->SetBtnPosition(nPosX, nPosY);
	}

	if(m_pImageTabBtn[5])
	{
		POINT pPos = m_pBackControl->GetFindControlTargetofMinPos("p_li51");
		nPosX = ptBkPos.x + GUILD_BTN_START_X + pPos.x;
		nPosY = ptBkPos.y + GUILD_BTN_START_Y + pPos.y;
		m_pImageTabBtn[5]->SetBtnPosition(nPosX, nPosY);
	}	
#else 
	{
		RECT rcMouseWhell, rcMousePos;
		POINT ptScrollPos = ptBkPos;

		ptScrollPos.x += 489;
		ptScrollPos.y += 116;

		m_pScrollMember->SetPosition(ptScrollPos.x ,ptScrollPos.y,11,192);
		rcMouseWhell.left		= ptScrollPos.x - 300;
		rcMouseWhell.top		= ptScrollPos.y - 30;
		rcMouseWhell.right		= ptScrollPos.x + 30;
		rcMouseWhell.bottom		= ptScrollPos.y + 252;
		m_pScrollMember->SetMouseWhellRect(rcMouseWhell);
		rcMousePos.left			= ptScrollPos.x - 11;
		rcMousePos.top			= ptScrollPos.y ;
		rcMousePos.right		= rcMousePos.left + 32;
		rcMousePos.bottom		= rcMousePos.top + 282;
		m_pScrollMember->SetMouseBallRect(rcMousePos);
	}
	{
		RECT rcMouseWhell, rcMousePos;
		POINT ptScrollPos = ptBkPos;

		ptScrollPos.x += 168;
		ptScrollPos.y += 225;

		m_pScrollGuildOpen->SetPosition(ptScrollPos.x ,ptScrollPos.y,11,88);
		rcMouseWhell.left		= ptScrollPos.x - 300;
		rcMouseWhell.top		= ptScrollPos.y - 30;
		rcMouseWhell.right		= ptScrollPos.x + 30;
		rcMouseWhell.bottom		= ptScrollPos.y + 100;
		m_pScrollGuildOpen->SetMouseWhellRect(rcMouseWhell);
		rcMousePos.left			= ptScrollPos.x - 11;
		rcMousePos.top			= ptScrollPos.y ;
		rcMousePos.right		= rcMousePos.left + 32;
		rcMousePos.bottom		= rcMousePos.top + 110;
		m_pScrollGuildOpen->SetMouseBallRect(rcMousePos);
	}
	{
		nPosX = ptBkPos.x + 27;
		nPosY = ptBkPos.y + 361;		
		m_pSpManageBtn->SetBtnPosition(nPosX, nPosY);	
	}
	{
		nPosX = ptBkPos.x + 110;
		nPosY = ptBkPos.y + 361;		
		m_pRegisterBtn->SetBtnPosition(nPosX, nPosY);	
	}

	if(m_pInviteBtn)
	{		
		nPosX = ptBkPos.x + 186;
		nPosY = ptBkPos.y + 361;		
		m_pInviteBtn->SetBtnPosition(nPosX, nPosY);	
	}
	if(m_pMarkBtn)
	{		
		nPosX = ptBkPos.x + 249;
		nPosY = ptBkPos.y + 361;		
		m_pMarkBtn->SetBtnPosition(nPosX, nPosY);	
	}
	if(m_pOutBtn)
	{		
		nPosX = ptBkPos.x + 312;
		nPosY = ptBkPos.y + 361;		
		m_pOutBtn->SetBtnPosition(nPosX, nPosY);	
	}
	if(m_pCancelBtn)
	{		
		nPosX = ptBkPos.x + 375;
		nPosY = ptBkPos.y + 361;		
		m_pCancelBtn->SetBtnPosition(nPosX, nPosY);	
	}
	if(m_pCancelQuildCancelBtn)
	{		
		nPosX = ptBkPos.x + 375;
		nPosY = ptBkPos.y + 361;		
		m_pCancelQuildCancelBtn->SetBtnPosition(nPosX, nPosY);	
	}
	if(m_pOpBtn)
	{		
		nPosX = ptBkPos.x + 438;
		nPosY = ptBkPos.y + 361;		
		m_pOpBtn->SetBtnPosition(nPosX, nPosY);	
	}
	if(m_pCrateBtn)
	{		
		nPosX = ptBkPos.x + 438;
		nPosY = ptBkPos.y + 361;		
		m_pCrateBtn->SetBtnPosition(nPosX, nPosY);	
	}
	if(m_pSearchBtn)
	{		
		nPosX = ptBkPos.x + 331;
		nPosY = ptBkPos.y + 57;		
		m_pSearchBtn->SetBtnPosition(nPosX, nPosY);	
	}
	if(m_pGuildBattleBtn)
	{		
		nPosX = ptBkPos.x + 331;
		nPosY = ptBkPos.y + 57;		
		m_pGuildBattleBtn->SetBtnPosition(nPosX, nPosY);	
	}
	if(m_pOpOpenBtn)
	{		
		nPosX = ptBkPos.x + 415;
		nPosY = ptBkPos.y + 57;		
		m_pOpOpenBtn->SetBtnPosition(nPosX, nPosY);	
	
	}
	POINT ptPos[MAX_GUILD_TAB] = 
	{
		{187, 88},
		{271, 88},
		{315, 88},
		{359, 88},
		{403, 88},
		{445, 88},
	};
	for(i =0;i<MAX_GUILD_TAB;i++)
	{
		if(m_pImageTabBtn[i])
		{			
			nPosX = ptBkPos.x + ptPos[i].x;
			nPosY = ptBkPos.y + ptPos[i].y;		
			m_pImageTabBtn[i]->SetBtnPosition(nPosX, nPosY);	
		}
	}
#endif

	
	
	if (m_pEditOpenArea)
	{		
		nPosX = ptBkPos.x + GUILD_OPENAREA_EDIT_X;
		nPosY = ptBkPos.y + GUILD_OPENAREA_EDIT_Y;		
		m_pEditOpenArea->SetPos(nPosX, nPosY);	
	}
	
	for (int nMainCnt =0;nMainCnt<MAX_GUILD_NUMBER;nMainCnt++)
	{
		if(m_pINFImageComboBox[nMainCnt])
		{
			POINT ptPosTmp;
			ptPosTmp.x = ptBkPos.x + LIST_BOX_RANK_START_X;
			ptPosTmp.y = ptBkPos.y + LIST_BOX_RANK_START_Y + LIST_BOX_INTERVAL*nMainCnt;						
			
			m_pINFImageComboBox[nMainCnt]->SetPos(ptPosTmp);
		}
	}
}
// 2016-07-19 panoskj removed this
//BOOL CINFCommunityGuild::SetVoiceGuild(UID32_t i_CharacterUID)
//{
//	vector<structGuildInfo*>::iterator itGuInfo = m_stGuildInfo.begin();
//	while(itGuInfo != m_stGuildInfo.end())
//	{
//		if(i_CharacterUID == (*itGuInfo)->MemberUniqueNumber)
//		{
//			(*itGuInfo)->IsUseVOIP = TRUE;
//			return TRUE;
//		}
//		itGuInfo++;
//	}
//	return FALSE;
//}
BOOL CINFCommunityGuild::NoneVoiceGuild(UID32_t i_CharacterUID)
{
	for (auto info : m_stGuildInfo)
	{
		if (i_CharacterUID == info->MemberUniqueNumber)
		{
			info->IsUseVOIP = false;
			return true;
		}
	}
	return false;
}

// ReSharper disable once CppMemberFunctionMayBeConst
void CINFCommunityGuild::SetNtoNCallState(BOOL bState)
{ 
	// 2016-07-19 panoskj disabled this
	// todo: remove

	// m_bNtoNCallState = bState; 
}	// 여단통화중!

BOOL CINFCommunityGuild::IsPossbleChangeRank(int nGuildRank)
{
	switch(nGuildRank)
	{
	case GUILD_RANK_SUBCOMMANDER:// 부여단장
	case GUILD_RANK_SQUAD_LEADER_1:
	case GUILD_RANK_SQUAD_LEADER_2:
	case GUILD_RANK_SQUAD_LEADER_3:
	case GUILD_RANK_SQUAD_LEADER_4:
	case GUILD_RANK_SQUAD_LEADER_5:
		{
		}
		break;
	default:
		{
			return TRUE;
		}
		break;
	}
	vector<structGuildInfo*>::iterator itGuInfo = m_stGuildInfo.begin();
	while(itGuInfo != m_stGuildInfo.end())
	{
		if((*itGuInfo)->GuildRank == nGuildRank)
		{
			// 이미 직급이 있다.
			return FALSE;
		}
		itGuInfo++;
	}
	return TRUE;
}

void CINFCommunityGuild::SetGuildRank(INT nGuildTotalFameRank, INT nGuildMonthlyFameRank)
{
	if(NULL == m_pGuildInfo)
	{
		return;
	}
	m_pGuildInfo->GuildTotalFameRank = nGuildTotalFameRank;
	m_pGuildInfo->GuildMonthlyFameRank = nGuildMonthlyFameRank;
}

// 2008-10-14 by bhsohn VOIP개선
void CINFCommunityGuild::SetVOIPSpeaking(char *i_pUserName, BOOL i_bEnableTalking)
{
	vector<structGuildInfo*>::iterator itGuInfo = m_stGuildInfo.begin();
	while(itGuInfo != m_stGuildInfo.end())
	{
		if(!stricmp((*itGuInfo)->MemberName, i_pUserName))
		{
			(*itGuInfo)->IsUseSpeakeingVOIP = i_bEnableTalking;
			RefreshGuildRankCombo();	// 콤보 박스 갱신
			return;
		}					
		itGuInfo++;
	}	
}

void CINFCommunityGuild::ReleaseVOIPInfo(char *i_pUserName)
{
	vector<structGuildInfo*>::iterator itGuInfo = m_stGuildInfo.begin();
	while(itGuInfo != m_stGuildInfo.end())
	{
		if(!stricmp((*itGuInfo)->MemberName, i_pUserName))
		{
			(*itGuInfo)->IsUseSpeakeingVOIP = FALSE;
			(*itGuInfo)->IsUseVOIP = FALSE;
			RefreshGuildRankCombo();	// 콤보 박스 갱신
			return;
		}					
		itGuInfo++;
	}	

}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2008-10-20 by bhsohn VOIP 버그 수정
/// \date		2008-10-20 ~ 2008-10-20
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CINFCommunityGuild::IsVoiceONGuildMaster()
{
	vector<structGuildInfo*>::iterator itGuild = m_stGuildInfo.begin();
	while(itGuild != m_stGuildInfo.end())
	{
		structGuildInfo* pInfo = (*itGuild);
		if(NULL == pInfo)
		{
			itGuild++;
			continue;
		}

		if(GUILD_RANK_COMMANDER == pInfo->GuildRank)		
		{
			return pInfo->IsUseVOIP;			
		}					
		itGuild++;
	}	
	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2008-10-20 by bhsohn VOIP 버그 수정
/// \date		2008-10-20 ~ 2008-10-20
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFCommunityGuild::ReleaseVOIPGuildInfo()
{
	vector<structGuildInfo*>::iterator itGuild = m_stGuildInfo.begin();
	while(itGuild != m_stGuildInfo.end())
	{
		structGuildInfo* pInfo = (*itGuild);
		if(NULL == pInfo)
		{
			itGuild++;
			continue;
		}
		
		pInfo->IsUseSpeakeingVOIP = FALSE;
		itGuild++;
	}	
	RefreshGuildRankCombo();	// 콤보 박스 갱신

}