// WeaponRocketData.cpp: implementation of the CWeaponRocketData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "WeaponRocketData.h"
#include "AtumApplication.h"
//#include "TraceData.h"
#include "EnemyData.h"
#include "MonsterData.h"
#include "Background.h"
#include "FieldWinSocket.h"
#include "SceneData.h"
#include "ShuttleChild.h"
//#include "ObjectRender.h"
#include "ItemData.h"
#include "Cinema.h"
#include "ObjRender.h"
#include "dxutil.h"
#include "PkNormalTimer.h"
#include "StoreData.h" // 2010-06-15 by shcho&hslee 펫시스템 - 오퍼레이터 구현	
#include "Interface.h" // 2010-06-15 by shcho&hslee 펫시스템 - 오퍼레이터 구현	
#include "INFOpMain.h" // 2010-06-15 by shcho&hslee 펫시스템 - 오퍼레이터 구현	
#include "INFOpInfo.h" // 2010-06-15 by shcho&hslee 펫시스템 - 오퍼레이터 구현	
#include "AtumDatabase.h"		// 2013-03-13 by bhsohn 스킬 번호 구조 수정

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

#define ROCKET_SPEED	800.0f
#define GROUND_ATTACK_SPEED	100.0f

// 2009. 11. 23 by jskim 리소스 로딩 구조 변경
// 2009. 08. 27 by ckPark 그래픽 리소스 변경 시스템 구현
// CWeaponRocketData::CWeaponRocketData(CAtumData * pAttack, 
// 									 ITEM * pWeaponITEM, 
// 									 ATTACK_DATA & attackData)
// CWeaponRocketData::CWeaponRocketData( CAtumData * pAttack,
// 									  ITEM * pWeaponITEM,
// 									  ATTACK_DATA & attackData,
// 									  ITEM* pEffectItem /* = NULL */ )
// end 2009. 08. 27 by ckPark 그래픽 리소스 변경 시스템 구현
CWeaponRocketData::CWeaponRocketData( CAtumData * pAttack,
									 ITEM * pWeaponITEM,
									 ATTACK_DATA & attackData,
									 ITEM* pEffectItem, /* = NULL */
									 int LoadingPriority )
//end 2009. 11. 23 by jskim 리소스 로딩 구조 변경
{
	// 2009. 11. 23 by jskim 리소스 로딩 구조 변경
	m_LoadingPriority = LoadingPriority;
	//end 2009. 11. 23 by jskim 리소스 로딩 구조 변경
	m_pAttacker = pAttack;
	m_pCharacterInfo = NULL;
	m_nSkillNum = attackData.AttackData.SkillNum;
	m_pTarget = (CAtumData*)g_pScene->FindUnitDataByClientIndex( attackData.AttackData.TargetInfo.TargetIndex );
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
	// 이펙트 아이템이 있다면 그것으로 생성
	m_pEffectItem	= pEffectItem;
	// end 2009. 08. 27 by ckPark 그래픽 리소스 변경 시스템 구현

	m_fWarheadSpeed = attackData.fWarheadSpeed;
	SetShuttleChildOrderTarget();
	m_pCinema =  NULL;		// 2004.06.29 jschoi
	m_nTargetMe = 0;

	m_vTargetPos = A2DX(attackData.AttackData.TargetInfo.TargetPosition);
	InitData();

	// 2005-07-19 by ispark
	// 나를 향한 타겟이라면 타겟을 저장한다. 이후에 삭제시 확인을 위해서이다. 
	// 2006-03-06 by ispark, 패턴이 없다면 경고음과 메세지 취소
	if(attackData.AttackData.TargetInfo.TargetIndex == g_pShuttleChild->m_myShuttleInfo.ClientIndex &&
		m_pItemData->CameraPattern != 0)
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
}

CWeaponRocketData::~CWeaponRocketData()
{
	FLOG( "~CWeaponRocketData()" );
//	util::del(m_pTraceList1);
//	util::del(m_pTraceList2);
	if(m_pCharacterInfo)//추가
	{
		m_pCharacterInfo->InvalidateDeviceObjects();
		m_pCharacterInfo->DeleteDeviceObjects();
		util::del(m_pCharacterInfo);
	}
	
	util::del(m_pCinema);		// 2004.06.29 jschoi

	CheckTargetWarning();
}

void CWeaponRocketData::InitData()
{
//	D3DXVECTOR3 vSide;
//	vSide.x = pAttack->m_mMatrix._11;
//	vSide.y = pAttack->m_mMatrix._12;
//	vSide.z = pAttack->m_mMatrix._13;
//	vSide *= -1.0f;
//
//	m_vUp.x = m_pAttacker->m_mMatrix._21;
//	m_vUp.y = m_pAttacker->m_mMatrix._22;
//	m_vUp.z = m_pAttacker->m_mMatrix._23;
//
	D3DXVECTOR3 vSide;
	if(m_nSkillNum == 0)
	{
		D3DXVECTOR3 vVel = m_pAttacker->m_vVel;
		m_vUp = m_pAttacker->m_vUp;
		D3DXVec3Cross(&vSide,&vVel,&m_vUp);
		D3DXVec3Normalize(&vSide,&vSide);
	}
	else
	{
		D3DXVECTOR3 vVel = m_pAttacker->m_vVel;
		vVel.y =0;
		D3DXVec3Normalize(&vVel,&vVel);
		m_vUp = D3DXVECTOR3(0,1,0);
		D3DXVec3Cross(&vSide,&vVel,&m_vUp);
		D3DXVec3Normalize(&vSide,&vSide);
	}


	if(m_bWeaponFlyType == 0)
	{
		m_vPos = m_pAttacker->m_vLWSecondaryPos + m_vFirePos.x*vSide;
	}
	else
	{
		m_vPos = m_pAttacker->m_vRWSecondaryPos + m_vFirePos.x*vSide;
	}
// 2006-01-17 by ispark, 이거 누구 한거야~!!! ㅡㅡ;
//	m_vTargetPos = m_vTargetPos + m_vPos - m_pAttacker->m_vPos;
	m_vStartPos = m_vPos;
	if(m_nSkillNum == 0)
	{
		m_vVel = m_vTargetPos - m_vPos;
	}
	else
	{
		m_vVel = m_vTargetPos - m_vPos;
		m_vVel.y = 0;
	}

	m_fTargetDistance = D3DXVec3Length(&(m_vPos - m_vTargetPos));
	m_vOldPos = m_vPos;
	D3DXVec3Normalize(&m_vVel,&m_vVel);
	m_dwWeaponState = _NORMAL;
	m_bSendData = FALSE;
	m_fTurnCheckTime = 0.0f;
	if(m_nSkillNum == 0)
	{
		m_fWeaponSpeed = ROCKET_SPEED;
		m_fWeaponLifeTime = 3.0f;
	}
	else
	{
		m_fWeaponSpeed = GROUND_ATTACK_SPEED;
		m_fWeaponLifeTime = 24.0f;
	}

	D3DXMatrixLookAtLH( &m_mMatrix, &(m_vPos), &(m_vPos + m_vVel), &m_vUp);
	D3DXMatrixInverse( &m_mMatrix, NULL, &m_mMatrix );
	char buf[256];
	
	// 2009. 08. 27 by ckPark 그래픽 리소스 변경 시스템 구현
	//wsprintf(buf,"%08d",m_pItemData->SourceIndex);							// 2005-08-23 by ispark

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
	m_dwPartType = _ROCKET;

//  시네마 포인트 셋팅/////////////////////////////////////////////////////////////////
	char str[32];

	if(m_nSkillNum == 0)
	{ // Attack_OK에 SkillNum이 0인 경우
//		sprintf(str,"%08d",m_pItemData->ItemNum);
		sprintf(str,"%08d",m_pItemData->CameraPattern);
	}
	else
	{ // Attack_OK에 SkillNum이 담겨있는 경우
		sprintf(str,"%08d",SKILL_BASE_NUM(m_nSkillNum));
	}

	util::del(m_pCinema);
	m_pCinema = g_pScene->LoadCinemaData(str, m_pItemData->CameraPattern);
	if(m_pCinema == NULL)
	{
		// 2006-03-06 by ispark, 디폴트 패턴을 쓰지 않고 기본 직선으로 표현
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
	if(m_nSkillNum == 0)
	{
		ep.vPosition = m_vPos;
		ep.vDirection = m_vVel;
		ep.vTarget = m_vVel;
		ep.vUpVector = m_vUp;
		D3DXVec3Normalize(&ep.vDirection,&ep.vDirection);
		ep.fVelocity = m_fWeaponSpeed;	// m_fWeaponSpeed 초기 스피드
		ep.fCurvature = DEFAULT_CURVATURE;	// 초기값 셋팅
	}
	else
	{
		ep.vPosition = m_vPos;
//		m_vVel.y = 0;
//		D3DXVec3Normalize(&m_vVel,&m_vVel);
		ep.vDirection = m_vVel;
		ep.vTarget = m_vVel;
		ep.vUpVector = m_vUp;
		D3DXVec3Normalize(&ep.vDirection,&ep.vDirection);
		ep.fVelocity = m_fWeaponSpeed;	// m_fWeaponSpeed 초기 스피드
		ep.fCurvature = DEFAULT_CURVATURE;	// 초기값 셋팅
	}
//	m_pCinema->InitCinemaData(ep,m_bWeaponFlyType == 0);
	m_pCinema->InitWeaponCinemaData(ep,m_bWeaponFlyType == 0, m_fWarheadSpeed, m_pItemData->BoosterAngle);
//  초기화 종료 ///////////////////////////////////////////////////////////////////////
}
/*
void CWeaponRocketData::ChangeTarget(MSG_FC_BATTLE_CHANGE_TARGET_OK* pMsg)
{
	FLOG( "CWeaponRocketData::ChangeTarget(MSG_FC_BATTLE_CHANGE_TARGET_OK* pMsg)" );
	if(pMsg->TargetIndex < 10000)
	{
		if(pMsg->TargetIndex == g_pShuttleChild->m_myShuttleInfo.ClientIndex)
		{
			m_pTarget = g_pShuttleChild;
		}
		else
		{
			CMapEnemyIterator itEnemy = g_pScene->m_mapEnemyList.find(pMsg->TargetIndex);
			if(itEnemy != g_pScene->m_mapEnemyList.end())
			{
				m_pTarget = itEnemy->second;
			}
			else
			{
				m_pTarget = NULL;
			}
		}
	}
	else
	{
		CMapMonsterIterator itMonster = g_pScene->m_mapMonsterList.find(pMsg->TargetIndex);
		if(itMonster != g_pScene->m_mapMonsterList.end())
		{
			m_pTarget = itMonster->second;
		}
		else
		{
			m_pTarget = NULL;
		}
	}
	if(!m_pTarget)
	{
		m_vTargetPos = A2DX(pMsg->TargetPosition);
	}
	else
	{
		m_vTargetPos = m_pTarget->m_vPos;
	}
	m_vStartPos = m_vPos;
	D3DXVec3Normalize(&m_vVel,&(m_vTargetPos - m_vStartPos));
	m_fTargetDistance = D3DXVec3Length(&(m_vPos - m_vTargetPos));
	m_bSendData = FALSE;
	m_bSetTarget = FALSE;
	m_fTurnCheckTime = 0.0f;
}
*/
void CWeaponRocketData::Tick()
{
	FLOG( "CWeaponRocketData::Tick()" );
	if(m_pAttacker == NULL)
	{
		m_bUsing = FALSE;
		return;
	}
	if(m_nSkillNum == 0)
	{
		NormalTick();
	}
	else
	{
		SkillTick();
		// SKILL EFFECT
		SetBodyConditionMatrixForSkillEffect(m_bodyCondition,m_mMatrix);
	}
}

void CWeaponRocketData::CheckWeaponCollision(CItemData *pTargetItem,float fMovingDistance)
{
	int nTargetIndex = m_nTargetIndex;
//	BOOL bAttackAvailable = TRUE;// 공격가능한 유닛인지 검사..(일반 유저공격 막기)
	DWORD dwTargetState = _NORMAL; // 공격가능한 상태인지 파악..(적의 상태가 DEAD이면 공격하지 않음)

	CMonsterData*	pMonster = NULL;
	CEnemyData *pEnemy = NULL;		// 2005-12-07 by ispark, 롤링시스템을 사용시를 위해서

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
			pEnemy = itEnemy->second;		// 2005-12-07 by ispark
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
			m_pTarget = NULL;
			return;
		}
	}

//	if(bAttackAvailable && ATTACK_AVAILABLE_STATE(dwTargetState))
	if(ATTACK_AVAILABLE_STATE(dwTargetState))
	{
		// 충돌 검사
		BOOL bCollision = FALSE;
		float fRocketSize = 5.0f;
		fMovingDistance = fMovingDistance < 10.0f ? 10.0f : fMovingDistance;
		D3DXVECTOR3 vSide,vUpTemp;
		D3DXMATRIX mat;
		D3DXVec3Cross(&vSide,&m_vUp,&m_vVel);
		D3DXVec3Cross(&vUpTemp,&m_vVel,&vSide);
		D3DXMatrixLookAtLH( &mat, &m_vPos, &(m_vPos+m_vVel), &vUpTemp);
		float fDist;

		// 충돌 검사
		if( m_pTarget )
		{
			if( m_pTarget->m_dwPartType == _MONSTER && pMonster && pMonster->m_pMonMesh)
			{
				float fMonsterRadius = pMonster->m_pMonMesh->m_fRadius;
				if(fMonsterRadius > BIG_MONSTER_SIZE ) // 대형 몬스터
				{
					if(D3DXVec3Length(&(m_vPos - m_pTarget->m_vPos)) < fMonsterRadius + fRocketSize + fMovingDistance)
					{
						pMonster->m_pMonMesh->Tick(pMonster->m_fCurrentTime);
						pMonster->m_pMonMesh->SetWorldMatrix(pMonster->m_mMatrix);
						float fcollDist = pMonster->m_pMonMesh->CheckCollision(mat,m_vPos,DEFAULT_COLLISION_DISTANCE,TRUE,FALSE).fDist;
						if(fcollDist < fMovingDistance )
						{
							bCollision = TRUE;
						}
					}		
				}
			}
			// 2005-12-14 by ispark, 로켓시 롤링 충돌 처리
			if(pMonster == NULL || pMonster->m_pMonMesh == NULL)
			{
				if(m_nSkillNum == 0 && m_pTarget && 
					(D3DXVec3Length(&(m_vStartPos - m_vPos)) >= m_fTargetDistance ||
					D3DXVec3Length(&(m_vPos - m_pTarget->m_vPos)) < m_pTarget->m_fObjectSize + 5.0f))
				{
					bCollision = RollingCollision(pEnemy);				
					if(bCollision == FALSE)
					{
						SendFieldSocketBattleAttackEvasion(nTargetIndex, pTargetItem==NULL ? 0:pTargetItem->m_nItemIndex,m_nClientIndex,m_pItemData->ItemNum);
					}
				}
			}
			else if(m_nSkillNum == 0 && D3DXVec3Length(&(m_vStartPos - m_vPos)) >= m_fTargetDistance)
			{
				bCollision = TRUE;
			}
			else if(m_nSkillNum != 0 && D3DXVec3Length(&(m_vPos - m_pTarget->m_vPos)) < m_pTarget->m_fObjectSize + 5.0f)
			{
				bCollision = TRUE;
			}
		}
	
//		if( m_pTarget && D3DXVec3Length(&(m_vPos - m_pTarget->m_vPos)) < m_pTarget->m_fObjectSize + 5.0f)
//		{
//			bCollision = TRUE;
//		}
		//지상 충돌
		if(!bCollision)
		{
			D3DXVECTOR3 vSide,vUpTemp;
			D3DXMATRIX mat;
			D3DXVec3Cross(&vSide,&m_vUp,&m_vVel);
			D3DXVec3Cross(&vUpTemp,&m_vVel,&vSide);
			D3DXMatrixLookAtLH( &mat, &m_vPos, &(m_vPos+m_vVel), &vUpTemp);
			fDist = g_pScene->m_pObjectRender->CheckCollMeshRangeObject(mat,m_vPos,fMovingDistance).fDist;
			if(fDist < fMovingDistance*8)
			{
				nTargetIndex = 0;
				bCollision = TRUE;
				m_pTarget = NULL;
				m_dwWeaponState = _EXPLODING;
			}
			if(!bCollision &&
				IsTileMapRenderEnable(g_pShuttleChild->m_myShuttleInfo.MapChannelIndex.MapIndex)) // 2005-12-07 by ispark, 바닥을 렌더링을 하면 충돌 검사
			{
				// 바닥 충돌
				float fHeight = g_pGround->CheckHeightMap(m_vPos + m_vVel*fMovingDistance);
				if(fHeight - 30.0f > m_vPos.y)
				{
					nTargetIndex = 0;
					m_pTarget = NULL;
					bCollision = TRUE;
					m_dwWeaponState = _EXPLODING;
				}
			}
		}
		if(bCollision)
		{
//			if(m_pTarget)
//				DBGOUT("len = %f \n", D3DXVec3Length(&(m_vPos - m_pTarget->m_vPos)));
			// 폭발 반경안에 들면 서버에게 보냄 충돌
//			DBGOUT("len = %f, ItemName %s, Exp = %d\n",D3DXVec3Length(&(m_vPos - m_pTarget->m_vPos)), m_pItemData->ItemName, m_pItemData->ExplosionRange);

// 			if(m_pTarget &&
// 				D3DXVec3Length(&(m_vPos - m_pTarget->m_vPos)) <= m_pItemData->ExplosionRange)
// 			{
// 				SendFieldSocketBattleAttackFind(nTargetIndex, pTargetItem==NULL ? 0:pTargetItem->m_nItemIndex,m_nClientIndex,m_pItemData->ItemNum);
// 				m_dwWeaponState = _EXPLODING;
// 			}
			
// 2013-07-12 by bhsohn 멀티 타겟 몬스터 2형 타겟 잘못 잡히는 버그 수정
			if(m_pTarget)
			{				
				D3DXVECTOR3	 vTargetPos = m_pTarget->m_vPos;
				if(_MONSTER == m_pTarget->m_dwPartType)					
				{
					CMonsterData* pTargetMonster = (CMonsterData*)m_pTarget;
					if(pTargetMonster->m_nindexSize > 0 )
					{
						vTargetPos = pTargetMonster->GetMultiPos( pTargetMonster->m_nMultiIndex );
					}															
				}
				if(D3DXVec3Length(&(m_vPos - vTargetPos)) <= m_pItemData->ExplosionRange)
				{
					SendFieldSocketBattleAttackFind(nTargetIndex, pTargetItem==NULL ? 0:pTargetItem->m_nItemIndex,m_nClientIndex,m_pItemData->ItemNum);
					m_dwWeaponState = _EXPLODING;
				}

			}
// END 2013-07-12 by bhsohn 멀티 타겟 몬스터 2형 타겟 잘못 잡히는 버그 수정

/*			// 프로토콜 전송
			if(m_nClientIndex == g_pShuttleChild->m_myShuttleInfo.ClientIndex)// 클라이언트(CShuttleChild)
			{
				if(pTargetItem)
				{
					SendFieldSocketBattleAttackFind(nTargetIndex, pTargetItem->m_nItemIndex);
				}
				else
				{
					SendFieldSocketBattleAttackFind(nTargetIndex, 0);
				}
			}
			else // 몬스터 2형 무기인 경우
			{
				if(pTargetItem)
				{
					SendFieldSocketBattleMonsterAttackItemFind(nTargetIndex, pTargetItem->m_nItemIndex);
				}
				else
				{
					SendFieldSocketBattleMonsterAttackFind(nTargetIndex);
				}
			}
			*/
//			m_dwWeaponState = _EXPLODING;
		}
	}
}


void CWeaponRocketData::NormalTick()
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
				D3DXVECTOR3 vVel;
				D3DXVec3Normalize(&vVel, &(m_pAttacker->m_vPos - m_pTarget->m_vPos));
				m_vPos += vVel*SIZE_OF_SHIELD_EFFECT;
				((CUnitData*)m_pTarget)->CreateSecondaryShieldDamage(m_pAttacker->m_vPos);
			}
			else if(m_pAttacker)	
			{
				// 랜덤하게 위치를 조정한다.
				D3DXVECTOR3 vVel;
				D3DXVec3Normalize(&vVel, &(m_pAttacker->m_vPos - m_pTarget->m_vPos));
				vVel = D3DXVECTOR3( vVel.x*((float)RANDI(0, 4)),vVel.y*((float)RANDI(0, 4)),vVel.z*((float)RANDI(0, 4)));
				m_vPos += vVel;
			}
		}
		SetBodyCondition(BODYCON_HIT_MASK);
		CheckTargetWarning();
		m_dwWeaponState = _EXPLODED;
		
	} 
	else if(m_dwWeaponState == _NORMAL)
	{
		CItemData *pTargetItem = g_pScene->FindFieldItemByFieldIndex( m_nTargetItemFieldIndex );
		m_fWeaponLifeCheckTime += fElapsedTime;
//		if(m_fWeaponLifeTime <= m_fWeaponLifeCheckTime)
		// 2006-01-17 by ispark
		// 몬스터가 쏜 로켓이면, 사거리를 강제로 증가
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
					m_vTargetPos = ((CMonsterData*)m_pTarget)->GetMultiPos(m_nMultiTargetIndex);
				}
				else if(m_pTarget)
				{
					m_vTargetPos = m_pTarget->m_vPos;
				}
				// end 2011. 03. 08 by jskim 인피3차 구현 - 넌 타겟 시스템	
			}
			else
			{
				m_bSetTarget = FALSE;
			}

			// MoveWeapon 대신 패턴 적용
			BOOL bResult;							// 목표점에 도달했는지 결과..
//			bResult=m_pCinema->Tick(m_vTargetPos);	// 결과가 FALSE 면 목표를 지났다.. 즉 도달했다.
			bResult=m_pCinema->WeaponTick(m_vTargetPos, fElapsedTime);	// 결과가 FALSE 면 목표를 지났다.. 즉 도달했다.
			EVENT_POINT ep;
			ep=m_pCinema->GetCurrentCinemaPoint();
			m_vPos = ep.vPosition;			// 위치
			m_vVel = ep.vDirection;			// 방향
			m_fWeaponSpeed = ep.fVelocity;	// 속력
			m_vUp = ep.vUpVector;
			vTargetPosition = ep.vTarget;
			if(bResult == FALSE)
			{
				nPatternType = TARGET_OFF;
			}

			// 내가 발사한 무기이거나 몬스터가 발사한 2형 무기일때 충돌 체크
			if(	m_nClientIndex == g_pShuttleChild->m_myShuttleInfo.ClientIndex ||
				(IS_SECONDARY_WEAPON_MONSTER(m_pItemData->Kind) &&

				// 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템
				// 몬스터 공격이면서 몬스터가 타겟이고 내가 위임받았다면 내가 충돌 처리를 한다
				( IS_SECONDARY_WEAPON_MONSTER(m_pItemData->Kind) &&
				IS_MONSTER_CLIENT_INDEX( m_nTargetIndex ) &&
				(m_nDelegateClientIdx == g_pShuttleChild->m_myShuttleInfo.ClientIndex) ) ||
				// end 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템

				m_nTargetIndex == g_pShuttleChild->m_myShuttleInfo.ClientIndex ))
			{
				float fMovingDistance = ep.fVelocity*fElapsedTime;
				CheckWeaponCollision(pTargetItem,fMovingDistance);
			}
			else if(m_nTargetIndex == g_pShuttleChild->m_myShuttleInfo.ClientIndex)
			{
				// 내가 타겟이고 내 화면에 보여할 부분
				if(m_pTarget)
				{
					if(m_bEvasion)
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

void CWeaponRocketData::SkillTick()
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
				//vVel = D3DXVECTOR3( vVel.x*((float)(rand()%5)),vVel.y*((float)(rand()%5)),vVel.z*((float)(rand()%5)));
				//m_vPos = m_pTarget->m_vPos + vVel;
			}
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
		m_fWeaponLifeCheckTime += fElapsedTime;
		if(m_fWeaponLifeTime <= m_fWeaponLifeCheckTime)
		{
			m_dwWeaponState = _EXPLODING;
			m_pTarget = NULL;
		}
		else
		{
//			if(pTargetItem && pTargetItem->m_dwState == _NORMAL)
//			{
//				m_vTargetPos = pTargetItem->m_vPos;
//			}
//			else if(m_pTarget && ATTACK_AVAILABLE_STATE(m_pTarget->m_dwState))
//			{
//				m_vTargetPos = m_pTarget->m_vPos;
//			}
//			else
//			{
//				m_bSetTarget = FALSE;
//			}

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
			if(bResult == FALSE)
			{
				nPatternType = TARGET_OFF;
			}

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


void CWeaponRocketData::CheckTargetByBomb(float fMovingDistance)
{
	// 2007-04-30 by bhsohn 타켓 버그
	int nTargetIndex=0;
	CItemData* pTargetItem = NULL;
	BOOL bCollision = FALSE;
	float fRocketSize = 15.0f;
	
	// Enemy 충돌 검사
	if(!bCollision)
	{
		// 2005-08-11 by ispark
		// 지상폭격 조건 수정
		// Enemy는 타겟 잡힌 한명만 공격, 주변은 무시
		if(	g_pShuttleChild->m_pOrderTarget != NULL								&&
			g_pShuttleChild->m_pOrderTarget->m_dwPartType == _ENEMY				&&
			(((CUnitData*)g_pShuttleChild->m_pOrderTarget)->IsPkAttackEnable()	||		// 1. 강제 공격이 아니거나
//			g_pShuttleChild->m_pPkNormalTimer->IsPkEnableNormalOrderTarget()))			// 2. Delay Time 이 없거나
			g_pShuttleChild->IsEnemyPKAttackTime(m_nTargetIndex)))	// 2005-11-03 by ispark	  Delay Time 이 없다
		{
			CEnemyData* pTarget = (CEnemyData*)g_pShuttleChild->m_pOrderTarget;
			// 2007-04-30 by bhsohn 타켓 버그
			if( ATTACK_AVAILABLE_STATE(pTarget->m_dwState)
				&& pTarget && D3DXVec3Length(&(m_vPos - pTarget->m_vPos)) < pTarget->m_fObjectSize + fRocketSize + fMovingDistance)
			{
				nTargetIndex = pTarget->m_infoCharacter.CharacterInfo.ClientIndex;
				pTargetItem = g_pScene->FindFieldItemByPartTypeAndParent( _DUMMY, pTarget );
				bCollision = TRUE;
			}
		}
	}

	// Monster 충돌 검사
	if(!bCollision)
	{
		CMapMonsterIterator it = g_pScene->m_mapMonsterList.begin();
		while( it != g_pScene->m_mapMonsterList.end() )
		{
			CMonsterData* pTarget = it->second;
			if( pTarget && D3DXVec3Length(&(m_vPos - pTarget->m_vPos)) < 
				(pTarget->m_pMonMesh!=NULL?pTarget->m_pMonMesh->m_fRadius : pTarget->m_fObjectSize) + fRocketSize + fMovingDistance)
			{
				// 2004-10-22 by jschoi 정밀한 충돌검사 실행
				D3DXMatrixLookAtLH( &m_mMatrix, &(m_vPos), &(m_vPos + m_vVel), &m_vUp);
				pTarget->m_pMonMesh->Tick(pTarget->m_fCurrentTime);
				pTarget->m_pMonMesh->SetWorldMatrix(pTarget->m_mMatrix);
				float fcollDist = pTarget->m_pMonMesh->CheckCollision(m_mMatrix,m_vPos,DEFAULT_COLLISION_DISTANCE,TRUE,FALSE).fDist;
				if(fcollDist < fRocketSize + fMovingDistance)
			{
				nTargetIndex = pTarget->m_info.MonsterIndex;
				pTargetItem = g_pScene->FindFieldItemByPartTypeAndParent( _DUMMY, pTarget );
				bCollision = TRUE;
				break;
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
		
		// 2004-10-23 by jschoi
		// 여기서 몬스터, Enemy 의 폭발반경에 대해 충돌 처리해야함.
		if(	bCollision && nTargetIndex == 0 )
		{
			float fLengthMin = 1000.0f;
			CMapEnemyIterator it = g_pScene->m_mapEnemyList.begin();
			while( it != g_pScene->m_mapEnemyList.end() )
			{
				if(	it->second->IsPkEnable())
				{
					CEnemyData* pTarget = it->second;
					if (pTarget == NULL)
						continue; 
					
					float fLengthTemp = D3DXVec3Length(&(m_vPos - pTarget->m_vPos));
					if( pTarget && fLengthTemp < pTarget->m_fObjectSize + m_pItemData->ExplosionRange)
					{
						if(fLengthMin > fLengthTemp)
					{
						nTargetIndex = pTarget->m_infoCharacter.CharacterInfo.ClientIndex;
						pTargetItem = g_pScene->FindFieldItemByPartTypeAndParent( _DUMMY, pTarget );
							fLengthMin = fLengthTemp;
						bCollision = TRUE;
						}
//						break;
					}
				}
				it++;
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
				if( pTarget && fLengthTemp < 
					(pTarget->m_pMonMesh!=NULL?pTarget->m_pMonMesh->m_fRadius : pTarget->m_fObjectSize) + m_pItemData->ExplosionRange)
				{
					if(fLengthMin > fLengthTemp)
					{
					nTargetIndex = pTarget->m_info.MonsterIndex;
					pTargetItem = g_pScene->FindFieldItemByPartTypeAndParent( _DUMMY, pTarget );
						fLengthMin = fLengthTemp;
					bCollision = TRUE;
					}
//					break;
				}
				it++;
			}
		}
	}
	
	if(bCollision)
	{
		SendFieldSocketBattleAttackFind(nTargetIndex, pTargetItem==NULL ? 0:pTargetItem->m_nItemIndex,m_nClientIndex,m_pItemData->ItemNum);
		m_dwWeaponState = _EXPLODING;
	}

}

void CWeaponRocketData::CheckTargetWarning()
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