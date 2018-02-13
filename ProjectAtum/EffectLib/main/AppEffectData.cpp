// AppEffectData.cpp: implementation of the CAppEffectData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AppEffectData.h"
#include "AtumData.h"
#include "Camera.h"
#include "EnemyData.h"
#include "StoreData.h"
#include "AtumApplication.h"
#include "Background.h"
#include "ShuttleChild.h"
#include "CharacterChild.h"				// 2005-07-21 by ispark
#include "AtumDatabase.h"
#include "ObjectAniData.h"
#include "SpriteAniData.h"
#include "ParticleSystem.h"
#include "dxutil.h"

#include "FxSystem.h" // 2011-10-10 by jhahn EP4 트리거 시스템	화산재 이펙트 추가		

// 2010-06-15 by shcho&hslee 펫시스템 - 펫 무기 생성
// 2010-06-15 by shcho&hslee 펫시스템 - 마을 이동 처리
#define MINDISTANCE	    10.0f
#define MINSPEED		42.0f
#define PETMINDIS		0.5f
// end 2010-06-15 by shcho&hslee 펫시스템 - 마을 이동 처리

// end 2010-06-15 by shcho&hslee 펫시스템 - 펫 무기 생성

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAppEffectData::CAppEffectData(CAtumNode * pParent, int nType, D3DXVECTOR3 vPos, int nWeaponPositionIndex, int LoadingPriority)
{
	FLOG("CAppEffectData(CAtumNode * pParent,int nType,D3DXVECTOR3 vPos)");
	DBGOUT_EFFECT("CAppEffectData Create : this[0x%08x], pParent[0x%08x],nType[%d],vPos(...)\n", this, pParent, nType);
	m_dwPartType = 100;	// 임시
	m_dwState = _NORMAL;
	m_pCamera = NULL;
	m_pParent = pParent;
	m_pCharacterInfo = NULL;
	m_fDistance = 0.0f;
	m_vTargetPos = vPos;
	m_nType = nType;

	if (m_pParent)
	{
		m_vPos = ((CAtumData *)m_pParent)->m_vPos;
		m_vVel = ((CAtumData *)m_pParent)->m_vVel;
		//		m_vUp = ((CAtumData *)m_pParent)->m_vUp;
		m_vUp = D3DXVECTOR3(0, 1, 0);

		m_vPos += m_vTargetPos.x*((CAtumData *)m_pParent)->m_vSideVel + m_vTargetPos.z*((CAtumData *)m_pParent)->m_vVel;

		if (m_nType == RC_EFF_GROUND_01) m_vPos.y = m_vTargetPos.y;
		else if (m_nType == RC_EFF_GROUND_02) SetHeight();
		else if (m_nType == RC_EFF_AGEAR_FIRE)
		{
			CUnitData* pUnit = (CUnitData*)m_pParent;
			m_vPos = vPos;
			//			m_vVel = vPos - (pUnit->m_vPos + pUnit->m_vPrimarySideBackPos);
			m_vVel = vPos - pUnit->m_vLWBackPos;
			m_vUp = pUnit->m_vUp;
			m_vTargetPos = m_vVel;
			//			m_pParent = NULL;
		}
		// 2009. 07. 07 by ckPark 로봇기어 요청사항(롤링, 선택화면, 무기, A기어포대)
		// 로봇 아머 BMI기어용 발사 이펙트
		else if (m_nType == RC_EFF_ROBOTAMOR_FIRE_EFFECT)
		{
			CUnitData* pUnit = (CUnitData*)m_pParent;
			m_vPos = vPos;
			m_vVel = vPos - pUnit->m_vLWBackPos;
			m_vUp = pUnit->m_vUp;
			m_vTargetPos = m_vVel;
		}
		// end 2009. 07. 07 by ckPark 로봇기어 요청사항(롤링, 선택화면, 무기, A기어포대)


		D3DXMatrixLookAtRH(&m_mMatrix, &(m_vPos), &(m_vPos + m_vVel), &m_vUp);
		D3DXMatrixInverse(&m_mMatrix, NULL, &m_mMatrix);
	}
	else
	{
		m_vPos = vPos;
		m_vVel = D3DXVECTOR3(0, 0, 1);
		m_vUp = D3DXVECTOR3(0, 1, 0);
		D3DXMatrixLookAtRH(&m_mMatrix, &(m_vPos), &(m_vPos + m_vVel), &m_vUp);
		D3DXMatrixInverse(&m_mMatrix, NULL, &m_mMatrix);
	}

	// 2010-06-15 by shcho&hslee 펫시스템 - 마을 이동 처리
	m_fPetSpeed = MINSPEED;
	m_fOldDistance = NULL;
	m_fPetAddSpeed = NULL;
	D3DXMATRIX matTempRightPet;
	D3DXMatrixIdentity(&matTempRightPet);
	matTempRightPet._41 = -PET_CITY_DEFULT_POS_X;
	matTempRightPet._42 = PET_CITY_DEFULT_POS_Y;
	matTempRightPet._43 = PET_CITY_DEFULT_POS_Z;
	m_mPetMatrix = matTempRightPet * m_mMatrix;
	// end 2010-06-15 by shcho&hslee 펫시스템 - 마을 이동 처리

	LoadCharacterEffect(m_nType);

	m_bodyCondition = BODYCON_FLY_MASK;
	if (m_pCharacterInfo)
	{
		// 2009. 11. 23 by jskim 리소스 로딩 구조 변경
		m_pCharacterInfo->m_LoadingPriority = LoadingPriority;
		//end 2009. 11. 23 by jskim 리소스 로딩 구조 변경
		m_pCharacterInfo->SetAllBodyConditionMatrix(m_mMatrix);
		m_pCharacterInfo->ChangeBodyCondition(m_bodyCondition);

		// 2012-09-20 by jhahn 아이템 미리보기 버그 수정 - 마크, 파트너 렌더링		
		// 2012-06-14 by isshin 아템미리보기
#ifdef C_ENEMY_INFO_ISSHIN
		if (m_pParent == g_pShuttleChild)
		{

			if (nWeaponPositionIndex != NULL)			   // 2012-10-16 by jhahn 아템미리보기 버그 수정
			{
				nWeaponPositionIndex -= ENEMY_WEAPON_INDEX_DUMMY;

			}
			m_pCharacterInfo->SetInven(nWeaponPositionIndex);
		}
		else if (m_pParent != g_pShuttleChild)
		{
			//	nWeaponPositionIndex += ENEMY_WEAPON_INDEX_DUMMY;									
			m_pCharacterInfo->SetInven(nWeaponPositionIndex);
		}

		//end 2012-09-20 by jhahn 아이템 미리보기 버그 수정 - 마크, 파트너 렌더링		
#else
		if(m_pParent == g_pShuttleChild)
		{
			m_pCharacterInfo->SetInven(nWeaponPositionIndex);
		}
#endif	
		// end 2012-06-14 by isshin 아템미리보기

	}
	else
	{
		m_bUsing = FALSE;
	}
	m_pGUIInfo = NULL;

	// 2009. 07. 07 by ckPark 로봇기어 요청사항(롤링, 선택화면, 무기, A기어포대)
	m_bRender = TRUE;
	// end 2009. 07. 07 by ckPark 로봇기어 요청사항(롤링, 선택화면, 무기, A기어포대)

	// 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템
	SetRepeat(FALSE);
	// end 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		내 위치에 이펙트 보이게 한다.
/// \author		// 2008-12-29 by bhsohn 럭키 머신 성공 이펙트 추가
/// \date		2008-12-29 ~ 2008-12-29
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CAppEffectData::SetAffDataMaxtrix(D3DXVECTOR3	i_vPos, D3DXVECTOR3	i_vVel, D3DXVECTOR3	i_vUp)
{
	m_vPos = i_vPos;
	m_vVel = i_vVel;
	m_vUp = i_vUp;

	D3DXVec3Normalize(&m_vVel, &m_vVel);
	D3DXVec3Normalize(&m_vUp, &m_vUp);

	D3DXMatrixLookAtRH(&m_mMatrix, &(m_vPos), &(m_vPos + m_vVel), &m_vUp);
	D3DXMatrixInverse(&m_mMatrix, NULL, &m_mMatrix);



}
// 2009. 11. 23 by jskim 리소스 로딩 구조 변경
// CAppEffectData::CAppEffectData(CCamera * pParent,
// 							   int nType,
// 							   float fDist, 
// 							   float fSide,
// 							   float fUp)
// {
// 	FLOG( "CAppEffectData(CCamera * pParent,int nType,float fDist, float fSide,float fUp)" );
CAppEffectData::CAppEffectData(CCamera * pParent,
							   int nType,
							   float fDist,
							   float fSide,
							   float fUp,
							   int LoadingPriority)
{
	FLOG("CAppEffectData(CCamera * pParent,int nType,float fDist, float fSide,float fUp, int LoadingPriority)");
	//end 2009. 11. 23 by jskim 리소스 로딩 구조 변경
	DBGOUT_EFFECT("CAppEffectData Create : this[0x%08x], pParent[0x%08x],nType[%d],fDist,fSide,fUp\n", this, pParent, nType);
	m_dwPartType = 100;	// 임시
	m_dwState = _NORMAL;
	m_pParent = NULL;
	m_pCharacterInfo = NULL;
	m_pCamera = pParent;
	m_fDistance = fDist;
	m_fSideDistance = fSide;
	m_fUpDistance = fUp;
	m_vPos = m_pCamera->GetEyePt();
	D3DXVec3Normalize(&m_vVel, &m_pCamera->GetViewDir());
	D3DXVECTOR3 vSide;
	D3DXVec3Normalize(&vSide, &m_pCamera->GetCross());
	D3DXVec3Cross(&m_vUp, &vSide, &m_vVel);
	D3DXVec3Normalize(&m_vUp, &m_vUp);
	m_vPos += m_fDistance*m_vVel + m_fSideDistance*vSide + m_fUpDistance*m_vUp;
	D3DXMatrixLookAtRH(&m_mMatrix, &(m_vPos), &(m_vPos + m_vVel), &m_vUp);
	D3DXMatrixInverse(&m_mMatrix, NULL, &m_mMatrix);

	m_nType = nType;
	LoadCharacterEffect(m_nType);

	m_bodyCondition = BODYCON_FLY_MASK;
	if (m_pCharacterInfo)
	{
		// 2009. 11. 23 by jskim 리소스 로딩 구조 변경
		m_pCharacterInfo->m_LoadingPriority = LoadingPriority;
		//end 2009. 11. 23 by jskim 리소스 로딩 구조 변경
		m_pCharacterInfo->SetAllBodyConditionMatrix(m_mMatrix);
		m_pCharacterInfo->ChangeBodyCondition(m_bodyCondition);
	}
	else
	{
		m_bUsing = FALSE;
	}
	m_pGUIInfo = NULL;

	// 2009. 07. 07 by ckPark 로봇기어 요청사항(롤링, 선택화면, 무기, A기어포대)
	m_bRender = TRUE;
	// end 2009. 07. 07 by ckPark 로봇기어 요청사항(롤링, 선택화면, 무기, A기어포대)

	// 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템
	SetRepeat(FALSE);
	// end 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템
}

// 2009. 11. 23 by jskim 리소스 로딩 구조 변경
// CAppEffectData::CAppEffectData(int nType,
// 							   D3DXVECTOR3 vPos, 
// 							   float fVelRate,
// 							   D3DXVECTOR3 vVel)
// {// 눈날릴때 쓰자
// 	FLOG( "CAppEffectData(int nType,D3DXVECTOR3 vPos, float fVelRate,D3DXVECTOR3 vVel)" );
CAppEffectData::CAppEffectData(int nType,
							   D3DXVECTOR3 vPos,
							   float fVelRate,
							   D3DXVECTOR3 vVel,
							   int LoadingPriority)
{// 눈날릴때 쓰자
	FLOG("CAppEffectData(int nType,D3DXVECTOR3 vPos, float fVelRate,D3DXVECTOR3 vVel, int LoadingPriority)");
	//end 2009. 11. 23 by jskim 리소스 로딩 구조 변경
	DBGOUT_EFFECT("CAppEffectData Create : this[0x%08x], nType[%d],fVelRate,vVel\n", this, nType);
	m_dwPartType = 100;	// 임시
	m_dwState = _NORMAL;
	m_pParent = NULL;
	m_pCharacterInfo = NULL;
	m_pCamera = NULL;
	m_fDistance = 0.0f;
	m_fSideDistance = 0.0f;
	m_fUpDistance = 0.0f;
	m_vPos = vPos;
	m_fVelRate = fVelRate;
	m_vVel = vVel;
	D3DXVECTOR3 vSide;
	vSide = D3DXVECTOR3(1, 0, 0);
	D3DXVec3Cross(&m_vUp, &vSide, &m_vVel);
	D3DXVec3Normalize(&m_vUp, &m_vUp);

	D3DXMatrixLookAtRH(&m_mMatrix, &(m_vPos), &(m_vPos + m_vVel), &m_vUp);
	D3DXMatrixInverse(&m_mMatrix, NULL, &m_mMatrix);

	m_nType = nType;
	LoadCharacterEffect(m_nType);

	m_bodyCondition = BODYCON_FLY_MASK;
	if (m_pCharacterInfo)
	{
		// 2009. 11. 23 by jskim 리소스 로딩 구조 변경
		m_pCharacterInfo->m_LoadingPriority = LoadingPriority;
		//end 2009. 11. 23 by jskim 리소스 로딩 구조 변경
		m_pCharacterInfo->SetAllBodyConditionMatrix(m_mMatrix);
		m_pCharacterInfo->ChangeBodyCondition(m_bodyCondition);
	}
	else
	{
		m_bUsing = FALSE;
	}
	m_pGUIInfo = NULL;

	// 2009. 07. 07 by ckPark 로봇기어 요청사항(롤링, 선택화면, 무기, A기어포대)
	m_bRender = TRUE;
	// end 2009. 07. 07 by ckPark 로봇기어 요청사항(롤링, 선택화면, 무기, A기어포대)

	// 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템
	SetRepeat(FALSE);
	// end 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템
}

// 2009. 11. 23 by jskim 리소스 로딩 구조 변경
// CAppEffectData::CAppEffectData(int nType,D3DXVECTOR3 vPos)
// {
// 	FLOG( "CAppEffectData(int nType,D3DXVECTOR3 vPos)" );
CAppEffectData::CAppEffectData(int nType, D3DXVECTOR3 vPos, int LoadingPriority)
{
	FLOG("CAppEffectData(int nType,D3DXVECTOR3 vPos,int LoadingPriority)");
	//end 2009. 11. 23 by jskim 리소스 로딩 구조 변경
	DBGOUT_EFFECT("CAppEffectData Create : this[0x%08x], nType[%d], vPos\n", this, nType);
	m_dwPartType = 100;	// 임시
	m_dwState = _NORMAL;
	m_pParent = NULL;
	m_pCharacterInfo = NULL;
	m_pCamera = NULL;
	m_fDistance = 0.0f;
	m_fSideDistance = 0.0f;
	m_fUpDistance = 0.0f;
	m_vPos = vPos;
	m_fVelRate = 0.0f;
	m_vVel = D3DXVECTOR3(0, 0, 1);
	D3DXVECTOR3 vSide;
	vSide = D3DXVECTOR3(1, 0, 0);
	D3DXVec3Cross(&m_vUp, &vSide, &m_vVel);
	D3DXVec3Normalize(&m_vUp, &m_vUp);

	D3DXMatrixLookAtRH(&m_mMatrix, &(m_vPos), &(m_vPos + m_vVel), &m_vUp);
	D3DXMatrixInverse(&m_mMatrix, NULL, &m_mMatrix);

	m_nType = nType;
	LoadCharacterEffect(m_nType);

	m_bodyCondition = BODYCON_FLY_MASK;
	if (m_pCharacterInfo)
	{
		// 2009. 11. 23 by jskim 리소스 로딩 구조 변경
		m_pCharacterInfo->m_LoadingPriority = LoadingPriority;
		//end 2009. 11. 23 by jskim 리소스 로딩 구조 변경
		m_pCharacterInfo->SetAllBodyConditionMatrix(m_mMatrix);
		m_pCharacterInfo->ChangeBodyCondition(m_bodyCondition);
	}
	else
	{
		m_bUsing = FALSE;
	}
	m_pGUIInfo = NULL;

	// 2009. 07. 07 by ckPark 로봇기어 요청사항(롤링, 선택화면, 무기, A기어포대)
	m_bRender = TRUE;
	// end 2009. 07. 07 by ckPark 로봇기어 요청사항(롤링, 선택화면, 무기, A기어포대)

	// 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템
	SetRepeat(FALSE);
	// end 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템
}
// 2009. 11. 23 by jskim 리소스 로딩 구조 변경
//ysw 9_22
// CAppEffectData::CAppEffectData(CAtumNode * pParent, 
// 							   CCamera * pCamera, 
// 							   int nType, 
// 							   D3DXVECTOR3 vPos,
// 							   float fDist, 
// 							   float fSide,
// 							   float fUp)
// {
// 	FLOG( "CAppEffectData(CAtumNode * pParent, CCamera * pCamera, int nType, D3DXVECTOR3 vPos,float fDist, float fSide,float fUp)" );
CAppEffectData::CAppEffectData(CAtumNode * pParent,
							   CCamera * pCamera,
							   int nType,
							   D3DXVECTOR3 vPos,
							   float fDist,
							   float fSide,
							   float fUp,
							   int LoadingPriority)
{
	FLOG("CAppEffectData(CAtumNode * pParent, CCamera * pCamera, int nType, D3DXVECTOR3 vPos,float fDist, float fSide,float fUp, int LoadingPriority)");
	//end 2009. 11. 23 by jskim 리소스 로딩 구조 변경
	DBGOUT_EFFECT("CAppEffectData Create : this[0x%08x], pParent[0x%08x],nType[%d],vPos,fDist,fSide,fUp\n", this, pParent, nType);
	m_dwPartType = 100;	// 임시
	m_dwState = _NORMAL;
	m_pCamera = pCamera;
	m_pParent = pParent;
	m_pCharacterInfo = NULL;
	m_fDistance = 0.0f;
	m_vTargetPos = vPos;
	m_fDistance = fDist;
	m_fSideDistance = fSide;
	m_fUpDistance = fUp;
	m_nType = nType;

	if (m_pParent)
	{
		m_vPos = ((CAtumData *)m_pParent)->m_vPos;
		m_vVel = ((CAtumData *)m_pParent)->m_vVel;
		m_vUp = D3DXVECTOR3(0, 1, 0);
		m_vPos += m_vTargetPos.x*((CAtumData *)m_pParent)->m_vSideVel + m_vTargetPos.z*((CAtumData *)m_pParent)->m_vVel;
		if (m_nType == RC_EFF_GROUND_01)
			m_vPos.y = m_vTargetPos.y;
		else if (m_nType == RC_EFF_GROUND_02)
			SetHeight();
		D3DXMatrixLookAtRH(&m_mMatrix, &(m_vPos), &(m_vPos + m_vVel), &m_vUp);
		D3DXMatrixInverse(&m_mMatrix, NULL, &m_mMatrix);
	}

	LoadCharacterEffect(m_nType);

	m_bodyCondition = BODYCON_FLY_MASK;
	if (m_pCharacterInfo)
	{
		// 2009. 11. 23 by jskim 리소스 로딩 구조 변경
		m_pCharacterInfo->m_LoadingPriority = LoadingPriority;
		//end 2009. 11. 23 by jskim 리소스 로딩 구조 변경
		m_pCharacterInfo->SetAllBodyConditionMatrix(m_mMatrix);
		m_pCharacterInfo->ChangeBodyCondition(m_bodyCondition);
	}
	else
	{
		m_bUsing = FALSE;
	}
	m_pGUIInfo = NULL;

	// 2009. 07. 07 by ckPark 로봇기어 요청사항(롤링, 선택화면, 무기, A기어포대)
	m_bRender = TRUE;
	// end 2009. 07. 07 by ckPark 로봇기어 요청사항(롤링, 선택화면, 무기, A기어포대)

	// 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템
	SetRepeat(FALSE);
	// end 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템
}

CAppEffectData::CAppEffectData(GUIUnitRenderInfo * pParent, int nType)
{
	FLOG("CAppEffectData(GUIUnitRenderInfo * pParent,int nType)");
	DBGOUT_EFFECT("CAppEffectData Create : this[0x%08x], pParent(GUI)[0x%08x],nType[%d]\n", this, pParent, nType);
	m_dwPartType = 100;	// 임시
	m_dwState = _NORMAL;
	m_pCamera = NULL;
	m_pParent = NULL;
	m_pGUIInfo = pParent;

	m_pCharacterInfo = NULL;
	m_fDistance = 0.0f;
	m_vTargetPos = D3DXVECTOR3(0, 0, 0);
	if (m_pGUIInfo)
	{
		m_vPos.x = m_pGUIInfo->matrix._41;
		m_vPos.y = m_pGUIInfo->matrix._42;
		m_vPos.z = m_pGUIInfo->matrix._43;
		m_vUp.x = m_pGUIInfo->matrix._21;
		m_vUp.y = m_pGUIInfo->matrix._22;
		m_vUp.z = m_pGUIInfo->matrix._23;
		m_vVel.x = m_pGUIInfo->matrix._31;
		m_vVel.y = m_pGUIInfo->matrix._32;
		m_vVel.z = m_pGUIInfo->matrix._33;
		m_vVel *= -1;
		D3DXMatrixLookAtRH(&m_mMatrix, &(m_vPos), &(m_vPos + m_vVel), &m_vUp);
		D3DXMatrixInverse(&m_mMatrix, NULL, &m_mMatrix);
	}

	m_nType = nType;
	LoadCharacterEffect(m_nType);

	if ((m_nType - 7000000) / 100000 == 2) m_bodyCondition = BODYCON_LANDED_MASK;

	if (m_pCharacterInfo) m_pCharacterInfo->SetAllBodyConditionMatrix(m_mMatrix);
	else m_bUsing = FALSE;

	m_bRender = TRUE;

	SetRepeat(FALSE);
}

CAppEffectData::~CAppEffectData()
{
	FLOG("~CAppEffectData()");

	if (m_pCharacterInfo)
	{
		m_pCharacterInfo->InvalidateDeviceObjects();
		m_pCharacterInfo->DeleteDeviceObjects();
		delete m_pCharacterInfo;
		m_pCharacterInfo = nullptr;
	}
}

void CAppEffectData::Tick()
{
	FLOG("CAppEffectData::Tick()");
	float fElapsedTime = g_pD3dApp->GetElapsedTime();

	if (m_dwState == _NORMAL)
	{
		if (m_pParent && m_pCamera)
		{
			if (m_nType == RC_EFF_TARGET_ARROW)
			{
				if (!COMPARE_BODYCON_BIT(m_bodyCondition, BODYCON_FLY_MASK))
				{
					m_bodyCondition = BODYCON_FLY_MASK;
					if (m_pCharacterInfo)
						m_pCharacterInfo->ChangeBodyCondition(m_bodyCondition);
				}
				if (g_pD3dApp->m_pShuttleChild->m_pOrderTarget)
				{

					D3DXMATRIX matRotate;
					D3DXVECTOR3 vUp, vSide, vVel, vTargetVel;

					D3DXVec3Normalize(&vTargetVel, &(g_pD3dApp->m_pShuttleChild->m_pOrderTarget->m_vPos - g_pD3dApp->m_pShuttleChild->m_vPos));
					vVel = ((CShuttleChild *)m_pParent)->m_vWeaponVel;
					float fTemp = -((vVel.x*vTargetVel.x + vVel.y*vTargetVel.y + vVel.z*vTargetVel.z) / (vVel.x*vVel.x + vVel.y*vVel.y + vVel.z*vVel.z));
					D3DXVECTOR3 vNew;
					vNew.x = vVel.x*fTemp + vTargetVel.x;
					vNew.y = vVel.y*fTemp + vTargetVel.y;
					vNew.z = vVel.z*fTemp + vTargetVel.z;
					D3DXVec3Normalize(&vNew, &vNew);

					//					D3DXVECTOR2 v1, v2, v3;

					D3DXVec3Normalize(&m_vVel, &g_pD3dApp->m_pCamera->GetViewDir());
					D3DXVec3Normalize(&m_vUp, &g_pD3dApp->m_pCamera->GetUpVec());
					D3DXVec3Normalize(&vSide, &g_pD3dApp->m_pCamera->GetCross());

					m_vPos = g_pD3dApp->m_pCamera->GetEyePt() + 50.0f*m_vVel + 11.5f*vNew;

					float fAngle = ACOS(D3DXVec3Dot(&m_vUp, &vNew));

					if (ACOS(D3DXVec3Dot(&g_pD3dApp->m_pShuttleChild->m_vSideVel, &vNew)) < D3DX_PI / 2.0f)
						fAngle *= -1.0f;// 좌우 구별

					D3DXMatrixRotationAxis(&matRotate, &m_vVel, fAngle);
					D3DXVec3TransformCoord(&m_vUp, &m_vUp, &matRotate);

					D3DXMatrixLookAtRH(&m_mMatrix, &(m_vPos), &(m_vPos + m_vVel), &m_vUp);
					D3DXMatrixInverse(&m_mMatrix, NULL, &m_mMatrix);

				}
			}
			else
			{// 스피드, 무기종류, Height,
				m_vPos = m_pCamera->GetEyePt();
				D3DXVec3Normalize(&m_vVel, &m_pCamera->GetViewDir());
				D3DXVECTOR3 vSide;
				D3DXVec3Normalize(&vSide, &m_pCamera->GetCross());
				D3DXVec3Cross(&m_vUp, &vSide, &m_vVel);
				D3DXVec3Normalize(&m_vUp, &m_vUp);
				m_vPos += m_fDistance*m_vVel + m_fSideDistance*vSide + m_fUpDistance*m_vUp;
				D3DXMatrixLookAtRH(&m_mMatrix, &m_vPos, &(m_vPos + m_vVel), &m_vUp);
				D3DXMatrixInverse(&m_mMatrix, NULL, &m_mMatrix);

				D3DXVECTOR3 v1 = D3DXVECTOR3(((CShuttleChild*)m_pParent)->m_mMatrix._31, 0,
											 ((CShuttleChild*)m_pParent)->m_mMatrix._33);
				D3DXVECTOR3 v2 = D3DXVECTOR3(-m_pCamera->GetViewDir().x, 0, -m_pCamera->GetViewDir().z);
				D3DXVec3Normalize(&v1, &v1);
				D3DXVec3Normalize(&v2, &v2);
				FLOAT fAngle = ACOS(D3DXVec3Dot(&v1, &v2));
				v1 = m_pCamera->GetViewDir();
				if (fAngle > 0)
				{
					D3DXVECTOR2 vTarget = D3DXVECTOR2(((CShuttleChild*)m_pParent)->m_mMatrix._31,
													  ((CShuttleChild*)m_pParent)->m_mMatrix._33); // T
					D3DXVECTOR2 vCam = D3DXVECTOR2(-m_pCamera->GetViewDir().x, -m_pCamera->GetViewDir().z); // A
					D3DXVECTOR2 vSide = D3DXVECTOR2(m_pCamera->GetCross().x, m_pCamera->GetCross().z); // B

					/////////////// 삼각형 점포함 테스트 ////////////////////////
					D3DXVec2Normalize(&vTarget, &vTarget);
					D3DXVec2Normalize(&vCam, &vCam);
					D3DXVec2Normalize(&vSide, &vSide);
					D3DXVECTOR2 vPlus = vCam + vSide; // C

					D3DXVECTOR2	vAB, vAT, vBC, vBT, vCA, vCT;
					vAB = vSide - vCam;
					vAT = vTarget - vCam;
					vBC = vPlus - vSide;
					vBT = vTarget - vSide;
					vCA = vCam - vPlus;
					vCT = vTarget - vPlus;

					float	fCross1, fCross2, fCross3;
					fCross1 = vAB.x*vAT.y - vAB.y*vAT.x;
					fCross2 = vBC.x*vBT.y - vBC.y*vBT.x;
					fCross3 = vCA.x*vCT.y - vCA.y*vCT.x;

					if ((fCross1 >= 0 && fCross2 >= 0 && fCross3 >= 0) || (fCross1 <= 0 && fCross2 <= 0 && fCross3 <= 0))
						fAngle *= -1;

					D3DXMATRIX matRotate;
					D3DXVECTOR3 vUp(0, -1, 0);

					D3DXMatrixRotationAxis(&matRotate, &vUp, fAngle);
					m_mMatrix = matRotate * m_mMatrix;

				}
			}

		}
		else if (m_pParent)
		{
			if (m_nType == RC_EFF_AGEAR_FIRE)
			{
				//				if(m_pParent->m_dwPartType == _SHUTTLE)
				//				{
				//					m_vPos = ((CAtumData *)m_pParent)->m_vLWPos;
				//					m_vUp.x = ((CAtumData *)m_pParent)->m_mMatrix._21;
				//					m_vUp.y = ((CAtumData *)m_pParent)->m_mMatrix._22;
				//					m_vUp.z = ((CAtumData *)m_pParent)->m_mMatrix._23;
				//					m_vVel = ((CShuttleChild *)m_pParent)->m_vWeaponVel;
				//				}
			}


			// 2009. 07. 07 by ckPark 로봇기어 요청사항(롤링, 선택화면, 무기, A기어포대)
			// 로봇 아머 BMI기어용 발사 이펙트
			else if (m_nType == RC_EFF_ROBOTAMOR_FIRE_EFFECT)
			{
				D3DXMATRIX matTemp;

				if (m_nFirePos == LEFT)
					matTemp = ((CAtumData *)m_pParent)->m_pMatWeaponPosition[0] * ((CAtumData *)m_pParent)->m_mMatrix;
				else if (m_nFirePos == RIGHT)
					matTemp = ((CAtumData *)m_pParent)->m_pMatWeaponPosition[1] * ((CAtumData *)m_pParent)->m_mMatrix;
				else
					matTemp = ((CAtumData *)m_pParent)->m_mMatrix;

				m_vPos.x = matTemp._41;
				m_vPos.y = matTemp._42;
				m_vPos.z = matTemp._43;
				m_vUp.x = matTemp._21;
				m_vUp.y = matTemp._22;
				m_vUp.z = matTemp._23;
				m_vVel.x = matTemp._31;
				m_vVel.y = matTemp._32;
				m_vVel.z = matTemp._33;
				m_vVel *= -1;
				D3DXMatrixLookAtRH(&m_mMatrix, &(m_vPos), &(m_vPos + m_vVel), &m_vUp);
				D3DXMatrixInverse(&m_mMatrix, NULL, &m_mMatrix);
			}
			// end 2009. 07. 07 by ckPark 로봇기어 요청사항(롤링, 선택화면, 무기, A기어포대)


			else
			{
				m_vPos = ((CAtumData *)m_pParent)->m_vPos;
				m_vVel = ((CAtumData *)m_pParent)->m_vVel;
				m_vPos += m_vTargetPos.x*((CAtumData *)m_pParent)->m_vSideVel + m_vTargetPos.z*((CAtumData *)m_pParent)->m_vVel;

				if (m_nType == RC_EFF_GROUND_01)
				{
					m_vPos.y = m_vTargetPos.y;
					D3DXMatrixLookAtRH(&m_mMatrix, &(m_vPos), &(m_vPos + m_vVel), &m_vUp);
					D3DXMatrixInverse(&m_mMatrix, NULL, &m_mMatrix);
				}
				else if (m_nType == RC_EFF_GROUND_02)
				{
					SetHeight();
					D3DXMatrixLookAtRH(&m_mMatrix, &(m_vPos), &(m_vPos + m_vVel), &m_vUp);
					D3DXMatrixInverse(&m_mMatrix, NULL, &m_mMatrix);
				}
				else if (m_nType / 1000000 == 7 ||				// item
						 m_nType / 1000000 == 4 ||				//2011-10-06 by jhahn 파트너 성장형 시스템
						 m_nType / 100000 == 1 ||				// 2010-06-15 by shcho&hslee 펫시스템 - 펫 무기 생성
						 m_nType / 10000000 == 1 ||
						 m_nType == RC_EFF_BGEAR_TRACE ||
						 m_nType == RC_EFF_MGEAR_TRACE ||
						 m_nType == RC_EFF_IGEAR_TRACE ||
						 m_nType == RC_EFF_AGEAR_TRACE)
				{
					// 장착 아이템
					// A기어 무기
					//					ITEM* pITEM = g_pDatabase->GetServerItemInfo( m_nType - EFFECT_WEAR_WEAPON_1 );
					//					if( pITEM &&
					//						(pITEM->Kind == ITEMKIND_CANNON ||
					//						pITEM->Kind == ITEMKIND_MASSDRIVE) )
					if (((m_pParent->m_dwPartType == _SHUTTLE &&
						IS_DT(((CShuttleChild *)m_pParent)->m_myShuttleInfo.UnitKind)) ||
						(m_pParent->m_dwPartType == _ENEMY &&
						IS_DT(((CEnemyData *)m_pParent)->m_infoCharacter.CharacterInfo.UnitKind))) &&
						(((CAtumData *)m_pParent)->m_pWeapon1_1_1 == this ||
						((CAtumData *)m_pParent)->m_pWeapon1_2 == this))
					{
						if ((m_pParent->m_dwPartType == _SHUTTLE &&
							((CShuttleChild *)m_pParent)->m_bAttackMode == _SIEGE)
							||
							m_pParent->m_dwPartType == _ENEMY)
						{
							D3DXMATRIX matTemp;
							D3DXVECTOR3 vVel;
							matTemp = ((CAtumData *)m_pParent)->m_pMatWeaponPosition[0] * ((CAtumData *)m_pParent)->m_mMatrix;
							m_vPos.x = matTemp._41;
							m_vPos.y = matTemp._42;
							m_vPos.z = matTemp._43;
							m_vUp.x = matTemp._21;
							m_vUp.y = matTemp._22;
							m_vUp.z = matTemp._23;

							//m_vPos = ((CAtumData *)m_pParent)->m_vPos;
							//m_vUp.x = ((CAtumData *)m_pParent)->m_mMatrix._21;
							//m_vUp.y = ((CAtumData *)m_pParent)->m_mMatrix._22;
							//m_vUp.z = ((CAtumData *)m_pParent)->m_mMatrix._23;
							vVel = ((CAtumData *)m_pParent)->m_vWeaponVel;

							float fTemp = -((m_vUp.x*vVel.x + m_vUp.y*vVel.y + m_vUp.z*vVel.z) / (m_vUp.x*m_vUp.x + m_vUp.y*m_vUp.y + m_vUp.z*m_vUp.z));
							m_vVel.x = m_vUp.x*fTemp + vVel.x;
							m_vVel.y = m_vUp.y*fTemp + vVel.y;
							m_vVel.z = m_vUp.z*fTemp + vVel.z;
							D3DXVec3Normalize(&m_vVel, &m_vVel);
							D3DXVec3Normalize(&vVel, &vVel);
							float fAngle = ACOS(D3DXVec3Dot(&vVel, &m_vVel));
							D3DXPLANE pl;
							D3DXVECTOR3 vSide;
							D3DXVec3Cross(&vSide, &m_vUp, &m_vVel);
							D3DXPlaneFromPoints(&pl, &m_vVel, &vSide, &m_vUp);
							if (vVel.x*pl.a + vVel.y*pl.b + vVel.z*pl.c + pl.d <= 0)
							{
								fAngle = 0.0f;
							}
							fAngle = (int)((fAngle*180.0f) / (D3DX_PI));
							m_pCharacterInfo->SetObjectAniStartTime(fAngle*10.0f + 0.1f);// 0.1f 는 이펙트 시작 시간이 0이면 무시되기 때문에 더해준다.

							// 2009. 07. 07 by ckPark 로봇기어 요청사항(롤링, 선택화면, 무기, A기어포대)
							// 로봇 아머 A기어용 포대
							float fAnimationStartTime = fAngle*10.0f + 0.1f;
							// end 2009. 07. 07 by ckPark 로봇기어 요청사항(롤링, 선택화면, 무기, A기어포대)

							D3DXMatrixLookAtRH(&m_mMatrix, &(m_vPos), &(m_vPos + m_vVel), &m_vUp);
							D3DXMatrixInverse(&m_mMatrix, NULL, &m_mMatrix);

							// 2009. 07. 07 by ckPark 로봇기어 요청사항(롤링, 선택화면, 무기, A기어포대)
							// 로봇 아머 A기어용 포대
							if (((CUnitData*)m_pParent)->IsRobotArmor()
								&& ((CUnitData*)m_pParent)->m_pRobotAGearWeapon_1)
							{
								((CUnitData*)m_pParent)->m_pRobotAGearWeapon_1->m_pCharacterInfo->SetObjectAniStartTime(fAnimationStartTime);
								((CUnitData*)m_pParent)->m_pRobotAGearWeapon_1->m_mMatrix = m_mMatrix;
							}
							// end 2009. 07. 07 by ckPark 로봇기어 요청사항(롤링, 선택화면, 무기, A기어포대)
						}
						else
						{
							D3DXMATRIX matRotate, matTrans1, matTrans2;
							D3DXVECTOR3 vUnitVel, vCenterPos, vWeaponPos;
							D3DXVECTOR3 vSide;
							D3DXVECTOR3 vPlanePos;	// 만나는 점
							D3DXPLANE	p;
							D3DXMATRIX matTemp;
							matTemp = ((CAtumData *)m_pParent)->m_pMatWeaponPosition[0] * ((CAtumData *)m_pParent)->m_mMatrix;
							m_vPos.x = matTemp._41;
							m_vPos.y = matTemp._42;
							m_vPos.z = matTemp._43;
							m_vUp.x = matTemp._21;
							m_vUp.y = matTemp._22;
							m_vUp.z = matTemp._23;
							vUnitVel.x = -matTemp._31;
							vUnitVel.y = -matTemp._32;
							vUnitVel.z = -matTemp._33;

							//							m_vPos = ((CAtumData *)m_pParent)->m_vPos;
							//							m_vUp.x = ((CAtumData *)m_pParent)->m_mMatrix._21;
							//							m_vUp.y = ((CAtumData *)m_pParent)->m_mMatrix._22;
							//							m_vUp.z = ((CAtumData *)m_pParent)->m_mMatrix._23;
							//							vUnitVel.x = -((CAtumData *)m_pParent)->m_mMatrix._31;
							//							vUnitVel.y = -((CAtumData *)m_pParent)->m_mMatrix._32;
							//							vUnitVel.z = -((CAtumData *)m_pParent)->m_mMatrix._33;
							vWeaponPos = ((CAtumData *)m_pParent)->m_vWeaponVel;
							D3DXVec3Cross(&vSide, &m_vUp, &vUnitVel);
							D3DXVec3Normalize(&vSide, &vSide);
							D3DXVec3Normalize(&m_vUp, &m_vUp);
							D3DXVec3Normalize(&vUnitVel, &vUnitVel);
							D3DXVec3Normalize(&vWeaponPos, &vWeaponPos);
							D3DXPlaneFromPointNormal(&p, &D3DXVECTOR3(0, 0, 0), &vSide); // vVel, vUp 으로 이루어진 평면
							D3DXPlaneIntersectLine(&vPlanePos, &p, &vWeaponPos, &(vWeaponPos - vSide));
							D3DXVec3Normalize(&vPlanePos, &vPlanePos);
							float fAngle = ACOS(D3DXVec3Dot(&vPlanePos, &vUnitVel)); // 이펙트의 AniTime(세로 각도)
							fAngle = (int)((fAngle*180.0f) / (D3DX_PI));
							D3DXPlaneFromPointNormal(&p, &D3DXVECTOR3(0, 0, 0), &m_vUp);
							if (vWeaponPos.x*p.a + vWeaponPos.y*p.b + vWeaponPos.z*p.c + p.d <= 0)
							{
								fAngle = 0.0f;
							}
							m_pCharacterInfo->SetObjectAniStartTime(fAngle*10.0f + 0.1f);// 0.1f 는 이펙트 시작 시간이 0이면 무시되기 때문에 더해준다.

							// 2009. 07. 07 by ckPark 로봇기어 요청사항(롤링, 선택화면, 무기, A기어포대)
							// 로봇 아머 A기어용 포대
							float fAnimationStartTime = fAngle*10.0f + 0.1f;
							// end 2009. 07. 07 by ckPark 로봇기어 요청사항(롤링, 선택화면, 무기, A기어포대)

							D3DXPlaneFromPointNormal(&p, &D3DXVECTOR3(0, 0, 0), &m_vUp); // vVel, vSide 으로 이루어진 평면
							D3DXPlaneIntersectLine(&vPlanePos, &p, &vWeaponPos, &(vWeaponPos - m_vUp));
							D3DXVec3Normalize(&vPlanePos, &vPlanePos);
							fAngle = ACOS(D3DXVec3Dot(&vPlanePos, &vUnitVel)); // 이펙트의 가로 방향(가로 각도)
							if (g_pShuttleChild->m_pt.x < (int)(g_pD3dApp->GetBackBufferDesc().Width / 2))
							{
								fAngle *= -1;
							}
							D3DXMatrixRotationAxis(&matRotate, &D3DXVECTOR3(0, 1, 0), fAngle);
							vCenterPos = -A_GEAR_PRIMARY_PORT_POSITION;
							D3DXMatrixTranslation(&matTrans1, vCenterPos.x, vCenterPos.y, vCenterPos.z);
							vCenterPos = A_GEAR_PRIMARY_PORT_POSITION;
							D3DXMatrixTranslation(&matTrans2, vCenterPos.x, vCenterPos.y, vCenterPos.z);
							D3DXMatrixLookAtRH(&m_mMatrix, &(m_vPos), &(m_vPos + vUnitVel), &m_vUp);
							D3DXMatrixInverse(&m_mMatrix, NULL, &m_mMatrix);
							m_mMatrix = matTrans1 * matRotate * matTrans2 * m_mMatrix;

							// 2009. 07. 07 by ckPark 로봇기어 요청사항(롤링, 선택화면, 무기, A기어포대)
							// 로봇 아머 A기어용 포대
							if (((CUnitData*)m_pParent)->IsRobotArmor()
								&& ((CUnitData*)m_pParent)->m_pRobotAGearWeapon_1)
							{
								((CUnitData*)m_pParent)->m_pRobotAGearWeapon_1->m_pCharacterInfo->SetObjectAniStartTime(fAnimationStartTime);
								((CUnitData*)m_pParent)->m_pRobotAGearWeapon_1->m_mMatrix = m_mMatrix;
							}
							// end 2009. 07. 07 by ckPark 로봇기어 요청사항(롤링, 선택화면, 무기, A기어포대)
						}
					}
					// 2009. 07. 07 by ckPark 로봇기어 요청사항(롤링, 선택화면, 무기, A기어포대)
					//else
					// 로봇 아머 A기어용 포대
					else if (!((CUnitData*)m_pParent)->IsRobotArmor() || ((CUnitData*)m_pParent)->m_pRobotAGearWeapon_1 != this)
						// end 2009. 07. 07 by ckPark 로봇기어 요청사항(롤링, 선택화면, 무기, A기어포대)
					{
						D3DXMATRIX matTemp;
						if (((CAtumData *)m_pParent)->m_pWeapon1_1_1 == this)
						{
							matTemp = ((CAtumData *)m_pParent)->m_pMatWeaponPosition[0] * ((CAtumData *)m_pParent)->m_mMatrix;
							m_vPos.x = matTemp._41;
							m_vPos.y = matTemp._42;
							m_vPos.z = matTemp._43;

							m_vVel.x = matTemp._31;
							m_vVel.y = matTemp._32;
							m_vVel.z = matTemp._33;
						}
						else if (((CAtumData *)m_pParent)->m_pWeapon1_1_2 == this)
						{
							matTemp = ((CAtumData *)m_pParent)->m_pMatWeaponPosition[1] * ((CAtumData *)m_pParent)->m_mMatrix;
							m_vPos.x = matTemp._41;
							m_vPos.y = matTemp._42;
							m_vPos.z = matTemp._43;

							m_vVel.x = matTemp._31;
							m_vVel.y = matTemp._32;
							m_vVel.z = matTemp._33;
						}
						else if (((CAtumData *)m_pParent)->m_pWeapon2_1_1 == this)
						{
							matTemp = ((CAtumData *)m_pParent)->m_pMatWeaponPosition[2] * ((CAtumData *)m_pParent)->m_mMatrix;
							m_vPos.x = matTemp._41;
							m_vPos.y = matTemp._42;
							m_vPos.z = matTemp._43;

							m_vVel.x = matTemp._31;
							m_vVel.y = matTemp._32;
							m_vVel.z = matTemp._33;
						}
						else if (((CAtumData *)m_pParent)->m_pWeapon2_1_2 == this)
						{
							matTemp = ((CAtumData *)m_pParent)->m_pMatWeaponPosition[3] * ((CAtumData *)m_pParent)->m_mMatrix;
							m_vPos.x = matTemp._41;
							m_vPos.y = matTemp._42;
							m_vPos.z = matTemp._43;

							m_vVel.x = matTemp._31;
							m_vVel.y = matTemp._32;
							m_vVel.z = matTemp._33;
						}
						// 2010-06-15 by shcho&hslee 펫시스템 - 펫 무기 생성
						// 2010-06-15 by shcho&hslee 펫시스템 - 마을 이동 처리
						else if ((((CAtumData *)m_pParent)->m_pPartner == this &&
							!g_pD3dApp->m_bCharacter ||
							((CAtumData *)m_pParent)->m_pDummyPartner == this))
						{
							matTemp = ((CAtumData *)m_pParent)->m_pMatPetPosition[0] * ((CAtumData *)m_pParent)->m_mMatrix;
							m_vPos.x = matTemp._41;
							m_vPos.y = matTemp._42;
							m_vPos.z = matTemp._43;

							m_vVel = ((CAtumData *)m_pParent)->m_vPetLeftVel;
							D3DXVec3Normalize(&m_vVel, &-m_vVel);
						}
						else if (((CAtumData *)m_pParent)->m_pPartner == this && g_pD3dApp->m_bCharacter)
						{
							// 2010-06-15 by shcho&hslee 펫시스템 - 마을 이동 처리
							//matTemp  = ((CAtumData *)m_pParent)->m_mMatrix;
							//m_vPos.x = matTemp._41 + PET_CITY_DEFULT_POS_X;
							//m_vPos.y = matTemp._42 + PET_CITY_DEFULT_POS_Y;
							//m_vPos.z = matTemp._43 + PET_CITY_DEFULT_POS_Z;
							//D3DXVec3Normalize(&m_vVel, &-m_vVel);
							D3DXMATRIX matRightPos;
							D3DXMatrixIdentity(&matRightPos);
							D3DXVECTOR3 vecRightPos, charpos, petpos;
							matTemp = ((CAtumData *)m_pParent)->m_mMatrix;

							// 펫이 서있을 좌표가 셔틀 m_mMatrix와 적 m_mMatrix의 위치가 달라 각각 지정 하였음.
							if (((CUnitData*)m_pParent) == (CUnitData*)g_pD3dApp->m_pShuttleChild)
							{
								matRightPos._41 = -PET_CITY_DEFULT_POS_X;
								matRightPos._42 = PET_CITY_DEFULT_POS_Y;
								matRightPos._43 = PET_CITY_DEFULT_POS_Z;
							}
							else
							{
								matRightPos._41 = -(PET_CITY_DEFULT_POS_X - PET_CITY_GAP_POS_X);
								matRightPos._42 = PET_CITY_DEFULT_POS_Y - PET_CITY_GAP_POS_Y;
								matRightPos._43 = PET_CITY_DEFULT_POS_Z;
							}
							matRightPos = matRightPos * matTemp;

							vecRightPos.x = matRightPos._41;
							vecRightPos.y = matRightPos._42;
							vecRightPos.z = matRightPos._43;

							petpos.x = m_mPetMatrix._41;
							petpos.y = matRightPos._42;
							petpos.z = m_mPetMatrix._43;

							charpos.x = matTemp._41;
							charpos.y = matRightPos._42;
							charpos.z = matTemp._43;

							if (D3DXVec3Length(&(charpos - petpos)) > MINDISTANCE)
							{
								((CUnitData *)m_pParent)->ChangeWearItemBodyCondition(WEAR_ITEM_KIND_PET, BODYCON_CHARACTER_MODE_RUN);
								SetPetPos(charpos, petpos, fElapsedTime);
								D3DXVec3Normalize(&m_vVel, &-(charpos - petpos));
							}
							else if (D3DXVec3Length(&(vecRightPos - petpos)) > PETMINDIS &&
									 D3DXVec3Length(&(charpos - petpos)) <= MINDISTANCE)
							{
								D3DXVECTOR3 vecMove;
								D3DXVec3Normalize(&vecMove, &(vecRightPos - petpos));

								m_vPos.x = petpos.x + vecMove.x * ((MINDISTANCE)* fElapsedTime);
								m_vPos.z = petpos.z + vecMove.z * ((MINDISTANCE)* fElapsedTime);
								m_vPos.y = petpos.y;
								D3DXVec3Normalize(&m_vVel, &-(vecRightPos - petpos));
							}
							else
							{
								m_vPos = petpos;
								((CUnitData *)m_pParent)->ChangeWearItemBodyCondition(WEAR_ITEM_KIND_PET, BODYCON_CHARACTER_MODE_STOP);

								m_vVel.x = matTemp._31;
								m_vVel.y = matTemp._32;
								m_vVel.z = matTemp._33;
								D3DXVec3Normalize(&m_vVel, &m_vVel);
							}
							m_fOldDistance = D3DXVec3Length(&(charpos - petpos));
							// end 2010-06-15 by shcho&hslee 펫시스템 - 마을 이동 처리
						}
						else if (((CAtumData *)m_pParent)->m_pPartner1 == this || ((CAtumData *)m_pParent)->m_pDummyPartner1 == this)
						{
							matTemp = ((CAtumData *)m_pParent)->m_pMatPetPosition[1] * ((CAtumData *)m_pParent)->m_mMatrix;
							m_vPos.x = matTemp._41;
							m_vPos.y = matTemp._42;
							m_vPos.z = matTemp._43;

							m_vVel = ((CAtumData *)m_pParent)->m_vPetRightVel;
							D3DXVec3Normalize(&m_vVel, &-m_vVel);
						}
						// end 2010-06-15 by shcho&hslee 펫시스템 - 펫 무기 생성
						else
						{
							matTemp = ((CAtumData *)m_pParent)->m_mMatrix;
							m_vPos = ((CAtumData *)m_pParent)->m_vPos;
							m_vVel.x = matTemp._31;
							m_vVel.y = matTemp._32;
							m_vVel.z = matTemp._33;
						}

						m_vUp.x = matTemp._21;
						m_vUp.y = matTemp._22;
						m_vUp.z = matTemp._23;
						// 						m_vVel.x = matTemp._31;
						// 						m_vVel.y = matTemp._32;
						// 						m_vVel.z = matTemp._33;
						m_vVel *= -1;

						// 2010-06-15 by shcho&hslee 펫시스템 - 마을 이동 처리
						//D3DXMatrixLookAtRH( &m_mMatrix, &(m_vPos), &(m_vPos + m_vVel), &m_vUp);
						//D3DXMatrixInverse( &m_mMatrix, NULL, &m_mMatrix );
						if (((CAtumData *)m_pParent)->m_pPartner == this && g_pD3dApp->m_bCharacter)
						{
							D3DXMatrixLookAtRH(&m_mMatrix, &(m_vPos), &(m_vPos + m_vVel), &m_vUp);
							D3DXMatrixInverse(&m_mMatrix, NULL, &m_mMatrix);
							m_mPetMatrix = m_mMatrix;
						}
						// 2011-03-21 by jhAhn 마크시스템 마크 위치 생성
						else if (g_pD3dApp->m_bCharacter)
						{
							matTemp = ((CAtumData *)m_pParent)->m_mMatrix;

							m_vPos.x = matTemp._41 + MARK_DEFULT_POS_X;
							m_vPos.y = matTemp._42 + MARK_DEFULT_POS_Y;
							m_vPos.z = matTemp._43 + MARK_DEFULT_POS_Z;

							D3DXMatrixLookAtRH(&m_mMatrix, &(m_vPos), &(m_vPos + m_vVel), &m_vUp);
							D3DXMatrixInverse(&m_mMatrix, NULL, &m_mMatrix);
						}
						else if (((CAtumData *)m_pParent)->m_pWingIn == this)
						{

							matTemp = ((CAtumData *)m_pParent)->m_mMatrix;
							D3DXMATRIX markPostion = ((CAtumData *)m_pParent)->m_pMatMarkPosition;
							markPostion = markPostion * matTemp;

							m_vPos.x = markPostion._41;
							m_vPos.y = markPostion._42;
							m_vPos.z = markPostion._43;

							D3DXMatrixLookAtRH(&m_mMatrix, &(m_vPos), &(m_vPos + m_vVel), &m_vUp);
							D3DXMatrixInverse(&m_mMatrix, NULL, &m_mMatrix);
						}
						//end 2011-03-21 by jhAhn 마크시스템 마크 위치 생성
						else
						{
							D3DXMatrixLookAtRH(&m_mMatrix, &(m_vPos), &(m_vPos + m_vVel), &m_vUp);
							D3DXMatrixInverse(&m_mMatrix, NULL, &m_mMatrix);
						}
						// end 2010-06-15 by shcho&hslee 펫시스템 - 마을 이동 처리
					}
				}
				else
				{
					// RC_EFF_USEITEM_REPAIR_HP
					D3DXMatrixLookAtRH(&m_mMatrix, &(m_vPos), &(m_vPos + m_vVel), &m_vUp);
					D3DXMatrixInverse(&m_mMatrix, NULL, &m_mMatrix);
				}
			}
		}
		else if (m_pCamera)
		{
			m_vPos = m_pCamera->GetEyePt();
			D3DXVec3Normalize(&m_vVel, &m_pCamera->GetViewDir());
			D3DXVECTOR3 vSide;
			D3DXVec3Normalize(&vSide, &m_pCamera->GetCross());
			D3DXVec3Cross(&m_vUp, &vSide, &m_vVel);
			D3DXVec3Normalize(&m_vUp, &m_vUp);
			m_vPos += m_fDistance*m_vVel + m_fSideDistance*vSide + m_fUpDistance*m_vUp;
			D3DXMatrixLookAtRH(&m_mMatrix, &(m_vPos), &(m_vPos + m_vVel), &m_vUp);
			D3DXMatrixInverse(&m_mMatrix, NULL, &m_mMatrix);

			if (m_nType == RC_EFF_GAGE_BOX || m_nType == RC_EFF_EXP_MIDDLE_GAGE)
			{
				D3DXVECTOR3		vAxis(-1, 0, 0);
				D3DXMATRIX		matRotate;

				D3DXVec3Normalize(&vAxis, &vAxis);

				D3DXMatrixRotationAxis(&matRotate, &vAxis, D3DXToRadian(64));
				m_mMatrix = matRotate * m_mMatrix;
			}
		}
		else if (m_pGUIInfo)
		{
			if (m_nType != 1100013)
			{
				m_vPos.x = m_pGUIInfo->matrix._41;
				m_vPos.y = m_pGUIInfo->matrix._42;
				m_vPos.z = m_pGUIInfo->matrix._43;
				m_vUp.x = m_pGUIInfo->matrix._21;
				m_vUp.y = m_pGUIInfo->matrix._22;
				m_vUp.z = m_pGUIInfo->matrix._23;
				m_vVel.x = m_pGUIInfo->matrix._31;
				m_vVel.y = m_pGUIInfo->matrix._32;
				m_vVel.z = m_pGUIInfo->matrix._33;
				m_vVel *= -1;
				D3DXMatrixLookAtRH(&m_mMatrix, &(m_vPos), &(m_vPos + m_vVel), &m_vUp);
				D3DXMatrixInverse(&m_mMatrix, NULL, &m_mMatrix);
			}
		}
		else
		{// 눈
			if (m_nType == RC_EFF_SNOW)
			{
				D3DXVECTOR3 dir, v;
				dir = D3DXVECTOR3(0, -1, 0);
				m_fVelRate += 10.0f*fElapsedTime;
				m_vVel.y -= 10.0f*fElapsedTime;
				D3DXVec3Normalize(&m_vVel, &m_vVel);
				m_vPos += m_fVelRate*m_vVel*fElapsedTime;
				D3DXMatrixLookAtRH(&m_mMatrix, &(m_vPos), &(g_pD3dApp->m_pCamera->GetEyePt()), &g_pD3dApp->m_pCamera->GetUpVec());
				D3DXMatrixInverse(&m_mMatrix, NULL, &m_mMatrix);
				float fDist;

				int i = (int)(m_vPos.x / TILE_SIZE);
				int j = (int)(m_vPos.z / TILE_SIZE);
				if (i >= 0 && i < g_pGround->m_projectInfo.sXSize && j >= 0 && j < g_pGround->m_projectInfo.sYSize)
				{
					if (g_pGround->IntersectTriangle(m_vPos, dir, g_pGround->m_pTileVertexArray[i*(g_pGround->m_projectInfo.sYSize + 1) + j].pos,
						g_pGround->m_pTileVertexArray[i*(g_pGround->m_projectInfo.sYSize + 1) + (j + 1)].pos,
						g_pGround->m_pTileVertexArray[(i + 1)*(g_pGround->m_projectInfo.sYSize + 1) + (j)].pos,
						&fDist))
					{
						v = m_vPos + dir*fDist;
					}
					else if (g_pGround->IntersectTriangle(m_vPos, dir, g_pGround->m_pTileVertexArray[i*(g_pGround->m_projectInfo.sYSize + 1) + j + 1].pos,
						g_pGround->m_pTileVertexArray[(i + 1)*(g_pGround->m_projectInfo.sYSize + 1) + (j)].pos,
						g_pGround->m_pTileVertexArray[(i + 1)*(g_pGround->m_projectInfo.sYSize + 1) + j + 1].pos,
						&fDist))
					{
						v = m_vPos + dir*fDist;
					}
				}
				else
				{
					m_bodyCondition = BODYCON_LANDED_MASK;
					m_pCharacterInfo->ChangeBodyCondition(m_bodyCondition);
				}
				if (v.y >= m_vPos.y)
				{
					m_vPos = v;
					m_bodyCondition = BODYCON_LANDED_MASK;
					m_pCharacterInfo->ChangeBodyCondition(m_bodyCondition);
					m_dwState = _EXPLODING;
				}
			}
		}
	}
	else if (m_dwState == _EXPLODING)
	{
	}

	// effect matrix & ticking

	switch (m_nType)
	{
	case RC_EFF_EXP_MIDDLE_GAGE:
	{
		if (m_pCharacterInfo)
		{
			m_pCharacterInfo->SetAllBodyConditionMatrix(m_mMatrix);
		}
	}
	break;
	default:
	{
		if (m_pCharacterInfo)
		{
			m_pCharacterInfo->SetAllBodyConditionMatrix(m_mMatrix);
			m_pCharacterInfo->Tick(fElapsedTime);
		}
		if (m_pCharacterInfo)
		{
			if (!m_pCharacterInfo->IsUsing())// && !(m_nType/1000000 == 7))
			{
				m_bUsing = CheckItemPoint();

				//m_bUsing = FALSE;
			}
		}
		else
		{
			m_bUsing = FALSE;
		}
	}
	}

	// 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템
	if (m_bRepeat)
	{
		// 플레이 시간 감소
		m_fPlayTime -= fElapsedTime;

		// 플레이 시간이 다 끝나면 무조건 이펙트를 삭제한다
		if (m_fPlayTime <= 0.0f)
#ifdef C_EPSODE4_UI_CHANGE_JSKIM
			// 2011-10-10 by jhahn EP4 트리거 시스템	화산재 이펙트 추가			
		{
			m_bUsing = FALSE;
			if (m_nType == 7514620)
			{
				g_pD3dApp->m_pFxSystem->SetingSandStromSurface(FALSE);

			}
		}
#else
			m_bUsing = FALSE;   
#endif
		//end 2011-10-10 by jhahn EP4 트리거 시스템	화산재 이펙트 추가		
		else
		{
			// 플레이 시간이 남아 있고
			// 이펙트 렌더랑이 끝났는데 플레이 카운트가 남았을 경우
			if (!m_bUsing && m_nPlayCount > 1)
			{
				// 자기 자신 이펙트를 복사해서 집어넣음으로써 이펙트를 반복 플레이 한다
				CAppEffectData* pEff = new CAppEffectData(m_nType, m_vPos);
				if (!pEff->m_bUsing)
				{
					util::del(pEff);
				}
				else
				{
					g_pD3dApp->m_pEffectList->AddChild(pEff);
					pEff->m_fPlayTime = m_fPlayTime + fElapsedTime;
					pEff->m_nPlayCount = m_nPlayCount - 1;
				}
			}
		}
	}
	// end 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템
}

void CAppEffectData::SetEffectTick(float fTick)
{
	FLOG("CAppEffectData::SetEffectTick(float fTick)");
	if (m_pCharacterInfo)
	{
		m_pCharacterInfo->SetSingleBodyConditionMatrix(BODYCON_FLY_MASK, m_mMatrix);
		set<BodyCond_t>::iterator it = m_pCharacterInfo->m_vecCurrentBodyCondition.begin();
		while (it != m_pCharacterInfo->m_vecCurrentBodyCondition.end())
		{
			BodyCond_t hyBody = *it;
			map<BodyCond_t, CBodyConditionInfo*>::iterator itBody = m_pCharacterInfo->m_mapBodyCondition.find(hyBody);
			if (itBody != m_pCharacterInfo->m_mapBodyCondition.end())
			{
				CBodyConditionInfo* pBodyInfo = itBody->second;
				vector<CEffectInfo*>::iterator itEffect = pBodyInfo->m_vecEffect.begin();
				while (itEffect != pBodyInfo->m_vecEffect.end())
				{
					CEffectInfo* pEffectInfo = *itEffect;
					switch (pEffectInfo->m_nEffectType)
					{
					case EFFECT_TYPE_OBJECT:
					{
						((CObjectAni*)pEffectInfo->m_pEffect)->m_fCurrentObjectAniTime = fTick;
					}
					break;
					case EFFECT_TYPE_SPRITE:
					{
						((CSpriteAni*)pEffectInfo->m_pEffect)->m_fCurrentTick = fTick;
					}
					break;
					case EFFECT_TYPE_PARTICLE:
					{
						((CParticleSystem*)pEffectInfo->m_pEffect)->m_fCurrentTick = fTick;
					}
					break;
					}
					itEffect++;
				}
			}
			it++;
		}
	}

}

void CAppEffectData::LoadCharacterEffect(unsigned FileNum)
{
	char buf[256];
	wsprintf(buf, "%08d", FileNum);
	LoadCharacterEffect(buf);
}

void CAppEffectData::LoadCharacterEffect(char* strFileName)
{
	FLOG("CAppEffectData::LoadCharacterEffect(char* strFileName)");
	if (m_pCharacterInfo)
	{
		m_pCharacterInfo->InvalidateDeviceObjects();
		m_pCharacterInfo->DeleteDeviceObjects();
		util::del(m_pCharacterInfo);
	}
	m_pCharacterInfo = new CCharacterInfo();
	if (m_pCharacterInfo->Load(strFileName))
	{
		m_pCharacterInfo->InitDeviceObjects();
		m_pCharacterInfo->RestoreDeviceObjects();
	}
	else
	{
		util::del(m_pCharacterInfo);
		//		DBGOUT("CAppEffectData::LoadCharacterEffect,캐릭터 파일이 없습니다.\n");
	}
}

void CAppEffectData::SetHeight()
{
	FLOG("CAppEffectData::SetHeight()");
	FLOAT fDist;
	int i, j;
	i = (int)(m_vPos.x / TILE_SIZE);
	j = (int)(m_vPos.z / TILE_SIZE);
	D3DXVECTOR3 v, dir;
	dir = D3DXVECTOR3(0, -1, 0);
	if (i >= 0 && i < g_pGround->m_projectInfo.sXSize && j >= 0 && j < g_pGround->m_projectInfo.sYSize)
	{
		if (g_pGround->IntersectTriangle(m_vPos, dir, g_pGround->m_pTileVertexArray[i*(g_pGround->m_projectInfo.sYSize + 1) + j].pos,
			g_pGround->m_pTileVertexArray[i*(g_pGround->m_projectInfo.sYSize + 1) + (j + 1)].pos,
			g_pGround->m_pTileVertexArray[(i + 1)*(g_pGround->m_projectInfo.sYSize + 1) + (j)].pos,
			&fDist))
		{
			v = m_vPos + dir*fDist;
			m_vPos.y = v.y + 1.0f;
		}
		else if (g_pGround->IntersectTriangle(m_vPos, dir, g_pGround->m_pTileVertexArray[i*(g_pGround->m_projectInfo.sYSize + 1) + j + 1].pos,
			g_pGround->m_pTileVertexArray[(i + 1)*(g_pGround->m_projectInfo.sYSize + 1) + (j)].pos,
			g_pGround->m_pTileVertexArray[(i + 1)*(g_pGround->m_projectInfo.sYSize + 1) + j + 1].pos,
			&fDist))
		{
			v = m_vPos + dir*fDist;
			m_vPos.y = v.y + 1.0f;
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			CAppEffectData::CheckItemPoint()
/// \brief		순간 이펙트 중 장착 아이템에 대한 바디 컨디션(유닛별)을 세팅한다.
/// \author		dhkwon
/// \date		2004-04-27 ~ 2004-04-27
/// \warning	순간이펙트는 Tick중 FALSE가 리턴되면 지워지는 경향이 있는데
///				장착 아이템에 있어서는 그렇지 않게 된다.(return TRUE)
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CAppEffectData::CheckItemPoint()
{
	FLOG("CAppEffectData::CheckItemPoint()");
	if (m_pParent)
	{
		if (m_nType / 1000000 == 7)
		{
			if (m_nType / 100000 == 71)
			{
				int nNum = m_nType - 100000;
				ITEM * item = g_pDatabase->GetServerItemInfo(nNum);
				if (item)
				{
					if (IS_PRIMARY_WEAPON_1(item->Kind))
					{
						if (((CAtumData *)m_pParent)->m_pWeapon1_1_1 == this)
						{
							//((CAtumData *)m_pParent)->m_pWeapon1_1 = NULL;
							DBGOUT_EFFECT("Non Existent Wear Item BodyCondition (1-1Type)\n");//자신의 장착 아이템 바디컨디션 없음(1-1형)\n
							return TRUE;
						}
					}
					else if (IS_PRIMARY_WEAPON_2(item->Kind))
					{
						if (((CAtumData *)m_pParent)->m_pWeapon1_2 == this)
						{
							//((CAtumData *)m_pParent)->m_pWeapon1_2 = NULL;
							DBGOUT_EFFECT("Non Existent Wear Item BodyCondition (1-2Type)\n");//자신의 장착 아이템 바디컨디션 없음(1-2형)\n
							return TRUE;
						}
					}
				}
			}
			else if (m_nType / 100000 == 72)
			{
				int nNum = m_nType + 700000;
				ITEM * item = g_pDatabase->GetServerItemInfo(nNum);
				if (item)
				{
					if (IS_SECONDARY_WEAPON_1(item->Kind))
					{
						if (((CAtumData *)m_pParent)->m_pWeapon2_1_1 == this)
						{
							((CAtumData *)m_pParent)->m_pWeapon2_1_1 = NULL;
							DBGOUT_EFFECT("Non Existent Wear Item BodyCondition (2-1type)\n");
							return TRUE;
						}
					}
					else if (IS_SECONDARY_WEAPON_2(item->Kind))
					{
						if (((CAtumData *)m_pParent)->m_pWeapon2_2 == this)
						{
							((CAtumData *)m_pParent)->m_pWeapon2_2 = NULL;
							DBGOUT_EFFECT("Non Existent Wear Item BodyCondition (2-2type)\n");
							return TRUE;
						}
					}
				}
			}
			else
			{
				if (m_pParent->m_dwPartType == _SHUTTLE)
				{
					CItemInfo* pItemInfo = g_pStoreData->FindItemInInventoryByWindowPos(POS_REAR);
					if (pItemInfo && ((CAtumData *)m_pParent)->m_pEngine == this)
					{
						DBGOUT_EFFECT("Shuttle : Non Existent Wear Item BodyCondition (Engine)\n");
						return TRUE;
					}
					pItemInfo = g_pStoreData->FindItemInInventoryByWindowPos(POS_PROW);
					if (pItemInfo && ((CAtumData *)m_pParent)->m_pRadar == this)
					{
						DBGOUT_EFFECT("Shuttle : Non Existent Wear Item BodyCondition (Rader)\n");
						return TRUE;
					}
					//					pItemInfo = g_pStoreData->FindItemInInventoryByWindowPos(POS_CENTER);
					//					if(pItemInfo && ((CAtumData *)m_pParent)->m_pArmor == this )
					//					{
					//						DBGOUT_EFFECT("자신의 장착 아이템 바디컨디션 없음(아머)\n");
					//						return TRUE;
					//					}
					pItemInfo = g_pStoreData->FindItemInInventoryByWindowPos(POS_ACCESSORY_UNLIMITED);
					if (pItemInfo && ((CAtumData *)m_pParent)->m_pContainer == this)
					{
						DBGOUT_EFFECT("Shuttle : Non Existent Wear Item BodyCondition (POS_ACCESSORY_UNLIMITED)\n");
						return TRUE;
					}
				}
				else if (m_pParent->m_dwPartType == _ENEMY)
				{
					//CItemInfo* pItemInfo = g_pStoreData->FindItemInInventoryByWindowPos(POS_REAR);
					ITEM* pITEM = g_pDatabase->GetServerItemInfo(((CEnemyData *)m_pParent)->m_infoCharacter.CharacterRenderInfo.RI_Rear);
					//					if(pITEM && ((CEnemyData *)m_pParent)->m_infoCharacter.CharacterRenderInfo.RI_Rear == m_nType )
					if (pITEM && pITEM->SourceIndex == m_nType)
					{
						DBGOUT_EFFECT("ENEMY : Non Existent Wear Item BodyCondition (Engine)\n");
						return TRUE;
					}
					//pItemInfo = g_pStoreData->FindItemInInventoryByWindowPos(POS_PROW);



					// 2009. 08. 27 by ckPark 그래픽 리소스 변경 시스템 구현
					//pITEM = g_pDatabase->GetServerItemInfo(((CEnemyData *)m_pParent)->m_infoCharacter.CharacterRenderInfo.RI_Prow);

					// 외형 아이템이 있다면 그것으로 생성
					int nItemNum = 0;
					if (((CEnemyData *)m_pParent)->m_infoCharacter.CharacterRenderInfo.RI_Prow_ShapeItemNum)
						nItemNum = ((CEnemyData *)m_pParent)->m_infoCharacter.CharacterRenderInfo.RI_Prow_ShapeItemNum;
					else
						nItemNum = ((CEnemyData *)m_pParent)->m_infoCharacter.CharacterRenderInfo.RI_Prow;

					pITEM = g_pDatabase->GetServerItemInfo(nItemNum);
					// end 2009. 08. 27 by ckPark 그래픽 리소스 변경 시스템 구현



					//					if(pITEM && ((CEnemyData *)m_pParent)->m_infoCharacter.CharacterRenderInfo.RI_Prow+500000 == m_nType )
					if (pITEM && pITEM->SourceIndex + 500000 == m_nType)
					{
						DBGOUT_EFFECT("ENEMY : Non Existent Wear Item BodyCondition (Rader)\n");
						return TRUE;
					}
					//pItemInfo = g_pStoreData->FindItemInInventoryByWindowPos(POS_CENTER);
					//if(pItemInfo && ((CEnemyData *)m_pParent)->m_infoCharacter.CharacterRenderInfo.RI_Center == m_nType )
					//{
					//	DBGOUT_EFFECT("ENEMY : Non Existent Wear Item BodyCondition (POS_CENTER)\n");
					//	return TRUE;
					//}
					//pItemInfo = g_pStoreData->FindItemInInventoryByWindowPos(POS_ATTACHMENT);
					pITEM = g_pDatabase->GetServerItemInfo(((CEnemyData *)m_pParent)->m_infoCharacter.CharacterRenderInfo.RI_AccessoryUnLimited);
					//					if(pITEM && ((CEnemyData *)m_pParent)->m_infoCharacter.CharacterRenderInfo.RI_Attachment == m_nType )
					if (pITEM && pITEM->SourceIndex == m_nType)
					{
						DBGOUT_EFFECT("ENEMY : Non Existent Wear Item BodyCondition (POS_ACCESSORY_UNLIMITED)\n");
						return TRUE;
					}
				}
				// 2009. 09. 21 by ckPark 인피니티 필드 몬스터 스킬 구현
				else if ((m_pParent->m_dwPartType == _MONSTER))
				{
					return TRUE;
				}
				// end 2009. 09. 21 by ckPark 인피니티 필드 몬스터 스킬 구현


				//				if(m_pParent->m_dwPartType == _SHUTTLE)
				//				{
				//					map<int, CItemInfo*>::iterator it = NULL;
				//					it = g_pStoreData->m_mapItemInInventory.find(POS_REAR);
				//					if(it != g_pStoreData->m_mapItemInInventory.end())
				//					{
				//						if(((CAtumData *)m_pParent)->m_pEngine == this)
				//						{
				//							//((CAtumData *)m_pParent)->m_pEngine = NULL;
				//							DBGOUT_EFFECT("자신의 장착 아이템 바디컨디션 없음(엔진)\n");
				//							return TRUE;
				//						}
				//					}
				//					it = g_pStoreData->m_mapItemInInventory.find(POS_PROW);
				//					if(it != g_pStoreData->m_mapItemInInventory.end())
				//					{
				//						if(((CAtumData *)m_pParent)->m_pRadar == this)
				//						{
				//							//((CAtumData *)m_pParent)->m_pRadar = NULL;
				//							DBGOUT_EFFECT("자신의 장착 아이템 바디컨디션 없음(레이다)\n");
				//							return TRUE;
				//						}
				//					}
				//					it = g_pStoreData->m_mapItemInInventory.find(POS_CENTER);
				//					if(it != g_pStoreData->m_mapItemInInventory.end())
				//					{
				//						if(((CAtumData *)m_pParent)->m_pArmor == this)
				//						{
				//							//((CAtumData *)m_pParent)->m_pArmor = NULL;
				//							DBGOUT_EFFECT("자신의 장착 아이템 바디컨디션 없음(아머)\n");
				//							return TRUE;
				//						}
				//					}
				//					it = g_pStoreData->m_mapItemInInventory.find(POS_ATTACHMENT);
				//					if(it != g_pStoreData->m_mapItemInInventory.end())
				//					{
				//						if(((CAtumData *)m_pParent)->m_pContainer == this)
				//						{
				//							//((CAtumData *)m_pParent)->m_pContainer = NULL;
				//							DBGOUT_EFFECT("자신의 장착 아이템 바디컨디션 없음(부착)\n");
				//							return TRUE;
				//						}
				//					}
				//				}
				//				else if(m_pParent->m_dwPartType == _ENEMY)
				//				{
				//					if(((CEnemyData *)m_pParent)->m_CharacterRenderInfo.RI_Rear == m_nType)
				//					{
				//						if(((CAtumData *)m_pParent)->m_pEngine == this)
				//						{
				//							//((CAtumData *)m_pParent)->m_pEngine = NULL;
				//							DBGOUT_EFFECT("타인의 장착 아이템 바디컨디션 없음(엔진)\n");
				//							return TRUE;
				//						}
				//					}
				//					// 선두
				//					if(((CEnemyData *)m_pParent)->m_CharacterRenderInfo.RI_Prow == m_nType)
				//					{
				//						if(((CAtumData *)m_pParent)->m_pRadar == this)
				//						{
				//							//((CAtumData *)m_pParent)->m_pRadar = NULL;
				//							DBGOUT_EFFECT("타인의 장착 아이템 바디컨디션 없음(레이더)\n");
				//							return TRUE;
				//						}
				//					}
				//					// 방어
				//					if(((CEnemyData *)m_pParent)->m_CharacterRenderInfo.RI_Center == m_nType)
				//					{
				//						if(((CAtumData *)m_pParent)->m_pArmor == this)
				//						{
				//							//((CAtumData *)m_pParent)->m_pArmor = NULL;
				//							DBGOUT_EFFECT("타인의 장착 아이템 바디컨디션 없음(아머)\n");
				//							return TRUE;
				//						}
				//					}
				//					// 컨테이너 (연료통)
				//					if(((CEnemyData *)m_pParent)->m_CharacterRenderInfo.RI_Attachment == m_nType)
				//					{
				//						if(((CAtumData *)m_pParent)->m_pContainer == this)
				//						{
				//							//((CAtumData *)m_pParent)->m_pContainer = NULL;
				//							DBGOUT_EFFECT("타인의 장착 아이템 바디컨디션 없음(부착물)\n");
				//							return TRUE;
				//						}
				//					}
				//				}
			}
			DBGOUT("AppEffectData delete character [%s]\n", m_pCharacterInfo->m_strCharacterFileName);
		}
		else if (m_nType / 10000000 == 1)
		{
			int nNum = m_nType % 1000000;
			nNum = nNum / 100000;
			if (nNum == 1)
			{
				if (((CAtumData *)m_pParent)->m_pWeapon1_1_1 == this)
				{
					//((CAtumData *)m_pParent)->m_pWeapon1_1 = NULL;
					DBGOUT_EFFECT("Shuttle : Non Existent Wear Item BodyCondition (1-1Type)\n");
					return TRUE;
				}
				//((CAtumData *)m_pParent)->m_pWeapon1_1 = NULL;
			}
			else if (nNum == 2)
			{
				if (((CAtumData *)m_pParent)->m_pWeapon1_2 == this)
				{
					//((CAtumData *)m_pParent)->m_pWeapon1_1 = NULL;
					DBGOUT_EFFECT("Shuttle : Non Existent Wear Item BodyCondition (1-2Type)\n");
					return TRUE;
				}
				//((CAtumData *)m_pParent)->m_pWeapon1_2 = NULL;
			}
			else if (nNum == 3)
			{
				if (((CAtumData *)m_pParent)->m_pWeapon2_1_1 == this)
				{
					//((CAtumData *)m_pParent)->m_pWeapon1_1 = NULL;
					DBGOUT_EFFECT("Shuttle : Non Existent Wear Item BodyCondition (2-1Type)\n");
					return TRUE;
				}
				//((CAtumData *)m_pParent)->m_pWeapon2_1 = NULL;
			}
			else if (nNum == 4)
			{
				if (((CAtumData *)m_pParent)->m_pWeapon2_2 == this)
				{
					//((CAtumData *)m_pParent)->m_pWeapon1_1 = NULL;
					DBGOUT_EFFECT("Shuttle : Non Existent Wear Item BodyCondition (2-2Type)\n");
					return TRUE;
				}
				//((CAtumData *)m_pParent)->m_pWeapon2_2 = NULL;
			}
			//DBGOUT("AppEffectData delete character [%s][Type:%d][weapon:%d(1:1_1,2:1_2,3:2_1,4:2_2)]\n",m_pCharacterInfo->m_strCharacterFileName,m_nType,nNum);
		}
	}
	return FALSE;
}

void CAppEffectData::ChangeBodyCondition(BodyCond_t hyBody)
{
	// 2013-04-15 by bhsohn 한국 Exception 오류 요소 안전코드 추가
	if (!m_pCharacterInfo)
	{
		return;
	}
	// END 2013-04-15 by bhsohn 한국 Exception 오류 요소 안전코드 추가

	ASSERT_ASSERT(m_pCharacterInfo);
	m_bodyCondition = hyBody;
	m_pCharacterInfo->ChangeBodyCondition(hyBody); // 2016-01-08 exception at this point

}

void CAppEffectData::ResetBodyCondition(BodyCond_t hyBody)
{
	// 2013-04-15 by bhsohn 한국 Exception 오류 요소 안전코드 추가
	if (!m_pCharacterInfo)
	{
		return;
	}
	// END 2013-04-15 by bhsohn 한국 Exception 오류 요소 안전코드 추가

	ASSERT_ASSERT(m_pCharacterInfo);
	m_bodyCondition = hyBody;
	m_pCharacterInfo->ResetBodyCondition(hyBody);

}
// 2010-06-15 by shcho&hslee 펫시스템 - 마을 이동 처리
void CAppEffectData::SetPetPos(D3DXVECTOR3 CharPos, D3DXVECTOR3 PetPos, float fElapsedTime)
{
	D3DXVECTOR3 vecMove;
	D3DXVec3Normalize(&vecMove, &(CharPos - PetPos));
	if (m_fOldDistance <= D3DXVec3Length(&(CharPos - PetPos)))
	{
		m_fPetAddSpeed = -(MINSPEED + ((MINDISTANCE - D3DXVec3Length(&(CharPos - PetPos))) * (MINSPEED / MINDISTANCE)));
	}
	m_vPos.x = PetPos.x + vecMove.x * ((m_fPetSpeed + m_fPetAddSpeed)* fElapsedTime);
	m_vPos.z = PetPos.z + vecMove.z * ((m_fPetSpeed + m_fPetAddSpeed)* fElapsedTime);
	m_vPos.y = CharPos.y;
}
// end 2010-06-15 by shcho&hslee 펫시스템 - 마을 이동 처리.
