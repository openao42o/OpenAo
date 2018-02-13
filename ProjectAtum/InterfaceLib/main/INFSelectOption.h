// INFSelectOption.h: interface for the CINFSelectOption class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFSELECTOPTION_H__A8F8C38A_7A61_436B_A840_31BD1770723B__INCLUDED_)
#define AFX_INFSELECTOPTION_H__A8F8C38A_7A61_436B_A840_31BD1770723B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INFBase.h"
#include "INFSystem.h"


class CGameData;
class CINFImage;
class CINFImageBtn;
class CINFImageRadioBtn;
class CINFListBox;
class CD3DHanFont;
class CINFImageEx;												   // 2011. 10. 10 by jskim UI시스템 변경

#define		OPTION_GRAPHIC_SEE_RANGE		0	// 시야거리
#define		OPTION_GRAPHIC_UNIT_DETAIL		1	// 유닛 디테일
#define		OPTION_GRAPHIC_SHADOW			2	// 그림자조절
#define		OPTION_GRAPHIC_EFFECT			3	// 이펙트 조절
#define		OPTION_GRAPHIC_GAMMA			4	// 감마 조절
#define		OPTION_GRAPHIC_CONTRAST			5	// 대비 조절
#define		OPTION_GRAPHIC_FILTER			6	// 필터 효과
#define		OPTION_GRAPHIC_END				7	// 필터 효과


class CINFSelectOption : public CINFBase  
{
public:
	CINFSelectOption();
	virtual ~CINFSelectOption();

	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	virtual void Render();

	virtual void Tick();
	int WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

	BOOL			IsShow() {		return m_bShow;	}
	BOOL			SetShow(BOOL i_bShow);
	int				OnMouseMove(WPARAM wParam, LPARAM lParam);
	int				OnLButtonDown(WPARAM wParam, LPARAM lParam);
	int				OnLButtonUp(WPARAM wParam, LPARAM lParam);

	void			InitGraphicSetting();
	void			UpdateBtn();
	void			SetOptionList();
	void			GetOptionList();
	int				GetGarphicOption_To_Cursel(int nMode, int nState);
	int				GetGarphicCursel_To_Option(int nMode, int nCursel);
	void			UpdateQuality(sOPTION_SYSTEM* pOption);
	BOOL			OptionQualityCombo();
	BOOL			OptionReset();						// 옵션 초기화.
	void			InitOptionGameInfo();
	BOOL			UpdateOption();						// 옵션 저장.

	sOPTION_SYSTEM					m_stOption;
	
	BOOL							m_bShow;
	POINT							m_ptPos;
	CINFImageEx *					m_pBG;								// 2011. 10. 10 by jskim UI시스템 변경
	CINFImageBtn *					m_pResetBtn;
	CINFImageBtn *					m_pOK;
	CINFImageBtn *					m_pClose;
	CINFListBox *					m_pGraphicLB[OPTION_GRAPHIC_END];
	char							m_szHaesangTxt[256];			// 해상도.
	POINT							m_ptHaesangPos;
	CD3DHanFont*					m_pFontHaeSang;
	CINFImageRadioBtn*				m_pMiniFrame;	


};

#endif // !defined(AFX_INFSELECTOPTION_H__A8F8C38A_7A61_436B_A840_31BD1770723B__INCLUDED_)
