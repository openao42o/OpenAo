// INFSystem.h: interface for the CINFSystem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFSYSTEM_H__D1ABB527_44B7_4158_96DE_4D806B53B315__INCLUDED_)
#define AFX_INFSYSTEM_H__D1ABB527_44B7_4158_96DE_4D806B53B315__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INFBase.h"
#include "INFGameMainQSlot.h"
#include "INFGameMainChat.h"

#define SYSTEM_SYSTEM 0
#define SYSTEM_OPTION 1 
#define SYSTEM_MODE		2

#define SIZE_SYSTEM_BUTTON_X 71
#define SIZE_SYSTEM_BUTTON_Y 18

#define OPTION_MODE_COMBO		0	// 해상도
#define OPTION_TERRAIN_COMBO	1	// 지형시야
#define OPTION_UNITDET_COMBO	2	// 유닛디테일
#define OPTION_SHADOW_COMBO		3	// 그림자 시야
#define OPTION_OBJ_COMBO		4	// 이펙트
#define OPTION_GAMMA_COMBO		5	// 감마
#define OPTION_CONTRAST			6	// 대비
#define OPTION_FILTER			7	// 필터
#define OPTION_QUALITY_COMBO	8	// 최소 프레임

#define OPTION_COMBO_NONE		9

#define OPTION_COMBO_NUMBER		9

#define OPTION_CONTRAST_0		"0"
#define OPTION_CONTRAST_1		"1"
#define OPTION_CONTRAST_2 		"2"
#define OPTION_CONTRAST_3 		"3"
#define OPTION_CONTRAST_4 		"4"
#define OPTION_CONTRAST_5		"5"
#define OPTION_CONTRAST_6 		"6"
#define OPTION_CONTRAST_7 		"7"
#define OPTION_CONTRAST_8 		"8"
#define OPTION_CONTRAST_9 		"9"

#define OPTION_FILTER_0			"0"
#define OPTION_FILTER_1			"1"
#define OPTION_FILTER_2 		"2"
#define OPTION_FILTER_3 		"3"
#define OPTION_FILTER_4 		"4"
#define OPTION_FILTER_5 		"5"
#define OPTION_FILTER_6 		"6"
#define OPTION_FILTER_7 		"7"
#define OPTION_FILTER_8 		"8"
#define OPTION_FILTER_9			"9"

#define OPTION_SHADOW_0			"0"
#define OPTION_SHADOW_1			"1"
#define OPTION_SHADOW_2 		"2"
#define OPTION_SHADOW_3 		"3"
#define OPTION_SHADOW_4 		"4"
#define OPTION_SHADOW_5 		"5"
#define OPTION_SHADOW_6 		"6"
#define OPTION_SHADOW_7 		"7"
#define OPTION_SHADOW_8 		"8"
#define OPTION_SHADOW_9 		"9"

#define OPTION_MODE_0		"640*480*16"
#define OPTION_MODE_1		"640*480*32"
#define OPTION_MODE_2		"800*600*16"
#define OPTION_MODE_3		"800*600*32"
#define OPTION_MODE_4		"1024*768*16"
#define OPTION_MODE_5		"1024*768*32"
#define OPTION_MODE_6		"1278*716*16"
#define OPTION_MODE_7		"1278*716*32"
#define OPTION_MODE_8		"1400*768*16"
#define OPTION_MODE_9		"1400*768*32"

#define OPTION_GAMMA_0		"5"
#define OPTION_GAMMA_1		"4"
#define OPTION_GAMMA_2		"3"  
#define OPTION_GAMMA_3		"2"
#define OPTION_GAMMA_4		"1"
#define OPTION_GAMMA_5		"0"
#define OPTION_GAMMA_6		"-1"
#define OPTION_GAMMA_7		"-2"
#define OPTION_GAMMA_8		"-3"
#define OPTION_GAMMA_9		"-4"

#define OPTION_EFFECT_0		"0"
#define OPTION_EFFECT_1		"1"
#define OPTION_EFFECT_2		"2"
#define OPTION_EFFECT_3		"3"
#define OPTION_EFFECT_4		"4"
#define OPTION_EFFECT_5		"5"
#define OPTION_EFFECT_6		"6"
#define OPTION_EFFECT_7		"7"
#define OPTION_EFFECT_8		"8"
#define OPTION_EFFECT_9		"9"

#define OPTION_TERRAIN_0		"0"
#define OPTION_TERRAIN_1		"1"
#define OPTION_TERRAIN_2		"2"
#define OPTION_TERRAIN_3		"3"
#define OPTION_TERRAIN_4		"4"
#define OPTION_TERRAIN_5		"5"
#define OPTION_TERRAIN_6		"6"
#define OPTION_TERRAIN_7		"7"
#define OPTION_TERRAIN_8		"8"
#define OPTION_TERRAIN_9		"9"

#define OPTION_SOUND_0		"-10000"
#define OPTION_SOUND_1		"-3500"
#define OPTION_SOUND_2		"-3000"
#define OPTION_SOUND_3		"-2500"
#define OPTION_SOUND_4		"-2000"
#define OPTION_SOUND_5		"-1500"
#define OPTION_SOUND_6		"-1000"
#define OPTION_SOUND_7		"-600"
#define OPTION_SOUND_8		"-300"
#define OPTION_SOUND_9		"0"

#define OPTION_MUSIC_0		"-10000"
#define OPTION_MUSIC_1		"-3500"
#define OPTION_MUSIC_2		"-3000"
#define OPTION_MUSIC_3		"-2500"
#define OPTION_MUSIC_4		"-2000"
#define OPTION_MUSIC_5		"-1500"
#define OPTION_MUSIC_6		"-1000"
#define OPTION_MUSIC_7		"-600"
#define OPTION_MUSIC_8		"-300"
#define OPTION_MUSIC_9		"0"

#define OPTION_UNITDETAIL_0		"0"
#define OPTION_UNITDETAIL_1		"1"
#define OPTION_UNITDETAIL_2		"2"
#define OPTION_UNITDETAIL_3		"3"
#define OPTION_UNITDETAIL_4		"4"
#define OPTION_UNITDETAIL_5		"5"
#define OPTION_UNITDETAIL_6		"6"
#define OPTION_UNITDETAIL_7		"7"
#define OPTION_UNITDETAIL_8		"8"
#define OPTION_UNITDETAIL_9		"9"

#define OPTION_LOW_QUALITY_ON	"ON"
#define OPTION_LOW_QUALITY_OFF	"OFF"

#define OPTION_LOW_HELPDESK_ON	"ON"
#define OPTION_LOW_HELPDESK_OFF	"OFF"

#define OPTION_COMBO_STR_LEN		32
#define OPTION_COMBO_WIDTH			51
#define OPTION_COMBO_HEIGHT			13
#define OPTION_COMBO_INTERVAL		20
#define OPTION_COMBO_START_X		121
#define OPTION_RESOLUTION_X			180
#define OPTION_COMBO_SORT			23
#define OPTION_COMBO_MIDDLE_Y		202
#define OPTION_COMBO_INTERVAL		20
#define OPTION_COMBO_LIST_NUMBER	10

#define MODE_COMBO_STR_LEN			32
#define MODE_COMBO_NUMBER			6
#define MODE_COMBO_LIST_NUMBER		2	// 콤보박스 내렸을때 라인 숫자

// 2006-03-07 by ispark, 언어에 따라 위치 수정
#if defined(LANGUAGE_ENGLISH) || defined(LANGUAGE_VIETNAM)|| defined(LANGUAGE_THAI)// 2008-04-30 by bhsohn 태국 버전 추가
#define OPTION_COMBO_START_Y		68//70
#else
#define OPTION_COMBO_START_Y		68//70
#endif
typedef struct 
{	
	int sGammaCtrl;				// 감마
	int sTerrainRender;			// 지형 렌더링 거리
	int	sTerrainEffectRender;	// 지형 이펙트 시야
	int sObjectViewDistance;	// 오브젝트 거리
	int sShadowState;			// 그림자
	int sUnitDetail;			// 유닛 디테일
	BOOL sLowQuality;			// 최소 프레임 
	int sContrast;				// 대비
	int sFilter;				// 필터

	//추가 
	BOOL sEyePt;				// 시점 전환(TRUE : 1인칭, FALSE : 3인칭)
	int	sSoundVolume;
	int sMusicVolume;
	int	sHandle ;
	
//	BOOL sHelpDesk;
	BOOL m_bChatCurrentSelect[CHAT_MENULIST_NUMBER];	// 채팅 옵션 조정 (현재 6개만 가능)
	BOOL sMouseReversLeft;			// 마우스 좌우반전
	BOOL sMouseReversUp;			// 마우스 상하반전
	BOOL sAutoBalance;				// 자동 수평조정
	BOOL sMenuBalance;				// 메뉴사용시 자동 수평조정
	BOOL sPetOPBalance;				// 2010. 10. 26 by jskim 펫 오퍼레이터 옵션 처리
#ifdef _WARRIOR_ADDITIONAL_INFO
	BOOL sWSWAddAIMInfo; //additional info interface option by WarriorSW 2015-05-16
#endif
#ifdef _SHOW_GUILD_NAME
	BOOL sShowGuildName;
#endif
	int sFovValue;
	BOOL sMp3Player;
	
	int	sTarget ;
	int	sHeight ;
	int	sPKMode;
	int	InputType ;
	int nReset;
}sOPTION_SYSTEM;

typedef struct 
{
	BOOL sHelpDesk;
	UID64_t	UniqueNumber[QSLOT_TAB_NUMBER][QSLOT_NUMBER] ;// quick slot
	int		ItemNum[QSLOT_TAB_NUMBER][QSLOT_NUMBER] ;// quick slot	
}sOPTION_CHARACTER;

class CAtumNode;
class CINFImage;
class CD3DHanFont;
class CINFSystem : public CINFBase
{
public:
	CINFSystem(CAtumNode* pParent);
	virtual ~CINFSystem();

		virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	void Render();
	virtual void Tick();

	virtual int WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
	int SystemWndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
	int OptionWndProc(UINT uMsg, WPARAM wParam, LPARAM lParam) ;
	int ModeWndProc(UINT uMsg, WPARAM wParam, LPARAM lParam) ;
	void OnSystemButtonClick(int nButton);
	int OptionComboProc(int nOption, int x, int y, int nWindowPosY);
	void RenderComboOptionList(int nWindowPos);
	void RenderComboOptionListSingle(int i, char* strText, int x, int y);
	void GetOptionString(char* strData, int nOption, int index);
	BOOL OptionQualityCombo();
	
	int ModeComboProc(int nOption, int x, int y, int nWindowPosY);
	void GetModeString(char* strData, int nOption, int index);
	void RenderComboModeList(int nWindowPosY);
	void RenderComboModeListSingle(int i, char* strText, int x, int y);

	void SendFieldSocketCharacterGameEnd();

	void		UpdateGamma(int nGama);
	void		UpdateQuality(int nQuality);
	void		UpdateReflactive(int Reflactive);
	
	void		RenderCheckOption(int nWindowPosY);

	D3DGAMMARAMP		m_GammaRamp ;
	WORD				m_wGamma;

public:
	BOOL				m_bRestored;
	BOOL				m_bInvalidated;
	BOOL				m_bRenderToolTip;
	
	CINFImageEx	*	m_pSystemSelectButton[3];
	CINFImageEx	*	m_pSystemEndButton[3];

	CINFImageEx	*	m_pLogOut;
	CINFImageEx	*	m_pOpLog;
	CINFImageEx	*	m_pOption;
	CINFImageEx	*	m_pMode;

	D3DGAMMARAMP			m_D3dGamma;

	CINFImageEx	*	m_pCombo;
	CINFImageEx	*	m_pCheck;

	CD3DHanFont *	m_pFontOptionList[OPTION_COMBO_LIST_NUMBER];
	CD3DHanFont *	m_pFontSelectOptionComboData[OPTION_COMBO_NUMBER];
	CD3DHanFont *	m_pFontToolTip;
	char			m_strSelectOptionComboData[OPTION_COMBO_NUMBER][OPTION_COMBO_STR_LEN];
		
	int				m_nSystemType;
	int				m_nSystemButtonState[2];
	BOOL			m_bShowOptionListBox[OPTION_COMBO_NUMBER] ; 
	int				m_nOptionInfo[OPTION_COMBO_NUMBER] ;

	char			m_strSelectModeComboData[MODE_COMBO_NUMBER][MODE_COMBO_STR_LEN];
	CD3DHanFont *	m_pFontModeList[MODE_COMBO_LIST_NUMBER];//2개
	CD3DHanFont *	m_pFontSelectModeComboData[MODE_COMBO_NUMBER];//6개
	BOOL			m_bShowModeListBox[MODE_COMBO_NUMBER] ; 
	int				m_nModeInfo[MODE_COMBO_NUMBER] ; // mode에 들어간 값 (0, 1)
	BOOL			m_bQuitGameMessage;
	BOOL			m_bLogOffGameMessage;
	float			m_fTickSecTime;
	float			m_fLastTickSecTime;

	sOPTION_SYSTEM	m_pSOptionOld;
};

#endif // !defined(AFX_INFSYSTEM_H__D1ABB527_44B7_4158_96DE_4D806B53B315__INCLUDED_)
