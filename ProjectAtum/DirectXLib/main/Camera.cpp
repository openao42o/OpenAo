// Camera.cpp: implementation of the CCamera class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Camera.h"
#include <stdio.h>
#include "AtumApplication.h"
#include "INFGameMain.h"
#include "ShuttleChild.h"
#include "CharacterChild.h"				// 2005-07-21 by ispark
#include "Background.h"
// 2008-03-19 by bhsohn Ep3옵션창
//#include "INFSystem.h"
#include "INFOptionSystem.h"

#include "SceneData.h"
//#include "ObjectRender.h"
#include "ObjRender.h"
#include "TutorialSystem.h"
#include "EnemyData.h"

#define GROUND_DEFAULT_RENDER_DISTANCE	300.0f

#define GROUND_RENDER_NOT		0
#define GROUND_RENDER_NORMAL	1
#define GROUND_RENDER_TILE		2

// 도시 카메라 세팅
#define MAP_TYPE_CITY_CAM_POS		(MAP_TYPE_CITY_UNIT_POS+D3DXVECTOR3(75,23,0))	
#define MAP_TYPE_CITY_CAM_POS_X		80		
#define MAP_TYPE_CITY_CAM_POS_Z		93
#define MAP_TYPE_CITY_CAM_POS_Y		55
#define MAP_TYPE_CITY_CAM_POS_MIN_Y	14
#define MAP_TYPE_CITY_CAM_MAX_LEN	D3DXVec3Length(&(MAP_TYPE_CITY_UNIT_POS - D3DXVECTOR3(2455,19+55,2480)))
#define CITY_CAMERA_FOV				(D3DX_PI*(60.0f/180.0f))
#define MIN_GAP_TO_GROUND			10.0f

#define SMOOTH_CAMERA_MAX_TIME		3.0f;

// 2005-08-18 by ispark
#define ROTATION_ANGLE				0.01f
#define ANGLE_X_UP					-1.5 + m_fAngleError	// 라디안 값
#define ANGLE_X_DOWN				0.9 + m_fAngleError

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCamera::CCamera()
		:m_vCamCurrentPos(0,150,0),m_vHeight(0,1,0),m_vDistance(0,0,0)
{
	FLOG( "CCamera()" );
	g_pCamera = this;
	m_fDistance = 0.0f; 
	m_fHeight = 30.0f;
	m_fOldDistance = 0.0f;
	m_fOldHeight = 30.0f;
	m_fOriDistance = 0.0f;
	m_fOriHeight = 30.0f;
	m_bTurning = FALSE;
	m_vObjOldPos = D3DXVECTOR3(0,0,0);
	m_vCollCamPos = D3DXVECTOR3(0,0,0);
	m_fCheckCollTime = 0.0f;
	m_bCamCollType = 0;

	m_fSideAngle = 0.0f;
	m_fOldSideAngle = 0.0f;
	m_bCamType = CAMERA_TYPE_NORMAL;
	m_bIsCamControl = FALSE;
	m_fCamControlCheckTime = 0.0f;
	m_fCheckSpeedRate = 0.0f;

	m_fDetailRateGround = 0.0f;
	m_fRenderDistance = GROUND_DEFAULT_RENDER_DISTANCE;
	m_fFrameLerp = 0.0f;
	m_bSmoothMove = FALSE;
	m_fSmoothTime = 0.0f;


	// 2005-07-20 by ispark
//	m_vChaVel = D3DXVECTOR3(0, 0, 1.0f);

	// 2005-08-16 by ispark
	m_fAngleX = 0.0f;
	m_fAngleY = 0.0f;

	m_fTotalAngleX = 0.0f;				// 2005-08-18 by ispark
	m_fAngleError = 0.0f;
	m_fCollDistance = 0.0f;				// 2005-08-19 by ispark			
}

CCamera::~CCamera()
{
	FLOG( "~CCamera()" );
	g_pCamera = NULL;

}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CCamera::Init()
/// \brief		카메라의 위치를 기어의 뒤에 셋팅한다.
/// \author		dgwoo
/// \date		2007-08-07 ~ 2007-08-07
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CCamera::Init()
{
	this->Init(g_pShuttleChild->m_vPos,g_pShuttleChild->m_vVel, 3.14f/13.0f, 30.0f);
}
void CCamera::Init( D3DXVECTOR3 vPos, 
				   D3DXVECTOR3 vVel , 
				   FLOAT angle, 
				   FLOAT distance )
{
	FLOG( "Init( D3DXVECTOR3 vPos, D3DXVECTOR3 vVel , FLOAT angle, FLOAT distance )" );
	D3DXVECTOR3 vUp(0,1,0);

	m_fDistance = distance;
	m_fOriDistance = distance;
	m_fHeight = tan(angle)*m_fDistance;
	m_fOriHeight = m_fHeight;
	m_fOldDistance = m_fDistance;
	m_fOldHeight = m_fHeight;
	m_fSideAngle = 0.0f;
	m_fOldSideAngle = 0.0f;
	m_bIsCamControl = FALSE;

	m_vCamNextPos = vPos - vVel*m_fDistance + vUp*m_fHeight;
	m_vNextTargetPos = vPos + vVel*10000.0f;

	m_vCamCurrentPos = m_vCamNextPos;
	m_vCurrentTargetPos = m_vNextTargetPos;
	SetViewParams( m_vCamCurrentPos, m_vCurrentTargetPos, vUp );
}

void CCamera::Init( D3DXVECTOR3 vStart, 
				   D3DXVECTOR3 vEnd , 
				   D3DXVECTOR3 vTargetPos, 
				   FLOAT angle, 
				   FLOAT distance )
{
	FLOG( "CCamera::Init( D3DXVECTOR3 vStart, D3DXVECTOR3 vEnd , D3DXVECTOR3 vTargetPos, FLOAT angle, FLOAT distance )" );
	D3DXVECTOR3 vUp(0,1,0);
	SetDirection( vStart, vEnd, vTargetPos );
	SetCamPos( angle, distance );
	m_vCamCurrentPos = m_vCamNextPos;
	m_vCurrentTargetPos = m_vNextTargetPos;
	SetViewParams( m_vCamCurrentPos, m_vCurrentTargetPos, vUp );
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			Init(D3DXVECTOR3 vPos, D3DXVECTOR3 vVel)
/// \brief		캐릭터 전용 초기화 : m_fHeight와 m_fDestance가 미리 입력 되어 있어야 한다.
/// \author		ispark
/// \date		2005-08-10 ~ 2005-08-10
/// \warning	
///
/// \param		
/// \return		void
///////////////////////////////////////////////////////////////////////////////
void CCamera::Init(D3DXVECTOR3 vPos, 
				   D3DXVECTOR3 vVel)
{
	FLOG( "Init( D3DXVECTOR3 vPos, D3DXVECTOR3 vVel )" );
	D3DXVECTOR3 vUp(0,1,0);

	m_fOriDistance = m_fDistance;
	m_fOriHeight = m_fHeight;
	m_fOldDistance = m_fDistance;
	m_fOldHeight = m_fHeight;
	m_fSideAngle = 0.0f;
	m_fOldSideAngle = 0.0f;
	m_fTotalAngleX = 0.0f;
	m_bIsCamControl = FALSE;

	vVel = -vVel;
	vVel.y = 0.1f;
	D3DXVec3Normalize(&vVel, &vVel);
	
	m_vCamNextPos = vPos + vVel * m_fDistance;	
	m_vNextTargetPos = vPos;

	D3DXVec3Normalize(&m_vDistance, &(m_vCamNextPos - m_vNextTargetPos));
	m_vCamNextPos = vPos + m_vDistance * m_fDistance;	
	m_vCamCurrentPos = m_vCamNextPos;
	m_vCurrentTargetPos = m_vNextTargetPos;

	// 각도 오차 계산
	D3DXVECTOR3 vDisPlan = m_vDistance;
	vDisPlan.y = 0.0f;
	D3DXVec3Normalize(&vDisPlan, &vDisPlan);
	m_fAngleError = D3DXVec3Dot(&vDisPlan, &m_vDistance);
	m_fAngleError = ACOS(m_fAngleError);

	// 충돌 처리
	float fCollDis = CheckCollMapObject(m_vNextTargetPos, m_vCamCurrentPos);
	if(fCollDis != 0)
	{
		D3DXVECTOR3 vNewCam = m_vDistance;
		vNewCam *= fCollDis;
		m_vCamCurrentPos = m_vNextTargetPos + vNewCam;
	}
	
	SetViewParams( m_vCamCurrentPos, m_vCurrentTargetPos, vUp );
}

void CCamera::SetDirection( D3DXVECTOR3 vStart, 
						   D3DXVECTOR3 vEnd , 
						   D3DXVECTOR3 vTargetPos )
{
	FLOG( "CCamera::SetDirection( D3DXVECTOR3 vStart, D3DXVECTOR3 vEnd , D3DXVECTOR3 vTargetPos)" );
	m_vTargetStart = vStart;
	m_vTargetEnd = vEnd;
	m_vNextTargetPos = vTargetPos;
	m_vNextCamVel = vEnd - vStart;
	D3DXVec3Normalize( &m_vNextCamVel, &m_vNextCamVel );
	m_vDistance = -m_fDistance * m_vNextCamVel;
	m_vCamNextPos = m_vNextTargetPos + m_vDistance + m_vHeight;
	m_bTurning = FALSE;
}

// SetDirection함수 후에 호출해야 한다.
void CCamera::SetCamPos( FLOAT angle, FLOAT distance )
{
	FLOG( "CCamera::SetCamPos( FLOAT angle, FLOAT distance )" );
	m_fAngle = angle;
	m_fDistance = distance;
	m_fOriDistance = distance;
	m_vDistance = -distance * m_vNextCamVel;
	m_vHeight = D3DXVECTOR3(0.0f,1.0f,0.0f)*tanf(m_fAngle) * distance;
	m_vCamNextPos = m_vNextTargetPos + m_vDistance + m_vHeight;
}

void CCamera::ChangeCamType()
{
	FLOG( "CCamera::ChangeCamType()" );

	if(m_bCamType == CAMERA_TYPE_FPS)
	{
		m_bCamType = CAMERA_TYPE_NORMAL;
	}
	else if(m_bCamType == CAMERA_TYPE_NORMAL)
	{
		m_bCamType = CAMERA_TYPE_FPS;
	}

	g_pSOption->sEyePt = m_bCamType;
	// 2008-03-19 by bhsohn Ep3옵션창
//	if(m_bCamType)
//	  strcpy(g_pGameMain->m_pSystem->m_strSelectModeComboData[0], STRMSG_C_INTERFACE_0002);//"1인칭"
//	else
//	  strcpy(g_pGameMain->m_pSystem->m_strSelectModeComboData[0],  STRMSG_C_INTERFACE_0003);//"3인칭"
//	if(m_bCamType)
//	{
//		g_pGameMain->m_pSystem->SetComboData(0, STRMSG_C_INTERFACE_0002);		//"1인칭"
//	}
//	else
//	{
//		g_pGameMain->m_pSystem->SetComboData(0, STRMSG_C_INTERFACE_0003);		//"3인칭"		
//	}
	// end 2008-03-19 by bhsohn Ep3옵션창
}
void CCamera::ObserveTick()
{
	FLOG( "CCamera::Tick()" );
	float fElapsedTime = g_pD3dApp->GetElapsedTime();
	static float fCheckRate = 0.0f;
	static int nOldBooster = 0;
	D3DXVECTOR3 vVel, vUp, vSide;
	D3DXMATRIX mat;
	float fTemp = 3.0f;

	vVel = g_pShuttleChild->m_stObserve.vObserveLook; //g_pD3dApp->m_pShuttleChild->m_vVel;
	vUp = g_pShuttleChild->m_stObserve.vObserveUp; //g_pD3dApp->m_pShuttleChild->m_vUp;
	// Matrix가 깨지는 현상이 발생되어, 추가_ ydkim
	if(vUp.x == 0 && vUp.y == 0 && vUp.z ==0 )
		vUp = D3DXVECTOR3(0,1,0);
	vSide = g_pShuttleChild->m_stObserve.vObserveSide;// g_pD3dApp->m_pShuttleChild->m_vSideVel;
	//vSide =  g_pD3dApp->m_pShuttleChild->m_vSideVel;
	D3DXVec3Normalize(&vVel,&vVel);


	if(!m_bIsCamControl &&    // 카메라를 컨트롤 중인가
		g_pShuttleChild->m_dwState != _LANDED)
	{
		if(m_fSideAngle > 0.011f || m_fSideAngle < -0.011f)
			m_fSideAngle *= 0.8f;
		else
			m_fSideAngle = 0.0f;
	}
	
	D3DXMatrixRotationAxis(&mat,&vUp,m_fSideAngle);
	D3DXVec3TransformCoord(&vVel,&vVel,&mat);


	if(IS_DT(g_pShuttleChild->m_myShuttleInfo.UnitKind) && // DT형이 땅에서 다니나?
		!g_pD3dApp->m_pShuttleChild->m_bIsAir)
	{
		m_vCamNextPos = g_pD3dApp->m_pShuttleChild->m_vPos - m_fDistance*vVel + m_fHeight*vUp;
		m_vNextTargetPos = g_pD3dApp->m_pShuttleChild->m_vPos + 10000.0f*vVel 
			+ (g_pD3dApp->GetBackBufferDesc().Height/(float)600)*(600 - g_pD3dApp->m_pShuttleChild->m_pt.y)*vUp;
	}
	else if(g_pD3dApp->m_pShuttleChild->m_dwState != _NORMAL)
	{
		m_vCamNextPos = g_pD3dApp->m_pShuttleChild->m_vPos - m_fDistance*vVel + m_fHeight*vUp;
		m_vNextTargetPos = g_pD3dApp->m_pShuttleChild->m_vPos + 10000.0f*vVel;
	}
	else if(IS_BT(g_pShuttleChild->m_myShuttleInfo.UnitKind) || 
		(IS_DT(g_pShuttleChild->m_myShuttleInfo.UnitKind) && 
		g_pD3dApp->m_pShuttleChild->m_bIsAir))
	{// 
//			m_fOriHeight = 15.0f;
		
		// 추가 2004.06.23 jschoi
		m_vCamNextPos = g_pD3dApp->m_pShuttleChild->m_vPos - m_fDistance*vVel + m_fHeight*vUp;
		m_vNextTargetPos = g_pD3dApp->m_pShuttleChild->m_vPos + 10000.0f*vVel;
	}
	else
	{
		// 추가 2004.06.23 jschoi
		m_vCamNextPos = g_pD3dApp->m_pShuttleChild->m_vPos - m_fDistance*vVel + m_fHeight*vUp;
		m_vNextTargetPos = g_pD3dApp->m_pShuttleChild->m_vPos + 10000.0f*vVel;
	}

	// 2005-01-27 by jschoi - 자연스런 카메라 이동 체크
	if(	g_pD3dApp->m_bChatMode || 
		g_pD3dApp->m_bFixUnitDirMode )	// 유닛 진행 방향 고정 모드
	{
		SetSmoothMove();
	}

	// 아래코드 현재 포지션을 계산하되 다음 포지션을 넘어서 지나치지 않게 셋팅한다.
	D3DXVECTOR3 vV1,vV2,vV3;
	D3DXVec3Normalize(&vV1,&(m_vCamNextPos - m_vCamCurrentPos));
	D3DXVECTOR3 vCamCurrentPos = m_vCamCurrentPos + (m_fCheckSpeedRate)*(m_vCamNextPos - m_vCamCurrentPos)*fElapsedTime;
	D3DXVec3Normalize(&vV2,&(m_vCamNextPos - vCamCurrentPos));
	vV3 = vV1 + vV2;
	if(D3DXVec3Length(&vV3) > 1.5f)
	{
		m_vCamCurrentPos = vCamCurrentPos;
	}
	else
	{
		m_vCamCurrentPos = m_vCamNextPos;
	}

	if(m_bCamType == CAMERA_TYPE_NORMAL)
	{
		if(fElapsedTime > 0.5f)
			m_vCamCurrentPos = m_vCamNextPos;

//		m_vCurrentTargetPos = m_vNextTargetPos;
		// 2004-12-21 by jschoi - 카메라 방향이 서서히 돌아가도록
		if(m_bSmoothMove)
		{
			D3DXVECTOR3 vTargetVel,vNextVel;
			D3DXVec3Normalize(&vTargetVel,&m_vCurrentTargetPos);
			D3DXVec3Normalize(&vNextVel,&m_vNextTargetPos);
			float fAngle = ACOS(D3DXVec3Dot(&vNextVel,&vTargetVel));
			if(fAngle > 4*fElapsedTime && fAngle < PI  && m_fSmoothTime>0.0f)
			{
				m_fSmoothTime -= fElapsedTime;
				float fFrameMove = fAngle*4*fElapsedTime;
				if(fFrameMove > 1.0f)
					fFrameMove = 1.0f;
				m_vCurrentTargetPos += (m_vNextTargetPos - m_vCurrentTargetPos)*fFrameMove;
			}
			else
			{
				m_vCurrentTargetPos = m_vNextTargetPos;
				m_bSmoothMove = FALSE;
			}
		}
		else
		{
			m_vCurrentTargetPos = m_vNextTargetPos;
		}
	}
	else
	{
		m_vCamCurrentPos = g_pD3dApp->m_pShuttleChild->m_vPos + 1.0f*vVel + 5.0f*vUp;
		m_vCurrentTargetPos = m_vNextTargetPos + 1.0f*vVel + 5.0f*vUp;
	}

	D3DXVECTOR3 vCamEyePos, vCamLookAtPos, vCamUp;	

	if(g_pShuttleChild && g_pShuttleChild->m_bTurnCamera)
	{
		if(GetCamType() == CAMERA_TYPE_FPS) // 1인칭
		{
			if(g_pD3dApp->m_pShuttleChild->m_bAttackMode == _SIEGE)
			{
				vCamEyePos = g_pD3dApp->m_pShuttleChild->m_vPos;// + g_pD3dApp->m_pShuttleChild->m_vWeaponPos.x*g_pD3dApp->m_pShuttleChild->m_vSideVel 
				vCamLookAtPos = vCamEyePos + 7.0f*g_pD3dApp->m_pShuttleChild->m_vWeaponVel;
				vCamUp = g_pD3dApp->m_pShuttleChild->m_vWeaponUp;
			}
			else
			{
				vCamEyePos = g_pD3dApp->m_pShuttleChild->m_vPos-D3DXVec3Length(&(g_pD3dApp->m_pShuttleChild->m_vPos-m_vCamCurrentPos))*vVel;
				vCamLookAtPos = g_pD3dApp->m_pShuttleChild->m_vPos-60.0f*vVel;
				vCamUp = vUp;
			}
		}
		else	// 기타 3인칭
		{
			vCamEyePos = g_pD3dApp->m_pShuttleChild->m_vPos+D3DXVec3Length(&(g_pD3dApp->m_pShuttleChild->m_vPos-m_vCamCurrentPos))*vVel;
			vCamLookAtPos = g_pD3dApp->m_pShuttleChild->m_vPos-20.0f*vVel;
			vCamUp = vUp;
		}
	}
	else
	{
		if(g_pD3dApp->m_pShuttleChild->m_bAttackMode == _SIEGE)
		{
			D3DXMATRIX matTemp = g_pShuttleChild->m_pMatWeaponPosition[0] * g_pShuttleChild->m_mMatrix;
			D3DXVECTOR3 vWeapon;
			vWeapon.x = matTemp._41;
			vWeapon.y = matTemp._42;
			vWeapon.z = matTemp._43;
			vCamEyePos = vWeapon + 5.0f*g_pShuttleChild->m_vWeaponVel;

			vCamLookAtPos = vCamEyePos + 7.0f*g_pShuttleChild->m_vWeaponVel;
			vCamUp = vUp;

				// 카메라 에니메이션을 위한 좌표 
				m_vCamSlowNextPos = vCamLookAtPos;
				m_fFrameLerp = 0;
		}
		else
		{
			vCamEyePos = vCamCurrentPos;
			vCamLookAtPos = m_vCurrentTargetPos;
			vCamUp = vUp;
		}
	}


	// 카메라 떨림 체크 - // 2005-03-31 by jschoi 튜토리얼 추가
	D3DXVECTOR3 vVelTemp,vCrossTemp;
	if(	g_pTutorial->IsTutorialMode() == FALSE ||
		g_pTutorial->IsUseShuttleTick() == TRUE )
	{
		if(m_fCheckCollTime > 0.25f)
			m_fCheckCollTime -= 10.0f*fElapsedTime;
		if(m_fCheckCollTime <= 0.25f)
		{
			m_fCheckCollTime = 0.25f;
			if(m_bCamCollType != 0)
			{
				m_bCamCollType = 0;
				m_bSmoothMove = FALSE;
			}
			else
			{
				m_fCheckCollTime = 10.75f;
			}
		}
		int nTemp = (int)m_fCheckCollTime;
		float fValue = m_fCheckCollTime - nTemp;
		float fCollRate = 1.0f;	// 데미지비율, 셔틀 속도 비율로 떨림 크기 설정될 변수
		if(g_pShuttleChild->m_bAttackMode == _SIEGE)
		{
			fCollRate = 0.1f;
		}
		
		D3DXVec3Normalize(&vVelTemp,&(vCamLookAtPos - vCamEyePos));
		D3DXVec3Cross(&vCrossTemp,&vCamUp,&vVelTemp);
		D3DXVec3Normalize(&vCrossTemp,&vCrossTemp);
		D3DXVec3Cross(&vCamUp,&vVelTemp,&vCrossTemp);
		if(fValue > 0.5f)
		{
			fValue = fValue - 0.5f - 0.25f;
		}
		else
		{
			fValue = -(fValue - 0.5f) - 0.25f;
		}
		switch(m_bCamCollType)
		{
		case 1:
			{// UpVec 흔들림
				D3DXMatrixRotationAxis(&mat,&vVelTemp,0.1f*fCollRate*fValue);
				D3DXVec3TransformCoord(&vCamUp,&vCamUp,&mat);
			}
			break;
		case 2:
			{// PosVec 흔들림(좌우)
				vCamEyePos += 5.0f*fCollRate*vCrossTemp*fValue;
	//			vCamLookAtPos += 10.0f*vCrossTemp*fValue;
			}
			break;
		case 3:
			{// PosVec 흔들림(상하)
				vCamEyePos += 5.0f*fCollRate*vCamUp*fValue;
	//			vCamLookAtPos += 10.0f*vCamUp*fValue;
			}
			break;
		case 4:
			{// PosVec 흔들림(좌우)
				vCamEyePos += 5.0f*fCollRate*vCrossTemp*fValue;
				vCamLookAtPos += 2.0f*fCollRate*vCrossTemp*fValue;
			}
			break;
		case 5:
			{// PosVec 흔들림(상하)
				vCamEyePos += 5.0f*fCollRate*vCamUp*fValue;
				vCamLookAtPos += 2.0f*fCollRate*vCamUp*fValue;
			}
			break;
		case 6:
			{// PosVec 흔들림(좌우상하)
				vCamEyePos += 5.0f*fCollRate*vCrossTemp*fValue;
				fValue = m_fCheckCollTime - 0.16f;
				nTemp = (int)m_fCheckCollTime;
				fValue = fValue - nTemp;
				if(fValue > 0.5f)
				{
					fValue = fValue - 0.5f - 0.25f;
				}
				else
				{
					fValue = -(fValue - 0.5f) - 0.25f;
				}
				vCamEyePos += 5.0f*fCollRate*vCamUp*fValue;
			}
			break;
		case 7:
			{// PosVec 흔들림(좌우상하,UpVec)
				vCamEyePos += 5.0f*fCollRate*vCrossTemp*fValue;
				fValue = m_fCheckCollTime - 0.16f;
				nTemp = (int)m_fCheckCollTime;
				fValue = fValue - nTemp;
				if(fValue > 0.5f)
				{
					fValue = fValue - 0.5f - 0.25f;
				}
				else
				{
					fValue = -(fValue - 0.5f) - 0.25f;
				}
				vCamEyePos += 5.0f*fCollRate*vCamUp*fValue;
				fValue = m_fCheckCollTime - 0.8f;
				nTemp = (int)m_fCheckCollTime;
				fValue = fValue - nTemp;
				if(fValue > 0.5f)
				{
					fValue = fValue - 0.5f - 0.25f;
				}
				else
				{
					fValue = -(fValue - 0.5f) - 0.25f;
				}
				D3DXMatrixRotationAxis(&mat,&vVelTemp,0.1f*fCollRate*fValue);
				D3DXVec3TransformCoord(&vCamUp,&vCamUp,&mat);
			}
			break;
		default:
			{
				if(g_pD3dApp->m_pShuttleChild->m_nBooster == BOOSTER_TYPE_MAX)
				{
					fCollRate = g_pD3dApp->m_pShuttleChild->m_fShuttleSpeed/50.0f;
					vCamEyePos += 0.5f*fCollRate*vCrossTemp*fValue;
					vCamLookAtPos += 0.5f*fCollRate*vCrossTemp*fValue;
					fValue = m_fCheckCollTime - 0.16f;
					nTemp = (int)m_fCheckCollTime;
					fValue = fValue - nTemp;
					if(fValue > 0.5f)
					{
						fValue = fValue - 0.5f - 0.25f;
					}
					else
					{
						fValue = -(fValue - 0.5f) - 0.25f;
					}
					vCamEyePos += 0.2f*fCollRate*vCamUp*fValue;
					fValue = m_fCheckCollTime - 0.8f;
					nTemp = (int)m_fCheckCollTime;
					fValue = fValue - nTemp;
					if(fValue > 0.5f)
					{
						fValue = fValue - 0.5f - 0.25f;
					}
					else
					{
						fValue = -(fValue - 0.5f) - 0.25f;
					}
					vCamLookAtPos += 0.2f*fCollRate*vCamUp*fValue;
				}
			}
		}
	}

	if(g_pD3dApp->m_pShuttleChild->m_bOrderMove)
	{
		vCamUp = g_pD3dApp->m_pShuttleChild->m_vUp;
		vCamLookAtPos = g_pD3dApp->m_pShuttleChild->m_vPos;
		vCamEyePos = GetEyePt();
		vCamEyePos += (g_pD3dApp->m_pShuttleChild->m_vPos - m_fDistance*vVel + m_fHeight*g_pD3dApp->m_pShuttleChild->m_vUp - vCamEyePos)*fElapsedTime;
		m_fCamControlCheckTime = 0.0f;
	}
	else if(m_bIsCamControl ||
		g_pShuttleChild->m_dwState == _LANDED)
	{

		POINT pt;
		GetCursorPos(&pt);
		ScreenToClient(g_pD3dApp->GetHwnd(), &pt);
		CheckMouseReverse(&pt);	

		// 2005-07-25 by ispark
		BOOL bResultMap = FALSE;
		BOOL bResultObj = FALSE;
		vCamUp = g_pShuttleChild->m_vUp;
		vCamLookAtPos = g_pShuttleChild->m_vPos;
		D3DXVec3Normalize(&vVel,&g_pShuttleChild->m_vVel);
		D3DXMatrixRotationAxis(&mat,&vCamUp,m_fSideAngle);
		D3DXVec3TransformCoord(&vVel,&vVel,&mat);

		vCamEyePos = g_pShuttleChild->m_vPos - m_fDistance*vVel + m_fHeight*g_pShuttleChild->m_vUp;

		m_fCamControlCheckTime = 0.0f;

		// 2006-05-12 by ispark, 지형 충돌
		if(CheckCollForMap(vCamEyePos))
		{
#ifdef _DEBUG
			bResultMap = FALSE;
#else
			bResultMap = TRUE;
#endif
		}
		
		if(bResultMap || bResultObj)
		{// 충돌이면
			SetViewParams(m_vOldCamEyePos, vCamLookAtPos, vCamUp);
			return ;
		}
	}
	else if(g_pD3dApp->m_pShuttleChild->m_bAttackMode != _SIEGE)
	{
		if(abs(m_fOriDistance - m_fDistance) > 0.01f)
		{
			m_fDistance += (m_fOriDistance - m_fDistance)*fElapsedTime;
		}
		else
		{
			m_fDistance = m_fOriDistance;
		}
		if(abs(m_fOriHeight - m_fHeight) > 0.01f)
		{
			m_fHeight += (m_fOriHeight - m_fHeight)*fElapsedTime;
		}
		else
		{
			m_fHeight = m_fOriHeight;
		}

		if(D3DXVec3Length(&(g_pD3dApp->m_pShuttleChild->m_vPos - vCamEyePos)) > 60.0f ) //100.0f) // 2005.6.13 by dhkwon
		{
			D3DXVec3Normalize(&vVelTemp, &(vCamEyePos - g_pD3dApp->m_pShuttleChild->m_vPos));
			vCamEyePos = g_pD3dApp->m_pShuttleChild->m_vPos + vVelTemp*60.0f; //100.0f; // 2005.6.13 by dhkwon
		}
		if(m_fDistance != m_fOriDistance || m_fHeight != m_fOriHeight)
		{
			vCamUp = g_pD3dApp->m_pShuttleChild->m_vUp;
			D3DXMatrixRotationAxis(&mat,&vCamUp,m_fSideAngle);
			D3DXVec3Normalize(&vVel,&g_pD3dApp->m_pShuttleChild->m_vVel);
			D3DXVec3TransformCoord(&vVel,&vVel,&mat);

			if(m_fCamControlCheckTime < 1.0f)
				m_fCamControlCheckTime += 0.005f*fElapsedTime;
			if(m_fCamControlCheckTime > 1.0f)
				m_fCamControlCheckTime = 1.0f;
			vCamLookAtPos = g_pD3dApp->m_pShuttleChild->m_vPos + m_fCamControlCheckTime*(vCamLookAtPos - g_pD3dApp->m_pShuttleChild->m_vPos);
		}
		else
		{
			if(m_fCamControlCheckTime < 1.0f)
				m_fCamControlCheckTime += 0.2f*fElapsedTime;
			if(m_fCamControlCheckTime > 1.0f)
				m_fCamControlCheckTime = 1.0f;
			vCamLookAtPos = g_pD3dApp->m_pShuttleChild->m_vPos + m_fCamControlCheckTime*(vCamLookAtPos - g_pD3dApp->m_pShuttleChild->m_vPos);
		}
	}

	if(m_bCamType == CAMERA_TYPE_FPS)
	{
		// ydkim 04.06.28
		if(IS_DT(g_pShuttleChild->m_myShuttleInfo.UnitKind) && // DT형이 땅에서 다니나?
			!g_pD3dApp->m_pShuttleChild->m_bIsAir)	
		{
			// DT 형이 지상에서 1인칭으로 바뀌었을때 
		}
		else
		{
			vCamEyePos = D3DXVECTOR3(g_pShuttleChild->m_mMatrix._41, g_pShuttleChild->m_mMatrix._42, g_pShuttleChild->m_mMatrix._43);
			vCamUp = D3DXVECTOR3(g_pShuttleChild->m_mMatrix._21, g_pShuttleChild->m_mMatrix._22, g_pShuttleChild->m_mMatrix._23);
		}
	}

	// 2004.06.23 jschoi
	// 이전값 저장.
	m_fOldHeight = m_fHeight;
	m_fOldDistance = m_fDistance;
	m_vOldCamEyePos = vCamEyePos;
	m_fOldSideAngle = m_fSideAngle;

	SetViewParams(vCamEyePos, vCamLookAtPos, vCamUp);

	return;
}
void CCamera::Tick()
{
	FLOG( "CCamera::Tick()" );
	float fElapsedTime = g_pD3dApp->GetElapsedTime();
	static float fCheckRate = 0.0f;
	static int nOldBooster = 0;
	D3DXVECTOR3 vVel, vUp, vSide;
	D3DXMATRIX mat;
	float fTemp = 3.0f;
	nOldBooster = g_pD3dApp->m_pShuttleChild->m_nBooster;
	if(g_pD3dApp->m_pShuttleChild->m_nBooster == BOOSTER_TYPE_MAX)
		fTemp = 4.0f;
	if(m_fCheckSpeedRate < fTemp)
		m_fCheckSpeedRate += fElapsedTime;
	if(m_fCheckSpeedRate > fTemp)
		m_fCheckSpeedRate = fTemp;

	vVel = g_pD3dApp->m_pShuttleChild->m_vVel;
	vUp = g_pD3dApp->m_pShuttleChild->m_vUp;
	// Matrix가 깨지는 현상이 발생되어, 추가_ ydkim
	if(vUp.x == 0 && vUp.y == 0 && vUp.z ==0 )
		vUp = D3DXVECTOR3(0,1,0);
	vSide = g_pD3dApp->m_pShuttleChild->m_vSideVel;
	D3DXVec3Normalize(&vVel,&vVel);

	// 2005-08-01 by ispark
//	if(g_pShuttleChild->m_dwState == _FALLING || g_pShuttleChild->m_dwState == _FALLEN)
//	{
//		vVel.y = 0.0f;
//		D3DXVec3Normalize(&vVel,&vVel);
//		vUp = D3DXVECTOR3(0,1,0);
//	}

	if(!m_bIsCamControl &&    // 카메라를 컨트롤 중인가
		g_pShuttleChild->m_dwState != _LANDED)
	{
		if(m_fSideAngle > 0.011f || m_fSideAngle < -0.011f)
			m_fSideAngle *= 0.8f;
		else
			m_fSideAngle = 0.0f;
	}
	
	D3DXMatrixRotationAxis(&mat,&vUp,m_fSideAngle);
	D3DXVec3TransformCoord(&vVel,&vVel,&mat);

//	if(g_pD3dApp->m_dwGameState == _SHOP)
//	{
//		m_fOriHeight = 10.0f;
//		m_vCamNextPos = g_pD3dApp->m_pShuttleChild->m_vPos - m_fDistance*vVel - m_fHeight*vUp;
//		m_vNextTargetPos = g_pD3dApp->m_pShuttleChild->m_vPos + 10000.0f*vVel;
//	}
//	else 

	if(IS_DT(g_pShuttleChild->m_myShuttleInfo.UnitKind) && // DT형이 땅에서 다니나?
		!g_pD3dApp->m_pShuttleChild->m_bIsAir)
	{
//		m_fOriHeight = 10.0f;
//		m_fOriDistance = 80.0f;
		m_vCamNextPos = g_pD3dApp->m_pShuttleChild->m_vPos - m_fDistance*vVel + m_fHeight*vUp;
		m_vNextTargetPos = g_pD3dApp->m_pShuttleChild->m_vPos + 10000.0f*vVel 
			+ (g_pD3dApp->GetBackBufferDesc().Height/(float)600)*(600 - g_pD3dApp->m_pShuttleChild->m_pt.y)*vUp;
	}
	else if(g_pD3dApp->m_pShuttleChild->m_dwState != _NORMAL)
	{
		m_vCamNextPos = g_pD3dApp->m_pShuttleChild->m_vPos - m_fDistance*vVel + m_fHeight*vUp;
		m_vNextTargetPos = g_pD3dApp->m_pShuttleChild->m_vPos + 10000.0f*vVel;
	}
	else if(IS_BT(g_pShuttleChild->m_myShuttleInfo.UnitKind) || 
		(IS_DT(g_pShuttleChild->m_myShuttleInfo.UnitKind) && 
		g_pD3dApp->m_pShuttleChild->m_bIsAir))
	{// 
//			m_fOriHeight = 15.0f;
		
		// 추가 2004.06.23 jschoi
		m_vCamNextPos = g_pD3dApp->m_pShuttleChild->m_vPos - m_fDistance*vVel + m_fHeight*vUp;
		m_vNextTargetPos = g_pD3dApp->m_pShuttleChild->m_vPos + 10000.0f*vVel;
	}
	else
	{
		// 추가 2004.06.23 jschoi
		m_vCamNextPos = g_pD3dApp->m_pShuttleChild->m_vPos - m_fDistance*vVel + m_fHeight*vUp;
		m_vNextTargetPos = g_pD3dApp->m_pShuttleChild->m_vPos + 10000.0f*vVel;
	}

	// 2005-01-27 by jschoi - 자연스런 카메라 이동 체크
	if(	g_pD3dApp->m_bChatMode || 
		g_pD3dApp->m_bFixUnitDirMode )	// 유닛 진행 방향 고정 모드
	{
		SetSmoothMove();
	}

	// 아래코드 현재 포지션을 계산하되 다음 포지션을 넘어서 지나치지 않게 셋팅한다.
	D3DXVECTOR3 vV1,vV2,vV3;
	D3DXVec3Normalize(&vV1,&(m_vCamNextPos - m_vCamCurrentPos));
	D3DXVECTOR3 vCamCurrentPos = m_vCamCurrentPos + (m_fCheckSpeedRate)*(m_vCamNextPos - m_vCamCurrentPos)*fElapsedTime;
	D3DXVec3Normalize(&vV2,&(m_vCamNextPos - vCamCurrentPos));
	vV3 = vV1 + vV2;
	if(D3DXVec3Length(&vV3) > 1.5f)
	{
		m_vCamCurrentPos = vCamCurrentPos;
	}
	else
	{
		m_vCamCurrentPos = m_vCamNextPos;
	}

	if(m_bCamType == CAMERA_TYPE_NORMAL)
	{
		if(fElapsedTime > 0.5f)
			m_vCamCurrentPos = m_vCamNextPos;

//		m_vCurrentTargetPos = m_vNextTargetPos;
		// 2004-12-21 by jschoi - 카메라 방향이 서서히 돌아가도록
		if(m_bSmoothMove)
		{
			D3DXVECTOR3 vTargetVel,vNextVel;
			D3DXVec3Normalize(&vTargetVel,&m_vCurrentTargetPos);
			D3DXVec3Normalize(&vNextVel,&m_vNextTargetPos);
			float fAngle = ACOS(D3DXVec3Dot(&vNextVel,&vTargetVel));
			if(fAngle > 4*fElapsedTime && fAngle < PI  && m_fSmoothTime>0.0f)
			{
				m_fSmoothTime -= fElapsedTime;
				float fFrameMove = fAngle*4*fElapsedTime;
				if(fFrameMove > 1.0f)
					fFrameMove = 1.0f;
				m_vCurrentTargetPos += (m_vNextTargetPos - m_vCurrentTargetPos)*fFrameMove;
			}
			else
			{
				m_vCurrentTargetPos = m_vNextTargetPos;
				m_bSmoothMove = FALSE;
			}
		}
		else
		{
			m_vCurrentTargetPos = m_vNextTargetPos;
		}
	}
	else
	{
		m_vCamCurrentPos = g_pD3dApp->m_pShuttleChild->m_vPos + 1.0f*vVel + 5.0f*vUp;
		m_vCurrentTargetPos = m_vNextTargetPos + 1.0f*vVel + 5.0f*vUp;
	}

	D3DXVECTOR3 vCamEyePos, vCamLookAtPos, vCamUp;	

	if(g_pShuttleChild && g_pShuttleChild->m_bTurnCamera)
	{
		if(GetCamType() == CAMERA_TYPE_FPS) // 1인칭
		{
			if(g_pD3dApp->m_pShuttleChild->m_bAttackMode == _SIEGE)
			{
				vCamEyePos = g_pD3dApp->m_pShuttleChild->m_vPos;// + g_pD3dApp->m_pShuttleChild->m_vWeaponPos.x*g_pD3dApp->m_pShuttleChild->m_vSideVel 
//					+ g_pD3dApp->m_pShuttleChild->m_vWeaponPos.y*g_pD3dApp->m_pShuttleChild->m_vUp 
//					+ g_pD3dApp->m_pShuttleChild->m_vWeaponPos.z*g_pD3dApp->m_pShuttleChild->m_vVel + 5.0f*g_pD3dApp->m_pShuttleChild->m_vWeaponVel;
				vCamLookAtPos = vCamEyePos + 7.0f*g_pD3dApp->m_pShuttleChild->m_vWeaponVel;
				vCamUp = g_pD3dApp->m_pShuttleChild->m_vWeaponUp;
			}
			else
			{
				vCamEyePos = g_pD3dApp->m_pShuttleChild->m_vPos-D3DXVec3Length(&(g_pD3dApp->m_pShuttleChild->m_vPos-m_vCamCurrentPos))*vVel;
				vCamLookAtPos = g_pD3dApp->m_pShuttleChild->m_vPos-60.0f*vVel;
				vCamUp = vUp;
			}
		}
		else	// 기타 3인칭
		{
			vCamEyePos = g_pD3dApp->m_pShuttleChild->m_vPos+D3DXVec3Length(&(g_pD3dApp->m_pShuttleChild->m_vPos-m_vCamCurrentPos))*vVel;
			vCamLookAtPos = g_pD3dApp->m_pShuttleChild->m_vPos-20.0f*vVel;
			vCamUp = vUp;
		}
	}
	else
	{
		if(g_pD3dApp->m_pShuttleChild->m_bAttackMode == _SIEGE)
		{
			// ydkim 04.06.28
			//if(!g_pD3dApp->m_pShuttleChild->m_bMouseLock)
			//{
//			vCamEyePos = g_pShuttleChild->m_vPos + g_pShuttleChild->m_vWeaponPos.x*g_pShuttleChild->m_vSideVel 
//				+ g_pShuttleChild->m_vWeaponPos.y*g_pShuttleChild->m_vUp 
//				+ g_pShuttleChild->m_vWeaponPos.z*g_pShuttleChild->m_vVel + 5.0f*g_pShuttleChild->m_vWeaponVel;
			D3DXMATRIX matTemp = g_pShuttleChild->m_pMatWeaponPosition[0] * g_pShuttleChild->m_mMatrix;
			D3DXVECTOR3 vWeapon;
			vWeapon.x = matTemp._41;
			vWeapon.y = matTemp._42;
			vWeapon.z = matTemp._43;
			vCamEyePos = vWeapon + 5.0f*g_pShuttleChild->m_vWeaponVel;

			vCamLookAtPos = vCamEyePos + 7.0f*g_pShuttleChild->m_vWeaponVel;
			vCamUp = vUp;
/*			CAppEffectData* pWeapon1 = g_pShuttleChild->m_pWeapon1_1;
			CAppEffectData* pWeapon2 = g_pShuttleChild->m_pWeapon1_2;
			if(pWeapon1 == NULL)
			{
				pWeapon1 = pWeapon2;
			}
			if(pWeapon1 != NULL)
			{
				D3DXVECTOR3 vAWeaponPos = A_GEAR_PRIMARY_PORT_POSITION;
				D3DXVECTOR3 vUnitPos = D3DXVECTOR3(g_pShuttleChild->m_mMatrix._41, g_pShuttleChild->m_mMatrix._42, g_pShuttleChild->m_mMatrix._43 );
				D3DXVECTOR3 vUnitVel = D3DXVECTOR3(g_pShuttleChild->m_mMatrix._31, g_pShuttleChild->m_mMatrix._32, g_pShuttleChild->m_mMatrix._33 );
				D3DXVECTOR3 vUnitUp = D3DXVECTOR3(g_pShuttleChild->m_mMatrix._21, g_pShuttleChild->m_mMatrix._22, g_pShuttleChild->m_mMatrix._23 ); 
//				vCamEyePos = vUnitPos + vAWeaponPos.y*vUnitUp + vAWeaponPos.z*vUnitVel;
				vCamEyePos = vUnitPos;
				vCamLookAtPos = vCamEyePos + 7.0f*g_pShuttleChild->m_vWeaponVel;
				vCamUp = vUnitUp;
			}
*/

				// 카메라 에니메이션을 위한 좌표 
				m_vCamSlowNextPos = vCamLookAtPos;
				m_fFrameLerp = 0;
			//}
			//else
			//{
			//	vCamUp = g_pD3dApp->m_pShuttleChild->m_vUp;
			//	vCamLookAtPos = g_pD3dApp->m_pShuttleChild->m_vPos;
			//	D3DXMatrixRotationAxis(&mat,&vCamUp,m_fSideAngle);
			//	D3DXVec3Normalize(&vVel,&g_pD3dApp->m_pShuttleChild->m_vVel);
			//	D3DXVec3TransformCoord(&vVel,&vVel,&mat);
			//	vCamEyePos = g_pD3dApp->m_pShuttleChild->m_vPos - m_fDistance*vVel + m_fHeight*g_pD3dApp->m_pShuttleChild->m_vUp;
			//}
		}
		else
		{
			vCamEyePos = vCamCurrentPos;
//			CheckCollForMap(vCamEyePos);		// 2004.06.23 jschoi
//			vCamEyePos = vCamCurrentPos;
//			CheckCollMapObject(vCamEyePos);		// 2004.06.23 jschoi
//			vCamEyePos = vCamCurrentPos;
			vCamLookAtPos = m_vCurrentTargetPos;
			vCamUp = vUp;
		}
	}


	// 카메라 떨림 체크 - // 2005-03-31 by jschoi 튜토리얼 추가
	D3DXVECTOR3 vVelTemp,vCrossTemp;
	if(	g_pTutorial->IsTutorialMode() == FALSE ||
		g_pTutorial->IsUseShuttleTick() == TRUE )
	{
		if(m_fCheckCollTime > 0.25f)
			m_fCheckCollTime -= 10.0f*fElapsedTime;
		if(m_fCheckCollTime <= 0.25f)
		{
			m_fCheckCollTime = 0.25f;
			if(m_bCamCollType != 0)
			{
				m_bCamCollType = 0;
				m_bSmoothMove = FALSE;
			}
			else
			{
				m_fCheckCollTime = 10.75f;
			}
		}
		int nTemp = (int)m_fCheckCollTime;
		float fValue = m_fCheckCollTime - nTemp;
		float fCollRate = 1.0f;	// 데미지비율, 셔틀 속도 비율로 떨림 크기 설정될 변수
		if(g_pShuttleChild->m_bAttackMode == _SIEGE)
		{
			fCollRate = 0.1f;
		}
		
		D3DXVec3Normalize(&vVelTemp,&(vCamLookAtPos - vCamEyePos));
		D3DXVec3Cross(&vCrossTemp,&vCamUp,&vVelTemp);
		D3DXVec3Normalize(&vCrossTemp,&vCrossTemp);
		D3DXVec3Cross(&vCamUp,&vVelTemp,&vCrossTemp);
		if(fValue > 0.5f)
		{
			fValue = fValue - 0.5f - 0.25f;
		}
		else
		{
			fValue = -(fValue - 0.5f) - 0.25f;
		}
		switch(m_bCamCollType)
		{
		case 1:
			{// UpVec 흔들림
				D3DXMatrixRotationAxis(&mat,&vVelTemp,0.1f*fCollRate*fValue);
				D3DXVec3TransformCoord(&vCamUp,&vCamUp,&mat);
			}
			break;
		case 2:
			{// PosVec 흔들림(좌우)
				vCamEyePos += 5.0f*fCollRate*vCrossTemp*fValue;
	//			vCamLookAtPos += 10.0f*vCrossTemp*fValue;
			}
			break;
		case 3:
			{// PosVec 흔들림(상하)
				vCamEyePos += 5.0f*fCollRate*vCamUp*fValue;
	//			vCamLookAtPos += 10.0f*vCamUp*fValue;
			}
			break;
		case 4:
			{// PosVec 흔들림(좌우)
				vCamEyePos += 5.0f*fCollRate*vCrossTemp*fValue;
				vCamLookAtPos += 2.0f*fCollRate*vCrossTemp*fValue;
			}
			break;
		case 5:
			{// PosVec 흔들림(상하)
				vCamEyePos += 5.0f*fCollRate*vCamUp*fValue;
				vCamLookAtPos += 2.0f*fCollRate*vCamUp*fValue;
			}
			break;
		case 6:
			{// PosVec 흔들림(좌우상하)
				vCamEyePos += 5.0f*fCollRate*vCrossTemp*fValue;
				fValue = m_fCheckCollTime - 0.16f;
				nTemp = (int)m_fCheckCollTime;
				fValue = fValue - nTemp;
				if(fValue > 0.5f)
				{
					fValue = fValue - 0.5f - 0.25f;
				}
				else
				{
					fValue = -(fValue - 0.5f) - 0.25f;
				}
				vCamEyePos += 5.0f*fCollRate*vCamUp*fValue;
			}
			break;
		case 7:
			{// PosVec 흔들림(좌우상하,UpVec)
				vCamEyePos += 5.0f*fCollRate*vCrossTemp*fValue;
				fValue = m_fCheckCollTime - 0.16f;
				nTemp = (int)m_fCheckCollTime;
				fValue = fValue - nTemp;
				if(fValue > 0.5f)
				{
					fValue = fValue - 0.5f - 0.25f;
				}
				else
				{
					fValue = -(fValue - 0.5f) - 0.25f;
				}
				vCamEyePos += 5.0f*fCollRate*vCamUp*fValue;
				fValue = m_fCheckCollTime - 0.8f;
				nTemp = (int)m_fCheckCollTime;
				fValue = fValue - nTemp;
				if(fValue > 0.5f)
				{
					fValue = fValue - 0.5f - 0.25f;
				}
				else
				{
					fValue = -(fValue - 0.5f) - 0.25f;
				}
				D3DXMatrixRotationAxis(&mat,&vVelTemp,0.1f*fCollRate*fValue);
				D3DXVec3TransformCoord(&vCamUp,&vCamUp,&mat);
			}
			break;
		default:
			{
				if(g_pD3dApp->m_pShuttleChild->m_nBooster == BOOSTER_TYPE_MAX)
				{
					fCollRate = g_pD3dApp->m_pShuttleChild->m_fShuttleSpeed/50.0f;
					vCamEyePos += 0.5f*fCollRate*vCrossTemp*fValue;
					vCamLookAtPos += 0.5f*fCollRate*vCrossTemp*fValue;
					fValue = m_fCheckCollTime - 0.16f;
					nTemp = (int)m_fCheckCollTime;
					fValue = fValue - nTemp;
					if(fValue > 0.5f)
					{
						fValue = fValue - 0.5f - 0.25f;
					}
					else
					{
						fValue = -(fValue - 0.5f) - 0.25f;
					}
					vCamEyePos += 0.2f*fCollRate*vCamUp*fValue;
					fValue = m_fCheckCollTime - 0.8f;
					nTemp = (int)m_fCheckCollTime;
					fValue = fValue - nTemp;
					if(fValue > 0.5f)
					{
						fValue = fValue - 0.5f - 0.25f;
					}
					else
					{
						fValue = -(fValue - 0.5f) - 0.25f;
					}
					vCamLookAtPos += 0.2f*fCollRate*vCamUp*fValue;
				}
			}
		}
	}

	if(g_pD3dApp->m_pShuttleChild->m_bOrderMove)
	{
		vCamUp = g_pD3dApp->m_pShuttleChild->m_vUp;
		vCamLookAtPos = g_pD3dApp->m_pShuttleChild->m_vPos;
		vCamEyePos = GetEyePt();
		vCamEyePos += (g_pD3dApp->m_pShuttleChild->m_vPos - m_fDistance*vVel + m_fHeight*g_pD3dApp->m_pShuttleChild->m_vUp - vCamEyePos)*fElapsedTime;
		m_fCamControlCheckTime = 0.0f;
	}
	else if(m_bIsCamControl ||
		g_pShuttleChild->m_dwState == _LANDED)
	{

		POINT pt;
		GetCursorPos(&pt);
		ScreenToClient(g_pD3dApp->GetHwnd(), &pt);
		CheckMouseReverse(&pt);	

		// 2005-07-25 by ispark
		BOOL bResultMap = FALSE;
		BOOL bResultObj = FALSE;
		vCamUp = g_pShuttleChild->m_vUp;
		vCamLookAtPos = g_pShuttleChild->m_vPos;
		D3DXVec3Normalize(&vVel,&g_pShuttleChild->m_vVel);
		D3DXMatrixRotationAxis(&mat,&vCamUp,m_fSideAngle);
		D3DXVec3TransformCoord(&vVel,&vVel,&mat);

		vCamEyePos = g_pShuttleChild->m_vPos - m_fDistance*vVel + m_fHeight*g_pShuttleChild->m_vUp;

		m_fCamControlCheckTime = 0.0f;

		// 2006-05-12 by ispark, 지형 충돌
		if(CheckCollForMap(vCamEyePos))
		{
#ifdef _DEBUG
			bResultMap = FALSE;
#else
			bResultMap = TRUE;
#endif
		}
		
		if(bResultMap || bResultObj)
		{// 충돌이면
			SetViewParams(m_vOldCamEyePos, vCamLookAtPos, vCamUp);
			return ;
		}
	}
	else if(g_pD3dApp->m_pShuttleChild->m_bAttackMode != _SIEGE)
	{
		if(abs(m_fOriDistance - m_fDistance) > 0.01f)
		{
			m_fDistance += (m_fOriDistance - m_fDistance)*fElapsedTime;
		}
		else
		{
			m_fDistance = m_fOriDistance;
		}
		if(abs(m_fOriHeight - m_fHeight) > 0.01f)
		{
			m_fHeight += (m_fOriHeight - m_fHeight)*fElapsedTime;
		}
		else
		{
			m_fHeight = m_fOriHeight;
		}

		if(D3DXVec3Length(&(g_pD3dApp->m_pShuttleChild->m_vPos - vCamEyePos)) > 60.0f ) //100.0f) // 2005.6.13 by dhkwon
		{
			D3DXVec3Normalize(&vVelTemp, &(vCamEyePos - g_pD3dApp->m_pShuttleChild->m_vPos));
			vCamEyePos = g_pD3dApp->m_pShuttleChild->m_vPos + vVelTemp*60.0f; //100.0f; // 2005.6.13 by dhkwon
//			CheckCollForMap(vCamEyePos);		// 2004.06.23 jschoi
//			vCamEyePos = g_pD3dApp->m_pShuttleChild->m_vPos + vVelTemp*100.0f;
//			CheckCollMapObject(vCamEyePos);		// 2004.06.23 jschoi
//			vCamEyePos = g_pD3dApp->m_pShuttleChild->m_vPos + vVelTemp*100.0f;
		}
		if(m_fDistance != m_fOriDistance || m_fHeight != m_fOriHeight)
		{
			vCamUp = g_pD3dApp->m_pShuttleChild->m_vUp;
//			vCamLookAtPos = g_pD3dApp->m_pShuttleChild->m_vPos;
			D3DXMatrixRotationAxis(&mat,&vCamUp,m_fSideAngle);
			D3DXVec3Normalize(&vVel,&g_pD3dApp->m_pShuttleChild->m_vVel);
			D3DXVec3TransformCoord(&vVel,&vVel,&mat);
//			vCamEyePos = g_pD3dApp->m_pShuttleChild->m_vPos - m_fDistance*vVel + m_fHeight*g_pD3dApp->m_pShuttleChild->m_vUp;

			if(m_fCamControlCheckTime < 1.0f)
				m_fCamControlCheckTime += 0.005f*fElapsedTime;
			if(m_fCamControlCheckTime > 1.0f)
				m_fCamControlCheckTime = 1.0f;
			vCamLookAtPos = g_pD3dApp->m_pShuttleChild->m_vPos + m_fCamControlCheckTime*(vCamLookAtPos - g_pD3dApp->m_pShuttleChild->m_vPos);
		}
		else
		{
			if(m_fCamControlCheckTime < 1.0f)
				m_fCamControlCheckTime += 0.2f*fElapsedTime;
			if(m_fCamControlCheckTime > 1.0f)
				m_fCamControlCheckTime = 1.0f;
			vCamLookAtPos = g_pD3dApp->m_pShuttleChild->m_vPos + m_fCamControlCheckTime*(vCamLookAtPos - g_pD3dApp->m_pShuttleChild->m_vPos);
		}
	}

	if(m_bCamType == CAMERA_TYPE_FPS)
	{
		// ydkim 04.06.28
		if(IS_DT(g_pShuttleChild->m_myShuttleInfo.UnitKind) && // DT형이 땅에서 다니나?
			!g_pD3dApp->m_pShuttleChild->m_bIsAir)	
		{
			// DT 형이 지상에서 1인칭으로 바뀌었을때 
		}
		else
		{
			vCamEyePos = D3DXVECTOR3(g_pShuttleChild->m_mMatrix._41, g_pShuttleChild->m_mMatrix._42, g_pShuttleChild->m_mMatrix._43);
			vCamUp = D3DXVECTOR3(g_pShuttleChild->m_mMatrix._21, g_pShuttleChild->m_mMatrix._22, g_pShuttleChild->m_mMatrix._23);
		}
	}

	// 2004.06.23 jschoi
	// 이전값 저장.
	m_fOldHeight = m_fHeight;
	m_fOldDistance = m_fDistance;
	m_vOldCamEyePos = vCamEyePos;
	m_fOldSideAngle = m_fSideAngle;
	SetViewParams(vCamEyePos, vCamLookAtPos, vCamUp);
}

void CCamera::Tick( D3DXVECTOR3 vObjPos,
				   D3DXVECTOR3 vObjFBVel,
				   D3DXVECTOR3 vObjUp,
				   BYTE type )
{
	FLOG( "Tick(D3DXVECTOR3 vObjPos, D3DXVECTOR3 vObjFBVel, D3DXVECTOR3 vObjUp, BYTE type)" );
	float fElapsedTime = g_pD3dApp->GetElapsedTime();
//	D3DXVECTOR3 vVel;//,vUp;//(0,1,0);
	m_vCamNextPos = vObjPos - m_fDistance*vObjFBVel + m_fHeight*vObjUp;
	m_vNextTargetPos = vObjPos + 20.0f*vObjFBVel;
//	float fVelRate = 10.0f;
//	vVel = fVelRate*(m_vCamNextPos - m_vCamCurrentPos)*fElapsedTime;
//	m_vCamCurrentPos += fVelRate*(m_vCamNextPos - m_vCamCurrentPos)*g_pD3dApp->GetElapsedTime();
/*
	D3DXVECTOR3 vV1,vV2,vV3;
	D3DXVec3Normalize(&vV1,&(m_vCamNextPos - m_vCamCurrentPos));
	D3DXVECTOR3 vCamCurrentPos = m_vCamCurrentPos + fVelRate*(m_vCamNextPos - m_vCamCurrentPos)*fElapsedTime;
	D3DXVec3Normalize(&vV2,&(m_vCamNextPos - vCamCurrentPos));
	vV3 = vV1 + vV2;
	if(D3DXVec3Length(&vV3) > 1.5f)
	{
		m_vCamCurrentPos = vCamCurrentPos;
	}
	else
	{
		m_vCamCurrentPos = m_vCamNextPos;
	}
*/	m_vCamCurrentPos = m_vCamNextPos;

	
	
//	vVel = fVelRate*(m_vNextTargetPos - m_vCurrentTargetPos)*g_pD3dApp->GetElapsedTime();
	m_vCurrentTargetPos = m_vNextTargetPos;// += vVel;
	
	if(type == 0)
	{
		vObjUp = GetUpVec() + (vObjUp - GetUpVec())*fElapsedTime;
	}
	SetViewParams( m_vCamCurrentPos, m_vCurrentTargetPos, vObjUp );
}

void CCamera::Tick( D3DXVECTOR3 vObjPos,
				   D3DXVECTOR3 vObjFBVel,
				   D3DXVECTOR3 vObjUp )//D3DXMATRIX m)
{
	FLOG( "CCamera::Tick(D3DXVECTOR3 vObjPos,D3DXVECTOR3 vObjFBVel,D3DXVECTOR3 vObjUp)" );
/*	D3DXVECTOR3 vUp,vVel,vPos,vTempVel,vTempUp;
	vVel.x = m._31;
	vVel.y = m._32;
	vVel.z = m._33;
	vUp.x = m._21;
	vUp.y = m._22;
	vUp.z = m._23;
	vPos.x = m._41;
	vPos.y = m._42;
	vPos.z = m._43;

	D3DXVec3Normalize(&vVel,&vVel);
	D3DXVec3Normalize(&vUp,&vUp);
	D3DXVec3Normalize(&vTempVel,&GetViewDir());
	D3DXVec3Normalize(&vTempUp,&GetUpVec());

	vVel = vTempVel + (vVel - vTempVel)*g_pD3dApp->GetElapsedTime();
	vUp = vTempUp + (vUp - vTempUp)*g_pD3dApp->GetElapsedTime();
*/
//	SetViewParams( vPos + 5.0f*vUp, vPos + 5.0f*vUp - 5.0f*vVel, vUp );

	
//	SetViewParams( vObjPos + 5.0f*vObjUp, vObjPos + 5.0f*vObjUp + 5.0f*vObjFBVel, vObjUp );

	SetViewParams(vObjPos, vObjPos + vObjFBVel, vObjUp);
	m_vCamCurrentPos = vObjPos;
	m_vCurrentTargetPos = vObjPos + vObjFBVel;
}

void CCamera::SetControlCamSideAngle(SHORT sDir)
{
	m_fSideAngle = m_fOldSideAngle;
	FLOG( "CCamera::SetControlCamSideAngle(SHORT sDir)" );
//	float fElapsedTime = g_pD3dApp->GetElapsedTime();
//	m_fSideAngle += 0.1f*sDir*fElapsedTime;
	// 2005-02-15 by jschoi
	m_fSideAngle += 0.01f*sDir;
}

void CCamera::SetControlCamAngle(SHORT sDir)
{
	FLOG( "CCamera::SetControlCamAngle(SHORT sDir, BOOL bSet)" );
	m_fHeight = m_fOldHeight;
	m_fDistance = m_fOldDistance;
//	float fElapsedTime = g_pD3dApp->GetElapsedTime();
	float fTemp = sqrt(m_fDistance*m_fDistance + m_fHeight*m_fHeight);
	float fAngleTemp = atan(m_fHeight/m_fDistance);
	
//	float fAngle = fAngleTemp + 0.1f*sDir*fElapsedTime;
	float fAngle = fAngleTemp + 0.01f*sDir;
	if(fAngle >= D3DX_PI/2.0f || fAngle <= -D3DX_PI/2.0f)
		fAngle = fAngleTemp;
	m_fHeight = sin(fAngle)*fTemp;
	m_fDistance = cos(fAngle)*fTemp;

}

void CCamera::SetControlCamPos( int nDir )
{
	FLOG( "CCamera::SetControlCamPos(int nDir)" );

	float fElapsedTime = g_pD3dApp->GetElapsedTime();

	if(nDir > 0)
		nDir = 1;
	else
		nDir = -1;
	if(m_fDistance >= m_fHeight)
	{
		float fDist = m_fDistance;
		m_fDistance += nDir*300.0f*fElapsedTime;
		if(nDir < 0)
		{
			if(m_fDistance < 10.0f)
				m_fDistance = fDist;
		}
		else
		{
			if(m_fDistance > 1000.0f)
				m_fDistance = fDist;
		}
		m_fHeight = (m_fHeight*m_fDistance)/fDist;
	}
	else
	{
		float fHeight = m_fHeight;
		m_fHeight += nDir*300.0f*fElapsedTime;
		if(nDir < 0)
		{
			if(m_fHeight < 10.0f)
				m_fHeight = fHeight;
		}
		else
		{
			if(m_fHeight > 1000.0f)
				m_fHeight = fHeight;
		}
		m_fDistance = (m_fHeight*m_fDistance)/fHeight;
	}

}

float CCamera::CheckMap( D3DXVECTOR3 vPos )
{
	FLOG( "CCamera::CheckMap(D3DXVECTOR3 vPos)" );
	FLOAT fDist;
	int i,j;
	i = (int)(vPos.x/TILE_SIZE);
	j = (int)(vPos.z/TILE_SIZE);
	vPos.y = 100.0f;
	D3DXVECTOR3 v,dir;
	dir = D3DXVECTOR3(0,-1,0);
	if( i >= 0 && 
		i < g_pGround->m_projectInfo.sXSize && 
		j >= 0 && 
		j < g_pGround->m_projectInfo.sYSize)
	{
		if(g_pGround->IntersectTriangle(vPos,dir,g_pGround->m_pTileVertexArray[i*(g_pGround->m_projectInfo.sYSize+1)+j].pos,
			g_pGround->m_pTileVertexArray[i*(g_pGround->m_projectInfo.sYSize+1)+(j+1)].pos,
			g_pGround->m_pTileVertexArray[(i+1)*(g_pGround->m_projectInfo.sYSize+1)+(j)].pos,
			&fDist))//, &fBary1, &fBary2 ))
		{
			v = vPos + dir*fDist;
			return v.y;
		}
		else if(g_pGround->IntersectTriangle(vPos,dir,g_pGround->m_pTileVertexArray[i*(g_pGround->m_projectInfo.sYSize+1)+j+1].pos,
			g_pGround->m_pTileVertexArray[(i+1)*(g_pGround->m_projectInfo.sYSize+1)+(j)].pos,
			g_pGround->m_pTileVertexArray[(i+1)*(g_pGround->m_projectInfo.sYSize+1)+j+1].pos,
			&fDist))//, &fBary1, &fBary2 ))
		{
			v = vPos + dir*fDist;
			return v.y;
		}
	}
	return 0.0f;
}


void CCamera::ChangeCamCollType( int nType /* = -1 */, float fTime /* = 7.0f */ )
{
	if (nType == -1) m_bCamCollType = RANDI(1, 7);
	
	else m_bCamCollType = nType;

	m_fCheckCollTime = fTime;
}
// end 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템

byte CCamera::CheckCullCam( D3DXVECTOR3 vPos )
{
//	FLOG( "CCamera::CheckCullCam(D3DXVECTOR3 vPos)" );
	if(vPos.x*m_pl[0].a + vPos.y*m_pl[0].b + vPos.z*m_pl[0].c + m_pl[0].d >= 0 
		&& vPos.x*m_pl[1].a + vPos.y*m_pl[1].b + vPos.z*m_pl[1].c + m_pl[1].d >= 0
		&& vPos.x*m_pl[2].a + vPos.y*m_pl[2].b + vPos.z*m_pl[2].c + m_pl[2].d >= 0
		&& vPos.x*m_pl[3].a + vPos.y*m_pl[3].b + vPos.z*m_pl[3].c + m_pl[3].d >= 0)
	{
		float fDist;
		fDist = D3DXVec3Length(&(vPos - GetEyePt()));
		if(fDist <= (g_pD3dApp->m_pScene->m_fFogEndValue + m_fRenderDistance)*m_fDetailRateGround)
		{
			return GROUND_RENDER_TILE;
		}
		else if(fDist <= g_pD3dApp->m_pScene->m_fFogEndValue + m_fRenderDistance)
		{
			return GROUND_RENDER_NORMAL;
		}
	}
	return GROUND_RENDER_NOT;
}

void CCamera::SetDetailGround( byte bType,bool bSet )
{
	FLOG( "CCamera::SetDetailGround(byte bType,bool bSet)" );
	float fElapsedTime = g_pD3dApp->GetElapsedTime();
	if(bType == 0)
	{// 맵 디테일 조절
		if(!bSet)
		{// 타일맵 렌더링 영역 줄임
			m_fDetailRateGround -= 0.1f*fElapsedTime;
			if(m_fDetailRateGround < 0.0f)
			{
				m_fDetailRateGround = 0.0f;
			}
		}
		else
		{// 타일맵 렌더링 영역 늘임
			m_fDetailRateGround += 0.1f*fElapsedTime;
			if(m_fDetailRateGround > 1.0f)
			{
				m_fDetailRateGround = 1.0f;
			}
		}
	}
	else
	{// 맵 렌더링 거리 조절
		if(!bSet)
		{// 맵 렌더링 영역 줄임
			m_fRenderDistance -= 100.0f*fElapsedTime;
		}
		else
		{// 맵 렌더링 영역 늘임
			m_fRenderDistance += 100.0f*fElapsedTime;
		}
	}
}



///////////////////////////////////////////////////////////////////////////////
/// \fn			CCamera::SetCityCamera()
/// \brief		도시 진입시 최초 세팅
/// \author		dhkwon
/// \date		2004-04-12 ~ 2004-04-12
/// \warning	아래 세 함수는 도시 카메라 컨트롤 함수이다.
///				SetCityCamera, SetCameraZoomInOut, SetCameraRotate
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CCamera::SetCityCamera()
{
	FLOG("CCamera::SetCityCamera()");
	m_vCamCurrentPos = MAP_TYPE_CITY_CAM_POS;
    auto fAspect = ((float)g_pD3dApp->GetBackBufferDesc().Width) / g_pD3dApp->GetBackBufferDesc().Height;

	SetViewParams(m_vCamCurrentPos, MAP_TYPE_CITY_UNIT_POS, D3DXVECTOR3{ 0, 1, 0 });
	SetProjParams(CITY_CAMERA_FOV, fAspect, 1.0f, 100000.0f );
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			CCamera::SetCameraZoomInOut(int nDir)
/// \brief		도시에서 줌인아웃
/// \author		dhkwon
/// \date		2004-04-12 ~ 2004-04-12
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CCamera::SetCameraZoomInOut(int nDir)
{
	FLOG("CCamera::SetCameraZoomInOut(int nDir)");
	if(nDir > 0)
		nDir = 1;
	else
		nDir = -1;
	D3DXVECTOR3 vVel = m_vCamCurrentPos - MAP_TYPE_CITY_UNIT_POS;
	D3DXVec3Normalize(&vVel, &vVel);
	m_vCamCurrentPos += vVel * nDir*10;
	float fLen = D3DXVec3Length(&(m_vCamCurrentPos-MAP_TYPE_CITY_UNIT_POS));
	if(fLen < 20 )
	{
		m_vCamCurrentPos += (20.0f - fLen)*vVel;
	}
	if(fLen > MAP_TYPE_CITY_CAM_MAX_LEN)
	{
		DBGOUT("too far to Set Camera.");//카메라 거리가 멀어 다시 세팅합니다
		m_vCamCurrentPos -= (fLen - MAP_TYPE_CITY_CAM_MAX_LEN)*vVel;
	}
	if (m_vCamCurrentPos.x - 2400 > MAP_TYPE_CITY_CAM_POS_X) m_vCamCurrentPos.x = MAP_TYPE_CITY_CAM_POS_X + 2400;
	if (m_vCamCurrentPos.x - 2400 < -MAP_TYPE_CITY_CAM_POS_X) m_vCamCurrentPos.x = -MAP_TYPE_CITY_CAM_POS_X + 2400;
	if (m_vCamCurrentPos.y > MAP_TYPE_CITY_CAM_POS_Y) m_vCamCurrentPos.y = MAP_TYPE_CITY_CAM_POS_Y;
	if (m_vCamCurrentPos.y < MAP_TYPE_CITY_CAM_POS_MIN_Y) m_vCamCurrentPos.y = MAP_TYPE_CITY_CAM_POS_MIN_Y;
	if (m_vCamCurrentPos.z - 2400 > MAP_TYPE_CITY_CAM_POS_Z) m_vCamCurrentPos.z = MAP_TYPE_CITY_CAM_POS_Z + 2400;
	if (m_vCamCurrentPos.z - 2400 < -MAP_TYPE_CITY_CAM_POS_Z) m_vCamCurrentPos.z = -MAP_TYPE_CITY_CAM_POS_Z + 2400;
	SetViewParams(m_vCamCurrentPos, MAP_TYPE_CITY_UNIT_POS, { 0, 1, 0 });
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			CCamera::SetCameraRotate(int x, int y)
/// \brief		도시에서 카메라 좌,우회전과 상하 이동
/// \author		dhkwon
/// \date		2004-04-12 ~ 2004-04-12
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CCamera::SetCameraRotate(int x, int y)
{
	FLOG("CCamera::SetCameraRotate(int x, int y)");
	D3DXMATRIX matRotate;
	D3DXVECTOR3 vecTemp = m_vCamCurrentPos - D3DXVECTOR3(2400,0,2400);
	D3DXMatrixRotationY( &matRotate,g_pD3dApp->GetElapsedTime()*x/5);
	D3DXVec3TransformCoord( &vecTemp, &vecTemp, &matRotate );
	vecTemp.y += (float)y/2;
	if(vecTemp.x > MAP_TYPE_CITY_CAM_POS_X) vecTemp.x = MAP_TYPE_CITY_CAM_POS_X;
	if(vecTemp.x < -MAP_TYPE_CITY_CAM_POS_X) vecTemp.x = -MAP_TYPE_CITY_CAM_POS_X;
	if(vecTemp.y > MAP_TYPE_CITY_CAM_POS_Y) vecTemp.y = MAP_TYPE_CITY_CAM_POS_Y;
	if(vecTemp.y < MAP_TYPE_CITY_CAM_POS_MIN_Y) vecTemp.y = MAP_TYPE_CITY_CAM_POS_MIN_Y;
	if(vecTemp.z > MAP_TYPE_CITY_CAM_POS_Z) vecTemp.z = MAP_TYPE_CITY_CAM_POS_Z;
	if(vecTemp.z < -MAP_TYPE_CITY_CAM_POS_Z) vecTemp.z = -MAP_TYPE_CITY_CAM_POS_Z;
	m_vCamCurrentPos = vecTemp + D3DXVECTOR3(2400,0,2400);
	SetViewParams(m_vCamCurrentPos, MAP_TYPE_CITY_UNIT_POS, { 0, 1, 0 });
}

// 카메라의 오브젝트와 충돌 검사
// 2004.06.23 jschoi 
BOOL CCamera::CheckCollMapObject(D3DXVECTOR3 &i_vPos)
{
	COLLISION_RESULT collResult;
//	collResult.fDist = 10000.0f;
//	collResult.vNormalVector = D3DXVECTOR3(0,0,0);
//	collResult.nCollType = COLL_NONE;
	
	D3DXVECTOR3 vUp(0,1,0);
	D3DXVECTOR3 vPos = i_vPos;
	D3DXVECTOR3 vVel = i_vPos - g_pD3dApp->m_pShuttleChild->m_vPos;
	D3DXVECTOR3 vSide;
	D3DXVec3Cross(&vSide,&vUp,&(vVel));
	D3DXMATRIX matArray[6];
//	vPos.y -= MIN_GAP_TO_GROUND;

	D3DXMatrixLookAtLH(&matArray[COLL_FRONT],&vPos,&(vPos + vVel),&vUp);	// 앞
	D3DXMatrixLookAtLH(&matArray[COLL_BACK],&vPos,&(vPos - vVel),&vUp);		// 뒤
	D3DXMatrixLookAtLH(&matArray[COLL_LEFT],&vPos,&(vPos - vSide),&vUp);		// 좌
	D3DXMatrixLookAtLH(&matArray[COLL_RIGHT],&vPos,&(vPos + vSide),&vUp);		// 우
	D3DXMatrixLookAtLH(&matArray[COLL_UP],&vPos,&(vPos + vUp),&vSide);		// 위
	D3DXMatrixLookAtLH(&matArray[COLL_DOWN],&vPos,&(vPos - vUp),&vSide);		// 아래

	float fCameraSize = 10.0f;

	float size = fCameraSize;	// 카메라의 크기

//	D3DXMATRIX mat;
//	D3DXMatrixLookAtLH(&mat,&vPos,&(vPos + vUp),&vSide);

	collResult = g_pScene->m_pObjectRender->CheckCollForCamera(matArray,size);

	if(collResult.nCollType != COLL_NONE )
	{
//		DBGOUT("카메라 충돌 : %d\n",collResult.nCollType );
	//	i_vPos = GetEyePt() + collResult.vNormalVector*(fCameraSize - collResult.fDist);		

//		if(collResult.nCollType == COLL_WHOLE)
//		{
		         // 이곳으로 들어온다면 나중에 뭔가 처리해 주자.
//			DBGOUT("완전충돌 \n");
//		}

		return TRUE;
	}
	return FALSE;
}

// 카메라의 지형맵+물지형 과의 충돌 검사
// 2004.06.23 jschoi 
BOOL CCamera::CheckCollForMap(D3DXVECTOR3 vPos)
{
	if(IsTileMapRenderEnable(g_pShuttleChild->m_myShuttleInfo.MapChannelIndex.MapIndex) == FALSE)
	{
		return FALSE;
	}
	D3DXVECTOR3 vCheckPos[6];
	D3DXVECTOR3 vCheckVel = vPos - g_pD3dApp->m_pShuttleChild->m_vPos;
	D3DXVECTOR3 vCheckSide;
	D3DXVECTOR3 vCheckUp = D3DXVECTOR3(0,1,0);
	D3DXVec3Normalize(&vCheckVel,&vCheckVel);
	D3DXVec3Cross(&vCheckSide,&vCheckUp,&(vCheckVel));
	float fCameraSize = 10.0f;
	vCheckPos[0] = vPos + vCheckVel*fCameraSize;	// 앞
	vCheckPos[1] = vPos - vCheckVel*fCameraSize;	// 뒤
	vCheckPos[2] = vPos + vCheckSide*fCameraSize;	// 좌
	vCheckPos[3] = vPos - vCheckSide*fCameraSize;	// 우아
//	vCheckPos[4] = vPos + vCheckUp*fCameraSize;		// 위
//	vCheckPos[4] = vPos - vCheckUp*fCameraSize;		// 중앙아래

	float fCheckHeight;

	for (int i = 0; i < 4 ; i++)
	{
		BOOL bIsWater = g_pShuttleChild->CheckIsWater(vCheckPos[i]);
		if(bIsWater)
		{
			fCheckHeight = g_pGround->m_projectInfo.fWaterHeight +MIN_GAP_TO_GROUND;
		}
		else
		{
			fCheckHeight = CheckMap(vCheckPos[i]) + MIN_GAP_TO_GROUND;
		}
		if( fCheckHeight > vCheckPos[i].y)
		{
			return TRUE;
		}
	}

	return FALSE;
}

// 2004-08-17 jschoi
// 카메라의 평면과 거리 구하는 함수
float CCamera::GetDistanceToViewPlane(D3DXVECTOR3 vPos)
{
	D3DXVECTOR3 vPlanePos;
	D3DXPlaneIntersectLine(&vPlanePos,&m_planeView,&vPos,&(vPos+GetViewDir())) ;

	if(vPlanePos != D3DXVECTOR3(0,0,0))
	{
		return D3DXVec3Length(&(vPlanePos - vPos));
	}
	return -1;
}

void CCamera::SetViewPlane()
{
	D3DXPlaneFromPointNormal( &m_planeView, &GetEyePt(), &GetViewDir() );
}

D3DXPLANE CCamera::GetViewPlane()
{
	return m_planeView;
}

void CCamera::SetSmoothMove() 
{ 
	m_bSmoothMove = TRUE; 
	m_fSmoothTime = SMOOTH_CAMERA_MAX_TIME;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			CCamera::RotationLocalX(float fangle)
/// \brief		카메라 X 3인칭 회전 : 회전값으로 계산, 상하 각도 제한
/// \author		ispark
/// \date		2005-08-17 ~ 2005-08-18
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CCamera::RotationLocal3X(float fangle)
{
	float fAngle_UpDown = 0.0f;
	D3DXMATRIX matRot;
	D3DXVECTOR3 vNewDst;
	D3DXVECTOR3 vCamLookAtPos = g_pD3dApp->m_pShuttleChild->m_vPos;

	vCamLookAtPos.y += GetCharacterHeight(g_pCharacterChild->m_nUnitNum);
	m_fCollDistance = 0.0f;

	// 각도 제한
	fAngle_UpDown = fangle * ROTATION_ANGLE;

	if(m_fTotalAngleX + (fangle * ROTATION_ANGLE) < ANGLE_X_UP)
		fAngle_UpDown = ANGLE_X_UP - m_fTotalAngleX;

	if(m_fDistance <= CHARACTER_CAMERA_DISTANCE_MIN)
	{
		// 거리가 최소일때 밑으로 내리는 각도를 축소시킨다.
		if(m_fTotalAngleX + (fangle * ROTATION_ANGLE) > ANGLE_X_DOWN - 0.3f)
			fAngle_UpDown = ANGLE_X_DOWN - m_fTotalAngleX - 0.3f;			
	}
	else
	{
		if(m_fTotalAngleX + (fangle * ROTATION_ANGLE) > ANGLE_X_DOWN)
			fAngle_UpDown = ANGLE_X_DOWN - m_fTotalAngleX;			
	}
	
	m_fTotalAngleX += fAngle_UpDown;

	D3DXMatrixRotationAxis(&matRot , &GetCross() , fAngle_UpDown);
	D3DXVec3TransformCoord(&vNewDst , &(m_vCamNextPos - vCamLookAtPos) , &matRot); // view * rot로 새로운 dst vector를 구한다.
	
	vNewDst += vCamLookAtPos;
	m_vCamNextPos = vNewDst;			// 계산용 위치

	D3DXVec3Normalize(&m_vDistance, &(vNewDst - vCamLookAtPos));

	// 충돌 처리
	auto fCollDis = CheckCollMapObject(vCamLookAtPos, vNewDst);

	// 2006-07-14 by ispark, 이벤트 오브젝트 렌더링시 확인을 위해서 오브젝트 충돌 안함

#ifdef _DEBUG
	if (fCollDis != 0 && g_pD3dApp->m_bEventObjectRender == false)
#else
	if (fCollDis != 0)
#endif
	{
		auto vNewCam = m_vDistance;
		m_fCollDistance = fCollDis;
		vNewCam *= fCollDis;
		vNewDst = vCamLookAtPos + vNewCam;
	}
	
	m_vCamCurrentPos = vNewDst;			// 최종 위치

	SetViewParams(vNewDst, vCamLookAtPos, { 0, 1, 0 });
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			CCamera::RotationLocalY(float fangle)
/// \brief		카메라 Y 3인칭 회전 : 회전값으로 계산
/// \author		ispark
/// \date		2005-08-17 ~ 2005-08-18
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CCamera::RotationLocal3Y(float fangle)
{
	D3DXMATRIX matRot;
	D3DXVECTOR3 vNewDst;
	D3DXVECTOR3 vCamLookAtPos = g_pD3dApp->m_pShuttleChild->m_vPos;

	vCamLookAtPos.y += GetCharacterHeight(g_pCharacterChild->m_nUnitNum);
	m_fCollDistance = 0.0f;

	D3DXMatrixRotationAxis(&matRot , &D3DXVECTOR3(0,1,0) , fangle * ROTATION_ANGLE);
	D3DXVec3TransformCoord(&vNewDst , &(m_vCamNextPos - vCamLookAtPos) , &matRot); // view * rot로 새로운 dst vector를 구한다.
	
	vNewDst += vCamLookAtPos;
	m_vCamNextPos = vNewDst;			// 계산용 위치

	D3DXVec3Normalize(&m_vDistance, &(vNewDst - vCamLookAtPos));

	// 충돌 처리
	float fCollDis = CheckCollMapObject(vCamLookAtPos, vNewDst);
	// 2006-07-14 by ispark, 이벤트 오브젝트 렌더링시 확인을 위해서 오브젝트 충돌 안함
	BOOL bEventObjectRender = FALSE;
#ifdef _DEBUG
	bEventObjectRender = g_pD3dApp->m_bEventObjectRender;
#endif
	if(fCollDis != 0 && bEventObjectRender == FALSE)
	{
		D3DXVECTOR3 vNewCam = m_vDistance;
		m_fCollDistance = fCollDis;
		vNewCam *= fCollDis;
		vNewDst = vCamLookAtPos + vNewCam;
	}
	
	m_vCamCurrentPos = vNewDst;			// 최종 위치

	SetViewParams(vNewDst, vCamLookAtPos, { 0, 1, 0 });
	return TRUE;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			CCamera::CharacterCamTick()
/// \brief		캐릭터 전용 카메라 Tick()
/// \author		ispark
/// \date		2005-08-16 ~ 2005-08-16
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CCamera::CharacterCamTick()
{
	if(m_fAngleY != 0)
		RotationLocal3X(m_fAngleY);
	if(m_fAngleX != 0)
		RotationLocal3Y(m_fAngleX);
//	DBGOUT("AngleX %f\n", m_fAngleX);
//	DBGOUT("AngleY %f\n", m_fAngleY);
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			CCamera::SetCamMove(D3DXVECTOR3 vMoveDir)
/// \brief		카메라 움직임
/// \author		ispark
/// \date		2005-08-16 ~ 2005-08-16
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CCamera::SetCamMove(BOOL bRButton, BOOL bWhell, BOOL bWarp)
{
//	DBGOUT("MoveDir %f, %f, %f\n", vMoveDir.x, vMoveDir.y, vMoveDir.z);
	D3DXVECTOR3 vCamLookAtPos = g_pD3dApp->m_pShuttleChild->m_vPos;
	vCamLookAtPos.y += GetCharacterHeight(g_pCharacterChild->m_nUnitNum);
	m_fCollDistance = 0.0f;

	// 거리를 여기서 계산
	D3DXVECTOR3 vNewCam = m_vDistance;
	D3DXVECTOR3 vTempNewCam = vNewCam;

	vNewCam *= m_fDistance;
	vNewCam += vCamLookAtPos;
	m_vCamNextPos = vNewCam;
	
	// 충돌 처리
	float fCollDis = CheckCollMapObject(vCamLookAtPos, vNewCam);
	// 2006-07-14 by ispark, 이벤트 오브젝트 렌더링시 확인을 위해서 오브젝트 충돌 안함
	BOOL bEventObjectRender = FALSE;
#ifdef _DEBUG
	bEventObjectRender = g_pD3dApp->m_bEventObjectRender;
#endif
	if(fCollDis != 0 && bEventObjectRender == FALSE)
	{
		m_fCollDistance = fCollDis;
		vNewCam = vTempNewCam * fCollDis;
		vNewCam += vCamLookAtPos;
	}
	
	m_vCamCurrentPos = vNewCam;
//
	SetViewParams(vNewCam, vCamLookAtPos, { 0, 1, 0 });

	//////////////////////////////////////////////////////////////////////////
	// 카메라 회전
	if(bRButton == FALSE && bWhell == FALSE && bWarp == FALSE)
	{
		D3DXVECTOR3 vChaVel = -g_pCharacterChild->GetVecPickDir();
		D3DXVECTOR3 vCamDist = vNewCam - vCamLookAtPos;
		D3DXVECTOR3 vCrossVec;
		vChaVel.y = 0.0f;
		vCamDist.y = 0.0f;
		D3DXVec3Normalize(&vCamDist, &vCamDist);
		D3DXVec3Normalize(&vChaVel, &vChaVel);

		D3DXVec3Cross(&vCrossVec, &vChaVel, &vCamDist);
		float dot = D3DXVec3Dot(&vChaVel, &vCamDist);
		if(0.999f > dot)
		{
			float fAngel;
			dot = dot < (1.0f - g_pD3dApp->GetElapsedTime()) ? dot : (1.0f - g_pD3dApp->GetElapsedTime());
			if(0.0f > vCrossVec.y)
			{
				fAngel = ACOS(dot);
			}
			else
			{
				fAngel = -ACOS(dot);
			}

			g_pCamera->SetAngleX(fAngel * 2.0f);
		}
	}
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			CCamera::CheckCollMapObject(D3DXVECTOR3 vPos, D3DXVECTOR3 vLookAt)
/// \brief		카메라와 오브젝트 충돌
/// \author		ispark
/// \date		2005-08-18 ~ 2005-08-18
/// \warning	
///
/// \param		
/// \return		COLLISION_RESULT
///////////////////////////////////////////////////////////////////////////////
float CCamera::CheckCollMapObject(D3DXVECTOR3 vPos, D3DXVECTOR3 vLookAt)
{
	COLLISION_RESULT collResult;
	D3DXVECTOR3 vUp(0,1,0), vNewDist;
	D3DXMATRIX matCam;
	float y1, y2, ny1, ny2;

	D3DXVec3Normalize(&vNewDist, &(vLookAt - vPos));
	y1 = GetCharacterHeight(g_pCharacterChild->m_nUnitNum);
	y2 = y1 - 3.0f;
	ny1 = m_fDistance;
	ny2 = (y1 * ny1) / y2;
	vLookAt = vNewDist * ny2 + vPos;

	D3DXMatrixLookAtLH(&matCam, &vPos, &vLookAt, &vUp);

	collResult = g_pScene->m_pObjectRender->CheckCollMesh(matCam, vPos, ny2);

	if(collResult.fDist != 0 && collResult.fDist <= ny2)
	{
//		if(collResult.vNormalVector.y > 0.6f)
//		{
			y1 = GetCharacterHeight(g_pCharacterChild->m_nUnitNum);
			y2 = y1 - 3.0f;
			ny1 = collResult.fDist;

			ny2 = (y2 * ny1) / y1;
			return ny2;
//		}
//		else
//			return collResult.fDist - 0.2f;
	}
	
	return 0.0f;
}

void CCamera::SetCamType(BYTE bType)
{
	if(CAMERA_TYPE_NORMAL == bType)
	{
		m_bCamType = bType;
	}
	else
	{
		 if(g_pShuttleChild->GetRenderInvenFlag() == FALSE)
			 m_bCamType = bType;		 
	}
}
