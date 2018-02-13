// CharacterInfo.cpp: implementation of the CCharacterInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CharacterInfo.h"
#include "SpriteAniData.h"
#include "AtumApplication.h"
#include "EffectRender.h"
#include "ParticleSystem.h"
#include "TraceAni.h"
#include "AtumSound.h"
#include "ObjectAniData.h"
#include "GameDataLast.h"
#include "Camera.h"
#include "dxutil.h"
#include "MemPoolClient.h"

#ifdef _DEBUG
void TempEffectLib() {} // Create Temp Global Folder
#endif
//////////////////////////////////////////////////////////////////////
// CEffectInfo
//////////////////////////////////////////////////////////////////////
CEffectInfo::CEffectInfo()
{
	FLOG( "CEffectInfo()" );
	memset((char*)this,0,sizeof(EffectData));
	m_pEffect = NULL;
	m_bRestored = FALSE;
	m_fCurrentTime = 0;
	m_pParent = NULL;
	m_pTexture = NULL;
	m_nInvenWeaponIndex = FALSE;
	m_nAlphaValue = SKILL_OBJECT_ALPHA_NONE;
	// 2009. 11. 23 by jskim 리소스 로딩 구조 변경
	m_LoadingPriority = _NOTHING_PRIORITY;
	//end 2009. 11. 23 by jskim 리소스 로딩 구조 변경
}

CEffectInfo::~CEffectInfo()
{
	FLOG( "~CEffectInfo()" );
	if(m_bRestored)
	{
		InvalidateDeviceObjects();
		DeleteDeviceObjects();
	}
}
void * CEffectInfo::operator new(size_t size)
{
	//DBGOUT("CEffectInfo::operator new(%d)\n",CMemPoolClient::GetObjectValidSize(size));
	return CMemPoolClient::ObjectNew(size);
}
void CEffectInfo::operator delete(void* p)
{
	//DBGOUT("CEffectInfo::operator delete(%d)\n",CMemPoolClient::GetObjectValidSize(sizeof(CEffectInfo)));
	CMemPoolClient::ObjectDelete(p,sizeof(CEffectInfo));
	return;
}
void CEffectInfo::InitData()
{
	FLOG( "CEffectInfo::InitData()" );
	memset((char*)this,0,sizeof(EffectData));
	m_pEffect = NULL;
	m_fCurrentTime = m_fStartTime;
	
}

void CEffectInfo::ParticleStop()
{
	FLOG( "CEffectInfo::ParticleStop()" );
	if(m_nEffectType == EFFECT_TYPE_PARTICLE)
	{
		if(m_pEffect)
		{
			((CParticleSystem*)m_pEffect)->EmitterStop();
		}
	}
}

// by dhkwon, 030917
void CEffectInfo::SetTexture(char* strTexture)
{
	FLOG( "CEffectInfo::SetTexture(char* strTexture)" );
	switch(m_nEffectType)
	{
	case EFFECT_TYPE_OBJECT:
		{
			if(m_pEffect)
			{
				((CObjectAni*)m_pEffect)->SetTexture(strTexture);
			}
		}
		break;
	case EFFECT_TYPE_SPRITE:
		{
			if(m_pEffect)
			{
				((CSpriteAni*)m_pEffect)->SetTexture(strTexture);
			}
		}
		break;
	case EFFECT_TYPE_PARTICLE:
		{
			if(m_pEffect)
			{
				((CParticleSystem*)m_pEffect)->SetTexture(strTexture);
			}
		}
		break;
	case EFFECT_TYPE_TRACE:
		{
			if(m_pEffect)
			{
				((CParticleSystem*)m_pEffect)->SetTexture(strTexture);
			}
		}
		break;
	}

}

BOOL CEffectInfo::Tick(float fElapsedTime)
{
	FLOG( "CEffectInfo::Tick(float fElapsedTime)" );
	if (m_pEffect == nullptr) return FALSE;
		
	if (m_bRestored == FALSE) return TRUE;
		
	m_fCurrentTime -= fElapsedTime;
		
	if (m_fCurrentTime > 0) return TRUE;

	switch (m_nEffectType)
	{
	case EFFECT_TYPE_OBJECT:
	{
		if (!((CObjectAni*)m_pEffect)->Tick(fElapsedTime))
		{
			if (((CObjectAni*)m_pEffect)->m_bZbufferEnable)
			{
#ifndef FPSIMPROVEMENTS
				vector<Effect*>::iterator it = g_pD3dApp->m_pEffectRender->m_vecZEnableEffect.begin();
				while(it != g_pD3dApp->m_pEffectRender->m_vecZEnableEffect.end())
				{
					if(m_pEffect == (char*)*it)
					{
						g_pD3dApp->m_pEffectRender->m_vecZEnableEffect.erase(it);
						break;
					}
					it++;
				}
#else
				g_pD3dApp->m_pEffectRender->EraseRemoveZEnableEffect((Effect*)m_pEffect);
#endif
			}
			((CObjectAni*)m_pEffect)->InvalidateDeviceObjects();
			((CObjectAni*)m_pEffect)->DeleteDeviceObjects();
			util::del(m_pEffect);
			m_bRestored = FALSE;
			return FALSE;
		}
	}
	break;
	case EFFECT_TYPE_SPRITE:
	{
		if (!((CSpriteAni*)m_pEffect)->Tick(fElapsedTime))
		{
			if (((CSpriteAni*)m_pEffect)->m_bZbufferEnable)
			{
#ifndef FPSIMPROVEMENTS
				vector<Effect*>::iterator it = g_pD3dApp->m_pEffectRender->m_vecZEnableEffect.begin();
				while(it != g_pD3dApp->m_pEffectRender->m_vecZEnableEffect.end())
				{
					if(m_pEffect == (char*)*it)
					{
						g_pD3dApp->m_pEffectRender->m_vecZEnableEffect.erase(it);
						break;
					}
					it++;
				}
#else
				g_pD3dApp->m_pEffectRender->EraseRemoveZEnableEffect((Effect*)m_pEffect);
#endif
			}
			((CSpriteAni*)m_pEffect)->InvalidateDeviceObjects();
			((CSpriteAni*)m_pEffect)->DeleteDeviceObjects();
			util::del(m_pEffect);
			m_bRestored = FALSE;
			return FALSE;
		}
	}
	break;
	case EFFECT_TYPE_PARTICLE: // this part is the bottleneck
	{
		auto system = (CParticleSystem*)m_pEffect;
		if (system->Tick(fElapsedTime) == FALSE)
		{
			if (system->m_bZbufferEnable) g_pD3dApp->m_pEffectRender->EraseRemoveZEnableEffect((Effect*)m_pEffect);

			system->InvalidateDeviceObjects();
			system->DeleteDeviceObjects();
			util::del(m_pEffect);
			m_bRestored = FALSE;
			return FALSE;
		}
	}
	break;
	case EFFECT_TYPE_TRACE:
	{
		if (!((CTraceAni*)m_pEffect)->Tick(fElapsedTime))
		{
			if (((CTraceAni*)m_pEffect)->m_bZbufferEnable)
			{
#ifndef FPSIMPROVEMENTS
				vector<Effect*>::iterator it = g_pD3dApp->m_pEffectRender->m_vecZEnableEffect.begin();
				while(it != g_pD3dApp->m_pEffectRender->m_vecZEnableEffect.end())
				{
					if(m_pEffect == (char*)*it)
					{
						g_pD3dApp->m_pEffectRender->m_vecZEnableEffect.erase(it);
						break;
					}
					it++;
				}
#else
				g_pD3dApp->m_pEffectRender->EraseRemoveZEnableEffect((Effect*)m_pEffect);
#endif
			}
			((CTraceAni*)m_pEffect)->InvalidateDeviceObjects();
			((CTraceAni*)m_pEffect)->DeleteDeviceObjects();
			util::del(m_pEffect);
			m_bRestored = FALSE;
			return FALSE;
		}
	}
	break;
	}
	return TRUE;
}

void CEffectInfo::Render()
{
	FLOG( "CEffectInfo::Render()" );

}

void CEffectInfo::SetObjectAniStartTime(float fStartObjectAniTime)
{
	FLOG( "CEffectInfo::SetObjectAniStartTime(float fStartObjectAniTime)" );
	if(m_nEffectType == EFFECT_TYPE_OBJECT && m_pEffect)
	{
		((CObjectAni*)m_pEffect)->SetObjectAniStartTime(fStartObjectAniTime);
	}
}

// 2015-10-19 PanKJ Experimental Optimizations
HRESULT CEffectInfo::InitDeviceObjects()
{
	FLOG( "CEffectInfo::InitDeviceObjects()" );

	if (!m_bRestored || !m_pEffect)
	{
		char* pEffect = nullptr;
		DWORD type = g_pD3dApp->m_pEffectRender->LoadEffectPtr(m_strEffectName, m_nEffectType, &pEffect);
		ASSERT_ASSERT(pEffect);
		switch(type)
		{
		case EFFECT_TYPE_OBJECT:
			{
				CObjectAni* pObjEffect = new CObjectAni();
				memcpy((void*)((char*)pObjEffect+sizeof(Effect)), pEffect, sizeof(ObjectAniData)-sizeof(Effect));
				pObjEffect->m_pParent = this;
				pObjEffect->InitDeviceObjects();
				m_pEffect = (char*)pObjEffect;
			}
			break;
		case EFFECT_TYPE_SPRITE:
			{
				CSpriteAni* pSprEffect = new CSpriteAni();
				memcpy((void*)((char*)pSprEffect+sizeof(Effect)), pEffect, sizeof(SpriteAniData)-sizeof(Effect));
				pSprEffect->m_pParent = this;
				pSprEffect->InitDeviceObjects();
				m_pEffect = (char*)pSprEffect;
			}
			break;
		case EFFECT_TYPE_PARTICLE:
			{

				CParticleSystem* pParEffect = new CParticleSystem();
				memcpy((void*)pParEffect, pEffect, sizeof(ParticleData));
				pParEffect->m_pParent = this;
				pParEffect->InitDeviceObjects();
				m_pEffect = (char*)pParEffect;
			}
			break;
		case EFFECT_TYPE_TRACE:
			{
				// 2004-11-02 by jschoi
				if(g_pD3dApp->m_bDegree != 0)
				{
					if( g_pSOption->sLowQuality ||
						g_pD3dApp->GetFPS() < 20.0f )
					{
						break;
					}
					CTraceAni* pTraEffect = new CTraceAni();
					memcpy((void*)pTraEffect, pEffect, sizeof(TraceData));
					pTraEffect->m_pParent = this;
					pTraEffect->InitDeviceObjects();
					m_pEffect = (char*)pTraEffect;
				}
			}
			break;
		default:
			{
				DBGOUT("Non Existent Effect :     Name = %s \n",m_strEffectName);
			}
			break;

		}
	//delete pEffect;
	//pEffect = NULL;
	}
	else
	{
		switch(m_nEffectType)
		{
		case EFFECT_TYPE_OBJECT:
			{
				((CObjectAni*)m_pEffect)->InitDeviceObjects();
			}
			break;
		case EFFECT_TYPE_SPRITE:
			{
				((CSpriteAni*)m_pEffect)->InitDeviceObjects();
			}
			break;
		case EFFECT_TYPE_PARTICLE:
			{
				((CParticleSystem*)m_pEffect)->InitDeviceObjects();
			}
			break;
		case EFFECT_TYPE_TRACE:
			{
				// 2006-07-07 by ispark, 밑에 두줄을 실행후 InitDeviceObjects()를 실행해야 한다.
				((CTraceAni*)m_pEffect)->InvalidateDeviceObjects();
				((CTraceAni*)m_pEffect)->DeleteDeviceObjects();
				((CTraceAni*)m_pEffect)->InitDeviceObjects();
			}
			break;

		}
	}
	
	m_fCurrentTime = m_fStartTime;
	m_bRestored = TRUE;
	
	return S_OK;
}

HRESULT CEffectInfo::RestoreDeviceObjects()
{
	FLOG( "CEffectInfo::RestoreDeviceObjects()" );
	switch(m_nEffectType)
	{
	case EFFECT_TYPE_OBJECT:
		{
			if(m_pEffect)
			{
				// 2009. 11. 23 by jskim 리소스 로딩 구조 변경
				((CObjectAni*)m_pEffect)->m_LoadingPriority = m_LoadingPriority;
				//end 2009. 11. 23 by jskim 리소스 로딩 구조 변경
				((CObjectAni*)m_pEffect)->RestoreDeviceObjects();
			}
		}
		break;
	case EFFECT_TYPE_SPRITE:
		{
			if(m_pEffect)
			{
				((CSpriteAni*)m_pEffect)->RestoreDeviceObjects();
			}
		}
		break;
	case EFFECT_TYPE_PARTICLE:
		{
			if(m_pEffect)
			{
				((CParticleSystem*)m_pEffect)->RestoreDeviceObjects();
			}
		}
		break;
	case EFFECT_TYPE_TRACE:
		{
			if(m_pEffect)
			{
				((CTraceAni*)m_pEffect)->RestoreDeviceObjects();
			}
		}
		break;
	}
	return S_OK;
}

HRESULT CEffectInfo::InvalidateDeviceObjects()
{
	FLOG( "CEffectInfo::InvalidateDeviceObjects()" );
	if(!m_bRestored)
	{
		return S_OK;
	}
	switch(m_nEffectType)
	{
	case EFFECT_TYPE_OBJECT:
		{
			if(m_pEffect)
			{
				((CObjectAni*)m_pEffect)->InvalidateDeviceObjects();
			}
		}
		break;
	case EFFECT_TYPE_SPRITE:
		{
			if(m_pEffect)
			{
				((CSpriteAni*)m_pEffect)->InvalidateDeviceObjects();
			}
		}
		break;
	case EFFECT_TYPE_PARTICLE:
		{
			if(m_pEffect)
			{
				((CParticleSystem*)m_pEffect)->InvalidateDeviceObjects();
			}
		}
		break;
	case EFFECT_TYPE_TRACE:
		{
			if(m_pEffect)
			{
				((CTraceAni*)m_pEffect)->InvalidateDeviceObjects();
			}
		}
		break;
	}
	//m_bRestored = FALSE;
	return S_OK;
}

HRESULT CEffectInfo::DeleteDeviceObjects()
{
	FLOG( "CEffectInfo::DeleteDeviceObjects()" );
	if(!m_bRestored)
	{
		return S_OK;
	}
	switch(m_nEffectType)
	{
	case EFFECT_TYPE_OBJECT:
		{
			if(m_pEffect)
			{
				if(((CObjectAni*)m_pEffect)->m_bZbufferEnable && 
					g_pD3dApp->m_pEffectRender)
				{
#ifndef FPSIMPROVEMENTS
					vector<Effect*>::iterator it = g_pD3dApp->m_pEffectRender->m_vecZEnableEffect.begin();
					while(it != g_pD3dApp->m_pEffectRender->m_vecZEnableEffect.end())
					{
						if(m_pEffect == (char*)*it)
						{
							g_pD3dApp->m_pEffectRender->m_vecZEnableEffect.erase(it);
							DBGOUT("ZBufferEnable Effect Erase(CObjectAni %08x)\n", m_pEffect);
							break;
						}
						it++;
					}
#else
					g_pD3dApp->m_pEffectRender->EraseRemoveZEnableEffect((Effect*)m_pEffect);
#endif
				}
				((CObjectAni*)m_pEffect)->DeleteDeviceObjects();
				CObjectAni* p = (CObjectAni*)m_pEffect;
				util::del(p);
				m_pEffect = NULL;
			}
		}
		break;
	case EFFECT_TYPE_SPRITE:
		{
			if(m_pEffect)
			{
				if(((CSpriteAni*)m_pEffect)->m_bZbufferEnable && 
					g_pD3dApp->m_pEffectRender)
				{
#ifndef FPSIMPROVEMENTS
					vector<Effect*>::iterator it = g_pD3dApp->m_pEffectRender->m_vecZEnableEffect.begin();
					while(it != g_pD3dApp->m_pEffectRender->m_vecZEnableEffect.end())
					{
						if(m_pEffect == (char*)*it)
						{
							g_pD3dApp->m_pEffectRender->m_vecZEnableEffect.erase(it);
							DBGOUT("ZBufferEnable Effect Erase(CSpriteAni %08x)\n", m_pEffect);
							break;
						}
						it++;
					}
#else
					g_pD3dApp->m_pEffectRender->EraseRemoveZEnableEffect((Effect*)m_pEffect);
#endif
				}
				((CSpriteAni*)m_pEffect)->DeleteDeviceObjects();
				CSpriteAni* p = (CSpriteAni*)m_pEffect;
				util::del(p);
				m_pEffect = NULL;
			}
		}
		break;
	case EFFECT_TYPE_PARTICLE:
		{
			if(m_pEffect)
			{
				if(((CParticleSystem*)m_pEffect)->m_bZbufferEnable &&  g_pD3dApp->m_pEffectRender)
					
					g_pD3dApp->m_pEffectRender->EraseRemoveZEnableEffect((Effect*)m_pEffect);

				((CParticleSystem*)m_pEffect)->DeleteDeviceObjects();

				CParticleSystem* p = (CParticleSystem*)m_pEffect;

				util::del(p);

				m_pEffect = NULL;
			}
		}
		break;
	case EFFECT_TYPE_TRACE:
		{
			if(m_pEffect)
			{
				if(((CTraceAni*)m_pEffect)->m_bZbufferEnable && 
					g_pD3dApp->m_pEffectRender)
				{
#ifndef FPSIMPROVEMENTS
					vector<Effect*>::iterator it = g_pD3dApp->m_pEffectRender->m_vecZEnableEffect.begin();
					while(it != g_pD3dApp->m_pEffectRender->m_vecZEnableEffect.end())
					{
						if(m_pEffect == (char*)*it)
						{
							g_pD3dApp->m_pEffectRender->m_vecZEnableEffect.erase(it);
							DBGOUT("ZBufferEnable Effect Erase(CTraceAni %08x)\n", m_pEffect);
							break;
						}
						it++;
					}
#else
					g_pD3dApp->m_pEffectRender->EraseRemoveZEnableEffect((Effect*)m_pEffect);
#endif
				}
				((CTraceAni*)m_pEffect)->DeleteDeviceObjects();
				CTraceAni* p = (CTraceAni*)m_pEffect;
				util::del(p);
				m_pEffect = NULL;
			}
		}
		break;
	}

	m_bRestored = FALSE;
	return S_OK;
}
//////////////////////////////////////////////////////////////////////
// CBodyConditionInfo
//////////////////////////////////////////////////////////////////////

CBodyConditionInfo::CBodyConditionInfo()
{
	FLOG( "CBodyConditionInfo()" );
	m_pParent = NULL;
	m_fCurrentAnimationTime = 0;
	m_bUsingParticle = TRUE;
	// 2009. 11. 23 by jskim 리소스 로딩 구조 변경
	m_LoadingPriority = _NOTHING_PRIORITY;
	//end 2009. 11. 23 by jskim 리소스 로딩 구조 변경
	// 2010. 03. 18 by jskim 몬스터변신 카드
	m_MonsterTransformer	= 0;
	m_MonsterTransScale		= 0.0f; 
	//end 2010. 03. 18 by jskim 몬스터변신 카드

	m_fFrameSpeed = 1.0f;
}

CBodyConditionInfo::~CBodyConditionInfo()
{
	for (auto ptr : m_vecEffect) delete ptr;
	
	m_vecEffect.clear();
}
void * CBodyConditionInfo::operator new(size_t size)
{
	//DBGOUT("CBodyConditionInfo::operator new(%d)\n",CMemPoolClient::GetObjectValidSize(size));
	return CMemPoolClient::ObjectNew(size);
}
void CBodyConditionInfo::operator delete(void* p)
{
	//DBGOUT("CBodyConditionInfo::operator delete(%d)\n",CMemPoolClient::GetObjectValidSize(sizeof(CBodyConditionInfo)));
	CMemPoolClient::ObjectDelete(p,sizeof(CBodyConditionInfo));
	return;
}

void CBodyConditionInfo::InitData()
{
	FLOG( "CBodyConditionInfo::InitData()" );
	memset((void*)this,0,sizeof(BodyConditionData));
	vector<CEffectInfo*>::iterator it = m_vecEffect.begin();
	m_fCurrentAnimationTime = 0;
	while(it != m_vecEffect.end())
	{
		CEffectInfo* pEffect = *it;
		util::del(pEffect);
		it = m_vecEffect.erase(it);
	}
}

CEffectInfo* CBodyConditionInfo::GetEffectInfo(char* szEffectName)
{
	vector<CEffectInfo*>::iterator it = m_vecEffect.begin();
	while(it != m_vecEffect.end())
	{
		if(strcmp((*it)->m_strEffectName, szEffectName) == 0 )
		{
			return *it;
		}
		it ++;
	}
	return NULL;
}



BOOL CBodyConditionInfo::Tick(float fElapsedTime)
{
	FLOG( "CBodyConditionInfo::Tick(float fElapsedTime)" );

	if (m_bCharacterRendering)
	{
		if (m_fStartAnimationTime != 0 || m_fEndAnimationTime != 0)
		{
			m_fCurrentAnimationTime += fElapsedTime * m_fAnimationVel * m_fFrameSpeed;

			if (m_fCurrentAnimationTime < m_fStartAnimationTime) m_fCurrentAnimationTime = m_fStartAnimationTime;

			else if (m_fCurrentAnimationTime > m_fEndAnimationTime)
			{
				if (m_bNotAnimationLooping)
				{
					m_fCurrentAnimationTime = m_fEndAnimationTime;
				}
				else
				{
					// 2010. 11. 12. by hsLee. 프레임 튀는 버그 수정. 
					m_fCurrentAnimationTime = m_fStartAnimationTime; // + ( m_fCurrentAnimationTime - m_fEndAnimationTime );
					// end 2010. 11. 12. by hsLee. 프레임 튀는 버그 수정. 
				}
			}
		}
	}


	for (CEffectInfo* info : m_vecEffect) info->Tick(fElapsedTime);


	if(!IsUsing())
	{
		if((m_fStartAnimationTime != 0 || m_fEndAnimationTime != 0) && m_bNotAnimationLooping)
		{
			if( m_fCurrentAnimationTime >= m_fEndAnimationTime)
				return FALSE;
			else
				return TRUE;
		}
		return FALSE;
	}
	return TRUE;
}







BOOL CBodyConditionInfo::EffectStop()
{
	FLOG( "CBodyConditionInfo::EffectStop()" );
	vector<CEffectInfo*>::iterator it = m_vecEffect.begin();
	BOOL re = TRUE;
	while(it != m_vecEffect.end())
	{
		if(!(*it)->m_pEffect)
		{
			it++;
			continue;
		}
		if((*it)->m_nEffectType == EFFECT_TYPE_PARTICLE)
		{
			(*it)->ParticleStop();
			m_bUsingParticle = FALSE;
			re = FALSE;
		}
		else
		{
			CEffectInfo* p = *it;
			p->InvalidateDeviceObjects();
			p->DeleteDeviceObjects();
			continue;
		}

		it++;
	}
	return re;
}


BOOL CBodyConditionInfo::IsUsing() // EFFECT가 끝났는지 확인
{
	FLOG( "CBodyConditionInfo::IsUsing()" );
	vector<CEffectInfo*>::iterator it = m_vecEffect.begin();
	int count = 0;
	while(it != m_vecEffect.end())
	{
		if((*it)->m_pEffect == NULL)
		{
			count ++;
		}
		it++;
	}
	if(m_vecEffect.size() == count)
	{
		return FALSE;
	}
	return TRUE;
}

void CBodyConditionInfo::Render()
{
	FLOG( "CBodyConditionInfo::Render()" );
	vector<CEffectInfo*>::iterator it = m_vecEffect.begin();
	while(it != m_vecEffect.end())
	{
		(*it)->Render();
		it ++;
	}

}

void CBodyConditionInfo::SetObjectAniStartTime(float fStartObjectAniTime)
{
	FLOG( "CBodyConditionInfo::SetObjectAniStartTime(float fStartObjectAniTime)" );
	m_fCurrentAnimationTime = 0;
	vector<CEffectInfo*>::iterator it = m_vecEffect.begin();
	while(it != m_vecEffect.end())
	{
		(*it)->SetObjectAniStartTime(fStartObjectAniTime);
		it ++;
	}
}

void CBodyConditionInfo::SetInven(int nInvenWeaponIndex)
{
	FLOG( "CBodyConditionInfo::SetInven(BOOL bInven)" );
	vector<CEffectInfo*>::iterator it = m_vecEffect.begin();
	while(it != m_vecEffect.end())
	{
		(*it)->SetInven(nInvenWeaponIndex);
		it ++;
	}
}


HRESULT CBodyConditionInfo::InitDeviceObjects()
{
	FLOG( "CBodyConditionInfo::InitDeviceObjects()" );
	m_fCurrentAnimationTime = 0;
	vector<CEffectInfo*>::iterator it = m_vecEffect.begin();
	while(it != m_vecEffect.end())
	{
		(*it)->InitDeviceObjects();
		it ++;
	}
	m_bUsingParticle = TRUE;
	if(strlen(m_strSoundFileName) && m_pParent->m_bShuttleChildEffect)
	{
		DBGOUT_EFFECT("		Sound Play : [this:0x%08X][file:%s][bodycon:%I64X]\n", m_pParent, m_strSoundFileName, m_nBodyCondition );
		D3DXVECTOR3 vPos;
		vPos.x = m_mMatrix._41;
		vPos.y = m_mMatrix._42;
		vPos.z = m_mMatrix._43;
		if(m_pParent->IsUpkeepBodyCondition(m_nBodyCondition))
			g_pD3dApp->m_pSound->PlayD3DSound(m_strSoundFileName,vPos);	
		else
			g_pD3dApp->m_pSound->PlayD3DSound(m_strSoundFileName,vPos);
	}
	return S_OK;
}

HRESULT CBodyConditionInfo::RestoreDeviceObjects()
{
	FLOG( "CBodyConditionInfo::RestoreDeviceObjects()" );
	vector<CEffectInfo*>::iterator it = m_vecEffect.begin();
	while(it != m_vecEffect.end())
	{
		// 2009. 11. 23 by jskim 리소스 로딩 구조 변경
		(*it)->m_LoadingPriority = m_LoadingPriority;
		//end 2009. 11. 23 by jskim 리소스 로딩 구조 변경
		(*it)->RestoreDeviceObjects();
		it ++;
	}
	return S_OK;
}

HRESULT CBodyConditionInfo::InvalidateDeviceObjects()
{
	FLOG( "CBodyConditionInfo::InvalidateDeviceObjects()" );
	vector<CEffectInfo*>::iterator it = m_vecEffect.begin();
	while(it != m_vecEffect.end())
	{
		(*it)->InvalidateDeviceObjects();
		it ++;
	}
/*	if(strlen(m_strSoundFileName))
	{
		if(m_pParent->IsUpkeepBodyCondition(m_nBodyCondition))
			g_pD3dApp->m_pSound->StopD3DSound(m_strSoundFileName);
	}
*/	return S_OK;
}

HRESULT CBodyConditionInfo::DeleteDeviceObjects()
{
	FLOG( "CBodyConditionInfo::DeleteDeviceObjects()" );
	vector<CEffectInfo*>::iterator it = m_vecEffect.begin();
	while(it != m_vecEffect.end())
	{
		(*it)->DeleteDeviceObjects();
		it ++;
	}
	return S_OK;
}

//////////////////////////////////////////////////////////////////////
// CCharacterInfo
//////////////////////////////////////////////////////////////////////

CCharacterInfo::CCharacterInfo()
{
	FLOG( "CCharacterInfo()" );
	memset((char*)this,0,sizeof(CharacterData));
	m_nCurrentBodyCondition = 0;
	m_pCharacterAnimationBodyCondition = NULL;
	m_hyCharacterAnimationBodyConditionFlag = 0;
	m_bShuttleChildEffect = TRUE;
	// 2009. 11. 23 by jskim 리소스 로딩 구조 변경
	m_LoadingPriority = _NOTHING_PRIORITY;
	//end 2009. 11. 23 by jskim 리소스 로딩 구조 변경

	// 2010. 03. 18 by jskim 몬스터변신 카드
	m_MonsterTransformer	= 0;
	m_MonsterTransScale		= 0.0f;
	//end 2010. 03. 18 by jskim 몬스터변신 카드
}

CCharacterInfo::~CCharacterInfo()
{
	FLOG( "~CCharacterInfo()" );
	map<BodyCond_t,CBodyConditionInfo*>::iterator it = m_mapBodyCondition.begin();

	while(it != m_mapBodyCondition.end())
	{
		CBodyConditionInfo* pBody = it->second;
		util::del(pBody);
		it++;
	}		
	m_mapBodyCondition.clear();
}

void CCharacterInfo::InitData()
{
	FLOG( "CCharacterInfo::InitData()" );
	memset((void*)this,0,sizeof(CharacterData));
	map<BodyCond_t,CBodyConditionInfo*>::iterator it = m_mapBodyCondition.begin();

	while(it != m_mapBodyCondition.end())
	{
		CBodyConditionInfo* pBody = it->second;
		util::del(pBody);
		it++;
	}		
	m_mapBodyCondition.clear();
	m_vecCurrentBodyCondition.clear();
}

D3DXVECTOR3 CCharacterInfo::GetEffectPos( BodyCond_t hyBody, char* szEffectName )
{
	map<BodyCond_t,CBodyConditionInfo*>::iterator it = m_mapBodyCondition.find(hyBody);
	if(it != m_mapBodyCondition.end())
	{
		CEffectInfo* pInfo = it->second->GetEffectInfo( szEffectName );
		if(pInfo)
		{
			return pInfo->m_vPos;
		}
	}
	return D3DXVECTOR3(0,0,0);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			CCharacterInfo::GetEffectMatrix(BodyCond_t hyBody, char *szEffectName)
/// \brief		메트릭스 가져오기
/// \author		ispark
/// \date		2006-01-02 ~ 2006-01-02
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
D3DXMATRIX CCharacterInfo::GetEffectMatrix(BodyCond_t hyBody, char *szEffectName)
{
	D3DXMATRIX matEffect;
	D3DXMatrixIdentity(&matEffect);
	
	map<BodyCond_t,CBodyConditionInfo*>::iterator it = m_mapBodyCondition.find(hyBody);
	if(it != m_mapBodyCondition.end())
	{
		CEffectInfo* pInfo = it->second->GetEffectInfo( szEffectName );
		if(pInfo)
		{
			D3DXMatrixLookAtRH(&matEffect, &pInfo->m_vPos, &pInfo->m_vTarget, &D3DXVECTOR3(0, 1, 0));
			D3DXMatrixInverse(&matEffect, NULL, &matEffect );			
			return matEffect;
		}
	}
	return matEffect;
}

void CCharacterInfo::SetInven(int nInvenWeaponIndex)
{
	map<BodyCond_t,CBodyConditionInfo*>::iterator it = m_mapBodyCondition.begin();

	while(it != m_mapBodyCondition.end())
	{
		CBodyConditionInfo* pBody = it->second;
		pBody->SetInven(nInvenWeaponIndex);
		it++;
	}		
}

void CCharacterInfo::SetShuttleChildEffect(BOOL bSet)
{ 
	m_bShuttleChildEffect = bSet; 
	map<BodyCond_t,CBodyConditionInfo*>::iterator it = m_mapBodyCondition.begin();

	while(it != m_mapBodyCondition.end())
	{
		CBodyConditionInfo* pBody = it->second;
		vector<CEffectInfo*>::iterator itEffect = pBody->m_vecEffect.begin();
		while( itEffect != pBody->m_vecEffect.end())
		{
			if( strcmp((*itEffect)->m_strEffectName, "06400125.eff") == 0 ||
				strcmp((*itEffect)->m_strEffectName, "06400126.eff") == 0 ||
				strcmp((*itEffect)->m_strEffectName, "06400127.eff") == 0 )
			{
				(*itEffect)->InvalidateDeviceObjects();
				(*itEffect)->DeleteDeviceObjects();
				util::del(*itEffect);
				itEffect = pBody->m_vecEffect.erase(itEffect);
				pBody->m_nEffectNumber--;
				continue;
			}
  			itEffect++;
		}
		it++;
	}		
}

BOOL CCharacterInfo::Tick(float fElapsedTime)
{
	FLOG( "CCharacterInfo::Tick(float fElapsedTime)" );
	set<BodyCond_t>::iterator itCurrent = m_vecCurrentBodyCondition.begin();
	while(itCurrent != m_vecCurrentBodyCondition.end())
	{
		BodyCond_t hySingleBodyCondition = (*itCurrent);
//		if(COMPARE_BODYCON_BIT(m_nCurrentBodyCondition,hySingleBodyCondition))
//		{
		map<BodyCond_t,CBodyConditionInfo*>::iterator it = m_mapBodyCondition.find(hySingleBodyCondition);
		if(it != m_mapBodyCondition.end())
		{
			if(!(it->second)->Tick(fElapsedTime))
		{
			(it->second)->InvalidateDeviceObjects();
			(it->second)->DeleteDeviceObjects();
			itCurrent = m_vecCurrentBodyCondition.erase(itCurrent);
			m_nCurrentBodyCondition &= ~hySingleBodyCondition;
			continue;
		}
	}
//		}

		itCurrent++;
	}
	return TRUE;
}
	





void CCharacterInfo::Render()
{
	FLOG( "CCharacterInfo::Render()" );
	set<BodyCond_t>::iterator itCurrent = m_vecCurrentBodyCondition.begin();
	while(itCurrent != m_vecCurrentBodyCondition.end())
	{
		BodyCond_t hySingleBodyCondition = (*itCurrent);
//		if(COMPARE_BODYCON_BIT(m_nCurrentBodyCondition,hySingleBodyCondition))
//		{
#ifdef _DEBUG
					if(hySingleBodyCondition == 0x0000000000000200)
					{
						DBGOUT("---------------Render BOOSTER3\n");
					}
#endif
			map<BodyCond_t,CBodyConditionInfo*>::iterator it = m_mapBodyCondition.find(hySingleBodyCondition);
			if(it != m_mapBodyCondition.end())
			{
				(it->second)->Render();
			}
//		}
		itCurrent++;
	}

}

// bodycondition이 중간에 바뀌는 경우 : 모든 이펙트를 렌더한다.
void CCharacterInfo::ChangeBodyCondition(BodyCond_t hyBody)
{
	FLOG( "CCharacterInfo::ChangeBodyCondition(BodyCond_t hyBody)" );
	BodyCond_t hyNewBodyCondition = ~m_nCurrentBodyCondition & hyBody;
	BodyCond_t hyDeleteBodyCondition = m_nCurrentBodyCondition & ~hyBody;
	BodyCond_t hyBodyCondition = hyBody;
	BodyCond_t hyRemainBodyCondition = m_nCurrentBodyCondition & hyBody;
	SetCharacterAnimationBodyCondition( NULL );
	DBGOUT_EFFECT("    ChangeBodyCondition[this:0x%08X][bodycon: %I64X --> %I64X ]\n", this, m_nCurrentBodyCondition, hyBody );
//	m_nCurrentBodyCondition = hyBody;
	if(hyDeleteBodyCondition != 0 )
	{
		for(int i=0;i<64;i++)
		{
			BodyCond_t hySingleBodyCondition = 1;
			hySingleBodyCondition <<= i;
			if(COMPARE_BODYCON_BIT(hyDeleteBodyCondition,hySingleBodyCondition))
			{
				map<BodyCond_t,CBodyConditionInfo*>::iterator it = m_mapBodyCondition.find(hySingleBodyCondition); // 2016-01-08 exception at this point
				if(it == m_mapBodyCondition.end())
				{
	//				hyBodyCondition = ~hySingleBodyCondition;
					continue;
				}
				if(!IsUpkeepBodyCondition(hySingleBodyCondition))// 지속형이 아닐때
				{
					hyBodyCondition |= hySingleBodyCondition;
					continue;// 계속 플레이한다.
				} 
				else if((it->second)->IsUsing())
				{
					if(!(it->second)->EffectStop())
					{
						hyBodyCondition |= hySingleBodyCondition;
						continue;// 사용중이면 계속 플레이한다.
					}
				}

				set<BodyCond_t>::iterator itCurrent = m_vecCurrentBodyCondition.begin();
				while(itCurrent != m_vecCurrentBodyCondition.end())
				{
					if((*itCurrent) == hySingleBodyCondition)
					{
						m_vecCurrentBodyCondition.erase(itCurrent);
						(it->second)->InvalidateDeviceObjects();
						(it->second)->DeleteDeviceObjects();
						break;
					}
					itCurrent++;
				}
			}
		}
	}

	if(hyNewBodyCondition != 0)
	{
		for(int i=0;i<64;i++)
		{
			BodyCond_t hySingleBodyCondition = 1;
			hySingleBodyCondition <<= i;
			if(COMPARE_BODYCON_BIT(hyNewBodyCondition,hySingleBodyCondition))
			{
				m_vecCurrentBodyCondition.insert(hySingleBodyCondition); // 2016-01-08 exception at this point
				map<BodyCond_t,CBodyConditionInfo*>::iterator it = m_mapBodyCondition.find(hySingleBodyCondition);
				if(it != m_mapBodyCondition.end())
				{
					(it->second)->InitDeviceObjects();

					// 2009. 11. 23 by jskim 리소스 로딩 구조 변경
					(it->second)->m_LoadingPriority = m_LoadingPriority;
					//end 2009. 11. 23 by jskim 리소스 로딩 구조 변경
						
					(it->second)->RestoreDeviceObjects();
					if(COMPARE_BODYCON_BIT(hySingleBodyCondition, m_hyCharacterAnimationBodyConditionFlag))
					{
						SetCharacterAnimationBodyCondition( it->second );
					}

				}
#ifdef _DBGOUT_EFFECT
				else
				{
					DBGOUT_EFFECT("ERROR : CCharacterInfo::ChangeBodyCondition, Wrong BodyCondition[this:0x%08X][bodycon:%I64X]\n",this, hySingleBodyCondition);
				}
#endif
			}
		}
	}
	if( hyRemainBodyCondition != 0 &&
		!(hyNewBodyCondition & m_hyCharacterAnimationBodyConditionFlag) &&
		hyRemainBodyCondition & m_hyCharacterAnimationBodyConditionFlag	)
	{
		for(int i=0;i<64;i++)
		{
			BodyCond_t hySingleBodyCondition = 1;
			hySingleBodyCondition <<= i;
			if(COMPARE_BODYCON_BIT(hyRemainBodyCondition,hySingleBodyCondition) && 
				COMPARE_BODYCON_BIT(m_hyCharacterAnimationBodyConditionFlag,hySingleBodyCondition))
			{
				map<BodyCond_t,CBodyConditionInfo*>::iterator it = m_mapBodyCondition.find(hySingleBodyCondition);
				if(it == m_mapBodyCondition.end())
				{
					continue;
				}
				SetCharacterAnimationBodyCondition( it->second );
				break;
			}
		}
	}
	m_nCurrentBodyCondition = hyBodyCondition;
}


// bodycondition을 처음 받아오는 경우 : 지속형의 이펙트만 보여준다.
void CCharacterInfo::SetBodyCondition(BodyCond_t hyBody)
{
	FLOG( "CCharacterInfo::SetBodyCondition(BodyCond_t hyBody)" );
	DBGOUT_EFFECT("    SetBodyCondition[this:0x%08X][bodycon: %I64X ]\n", this, hyBody );
	m_nCurrentBodyCondition = hyBody;
	m_vecCurrentBodyCondition.clear();
	SetCharacterAnimationBodyCondition( NULL );
	for(int i=0;i<64;i++)
	{
		BodyCond_t hySingleBodyCondition = 1;
		hySingleBodyCondition <<= i;
		if(COMPARE_BODYCON_BIT(m_nCurrentBodyCondition,hySingleBodyCondition))
		{
			m_vecCurrentBodyCondition.insert(hySingleBodyCondition);
			map<BodyCond_t,CBodyConditionInfo*>::iterator it = m_mapBodyCondition.find(hySingleBodyCondition);
			if(it != m_mapBodyCondition.end() && IsUpkeepBodyCondition(hySingleBodyCondition))
			{
				(it->second)->InitDeviceObjects();
				// 2009. 11. 23 by jskim 리소스 로딩 구조 변경
				(it->second)->m_LoadingPriority = m_LoadingPriority;
				//end 2009. 11. 23 by jskim 리소스 로딩 구조 변경
				(it->second)->RestoreDeviceObjects();				
				if(COMPARE_BODYCON_BIT(hySingleBodyCondition, m_hyCharacterAnimationBodyConditionFlag))
				{
					SetCharacterAnimationBodyCondition( it->second );
				}
			}
		}
	}
}

// 같은 바디컨디션 애니메이션을 새로 시작된다. 지속형이라도(IsUpkeepBodyCondition()=FALSE) 처음부터 수행한다.
void CCharacterInfo::ResetBodyCondition(BodyCond_t hyBody)
{
	FLOG( "CCharacterInfo::ResetBodyCondition(BodyCond_t hyBody)" );
	DBGOUT_EFFECT("    ResetBodyCondition[this:0x%08X][bodycon: %I64X --> %I64X ]\n", this, m_nCurrentBodyCondition, hyBody );
	BodyCond_t hyBodyCondition = hyBody;
//	BodyCond_t hyRemainBodyCondition = m_nCurrentBodyCondition & hyBody;
	BodyCond_t hyDeleteBodyCondition = m_nCurrentBodyCondition & ~hyBody;
//	BodyCond_t hyNewBodyCondition = ~m_nCurrentBodyCondition & hyBody;
	SetCharacterAnimationBodyCondition( NULL );
	if(hyDeleteBodyCondition != 0 )
	{
		for(int i=0;i<64;i++)
		{
			BodyCond_t hySingleBodyCondition = 1;
			hySingleBodyCondition <<= i;
			if(COMPARE_BODYCON_BIT(hyDeleteBodyCondition,hySingleBodyCondition))
			{
				map<BodyCond_t,CBodyConditionInfo*>::iterator it = m_mapBodyCondition.find(hySingleBodyCondition);
				if(it == m_mapBodyCondition.end())
				{
	//				hyBodyCondition = ~hySingleBodyCondition;
					continue;
				}
				if(!IsUpkeepBodyCondition(hySingleBodyCondition))// 지속형이 아닐때
				{
					hyBodyCondition |= hySingleBodyCondition;
						continue;// 계속 플레이한다.
				} 
				else if((it->second)->IsUsing())
				{
					if(!(it->second)->EffectStop())
					{
						hyBodyCondition |= hySingleBodyCondition;
						continue;// 사용중이면 계속 플레이한다.
					}
				}

				set<BodyCond_t>::iterator itCurrent = m_vecCurrentBodyCondition.begin();
				while(itCurrent != m_vecCurrentBodyCondition.end())
				{
					if((*itCurrent) == hySingleBodyCondition)
					{
						m_vecCurrentBodyCondition.erase(itCurrent);
						(it->second)->InvalidateDeviceObjects();
						(it->second)->DeleteDeviceObjects();
						break;
					}
					itCurrent++;
				}
			}
		}
	}
	m_nCurrentBodyCondition = hyBody;
	m_vecCurrentBodyCondition.clear();
	for(int i=0;i<64;i++)
	{
		BodyCond_t hySingleBodyCondition = 1;
		hySingleBodyCondition <<= i;
		if(COMPARE_BODYCON_BIT(m_nCurrentBodyCondition,hySingleBodyCondition))
		{
			m_vecCurrentBodyCondition.insert(hySingleBodyCondition);
			map<BodyCond_t,CBodyConditionInfo*>::iterator it = m_mapBodyCondition.find(hySingleBodyCondition);
			if(it != m_mapBodyCondition.end())
			{
				(it->second)->InitDeviceObjects();// 이펙트는 이 안에서 자동으로 Invalidate, delete된다.
				// 2009. 11. 23 by jskim 리소스 로딩 구조 변경
				(it->second)->m_LoadingPriority = m_LoadingPriority;
				//end 2009. 11. 23 by jskim 리소스 로딩 구조 변경
				(it->second)->RestoreDeviceObjects();
				if(COMPARE_BODYCON_BIT(hySingleBodyCondition, m_hyCharacterAnimationBodyConditionFlag))
				{
					SetCharacterAnimationBodyCondition( it->second );
				}
			}
		}
	}
}

BOOL CCharacterInfo::IsUpkeepBodyCondition(BodyCond_t hyBody)
{
	FLOG( "CCharacterInfo::IsUpkeepBodyCondition(BodyCond_t hyBody)" );
	if( hyBody == BODYCON_EXPLOSION_MASK)// 폭발
		return FALSE;
	if( hyBody == BODYCON_HIT_MASK)// 타격
		return FALSE;
	if( hyBody == BODYCON_FIRE_MASK)//발사
		return FALSE;
	return TRUE;
}

void CCharacterInfo::SetObjectAniStartTime(float fStartObjectAniTime)
{
	FLOG( "CCharacterInfo::SetObjectAniStartTime(float fStartObjectAniTime)" );
	set<BodyCond_t>::iterator itCurrent = m_vecCurrentBodyCondition.begin();
	while(itCurrent != m_vecCurrentBodyCondition.end())
	{
		BodyCond_t hySingleBodyCondition = (*itCurrent);
//		if(COMPARE_BODYCON_BIT(m_nCurrentBodyCondition,hySingleBodyCondition))
		{
			map<BodyCond_t,CBodyConditionInfo*>::iterator it = m_mapBodyCondition.find(hySingleBodyCondition);
			if(it != m_mapBodyCondition.end() && IsUpkeepBodyCondition(hySingleBodyCondition))
			{
				(it->second)->SetObjectAniStartTime(fStartObjectAniTime);
			}
		}
		itCurrent++;
	}
}


HRESULT CCharacterInfo::InitDeviceObjects()
{
	FLOG( "CCharacterInfo::InitDeviceObjects()" );
	set<BodyCond_t>::iterator itCurrent = m_vecCurrentBodyCondition.begin();
	while(itCurrent != m_vecCurrentBodyCondition.end())
	{
		BodyCond_t hySingleBodyCondition = (*itCurrent);
//		if(COMPARE_BODYCON_BIT(m_nCurrentBodyCondition,hySingleBodyCondition))
		{
			map<BodyCond_t,CBodyConditionInfo*>::iterator it = m_mapBodyCondition.find(hySingleBodyCondition);
			if(it != m_mapBodyCondition.end() && IsUpkeepBodyCondition(hySingleBodyCondition))
			{
				(it->second)->InitDeviceObjects();
			}
		}
		itCurrent++;
	}
	return S_OK;
}

HRESULT CCharacterInfo::RestoreDeviceObjects()
{
	FLOG( "CCharacterInfo::RestoreDeviceObjects()" );
	set<BodyCond_t>::iterator itCurrent = m_vecCurrentBodyCondition.begin();
	while(itCurrent != m_vecCurrentBodyCondition.end())
	{
		BodyCond_t hySingleBodyCondition = (*itCurrent);
//		if(COMPARE_BODYCON_BIT(m_nCurrentBodyCondition,hySingleBodyCondition))
		{
			map<BodyCond_t,CBodyConditionInfo*>::iterator it = m_mapBodyCondition.find(hySingleBodyCondition);
			if(it != m_mapBodyCondition.end() && IsUpkeepBodyCondition(hySingleBodyCondition))
			{
				(it->second)->RestoreDeviceObjects();
			}
		}
		itCurrent++;
	}
	return S_OK;
}

HRESULT CCharacterInfo::InvalidateDeviceObjects()
{
	FLOG( "CCharacterInfo::InvalidateDeviceObjects()" );
	set<BodyCond_t>::iterator itCurrent = m_vecCurrentBodyCondition.begin();
	while(itCurrent != m_vecCurrentBodyCondition.end())
	{
		BodyCond_t hySingleBodyCondition = (*itCurrent);
//		if(COMPARE_BODYCON_BIT(m_nCurrentBodyCondition,hySingleBodyCondition))
		{
			map<BodyCond_t,CBodyConditionInfo*>::iterator it = m_mapBodyCondition.find(hySingleBodyCondition);
			if(it != m_mapBodyCondition.end())
			{
				(it->second)->InvalidateDeviceObjects();
			}
		}
		itCurrent++;
	}
	return S_OK;
}

HRESULT CCharacterInfo::DeleteDeviceObjects()
{
	FLOG( "CCharacterInfo::DeleteDeviceObjects()" );
	set<BodyCond_t>::iterator itCurrent = m_vecCurrentBodyCondition.begin();
	while(itCurrent != m_vecCurrentBodyCondition.end())
	{
		BodyCond_t hySingleBodyCondition = (*itCurrent);
//		if(COMPARE_BODYCON_BIT(m_nCurrentBodyCondition,hySingleBodyCondition))
		{
			map<BodyCond_t,CBodyConditionInfo*>::iterator it = m_mapBodyCondition.find(hySingleBodyCondition);
			if(it != m_mapBodyCondition.end())
			{
				(it->second)->DeleteDeviceObjects();
			}
		}
		itCurrent++;
	}
	return S_OK;
}
// 2010. 03. 18 by jskim 몬스터변신 카드
//BOOL CCharacterInfo::Load(char* strName)
BOOL CCharacterInfo::Load(char* strName,int nMonsterTransformer /* = 0 */, float nMonsterTransScale /* = 0.0f */)
//end 2010. 03. 18 by jskim 몬스터변신 카드
{
	FLOG( "CCharacterInfo::Load(char* strName)" );
	DBGOUT_EFFECT(" Character file[this:0x%08X][name:%s]\n", this, strName );	
	
	if (!g_pD3dApp->m_pEffectRender) return FALSE;
 
	DataHeader* pDataHeader = g_pD3dApp->m_pEffectRender->FindObjectInfo(strName);
	if (!pDataHeader) return FALSE;
//	{
//		DBGOUT("이펙트 파일이 없습니다.[%s](objectInfo.inf)\n", strName);
//		return FALSE;
//	}
	
	char* pData = pDataHeader->m_pData;
	memcpy((void*)this, pData, sizeof(CharacterData));
	pData += sizeof(CharacterData);
	// 바디 컨디션 정보
	for (int i = 0; i < m_nBodyConditionNumber; i++)
	{
		CBodyConditionInfo* pBody = new CBodyConditionInfo;
		memcpy((void*)pBody, pData, sizeof(BodyConditionData));
		pData += sizeof(BodyConditionData);
		pBody->m_pParent = this;
		m_mapBodyCondition[pBody->m_nBodyCondition] = pBody;
		// 이펙트 정보
		DBGOUT_EFFECT("	BodyCondition[%I64X]\n", pBody->m_nBodyCondition );
		for (int j = 0; j < pBody->m_nEffectNumber; j++)
		{
			CEffectInfo* pEffect = new CEffectInfo;
			memcpy((void*)pEffect, pData, sizeof(EffectData));
			pData += sizeof(EffectData);
			pEffect->m_pParent = pBody;
			// 2010. 03. 18 by jskim 몬스터변신 카드
			if (nMonsterTransformer)
			{
				pEffect->m_vPos.x *= nMonsterTransScale;
				pEffect->m_vPos.y *= nMonsterTransScale;
				pEffect->m_vPos.z *= nMonsterTransScale;
			}
			//end 2010. 03. 18 by jskim 몬스터변신 카드

			pBody->m_vecEffect.push_back(pEffect);
			DBGOUT_EFFECT("		Effect[%s]\n", pEffect->m_strEffectName);
		}
	}
	return TRUE;

}

BOOL CCharacterInfo::IsUsing()
{
	FLOG( "CCharacterInfo::IsUsing()" );
	set<BodyCond_t>::iterator it = m_vecCurrentBodyCondition.begin();
	while(it != m_vecCurrentBodyCondition.end())
	{
		if(IsUsing((*it)))
		{
			return TRUE;
		}
		it++;
	}
	return FALSE;

}

BOOL CCharacterInfo::IsUsing(BodyCond_t hyBody)
{
	FLOG( "CCharacterInfo::IsUsing(BodyCond_t hyBody)" );
	map<BodyCond_t,CBodyConditionInfo*>::iterator it = m_mapBodyCondition.find(hyBody);

	if(it != m_mapBodyCondition.end())
	{
		if((it->second)->IsUsing())
			return TRUE;
	}
	return FALSE;

}

void CCharacterInfo::SetSingleBodyConditionMatrix(BodyCond_t hyBody, D3DXMATRIX m)
{
	FLOG( "CCharacterInfo::SetSingleBodyConditionMatrix(BodyCond_t hyBody, D3DXMATRIX m)" );
	map<BodyCond_t,CBodyConditionInfo*>::iterator it = m_mapBodyCondition.find(hyBody);

	if(it != m_mapBodyCondition.end())
	{
		(it->second)->m_mMatrix = m;
	}
	

}

void CCharacterInfo::SetAllBodyConditionMatrix(D3DXMATRIX m)
{
	FLOG( "CCharacterInfo::SetAllBodyConditionMatrix(D3DXMATRIX m)" );
	map<BodyCond_t,CBodyConditionInfo*>::iterator it = m_mapBodyCondition.begin();
	while(it != m_mapBodyCondition.end())
	{
		// 2010. 03. 18 by jskim 몬스터변신 카드
		(it->second)->m_MonsterTransformer	= m_MonsterTransformer;
		(it->second)->m_MonsterTransScale	= m_MonsterTransScale;
		//end 2010. 03. 18 by jskim 몬스터변신 카드
		(it->second)->m_mMatrix = m;
		it++;
	}
	

}


///////////////////////////////////////////////////////////////////////////////
/// \fn			CCharacterInfo::TurnOnSingleBodyCondition(BodyCond_t hySingleBodyCondition)
/// \brief		BodyCon 하나를 켠다.
/// \author		dhkwon
/// \date		2004-04-09 ~ 2004-04-09
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CCharacterInfo::TurnOnSingleBodyCondition(BodyCond_t hySingleBodyCondition)
{
	map<BodyCond_t,CBodyConditionInfo*>::iterator it = m_mapBodyCondition.find(hySingleBodyCondition);
	m_nCurrentBodyCondition |= hySingleBodyCondition;
	if(it != m_mapBodyCondition.end())
	{
		if(!it->second->m_vecEffect.empty())
		{
			m_vecCurrentBodyCondition.insert(hySingleBodyCondition);
			(it->second)->InitDeviceObjects();// 이펙트는 이 안에서 자동으로 Invalidate, delete된다.
			// 2009. 11. 23 by jskim 리소스 로딩 구조 변경
			(it->second)->m_LoadingPriority = m_LoadingPriority;
			//end 2009. 11. 23 by jskim 리소스 로딩 구조 변경
			(it->second)->RestoreDeviceObjects();
			if(COMPARE_BODYCON_BIT(hySingleBodyCondition, m_hyCharacterAnimationBodyConditionFlag))
			{
				SetCharacterAnimationBodyCondition( it->second );
			}
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			CCharacterInfo::TurnOffSingleBodyCondition(BodyCond_t hySingleBodyCondition)
/// \brief		BodyCon 하나를 끈다.
/// \author		dhkwon
/// \date		2004-04-09 ~ 2004-04-09
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CCharacterInfo::TurnOffSingleBodyCondition(BodyCond_t hySingleBodyCondition)
{
	set<BodyCond_t>::iterator itSetSingle = m_vecCurrentBodyCondition.find(hySingleBodyCondition);
	m_nCurrentBodyCondition &= ~hySingleBodyCondition;
	if(itSetSingle != m_vecCurrentBodyCondition.end())
	{
		if(COMPARE_BODYCON_BIT(hySingleBodyCondition, m_hyCharacterAnimationBodyConditionFlag))
		{
			SetCharacterAnimationBodyCondition( NULL );
		}
		m_vecCurrentBodyCondition.erase(itSetSingle);
		map<BodyCond_t,CBodyConditionInfo*>::iterator itBodyCon = m_mapBodyCondition.find(hySingleBodyCondition);
		if(itBodyCon != m_mapBodyCondition.end())
		{
			if(!IsUpkeepBodyCondition(hySingleBodyCondition))// 지속형이 아닐때
			{
				//m_nCurrentBodyCondition |= hySingleBodyCondition;
				m_vecCurrentBodyCondition.insert( hySingleBodyCondition );
				return;// 계속 플레이한다.
			} 
			else if((itBodyCon->second)->IsUsing())
			{
				if(!(itBodyCon->second)->EffectStop())
				{
					//m_nCurrentBodyCondition |= hySingleBodyCondition;
					m_vecCurrentBodyCondition.insert( hySingleBodyCondition );
					return;// 사용중이면 계속 플레이한다.
				}
			}
			(itBodyCon->second)->InvalidateDeviceObjects();
			(itBodyCon->second)->DeleteDeviceObjects();
		}
	}

}

///////////////////////////////////////////////////////////////////////////////
/// \fn			CCharacterInfo::FindBodyConditionInfo(BodyCond_t hyBody)
/// \brief		바디 컨디션 정보를 찾는다.
/// \author		ispark
/// \date		2006-01-23 ~ 2006-01-23
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
CBodyConditionInfo* CCharacterInfo::FindBodyConditionInfo(BodyCond_t hyBody)
{
	map<BodyCond_t,CBodyConditionInfo*>::iterator it = m_mapBodyCondition.find(hyBody);
	if(it != m_mapBodyCondition.end())
	{
		return it->second;
	}

	return NULL;
}

// 2010. 02. 04 by ckPark 공격 준비 도중에 죽은 몬스터 애니메이션 버그 수정
void	CCharacterInfo::ForceTurnOffBodyCondition( BodyCond_t nBodyCon )
{
	for( int i=0; i<64; ++i )
	{
		BodyCond_t nDeleteBodyCon = m_nCurrentBodyCondition & (nBodyCon >> i);
		if( nDeleteBodyCon )
		{
			m_nCurrentBodyCondition &= ~nDeleteBodyCon;
			
			set<BodyCond_t>::iterator itSetSingle = m_vecCurrentBodyCondition.find(nDeleteBodyCon);
			if(itSetSingle != m_vecCurrentBodyCondition.end())
			{
				if(COMPARE_BODYCON_BIT(nDeleteBodyCon, m_hyCharacterAnimationBodyConditionFlag))
				{
					SetCharacterAnimationBodyCondition( NULL );
				}
				
				itSetSingle = m_vecCurrentBodyCondition.erase(itSetSingle);
				map<BodyCond_t,CBodyConditionInfo*>::iterator itBodyCon = m_mapBodyCondition.find(nDeleteBodyCon);
				if(itBodyCon != m_mapBodyCondition.end())
				{
					(itBodyCon->second)->InvalidateDeviceObjects();
					(itBodyCon->second)->DeleteDeviceObjects();
				}
			}
		}
	}
}
// end 2010. 02. 04 by ckPark 공격 준비 도중에 죽은 몬스터 애니메이션 버그 수정