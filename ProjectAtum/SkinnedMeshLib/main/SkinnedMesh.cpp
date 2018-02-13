#include "StdAfx.h"
#include "SkinnedMesh.h"
#include "rmxfguid.h"
#include <malloc.h> // _alloca



#include "tchar.h"
#include "stdio.h"
#include "AtumApplication.h"
#include "d3dutil.h"
#include "dxutil.h"
#include "ShuttleChild.h"
#include "Frustum.h"
#include "Camera.h"
#include "SceneData.h"
#include "rmxftmpl.h"
#include "Chat.h"
#include "FxSystem.h"

#include "AtumDatabase.h"

#define D3DRM_XTEMPLATE_BYTES 3278
#define D3DFVF_VERTEX (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_TEX1)

#ifdef _DEBUG 
#define LOADING_TICK 30
#else
#define LOADING_TICK 500
#endif

extern LPDIRECT3DDEVICE9 g_pD3dDev;
extern unsigned char D3DRM_XTEMPLATES[];

char* szTemplates = "xof 0303txt 0032\
template XSkinMeshHeader {\
<3cf169ce-ff7c-44ab-93c0-f78f62d172e2>\
WORD nMaxSkinWeightsPerVertex;\
WORD nMaxSkinWeightsPerFace;\
WORD nBones;}\
template VertexDuplicationIndices {\
<b8d65549-d7c9-4995-89cf-53a9a8b031e3>\
DWORD nIndices;\
DWORD nOriginalVertices;\
array DWORD indices[nIndices];}\
template SkinWeights {\
<6f0d123b-bad2-4167-a0d0-80224f25fabb>\
STRING transformNodeName;\
DWORD nWeights;\
array DWORD vertexIndices[nWeights];\
array FLOAT weights[nWeights];\
Matrix4x4 matrixOffset;}";

void SFrame::SetTime(float fGlobalTime)
{
	UINT iKey;
	UINT dwp2;
	UINT dwp3;
	D3DXMATRIXA16 matResult;
	D3DXMATRIXA16 matTemp;
	float fTime1;
	float fTime2;
	float fLerpValue;
	D3DXVECTOR3 vScale;
	D3DXVECTOR3 vPos;
	D3DXQUATERNION quat;
	BOOL bAnimate = false;
	DWORD uGlobalTime = (DWORD)fGlobalTime, uTime;

	// if (pframeToAnimate == nullptr) return;

	if (m_pMatrixKeys)
	{
		// 2015-10-20 PanKJ Optimization: fmod is expensive
		uTime = uGlobalTime % m_pMatrixKeys[m_cMatrixKeys - 1].dwTime;

		dwp3 = m_mapMatrixKeys.upper_bound(uTime)->second;
		dwp2 = dwp3 > 0 ? dwp3 - 1 : 0;

		DWORD range = m_pMatrixKeys[dwp3].dwTime - m_pMatrixKeys[dwp2].dwTime;
		DWORD doubledt = 2 * (uTime - m_pMatrixKeys[dwp2].dwTime);

		if (doubledt >= range) iKey = dwp3;
		else iKey = dwp2;

		pframeToAnimate->matRot = m_pMatrixKeys[iKey].mat;
	}
	else
	{
		if (m_pScaleKeys)
		{
			dwp2 = dwp3 = 0;

			// 2015-10-20 PanKJ Optimization: fmod is expensive
			uTime = uGlobalTime % m_pScaleKeys[m_cScaleKeys - 1].dwTime;

			for (iKey = 0; iKey < m_cScaleKeys; iKey++)
			{
				if (m_pScaleKeys[iKey].dwTime > uTime)
				{
					dwp3 = iKey;

					if (iKey > 0) dwp2 = iKey - 1;
					else dwp2 = iKey;

					break;
				}
			}

			fTime1 = (float)m_pScaleKeys[dwp2].dwTime;
			fTime2 = (float)m_pScaleKeys[dwp3].dwTime;

			if (m_pScaleKeys[dwp2].dwTime == m_pScaleKeys[dwp3].dwTime) fLerpValue = 0;

			else fLerpValue = (uTime + fGlobalTime - uGlobalTime - fTime1) / (fTime2 - fTime1);


			D3DXVec3Lerp(&vScale, &m_pScaleKeys[dwp2].vScale, &m_pScaleKeys[dwp3].vScale, fLerpValue);

			D3DXMatrixScaling(&matResult, vScale.x, vScale.y, vScale.z);

			bAnimate = true;
		}

		//check rot keys
		if (m_pRotateKeys)
		{
			int i1 = 0;
			int i2 = 0;

			uTime = uGlobalTime % m_pRotateKeys[m_cRotateKeys - 1].dwTime;

			for (iKey = 0; iKey < m_cRotateKeys; iKey++)
			{
				if (m_pRotateKeys[iKey].dwTime > uTime)
				{
					i1 = (iKey > 0) ? iKey - 1 : 0;
					i2 = iKey;
					break;
				}
			}

			fTime1 = (float)m_pRotateKeys[i1].dwTime;
			fTime2 = (float)m_pRotateKeys[i2].dwTime;

			if (m_pRotateKeys[i1].dwTime == m_pRotateKeys[i2].dwTime) fLerpValue = 0;

			else fLerpValue = (uTime + fGlobalTime - uGlobalTime - fTime1) / (fTime2 - fTime1);

			D3DXQuaternionSlerp(&quat, &m_pRotateKeys[i1].quatRotate, &m_pRotateKeys[i2].quatRotate, fLerpValue);

			quat.w = -quat.w;
			D3DXMatrixRotationQuaternion(&matTemp, &quat);
			D3DXMatrixMultiply(&matResult, &matResult, &matTemp);

			bAnimate = true;
		}

		if (m_pPositionKeys)
		{
			dwp2 = dwp3 = 0;

			uTime = uGlobalTime % m_pPositionKeys[m_cPositionKeys - 1].dwTime;

			for (iKey = 0; iKey < m_cPositionKeys; iKey++)
			{
				if (m_pPositionKeys[iKey].dwTime > uTime)
				{
					dwp3 = iKey;

					if (iKey > 0) dwp2 = iKey - 1;
					else dwp2 = iKey;

					break;
				}
			}

			fTime1 = (float)m_pPositionKeys[dwp2].dwTime;
			fTime2 = (float)m_pPositionKeys[dwp3].dwTime;

			if (m_pPositionKeys[dwp2].dwTime == m_pPositionKeys[dwp3].dwTime) fLerpValue = 0;

			else fLerpValue = (uTime + fGlobalTime - uGlobalTime - fTime1) / (fTime2 - fTime1);

			D3DXVec3Lerp((D3DXVECTOR3*)&vPos, &m_pPositionKeys[dwp2].vPos, &m_pPositionKeys[dwp3].vPos, fLerpValue);

			D3DXMatrixTranslation(&matTemp, vPos.x, vPos.y, vPos.z);

			D3DXMatrixMultiply(&matResult, &matResult, &matTemp);

			bAnimate = true;
		}
		else
		{
			D3DXMatrixTranslation(&matTemp, pframeToAnimate->matRotOrig._41, pframeToAnimate->matRotOrig._42, pframeToAnimate->matRotOrig._43);

			D3DXMatrixMultiply(&matResult, &matResult, &matTemp);
		}

		if (bAnimate)
		{
			pframeToAnimate->matRot = matResult;
		}
	}
}

CSkinnedMesh::CSkinnedMesh()
{
	FLOG("CSkinnedMesh()");
	m_bProgressiveMesh = false;
	InitSkinnedMesh();
}

CSkinnedMesh::CSkinnedMesh(bool bProgressiveMesh)
{
	FLOG("CSkinnedMesh(BOOL bProgressiveMesh)");
	m_bProgressiveMesh = bProgressiveMesh;
	InitSkinnedMesh();
}

void CSkinnedMesh::InitSkinnedMesh()
{
	m_pd3dDevice = nullptr;

	m_pmcSelectedMesh = nullptr;
	m_pframeSelected = nullptr;
	m_pdeHead = nullptr;
	m_pdeSelected = nullptr;
	m_pBoneMatrices = nullptr;
	m_maxBones = 0;
	m_method = SOFTWARE; //m_method = D3DNONINDEXED;

	m_dwFVF = D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_NORMAL | D3DFVF_TEX1;

	m_bTotalTextureNum = 0;
	m_bTextureNum = 0;
	m_pTexture = nullptr;

	m_pGameData = nullptr;
	m_pGameDataToDecal = nullptr;
	m_pDataHeader = nullptr;
	m_nRenderCount = 0;
	m_bMaterial = FALSE;
	m_bCheckBlend = FALSE;
	D3DUtil_InitMaterial(m_material, 1.0f, 1.0f, 1.0f);// 임시 코드
	m_material.Specular.r = 1.0f;
	m_material.Specular.g = 1.0f;
	m_material.Specular.b = 1.0f;
	m_material.Power = 1.0f;
	m_bMultiTexture = FALSE;
	m_pOrderTexture = nullptr;
	m_fRadius = 0.0f;
	m_vCenter = D3DXVECTOR3(0, 0, 0);

	for (auto i = 0; i < MAX_DECAL_NUM; i++)
	{
		m_nSelectDecalNum[i] = 0;
	}
	//	m_pDecalTextures  = NULL;
	m_nDecalCount = 0;
	m_unTexColor = 0;
	m_unTexSelectColor = 0;
	m_unUnitTexOld = 0;

	// 2007-08-29 by bhsohn 기본 아머만 로딩하게끔 변경
	m_pLoadMem = nullptr;
	// 2007-11-08 by bhsohn 인벤 이펙트 관련 처리

	memset(m_szMeshObjFileName, 0x00, 256);

	pxofapi = nullptr;
	pxofenum = nullptr;

	pdeMesh = nullptr;
	FileLoadMemory = {nullptr, 0};
	dwOptions = D3DXMESH_MANAGED;
	pxofobjCur = nullptr;
	pHeader = nullptr;
	m_bIsLoadingFlag = FALSE;
	m_StartTime = 0;
}


CSkinnedMesh::~CSkinnedMesh()
{
	FLOG("~CSkinnedMesh()");
	util::del_array(m_pDataHeader);
	util::del(m_pGameDataToDecal);

	// 2007-08-29 by bhsohn 기본 아머만 로딩하게끔 변경
	util::del_array(m_pLoadMem);
}

HRESULT CSkinnedMesh::InitDeviceObjects()
{
	FLOG("CSkinnedMesh::InitDeviceObjects()");
	m_pd3dDevice = g_pD3dDev;
	m_d3dCaps = g_pD3dApp->GetDeviceCaps();
	//m_pd3dDevice->GetDeviceCaps( &m_d3dCaps );
	return CAtumNode::InitDeviceObjects();
}

HRESULT CSkinnedMesh::RestoreDeviceObjects()
{
	FLOG("CSkinnedMesh::RestoreDeviceObjects()");
	return CAtumNode::RestoreDeviceObjects();
}

HRESULT CSkinnedMesh::InvalidateDeviceObjects()
{
	FLOG("CSkinnedMesh::InvalidateDeviceObjects()");
	for (auto pdeCurr = m_pdeHead; pdeCurr != nullptr; pdeCurr = pdeCurr->pdeNext)
	{
		ReleaseDeviceDependentMeshes(pdeCurr->pframeRoot);
	}
	return CAtumNode::InvalidateDeviceObjects();
}

HRESULT CSkinnedMesh::DeleteDeviceObjects()
{
	FLOG("CSkinnedMesh::DeleteDeviceObjects()");
	if (m_pdeSelected == m_pdeHead)
		m_pdeSelected = nullptr;
	delete m_pdeHead;
	m_pdeHead = nullptr;
	delete[] m_pBoneMatrices;
	m_pBoneMatrices = nullptr;

	//if (m_pTexture)
	//{
	//	if (m_pLoadMem)
	//	{
	//		for (auto n = 0; n < m_bTotalTextureNum; n++)

	//			if (m_pTexture[n] && m_pLoadMem[n]) m_pTexture[n]->Release();

	//		util::del_array(m_pLoadMem);
	//	}

	//	if (m_bTotalTextureNum > 0)
	//	util::del_array(m_pTexture);
	//}

	if (m_pTexture)
	{
		for (int n = 0; n < m_bTotalTextureNum; n++)
		{
			if (m_pTexture[n])
			{
				if (IsLoadTexture(n))
				{
					m_pTexture[n]->Release();
					m_pTexture[n] = nullptr;
				}
				m_pTexture[n] = nullptr;
			}
		}
		if (m_bTotalTextureNum > 0)
			util::del_array(m_pTexture);

		util::del_array(m_pLoadMem);
	}

	//	if(m_pDecalTextures)
	//	{
	//		for(int i=0; i<MAX_DECAL_NUM; i++)
	//		{		
	//			SAFE_RELEASE(m_pDecalTextures[i]);
	//		}
	//		util::del_array(m_pDecalTextures);
	//	}

	util::del_array(m_pDataHeader);
	m_pOrderTexture = nullptr;
	util::del(m_pGameDataToDecal);

	return CAtumNode::DeleteDeviceObjects();
}

void CSkinnedMesh::Tick(float fElapsedTime)
{
	SDrawElement* pdeCur;
	SFrame* pframeCur;

	pdeCur = m_pdeHead;
	while (pdeCur != nullptr)
	{
		pdeCur->fCurTime = fElapsedTime * 160;
		pframeCur = pdeCur->pframeAnimHead;
		while (pframeCur != nullptr)
		{
			pframeCur->SetTime(pdeCur->fCurTime);
			pframeCur = pframeCur->pframeAnimNext;
		}

		pdeCur = pdeCur->pdeNext;
	}
}

HRESULT CSkinnedMesh::SetProjectionMatrix() const
{
	//	FLOG( "CSkinnedMesh::SetProjectionMatrix()" );
	D3DXMATRIX mat;

	if (m_pdeHead == nullptr)
		return S_OK;

	FLOAT fAspect = 1.0f;
	D3DXMatrixPerspectiveFovRH(&mat, 0.25f * 3.141592654f, fAspect, m_pdeSelected->fRadius / 64, m_pdeSelected->fRadius * 200);
	HRESULT hr = m_pd3dDevice->SetTransform(D3DTS_PROJECTION, (D3DMATRIX*)&mat);
	if (FAILED(hr))
		return hr;
	// Set Projection Matrix for vertex shader
	D3DXMatrixTranspose(&mat, &mat);
	return S_OK;
}

COLLISION_RESULT CSkinnedMesh::CheckCollision(D3DXMATRIX mat, D3DXVECTOR3 vPos, float fCheckDistance, BOOL bUpdateFrame, BOOL bWithNormal)
{
	//	FLOG( "CSkinnedMesh::CheckCollision(D3DXMATRIX mat)" );
	COLLISION_RESULT collResult, checkcollResult;
	
	//	collResult.fDist = 10000.0f;
	//	collResult.vNormalVector = D3DXVECTOR3(0,0,0);
		
	for (auto pdeCur = m_pdeHead; pdeCur; pdeCur = pdeCur->pdeNext)
	{
		if (bUpdateFrame)
		
			UpdateFrames(pdeCur->pframeRoot, m_mWorld, vPos, fCheckDistance);
		
		checkcollResult = CheckCollDist(pdeCur->pframeRoot, mat, vPos, fCheckDistance, bWithNormal);
		
		if (collResult.fDist > checkcollResult.fDist)
		
			collResult = checkcollResult;
	}
	
	return collResult;
}

COLLISION_RESULT CSkinnedMesh::CheckCollDist(SFrame* pframeCur, D3DXMATRIX mat, D3DXVECTOR3 vPos, float fCheckDistance, BOOL bWithNormal)
{
	//	FLOG( "CSkinnedMesh::CheckCollDist(SFrame *pframeCur,D3DXMATRIX mat)" );
	COLLISION_RESULT collResult, checkcollResult;
	SMeshContainer* pmcMesh = nullptr;
	SFrame* pframeChild;

	if (pframeCur->pmcMesh != nullptr)
		m_pd3dDevice->SetTransform(D3DTS_WORLD, &pframeCur->matCombined);

	pmcMesh = pframeCur->pmcMesh;

	while (pmcMesh != nullptr)
	{
		//		D3DXVec3TransformCoord(&vMeshCenter,&pmcMesh->vCenter,&m_mWorld);				
		//		if(D3DXVec3Length(&(vMeshCenter - g_pShuttleChild->m_vPos)) - pmcMesh->fRadius < fCheckDistance )
		//		{
		//			checkcollResult = CheckCollDistDetail(pmcMesh,mat,bWithNormal);
		//			if(collResult.fDist > checkcollResult.fDist)
		//				collResult = checkcollResult;
		//		}
		checkcollResult = CheckCollDistDetail(pmcMesh, mat, bWithNormal);

		if (collResult.fDist > checkcollResult.fDist)

			collResult = checkcollResult;

		pmcMesh = pmcMesh->pmcNext;
	}
	pframeChild = pframeCur->pframeFirstChild;

	while (pframeChild != nullptr)
	{
		// 2005-01-05 by jschoi
		pmcMesh = pframeChild->pmcMesh;
		if (pmcMesh)
		{
			D3DXVECTOR3 vMeshCenter;

			D3DXVec3TransformCoord(&vMeshCenter, &pmcMesh->vCenter, &m_mWorld);

			vMeshCenter -= vPos;

			if (D3DXVec3Length(&vMeshCenter) - pmcMesh->fRadius < fCheckDistance)
			{
				checkcollResult = CheckCollDist(pframeChild, mat, vPos, fCheckDistance, bWithNormal);

				if (collResult.fDist > checkcollResult.fDist) collResult = checkcollResult;
			}
		}
		else
		{
			checkcollResult = CheckCollDist(pframeChild, mat, vPos, fCheckDistance, bWithNormal);

			if (collResult.fDist > checkcollResult.fDist) collResult = checkcollResult;
		}

		//		checkcollResult = CheckCollDist(pframeChild,mat,fCheckDistance,bWithNormal);
		//		if(collResult.fDist > checkcollResult.fDist)
		//			collResult = checkcollResult;

		pframeChild = pframeChild->pframeSibling;
	}
	return collResult;
}

COLLISION_RESULT CSkinnedMesh::CheckCollDistDetail(SMeshContainer* pmcMesh, const D3DXMATRIX& mat, BOOL bWithNormal)
{
	COLLISION_RESULT collResult;

	if (pmcMesh->m_pSkinMesh)
	{
		if (m_method == SOFTWARE)
		{
			D3DXMATRIX Identity;
			DWORD cBones = pmcMesh->m_pSkinMeshInfo->GetNumBones();
			// set up bone transforms
			for (DWORD iBone = 0; iBone < cBones; ++iBone)
			{
				D3DXMatrixMultiply
					(
						&m_pBoneMatrices[iBone], // output
						&pmcMesh->m_pBoneOffsetMat[iBone],
						pmcMesh->m_pBoneMatrix[iBone]
					);
			}
			// set world transform
			D3DXMatrixIdentity(&Identity);
			m_pd3dDevice->SetTransform(D3DTS_WORLD, &Identity);
			// generate skinned mesh
			if (!pmcMesh->pMesh)
			{
				//				collResult.fDist = DEFAULT_COLLISION_DISTANCE;
				return collResult;
			}
			//            pmcMesh->m_pSkinMesh->UpdateSkinnedMesh(m_pBoneMatrices, NULL, pmcMesh->pMesh);
		}
	}

	DWORD dwFace; BOOL bHit = false; // , bIntersections;
	FLOAT fBary1, fBary2, fDist;
	//	D3DXMATRIX matProj;
	D3DXVECTOR3 vPickRayDir, vPickRayOrig;
	D3DXMATRIX m, matWorld, matCombinedInv;
	//	g_pD3dDev->GetTransform( D3DTS_PROJECTION, &matProj );
	// Get the inverse view matrix
	g_pD3dDev->GetTransform(D3DTS_WORLD, &matWorld);
	m = matWorld * mat;
	D3DXMatrixInverse(&m, nullptr, &m);
	vPickRayDir.x = m._31;
	vPickRayDir.y = m._32;
	vPickRayDir.z = m._33;
	vPickRayOrig.x = m._41;
	vPickRayOrig.y = m._42;
	vPickRayOrig.z = m._43;

	D3DXMatrixInverse(&matCombinedInv, nullptr, &matWorld);

	auto destdir = vPickRayOrig + vPickRayDir;
	auto orig = vPickRayOrig;

	D3DXVec3TransformCoord(&destdir, &destdir,&matCombinedInv);
	D3DXVec3TransformCoord(&orig, &orig,&matCombinedInv);

	destdir -= orig;
	D3DXVec3Normalize(&destdir, &destdir);

	//if (D3DXBoxBoundProbe(&pmcMesh->m_vecMinXYZ, &pmcMesh->m_vecMaxXYZ, &vPickRayOrig, &vPickRayDir))
		D3DXIntersect(pmcMesh->pMesh, &vPickRayOrig, &vPickRayDir, &bHit, &dwFace, &fBary1, &fBary2, &fDist, nullptr, nullptr);

	if (bHit)
	{
		//bIntersections = TRUE;
		m_Intersection.dwFace = dwFace;
		m_Intersection.fBary1 = fBary1;
		m_Intersection.fBary2 = fBary2;
		m_Intersection.fDist = fDist;
	//}
	//else
	//{
	//	bIntersections = FALSE;
	//}
		// why have this?
	//if (bIntersections)
	//{
		if (bWithNormal) // 2004-11-03 by jschoi 법선 벡터가 필요할때만 구한다.
		{
			WORD* pIndices;
			D3DVERTEX* pVertices;

			D3DVERTEX vThisTri[3];
			WORD* iThisTri;

			//			LPDIRECT3DVERTEXBUFFER9 pVB;
			//			LPDIRECT3DINDEXBUFFER9  pIB;	
			//			pmcMesh->pMesh->GetVertexBuffer(&pVB);
			//			pmcMesh->pMesh->GetIndexBuffer( &pIB );
			//			pIB->Lock( 0,0,(void**)&pIndices, 0 );
			//			pVB->Lock( 0,0,(void**)&pVertices, 0 );

			pmcMesh->pMesh->LockVertexBuffer(D3DLOCK_READONLY, (LPVOID*)&pVertices);
			pmcMesh->pMesh->LockIndexBuffer(D3DLOCK_READONLY, (LPVOID*)&pIndices);

			iThisTri = &pIndices[3 * m_Intersection.dwFace];
			// get vertices hit
			vThisTri[0] = pVertices[iThisTri[0]];
			vThisTri[1] = pVertices[iThisTri[1]];
			vThisTri[2] = pVertices[iThisTri[2]];

			pmcMesh->pMesh->UnlockVertexBuffer();
			pmcMesh->pMesh->UnlockIndexBuffer();
			//			pVB->Unlock();
			//			pIB->Unlock();

			//			pVB->Release();
			//			pIB->Release();	

			D3DXVec3TransformCoord(&vThisTri[0].p, &vThisTri[0].p, &matWorld);
			D3DXVec3TransformCoord(&vThisTri[1].p, &vThisTri[1].p, &matWorld);
			D3DXVec3TransformCoord(&vThisTri[2].p, &vThisTri[2].p, &matWorld);
			//			D3DXVec3TransformNormal(&vThisTri[0].n, &vThisTri[0].n, &matWorld);
			//			D3DXVec3TransformNormal(&vThisTri[1].n, &vThisTri[1].n, &matWorld);
			//			D3DXVec3TransformNormal(&vThisTri[2].n, &vThisTri[2].n, &matWorld);

			// 법선을 구하자.

			D3DXVECTOR3 vTempNormal, vNormalVector;
			D3DXVECTOR3 vCross1, vCross2;
			vCross1 = vThisTri[0].p - vThisTri[1].p;
			vCross2 = vThisTri[1].p - vThisTri[2].p;
			D3DXVec3Cross(&vTempNormal, &vCross1, &vCross2);
			D3DXVec3Normalize(&vNormalVector, &vTempNormal);

			//			vNormalVector = vThisTri[0].n + vThisTri[1].n + vThisTri[2].n;
			//			D3DXVec3Normalize(&vNormalVector,&vNormalVector);

			collResult.vNormalVector = vNormalVector;
		}

		collResult.fDist = fDist;

		//return collResult; // 어쩌구 구조체;
	}

	return collResult;// 어쩌구 구조체 충돌 없다.;
}

void CSkinnedMesh::Render(BOOL bMultiTexture, DWORD nType)
{
	FLOG("CSkinnedMesh::Render(BOOL bMultiTexture)");

	if (nType == _SHUTTLE) m_unTexColor = 0;

	m_bMultiTexture = bMultiTexture;

	UINT cTriangles = 0;

	for (auto pdeCur = m_pdeHead; pdeCur != nullptr; pdeCur = pdeCur->pdeNext)
	{
		if (FAILED(UpdateFrames(pdeCur->pframeRoot, m_mWorld))) return;

		if (FAILED(DrawFrames(pdeCur->pframeRoot, cTriangles, nType))) return;
	}
}

// by dhkwon, 030917, 텍스쳐 강제 세팅을 위해 사용, 렌더링 후에는 이함수를 통해 원래대로 돌려야 한다.
// ReSharper disable once CppMemberFunctionMayBeConst
LPDIRECT3DTEXTURE9 CSkinnedMesh::SetTexture(LPDIRECT3DTEXTURE9 pTexture, BYTE tex_num)
{
	FLOG("CSkinnedMesh::SetTexture(LPDIRECT3DTEXTURE8 pTexture, BYTE tex_num)");
	// 2007-08-29 by bhsohn 최소 프레임시 기본 아머만 로딩하게끔 변경
	if (IsLoadTexture(tex_num) == FALSE) tex_num = 0;

	auto pTemp = m_pTexture[tex_num];
	m_pTexture[tex_num] = pTexture;
	return pTemp;
}

/* Devx Unused
HRESULT CSkinnedMesh::LoadMeshHierarchy(char* szFileName)
{
	FLOG("CSkinnedMesh::LoadMeshHierarchy(char* szFileName)");
	TCHAR* pszFile = szFileName;
	SDrawElement *pdeMesh = NULL;
	HRESULT hr = S_OK;
	LPDIRECTXFILE pxofapi = NULL;
	LPDIRECTXFILEENUMOBJECT pxofenum = NULL;
	LPDIRECTXFILEDATA pxofobjCur = NULL;
	DWORD dwOptions;
	int cchFileName;

	if (pszFile == NULL)
		return E_INVALIDARG;

	pdeMesh = new SDrawElement();

	delete pdeMesh->pframeRoot;
	pdeMesh->pframeAnimHead = NULL;

	pdeMesh->pframeRoot = new SFrame();
	if (pdeMesh->pframeRoot == NULL)
	{
		hr = E_OUTOFMEMORY;
		goto e_Exit;
	}


	dwOptions = 0; // D3DXMESH_MANAGED;

	cchFileName = strlen(pszFile);
	if (cchFileName < 2)
	{
		hr = E_FAIL;
		goto e_Exit;
	}

	hr = DirectXFileCreate(&pxofapi);
	if (FAILED(hr))
		goto e_Exit;

	// Register templates for d3drm.
	hr = pxofapi->RegisterTemplates((LPVOID)D3DRM_XTEMPLATES,
									D3DRM_XTEMPLATE_BYTES);
	if (FAILED(hr))
		goto e_Exit;


	hr = pxofapi->RegisterTemplates((LPVOID)szTemplates,
									strlen(szTemplates));
	if (FAILED(hr))
		goto e_Exit;


	// Create enum object.
	hr = pxofapi->CreateEnumObject((LPVOID)pszFile,
								   DXFILELOAD_FROMFILE,
								   &pxofenum);
	if (FAILED(hr))
		goto e_Exit;


	// Enumerate top level objects.
	// Top level objects are always data object.
	while (SUCCEEDED(pxofenum->GetNextDataObject(&pxofobjCur)))
	{
		hr = LoadFrames(pxofobjCur, pdeMesh, dwOptions, m_dwFVF,
						m_pd3dDevice,
						pdeMesh->pframeRoot);
		GXRELEASE(pxofobjCur);

		if (FAILED(hr))
			goto e_Exit;
	}

	hr = FindBones(pdeMesh->pframeRoot, pdeMesh);
	if (FAILED(hr))
		goto e_Exit;


	delete[]pdeMesh->szName;
	pdeMesh->szName = new char[cchFileName + 1];
	if (pdeMesh->szName == NULL)
	{
		hr = E_OUTOFMEMORY;
		goto e_Exit;
	}
	memcpy(pdeMesh->szName, pszFile, cchFileName + 1);

	// delete the current mesh, now that the load has succeeded
	DeleteSelectedMesh();

	// link into the draw list
	pdeMesh->pdeNext = m_pdeHead;
	m_pdeHead = pdeMesh;

	m_pdeSelected = pdeMesh;
	m_pmcSelectedMesh = pdeMesh->pframeRoot->pmcMesh;


	m_pframeSelected = pdeMesh->pframeRoot;

	hr = CalculateBoundingSphere(pdeMesh);
	m_fRadius = pdeMesh->fRadius;
	m_vCenter = pdeMesh->vCenter;

	if (FAILED(hr))
		goto e_Exit;

	SetProjectionMatrix();

	m_pdeSelected->fCurTime = 0.0f;
	m_pdeSelected->fMaxTime = 2000.0f;

	D3DXMatrixTranslation(&m_pdeSelected->pframeRoot->matRot,
						  -pdeMesh->vCenter.x, -pdeMesh->vCenter.y, -pdeMesh->vCenter.z);
	m_pdeSelected->pframeRoot->matRotOrig = m_pdeSelected->pframeRoot->matRot;

e_Exit:
	// 2009. 11. 23 by jskim 리소스 로딩 구조 변경
	//     GXRELEASE(pxofobjCur);
	//     GXRELEASE(pxofenum);
	//     GXRELEASE(pxofapi);
	//     
	//     if (FAILED(hr))
	//     {
	// 		// 에러 발생
	// 		g_pD3dApp->CheckError(CLIENT_ERR_MESHLOAD);  
	//         delete pdeMesh;
	//     }
	// 
	//     SDrawElement *pdeCur;
	//     pdeCur = m_pdeHead;
	//     while (pdeCur != NULL)
	//     {
	//         D3DXMatrixIdentity(&pdeCur->pframeRoot->matRot);
	//         D3DXMatrixIdentity(&pdeCur->pframeRoot->matTrans);
	//         pdeCur = pdeCur->pdeNext;
	//     }
	ReleaseData(hr);
	//end 2009. 11. 23 by jskim 리소스 로딩 구조 변경
	return hr;
}
*/

HRESULT CSkinnedMesh::LoadMeshHierarchyFromMem(CGameData* pGameData, int nMeshIndex, D3DXVECTOR3 vObjScale)
{
	FLOG("CSkinnedMesh::LoadMeshHierarchyFromMem(CGameData* pGameData)");

	pdeMesh = new SDrawElement();
	//FileLoadMemory = new DXFILELOADMEMORY;
	m_pGameData = pGameData;
	delete pdeMesh->pframeRoot;
	pdeMesh->pframeAnimHead = nullptr;
	pdeMesh->pframeRoot = new SFrame();

	auto hr = S_OK;

	UpdateMeshObjName(m_szMeshObjFileName);

	auto totalNum = m_pGameData->GetTotalNumber();
	m_pDataHeader = new DataHeader[totalNum];

	char buf[32];
	memset(buf, 0x00, 32);

	util::del_array(m_pLoadMem);

	m_pLoadMem = new BOOL[totalNum];

	auto nCnt = 0;

	for (nCnt = 0; nCnt < totalNum; nCnt++) m_pLoadMem[nCnt] = TRUE;

	auto bMyArmor = false;

	if (IsArmorMesh(nMeshIndex))
	{
		if (_SELECT == g_pD3dApp->GetGameState()) bMyArmor = true;
		else if (NULL != g_pShuttleChild && nMeshIndex == g_pShuttleChild->GetUnitNum()) bMyArmor = true;
	}

	auto bDefaultArmor = (IsArmorMesh(nMeshIndex) && IsLowQuality() && (!bMyArmor));

	if (IsArmorMesh(nMeshIndex)) bDefaultArmor = true;

	if (IsArmorMesh(nMeshIndex))
	{
		char szArmorNum[32];
		itoa(nMeshIndex, szArmorNum, 10);
		szArmorNum[0] = '0';
		szArmorNum[1] = '8';

		auto nArmorItemNum = atoi(szArmorNum);

		auto pItem = g_pDatabase->GetServerItemInfo(nArmorItemNum);

		if (pItem && COMPARE_BIT_FLAG(pItem->ItemAttribute, ITEM_ATTR_ROBOT_ARMOR)) bDefaultArmor = false;
	}

	if (bDefaultArmor) for (nCnt = 1; nCnt < totalNum - 2; nCnt++) m_pLoadMem[nCnt] = FALSE;

	auto pHeader = m_pGameData->GetStartPosition();

	auto i = 0;

	while (pHeader)
	{
		m_pDataHeader[i] = *pHeader;
		m_pDataHeader[i++].m_pData = pHeader->m_pData;
		pHeader = m_pGameData->GetNext();
	}

	if (pdeMesh->pframeRoot == nullptr)
	{
		hr = E_OUTOFMEMORY;
		goto e_Exit;
	}

	//dwOptions = 0;
	dwOptions = D3DXMESH_MANAGED;

	hr = D3DXFileCreate(&pxofapi);

	if (FAILED(hr)) goto e_Exit;

	// Register templates for d3drm.
	hr = pxofapi->RegisterTemplates((LPVOID)D3DRM_XTEMPLATES, D3DRM_XTEMPLATE_BYTES);
	if (FAILED(hr)) goto e_Exit;

	hr = pxofapi->RegisterTemplates((LPVOID)szTemplates, strlen(szTemplates));

	if (FAILED(hr)) goto e_Exit;

	FileLoadMemory.lpMemory = m_pDataHeader[0].m_pData;
	FileLoadMemory.dSize = m_pDataHeader[0].m_DataSize;

	// Create enum object.
	hr = pxofapi->CreateEnumObject((LPCVOID)&FileLoadMemory, D3DXF_FILEFORMAT_BINARY | D3DXF_FILELOAD_FROMMEMORY, &pxofenum);
	if (FAILED(hr)) goto e_Exit;

	// Create enum texture
	if (totalNum > 1)
	{
		m_pTexture = new LPDIRECT3DTEXTURE9[totalNum - 1];

		m_bTotalTextureNum = totalNum - 1;

		for (i = 1; i < totalNum; i++)
		{
			int nLoadIdx = i - 1;

			if (FALSE == IsLoadTexture(nLoadIdx)) continue;

			if (FAILED(D3DXCreateTextureFromFileInMemory(m_pd3dDevice, (LPCVOID)m_pDataHeader[i].m_pData,
				m_pDataHeader[i].m_DataSize, &(m_pTexture[i - 1]))))
			{
				hr = E_FAIL;
				if (g_pD3dApp && g_pD3dApp->m_pChat)
				{
					char errStr[512];
					sprintf_s(errStr, 512, STRERR_C_RESOURCE_0012, m_pDataHeader[i].m_FileName);
					g_pD3dApp->m_pChat->CreateChatChild(errStr, COLOR_ERROR);// "리소스 파일 로딩에 문제가 있어서 오브젝트 생성이 중단됨."
				}
				DBGOUT("\n ERROR : Loading Resource File .\n\n");
				goto e_Exit;
			}
		}
	}
	else
	{
		m_pTexture = new LPDIRECT3DTEXTURE9[1];
		m_pTexture[0] = nullptr;
	}

	ASSERT_ASSERT(m_pTexture);

	// Enumerate top level objects.
	// Top level objects are always data object.
	//while (SUCCEEDED(pxofenum->GetNextDataObject(&pxofobjCur)))

	SIZE_T children = 0;
	hr = pxofenum->GetChildren(&children);
	if (FAILED(hr)) goto e_Exit;

	for (SIZE_T child = 0; child < children; child++)
	{
		if (SUCCEEDED(pxofenum->GetChild(child, &pxofobjCur)))
		{
			hr = LoadFrames(pxofobjCur, pdeMesh, dwOptions, m_dwFVF, m_pd3dDevice, pdeMesh->pframeRoot);

			GXRELEASE(pxofobjCur);

			if (FAILED(hr)) goto e_Exit;
		}
	}

	hr = FindBones(pdeMesh->pframeRoot, pdeMesh);

	if (FAILED(hr)) goto e_Exit;

	delete[]pdeMesh->szName;
	pdeMesh->szName = nullptr;

	// delete the current mesh, now that the load has succeeded
	DeleteSelectedMesh();

	// link into the draw list
	pdeMesh->pdeNext = m_pdeHead;
	m_pdeHead = pdeMesh;

	m_pdeSelected = pdeMesh;
	m_pmcSelectedMesh = pdeMesh->pframeRoot->pmcMesh;

	m_pframeSelected = pdeMesh->pframeRoot;

	hr = CalculateBoundingSphere(pdeMesh, vObjScale);
	m_fRadius = pdeMesh->fRadius;
	m_vCenter = pdeMesh->vCenter;

	if (FAILED(hr)) goto e_Exit;

	SetProjectionMatrix();

	m_pdeSelected->fCurTime = 0.0f;
	m_pdeSelected->fMaxTime = 2000.0f;

	D3DXMatrixTranslation(&m_pdeSelected->pframeRoot->matRot, -pdeMesh->vCenter.x, -pdeMesh->vCenter.y, -pdeMesh->vCenter.z);
	m_pdeSelected->pframeRoot->matRotOrig = m_pdeSelected->pframeRoot->matRot;

e_Exit:
	ReleaseData(hr);
	return hr;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2007-08-29 by bhsohn 최소 프레임시 기본 아머만 로딩하게끔 변경
/// \date		2007-08-29 ~ 2007-08-29
/// \warning	
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CSkinnedMesh::IsArmorMesh(int nMeshIndex)
{
	if (nMeshIndex >= 12000000 && nMeshIndex < 13000000)
	{
		return TRUE;
	}
	return FALSE;
}

// ReSharper disable once CppMemberFunctionMayBeConst
HRESULT CSkinnedMesh::LoadTexture(char* szfileName, BYTE tex_num)
{
	FLOG("CSkinnedMesh::LoadTexture(char* szfileName, BYTE tex_num)");
	if (FAILED(D3DXCreateTextureFromFile(m_pd3dDevice, szfileName, &m_pTexture[tex_num - 1]))) return E_FAIL;
	return TRUE;
}

HRESULT CSkinnedMesh::LoadFrames(LPD3DXFILEDATA pxofobjCur, SDrawElement* pde, DWORD options, DWORD fvf, LPDIRECT3DDEVICE9 pD3DDevice, SFrame* pframeParent)
{
	//	FLOG( "CSkinnedMesh::LoadFrames(LPDIRECTXFILEDATA pxofobjCur, SDrawElement *pde, DWORD options, DWORD fvf, LPDIRECT3DDEVICE9 pD3DDevice, SFrame *pframeParent)" );
	auto hr = S_OK;
	LPD3DXFILEDATA pxofobjChild = nullptr;
	LPD3DXFILEDATA pxofChild = nullptr;
	//LPDIRECTXFILEOBJECT pxofChild = NULL;
	GUID type;
	SIZE_T cbSize;
	D3DXMATRIX* pmatNew;
	SFrame* pframeCur;
	DWORD cchName;

	// Get the type of the object
	hr = pxofobjCur->GetType(&type);

	if (FAILED(hr)) goto e_Exit;

	if (type == TID_D3DRMMesh)
	{
		hr = LoadMesh(pxofobjCur, options, fvf, pD3DDevice, pframeParent);

		if (FAILED(hr)) goto e_Exit;
	}
	else if (type == TID_D3DRMFrameTransformMatrix)
	{
		//hr = pxofobjCur->GetData(NULL, &cbSize, (PVOID*)&pmatNew);

		hr = pxofobjCur->Lock(&cbSize, (LPCVOID*)&pmatNew);

		if (FAILED(hr)) goto e_Exit;

		// update the parents matrix with the new one
		pframeParent->matRot = *pmatNew;
		pframeParent->matRotOrig = *pmatNew;
	}
	else if (type == TID_D3DRMAnimationSet)
	{
		LoadAnimationSet(pxofobjCur, pde, options, fvf, pD3DDevice, pframeParent);
	}
	else if (type == TID_D3DRMAnimation)
	{
		LoadAnimation(pxofobjCur, pde, options, fvf, pD3DDevice, pframeParent);
	}
	else if (type == TID_D3DRMFrame)
	{
		pframeCur = new SFrame();
		if (pframeCur == nullptr)
		{
			hr = E_OUTOFMEMORY;
			goto e_Exit;
		}

		hr = pxofobjCur->GetName(nullptr, &cchName);
		if (FAILED(hr))
			goto e_Exit;

		if (cchName > 0)
		{
			pframeCur->szName = new char[cchName];
			if (pframeCur->szName == nullptr)
			{
				hr = E_OUTOFMEMORY;
				goto e_Exit;
			}

			hr = pxofobjCur->GetName(pframeCur->szName, &cchName);
			if (FAILED(hr))
				goto e_Exit;
		}

		pframeParent->AddFrame(pframeCur);

		// Enumerate child objects.
		// Child object can be data, data reference or binary.
		// Use QueryInterface() to find what type of object a child is.
		//while (SUCCEEDED(pxofobjCur->GetNextObject(&pxofChild)))

		SIZE_T children = 0;
		hr = pxofobjCur->GetChildren(&children);
		if (FAILED(hr)) goto e_Exit;

		for (SIZE_T child = 0; child < children; child++)
		{
			if (SUCCEEDED(pxofobjCur->GetChild(child, &pxofChild)))
			{
				if (!pxofChild->IsReference())
				{
					// Query the child for it's FileData
					hr = pxofChild->QueryInterface(IID_ID3DXFileData, (LPVOID *)&pxofobjChild);

					if (SUCCEEDED(hr))
					{
						hr = LoadFrames(pxofobjChild, pde, options, fvf, pD3DDevice, pframeCur);

						if (FAILED(hr)) goto e_Exit;

						GXRELEASE(pxofobjChild);
					}
				}

				GXRELEASE(pxofChild);
			}
		}
	}

e_Exit:
	GXRELEASE(pxofobjChild);
	GXRELEASE(pxofChild);
	return hr;
}

HRESULT CSkinnedMesh::LoadMesh(LPD3DXFILEDATA pxofobjCur, DWORD options, DWORD fvf, LPDIRECT3DDEVICE9 pD3DDevice, SFrame* pframeParent)
{
	//	FLOG( "CSkinnedMesh::LoadMesh(LPDIRECTXFILEDATA pxofobjCur, DWORD options, DWORD fvf, LPDIRECT3DDEVICE9 pD3DDevice, SFrame *pframeParent)" );
	HRESULT hr = S_OK;
	SMeshContainer* pmcMesh = nullptr;
	LPD3DXBUFFER pbufMaterials = nullptr;
	LPD3DXBUFFER pbufAdjacency = nullptr;
	DWORD cchName;
	UINT cFaces;
	UINT iMaterial;
	LPDIRECT3DDEVICE9 m_pDevice = m_pd3dDevice;
	LPDWORD pAdjacencyIn;
	//    LPD3DXPMESH  pPMesh = NULL;
	//   LPD3DXPMESH  pPMeshTemp = NULL;
	//   LPD3DXMESH   pMesh = NULL;
	//    LPD3DXMESH   pTempMesh = NULL;
	int i;
	//	DWORD num;

	// 2005-01-05 by jschoi
	UINT iBone, cBones;

	pmcMesh = new SMeshContainer();
	//	for(i = 0;i < 5;i++)
	//		pmcMesh->pMeshes[i] = NULL;
	if (pmcMesh == nullptr)
	{
		hr = E_OUTOFMEMORY;
		goto e_Exit;
	}

	hr = pxofobjCur->GetName(nullptr, &cchName);

	if (FAILED(hr)) goto e_Exit;

	if (cchName > 0)
	{
		pmcMesh->szName = new char[cchName];
		if (pmcMesh->szName == nullptr)
		{
			hr = E_OUTOFMEMORY;
			goto e_Exit;
		}

		hr = pxofobjCur->GetName(pmcMesh->szName, &cchName);
		if (FAILED(hr))
			goto e_Exit;
	}

	// 2005-01-04 by jschoi
	//    hr = D3DXLoadSkinMeshFromXof(pxofobjCur, options, pD3DDevice, &pbufAdjacency, &pbufMaterials, &pmcMesh->cMaterials, 
	//        &pmcMesh->m_pBoneNamesBuf, &pmcMesh->m_pBoneOffsetBuf, &pmcMesh->m_pSkinMesh);
	hr = D3DXLoadSkinMeshFromXof(pxofobjCur, options, pD3DDevice, &pbufAdjacency, &pbufMaterials, nullptr, &pmcMesh->cMaterials,
	                             &pmcMesh->m_pSkinMeshInfo, &pmcMesh->m_pSkinMesh);


	//	DWORD num;//,num2,num3;

	if (FAILED(hr))
	{
		if (hr == D3DXERR_LOADEDMESHASNODATA)
			hr = S_OK;
		goto e_Exit;
	}


	cFaces = pmcMesh->m_pSkinMesh->GetNumFaces();

	pAdjacencyIn = static_cast<LPDWORD>(pbufAdjacency->GetBufferPointer());

	pmcMesh->m_rgiAdjacency = new DWORD[cFaces * 3];

	if (pmcMesh->m_rgiAdjacency == nullptr)
	{
		hr = E_OUTOFMEMORY;

		goto e_Exit;
	}

	memcpy(pmcMesh->m_rgiAdjacency, pAdjacencyIn, cFaces * 3 * sizeof(DWORD));

	// Process skinning data
	if (pmcMesh->m_pSkinMeshInfo->GetNumBones() != 0)
	{
		cBones = pmcMesh->m_pSkinMeshInfo->GetNumBones();
		// Update max bones of any mesh in the app
		m_maxBones = max(cBones, m_maxBones);

		pmcMesh->m_pBoneMatrix = new D3DXMATRIX*[cBones];
		pmcMesh->m_pBoneOffsetMat = new D3DXMATRIX[cBones];
		if (pmcMesh->m_pBoneMatrix == nullptr)
			goto e_Exit;
		// 2005-01-05 by jschoi
		//      pmcMesh->m_pBoneOffsetMat = reinterpret_cast<D3DXMATRIX*>(pmcMesh->m_pBoneOffsetBuf->GetBufferPointer());
		for (iBone = 0; iBone < cBones; iBone++)
		{
			pmcMesh->m_pBoneOffsetMat[iBone] = *(pmcMesh->m_pSkinMeshInfo->GetBoneOffsetMatrix(iBone));
		}

		hr = GenerateMesh(pmcMesh);

		if (FAILED(hr))
			goto e_Exit;
		/////////////////////// 본데이타를 무시하고 Temp Mesh를 생성하고 Progressive 를 적용한다 /////////////////////////////////////
		//		if(m_bProgressiveMesh)
		//		{
		//	        pmcMesh->m_pSkinMesh->GetOriginalMesh(&pTempMesh);
		//			D3DXCleanMesh(pTempMesh, (DWORD*)pbufAdjacency->GetBufferPointer(), &pMesh, (DWORD*)pbufAdjacency->GetBufferPointer(), NULL );
		//		}
	}
	else
	{
		pmcMesh->pMesh = pmcMesh->m_pSkinMesh;
		pmcMesh->m_pSkinMesh = nullptr;

		SAFE_RELEASE(pmcMesh->m_pSkinMeshInfo);
		pmcMesh->m_pSkinMeshInfo = nullptr;

		pmcMesh->cpattr = pmcMesh->cMaterials;
	}

	if ((pbufMaterials == nullptr) || (pmcMesh->cMaterials == 0))
	{
		pmcMesh->rgMaterials = new D3DMATERIAL9[1];
		pmcMesh->pTextures = new LPDIRECT3DTEXTURE9[1];
		if (pmcMesh->rgMaterials == nullptr || pmcMesh->pTextures == nullptr)
		{
			hr = E_OUTOFMEMORY;
			goto e_Exit;
		}

		memset(pmcMesh->rgMaterials, 0, sizeof(D3DMATERIAL9));
		pmcMesh->rgMaterials[0].Diffuse.r = 1.0f;
		pmcMesh->rgMaterials[0].Diffuse.g = 1.0f;
		pmcMesh->rgMaterials[0].Diffuse.b = 1.0f;
		pmcMesh->rgMaterials[0].Ambient = pmcMesh->rgMaterials[0].Diffuse;
		pmcMesh->pTextures[0] = nullptr;
	}
	else
	{
		pmcMesh->rgMaterials = new D3DMATERIAL9[pmcMesh->cMaterials];
		pmcMesh->pTextures = new LPDIRECT3DTEXTURE9[pmcMesh->cMaterials];
		if (pmcMesh->rgMaterials == nullptr || pmcMesh->pTextures == nullptr)
		{
			hr = E_OUTOFMEMORY;
			goto e_Exit;
		}

		LPD3DXMATERIAL pMaterials = (LPD3DXMATERIAL)pbufMaterials->GetBufferPointer();

		for (iMaterial = 0; iMaterial < pmcMesh->cMaterials; iMaterial++)
		{
			pmcMesh->rgMaterials[iMaterial] = pMaterials[iMaterial].MatD3D;
			pmcMesh->rgMaterials[iMaterial].Ambient.r = 1.0f;
			pmcMesh->rgMaterials[iMaterial].Ambient.g = 1.0f;
			pmcMesh->rgMaterials[iMaterial].Ambient.b = 1.0f;

			pmcMesh->pTextures[iMaterial] = nullptr;
			if (pMaterials[iMaterial].pTextureFilename != nullptr)
			{
				//				ChangeFilePath(pMaterials[iMaterial].pTextureFilename);
				if (m_pDataHeader && m_pGameData)
				{
					int num = m_pGameData->GetTotalNumber();
					for (i = 1; i < num; i++)
					{
						// 2007-08-29 by bhsohn 최소 프레임시 기본 아머만 로딩하게끔 변경
						int nLoadIdx = i - 1;
						if (FALSE == IsLoadTexture(nLoadIdx))
						{
							continue;
						}
						// end 2007-08-29 by bhsohn 최소 프레임시 기본 아머만 로딩하게끔 변경

						// ChangeFilePath(m_pDataHeader[i].m_FileName);
						// int nCount = strlen(pMaterials[iMaterial].pTextureFilename) - strlen(m_pDataHeader[i].m_FileName);

						if (strcmp(m_pDataHeader[i].m_FileName, pMaterials[iMaterial].pTextureFilename) == 0 && m_pTexture != nullptr)
						{
							// 2005-03-17 by jschoi - 널체크

							if (m_pTexture[i - 1]) m_pTexture[i - 1]->AddRef();
							else DBGOUT("Texture is NULL.\n");

							pmcMesh->pTextures[iMaterial] = m_pTexture[i - 1];

							break;
						}
					}
				}
				else
				{
					TCHAR szPath[MAX_PATH];
					//	DXUtil_FindMediaFile(szPath, pMaterials[iMaterial].pTextureFilename);
					DXUtil_FindMediaFileCb(szPath, sizeof(szPath), pMaterials[iMaterial].pTextureFilename);
					hr = D3DXCreateTextureFromFile(m_pDevice, szPath, &(pmcMesh->pTextures[iMaterial]));
					if (FAILED(hr))
						pmcMesh->pTextures[iMaterial] = nullptr;
				}
			}
		}
	}
	// add the mesh to the parent frame
	pframeParent->AddMesh(pmcMesh);
	/* 	// 임시
		if(m_bProgressiveMesh)
		{
		DWORD maxnum = pMesh->GetNumVertices();
		DWORD minnum = maxnum/10;
		num = (DWORD)((maxnum-minnum)/5);
		for(i = 0;i < 5;i++)
		D3DXSimplifyMesh(pMesh,(DWORD*)pbufAdjacency->GetBufferPointer(),NULL,NULL,(maxnum-(num*(i+1))),D3DXMESHSIMP_VERTEX,&pmcMesh->pMeshes[i]);
		SAFE_RELEASE(pTempMesh);
		SAFE_RELEASE(pPMesh);
		}
		*/
	pmcMesh = nullptr;

e_Exit:
	util::del(pmcMesh); // 2006-07-05 by ispark
	//    delete pmcMesh;
	//	pmcMesh = NULL;

	//    SAFE_RELEASE( pMesh );
	//    SAFE_RELEASE( pPMesh );
	GXRELEASE(pbufAdjacency);
	GXRELEASE(pbufMaterials);
	return hr;
}

HRESULT CSkinnedMesh::GenerateMesh(SMeshContainer* pmcMesh)
{
	//	FLOG( "CSkinnedMesh::GenerateMesh(SMeshContainer *pmcMesh)" );
	// ASSUMPTION:  pmcMesh->m_rgiAdjacency contains the current adjacency
	auto hr = S_OK;
	LPDIRECT3DDEVICE9 pDevice = nullptr;

	// auto cFaces = pmcMesh->m_pSkinMesh->GetNumFaces();

	// DWORD* rgiAdjacency;

	hr = pmcMesh->m_pSkinMesh->GetDevice(&pDevice);

	if (FAILED(hr)) goto e_Exit;

	GXRELEASE(pmcMesh->pMesh);
	delete[] m_pBoneMatrices;

	pmcMesh->pMesh = nullptr;
	m_pBoneMatrices = nullptr;

	if (m_method == D3DNONINDEXED)
	{
		LPD3DXBONECOMBINATION rgBoneCombinations;

		hr = pmcMesh->m_pSkinMeshInfo->ConvertToBlendedMesh(
			            pmcMesh->m_pSkinMesh, D3DXMESH_MANAGED | D3DXMESHOPT_VERTEXCACHE, pmcMesh->m_rgiAdjacency,
			            nullptr, nullptr, /*&pmcMesh->cpattr*/ nullptr, &pmcMesh->m_maxFaceInfl,
			            &pmcMesh->m_numBoneComb, &pmcMesh->m_pBoneCombinationBuf, &pmcMesh->pMesh);

		if (FAILED(hr)) goto e_Exit;

		// calculate the max face influence count

		if ((pmcMesh->pMesh->GetFVF() & D3DFVF_POSITION_MASK) != D3DFVF_XYZ)

			pmcMesh->m_maxFaceInfl = 1 + ((pmcMesh->pMesh->GetFVF() & D3DFVF_POSITION_MASK) - D3DFVF_XYZRHW) / 2;

		else pmcMesh->m_maxFaceInfl = 1;

		/* If the device can only do 2 matrix blends, ConvertToBlendedMesh cannot approximate all meshes to it
			Thus we split the mesh in two parts: The part that uses at most 2 matrices and the rest. The first is
			drawn using the device's HW vertex processing and the rest is drawn using SW vertex processing. */
		if (m_d3dCaps.MaxVertexBlendMatrices <= 2)
		{
			// calculate the index of the attribute table to split on
			rgBoneCombinations = reinterpret_cast<LPD3DXBONECOMBINATION>(pmcMesh->m_pBoneCombinationBuf->GetBufferPointer());

			for (pmcMesh->iAttrSplit = 0; pmcMesh->iAttrSplit < pmcMesh->cpattr; pmcMesh->iAttrSplit++)
			{
				DWORD cInfl = 0;

				for (DWORD iInfl = 0; iInfl < pmcMesh->m_maxFaceInfl; iInfl++)

					if (rgBoneCombinations[pmcMesh->iAttrSplit].BoneId[iInfl] != UINT_MAX) ++cInfl;

				if (cInfl > m_d3dCaps.MaxVertexBlendMatrices) break;
			}

			// if there is both HW and SW, add the Software Processing flag
			if (pmcMesh->iAttrSplit < pmcMesh->cpattr)
			{
				LPD3DXMESH pMeshTmp;

				hr = pmcMesh->pMesh->CloneMeshFVF(D3DXMESH_SOFTWAREPROCESSING | pmcMesh->pMesh->GetOptions(),
				                                  pmcMesh->pMesh->GetFVF(), pDevice, &pMeshTmp);

				if (FAILED(hr)) goto e_Exit;

				pmcMesh->pMesh->Release();
				pmcMesh->pMesh = pMeshTmp;
				pMeshTmp = nullptr;
			}
		}
		else pmcMesh->iAttrSplit = pmcMesh->cpattr;
	}
	else if (m_method == D3DINDEXEDVS)
	{
		// Get palette size
		pmcMesh->m_paletteSize = min(28, pmcMesh->m_pSkinMeshInfo->GetNumBones());

		DWORD flags = D3DXMESHOPT_VERTEXCACHE;
		if (m_d3dCaps.VertexShaderVersion >= D3DVS_VERSION(1, 1))
		{
			pmcMesh->m_bUseSW = false;
			flags |= D3DXMESH_MANAGED;
		}
		else
		{
			pmcMesh->m_bUseSW = true;
			flags |= D3DXMESH_SYSTEMMEM;
		}

		hr = pmcMesh->m_pSkinMeshInfo->ConvertToIndexedBlendedMesh(pmcMesh->m_pSkinMesh,
		                                                           flags,
		                                                           pmcMesh->m_paletteSize,
		                                                           pmcMesh->m_rgiAdjacency,
		                                                           nullptr,
		                                                           nullptr, //&pmcMesh->cpattr
		                                                           nullptr,
		                                                           &pmcMesh->m_maxFaceInfl,
		                                                           &pmcMesh->m_numBoneComb,
		                                                           &pmcMesh->m_pBoneCombinationBuf,
		                                                           &pmcMesh->pMesh);
		if (FAILED(hr))
			goto e_Exit;

		if ((pmcMesh->pMesh->GetFVF() & D3DFVF_POSITION_MASK) != D3DFVF_XYZ)
		{
			pmcMesh->m_maxFaceInfl = ((pmcMesh->pMesh->GetFVF() & D3DFVF_POSITION_MASK) - D3DFVF_XYZRHW) / 2;
		}
		else
		{
			pmcMesh->m_maxFaceInfl = 1;
		}

		// FVF has to match our declarator. Vertex shaders are not as forgiving as FF pipeline
		DWORD newFVF = (pmcMesh->pMesh->GetFVF() & D3DFVF_POSITION_MASK) | D3DFVF_NORMAL | D3DFVF_TEX1 | D3DFVF_LASTBETA_UBYTE4;
		if (newFVF != pmcMesh->pMesh->GetFVF())
		{
			LPD3DXMESH pMesh;
			hr = pmcMesh->pMesh->CloneMeshFVF(pmcMesh->pMesh->GetOptions(), newFVF, pDevice, &pMesh);
			if (!FAILED(hr))
			{
				pmcMesh->pMesh->Release();
				pmcMesh->pMesh = pMesh;
				pMesh = nullptr;
			}
		}
	}
	else if (m_method == D3DINDEXED)
	{
		DWORD maxFaceInfl;
		DWORD flags = D3DXMESHOPT_VERTEXCACHE;

		// 2005-01-04 by jschoi
		//        hr = pmcMesh->m_pSkinMesh->GetMaxFaceInfluences(&maxFaceInfl);
		//        if (FAILED(hr))
		//            goto e_Exit;
		maxFaceInfl = pmcMesh->m_maxFaceInfl;

		// 12 entry palette guarantees that any triangle (4 independent influences per vertex of a tri)
		// can be handled
		maxFaceInfl = min(maxFaceInfl, 12);

		if (m_d3dCaps.MaxVertexBlendMatrixIndex + 1 < maxFaceInfl)
		{
			// HW does not support indexed vertex blending. Use SW instead
			pmcMesh->m_paletteSize = min(256, pmcMesh->m_pSkinMeshInfo->GetNumBones());
			pmcMesh->m_bUseSW = true;
			flags |= D3DXMESH_SYSTEMMEM;
		}
		else
		{
			pmcMesh->m_paletteSize = pmcMesh->m_pSkinMeshInfo->GetNumBones(); // min(m_d3dCaps.MaxVertexBlendMatrixIndex + 1, pmcMesh->m_pSkinMeshInfo->GetNumBones()); 2015-10-27 PanKJ
			pmcMesh->m_bUseSW = false;
			flags |= D3DXMESH_MANAGED;
		}

		hr = pmcMesh->m_pSkinMeshInfo->ConvertToIndexedBlendedMesh(pmcMesh->m_pSkinMesh,
		                                                           flags,
		                                                           pmcMesh->m_paletteSize,
		                                                           pmcMesh->m_rgiAdjacency,
		                                                           nullptr,
		                                                           nullptr, //&pmcMesh->cpattr
		                                                           nullptr,
		                                                           &pmcMesh->m_maxFaceInfl,
		                                                           &pmcMesh->m_numBoneComb,
		                                                           &pmcMesh->m_pBoneCombinationBuf,
		                                                           &pmcMesh->pMesh);
		if (FAILED(hr))
			goto e_Exit;

		// Here we are talking of max vertex influence which we determine from 
		// the FVF of the returned mesh
		if ((pmcMesh->pMesh->GetFVF() & D3DFVF_POSITION_MASK) != D3DFVF_XYZ)
		{
			pmcMesh->m_maxFaceInfl = ((pmcMesh->pMesh->GetFVF() & D3DFVF_POSITION_MASK) - D3DFVF_XYZRHW) / 2;
		}
		else
		{
			pmcMesh->m_maxFaceInfl = 1;
		}
	}
	else if (m_method == SOFTWARE)
	{
		// 2005-01-04 by jschoi
		//        hr = pmcMesh->m_pSkinMeshInfo->GenerateSkinnedMesh
		//                                   (
		//                                       D3DXMESH_DYNAMIC,          // options
		//                                       0.0f,                        // minimum bone weight allowed
		//                                       pmcMesh->m_rgiAdjacency,     // adjacency of in-mesh
		//                                       NULL,     // adjacency of out-mesh
		//                                       NULL,     // face remap array
		//                                       NULL,     // vertex remap buffer
		//                                       &pmcMesh->pMesh              // out-mesh
		//                                   );

		hr = pmcMesh->m_pSkinMesh->CloneMeshFVF(D3DXMESH_MANAGED, pmcMesh->m_pSkinMesh->GetFVF(), m_pd3dDevice, &pmcMesh->pMesh);

		if (FAILED(hr)) goto e_Exit;

		hr = pmcMesh->pMesh->GetAttributeTable(nullptr, &pmcMesh->cpattr);

		if (FAILED(hr)) goto e_Exit;

		delete[] pmcMesh->m_pAttrTable;
		pmcMesh->m_pAttrTable = new D3DXATTRIBUTERANGE[pmcMesh->cpattr];
		if (pmcMesh->m_pAttrTable == nullptr)
		{
			hr = E_OUTOFMEMORY;
			goto e_Exit;
		}

		hr = pmcMesh->pMesh->GetAttributeTable(pmcMesh->m_pAttrTable, nullptr);

		if (FAILED(hr)) goto e_Exit;

		// 2005-01-04 by jschoi
		//		hr = pmcMesh->m_pSkinMeshInfo->GetMaxFaceInfluences(&pmcMesh->m_maxFaceInfl);

		LPDIRECT3DINDEXBUFFER9 pIB;
		hr = pmcMesh->m_pSkinMesh->GetIndexBuffer(&pIB);

		if (FAILED(hr)) goto e_Exit;

		hr = pmcMesh->m_pSkinMeshInfo->GetMaxFaceInfluences(pIB, pmcMesh->m_pSkinMesh->GetNumFaces(), &pmcMesh->m_maxFaceInfl);

		pIB->Release();

		if (FAILED(hr)) goto e_Exit;

		// Allocate space for blend matrices
		m_pBoneMatrices = new D3DXMATRIXA16[m_maxBones];
		if (m_pBoneMatrices == nullptr)
		{
			hr = E_OUTOFMEMORY;
			goto e_Exit;
		}
	}
	pmcMesh->m_Method = m_method;

e_Exit:

	GXRELEASE(pDevice);
	return hr;
}

// ReSharper disable once CppMemberFunctionMayBeConst
HRESULT CSkinnedMesh::FindBones(SFrame* pframeCur, SDrawElement* pde)
{
	FLOG("CSkinnedMesh::FindBones(SFrame *pframeCur, SDrawElement *pde)");

	SMeshContainer* pmcMesh;
	SFrame* pframeChild;

	pmcMesh = pframeCur->pmcMesh;
	while (pmcMesh != nullptr)
	{
		if (pmcMesh->m_pSkinMesh)
		{
			for (DWORD i = 0; i < pmcMesh->m_pSkinMeshInfo->GetNumBones(); ++i)
			{
				auto strBoneName = pmcMesh->m_pSkinMeshInfo->GetBoneName(i);
				auto pFrame = pde->FindFrame(strBoneName);
				pmcMesh->m_pBoneMatrix[i] = &(pFrame->matCombined);
			}
		}
		pmcMesh = pmcMesh->pmcNext;
	}

	pframeChild = pframeCur->pframeFirstChild;

	while (pframeChild != nullptr)
	{
		auto hr = FindBones(pframeChild, pde);

		if (FAILED(hr)) return hr;

		pframeChild = pframeChild->pframeSibling;
	}

	return S_OK;
}

HRESULT CSkinnedMesh::LoadAnimation(LPD3DXFILEDATA pxofobjCur, SDrawElement* pde, DWORD options, DWORD fvf, LPDIRECT3DDEVICE9 pD3DDevice, SFrame* pframeParent)
{
	//	FLOG( "CSkinnedMesh::LoadAnimation(LPDIRECTXFILEDATA pxofobjCur, SDrawElement *pde, DWORD options, DWORD fvf, LPDIRECT3DDEVICE9 pD3DDevice, SFrame *pframeParent)" );
	HRESULT hr = S_OK;
	SRotateKeyXFile* pFileRotateKey;
	SScaleKeyXFile* pFileScaleKey;
	SPositionKeyXFile* pFilePosKey;
	SMatrixKeyXFile* pFileMatrixKey;
	SFrame* pframeCur;
	LPD3DXFILEDATA pxofobjChild = nullptr;
	LPD3DXFILEDATA pxofChild = nullptr;
	//LPDIRECTXFILEOBJECT pxofChild = NULL;
	//LPDIRECTXFILEDATAREFERENCE pxofobjChildRef = NULL;
	GUID type;
	DWORD dwSize;
	const BYTE* pData;
	DWORD dwKeyType;
	DWORD cKeys;
	DWORD iKey;
	DWORD cchName;
	char* szFrameName;

	pframeCur = new SFrame();
	if (pframeCur == nullptr)
	{
		hr = E_OUTOFMEMORY;
		goto e_Exit;
	}
	pframeCur->bAnimationFrame = true;

	pframeParent->AddFrame(pframeCur);
	pde->AddAnimationFrame(pframeCur);

	// Enumerate child objects.
	// Child object can be data, data reference or binary.
	// Use QueryInterface() to find what type of object a child is.
	//while (SUCCEEDED(pxofobjCur->GetNextObject(&pxofChild)))

	SIZE_T children = 0;
	hr = pxofobjCur->GetChildren(&children);
	if (FAILED(hr)) goto e_Exit;

	for (SIZE_T child = 0; child < children; child++)
	{
		if (SUCCEEDED(pxofobjCur->GetChild(child, &pxofChild)))
		{
			if (pxofChild->IsReference()) //SUCCEEDED(hr))
			{
				// Query the child for it's FileDataReference
				//hr = pxofChild->QueryInterface(IID_IDirectXFileDataReference, (LPVOID *)&pxofobjChildRef);
				//hr = pxofobjChildRef->Resolve(&pxofobjChild);

				if (true) //SUCCEEDED(hr))
				{
					hr = pxofChild->GetType(&type);
					if (FAILED(hr)) goto e_Exit;

					if (TID_D3DRMFrame == type)
					{
						if (pframeCur->pframeToAnimate != nullptr)
						{
							hr = E_INVALIDARG;
							goto e_Exit;
						}

						hr = pxofChild->GetName(nullptr, &cchName);
						if (FAILED(hr))
							goto e_Exit;

						if (cchName == 0)
						{
							hr = E_INVALIDARG;
							goto e_Exit;
						}

						szFrameName = (char*)_alloca(cchName);
						if (szFrameName == nullptr)
						{
							hr = E_OUTOFMEMORY;
							goto e_Exit;
						}

						hr = pxofChild->GetName(szFrameName, &cchName);
						if (FAILED(hr))
							goto e_Exit;

						pframeCur->pframeToAnimate = pde->FindFrame(szFrameName);
						if (pframeCur->pframeToAnimate == nullptr)
						{
							hr = E_INVALIDARG;
							goto e_Exit;
						}
					}

					GXRELEASE(pxofobjChild);
				}

				//GXRELEASE(pxofobjChildRef);
			}
			else
			{
				// Query the child for it's FileData
				hr = pxofChild->QueryInterface(IID_ID3DXFileData, (LPVOID *)&pxofobjChild);

				if (SUCCEEDED(hr))
				{
					hr = pxofobjChild->GetType(&type);

					if (FAILED(hr)) goto e_Exit;

					if (TID_D3DRMFrame == type)
					{
						hr = LoadFrames(pxofobjChild, pde, options, fvf, pD3DDevice, pframeCur);
						if (FAILED(hr)) goto e_Exit;
					}
					else if (TID_D3DRMAnimationOptions == type)
					{
					}
					else if (TID_D3DRMAnimationKey == type)
					{
						hr = pxofobjChild->Lock(&dwSize, (LPCVOID*)&pData);

						if (FAILED(hr)) goto e_Exit;

						dwKeyType = ((DWORD*)pData)[0];
						cKeys = ((DWORD*)pData)[1];

						if (dwKeyType == 0)
						{
							if (pframeCur->m_pRotateKeys != nullptr)
							{
								hr = E_INVALIDARG;
								goto e_UnlockFirst;
							}

							pframeCur->m_pRotateKeys = new SRotateKey[cKeys];
							if (pframeCur->m_pRotateKeys == nullptr)
							{
								hr = E_OUTOFMEMORY;
								goto e_UnlockFirst;
							}

							pframeCur->m_cRotateKeys = cKeys;
							//NOTE x files are w x y z and QUATERNIONS are x y z w

							pFileRotateKey = (SRotateKeyXFile*)(pData + (sizeof(DWORD) * 2));
							for (iKey = 0; iKey < cKeys; iKey++)
							{
								pframeCur->m_pRotateKeys[iKey].dwTime = pFileRotateKey->dwTime;
								pframeCur->m_pRotateKeys[iKey].quatRotate.x = pFileRotateKey->x;
								pframeCur->m_pRotateKeys[iKey].quatRotate.y = pFileRotateKey->y;
								pframeCur->m_pRotateKeys[iKey].quatRotate.z = pFileRotateKey->z;
								pframeCur->m_pRotateKeys[iKey].quatRotate.w = pFileRotateKey->w;

								pFileRotateKey += 1;
							}
						}
						else if (dwKeyType == 1)
						{
							if (pframeCur->m_pScaleKeys != nullptr)
							{
								hr = E_INVALIDARG;
								goto e_UnlockFirst;
							}

							pframeCur->m_pScaleKeys = new SScaleKey[cKeys];
							if (pframeCur->m_pScaleKeys == nullptr)
							{
								hr = E_OUTOFMEMORY;
								goto e_UnlockFirst;
							}

							pframeCur->m_cScaleKeys = cKeys;

							pFileScaleKey = (SScaleKeyXFile*)(pData + (sizeof(DWORD) * 2));
							for (iKey = 0; iKey < cKeys; iKey++)
							{
								pframeCur->m_pScaleKeys[iKey].dwTime = pFileScaleKey->dwTime;
								pframeCur->m_pScaleKeys[iKey].vScale = pFileScaleKey->vScale;

								pFileScaleKey += 1;
							}
						}
						else if (dwKeyType == 2)
						{
							if (pframeCur->m_pPositionKeys != nullptr)
							{
								hr = E_INVALIDARG;
								goto e_UnlockFirst;
							}

							pframeCur->m_pPositionKeys = new SPositionKey[cKeys];
							if (pframeCur->m_pPositionKeys == nullptr)
							{
								hr = E_OUTOFMEMORY;
								goto e_UnlockFirst;
							}

							pframeCur->m_cPositionKeys = cKeys;

							pFilePosKey = (SPositionKeyXFile*)(pData + (sizeof(DWORD) * 2));
							for (iKey = 0; iKey < cKeys; iKey++)
							{
								pframeCur->m_pPositionKeys[iKey].dwTime = pFilePosKey->dwTime;
								pframeCur->m_pPositionKeys[iKey].vPos = pFilePosKey->vPos;

								pFilePosKey += 1;
							}
						}
						else if (dwKeyType == 4)
						{
							if (pframeCur->m_pMatrixKeys != nullptr)
							{
								hr = E_INVALIDARG;
								goto e_UnlockFirst;
							}
							// 2015-10-25 PanKJ Initialization of m_pMatrixKeys here
							pframeCur->m_pMatrixKeys = new SMatrixKey[cKeys];
							if (pframeCur->m_pMatrixKeys == nullptr)
							{
								hr = E_OUTOFMEMORY;
								goto e_UnlockFirst;
							}

							pframeCur->m_cMatrixKeys = cKeys;

							pFileMatrixKey = (SMatrixKeyXFile*)(pData + (sizeof(DWORD) * 2));
							for (iKey = 0; iKey < cKeys; iKey++)
							{
								pframeCur->m_pMatrixKeys[iKey].dwTime = pFileMatrixKey->dwTime;
								pframeCur->m_pMatrixKeys[iKey].mat = pFileMatrixKey->mat;
								pframeCur->m_mapMatrixKeys[pFileMatrixKey->dwTime] = iKey; // 2015-10-26 PanKJ experimental optimization
								pFileMatrixKey += 1;
							}
						}
						else
						{
							hr = E_INVALIDARG;
							goto e_UnlockFirst;
						}

					e_UnlockFirst:
						pxofobjChild->Unlock();
					}

					GXRELEASE(pxofobjChild);
				}
			}

			GXRELEASE(pxofChild);
		}
	}
e_Exit:
	GXRELEASE(pxofobjChild);
	GXRELEASE(pxofChild);
	//GXRELEASE(pxofobjChildRef);
	return hr;
}

HRESULT CSkinnedMesh::LoadAnimationSet(LPD3DXFILEDATA pxofobjCur, SDrawElement* pde, DWORD options, DWORD fvf, LPDIRECT3DDEVICE9 pD3DDevice, SFrame* pframeParent)
{
	//	FLOG( "CSkinnedMesh::LoadAnimationSet(LPDIRECTXFILEDATA pxofobjCur, SDrawElement *pde,DWORD options, DWORD fvf, LPDIRECT3DDEVICE8 pD3DDevice, SFrame *pframeParent)" );
	SFrame* pframeCur;
	GUID type;
	HRESULT hr = S_OK;
	LPD3DXFILEDATA pxofobjChild = nullptr;
	LPD3DXFILEDATA pxofChild = nullptr;
	//LPDIRECTXFILEOBJECT pxofChild = NULL;
	DWORD cchName;

	pframeCur = new SFrame();
	if (pframeCur == nullptr)
	{
		hr = E_OUTOFMEMORY;
		goto e_Exit;
	}
	pframeCur->bAnimationFrame = true;

	pframeParent->AddFrame(pframeCur);

	hr = pxofobjCur->GetName(nullptr, &cchName);
	if (FAILED(hr))
		goto e_Exit;

	if (cchName > 0)
	{
		pframeCur->szName = new char[cchName];
		if (pframeCur->szName == nullptr)
		{
			hr = E_OUTOFMEMORY;
			goto e_Exit;
		}

		hr = pxofobjCur->GetName(pframeCur->szName, &cchName);
		if (FAILED(hr))
			goto e_Exit;
	}
	// Enumerate child objects.
	// Child object can be data, data reference or binary.
	// Use QueryInterface() to find what type of object a child is.
	//while (SUCCEEDED(pxofobjCur->GetNextObject(&pxofChild)))

	SIZE_T children = 0;
	hr = pxofobjCur->GetChildren(&children);
	if (FAILED(hr)) goto e_Exit;

	for (SIZE_T child = 0; child < children; child++)
	{
		if (SUCCEEDED(pxofobjCur->GetChild(child, &pxofChild)))
		{
			if (!pxofChild->IsReference())
			{
				// Query the child for it's FileData
				hr = pxofChild->QueryInterface(IID_ID3DXFileData, (LPVOID *)&pxofobjChild);
				if (FAILED(hr)) goto e_Exit;

				hr = pxofobjChild->GetType(&type);
				if (FAILED(hr)) goto e_Exit;

				if (TID_D3DRMAnimation == type)
				{
					hr = LoadAnimation(pxofobjChild, pde, options, fvf, pD3DDevice, pframeCur);
					if (FAILED(hr)) goto e_Exit;
				}

				GXRELEASE(pxofobjChild);
			}

			GXRELEASE(pxofChild);
		}
	}
e_Exit:
	GXRELEASE(pxofobjChild);
	GXRELEASE(pxofChild);
	return hr;
}

// ReSharper disable once CppMemberFunctionMayBeConst
HRESULT CSkinnedMesh::UpdateFrames(SFrame* pframeCur, const D3DXMATRIX& matCur)
{
	D3DXMatrixMultiply(&pframeCur->matCombined, &pframeCur->matRot, &matCur);
	D3DXMatrixMultiply(&pframeCur->matCombined, &pframeCur->matCombined, &pframeCur->matTrans);

	for (auto pframeChild = pframeCur->pframeFirstChild; pframeChild; pframeChild = pframeChild->pframeSibling)
	{
		auto pmcMesh = pframeChild->pmcMesh;
		auto bResult = true;

		if (g_bDetailDrawFrame == TRUE && pmcMesh != nullptr && m_bProgressiveMesh == false)
		{
			D3DXMATRIX matTemp;
			D3DXMatrixIdentity(&matTemp);
			matTemp._41 = pmcMesh->vCenter.x;
			matTemp._42 = pmcMesh->vCenter.y;
			matTemp._43 = pmcMesh->vCenter.z;
			D3DXMatrixMultiply(&matTemp, &matTemp, &m_mWorld);

			D3DXVECTOR3 vMeshPos = {matTemp._41, matTemp._42, matTemp._43};

			if (g_pFrustum->CheckSphere(&vMeshPos, pmcMesh->fRadius)) // todo : review, this is may cause objects to freeze
			{
				vMeshPos -= g_pCamera->m_vCamCurrentPos;
				if (D3DXVec3Length(&vMeshPos) - pmcMesh->fRadius > g_pScene->m_fFogEndValue) bResult = false;
			}
		}

		if (bResult) UpdateFrames(pframeChild, pframeCur->matCombined);
	}

	return S_OK;
}

// ReSharper disable once CppMemberFunctionMayBeConst
HRESULT CSkinnedMesh::UpdateFrames(SFrame* pframeCur, D3DXMATRIX& matCur, D3DXVECTOR3 vPos, float fCheckDistance)
{
	D3DXMatrixMultiply(&pframeCur->matCombined, &pframeCur->matRot, &matCur);
	D3DXMatrixMultiply(&pframeCur->matCombined, &pframeCur->matCombined, &pframeCur->matTrans);

	for (auto pframeChild = pframeCur->pframeFirstChild; pframeChild; pframeChild = pframeChild->pframeSibling)
	{
		auto pmcMesh = pframeChild->pmcMesh;

		if (pmcMesh)
		{
			D3DXVECTOR3 vMeshCenter;

			D3DXVec3TransformCoord(&vMeshCenter, &pmcMesh->vCenter, &m_mWorld);

			vMeshCenter -= vPos;

			if (D3DXVec3Length(&vMeshCenter) - pmcMesh->fRadius < fCheckDistance)

				UpdateFrames(pframeChild, pframeCur->matCombined, vPos, fCheckDistance);
		}

		else UpdateFrames(pframeChild, pframeCur->matCombined, vPos, fCheckDistance);
	}

	return S_OK;
}

HRESULT CSkinnedMesh::DrawMeshContainer(SMeshContainer* pmcMesh, DWORD nType) // 2015-10-24 PanKJ todo: needs heavy optimization
{
	UINT ipattr;

	if (pmcMesh->m_pSkinMeshInfo != nullptr)
	{
		// D3DXMATRIX mat;
		LPD3DXBONECOMBINATION pBoneComb;
		DWORD AttribIdPrev;

		// 2005-01-05 by jschoi
		//		if (m_method != pmcMesh->m_Method)
		//		{
		//			GenerateMesh(pmcMesh);
		//		}

		if (m_method == D3DNONINDEXED)
		{
			AttribIdPrev = UNUSED32;
			pBoneComb = reinterpret_cast<LPD3DXBONECOMBINATION>(pmcMesh->m_pBoneCombinationBuf->GetBufferPointer());
			// Draw using default vtx processing of the device (typically HW)
			for (ipattr = 0; ipattr < pmcMesh->cpattr; ipattr++)
			{
				DWORD numBlend = 0;

				for (DWORD i = 0; i < pmcMesh->m_maxFaceInfl; ++i)

					if (pBoneComb[ipattr].BoneId[i] != UINT_MAX) numBlend = i;

				if (m_d3dCaps.MaxVertexBlendMatrices >= numBlend + 1)
				{
					for (DWORD i = 0; i < pmcMesh->m_maxFaceInfl; ++i)
					{
						auto matid = pBoneComb[ipattr].BoneId[i];
						if (matid != UINT_MAX)
						{
							m_pd3dDevice->SetTransform(D3DTS_WORLDMATRIX(i), pmcMesh->m_pBoneMatrix[matid]);
							m_pd3dDevice->MultiplyTransform(D3DTS_WORLDMATRIX(i), &pmcMesh->m_pBoneOffsetMat[matid]);
						}
					}

					m_pd3dDevice->SetRenderState(D3DRS_VERTEXBLEND, numBlend);

					if ((AttribIdPrev != pBoneComb[ipattr].AttribId) || (AttribIdPrev == UNUSED32))
					{
						m_pd3dDevice->SetMaterial(&(pmcMesh->rgMaterials[pBoneComb[ipattr].AttribId]));
						if (pmcMesh->pTextures[pBoneComb[ipattr].AttribId])
							m_pd3dDevice->SetTexture(0, pmcMesh->pTextures[pBoneComb[ipattr].AttribId]);
						else
							m_pd3dDevice->SetTexture(0, m_pTexture[m_bTextureNum - 1]);
						AttribIdPrev = pBoneComb[ipattr].AttribId;
					}

					pmcMesh->pMesh->DrawSubset(ipattr);
				}
			}

			// If necessary, draw parts that HW could not handle using SW
			if (pmcMesh->iAttrSplit < pmcMesh->cpattr)
			{
				AttribIdPrev = UNUSED32;
				// 2005-01-04 by jschoi
				m_pd3dDevice->SetSoftwareVertexProcessing(TRUE);

				for (ipattr = pmcMesh->iAttrSplit; ipattr < pmcMesh->cpattr; ipattr++)
				{
					DWORD numBlend = 0;
					for (DWORD i = 0; i < pmcMesh->m_maxFaceInfl; ++i)
					{
						if (pBoneComb[ipattr].BoneId[i] != UINT_MAX)
						{
							numBlend = i;
						}
					}

					if (m_d3dCaps.MaxVertexBlendMatrices < numBlend + 1)
					{
						for (DWORD i = 0; i < pmcMesh->m_maxFaceInfl; ++i)
						{
							DWORD matid = pBoneComb[ipattr].BoneId[i];
							if (matid != UINT_MAX)
							{
								m_pd3dDevice->SetTransform(D3DTS_WORLDMATRIX(i), pmcMesh->m_pBoneMatrix[matid]);
								m_pd3dDevice->MultiplyTransform(D3DTS_WORLDMATRIX(i), &pmcMesh->m_pBoneOffsetMat[matid]);
							}
						}

						m_pd3dDevice->SetRenderState(D3DRS_VERTEXBLEND, numBlend);

						if ((AttribIdPrev != pBoneComb[ipattr].AttribId) || (AttribIdPrev == UNUSED32))
						{
							m_pd3dDevice->SetMaterial(&(pmcMesh->rgMaterials[pBoneComb[ipattr].AttribId]));

							if (pmcMesh->pTextures[pBoneComb[ipattr].AttribId])
							{
								m_pd3dDevice->SetTexture(0, pmcMesh->pTextures[pBoneComb[ipattr].AttribId]);
							}
							else
							{
								m_pd3dDevice->SetTexture(0, m_pTexture[m_bTextureNum - 1]);
							}
							AttribIdPrev = pBoneComb[ipattr].AttribId;
						}

						// 2005-01-05 by jschoi 블렌딩 메시 현재 사용 안함
						// 						if(g_pD3dApp->m_pShuttleChild && g_pD3dApp->m_dwGameState == _GAME && m_bCheckBlend)
						//						{
						//							SetMeshRenderState(pmcMesh);
						//						}
						pmcMesh->pMesh->DrawSubset(ipattr);
					}
				}
				// 2005-01-04 by jschoi
				m_pd3dDevice->SetSoftwareVertexProcessing(FALSE);
			}
			m_pd3dDevice->SetRenderState(D3DRS_VERTEXBLEND, 0);
		}
		else if (m_method == D3DINDEXEDVS) // VertexShader를 사용할 경우
		{
			// Use COLOR instead of UBYTE4 since Geforce3 does not support it
			// vConst.w should be 3, but due to about hack, mul by 255 and add epsilon
			D3DXVECTOR4 vConst(1.0f, 0.0f, 0.0f, 765.01f);
			LPDIRECT3DVERTEXBUFFER9 pVB;
			LPDIRECT3DINDEXBUFFER9 pIB;

			if (pmcMesh->m_bUseSW)
			{
				// 2005-01-04 by jschoi
				m_pd3dDevice->SetSoftwareVertexProcessing(TRUE);
			}

			pmcMesh->pMesh->GetVertexBuffer(&pVB);
			pmcMesh->pMesh->GetIndexBuffer(&pIB);
			auto hr = m_pd3dDevice->SetStreamSource(0, pVB, 0, D3DXGetFVFVertexSize(pmcMesh->pMesh->GetFVF()));
			if (FAILED(hr)) return hr;
			hr = m_pd3dDevice->SetIndices(pIB);
			if (FAILED(hr)) return hr;
			pVB->Release();
			pIB->Release();
			hr = m_pd3dDevice->SetFVF(m_dwIndexedVertexShader[pmcMesh->m_maxFaceInfl - 1]);
			if (FAILED(hr)) return hr;

			pBoneComb = reinterpret_cast<LPD3DXBONECOMBINATION>(pmcMesh->m_pBoneCombinationBuf->GetBufferPointer());
			for (ipattr = 0; ipattr < pmcMesh->cpattr; ipattr++)
			{
				for (DWORD i = 0; i < pmcMesh->m_paletteSize; ++i)
				{
					auto matid = pBoneComb[ipattr].BoneId[i];
					if (matid != UINT_MAX)
					{
						D3DXMATRIXA16 mat;
						D3DXMatrixMultiply(&mat, &pmcMesh->m_pBoneOffsetMat[matid], pmcMesh->m_pBoneMatrix[matid]);
					}
				}

				// Sum of all ambient and emissive contribution
				D3DXCOLOR ambEmm;

				auto ambient = D3DXCOLOR(pmcMesh->rgMaterials[pBoneComb[ipattr].AttribId].Ambient);
				auto multiplier = D3DXCOLOR(.25, .25, .25, 1.0);

				D3DXColorModulate(&ambEmm, &ambient, &multiplier);

				ambEmm += D3DXCOLOR(pmcMesh->rgMaterials[pBoneComb[ipattr].AttribId].Emissive);
				vConst.y = pmcMesh->rgMaterials[pBoneComb[ipattr].AttribId].Power;

				if (pmcMesh->pTextures[pBoneComb[ipattr].AttribId])

					m_pd3dDevice->SetTexture(0, pmcMesh->pTextures[pBoneComb[ipattr].AttribId]);

				else m_pd3dDevice->SetTexture(0, m_pTexture[m_bTextureNum - 1]);

				// 2005-01-05 by jschoi - 블렌딩 메시 사용안함
				// 				if(g_pD3dApp->m_pShuttleChild && g_pD3dApp->m_dwGameState == _GAME && m_bCheckBlend)
				//				{
				//					SetMeshRenderState(pmcMesh);
				//				}

				m_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, pBoneComb[ipattr].VertexStart, pBoneComb[ipattr].VertexCount, pBoneComb[ipattr].FaceStart * 3, pBoneComb[ipattr].FaceCount);
			}

			// VertexProcessing 복원
			if (pmcMesh->m_bUseSW) m_pd3dDevice->SetSoftwareVertexProcessing(FALSE);
		}
		else if (m_method == D3DINDEXED)
		{
			// Vertex processing이 지원되지 않는 그래픽 카드라면 Software Vertex Processing 사용
			if (pmcMesh->m_bUseSW) m_pd3dDevice->SetSoftwareVertexProcessing(TRUE);

			// set the number of vertex blend indices to be blended
			if (pmcMesh->m_maxFaceInfl == 1) m_pd3dDevice->SetRenderState(D3DRS_VERTEXBLEND, D3DVBF_0WEIGHTS);

			else m_pd3dDevice->SetRenderState(D3DRS_VERTEXBLEND, pmcMesh->m_maxFaceInfl - 1);

			if (pmcMesh->m_maxFaceInfl) m_pd3dDevice->SetRenderState(D3DRS_INDEXEDVERTEXBLENDENABLE, TRUE);

			// for each attribute group in the mesh, calculate the set of matrices in the palette and then draw the mesh subset
			pBoneComb = reinterpret_cast<LPD3DXBONECOMBINATION>(pmcMesh->m_pBoneCombinationBuf->GetBufferPointer());
			for (ipattr = 0; ipattr < pmcMesh->cpattr; ipattr++)
			{
				for (DWORD i = 0; i < pmcMesh->m_paletteSize; ++i)
				{
					auto matid = pBoneComb[ipattr].BoneId[i];
					if (matid != UINT_MAX)
					{
						m_pd3dDevice->SetTransform(D3DTS_WORLDMATRIX(i), &pmcMesh->m_pBoneOffsetMat[matid]);
						m_pd3dDevice->MultiplyTransform(D3DTS_WORLDMATRIX(i), pmcMesh->m_pBoneMatrix[matid]);
					}
				}

				m_pd3dDevice->SetMaterial(&(pmcMesh->rgMaterials[pBoneComb[ipattr].AttribId]));

				if (pmcMesh->pTextures[pBoneComb[ipattr].AttribId])

					m_pd3dDevice->SetTexture(0, pmcMesh->pTextures[pBoneComb[ipattr].AttribId]);

				else m_pd3dDevice->SetTexture(0, m_pTexture[m_bTextureNum - 1]);

				pmcMesh->pMesh->DrawSubset(ipattr);
			}
			m_pd3dDevice->SetRenderState(D3DRS_INDEXEDVERTEXBLENDENABLE, FALSE);
			m_pd3dDevice->SetRenderState(D3DRS_VERTEXBLEND, 0);

			// 다시 원래대로 복원해준다. 반드시..
			if (pmcMesh->m_bUseSW) m_pd3dDevice->SetSoftwareVertexProcessing(FALSE);
		}
		else if (m_method == SOFTWARE)
		{
			D3DXMATRIX Identity;
			auto cBones = pmcMesh->m_pSkinMeshInfo->GetNumBones();
			PBYTE pbVerticesSrc;
			PBYTE pbVerticesDest;

			// set up bone transforms
			for (DWORD iBone = 0; iBone < cBones; ++iBone)

				D3DXMatrixMultiply(&m_pBoneMatrices[iBone], &pmcMesh->m_pBoneOffsetMat[iBone], pmcMesh->m_pBoneMatrix[iBone]);

			// set world transform
			D3DXMatrixIdentity(&Identity);
			m_pd3dDevice->SetTransform(D3DTS_WORLD, &Identity);

			// 2005-01-04 by jschoi - UpdateSkinnedMesh 
			if (pmcMesh->pMesh == nullptr) // 2005-01-05 by jschoi - 디바이스를 잃으면서 pMesh가 NULL이 된다.
			{
				auto hr = pmcMesh->m_pSkinMesh->CloneMeshFVF(D3DXMESH_MANAGED, pmcMesh->m_pSkinMesh->GetFVF(), m_pd3dDevice, &pmcMesh->pMesh);
				if (FAILED(hr)) return hr;
			}
			pmcMesh->m_pSkinMesh->LockVertexBuffer(D3DLOCK_READONLY, (LPVOID*)&pbVerticesSrc);
			pmcMesh->pMesh->LockVertexBuffer(0, (LPVOID*)&pbVerticesDest);
			pmcMesh->m_pSkinMeshInfo->UpdateSkinnedMesh(m_pBoneMatrices, nullptr, pbVerticesSrc, pbVerticesDest);
			pmcMesh->m_pSkinMesh->UnlockVertexBuffer();
			pmcMesh->pMesh->UnlockVertexBuffer();

			for (ipattr = 0; ipattr < pmcMesh->cpattr; ipattr++)
			{
				if (m_bProgressiveMesh) m_pd3dDevice->SetMaterial(&m_material);
				else m_pd3dDevice->SetMaterial(&(pmcMesh->rgMaterials[ipattr]));

				if (pmcMesh->pTextures[pmcMesh->m_pAttrTable[ipattr].AttribId])

					g_pD3dDev->SetTexture(0, pmcMesh->pTextures[pmcMesh->m_pAttrTable[ipattr].AttribId]);

				else g_pD3dDev->SetTexture(0, m_pTexture[m_bTextureNum - 1]);

				pmcMesh->pMesh->DrawSubset(ipattr);
			}
		}
		else return E_FAIL;
	}
	else
	{
		if (m_bProgressiveMesh) m_pd3dDevice->SetMaterial(&m_material);

		for (ipattr = 0; ipattr < pmcMesh->cpattr; ipattr++)
		{
			if (!m_bProgressiveMesh) m_pd3dDevice->SetMaterial(&(pmcMesh->rgMaterials[ipattr]));

			if (m_pOrderTexture) m_pd3dDevice->SetTexture(0, m_pOrderTexture);
			else
			{
				if (nType == _SHUTTLE || nType == _ENEMY)
				{
					if (m_unTexSelectColor >= m_bTotalTextureNum) m_unTexSelectColor = 0;
					if (m_unTexSelectColor <= 0) m_unTexSelectColor = 1;
					if (pmcMesh->pTextures[ipattr])
					{
						int nLoadIdx = m_unTexSelectColor - 1;
						if (FALSE == IsLoadTexture(nLoadIdx)) nLoadIdx = 0;
						m_pd3dDevice->SetTexture(0, m_pTexture[nLoadIdx]);
						m_unTexColor++;
					}
				}
				else
				{
					if (pmcMesh->pTextures[ipattr]) m_pd3dDevice->SetTexture(0, pmcMesh->pTextures[ipattr]);
					else
					{
						auto nTextureNum = m_bTextureNum - 1;
						if (nTextureNum >= 0) m_pd3dDevice->SetTexture(0, m_pTexture[m_bTextureNum - 1]);
					}
				}

				if (m_bMultiTexture)
				{
					auto nLoadIdx = 1;
					if (!IsLoadTexture(1)) nLoadIdx = 0;
					m_pd3dDevice->SetTexture(1, m_pTexture[nLoadIdx]);
				}
			}

			if (g_pD3dApp->m_pFxSystem->GetMetarSurface()) // always false with current config
			{
				D3DXMATRIX matView, matWorld, matProj;
				D3DXMatrixIdentity(&matView);
				D3DXMatrixIdentity(&matWorld);
				D3DXMatrixIdentity(&matProj);

				g_pD3dDev->GetTransform(D3DTS_PROJECTION, &matProj);
				g_pD3dDev->GetTransform(D3DTS_VIEW, &matView);
				g_pD3dDev->GetTransform(D3DTS_WORLD, &matWorld);

				g_pD3dApp->m_pFxSystem->SetMeshFrame(pmcMesh);
				g_pD3dApp->m_pFxSystem->DrawMetalFilterBegin(matWorld, matView, matProj, ipattr, nType);
			}
			else if (g_pD3dApp->m_pFxSystem->GetEnvSurface()) // always false with current config
			{
				D3DXMATRIX matView, matWorld, matProj;
				D3DXMatrixIdentity(&matView);
				D3DXMatrixIdentity(&matWorld);
				D3DXMatrixIdentity(&matProj);

				g_pD3dDev->GetTransform(D3DTS_PROJECTION, &matProj);
				g_pD3dDev->GetTransform(D3DTS_VIEW, &matView);
				g_pD3dDev->GetTransform(D3DTS_WORLD, &matWorld);

				g_pD3dApp->m_pFxSystem->SetMeshFrame(pmcMesh);
				g_pD3dApp->m_pFxSystem->DrawEvnFilterBegin(matWorld, matView, matProj, ipattr, nType);
			}
			else
			{
				pmcMesh->pMesh->DrawSubset(ipattr);
			}
		}
	}
	return S_OK;
}

void CSkinnedMesh::SetMeshRenderState(SMeshContainer* pmcMesh)//,BOOL bSkinMesh)
{
	FLOG("CSkinnedMesh::SetMeshRenderState(SMeshContainer *pmcMesh)");
	//BYTE bBlendType = 0;
	auto bHit = FALSE;
	auto fDistance = g_pD3dApp->m_pShuttleChild->m_fDistanceCamera;
	D3DXMATRIX matProj;
	D3DXVECTOR3 vPickRayDir, vPickRayOrig;//,v2,vSide;
	D3DXVECTOR3 v;
	D3DXMATRIX matView, m, matWorld; // , matTemp;
	g_pD3dDev->GetTransform(D3DTS_PROJECTION, &matProj);
	// Get the inverse view matrix
	g_pD3dDev->GetTransform(D3DTS_VIEW, &matView);
	g_pD3dDev->GetTransform(D3DTS_WORLD, &matWorld);
	m = matWorld * matView;
	D3DXMatrixInverse(&m, nullptr, &m);
	// 셔틀의 중앙과 블렌딩 체크
	// Compute the vector of the pick ray in screen space
	v.x = (((2.0f * g_pD3dApp->m_pShuttleChild->m_nObjScreenX) / g_pD3dApp->GetBackBufferDesc().Width) - 1) / matProj._11;
	v.y = -(((2.0f * g_pD3dApp->m_pShuttleChild->m_nObjScreenY) / g_pD3dApp->GetBackBufferDesc().Height) - 1) / matProj._22;
	v.z = 1.0f;
	// Transform the screen space pick ray into 3D space
	vPickRayDir.x = v.x * m._11 + v.y * m._21 + v.z * m._31;
	vPickRayDir.y = v.x * m._12 + v.y * m._22 + v.z * m._32;
	vPickRayDir.z = v.x * m._13 + v.y * m._23 + v.z * m._33;
	vPickRayOrig.x = m._41;
	vPickRayOrig.y = m._42;
	vPickRayOrig.z = m._43;
	D3DXIntersect(pmcMesh->pMesh, &(vPickRayOrig), &(vPickRayDir), &bHit, nullptr, nullptr, nullptr, &m_fCollDist, nullptr, nullptr);
	if (bHit)
	{
		if (m_fCollDist < fDistance)
		{
			m_bCheckBlend = FALSE;
			g_pShuttleChild->m_bCheckBlend = TRUE;
		}
		else
		{
			//			g_pShuttleChild->m_bCheckBlend = FALSE;
		}
	}
	else
	{
		//		g_pShuttleChild->m_bCheckBlend = FALSE;
	}
}


HRESULT CSkinnedMesh::DrawFrames(SFrame* pframeCur, UINT& cTriangles, DWORD nType)
{
	//	FLOG( "CSkinnedMesh::DrawFrames(SFrame *pframeCur, UINT &cTriangles)" );

	if (pframeCur->pmcMesh != nullptr)
	{
		auto hr = m_pd3dDevice->SetTransform(D3DTS_WORLD, &pframeCur->matCombined);
		if (FAILED(hr)) return hr;
	}

	D3DXVECTOR3 vMeshPos;
	D3DXMATRIX matTemp;

	for (auto pmcMesh = pframeCur->pmcMesh; pmcMesh != nullptr; pmcMesh = pmcMesh->pmcNext)
	{
		auto hr = DrawMeshContainer(pmcMesh, nType);
		if (FAILED(hr)) return hr;
		cTriangles += pmcMesh->pMesh->GetNumFaces();
	}

	for (auto pframeChild = pframeCur->pframeFirstChild; pframeChild != nullptr; pframeChild = pframeChild->pframeSibling)
	{
		auto pmcMesh = pframeChild->pmcMesh;

		auto bResult = true;

		if (g_bDetailDrawFrame == TRUE && pmcMesh != nullptr && m_bProgressiveMesh == false)
		{
			D3DXMatrixIdentity(&matTemp);
			matTemp._41 = pmcMesh->vCenter.x;
			matTemp._42 = pmcMesh->vCenter.y;
			matTemp._43 = pmcMesh->vCenter.z;

			D3DXMatrixMultiply(&matTemp, &matTemp, &m_mWorld);

			vMeshPos.x = matTemp._41;
			vMeshPos.y = matTemp._42;
			vMeshPos.z = matTemp._43;

			vMeshPos -= g_pCamera->m_vCamCurrentPos;

			if (D3DXVec3Length(&vMeshPos) - pmcMesh->fRadius > g_pScene->m_fFogEndValue) bResult = false;
		}

		if (bResult)
		{
			auto hr = DrawFrames(pframeChild, cTriangles, nType);
			if (FAILED(hr)) return hr;
		}
	}

	return S_OK;
}


HRESULT CSkinnedMesh::DeleteSelectedMesh()
{
	FLOG("CSkinnedMesh::DeleteSelectedMesh()");
	if (m_pdeSelected != nullptr)
	{
		auto pdeCur = m_pdeHead;
		SDrawElement* pdePrev = nullptr;
		while ((pdeCur != nullptr) && (pdeCur != m_pdeSelected))
		{
			pdePrev = pdeCur;
			pdeCur = pdeCur->pdeNext;
		}

		if (pdePrev == nullptr) m_pdeHead = m_pdeHead->pdeNext;
		else pdePrev->pdeNext = pdeCur->pdeNext;

		m_pdeSelected->pdeNext = nullptr;
		if (m_pdeHead == m_pdeSelected)
			m_pdeHead = nullptr;
		delete m_pdeSelected;
		m_pdeSelected = nullptr;
	}

	return S_OK;
}

// ReSharper disable once CppMemberFunctionMayBeConst
void CSkinnedMesh::ReleaseDeviceDependentMeshes(SFrame* pframe)
{
	//	FLOG( "CSkinnedMesh::ReleaseDeviceDependentMeshes(SFrame* pframe)" );
	if (pframe->pmcMesh != nullptr)
	{
		for (SMeshContainer* pmcCurr = pframe->pmcMesh; pmcCurr != nullptr; pmcCurr = pmcCurr->pmcNext)
		{
			if (pmcCurr->m_pSkinMesh != nullptr)
			{
				GXRELEASE(pmcCurr->pMesh);
				pmcCurr->m_Method = NONE;
			}
			/*			for(i=0;i<5;i++)
						{
						if(pmcCurr->pMeshes[i])
						GXRELEASE(pmcCurr->pMeshes[i]);
						}
						*/
		}
	}

	if (pframe->pframeFirstChild != nullptr)
		ReleaseDeviceDependentMeshes(pframe->pframeFirstChild);

	if (pframe->pframeSibling != nullptr)
		ReleaseDeviceDependentMeshes(pframe->pframeSibling);
}

// global func

HRESULT CalculateBoundingSphere(SDrawElement* pdeCur, D3DXVECTOR3 vObjScaleSphere) // 2012-05-21 by isshin 맵툴 개선 오브젝트 스케일 적용
{
	auto hr = S_OK;
	UINT cVertices = 0;
	float fRadiusSq = 0;
	D3DXMATRIXA16 matCur;
	D3DXMatrixIdentity(&matCur);

	D3DXVECTOR3 VecZero = {0, 0, 0};

	// 메시 하부 구조로 계속 들어가면서 최 상단의 vCenter 좌표에 상대적인 각각의 세부 메시의 중심 좌표를 SMeshContainer 에 기록한다.
	hr = CalculateSum(pdeCur->pframeRoot, &matCur, &VecZero, &cVertices, vObjScaleSphere); // 2012-05-21 by isshin 맵툴 개선 오브젝트 스케일 적용

	// 메시 하부 구조로 계속 들어가면서 반지름을 구해 각각의 SMeshContainer 세부 메시에 반지름을 해당 SMeshContainer 에 기록한다.
	hr = CalculateRadius(pdeCur->pframeRoot, &matCur, &VecZero, &fRadiusSq, vObjScaleSphere); // 2012-05-21 by isshin 맵툴 개선 오브젝트 스케일 적용
	if (FAILED(hr))
		goto e_Exit;

	// 최 상단(전체 메시)의 반지름을 SDrawElement 에 기록한다. 최 상단의 상대 vCenter는 항상 (0,0,0)이다.

	D3DXMatrixIdentity(&matCur);

	if (FAILED(hr))
		goto e_Exit;

	pdeCur->fRadius = float(sqrt(double(fRadiusSq)));
	pdeCur->fRadius *= max(max(vObjScaleSphere.x, vObjScaleSphere.y), vObjScaleSphere.z); // 2012-09-20 by jhahn 맵툴 오브젝트 확대축소 충돌범위 버그 수정
	pdeCur->vCenter = D3DXVECTOR3(0, 0, 0);

e_Exit:
	return hr;
}

HRESULT CalculateSum(SFrame* pframe, D3DXMATRIX* pmatCur, D3DXVECTOR3* pvCenter, // 2012-05-21 by isshin 맵툴 개선 오브젝트 스케일 적용
                     UINT* pcVertices, D3DXVECTOR3 vObjScaleSphereSum)
{
	auto hr = S_OK;
	PBYTE pbPoints = nullptr;
	UINT cVerticesLocal = 0;
	PBYTE pbCur;
	D3DXVECTOR3* pvCur;
	D3DXVECTOR3 vTransformedCur;
	UINT iPoint;
	SMeshContainer* pmcCur;
	SFrame* pframeCur;
	UINT cVertices;
	D3DXMATRIXA16 matLocal;

	//	D3DXMATRIXA16 matObjScale;
	//	D3DXMatrixIdentity(&matObjScale);
	//	D3DXMatrixScaling(&matObjScale, vObjScaleSphereSum.x, vObjScaleSphereSum.y, vObjScaleSphereSum.z);	    
	//	D3DXMatrixMultiply(&matLocal, &matObjScale, pmatCur);						// 2012-05-21 by isshin 맵툴 개선 오브젝트 스케일 적용
	D3DXMatrixMultiply(&matLocal, &pframe->matRot, pmatCur);

	pmcCur = pframe->pmcMesh;

	while (pmcCur != nullptr)
	{
		D3DXVECTOR3 vLocalCenter(0, 0, 0);
		UINT nVerticesCount = 0;
		DWORD fvfsize = D3DXGetFVFVertexSize(pmcCur->pMesh->GetFVF());

		cVertices = pmcCur->pMesh->GetNumVertices();

		hr = pmcCur->pMesh->LockVertexBuffer(0, (LPVOID*)&pbPoints);
		if (FAILED(hr))
			goto e_Exit;

		for (iPoint = 0 , pbCur = pbPoints; iPoint < cVertices; iPoint++ , pbCur += fvfsize)
		{
			pvCur = (D3DXVECTOR3*)pbCur;

			if ((pvCur->x != 0.0) || (pvCur->y != 0.0) || (pvCur->z != 0.0))
			{
				cVerticesLocal++;

				D3DXVec3TransformCoord(&vTransformedCur, pvCur, &matLocal);

				pvCenter->x += vTransformedCur.x;
				pvCenter->y += vTransformedCur.y;
				pvCenter->z += vTransformedCur.z;

				nVerticesCount++;
				vLocalCenter += vTransformedCur;
			}
		}


		pmcCur->pMesh->UnlockVertexBuffer();
		pbPoints = nullptr;

		pmcCur->vCenter = vLocalCenter / (float)nVerticesCount;

		pmcCur = pmcCur->pmcNext;
	}

	*pcVertices += cVerticesLocal;

	pframeCur = pframe->pframeFirstChild;
	while (pframeCur != nullptr)
	{
		hr = CalculateSum(pframeCur, &matLocal, pvCenter, pcVertices);
		if (FAILED(hr))
			goto e_Exit;

		pframeCur = pframeCur->pframeSibling;
	}

e_Exit:
	if (pbPoints != nullptr)
	{
		pmcCur->pMesh->UnlockVertexBuffer();
	}

	return hr;
}


HRESULT CalculateRadius(SFrame* pframe, D3DXMATRIX* pmatCur, D3DXVECTOR3* pvCenter,
                        float* pfRadiusSq, D3DXVECTOR3 vObjScaleSphereRadius) // 2012-05-21 by isshin 맵툴 개선 오브젝트 스케일 적용
{
	auto hr = S_OK;
	PBYTE pbPoints = nullptr;
	PBYTE pbCur;
	D3DXVECTOR3* pvCur;
	D3DXVECTOR3 vDist;
	UINT iPoint;
	UINT cVertices;
	SMeshContainer* pmcCur;
	SFrame* pframeCur;
	float fRadiusLocalSq;
	float fDistSq;
	D3DXMATRIXA16 matLocal;
	//D3DXVECTOR3 minxyz = { 0, 0, 0 }, maxxyz = { 0, 0, 0 };
	// 2012-05-21 by isshin 맵툴 개선 오브젝트 스케일 적용
	//	D3DXMATRIXA16 matObjScale;
	//	D3DXMatrixIdentity(&matObjScale);												
	//	D3DXMatrixScaling(&matObjScale, vObjScaleSphereRadius.x, vObjScaleSphereRadius.y, vObjScaleSphereRadius.z);
	//	D3DXMatrixMultiply(pmatCur,&matObjScale,pmatCur);
	// end 2012-05-21 by isshin 맵툴 개선 오브젝트 스케일 적용

	D3DXMatrixMultiply(&matLocal, &pframe->matRot, pmatCur);

	pmcCur = pframe->pmcMesh;
	fRadiusLocalSq = *pfRadiusSq;


	while (pmcCur != nullptr)
	{
		float fRealLocalRadius = 0;
		float fLocalDistSq = 0;
		D3DXVECTOR3 vLocalDist(0, 0, 0);
		DWORD fvfsize = D3DXGetFVFVertexSize(pmcCur->pMesh->GetFVF());

		cVertices = pmcCur->pMesh->GetNumVertices();

		hr = pmcCur->pMesh->LockVertexBuffer(0, (LPVOID*)&pbPoints);
		if (FAILED(hr))
			goto e_Exit;

		pmcCur->m_vecMinXYZ = { 0, 0 ,0 };
		pmcCur->m_vecMaxXYZ = { 0, 0, 0 };

		D3DXComputeBoundingBox(reinterpret_cast<D3DXVECTOR3*>(pbPoints),
			cVertices, fvfsize, &pmcCur->m_vecMinXYZ, &pmcCur->m_vecMaxXYZ);

		for (iPoint = 0 , pbCur = pbPoints; iPoint < cVertices; iPoint++ , pbCur += fvfsize)
		{
			pvCur = (D3DXVECTOR3*)pbCur;

			if ((pvCur->x == 0.0) && (pvCur->y == 0.0) && (pvCur->z == 0.0))
				continue;

			D3DXVec3TransformCoord(&vDist, pvCur, &matLocal);

			vDist -= *pvCenter;
			fDistSq = D3DXVec3LengthSq(&vDist);

			fRadiusLocalSq = max(fDistSq, fRadiusLocalSq);

			D3DXVec3TransformCoord(&vLocalDist, pvCur, &matLocal);
			vLocalDist -= pmcCur->vCenter;
			fLocalDistSq = D3DXVec3LengthSq(&vLocalDist);

			fRealLocalRadius = max(fLocalDistSq, fRealLocalRadius);
		}

		pmcCur->pMesh->UnlockVertexBuffer();
		pbPoints = nullptr;

		pmcCur->fRadius = (float)sqrt((double)fRealLocalRadius); // 세부 메시 반지름 세팅


		pmcCur = pmcCur->pmcNext;
	}

	*pfRadiusSq = fRadiusLocalSq;

	pframeCur = pframe->pframeFirstChild;

	while (pframeCur != nullptr)
	{
		hr = CalculateRadius(pframeCur, &matLocal, pvCenter, pfRadiusSq);
		if (FAILED(hr))
			goto e_Exit;

		pframeCur = pframeCur->pframeSibling;
	}

e_Exit:
	if (pbPoints != nullptr) pmcCur->pMesh->UnlockVertexBuffer();

	return hr;
}

void CSkinnedMesh::ChangeFilePath(char* strFilePath)
{
	char strTemp[9];

	int nStrSize = strlen(strFilePath);
	if (nStrSize > 8)
	{
		nStrSize -= 5;
		memset(strTemp, 0x30, 8);
		strTemp[8] = '\0';
		for (auto i = 7; i >= 0; i--)
		{
			strTemp[i] = strFilePath[nStrSize];
			nStrSize--;
			if (nStrSize == 0 ||
				strFilePath[nStrSize] == '\\')
			{
				break;
			}
		}
		DBGOUT("strFilePath : %s\n", strFilePath);
		DBGOUT("strTemp : %s\n\n", strTemp);
		strcpy(strFilePath, strTemp);
	}
}


COLLISION_RESULT CSkinnedMesh::Pick(SMeshContainer* pmcMesh, float fx, float fy)
{
	COLLISION_RESULT collResult;

	//    if (pmcMesh->m_pSkinMesh)
	//    {
	//        if (m_method == SOFTWARE)
	//        {
	//            D3DXMATRIX  Identity;
	//            DWORD       cBones  = pmcMesh->m_pSkinMeshInfo->GetNumBones();
	//            // set up bone transforms
	//            for (DWORD iBone = 0; iBone < cBones; ++iBone)
	//            {
	//                D3DXMatrixMultiply
	//                (
	//                    &m_pBoneMatrices[iBone],                 // output
	//                    &pmcMesh->m_pBoneOffsetMat[iBone], 
	//                    pmcMesh->m_pBoneMatrix[iBone]
	//                );
	//            }
	//            // set world transform
	//            D3DXMatrixIdentity(&Identity);
	//            m_pd3dDevice->SetTransform(D3DTS_WORLD, &Identity);
	//            // generate skinned mesh
	//			if(!pmcMesh->pMesh)
	//			{
	////				collResult.fDist = DEFAULT_COLLISION_DISTANCE;
	//				return collResult;
	//			}
	////            pmcMesh->m_pSkinMesh->UpdateSkinnedMesh(m_pBoneMatrices, NULL, pmcMesh->pMesh);
	//		}
	//	}


	BOOL bHit;
	// BOOL bIntersections;
	DWORD dwFace;
	FLOAT fBary1, fBary2, fDist;
	D3DXVECTOR3 vPickRayDir, vPickRayOrig;
	D3DXVECTOR3 v;

	D3DXMATRIX matProj;
	m_pd3dDevice->GetTransform(D3DTS_PROJECTION, &matProj);

	v.x = (((2.0f * fx) / g_pD3dApp->GetBackBufferDesc().Width) - 1) / matProj._11;
	v.y = -(((2.0f * fy) / g_pD3dApp->GetBackBufferDesc().Height) - 1) / matProj._22;
	v.z = 1.0f;

	// Get the inverse view matrix
	D3DXMATRIX matView, matWorld, m;
	m_pd3dDevice->GetTransform(D3DTS_VIEW, &matView);
	//    D3DXMatrixInverse( &m, NULL, &matView );

	g_pD3dDev->GetTransform(D3DTS_WORLD, &matWorld);
	m = matWorld * matView;
	D3DXMatrixInverse(&m, nullptr, &m);

	// Transform the screen space pick ray into 3D space
	vPickRayDir.x = v.x * m._11 + v.y * m._21 + v.z * m._31;
	vPickRayDir.y = v.x * m._12 + v.y * m._22 + v.z * m._32;
	vPickRayDir.z = v.x * m._13 + v.y * m._23 + v.z * m._33;
	vPickRayOrig.x = m._41;
	vPickRayOrig.y = m._42;
	vPickRayOrig.z = m._43;

	D3DXIntersect(pmcMesh->pMesh, &vPickRayOrig, &vPickRayDir, &bHit, &dwFace, &fBary1, &fBary2, &fDist, nullptr, nullptr);

	if (bHit)
	{
		// bIntersections = TRUE;
		m_Intersection.dwFace = dwFace;
		m_Intersection.fBary1 = fBary1;
		m_Intersection.fBary2 = fBary2;
		m_Intersection.fDist = fDist;

		collResult.fDist = fDist;
		collResult.vPicking = vPickRayOrig + vPickRayDir * fDist; // Picking한 위치 값
		D3DXVec3TransformCoord(&collResult.vPicking, &collResult.vPicking, &matWorld);
		//		DBGOUT("2D %f, %f ->", fx, fy);
		//		DBGOUT("Pick %f, %f, %f\n", collResult.vPicking.x, collResult.vPicking.y, collResult.vPicking.z);

		// 2005-07-26 by ispark 
		// Normal 값이 필요하므로 
		WORD* pIndices;
		D3DVERTEX* pVertices;

		D3DVERTEX vThisTri[3];
		WORD* iThisTri;

		pmcMesh->pMesh->LockVertexBuffer(D3DLOCK_READONLY, (LPVOID*)&pVertices);
		pmcMesh->pMesh->LockIndexBuffer(D3DLOCK_READONLY, (LPVOID*)&pIndices);

		iThisTri = &pIndices[3 * m_Intersection.dwFace];
		// get vertices hit
		vThisTri[0] = pVertices[iThisTri[0]];
		vThisTri[1] = pVertices[iThisTri[1]];
		vThisTri[2] = pVertices[iThisTri[2]];

		pmcMesh->pMesh->UnlockVertexBuffer();
		pmcMesh->pMesh->UnlockIndexBuffer();

		D3DXVec3TransformCoord(&vThisTri[0].p, &vThisTri[0].p, &matWorld);
		D3DXVec3TransformCoord(&vThisTri[1].p, &vThisTri[1].p, &matWorld);
		D3DXVec3TransformCoord(&vThisTri[2].p, &vThisTri[2].p, &matWorld);
		//		D3DXVec3TransformNormal(&vThisTri[0].n, &vThisTri[0].n, &matWorld);
		//		D3DXVec3TransformNormal(&vThisTri[1].n, &vThisTri[1].n, &matWorld);
		//		D3DXVec3TransformNormal(&vThisTri[2].n, &vThisTri[2].n, &matWorld);

		// 법선을 구하자.

		D3DXVECTOR3 vTempNormal, vNormalVector;
		D3DXVECTOR3 vCross1, vCross2;
		vCross1 = vThisTri[0].p - vThisTri[1].p;
		vCross2 = vThisTri[1].p - vThisTri[2].p;
		D3DXVec3Cross(&vTempNormal, &vCross1, &vCross2);
		D3DXVec3Normalize(&vNormalVector, &vTempNormal);

		//		vNormalVector = vThisTri[0].n + vThisTri[1].n + vThisTri[2].n;
		//		D3DXVec3Normalize(&vNormalVector,&vNormalVector);

		collResult.vNormalVector = vNormalVector;
	}
	else
	{
		// bIntersections = FALSE;
	}

	return collResult;
}


COLLISION_RESULT CSkinnedMesh::CheckCollision(float fx, float fy, BOOL bUpdateFrame)
{
	//	FLOG( "CSkinnedMesh::CheckCollision(D3DXMATRIX mat)" );
	COLLISION_RESULT collResult, checkcollResult;
	SDrawElement* pdeCur;
	pdeCur = m_pdeHead;
	while (pdeCur != nullptr)
	{
		if (bUpdateFrame)
		{
			UpdateFrames(pdeCur->pframeRoot, m_mWorld);
		}
		checkcollResult = CheckCollDist(pdeCur->pframeRoot, fx, fy);
		if (collResult.fDist > checkcollResult.fDist)
		{
			collResult = checkcollResult;
		}
		pdeCur = pdeCur->pdeNext;
	}
	return collResult;
}

COLLISION_RESULT CSkinnedMesh::CheckCollDist(SFrame* pframeCur, float fx, float fy)
{
	//	FLOG( "CSkinnedMesh::CheckCollDist(SFrame *pframeCur,D3DXMATRIX mat)" );
	COLLISION_RESULT collResult, checkcollResult;
	SMeshContainer* pmcMesh;
	SFrame* pframeChild;

	if (pframeCur->pmcMesh != nullptr)
	{
		m_pd3dDevice->SetTransform(D3DTS_WORLD, &pframeCur->matCombined);
	}
	pmcMesh = pframeCur->pmcMesh;

	D3DXVECTOR3 vMeshCenter;

	while (pmcMesh != nullptr && pmcMesh->pMesh != nullptr)
	{
		checkcollResult = Pick(pmcMesh, fx, fy);
		if (collResult.fDist > checkcollResult.fDist)
			collResult = checkcollResult;

		pmcMesh = pmcMesh->pmcNext;
	}
	pframeChild = pframeCur->pframeFirstChild;

	while (pframeChild != nullptr)
	{
		// 2005-01-05 by jschoi
		pmcMesh = pframeChild->pmcMesh;
		if (pmcMesh)
		{
			D3DXVec3TransformCoord(&vMeshCenter, &pmcMesh->vCenter, &m_mWorld);
			//			if(D3DXVec3Length(&(vMeshCenter - vPos)) - pmcMesh->fRadius < fCheckDistance)
			//			{
			checkcollResult = CheckCollDist(pframeChild, fx, fy);
			if (collResult.fDist > checkcollResult.fDist)
				collResult = checkcollResult;
			// 			}
		}
		else
		{
			checkcollResult = CheckCollDist(pframeChild, fx, fy);
			if (collResult.fDist > checkcollResult.fDist)
				collResult = checkcollResult;
		}

		pframeChild = pframeChild->pframeSibling;
	}
	return collResult;
}

HRESULT CSkinnedMesh::SetResourceFile(char* szFileName)
{
	FLOG("CINFSelect::SetResourceFile(char* szFileName)");
	util::del(m_pGameDataToDecal);
	m_pGameDataToDecal = new CGameData;
	if (!m_pGameDataToDecal->SetFile(szFileName, FALSE, nullptr, 0))
	{
		util::del(m_pGameDataToDecal);
		return E_FAIL;
	}

	return S_OK;
}

// ReSharper disable once CppMemberFunctionMayBeConst
DataHeader* CSkinnedMesh::FindResource(char* szRcName)
{
	FLOG("CINFSelect::FindResource(char* szRcName)");

	DataHeader* pHeader = nullptr;

	if (m_pGameDataToDecal) pHeader = m_pGameDataToDecal->Find(szRcName);

	return pHeader;
}

// 2007-08-29 by bhsohn 최소 프레임시 기본 아머만 로딩하게끔 변경
// 그림자 쉐도우 텍스처
// ReSharper disable once CppMemberFunctionMayBeConst
LPDIRECT3DTEXTURE9 CSkinnedMesh::GetShadowTex()
{
	if (m_bTotalTextureNum > 0) return m_pTexture[m_bTotalTextureNum - 1];

	return nullptr;
}

BOOL CSkinnedMesh::IsLowQuality()
{
	return g_pSOption->sLowQuality;
}

BOOL CSkinnedMesh::IsLoadTexture(int nTexture) const
{
	if (m_pLoadMem == nullptr) return FALSE;

	return m_pLoadMem[nTexture];
}

// 2007-11-08 by bhsohn 인벤 이펙트 관련 처리
void CSkinnedMesh::UpdateMeshObjName(char* pDstPath)
{
	if (m_pGameData == nullptr) return;

	char strObjPath[MAX_PATH];
	char buf[16];

	memset(strObjPath, 0x00, MAX_PATH);
	memset(buf, 0x00, 16);

	g_pD3dApp->LoadPath(strObjPath, IDS_DIRECTORY_EFFECT, buf);

	auto pMeshFileName = m_pGameData->GetZipFilePath();
	auto pFind = strstr(pMeshFileName, strObjPath);

	if (pFind == nullptr || strlen(pFind) < strlen(strObjPath)) return;

	auto pObjFile = pFind + strlen(strObjPath);

	strncpy(m_szMeshObjFileName, pObjFile, strlen(pObjFile) + 1);
}

char* CSkinnedMesh::GetMeshObjFileName()
{
	return m_szMeshObjFileName;
}

#include "fstream"

// 2009. 11. 23 by jskim 리소스 로딩 구조 변경
HRESULT CSkinnedMesh::LoadMeshHierarchyFromMemLoadStart(structLoadingGameData* GameData, int nMeshIndex)
{
	//static ofstream fdg { "debug123.txt" };

	//fdg << GameData->MeshType << '\t' << GameData->Step << '\t' << GameData->MeshIndex << endl;

	switch (GameData->Step)
	{
	case _RESOURCE_LOADING_START:
		if (GetIsLoadingStartTime()) SetIsLoadingStartTime();
		GameData->Step++;
		if (TimeOut()) return E_FAIL;

	case _RESOURCE_LOADING_STEP1:
		if (GetIsLoadingStartTime()) SetIsLoadingStartTime();
		LoadMeshHierarchyFromMemLoadStep1(GameData, nMeshIndex);
		GameData->Step++;
		if (TimeOut()) return E_FAIL;

	case _RESOURCE_LOADING_STEP2:
		if (GetIsLoadingStartTime()) SetIsLoadingStartTime();
		LoadMeshHierarchyFromMemLoadStep2();
		GameData->Step++;
		if (TimeOut()) return E_FAIL;

	case _RESOURCE_LOADING_STEP3:
		if (GetIsLoadingStartTime()) SetIsLoadingStartTime();
		LoadMeshHierarchyFromMemLoadStep3();
		GameData->Step++;
		if (TimeOut()) return E_FAIL;

	case _RESOURCE_LOADING_STEP4:
		if (GetIsLoadingStartTime()) SetIsLoadingStartTime();
		if (FAILED(LoadMeshHierarchyFromMemLoadStep4(GameData))) return E_FAIL;
		GameData->Step++;

	case _RESOURCE_LOADING_STEP5:
		if (GetIsLoadingStartTime()) SetIsLoadingStartTime();
		if (FAILED(LoadMeshHierarchyFromMemLoadStep5())) return E_FAIL;
		GameData->Step++;

	case _RESOURCE_LOADING_STEP6:
		if (GetIsLoadingStartTime()) SetIsLoadingStartTime();
		LoadMeshHierarchyFromMemLoadStep6();
		GameData->Step = _RESOURCE_LOADING_END;
		break;
	}
	return S_OK;
}

HRESULT CSkinnedMesh::LoadMeshHierarchyFromMemLoadStep1(structLoadingGameData* pGameData, int nMeshIndex)
{
	FLOG("CSkinnedMesh::LoadMeshHierarchyFromMemStep1");
	char Stepbuf[32];
	memset(Stepbuf, 0x00, 32);
	pdeMesh = new SDrawElement();
	//FileLoadMemory = new DXFILELOADMEMORY;
	HRESULT hr;

	delete pdeMesh->pframeRoot;
	pdeMesh->pframeAnimHead = nullptr;
	pdeMesh->pframeRoot = new SFrame();

	// object file loading
	// char* pPointer = nullptr;

	m_pGameData = pGameData->pGameData;

	// 2007-11-08 by bhsohn 인벤 이펙트 관련 처리
	UpdateMeshObjName(m_szMeshObjFileName);

	int totalNum = m_pGameData->GetTotalNumber();
	m_pDataHeader = new DataHeader[totalNum];

	// 2007-08-29 by bhsohn 최소 프레임시 기본 아머만 로딩하게끔 변경			
	{
		util::del_array(m_pLoadMem);
		m_pLoadMem = new BOOL[totalNum];
		int nCnt = 0;
		for (nCnt = 0; nCnt < totalNum; nCnt++)
		{
			m_pLoadMem[nCnt] = TRUE;
		}
		// 2007-10-18 by bhsohn 내 아머는 무조건 다 로드
		// 내 아머는 무조건 다 로드
		BOOL bMyArmor = FALSE;
		if (IsArmorMesh(nMeshIndex))
		{
			if (_SELECT == g_pD3dApp->GetGameState())
			{
				bMyArmor = TRUE;
			}
			else if (NULL != g_pShuttleChild && nMeshIndex == g_pShuttleChild->GetUnitNum())
			{
				bMyArmor = TRUE;
			}
		}
		// end 2007-10-18 by bhsohn 내 아머는 무조건 다 로드

		// 2009-03-09 by bhsohn 일본은 기본 아머만 로드
		BOOL bDefaultArmor = (IsArmorMesh(nMeshIndex) && IsLowQuality() && (FALSE == bMyArmor));
		if (IsArmorMesh(nMeshIndex))
			bDefaultArmor = TRUE;

		// end 2009. 07. 27 by ckPark 예당도 기본 아머 텍스쳐만 로딩하도록 변경

		// 2009. 07. 07 by ckPark 로봇기어 요청사항(롤링, 선택화면, 무기, A기어포대)
		// 로봇아머 텍스쳐 2장 사용
		if (IsArmorMesh(nMeshIndex))
		{
			char szArmorNum[32];
			itoa(nMeshIndex, szArmorNum, 10);
			szArmorNum[0] = '0';
			szArmorNum[1] = '8';

			int nArmorItemNum = atoi(szArmorNum);

			ITEM* pItem = g_pDatabase->GetServerItemInfo(nArmorItemNum);
			if (pItem && COMPARE_BIT_FLAG(pItem->ItemAttribute, ITEM_ATTR_ROBOT_ARMOR))
				bDefaultArmor = FALSE;
		}
		// end 2009. 07. 07 by ckPark 로봇기어 요청사항(롤링, 선택화면, 무기, A기어포대)


		if (bDefaultArmor)
		{
			for (nCnt = 1; nCnt < totalNum - 2; nCnt++)
			{
				// 첫번? 아머만 뺴�?모두 Disable
				m_pLoadMem[nCnt] = FALSE;
			}
		}
		// end 2009-03-09 by bhsohn 일본은 기본 아머만 로드
	}
	// end 2007-08-29 by bhsohn 최소 프레임시 기본 아머만 로딩하게끔 변경
	pHeader = m_pGameData->GetStartPosition();
	auto i = 0;
	while (pHeader)
	{
		m_pDataHeader[i] = *pHeader;
		m_pDataHeader[i++].m_pData = pHeader->m_pData;
		pHeader = m_pGameData->GetNext();
	}

	if (pdeMesh->pframeRoot == nullptr)
	{
		hr = E_OUTOFMEMORY;
		ReleaseData(hr);
		return E_FAIL;
	}
	FileLoadMemory.lpMemory = m_pDataHeader[0].m_pData;
	FileLoadMemory.dSize = m_pDataHeader[0].m_DataSize;
	return S_OK;
}

HRESULT CSkinnedMesh::LoadMeshHierarchyFromMemLoadStep2()
{
	FLOG("CSkinnedMesh::LoadMeshHierarchyFromMemStep2");

	//dwOptions = 0;
	dwOptions = D3DXMESH_MANAGED;

	HRESULT hr;
	hr = D3DXFileCreate(&pxofapi);
	if (FAILED(hr))
	{
		ReleaseData(hr);
		return E_FAIL;
	}

	// Register templates for d3drm.
	hr = pxofapi->RegisterTemplates((LPVOID)D3DRM_XTEMPLATES, D3DRM_XTEMPLATE_BYTES);
	if (FAILED(hr))
	{
		ReleaseData(hr);
		return E_FAIL;
	}

	hr = pxofapi->RegisterTemplates((LPVOID)szTemplates, strlen(szTemplates));
	if (FAILED(hr))
	{
		ReleaseData(hr);
		return E_FAIL;
	}
	return S_OK;
}

// 
HRESULT CSkinnedMesh::LoadMeshHierarchyFromMemLoadStep3()
{
	FLOG("CSkinnedMesh::LoadMeshHierarchyFromMemStep3");

	HRESULT hr;
	hr = pxofapi->CreateEnumObject((LPCVOID)&FileLoadMemory, D3DXF_FILEFORMAT_BINARY | D3DXF_FILELOAD_FROMMEMORY, &pxofenum);
	if (FAILED(hr))
	{
		ReleaseData(hr);
		return E_FAIL;
	}
	return S_OK;
}

HRESULT CSkinnedMesh::LoadMeshHierarchyFromMemLoadStep4(structLoadingGameData* pGameData)
{
	FLOG("CSkinnedMesh::LoadMeshHierarchyFromMemStep4");

	auto totalNum = pGameData->pGameData->GetTotalNumber();

	if (pGameData->Text_Cnt == 0) pGameData->Text_Cnt = 1;

	auto cnt = pGameData->Text_Cnt;

	if (totalNum > 1)
	{
		if (m_pTexture == nullptr) m_pTexture = new LPDIRECT3DTEXTURE9[pGameData->pGameData->GetTotalNumber() - 1];

		m_bTotalTextureNum = totalNum - 1;
		for (auto i = cnt; i < totalNum; i++)
		{
			// 2007-08-29 by bhsohn
			// 최소 프레임시 기본 아머만 로딩하게끔 변경

			auto nLoadIdx = i - 1;

			if (!IsLoadTexture(nLoadIdx)) continue;

			// end 2007-08-29 by bhsohn		

			if (FAILED(D3DXCreateTextureFromFileInMemory(m_pd3dDevice, (LPCVOID)m_pDataHeader[i].m_pData,
				m_pDataHeader[i].m_DataSize, &(m_pTexture[i - 1]))))
			{
				if (g_pD3dApp && g_pD3dApp->m_pChat)
				{
					g_pD3dApp->m_pChat->CreateChatChild(STRERR_C_RESOURCE_0012, COLOR_ERROR);// "리소스 파일 로딩에 문제가 있어서 오브젝트 생성이 중단됨."
					pGameData->Step = _RESOURCE_LOADING_END;
				}
				DBGOUT("\n ERROR : Loading Resource File .\n\n");
				return E_FAIL;
			}
			if (TimeOut())
			{
				pGameData->Text_Cnt = ++cnt;
				return E_FAIL;
			}
		}
	}
	else
	{
		m_pTexture = new LPDIRECT3DTEXTURE9[1];
		m_pTexture[0] = nullptr;
	}
	return S_OK;
}

HRESULT CSkinnedMesh::LoadMeshHierarchyFromMemLoadStep5()
{
	FLOG("CSkinnedMesh::LoadMeshHierarchyFromMemStep5");
	HRESULT hr;
	//while (SUCCEEDED(pxofenum->GetNextDataObject(&pxofobjCur)))

	SIZE_T children = 0;
	if (pxofenum == nullptr)
		return E_FAIL;
	hr = pxofenum->GetChildren(&children);
	if (FAILED(hr)) return hr;

	for (SIZE_T child = 0; child < children; child++)
	{
		if (SUCCEEDED(pxofenum->GetChild(child, &pxofobjCur)))
		{
			hr = LoadFrames(pxofobjCur, pdeMesh, dwOptions, m_dwFVF,
			                m_pd3dDevice, pdeMesh->pframeRoot);
			GXRELEASE(pxofobjCur);

			if (FAILED(hr))
			{
				ReleaseData(hr);
				return E_FAIL;
			}
			if (TimeOut())
			{
				return E_FAIL;
			}
		}
	}

	return S_OK;
}

HRESULT CSkinnedMesh::LoadMeshHierarchyFromMemLoadStep6()
{
	FLOG("CSkinnedMesh::LoadMeshHierarchyFromMemStep6");

	HRESULT hr;
	hr = FindBones(pdeMesh->pframeRoot, pdeMesh);

	if (FAILED(hr))
	{
		ReleaseData(hr);
		return E_FAIL;
	}

	delete[]pdeMesh->szName;
	pdeMesh->szName = nullptr;

	DeleteSelectedMesh();

	// link into the draw list
	pdeMesh->pdeNext = m_pdeHead;
	m_pdeHead = pdeMesh;

	m_pdeSelected = pdeMesh;
	m_pmcSelectedMesh = pdeMesh->pframeRoot->pmcMesh;

	m_pframeSelected = pdeMesh->pframeRoot;

	hr = CalculateBoundingSphere(pdeMesh);
	m_fRadius = pdeMesh->fRadius;
	m_vCenter = pdeMesh->vCenter;

	if (FAILED(hr))
	{
		ReleaseData(hr);
		return E_FAIL;
	}

	SetProjectionMatrix();

	m_pdeSelected->fCurTime = 0.0f;
	m_pdeSelected->fMaxTime = 2000.0f;

	D3DXMatrixTranslation(&m_pdeSelected->pframeRoot->matRot,
	                      -pdeMesh->vCenter.x, -pdeMesh->vCenter.y, -pdeMesh->vCenter.z);
	m_pdeSelected->pframeRoot->matRotOrig = m_pdeSelected->pframeRoot->matRot;

	ReleaseData(hr);
	return S_OK;
}

void CSkinnedMesh::ReleaseData(HRESULT hr)
{
	//	util::del(m_pGameData);// m_pDataHeader는 m_pData는 무효하고, 나머지는 유효하다.
	//util::del(pMemory);
	int totalNum = m_pGameData->GetTotalNumber();
	for (int i = 0; i < totalNum; i++)
		m_pDataHeader[i].m_pData = nullptr;
	GXRELEASE(pxofobjCur);
	GXRELEASE(pxofenum);
	GXRELEASE(pxofapi);

	if (FAILED(hr) && pdeMesh != nullptr)
	{
		// 에러 발생
		delete pdeMesh;
		pdeMesh = nullptr;
	}

	SDrawElement* pdeCur;
	pdeCur = m_pdeHead;
	while (pdeCur != nullptr)
	{
		D3DXMatrixIdentity(&pdeCur->pframeRoot->matRot);
		D3DXMatrixIdentity(&pdeCur->pframeRoot->matTrans);
		pdeCur = pdeCur->pdeNext;
	}
}

void CSkinnedMesh::DeleteLoadingGameData()
{
	if (this->GetIsLoadingFlag())
	{
		g_pD3dApp->DeleteLoadingGameData(this);
	}
}

BOOL CSkinnedMesh::TimeOut()
{
	if (timeGetTime() - m_StartTime > LOADING_TICK)
	{
		m_StartTime = NULL;
		return TRUE;
	}
	return FALSE;
}

void CSkinnedMesh::SetIsLoadingStartTime()
{
	m_StartTime = timeGetTime();
}

int CSkinnedMesh::GetIsLoadingStartTime() const
{
	if (NULL == m_StartTime) return 1;

	return m_StartTime;
}

