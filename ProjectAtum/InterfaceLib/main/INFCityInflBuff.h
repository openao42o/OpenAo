// INFCityArena.h: interface for the CINFCityInflBuff class.
// 2007-04-19 by dgwoo
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFCITYINFL_BUFF_H__81371F39_ACB1_4435_AF54_E6C8C699A632__INCLUDED_)
#define AFX_INFCITYINFL_BUFF_H__81371F39_ACB1_4435_AF54_E6C8C699A632__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INFBase.h"
#include "INFArenaCreate.h"
#include "INFArenaScrollBar.h"


class CINFListBox;

class CINFImage;
class CINFImageEx;	
class CINFGroupImage;

// 모선전 진형
#define	INFLUENCE_INDEX_VCN					0	// 바이제니유
#define	INFLUENCE_INDEX_ANI					1	// 알링턴
#define	MAX_INFLUENCE_INDEX					2	// 최대 진영수 

////////// 
#define	MAX_INFLBUFF_ANI_X					4	// 최대 진영수 
#define	MAX_INFLBUFF_ANI_Y					2	// 최대 진영수 

//////////// 연승 버퍼 ////////////
#define		MAX_CONTINUEWIN					8



class CINFCityInflBuff : public CINFBase  
{
public:	
	CINFCityInflBuff(CAtumNode* pParent);
	virtual ~CINFCityInflBuff();
	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	virtual void Render();
	virtual void Tick();
	virtual int WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual int OnMouseWheel(WPARAM wParam, LPARAM lParam);
	virtual int OnLButtonDown(WPARAM wParam, LPARAM lParam);
	virtual int OnLButtonUp(WPARAM wParam, LPARAM lParam);
	virtual int OnMouseMove(WPARAM wParam, LPARAM lParam);

protected:
	void	InitAniData();
	void	OnClickGetBuff();

protected:
	float				m_fAniTime;
	RECT				m_rcAniData[MAX_INFLBUFF_ANI_Y*MAX_INFLBUFF_ANI_X];	
	int					m_nAniIdx;
	BOOL				m_bAniPlay;

	CINFImageEx*		m_pImgBK[MAX_INFLUENCE_INDEX];	
	CINFImageEx*		m_pImgNum[MAX_INFLUENCE_INDEX][MAX_CONTINUEWIN];
	CINFImageEx*		m_pImgText;	

	CINFImageBtn*		m_pGetBuffBtn[MAX_INFLUENCE_INDEX];

	CINFImageEx*		m_pImgWinTxt[MAX_CONTINUEWIN];	
	

};

#endif // !defined(AFX_INFCITYINFL_BUFF_H__81371F39_ACB1_4435_AF54_E6C8C699A632__INCLUDED_)
