// INFSelectMenu.h: interface for the CINFSelectMenu class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFSELECTMENU_H__F22FDA13_ADA7_4EED_B2AC_682DC56CDFC0__INCLUDED_)
#define AFX_INFSELECTMENU_H__F22FDA13_ADA7_4EED_B2AC_682DC56CDFC0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INFBase.h"

#define SEL_BUTTON_NUMBER			5
#define SEL_CHARACTER_INFO_NUMBER	8
#define SEL_GAGE_NUMBER				3 // todo : examine what this does

class CAtumNode;
class CINFImage;
class CD3DHanFont;
// 2011. 10. 10 by jskim UI시스템 변경
class CINFGroupManager;
class CINFGroupImage;
class CINFImageEx;
// end2011. 10. 10 by jskim UI시스템 변경
class CINFSelectMenu : public CINFBase
{
public:
	CINFSelectMenu(CAtumNode* pParent);
	virtual ~CINFSelectMenu();

	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	void Render();
	virtual void Tick();
	virtual int WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

	int OnButtonClicked(int i);
	void RenderGage(CINFImage* pImage, int x, int y, float width, float height);

public:
	BOOL			m_bRestored;
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경

	CINFGroupManager* m_GruopSelectmanager;
	CINFGroupImage*   m_PSelTitle;
	CINFGroupImage*   m_PBack;
	CINFGroupImage*   m_PControlSelbtn;
	CINFImageEx*	m_pButton[SEL_BUTTON_NUMBER][4];
	CINFImageEx*	m_pGage[3];
#else		   
 	CINFImageEx*	m_pBack;
 	CINFImageEx*	m_pTitle;	  
	CINFImageEx*	m_pButton[SEL_BUTTON_NUMBER][4];
	CINFImageEx*	m_pGage[3];
#endif
	float			m_fButtonPos[SEL_BUTTON_NUMBER][2];
	CD3DHanFont*	m_pFontLevel;
	CD3DHanFont*	m_pFontCharacterName;
	CD3DHanFont*	m_pFontCharacterInfo[SEL_CHARACTER_INFO_NUMBER];
	CD3DHanFont*	m_pFontGage[SEL_GAGE_NUMBER];
	int				m_nLevelPos[2];
	int				m_nCharacterNamePos[2];
	int				m_nCharacterInfoPos[SEL_CHARACTER_INFO_NUMBER][2];
	int				m_nGagePos[SEL_GAGE_NUMBER][2];
	int				m_nButtonState[SEL_BUTTON_NUMBER];
	
private:
	// 2007-05-15 by bhsohn 기어 스탯 관련 처리
	CD3DHanFont*	m_pFontOverStat;
};

#endif // !defined(AFX_INFSELECTMENU_H__F22FDA13_ADA7_4EED_B2AC_682DC56CDFC0__INCLUDED_)
