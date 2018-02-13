// Background.h: interface for the CBackground class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BACKGROUND_H__7699F556_5513_4782_B433_E68C5F32BBC0__INCLUDED_)
#define AFX_BACKGROUND_H__7699F556_5513_4782_B433_E68C5F32BBC0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "StdAfx.h"
#include "AtumNode.h"

class CGameData;
class CINFImage;
class CQuadGround;
class CSkinnedMesh;
class DataHeader;
class CINFImageEx;		// 2011. 10. 10 by jskim UI시스템 변경

class CBackground : public CAtumNode  
{
public:
	CBackground(PROJECTINFO info);//,int n);
	virtual ~CBackground();
	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	virtual void Render();
	virtual void Tick(float fElapsedTime);
	BOOL IntersectTriangle( const D3DXVECTOR3& orig,
                                       const D3DXVECTOR3& dir, D3DXVECTOR3& v0,
                                       D3DXVECTOR3& v1, D3DXVECTOR3& v2,
                                       FLOAT* t);//, FLOAT* u, FLOAT* v );
	float CheckHeightMap(D3DXVECTOR3 vPos);
	void EffectTick(EFFECTINFOCLIENT * info);

	//void LoadMapFile();

	void GetBlockAdjacentToPosition(float x, float z, float fRadius, TWO_BLOCK_INDEXES &blockIdx);
	void CheckCollMap(D3DXVECTOR3 vPos, float *fHeight, D3DXVECTOR3 * vNor);
	
public:
	D3DLIGHT9					m_lightSupport[4];				// 맵의 보조 라이트
	D3DMATERIAL9				m_mtrl;
	D3DXMATRIX					m_mat;
	TILEINFOCLIENT*				m_pTileInfo;				// 렌더링시 참조하는 타일번호, N x M 개 
	BOOL		*				m_bTileRender;
	VERTEXINFO*					m_pTileVertexArray;
//	LPDIRECT3DVERTEXBUFFER8	*	m_pVBRender;				//[m_projectInfo.m_sYSize];
	INT							m_nCreateTexNumber;
	int							m_nObjectNumber;
	FLOAT						m_fCheckWaterTime;
	MapInfo						m_info;
	MAP_INFO *					m_pMapInfo;

	PROJECTINFO					m_projectInfo;
	CGameData	*				m_pBackgroundData;						// 데이타파일 (맵파일)
	CGameData	*				m_pTextureData;				// Texture ( 512x512, 16장)
	D3DXMATRIX					m_mMatrix;

	BOOL						m_bBazaar;					// 2006-07-21 by ispark, 바잡맵인지
	////////////////////////////////////////////////////////
	CAtumNode	**				m_ppObjectList;
	CAtumNode	*				m_pBigObject;				// 큰 오브젝트 데이타
	CAtumNode	*				m_pWaterObject;				// 물 오브젝트 
	CAtumNode	*				m_pObjectEvent;				// 이벤트 오브젝트 데이타
	CAtumNode	*				m_pObjectMonster;			// 오브젝트 몬스터 데이타

//	CINFImage	*				m_pMinimap;
//	CSkinnedMesh*				m_pMeshMinimap;				// 2005-09-12 by ispark 별도 처리
	/////////////// Water Render //////////////////////
	LPDIRECT3DTEXTURE9			m_pWaterTexture[WATER_TEXTURE_COUNT];			// Water Texture
	LPDIRECT3DVERTEXBUFFER9	*	m_pVBWater;
	WATERINFO		*			m_bWaterRender;
	FLOAT						m_fCheckWaterTexAniTime;
	BYTE						m_bWaterTexCont;

	CQuadGround		*			m_pQuad;						// 쿼드 트리

	map<INT,EFFECTINFOCLIENT *>			m_mapEffectList;		// 구분 맵 인덱스 * (00~99) <- 이펙트 수(렌더링용)
	vector<MONSTERINFO *>				m_vecMonsterResList;	// 구분 맵 인덱스 * (00~99) <- 이펙트 수(렌더링용)


	///////////////////////////////////////////////////////////////////////////////
	// 2004-06-04, cmkwon, Object Render를 위해
	
	float						m_fSizeMap_X;				// 맵의 X축 Size로 단위는 단위벡터임
	float						m_fSizeMap_Z;				// 맵의 Z축 Size로 단위는 단위벡터임
	int							m_NUM_BLOCK_X;
	int							m_NUM_BLOCK_Z;

	//////////////////////////////////////////////////////////////////////////
	// 2005-09-06 by ispark 미니맵
	BOOL						m_bMiniMap;
	// 2011. 10. 10 by jskim UI시스템 변경
	CINFImageEx*				m_pMiniBigMapImage;
	CINFImageEx*				m_pMiniBigMapCityImage;			// 2006-04-08 by ispark, 도시 전용 이미지
	// end 2011. 10. 10 by jskim UI시스템 변경
	DataHeader*					m_pMiniMapDataHeader;
	DataHeader*					m_pMiniMapCityDataHeader;

	int							m_nMapNumber;					// 맵번호

	//////////////////////////////////////////////////////////////////////////
	LPDIRECT3DTEXTURE9			m_pDetailMap;					// 디테일


	// 2008. 12. 11 by ckPark 물렌더링
	LPDIRECT3DVERTEXBUFFER9		m_pToRenderWaterVB;				// 최종적으로 뭉쳐서 그릴 물 버텍스 버퍼
	// end 2008. 12. 11 by ckPark 물렌더링
};

#endif // !defined(AFX_BACKGROUND_H__7699F556_5513_4782_B433_E68C5F32BBC0__INCLUDED_)
