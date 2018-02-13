// SpriteAniData.cpp: implementation of the CSpriteAniData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "effect.h"
#include "SpriteAniData.h"
#include "AtumApplication.h"
#include "Camera.h"

extern CAtumApplication				*g_pD3dApp;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSpriteAni::CSpriteAni()
{
	FLOG( "CSpriteAni()" );
	memset((void*)this, 0, sizeof(SpriteAniData));

	fDistance = 0.0f;
	dwType = EFFECT_TYPE_SPRITE;
	m_nSpriteType = 0;
	m_fCurrentTick = 0;
	m_fCurrentTime = 0;
	
//	m_bRestored = FALSE;

	m_fCurrentRotateAngle = 0;
	m_fCurrentRotateTime = 0;
	m_pParent = NULL;

}

CSpriteAni::~CSpriteAni()
{
	FLOG( "~CSpriteAni()" );
}

// by dhkwon, 030917
void CSpriteAni::SetTexture(char* strTexture)
{
	FLOG( "CSpriteAni::SetTexture(char* strTexture)" );
	strcpy(m_strTextureFile, strTexture );
}


BOOL CSpriteAni::Tick(float fElapsedTime)
{
	FLOG( "CSpriteAni::Tick(float fElapsedTime)" );

	m_fCurrentTick -= fElapsedTime;
	if(m_fCurrentTick < 0)
	{
		m_fCurrentTime += fElapsedTime;//m_fTick;
		if(m_fCurrentTime > m_fVel)
		{
			m_nSpriteType ++;
			int nSpriteNumber =0;
			switch(m_nTextureVertexBufferType)
			{
			case 0:
				nSpriteNumber = 4;
				break;
			case 1:
				nSpriteNumber = 8;
				break;
			case 2:
				nSpriteNumber = 16;
				break;
			case 3:
				nSpriteNumber = 2;
				break;
			case 4:
				nSpriteNumber = 1;
				break;
			}
			if(m_nSpriteType>=nSpriteNumber)
			{
				m_nSpriteType = nSpriteNumber-1;
			}
			m_fCurrentTime = 0;
		}
		// color
		m_fColorChangeStartTime -= fElapsedTime;//m_fTick;
		if(m_fColorChangeStartTime<=0 && //((DWORD)m_cColor) != 0 && 
			(m_cColorStep.r!=0 || m_cColorStep.g!=0 || m_cColorStep.b!=0 || m_cColorStep.a!=0))
		{
			m_cColor += m_cColorStep * VEL_COLOR_CHANGE;
			if(m_cColor.r < 0 ) m_cColor.r = 0;
			if(m_cColor.g < 0 ) m_cColor.g = 0;
			if(m_cColor.b < 0 ) m_cColor.b = 0;
			if(m_cColor.a < 0 ) m_cColor.a = 0;
			if(m_cColor.r > 1 ) m_cColor.r = 1;
			if(m_cColor.g > 1 ) m_cColor.g = 1;
			if(m_cColor.b > 1 ) m_cColor.b = 1;
			if(m_cColor.a > 1 ) m_cColor.a = 1;
			if((DWORD)m_cColor == 0 )
			{
				if(m_nColorLoop>0)
				{
					m_cColorStep *= -1;
//					m_cColor += m_cColorStep * VEL_COLOR_CHANGE;
				}
				else if(m_nColorLoop==0)
				{
					return FALSE;
				}
				else if(m_nColorLoop==-1) // 무한 반복
				{
					m_cColorStep *= -1;
//					m_cColor += m_cColorStep * VEL_COLOR_CHANGE;
				}
			}
			else if( (DWORD)m_cColor == 0xFFFFFFFF )
			{
				if(m_nColorLoop != 0)
				{
					m_cColorStep *= -1;
//					m_cColor += m_cColorStep * VEL_COLOR_CHANGE;
				}
				if(m_nColorLoop > 0)
				{
					m_nColorLoop--;
				}
			}
		}

		if(m_pParent && m_pParent->m_fBillboardRotateAngle > 0
			&& m_pParent->m_fBillboardRotatePerSec > 0)
		{
			m_fCurrentRotateTime += fElapsedTime;//m_fTick;
			if(m_fCurrentRotateTime > m_pParent->m_fBillboardRotatePerSec)
			{
				m_fCurrentRotateAngle += m_pParent->m_fBillboardRotateAngle;
				m_fCurrentRotateTime = 0;
			}

		}
		m_fCurrentTick = 0;//m_fTick;
	}
//	if(m_bZbufferEnable)
//	{
//		D3DXVECTOR3 vPos = m_pParent->m_vPos;//D3DXVECTOR3(m_pParent->m_vPos.x,	m_pParent->m_vPos.y,m_pParent->m_vPos.z);
//		D3DXVec3TransformCoord( &vPos, &vPos, &m_pParent->m_pParent->m_mMatrix );
//		fDistance = g_pCamera->GetDistanceToViewPlane(vPos);
//	}
	return TRUE;
}

void CSpriteAni::Render()
{
	FLOG( "CSpriteAni::Render()" );
}

HRESULT CSpriteAni::InitDeviceObjects()
{
	FLOG( "CSpriteAni::InitDeviceObjects()" );
	fDistance = 0.0f;
	m_nSpriteType = 0;
	m_fCurrentTick = 0;
	m_fCurrentTime = 0;
	
//	m_bRestored = FALSE;

	m_fCurrentRotateAngle = 0;
	m_fCurrentRotateTime = 0;
	if(m_pParent && (m_pParent->m_fRandomUpLargeAngle != 0 || m_pParent->m_fRandomUpSmallAngle != 0))
	{
		m_fCurrentRotateAngle = Random(m_pParent->m_fRandomUpLargeAngle, m_pParent->m_fRandomUpSmallAngle);
	}
	return S_OK;
}

HRESULT CSpriteAni::RestoreDeviceObjects()
{
	FLOG( "CSpriteAni::RestoreDeviceObjects()" );
//	m_bRestored = TRUE;
	return S_OK;
}

HRESULT CSpriteAni::InvalidateDeviceObjects()
{
	FLOG( "CSpriteAni::InvalidateDeviceObjects()" );
	return S_OK;
}

HRESULT CSpriteAni::DeleteDeviceObjects()
{
	FLOG( "CSpriteAni::DeleteDeviceObjects()" );
//	m_bRestored = FALSE;
	return S_OK;
}
