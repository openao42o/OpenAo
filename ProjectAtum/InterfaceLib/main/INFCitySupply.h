// INFCitySupply.h: interface for the CINFCitySupply class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFCITYSUPPLY_H__4437050F_1521_498E_8C67_72549B1A032C__INCLUDED_)
#define AFX_INFCITYSUPPLY_H__4437050F_1521_498E_8C67_72549B1A032C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INFBase.h"

class CINFImage;
class CD3DHanFont;
class CINFScrollBar;
class CItemInfo;
class CINFImageEx;
class CINFImageRadioBtn;

struct stSupply
{
	char	chItemName[128];
	int		nItemNum;
	int		nItemIndexNum;			// 아이템의 캐릭터 파라미터
	int		nAllBtState;
	int		nSlideMoveRate;
	float	fSupplyRate;
	float	fCurrentSupplyRate;
	BOOL	bCheck;
	BOOL	bSlideBar;
};

class CINFCitySupply : public CINFBase
{
public:
	CINFCitySupply(CAtumNode* pParent, BUILDINGNPC* pBuilding);
	virtual ~CINFCitySupply();

	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();

	virtual void Render();
	virtual void Tick();
	virtual int WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

	void InitData(BOOL bSupply = FALSE);
	void InitItemCheck();

protected:
	void DrawSupplyPrice(int nIndex);
	void SendShopRepair();
	
private:
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	CINFImageEx	*	m_pSupplyButton[4];
	CINFImageEx	*	m_pCancelButton[4];
	CINFImageEx	*	m_pAllButton[3];
	CINFImageRadioBtn*	m_pRadioButton;
// 	CINFImageEx	*	m_pSupplyTitle;
// 	CINFImageEx	*	m_pSupplyBack;
	CINFImageEx	*	m_pSlideBar;
#else 
	CINFImageEx	*	m_pSupplyButton[4];
	CINFImageEx	*	m_pCancelButton[4];
	CINFImageEx	*	m_pAllButton[3];
	CINFImageEx	*	m_pRadioButton;
	CINFImageEx	*	m_pSupplyTitle;
	CINFImageEx	*	m_pSupplyBack;
	CINFImageEx	*	m_pSlideBar;
#endif
//	CINFImage	*	m_pClose;

	CD3DHanFont*	m_pFontItemName;

	stSupply		m_stSupply[6];

	int				m_nSupplyButtonState;
	int				m_nCancelButtonState;
	int				m_nLButtonDownState;

	// 스크롤 코드는 나중을 위해서 남겨둠
	int				m_nItemLength;
	POINT			m_ptScroll;
	POINT			m_ptTempScroll;
	POINT			m_ptTempSlideBar;

	BOOL			m_bLButtonDown;
	BOOL			m_bScrollLock;
	BOOL			m_bAllSupply;
	BOOL			m_bSlideBarLock;

	// 계산
	int				m_nTotalPrice;

	BUILDINGNPC*	m_pBuildingInfo;

	CD3DHanFont*	m_pInfluenceTex;
};

#endif // !defined(AFX_INFCITYSUPPLY_H__4437050F_1521_498E_8C67_72549B1A032C__INCLUDED_)
