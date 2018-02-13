// ParticleSystem.cpp: implementation of the CParticleSystem class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Effect.h"
#include "ParticleSystem.h"
#include "AtumApplication.h"
#include "characterinfo.h"
#include "ObjectAniData.h"
#include "TraceAni.h"
#include "Camera.h"
#include "EffectRender.h"
#include "dxutil.h"

extern CAtumApplication* g_pD3dApp;


//////////////////////////////////////////////////////////////////////
// CParticle 
//////////////////////////////////////////////////////////////////////

CParticle::CParticle(int nParticleType)
{
	FLOG("CParticle(int nParticleType)");
	fDistance = 0.0f;
	dwType = EFFECT_TYPE_PARTICLE;

	m_nParticleType = nParticleType;//DELETE
	m_fSize = 0;
	m_fLifeTime = 0;
	m_fCurrentLifeTime = 0;

	m_cColor = D3DXCOLOR(0, 0, 0, 0);
	m_cColorStep = D3DXCOLOR(0, 0, 0, 0);
	m_vPos = D3DXVECTOR3(0, 0, 0);
	m_vVel = D3DXVECTOR3(0, 0, 0);
	m_vDir = D3DXVECTOR3(0, 0, 0);

	m_fTextureSizeVel = 0;
	m_fTextureStartSize = 0;
	m_fTextureSizeMax = 0;
	m_fTextureSizeMin = 0;
	m_nTextureSizeChangeType = 0;
	m_fCurrentTextureAnimationTime = 0;
	m_nTextureNumber = 0;

	m_fGravity = 0;

	m_nPersistence = 0;
	m_fColorChangeStartTime = 0;
	m_bRotating = false;
	m_fCurrentRotateAngle = 0;
	m_fCurrentRotateTime = 0;
	m_vStartPos = D3DXVECTOR3(0, 0, 0);
	m_pParent = nullptr;
	m_pObjectAni = nullptr;
	m_pTraceAni = nullptr;
	D3DXMatrixIdentity(&m_mOldParentMatrix);
	m_fCullRadius = 0.0f;
}

CParticle::~CParticle()
{
	FLOG("~CParticle()");
}

BOOL CParticle::Tick(float fElapsedTime)
{
	FLOG("CParticle::Tick(float fElapsedTime)");

	m_fLifeTime -= fElapsedTime;
	m_fCurrentLifeTime += fElapsedTime;

	if (m_fLifeTime < 0) return FALSE;

	D3DXVECTOR3 vOldPos = m_vPos;

	m_vPos += m_vVel * m_fCurrentLifeTime;

	if (m_fRotateAngle == 0)
	{
		m_vPos += m_vDir * fElapsedTime;//방향 및 가속도

		if (m_nParticleType == PARTICLE_OBJECT_TYPE && m_pParent->m_nObjMoveTargetType == OBJ_MOVE_TYPE0)
		{
			m_vObjTarget = m_vVel * m_fCurrentLifeTime + m_vDir * fElapsedTime;

			m_vObjUp = D3DXVECTOR3(0, 1, 0);
		}
	}
	else
	{
		D3DXVECTOR3 vdPos = m_vPos - m_vStartPos;
		D3DXVec3TransformCoord(&vdPos, &vdPos, &m_mRotation);
		m_vPos = m_vStartPos + vdPos;

		bool temp = m_nParticleType == PARTICLE_OBJECT_TYPE && m_pParent->m_nObjMoveTargetType == OBJ_MOVE_TYPE0;

		if (m_fCircleForce == 0.0f)
		{
			if (temp) m_vObjUp = D3DXVECTOR3(0, 1, 0);
		}
		else
		{
			D3DXVec3Normalize(&vdPos, &vdPos);
			m_vPos += vdPos * m_fCircleForce;
			if (temp)
			{
				m_vObjTarget = m_vVel * m_fCurrentLifeTime + vdPos * m_fCircleForce;
				m_vObjUp = D3DXVECTOR3(0, 1, 0);
			}
		}
	}

	m_vPos.y -= m_fGravity * m_fCurrentLifeTime;

	if (m_pParent->m_pParent->m_bUseCharacterMatrix)
	{
		D3DXVECTOR3 vOld(m_mOldParentMatrix._41, m_mOldParentMatrix._42, m_mOldParentMatrix._43);
		m_mOldParentMatrix = m_pParent->m_pParent->m_pParent->m_mMatrix;
		D3DXVECTOR3 vNew(m_mOldParentMatrix._41, m_mOldParentMatrix._42, m_mOldParentMatrix._43);
		D3DXVECTOR3 vMove = vNew - vOld;
		m_vPos += vMove;
		m_vStartPos += vMove;

		if (m_nParticleType == PARTICLE_OBJECT_TYPE &&
			m_pParent->m_nObjMoveTargetType == OBJ_MOVE_TYPE0 &&
			m_pParent->m_nObjCreateTargetType != OBJ_MOVE_TYPE1)
			
			m_vObjTarget = m_vPos - vOldPos;
	}

	if (m_nTextureSizeChangeType != TEXTURE_SIZE_FIX || m_nTextureAnimationType != TEXTURE_ANIMATION_RANDOM)
	{
		m_fCurrentTextureAnimationTime += fElapsedTime;

		if (m_fTextureAnimationTime > 0 && m_fCurrentTextureAnimationTime > m_fTextureAnimationTime)
		{
			if (m_nTextureSizeChangeType == TEXTURE_SIZE_STEP)
			{
				m_fSize += m_fTextureSizeVel;
				if (m_fSize < 0) return FALSE;
			}

			switch (m_nTextureAnimationType)
			{
			case TEXTURE_ANIMATION_RANDOM:
				m_nTextureType = RANDI(0, m_nTextureNumber - 1);
				break;
			case TEXTURE_ANIMATION_STEP:
				m_nTextureType++;
				if (m_nTextureType >= m_nTextureNumber) m_nTextureType = 0;
				break;
			}

			m_fCurrentTextureAnimationTime = 0;
		}
	}

	switch (m_nParticleType)
	{
	case PARTICLE_OBJECT_TYPE:
		if (m_pObjectAni)
		{
			m_fCullRadius = m_pObjectAni->m_fRadius;
			if (!m_pObjectAni->Tick(fElapsedTime)) return FALSE;
		}
		break;
	case PARTICLE_SPRITE_TYPE:
	{
		m_fColorChangeStartTime -= fElapsedTime;

		if (m_fColorChangeStartTime <= 0 && //((DWORD)m_cColor) != 0 && 
			(m_cColorStep.r != 0 || m_cColorStep.g != 0 || m_cColorStep.b != 0 || m_cColorStep.a != 0))
		{
			m_cColor += m_cColorStep * VEL_COLOR_CHANGE;
			if (m_cColor.r < 0) m_cColor.r = 0;
			if (m_cColor.g < 0) m_cColor.g = 0;
			if (m_cColor.b < 0) m_cColor.b = 0;
			if (m_cColor.a < 0) m_cColor.a = 0;
			if (m_cColor.r > 1) m_cColor.r = 1;
			if (m_cColor.g > 1) m_cColor.g = 1;
			if (m_cColor.b > 1) m_cColor.b = 1;
			if (m_cColor.a > 1) m_cColor.a = 1;
			if ((DWORD)m_cColor == 0)
			{
				if (m_pParent->m_nColorLoop > 0 || m_pParent->m_nColorLoop == -1) m_cColorStep *= -1;

				else if (m_pParent->m_nColorLoop == 0) return FALSE;
			}
			else if ((DWORD)m_cColor == 0xFFFFFFFF)
			{
				if (m_pParent->m_nColorLoop != 0) m_cColorStep *= -1;

				if (m_pParent->m_nColorLoop > 0) m_pParent->m_nColorLoop--;
			}
		}
		
		if (m_pParent && m_pParent->m_pParent
			&& m_pParent->m_pParent->m_fBillboardRotateAngle > 0
			&& m_pParent->m_pParent->m_fBillboardRotatePerSec > 0)
		{
			m_fCurrentRotateTime += fElapsedTime;
			if (m_fCurrentRotateTime > m_pParent->m_pParent->m_fBillboardRotatePerSec)
			{
				m_bRotating = true;
				m_fCurrentRotateAngle += m_pParent->m_pParent->m_fBillboardRotateAngle;
				m_fCurrentRotateTime = 0;
			}
		}

		m_fCullRadius = D3DXVec3Length(&m_vPos);
	}
	break;
	case PARTICLE_TRACE_TYPE: break;
	}

	return TRUE;
}

HRESULT CParticle::InitDeviceObjects()
{
	FLOG("CParticle::InitDeviceObjects()");
	//	m_vStartPos = m_vPos;
	switch (m_nParticleType)
	{
	case PARTICLE_OBJECT_TYPE:
	{
		char* pEffect = new char[sizeof(ParticleData)];//임의의 큰 사이즈
		DWORD type = g_pD3dApp->m_pEffectRender->LoadEffect(m_pParent->m_strTextureName[m_nTextureType], PARTICLE_OBJECT_TYPE, pEffect);
		if (m_pObjectAni)
		{
			m_pObjectAni->InvalidateDeviceObjects();
			m_pObjectAni->DeleteDeviceObjects();
			util::del(m_pObjectAni);

		}
		m_pObjectAni = new CObjectAni();
		memcpy((void*)((char*)m_pObjectAni + sizeof(Effect)), pEffect, sizeof(ObjectAniData) - sizeof(Effect));
		m_pObjectAni->m_pParent = m_pParent->m_pParent;// 부모의 부모(EffectInfo)
		m_pObjectAni->InitDeviceObjects();
		m_pObjectAni->RestoreDeviceObjects();
		delete[] pEffect;
		pEffect = nullptr;
	}
	break;
	case PARTICLE_SPRITE_TYPE: break;
	case PARTICLE_TRACE_TYPE:
		ASSERT_NEVER_GET_HERE();
		break;
	}
	return S_OK;
}

HRESULT CParticle::RestoreDeviceObjects()
{
	FLOG("CParticle::RestoreDeviceObjects()");
	return S_OK;
}

HRESULT CParticle::InvalidateDeviceObjects()
{
	FLOG("CParticle::InvalidateDeviceObjects()");
	return S_OK;
}

HRESULT CParticle::DeleteDeviceObjects()
{
	FLOG("CParticle::DeleteDeviceObjects()");
	switch (m_nParticleType)
	{
	case PARTICLE_OBJECT_TYPE:
	{
		m_pObjectAni->InvalidateDeviceObjects();
		m_pObjectAni->DeleteDeviceObjects();
		util::del(m_pObjectAni);
	}
	break;
	case PARTICLE_SPRITE_TYPE: break;
	case PARTICLE_TRACE_TYPE: break;
	}
	return S_OK;
}

//////////////////////////////////////////////////////////////////////
// CParticleSystem 
//////////////////////////////////////////////////////////////////////

CParticleSystem::CParticleSystem()
{
	FLOG("CParticleSystem()");
	m_dwStateBlock = 0;
	m_bLoop = FALSE;
	m_dwDestBlend = 1;
	m_dwSrcBlend = 1;
	m_nEmitMass = 0;
	m_fEmitTime = 0.0f;
	m_fGravity = 0.0f;
	//	m_fLifeTime = 0.0f;
	m_fDelayTime = 0.0f;
	m_cStartColor = D3DXCOLOR(0, 0, 0, 0);
	m_cColorVel = D3DXCOLOR(0, 0, 0, 0);
	m_vVel = D3DXVECTOR3(0, 0, 0);
	m_fTextureSizeVel = 0.0f;
	m_fTextureStartSize = 0.0f;
	m_fTextureSizeMax = 0.0f;
	m_fTextureSizeMin = 0.0f;
	m_fTick = 0.0f;
	m_nTextureSizeChangeType = -1;
	memset(m_strName, 0x00, sizeof(m_strName));
	m_nTextureNumber = 0;

	m_fEmitLifeTime = 0.0f;
	m_fParticleLifeTime = 0.0f;
	m_vPos = D3DXVECTOR3(0, 0, 0);
	m_vArea = D3DXVECTOR3(0, 0, 0);
	m_vDir = D3DXVECTOR3(0, 0, 0);

	m_bCreateRandom = TRUE;
	m_fCircleForce = 0.0f;
	m_fCreateDensity = 0.0f;
	m_fEmitRadius = 0.0f;
	m_fRotateAngle = 0.0f;
	m_fTextureAnimationTime = 0.0f;
	m_nEmitterType = 0;
	m_nTextureAnimationType = 0;
	m_nParticleType = 0;

	m_fCurrentEmitLifeTime = 0.0f;
	m_fCurrentDelayTime = 0.0f;

	m_fEmitAngle = 0.0f;

	memset(m_strTextureName, 0x00, sizeof(m_strTextureName));

	m_bZbufferEnable = FALSE;
	m_nPersistence = 0;
	m_fColorChangeStartTime = 0;

	m_pParent = NULL;

	m_fCurrentTick = 0;
	m_fRadius = 0.0f;
}

CParticleSystem::~CParticleSystem()
{
	FLOG("~CParticleSystem()");
	auto iter = m_vecParticle.begin();
	while (iter != m_vecParticle.end())
	{
		CParticle* p = (CParticle*)(*iter);
		iter = m_vecParticle.erase(iter);
		delete p;
		p = NULL;
	}
}

void CParticleSystem::SetTexture(char* strTexture, int index)
{
	strcpy(m_strTextureName[index], strTexture);
}

BOOL CParticleSystem::Tick(float fElapsedTime)
{
	BOOL r = TickInternal(fElapsedTime);
	if (r == TRUE && m_nParticleType == PARTICLE_SPRITE_TYPE) UpdateInstances();
	else m_vecParticleInstances.clear();
	return r;
}

BOOL CParticleSystem::TickInternal(float fElapsedTime)
{
	FLOG("CParticleSystem::Tick(float fElapsedTime)");
	//	fElapsedTime = 0.3f;
	//	m_fRadius = 0.0f;
	if (!m_bLoop)// 루프가 아닌경우
	{
		if (m_fCurrentEmitLifeTime < 0 && m_vecParticle.empty())
		{
			return FALSE;//dead, 파티클이 남아있으면 안된다.
		}
		m_fCurrentEmitLifeTime -= fElapsedTime;
		m_fCurrentTick -= fElapsedTime;
	}
	else if (m_fDelayTime > 0) { // 루프이면서 딜레이가 있는 경우
		if (m_fCurrentEmitLifeTime < 0 && m_vecParticle.empty())
		{
			m_fCurrentDelayTime -= fElapsedTime;
			if (m_fCurrentDelayTime > 0)
			{
				return TRUE;
			}
			else
			{
				m_fCurrentDelayTime = m_fDelayTime;
				m_fCurrentEmitLifeTime = m_fEmitLifeTime;
			}
		}
		m_fCurrentEmitLifeTime -= fElapsedTime;
		m_fCurrentTick -= fElapsedTime;
	}
	else // 루프이면서 딜레이가 없는 경우
	{
		m_fCurrentTick -= fElapsedTime;
	}
	if (m_fCurrentTick < 0)
	{
		// 생성
		if (m_fCurrentEmitLifeTime > 0)
		{
			if (m_fTick == 0)
				m_fCurrentEmitTime -= fElapsedTime;
			else
				m_fCurrentEmitTime -= fElapsedTime;//m_fTick;//fElapsedTime;
			if (m_fCurrentEmitTime < 0)
			{
				SpawnParticles();

				m_fCurrentEmitTime = m_fEmitTime;
			}
		}
		else {
			if (m_vecParticle.empty())
			{
				return FALSE;//dead, 파티클이 남아있으면 안된다.
			}

		}
		// 이동, 삭제
		vector<CParticle*> vecParticle;
		auto iter = m_vecParticle.begin();
		m_fRadius = 0.0f;
		while (iter != m_vecParticle.end())
		{
			float tick = fElapsedTime;//m_fTick;
			if (tick == 0)
				tick = fElapsedTime;
			CParticle* pParticle = NULL;
			CParticle* p = (CParticle*)(*iter);
			// 잔상(persistence)
			if (m_nPersistence > 0)
			{
				pParticle = new CParticle(m_nParticleType);
				memcpy(pParticle, p, sizeof(CParticle));
			}

			if (m_fRadius < p->m_fCullRadius)
				m_fRadius = p->m_fCullRadius;

			if (p->Tick(tick>fElapsedTime ? tick : fElapsedTime))
			{
				if (p->m_nPersistence > 0
					&& m_nPersistence > 0)
				{
					p->m_nPersistence = 0;
					pParticle->m_nPersistence--;
					D3DXCOLOR step(0.03f, 0.03f, 0.03f, 0.03f);
					pParticle->m_cColor -= step;
					if (pParticle->m_cColor.r < 0) pParticle->m_cColor.r = 0;
					if (pParticle->m_cColor.g < 0) pParticle->m_cColor.g = 0;
					if (pParticle->m_cColor.b < 0) pParticle->m_cColor.b = 0;
					if (pParticle->m_cColor.a < 0) pParticle->m_cColor.a = 0;

					if (pParticle->m_cColor.r == 0 && pParticle->m_cColor.g == 0 && pParticle->m_cColor.b == 0 && pParticle->m_cColor.a == 0)
					{
						util::del(pParticle);
					}
					else
					{
						D3DXVECTOR3 vDir = p->m_vPos - pParticle->m_vPos;
						//						D3DXVECTOR3 vDir = pParticle->m_vPos - p->m_vPos;
						D3DXVec3Normalize(&vDir, &vDir);
						pParticle->m_vPos = p->m_vPos + vDir * pParticle->m_fSize / 10;
						pParticle->InitDeviceObjects();
						pParticle->RestoreDeviceObjects();
						vecParticle.push_back(pParticle);
					}
				}
				else
				{
					util::del(pParticle);
				}

				iter++;
				//				continue;			
			}
			else
			{
				util::del(pParticle);
				iter = m_vecParticle.erase(iter);
				p->InvalidateDeviceObjects();
				p->DeleteDeviceObjects();
				util::del(p);
			}
		}
		m_fCurrentTick = m_fTick;// 이 값이 0이면 무한으로 Tick이 들어감
		if (vecParticle.size() > 0)
		{
			m_vecParticle.insert(m_vecParticle.end(), vecParticle.begin(), vecParticle.end());
		}
	}

	return TRUE;
}

void CParticleSystem::UpdateInstances()
{
	auto count = m_vecParticle.size();
	m_vecParticleInstances.resize(count);

	if (HasMonsterTransformer()) for (unsigned i = 0; i < count; i++)
	{
		auto p = (CParticle*)m_vecParticle[i];
		
		m_vecParticleInstances[i] = { p->m_vPos, p->m_cColor, p->m_fSize * GetMonsterTransformer(), p->m_fRotateAngle };
	}
	else for (unsigned i = 0; i < count; i++)
	{
		auto p = (CParticle*)m_vecParticle[i];

		m_vecParticleInstances[i] = { p->m_vPos, p->m_cColor, p->m_fSize, p->m_fRotateAngle };
	}
}

void CParticleSystem::SpawnParticles()
{
	if (m_nParticleType == PARTICLE_OBJECT_TYPE) return; // devx disable object particles

	for (int i = 0; i < m_nEmitMass; i++)
	{
		D3DXMATRIX mat;
		CParticle* p = new CParticle(m_nParticleType);
		p->m_pParent = this;
		p->m_fLifeTime = m_fParticleLifeTime;
		p->m_fSize = m_fTextureStartSize;
		p->m_cColor = m_cStartColor;
		p->m_cColorStep = m_cColorVel;
		p->m_fTextureSizeVel = m_fTextureSizeVel;

		switch (m_nEmitterType)
		{
		case EMITTER_POINT:
			p->m_vPos = m_vPos;
			break;
		case EMITTER_CIRCLE:
			if (m_bCreateRandom)
			{
				p->m_vPos.x = m_vPos.x + Random(m_fEmitRadius, -m_fEmitRadius);
				p->m_vPos.y = m_vPos.y;
				p->m_vPos.z = m_vPos.z + Random(m_fEmitRadius, -m_fEmitRadius);
			}
			else
			{
				D3DXMATRIX	matTemp;
				if (m_vArea == D3DXVECTOR3(0, 0, 0)) m_vArea.y = 1.0f;
				D3DXVECTOR3 vTemp(m_vArea), vPos(m_fEmitRadius, 0, 0);
				D3DXMatrixRotationAxis(&matTemp, &vTemp, m_fEmitAngle*i);
				D3DXVec3TransformCoord(&p->m_vPos, &vPos, &matTemp);
				p->m_vPos += m_vPos;
			}
			break;
		case EMITTER_SPHERE:
			p->m_vPos.x = m_vPos.x + Random(m_fEmitRadius, -m_fEmitRadius);
			p->m_vPos.y = m_vPos.y + Random(m_fEmitRadius, -m_fEmitRadius);
			p->m_vPos.z = m_vPos.z + Random(m_fEmitRadius, -m_fEmitRadius);
			break;
		case EMITTER_AREA:
			p->m_vPos.x = Random(m_vPos.x, m_vArea.x);
			p->m_vPos.y = Random(m_vPos.y, m_vArea.y);
			p->m_vPos.z = Random(m_vPos.z, m_vArea.z);
			break;
		}


		D3DXVECTOR3 pos;

		if (m_pParent)
		{
			pos = m_pParent->m_vPos;
			D3DXVec3TransformCoord(&pos, &pos, &m_pParent->m_pParent->m_mMatrix);
		}
		else pos = D3DXVECTOR3(0, 0, 0);

		p->m_vPos += pos;
		p->m_vStartPos = pos;
		p->m_vVel = m_vVel;

		p->m_vDir = D3DXVECTOR3(m_vDir.x + Random(m_fCreateDensity, -m_fCreateDensity),
								m_vDir.y + Random(m_fCreateDensity, -m_fCreateDensity),
								m_vDir.z + Random(m_fCreateDensity, -m_fCreateDensity));

		if (m_pParent->m_bUseCharacterMatrix)
		{
			D3DXMATRIX m = m_pParent->m_pParent->m_mMatrix;
			m._41 = 0;
			m._42 = 0;
			m._43 = 0;
			D3DXVec3TransformCoord(&p->m_vVel, &p->m_vVel, &m);
			D3DXVec3TransformCoord(&p->m_vDir, &p->m_vDir, &m);
			p->m_mOldParentMatrix = m_pParent->m_pParent->m_mMatrix;
		}

		p->m_fCircleForce = m_fCircleForce;
		p->m_fGravity = m_fGravity;
		p->m_fRotateAngle = m_fRotateAngle;

		D3DXMatrixRotationAxis(&p->m_mRotation, &p->m_vDir, p->m_fRotateAngle); // devx 2015-11-18

		p->m_nTextureNumber = m_nTextureNumber;
		p->m_fTextureAnimationTime = m_fTextureAnimationTime;
		p->m_nTextureAnimationType = m_nTextureAnimationType;

		if (m_nTextureAnimationType == TEXTURE_ANIMATION_RANDOM) p->m_nTextureType = RANDI(0, m_nTextureNumber - 1);
		else p->m_nTextureType = 0;

		p->m_fTextureSizeMax = m_fTextureSizeMax;
		p->m_fTextureSizeMin = m_fTextureSizeMin;
		p->m_fTextureSizeMax = m_fTextureSizeMax;
		p->m_fTextureSizeMin = m_fTextureSizeMin;

		if (m_nTextureSizeChangeType == TEXTURE_SIZE_RANDOM)
		{
			p->m_fSize = Random(m_fTextureSizeMax, m_fTextureSizeMin);
			p->m_fTextureStartSize = p->m_fSize;
		}
		else p->m_fTextureStartSize = m_fTextureStartSize;

		p->m_fTextureSizeVel = m_fTextureSizeVel;
		p->m_nTextureSizeChangeType = m_nTextureSizeChangeType;
		p->m_fColorChangeStartTime = m_fColorChangeStartTime;

		p->m_nPersistence = m_nPersistence;
		if (m_pParent && m_pParent->m_fBillboardRotateAngle > 0)
		{
			p->m_bRotating = true;
			p->m_fCurrentRotateAngle = Random(m_pParent->m_fRandomUpLargeAngle, m_pParent->m_fRandomUpSmallAngle);
		}

		switch (m_nObjCreateTargetType)
		{
		case OBJ_MOVE_TYPE0://파티클 이동방향을 가리킴
		{
			p->m_vObjTarget = p->m_vPos + p->m_vVel + p->m_vDir;
			p->m_vObjUp = D3DXVECTOR3(0, 1, 0);
		}
		break;
		case OBJ_MOVE_TYPE1:// 유닛 방향으로 가리킴
		{
			p->m_vObjTarget = m_pParent ? m_pParent->m_vTarget : D3DXVECTOR3(0, 0, 1);
			p->m_vObjUp = m_pParent ? m_pParent->m_vUp : D3DXVECTOR3(0, 1, 0);
		}
		break;
		case OBJ_MOVE_TYPE2:// 랜덤으로 움직임
		{
			p->m_vObjTarget.x = Random(1.0f, -1.0f);
			p->m_vObjTarget.y = Random(1.0f, -1.0f);
			p->m_vObjTarget.z = Random(1.0f, -1.0f);
			D3DXVec3Normalize(&p->m_vObjTarget, &p->m_vObjTarget);
			p->m_vObjUp = m_pParent ? m_pParent->m_vUp : D3DXVECTOR3(0, 1, 0);
		}
		break;
		case OBJ_MOVE_TYPE3:// 빌보드
		{
			p->m_vObjTarget = p->m_vPos + g_pD3dApp->m_pCamera->GetViewDir();
			p->m_vObjUp = m_pParent ? m_pParent->m_vUp : D3DXVECTOR3(0, 1, 0);
		}
		break;
		}

		p->InitDeviceObjects();
		p->RestoreDeviceObjects();
		m_vecParticle.push_back(p);

	}
}

void CParticleSystem::Render()
{
	FLOG("CParticleSystem::Render()");

}

HRESULT CParticleSystem::InitDeviceObjects()
{
	FLOG("CParticleSystem::InitDeviceObjects()");

	m_fCurrentEmitTime = 0; // 발산시스템 시간

	m_fCurrentEmitLifeTime = m_fEmitLifeTime;
	m_fCurrentTick = 0;

	if (m_nEmitterType == EMITTER_POINT && m_bCreateRandom)
	{
		m_vPos.x = Random(m_vPos.x, m_vArea.x);
		m_vPos.y = Random(m_vPos.y, m_vArea.y);
		m_vPos.z = Random(m_vPos.z, m_vArea.z);
	}

	// 속도가 느릴경우
	if (g_pSOption->sLowQuality || g_pD3dApp->GetFPS() < 20.0f)
	{
		m_nEmitMass = 1;
		m_nPersistence = 0; // 최소 프레임때 튕김 주석처리.
	}

	return S_OK;
}

HRESULT CParticleSystem::RestoreDeviceObjects()
{
	FLOG("CParticleSystem::RestoreDeviceObjects()");
	/*	g_pD3dDev->BeginStateBlock();
		g_pD3dDev->SetRenderState( D3DRS_ZENABLE, m_bZbufferEnable );
		g_pD3dDev->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
		g_pD3dDev->SetRenderState( D3DRS_SRCBLEND, m_dwSrcBlend );
		g_pD3dDev->SetRenderState( D3DRS_DESTBLEND, m_dwDestBlend );
		g_pD3dDev->SetRenderState( D3DRS_LIGHTING, TRUE );
		g_pD3dDev->EndStateBlock( &m_dwStateBlock );
		*/	return S_OK;
}

HRESULT CParticleSystem::InvalidateDeviceObjects()
{
	FLOG("CParticleSystem::InvalidateDeviceObjects()");
	//	g_pD3dDev->DeleteStateBlock( m_dwStateBlock );
	return S_OK;
}

HRESULT CParticleSystem::DeleteDeviceObjects()
{
	FLOG("CParticleSystem::DeleteDeviceObjects()");

	return S_OK;
}

bool CParticleSystem::HasMonsterTransformer()
{
	if (MonsterTransformState == 1) return true;

	if (MonsterTransformState == 0) return false;

	if (m_pParent->m_pParent->m_MonsterTransformer != 0 &&
		m_pParent->m_pParent->m_MonsterTransScale > 0.0f &&
		m_pParent->m_pParent->m_MonsterTransScale != 1.0f)
	{
		MonsterTransformState = 1;
		MonsterTransformValue = m_pParent->m_pParent->m_MonsterTransScale;
		return true;
	}

	MonsterTransformState = 0;
	return false;
}