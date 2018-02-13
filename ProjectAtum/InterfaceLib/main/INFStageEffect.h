// INFStateEffect.h: interface for the CINFStageEffect class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFSTATEEFFECT_H__6EC66A09_2DC8_412C_9453_14D9EB971AE5__INCLUDED_)
#define AFX_INFSTATEEFFECT_H__6EC66A09_2DC8_412C_9453_14D9EB971AE5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AtumNode.h"
#define STAGE_IMG_NUM		4

///////////////////////////////////////////////////////////////////////////////
/// \class		CINFStageEffect
///
/// \brief		illust image의 FADEIN/FADEOUT, sound 플레이, SKIP 기능
/// \author		dhkwon
/// \version	
/// \date		2004-06-03 ~ 2004-06-03
/// \warning	
///////////////////////////////////////////////////////////////////////////////

class CINFImage;
class DataHeader;
class CINFImageEx;									  // 2011. 10. 10 by jskim UI시스템 변경
class CINFStageEffect : public CAtumNode  
{
public:
	CINFStageEffect(int nImageIndex, float fPlayTime, BOOL bSkipEnable=FALSE);
	virtual ~CINFStageEffect();

	BOOL IsRuning() { return (m_bRunning || !m_bEndWork); } // StageEffect의 FadeIn/FadeOut완료와 동시진행작업의 완료가 되어야 한다.
	void SetEndWork( BOOL bSet ) { m_bEndWork = bSet; }	// 현재 StageEffect와 동시진행하는 작업이 완료되면 세팅한다.
	BOOL GetEndWork() { return m_bEndWork; }

	HRESULT InitDeviceObjects();
	HRESULT RestoreDeviceObjects();
	HRESULT InvalidateDeviceObjects();
	HRESULT DeleteDeviceObjects();
	void Tick();
	void Render();
	int WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
protected:
	BOOL					m_bRestored;
	BOOL					m_bRunning;
	int						m_nState;
	int						m_nIndex;
	float					m_fPlayTime;
	BOOL					m_bSkipEnable;
	float					m_fCurrentTime;
	BOOL					m_bEndWork;
//	CINFImage				*m_pImage[STAGE_IMG_NUM];
//	DataHeader				*m_pDataHeader[STAGE_IMG_NUM];

	// 2006-11-14 by ispark, 이미지 통합
	CINFImage				*m_pImage;					// 2011. 10. 10 by jskim UI시스템 변경
	DataHeader				*m_pDataHeader;
};

#endif // !defined(AFX_INFSTATEEFFECT_H__6EC66A09_2DC8_412C_9453_14D9EB971AE5__INCLUDED_)
