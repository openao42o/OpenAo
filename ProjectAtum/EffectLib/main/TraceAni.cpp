// TraceAni.cpp: implementation of the CTraceAni class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TraceAni.h"
//#include "AtumApplication.h"
#include "DbgOut_C.h"
#include "CharacterInfo.h"
#include "Camera.h"
//#include "dxutil.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CEffectPlane::CEffectPlane(CTraceAni* pParent, int nIBIndex)
{
	FLOG("CEffectPlane::CEffectPlane(CTraceAni* pParent, int nIBIndex )");
	m_pParent = pParent;
	m_vPos = D3DXVECTOR3(0, 0, 0);
	m_nIBIndex = nIBIndex;
	m_bIsFirstIndex = FALSE;
	if (pParent) m_bZbufferEnable = pParent->m_bZbufferEnable;
	dwType = EFFECT_TYPE_TRACE;
	fDistance = 0;
	//m_pVB = NULL;
}

CEffectPlane::~CEffectPlane()
{
	FLOG("CEffectPlane::~CEffectPlane()");
}

HRESULT CEffectPlane::InitDeviceObjects()
{
	FLOG("CEffectPlane::InitDeviceObjects()");
	return S_OK;
}

HRESULT CEffectPlane::RestoreDeviceObjects()
{
	FLOG("CEffectPlane::RestoreDeviceObjects()");

	//if (FAILED(g_pD3dDev->CreateVertexBuffer(sizeof(SPRITE_VERTEX) * 4 * PLANE_VB_SIZE, 0, D3DFVF_SPRITE_VERTEX, D3DPOOL_MANAGED, &m_pVB, nullptr))) return E_FAIL;

	return S_OK;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			CEffectPlane::SetEndIndex()
/// \brief		잔상 꼬리의 마지막 플랜임을 표시한다. 색이 서서히 없어짐
/// \author		dhkwon
/// \date		2004-10-15 ~ 2004-10-15
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CEffectPlane::SetEndIndex()
{
	m_TraceInstance.newscale = 0.8f;
	m_TraceInstance.oldscale = 0.2f;

	m_TraceInstance.newcol = m_pParent->GetAlphaValue() * 0.6f;
	m_TraceInstance.oldcol = 0.0f;

	//SPRITE_VERTEX* v;

	//m_pVB->Lock(0, 0, (void**)&v, 0);

	//for (int i = 0; i < PLANE_VB_SIZE / 2; i++)
	//{
	//	//if (i % 2 == 0)
	//	//{
	//		v[i * 8 + 0].c = 0x00000000;
	//		v[i * 8 + 1].c = 0x33333333;
	//		v[i * 8 + 2].c = 0x00000000;
	//		v[i * 8 + 3].c = 0x33333333;
	//	//}
	//	//else
	//	//{
	//		v[i * 8 + 4].c = 0x00000000;
	//		v[i * 8 + 5].c = 0x00000000;
	//		v[i * 8 + 6].c = 0x33333333;
	//		v[i * 8 + 7].c = 0x33333333;
	//	//}
	//}
	//m_pVB->Unlock();
}

// Use this when you dont want to use SetEndIndex + SetEndIndex2 combo
void CEffectPlane::SetEndIndexOriginal()
{
	m_TraceInstance.newscale = 0.1f;
	m_TraceInstance.oldscale = 0.7f;

	m_TraceInstance.newcol = m_pParent->GetAlphaValue();
	m_TraceInstance.oldcol = 0.0f;
}

// Sets the index before last index
void CEffectPlane::SetEndIndex2()
{
	m_TraceInstance.newscale = 1.0f;
	m_TraceInstance.oldscale = 0.8f;
	m_TraceInstance.newcol = m_pParent->GetAlphaValue();
	m_TraceInstance.oldcol = m_pParent->GetAlphaValue() * 0.6;
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			VOID CEffectPlane::SetFirstIndex(BOOL bSet)
/// \brief		최초 생성한 유닛이면 bSet=TRUE으로 좌표계산,
///				그렇지 않으면 bSet=FALSE로 컬러만 바꿔준다.
/// \author		dhkwon
/// \date		2004-03-30 ~ 2004-03-30
/// \warning	RestoreDeviceObjects이후에 반드시 불러줘야 한다.
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CEffectPlane::SetFirstIndex(bool bSet)
{
	FLOG("CEffectPlane::SetFirstIndex(BOOL bSet)");
	m_bIsFirstIndex = bSet;
	D3DXVECTOR3 vSide[PLANE_VB_SIZE / 2];
	vSide[0] = D3DXVECTOR3(m_pParent->m_pParent->m_pParent->m_mMatrix._11, m_pParent->m_pParent->m_pParent->m_mMatrix._12, m_pParent->m_pParent->m_pParent->m_mMatrix._13);
	D3DXVECTOR3 vTarget = m_pParent->m_vCurrentPos - m_pParent->m_vOldPos;
	D3DXVec3Normalize(&vSide[0], &vSide[0]);
	vSide[0] = m_pParent->m_fHalfSize * vSide[0];
	D3DXMATRIX mat;

	for (int i = 1; i < PLANE_VB_SIZE / 2; i++)
	{
		D3DXMatrixRotationAxis(&mat, &vTarget, PI / (PLANE_VB_SIZE / 8) * i);
		D3DXVec3TransformCoord(&vSide[i], &vSide[0], &mat);
	}

	m_TraceInstance.newscale = bSet ? 0.5f : 1.0f;
	m_TraceInstance.oldscale = 1.0f;

	m_TraceInstance.newcol = bSet ? 0.6 : m_pParent->GetAlphaValue();
	m_TraceInstance.oldcol = m_pParent->GetAlphaValue();

	m_TraceInstance.newpos = m_pParent->m_vCurrentPos;
	m_TraceInstance.oldpos = m_pParent->m_vOldPos;
	m_TraceInstance.offset_h = vSide[0];
	m_TraceInstance.offset_v = vSide[1];

	//SPRITE_VERTEX* v;

	//m_pVB->Lock(0, 0, (void**)&v, 0);

	//for (int i = 0; i < PLANE_VB_SIZE / 2; i++)
	//{
	//	//if (i % 2 == 0)
	//	//{
	//		v[i * 8 + 0].p = m_pParent->m_vOldPos;
	//		v[i * 8 + 1].p = m_pParent->m_vCurrentPos;
	//		v[i * 8 + 2].p = m_pParent->m_vOldPos + vSide[i];
	//		v[i * 8 + 3].p = m_pParent->m_vCurrentPos + vSide[i];

	//		v[i * 8 + 0].tu = 0; v[i * 8 + 0].tv = 1;	v[i * 8 + 0].c = 0x33333333;
	//		v[i * 8 + 1].tu = 1; v[i * 8 + 1].tv = 1;	v[i * 8 + 1].c = 0x33333333;
	//		v[i * 8 + 2].tu = 0; v[i * 8 + 2].tv = 0;	v[i * 8 + 2].c = 0x33333333;
	//		v[i * 8 + 3].tu = 1; v[i * 8 + 3].tv = 0;	v[i * 8 + 3].c = 0x33333333;
	//	//}
	//	//else
	//	//{
	//		v[i * 8 + 4].p = m_pParent->m_vOldPos;
	//		v[i * 8 + 5].p = m_pParent->m_vOldPos + vSide[i];
	//		v[i * 8 + 6].p = m_pParent->m_vCurrentPos;
	//		v[i * 8 + 7].p = m_pParent->m_vCurrentPos + vSide[i];

	//		v[i * 8 + 4].tu = 0; v[i * 8 + 4].tv = 1;	v[i * 8 + 4].c = 0x33333333;
	//		v[i * 8 + 5].tu = 0; v[i * 8 + 5].tv = 0;	v[i * 8 + 5].c = 0x33333333;
	//		v[i * 8 + 6].tu = 1; v[i * 8 + 6].tv = 1;	v[i * 8 + 6].c = 0x33333333;
	//		v[i * 8 + 7].tu = 1; v[i * 8 + 7].tv = 0;	v[i * 8 + 7].c = 0x33333333;
	//	//}
	//}
	//m_pVB->Unlock();
}

BOOL CEffectPlane::Tick(float fElapsedTime)
{
	FLOG("CEffectPlane::Tick(float fElapsedTime)");

	if (m_bIsFirstIndex)
	{
		D3DXVECTOR3 vSide[PLANE_VB_SIZE / 2];
		vSide[0] = D3DXVECTOR3(m_pParent->m_pParent->m_pParent->m_mMatrix._11, m_pParent->m_pParent->m_pParent->m_mMatrix._12, m_pParent->m_pParent->m_pParent->m_mMatrix._13);
		D3DXVECTOR3 vTarget = m_pParent->m_vCurrentPos - m_pParent->m_vOldPos;
		D3DXVec3Normalize(&vSide[0], &vSide[0]);
		vSide[0] = m_pParent->m_fHalfSize * vSide[0];
		D3DXMATRIX mat;

		for (int i = 1; i < PLANE_VB_SIZE / 2; i++)
		{
			D3DXMatrixRotationAxis(&mat, &vTarget, PI / (PLANE_VB_SIZE / 8) * i);
			D3DXVec3TransformCoord(&vSide[i], &vSide[0], &mat);
		}

		m_TraceInstance.newpos = m_pParent->m_vCurrentPos;
		m_TraceInstance.offset_h = vSide[0];
		m_TraceInstance.offset_v = vSide[1];

		//SPRITE_VERTEX* v;

		//m_pVB->Lock(0, 0, (void**)&v, 0);

		//for (int i = 0; i < PLANE_VB_SIZE / 2; i++)
		//{
		//	//if (i % 2 == 0)
		//	//{
		//		v[i * 8 + 1].p = m_pParent->m_vCurrentPos;
		//		v[i * 8 + 3].p = m_pParent->m_vCurrentPos;
		//	//}
		//	//else
		//	//{
		//		v[i * 8 + 6].p = m_pParent->m_vCurrentPos;
		//		v[i * 8 + 7].p = m_pParent->m_vCurrentPos;
		//	//}
		//}
		//m_pVB->Unlock();
	}

	return TRUE;
}

void CEffectPlane::Render() // 2015-10-24 PanKJ todo: optimize
{
	// this function does nothing, use particle rendering to render traces

	//FLOG("CEffectPlane::Render()");
	//g_pD3dDev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	//g_pD3dDev->SetStreamSource(0, m_pVB, 0, sizeof(SPRITE_VERTEX));
	//g_pD3dDev->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2 * PLANE_VB_SIZE);
	//g_pD3dDev->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	//g_pD3dDev->DrawPrimitive(D3DPT_TRIANGLESTRIP, 8, 2);
	//g_pD3dDev->DrawPrimitive(D3DPT_TRIANGLESTRIP, 16, 2);
	//g_pD3dDev->DrawPrimitive(D3DPT_TRIANGLESTRIP, 24, 2);
	//g_pD3dDev->DrawPrimitive(D3DPT_TRIANGLESTRIP, 32, 2);
	//g_pD3dDev->DrawPrimitive(D3DPT_TRIANGLESTRIP, 40, 2);
	//g_pD3dDev->DrawPrimitive(D3DPT_TRIANGLESTRIP, 48, 2);
	//g_pD3dDev->DrawPrimitive(D3DPT_TRIANGLESTRIP, 56, 2);
}


HRESULT CEffectPlane::InvalidateDeviceObjects()
{
	FLOG("CEffectPlane::InvalidateDeviceObjects()");
	//SAFE_RELEASE(m_pVB); // crash here
	return S_OK;
}

HRESULT CEffectPlane::DeleteDeviceObjects()
{
	FLOG("CEffectPlane::DeleteDeviceObjects()");
	return S_OK;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CTraceAni::CTraceAni()
{
	FLOG("CTraceAni()");
	memset((void*)this, 0, sizeof(CTraceAni));
	m_fRadius = 0.0f;
}

CTraceAni::~CTraceAni()
{
	FLOG("~CTraceAni()");
}

BOOL CTraceAni::Tick(float fElapsedTime)
{
	FLOG("CTraceAni::Tick(float fElapsedTime)");
	m_fCurrentTextureTick += fElapsedTime;

	if (m_nTextureNumber > 0 && m_fCurrentTextureTick > m_fTextureAnimationTime)
	{
		// texture 애니메이션
		m_nCurrentTextureNumber++;

		m_nCurrentTextureNumber %= m_nTextureNumber;

		m_fCurrentTextureTick = 0.0f;
	}

	m_fCurrentCreateTick += fElapsedTime;

	if (m_pParent)
	{
		D3DXMATRIX mat = m_pParent->m_pParent->m_mMatrix;
		mat._41 = 0;
		mat._42 = 0;
		mat._43 = 0;
		D3DXVECTOR3 vPos;
		D3DXVec3TransformCoord(&vPos, &m_pParent->m_vPos, &mat);
		m_vCurrentPos.x = m_pParent->m_pParent->m_mMatrix._41 + vPos.x;
		m_vCurrentPos.y = m_pParent->m_pParent->m_mMatrix._42 + vPos.y;
		m_vCurrentPos.z = m_pParent->m_pParent->m_mMatrix._43 + vPos.z;
	}

	if (m_fCurrentCreateTick > m_fCreateTick)
	{
		if (m_bCreateTrace)
		{
			auto nOldIndex = m_nCurrentCreateIndex;
			//현재 생성할 VB 위치가 마지막이면 앞으로 돌아간다
			if (++m_nCurrentCreateIndex >= m_nNumberOfTrace)
			{
				m_nCurrentCreateIndex = 0;
			}
			//현재 생성하는 VB 16개 단위의 개수
			if (++m_nCurrentNumberOfTrace > m_nNumberOfTrace)// 버퍼에 꽉차게 만들어진 경우
			{
				m_nCurrentNumberOfTrace = m_nNumberOfTrace;
				m_pEffectPlane[m_nCurrentCreateIndex]->InvalidateDeviceObjects();
				m_pEffectPlane[m_nCurrentCreateIndex]->DeleteDeviceObjects();

				util::del(m_pEffectPlane[m_nCurrentCreateIndex]);

				if (++m_nCurrentDeleteIndex > m_nNumberOfTrace - 1) m_nCurrentDeleteIndex = 0;

				int nEndIndex = m_nCurrentCreateIndex + 1 > m_nNumberOfTrace - 1 ? 0 : m_nCurrentCreateIndex + 1;

				if (m_nNumberOfTrace <= 2) m_pEffectPlane[nEndIndex]->SetEndIndexOriginal();
				else
				{
					int nEndIndex2 = nEndIndex + 1 > m_nNumberOfTrace - 1 ? 0 : nEndIndex + 1;
					m_pEffectPlane[nEndIndex]->SetEndIndex();
					m_pEffectPlane[nEndIndex2]->SetEndIndex2();
				}
			}
			else
			{
				// delete index를 계산할 필요가 없다. 마지막에 생성된 것이 지울 인덱스이기 때문에.

			}

			if (m_pEffectPlane[nOldIndex]) m_pEffectPlane[nOldIndex]->SetFirstIndex(false);

			// 2016-08-28 panoskj
			if (m_nCurrentNumberOfTrace < m_nNumberOfTrace)
			{
				if (m_pEffectPlane[0]) m_pEffectPlane[0]->SetEndIndex(); 
				if (m_pEffectPlane[1]) m_pEffectPlane[1]->SetEndIndex2();
			}

			m_vOldPos = m_vCurrentPos;

			if (m_pEffectPlane[m_nCurrentCreateIndex])
			{
				m_pEffectPlane[m_nCurrentCreateIndex]->InvalidateDeviceObjects();
				m_pEffectPlane[m_nCurrentCreateIndex]->DeleteDeviceObjects();
				util::del(m_pEffectPlane[m_nCurrentCreateIndex]);
			}
			m_pEffectPlane[m_nCurrentCreateIndex] = new CEffectPlane { this, 0 };
			m_pEffectPlane[m_nCurrentCreateIndex]->InitDeviceObjects();
			m_pEffectPlane[m_nCurrentCreateIndex]->RestoreDeviceObjects();
			m_pEffectPlane[m_nCurrentCreateIndex]->SetFirstIndex(true);


			m_nCurrentRenderIndex = m_nCurrentCreateIndex;
			// CEffectPlane은 IB 포인터와 1:1로 매칭. 최초 생성한 것의 중심점만 바꾼다.
			// 인덱스 = m_nCurrentCreateIndex X 16

			// U 좌표 계산
			// VB를 앞으로 두칸씩 이동한다. memcpy를 이용해 한번에 복사
			// 마지막점 잔상 생성 : m_pVBTrace : 카메라 사이드벡타 + m_vCurrentPos
		}
		else
		{
			DBGOUT("CTraceAni::Tick(float fElapsedTime), Trace Delete[index:%d]\n", m_nCurrentDeleteIndex);
			// 잔상을 하나씩 줄여 나간다.
			//현재 생성할 VB 위치가 마지막이면 앞으로 돌아간다
			if (m_nCurrentNumberOfTrace > 0)
			{
				// int nDeleteIndex = m_nCurrentDeleteIndex; // unused variable?

				m_pEffectPlane[m_nCurrentCreateIndex]->InvalidateDeviceObjects();
				m_pEffectPlane[m_nCurrentCreateIndex]->DeleteDeviceObjects();
				util::del(m_pEffectPlane[m_nCurrentCreateIndex]);


				if (++m_nCurrentDeleteIndex > m_nNumberOfTrace) m_nCurrentDeleteIndex = 0; // 버퍼 인덱스를 넘어 처음 인덱스로 이동

				m_nCurrentNumberOfTrace--;
			}
			else
			{
				// 잔상 개수가 0이면 소멸
				return FALSE;
			}
		}
		m_fCurrentCreateTick = 0.0f;
	}
	else	// 마지막 점 이동 계산
	{
		for (int i = 0; i < m_nNumberOfTrace; i++)
		{
			if (m_pEffectPlane[i])
			{
				m_pEffectPlane[i]->Tick(fElapsedTime);
				auto dist = m_pEffectPlane[i]->GetPos() - m_pEffectPlane[0]->GetPos();
				m_fRadius = D3DXVec3Length(&dist);
			}
		}
	}

	return TRUE;
}

void CTraceAni::Render()
{
	FLOG("CTraceAni::Render()");

}

HRESULT CTraceAni::InitDeviceObjects()
{
	FLOG("CTraceAni::InitDeviceObjects()");
	m_pEffectPlane = new CEffectPlane*[m_nNumberOfTrace];
	memset(m_pEffectPlane, 0x00, sizeof(DWORD)*m_nNumberOfTrace);
	return S_OK;
}

HRESULT CTraceAni::RestoreDeviceObjects()
{
	FLOG("CTraceAni::RestoreDeviceObjects()");
	if (m_pParent)
	{
		D3DXMATRIX mat = m_pParent->m_pParent->m_mMatrix;//유닛 매트릭스
		mat._41 = 0;
		mat._42 = 0;
		mat._43 = 0;
		D3DXVECTOR3 vPos;
		D3DXVec3TransformCoord(&vPos, &m_pParent->m_vPos, &mat);// 이펙트 상대 좌표
		m_vOldPos.x = m_pParent->m_pParent->m_mMatrix._41 + vPos.x;
		m_vOldPos.y = m_pParent->m_pParent->m_mMatrix._42 + vPos.y;
		m_vOldPos.z = m_pParent->m_pParent->m_mMatrix._43 + vPos.z;
		m_vCurrentPos = m_vOldPos;

		if (m_pEffectPlane[0])
		{
			m_pEffectPlane[0]->InvalidateDeviceObjects();
			m_pEffectPlane[0]->DeleteDeviceObjects();
			util::del(m_pEffectPlane[0]);							// 2006-07-07 by ispark
		}
		m_pEffectPlane[0] = new CEffectPlane(this, 0);
		m_pEffectPlane[0]->InitDeviceObjects();
		if (m_pEffectPlane[0]->RestoreDeviceObjects() == S_OK)
		{
			m_pEffectPlane[0]->SetFirstIndex(TRUE);
			m_pEffectPlane[0]->SetEndIndex();
		}

	}

	m_nCurrentNumberOfTrace = 1;
	m_fCurrentCreateTick = 0;//한텀 쉰다음에 다음 잔상을 그린다.
	m_nCurrentTextureNumber = 0;
	m_bCreateTrace = TRUE;
	//	m_vPos1 = m_vCurrentPos;
	m_fCurrentTextureTick = 0;
	m_nCurrentCreateIndex = 0;
	m_nCurrentRenderIndex = 0;
	m_nCurrentDeleteIndex = 0;
	m_bRestored = TRUE;
	return S_OK;
}

HRESULT CTraceAni::InvalidateDeviceObjects()
{
	FLOG("CTraceAni::InvalidateDeviceObjects()");
	//	g_pD3dDev->DeleteStateBlock( m_dwStateBlock );
	for (int i = 0; i < m_nNumberOfTrace; i++)
	{
		if (m_pEffectPlane[i])
		{
			m_pEffectPlane[i]->InvalidateDeviceObjects();
			m_pEffectPlane[i]->DeleteDeviceObjects();
			util::del(m_pEffectPlane[i]);
		}
	}
	m_bRestored = FALSE;
	return S_OK;
}

HRESULT CTraceAni::DeleteDeviceObjects()
{
	FLOG("CTraceAni::DeleteDeviceObjects()");
	util::del_array(m_pEffectPlane);
	return S_OK;
}
/*
CTraceAni::CTraceAni()
{
FLOG( "CTraceAni()" );
memset((void*)this,0,sizeof(CTraceAni));
//	m_pRenderer = g_pD3dApp->m_pTraceAniRender;
}

CTraceAni::~CTraceAni()
{
FLOG( "~CTraceAni()" );

}

BOOL CTraceAni::Tick(float fElapsedTime)
{
FLOG( "CTraceAni::Tick(float fElapsedTime)" );
m_fCurrentTextureTick += fElapsedTime;
if(m_nTextureNumber > 0 && m_fCurrentTextureTick > m_fTextureAnimationTime)
{
// texture 애니메이션
m_nCurrentTextureNumber++;
if(m_nCurrentTextureNumber == m_nTextureNumber)
{
m_nCurrentTextureNumber = 0;
}
m_fCurrentTextureTick = 0.0f;
}
m_fCurrentCreateTick += fElapsedTime;
if(m_pParent)
{
D3DXMATRIX mat = m_pParent->m_pParent->m_mMatrix;
mat._41 = 0;
mat._42 = 0;
mat._43 = 0;
D3DXVECTOR3 vPos;
D3DXVec3TransformCoord( &vPos, &m_pParent->m_vPos, &mat );
m_vCurrentPos.x = m_pParent->m_pParent->m_mMatrix._41 + vPos.x;
m_vCurrentPos.y = m_pParent->m_pParent->m_mMatrix._42 + vPos.y;
m_vCurrentPos.z = m_pParent->m_pParent->m_mMatrix._43 + vPos.z;
}
if(m_fCurrentCreateTick > m_fCreateTick )
{
if(m_bCreateTrace)
{
SPRITE_VERTEX* v1, *v2;
m_pVBTrace[0]->Lock( 0, 0, (BYTE**)&v1, 0 );
m_pVBTrace[1]->Lock( 0, 0, (BYTE**)&v2, 0 );
m_nCurrentNumberOfTrace++;
if(m_nCurrentNumberOfTrace > m_nNumberOfTrace )
{
m_nCurrentNumberOfTrace = m_nNumberOfTrace;
char* pVB = new char[sizeof(SPRITE_VERTEX)*m_nCurrentNumberOfTrace*2];
memcpy( pVB, ((char*)v1)+sizeof(SPRITE_VERTEX)*2, sizeof(SPRITE_VERTEX)*m_nCurrentNumberOfTrace*2);
memcpy( v1, pVB, sizeof(SPRITE_VERTEX)*m_nCurrentNumberOfTrace*2 );
memcpy( pVB, ((char*)v2)+sizeof(SPRITE_VERTEX)*2, sizeof(SPRITE_VERTEX)*m_nCurrentNumberOfTrace*2);
memcpy( v2, pVB, sizeof(SPRITE_VERTEX)*m_nCurrentNumberOfTrace*2 );
delete pVB;
}
// U 좌표 계산
//int nNum = m_nCurrentNumberOfTrace>m_nNumberOfTrace ? m_nNumberOfTrace*2 : m_nCurrentNumberOfTrace*2;
//			m_vPos3 = m_vPos2;
//			m_vPos2 = m_vPos1;
int nNum = m_nCurrentNumberOfTrace*2;
for(int i=0;i<nNum;i++)
{
if(v1[i].p.x == 0)
{
char buf[256];
sprintf( buf, "1.index[%d],p[%f,%f,%f],tu[%f],tv[%f]\n", i, v1[i].p.x,v1[i].p.y,v1[i].p.z,v1[i].tu, v1[i].tv );
DBGOUT( buf );
}
if(nNum == 2)
{
v1[i].tu = 0.0f;
v2[i].tu = 0.0f;
}
else if(i/2 == nNum/2-1)// 마지막 생성점 바로 다음
{
if(nNum/2 == 1)
{
v1[i].tu = 0.0f;// 점이 2개밖에 없는경우(아래 생성되는것까지 4개인 경우)
v2[i].tu = 0.0f;// 점이 2개밖에 없는경우(아래 생성되는것까지 4개인 경우)
}
else
{
v1[i].tu = m_fUPosOfTail;
v2[i].tu = m_fUPosOfTail;
}
}
else
{
v1[i].tu = ((1.0f-m_fUPosOfTail) * (i/2)) / ((nNum/2)-1) ;// i/2:생성된 순서, nNum/2:전체 개수
v2[i].tu = ((1.0f-m_fUPosOfTail) * (i/2)) / ((nNum/2)-1) ;// i/2:생성된 순서, nNum/2:전체 개수
}
}
// VB를 앞으로 두칸씩 이동한다. memcpy를 이용해 한번에 복사
// 마지막점 잔상 생성 : m_pVBTrace : 카메라 사이드벡타 + m_vCurrentPos
if(m_pParent)
{
D3DXVECTOR3 vSide;
D3DXVECTOR3 vTarget(m_pParent->m_pParent->m_mMatrix._31,m_pParent->m_pParent->m_mMatrix._32,m_pParent->m_pParent->m_mMatrix._33);
D3DXVec3Cross(&vSide, &g_pD3dApp->m_pCamera->GetViewDir(), &vTarget);
D3DXVec3Normalize( &vSide, &vSide);
v1[nNum].p = m_vCurrentPos - vSide * m_fHalfSize;	v1[nNum].tu=1;	v1[nNum].tv=1;	v1[nNum].c=0xFFFFFFFF;
v1[nNum+1].p = m_vCurrentPos + vSide * m_fHalfSize;	v1[nNum+1].tu=1;	v1[nNum+1].tv=0;	v1[nNum+1].c=0xFFFFFFFF;
v2[nNum].p = m_vCurrentPos + vSide * m_fHalfSize;	v2[nNum].tu=1;	v2[nNum].tv=1;	v2[nNum].c=0xFFFFFFFF;
v2[nNum+1].p = m_vCurrentPos - vSide * m_fHalfSize;	v2[nNum+1].tu=1;	v2[nNum+1].tv=0;	v2[nNum+1].c=0xFFFFFFFF;
}
//			D3DXVECTOR3 vCamSide;
//			D3DXVec3Normalize( &vCamSide, &g_pD3dApp->m_pCamera->GetCross());
///			char buf[256];
//			v[nNum].p = m_vCurrentPos - vCamSide * m_fHalfSize;		v[nNum].tu=1;	v[nNum].tv=1;	v[nNum].c=0xFFFFFFFF;
//			if(v[nNum].p.x == 0)
//			{
//				sprintf( buf, "2.index[%d],p[%f,%f,%f],tu[%f],tv[%f]\n", nNum, v[nNum].p.x,v[nNum].p.y,v[nNum].p.z,v[nNum].tu, v[nNum].tv );
//				DBGOUT( buf );
//			}
//			v[nNum+1].p = m_vCurrentPos + vCamSide * m_fHalfSize;	v[nNum+1].tu=1;	v[nNum+1].tv=0;	v[nNum+1].c=0xFFFFFFFF;
//			if(v[nNum+1].p.x == 0)
//			{
//				sprintf( buf, "3.index[%d],p[%f,%f,%f],tu[%f],tv[%f]\n", nNum+1, v[nNum+1].p.x,v[nNum+1].p.y,v[nNum+1].p.z,v[nNum+1].tu, v[nNum+1].tv );
//				DBGOUT( buf );
//			}
m_pVBTrace[0]->Unlock();
m_pVBTrace[1]->Unlock();
}
else
{
// 잔상을 하나씩 줄여 나간다.
m_nCurrentNumberOfTrace--;
if(m_nCurrentNumberOfTrace == 0)
{
m_nCurrentNumberOfTrace = 0;
return FALSE;
}
SPRITE_VERTEX* v1,* v2;
m_pVBTrace[0]->Lock( 0, 0, (BYTE**)&v1, 0 );
m_pVBTrace[1]->Lock( 0, 0, (BYTE**)&v2, 0 );
char* pVB = new char[sizeof(SPRITE_VERTEX)*m_nCurrentNumberOfTrace*2];
memcpy( pVB, v1+sizeof(SPRITE_VERTEX)*2, sizeof(SPRITE_VERTEX)*m_nCurrentNumberOfTrace*2);
memcpy( v1, pVB, sizeof(SPRITE_VERTEX)*m_nCurrentNumberOfTrace*2 );
memcpy( pVB, v2+sizeof(SPRITE_VERTEX)*2, sizeof(SPRITE_VERTEX)*m_nCurrentNumberOfTrace*2);
memcpy( v2, pVB, sizeof(SPRITE_VERTEX)*m_nCurrentNumberOfTrace*2 );
delete pVB;
m_pVBTrace[0]->Unlock();
m_pVBTrace[1]->Unlock();
// 잔상 개수가 0이면 소멸
}
m_fCurrentCreateTick = 0.0f;
}
else
{
SPRITE_VERTEX* v1,* v2;
m_pVBTrace[0]->Lock( 0, 0, (BYTE**)&v1, 0 );
m_pVBTrace[1]->Lock( 0, 0, (BYTE**)&v2, 0 );
//int nNum = m_nCurrentNumberOfTrace>m_nNumberOfTrace ? m_nNumberOfTrace*2 : m_nCurrentNumberOfTrace*2;
int nNum = m_nCurrentNumberOfTrace*2;
if(m_pParent)
{
D3DXVECTOR3 vSide;
D3DXVECTOR3 vTarget(m_pParent->m_pParent->m_mMatrix._31,m_pParent->m_pParent->m_mMatrix._32,m_pParent->m_pParent->m_mMatrix._33);
D3DXVec3Cross(&vSide, &g_pD3dApp->m_pCamera->GetViewDir(), &vTarget);
D3DXVec3Normalize( &vSide, &vSide);
v1[nNum].p = m_vCurrentPos - vSide * m_fHalfSize;
v1[nNum+1].p = m_vCurrentPos + vSide * m_fHalfSize;
v2[nNum].p = m_vCurrentPos + vSide * m_fHalfSize;
v2[nNum+1].p = m_vCurrentPos - vSide * m_fHalfSize;
}
//		D3DXVECTOR3 vCamSide;
//		D3DXVec3Normalize( &vCamSide, &g_pD3dApp->m_pCamera->GetCross());
//		v[nNum].p = m_vCurrentPos - vCamSide * m_fHalfSize;
//		v[nNum+1].p = m_vCurrentPos + vCamSide * m_fHalfSize;
m_pVBTrace[0]->Unlock();
m_pVBTrace[1]->Unlock();
//		m_vPos1 = m_vCurrentPos;
}

return TRUE;
}

void CTraceAni::Render()
{
FLOG( "CTraceAni::Render()" );

}

HRESULT CTraceAni::InitDeviceObjects()
{
FLOG( "CTraceAni::InitDeviceObjects()" );
return S_OK;
}

HRESULT CTraceAni::RestoreDeviceObjects()
{
FLOG( "CTraceAni::RestoreDeviceObjects()" );
if( FAILED( g_pD3dDev->CreateVertexBuffer( (m_nNumberOfTrace+1)*2 * sizeof(SPRITE_VERTEX),
D3DUSAGE_WRITEONLY, D3DFVF_SPRITE_VERTEX, D3DPOOL_MANAGED, &m_pVBTrace[0] ) ) )
return E_FAIL;
if( FAILED( g_pD3dDev->CreateVertexBuffer( (m_nNumberOfTrace+1)*2 * sizeof(SPRITE_VERTEX),
D3DUSAGE_WRITEONLY, D3DFVF_SPRITE_VERTEX, D3DPOOL_MANAGED, &m_pVBTrace[1] ) ) )
return E_FAIL;

SPRITE_VERTEX* v1,* v2;
m_pVBTrace[0]->Lock( 0, 0, (BYTE**)&v1, 0 );
m_pVBTrace[1]->Lock( 0, 0, (BYTE**)&v2, 0 );
memset( v1, 0x00, (m_nNumberOfTrace+1)*2 * sizeof(SPRITE_VERTEX));
memset( v2, 0x00, (m_nNumberOfTrace+1)*2 * sizeof(SPRITE_VERTEX));
if(m_pParent)
{
D3DXMATRIX mat = m_pParent->m_pParent->m_mMatrix;
mat._41 = 0;
mat._42 = 0;
mat._43 = 0;
D3DXVECTOR3 vPos;
D3DXVec3TransformCoord( &vPos, &m_pParent->m_vPos, &mat );
m_vCurrentPos.x = m_pParent->m_pParent->m_mMatrix._41 + vPos.x;
m_vCurrentPos.y = m_pParent->m_pParent->m_mMatrix._42 + vPos.y;
m_vCurrentPos.z = m_pParent->m_pParent->m_mMatrix._43 + vPos.z;
D3DXVECTOR3 vSide;
D3DXVECTOR3 vTarget(m_pParent->m_pParent->m_mMatrix._31,m_pParent->m_pParent->m_mMatrix._32,m_pParent->m_pParent->m_mMatrix._33);
D3DXVec3Cross(&vSide, &g_pD3dApp->m_pCamera->GetViewDir(), &vTarget);
D3DXVec3Normalize( &vSide, &vSide);
v1[0].p = m_vCurrentPos - vSide * m_fHalfSize;
v1[1].p = m_vCurrentPos + vSide * m_fHalfSize;
v1[0].tu=0; v1[0].tv=1;	v1[0].c=0xFFFFFFFF;
v1[1].tu=0; v1[1].tv=0;	v1[1].c=0xFFFFFFFF;
v2[0].p = m_vCurrentPos + vSide * m_fHalfSize;
v2[1].p = m_vCurrentPos - vSide * m_fHalfSize;
v2[0].tu=0; v2[0].tv=1;	v2[0].c=0xFFFFFFFF;
v2[1].tu=0; v2[1].tv=0;	v2[1].c=0xFFFFFFFF;
}
m_pVBTrace[0]->Unlock();
m_pVBTrace[1]->Unlock();

m_nCurrentNumberOfTrace = 0;
m_fCurrentCreateTick = m_fCreateTick;//다음 Tick에서 바로 첫번째 잔상을 그린다.
m_nCurrentTextureNumber = 0;
m_bCreateTrace = TRUE;
//	m_vPos1 = m_vCurrentPos;

//	g_pD3dDev->BeginStateBlock();
//		g_pD3dDev->SetRenderState( D3DRS_ZENABLE, m_bZbufferEnable );
//		g_pD3dDev->SetRenderState( D3DRS_LIGHTING, TRUE );
//		g_pD3dDev->SetRenderState( D3DRS_ALPHABLENDENABLE, m_bAlphaBlendEnable );
//		g_pD3dDev->SetRenderState( D3DRS_SRCBLEND, m_dwSrcBlend );
//		g_pD3dDev->SetRenderState( D3DRS_DESTBLEND, m_dwDestBlend );
//	g_pD3dDev->EndStateBlock( &m_dwStateBlock );
m_bRestored = TRUE;
return S_OK;
}

HRESULT CTraceAni::InvalidateDeviceObjects()
{
FLOG( "CTraceAni::InvalidateDeviceObjects()" );
SAFE_RELEASE( m_pVBTrace[0] );
SAFE_RELEASE( m_pVBTrace[1] );
//	g_pD3dDev->DeleteStateBlock( m_dwStateBlock );
m_bRestored = FALSE;
return S_OK;
}

HRESULT CTraceAni::DeleteDeviceObjects()
{
FLOG( "CTraceAni::DeleteDeviceObjects()" );
return S_OK;
}
*/