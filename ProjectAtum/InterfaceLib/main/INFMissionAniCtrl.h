// INFMissionAniCtrl.h: interface for the CINFMissionAniCtrl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFMISSIONANICTRL_H__4934DA80_E04F_4850_8B80_FDB301C5132E__INCLUDED_)
#define AFX_INFMISSIONANICTRL_H__4934DA80_E04F_4850_8B80_FDB301C5132E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INFBase.h"

// 진형간 에니메이션
#define	MANI_INFLUENCE_BCU		0
#define	MANI_INFLUENCE_ANI		1
#define	MAX_MANI_INFLUENCE		2

// 애니메이션 상태 
#define	MANI_ANIMATION_MARK				0		// 마크
#define	MANI_ANIMATION_NORMAL			1		// 일반
#define	MANI_ANIMATION_ANGER			2		//화난 표정
#define	MAX_MANI_MISSION_ANIMATION		3


class CINFMissionAniCtrl  : public CINFBase
{
public:
	CINFMissionAniCtrl();
	virtual ~CINFMissionAniCtrl();

	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	virtual void	Render();
	virtual void	Tick();

	void SetWindowPos(float fPosX, float fPosY, float fWidth, float fHeight);
	void InitAnimation(BOOL bFirstPopup);	

	void StartAnimation(BOOL bErrorQuest);

	// 진형 선택
	void SetMAniInfluence(BYTE byInfluence);
private:
	void InitAniInfo(BOOL bStartAnimation, BOOL bInitStep);
	void GetMaxFrame(int nAniPos, int* nMaxAniFrame, int* nDiffX, int* nDiffY);

	void RenderNormal();	

private:
	CINFImageEx	*			m_pImageAni[MAX_MANI_INFLUENCE][MAX_MANI_MISSION_ANIMATION];	

	CINFImageEx	*			m_pImageGrrr;				// 통신 장애 영상	
	int				m_nAniPos;

	int				m_nAniStep;
	int				m_nAniStepX;
	int				m_nAniStepY;

	int				m_nGrrrStep;
	int				m_nGrrrStepX;
	int				m_nGrrrStepY;

	float			m_fAniTime;
	float			m_fGrrrTime;		// 지짐 시간 

	float			m_fPosX;
	float			m_fPosY;

	float			m_fWidth;
	float			m_fHeight;

	float			m_fFrameWidth[MAX_MANI_INFLUENCE][MAX_MANI_MISSION_ANIMATION];
	float			m_fFrameHeight[MAX_MANI_INFLUENCE][MAX_MANI_MISSION_ANIMATION];

	float			m_fShowY[MAX_MANI_INFLUENCE][MAX_MANI_MISSION_ANIMATION];

	float			m_fGrrrWidth;
	float			m_fGrrrHeight;

	BOOL			m_bStartAnimation;

	int				m_nInfluence;
	BOOL			m_bSelErrorQuest;	

};

#endif // !defined(AFX_INFMISSIONANICTRL_H__4934DA80_E04F_4850_8B80_FDB301C5132E__INCLUDED_)
