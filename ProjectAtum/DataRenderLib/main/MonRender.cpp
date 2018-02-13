// MonRender.cpp: implementation of the CMonRender class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AtumApplication.h"
#include "MonRender.h"
#include "MonsterData.h"
#include "ShuttleChild.h"
#include "CharacterChild.h"				// 2005-07-21 by ispark
#include "AtumDatabase.h"
#include "SceneData.h"
#include "dxutil.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMonRender::CMonRender()
{
//	m_dwNormalStateBlock = 0;
//	m_dwAlphaBlendStateBlock = 0;
}

CMonRender::~CMonRender()
{
	map<int,CSkinnedMesh*>::iterator it = m_mapSkinnedMesh.begin();
	while( it != m_mapSkinnedMesh.end()) 
	{
		util::del(it->second);
		it++;
	}
	m_mapSkinnedMesh.clear();
}

void CMonRender::RenderShadow(CAtumNode * pNode)
{
	CMonsterData * pMonsterData = (CMonsterData *)pNode;

	if(!pMonsterData->m_pMonsterInfo)
	{
		return;
	}
	// 2009. 11. 23 by jskim 리소스 로딩 구조 변경
	// 	D3DXMATRIX	matTemp;
	// 	D3DXMatrixIdentity(&matTemp);
	// 	if( pMonsterData->m_pMonMesh &&
	// 		pMonsterData->m_bShadowIsRender && 
	// 		pMonsterData->m_dwState != _EXPLODED && 
	// 		pMonsterData->m_dwState != _EXPLODING)
	// 	{
	// 		// 2007-08-29 by bhsohn 최소 프레임시 기본 아머만 로딩하게끔 변경
	// 		//g_pD3dDev->SetTexture( 0, pMonsterData->m_pMonMesh->m_pTexture[pMonsterData->m_pMonMesh->m_bTotalTextureNum-1] );
	// 		LPDIRECT3DTEXTURE9	pShadowTex = pMonsterData->m_pMonMesh->GetShadowTex();
	// 		g_pD3dDev->SetTexture( 0, pShadowTex );
	// 
	// 		g_pD3dDev->SetTransform( D3DTS_WORLD, &matTemp );
	// 		// 2005-01-03 by jschoi
	// // 		g_pD3dDev->SetVertexShader( D3DFVF_SPRITEVERTEX );
	// 		g_pD3dDev->SetFVF( D3DFVF_SPRITEVERTEX );
	// 		g_pD3dDev->SetStreamSource( 0, pMonsterData->m_pVBShadow,0, sizeof(SPRITEVERTEX) );
	// 		g_pD3dDev->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2 );
	// 	}

	int nMonsterNum = pMonsterData->m_pMonsterInfo->SourceIndex;
	map<int,CSkinnedMesh*>::iterator it = m_mapSkinnedMesh.find(nMonsterNum);
	D3DXMATRIX	matTemp;
	D3DXMatrixIdentity(&matTemp);
	if( it!= m_mapSkinnedMesh.end() &&
		pMonsterData->m_pMonMesh &&
		pMonsterData->m_bShadowIsRender && 
		pMonsterData->m_dwState != _EXPLODED && 
		pMonsterData->m_dwState != _EXPLODING)
	{
		if(it->second->GetIsLoadingFlag() != TRUE)
		{
		// 2007-08-29 by bhsohn 최소 프레임시 기본 아머만 로딩하게끔 변경
		//g_pD3dDev->SetTexture( 0, pMonsterData->m_pMonMesh->m_pTexture[pMonsterData->m_pMonMesh->m_bTotalTextureNum-1] );
		LPDIRECT3DTEXTURE9	pShadowTex = pMonsterData->m_pMonMesh->GetShadowTex();
		g_pD3dDev->SetTexture( 0, pShadowTex );

		g_pD3dDev->SetTransform( D3DTS_WORLD, &matTemp );
		// 2005-01-03 by jschoi
// 		g_pD3dDev->SetVertexShader( D3DFVF_SPRITEVERTEX );
		g_pD3dDev->SetFVF( D3DFVF_SPRITEVERTEX );
		g_pD3dDev->SetStreamSource( 0, pMonsterData->m_pVBShadow,0, sizeof(SPRITEVERTEX) );
		g_pD3dDev->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2 );
	    } 
	}
}

void CMonRender::Render(CMonsterData * pMonData)
{
	if(!pMonData->m_pMonsterInfo)
	{
		return;
	}
	if(pMonData->m_pMonMesh == NULL)
	{
		return;
	}

	D3DXMATRIX	matTemp;
	D3DXMatrixIdentity(&matTemp);


	if(	!IS_OBJECTMONSTER_BY_MONSTERFORM(pMonData->m_info.MonsterForm)  &&
		(pMonData->m_dwState == _EXPLODING || 
		pMonData->m_dwState == _EXPLODED || 
		pMonData->m_dwState == _FALLING ||
		pMonData->m_dwState == _FALLEN) )
	{
		// 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템
		
// 		g_pD3dDev->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
// 		g_pD3dDev->SetRenderState( D3DRS_ALPHATESTENABLE,  FALSE );
// 		g_pD3dDev->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_ONE );
// 		g_pD3dDev->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );

		// 알파 블랜딩없이 추락하는 몬스터일 경우
		if( COMPARE_MPOPTION_BIT( pMonData->m_pMonsterInfo->MPOption, MPOPTION_BIT_NO_ALPHABLENDING_IN_DEAD ) )
		{
			g_pD3dDev->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );
			g_pD3dDev->SetRenderState( D3DRS_ALPHATESTENABLE,  FALSE );
		}
		else
		{
			g_pD3dDev->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
			g_pD3dDev->SetRenderState( D3DRS_ALPHATESTENABLE,  FALSE );
			g_pD3dDev->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_ONE );
			g_pD3dDev->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );
		}

		// end 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템
	}
	else
	{
//		g_pD3dDev->SetLight( 0, &g_pScene->m_light0 );
//		g_pD3dDev->LightEnable( 0, TRUE ); /// 0번 광원을 켠다 
//		g_pD3dDev->SetRenderState( D3DRS_LIGHTING, TRUE ); /// 광원설정을 켠다
//		g_pD3dDev->SetRenderState( D3DRS_SPECULARENABLE, TRUE );
//		g_pD3dDev->LightEnable( 2, FALSE );
//		g_pD3dDev->LightEnable( 3, FALSE );

		if(pMonData->m_bAlphaBlending)
		{
//			g_pD3dDev->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
//			g_pD3dDev->SetRenderState( D3DRS_ALPHATESTENABLE,  FALSE );
//			g_pD3dDev->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
//			g_pD3dDev->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
			g_pD3dDev->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
			g_pD3dDev->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
			g_pD3dDev->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
			g_pD3dDev->SetRenderState( D3DRS_ALPHATESTENABLE,  TRUE );
			g_pD3dDev->SetRenderState( D3DRS_ALPHAFUNC,  D3DCMP_GREATEREQUAL );
			g_pD3dDev->SetRenderState( D3DRS_ALPHAREF,  0x08 );
		}
		else
		{
			g_pD3dDev->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );
			g_pD3dDev->SetRenderState( D3DRS_ALPHATESTENABLE,  FALSE );
		}
	}
	// 2005-01-05 by jschoi - 현재 블렌딩 메시 사용안함.
//	pMonData->m_pMonMesh->m_bCheckBlend = pMonData->m_bCheckBlend;

	pMonData->m_pMonMesh->Tick(pMonData->m_fCurrentTime);
	pMonData->m_pMonMesh->SetWorldMatrix(pMonData->m_mMatrix);
	pMonData->m_pMonMesh->m_bProgressiveMesh = false;
	pMonData->m_pMonMesh->m_material = pMonData->m_material;
	if(pMonData->m_bAniColl)
	{
		g_pD3dDev->LightEnable( 2, TRUE );
		D3DLIGHT9 light;
	
		ZeroMemory( &light, sizeof(D3DLIGHT9) );
		light.Type = D3DLIGHT_POINT;
		light.Diffuse.r = 1.0f;
		light.Diffuse.g = 0.2f;
		light.Diffuse.b = 0.2f;
		light.Ambient.r  = 0.2f;
		light.Ambient.g  = 0.2f;
		light.Ambient.b  = 0.2f;
		light.Position = pMonData->m_vPos - (pMonData->m_fObjectSize*1.2f)*pMonData->m_vAttackVel;
		light.Range = pMonData->m_fObjectSize*2.5f;
		light.Attenuation1 = 0.1f;
		g_pD3dDev->SetLight( 2, &light );
		pMonData->m_pMonMesh->AnotherTexture(pMonData->m_pMonsterInfo->TextureIndex);
		pMonData->m_pMonMesh->Render();
		g_pD3dDev->LightEnable( 2, FALSE );
	}
	else
	{
		pMonData->m_pMonMesh->AnotherTexture(pMonData->m_pMonsterInfo->TextureIndex);
		pMonData->m_pMonMesh->Render();
	}		
}

void CMonRender::Tick(float fElapsedTime)
{

}

HRESULT CMonRender::InitDeviceObjects()
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

			p += nCont*sizeof(int);
			memcpy(&nCont,p,sizeof(int));
			p += sizeof(int);

			for(int i=0;i<nCont;i++)
			{
				int nMonType;
				memcpy(&nMonType,p,sizeof(int));
				MEX_MONSTER_INFO * pMonster = g_pDatabase->CheckMonsterInfo(nMonType);
				if(pMonster && pMonster->SourceIndex > 0)
				{
					// 2009. 11. 23 by jskim 리소스 로딩 구조 변경
					//InitData(pMonster->SourceIndex);
					InitData(pMonster->SourceIndex, _MONSTER_TYPE);				
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

HRESULT CMonRender::RestoreDeviceObjects()
{
	map<int,CSkinnedMesh*>::iterator it = m_mapSkinnedMesh.begin();
	while(it != m_mapSkinnedMesh.end())
	{
		it->second->RestoreDeviceObjects();
		it++;
	}
/*	CMapMonsterIterator it2 = g_pScene->m_mapMonsterList.begin();
	while( it2 != g_pScene->m_mapMonsterList.end() )
	{
//		it2->second->RestoreShadow();
		it2->second->RestoreDeviceObjects();
		it2++;
	}
*/
	return S_OK;
}

HRESULT CMonRender::InvalidateDeviceObjects()
{
	map<int,CSkinnedMesh*>::iterator it = m_mapSkinnedMesh.begin();
	while(it != m_mapSkinnedMesh.end())
	{
		it->second->InvalidateDeviceObjects();
		it++;
	}
/*	CMapMonsterIterator it2 = g_pScene->m_mapMonsterList.begin();
	while( it2 != g_pScene->m_mapMonsterList.end() )
	{
//		it2->second->InvalidateShadow();
		it2->second->InvalidateDeviceObjects();
		it2++;
	}
*/
	return S_OK;
}

HRESULT CMonRender::DeleteDeviceObjects()
{
	map<int,CSkinnedMesh*>::iterator it = m_mapSkinnedMesh.begin();
	while(it != m_mapSkinnedMesh.end())
	{
		// 2009. 11. 23 by jskim 리소스 로딩 구조 변경	
		it->second->DeleteLoadingGameData();
		//end 2009. 11. 23 by jskim 리소스 로딩 구조 변경
		it->second->DeleteDeviceObjects();
		util::del(it->second);
		it++;
	}
	m_mapSkinnedMesh.clear();
	return S_OK;
}

/// 2004.06.18 jschoi 충돌검사 추가 함수(최적화 및 렌딩상태 추가)
COLLISION_RESULT CMonRender::CheckCollMesh(D3DXMATRIX *mat,float *size, float *fColledMonsterSize, ClientIndex_t *o_monsterIndex)
{
	COLLISION_RESULT collResult;
	*o_monsterIndex = 0;
//	collResult.fDist = 10000.0f;
//	collResult.vNormalVector = D3DXVECTOR3(0,0,0);
//	collResult.nCollType = COLL_NONE;
	
	CMapMonsterIterator it = g_pScene->m_mapMonsterList.begin();
	while(it != g_pScene->m_mapMonsterList.end())
	{
		CMonsterData * pMon = it->second;
		if( pMon && pMon->m_pMonMesh &&
			ATTACK_AVAILABLE_STATE(pMon->m_dwState) &&
			D3DXVec3Length(&(pMon->m_vPos - g_pShuttleChild->m_vPos)) < pMon->m_pMonMesh->m_fRadius + size[0])
		{
			pMon->m_pMonMesh->Tick(pMon->m_fCurrentTime);
			pMon->m_pMonMesh->SetWorldMatrix(pMon->m_mMatrix);
			for(int nCollType = 0 ; nCollType < 6; nCollType++)
			{
				collResult = pMon->m_pMonMesh->CheckCollision(mat[nCollType],g_pShuttleChild->m_vPos,size[nCollType],FALSE,TRUE);
				if(collResult.fDist < size[nCollType])
				{
					g_pShuttleChild->m_vCollCenter = pMon->m_vPos;
					collResult.nCollType = nCollType;
					// 2004-10-22 by jschoi 충돌한 몬스터의 크기를 담아줌
					*fColledMonsterSize = pMon->m_pMonMesh->m_fRadius;
					*o_monsterIndex = pMon->m_info.MonsterIndex;
					return collResult;
				}
			}
		}
		it++;
	}

	
//	int x = (int)(g_pShuttleChild->m_vPos.x/(TILE_SIZE*3));
//	int y = (int)(g_pShuttleChild->m_vPos.z/(TILE_SIZE*3));
//	CVecMonsterIterator it = g_pScene->m_vecMonsterList[x*g_pScene->m_nBlockSizeX + y].begin();
//	
//	while(it != g_pScene->m_vecMonsterList[x*g_pScene->m_nBlockSizeX + y].end())
//	{
//		CMonsterData * pMon = (CMonsterData *)(*it);
//		if( pMon->m_bCheckColl && 
//			pMon->m_pMonsterInfo && 
//			pMon->m_dwState == _NORMAL &&
//			pMon->m_pMonMesh &&
//			D3DXVec3Length(&(pMon->m_vPos - g_pShuttleChild->m_vPos)) < pMon->m_fObjectSize + size[0])//	&& m_pMeshNode[pMon->m_pMonsterInfo->RenderIndex])
//		{
//			pMon->m_pMonMesh->Tick(pMon->m_fCurrentTime);
//			pMon->m_pMonMesh->SetWorldMatrix(pMon->m_mMatrix);
//			
//			for(int nCollType = 0 ; nCollType < 6; nCollType++)
//			{
//				collResult = pMon->m_pMonMesh->CheckCollision(mat[nCollType]);
//				if(collResult.fDist < size[nCollType])
//				{
//					g_pShuttleChild->m_vCollCenter = pMon->m_vPos;
//					collResult.nCollType = nCollType;
//					return collResult;
//				}
//			}
//			// 충돌 검사를 했는지 판단하여 더이상 충돌 검출할 대상이 없을때는 그냥 리턴하게 된다.
//			g_pShuttleChild->m_bCollMonsterCheck--;
//			if(g_pShuttleChild->m_bCollMonsterCheck <=0)
//			{
//				collResult.nCollType = -1;
//				return collResult;
//			}
//		}
//		it++;
// 	}
	collResult.nCollType = COLL_NONE;
	return collResult;
}
