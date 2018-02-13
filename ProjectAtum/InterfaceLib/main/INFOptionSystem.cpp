// INFOptionSystem.cpp: implementation of the CINFOptionSystem class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "INFSystem.h"
#include "AtumApplication.h"
#include "INFGameMain.h"
#include "INFWindow.h"
#include "D3DHanFont.h"
#include "ShuttleChild.h"
#include "CharacterChild.h"				// 2005-07-21 by ispark
#include "AtumSound.h"
#include "Camera.h"
#include "Interface.h"
#include "FieldWinSocket.h"
#include "D3DHanFont.h"
#include "RangeTime.h"
#include "INFInven.h"
#include "INFImage.h"
#include "GameDataLast.h"
#include "Chat.h"
#include "dxutil.h"
#include "StoreData.h"
#include "FxSystem.h"
#include "INFImageBtn.h"
#include "INFImageRadioBtn.h"
#include "D3DHanFont.h"
#include "INFListBox.h"
#include "INFGameMainChat.h"
#include "INFGameMainWisperChat.h"
#include "INFTarget.h"	// 2009-03-04 by bhsohn 메뉴창 띠울시, 마우스 없어질수 있는 문제 수정

#include "INFOptionSystem.h"
#include "CustomOptimizer.h"

// 2011-07-06 by shcho, hsson 아레나 게임 진입 시 캐릭터 선택 못하게 수정
#include "../InterfaceLib/main/INFGameArena.h"
// end 2011-07-06 by shcho, hsson 아레나 게임 진입 시 캐릭터 선택 못하게 수정

#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
#include "INFGroupManager.h"
#include "INFGroupImage.h"
#include "INFImageEx.h"

#include "INFArenaScrollBar.h"
#include "MusicMP3Ex.h"		  
#include "INFMp3Player.h"
#endif
// 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현
#include "INFToolTip.h"
// end 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현

//ydkim. 04.07.02
#define END_GAME_TIMING_ATTACK		5000			// 게임도중 로그오프 게임종료 타이밍 


// 버튼관련 이벤트 
#define	OPTIONSYS_LOGOFF_BTN		0				// 로그오프
#define	OPTIONSYS_GAMEEND_BTN		1				// 게임종료
#define	OPTIONSYS_INITSETUP_BTN		2				// 초기화버튼

#define MIN_QUALITY_RENDER_TERRAIN		3
#define MIN_QUALITY_RENDER				0

// 옵션초기화 값
#define OPTION_DEFAULT_HIGH_TERRAINEFFECTRENDER			9
#define OPTION_DEFAULT_HIGH_TERRAINRENDER				9
#define OPTION_DEFAULT_HIGH_SHOWSHADOW					0
#define OPTION_DEFAULT_HIGH_OBJECTVIEW					9
#define OPTION_DEFAULT_HIGH_UNITDETAIL					9
#define OPTION_DEFAULT_HIGH_CONTRAST					0
#define OPTION_DEFAULT_HIGH_FILTER						0

#define OPTION_DEFAULT_MIDDLE_TERRAINEFFECTRENDER		7
#define OPTION_DEFAULT_MIDDLE_TERRAINRENDER				4
#define OPTION_DEFAULT_MIDDLE_SHOWSHADOW				0
#define OPTION_DEFAULT_MIDDLE_OBJECTVIEW				4
#define OPTION_DEFAULT_MIDDLE_UNITDETAIL				4
#define OPTION_DEFAULT_MIDDLE_CONTRAST					0
#define OPTION_DEFAULT_MIDDLE_FILTER					0

#define OPTION_DEFAULT_LOW_TERRAINEFFECTRENDER			0
#define OPTION_DEFAULT_LOW_TERRAINRENDER				0
#define OPTION_DEFAULT_LOW_SHOWSHADOW					0
#define OPTION_DEFAULT_LOW_OBJECTVIEW					0
#define OPTION_DEFAULT_LOW_UNITDETAIL					0
#define OPTION_DEFAULT_LOW_CONTRAST						0
#define OPTION_DEFAULT_LOW_FILTER						0

#define MODE_CAMERA_TYPE_FPS		1
#define MODE_CAMERA_TYPE_NORMAL		0

////////////// UI좌표
// 적용
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
#define OPTION_BTN_APP_X			76
#define OPTION_BTN_APP_Y			445
// 초기화
#define OPTION_BTN_INIT_X			155
#define OPTION_BTN_INIT_Y			445
// 로그오프
#define OPTION_BTN_LOGOFF_X			234
#define OPTION_BTN_LOGOFF_Y			445
// 게임종료
#define OPTION_BTN_GAMEEND_X			313
#define OPTION_BTN_GAMEEND_Y			445

// 라디오 버튼
#define OPTION_RADIO_GAME_INFO_X		306
#define OPTION_RADIO_GAME_INFO_Y		51
#define OPTION_RADIO_GAME_INFO_CAP_X	70
#define OPTION_RADIO_GAME_INFO_CAP_Y	22

#define OPTION_RADIO_VEDIO_INFO_X		304
#define OPTION_RADIO_VEDIO_INFO_Y		OPTION_RADIO_SMALLFRM_Y + 35

#define OPTION_VIDEO_FOV_POS_X			230
#define OPTION_VIDEO_FOV_SIZE_X			150
#define OPTION_VIDEO_FOV_SIZE_Y			1

#define OPTION_RADIO_SOUND_INFO_X		304
#define OPTION_RADIO_SOUND_INFO_Y		OPTION_HAESANG_Y + 57

//#define OPTION_RADIO_SOUND_MP3_X		63
//#define OPTION_RADIO_SOUND_MP3_Y		OPTION_HAESANG_Y + 94

#define OPTION_RADIO_SOUND_VOL_POS_X	230
#define OPTION_RADIO_SOUND_VOL_POS_Y	70

#define OPTION_RADIO_EFFECT_VOL_POS_X		230
#define OPTION_RADIO_EFFECT_VOL_POS_Y		OPTION_RADIO_SOUND_VOL_POS_Y + OPTION_COMBO_MAIN_HEIGHT

#define OPTION_RADIO_VOL_SIZE_X		150
#define OPTION_RADIO_VOL_SIZE_Y		1

#define OPTION_RADIO_SOUND_MP3_X		63
#define OPTION_RADIO_SOUND_MP3_Y		OPTION_HAESANG_Y + 99

#define OPTION_RADIO_ECT_INFO_X			304
#define OPTION_RADIO_ECT_SOUND_INFO_Y	OPTION_HAESANG_Y - 3

// 해상도
#define OPTION_HAESANG_X	252 
#define OPTION_HAESANG_X_W	433 
#define OPTION_HAESANG_Y	66

// 최소 프레임
#define OPTION_RADIO_SMALLFRM_X		304
#define OPTION_RADIO_SMALLFRM_Y		OPTION_COMBO_SEE_CHANGE_Y + OPTION_COMBO_MAIN_HEIGHT

// 닫기 버튼
#define OPTION_BTN_CLOSE_X			446
#define OPTION_BTN_CLOSE_Y			6

//////////////////////// 콤보박스 위치////////////////////////
#define OPTION_COMBO_MAIN_WIDTH			102
#define OPTION_COMBO_MAIN_HEIGHT		22
#define OPTION_COMBO_ELE_WIDTH			70
#define OPTION_COMBO_ELE_HEIGHT			15		// 2012-03-29 by mspark, 콤보 박스 클릭 시 보이는 리스트 배경 이미지 간격 수정 - 기존 13에서 15로 수정

// 시야거리
#define OPTION_COMBO_SEE_RANGE_X		317 
#define OPTION_COMBO_SEE_RANGE_Y		OPTION_HAESANG_Y + OPTION_COMBO_MAIN_HEIGHT
// 유닛디테일
#define OPTION_COMBO_UNIT_DETAIL_X		317 
#define OPTION_COMBO_UNIT_DETAIL_Y		OPTION_COMBO_SEE_RANGE_Y + OPTION_COMBO_MAIN_HEIGHT
// 그림자조절
#define OPTION_COMBO_SHADOW_X			317 
#define OPTION_COMBO_SHADOW_Y			OPTION_COMBO_UNIT_DETAIL_Y + OPTION_COMBO_MAIN_HEIGHT
// 이펙트 조절
#define OPTION_COMBO_EFFECT_X			317 
#define OPTION_COMBO_EFFECT_Y			OPTION_COMBO_SHADOW_Y + OPTION_COMBO_MAIN_HEIGHT
// 감마 조절
#define OPTION_COMBO_GAMMA_X			317 
#define OPTION_COMBO_GAMMA_Y			OPTION_COMBO_EFFECT_Y + OPTION_COMBO_MAIN_HEIGHT
// 대비 조절
#define OPTION_COMBO_CONTRAST_X			317 
#define OPTION_COMBO_CONTRAST_Y			OPTION_COMBO_GAMMA_Y + OPTION_COMBO_MAIN_HEIGHT
// 필터효과
#define OPTION_COMBO_FILTER_X			317 
#define OPTION_COMBO_FILTER_Y			OPTION_COMBO_CONTRAST_Y + OPTION_COMBO_MAIN_HEIGHT
// 시점변환
#define OPTION_COMBO_SEE_CHANGE_X		317 
#define OPTION_COMBO_SEE_CHANGE_Y		OPTION_COMBO_FILTER_Y + OPTION_COMBO_MAIN_HEIGHT
// 효과음 볼륨
#define OPTION_COMBO_MUSIC_VOL_X		307 
#define OPTION_COMBO_MUSIC_VOL_Y		478

//////////////////////// ETC 라디오버튼 위치////////////////////////
// 마우스 포인터
#define OPTION_RADIO_MOUSE_X			304
#define OPTION_RADIO_MOUSE_Y			496
//헬프기능
#define OPTION_RADIO_HELPER_X			304
#define OPTION_RADIO_HELPER_Y			514
// 오퍼레이터 기능
#define OPTION_RADIO_OPER_X				154
#define OPTION_RADIO_OPER_Y				532
// 자동 수평유지
#define OPTION_RADIO_MAINTAIN_X			304
#define OPTION_RADIO_MAINTAIN_Y			550
// 마우스 상하 반전
#define OPTION_RADIO_REVERSE_TB_X		304
#define OPTION_RADIO_REVERSE_TB_Y		568
// 마우스 좌우 반전
#define OPTION_RADIO_REVERSE_LR_X		304
#define OPTION_RADIO_REVERSE_LR_Y		586
// 메뉴 사용시 유닛정지
#define OPTION_RADIO_MENU_X				304
#define OPTION_RADIO_MENU_Y				604
// Music 플레이어 보기
#define OPTION_RADIO_MP3_X				304
#define OPTION_RADIO_MP3_Y				622

// 2008-11-13 by bhsohn 조이스틱 작업
// 조이스틱 옵션
#define OPTION_JOYSTICK_BTN_X		352
#define OPTION_JOYSTICK_BTN_Y		395
// end 2008-11-13 by bhsohn 조이스틱 작업	 
#else
#define OPTION_BTN_APP_X			81
#define OPTION_BTN_APP_Y			648
// 초기화
#define OPTION_BTN_INIT_X			160
#define OPTION_BTN_INIT_Y			648
// 로그오프
#define OPTION_BTN_LOGOFF_X			239
#define OPTION_BTN_LOGOFF_Y			648
// 게임종료
#define OPTION_BTN_GAMEEND_X			318
#define OPTION_BTN_GAMEEND_Y			648

// 라디오 버튼
#define OPTION_RADIO_GAME_INFO_X		306
#define OPTION_RADIO_GAME_INFO_Y		51
#define OPTION_RADIO_GAME_INFO_CAP_X	70
#define OPTION_RADIO_GAME_INFO_CAP_Y	16

// 해상도
#define OPTION_HAESANG_X	410 
#define OPTION_HAESANG_Y	284

// 최소 프레임
#define OPTION_RADIO_SMALLFRM_X		304
#define OPTION_RADIO_SMALLFRM_Y		413

// 닫기 버튼
#define OPTION_BTN_CLOSE_X			428
#define OPTION_BTN_CLOSE_Y			9

//////////////////////// 콤보박스 위치////////////////////////
#define OPTION_COMBO_MAIN_WIDTH			70
#define OPTION_COMBO_MAIN_HEIGHT		17
#define OPTION_COMBO_ELE_WIDTH			70
#define OPTION_COMBO_ELE_HEIGHT			15		// 2012-03-29 by mspark, 콤보 박스 클릭 시 보이는 리스트 배경 이미지 간격 수정 - 기존 13에서 15로 수정

// 시야거리
#define OPTION_COMBO_SEE_RANGE_X		307 
#define OPTION_COMBO_SEE_RANGE_Y		303
// 유닛디테일
#define OPTION_COMBO_UNIT_DETAIL_X		307 
#define OPTION_COMBO_UNIT_DETAIL_Y		319
// 그림자조절
#define OPTION_COMBO_SHADOW_X			307 
#define OPTION_COMBO_SHADOW_Y			335
// 이펙트 조절
#define OPTION_COMBO_EFFECT_X			307 
#define OPTION_COMBO_EFFECT_Y			351
// 감마 조절
#define OPTION_COMBO_GAMMA_X			307 
#define OPTION_COMBO_GAMMA_Y			367
// 대비 조절
#define OPTION_COMBO_CONTRAST_X			307 
#define OPTION_COMBO_CONTRAST_Y			383
// 필터효과
#define OPTION_COMBO_FILTER_X			307 
#define OPTION_COMBO_FILTER_Y			399
// 시점변환
#define OPTION_COMBO_SEE_CHANGE_X		307 
#define OPTION_COMBO_SEE_CHANGE_Y		460
// 효과음 볼륨
#define OPTION_COMBO_MUSIC_VOL_X		307 
#define OPTION_COMBO_MUSIC_VOL_Y		478

//////////////////////// ETC 라디오버튼 위치////////////////////////
// 마우스 포인터
#define OPTION_RADIO_MOUSE_X			304
#define OPTION_RADIO_MOUSE_Y			496
//헬프기능
#define OPTION_RADIO_HELPER_X			304
#define OPTION_RADIO_HELPER_Y			514
// 오퍼레이터 기능
#define OPTION_RADIO_OPER_X				304
#define OPTION_RADIO_OPER_Y				532
// 자동 수평유지
#define OPTION_RADIO_MAINTAIN_X			304
#define OPTION_RADIO_MAINTAIN_Y			550
// 마우스 상하 반전
#define OPTION_RADIO_REVERSE_TB_X		304
#define OPTION_RADIO_REVERSE_TB_Y		568
// 마우스 좌우 반전
#define OPTION_RADIO_REVERSE_LR_X		304
#define OPTION_RADIO_REVERSE_LR_Y		586
// 메뉴 사용시 유닛정지
#define OPTION_RADIO_MENU_X				304
#define OPTION_RADIO_MENU_Y				604
// Music 플레이어 보기
#define OPTION_RADIO_MP3_X				304
#define OPTION_RADIO_MP3_Y				622

// 2008-11-13 by bhsohn 조이스틱 작업
// 조이스틱 옵션
#define OPTION_JOYSTICK_BTN_X		306
#define OPTION_JOYSTICK_BTN_Y		588
// end 2008-11-13 by bhsohn 조이스틱 작업
#endif

// 2012-03-13 by mspark, 게임 사운드 옵션 문제 해결
#define SOUND_SCROLL_RETURN			10000		// 기존 ispark의 효과음을 구하는 계산식에서 -10000값을 이용한 계산으로 인해 스크롤바가 잘못 옮겨지는 것을 정상으로 되돌리기 위한 상수
// end 2012-03-13 by mspark, 게임 사운드 옵션 문제 해결

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CINFOptionSystem::CINFOptionSystem(CAtumNode* pParent)
{
	m_pParent = pParent;
	
	m_bRestored = FALSE;
	m_bInvalidated = FALSE;
	memcpy(&m_pSOptionOld, g_pSOption, sizeof(sOPTION_SYSTEM));	
	memcpy(&m_struSOptionCharacterOld, g_pSOptionCharacter, sizeof(sOPTION_CHARACTER));	
	
	m_bQuitGameMessage = FALSE;
	m_bLogOffGameMessage = FALSE;
	m_fTickSecTime = 0.0f;
	m_fLastTickSecTime = 0.0f;
	m_bRenderToolTip = FALSE;	

	m_bShow = FALSE;

	m_pOptionBk = NULL;
	int nCnt = 0;
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	for( nCnt = 0; nCnt < TOTAL_OPTION_TAB; nCnt++ )
	{
		m_pOptionSubBk[nCnt] = NULL;
	}
#endif	
	for(nCnt = 0;nCnt < MODE_COMBO_NUMBER;nCnt++)
	{
		memset(m_strSelectModeComboData[nCnt], 0x00, MODE_COMBO_STR_LEN);
	}

	m_ptBakPos.x = m_ptBakPos.y = 0;
	m_ptBakSize.x = m_ptBakSize.y = 1;
	m_ptHaesangPos.x = m_ptHaesangPos.y = 0;
	memset(m_szHaesangTxt, 0x00,256);
	

	m_pOptionApp = NULL ;	// 적용
	m_pOptionInit = NULL;	// 초기화
	m_pOptionLogOff = NULL;	// 로그오프
	m_pOptionGameEnd= NULL;	// 로그오프

	// 2008-11-13 by bhsohn 조이스틱 작업
	m_pOptionJoystic = NULL;
	
	m_pFontHaeSang = NULL;

	int nMainId = 0;	
	// 게임옵션
#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	for(nMainId = 0;nMainId < MAX_OPTION_RADIO;nMainId++)
	{
		for(nCnt = 0;nCnt < MAX_OPTION_SYS_CNT;nCnt++)
		{
			m_pGameOpRadioBtn[nMainId][nCnt] = NULL;
		}
	}
#endif
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	for(nMainId = 0;nMainId < MAX_VEDIO_OPTION_RADIO;nMainId++)
	{
		m_pGameVedioOpRadioBtn[nMainId] = NULL;
	}
	for(nMainId = 0;nMainId < MAX_SOUND_OPTION_RADIO;nMainId++)
	{
		m_pGameVedioOpRadioBtn[nMainId] = NULL;
	}

	for(nMainId = 0;nMainId < MAX_ETC_OPTION_RADIO;nMainId++)
	{
		m_pGameVedioOpRadioBtn[nMainId] = NULL;
	}
#endif

	memset(&m_struOpInfo, 0x00, sizeof(structOptionGameInfo));
	m_struOpInfo = InitOptionGameInfo();


	// 그래픽
	for(nCnt = 0;nCnt < MAX_OPTION_GRAPHIC_COMBO;nCnt++)
	{
		m_pComboGraphic[nCnt] = NULL;
	}
	
	m_pMiniFrame = NULL;
#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	for(nCnt = 0;nCnt < MAX_OPTION_ETC_RADIO;nCnt++)
	{
		m_pEtcRadio[nCnt] = NULL;
	}
#endif
	m_pCloseBtn = NULL;
	
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경	
	m_nCurTeb = VEDIO_OPTION_TAB;
	m_pFovBar = NULL;
	m_pFov = NULL;
	m_pSoundVolBar = NULL;
	m_pEffectVolBar = NULL;
	m_pVol = NULL;																				  
	m_bmp3Playview = FALSE;
#endif
	// 2012-03-13 mspark, 게임 사운드 옵션 문제 해결 - 추가
	m_nSoundVolumeTemp = 0;
	m_nMusicVolumeTemp = 0;
	// 2012-03-13 mspark, 게임 사운드 옵션 문제 해결 - 추가

	// 2012-10-31 by jhjang 포인터 초기화 코드 추가
#ifdef C_EPSODE4_UI_CHANGE_JSKIM
	for(nCnt = 0 ; nCnt < MAX_ETC_OPTION_RADIO ; nCnt++)
	{
		m_pGameEctORadioBtn[nCnt] = NULL;
	}
#endif //C_EPSODE4_UI_CHANGE_JSKIM
	// end 2012-10-31 by jhjang 포인터 초기화 코드 추가
	m_pMp3On = NULL;
}


CINFOptionSystem::~CINFOptionSystem()
{
	FLOG( "~CINFOptionSystem()" );		
	util::del(m_pOptionBk);	
	int nCnt = 0;
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	for( nCnt = 0; nCnt < TOTAL_OPTION_TAB; nCnt++ )
	{
		util::del( m_pOptionSubBk[nCnt] );
	}																							  
#endif
	util::del(m_pOptionApp);	// 적용
	util::del(m_pOptionInit);	// 초기화
	util::del(m_pOptionLogOff);	// 로그오프
	util::del(m_pOptionGameEnd);	// 로그오프
	// 2008-11-13 by bhsohn 조이스틱 작업
	util::del(m_pOptionJoystic);
	util::del(m_pFontHaeSang);	
	
#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	int nMainId = 0;	
	for(nMainId = 0;nMainId < MAX_OPTION_RADIO;nMainId++)
	{
		for(nCnt = 0;nCnt < MAX_OPTION_SYS_CNT;nCnt++)
		{		
			util::del(m_pGameOpRadioBtn[nMainId][nCnt]);		
		}
	}
#endif
	
	for(nCnt = 0;nCnt < MAX_OPTION_GRAPHIC_COMBO;nCnt++)
	{
		util::del(m_pComboGraphic[nCnt]);
	}	
	util::del(m_pMiniFrame);
#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	for(nCnt = 0;nCnt < MAX_OPTION_ETC_RADIO;nCnt++)
	{
		util::del(m_pEtcRadio[nCnt]);	 
	}
#endif
	util::del(m_pCloseBtn );

#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	util::del( m_pFovBar );
	util::del( m_pFov );
	util::del( m_pSoundVolBar );
	util::del( m_pEffectVolBar );

	util::del(m_pVol);																		  
#endif
}


HRESULT CINFOptionSystem::InitDeviceObjects()
{
	FLOG( "CINFOptionSystem::InitDeviceObjects()" );
	DataHeader	* pDataHeader ;
	int nCnt = 0;

#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경	
	if(NULL == m_pOptionBk)
	{
		pDataHeader = g_pGameMain->m_GruopImagemanager->FindResource("OPTION");	
		m_pOptionBk = g_pGameMain->m_GruopImagemanager->GetGroupImage( pDataHeader );
		m_pOptionBk->InitDeviceObjects( g_pD3dApp->m_pImageList );
		m_pOptionBk->RestoreDeviceObjects();
	}		  
#else 
 	if(NULL == m_pOptionBk)
 	{
 		m_pOptionBk = new CINFImageEx;
	}
	pDataHeader = FindResource("optionbk");
 	m_pOptionBk->InitDeviceObjects(pDataHeader);
#endif

#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	pDataHeader = g_pGameMain->m_GruopImagemanager->FindResource("optionbk"); //  영상 배경 	
	m_pOptionSubBk[VEDIO_OPTION_TAB] = g_pGameMain->m_GruopImagemanager->GetGroupImage( pDataHeader );
	m_pOptionSubBk[VEDIO_OPTION_TAB]->InitDeviceObjects( g_pD3dApp->m_pImageList );
	m_pOptionSubBk[VEDIO_OPTION_TAB]->RestoreDeviceObjects();
	
	pDataHeader = g_pGameMain->m_GruopImagemanager->FindResource("optionbk1"); //  사운드 배경
	m_pOptionSubBk[SOUND_OPTION_TAB] = g_pGameMain->m_GruopImagemanager->GetGroupImage( pDataHeader );
	m_pOptionSubBk[SOUND_OPTION_TAB]->InitDeviceObjects( g_pD3dApp->m_pImageList );
	m_pOptionSubBk[SOUND_OPTION_TAB]->RestoreDeviceObjects();

	pDataHeader = g_pGameMain->m_GruopImagemanager->FindResource("optionbk2"); //  기타 배경
	m_pOptionSubBk[EX_OPTION_TAB] = g_pGameMain->m_GruopImagemanager->GetGroupImage( pDataHeader );
	m_pOptionSubBk[EX_OPTION_TAB]->InitDeviceObjects( g_pD3dApp->m_pImageList );
	m_pOptionSubBk[EX_OPTION_TAB]->RestoreDeviceObjects();										  
#endif

	// 적용
	{	
		char szUpBtn[30], szDownBtn[30], szSelBtn[30], szDisBtn[30];				
		wsprintf(szUpBtn, "optionapp3");
		wsprintf(szDownBtn, "optionapp1");
		wsprintf(szSelBtn, "optionapp0");
		wsprintf(szDisBtn, "optionapp2");
		if(NULL == m_pOptionApp)
		{
			m_pOptionApp = new CINFImageBtn;
		}
		m_pOptionApp->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn);		
	}

	// 초기화
	{	
		char szUpBtn[30], szDownBtn[30], szSelBtn[30], szDisBtn[30];	
		wsprintf(szUpBtn, "optioninit3");
		wsprintf(szDownBtn, "optioninit1");
		wsprintf(szSelBtn, "optioninit0");
		wsprintf(szDisBtn, "optioninit2");		
		if(NULL == m_pOptionInit)
		{
			m_pOptionInit = new CINFImageBtn;
		}
		m_pOptionInit->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn);		
	}

	// 로그오프
	{	
		char szUpBtn[30], szDownBtn[30], szSelBtn[30], szDisBtn[30];		
				
		wsprintf(szUpBtn, "optionoff3");
		wsprintf(szDownBtn, "optionoff1");
		wsprintf(szSelBtn, "optionoff0");
		wsprintf(szDisBtn, "optionoff2");		

		if(NULL == m_pOptionLogOff)
		{
			m_pOptionLogOff = new CINFImageBtn;
		}
		m_pOptionLogOff->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn);		
	}
	
	// 게임종료
	{	
		char szUpBtn[30], szDownBtn[30], szSelBtn[30], szDisBtn[30];		
		wsprintf(szUpBtn, "optionend3");
		wsprintf(szDownBtn, "optionend1");
		wsprintf(szSelBtn, "optionend0");
		wsprintf(szDisBtn, "optionend2");
		if(NULL == m_pOptionGameEnd)
		{
			m_pOptionGameEnd = new CINFImageBtn;
		}
		m_pOptionGameEnd->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn);		
	}


	// 2008-11-13 by bhsohn 조이스틱 작업	
	{	
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
		char szUpBtn[30], szDownBtn[30], szSelBtn[30], szDisBtn[30];		
		wsprintf(szUpBtn, "p_bt43");
		wsprintf(szDownBtn, "p_bt41");
		wsprintf(szSelBtn, "p_bt40");
		wsprintf(szDisBtn, "p_bt42");
		if(NULL == m_pOptionJoystic)
		{
			m_pOptionJoystic = new CINFImageBtn;
		}
		m_pOptionJoystic->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn,"STRTOOLTIP9");	

#else
		char szUpBtn[30], szDownBtn[30], szSelBtn[30], szDisBtn[30];		
		wsprintf(szUpBtn, "op_j3");
		wsprintf(szDownBtn, "op_j1");
		wsprintf(szSelBtn, "op_j0");
		wsprintf(szDisBtn, "op_j2");
		if(NULL == m_pOptionJoystic)
		{
			m_pOptionJoystic = new CINFImageBtn;
		}
		m_pOptionJoystic->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn);		
#endif		
	}

	// end 2008-11-13 by bhsohn 조이스틱 작업

	{	
		if(NULL == m_pFontHaeSang)
		{
			m_pFontHaeSang = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9, D3DFONT_ZENABLE, FALSE,128,32);
		}
		m_pFontHaeSang->InitDeviceObjects(g_pD3dDev);
	}

	// 라디오 버튼


	int nMainId = 0;	
#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	{
		char chRadioOff[30], chRadioOn[30];
		
		wsprintf(chRadioOff, "radio_b");
		wsprintf(chRadioOn, "radio_a");
		for(nMainId = 0;nMainId < MAX_OPTION_RADIO;nMainId++)
		{
			for(nCnt = 0;nCnt < MAX_OPTION_SYS_CNT;nCnt++)
			{		
				if(NULL == m_pGameOpRadioBtn[nMainId][nCnt])
				{		
					m_pGameOpRadioBtn[nMainId][nCnt] = new CINFImageRadioBtn;
				}				
				m_pGameOpRadioBtn[nMainId][nCnt]->InitDeviceObjects(chRadioOff, chRadioOn);
			}		
		}		
	}
#else
// 	//VEDIO
 	{
 		char chRadioOff[30], chRadioOn[30];
 		
		wsprintf(chRadioOff, "radio_17b");
		wsprintf(chRadioOn, "radio_17a");
 		
		int nMainId = 0;	
 		for(nMainId = 0;nMainId < MAX_VEDIO_OPTION_RADIO;nMainId++)
 		{
 			for(nCnt = 0;nCnt < MAX_OPTION_SYS_CNT;nCnt++)
 			{		
 				if(NULL == m_pGameVedioOpRadioBtn[nMainId])
 				{		
 					m_pGameVedioOpRadioBtn[nMainId] = new CINFImageRadioBtn;
 				}				
 				m_pGameVedioOpRadioBtn[nMainId]->InitDeviceObjects(chRadioOff, chRadioOn);
 			}		
 		}		
 	}
 	//SOUND
 	{
 		char chRadioOff[30], chRadioOn[30];
 		
		wsprintf(chRadioOff, "radio_17b");
		wsprintf(chRadioOn, "radio_17a");
 		
 		int nMainId = 0;	
 		for(nMainId = 0;nMainId < MAX_SOUND_OPTION_RADIO;nMainId++)
 		{
 			for(nCnt = 0;nCnt < MAX_OPTION_SYS_CNT;nCnt++)
			{		
				if(NULL == m_pGameSoundOpRadioBtn[nMainId])
				{		
					m_pGameSoundOpRadioBtn[nMainId] = new CINFImageRadioBtn;
				}				
				m_pGameSoundOpRadioBtn[nMainId]->InitDeviceObjects(chRadioOff, chRadioOn);
			}		
		}		
	}
	//ECT
	{
		char chRadioOff[30], chRadioOn[30];
		
		wsprintf(chRadioOff, "radio_17b");
		wsprintf(chRadioOn, "radio_17a");
		
		int nMainId = 0;	
		for(nMainId = 0;nMainId < MAX_ETC_OPTION_RADIO;nMainId++)
		{
			for(nCnt = 0;nCnt < MAX_OPTION_SYS_CNT;nCnt++)
			{		
				if(NULL == m_pGameEctORadioBtn[nMainId])
				{		
					m_pGameEctORadioBtn[nMainId] = new CINFImageRadioBtn;
				}				
				m_pGameEctORadioBtn[nMainId]->InitDeviceObjects(chRadioOff, chRadioOn);
			}		
		}		
 	}			
#endif

	// 콤보 버튼
	{
		for(nCnt = 0;nCnt < MAX_OPTION_GRAPHIC_COMBO;nCnt++)
		{
			if(NULL == m_pComboGraphic[nCnt])
			{
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
				m_pComboGraphic[nCnt] = new CINFListBox("cbarena","cbarenab");					  
#else
				m_pComboGraphic[nCnt] = new CINFListBox("cbopa","s_combo");				
#endif	
				m_pComboGraphic[nCnt]->InitDeviceObjects();				
			}			
		}
		
		// 시야거리
		{
			char chTmpBuff[32];
			
			for(nCnt = 0;nCnt < 10;nCnt++)
			{
				sprintf(chTmpBuff, "%d", nCnt);
				m_pComboGraphic[OPTION_GRAPHIC_SEE_RANGE]->AddElement(chTmpBuff);	// 시야거리
				m_pComboGraphic[OPTION_GRAPHIC_UNIT_DETAIL]->AddElement(chTmpBuff);	// 유닛 디테일
				m_pComboGraphic[OPTION_GRAPHIC_SHADOW]->AddElement(chTmpBuff);	// 그림자조절
				
			}
			// 2012-06-01 by jhahn, 이펙트 콤보수정
#if C_USER_EFFECT_CONTROL  // 2012-10-23 by jhahn, 이펙트 옵션 조절
			
			for(nCnt = 0;nCnt < 3;nCnt++)
			{
				sprintf(chTmpBuff, "%d", nCnt);
				m_pComboGraphic[OPTION_GRAPHIC_EFFECT]->AddElement(chTmpBuff);	// 이펙트 조절			
			}
#else
			
			for(nCnt = 0;nCnt < 9;nCnt++)
			{
				sprintf(chTmpBuff, "%d", nCnt);
				m_pComboGraphic[OPTION_GRAPHIC_EFFECT]->AddElement(chTmpBuff);	// 이펙트 조절			
			}


#endif
			//end 2012-06-01 by jhahn, 이펙트 콤보수정
			
			for(nCnt = 5;nCnt >= -4;nCnt--)
			{
				sprintf(chTmpBuff, "%d", nCnt);			
				m_pComboGraphic[OPTION_GRAPHIC_GAMMA]->AddElement(chTmpBuff);	// 감마 조절
			}
			{
				sprintf(chTmpBuff, "%s", STRMSG_C_INTERFACE_0035);						
				m_pComboGraphic[OPTION_GRAPHIC_CONTRAST]->AddElement(chTmpBuff);	// 대비 조절
				m_pComboGraphic[OPTION_GRAPHIC_FILTER]->AddElement(chTmpBuff);	// 필터 효과
			}
			for(nCnt = 1;nCnt < 10;nCnt++)
			{
				sprintf(chTmpBuff, "%d", nCnt);			
				m_pComboGraphic[OPTION_GRAPHIC_CONTRAST]->AddElement(chTmpBuff);	// 대비 조절
				m_pComboGraphic[OPTION_GRAPHIC_FILTER]->AddElement(chTmpBuff);	// 필터 효과			
			}

			// 시점변환
			sprintf(chTmpBuff, "%s", STRMSG_C_INTERFACE_0002);			//1 인칭
			m_pComboGraphic[OPTION_ETC_SEE_CHANGE]->AddElement(chTmpBuff);	// 시점변환			
			sprintf(chTmpBuff, "%s", STRMSG_C_INTERFACE_0003);			//3 인칭
			m_pComboGraphic[OPTION_ETC_SEE_CHANGE]->AddElement(chTmpBuff);	// 시점변환	
			m_pComboGraphic[OPTION_ETC_SEE_CHANGE]->SetSelectItem(1); //2012-09-17 by jhahn	시점 변환 버그 수정
			
#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
			// 볼륨 조절
			for(nCnt = 0;nCnt < 10;nCnt++)
			{
				sprintf(chTmpBuff, "%d", nCnt);
				m_pComboGraphic[OPTION_ETC_VOLUMNE]->AddElement(chTmpBuff);	// 볼륨
			}
#endif

			for(nCnt = 0;nCnt < MAX_OPTION_GRAPHIC_COMBO;nCnt++)
			{
				m_pComboGraphic[nCnt]->UpdateItem();				
			}
		}		

		{
			char chRadioOff[30], chRadioOn[30];
			
			wsprintf(chRadioOff, "radio_17b");
			wsprintf(chRadioOn, "radio_17a");			
			
			if(NULL == m_pMiniFrame)
			{
				m_pMiniFrame = new CINFImageRadioBtn;
			}
			m_pMiniFrame->InitDeviceObjects(chRadioOff, chRadioOn);
		}		
	}

#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	// ETC라디오 버튼
	{
		char chRadioOff[30], chRadioOn[30];
		
		wsprintf(chRadioOff, "radio_17b");
		wsprintf(chRadioOn, "radio_17a");			
		
		for(nCnt = 0;nCnt < MAX_OPTION_ETC_RADIO;nCnt++)
		{		
			if(NULL == m_pEtcRadio[nCnt])
			{		
				m_pEtcRadio[nCnt] = new CINFImageRadioBtn;
			}				
			m_pEtcRadio[nCnt]->InitDeviceObjects(chRadioOff, chRadioOn);
		}
				
	}	
#endif	
	// 닫기 버튼
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
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	{
		if(NULL == m_pMp3On )
		{
			m_pMp3On = new CINFImageEx;
		}
		pDataHeader = FindResource( "mp_off" );
		m_pMp3On->InitDeviceObjects( pDataHeader );	
	}
	{
		char  szScBall[30];
		if (NULL == m_pFovBar)
		{
			m_pFovBar = new CINFArenaScrollBar;
		}
		wsprintf(szScBall, "mp_vol");

		m_pFovBar->InitDeviceObjects(0, szScBall);
	}
	{
		char  szScBall[30];	
		if(NULL == m_pSoundVolBar)
		{
			m_pSoundVolBar = new CINFArenaScrollBar;
		}		
		wsprintf(szScBall,"mp_vol");
		
		m_pSoundVolBar->InitDeviceObjects( 0, szScBall );
	}
	{
		char  szScBall[30];	
		if(NULL == m_pEffectVolBar)
		{
			m_pEffectVolBar = new CINFArenaScrollBar;
		}		
		wsprintf(szScBall,"mp_vol");
		
		m_pEffectVolBar->InitDeviceObjects( 0, szScBall );
	}
	m_pVol = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9, D3DFONT_ZENABLE, FALSE,256,32);
	m_pVol->InitDeviceObjects(g_pD3dDev);
#endif
	// 라디오 버튼 갱신
	UpdateOptionInterface(&m_struOpInfo);
	
	// 2012-03-13 mspark, 게임 사운드 옵션 문제 해결 - 추가
	m_nSoundVolumeTemp = g_pSOption->sSoundVolume;
	m_nMusicVolumeTemp = g_pSOption->sMusicVolume;
	m_nFOVTemp = g_pSOption->sFovValue;
	// 2012-03-13 mspark, 게임 사운드 옵션 문제 해결 - 추가

	return S_OK ;
}

HRESULT CINFOptionSystem::RestoreDeviceObjects()
{
	FLOG( "CINFOptionSystem::RestoreDeviceObjects()" );
	
	int nCnt = 0;
	// 배경
	if(m_pOptionBk )
	{
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
		m_pOptionBk->RestoreDeviceObjects();	
		m_ptBakSize.x  = m_pOptionBk->GetMaxPos().x - m_pOptionBk->GetMinPos().x;
		m_ptBakSize.y  = m_pOptionBk->GetMaxPos().y - m_pOptionBk->GetMinPos().y;
#else 
		m_pOptionBk->RestoreDeviceObjects();	
		m_ptBakSize = m_pOptionBk->GetImgSize();
#endif

		m_ptBakPos.x = (g_pD3dApp->GetBackBufferDesc().Width/2 - m_ptBakSize.x/2);
		m_ptBakPos.y = (g_pD3dApp->GetBackBufferDesc().Height/2 - m_ptBakSize.y/2);

		if(m_ptBakPos.x < 0)
		{
			m_ptBakPos.x = 0;
		}
		if(m_ptBakPos.y < 0)
		{
			m_ptBakPos.y = 0;
		}
	}
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	for( nCnt = 0; nCnt < TOTAL_OPTION_TAB; nCnt++ )
	{
		m_pOptionSubBk[nCnt]->RestoreDeviceObjects();
	}
#endif
	if(m_pOptionApp)
	{
		m_pOptionApp->RestoreDeviceObjects();
	}
	if(m_pOptionInit)
	{
		m_pOptionInit->RestoreDeviceObjects();
	}
	if(m_pOptionLogOff)
	{
		m_pOptionLogOff->RestoreDeviceObjects();
	}
	if(m_pOptionGameEnd)
	{
		m_pOptionGameEnd->RestoreDeviceObjects();
	}
	// 2008-11-13 by bhsohn 조이스틱 작업
	if(m_pOptionJoystic)
	{
		m_pOptionJoystic->RestoreDeviceObjects();
	}
	// end 2008-11-13 by bhsohn 조이스틱 작업

	if(m_pFontHaeSang)
	{
		m_pFontHaeSang->RestoreDeviceObjects();
	}
#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	{		
		int nMainId = 0;	
		for(nMainId = 0;nMainId < MAX_OPTION_RADIO;nMainId++)
		{
			for(nCnt = 0;nCnt < MAX_OPTION_SYS_CNT;nCnt++)
			{	
				if(m_pGameOpRadioBtn[nMainId][nCnt])
				{
					m_pGameOpRadioBtn[nMainId][nCnt]->RestoreDeviceObjects();			
				}
			}
		}		

	}
#else
	//VEDIO
	{
		int nMainId = 0;	
		for(nMainId = 0;nMainId < MAX_VEDIO_OPTION_RADIO;nMainId++)
		{
			m_pGameVedioOpRadioBtn[nMainId]->RestoreDeviceObjects();
		}		
	}
	//SOUND
	{
		int nMainId = 0;	
		for(nMainId = 0;nMainId < MAX_SOUND_OPTION_RADIO;nMainId++)
		{
			m_pGameSoundOpRadioBtn[nMainId]->RestoreDeviceObjects();
		}		
	}
	//ECT
	{
		int nMainId = 0;	
		for(nMainId = 0;nMainId < MAX_ETC_OPTION_RADIO;nMainId++)
		{
			m_pGameEctORadioBtn[nMainId]->RestoreDeviceObjects();
		}		
	}
#endif
	{
		for(nCnt = 0;nCnt < MAX_OPTION_GRAPHIC_COMBO;nCnt++)
		{	
			if(m_pComboGraphic[nCnt])
			{
				m_pComboGraphic[nCnt]->RestoreDeviceObjects();			
			}
		}
		
		if(m_pMiniFrame)
		{
			m_pMiniFrame->RestoreDeviceObjects();
		}
#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
		for(nCnt = 0;nCnt < MAX_OPTION_ETC_RADIO;nCnt++)
		{
			if(m_pEtcRadio[nCnt])
			{
				m_pEtcRadio[nCnt]->RestoreDeviceObjects();			
			}
		}		
#else
		if( m_pMp3On )
			m_pMp3On->RestoreDeviceObjects();
#endif
	}
	if(m_pCloseBtn)
	{
		m_pCloseBtn->RestoreDeviceObjects();		
	}
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	if (m_pFovBar)
		m_pFovBar->RestoreDeviceObjects();

	if( m_pSoundVolBar )
		m_pSoundVolBar->RestoreDeviceObjects();

	if( m_pEffectVolBar )
		m_pEffectVolBar->RestoreDeviceObjects();

	m_pVol->RestoreDeviceObjects();																  
#endif

	UpdateBtnPos(m_ptBakPos.x, m_ptBakPos.y);

	m_bRestored = TRUE;
	return S_OK ;
}

HRESULT CINFOptionSystem::DeleteDeviceObjects()
{
	FLOG( "CINFOptionSystem::DeleteDeviceObjects()" );
	int nCnt;	
	if(m_pOptionBk)
	{
		m_pOptionBk->DeleteDeviceObjects();
		util::del(m_pOptionBk);
	}
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경

	for( nCnt = 0; nCnt < TOTAL_OPTION_TAB; nCnt++ )
	{
		if(m_pOptionSubBk[nCnt])
		{
			m_pOptionSubBk[nCnt]->DeleteDeviceObjects();
			util::del(m_pOptionSubBk[nCnt]);
		}
	}
#endif
	if(m_pOptionApp)
	{		
		m_pOptionApp->DeleteDeviceObjects();
		util::del(m_pOptionApp);
	}
	if(m_pOptionInit)
	{		
		m_pOptionInit->DeleteDeviceObjects();
		util::del(m_pOptionInit);
	}
	if(m_pOptionLogOff)
	{		
		m_pOptionLogOff->DeleteDeviceObjects();
		util::del(m_pOptionLogOff);
	}
	if(m_pOptionGameEnd)
	{		
		m_pOptionGameEnd->DeleteDeviceObjects();
		util::del(m_pOptionGameEnd);
	}
	// 2008-11-13 by bhsohn 조이스틱 작업
	if(m_pOptionJoystic)
	{
		m_pOptionJoystic->DeleteDeviceObjects();
		util::del(m_pOptionJoystic);
	}
	// end 2008-11-13 by bhsohn 조이스틱 작업
	if(m_pFontHaeSang)
	{
		m_pFontHaeSang->DeleteDeviceObjects();
		util::del(m_pFontHaeSang);
	}

#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	int nMainId = 0;	
	for(nMainId = 0;nMainId < MAX_OPTION_RADIO;nMainId++)
	{
		for(nCnt = 0;nCnt < MAX_OPTION_SYS_CNT;nCnt++)
		{		
			if(m_pGameOpRadioBtn[nMainId][nCnt])
			{
				m_pGameOpRadioBtn[nMainId][nCnt]->DeleteDeviceObjects();	
				util::del(m_pGameOpRadioBtn[nMainId][nCnt]);
			}
		}
	}
#else
	//VEDIO
	{
		int nMainId = 0;	
		for(nMainId = 0;nMainId < MAX_VEDIO_OPTION_RADIO;nMainId++)
		{
			m_pGameVedioOpRadioBtn[nMainId]->DeleteDeviceObjects();
				util::del(m_pGameVedioOpRadioBtn[nMainId]);
		}		
	}
	//SOUND
	{
		int nMainId = 0;	
		for(nMainId = 0;nMainId < MAX_SOUND_OPTION_RADIO;nMainId++)
		{
			m_pGameSoundOpRadioBtn[nMainId]->DeleteDeviceObjects();
			util::del(m_pGameSoundOpRadioBtn[nMainId]);
		}
	}
	//ECT
	{
		int nMainId = 0;	
		for(nMainId = 0;nMainId < MAX_ETC_OPTION_RADIO;nMainId++)
		{
			m_pGameEctORadioBtn[nMainId]->DeleteDeviceObjects();
			util::del(m_pGameEctORadioBtn[nMainId]);
		}		
	}
#endif
	for(nCnt = 0;nCnt < MAX_OPTION_GRAPHIC_COMBO;nCnt++)
	{			
		if(m_pComboGraphic[nCnt])
		{
			m_pComboGraphic[nCnt]->DeleteDeviceObjects();	
			util::del(m_pComboGraphic[nCnt]);
		}
	}	

	if(m_pMiniFrame)
	{
		m_pMiniFrame->DeleteDeviceObjects();	
		util::del(m_pMiniFrame);		
	}
#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	for(nCnt = 0;nCnt < MAX_OPTION_ETC_RADIO;nCnt++)
	{
		if(m_pEtcRadio[nCnt])
		{
			m_pEtcRadio[nCnt]->DeleteDeviceObjects();	
			util::del(m_pEtcRadio[nCnt]);
		}
	}	
#endif

	if(m_pCloseBtn)
	{
		m_pCloseBtn->DeleteDeviceObjects();	
		util::del(m_pCloseBtn);
	}
	
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	if (m_pFovBar)
	{
		m_pFovBar->DeleteDeviceObjects();
		util::del(m_pFovBar);
	}

	if( m_pMp3On )
	{
		m_pMp3On->DeleteDeviceObjects();
		util::del( m_pMp3On );
	}

	if( m_pSoundVolBar )
	{
		m_pSoundVolBar->DeleteDeviceObjects();
		util::del( m_pSoundVolBar );
	}
	
	if( m_pEffectVolBar )
	{
		m_pEffectVolBar->DeleteDeviceObjects();
		util::del( m_pEffectVolBar );
	}
	
	if( m_pVol )
	{
		m_pVol->DeleteDeviceObjects();
		util::del( m_pVol );
	}  
#endif
	m_bInvalidated = FALSE;

	return S_OK ;
}


HRESULT CINFOptionSystem::InvalidateDeviceObjects()
{
	FLOG( "CINFOptionSystem::InvalidateDeviceObjects()" );
	int nCnt = 0;	
	if(m_pOptionBk )
		m_pOptionBk->InvalidateDeviceObjects();
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경

	for( nCnt = 0; nCnt < TOTAL_OPTION_TAB; nCnt++ )
	{
		m_pOptionSubBk[nCnt]->InvalidateDeviceObjects();
	}
#endif
	if(m_pOptionApp )
		m_pOptionApp->InvalidateDeviceObjects();
	if(m_pOptionInit)
		m_pOptionInit->InvalidateDeviceObjects();
	if(m_pOptionLogOff)
		m_pOptionLogOff->InvalidateDeviceObjects();
	if(m_pOptionGameEnd)
		m_pOptionGameEnd->InvalidateDeviceObjects();
	// 2008-11-13 by bhsohn 조이스틱 작업
	if(m_pOptionJoystic)
	{
		m_pOptionJoystic->InvalidateDeviceObjects();
	}
	// end 2008-11-13 by bhsohn 조이스틱 작업
	if(m_pFontHaeSang)
	{
		m_pFontHaeSang->InvalidateDeviceObjects();
	}
#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	int nMainId = 0;	
	for(nMainId = 0;nMainId < MAX_OPTION_RADIO;nMainId++)
	{
		for(nCnt = 0;nCnt < MAX_OPTION_SYS_CNT;nCnt++)
		{		
			if(m_pGameOpRadioBtn[nMainId][nCnt])
			{
				m_pGameOpRadioBtn[nMainId][nCnt]->InvalidateDeviceObjects();
			}
		}
	}
#else
	//VEDIO
	{
		int nMainId = 0;	
		for(nMainId = 0;nMainId < MAX_VEDIO_OPTION_RADIO;nMainId++)
		{
			m_pGameVedioOpRadioBtn[nMainId]->InvalidateDeviceObjects();
		}		
	}
	//SOUND
	{
		int nMainId = 0;	
		for(nMainId = 0;nMainId < MAX_SOUND_OPTION_RADIO;nMainId++)
		{
			m_pGameSoundOpRadioBtn[nMainId]->InvalidateDeviceObjects();
		}		
	}
	//ECT
	{
		int nMainId = 0;	
		for(nMainId = 0;nMainId < MAX_ETC_OPTION_RADIO;nMainId++)
		{
			m_pGameEctORadioBtn[nMainId]->InvalidateDeviceObjects();
		}		
	} 
#endif
	for(nCnt = 0;nCnt < MAX_OPTION_GRAPHIC_COMBO;nCnt++)
	{			
		if(m_pComboGraphic[nCnt])
		{
			m_pComboGraphic[nCnt]->InvalidateDeviceObjects();
		}
	}
		
	if(m_pMiniFrame)
	{
		m_pMiniFrame->InvalidateDeviceObjects();
	}

#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	for(nCnt = 0;nCnt < MAX_OPTION_ETC_RADIO;nCnt++)
	{
		if(m_pEtcRadio[nCnt])
		{
			m_pEtcRadio[nCnt]->InvalidateDeviceObjects();	
		}
	}
#endif
	if(m_pCloseBtn)
	{
		m_pCloseBtn->InvalidateDeviceObjects();		
	}
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경	
	if (m_pFovBar)
	{
		m_pFovBar->InvalidateDeviceObjects();
	}

	if( m_pMp3On )
	{
		m_pMp3On->InvalidateDeviceObjects();
	}

	if( m_pSoundVolBar )
	{
		m_pSoundVolBar->InvalidateDeviceObjects();
	}
	
	if( m_pEffectVolBar )
	{
		m_pEffectVolBar->InvalidateDeviceObjects();
	}

	if( m_pVol )
	{
		m_pVol->InvalidateDeviceObjects();
	}
#endif
	
	m_bRestored = FALSE;
	m_bInvalidated = TRUE;

	return S_OK ;
}


void CINFOptionSystem::Tick()
{
	FLOG( "CINFOptionSystem::Tick()" );
	
	if(TRUE == m_bQuitGameMessage)
	{
		DWORD dwCurTick = timeGetTime();
		if(dwCurTick - m_fLastTickSecTime < END_GAME_TIMING_ATTACK) 
		{
			if(dwCurTick - m_fLastTickSecTime > m_fTickSecTime)
			{
				m_fTickSecTime += 1000.0f;

				int tempTime = ((END_GAME_TIMING_ATTACK-m_fTickSecTime)/1000)+1;
				char message[256];
				
				if(TRUE == m_bLogOffGameMessage)
					wsprintf(message,STRMSG_C_INTERFACE_0036, tempTime);//"로그오프까지 \\r%d초\\r 남았습니다."
				else
					wsprintf(message,STRMSG_C_INTERFACE_0037, tempTime);//"게임종료까지 \\r%d초\\r 남았습니다."

//				g_pD3dApp->m_pChat->CreateChatChild(message,COLOR_SYSTEM);
				g_pGameMain->m_pInfWindow->SetMessageBoxTitleChange(message, _Q_QUIT_MESSAGE);				
			}
		}
		else
		{
			// End the game
			g_pD3dApp->EndGame(m_bLogOffGameMessage);

			// Reset Vars
			m_bQuitGameMessage = FALSE;
			m_fTickSecTime = 0.0f;
			m_fLastTickSecTime = 0.0f;
		}
	}
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	switch(m_nCurTeb)
	{
	case VEDIO_OPTION_TAB:
		{
			VideoTick();
		}
		break;
	case SOUND_OPTION_TAB:
		{
			SoundTick();	
		}
		break;		
	case EX_OPTION_TAB:
		{
			ExTick();
		}
		break;
	}
#endif
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CINFOptionSystem::OptionQualityCombo()
/// \brief		최소프레임 설정.
/// \author		
/// \date		
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CINFOptionSystem::OptionQualityCombo()
{
	m_struOpInfo.struSOptionSetup.sLowQuality ^= TRUE;	
	UpdateQuality(&m_struOpInfo.struSOptionSetup);

	// 라디오 버튼 갱신
	UpdateOptionInterface(&m_struOpInfo);

	//g_pInterface->SaveOptionFile();
	
	// 최소 프레임 모드를 키면 리로드한다. 	
	//g_pD3dApp->ReLoadEnemyRenderList();		
	// end 2007-08-29 by bhsohn 최소 프레임시 기본 아머만 로딩하게끔 변경
	return TRUE;
}

void CINFOptionSystem::UpdateQuality(sOPTION_SYSTEM* pOption)
{
	if(pOption->sLowQuality)
	{		
		pOption->sLowQuality				= TRUE;
		pOption->sTerrainRender			= MIN_QUALITY_RENDER;
		pOption->sTerrainEffectRender	= MIN_QUALITY_RENDER;
//		g_pSOption->sObjectViewDistance		= MIN_QUALITY_RENDER;
		pOption->sShadowState			= MIN_QUALITY_RENDER;
		pOption->sUnitDetail				= MIN_QUALITY_RENDER;
		pOption->sContrast				= MIN_QUALITY_RENDER;
		pOption->sFilter					= MIN_QUALITY_RENDER;
//		pOption->sReflactive				= FALSE;

		//SetFogLevel( g_pShuttleChild->m_myShuttleInfo.MapChannelIndex.MapIndex, TRUE );
	}
	else
	{
		// 말것인가
		memcpy(pOption, &m_pSOptionOld, sizeof(sOPTION_SYSTEM));
		pOption->sLowQuality				= FALSE;
		
		//SetFogLevel( g_pShuttleChild->m_myShuttleInfo.MapChannelIndex.MapIndex, TRUE );
	}

}


void CINFOptionSystem::Render()
{
	FLOG( "CINFOptionSystem::Render()" );	

	// Show여부 
	if(!IsOptionShow())
	{
		return;
	}

	// 배경
	{
		m_pOptionBk->Move(m_ptBakPos.x, m_ptBakPos.y);
		m_pOptionBk->Render();
	}
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	switch(m_nCurTeb)
	{
		case VEDIO_OPTION_TAB:
			{
				VideoRender();
			}
			break;
		case SOUND_OPTION_TAB:
			{
				SoundRender();
			}
			break;
		case EX_OPTION_TAB:
			{
				ExRender();
			}
			break;
	}
	m_pCloseBtn->Render();	

	m_pOptionApp->Render();	// 적용
	m_pOptionInit->Render();	// 초기화
	m_pOptionLogOff->Render();	// 로그오프
	m_pOptionGameEnd->Render();	// 게임종료	  
#else
	// 버튼 
	{
		m_pOptionApp->Render();	// 적용
		m_pOptionInit->Render();	// 초기화
		m_pOptionLogOff->Render();	// 로그오프
		m_pOptionGameEnd->Render();	// 게임종료
		// 2008-11-13 by bhsohn 조이스틱 작업
		m_pOptionJoystic->Render();	// 조이스틱옵션
		// end 2008-11-13 by bhsohn 조이스틱 작업
	}
	// 그래픽
	{
		// 해상도
		m_pFontHaeSang->DrawText(m_ptHaesangPos.x,m_ptHaesangPos.y,
									GUI_FONT_COLOR_W,
									m_szHaesangTxt);
	}

	int nCnt =0;
	int nMainId = 0;	
	for(nMainId = 0;nMainId < MAX_OPTION_RADIO;nMainId++)
	{
		for(nCnt =0; nCnt < MAX_OPTION_SYS_CNT; nCnt++)
		{		
			m_pGameOpRadioBtn[nMainId][nCnt]->Render();
		}	
	}
	
	m_pMiniFrame->Render();	
	
	for(nCnt = 0;nCnt < MAX_OPTION_ETC_RADIO;nCnt++)
	{
		// 2008-11-13 by bhsohn 조이스틱 작업
		// 마우스 좌우 반전은 체크안함
		if(OPTION_ETC_MOUSE_REVERSE_LR == nCnt)
		{
			continue;
		}
		// end 2008-11-13 by bhsohn 조이스틱 작업

		m_pEtcRadio[nCnt]->Render();		
	}

	// 콤보박스는 가장 마지막에 랜더링 	
	{
		int nSelCombo = -1;
		for(nCnt = 0;nCnt < MAX_OPTION_GRAPHIC_COMBO;nCnt++)
		{
			if(m_pComboGraphic[nCnt]->IsShowItem())
			{
				nSelCombo = nCnt;
				break;
			}
		}
		for(nCnt = 0;nCnt < MAX_OPTION_GRAPHIC_COMBO;nCnt++)
		{	
			if(nCnt != nSelCombo)
			{
				m_pComboGraphic[nCnt]->Render();
			}
		}
		if(nSelCombo != -1)
		{
			m_pComboGraphic[nSelCombo]->Render();
		}
	}
	m_pCloseBtn->Render();	
#endif
}

int CINFOptionSystem::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	FLOG( "CINFOptionSystem::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)" );
	// Show여부 
	if(!IsOptionShow())
	{
		return INF_MSGPROC_NORMAL;
	}
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	switch(m_nCurTeb)
	{
	case VEDIO_OPTION_TAB:
		{
			if(VideoProc( uMsg, wParam, lParam) == INF_MSGPROC_BREAK )
				return INF_MSGPROC_BREAK;
		}
		break;
	case SOUND_OPTION_TAB:
		{
			if(SoundProc( uMsg, wParam, lParam) == INF_MSGPROC_BREAK )
				return INF_MSGPROC_BREAK;
		}
		break;
	case EX_OPTION_TAB:
		{
			if(ExProc( uMsg, wParam, lParam) == INF_MSGPROC_BREAK)
				return INF_MSGPROC_BREAK;
		}
		break;
	}
#endif

	switch(uMsg)
	{
	case WM_MOUSEMOVE:
		{
			return OnMouseMove(wParam, lParam);
		}
		break;
	case WM_LBUTTONDOWN:
		{
			return OnLButtonDown(wParam, lParam);
		}
		break;
	case WM_RBUTTONDOWN:
		{
//			 2007-11-28 by bhsohn 스크린 샷 모드 추가
//						if(g_pInterface->IsScreenShotMode())
//						{
//							OptionQualityCombo();
//						}
//						// end 2007-11-28 by bhsohn 스크린 샷 모드 추가
		}
		break;
	case WM_LBUTTONUP:
		{
			return OnLButtonUp(wParam, lParam);
		}
		break;
	}
	return INF_MSGPROC_NORMAL;

}
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
int CINFOptionSystem::VideoProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
	switch(uMsg)
	{
	case WM_MOUSEMOVE:
		{
			return OnVedioMouseMove(wParam, lParam);
		}
		break;
	case WM_LBUTTONDOWN:
		{
			return OnVedioLButtonDown(wParam, lParam);
		}
		break;
	case WM_RBUTTONDOWN:
		{
		}
		break;
	case WM_LBUTTONUP:
		{
			return OnVedioLButtonUp(wParam, lParam);
		}
		break;
	}
	return INF_MSGPROC_NORMAL;
}

int CINFOptionSystem::SoundProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	structOptionGameInfo* pOptionInfo = &m_struOpInfo;
	if( pOptionInfo->struSOptionSetup.sMp3Player == OPTION_DEFAULT_MP3_USE )
	{
		if( g_pGameMain->m_pMp3Player->WndProc(uMsg, wParam, lParam) == INF_MSGPROC_BREAK )
			return INF_MSGPROC_BREAK;
	}

	switch(uMsg)
	{
	case WM_MOUSEMOVE:
		{
			return OnSoundMouseMove(wParam, lParam);
		}
		break;
	case WM_LBUTTONDOWN:
		{
			return OnSoundLButtonDown(wParam, lParam);
		}
		break;
	case WM_RBUTTONDOWN:
		{
		}
		break;
	case WM_LBUTTONUP:
		{
			return OnSoundLButtonUp(wParam, lParam);
		}
		break;
	}

	return INF_MSGPROC_NORMAL;
}

int CINFOptionSystem::ExProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_MOUSEMOVE:
		{
			return OnExMouseMove(wParam, lParam);
		}
		break;
	case WM_LBUTTONDOWN:
		{
			return OnExLButtonDown(wParam, lParam);
		}
		break;
	case WM_RBUTTONDOWN:
		{
		}
		break;
	case WM_LBUTTONUP:
		{
			return OnExLButtonUp(wParam, lParam);
		}
		break;
	}
	return INF_MSGPROC_NORMAL;
}					
#endif

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		콤보박스에서 명칭변경
/// \author		
/// \date		2008-03-25 ~ 2008-03-25
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFOptionSystem::SetComboData(int nIdx, char* pSrc)
{
	if((nIdx >= MODE_COMBO_NUMBER)
		|| (nIdx < 0))
	{
		return;
	}
	strncpy(m_strSelectModeComboData[nIdx], pSrc, MODE_COMBO_STR_LEN);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		현재 보이고 있냐 여부
/// \author		
/// \date		2008-03-25 ~ 2008-03-25
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CINFOptionSystem::IsOptionShow()
{
	return m_bShow;

}
///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		보이게 안보이게
/// \author		
/// \date		2008-03-25 ~ 2008-03-25
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void  CINFOptionSystem::ShowOptionWnd(BOOL bShow)
{
	int nFOV = m_nFOVTemp;
	m_pFovBar->SetScrollStep(nFOV, TRUE);

	// 2012-03-13 mspark, 게임 사운드 옵션 문제 해결 - 추가
	int nVol = (SOUND_SCROLL_RETURN + m_nSoundVolumeTemp) / 100;
	m_pSoundVolBar->SetScrollStep( nVol, TRUE );

	nVol = m_nMusicVolumeTemp;
  	m_pEffectVolBar->SetScrollStep( nVol, TRUE );

	int nNewVol = -10000;
	if(m_nMusicVolumeTemp > 0)
	{
		float fPercent = (float)m_nMusicVolumeTemp*0.01f;
		float fLog     = (float)log(fPercent);
		nNewVol = (int)(fLog*1000.f);
	}
	// 2013-02-05 by bhsohn Sound 없는 장비에서 Exception오류 나는 현상 수정
	if(g_pD3dApp->m_pSound && g_pD3dApp->m_pSound->IsSoundDevice())
	{
		g_pD3dApp->m_pSound->GetMusicMp3()->SetAtumMusicVolume( nNewVol );
	}
	// end 2012-03-13 mspark, 게임 사운드 옵션 문제 해결 - 추가

	m_bShow = bShow;

	// 2009. 08. 19 by jsKim 랜딩 중 메뉴 생성할 경우 커서가 변하지 않는 버그
	if(bShow)
	{
		// 2012-12-17 by jhjang 비행중 마우스 커서가 제대로 동작하지 않는 버그 수정
		//g_INFCnt++;
		g_pD3dApp->AddINFCnt();
		// 2012-12-17 by jhjang 비행중 마우스 커서가 제대로 동작하지 않는 버그 수정
	}
	else
	{
		// 2012-12-17 by jhjang 비행중 마우스 커서가 제대로 동작하지 않는 버그 수정
		//g_INFCnt--;
		//if(g_INFCnt==0)
		g_pD3dApp->RemoveINFCnt();
		// end 2012-12-17 by jhjang 비행중 마우스 커서가 제대로 동작하지 않는 버그 수정
		// 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현
		g_pInterface->m_pToolTip->m_bToolTipState = FALSE;
		// end 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현
	}
	// end 2009. 08. 19 by jsKim 랜딩 중 메뉴 생성할 경우 커서가 변하지 않는 버그
	if(bShow)
	{
		// 2009-03-04 by bhsohn 메뉴창 띠울시, 마우스 없어질수 있는 문제 수정
		if(g_pInterface->m_pTarget)
		{
			g_pInterface->m_pTarget->SetMouseType(MOUSE_TYPE_0);
		}
		// end 2009-03-04 by bhsohn 메뉴창 띠울시, 마우스 없어질수 있는 문제 수정

		memcpy(&m_pSOptionOld, g_pSOption, sizeof(sOPTION_SYSTEM));
		memcpy(&m_struSOptionCharacterOld, g_pSOptionCharacter, sizeof(sOPTION_CHARACTER));	

		memcpy(&m_struOpInfo.struSOptionSetup, g_pSOption, sizeof(sOPTION_SYSTEM));		
		memcpy(&m_struOpInfo.struSOptionCharacter, g_pSOptionCharacter, sizeof(sOPTION_CHARACTER));		
		
		// 오퍼레이터
		m_struOpInfo.bOperator = g_pInterface->GetOperatorMode();

		// 옵션 ETC메뉴
		memcpy(&m_struOpInfo.struSOptionEtc, g_pSOptionEtc, sizeof(sOPTION_ETC));
		
		// 인터페이스 감추기
		//m_struOpInfo.struSOptionEtc.bRadioInfo[OPTION_RADIO_INTERFACE_HIDE] = !g_pInterface->GetShowInterface();			
		m_struOpInfo.struSOptionEtc.bRadioInfo[OPTION_RADIO_INTERFACE_HIDE] = g_pD3dApp->IsOptionEtc(OPTION_RADIO_INTERFACE_HIDE);

		// 라디오 버튼 갱신
		UpdateOptionInterface(&m_struOpInfo);	
		
		// 2008-11-13 by bhsohn 조이스틱 작업
		{
			CJoyStickInput *pJoyStick = g_pD3dApp->GetJoystickControl();
			BOOL bEnableBtn = FALSE;
			if(pJoyStick || g_pD3dApp->GetJoystickCtrlDeviceListLen() > 0)
			{
				bEnableBtn = TRUE;				
			}

			m_pOptionJoystic->EnableBtn(bEnableBtn);
		}
		// end 2008-11-13 by bhsohn 조이스틱 작업
		
	}
}
void CINFOptionSystem::UpdateBtnPos(int nBackPosX, int nBackPosY)
{
	int nCnt = 0;
 	int nPosX, nPosY;
	{
		nPosX = nBackPosX + OPTION_BTN_APP_X;
		nPosY = nBackPosY + OPTION_BTN_APP_Y;		
		m_pOptionApp->SetBtnPosition(nPosX, nPosY);
	}
	{
		nPosX = nBackPosX + OPTION_BTN_INIT_X;
		nPosY = nBackPosY + OPTION_BTN_INIT_Y;		
		m_pOptionInit->SetBtnPosition(nPosX, nPosY);
	}
	{
		nPosX = nBackPosX + OPTION_BTN_LOGOFF_X;
		nPosY = nBackPosY + OPTION_BTN_LOGOFF_Y;		
		m_pOptionLogOff->SetBtnPosition(nPosX, nPosY);
		// 2009-05-06 by bhsohn 중국 로그오프창 막음
	#if defined(LANGUAGE_CHINA) 
		#ifndef TAIWAN_RELEASE
			m_pOptionLogOff->EnableBtn(FALSE);
		#endif
	#endif
		// end 2009-05-06 by bhsohn 중국 로그오프창 막음
	}
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	{		
		nPosX = nBackPosX + OPTION_BTN_CLOSE_X;
		nPosY = nBackPosY + OPTION_BTN_CLOSE_Y;	
		
		m_pCloseBtn->SetBtnPosition(nPosX, nPosY);
	}
#endif
	{
		nPosX = nBackPosX + OPTION_BTN_GAMEEND_X;
		nPosY = nBackPosY + OPTION_BTN_GAMEEND_Y;		
		m_pOptionGameEnd->SetBtnPosition(nPosX, nPosY);
	}
	// 2008-11-13 by bhsohn 조이스틱 작업		
	{
		nPosX = nBackPosX + OPTION_JOYSTICK_BTN_X;
		nPosY = nBackPosY + OPTION_JOYSTICK_BTN_Y;		
		m_pOptionJoystic->SetBtnPosition(nPosX, nPosY);				
	}
	// end 2008-11-13 by bhsohn 조이스틱 작업		
#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	int nMainId = 0;	
	for(nMainId = 0;nMainId < MAX_OPTION_RADIO;nMainId++)
	{
		for(nCnt = 0;nCnt < MAX_OPTION_SYS_CNT;nCnt++)
		{
			nPosX = nBackPosX + OPTION_RADIO_GAME_INFO_X + (nCnt*OPTION_RADIO_GAME_INFO_CAP_X);
			nPosY = nBackPosY + OPTION_RADIO_GAME_INFO_Y+ (nMainId*OPTION_RADIO_GAME_INFO_CAP_Y);		
			m_pGameOpRadioBtn[nMainId][nCnt]->SetPosition(nPosX, nPosY, 60);		
		}
	}
#endif
	// 해상도
	{
		
		if(g_pD3dApp->m_IsFullMode)
		{
			wsprintf( m_szHaesangTxt, "%d*%d", g_pD3dApp->m_nWidth,g_pD3dApp->m_nHeight);
		}
		else
		{
			wsprintf( m_szHaesangTxt, "%d*%d%s", g_pD3dApp->m_nWidth,g_pD3dApp->m_nHeight,STRMSG_C_071221_0100);
		}
		SIZE sizeStrSize = m_pFontHaeSang->GetStringSize(m_szHaesangTxt);
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
		m_ptHaesangPos.x = nBackPosX + OPTION_HAESANG_X + ( ( OPTION_HAESANG_X_W - OPTION_HAESANG_X ) / 2 ) - ( sizeStrSize.cx / 2 ); 
#else								  
		m_ptHaesangPos.x = nBackPosX + OPTION_HAESANG_X- sizeStrSize.cx;
#endif
		m_ptHaesangPos.y = nBackPosY + OPTION_HAESANG_Y;
	}

		int nMainWidth, nMainHeight;
		int nEleWidth, nEleHeight;
		nMainWidth = OPTION_COMBO_MAIN_WIDTH;
		nMainHeight = OPTION_COMBO_MAIN_HEIGHT;
		nEleWidth = OPTION_COMBO_ELE_WIDTH;
		nEleHeight = OPTION_COMBO_ELE_HEIGHT;
		
		POINT	ptMainArena[MAX_OPTION_GRAPHIC_COMBO] =
		{
			{nBackPosX+OPTION_COMBO_SEE_RANGE_X, nBackPosY+OPTION_COMBO_SEE_RANGE_Y},
			{nBackPosX+OPTION_COMBO_UNIT_DETAIL_X, nBackPosY+OPTION_COMBO_UNIT_DETAIL_Y},
			{nBackPosX+OPTION_COMBO_SHADOW_X, nBackPosY+OPTION_COMBO_SHADOW_Y},
			{nBackPosX+OPTION_COMBO_EFFECT_X, nBackPosY+OPTION_COMBO_EFFECT_Y},
			{nBackPosX+OPTION_COMBO_GAMMA_X, nBackPosY+OPTION_COMBO_GAMMA_Y},
			{nBackPosX+OPTION_COMBO_CONTRAST_X, nBackPosY+OPTION_COMBO_CONTRAST_Y},
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
			{nBackPosX+OPTION_COMBO_FILTER_X, nBackPosY+OPTION_COMBO_FILTER_Y},
			{nBackPosX+OPTION_COMBO_SEE_CHANGE_X, nBackPosY+OPTION_COMBO_SEE_CHANGE_Y}  
#else	  
			{nBackPosX+OPTION_COMBO_FILTER_X, nBackPosY+OPTION_COMBO_FILTER_Y},
			{nBackPosX+OPTION_COMBO_SEE_CHANGE_X, nBackPosY+OPTION_COMBO_SEE_CHANGE_Y},
			{nBackPosX+OPTION_COMBO_MUSIC_VOL_X, nBackPosY+OPTION_COMBO_MUSIC_VOL_Y}
#endif
		};
		
		POINT	ptElementArena[MAX_OPTION_GRAPHIC_COMBO] =
		{
			{ptMainArena[0].x, ptMainArena[0].y + nMainHeight},
			{ptMainArena[1].x, ptMainArena[1].y + nMainHeight},
			{ptMainArena[2].x, ptMainArena[2].y + nMainHeight},
			{ptMainArena[3].x, ptMainArena[3].y + nMainHeight},
			{ptMainArena[4].x, ptMainArena[4].y + nMainHeight},
			{ptMainArena[5].x, ptMainArena[5].y + nMainHeight},
			{ptMainArena[6].x, ptMainArena[6].y + nMainHeight},
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
			{ptMainArena[7].x, ptMainArena[7].y + nMainHeight}									  
#else 
			{ptMainArena[7].x, ptMainArena[7].y + nMainHeight},
			{ptMainArena[8].x, ptMainArena[8].y + nMainHeight}
#endif
		};
		
		for(nCnt = 0;nCnt < MAX_OPTION_GRAPHIC_COMBO;nCnt++)
		{
			m_pComboGraphic[nCnt]->SetMainArea(ptMainArena[nCnt].x, 
												ptMainArena[nCnt].y,
											nMainWidth, nMainHeight);
			m_pComboGraphic[nCnt]->SetElementArea(ptElementArena[nCnt].x,
													ptElementArena[nCnt].y,
													nEleWidth, nEleHeight);
			m_pComboGraphic[nCnt]->SetBGPos(ptElementArena[nCnt].x+6,
													ptElementArena[nCnt].y,
													nEleWidth, nEleHeight);
		}
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
		nPosX = nBackPosX + OPTION_RADIO_SMALLFRM_X + nMainWidth / 2;							  
#else
		nPosX = nBackPosX + OPTION_RADIO_SMALLFRM_X;
#endif
		nPosY = nBackPosY + OPTION_RADIO_SMALLFRM_Y;
		m_pMiniFrame->SetPosition(nPosX, nPosY, 60);
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	//VEDIO
	{
		int nMainId = 0;	
		for(nMainId = 0;nMainId < MAX_VEDIO_OPTION_RADIO;nMainId++)
		{
			//NEMERIAN GUSS TODO: replace constant with preprocessor value
			if (nMainId >= 4) {
				nPosX = nBackPosX + OPTION_RADIO_VEDIO_INFO_X+50 + nMainWidth / 2;
				nPosY = nBackPosY + OPTION_RADIO_VEDIO_INFO_Y + ((nMainId-4)*OPTION_RADIO_GAME_INFO_CAP_Y);
			}
			else {
				nPosX = nBackPosX + OPTION_RADIO_VEDIO_INFO_X / 2 + nMainWidth / 2;
				nPosY = nBackPosY + OPTION_RADIO_VEDIO_INFO_Y + (nMainId*OPTION_RADIO_GAME_INFO_CAP_Y);
			}
			m_pGameVedioOpRadioBtn[nMainId]->SetPosition(nPosX, nPosY, 60);
		}
		{
			m_pFovBar->SetPosition(nBackPosX + OPTION_VIDEO_FOV_POS_X, nBackPosY + OPTION_RADIO_VEDIO_INFO_Y + (4*OPTION_RADIO_GAME_INFO_CAP_Y) + 5, OPTION_VIDEO_FOV_SIZE_X, OPTION_VIDEO_FOV_SIZE_Y);
			RECT rect =
			{
				nBackPosX + OPTION_VIDEO_FOV_POS_X,
				nBackPosY + OPTION_RADIO_VEDIO_INFO_Y + ((nMainId - 4)*OPTION_RADIO_GAME_INFO_CAP_Y) + 5,
				nBackPosX + OPTION_VIDEO_FOV_POS_X + OPTION_VIDEO_FOV_SIZE_X,
				nBackPosY + OPTION_VIDEO_FOV_POS_X + OPTION_VIDEO_FOV_SIZE_Y
			};
			m_pFovBar->SetMouseBallRect(rect);
			m_pFovBar->SetOnlyMaxItem(100, TRUE);
		}
	}
	//SOUND
	{
		int nMainId = 0;	
		for(nMainId = 0;nMainId < MAX_SOUND_OPTION_RADIO;nMainId++)
		{
			nPosX = nBackPosX + OPTION_RADIO_SOUND_INFO_X + nMainWidth / 2;
			nPosY = nBackPosY + OPTION_RADIO_SOUND_INFO_Y + (nMainId*OPTION_RADIO_GAME_INFO_CAP_Y);	
			m_pGameSoundOpRadioBtn[nMainId]->SetPosition(nPosX, nPosY, 60);
		}

		m_pMp3On->Move(nBackPosX + OPTION_RADIO_SOUND_MP3_X, nBackPosY + OPTION_RADIO_SOUND_MP3_Y);

		{
			m_pEffectVolBar->SetPosition( nBackPosX + OPTION_RADIO_SOUND_VOL_POS_X, nBackPosY + OPTION_RADIO_SOUND_VOL_POS_Y, OPTION_RADIO_VOL_SIZE_X, OPTION_RADIO_VOL_SIZE_Y );
			RECT rect = 
			{
				nBackPosX + OPTION_RADIO_SOUND_VOL_POS_X,
				nBackPosY + OPTION_RADIO_SOUND_VOL_POS_Y,
				nBackPosX + OPTION_RADIO_SOUND_VOL_POS_X + OPTION_RADIO_VOL_SIZE_X,
				nBackPosY + OPTION_RADIO_SOUND_VOL_POS_X + OPTION_RADIO_VOL_SIZE_Y
			};
			m_pEffectVolBar->SetMouseBallRect( rect );
			m_pEffectVolBar->SetOnlyMaxItem( 100, TRUE );

		}
		{
			m_pSoundVolBar->SetPosition( nBackPosX + OPTION_RADIO_EFFECT_VOL_POS_X, nBackPosY + OPTION_RADIO_EFFECT_VOL_POS_Y, OPTION_RADIO_VOL_SIZE_X, OPTION_RADIO_VOL_SIZE_Y );
			RECT rect = 
			{
				nBackPosX + OPTION_RADIO_SOUND_VOL_POS_X,
				nBackPosY + OPTION_RADIO_SOUND_VOL_POS_Y,
				nBackPosX + OPTION_RADIO_SOUND_VOL_POS_X + OPTION_RADIO_VOL_SIZE_X,
				nBackPosY + OPTION_RADIO_SOUND_VOL_POS_X + OPTION_RADIO_VOL_SIZE_Y
			};
			m_pSoundVolBar->SetMouseBallRect( rect );
			m_pSoundVolBar->SetOnlyMaxItem( 100, TRUE );
		}	

	}
	//ECT
	{
		int nMainId = 0;	
		for(nMainId = 0;nMainId < MAX_ETC_OPTION_RADIO;nMainId++)
		{
			nPosX = nBackPosX + OPTION_RADIO_ECT_INFO_X + nMainWidth / 2;
			nPosY = nBackPosY + OPTION_RADIO_ECT_SOUND_INFO_Y + (nMainId*OPTION_RADIO_GAME_INFO_CAP_Y);	
			m_pGameEctORadioBtn[nMainId]->SetPosition(nPosX, nPosY, 60);
		}		
	} 
#endif
		
	// 체크버튼
#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	{
		POINT	ptRadio[MAX_OPTION_ETC_RADIO] =
		{
			{nBackPosX+OPTION_RADIO_MOUSE_X, nBackPosY+OPTION_RADIO_MOUSE_Y},
			{nBackPosX+OPTION_RADIO_HELPER_X, nBackPosY+OPTION_RADIO_HELPER_Y},
			{nBackPosX+OPTION_RADIO_OPER_X, nBackPosY+OPTION_RADIO_OPER_Y},
			{nBackPosX+OPTION_RADIO_MAINTAIN_X, nBackPosY+OPTION_RADIO_MAINTAIN_Y},
			{nBackPosX+OPTION_RADIO_REVERSE_TB_X, nBackPosY+OPTION_RADIO_REVERSE_TB_Y},
			{nBackPosX+OPTION_RADIO_REVERSE_LR_X, nBackPosY+OPTION_RADIO_REVERSE_LR_Y},
			{nBackPosX+OPTION_RADIO_MENU_X, nBackPosY+OPTION_RADIO_MENU_Y},
			{nBackPosX+OPTION_RADIO_MP3_X, nBackPosY+OPTION_RADIO_MP3_Y}			
		};
		for(nCnt = 0;nCnt < MAX_OPTION_ETC_RADIO;nCnt++)
		{			
			m_pEtcRadio[nCnt]->SetPosition(ptRadio[nCnt].x, ptRadio[nCnt].y, 60);
		}

	}	// 닫기 버튼
	// 닫기 버튼
	{		
		nPosX = nBackPosX + OPTION_BTN_CLOSE_X;
		nPosY = nBackPosY + OPTION_BTN_CLOSE_Y;	

		m_pCloseBtn->SetBtnPosition(nPosX, nPosY);
	}
#endif
}

int CINFOptionSystem::OnLButtonDown(WPARAM wParam, LPARAM lParam)
{
	POINT pt;
	pt.x = LOWORD(lParam);
	pt.y = HIWORD(lParam);
	CheckMouseReverse(&pt);
	if(TRUE == m_pOptionApp->OnLButtonDown(pt))
	{
		// 버튼위에 마우스가 있다.
		return  INF_MSGPROC_BREAK;
	}		
	if(TRUE == m_pOptionInit->OnLButtonDown(pt))
	{
		// 버튼위에 마우스가 있다.
		return  INF_MSGPROC_BREAK;
	}		
	if(TRUE == m_pOptionLogOff->OnLButtonDown(pt))
	{
		// 버튼위에 마우스가 있다.
		return  INF_MSGPROC_BREAK;
	}		
	if(TRUE == m_pOptionGameEnd->OnLButtonDown(pt))
	{
		// 버튼위에 마우스가 있다.
		return  INF_MSGPROC_BREAK;
	}		
	
#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경

	// 2008-11-13 by bhsohn 조이스틱 작업
	if(TRUE == m_pOptionJoystic->OnLButtonDown(pt))
	{
		// 버튼위에 마우스가 있다.
		return  INF_MSGPROC_BREAK;
	}		
	// end 2008-11-13 by bhsohn 조이스틱 작업
	
	int nCnt = 0;	
	// 라디오 버튼 처리
	{		
		// 1:1전투
		int nMainId = 0;	
		BOOL bBreak = FALSE;
		for(nMainId = 0;nMainId < MAX_OPTION_RADIO;nMainId++)
		{
			if(bBreak )
			{
				break;
			}
			for(nCnt = 0;nCnt <MAX_OPTION_SYS_CNT;nCnt++)
			{
				if(bBreak )
				{
					break;
				}
				if(TRUE == m_pGameOpRadioBtn[nMainId][nCnt]->OnLButtonDown(pt))
				{
					BOOL bRadioCheck = TRUE;
					if(OPTION_SYS_OFF == nCnt)
					{
						bRadioCheck = FALSE;
					}
					SetRadioInfo(nMainId, bRadioCheck);
					bBreak = TRUE;
				}		
			}	
		}
	}	

	
	// 그래픽 관련	
	int nGraphicOptionSel = -1;
	{
		for(nCnt = 0;nCnt < MAX_OPTION_GRAPHIC_COMBO;nCnt++)
		{
			int nLBtnDown = m_pComboGraphic[nCnt]->LButtonDown(pt);
			if(-1 != nLBtnDown)				
			{
				// 안보이다가 보이는 상황
				nGraphicOptionSel = nCnt;
				break;
			}
		}
		// 선택된거를 제외한거 전체를 히든으로
		if(nGraphicOptionSel != -1)
		{
			for(nCnt = 0;nCnt < MAX_OPTION_GRAPHIC_COMBO;nCnt++)
			{
				if(nCnt != nGraphicOptionSel)
				{
					m_pComboGraphic[nCnt]->ShowItem(FALSE);					
				}
			}
			// 옵션정보 갱신
			UpdateOptionInfo();
			// 라디오 버튼 갱신
			UpdateOptionInterface(&m_struOpInfo);
			return  INF_MSGPROC_BREAK;
		}			
	}
	// 그래픽 체크버튼
	{		
		if(TRUE == m_pMiniFrame->OnLButtonDown(pt))
		{
			// 최소 프레임 버튼클릭
			OptionQualityCombo();
			return  INF_MSGPROC_BREAK;			
		}		
	}

	{
		for(nCnt = 0;nCnt < MAX_OPTION_ETC_RADIO;nCnt++)
		{	
			// 2008-11-13 by bhsohn 조이스틱 작업
			// 마우스 좌우 반전은 체크안함
			if(OPTION_ETC_MOUSE_REVERSE_LR == nCnt)
			{
				continue;
			}
			// end 2008-11-13 by bhsohn 조이스틱 작업

			if(TRUE == m_pEtcRadio[nCnt]->OnLButtonDown(pt))
			{
				OnClickEtcRadio(nCnt);
				// 유저 인터페이스 갱신
				UpdateOptionInterface(&m_struOpInfo);
				return  INF_MSGPROC_BREAK;
			}
		}
	}
#endif
	{
		if(TRUE == m_pCloseBtn->OnLButtonDown(pt))
		{
			// 버튼위에 마우스가 있다.
			return  INF_MSGPROC_BREAK;
		}		
	}
	// 라디오 버튼 갱신
	UpdateOptionInterface(&m_struOpInfo);
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	int size = m_ptBakSize.x / TOTAL_OPTION_TAB;
	if( pt.y > m_ptBakPos.y + 26 &&
	    pt.y < m_ptBakPos.y + 60 )
	{
		for( int i = 0; i < TOTAL_OPTION_TAB; i++ )
		{
			if( pt.x > m_ptBakPos.x + size * i &&
				pt.x < m_ptBakPos.x + size * (i + 1)) 
			{
				m_nCurTeb = i;
				UpdateOptionInfo();
				UpdateOptionInterface(&m_struOpInfo);
				if( i == SOUND_OPTION_TAB )
				{
					//int nVol = - ( pOptionInfo->struSOptionSetup.sSoundVolume / 100 );
					// 2012-03-13 by mspark, 게임 사운드 옵션 문제 해결
					int nVol = (SOUND_SCROLL_RETURN + m_struOpInfo.struSOptionSetup.sSoundVolume) / 100;
					// end 2012-03-13 by mspark, 게임 사운드 옵션 문제 해결
					m_pSoundVolBar->SetScrollStep( nVol, TRUE );
		
					nVol = m_struOpInfo.struSOptionSetup.sMusicVolume;
  					m_pEffectVolBar->SetScrollStep( nVol, TRUE );
				}
				break;
			}
		}
	} 
#endif

	if((m_ptBakPos.x <= pt.x && pt.x <= (m_ptBakPos.x+m_ptBakSize.x))
		&& (m_ptBakPos.y <= pt.y && pt.y <= (m_ptBakPos.y+m_ptBakSize.y)))
	{
		// 배경위에서 버튼클릭
		return  INF_MSGPROC_BREAK;
	}

	return INF_MSGPROC_NORMAL;
}

int CINFOptionSystem::OnMouseMove(WPARAM wParam, LPARAM lParam)
{
	POINT pt;
	int nCnt = 0;
	pt.x = LOWORD(lParam);
	pt.y = HIWORD(lParam);

	m_pOptionApp->OnMouseMove(pt);	
	m_pOptionInit->OnMouseMove(pt);	
	m_pOptionLogOff->OnMouseMove(pt);	
	m_pOptionGameEnd->OnMouseMove(pt);	
#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경 

	for(nCnt = 0;nCnt < MAX_OPTION_GRAPHIC_COMBO;nCnt++)
	{
		m_pComboGraphic[nCnt]->MouseMove(pt);
	}	
	// 2008-11-13 by bhsohn 조이스틱 작업
	m_pOptionJoystic->OnMouseMove(pt);	
	// end 2008-11-13 by bhsohn 조이스틱 작업	

#endif

	m_pCloseBtn->OnMouseMove(pt);	

	return INF_MSGPROC_NORMAL;

}

int CINFOptionSystem::OnLButtonUp(WPARAM wParam, LPARAM lParam)
{
	POINT pt;
	pt.x = LOWORD(lParam);
	pt.y = HIWORD(lParam);

	{
		if(TRUE == m_pOptionApp->OnLButtonUp(pt))
		{			
			// 적용버튼
			OnClickApply();
			// 버튼 클릭 			
			g_pD3dApp->m_pSound->PlayD3DSound(SOUND_SELECT_BUTTON, D3DXVECTOR3(0,0,0), FALSE);			
			return  INF_MSGPROC_BREAK;
		}
	}
	{
		if(TRUE == m_pOptionInit->OnLButtonUp(pt))
		{	
			// 로그오프
			OnSystemButtonClick(OPTIONSYS_INITSETUP_BTN);
			// 버튼 클릭 			
			g_pD3dApp->m_pSound->PlayD3DSound(SOUND_SELECT_BUTTON, D3DXVECTOR3(0,0,0), FALSE);			
			return  INF_MSGPROC_BREAK;
		}
	}
	{
		if(TRUE == m_pOptionLogOff->OnLButtonUp(pt))
		{	
			// 로그오프
			OnSystemButtonClick(OPTIONSYS_LOGOFF_BTN);
			// 버튼 클릭 			
			g_pD3dApp->m_pSound->PlayD3DSound(SOUND_SELECT_BUTTON, D3DXVECTOR3(0,0,0), FALSE);			
			return  INF_MSGPROC_BREAK;
		}
	}
	{
		if(TRUE == m_pOptionGameEnd->OnLButtonUp(pt))
		{
			// 로그오프
			OnSystemButtonClick(OPTIONSYS_GAMEEND_BTN);

			// 버튼 클릭 			
			g_pD3dApp->m_pSound->PlayD3DSound(SOUND_SELECT_BUTTON, D3DXVECTOR3(0,0,0), FALSE);			
			return  INF_MSGPROC_BREAK;
		}
	}
	// 2008-11-13 by bhsohn 조이스틱 작업
	{
		if(TRUE == m_pOptionJoystic->OnLButtonUp(pt))
		{
			OnClickJoystciOption();
			// 버튼 클릭 			
			// 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현
			g_pInterface->m_pToolTip->m_bToolTipState = FALSE;
			// end 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현
			g_pD3dApp->m_pSound->PlayD3DSound(SOUND_SELECT_BUTTON, D3DXVECTOR3(0,0,0), FALSE);			
			return  INF_MSGPROC_BREAK;
		}		
	}
	// end 2008-11-13 by bhsohn 조이스틱 작업
	{
		if(TRUE == m_pCloseBtn->OnLButtonUp(pt))
		{		
		
			// 현재 윈도우를 숨긴다.
			ShowOptionWnd(FALSE);
			// 버튼 클릭 
			// 2009. 08. 19 by jsKim 랜딩 중 메뉴 생성할 경우 커서가 변하지 않는 버그
			// 2012-12-17 by jhjang 비행중 마우스 커서가 제대로 동작하지 않는 버그 수정
			//g_INFCnt--;
			//if(g_INFCnt==0)
			if(g_pD3dApp->RemoveINFCnt() == 0)
			{
				g_pGameMain->m_bChangeMousePoint = FALSE;
			}
			// end 2012-12-17 by jhjang 비행중 마우스 커서가 제대로 동작하지 않는 버그 수정
			// end 2009. 08. 19 by jsKim 랜딩 중 메뉴 생성할 경우 커서가 변하지 않는 버그
			g_pD3dApp->m_pSound->PlayD3DSound(SOUND_CLOSE_MENU, g_pShuttleChild->m_vPos, FALSE);
			return  INF_MSGPROC_BREAK;
		}
	}
	return INF_MSGPROC_NORMAL;
}

void CINFOptionSystem::OnSystemButtonClick(int nButton)
{
	DWORD dwCurTick = timeGetTime();
	
	// 2004-12-17 by jschoi - 워프중에는 종료를 할 수 없다.
	if( g_pShuttleChild->m_nCurrentPatternNumber == PATTERN_UNIT_WARP_OUT ||
		g_pShuttleChild->m_nCurrentPatternNumber == PATTERN_UNIT_WARP_IN  ||
		g_pShuttleChild->m_nCurrentPatternNumber == PATTERN_UNIT_CITY_OUT ||
		g_pShuttleChild->m_nCurrentPatternNumber == PATTERN_UNIT_CITY_IN  )
	{
		return;
	}
	// 2006-08-02 by ispark, 개인 상점
	if(g_pInterface->m_pBazaarShop)
	{
		g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_060802_0003,COLOR_ERROR);// "개인상점 이용중에는 사용 할 수 없습니다."
		return;
	}
	if(TRUE == m_bQuitGameMessage)
	{
		// 게임종료중에 못들어오게함
		return;
	}
	
	switch(nButton)
	{
	case OPTIONSYS_LOGOFF_BTN://선택
		{
			// 2011-07-06 by shcho, hsson 아레나 게임 진입 시 캐릭터 선택 못하게 수정
			if( g_pD3dApp->m_pInterface->IsArenaGameState( ARENA_GAME_STATE_TIMER )
			 || g_pD3dApp->m_pInterface->IsArenaGameState( ARENA_GAME_STATE_SCORE ) )
			{
				g_pGameMain->m_pInfWindow->AddMsgBox(STRMSG_C_080225_0203, _MESSAGE); // "아레나에서는 이용할 수 없습니다."
				return;
			}
			// end 2011-07-06 by shcho, hsson 아레나 게임 진입 시 캐릭터 선택 못하게 수정

			// 2012-12-21 by bhsohn 베트남 게임 종료 안되는 문제 해결
			g_pD3dApp->SetClientLogOff(TRUE);
			// END 2012-12-21 by bhsohn 베트남 게임 종료 안되는 문제 해결

			m_bQuitGameMessage = TRUE;
			m_bLogOffGameMessage = TRUE;
			m_fLastTickSecTime = dwCurTick;
			g_pGameMain->RightWindowShow(FALSE,g_pGameMain->m_nRightWindowInfo);
			g_pGameMain->LeftWindowShow(FALSE,g_pGameMain->m_nLeftWindowInfo);
			g_pGameMain->m_pInfWindow->AddMsgBox(STRMSG_C_INTERFACE_0040, _Q_QUIT_MESSAGE);//"로그오프까지 5초 남았습니다."			

			// 현재 윈도우를 숨긴다.
			ShowOptionWnd(FALSE);

		}
		break;
	case OPTIONSYS_GAMEEND_BTN://종료
		{
			// 2012-12-21 by bhsohn 베트남 게임 종료 안되는 문제 해결
			g_pD3dApp->SetClientLogOff(TRUE);
			// END 2012-12-21 by bhsohn 베트남 게임 종료 안되는 문제 해결

			m_bQuitGameMessage = TRUE;
			m_fLastTickSecTime = dwCurTick;
			g_pGameMain->RightWindowShow(FALSE,g_pGameMain->m_nRightWindowInfo);
			g_pGameMain->LeftWindowShow(FALSE,g_pGameMain->m_nLeftWindowInfo);
			g_pGameMain->m_pInfWindow->AddMsgBox(STRMSG_C_INTERFACE_0041, _Q_QUIT_MESSAGE);//"게임종료까지 5초 남았습니다."

			// 현재 윈도우를 숨긴다.
			ShowOptionWnd(FALSE);
			return;
		}
		break;
	case OPTIONSYS_INITSETUP_BTN: // 초기화
		{
			m_struOpInfo = InitOptionGameInfo();
			// 라디오 버튼 갱신
			UpdateOptionInterface(&m_struOpInfo);

		}
		break;
	}
}
structOptionGameInfo CINFOptionSystem::InitOptionGameInfo()
{
	structOptionGameInfo	struTmpOpInfo;
	memset(&struTmpOpInfo, 0x00, sizeof(structOptionGameInfo));	

	g_pD3dApp->InitOptionEtcInfo(&struTmpOpInfo.struSOptionEtc);
	

	// 옵션정보
	{				
		struTmpOpInfo.struSOptionSetup.sSoundVolume = OPTION_DEFAULT_SOUNDVOLUME;
		struTmpOpInfo.struSOptionSetup.sMusicVolume = OPTION_DEFAULT_MUSICVOLUME;
		struTmpOpInfo.struSOptionSetup.sGammaCtrl = OPTION_DEFAULT_GAMMA;
		struTmpOpInfo.struSOptionSetup.sLowQuality	= OPTION_DEFAULT_LOWQUALITY;
		struTmpOpInfo.struSOptionSetup.sMouseReversLeft = OPTION_DEFAULT_REVERSE_LEFT;			// 마우스 좌우반전
		struTmpOpInfo.struSOptionSetup.sMouseReversUp = OPTION_DEFAULT_REVERSE_UP;				// 마우스 상하반전
		struTmpOpInfo.struSOptionSetup.sAutoBalance = OPTION_DEFAULT_AUTO_BALACE;				// 자동 수평조정
		struTmpOpInfo.struSOptionSetup.sMenuBalance = OPTION_DEFAULT_MENU_USE;					// 메뉴사용시 자동 수평조정
		struTmpOpInfo.struSOptionSetup.sMp3Player = OPTION_DEFAULT_MP3_USE;					// 뮤직플레이어
		struTmpOpInfo.struSOptionSetup.sHandle = TRUE ;										// 마우스 포인트 보이기
		switch(g_pD3dApp->m_bDegree)
		{
		case 2:// HIGH
			{
				struTmpOpInfo.struSOptionSetup.sTerrainEffectRender = OPTION_DEFAULT_HIGH_TERRAINEFFECTRENDER;
				struTmpOpInfo.struSOptionSetup.sTerrainRender = OPTION_DEFAULT_HIGH_TERRAINRENDER;
				struTmpOpInfo.struSOptionSetup.sShadowState = OPTION_DEFAULT_HIGH_SHOWSHADOW;
				struTmpOpInfo.struSOptionSetup.sUnitDetail = OPTION_DEFAULT_HIGH_UNITDETAIL;
				struTmpOpInfo.struSOptionSetup.sContrast = OPTION_DEFAULT_HIGH_CONTRAST;
				struTmpOpInfo.struSOptionSetup.sFilter = OPTION_DEFAULT_HIGH_FILTER;
			}
			break;
		case 1:// Middle
			{
				struTmpOpInfo.struSOptionSetup.sTerrainEffectRender = OPTION_DEFAULT_MIDDLE_TERRAINEFFECTRENDER;
				struTmpOpInfo.struSOptionSetup.sTerrainRender = OPTION_DEFAULT_MIDDLE_TERRAINRENDER;
				struTmpOpInfo.struSOptionSetup.sShadowState = OPTION_DEFAULT_MIDDLE_SHOWSHADOW;
				struTmpOpInfo.struSOptionSetup.sUnitDetail = OPTION_DEFAULT_MIDDLE_UNITDETAIL;
				struTmpOpInfo.struSOptionSetup.sContrast = OPTION_DEFAULT_MIDDLE_CONTRAST;
				struTmpOpInfo.struSOptionSetup.sFilter = OPTION_DEFAULT_MIDDLE_FILTER;
			}
			break;
		case 0:// LOW
			{
				struTmpOpInfo.struSOptionSetup.sTerrainEffectRender = OPTION_DEFAULT_LOW_TERRAINEFFECTRENDER;
				struTmpOpInfo.struSOptionSetup.sTerrainRender = OPTION_DEFAULT_LOW_TERRAINRENDER;
				struTmpOpInfo.struSOptionSetup.sShadowState = OPTION_DEFAULT_LOW_SHOWSHADOW;
				struTmpOpInfo.struSOptionSetup.sUnitDetail = OPTION_DEFAULT_LOW_UNITDETAIL;
				struTmpOpInfo.struSOptionSetup.sContrast = OPTION_DEFAULT_LOW_CONTRAST;
				struTmpOpInfo.struSOptionSetup.sFilter = OPTION_DEFAULT_LOW_FILTER;
			}
			break;
		}
		
		struTmpOpInfo.struSOptionSetup.sEyePt = FALSE;
		struTmpOpInfo.struSOptionSetup.sTarget = TRUE ;
		struTmpOpInfo.struSOptionSetup.sHeight = FALSE ;
		struTmpOpInfo.struSOptionSetup.sPKMode = 0 ;
		struTmpOpInfo.struSOptionSetup.InputType = 0 ;	
		
		// 채팅옵션 저장
		for(int i = 0; i < CHAT_MENULIST_NUMBER ; i++)
		{
			struTmpOpInfo.struSOptionSetup.m_bChatCurrentSelect[i] = TRUE;
		}
	}
	{
		struTmpOpInfo.struSOptionCharacter.sHelpDesk = TRUE;
		memset(struTmpOpInfo.struSOptionCharacter.UniqueNumber, 0x00, QSLOT_NUMBER*QSLOT_TAB_NUMBER*sizeof(UID64_t));
		memset(struTmpOpInfo.struSOptionCharacter.ItemNum, 0x00, QSLOT_NUMBER*QSLOT_TAB_NUMBER*sizeof(int));

	}
	{
		struTmpOpInfo.bOperator = TRUE;	// 오퍼레이터
		struTmpOpInfo.struSOptionSetup.sFovValue = 0;
	}

	return struTmpOpInfo;
}
void CINFOptionSystem::SetRadioInfo(int nMainId, BOOL bOnOff)
{
	m_struOpInfo.struSOptionEtc.bRadioInfo[nMainId] = bOnOff;
}

void CINFOptionSystem::UpdateOptionInterface(structOptionGameInfo* pOptionInfo)
{
#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	int nMainId = 0;
	for(nMainId = 0;nMainId < MAX_OPTION_RADIO;nMainId++)
	{
		if(pOptionInfo->struSOptionEtc.bRadioInfo[nMainId])
		{
			m_pGameOpRadioBtn[nMainId][OPTION_SYS_ON]->SetRadioBtn(TRUE);
			m_pGameOpRadioBtn[nMainId][OPTION_SYS_OFF]->SetRadioBtn(FALSE);
		}
		else
		{
			m_pGameOpRadioBtn[nMainId][OPTION_SYS_ON]->SetRadioBtn(FALSE);
			m_pGameOpRadioBtn[nMainId][OPTION_SYS_OFF]->SetRadioBtn(TRUE);
		}		
	}	
#else
	switch(m_nCurTeb)
	{
	case VEDIO_OPTION_TAB:
		{
			m_pGameVedioOpRadioBtn[VEDIO_OPTION_OPERATOR]->SetRadioBtn( pOptionInfo->bOperator );
			m_pGameVedioOpRadioBtn[VEDIO_OPTION_INTERFACE_POS]->SetRadioBtn( pOptionInfo->struSOptionEtc.bRadioInfo[ OPTION_RADIO_INTERFACE_POS ] );
			m_pGameVedioOpRadioBtn[VEDIO_OPTION_INTERFACE_HIDE]->SetRadioBtn( pOptionInfo->struSOptionEtc.bRadioInfo[ OPTION_RADIO_INTERFACE_HIDE ] );
			m_pGameVedioOpRadioBtn[VEDIO_OPTION_PET_OP_SYS]->SetRadioBtn( pOptionInfo->struSOptionSetup.sPetOPBalance );
#ifdef _WARRIOR_ADDITIONAL_INFO
			m_pGameVedioOpRadioBtn[VEDIO_OPTION_WSW_ADD_AIMINFO]->SetRadioBtn(pOptionInfo->struSOptionSetup.sWSWAddAIMInfo);
#endif
#ifdef _SHOW_GUILD_NAME
			m_pGameVedioOpRadioBtn[VEDIO_OPTION_SHOW_GUILDNAME]->SetRadioBtn(pOptionInfo->struSOptionSetup.sShowGuildName);
#endif
		}
		break;
	case SOUND_OPTION_TAB:
		{
			m_pGameSoundOpRadioBtn[ SOUND_OPTION_MUSIC ]->SetRadioBtn( pOptionInfo->struSOptionSetup.sMp3Player );			
		}
		break;
	case EX_OPTION_TAB:
		{
			m_pGameEctORadioBtn[ETC_OPTION_MAN_TO_MAN]->SetRadioBtn( pOptionInfo->struSOptionEtc.bRadioInfo[ OPTION_RADIO_MAN_TO_MAN ] );
			m_pGameEctORadioBtn[ETC_FORMATIONWAR]->SetRadioBtn( pOptionInfo->struSOptionEtc.bRadioInfo[ OPTION_RADIO_FORMATIONWAR ] );
			m_pGameEctORadioBtn[ETC_GUILD_WAR]->SetRadioBtn( pOptionInfo->struSOptionEtc.bRadioInfo[ OPTION_RADIO_GUILD_WAR ] );
			m_pGameEctORadioBtn[ETC_WHISPER]->SetRadioBtn( pOptionInfo->struSOptionEtc.bRadioInfo[ OPTION_RADIO_WHISPER ] );
			m_pGameEctORadioBtn[ETC_FORMATION_INVITE]->SetRadioBtn( pOptionInfo->struSOptionEtc.bRadioInfo[ OPTION_RADIO_FORMATION_INVITE ] );
			m_pGameEctORadioBtn[ETC_GUILD_INVITE]->SetRadioBtn( pOptionInfo->struSOptionEtc.bRadioInfo[ OPTION_RADIO_GUILD_INVITE ] );
			m_pGameEctORadioBtn[ETC_EXCHANGE]->SetRadioBtn( pOptionInfo->struSOptionEtc.bRadioInfo[ OPTION_RADIO_EXCHANGE ] );
			m_pGameEctORadioBtn[ETC_FRIEND_ID_HIDE]->SetRadioBtn( pOptionInfo->struSOptionEtc.bRadioInfo[ OPTION_RADIO_FRIEND_ID_HIDE ] );
			m_pGameEctORadioBtn[ETC_ENEMY_ID_HIDE]->SetRadioBtn( pOptionInfo->struSOptionEtc.bRadioInfo[ OPTION_RADIO_ENEMY_ID_HIDE ] );
			m_pGameEctORadioBtn[ETC_MOUSE_LOCK]->SetRadioBtn( pOptionInfo->struSOptionEtc.bRadioInfo[ OPTION_RADIO_MOUSE_LOCK ] );
			m_pGameEctORadioBtn[ETC_CHAT_FILTER]->SetRadioBtn( pOptionInfo->struSOptionEtc.bRadioInfo[ OPTION_RADIO_CHAT_FILTER ] );
			m_pGameEctORadioBtn[ETC_SHOW_MOUSE]->SetRadioBtn( pOptionInfo->struSOptionSetup.sHandle );
			m_pGameEctORadioBtn[ETC_HELP]->SetRadioBtn( pOptionInfo->struSOptionCharacter.sHelpDesk );
			m_pGameEctORadioBtn[ETC_MAINTAIN]->SetRadioBtn( pOptionInfo->struSOptionSetup.sAutoBalance );
			m_pGameEctORadioBtn[ETC_MOUSE_REVERSE_TB]->SetRadioBtn( pOptionInfo->struSOptionSetup.sMouseReversUp );
		}
		break;
	}	
#endif
	
	// 그래픽옵션
	{	
		// 시야거리
		m_pComboGraphic[OPTION_GRAPHIC_SEE_RANGE]->SetSelectItem(GetGarphicOption_To_Cursel(OPTION_GRAPHIC_SEE_RANGE, pOptionInfo->struSOptionSetup.sTerrainRender));
		// 유닛 디테일
		m_pComboGraphic[OPTION_GRAPHIC_UNIT_DETAIL]->SetSelectItem(GetGarphicOption_To_Cursel(OPTION_GRAPHIC_UNIT_DETAIL, pOptionInfo->struSOptionSetup.sUnitDetail));
		// 그림자조절
		m_pComboGraphic[OPTION_GRAPHIC_SHADOW]->SetSelectItem(GetGarphicOption_To_Cursel(OPTION_GRAPHIC_SHADOW, pOptionInfo->struSOptionSetup.sShadowState));
		// 이펙트 조절
		m_pComboGraphic[OPTION_GRAPHIC_EFFECT]->SetSelectItem(GetGarphicOption_To_Cursel(OPTION_GRAPHIC_EFFECT, pOptionInfo->struSOptionSetup.sTerrainEffectRender));

		// 감마 조절		
		m_pComboGraphic[OPTION_GRAPHIC_GAMMA]->SetSelectItem(GetGarphicOption_To_Cursel(OPTION_GRAPHIC_GAMMA, pOptionInfo->struSOptionSetup.sGammaCtrl));
		// 대비 조절
		m_pComboGraphic[OPTION_GRAPHIC_CONTRAST]->SetSelectItem(GetGarphicOption_To_Cursel(OPTION_GRAPHIC_CONTRAST, pOptionInfo->struSOptionSetup.sContrast));
		// 필터 효과
		m_pComboGraphic[OPTION_GRAPHIC_FILTER]->SetSelectItem(GetGarphicOption_To_Cursel(OPTION_GRAPHIC_FILTER, pOptionInfo->struSOptionSetup.sFilter));
		
		// 최소 프레임모드
		m_pMiniFrame->SetRadioBtn(pOptionInfo->struSOptionSetup.sLowQuality);
	}
#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	// ETc옵션
	{
		// 시전변환
		m_pComboGraphic[OPTION_ETC_SEE_CHANGE]->SetSelectItem(GetEtcOption_To_Cursel(OPTION_ETC_SEE_CHANGE, pOptionInfo->struSOptionSetup.sEyePt));

		// 시전변환
		m_pComboGraphic[OPTION_ETC_VOLUMNE]->SetSelectItem(GetEtcOption_To_Cursel(OPTION_ETC_VOLUMNE, pOptionInfo->struSOptionSetup.sSoundVolume));

		// 마우스 포인터 보이기
		m_pEtcRadio[OPTION_ETC_SHOW_MOUSE]->SetRadioBtn(pOptionInfo->struSOptionSetup.sHandle);
		// 헬프기능
		m_pEtcRadio[OPTION_ETC_HELP]->SetRadioBtn(pOptionInfo->struSOptionCharacter.sHelpDesk);
		// 오퍼레이터 기능
		m_pEtcRadio[OPTION_ETC_OPERATOR]->SetRadioBtn(pOptionInfo->bOperator);
		// 자동 수평유지
		m_pEtcRadio[OPTION_ETC_MAINTAIN]->SetRadioBtn(pOptionInfo->struSOptionSetup.sAutoBalance);
		// 상하 반전 
		m_pEtcRadio[OPTION_ETC_MOUSE_REVERSE_TB]->SetRadioBtn(pOptionInfo->struSOptionSetup.sMouseReversUp);
		// 마우스 좌우 반전
		m_pEtcRadio[OPTION_ETC_MOUSE_REVERSE_LR]->SetRadioBtn(pOptionInfo->struSOptionSetup.sMouseReversLeft);
		// 메뉴 사용시 유닛정지
		// 2010. 10. 26 by jskim 펫 오퍼레이터 옵션 처리
		//m_pEtcRadio[OPTION_ETC_MENU_UNISTOP]->SetRadioBtn(pOptionInfo->struSOptionSetup.sMenuBalance);
		m_pEtcRadio[OPTION_ETC_PET_OP_SYS]->SetRadioBtn(pOptionInfo->struSOptionSetup.sPetOPBalance); 
		// end 2010. 10. 26 by jskim 펫 오퍼레이터 옵션 처리
		// Music플레이어 보기
		m_pEtcRadio[OPTION_ETC_MUSIC]->SetRadioBtn(pOptionInfo->struSOptionSetup.sMp3Player);
	}
#endif
	
}

void CINFOptionSystem::UpdateOptionInfo()
{
	structOptionGameInfo* pOptionInfo = &m_struOpInfo;
	// 그래픽옵션
	int nCursel = 0;
	{	
		// 시야거리
		nCursel = m_pComboGraphic[OPTION_GRAPHIC_SEE_RANGE]->GetSelect();
		pOptionInfo->struSOptionSetup.sTerrainRender = GetGarphicCursel_To_Option(OPTION_GRAPHIC_SEE_RANGE, nCursel);

		// 유닛 디테일
		nCursel = m_pComboGraphic[OPTION_GRAPHIC_UNIT_DETAIL]->GetSelect();
		pOptionInfo->struSOptionSetup.sUnitDetail = GetGarphicCursel_To_Option(OPTION_GRAPHIC_UNIT_DETAIL, nCursel);
		
		// 그림자조절
		nCursel = m_pComboGraphic[OPTION_GRAPHIC_SHADOW]->GetSelect();
		pOptionInfo->struSOptionSetup.sShadowState = GetGarphicCursel_To_Option(OPTION_GRAPHIC_SHADOW, nCursel);		

		// 이펙트 조절
		nCursel = m_pComboGraphic[OPTION_GRAPHIC_EFFECT]->GetSelect();
		pOptionInfo->struSOptionSetup.sTerrainEffectRender = GetGarphicCursel_To_Option(OPTION_GRAPHIC_EFFECT, nCursel);		

		// 감마 조절		
		nCursel = m_pComboGraphic[OPTION_GRAPHIC_GAMMA]->GetSelect();
		pOptionInfo->struSOptionSetup.sGammaCtrl = GetGarphicCursel_To_Option(OPTION_GRAPHIC_GAMMA, nCursel);
		
		// 대비 조절
		nCursel = m_pComboGraphic[OPTION_GRAPHIC_CONTRAST]->GetSelect();
		pOptionInfo->struSOptionSetup.sContrast = GetGarphicCursel_To_Option(OPTION_GRAPHIC_CONTRAST, nCursel);
		
		// 필터 효과
		nCursel = m_pComboGraphic[OPTION_GRAPHIC_FILTER]->GetSelect();
		pOptionInfo->struSOptionSetup.sFilter = GetGarphicCursel_To_Option(OPTION_GRAPHIC_FILTER, nCursel);		
	}

	// ETC 관련사항
	{		
		// 시점변환
		nCursel = m_pComboGraphic[OPTION_ETC_SEE_CHANGE]->GetSelect();
		pOptionInfo->struSOptionSetup.sEyePt = GetEtcCursel_To_Option(OPTION_ETC_SEE_CHANGE, nCursel);

#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
		nCursel = m_pComboGraphic[OPTION_ETC_VOLUMNE]->GetSelect();
		pOptionInfo->struSOptionSetup.sSoundVolume = GetEtcCursel_To_Option(OPTION_ETC_VOLUMNE, nCursel);
#endif

	}
}

int CINFOptionSystem::GetGarphicOption_To_Cursel(int nMode, int nState)
{
	int nCurSel = nState;
	if(OPTION_GRAPHIC_GAMMA == nMode)
	{
		switch(nState)
		{
		case 5:
			{
				nCurSel = 0;
			}
			break;
		case 4:
			{
				nCurSel = 1;
			}
			break;
		case 3:
			{
				nCurSel = 2;
			}
			break;
		case 2:
			{
				nCurSel = 3;
			}
			break;
		case 1:
			{
				nCurSel = 4;
			}
			break;
		case 0:
			{
				nCurSel = 5;
			}
			break;
		case -1:
			{
				nCurSel = 6;
			}
			break;
		case -2:
			{
				nCurSel = 7;
			}
			break;
		case -3:
			{
				nCurSel = 8;
			}
			break;
		case -4:
			{
				nCurSel = 9;
			}
			break;
		}		
	}
	return nCurSel;
}
int CINFOptionSystem::GetGarphicCursel_To_Option(int nMode, int nCursel)
{
	int nState= nCursel;
	if(OPTION_GRAPHIC_GAMMA == nMode)
	{
		switch(nCursel)
		{
		case 0:
			{
				nState = 5;
			}
			break;
		case 1:
			{
				nState = 4;				
			}
			break;
		case 2:
			{
				nState = 3;
			}
			break;
		case 3:
			{
				nState = 2;				
			}
			break;
		case 4:
			{
				nState = 1;				
			}
			break;
		case 5:
			{
				nState = 0;				
			}
			break;
		case 6:
			{
				nState = -1;
			}
			break;
		case 7:
			{
				nState = -2;
			}
			break;
		case 8:
			{
				nState = -3;
			}
			break;
		case 9:
			{
				nState = -4;
			}
			break;
		}		
	}
	return nState;
}


int CINFOptionSystem::GetEtcOption_To_Cursel(int nMode, int nState)
{
	int nCurSel = nState;
	if(OPTION_ETC_SEE_CHANGE == nMode)
	{
		switch(nState)
		{
		case FALSE:
			{
				nCurSel = 1;
			}
			break;
		case TRUE:
			{
				nCurSel = 0;
			}
			break;
		}
	}
#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	else if(OPTION_ETC_VOLUMNE == nMode)
	{
		switch(nState)
		{
		case -10000:
			{
				nCurSel = 0;
			}
			break;
		case -3500:
			{
				nCurSel = 1;
			}
			break;
		case -3000:
			{
				nCurSel = 2;
			}
			break;
		case -2500:
			{
				nCurSel = 3;
			}
			break;
		case -2000:
			{
				nCurSel = 4;
			}
			break;
		case -1500:
			{
				nCurSel = 5;
			}
			break;
		case -1000:
			{
				nCurSel = 6;
			}
			break;
		case -600:
			{
				nCurSel = 7;
			}
			break;
		case -300:
			{
				nCurSel = 8;
			}
			break;
		case 0:
			{
				nCurSel = 9;
			}
			break;
		}		
	}
#endif
	return nCurSel;
}
int CINFOptionSystem::GetEtcCursel_To_Option(int nMode, int nCursel)
{
	int nState= nCursel;
	if(OPTION_ETC_SEE_CHANGE == nMode)
	{
		switch(nCursel)
		{
		case 1:
			{				
				nState = FALSE;
			}
			break;
		case 0:
			{				
				nState = TRUE;
			}
			break;
		}
	}
#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	else if(OPTION_ETC_VOLUMNE == nMode)
	{
		switch(nCursel)
		{
		case 0:
			{
				nState = -10000;
			}
			break;
		case 1:
			{
				nState = -3500;				
			}
			break;
		case 2:
			{
				nState = -3000;
			}
			break;
		case 3:
			{
				nState = -2500;				
			}
			break;
		case 4:
			{
				nState = -2000;				
			}
			break;
		case 5:
			{
				nState = -1500;				
			}
			break;
		case 6:
			{
				nState = -1000;
			}
			break;
		case 7:
			{
				nState = -600;
			}
			break;
		case 8:
			{
				nState = -300;
			}
			break;
		case 9:
			{
				nState = 0;
			}
			break;
		}		
	}
#endif
	return nState;
}
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
void CINFOptionSystem::OnClickRadio( int nRadioId )
{
	switch( m_nCurTeb )
	{
	case VEDIO_OPTION_TAB:
		{
			OnClickVedioRadio(nRadioId);
		}
		break;
	case SOUND_OPTION_TAB:
		{
			OnClickSoundRadio(nRadioId);
		}
		break;
	case EX_OPTION_TAB:
		{
			OnClickEtcRadio(nRadioId);
		}
		break;
	}
}

void CINFOptionSystem::OnClickVedioRadio(int nRadioId)
{
	structOptionGameInfo* pOptionInfo = &m_struOpInfo;

	switch(nRadioId)
	{
	case VEDIO_OPTION_OPERATOR:
		{
			pOptionInfo->bOperator ^= TRUE;	
		}
		break;
	case VEDIO_OPTION_INTERFACE_POS:
		{
			m_struOpInfo.struSOptionEtc.bRadioInfo[OPTION_RADIO_INTERFACE_POS] ^= TRUE;
		}
		break;
	case VEDIO_OPTION_INTERFACE_HIDE:
		{
			m_struOpInfo.struSOptionEtc.bRadioInfo[OPTION_RADIO_INTERFACE_HIDE] ^= TRUE;
		}
		break;
	case VEDIO_OPTION_PET_OP_SYS:
		{
			pOptionInfo->struSOptionSetup.sPetOPBalance ^= TRUE;
		}
		break;
#ifdef _WARRIOR_ADDITIONAL_INFO
		case VEDIO_OPTION_WSW_ADD_AIMINFO:
		{
			pOptionInfo->struSOptionSetup.sWSWAddAIMInfo ^= TRUE;
		}
		break;
#endif
#ifdef _SHOW_GUILD_NAME
		case VEDIO_OPTION_SHOW_GUILDNAME:
		{
			pOptionInfo->struSOptionSetup.sShowGuildName ^= TRUE;
		}
		break;
#endif
	}	
}

void CINFOptionSystem::OnClickSoundRadio(int nRadioId)
{
	structOptionGameInfo* pOptionInfo = &m_struOpInfo;
	switch(nRadioId)
	{		
	case SOUND_OPTION_MUSIC:
		{
			pOptionInfo->struSOptionSetup.sMp3Player ^= TRUE;
// 2012-10-15 by jhahn 배경 음악 mp3플레이 버그 수정
			g_pGameMain->m_bMp3PlayFlag = pOptionInfo->struSOptionSetup.sMp3Player;
			g_pSOption->sMp3Player = g_pGameMain->m_bMp3PlayFlag;
//end 2012-10-15 by jhahn 배경 음악 mp3플레이 버그 수정
		}
		break;
	}
}
#endif
void CINFOptionSystem::OnClickEtcRadio(int nRadioId)
{
	structOptionGameInfo* pOptionInfo = &m_struOpInfo;
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	switch(nRadioId)
	{
	case ETC_OPTION_MAN_TO_MAN:
		{
			m_struOpInfo.struSOptionEtc.bRadioInfo[ OPTION_RADIO_MAN_TO_MAN ] ^= TRUE;
		}
		break;
	case ETC_FORMATIONWAR:
		{
			m_struOpInfo.struSOptionEtc.bRadioInfo[ OPTION_RADIO_FORMATIONWAR ] ^= TRUE;
		}
		break;
	case ETC_GUILD_WAR:
		{
			m_struOpInfo.struSOptionEtc.bRadioInfo[ OPTION_RADIO_GUILD_WAR ] ^= TRUE;
		}
		break;
	case ETC_WHISPER:
		{
			m_struOpInfo.struSOptionEtc.bRadioInfo[ OPTION_RADIO_WHISPER ] ^= TRUE;	
		}
		break;
	case ETC_FORMATION_INVITE:
		{
			m_struOpInfo.struSOptionEtc.bRadioInfo[ OPTION_RADIO_FORMATION_INVITE ] ^= TRUE;	
		}
		break;
	case ETC_GUILD_INVITE:
		{
			m_struOpInfo.struSOptionEtc.bRadioInfo[ OPTION_RADIO_GUILD_INVITE ] ^= TRUE;	
		}
		break;
	case ETC_EXCHANGE:
		{
			m_struOpInfo.struSOptionEtc.bRadioInfo[ OPTION_RADIO_EXCHANGE ] ^= TRUE;	
		}
		break;
	case ETC_FRIEND_ID_HIDE:
		{
			m_struOpInfo.struSOptionEtc.bRadioInfo[ OPTION_RADIO_FRIEND_ID_HIDE ] ^= TRUE;	
		}
		break;
	case ETC_ENEMY_ID_HIDE:
		{
			m_struOpInfo.struSOptionEtc.bRadioInfo[ OPTION_RADIO_ENEMY_ID_HIDE ] ^= TRUE;	
		}
		break;
	case ETC_MOUSE_LOCK:
		{
			m_struOpInfo.struSOptionEtc.bRadioInfo[ OPTION_RADIO_MOUSE_LOCK ] ^= TRUE;	
		}
		break;
	case ETC_CHAT_FILTER:
		{
			m_struOpInfo.struSOptionEtc.bRadioInfo[ OPTION_RADIO_CHAT_FILTER ] ^= TRUE;	
		}
		break;
	case ETC_SHOW_MOUSE:
		{
			pOptionInfo->struSOptionSetup.sHandle ^= TRUE;
		}
		break;
	case ETC_HELP:
		{
			pOptionInfo->struSOptionCharacter.sHelpDesk ^= TRUE;			
		}
		break;
	case ETC_MAINTAIN:
		{
			pOptionInfo->struSOptionSetup.sAutoBalance ^= TRUE;	
		}
		break;
	case ETC_MOUSE_REVERSE_TB:
		{
			pOptionInfo->struSOptionSetup.sMouseReversUp ^= TRUE;
		}
		break;
	}
#else
	switch(nRadioId)
	{		
	case OPTION_ETC_SHOW_MOUSE:// 마우스 포인터 보이기
		{
			if(IS_DT(g_pShuttleChild->m_myShuttleInfo.UnitKind) == FALSE )
			{
				pOptionInfo->struSOptionSetup.sHandle ^= TRUE;
			}
			else
			{
				pOptionInfo->struSOptionSetup.sHandle = TRUE;
			}
		}
		break;
	case OPTION_ETC_HELP:// 헬프기능
		{
			pOptionInfo->struSOptionCharacter.sHelpDesk ^= TRUE;
		}
		break;
	case OPTION_ETC_OPERATOR:// 오퍼레이터 기능
		{			
			pOptionInfo->bOperator ^= TRUE;			
		}
		break;
	case OPTION_ETC_MAINTAIN:// 수평유지
		{
			pOptionInfo->struSOptionSetup.sAutoBalance ^= TRUE;
		}
		break;
	case OPTION_ETC_MOUSE_REVERSE_TB:// 마우스 상하 반전
		{
			pOptionInfo->struSOptionSetup.sMouseReversUp ^= TRUE;
		}
		break;
	case OPTION_ETC_MOUSE_REVERSE_LR:// 마우스 좌우 반전
		{
			// 2008-11-13 by bhsohn 조이스틱 작업
			// 마우스 좌우 반전없어짐
			//pOptionInfo->struSOptionSetup.sMouseReversLeft ^= TRUE;
			// end 2008-11-13 by bhsohn 조이스틱 작업
		}
		break;
	// 2010. 10. 26 by jskim 펫 오퍼레이터 옵션 처리
	//case OPTION_ETC_MENU_UNISTOP:// 메뉴 사용시 유닛정지
	case OPTION_ETC_PET_OP_SYS:// 메뉴 사용시 유닛정지
	// end 2010. 10. 26 by jskim 펫 오퍼레이터 옵션 처리
		{
			// 2010. 10. 26 by jskim 펫 오퍼레이터 옵션 처리
			//pOptionInfo->struSOptionSetup.sMenuBalance ^= TRUE;
			pOptionInfo->struSOptionSetup.sPetOPBalance ^= TRUE;
			// end 2010. 10. 26 by jskim 펫 오퍼레이터 옵션 처리
		}
		break;
	case OPTION_ETC_MUSIC:// Music플레이어 보기
		{
			pOptionInfo->struSOptionSetup.sMp3Player ^= TRUE;
		}
		break;	
	}
#endif
}

// 적용버튼
void CINFOptionSystem::OnClickApply()
{
	// 인터페이스 위치
	{
		m_struOpInfo.struSOptionEtc.stRateRect = g_pGameMain->m_pChat->m_pSystemMsgW->GetSysMsgWndRect();
	}

	// 2009. 02. 03 by ckPark 채팅창 인터페이스 크기 저장
	m_struOpInfo.struSOptionEtc.stChatRect	= g_pGameMain->m_pChat->GetChatWndRect();
	// end 2009. 02. 03 by ckPark 채팅창 인터페이스 크기 저장

	// 시야거리변경
	BOOL bSetFogLevel = FALSE;
	if(m_pSOptionOld.sTerrainRender != m_struOpInfo.struSOptionSetup.sTerrainRender)
	{
		bSetFogLevel = TRUE;		
	}	
	// 감마값변경
	if(m_pSOptionOld.sGammaCtrl != m_struOpInfo.struSOptionSetup.sGammaCtrl)
	{
		UpdateGamma(m_struOpInfo.struSOptionSetup.sGammaCtrl);
	}
	// 최소 프레임 틀림
	if(m_pSOptionOld.sLowQuality != m_struOpInfo.struSOptionSetup.sLowQuality)
	{
		bSetFogLevel = TRUE;
		// 최소 프레임 모드를 키면 리소스를 리로드한다. 	
		g_pD3dApp->ReLoadEnemyRenderList();		
	}	

	// 시점 변환
	if(m_pSOptionOld.sEyePt != m_struOpInfo.struSOptionSetup.sEyePt)	
	{
		if(m_struOpInfo.struSOptionSetup.sEyePt)
		{
			if(g_pShuttleChild->m_dwState == _LANDED || g_pShuttleChild->m_bIsAir == FALSE)
			{
				g_pShuttleChild->m_bLandingOldCamtypeFPS = TRUE;
			}
			else
			{
				g_pD3dApp->m_pCamera->SetCamType( MODE_CAMERA_TYPE_FPS );
			}			
		}
		else
		{
			if(g_pShuttleChild->m_bLandingOldCamtypeFPS != TRUE)
			{
				g_pD3dApp->m_pCamera->SetCamType( MODE_CAMERA_TYPE_NORMAL );
			}
		}		
	}

	// Mp3플레이 변경
	if(m_pSOptionOld.sMp3Player != m_struOpInfo.struSOptionSetup.sMp3Player)	
	{
		g_pGameMain->m_bMp3PlayFlag = m_struOpInfo.struSOptionSetup.sMp3Player;
	}

	// 2012-03-13 mspark, 게임 사운드 옵션 문제 해결 - 추가
	m_nSoundVolumeTemp = m_struOpInfo.struSOptionSetup.sSoundVolume;
	m_nMusicVolumeTemp = m_struOpInfo.struSOptionSetup.sMusicVolume;
	m_nFOVTemp = m_struOpInfo.struSOptionSetup.sFovValue;
	// end 2012-03-13 mspark, 게임 사운드 옵션 문제 해결 - 추가
	
	// 인터페이스 모두 감추기 버튼
	{
//		BOOL bShowInter = m_struOpInfo.struSOptionEtc.bRadioInfo[OPTION_RADIO_INTERFACE_HIDE];
//		g_pInterface->SetScreenShotMode(bShowInter);
		// 인터페이스 숨기기 보이기
		BOOL bShowInter = !m_struOpInfo.struSOptionEtc.bRadioInfo[OPTION_RADIO_INTERFACE_HIDE];
		g_pInterface->SetShowInterface(bShowInter);
	}	

	if(bSetFogLevel)
	{		
		SetFogLevel( g_pShuttleChild->m_myShuttleInfo.MapChannelIndex.MapIndex, TRUE );
	}

	memcpy(g_pSOption, &m_struOpInfo.struSOptionSetup, sizeof(sOPTION_SYSTEM));		
	memcpy(g_pSOptionCharacter, &m_struOpInfo.struSOptionCharacter, sizeof(sOPTION_CHARACTER));
	// 옵션 ETC메뉴
	memcpy(g_pSOptionEtc, &m_struOpInfo.struSOptionEtc, sizeof(sOPTION_ETC));

	// 2009-02-02 by bhsohn 셋업정보 제대로 안되는 버그 수정
	memcpy(&m_pSOptionOld, g_pSOption, sizeof(sOPTION_SYSTEM));
	// end 2009-02-02 by bhsohn 셋업정보 제대로 안되는 버그 수정

	{
		// 오퍼레이터 변수
		g_pInterface->SetOperatorMode(m_struOpInfo.bOperator);
	}

	// 2008-11-06 by bhsohn 마우스 가두기 모드 보완
	g_pD3dApp->RefreshMouseMode();
	// end 2008-11-06 by bhsohn 마우스 가두기 모드 보완
	

	// 저장
	g_pInterface->SaveOptionFile();
}

void CINFOptionSystem::UpdateGamma(int nGama)
{
	FLOG( "CINFSystem::UpdateGamma(int nGama)" );
    D3DGAMMARAMP	D3dGammaRamp;
    WORD			dwGamma = 0;

	if(nGama == 5)
		nGama = 256;
	else if(nGama == 4)
		nGama = 256;
	else if(nGama == 3)
		nGama = 256;
	else if(nGama == 2)
		nGama = 256;
	else if(nGama == 1)
		nGama = 256;
	else if(nGama == 0)
		nGama = 256;
	else if(nGama == -1)
		nGama = 216;
	else if(nGama == -2)
		nGama = 192;
	else if(nGama == -3)
		nGama = 176;
	else
		nGama = 168;

    ZeroMemory( &D3dGammaRamp, sizeof(D3dGammaRamp) );
	// 2005-01-04 by jschoi
//	g_pD3dDev->GetGammaRamp(&D3dGammaRamp ) ;
	g_pD3dDev->GetGammaRamp(0, &D3dGammaRamp ) ;

    for( int iColor = 0; iColor < 256; iColor++ )
    {
        D3dGammaRamp.red[iColor]   = dwGamma;
        D3dGammaRamp.green[iColor] = dwGamma;
        D3dGammaRamp.blue[iColor]  = dwGamma;

        dwGamma += (WORD)nGama ;
    }

	// 2005-01-04 by jschoi
//	g_pD3dDev->SetGammaRamp( D3DSGR_NO_CALIBRATION  , &D3dGammaRamp ) ;
	g_pD3dDev->SetGammaRamp(0, D3DSGR_NO_CALIBRATION  , &D3dGammaRamp ) ;
}

structOptionGameInfo* CINFOptionSystem::GetSystemOption()
{
	return &m_struOpInfo;
}

void CINFOptionSystem::RefreshShowInterface(BOOL bShowInterface)
{
	if(!IsOptionShow())
	{
		return;
	}
	m_struOpInfo.struSOptionEtc.bRadioInfo[OPTION_RADIO_INTERFACE_HIDE] = bShowInterface;	
	// 라디오 버튼 갱신
	UpdateOptionInterface(&m_struOpInfo);
}



// 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템
void	CINFOptionSystem::EnableLogOffBtn( BOOL bEnable )
{
	// 인피 던젼상에서는 로그오프 막고 메인필드서버로 갈때 다시 복수
	m_pOptionLogOff->EnableBtn( bEnable );

	// 2009-05-06 by bhsohn 중국 로그오프창 막음
	#if defined(LANGUAGE_CHINA) 
		#ifndef TAIWAN_RELEASE
			m_pOptionLogOff->EnableBtn(FALSE);
		#endif
	#endif
	// end 2009-05-06 by bhsohn 중국 로그오프창 막음
}
// end 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템




// 2008-11-13 by bhsohn 조이스틱 작업
void CINFOptionSystem::OnClickJoystciOption()
{
	BOOL bShow = g_pGameMain->IsShowOpJoystick();
	bShow ^= TRUE;
	g_pGameMain->ShowOpJoystick(bShow);
}
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
void CINFOptionSystem::VideoRender()
{
	m_pOptionSubBk[VEDIO_OPTION_TAB]->Move(m_ptBakPos.x, m_ptBakPos.y);
	m_pOptionSubBk[VEDIO_OPTION_TAB]->Render();

	// 버튼 
	{

		// 2008-11-13 by bhsohn 조이스틱 작업
		//m_pOptionJoystic->Render();	// 조이스틱옵션
		// end 2008-11-13 by bhsohn 조이스틱 작업
	}
	// 그래픽
	{
		// 해상도
		m_pFontHaeSang->DrawText(m_ptHaesangPos.x,m_ptHaesangPos.y,
			GUI_FONT_COLOR_W,
			m_szHaesangTxt);
	}
	
 	int nCnt =0;
	m_pMiniFrame->Render();
	
	// 콤보박스는 가장 마지막에 랜더링 	
	{
		int nSelCombo = -1;
		for(nCnt = 0;nCnt < MAX_OPTION_GRAPHIC_COMBO;nCnt++)
		{
			if(m_pComboGraphic[nCnt]->IsShowItem())
			{
				nSelCombo = nCnt;
				break;
			}
		}
		for(nCnt = 0;nCnt < MAX_OPTION_GRAPHIC_COMBO;nCnt++)
		{	
			if(nCnt != nSelCombo)
			{
				m_pComboGraphic[nCnt]->Render();
			}
		}
		if(nSelCombo != -1)
		{
			m_pComboGraphic[nSelCombo]->Render();
		}
	}
	
	for(nCnt = 0;nCnt < MAX_VEDIO_OPTION_RADIO;nCnt++)
	{
		m_pGameVedioOpRadioBtn[nCnt]->Render();
	}
	m_pFovBar->Render();
}

void CINFOptionSystem::SoundRender()
{
	structOptionGameInfo* pOptionInfo = &m_struOpInfo;
	m_pOptionSubBk[SOUND_OPTION_TAB]->Move(m_ptBakPos.x, m_ptBakPos.y);
	m_pOptionSubBk[SOUND_OPTION_TAB]->Render();
	
	m_pSoundVolBar->Render();

	char buff[MAX_PATH];
	sprintf( buff, "%d", m_pSoundVolBar->GetScrollStep() );
	
	SIZE szSizeTmp = m_pVol->GetStringSize( buff );
	m_pVol->DrawText( m_ptBakPos.x + OPTION_RADIO_SOUND_VOL_POS_X + 180 - szSizeTmp.cx, m_ptBakPos.y + OPTION_RADIO_EFFECT_VOL_POS_Y - 3, GUI_FONT_COLOR, buff);
	
	m_pEffectVolBar->Render();
	sprintf( buff, "%d", m_pEffectVolBar->GetScrollStep() );
	szSizeTmp = m_pVol->GetStringSize( buff );
	m_pVol->DrawText( m_ptBakPos.x + OPTION_RADIO_SOUND_VOL_POS_X + 180 - szSizeTmp.cx, m_ptBakPos.y + OPTION_RADIO_SOUND_VOL_POS_Y - 3, GUI_FONT_COLOR, buff, 0L);

	int nCnt = 0;
	for(nCnt = 0;nCnt < MAX_SOUND_OPTION_RADIO;nCnt++)
	{
		m_pGameSoundOpRadioBtn[nCnt]->Render();
	}

	if( pOptionInfo->struSOptionSetup.sMp3Player != OPTION_DEFAULT_MP3_USE )
	{
		if( m_pMp3On )
		{
			m_pMp3On->Render();
			if( m_bmp3Playview == TRUE )
			{
				m_bmp3Playview = FALSE;
				g_pD3dApp->m_pSound->m_bPlayMusic = FALSE;
				g_pD3dApp->m_pSound->PlayBackSound();
				g_pGameMain->m_pMp3Player->SetStopButton(TRUE);
				g_pGameMain->m_pMp3Player->SetPlayButton(FALSE);

			}
		}	
	}	
	else
	{
		m_bmp3Playview = TRUE;
		g_pGameMain->m_pMp3Player->Render(m_ptBakPos.x, m_ptBakPos.y);
	}
}

void CINFOptionSystem::ExRender()
{
	m_pOptionSubBk[EX_OPTION_TAB]->Move(m_ptBakPos.x, m_ptBakPos.y);
	m_pOptionSubBk[EX_OPTION_TAB]->Render();

	int nCnt = 0;
	for(nCnt = 0;nCnt < MAX_ETC_OPTION_RADIO;nCnt++)
	{
		m_pGameEctORadioBtn[nCnt]->Render();
	}
	// 2008-11-13 by bhsohn 조이스틱 작업
	m_pOptionJoystic->Render();	// 조이스틱옵션
	// end 2008-11-13 by bhsohn 조이스틱 작업
}

void CINFOptionSystem::VideoTick()
{

}

void CINFOptionSystem::SoundTick()
{
	g_pGameMain->m_pMp3Player->Tick();
}

void CINFOptionSystem::ExTick()
{
	
}

int CINFOptionSystem::OnVedioLButtonDown(WPARAM wParam, LPARAM lParam)
{
	POINT pt;
	pt.x = LOWORD(lParam);
	pt.y = HIWORD(lParam);
	CheckMouseReverse(&pt);
	// 2008-11-13 by bhsohn 조이스틱 작업
	// 그래픽 관련	
	int nGraphicOptionSel = -1;
	int nCnt = 0;
	{
		for(nCnt = 0;nCnt < MAX_OPTION_GRAPHIC_COMBO;nCnt++)
		{
			int nLBtnDown = m_pComboGraphic[nCnt]->LButtonDown(pt);
			if(-1 != nLBtnDown)				
			{
				// 안보이다가 보이는 상황
				nGraphicOptionSel = nCnt;
				break;
			}
		}
		// 선택된거를 제외한거 전체를 히든으로
		if(nGraphicOptionSel != -1)
		{
			for(nCnt = 0;nCnt < MAX_OPTION_GRAPHIC_COMBO;nCnt++)
			{
				if(nCnt != nGraphicOptionSel)
				{
					m_pComboGraphic[nCnt]->ShowItem(FALSE);					
				}
			}
			// 옵션정보 갱신
			UpdateOptionInfo();
			// 라디오 버튼 갱신
			UpdateOptionInterface(&m_struOpInfo);
			return  INF_MSGPROC_BREAK;
		}			
	}
	{		
		if(TRUE == m_pMiniFrame->OnLButtonDown(pt))
		{
			// 최소 프레임 버튼클릭
			OptionQualityCombo();
			return  INF_MSGPROC_BREAK;			
		}
	}
	{
		if (m_pFovBar->IsMouseBallPos(pt))
		{
			m_pFovBar->SetMouseMoveMode(TRUE);
			return INF_MSGPROC_BREAK;
		}
	}

	for(nCnt = 0;nCnt < MAX_VEDIO_OPTION_RADIO;nCnt++)
	{	
		if(TRUE == m_pGameVedioOpRadioBtn[nCnt]->OnLButtonDown(pt))
		{
			OnClickRadio(nCnt);
			UpdateOptionInterface(&m_struOpInfo);
			return  INF_MSGPROC_BREAK;
		}
	}
	return INF_MSGPROC_NORMAL;
}
int CINFOptionSystem::OnVedioMouseMove(WPARAM wParam, LPARAM lParam)
{
	POINT pt;
	pt.x = LOWORD(lParam);
	pt.y = HIWORD(lParam);
	CheckMouseReverse(&pt);
	int nCnt = 0;
	for(nCnt = 0;nCnt < MAX_OPTION_GRAPHIC_COMBO;nCnt++)
	{
		m_pComboGraphic[nCnt]->MouseMove(pt);
	}
	if (m_pFovBar->GetMouseMoveMode())
	{
		m_pFovBar->SetScrollPos(pt, TRUE);
		return INF_MSGPROC_BREAK;
	}
	return INF_MSGPROC_NORMAL;
}

int CINFOptionSystem::OnVedioLButtonUp(WPARAM wParam, LPARAM lParam)
{
	if (m_pFovBar->GetMouseMoveMode())
	{
		m_pFovBar->SetMouseMoveMode(FALSE);
		if (m_struOpInfo.struSOptionSetup.sFovValue != m_pFovBar->GetScrollStep())
		{
			m_struOpInfo.struSOptionSetup.sFovValue = m_pFovBar->GetScrollStep();
			float nNewFov = m_pFovBar->GetScrollStep() / 100.0;
			// 2013-02-05 by bhsohn Sound 없는 장비에서 Exception오류 나는 현상 수정			
			if (g_pD3dApp->m_pCamera)
			{
				FLOAT fAspect = ((FLOAT)g_pD3dApp->m_d3dsdBackBuffer.Width) / g_pD3dApp->m_d3dsdBackBuffer.Height;
				float fovModifier = 3 - nNewFov;

				char message[256];
				sprintf(message, "option change fov value: %f(3-%f)(%d)", fovModifier, nNewFov, m_pFovBar->GetScrollStep());
				g_pD3dApp->m_pChat->CreateChatChild(message, COLOR_TRADE);

				g_pD3dApp->m_pCamera->SetProjParams(D3DX_PI / fovModifier, fAspect, 1.0f, 100000.0f);
			}
		}
		return INF_MSGPROC_BREAK;
	}
	return INF_MSGPROC_NORMAL;
}
int CINFOptionSystem::OnSoundLButtonDown(WPARAM wParam, LPARAM lParam)
{
	POINT pt;
	pt.x = LOWORD(lParam);
	pt.y = HIWORD(lParam);
	CheckMouseReverse(&pt);
	{
		int nCnt = 0;
		for(nCnt = 0;nCnt < MAX_SOUND_OPTION_RADIO;nCnt++)
		{	
			if(TRUE == m_pGameSoundOpRadioBtn[nCnt]->OnLButtonDown(pt))
			{
				OnClickRadio(nCnt);
				UpdateOptionInterface(&m_struOpInfo);
				return  INF_MSGPROC_BREAK;
			}
		}
	}
	if( m_pSoundVolBar->IsMouseBallPos( pt ) )
	{
		m_pSoundVolBar->SetMouseMoveMode( TRUE );		
		return INF_MSGPROC_BREAK;
	}

	if( m_pEffectVolBar->IsMouseBallPos( pt ) )
	{
		m_pEffectVolBar->SetMouseMoveMode( TRUE );		
		return INF_MSGPROC_BREAK;
	}
	return INF_MSGPROC_NORMAL;
}
int CINFOptionSystem::OnSoundMouseMove(WPARAM wParam, LPARAM lParam)
{
	POINT pt;
	pt.x = LOWORD(lParam);
	pt.y = HIWORD(lParam);
	CheckMouseReverse(&pt);
	structOptionGameInfo* pOptionInfo = &m_struOpInfo;
	if( m_pSoundVolBar->GetMouseMoveMode() )
	{
		m_pSoundVolBar->SetScrollPos( pt, TRUE);
		return INF_MSGPROC_BREAK;
	}
	if( m_pEffectVolBar->GetMouseMoveMode() )
	{
		m_pEffectVolBar->SetScrollPos( pt, TRUE);
		return INF_MSGPROC_BREAK;
	}
	return INF_MSGPROC_NORMAL;
}
int CINFOptionSystem::OnSoundLButtonUp(WPARAM wParam, LPARAM lParam)
{
	POINT pt;
	pt.x = LOWORD(lParam);
	pt.y = HIWORD(lParam);
	CheckMouseReverse(&pt);
	structOptionGameInfo* pOptionInfo = &m_struOpInfo;
	if( m_pSoundVolBar->GetMouseMoveMode() )
	{
		m_pSoundVolBar->SetMouseMoveMode( FALSE );
		pOptionInfo->struSOptionSetup.sSoundVolume = - ( 10000 - ( m_pSoundVolBar->GetScrollStep() * 100 ) );
		return INF_MSGPROC_BREAK;
	}

	if( m_pEffectVolBar->GetMouseMoveMode() )
	{
		m_pEffectVolBar->SetMouseMoveMode( FALSE );
		if( pOptionInfo->struSOptionSetup.sMusicVolume != m_pEffectVolBar->GetScrollStep() )
		{
			pOptionInfo->struSOptionSetup.sMusicVolume = m_pEffectVolBar->GetScrollStep();
			int nNewVol = -10000;
			if(m_pEffectVolBar->GetScrollStep() > 0)
			{
				float fPercent = (float)m_pEffectVolBar->GetScrollStep()*0.01f;
				float fLog     = (float)log(fPercent);
				nNewVol = (int)(fLog*1000.f);
			}
			// 2013-02-05 by bhsohn Sound 없는 장비에서 Exception오류 나는 현상 수정			
			if(g_pD3dApp->m_pSound && g_pD3dApp->m_pSound->IsSoundDevice())
			{
				g_pD3dApp->m_pSound->GetMusicMp3()->SetAtumMusicVolume( nNewVol );
			}
		}
		return INF_MSGPROC_BREAK;
	}
	return INF_MSGPROC_NORMAL;
}

int CINFOptionSystem::OnExLButtonDown(WPARAM wParam, LPARAM lParam)
{
	POINT pt;
	pt.x = LOWORD(lParam);
	pt.y = HIWORD(lParam);
	CheckMouseReverse(&pt);
	// 2008-11-13 by bhsohn 조이스틱 작업
	if(TRUE == m_pOptionJoystic->OnLButtonDown(pt))
	{
		// 버튼위에 마우스가 있다.
		return  INF_MSGPROC_BREAK;
	}		
	// end 2008-11-13 by bhsohn 조이스틱 작업
	
	int nCnt = 0;	
	// 라디오 버튼 처리
// 	{		
// 		// 1:1전투
// 		int nMainId = 0;	
// 		BOOL bBreak = FALSE;
// 		for(nMainId = 0;nMainId < MAX_OPTION_RADIO;nMainId++)
// 		{
// 			if(bBreak )
// 			{
// 				break;
// 			}
// 			for(nCnt = 0;nCnt <MAX_OPTION_SYS_CNT;nCnt++)
// 			{
// 				if(bBreak )
// 				{
// 					break;
// 				}
// 				if(TRUE == m_pGameOpRadioBtn[nMainId][nCnt]->OnLButtonDown(pt))
// 				{
// 					BOOL bRadioCheck = TRUE;
// 					if(OPTION_SYS_OFF == nCnt)
// 					{
// 						bRadioCheck = FALSE;
// 					}
// 					SetRadioInfo(nMainId, bRadioCheck);
// 					bBreak = TRUE;
// 				}
// 			}	
// 		}
// 	}	
	{
		for(nCnt = 0;nCnt < MAX_ETC_OPTION_RADIO;nCnt++)
		{	
			if(TRUE == m_pGameEctORadioBtn[nCnt]->OnLButtonDown(pt))
			{
				OnClickRadio(nCnt);
				// 유저 인터페이스 갱신
				UpdateOptionInterface(&m_struOpInfo);
				return  INF_MSGPROC_BREAK;
			}
		}
	}
	return INF_MSGPROC_NORMAL;
} 
int CINFOptionSystem::OnExMouseMove(WPARAM wParam, LPARAM lParam)
{
	POINT pt;
	pt.x = LOWORD(lParam);
	pt.y = HIWORD(lParam);
	CheckMouseReverse(&pt);
	// 2008-11-13 by bhsohn 조이스틱 작업
	m_pOptionJoystic->OnMouseMove(pt);	
	// end 2008-11-13 by bhsohn 조이스틱 작업
	return INF_MSGPROC_NORMAL;
}
int CINFOptionSystem::OnExLButtonUp(WPARAM wParam, LPARAM lParam)
{
	return INF_MSGPROC_NORMAL;
}
#endif
