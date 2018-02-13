// INFGameMainMiniMap.h: interface for the CINFGameMainMiniMap class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFGAMEMAINMINIMAP_H__964B7FEB_C0C4_4693_A01A_563E957C1DD4__INCLUDED_)
#define AFX_INFGAMEMAINMINIMAP_H__964B7FEB_C0C4_4693_A01A_563E957C1DD4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INFBase.h"

typedef struct
{	
	int			nQuestIndex;
	int			nMonsterPosX;
	int			nMonsterPosY;
	char		sQuestMonsterName[64];
} MiniMap_PosData;


class CAtumNode;
class CINFImage;
class CD3DHanFont;
class CSkinnedMesh;
class CINFImageBtn;	// 2008-11-3 by bhsohn 서버 시간 보기 버튼 추가
//class CItemData;
class CINFImageEx;
class CINFGroupImage;

class CINFGameMainMiniMap : public CINFBase  
{
public:
	CINFGameMainMiniMap(CAtumNode* pParent);
	virtual ~CINFGameMainMiniMap();
	
	// 2008-11-3 by bhsohn 서버 시간 보기 버튼 추가
	//void SetShowState(BOOL bState) { m_bShowState = bState; }	
	//BOOL GetShowState() { return m_bShowState; }
	void SetZoomState(BYTE bState) { m_bZoomState = bState; }
	BYTE IsRenderBigMap() { return m_bZoomState; }

	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	void Render();
	void RenderBigMap();
	void RenderSmallMap();
	void RenderMap();
	void RenderEnemy();
	void RenderMonster();
	void RenderOperation();
	void SetMonsterMiniMap();
	void InitMapSet();									// 2007-02-22 by dgwoo 맵셋팅.
	virtual void Tick();
	virtual int WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
	void OnButtonClicked(int nButton);
	
protected:
	void CheckRadar();										
	void TickRadarMap();										// 2005-09-29 by ispark
	void RenderRadarMap();
	HRESULT InitRadarMap();	
	void GetMiniBigMapCityPos(D3DXVECTOR3 vPos, float * fPosx, float * fPosy);	// 2006-05-04 by ispark
	void GetMiniBigMapPos(D3DXVECTOR3 vPos, float * fPosx, float * fPosy);		// 2006-09-04 by ispark
	void GetMiniSmallMapCityPos(D3DXVECTOR3 vPos, float * fPosx, float * fPosy);
	void GetMiniSmallMapPos(D3DXVECTOR3 vPos, float * fPosx, float * fPosy);	

	// 2008-11-3 by bhsohn 서버 시간 보기 버튼 추가
	void OnClickServerTime();

protected:
	BOOL			m_bRestored;
//	CSkinnedMesh*	m_pMeshUnit;							// 2005-09-13 by ispark 별도 처리
//	CSkinnedMesh*	m_pMeshParty;							// 2005-09-13 by ispark 별도 처리
//	CSkinnedMesh*	m_pMeshArea;							// 2005-09-13 by ispark 별도 처리

	CINFImageEx*		m_pBackImage;
	CINFImageEx*		m_pSiteImage;
	CINFImageEx*		m_pPlayerImage;
	CINFImageEx*		m_pUserImage[3];
	CINFImageEx*		m_pPartyImage[3];
	CINFImageEx*		m_pMonsterImage[3];
	CINFImageEx*		m_pInfluenceImage[3];
	CINFImageEx*		m_pGuildImage[3];// 2012-04-09 by isshin 레이더 및 미니맵 파티,여단원 표시 - 레이다 여단원

#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	CINFGroupImage*		m_pMiniMapBK;
	CINFGroupImage*		m_pBigMapBK;
	//CINFImage*		m_pButtonZoom[3][BUTTON_STATE_NUMBER];
	// 2008-11-3 by bhsohn 서버 시간 보기 버튼 추가
	//CINFImage*		m_pButtonShowHide[2][BUTTON_STATE_NUMBER];
	//CINFImageBtn*		m_pServerTimeBtn;														  
#else	  														  
      CINFImage*		m_pButtonZoom[3][BUTTON_STATE_NUMBER];
	CINFImageBtn*		m_pServerTimeBtn;
#endif

	CINFImageEx*		m_pDeadImage;
	CINFImageEx*		m_pLockonImage;
	CINFImageEx*		m_pImgStateWarning;						// 2005-07-08 by ispark // 유닛 미사일 경교
	CINFImageEx*		m_pMiniMapFrame;						// 미니맵 틀
	CINFImageEx*		m_pMiniMapUnitImage;					// 기어
	CINFImageEx*		m_pMiniMapPartyImage;					// 파티 이미지
	CINFImageEx*		m_pMiniMapGuildImage;					// 2012-04-09 by isshin 레이더 및 미니맵 파티,여단원 표시 - 미니맵 여단원
	CINFImageEx*		m_pArenaMapInfImg;						// 아레나 맵에서 같은 세력을 표시해줄때.
	CINFImageEx*		m_pImgQuestTarget;						// 2007-02-26 by dgwoo 원표시.
	
	CD3DHanFont*	m_pFontPosition;						// 파티원들의 이름.
	CD3DHanFont*	m_pFontQuestTarget;						// 퀘스트 타겟의 이름.
	//vector<CD3DHanFont*>	m_vecQuestPosition;

	// 2008-11-3 by bhsohn 서버 시간 보기 버튼 추가
	//int				m_nButtonState[2];	// zoom, show
	int				m_nZoomButtonState;		// zoom
	//BOOL			m_bShowState;		// SHOW(TRUE), HIDE(FALSE)
	// end 2008-11-3 by bhsohn 서버 시간 보기 버튼 추가

	BYTE			m_bZoomState;		// 0: HIDE 1: BIGMAP 2: SMALLMAP
	//BOOL			m_bShowMap;			// 미니맵 Show Hide
	D3DXMATRIX		m_matRotate;		// 유닛이 Z축에 기울어진 각에 대한 매트릭스
	float			m_fAngle;			// 유닛이 기울어진 각도
	float			m_fRadarBackAngle;	// 레이더 본체 각도
	float			m_fCurrentTime;

	BOOL			m_bMissileWarningRender;// 미사일 경고 화면 렌더링
	float			m_fMissileWarningTime;	// 미사일 경고 화면 렌더링 시간
	
	float			m_fRange;				// 2005-08-16 by ispark 레이더 물체감지범위
	vector<MiniMap_PosData*> m_vecMosterPosition;

	CINFImageEx*	m_pRadarMiniMap;
	BYTE			m_byAlphaMiniMap[118][118];

	DataHeader*		m_MiniMapSubDataHeader[4];

	BOOL			m_bMiniMapState;		// 2006-09-04 by ispark, 현 유저상태에 따라 미니맵 유,무 확인
	int				m_nTestMapPosX;
	int				m_nTestMapPosY;
};

#endif // !defined(AFX_INFGAMEMAINMINIMAP_H__964B7FEB_C0C4_4693_A01A_563E957C1DD4__INCLUDED_)
