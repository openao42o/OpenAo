// INFCityWarp.h: interface for the CINFCityWarp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFCITYWARP_H__8C8FFF17_5918_4808_A782_6368A036D467__INCLUDED_)
#define AFX_INFCITYWARP_H__8C8FFF17_5918_4808_A782_6368A036D467__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INFBase.h"

#ifdef C_EPSODE4_UI_CHANGE_JSKIM
#define CITY_WARP_BUTTON_NUMBER			4
#else 
#define CITY_WARP_BUTTON_NUMBER			3
#endif
#define CITY_WARP_BUTTON_MOVE			0
#define CITY_WARP_BUTTON_CANCEL			1

#ifdef C_EPSODE4_UI_CHANGE_JSKIM
#define CITY_WARP_LIST_NUMBER			8
#else
#define CITY_WARP_LIST_NUMBER			6
#endif

#define CITY_WARP_LIST_STRING_LENGTH	64

class CAtumNode;
class CINFImage;
class CINFImageEx;
class CINFGroupImage;
class CD3DHanFont;
class CINFScrollBar;
class CINFCityWarp : public CINFBase  
{
public:
	CINFCityWarp(CAtumNode* pParent);
	virtual ~CINFCityWarp();

	void Reset();
	void AddWarpTargetInfoList(WARP_TARGET_MAP_INFO_4_EXCHANGE* pInfo);
	void RecvWarpListDone();
	WARP_TARGET_MAP_INFO_4_EXCHANGE *GetCurrentWarpInfo();

	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	void Render();
	virtual void Tick();
	virtual int WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
	void OnButtonClicked(int nButton);
	void SendFieldSocketRequestShopWarp();//int nMapIndex, int nTargetIndex);

protected:
	BOOL		m_bRestored;
	CINFScrollBar*	m_pScroll;
	
#ifdef C_EPSODE4_UI_CHANGE_JSKIM
	CINFGroupImage*	m_pImgBack;
#else
	CINFImageEx*	m_pImgBack;
#endif
	CINFImageEx*	m_pImgTitle;
	CINFImageEx*	m_pButtonMove[CITY_WARP_BUTTON_NUMBER];
	CINFImageEx*	m_pButtonCancel[CITY_WARP_BUTTON_NUMBER];
	CINFImageEx*	m_pImgHightLight;

	int			m_nButtonState[2];		// 버튼 상태, CITY_WARP_BUTTON_MOVE, CITY_WARP_BUTTON_CANCEL
	CD3DHanFont *m_pFontWarpList[CITY_WARP_LIST_NUMBER];
	CD3DHanFont *m_pFontWarpPrice[CITY_WARP_LIST_NUMBER];
	char		m_szWarpList[CITY_WARP_LIST_NUMBER][CITY_WARP_LIST_STRING_LENGTH];
	int			m_nWarpListLineNumber;		// 리스트 전체 개수
	int			m_nMapIndex;				// 워프할 맵 인덱스
	int			m_nTargetIndex;				// 워프할 맵의 타겟 인덱스
	
	CVectorWarpTargetInfo m_vecWarpTargetInfo;

	CD3DHanFont*	m_pInfluenceTex;
};

#endif // !defined(AFX_INFCITYWARP_H__8C8FFF17_5918_4808_A782_6368A036D467__INCLUDED_)
