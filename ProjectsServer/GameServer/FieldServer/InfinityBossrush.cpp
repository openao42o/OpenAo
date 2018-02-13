// InfinityBossrush.cpp: implementation of the CInfinityBossrush class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "InfinityBossrush.h"
#include "FieldGlobal.h"
#include "FieldIOCPSocket.h"
#include "InfinityTickManager.h"
#include "FieldMapChannel.h"
#include "AtumLogSender.h"				// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 게임로그 추가

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CInfinityBossrush::CInfinityBossrush()
{
	this->InitInfinityBossrush();
}

CInfinityBossrush::~CInfinityBossrush()
{	

}

void CInfinityBossrush::InitInfinityBossrush() 
{
	m_CheckDelayForcedCreateMonsterCount = 0;
}

void CInfinityBossrush::DoSecondlyWorkInfinity(ATUM_DATE_TIME *pDateTime) {
	
	// 2009-09-09 ~ 2010-01-15 by dhjin, 인피니티 - 인피 성공 여부 판단 후에도 입찰 가능하게 수정. this->TenderProcessing(); 밑으로 옮김
// 	if(INFINITY_STATE_PLAYING != m_eInfinityState) {
// 		return;
// 	}

	// 2010-03-31 by dhjin, 인피니티(기지방어) - 밑과 같이 수정
// 	// 입찰 체크
// 	this->TenderProcessing();
// 
// 	if(INFINITY_STATE_PLAYING != m_eInfinityState) {
// 		// 2010-03-31 by dhjin, 인피니티(기지방어) -
// 		if(INFINITY_STATE_DONE_SUCCESS == m_eInfinityState) {
// 			// 성공 종료 연출 처리
// 			this->ProcessingCinemaFinSuccess();
// 		}
// 		else if(INFINITY_STATE_DONE_FAIL == m_eInfinityState) {
// 			// 성공 종료 연출 처리
// 			this->ProcessingCinemaFinFail();
// 		}
// 		return;
// 	}
	if(FALSE == this->DoSecondlyWorkInfinityBaseIsContinue()) 
	{
		return;
	}

	if(0 < m_CheckDelayForcedCreateMonsterCount)
	{
		m_CheckDelayForcedCreateMonsterCount--;
	}

	DWORD dwCurTick = timeGetTime();
	DWORD dwPassTick = dwCurTick - m_InfinityCreatedTick;
	
	// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 사망시 패널티 추가, 밑과 같이 수정
//	if(dwPassTick > this->m_InfinityModeInfo.LimitTime) {
	if(dwPassTick > this->m_InfinityModeInfo.LimitTime + m_InfinityLimitTick) {
		// 실패 종료
		this->Fin(FALSE, INFINITY_FIN_FAIL_BY_TIMEOVER);
		return;
	}

	if(0 >= m_CinemaInfoList.size()
		&& TRUE == this->CheckEmptyMonster_DeadForNextStepAlive()) {
		// 성공 종료
		this->Fin(TRUE, INFINITY_FIN_SUCCESS_BY_PASS_ALL_STEP);
		return;
	}

	if(0 >= m_CinemaInfoList.size()) {
		return;
	}

	this->ProcessingCinema(dwCurTick, dwPassTick);
}

void CInfinityBossrush::ProcessingCinema(DWORD i_dwCurTick, DWORD i_dwPassTick) 
{	// 2010-03-31 by dhjin, 인피니티(기지방어) - 연출 진행

	vectorCinemaInfo::iterator itrCinema = m_CinemaInfoList.begin();
	while(itrCinema != m_CinemaInfoList.end())
	{
		CINEMAINFO *pCinemaInfo = &*itrCinema;

		if(TICK_FIRST_FORCED_CREATE_MONSTER_TERM < i_dwPassTick
			&& 0 == m_CheckDelayForcedCreateMonsterCount  
			&& FALSE == this->CheckKeyMonster_DeadForNextStepAlive()
			&& pCinemaInfo->CinemaOrder != m_CinemaOrder )  
		{
//			g_pFieldGlobal->WriteSystemLogEX(TRUE, "[TEMP] [Infinity][%I64d] CInfinityBossrush::ProcessingCinema# 10000 ! PassTick(%6d) Cinema<Num(%d) Order(%d) StartTime(%6d) m_CinemaOrder(%d) \r\n"
//				, m_nInfinityCreateUID, i_dwPassTick, pCinemaInfo->CinemaNum, pCinemaInfo->CinemaOrder, pCinemaInfo->StartTime, m_CinemaOrder);

			// 키 몹 없음. 소환하기 위해 설정
			m_CinemaOrder	= pCinemaInfo->CinemaOrder;
			i_dwPassTick	= pCinemaInfo->StartTime;		// 진행중인 강제 소환이 있다면 시간 비교값을 강제로 바꿔 소환 가능하게 한다.
		}
		
		if(pCinemaInfo->CinemaOrder != m_CinemaOrder )
		{
			return;
		}

		// 시네마 체크
		if(i_dwPassTick >= pCinemaInfo->StartTime)
		{// 2010-03-31 by dhjin, 인피니티(기지방어) - 

//			g_pFieldGlobal->WriteSystemLogEX(TRUE, "[TEMP] [Infinity][%I64d] CInfinityBossrush::ProcessingCinema# 20000 ! PassTick(%6d) Cinema<Num(%d) Order(%d) StartTime(%6d) m_CinemaOrder(%d) \r\n"
//				, m_nInfinityCreateUID, i_dwPassTick, pCinemaInfo->CinemaNum, pCinemaInfo->CinemaOrder, pCinemaInfo->StartTime, m_CinemaOrder);

			// 시네마 걸렸어~!! 정보 처리 후 지우기
			INIT_MSG_WITH_BUFFER(MSG_FC_INFINITY_CINEMA, T_FC_INFINITY_CINEMA, pSMsg, SendBuf);
			*pSMsg = *pCinemaInfo;	
			this->SendAddDataPlayerList(SendBuf, MSG_SIZE(MSG_FC_INFINITY_CINEMA));

			if(pCinemaInfo->ChangeObjectIdx)
			{
				INIT_MSG_WITH_BUFFER(MSG_FN_NPCSERVER_NEW_CHANGE_OBJECT, T_FN_NPCSERVER_NEW_CHANGE_OBJECT, pChObjMsg, ChObjSendBuf);
				pChObjMsg->mapChann			= m_pInfinityMapChannel->GetMapChannelIndex();
				pChObjMsg->DeleteObjectUID	= pCinemaInfo->ObjectIdx;
				pChObjMsg->NewObjectNum		= pCinemaInfo->ChangeObjectIdx;
				ms_pFieldIOCP4->Send2NPCServerByTCP(ChObjSendBuf, MSG_SIZE(MSG_FN_NPCSERVER_NEW_CHANGE_OBJECT));
			}
			if(pCinemaInfo->MonsterIdx)
			{
				// 2010-03-31 by dhjin, 인피니티(기지방어) - // 몬스터 소환
				INIT_MSG_WITH_BUFFER(MSG_FN_NPCSERVER_CINEMA_MONSTER_CREATE, T_FN_NPCSERVER_CINEMA_MONSTER_CREATE, pSummon, SendBuf2);
				pSummon->mapChann			= m_pInfinityMapChannel->GetMapChannelIndex();
				pSummon->MonsterUnitKind	= pCinemaInfo->MonsterIdx;

				// 2010. 05. 31 by hsLee 인피니티 필드 2차 난이도 조절. (오브젝트 몬스터 밸런스 적용 문제 수정.) - 몬스터 밸런스 아군 몬스터 구분 예외처리.
				bool bDefaultBalance = false;
				bool bKey_BossMonster = false;
				
				if ( ms_pFieldIOCP4 )
				{
					MONSTER_INFO *pMonsterInfo = ms_pFieldIOCP4->GetMonsterInfo ( itrCinema->MonsterIdx );
					
// 					if ( pMonsterInfo && pMonsterInfo->Belligerence != BELL_INFINITY_DEFENSE_MONSTER )
// 						bDefaultBalance = false;

					// 2010. 06. 10 by hsLee 인피니티 필드 2차 난이도 조절. (보스러쉬 몬스터 개체 수 예외처리.)
					if ( NULL != GetKeyMonsterInfoFromKeyMonsterDeadForNextStep( pMonsterInfo->MonsterUnitKind ) 
						|| COMPARE_MPOPTION_BIT(pMonsterInfo->MPOption, MPOPTION_BIT_KEY_MONSTER_ALIVE_FOR_GAMECLEAR) )
					{
						bKey_BossMonster = true;
					}

				}
				// End 2010. 05. 31 by hsLee 인피니티 필드 2차 난이도 조절. (오브젝트 몬스터 밸런스 적용 문제 수정.)

				// 2010. 05. 19 by hsLee 인피니티 필드 2차 난이도 조절. (신호처리 + 몬스터 처리(서버) ) - 몬스터 밸런스 정보 추가.
				// 2010. 05. 31 by hsLee 인피니티 필드 2차 난이도 조절. (오브젝트 몬스터 밸런스 적용 문제 수정.) - 몬스터 밸런스 아군 몬스터 구분 예외처리.
				if ( bDefaultBalance || bKey_BossMonster )
					pSummon->MonsterSummonCount = pCinemaInfo->SummonCount;
				else
					pSummon->MonsterSummonCount	= pCinemaInfo->SummonCount * ( this->m_InfinityDifficultyData.fSummonCountRatio <= 0.0f ? 1.0f : this->m_InfinityDifficultyData.fSummonCountRatio );

				pSummon->MonsterSummonCount = max ( pSummon->MonsterSummonCount , 1 );
				// End 2010. 05. 31 by hsLee 인피니티 필드 2차 난이도 조절. (오브젝트 몬스터 밸런스 적용 문제 수정.) - 몬스터 밸런스 아군 몬스터 구분 예외처리.
				// End 2010. 05. 19 by hsLee 인피니티 필드 2차 난이도 조절. (신호처리 + 몬스터 처리(서버) ) - 몬스터 밸런스 정보 추가.

				pSummon->MaxRandomDistance	= pCinemaInfo->SummonRandomPos;

				// 2010. 05. 19 by hsLee 인피니티 필드 2차 난이도 조절. (신호처리 + 몬스터 처리(서버) ) - 몬스터 밸런스 정보 추가.
				// 2010. 05. 31 by hsLee 인피니티 필드 2차 난이도 조절. (오브젝트 몬스터 밸런스 적용 문제 수정.) - 몬스터 밸런스 아군 몬스터 구분 예외처리.
				if ( bDefaultBalance )
					pSummon->MonsterBalanceInfo.Init(0);
				else
					pSummon->MonsterBalanceInfo	= this->m_InfinityDifficultyData;
				// End 2010. 05. 31 by hsLee 인피니티 필드 2차 난이도 조절. (오브젝트 몬스터 밸런스 적용 문제 수정.)
				// End 2010. 05. 19 by hsLee 인피니티 필드 2차 난이도 조절. (신호처리 + 몬스터 처리(서버) ) - 몬스터 밸런스 정보 추가.
							
				ms_pFieldIOCP4->Send2NPCServerByTCP(SendBuf2, MSG_SIZE(MSG_FN_NPCSERVER_CINEMA_MONSTER_CREATE));
			}

			this->SetDeleteCinemaInfo(pCinemaInfo);		// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 팅긴 유저 재접속 처리
			CAtumLogSender::SendLogMessageInfinityCinema(pCinemaInfo, this->GetInfinityCreateUID());			// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 게임로그 추가, 연출 관련 로그
			itrCinema = m_CinemaInfoList.erase(itrCinema);	
			
//			g_pFieldGlobal->WriteSystemLogEX(TRUE, "[TEMP] [Infinity][%I64d] CInfinityBossrush::ProcessingCinema# 21000 ! PassTick(%6d) Cinema<Num(%d) Order(%d) StartTime(%6d) m_CinemaOrder(%d) \r\n"
//				, m_nInfinityCreateUID, i_dwPassTick, itrCinema->CinemaNum, itrCinema->CinemaOrder, itrCinema->StartTime, m_CinemaOrder);

			if(FALSE != m_CinemaOrder
				&& 0 < m_CinemaInfoList.size())
			{
				if(m_CinemaOrder != itrCinema->CinemaOrder)
				{
					// 다음 강제 소환이 없다면 강제 소환 해제 관련 변수 설정
					m_CheckDelayForcedCreateMonsterCount = COUNT_DELAY_FORCED_CREATE_MONSTER_TERM;
					return;	// 다음 단계이기 때문에 더 체크하지 않고 리턴 시킨다.

				}
			}
			continue;
		}
		break;
	}

}


// 2010. 06. 04 by hsLee 인티피니 필드 2차 난이도 조절. (GM 명령어 추가. /nextscene(다음 시네마 씬 호출.) )
/**********************************************************************************************
**
**	인티피니 영자 씬 넘기는 처리.
**
**	Create Info : 2010. 06. 04 by hsLee
**
**	Update Info : 갱신할 Scene Count 인자로 추가. 'nUpdateScene' 2010. 08. 25. by hsLee.
**
***********************************************************************************************/
void CInfinityBossrush :: ProcessingCinema ( CFieldIOCPSocket * i_MasterFISoc , int nUpdateScene /*= 1*/ )
{

	if ( NULL == i_MasterFISoc || this->GetMasterPlayerClientIdx() != i_MasterFISoc->m_character.ClientIndex )
		return;

	nUpdateScene = max ( nUpdateScene , 1 );

	vectorCinemaInfo::iterator itrCinema = m_CinemaInfoList.begin();

	int nExquteCnt = 0;

	while ( itrCinema != m_CinemaInfoList.end() )
	{
		CINEMAINFO *pCinemaInfo = &*itrCinema;

		// 2010-03-31 by dhjin, 인피니티(기지방어) - 

		// 시네마 걸렸어~!! 정보 처리 후 지우기
		INIT_MSG_WITH_BUFFER ( MSG_FC_INFINITY_CINEMA , T_FC_INFINITY_CINEMA , pSMsg , SendBuf );
			*pSMsg = *pCinemaInfo;

		this->SendAddDataPlayerList ( SendBuf , MSG_SIZE ( MSG_FC_INFINITY_CINEMA ) );

		if ( pCinemaInfo->ChangeObjectIdx )
		{
			INIT_MSG_WITH_BUFFER ( MSG_FN_NPCSERVER_NEW_CHANGE_OBJECT, T_FN_NPCSERVER_NEW_CHANGE_OBJECT, pChObjMsg, ChObjSendBuf );

			pChObjMsg->mapChann			= m_pInfinityMapChannel->GetMapChannelIndex();
			pChObjMsg->DeleteObjectUID	= pCinemaInfo->ObjectIdx;
			pChObjMsg->NewObjectNum		= pCinemaInfo->ChangeObjectIdx;

			ms_pFieldIOCP4->Send2NPCServerByTCP ( ChObjSendBuf , MSG_SIZE ( MSG_FN_NPCSERVER_NEW_CHANGE_OBJECT ) );
		}

		if ( pCinemaInfo->MonsterIdx )
		{
			// 2010-03-31 by dhjin, 인피니티(기지방어) - // 몬스터 소환
			INIT_MSG_WITH_BUFFER(MSG_FN_NPCSERVER_CINEMA_MONSTER_CREATE, T_FN_NPCSERVER_CINEMA_MONSTER_CREATE, pSummon, SendBuf2);
				pSummon->mapChann			= m_pInfinityMapChannel->GetMapChannelIndex();
				pSummon->MonsterUnitKind	= pCinemaInfo->MonsterIdx;

			// 2010. 05. 31 by hsLee 인피니티 필드 2차 난이도 조절. (오브젝트 몬스터 밸런스 적용 문제 수정.) - 몬스터 밸런스 아군 몬스터 구분 예외처리.
			bool bDefaultBalance = false;
			bool bKey_BossMonster = false;
			
			// 2010. 06. 08 by hsLee 인피니티 필드 2차 난이도 조절. (아군 동일 밸런스 적용.)
 			if ( ms_pFieldIOCP4 )
 			{
 				MONSTER_INFO *pMonsterInfo = ms_pFieldIOCP4->GetMonsterInfo ( itrCinema->MonsterIdx );

// 				if ( pMonsterInfo && pMonsterInfo->Belligerence != BELL_INFINITY_DEFENSE_MONSTER )
// 					bDefaultBalance = false;

 				// 2010. 06. 10 by hsLee 인피니티 필드 2차 난이도 조절. (보스러쉬 몬스터 개체 수 예외처리.)
				if ( NULL != GetKeyMonsterInfoFromKeyMonsterDeadForNextStep( pMonsterInfo->MonsterUnitKind ) 
					|| COMPARE_MPOPTION_BIT(pMonsterInfo->MPOption, MPOPTION_BIT_KEY_MONSTER_ALIVE_FOR_GAMECLEAR) )
				{
					bKey_BossMonster = true;
				}
 
 			}

			// End 2010. 05. 31 by hsLee 인피니티 필드 2차 난이도 조절. (오브젝트 몬스터 밸런스 적용 문제 수정.)

			// 2010. 05. 19 by hsLee 인피니티 필드 2차 난이도 조절. (신호처리 + 몬스터 처리(서버) ) - 몬스터 밸런스 정보 추가.
			// 2010. 05. 31 by hsLee 인피니티 필드 2차 난이도 조절. (오브젝트 몬스터 밸런스 적용 문제 수정.) - 몬스터 밸런스 아군 몬스터 구분 예외처리.
			if ( bDefaultBalance || bKey_BossMonster )
				pSummon->MonsterSummonCount = pCinemaInfo->SummonCount;
			else
				pSummon->MonsterSummonCount	= pCinemaInfo->SummonCount * ( this->m_InfinityDifficultyData.fSummonCountRatio <= 0.0f ? 1.0f : this->m_InfinityDifficultyData.fSummonCountRatio );

			pSummon->MonsterSummonCount = max ( pSummon->MonsterSummonCount , 1 );
			// End 2010. 05. 31 by hsLee 인피니티 필드 2차 난이도 조절. (오브젝트 몬스터 밸런스 적용 문제 수정.) - 몬스터 밸런스 아군 몬스터 구분 예외처리.
			// End 2010. 05. 19 by hsLee 인피니티 필드 2차 난이도 조절. (신호처리 + 몬스터 처리(서버) ) - 몬스터 밸런스 정보 추가.

			pSummon->MaxRandomDistance	= pCinemaInfo->SummonRandomPos;

			// 2010. 05. 19 by hsLee 인피니티 필드 2차 난이도 조절. (신호처리 + 몬스터 처리(서버) ) - 몬스터 밸런스 정보 추가.
			// 2010. 05. 31 by hsLee 인피니티 필드 2차 난이도 조절. (오브젝트 몬스터 밸런스 적용 문제 수정.) - 몬스터 밸런스 아군 몬스터 구분 예외처리.
			if ( bDefaultBalance )
				pSummon->MonsterBalanceInfo.Init(0);
			else
				pSummon->MonsterBalanceInfo	= this->m_InfinityDifficultyData;
			// End 2010. 05. 31 by hsLee 인피니티 필드 2차 난이도 조절. (오브젝트 몬스터 밸런스 적용 문제 수정.)
			// End 2010. 05. 19 by hsLee 인피니티 필드 2차 난이도 조절. (신호처리 + 몬스터 처리(서버) ) - 몬스터 밸런스 정보 추가.
						
			ms_pFieldIOCP4->Send2NPCServerByTCP ( SendBuf2 , MSG_SIZE( MSG_FN_NPCSERVER_CINEMA_MONSTER_CREATE ) );
		}

		this->SetDeleteCinemaInfo( pCinemaInfo );		// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 팅긴 유저 재접속 처리

		CAtumLogSender::SendLogMessageInfinityCinema ( pCinemaInfo , this->GetInfinityCreateUID() );			// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 게임로그 추가, 연출 관련 로그

		itrCinema = m_CinemaInfoList.erase( itrCinema );

		nExquteCnt++;

		if ( nExquteCnt >= nUpdateScene )
			break;

	}

}

// End 2010. 06. 04 by hsLee 인티피니 필드 2차 난이도 조절. (GM 명령어 추가. /nextscene(다음 시네마 씬 호출.) )