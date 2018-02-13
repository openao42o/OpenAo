// Cinema.cpp: implementation of the CCinema class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Cinema.h"
#include "DXUtil.h"

#ifndef _ATUM_CLIENT

	#include "CinemaEditer.h"
	#include "CinemaEditerView.h"
	extern LPDIRECT3DDEVICE9		g_pD3dDev;
	extern CCinemaEditerView*		g_AppFormView;

#else

	#include "AtumApplication.h"
	extern CAtumApplication			*g_pD3dApp;

#endif // _ATUM_CLIENT_endif 시네마에디터에서만 사용함


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


using namespace std;

CCinema::CCinema()
{
	// 붉은 색 X 축, 녹색 Y 축 , 파란색 Z 축
	m_CurrentCinemaPoint.vPosition = D3DXVECTOR3(0,0,0);	// 시네마
	m_CurrentCinemaPoint.vDirection = D3DXVECTOR3(0,0,1);
	m_CurrentCinemaPoint.vUpVector = D3DXVECTOR3(0,1,0);
	m_CurrentCinemaPoint.vTarget = D3DXVECTOR3(0,0,0);
	m_CurrentCinemaPoint.fVelocity = 100.0f;					// 초기속도 나중에 생성자에서 또는 Setting을 해야함..
	m_CurrentCinemaPoint.fCurvature = 1.0f;
	m_bRender = TRUE;
	m_nTargetPointIndex= 0;		// 타켓 점의 번호
	m_nNumOfCinema = 0;			// 현재 Cinema 포인트의 개수
	m_nNumOfEvent = 0;			// 현재 Event 포인트의 개수
	m_pCinemaVB = NULL;			// Cinema 포인트 점버퍼
	m_pEventVB = NULL;			// Event 포인트 점버퍼
	m_pSelectedVB = NULL;		// Selected 포인트 점버퍼
	m_pCinemaUpVectorVB = NULL;		// UpVector 를 표현하기 위한 점버퍼
	m_pEventUpVectorVB = NULL;  // Event Point 에서 UpVector데이터를 Edit 하기 위한 점버퍼
	m_pTargetVertexVB = NULL;	/// TargetVertex
	m_pTargetLineVB = NULL;		/// TargetLine
	m_vecEventPoint.clear();
	m_bNewEventPoint = TRUE;	// 처음에는 새로운 이벤트 포인터 이다.
//	m_fDistanceToTarget = 0;
	m_nSelectedPoint = -1;		// 선택된 포인터 인덱스 -1 : 현재 선택 안됨.
	m_nFrame = 0;
	m_bInRange = FALSE;
	m_bUpVectorRender = FALSE;	// 렌더링시 UpVector 변화를 렌더링 할 것인가?
	m_bTarget = TRUE;
	m_bSelectedType = TRUE;
	m_bTargetRender = FALSE;

	m_nWeaponSpeed = 0;
	m_fWeaponAngle = 0.0f;

	m_bDefaultData = FALSE;
}

CCinema::~CCinema()
{
	SAFE_RELEASE( m_pTargetVertexVB );
	SAFE_RELEASE( m_pTargetLineVB );
	SAFE_RELEASE( m_pEventUpVectorVB );
	SAFE_RELEASE( m_pCinemaUpVectorVB );
	SAFE_RELEASE( m_pSelectedVB );
	SAFE_RELEASE( m_pEventVB );
	SAFE_RELEASE( m_pCinemaVB );
	m_vecEventPoint.clear();
}

void CCinema::InitData(char* i_pData)
{

	memcpy((char*)&m_header,i_pData,sizeof(CINEMA_HEADER));
	EVENT_POINT ep;
	for( int i = 0 ; i < m_header.nEventNum ; i++ )
	{
		memcpy((char*)&ep,i_pData+sizeof(CINEMA_HEADER)+i*sizeof(EVENT_POINT),sizeof(EVENT_POINT));
		InsertEventPoint(0,ep);
	}

}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CCinema::InitDefaultData()
/// \brief		디폴트 값
/// \author		ispark
/// \date		2006-03-06 ~ 2006-03-06
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CCinema::InitDefaultData()
{
	m_bDefaultData = TRUE;
}

BOOL CCinema::InsertEventPoint(int index, EVENT_POINT EventPoint)
{
	if (index == 0)
	{
		m_vecEventPoint.push_back(EventPoint);
		m_nNumOfEvent++;	
	}
	m_bRender = FALSE;
	// 만약 인덱스가 Size 보다 크면 에러
	return TRUE;
}

void CCinema::InitCinemaData(EVENT_POINT i_StartEventPoint, BOOL bLHMode)
{
	m_CurrentCinemaPoint = i_StartEventPoint;
/*
	float fAngle;	// 사이각
	D3DXVec3Normalize(&m_CurrentCinemaPoint.vDirection,&m_CurrentCinemaPoint.vDirection);

#ifdef _ATUM_CLIENT
	fAngle = ACOS(D3DXVec3Dot(&D3DXVECTOR3(1,0,0),&m_CurrentCinemaPoint.vDirection));
#else
	fAngle = acos(D3DXVec3Dot(&D3DXVECTOR3(1,0,0),&m_CurrentCinemaPoint.vDirection));
#endif // _ATUM_CLIENT_endif

	// 1. 내적을 이용해 두 벡터의 사이각을 구한다.
	D3DXVECTOR3 vAxis;	// 회전축
	D3DXVec3Cross(&vAxis, &D3DXVECTOR3(1,0,0), &m_CurrentCinemaPoint.vDirection);	// 2. 외적을 이용해 두 벡터의 회전축을 구한다.
	D3DXMATRIX matTrans;
	D3DXMatrixRotationAxis(&matTrans, &vAxis, fAngle);			// 3. 회전축에 대한 변환 행렬을 구한다.
	D3DXVECTOR3 vResult;		// 임시 결과

	vector<EVENT_POINT>::iterator iter = m_vecEventPoint.begin();

	while(iter != m_vecEventPoint.end())
	{
		D3DXVec3TransformCoord(&vResult, &iter->vPosition, &matTrans);
		iter->vPosition = m_CurrentCinemaPoint.vPosition + vResult;
		D3DXVec3TransformCoord(&iter->vDirection, &iter->vDirection, &matTrans);
		D3DXVec3TransformCoord(&iter->vTarget, &iter->vTarget, &matTrans);
		D3DXVec3TransformCoord(&iter->vUpVector, &iter->vUpVector, &matTrans);
		iter++;
	}	
*/
	D3DXMATRIX	mat, mat2;
	D3DXMatrixLookAtLH( &mat, &(m_CurrentCinemaPoint.vPosition), 
		&(m_CurrentCinemaPoint.vPosition + m_CurrentCinemaPoint.vDirection), &m_CurrentCinemaPoint.vUpVector);
	D3DXMatrixInverse( &mat, NULL, &mat );
	mat2 = mat;
	mat2._41 = 0;
	mat2._42 = 0;
	mat2._43 = 0;



	vector<EVENT_POINT>::iterator iter = m_vecEventPoint.begin();

	if(iter != m_vecEventPoint.end())
	{
		if(bLHMode)
		{
			while(iter != m_vecEventPoint.end())
			{
				D3DXVec3TransformCoord(&iter->vPosition, &iter->vPosition, &mat);
				D3DXVec3TransformCoord(&iter->vDirection, &iter->vDirection, &mat2);
				D3DXVec3TransformCoord(&iter->vTarget, &iter->vTarget, &mat);
				D3DXVec3TransformCoord(&iter->vUpVector, &iter->vUpVector, &mat2);
				iter++;
			}	
		}
		else
		{
			D3DXPLANE p;
			D3DXVECTOR3 vNormal;//( mat2._11, mat2._12, mat2._13 );

			// 평면의 방정식 구하기
			D3DXVec3Cross( &vNormal, &iter->vDirection, &iter->vUpVector);
			D3DXPlaneFromPointNormal( &p, &iter->vPosition, &vNormal );
			while(iter != m_vecEventPoint.end())
			{
				// 평면과 대칭점 구하기
				D3DXVECTOR3 v = iter->vPosition;
				float fT = - (p.a*v.x+p.b*v.y+p.c*v.z+p.d) / ( p.a*p.a + p.b*p.b + p.c*p.c );
				iter->vPosition.x = 2*fT*p.a + v.x;
				iter->vPosition.y = 2*fT*p.b + v.y;
				iter->vPosition.z = 2*fT*p.c + v.z;
				D3DXVec3TransformCoord(&iter->vPosition, &iter->vPosition, &mat);
				v = iter->vDirection;
				fT = - (p.a*v.x+p.b*v.y+p.c*v.z+p.d) / ( p.a*p.a + p.b*p.b + p.c*p.c );
				iter->vDirection.x = 2*fT*p.a + v.x;
				iter->vDirection.y = 2*fT*p.b + v.y;
				iter->vDirection.z = 2*fT*p.c + v.z;
				D3DXVec3TransformCoord(&iter->vDirection, &iter->vDirection, &mat2);
				v = iter->vTarget;
				fT = - (p.a*v.x+p.b*v.y+p.c*v.z+p.d) / ( p.a*p.a + p.b*p.b + p.c*p.c );
				iter->vTarget.x = 2*fT*p.a + v.x;
				iter->vTarget.y = 2*fT*p.b + v.y;
				iter->vTarget.z = 2*fT*p.c + v.z;
				D3DXVec3TransformCoord(&iter->vTarget, &iter->vTarget, &mat);
				v = iter->vUpVector;
				fT = - (p.a*v.x+p.b*v.y+p.c*v.z+p.d) / ( p.a*p.a + p.b*p.b + p.c*p.c );
				iter->vUpVector.x = 2*fT*p.a + v.x;
				iter->vUpVector.y = 2*fT*p.b + v.y;
				iter->vUpVector.z = 2*fT*p.c + v.z;
				D3DXVec3TransformCoord(&iter->vUpVector, &iter->vUpVector, &mat2);
				iter++;
			}	
		}
	}
}

EVENT_POINT	CCinema::GetCurrentCinemaPoint()
{
	return m_CurrentCinemaPoint;
}


#ifdef _ATUM_CLIENT

BOOL CCinema::Tick(D3DXVECTOR3 i_vTargetPosition)
{
	float fElapsedTime=g_pD3dApp->GetElapsedTime();	
	float fDistanceToStart;
	float fDistanceToTarget;

	vector<EVENT_POINT>::size_type size = m_vecEventPoint.size();	// 현재 벡터의 Size를 구한다.

	// 타켓이 없을때..
	if(i_vTargetPosition == D3DXVECTOR3(0,0,0))
	{
		m_bTarget = FALSE;		
	}

	// 패턴이 처음 적용될때..
	if (m_nTargetPointIndex == 0)
	{
		m_TargetEventPoint = m_vecEventPoint[m_nTargetPointIndex];		// 목표 점의 포인터를 받아온다.
		m_CurrentCinemaPoint = m_TargetEventPoint;
		m_nTargetPointIndex++;
		m_bNewEventPoint = TRUE;
//		m_fDistanceToTarget = 0.0f;
		m_nFrame = 0;
		return TRUE;
	}
	// 중간 패턴을 지날 때..
	else if(m_nTargetPointIndex > 0 && m_nTargetPointIndex < size)
	{
		m_TargetEventPoint = m_vecEventPoint[m_nTargetPointIndex];		// 목표 점의 포인터를 받아온다.
	}
	// 마지막 타켓 일때..
	else if (m_nTargetPointIndex == size)
	{
		m_TargetEventPoint.vPosition = i_vTargetPosition;
		m_TargetEventPoint.fVelocity = m_CurrentCinemaPoint.fVelocity;
	}
	// 타켓이 없을때..
	else if(m_nTargetPointIndex > size)
	{
		
	}

	// 목표 위치에서 현재 위치를 빼서 목표 방향을 설정한다.
	m_TargetEventPoint.vDirection = m_TargetEventPoint.vPosition - m_CurrentCinemaPoint.vPosition;	
	// 목표 방향을 단위 방향으로 만든다.
	D3DXVec3Normalize(&m_TargetEventPoint.vDirection,&m_TargetEventPoint.vDirection);	

	if(m_nTargetPointIndex < size)
	{
		m_TargetEventPoint.vDirection = m_TargetEventPoint.vDirection * 2;
	}
	// 차이 방향을 구한다.
	m_vDifferenceDirection =m_TargetEventPoint.vDirection -  m_CurrentCinemaPoint.vDirection ;

	if(m_nTargetPointIndex < size)
	{
	// 차이 방향을 단위 방향으로 만든다.
	D3DXVec3Normalize(&m_vDifferenceDirection,&m_vDifferenceDirection);		
	}
	
	// 새로운 이벤트 포인터라면 기본정보를 셋팅한다.
	if (m_bNewEventPoint)	
	{

		// 이번 영역에서 시작점의 데이터를 설정한다.(속력 계산을 위해..)
		m_StartEventPoint = m_CurrentCinemaPoint;		// 현재 CinemaPoint가 이번영역에서 StartPoint가 된다.

		/// 시작 속력이 타켓 속력 보다 크다면..
		if (m_StartEventPoint.fVelocity >= m_TargetEventPoint.fVelocity)
		{
			m_nMaxVelocity = m_StartEventPoint.fVelocity;
			m_nMinVelocity = m_TargetEventPoint.fVelocity;
		}
		else
		{
			m_nMinVelocity = m_StartEventPoint.fVelocity;
			m_nMaxVelocity = m_TargetEventPoint.fVelocity;
		}

		// 이번 영역에서 전체 거리를 구한다. ( 속력 계산을 위해..)
		m_fTotalDistance = D3DXVec3Length(&(m_TargetEventPoint.vPosition-m_StartEventPoint.vPosition));

		// 새로운 목표점과 현재 위치의 거리는 전체 거리와 같다.
//		m_fDistanceToTarget = m_fTotalDistance;
//		m_fDistance = 0;
		// 이제 더이상 새로운 EventPoint가 아니다.
		m_bNewEventPoint = FALSE;
		m_bInRange = FALSE;

	}

	// 시작 점 부터 현재 점 까지 거리
	fDistanceToStart = D3DXVec3Length(&(m_StartEventPoint.vPosition - m_CurrentCinemaPoint.vPosition));

	// 끝점 부터 현재 점 까지 거리
	fDistanceToTarget = D3DXVec3Length(&(m_TargetEventPoint.vPosition - m_CurrentCinemaPoint.vPosition));

	// 현재 속력을 구한다.
	m_CurrentCinemaPoint.fVelocity = ((m_StartEventPoint.fVelocity*fDistanceToTarget)+(m_TargetEventPoint.fVelocity*fDistanceToStart))/(m_fTotalDistance);
	// 구한 속력이 최소 속력보다 작다면 최소 속력으로
	if(m_CurrentCinemaPoint.fVelocity < m_nMinVelocity)
	{
		m_CurrentCinemaPoint.fVelocity = m_nMinVelocity;
	}
	// 구한 속력이 최대 속력보다 크다면 최대 속력으로
	if(m_CurrentCinemaPoint.fVelocity > m_nMaxVelocity)
	{
		m_CurrentCinemaPoint.fVelocity = m_nMaxVelocity;
	}

	// 현재 방향을 구한다.
	if (m_bTarget)
	{
		if(m_nTargetPointIndex < size)
		{
			m_CurrentCinemaPoint.vDirection = m_CurrentCinemaPoint.vDirection + 2*(m_vDifferenceDirection*fElapsedTime*m_StartEventPoint.fCurvature)*(m_CurrentCinemaPoint.fVelocity/m_fTotalDistance); // 회전율 적용

		}
		else if(m_nTargetPointIndex == size)
		{
			m_CurrentCinemaPoint.vDirection = m_CurrentCinemaPoint.vDirection + 2*(m_vDifferenceDirection*fElapsedTime*m_StartEventPoint.fCurvature); // 회전율 적용
		}
	}					
	D3DXVec3Normalize(&m_CurrentCinemaPoint.vDirection,&m_CurrentCinemaPoint.vDirection);	

	// 현재 위치를 구한다.
	m_CurrentCinemaPoint.vPosition = m_CurrentCinemaPoint.vPosition + m_CurrentCinemaPoint.vDirection*m_CurrentCinemaPoint.fVelocity*fElapsedTime;

	// 현재 타켓위치를 구한다.
	m_CurrentCinemaPoint.vTarget = (m_StartEventPoint.vTarget*fDistanceToTarget + m_TargetEventPoint.vTarget*fDistanceToStart)/(fDistanceToTarget+fDistanceToStart);

	// fDistance 는 지금의 시작 점 부터 현재 점까지 거리 
	// 현재 업벡터를 구한다.
//	m_CurrentCinemaPoint.vUpVector = m_CurrentCinemaPoint.vUpVector + m_vDifferenceUpVector*(fDistanceToStart-m_fDistance)/m_fTotalDistance;
	m_CurrentCinemaPoint.vUpVector = (m_StartEventPoint.vUpVector*fDistanceToTarget+m_TargetEventPoint.vUpVector*fDistanceToStart)/(fDistanceToTarget+fDistanceToStart);

	D3DXVec3Normalize(&m_CurrentCinemaPoint.vUpVector,&m_CurrentCinemaPoint.vUpVector);

//	m_fDistance = fDistanceToStart;
	
// 2004.06.29 jschoi 수정

	if(m_nTargetPointIndex < size)
	{
		float fDistanceToNext = m_CurrentCinemaPoint.fVelocity*fElapsedTime*5;
		//	DBGOUT("%.2f\n",fDistanceToNext);
		if (fDistanceToNext < 15)
			fDistanceToNext = 15;
		if(fDistanceToTarget < fDistanceToNext)
		{
			//		m_bInRange = TRUE;
			m_CurrentCinemaPoint.fVelocity = m_TargetEventPoint.fVelocity;
			m_CurrentCinemaPoint.fCurvature = m_TargetEventPoint.fCurvature;
			m_nTargetPointIndex++;
			m_bNewEventPoint = TRUE;
			//			m_fDistanceToTarget = 0.0f;	
		}
		
	}
	else if(m_nTargetPointIndex == size)
	{
		float fDistanceToNext = m_CurrentCinemaPoint.fVelocity*fElapsedTime*5;
		if (fDistanceToNext < 15)
			fDistanceToNext = 15;
		if(fDistanceToTarget < fDistanceToNext)
		{
			//		m_bInRange = TRUE;
			m_CurrentCinemaPoint.fVelocity = m_TargetEventPoint.fVelocity;
			m_CurrentCinemaPoint.fCurvature = m_TargetEventPoint.fCurvature;
			m_nTargetPointIndex++;
			m_bNewEventPoint = TRUE;
			//		m_fDistanceToTarget = 0.0f;
		}
		
	}

	if(m_bTarget == FALSE || m_nTargetPointIndex >= size)
	{
		return FALSE;
	}

	return TRUE;
}

BOOL CCinema::SkillTick()
{
	float fElapsedTime=g_pD3dApp->GetElapsedTime();	
	float fDistanceToStart;
	float fDistanceToTarget;

	vector<EVENT_POINT>::size_type size = m_vecEventPoint.size();	// 현재 벡터의 Size를 구한다.

	if (m_nTargetPointIndex >= size)
	{
		m_CurrentCinemaPoint.vPosition = 
			m_CurrentCinemaPoint.vPosition + m_CurrentCinemaPoint.vDirection*m_CurrentCinemaPoint.fVelocity*fElapsedTime;
		return TRUE;
	}

	m_TargetEventPoint = m_vecEventPoint[m_nTargetPointIndex];		// 목표 점의 포인터를 받아온다.

	if (m_nTargetPointIndex == 0)
	{
		m_CurrentCinemaPoint = m_TargetEventPoint;
		m_nTargetPointIndex++;
		m_bNewEventPoint = TRUE;
//		m_fDistanceToTarget = 0.0f;
		m_nFrame = 0;
		return FALSE;
	}

	// 목표 위치에서 현재 위치를 빼서 목표 방향을 설정한다.
	m_TargetEventPoint.vDirection = m_TargetEventPoint.vPosition - m_CurrentCinemaPoint.vPosition;	
	// 목표 방향을 단위 방향으로 만든다.
	D3DXVec3Normalize(&m_TargetEventPoint.vDirection,&m_TargetEventPoint.vDirection);	
	m_TargetEventPoint.vDirection = m_TargetEventPoint.vDirection * 2;
	// 차이 방향을 구한다.
	m_vDifferenceDirection =m_TargetEventPoint.vDirection -  m_CurrentCinemaPoint.vDirection ;
	// 차이 방향을 단위 방향으로 만든다.
	D3DXVec3Normalize(&m_vDifferenceDirection,&m_vDifferenceDirection);		
	

	if (m_bNewEventPoint)	// 새로운 이벤트 포인터라면 기본정보를 셋팅한다.
	{

		// 이번 영역에서 시작점의 데이터를 설정한다.(속력 계산을 위해..)
		m_StartEventPoint = m_CurrentCinemaPoint;		// 현재 CinemaPoint가 이번영역에서 StartPoint가 된다.

		// 차이 업벡터를 구한다.
		m_vDifferenceUpVector = m_TargetEventPoint.vUpVector - m_StartEventPoint.vUpVector;

		// 차이 타켓벡터를 구한다.
//		m_vDifferenceTarget = m_TargetEventPoint.vDirection - m_StartEventPoint.vDirection;

		/// 시작 속력이 타켓 속력 보다 크다면..
		if (m_StartEventPoint.fVelocity >= m_TargetEventPoint.fVelocity)
		{
			m_nMaxVelocity = m_StartEventPoint.fVelocity;
			m_nMinVelocity = m_TargetEventPoint.fVelocity;
		}
		else
		{
			m_nMinVelocity = m_StartEventPoint.fVelocity;
			m_nMaxVelocity = m_TargetEventPoint.fVelocity;
		}

		// 이번 영역에서 전체 거리를 구한다. ( 속력 계산을 위해..)
		m_fTotalDistance = D3DXVec3Length(&(m_TargetEventPoint.vPosition-m_StartEventPoint.vPosition));

		// 새로운 목표점과 현재 위치의 거리는 전체 거리와 같다.
//		m_fDistanceToTarget = m_fTotalDistance;
//		m_fDistance = 0;
		// 이제 더이상 새로운 EventPoint가 아니다.
		m_bNewEventPoint = FALSE;
		m_bInRange = FALSE;

	}

	// 시작 점 부터 현재 점 까지 거리
	fDistanceToStart = D3DXVec3Length(&(m_StartEventPoint.vPosition - m_CurrentCinemaPoint.vPosition));

	// 끝점 부터 현재 점 까지 거리
	fDistanceToTarget = D3DXVec3Length(&(m_TargetEventPoint.vPosition - m_CurrentCinemaPoint.vPosition));

	// 현재 속력을 구한다.
	m_CurrentCinemaPoint.fVelocity = ((m_StartEventPoint.fVelocity*fDistanceToTarget)+(m_TargetEventPoint.fVelocity*fDistanceToStart))/(m_fTotalDistance);
	// 구한 속력이 최소 속력보다 작다면 최소 속력으로
	if(m_CurrentCinemaPoint.fVelocity < m_nMinVelocity)
	{
		m_CurrentCinemaPoint.fVelocity = m_nMinVelocity;
	}
	// 구한 속력이 최대 속력보다 크다면 최대 속력으로
	if(m_CurrentCinemaPoint.fVelocity > m_nMaxVelocity)
	{
		m_CurrentCinemaPoint.fVelocity = m_nMaxVelocity;
	}

	// 현재 방향을 구한다.
	m_CurrentCinemaPoint.vDirection = m_CurrentCinemaPoint.vDirection + 2*(m_vDifferenceDirection*fElapsedTime*m_StartEventPoint.fCurvature*m_CurrentCinemaPoint.fVelocity/m_fTotalDistance); // 회전율 적용

	// 현재 방향을 단위 방향으로 				
	D3DXVec3Normalize(&m_CurrentCinemaPoint.vDirection,&m_CurrentCinemaPoint.vDirection);	

	// 현재 위치를 구한다.
	m_CurrentCinemaPoint.vPosition = m_CurrentCinemaPoint.vPosition + m_CurrentCinemaPoint.vDirection*m_CurrentCinemaPoint.fVelocity*fElapsedTime;

	// 현재 타켓위치를 구한다.
	m_CurrentCinemaPoint.vTarget = (m_StartEventPoint.vTarget*fDistanceToTarget + m_TargetEventPoint.vTarget*fDistanceToStart)/(fDistanceToTarget+fDistanceToStart);
//	m_CurrentCinemaPoint.vTarget = m_StartEventPoint.vTarget;

	// 현재 업벡터를 구한다.
//	m_CurrentCinemaPoint.vUpVector = m_CurrentCinemaPoint.vUpVector + m_vDifferenceUpVector*(fDistanceToStart-m_fDistance)/m_fTotalDistance;
	m_CurrentCinemaPoint.vUpVector = (m_StartEventPoint.vUpVector*fDistanceToTarget+m_TargetEventPoint.vUpVector*fDistanceToStart)/(fDistanceToTarget+fDistanceToStart);

	// 현재 UpVector를 단위 방향으로 만든다.
	D3DXVec3Normalize(&m_CurrentCinemaPoint.vUpVector,&m_CurrentCinemaPoint.vUpVector);	

//	m_fDistance = fDistanceToStart;

// 2004.06.29 jschoi 수정


	if(m_nTargetPointIndex < size)
	{
		float fDistanceToNext = m_CurrentCinemaPoint.fVelocity*fElapsedTime*5;
		//	DBGOUT("%.2f\n",fDistanceToNext);
		if (fDistanceToNext < 15)
			fDistanceToNext = 15;
		if(fDistanceToTarget < fDistanceToNext)
		{
			//		m_bInRange = TRUE;
			m_CurrentCinemaPoint.fVelocity = m_TargetEventPoint.fVelocity;
			m_CurrentCinemaPoint.fCurvature = m_TargetEventPoint.fCurvature;
			m_nTargetPointIndex++;
			m_bNewEventPoint = TRUE;
			//			m_fDistanceToTarget = 0.0f;	
		}
		
	}
	else if(m_nTargetPointIndex == size)
	{
		float fDistanceToNext = m_CurrentCinemaPoint.fVelocity*fElapsedTime*5;
		if (fDistanceToNext < 15)
			fDistanceToNext = 15;
		if(fDistanceToTarget < 15)
		{
			//		m_bInRange = TRUE;
			m_CurrentCinemaPoint.fVelocity = m_TargetEventPoint.fVelocity;
			m_CurrentCinemaPoint.fCurvature = m_TargetEventPoint.fCurvature;
			m_nTargetPointIndex++;
			m_bNewEventPoint = TRUE;
			//		m_fDistanceToTarget = 0.0f;
		}
		
	}

	return FALSE;
}

#endif	// _ATUM_CLIENT_endif 클라이언트에서만 사용되는 함수..


#ifndef _ATUM_CLIENT	// 여기부터 시네마 에디터에서만 쓰는 함수들..

BOOL CCinema::Tick()
{
	float fElapsedTime=g_AppFormView->GetElapsedTime();	
	float fDistanceToStart;
	float fDistanceToTarget;

	vector<EVENT_POINT>::size_type size = m_vecEventPoint.size();	// 현재 벡터의 Size를 구한다.

	if (size<2)
		return FALSE;

	if (m_nTargetPointIndex >= size)
		return FALSE;

	m_TargetEventPoint = m_vecEventPoint[m_nTargetPointIndex];		// 목표 점의 포인터를 받아온다.

	if (m_nTargetPointIndex == 0)
	{
		m_CurrentCinemaPoint = m_TargetEventPoint;
		m_nTargetPointIndex++;
		m_bNewEventPoint = TRUE;
//		m_fDistanceToTarget = 0.0f;
		m_nFrame = 0;
		return FALSE;
	}

	// 목표 위치에서 현재 위치를 빼서 목표 방향을 설정한다.
	m_TargetEventPoint.vDirection = m_TargetEventPoint.vPosition - m_CurrentCinemaPoint.vPosition;	
	// 목표 방향을 단위 방향으로 만든다.
	D3DXVec3Normalize(&m_TargetEventPoint.vDirection,&m_TargetEventPoint.vDirection);	
	m_TargetEventPoint.vDirection = m_TargetEventPoint.vDirection * 2;
	// 차이 방향을 구한다.
	m_vDifferenceDirection =m_TargetEventPoint.vDirection -  m_CurrentCinemaPoint.vDirection ;
	// 차이 방향을 단위 방향으로 만든다.
	D3DXVec3Normalize(&m_vDifferenceDirection,&m_vDifferenceDirection);		
	

	if (m_bNewEventPoint)	// 새로운 이벤트 포인터라면 기본정보를 셋팅한다.
	{
		// 이번 영역에서 시작점의 데이터를 설정한다.(속력 계산을 위해..)
		m_StartEventPoint = m_CurrentCinemaPoint;		// 현재 CinemaPoint가 이번영역에서 StartPoint가 된다.

		// 차이 업벡터를 구한다.
		m_vDifferenceUpVector = m_TargetEventPoint.vUpVector - m_StartEventPoint.vUpVector;

		// 차이 타켓벡터를 구한다.
//		m_vDifferenceTarget = m_TargetEventPoint.vDirection - m_StartEventPoint.vDirection;

		/// 시작 속력이 타켓 속력 보다 크다면..
		if (m_StartEventPoint.fVelocity >= m_TargetEventPoint.fVelocity)
		{
			m_nMaxVelocity = m_StartEventPoint.fVelocity;
			m_nMinVelocity = m_TargetEventPoint.fVelocity;
		}
		else
		{
			m_nMinVelocity = m_StartEventPoint.fVelocity;
			m_nMaxVelocity = m_TargetEventPoint.fVelocity;
		}

		// 이번 영역에서 전체 거리를 구한다. ( 속력 계산을 위해..)
		m_fTotalDistance = D3DXVec3Length(&(m_TargetEventPoint.vPosition-m_StartEventPoint.vPosition));

		// 새로운 목표점과 현재 위치의 거리는 전체 거리와 같다.
//		m_fDistanceToTarget = m_fTotalDistance;
//		m_fDistance = 0;
		// 이제 더이상 새로운 EventPoint가 아니다.
		m_bNewEventPoint = FALSE;
		m_bInRange = FALSE;

	}

	// 시작 점 부터 현재 점 까지 거리
	fDistanceToStart = D3DXVec3Length(&(m_StartEventPoint.vPosition - m_CurrentCinemaPoint.vPosition));

	// 끝점 부터 현재 점 까지 거리
	fDistanceToTarget = D3DXVec3Length(&(m_TargetEventPoint.vPosition - m_CurrentCinemaPoint.vPosition));

	// 현재 속력을 구한다.
	m_CurrentCinemaPoint.fVelocity = ((m_StartEventPoint.fVelocity*fDistanceToTarget)+(m_TargetEventPoint.fVelocity*fDistanceToStart))/(m_fTotalDistance);
	// 구한 속력이 최소 속력보다 작다면 최소 속력으로
	if(m_CurrentCinemaPoint.fVelocity < m_nMinVelocity)
	{
		m_CurrentCinemaPoint.fVelocity = m_nMinVelocity;
	}
	// 구한 속력이 최대 속력보다 크다면 최대 속력으로
	if(m_CurrentCinemaPoint.fVelocity > m_nMaxVelocity)
	{
		m_CurrentCinemaPoint.fVelocity = m_nMaxVelocity;
	}

	// 현재 방향을 구한다.
	m_CurrentCinemaPoint.vDirection = m_CurrentCinemaPoint.vDirection + 2*(m_vDifferenceDirection*fElapsedTime*m_StartEventPoint.fCurvature*m_CurrentCinemaPoint.fVelocity/m_fTotalDistance); // 회전율 적용

	// 현재 방향을 단위 방향으로 				
	D3DXVec3Normalize(&m_CurrentCinemaPoint.vDirection,&m_CurrentCinemaPoint.vDirection);	

	// 현재 위치를 구한다.
	m_CurrentCinemaPoint.vPosition = m_CurrentCinemaPoint.vPosition + m_CurrentCinemaPoint.vDirection*m_CurrentCinemaPoint.fVelocity*fElapsedTime;

	// 현재 타켓위치를 구한다.
	m_CurrentCinemaPoint.vTarget = (m_StartEventPoint.vTarget*fDistanceToTarget + m_TargetEventPoint.vTarget*fDistanceToStart)/(fDistanceToTarget+fDistanceToStart);

//	DBGOUT("%.2f, %.2f, %.2f \n",m_CurrentCinemaPoint.vTarget.x,m_CurrentCinemaPoint.vTarget.y,m_CurrentCinemaPoint.vTarget.z);

	// 현재 업벡터를 구한다.
	m_CurrentCinemaPoint.vUpVector = (m_StartEventPoint.vUpVector*fDistanceToTarget+m_TargetEventPoint.vUpVector*fDistanceToStart)/(fDistanceToTarget+fDistanceToStart);
//	m_CurrentCinemaPoint.vUpVector = m_CurrentCinemaPoint.vUpVector + m_vDifferenceUpVector*(fDistanceToStart-m_fDistance)/m_fTotalDistance;

	// 현재 UpVector를 단위 방향으로 만든다.
	D3DXVec3Normalize(&m_CurrentCinemaPoint.vUpVector,&m_CurrentCinemaPoint.vUpVector);	

//	m_fDistance = fDistanceToStart;

	XYZ_DIFUSE_VERTEX* v;

	m_pCinemaVB->Lock( 0, 0, (BYTE**)&v, 0  );

	if(m_nNumOfCinema>=99999)
		return FALSE;
	v[m_nNumOfCinema].p = m_CurrentCinemaPoint.vPosition;
	v[m_nNumOfCinema].c = 0xffdddddd;

	m_pCinemaVB->Unlock();

	// 시네마 업벡터의 점 버퍼를 채운다.
	m_pCinemaUpVectorVB->Lock( 0, 0, (BYTE**)&v, 0  );
	D3DXVECTOR3 vTemp;

	if(m_nNumOfCinema>=99999)
		return FALSE;
	v[m_nNumOfCinema*2].p = m_CurrentCinemaPoint.vPosition;
	v[m_nNumOfCinema*2].c = 0xffbbbbbb;
	vTemp = m_CurrentCinemaPoint.vPosition + m_CurrentCinemaPoint.vUpVector*50;

	v[m_nNumOfCinema*2+1].p = vTemp;
	v[m_nNumOfCinema*2+1].c = 0xff5555aa;

	m_pCinemaVB->Unlock();
	m_nNumOfCinema++;

// 2004.06.29 jschoi 수정


	float fDistanceToNext = m_CurrentCinemaPoint.fVelocity*fElapsedTime*5;
	if (fDistanceToNext < 15)
		fDistanceToNext = 15;
	if(fDistanceToTarget < fDistanceToNext)
	{
		m_bInRange = TRUE;
		m_CurrentCinemaPoint.fVelocity = m_TargetEventPoint.fVelocity;
		m_CurrentCinemaPoint.fCurvature = m_TargetEventPoint.fCurvature;
		m_nTargetPointIndex++;
		m_bNewEventPoint = TRUE;
//		m_fDistanceToTarget = 0.0f;
		m_nFrame = 0;
		return FALSE;
	}
	else
	{
//		m_fDistanceToTarget = fDistanceToTarget;
		m_nFrame++;
	}
	return TRUE;
/*
	// 방금 전의 끝점까지 거리가 지금 구한 끝점까지 거리보다 크면 그 점을 지나친거다. 
	// 그리고 거리 15 안쪽으로 한번이라도 들어 왔어야 한다.
	if(fDistanceToTarget > m_fDistanceToTarget && m_bInRange)
	{
		m_CurrentCinemaPoint.fVelocity = m_TargetEventPoint.fVelocity;
		m_CurrentCinemaPoint.fCurvature = m_TargetEventPoint.fCurvature;
		m_nTargetPointIndex++;
		m_bNewEventPoint = TRUE;
		m_fDistanceToTarget = 0.0f;
		m_nFrame = 0;
		return FALSE;
	}
*/
}


HRESULT CCinema::Render()
{
	float fCinemaPointSize = 5.0f;
	float fEventPointSize = 12.0f;
	float fSelectedPointSize = 14.0f;
	float fTargetPointSize = 10.0f;

	// 이벤트 포인트 점 버퍼
	g_pD3dDev->SetRenderState( D3DRS_POINTSIZE,*((DWORD*)&(fEventPointSize)));
	g_pD3dDev->SetStreamSource( 0, m_pEventVB,0, sizeof(XYZ_DIFUSE_VERTEX) );
	g_pD3dDev->DrawPrimitive( D3DPT_POINTLIST , 0,m_nNumOfEvent);

	if(m_bUpVectorRender)
	{
		// 이벤트 포인트의 UpVector 라인
		g_pD3dDev->SetStreamSource( 0, m_pEventUpVectorVB,0, sizeof(XYZ_DIFUSE_VERTEX) );
		g_pD3dDev->DrawPrimitive( D3DPT_LINELIST , 0,m_nNumOfEvent);
	}

	if(m_bTargetRender)
	{
		// 타켓 포인트의 점 버퍼
		g_pD3dDev->SetRenderState( D3DRS_POINTSIZE,*((DWORD*)&(fTargetPointSize)));
		g_pD3dDev->SetStreamSource( 0, m_pTargetVertexVB,0, sizeof(XYZ_DIFUSE_VERTEX) );
		g_pD3dDev->DrawPrimitive( D3DPT_POINTLIST , 0,m_nNumOfEvent);

		// 타켓 라인의 렌더링
		g_pD3dDev->SetStreamSource( 0, m_pTargetLineVB,0, sizeof(XYZ_DIFUSE_VERTEX) );
		g_pD3dDev->DrawPrimitive( D3DPT_LINELIST , 0,m_nNumOfEvent);
	}

	if(m_nSelectedPoint != -1)
	{
		// 선택된 포인트 점 버퍼
		g_pD3dDev->SetRenderState( D3DRS_POINTSIZE,*((DWORD*)&(fSelectedPointSize)));
		g_pD3dDev->SetStreamSource( 0, m_pSelectedVB,0, sizeof(XYZ_DIFUSE_VERTEX) );
		g_pD3dDev->DrawPrimitive( D3DPT_POINTLIST , 0,1);
	}

	if(m_bRender)
	{
		// 시네마 포인트 점 버퍼
		g_pD3dDev->SetRenderState( D3DRS_POINTSIZE,*((DWORD*)&fCinemaPointSize));
		g_pD3dDev->SetStreamSource( 0, m_pCinemaVB,0, sizeof(XYZ_DIFUSE_VERTEX) );
//		g_pD3dDev->DrawPrimitive( D3DPT_POINTLIST , 0,m_nNumOfCinema);
		g_pD3dDev->DrawPrimitive( D3DPT_LINESTRIP , 0,m_nNumOfCinema-1);

//		if(m_bUpVectorRender)
//		{
			// UpVector 점 버퍼 라인 렌더링
//			g_pD3dDev->SetStreamSource( 0, m_pCinemaUpVectorVB, sizeof(XYZ_DIFUSE_VERTEX) );
//			g_pD3dDev->DrawPrimitive( D3DPT_LINELIST , 0,m_nNumOfCinema);
//		}

	}

	return S_OK;
}


BOOL CCinema::Reset()
{
	m_bRender = TRUE;
	m_CurrentCinemaPoint.vPosition = D3DXVECTOR3(0,0,0);
	m_CurrentCinemaPoint.vDirection = D3DXVECTOR3(1,0,0);
	m_CurrentCinemaPoint.vUpVector = D3DXVECTOR3(0,1,0);
	m_CurrentCinemaPoint.vTarget = D3DXVECTOR3(0,0,0);
	m_CurrentCinemaPoint.fVelocity = 100.0f;
	m_CurrentCinemaPoint.fCurvature = 1.0f;
	m_nNumOfCinema = 0;
	m_bNewEventPoint = TRUE;	// 처음에는 새로운 이벤트 포인터 이다.
	m_nTargetPointIndex= 0;		// 타켓 점의 번호
	m_nFrame = 0;
	return TRUE;
}


BOOL CCinema::RemoveEventPoint(int index)
{
	vector<EVENT_POINT>::size_type size = m_vecEventPoint.size();	// 현재 벡터의 Size를 구한다.
	// 만약 인덱스가 Size 보다 크면 에러
	if (index == 0 && m_nSelectedPoint != -1 && size>0)
	{
		vector<EVENT_POINT>::iterator iter = &m_vecEventPoint[m_nSelectedPoint];
		m_vecEventPoint.erase(iter);
		m_nSelectedPoint = -1;
		m_nNumOfEvent--;
		UpdatePoint();

	}
	return TRUE;
}

HRESULT CCinema::InitDeviceObjects()
{

	return S_OK;
}

HRESULT CCinema::RestoreDeviceObjects()
{

	CreateVB();

	return S_OK;
}

HRESULT CCinema::InvalidateDeviceObjects()
{

	SAFE_RELEASE( m_pTargetVertexVB );
	SAFE_RELEASE( m_pTargetLineVB );
	SAFE_RELEASE( m_pEventUpVectorVB );
	SAFE_RELEASE( m_pCinemaUpVectorVB );
	SAFE_RELEASE( m_pSelectedVB );
	SAFE_RELEASE( m_pEventVB );
	SAFE_RELEASE( m_pCinemaVB );

	return S_OK;
}

HRESULT CCinema::DeleteDeviceObjects()
{

	return S_OK;
}

BOOL CCinema::SaveEventPoints()
{
	return TRUE;
}

BOOL CCinema::LoadEventPoints()
{
	return TRUE;
}

BOOL CCinema::SaveCameraPoints()
{
	return TRUE;
}

BOOL CCinema::LoadCameraPoints()
{
	return TRUE;
}

BOOL CCinema::UpdatePoint()
{
	XYZ_DIFUSE_VERTEX* v;

	// 선택된 Point 재 설정
	if(m_nSelectedPoint != -1)
	{		
		m_pSelectedVB->Lock( 0, 0, (BYTE**)&v, 0  );
		v[0].p = m_vecEventPoint[m_nSelectedPoint].vPosition;
		v[0].c = 0xffff0000;
		m_pSelectedVB->Unlock();
	}

	// Event Point 재 설정
	vector<EVENT_POINT>::iterator iter = m_vecEventPoint.begin();
	int i = 0;
	m_pEventVB->Lock( 0, 0, (BYTE**)&v, 0  );
	while (iter != m_vecEventPoint.end())
	{
		v[i].p = iter->vPosition;
		v[i].c = 0xff00ff00;
		iter++;
		i++;
	}
	m_pEventVB->Unlock();

	/// UpVector 재 설정 
	iter = m_vecEventPoint.begin();
	i = 0;
	m_pEventUpVectorVB->Lock( 0, 0, (BYTE**)&v, 0  );
	while (iter != m_vecEventPoint.end())
	{
		v[i*2].p = iter->vPosition;
		v[i*2].c = 0xffffff00;
		v[i*2+1].p = iter->vPosition + iter->vUpVector*100;
		v[i*2+1].c = 0xffffff00;
		iter++;
		i++;
	}
	m_pEventUpVectorVB->Unlock();

	// Target Point 재 설정
	iter = m_vecEventPoint.begin();
	i = 0;
	m_pTargetVertexVB->Lock( 0, 0, (BYTE**)&v, 0  );
	while (iter != m_vecEventPoint.end())
	{
		v[i].p = iter->vTarget;
		v[i].c = 0xff0055ff;
		iter++;
		i++;
	}
	m_pTargetVertexVB->Unlock();

	/// Target Line 재 설정 
	iter = m_vecEventPoint.begin();
	i = 0;
	m_pTargetLineVB->Lock( 0, 0, (BYTE**)&v, 0  );
	while (iter != m_vecEventPoint.end())
	{
		v[i*2].p = iter->vPosition;
		v[i*2].c = 0xffff1111;
		v[i*2+1].p = iter->vTarget;
		v[i*2+1].c = 0xffff1111;
		iter++;
		i++;
	}
	m_pTargetLineVB->Unlock();


	m_bRender = FALSE;


	return TRUE;
}


HRESULT CCinema::CreateVB()
{
	//Cinema Point 점 버퍼 생성
    if( FAILED( g_pD3dDev->CreateVertexBuffer( 100000*sizeof( XYZ_DIFUSE_VERTEX ),
		D3DUSAGE_WRITEONLY, D3DFVF_XYZ_DIFUSE_VERTEX, D3DPOOL_MANAGED, &m_pCinemaVB,NULL ) ) )
        return E_FAIL;

	//Event Point 점 버퍼 생성
    if( FAILED( g_pD3dDev->CreateVertexBuffer( 100*sizeof( XYZ_DIFUSE_VERTEX ),
		D3DUSAGE_WRITEONLY, D3DFVF_XYZ_DIFUSE_VERTEX, D3DPOOL_MANAGED, &m_pEventVB,NULL ) ) )
        return E_FAIL;

	//UpVector 점 버퍼 생성 ( CinemaPoint*2 개수)
    if( FAILED( g_pD3dDev->CreateVertexBuffer( 200000*sizeof( XYZ_DIFUSE_VERTEX ),
		D3DUSAGE_WRITEONLY, D3DFVF_XYZ_DIFUSE_VERTEX, D3DPOOL_MANAGED, &m_pCinemaUpVectorVB,NULL ) ) )
        return E_FAIL;

	//Event UpVector 점 버퍼 생성 ( EventPoint*2 개수)
    if( FAILED( g_pD3dDev->CreateVertexBuffer( 200*sizeof( XYZ_DIFUSE_VERTEX ),
		D3DUSAGE_WRITEONLY, D3DFVF_XYZ_DIFUSE_VERTEX, D3DPOOL_MANAGED, &m_pEventUpVectorVB,NULL ) ) )
        return E_FAIL;

	//Selected Point 점 버퍼 생성
    if( FAILED( g_pD3dDev->CreateVertexBuffer( 1*sizeof( XYZ_DIFUSE_VERTEX ),
		D3DUSAGE_WRITEONLY, D3DFVF_XYZ_DIFUSE_VERTEX, D3DPOOL_MANAGED, &m_pSelectedVB,NULL ) ) )
        return E_FAIL;

	//Target Point 점 버퍼 생성
    if( FAILED( g_pD3dDev->CreateVertexBuffer( 100*sizeof( XYZ_DIFUSE_VERTEX ),
		D3DUSAGE_WRITEONLY, D3DFVF_XYZ_DIFUSE_VERTEX, D3DPOOL_MANAGED, &m_pTargetVertexVB ,NULL) ) )
        return E_FAIL;

	//TargetLine Point 점 버퍼 생성
    if( FAILED( g_pD3dDev->CreateVertexBuffer( 200*sizeof( XYZ_DIFUSE_VERTEX ),
		D3DUSAGE_WRITEONLY, D3DFVF_XYZ_DIFUSE_VERTEX, D3DPOOL_MANAGED, &m_pTargetLineVB,NULL ) ) )
        return E_FAIL;

	return S_OK;
}

int CCinema::GetTargetIndex()
{
	return m_nTargetPointIndex;
}

BOOL CCinema::LengthPoint(D3DXVECTOR3 vEyes, D3DXVECTOR3 vDir, D3DXVECTOR3 vPosition)
{
	/// 선택 범위 Size
	float fTestSize = 100.0f;

	// 계산용 임시 변수
	float fTempLen,u,w;
	// 선분의 기준 점에서 점까지의 벡터
	/// 눈에서 계산할 점의으로의 방향 벡터
	D3DXVECTOR3 vPl;
	vPl = vPosition - vEyes;
	// 기준 점에서 점까지의 벡터와 방향 벡터의 내적
	u = D3DXVec3Dot(&vDir, &vPl);
	// 점의 방향이 방향 벡터와 90도 이상인 경우에는 제외
//	if(u<0.001f)
//		return FALSE;
	// 기준 점에서 점까지의 벡터의 크기 계산
	w = D3DXVec3Length(&vPl);
	// 거리 계산
	fTempLen = sqrt(w*w-u*u);
	// 선택 영역에 포함되는지 검사한다.

	if(fTempLen < fTestSize)					/// fTestSize 검색 범위
	{
		/// 현재 최소 거리인지 확인 한다.
		if(fTempLen < m_fMinimumLength)
		{
			m_fMinimumLength = fTempLen;			/// 최소 거리 저장
			return TRUE;						
		}
	}
	return FALSE;
}

int CCinema::PickUpPoint(CPoint i_pointMouse, DWORD i_dwViewType, float i_fZoom)
{

	float fShortDistance = 100.0f;	// 기본 최단 거리
	float fDistance;
	int nResult = -1;			// 반환값 초기화 

	// 점 버퍼를 연다
//	XYZ_DIFUSE_VERTEX* v;

//	m_pEventVB->Lock( 0, 0, (BYTE**)&v, 0  );

	CPoint pointPixel;

	/// 버퍼의 모든 점들을 2D 좌표로 환산한 뒤 입력으로 받은 마우스 포인터와 거리를 계산하여 가장 가까운 거리에 있는 점을 리턴한다.
	for( int i = 0; i < m_nNumOfEvent ; i++)
	{
		pointPixel = GetPixelFromPosition(m_vecEventPoint[i].vPosition,i_dwViewType,i_fZoom);
		fDistance = (float)sqrt((pointPixel.x - i_pointMouse.x)*(pointPixel.x - i_pointMouse.x)+(pointPixel.y - i_pointMouse.y)*(pointPixel.y - i_pointMouse.y));
		if(fDistance < fShortDistance)
		{
			nResult = i;
			m_bSelectedType = TRUE;
			fShortDistance = fDistance;
		}
		if(m_bTargetRender)
		{
			pointPixel = GetPixelFromPosition(m_vecEventPoint[i].vTarget,i_dwViewType,i_fZoom);
			fDistance = (float)sqrt((pointPixel.x - i_pointMouse.x)*(pointPixel.x - i_pointMouse.x)+(pointPixel.y - i_pointMouse.y)*(pointPixel.y - i_pointMouse.y));
			if(fDistance < fShortDistance)
			{
				nResult = i;
				m_bSelectedType = FALSE;
				fShortDistance = fDistance;
			}
		}		
	}
	// 버퍼를 닫는다.
//	m_pEventVB->Unlock();

	if(fShortDistance < 10)
	{
		m_nSelectedPoint = nResult;
		UpdatePoint();
	}
	else
	{
		m_nSelectedPoint = -1;
	}
	return -1;
}


CPoint CCinema::GetPixelFromPosition(D3DXVECTOR3 i_vPosition, DWORD i_dwViewType, float i_fZoom)
{
	CPoint pointPixel;
	switch(i_dwViewType)
	{
	case TOPVIEW:	/// TOPVIEW
		{
			pointPixel.x = (WINDOW_WIDTH/2)+(i_vPosition.x*10)/i_fZoom;
			pointPixel.y = (WINDOW_HEIGHT/2)-(i_vPosition.z*7.5)/i_fZoom;
		}
		break;
	case SIDEVIEW: /// SIDEVIEW
		{
			pointPixel.x = (WINDOW_WIDTH/2)+(i_vPosition.z*10)/i_fZoom;
			pointPixel.y = (WINDOW_HEIGHT/2)-(i_vPosition.y*7.5)/i_fZoom;
		}
		break;
	case FRONTVIEW: /// FRONTVIEW
		{
			pointPixel.x = (WINDOW_WIDTH/2)+(i_vPosition.x*10)/i_fZoom;
			pointPixel.y = (WINDOW_HEIGHT/2)-(i_vPosition.y*7.5)/i_fZoom;
		}
		break;
	}
	return pointPixel;
}

void CCinema::MovePoint(CPoint i_pointMouseMove, DWORD i_dwViewType, float i_fZoom)		/// 선택된 이벤트 포인터 이동
{
	if(m_nSelectedPoint == -1)
		return;
	if(m_bSelectedType)
	{
		switch(i_dwViewType)
		{
		case TOPVIEW: /// TOPVIEW
			{
				m_vecEventPoint[m_nSelectedPoint].vPosition.x -= (i_pointMouseMove.x*i_fZoom)/10;
				m_vecEventPoint[m_nSelectedPoint].vPosition.z += (i_pointMouseMove.y*1.3333*i_fZoom)/10;
			}
			break;
		case SIDEVIEW: /// SIDEVIEW
			{
				m_vecEventPoint[m_nSelectedPoint].vPosition.z -= (i_pointMouseMove.x*i_fZoom)/10;
				m_vecEventPoint[m_nSelectedPoint].vPosition.y += (i_pointMouseMove.y*1.3333*i_fZoom)/10;
			}
			break;
		case FRONTVIEW: /// FRONTVIEW
			{
				m_vecEventPoint[m_nSelectedPoint].vPosition.x -= (i_pointMouseMove.x*i_fZoom)/10;
				m_vecEventPoint[m_nSelectedPoint].vPosition.y += (i_pointMouseMove.y*1.3333*i_fZoom)/10;
			}
			break;
		}
	}
	else
	{
		switch(i_dwViewType)
		{
		case TOPVIEW: /// TOPVIEW
			{
				m_vecEventPoint[m_nSelectedPoint].vTarget.x -= (i_pointMouseMove.x*i_fZoom)/10;
				m_vecEventPoint[m_nSelectedPoint].vTarget.z += (i_pointMouseMove.y*1.3333*i_fZoom)/10;
			}
			break;
		case SIDEVIEW: /// SIDEVIEW
			{
				m_vecEventPoint[m_nSelectedPoint].vTarget.z -= (i_pointMouseMove.x*i_fZoom)/10;
				m_vecEventPoint[m_nSelectedPoint].vTarget.y += (i_pointMouseMove.y*1.3333*i_fZoom)/10;
			}
			break;
		case FRONTVIEW: /// FRONTVIEW
			{
				m_vecEventPoint[m_nSelectedPoint].vTarget.x -= (i_pointMouseMove.x*i_fZoom)/10;
				m_vecEventPoint[m_nSelectedPoint].vTarget.y += (i_pointMouseMove.y*1.3333*i_fZoom)/10;
			}
			break;
		}

	}
	UpdatePoint();	
}

void CCinema::ChangeUpVector(CPoint i_pointMouseMove,DWORD i_dwViewType, float i_fZoom)
{
	if(m_nSelectedPoint == -1)
		return ; /// 선택된 포인트가 없으면 그냥 리턴
	if(!m_bUpVectorRender)
		return ; /// Up Vector 이 렌더링중이 아니면 그냥 리턴
	switch(i_dwViewType)
	{
	case TOPVIEW:	/// TOPVIEW
		{
			m_vecEventPoint[m_nSelectedPoint].vUpVector.x -= (i_pointMouseMove.x*i_fZoom)/1000;
			m_vecEventPoint[m_nSelectedPoint].vUpVector.z += (i_pointMouseMove.y*1.3333*i_fZoom)/1000;			
		}
		break;
	case SIDEVIEW: /// SIDEVIEW
		{
			m_vecEventPoint[m_nSelectedPoint].vUpVector.z -= (i_pointMouseMove.x*i_fZoom)/1000;
			m_vecEventPoint[m_nSelectedPoint].vUpVector.y += (i_pointMouseMove.y*1.3333*i_fZoom)/1000;
		}
		break;
	case FRONTVIEW: /// FRONTVIEW
		{
			m_vecEventPoint[m_nSelectedPoint].vUpVector.x -= (i_pointMouseMove.x*i_fZoom)/1000;
			m_vecEventPoint[m_nSelectedPoint].vUpVector.y += (i_pointMouseMove.y*1.3333*i_fZoom)/1000;
		}
		break;
	}
	D3DXVec3Normalize(&m_vecEventPoint[m_nSelectedPoint].vUpVector,&m_vecEventPoint[m_nSelectedPoint].vUpVector);
	UpdatePoint();
}

void CCinema::Save(char* strName)
{
	HANDLE hFile = NULL;
	DWORD dwWritten;
	hFile = CreateFile(strName, GENERIC_WRITE,0,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	vector<EVENT_POINT>::size_type size = m_vecEventPoint.size();
	vector<EVENT_POINT>::iterator iter = m_vecEventPoint.begin();

	CINEMA_HEADER header;

	header.nEventNum = (int)size;

	// 타켓을 렌더링 하고 있다면 타켓방향이 있는 패턴이다.
	if(m_bTargetRender)
	{
		header.nPatternType = 1;	// 타켓방향이 있는 패턴
	}
	else
	{
		header.nPatternType = 2;	// 타켓방향이 없는 패턴
	}

	ZeroMemory(&header.strDescription,sizeof(header.strDescription));

	WriteFile(hFile,(char*)&header,sizeof(header),&dwWritten,NULL);

	EVENT_POINT ep;
	while(iter != m_vecEventPoint.end())
	{
		ep.vPosition = iter->vPosition;
		ep.vDirection = iter->vDirection;
		ep.vUpVector = iter->vUpVector;
		ep.fVelocity = iter->fVelocity;
		ep.fCurvature = iter->fCurvature;
		ep.vTarget = iter->vTarget;
		WriteFile(hFile,(char*)&ep,sizeof(EVENT_POINT),&dwWritten,NULL);
		iter++;
	}
	CloseHandle(hFile);
}

void CCinema::Open(char* strName)
{
	HANDLE hFile = NULL;
	DWORD dwRead;
	hFile = CreateFile(strName, GENERIC_READ,0,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);

	ReadFile(hFile,(char*)&m_header,sizeof(CINEMA_HEADER),&dwRead,NULL);

	EVENT_POINT ep;
	m_vecEventPoint.clear();
	for(int i = 0 ; i < m_header.nEventNum ; i++)
	{
		ReadFile(hFile,(char*)&ep,sizeof(EVENT_POINT),&dwRead,NULL);
		InsertEventPoint(0, ep);
	}
	CloseHandle(hFile);

	if(m_header.nPatternType == 1)
	{
		m_bTargetRender = TRUE;
	}
	else if(m_header.nPatternType == 2)
	{
		m_bTargetRender = FALSE;
	}
	m_nNumOfEvent = i;
	UpdatePoint();
	m_nSelectedPoint = -1;
	m_bRender = FALSE;

}

int	CCinema::GetSelectedPoint()
{
	return m_nSelectedPoint;
}

void CCinema::UpVectorRender()
{
	m_bUpVectorRender = !m_bUpVectorRender;
}


BOOL CCinema::IsRender()
{
	return m_bRender;
}


void CCinema::TargetRender()
{
	m_bTargetRender = !m_bTargetRender;
}

BOOL CCinema::IsTargetRender()
{
	return m_bTargetRender;
}

void CCinema::Next()
{
	vector<EVENT_POINT>::size_type size = m_vecEventPoint.size();

	if(m_vecEventPoint.empty())
		return ;

	if(m_nSelectedPoint == -1)
	{
		m_nSelectedPoint = 0;
	}
	else if((m_nSelectedPoint+1) < size)
	{
		m_nSelectedPoint++;
	}
	else if((m_nSelectedPoint+1) == size)
	{
		m_nSelectedPoint = 0;
	}
}

#endif // _ATUM_CLIENT_endif 여기까지 시네마 에디터에서만 쓰는 함수들..



///////////////////////////////////////////////////////////////////////////////
/// \fn			CCinema::WeaponTick(D3DXVECTOR3 i_vTargetPosition, float fElapsedTime)
/// \brief		2형 무기 시네마 틱
/// \author		ispark
/// \date		2005-12-02 ~ 2005-12-02
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
// 2010. 03. 18 by ckPark 인피니티 필드 2차(몬스터 스킬 추가)
//BOOL CCinema::WeaponTick(D3DXVECTOR3 i_vTargetPosition, float fElapsedTime)
BOOL CCinema::WeaponTick( D3DXVECTOR3 i_vTargetPosition, float fElapsedTime, float fDecSpeed /* = 0.0f */ )
// end 2010. 03. 18 by ckPark 인피니티 필드 2차(몬스터 스킬 추가)
{
	float fDistanceToStart;
	float fDistanceToTarget;

	vector<EVENT_POINT>::size_type size = m_vecEventPoint.size();	// 현재 벡터의 Size를 구한다.

	// 타켓이 없을때..
	if(i_vTargetPosition == D3DXVECTOR3(0,0,0))
	{
		m_bTarget = FALSE;		
	}

	// 패턴이 처음 적용될때..
	if(m_bDefaultData == FALSE && m_nTargetPointIndex == 0)
	{
		m_TargetEventPoint = m_vecEventPoint[m_nTargetPointIndex];		// 목표 점의 포인터를 받아온다.
		m_CurrentCinemaPoint = m_TargetEventPoint;
		m_nTargetPointIndex++;
		m_bNewEventPoint = TRUE;
//		m_fDistanceToTarget = 0.0f;
		m_nFrame = 0;
		return TRUE;
	}
	// 중간 패턴을 지날 때..
	else if(m_nTargetPointIndex > 0 && m_nTargetPointIndex < size)
	{
		m_TargetEventPoint = m_vecEventPoint[m_nTargetPointIndex];		// 목표 점의 포인터를 받아온다.
	}
	// 마지막 타켓 일때..
	else if (m_nTargetPointIndex == size)
	{
		m_TargetEventPoint.vPosition = i_vTargetPosition;
		m_TargetEventPoint.fVelocity = m_CurrentCinemaPoint.fVelocity = m_nWeaponSpeed;
	}
	// 타켓이 없을때..
	else if(m_nTargetPointIndex > size)
	{
		
	}

	if(m_bDefaultData == TRUE && m_nTargetPointIndex == 0)
	{
		m_TargetEventPoint.vPosition = i_vTargetPosition;
		m_TargetEventPoint.fVelocity = m_CurrentCinemaPoint.fVelocity = m_nWeaponSpeed;
	}
	
	// 목표 위치에서 현재 위치를 빼서 목표 방향을 설정한다.
	m_TargetEventPoint.vDirection = m_TargetEventPoint.vPosition - m_CurrentCinemaPoint.vPosition;	
	// 목표 방향을 단위 방향으로 만든다.
	D3DXVec3Normalize(&m_TargetEventPoint.vDirection,&m_TargetEventPoint.vDirection);	

	if(m_nTargetPointIndex < size)
	{
		m_TargetEventPoint.vDirection = m_TargetEventPoint.vDirection * 2;
	}
	// 차이 방향을 구한다.
	m_vDifferenceDirection = m_TargetEventPoint.vDirection - m_CurrentCinemaPoint.vDirection ;

	if(m_nTargetPointIndex < size)
	{
	// 차이 방향을 단위 방향으로 만든다.
	D3DXVec3Normalize(&m_vDifferenceDirection,&m_vDifferenceDirection);		
	}
	
	// 새로운 이벤트 포인터라면 기본정보를 셋팅한다.
	if(m_bNewEventPoint)	
	{

		// 이번 영역에서 시작점의 데이터를 설정한다.(속력 계산을 위해..)
		m_StartEventPoint = m_CurrentCinemaPoint;		// 현재 CinemaPoint가 이번영역에서 StartPoint가 된다.

		/// 시작 속력이 타켓 속력 보다 크다면..
		if (m_StartEventPoint.fVelocity >= m_TargetEventPoint.fVelocity)
		{
			m_nMaxVelocity = m_StartEventPoint.fVelocity;
			m_nMinVelocity = m_TargetEventPoint.fVelocity;
		}
		else
		{
			m_nMinVelocity = m_StartEventPoint.fVelocity;
			m_nMaxVelocity = m_TargetEventPoint.fVelocity;
		}

		// 이번 영역에서 전체 거리를 구한다. ( 속력 계산을 위해..)
		m_fTotalDistance = D3DXVec3Length(&(m_TargetEventPoint.vPosition-m_StartEventPoint.vPosition));

		// 새로운 목표점과 현재 위치의 거리는 전체 거리와 같다.
//		m_fDistanceToTarget = m_fTotalDistance;
//		m_fDistance = 0;
		// 이제 더이상 새로운 EventPoint가 아니다.
		m_bNewEventPoint = FALSE;
		m_bInRange = FALSE;

	}

	// 시작 점 부터 현재 점 까지 거리
	fDistanceToStart = D3DXVec3Length(&(m_StartEventPoint.vPosition - m_CurrentCinemaPoint.vPosition));

	// 끝점 부터 현재 점 까지 거리
	fDistanceToTarget = D3DXVec3Length(&(m_TargetEventPoint.vPosition - m_CurrentCinemaPoint.vPosition));

	// 현재 속력을 구한다.
	m_CurrentCinemaPoint.fVelocity = ((m_StartEventPoint.fVelocity*fDistanceToTarget)+(m_TargetEventPoint.fVelocity*fDistanceToStart))/(m_fTotalDistance);
	// 구한 속력이 최소 속력보다 작다면 최소 속력으로
	if(m_CurrentCinemaPoint.fVelocity < m_nMinVelocity)
	{
		m_CurrentCinemaPoint.fVelocity = m_nMinVelocity;
	}
	// 구한 속력이 최대 속력보다 크다면 최대 속력으로
	if(m_CurrentCinemaPoint.fVelocity > m_nMaxVelocity)
	{
		m_CurrentCinemaPoint.fVelocity = m_nMaxVelocity;
	}

	// 2010. 03. 18 by ckPark 인피니티 필드 2차(몬스터 스킬 추가)
	float fCurEPVelocity, fTarEPVelocity;
	fCurEPVelocity = m_CurrentCinemaPoint.fVelocity;
	fTarEPVelocity = m_TargetEventPoint.fVelocity;
	if( fDecSpeed != 0.0f )
	{
		float fDecreasedSpeed = m_CurrentCinemaPoint.fVelocity - fDecSpeed;
		if( fDecreasedSpeed < 0.0f )
			fDecreasedSpeed = 0.0f;

		fCurEPVelocity = fTarEPVelocity = fDecreasedSpeed;
	}
	// end 2010. 03. 18 by ckPark 인피니티 필드 2차(몬스터 스킬 추가)

	// 현재 방향을 구한다.
	if(m_bTarget)
	{
		if(m_nTargetPointIndex < size)
		{
			m_CurrentCinemaPoint.vDirection = m_CurrentCinemaPoint.vDirection + 2*(m_vDifferenceDirection*fElapsedTime*m_StartEventPoint.fCurvature)*(m_CurrentCinemaPoint.fVelocity/m_fTotalDistance); // 회전율 적용
		}
		else if(m_nTargetPointIndex == size)
		{
			// 패턴이 아닌 쫓아가는 공식
			float dot;
			double dRadian = 0;
			D3DXVECTOR3 vCrossVec;

			D3DXVec3Cross(&vCrossVec, &m_TargetEventPoint.vDirection, &m_CurrentCinemaPoint.vDirection);
			dot = D3DXVec3Dot(&m_CurrentCinemaPoint.vDirection, &m_TargetEventPoint.vDirection);
			float fElapsedWeaponAngle = m_fWeaponAngle * fElapsedTime;
			if(D3DXToRadian(fElapsedWeaponAngle) < ACOS(dot))
			{
				dot = cos(D3DXToRadian(fElapsedWeaponAngle));
			}
			if(1.0f > dot)
			{
				dRadian = ACOS(dot);
			}
//			DBGOUT("%f(%f), Time = %f\n", D3DXToDegree(ACOS(dot)), m_fWeaponAngle, fElapsedTime);
//			DBGOUT("%f, Time = %f\n", dRadian, fElapsedTime);
//			DBGOUT("%f, Time = %f\n", D3DXToDegree(dRadian), fElapsedTime);
			D3DXQUATERNION qOrg(m_CurrentCinemaPoint.vDirection.x, m_CurrentCinemaPoint.vDirection.y, m_CurrentCinemaPoint.vDirection.z, 0.0f);
			D3DXQUATERNION qRot, qRotConj;
			D3DXQuaternionRotationAxis(&qRot, &vCrossVec, (float)dRadian);
			D3DXQuaternionConjugate(&qRotConj, &qRot);
			D3DXQUATERNION qRst = qRot * qOrg * qRotConj;

			m_CurrentCinemaPoint.vDirection.x = qRst.x;
			m_CurrentCinemaPoint.vDirection.y = qRst.y;
			m_CurrentCinemaPoint.vDirection.z = qRst.z;
		}
	}

	D3DXVec3Normalize(&m_CurrentCinemaPoint.vDirection,&m_CurrentCinemaPoint.vDirection);	

	// 현재 위치를 구한다.
	// 2010. 03. 18 by ckPark 인피니티 필드 2차(몬스터 스킬 추가)
// 	if(m_nTargetPointIndex < size)
// 	{
// 		m_CurrentCinemaPoint.vPosition = m_CurrentCinemaPoint.vPosition + m_CurrentCinemaPoint.vDirection*m_CurrentCinemaPoint.fVelocity*fElapsedTime;
// 	}
// 	else if(m_nTargetPointIndex == size)
// 	{
// 		m_CurrentCinemaPoint.vPosition = m_CurrentCinemaPoint.vPosition + m_CurrentCinemaPoint.vDirection*m_nWeaponSpeed*fElapsedTime;
// 	}

	if(m_nTargetPointIndex < size)
	{
		m_CurrentCinemaPoint.vPosition = m_CurrentCinemaPoint.vPosition
										 + m_CurrentCinemaPoint.vDirection * fCurEPVelocity * fElapsedTime;
	}
	else if(m_nTargetPointIndex == size)
	{
		float fWeaponSpeed = (float)m_nWeaponSpeed - fDecSpeed;
		if( fWeaponSpeed < 0.0f )
			fWeaponSpeed = 0.0f;
		m_CurrentCinemaPoint.vPosition = m_CurrentCinemaPoint.vPosition
										 + m_CurrentCinemaPoint.vDirection * fWeaponSpeed * fElapsedTime;
	}
	// end 2010. 03. 18 by ckPark 인피니티 필드 2차(몬스터 스킬 추가)

	// 현재 타켓위치를 구한다.
	m_CurrentCinemaPoint.vTarget = (m_StartEventPoint.vTarget*fDistanceToTarget + m_TargetEventPoint.vTarget*fDistanceToStart)/(fDistanceToTarget+fDistanceToStart);

	// fDistance 는 지금의 시작 점 부터 현재 점까지 거리 
	// 현재 업벡터를 구한다.
//	m_CurrentCinemaPoint.vUpVector = m_CurrentCinemaPoint.vUpVector + m_vDifferenceUpVector*(fDistanceToStart-m_fDistance)/m_fTotalDistance;
	m_CurrentCinemaPoint.vUpVector = (m_StartEventPoint.vUpVector*fDistanceToTarget+m_TargetEventPoint.vUpVector*fDistanceToStart)/(fDistanceToTarget+fDistanceToStart);

	D3DXVec3Normalize(&m_CurrentCinemaPoint.vUpVector,&m_CurrentCinemaPoint.vUpVector);

//	m_fDistance = fDistanceToStart;
	
// 2004.06.29 jschoi 수정

	if(m_nTargetPointIndex < size)
	{
		float fDistanceToNext = m_CurrentCinemaPoint.fVelocity*fElapsedTime*5;
		//	DBGOUT("%.2f\n",fDistanceToNext);
		if (fDistanceToNext < 15)
			fDistanceToNext = 15;
		if(fDistanceToTarget < fDistanceToNext)
		{
			//		m_bInRange = TRUE;
			m_CurrentCinemaPoint.fVelocity = m_TargetEventPoint.fVelocity;
			m_CurrentCinemaPoint.fCurvature = m_TargetEventPoint.fCurvature;
			m_nTargetPointIndex++;
			m_bNewEventPoint = TRUE;
			//			m_fDistanceToTarget = 0.0f;	
		}
		
	}
	else if(m_nTargetPointIndex == size)
	{
//		float fDistanceToNext = m_CurrentCinemaPoint.fVelocity*fElapsedTime*5;
//		if (fDistanceToNext < 15)
//			fDistanceToNext = 15;
//		if(fDistanceToTarget < fDistanceToNext)
//		{
//			//		m_bInRange = TRUE;
//			m_CurrentCinemaPoint.fVelocity = m_TargetEventPoint.fVelocity;
//			m_CurrentCinemaPoint.fCurvature = m_TargetEventPoint.fCurvature;
//			m_nTargetPointIndex++;
//			m_bNewEventPoint = TRUE;
//			//		m_fDistanceToTarget = 0.0f;
//		}
	}

	if(m_bTarget == FALSE || m_nTargetPointIndex >= size)
	{
		return FALSE;
	}

	return TRUE;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			CCinema::InitWeaponCinemaData(EVENT_POINT i_StartEventPoint, BOOL bLHMode, int Speed, float Angle)
/// \brief		무기관련 초기 셋팅
/// \author		ispark
/// \date		2005-12-02 ~ 2005-12-02
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CCinema::InitWeaponCinemaData(EVENT_POINT i_StartEventPoint, BOOL bLHMode, int Speed, float Angle)
{
	m_nWeaponSpeed = Speed;
	m_fWeaponAngle = Angle;

	InitCinemaData(i_StartEventPoint, bLHMode);
}
