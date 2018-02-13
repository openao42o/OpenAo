#pragma once

#include "INFBase.h"
#include "AtumParam.h"
#include "D3DHanFont.h"

enum MAP_STATE{
	MAP_STATE_ATT = 0,
	MAP_STATE_DEF,
	MAP_STATE_COUNT
};

enum HP_STATE{
	HP_STATE_ATT = 0,
	HP_STATE_DEF,
};

#define		IMPORTANT_ATT_WARNING_GAP		0.5f
#define		IMPORTANT_ATT_WARNING_TIME		10.f

class CINFImage;
class CINFImageEx;
class CINFStrategicPointInfo : public CINFBase  
{
public:
	CINFStrategicPointInfo();
	virtual ~CINFStrategicPointInfo();
	
	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	virtual void	Render();
	virtual void	Tick();

	void			UpdateHPRate(float i_HPRate = MS_WAR_HP_RATE);
	void			SetImportantPos(int i_X,int i_Y);
	BOOL			SetMapNum(DWORD i_nMapNum);
	void			Init(MapIndex_t i_nMapNum, BYTE i_nInfluenceType, ATUM_DATE_TIME i_StartTime);
	void			InitAllState();
	void			AttWarning();
	void			SetSpawnTime(ATUM_DATE_TIME i_StartTime);

	MAP_STATE		m_nMapState;
	HP_STATE		m_nHPState;
	float			m_fHP;
	int				m_nStartX;
	int				m_nStartY;

	INT				m_bMapInfluence;

	float			m_fWarningTime;
	float			m_fWarningGap;
	BOOL			m_bOldWarning;

	DWORD			m_nMapNum;
	CINFImageEx*	m_pImgMapState[MAP_STATE_COUNT];
	CINFImageEx*	m_pImgHP;
	CINFImageEx*	m_pImgHPBg;
	CINFImageEx*	m_pImgAttWarning;

	ATUM_DATE_TIME	m_SPStartTime;
	CD3DHanFont*	m_pRemaingTimeFont;

};
