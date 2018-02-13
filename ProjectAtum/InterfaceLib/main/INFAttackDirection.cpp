// INFAttackDirection.cpp: implementation of the CINFAttackDirection class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "RangeTime.h"
#include "INFAttackDirection.h"
#include "AtumApplication.h"
#include "INFImage.h"
#include "GameDataLast.h"
#include "dxutil.h"
#include "INFImageEx.h"	                     // 2011. 10. 10 by jskim UI시스템 변경

typedef DWORD InfAttackDirection_t;

#define INF_ATTACK_DIRECTION_MAXBIT	32


// Inf(Interface) Attack Directon Shift Amount
#define INF_ATTACK_DIRECTION0		0
#define INF_ATTACK_DIRECTION1		1
#define INF_ATTACK_DIRECTION2		2
#define INF_ATTACK_DIRECTION3		3
#define INF_ATTACK_DIRECTION4		4
#define INF_ATTACK_DIRECTION5		5
#define INF_ATTACK_DIRECTION6		6
#define INF_ATTACK_DIRECTION7		7


#define IMAGE_POSITION_RADIUS		250

#define IMAGE_SIZE_X				193	
#define IMAGE_SIZE_Y				115	

#define IMAGE_SCALE					((float)g_pD3dApp->GetBackBufferDesc().Width/1024.0f)

#define IMAGE_CENTER_X				(g_pD3dApp->GetBackBufferDesc().Width/2)
#define IMAGE_CENTER_Y				(g_pD3dApp->GetBackBufferDesc().Height/2)

#define IMAGE_START_X				((g_pD3dApp->GetBackBufferDesc().Width/2) - (IMAGE_SIZE_X/2)*IMAGE_SCALE)
#define IMAGE_START_Y				((g_pD3dApp->GetBackBufferDesc().Height/2) - (IMAGE_SIZE_Y/2)*IMAGE_SCALE)+45


#define IMAGE_VIEWING_TIME			1
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CINFAttackDirection::CINFAttackDirection()
{

}

CINFAttackDirection::CINFAttackDirection(CAtumNode* pParent)
{
	m_pParent = pParent;
	m_bRestored = FALSE;
}

CINFAttackDirection::~CINFAttackDirection()
{
	for( int i=0; i<INF_AD_DIRECTION_MAX; i++)
	{
		util::del(m_pImgAttackDirection[i]);
	}
}

HRESULT CINFAttackDirection::InitDeviceObjects()
{
	ASSERT_ASSERT(m_pGameData);
	DataHeader *pDataHeader = NULL;
	char buf[64];
	wsprintf(buf, "UA");
	for(int i=0;i<INF_AD_DIRECTION_MAX;i++)
	{
		pDataHeader = m_pGameData->Find(buf);
		m_pImgAttackDirection[i] = new CINFImageEx;
		m_pImgAttackDirection[i]->InitDeviceObjects(pDataHeader );

		tRemainTime[i].Set(0);
	}

	return S_OK;
}

HRESULT CINFAttackDirection::RestoreDeviceObjects()
{
	float fAngle;
	fAngle = PI/4;

	
	float fRotX   = (IMAGE_SIZE_X/2)*IMAGE_SCALE;
	float fRadius = IMAGE_POSITION_RADIUS*IMAGE_SCALE;

	float fStartX = IMAGE_START_X;
	float fStartY = (IMAGE_START_Y-fRadius);
	
  	if(!m_bRestored)
	{
		for(int i=0;i<INF_AD_DIRECTION_MAX;i++)
		{

			m_pImgAttackDirection[i]->RestoreDeviceObjects();

			m_pImgAttackDirection[i]->Move(fStartX, fStartY);
			m_pImgAttackDirection[i]->Rotate(fRotX,fRadius, -fAngle*i );
			m_pImgAttackDirection[i]->SetScale(IMAGE_SCALE,IMAGE_SCALE);
		}
		
		m_bRestored = TRUE;
	}
	
	return S_OK;
}

HRESULT CINFAttackDirection::DeleteDeviceObjects()
{
	for(int i=0;i<INF_AD_DIRECTION_MAX;i++)
	{
		m_pImgAttackDirection[i]->DeleteDeviceObjects();
		util::del(m_pImgAttackDirection[i]);
	}
	
	return S_OK;
}

HRESULT CINFAttackDirection::InvalidateDeviceObjects()
{
	if(m_bRestored)
	{
		for(int i=0;i<INF_AD_DIRECTION_MAX;i++)
		{
			m_pImgAttackDirection[i]->InvalidateDeviceObjects();
		}
		m_bRestored = FALSE;
	}
	return S_OK;
}


void CINFAttackDirection::Tick()
{
/*	
	if( g_pD3dApp->m_pKeyBoard->GetAsyncKeyState(DIK_1) )
	{
		OnAttackDirection( INF_ATTACK_DIRECTION0 );
	}

	if( g_pD3dApp->m_pKeyBoard->GetAsyncKeyState(DIK_2) )
	{
		OnAttackDirection( INF_ATTACK_DIRECTION1 );
	}

	if( g_pD3dApp->m_pKeyBoard->GetAsyncKeyState(DIK_3) )
	{
		OnAttackDirection( INF_ATTACK_DIRECTION2 );
	}

	if( g_pD3dApp->m_pKeyBoard->GetAsyncKeyState(DIK_4) )
	{
		OnAttackDirection( INF_ATTACK_DIRECTION3 );
	}

	if( g_pD3dApp->m_pKeyBoard->GetAsyncKeyState(DIK_5) )
	{
		OnAttackDirection( INF_ATTACK_DIRECTION4 );
	}

	if( g_pD3dApp->m_pKeyBoard->GetAsyncKeyState(DIK_6) )
	{
		OnAttackDirection( INF_ATTACK_DIRECTION5 );
	}

	if( g_pD3dApp->m_pKeyBoard->GetAsyncKeyState(DIK_7) )
	{
		OnAttackDirection( INF_ATTACK_DIRECTION6 );
	}

	if( g_pD3dApp->m_pKeyBoard->GetAsyncKeyState(DIK_8) )
	{
		OnAttackDirection( INF_ATTACK_DIRECTION7 );
	}
*/
	float fElapsedTime = g_pD3dApp->GetElapsedTime();

	for(int i=0;i<INF_AD_DIRECTION_MAX;i++)
	{
		tRemainTime[i].Tick( fElapsedTime );
	}
}

void CINFAttackDirection::Render()
{
	DWORD dwColor = 0x00FFFFFF;
	for(int i=0;i<INF_AD_DIRECTION_MAX;i++)
	{
		if( tRemainTime[i].Get() > 0.0f )
		{
			DWORD alpha = 255* tRemainTime[i].GetRemainRate();
			alpha = alpha << 24;
			m_pImgAttackDirection[i]->SetColor( alpha | dwColor );
			m_pImgAttackDirection[i]->Render();
		}
		else
			OffAttackDirection( i );
	}
}

//! m_dwAttackDirectionBits를 입력갑에 의해 on 시킴
void CINFAttackDirection::OnAttackDirection( DWORD dwShiftNum )
{
	DWORD dwOnBit = 1;
	dwOnBit = dwOnBit << dwShiftNum;
	m_dwAttackDirectionBits |= dwOnBit;

	// set rendering time
	tRemainTime[dwShiftNum].Set( IMAGE_VIEWING_TIME );
}


//! m_dwAttackDirectionBits를 입력갑에 의해 off 시킴
void CINFAttackDirection::OffAttackDirection( DWORD dwShiftNum )
{
	DWORD dwOnBit = 1;
	dwOnBit = dwOnBit << dwShiftNum;
	m_dwAttackDirectionBits &= ~dwOnBit;

	// set rendering time
	tRemainTime[dwShiftNum].Set( 0 );
}


//! 입력된 dwDirection 가 셑팅되어 있는지 검사
BOOL CINFAttackDirection::IsSeted( DWORD dwDirection )
{
	DWORD dwOnBit = 1;
	dwOnBit = dwOnBit << dwDirection;

	return (m_dwAttackDirectionBits & dwOnBit) ? TRUE : FALSE;
}