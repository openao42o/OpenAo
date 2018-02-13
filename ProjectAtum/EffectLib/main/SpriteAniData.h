#pragma once

// SpriteAniData.h: interface for the CSpriteAniData class.
//////////////////////////////////////////////////////////////////////

#include "characterinfo.h"

struct SpriteAniData : public Effect
{
	char	m_strName[20];
	int		m_nTextureVertexBufferType;
	float	m_fTextureSize;
	float	m_fVel;
	float	m_fTick;
	BOOL	m_bZbufferEnable;
	D3DXCOLOR m_cColor;
	D3DXCOLOR m_cColorStep;
	float	m_fColorChangeStartTime;
	BOOL	m_bAlphaBlending;
	char	m_strTextureFile[20];
	int		m_nSrcBlend;
	int		m_nDestBlend;
	int		m_nTextureRenderState;
	BOOL	m_bLightMapAlphaBlending;
	char	m_strLightMapFile[20];
	int		m_nLightMapSrcBlend;
	int		m_nLightMapDestBlend;
	int		m_nLightMapRenderState;
//	BOOL	m_bColorLoop;
	int		m_nColorLoop;
	BOOL	m_bZWriteEnable;
};

class CSpriteAni : public SpriteAniData  
{
public:
	CSpriteAni();
	~CSpriteAni();
	BOOL Tick(float fElapsedTime);
	void Render();
	HRESULT InitDeviceObjects();
	HRESULT RestoreDeviceObjects();
	HRESULT InvalidateDeviceObjects();
	HRESULT DeleteDeviceObjects();
	void SetTexture(char* strTexture);// by dhkwon, 030917

//	CSpriteAniRender*	m_pRenderer;
//	FLOAT				m_fDistance;
	int					m_nSpriteType;
	FLOAT				m_fCurrentTick;
	FLOAT				m_fCurrentTime;
	float				m_fCurrentRotateAngle;
	float				m_fCurrentRotateTime;
	CEffectInfo*		m_pParent;
};