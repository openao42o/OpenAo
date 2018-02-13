// GlobalImage.h: interface for the INFImageAnimation class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GLOBALIMAGE_H__8B409C14_93C0_4282_B7B5_8367FF326F11__INCLUDED_)
#define AFX_GLOBALIMAGE_H__8B409C14_93C0_4282_B7B5_8367FF326F11__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INFBase.h"

class CINFImageEx;                // 2011. 10. 10 by jskim UI시스템 변경
class CINFImage;
class DataHeader;
class CGameData;
class CD3DHanFont;

class CGlobalImage: public CINFBase
{

public:
	CGlobalImage();
	virtual ~CGlobalImage();
	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	virtual void Render();
	virtual void Tick();
	virtual int WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
	
//	HRESULT	SetResourceFile(char* szFileName) ;
//	DataHeader* FindResource(char* szRcName);
	
	BOOL	GetRenderFlag(){ return 	m_bRenderFlag;}
	void	SetImageInfo(int nImageIndex, BOOL bMissionImg=FALSE, int nQuestIndex = 0, int x=0, int y=0, DWORD dwType=ALPHA_DELAY_WAVE, float fTotalTime=8.0f, float fDelay=2.0f);

	// 2008-03-27 by bhsohn 미션성공창이 떠있는 상태에서는 상점이 안열리게 변경
	int		GetSelectImageIndex();

private:
	// 2007-03-20 by bhsohn 게임 등급물 위원회 아이콘 추가
	void	TickGdwl();
	void	RederGdwl();

protected:
	CINFImageEx	*			m_pImageGlobal[GLOBAL_IMAGE_MAX_COUNT];		                // 2011. 10. 10 by jskim UI시스템 변경
	CD3DHanFont	*			m_pFontTitle;

	BOOL					m_bRenderFlag;
	float					m_fViewTime;
	DWORD					m_dwAlpha;

	int						m_nSelectImageIndex;
	DWORD					m_dwSelectType;
	float					m_fTotalTime;
	float					m_fDelay;
	int						m_nRenderPosX;
	int						m_nRenderPosY;
	BOOL					m_bMissionImg;

	int						m_nMissionIndex;

	// 2007-03-20 by bhsohn 게임 등급물 위원회 아이콘 추가
	CINFImageEx	*			m_pImageGdwl;												// 2011. 10. 10 by jskim UI시스템 변경
	float					m_fGdwlViewTime;
	DWORD					m_dwGdwlAlpha;
	float					m_fGdwlCheckTotalTime;
	float					m_fGdwlLiveTotalTime;
	float					m_fGdwlDelay;	
	BOOL					m_bGdwlRenderFlag;
	

//public:
//	CGameData	*			m_pGameData;	
};

#endif
