// ObjectAniData.cpp: implementation of the CObjectAniData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "effect.h"
#include "ObjectAniData.h"
#include "AtumApplication.h"
#include "EffectRender.h"
#include "Camera.h"
#include "SkinnedMesh.h"

extern CAtumApplication				*g_pD3dApp;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CObjectAni::CObjectAni()
{
	FLOG( "CObjectAni()" );
	memset((void*)this,0,sizeof(CObjectAni));
	m_pParent = NULL;
	m_fRadius = 0.0f;
	// 2009. 11. 23 by jskim 리소스 로딩 구조 변경
	m_LoadingPriority = _NOTHING_PRIORITY;
	//end 2009. 11. 23 by jskim 리소스 로딩 구조 변경
}

CObjectAni::~CObjectAni()
{
	FLOG( "~CObjectAni()" );

}

// by dhkwon, 030917
void CObjectAni::SetTexture(char* strTexture)
{
	FLOG( "CObjectAni::SetTexture(char* strTexture)" );
	if(strTexture)
		strcpy(m_strTextureFile, strTexture);
	else
		m_strTextureFile[0] = NULL;
}

BOOL CObjectAni::Tick(float fElapsedTime)
{
	FLOG( "CObjectAni::Tick(float fElapsedTime)" );

	m_fCurrentTick -= fElapsedTime;
	map<string,CSkinnedMesh*>::iterator it = g_pD3dApp->m_pEffectRender->m_mapObjNameToMesh.find(m_strObjectFile);
	if(m_fCurrentTick < 0)
	{
		// texture animation이 있는 경우 Mesh에서 Texture number를 가져온다.
		if( m_nObjectAniType > 0 &&
		// 2010. 07. 23 by dhkwon, jskim 지연 로딩시 m_nTotalTextureNumber 셋팅 안되는 버그 수정
			//m_nTotalTextureNumber < 0)
			m_nTotalTextureNumber <= 0)
		//end 2010. 07. 23 by dhkwon, jskim 지연 로딩시 m_nTotalTextureNumber 셋팅 안되는 버그 수정
		{
			if(it != g_pD3dApp->m_pEffectRender->m_mapObjNameToMesh.end() )
			{
				m_nTotalTextureNumber = it->second->m_bTotalTextureNum;
			}
		}
		switch(m_nObjectAniType)
		{
		case 0:// Object animation
			{
				m_fCurrentObjectAniTime += fElapsedTime*m_fObjectAniVel;//m_fTick*m_fObjectAniVel;
				 if(m_fStartTime != 0 || m_fEndTime != 0)
				 {
					 if(m_fCurrentObjectAniTime < m_fStartTime)
					 {
						 m_fCurrentObjectAniTime = m_fStartTime;
					 } else if( m_fCurrentObjectAniTime > m_fEndTime)
					 {
						 if(m_bObjectAnimationLoop)
						 {
							 m_fCurrentObjectAniTime = m_fEndTime;
						 } 
						 else
						 {
							m_fCurrentObjectAniTime = m_fStartTime;
						 }
					 }
				 }
			}
			break;
		case 1:// Texture animation
			{
				m_fCurrentTextureAniTime += fElapsedTime;//m_fTick;
				if(m_fCurrentTextureAniTime>m_fTextureAniVel)
				{
					m_nCurrentTextureType++;
					if(m_nCurrentTextureType>=m_nTotalTextureNumber)
					{
						if(m_bAnimationLoop)
							m_nCurrentTextureType = 0;
						else
							m_nCurrentTextureType = m_nTotalTextureNumber-1;
					}
					m_fCurrentTextureAniTime = 0;
				}
			}
			break;
		case 2:// Object-Texture animation
			{
				m_fCurrentObjectAniTime += fElapsedTime*m_fObjectAniVel;//m_fTick*m_fObjectAniVel;
				 if(m_fStartTime != 0 || m_fEndTime != 0)
				 {
					 if(m_fCurrentObjectAniTime < m_fStartTime)
					 {
						 m_fCurrentObjectAniTime = m_fStartTime;
					 } else if( m_fCurrentObjectAniTime > m_fEndTime)
					 {
						 if(m_bObjectAnimationLoop)
						 {
							 m_fCurrentObjectAniTime = m_fEndTime;
						 } 
						 else
						 {
							m_fCurrentObjectAniTime = m_fStartTime;
						 }
					 }
				 }
				m_fCurrentTextureAniTime += fElapsedTime;//m_fTick;
				if(m_fCurrentTextureAniTime>m_fTextureAniVel)
				{
					m_nCurrentTextureType++;
					if(m_nCurrentTextureType>=m_nTotalTextureNumber)
					{
						if(m_bAnimationLoop)
							m_nCurrentTextureType = 0;
						else
							m_nCurrentTextureType = m_nTotalTextureNumber-1;
					}
					m_fCurrentTextureAniTime = 0;
				}
			}
			break;
		}
		// 2006-12-14 by ispark, 텍스처 오버하지 않기
		if(0 >= m_nCurrentTextureType)
		{
			m_nCurrentTextureType = 0;
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
		if(m_pParent && m_pParent->m_bUseBillboard && m_pParent->m_fBillboardRotatePerSec>0)
		{
			m_fCurrentBillboardRotateTime += fElapsedTime;//m_fTick;
			if(m_fCurrentBillboardRotateTime > m_pParent->m_fBillboardRotatePerSec)
			{
				m_fCurrentBillboardRotateAngle += m_pParent->m_fBillboardRotateAngle;
				m_fCurrentBillboardRotateTime = 0;
			}
		}
		m_fCurrentTick = 0;//m_fTick;
	}

	if(it != g_pD3dApp->m_pEffectRender->m_mapObjNameToMesh.end() )
	{
		m_fRadius = it->second->m_fRadius;
	}

//	if(m_bZbufferEnable)
//	{
//		D3DXVECTOR3 vPos = m_pParent->m_vPos;//D3DXVECTOR3(m_pParent->m_vPos.x,	m_pParent->m_vPos.y,m_pParent->m_vPos.z);
//		D3DXVec3TransformCoord( &vPos, &vPos, &m_pParent->m_pParent->m_mMatrix );
//		fDistance = g_pCamera->GetDistanceToViewPlane(vPos);
//	}

	return TRUE;
}

void CObjectAni::Render()
{
	FLOG( "CObjectAni::Render()" );
}

void CObjectAni::SetObjectAniStartTime(float fStartObjectAniTime)
{
	FLOG( "CObjectAni::SetObjectAniStartTime(float fStartObjectAniTime)" );
	float fEndTimeInterval = m_fEndTime - m_fStartTime;
	m_fCurrentObjectAniTime -= m_fStartTime;
	m_fStartTime = fStartObjectAniTime;
	m_fCurrentObjectAniTime += m_fStartTime;
	m_fEndTime = m_fStartTime + fEndTimeInterval;
}

HRESULT CObjectAni::InitDeviceObjects()
{
	FLOG( "CObjectAni::InitDeviceObjects()" );
	fDistance = 0;
	m_nCurrentTextureType = 0;
	m_nTotalTextureNumber = -1;
	m_fCurrentTick = 0;//m_fTick;
	m_fCurrentTextureAniTime = 0;
	m_fCurrentObjectAniTime = m_fStartTime;
	m_fCurrentBillboardRotateAngle = 0;
	m_fCurrentBillboardRotateTime = 0;
	if(m_pParent && //!m_pParent->m_bUseBillboard && 
		(m_pParent->m_fRandomUpLargeAngle != 0 || m_pParent->m_fRandomUpSmallAngle != 0))
	{
		m_fCurrentRandomUpAngleX = Random(m_pParent->m_fRandomUpLargeAngle, m_pParent->m_fRandomUpSmallAngle);
		m_fCurrentRandomUpAngleZ = Random(m_pParent->m_fRandomUpLargeAngle, m_pParent->m_fRandomUpSmallAngle);
	}
	return S_OK;
}

HRESULT CObjectAni::RestoreDeviceObjects()
{
	FLOG( "CObjectAni::RestoreDeviceObjects()" );
//	int index = -1;
//	if(strlen(m_strObjectFile)>0)
//	{
//		index = g_pD3dApp->m_pEffectRender->LoadObject(m_strObjectFile);
//	}
//	if(index>=0)
//	{
//		m_nTotalTextureNumber = g_pD3dApp->m_pEffectRender->m_pObjEffectMesh[index]->m_bTotalTextureNum;
//	}
	CSkinnedMesh* pMesh = NULL;
	if(strlen(m_strObjectFile)>0)
	{
		// 2009. 11. 23 by jskim 리소스 로딩 구조 변경
		pMesh = g_pD3dApp->m_pEffectRender->LoadObject(m_strObjectFile, m_LoadingPriority);			
		//end 2009. 11. 23 by jskim 리소스 로딩 구조 변경
	}
	if(pMesh)
	{
		m_nTotalTextureNumber = pMesh->m_bTotalTextureNum;
	}
	m_bRestored = TRUE;
	return S_OK;
}

HRESULT CObjectAni::InvalidateDeviceObjects()
{
	FLOG( "CObjectAni::InvalidateDeviceObjects()" );
	return S_OK;
}

HRESULT CObjectAni::DeleteDeviceObjects()
{
	FLOG( "CObjectAni::DeleteDeviceObjects()" );
	m_bRestored = FALSE;
	return S_OK;
}
