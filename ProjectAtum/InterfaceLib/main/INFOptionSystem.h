// INFOptionSystem.h: interface for the CINFOptionSystem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFOPTIONSYSTEM_H__664F80FC_7B76_47AA_9E60_9F4272DA8E82__INCLUDED_)
#define AFX_INFOPTIONSYSTEM_H__664F80FC_7B76_47AA_9E60_9F4272DA8E82__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INFBase.h"
#include "INFGameMainQSlot.h"
#include "INFSystem.h"

class CAtumNode;
class CINFImage;
class CD3DHanFont;
class CINFImageBtn;
class CINFImageRadioBtn;
class CD3DHanFont;
class CINFListBox;
class CINFGroupImage;											  // 2011. 10. 10 by jskim UI시스템 변경
class CINFArenaScrollBar;

// 라디오버튼 갯수
#define		OPTION_SYS_ON			0
#define		OPTION_SYS_OFF			1
#define		MAX_OPTION_SYS_CNT		2

#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
// 그래픽 옵션  콤모
#define		OPTION_GRAPHIC_SEE_RANGE		0	// 시야거리
#define		OPTION_GRAPHIC_UNIT_DETAIL		1	// 유닛 디테일
#define		OPTION_GRAPHIC_SHADOW			2	// 그림자조절
#define		OPTION_GRAPHIC_EFFECT			3	// 이펙트 조절
#define		OPTION_GRAPHIC_GAMMA			4	// 감마 조절
#define		OPTION_GRAPHIC_CONTRAST			5	// 대비 조절
#define		OPTION_GRAPHIC_FILTER			6	// 필터 효과
// 기타 옵션 콤보
#define		OPTION_ETC_SEE_CHANGE			7	// 시점변환
//#define		OPTION_ETC_VOLUMNE				8	// 볼륨 조절
#define		MAX_OPTION_GRAPHIC_COMBO		8
#else
// 그래픽 옵션  콤모
#define		OPTION_GRAPHIC_SEE_RANGE		0	// 시야거리
#define		OPTION_GRAPHIC_UNIT_DETAIL		1	// 유닛 디테일
#define		OPTION_GRAPHIC_SHADOW			2	// 그림자조절
#define		OPTION_GRAPHIC_EFFECT			3	// 이펙트 조절
#define		OPTION_GRAPHIC_GAMMA			4	// 감마 조절
#define		OPTION_GRAPHIC_CONTRAST			5	// 대비 조절
#define		OPTION_GRAPHIC_FILTER			6	// 필터 효과
// 기타 옵션 콤보
#define		OPTION_ETC_SEE_CHANGE			7	// 시점변환
#define		OPTION_ETC_VOLUMNE				8	// 볼륨 조절
#define		MAX_OPTION_GRAPHIC_COMBO		9
#endif


// 기타 옵션 체크 버튼
#define		OPTION_ETC_SHOW_MOUSE			0	// 마우스 포인터 보이기
#define		OPTION_ETC_HELP					1	// 헬프기능
#define		OPTION_ETC_OPERATOR				2	// 오퍼레이터 기능
#define		OPTION_ETC_MAINTAIN				3	// 수평유지
#define		OPTION_ETC_MOUSE_REVERSE_TB		4	// 마우스 상하 반전
#define		OPTION_ETC_MOUSE_REVERSE_LR		5	// 마우스 좌우 반전
// 2010. 10. 26 by jskim 펫 오퍼레이터 옵션 처리
//#define		OPTION_ETC_MENU_UNISTOP			6	// 메뉴 사용시 유닛정지
#define		OPTION_ETC_PET_OP_SYS			6	// 메뉴 사용시 유닛정지
// end 2010. 10. 26 by jskim 펫 오퍼레이터 옵션 처리
#define		OPTION_ETC_MUSIC				7	// Music플레이어 보기
#define		MAX_OPTION_ETC_RADIO			8	

enum
{
	VEDIO_OPTION_TAB,
	SOUND_OPTION_TAB,
	EX_OPTION_TAB
};

#define  TOTAL_OPTION_TAB 3
typedef struct 
{		
	sOPTION_SYSTEM			struSOptionSetup;
	sOPTION_CHARACTER		struSOptionCharacter;
	sOPTION_ETC				struSOptionEtc;	// 다른 옵션
	
	
	BOOL		bOperator;								// 오퍼레이터
	
} structOptionGameInfo;


class CINFOptionSystem  : public CINFBase
{
public:	
	CINFOptionSystem(CAtumNode* pParent);
	virtual ~CINFOptionSystem();

	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	void Render();
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	void VideoRender();
	void SoundRender();
	void ExRender();
	void VideoTick();
	void SoundTick();
	int VideoProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
	int SoundProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
	int ExProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
#endif
	void ExTick();

	virtual void Tick();

	virtual int WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

	void SetComboData(int nIdx, char* pSrc);

	BOOL IsOptionShow();
	void  ShowOptionWnd(BOOL bShow);

	structOptionGameInfo* GetSystemOption();

	void RefreshShowInterface(BOOL bShowInterface);

	// 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템
	void	EnableLogOffBtn( BOOL bEnable );
	// end 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템

private:
	void UpdateBtnPos(int nBackPosX, int nBackPosY);

	int OnLButtonDown(WPARAM wParam, LPARAM lParam);
	int OnMouseMove(WPARAM wParam, LPARAM lParam);
	int OnLButtonUp(WPARAM wParam, LPARAM lParam);
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	int OnVedioLButtonDown(WPARAM wParam, LPARAM lParam);
	int OnVedioMouseMove(WPARAM wParam, LPARAM lParam);
	int OnVedioLButtonUp(WPARAM wParam, LPARAM lParam);

	int OnSoundLButtonDown(WPARAM wParam, LPARAM lParam);
	int OnSoundMouseMove(WPARAM wParam, LPARAM lParam);
	int OnSoundLButtonUp(WPARAM wParam, LPARAM lParam);

	int OnExLButtonDown(WPARAM wParam, LPARAM lParam);
	int OnExMouseMove(WPARAM wParam, LPARAM lParam);
	int OnExLButtonUp(WPARAM wParam, LPARAM lParam);
#endif
	// 시스템 버튼 클릭 
	void OnSystemButtonClick(int nButton);

	structOptionGameInfo InitOptionGameInfo();

	// 라디오 버튼 갱신
	void SetRadioInfo(int nMainId, BOOL bOnOff);

	// 옵션 인터페이스 갱신 
	void UpdateOptionInterface(structOptionGameInfo* pOptionInfo);
	// 옵션값 갱신
	void UpdateOptionInfo();

	// 최소 프레임 클릭
	BOOL OptionQualityCombo();
	void UpdateQuality(sOPTION_SYSTEM* pOption);

	// 그래픽옵션 커서값을 얻어온다.
	int GetGarphicOption_To_Cursel(int nMode, int nState);
	int GetGarphicCursel_To_Option(int nMode, int nCursel);

	// Etc옵션
	int GetEtcOption_To_Cursel(int nMode, int nState);
	int GetEtcCursel_To_Option(int nMode, int nState);

	// 라디오 버튼 클릭

	void OnClickEtcRadio(int nRadioId);
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	void OnClickRadio(int nRadioId);
	void OnClickVedioRadio(int nRadioId);
	void OnClickSoundRadio(int nRadioId);

	
#endif


	// 적용버튼
	void OnClickApply();

	// 감마적용
	void UpdateGamma(int nGama);

	// 2008-11-13 by bhsohn 조이스틱 작업
	void OnClickJoystciOption();

	
public:
	BOOL				m_bRestored;
	BOOL				m_bInvalidated;
	BOOL				m_bRenderToolTip;

	BOOL			m_bQuitGameMessage;
	BOOL			m_bLogOffGameMessage;
	float			m_fTickSecTime;
	float			m_fLastTickSecTime;
private:
	// 초기화를 위한 과거 버퍼
	sOPTION_SYSTEM			m_pSOptionOld;
	sOPTION_CHARACTER		m_struSOptionCharacterOld;

	// 현재 셋업파일에 적용된 형식
	structOptionGameInfo	m_struOpInfo;


	
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	CINFGroupImage*	m_pOptionBk;
	CINFGroupImage*	m_pOptionSubBk[ TOTAL_OPTION_TAB ];											  
#else
	CINFImageEx	*	m_pOptionBk;
#endif

	char			m_strSelectModeComboData[MODE_COMBO_NUMBER][MODE_COMBO_STR_LEN];

	BOOL			m_bShow;		// 

	POINT			m_ptBakPos;		// 위치
	POINT			m_ptBakSize;	// 크기

	CINFImageBtn*	m_pOptionApp;	// 적용
	CINFImageBtn*	m_pOptionInit;	// 초기화
	CINFImageBtn*	m_pOptionLogOff;	// 로그오프
	CINFImageBtn*	m_pOptionGameEnd;	// 게임종료

	// 2008-11-13 by bhsohn 조이스틱 작업
	CINFImageBtn*	m_pOptionJoystic;	// 조이스틱 옵션

	// 해상도
	CD3DHanFont *	m_pFontHaeSang;		
	POINT			m_ptHaesangPos;		
	char			m_szHaesangTxt[256];		

	// 라디오 버튼 
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	CINFImageRadioBtn*		m_pGameVedioOpRadioBtn[MAX_VEDIO_OPTION_RADIO];
	CINFImageRadioBtn*		m_pGameSoundOpRadioBtn[MAX_SOUND_OPTION_RADIO];
	CINFImageRadioBtn*		m_pGameEctORadioBtn[MAX_ETC_OPTION_RADIO];
#else
	CINFImageRadioBtn*		m_pGameOpRadioBtn[MAX_OPTION_RADIO][MAX_OPTION_SYS_CNT];
#endif
	


	// 그래픽 콤보 박스
	CINFListBox *			m_pComboGraphic[MAX_OPTION_GRAPHIC_COMBO];
	CINFImageRadioBtn*		m_pMiniFrame;	// 초기화

#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	CINFImageEx*			m_pMp3On;															  
	BOOL					m_bmp3Playview;
#else
	CINFImageRadioBtn*		m_pEtcRadio[MAX_OPTION_ETC_RADIO];		
#endif
	// 기타 옵션	
	

	// 닫기 버튼
	CINFImageBtn*			m_pCloseBtn;
	
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	int						m_nCurTeb;
	CINFArenaScrollBar*		m_pFovBar;
	CD3DHanFont*			m_pFov;
	// 스크롤 바 
	CINFArenaScrollBar*		m_pSoundVolBar;
	// 스크롤 바 
	CINFArenaScrollBar*		m_pEffectVolBar;
	CD3DHanFont*			m_pVol;																  
#endif

	// 2012-03-13 mspark, 게임 사운드 옵션 문제 해결 - 추가
	int						m_nSoundVolumeTemp;
	int						m_nMusicVolumeTemp;
	int						m_nFOVTemp;
	// end 2012-03-13 mspark, 게임 사운드 옵션 문제 해결 - 추가
};

#endif // !defined(AFX_INFOPTIONSYSTEM_H__664F80FC_7B76_47AA_9E60_9F4272DA8E82__INCLUDED_)
