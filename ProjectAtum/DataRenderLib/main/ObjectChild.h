// ObjectChild.h: interface for the CObjectChild class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OBJECTCHILD_H__66461ECB_74A2_47DA_BD3A_578ED7698888__INCLUDED_)
#define AFX_OBJECTCHILD_H__66461ECB_74A2_47DA_BD3A_578ED7698888__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AtumNode.h"

class CSkinnedMesh;
class CCharacterInfo;
class CObjRender;
class CChatMoveData;
class CObjectChild : public CAtumNode  
{
public:
	CObjectChild(OBJECTINFOCLIENT objInfo, OBJECTSCALEINFO ScaleInfo);	// 2012-05-02 by isshin 맵툴 개선 오브젝트 스케일 정보 - OBJECTSCALEINFO 추가
	virtual ~CObjectChild();
	virtual void Render();//int min_x,int max_x,int min_z,int max_z);
	virtual void ObjectNameRender();
	virtual void Tick();
	
	
	// 2013-05-07 by bhsohn 세력포인트 개선안 시스템
	BOOL IsShowNode();
	void SetShowNode(BOOL bShowNode);
	// END 2013-05-07 by bhsohn 세력포인트 개선안 시스템

	void CheckShadowPos();
	void ChangeBodycondition( BodyCond_t bodycon );
	void ChangeBodyconditionEvent(BodyCond_t bodycon);

//	float GetObjectSize(int type);

//	FLOAT			m_fMaxTime;

//	FLOAT			m_fShadowSizeWidth;		// 그림자 가로 크기
//	FLOAT			m_fShadowSizeHeight;	// 그림자 세로 크기
	D3DXVECTOR3			m_vPos;					// 렌더링 상의 좌표
	D3DXVECTOR3			m_vUp;
	D3DXVECTOR3			m_vVel;
	D3DXVECTOR3			m_vScale;				// 2012-05-02 by isshin 맵툴 개선 오브젝트 스케일 정보
	FLOAT				m_fCurrentTime;
	D3DXMATRIX			m_mMatrix;
	D3DXMATRIX			m_mScaleMatrix;			// 2012-05-02 by isshin 맵툴 개선 오브젝트 스케일 정보
	INT					m_nObjScreenX;			// 스크린상 X 좌표
	INT					m_nObjScreenY;			// 스크린상 Y 좌표
	INT					m_nObjScreenW;			// 스크린상 W 좌표
	BOOL				m_bCheckBlend;
	D3DXVECTOR3			m_vShadowPos1;			// 그림자 위치 (앞부분 왼쪽)
	D3DXVECTOR3			m_vShadowPos2;			// 그림자 위치 (앞부분 오른쪽)
	D3DXVECTOR3			m_vShadowPos3;			// 그림자 위치 (뒷부분 오른쪽)
	D3DXVECTOR3			m_vShadowPos4;			// 그림자 위치 (뒷부분 왼쪽)
	FLOAT				m_fDistanceCamera;		// 카메라로부터의 거리
	D3DXVECTOR3		m_vOriPos;
	INT				m_nCode;
	BOOL			m_bCheckAttack;
	BOOL			m_bCheckColl;
	BOOL			m_bCheckAni;
	MAPOBJECTINFO *	m_pObjectInfo;
	CObjRender *	m_pRenderer;
	INT				m_nObjectCont;			// 클라이언트에서 관리할 오브젝트 인덱스
	BOOL			m_bCheckRenderDist;
	POINT			m_ptBlockPos;

	DWORD			m_dwObjectMonsterUniqueNumber;		// Object Monster 가 지정한 몬스터 유니크넘버
	BYTE			m_bObjectTexIndex;
	UINT			m_nNextEventIndex;		// 이벤트를 종료하기 위해 검색하는 다음인덱스
//	BYTE			m_bBodyCondition;
	BOOL			m_bIsEvent;
	BYTE			m_bEventType;
	short			m_sEventIndexFrom;		// 이벤트 오브젝트의 실제 인덱스
	short			m_sEventIndexTo;
	short			m_sEventIndex3;
	char			m_strEventParam[40];
	CChatMoveData	*		m_pMoveChat;
	BOOL			m_bEnableObjectMonsterObject;		// 만약 오브젝트 몬스터라면, 오브젝트 몬스터 오브젝트의 활성화 여부

	BodyCond_t		m_BodyCondition;					// 상태, bit flag 사용
	CCharacterInfo*		m_pCharacterInfo;
	void LoadCharacterEffect(char* strFileName);
//	LPDIRECT3DVERTEXBUFFER8 m_pVBShadow;			// Shadow VB
	CSkinnedMesh*		m_pObjMesh;
	// 2013-05-07 by bhsohn 세력포인트 개선안 시스템
	BOOL			m_bShowNode;
	// END 2013-05-07 by bhsohn 세력포인트 개선안 시스템

};

#endif // !defined(AFX_OBJECTCHILD_H__66461ECB_74A2_47DA_BD3A_578ED7698888__INCLUDED_)
