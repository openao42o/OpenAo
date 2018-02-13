// NPCMonster.cpp: implementation of the CNPCMonster class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NPCMonster.h"
#include "NPCGlobal.h"
#include "NPCMapProject.h"
#include "NPCMapChannel.h"

// start 2011-05-23 by hskim, 인피니티 3차 - 웨이포인트 구현
#include "NPCIOCP.h"
// end 2011-05-23 by hskim, 인피니티 3차 - 웨이포인트 구현

const char * GNPCGetStringVector3(D3DXVECTOR3 *i_pVec3Pos, string &str)
{
	char buffer[512];
	sprintf(buffer, "%4d,%3d,%4d", (int)i_pVec3Pos->x, (int)i_pVec3Pos->y, (int)i_pVec3Pos->z);
	str += buffer;
	return str.c_str();
}

//////////////////////////////////////////////////////////////////////
// Local Global Function
//////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// 현재 Target Vector을 기준으로 회전 시킨 Target Vector를 구하는 함수
// vecTarget	: 기준이 되는 Target Vector
// enDirection	: 회전 시킨 값
// return		: 기준 Target Vector에서 회전 시킨 Vector
int GNPCRotateTargetVectorHorizontal(D3DXVECTOR3 *pDestTargetVector3, const D3DXVECTOR3 *pSourTargetVector3, int enDirection, int nRandomRange)
{
	D3DXMATRIX	mat;
	D3DXVECTOR3	vUp(0, 1, 0);

	if(MSD_TARGET_180 == enDirection
		|| nRandomRange == 0)
	{
		if(MSD_TARGET == enDirection)
		{
			*pDestTargetVector3 = *pSourTargetVector3;
		}
		else if(MSD_TARGET_180 == enDirection)
		{
			*pDestTargetVector3 = -(*pSourTargetVector3);
		}
		else
		{
			D3DXMatrixRotationAxis(&mat, &vUp, PI * enDirection/180);				// UpVector를 기준으로 회전하는 메트릭스를 만든다.
			D3DXVec3TransformCoord(pDestTargetVector3, pSourTargetVector3, &mat);	// 위에서 만든 메트릭스로 vecTarget를 회전 시킨다.
		}
	}
	else
	{
		if(enDirection > 0)
		{
			enDirection = (Unit_Move_Direction)(enDirection + RANDI(0, nRandomRange));
		}
		else
		{
			enDirection = (Unit_Move_Direction)(enDirection - RANDI(0, nRandomRange));
		}

		D3DXMatrixRotationAxis(&mat, &vUp, PI * enDirection/180);					// UpVector를 기준으로 회전하는 메트릭스를 만든다.
		D3DXVec3TransformCoord(pDestTargetVector3, pSourTargetVector3, &mat);		// 위에서 만든 메트릭스로 vecTarget를 회전 시킨다.
	}

	//pDestTargetVector3->y = 0.0f;
	D3DXVec3Normalize(pDestTargetVector3, pDestTargetVector3);
	return enDirection;
}


////////////////////////////////////////////////////////////////////////////////
//
//  설      명 : pSourTargetVector3벡터를 수직(위, 아래)방향으로 회전 시키는 함수
//	리  턴  값 : int, 회전된 각, 360도 기준
//	pDestTargetVector3 : 결과로 회전된 방향벡터
//	pSourTargetVector3 : 단위 벡터로 이 벡터를 회전시킨다.
//	enDirection	: 뱡향기 기본 각을 나타낸다( 30 => 위로 30도, -45 => 아래로 45)
//  nRandomRange : enDirection을 기본각으로 랜덤각을 나타낸다
int GNPCRotateTargetVectorVertical(D3DXVECTOR3 *pDestTargetVector3, const D3DXVECTOR3 *pSourTargetVector3, int enDirection, int nRandomRange)
{	
	if(MSD_TARGET_180 == enDirection
		|| nRandomRange == 0)
	{
		if(MSD_TARGET == enDirection)
		{
			*pDestTargetVector3 = *pSourTargetVector3;
		}
		else if(MSD_TARGET_180 == enDirection)
		{
			*pDestTargetVector3 = -(*pSourTargetVector3);
		}
		else
		{
			D3DXMATRIX	mat;
			D3DXVECTOR3	vHori;
			GNPCRotateTargetVectorHorizontal(&vHori, pSourTargetVector3, MSD_RIGHT_90, 0);
			vHori.y = 0.0f;
			D3DXVec3Normalize(&vHori, &vHori);
		
			D3DXMatrixRotationAxis(&mat, &vHori, PI * enDirection/180);				// UpVector를 기준으로 회전하는 메트릭스를 만든다.
			D3DXVec3TransformCoord(pDestTargetVector3, pSourTargetVector3, &mat);	// 위에서 만든 메트릭스로 vecTarget를 회전 시킨다.
		}
	}
	else
	{
		if(enDirection > 0)
		{
			enDirection = (Unit_Move_Direction)(enDirection + RANDI(0, nRandomRange));
		}
		else
		{
			enDirection = (Unit_Move_Direction)(enDirection - RANDI(0, nRandomRange));
		}
		
		D3DXMATRIX	mat;
		D3DXVECTOR3	vHori;
		GNPCRotateTargetVectorHorizontal(&vHori, pSourTargetVector3, MSD_RIGHT_90, 0);
		vHori.y = 0.0f;
		D3DXVec3Normalize(&vHori, &vHori);

		D3DXMatrixRotationAxis(&mat, &vHori, PI * enDirection/180);					// UpVector를 기준으로 회전하는 메트릭스를 만든다.
		D3DXVec3TransformCoord(pDestTargetVector3, pSourTargetVector3, &mat);		// 위에서 만든 메트릭스로 vecTarget를 회전 시킨다.
	}

	D3DXVec3Normalize(pDestTargetVector3, pDestTargetVector3);
	return enDirection;
}


////////////////////////////////////////////////////////////////////////////////
//
// 함 수 이 름  : GetMaxTargetVector
// 반환되는 형  : void
// 함 수 인 자  : D3DXVECTOR3 *pCurTarget
// 함 수 인 자  : D3DXVECTOR3 *pNextTarget
// 함 수 인 자  : float fMaxRotationAngle 최대 이동 가능한 각, 0.0872 = 3.14 * 0.027777(5도)
// 함 수 설 명  :
//
D3DXMATRIX GNPCGetMaxTargetVector(D3DXVECTOR3 *pCurTarget
							, D3DXVECTOR3 *pNextTarget
							, float fMaxRotationAngle)
{
	float		fTempAngel_0;
	D3DXVECTOR3	vecTemp;
	D3DXMATRIX	mat;

	fTempAngel_0 = ACOS(D3DXVec3Dot(pCurTarget, pNextTarget));
	if(fTempAngel_0 <= fMaxRotationAngle)						// 두개 Target Vector 각이 최대각보다 작다
	{
		*pCurTarget = *pNextTarget;
		D3DXVec3Cross(&vecTemp, pNextTarget, pCurTarget);
		D3DXMatrixRotationAxis(&mat, &vecTemp, -(fTempAngel_0));
		return mat;
	}

	D3DXVec3Cross(&vecTemp, pNextTarget, pCurTarget);

	///////////////////////////////////////////////////////////////////////////////
	// 1. 회전해야할 각이 최대각의 두배보다 크면 최대각을 회전하고
	// 2. 회전해야할 각이 최대각의 두배보다 크지 않으면 최대각/2을 회전한다.
	if(fTempAngel_0 > fMaxRotationAngle*2)
	{
		D3DXMatrixRotationAxis(&mat, &vecTemp, -(fMaxRotationAngle));
	}
	else
	{
		D3DXMatrixRotationAxis(&mat, &vecTemp, -(fMaxRotationAngle/2));
	}	
	D3DXVec3TransformCoord(pCurTarget, pCurTarget, &mat);
	return mat;
}


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNPCMonster::CNPCMonster()
{	
}

CNPCMonster::~CNPCMonster()
{
// 20040528 cmkwon, 2형 무기는 NPC에서 발사만 하고 관리는 하지 않는다, 이동이나 타격은 Client에서 처리한다.
//	///////////////////////////////////////////////////////////////////////////
//	// 발사한 미사일 삭제 처리
//	if(m_mtvectorShootedSecondaryItemInfoPtr.empty() == false)
//	{
//		for (auto x : m_mtvectorShootedSecondaryItemInfoPtr) delete x;
//		m_mtvectorShootedSecondaryItemInfoPtr.clear();
//	}

	///////////////////////////////////////////////////////////////////////////
	// 몬스터가 공격당한 ItemInfo 벡터 삭제 처리
	m_mtvectorAttackedItemInfo.lock();
	m_mtvectorAttackedItemInfo.clear();
	m_mtvectorAttackedItemInfo.unlock();	
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			void CNPCMonster::SetTargetIndex(ClientIndex_t i_nTargetIdx)
/// \brief		몬스터의 TargetIndex를 설정한다.
/// \author		cmkwon
/// \date		2004-04-02 ~ 2004-04-02
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CNPCMonster::SetTargetIndex(ClientIndex_t i_nTargetIdx)
{
	if(i_nTargetIdx == 0)
	{		
	}
	else
	{
		///////////////////////////////////////////////////////////////////
		// 자신이 몬스터파티장이면 파티를 초기화(해산)한다.
		if(m_nPartyManagerIndex == MonsterIndex)
		{
			this->ResetPartyVariable();
		}

		///////////////////////////////////////////////////////////////////////////////////
		// ShotNumCount는 모두 초기화 한다.
		// AttackTime도 초기화 해야한다.
		// 1. m_nTargetIndex가 0이면 ReAttackTime은 초기화 한다.
		// 2. m_nTargetIndex가 0이 아니면 ReAttackTime은 초기화 하지 않는다		
		if(m_nTargetIndex == 0)
		{
			ResetAllLastReAttackTime(m_dwCurrentTick);
			this->SetTimeSetEnforceTargetVector(m_dwCurrentTick);
		}
		ResetCurrentShotNumCount();
		ResetLastAttackTime(m_dwCurrentTick);

		// 2009-12-11 by cmkwon, 데미지 어그로로 타겟을 변경하는 몬스터 구현 - CMonster 클래스로 이동
		//m_dwSetTargetIndexLastTick	= m_dwCurrentTick;
		m_dwTimeMonsterLastAttack	= m_dwCurrentTick;
		
		///////////////////////////////////////////////////////////////////////////
		// 2004-03-29 by cmkwon, 임시 방편으로 처리 되어있는것 공격 받으면 CurrentMonsterForm 을 변경한다.
		// 현재는 렛티만 처리
		if(MonsterInfoPtr->MonsterUnitKind == 2003300
			&& CurrentMonsterForm == FORM_GROUND_MOVE)
		{
			this->SetCurrentMonsterForm(FORM_FLYING_RIGHT);
			SetMonsterMoveInfo();
		}		
	}
	
	////////////////////////////////////////////////////////////////////////////////
	// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 타겟 변경시 대화 전송
	if(i_nTargetIdx != m_nTargetIndex
		&& 0 != i_nTargetIdx) {
		HPACTION_TALK_HPRATE MsgTalk;
		util::zero(&MsgTalk, sizeof(HPACTION_TALK_HPRATE));
		if(this->m_HPAction.GetTalkTargetChange(MsgTalk.HPTalk)) {
			// Attack값 전송.
			MsgTalk.HPTalkImportance	= HPACTION_TALK_IMPORTANCE_CHANNEL;
			MsgTalk.TargetClientIdx		= i_nTargetIdx;
			this->m_CurrentNPCMapChannel->SendFSvrHPTalk(this, &MsgTalk);
		}
	}

	int nOldTargetIdx = m_nTargetIndex;	// 2009-12-10 by cmkwon, TEMP 

	m_nTargetIndex = i_nTargetIdx;

	this->SetNPCMonsterFlagMoveTargetVectorSetAttack(FALSE);

	this->SetChangedTargetIndexTick(m_dwCurrentTick);		// 2009-12-11 by cmkwon, 데미지 어그로로 타겟을 변경하는 몬스터 구현 - 

	this->SetTimeCheckValidTarget(m_dwCurrentTick);			// 2010-01-06 by cmkwon, 몬스터 공격시 타겟 공격 가능 체크 추가(타겟변경) - 
	this->SetTimeCheckedLastValidTarget(m_dwCurrentTick);	// 2010-01-06 by cmkwon, 몬스터 공격시 타겟 공격 가능 체크 추가(타겟변경) - 

	this->SetTimeLastCheckChangeTarget(m_dwCurrentTick);	// 2010-03-16 by cmkwon, 인피2차 MtoM, MtoC 타겟 변경 관련 수정 - 

// 2009-12-10 by cmkwon, TEMP 
//	g_pNPCGlobal->WriteSystemLogEX(TRUE, "[TEMP] infi2 100317 20000 CNPCMonster::SetTargetIndex# MonIdx(%d) OldTargetIdx(%d) nTargetIndex(%d) CurTick(%d) nAttackerCnt(%d)\r\n"
//		, this->MonsterIndex, nOldTargetIdx, m_nTargetIndex, m_dwCurrentTick, m_mtvectorAttackedInfoPtr.size());
}

void CNPCMonster::SetMoveState(SHORT i_MoveState)
{
	m_enMoveState			= i_MoveState;
	m_MoveInfo.MoveCount	= 0;

	if(MSS_NORMAL == m_enMoveState)
	{
		this->SetNPCMonsterFlagEnforceTargetVector(FALSE);
		this->SetTimeSetEnforceTargetVector(this->m_dwCurrentTick);
	}
	this->SetSpeedDirectMoveInfo(MSD_SPEED_UP);
}


void CNPCMonster::SetMoveTargetVector(D3DXVECTOR3 * i_pVector3)
{
	if(i_pVector3->x == 0.0f && i_pVector3->y == 0.0f && i_pVector3->z == 0.0f)
	{
		GetRandomPlaneUnitVector(i_pVector3);
	}
	D3DXVec3Normalize(&m_MoveTargetVector, i_pVector3);
}

void CNPCMonster::SetTargetVector(D3DXVECTOR3 * i_pVector3, BOOL i_bUnconditionalFlag/*=FALSE*/)
{
	if(FALSE == i_bUnconditionalFlag
		&& MonsterInfoPtr->MonsterForm == FORM_OBJECT_STOP)
	{
		return;
	}

	if(i_pVector3->x == 0.0f && i_pVector3->y == 0.0f && i_pVector3->z == 0.0f)
	{
		GetRandomPlaneUnitVector(i_pVector3);
	}
	D3DXVec3Normalize(&TargetVector, i_pVector3);

	if(FORM_OBJECT_PLANE_ROTATE == MonsterInfoPtr->MonsterForm)
	{
		CNPCMapProject::ChangePlaneUnitVec3(&TargetVector, &TargetVector, i_pVector3);
	}
}

void CNPCMonster::SetEnforceTargetVector(D3DXVECTOR3 * i_pVector3
										 , float i_fSpeed
										 , SHORT i_MoveState)
{
	if(FALSE == m_MoveInfo.MovableFlag){		return;}

	if(i_pVector3->x == 0.0f && i_pVector3->y == 0.0f && i_pVector3->z == 0.0f)
	{
		GetRandomPlaneUnitVector(i_pVector3);
	}

	///////////////////////////////////////////////////////////////////////////////
	// Flag를 TRUE로 설정한다.
	this->SetNPCMonsterFlagEnforceTargetVector(TRUE);
	SetTimeSetEnforceTargetVector(timeGetTime());

	if(TRUE == m_MoveInfo.FBFlag
		&& MSS_MAP_IMPACT != i_MoveState)
	{
		///////////////////////////////////////////////////////////////////////////////
		// 향할 TargetVector를 설정한다.
		D3DXVECTOR3	tmVec3;
		CNPCMapProject::ChangePlaneUnitVec3(&tmVec3, i_pVector3, i_pVector3);
		GNPCGetMaxTargetVector(&tmVec3, i_pVector3, MONSTER_COPTER_MAXMOVE_PINPOINT);
		m_EnforceTargetVector = tmVec3;	
		D3DXVec3Normalize(&m_EnforceTargetVector, &m_EnforceTargetVector);
	}
	else
	{
		///////////////////////////////////////////////////////////////////////////////
		// 향할 TargetVector를 설정한다.
		m_EnforceTargetVector = (*i_pVector3);	
		D3DXVec3Normalize(&m_EnforceTargetVector, &m_EnforceTargetVector);
	}

	///////////////////////////////////////////////////////////////////////////////
	// Speed를 설정한다.
	this->SetCurrentSpeed(i_fSpeed);

	///////////////////////////////////////////////////////////////////////////////
	// MoveState가 설정된다.
	this->SetMoveState(i_MoveState);

	///////////////////////////////////////////////////////////////////////////////
	// MoveState에 따라 기본적으로 지속될 시간 설정
	USHORT usTimeTerm = 0;
	switch(m_enMoveState)
	{
	case MSS_MAP_IMPACT:
	case MSS_RANGE_DISTANCE_IMPACT:
		{
			if(m_MoveInfo.FBFlag)

				usTimeTerm	= RANDI(2000, 3000);

			else

				usTimeTerm	= RANDI(2000, 4000);

		}
		break;
	case MSS_OUT_OF_ATTACK_RANGE:
		{
			if(m_MoveInfo.FBFlag)
			{
				usTimeTerm	= RANDI(2000, 4000);
			}
			else
			{
				usTimeTerm	= RANDI(2000, 3000);
			}
		}
		break;
	case MSS_QUICK_TURN_GENERAL:
		{
			if(MonsterInfoPtr->ItemInfo[ARRAY_INDEX_MONSTER_SKILL_ITEM].pItemInfo
				&& RAND100() < 30)
			{
				this->SetMoveState(MSS_QUICK_TURN_SKILL);
			}
			else
			{
 				float fMaxDistance = this->MonsterInfoPtr->Range;			
				// todo : look at this
				usTimeTerm = 1000 * ((fMaxDistance*0.4f)/(this->GetQuickSpeed()*5))
					+ RANDI(0, (1000 * ((fMaxDistance*0.7f)/(this->GetQuickSpeed()*5))) - 1);
			}
		}
		break;
	}
	this->SetTimeEnforceTargetVectorTerm(usTimeTerm);
}

void CNPCMonster::ResetPartyVariable(void)
{
	m_nPartyManagerIndex			= 0;
	m_byPartyMemberCounts			= 0;
	m_byPartyFormationIndex			= 0xFF;
}

void CNPCMonster::ChangeBodyCondition(BodyCond_t * pBodyCon)
{
#ifdef _DEBUG
//	if(m_pUsingMonsterItem)
//	{
//	
//	DBGOUT("		Change BodyCondition Current(0x%08X %08X) ==> New(0x%08X %08X)	ItemIndex(%d) ItemName(%s) SpeedPercent(%d)\n"
//		, *((int*)&BodyCondition + 1), *((int*)&BodyCondition)
//		, *((int*)pBodyCon + 1), *((int*)pBodyCon)
//		, m_pUsingMonsterItem->byArrayIndex, m_pUsingMonsterItem->pItemInfo->ItemName
//		, m_usSpeedPercent);
//	DBGOUT("	Change BodyCon Current(%08X) ==> New(%08X) ItemIndex(%d) ItemName(%s) SpeedPercent(%3d), CurTick(%d)\n"
//		, *((int*)&BodyCondition), *((int*)pBodyCon)
//		, m_pUsingMonsterItem->byArrayIndex, m_pUsingMonsterItem->pItemInfo->ItemName
//		, m_usSpeedPercent, m_dwCurrentTick);
//	}
#endif

	BodyCondition = *pBodyCon;	
}

void CNPCMonster::ResetAttackBodyCondition(void)
{
	BodyCond_t tmBodyCon = this->BodyCondition;
	CLEAR_BODYCON_BIT(tmBodyCon, BODYCON_MON_ATTACKALL_MASK);
	this->ChangeBodyCondition(&tmBodyCon);
}


void CNPCMonster::GetRandomPlaneUnitVector(D3DXVECTOR3 * o_pVec3Tar)
{
	o_pVec3Tar->x = RANDF(-0.99f, 0.99f);
	o_pVec3Tar->y = 0.0f;
	o_pVec3Tar->z = RANDF(-0.99f, 0.99f);
	if(o_pVec3Tar->x == 0.0f && o_pVec3Tar->z == 0.0f)
	{
		switch(RANDI(0, 3))
		{
		case 0:		o_pVec3Tar->x = 0.50f;		o_pVec3Tar->z = 0.50f;	break;
		case 1:		o_pVec3Tar->x = 0.50f;		o_pVec3Tar->z = -0.50f;	break;
		case 2:		o_pVec3Tar->x = -0.50f;		o_pVec3Tar->z = 0.50f;	break;
		default:{	o_pVec3Tar->x = -0.50f;		o_pVec3Tar->z = -0.50f;	}
		}
	}
	D3DXVec3Normalize(o_pVec3Tar, o_pVec3Tar);
}

void CNPCMonster::GetRandomVector(D3DXVECTOR3 * o_pVec3Tar)
{
	o_pVec3Tar->x = RANDF(-0.99f, 0.99f);
	o_pVec3Tar->y = RANDF(-0.90f, 0.90f);
	o_pVec3Tar->z = RANDF(-0.99f, 0.99f);
	if(o_pVec3Tar->x == 0.0f && o_pVec3Tar->z == 0.0f)
	{
		switch(RANDI(0, 3))
		{
		case 0:		o_pVec3Tar->x = 0.50f;		o_pVec3Tar->z = 0.50f;	break;
		case 1:		o_pVec3Tar->x = 0.50f;		o_pVec3Tar->z = -0.50f;	break;
		case 2:		o_pVec3Tar->x = -0.50f;		o_pVec3Tar->z = 0.50f;	break;
		default:{	o_pVec3Tar->x = -0.50f;		o_pVec3Tar->z = -0.50f;	}
		}
	}
	D3DXVec3Normalize(o_pVec3Tar, o_pVec3Tar);
}

struct sort_SSUMMON_EVENT_MONSTER_By_SummonDelayTime			// 2008-04-16 by cmkwon, 몬스터 사망 시 몬스터 소환 이벤트 시스템 구현 - 
{
	bool operator()(SSUMMON_EVENT_MONSTER op1, SSUMMON_EVENT_MONSTER op2)
	{
		return op1.SummonDelayTime < op2.SummonDelayTime;		// 오름차순 정렬
	}
};

// Makes a list consisting of the given summon list plus
// the summons from items and shuffles it
void CNPCMonster::SetSummonEventMonsterListAfterDead(mtvectSSUMMON_EVENT_MONSTER *i_pSummonEvMonList)
{
	mt_auto_lock mtA { m_summonEventMonsterListAfterDead };
	
	m_summonEventMonsterListAfterDead.clear();
		
	if (MonsterInfoPtr) for (auto& inf : MonsterInfoPtr->ItemInfo)
	{
		auto pMonItem = inf.pItemInfo;
		
		// Charging=0 인 아이템이 죽을 때 사용하는 몬스터 아이템
		if (pMonItem && pMonItem->Charging == 0 && pMonItem->ArrDestParameter[0] == DES_SUMMON)
		{
			SSUMMON_EVENT_MONSTER summonEvMon;

			util::zero(summonEvMon);

			// 명시
			summonEvMon.IsSummonChecked = false;

			summonEvMon.SummonMonsterNum = pMonItem->ArrParameterValue[0];

			summonEvMon.SummonMonsterCount = max(1, int(pMonItem->MultiNum * MonsterInfoBalance.fSummonCountRatio));
			// 초단위로 변경해서 넣어 준다
			summonEvMon.SummonDelayTime = pMonItem->AttackTime / 1000;
			// 10K 단위 확률로  변경
			summonEvMon.SummonProbability = Prob256_TO_Prob10K(pMonItem->HitRate);


			m_summonEventMonsterListAfterDead.push_back(summonEvMon);
		}
	}

	if (!i_pSummonEvMonList->empty())
		m_summonEventMonsterListAfterDead.insert(
			m_summonEventMonsterListAfterDead.end(),
			i_pSummonEvMonList->begin(),
			i_pSummonEvMonList->end());


	if (m_summonEventMonsterListAfterDead.size() >= 2) random::shuffle(m_summonEventMonsterListAfterDead);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CNPCMonster::CheckSummonEventMonsterListAfterDead(SSUMMON_EVENT_MONSTER *o_pSummonEventMonster)
/// \brief		// 2008-04-16 by cmkwon, 몬스터 사망 시 몬스터 소환 이벤트 시스템 구현 - CNPCMonster::CheckSummonEventMonsterListAfterDead() 추가
/// \author		cmkwon
/// \date		2008-04-16 ~ 2008-04-16
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CNPCMonster::CheckSummonEventMonsterListAfterDead(SSUMMON_EVENT_MONSTER *o_pSummonEventMonster)
{
	BOOL bRetDeleteProcess = TRUE;
	if(m_summonEventMonsterListAfterDead.empty())
	{
		return bRetDeleteProcess;
	}

	mt_auto_lock mtA(&m_summonEventMonsterListAfterDead);
	if(m_summonEventMonsterListAfterDead.empty())
	{
		return bRetDeleteProcess;
	}

	DWORD	dwElapsedTimeAfterDead = timeGetTime() - this->m_dwTimeDeath;
	mtvectSSUMMON_EVENT_MONSTER::iterator itr(m_summonEventMonsterListAfterDead.begin());
	for(; itr != m_summonEventMonsterListAfterDead.end(); itr++)
	{
		SSUMMON_EVENT_MONSTER *pSummonEvMon = &*itr;
		if(pSummonEvMon->IsSummonChecked)
		{// 2008-04-16 by cmkwon, 소환 체크 여부 체크
			continue;
		}

		if(MILLISECOND_TO_SECOND(dwElapsedTimeAfterDead) < pSummonEvMon->SummonDelayTime)
		{// 2008-04-16 by cmkwon, 지연시간 경과 체크
			bRetDeleteProcess = FALSE;
			break;
		}


		Prob10K_t nRandVal = RAND10K();
		if(nRandVal > pSummonEvMon->SummonProbability)
		{// 2008-04-16 by cmkwon, 소환 확률 체크 - 실패시 처리
			pSummonEvMon->IsSummonChecked	= TRUE;			
			continue;
		}

		*o_pSummonEventMonster	= *pSummonEvMon;
		return bRetDeleteProcess;					// 2008-04-16 by cmkwon, 1 가지가 소환 성공 하였기 때문에 Summoner 도 삭제 처리 해도 된다.
	}

	return bRetDeleteProcess;
}


void CNPCMonster::SetCurrentSpeed(float i_fSpeed)
{
	m_CurrentSpeed		= i_fSpeed;
}
float CNPCMonster::GetCurrentSpeed(void)
{
	if(COMPARE_BODYCON_BIT(BodyCondition, BODYCON_MON_PREATTACK_ALL_MASK))
	{
		return (m_CurrentSpeed * m_pUsingMonsterItem->pItemInfo->SpeedPenalty/100.0f);
	}
	return (m_CurrentSpeed * m_usSpeedPercent/100.0f);
}


void CNPCMonster::SetTimeSetEnforceTargetVector(DWORD i_dwCurTick)
{
	m_dwTimeSetEnforceTargetVector = i_dwCurTick;
}

void CNPCMonster::SetTimeEnforceTargetVectorTerm(USHORT i_usTerm)
{
	m_usTimeEnforceTargetVectorTerm = i_usTerm;
}

void CNPCMonster::SetTimeQuickTurnTerm(USHORT i_usTerm)
{
	m_usTimeQuickTurnTerm = i_usTerm;
}

void CNPCMonster::SetSpeedDirectMoveInfo(SHORT i_sSpeedDirect)
{
	m_MoveInfo.SpeedDirect = i_sSpeedDirect;
}


void CNPCMonster::SetNPCMonsterFlagEnforceTargetVector(BOOL i_bFlag)
{
	m_FlagNPCMonster.EnforceTargetVector = i_bFlag;
	if(FALSE == i_bFlag
		&& this->m_MoveInfo.FBFlag)
	{
		this->SetTimeQuickTurnTerm(1000 + timeGetTime()%MonsterInfoPtr->QuickTurnTerm);
	}
}
void CNPCMonster::SetNPCMonsterFlagMoveTargetVectorSetAttack(BOOL i_bFlag)
{
	m_FlagNPCMonster.MoveTargetVectorSetAttack = i_bFlag;
}

void CNPCMonster::SetCurrentMonsterForm(SHORT i_sMonForm)
{
	CurrentMonsterForm = i_sMonForm;
}

void CNPCMonster::SetCurrentMovePattern(SHORT i_sMovePat)
{
	CurrentMovePattern = i_sMovePat;
}

void CNPCMonster::SetUsingMonsterItem(MONSTER_ITEM *i_pMonItem)
{
// 2010-07-05 by jskim, NPC서버 죽는 버그 수정 - 보안2
//	if(m_pUsingMonsterItem != i_pMonItem)
	if(i_pMonItem == NULL)
	{
		g_pNPCGlobal->WriteSystemLogEX(TRUE,"CheckMonsterSelectedItem Error");
	}
	if(NULL != i_pMonItem
		&& m_pUsingMonsterItem != i_pMonItem)
//end 2010-07-05 by jskim, NPC서버 죽는 버그 수정 - 보안2
	{
		m_dwTimeChangeUsingMonsterItem	= timeGetTime();
		m_pUsingMonsterItem				= i_pMonItem;	
	}

#ifdef _DEBUG
//	if(m_pUsingMonsterItem)
//	{
//		DBGOUT("	Index(%d) ItemName(%10s) ReAttackTime(%5d) AttackTime(%4d)	==> ReattackTerm(%d)\n"
//			, m_pUsingMonsterItem->byArrayIndex, m_pUsingMonsterItem->pItemInfo->ItemName
//			, m_pUsingMonsterItem->pItemInfo->ReAttacktime, m_pUsingMonsterItem->pItemInfo->AttackTime
//			, m_dwCurrentTick - m_ArrLastReattackTime[m_pUsingMonsterItem->byArrayIndex]);
//	}
#endif
}

void CNPCMonster::SetCurrentTick(DWORD i_dwCurTick)
{
	m_dwCurrentTick = i_dwCurTick;
}

//////////////////////////////////////////////////////////////////////
// Method / 이동 관련
//////////////////////////////////////////////////////////////////////

/************************************************************************************
**
**	Method / 생성 관련.
**
**	Create Info : ??. ??. ??
**
**	Update Info : // 2010. 05. 19 by hsLee 인피니티 필드 2차 난이도 조절. (신호처리 + 몬스터 처리(서버) ) - 몬스터 확장 정보 추가.
**
*************************************************************************************/
void CNPCMonster::CreateNPCMonster(MONSTER_INFO * i_pMonInfo
								   , D3DXVECTOR3 *i_pCreatePos
								   , DWORD i_dwCurTick
								   , int i_CreateInfoIndex
								   , BYTE i_byMonTargetType
								   , int i_nTargetTypeData
								   , ClientIndex_t i_cltIdxForTargetType
								   , int i_nMapMonsterVisibleDiameter
								   , int i_nTargetIndex/*=0*/
								   , BYTE i_byObjectMonsterType/*=EVENT_TYPE_NO_OBJECT_MONSTER*/
								   , D3DXVECTOR3 *i_pUnitVec3ObjMonsterTarget/*=NULL*/
								   , MONSTER_BALANCE_DATA * i_pMonInfoBalance /*= NULL*/
								   )
{
	this->SetMonsterInfoPtr(i_pMonInfo);
	
	// 2010. 05. 19 by hsLee 인피니티 필드 2차 난이도 조절. (신호처리 + 몬스터 처리(서버) )
	if ( i_pMonInfoBalance )
		SetMonsterInfoExtend_AllData ( i_pMonInfoBalance );

	//CurrentHP					= MonsterInfoPtr->MonsterHP;
	CurrentHP					= MonsterInfoExtend.fMaxHP;
	// End 2010. 05. 19 by hsLee 인피니티 필드 2차 난이도 조절. (신호처리 + 몬스터 처리(서버) )


	BodyCondition				= 0;
	SET_BODYCON_BIT(BodyCondition, BODYCON_CREATION_MASK);
	m_dwIndexCreatedMonsterData	= i_CreateInfoIndex;
	PositionVector				= *i_pCreatePos;
	m_BeforePosition			= *i_pCreatePos;
	m_CreatedPosition			= *i_pCreatePos;

	m_dwTimeCreated				= i_dwCurTick;
	m_byObjectMonsterType		= i_byObjectMonsterType;
	m_dwTimeLastMoved			= i_dwCurTick;

	m_enMonsterState			= MS_CREATED;

	m_byMonsterTargetType		= i_byMonTargetType;
	m_nTargetTypeData			= i_nTargetTypeData;
	if(0 != i_cltIdxForTargetType)
	{
		m_mtvectClientIdxForTargetType.pushBackLock(i_cltIdxForTargetType);
	}

	m_nTargetIndex				= i_nTargetIndex;
	if(MONSTER_TARGETTYPE_TUTORIAL == m_byMonsterTargetType
		&& 0 != i_cltIdxForTargetType)
	{// 2007-07-19 by cmkwon, 튜토리얼 몬스터 수정 - 초기 타겟 설정
		m_nTargetIndex				= i_cltIdxForTargetType;
	}
	
	////////////////////////////////////////////////////////////////////////
	// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 
	if(this->m_HPAction.CheckValidSizeTalkCreate()) {
		HPACTION_TALK_HPRATE MsgTalk;
		util::zero(&MsgTalk, sizeof(HPACTION_TALK_HPRATE));
		if(this->m_HPAction.GetTalkCreate(&MsgTalk)) {
			// 생성시 대화가 있다면 전송한다.
			this->m_CurrentNPCMapChannel->SendFSvrHPTalk(this, &MsgTalk);
		}
	}

	InitializeMonster();

	if(i_byObjectMonsterType != EVENT_TYPE_NO_OBJECT_MONSTER
		&& i_pUnitVec3ObjMonsterTarget)
	{// 오브젝트 몬스터의 타겟 벡터 설정, InitializeMonster()이후 에 설정 해야함

		SetMoveTargetVector(i_pUnitVec3ObjMonsterTarget);
		SetTargetVector(i_pUnitVec3ObjMonsterTarget, TRUE);	// Object의 타겟을 무조건 설정한다.
	}

	SendMoveRange				= i_nMapMonsterVisibleDiameter + MonsterInfoPtr->Size*2;
	if(COMPARE_MPOPTION_BIT(MonsterInfoPtr->MPOption, MPOPTION_BIT_BOSS_MONSTER)
		|| EVENT_TYPE_NO_OBJECT_MONSTER != m_byObjectMonsterType)
	{
		SendMoveRange			+= 1000;
	}
}
////////////////////////////////////////////////////////////////////////////////
//
// 함 수 이 름  : CMonster::InitializeMonster
// 반환되는 형  : void
// 함 수 인 자  : void
// 함 수 설 명  : inline 함수
//					몬스터 생성시에 TargetVector와 이동시 필요한 변수들을 몬스터의 형태와
//					MovePattern에 따라 설정한다.
void CNPCMonster::InitializeMonster(void)
{
	////////////////////////////////////////////////////////////////////////////////
	// Monster Speed 설정
	this->SetCurrentSpeed(this->GetSpeed());

	////////////////////////////////////////////////////////////////////////////////
	// Monster MoveTargetVector, TargetVector 설정 
	D3DXVECTOR3	tmVec3;
	GetRandomPlaneUnitVector(&tmVec3);
	this->SetMoveTargetVector(&tmVec3);
	this->SetTargetVector(&tmVec3, TRUE);

	////////////////////////////////////////////////////////////////////////////////
	// Current Monster Form 설정

	switch(MonsterInfoPtr->MonsterForm)
	{
	case FORM_FLYING_RIGHT:
	case FORM_FLYING_COPTER:
	case FORM_GROUND_MOVE:
	case FORM_SWIMMINGFLYING_RIGHT:
	case FORM_SWIMMINGFLYING_COPTER:
		{
			this->SetCurrentMonsterForm(MonsterInfoPtr->MonsterForm);
		}
		break;
	case FORM_FLYINGandGROUND_RIGHT:
		m_nTimeGapChangeMonsterForm = 10000 + RANDI(0, 6); // wtf is this now
		this->SetCurrentMonsterForm(FORM_FLYING_RIGHT);
		break;
	case FORM_FLYINGandGROUND_COPTER:
		m_nTimeGapChangeMonsterForm = 10000 + RANDI(0, 6); // ??
		this->SetCurrentMonsterForm(FORM_FLYING_COPTER);
		break;
	}

	////////////////////////////////////////////////////////////////////////////////
	// Current Monster MovePattern 설정
	this->SetCurrentMovePattern(MonsterInfoPtr->MovePattern);
	
	////////////////////////////////////////////////////////////////////////////////
	// Form과 MovePattern에 맞게 이동 관련 변수 설정
	SetMonsterMoveInfo();
	
	////////////////////////////////////////////////////////////////////////////////
	// Monster Item을 하나 선택한다.
	SelectUsingMonsterItem();

	///////////////////////////////////////////////////////////////////////////////
	// 2010-03-16 by cmkwon, 인피2차 MtoM, MtoC 타겟 변경 관련 수정 - 
	this->SetTimeLastCheckChangeTarget(0);

	m_SkillManager.InitSkill();			// 2010-03-31 by dhjin, 인피니티(기지방어) -
}

void CNPCMonster::SetMonsterMoveInfo(void)
{
	m_MoveInfo.MovableFlag			= TRUE;
	m_MoveInfo.FBFlag				= TRUE;
	m_MoveInfo.FBDirect				= MSD_FRONT;
	m_MoveInfo.LRDirect				= MSD_LEFT;	
	if(RAND100() < 50)
	{
		m_MoveInfo.LRDirect			= MSD_RIGHT;
	}
	
	m_MoveInfo.UDDirect				= MSD_UP;
	if(RAND100() < 50)
	{
		m_MoveInfo.UDDirect			= MSD_DOWN;
	}
	m_MoveInfo.SpeedDirect			= MSD_SPEED_UP;

	m_MoveInfo.FBCurrentCount		= 0;
	m_MoveInfo.LRCurrentCount		= 0;
	m_MoveInfo.LRSubCurrentCount	= 0;
	m_MoveInfo.UDCurrentCount		= 0;
	m_MoveInfo.UDSubCurrentCount	= 0;

	switch(CurrentMovePattern)
	{
	case MOVPAT_STOP:
		{
			m_MoveInfo.MovableFlag					= FALSE;
		}
		break;
	case MOVPAT_FOLLOWPATH:		// 2011-05-23 by hskim, 인피니티 3차 - 웨이포인트 구현
	case MOVPAT_STRAIGHT:
		{
			m_MoveInfo.FBChangeBaseCount			= MON_MOVE_FBCOUNT_0;
			m_MoveInfo.FBChangeRandomCount			= MON_MOVE_RANDOMCOUNT_1;
			
			m_MoveInfo.LRChangeBaseCount			= MON_MOVE_LRCOUNT_1;
			m_MoveInfo.LRChangeRandomCount			= MON_MOVE_RANDOMCOUNT_1;
			m_MoveInfo.LRMoveDirection				= MSD_LEFT_5;
			m_MoveInfo.LRMoveDirectionRandomCount	= 10;
			m_MoveInfo.LRSubChangeBaseCount			= m_MoveInfo.LRChangeBaseCount*2;
			m_MoveInfo.LRSubChangeRandomCount		= 0;
			m_MoveInfo.LRSubMoveDirection			= 0;
			
			m_MoveInfo.UDChangeBaseCount			= MON_MOVE_UDCOUNT_1;
			m_MoveInfo.UDChangeRandomCount			= MON_MOVE_RANDOMCOUNT_1;
			m_MoveInfo.UDMoveDirection				= MSD_UP_1;
			m_MoveInfo.UDMoveDirectionRandomCount	= 2;
			m_MoveInfo.UDSubChangeBaseCount			= m_MoveInfo.UDChangeBaseCount*2;			// UDChangeBaseCount보다 크게
			m_MoveInfo.UDSubChangeRandomCount		= 0;
			m_MoveInfo.UDSubMoveDirection			= 0;
		}
		break;
	case MOVPAT_RIGHTANDLEFT:
		{
			m_MoveInfo.FBChangeBaseCount			= MON_MOVE_FBCOUNT_0;
			m_MoveInfo.FBChangeRandomCount			= MON_MOVE_RANDOMCOUNT_1;
			
			m_MoveInfo.LRChangeBaseCount			= MON_MOVE_LRCOUNT_1;			
			m_MoveInfo.LRChangeRandomCount			= MON_MOVE_RANDOMCOUNT_1;
			m_MoveInfo.LRMoveDirection				= MSD_LEFT_10;
			m_MoveInfo.LRMoveDirectionRandomCount	= 35;
			m_MoveInfo.LRSubChangeBaseCount			= m_MoveInfo.LRChangeBaseCount*2;
			m_MoveInfo.LRSubChangeRandomCount		= 0;
			m_MoveInfo.LRSubMoveDirection			= 0;
			
			m_MoveInfo.UDChangeBaseCount			= MON_MOVE_UDCOUNT_1;
			m_MoveInfo.UDChangeRandomCount			= MON_MOVE_RANDOMCOUNT_1;
			m_MoveInfo.UDMoveDirection				= MSD_UP_1;
			m_MoveInfo.UDMoveDirectionRandomCount	= 2;
			m_MoveInfo.UDSubChangeBaseCount			= m_MoveInfo.UDChangeBaseCount*2;
			m_MoveInfo.UDSubChangeRandomCount		= 0;
			m_MoveInfo.UDSubMoveDirection			= 0;
		}
		break;
	case MOVPAT_8RIGHTANDLEFT:
		{
			m_MoveInfo.FBChangeBaseCount			= MON_MOVE_FBCOUNT_0;
			m_MoveInfo.FBChangeRandomCount			= MON_MOVE_RANDOMCOUNT_1;
			
			m_MoveInfo.LRChangeBaseCount			= MON_MOVE_LRCOUNT_1;
			m_MoveInfo.LRChangeRandomCount			= MON_MOVE_RANDOMCOUNT_1;
			m_MoveInfo.LRMoveDirection				= MSD_LEFT_1;
			m_MoveInfo.LRMoveDirectionRandomCount	= 5;
			m_MoveInfo.LRSubChangeBaseCount			= m_MoveInfo.LRChangeBaseCount/10;
			m_MoveInfo.LRSubChangeRandomCount		= MON_MOVE_RANDOMCOUNT_0;
			m_MoveInfo.LRSubMoveDirection			= MSD_LEFT_2;
			
			m_MoveInfo.UDChangeBaseCount			= MON_MOVE_UDCOUNT_1;
			m_MoveInfo.UDChangeRandomCount			= MON_MOVE_RANDOMCOUNT_1;
			m_MoveInfo.UDMoveDirection				= MSD_UP_1;
			m_MoveInfo.UDMoveDirectionRandomCount	= 2;
			m_MoveInfo.UDSubChangeBaseCount			= m_MoveInfo.UDChangeBaseCount*2;
			m_MoveInfo.UDSubChangeRandomCount		= 0;
			m_MoveInfo.UDSubMoveDirection			= 0;
		}
		break;
	case MOVPAT_UPANDDOWN:
		{
			m_MoveInfo.FBChangeBaseCount			= MON_MOVE_FBCOUNT_1;
			m_MoveInfo.FBChangeRandomCount			= MON_MOVE_RANDOMCOUNT_1;
			
			m_MoveInfo.LRChangeBaseCount			= MON_MOVE_LRCOUNT_1;
			m_MoveInfo.LRChangeRandomCount			= MON_MOVE_RANDOMCOUNT_1;
			m_MoveInfo.LRMoveDirection				= MSD_LEFT_5;
			m_MoveInfo.LRMoveDirectionRandomCount	= 10;
			m_MoveInfo.LRSubChangeBaseCount			= m_MoveInfo.LRChangeBaseCount*2;
			m_MoveInfo.LRSubChangeRandomCount		= 0;
			m_MoveInfo.LRSubMoveDirection			= 0;
			
			m_MoveInfo.UDChangeBaseCount			= MON_MOVE_UDCOUNT_0;
			m_MoveInfo.UDChangeRandomCount			= MON_MOVE_RANDOMCOUNT_2;
			m_MoveInfo.UDMoveDirection				= MSD_UP_5;
			m_MoveInfo.UDMoveDirectionRandomCount	= 5;
			m_MoveInfo.UDSubChangeBaseCount			= m_MoveInfo.UDChangeBaseCount/10;
			m_MoveInfo.UDSubChangeRandomCount		= MON_MOVE_RANDOMCOUNT_0;
			m_MoveInfo.UDSubMoveDirection			= MSD_UP_2;
		}
		break;
	case MOVPAT_OBLIQUE:
		{
			m_MoveInfo.FBChangeBaseCount			= MON_MOVE_FBCOUNT_0;
			m_MoveInfo.FBChangeRandomCount			= MON_MOVE_RANDOMCOUNT_1;
			
			m_MoveInfo.LRChangeBaseCount			= MON_MOVE_LRCOUNT_1;			
			m_MoveInfo.LRChangeRandomCount			= MON_MOVE_RANDOMCOUNT_1;
			m_MoveInfo.LRMoveDirection				= MSD_LEFT_10;
			m_MoveInfo.LRMoveDirectionRandomCount	= 35;
			m_MoveInfo.LRSubChangeBaseCount			= m_MoveInfo.LRChangeBaseCount*2;
			m_MoveInfo.LRSubChangeRandomCount		= 0;
			m_MoveInfo.LRSubMoveDirection			= 0;
			
			m_MoveInfo.UDChangeBaseCount			= m_MoveInfo.LRChangeBaseCount;
			m_MoveInfo.UDChangeRandomCount			= m_MoveInfo.LRChangeRandomCount;
			m_MoveInfo.UDMoveDirection				= MSD_UP_5;
			m_MoveInfo.UDMoveDirectionRandomCount	= 20;
			m_MoveInfo.UDSubChangeBaseCount			= m_MoveInfo.UDChangeBaseCount*2;
			m_MoveInfo.UDSubChangeRandomCount		= 0;
			m_MoveInfo.UDSubMoveDirection			= 0;
		}
		break;
	case MOVPAT_PATROL:
	case MOVPAT_OBLIQUEANDCURVE:
	case MOVPAT_TELEPORT:			// 순간이동 몬스터를 사선이동 타입으로 설정
		{
			m_MoveInfo.FBChangeBaseCount			= MON_MOVE_FBCOUNT_0;
			m_MoveInfo.FBChangeRandomCount			= MON_MOVE_RANDOMCOUNT_1;
			
			m_MoveInfo.LRChangeBaseCount			= MON_MOVE_LRCOUNT_1;
			m_MoveInfo.LRChangeRandomCount			= MON_MOVE_RANDOMCOUNT_2;
			m_MoveInfo.LRMoveDirection				= MSD_LEFT_1;
			m_MoveInfo.LRMoveDirectionRandomCount	= 5;
			m_MoveInfo.LRSubChangeBaseCount			= m_MoveInfo.LRChangeBaseCount/10;
			m_MoveInfo.LRSubChangeRandomCount		= MON_MOVE_RANDOMCOUNT_0;
			m_MoveInfo.LRSubMoveDirection			= MSD_LEFT_2;
			
			m_MoveInfo.UDChangeBaseCount			= m_MoveInfo.LRChangeBaseCount;
			m_MoveInfo.UDChangeRandomCount			= m_MoveInfo.LRChangeRandomCount;
			m_MoveInfo.UDMoveDirection				= MSD_UP_5;
			m_MoveInfo.UDMoveDirectionRandomCount	= 5;
			m_MoveInfo.UDSubChangeBaseCount			= m_MoveInfo.UDChangeBaseCount/10;
			m_MoveInfo.UDSubChangeRandomCount		= MON_MOVE_RANDOMCOUNT_0;
			m_MoveInfo.UDSubMoveDirection			= MSD_UP_2;
		}
		break;
	default:
		{
			m_MoveInfo.MovableFlag					= FALSE;
		}
	}
	switch(CurrentMonsterForm)
	{
	case FORM_FLYING_RIGHT:
	case FORM_SWIMMINGFLYING_RIGHT:
		{
			m_MoveInfo.FBFlag								= FALSE;
			switch(CurrentMovePattern)
			{
			case MOVPAT_STRAIGHT:
			case MOVPAT_RIGHTANDLEFT:			
				{										
					m_MoveInfo.LRMoveDirectionRandomCount	= m_MoveInfo.LRMoveDirectionRandomCount/2;
				}
				break;
			case MOVPAT_8RIGHTANDLEFT:
				{
					m_MoveInfo.LRSubChangeBaseCount			= m_MoveInfo.LRChangeBaseCount/5;
					m_MoveInfo.LRSubMoveDirection			= m_MoveInfo.LRSubMoveDirection*2;
				}
				break;
			case MOVPAT_UPANDDOWN:
				{
					m_MoveInfo.UDSubChangeBaseCount			= m_MoveInfo.UDChangeBaseCount/5;
					m_MoveInfo.UDSubMoveDirection			= m_MoveInfo.UDSubMoveDirection*2;
				}
				break;
			case MOVPAT_OBLIQUE:
			case MOVPAT_TELEPORT:
				{
					m_MoveInfo.LRMoveDirectionRandomCount	= m_MoveInfo.LRMoveDirectionRandomCount/2;
					m_MoveInfo.UDMoveDirectionRandomCount	= m_MoveInfo.UDMoveDirectionRandomCount/2;					
				}
				break;
			}
		}
		break;
	case FORM_FLYING_COPTER:
	case FORM_SWIMMINGFLYING_COPTER:	
		{
			m_MoveInfo.FBFlag								= TRUE;
		}
		break;
	case FORM_GROUND_MOVE:
		{
			m_MoveInfo.FBFlag								= TRUE;
			switch(CurrentMovePattern)
			{
			case MOVPAT_STRAIGHT:
			case MOVPAT_RIGHTANDLEFT:
			case MOVPAT_8RIGHTANDLEFT:
			case MOVPAT_UPANDDOWN:
			case MOVPAT_OBLIQUE:
			case MOVPAT_PATROL:
			case MOVPAT_OBLIQUEANDCURVE:
			case MOVPAT_TELEPORT:			// 순간이동 몬스터를 사선이동 타입으로 설정
				{
					m_MoveInfo.UDChangeBaseCount			= MON_MOVE_UDCOUNT_2;
					m_MoveInfo.UDChangeRandomCount			= MON_MOVE_RANDOMCOUNT_1;
					m_MoveInfo.UDMoveDirection				= MSD_UP_1;
					m_MoveInfo.UDMoveDirectionRandomCount	= 0;
					m_MoveInfo.UDSubChangeBaseCount			= m_MoveInfo.UDChangeBaseCount*2;			// UDChangeBaseCount보다 크게
					m_MoveInfo.UDSubChangeRandomCount		= 0;
					m_MoveInfo.UDSubMoveDirection			= m_MoveInfo.UDMoveDirection;
				}
				break;
			}
		}// FORM_GROUND_MOVE_end
		break;
	case FORM_OBJECT_STOP:
		{
			m_MoveInfo.MovableFlag				= FALSE;
			m_MoveInfo.FBFlag					= FALSE;
		}
		break;
	case FORM_OBJECT_PLANE_ROTATE:
		{
			m_MoveInfo.MovableFlag				= FALSE;
			m_MoveInfo.FBFlag					= FALSE;
		}
		break;
	case FORM_OBJECT_CANNON:
		{
			m_MoveInfo.MovableFlag				= FALSE;
			m_MoveInfo.FBFlag					= FALSE;
		}
		break;
	}
	m_MoveInfo.FBChangeCount				= m_MoveInfo.FBChangeBaseCount;
	m_MoveInfo.LRChangeCount				= m_MoveInfo.LRChangeBaseCount;
	m_MoveInfo.LRSubChangeCount				= m_MoveInfo.LRSubChangeBaseCount;
	m_MoveInfo.UDChangeCount				= m_MoveInfo.UDChangeBaseCount;
	m_MoveInfo.UDSubChangeCount				= m_MoveInfo.UDSubChangeBaseCount;	
}

float CNPCMonster::GetSpeed(void)
{
	return MonsterInfoPtr->Speed/(1000/MONSTER_UPDATE_MOVE_TERM_TICK);
}

float CNPCMonster::GetQuickSpeed(void)
{
	return max(1.0f, MonsterInfoPtr->QuickSpeed);
}

void CNPCMonster::UpdateCurrentSpeed(void)
{
	if ( m_pUsingMonsterItem && NULL == m_pUsingMonsterItem->pItemInfo )
	{
		this->SelectUsingMonsterItem();		// 2012-12-17 by hskim, NPC 서버 죽는 문제 보안 (몬스터 무기의 pItemInfo 가 NULL 되는 문제)
	}

	if(MSD_SPEED_UP == m_MoveInfo.SpeedDirect)
	{
		float fMaxSpeed = 0.0f;

		if(m_pUsingMonsterItem && m_pUsingMonsterItem->pItemInfo
			&& ORBIT_BODYSLAM == m_pUsingMonsterItem->pItemInfo->OrbitType
			&& this->m_dwCurrentTick - this->m_dwTimeMonsterLastAttack > 3 * m_pUsingMonsterItem->pItemInfo->ReAttacktime )
		{
			fMaxSpeed = this->GetQuickSpeed();
		}
		else if((FORM_GROUND_MOVE == CurrentMonsterForm || (MSS_NORMAL == m_enMoveState && MSD_FRONT == this->m_MoveInfo.FBDirect))
			|| (MSS_MAP_IMPACT == m_enMoveState && m_nTargetIndex <= 0))
		{
			fMaxSpeed = this->GetSpeed();
		}
		else if(MSS_NORMAL == m_enMoveState
			&& MSD_FRONT == this->m_MoveInfo.FBDirect)
		{
			fMaxSpeed = (this->GetSpeed() + this->GetQuickSpeed())/2;
		}
		else
		{
			fMaxSpeed = this->GetQuickSpeed();
		}
		
		if(fMaxSpeed < m_CurrentSpeed)
		{
			this->SetCurrentSpeed(fMaxSpeed);
		}
		else if(fMaxSpeed > m_CurrentSpeed)
		{
			this->SetCurrentSpeed(m_CurrentSpeed + (this->GetQuickSpeed() - this->GetSpeed())/5);
			if(fMaxSpeed < m_CurrentSpeed)
			{
				this->SetCurrentSpeed(fMaxSpeed);
			}
		}
	}
	else
	{
		float fMinSpeed = 0.0f;
		if(FALSE == m_MoveInfo.FBFlag)
		{
			fMinSpeed = this->GetSpeed()/2;
		}
		else
		{
			fMinSpeed = this->GetSpeed()/3;
		}

		if(fMinSpeed < m_CurrentSpeed)
		{
			this->SetCurrentSpeed(m_CurrentSpeed - 1.0f);
			if(fMinSpeed > m_CurrentSpeed)
			{
				this->SetCurrentSpeed(fMinSpeed);
			}
		}
	}
}


void CNPCMonster::ResetLastAttackTime(DWORD i_dwCurTick)
{
	m_dwLastAttackTime = i_dwCurTick;
}

void CNPCMonster::ResetAllLastReAttackTime(DWORD i_dwCurTick)
{
	util::zero(m_ArrLastReattackTime, ARRAY_SIZE_MONSTER_ITEM * sizeof(m_ArrLastReattackTime[0]));
}

void CNPCMonster::ResetCurrentShotNumCount(void)
{	
	m_nCurrentShotNumCount = 0;
}


void CNPCMonster::UpdatePositionVector(float i_fTimeRate)
{
	if(MOVPAT_STOP == CurrentMovePattern){			return;}

	
	///////////////////////////////////////////////////////////////////////////////
	// 앞뒤 이동을 방향을 처리한다.
	//	1. FBFlag가 TRUE이고 공격 중 일때만 앞뒤의 방향 전환이 처리된다.
	//	2. 그 이외는 항상 앞으로만 이동	
	if(m_MoveInfo.FBFlag
		&& 0 != m_nTargetIndex
		&& FALSE == m_FlagNPCMonster.EnforceTargetVector
		&& MOVPAT_FOLLOWPATH != CurrentMovePattern )		// 2011-05-23 by hskim, 인피니티 3차 - 웨이포인트 구현
	{
		if(m_MoveInfo.FBCurrentCount > m_MoveInfo.FBChangeCount)
		{
			m_MoveInfo.FBDirect			*= -1;
			m_MoveInfo.FBCurrentCount	= 0;
			m_MoveInfo.FBChangeCount	= m_MoveInfo.FBChangeBaseCount + RANDI(0, m_MoveInfo.FBChangeRandomCount - 1);
		}
	}
	else
	{
		m_MoveInfo.FBDirect				= MSD_FRONT;
		m_MoveInfo.FBCurrentCount		= 0;
	}
	
	if(MSS_MAP_IMPACT != m_enMoveState)
	{
		///////////////////////////////////////////////////////////////////////////////
		// 1. 좌우 이동의 방향을 처리
		// 2. Sub로 이동의 방향을 처리(곡선이동을 위해), 좌우 방향이 바뀌지는 않는다
		if(m_MoveInfo.LRCurrentCount > m_MoveInfo.LRChangeCount)
		{
			m_MoveInfo.LRDirect				*= -1;
			m_MoveInfo.LRCurrentCount		= 0;
			m_MoveInfo.LRSubCurrentCount	= 0;
			m_MoveInfo.LRChangeCount = m_MoveInfo.LRChangeBaseCount + RANDI(0, m_MoveInfo.LRChangeRandomCount - 1);
			GNPCRotateTargetVectorHorizontal(&m_MoveTargetVector, &m_MoveTargetVector
				, m_MoveInfo.LRDirect * m_MoveInfo.LRMoveDirection
				, m_MoveInfo.LRMoveDirectionRandomCount);
		}
		else if(m_MoveInfo.LRChangeBaseCount > m_MoveInfo.LRSubChangeBaseCount
			&& m_MoveInfo.LRSubCurrentCount > m_MoveInfo.LRSubChangeCount)
		{
			m_MoveInfo.LRSubCurrentCount	= 0;
			m_MoveInfo.LRSubChangeCount		= m_MoveInfo.LRSubChangeBaseCount + RANDI(0, m_MoveInfo.LRSubChangeRandomCount - 1);
			GNPCRotateTargetVectorHorizontal(&m_MoveTargetVector, &m_MoveTargetVector
				, m_MoveInfo.LRDirect * m_MoveInfo.LRSubMoveDirection
				, 2);
		}
		
		///////////////////////////////////////////////////////////////////////////////
		// 몬스터 위아래 방향 처리
		if(m_MoveInfo.UDCurrentCount > m_MoveInfo.UDChangeCount)
		{				
			m_MoveInfo.UDDirect				*= -1;
			m_MoveInfo.UDCurrentCount		= 0;
			m_MoveInfo.UDSubCurrentCount	= 0;
			m_MoveInfo.UDChangeCount		= m_MoveInfo.UDChangeBaseCount + RANDI(0, m_MoveInfo.UDChangeRandomCount - 1);
			
			GNPCRotateTargetVectorVertical(&m_MoveTargetVector, &m_MoveTargetVector
				, m_MoveInfo.UDDirect * m_MoveInfo.UDMoveDirection
				, m_MoveInfo.UDMoveDirectionRandomCount);
		}			
		else if (m_MoveInfo.UDChangeBaseCount > m_MoveInfo.UDSubChangeBaseCount
			&& m_MoveInfo.UDSubCurrentCount > m_MoveInfo.UDSubChangeCount)
		{
			m_MoveInfo.UDSubCurrentCount	= 0;
			m_MoveInfo.UDSubChangeCount		= m_MoveInfo.UDSubChangeBaseCount + RANDI(0, m_MoveInfo.UDSubChangeRandomCount - 1);
			
			GNPCRotateTargetVectorVertical(&m_MoveTargetVector, &m_MoveTargetVector
				, m_MoveInfo.UDDirect * m_MoveInfo.UDSubMoveDirection
				, 2);
		}
	}
		
	///////////////////////////////////////////////////////////////////////////////
	// 현재 PositionVector를 Update한다

	// 2011-05-23 by hskim, 인피니티 3차 - 웨이포인트 구현
	SWAY_POINT *pWayPoint = GetWayPointManager()->GetWayPointInfo(MonsterInfoPtr->WayPointPattern, m_nWayPointNowStep);

	if( MOVPAT_FOLLOWPATH == CurrentMovePattern )
	{
		/*
		{
			char	szTemp[1024];
			util::zero(szTemp, 1024);
			SWAY_POINT *pWayPoint = GetWayPointManager()->GetWayPointInfo(MonsterInfoPtr->WayPointPattern, m_nWayPointNowStep);
		sprintf(szTemp, "웨이포인트 이동 패턴 번호 %3d 현재 스텝 %2d 현재 좌표[X %4.1f Y %4.1f Z %4.1f] 목표 좌표[X %4.1f Y %4.1f Z %4.1f]\r\n", 
			MonsterInfoPtr->WayPointPattern, m_nWayPointNowStep, 
			PositionVector.x, PositionVector.y, PositionVector.z,
			pWayPoint->vPoint.x, pWayPoint->vPoint.y, pWayPoint->vPoint.z);
			g_pNPCGlobal->WriteSystemLog(szTemp);
		}
		*/

		if( TRUE == GetWayPointManager()->IsCollision(MonsterInfoPtr->WayPointPattern, m_nWayPointNowStep, PositionVector) )
		{
			BOOL bLastStep = FALSE;

			/*
			char	szTemp[1024];
  			util::zero(szTemp, 1024);
			SWAY_POINT *pWayPoint = GetWayPointManager()->GetWayPointInfo(MonsterInfoPtr->WayPointPattern, m_nWayPointNowStep);

			sprintf(szTemp, "============> 충돌 => 웨이포인트 이동 패턴 번호 %3d 현재 스텝 %2d 현재 좌표[X %4.1f Y %4.1f Z %4.1f] 목표 좌표[X %4.1f Y %4.1f Z %4.1f]\r\n", 
				MonsterInfoPtr->WayPointPattern, m_nWayPointNowStep, 
				PositionVector.x, PositionVector.y, PositionVector.z,
				pWayPoint->vPoint.x, pWayPoint->vPoint.y, pWayPoint->vPoint.z);
			g_pNPCGlobal->WriteSystemLog(szTemp);
			*/

			if( m_nWayPointDirection > 0 )
			{
				bLastStep = GetWayPointManager()->IsLastStep(MonsterInfoPtr->WayPointPattern, m_nWayPointNowStep);
			}
			else
			{
				bLastStep = GetWayPointManager()->IsFirstStep(m_nWayPointNowStep);
			}

			if( TRUE == bLastStep )
			{
				switch( GetWayPointManager()->GetCompletionAction(MonsterInfoPtr->WayPointPattern) )
				{
				case WAY_POINT_COMPLETION_ACTION_REPEAT:

					m_nWayPointNowStep = 0;

					break;

				case WAY_POINT_COMPLETION_ACTION_RECIPROCATION:

					m_nWayPointDirection *= -1;

					break;

				case WAY_POINT_COMPLETION_ACTION_STOP:
				default:

					return;	// <- return !! 함수 종료 (이동 안함)
				}
			}
			else
			{
				m_nWayPointNowStep += m_nWayPointDirection;
			}
		}

		D3DXVECTOR3 tmUnitVec3M2C, tmTargetPosVec(pWayPoint->vPoint);
		D3DXVec3Normalize(&tmUnitVec3M2C, &(tmTargetPosVec - PositionVector));
		
		tmUnitVec3M2C = tmUnitVec3M2C;

		m_MoveTargetVector = tmUnitVec3M2C;
	}
	// end 2011-05-23 by hskim, 인피니티 3차 - 웨이포인트 구현

	// start 2011-05-23 by hskim, 인피니티 3차 - 웨이포인트 구현
	if( MOVPAT_FOLLOWPATH != CurrentMovePattern )
	{
	PositionVector += m_MoveInfo.FBDirect * m_MoveTargetVector * this->GetCurrentSpeed() * i_fTimeRate;
	}
	else
	{	
		D3DXVECTOR3 vecDelta = m_MoveTargetVector * this->GetCurrentSpeed() * i_fTimeRate;

		/*
		D3DXVECTOR3 vecBetween = pWayPoint->vPoint - PositionVector;

		if( fabs(vecDelta.x) > fabs(vecBetween.x) ) vecDelta.x = vecBetween.x;
		if( fabs(vecDelta.y) > fabs(vecBetween.y) ) vecDelta.x = vecBetween.y;
		if( fabs(vecDelta.z) > fabs(vecBetween.z) ) vecDelta.x = vecBetween.z;
		*/

		PositionVector += vecDelta;
	}
	// end 2011-05-23 by hskim, 인피니티 3차 - 웨이포인트 구현
//	char	szTemp[1024];
//	util::zero(szTemp, 1024);
//	sprintf(szTemp, "Cur(%s), Next(%s), MovTar(%-3d,%-3d,%-3d) MState[%d] TarIndex(%3d), "
//		, GET_STRING_VECTOR3(&this->m_BeforePosition), GET_STRING_VECTOR3(&this->PositionVector)
//		, (int)(m_MoveTargetVector.x * 100), (int)(m_MoveTargetVector.y * 100), (int)(m_MoveTargetVector.z * 100)
//		, this->m_enMoveState, this->m_nTargetIndex);
//
//	sprintf(&szTemp[strlen(szTemp)], "FBFlag[%d] FBD[%2d] FBChange[%2d] FBRan[%2d] FBCur[%2d], "
//		, this->m_MoveInfo.FBFlag, this->m_MoveInfo.FBDirect, this->m_MoveInfo.FBChangeCount
//		, this->m_MoveInfo.FBChangeRandomCount, this->m_MoveInfo.FBCurrentCount);
//
//	sprintf(&szTemp[strlen(szTemp)], "LRD[%2d] LRChange[%2d] LRPer[%2d] LRCur[%2d] LRAngle[%2d], "
//		, this->m_MoveInfo.LRDirect, this->m_MoveInfo.LRChangeCount, this->m_MoveInfo.LRChangeRandomCount
//		, this->m_MoveInfo.LRCurrentCount, this->m_MoveInfo.LRMoveDirection);
//	sprintf(&szTemp[strlen(szTemp)], "LRSubChange[%2d] LRSubPer[%2d] LRSubCur[%2d] LRSubAngle[%2d], "
//		, this->m_MoveInfo.LRSubChangeCount, this->m_MoveInfo.LRSubChangeRandomCount
//		, this->m_MoveInfo.LRSubCurrentCount, this->m_MoveInfo.LRSubMoveDirection);
//	
//	sprintf(&szTemp[strlen(szTemp)], "UD UDD[%2d] UDChange[%2d] UDPer[%2d] UDCur[%2d] UDAngle[%2d], "
//		, this->m_MoveInfo.UDDirect, this->m_MoveInfo.UDChangeCount, this->m_MoveInfo.UDChangeRandomCount
//		, this->m_MoveInfo.UDCurrentCount, this->m_MoveInfo.UDMoveDirection);
//	sprintf(&szTemp[strlen(szTemp)], "UDSub UDSubChange[%2d] UDSubPer[%2d] UDSubCur[%2d] UDSubAngle[%2d], "
//		, this->m_MoveInfo.UDSubChangeCount, this->m_MoveInfo.UDSubChangeRandomCount
//		, this->m_MoveInfo.UDSubCurrentCount, this->m_MoveInfo.UDSubMoveDirection);
//	
//	sprintf(&szTemp[strlen(szTemp)], "\r\n");
//	g_pNPCGlobal->WriteSystemLog(szTemp);
//	DBGOUT(szTemp);
}

void CNPCMonster::UpdateEnforceTargetVector(void)
{
	if (MSS_NORMAL == m_enMoveState){				return;}
	if(FALSE == m_FlagNPCMonster.EnforceTargetVector)
	{
		if(this->m_dwCurrentTick - m_dwTimeSetEnforceTargetVector > m_usTimeEnforceTargetVectorTerm)
		{
//			DBGOUT("			해제 Time Term(%d)\n", m_dwCurrentTick - m_dwTimeSetEnforceTargetVector);			
			this->SetMoveState(MSS_NORMAL);
		}
		return;
	}
	
	float ftmAngleGap = ACOS(D3DXVec3Dot(&m_MoveTargetVector, &m_EnforceTargetVector));
//	DBGOUT("		각 차이 (%d) Order(%2d, %2d, %2d) Target(%2d, %2d, %2d)\n"
//		, (int)(ftmAngleGap*100), (int)(m_EnforceTargetVector.x * 100), (int)(m_EnforceTargetVector.y * 100), (int)(m_EnforceTargetVector.z * 100)
//		, (int)(m_MoveTargetVector.x * 100), (int)(m_MoveTargetVector.y * 100), (int)(m_MoveTargetVector.z * 100));
	
	if(ftmAngleGap < MONSTER_ANGLE_PI_5)
	{
		this->SetNPCMonsterFlagEnforceTargetVector(FALSE);
	}
	else
	{
		///////////////////////////////////////////////////////////////////////////
		// 1. 직진형 몬스터, 콥터형이지만 각 차이가 90도ㅗ보다 작을때
		//		==> 평면하고 상관없이 회전이 처리된다
		// 2. 콥터형 몬스터이면서 각 차이가 90도보다 클때
		//		==> 평면하고의 각 차이가 크면 평면으로 각을 처리하고 회전한다.
		if(FALSE == m_MoveInfo.FBFlag
			|| ftmAngleGap < PI/2)
		{
			GNPCGetMaxTargetVector(&m_MoveTargetVector, &m_EnforceTargetVector, this->MonsterInfoPtr->TurnAngle);
		}
		else
		{
			D3DXVECTOR3	tmVec3Plane1, tmVec3Plane2;
			CNPCMapProject::ChangePlaneUnitVec3(&tmVec3Plane1, &m_MoveTargetVector, &m_MoveTargetVector);
			CNPCMapProject::ChangePlaneUnitVec3(&tmVec3Plane2, &m_EnforceTargetVector, &m_EnforceTargetVector);		
			float fPinPoint = ACOS(D3DXVec3Dot(&tmVec3Plane1, &tmVec3Plane2));
			if(fPinPoint < MONSTER_COPTER_MAXTARGET_PINPOINT)
			{
				GNPCGetMaxTargetVector(&m_MoveTargetVector, &m_EnforceTargetVector, this->MonsterInfoPtr->TurnAngle);
			}
			else
			{
				GNPCGetMaxTargetVector(&tmVec3Plane1, &tmVec3Plane2, this->MonsterInfoPtr->TurnAngle);
				m_MoveTargetVector = tmVec3Plane1;
			}
		}		
	}
}

BOOL CNPCMonster::CheckMoveRange(void)
{
	if(MSS_NORMAL != m_enMoveState)
	{
		return FALSE;
	}
	else if(FALSE == m_MoveInfo.MovableFlag)
	{
		return TRUE;
	}

	D3DXVECTOR3	TempVector3 = m_CreatedPosition - PositionVector;
	TempVector3.y = 0.0f;
	
	float fM2CDistance = D3DXVec3Length(&TempVector3) - MONSTER_BASIC_ARROUND_MOVABLE_RANGE;
//	DBGOUT("		거리 (%3d)\n", (int)fM2CDistance);

// 2004-04-16 cmkwon, 일단 무조건 생성 위치로 가도록 놔둔다
//	///////////////////////////////////////////////////////////////////////////////
//	// 이동 영역을 너무 많이 벗어 났을 경우 현재 위치를 생성 위치로 설정
//	if(fM2CDistance > 100.0f)
//	{
//		
//		///////////////////////////////////////////////////////////////////////////////
//		// 1. 이동 타임이 패트롤이면 원래 생성 좌표 근처로 이동시킨다
//		// 2. 나머지는 현재의 좌표를 생성 좌표로 설정한다.
//		if(CurrentMovePattern == MOVPAT_PATROL)
//		{
//			PositionVector = m_CreatedPosition + TargetVector*RAND100();
//		}
//		else
//		{
//			m_CreatedPosition = PositionVector;
//		}
//		return;
//	}
	

	///////////////////////////////////////////////////////////////////////////////
	// 이동 영역을 조금 벗어났거나 높이차이가 클때
	if(fM2CDistance > 0.0f
		|| abs(m_CreatedPosition.y - PositionVector.y) > 500.0f)
	{
		return TRUE;
	}
	return FALSE;
}


void CNPCMonster::UpdateMoveInfoAllCurrentCount(void)
{
	m_MoveInfo.FBCurrentCount++;
	m_MoveInfo.LRCurrentCount++;
	m_MoveInfo.LRSubCurrentCount++;
	m_MoveInfo.UDCurrentCount++;
	m_MoveInfo.UDSubCurrentCount++;
	m_MoveInfo.MoveCount++;
}


void CNPCMonster::UpdateMoveInfoAttack(D3DXVECTOR3 *i_pVec3Pos, D3DXVECTOR3 * i_pUnitVec3Tar)
{
	D3DXVECTOR3		Vec3TarM2T		= (*i_pVec3Pos) - PositionVector;
	float			fDistanceM2T	= D3DXVec3Length(&Vec3TarM2T);
	MONSTER_ITEM	*pTmMonItem		= this->m_pUsingMonsterItem;
	int				nRand100		= RAND100();
	int				nPercent		= 0;
	D3DXVECTOR3 UnitVec3TarM2T;
	D3DXVec3Normalize(&UnitVec3TarM2T, &Vec3TarM2T);

	///////////////////////////////////////////////////////////////////////////////
	// 1. 직진형 몬스터(지상형은 없다)
	//		1.1 몸통 공격 몬스터이면 타켓과의 거리를 체크하여 속도보다 가까이있으면 속도를 거리로 설정한다.
	//		1.2 몸통 공격 몬스터가 아니고 타켓과의 거리가 너무 가까우면 MSS_UNIT_IMPACT상태로 설정된다.
	if (FALSE == m_MoveInfo.FBFlag)
	{
		if(ORBIT_BODYSLAM == pTmMonItem->pItemInfo->OrbitType)
		{
			if(fDistanceM2T < this->GetCurrentSpeed())
			{
				this->SetCurrentSpeed(fDistanceM2T);
			}
		}
		else
		{
			if(this->m_dwCurrentTick - this->m_dwTimeMonsterLastAttack < 1000)
			{
				///////////////////////////////////////////////////////////////////////////////
				// 마지막 공격한 시간에서 1초가 지나지 않았으면
				// 1. 타겟과의 거리가 MONSTER_MIN_ATTACK_DISTANCE*2보다 크면 확륙이 30%로 QuickTurn 발생
				// 2. 타겟과의 거리가 MONSTER_MIN_ATTACK_DISTANCE보다 크면 확륙이 40%로 QuickTurn 발생
				// 3. 타겟과의 거리가 MONSTER_MIN_ATTACK_DISTANCE보다 작으면 QuickTurn 발생
				if(fDistanceM2T > MONSTER_MIN_ATTACK_DISTANCE*2)
				{
					nPercent = 15;
				}
				else if(fDistanceM2T > MONSTER_MIN_ATTACK_DISTANCE)
				{
					nPercent = 40;
				}
				else
				{
					nPercent = 80;
				}
			}
			else
			{
				///////////////////////////////////////////////////////////////////////////////
				// 마지막 공격한 시간에서 1초가 지났다
				// 1. 타겟과의 거리가 MONSTER_MIN_ATTACK_DISTANCE보다 작고 마지막 공격한 시간이 20초를 초과 하지 않았을때 QuickTurn 진행
				// 2. QuickTurnTerm 시간이 경과 했다면
				//		2.1 타겟이 사정 거리 안에 있으면 QuickTurn 이동 
				//		2.2 타겟이 사정 거리 안에 없으면 시간을 초기화
				if(this->m_dwCurrentTick - this->m_dwTimeSetEnforceTargetVector > this->MonsterInfoPtr->QuickTurnTerm)
				{
					if(fDistanceM2T > pTmMonItem->pItemInfo->Range)
					{						
						this->SetEnforceTargetVector(&UnitVec3TarM2T, this->GetQuickSpeed(), MSS_OUT_OF_ATTACK_RANGE);
					}
					else
					{
						nPercent = 30;
					}
				}
				else if(fDistanceM2T < MONSTER_MIN_ATTACK_DISTANCE
					&& PI*2/3 < ACOS(D3DXVec3Dot(&this->TargetVector, &UnitVec3TarM2T)))
				{
					if(MonsterInfoPtr->Level < 5)
					{
						nPercent = 15;
					}
					else
					{
						nPercent = 70;
					}					
				}
			}
			if(nRand100 < nPercent)
			{
				if(fDistanceM2T < pTmMonItem->pItemInfo->Range * 0.9)
				{
					GNPCRotateTargetVectorHorizontal(&UnitVec3TarM2T, &UnitVec3TarM2T
						, this->m_MoveInfo.LRDirect*(MONSTER_MAX_QUICK_TURN_ANGLE - this->MonsterInfoPtr->QuickTurnAngle)
						, this->MonsterInfoPtr->QuickTurnAngle);
				}
				else
				{
					GNPCRotateTargetVectorHorizontal(&UnitVec3TarM2T, &UnitVec3TarM2T
						, this->m_MoveInfo.LRDirect*(MSD_LEFT_15)
						, 30);
				}				
				this->SetEnforceTargetVector(&UnitVec3TarM2T, this->GetQuickSpeed(), MSS_QUICK_TURN_GENERAL);
			}
		}		
		return;
	}

	///////////////////////////////////////////////////////////////////////////////
	// 2. 지상형 몬스터(지상형은 콥터형만 있다)
	if (FORM_GROUND_MOVE == CurrentMonsterForm)
	{
		if(fDistanceM2T >= pTmMonItem->pItemInfo->Range/2)
		{
			if(MSD_FRONT == this->m_MoveInfo.FBDirect)
			{
				this->SetSpeedDirectMoveInfo(MSD_SPEED_UP);
			}
			else 
			{
				this->SetSpeedDirectMoveInfo(MSD_SPEED_DOWN);
			}
		}
		else if(fDistanceM2T > MONSTER_MIN_ATTACK_DISTANCE)
		{
			if(MSD_FRONT == this->m_MoveInfo.FBDirect)
			{
				this->SetSpeedDirectMoveInfo(MSD_SPEED_DOWN);
			}
			else
			{
				this->SetSpeedDirectMoveInfo(MSD_SPEED_UP);
			}
		}
		return;
	}
	
	///////////////////////////////////////////////////////////////////////////////
	// 3. 콥터형 몬스터(공중비행형 콥터)
	if(this->m_dwCurrentTick - this->m_dwTimeMonsterLastAttack < 1000)
	{
		///////////////////////////////////////////////////////////////////////////////
		// 마지막 공격한 시간에서 1초가 지나지 않았으면
		// 1. 타겟과의 거리가 MONSTER_MIN_ATTACK_DISTANCE*2보다 크면 확륙이 30%로 QuickTurn 발생
		// 2. 타겟과의 거리가 MONSTER_MIN_ATTACK_DISTANCE보다 크면 확륙이 40%로 QuickTurn 발생
		// 3. 타겟과의 거리가 MONSTER_MIN_ATTACK_DISTANCE보다 작으면 QuickTurn 발생
		if(fDistanceM2T > MONSTER_MIN_ATTACK_DISTANCE*2)
		{
			nPercent = 15;
		}
		else if(fDistanceM2T > MONSTER_MIN_ATTACK_DISTANCE)
		{
			nPercent = 40;
		}
		else
		{
			nPercent = 70;
		}
	}
	else
	{
		///////////////////////////////////////////////////////////////////////////////
		// 마지막 공격한 시간에서 1초가 지났다
		// 1. 타겟과의 거리가 MONSTER_MIN_ATTACK_DISTANCE보다 작고 마지막 공격한 시간이 20초를 초과 하지 않았을때 QuickTurn 진행
		// 2. QuickTurnTerm 시간이 경과 했다면
		//		2.1 타겟이 사정 거리 안에 있으면 QuickTurn 이동 
		//		2.2 타겟이 사정 거리 안에 없으면 시간을 초기화
		if(this->m_dwCurrentTick - this->m_dwTimeSetEnforceTargetVector > this->MonsterInfoPtr->QuickTurnTerm)
		{
			if(fDistanceM2T > pTmMonItem->pItemInfo->Range)
			{						
				this->SetEnforceTargetVector(&UnitVec3TarM2T, this->GetQuickSpeed(), MSS_OUT_OF_ATTACK_RANGE);
			}
			else
			{
				nPercent = 30;
			}
		}
		else if(fDistanceM2T > pTmMonItem->pItemInfo->Range && MSD_FRONT == m_MoveInfo.FBDirect)
		{
			GNPCRotateTargetVectorHorizontal(&UnitVec3TarM2T, &UnitVec3TarM2T
				, this->m_MoveInfo.LRDirect*(MSD_LEFT_1)
				, 10);
			this->SetEnforceTargetVector(&UnitVec3TarM2T, this->GetQuickSpeed(), MSS_OUT_OF_ATTACK_RANGE);
		}
		else if(fDistanceM2T < MONSTER_MIN_ATTACK_DISTANCE
			&& PI*2/3 < ACOS(D3DXVec3Dot(&this->TargetVector, &UnitVec3TarM2T)))
		{
			if(MonsterInfoPtr->Level < 5)
			{
				nPercent = 10;
			}
			else
			{
				nPercent = 70;
			}
		}
		else if(fDistanceM2T < MONSTER_MIN_ATTACK_DISTANCE
			&& MSD_FRONT == m_MoveInfo.FBDirect)
		{
			m_MoveInfo.FBDirect = MSD_BACK;
			m_MoveInfo.FBCurrentCount = 0;
		}
	}

	if(nRand100 < nPercent)
	{
		if(fDistanceM2T < pTmMonItem->pItemInfo->Range * 0.7)
		{
			GNPCRotateTargetVectorHorizontal(&UnitVec3TarM2T, &UnitVec3TarM2T
				, this->m_MoveInfo.LRDirect*(MONSTER_MAX_QUICK_TURN_ANGLE - this->MonsterInfoPtr->QuickTurnAngle)
				, this->MonsterInfoPtr->QuickTurnAngle);
			this->SetEnforceTargetVector(&UnitVec3TarM2T, this->GetQuickSpeed(), MSS_QUICK_TURN_GENERAL);
		}
		else if(fDistanceM2T < pTmMonItem->pItemInfo->Range)
		{
			GNPCRotateTargetVectorHorizontal(&UnitVec3TarM2T, &UnitVec3TarM2T
				, this->m_MoveInfo.LRDirect*(MSD_LEFT_15)
				, 30);
			this->SetEnforceTargetVector(&UnitVec3TarM2T, this->GetQuickSpeed(), MSS_QUICK_TURN_GENERAL);
		}
		else
		{
			GNPCRotateTargetVectorHorizontal(&UnitVec3TarM2T, &UnitVec3TarM2T
				, this->m_MoveInfo.LRDirect*(MSD_LEFT_1)
				, 10);
			this->SetEnforceTargetVector(&UnitVec3TarM2T, this->GetQuickSpeed(), MSS_OUT_OF_ATTACK_RANGE);
		}
	}
}

//////////////////////////////////////////////////////////////////////
// Method / Attack 관련
//////////////////////////////////////////////////////////////////////
int CNPCMonster::GetFirstTargetIndexfromAttackedInfo(void)
{
	int nRet = 0;
	mt_auto_lock mtA(&m_mtvectorAttackedInfoPtr);
	if(!m_mtvectorAttackedInfoPtr.empty())
	{
		nRet = (*m_mtvectorAttackedInfoPtr.begin())->UnitIndex;
	}
	return nRet;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			void CNPCMonster::SelectTargetIndex(int nTargetIdx)
/// \brief		몬스터가 공격하기 위한 TargetIndex를 선택하는 함수
/// \author		cmkwon
/// \date		2004-04-02 ~ 2004-04-02
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CNPCMonster::SelectTargetIndex(int nTargetIdx)
{
	///////////////////////////////////////////////////////////////////////////
	// 비공격 몬스터는 TargetIndex를 설정하지 않는다.
	if(MonsterInfoPtr->Belligerence == BELL_NOTATTACK
		|| IS_STRATEGYPOINT_MONSTER(MonsterInfoPtr->Belligerence)		// 2006-12-13 by cmkwon, 전략포인트 몬스터도 절대 공격하지 않는다
		|| NULL == this->m_pUsingMonsterItem)
	{
		return;
	}
	
	if(0 == nTargetIdx)
	{	// 인자가 0이므로 몬스터를 공격한 리스트에서 TargetIndex를 찾는다.

		nTargetIdx = GetFirstTargetIndexfromAttackedInfo();
	}
	
	SetTargetIndex(nTargetIdx);
}

void CNPCMonster::LockVectorAttackedItemInfo(void)
{
	m_mtvectorAttackedItemInfo.lock();
}

void CNPCMonster::UnlockVectorAttackedItemInfo(void)
{
	m_mtvectorAttackedItemInfo.unlock();
}

void CNPCMonster::InsertAttackedItemInfo(AttackedItemInfo * i_pItemInfo)
{
	m_mtvectorAttackedItemInfo.lock();
	m_mtvectorAttackedItemInfo.push_back(*i_pItemInfo);
	m_mtvectorAttackedItemInfo.unlock();
}

BOOL CNPCMonster::SelectUsingMonsterItem(void)
{
	///////////////////////////////////////////////////////////////////////////////
	// Attack 관련 변수 초기화
	ResetCurrentShotNumCount();
	ResetLastAttackTime(this->m_dwCurrentTick);

	MONSTER_ITEM	*pTmItem = NULL;
	int				nMinRemainReAttackTerm = 100000;

	////////////////////////////////////////////////////////////////////////////
	// 2009-09-09 ~ 2010 by dhjin, 인피니티 - HP 량에 따른 공격 방식을 먼저 적용한다.
	int				ItemArrayIdx = 0;
	if(this->m_HPAction.GetNextAttackItem(&ItemArrayIdx)) {
		// 연속 공격 체크
		SetUsingMonsterItem(&MonsterInfoPtr->ItemInfo[ItemArrayIdx-1]);
		return TRUE;		
	}

	int CurrentMonHPRate = 0, SelectVectIdx = 0;
	if(1 > this->CurrentHP) {
		CurrentMonHPRate = 1;
	}
	else {

		// 2010. 05. 19 by hsLee 인피니티 필드 2차 난이도 조절. (신호처리 + 몬스터 처리(서버) )
		//CurrentMonHPRate = (this->CurrentHP * 100) / this->MonsterInfoPtr->MonsterHP;
		CurrentMonHPRate = (this->CurrentHP * 100) / this->MonsterInfoExtend.fMaxHP;
		// End 2010. 05. 19 by hsLee 인피니티 필드 2차 난이도 조절. (신호처리 + 몬스터 처리(서버) )
	}
			
	if(this->m_HPAction.CheckValidSizeAttackItemIdx()
		&& this->m_HPAction.GetAttackItemIdxHPRate(CurrentMonHPRate, &ItemArrayIdx, &SelectVectIdx)) {
		if(NULL == MonsterInfoPtr->ItemInfo[ItemArrayIdx-1].pItemInfo) {
			return FALSE;
		}

		if(m_dwCurrentTick - m_ArrLastReattackTime[ItemArrayIdx-1] + 3000 > MonsterInfoPtr->ItemInfo[ItemArrayIdx-1].pItemInfo->ReAttacktime)
		{
			// 2009-09-09 ~ 2010-01-13 by dhjin, 인피니티 - 한 아이템으로 여러 대사 가능하게 수정, 밑과 같이 수정 HPActionItem 사용시 사용 카운트를 줄인다.
			// this->m_HPAction.SetSuccessAttackItemIdxHPRate(SelectVectIdx);
			this->m_HPAction.SetHPTalkAttack(ItemArrayIdx, MonsterInfoPtr->ItemInfo[ItemArrayIdx-1].pItemInfo->ItemNum);
			SetUsingMonsterItem(&MonsterInfoPtr->ItemInfo[ItemArrayIdx-1]);
			if(NULL != this->m_CurrentNPCMapChannel) {
				HPACTION_TALK_HPRATE MsgTalk;
				util::zero(&MsgTalk, sizeof(HPACTION_TALK_HPRATE));
				if(this->m_HPAction.GetPreHPTalkAttack(this->m_pUsingMonsterItem->pItemInfo->ItemNum, MsgTalk.HPTalk, &MsgTalk.HPCameraTremble)) {
					// Attack값 전송.
					MsgTalk.HPTalkImportance	= HPACTION_TALK_IMPORTANCE_CHANNEL;
					this->m_CurrentNPCMapChannel->SendFSvrHPTalk(this, &MsgTalk);
				}
			}
			if(DES_SKILL_INVINCIBLE == MonsterInfoPtr->ItemInfo[ItemArrayIdx-1].pItemInfo->ArrDestParameter[0]) {
				// 배리어 스킬 관련된 정보를 저장해둔다.
				this->m_BarrierUseItemArrayIdx	= ItemArrayIdx;
			}
			return TRUE;
		}
	}

	int nRand = RAND100();
////////////////////////////////////////////////////////////////////////////////
// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 중복 적용을 피하기 위해 공통 아이템에서만 공격을 설정한다. 밑과 같이 수정
//	for (int i = 0; i < ARRAY_SIZE_MONSTER_ITEM-1; i++)
	for (int i = 0; i < ARRAY_SIZE_MONSTER_ITEM - ARRAY_SIZE_MONSTER_SKILL_ITEM - 1; i++)
	{
		if(MonsterInfoPtr->ItemInfo[i].pItemInfo
			&& ORBIT_OBJBEAM != MonsterInfoPtr->ItemInfo[i].pItemInfo->OrbitType
			&& 0 != MonsterInfoPtr->ItemInfo[i].pItemInfo->Charging)			// 2005-12-21 by cmkwon, Charging이 0이면 죽은 후 사용 할 소환 아이템이다
		{
			if(MonsterInfoPtr->ItemInfo[i].dwUsingPercent > nRand
				&& m_dwCurrentTick - m_ArrLastReattackTime[i] + 3000 > MonsterInfoPtr->ItemInfo[i].pItemInfo->ReAttacktime)
			{
				SetUsingMonsterItem(&MonsterInfoPtr->ItemInfo[i]);
				return TRUE;
			}
			else if(NULL == pTmItem)
			{
				pTmItem = &MonsterInfoPtr->ItemInfo[i];
				nMinRemainReAttackTerm = MonsterInfoPtr->ItemInfo[i].pItemInfo->ReAttacktime - (m_dwCurrentTick - m_ArrLastReattackTime[i]);
			}
			else if(nMinRemainReAttackTerm < MonsterInfoPtr->ItemInfo[i].pItemInfo->ReAttacktime - (m_dwCurrentTick - m_ArrLastReattackTime[i]))
			{
				pTmItem = &MonsterInfoPtr->ItemInfo[i];
				nMinRemainReAttackTerm = MonsterInfoPtr->ItemInfo[i].pItemInfo->ReAttacktime - (m_dwCurrentTick - m_ArrLastReattackTime[i]);
			}
		}
	}

	if(pTmItem)
	{
		SetUsingMonsterItem(pTmItem);
		return TRUE;
	}
	
	return FALSE;
}


	
///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CNPCMonster::CheckEnableSearchTarget(void)
/// \brief		// 2007-06-26 by cmkwon, 몬스터 타겟 검색 시간 수정 - 체크 함수 추가
/// \author		cmkwon
/// \date		2007-06-26 ~ 2007-06-26
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CNPCMonster::CheckEnableSearchTarget(void)
{
	DWORD dwElapedTick	= m_dwCurrentTick - m_dwSearchTargetLastTick;
	if(2000 > dwElapedTick)
	{
		return FALSE;
	}

	m_dwSearchTargetLastTick		= m_dwCurrentTick;
	return TRUE;
}


///////////////////////////////////////////////////////////////////////////////
// virtual
///////////////////////////////////////////////////////////////////////////////
void CNPCMonster::ResetMonster(void)
{
	CMonster::ResetMonster();

	this->SetCurrentMonsterForm(0);
	this->SetCurrentMovePattern(0);	
	
	// 모든 Flag 초기화
	m_FlagNPCMonster.EnforceTargetVector		= FALSE;
	m_FlagNPCMonster.MoveTargetVectorSetAttack	= FALSE;

	// 생성 정보
	m_dwIndexCreatedMonsterData		= 0xFFFF;
	m_dwTimeCreated					= 0;
	m_dwTimeDeath					= 0;

	// 이동 정보	
	m_enMoveState					= MSS_NORMAL;
	util::zero(&m_MoveInfo, sizeof(MOVEINFO));
	m_CurrentSpeed					= 0.0f;
	m_usSpeedPercent				= 100;
	util::zero(&m_BeforePosition, sizeof(D3DXVECTOR3));
	util::zero(&m_MoveTargetVector, sizeof(D3DXVECTOR3));
	util::zero(&m_CreatedPosition, sizeof(D3DXVECTOR3));
	m_dwTimeLastMoved				= 0;
//	m_dwTimeDead					= 0;	// 2008-04-16 by cmkwon, 몬스터 사망 시 몬스터 소환 이벤트 시스템 구현 - 사용하지 않는 변수임
	m_nTimeGapChangeMonsterForm		= 0;
	m_dwLastRealMoveTime			= 0;
	
	m_dwAttackMinDistance			= 0;


	// 빠르게 이동 관련
	util::zero(&m_EnforceTargetVector, sizeof(D3DXVECTOR3));
	SetTimeSetEnforceTargetVector(GetTickCount());
	m_usTimeEnforceTargetVectorTerm	= 0;
	m_usTimeQuickTurnTerm			= 0;

	// 공격 관련
	m_dwTimeLastAttackRoutine		= 0;
	m_nTargetIndex					= 0;
	m_pUsingMonsterItem				= NULL;
	m_dwTimeMonsterLastAttack		= 0;
	// 2009-12-11 by cmkwon, 데미지 어그로로 타겟을 변경하는 몬스터 구현 - CMonster 클래스로 이동
	//m_dwSetTargetIndexLastTick		= 0;

	ResetCurrentShotNumCount();
	ResetAllLastReAttackTime(0);
	ResetLastAttackTime(0);

	// 몬스터 파티 관련
	this->ResetPartyVariable();

	m_dwLastHPRecoveryTime			= 0;
	
// 20040528 cmkwon, 2형 무기는 NPC에서 발사만 하고 관리는 하지 않는다, 이동이나 타격은 Client에서 처리한다.
//	if(m_mtvectorShootedSecondaryItemInfoPtr.empty() == false)
//	{
//		for (auto x : m_mtvectorShootedSecondaryItemInfoPtr) delete x;
//		m_mtvectorShootedSecondaryItemInfoPtr.clear();
//	}

	///////////////////////////////////////////////////////////////////////////
	// 몬스터가 공격당한 ItemInfo 벡터 삭제 처리
	m_mtvectorAttackedItemInfo.lock();
	m_mtvectorAttackedItemInfo.clear();
	m_mtvectorAttackedItemInfo.unlock();

	m_dwSearchTargetLastTick		= 0;	// 2007-06-26 by cmkwon, 몬스터 타겟 검색 시간 수정

	// start 2011-05-23 by hskim, 인피니티 3차 - 웨이포인트 구현
	m_nWayPointNowStep				= 0;
	m_nWayPointDirection			= 1;
	// end 2011-05-23 by hskim, 인피니티 3차 - 웨이포인트 구현
}

void CNPCMonster::InserttoAttackedInfoList(ActionInfo *i_pAttackedInfo)
{
	///////////////////////////////////////////////////////////////////////////////
	// 인자가 유효한지 체크
	// 몬스터가 공격을 전혀 하지 않는 성향 체크
	if (NULL == i_pAttackedInfo
		|| i_pAttackedInfo->UnitIndex == 0
		|| NULL == MonsterInfoPtr
		|| MonsterInfoPtr->Belligerence == BELL_NOTATTACK
		|| IS_STRATEGYPOINT_MONSTER(MonsterInfoPtr->Belligerence))		// 2006-12-13 by cmkwon, 전략포인트 몬스터도 절대 공격하지 않는다
	{
		return;
	}

// 2009-12-11 by cmkwon, 데미지 어그로로 타겟을 변경하는 몬스터 구현 - CNPCMonster::InserttoAttackedInfoList#, 아래와 같이 수정함.
// 	mtvectorActionInfoPtr::iterator it;
// 	mt_auto_lock mtA(&m_mtvectorAttackedInfoPtr);
// 
// 	///////////////////////////////////////////////////////////////////////////////
// 	// 1. 현재 타겟 인덱스가 설정되어 있다면 설정된 인덱스가 공격받은 정보리스트에 존재하는지 체크	
// 	// 2. 현재 TargetIndex가 0이면 인자로 들어온 인덱스를 설정한다.
// 	if(m_nTargetIndex != 0)
// 	{
// 		if(MonsterInfoPtr->Belligerence != BELL_TAGETATATTACK)
// 		{
// 			///////////////////////////////////////////////////////////////////////////////
// 			// 1. 현재 TargetIndex로 설정된 캐릭이 공격당한 정보 리스트에 없으면 인자의 인덱스로 설정
// 			// 2. 리스트에 있고 공격한지 3초가 지났으면 인자의 인덱스로 설정
// 			it = find_if(m_mtvectorAttackedInfoPtr.begin(), m_mtvectorAttackedInfoPtr.end(), listActionInfoPtr_UnitIndex_Find_Object(m_nTargetIndex));
// 			if(it == m_mtvectorAttackedInfoPtr.end())
// 			{
// 				SelectTargetIndex(i_pAttackedInfo->UnitIndex);
// 			}
// 			else if(i_pAttackedInfo->dwCurTick - (*it)->dwCurTick > 3000)
// 			{
// 				SelectTargetIndex(i_pAttackedInfo->UnitIndex);
// 			}
// 		}
// 	}
// 	else
// 	{
// 		SelectTargetIndex(i_pAttackedInfo->UnitIndex);
// 	}
// 
// 	///////////////////////////////////////////////////////////////////////////////
// 	// 1. 인자의 UnitIndex를 가진 객체정보가 리스트에 존재하지 않으므로 추가한다.
// 	// 2. 인자의 UnitIndex를 가진 정보가 리스트에 존재하므로 업데이트 하고 인자는 delete한다.
// 	it = find_if(m_mtvectorAttackedInfoPtr.begin(), m_mtvectorAttackedInfoPtr.end(),
// 		listActionInfoPtr_UnitIndex_Find_Object(i_pAttackedInfo->UnitIndex));
// 	if(it == m_mtvectorAttackedInfoPtr.end())
// 	{
// 		ActionInfo *ptmAInfo = new ActionInfo;
// 		*ptmAInfo = *i_pAttackedInfo;
// 		m_mtvectorAttackedInfoPtr.push_back(ptmAInfo);		
// 	}
// 	else
// 	{
// 		(*it)->dwCurTick	= i_pAttackedInfo->dwCurTick;
// 		(*it)->fSumDamage	+= i_pAttackedInfo->fSumDamage;
// 	}
	///////////////////////////////////////////////////////////////////////////////
	// 2009-12-11 by cmkwon, 데미지 어그로로 타겟을 변경하는 몬스터 구현 - CNPCMonster::InserttoAttackedInfoList#
	mt_auto_lock mtA(&m_mtvectorAttackedInfoPtr);
	if(0 == m_nTargetIndex)
	{
		SelectTargetIndex(i_pAttackedInfo->UnitIndex);
	}
 	mtvectorActionInfoPtr::iterator it = find_if(m_mtvectorAttackedInfoPtr.begin(), m_mtvectorAttackedInfoPtr.end(),
		listActionInfoPtr_UnitIndex_Find_Object(i_pAttackedInfo->UnitIndex));
	if(it == m_mtvectorAttackedInfoPtr.end())
	{
		ActionInfo *ptmAInfo	= new ActionInfo;
		*ptmAInfo				= *i_pAttackedInfo;
		m_mtvectorAttackedInfoPtr.push_back(ptmAInfo);

// 2009-12-10 by cmkwon, TEMP 
//		g_pNPCGlobal->WriteSystemLogEX(TRUE, "[TEMP] 091210 30100 MonIdx(%d) nTargetIndex(%d) CurTick(%d) nAttackerCnt(%d) AttackIdx(%d, %f)\r\n"
//			, this->MonsterIndex, m_nTargetIndex, m_dwCurrentTick, m_mtvectorAttackedInfoPtr.size(), ptmAInfo->UnitIndex, ptmAInfo->fSumDamage);
	}
	else
	{
		ActionInfo *ptmAInfo	= *it;
		ptmAInfo->dwCurTick		= i_pAttackedInfo->dwCurTick;
		ptmAInfo->fSumDamage	+= i_pAttackedInfo->fSumDamage;

// 2009-12-10 by cmkwon, TEMP 
//		g_pNPCGlobal->WriteSystemLogEX(TRUE, "[TEMP] 091210 30100 MonIdx(%d) nTargetIndex(%d) CurTick(%d) nAttackerCnt(%d) AttackIdx(%d, %f)\r\n"
//			, this->MonsterIndex, m_nTargetIndex, m_dwCurrentTick, m_mtvectorAttackedInfoPtr.size(), ptmAInfo->UnitIndex, ptmAInfo->fSumDamage);
	}

// 2009-12-10 by cmkwon, TEMP 
//	g_pNPCGlobal->WriteSystemLogEX(TRUE, "[TEMP] 091210 31000 MonIdx(%d) nTargetIndex(%d) CurTick(%d) nAttackerCnt(%d) AttackIdx(%d, %f)\r\n"
//		, this->MonsterIndex, m_nTargetIndex, m_dwCurrentTick, m_mtvectorAttackedInfoPtr.size(), i_pAttackedInfo->UnitIndex, i_pAttackedInfo->fSumDamage);
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			void CNPCMonster::DeleteAttackedInfowithIndex(int i_AttackIndex)
/// \brief		
/// \author		cmkwon
/// \date		2004-03-25 ~ 2004-03-25
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CNPCMonster::DeleteAttackedInfowithIndex(int i_AttackIndex)
{
	mtvectorActionInfoPtr::iterator it;
	if(i_AttackIndex == 0)
	{	// 현재 m_nTargetIndex에 설정된 공격자 정보를 삭제

		///////////////////////////////////////////////////////////////////////
		// TargetIndex가 0 이면 리턴
		if(m_nTargetIndex == 0){				return;}
		
		///////////////////////////////////////////////////////////////////////
		// TargetIndex의 정보를 찾아서 지운다.
		m_mtvectorAttackedInfoPtr.lock();
		it = find_if(m_mtvectorAttackedInfoPtr.begin(), m_mtvectorAttackedInfoPtr.end()
			, listActionInfoPtr_UnitIndex_Find_Object(m_nTargetIndex));
		if(it != m_mtvectorAttackedInfoPtr.end())
		{
			delete (*it);
			m_mtvectorAttackedInfoPtr.erase(it);
		}
		m_mtvectorAttackedInfoPtr.unlock();
		SelectTargetIndex();
	}
	else
	{	
		///////////////////////////////////////////////////////////////////////
		// 인자로 주어진 인덱스 정보를 삭제한다.
		m_mtvectorAttackedInfoPtr.lock();
		it = find_if(m_mtvectorAttackedInfoPtr.begin(), m_mtvectorAttackedInfoPtr.end()
			, listActionInfoPtr_UnitIndex_Find_Object(i_AttackIndex));
		if(it != m_mtvectorAttackedInfoPtr.end())
		{
			delete (*it);
			m_mtvectorAttackedInfoPtr.erase(it);
		}
		m_mtvectorAttackedInfoPtr.unlock();
		
		if (i_AttackIndex == m_nTargetIndex) {	SelectTargetIndex();}
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CNPCMonster::SetUserPositionVector(D3DXVECTOR3 * i_pUserPos, D3DXVECTOR3 * i_pUserTarget)
/// \brief		인피니티 - 
/// \author		dhjin
/// \date		2009-09-09 ~ 2010
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CNPCMonster::SetUserPositionVector(D3DXVECTOR3 * i_pUserPos, D3DXVECTOR3 * i_pUserTarget) {
	m_BeforePosition = PositionVector;
	///////////////////////////////////////////////////////////////////////////////
	// Monster PositionVector and TargetVector Update
	// 1. 몬스터와 유저 
	D3DXVECTOR3 tm = -(*i_pUserTarget);
	D3DXVec3Normalize(&tm, &tm);
	PositionVector = * i_pUserPos;
	PositionVector += tm * this->MonsterInfoPtr->Size;
/*		*pDestTargetVector3 = -(*pSourTargetVector3);
	PositionVector	= -(*i_pUserPos);
	D3DXVec3Normalize(pDestTargetVector3, pDestTargetVector3);
	if(0 <= i_pUserTarget->x) {
		PositionVector.x -= this->MonsterInfoPtr->Size * 2;
	}
	else {
		PositionVector.x += this->MonsterInfoPtr->Size * 2;
	}
	
	if(0 <= i_pUserTarget->z) {
		PositionVector.z -= this->MonsterInfoPtr->Size * 2;
	}
	else {
		PositionVector.z += this->MonsterInfoPtr->Size * 2;
	}
*/
	this->SetTargetVector(i_pUserTarget);
	this->SetMoveTargetVector(i_pUserTarget);

	///////////////////////////////////////////////////////////////////////////////
	// Move State Change
	this->SetMoveState(MSS_NORMAL);
}

BOOL CNPCMonster::SetCurrentMapChannel(CNPCMapChannel * i_pCurrentMapChannel) {
	if(NULL != i_pCurrentMapChannel) {
		m_CurrentNPCMapChannel	= i_pCurrentMapChannel;
		return TRUE;
	}
	
	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2009-12-11 by cmkwon, 데미지 어그로로 타겟을 변경하는 몬스터 구현 - 
/// \author		cmkwon
/// \date		2009-12-16 ~ 2009-12-16
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CNPCMonster::IsChangeableTarget(void)
{
	if(FALSE == IsValidMonster())
	{
		return FALSE;
	}

	if(MONSTER_TARGETTYPE_TUTORIAL == this->m_byMonsterTargetType
		|| BELL_TAGETATATTACK == this->MonsterInfoPtr->Belligerence
		|| ATTACKOBJ_FIRSTATTACK == this->MonsterInfoPtr->AttackObject)
	{
		return FALSE;
	}

	// 2009-09-09 ~ 2010-01-20 by dhjin, 인피니티 - 바디컨디션이 공격 준비 동작이면 타겟을 변경을 체크하지 않는다.
	if(COMPARE_BODYCON_BIT(this->BodyCondition, this->m_CurrentNPCMapChannel->GetPreAttackBodyCondMask(this->m_pUsingMonsterItem->byBodyConArrayIndex))) {
		return FALSE;
	}

	// 2010-03-16 by cmkwon, 인피2차 MtoM, MtoC 타겟 변경 관련 수정 - ChangeTarget이 FALSE이고 AttackObj가 어그로가 아니면 타겟 변하지 않음
	//						타겟 변경은 ATTACKOBJ_AGGRO와 같은 15초 단위로 처리한다.
	if(FALSE == this->MonsterInfoPtr->ChangeTarget
		&& ATTACKOBJ_AGGRO != this->MonsterInfoPtr->AttackObject)
	{
		return FALSE;
	}

// 2010-03-16 by cmkwon, 인피2차 MtoM, MtoC 타겟 변경 관련 수정 - 
// 	int nTargetChangedTickGap = (int)((INT64)this->m_dwCurrentTick - (INT64)this->GetChangedTargetIndexTick());
// 	if(TICK_MONSTER_TARGET_UNCHANGE_TERM > nTargetChangedTickGap)
// 	{
// 		return FALSE;
// 	}


	///////////////////////////////////////////////////////////////////////////////
	// 2010-03-16 by cmkwon, 인피2차 MtoM, MtoC 타겟 변경 관련 수정 - 
	int nTargetChangedTickGap = (int)((INT64)this->m_dwCurrentTick - (INT64)this->GetTimeLastCheckChangeTarget());

	if(1000 > nTargetChangedTickGap)
	{// 2010-04-14 by cmkwon, 인피2차 몬스터 랜텀 타겟 변경 처리 - 1초 동안은 타겟 변경하지 않음.
		return FALSE;
	}

	///////////////////////////////////////////////////////////////////////////////
	// 2010-04-14 by cmkwon, 인피2차 몬스터 랜텀 타겟 변경 처리 - 
	if(ATTACKOBJ_AGGRO != MonsterInfoPtr->AttackObject
		&& nTargetChangedTickGap > MonsterInfoPtr->ChangeTargetTime)
	{
		return TRUE;
	}

	if(TICK_MONSTER_TARGET_UNCHANGE_TERM > nTargetChangedTickGap)
	{
		return FALSE;
	}

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2010-01-06 by cmkwon, 몬스터 공격시 타겟 공격 가능 체크 추가(타겟변경) - 
/// \author		cmkwon
/// \date		2010-01-06 ~ 2010-01-06
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
DWORD CNPCMonster::GetTimeCheckValidTarget(void)
{
	return m_dwTimeCheckValidTarget;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2010-01-06 by cmkwon, 몬스터 공격시 타겟 공격 가능 체크 추가(타겟변경) - 
/// \author		cmkwon
/// \date		2010-01-06 ~ 2010-01-06
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CNPCMonster::SetTimeCheckValidTarget(DWORD i_dwTick)
{
	m_dwTimeCheckValidTarget = i_dwTick;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2010-01-06 by cmkwon, 몬스터 공격시 타겟 공격 가능 체크 추가(타겟변경) - 
/// \author		cmkwon
/// \date		2010-01-06 ~ 2010-01-06
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
DWORD CNPCMonster::GetTimeCheckedLastValidTarget(void)
{
	return m_dwTimeCheckedLastValidTarget;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2010-01-06 by cmkwon, 몬스터 공격시 타겟 공격 가능 체크 추가(타겟변경) - 
/// \author		cmkwon
/// \date		2010-01-06 ~ 2010-01-06
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CNPCMonster::SetTimeCheckedLastValidTarget(DWORD i_dwTick)
{
	m_dwTimeCheckedLastValidTarget = i_dwTick;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2010-03-16 by cmkwon, 인피2차 MtoM, MtoC 타겟 변경 관련 수정 - 
/// \author		cmkwon
/// \date		2010-03-16 ~ 2010-03-16
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
DWORD CNPCMonster::GetTimeLastCheckChangeTarget(void)
{
	return m_dwTimeLastCheckChangeTarget;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2010-03-16 by cmkwon, 인피2차 MtoM, MtoC 타겟 변경 관련 수정 - 
/// \author		cmkwon
/// \date		2010-03-16 ~ 2010-03-16
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CNPCMonster::SetTimeLastCheckChangeTarget(DWORD i_dwTick)
{
	m_dwTimeLastCheckChangeTarget = i_dwTick;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CNPCMonster::CheckExpireSkill()
/// \brief		인피니티(기지방어) - 
/// \author		dhjin
/// \date		2010-03-31
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CNPCMonster::CheckExpireSkill()
{
	vector<ItemNum_t> ExpireSkillNumList;
	
	m_SkillManager.AllReleaseTimeLimitSkill(&ExpireSkillNumList);

	for (auto skillnum : ExpireSkillNumList)
	{
		MessageData<T_FN_BATTLE_ATTACK_SKILL_CANCEL> pSendMsg;

		pSendMsg->MapInfo			= m_CurrentNPCMapChannel->GetMapChannelIndex();
		pSendMsg->MonsterIndex		= MonsterIndex;
		pSendMsg->SkillItemNum		= skillnum;

		m_CurrentNPCMapChannel->Send2FieldServerW(pSendMsg);
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		인피니티(기지방어) - 스킬 타겟 타입으로 대상 인덱스 가져오기
/// \author		dhjin
/// \date		2010-03-31
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CNPCMonster::GetSkillTargets(vector<ClientIndex_t> *o_pVectorClientIndex, ClientIndex_t  i_TargetIdx)
{
	ITEM *pAttItemInfo = this->m_pUsingMonsterItem->pItemInfo;
	switch(pAttItemInfo->SkillTargetType)
	{
	case SKILLTARGETTYPE_ME:
		{
			o_pVectorClientIndex->push_back(this->MonsterIndex);
		}
		break;
	case SKILLTARGETTYPE_ONE:
		{
			o_pVectorClientIndex->push_back(i_TargetIdx);
		}
		break;
	case SKILLTARGETTYPE_ONE_OURS_INRANGE_WITHOUT_ME:
		{
			ClientIndex_t nCliIdx = this->m_CurrentNPCMapChannel->GetTargetOur(this, this->m_CurrentNPCMapChannel->CheckRepairDesParam(pAttItemInfo->ArrDestParameter[0]));
			if(0 != nCliIdx)
			{
				o_pVectorClientIndex->push_back(nCliIdx);
			}
		}
		break;
	case SKILLTARGETTYPE_ALL_OURS_INRANGE_WITHOUT_ME:
		{
			this->m_CurrentNPCMapChannel->GetTargetOurList(this, o_pVectorClientIndex);
		}
		break;
	case SKILLTARGETTYPE_ONE_ENEMY_INRANGE:
		{
			o_pVectorClientIndex->push_back(i_TargetIdx);
		}
		break;
	case SKILLTARGETTYPE_MUTI_ENEMY_INRANGE:
		{
			this->m_CurrentNPCMapChannel->GetMultiTargetEnemyList(this, i_TargetIdx, o_pVectorClientIndex, this->m_pUsingMonsterItem->pItemInfo->MultiTarget);
		}
		break;
	default:
		{
		}
	}	

}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		인피니티(기지방어) - 스킬 사용!!
/// \author		dhjin
/// \date		2010-03-31
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CNPCMonster::UseSkill(ClientIndex_t i_TargetIdx)
{
	if(TRUE == this->UseOneSkillETC(i_TargetIdx))
	{
		return;
	}

	vectClientIndex_t	vectTargetList;
	GetSkillTargets(&vectTargetList, i_TargetIdx);

	switch(this->m_pUsingMonsterItem->pItemInfo->SkillType)
	{
	case SKILLTYPE_CLICK:
		this->UseSkillClick(&vectTargetList);
		break;
	case SKILLTYPE_TIMELIMIT:
		this->UseSkillTimeLimit(&vectTargetList);
		break;
	case SKILLTYPE_TOGGLE:
		break;
	case SKILLTYPE_CHARGING:
		break;
	default:
		return;
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CNPCMonster::UseOneSkillETC(ClientIndex_t i_TargetIdx)
/// \brief		인피니티(기지방어) - 예외 처리가 필요한 자기 자신 스킬
/// \author		dhjin
/// \date		2010-03-31
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CNPCMonster::UseOneSkillETC(ClientIndex_t i_TargetIdx)
{
	CLIENT_INFO *pTargetCli = NULL;
	CNPCMonster	*pTargetNMon = NULL;
	switch(this->m_pUsingMonsterItem->pItemInfo->ArrDestParameter[0])
	{
	case DES_WARP:
		// 순간이동 스킬이면 벡터를 타겟 좌표로 변경시킨다.
		if(FALSE == this->m_CurrentNPCMapChannel->GetUnitObject(i_TargetIdx, &pTargetCli, &pTargetNMon))
		{
			return FALSE;
		}
		if(NULL != pTargetCli)
		{
			this->SetUserPositionVector(&pTargetCli->PositionVector, &pTargetCli->TargetVector);
		}
		else if(NULL != pTargetNMon)
		{
			this->SetUserPositionVector(&pTargetNMon->PositionVector, &pTargetNMon->TargetVector);
		}
		this->m_CurrentNPCMapChannel->CheckMonsterPositionWarp(this, min(2.0f, (float)((this->m_dwCurrentTick - this->m_dwTimeLastMoved)/MONSTER_UPDATE_MOVE_TERM_TICK)) );
		this->m_CurrentNPCMapChannel->UpdateBlockPosition(this->m_BeforePosition.x, this->m_BeforePosition.z, this->PositionVector.x, this->PositionVector.z, this->MonsterIndex);
		break;
	case DES_SKILL_INVINCIBLE:
		this->m_HPAction.EraseHPActionByUseItemArrayIdx(this->m_BarrierUseItemArrayIdx);
		break;
	case DES_SUPERINTEND:
		// 지휘 스킬 처리
		this->m_CurrentNPCMapChannel->SetMonsterTargetInRangeByBell(this);
		break;
	default:
		return FALSE;
	}

	MessageData<T_FN_BATTLE_ATTACK_SKILL> pSendMsg;

	pSendMsg->MapInfo				= this->m_CurrentNPCMapChannel->GetMapChannelIndex();
	pSendMsg->MonsterIndex			= this->MonsterIndex;
	pSendMsg->ClientIndex			= this->MonsterIndex;
	pSendMsg->SkillItemNum			= this->m_pUsingMonsterItem->pItemInfo->ItemNum;
	pSendMsg->PositionVector		= this->PositionVector;
	pSendMsg->TargetVector			= this->TargetVector*1000.0f;
	
	// 2010. 06. 08 by hsLee 인피니티 필드 2차 난이도 조절. (아군 동일 밸런스 적용.) - 몬스터 스킬 밸런스 적용.
	pSendMsg->fIncreasePowerRatio	= this->MonsterInfoBalance.fAtkMsgRatio;
	
	m_CurrentNPCMapChannel->Send2FieldServerW(pSendMsg);

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		인피니티(기지방어) - 클릭형 
/// \author		dhjin
/// \date		2010-03-31
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CNPCMonster::UseSkillClick(vectClientIndex_t * i_pTargetIdxList)
{
	if(NULL == i_pTargetIdxList || i_pTargetIdxList->empty())	// 2010-04-01 by cmkwon, 인피2차 추가 수정 - 
	{
		return;
	}

	INIT_MSG_WITH_BUFFER(MSG_FN_BATTLE_ATTACK_SKILL, T_FN_BATTLE_ATTACK_SKILL, pSendMsg, pSendBuf);
	pSendMsg->MapInfo				= this->m_CurrentNPCMapChannel->GetMapChannelIndex();
	pSendMsg->MonsterIndex			= this->MonsterIndex;
	pSendMsg->SkillItemNum			= this->m_pUsingMonsterItem->pItemInfo->ItemNum;
	pSendMsg->PositionVector		= this->PositionVector;	// 몬스터에 현재 위치 정보는 전송하자!

	// 2010. 06. 08 by hsLee 인피니티 필드 2차 난이도 조절. (아군 동일 밸런스 적용.) - 몬스터 스킬 밸런스 적용.
	pSendMsg->fIncreasePowerRatio	= this->MonsterInfoBalance.fAtkMsgRatio;


	vectorClientIndex::iterator itr(i_pTargetIdxList->begin());
	for(; itr != i_pTargetIdxList->end(); itr++) {
		pSendMsg->ClientIndex		= *itr;	
		this->m_CurrentNPCMapChannel->Send2FieldServerW(pSendBuf, MSG_SIZE(MSG_FN_BATTLE_ATTACK_SKILL));
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		인피니티(기지방어) - 타임리밋형
/// \author		dhjin
/// \date		2010-03-31
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CNPCMonster::UseSkillTimeLimit(vectClientIndex_t * i_pTargetIdxList)
{
	if(NULL == i_pTargetIdxList || i_pTargetIdxList->empty())		// 2010-04-01 by cmkwon, 인피2차 추가 수정 - 
	{
		return;
	}

	CNPCMonster	*pTargetNMon = NULL;
	INIT_MSG_WITH_BUFFER(MSG_FN_BATTLE_ATTACK_SKILL, T_FN_BATTLE_ATTACK_SKILL, pSendMsg, pSendBuf);
	pSendMsg->MapInfo				= this->m_CurrentNPCMapChannel->GetMapChannelIndex();
	pSendMsg->MonsterIndex			= this->MonsterIndex;
	pSendMsg->SkillItemNum			= this->m_pUsingMonsterItem->pItemInfo->ItemNum;
	pSendMsg->PositionVector		= this->PositionVector;	// 몬스터에 현재 위치 정보는 전송하자!

	// 2010. 06. 08 by hsLee 인피니티 필드 2차 난이도 조절. (아군 동일 밸런스 적용.) - 몬스터 스킬 밸런스 적용.
	pSendMsg->fIncreasePowerRatio	= this->MonsterInfoBalance.fAtkMsgRatio;

	vectorClientIndex::iterator itr(i_pTargetIdxList->begin());
	for(; itr != i_pTargetIdxList->end(); itr++) 
	{
		pSendMsg->ClientIndex		= *itr;	
		this->m_CurrentNPCMapChannel->Send2FieldServerW(pSendBuf, MSG_SIZE(MSG_FN_BATTLE_ATTACK_SKILL));
		
		// 버프를 설정한다.
		pTargetNMon = this->m_CurrentNPCMapChannel->GetNPCMonster(*itr);
		if(NULL == pTargetNMon
			|| FALSE == pTargetNMon->IsValidMonster(TRUE)
			|| NULL == pTargetNMon->m_pUsingMonsterItem)	// 2010-05-20 by cmkwon, 인피2차 NPC 서버 crash 버그 수정 - NULL 체크
		{
			continue;
		}

		// 2010. 06. 08 by hsLee 인피니티 필드 2차 난이도 조절. (아군 동일 밸런스 적용.) - 몬스터 스킬 밸런스 적용.
		//pTargetNMon->m_SkillManager.ApplyTimeLimitSkill(pTargetNMon->m_pUsingMonsterItem->pItemInfo );
		pTargetNMon->m_SkillManager.ApplyTimeLimitSkill ( pTargetNMon->m_pUsingMonsterItem->pItemInfo , this->MonsterInfoBalance.fAtkMsgRatio );
	}
}

// start 2011-05-23 by hskim, 인피니티 3차 - 웨이포인트 구현
CWayPointManager *CNPCMonster::GetWayPointManager()
{
	return m_CurrentNPCMapChannel->GetNPCIOCPServer()->m_pWayPointManager;
}
// end 2011-05-23 by hskim, 인피니티 3차 - 웨이포인트 구현
