// SunData.cpp: implementation of the CSunData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SunData.h"
#include "SunRender.h"
#include "CharacterInfo.h"
#include "AtumApplication.h"
#include "ParticleSystem.h"
#include "Camera.h"
#include "SceneData.h"
#include "ShuttleChild.h"
#include "CharacterChild.h"				// 2005-07-21 by ispark
#include "ObjectAniData.h"
#include "SpriteAniData.h"
#include "ObjRender.h"
#include "Background.h"
#include "ObjectChild.h"
#include "dxutil.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSunData::CSunData()
{
	FLOG( "CSunData()" );
	m_pCharacterInfo = NULL;
	m_pRenderer = g_pScene->m_pSunRender;
	m_vPos = g_pShuttleChild->m_vPos - 300.0f*g_pScene->m_light0.Direction;
//	m_vCenterPos = D3DXVECTOR3(0,0,0);
	int i;
	for(i=0;i<10;i++)
		m_vLensPos[i] = D3DXVECTOR3(0,0,0);
	m_fMaterialValue = 0.5f;

	D3DXVECTOR3 vUp = D3DXVECTOR3(0,1,0);
	D3DXVECTOR3 vVel = D3DXVECTOR3(1,0,0);
	
	D3DXMatrixLookAtRH( &m_mMatrix, &(m_vPos), &(m_vPos + vVel), &vUp);
	D3DXMatrixInverse( &m_mMatrix, NULL, &m_mMatrix );

	char buf[256];
	wsprintf(buf,"%08d",RC_EFF_SUN);
	LoadCharacterEffect(buf);
	m_bodyCondition = BODYCON_FLY_MASK|BODYCON_DEAD_MASK;
	if(m_pCharacterInfo)
	{
		m_pCharacterInfo->SetAllBodyConditionMatrix(m_mMatrix);
		m_pCharacterInfo->ChangeBodyCondition(m_bodyCondition);
	}
	m_fScale[0] = 8.0f;
	m_fScale[1] = 13.0f;
	m_fScale[2] = 4.0f;
	m_fScale[3] = 2.0f;
	m_fScale[4] = 9.5f;
	m_fScale[5] = 6.0f;
	m_fScale[6] = 4.0f;
	m_fScale[7] = 14.0f;
	m_fScale[8] = 6.0f;
	m_fScale[9] = 3.0f;

	m_bTexture[0] = 2;
	m_bTexture[1] = 1;
	m_bTexture[2] = 3;
	m_bTexture[3] = 2;
	m_bTexture[4] = 2;
	m_bTexture[5] = 1;
	m_bTexture[6] = 0;
	m_bTexture[7] = 4;
	m_bTexture[8] = 3;
	m_bTexture[9] = 0;
}

CSunData::~CSunData()
{
	FLOG( "~CSunData()" );
	if(m_pCharacterInfo)//추가
	{
		m_pCharacterInfo->InvalidateDeviceObjects();
		m_pCharacterInfo->DeleteDeviceObjects();
		util::del(m_pCharacterInfo);
	}
}

void CSunData::Tick()
{
	FLOG( "CSunData::Tick()" );
	float fElapsedTime = g_pD3dApp->GetElapsedTime();
	float fFrame = 0.0f;
	m_vPos = g_pD3dApp->m_pCamera->GetEyePt() - SUN_DISTANCE *g_pScene->m_light0.Direction;
//	m_vPos = g_pD3dApp->m_pCamera->GetEyePt() - 300.0f*g_pScene->m_light0.Direction;
	g_pD3dApp->CalcObjectSourceScreenCoords(m_vPos, g_pD3dApp->GetBackBufferDesc().Width, g_pD3dApp->GetBackBufferDesc().Height, 
		m_nObjScreenX, m_nObjScreenY, m_nObjScreenW );
	int nCheckWidth = g_pD3dApp->GetBackBufferDesc().Width+300;
	int nCheckHeight = g_pD3dApp->GetBackBufferDesc().Height+300;
	if(m_nObjScreenX > -300 && m_nObjScreenX < nCheckWidth && m_nObjScreenY > -300 && m_nObjScreenY < nCheckHeight && m_nObjScreenW > 0)
	{
/*
		// 렌즈 플레어의 센터 좌표(스크린상의 센터를 3D좌표로 만든다)
		m_vCenterPos = g_pShuttleChild->m_vPos;
		// 렌즈 플레어를 그릴 좌표를 구한다 - 3D 방식으로 구해보자
		D3DXVECTOR3 vTemp,vTemp2;
		D3DXVec3Normalize(&vTemp,&(m_vPos-g_pShuttleChild->m_vPos));
		for(int i=0;i<10;i++)
		{
			vTemp2 = g_pShuttleChild->m_vPos + 10.0f*(i*i-3)*vTemp - 5.0f*vTemp;
			D3DXVec3Normalize(&vTemp2,&(vTemp2 - g_pD3dApp->m_pCamera->GetEyePt()));
			m_vLensPos[i] = g_pD3dApp->m_pCamera->GetEyePt() + 80.0f*vTemp2;
		}
*/
/*
		D3DXVECTOR2 vSunPos,vCenterPos, vStartPos, vEndPos;
		vSunPos = D3DXVECTOR2(m_nObjScreenX,m_nObjScreenY);
		vCenterPos = D3DXVECTOR2(g_pShuttleChild->m_nObjScreenX,g_pShuttleChild->m_nObjScreenY);
//		float fLength = D3DXVec2Length(&(vSunPos - vCenterPos));
		vStartPos = vCenterPos + 1.2f*(vSunPos - vCenterPos);
		vEndPos = vSunPos + 1.5f*(vCenterPos - vSunPos);
		for(int i=0;i<10;i++)
		{
			m_vLensPos2[i] = vStartPos + (i*(vEndPos - vStartPos))/10.0f;
		}
*/		

		D3DXVECTOR3 vSunPos,vCenterPos, vStartPos, vEndPos,vVel;
//		vSunPos = m_vPos;
		vSunPos = g_pD3dApp->m_pCamera->GetEyePt() - 300.0f*g_pScene->m_light0.Direction;
//		vCenterPos = g_pShuttleChild->m_vPos;
		D3DXVec3Normalize(&vVel,&(g_pD3dApp->m_pCamera->GetViewDir()));
		vCenterPos = g_pD3dApp->m_pCamera->GetEyePt() + 200.0f*vVel;
		vStartPos = vCenterPos + 1.0f*(vSunPos - vCenterPos);
		vEndPos = vSunPos + 1.7f*(vCenterPos - vSunPos);
		for(int i=0;i<10;i++)
		{
			m_vLensPos[i] = vStartPos + (i*(vEndPos - vStartPos))/10.0f;
			D3DXVec3Normalize(&vVel,&(m_vLensPos[i] - g_pD3dApp->m_pCamera->GetEyePt()));
			m_vLensPos[i] = g_pD3dApp->m_pCamera->GetEyePt() + 80.0f*vVel;
		}


//		int x, y, w;
//		g_pD3dApp->CalcObjectSourceScreenCoords(g_pShuttleChild->m_vPos, g_pD3dApp->GetBackBufferDesc().Width, g_pD3dApp->GetBackBufferDesc().Height, 
//			x, y, w );
		D3DXVECTOR2 vPos1 = D3DXVECTOR2(m_nObjScreenX,m_nObjScreenY);
		D3DXVECTOR2 vPos2 = D3DXVECTOR2(g_pShuttleChild->m_nObjScreenX,g_pShuttleChild->m_nObjScreenY);
		D3DXVECTOR2 vPos3 = D3DXVECTOR2(g_pD3dApp->GetBackBufferDesc().Width/2.0f,g_pD3dApp->GetBackBufferDesc().Height/2.0f);
		float fLength = D3DXVec2Length(&(vPos1-vPos2));
		float fRate = sqrt((g_pD3dApp->GetBackBufferDesc().Width/2.0f)*(g_pD3dApp->GetBackBufferDesc().Width/2.0f) +
				(g_pD3dApp->GetBackBufferDesc().Height/2.0f)*(g_pD3dApp->GetBackBufferDesc().Height/2.0f));
		if(fLength == 0.0f)
			m_fMaterialValue = 0.5f;
		else
		{
//			m_fMaterialValue = 1.0f/((g_pD3dApp->GetBackBufferDesc().Width/2.0f)/fLength);
			m_fMaterialValue = 0.5f - 0.5f*(fLength/fRate);
			if(m_fMaterialValue > 0.5f)
				m_fMaterialValue = 0.5f;
			else if(m_fMaterialValue < 0.0f)
				m_fMaterialValue = 0.0f;
		}
		fFrame = D3DXVec2Length(&(vPos1 - vPos3));
		if(fFrame > fRate)
			fFrame = 0.0f;
		else if(fFrame <= 0.0f)
		{
			fFrame = 100.0f;
		}
		else
		{
			fFrame = 100.0f - (fFrame / fRate)*100.0f;
			if(fFrame > 100.0f)
				fFrame = 100.0f;
		}
		if(fFrame < 30.0f)
			fFrame = 30.0f;
	}

	m_mMatrix._41 = m_vPos.x;
	m_mMatrix._42 = m_vPos.y;
	m_mMatrix._43 = m_vPos.z;
	
	if(m_pCharacterInfo)
	{
		if(g_pScene->m_bNight)
		{
			if(!COMPARE_BODYCON_BIT(m_bodyCondition,BODYCON_LANDED_MASK))
			{
				m_bodyCondition = BODYCON_LANDED_MASK;
				m_pCharacterInfo->ChangeBodyCondition(m_bodyCondition);
			}
		}
		else
		{
			if(!COMPARE_BODYCON_BIT(m_bodyCondition,BODYCON_FLY_MASK|BODYCON_DEAD_MASK))
			{
				m_bodyCondition = BODYCON_FLY_MASK|BODYCON_DEAD_MASK;
				m_pCharacterInfo->ChangeBodyCondition(m_bodyCondition);
			}
		}
		m_pCharacterInfo->SetAllBodyConditionMatrix(m_mMatrix );

//		m_pCharacterInfo->SetSingleBodyConditionMatrix(m_bodyCondition, m_mMatrix);
		D3DXVECTOR3 vBackLightPos = g_pD3dApp->m_pCamera->GetEyePt() - (SUN_DISTANCE - 100.0f) *g_pScene->m_light0.Direction;
		m_mMatrix._41 = vBackLightPos.x;
		m_mMatrix._42 = vBackLightPos.y;
		m_mMatrix._43 = vBackLightPos.z;
		m_pCharacterInfo->SetSingleBodyConditionMatrix(BODYCON_DEAD_MASK, m_mMatrix);	
		m_pCharacterInfo->Tick( fElapsedTime );

		set<BodyCond_t>::iterator it = m_pCharacterInfo->m_vecCurrentBodyCondition.begin();
		while(it != m_pCharacterInfo->m_vecCurrentBodyCondition.end())
		{
			BodyCond_t hyBody = *it;
			map<BodyCond_t,CBodyConditionInfo*>::iterator itBody = m_pCharacterInfo->m_mapBodyCondition.find(hyBody);
			if(itBody != m_pCharacterInfo->m_mapBodyCondition.end())
			{
				CBodyConditionInfo* pBodyInfo = itBody->second;
				vector<CEffectInfo*>::iterator itEffect = pBodyInfo->m_vecEffect.begin();
				while(itEffect != pBodyInfo->m_vecEffect.end())
				{
					CEffectInfo* pEffectInfo = *itEffect;
					switch(pEffectInfo->m_nEffectType)
					{
					case EFFECT_TYPE_OBJECT:
						{
							((CObjectAni*)pEffectInfo->m_pEffect)->m_fCurrentObjectAniTime = fFrame;
						}
						break;
					case EFFECT_TYPE_SPRITE:
						{
							((CSpriteAni*)pEffectInfo->m_pEffect)->m_fCurrentTick = fFrame;
						}
						break;
					case EFFECT_TYPE_PARTICLE:
						{
							((CParticleSystem*)pEffectInfo->m_pEffect)->m_fCurrentTick = fFrame;
						}
						break;
					}
					itEffect++;
				}
			}
			it++;
		}
	}
}
/*
void CSunData::Render()
{
	if(m_nObjScreenX > -300 && m_nObjScreenX < g_pD3dApp->GetBackBufferDesc().Width+300 
		&& m_nObjScreenY > -300 && m_nObjScreenY < g_pD3dApp->GetBackBufferDesc().Height+300 && m_nObjScreenW > 0)
	{
		m_pRenderer->Render(this);
	}
}
*/
void CSunData::RenderRens()
{
	FLOG( "CSunData::RenderRens()" );
	int nCheckWidth = g_pD3dApp->GetBackBufferDesc().Width+300;
	int nCheckHeight = g_pD3dApp->GetBackBufferDesc().Height+300;
	if(m_nObjScreenX > -300 && 
		m_nObjScreenX < nCheckWidth && 
		m_nObjScreenY > -300 && 
		m_nObjScreenY < nCheckHeight && 
		m_nObjScreenW > 0 &&
		m_pRenderer)
	{
		D3DXMATRIX mat;
		D3DXVECTOR3 vPos = g_pCamera->GetEyePt();
		D3DXVECTOR3 vVel = m_vPos - g_pCamera->GetEyePt();
		D3DXVECTOR3 vSide = g_pCamera->GetCross();
		D3DXVECTOR3 vUp;
		D3DXVec3Cross(&vUp,&vVel,&vSide);		
		D3DXMatrixLookAtLH(&mat,&vPos,&(vPos + vVel),&vUp);
		float fDist = CheckCollMesh(mat,vPos);
		if(fDist == 10000)
		{
			float fTempDistance = 0.0f;
			D3DXVECTOR3 vTempPos,vEyePos,vEyeVel;
			vEyePos = g_pCamera->GetEyePt();
			vEyeVel = g_pScene->m_pSunData->m_vPos - vEyePos;
			D3DXVec3Normalize(&vEyeVel,&vEyeVel);
			BOOL bIsRenderRens = TRUE;
			int nMapSizeX = (int)g_pGround->m_projectInfo.sXSize*TILE_SIZE;
			int nMapSizeY = (int)g_pGround->m_projectInfo.sYSize*TILE_SIZE;
			while (fTempDistance < SUN_DISTANCE )	// 지형 및 물과 충돌 검사
			{
				vTempPos = vEyePos + vEyeVel * fTempDistance;
				if(	vTempPos.x >= 0 && 
					vTempPos.x < nMapSizeX && 
					vTempPos.z >= 0 && 
					vTempPos.z < nMapSizeY &&
					g_pGround->CheckHeightMap(vTempPos) > vTempPos.y)
				{	// 지형 및 물과 충돌 했다.
					bIsRenderRens = FALSE;
					break;
				}
				fTempDistance += TILE_SIZE;
			}
			if(bIsRenderRens)
		{
		m_pRenderer->RenderRens(this);
		}
	}
	}
	else
	{
		int temp = 0;
	}
}

void CSunData::LoadCharacterEffect(char* strFileName)
{
	FLOG( "CSunData::LoadCharacterEffect(char* strFileName)" );
	if(m_pCharacterInfo)
	{
		m_pCharacterInfo->InvalidateDeviceObjects();
		m_pCharacterInfo->DeleteDeviceObjects();
		util::del(m_pCharacterInfo);
	}
	m_pCharacterInfo = new CCharacterInfo();
	if(m_pCharacterInfo->Load(strFileName))
	{
		m_pCharacterInfo->InitDeviceObjects();
		m_pCharacterInfo->RestoreDeviceObjects();
	}
	else
	{
		util::del(m_pCharacterInfo);
		DBGOUT("CSunData::LoadCharacterEffect : Can't Find Effect File.\n");
	}

}

FLOAT CSunData::CheckCollMesh(D3DXMATRIX mat,
								   D3DXVECTOR3 vPos)
{
	float fDist = 10000.0f,fDistResult;
	vectorCObjectChildPtr::iterator itObj(g_pScene->m_vectorCulledObjectPtrList.begin());
	while(itObj != g_pScene->m_vectorCulledObjectPtrList.end())
	{
		CObjectChild * pObject = *itObj;
		if(pObject && pObject->m_pObjectInfo && pObject->m_pObjMesh)
		{
			pObject->m_pObjMesh->Tick(pObject->m_fCurrentTime);
			pObject->m_pObjMesh->SetWorldMatrix(pObject->m_mMatrix);
			fDistResult = pObject->m_pObjMesh->CheckCollision(mat,vPos,DEFAULT_COLLISION_DISTANCE,TRUE,FALSE).fDist;
			if(fDist > fDistResult)
				fDist = fDistResult;
		}
		itObj++;
	}
	return fDist;
}