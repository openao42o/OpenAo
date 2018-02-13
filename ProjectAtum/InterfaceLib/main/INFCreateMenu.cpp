// INFCreateMenu.cpp: implementation of the CINFCreateMenu class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RangeTime.h"
#include "INFUnitCreateInfo.h"
#include "INFCreateMenu.h"
#include "AtumApplication.h"
#include "oleauto.h"
#include "INFSelect.h"
#include "D3DHanFont.h"
#include "INFImage.h"
#include "GameDataLast.h"
#include "AbuseFilter.h"
#include "INFWindow.h"
#include "INFSelectBack.h"
#include "INFPilotFace.h"
#include "FieldWinSocket.h"
#include "INFSelectMenu.h"
#include "AtumSJ.h"
#include "AtumSound.h"
#include "ShuttleChild.h"
#include "CharacterChild.h"				// 2005-07-21 by ispark
#include "dxutil.h"

// 2011. 10. 10 by jskim UI½Ã½ºÅÛ º¯°æ
#include "INFImageEx.h"
#include "INFGroupImage.h"
#include "INFGroupManager.h"
#include "INFControl.h"
#include "INFGameMain.h"
// end 2011. 10. 10 by jskim UI½Ã½ºÅÛ º¯°æ

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI½Ã½ºÅÛ º¯°æ

#define DIS_EFF_STEP_TIME							0.5

#define CREATE_FONT_COLOR		RGB(222,222,222)

#define CRE_BUTTON_STATE_UP		0
#define CRE_BUTTON_STATE_DOWN	1
#define CRE_BUTTON_STATE_NORMAL	2

#define POS_CRE_TITLE_X		g_pD3dApp->GetBackBufferDesc().Width /2 - ( ( m_pTitle->GetMaxPos().x - m_pTitle->GetMinPos().x) / 2 )
#define POS_CRE_BACK_X		g_pD3dApp->GetBackBufferDesc().Width /2 - ( ( m_pBack->GetMaxPos().x - m_pBack->GetMinPos().x) / 2 )
#define POS_CRE_BACK_Y		g_pD3dApp->GetBackBufferDesc().Height - 148

#define POS_CRE_CHAR_FACE_X	POS_CRE_BACK_X+200
#define POS_CRE_CHAR_FACE_Y	POS_CRE_BACK_Y+37

// 2005-07-05 by ispark
// CreateMenu UI º¯°æ¿¡ µû¸¥ ÁÂÇ¥ ¼öÁ¤
//#define POS_CRE_OK_START_X		POS_CRE_BACK_X+448
//#define POS_CRE_CANCEL_START_X	POS_CRE_BACK_X+525
#define POS_CRE_LEFT_START_X		POS_CRE_BACK_X+169
#define POS_CRE_LEFT_START_Y		POS_CRE_BACK_Y+70


#define CRE_BUTTON_OK		0
#define CRE_BUTTON_CANCEL	1 
#define CRE_BUTTON_LEFT		2
#define CRE_BUTTON_RIGHT	3
#define CRE_BUTTON_UP		4
#define CRE_BUTTON_DOWN		5

#define CRE_SIZE_BUTTON_X(i)	(i > 1 ? 28 : 77)
#define CRE_SIZE_BUTTON_Y(i)	(i > 1 ? 36 : 36)

#define POS_CRE_CHARACTER_INFO_START_X	-100
#define DISTANCE_CRE_CHARACTER_INFO_X	88
#define DISTANCE_CRE_CHARACTER_INFO_Y	20

#define POS_CRE_INFO_X	240
#define POS_CRE_INFO_Y	464
// Ä³¸¯ÅÍ Ãß°¡½Ã ¾Æ·¡ ¼ýÀÚ¸¦ Áõ°¡½ÃÄÑ¾ß ÇÑ´Ù.
//#define CRE_FEMALE_LAST_NUMBER	3
//#define CRE_MAN_LAST_NUMBER		103

#define CREATE_CHARACTER_INFO_RACE		0	// Á¾Á·

#define CREATE_CHARACTER_INFO_DODGE		2	// È¸ÇÇ
#define CREATE_CHARACTER_INFO_ATTACK	3	// °ø°Ý
#define CREATE_CHARACTER_INFO_SOUL		4	// °¨ÀÀ
#define CREATE_CHARACTER_INFO_FUEL		5	// ¿¬·á
#define CREATE_CHARACTER_INFO_DEFENSE	6	// ¹æ¾î
#define CREATE_CHARACTER_INFO_ENDURANCE	7	// ³»±¸

#define CRE_CHANGE_SHOW_CURSOR_TIME		0.7f

#define CREATE_CHARACTER_INFO_A			0
#define CREATE_CHARACTER_INFO_B			1
#define CREATE_CHARACTER_INFO_M			3
#define CREATE_CHARACTER_INFO_I			2

#define CREATE_CHARACTER_INFO_UNIT_STYLE_U	-1
#define CREATE_CHARACTER_INFO_UNIT_STYLE_A	0
#define CREATE_CHARACTER_INFO_UNIT_STYLE_D	1
#define CREATE_CHARACTER_INFO_UNIT_STYLE_F	2

#define CREATE_CHARACTER_INFO_RADIO_DEF		1
#define CREATE_CHARACTER_INFO_RADIO_SEL		0

#define CREATE_CHARACTER_FOCUS_NAME			0
#define CREATE_CHARACTER_FOCUS_STEC			1
#define CREATE_CHARACTER_FOCUS_NAME_UNABLE	2
#define CREATE_CHARACTER_FOCUS_SAME_NAME	3
#define CREATE_CHARACTER_FOCUS_NONE			-1

#define CREATE_CHARACTER_INFO_STAT_X		((g_pD3dApp->GetBackBufferDesc().Width - 400)/2)
#define CREATE_CHARACTER_INFO_STAT_Y		(g_pD3dApp->GetBackBufferDesc().Height-217)

#define FADE_TIME							2.0f

#define CRE_UP								(g_pD3dApp->GetBackBufferDesc().Width/800.0f) * 50.0f

// 2006-03-07 by ispark, ¾ð¾î¿¡ µû¶ó À§Ä¡ ¼öÁ¤
#if defined(LANGUAGE_ENGLISH) || defined(LANGUAGE_VIETNAM)|| defined(LANGUAGE_THAI)// 2008-04-30 by bhsohn ÅÂ±¹ ¹öÀü Ãß°¡
	//#define POS_CRE_CHARACTER_NAME_X		215//243
	#ifdef INNOVA_RELEASE
		#define POS_CRE_CHARACTER_NAME_X		( g_pD3dApp->GetBackBufferDesc().Width / 2) - (size.cx / 2)//243	// 2008-09-18 by bhsohn ·¯½Ã¾Æ Ä³¸¯ »ý¼ºÃ¢¿¡¼­ Ä³¸¯ÀÔ·Â À§Ä¡ º¯°æ
	#else
		#define POS_CRE_CHARACTER_NAME_X		( g_pD3dApp->GetBackBufferDesc().Width / 2) - (size.cx / 2)//243
	#endif
	#define POS_CRE_CHARACTER_NAME_Y		POS_CRE_BACK_Y + 5//32
	#define POS_CRE_CHARACTER_INFO_START_Y	48//50 
	#define CREATE_CHARACTER_INFO_RACE_X	10
	#define CREATE_CHARACTER_FOCUS_NAME_X	207
#else
#define POS_CRE_CHARACTER_NAME_X		( g_pD3dApp->GetBackBufferDesc().Width / 2) - (size.cx / 2)
#define POS_CRE_CHARACTER_NAME_Y		POS_CRE_BACK_Y + 5//32
#define POS_CRE_CHARACTER_INFO_START_Y	48//48//50		// 2006-08-11 by dgwooÄ³¸¯ÅÍ »ý¼º½Ã Y°ª À§Ä¡ º¯°æ.
#define CREATE_CHARACTER_INFO_RACE_X	10
#define CREATE_CHARACTER_FOCUS_NAME_X	232
#endif		

#else	
#define CREATE_FONT_COLOR		RGB(222,222,222)

#define CRE_BUTTON_STATE_UP		0
#define CRE_BUTTON_STATE_DOWN	1
#define CRE_BUTTON_STATE_NORMAL	2

#define POS_CRE_TITLE_X		g_pD3dApp->GetBackBufferDesc().Width /2 - 294/2
#define POS_CRE_BACK_X		g_pD3dApp->GetBackBufferDesc().Width /2 - 640/2	
#define POS_CRE_BACK_Y		g_pD3dApp->GetBackBufferDesc().Height - 148

#define POS_CRE_CHAR_FACE_X	POS_CRE_BACK_X+46
#define POS_CRE_CHAR_FACE_Y	POS_CRE_BACK_Y+26

// 2005-07-05 by ispark
// CreateMenu UI º¯°æ¿¡ µû¸¥ ÁÂÇ¥ ¼öÁ¤
//#define POS_CRE_OK_START_X		POS_CRE_BACK_X+448
#define POS_CRE_OK_START_Y		POS_CRE_BACK_Y+67
//#define POS_CRE_CANCEL_START_X	POS_CRE_BACK_X+525
#define POS_CRE_CANCEL_START_Y	POS_CRE_BACK_Y+67
#define POS_CRE_OK_START_X		POS_CRE_BACK_X+411
#define POS_CRE_CANCEL_START_X	POS_CRE_BACK_X+488

#define POS_CRE_LEFT_START_X		POS_CRE_BACK_X-21
#define POS_CRE_LEFT_START_Y		POS_CRE_BACK_Y+61
#define POS_CRE_RIGHT_START_X		POS_CRE_BACK_X+108
#define POS_CRE_RIGHT_START_Y		POS_CRE_BACK_Y+61

#define CRE_BUTTON_OK		0
#define CRE_BUTTON_CANCEL	1
#define CRE_BUTTON_LEFT		2
#define CRE_BUTTON_RIGHT	3

#define CRE_SIZE_BUTTON_X(i)	(i > 1 ? 28 : 77)
#define CRE_SIZE_BUTTON_Y(i)	(i > 1 ? 36 : 36)

#define POS_CRE_CHARACTER_INFO_START_X	256
#define DISTANCE_CRE_CHARACTER_INFO_X	88
#define DISTANCE_CRE_CHARACTER_INFO_Y	18

// Ä³¸¯ÅÍ Ãß°¡½Ã ¾Æ·¡ ¼ýÀÚ¸¦ Áõ°¡½ÃÄÑ¾ß ÇÑ´Ù.
//#define CRE_FEMALE_LAST_NUMBER	3
//#define CRE_MAN_LAST_NUMBER		103

#define CREATE_CHARACTER_INFO_RACE		0	// Á¾Á·

#define CREATE_CHARACTER_INFO_DODGE		2	// È¸ÇÇ
#define CREATE_CHARACTER_INFO_ATTACK	3	// °ø°Ý
#define CREATE_CHARACTER_INFO_SOUL		4	// °¨ÀÀ
#define CREATE_CHARACTER_INFO_FUEL		5	// ¿¬·á
#define CREATE_CHARACTER_INFO_DEFENSE	6	// ¹æ¾î
#define CREATE_CHARACTER_INFO_ENDURANCE	7	// ³»±¸

#define CRE_CHANGE_SHOW_CURSOR_TIME		0.7f

#define CREATE_CHARACTER_INFO_A			0
#define CREATE_CHARACTER_INFO_B			1
#define CREATE_CHARACTER_INFO_M			3
#define CREATE_CHARACTER_INFO_I			2

#define CREATE_CHARACTER_INFO_UNIT_STYLE_U	-1
#define CREATE_CHARACTER_INFO_UNIT_STYLE_A	0
#define CREATE_CHARACTER_INFO_UNIT_STYLE_D	1
#define CREATE_CHARACTER_INFO_UNIT_STYLE_F	2

#define CREATE_CHARACTER_INFO_RADIO_DEF		1
#define CREATE_CHARACTER_INFO_RADIO_SEL		0

#define CREATE_CHARACTER_FOCUS_NAME			0
#define CREATE_CHARACTER_FOCUS_STEC			1
#define CREATE_CHARACTER_FOCUS_NAME_UNABLE	2
#define CREATE_CHARACTER_FOCUS_SAME_NAME	3
#define CREATE_CHARACTER_FOCUS_NONE			-1

#define CREATE_CHARACTER_INFO_STAT_X		((g_pD3dApp->GetBackBufferDesc().Width - 400)/2)
#define CREATE_CHARACTER_INFO_STAT_Y		(g_pD3dApp->GetBackBufferDesc().Height-217)

// 2006-03-07 by ispark, ¾ð¾î¿¡ µû¶ó À§Ä¡ ¼öÁ¤
#if defined(LANGUAGE_ENGLISH) || defined(LANGUAGE_VIETNAM)|| defined(LANGUAGE_THAI)// 2008-04-30 by bhsohn ÅÂ±¹ ¹öÀü Ãß°¡
	//#define POS_CRE_CHARACTER_NAME_X		215//243
	#ifdef INNOVA_RELEASE
		#define POS_CRE_CHARACTER_NAME_X		225//243	// 2008-09-18 by bhsohn ·¯½Ã¾Æ Ä³¸¯ »ý¼ºÃ¢¿¡¼­ Ä³¸¯ÀÔ·Â À§Ä¡ º¯°æ
	#else
		#define POS_CRE_CHARACTER_NAME_X		215//243
	#endif
	#define POS_CRE_CHARACTER_NAME_Y		28//32
	#define POS_CRE_CHARACTER_INFO_START_Y	48//50
	#define CREATE_CHARACTER_INFO_RACE_X	25
	#define CREATE_CHARACTER_FOCUS_NAME_X	207
#else
#define POS_CRE_CHARACTER_NAME_X		243
#define POS_CRE_CHARACTER_NAME_Y		28//32
#define POS_CRE_CHARACTER_INFO_START_Y	47//48//50		// 2006-08-11 by dgwooÄ³¸¯ÅÍ »ý¼º½Ã Y°ª À§Ä¡ º¯°æ.
#define CREATE_CHARACTER_INFO_RACE_X	30
#define CREATE_CHARACTER_FOCUS_NAME_X	232
#endif

#endif
CINFCreateMenu::CINFCreateMenu(CAtumNode* pParent)
{
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI½Ã½ºÅÛ º¯°æ
	FLOG( "CINFCreateMenu(CAtumNode* pParent)" );
	m_pParent = pParent;
	m_bRestored = FALSE;
	m_pGameData = NULL;

	// 2005-07-05 by ispark
	// ÀÚµ¿ ½ºÅÈ ºÐ¹è »èÁ¦
//	for(int i=0; i<CRE_CHARACTER_UNIT_NUMBER; i++)
//		m_pBack[i] = NULL;
	//m_pBack = NULL;
//	for(i=0; i<CRE_CHARACTER_RADIO_BUTTON_NUMBER; i++)
//		m_pImgRadio[i] = NULL;
	
	//m_pTitle = NULL;
	m_pImgFocus[0] = NULL;
	m_pImgFocus[1] = NULL;
	m_pImgFocus[2] = NULL;
	m_pImgFocus[3] = NULL;
	memset(&m_createUnit, 0x00, sizeof(MSG_FC_CHARACTER_CREATE));

	int i;
	for(i=0;i<4;i++)
	{
		m_pButton[CRE_BUTTON_OK][i] = NULL;
		m_pButton[CRE_BUTTON_CANCEL][i] = NULL;
		m_pButton[CRE_BUTTON_LEFT][i] = NULL;
		m_pButton[CRE_BUTTON_RIGHT][i] = NULL;
		m_pButton[CRE_BUTTON_UP][i] = NULL;
		m_pButton[CRE_BUTTON_DOWN][i] = NULL;

	}
	m_fButtonPos[CRE_BUTTON_OK][0] = 0.0f;
	m_fButtonPos[CRE_BUTTON_OK][1] = 0.0f;
	m_fButtonPos[CRE_BUTTON_CANCEL][0] = 0.0f;
	m_fButtonPos[CRE_BUTTON_CANCEL][1] = 0.0f;
	m_fButtonPos[CRE_BUTTON_LEFT][0] = 0.0f;
	m_fButtonPos[CRE_BUTTON_LEFT][1] = 0.0f;
	m_fButtonPos[CRE_BUTTON_UP][0] = 0.0f;
	m_fButtonPos[CRE_BUTTON_UP][1] = 0.0f;
	m_fButtonPos[CRE_BUTTON_DOWN][0] = 0.0f;
	m_fButtonPos[CRE_BUTTON_DOWN][1] = 0.0f;		

	m_nButtonState[CRE_BUTTON_OK] = CRE_BUTTON_STATE_NORMAL;
	m_nButtonState[CRE_BUTTON_CANCEL] = CRE_BUTTON_STATE_NORMAL;
	m_nButtonState[CRE_BUTTON_LEFT] = CRE_BUTTON_STATE_NORMAL;
	m_nButtonState[CRE_BUTTON_RIGHT] = CRE_BUTTON_STATE_NORMAL;
	m_nButtonState[CRE_BUTTON_UP] = CRE_BUTTON_STATE_NORMAL;
	m_nButtonState[CRE_BUTTON_DOWN] = CRE_BUTTON_STATE_NORMAL;

	// 2009-02-10 by bhsohn Japan Charcter Create
	//memset(m_strCharacterName, 0x00, SIZE_MAX_CHARACTER_NAME);
	ZERO_MEMORY(m_strCharacterName);
	// end 2009-02-10 by bhsohn Japan Charcter Create
	m_pFontCharacterName = NULL;
	memset(m_pFontCharacterInfo, 0x00, sizeof(DWORD)*CRE_CHARACTER_INFO_NUMBER);
	m_nCharacterNamePos[0] = 0;
	m_nCharacterNamePos[1] = 0;
	memset(*m_nCharacterInfoPos, 0x00, sizeof(DWORD)*SEL_CHARACTER_INFO_NUMBER*2);

	m_bShowCursor = FALSE;
	m_fChangeShowCursorTime = 0.0f;

	m_bBlocking = FALSE;
	m_nSelectUnit = UNITKIND_BGEAR;
	for(i=0; i<CREATE_CHARACTER_RADIO_ALL_AUTTON; i++)
		m_nRadioButton[i] = CREATE_CHARACTER_INFO_RADIO_DEF;
//	m_nSelectUnitStyle = CREATE_CHARACTER_INFO_UNIT_STYLE_U;	// 2005-07-04 by ispark ÀÚµ¿ ½ºÅÈ ºÐ¹è »èÁ¦
	m_nSelectUnitStyle = CREATE_CHARACTER_INFO_UNIT_STYLE_F;
	m_nFocus = CREATE_CHARACTER_FOCUS_NONE;
	m_nFocusStat  = CREATE_CHARACTER_FOCUS_NONE;
//	m_nCreateUnitStat = -1;
	m_nCreateUnitStat = AUTOSTAT_TYPE_FREESTYLE;				// 2005-07-04 by ispark ÀÚµ¿ ½ºÅÈ ºÐ¹è »èÁ¦
//	m_nRenderToolTipIndex = -1;									// 2005-07-04 by ispark ÀÚµ¿ ½ºÅÈ ºÐ¹è »èÁ¦
	for(i=0; i<3; i++)
		m_pImgTextPopUp[i] = NULL;
	// 2005-09-12 by ispark
	// »ç¿ëÇÒ ¼ö ÀÖ´Â FACE ³Ñ¹ö¸¦ ºÎ¿©ÇÑ´Ù.
	for(i = 0; i < CRE_TOTAL_NUMBER; i++)
		m_nFaceUseIndex[i] = -1;
	//¿©ÀÚ Ä³¸¯ÅÍ 
	m_nFaceUseIndex[0] = 0;
	m_nFaceUseIndex[1] = 1;
	m_nFaceUseIndex[2] = 2;
	m_nFaceUseIndex[3] = 3;
	m_nFaceUseIndex[4] = 4;
	//³²ÀÚ Ä³¸¯ÅÍ
	m_nFaceUseIndex[5] = 100;
	m_nFaceUseIndex[6] = 101;
	m_nFaceUseIndex[7] = 102;
	m_nFaceUseIndex[8] = 103;
	m_nFaceUseIndex[9] = 104;
	

	// ³²ÀÚ¸¦ ±âÁØÀ¸·Î Àâ¾Ò´Ù
	m_nChoiseFaceIndex = 6;
	m_createUnit.PilotFace = m_nFaceUseIndex[m_nChoiseFaceIndex];

	// 2011. 10. 18 by jskim EP4 UIº¯°æ - createÃ¢ º¯°æ
	m_GruopCreatemanager = NULL;
	m_pTitle = NULL;
	m_pBack = NULL;
	m_pBackTemp = NULL;
	m_bEffectFrame = FALSE;
	m_PControlCrebtn = NULL;
	// end 2011. 10. 18 by jskim EP4 UIº¯°æ - createÃ¢ º¯°æ
	m_nBtnUpFadeMode = FADE_IN;
	m_fBtnUpFadeInTime = 0;
	m_dwBtnUpBkAlpha = 0x00ffffff;

	m_nBtnDownFadeMode = FADE_NORMAL;
	m_fBtnDownFadeInTime = 0;
	m_dwBtnUDownpBkAlpha = 0xffffffff;

	m_bRenderInfoBtn = TRUE;  
#else
	FLOG( "CINFCreateMenu(CAtumNode* pParent)" );
	m_pParent = pParent;
	m_bRestored = FALSE;
	m_pGameData = NULL;

	// 2005-07-05 by ispark
	// ÀÚµ¿ ½ºÅÈ ºÐ¹è »èÁ¦
//	for(int i=0; i<CRE_CHARACTER_UNIT_NUMBER; i++)
//		m_pBack[i] = NULL;
	m_pBack = NULL;
	
//	for(i=0; i<CRE_CHARACTER_RADIO_BUTTON_NUMBER; i++)
//		m_pImgRadio[i] = NULL;
	
	m_pTitle = NULL;
	m_pImgFocus[0] = NULL;
	m_pImgFocus[1] = NULL;
	m_pImgFocus[2] = NULL;
	m_pImgFocus[3] = NULL;
	for(int i=0;i<9;i++)
		m_pImgToolTip[i] = NULL;
	memset(&m_createUnit, 0x00, sizeof(MSG_FC_CHARACTER_CREATE));

	for(i=0;i<2;i++)
	{
		m_pButton[CRE_BUTTON_OK][i] = NULL;
		m_pButton[CRE_BUTTON_CANCEL][i] = NULL;
		m_pButton[CRE_BUTTON_LEFT][i] = NULL;
		m_pButton[CRE_BUTTON_RIGHT][i] = NULL;
	}
	m_fButtonPos[CRE_BUTTON_OK][0] = 0.0f;
	m_fButtonPos[CRE_BUTTON_OK][1] = 0.0f;
	m_fButtonPos[CRE_BUTTON_CANCEL][0] = 0.0f;
	m_fButtonPos[CRE_BUTTON_CANCEL][1] = 0.0f;
	m_fButtonPos[CRE_BUTTON_LEFT][0] = 0.0f;
	m_fButtonPos[CRE_BUTTON_LEFT][1] = 0.0f;
	m_fButtonPos[CRE_BUTTON_RIGHT][0] = 0.0f;
	m_fButtonPos[CRE_BUTTON_RIGHT][1] = 0.0f;

	m_nButtonState[CRE_BUTTON_OK] = CRE_BUTTON_STATE_NORMAL;
	m_nButtonState[CRE_BUTTON_CANCEL] = CRE_BUTTON_STATE_NORMAL;
	m_nButtonState[CRE_BUTTON_LEFT] = CRE_BUTTON_STATE_NORMAL;
	m_nButtonState[CRE_BUTTON_RIGHT] = CRE_BUTTON_STATE_NORMAL;

	// 2009-02-10 by bhsohn Japan Charcter Create
	//memset(m_strCharacterName, 0x00, SIZE_MAX_CHARACTER_NAME);
	ZERO_MEMORY(m_strCharacterName);
	// end 2009-02-10 by bhsohn Japan Charcter Create
	m_pFontCharacterName = NULL;
	memset(m_pFontCharacterInfo, 0x00, sizeof(DWORD)*CRE_CHARACTER_INFO_NUMBER);
	m_nCharacterNamePos[0] = 0;
	m_nCharacterNamePos[1] = 0;
	memset(*m_nCharacterInfoPos, 0x00, sizeof(DWORD)*SEL_CHARACTER_INFO_NUMBER*2);

	m_bShowCursor = FALSE;
	m_fChangeShowCursorTime = 0.0f;

	m_bBlocking = FALSE;
	m_nSelectUnit = UNITKIND_BGEAR;
	for(i=0; i<CREATE_CHARACTER_RADIO_ALL_AUTTON; i++)
		m_nRadioButton[i] = CREATE_CHARACTER_INFO_RADIO_DEF;
//	m_nSelectUnitStyle = CREATE_CHARACTER_INFO_UNIT_STYLE_U;	// 2005-07-04 by ispark ÀÚµ¿ ½ºÅÈ ºÐ¹è »èÁ¦
	m_nSelectUnitStyle = CREATE_CHARACTER_INFO_UNIT_STYLE_F;
	m_nFocus = CREATE_CHARACTER_FOCUS_NONE;
	m_nFocusStat  = CREATE_CHARACTER_FOCUS_NONE;
//	m_nCreateUnitStat = -1;
	m_nCreateUnitStat = AUTOSTAT_TYPE_FREESTYLE;				// 2005-07-04 by ispark ÀÚµ¿ ½ºÅÈ ºÐ¹è »èÁ¦
//	m_nRenderToolTipIndex = -1;									// 2005-07-04 by ispark ÀÚµ¿ ½ºÅÈ ºÐ¹è »èÁ¦
	for(i=0; i<3; i++)
		m_pImgTextPopUp[i] = NULL;
	// 2005-09-12 by ispark
	// »ç¿ëÇÒ ¼ö ÀÖ´Â FACE ³Ñ¹ö¸¦ ºÎ¿©ÇÑ´Ù.
	for(i = 0; i < CRE_TOTAL_NUMBER; i++)
		m_nFaceUseIndex[i] = -1;
	//¿©ÀÚ Ä³¸¯ÅÍ 
	m_nFaceUseIndex[0] = 0;
	m_nFaceUseIndex[1] = 1;
	m_nFaceUseIndex[2] = 2;
	m_nFaceUseIndex[3] = 3;
	m_nFaceUseIndex[4] = 4;
	//³²ÀÚ Ä³¸¯ÅÍ
	m_nFaceUseIndex[5] = 100;
	m_nFaceUseIndex[6] = 101;
	m_nFaceUseIndex[7] = 102;
	m_nFaceUseIndex[8] = 103;
	m_nFaceUseIndex[9] = 104;
	

	// ³²ÀÚ¸¦ ±âÁØÀ¸·Î Àâ¾Ò´Ù
	m_nChoiseFaceIndex = 6;
	m_createUnit.PilotFace = m_nFaceUseIndex[m_nChoiseFaceIndex];
#endif

}

CINFCreateMenu::~CINFCreateMenu()
{
	FLOG( "~CINFCreateMenu()" );

	// 2005-07-05 by ispark
	// ÀÚµ¿ ½ºÅÈ ºÐ¹è »èÁ¦
//	for(int i=0; i<CRE_CHARACTER_UNIT_NUMBER; i++)
//		util::del( m_pBack[i] );
	util::del(m_pBack);
//	for(i=0; i<CRE_CHARACTER_RADIO_BUTTON_NUMBER; i++)
//		util::del( m_pImgRadio[i] );
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI½Ã½ºÅÛ º¯°æ
	util::del( m_pTitle );	
	// 2011. 10. 18 by jskim EP4 UIº¯°æ - createÃ¢ º¯°æ
	util::del( m_GruopCreatemanager );
	util::del( m_PControlCrebtn );
	// end 2011. 10. 18 by jskim EP4 UIº¯°æ - createÃ¢ º¯°æ
	util::del( m_pImgFocus[0] );
	util::del( m_pImgFocus[1] );
	util::del( m_pImgFocus[2] );
	util::del( m_pImgFocus[3] );

	int i;
	for(i=0;i<CRE_BUTTON_NUMBER;i++)
	{
		util::del(m_pButton[i][0]);
		util::del(m_pButton[i][1]);
		util::del(m_pButton[i][2]);
		util::del(m_pButton[i][3]);
	} 
#else 
	util::del( m_pTitle );
	util::del( m_pImgFocus[0] );
	util::del( m_pImgFocus[1] );
	util::del( m_pImgFocus[2] );
	util::del( m_pImgFocus[3] );
	for(int i=0;i<9;i++)
		util::del( m_pImgToolTip[i]);
	for(i=0;i<CRE_BUTTON_NUMBER;i++)
	{
		util::del(m_pButton[i][0]);
		util::del(m_pButton[i][1]);
	}
#endif
	util::del( m_pFontCharacterName );
	for(i=0;i<SEL_CHARACTER_INFO_NUMBER; i++)
		util::del( m_pFontCharacterInfo[i] );
	for(i=0; i<3; i++)
		util::del(m_pImgTextPopUp[i]);
}

HRESULT CINFCreateMenu::InitDeviceObjects()
{
	FLOG( "CINFCreateMenu::InitDeviceObjects()" );

	char buf[32];
	DataHeader * pDataHeader;

#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI½Ã½ºÅÛ º¯°æ
	// 2011. 10. 18 by jskim EP4 UIº¯°æ - CreateÃ¢ º¯°æ
	m_GruopCreatemanager = new CINFGroupManager( g_pD3dApp->m_pImageList, m_pGameData );
	m_GruopCreatemanager->InitDeviceObjects();	
	pDataHeader = m_GruopCreatemanager->FindResource("Cretitle");
	m_pTitle = m_GruopCreatemanager->GetGroupImage( pDataHeader );
	m_pTitle->InitDeviceObjects( g_pD3dApp->m_pImageList );

	pDataHeader = m_GruopCreatemanager->FindResource("Creinfo");
	m_pBack = m_GruopCreatemanager->GetGroupImage( pDataHeader );
	m_pBack->InitDeviceObjects( g_pD3dApp->m_pImageList );

	pDataHeader = m_GruopCreatemanager->FindResource("Crebtn");
	m_PControlCrebtn = m_GruopCreatemanager->GetGroupImage( pDataHeader );

	// end 2011. 10. 18 by jskim EP4 UIº¯°æ - CreateÃ¢ º¯°æ
	m_pFontCharacterName = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),10, D3DFONT_ZENABLE,  TRUE,512,32);
	m_pFontCharacterName->InitDeviceObjects(g_pD3dDev);

	int i;
	for(i=0;i<CRE_CHARACTER_INFO_NUMBER; i++)
	{
		m_pFontCharacterInfo[i] = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9, D3DFONT_ZENABLE,  FALSE,128,32);
		m_pFontCharacterInfo[i]->InitDeviceObjects(g_pD3dDev);
	}

	// 2005-07-05 by ispark
	// ÀÚµ¿ ½ºÅÈ ºÐ¹è »èÁ¦ 
//	for(i=0; i<CRE_CHARACTER_UNIT_NUMBER; i++)
//	{
//		wsprintf(buf, "creback");
//		m_pBack[i] = new CINFImage;
//		pDataHeader = FindResource(buf);
//		m_pBack[i]->InitDeviceObjects(pDataHeader->m_pData,pDataHeader->m_DataSize);
// 		m_pBack = new CINFImage;
// 		pDataHeader = FindResource(buf);
// 		m_pBack->InitDeviceObjects(pDataHeader->m_pData,pDataHeader->m_DataSize);
//	}

//	m_pImgRadio[0] = new CINFImage;
//	pDataHeader = FindResource("radio_a");
//	m_pImgRadio[0]->InitDeviceObjects(pDataHeader->m_pData,pDataHeader->m_DataSize);
//	m_pImgRadio[1] = new CINFImage;
//	pDataHeader = FindResource("radio_b");
//	m_pImgRadio[1]->InitDeviceObjects(pDataHeader->m_pData,pDataHeader->m_DataSize);
	
	m_pImgFocus[0] = new CINFImageEx;
	pDataHeader = FindResource("selfocn");
	m_pImgFocus[0]->InitDeviceObjects( pDataHeader );
	m_pImgFocus[1] = new CINFImageEx;
	pDataHeader = FindResource("selfocs");
	m_pImgFocus[1]->InitDeviceObjects( pDataHeader );
	m_pImgFocus[2] = new CINFImageEx;
	pDataHeader = FindResource("selfoc1");
	m_pImgFocus[2]->InitDeviceObjects( pDataHeader );
	m_pImgFocus[3] = new CINFImageEx;
	pDataHeader = FindResource("selfoc2");
	m_pImgFocus[3]->InitDeviceObjects( pDataHeader );

// 	m_pTitle = new CINFImage;
// 	pDataHeader = FindResource("cretitle");
// 	m_pTitle->InitDeviceObjects(pDataHeader->m_pData,pDataHeader->m_DataSize);
	
	for(i=0;i<CRE_BUTTON_NUMBER - 2;i++)
	{
		for(int j=0;j<4;j++)
		{
			wsprintf( buf, "crebtn%d%d",i,j);
			m_pButton[i][j] =new CINFImageEx;
			pDataHeader = FindResource(buf);
			if(pDataHeader)
				m_pButton[i][j]->InitDeviceObjects( pDataHeader );
//			else
//				ERROR Reporting ( exit )
		}
	}

	
	
	{
		wsprintf(buf, "creback0");
		//		m_pBack[i] = new CINFImage;
		//		pDataHeader = FindResource(buf);
		//		m_pBack[i]->InitDeviceObjects(pDataHeader->m_pData,pDataHeader->m_DataSize);
		m_pBackTemp = new CINFImageEx;
		pDataHeader = FindResource(buf);
		m_pBackTemp->InitDeviceObjects(pDataHeader);
		
	}

	
	for(i=0;i< 4;i++)
	{
		wsprintf( buf, "arrowd0%d",i);
		m_pButton[CRE_BUTTON_UP][i] = new CINFImageEx;
		pDataHeader = FindResource(buf);
		if(pDataHeader)
			m_pButton[CRE_BUTTON_UP][i]->InitDeviceObjects( pDataHeader );

		wsprintf( buf, "arrowu0%d",i);
		m_pButton[CRE_BUTTON_DOWN][i] = new CINFImageEx;
		pDataHeader = FindResource(buf);
		if(pDataHeader)
			m_pButton[CRE_BUTTON_DOWN][i]->InitDeviceObjects( pDataHeader );
	}
	
	for(i=0; i<3; i++)
	{
		wsprintf(buf, "boxtip%d",i);		
		m_pImgTextPopUp[i] = new CINFImageEx;
		pDataHeader = FindResource(buf);
		m_pImgTextPopUp[i]->InitDeviceObjects( pDataHeader );
	}
#else 
	m_pFontCharacterName = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),10, D3DFONT_ZENABLE,  TRUE,512,32);
	m_pFontCharacterName->InitDeviceObjects(g_pD3dDev);

	for(int i=0;i<CRE_CHARACTER_INFO_NUMBER; i++)
	{
		m_pFontCharacterInfo[i] = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9, D3DFONT_ZENABLE,  FALSE,128,32);
		m_pFontCharacterInfo[i]->InitDeviceObjects(g_pD3dDev);
	}

	// 2005-07-05 by ispark
	// ÀÚµ¿ ½ºÅÈ ºÐ¹è »èÁ¦ 
//	for(i=0; i<CRE_CHARACTER_UNIT_NUMBER; i++)
//	{
		wsprintf(buf, "creback");
//		m_pBack[i] = new CINFImage;
//		pDataHeader = FindResource(buf);
//		m_pBack[i]->InitDeviceObjects(pDataHeader->m_pData,pDataHeader->m_DataSize);
		m_pBack = new CINFImageEx;
		pDataHeader = FindResource(buf);
		m_pBack->InitDeviceObjects(pDataHeader);


	
//	}

//	m_pImgRadio[0] = new CINFImage;
//	pDataHeader = FindResource("radio_a");
//	m_pImgRadio[0]->InitDeviceObjects(pDataHeader->m_pData,pDataHeader->m_DataSize);
//	m_pImgRadio[1] = new CINFImage;
//	pDataHeader = FindResource("radio_b");
//	m_pImgRadio[1]->InitDeviceObjects(pDataHeader->m_pData,pDataHeader->m_DataSize);
	
	m_pImgFocus[0] = new CINFImageEx;
	pDataHeader = FindResource("selfocn");
	m_pImgFocus[0]->InitDeviceObjects(pDataHeader);
	m_pImgFocus[1] = new CINFImageEx;
	pDataHeader = FindResource("selfocs");
	m_pImgFocus[1]->InitDeviceObjects(pDataHeader);
	m_pImgFocus[2] = new CINFImageEx;
	pDataHeader = FindResource("selfoc1");
	m_pImgFocus[2]->InitDeviceObjects(pDataHeader);
	m_pImgFocus[3] = new CINFImageEx;
	pDataHeader = FindResource("selfoc2");
	m_pImgFocus[3]->InitDeviceObjects(pDataHeader);

	for(i=0;i<9;i++)
	{
		wsprintf( buf, "toimg%d",i);
		m_pImgToolTip[i] = new CINFImageEx;
		pDataHeader = FindResource(buf);
		m_pImgToolTip[i]->InitDeviceObjects(pDataHeader);
	}	

	m_pTitle = new CINFImageEx;
	pDataHeader = FindResource("cretitle");
	m_pTitle->InitDeviceObjects(pDataHeader);
	
	for(i=0;i<CRE_BUTTON_NUMBER;i++)
	{
		for(int j=0;j<2;j++)
		{
			wsprintf( buf, "crebtn%d%d",i,j);
			m_pButton[i][j] =new CINFImageEx;
			pDataHeader = FindResource(buf);
			if(pDataHeader)
				m_pButton[i][j]->InitDeviceObjects(pDataHeader);
//			else
//				ERROR Reporting ( exit )
		}
	}
	
	for(i=0; i<3; i++)
	{
		wsprintf(buf, "boxtip%d",i);		
		m_pImgTextPopUp[i] = new CINFImageEx;
		pDataHeader = FindResource(buf);
		m_pImgTextPopUp[i]->InitDeviceObjects( pDataHeader );
	}
#endif						  

	return S_OK ;
}


HRESULT CINFCreateMenu::RestoreDeviceObjects()
{
	FLOG( "CINFCreateMenu::RestoreDeviceObjects()" );
	m_pTitle->RestoreDeviceObjects();
	m_pBack->RestoreDeviceObjects();

	m_pFontCharacterName->RestoreDeviceObjects();
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI½Ã½ºÅÛ º¯°æ
// 	m_nCharacterNamePos[0] = POS_CRE_BACK_X+POS_CRE_CHARACTER_NAME_X;
// 	m_nCharacterNamePos[1] = POS_CRE_BACK_Y+POS_CRE_CHARACTER_NAME_Y;

					        // 2011. 10. 10 by jskim UI½Ã½ºÅÛ º¯°æ
	m_pBackTemp->RestoreDeviceObjects();
	int i;
	for(i=0;i<CRE_CHARACTER_INFO_NUMBER; i++)
	{
		m_pFontCharacterInfo[i]->RestoreDeviceObjects();
#ifndef C_EPSODE4_UI_CHANGE_JSKIM
		m_nCharacterInfoPos[i][0] = POS_CRE_BACK_X+POS_CRE_CHARACTER_INFO_START_X+DISTANCE_CRE_CHARACTER_INFO_X*(i%2);
		m_nCharacterInfoPos[i][1] = POS_CRE_BACK_Y+POS_CRE_CHARACTER_INFO_START_Y+DISTANCE_CRE_CHARACTER_INFO_Y*(i/2);
#else
		m_nCharacterInfoPos[i][0] = POS_CRE_LEFT_START_X+POS_CRE_INFO_X+DISTANCE_CRE_CHARACTER_INFO_X*(i%2);
		m_nCharacterInfoPos[i][1] = POS_CRE_BACK_Y+POS_CRE_CHARACTER_INFO_START_Y+DISTANCE_CRE_CHARACTER_INFO_Y*(i/2);
#endif
	}
	// 2005-07-05 by ispark
	// ÀÚµ¿ ½ºÅÈ ºÐ¹è »èÁ¦
//	for(i=0; i<CRE_CHARACTER_UNIT_NUMBER; i++)
//		m_pBack[i]->RestoreDeviceObjects();
// 	m_pBack->RestoreDeviceObjects();
// //	for(i=0; i<CRE_CHARACTER_RADIO_BUTTON_NUMBER; i++)
// //		m_pImgRadio[i]->RestoreDeviceObjects();
// 
// 	m_pTitle->RestoreDeviceObjects();
//	DataHeader * pDataHeader;
	
	// 2011. 10. 18 by jskim EP4 UIº¯°æ - CreateÃ¢ º¯°æ
// 	pDataHeader = m_GruopCreatemanager->FindResource("Cretitle");
// 	m_pTitle = m_GruopCreatemanager->GetGroupImage( pDataHeader );
// 	m_pTitle->InitDeviceObjects( g_pD3dApp->m_pImageList );
// 	m_pTitle->RestoreDeviceObjects();
// 	
// 	pDataHeader = m_GruopCreatemanager->FindResource("Creinfo");
// 	m_pBack = m_GruopCreatemanager->GetGroupImage( pDataHeader );
// 	m_pBack->InitDeviceObjects( g_pD3dApp->m_pImageList );
// 	m_pBack->RestoreDeviceObjects();


	m_pImgFocus[0]->RestoreDeviceObjects();
	m_pImgFocus[1]->RestoreDeviceObjects();
	m_pImgFocus[2]->RestoreDeviceObjects();
	m_pImgFocus[3]->RestoreDeviceObjects();
	
	for(i=0;i<CRE_BUTTON_NUMBER;i++)
	{
		m_pButton[i][0]->RestoreDeviceObjects();
		m_pButton[i][1]->RestoreDeviceObjects();
		m_pButton[i][2]->RestoreDeviceObjects();
		m_pButton[i][3]->RestoreDeviceObjects();
	}
	
	
	POINT pMinPos = m_PControlCrebtn->GetFindControlTargetofMinPos( "crebtn00" );
	m_fButtonPos[CRE_BUTTON_OK][0] = POS_CRE_LEFT_START_X + pMinPos.x;
	m_fButtonPos[CRE_BUTTON_OK][1] = POS_CRE_LEFT_START_Y + pMinPos.y;

	pMinPos = m_PControlCrebtn->GetFindControlTargetofMinPos( "crebtn10" );
	m_fButtonPos[CRE_BUTTON_CANCEL][0] = POS_CRE_LEFT_START_X + pMinPos.x;
	m_fButtonPos[CRE_BUTTON_CANCEL][1] = POS_CRE_LEFT_START_Y + pMinPos.y;

	pMinPos = m_PControlCrebtn->GetFindControlTargetofMinPos( "crebtn20" );
	m_fButtonPos[CRE_BUTTON_LEFT][0] = POS_CRE_LEFT_START_X + pMinPos.x;
	m_fButtonPos[CRE_BUTTON_LEFT][1] = POS_CRE_LEFT_START_Y + pMinPos.y;

	pMinPos = m_PControlCrebtn->GetFindControlTargetofMinPos( "crebtn30" );
	m_fButtonPos[CRE_BUTTON_RIGHT][0] = POS_CRE_LEFT_START_X + pMinPos.x;
	m_fButtonPos[CRE_BUTTON_RIGHT][1] = POS_CRE_LEFT_START_Y + pMinPos.y;

	m_fButtonPos[CRE_BUTTON_UP][0] = g_pD3dApp->GetBackBufferDesc().Width /2 - m_pButton[CRE_BUTTON_UP][0]->GetImgSize().x / 2;
	m_fButtonPos[CRE_BUTTON_UP][1] = (int)(CRE_UP + 130);

	m_fButtonPos[CRE_BUTTON_DOWN][0] = g_pD3dApp->GetBackBufferDesc().Width /2 - m_pButton[CRE_BUTTON_DOWN][0]->GetImgSize().x / 2;
	m_fButtonPos[CRE_BUTTON_DOWN][1] = (int)(CRE_UP + 455);
#else			   
	m_nCharacterNamePos[0] = POS_CRE_BACK_X+POS_CRE_CHARACTER_NAME_X;
	m_nCharacterNamePos[1] = POS_CRE_BACK_Y+POS_CRE_CHARACTER_NAME_Y;
	for(int i=0;i<CRE_CHARACTER_INFO_NUMBER; i++)
	{
		m_pFontCharacterInfo[i]->RestoreDeviceObjects();
		m_nCharacterInfoPos[i][0] = POS_CRE_BACK_X+POS_CRE_CHARACTER_INFO_START_X+DISTANCE_CRE_CHARACTER_INFO_X*(i%2);
		m_nCharacterInfoPos[i][1] = POS_CRE_BACK_Y+POS_CRE_CHARACTER_INFO_START_Y+DISTANCE_CRE_CHARACTER_INFO_Y*(i/2);
	}

	// 2005-07-05 by ispark
	// ÀÚµ¿ ½ºÅÈ ºÐ¹è »èÁ¦
//	for(i=0; i<CRE_CHARACTER_UNIT_NUMBER; i++)
//		m_pBack[i]->RestoreDeviceObjects();
//	m_pBack->RestoreDeviceObjects();
//	for(i=0; i<CRE_CHARACTER_RADIO_BUTTON_NUMBER; i++)
//		m_pImgRadio[i]->RestoreDeviceObjects();

//	m_pTitle->RestoreDeviceObjects();
	m_pImgFocus[0]->RestoreDeviceObjects();
	m_pImgFocus[1]->RestoreDeviceObjects();
	m_pImgFocus[2]->RestoreDeviceObjects();
	m_pImgFocus[3]->RestoreDeviceObjects();
	for(i=0;i<9;i++)
		m_pImgToolTip[i]->RestoreDeviceObjects();
	for(i=0;i<CRE_BUTTON_NUMBER;i++)
	{
		m_pButton[i][0]->RestoreDeviceObjects();
		m_pButton[i][1]->RestoreDeviceObjects();
	}
	
	m_fButtonPos[CRE_BUTTON_OK][0] = POS_CRE_OK_START_X;
	m_fButtonPos[CRE_BUTTON_OK][1] = POS_CRE_OK_START_Y;
	m_fButtonPos[CRE_BUTTON_CANCEL][0] = POS_CRE_CANCEL_START_X;
	m_fButtonPos[CRE_BUTTON_CANCEL][1] = POS_CRE_CANCEL_START_Y;
	m_fButtonPos[CRE_BUTTON_LEFT][0] = POS_CRE_LEFT_START_X;
	m_fButtonPos[CRE_BUTTON_LEFT][1] = POS_CRE_LEFT_START_Y;
	m_fButtonPos[CRE_BUTTON_RIGHT][0] = POS_CRE_RIGHT_START_X;
	m_fButtonPos[CRE_BUTTON_RIGHT][1] = POS_CRE_RIGHT_START_Y;
#endif
	for(i=0; i<3; i++)
	{
		if(m_pImgTextPopUp[i])
			m_pImgTextPopUp[i]->RestoreDeviceObjects();
	}
	
	m_bRestored = TRUE;
	return S_OK ;
}

HRESULT CINFCreateMenu::InvalidateDeviceObjects()
{
	FLOG( "CINFCreateMenu::InvalidateDeviceObjects()" );
	m_pBack->InvalidateDeviceObjects();
	m_pTitle->InvalidateDeviceObjects();

	m_pFontCharacterName->InvalidateDeviceObjects();
	int i;
	for(i=0;i<CRE_CHARACTER_INFO_NUMBER; i++)
	{
		m_pFontCharacterInfo[i]->InvalidateDeviceObjects();
	}

	// 2005-07-05 by ispark
	// ÀÚµ¿ ½ºÅÈ ºÐ¹è »èÁ¦
//	for(i=0; i<CRE_CHARACTER_UNIT_NUMBER; i++)
//		m_pBack[i]->InvalidateDeviceObjects();

	m_pImgFocus[0]->InvalidateDeviceObjects();
	m_pImgFocus[1]->InvalidateDeviceObjects();
	m_pImgFocus[2]->InvalidateDeviceObjects();
	m_pImgFocus[3]->InvalidateDeviceObjects();
#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI½Ã½ºÅÛ º¯°æ
	for(i=0;i<9;i++)
		m_pImgToolTip[i]->InvalidateDeviceObjects();
#endif
	for(i=0;i<CRE_BUTTON_NUMBER;i++)
	{
		m_pButton[i][0]->InvalidateDeviceObjects();
		m_pButton[i][1]->InvalidateDeviceObjects();
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI½Ã½ºÅÛ º¯°æ
		m_pButton[i][2]->InvalidateDeviceObjects();
		m_pButton[i][3]->InvalidateDeviceObjects();	   
#endif
	}
	for(i=0; i<3; i++)
	{
		if ( m_pImgTextPopUp[i] )
			m_pImgTextPopUp[i]->InvalidateDeviceObjects();
	}
	
	m_bRestored = FALSE;
	return S_OK ;
}

HRESULT CINFCreateMenu::DeleteDeviceObjects()
{
	FLOG( "CINFCreateMenu::DeleteDeviceObjects()" );
	m_pFontCharacterName->DeleteDeviceObjects();
	util::del(m_pFontCharacterName);
	int i;
	for(i=0;i<CRE_CHARACTER_INFO_NUMBER; i++)
	{
		m_pFontCharacterInfo[i]->DeleteDeviceObjects();
		util::del(m_pFontCharacterInfo[i]);
	}

	// 2005-07-05 by ispark
	// ÀÚµ¿ ½ºÅÈ ºÐ¹è »èÁ¦
//	for(i=0; i<CRE_CHARACTER_UNIT_NUMBER; i++)
//	{
//		m_pBack[i]->DeleteDeviceObjects();
//		util::del(m_pBack[i]);
//	}
	util::del(m_pBack);
//	for(i=0; i<CRE_CHARACTER_RADIO_BUTTON_NUMBER; i++)
//	{
//		m_pImgRadio[i]->DeleteDeviceObjects();
//		util::del(m_pImgRadio[i]);
//	}		

	m_pTitle->DeleteDeviceObjects();
	util::del(m_pTitle);
	m_pImgFocus[0]->DeleteDeviceObjects();
	util::del(m_pImgFocus[0]);
	m_pImgFocus[1]->DeleteDeviceObjects();
	util::del(m_pImgFocus[1]);
	m_pImgFocus[2]->DeleteDeviceObjects();
	util::del(m_pImgFocus[2]);
	m_pImgFocus[3]->DeleteDeviceObjects();
	util::del(m_pImgFocus[3]);
#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI½Ã½ºÅÛ º¯°æ
	for(i=0;i<9;i++)
	{
		m_pImgToolTip[i]->DeleteDeviceObjects();
		util::del(m_pImgToolTip[i]);
	}
	m_pBackTemp->DeleteDeviceObjects();
	util::del(m_pBackTemp);
	
#endif
	for(i=0;i<CRE_BUTTON_NUMBER;i++)
	{
		m_pButton[i][0]->DeleteDeviceObjects();
		m_pButton[i][1]->DeleteDeviceObjects();
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI½Ã½ºÅÛ º¯°æ
		m_pButton[i][2]->DeleteDeviceObjects();
		m_pButton[i][3]->DeleteDeviceObjects();
#endif
		util::del(m_pButton[i][0]);
		util::del(m_pButton[i][1]);
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI½Ã½ºÅÛ º¯°æ
		util::del(m_pButton[i][2]);
		util::del(m_pButton[i][3]);
#endif
	}
	
	for(i=0; i<3; i++)
	{
		if ( m_pImgTextPopUp[i] )
			m_pImgTextPopUp[i]->DeleteDeviceObjects();

		util::del(m_pImgTextPopUp[i]);
	}


	return S_OK ;
}

void CINFCreateMenu::Tick()
{
	FLOG( "CINFCreateMenu::Tick()" );
	m_fChangeShowCursorTime += g_pD3dApp->GetElapsedTime();
	if(m_fChangeShowCursorTime > CRE_CHANGE_SHOW_CURSOR_TIME)
	{
		m_bShowCursor = !m_bShowCursor;
		m_fChangeShowCursorTime = 0;
	}
 #ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI½Ã½ºÅÛ º¯°æ

	//2011-10-06 by jhahn ÆÄÆ®³Ê ¼ºÀåÇü ½Ã½ºÅÛ
	m_fTimeEffectBG += g_pD3dApp->GetElapsedTime();		
	
	if(m_fTimeEffectBG >= DIS_EFF_STEP_TIME )
	{
		m_fTimeEffectBG = NULL;
		m_nEffStep += 1;
		if(m_nEffStep >= 2)
		{
			m_nEffStep = NULL;
		}
	}
	
// 	m_tBlinkTime.Tick( g_pD3dApp->GetElapsedTime() );
// 	
// 	if( m_tBlinkTime.IsOverEnd() )
// 		m_tBlinkTime.Start();			  
//end 2011-10-06 by jhahn ÆÄÆ®³Ê ¼ºÀåÇü ½Ã½ºÅÛ
	switch(m_nBtnUpFadeMode)
	{
	case FADE_IN: 
		{
			m_fBtnUpFadeInTime += g_pD3dApp->GetCheckElapsedTime();			
			if(m_fBtnUpFadeInTime > FADE_TIME)
			{
				m_nBtnUpFadeMode = FADE_NORMAL;				 
				m_dwBtnUpBkAlpha = 0xffffffff;
				m_fBtnUpFadeInTime = 0;	
				break;
			}
			m_dwBtnUpBkAlpha = g_pGameMain->GetCurrentColor( m_fBtnUpFadeInTime, ALPHA_FADE_IN, FADE_TIME);	
		}
		break;
	case FADE_OUT:
		{
			m_fBtnUpFadeInTime += g_pD3dApp->GetCheckElapsedTime();			
			if(m_fBtnUpFadeInTime > FADE_TIME)
			{
				m_nBtnUpFadeMode = FADE_NORMAL;				 
				m_dwBtnUpBkAlpha = 0;
				m_fBtnUpFadeInTime = 0;
				break;				
			}
			m_dwBtnUpBkAlpha = g_pGameMain->GetCurrentColor( m_fBtnUpFadeInTime, ALPHA_FADE_OUT, FADE_TIME);
		}
		break;
	}
	switch(m_nBtnDownFadeMode)
	{
	case FADE_IN: 
		{
			m_fBtnDownFadeInTime += g_pD3dApp->GetCheckElapsedTime();			
			if(m_fBtnDownFadeInTime > FADE_TIME)
			{
				m_nBtnDownFadeMode = FADE_NORMAL;				 
				m_dwBtnUDownpBkAlpha = 0xffffffff;
				m_fBtnDownFadeInTime = 0;
				break;				
			}
			m_dwBtnUDownpBkAlpha = g_pGameMain->GetCurrentColor( m_fBtnDownFadeInTime, ALPHA_FADE_IN, FADE_TIME);	
		}
		break;
	case FADE_OUT:
		{
			m_fBtnDownFadeInTime += g_pD3dApp->GetCheckElapsedTime();			
			if(m_fBtnDownFadeInTime > FADE_TIME)
			{
				m_nBtnDownFadeMode = FADE_NORMAL;				 
				m_dwBtnUDownpBkAlpha = 0;
				m_fBtnDownFadeInTime = 0;
				break;				
			}
			m_dwBtnUDownpBkAlpha = g_pGameMain->GetCurrentColor(m_fBtnDownFadeInTime, ALPHA_FADE_OUT, FADE_TIME);
		}
	break;
	}	 
#endif
}

void CINFCreateMenu::Render()
{
	FLOG( "CINFCreateMenu::Render()" );
	m_pTitle->Move(POS_CRE_TITLE_X, 0);
	m_pTitle->Render();	

	// 2005-07-05 by ispark
	// ÀÚµ¿ ½ºÅÈ ºÐ¹è »èÁ¦
//	m_pBack[m_nSelectUnit]->Move(POS_CRE_BACK_X, POS_CRE_BACK_Y);
//	m_pBack[m_nSelectUnit]->Render();
	m_pBack->Move(POS_CRE_BACK_X, POS_CRE_BACK_Y);
	m_pBack->Render();

	if (m_nEffStep)
	{
		m_pBackTemp->Move(POS_CRE_BACK_X, POS_CRE_BACK_Y + 33);
		m_pBackTemp->Render();}
	

	for(int i=0;i<CRE_BUTTON_NUMBER;i++)
	{
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI½Ã½ºÅÛ º¯°æ
		if( m_bRenderInfoBtn == FALSE &&  i == CRE_BUTTON_UP || m_bRenderInfoBtn == TRUE &&  i == CRE_BUTTON_DOWN )
		{
			continue;
		}
		if(m_nButtonState[i] != CRE_BUTTON_STATE_NORMAL)
		{
			if( i == CRE_BUTTON_UP )
			{
				m_pButton[i][m_nButtonState[i]]->SetColor(m_dwBtnUpBkAlpha);
			}
			if( i == CRE_BUTTON_DOWN )
			{
				m_pButton[i][m_nButtonState[i]]->SetColor(m_dwBtnUDownpBkAlpha);
			}
		
			m_pButton[i][m_nButtonState[i]]->Move(m_fButtonPos[i][0],m_fButtonPos[i][1]);
			m_pButton[i][m_nButtonState[i]]->Render();

		
		}
		else
		{
			if( i == CRE_BUTTON_UP )
			{
				m_pButton[i][3]->SetColor(m_dwBtnUpBkAlpha);
			}
			if( i == CRE_BUTTON_DOWN )
			{
				m_pButton[i][3]->SetColor(m_dwBtnUDownpBkAlpha);
			}
	

			if ( ( i == CRE_BUTTON_DOWN )|| ( i == CRE_BUTTON_UP ) )
			{
				if (m_nEffStep)
				{
			m_pButton[i][3]->Move(m_fButtonPos[i][0],m_fButtonPos[i][1]);
			m_pButton[i][3]->Render();
				}				
			}
			else
			{
				m_pButton[i][3]->Move(m_fButtonPos[i][0],m_fButtonPos[i][1]);
				m_pButton[i][3]->Render();
			}
			

			
			
		}  
#else
		if(m_nButtonState[i] != CRE_BUTTON_STATE_NORMAL)
		{
			m_pButton[i][m_nButtonState[i]]->Move(m_fButtonPos[i][0]+37,m_fButtonPos[i][1]);
			m_pButton[i][m_nButtonState[i]]->Render();
		}

#endif
	}

	CINFImageEx* pFace = ((CINFSelect*)m_pParent)->m_pPilotFace->FindPilotImage(m_createUnit.PilotFace);
	if(pFace)
	{
		pFace->Move(POS_CRE_CHAR_FACE_X,POS_CRE_CHAR_FACE_Y);
		pFace->Render();
	}
	char buf[1024];
	memset(buf, 0x00, sizeof(buf));
	if(m_bShowCursor)
		wsprintf(buf, "%s_", m_strCharacterName);
	else if(strlen(m_strCharacterName)>0)
		//strcpy( buf, m_strCharacterName);
		wsprintf(buf, "%s ", m_strCharacterName);
	if(buf[0])
	{
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI½Ã½ºÅÛ º¯°æ
		SIZE size = m_pFontCharacterName->GetStringSize( buf );
		m_pFontCharacterName->DrawText(POS_CRE_CHARACTER_NAME_X, POS_CRE_CHARACTER_NAME_Y, CREATE_FONT_COLOR, buf, 0L);
#else 
		m_pFontCharacterName->DrawText(m_nCharacterNamePos[0], m_nCharacterNamePos[1], CREATE_FONT_COLOR, buf, 0L);
#endif
	}

	if(SET_CREATEMENU_CANDIDATE == g_nRenderCandidate)
		g_pD3dApp->RenderCandidate(m_nCharacterNamePos[0], m_nCharacterNamePos[1]-15);

	// 2007-05-21 by bhsohn China IME Working
	g_pD3dApp->RenderIMEType(m_nCharacterNamePos[0], m_nCharacterNamePos[1]-15);

//	strcpy( buf, GetRaceString(m_createUnit.Race));
	if(COMPARE_RACE(m_createUnit.Race, RACE_DECA))
		strcpy( buf, STRMSG_C_SELECT_0001);//"µ¥Ä«"
	if(COMPARE_RACE(m_createUnit.Race, RACE_BATTALUS))
		strcpy( buf, STRMSG_C_SELECT_0002);//"¹ÙÅ»·¯½º"
	if(COMPARE_RACE(m_createUnit.Race, RACE_DEMO))
		strcpy( buf, STRMSG_C_SELECT_0003);//"µ¥¸ð"
	if(COMPARE_RACE(m_createUnit.Race, RACE_GAMEMASTER))
		strcpy( buf, STRMSG_C_SELECT_0004);//"°ÔÀÓ¸¶½ºÅÍ"
	if(COMPARE_RACE(m_createUnit.Race, RACE_OPERATION))
		strcpy( buf, STRMSG_C_SELECT_0005);//"°ü¸®ÀÚ"
	if(COMPARE_RACE(m_createUnit.Race, RACE_GUEST))
		strcpy( buf, STRMSG_C_SELECT_0006);//"°Ô½ºÆ®"
	if(COMPARE_RACE(m_createUnit.Race, RACE_MONITOR))
		strcpy( buf, STRMSG_C_SELECT_0007);//"¸ð´ÏÅÍ"

	m_pFontCharacterInfo[CREATE_CHARACTER_INFO_RACE]->DrawText(m_nCharacterInfoPos[CREATE_CHARACTER_INFO_RACE][0]-CREATE_CHARACTER_INFO_RACE_X, m_nCharacterInfoPos[CREATE_CHARACTER_INFO_RACE][1], CREATE_FONT_COLOR, buf, 0L);
	// 2012-03-29 by mspark, ±â¾î »ý¼ºÈ­¸é¿¡¼­ ½ºÅÈ ¼öÄ¡ÀÇ À§Ä¡ ¼öÁ¤ - xÁÂÇ¥ +6 Ãß°¡
	wsprintf( buf, "%d", m_createUnit.GearStat1.AttackPart);
	m_pFontCharacterInfo[CREATE_CHARACTER_INFO_RACE]->DrawText(m_nCharacterInfoPos[CREATE_CHARACTER_INFO_ATTACK][0] + 6, m_nCharacterInfoPos[CREATE_CHARACTER_INFO_ATTACK][1], CREATE_FONT_COLOR, buf, 0L);
	wsprintf( buf, "%d", m_createUnit.GearStat1.DefensePart);
	m_pFontCharacterInfo[CREATE_CHARACTER_INFO_ENDURANCE]->DrawText(m_nCharacterInfoPos[CREATE_CHARACTER_INFO_ENDURANCE][0] + 6, m_nCharacterInfoPos[CREATE_CHARACTER_INFO_ENDURANCE][1], CREATE_FONT_COLOR, buf, 0L);
	wsprintf( buf, "%d", m_createUnit.GearStat1.FuelPart);
	m_pFontCharacterInfo[CREATE_CHARACTER_INFO_FUEL]->DrawText(m_nCharacterInfoPos[CREATE_CHARACTER_INFO_FUEL][0] + 6, m_nCharacterInfoPos[CREATE_CHARACTER_INFO_FUEL][1], CREATE_FONT_COLOR, buf, 0L);
	// end 2012-03-29 by mspark, ±â¾î »ý¼ºÈ­¸é¿¡¼­ ½ºÅÈ ¼öÄ¡ÀÇ À§Ä¡ ¼öÁ¤ - xÁÂÇ¥ +6 Ãß°¡
	wsprintf( buf, "%d", m_createUnit.GearStat1.SoulPart);
	m_pFontCharacterInfo[CREATE_CHARACTER_INFO_SOUL]->DrawText(m_nCharacterInfoPos[CREATE_CHARACTER_INFO_SOUL][0], m_nCharacterInfoPos[CREATE_CHARACTER_INFO_SOUL][1], CREATE_FONT_COLOR, buf, 0L);
	wsprintf( buf, "%d", m_createUnit.GearStat1.DodgePart);
	m_pFontCharacterInfo[CREATE_CHARACTER_INFO_DODGE]->DrawText(m_nCharacterInfoPos[CREATE_CHARACTER_INFO_DODGE][0], m_nCharacterInfoPos[CREATE_CHARACTER_INFO_DODGE][1], CREATE_FONT_COLOR, buf, 0L);
	wsprintf( buf, "%d", m_createUnit.GearStat1.ShieldPart);
	m_pFontCharacterInfo[CREATE_CHARACTER_INFO_DEFENSE]->DrawText(m_nCharacterInfoPos[CREATE_CHARACTER_INFO_DEFENSE][0], m_nCharacterInfoPos[CREATE_CHARACTER_INFO_DEFENSE][1], CREATE_FONT_COLOR, buf, 0L);

	// 2005-07-04 by ispark 
	// ÀÚµ¿ ½ºÅÈ ºÐ¹è »èÁ¦
	// Render Radio Button
//	for(i=0; i<CREATE_CHARACTER_RADIO_ALL_AUTTON; i++)
//	{
//		m_pImgRadio[m_nRadioButton[i]]->Move(POS_CRE_BACK_X+390, POS_CRE_BACK_Y+70+(i*17));
//		m_pImgRadio[m_nRadioButton[i]]->Render();
//	}

	// ÀÌ¸§ÀÌ ¾ø°Å³ª ÀÌ¸§ÀÌ °°°Å³ª ÀÌ¸§¿¡ Çü½ÄÀÌ Æ²¸±¶§
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI½Ã½ºÅÛ º¯°æ
	if(m_nFocus != CREATE_CHARACTER_FOCUS_NONE)
	{
		if(m_nFocus == CREATE_CHARACTER_FOCUS_NAME)
		{
			m_pImgFocus[CREATE_CHARACTER_FOCUS_NAME]->Move(POS_CRE_BACK_X + 306, POS_CRE_BACK_Y-26);
			m_pImgFocus[CREATE_CHARACTER_FOCUS_NAME]->Render();
		}
		else if(m_nFocus == CREATE_CHARACTER_FOCUS_NAME_UNABLE)
		{
			m_pImgFocus[CREATE_CHARACTER_FOCUS_NAME_UNABLE]->Move(POS_CRE_BACK_X + 306, POS_CRE_BACK_Y-26);
			m_pImgFocus[CREATE_CHARACTER_FOCUS_NAME_UNABLE]->Render();
		}
		else if(m_nFocus == CREATE_CHARACTER_FOCUS_SAME_NAME)
		{
			m_pImgFocus[CREATE_CHARACTER_FOCUS_SAME_NAME]->Move(POS_CRE_BACK_X + 306, POS_CRE_BACK_Y-26);
			m_pImgFocus[CREATE_CHARACTER_FOCUS_SAME_NAME]->Render();
		}
	}
	
	if(m_nFocusStat == CREATE_CHARACTER_FOCUS_STEC)
	{
		m_pImgFocus[CREATE_CHARACTER_FOCUS_STEC]->Move(POS_CRE_BACK_X + 450,POS_CRE_BACK_Y-31);
		m_pImgFocus[CREATE_CHARACTER_FOCUS_STEC]->Render();
	}	
#else 
	if(m_nFocus != CREATE_CHARACTER_FOCUS_NONE)
	{
		if(m_nFocus == CREATE_CHARACTER_FOCUS_NAME)
		{
			m_pImgFocus[CREATE_CHARACTER_FOCUS_NAME]->Move(POS_CRE_BACK_X+CREATE_CHARACTER_FOCUS_NAME_X, POS_CRE_BACK_Y-10);
			m_pImgFocus[CREATE_CHARACTER_FOCUS_NAME]->Render();
		}
		else if(m_nFocus == CREATE_CHARACTER_FOCUS_NAME_UNABLE)
		{
			m_pImgFocus[CREATE_CHARACTER_FOCUS_NAME_UNABLE]->Move(POS_CRE_BACK_X+CREATE_CHARACTER_FOCUS_NAME_X, POS_CRE_BACK_Y-10);
			m_pImgFocus[CREATE_CHARACTER_FOCUS_NAME_UNABLE]->Render();
		}
		else if(m_nFocus == CREATE_CHARACTER_FOCUS_SAME_NAME)
		{
			m_pImgFocus[CREATE_CHARACTER_FOCUS_SAME_NAME]->Move(POS_CRE_BACK_X+CREATE_CHARACTER_FOCUS_NAME_X, POS_CRE_BACK_Y-10);
			m_pImgFocus[CREATE_CHARACTER_FOCUS_SAME_NAME]->Render();
		}
	}
	
	if(m_nFocusStat == CREATE_CHARACTER_FOCUS_STEC)
	{
		m_pImgFocus[CREATE_CHARACTER_FOCUS_STEC]->Move(POS_CRE_BACK_X+376,POS_CRE_BACK_Y+4);
		m_pImgFocus[CREATE_CHARACTER_FOCUS_STEC]->Render();
	}	
#endif
	// 2005-07-04 by ispark 
	// ÀÚµ¿ ½ºÅÈ ºÐ¹è »èÁ¦
	// ÅøÆÁ°ü·Ã ·»´õ¸µ
//	if(m_nRenderToolTipIndex != -1)
//	{
//		m_pImgToolTip[m_nRenderToolTipIndex]->Move(CREATE_CHARACTER_INFO_STAT_X,CREATE_CHARACTER_INFO_STAT_Y);
//		m_pImgToolTip[m_nRenderToolTipIndex]->Render();
//	}
}

int CINFCreateMenu::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	FLOG( "CINFCreateMenu::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)" );
//	if(m_bBlocking)
//	{
//		return INF_MSGPROC_NORMAL;
//	}
	switch(uMsg)
	{
	case WM_LBUTTONDOWN:
		{
			POINT pt;
 			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);

			// ¾ÆÀÌµð³ª ½ºÅÝÀ» ´©¸£¸é ÀÌ¹ÌÁö ·»´õ¸µ ÇØÁ¦
			if( pt.x > m_nCharacterNamePos[0] && pt.x < m_nCharacterNamePos[0] + 130 &&
				pt.y > m_nCharacterNamePos[1] && pt.y < m_nCharacterNamePos[1] + 15)
			{
				m_nFocus = CREATE_CHARACTER_FOCUS_NONE;
				break;
			}
			
			for(int i=0; i<CRE_BUTTON_NUMBER; i++)
			{
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI½Ã½ºÅÛ º¯°æ
				if( m_bRenderInfoBtn == FALSE &&  i == CRE_BUTTON_UP || m_bRenderInfoBtn == TRUE &&  i == CRE_BUTTON_DOWN )
				{
					continue;
				}
				if( pt.x > m_fButtonPos[i][0] && pt.x < m_fButtonPos[i][0] + CRE_SIZE_BUTTON_X(i) &&
					pt.y > m_fButtonPos[i][1] && pt.y < m_fButtonPos[i][1] + CRE_SIZE_BUTTON_Y(i) )
				{
					m_nButtonState[i] = CRE_BUTTON_STATE_DOWN;
					g_pD3dApp->m_pSound->PlayD3DSound(SOUND_SELECT_BUTTON, D3DXVECTOR3(0,0,0), FALSE);
				}
				else
				{
					m_nButtonState[i] = CRE_BUTTON_STATE_NORMAL;
				}
#else
				if( pt.x > m_fButtonPos[i][0]+37 && pt.x < m_fButtonPos[i][0]+37 + CRE_SIZE_BUTTON_X(i) &&
					pt.y > m_fButtonPos[i][1] && pt.y < m_fButtonPos[i][1] + CRE_SIZE_BUTTON_Y(i) )
				{
					m_nButtonState[i] = CRE_BUTTON_STATE_DOWN;
					g_pD3dApp->m_pSound->PlayD3DSound(SOUND_SELECT_BUTTON, D3DXVECTOR3(0,0,0), FALSE);
				}
				else
				{
					m_nButtonState[i] = CRE_BUTTON_STATE_NORMAL;
				}
#endif
			}

			// 2005-07-04 by ispark 
			// ÀÚµ¿ ½ºÅÈ ºÐ¹è »èÁ¦
			// Radio Button
//			for(i=0; i<CREATE_CHARACTER_RADIO_ALL_AUTTON; i++)
//			{				
//				if( pt.x > POS_CRE_BACK_X+396 && pt.x < POS_CRE_BACK_X+448 &&
//					pt.y > POS_CRE_BACK_Y+75+(i*17)  && pt.y < POS_CRE_BACK_Y+87+(i*17))
//				{
//					for(int j=0; j<CREATE_CHARACTER_RADIO_ALL_AUTTON; j++)
//						m_nRadioButton[j] = CREATE_CHARACTER_INFO_RADIO_DEF;
//					m_nRadioButton[i] = CREATE_CHARACTER_INFO_RADIO_SEL;
//					m_nSelectUnitStyle = i;
//					m_nFocusStat = CREATE_CHARACTER_FOCUS_NONE;
//					SetCharacterUnitStyle();
//				}				
//			}			
		}
		break;
	case WM_LBUTTONUP:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);
			int i;
			for(i=0; i<CRE_BUTTON_NUMBER; i++)
			{
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI½Ã½ºÅÛ º¯°æ
				if( m_bRenderInfoBtn == FALSE &&  i == CRE_BUTTON_UP || m_bRenderInfoBtn == TRUE &&  i == CRE_BUTTON_DOWN )
				{
					continue;
				}																				  
				if( pt.x > m_fButtonPos[i][0] && pt.x < m_fButtonPos[i][0] + CRE_SIZE_BUTTON_X(i) &&
					pt.y > m_fButtonPos[i][1] && pt.y < m_fButtonPos[i][1] + CRE_SIZE_BUTTON_Y(i) )
				{
					if(m_nButtonState[i] == CRE_BUTTON_STATE_DOWN)
					{
						m_nButtonState[i] = CRE_BUTTON_STATE_UP;
						OnButtonClicked(i);
					}
					else
					{
						m_nButtonState[i] = CRE_BUTTON_STATE_NORMAL;
					}
				}
				else
				{
					m_nButtonState[i] = CRE_BUTTON_STATE_NORMAL;
				}  
#else
				if( pt.x > m_fButtonPos[i][0]+37 && pt.x < m_fButtonPos[i][0]+37 + CRE_SIZE_BUTTON_X(i) &&
					pt.y > m_fButtonPos[i][1] && pt.y < m_fButtonPos[i][1] + CRE_SIZE_BUTTON_Y(i) )
				{
					if(m_nButtonState[i] == CRE_BUTTON_STATE_DOWN)
					{
						m_nButtonState[i] = CRE_BUTTON_STATE_UP;
						OnButtonClicked(i);
					}
					else
					{
						m_nButtonState[i] = CRE_BUTTON_STATE_NORMAL;
					}
				}
				else
				{
					m_nButtonState[i] = CRE_BUTTON_STATE_NORMAL;
				}
#endif
			}
			for(i=0; i<CRE_BUTTON_NUMBER; i++)
				m_nButtonState[i] = CRE_BUTTON_STATE_NORMAL;
		}
		break;
	case WM_MOUSEMOVE:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);
			BOOL bChangeMouseState = FALSE;
//			m_nRenderToolTipIndex = -1;						// 2005-07-04 by ispark ÀÚµ¿ ½ºÅÈ ºÐ¹è »èÁ¦
			for(int i=0; i<CRE_BUTTON_NUMBER; i++)
			{
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI½Ã½ºÅÛ º¯°æ
				if( m_bRenderInfoBtn == FALSE &&  i == CRE_BUTTON_UP || m_bRenderInfoBtn == TRUE &&  i == CRE_BUTTON_DOWN )
				{
					continue;
				}
				if( pt.x > m_fButtonPos[i][0] && pt.x < m_fButtonPos[i][0] + CRE_SIZE_BUTTON_X(i) &&
					pt.y > m_fButtonPos[i][1] && pt.y < m_fButtonPos[i][1] + CRE_SIZE_BUTTON_Y(i) )
				{				
#else
				if( pt.x > m_fButtonPos[i][0]+37 && pt.x < m_fButtonPos[i][0]+37 + CRE_SIZE_BUTTON_X(i) &&
					pt.y > m_fButtonPos[i][1] && pt.y < m_fButtonPos[i][1] + CRE_SIZE_BUTTON_Y(i) )
				{
#endif
					if(m_nButtonState[i] != CRE_BUTTON_STATE_DOWN)
					{
						m_nButtonState[i] = CRE_BUTTON_STATE_UP;
					}
					((CINFSelect*)m_pParent)->SetMouseState( SELECT_MOUSE_STATE_UP );
					bChangeMouseState = TRUE;
				}
				else
				{
					m_nButtonState[i] = CRE_BUTTON_STATE_NORMAL;
					if(!bChangeMouseState) {
						((CINFSelect*)m_pParent)->SetMouseState( SELECT_MOUSE_STATE_NORMAL ); }
				}
			}
			// 2005-07-04 by ispark 
			// ÀÚµ¿ ½ºÅÈ ºÐ¹è »èÁ¦
			// ½ºÅÝºÐ¹è ÅøÆÁ
//			for(i=0; i<CREATE_CHARACTER_RADIO_ALL_AUTTON; i++)
//			{				
//				if( pt.x > POS_CRE_BACK_X+396 && pt.x < POS_CRE_BACK_X+448 &&
//					pt.y > POS_CRE_BACK_Y+75+(i*17)  && pt.y < POS_CRE_BACK_Y+87+(i*17))
//				{
//					m_nSelectUnitStyle = i;
//					SetCharacterUnitStyle(TRUE);
//				}				
//			}
		}
		break;
	}
	return INF_MSGPROC_NORMAL;
}

void CINFCreateMenu::OnButtonClicked(int i)
{
	FLOG( "CINFCreateMenu::OnButtonClicked(int i)" );
	switch(i)
	{
	case CRE_BUTTON_OK:
		{
			// 2007-07-25 by bhsohn À¯Àú°¡ Æ©Åä¸®¾ó¸Ê¿¡ ÀÖ¾î¼­ ¸¶À»·Î °¬À»½Ã, Ã¹¹ø? À¯Àú·Î °£ÁÖ
			g_pD3dApp->EndNarrationSound();

			BOOL bFlag = FALSE;
			if(strlen(m_strCharacterName)==0)
			{
//				2005-05-13 by ydkim
//				char buf[256];
//				LoadString(NULL, IDS_MSGBOX_INPUT_NAME, buf, sizeof(buf));
//				CINFWindow* pWindow = ((CINFSelect*)m_pParent)->m_pInfWindow;
//				if(!pWindow->IsExistMsgBox(_Q_SELECT_DELETE))
//				{
//					pWindow->AddMsgBox(buf, _MESSAGE_ERROR_CREATE);
//				}
				m_nFocus = CREATE_CHARACTER_FOCUS_NAME;
				bFlag = TRUE;
			}
			else if(!ErrCheckCharacterName(m_strCharacterName))
			{
				m_nFocus = CREATE_CHARACTER_FOCUS_NAME_UNABLE;

				// 2009-02-10 by bhsohn Japan Charcter Create
				//memset(m_strCharacterName,0x00,SIZE_MAX_CHARACTER_NAME);
				ZERO_MEMORY(m_strCharacterName);
				// end 2009-02-10 by bhsohn Japan Charcter Create
				g_pD3dApp->CleanText();
				bFlag = TRUE;
			}
			
			// 2005-07-04 by ispark 
			// ÀÚµ¿ ½ºÅÈ ºÐ¹è »èÁ¦
//			if(m_nSelectUnitStyle == CREATE_CHARACTER_INFO_UNIT_STYLE_U || 
//					m_nCreateUnitStat == CREATE_CHARACTER_INFO_UNIT_STYLE_U)
//			{
////				2005-05-13 by ydkim
////				CINFWindow* pWindow = ((CINFSelect*)m_pParent)->m_pInfWindow;
////				if(!pWindow->IsExistMsgBox(_Q_SELECT_DELETE))
////				{
////					pWindow->AddMsgBox(STRMSG_C_SELECT_0008, _MESSAGE_ERROR_CREATE);//"½ºÅÈ ºÐ¹è ¿É¼ÇÀ» ¼±ÅÃÇÏ¿© ÁÖ½Ê½Ã¿À."
////				}
//				m_nFocusStat = CREATE_CHARACTER_FOCUS_STEC;
//				bFlag = TRUE;
//			}
			
			if(FALSE == bFlag) 
			{
				MSG_FC_CHARACTER_CREATE sMsg;
				memset(&sMsg,0x00,sizeof(sMsg));
				// 2009-02-10 by bhsohn Japan Charcter Create
				//strcpy(m_createUnit.CharacterName, m_strCharacterName);
				util::strncpy(m_createUnit.CharacterName, m_strCharacterName, SIZE_MAXUSE_CHARACTER_NAME);
				// end 2009-02-10 by bhsohn Japan Charcter Create
				sMsg = m_createUnit;
				if(ErrCheckCharacterName(sMsg.CharacterName))
				{
					sMsg.AutoStatType1 = m_nCreateUnitStat;
					sMsg.AccountUniqueNumber = g_pD3dApp->m_accountUniqueNumber;
					char buffer[SIZE_MAX_PACKET];
					int nType = T_FC_CHARACTER_CREATE;
					memcpy(buffer, &nType, SIZE_FIELD_TYPE_HEADER);
					memcpy(buffer+SIZE_FIELD_TYPE_HEADER, &sMsg, sizeof(sMsg));
					g_pD3dApp->m_pFieldWinSocket->Write(buffer, SIZE_FIELD_TYPE_HEADER + sizeof(sMsg));
					m_bBlocking = TRUE;

					g_pD3dApp->m_bRequestEnable = FALSE;
				}
				else
				{// Ä³¸¯ÅÍ ÀÌ¸§ ¿¡·¯
					CINFWindow* pWindow = ((CINFSelect*)m_pParent)->m_pInfWindow;
					if(!pWindow->IsExistMsgBox(_Q_SELECT_DELETE))
					{
						pWindow->AddMsgBox(STRMSG_C_SELECT_0009, _MESSAGE_ERROR_CREATE);//"¿Ã¹Ù¸£Áö ¾ÊÀº ÀÌ¸§ÀÔ´Ï´Ù"
					}
				}
				// 2009-02-10 by bhsohn Japan Charcter Create
				//memset(m_strCharacterName,0x00,SIZE_MAX_CHARACTER_NAME);
				ZERO_MEMORY(m_strCharacterName);
				// end 2009-02-10 by bhsohn Japan Charcter Create
				g_pD3dApp->CleanText();
//				g_input.SetChangeEnterState();
				//g_pD3dApp->m_inputkey.ToggleHanEng(g_pD3dApp->GetHwnd(),IME_CMODE_NATIVE);
			}
		}
		break;
	case CRE_BUTTON_CANCEL:
		{
			// 2007-07-25 by bhsohn À¯Àú°¡ Æ©Åä¸®¾ó¸Ê¿¡ ÀÖ¾î¼­ ¸¶À»·Î °¬À»½Ã, Ã¹¹ø? À¯Àú·Î °£ÁÖ
			g_pD3dApp->EndNarrationSound();
			
			// ÃÊ±âÈ­
			// 2005-07-04 by ispark 
			// ÀÚµ¿ ½ºÅÈ ºÐ¹è »èÁ¦
			g_pSelect->m_pCreateMenu->m_nCreateUnitStat = AUTOSTAT_TYPE_FREESTYLE;
//			g_pSelect->m_pCreateMenu->m_nRenderToolTipIndex = -1;
			g_pSelect->m_pCreateMenu->m_nFocusStat = -1;
			g_pSelect->m_pCreateMenu->m_nFocus = -1;
			
			m_nButtonState[CRE_BUTTON_CANCEL] = CRE_BUTTON_STATE_NORMAL;
			((CINFSelect*)m_pParent)->m_pSelectBack->ChangeMode(SELECT_MODE);
			((CINFSelect*)m_pParent)->m_pSelectBack->m_bInitEffectItem = false;
			g_pD3dApp->DeleteEffectList();
			g_pD3dApp->ChangeGameState(_SELECT);
			m_nCreateUnitStat = AUTOSTAT_TYPE_FREESTYLE;
		}
		break;

	// 2005-07-04 by ispark
	// CRE_BUTTON_LEFT¿Í CRE_BUTTON_RIGHT ÀÌ ºÎºÐÀº ¾²°í ÀÖÁö ¾ÊÀ½
	case CRE_BUTTON_LEFT:
		{
//			m_nCreateUnitStat = -1;		// 2005-07-04 by ispark ÀÚµ¿ ½ºÅÈ ºÐ¹è »èÁ¦
			// 2009-02-10 by bhsohn Japan Charcter Create
			//memset(m_strCharacterName,0x00,SIZE_MAX_CHARACTER_NAME);
			ZERO_MEMORY(m_strCharacterName);
			// end 2009-02-10 by bhsohn Japan Charcter Create
			g_pD3dApp->CleanText();

			// 2005-09-12 by ispark		Face ¼±ÅÃ½Ã ÇöÀç °³¹æµÈ Face¸¸ ¼±ÅÃ
			for(int j = 0; j < CRE_TOTAL_NUMBER; j++)
			{
				m_nChoiseFaceIndex--;
				if(0 > m_nChoiseFaceIndex)
					m_nChoiseFaceIndex = CRE_TOTAL_NUMBER - 1;

				if(m_nFaceUseIndex[m_nChoiseFaceIndex] != -1)
				{
					m_createUnit.PilotFace = m_nFaceUseIndex[m_nChoiseFaceIndex];
					break;
				}
			}			
		}
		break;
	case CRE_BUTTON_RIGHT:
		{
//			m_nCreateUnitStat = -1;		// 2005-07-04 by ispark ÀÚµ¿ ½ºÅÈ ºÐ¹è »èÁ¦
			// 2009-02-10 by bhsohn Japan Charcter Create
			//memset(m_strCharacterName,0x00,SIZE_MAX_CHARACTER_NAME);
			ZERO_MEMORY(m_strCharacterName);
			// end 2009-02-10 by bhsohn Japan Charcter Create
			g_pD3dApp->CleanText();

			// 2005-09-12 by ispark		Face ¼±ÅÃ½Ã ÇöÀç °³¹æµÈ Face¸¸ ¼±ÅÃ
			for(int j = 0; j < CRE_TOTAL_NUMBER; j++)
			{
				m_nChoiseFaceIndex++;
//				if(CRE_TOTAL_NUMBER <= m_nChoiseFaceIndex)
//					m_nChoiseFaceIndex = 0;
				m_nChoiseFaceIndex = m_nChoiseFaceIndex % CRE_TOTAL_NUMBER;

				if(m_nFaceUseIndex[m_nChoiseFaceIndex] != -1)
				{
					m_createUnit.PilotFace = m_nFaceUseIndex[m_nChoiseFaceIndex];
					break;
				}
			}			
		}
		break;
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI½Ã½ºÅÛ º¯°æ
	case CRE_BUTTON_DOWN:
		{
			if( m_bRenderInfoBtn == FALSE )
			{
				m_bRenderInfoBtn = TRUE;
				m_nBtnUpFadeMode = FADE_IN;
				m_dwBtnUpBkAlpha = 0x00ffffff;
				((CINFSelect*)m_pParent)->SetBKInfoFadeIn();
			}
			
		}
		break;
	case CRE_BUTTON_UP:
		{
			if(m_bRenderInfoBtn == TRUE)
			{
				m_bRenderInfoBtn = FALSE;
				m_nBtnDownFadeMode = FADE_IN;
				m_dwBtnUDownpBkAlpha = 0x00ffffff;
				((CINFSelect*)m_pParent)->SetBKInfoFadeOut();
			}			
		}
		break;
#endif
	}
}

void CINFCreateMenu::CharacterSet(USHORT uUnitKind)
{
	FLOG( "CINFCreateMenu::CharacterSet(USHORT uUnitKind)" );
	m_createUnit.AccountUniqueNumber = g_pD3dApp->m_accountUniqueNumber;
	strcpy( m_createUnit.CharacterName, m_strCharacterName);
	m_createUnit.UnitKind = uUnitKind;
	m_createUnit.Race = RACE_DECA;
	if(m_createUnit.PilotFace >= 100)
		m_createUnit.Color = 1;
	else
		m_createUnit.Color = 0;
	const GEAR_STAT_CHANGE_VALUE* pStat = CAtumSJ::GetGearStatChangeValueStruct(uUnitKind);
	m_createUnit.GearStat1.AttackPart = pStat->GearStat1.AttackPart;
	m_createUnit.GearStat1.FuelPart = pStat->GearStat1.FuelPart;
	m_createUnit.GearStat1.DefensePart = pStat->GearStat1.DefensePart;
	m_createUnit.GearStat1.SoulPart = pStat->GearStat1.SoulPart;
	m_createUnit.GearStat1.ShieldPart = pStat->GearStat1.ShieldPart;
	m_createUnit.GearStat1.DodgePart = pStat->GearStat1.DodgePart;

	// ½ºÅÝ ºÐ¹è ¹æ½Ä 
	// 2005-07-04 by ispark 
	// ÀÚµ¿ ½ºÅÈ ºÐ¹è »èÁ¦
//	m_nSelectUnitStyle	= CREATE_CHARACTER_INFO_UNIT_STYLE_U;
	m_nSelectUnitStyle	= CREATE_CHARACTER_INFO_UNIT_STYLE_F;
//	for(int j=0; j<CREATE_CHARACTER_RADIO_ALL_AUTTON; j++)
//		m_nRadioButton[j] = CREATE_CHARACTER_INFO_RADIO_DEF;
	switch(m_createUnit.UnitKind)
	{
	case UNITKIND_BGEAR:	m_nSelectUnit = CREATE_CHARACTER_INFO_B;	break;
	case UNITKIND_MGEAR:	m_nSelectUnit = CREATE_CHARACTER_INFO_M;	break;
	case UNITKIND_AGEAR:	m_nSelectUnit = CREATE_CHARACTER_INFO_A;	break;
	case UNITKIND_IGEAR:	m_nSelectUnit = CREATE_CHARACTER_INFO_I;	break;
	}
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI½Ã½ºÅÛ º¯°æ
	if( m_bRenderInfoBtn == FALSE )
	{
// 		m_dwBtnUDownpBkAlpha = 0x00ffffff;
// 		m_fBtnDownFadeInTime = 0;
// 		m_nBtnDownFadeMode = FADE_IN;
// 		((CINFSelect*)m_pParent)->SetBKInfoFadeOut();

		m_bRenderInfoBtn = TRUE;
		m_nBtnUpFadeMode = FADE_IN;
		m_dwBtnUpBkAlpha = 0x00ffffff;
		((CINFSelect*)m_pParent)->SetBKInfoFadeIn();											  

	}
	#endif
}

BOOL CINFCreateMenu::ErrCheckCharacterName(char * strName)
{
	FLOG("CAtumApplication::ErrCheckCharacterName(char * strName)");

	int strLen = strlen(strName);

	int nLen = MultiByteToWideChar(CP_ACP, 0, strName,
		strlen(strName), NULL, NULL);
	BSTR bmsg = ::SysAllocStringLen(NULL, nLen);
	if (bmsg == NULL)
		return FALSE;
	MultiByteToWideChar(CP_ACP, 0, strName, strlen(strName),
		bmsg, nLen);

    BYTE *rtval = new BYTE[strLen*4];
    int i;

    for(i=0;i<strLen;i++)
    {
/*		if((strName[i] >= 'A' && strName[i] <= 'Z') ||
			(strName[i] >= 'a' && strName[i] <= 'z') ||
			(strName[i] == '_') || (strName[i] >= '0' && strName[i] <= '9'))
		{
			continue;
		}
*/
		if( bmsg[i] > 0x0040 && bmsg[i] < 0x005b)
		{//¿µ¾î(´ë¹®ÀÚ)
			continue;
		}
		if( bmsg[i] > 0x0060 && bmsg[i] < 0x007b)
		{//¿µ¾î(¼Ò¹®ÀÚ)
			continue;
		}
		if( bmsg[i] >= 0x0030 && bmsg[i] <= 0x0039)
		{//¼ýÀÚ
			continue;
		}
		if( bmsg[i] == 0x005f)//'_'
		{//Æ¯¼ö¹®ÀÚ
			continue;
		}

//		if( bmsg[i] > 0x0040 && bmsg[i] < 0x007b)
//		{//¿µ¾î
//			continue;
//		}
//		if( bmsg[i] > 0x0029 && bmsg[i] < 0x0031+9)
//		{//¼ýÀÚ
//			continue;
//		}

		// 2007-10-22 by bhsohn Ä³¸¯ÅÍ¸í °ø¹é °ü·Ã Ã³¸®
//#ifdef LANGUAGE_KOREA
//		//if(PRIMARYLANGID(g_input.GetLangId()) == LANG_KOREAN)
//		{
//			if(bmsg[i] == 0 )
//			{				
//				int temp,temp2;
//				char bufChat[SIZE_MAX_CHARACTER_NAME] = {0,};
//				strncpy(bufChat,strName,strlen(strName));
//				// 2006-04-13 by ispark
//				g_pD3dApp->CheckReadyStringFiltering(bufChat, &temp, &temp2);
//	//			g_pD3dApp->m_pAbuseFilter->AbuseFiltering(bufChat,&temp,&temp2);
//
//				// 2007-10-22 by bhsohn Ä³¸¯ÅÍ¸í °ø¹é °ü·Ã Ã³¸®
//				delete[] rtval;
//				// end 2007-10-22 by bhsohn Ä³¸¯ÅÍ¸í °ø¹é °ü·Ã Ã³¸®				
//				if(temp > 0)
//					return FALSE;
//				return TRUE;
//			}
//			if (bmsg[i] < 0xAC00 || bmsg[i] > 0xd7a3)
//			{
//				rtval[i*4] = rtval[i*4+1]= rtval[i*4+2]= rtval[i*4+3] = 0;
//				// 2007-10-22 by bhsohn Ä³¸¯ÅÍ¸í °ø¹é °ü·Ã Ã³¸®
//				delete[] rtval;
//				// end 2007-10-22 by bhsohn Ä³¸¯ÅÍ¸í °ø¹é °ü·Ã Ã³¸®
//				return FALSE;
//			}
//			else
//			{
//				int chohab = bmsg[i] - 0xAc00;
//				int cho = chohab /21 / 28;
//				int chung = (chohab % (21*28)) / 28;
//				int chong = chohab % 28;
//				
//				switch (cho)
//				{
//				case 0:			// °¡
//				case 1:			// ±î
//				case 5:			// ¶ó
//				case 15:		// Ä«
//				case 11:		// ¾Æ
//				case 2:			// ³ª
//				case 12:		// ÀÚ
//				case 16:		// Å¸
//				case 9:			// »ç
//				case 18:		// ÇÏ
//				case 3:			// ´Ù
//				case 4:			// µû
//				case 10:		// ½Î
//				case 13:		// Â¥
//				case 14:		// Â÷
//				case 6:			// ¸¶
//				case 7:			// ¹Ù
//				case 8:			// ºü
//				case 17:		// ÆÄ
//					break;
//				default:
//					{					
//						// 2007-10-22 by bhsohn Ä³¸¯ÅÍ¸í °ø¹é °ü·Ã Ã³¸®
//						delete[] rtval;
//						// end 2007-10-22 by bhsohn Ä³¸¯ÅÍ¸í °ø¹é °ü·Ã Ã³¸®
//						return FALSE;
//					}
//				}
//				
//				
//				switch (chung)
//				{
//				case 0:			// ¾Æ
//				case 1:			// ¾Ö
//				case 2:			// ¾ß
//				case 3:			// ¾ê
//				case 4:			// ¾î
//				case 5:			// ¿¡
//				case 6:			// ¿©
//				case 7:			// ¿¹
//				case 8:			// ¿À
//				case 17:		// À¯
//				case 9:			// ¿Í
//				case 12:		// ¿ä
//				case 10:		// ¿Ö
//				case 11:		// ¿Ü
//				case 13:		// ¿ì
//				case 14:		// ¿ö
//				case 15:		// ¿þ
//				case 16:		// À§
//				case 18:		// À¸
//				case 19:		// ÀÇ
//				case 20:		// ÀÌ
//					break;
//				default:
//					{
//						// 2007-10-22 by bhsohn Ä³¸¯ÅÍ¸í °ø¹é °ü·Ã Ã³¸®
//						delete[] rtval;
//						// end 2007-10-22 by bhsohn Ä³¸¯ÅÍ¸í °ø¹é °ü·Ã Ã³¸®
//						
//						return FALSE;
//					}
//				}
//				
//				
//				switch (chong)
//				{
//				case 0:			// ¾Æ
//				case 1:			// ¾Ç
//				case 2:			// ?
//				case 3:			// ?
//				case 8:			// ¾Ë
//				case 9:			// ¾Ì
//				case 10:		// ¾Í
//				case 11:		// ?
//				case 12:		// ?
//				case 13:		// ?
//				case 14:		// ?
//				case 15:		// ¾Î
//				case 24:		// ?
//				case 4:			// ¾È
//				case 5:			// ¾É
//				case 6:			// ¾Ê
//				case 22:		// ?
//				case 19:		// ¾Ñ
//				case 21:		// ¾Ó
//				case 7:			// ?
//				case 20:		// ¾Ò
//				case 23:		// ?
//				case 25:		// ¾Ô
//				case 27:		// ?
//				case 16:		// ¾Ï
//				case 17:		// ¾Ð
//				case 18:		// ?
//				case 26:		// ¾Õ
//					break;
//				default:
//					{
//						// 2007-10-22 by bhsohn Ä³¸¯ÅÍ¸í °ø¹é °ü·Ã Ã³¸®
//						delete[] rtval;
//						// end 2007-10-22 by bhsohn Ä³¸¯ÅÍ¸í °ø¹é °ü·Ã Ã³¸®						
//						return FALSE;
//					}
//				}
//				
//			}
//        }
		// end 2007-10-22 by bhsohn Ä³¸¯ÅÍ¸í °ø¹é °ü·Ã Ã³¸®
    }
//	if(PRIMARYLANGID(g_input.GetLangId()) == LANG_KOREAN)
	{
		int temp,temp2;
		// 2009-02-11 by bhsohn Japan MemBug
		//char bufChat[SIZE_MAX_CHARACTER_NAME] = {0,};
		char bufChat[MAX_PATH] = {0,};
		ZERO_MEMORY(bufChat);
		// end 2009-02-11 by bhsohn Japan MemBug
		strncpy(bufChat,strName,strlen(strName));
		// 2006-04-13 by ispark
		g_pD3dApp->CheckReadyStringFiltering(bufChat, &temp, &temp2);
//		g_pD3dApp->m_pAbuseFilter->AbuseFiltering(bufChat,&temp,&temp2);
		// 2007-10-22 by bhsohn Ä³¸¯ÅÍ¸í °ø¹é °ü·Ã Ã³¸®
		delete[] rtval;
		// end 2007-10-22 by bhsohn Ä³¸¯ÅÍ¸í °ø¹é °ü·Ã Ã³¸®
		if(temp > 0)
			return FALSE;
	}
	return TRUE;
}

// 2005-07-04 by ispark 
// ÀÚµ¿ ½ºÅÈ ºÐ¹è »èÁ¦
//void CINFCreateMenu::SetCharacterUnitStyle(BOOL bToolTip)
//{
//	switch(m_nSelectUnit) 
//	{
//	case CREATE_CHARACTER_INFO_B:
//		{
//			if(!bToolTip)
//			{
//				if(m_nSelectUnitStyle == CREATE_CHARACTER_INFO_UNIT_STYLE_A) m_nCreateUnitStat		= AUTOSTAT_TYPE_BGEAR_ATTACK;
//				else if(m_nSelectUnitStyle == CREATE_CHARACTER_INFO_UNIT_STYLE_D) m_nCreateUnitStat = AUTOSTAT_TYPE_BGEAR_MULTI;
//				else m_nCreateUnitStat = AUTOSTAT_TYPE_FREESTYLE;
//			}
//			else
//			{
//				if(m_nSelectUnitStyle == CREATE_CHARACTER_INFO_UNIT_STYLE_A) m_nRenderToolTipIndex		= AUTOSTAT_TYPE_BGEAR_ATTACK;
//				else if(m_nSelectUnitStyle == CREATE_CHARACTER_INFO_UNIT_STYLE_D) m_nRenderToolTipIndex = AUTOSTAT_TYPE_BGEAR_MULTI;
//				else m_nRenderToolTipIndex = AUTOSTAT_TYPE_FREESTYLE;
//			}
//		}
//		break;
//	case CREATE_CHARACTER_INFO_A:
//		{
//			if(!bToolTip)
//			{
//				if(m_nSelectUnitStyle == CREATE_CHARACTER_INFO_UNIT_STYLE_A) m_nCreateUnitStat		= AUTOSTAT_TYPE_AGEAR_ATTACK;
//				else if(m_nSelectUnitStyle == CREATE_CHARACTER_INFO_UNIT_STYLE_D) m_nCreateUnitStat = AUTOSTAT_TYPE_AGEAR_SHIELD;
//				else m_nCreateUnitStat = AUTOSTAT_TYPE_FREESTYLE;
//			}
//			else
//			{
//				if(m_nSelectUnitStyle == CREATE_CHARACTER_INFO_UNIT_STYLE_A) m_nRenderToolTipIndex		= AUTOSTAT_TYPE_AGEAR_ATTACK;
//				else if(m_nSelectUnitStyle == CREATE_CHARACTER_INFO_UNIT_STYLE_D) m_nRenderToolTipIndex = AUTOSTAT_TYPE_AGEAR_SHIELD;
//				else m_nRenderToolTipIndex = AUTOSTAT_TYPE_FREESTYLE;
//			}
//		}
//		break;
//	case CREATE_CHARACTER_INFO_M:
//		{
//			if(!bToolTip)
//			{
//				if(m_nSelectUnitStyle == CREATE_CHARACTER_INFO_UNIT_STYLE_A) m_nCreateUnitStat		= AUTOSTAT_TYPE_MGEAR_DEFENSE;
//				else if(m_nSelectUnitStyle == CREATE_CHARACTER_INFO_UNIT_STYLE_D) m_nCreateUnitStat = AUTOSTAT_TYPE_MGEAR_SUPPORT;
//				else m_nCreateUnitStat = AUTOSTAT_TYPE_FREESTYLE;
//			}
//			else
//			{
//				if(m_nSelectUnitStyle == CREATE_CHARACTER_INFO_UNIT_STYLE_A) m_nRenderToolTipIndex		= AUTOSTAT_TYPE_MGEAR_DEFENSE;
//				else if(m_nSelectUnitStyle == CREATE_CHARACTER_INFO_UNIT_STYLE_D) m_nRenderToolTipIndex = AUTOSTAT_TYPE_MGEAR_SUPPORT;
//				else m_nRenderToolTipIndex = AUTOSTAT_TYPE_FREESTYLE;
//			}
//		}
//		break;
//	case CREATE_CHARACTER_INFO_I:
//		{
//			if(!bToolTip)
//			{
//				if(m_nSelectUnitStyle == CREATE_CHARACTER_INFO_UNIT_STYLE_A) m_nCreateUnitStat		= AUTOSTAT_TYPE_IGEAR_ATTACK;
//				else if(m_nSelectUnitStyle == CREATE_CHARACTER_INFO_UNIT_STYLE_D) m_nCreateUnitStat = AUTOSTAT_TYPE_IGEAR_DODGE;
//				else m_nCreateUnitStat = AUTOSTAT_TYPE_FREESTYLE;
//			}
//			else
//			{
//				if(m_nSelectUnitStyle == CREATE_CHARACTER_INFO_UNIT_STYLE_A) m_nRenderToolTipIndex		= AUTOSTAT_TYPE_IGEAR_ATTACK;
//				else if(m_nSelectUnitStyle == CREATE_CHARACTER_INFO_UNIT_STYLE_D) m_nRenderToolTipIndex = AUTOSTAT_TYPE_IGEAR_DODGE;
//				else m_nRenderToolTipIndex = AUTOSTAT_TYPE_FREESTYLE;
//			}
//		}
//		break;
//	}
//}

void CINFCreateMenu::RenderPopUpWindowImage(int x, int y, int cx, int cy)
{
	//FLOG( "CINFCreateMenu::RenderPopUpWindowImage(int x, int y, int cx, int cy, int cstringLen)" );
	FLOG( "CINFCreateMenu::RenderPopUpWindowImage(int x, int y, int cx, int cy)" );

	m_pImgTextPopUp[0]->Move(x-7, y-2);
	m_pImgTextPopUp[0]->Render();

	m_pImgTextPopUp[1]->Move(x, y-2);
	m_pImgTextPopUp[1]->SetScale(cx, 1);
	m_pImgTextPopUp[1]->Render();

	m_pImgTextPopUp[2]->Move(x+cx, y-2);
	m_pImgTextPopUp[2]->Render();
}
