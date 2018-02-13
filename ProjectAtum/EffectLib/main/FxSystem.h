#pragma once

// FxSystem.h: interface for the CFxSystem class.
//////////////////////////////////////////////////////////////////////

#include "d3d9.h"
#include "SkinnedMesh.h"

// 2011-10-10 by jhahn EP4 트리거 시스템	화산재 이펙트 추가
#include <windows.h>
#include <commctrl.h>
#include <commdlg.h>
#include <basetsd.h>
#include <math.h>
#include <stdio.h>
#include <d3dx9.h>
//#include <dxerr9.h>
#include <tchar.h>

//-------------------------------------------------------------
// 정점구조체
//-------------------------------------------------------------
// 텍스처를 1장만 붙인다
typedef struct {
	FLOAT       p[4];
	FLOAT       tu, tv;
} TVERTEX;
// 텍스처를 3장 붙인다
typedef struct {
	FLOAT       p[4];
	FLOAT       t[3][2];
} T3VERTEX;
// 텍스처를 4장 붙인다
typedef struct {
	FLOAT       p[3];
	FLOAT       t[4][2];
} T4VERTEX;

#define SMALL_WIDTH		((g_pD3dApp->GetBackBufferDesc().Width )/8)
#define SMALL_HEIGHT	((g_pD3dApp->GetBackBufferDesc().Height)/8)
//end 2011-10-10 by jhahn EP4 트리거 시스템	화산재 이펙트 추가
class CFxSystem  
{
public:
	CFxSystem();
	virtual ~CFxSystem();
	void	InitDeviceObjects();
	void	RestoreDeviceObjects();
	void	InvalidateDeviceObjects();
	void	DeleteDeviceObjects();
	void	Tick(float fElapsedTime);
	void	BeginTargetSurface();
	void	EndTargetSurface();
	LPDIRECT3DTEXTURE9	GetTargetSurface() { return m_pTargetTexture[0]; }
	void	SetSourceAlpha(BYTE byAlpha) { m_byAlpha = byAlpha; }
	BYTE	GetSourceAlpha() { return m_byAlpha; }
	BOOL	IsRenderBlurFilter() { return m_bSupportBlurFilter; }
	BOOL	GetMetarSurface(){return m_bSupportMetal;}
	BOOL	GetEnvSurface(){return m_bSupportEvn;}
	
	void	MakeTargetSurface();
	void	DrawTargetSurface();
	void	MakeGaussFilterSurface();
	void	DrawGaussFilterSurface();
// 2011-10-10 by jhahn EP4 트리거 시스템	화산재 이펙트 추가
	void	MakeSandStromSurface();
	void	SetingSandStromSurface( BOOL Setstorm){m_bStormShader = Setstorm;}
	BOOL	GetingSandStromSurface(){return m_bStormShader;}
	void	SetingMgamaSurface( BOOL Setmagma){m_bMagmaShader = Setmagma;}
	BOOL	GetingMgamaSurface(){return m_bMagmaShader;}
//end02 2011-10-10 by jhahn EP4 트리거 시스템	화산재 이펙트 추가

	

	// 메탈
	void	DrawMetalFilterBegin(D3DXMATRIX mWorld, D3DXMATRIX mView, D3DXMATRIX mProj, UINT ipattr, DWORD nType=0);
	void	SetMeshFrame(SMeshContainer *pMesh){m_pframeCur = pMesh;}
	
	// 환경광
	void	DrawEvnFilterBegin(D3DXMATRIX mWorld, D3DXMATRIX mView, D3DXMATRIX mProj, UINT ipattr, DWORD nType=0);

	BOOL	IsRenderGaussBlur()	{ return m_bSupportGaussBlur; }
// 2011-10-10 by jhahn EP4 트리거 시스템	화산재 이펙트 추가
	BOOL	IsRenderSandStromBlur(){ return TRUE; }
//end 2011-10-10 by jhahn EP4 트리거 시스템	화산재 이펙트 추가
	HRESULT CreatePSFromCompiledFile (LPDIRECT3DDEVICE9 pd3dDevice,TCHAR* strPSPath,LPDIRECT3DPIXELSHADER9& pPS);
	HRESULT CreateEffectFromCompiledFile(LPDIRECT3DDEVICE9 pd3dDevice,TCHAR* strFilePath,LPD3DXEFFECT& pEffect);

public:
	float				m_fEffectTime;

	LPDIRECT3DTEXTURE9	m_pTargetTexture[2];
	LPDIRECT3DSURFACE9	m_pTargetSurface[2];

	BYTE				m_byAlpha;
	BOOL				m_bSupportBlurFilter;
	SMeshContainer		*m_pframeCur;		// 메쉬 프레임

	// 가우스 블러 필터
	LPDIRECT3DTEXTURE9	m_pGaussFilterTexture[2];
	LPDIRECT3DSURFACE9	m_pGaussFilterSurface[2];

	// 셰이더
	LPD3DXEFFECT		    m_pEffect;		// 이펙트
	D3DXHANDLE				m_hTechnique;	// 테크닉
	D3DXHANDLE				m_hafWeight;	// 가중치 배열
	D3DXHANDLE				m_htSrcMap;		// 텍스처		
	DWORD					m_dwGaussBlurNum;
	BOOL					m_bUseGaussBlur;				// 가우스블러 사용중
	BOOL					m_bSupportGaussBlur;			// 정점쉐이더 1.1 + 픽셀쉐이더 1.1 이상 지원
	DWORD					m_dwShaderVersion;				// 1(1.1) 2(2.0)
// 2011-10-10 by jhahn EP4 트리거 시스템	화산재 이펙트 추가
	BOOL					m_bSupportFocusBlur;			// 정점쉐이더 1.1 + 픽셀쉐이더 1.1 이상 지원


	//----------------------------------------------------------------------------------------------------------------------------------
	LPD3DXEFFECT		    m_pEffect2;		// 이펙트
	D3DXHANDLE				m_hTechnique2;	// 테크닉
	D3DXHANDLE				m_hafWeight2;	// 가중치배열
	D3DXHANDLE				m_htSrcTex2;		// 텍스처
	D3DXHANDLE				m_htBlurTex2;	// 텍스처
	D3DXHANDLE				m_htBlendTex2;	// 텍스처
	D3DXHANDLE				m_htDummyTex2;

	
	LPDIRECT3DTEXTURE9		m_pTex;
	LPDIRECT3DTEXTURE9		m_pDummyTex;
	

	// 렌더링 타겟
	LPDIRECT3DSURFACE9		m_pMapZ2;		// 깊이버퍼
	LPDIRECT3DTEXTURE9		m_pOriginalTex2;	// 텍스처
	LPDIRECT3DSURFACE9		m_pOriginalSurf2;// 표면
	LPDIRECT3DTEXTURE9		m_pPostTex2[2];	// 텍스처
	LPDIRECT3DSURFACE9		m_pPostSurf2[2];	// 표면

	BOOL					m_bStormShader;
	BOOL					m_bMagmaShader;
	//----------------------------------------------------------------------------------------------------------------------------------
//end 2011-10-10 by jhahn EP4 트리거 시스템	화산재 이펙트 추가
	// 셰이더 (금속광)
	LPD3DXEFFECT		    m_pEffectMetal;		// 이펙트
	D3DXHANDLE				m_hTechniqueMetal;	// 테크닉
	D3DXHANDLE				m_hmWVP;			// 로컬-투영변환행렬
	D3DXHANDLE				m_hmW;				// 로컬-월드변환행렬
	D3DXHANDLE				m_hvLightDir;		// 광원방향
	D3DXHANDLE				m_hvColor;			// 정점색
	D3DXHANDLE				m_hvEyePos;			// 시점의 위치	
	BOOL					m_bSupportMetal;	// 사용 정의

	// 셰이더 (환경)
	LPD3DXEFFECT			m_pEffectEvn;
    LPDIRECT3DTEXTURE9		m_pSphereMap;
	BOOL					m_bSupportEvn;	
};
