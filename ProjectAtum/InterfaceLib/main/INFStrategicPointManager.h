#pragma once
#include "INFBase.h"
#include "INFImageBtnBoth.h"
#include "INFStrategicPointInfo.h"
#include "StdAfx.h"

#define SLIDE_TIME											0.25f

#define STRATEGICPOINT_INFO_BALLOON							9
#define STRATEGICPOINT_MAXLINE								6

#define GAMEMAIN_STRATEGICPOINT_INFO_START_X				187
#define GAMEMAIN_STRATEGICPOINT_INFO_START_Y				40
#define GAMEMAIN_STRATEGICPOINT_INFO_HEIGHT_GAP				55

#define GAMEMAIN_STRATEGICPOINT_INFO_INFL_START_X			185
#define GAMEMAIN_STRATEGICPOINT_INFO_INFL_START_Y			(GAMEMAIN_STRATEGICPOINT_INFO_START_Y + 3)
#define GAMEMAIN_STRATEGICPOINT_INFO_TIME_START_Y			(GAMEMAIN_STRATEGICPOINT_INFO_START_Y + 18)
#define GAMEMAIN_STRATEGICPOINT_INFO_AREA_START_Y			(GAMEMAIN_STRATEGICPOINT_INFO_START_Y + 33)

#define GAMEMAIN_STRATEGICPOINT_INFO_WIDTH					60
#define GAMEMAIN_STRATEGICPOINT_INFO_EACH_HEIGHT			15
#define GAMEMAIN_STRATEGICPOINT_INFO_CAP_HEIGHT				10
#define STRATEGICPOINT_IMPORTANT_GAP						52
#define STRATEGICPOINT_BUTTON_IMPORTANT_GAP					20

#define APPLY_HEIGHTGAP(_BASE,_MULTIPLYER)					(_BASE+((_MULTIPLYER)*GAMEMAIN_STRATEGICPOINT_INFO_HEIGHT_GAP))

#define ID_STRATEGICPOINT_BALLOON_TLH				6
#define ID_STRATEGICPOINT_BALLOON_TLW				8
#define ID_STRATEGICPOINT_BALLOON_TMH				6
#define ID_STRATEGICPOINT_BALLOON_TMW				1
#define ID_STRATEGICPOINT_BALLOON_TRH				6
#define ID_STRATEGICPOINT_BALLOON_TRW				6

#define ID_STRATEGICPOINT_BALLOON_MLH				1
#define ID_STRATEGICPOINT_BALLOON_MLW				8
#define ID_STRATEGICPOINT_BALLOON_MMH				1
#define ID_STRATEGICPOINT_BALLOON_MMW				1
#define ID_STRATEGICPOINT_BALLOON_MRH				1
#define ID_STRATEGICPOINT_BALLOON_MRW				6

#define ID_STRATEGICPOINT_BALLOON_BLH				6
#define ID_STRATEGICPOINT_BALLOON_BLW				8
#define ID_STRATEGICPOINT_BALLOON_BMH				6
#define ID_STRATEGICPOINT_BALLOON_BMW				1
#define ID_STRATEGICPOINT_BALLOON_BRH				6
#define ID_STRATEGICPOINT_BALLOON_BRW				6

/**
 * CINFStrategicPointManager
 * This class is for displaying MS Like SP Infos
 * Created by killburne 01.05.2015
 */
class CINFStrategicPointManager :
	public CINFBase
{
public:
	CINFStrategicPointManager();
	virtual ~CINFStrategicPointManager();

	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	virtual void	Render();
	virtual void	Tick();
	void			RenderStrategicPointInfo(const vector<CINFStrategicPointInfo*> &i_vecInfo, BYTE influenceType, int row);
	void			RenderStrategicPointBalloon(int x, int y, int cx, int cy);
	void			TickShowIng();
	virtual int		WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
	int				WndProcOption(UINT uMsg, WPARAM wParam, LPARAM lParam);

	void			SetStrategicPoint(SSTRATEGYPOINT_DISPLAY_INFO* strategicPoint);
	void			RemoveStrategicPoint(MapIndex_t mapIndex);
	void			ResetAllStrategicPoints();

	SIZE			GetStringSize(char* pTxt);
	SIZE			GetStrategicPointInfoFontWidth();

	void			SetButtonPosBCU(UINT i_nPosX, UINT i_nPosY);
	void			SetButtonPosANI(UINT i_nPosX, UINT i_nPosY);
	void			SetButtonPosNGC(UINT i_nPosX, UINT i_nPosY);

private:
	vector<CINFStrategicPointInfo*>	m_vecInfoBCU;
	vector<CINFStrategicPointInfo*>	m_vecInfoANI;
	vector<CINFStrategicPointInfo*>	m_vecInfoNGC;

	float						m_fShowTime;

	bool						m_bShowBCU;
	bool						m_bShowANI;
	bool						m_bShowNGC;
	int							m_nShowNumBCU;
	int							m_nShowNumANI;
	int							m_nShowNumNGC;

	int							m_nButtonPosXBCU;
	int							m_nButtonPosXANI;
	int							m_nButtonPosXNGC;
	int							m_nButtonPosYBCU;
	int							m_nButtonPosYANI;
	int							m_nButtonPosYNGC;

	CINFImageBtnBoth*			m_pImgStrategicPointBCUButton;
	CINFImageBtnBoth*			m_pImgStrategicPointANIButton;
	CINFImageBtnBoth*			m_pImgStrategicPointNGCButton;

	SIZE						m_sizeMotherShipInfo;
	CINFImageEx*				m_pBalonChat[STRATEGICPOINT_INFO_BALLOON];
	CD3DHanFont*				m_pFontStrategicPointInfo[STRATEGICPOINT_MAXLINE];
};