// WeaponMissileData.cpp: implementation of the CWeaponMissileData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "WeaponMissileData.h"
#include "AtumApplication.h"
#include "Cinema.h"	
#include "EnemyData.h"
#include "MonsterData.h"	
#include "SceneData.h"
#include "ShuttleChild.h"
#include "FieldWinSocket.h"
#include "ItemData.h"
//#include "ObjectRender.h"
#include "Background.h"
#include "ObjRender.h"
#include "dxutil.h"
#include "ItemInfo.h"
#include "StoreData.h" // 2010-06-15 by shcho&hslee 펫시스템 - 오퍼레이터 구현	
#include "Interface.h" // 2010-06-15 by shcho&hslee 펫시스템 - 오퍼레이터 구현	
#include "INFOpMain.h" // 2010-06-15 by shcho&hslee 펫시스템 - 오퍼레이터 구현	
#include "INFOpInfo.h" // 2010-06-15 by shcho&hslee 펫시스템 - 오퍼레이터 구현	
#include "AtumDatabase.h"		// 2013-03-13 by bhsohn 스킬 번호 구조 수정

#ifdef COLLCHECKLAG
extern int g_nCheckCollisionState;
#endif

///////////////////////////////////////////////////////////////////////
///		스킬 관련 정의문
///		jschoi
///		2006.06.26
///////////////////////////////////////////////////////////////////////

#define TARGET_ON		1
#define TARGET_OFF		2

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#define MISSILE_SPEED	100.0f
#ifdef _DEBUG
extern int		g_nMissileCount;
#endif
// 2009. 11. 23 by jskim 리소스 로딩 구조 변경
// 2009. 08. 27 by ckPark 그래픽 리소스 변경 시스템 구현
// CWeaponMissileData::CWeaponMissileData(CAtumData * pAttack, 
// 									 ITEM * pWeaponITEM, 
// 									 ATTACK_DATA & attackData)

// CWeaponMissileData::CWeaponMissileData( CAtumData * pAttack,
// 									    ITEM * pWeaponITEM,
// 										ATTACK_DATA & attackData,
// 										ITEM* pEffectItem /* = NULL */ )
// end 2009. 08. 27 by ckPark 그래픽 리소스 변경 시스템 구현
CWeaponMissileData::CWeaponMissileData( CAtumData * pAttack,
									    ITEM * pWeaponITEM,
 										ATTACK_DATA & attackData,
										ITEM* pEffectItem, /* = NULL */
										int LoadingPriority )
//end 2009. 11. 23 by jskim 리소스 로딩 구조 변경
{
	// 2009. 11. 23 by jskim 리소스 로딩 구조 변경
	m_LoadingPriority = LoadingPriority;
	//end 2009. 11. 23 by jskim 리소스 로딩 구조 변경
	m_pCharacterInfo = NULL;
	
	// 2005-08-16 by ispark
//	m_fWeaponLifeTime = 8.0f;
	m_fWeaponLifeTime = 0.0f;
	m_nSkillNum = attackData.AttackData.SkillNum;
	m_fExplosionRange = attackData.fExplosionRange;
	m_pTarget = (CAtumData*)g_pScene->FindUnitDataByClientIndex( attackData.AttackData.TargetInfo.TargetIndex );
	m_pAttacker = pAttack;
	m_nWeaponIndex = attackData.AttackData.WeaponIndex;
	m_nClientIndex = attackData.AttackData.AttackIndex;
	m_nTargetIndex = attackData.AttackData.TargetInfo.TargetIndex;
	m_nTargetItemFieldIndex = attackData.AttackData.TargetInfo.TargetItemFieldIndex;
	// 2011. 03. 08 by jskim 인피3차 구현 - 넌 타겟 시스템
	m_nMultiTargetIndex = attackData.AttackData.TargetInfo.MultiTargetIndex;
	// end 2011. 03. 08 by jskim 인피3차 구현 - 넌 타겟 시스템
	m_bWeaponFlyType = attackData.bZigZagWeapon;
	m_vFirePos = A2DX(attackData.AttackData.FirePosition);
	m_pItemData = pWeaponITEM;

	// 2009. 08. 27 by ckPark 그래픽 리소스 변경 시스템 구현
	m_pEffectItem = pEffectItem;
	// end 2009. 08. 27 by ckPark 그래픽 리소스 변경 시스템 구현

	// 2007-06-15 by dgwoo 인첸트까지된 무기 속도.
	m_fWarheadSpeed = attackData.fWarheadSpeed;
//	if(pMsg->Distance - 128 >= 0)
//		m_bWeaponFlyType = 0;
//	else
//		m_bWeaponFlyType = 1;
	m_pCinema = NULL;
	SetShuttleChildOrderTarget();
	InitData();
//	g_pShuttleChild->m_fMissileFireTime += 0.1f;
//	m_fFireTime = g_pShuttleChild->m_fMissileFireTime;
//	if(m_fFireTime <= 0 && m_fFireTime > 1)
//	{
//		InitData();
//		m_fFireTime = 0;
//	}
	// 2005-11-25 by ispark, 지상폭격이면 타겟 중심에서 반경 50에 랜덤
//	if(m_nSkillNum != 0)
	if(SKILL_BASE_NUM(m_nSkillNum) == BGEAR_SKILL_BASENUM_GROUNDBOMBINGMODE)
	{
		m_vTargetPos = A2DX(attackData.AttackData.TargetInfo.TargetPosition);
		m_vTargetPos.x += (float)RANDI(-50, 50);
		m_vTargetPos.z += (float)RANDI(-50, 50);
	}
	// 2006-12-01 by ispark, 공중 폭격
	if(SKILL_BASE_NUM(m_nSkillNum) == BGEAR_SKILL_BASENUM_AIRBOMBINGMODE)
	{
		m_vTargetPos = A2DX(attackData.AttackData.TargetInfo.TargetPosition);
		m_vTargetPos.x += (float)RANDI(-50, 50);
		m_vTargetPos.y += (float)RANDI(-50, 50);
		m_vTargetPos.z += (float)RANDI(-50, 50);
	}

	// 2005-07-19 by ispark
	// 나를 향한 타겟이라면 타겟을 저장한다. 이후에 삭제시 확인을 위해서이다. 
	if(attackData.AttackData.TargetInfo.TargetIndex == g_pShuttleChild->m_myShuttleInfo.ClientIndex)
	{
		m_nTargetMe = g_pShuttleChild->m_myShuttleInfo.ClientIndex;
		g_pShuttleChild->SetMissileWarning(TRUE);
		g_pShuttleChild->SetMissileCount(g_pShuttleChild->GetMissileCount() + 1);

		// 2010-06-15 by shcho&hslee 펫시스템 - 오퍼레이터 구현	
		if( g_pStoreData->FindItemInInventoryByWindowPos( POS_PET ) )
		{
			g_pInterface->m_pINFOpMain->GetOpInfo()->SetOperatorAction(1,2);
		}
		// end 2010-06-15 by shcho&hslee 펫시스템 - 오퍼레이터 구현

	}


	// 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템
	m_nDelegateClientIdx	= attackData.AttackData.DelegateClientIdx;
	// end 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템

#ifdef COLLCHECKLAG
	m_fLastCollisionDistance = DEFAULT_COLLISION_DISTANCE;
	m_vLastVelocity = { 0, 0, 0 };
#endif

#ifdef _DEBUG
	g_nMissileCount = g_pShuttleChild->GetMissileCount();
#endif
}


/*
CWeaponMissileData::CWeaponMissileData(CAtumData * pAttack,
									   CAtumData * pTarget,
									   MSG_FC_BATTLE_ATTACK_RESULT_SECONDARY* pMsg)//float fDist,int nWeaponIndex, int nClientIndex,D3DXVECTOR3 vTargetPos);
{
	FLOG( "CWeaponMissileData(CAtumData * pAttack,CAtumData * pTarget,MSG_FC_BATTLE_ATTACK_RESULT_SECONDARY* pMsg)" );
	m_pCharacterInfo = NULL;
	
	m_fWeaponLifeTime = 8.0f;
	m_pTarget = pTarget;
	m_pAttacker = pAttack;
	m_nWeaponIndex = pMsg->WeaponIndex;
	m_nClientIndex = pMsg->AttackIndex;
	m_nTargetIndex = pMsg->TargetIndex;
	m_nTargetItemFieldIndex = -1;
	if(pMsg->Distance - 128 >= 0)
		m_bWeaponFlyType = 0;
	else
		m_bWeaponFlyType = 1;
	m_pCinema = NULL;
	SetShuttleChildOrderTarget();
}

CWeaponMissileData::CWeaponMissileData(CAtumData * pAttack,
									   CAtumData * pTarget,
									   MSG_FC_BATTLE_ATTACK_ITEM_RESULT_SECONDARY* pMsg)//float fDist,int nWeaponIndex, int nClientIndex,D3DXVECTOR3 vTargetPos);
{
	FLOG( "CWeaponMissileData(CAtumData * pAttack,CAtumData * pTarget,MSG_FC_BATTLE_ATTACK_ITEM_RESULT_SECONDARY* pMsg)" );
	m_pCharacterInfo = NULL;
	
	m_fWeaponLifeTime = 8.0f;
	m_pTarget = pTarget;
	m_pAttacker = pAttack;
	m_nWeaponIndex = pMsg->WeaponIndex;
	m_nClientIndex = pMsg->AttackIndex;
	m_nTargetIndex = pMsg->TargetIndex;
	m_nTargetItemFieldIndex = pMsg->TargetItemFieldIndex;
	if(pMsg->Distance - 128 >= 0)
		m_bWeaponFlyType = 0;
	else
		m_bWeaponFlyType = 1;
	SetShuttleChildOrderTarget();
}
*/
CWeaponMissileData::~CWeaponMissileData()
{
	FLOG( "~CWeaponMissileData()" );

	if(m_pCharacterInfo)//추가
	{
		m_pCharacterInfo->InvalidateDeviceObjects();
		m_pCharacterInfo->DeleteDeviceObjects();
		util::del(m_pCharacterInfo);
	}
	util::del( m_pCinema );

	// 2005-07-11 by ispark
	// 미사일 경고 
	// 미사일 갯수를 하나씩 삭제를 한다.
	// 만약에 갯수가 마이너스라면 0으로 초기화 한다.
	// 1번 파괴 명령없이 사라지는 경우다.
	CheckTargetWarning();

#ifdef _DEBUG
	if(g_pShuttleChild)
		g_nMissileCount = g_pShuttleChild->GetMissileCount();
#endif

}

void CWeaponMissileData::InitData()
{
	FLOG( "CWeaponMissileData::InitData()" );
	if(!m_pItemData)
	{
		m_bUsing = FALSE;
		return;
	}

	D3DXVECTOR3 vVel = m_pAttacker->m_vVel;
	m_vUp = m_pAttacker->m_vUp;
	D3DXVECTOR3 vSide;
	D3DXVec3Cross(&vSide,&vVel,&m_vUp);
	D3DXVec3Normalize(&vSide,&vSide);

	if(m_bWeaponFlyType == 0)
	{
		m_vPos = m_pAttacker->m_vLWSecondaryPos + m_vFirePos.x*vSide/4;
//		m_vPos = m_pAttacker->m_vLWSecondaryPos + m_vFirePos.x*vSide;
	}
	else
	{
		m_vPos = m_pAttacker->m_vRWSecondaryPos + m_vFirePos.x*vSide/4;
//		m_vPos = m_pAttacker->m_vRWSecondaryPos + m_vFirePos.x*vSide;
	}

	m_vVel = m_pAttacker->m_vVel;
	m_vOriPos = m_vPos;
	D3DXVec3Normalize(&m_vVel,&m_vVel);
	m_dwWeaponState = _NORMAL;
	m_fWeaponSpeed = MISSILE_SPEED;
	m_bSetTarget = FALSE;

	D3DXMatrixLookAtLH( &m_mMatrix, &(m_vPos), &(m_vPos + m_vVel), &m_vUp);
	D3DXMatrixInverse( &m_mMatrix, NULL, &m_mMatrix );
	char buf[256];
	memset(buf,0x00,sizeof(buf));
	
	// 2009. 08. 27 by ckPark 그래픽 리소스 변경 시스템 구현
	//wsprintf(buf,"%08d",m_pItemData->SourceIndex);								// 2005-08-23 by ispark

	// 이펙트 아이템이 있다면 그것으로 생성
	if( m_pEffectItem )
		wsprintf( buf, "%08d", m_pEffectItem->SourceIndex );
	else
		wsprintf( buf, "%08d", m_pItemData->SourceIndex );
	
	// end 2009. 08. 27 by ckPark 그래픽 리소스 변경 시스템 구현

	LoadCharacterEffect(buf);
//	m_bodyCondition = BODYCON_BULLET_MASK;
	if(m_pCharacterInfo)
	{
		// 2009. 11. 23 by jskim 리소스 로딩 구조 변경
		m_pCharacterInfo->m_LoadingPriority = m_LoadingPriority;
		//end 2009. 11. 23 by jskim 리소스 로딩 구조 변경
		m_pCharacterInfo->SetAllBodyConditionMatrix(m_mMatrix);
//		m_pCharacterInfo->ChangeBodyCondition(m_bodyCondition);
		m_pCharacterInfo->ChangeBodyCondition(BODYCON_BULLET_MASK);
		// 2004-10-12 by jschoi
		if(m_nSkillNum != 0)
		{
			ChangeBodyConditionForSkillEffect(m_nSkillNum,BODYCON_BULLET_MASK);
		}
	}
	else
	{
		// temporary item
		LoadCharacterEffect("01200016");
		if(m_pCharacterInfo)
		{
			// 2009. 11. 23 by jskim 리소스 로딩 구조 변경
			m_pCharacterInfo->m_LoadingPriority = m_LoadingPriority;
			//end 2009. 11. 23 by jskim 리소스 로딩 구조 변경
			m_pCharacterInfo->SetAllBodyConditionMatrix(m_mMatrix);
			m_pCharacterInfo->ChangeBodyCondition(BODYCON_BULLET_MASK);
			if(m_nSkillNum != 0)
			{
				ChangeBodyConditionForSkillEffect(m_nSkillNum,BODYCON_BULLET_MASK);
			}
		}
		else
		{
			g_pD3dApp->NetworkErrorMsgBox(STRMSG_C_050512_0001);
			return;
		}		
	}
	m_dwPartType = _MISSILE;

//  시네마 포인트 셋팅/////////////////////////////////////////////////////////////////
	char str[32];
//	sprintf(str,"%08d",m_pItemData->ItemNum);
	sprintf(str,"%08d",m_pItemData->CameraPattern);
	util::del(m_pCinema);
	m_pCinema = g_pScene->LoadCinemaData(str, m_pItemData->CameraPattern);
	if(m_pCinema == NULL)
	{
		#ifdef _DEBUG
			DBGOUT( "Cinema : Can't Find Pattern(Cinema) Files(%s)\n",str);
		#endif //_DEBUG_endif
//		m_pCinema = g_pScene->LoadCinemaData(PATTERN_DEFAULT);
		if(m_pCinema == NULL)
		{
			g_pD3dApp->NetworkErrorMsgBox(STRMSG_C_050513_0001);
			return;
		}
	}
	EVENT_POINT ep;
	ep.vPosition = m_vPos;
//	ep.vDirection = m_vVel;
//	ep.vTarget = m_vVel;

	// 2005-11-25 by ispark, 폭격이면 타겟이 없다. 그러나 타겟은 바닥이므로 랜덤으로 나가게 한다. 훨 멋있다.
	if(m_pTarget || m_nSkillNum != 0)
	{
		D3DXVECTOR3 vRandomVector;
		vRandomVector.x = (float)RANDF(-0.25, 0.25);
		vRandomVector.y = (float)RANDF(-0.25, 0.25);
		vRandomVector.z = (float)RANDF(-0.25, 0.25);
		ep.vDirection = m_vVel + vRandomVector;
		ep.vTarget = m_vVel + vRandomVector;
	}
	else
	{
		ep.vDirection = m_vVel;
		ep.vTarget = m_vVel;
	}
	ep.vUpVector = m_vUp;
	D3DXVec3Normalize(&ep.vDirection,&ep.vDirection);
	ep.fVelocity = m_fWeaponSpeed;	// m_fWeaponSpeed 초기 스피드
	ep.fCurvature = DEFAULT_CURVATURE;	// 초기값 셋팅
//	m_pCinema->InitCinemaData(ep,m_bWeaponFlyType == 0);

	// 2007-06-15 by dgwoo 아래 소스로 변경.
//	m_pCinema->InitWeaponCinemaData(ep,m_bWeaponFlyType == 0, m_pItemData->RepeatTime, m_pItemData->BoosterAngle);
	m_pCinema->InitWeaponCinemaData(ep,m_bWeaponFlyType == 0, m_fWarheadSpeed, m_pItemData->BoosterAngle);
//  초기화 종료 ///////////////////////////////////////////////////////////////////////
}

void CWeaponMissileData::Tick()
{
	FLOG( "CWeaponMissileData::Tick()" );
	float fElapsedTime = g_pD3dApp->GetElapsedTime();
//	if(m_fFireTime > 0)
//	{
//		m_fFireTime -= fElapsedTime;
//		if(m_fFireTime <= 0)
//		{
//			InitData();
//			m_fFireTime = 0;
//		}
//		return;
//	}
	if(m_pAttacker == NULL)
	{
		m_bUsing = FALSE;
		return;
	}

// 2005-11-25 by ispark, 지상폭격, 공중폭격
	if(m_nSkillNum != 0)
	{
		SkillTick();
		// SKILL EFFECT
		SetBodyConditionMatrixForSkillEffect(m_bodyCondition,m_mMatrix);
		return;
	}

	m_vOldPos = m_vPos;
//	D3DXVECTOR3 vVel;
//	D3DXVECTOR3 vSide,vUpTemp;
//	D3DXMATRIX mat;
//	D3DXVECTOR3 vUp(0,1,0);

	// 패턴 타켓방향 관련 2004-07-27 jschoi
	int nPatternType = m_pCinema->GetHeader().nPatternType;
	D3DXVECTOR3 vTargetPosition;

	if( m_dwWeaponState == _EXPLODING ) // exploding state
	{
		if(m_pTarget)
		{
			if(!((CUnitData*)m_pTarget)->m_bShielding)
			{
				// 랜덤하게 위치를 조정한다.
				D3DXVECTOR3 vVel;
				D3DXVec3Normalize(&vVel, &(m_pAttacker->m_vPos - m_pTarget->m_vPos));
				vVel = D3DXVECTOR3( vVel.x*((float)RANDI(0, 4)),vVel.y*((float)RANDI(0, 4)),vVel.z*((float)RANDI(0, 4)));
				m_vPos += vVel;
			}
			else	// 쉴드 발동 중인경우 좌표는 쉴드크기 만큼으로 조정한다.
			{// 쉴드 발동 중일때 쉴드 Hit 이펙트 추가
				D3DXVECTOR3 vVel;
				D3DXVec3Normalize(&vVel, &(m_vPos - m_pTarget->m_vPos));
				m_vPos += vVel*SIZE_OF_SHIELD_EFFECT;
				((CUnitData*)m_pTarget)->CreateSecondaryShieldDamage(m_pAttacker->m_vPos);
			}

			// 2005-07-11 by ispark
			// 미사일 경고 
			// 미사일 갯수를 하나씩 삭제를 한다.
			// 만약에 갯수가 마이너스라면 0으로 초기화 한다.
			// 2번 파괴 명령이 내려지는 경우다.
			CheckTargetWarning();
		}
		SetBodyCondition(BODYCON_HIT_MASK);
		if(m_nSkillNum != 0)
		{
			ChangeBodyConditionForSkillEffect(m_nSkillNum,BODYCON_HIT_MASK);
		}

		m_dwWeaponState = _EXPLODED;
	} 
	else if(m_dwWeaponState == _NORMAL)
	{
		CItemData *pTargetItem = g_pScene->FindFieldItemByFieldIndex( m_nTargetItemFieldIndex );
//		if(!pTargetItem && m_pTarget)
//		{
//			pTargetItem = g_pScene->FindFieldItemByParent( m_pTarget );
//		}
		m_fWeaponLifeCheckTime += fElapsedTime;
		if(m_fWeaponLifeCheckTime > 0.1f) // target is set after 0.1 second
			m_bSetTarget = TRUE;

		// 2006-10-19 by ispark, 죽었을 때 타겟 경고 없애기
		if(m_pTarget == NULL ||
			(g_pD3dApp->m_pShuttleChild == m_pTarget && g_pShuttleChild->CheckUnitState() == BREAKDOWN))
		{
			CheckTargetWarning(); // 타겟이 없어지면 경고 메세지 삭제
		}
		// 2005-08-16 by ispark
//		if(m_fWeaponLifeTime <= m_fWeaponLifeCheckTime)
		// 몬스터가 쏜 미사일이면, 사거리를 강제로 증가
		float fMonWeapon = 0.0f;
		if(IS_SECONDARY_WEAPON_MONSTER(m_pItemData->Kind))
		{
			fMonWeapon = m_pItemData->Range * 2.0f;
		}
		
		if(m_fWeaponLifeCheckTime * m_fWeaponSpeed > m_pItemData->Range + fMonWeapon)
		{
			m_dwWeaponState = _EXPLODING;
			m_pTarget = NULL;
		}
		else
		{
			CheckTargetState();
			if(pTargetItem && pTargetItem->m_dwState == _NORMAL)
			{
				m_vTargetPos = pTargetItem->m_vPos;
			}
			else if(m_pTarget && ATTACK_AVAILABLE_STATE(m_pTarget->m_dwState))
			{
				// 2011. 03. 08 by jskim 인피3차 구현 - 넌 타겟 시스템
				//m_vTargetPos = m_pTarget->m_vPos;
				// 2011. 09. 28 by jskim 넌 타겟 시스템 버그 수정( 케스팅 문제 )
				//if(m_pTarget && ((CMonsterData*)m_pTarget)->m_vecvmultiData.size() > 0)
				if( m_pTarget && 
					m_pTarget->m_dwPartType == _MONSTER &&
					((CMonsterData*)m_pTarget)->m_vecvmultiData.size() > 0)
				// end 2011. 09. 28 by jskim 넌 타겟 시스템 버그 수정( 케스팅 문제 )
				{
					m_vTargetPos = ((CMonsterData*)m_pTarget)->GetMultiPos( m_nMultiTargetIndex );
				}
				else if(m_pTarget)
				{
					m_vTargetPos = m_pTarget->m_vPos;
				}
				// end 2011. 03. 08 by jskim 인피3차 구현 - 넌 타겟 시스템				
			}
			else
			{
				m_vTargetPos = D3DXVECTOR3(0,0,0);
				m_bSetTarget = FALSE;
			}

//			DBGOUT("%d. 타겟(%.0f, %.0f, %.0f) ", m_nWeaponIndex, m_vTargetPos.x, m_vTargetPos.y, m_vTargetPos.z);
//			DBGOUT("미사일(%.0f, %.0f, %.0f) ", m_vPos.x, m_vPos.y, m_vPos.z);
//			DBGOUT("거리 = %.0f\n", D3DXVec3Length(&(m_vPos - m_vTargetPos)));
			// MoveWeapon 대신 패턴 적용
			BOOL bResult;							// 목표점에 도달했는지 결과..
//			bResult=m_pCinema->Tick(m_vTargetPos);	// 결과가 FALSE 면 목표를 지났다.. 즉 도달했다.


			// 2010. 03. 18 by ckPark 인피니티 필드 2차(몬스터 스킬 추가)

			//bResult=m_pCinema->WeaponTick(m_vTargetPos, fElapsedTime);	// 결과가 FALSE 면 목표를 지났다.. 즉 도달했다.
			float fDecSpeed = 0.0f;
			if( m_pTarget && m_pTarget->m_dwPartType == _MONSTER )
			{
				ITEM* pSkillItem = ((CMonsterData*)(m_pTarget))->GetSkillItemWithDesParam( DES_BLIND );
				if( !pSkillItem || pSkillItem->Range < D3DXVec3Length( &(m_vPos - m_pTarget->m_vPos) ) )
					m_nBlindCumulate = m_nBlindSpeedDownTime = 0;
				else
				{
					DWORD nCurTime	= timeGetTime();
					if( nCurTime > m_nBlindSpeedDownTime + BLIND_INTERVAL )
					{
						m_nBlindCumulate++;
						m_nBlindSpeedDownTime = nCurTime;
					}

					if( m_nBlindCumulate )
						fDecSpeed = m_nBlindCumulate * pSkillItem->GetParameterValue( DES_BLIND );
				}
			}

			bResult=m_pCinema->WeaponTick( m_vTargetPos, fElapsedTime, fDecSpeed );

			// end 2010. 03. 18 by ckPark 인피니티 필드 2차(몬스터 스킬 추가)


			EVENT_POINT ep;
			ep=m_pCinema->GetCurrentCinemaPoint();
			m_vPos = ep.vPosition;			// 위치
			m_vVel = ep.vDirection;			// 방향
			m_fWeaponSpeed = ep.fVelocity;	// 속력
			//DBGOUT("미사일 속도 : %3.f\n",m_fWeaponSpeed);
			m_vUp = ep.vUpVector;
			vTargetPosition = ep.vTarget;
			if(bResult == FALSE)
			{
				nPatternType = TARGET_OFF;
			}

			// 내가 발사한 무기이거나 몬스터가 발사한 2형 무기일때 충돌 체크
			if(	m_nClientIndex == g_pShuttleChild->m_myShuttleInfo.ClientIndex ||

				// 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템
				// 몬스터 공격이면서 몬스터가 타겟이고 내가 위임받았다면 내가 충돌 처리를 한다
				( IS_SECONDARY_WEAPON_MONSTER(m_pItemData->Kind) &&
				  IS_MONSTER_CLIENT_INDEX( m_nTargetIndex ) &&
				  (m_nDelegateClientIdx == g_pShuttleChild->m_myShuttleInfo.ClientIndex) ) ||
				// end 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템

				(IS_SECONDARY_WEAPON_MONSTER(m_pItemData->Kind) &&
				m_nTargetIndex == g_pShuttleChild->m_myShuttleInfo.ClientIndex ))
			{
				float fMovingDistance = ep.fVelocity*fElapsedTime;
#ifndef COLLCHECKLAG
				CheckWeaponCollision(pTargetItem,fMovingDistance);
#else
				//if (g_nCheckCollisionState == 0)
					CheckWeaponCollision(pTargetItem,fMovingDistance);
#endif
				// 2006-12-08 by ispark, 레이더 사거리를 벗어나면 타겟을 없앰
				// 레이더 사거리의 1.5배
				//float fRadarRange = (float)g_pShuttleChild->m_pRadarItemInfo->ItemInfo->Range * 1.5f;
				if(m_pAttacker == g_pShuttleChild)
				{
					if(g_pShuttleChild->m_pRadarItemInfo)
					{
						float fRadarRange = CAtumSJ::GetSecondaryRadarRange(g_pShuttleChild->m_pRadarItemInfo->ItemInfo, &g_pShuttleChild->m_paramFactor) * 1.5f;
						if(D3DXVec3Length(&(m_pAttacker->m_vPos - m_vPos)) > fRadarRange)
						{ 
							m_pTarget = NULL;
						}
					}
					
				}
			}
			else if(m_nTargetIndex == g_pShuttleChild->m_myShuttleInfo.ClientIndex)
			{
				// 내가 타겟이고 내 화면에 보여할 부분
				if(m_pTarget)
				{
				D3DXVECTOR3 vTargetVel = m_pTarget->m_vPos - m_vPos;
					float fMovingDistance = m_fWeaponSpeed * fElapsedTime;

					if(D3DXVec3Length(&vTargetVel) < m_pTarget->m_fObjectSize + fMovingDistance + 5.0f &&
						m_bEvasion)
					{
						m_pTarget = NULL;
					}
				}
			}
			// 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템
			else if( m_nTargetIndex != g_pShuttleChild->m_myShuttleInfo.ClientIndex )
			{
				// 다른 유저가 피한것 타겟 없애준다
				if( m_bEvasion )
					m_pTarget = NULL;
			}
			// end 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템
		}
	}
	else if( m_dwWeaponState == _EXPLODED )
	{
		if(m_pCharacterInfo)
		{
			if(!m_pCharacterInfo->IsUsing())
				m_bUsing = FALSE;
		}
		else
		{
			m_bUsing = FALSE;
		}
	}
	D3DXVECTOR3 vSide;
	D3DXVec3Cross(&vSide,&m_vUp,&m_vVel);
	D3DXVec3Cross(&m_vUp,&m_vVel,&vSide);

	// 패턴 타켓 방향 관련 2004-07-27 jschoi
	if(nPatternType == TARGET_OFF)
	{
		D3DXMatrixLookAtRH( &m_mMatrix, &(m_vPos), &(m_vPos + m_vVel), &m_vUp);
	}
	else
	{
		D3DXMatrixLookAtRH( &m_mMatrix, &(m_vPos), &(vTargetPosition), &m_vUp);
	}

	D3DXMatrixInverse( &m_mMatrix, NULL, &m_mMatrix );
	// effect matrix & ticking

	if(m_pCharacterInfo)
	{
		m_pCharacterInfo->SetAllBodyConditionMatrix(m_mMatrix );
//		m_pCharacterInfo->SetSingleBodyConditionMatrix( BODYCON_FIRE_MASK,m_mFireMatrix );
		m_pCharacterInfo->Tick(fElapsedTime);
	}
	
	// SKILL EFFECT
	SetBodyConditionMatrixForSkillEffect(m_bodyCondition,m_mMatrix);
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			CWeaponMissileData::CheckWeaponCollision()
/// \brief		웨폰 충돌처리
/// \author		dhkwon
/// \date		2004-05-28 ~ 2004-05-28
/// \warning	차후에 하나로 묶을 예정
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
//byroo nemerian
void CWeaponMissileData::CheckWeaponCollision(CItemData *pTargetItem,float fMovingDistance)
{
	int nTargetIndex = m_nTargetIndex;
//	BOOL bAttackAvailable = TRUE;// 공격가능한 유닛인지 검사..(일반 유저공격 막기)
	DWORD dwTargetState = _NORMAL; // 공격가능한 상태인지 파악..(적의 상태가 DEAD이면 공격하지 않음)
	CMonsterData*	pMonster = NULL;
	CEnemyData *pEnemy = NULL;		// 2005-07-07 by ispark	// 롤링시스템을 사용시를 위해서

	// 타겟 유닛 구분
	if(m_nTargetIndex == g_pShuttleChild->m_myShuttleInfo.ClientIndex) // ShuttleChild : Monster ==> ShuttleChild
	{
//		bAttackAvailable = TRUE;
		dwTargetState = g_pShuttleChild->m_dwState;
	}
	else if( m_pTarget && m_pTarget->m_dwPartType == _ENEMY) // ENEMY : ShuttleChild ==> Enemy
	{
		CMapEnemyIterator itEnemy = 
			g_pScene->m_mapEnemyList.find(((CEnemyData *)m_pTarget)->m_infoCharacter.CharacterInfo.ClientIndex);
		if(itEnemy != g_pScene->m_mapEnemyList.end())
		{
			pEnemy = itEnemy->second;		// 2005-07-07 by ispark
//			bAttackAvailable = itEnemy->second->m_bAttackEnemy;
			dwTargetState = itEnemy->second->m_dwState;
		}
		else 
		{
			m_pTarget = NULL;
			return;
		}
	}
	else if( m_pTarget && m_pTarget->m_dwPartType == _MONSTER)	// MONSTER : ShuttleChild ==> Monster
	{
		CMapMonsterIterator itMonster = g_pScene->m_mapMonsterList.find(((CMonsterData *)m_pTarget)->m_info.MonsterIndex);
		if(itMonster != g_pScene->m_mapMonsterList.end())
		{
//			bAttackAvailable = TRUE;
			dwTargetState = itMonster->second->m_dwState;
			pMonster = itMonster->second;
		}
		else
		{
			return;
		}
	}

//	if(bAttackAvailable && ATTACK_AVAILABLE_STATE(dwTargetState))
	if(ATTACK_AVAILABLE_STATE(dwTargetState))
	{
		BOOL bCollision = FALSE;
		float fMissileSize = 5.0f;
		fMovingDistance = fMovingDistance < 10.0f ? 10.0f : fMovingDistance;
		D3DXVECTOR3 vSide,vUpTemp;
		D3DXMATRIX mat;
		D3DXVec3Cross(&vSide,&m_vUp,&m_vVel);
		D3DXVec3Cross(&vUpTemp,&m_vVel,&vSide);
		D3DXMatrixLookAtLH( &mat, &m_vPos, &(m_vPos+m_vVel), &vUpTemp);
		float fDist;

		// 충돌 검사
		if (m_pTarget)
		{
			auto vTargetDist = m_pTarget->m_vPos - m_vPos;

			if (!pMonster || !pMonster->m_pMonMesh)
			{
				if(D3DXVec3Length(&vTargetDist) < m_pTarget->m_fObjectSize + fMissileSize + fMovingDistance)
//				if(D3DXVec3Length(&vTargetVel) < (m_pTarget->m_fObjectSize / 2) + fMissileSize)
				{
					// 2005-07-07 by ispark
					// 롤링 시스템
					bCollision = RollingCollision(pEnemy);
					if(bCollision == FALSE)
					{
						SendFieldSocketBattleAttackEvasion(nTargetIndex, pTargetItem==NULL ? 0:pTargetItem->m_nItemIndex,m_nClientIndex,m_pItemData->ItemNum);
					}
				}				
			}
			else
			{
				auto fMonsterRadius = pMonster->m_pMonMesh->m_fRadius;
				if (m_pTarget->m_dwPartType == _MONSTER && fMonsterRadius > BIG_MONSTER_SIZE)
				{
					if (D3DXVec3Length(&vTargetDist) < fMonsterRadius + fMissileSize + fMovingDistance)
					{
						pMonster->m_pMonMesh->Tick(pMonster->m_fCurrentTime);
						pMonster->m_pMonMesh->SetWorldMatrix(pMonster->m_mMatrix);
						auto distance = sqrt(pow(m_vPos.x - m_pTarget->m_vPos.x, 2) + pow(m_vPos.y - m_pTarget->m_vPos.y, 2) + pow(m_vPos.z - m_pTarget->m_vPos.z, 2));

						if (distance - fMissileSize - m_pTarget->m_fObjectSize <= 0)
						//float fcollDist = pMonster->m_pMonMesh->CheckCollision(mat,m_vPos,DEFAULT_COLLISION_DISTANCE,TRUE,FALSE).fDist;
						//if (fcollDist < fMovingDistance)
						{
							m_vPos += m_vVel * fMovingDistance;
							bCollision = TRUE;
						}

					}		
				}
				else	// 소형 몬스터
				{
//					if(D3DXVec3Length(&vTargetVel) < m_pTarget->m_fObjectSize + fMissileSize + fMovingDistance)
					if(D3DXVec3Length(&vTargetDist) <  m_pTarget->m_fObjectSize + fMissileSize + fMovingDistance)
					{
						bCollision = TRUE;
					}
				}
			}
		}
		//지상 충돌
		if(!bCollision)
		{

#ifndef COLLCHECKLAG
			fDist = g_pScene->m_pObjectRender->CheckCollMeshRangeObject(mat, m_vPos, fMovingDistance).fDist;
#else
			if (m_vLastVelocity != m_vVel || m_fLastCollisionDistance == DEFAULT_COLLISION_DISTANCE)
				
				fDist = g_pScene->m_pObjectRender->CheckCollMeshRangeObject(mat, m_vPos, fMovingDistance).fDist;

			else fDist = m_fLastCollisionDistance;
#endif
			if (fDist < fMovingDistance)
			{
				m_vPos += m_vVel * fDist;
				nTargetIndex = 0;
				bCollision = TRUE;
				m_pTarget = NULL;
			}
#ifdef COLLCHECKLAG
			else
			{
				m_fLastCollisionDistance = fDist != DEFAULT_COLLISION_DISTANCE ? fDist - fMovingDistance : fDist;
				m_vLastVelocity = m_vVel;
			}
#endif

			if (!bCollision && IsTileMapRenderEnable(g_pShuttleChild->m_myShuttleInfo.MapChannelIndex.MapIndex)) // 2005-12-07 by ispark, 바닥을 렌더링을 하면 충돌 검사
			{
				// 바닥 충돌
				float fHeight = g_pGround->CheckHeightMap(m_vPos + m_vVel*fMovingDistance);
				if(fHeight - 20.0f > m_vPos.y)
				{
					nTargetIndex = 0;
					m_pTarget = NULL;
					bCollision = TRUE;
				}				
			}
		}

		if (bCollision)
		{
			SendFieldSocketBattleAttackFind(nTargetIndex, pTargetItem==NULL ? 0:pTargetItem->m_nItemIndex,m_nClientIndex,m_pItemData->ItemNum);
			// 프로토콜 전송
/*			if(m_nClientIndex == g_pShuttleChild->m_myShuttleInfo.ClientIndex)// 클라이언트(CShuttleChild)
			{
				if(pTargetItem)
				{
//					SendFieldSocketBattleAttackItemFind(nTargetIndex, pTargetItem->m_nItemIndex);
					SendFieldSocketBattleAttackFind(nTargetIndex, pTargetItem==NULL ? 0:pTargetItem->m_nItemIndex,m_nClientIndex,m_pItemData->ItemNum);
				}
				else
				{
//					SendFieldSocketBattleAttackFind(nTargetIndex, 0);
					SendFieldSocketBattleAttackFind(nTargetIndex, 0 ,m_nClientIndex , m_pItemData->ItemNum);
				}
			}
			else // 몬스터 2형 무기인 경우
			{
				if(pTargetItem)
				{
//					SendFieldSocketBattleMonsterAttackItemFind(nTargetIndex, pTargetItem->m_nItemIndex);
					SendFieldSocketBattleAttackFind(nTargetIndex, pTargetItem->m_nItemIndex,m_nClientIndex,m_pItemData->ItemNum);
				}
				else
				{
//					SendFieldSocketBattleMonsterAttackFind(nTargetIndex);
					SendFieldSocketBattleAttackFind(nTargetIndex, 0 ,m_nClientIndex , m_pItemData->ItemNum);
				}
			}
*/			//m_pTarget = NULL;
			m_dwWeaponState = _EXPLODING;
		}
	}
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			CWeaponMissileData::SkillTick()
/// \brief		미사일 지상폭격
/// \author		ispark
/// \date		2005-11-25 ~ 2005-11-25
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CWeaponMissileData::SkillTick()
{
	float fElapsedTime = g_pD3dApp->GetElapsedTime();
	m_vOldPos = m_vPos;
//	D3DXVECTOR3 vSide,vUpTemp;
//	D3DXMATRIX mat;
//	D3DXVECTOR3 vUp(0,1,0);

	m_fTargetDistance = D3DXVec3Length(&(m_vStartPos - (m_pTarget ? m_pTarget->m_vPos : m_vTargetPos)));
	
	// 패턴 타켓방향 관련 2004-07-27 jschoi
	int nPatternType = m_pCinema->GetHeader().nPatternType;
	D3DXVECTOR3 vTargetPosition;

	if(m_dwWeaponState == _EXPLODING) // exploding state
	{
		if(m_pTarget)
		{
			if(((CUnitData*)m_pTarget)->m_bShielding == TRUE && m_pAttacker)
			{// 쉴드 발동 중일때 쉴드 Hit 이펙트 추가
				//D3DXVECTOR3 vVel;
				//D3DXVec3Normalize(&vVel, &(m_pAttacker->m_vPos - m_pTarget->m_vPos));
				//m_vPos = m_pTarget->m_vPos + vVel*SIZE_OF_SHIELD_EFFECT;
				((CUnitData*)m_pTarget)->CreateSecondaryShieldDamage(m_pAttacker->m_vPos);
			}
			else	
			{
				// 랜덤하게 위치를 조정한다.
				//D3DXVECTOR3 vVel;
				//D3DXVec3Normalize(&vVel, &(m_pAttacker->m_vPos - m_pTarget->m_vPos));
				//vVel = D3DXVECTOR3( vVel.x*((float)RANDI(0, 4)),vVel.y*((float)RANDI(0, 4)),vVel.z*((float)RANDI(0, 4)));
				//m_vPos = m_pTarget->m_vPos + vVel;
			}
		}
		//폭발 이펙트
		SetBodyCondition(BODYCON_HIT_MASK);
		if(m_nSkillNum != 0)
		{
			ChangeBodyConditionForSkillEffect(m_nSkillNum,BODYCON_HIT_MASK);
		}	
		m_dwWeaponState = _EXPLODED;
		
	} 
	else if(m_dwWeaponState == _NORMAL)
	{
		CItemData *pTargetItem = g_pScene->FindFieldItemByFieldIndex( m_nTargetItemFieldIndex );
		m_fWeaponLifeCheckTime += fElapsedTime;
		if(m_fWeaponLifeCheckTime > 0.5f)
			m_bSetTarget = TRUE;
		if(m_fWeaponLifeCheckTime * m_fWeaponSpeed > m_pItemData->Range)
		{
			m_dwWeaponState = _EXPLODING;
			m_pTarget = NULL;
		}
		else
		{
			CheckTargetState();
			if(pTargetItem && pTargetItem->m_dwState == _NORMAL)
			{
				m_vTargetPos = pTargetItem->m_vPos;
			}
			else if(m_pTarget && ATTACK_AVAILABLE_STATE(m_pTarget->m_dwState))
			{
				m_vTargetPos = m_pTarget->m_vPos;
			}
			else
			{
				m_bSetTarget = FALSE;
			}

//			DBGOUT("%d. 타겟(%.0f, %.0f, %.0f) ", m_nWeaponIndex, m_vTargetPos.x, m_vTargetPos.y, m_vTargetPos.z);
//			DBGOUT("미사일(%.0f, %.0f, %.0f) ", m_vPos.x, m_vPos.y, m_vPos.z);
//			DBGOUT("거리 = %.0f\n", D3DXVec3Length(&(m_vPos - m_vTargetPos)));
			// MoveWeapon 대신 패턴 적용
			BOOL bResult;							// 목표점에 도달했는지 결과..
			bResult=m_pCinema->Tick(m_vTargetPos);	// 결과가 FALSE 면 목표를 지났다.. 즉 도달했다.
			EVENT_POINT ep;
			ep=m_pCinema->GetCurrentCinemaPoint();
			m_vPos = ep.vPosition;			// 위치
			m_vVel = ep.vDirection;			// 방향
			m_fWeaponSpeed = ep.fVelocity;	// 속력
			m_vUp = ep.vUpVector;
			vTargetPosition = ep.vTarget;

			// 내가 발사한 무기이거나 몬스터가 발사한 2형 무기일때 충돌 체크
			if(	m_nClientIndex == g_pShuttleChild->m_myShuttleInfo.ClientIndex ||
				(IS_SECONDARY_WEAPON_MONSTER(m_pItemData->Kind) &&
				m_nTargetIndex == g_pShuttleChild->m_myShuttleInfo.ClientIndex ))
			{
				float fMovingDistance = ep.fVelocity*fElapsedTime;
				CheckTargetByBomb(fMovingDistance);
			}
		}
	}
	else if(m_dwWeaponState == _EXPLODED)
	{
		if(m_pCharacterInfo)
		{
			if(!m_pCharacterInfo->IsUsing())
				m_bUsing = FALSE;
		}
		else
		{
			m_bUsing = FALSE;
		}
	}
	D3DXVECTOR3 vSide;
	D3DXVec3Cross(&vSide,&m_vUp,&m_vVel);
	D3DXVec3Cross(&m_vUp,&m_vVel,&vSide);

	// 패턴 타켓 방향 관련 2004-07-27 jschoi
	if(nPatternType == TARGET_OFF)
	{
		D3DXMatrixLookAtRH( &m_mMatrix, &(m_vPos), &(m_vPos + m_vVel), &m_vUp);
	}
	else
	{
		D3DXMatrixLookAtRH( &m_mMatrix, &(m_vPos), &(vTargetPosition), &m_vUp);
	}

	D3DXMatrixInverse( &m_mMatrix, NULL, &m_mMatrix );
	// effect matrix & ticking
	if(m_pCharacterInfo)
	{
		m_pCharacterInfo->SetAllBodyConditionMatrix(m_mMatrix );
//		m_pCharacterInfo->SetSingleBodyConditionMatrix( BODYCON_FIRE_MASK,m_mFireMatrix );
		m_pCharacterInfo->Tick(fElapsedTime);
	}
}

void CWeaponMissileData::CheckTargetByBomb(float fMovingDistance)
{
	int nTargetIndex;
	CItemData* pTargetItem = NULL;
	BOOL bCollision = FALSE;
	float fExplosionRange = 0;
	
	// Enemy 충돌 검사
	if(!bCollision)
	{
//		// 2005-08-11 by ispark
//		// 지상폭격 조건 수정
//		// Enemy는 타겟 잡힌 한명만 공격, 주변은 무시
//		if(	g_pShuttleChild->m_pOrderTarget != NULL								&&
//			g_pShuttleChild->m_pOrderTarget->m_dwPartType == _ENEMY				&&
//			(((CUnitData*)g_pShuttleChild->m_pOrderTarget)->IsPkAttackEnable()	||		// 1. 강제 공격이 아니거나
////			g_pShuttleChild->m_pPkNormalTimer->IsPkEnableNormalOrderTarget()))			// 2. Delay Time 이 없거나
//			g_pShuttleChild->IsEnemyPKAttackTime(m_nTargetIndex)))	// 2005-11-03 by ispark	  Delay Time 이 없다
//		{
//			CEnemyData* pTarget = (CEnemyData*)g_pShuttleChild->m_pOrderTarget;
//			if( pTarget && D3DXVec3Length(&(m_vPos - pTarget->m_vPos)) < pTarget->m_fObjectSize + fRocketSize + fMovingDistance)
//			{
//				nTargetIndex = pTarget->m_infoCharacter.CharacterInfo.ClientIndex;
//				pTargetItem = g_pScene->FindFieldItemByPartTypeAndParent( _DUMMY, pTarget );
//				bCollision = TRUE;
//			}
//		}
		// 2006-12-04 by ispark
		// 지상폭격 조건 수정
		// Enemy 전체 검색
		float fLengthMin = 1000.0f;
		CEnemyData* pCollisionTarget = NULL;
		CMapEnemyIterator it = g_pScene->m_mapEnemyList.begin();
		while( it != g_pScene->m_mapEnemyList.end() )
		{
			if(	it->second->IsPkEnable())
			{
				CEnemyData* pTarget = it->second;
				float fLengthTemp = D3DXVec3Length(&(m_vPos - pTarget->m_vPos));
				fExplosionRange = 0;
				if( SKILL_BASE_NUM(m_nSkillNum) != BGEAR_SKILL_BASENUM_GROUNDBOMBINGMODE)
				{
					fExplosionRange = m_fExplosionRange;
				}

				
				// 2007-04-30 by bhsohn 타켓 버그
				if( ATTACK_AVAILABLE_STATE(it->second->m_dwState)
				&& pTarget && fLengthTemp < pTarget->m_fObjectSize + fMovingDistance + fExplosionRange)
				{
					if(fLengthMin > fLengthTemp)
					{
						pCollisionTarget = pTarget;
						nTargetIndex = pTarget->m_infoCharacter.CharacterInfo.ClientIndex;
						pTargetItem = g_pScene->FindFieldItemByPartTypeAndParent( _DUMMY, pTarget );
						fLengthMin = fLengthTemp;
						bCollision = TRUE;
					}
				}
			}
			it++;
		}

		BOOL bCheckRolling = RollingCollision(pCollisionTarget);
		if(bCheckRolling == FALSE)
		{
			bCollision = FALSE;
		}
	}

	// 2013-02-18 by bhsohn 지폭/공폭 데미지 안들어가는 현상 처리
	char chDebugColl[256] = "";	
	// END 2013-02-18 by bhsohn 지폭/공폭 데미지 안들어가는 현상 처리

	// Monster 충돌 검사
	if(!bCollision)
	{
		float fLengthMin = 1000.0f;
		CMapMonsterIterator it = g_pScene->m_mapMonsterList.begin();
		while( it != g_pScene->m_mapMonsterList.end() )
		{
			CMonsterData* pTarget = it->second;
			float fLengthTemp = D3DXVec3Length(&(m_vPos - pTarget->m_vPos));
			fExplosionRange = 0;
			// 2013-02-18 by bhsohn 지폭/공폭 데미지 안들어가는 현상 처리
//			if( SKILL_BASE_NUM(m_nSkillNum) != BGEAR_SKILL_BASENUM_GROUNDBOMBINGMODE)
			if( SKILL_BASE_NUM(m_nSkillNum) == BGEAR_SKILL_BASENUM_GROUNDBOMBINGMODE)
			{
				fExplosionRange = m_fExplosionRange;
			}
			// 2013-03-21 by bhsohn 대형 몬스터 공폭 안들어가는 문제 해결
			else if( SKILL_BASE_NUM(m_nSkillNum) == BGEAR_SKILL_BASENUM_AIRBOMBINGMODE)
			{
				fExplosionRange = m_fExplosionRange;
			}
			// END 2013-03-21 by bhsohn 대형 몬스터 공폭 안들어가는 문제 해결			
			
			if( pTarget && fLengthTemp < 
				(pTarget->m_pMonMesh!=NULL? pTarget->m_pMonMesh->m_fRadius : pTarget->m_fObjectSize) + fExplosionRange)
			{
				// 2007-05-16 by bhsohn 같은 세력 몬스터 공폭/지폭 사용시, 터지는 현상 처리
				//if(fLengthMin > fLengthTemp)
				// 같은 세력이 아니어야만 한다. 
				if( FALSE == IS_SAME_CHARACTER_MONSTER_INFLUENCE(g_pShuttleChild->m_myShuttleInfo.InfluenceType, pTarget->m_pMonsterInfo->Belligerence)
					&&(fLengthMin > fLengthTemp))
				{
					float fMonsterRadius;
					if (pTarget->m_pMonMesh)
					{
						fMonsterRadius = pTarget->m_pMonMesh->m_fRadius;
					}
					else 
					{
						fMonsterRadius = pTarget->m_fObjectSize;
					}
					
					if(pTarget->m_dwPartType == _MONSTER && fMonsterRadius > BIG_MONSTER_SIZE)
					{
						auto distance = sqrt(pow(m_vPos.x - pTarget->m_vPos.x, 2) + pow(m_vPos.y - pTarget->m_vPos.y, 2) + pow(m_vPos.z - pTarget->m_vPos.z, 2));
						
						if(distance - fExplosionRange - pTarget->m_fObjectSize <= 0)
						{
							m_vPos += m_vVel * distance;
							nTargetIndex = pTarget->m_info.MonsterIndex;
							pTargetItem = g_pScene->FindFieldItemByPartTypeAndParent( _DUMMY, pTarget );
							//fLengthMin = fLengthTemp;
							bCollision = TRUE;
						}
						//*--------------------------------------------------------------------------*//							nTargetIndex = pTarget->m_info.MonsterIndex;
					}
					else
					{
						nTargetIndex = pTarget->m_info.MonsterIndex;
						pTargetItem = g_pScene->FindFieldItemByPartTypeAndParent( _DUMMY, pTarget );
						//fLengthMin = fLengthTemp;
						bCollision = TRUE;
					}
				}
			}
			it++;
		}
	}

	//지상 충돌
	if(!bCollision)
	{
		D3DXVECTOR3 vSide,vUpTemp;
		D3DXMATRIX mat;
		D3DXVec3Cross(&vSide,&m_vUp,&m_vVel);
		D3DXVec3Cross(&vUpTemp,&m_vVel,&vSide);
		D3DXMatrixLookAtLH( &mat, &m_vPos, &(m_vPos+m_vVel), &vUpTemp);
		float fDist = g_pScene->m_pObjectRender->CheckCollMeshRangeObject(mat,m_vPos,fMovingDistance).fDist;
		if(fDist < fMovingDistance)
		{
			nTargetIndex = 0;
			bCollision = TRUE;
		}
		
		if(!bCollision)
		{
			// 바닥 충돌
			float fHeight = g_pGround->CheckHeightMap(m_vPos);
			if(fHeight > m_vPos.y)
			{
				nTargetIndex = 0;
				bCollision = TRUE;
			}
		}
		
		// 2006-12-01 by ispark, 공중 폭격
		if(!bCollision &&
			SKILL_BASE_NUM(m_nSkillNum) == BGEAR_SKILL_BASENUM_AIRBOMBINGMODE)
		{
			float fLength = D3DXVec3Length(&(m_vPos - m_vTargetPos));
			if(fLength < 50.0f)
			{
				nTargetIndex = 0;
				bCollision = TRUE;
			}
		}
		// 2007-10-04 by dgwoo 지상 폭격.
		fExplosionRange = 0;
		if( SKILL_BASE_NUM(m_nSkillNum) == BGEAR_SKILL_BASENUM_GROUNDBOMBINGMODE)
		{
			fExplosionRange = m_fExplosionRange;
		}



		// 2004-10-23 by jschoi
		//////////////////////////////////////////////////////////////////////////
		// 바닥 충돌
		// 여기서 몬스터, Enemy 의 폭발반경에 대해 충돌 처리해야함.
		if(	bCollision && nTargetIndex == 0 )
		{
			float fLengthMin = 1000.0f;
			CEnemyData* pCollisionTarget = NULL;
			CMapEnemyIterator it = g_pScene->m_mapEnemyList.begin();
			while( it != g_pScene->m_mapEnemyList.end() )
			{
				if(	it->second->IsPkEnable())
				{
					CEnemyData* pTarget = it->second;
					float fLengthTemp = D3DXVec3Length(&(m_vPos - pTarget->m_vPos));

					// 2007-04-30 by bhsohn 타켓 버그					
					if( ATTACK_AVAILABLE_STATE(it->second->m_dwState)
						&& pTarget && fLengthTemp < pTarget->m_fObjectSize + fMovingDistance + m_fExplosionRange)
					{
						if(fLengthMin > fLengthTemp)
						{
							pCollisionTarget = pTarget;
							nTargetIndex = pTarget->m_infoCharacter.CharacterInfo.ClientIndex;
							pTargetItem = g_pScene->FindFieldItemByPartTypeAndParent( _DUMMY, pTarget );
							fLengthMin = fLengthTemp;
							bCollision = TRUE;
						}
					}
				}
				it++;
			}

			BOOL bCheckRolling = RollingCollision(pCollisionTarget);
			if(bCheckRolling == FALSE)
			{
				bCollision = FALSE;
			}
		}

		if(	bCollision && nTargetIndex == 0 )
		{
			float fLengthMin = 1000.0f;
			CMapMonsterIterator it = g_pScene->m_mapMonsterList.begin();
			while( it != g_pScene->m_mapMonsterList.end() )
			{
				CMonsterData* pTarget = it->second;
				float fLengthTemp = D3DXVec3Length(&(m_vPos - pTarget->m_vPos));
// 2007-10-04 by dgwoo 지폭시 탄에 저장되어있는 폭발반경을 적용한다.
//				if( pTarget && fLengthTemp < 
//					(pTarget->m_pMonMesh!=NULL?pTarget->m_pMonMesh->m_fRadius : pTarget->m_fObjectSize) + CAtumSJ::GetExplosionRange(m_pItemData, &g_pShuttleChild->m_paramFactor))
				if( pTarget && fLengthTemp < 
					(pTarget->m_pMonMesh!=NULL?pTarget->m_pMonMesh->m_fRadius : pTarget->m_fObjectSize*10) + fExplosionRange)
				{
					if(fLengthMin > fLengthTemp)
					{
						float fMonsterRadius = pTarget->m_pMonMesh->m_fRadius;
						if(pTarget->m_dwPartType == _MONSTER && fMonsterRadius > BIG_MONSTER_SIZE)
						{
							//*--------------------------------------------------------------------------*//
							// 2006-12-13 by ispark, 몬스터 메쉬 충돌 검사
							D3DXVECTOR3 vSide,vUpTemp;
							D3DXMATRIX matMonster;
							D3DXVec3Cross(&vSide,&m_vUp,&m_vVel);
							D3DXVec3Cross(&vUpTemp,&m_vVel,&vSide);
							D3DXMatrixLookAtLH( &matMonster, &m_vPos, &(m_vPos+m_vVel), &vUpTemp);
							pTarget->m_pMonMesh->Tick(pTarget->m_fCurrentTime);
							pTarget->m_pMonMesh->SetWorldMatrix(pTarget->m_mMatrix);
							float fcollDist = pTarget->m_pMonMesh->CheckCollision(matMonster,m_vPos,DEFAULT_COLLISION_DISTANCE,TRUE,FALSE).fDist;
							// 2013-02-18 by bhsohn 지폭/공폭 데미지 안들어가는 현상 처리
//							if(fcollDist <= CAtumSJ::GetExplosionRange(m_pItemData, &g_pShuttleChild->m_paramFactor) )
//							DBGOUT("fcollDist[%.2f] fExplosionRange[%.2f] #1 \n", fcollDist, fExplosionRange);							
							sprintf(chDebugColl, "fcollDist[%.2f] fExplosionRange[%.2f] #2 \n", fcollDist, fExplosionRange);

							if(fcollDist <= fExplosionRange)
							{
								nTargetIndex = pTarget->m_info.MonsterIndex;
								pTargetItem = g_pScene->FindFieldItemByPartTypeAndParent( _DUMMY, pTarget );
								//fLengthMin = fLengthTemp;
								bCollision = TRUE;
							}
							//*--------------------------------------------------------------------------*//							nTargetIndex = pTarget->m_info.MonsterIndex;
						}
						else
						{
							nTargetIndex = pTarget->m_info.MonsterIndex;
							pTargetItem = g_pScene->FindFieldItemByPartTypeAndParent( _DUMMY, pTarget );
							//fLengthMin = fLengthTemp;
							bCollision = TRUE;
						}
					}
				}
				it++;
			}
		}
	}
	
	if(bCollision)
	{
		SendFieldSocketBattleAttackFind(nTargetIndex, pTargetItem==NULL ? 0:pTargetItem->m_nItemIndex,m_nClientIndex,m_pItemData->ItemNum);

		// 2013-02-18 by bhsohn 지폭/공폭 데미지 안들어가는 현상 처리	
// 		int nMyClientIndex = m_nClientIndex;
// 		if(nMyClientIndex == g_pShuttleChild->m_myShuttleInfo.ClientIndex)
// 		{		
// 			DBGOUT("CWSlowData::SendFieldSocketBattleAttackFind nClientIndex[%d] nTargetIndex[%d] [%s] \n", 
// 				nMyClientIndex, nTargetIndex, chDebugColl);
// 		}
		// END 2013-02-18 by bhsohn 지폭/공폭 데미지 안들어가는 현상 처리

		m_dwWeaponState = _EXPLODING;
	}

}

void CWeaponMissileData::CheckTargetWarning()
{
	if(g_pShuttleChild && (m_nTargetMe == g_pShuttleChild->m_myShuttleInfo.ClientIndex))
	{
		int nMissileCount = g_pShuttleChild->GetMissileCount();
		nMissileCount--;
//		DBGOUT("삭제 %d\n", nMissileCount);
		g_pShuttleChild->SetMissileCount(nMissileCount);
		if(nMissileCount <= 0)
		{
			g_pShuttleChild->SetMissileWarning(FALSE);
			g_pShuttleChild->SetMissileCount(0);
		}
		m_nTargetMe = -1;
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CWeaponMissileData::CheckTargetState()
/// \brief		타겟 상태 검사
/// \author		ispark
/// \date		2006-12-08 ~ 2006-12-08
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CWeaponMissileData::CheckTargetState()
{
	//////////////////////////////////////////////////////////////////////////
	// 내가 타겟이면
	if(m_nTargetIndex == g_pShuttleChild->m_myShuttleInfo.ClientIndex)
	{
		if((m_pAttacker->m_dwPartType == _MONSTER &&				// 인비지블을 인식하는 몬스터이거나 .
			COMPARE_MPOPTION_BIT(((CMonsterData*)m_pAttacker)->m_pMonsterInfo->MPOption,MPOPTION_BIT_RECOGNIZE_INVISIBLE))
			|| g_pShuttleChild->m_bySkillStateFlag == CL_SKILL_NONE) //스킬을 발동중이 아닐때.
		{// 
		}
		else
		{
			// 2007-04-24 by bhsohn 내가 인비지블 상태일떄 미사일이 이상한 곳에 터지는 현상처리
			//m_pTarget = NULL;
			if(m_pAttacker->m_dwPartType != _ENEMY)
			{
				m_pTarget = NULL;
			}
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// 타겟이 상대방이면
	if( m_pTarget && m_pTarget->m_dwPartType == _ENEMY)
	{
		// 2006-12-08 by ispark, 인비지블, 위장이라면 타겟을 잃는다.
		//if(((CEnemyData *)m_pTarget)->m_bySkillStateFlag != CL_SKILL_NONE)
		// 2007-02-08 by dgwoo 알파 값에 따라 타겟을 잃을때.
		if(((CEnemyData *)m_pTarget)->m_nAlphaValue == SKILL_OBJECT_ALPHA_OTHER_INFLUENCE)
		{
			// 2007-06-15 by dgwoo 이미 타겟을 한번 잡은 미사일은 계속 따라가도록 수정.
			//m_pTarget = NULL;
		}
	}
}