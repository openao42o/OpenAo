// Cinema.h: interface for the CCinema class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CINEMA_H__DA0207CA_981F_45C0_867F_3F05E38B6EFA__INCLUDED_)
#define AFX_CINEMA_H__DA0207CA_981F_45C0_867F_3F05E38B6EFA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef _ATUM_CLIENT

#include <vector>
#include "dbgout_c.h"

#endif // _ATUM_CLIENT_endif 시네마에디터에서만 사용함

typedef struct
{
	D3DXVECTOR3		vPosition;			/// 위치
	D3DXVECTOR3		vDirection;			/// 방향
	D3DXVECTOR3		vUpVector;			/// UpVector
	D3DXVECTOR3		vTarget;			/// 유닛의 Target
	FLOAT			fVelocity;			/// 속력
	FLOAT			fCurvature;			/// 곡률
} EVENT_POINT;


// 헤더 크기 128바이트
struct CINEMA_HEADER
{
	int			nEventNum;
	int			nPatternType;
	char		strDescription[120];
};

class CD3DFont;
class CCinema
{
public:

	CCinema();
	virtual ~CCinema();
	void		InitDefaultData();

	// 2010. 03. 18 by ckPark 인피니티 필드 2차(몬스터 스킬 추가)
	//BOOL		WeaponTick(D3DXVECTOR3 i_vTargetPosition, float fElapsedTime);
	BOOL		WeaponTick( D3DXVECTOR3 i_vTargetPosition, float fElapsedTime, float fDecSpeed = 0.0f );
	// end 2010. 03. 18 by ckPark 인피니티 필드 2차(몬스터 스킬 추가)

	BOOL		Tick(D3DXVECTOR3 i_vTargetPosition);
	BOOL		SkillTick();
	void		InitData(char* i_pData);	// cin 파일 셋팅
	BOOL		InsertEventPoint(int index, EVENT_POINT EventPoint);	/// index 위치에 EventPoint를 삽입한다.
																		/// index 값에 0 을 입력하면 Default로 맨마지막 원소로 삽입된다.
	void		InitCinemaData(EVENT_POINT i_StartEventPoint, BOOL bLHMode=TRUE);
	void		InitWeaponCinemaData(EVENT_POINT i_StartEventPoint, BOOL bLHMode, int Speed, float Angle);
	EVENT_POINT	GetCurrentCinemaPoint();
	std::vector<EVENT_POINT>	m_vecEventPoint;	/// EventPoint 데이터 벡터 리스트
	CINEMA_HEADER	GetHeader() { return m_header;}

	// 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템
	// 시네마 패턴이 끝났는가
	BOOL		IsCinemaOver( void )
	{
		vector<EVENT_POINT>::size_type size = m_vecEventPoint.size();	// 현재 벡터의 Size를 구한다.

		return (m_nTargetPointIndex >= size);
	}

	// 현재 시네마패턴의 속도를 설정한다
	void		SetCurrentCinemaPointVelocity( float fVelocity )
	{
		m_CurrentCinemaPoint.fVelocity	= fVelocity;
	}
	// end 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템

private:

	EVENT_POINT			m_CurrentCinemaPoint;		/// 현재 위치, 방향, 속력
	EVENT_POINT			m_CurrentCameraPoint;		/// 현재 카메라 
	EVENT_POINT			m_StartEventPoint;			/// 지금 위치의 시작점 (속력 연산을 위해 필요)
	EVENT_POINT			m_TargetEventPoint;			/// 지금 위치의 목표점 
	int					m_nTargetPointIndex;		/// 목표점 번호

	BOOL				m_bNewEventPoint;			/// 새로운 이벤트 포인터 인가
	D3DXVECTOR3			m_vDifferenceDirection;		/// 시작 방향과 타켓 방향의 차이 방향
	D3DXVECTOR3			m_vDifferenceUpVector;		/// 차이 UpVector(업벡터의 차이 방향)
	D3DXVECTOR3			m_vDifferenceTarget;		/// 유닛 타켓 방향의 차이 방향
	float				m_fTotalDistance;			/// 이번 영역의 전체 거리 (StartPoint 와 TargetPoint 의 거리)
//	float				m_fDistanceToTarget;		/// 방금 전 이동시 Target 과의 거리
//	float				m_fTatalTargetDistance;		/// 이번 영역에서 타켓의 전처 거리

	BOOL				m_bInRange;					/// 목표 근처에 도달했는가?
	int					m_nMaxVelocity;				/// 최대 속력
	int					m_nMinVelocity;				/// 최소 속력
	BOOL				m_bTarget;					/// 타켓이 있는가?
	float				m_fDistance;				/// 거리


	int					m_nNumOfCinema;				/// CinemaPoint개수
	int					m_nNumOfEvent;				/// EventPoint개수
	LPDIRECT3DVERTEXBUFFER9		m_pCinemaVB;		/// CinemaVertex
	LPDIRECT3DVERTEXBUFFER9		m_pEventVB;			/// EventVertex
	LPDIRECT3DVERTEXBUFFER9		m_pSelectedVB;		/// SelectedVertex
	LPDIRECT3DVERTEXBUFFER9		m_pCinemaUpVectorVB;/// CinemaUpVectorVertex
	LPDIRECT3DVERTEXBUFFER9		m_pEventUpVectorVB;	/// EventUpVectorVertex
	LPDIRECT3DVERTEXBUFFER9		m_pTargetVertexVB;	/// TargetVertex
	LPDIRECT3DVERTEXBUFFER9		m_pTargetLineVB;	/// TargetLine
	float				m_fMinimumLength;			/// LengthPoint에서 사용되는 내부 변수 (최소 거리)
	int					m_nSelectedPoint;			/// 선택된 포인터 인덱스
	BOOL				m_bRender;					/// 그릴까?
	int					m_nFrame;					/// 최소프레임 계산을 위해서..
	BOOL				m_bUpVectorRender;			/// UpVector렌더링 할까?
	BOOL				m_bSelectedType;			/// 선택된 점이 점인가?
	BOOL				m_bTargetRender;			/// Target 을 렌더링 할것인가?
	CINEMA_HEADER		m_header;					/// header

	// 2005-12-02 by ispark, 무기 관련
	int					m_nWeaponSpeed;				// 스피트
	float				m_fWeaponAngle;				// 선회력

	BOOL				m_bDefaultData;				// 디폴트 

#ifndef _ATUM_CLIENT
public:
	BOOL		Tick();	
	HRESULT		Render();
	BOOL		Reset();					/// Cinema Point 를 다시 구하고 다시 그린다.
	BOOL		RemoveEventPoint(int index);							/// index 위치의 EventPoint를 삭제한다.
	HRESULT		InitDeviceObjects();
	HRESULT		RestoreDeviceObjects();
	HRESULT		InvalidateDeviceObjects();
	HRESULT		DeleteDeviceObjects();
	BOOL		SaveEventPoints();
	BOOL		LoadEventPoints();
	BOOL		SaveCameraPoints();
	BOOL		LoadCameraPoints();
	BOOL		LengthPoint(D3DXVECTOR3 vEyes, D3DXVECTOR3 vDir, D3DXVECTOR3 vPosition);
	CPoint		GetPixelFromPosition(D3DXVECTOR3 i_vPosition, DWORD i_dwViewType, float i_fZoom);
	int			PickUpPoint(CPoint i_pointMouse, DWORD i_dwViewType, float i_fZoom);		/// 마우스로 3D 공간상에 클릭한 점을 리턴한다.
	BOOL		UpdatePoint();					/// 변화가 이루어지면 Update한다.
	HRESULT		CreateVB();						/// m_pCinemaVB 와 m_pEventVB에 점버퍼 활당/생성 
	void		MovePoint(CPoint i_pointMouseMove, DWORD i_dwViewType, float i_fZoom);		/// 선택된 이벤트 포인터 이동
	void		Save(char* strName);
	void		Open(char* strName);
	int			GetSelectedPoint();
	void		UpVectorRender();					/// UpVector를 렌더링 할 것인가?
	void		ChangeUpVector(CPoint i_pointMouseMove,DWORD i_dwViewType, float i_fZoom);	/// 선택된 이벤트 포인트의 UpVector 변경
	int			GetTargetIndex();
	BOOL		IsRender();
	void		TargetRender();
	BOOL		IsTargetRender();					/// 지금 타켓 벡터를 렌더링하고 있는가?
	void		Next();
private:


#endif // _ATUM_CLIENT_endif 시네마에디터에서만 사용함


};

#endif // !defined(AFX_CINEMA_H__DA0207CA_981F_45C0_867F_3F05E38B6EFA__INCLUDED_)

