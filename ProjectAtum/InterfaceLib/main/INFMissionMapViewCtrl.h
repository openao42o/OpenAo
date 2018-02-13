// INFMissionMapViewCtrl.h: interface for the CINFMissionMapViewCtrl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFMISSIONMAPVIEWCTRL_H__1526ABC7_38E3_48E8_A354_6834D65EB0EF__INCLUDED_)
#define AFX_INFMISSIONMAPVIEWCTRL_H__1526ABC7_38E3_48E8_A354_6834D65EB0EF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INFBase.h"
#include "INFArenaScrollBar.h"
#include "INFImageEx.h"								 // 2011. 10. 10 by jskim UI시스템 변경

typedef struct 
{
	CINFImageEx*	pInfImage;		// 맵 이미지
	DataHeader*		pHeader;
	MapIndex_t		MapIndex;
	char			chMapName[256];
	vector<MEX_MONSTER_INFO>	vecMonsterInfo;	
} stMapViewInfo;

typedef struct
{	
	int			nQuestIndex;
	int			nMonsterPosX;
	int			nMonsterPosY;
} MapView_PosData;

#define	MONSTER_INFO_ITEM_LEN	2
// 몬스터 최대 탭
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
     #define	MAX_MONSTER_INFO		4																  
#else																									  
#define	MAX_MONSTER_INFO		8
#endif

class CINFMissionMapViewCtrl  : public CINFBase
{
public:
	CINFMissionMapViewCtrl();
	virtual ~CINFMissionMapViewCtrl();

	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	virtual void	Render();
	virtual void	Tick();

	void UpdateMapInfo(CQuest* pQuest, MapIndex_t	MapIndex);
	void RestoreMapView(BOOL bAll, MapIndex_t	MapIndex);

	void SetWindowPos(float fPosX, float fPosY, float fPosWidth, float fPosHeight);

	// 이벤트 메시지 
	BOOL OnLButtonDown(POINT pt);	
	BOOL OnMouseWheel(POINT pt, WPARAM wParam, LPARAM lParam);
	BOOL OnLButtonUp(POINT pt);
	BOOL OnMouseMove(POINT pt);

private:	
	void RenderMapInfo();
	// 몬스터 정보 업데이트
	void RenderMonsterInfo(vector<MEX_MONSTER_INFO>	*i_vecMonsterInfo);
	// 퀘스트 몬스터 위치 표시
	void RenderQuestMonsterPos();

	// 몬스터 위치 정보 갱신 
	void SetMonsterMiniMap(CQuest* pQuest, MapIndex_t	MapIndex, vector<MapView_PosData> *o_vecMosterPosition);

	// 호전성, 비호전성 변수
	void GetAttackString(MEX_MONSTER_INFO * i_pMonsterInfo, char* o_pTxt);

	BOOL IsLoadMapInfo(MapIndex_t	MapIndex, int* o_nItemCnt);
	int  AddMapInfo(MapIndex_t MapIndex);
	// 맵에 대한 몬스터 정보를 얻어온다.
	HRESULT GetMapIndex_To_Monster(MapIndex_t	MapIndex, vector<MEX_MONSTER_INFO> *o_vecQuestInfo);
	BOOL GetToolTipTxt(int nSelTooltip, char* chBuf);

	void UpdateScrollPos(float fPosX, float fPosY, float fPosWidth, float fPosHeight);

	// 맵에 대한 정보를 얻어온다. 
	stMapViewInfo* GetMapIdx_To_MapViewInfo(MapIndex_t selMapIndex);

	BOOL UpdateToolTipInfo(POINT pt);

	// 세력몬스터이냐?
	BOOL IsBelligerenceMonster(BYTE monsterBelligerence);

private:
	vector<stMapViewInfo*>	m_vecMapInfo;
	MapIndex_t				m_selMapIndex;

	// 몬스터 정보
	CD3DHanFont *			m_pFontMonInfo;
	// 출몰 몬스터
	CD3DHanFont *			m_pFontMonTitle;
	// 맵 이름
	CD3DHanFont *			m_pFontMapName;

	float					m_fPosX;
	float					m_fPosY;
	float					m_fWidth;
	float					m_fHeight;

	CINFImageEx	*			m_pImageItem;
	CINFImageEx	*			m_pImgQuestTarget;

#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	POINT					m_ptMonPos[MAX_MONSTER_INFO];
#endif
	POINT					m_ptMonTitle;

	// 스크롤 바 
	CINFArenaScrollBar*		m_pINFScrollBar;

	vector<MapView_PosData> m_vecMosterPosition;

};

#endif // !defined(AFX_INFMISSIONMAPVIEWCTRL_H__1526ABC7_38E3_48E8_A354_6834D65EB0EF__INCLUDED_)
