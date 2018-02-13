// INFCreateMenu.h: interface for the CINFCreateMenu class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFCREATEMENU_H__88045B6C_4FA7_45E8_BDE4_1FF2FA9DDB47__INCLUDED_)
#define AFX_INFCREATEMENU_H__88045B6C_4FA7_45E8_BDE4_1FF2FA9DDB47__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INFBase.h"

#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
#define CRE_BUTTON_NUMBER					6
#define CRE_CHARACTER_INFO_NUMBER			8
#define CRE_CHARACTER_UNIT_NUMBER			4
#define CRE_CHARACTER_RADIO_BUTTON_NUMBER	2
#define CREATE_CHARACTER_RADIO_ALL_AUTTON	3
#define CRE_TOTAL_NUMBER					10													  
#else												
#define CRE_BUTTON_NUMBER					4
#define CRE_CHARACTER_INFO_NUMBER			8
#define CRE_CHARACTER_UNIT_NUMBER			4
#define CRE_CHARACTER_RADIO_BUTTON_NUMBER	2
#define CREATE_CHARACTER_RADIO_ALL_AUTTON	3
#define CRE_TOTAL_NUMBER					10
#endif// 2011. 10. 10 by jskim UI시스템 변경
class CINFGroupManager;
class CINFGroupImage;
// end 2011. 10. 10 by jskim UI시스템 변경
class CGameData;
class CINFImage;
class CINFImageEx;
class CD3DHanFont;
class CINFCreateMenu : public CINFBase
{
public:
	CINFCreateMenu(CAtumNode* pParent);
	virtual ~CINFCreateMenu();

	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	void	Render();
	virtual void Tick();
	virtual int		WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

	BOOL	ErrCheckCharacterName(char * strName);
	void	OnButtonClicked(int i);
	void	CharacterSet(USHORT uUnitKind);
//	void	SetCharacterUnitStyle(BOOL bToolTip = FALSE);	// 2005-07-04 by ispark 자동 스탯 분배 삭제
	void	RenderPopUpWindowImage(int x, int y, int cx, int cy);

public:
	BOOL			m_bRestored;
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	CINFGroupManager* m_GruopCreatemanager;
	CINFGroupImage*   m_pTitle;
	CINFGroupImage*   m_pBack;
	CINFGroupImage*   m_PControlCrebtn;
	CINFImageEx*	  m_pBackTemp;
	BOOL			  m_bEffectFrame;
#else
	CGameData*		m_pGameData;
	//CINFImageEx*	m_pBack[CRE_CHARACTER_UNIT_NUMBER];
	CINFImageEx*	m_pBack;
	CINFImageEx*	m_pTitle;
	CINFImageEx*	m_pImgToolTip[9];
#endif
	CINFImageEx*	m_pButton[CRE_BUTTON_NUMBER][4];
//	CINFImage*		m_pImgRadio[CRE_CHARACTER_RADIO_BUTTON_NUMBER];
	CINFImageEx*	m_pImgFocus[4];
	CINFImageEx*	m_pImgTextPopUp[3];
	float			m_fButtonPos[CRE_BUTTON_NUMBER][2];
	int				m_nButtonState[CRE_BUTTON_NUMBER];
	// 2009-02-10 by bhsohn Japan Charcter Create
	//char			m_strCharacterName[SIZE_MAX_CHARACTER_NAME];
	char			m_strCharacterName[MAX_PATH];
	// end 2009-02-10 by bhsohn Japan Charcter Create
	CD3DHanFont*	m_pFontCharacterName;
	CD3DHanFont*	m_pFontCharacterInfo[CRE_CHARACTER_INFO_NUMBER];
	int				m_nCharacterNamePos[2];
	int				m_nCharacterInfoPos[CRE_CHARACTER_INFO_NUMBER][2];

	BOOL			m_bShowCursor;
	float			m_fChangeShowCursorTime;
	BOOL			m_bBlocking;//create시에 블락시키는 변수

	MSG_FC_CHARACTER_CREATE m_createUnit;
	int				m_nSelectUnit;
	int				m_nRadioButton[CREATE_CHARACTER_RADIO_ALL_AUTTON];
	int				m_nSelectUnitStyle;
	int				m_nFocus;
	int				m_nFocusStat;
	int				m_nCreateUnitStat;
//	int				m_nRenderToolTipIndex;		// 2005-07-04 by ispark 자동 스탯 분배 삭제
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	BOOL			m_bRenderInfoBtn;

	int				m_nBtnUpFadeMode;
	float			m_fBtnUpFadeInTime;
	DWORD			m_dwBtnUpBkAlpha;

	int				m_nBtnDownFadeMode;
	float			m_fBtnDownFadeInTime;
	DWORD			m_dwBtnUDownpBkAlpha;														  

	float					m_fTimeEffectBG;			// 배경 프레임 유지 시간
	int						m_nEffStep;					// 이팩트 스텝

#endif

protected:
	int				m_nFaceUseIndex[CRE_TOTAL_NUMBER];			// 페이스 넘버
	int				m_nChoiseFaceIndex;			// 페이스 선택 번호
};

#endif // !defined(AFX_INFCREATEMENU_H__88045B6C_4FA7_45E8_BDE4_1FF2FA9DDB47__INCLUDED_)
