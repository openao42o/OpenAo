// INFMapLoad.h: interface for the CINFMapLoad class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFMAPLOAD_H__2C684A39_4413_4219_AF25_695731758D95__INCLUDED_)
#define AFX_INFMAPLOAD_H__2C684A39_4413_4219_AF25_695731758D95__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AtumNode.h"

#define NUMBER_LOADBACK		4
#define MAPLOAD_IMG_NUM		4
#define MAPLOAD_NAME_IMG	0.0f

class DataHeader;
class CINFImage;
class CGameData;
class CINFMapName;
class CINFImageEx;

class CINFMapLoad : public CAtumNode  
{
public:
	CINFMapLoad();
	virtual ~CINFMapLoad();

	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	virtual void Tick();
	virtual void Render();
	void InitData();
	void SetShuttleHeight();

	HRESULT	SetResourceFile(char* szFileName) ;
	DataHeader* FindResource(char* szRcName);
	BOOL	GetRenderMapNameImg(){return m_bMapNameRender;}
	void	SetRenderMapNameImg(BOOL bFlag){m_bMapNameRender = bFlag;}

	// 2007-04-05 by bhsohn 맵로드시, 체크섬 추가
	void			SendServerMapLoad();

	// 2007-11-20 by bhsohn 맵로딩 하는 방식 변경
	BYTE			GetLoginChatMode();
	void			SetLoginChatMode(BYTE bLoginChat);
	void			MapLoadNextStep();	// Load 다음 화면으로 
	void			MapLoadComplete();
	BOOL			IsMapLoad();
	void			SetMapLoad(BOOL bMapLoad);

//	CINFImage	*	m_pLoadBack[NUMBER_LOADBACK];	
//	CINFImage	*	m_pLoadBar;	
//	CINFImage	*	m_pLoadRound;
//	CINFImage	*	m_pLoadAtum;
	BOOL			m_bMapLoad;
//	CINFImage	*	m_pImgBack[MAPLOAD_IMG_NUM];
	CINFImage	*	m_pImgBack;
	CINFImage	*	m_pImgNowLoading;
	CINFImage	*	m_pImgBar[2];
	CINFImage	*	m_pImgCopyRight;
	CINFImage   *	m_pImgSpaceCowboy;
	CINFImage	*	m_pImgUnderBack;				// 2006-03-13 by ispark
	CGameData	*	m_pGameData;
	DataHeader	*	m_pDataHeader[7];

//	FLOAT			m_fRotateRate;
	float			m_fLoadingRate;
	BYTE			m_bLoginChat;

	BOOL			m_bMapNameRender;
	float			m_fMapNameViewTime;
	DWORD			m_dwAlpha;

private:
	// 2007-04-24 by bhsohn 와이드 모니터 관련 처리
	float GetYScale();
};

#endif // !defined(AFX_INFMAPLOAD_H__2C684A39_4413_4219_AF25_695731758D95__INCLUDED_)
