// EffectRender.cpp: implementation of the CEffectRender class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AtumApplication.h"
#include "EffectRender.h"
#include "ObjectChild.h"
#include "GameDataLast.h"
#include "SpriteAniData.h"
#include "ParticleSystem.h"
#include "ObjectAniData.h"
#include "TraceAni.h"
#include "EnemyData.h"
#include "MonsterData.h"
#include "SkinnedMesh.h"
#include "SceneData.h"
#include "Camera.h"
#include "Background.h"
#include "Weapon.h"
#include "ShuttleChild.h"
#include "CharacterChild.h" // 2005-07-21 by ispark

#include "ItemData.h"
#include "dxutil.h"
#include "SunData.h"
#include "D3DHanFont.h"
#include "ObjRender.h"
#include "SkillEffect.h"
#include "TutorialSystem.h"
#include "Frustum.h"
#include "INFGameMain.h" // 2008-08-22 by bhsohn EP3 인벤토리 처리

#include "MeshInitThread.h" // 2009. 11. 23 by jskim 리소스 로딩 구조 변경


extern LPDIRECT3DDEVICE9 g_pD3dDev;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEffectRender::CEffectRender()
{
	FLOG("CEffectRender()");
	m_pVB1 = NULL;
	memset(m_pVB2, 0x00, 4 * 2);
	memset(m_pVB4, 0x00, 4 * 4);
	memset(m_pVB8, 0x00, 4 * 8);
	memset(m_pVB16, 0x00, 4 * 16);

	m_pTexEffectData = NULL;
	memset(m_pTexture, 0x00, TEX_EFFECT_NUM * sizeof(DWORD));
	memset(m_nTextureRenderCount, 0x00, TEX_EFFECT_NUM * sizeof(int));
	//	memset(m_pObjEffectMesh, 0x00, OBJ_EFFECT_NUM*sizeof(DWORD));
	m_fTextureCheckTime = 300.0f;// 5분에 한번씩 이펙트 사용여부 검사

	m_pEffectData = NULL;
	m_pObjectData = NULL;


	m_bZBufferTemp = FALSE;
	m_nParticleEffectCountPerSecond = 0;
	m_nSpriteEffectCountPerSecond = 0;
	m_nObjectEffectCountPerSecond = 0;
	m_nTraceEffectCountPerSecond = 0;

	DBGOUT_EFFECT("-------------------- Effect wear item postion --------------------\n");
	DBGOUT_EFFECT("1-1Type Weapon		:	0\n");
	DBGOUT_EFFECT("1-2Type Weapon		:	1\n");
	DBGOUT_EFFECT("2-1Type Weapon		:	2\n");
	DBGOUT_EFFECT("2-2Type Weapon		:	3\n");
	DBGOUT_EFFECT("Front(Rader)			:	4\n");
	DBGOUT_EFFECT("Center(Armor)		:	5\n");
	DBGOUT_EFFECT("Option(ATTACH)		:	6\n");
	DBGOUT_EFFECT("Back(Engine)			:	7\n");
	DBGOUT_EFFECT("-------------------- wear item postion --------------------\n");

	// 2007-11-08 by bhsohn 인벤 이펙트 관련 처리
	// 인벤 파티클 초기화
	m_vecInvenParticleInfo.clear();
	m_vecEnemyItemParticleInfo.clear(); // 2012-07-13 by isshin 아템미리보기 인첸트이펙스 적용
}

CEffectRender::~CEffectRender()
{
	FLOG("~CEffectRender()");
	SAFE_RELEASE(m_pVB1);
	int i;
	for (i = 0; i < 2; i++)
	SAFE_RELEASE(m_pVB2[i]);
	for (i = 0; i < 4; i++)
	SAFE_RELEASE(m_pVB4[i]);
	for (i = 0; i < 8; i++)
	SAFE_RELEASE(m_pVB8[i]);
	for (i = 0; i < 16; i++)
	SAFE_RELEASE(m_pVB16[i]);

	util::del(m_pTexEffectData);
	util::del(m_pEffectData);
	util::del(m_pObjectData);
	for (i = 0; i < TEX_EFFECT_NUM; i++)
	SAFE_RELEASE(m_pTexture[i]);

	for (auto tex : m_TextureManager.m_mapTextures)
	SAFE_RELEASE(tex.second);

	//	for(i=0;i<OBJ_EFFECT_NUM;i++)
	//		util::del(m_pObjEffectMesh[i]);
	m_vecEnemyItemParticleInfo.clear(); // 2012-07-13 by isshin 아템미리보기 인첸트이펙스 적용
}

CSkinnedMesh* CEffectRender::LoadObject(char* strName, int LoadingPriority)
{
	FLOG("CEffectRender::LoadObject(char* strName)");

	if (*strName == '\0') return nullptr;

	// 2015-10-22 PanKJ expensive computations in next line, should replace string maps with integer maps
	auto it = m_mapObjNameToMesh.find(strName);

	if (it == m_mapObjNameToMesh.end())
	{
		m_vecLoadObj.push_back(strName);

		m_vecLoadingPriority.emplace_back(LoadingPriority, strName);
	}
	else return it->second;

	return nullptr;
}

void CEffectRender::LoadObjectToMap(char* strName)
{
	FLOG("CEffectRender::LoadObject(char* strName)");

	if (*strName == '\0') return;

	auto obj = m_mapObjNameToMesh.find(strName);

	if (obj == m_mapObjNameToMesh.end())
	{
		int tempPriority = _NOTHING_STEP;

		for (auto& info : m_vecLoadingPriority)
		{
			if (!strcmp(info.chEffectName, strName))
			{
				tempPriority = info.LoadingPriority;
				break;
			}
		}

		CSkinnedMesh* pMesh = new CSkinnedMesh(TRUE);
		pMesh->InitDeviceObjects();
		pMesh->m_nRenderCount = 2;
		m_mapObjNameToMesh[strName] = pMesh; // 2015-10-21 PanKJ consider changing this map to use integer indexing instead of strings

		if (g_pD3dApp->m_dwGameState != _GAME || tempPriority == _NOTHING_STEP)
		{
			char strPath[MAX_PATH];
			CGameData* pMeshData = new CGameData();
			g_pD3dApp->LoadPath(strPath, IDS_DIRECTORY_EFFECT, strName);
			if (pMeshData->SetFile(strPath, FALSE, NULL, 0))
			{
#ifdef _DEBUG
				// 보안 코드
				char buf[64];
				memset( buf, 0x00, sizeof(buf) );
				strncpy( buf, strName, 8 );
				if(pMeshData->Find(buf) == NULL)
				{
					util::del(pMeshData);
					g_pD3dApp->NetworkErrorMsgBox(STRERR_C_RESOURCE_0001);
					DBGOUT("Resource File Error(%s)\n",strName);
					return ;
				}
#endif
				pMesh->LoadMeshHierarchyFromMem(pMeshData);
				util::del(pMeshData);
				return;
			}
			else
			{
				util::del(pMeshData);
				DBGOUT("Can't Find Effect OBJECT File.(%s)\n", strName);
				return;
			}
		}
		
		
		auto LoadingGameInfo = new structLoadingGameInfo;
		strcpy(LoadingGameInfo->MeshName, strName);
		LoadingGameInfo->MeshType = _EFFECT_TYPE;
		LoadingGameInfo->pSkinnedMesh = pMesh;
		LoadingGameInfo->LoadingPriority = tempPriority;
		EnterCriticalSection(&g_pD3dApp->m_cs);
		g_pD3dApp->m_pMeshInitThread->QuePushGameData(LoadingGameInfo);
		LeaveCriticalSection(&g_pD3dApp->m_cs);
		
		//end 2009. 11. 23 by jskim 리소스 로딩 구조 변경
	}
}

int CEffectRender::GetEmptyTextureIndex()
{
	FLOG("CEffectRender::GetEmptyTextureIndex()");
	int index = 0;
	for (int i = 0; i < TEX_EFFECT_NUM; i++)
	{
		if (!m_pTexture[i])
			return i;
	}
	return -1;
}

DataHeader* CEffectRender::FindEffectInfo(char* strName)
{
	if (!m_pEffectData || *strName == '\0') return nullptr;

	unsigned number = 0;
	for (const char* ptr = strName; *ptr != '\0'; ptr++)
	{
		if (*ptr >= '0' && *ptr <= '9') number = number * 10 + (*ptr - '0');
		else break;
	}

	return m_pEffectData->find_by_number(number);
}

DataHeader* CEffectRender::FindObjectInfo(char* strName)
{
	FLOG("CEffectRender::FindObjectInfo(char* strName)");

	if (m_pObjectData == nullptr) return nullptr;

	DataHeader* pDataHeader = m_pObjectData->GetStartPosition();

	while (pDataHeader)
	{
		// 8글자만 비교한다. 확장자는 제외시킨다.
		if (strncmp(strName, pDataHeader->m_FileName, 8) == 0) return pDataHeader;

		pDataHeader = m_pObjectData->GetNext();
	}

	return nullptr;
}

int CEffectRender::LoadTexture(char* strName)
{
	FLOG("CEffectRender::LoadTexture(char* strName)");

	if (m_pTexEffectData == nullptr || *strName == '\0') return -1;

	auto it = m_mapTexNameToIndex.find(strName);

	if (it == m_mapTexNameToIndex.end())
	{
		DataHeader* pDataHeader = m_pTexEffectData->GetStartPosition();
		while (pDataHeader)
		{
			if (strncmp(strName, pDataHeader->m_FileName, 8) == 0)
			{
				int index = GetEmptyTextureIndex();
				if (index < 0)
				{
					DBGOUT("ERROR, CEffectRender::LoadTexture, GetEmptyTextureIndex() < 0\n");
					return -1;
				}
				// load texture from memory
				HRESULT hr = D3DXCreateTextureFromFileInMemory(g_pD3dDev, pDataHeader->m_pData, pDataHeader->m_DataSize, &m_pTexture[index]);
				if (hr != D3D_OK)
				{
					m_pTexture[index] = NULL;
					DBGOUT("ERROR, CEffectRender::LoadTexture, D3DXCreateTextureFromFileInMemory() != D3D_OK\n");
					return -1;
				}
				m_nTextureRenderCount[index] = 2;
				m_mapTexNameToIndex[strName] = index;

				return index;
			}
			pDataHeader = m_pTexEffectData->GetNext();
		}
	}
	else return it->second;

	DBGOUT("ERROR, CEffectRender::LoadTexture (%s)\n", strName);

	return -1;
}

void CEffectRender::Tick(float fElapsedTime)
{
	FLOG("CEffectRender::Tick(float fElapsedTime)");
	vector<string>::iterator it = m_vecLoadObj.begin();
	while (it != m_vecLoadObj.end())
	{
		LoadObjectToMap((char*)(*it).c_str());
		it++;
	}
	m_vecLoadObj.clear();
	// texture delete : 60초에 한번씩
	m_fTextureCheckTime -= fElapsedTime;
	if (m_fTextureCheckTime > 0.0f)
		return;
	// 2009. 11. 23 by jskim 리소스 로딩 구조 변경
	//m_fTextureCheckTime = 300.0f;
	if (g_pD3dApp->IsEmptyLoadingGameDataList())
	{
		m_fTextureCheckTime = 300.0f;
	}
	else
	{
		m_fTextureCheckTime = 30.0f;
		return;
	}
	//end 2009. 11. 23 by jskim 리소스 로딩 구조 변경

	//for (auto it = m_mapTextures.begin(); it != m_mapTextures.end(); it++)
	//{
	//	SAFE_RELEASE(it->second.Get());
	//	if (it->second.Usage() < 1.0f) m_mapTextures.erase(it);
	//}
	m_TextureManager.update();
	for (int i = 0; i < TEX_EFFECT_NUM; i++)
	{
		if (m_nTextureRenderCount[i] == 0)
			continue;
		//		m_nTextureRenderCount[i]--;
		if (m_nTextureRenderCount[i] == 2)// if(m_nTextureRenderCount[i] == 1)
		{
			map<string, int>::iterator it = m_mapTexNameToIndex.begin();
			while (it != m_mapTexNameToIndex.end())
			{
				if (it->second == i)
				{
					m_mapTexNameToIndex.erase(it);
					break;
				}
				it++;
			}
			m_nTextureRenderCount[i] = 0;
			SAFE_RELEASE(m_pTexture[i]);
		}
		else if (m_nTextureRenderCount[i] > 2) m_nTextureRenderCount[i]--;
		else// if(m_nTextureRenderCount[i] >= 2)
		{
			m_nTextureRenderCount[i] = 2;
		}
	}
	/*	for(i=0;i<OBJ_EFFECT_NUM;i++)
		{
		if(m_pObjEffectMesh[i] && m_pObjEffectMesh[i]->m_nRenderCount)
		{
		if(m_pObjEffectMesh[i]->m_nRenderCount == 2)// if(m_nObjRenderCount[i] == 1)
		{
		map<string,int>::iterator it = m_mapObjNameToIndex.begin();
		while(it != m_mapObjNameToIndex.end())
		{
		if(it->second == i)
		{
		m_mapObjNameToIndex.erase(it);
		break;
		}
		it++;
		}
		m_pObjEffectMesh[i]->InvalidateDeviceObjects();
		m_pObjEffectMesh[i]->DeleteDeviceObjects();
		util::del(m_pObjEffectMesh[i]);
		}
		else// if(m_nObjRenderCount[i] >= 2)
		{
		m_pObjEffectMesh[i]->m_nRenderCount = 2;
		}
		}
		}*/
	map<string, CSkinnedMesh*>::iterator itMesh = m_mapObjNameToMesh.begin();
	while (itMesh != m_mapObjNameToMesh.end())
	{
		if (itMesh->second->m_nRenderCount == 2)
		{
			// 2009. 11. 23 by jskim 리소스 로딩 구조 변경
			itMesh->second->DeleteLoadingGameData();
			//end 2009. 11. 23 by jskim 리소스 로딩 구조 변경
			itMesh->second->InvalidateDeviceObjects();
			itMesh->second->DeleteDeviceObjects(); // 2016-01-08 exception at this point (3)
			util::del(itMesh->second);
			itMesh = m_mapObjNameToMesh.erase(itMesh);
			continue;
		}
		itMesh++;
	}
}

void CEffectRender::DevideCharacterEffect(CCharacterInfo* pChar, BOOL bAlpha, int nAlphaValue)
{
	FLOG("CEffectRender::DevideCharacterEffect(CCharacterInfo* pChar)");

	int nParticleEffectCountPerSecond = 0,
		nSpriteEffectCountPerSecond = 0,
		nObjectEffectCountPerSecond = 0,
		nTraceEffectCountPerSecond = 0;

	for (auto current : pChar->m_vecCurrentBodyCondition)
	{
		auto condition = pChar->m_mapBodyCondition.find(current);

		if (condition == pChar->m_mapBodyCondition.end()) continue;

		for (auto pEffectInfo : condition->second->m_vecEffect)
		{
			if (pEffectInfo->m_pEffect == nullptr || pEffectInfo->m_fCurrentTime > 0) continue;

			// effect rendering
			pEffectInfo->m_nAlphaValue = nAlphaValue;

			switch (pEffectInfo->m_nEffectType)
			{
			case EFFECT_TYPE_OBJECT:
				DivideCharacterEffectObject((CObjectAni*)pEffectInfo->m_pEffect, bAlpha, nAlphaValue);
				nObjectEffectCountPerSecond++;
				break;

			case EFFECT_TYPE_SPRITE:
				DivideCharacterEffectSprite((CSpriteAni*)pEffectInfo->m_pEffect, bAlpha, nAlphaValue);
				nSpriteEffectCountPerSecond++;
				break;

			case EFFECT_TYPE_PARTICLE:
				DivideCharacterEffectParticle((CParticleSystem*)pEffectInfo->m_pEffect, bAlpha, nAlphaValue);
				nParticleEffectCountPerSecond += ((CParticleSystem*)pEffectInfo->m_pEffect)->m_vecParticle.size();
				break;

			case EFFECT_TYPE_TRACE:
				DivideCharacterEffectTrace((CTraceAni*)pEffectInfo->m_pEffect, bAlpha, nAlphaValue);
				nTraceEffectCountPerSecond++;
				break;
			}
		}
	}

	m_nParticleEffectCountPerSecond += nParticleEffectCountPerSecond;
	m_nSpriteEffectCountPerSecond += nSpriteEffectCountPerSecond;
	m_nObjectEffectCountPerSecond += nObjectEffectCountPerSecond;
	m_nTraceEffectCountPerSecond += nTraceEffectCountPerSecond;
}

void CEffectRender::DivideCharacterEffectObject(CObjectAni* effect, BOOL bAlpha, int nAlphaValue)
{
	if (g_pD3dApp->m_dwGameState == _GAME && !g_pFrustum->CheckSphere(
		effect->m_pParent->m_pParent->m_mMatrix._41 - effect->m_pParent->m_vPos.z,
		effect->m_pParent->m_pParent->m_mMatrix._42 + effect->m_pParent->m_vPos.y,
		effect->m_pParent->m_pParent->m_mMatrix._43 + effect->m_pParent->m_vPos.x,
		effect->m_fRadius))
		return;

	if (effect->m_bAlphaBlending == FALSE) ObjectAniRender(effect, bAlpha, nAlphaValue); // 2016-01-08 exception at this point

	else if (effect->m_bZbufferEnable) PushBackToZEnableEffect(effect);
}

void CEffectRender::DivideCharacterEffectSprite(CSpriteAni* effect, BOOL bAlpha, int nAlphaValue)
{
	if (g_pD3dApp->m_dwGameState == _GAME && !g_pFrustum->CheckSphere(
		effect->m_pParent->m_pParent->m_mMatrix._41 - effect->m_pParent->m_vPos.z,
		effect->m_pParent->m_pParent->m_mMatrix._42 + effect->m_pParent->m_vPos.y,
		effect->m_pParent->m_pParent->m_mMatrix._43 + effect->m_pParent->m_vPos.x,
		effect->m_fTextureSize))
		return;

	if (effect->m_bZbufferEnable) PushBackToZEnableEffect(effect);
}

void CEffectRender::DivideCharacterEffectParticle(CParticleSystem* effect, BOOL bAlpha, int nAlphaValue)
{
	if (g_pD3dApp->m_dwGameState == _GAME && !g_pFrustum->CheckSphere(
		effect->m_pParent->m_pParent->m_mMatrix._41 - effect->m_pParent->m_vPos.z,
		effect->m_pParent->m_pParent->m_mMatrix._42 + effect->m_pParent->m_vPos.y,
		effect->m_pParent->m_pParent->m_mMatrix._43 + effect->m_pParent->m_vPos.x,
		effect->m_fRadius))
		return;

	if (effect->m_bZbufferEnable)
	{
		int nInvenIdx = effect->m_pParent->m_nInvenWeaponIndex;

		if (g_pD3dApp->m_bCharacter == FALSE || nInvenIdx == 0)
		{
			switch (effect->m_nParticleType)
			{
			case PARTICLE_OBJECT_TYPE: // panoskj 2016-01-03 Disabled
				m_vectorObjectParticles.push_back(effect);
				break;
			case PARTICLE_SPRITE_TYPE:
				{
					LPDIRECT3DTEXTURE9 texture = nullptr;

					if (effect->m_pParent != nullptr && effect->m_pParent->m_pTexture != nullptr)

						texture = effect->m_pParent->m_pTexture;

					else texture = LoadTextureNew(effect->m_strTextureName[0]);

					if (texture == nullptr) break;

					auto range = m_mapSpriteRenderInfo.equal_range(texture);

					bool found = false;

					for (auto info = range.first; info != range.second; info++)
						if (info->second.matches(effect))
						{
							info->second.add(effect);
							found = true;
							break;
						}

					if (!found) m_mapSpriteRenderInfo.emplace(texture, SpriteRenderInfo{effect});
				}

				/*
				for (unsigned i = 0; i < effect->m_vecParticle.size(); i++)
				{
					LPDIRECT3DTEXTURE9 texture = nullptr;

					if (effect->m_pParent && effect->m_pParent->m_pTexture)
						
						texture = effect->m_pParent->m_pTexture;

					else texture = LoadTextureNew(effect->m_strTextureName[((CParticle*)effect->m_vecParticle[i])->m_nTextureType]);

					if (texture == nullptr) continue;

					auto range = m_mapSpriteRenderInfo.equal_range(texture);

					bool found = false;

					for (auto info = range.first; info != range.second; info++) if (info->second.matches(effect))
					{
						info->second.vecInstances.push_back(effect->m_vecParticleInstances[i]);
						found = true;
						break;
					}

					if (!found)
					{
						auto info = SpriteRenderInfo { effect->m_bZWriteEnable, effect->m_bZbufferEnable, effect->m_dwSrcBlend, effect->m_dwDestBlend };
						info.vecInstances.push_back(effect->m_vecParticleInstances[i]);
						m_mapSpriteRenderInfo.emplace(texture, info);
					}
				}
				*/
				break;
			}
		}

		for (auto pEffectParticle : effect->m_vecParticle)
		{
			//if (g_pD3dApp->m_bCharacter == FALSE || nInvenIdx == 0)
			//{
			//	m_vecZEnableEffect.push_back(pEffectParticle);

			//	auto ptype = effect->m_nParticleType;

			//	if (ptype == PARTICLE_OBJECT_TYPE) m_vecZEnableEffectParticleObject.push_back(pEffectParticle);

			//	else if (ptype == PARTICLE_SPRITE_TYPE) m_vecZEnableEffectParticleSprite.push_back(pEffectParticle);
			//}

			if (nInvenIdx > 0 && g_pGameMain->IsEquipInvenShow() == TRUE)

				AddInvenPaticleName(nInvenIdx - 1, effect->m_strName);
		}
	}
}

void CEffectRender::DivideCharacterEffectTrace(CTraceAni* effect, BOOL bAlpha, int nAlphaValue)
{
	if (g_pD3dApp->m_dwGameState == _GAME && !g_pFrustum->CheckSphere(
		effect->m_pParent->m_pParent->m_mMatrix._41 - effect->m_pParent->m_vPos.z,
		effect->m_pParent->m_pParent->m_mMatrix._42 + effect->m_pParent->m_vPos.y,
		effect->m_pParent->m_pParent->m_mMatrix._43 + effect->m_pParent->m_vPos.x,
		effect->m_fRadius))
		return;

	if (effect->m_bZbufferEnable)
	{
		if (effect->m_nCurrentNumberOfTrace <= 0) return;

		auto texture = LoadTextureNew(effect->m_strTextureName[effect->m_nCurrentTextureNumber]);

		if (texture != nullptr && effect->m_pParent && effect->m_pParent->m_pTexture)

			texture = effect->m_pParent->m_pTexture;

		if (texture == nullptr) return;

		auto range = m_mapTraceRenderInfo.equal_range(texture);

		for (auto i = range.first; i != range.second; ++i)

			if (i->second.matches(effect))
			{
				i->second.add(effect);
				return;
			}

		m_mapTraceRenderInfo.emplace(texture, TraceRenderInfo{effect});
	}
}

void CEffectRender::DevideZBufferEnableEffect()
{
	FLOG("CEffectRender::DevideZBufferEnableEffect()");
	CCharacterInfo* pChar;
	m_nParticleEffectCountPerSecond = 0;
	m_nSpriteEffectCountPerSecond = 0;
	m_nObjectEffectCountPerSecond = 0;
	m_nTraceEffectCountPerSecond = 0;

	// Object - Normal
	//	if(g_pD3dApp->m_bDegree == 2) // 고사양
	//	{
	//		vectorCObjectChildPtr::iterator itObj(g_pScene->m_vectorRangeObjectPtrList.begin());
	//		while(itObj != g_pScene->m_vectorRangeObjectPtrList.end())
	//		{
	//			CObjectChild * pObj = *itObj;
	//			pChar = pObj->m_pCharacterInfo;
	//			if(pChar)
	//			{
	//				DevideCharacterEffect(pChar);
	//			}
	//	//		m_bZBufferTemp = FALSE;
	//			itObj++;
	//		}
	//	}
	//	else
	//	{
	float fDistanceToCamera;
	float fDistance;

	for (auto pObj : g_pScene->m_vectorCulledObjectPtrList)
	{
		pChar = pObj->m_pCharacterInfo;

		fDistanceToCamera = D3DXVec3Length(&(pObj->m_vPos - g_pD3dApp->m_pCamera->GetEyePt()));

		if (g_pTutorial->IsTutorialMode() == TRUE && pObj->m_nCode == TUTORIAL_GATE && !g_pTutorial->IsEnableTutorialGate(pObj->m_vPos))

			int i = 0;

		else if (pObj->IsShowNode())
		{
			fDistance = g_pScene->m_fFogEndValue + pObj->m_pObjMesh->m_fRadius;

			if (fDistanceToCamera < fDistance && pChar != nullptr) DevideCharacterEffect(pChar);
		}
	}

	if (g_pGround != nullptr && g_pGround->m_pObjectEvent != nullptr)
	{
		CObjectChild* pObjEvent = (CObjectChild *)g_pGround->m_pObjectEvent->m_pChild;

		while (pObjEvent)
		{
			pChar = pObjEvent->m_pCharacterInfo;

			if (pChar)
			{
				if (g_pTutorial->IsTutorialMode() == TRUE && pObjEvent->m_nCode == TUTORIAL_GATE && !g_pTutorial->IsEnableTutorialGate(pObjEvent->m_vPos))

					int i = 0;

				else if (pObjEvent->IsShowNode()) DevideCharacterEffect(pChar);
			}

			pObjEvent = (CObjectChild *)pObjEvent->m_pNext;
		}
	}

	if (g_pD3dApp->m_bCharacter == FALSE)
	{
		if (g_pShuttleChild != nullptr)
		{
			pChar = g_pD3dApp->m_pShuttleChild->m_pCharacterInfo;

			if (pChar && g_pD3dApp->m_pCamera->GetCamType() == CAMERA_TYPE_NORMAL)
			{
				if (g_pShuttleChild->m_nAlphaValue < SKILL_OBJECT_ALPHA_NONE)
				{
					ALPHA_CHARACTERINFO stAlphaInfo;
					stAlphaInfo.pCharInfo = pChar;
					stAlphaInfo.nAlphaValue = g_pShuttleChild->m_nAlphaValue;
					g_pScene->m_vecAlphaEffectRender.push_back(stAlphaInfo);
				}
				else DevideCharacterEffect(pChar);
			}

			for (auto& info : g_pShuttleChild->m_pSkillEffect->m_vecSkillEffect)

				if (info.pCharacterInfo != nullptr) DevideCharacterEffect(info.pCharacterInfo);

			for (auto* pItem : g_pShuttleChild->m_pChaffData)

				if (pItem->m_pCharacterInfo != nullptr) DevideCharacterEffect(pItem->m_pCharacterInfo);

			for (auto pItem : g_pScene->m_vecScanData)

				if (pItem->m_pCharacterInfo != nullptr) DevideCharacterEffect(pItem->m_pCharacterInfo);
		}
	}
	else
	{
		if (g_pCharacterChild != nullptr && g_pCharacterChild->GetbPickMove())
		{
			pChar = g_pCharacterChild->m_pPickingInfo;

			if (pChar) DevideCharacterEffect(pChar);
		}
	}

	for (auto pEnemy : g_pD3dApp->m_pScene->m_vecEnemyRenderList)
	{
		if (pEnemy->m_bRender == FALSE) continue;

		if (pEnemy->m_bEnemyCharacter == FALSE)
		{
			pChar = pEnemy->m_pCharacterInfo;

			if (pChar)
			{
				if (pEnemy->m_nAlphaValue < SKILL_OBJECT_ALPHA_NONE)
				{
					ALPHA_CHARACTERINFO stAlphaInfo;
					stAlphaInfo.pCharInfo = pChar;
					stAlphaInfo.nAlphaValue = pEnemy->m_nAlphaValue;
					g_pScene->m_vecAlphaEffectRender.push_back(stAlphaInfo);
				}
				else DevideCharacterEffect(pChar);
			}

			for (auto& effectinfo : pEnemy->m_pSkillEffect->m_vecSkillEffect)
			{
				if (effectinfo.pCharacterInfo)
				{
					if (pEnemy->m_nAlphaValue < SKILL_OBJECT_ALPHA_NONE)
					{
						ALPHA_CHARACTERINFO stAlphaInfo;
						stAlphaInfo.pCharInfo = effectinfo.pCharacterInfo;
						stAlphaInfo.nAlphaValue = pEnemy->m_nAlphaValue;
						g_pScene->m_vecAlphaEffectRender.push_back(stAlphaInfo);
					}
					else
#if C_USER_EFFECT_CONTROL
						if (g_pSOption->sTerrainEffectRender > 0)
#endif
						DevideCharacterEffect(effectinfo.pCharacterInfo);
				}
			}

			for (auto pItem : pEnemy->m_pChaffData)
			{
				if (pItem->m_pCharacterInfo)
				{
					// 2006-11-16 by ispark, 알파 체크
					if (pEnemy->m_nAlphaValue < SKILL_OBJECT_ALPHA_NONE)
					{
						ALPHA_CHARACTERINFO stAlphaInfo;
						stAlphaInfo.pCharInfo = pChar;
						stAlphaInfo.nAlphaValue = pEnemy->m_nAlphaValue;
						g_pScene->m_vecAlphaEffectRender.push_back(stAlphaInfo);
					}
					else
#if C_USER_EFFECT_CONTROL
						if (g_pSOption->sTerrainEffectRender > 0)
#endif
						DevideCharacterEffect(pItem->m_pCharacterInfo);
				}
			}
		}
	}

	for (auto pMonster : g_pD3dApp->m_pScene->m_vecMonsterRenderList)
	{
		pChar = pMonster->m_pCharacterInfo;

		if (pChar) DevideCharacterEffect(pChar);

		for (auto effectinfo : pMonster->m_pSkillEffect->m_vecSkillEffect)

			if (effectinfo.pCharacterInfo) DevideCharacterEffect(effectinfo.pCharacterInfo);
	}

	if (g_pD3dApp->m_pScene->m_pWeaponData != nullptr)
	{
		for (auto pWeapon = (CWeapon*)g_pD3dApp->m_pScene->m_pWeaponData->m_pChild; pWeapon != nullptr; pWeapon = (CWeapon*)pWeapon->m_pNext)
		{
			pChar = pWeapon->m_pCharacterInfo;

			if (pChar != nullptr)
			{
				if (g_pShuttleChild->GetSkillMissileWarning() && pWeapon->m_pTarget == g_pShuttleChild &&
					pWeapon->m_pItemData != nullptr && IS_SECONDARY_WEAPON(pWeapon->m_pItemData->Kind));
				else DevideCharacterEffect(pChar);
			}
		}
	}

	if (g_pD3dApp->m_pScene->m_pItemData != nullptr)
	{
		for (auto pItem = (CItemData*)g_pD3dApp->m_pScene->m_pItemData->m_pChild; pItem != nullptr; pItem = (CItemData *)pItem->m_pNext)
		{
			pChar = pItem->m_pCharacterInfo;

			if (pChar && pItem->m_bIsRender) DevideCharacterEffect(pChar);
		}
	}

	if ((g_pD3dApp->m_dwGameState == _GAME || g_pD3dApp->m_dwGameState == _SHOP) &&
		g_pScene->m_byWeatherType != WEATHER_FOGGY && g_pScene->m_byWeatherType != WEATHER_RAINY && g_pScene->m_byWeatherType != WEATHER_SNOWY &&
		g_pScene->m_pSunData && g_pScene->m_pSunData->m_pCharacterInfo && IsSunRenderEnable(g_pShuttleChild->m_myShuttleInfo.MapChannelIndex.MapIndex))
	{
		pChar = g_pD3dApp->m_pScene->m_pSunData->m_pCharacterInfo;
		if (pChar != nullptr) DevideCharacterEffect(pChar);
	}

	if (g_pD3dApp->m_pEffectList != nullptr)
	{
		for (auto pEffect = (CAppEffectData*)g_pD3dApp->m_pEffectList->m_pChild; pEffect != nullptr; pEffect = (CAppEffectData*)pEffect->m_pNext)
		{
			if (pEffect->m_bRender == FALSE) continue;

			bool bEffectRender = false;

			if (pEffect->m_nType / 1000000 == 7 ||
				pEffect->m_nType / 10000000 == 1 ||
				pEffect->m_nType / 100000 == 1 ||
				pEffect->m_nType / 1000000 == 4)

				bEffectRender = true;

			pChar = pEffect->m_pCharacterInfo;

			if (pChar != nullptr)
			{
				if (bEffectRender && pEffect->m_pParent != nullptr && (pEffect->m_pParent->m_dwPartType == _ENEMY || pEffect->m_pParent->m_dwPartType == _ADMIN))
				{
					if (((CEnemyData*)pEffect->m_pParent)->m_bEnemyCharacter == FALSE)
					{
						if (CheckAlphaRender(pEffect, pEffect->m_pParent->m_dwPartType))
						{
							ALPHA_CHARACTERINFO stAlphaInfo;
							stAlphaInfo.pCharInfo = pChar;
							stAlphaInfo.nAlphaValue = ((CEnemyData *)pEffect->m_pParent)->m_nAlphaValue;
							g_pScene->m_vecAlphaEffectRender.push_back(stAlphaInfo);
						}
						else if (((CEnemyData *)pEffect->m_pParent)->m_bIsRender &&
							(((CAtumData*)pEffect->m_pParent)->m_bDegree != 0 ||
								pEffect->m_nType / 100000 == 78) || bEffectRender)
						{
#if C_USER_EFFECT_CONTROL
							if ((g_pSOption->sTerrainEffectRender <= 1) && (((CEnemyData *)pEffect->m_pParent)->m_pWingIn))

								((CEnemyData *)pEffect->m_pParent)->m_pWingIn->m_bRender = FALSE;

							else if (((CEnemyData *)pEffect->m_pParent)->m_pWingIn)

								((CEnemyData *)pEffect->m_pParent)->m_pWingIn->m_bRender = TRUE;

							if (g_pSOption->sTerrainEffectRender > 1)
#endif
							DevideCharacterEffect(pChar);
						}
					}
					else
					{
						// 2006-06-29 by ispark, 악세사리 경우는 캐릭터 상태에서도 렌더링한다.
						if ((((CEnemyData *)pEffect->m_pParent)->m_pContainer &&
								((CEnemyData *)pEffect->m_pParent)->m_pContainer->m_pCharacterInfo == pChar) ||
							(((CEnemyData *)pEffect->m_pParent)->m_pAccessories &&
								((CEnemyData *)pEffect->m_pParent)->m_pAccessories->m_pCharacterInfo == pChar) ||
							(((CEnemyData *)pEffect->m_pParent)->m_pWingIn &&
								((CEnemyData *)pEffect->m_pParent)->m_pWingIn->m_pCharacterInfo == pChar) ||
							// 2011. 01. 13 by jskim, 인비지블 상태에서 펫이 보이는 버그 수정
							(((CEnemyData *)pEffect->m_pParent)->m_pPartner &&
								((CEnemyData *)pEffect->m_pParent)->m_pPartner->m_pCharacterInfo == pChar) ||
							(((CEnemyData *)pEffect->m_pParent)->m_pPartner1 &&
								((CEnemyData *)pEffect->m_pParent)->m_pPartner1->m_pCharacterInfo == pChar))
						// end 2011. 01. 13 by jskim, 인비지블 상태에서 펫이 보이는 버그 수정							
						{
							if (((CEnemyData *)pEffect->m_pParent)->m_bIsRender)
							{
								DevideCharacterEffect(pChar);
							}
						}
					}
				}
				else
				{
					if (pEffect->m_pParent && CheckAlphaRender(pEffect, _SHUTTLE))
					{
						ALPHA_CHARACTERINFO stAlphaInfo;
						stAlphaInfo.pCharInfo = pChar;
						stAlphaInfo.nAlphaValue = g_pShuttleChild->m_nAlphaValue;
						g_pScene->m_vecAlphaEffectRender.push_back(stAlphaInfo);
					}
					else if (g_pD3dApp->m_pCamera->GetCamType() == CAMERA_TYPE_FPS && g_pShuttleChild == pEffect->m_pParent)
					{
#if C_USER_EFFECT_CONTROL
						if ((g_pShuttleChild->m_pWeapon1_1_1 &&
							pChar == g_pShuttleChild->m_pWeapon1_1_1->m_pCharacterInfo) ||
							(g_pShuttleChild->m_pWeapon1_1_2 &&
							pChar == g_pShuttleChild->m_pWeapon1_1_2->m_pCharacterInfo) ||
							(g_pShuttleChild->m_pWeapon2_1_1 &&
							pChar == g_pShuttleChild->m_pWeapon2_1_1->m_pCharacterInfo) ||
							(g_pShuttleChild->m_pWeapon2_1_2 &&
							pChar == g_pShuttleChild->m_pWeapon2_1_2->m_pCharacterInfo) ||
							(g_pShuttleChild->m_pWeapon1_2 &&
							pChar == g_pShuttleChild->m_pWeapon1_2->m_pCharacterInfo) &&
							g_pSOption->sTerrainEffectRender <= 1)
#else
						if ((g_pShuttleChild->m_pWeapon1_1_1 &&
								pChar == g_pShuttleChild->m_pWeapon1_1_1->m_pCharacterInfo) ||
							(g_pShuttleChild->m_pWeapon1_1_2 &&
								pChar == g_pShuttleChild->m_pWeapon1_1_2->m_pCharacterInfo) ||
							(g_pShuttleChild->m_pWeapon2_1_1 &&
								pChar == g_pShuttleChild->m_pWeapon2_1_1->m_pCharacterInfo) ||
							(g_pShuttleChild->m_pWeapon2_1_2 &&
								pChar == g_pShuttleChild->m_pWeapon2_1_2->m_pCharacterInfo) ||
							(g_pShuttleChild->m_pWeapon1_2 &&
								pChar == g_pShuttleChild->m_pWeapon1_2->m_pCharacterInfo))
#endif
							DevideCharacterEffect(pChar);
					}
					else DevideCharacterEffect(pChar);
				}
			}
		}
	}
}

void CEffectRender::Render()
{
	FLOG("CEffectRender::Render()");

	g_pScene->m_vecAlphaEffectRender.clear();

	ClearAllZEnableEffect();

	DevideZBufferEnableEffect();

	RenderZEnable();

	CCharacterInfo* pChar;

	m_bZBufferTemp = FALSE;

	float fDistanceToCamera;
	float fDistance;

	for (auto pObj : g_pScene->m_vectorCulledObjectPtrList)
	{
		fDistanceToCamera = D3DXVec3Length(&(pObj->m_vPos - g_pD3dApp->m_pCamera->GetEyePt()));

		if (g_pTutorial->IsTutorialMode() == TRUE && pObj->m_nCode == TUTORIAL_GATE && !g_pTutorial->IsEnableTutorialGate(pObj->m_vPos))

			int i = 0; // HUH?

		else if (pObj->IsShowNode())
		{
			fDistance = g_pScene->m_fFogEndValue + pObj->m_pObjMesh->m_fRadius;

			if (fDistanceToCamera < fDistance)
			{
				pChar = pObj->m_pCharacterInfo;

				if (pChar != nullptr) RenderCharacterInfo(pChar);
			}
		}
	}

	if (g_pGround != nullptr && g_pGround->m_pObjectEvent != nullptr)
	{
		for (auto pObjEvent = (CObjectChild*)g_pGround->m_pObjectEvent->m_pChild; pObjEvent != nullptr; pObjEvent = (CObjectChild*)pObjEvent->m_pNext)
		{
			pChar = pObjEvent->m_pCharacterInfo;

			if (pChar != nullptr && (g_pD3dApp->m_dwGameState != _GAME ||
				g_pFrustum->CheckSphere(&pChar->m_vPos, pObjEvent->m_pObjMesh->m_fRadius)))
			{
				if (g_pTutorial->IsTutorialMode() == TRUE &&
					pObjEvent->m_nCode == TUTORIAL_GATE &&
					!g_pTutorial->IsEnableTutorialGate(pObjEvent->m_vPos))

					int i = 0;

				else if (pObjEvent->IsShowNode()) RenderCharacterInfo(pChar);
			}
		}
	}

	if (g_pD3dApp->m_bCharacter == FALSE)
	{
		if (g_pShuttleChild != nullptr)
		{
			pChar = g_pD3dApp->m_pShuttleChild->m_pCharacterInfo;

			if (pChar != nullptr) RenderCharacterInfo(pChar);

			for (auto& effectinfo : g_pShuttleChild->m_pSkillEffect->m_vecSkillEffect)

				if (effectinfo.pCharacterInfo != nullptr) RenderCharacterInfo(effectinfo.pCharacterInfo);

			for (auto pItem : g_pShuttleChild->m_pChaffData)

				if (pItem->m_pCharacterInfo != nullptr) RenderCharacterInfo(pItem->m_pCharacterInfo);
		}
	}
	else
	{
		if (g_pCharacterChild != nullptr && g_pCharacterChild->GetbPickMove() == TRUE)
		{
			pChar = g_pCharacterChild->m_pPickingInfo;

			if (pChar != nullptr) RenderCharacterInfo(pChar);
		}
	}

	// Enemy
	// 2005-07-26 by ispark

	for (auto enemy : g_pD3dApp->m_pScene->m_vecEnemyRenderList)
	{
		if (enemy->m_bEnemyCharacter == FALSE)
		{
			pChar = enemy->m_pCharacterInfo;

			if (pChar)
			{
				if (enemy->m_nAlphaValue < SKILL_OBJECT_ALPHA_NONE)
				{
					ALPHA_CHARACTERINFO stAlphaInfo;
					stAlphaInfo.pCharInfo = pChar;
					stAlphaInfo.nAlphaValue = enemy->m_nAlphaValue;
					g_pScene->m_vecAlphaEffectRender.push_back(stAlphaInfo);
				}
				else
				{
					m_bZBufferTemp = enemy->m_bZBuffer;
					RenderCharacterInfo(pChar);
					m_bZBufferTemp = FALSE;
				}
			}

			for (auto effect : enemy->m_pSkillEffect->m_vecSkillEffect)
			{
				if (effect.pCharacterInfo)
				{
					if (enemy->m_nAlphaValue < SKILL_OBJECT_ALPHA_NONE)
					{
						ALPHA_CHARACTERINFO stAlphaInfo;
						stAlphaInfo.pCharInfo = effect.pCharacterInfo;
						stAlphaInfo.nAlphaValue = enemy->m_nAlphaValue;
						g_pScene->m_vecAlphaEffectRender.push_back(stAlphaInfo);
					}
					else RenderCharacterInfo(effect.pCharacterInfo);
				}
			}

			for (auto item : enemy->m_pChaffData)
			{
				if (item->m_pCharacterInfo)
				{
					if (enemy->m_nAlphaValue < SKILL_OBJECT_ALPHA_NONE)
					{
						ALPHA_CHARACTERINFO stAlphaInfo;
						stAlphaInfo.pCharInfo = pChar;
						stAlphaInfo.nAlphaValue = enemy->m_nAlphaValue;
						g_pScene->m_vecAlphaEffectRender.push_back(stAlphaInfo);
					}
					else RenderCharacterInfo(item->m_pCharacterInfo);
				}
			}
		}
	}

	// monster
	//	if(g_pD3dApp->m_bDegree == 2) // 고사양
	//	{
	//		CMapMonsterIterator itMonster = g_pD3dApp->m_pScene->m_mapMonsterList.begin();
	//		while(itMonster != g_pD3dApp->m_pScene->m_mapMonsterList.end())
	//		{
	//			pChar = (itMonster->second)->m_pCharacterInfo;
	//			if(pChar)
	//			{
	//				RenderCharacterInfo(pChar);
	//			}
	//			itMonster++;
	//		}
	//	}
	//	else
	//	{

	for (auto monster : g_pD3dApp->m_pScene->m_vecMonsterRenderList)
	{
		pChar = monster->m_pCharacterInfo;

		if (pChar) RenderCharacterInfo(pChar);

		for (auto& effect : monster->m_pSkillEffect->m_vecSkillEffect)

			if (effect.pCharacterInfo) RenderCharacterInfo(effect.pCharacterInfo);
	}

	if (g_pD3dApp->m_pScene->m_pWeaponData)
	{
		for (auto pWeapon = (CWeapon*)g_pD3dApp->m_pScene->m_pWeaponData->m_pChild; pWeapon != nullptr; pWeapon = (CWeapon*)pWeapon->m_pNext)
		{
			pChar = pWeapon->m_pCharacterInfo;

			if (pChar)
			{
				if (g_pShuttleChild->GetSkillMissileWarning() && pWeapon->m_pTarget == g_pShuttleChild &&
					pWeapon->m_pItemData != NULL && IS_SECONDARY_WEAPON(pWeapon->m_pItemData->Kind))
				{
				}
				else RenderCharacterInfo(pChar);
			}
		}
	}
	// Item
	if (g_pD3dApp->m_pScene->m_pItemData)
	{
		CItemData* pItem = (CItemData*)g_pD3dApp->m_pScene->m_pItemData->m_pChild;
		while (pItem)
		{
			pChar = pItem->m_pCharacterInfo;
			if (pChar && pItem->m_bIsRender)
			{
				RenderCharacterInfo(pChar);
			}
			pItem->RenderItemName(); // 아이템 이름이 있을 경우 렌더링
			pItem = (CItemData *)pItem->m_pNext;
		}
	}
	if (//g_pScene->m_byMapType != MAP_TYPE_CITY && 
		(g_pD3dApp->m_dwGameState == _GAME || g_pD3dApp->m_dwGameState == _SHOP) &&
		g_pScene->m_byWeatherType != WEATHER_FOGGY && // 안개낀
		g_pScene->m_byWeatherType != WEATHER_RAINY && // 비
		g_pScene->m_byWeatherType != WEATHER_SNOWY && // 눈
		//		g_pScene->m_byWeatherType != WEATHER_CLOUDY &&			// 구름낀
		g_pScene->m_pSunData &&
		g_pScene->m_pSunData->m_pCharacterInfo &&
		IsSunRenderEnable(g_pShuttleChild->m_myShuttleInfo.MapChannelIndex.MapIndex))
	{
		pChar = g_pD3dApp->m_pScene->m_pSunData->m_pCharacterInfo;

		if (pChar) RenderCharacterInfo(pChar);
	}
	// 적기의 미사일이나 총알등을 찍는 루틴. 
	if (g_pD3dApp->m_pEffectList)
	{
		CAppEffectData* pEffect = (CAppEffectData *)g_pD3dApp->m_pEffectList->m_pChild;
		while (pEffect)
		{
			pChar = pEffect->m_pCharacterInfo;
			if (pChar)
			{
				if (pEffect->m_pParent &&
					(pEffect->m_pParent->m_dwPartType == _ENEMY || pEffect->m_pParent->m_dwPartType == _ADMIN) &&
					(pEffect->m_nType / 1000000 == 7 || pEffect->m_nType / 10000000 == 1))
				{// 테스트(장착 아이템일경우)
					// 2005-07-29 by ispark
					// 적 캐릭터일 때 찍지마
					if (((CEnemyData *)pEffect->m_pParent)->m_bEnemyCharacter == FALSE)
					{
						if (((CEnemyData *)pEffect->m_pParent)->m_bIsRender &&
							(((CAtumData*)pEffect->m_pParent)->m_bDegree != 0 || // LOW가 아니거나
								pEffect->m_nType / 100000 == 78) && // Skill Effect인 경우
							(((CEnemyData *)pEffect->m_pParent)->m_bySkillStateFlag == CL_SKILL_NONE || // 2006-12-12 by ispark, 위장, 인비지블이 아니고
								(((CEnemyData *)pEffect->m_pParent)->m_bySkillStateFlag != CL_SKILL_NONE && // 2006-12-11 by ispark, 위장, 인비지블이라면
#ifdef FREEWAR_
									(IsInfluenceCharacter(g_pShuttleChild->m_myShuttleInfo.InfluenceType, ((CEnemyData *)pEffect->m_pParent)->m_infoCharacter.CharacterInfo.InfluenceType, 1, g_pShuttleChild->m_myShuttleInfo.MapChannelIndex.MapIndex) || // 2006-12-12 by ispark, 같은 세력 이거나
#else
							(IsInfluenceCharacter(g_pShuttleChild->m_myShuttleInfo.InfluenceType, ((CEnemyData *)pEffect->m_pParent)->m_infoCharacter.CharacterInfo.InfluenceType) || // 2006-12-12 by ispark, 같은 세력 이거나
#endif
										!(((CEnemyData *)pEffect->m_pParent)->IsPkEnable())))))
						{
							RenderCharacterInfo(pChar);
						}
					}
					else
					{
						// 2006-06-29 by ispark, 악세사리 경우는 캐릭터 상태에서도 렌더링한다.
						if ((((CEnemyData *)pEffect->m_pParent)->m_pContainer &&
								((CEnemyData *)pEffect->m_pParent)->m_pContainer->m_pCharacterInfo == pChar) ||
							(((CEnemyData *)pEffect->m_pParent)->m_pAccessories &&
								((CEnemyData *)pEffect->m_pParent)->m_pAccessories->m_pCharacterInfo == pChar) ||
							(((CEnemyData *)pEffect->m_pParent)->m_pWingIn &&
								((CEnemyData *)pEffect->m_pParent)->m_pWingIn->m_pCharacterInfo == pChar) ||
							// 2011. 01. 13 by jskim, 인비지블 상태에서 펫이 보이는 버그 수정
							(((CEnemyData *)pEffect->m_pParent)->m_pPartner &&
								((CEnemyData *)pEffect->m_pParent)->m_pPartner->m_pCharacterInfo == pChar) ||
							(((CEnemyData *)pEffect->m_pParent)->m_pPartner1 &&
								((CEnemyData *)pEffect->m_pParent)->m_pPartner1->m_pCharacterInfo == pChar))
						// end 2011. 01. 13 by jskim, 인비지블 상태에서 펫이 보이는 버그 수정							
						{
							if (((CEnemyData *)pEffect->m_pParent)->m_bIsRender)
							{
								DevideCharacterEffect(pChar);
							}
						}
					}
				}
				else
				{
					if (g_pD3dApp->m_pCamera->GetCamType() == CAMERA_TYPE_FPS &&
						g_pShuttleChild == pEffect->m_pParent)
					{
						if ((g_pShuttleChild->m_pWeapon1_1_1 &&
								pChar == g_pShuttleChild->m_pWeapon1_1_1->m_pCharacterInfo) ||
							(g_pShuttleChild->m_pWeapon1_1_2 &&
								pChar == g_pShuttleChild->m_pWeapon1_1_2->m_pCharacterInfo) ||
							(g_pShuttleChild->m_pWeapon1_2 &&
								pChar == g_pShuttleChild->m_pWeapon1_2->m_pCharacterInfo) ||
							pEffect->m_nType == RC_EFF_AGEAR_FIRE)
						{
							RenderCharacterInfo(pChar);
						}
					}
					else
					{
						// 2005-07-27 by ispark
						// 캐릭터 렌더링시에는 무기를 그리지 않는다.
						// DevideCharacterEffect(pChar);
						if (g_pD3dApp->m_bCharacter)
						{
							//							if(g_pShuttleChild != pEffect->m_pParent)
							{
								RenderCharacterInfo(pChar);
							}
						}
						else
						{
							RenderCharacterInfo(pChar);
						}
					}
				}
			}
			pEffect = (CAppEffectData*)pEffect->m_pNext;
		}
	}

	// 2005-01-04 by jschoi
	//	g_pD3dDev->SetTextureStageState( 0, D3DTSS_MIPFILTER, D3DTEXF_NONE );	
	//	g_pD3dDev->SetSamplerState(0,D3DSAMP_MIPFILTER,D3DTEXF_NONE);
}

void CEffectRender::RenderCharacterInfo(CCharacterInfo* pChar, BOOL bAlpha, int nAlphaValue)
{
	FLOG("CEffectRender::RenderCharacterInfo(CCharacterInfo* pChar)");

	for (auto itCurrent : pChar->m_vecCurrentBodyCondition)
	{
		auto itBody = pChar->m_mapBodyCondition.find(itCurrent);
		if (itBody != pChar->m_mapBodyCondition.end())
		{
			CBodyConditionInfo* pBody = itBody->second;
			for (auto pEffectInfo : pBody->m_vecEffect)
			{
				switch (pEffectInfo->m_nEffectType)
				{
				case EFFECT_TYPE_OBJECT:
					{
						if (pEffectInfo->m_pEffect != nullptr)
						{
							auto pEffect = (CObjectAni*)pEffectInfo->m_pEffect;

							D3DXVECTOR3 vPos(pEffect->m_pParent->m_pParent->m_mMatrix._41 - pEffect->m_pParent->m_vPos.z,
							                 pEffect->m_pParent->m_pParent->m_mMatrix._42 + pEffect->m_pParent->m_vPos.y,
							                 pEffect->m_pParent->m_pParent->m_mMatrix._43 + pEffect->m_pParent->m_vPos.x);

							bool bResult = g_pD3dApp->m_dwGameState != _GAME || g_pFrustum->CheckSphere(&vPos, pEffect->m_fRadius);

							if (bResult && !pEffect->m_bZbufferEnable && pEffect->m_bAlphaBlending)

								ObjectAniRender(pEffect, bAlpha, nAlphaValue);
						}
						break;
					}
				case EFFECT_TYPE_SPRITE:
					{
						if (pEffectInfo->m_pEffect)
						{
							auto pEffect = (CSpriteAni*)pEffectInfo->m_pEffect;

							D3DXVECTOR3 vPos(pEffect->m_pParent->m_pParent->m_mMatrix._41 - pEffect->m_pParent->m_vPos.z,
							                 pEffect->m_pParent->m_pParent->m_mMatrix._42 + pEffect->m_pParent->m_vPos.y,
							                 pEffect->m_pParent->m_pParent->m_mMatrix._43 + pEffect->m_pParent->m_vPos.x);

							bool bResult = g_pD3dApp->m_dwGameState != _GAME || g_pFrustum->CheckSphere(&vPos, pEffect->m_fTextureSize);

							if (bResult && !pEffect->m_bZbufferEnable)

								SpriteAniRender(pEffect);
						}
						break;
					}
				case EFFECT_TYPE_PARTICLE:
					{
						if (pEffectInfo->m_pEffect)
						{
							auto pEffect = (CParticleSystem*)pEffectInfo->m_pEffect;

							D3DXVECTOR3 vPos(pEffect->m_pParent->m_pParent->m_mMatrix._41 - pEffect->m_pParent->m_vPos.z,
							                 pEffect->m_pParent->m_pParent->m_mMatrix._42 + pEffect->m_pParent->m_vPos.y,
							                 pEffect->m_pParent->m_pParent->m_mMatrix._43 + pEffect->m_pParent->m_vPos.x);

							bool bResult = g_pD3dApp->m_dwGameState != _GAME || g_pFrustum->CheckSphere(&vPos, pEffect->m_fRadius);

							if (bResult && !pEffect->m_bZbufferEnable)

								ParticleSystemRender(pEffect);
						}
						break;
					}
				case EFFECT_TYPE_TRACE:
					{
						if (pEffectInfo->m_pEffect)
						{
							auto pEffect = (CTraceAni*)pEffectInfo->m_pEffect;

							D3DXVECTOR3 vPos(pEffect->m_pParent->m_pParent->m_mMatrix._41 - pEffect->m_pParent->m_vPos.z,
							                 pEffect->m_pParent->m_pParent->m_mMatrix._42 + pEffect->m_pParent->m_vPos.y,
							                 pEffect->m_pParent->m_pParent->m_mMatrix._43 + pEffect->m_pParent->m_vPos.x);

							bool bResult = g_pD3dApp->m_dwGameState != _GAME || g_pFrustum->CheckSphere(&vPos, pEffect->m_fRadius);

							if (bResult && !pEffect->m_bZbufferEnable)

								TraceAniRender(pEffect);
						}
						break;
					}
				}
			}
		}
	}
}

void CEffectRender::SpriteAniRender(CSpriteAni* pEffect)
{
	FLOG("CEffectRender::SpriteAniRender(CSpriteAni* pEffect)");
	DWORD dwSrc, dwDest, dwColorOp;
	g_pD3dDev->GetRenderState(D3DRS_SRCBLEND, &dwSrc);
	g_pD3dDev->GetRenderState(D3DRS_DESTBLEND, &dwDest);
	g_pD3dDev->GetTextureStageState(0, D3DTSS_COLOROP, &dwColorOp);

	g_pD3dDev->SetRenderState(D3DRS_LIGHTING, TRUE);
	//	g_pD3dDev->SetRenderState( D3DRS_ZENABLE, TRUE);
	g_pD3dDev->SetRenderState(D3DRS_ZENABLE, pEffect->m_bZbufferEnable);

	if (pEffect->m_bZWriteEnable == FALSE)
	{
		g_pD3dDev->SetRenderState(D3DRS_ZWRITEENABLE, pEffect->m_bZWriteEnable);
	}
	g_pD3dDev->SetRenderState(D3DRS_ALPHABLENDENABLE, pEffect->m_bAlphaBlending);
	if (pEffect->m_bAlphaBlending)
	{
		g_pD3dDev->SetRenderState(D3DRS_SRCBLEND, pEffect->m_nSrcBlend);
		g_pD3dDev->SetRenderState(D3DRS_DESTBLEND, pEffect->m_nDestBlend);
	}
	g_pD3dDev->SetTextureStageState(0, D3DTSS_COLOROP, pEffect->m_nTextureRenderState);
	//    g_pD3dDev->SetTextureStageState( 0, D3DTSS_MAGFILTER, D3DTEXF_LINEAR  );
	//    g_pD3dDev->SetTextureStageState( 0, D3DTSS_MINFILTER, D3DTEXF_LINEAR  );
	g_pD3dDev->SetFVF(D3DFVF_SPRITE_VERTEX);

	// set light
	D3DXVECTOR3 vAxis = g_pD3dApp->m_pCamera->GetViewDir();
	m_light2 = g_pD3dApp->m_pScene->m_light2;
	m_light2.Direction = vAxis;
	g_pD3dDev->SetLight(2, &m_light2);
	g_pD3dDev->LightEnable(2, TRUE);

	D3DXMATRIX matScale;
	//	D3DXVECTOR3 pos = pEffect->m_pParent->m_pParent->m_pParent->m_vPos + pEffect->m_pParent->m_vPos;
	D3DXVECTOR3 pos = pEffect->m_pParent->m_vPos;
	D3DXVec3TransformCoord(&pos, &pos, &pEffect->m_pParent->m_pParent->m_mMatrix);

	D3DXMatrixScaling(&matScale, pEffect->m_fTextureSize, pEffect->m_fTextureSize, pEffect->m_fTextureSize);
	matScale *= g_pD3dApp->m_pCamera->GetBillboardMatrix();
	if ((pEffect->m_pParent && pEffect->m_pParent->m_fBillboardRotateAngle > 0) ||
		pEffect->m_fCurrentRotateAngle != 0)
	{
		D3DXMATRIX matRotate;
		//		vAxis = pEffect->m_pParent->m_vPos - g_pD3dApp->m_pCamera->GetEyePt();
		//		vAxis = pos - g_pD3dApp->m_pCamera->GetEyePt();
		D3DXMatrixRotationAxis(&matRotate, &vAxis, pEffect->m_fCurrentRotateAngle);
		matScale *= matRotate;
	}
	matScale._41 = pos.x;
	matScale._42 = pos.y;
	matScale._43 = pos.z;

	D3DMATERIAL9 mtrl;
	D3DUtil_InitMaterial(mtrl, pEffect->m_cColor.r,
	                     pEffect->m_cColor.g, pEffect->m_cColor.b, pEffect->m_cColor.a);

	g_pD3dDev->SetTransform(D3DTS_WORLD, &matScale);
	if (pEffect->m_nTextureVertexBufferType == 0)
		g_pD3dDev->SetStreamSource(0, m_pVB4[pEffect->m_nSpriteType], 0, sizeof(SPRITE_VERTEX));
	else if (pEffect->m_nTextureVertexBufferType == 1)
		g_pD3dDev->SetStreamSource(0, m_pVB8[pEffect->m_nSpriteType], 0, sizeof(SPRITE_VERTEX));
	else if (pEffect->m_nTextureVertexBufferType == 2)
		g_pD3dDev->SetStreamSource(0, m_pVB16[pEffect->m_nSpriteType], 0, sizeof(SPRITE_VERTEX));
	else if (pEffect->m_nTextureVertexBufferType == 3)
		g_pD3dDev->SetStreamSource(0, m_pVB2[pEffect->m_nSpriteType], 0, sizeof(SPRITE_VERTEX));
	else if (pEffect->m_nTextureVertexBufferType == 4)
		g_pD3dDev->SetStreamSource(0, m_pVB1, 0, sizeof(SPRITE_VERTEX));
	g_pD3dDev->SetMaterial(&mtrl);
	// set texture
	if (pEffect->m_pParent && pEffect->m_pParent->m_pTexture)
	{
		g_pD3dDev->SetTexture(0, pEffect->m_pParent->m_pTexture);
	}
	else
	{
		int index = LoadTexture(pEffect->m_strTextureFile);
		if (index >= 0)
		{
			g_pD3dDev->SetTexture(0, m_pTexture[index]);
			if (m_nTextureRenderCount[index] == 2)
				m_nTextureRenderCount[index]++;
		}
	}
	g_pD3dDev->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	if (strlen(pEffect->m_strLightMapFile) > 0)
	{
		g_pD3dDev->SetRenderState(D3DRS_ALPHABLENDENABLE, pEffect->m_bLightMapAlphaBlending);
		g_pD3dDev->SetRenderState(D3DRS_SRCBLEND, pEffect->m_nLightMapSrcBlend);
		g_pD3dDev->SetRenderState(D3DRS_DESTBLEND, pEffect->m_nLightMapDestBlend);
		g_pD3dDev->SetTextureStageState(0, D3DTSS_COLOROP, pEffect->m_nLightMapRenderState);
		//		Set texture
		if (pEffect->m_pParent && pEffect->m_pParent->m_pTexture)
		{
			g_pD3dDev->SetTexture(0, pEffect->m_pParent->m_pTexture);
		}
		else
		{
			int index = LoadTexture(pEffect->m_strLightMapFile);
			if (index >= 0)
			{
				g_pD3dDev->SetTexture(0, m_pTexture[index]);
				if (m_nTextureRenderCount[index] == 2)
					m_nTextureRenderCount[index]++;
			}
		}
		g_pD3dDev->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	}
	g_pD3dDev->SetRenderState(D3DRS_SRCBLEND, dwSrc);
	g_pD3dDev->SetRenderState(D3DRS_DESTBLEND, dwDest);
	g_pD3dDev->SetTextureStageState(0, D3DTSS_COLOROP, dwColorOp);

	g_pD3dDev->LightEnable(2, FALSE);
	if (pEffect->m_bZWriteEnable == FALSE)
	{
		g_pD3dDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	}
}


void CEffectRender::ParticleSystemRender(CParticleSystem* pEffect)
{
	FLOG("CEffectRender::ParticleSystemRender(CParticleSystem* pEffect)");
	//	g_pD3dDev->ApplyStateBlock( pEffect->m_dwStateBlock );
	switch (pEffect->m_nParticleType)
	{
	case PARTICLE_OBJECT_TYPE:
		{
			for (int i = 0; i < pEffect->m_vecParticle.size(); i++)
			{
				ObjectParticleRender(((CParticle*)pEffect->m_vecParticle[i])->m_pObjectAni, (CParticle*)pEffect->m_vecParticle[i]);
			}
		}
		break;
	case PARTICLE_SPRITE_TYPE:
		{
			g_pD3dDev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
			g_pD3dDev->SetFVF(D3DFVF_SPRITE_VERTEX);
			//	g_pD3dDev->SetTextureStageState( 0, D3DTSS_MINFILTER, D3DTEXF_LINEAR );
			//	g_pD3dDev->SetTextureStageState( 0, D3DTSS_MAGFILTER, D3DTEXF_LINEAR );

			//			g_pD3dDev->SetRenderState( D3DRS_ALPHATESTENABLE,  FALSE );
			//			g_pD3dDev->SetRenderState( D3DRS_ALPHAREF,         0x08 );
			//			g_pD3dDev->SetRenderState( D3DRS_ALPHAFUNC,  D3DCMP_GREATEREQUAL );

			//			g_pD3dDev->SetRenderState( D3DRS_ZENABLE, TRUE);
			g_pD3dDev->SetRenderState(D3DRS_ZENABLE, pEffect->m_bZbufferEnable);
			g_pD3dDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
			g_pD3dDev->SetRenderState(D3DRS_SRCBLEND, pEffect->m_dwSrcBlend);
			g_pD3dDev->SetRenderState(D3DRS_DESTBLEND, pEffect->m_dwDestBlend);
			g_pD3dDev->SetRenderState(D3DRS_LIGHTING, TRUE);
			g_pD3dDev->SetStreamSource(0, m_pVB1, 0, sizeof(SPRITE_VERTEX));
			// set light
			D3DXVECTOR3 vAxis = g_pD3dApp->m_pCamera->GetViewDir();
			m_light2 = g_pD3dApp->m_pScene->m_light2;
			m_light2.Direction = vAxis;
			g_pD3dDev->SetLight(2, &m_light2);
			g_pD3dDev->LightEnable(2, TRUE);
			int nOldTextureIndex = -1;
			for (int i = 0; i < pEffect->m_vecParticle.size(); i++)
			{
				CParticle* p = (CParticle*)pEffect->m_vecParticle[i];
				nOldTextureIndex = ParticleRender(pEffect, p, vAxis, nOldTextureIndex);
			}
			g_pD3dDev->LightEnable(2, FALSE);
		}
		break;
	case PARTICLE_TRACE_TYPE: break;
	}
}

int CEffectRender::ParticleRender(CParticleSystem* pParticleSystem, CParticle* p, D3DXVECTOR3 vAxis, int nOldTextureIndex)
{
	FLOG("CEffectRender::ParticleRender(CParticleSystem* pParticleSystem, CParticle* p, D3DXVECTOR3 vAxis, int nOldTextureIndex)");
	D3DXMATRIX matScale;
	D3DXVECTOR3 pos;
	D3DMATERIAL9 mtrl;
	D3DUtil_InitMaterial(mtrl, p->m_cColor.r, p->m_cColor.g, p->m_cColor.b, p->m_cColor.a);
	pos = p->m_vPos;//실제 좌표

	// 2010. 03. 18 by jskim 몬스터변신 카드
	float tempScale = 0.0f;
	if (p->m_pParent->m_pParent->m_pParent->m_MonsterTransformer &&
		p->m_pParent->m_pParent->m_pParent->m_MonsterTransScale > 0)
	{
		tempScale = p->m_fSize * p->m_pParent->m_pParent->m_pParent->m_MonsterTransScale;
	}
	else
	{
		tempScale = p->m_fSize;
	}
	//D3DXMatrixScaling(&matScale, p->m_fSize,p->m_fSize,p->m_fSize);	
	D3DXMatrixScaling(&matScale, tempScale, tempScale, tempScale);
	// 2010. 03. 18 by jskim 몬스터변신 카드

	matScale *= g_pD3dApp->m_pCamera->GetBillboardMatrix();
	// 2004.2.16 파티클 랜덤 회전
	if (pParticleSystem->m_pParent && p->m_fCurrentRotateAngle != 0)//pParticleSystem->m_pParent->m_fBillboardRotateAngle > 0)
	{
		D3DXMATRIX matRotate;
		//			vAxis = pParticleSystem->m_pParent->m_vPos - g_pD3dApp->m_pCamera->GetEyePt();
		//			vAxis = p->m_vPos - g_pD3dApp->m_pCamera->GetEyePt();
		D3DXMatrixRotationAxis(&matRotate, &vAxis, p->m_fCurrentRotateAngle);
		matScale *= matRotate;
	}
	matScale._41 = pos.x;
	matScale._42 = pos.y;
	matScale._43 = pos.z;
	//		matScale._41 += pParticleSystem->m_pParent->m_vPos.x;
	//		matScale._42 += pParticleSystem->m_pParent->m_vPos.y;
	//		matScale._43 += pParticleSystem->m_pParent->m_vPos.z;

	g_pD3dDev->SetTransform(D3DTS_WORLD, &matScale);
	// set texture

	int index = 0;
	if (pParticleSystem->m_pParent && pParticleSystem->m_pParent->m_pTexture)
	{
		g_pD3dDev->SetTexture(0, pParticleSystem->m_pParent->m_pTexture);
	}
	else if (nOldTextureIndex == -1 || pParticleSystem->m_nTextureNumber != 1)
	{
		index = LoadTexture(pParticleSystem->m_strTextureName[p->m_nTextureType]);
		if (index >= 0 && nOldTextureIndex != index)
		{
			g_pD3dDev->SetTexture(0, m_pTexture[index]);
			if (m_nTextureRenderCount[index] == 2)
				m_nTextureRenderCount[index]++;
			nOldTextureIndex = index;
		}
	}
	g_pD3dDev->SetMaterial(&mtrl);
	g_pD3dDev->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	return nOldTextureIndex;
}

// 2007-11-08 by bhsohn 인벤 이펙트 관련 처리
void CEffectRender::RenderParticleInvenVector(int nMatIndex, D3DXMATRIX matShuttlePos, D3DXMATRIX matPos, float fUnitScaling)
{
	if (m_vecInvenParticleInfo.empty())
	{
		return;
	}
	int nSize = m_vecInvenParticleInfo.size();
	int nRender = 0;

	vector<structInvenParticleInfo>::iterator itInvenEffect = m_vecInvenParticleInfo.begin();

	while (itInvenEffect != m_vecInvenParticleInfo.end())
	{
		structInvenParticleInfo struParticleInfo = (*itInvenEffect);

		if (struParticleInfo.nWindowInvenIdx != nMatIndex)
		{
			itInvenEffect++;
			continue;
		}
		CEffectInfo* pEffectInfo = GetEffectInfo(struParticleInfo.chEffectName, struParticleInfo.nWindowInvenIdx);
		if (NULL == pEffectInfo)
		{
			itInvenEffect++;
			continue;
		}

		CParticleSystem* pParticleSystem = (CParticleSystem*)pEffectInfo->m_pEffect;

		if (NULL == pParticleSystem)
		{
			itInvenEffect++;
			continue;
		}

		{
			g_pD3dDev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
			g_pD3dDev->SetFVF(D3DFVF_SPRITE_VERTEX);
			g_pD3dDev->SetRenderState(D3DRS_ZENABLE, pParticleSystem->m_bZbufferEnable);
			//			g_pD3dDev->SetRenderState( D3DRS_ZENABLE, TRUE);
			if (pParticleSystem->m_bZWriteEnable == FALSE)
			{
				g_pD3dDev->SetRenderState(D3DRS_ZWRITEENABLE, pParticleSystem->m_bZWriteEnable);
			}

			g_pD3dDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
			g_pD3dDev->SetRenderState(D3DRS_SRCBLEND, pParticleSystem->m_dwSrcBlend);
			g_pD3dDev->SetRenderState(D3DRS_DESTBLEND, pParticleSystem->m_dwDestBlend);

			g_pD3dDev->SetRenderState(D3DRS_LIGHTING, TRUE);

			g_pD3dDev->SetStreamSource(0, m_pVB1, 0, sizeof(SPRITE_VERTEX));
			// set light
			D3DXVECTOR3 vAxis = g_pD3dApp->m_pCamera->GetViewDir();
			m_light2 = g_pD3dApp->m_pScene->m_light2;
			m_light2.Direction = vAxis;
			g_pD3dDev->SetLight(2, &m_light2);
			g_pD3dDev->LightEnable(2, TRUE);

			// 실제로 파티클 이펙트 랜더링 해주는 곳			
			int nOldTextureIndex = -1;
			int i = 0;
			for (i = 0; i < pParticleSystem->m_vecParticle.size(); i++)
			{
				CParticle* p = (CParticle*)pParticleSystem->m_vecParticle[i];
				nOldTextureIndex = InvenParticleRender(pParticleSystem, p, vAxis, nOldTextureIndex, fUnitScaling, &matPos, &matShuttlePos);
			}

			g_pD3dDev->LightEnable(2, FALSE);
			if (pParticleSystem->m_bZWriteEnable == FALSE)
			{
				g_pD3dDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
			}
			// 파티클 환경 종료
			{
				g_pD3dDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
				g_pD3dDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
			}

			nRender++;
		}
		itInvenEffect++;
	}
}

// 2012-07-13 by isshin 아템미리보기 인첸트이펙스 적용
void CEffectRender::RenderParticleEnemyItemVector(int nMatIndex, D3DXMATRIX matShuttlePos, D3DXMATRIX matPos, float fUnitScaling, UID32_t TargetCharcterUID)
{
	if (m_vecEnemyItemParticleInfo.empty())
	{
		return;
	}
	int nSize = m_vecEnemyItemParticleInfo.size();
	int nRender = 0;

	vector<structInvenParticleInfo>::iterator itInvenEffect = m_vecEnemyItemParticleInfo.begin();

	while (itInvenEffect != m_vecEnemyItemParticleInfo.end())
	{
		structInvenParticleInfo struParticleInfo = (*itInvenEffect);

		if (struParticleInfo.nWindowInvenIdx != nMatIndex)
		{
			itInvenEffect++;
			continue;
		}
		CEffectInfo* pEffectInfo = GetEnemyEffectInfo(struParticleInfo.chEffectName, struParticleInfo.nWindowInvenIdx, TargetCharcterUID);
		if (NULL == pEffectInfo)
		{
			itInvenEffect++;
			continue;
		}

		CParticleSystem* pParticleSystem = (CParticleSystem*)pEffectInfo->m_pEffect;

		if (NULL == pParticleSystem)
		{
			itInvenEffect++;
			continue;
		}

		{
			g_pD3dDev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
			g_pD3dDev->SetFVF(D3DFVF_SPRITE_VERTEX);
			g_pD3dDev->SetRenderState(D3DRS_ZENABLE, pParticleSystem->m_bZbufferEnable);
			//			g_pD3dDev->SetRenderState( D3DRS_ZENABLE, TRUE);
			if (pParticleSystem->m_bZWriteEnable == FALSE)
			{
				g_pD3dDev->SetRenderState(D3DRS_ZWRITEENABLE, pParticleSystem->m_bZWriteEnable);
			}

			g_pD3dDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
			g_pD3dDev->SetRenderState(D3DRS_SRCBLEND, pParticleSystem->m_dwSrcBlend);
			g_pD3dDev->SetRenderState(D3DRS_DESTBLEND, pParticleSystem->m_dwDestBlend);

			g_pD3dDev->SetRenderState(D3DRS_LIGHTING, TRUE);

			g_pD3dDev->SetStreamSource(0, m_pVB1, 0, sizeof(SPRITE_VERTEX));
			// set light
			D3DXVECTOR3 vAxis = g_pD3dApp->m_pCamera->GetViewDir();
			m_light2 = g_pD3dApp->m_pScene->m_light2;
			m_light2.Direction = vAxis;
			g_pD3dDev->SetLight(2, &m_light2);
			g_pD3dDev->LightEnable(2, TRUE);

			// 실제로 파티클 이펙트 랜더링 해주는 곳			
			int nOldTextureIndex = -1;
			int i = 0;
			for (i = 0; i < pParticleSystem->m_vecParticle.size(); i++)
			{
				CParticle* p = (CParticle*)pParticleSystem->m_vecParticle[i];
				nOldTextureIndex = InvenParticleRender(pParticleSystem, p, vAxis, nOldTextureIndex, fUnitScaling, &matPos, &matShuttlePos);
			}

			g_pD3dDev->LightEnable(2, FALSE);
			if (pParticleSystem->m_bZWriteEnable == FALSE)
			{
				g_pD3dDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
			}
			// 파티클 환경 종료
			{
				g_pD3dDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
				g_pD3dDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
			}

			nRender++;
		}
		itInvenEffect++;
	}
}

// end 2012-07-13 by isshin 아템미리보기 인첸트이펙스 적용

// 2007-11-08 by bhsohn 인벤 이펙트 관련 처리
void CEffectRender::ResetContentInvneParticle()
{
	// 인벤 파티클 초기화
	m_vecInvenParticleInfo.clear();
}

// 2012-07-13 by isshin 아템미리보기 인첸트이펙스 적용
void CEffectRender::ResetContentEnemyItemParticle()
{
	m_vecEnemyItemParticleInfo.clear();
}

// end 2012-07-13 by isshin 아템미리보기 인첸트이펙스 적용

int CEffectRender::InvenParticleRender(CParticleSystem* pParticleSystem, CParticle* p, D3DXVECTOR3 vAxis, int nOldTextureIndex, float fUnitScaling, D3DXMATRIX* pmatPaticlePos, D3DXMATRIX* pmatShttlePos)
{
	FLOG("CEffectRender::ParticleRender(CParticleSystem* pParticleSystem, CParticle* p, D3DXVECTOR3 vAxis, int nOldTextureIndex)");
	D3DXMATRIX matScale;
	D3DXVECTOR3 pos;
	D3DMATERIAL9 mtrl;
	D3DUtil_InitMaterial(mtrl, p->m_cColor.r / 255, p->m_cColor.g, p->m_cColor.b, p->m_cColor.a);

	float fSize = p->m_fSize;
	pos = p->m_vPos;//실제 좌표
	if (pmatPaticlePos && pmatShttlePos)
	{
		// 스케일링
		//fSize *= 0.0074f;
		fSize *= (fUnitScaling / 2);


		// 현재 월드에 대한 정보		
		D3DXVECTOR3 vInvenShuttle, vParticlePos, vObjInvenPos; // 실제 그려질 파티클 위치
		D3DXVECTOR3 vDirWorldVel; // 월드 방향벡터 
		D3DXVECTOR3 vDirInvenVel; // 인벤 방향벡터 
		D3DXVECTOR3 vWeaponInvenVel; // 무기 방향벡터

		vWeaponInvenVel = D3DXVECTOR3(0, 0, 0);
		D3DXVECTOR3 vParCenter = D3DXVECTOR3(0, 0, 0);// 파티클의 중심점			
		D3DXVECTOR3 vParResultDir = D3DXVECTOR3(0, 0, 0);// 파티클의 중심점			

		// 인벤토리내에 기체의 위치
		{
			vInvenShuttle.x = pmatShttlePos->_41;
			vInvenShuttle.y = pmatShttlePos->_42;
			vInvenShuttle.z = pmatShttlePos->_43;
		}

		// 인벤토리내 무기 오브젝트 위치
		{
			vObjInvenPos.x = pmatPaticlePos->_41;
			vObjInvenPos.y = pmatPaticlePos->_42;
			vObjInvenPos.z = pmatPaticlePos->_43;
		}

		// 파티클에 실질적으로 위치할 위치
		{
			vParticlePos.x = pmatPaticlePos->_41;
			vParticlePos.y = pmatPaticlePos->_42;
			vParticlePos.z = pmatPaticlePos->_43;
		}

		// 인벤 방향 벡터
		{
			vDirInvenVel.x = pmatPaticlePos->_31;
			vDirInvenVel.y = pmatPaticlePos->_32;
			vDirInvenVel.z = pmatPaticlePos->_33;
			vDirInvenVel *= -1;
			D3DXVec3Normalize(&vDirInvenVel, &vDirInvenVel);
		}

		// 월드 좌표 방향 벡터
		{
			vDirWorldVel.x = pParticleSystem->m_pParent->m_pParent->m_mMatrix._31;
			vDirWorldVel.y = pParticleSystem->m_pParent->m_pParent->m_mMatrix._32;
			vDirWorldVel.z = pParticleSystem->m_pParent->m_pParent->m_mMatrix._33;
			vDirWorldVel *= -1;
			D3DXVec3Normalize(&vDirWorldVel, &vDirWorldVel);
		}
		// 월드 좌표에 실질적인 파티클의 중심점		
		{
			vParCenter.x = pParticleSystem->m_pParent->m_pParent->m_mMatrix._41;
			vParCenter.y = pParticleSystem->m_pParent->m_pParent->m_mMatrix._42;
			vParCenter.z = pParticleSystem->m_pParent->m_pParent->m_mMatrix._43;
		}

		D3DXVECTOR3 vCrossWorldAxisY;// Y 축을 구한다.
		D3DXVECTOR3 vCrossWorldAxisX;// X 축을 구한다.
		float fAngleRadianY = 0.0f;
		float fAngleRadianX = 0.0f;
		{
			D3DXVECTOR3 vTmpWeaponVel = p->m_vPos - vParCenter;
			D3DXVec3Normalize(&vTmpWeaponVel, &vTmpWeaponVel);


			D3DXVec3Cross(&vCrossWorldAxisY, &vDirWorldVel, &vTmpWeaponVel);
			D3DXVec3Normalize(&vCrossWorldAxisY, &vCrossWorldAxisY); // 중심축이다. 

			D3DXVec3Cross(&vCrossWorldAxisX, &vDirWorldVel, &vCrossWorldAxisY);
			D3DXVec3Normalize(&vCrossWorldAxisX, &vCrossWorldAxisX); // 중심축이다. 

			fAngleRadianX = GetRadianVectorBetweenVector(vCrossWorldAxisX, vTmpWeaponVel);
			fAngleRadianY = GetRadianVectorBetweenVector(vDirWorldVel, vTmpWeaponVel);
		}

		{
			D3DXMATRIX matRotateY;
			D3DXMATRIX matRotateX;
			D3DXMATRIX matRotate;

			D3DXMatrixIdentity(&matRotateY);
			D3DXMatrixIdentity(&matRotateX);
			D3DXMatrixRotationAxis(&matRotateY, &vCrossWorldAxisY, fAngleRadianY);
			D3DXMatrixRotationAxis(&matRotateX, &vCrossWorldAxisX, fAngleRadianX);
			matRotate = matRotateX * matRotateY;
			// 실질적인 이펙트 벡터를 구한다.
			D3DXVec3TransformCoord(&vParResultDir, &vDirInvenVel, &matRotate);
			D3DXVec3Normalize(&vParResultDir, &vParResultDir); // 중심축이다. 			
		}

		{
			// 월드좌표에 파티클 거리
			D3DXVECTOR3 vParVel = vParCenter - p->m_vPos;
			float fParticleLength = D3DXVec3Length(&vParVel);

			//fParticleLength *= 0.025f;				
			fParticleLength *= fUnitScaling;

			vParResultDir *= fParticleLength;
			vParticlePos += vParResultDir;
		}

		//		{
		//			//p->m_vDir;		
		//			// 월드좌표에 파티클 거리
		//			D3DXVECTOR3 vParVel = vParCenter - p->m_vPos;
		//			float fParticleLength = D3DXVec3Length(&vParVel);
		//			D3DXVec3Normalize( &vParVel, &vParVel);
		//			
		//			//fParticleLength *= 0.025f;				
		//			fParticleLength *= fUnitScaling;
		//			
		//			vParVel*= fParticleLength;
		//			vParticlePos += vParVel;
		//		}
		pos = vParticlePos;
	}

	D3DXMatrixScaling(&matScale, fSize, fSize, fSize);

	//matScale *= g_pD3dApp->m_pCamera->GetBillboardMatrix();	

	// 2004.2.16 파티클 랜덤 회전
	if (pParticleSystem->m_pParent && p->m_fCurrentRotateAngle != 0)//pParticleSystem->m_pParent->m_fBillboardRotateAngle > 0)
	{
		D3DXMATRIX matRotate;
		//			vAxis = pParticleSystem->m_pParent->m_vPos - g_pD3dApp->m_pCamera->GetEyePt();
		//			vAxis = p->m_vPos - g_pD3dApp->m_pCamera->GetEyePt();
		D3DXMatrixRotationAxis(&matRotate, &vAxis, p->m_fCurrentRotateAngle);
		matScale *= matRotate;
	}
	matScale._41 = pos.x;
	matScale._42 = pos.y;
	matScale._43 = pos.z;
	//		matScale._41 += pParticleSystem->m_pParent->m_vPos.x;
	//		matScale._42 += pParticleSystem->m_pParent->m_vPos.y;
	//		matScale._43 += pParticleSystem->m_pParent->m_vPos.z;

	g_pD3dDev->SetTransform(D3DTS_WORLD, &matScale);
	// set texture

	int index = 0;
	if (pParticleSystem->m_pParent && pParticleSystem->m_pParent->m_pTexture)
	{
		g_pD3dDev->SetTexture(0, pParticleSystem->m_pParent->m_pTexture);
	}
	else if (nOldTextureIndex == -1 || pParticleSystem->m_nTextureNumber != 1)
	{
		index = LoadTexture(pParticleSystem->m_strTextureName[p->m_nTextureType]);
		if (index >= 0 && nOldTextureIndex != index)
		{
			g_pD3dDev->SetTexture(0, m_pTexture[index]);
			if (m_nTextureRenderCount[index] == 2)
				m_nTextureRenderCount[index]++;
			nOldTextureIndex = index;
		}
	}
	g_pD3dDev->SetMaterial(&mtrl);
	g_pD3dDev->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	return nOldTextureIndex;
}


void CEffectRender::EffectPlaneRender(CEffectPlane* pEffect)
{
	if (pEffect->m_pParent->m_nCurrentNumberOfTrace <= 0)
		return;
	int index = LoadTexture(pEffect->m_pParent->m_strTextureName[pEffect->m_pParent->m_nCurrentTextureNumber]);

	DWORD dwSrc, dwDest, dwColorOp;
	g_pD3dDev->GetRenderState(D3DRS_SRCBLEND, &dwSrc);
	g_pD3dDev->GetRenderState(D3DRS_DESTBLEND, &dwDest);
	g_pD3dDev->GetTextureStageState(0, D3DTSS_COLOROP, &dwColorOp);

	//	g_pD3dDev->ApplyStateBlock( pEffect->m_dwStateBlock );
	g_pD3dDev->SetRenderState(D3DRS_ZENABLE, pEffect->m_pParent->m_bZbufferEnable);
	if (pEffect->m_pParent->m_bZWriteEnable == FALSE)
	{
		g_pD3dDev->SetRenderState(D3DRS_ZWRITEENABLE, pEffect->m_pParent->m_bZWriteEnable);
	}
	g_pD3dDev->SetRenderState(D3DRS_LIGHTING, FALSE);
	g_pD3dDev->SetRenderState(D3DRS_ALPHABLENDENABLE, pEffect->m_pParent->GetAlphaValue());
	g_pD3dDev->SetRenderState(D3DRS_SRCBLEND, pEffect->m_pParent->m_dwSrcBlend);
	g_pD3dDev->SetRenderState(D3DRS_DESTBLEND, pEffect->m_pParent->m_dwDestBlend);
	g_pD3dDev->SetTextureStageState(0, D3DTSS_COLOROP, pEffect->m_pParent->m_nTextureRenderState);
	g_pD3dDev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	//	g_pD3dDev->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1 );
	//	g_pD3dDev->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_DISABLE );
	// 2005-01-03 by jschoi
	//    g_pD3dDev->SetTextureStageState( 0, D3DTSS_MAGFILTER, D3DTEXF_LINEAR  );
	//    g_pD3dDev->SetTextureStageState( 0, D3DTSS_MINFILTER, D3DTEXF_LINEAR  );
	//	g_pD3dDev->SetSamplerState(0,D3DSAMP_MINFILTER,D3DTEXF_LINEAR);
	//	g_pD3dDev->SetSamplerState(0,D3DSAMP_MAGFILTER,D3DTEXF_LINEAR);

	D3DXMATRIX mat;
	D3DXMatrixIdentity(&mat);
	g_pD3dDev->SetTransform(D3DTS_WORLD, &mat);
	g_pD3dDev->SetFVF(D3DFVF_SPRITE_VERTEX);
	if (index >= 0)
	{
		if (pEffect->m_pParent &&
			pEffect->m_pParent->m_pParent &&
			pEffect->m_pParent->m_pParent->m_pTexture)
		{
			g_pD3dDev->SetTexture(0, pEffect->m_pParent->m_pParent->m_pTexture);
		}
		else
		{
			g_pD3dDev->SetTexture(0, m_pTexture[index]);
		}
		if (m_nTextureRenderCount[index] == 2)
		{
			m_nTextureRenderCount[index]++;
		}
	}

	//	g_pD3dDev->SetTexture( 0, NULL);
	D3DMATERIAL9 mtrl;
	D3DUtil_InitMaterial(mtrl, 1.0f, 1.0f, 1.0f, 1.0f);
	g_pD3dDev->SetMaterial(&mtrl);
	pEffect->Render();
	g_pD3dDev->SetRenderState(D3DRS_SRCBLEND, dwSrc);
	g_pD3dDev->SetRenderState(D3DRS_DESTBLEND, dwDest);
	g_pD3dDev->SetTextureStageState(0, D3DTSS_COLOROP, dwColorOp);
	g_pD3dDev->SetRenderState(D3DRS_LIGHTING, TRUE);

	if (pEffect->m_pParent->m_bZWriteEnable == FALSE)
	{
		g_pD3dDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	}
}

void CEffectRender::TraceAniRender(CTraceAni* pEffect)
{
	FLOG("CEffectRender::TraceAniRender( CTraceAni* pEffect )");

	for (int i = 0; i < pEffect->m_nNumberOfTrace; i++)
	{
		if (pEffect->m_pEffectPlane[i])
		{
			EffectPlaneRender(pEffect->m_pEffectPlane[i]);
		}
	}

	/*	if(pEffect->m_nCurrentNumberOfTrace<=0)
			return;
			int index = LoadTexture(pEffect->m_strTextureName[pEffect->m_nCurrentTextureNumber]);

			DWORD dwSrc,dwDest,dwColorOp;
			g_pD3dDev->GetRenderState(D3DRS_SRCBLEND,&dwSrc);
			g_pD3dDev->GetRenderState(D3DRS_DESTBLEND,&dwDest);
			g_pD3dDev->GetTextureStageState(0,D3DTSS_COLOROP,&dwColorOp);

			//	g_pD3dDev->ApplyStateBlock( pEffect->m_dwStateBlock );
			g_pD3dDev->SetRenderState( D3DRS_ZENABLE, pEffect->m_bZbufferEnable );
			g_pD3dDev->SetRenderState( D3DRS_LIGHTING, TRUE );
			g_pD3dDev->SetRenderState( D3DRS_ALPHABLENDENABLE, pEffect->m_bAlphaBlendEnable );
			g_pD3dDev->SetRenderState( D3DRS_SRCBLEND, pEffect->m_dwSrcBlend );
			g_pD3dDev->SetRenderState( D3DRS_DESTBLEND, pEffect->m_dwDestBlend );
			g_pD3dDev->SetTextureStageState(0,D3DTSS_COLOROP,pEffect->m_nTextureRenderState);
			g_pD3dDev->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
			//	g_pD3dDev->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1 );
			//	g_pD3dDev->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_DISABLE );
			g_pD3dDev->SetTextureStageState( 0, D3DTSS_MAGFILTER, D3DTEXF_LINEAR  );
			g_pD3dDev->SetTextureStageState( 0, D3DTSS_MINFILTER, D3DTEXF_LINEAR  );

			D3DXMATRIX mat;
			D3DXMatrixIdentity( &mat);
			g_pD3dDev->SetTransform( D3DTS_WORLD, &mat );
			g_pD3dDev->SetVertexShader( D3DFVF_SPRITE_VERTEX );
			if(index>=0)
			{
			if(pEffect->m_pParent && pEffect->m_pParent->m_pTexture)
			{
			g_pD3dDev->SetTexture( 0, pEffect->m_pParent->m_pTexture);
			}
			else
			{
			g_pD3dDev->SetTexture( 0, m_pTexture[index]);
			}
			if(m_nTextureRenderCount[index] == 2)
			m_nTextureRenderCount[index]++;
			}
			D3DMATERIAL8 mtrl;
			D3DUtil_InitMaterial(mtrl, 1.0f,1.0f,1.0f, 1.0f);
			g_pD3dDev->SetMaterial( &mtrl );
			g_pD3dDev->SetStreamSource( 0, pEffect->m_pVBTrace[0], sizeof(SPRITE_VERTEX) );
			g_pD3dDev->DrawPrimitive( D3DPT_TRIANGLESTRIP , 0,pEffect->m_nCurrentNumberOfTrace*2 );
			g_pD3dDev->SetStreamSource( 0, pEffect->m_pVBTrace[1], sizeof(SPRITE_VERTEX) );
			g_pD3dDev->DrawPrimitive( D3DPT_TRIANGLESTRIP , 0,pEffect->m_nCurrentNumberOfTrace*2 );
			g_pD3dDev->SetRenderState(D3DRS_SRCBLEND,dwSrc);
			g_pD3dDev->SetRenderState(D3DRS_DESTBLEND,dwDest);
			g_pD3dDev->SetTextureStageState(0,D3DTSS_COLOROP,dwColorOp);
			*/
}


void CEffectRender::ObjectAniRender(CObjectAni* pEffect, BOOL bAlpha, int nAlphaValue)
{
	FLOG("CEffectRender::ObjectAniRender(CObjectAni* pEffect)");
	if (strlen(pEffect->m_strName) == 0)
	{
		return;
	}
	DWORD dwSrc, dwDest, dwColorOp;
	DWORD dwFogValue = FALSE;
	g_pD3dDev->GetRenderState(D3DRS_SRCBLEND, &dwSrc);
	g_pD3dDev->GetRenderState(D3DRS_DESTBLEND, &dwDest);
	g_pD3dDev->GetTextureStageState(0, D3DTSS_COLOROP, &dwColorOp);

	g_pD3dDev->SetRenderState(D3DRS_ALPHATESTENABLE, pEffect->m_bAlphaTestEnble);
	if (pEffect->m_bAlphaTestEnble)
	{
		g_pD3dDev->SetRenderState(D3DRS_ALPHAREF, pEffect->m_nAlphaTestValue);
		g_pD3dDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
	}

	g_pD3dDev->SetRenderState(D3DRS_LIGHTING, TRUE);

	//	g_pD3dDev->SetRenderState( D3DRS_ZENABLE, TRUE);
	// 2008-07-30 by dgwoo 아겨 특정아머 부스터 사용시 이펙트가 아머뒤에 있을경우에도 보인다.
	// 2008-10-22 by bhsohn 부스터 쓰면서 공폭 사용시, 네모난 텍스처 생기는 버그 수정
	g_pD3dDev->SetRenderState(D3DRS_ZENABLE, pEffect->m_bZbufferEnable);
	//g_pD3dDev->SetRenderState( D3DRS_ZENABLE, TRUE);

	if (pEffect->m_bZWriteEnable == FALSE)
	{
		g_pD3dDev->SetRenderState(D3DRS_ZWRITEENABLE, pEffect->m_bZWriteEnable);
	}
	g_pD3dDev->SetRenderState(D3DRS_ALPHABLENDENABLE, pEffect->m_bAlphaBlending);
	if (pEffect->m_bAlphaBlending)
	{
		g_pD3dDev->GetRenderState(D3DRS_FOGENABLE, &dwFogValue);
		if (pEffect->m_nSrcBlend == D3DBLEND_ONE && pEffect->m_nDestBlend == D3DBLEND_ONE && dwFogValue == TRUE)
		{
			g_pD3dDev->SetRenderState(D3DRS_FOGENABLE, FALSE);
		}
		g_pD3dDev->SetRenderState(D3DRS_SRCBLEND, pEffect->m_nSrcBlend);
		g_pD3dDev->SetRenderState(D3DRS_DESTBLEND, pEffect->m_nDestBlend);
	}
	g_pD3dDev->SetTextureStageState(0, D3DTSS_COLOROP, pEffect->m_nTextureRenderState);
	g_pD3dDev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);

	//*--------------------------------------------------------------------------*//
	// 2006-11-16 by ispark, 알파
	if (bAlpha)
	{
		g_pD3dApp->SetAlphaRenderState(nAlphaValue);
	}
	//*--------------------------------------------------------------------------*//

	D3DXMATRIX matScale;
	D3DXVECTOR3 pos;
	if (pEffect->m_pParent)
	{
		pos = pEffect->m_pParent->m_vPos;//상대 좌표
	}
	else
	{
		pos = D3DXVECTOR3(0, 0, 0);
	}
	// 2010. 03. 18 by jskim 몬스터변신 카드
	float tempScale = 0.0f;
	if (pEffect->m_pParent->m_pParent->m_MonsterTransformer &&
		pEffect->m_pParent->m_pParent->m_MonsterTransScale > 0)
	{
		tempScale = pEffect->m_fScale * pEffect->m_pParent->m_pParent->m_MonsterTransScale;
	}
	else
	{
		tempScale = pEffect->m_fScale;
	}
	//D3DXMatrixScaling(&matScale, pEffect->m_fScale,pEffect->m_fScale,pEffect->m_fScale);
	D3DXMatrixScaling(&matScale, tempScale, tempScale, tempScale);
	//end 2010. 03. 18 by jskim 몬스터변신 카드
	if (pEffect->m_pParent && pEffect->m_pParent->m_bUseBillboard)
	{
		if (pEffect->m_pParent->m_bGroundBillboard == TRUE)
		{
			D3DXVECTOR3 vPos(pEffect->m_pParent->m_pParent->m_mMatrix._41,
			                 pEffect->m_pParent->m_pParent->m_mMatrix._42,
			                 pEffect->m_pParent->m_pParent->m_mMatrix._43);
			D3DXMATRIX matTemp;
			D3DXVECTOR3 vVel = g_pD3dApp->m_pCamera->GetEyePt() - vPos;
			D3DXVECTOR3 vUp(0, 1, 0);
			if (vVel == vUp)
			{
				vVel = D3DXVECTOR3(1, 0, 0);
			}
			else
			{
				D3DXVec3Cross(&vVel, &vVel, &vUp);
				D3DXVec3Cross(&vVel, &vVel, &vUp);
			}
			D3DXVec3Normalize(&vVel, &vVel);
			D3DXMatrixLookAtLH(&matTemp, &(vPos), &(vPos + vVel), &vUp);
			D3DXMatrixInverse(&matTemp, NULL, &matTemp);
			matScale = matTemp * matScale;
		}
		else
		{
			D3DXMATRIX matBillboard = g_pD3dApp->m_pCamera->GetBillboardMatrix();
			D3DXMATRIX matRotate;
			D3DXVECTOR3 vAxis = g_pD3dApp->m_pCamera->GetViewDir();//pos - g_pD3dApp->m_pCamera->GetEyePt();
			D3DXMatrixRotationAxis(&matRotate, &vAxis, pEffect->m_pParent->m_fBillboardAngle);
			matBillboard *= matRotate;

			if (pEffect->m_pParent->m_fBillboardRotatePerSec > 0)
			{
				D3DXMatrixRotationAxis(&matRotate, &vAxis, pEffect->m_fCurrentBillboardRotateAngle);
				matBillboard *= matRotate;
			}
			if (pEffect->m_fCurrentRandomUpAngleX != 0)
			{
				D3DXMatrixRotationAxis(&matRotate, &vAxis, pEffect->m_fCurrentRandomUpAngleX);
				matBillboard *= matRotate;
			}
			matScale._41 = pEffect->m_pParent->m_vPos.x;
			matScale._42 = pEffect->m_pParent->m_vPos.y;
			matScale._43 = pEffect->m_pParent->m_vPos.z;

			matBillboard = matScale * matBillboard;

			matScale = matScale * pEffect->m_pParent->m_pParent->m_mMatrix;
			matBillboard._41 = matScale._41;
			matBillboard._42 = matScale._42;
			matBillboard._43 = matScale._43;
			matScale = matBillboard;
		}
		// set light
		if (!pEffect->m_bUseEnvironmentLight)
		{
			m_light2 = g_pD3dApp->m_pScene->m_light2;
			m_light2.Direction = g_pD3dApp->m_pCamera->GetViewDir();
			g_pD3dDev->SetLight(2, &m_light2);
			g_pD3dDev->LightEnable(2, TRUE);
		}
	}
	else
	{
		D3DXMATRIX lookat;
		D3DXVECTOR3 up = pEffect->m_pParent->m_vUp;
		D3DXVECTOR3 target = pEffect->m_pParent->m_vTarget;
		if (pEffect->m_fCurrentRandomUpAngleX != 0 || pEffect->m_fCurrentRandomUpAngleZ != 0)
		{
			up = D3DXVECTOR3(0, 1, 0);
			target = pEffect->m_pParent->m_vPos;
			D3DXVECTOR3 vel = D3DXVECTOR3(0, 0, 1);
			D3DXMATRIX matRotateX, matRotateZ;
			D3DXVECTOR3 vAxis(1, 0, 0);// up vector 회전축(X축)
			D3DXMatrixRotationAxis(&matRotateX, &vAxis, pEffect->m_fCurrentRandomUpAngleX);
			D3DXVec3TransformCoord(&up, &up, &matRotateX);
			D3DXVec3TransformCoord(&vel, &vel, &matRotateX);
			vAxis = D3DXVECTOR3(0, 0, 1);// up vector 회전축(X축)
			D3DXMatrixRotationAxis(&matRotateZ, &vAxis, pEffect->m_fCurrentRandomUpAngleZ);
			D3DXVec3TransformCoord(&up, &up, &matRotateZ);
			D3DXVec3TransformCoord(&vel, &vel, &matRotateZ);
			target += vel;
		}
		D3DXMatrixLookAtLH(&lookat, &pos, &target, &up);
		D3DXMatrixInverse(&lookat, NULL, &lookat);
		matScale = lookat * matScale;// * matScale;
		D3DXMATRIX mat = pEffect->m_pParent->m_pParent->m_mMatrix;
		mat._41 = 0;
		mat._42 = 0;
		mat._43 = 0;
		matScale *= mat;
		matScale._41 += pEffect->m_pParent->m_pParent->m_mMatrix._41;
		matScale._42 += pEffect->m_pParent->m_pParent->m_mMatrix._42;
		matScale._43 += pEffect->m_pParent->m_pParent->m_mMatrix._43;

		// set light
		if (!pEffect->m_bUseEnvironmentLight)
		{
			m_light2 = g_pD3dApp->m_pScene->m_light2;
			m_light2.Direction = g_pD3dApp->m_pCamera->GetViewDir();
			g_pD3dDev->SetLight(2, &m_light2);
			g_pD3dDev->LightEnable(2, TRUE);
		}
	}
	CSkinnedMesh* pMesh = LoadObject(pEffect->m_strObjectFile);
	if (!pMesh)
	{
		if (!pEffect->m_bUseEnvironmentLight)
		{
			g_pD3dDev->LightEnable(2, FALSE);
		}
		if (pEffect->m_nSrcBlend == D3DBLEND_ONE && pEffect->m_nDestBlend == D3DBLEND_ONE && dwFogValue == TRUE)
		{
			g_pD3dDev->SetRenderState(D3DRS_FOGENABLE, dwFogValue);
		}
		if (pEffect->m_bZWriteEnable == FALSE)
		{
			g_pD3dDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
		}
		return;
	}
	switch (pEffect->m_nObjectAniType)
	{
	case 0: // object animation
		{ //m_pObjEffectMesh[index]
			pMesh->Tick(pEffect->m_fCurrentObjectAniTime);
			pMesh->AnotherTexture(1);
		}
		break;
	case 1: // texture animation
		{
			pMesh->AnotherTexture(pEffect->m_nCurrentTextureType + 1);
		}
		break;
	case 2: // object + texture animation
		{
			pMesh->Tick(pEffect->m_fCurrentObjectAniTime);
			pMesh->AnotherTexture(pEffect->m_nCurrentTextureType + 1);
		}
		break;
	}

	D3DMATERIAL9 mtrl;
	D3DUtil_InitMaterial(mtrl, pEffect->m_cColor.r,
	                     pEffect->m_cColor.g, pEffect->m_cColor.b, pEffect->m_cColor.a);
	BOOL bTemp = pMesh->m_bMaterial;
	pMesh->m_bMaterial = TRUE;
	pMesh->m_material = mtrl;
	//	절대 좌표 세팅
	pMesh->SetWorldMatrix(matScale);//*pEffect->m_pParent->m_pParent->m_mMatrix);

	if (!pEffect->m_bZbufferEnable && m_bZBufferTemp)
	{
		D3DXVECTOR3 vTempPos;
		vTempPos.x = matScale._41;
		vTempPos.y = matScale._42;
		vTempPos.z = matScale._43;
		D3DXMATRIX matTemp;
		D3DXVECTOR3 vPos, vVel, vUp, vSide;
		vPos = g_pD3dApp->m_pCamera->GetEyePt();
		D3DXVec3Normalize(&vUp, &g_pD3dApp->m_pCamera->GetUpVec());
		D3DXVec3Normalize(&vVel, &(vTempPos - vPos));
		D3DXVec3Cross(&vSide, &vUp, &vVel);
		D3DXVec3Cross(&vUp, &vVel, &vSide);

		D3DXMatrixLookAtLH(&matTemp, &vPos, &(vPos + vVel), &vUp);
		float fDist1, fDist2;
		fDist1 = D3DXVec3Length(&(vTempPos - vPos));
		fDist2 = g_pD3dApp->m_pScene->m_pObjectRender->CheckPickMesh(matTemp, vPos).fDist;
		if (fDist1 > fDist2)
			g_pD3dDev->SetRenderState(D3DRS_ZENABLE, TRUE);
		else
			g_pD3dDev->SetRenderState(D3DRS_ZENABLE, FALSE);
	}
	// by dhkwon, 030917
	LPDIRECT3DTEXTURE9 pOrgTex = NULL;
	if (pEffect->m_pParent && pEffect->m_pParent->m_pTexture)
	{
		pOrgTex = pMesh->SetTexture(pEffect->m_pParent->m_pTexture, 0);
	}
	else if (pEffect->m_strTextureFile[0])
	{
		int i = LoadTexture(pEffect->m_strTextureFile);
		if (m_nTextureRenderCount[i] == 2)
			m_nTextureRenderCount[i]++;
		pOrgTex = pMesh->SetTexture(m_pTexture[i], 0);
	}
	DWORD dwLightColorOp = 0;
	if (pEffect->m_bLightMapUse)
	{
		g_pD3dDev->GetTextureStageState(1, D3DTSS_COLOROP, &dwLightColorOp);
		g_pD3dDev->SetTextureStageState(1, D3DTSS_COLOROP, pEffect->m_nLightMapRenderState);
		g_pD3dDev->SetTextureStageState(1, D3DTSS_TEXCOORDINDEX, 0);

		g_pD3dDev->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		g_pD3dDev->SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_CURRENT);
	}

	// 2006-01-24 by ispark
	if ((!g_pD3dApp->m_bCharacter && // 기어 이면서
			g_pD3dApp->m_pCamera->GetCamType() != CAMERA_TYPE_FPS && //  1인칭 모드가 아니구
			pEffect->m_pParent->m_nInvenWeaponIndex > 0) || // 인벤 무기 인덱스가 0 이상이면 렌더링 하지마라
		pEffect->m_pParent->m_nInvenWeaponIndex == 0) // 인벤 무기 인덱스가 0이면 렌더링 
	{
		if (pEffect->m_bLightMapUse)
		{
			pMesh->Render(TRUE);
		}
		else
		{
			pMesh->Render(); // 2016-01-08 exception at this point
		}
	}

	// 2008-08-22 by bhsohn EP3 인벤토리 처리	
	//if(pEffect->m_pParent->m_nInvenWeaponIndex > 0	&&  g_pShuttleChild->m_bInvenRender == TRUE)

	// 2012-06-14 by isshin 아템미리보기
#ifdef C_ENEMY_INFO_ISSHIN
	if ((pEffect->m_pParent->m_nInvenWeaponIndex > 0 && pEffect->m_pParent->m_nInvenWeaponIndex < ENEMY_WEAPON_INDEX_DUMMY) && g_pGameMain->IsEquipInvenShow() == TRUE)
#else
	if (pEffect->m_pParent->m_nInvenWeaponIndex > 0 && g_pGameMain->IsEquipInvenShow() == TRUE)
#endif
	// end 2012-06-14 by isshin 아템미리보기
	{
		D3DXMATRIX pMatOldView, pMatOldProj, pMatPresView, pMatPresProj, pMatrix;
		D3DXMatrixIdentity(&pMatOldView);
		D3DXMatrixIdentity(&pMatOldProj);
		D3DXMatrixIdentity(&pMatPresView);
		D3DXMatrixIdentity(&pMatPresProj);
		D3DXMatrixIdentity(&pMatrix);

		// 현재 뷰와 프로젝션을 가져온다
		g_pD3dDev->GetTransform(D3DTS_VIEW, &pMatOldView);
		g_pD3dDev->GetTransform(D3DTS_PROJECTION, &pMatOldProj);

		g_pD3dDev->SetTransform(D3DTS_VIEW, &pMatPresView);
		g_pD3dDev->SetTransform(D3DTS_PROJECTION, &pMatPresProj);
		g_pD3dDev->SetRenderState(D3DRS_LIGHTING, FALSE);

		if (pEffect->m_pParent->m_nInvenWeaponIndex > 4)
		{
			// 2008-08-22 by bhsohn EP3 인벤토리 처리
			//pMesh->SetWorldMatrix(g_pShuttleChild->m_pMatInven);
			pMesh->SetWorldMatrix(g_pGameMain->GetInvenMatInven());
			pMesh->Render();
		}
		else
			g_pShuttleChild->SetInvenMesh(pEffect->m_pParent->m_nInvenWeaponIndex - 1, pMesh);
		//			pMesh->SetWorldMatrix(g_pShuttleChild->m_pMatInvenWeaponSetPosition[pEffect->m_pParent->m_nInvenWeaponIndex - 1]);
		//		pMesh->Render();

		g_pD3dDev->SetTransform(D3DTS_VIEW, &pMatOldView);
		g_pD3dDev->SetTransform(D3DTS_PROJECTION, &pMatOldProj);
		g_pD3dDev->SetRenderState(D3DRS_LIGHTING, TRUE);
	}


	//	if(g_pShuttleChild->m_bInvenRender && bInven == TRUE && pEffect->m_pParent->m_nInvenWeaponIndex > 0)
	//	{
	//		//////////////////////////////////////////////////////////////////////////
	//		//
	//		D3DXMATRIX pMatOldView, pMatOldProj, pMatPresView, pMatPresProj, pMatrix;
	//		D3DXMatrixIdentity(&pMatOldView);
	//		D3DXMatrixIdentity(&pMatOldProj);
	//		D3DXMatrixIdentity(&pMatPresView);
	//		D3DXMatrixIdentity(&pMatPresProj);
	//		D3DXMatrixIdentity(&pMatrix);		
	//
	//		// 현재 뷰와 프로젝션을 가져온다
	//		g_pD3dDev->GetTransform( D3DTS_VIEW,       &pMatOldView );
	//		g_pD3dDev->GetTransform( D3DTS_PROJECTION, &pMatOldProj );	
	//		
	//		g_pD3dDev->SetTransform( D3DTS_VIEW,		&pMatPresView);
	//		g_pD3dDev->SetTransform( D3DTS_PROJECTION,	&pMatPresProj);
	//		g_pD3dDev->SetRenderState( D3DRS_LIGHTING, FALSE );
	//		
	//		// 무기
	//		if(pEffect->m_pParent->m_nInvenWeaponIndex > 4)
	//		{
	//			pMesh->SetWorldMatrix(g_pShuttleChild->m_pMatInven);
	//			if(pEffect->m_bLightMapUse)	
	//				pMesh->Render(TRUE);
	//			else 
	//				pMesh->Render();	
	//		}
	//		else
	//			g_pShuttleChild->SetInvenMesh(pEffect->m_pParent->m_nInvenWeaponIndex - 1, pMesh);
	////			pMesh->SetWorldMatrix(g_pShuttleChild->m_pMatInvenWeaponSetPosition[pEffect->m_pParent->m_nInvenWeaponIndex - 1]);
	////		if(pEffect->m_bLightMapUse)	
	////			pMesh->Render(TRUE);
	////		else 
	////			pMesh->Render();	
	//		// 복구
	//		g_pD3dDev->SetTransform( D3DTS_VIEW,		&pMatOldView );
	//		g_pD3dDev->SetTransform( D3DTS_PROJECTION,	&pMatOldProj );
	//		g_pD3dDev->SetRenderState( D3DRS_LIGHTING, TRUE );
	//		//
	//		//////////////////////////////////////////////////////////////////////////		
	//	}		

	if ((pEffect->m_pParent && pEffect->m_pParent->m_pTexture) || pEffect->m_strTextureFile[0])
	{
		pMesh->SetTexture(pOrgTex, 0);
	}

	if (pMesh && pMesh->m_nRenderCount == 2)
		pMesh->m_nRenderCount++;
	if (pMesh)
		pMesh->m_bMaterial = bTemp;
	g_pD3dDev->SetRenderState(D3DRS_SRCBLEND, dwSrc);
	g_pD3dDev->SetRenderState(D3DRS_DESTBLEND, dwDest);
	g_pD3dDev->SetTextureStageState(0, D3DTSS_COLOROP, dwColorOp);
	if (!pEffect->m_bUseEnvironmentLight)
	{
		g_pD3dDev->LightEnable(2, FALSE);
	}
	if (pEffect->m_bLightMapUse)
	{
		g_pD3dDev->SetTextureStageState(1, D3DTSS_COLOROP, dwLightColorOp);
	}
	if (pEffect->m_nSrcBlend == D3DBLEND_ONE && pEffect->m_nDestBlend == D3DBLEND_ONE && dwFogValue == TRUE)
	{
		g_pD3dDev->SetRenderState(D3DRS_FOGENABLE, dwFogValue);
	}
	if (pEffect->m_bAlphaTestEnble)
	{
		g_pD3dDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	}
	if (pEffect->m_bZWriteEnable == FALSE)
	{
		g_pD3dDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	}
}

void CEffectRender::ObjectParticleRender(CObjectAni* pEffect, CParticle* pParticle)
{
	FLOG("CEffectRender::ObjectParticleRender(CObjectAni* pEffect, CParticle* pParticle)");
	DWORD dwSrc, dwDest, dwColorOp;
	DWORD dwFogValue = FALSE;
	g_pD3dDev->GetRenderState(D3DRS_SRCBLEND, &dwSrc);
	g_pD3dDev->GetRenderState(D3DRS_DESTBLEND, &dwDest);
	g_pD3dDev->GetTextureStageState(0, D3DTSS_COLOROP, &dwColorOp);

	g_pD3dDev->SetRenderState(D3DRS_ALPHATESTENABLE, pEffect->m_bAlphaTestEnble);
	if (pEffect->m_bAlphaTestEnble)
	{
		g_pD3dDev->SetRenderState(D3DRS_ALPHAREF, pEffect->m_nAlphaTestValue);
		g_pD3dDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
	}

	g_pD3dDev->SetRenderState(D3DRS_LIGHTING, TRUE);

	//	g_pD3dDev->SetRenderState( D3DRS_ZENABLE, TRUE);
	g_pD3dDev->SetRenderState(D3DRS_ZENABLE, pEffect->m_bZbufferEnable);

	if (pEffect->m_bZWriteEnable == FALSE)
	{
		g_pD3dDev->SetRenderState(D3DRS_ZWRITEENABLE, pEffect->m_bZWriteEnable);
	}
	g_pD3dDev->SetRenderState(D3DRS_ALPHABLENDENABLE, pEffect->m_bAlphaBlending);
	if (pEffect->m_bAlphaBlending)
	{
		g_pD3dDev->GetRenderState(D3DRS_FOGENABLE, &dwFogValue);
		if (pEffect->m_nSrcBlend == D3DBLEND_ONE && pEffect->m_nDestBlend == D3DBLEND_ONE && dwFogValue == TRUE)
		{
			g_pD3dDev->SetRenderState(D3DRS_FOGENABLE, FALSE);
		}
		g_pD3dDev->SetRenderState(D3DRS_SRCBLEND, pEffect->m_nSrcBlend);
		g_pD3dDev->SetRenderState(D3DRS_DESTBLEND, pEffect->m_nDestBlend);
	}
	g_pD3dDev->SetTextureStageState(0, D3DTSS_COLOROP, pEffect->m_nTextureRenderState);
	g_pD3dDev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	//	g_pD3dDev->SetTextureStageState( 0, D3DTSS_MAGFILTER, D3DTEXF_LINEAR  );
	//	g_pD3dDev->SetTextureStageState( 0, D3DTSS_MINFILTER, D3DTEXF_LINEAR  );
	D3DXMATRIX matScale;
	D3DXVECTOR3 pos;
	if (pParticle)
	{
		//		pos = pData->m_pParent->m_vPos;
		pos = pParticle->m_vPos;//좌표
	}
	else
	{
		pos = D3DXVECTOR3(0, 0, 0);
	}
	// 2010. 03. 18 by jskim 몬스터변신 카드
	//D3DXMatrixScaling(&matScale, pEffect->m_fScale,pEffect->m_fScale,pEffect->m_fScale);
	float tempScale = 0.0f;
	if (pEffect->m_pParent->m_pParent->m_MonsterTransformer &&
		pEffect->m_pParent->m_pParent->m_MonsterTransScale > 0)
	{
		tempScale = pEffect->m_fScale * pEffect->m_pParent->m_pParent->m_MonsterTransScale;
	}
	else
	{
		tempScale = pEffect->m_fScale;
	}

	D3DXMatrixScaling(&matScale, tempScale, tempScale, tempScale);
	//end 2010. 03. 18 by jskim 몬스터변신 카드
	if (pEffect->m_pParent && pEffect->m_pParent->m_bUseBillboard)
	{
		/*		matScale *= g_pD3dApp->m_pCamera->GetBillboardMatrix();
				D3DXMATRIX matRotate;
				D3DXVECTOR3 vAxis = g_pD3dApp->m_pCamera->GetViewDir();//pos - g_pD3dApp->m_pCamera->GetEyePt();
				D3DXMatrixRotationAxis( &matRotate, &vAxis, pEffect->m_pParent->m_fBillboardAngle );
				matScale *= matRotate;

				if(pEffect->m_pParent->m_fBillboardRotatePerSec>0)
				{
				D3DXMatrixRotationAxis( &matRotate, &vAxis, pEffect->m_fCurrentBillboardRotateAngle );
				matScale *= matRotate;
				}
				if(pEffect->m_fCurrentRandomUpAngleX != 0)
				{
				D3DXMatrixRotationAxis( &matRotate, &vAxis, pEffect->m_fCurrentRandomUpAngleX );
				matScale *= matRotate;
				}
				*/
		D3DXMATRIX matBillboard = g_pD3dApp->m_pCamera->GetBillboardMatrix();
		D3DXMATRIX matRotate;
		D3DXVECTOR3 vAxis = g_pD3dApp->m_pCamera->GetViewDir();//pos - g_pD3dApp->m_pCamera->GetEyePt();
		D3DXMatrixRotationAxis(&matRotate, &vAxis, pEffect->m_pParent->m_fBillboardAngle);
		matBillboard *= matRotate;

		if (pEffect->m_pParent->m_fBillboardRotatePerSec > 0)
		{
			D3DXMatrixRotationAxis(&matRotate, &vAxis, pEffect->m_fCurrentBillboardRotateAngle);
			matBillboard *= matRotate;
		}
		if (pEffect->m_fCurrentRandomUpAngleX != 0)
		{
			D3DXMatrixRotationAxis(&matRotate, &vAxis, pEffect->m_fCurrentRandomUpAngleX);
			matBillboard *= matRotate;
		}
		/*
				matScale._41 = pEffect->m_pParent->m_pParent->m_mMatrix._41;
				matScale._42 = pEffect->m_pParent->m_pParent->m_mMatrix._42;
				matScale._43 = pEffect->m_pParent->m_pParent->m_mMatrix._43;
	
				matScale._41 += pEffect->m_pParent->m_vPos.x;
				matScale._42 += pEffect->m_pParent->m_vPos.y;
				matScale._43 += pEffect->m_pParent->m_vPos.z;
				*/
		matScale._41 = pEffect->m_pParent->m_vPos.x;
		matScale._42 = pEffect->m_pParent->m_vPos.y;
		matScale._43 = pEffect->m_pParent->m_vPos.z;
		matBillboard = matScale * matBillboard;
		matScale = matScale * pEffect->m_pParent->m_pParent->m_mMatrix;
		matBillboard._41 = matScale._41;
		matBillboard._42 = matScale._42;
		matBillboard._43 = matScale._43;
		matScale = matBillboard;
		// set light
		if (!pEffect->m_bUseEnvironmentLight)
		{
			m_light2 = g_pD3dApp->m_pScene->m_light2;
			m_light2.Direction = g_pD3dApp->m_pCamera->GetViewDir();
			g_pD3dDev->SetLight(2, &m_light2);
			g_pD3dDev->LightEnable(2, TRUE);
		}
	}
	else
	{
		D3DXVECTOR3 up, target;
		switch (pParticle->m_pParent->m_nObjMoveTargetType)
		{
		case OBJ_MOVE_TYPE0: //파티클 이동방향을 가리킴
			{
				target = pParticle->m_vObjTarget;
				up = pParticle->m_vObjUp;
			}
			break;
		case OBJ_MOVE_TYPE1: // 유닛 방향으로 가리킴
			{
				target = pEffect->m_pParent ? pEffect->m_pParent->m_vTarget : D3DXVECTOR3(0, 0, 1);
				up = pEffect->m_pParent ? pEffect->m_pParent->m_vUp : D3DXVECTOR3(0, 1, 0);
			}
			break;
		case OBJ_MOVE_TYPE2: // 랜덤으로 움직임
			{
				target = pParticle->m_vObjTarget;
				up = pParticle->m_vObjUp;
				//				target.x = Random(1.0f, -1.0f);
				//				target.y = Random(1.0f, -1.0f);
				//				target.z = Random(1.0f, -1.0f);
				//				D3DXVec3Normalize(&target,&target);
				//				up = pEffect->m_pParent ?  pEffect->m_pParent->m_vUp : D3DXVECTOR3(0,1,0);
			}
			break;
		case OBJ_MOVE_TYPE3: // 빌보드
			{
				target = pParticle->m_vPos - g_pD3dApp->m_pCamera->GetViewDir();
				up = g_pD3dApp->m_pCamera->GetUpVec();
			}
			break;
		}
		D3DXMATRIX lookat;
		//		D3DXVECTOR3 up = pEffect->m_pParent->m_vUp;
		//		D3DXVECTOR3 target = pEffect->m_pParent->m_vTarget;
		if (pEffect->m_fCurrentRandomUpAngleX != 0 || pEffect->m_fCurrentRandomUpAngleZ != 0)
		{
			up = D3DXVECTOR3(0, 1, 0);
			target = pParticle->m_vPos;
			D3DXVECTOR3 vel = D3DXVECTOR3(0, 0, 1);
			D3DXMATRIX matRotateX, matRotateZ;
			D3DXVECTOR3 vAxis(1, 0, 0);// up vector 회전축(X축)
			D3DXMatrixRotationAxis(&matRotateX, &vAxis, pEffect->m_fCurrentRandomUpAngleX);
			D3DXVec3TransformCoord(&up, &up, &matRotateX);
			D3DXVec3TransformCoord(&vel, &vel, &matRotateX);
			vAxis = D3DXVECTOR3(0, 0, 1);// up vector 회전축(X축)
			D3DXMatrixRotationAxis(&matRotateZ, &vAxis, pEffect->m_fCurrentRandomUpAngleZ);
			D3DXVec3TransformCoord(&up, &up, &matRotateZ);
			D3DXVec3TransformCoord(&vel, &vel, &matRotateZ);
			target += vel;
		}

		D3DXMatrixLookAtLH(&lookat, &D3DXVECTOR3(0, 0, 0), &target, &up);
		//		D3DXMatrixLookAtLH( &lookat, &pos, &target, &up);
		D3DXMatrixInverse(&lookat, NULL, &lookat);
		matScale = lookat * matScale;// * matScale;
		D3DXMATRIX mat = pEffect->m_pParent->m_pParent->m_mMatrix;
		mat._41 = pos.x;
		mat._42 = pos.y;
		mat._43 = pos.z;
		matScale = matScale * mat;
		// set light
		if (!pEffect->m_bUseEnvironmentLight)
		{
			m_light2 = g_pD3dApp->m_pScene->m_light2;
			m_light2.Direction = g_pD3dApp->m_pCamera->GetViewDir();
			g_pD3dDev->SetLight(2, &m_light2);
			g_pD3dDev->LightEnable(2, TRUE);
		}
	}

	//	int index = LoadObject(pEffect->m_strObjectFile);
	//	if(index<0)
	//		return;
	CSkinnedMesh* pMesh = LoadObject(pEffect->m_strObjectFile);
	if (!pMesh)
	{
		if (!pEffect->m_bUseEnvironmentLight)
		{
			g_pD3dDev->LightEnable(2, FALSE);
		}
		if (pEffect->m_nSrcBlend == D3DBLEND_ONE && pEffect->m_nDestBlend == D3DBLEND_ONE && dwFogValue == TRUE)
		{
			g_pD3dDev->SetRenderState(D3DRS_FOGENABLE, dwFogValue);
		}
		if (pEffect->m_bZWriteEnable == FALSE)
		{
			g_pD3dDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
		}
		return;
	}
	switch (pEffect->m_nObjectAniType)
	{
	case 0: // object animation
		{
			pMesh->Tick(pEffect->m_fCurrentObjectAniTime);
			pMesh->AnotherTexture(1);
		}
		break;
	case 1: // texture animation
		{
			pMesh->AnotherTexture(pEffect->m_nCurrentTextureType + 1);
		}
		break;
	case 2: // object + texture animation
		{
			pMesh->Tick(pEffect->m_fCurrentObjectAniTime);
			pMesh->AnotherTexture(pEffect->m_nCurrentTextureType + 1);
		}
		break;
	}

	D3DMATERIAL9 mtrl;
	D3DUtil_InitMaterial(mtrl, pEffect->m_cColor.r,
	                     pEffect->m_cColor.g, pEffect->m_cColor.b, pEffect->m_cColor.a);
	BOOL bTemp = pMesh->m_bMaterial;
	pMesh->m_bMaterial = TRUE;
	pMesh->m_material = mtrl;
	//	절대 좌표 세팅
	pMesh->SetWorldMatrix(matScale);//*pEffect->m_pParent->m_pParent->m_mMatrix);
	if (!pEffect->m_bZbufferEnable && m_bZBufferTemp)
	{
		D3DXVECTOR3 vTempPos;
		vTempPos.x = matScale._41;
		vTempPos.y = matScale._42;
		vTempPos.z = matScale._43;
		D3DXMATRIX matTemp;
		D3DXVECTOR3 vPos, vVel, vUp, vSide;
		vPos = g_pD3dApp->m_pCamera->GetEyePt();
		D3DXVec3Normalize(&vUp, &g_pD3dApp->m_pCamera->GetUpVec());
		D3DXVec3Normalize(&vVel, &(vTempPos - vPos));
		D3DXVec3Cross(&vSide, &vUp, &vVel);
		D3DXVec3Cross(&vUp, &vVel, &vSide);

		D3DXMatrixLookAtLH(&matTemp, &vPos, &(vPos + vVel), &vUp);
		float fDist1, fDist2;
		fDist1 = D3DXVec3Length(&(vTempPos - vPos));
		fDist2 = g_pD3dApp->m_pScene->m_pObjectRender->CheckPickMesh(matTemp, vPos).fDist;
		if (fDist1 > fDist2)
			g_pD3dDev->SetRenderState(D3DRS_ZENABLE, TRUE);
		else
			g_pD3dDev->SetRenderState(D3DRS_ZENABLE, FALSE);
	}
	// by dhkwon, 030917
	LPDIRECT3DTEXTURE9 pOrgTex = NULL;
	if (pEffect->m_pParent && pEffect->m_pParent->m_pTexture)
	{
		pOrgTex = pMesh->SetTexture(pEffect->m_pParent->m_pTexture, 0);
	}
	else if (pEffect->m_strTextureFile[0])
	{
		int i = LoadTexture(pEffect->m_strTextureFile);
		if (m_nTextureRenderCount[i] == 2)
			m_nTextureRenderCount[i]++;
		pOrgTex = pMesh->SetTexture(m_pTexture[i], 0);
	}
	DWORD dwLightColorOp = 0;
	if (pEffect->m_bLightMapUse)
	{
		g_pD3dDev->GetTextureStageState(1, D3DTSS_COLOROP, &dwLightColorOp);
		g_pD3dDev->SetTextureStageState(1, D3DTSS_COLOROP, pEffect->m_nLightMapRenderState);
		g_pD3dDev->SetTextureStageState(1, D3DTSS_TEXCOORDINDEX, 0);
		// 2005-01-04 by jschoi
		//		g_pD3dDev->SetTextureStageState( 1, D3DTSS_MAGFILTER, D3DTEXF_LINEAR );
		//		g_pD3dDev->SetSamplerState(1,D3DSAMP_MAGFILTER,D3DTEXF_LINEAR);

		g_pD3dDev->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		g_pD3dDev->SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_CURRENT);
		//		g_pD3dDev->SetTextureStageState( 1, D3DTSS_ALPHAOP,   D3DTOP_DISABLE );
	}
	if (pEffect->m_bLightMapUse)
	{
		pMesh->Render(TRUE);
		//		pMesh->Render();
	}
	else
	{
		pMesh->Render();
	}
	if ((pEffect->m_pParent && pEffect->m_pParent->m_pTexture) || pEffect->m_strTextureFile[0])
	{
		pMesh->SetTexture(pOrgTex, 0);
	}

	//	if(m_nObjRenderCount[index] < 3)
	//		m_nObjRenderCount[index]++;
	if (pMesh && pMesh->m_nRenderCount == 2)
		pMesh->m_nRenderCount++;
	/*	if(pEffect->m_bLightMapUse)
		{
		g_pD3dDev->SetRenderState( D3DRS_ALPHABLENDENABLE,  pEffect->m_bLightMapAlphaBlending );
		g_pD3dDev->SetRenderState(D3DRS_SRCBLEND,pEffect->m_nLightMapSrcBlend);
		g_pD3dDev->SetRenderState(D3DRS_DESTBLEND,pEffect->m_nLightMapDestBlend);
		g_pD3dDev->SetTextureStageState(0,D3DTSS_COLOROP,pEffect->m_nLightMapRenderState);
		// by dhkwon, 030917
		//		LPDIRECT3DTEXTURE8 pOrgTex = NULL;
		if(pEffect->m_strTextureFile[0])
		{
		int i = LoadTexture(pEffect->m_strTextureFile);
		if(m_nTextureRenderCount[i] == 2)
		m_nTextureRenderCount[i]++;
		pOrgTex = m_pObjEffectMesh[index]->SetTexture(m_pTexture[i], 0);
		}
		m_pObjEffectMesh[index]->Render();
		if(pEffect->m_strTextureFile[0])
		{
		m_pObjEffectMesh[index]->SetTexture(pOrgTex, 0);
		}
		}
		*/
	if (pMesh)
		pMesh->m_bMaterial = bTemp;
	g_pD3dDev->SetRenderState(D3DRS_SRCBLEND, dwSrc);
	g_pD3dDev->SetRenderState(D3DRS_DESTBLEND, dwDest);
	g_pD3dDev->SetTextureStageState(0, D3DTSS_COLOROP, dwColorOp);
	if (!pEffect->m_bUseEnvironmentLight)
	{
		g_pD3dDev->LightEnable(2, FALSE);
	}
	if (pEffect->m_bLightMapUse)
	{
		g_pD3dDev->SetTextureStageState(1, D3DTSS_COLOROP, dwLightColorOp);
	}
	if (pEffect->m_bAlphaTestEnble)
	{
		g_pD3dDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	}
	if (pEffect->m_bZWriteEnable == FALSE)
	{
		g_pD3dDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	}
}

#define DEBUG_COLOR		D3DCOLOR_ARGB(255,255,255,0)

void CEffectRender::RenderZEnable()
{
	FLOG("CEffectRender::RenderZEnable()");

	if (m_vecZEnableEffect.size() == 0) return;

	if (g_pD3dApp->m_dwGameState != _GAME && g_pD3dApp->m_dwGameState != _SELECT && g_pD3dApp->m_dwGameState != _CREATE &&
		g_pD3dApp->m_dwGameState != _LOGO && g_pD3dApp->m_dwGameState != _SHOP && g_pD3dApp->m_dwGameState != _CITY) // g_pD3dApp->m_dwGameState != _WAITING)
	{
		// DBGOUT_EFFECT("ZBufferEnable Effect Disable state , effect number : %d\n", m_vecZEnableEffect.size());

		ClearAllZEnableEffect();

		return;
	}

	// effect sorting
	// sort(m_vecZEnableEffect.begin(), m_vecZEnableEffect.end(), CompareEffect());

#ifdef _DBGOUT_EFFECT
	int linePixel = 14;
	int lineNumber = 0;
#endif

	// effect rendering
	for (auto pEffect : m_vecZEnableEffectObject)
	{
#ifdef _DBGOUT_EFFECT

		char buf[256];

		sprintf(buf, "[OBJECT][obj:%s][%8.2f,%8.2f,%8.2f][%8.2f]",
			((CObjectAni*)pEffect)->m_strObjectFile,
			((CObjectAni*)pEffect)->m_pParent->m_vPos.x,
			((CObjectAni*)pEffect)->m_pParent->m_vPos.y,
			((CObjectAni*)pEffect)->m_pParent->m_vPos.z,
			pEffect->fDistance);

		g_pD3dApp->m_pFontDebug->DrawText(10, linePixel*lineNumber++, DEBUG_COLOR, buf, 0);

#endif

		if (((CObjectAni*)pEffect)->m_pParent->m_nAlphaValue == SKILL_OBJECT_ALPHA_OTHER_INFLUENCE) continue;

		ObjectAniRender((CObjectAni*)pEffect); // 2016-01-08 exception at this point (2)
	}

	for (auto pEffect : m_vecZEnableEffectSprite)
	{
#ifdef _DBGOUT_EFFECT

		char buf[256];

		sprintf(buf, "[SPRITE][tex:%s][%8.2f,%8.2f,%8.2f][%8.2f]",
			((CSpriteAni*)pEffect)->m_strTextureFile,
			((CSpriteAni*)pEffect)->m_pParent->m_vPos.x,
			((CSpriteAni*)pEffect)->m_pParent->m_vPos.y,
			((CSpriteAni*)pEffect)->m_pParent->m_vPos.z,
			pEffect->fDistance);

		g_pD3dApp->m_pFontDebug->DrawText(10, linePixel*lineNumber++, DEBUG_COLOR, buf, 0);

#endif

		// 2006-12-06 by ispark, 알파값 SKILL_OBJECT_ALPHA_OTHER_INFLUENCE(0) 인것은 렌더링 안한다.
		if (((CSpriteAni*)pEffect)->m_pParent->m_nAlphaValue == SKILL_OBJECT_ALPHA_OTHER_INFLUENCE) continue;

		SpriteAniRender((CSpriteAni*)pEffect);
	}

	RenderAllTraceEffects();

	//Render object particles
	//DevX temporarily disabled this functionality, it is laggy
	//RenderAllObjectParticles();

	//for (auto system : m_vectorObjectParticles) ParticleSystemRender(system);

	//Render sprite particles
	RenderAllSpriteParticles();

	ClearAllZEnableEffect();
}

void CEffectRender::PushBackToZEnableEffect(Effect* pEffect)
{
	m_vecZEnableEffect.push_back(pEffect);

	switch (pEffect->dwType)
	{
	case EFFECT_TYPE_OBJECT:
		m_vecZEnableEffectObject.push_back(pEffect);
		break;
	case EFFECT_TYPE_SPRITE:
		m_vecZEnableEffectSprite.push_back(pEffect);
		break;
	case EFFECT_TYPE_TRACE:
		if (static_cast<CEffectPlane*>(pEffect)->m_pParent->m_pParent->m_nAlphaValue == SKILL_OBJECT_ALPHA_OTHER_INFLUENCE) break;
		m_vecZEnableEffectTrace.push_back(pEffect);
		break;

	case EFFECT_TYPE_PARTICLE:
		{
			auto pParticle = static_cast<CParticle*>(pEffect);

			switch (pParticle->m_pParent->m_nParticleType)
			{
			case PARTICLE_OBJECT_TYPE:
				m_vecZEnableEffectParticleObject.push_back(pEffect);
				break;
			case PARTICLE_SPRITE_TYPE:
				m_vecZEnableEffectParticleSprite.push_back(pEffect);
				break;
			}

			break;
		}
	}
}

void CEffectRender::ClearAllZEnableEffect()
{
	m_vecZEnableEffect.clear();

	m_vecZEnableEffectObject.clear();
	m_vecZEnableEffectSprite.clear();
	m_vecZEnableEffectTrace.clear();

	m_vecZEnableEffectParticleObject.clear();
	m_vecZEnableEffectParticleSprite.clear();

	m_vectorObjectParticles.clear();
	m_vectorSpriteParticles.clear();
	
	m_mapSpriteRenderInfo.clear();
	m_mapTraceRenderInfo.clear();
}

void CEffectRender::EraseRemoveZEnableEffect(Effect* pEffect)
{
	m_vecZEnableEffect.erase(remove(m_vecZEnableEffect.begin(), m_vecZEnableEffect.end(), pEffect), m_vecZEnableEffect.end());

	switch (pEffect->dwType)
	{
	case EFFECT_TYPE_OBJECT:
		m_vecZEnableEffectObject.erase(remove(m_vecZEnableEffectObject.begin(), m_vecZEnableEffectObject.end(), pEffect), m_vecZEnableEffectObject.end());
		break;
	case EFFECT_TYPE_SPRITE:
		m_vecZEnableEffectSprite.erase(remove(m_vecZEnableEffectSprite.begin(), m_vecZEnableEffectSprite.end(), pEffect), m_vecZEnableEffectSprite.end());
		break;
	case EFFECT_TYPE_TRACE:
		m_vecZEnableEffectTrace.erase(remove(m_vecZEnableEffectTrace.begin(), m_vecZEnableEffectTrace.end(), pEffect), m_vecZEnableEffectTrace.end());
		break;

	case EFFECT_TYPE_PARTICLE:
		{
			switch (((CParticle*)pEffect)->m_pParent->m_nParticleType)
			{
			case PARTICLE_OBJECT_TYPE:
				m_vecZEnableEffectParticleObject.erase(remove(m_vecZEnableEffectParticleObject.begin(), m_vecZEnableEffectParticleObject.end(), pEffect), m_vecZEnableEffectParticleObject.end());
				m_vectorObjectParticles.erase(remove(m_vectorObjectParticles.begin(), m_vectorObjectParticles.end(), ((CParticle*)pEffect)->m_pParent));
				break;
			case PARTICLE_SPRITE_TYPE:
				m_vecZEnableEffectParticleSprite.erase(remove(m_vecZEnableEffectParticleSprite.begin(), m_vecZEnableEffectParticleSprite.end(), pEffect), m_vecZEnableEffectParticleSprite.end());
				m_vectorSpriteParticles.erase(remove(m_vectorSpriteParticles.begin(), m_vectorSpriteParticles.end(), ((CParticle*)pEffect)->m_pParent));
				break;
			}

			break;
		}
	}
}


HRESULT CEffectRender::InitDeviceObjects()// load game data file(texture)
{
	FLOG("CEffectRender::InitDeviceObjects()");

	char strPath[MAX_PATH];

	m_pTexEffectData = new CGameDataOpt;
	strcpy(strPath, ".\\Res-Tex\\spreff.tex");
	m_pTexEffectData->SetFile(strPath, FALSE, NULL, 0);

	m_pEffectData = new CGameDataOpt; // 2015-10-20 PanKJ Use Optimized CGameData
	strcpy(strPath, ".\\Res-Eff\\effectInfo.inf");
	m_pEffectData->SetFile(strPath, FALSE, NULL, 0);

	m_pObjectData = new CGameDataOpt;
	strcpy(strPath, ".\\Res-Eff\\objectInfo.inf");
	m_pObjectData->SetFile(strPath, FALSE, NULL, 0);

	return S_OK;
}

HRESULT CEffectRender::RestoreDeviceObjects()// create vertex buffer
{
	FLOG("CEffectRender::RestoreDeviceObjects()");
	float hsx, hsy;
	int i, j;
	hsx = 0.5f;
	hsy = 0.5f;
	SPRITE_VERTEX* v;
	for (i = 0; i < 4; i++)
	{
		for (j = 0; j < 4; j++)
		{
			if (FAILED(g_pD3dDev->CreateVertexBuffer(4 * sizeof(SPRITE_VERTEX),
				0, D3DFVF_SPRITE_VERTEX, D3DPOOL_MANAGED, &m_pVB16[4 * i + j], NULL)))
				return E_FAIL;
			m_pVB16[4 * i + j]->Lock(0, 0, (void**)&v, 0);
			v[0].p = D3DXVECTOR3(-hsx, -hsy, 0);
			v[0].tu = 0.25f * j;
			v[0].tv = 0.25f * (i + 1);
			v[0].c = 0xffffffff;
			v[1].p = D3DXVECTOR3(-hsx, hsy, 0);
			v[1].tu = 0.25f * j;
			v[1].tv = 0.25f * i;
			v[1].c = 0xffffffff;
			v[2].p = D3DXVECTOR3(hsx, -hsy, 0);
			v[2].tu = 0.25f * (j + 1);
			v[2].tv = 0.25f * (i + 1);
			v[2].c = 0xffffffff;
			v[3].p = D3DXVECTOR3(hsx, hsy, 0);
			v[3].tu = 0.25f * (j + 1);
			v[3].tv = 0.25f * i;
			v[3].c = 0xffffffff;
			m_pVB16[4 * i + j]->Unlock();
		}
	}
	for (i = 0; i < 2; i++)
	{
		for (j = 0; j < 4; j++)
		{
			if (FAILED(g_pD3dDev->CreateVertexBuffer(4 * sizeof(SPRITE_VERTEX),
				0, D3DFVF_SPRITE_VERTEX, D3DPOOL_MANAGED, &m_pVB8[4 * i + j], NULL)))
				return E_FAIL;
			m_pVB8[2 * i + j]->Lock(0, 0, (void**)&v, 0);
			v[0].p = D3DXVECTOR3(-hsx, -hsy, 0);
			v[0].tu = 0.25f * j;
			v[0].tv = 0.5f * (i + 1);
			v[0].c = 0xffffffff;
			v[1].p = D3DXVECTOR3(-hsx, hsy, 0);
			v[1].tu = 0.25f * j;
			v[1].tv = 0.5f * i;
			v[1].c = 0xffffffff;
			v[2].p = D3DXVECTOR3(hsx, -hsy, 0);
			v[2].tu = 0.25f * (j + 1);
			v[2].tv = 0.5f * (i + 1);
			v[2].c = 0xffffffff;
			v[3].p = D3DXVECTOR3(hsx, hsy, 0);
			v[3].tu = 0.25f * (j + 1);
			v[3].tv = 0.5f * i;
			v[3].c = 0xffffffff;
			m_pVB8[2 * i + j]->Unlock();
		}
	}
	for (i = 0; i < 2; i++)
	{
		for (j = 0; j < 2; j++)
		{
			if (FAILED(g_pD3dDev->CreateVertexBuffer(4 * sizeof(SPRITE_VERTEX),
				0, D3DFVF_SPRITE_VERTEX, D3DPOOL_MANAGED, &m_pVB4[2 * i + j], NULL)))
				return E_FAIL;
			m_pVB4[2 * i + j]->Lock(0, 0, (void**)&v, 0);
			v[0].p = D3DXVECTOR3(-hsx, -hsy, 0);
			v[0].tu = 0.5f * j;
			v[0].tv = 0.5f * (i + 1);
			v[0].c = 0xffffffff;
			v[1].p = D3DXVECTOR3(-hsx, hsy, 0);
			v[1].tu = 0.5f * j;
			v[1].tv = 0.5f * i;
			v[1].c = 0xffffffff;
			v[2].p = D3DXVECTOR3(hsx, -hsy, 0);
			v[2].tu = 0.5f * (j + 1);
			v[2].tv = 0.5f * (i + 1);
			v[2].c = 0xffffffff;
			v[3].p = D3DXVECTOR3(hsx, hsy, 0);
			v[3].tu = 0.5f * (j + 1);
			v[3].tv = 0.5f * i;
			v[3].c = 0xffffffff;
			m_pVB4[2 * i + j]->Unlock();
		}
	}

	for (i = 0; i < 2; i++)
	{
		if (FAILED(g_pD3dDev->CreateVertexBuffer(4 * sizeof(SPRITE_VERTEX),
			0, D3DFVF_SPRITE_VERTEX, D3DPOOL_MANAGED, &m_pVB2[i], NULL)))
			return E_FAIL;
		m_pVB2[i]->Lock(0, 0, (void**)&v, 0);
		v[0].p = D3DXVECTOR3(-hsx, -hsy, 0);
		v[0].tu = 0.5f * i;
		v[0].tv = 1.0f;
		v[0].c = 0xffffffff;
		v[1].p = D3DXVECTOR3(-hsx, hsy, 0);
		v[1].tu = 0.5f * i;
		v[1].tv = 0.0f;
		v[1].c = 0xffffffff;
		v[2].p = D3DXVECTOR3(hsx, -hsy, 0);
		v[2].tu = 0.5f * (i + 1);
		v[2].tv = 1.0f;
		v[2].c = 0xffffffff;
		v[3].p = D3DXVECTOR3(hsx, hsy, 0);
		v[3].tu = 0.5f * (i + 1);
		v[3].tv = 0.0f;
		v[3].c = 0xffffffff;
		m_pVB2[i]->Unlock();
	}

	if (FAILED(g_pD3dDev->CreateVertexBuffer(4 * sizeof(SPRITE_VERTEX), 0, D3DFVF_SPRITE_VERTEX, D3DPOOL_MANAGED, &m_pVB1, NULL))) return E_FAIL;

	m_pVB1->Lock(0, 0, (void**)&v, 0);
	v[0].p = D3DXVECTOR3(-0.5f, -0.5f, 0);
	v[0].tu = 0.0f;
	v[0].tv = 1.0f;
	v[0].c = 0xffffffff;
	v[1].p = D3DXVECTOR3(-0.5f, 0.5f, 0);
	v[1].tu = 0.0f;
	v[1].tv = 0.0f;
	v[1].c = 0xffffffff;
	v[2].p = D3DXVECTOR3(0.5f, -0.5f, 0);
	v[2].tu = 1.0f;
	v[2].tv = 1.0f;
	v[2].c = 0xffffffff;
	v[3].p = D3DXVECTOR3(0.5f, 0.5f, 0);
	v[3].tu = 1.0f;
	v[3].tv = 0.0f;
	v[3].c = 0xffffffff;
	m_pVB1->Unlock();

	DBGOUT("CEffectRender::RestoreDeviceObjects()\n");
	return S_OK;
}

HRESULT CEffectRender::InvalidateDeviceObjects()
{
	FLOG("CEffectRender::InvalidateDeviceObjects()");
	SAFE_RELEASE(m_pVB1);
	int i;
	for (i = 0; i < 2; i++)
	SAFE_RELEASE(m_pVB2[i]);
	for (i = 0; i < 4; i++)
	SAFE_RELEASE(m_pVB4[i]);
	for (i = 0; i < 8; i++)
	SAFE_RELEASE(m_pVB8[i]);
	for (i = 0; i < 16; i++)
	SAFE_RELEASE(m_pVB16[i]);
	return S_OK;
}

HRESULT CEffectRender::DeleteDeviceObjects()
{
	FLOG("CEffectRender::DeleteDeviceObjects()");

	util::del(m_pTexEffectData);
	util::del(m_pEffectData);
	util::del(m_pObjectData);

	for (int i = 0; i < TEX_EFFECT_NUM; i++)
	SAFE_RELEASE(m_pTexture[i]);

	m_mapTexNameToIndex.clear();

	for (auto mesh : m_mapObjNameToMesh)
	{
		mesh.second->DeleteLoadingGameData();
		mesh.second->InvalidateDeviceObjects();
		mesh.second->DeleteDeviceObjects();

		util::del(mesh.second);
	}

	m_mapObjNameToMesh.clear();

	for (auto tex : m_mapTextToTexture)
	SAFE_RELEASE(tex.second);

	m_mapTextToTexture.clear();
	m_mapTextRenderCount.clear();

	for (auto tex : m_TextureManager.m_mapTextures)
	SAFE_RELEASE(tex.second);

	m_TextureManager.m_mapTextures.clear();
	m_TextureManager.m_mapTextureUsage.clear();

	m_vecLoadingPriority.clear();

	return S_OK;
}


// pEffect는 최대사이즈로 버퍼를 잡고 이 함수에 들어간다.
// 함수가 끝난 후에 내용을 사이즈만큼 복사하고, 버퍼는 삭제한다.
DWORD CEffectRender::LoadEffect(char* strName, DWORD dwEffectType, char* pEffect)
{
	FLOG("CEffectRender::LoadEffect(char* strName, DWORD dwEffectType, char* pEffect)");
	EffectFileHeader header;
	switch (dwEffectType)
	{
	case EFFECT_TYPE_OBJECT:
		{
			DataHeader* pDataHeader = FindEffectInfo(strName);
			if (pDataHeader)
			{
				memcpy((void*)&header, pDataHeader->m_pData, sizeof(EffectFileHeader));
				memcpy(pEffect, pDataHeader->m_pData + sizeof(EffectFileHeader), sizeof(ObjectAniData) - sizeof(Effect));
			}
		}
		break;
	case EFFECT_TYPE_SPRITE:
		{
			DataHeader* pDataHeader = FindEffectInfo(strName);
			if (pDataHeader)
			{
				memcpy((void*)&header, pDataHeader->m_pData, sizeof(EffectFileHeader));
				memcpy(pEffect, pDataHeader->m_pData + sizeof(EffectFileHeader), sizeof(SpriteAniData) - sizeof(Effect));
			}
		}
		break;
	case EFFECT_TYPE_PARTICLE:
		{
			DataHeader* pDataHeader = FindEffectInfo(strName);
			if (pDataHeader)
			{
				memcpy((void*)&header, pDataHeader->m_pData, sizeof(EffectFileHeader));
				memcpy(pEffect, pDataHeader->m_pData + sizeof(EffectFileHeader), sizeof(ParticleData));
			}
		}
		break;
	case EFFECT_TYPE_TRACE:
		{
			DataHeader* pDataHeader = FindEffectInfo(strName);
			if (pDataHeader)
			{
				memcpy((void*)&header, pDataHeader->m_pData, sizeof(EffectFileHeader));
				memcpy(pEffect, pDataHeader->m_pData + sizeof(EffectFileHeader), sizeof(TraceData));
			}
		}
		break;
	}
	return header.dwEffectType;
}

// 2015-10-19 PanKJ Experimental Optimizations
DWORD CEffectRender::LoadEffectPtr(char* strName, DWORD dwEffectType, char** ppEffect)
{
	DataHeader* pDataHeader = FindEffectInfo(strName);

	if (pDataHeader == nullptr)
	{
		DBGOUT("ERROR!!! CEffectRender::LoadEffectPtr(char*, DWORD, char**)");
		return -1;
	}

	auto pEffectHeader = (EffectFileHeader*)pDataHeader->m_pData;

	*ppEffect = pDataHeader->m_pData + sizeof(EffectFileHeader);

	return pEffectHeader->dwEffectType;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CEffectRender::AddFontTexture(char* strText, LPDIRECT3DTEXTURE8 pTexture )
/// \brief		폰트 텍스쳐를 ADD한다.
/// \author		dhkwon
/// \date		2004-03-25 ~ 2004-03-25
/// \warning	
///
/// \param		
/// \return		BOOL ( TRUE:Add성공, FALSE:이미 존재)
///////////////////////////////////////////////////////////////////////////////
BOOL CEffectRender::AddFontTexture(char* strText, LPDIRECT3DTEXTURE9 pTexture)
{
	FLOG("CEffectRender::AddFontTexture(char* strText, LPDIRECT3DTEXTURE9 pTexture )");
	auto it = m_mapTextToTexture.find(strText);
	if (it == m_mapTextToTexture.end())
	{
		m_mapTextToTexture[strText] = pTexture;
		m_mapTextRenderCount[strText] = 2;
		return TRUE;
	}

	auto it2 = m_mapTextRenderCount.find(strText);

	if (it2 != m_mapTextRenderCount.end()) (it2->second)++;

	return FALSE;// 이미 존재함
}

// 폰트에서 Texture를 지우지 않고, 이 함수를 통해 Texture가 지워진다.
// 곧, 폰트에서 Effect Render로 들어갈 Texture를 빼면 그 폰트의 Texture=NULL로 세팅한다.
BOOL CEffectRender::DeleteFontTexture(char* strText)
{
	FLOG("CEffectRender::DeleteFontTexture(char* strText)");
	auto it = m_mapTextToTexture.find(strText);
	if (it != m_mapTextToTexture.end())
	{
		auto it2 = m_mapTextRenderCount.find(strText);
		if (it2 != m_mapTextRenderCount.end())
		{
			if ((it2->second) > 2) it2->second--;
			else
			{
				LPDIRECT3DTEXTURE9 pTexture = it->second;
				SAFE_RELEASE(pTexture);
				m_mapTextToTexture.erase(strText);
				m_mapTextRenderCount.erase(strText);
			}
			return TRUE;
		}

		return FALSE;

	}
	
	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CEffectRender::CheckAlphaRender(CAppEffectData* pParent, DWORD dwType)
/// \brief		
/// \author		ispark
/// \date		2006-11-16 ~ 2006-11-16
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CEffectRender::CheckAlphaRender(CAppEffectData* pEffect, DWORD dwType)
{
	CCharacterInfo* pChar = pEffect->m_pCharacterInfo;

	if (dwType == _SHUTTLE)
	{
		if (((g_pShuttleChild->m_pWeapon1_1_1 &&
					pChar == g_pShuttleChild->m_pWeapon1_1_1->m_pCharacterInfo) ||
				(g_pShuttleChild->m_pWeapon1_1_2 &&
					pChar == g_pShuttleChild->m_pWeapon1_1_2->m_pCharacterInfo) ||
				(g_pShuttleChild->m_pWeapon2_1_1 &&
					pChar == g_pShuttleChild->m_pWeapon2_1_1->m_pCharacterInfo) ||
				(g_pShuttleChild->m_pWeapon2_1_2 &&
					pChar == g_pShuttleChild->m_pWeapon2_1_2->m_pCharacterInfo) ||
				(g_pShuttleChild->m_pWeapon1_2 &&
					pChar == g_pShuttleChild->m_pWeapon1_2->m_pCharacterInfo) ||
				(g_pShuttleChild->m_pEngine &&
					pChar == g_pShuttleChild->m_pEngine->m_pCharacterInfo) ||
				(g_pShuttleChild->m_pRadar &&
					pChar == g_pShuttleChild->m_pRadar->m_pCharacterInfo) ||
				(g_pShuttleChild->m_pContainer &&
					pChar == g_pShuttleChild->m_pContainer->m_pCharacterInfo) ||
				(g_pShuttleChild->m_pAccessories &&
					pChar == g_pShuttleChild->m_pAccessories->m_pCharacterInfo) ||
				(g_pShuttleChild->m_pWingIn &&
					pChar == g_pShuttleChild->m_pWingIn->m_pCharacterInfo) ||
				// 2011. 01. 13 by jskim, 인비지블 상태에서 펫이 보이는 버그 수정
				(g_pShuttleChild->m_pPartner &&
					pChar == g_pShuttleChild->m_pPartner->m_pCharacterInfo) ||
				(g_pShuttleChild->m_pPartner1 &&
					pChar == g_pShuttleChild->m_pPartner1->m_pCharacterInfo)) &&
			// end 2011. 01. 13 by jskim, 인비지블 상태에서 펫이 보이는 버그 수정 
			g_pShuttleChild->m_nAlphaValue < SKILL_OBJECT_ALPHA_NONE)
		{
			return TRUE;
		}
	}
	else if (dwType == _ENEMY ||
		dwType == _ADMIN)
	{
		CEnemyData* pEnemyData = (CEnemyData *)pEffect->m_pParent;
		if (((pEnemyData->m_pWeapon1_1_1 &&
					pChar == pEnemyData->m_pWeapon1_1_1->m_pCharacterInfo) ||
				(pEnemyData->m_pWeapon1_1_2 &&
					pChar == pEnemyData->m_pWeapon1_1_2->m_pCharacterInfo) ||
				(pEnemyData->m_pWeapon2_1_1 &&
					pChar == pEnemyData->m_pWeapon2_1_1->m_pCharacterInfo) ||
				(pEnemyData->m_pWeapon2_1_2 &&
					pChar == pEnemyData->m_pWeapon2_1_2->m_pCharacterInfo) ||
				(pEnemyData->m_pWeapon1_2 &&
					pChar == pEnemyData->m_pWeapon1_2->m_pCharacterInfo) ||
				(pEnemyData->m_pEngine &&
					pChar == pEnemyData->m_pEngine->m_pCharacterInfo) ||
				(pEnemyData->m_pRadar &&
					pChar == pEnemyData->m_pRadar->m_pCharacterInfo) ||
				(pEnemyData->m_pContainer &&
					pChar == pEnemyData->m_pContainer->m_pCharacterInfo) ||
				(pEnemyData->m_pAccessories &&
					pChar == pEnemyData->m_pAccessories->m_pCharacterInfo) ||
				(pEnemyData->m_pWingIn &&
					pChar == pEnemyData->m_pWingIn->m_pCharacterInfo) ||
				// 2011. 01. 13 by jskim, 인비지블 상태에서 펫이 보이는 버그 수정
				(pEnemyData->m_pPartner &&
					pChar == pEnemyData->m_pPartner->m_pCharacterInfo) ||
				(pEnemyData->m_pPartner1 &&
					pChar == pEnemyData->m_pPartner1->m_pCharacterInfo)) &&
			// end 2011. 01. 13 by jskim, 인비지블 상태에서 펫이 보이는 버그 수정			
			pEnemyData->m_nAlphaValue < SKILL_OBJECT_ALPHA_NONE)
		{
			return TRUE;
		}
	}
	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CEffectRender::AddInvenPaticleName(int nInvenIdx, char* pEffectName)
/// \brief		인벤 이펙트 추가
/// \author		// 2007-11-08 by bhsohn 인벤 이펙트 관련 처리
/// \date		2007-11-08 ~ 2007-11-08
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CEffectRender::AddInvenPaticleName(int nInvenIdx, char* pEffectName)
{
	structInvenParticleInfo struInvenParticleInfoTmp;
	memset(&struInvenParticleInfoTmp, 0x00, sizeof(structInvenParticleInfo));

	struInvenParticleInfoTmp.nWindowInvenIdx = nInvenIdx;
	strncpy(struInvenParticleInfoTmp.chEffectName, pEffectName, 32);

	m_vecInvenParticleInfo.push_back(struInvenParticleInfoTmp);
}

// 2012-07-13 by isshin 아템미리보기 인첸트이펙스 적용
void CEffectRender::AddEnemyItemPaticleName(int nInvenIdx, char* pEffectName)
{
	structInvenParticleInfo struInvenParticleInfoTmp;
	memset(&struInvenParticleInfoTmp, 0x00, sizeof(structInvenParticleInfo));

	struInvenParticleInfoTmp.nWindowInvenIdx = nInvenIdx;
	strncpy(struInvenParticleInfoTmp.chEffectName, pEffectName, 32);

	m_vecEnemyItemParticleInfo.push_back(struInvenParticleInfoTmp);
}

// end 2012-07-13 by isshin 아템미리보기 인첸트이펙스 적용

///////////////////////////////////////////////////////////////////////////////
/// \fn			CEffectInfo* CEffectRender::GetEffectInfo(char* pEffectName, int nWindowInvenIdx)
/// \brief		인벤 이펙트 정보 가져온다.
/// \author		// 2007-11-08 by bhsohn 인벤 이펙트 관련 처리
/// \date		2007-11-08 ~ 2007-11-08
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
CEffectInfo* CEffectRender::GetEffectInfo(char* pEffectName, int nWindowInvenIdx)
{
	if (NULL == g_pD3dApp->m_pEffectList)
	{
		return NULL;
	}
	CAppEffectData* pEffect = (CAppEffectData *)g_pD3dApp->m_pEffectList->m_pChild;
	while (pEffect)
	{
		CCharacterInfo* pChar = pEffect->m_pCharacterInfo;
		if (pChar)
		{
			//			if( pEffect->m_pParent 
			//				&& (pEffect->m_pParent->m_dwPartType == _SHUTTLE))
			{
				CEffectInfo* pRtnParticle = GetCharInfo_To_Effect(pChar, pEffectName, nWindowInvenIdx);
				if (pRtnParticle)
				{
					return pRtnParticle;
				}
			}
		}
		// 다음으로 넘어감.
		pEffect = (CAppEffectData*)pEffect->m_pNext;
	}
	return NULL;
}

// 2012-07-13 by isshin 아템미리보기 인첸트이펙스 적용
CEffectInfo* CEffectRender::GetEnemyEffectInfo(char* pEffectName, int nWindowInvenIdx, UID32_t TargetCharcterUID)
{
	if (NULL == g_pD3dApp->m_pEffectList)
	{
		return NULL;
	}

	CAppEffectData* pEffect = (CAppEffectData *)g_pD3dApp->m_pEffectList->m_pChild;
	while (pEffect)
	{
		// 2012-10-20 by bhsohn 인첸트 이펙트 메모리 버그 수정
		BOOL bContinue = FALSE;
		if (!pEffect->m_pParent)
		{
			bContinue = TRUE;
		}
		else if (!(pEffect->m_pParent->m_dwPartType == _ENEMY || pEffect->m_pParent->m_dwPartType == _ADMIN))
		{
			bContinue = TRUE;
		}

		if (bContinue)
		{
			pEffect = (CAppEffectData*)pEffect->m_pNext;
			continue;
		}
		// END 2012-10-20 by bhsohn 인첸트 이펙트 메모리 버그 수정

		if (((CEnemyData *)pEffect->m_pParent)->GetEnemyInfo().CharacterInfo.CharacterUniqueNumber == TargetCharcterUID)
		{
			CCharacterInfo* pChar = pEffect->m_pCharacterInfo;
			if (pChar)
			{
				CEffectInfo* pRtnParticle = GetEnemyCharInfo_To_Effect(pChar, pEffectName, nWindowInvenIdx);
				if (pRtnParticle)
				{
					return pRtnParticle;
				}
			}
		}
		// 다음으로 넘어감.
		pEffect = (CAppEffectData*)pEffect->m_pNext;
	}
	return NULL;
}

// end 2012-07-13 by isshin 아템미리보기 인첸트이펙스 적용

///////////////////////////////////////////////////////////////////////////////
/// \fn			CEffectInfo* CEffectRender::GetCharInfo_To_Effect(CCharacterInfo* pChar, char* pEffectName,int nWindowInvenIdx)
/// \brief		인벤 이펙트 정보 가져온다.
/// \author		// 2007-11-08 by bhsohn 인벤 이펙트 관련 처리
/// \date		2007-11-08 ~ 2007-11-08
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
CEffectInfo* CEffectRender::GetCharInfo_To_Effect(CCharacterInfo* pChar, char* pEffectName, int nWindowInvenIdx)
{
	set<BodyCond_t>::iterator itCurrent = pChar->m_vecCurrentBodyCondition.begin();
	while (itCurrent != pChar->m_vecCurrentBodyCondition.end())
	{
		map<BodyCond_t, CBodyConditionInfo*>::iterator itBody = pChar->m_mapBodyCondition.find(*itCurrent);
		if (itBody != pChar->m_mapBodyCondition.end())
		{
			CBodyConditionInfo* pBody = itBody->second;
			// 캐릭터 보디 컨디션의 이펙트를 체크
			vector<CEffectInfo*>::iterator itEffect = pBody->m_vecEffect.begin();
			while (itEffect != pBody->m_vecEffect.end())
			{
				// effect rendering
				CEffectInfo* pEffectInfo = (*itEffect);
				switch (pEffectInfo->m_nEffectType)
				{
				case EFFECT_TYPE_PARTICLE:
					{
						if (pEffectInfo->m_pEffect
							&& (0 == strncmp(pEffectName, pEffectInfo->m_strEffectName, strlen(pEffectName)))
							&& ((pEffectInfo->m_nInvenWeaponIndex - 1) == nWindowInvenIdx))
						{
							//							CParticleSystem* pEffect = (CParticleSystem*)pEffectInfo->m_pEffect;						
							//							return pEffect;
							return pEffectInfo;
						}
					}
				}
				itEffect++;
			}
		}
		itCurrent++;
	}
	return NULL;
}

// 2012-07-13 by isshin 아템미리보기 인첸트이펙스 적용
CEffectInfo* CEffectRender::GetEnemyCharInfo_To_Effect(CCharacterInfo* pChar, char* pEffectName, int nWindowInvenIdx)
{
	set<BodyCond_t>::iterator itCurrent = pChar->m_vecCurrentBodyCondition.begin();
	while (itCurrent != pChar->m_vecCurrentBodyCondition.end())
	{
		map<BodyCond_t, CBodyConditionInfo*>::iterator itBody = pChar->m_mapBodyCondition.find(*itCurrent);
		if (itBody != pChar->m_mapBodyCondition.end())
		{
			CBodyConditionInfo* pBody = itBody->second;
			// 캐릭터 보디 컨디션의 이펙트를 체크
			vector<CEffectInfo*>::iterator itEffect = pBody->m_vecEffect.begin();
			while (itEffect != pBody->m_vecEffect.end())
			{
				// effect rendering
				CEffectInfo* pEffectInfo = (*itEffect);
				switch (pEffectInfo->m_nEffectType)
				{
				case EFFECT_TYPE_PARTICLE:
					{
						if (pEffectInfo->m_pEffect
							&& (0 == strncmp(pEffectName, pEffectInfo->m_strEffectName, strlen(pEffectName)))
							&& ((pEffectInfo->m_nInvenWeaponIndex - 1 - ENEMY_WEAPON_INDEX_DUMMY) == nWindowInvenIdx))
						{
							//							CParticleSystem* pEffect = (CParticleSystem*)pEffectInfo->m_pEffect;						
							//							return pEffect;
							return pEffectInfo;
						}
					}
				}
				itEffect++;
			}
		}
		itCurrent++;
	}
	return NULL;
}

// end 2012-07-13 by isshin 아템미리보기 인첸트이펙스 적용

CEffectInfo* CEffectRender::GetObjEffectInfo(char* pObjName)
{
	// search g_pD3dApp->m_pEffectList
	for (
		auto pEffect = reinterpret_cast<CAppEffectData*>(g_pD3dApp->m_pEffectList->m_pChild);
		pEffect;
		pEffect = reinterpret_cast<CAppEffectData*>(pEffect->m_pNext))
	{
		auto pChar = pEffect->m_pCharacterInfo;

		if (!pChar) continue;
		
		// search body conditions
		for (const auto& bodycond : pChar->m_vecCurrentBodyCondition)
		{
			// find bodycondition info
			auto itBody = pChar->m_mapBodyCondition.find(bodycond);

			if (itBody != pChar->m_mapBodyCondition.end())
			{			
				// search CBodyConditionInfo::m_vecEffect
				for (auto pEffectInfo : itBody->second->m_vecEffect)

					if (strncmp(pObjName, pEffectInfo->m_strEffectName, 8) == 0)
							
						return pEffectInfo;
			}
		}
	}

	return nullptr;
}

