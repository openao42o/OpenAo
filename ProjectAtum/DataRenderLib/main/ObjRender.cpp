// ObjRender.cpp: implementation of the CObjRender class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AtumApplication.h"
#include "ObjRender.h"
#include "ObjectChild.h"
#include "SceneData.h"
#include "ShuttleChild.h"
#include "CharacterChild.h"				// 2005-07-21 by ispark
#include "AtumDatabase.h"
#include "Background.h"
#include "Camera.h"
#include "dxutil.h"
#include "INFGameMain.h"
#include "INFGameMainMiniMap.h"
#include "MonsterData.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CObjRender::CObjRender()
{

}

CObjRender::~CObjRender()
{
	map<int,CSkinnedMesh*>::iterator it = m_mapSkinnedMesh.begin();
	while( it != m_mapSkinnedMesh.end()) 
	{
		util::del(it->second);
		it++;
	}
	m_mapSkinnedMesh.clear();
}

void CObjRender::RenderShadow(CAtumNode * pNode)
{

}

void CObjRender::Render(CObjectChild * pObj)
{
	if(pObj->m_pObjMesh)
	{
		DWORD dwAlpha;
		g_pD3dDev->GetRenderState(D3DRS_ALPHABLENDENABLE,&dwAlpha);

		// 2005-01-05 by jschoi - 현재 블렌딩 메시 사용안함.
//		if(pObj->m_bCheckBlend)
//		{
//			g_pD3dDev->SetRenderState( D3DRS_ALPHABLENDENABLE,  TRUE );
//			g_pD3dDev->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_ONE );
//			g_pD3dDev->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );
//		}
//		else
//		{
//			g_pD3dDev->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );
//		}
		g_pD3dDev->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );

		if(pObj->m_pObjectInfo->Alpha)
		{
			g_pD3dDev->SetRenderState( D3DRS_ALPHATESTENABLE,  TRUE );
			
			g_pD3dDev->SetRenderState( D3DRS_ALPHAFUNC,  D3DCMP_GREATEREQUAL );
			g_pD3dDev->SetRenderState( D3DRS_ALPHAREF,  0x7F );

			g_pD3dDev->SetRenderState( D3DRS_ALPHABLENDENABLE,  TRUE );
			g_pD3dDev->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
			g_pD3dDev->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
		}
		else
		{
			g_pD3dDev->SetRenderState( D3DRS_ALPHATESTENABLE,  FALSE );
		}
		pObj->m_pObjMesh->Tick(pObj->m_fCurrentTime);
		pObj->m_pObjMesh->SetWorldMatrix(pObj->m_mMatrix);
		pObj->m_pObjMesh->m_bMaterial = FALSE;		// 임시 코드

		// 2005-01-05 by jschoi - 블렌딩메시 현재 사용안함.
//		pObj->m_pObjMesh->m_bCheckBlend = pObj->m_bCheckBlend;	// 임시로 블렌딩 제거 

		pObj->m_pObjMesh->m_bProgressiveMesh = false;
		pObj->m_pObjMesh->AnotherTexture(1);
		g_pD3dDev->SetRenderState( D3DRS_ZENABLE,  TRUE );
		g_pD3dDev->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE );
		g_pD3dDev->SetRenderState( D3DRS_ZENABLE,  TRUE );
		g_pD3dDev->SetRenderState( D3DRS_LIGHTING, TRUE );
		g_pD3dDev->SetRenderState( D3DRS_FOGENABLE,  IsFogEnableMap(g_pShuttleChild->m_myShuttleInfo.MapChannelIndex.MapIndex) );
//		g_pD3dDev->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_ONE );
//		g_pD3dDev->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );
//		g_pD3dDev->SetSamplerState(0,D3DSAMP_MINFILTER,D3DTEXF_LINEAR);
//		g_pD3dDev->SetSamplerState(0,D3DSAMP_MAGFILTER,D3DTEXF_LINEAR);
//		g_pD3dDev->SetSamplerState(0,D3DSAMP_MIPFILTER,D3DTEXF_LINEAR);
	
		pObj->m_pObjMesh->Render();	
		g_pD3dDev->SetRenderState( D3DRS_ALPHABLENDENABLE, dwAlpha );
	}
}
/*
void CObjRender::RenderShadow(CAtumNode * pNode)
{
	CObjectChild * pObj = (CObjectChild *)pNode;

	if(!pObj->m_pObjMesh->m_bTotalTextureNum > 1)
	{
		return;
	}

	D3DXMATRIX	matTemp;
	D3DXMatrixIdentity(&matTemp);
		g_pD3dDev->SetTexture( 0, pObj->m_pObjMesh->m_pTexture[pMonsterData->m_pMonMesh->m_bTotalTextureNum-1] );
		g_pD3dDev->SetTransform( D3DTS_WORLD, &matTemp );
		g_pD3dDev->SetVertexShader( D3DFVF_SPRITEVERTEX );
		g_pD3dDev->SetStreamSource( 0, pObj->m_pVBShadow, sizeof(SPRITEVERTEX) );
		g_pD3dDev->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2 );
}
*/
void CObjRender::Tick(float fElapsedTime)
{

}

HRESULT CObjRender::InitDeviceObjects()
{
	DataHeader*  pHeader;
	CGameData * pData = new CGameData;
	char strPath[256];
	int nCont = 0;
	wsprintf(strPath,".\\Res-Map\\omd.tex");
	if(pData->SetFile(strPath,FALSE, NULL,0))
	{
		char* p;
		wsprintf( strPath, "%04d",g_pShuttleChild->m_myShuttleInfo.MapChannelIndex.MapIndex );
		pHeader = pData->Find(strPath);
		if(pHeader)
		{
			p = pHeader->m_pData;
			p += 20;
			memcpy(&nCont,p,sizeof(int));
			p += sizeof(int);
			for(int i=0;i<nCont;i++)
			{
				int nObjType;
				memcpy(&nObjType,p,sizeof(int));
				
				MAPOBJECTINFO *pObject = g_pDatabase->CheckObjectInfo(nObjType);
				if(pObject && pObject->Code > 0)
				{
					// 2009. 11. 23 by jskim 리소스 로딩 구조 변경
					//InitData(pObject->Code);
					InitData(pObject->Code, _OBJECT_TYPE);
					//end 2009. 11. 23 by jskim 리소스 로딩 구조 변경
				}
				p += sizeof(int);
			}
			util::del(pData);
		}
		else
		{
			util::del(pData);
			return E_FAIL;
		}
	}
	else
	{
		util::del(pData);
		return E_FAIL;
	}
	return S_OK;
}

HRESULT CObjRender::RestoreDeviceObjects()
{
	map<int,CSkinnedMesh*>::iterator it = m_mapSkinnedMesh.begin();
	while(it != m_mapSkinnedMesh.end())
	{
		it->second->RestoreDeviceObjects();
		it++;
	}
	return S_OK;
}

HRESULT CObjRender::InvalidateDeviceObjects()
{
	map<int,CSkinnedMesh*>::iterator it = m_mapSkinnedMesh.begin();
	while(it != m_mapSkinnedMesh.end())
	{
		it->second->InvalidateDeviceObjects();
		it++;
	}
	return S_OK;
}

HRESULT CObjRender::DeleteDeviceObjects()
{
	map<int,CSkinnedMesh*>::iterator it = m_mapSkinnedMesh.begin();
	while(it != m_mapSkinnedMesh.end())
	{
		it->second->DeleteDeviceObjects();
		util::del(it->second);
		it++;
	}
	m_mapSkinnedMesh.clear();
	return S_OK;
}


//COLLISION_RESULT CObjRender::CheckCollMesh(D3DXMATRIX mat,
//								   BYTE bColl)
//{
//	COLLISION_RESULT collResult,checkcollResult;
//
//	//CMapObjectIterator itObj = g_pScene->m_mapObjectList.begin();
//	vectorCObjectChildPtr::iterator itObj(g_pScene->m_vectorCollisionObjectPtrList.begin());
//	while(itObj != g_pScene->m_vectorCollisionObjectPtrList.end())
//	{
//		CObjectChild * pObject = *itObj;
//		if(pObject)
//		{
//			if(bColl==0)
//			{	// 셔틀과 충돌 체크 거리
//				if(	/*pObject->m_bCheckColl && */
//					pObject->m_pObjectInfo && 
//					pObject->m_pObjectInfo->Collision && 
//					pObject->m_pObjMesh)
//				{
//					pObject->m_pObjMesh->Tick(pObject->m_fCurrentTime);
//					pObject->m_pObjMesh->SetWorldMatrix(pObject->m_mMatrix);
//					checkcollResult = pObject->m_pObjMesh->CheckCollision(mat,g_pShuttleChild->m_vPos,DEFAULT_COLLISION_DISTANCE,TRUE,FALSE);
//					if(collResult.fDist > checkcollResult.fDist)
//						collResult = checkcollResult;
//					// 충돌 검사를 했는지 판단하여 더이상 충돌 검출할 대상이 없을때는 그냥 리턴하게 된다.
////					g_pShuttleChild->m_bCollObjectCheck--;
////					if(g_pShuttleChild->m_bCollObjectCheck <= 0)
////						return collResult;
//				}
//			}
//			else if(bColl==1)
//			{	// 총알과 충돌 체크 거리
//				if(	/*pObject->m_bCheckAttack &&*/ 
//					pObject->m_pObjectInfo && 
//					pObject->m_pObjectInfo->Collision && 
//					pObject->m_pObjMesh)
//				{
//					if(pObject->m_bCheckAni)
//					{
//						// 애니메이션이 있는 오브젝트는 거리로 예측 시간을 구해서 애니를 적용시킨후 충돌체크를한다.
//						float fTime = D3DXVec3Length(&(g_pShuttleChild->m_vPos-pObject->m_vPos))/300.0f;
//						fTime = pObject->m_fCurrentTime + fTime*15.0f;
//						if(fTime > 500.0f)
//							fTime -= 500.0f;
//						pObject->m_pObjMesh->Tick(fTime);
//					}
//					else
//					{
//						pObject->m_pObjMesh->Tick(pObject->m_fCurrentTime);
//					}
//					pObject->m_pObjMesh->SetWorldMatrix(pObject->m_mMatrix);
//					checkcollResult = pObject->m_pObjMesh->CheckCollision(mat,DEFAULT_COLLISION_DISTANCE,TRUE,FALSE);
//					if(collResult.fDist > checkcollResult.fDist)
//						collResult = checkcollResult;
//					
////					g_pShuttleChild->m_bCollAttackCheck--;
////					if(g_pShuttleChild->m_bCollAttackCheck <= 0)
////						return collResult;
//				}
//			}
//		}
//		itObj++;
//	}
//	return collResult;
//}

COLLISION_RESULT CObjRender::CheckCollMesh(D3DXMATRIX mat,
								   D3DXVECTOR3 vPos)
{
	COLLISION_RESULT collResult,checkcollResult;

	vectorCObjectChildPtr::iterator itObj(g_pScene->m_vectorCollisionObjectPtrList.begin());
	// 사용함..
	while(itObj != g_pScene->m_vectorCollisionObjectPtrList.end())
	{
		CObjectChild * pObject = *itObj;
		if(pObject && pObject->m_pObjectInfo && pObject->m_pObjectInfo->Collision && pObject->m_pObjMesh)
		{
			float fRadius = pObject->m_pObjMesh->m_fRadius;
			if( D3DXVec3Length(&(pObject->m_vOriPos - vPos)) < fRadius * 2.0f)
			{
				pObject->m_pObjMesh->Tick(pObject->m_fCurrentTime);
				pObject->m_pObjMesh->SetWorldMatrix(pObject->m_mMatrix);
				checkcollResult = pObject->m_pObjMesh->CheckCollision(mat,vPos,DEFAULT_COLLISION_DISTANCE,TRUE,TRUE);
				if(collResult.fDist > checkcollResult.fDist)
					collResult = checkcollResult;
			}
		}
		itObj++;
	}
	
	return collResult;
}


COLLISION_RESULT CObjRender::CheckCollMeshWithOutWaterObject(D3DXMATRIX mat,
								   D3DXVECTOR3 vPos)
{
	COLLISION_RESULT collResult,checkcollResult;

	vectorCObjectChildPtr::iterator itObj(g_pScene->m_vectorCollisionObjectPtrList.begin());
	// 사용함..
	while(itObj != g_pScene->m_vectorCollisionObjectPtrList.end())
	{
		CObjectChild * pObject = *itObj;
		if(	pObject && 
			pObject->m_pObjectInfo && 
			pObject->m_pObjectInfo->ObjectRenderType != OBJECT_WATER && 
			pObject->m_pObjectInfo->Collision && 
			pObject->m_pObjMesh)
		{
			float fRadius = pObject->m_pObjMesh->m_fRadius;
			if( D3DXVec3Length(&(pObject->m_vOriPos - vPos)) < fRadius * 2.0f)
			{
				pObject->m_pObjMesh->Tick(pObject->m_fCurrentTime);
				pObject->m_pObjMesh->SetWorldMatrix(pObject->m_mMatrix);
				checkcollResult = pObject->m_pObjMesh->CheckCollision(mat,vPos,DEFAULT_COLLISION_DISTANCE,TRUE,FALSE);
				if(collResult.fDist > checkcollResult.fDist)
					collResult = checkcollResult;
			}
		}
		itObj++;
	}
	
	return collResult;
}


COLLISION_RESULT CObjRender::CheckCollMeshWaterObject(D3DXMATRIX mat,
								   D3DXVECTOR3 vPos)
{
	COLLISION_RESULT collResult,checkcollResult;

	vectorCObjectChildPtr::iterator itObj(g_pScene->m_vectorCollisionObjectPtrList.begin());
	// 사용함..
	while(itObj != g_pScene->m_vectorCollisionObjectPtrList.end())
	{
		CObjectChild * pObject = *itObj;
		if(	pObject && 
			pObject->m_pObjectInfo && 
			pObject->m_pObjectInfo->ObjectRenderType == OBJECT_WATER && 
			pObject->m_pObjectInfo->Collision && 
			pObject->m_pObjMesh)
		{
			float fRadius = pObject->m_pObjMesh->m_fRadius;
			if( D3DXVec3Length(&(pObject->m_vOriPos - vPos)) < fRadius)
			{
				pObject->m_pObjMesh->Tick(pObject->m_fCurrentTime);
				pObject->m_pObjMesh->SetWorldMatrix(pObject->m_mMatrix);
				checkcollResult = pObject->m_pObjMesh->CheckCollision(mat,vPos,DEFAULT_COLLISION_DISTANCE,TRUE,FALSE);
				if(collResult.fDist > checkcollResult.fDist)
					collResult = checkcollResult;
			}
		}
		itObj++;
	}
	
	return collResult;
}

COLLISION_RESULT CObjRender::CheckCollMeshRangeObject(D3DXMATRIX mat, D3DXVECTOR3 vPos, float fMovingDistance, CMonsterData* pTarget)
{
	COLLISION_RESULT collResult, checkcollResult;

	// 사용함..
	for (auto i = 0; i < g_pScene->m_vectorRangeObjectPtrList.size(); ++i)
	{
		CObjectChild * pObject = g_pScene->m_vectorRangeObjectPtrList[i];
		if (pObject && pObject->m_pObjectInfo && pObject->m_pObjectInfo->Collision && pObject->m_pObjMesh)
		{
			auto vecTargetDir = pObject->m_vOriPos - vPos;
			auto fRadius = pObject->m_pObjMesh->m_fRadius;
			if (D3DXVec3Length(&vecTargetDir) < fRadius /** 2.0f*/ + fMovingDistance) // 2016-09-07 panoskj
			{
				pObject->m_pObjMesh->Tick(pObject->m_fCurrentTime);
				pObject->m_pObjMesh->SetWorldMatrix(pObject->m_mMatrix);
				checkcollResult = pObject->m_pObjMesh->CheckCollision(mat, vPos, DEFAULT_COLLISION_DISTANCE, TRUE, FALSE);

				if (collResult.fDist > checkcollResult.fDist)

					collResult = checkcollResult;
			}
		}
		//itObj++;
	}

	if (collResult.fDist == DEFAULT_COLLISION_DISTANCE)	// 충돌 안했다면..
	{
		// 2004-11-29 by jschoi - 오브젝트 몬스터 오브젝트를 충돌 리스트에 추가
		CObjectChild * pObjectMonster = (CObjectChild *)g_pGround->m_pObjectMonster->m_pChild;

		while (pObjectMonster)
		{
			if (pObjectMonster->m_pObjectInfo &&
				pObjectMonster->m_bEnableObjectMonsterObject == FALSE &&
				(pTarget == NULL || pObjectMonster->m_dwObjectMonsterUniqueNumber != pTarget->m_pMonsterInfo->SourceIndex))
			{
				float fRadius = 0;
				if (pObjectMonster->m_pObjMesh)
				{
					fRadius = pObjectMonster->m_pObjMesh->m_fRadius;
				}

				if (D3DXVec3Length(&(pObjectMonster->m_vOriPos - vPos)) < fRadius * 2.0f + fMovingDistance)
				{
					pObjectMonster->m_pObjMesh->Tick(pObjectMonster->m_fCurrentTime);
					pObjectMonster->m_pObjMesh->SetWorldMatrix(pObjectMonster->m_mMatrix);
					checkcollResult = pObjectMonster->m_pObjMesh->CheckCollision(mat, vPos, DEFAULT_COLLISION_DISTANCE, TRUE, FALSE);
					if (collResult.fDist > checkcollResult.fDist)
					{
						collResult = checkcollResult;
					}
				}
			}
			pObjectMonster = (CObjectChild *)pObjectMonster->m_pNext;
		}
	}
	return collResult;
}

COLLISION_RESULT CObjRender::CheckCollMesh(D3DXMATRIX mat, D3DXVECTOR3 vPos, float fMovingDistance)
{
	COLLISION_RESULT collResult,checkcollResult;

	vectorCObjectChildPtr::iterator itObj(g_pScene->m_vectorCollisionObjectPtrList.begin());
	// 사용함..
	while(itObj != g_pScene->m_vectorCollisionObjectPtrList.end())
	{
		CObjectChild * pObject = *itObj;
		if(pObject && pObject->m_pObjectInfo && pObject->m_pObjectInfo->Collision && pObject->m_pObjMesh)
		{
			float fRadius = pObject->m_pObjMesh->m_fRadius;
			if(	D3DXVec3Length(&(pObject->m_vOriPos - vPos)) < fRadius * 2.0f + fMovingDistance)
			{
				pObject->m_pObjMesh->Tick(pObject->m_fCurrentTime);
				pObject->m_pObjMesh->SetWorldMatrix(pObject->m_mMatrix);
				checkcollResult = pObject->m_pObjMesh->CheckCollision(mat,vPos,fMovingDistance,TRUE,TRUE);	// 2005-08-18 by ispark
				if(collResult.fDist > checkcollResult.fDist)
					collResult = checkcollResult;
			}
		}
		itObj++;
	}
	
	return collResult;
}

/// 2004.06.17 jschoi 충돌검사 추가 함수(최적화 및 렌딩상태 추가)
COLLISION_RESULT CObjRender::CheckCollMesh(D3DXMATRIX *mat,float *size)
{
	COLLISION_RESULT collResult,checkCollResult;
	
	for (auto pObject : g_pScene->m_vectorCollisionObjectPtrList)
	{
		if(pObject && pObject->m_pObjectInfo && pObject->m_pObjectInfo->Collision && pObject->m_pObjMesh)
		{
			// 셔틀과 충돌 체크 거리
			float fRadius = pObject->m_pObjMesh->m_fRadius;
			if(	D3DXVec3Length(&(pObject->m_vOriPos - g_pShuttleChild->m_vPos)) < fRadius * 2.0f + size[0])
			{
				pObject->m_pObjMesh->Tick(pObject->m_fCurrentTime);
				pObject->m_pObjMesh->SetWorldMatrix(pObject->m_mMatrix);
				for(int nCollType = 0 ; nCollType < 6; nCollType++)
				{
					// 2004-11-02 by jschoi
					// 처음 한번만 UpdateFrame() 한다.
					if (nCollType == 0 && g_pGameMain->m_pMiniMap->IsRenderBigMap())
					{
						checkCollResult = pObject->m_pObjMesh->CheckCollision(mat[nCollType],g_pShuttleChild->m_vPos,size[nCollType],TRUE,TRUE);
					}
					else
					{
						checkCollResult = pObject->m_pObjMesh->CheckCollision(mat[nCollType],g_pShuttleChild->m_vPos,size[nCollType],FALSE,TRUE);
					}
					
					// 2005-01-06 by jschoi - UpdateFrame()은 여기서 하지 않는다.
//					checkCollResult = pObject->m_pObjMesh->CheckCollision(mat[nCollType],g_pShuttleChild->m_vPos,size[nCollType],FALSE,TRUE);

					if(checkCollResult.fDist < size[nCollType] && checkCollResult.fDist < collResult.fDist)
					{
						collResult = checkCollResult;
						collResult.nCollType = nCollType;
					}
				}
			}
		}
	}
	return collResult;
}

COLLISION_RESULT CObjRender::CheckPickMesh(D3DXMATRIX mat,
								   D3DXVECTOR3 vPos)
{
	COLLISION_RESULT collResult,checkcollResult;

	vectorCObjectChildPtr::iterator itObj(g_pScene->m_vectorCollisionObjectPtrList.begin());
	while(itObj != g_pScene->m_vectorCollisionObjectPtrList.end())
	{
		CObjectChild * pObject = *itObj;
		if(pObject && pObject->m_pObjectInfo && pObject->m_pObjMesh)
		{
			pObject->m_pObjMesh->Tick(pObject->m_fCurrentTime);
			pObject->m_pObjMesh->SetWorldMatrix(pObject->m_mMatrix);
			checkcollResult = pObject->m_pObjMesh->CheckCollision(mat,vPos,DEFAULT_COLLISION_DISTANCE,TRUE,FALSE);
			if(collResult.fDist > checkcollResult.fDist)
				collResult = checkcollResult;

		}
		itObj++;
	}
	
	return collResult;
}

CAtumNode * CObjRender::CheckCollMeshEvent(float fAllowedLength)
{
	CObjectChild * pObject = NULL;
	if(g_pGround && g_pGround->m_pObjectEvent && g_pShuttleChild)
	{
		D3DXMATRIX mat1,mat2;
		D3DXVECTOR3 vPos,vVel,vUp;
		vPos = g_pShuttleChild->m_vPos;
		vVel = g_pShuttleChild->m_vVel;
		vUp = g_pShuttleChild->m_vUp;
		D3DXMatrixLookAtLH(&mat1,&(vPos),&(vPos + vVel),&vUp);
		D3DXMatrixLookAtLH(&mat2,&(vPos),&(vPos - vVel),&vUp);
		pObject = (CObjectChild *)g_pGround->m_pObjectEvent->m_pChild;
		while(	pObject && 
				pObject->m_pObjMesh && 
				pObject->m_bIsEvent )
		{
			float fRadius = pObject->m_pObjMesh->m_fRadius;
			if(	D3DXVec3Length(&(pObject->m_vPos - g_pShuttleChild->m_vPos)) < fAllowedLength )
			{
				pObject->m_pObjMesh->Tick(pObject->m_fCurrentTime);
				pObject->m_pObjMesh->SetWorldMatrix(pObject->m_mMatrix);
				// 2004-11-01 by jschoi
				if(pObject->m_pObjMesh->CheckCollision(mat1,g_pShuttleChild->m_vPos,DEFAULT_COLLISION_DISTANCE,TRUE,FALSE).fDist < 30.0f		//	Event Object의 경우 Tick을 돌지 않으므로 
					|| pObject->m_pObjMesh->CheckCollision(mat2,g_pShuttleChild->m_vPos,DEFAULT_COLLISION_DISTANCE,FALSE,FALSE).fDist < 30.0f)	//	충돌 검사시 UpdateFrame을 해줘야 한다.
				{
					return pObject;
				}
			}
			pObject = (CObjectChild *)pObject->m_pNext;
		}
	}

	// 2005-04-12 by jschoi
//	return pObject;
	return NULL;
}


/// 2004.06.17 jschoi 충돌검사 추가 함수(최적화 및 렌딩상태 추가)
// 2004-11-15 by jschoi 현재 사용안함
COLLISION_RESULT CObjRender::CheckCollForCamera(D3DXMATRIX *mat,float size)
{
	COLLISION_RESULT collResult;

	vectorCObjectChildPtr::iterator itObj(g_pScene->m_vectorCollisionObjectPtrList.begin());
	while(itObj != g_pScene->m_vectorCollisionObjectPtrList.end())
	{
		CObjectChild * pObject = *itObj;

		if(pObject && pObject->m_pObjectInfo && pObject->m_pObjectInfo->Collision && pObject->m_pObjMesh)
		{
			// 셔틀과 충돌 체크 거리
			float fRadius = pObject->m_pObjMesh->m_fRadius;
			if(	D3DXVec3Length(&(pObject->m_vOriPos - g_pCamera->GetEyePt())) < fRadius)
			{
				pObject->m_pObjMesh->Tick(pObject->m_fCurrentTime);
				pObject->m_pObjMesh->SetWorldMatrix(pObject->m_mMatrix);
				
				for(int nCollType = 0 ; nCollType < 6; nCollType++)
				{
					collResult = pObject->m_pObjMesh->CheckCollision(mat[nCollType],g_pCamera->GetEyePt(),size,FALSE,FALSE);
					if(collResult.fDist < size)
					{
						collResult.nCollType = nCollType;
						return collResult;
					}
				}
			}
		}
		itObj++;
	}
	collResult.nCollType = COLL_NONE;
	return collResult;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			CObjRender::CheckPickMesh(float fx, float xy)
/// \brief		Picking 함수
/// \author		ispark
/// \date		2005-07-14 ~ 2005-07-14
/// \warning	
///
/// \param		
/// \return		COLLISION_RESULT
///////////////////////////////////////////////////////////////////////////////
COLLISION_RESULT CObjRender::CheckPickMesh(float fx, float fy)
{
	COLLISION_RESULT collResult,checkcollResult;

	vectorCObjectChildPtr::iterator itObj(g_pScene->m_vectorCollisionObjectPtrList.begin());
	while(itObj != g_pScene->m_vectorCollisionObjectPtrList.end())
	{
		CObjectChild * pObject = *itObj;
		if(pObject && pObject->m_pObjectInfo && pObject->m_pObjMesh)
		{
			pObject->m_pObjMesh->Tick(pObject->m_fCurrentTime);
			pObject->m_pObjMesh->SetWorldMatrix(pObject->m_mMatrix);
			checkcollResult = pObject->m_pObjMesh->CheckCollision(fx, fy, TRUE);
			if(collResult.fDist > checkcollResult.fDist)
				collResult = checkcollResult;

		}
		itObj++;
	}
	
	return collResult;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			CObjRender::CheckPickMesh(float fx, float xy)
/// \brief		Picking 오브젝트 
/// \author		ispark
/// \date		2005-07-14 ~ 2005-07-14
/// \warning	
///
/// \param		
/// \return		COLLISION_RESULT
///////////////////////////////////////////////////////////////////////////////
CObjectChild* CObjRender::GetPickMesh(float fx, float fy)
{
	COLLISION_RESULT collResult,checkcollResult;
	CObjectChild * pObjectRes = NULL;

	vectorCObjectChildPtr::iterator itObj(g_pScene->m_vectorCollisionObjectPtrList.begin());
	while(itObj != g_pScene->m_vectorCollisionObjectPtrList.end())
	{
		CObjectChild * pObject = *itObj;
		if(pObject && pObject->m_pObjectInfo && pObject->m_pObjMesh)
		{
			pObject->m_pObjMesh->Tick(pObject->m_fCurrentTime);
			pObject->m_pObjMesh->SetWorldMatrix(pObject->m_mMatrix);
			checkcollResult = pObject->m_pObjMesh->CheckCollision(fx, fy, TRUE);
			if(collResult.fDist > checkcollResult.fDist)
			{
				pObjectRes = pObject;
				collResult = checkcollResult;
			}

		}
		itObj++;
	}
	
	return pObjectRes;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			CObjRender::CheckCollMeshEvent(int nEventType)
/// \brief		이벤트 타입 번호로 이벤트 충돌 구조체 가져오기
/// \author		ispark
/// \date		2005-08-01 ~ 2005-08-01
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
COLLISION_RESULT CObjRender::CheckCollMeshEvent(float fAllowedLength, int nEventType)
{
	CObjectChild * pObject = NULL;
	COLLISION_RESULT collResult;
	if(g_pGround && g_pGround->m_pObjectEvent && g_pShuttleChild)
	{
		D3DXMATRIX mat1,mat2;
		D3DXVECTOR3 vPos,vVel,vUp;
		vPos = g_pShuttleChild->m_vPos;
		vVel = g_pShuttleChild->m_vVel;
		vUp = g_pShuttleChild->m_vUp;
		D3DXMatrixLookAtLH(&mat1,&(vPos),&(vPos + vVel),&vUp);
		D3DXMatrixLookAtLH(&mat2,&(vPos),&(vPos - vVel),&vUp);
		pObject = (CObjectChild *)g_pGround->m_pObjectEvent->m_pChild;
		while(	pObject && 
				pObject->m_pObjMesh && 
				pObject->m_bIsEvent )
		{
			if(nEventType == pObject->m_bEventType)
			{
				float fRadius = pObject->m_pObjMesh->m_fRadius;
				if(	D3DXVec3Length(&(pObject->m_vPos - g_pShuttleChild->m_vPos)) < fAllowedLength )
				{
					pObject->m_pObjMesh->Tick(pObject->m_fCurrentTime);
					pObject->m_pObjMesh->SetWorldMatrix(pObject->m_mMatrix);
					// 2004-11-01 by jschoi
					collResult = pObject->m_pObjMesh->CheckCollision(mat1,g_pShuttleChild->m_vPos,DEFAULT_COLLISION_DISTANCE,TRUE,FALSE);
					return collResult;
				}
			}
			pObject = (CObjectChild *)pObject->m_pNext;
		}
	}

	// 2005-04-12 by jschoi
//	return pObject;
	collResult.nCollType = COLL_NONE;
	return collResult;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			COLLISION_RESULT CObjRender::CheckCollMeshEventOther(D3DXVECTOR3 vDesPos, D3DXVECTOR3 vDesVel, D3DXVECTOR3 vDesUp, float fAllowedLength, int nEventType)
/// \brief		
/// \author		ispark
/// \date		2006-07-26 ~ 2006-07-26
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
CAtumNode * CObjRender::CheckCollMeshEventOther(D3DXVECTOR3 vDesPos, D3DXVECTOR3 vDesVel, D3DXVECTOR3 vDesUp, float fAllowedLength, int nEventType)
{
	CObjectChild * pObject = NULL;
	COLLISION_RESULT collResult;
	if(g_pGround && g_pGround->m_pObjectEvent)
	{
		D3DXMATRIX mat1,mat2;
		D3DXVECTOR3 vPos,vVel,vUp;
		vPos = vDesPos;
		vVel = vDesVel;
		vUp = vDesUp;
		D3DXMatrixLookAtLH(&mat1,&(vPos),&(vPos + vVel),&vUp);
		D3DXMatrixLookAtLH(&mat2,&(vPos),&(vPos - vVel),&vUp);
		pObject = (CObjectChild *)g_pGround->m_pObjectEvent->m_pChild;
		while(	pObject && 
				pObject->m_pObjMesh && 
				pObject->m_bIsEvent )
		{
			if(nEventType == pObject->m_bEventType)
			{
				float fRadius = pObject->m_pObjMesh->m_fRadius;
				if(	D3DXVec3Length(&(pObject->m_vPos - vDesPos)) < fAllowedLength )
				{
					return pObject;
				}
			}
			pObject = (CObjectChild *)pObject->m_pNext;
		}
	}

	return NULL;	
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			CObjRender::GetPickObject(float fx, float xy)
/// \brief		Picking 함수
/// \author		ispark
/// \date		2005-07-14 ~ 2005-07-14
/// \warning	
///
/// \param		
/// \return		COLLISION_RESULT
///////////////////////////////////////////////////////////////////////////////
COLLISION_OBJ_RESULT CObjRender::GetPickObject(float fx, float fy)
{
	COLLISION_OBJ_RESULT collResult;
	COLLISION_RESULT checkcollResult;

	vectorCObjectChildPtr::iterator itObj(g_pScene->m_vectorCollisionObjectPtrList.begin());
	while(itObj != g_pScene->m_vectorCollisionObjectPtrList.end())
	{
		CObjectChild * pObject = *itObj;
		// 2013-05-07 by bhsohn 세력포인트 개선안 시스템
//		if(pObject && pObject->m_pObjectInfo && pObject->m_pObjMesh)
		if(pObject && pObject->m_pObjectInfo && pObject->m_pObjMesh && pObject->IsShowNode())
		{
			pObject->m_pObjMesh->Tick(pObject->m_fCurrentTime);
			pObject->m_pObjMesh->SetWorldMatrix(pObject->m_mMatrix);
			checkcollResult = pObject->m_pObjMesh->CheckCollision(fx, fy, TRUE);

			if(collResult.stCollision.fDist > checkcollResult.fDist)
			{
				collResult.pObject = pObject;
				collResult.stCollision = checkcollResult;
			}

		}
		itObj++;
	}
	
	return collResult;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			CObjectChild* CObjRender::GetPickObjectShopNPC(float fx, float fy)
/// \brief		별도 상점 NPC 체크(OBJECT_SHOP_NPC)
/// \author		ispark
/// \date		2006-09-14 ~ 2006-09-14
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
CObjectChild* CObjRender::GetPickObjectShopNPC(float fx, float fy)
{
	CObjectChild* pObjTemp = NULL;
	float fLength = 100.0f;
	D3DXVECTOR2 vPos1 = D3DXVECTOR2(fx, fy);
	
	int nObjScreenX = 0;
	int nObjScreenY = 0;
	int nObjScreenW = 0;
	
	vectorCObjectChildPtr::iterator itObj(g_pScene->m_vectorCollisionObjectPtrList.begin());
	while(itObj != g_pScene->m_vectorCollisionObjectPtrList.end())
	{
		CObjectChild * pObject = *itObj;
		
		D3DXVECTOR3 vObjPos = pObject->m_vPos;
		vObjPos.y += (pObject->m_pObjMesh->m_fRadius / 2.0f);
		g_pD3dApp->CalcObjectSourceScreenCoords(vObjPos, g_pD3dApp->GetBackBufferDesc().Width, g_pD3dApp->GetBackBufferDesc().Height, 
			nObjScreenX, nObjScreenY, nObjScreenW );

		if(pObject && pObject->m_bEventType == EVENT_TYPE_OBJ_BUILDING_NPC &&
			pObject->m_pObjectInfo->ObjectRenderType == OBJECT_SHOP_NPC && nObjScreenW > 0)
		{
			D3DXVECTOR2 vPos2 = D3DXVECTOR2(nObjScreenX, nObjScreenY);
			float fLengthTemp = D3DXVec2Length(&(vPos1-vPos2));
			if(fLengthTemp < fLength)
			{
				fLength = fLengthTemp;
				pObjTemp = pObject;
			}

		}
		itObj++;
	}

	return pObjTemp;
}