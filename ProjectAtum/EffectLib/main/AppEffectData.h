#pragma once

// AppEffectData.h: interface for the CAppEffectData class.
//////////////////////////////////////////////////////////////////////

#include "AtumNode.h"
#include "CharacterInfo.h"
// 2009. 11. 23 by jskim 리소스 로딩 구조 변경
#include "AtumDefine.h"
//end 2009. 11. 23 by jskim 리소스 로딩 구조 변경

class CCamera;
class CCharacterInfo;

class CAppEffectData : public CAtumNode  
{
public:
	// 2009. 11. 23 by jskim 리소스 로딩 구조 변경
	//CAppEffectData(CAtumNode * pParent,int nType,D3DXVECTOR3 vPos, int nWeaponPositionIndex = 0);
	//CAppEffectData(CCamera * pParent,int nType,float fDist = 0, float fSide = 0,float fUp = 0);
	//CAppEffectData(int nType,D3DXVECTOR3 vPos, float fVelRate,D3DXVECTOR3 vVel);
	//CAppEffectData(int nType,D3DXVECTOR3 vPos);
	//CAppEffectData(GUIUnitRenderInfo * pParent,int nType);
	//ysw 9_22
	//CAppEffectData(CAtumNode * pParent2, CCamera * pParent, int nType, D3DXVECTOR3 vPos,float fDist = 0, float fSide = 0,float fUp = 0);
//
 	CAppEffectData(CAtumNode * pParent,int nType,D3DXVECTOR3 vPos, int nWeaponPositionIndex = 0, int LoadingPriority =_NOTHING_STEP);
 	CAppEffectData(CCamera * pParent,int nType,float fDist = 0, float fSide = 0,float fUp = 0, int LoadingPriority =_NOTHING_STEP);
 	CAppEffectData(int nType,D3DXVECTOR3 vPos, float fVelRate,D3DXVECTOR3 vVel, int LoadingPriority =_NOTHING_STEP);
 	CAppEffectData(int nType,D3DXVECTOR3 vPos, int LoadingPriority =_NOTHING_STEP);
 	CAppEffectData(GUIUnitRenderInfo * pParent,int nType);
// 	//ysw 9_22
 	CAppEffectData(CAtumNode * pParent2, CCamera * pParent, int nType, D3DXVECTOR3 vPos,float fDist = 0, float fSide = 0,float fUp = 0, int LoadingPriority =_NOTHING_STEP);
//end 2009. 11. 23 by jskim 리소스 로딩 구조 변경

	virtual ~CAppEffectData();
	virtual void Tick();
	void SetHeight(); 
	void SetEffectTick(float fTick);
	BOOL CheckItemPoint();
	void ChangeBodyCondition(BodyCond_t hyBody);
	void ResetBodyCondition(BodyCond_t hyBody);

	void LoadCharacterEffect(char* strFileName);
	void LoadCharacterEffect(unsigned FileNum);
	
	// 2010-06-15 by shcho&hslee 펫시스템 - 펫 무기 생성
	// 2010-06-15 by shcho&hslee 펫시스템 - 마을 이동 처리
	void SetPetPos(D3DXVECTOR3 CharPos,D3DXVECTOR3 PetPos, float fElapsedTime);
	// end 2010-06-15 by shcho&hslee 펫시스템 - 마을 이동 처리
	// end 2010-06-15 by shcho&hslee 펫시스템 - 펫 무기 생성

	// 2008-12-29 by bhsohn 럭키 머신 성공 이펙트 추가
	void SetAffDataMaxtrix(D3DXVECTOR3	i_vPos, D3DXVECTOR3	i_vVel, D3DXVECTOR3	i_vUp);

	DWORD				m_dwState;
	CCamera		*		m_pCamera;
	BodyCond_t			m_bodyCondition;
	CCharacterInfo*		m_pCharacterInfo;
	D3DXVECTOR3			m_vPos;						// Position
	D3DXVECTOR3			m_vVel;						// 방향
	D3DXVECTOR3			m_vUp;						// Position
	D3DXVECTOR3			m_vTargetPos;						// Position
	D3DXMATRIX			m_mMatrix;
	INT					m_nType;
	FLOAT				m_fDistance;
	FLOAT				m_fSideDistance;
	FLOAT				m_fUpDistance;
	FLOAT				m_fVelRate;

	GUIUnitRenderInfo * m_pGUIInfo;

	// 2009. 07. 07 by ckPark 로봇기어 요청사항(롤링, 선택화면, 무기, A기어포대)
	BOOL				m_bRender;

	enum	RobotWeapon1_1_FirePos{ NONE, LEFT, RIGHT };

	RobotWeapon1_1_FirePos	m_nFirePos;
	// end 2009. 07. 07 by ckPark 로봇기어 요청사항(롤링, 선택화면, 무기, A기어포대)


	// 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템

	BOOL				m_bRepeat;			// 반복 시키는 이펙트인가?
	float				m_fPlayTime;		// 총 재생 시간
	EffectPlayCount_t	m_nPlayCount;		// 총 반복재생 횟수

	// 반복 재생 여부 셋팅
	void	SetRepeat( BOOL bRepeat, float	fPlayTime = 0, EffectPlayCount_t nPlayCount = 0 )
	{
		m_bRepeat = bRepeat;
		
		// 반복 재생이 아니라면 변수 초기화
		if( !bRepeat )
		{
			m_fPlayTime	= 0.0f;
			m_nPlayCount= 0;
		}
		else
		{
			m_fPlayTime	= fPlayTime;
			m_nPlayCount= nPlayCount;
		}
	}

	// end 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템
	
	// 2010-06-15 by shcho&hslee 펫시스템 - 마을 이동 처리
	D3DXMATRIX m_mPetMatrix;
	float		m_fPetSpeed;
	float		m_fOldDistance;
	float		m_fPetAddSpeed;
	// end 2010-06-15 by shcho&hslee 펫시스템 - 마을 이동 처리
};