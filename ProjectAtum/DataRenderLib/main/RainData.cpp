// RainData.cpp: implementation of the CRainData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RainData.h"
#include "AtumApplication.h"
#include "AppEffectData.h"
#include "Background.h"
#include "Camera.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRainData::CRainData(D3DXVECTOR3 vPos, float fVelRate,D3DXVECTOR3 vVel)
{
	FLOG( "CRainData(D3DXVECTOR3 vPos, float fVelRate,D3DXVECTOR3 vVel)" );
	m_vPos = vPos;
	m_fVelRate = fVelRate;
	m_vVel = vVel;
	D3DXVECTOR3 vSide;
	vSide = D3DXVECTOR3(1,0,0);
	D3DXVec3Cross(&m_vUp, &vSide, &m_vVel);
	D3DXVec3Normalize(&m_vUp,&m_vUp);

	D3DXMatrixLookAtRH( &m_mMatrix, &(m_vPos), &(m_vPos + m_vVel), &m_vUp);
	D3DXMatrixInverse( &m_mMatrix, NULL, &m_mMatrix );

}

CRainData::~CRainData()
{
	FLOG( "~CRainData()" );

}

void CRainData::Tick()
{
	FLOG( "CRainData::Tick()" );
	float fElapsedTime = g_pD3dApp->GetElapsedTime();
	D3DXVECTOR3 dir,v;
	dir = D3DXVECTOR3(0,-1,0);
	m_fVelRate += 10.0f*fElapsedTime;
	m_vVel.y -= 10.0f*fElapsedTime;
	D3DXVec3Normalize(&m_vVel,&m_vVel);
	m_vPos += m_fVelRate*m_vVel*fElapsedTime;
	D3DXMatrixLookAtRH( &m_mMatrix, &(m_vPos), &(g_pD3dApp->m_pCamera->GetEyePt()), &g_pD3dApp->m_pCamera->GetUpVec());
	D3DXMatrixInverse( &m_mMatrix, NULL, &m_mMatrix );

	float fDist;
	int i = (int)(m_vPos.x/TILE_SIZE);
	int j = (int)(m_vPos.z/TILE_SIZE);
	if(i >= 0 && i < g_pGround->m_projectInfo.sXSize && j >= 0 && j < g_pGround->m_projectInfo.sYSize)
	{
		if(g_pGround->IntersectTriangle(m_vPos,dir,g_pGround->m_pTileVertexArray[i*(g_pGround->m_projectInfo.sYSize+1)+j].pos,
			g_pGround->m_pTileVertexArray[i*(g_pGround->m_projectInfo.sYSize+1)+(j+1)].pos,
			g_pGround->m_pTileVertexArray[(i+1)*(g_pGround->m_projectInfo.sYSize+1)+(j)].pos,
			&fDist))
		{
			v = m_vPos + dir*fDist;
		}
		else if(g_pGround->IntersectTriangle(m_vPos,dir,g_pGround->m_pTileVertexArray[i*(g_pGround->m_projectInfo.sYSize+1)+j+1].pos,
			g_pGround->m_pTileVertexArray[(i+1)*(g_pGround->m_projectInfo.sYSize+1)+(j)].pos,
			g_pGround->m_pTileVertexArray[(i+1)*(g_pGround->m_projectInfo.sYSize+1)+j+1].pos,
			&fDist))
		{
			v = m_vPos + dir*fDist;
		}
	}
	else
	{
		m_bUsing = FALSE;
	}
	if(v.y >= m_vPos.y + 10.0f)
	{
		m_bUsing = FALSE;
		CAppEffectData * pData = new CAppEffectData(NULL,RC_EFF_COLL_01,v);
		pData = (CAppEffectData *)g_pD3dApp->m_pEffectList->AddChild(pData);
	}
}
