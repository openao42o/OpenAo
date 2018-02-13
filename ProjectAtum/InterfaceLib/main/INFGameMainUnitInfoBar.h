// INFGameMainUnitInfoBar.h: interface for the CINFGameMainUnitInfoBar class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFGAMEMAINUNITINFOBAR_H__D89CEB23_A523_4B16_A8DD_203B1EEFA70F__INCLUDED_)
#define AFX_INFGAMEMAINUNITINFOBAR_H__D89CEB23_A523_4B16_A8DD_203B1EEFA70F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INFBase.h"

#define GAMEMAIN_BACK_UP_PIXEL			0
#define GAMEMAIN_BACK_DOWN_PIXEL		1

#define GAMEMAIN_GAGE_HP_UP				0
#define GAMEMAIN_GAGE_DP_UP				1
#define GAMEMAIN_GAGE_BURN_UP			2
#define GAMEMAIN_GAGE_SP_UP				3
#define GAMEMAIN_GAGE_EP_UP				4
#define GAMEMAIN_GAGE_SPEED_UP			5
#define GAMEMAIN_GAGE_EXP_UP			6
//추가 - 문자출력을 위해서
#define GAMEMAIN_NEW_STRING_HP			7
#define GAMEMAIN_NEW_STRING_MAX_HP		8
#define GAMEMAIN_NEW_STRING_DP			9
#define GAMEMAIN_NEW_STRING_MAX_DP		10
#define GAMEMAIN_NEW_STRING_MAX_SP		11

#define GAMEMAIN_GAGE_NUMBER			9		
#define GAMEMAIN_GAGE_LINE_NUMBER		12		
#define GAMEMAIN_GAGE_LINE_LENGTH		32

#define GAMEMAIN_BUTTON_SHOW			0
#define GAMEMAIN_BUTTON_HIDE			1

#define GAMEMAIN_GAGE_HP_MAX_GRADE		20
#define GAMEMAIN_GAGE_DP_MAX_GRADE		20
#define GAMEMAIN_GAGE_SP_MAX_GRADE		20
#define GAMEMAIN_GAGE_BOOSTER_MAX_GRADE	18
#define GAMEMAIN_GAGE_FUEL_MAX_GRADE	17

#define GAMEMAIN_EVENT_RECOVERY_MAX		5

#define GAMEMAIN_POS_X					0		// 2011. 10. 10 by jskim UI시스템 변경
#define GAMEMAIN_POS_Y					0		// 2011. 10. 10 by jskim UI시스템 변경

//2015-08-15 by St0rmy, better Ping Display
#define PING_DISPLAY_POS_X					g_pD3dApp->GetBackBufferDesc().Width - 40
#define PING_DISPLAY_POS_Y					160

#define PING_DISPLAY_SIZE_X				24
#define PING_DISPLAY_SIZE_Y				24

#define PING_DISPLAY_COUNT				4		//Count of possible display options
#define PING_STATUS_GOOD				0		//Best one, green 4 bars
#define PING_STATUS_MEDIUM				1		//Medium one, yellow 3 bars
#define PING_STATUS_BAD					2		//Bad one, orange 2 bars
#define PING_STATUS_WORST				3		//Worst one, red 1 bar (point)

#define PING_LIMIT_GOOD					0.150f	//100 ms
#define PING_LIMIT_MEDIUM				0.250f	//200 ms
#define PING_LIMIT_BAD					0.350f	//300 ms


// 2006-08-29 by dgwoo 각종 이벤트 관련 정보
struct EVENT_INFO
{//추가되는 요소를 변수로 추가...
	float			fEXPRate;
	float			fSPIRate;
	float			fEXPRepairRate;
	float			fDropItemRate;
	float			fDropRareRate;
	float			fWarPointRate;
	EVENT_INFO(){
		fEXPRate = 0;
		fSPIRate = 0;
		fEXPRepairRate = 0;
		fDropItemRate = 0;
		fDropRareRate = 0;
		fWarPointRate = 0;
	};
};
// 2007-11-28 by dgwoo 선물메시지 구조체.
#define	GIFT_ICON_IMG_COUNT				2				// 선물 이미지 갯수.

struct GIFT_NOTIFY_MSG
{
	UID64_t		NotifyMsgUID;
	UID64_t		CharacterUID;
	char		NotifyMsgString[SIZE_MAX_NOTIFY_MSG_STRING];
};

class CINFImage;
class CINFImageEx;		// 2011. 10. 10 by jskim UI시스템 변경
class CINFImageBtn;
class CINFGroupImage;		// 2011. 10. 10 by jskim UI시스템 변경
class CD3DHanFont;
class CINFGameMainUnitInfoBar : public CINFBase  
{
public:
	CINFGameMainUnitInfoBar(CAtumNode* pParent);
	virtual ~CINFGameMainUnitInfoBar();

	void			UpdateAllGage();
	void			SetHP(float fCurrent, float fMax);
	void			SetDP(float fCurrent, float fMax);
	void			SetBURN(float fCurrent, float fMax);
	void			SetSP(float fCurrent, float fMax);
	void			SetEP(float fCurrent, float fMax);
	void			SetSpeed(float fCurrent, float fMin, float fMax);
	void			SetEXP(float fExp);
	void			SetFame();
	void			SetHappyHourEventIconPos();

	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	void			Render();
	void			RenderCityUpInfo();							// 도시에서 사용
	void			RenderSpeed();
	void			HideRender();								// GUI가 숨겨진 상황(F키)에서 Render()
	void			RenderMainGage(BOOL bShowAll = TRUE);
	void			RenderGageString(BOOL bShowAll = TRUE);
	void			RenderPremiumCard(BOOL bIsCity);			// PremiumCard 렌더링
	void			RenderMapPosition(UINT x, UINT y);
	BOOL			RenderCurrentEvent(UINT x, UINT y);
	void			RenderHappyHourEvent(BOOL bIsCity);
	void			RenderTextHappyHourEvent();					
	void			RenderEventRecovery();
	void			RenderStringBar();
	void			RenderGiftIcon(BOOL i_bCity);							// 2007-11-28 by dgwoo 선물 받았을 경우 아이콘.

	// 2012-10-12 by jhjang 해피 아워 경험치 보너스 시스템 리뉴얼
	void			RenderBonusExpRate();
	void			RenderBonusExpRateText();					
	void			RenderRestKillCount(BOOL bIsMinimap);
	// end 2012-10-12 by jhjang 해피 아워 경험치 보너스 시스템 리뉴얼

	virtual void	Tick();
	virtual int		WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
	int				WndProcCity(UINT uMsg, WPARAM wParam, LPARAM lParam);
	int				GetMaxSize(int nNum);						// nNum의 자리수를 구한다.
	int				CutOffFirstNumber(int& nNum,int nMaxNum);	// nNum의 첫자리를 자른다 - 짤린값 리턴.
	void			ShowMapNameTitle(UINT x, UINT y);							// 게임 타이틀 바에 맵이름 표시
	BOOL			IsRenderMapName();					// 마우스 포인터가 레이더에 위치 하지 않으면 맵이름 렌더링	

	// 2007-11-01 by bhsohn 해피 아우어 이벤트 처리 방식 변경
	BOOL			IsHappyHourEvent();
	BOOL			IsHappyHourPcBang();
	void			SetHappyHourInfluence(BOOL bLevelup, BOOL bHappyHour, MSG_FC_INFO_GET_HAPPY_HOUR_EVENT_INFO_OK* pMsg);
	void			SetHappyHourPcBang(BOOL bLevelup, BOOL bPcBang, MSG_FC_INFO_GET_HAPPY_HOUR_EVENT_INFO_OK* pMsg);
	void			InitHappyHourInfo();
	void			AddGiftMsg(UINT i_NotifyMsgUID,UINT i_CharacterUID,char * NotifyString);
	void			DeleteGiftMsg();

	// 2008-05-21 by bhsohn 모선전, 아이템 해피 아우어 추가
	void			SetHappyHourMotherShip(BOOL bLevelup, BOOL bStart, MSG_FC_INFO_GET_HAPPY_HOUR_EVENT_INFO_OK* pMsg);
	void			SetHappyHourItem(BOOL bLevelup, BOOL bStart, MSG_FC_INFO_GET_HAPPY_HOUR_EVENT_INFO_OK* pMsg);
	// end 2008-05-21 by bhsohn 모선전, 아이템 해피 아우어 추가

	// 2012-10-12 by jhjang 해피 아워 경험치 보너스 시스템 리뉴얼
	void	SetRestCount(INT nRestCount);
	void	SetKillCount(INT nKillCount);
	INT		GetRestCount() { return m_nRestCount; };
	INT		GetKillCount() { return m_nKillCount; };
	void	SetBonusExpRate(INT nPartyRate,INT nFriendRate,INT nGuildRate);
	// end 2012-10-12 by jhjang 해피 아워 경험치 보너스 시스템 리뉴얼

	// 2012-11-16 by mspark, 데카의 기운 표시 리뉴얼
	void	RestCountImageOnOff();
	void	KillCountImageOnOff();
	// end 2012-11-16 by mspark, 데카의 기운 표시 리뉴얼

// 2013. 04. 04 by ssjung 보너스 EXP, 멤버쉽 에 마우스를 가져다 댔을 때 시스템 메시지를 표시 안하게 수정
	BOOL    GetRenderBonusExpRateTextOnOff();
	BOOL	GetRenderPremiumToolTipTextOnOff();

// 2013. 04. 04 by ssjung 보너스 EXP, 멤버쉽 에 마우스를 가져다 댔을 때 시스템 메시지를 표시 안하게 수정

protected:

	// 2007-11-01 by bhsohn 해피 아우어 이벤트 처리 방식 변경
	// 2008-03-14 by bhsohn 레벨업에 따른 해피아우어 처리
	void			SetHappyHourInfo(BOOL bLevelup, MSG_FC_INFO_GET_HAPPY_HOUR_EVENT_INFO_OK* pMsg, EVENT_INFO* pEventInfo);	
	void			UpdateHappyHourTooltip();	

	void			CreateMsgBox();

	void DecaToolTipFunction(BOOL nMode, int nY);	//TRUE면 데카의 축복 FALSE면 데카의 기운 
	// 2013-07-15 by ssjung 캐나다 데카의 축복, 기운 관련 툴팁 표시
	
protected:
	BOOL			m_bRestored;
	BOOL			m_bRenderMapName;
	float			m_fWarningTime;
	BOOL			m_bWarning;
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
    CINFGroupImage*		m_pGageBarInfo;				// 2011. 10. 10 by jskim UI시스템 변경
#else
	CINFImageEx*		m_pImgBackPixel[2];			// 2011. 10. 10 by jskim UI시스템 변경
	CINFImageEx*		m_pImgBackMinimap;
#endif


	CINFImageEx* m_fpsNumbers[10];
								  
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	// 2004-12-08 by jschoi - 추가
	// 2011. 10. 10 by jskim UI시스템 변경
	CINFImageEx*		m_pImgMainGageBack;										// 메인 게이지 배경(좌상단)
	CINFImageEx*		m_pImgMainExpBack;										// 메인 경험치 배경(중앙하단)
	CINFImageEx*		m_pImgMainExpBar;										// 메인 경험치 막대(중앙하단)
	CINFImageEx*		m_pImgMainExpBar1;										// 메인 경험치 막대(중앙하단)
	CINFImageEx*		m_pImgMainGageHP[GAMEMAIN_GAGE_HP_MAX_GRADE];			// 메인 게이지 HP
	CINFImageEx*		m_pImgMainGageDP[GAMEMAIN_GAGE_DP_MAX_GRADE];			// 메인 게이지 DP
	CINFImageEx*		m_pImgMainGageSP[GAMEMAIN_GAGE_SP_MAX_GRADE];			// 메인 게이지 SP
	CINFImageEx*		m_pImgMainGageBooster[GAMEMAIN_GAGE_BOOSTER_MAX_GRADE];	// 메인 게이지 Booster
	CINFImageEx*		m_pImgMainGageFuel[GAMEMAIN_GAGE_FUEL_MAX_GRADE];		// 메인 게이지 Fuel
	CINFImageEx*		m_pImgMainIconFuel[2];									// 메인 아이콘 Fuel
	CINFImageEx*		m_pImgMainSpeedNum;										// 메인 디지털 스피드
	CINFImageEx*		m_pImgMainMinus;										// 메인 디지털 스피드 마이너스(추가)	
	CINFImageEx*		m_pImgMainWarning;										// 메인 경고
	CINFImageEx*		m_pImgGeneralPremiumCard;								// General Premium Card Image
#else 
    CINFImageEx*		m_pImgMainGageBack;										// 메인 게이지 배경(좌상단)
	CINFImageEx*		m_pImgMainExpBack;										// 메인 경험치 배경(중앙하단)
	CINFImageEx*		m_pImgMainExpBar;										// 메인 경험치 막대(중앙하단)
	CINFImageEx*		m_pImgMainGageHP[GAMEMAIN_GAGE_HP_MAX_GRADE];			// 메인 게이지 HP
	CINFImageEx*		m_pImgMainGageDP[GAMEMAIN_GAGE_DP_MAX_GRADE];			// 메인 게이지 DP
	CINFImageEx*		m_pImgMainGageSP[GAMEMAIN_GAGE_SP_MAX_GRADE];			// 메인 게이지 SP
	CINFImageEx*		m_pImgMainGageBooster[GAMEMAIN_GAGE_BOOSTER_MAX_GRADE];	// 메인 게이지 Booster
	CINFImageEx*		m_pImgMainGageFuel[GAMEMAIN_GAGE_FUEL_MAX_GRADE];		// 메인 게이지 Fuel
	CINFImageEx*		m_pImgMainIconFuel[2];									// 메인 아이콘 Fuel
	CINFImageEx*		m_pImgMainSpeedNum;										// 메인 디지털 스피드
	CINFImageEx*		m_pImgMainMinus;										// 메인 디지털 스피드 마이너스(추가)	
	CINFImageEx*		m_pImgMainWarning;										// 메인 경고
	CINFImageEx*		m_pImgGeneralPremiumCard;								// General Premium Card Image
#endif
	// end 2011. 10. 10 by jskim UI시스템 변경
	// 2009. 03. 16 by ckPark 멤버쉽 툴팁 추가

	BOOL			m_bPremiumToolTip;

	// end 2009. 03. 16 by ckPark 멤버쉽 툴팁 추가
	// 2011. 10. 10 by jskim UI시스템 변경
	CINFImageEx*	m_pImgSuperPremiumCard;									// Super Premium Card Image
	CINFImageEx*	m_pImgHappyHourEvent;									// 해피아워 관련 이벤트 이미지
	CINFImageEx*	m_pImgPCHappyHourEvent;									// PC방 해피아워 관련 이벤트 이미지
	CINFImageEx*	m_pImgPCHomePremiumEvent;								// 2010. 06. 08 by jskim 홈프리미엄 UI 작업
	CINFImageEx*	m_pImgEventRecovery[GAMEMAIN_EVENT_RECOVERY_MAX];		// 이벤트 회복율 관련 이벤트 이미지
	CINFImageEx*	m_pImgGiftIcon[GIFT_ICON_IMG_COUNT];					// 선물 아이콘 이미지 
	//CINFImageBtn*	m_pImgVoiceChatIcon;									// 음성채팅 아이콘.
	CINFImageEx*	m_pImgVoiceChatIcon;									// 음성채팅 아이콘.
	CINFImageEx*	m_pImgVoiceSPKIcon;									// 스피커 아이콘.
	// end 2011. 10. 10 by jskim UI시스템 변경
	CD3DHanFont*	m_pFontEventRecovery;									// 이벤트 회복율 내용
	CD3DHanFont*	m_pFontDecaToolTip;										// 2013-07-15 by ssjung 캐나다 데카의 축복, 기운 관련 툴팁 표시


	// 2012-10-12 by jhjang 해피 아워 경험치 보너스 시스템 리뉴얼
	CINFImageEx*	m_pImgBonusExpRate;
	CINFImageEx*	m_pImgRestCount;
	CINFImageEx*	m_pImgKillCount;
	CINFImageEx*	m_pImgKillCount2;
	CINFImageEx*	m_pImgKillEff;

	CINFGroupImage*	m_pGroupImgRestCountBack;
	CINFGroupImage*	m_pGroupImgKillCountBack;

	BOOL			m_bRenderBonusExpRateText;
	INT				m_nPartyRate;
	INT				m_nFriendRate;
	INT				m_nGuildRate;
	
	INT				m_nRestCount;
	INT				m_nKillCount;
	float			m_fKillCountScale;
	bool			m_bChangeKillCount;
	bool			m_bCompleteKillCount;
	INT				m_nCompleteKillCountBlank;
	BYTE			m_nCompleteKillCountAlpha;
	// 2012-10-12 by jhjang 해피 아워 경험치 보너스 시스템 리뉴얼

	// 2012-11-16 by mspark, 데카의 기운 표시 리뉴얼
	CINFImageBtn*			m_pImgRestCountOnBtn;
	CINFImageBtn*			m_pImgRestCountOffBtn;
	CINFImageBtn*			m_pImgKillCountOnBtn;
	CINFImageBtn*			m_pImgKillCountOffBtn;

	UINT			m_nStateRestCountOnBtn;
	UINT			m_nStateRestCountOffBtn;
	UINT			m_nStateKillCountOnBtn;
	UINT			m_nStateKillCountOffBtn;

	UINT			m_nRestCountPosX;
	UINT			m_nRestCountPosY;
	UINT			m_nKillCountPosX;
	UINT			m_nKillCountPosY;

	bool			m_bCheckRestCountON;
	bool			m_bCheckKillCountON;

	bool			m_bCheckRestCountBtnClick;
	bool			m_bCheckKillCountBtnClick;

	float			m_fPlusRestCountPosX;
	float			m_fPlusKillCountPosX;
	// end 2012-11-16 by mspark, 데카의 기운 표시 리뉴얼


	float			m_fGageRate[GAMEMAIN_GAGE_NUMBER];

	CD3DHanFont*	m_pFontGageInfo[GAMEMAIN_GAGE_LINE_NUMBER];
#ifdef _WARRIOR_CLASS_KILL_BAR
	char			m_szGageInfo[GAMEMAIN_GAGE_LINE_NUMBER][128];	
#else
	char			m_szGageInfo[GAMEMAIN_GAGE_LINE_NUMBER][GAMEMAIN_GAGE_LINE_LENGTH];	
#endif

	CD3DHanFont*	m_pFontUnitPosInfo;

//	CINFImage*		m_pButtonShowHide[2][BUTTON_STATE_NUMBER];	//	GAMEMAIN_BUTTON_SHOW, GAMEMAIN_BUTTON_HIDE
	int				m_nButtonState;

	// 맵이름 표시
	CD3DHanFont*	m_pFontMapName;
	
	// 현재 이벤트 표시
	CD3DHanFont*	m_pFontCurrentEvent;
	CD3DHanFont*	m_pFontEventHappyHour;

	D3DXVECTOR2		m_v2MousePt;
	D3DXVECTOR2		m_v2RaderCenter;

	// 미션 이름 표시
	CD3DHanFont*	m_pFontMissionName;	

public:
	vector<string>	m_vecEventHappyHour;
	vector<GIFT_NOTIFY_MSG> m_vecGiftMsg;
	EVENT_INFO		m_HappyEventInfo;
	// 2007-11-01 by bhsohn 해피 아우어 이벤트 처리 방식 변경
	EVENT_INFO		m_HappyEventAllInfluInfo;		

	EVENT_INFO		m_PCHappyEventInfo;
	BOOL			m_bRenderEvent;
	BOOL			m_bRenderEventRecovery;
	int				m_nHappyHourX;
	int				m_nHappyHourY;
	int				m_nPCHappyHourX;
	int				m_nPCHappyHourY;
	BOOL			m_bMemberPCBang;
	// 2010. 06. 08 by jskim 홈프리미엄 UI 작업
	BOOL			m_pMemberPremiumEvent;
	//end 2010. 06. 08 by jskim 홈프리미엄 UI 작업
	//--------------------------------------------------------------------------//
	//			m_BHappyHourEvent Flag				
	//			0x01				HappyHourEvent		해피 아워 이벤트
	//			0x10				PCHappyHourEvent	PC해피 아워 이벤트.
	//--------------------------------------------------------------------------//
	// 2007-11-01 by bhsohn 해피 아우어 이벤트 처리 방식 변경
	//BYTE			m_BHappyHourEvent;				
	BYTE			m_byHappyHourInfluence;
	BOOL			m_bHappyHourAllInfluence;
	BOOL			m_bPcBangHappyHour;
	
	// 2008-05-21 by bhsohn 모선전, 아이템 해피 아우어 추가
	BOOL			m_bMotherHappyHour;
	EVENT_INFO		m_MotherShipEventInfo;

	BOOL			m_bItemHappyHour;
	EVENT_INFO		m_ItemEventInfo;
	

	int				m_nGiftX;
	int				m_nGiftY;
	POINT			m_ptGift;
	float			m_fGiftTime;

// 2013-07-15 by ssjung 캐나다 데카의 축복, 기운 관련 툴팁 표시
	BOOL			m_bRestCountToolTip;
	BOOL			m_bKillCountToolTip;
// end 2013-07-15 by ssjung 캐나다 데카의 축복, 기운 관련 툴팁 표시

	//2015-08-15 by St0rmy, better Ping Display
	CINFImageEx*	m_pImgPing[PING_DISPLAY_COUNT];

};

#endif // !defined(AFX_INFGAMEMAINUNITINFOBAR_H__D89CEB23_A523_4B16_A8DD_203B1EEFA70F__INCLUDED_)
