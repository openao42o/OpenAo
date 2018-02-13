// SkinnedMesh.cpp: implementation of the CSkinnedMesh class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SkinnedMesh.h"
#include "rmxfguid.h"
#include "tchar.h"
#include "stdio.h"
#include "d3dutil.h"
#include "dxutil.h"
#include "NPCGlobal.h"
#include "rmxftmpl.h"
#include "DebugCheckTime.h"

#define D3DRM_XTEMPLATE_BYTES 3278
#define D3DFVF_VERTEX (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_TEX1)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//extern unsigned char * D3DRM_XTEMPLATES;

char *szTemplates = "xof 0303txt 0032\
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
    float fTime;

    if (m_pMatrixKeys )
    {
        fTime = (float)fmod(fGlobalTime, m_pMatrixKeys[m_cMatrixKeys-1].dwTime);

        for (iKey = 0 ;iKey < m_cMatrixKeys ; iKey++)
        {
            if ((float)m_pMatrixKeys[iKey].dwTime > fTime)
            {
                dwp3 = iKey;

                if (iKey > 0)
                {
                    dwp2= iKey - 1;
                }
                else  // when iKey == 0, then dwp2 == 0
                {
                    dwp2 = iKey;
                }

                break;
            }
        }
        fTime1 = (float)m_pMatrixKeys[dwp2].dwTime;
        fTime2 = (float)m_pMatrixKeys[dwp3].dwTime;

        if ((fTime2 - fTime1) ==0)
            fLerpValue = 0;
        else
            fLerpValue =  (fTime - fTime1)  / (fTime2 - fTime1);

        if (fLerpValue > 0.5)
        {
            iKey = dwp3;
        }
        else
        {
            iKey = dwp2;
        }

        pframeToAnimate->matRot = m_pMatrixKeys[iKey].mat;
    }
    else
    {
        D3DXMatrixIdentity(&matResult);

        if (m_pScaleKeys)
        {
            dwp2 = dwp3 = 0;

            fTime = (float)fmod(fGlobalTime, m_pScaleKeys[m_cScaleKeys-1].dwTime);

            for (iKey = 0 ;iKey < m_cScaleKeys ; iKey++)
            {
                if ((float)m_pScaleKeys[iKey].dwTime > fTime)
                {
                    dwp3 = iKey;

                    if (iKey > 0)
                    {
                        dwp2= iKey - 1;
                    }
                    else  // when iKey == 0, then dwp2 == 0
                    {
                        dwp2 = iKey;
                    }

                    break;
                }
            }
            fTime1 = (float)m_pScaleKeys[dwp2].dwTime;
            fTime2 = (float)m_pScaleKeys[dwp3].dwTime;

            if ((fTime2 - fTime1) ==0)
                fLerpValue = 0;
            else
                fLerpValue =  (fTime - fTime1)  / (fTime2 - fTime1);

            D3DXVec3Lerp(&vScale,
                    &m_pScaleKeys[dwp2].vScale,
                    &m_pScaleKeys[dwp3].vScale,
                    fLerpValue);


            D3DXMatrixScaling(&matTemp, vScale.x, vScale.y, vScale.z);

            D3DXMatrixMultiply(&matResult, &matResult, &matTemp);

            bAnimate = true;
        }

        //check rot keys
        if (m_pRotateKeys )
        {
            int i1 = 0;
            int i2 = 0;

            fTime = (float)fmod(fGlobalTime, m_pRotateKeys[m_cRotateKeys-1].dwTime);

            for (iKey = 0 ;iKey < m_cRotateKeys ; iKey++)
            {
                if ((float)m_pRotateKeys[iKey].dwTime > fTime)
                {
                    i1 = (iKey > 0) ? iKey - 1 : 0;
                    i2 = iKey;
                    break;
                }
            }

            fTime1 = (float)m_pRotateKeys[i1].dwTime;
            fTime2 = (float)m_pRotateKeys[i2].dwTime;

            if ((fTime2 - fTime1) ==0)
                fLerpValue = 0;
            else
                fLerpValue =  (fTime - fTime1)  / (fTime2 - fTime1);
           D3DXQuaternionSlerp(&quat, &m_pRotateKeys[i1].quatRotate, &m_pRotateKeys[i2].quatRotate, fLerpValue);

            quat.w = -quat.w;
            D3DXMatrixRotationQuaternion(&matTemp, &quat);
            D3DXMatrixMultiply(&matResult, &matResult, &matTemp);

            bAnimate = true;
        }

        if (m_pPositionKeys)
        {
            dwp2=dwp3=0;

            fTime = (float)fmod(fGlobalTime, m_pPositionKeys[m_cPositionKeys-1].dwTime);

            for (iKey = 0 ;iKey < m_cPositionKeys ; iKey++)
            {
                if ((float)m_pPositionKeys[iKey].dwTime > fTime)
                {
                    dwp3 = iKey;

                    if (iKey > 0)
                    {
                        dwp2= iKey - 1;
                    }
                    else  // when iKey == 0, then dwp2 == 0
                    {
                        dwp2 = iKey;
                    }

                    break;
                }
            }
            fTime1 = (float)m_pPositionKeys[dwp2].dwTime;
            fTime2 = (float)m_pPositionKeys[dwp3].dwTime;

            if ((fTime2 - fTime1) ==0)
                fLerpValue = 0;
            else
                fLerpValue =  (fTime - fTime1)  / (fTime2 - fTime1);


            D3DXVec3Lerp((D3DXVECTOR3*)&vPos,
                    &m_pPositionKeys[dwp2].vPos,
                    &m_pPositionKeys[dwp3].vPos,
                    fLerpValue);

//			m_vCurrentPos = vPos;
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

//////////////////////////////////////////////////////////////////
// CSkinnedMesh 
CSkinnedMesh::CSkinnedMesh()
{
	//	FLOG( "CSkinnedMesh()" );
	m_pd3dDevice = nullptr;

    m_pmcSelectedMesh = nullptr;
    m_pframeSelected = nullptr;
    m_pdeHead = nullptr;
    m_pdeSelected = nullptr;
    m_pBoneMatrices = nullptr;
    m_maxBones = 0;
    m_method = SOFTWARE;
//    m_method = D3DNONINDEXED;
    m_dwFVF = D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_NORMAL | D3DFVF_TEX1;

	m_bTotalTextureNum = 0;
	m_bTextureNum = 0;

	m_pGameData = nullptr;
	m_nRenderCount = 0;
	m_bMaterial = FALSE;
	m_bCheckBlend = FALSE;
	m_bProgressiveMesh = 0;
	D3DUtil_InitMaterial( m_material, 1.0f, 1.0f, 1.0f );// 임시 코드
	m_material.Specular.r = 1.0f;
	m_material.Specular.g = 1.0f;
	m_material.Specular.b = 1.0f;
	m_material.Power = 1.0f;
	m_bMultiTexture = FALSE;
	m_pOrderTexture = nullptr;
	m_fRadius = 0.0f;
	m_vCenter = D3DXVECTOR3(0,0,0);
}

CSkinnedMesh::CSkinnedMesh(bool bProgressiveMesh)
{
	//	FLOG( "CSkinnedMesh(BOOL bProgressiveMesh)" );
	m_pd3dDevice = nullptr;

    m_pmcSelectedMesh = nullptr;
    m_pframeSelected = nullptr;
    m_pdeHead = nullptr;
    m_pdeSelected = nullptr;
    m_pBoneMatrices = nullptr;
    m_maxBones = 0;
    m_method = SOFTWARE;
//    m_method = D3DNONINDEXED;
    m_dwFVF = D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_NORMAL | D3DFVF_TEX1;

	m_bTotalTextureNum = 0;
	m_bTextureNum = 0;

	m_pGameData = nullptr;
	m_nRenderCount = 0;
	m_bMaterial = FALSE;
	m_bCheckBlend = FALSE;
	m_bProgressiveMesh = bProgressiveMesh;
	D3DUtil_InitMaterial( m_material, 1.0f, 1.0f, 1.0f );// 임시 코드
	m_material.Specular.r = 1.0f;
	m_material.Specular.g = 1.0f;
	m_material.Specular.b = 1.0f;
	m_material.Power = 1.0f;
	m_bMultiTexture = FALSE;
	m_pOrderTexture = nullptr;
	m_fRadius = 0.0f;
	m_vCenter = D3DXVECTOR3(0,0,0);
}

CSkinnedMesh::~CSkinnedMesh()
{
}

HRESULT CSkinnedMesh::InitDeviceObjects()
{
	//	FLOG( "CSkinnedMesh::InitDeviceObjects()" );
// 2004-06-17, cmkwon,
	m_pd3dDevice = g_pNPCGlobal->m_D3DApp.GetD3DDevice();
    m_pd3dDevice->GetDeviceCaps( &m_d3dCaps );
	return CAtumNode::InitDeviceObjects();
}

HRESULT CSkinnedMesh::RestoreDeviceObjects()
{
 	//	FLOG( "CSkinnedMesh::RestoreDeviceObjects()" );
   HRESULT hr = S_OK;

	return CAtumNode::RestoreDeviceObjects();
}

HRESULT CSkinnedMesh::InvalidateDeviceObjects()
{
	//	FLOG( "CSkinnedMesh::InvalidateDeviceObjects()" );
	for (SDrawElement* pdeCurr = m_pdeHead; pdeCurr != NULL; pdeCurr = pdeCurr->pdeNext)
	{
		ReleaseDeviceDependentMeshes(pdeCurr->pframeRoot);
	}
	return CAtumNode::InvalidateDeviceObjects();
}
HRESULT CSkinnedMesh::DeleteDeviceObjects()
{
	if (m_pdeSelected == m_pdeHead) m_pdeSelected = nullptr;

	delete m_pdeHead;
	m_pdeHead = nullptr;

	delete [] m_pBoneMatrices;
	m_pBoneMatrices = nullptr;

	m_pOrderTexture = nullptr;
	return CAtumNode::DeleteDeviceObjects();
}

void CSkinnedMesh::Tick(float fElapsedTime)
{
	//	FLOG( "CSkinnedMesh::Tick(float fElapsedTime)" );
	SDrawElement *pdeCur;
	SFrame *pframeCur;

	pdeCur = m_pdeHead;
	while (pdeCur != NULL)
	{
		pdeCur->fCurTime = fElapsedTime*160;
		pframeCur = pdeCur->pframeAnimHead;
		while (pframeCur != NULL)
		{
			pframeCur->SetTime(pdeCur->fCurTime);
			pframeCur = pframeCur->pframeAnimNext;
		}

		pdeCur = pdeCur->pdeNext;
	}
}

HRESULT CSkinnedMesh::SetProjectionMatrix()
{
//	//	FLOG( "CSkinnedMesh::SetProjectionMatrix()" );
	D3DXMATRIX mat;

	if (m_pdeHead == NULL)
		return S_OK;

	FLOAT fAspect = 1.0f;
//	D3DXMatrixPerspectiveFovRH(&mat, 0.25f*3.141592654f, fAspect, m_pdeSelected->fRadius / 64, m_pdeSelected->fRadius * 200);
	D3DXMatrixPerspectiveFovRH(&mat, D3DX_PI/3, 800.0f/600.0f, 1.0f, 10000.0f);
	HRESULT hr = m_pd3dDevice->SetTransform( D3DTS_PROJECTION, (D3DMATRIX*)&mat );
	if (FAILED(hr))
		return hr;
	// Set Projection Matrix for vertex shader
	D3DXMatrixTranspose(&mat, &mat);
	return S_OK;
}

COLLISION_RESULT CSkinnedMesh::CheckCollision(const D3DXMATRIX& mat)
{
//	//	FLOG( "CSkinnedMesh::CheckCollision(D3DXMATRIX mat)" );
	COLLISION_RESULT collResult,checkcollResult;
	SDrawElement *pdeCur;
    pdeCur = m_pdeHead;
	collResult.fDist = 10000.0f;
	collResult.vNormalVector = D3DXVECTOR3(0,0,0);
    while (pdeCur != NULL)
    {
        UpdateFrames(pdeCur->pframeRoot, m_mWorld);
		checkcollResult = CheckCollDist(pdeCur->pframeRoot,mat);
		if(collResult.fDist > checkcollResult.fDist)
			collResult = checkcollResult;
        pdeCur = pdeCur->pdeNext;
    }
	return collResult;
}

COLLISION_RESULT CSkinnedMesh::CheckCollDist(SFrame *pframeCur, const D3DXMATRIX& mat)
{
//	//	FLOG( "CSkinnedMesh::CheckCollDist(SFrame *pframeCur,D3DXMATRIX mat)" );
	COLLISION_RESULT collResult,checkcollResult;
	SMeshContainer *pmcMesh;
	SFrame *pframeChild;
	collResult.fDist = 10000.0f;
	collResult.vNormalVector = D3DXVECTOR3(0,0,0);
//	if (pframeCur->pmcMesh != NULL)
//	{
//		m_pd3dDevice->SetTransform(D3DTS_WORLD, &pframeCur->matCombined);
//	}
	pmcMesh = pframeCur->pmcMesh;
	while (pmcMesh != NULL)
	{
		// 충돌 체크해야함
		checkcollResult = CheckCollDistDetail(pmcMesh, pframeCur->matCombined,mat);
		if(collResult.fDist > checkcollResult.fDist)
			collResult = checkcollResult;
		pmcMesh = pmcMesh->pmcNext;
	}
	pframeChild = pframeCur->pframeFirstChild;
	while (pframeChild != NULL)
	{
		checkcollResult = CheckCollDist(pframeChild,mat);
		if(collResult.fDist > checkcollResult.fDist)
			collResult = checkcollResult;
		pframeChild = pframeChild->pframeSibling;
	}
	return collResult;
}

COLLISION_RESULT CSkinnedMesh::CheckCollDistDetail(SMeshContainer *pmcMesh, const D3DXMATRIX& matThis, const D3DXMATRIX& mat)
{
	//	FLOG( "CSkinnedMesh::CheckCollDistDetail(SMeshContainer *pmcMesh,D3DXMATRIX mat)" );
	COLLISION_RESULT collResult;
	//	collResult.fDist = DEFAULT_COLLISION_DISTANCE;
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

	BOOL bHit, bIntersections;
	DWORD dwFace;
	FLOAT fBary1, fBary2, fDist;

	//	D3DXMATRIX matProj;
	D3DXVECTOR3 vPickRayDir, vPickRayOrig;
	D3DXMATRIX m, matWorld;

		//D3DXMatrixInverse( &m, NULL, &matThis );
	//matWorld = matThis;
	//m = mat*m;


	//	g_pD3dDev->GetTransform( D3DTS_PROJECTION, &matProj );
	// Get the inverse view matrix
	m_pd3dDevice->GetTransform(D3DTS_WORLD, &matWorld);
	m = matWorld * mat;
	D3DXMatrixInverse(&m, nullptr, &m);
	vPickRayDir.x = m._31;
	vPickRayDir.y = m._32;
	vPickRayDir.z = m._33;
	vPickRayOrig.x = m._41;
	vPickRayOrig.y = m._42;
	vPickRayOrig.z = m._43;

	D3DXIntersect(pmcMesh->pMesh, &vPickRayOrig, &vPickRayDir, &bHit, &dwFace, &fBary1, &fBary2, &fDist, nullptr, nullptr);

	if (bHit)
	{
		bIntersections = TRUE;
		m_Intersection.dwFace = dwFace;
		m_Intersection.fBary1 = fBary1;
		m_Intersection.fBary2 = fBary2;
		m_Intersection.fDist = fDist;
	}
	else
	{
		bIntersections = FALSE;
	}

	if (bIntersections)
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

		collResult.fDist = fDist;

		return collResult; // 어쩌구 구조체;
	}

	return collResult;// 어쩌구 구조체 충돌 없다.;
}


HRESULT CSkinnedMesh::LoadMeshHierarchyFromMem(CGameData* pGameData, const D3DXVECTOR3& vObjScale)	// 2012-05-23 by isshin 맵툴 개선 오브젝트 스케일 적용
{
	m_pGameData = pGameData;

    auto pdeMesh = new SDrawElement();
    
    pdeMesh->pframeRoot = new SFrame();

	LPD3DXFILE pxofapi = nullptr;

    auto hr = pdeMesh->pframeRoot ? D3DXFileCreate(&pxofapi) : E_OUTOFMEMORY;

	// Register templates for d3drm.

	if (SUCCEEDED(hr)) hr = pxofapi->RegisterTemplates(LPVOID(D3DRM_XTEMPLATES), D3DRM_XTEMPLATE_BYTES);

	if (SUCCEEDED(hr)) hr = pxofapi->RegisterTemplates(LPVOID(szTemplates), strlen(szTemplates));

	LPD3DXFILEENUMOBJECT pxofenum = nullptr;

	if (SUCCEEDED(hr))
	{
		auto data = m_pGameData->GetStartPosition();

		D3DXF_FILELOADMEMORY Memory { data->m_pData, data->m_DataSize };

		// Create enum object.

		hr = pxofapi->CreateEnumObject(&Memory, DXFILELOAD_FROMMEMORY, &pxofenum);
	}

	if (SUCCEEDED(hr))
	{
		SIZE_T children = 0;

		hr = pxofenum->GetChildren(&children);

		// Enumerate top level objects.
		// Top level objects are always data objects.

		for (SIZE_T child = 0; SUCCEEDED(hr) && child < children; child++)
		{
			LPD3DXFILEDATA pxofobjCur = nullptr;

			hr = pxofenum->GetChild(child, &pxofobjCur);

			if (SUCCEEDED(hr)) hr = LoadFrames(pxofobjCur, pdeMesh, 0, m_dwFVF, m_pd3dDevice, pdeMesh->pframeRoot);

			GXRELEASE(pxofobjCur);
		}

	}

	GXRELEASE(pxofenum);
	GXRELEASE(pxofapi);

	if (SUCCEEDED(hr)) hr = FindBones(pdeMesh->pframeRoot, pdeMesh);
    
	if (SUCCEEDED(hr))
	{    
		hr = CalculateBoundingSphere(pdeMesh, vObjScale);	// 2012-05-23 by isshin 맵툴 개선 오브젝트 스케일 적용
  		m_fRadius = pdeMesh->fRadius;
		m_vCenter = pdeMesh->vCenter;
	}

	if (SUCCEEDED(hr))
	{
	    SetProjectionMatrix();
    
		pdeMesh->fCurTime = 0.0f;
		pdeMesh->fMaxTime = 2000.0f;

		// panoskj points out code that doesn't do what it seems
		// in comments you can find the (possibly) intended effect
		// moreover pframeRoot->matRot is later set to identity

		D3DXMATRIX objscale;
		//D3DXMATRIX objtrans;

		D3DXMatrixScaling(&objscale, vObjScale.x, vObjScale.y, vObjScale.z);
		//D3DXMatrixTranslation(&objtrans, -pdeMesh->vCenter.x, -pdeMesh->vCenter.y, -pdeMesh->vCenter.z);

		//m_pdeSelected->pframeRoot->matRot = objscale * m_pdeSelected->pframeRoot->matRot * objtrans;

		D3DXMatrixMultiply(&pdeMesh->pframeRoot->matRot, &objscale, &pdeMesh->pframeRoot->matRot);
		D3DXMatrixTranslation(&pdeMesh->pframeRoot->matRot, -pdeMesh->vCenter.x, -pdeMesh->vCenter.y, -pdeMesh->vCenter.z);
    
		pdeMesh->pframeRoot->matRotOrig = pdeMesh->pframeRoot->matRot;
	}

	if (SUCCEEDED(hr))
	{
		delete[] pdeMesh->szName;
		pdeMesh->szName = nullptr;

		// delete the current mesh, now that the load has succeeded
		DeleteSelectedMesh();

		// link into the draw list
		pdeMesh->pdeNext = m_pdeHead;
		m_pdeHead = pdeMesh;

		m_pdeSelected = pdeMesh;
		m_pmcSelectedMesh = pdeMesh->pframeRoot->pmcMesh;

		m_pframeSelected = pdeMesh->pframeRoot;
	}
	else delete pdeMesh;

    for (auto cur = m_pdeHead; cur; cur = cur->pdeNext)
    {
        D3DXMatrixIdentity(&cur->pframeRoot->matRot);
        D3DXMatrixIdentity(&cur->pframeRoot->matTrans);
    }

    return hr;
}

//HRESULT CSkinnedMesh::LoadTexture(char* szfileName, 
//								  BYTE tex_num)
//{
//	//	FLOG( "CSkinnedMesh::LoadTexture(char* szfileName, BYTE tex_num)" );
//	if( FAILED( D3DXCreateTextureFromFile(m_pd3dDevice, szfileName, &m_pTexture[tex_num-1]))) return E_FAIL;
//		return TRUE;
//}

//HRESULT CSkinnedMesh::LoadFrames(LPDIRECTXFILEDATA pxofobjCur, 
//								 SDrawElement *pde, 
//								 DWORD options, DWORD fvf, 
//								 LPDIRECT3DDEVICE9 pD3DDevice, 
//								 SFrame *pframeParent)
//{
////	//	FLOG( "CSkinnedMesh::LoadFrames(LPDIRECTXFILEDATA pxofobjCur, SDrawElement *pde, DWORD options, DWORD fvf, LPDIRECT3DDEVICE8 pD3DDevice, SFrame *pframeParent)" );
//    HRESULT hr = S_OK;
//    LPDIRECTXFILEDATA pxofobjChild = NULL;
//    LPDIRECTXFILEOBJECT pxofChild = NULL;
//    const GUID *type;
//    DWORD cbSize;
//    D3DXMATRIX *pmatNew;
//    SFrame *pframeCur;
//    DWORD cchName;
//    
//    // Get the type of the object
//    hr = pxofobjCur->GetType(&type);
//    if (FAILED(hr))
//        goto e_Exit;
//    
//    
//    if (*type == TID_D3DRMMesh)
//    {
//        hr = LoadMesh(pxofobjCur, options, fvf, pD3DDevice, pframeParent);
//        if (FAILED(hr))
//            goto e_Exit;
//    }
//    else if (*type == TID_D3DRMFrameTransformMatrix)
//    {
//        hr = pxofobjCur->GetData(NULL, &cbSize, (PVOID*)&pmatNew);
//        if (FAILED(hr))
//            goto e_Exit;
//        
//        // update the parents matrix with the new one
//        pframeParent->matRot = *pmatNew;
//        pframeParent->matRotOrig = *pmatNew;
//    }
//    else if (*type == TID_D3DRMAnimationSet)
//    {
//        LoadAnimationSet(pxofobjCur, pde, options, fvf, pD3DDevice, pframeParent);
//    }
//    else if (*type == TID_D3DRMAnimation)
//    {
//        LoadAnimation(pxofobjCur, pde, options, fvf, pD3DDevice, pframeParent);
//    }
//    else if (*type == TID_D3DRMFrame)
//    {
//        pframeCur = new SFrame();
//        if (pframeCur == NULL)
//        {
//            hr = E_OUTOFMEMORY;
//            goto e_Exit;
//        }
//        
//        hr = pxofobjCur->GetName(NULL, &cchName);
//        if (FAILED(hr))
//            goto e_Exit;
//        
//        if (cchName > 0)
//        {
//            pframeCur->szName = new char[cchName];
//            if (pframeCur->szName == NULL)
//            {
//                hr = E_OUTOFMEMORY;
//                goto e_Exit;
//            }
//            
//            hr = pxofobjCur->GetName(pframeCur->szName, &cchName);
//            if (FAILED(hr))
//                goto e_Exit;
//        }
//        
//        pframeParent->AddFrame(pframeCur);
//        
//        // Enumerate child objects.
//        // Child object can be data, data reference or binary.
//        // Use QueryInterface() to find what type of object a child is.
//        while (SUCCEEDED(pxofobjCur->GetNextObject(&pxofChild)))
//        {
//            // Query the child for it's FileData
//            hr = pxofChild->QueryInterface(IID_IDirectXFileData,
//                (LPVOID *)&pxofobjChild);
//            if (SUCCEEDED(hr))
//            {
//                hr = LoadFrames(pxofobjChild, pde, options, fvf, pD3DDevice, pframeCur);
//                if (FAILED(hr))
//                    goto e_Exit;
//                
//                GXRELEASE(pxofobjChild);
//            }
//            
//            GXRELEASE(pxofChild);
//        }
//        
//    }
//    
//e_Exit:
//    GXRELEASE(pxofobjChild);
//    GXRELEASE(pxofChild);
//    return hr;
//}

//HRESULT CSkinnedMesh::LoadMesh(LPDIRECTXFILEDATA pxofobjCur,
//                                    DWORD options, DWORD fvf, 
//									LPDIRECT3DDEVICE9 pD3DDevice,
//                                    SFrame *pframeParent)
//{
////	//	FLOG( "CSkinnedMesh::LoadMesh(LPDIRECTXFILEDATA pxofobjCur, DWORD options, DWORD fvf, LPDIRECT3DDEVICE8 pD3DDevice, SFrame *pframeParent)" );
//    HRESULT hr = S_OK;
//    SMeshContainer *pmcMesh = NULL;
//    LPD3DXBUFFER pbufMaterials = NULL;
//    LPD3DXBUFFER pbufAdjacency = NULL;
//    DWORD cchName;
//    UINT cFaces;
//    UINT iMaterial;
//    LPDIRECT3DDEVICE9 m_pDevice = m_pd3dDevice;
//    LPDWORD pAdjacencyIn;
////    LPD3DXPMESH  pPMesh = NULL;
// //   LPD3DXPMESH  pPMeshTemp = NULL;
// //   LPD3DXMESH   pMesh = NULL;
////    LPD3DXMESH   pTempMesh = NULL;
//	int i;  
////	DWORD num;
//  
//    pmcMesh = new SMeshContainer();
////	for(i = 0;i < 5;i++)
////		pmcMesh->pMeshes[i] = NULL;
//	if (pmcMesh == NULL)
//    {
//        hr = E_OUTOFMEMORY;
//        goto e_Exit;
//    }
//    
//    hr = pxofobjCur->GetName(NULL, &cchName);
//    if (FAILED(hr))
//        goto e_Exit;
//    
//    if (cchName > 0)
//    {
//        pmcMesh->szName = new char[cchName];
//        if (pmcMesh->szName == NULL)
//        {
//            hr = E_OUTOFMEMORY;
//            goto e_Exit;
//        }
//        
//        hr = pxofobjCur->GetName(pmcMesh->szName, &cchName);
//        if (FAILED(hr))
//            goto e_Exit;
//    }
//    
//    hr = D3DXLoadSkinMeshFromXof(pxofobjCur, options, pD3DDevice, &pbufAdjacency, &pbufMaterials, &pmcMesh->cMaterials, 
//        &pmcMesh->m_pBoneNamesBuf, &pmcMesh->m_pBoneOffsetBuf, &pmcMesh->m_pSkinMesh);
//
////	DWORD num;//,num2,num3;
//
//    if (FAILED(hr))
//    {
//        if (hr == D3DXERR_LOADEDMESHASNODATA)
//            hr = S_OK;
//        goto e_Exit;
//    }
//	
//	
//    cFaces = pmcMesh->m_pSkinMesh->GetNumFaces();
//
//    pAdjacencyIn = static_cast<LPDWORD>(pbufAdjacency->GetBufferPointer());
//
//    pmcMesh->m_rgiAdjacency = new DWORD[cFaces * 3];
//
//    if (pmcMesh->m_rgiAdjacency == NULL)
//    {
//        hr = E_OUTOFMEMORY;
//
//        goto e_Exit;
//    }
//
//    memcpy(pmcMesh->m_rgiAdjacency, pAdjacencyIn, cFaces * 3 * sizeof(DWORD));
//
//    // Process skinning data
//    if (pmcMesh->m_pSkinMesh->GetNumBones())
//    {
//        // Update max bones of any mesh in the app
//        m_maxBones = max(pmcMesh->m_pSkinMesh->GetNumBones(), m_maxBones);
//        
//        pmcMesh->m_pBoneMatrix = new D3DXMATRIX*[pmcMesh->m_pSkinMesh->GetNumBones()];
//        if (pmcMesh->m_pBoneMatrix == NULL)
//            goto e_Exit;
//        pmcMesh->m_pBoneOffsetMat = reinterpret_cast<D3DXMATRIX*>(pmcMesh->m_pBoneOffsetBuf->GetBufferPointer());
//        
//        hr = GenerateMesh(pmcMesh);
//
//        if (FAILED(hr))
//            goto e_Exit;
//		/////////////////////// 본데이타를 무시하고 Temp Mesh를 생성하고 Progressive 를 적용한다 /////////////////////////////////////
////		if(m_bProgressiveMesh)
////		{
////	        pmcMesh->m_pSkinMesh->GetOriginalMesh(&pTempMesh);
////			D3DXCleanMesh(pTempMesh, (DWORD*)pbufAdjacency->GetBufferPointer(), &pMesh, (DWORD*)pbufAdjacency->GetBufferPointer(), NULL );
////		}
//    }
//    else
//    {
//        pmcMesh->m_pSkinMesh->GetOriginalMesh(&(pmcMesh->pMesh));
//        pmcMesh->m_pSkinMesh->Release();
//        pmcMesh->m_pSkinMesh = nullptr;
//        pmcMesh->cpattr = pmcMesh->cMaterials;
//		/////////////////////// 본데이타가 없으면 Temp Mesh를 생성하고 Progressive 를 적용한다 ///////////////////////////////////////
////		if(m_bProgressiveMesh)
////			D3DXCleanMesh(pmcMesh->pMesh, (DWORD*)pbufAdjacency->GetBufferPointer(), &pMesh, (DWORD*)pbufAdjacency->GetBufferPointer(), NULL );
//    }
//    if ((pbufMaterials == NULL) || (pmcMesh->cMaterials == 0))
//    {
//        pmcMesh->rgMaterials = new D3DMATERIAL9[1];
//        pmcMesh->pTextures = new LPDIRECT3DTEXTURE9[1];
//        if (pmcMesh->rgMaterials == NULL || pmcMesh->pTextures == NULL)
//        {
//            hr = E_OUTOFMEMORY;
//            goto e_Exit;
//        }
//        
//        memset(pmcMesh->rgMaterials, 0, sizeof(D3DMATERIAL9));
//        pmcMesh->rgMaterials[0].Diffuse.r = 1.0f;
//        pmcMesh->rgMaterials[0].Diffuse.g = 1.0f;
//        pmcMesh->rgMaterials[0].Diffuse.b = 1.0f;
//        pmcMesh->rgMaterials[0].Specular = pmcMesh->rgMaterials[0].Diffuse;
//        pmcMesh->pTextures[0] = NULL;
//    }
//    else
//    {
//        pmcMesh->rgMaterials = new D3DMATERIAL9[pmcMesh->cMaterials];
//        pmcMesh->pTextures = new LPDIRECT3DTEXTURE9[pmcMesh->cMaterials];
//        if (pmcMesh->rgMaterials == NULL || pmcMesh->pTextures == NULL)
//        {
//            hr = E_OUTOFMEMORY;
//            goto e_Exit;
//        }
//        
//        LPD3DXMATERIAL pMaterials = (LPD3DXMATERIAL)pbufMaterials->GetBufferPointer();
//        
//        for (iMaterial = 0; iMaterial < pmcMesh->cMaterials; iMaterial++)
//        {
//            
//            pmcMesh->rgMaterials[iMaterial] = pMaterials[iMaterial].MatD3D;
//			pmcMesh->rgMaterials[iMaterial].Ambient.r = 1.0f;
// 			pmcMesh->rgMaterials[iMaterial].Ambient.g = 1.0f;
// 			pmcMesh->rgMaterials[iMaterial].Ambient.b = 1.0f;
//          
//            pmcMesh->pTextures[iMaterial] = NULL;
//            if (pMaterials[iMaterial].pTextureFilename != NULL)
//            {
//				if(m_pDataHeader && m_pGameData)
//				{
//					int num = m_pGameData->GetTotalNumber();
//					for(i=1; i<num; i++ )
//					{
//						if(strcmp(m_pDataHeader[i].m_FileName, pMaterials[iMaterial].pTextureFilename) == 0 && m_pTexture)
//						{
//							m_pTexture[i-1]->AddRef();
//							pmcMesh->pTextures[iMaterial] = m_pTexture[i-1];
//							break;
//						}
//					}
//
//				}
//				else 
//				{
//					TCHAR szPath[MAX_PATH];
//					DXUtil_FindMediaFile(szPath, pMaterials[iMaterial].pTextureFilename);
//
//					hr = D3DXCreateTextureFromFile(m_pDevice, szPath, &(pmcMesh->pTextures[iMaterial]));
//					if (FAILED(hr))
//						pmcMesh->pTextures[iMaterial] = NULL;
//				}
//            }
//        }
//    }
//    // add the mesh to the parent frame
//    pframeParent->AddMesh(pmcMesh);
// 	// 임시
//	if(m_bProgressiveMesh)
//	{
//		DWORD maxnum = pMesh->GetNumVertices();
//		DWORD minnum = maxnum/10;
//		num = (DWORD)((maxnum-minnum)/5);
//		for(i = 0;i < 5;i++)
//			D3DXSimplifyMesh(pMesh,(DWORD*)pbufAdjacency->GetBufferPointer(),NULL,NULL,(maxnum-(num*(i+1))),D3DXMESHSIMP_VERTEX,&pmcMesh->pMeshes[i]);
//		SAFE_RELEASE(pTempMesh);
//		SAFE_RELEASE(pPMesh);
//	}
//*/	pmcMesh = NULL;
//   
//e_Exit:
//    delete pmcMesh;
//	pmcMesh = NULL;
//    
////    SAFE_RELEASE( pMesh );
////    SAFE_RELEASE( pPMesh );
//    GXRELEASE(pbufAdjacency);
//    GXRELEASE(pbufMaterials);
//    return hr;
//}

HRESULT CSkinnedMesh::LoadFrames(LPD3DXFILEDATA pxofobjCur, SDrawElement* pde, DWORD options, DWORD fvf, LPDIRECT3DDEVICE9 pD3DDevice, SFrame* pframeParent)
{
	//	FLOG( "CSkinnedMesh::LoadFrames(LPDIRECTXFILEDATA pxofobjCur, SDrawElement *pde, DWORD options, DWORD fvf, LPDIRECT3DDEVICE9 pD3DDevice, SFrame *pframeParent)" );
	auto hr = S_OK;
	//LPD3DXFILEDATA pxofobjChild = nullptr;
	LPD3DXFILEDATA pxofChild = nullptr;
	//LPDIRECTXFILEOBJECT pxofChild = NULL;
	GUID type;
	SIZE_T cbSize;
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
		const D3DXMATRIX* pmatNew;
		hr = pxofobjCur->Lock(&cbSize, reinterpret_cast<LPCVOID*>(&pmatNew));

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
					//hr = pxofChild->QueryInterface(IID_ID3DXFileData, reinterpret_cast<LPVOID*>(&pxofobjChild));

					//if (SUCCEEDED(hr))
					//{
						hr = LoadFrames(pxofChild, pde, options, fvf, pD3DDevice, pframeCur);

						if (FAILED(hr)) goto e_Exit;

						//GXRELEASE(pxofobjChild);
					//}
				}

				GXRELEASE(pxofChild);
			}
		}
	}

e_Exit:
	//GXRELEASE(pxofobjChild);
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

		util::release(pmcMesh->m_pSkinMeshInfo);

		pmcMesh->cpattr = pmcMesh->cMaterials;
	}

	if (!pbufMaterials || pmcMesh->cMaterials == 0)
	{
		pmcMesh->rgMaterials = new D3DMATERIAL9[1];
		//pmcMesh->pTextures = new LPDIRECT3DTEXTURE9[1];
		if (pmcMesh->rgMaterials == nullptr) // || pmcMesh->pTextures == nullptr)
		{
			hr = E_OUTOFMEMORY;
			goto e_Exit;
		}

		memset(pmcMesh->rgMaterials, 0, sizeof(D3DMATERIAL9));
		pmcMesh->rgMaterials[0].Diffuse.r = 1.0f;
		pmcMesh->rgMaterials[0].Diffuse.g = 1.0f;
		pmcMesh->rgMaterials[0].Diffuse.b = 1.0f;
		pmcMesh->rgMaterials[0].Ambient = pmcMesh->rgMaterials[0].Diffuse;
		//pmcMesh->pTextures[0] = nullptr;
	}
	else
	{
		pmcMesh->rgMaterials = new D3DMATERIAL9[pmcMesh->cMaterials];
		//pmcMesh->pTextures = new LPDIRECT3DTEXTURE9[pmcMesh->cMaterials];
		if (pmcMesh->rgMaterials == nullptr) // || pmcMesh->pTextures == nullptr)
		{
			hr = E_OUTOFMEMORY;
			goto e_Exit;
		}

		auto pMaterials = LPD3DXMATERIAL(pbufMaterials->GetBufferPointer());

		for (iMaterial = 0; iMaterial < pmcMesh->cMaterials; iMaterial++)
		{
			pmcMesh->rgMaterials[iMaterial] = pMaterials[iMaterial].MatD3D;
			pmcMesh->rgMaterials[iMaterial].Ambient.r = 1.0f;
			pmcMesh->rgMaterials[iMaterial].Ambient.g = 1.0f;
			pmcMesh->rgMaterials[iMaterial].Ambient.b = 1.0f;

			//pmcMesh->pTextures[iMaterial] = nullptr;
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



//HRESULT CSkinnedMesh::GenerateMesh(SMeshContainer *pmcMesh)
//{
////	//	FLOG( "CSkinnedMesh::GenerateMesh(SMeshContainer *pmcMesh)" );
//	// ASSUMPTION:  pmcMesh->m_rgiAdjacency contains the current adjacency
//	HRESULT hr = S_OK;
//	LPDIRECT3DDEVICE9 pDevice = NULL;
//	DWORD cFaces = pmcMesh->m_pSkinMesh->GetNumFaces();
////	DWORD* rgiAdjacency;
//
//	hr  = pmcMesh->m_pSkinMesh->GetDevice(&pDevice);
//	if (FAILED(hr))
//		goto e_Exit;
//
//	GXRELEASE(pmcMesh->pMesh);
//	delete [] m_pBoneMatrices;
//
//	pmcMesh->pMesh      = NULL;
//	m_pBoneMatrices     = nullptr;
//    
//	if (m_method == D3DNONINDEXED)
//	{
//		LPD3DXBONECOMBINATION   rgBoneCombinations;
//
//		hr = pmcMesh->m_pSkinMesh->ConvertToBlendedMesh
//                                   (
//                                       D3DXMESH_MANAGED|D3DXMESHOPT_VERTEXCACHE, 
//                                       pmcMesh->m_rgiAdjacency, 
//                                       NULL, 
//                                       &pmcMesh->cpattr, 
//                                       &pmcMesh->m_pBoneCombinationBuf, 
//                                       NULL,
//                                       NULL,
//                                       &pmcMesh->pMesh
//                                   );
//		
//		if (FAILED(hr))
//		{
//			goto e_Exit;
//		}
//
//		// calculate the max face influence count
//
//		if ((pmcMesh->pMesh->GetFVF() & D3DFVF_POSITION_MASK) != D3DFVF_XYZ)
//		{
//			pmcMesh->m_maxFaceInfl = 1 + ((pmcMesh->pMesh->GetFVF() & D3DFVF_POSITION_MASK) - D3DFVF_XYZRHW) / 2;
//		}
//		else
//		{
//			pmcMesh->m_maxFaceInfl = 1;
//		}
//
//		/* If the device can only do 2 matrix blends, ConvertToBlendedMesh cannot approximate all meshes to it
//			Thus we split the mesh in two parts: The part that uses at most 2 matrices and the rest. The first is
//			drawn using the device's HW vertex processing and the rest is drawn using SW vertex processing. */
//		if (m_d3dCaps.MaxVertexBlendMatrices <= 2)       
//		{
//			// calculate the index of the attribute table to split on
//			rgBoneCombinations  = reinterpret_cast<LPD3DXBONECOMBINATION>(pmcMesh->m_pBoneCombinationBuf->GetBufferPointer());
//
//			for (pmcMesh->iAttrSplit = 0; pmcMesh->iAttrSplit < pmcMesh->cpattr; pmcMesh->iAttrSplit++)
//			{
//                DWORD   cInfl   = 0;
//
//                for (DWORD iInfl = 0; iInfl < pmcMesh->m_maxFaceInfl; iInfl++)
//                {
//                    if (rgBoneCombinations[pmcMesh->iAttrSplit].BoneId[iInfl] != UINT_MAX)
//                    {
//                        ++cInfl;
//                    }
//                }
//
//                if (cInfl > m_d3dCaps.MaxVertexBlendMatrices)
//                {
//                    break;
//                }
//            }
//
//            // if there is both HW and SW, add the Software Processing flag
//            if (pmcMesh->iAttrSplit < pmcMesh->cpattr)
//            {
//                LPD3DXMESH pMeshTmp;
//
//                hr = pmcMesh->pMesh->CloneMeshFVF(D3DXMESH_SOFTWAREPROCESSING|pmcMesh->pMesh->GetOptions(), 
//                                                    pmcMesh->pMesh->GetFVF(),
//                                                    pDevice, &pMeshTmp);
//                if (FAILED(hr))
//                {
//                    goto e_Exit;
//                }
//
//                pmcMesh->pMesh->Release();
//                pmcMesh->pMesh = pMeshTmp;
//                pMeshTmp = NULL;
//            }
//        }
//        else
//        {
//            pmcMesh->iAttrSplit = pmcMesh->cpattr;
//        }
//    }
//    else if (m_method == D3DINDEXEDVS)
//    {
//        // Get palette size
//        pmcMesh->m_paletteSize = min(28, pmcMesh->m_pSkinMesh->GetNumBones());
//
//        DWORD flags = D3DXMESHOPT_VERTEXCACHE;
//        if (m_d3dCaps.VertexShaderVersion >= D3DVS_VERSION(1, 1))
//        {
//            pmcMesh->m_bUseSW = false;
//            flags |= D3DXMESH_MANAGED;
//        }
//        else
//        {
//            pmcMesh->m_bUseSW = true;
//            flags |= D3DXMESH_SYSTEMMEM;
//        }
//
//        hr = pmcMesh->m_pSkinMesh->ConvertToIndexedBlendedMesh(flags, pmcMesh->m_rgiAdjacency, pmcMesh->m_paletteSize, NULL,
//            &pmcMesh->cpattr, &pmcMesh->m_pBoneCombinationBuf, NULL, NULL, &pmcMesh->pMesh);
//        if (FAILED(hr))
//            goto e_Exit;
//
//        if ((pmcMesh->pMesh->GetFVF() & D3DFVF_POSITION_MASK) != D3DFVF_XYZ)
//        {
//            pmcMesh->m_maxFaceInfl = ((pmcMesh->pMesh->GetFVF() & D3DFVF_POSITION_MASK) - D3DFVF_XYZRHW) / 2;
//        }
//        else
//        {
//            pmcMesh->m_maxFaceInfl = 1;
//        }
//
//        // FVF has to match our declarator. Vertex shaders are not as forgiving as FF pipeline
//        DWORD newFVF = (pmcMesh->pMesh->GetFVF() & D3DFVF_POSITION_MASK) | D3DFVF_NORMAL | D3DFVF_TEX1 | D3DFVF_LASTBETA_UBYTE4;
//        if (newFVF != pmcMesh->pMesh->GetFVF())
//        {
//            LPD3DXMESH pMesh;
//            hr = pmcMesh->pMesh->CloneMeshFVF(pmcMesh->pMesh->GetOptions(), newFVF, pDevice, &pMesh);
//            if (!FAILED(hr))
//            {
//                pmcMesh->pMesh->Release();
//                pmcMesh->pMesh = pMesh;
//                pMesh = NULL;
//            }
//        }
//    }
//    else if (m_method == D3DINDEXED)
//    {
//        DWORD maxFaceInfl;
//        DWORD flags = D3DXMESHOPT_VERTEXCACHE;
//
//        hr = pmcMesh->m_pSkinMesh->GetMaxFaceInfluences(&maxFaceInfl);
//        if (FAILED(hr))
//            goto e_Exit;
//
//        // 12 entry palette guarantees that any triangle (4 independent influences per vertex of a tri)
//        // can be handled
//        maxFaceInfl = min(maxFaceInfl, 12);
//
//        if (m_d3dCaps.MaxVertexBlendMatrixIndex + 1 < maxFaceInfl)
//        {
//            // HW does not support indexed vertex blending. Use SW instead
//            pmcMesh->m_paletteSize = min(256, pmcMesh->m_pSkinMesh->GetNumBones());
//            pmcMesh->m_bUseSW = true;
//            flags |= D3DXMESH_SYSTEMMEM;
//        }
//        else
//        {
//            pmcMesh->m_paletteSize = min(m_d3dCaps.MaxVertexBlendMatrixIndex + 1, pmcMesh->m_pSkinMesh->GetNumBones());
//            pmcMesh->m_bUseSW = false;
//            flags |= D3DXMESH_MANAGED;
//        }
//
//        hr = pmcMesh->m_pSkinMesh->ConvertToIndexedBlendedMesh(flags, pmcMesh->m_rgiAdjacency, pmcMesh->m_paletteSize, NULL,
//            &pmcMesh->cpattr, &pmcMesh->m_pBoneCombinationBuf, NULL, NULL, &pmcMesh->pMesh);
//        if (FAILED(hr))
//            goto e_Exit;
//
//        // Here we are talking of max vertex influence which we determine from 
//        // the FVF of the returned mesh
//        if ((pmcMesh->pMesh->GetFVF() & D3DFVF_POSITION_MASK) != D3DFVF_XYZ)
//        {
//            pmcMesh->m_maxFaceInfl = ((pmcMesh->pMesh->GetFVF() & D3DFVF_POSITION_MASK) - D3DFVF_XYZRHW) / 2;
//        }
//        else
//        {
//            pmcMesh->m_maxFaceInfl = 1;
//        }
//    }
//    else if (m_method == SOFTWARE)
//    {
//        hr = pmcMesh->m_pSkinMesh->GenerateSkinnedMesh
//                                   (
//                                       D3DXMESH_DYNAMIC,          // options
//                                       0.0f,                        // minimum bone weight allowed
//                                       pmcMesh->m_rgiAdjacency,     // adjacency of in-mesh
//                                       NULL,     // adjacency of out-mesh
//                                       NULL,     // face remap array
//                                       NULL,     // vertex remap buffer
//                                       &pmcMesh->pMesh              // out-mesh
//                                   );
//        if (FAILED(hr))
//            goto e_Exit;
//
//        hr = pmcMesh->pMesh->GetAttributeTable(NULL, &pmcMesh->cpattr);
//        if (FAILED(hr))
//            goto e_Exit;
//
//        delete[] pmcMesh->m_pAttrTable;
//        pmcMesh->m_pAttrTable  = new D3DXATTRIBUTERANGE[pmcMesh->cpattr];
//        if (pmcMesh->m_pAttrTable == NULL)
//        {
//            hr = E_OUTOFMEMORY;
//            goto e_Exit;
//        }
//
//        hr = pmcMesh->pMesh->GetAttributeTable(pmcMesh->m_pAttrTable, NULL);
//        if (FAILED(hr))
//            goto e_Exit;
//
//        hr = pmcMesh->m_pSkinMesh->GetMaxFaceInfluences(&pmcMesh->m_maxFaceInfl);
//        if (FAILED(hr))
//            goto e_Exit;
//
//        // Allocate space for blend matrices
//        m_pBoneMatrices  = new D3DXMATRIXA16[m_maxBones];
//        if (m_pBoneMatrices == NULL)
//        {
//            hr = E_OUTOFMEMORY;
//            goto e_Exit;
//        }
//    }
//    pmcMesh->m_Method = m_method;
//
//e_Exit:
//
//    GXRELEASE(pDevice);
//    return hr;
//}


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


HRESULT CSkinnedMesh::FindBones(SFrame *pframeCur, SDrawElement *pde)
{
	//	FLOG( "CSkinnedMesh::FindBones(SFrame *pframeCur, SDrawElement *pde)" );
    HRESULT hr = S_OK;
    SMeshContainer *pmcMesh;
    SFrame *pframeChild;
    
    pmcMesh = pframeCur->pmcMesh;
    while (pmcMesh != NULL)
    {
        if (pmcMesh->m_pSkinMesh)
        {
            char** pBoneName = static_cast<char**>(pmcMesh->m_pBoneNamesBuf->GetBufferPointer());
            for (DWORD i = 0; i < pmcMesh->m_pSkinMeshInfo->GetNumBones(); ++i)
            {
                SFrame* pFrame = pde->FindFrame(pBoneName[i]);
                pmcMesh->m_pBoneMatrix[i] = &(pFrame->matCombined);
            }
        }
        pmcMesh = pmcMesh->pmcNext;
    }
    
    pframeChild = pframeCur->pframeFirstChild;
    while (pframeChild != NULL)
    {
        hr = FindBones(pframeChild, pde);
        if (FAILED(hr))
            return hr;
        
        pframeChild = pframeChild->pframeSibling;
    }
    
    return S_OK;
}

HRESULT CSkinnedMesh::LoadAnimation(LPD3DXFILEDATA pxofobjCur, SDrawElement *pde, DWORD options, DWORD fvf, LPDIRECT3DDEVICE9 pD3DDevice, SFrame *pframeParent)
{
	auto hr = S_OK;

    SFrame *pframeCur;
    //LPD3DXFILEDATA pxofobjChild = nullptr;
	LPD3DXFILEDATA pxofChild = nullptr;
    //LPDIRECTXFILEDATAREFERENCE pxofobjChildRef = NULL;
    GUID type;
    DWORD dwSize;
    LPCVOID pData;
    DWORD dwKeyType;
    DWORD cKeys;
    DWORD iKey;
    DWORD cchName;
    char *szFrameName;
    
    pframeCur = new SFrame();
    if (!pframeCur)
    {
        hr = E_OUTOFMEMORY;
        goto e_Exit;
    }
    pframeCur->bAnimationFrame = true;
    
    pframeParent->AddFrame(pframeCur);
    pde->AddAnimationFrame(pframeCur);
    
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

						szFrameName = LPSTR(_alloca(cchName));
						if (!szFrameName)
						{
							hr = E_OUTOFMEMORY;
							goto e_Exit;
						}

						hr = pxofChild->GetName(szFrameName, &cchName);
						if (FAILED(hr)) goto e_Exit;

						pframeCur->pframeToAnimate = pde->FindFrame(szFrameName);
						if (!pframeCur->pframeToAnimate)
						{
							hr = E_INVALIDARG;
							goto e_Exit;
						}
					}

					//GXRELEASE(pxofobjChild);
				}

				//GXRELEASE(pxofobjChildRef);
			}
			else
			{
				// Query the child for it's FileData
				//hr = pxofChild->QueryInterface(IID_ID3DXFileData, reinterpret_cast<LPVOID*>(&pxofobjChild));

				//if (SUCCEEDED(hr))
				//{
					hr = pxofChild->GetType(&type);

					if (FAILED(hr)) goto e_Exit;

					if (TID_D3DRMFrame == type)
					{
						hr = LoadFrames(pxofChild, pde, options, fvf, pD3DDevice, pframeCur);
						if (FAILED(hr)) goto e_Exit;
					}
					else if (TID_D3DRMAnimationOptions == type)
					{
					}
					else if (TID_D3DRMAnimationKey == type)
					{
						hr = pxofChild->Lock(&dwSize, &pData);

						if (FAILED(hr)) goto e_Exit;

						auto data = reinterpret_cast<const DWORD*>(pData);

						dwKeyType = data[0];
						cKeys = data[1];

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

							auto pFileRotateKey = reinterpret_cast<const SRotateKeyXFile*>(data + sizeof(DWORD) * 2);
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

							auto pFileScaleKey = reinterpret_cast<const SScaleKeyXFile*>(data + (sizeof(DWORD) * 2));
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

							auto pFilePosKey = reinterpret_cast<const SPositionKeyXFile*>(data + (sizeof(DWORD) * 2));
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

							auto pFileMatrixKey = reinterpret_cast<const SMatrixKeyXFile*>(data + sizeof(DWORD) * 2);
							for (iKey = 0; iKey < cKeys; iKey++)
							{
								pframeCur->m_pMatrixKeys[iKey].dwTime = pFileMatrixKey->dwTime;
								pframeCur->m_pMatrixKeys[iKey].mat = pFileMatrixKey->mat;
								pFileMatrixKey += 1;
							}
						}
						else
						{
							hr = E_INVALIDARG;
							goto e_UnlockFirst;
						}

					e_UnlockFirst:
						pxofChild->Unlock();
					//}

					//GXRELEASE(pxofobjChild);
					}
			}

			GXRELEASE(pxofChild);
		}
	}
    
e_Exit:
    //GXRELEASE(pxofobjChild);
    GXRELEASE(pxofChild);

    return hr;
}

HRESULT CSkinnedMesh::LoadAnimationSet(LPD3DXFILEDATA pxofobjCur, SDrawElement *pde, DWORD options, DWORD fvf, LPDIRECT3DDEVICE9 pD3DDevice, SFrame *pframeParent)
{
	SFrame *pframeCur;
    GUID type;
    auto hr = S_OK;
    //LPD3DXFILEDATA pxofobjChild = nullptr;
	LPD3DXFILEDATA pxofChild = nullptr;
    DWORD cchName;
    
    pframeCur = new SFrame();
    if (!pframeCur)
    {
        hr = E_OUTOFMEMORY;
        goto e_Exit;
    }
    pframeCur->bAnimationFrame = true;
    
    pframeParent->AddFrame(pframeCur);
    
    hr = pxofobjCur->GetName(nullptr, &cchName);
    
	if (FAILED(hr)) goto e_Exit;
    
    if (cchName > 0)
    {
        pframeCur->szName = new char[cchName];
        if (!pframeCur->szName)
        {
            hr = E_OUTOFMEMORY;
            goto e_Exit;
        }
        
        hr = pxofobjCur->GetName(pframeCur->szName, &cchName);
        if (FAILED(hr)) goto e_Exit;
    }

	// Enumerate child objects.
    // Child object can be data, data reference or binary.
    // Use QueryInterface() to find what type of object a child is.

	SIZE_T children = 0;

	pxofobjCur->GetChildren(&children);

	if (FAILED(hr)) goto e_Exit;
    
	for (SIZE_T child = 0; child < children; child++)
	{
		if (SUCCEEDED(pxofobjCur->GetChild(child, &pxofChild)))
		{
			if (!pxofChild->IsReference())
			{
				// Query the child for it's FileData
				//hr = pxofChild->QueryInterface(IID_ID3DXFileData, reinterpret_cast<LPVOID*>(&pxofobjChild));
				//if (FAILED(hr)) goto e_Exit;

				hr = pxofChild->GetType(&type);
				if (FAILED(hr)) goto e_Exit;

				if (TID_D3DRMAnimation == type)
				{
					hr = LoadAnimation(pxofChild, pde, options, fvf, pD3DDevice, pframeCur);
					if (FAILED(hr)) goto e_Exit;
				}

				//GXRELEASE(pxofobjChild);
			}

			GXRELEASE(pxofChild);
		}
	}
    
e_Exit:
    //GXRELEASE(pxofobjChild);
    GXRELEASE(pxofChild);
    return hr;
}

HRESULT CSkinnedMesh::UpdateFrames(SFrame *pframeCur, const D3DXMATRIX &matCur)
{
    D3DXMatrixMultiply(&pframeCur->matCombined, &pframeCur->matRot, &matCur);
    D3DXMatrixMultiply(&pframeCur->matCombined, &pframeCur->matCombined, &pframeCur->matTrans );
    
    for (auto pframeChild = pframeCur->pframeFirstChild; pframeChild; pframeChild = pframeChild->pframeSibling)
    {
        auto hr = UpdateFrames(pframeChild, pframeCur->matCombined);
        
		if (FAILED(hr)) return hr;
    }

    return S_OK;
}


HRESULT  CSkinnedMesh::DeleteSelectedMesh()
{
	if (m_pdeSelected)
    {
        auto pdeCur = m_pdeHead;
        SDrawElement *pdePrev = nullptr;
        while (pdeCur && pdeCur != m_pdeSelected)
        {
            pdePrev = pdeCur;
            pdeCur = pdeCur->pdeNext;
        }

        if (!pdePrev) m_pdeHead = m_pdeHead->pdeNext;
        else pdePrev->pdeNext = pdeCur->pdeNext;

        m_pdeSelected->pdeNext = nullptr;

        if (m_pdeHead == m_pdeSelected) m_pdeHead = nullptr;

        delete m_pdeSelected;
        m_pdeSelected = nullptr;
    }

    return S_OK;
}

void CSkinnedMesh::ReleaseDeviceDependentMeshes(SFrame* pframe)
{
    for (auto pmcCurr = pframe->pmcMesh; pmcCurr; pmcCurr = pmcCurr->pmcNext)
    {
        if (pmcCurr->m_pSkinMesh)
        {
            GXRELEASE(pmcCurr->pMesh);
			pmcCurr->m_Method = NONE;
		}   
	}

    if (pframe->pframeFirstChild) ReleaseDeviceDependentMeshes(pframe->pframeFirstChild);

    if (pframe->pframeSibling) ReleaseDeviceDependentMeshes(pframe->pframeSibling);
}

// global func

HRESULT CalculateBoundingSphere(SDrawElement *pdeCur, const D3DXVECTOR3& vObjScale)	// 2012-05-23 by isshin 맵툴 개선 오브젝트 스케일 적용
{
	D3DXMATRIXA16 objscale;
	
	D3DXMatrixScaling(&objscale, vObjScale.x, vObjScale.y, vObjScale.z);

    D3DXVECTOR3 vCenter { 0, 0, 0 };
    
	auto fRadiusSq = 0.0f;

	auto hr = CalculateRadius(pdeCur->pframeRoot, &objscale, &vCenter, &fRadiusSq);

    if (SUCCEEDED(hr))
	{
		pdeCur->fRadius = float(sqrt(double(fRadiusSq)));
		pdeCur->vCenter = vCenter;
    }
    
    return hr;
}

HRESULT CalculateSum(SFrame *pframe, D3DXMATRIX *pmatCur, D3DXVECTOR3 *pvCenter, UINT *pcVertices)
{
    auto hr = S_OK;
    LPVOID pbPoints = nullptr;
    UINT cVerticesLocal = 0;
    PBYTE pbCur;
    D3DXVECTOR3 *pvCur;
    D3DXVECTOR3 vTransformedCur;
    UINT iPoint;
    SMeshContainer *pmcCur;
    SFrame *pframeCur;
    UINT cVertices;
    D3DXMATRIXA16 matLocal;
    
    D3DXMatrixMultiply(&matLocal, &pframe->matRot, pmatCur);
    
    pmcCur = pframe->pmcMesh;

    while (pmcCur)
    {
        DWORD fvfsize = D3DXGetFVFVertexSize(pmcCur->pMesh->GetFVF());
        
        cVertices = pmcCur->pMesh->GetNumVertices();
        
        hr = pmcCur->pMesh->LockVertexBuffer(0, &pbPoints);
        
		if (FAILED(hr)) goto e_Exit;
        
        for (iPoint=0, pbCur = PBYTE(pbPoints); iPoint < cVertices; iPoint++, pbCur += fvfsize)
        {
            pvCur = LPD3DXVECTOR3(pbCur);
            
            if (pvCur->x != 0.0 || pvCur->y != 0.0 || pvCur->z != 0.0)
            {
                cVerticesLocal++;
                
                D3DXVec3TransformCoord(&vTransformedCur, pvCur, &matLocal);
                
                pvCenter->x += vTransformedCur.x;
                pvCenter->y += vTransformedCur.y;
                pvCenter->z += vTransformedCur.z;
            }
        }
        
        
        pmcCur->pMesh->UnlockVertexBuffer();
        pbPoints = nullptr;
        
        pmcCur = pmcCur->pmcNext;
    }
    
    *pcVertices += cVerticesLocal;
    
    pframeCur = pframe->pframeFirstChild;
   
	while (pframeCur)
    {
        hr = CalculateSum(pframeCur, &matLocal, pvCenter, pcVertices);
        
		if (FAILED(hr)) goto e_Exit;
        
        pframeCur = pframeCur->pframeSibling;
    }
    
e_Exit:
    if (pbPoints) pmcCur->pMesh->UnlockVertexBuffer();
    
    return hr;
}

HRESULT CalculateRadius(SFrame *pframe, D3DXMATRIX *pmatCur, D3DXVECTOR3 *pvCenter, float *pfRadiusSq)
{
    auto hr = S_OK;
    LPVOID pbPoints = nullptr;
    PBYTE pbCur;
    D3DXVECTOR3 *pvCur;
    D3DXVECTOR3 vDist;
    UINT iPoint;
    UINT cVertices;
    SMeshContainer *pmcCur;
    SFrame *pframeCur;
    float fRadiusLocalSq;
    float fDistSq;
    D3DXMATRIXA16 matLocal;
    
    D3DXMatrixMultiply(&matLocal, &pframe->matRot, pmatCur);
    
    pmcCur = pframe->pmcMesh;
    fRadiusLocalSq = *pfRadiusSq;
    while (pmcCur)
    {
        DWORD fvfsize = D3DXGetFVFVertexSize(pmcCur->pMesh->GetFVF());
        
        cVertices = pmcCur->pMesh->GetNumVertices();
        
        hr = pmcCur->pMesh->LockVertexBuffer(0, &pbPoints);
        if (FAILED(hr)) goto e_Exit;
        
        for (iPoint=0, pbCur = PBYTE(pbPoints); iPoint < cVertices; iPoint++, pbCur += fvfsize)
        {
            pvCur = LPD3DXVECTOR3(pbCur);
            
            if (pvCur->x == 0.0 && pvCur->y == 0.0 && pvCur->z == 0.0) continue;
            
            D3DXVec3TransformCoord(&vDist, pvCur, &matLocal);
            
            vDist -= *pvCenter;
            
            fDistSq = D3DXVec3LengthSq(&vDist);
            
            if (fDistSq > fRadiusLocalSq) fRadiusLocalSq = fDistSq;
        }
        
        
        pmcCur->pMesh->UnlockVertexBuffer();
        pbPoints = nullptr;
        
        pmcCur = pmcCur->pmcNext;
    }
    
    *pfRadiusSq = fRadiusLocalSq;
    
    pframeCur = pframe->pframeFirstChild;
    while (pframeCur)
    {
        hr = CalculateRadius(pframeCur, &matLocal, pvCenter, pfRadiusSq);
        if (FAILED(hr)) goto e_Exit;
        
        pframeCur = pframeCur->pframeSibling;
    }
    
e_Exit:
    if (pbPoints) pmcCur->pMesh->UnlockVertexBuffer();
    
    return hr;
}

