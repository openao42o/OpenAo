// InfinityDefence.cpp: implementation of the CInfinityDefence class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "InfinityDefence.h"
#include "FieldGlobal.h"
#include "FieldIOCPSocket.h"
#include "InfinityTickManager.h"
#include "FieldMapChannel.h"
#include "AtumLogSender.h"				// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 게임로그 추가

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CInfinityDefence::CInfinityDefence()
{
	this->InitInfinityDefence();
}

CInfinityDefence::~CInfinityDefence()
{

}

void CInfinityDefence::InitInfinityDefence() {

}

void CInfinityDefence::DoSecondlyWorkInfinity(ATUM_DATE_TIME *pDateTime) 
{
	if(FALSE == this->DoSecondlyWorkInfinityBaseIsContinue()) 
	{
		return;
	}

	DWORD dwCurTick = timeGetTime();
	DWORD dwPassTick = dwCurTick - m_InfinityCreatedTick;	
	if(dwPassTick > this->m_InfinityModeInfo.LimitTime)
	{// 2010-03-29 by cmkwon, 제한시간이 경과 되면 성공 처리
		this->Fin(TRUE, INFINITY_FIN_SUCCESS_BY_TIMEOVER);
		return;
	}

	if(FALSE == this->CheckEmptyMonster_AliveForGameClearAlive()) 
	{// 2010-03-29 by cmkwon, 기지 몬스터 파괴되면 실패 처리
		// 실패 종료
		this->Fin(FALSE, INFINITY_FIN_FAIL_BY_DEAD_KEYMONSTER);
		return;
	}

	///////////////////////////////////////////////////////////////////////////////

	// 2010-04-09 by cmkwon, 인피2차(성공 조건 추가) - 
	if( 0 >= m_CinemaInfoList.size() )
	{
		if( TRUE == this->CheckEmptyMonster_DeadForNextStepAlive() )
		{
			this->Fin(TRUE, INFINITY_FIN_SUCCESS_BY_DEAD_ALL_KEY_MONSTER_FOR_NEXTSTEP);
			return;
		}
	}

	this->ProcessingCinema(dwCurTick, dwPassTick);
}


/**************************************************************************************************************************
**
**	인티피니 기지방어 - 시네마 연출 처리.
**
**	Create Info :	2010-03-31 by dhjin
**
**	Update Info :	2010. 05. 19 by hsLee - 몬스터 밸런스 정보 적용.
**
**					2010. 05. 31 by hsLee - 몬스터 추가 Count가 '0'이 될 수 없도록 예외처리 추가.
**
***************************************************************************************************************************/
void CInfinityDefence::ProcessingCinema(DWORD i_dwCurTick, DWORD i_dwPassTick) 
{	// 2010-03-31 by dhjin, 인피니티(기지방어) - 연출 진행
	vectorCinemaInfo::iterator itrCinema = m_CinemaInfoList.begin();
	while(itrCinema != m_CinemaInfoList.end()) 
	{
		// 시네마 체크
		if(i_dwPassTick >= itrCinema->StartTime) 
		{		// 2010-03-31 by dhjin, 인피니티(기지방어) - 
			// 시네마 걸렸어~!! 정보 처리 후 지우기
			INIT_MSG_WITH_BUFFER(MSG_FC_INFINITY_CINEMA, T_FC_INFINITY_CINEMA, pSMsg, SendBuf);
			*pSMsg = *itrCinema;	
				this->SendAddDataPlayerList(SendBuf, MSG_SIZE(MSG_FC_INFINITY_CINEMA));
			
			if ( itrCinema->ChangeObjectIdx )
			{
				INIT_MSG_WITH_BUFFER(MSG_FN_NPCSERVER_NEW_CHANGE_OBJECT, T_FN_NPCSERVER_NEW_CHANGE_OBJECT, pChObjMsg, ChObjSendBuf);
				pChObjMsg->mapChann			= m_pInfinityMapChannel->GetMapChannelIndex();
				pChObjMsg->DeleteObjectUID	= itrCinema->ObjectIdx;
				pChObjMsg->NewObjectNum		= itrCinema->ChangeObjectIdx;
				((CFieldIOCP*)(m_pInfinityMapChannel->m_pFieldMapWorkspace->m_pIOCPServer))->Send2NPCServerByTCP(ChObjSendBuf, MSG_SIZE(MSG_FN_NPCSERVER_NEW_CHANGE_OBJECT));
			}

			if( itrCinema->MonsterIdx ) 
			{
				// 2010-03-31 by dhjin, 인피니티(기지방어) - // 몬스터 소환
				INIT_MSG_WITH_BUFFER(MSG_FN_NPCSERVER_CINEMA_MONSTER_CREATE, T_FN_NPCSERVER_CINEMA_MONSTER_CREATE, pSummon, SendBuf2);
				pSummon->mapChann			= m_pInfinityMapChannel->GetMapChannelIndex();
				pSummon->MonsterUnitKind	= itrCinema->MonsterIdx;

				// 2010. 05. 31 by hsLee 인피니티 필드 2차 난이도 조절. (오브젝트 몬스터 밸런스 적용 문제 수정.) - 몬스터 밸런스 아군 몬스터 구분 예외처리.
				bool bDefaultBalance = false;
				bool bKey_BossMonster = false;

				// 2010. 06. 14 by hsLee 인피니티 필드 2차 난이도 조절. (아군 몬스터 밸런스 상한선 제한.) - 기지방어 관련 아군 몬스터 밸런스 예외처리.
				bool bLimitBalance = false;

				MONSTER_INFO *pMonsterInfo = ms_pFieldIOCP4->GetMonsterInfo ( itrCinema->MonsterIdx );
				
 				if ( ms_pFieldIOCP4 )
 				{
 					pMonsterInfo = ms_pFieldIOCP4->GetMonsterInfo ( itrCinema->MonsterIdx );
 					
					// 2010. 06. 14 by hsLee 인피니티 필드 2차 난이도 조절. (아군 몬스터 밸런스 상한선 제한.) - 기지방어 관련 아군 몬스터 밸런스 예외처리.
					if ( pMonsterInfo && pMonsterInfo->Belligerence == BELL_INFINITY_DEFENSE_MONSTER )
						bLimitBalance = true;

//  					if ( pMonsterInfo && pMonsterInfo->Belligerence != BELL_INFINITY_DEFENSE_MONSTER )
// 						bDefaultBalance = false;
 				 
 					if ( COMPARE_MPOPTION_BIT(pMonsterInfo->MPOption, MPOPTION_BIT_KEY_MONSTER_ALIVE_FOR_GAMECLEAR) )
 						bKey_BossMonster = true;
 				}
				// 2010. 05. 31 by hsLee 인피니티 필드 2차 난이도 조절. (오브젝트 몬스터 밸런스 적용 문제 수정.) - 몬스터 밸런스 아군 몬스터 구분 예외처리.

				// 2010. 05. 19 by hsLee 인피니티 필드 2차 난이도 조절. (신호처리 + 몬스터 처리(서버) ) - 몬스터 밸런스 정보 추가.
				// 2010. 05. 31 by hsLee 인피니티 필드 2차 난이도 조절. (오브젝트 몬스터 밸런스 적용 문제 수정.) - 몬스터 소환 카운트 '0'이 될수 없게 예외처리.
				if ( bDefaultBalance || bKey_BossMonster )
					pSummon->MonsterSummonCount = itrCinema->SummonCount;
				else
					pSummon->MonsterSummonCount	= itrCinema->SummonCount * this->m_InfinityDifficultyData.fSummonCountRatio;
				
				pSummon->MonsterSummonCount = max ( pSummon->MonsterSummonCount , 1 );
				// End 2010. 05. 31 by hsLee 인피니티 필드 2차 난이도 조절. (오브젝트 몬스터 밸런스 적용 문제 수정.)
				// End 2010. 05. 19 by hsLee 인피니티 필드 2차 난이도 조절. (신호처리 + 몬스터 처리(서버) ) - 몬스터 밸런스 정보 추가.

				pSummon->MaxRandomDistance	= itrCinema->SummonRandomPos;

				// 2010. 05. 19 by hsLee 인피니티 필드 2차 난이도 조절. (신호처리 + 몬스터 처리(서버) ) - 몬스터 밸런스 정보 추가.
				// 2010. 05. 31 by hsLee 인피니티 필드 2차 난이도 조절. (오브젝트 몬스터 밸런스 적용 문제 수정.) - 몬스터 밸런스 아군 몬스터 구분 예외처리.
				if ( bDefaultBalance )
				{
					pSummon->MonsterBalanceInfo.Init(0);
				}
				else
				{
					// 2010. 06. 14 by hsLee 인피니티 필드 2차 난이도 조절. (아군 몬스터 밸런스 상한선 제한.) - 기지방어 관련 아군 몬스터 밸런스 예외처리.
					if ( bLimitBalance && this->m_InfinityDifficultyLevel > DEFAULT_LEVEL_INFINITY_DIFFICULTY )
					{
						if ( ms_pFieldIOCP4 )
						{
							MONSTER_BALANCE_DATA sUpdateBalanceData ( DEFAULT_LEVEL_INFINITY_DIFFICULTY );

							const INFINITY_DIFFICULTY_BONUS_INFO *a_pInfinity_BonusInfo = ms_pFieldIOCP4->m_InfinityManager.Get_Difficulty_BonusInfo( sUpdateBalanceData.DifficultyStep );
							const INFINITY_DIFFICULTY_MONSTER_SETTING_INFO *a_pInfinity_MonsterInfo = ms_pFieldIOCP4->m_InfinityManager.Get_Difficulty_MonsterInfo ( sUpdateBalanceData.DifficultyStep );

							if ( a_pInfinity_BonusInfo )
								sUpdateBalanceData += *a_pInfinity_BonusInfo;

							if ( a_pInfinity_MonsterInfo )
								sUpdateBalanceData += *a_pInfinity_MonsterInfo;

							pSummon->MonsterBalanceInfo = sUpdateBalanceData;
						}
						else
						{
							pSummon->MonsterBalanceInfo.Init(0);
						}
					}
					else
					{
						pSummon->MonsterBalanceInfo = this->m_InfinityDifficultyData;
					}
				}
				// End 2010. 05. 31 by hsLee 인피니티 필드 2차 난이도 조절. (오브젝트 몬스터 밸런스 적용 문제 수정.)
				// End 2010. 05. 19 by hsLee 인피니티 필드 2차 난이도 조절. (신호처리 + 몬스터 처리(서버) ) - 몬스터 밸런스 정보 추가.

				((CFieldIOCP*)(m_pInfinityMapChannel->m_pFieldMapWorkspace->m_pIOCPServer))->Send2NPCServerByTCP(SendBuf2, MSG_SIZE(MSG_FN_NPCSERVER_CINEMA_MONSTER_CREATE));
			}

			this->SetDeleteCinemaInfo(&*itrCinema);		// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 팅긴 유저 재접속 처리
			CAtumLogSender::SendLogMessageInfinityCinema(&*itrCinema, this->GetInfinityCreateUID());			// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 게임로그 추가, 연출 관련 로그
			itrCinema = m_CinemaInfoList.erase(itrCinema);

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
void CInfinityDefence :: ProcessingCinema ( CFieldIOCPSocket * i_pMasterFISoc , int nUpdateScene /*= 1*/ )
{

	if ( NULL == i_pMasterFISoc || this->GetMasterPlayerClientIdx() != i_pMasterFISoc->m_character.ClientIndex )
		return;

	nUpdateScene = max ( nUpdateScene , 1 );

	// 2010-03-31 by dhjin, 인피니티(기지방어) - 연출 진행
	vectorCinemaInfo::iterator itrCinema = m_CinemaInfoList.begin();

	int nExquteCnt = 0;

	while ( itrCinema != m_CinemaInfoList.end() )
	{	// 시네마 체크

		// 2010-03-31 by dhjin, 인피니티(기지방어) - 

		// 시네마 걸렸어~!! 정보 처리 후 지우기
		INIT_MSG_WITH_BUFFER ( MSG_FC_INFINITY_CINEMA , T_FC_INFINITY_CINEMA , pSMsg , SendBuf );
			*pSMsg = *itrCinema;

		this->SendAddDataPlayerList ( SendBuf , MSG_SIZE( MSG_FC_INFINITY_CINEMA ) );
		
		if ( itrCinema->ChangeObjectIdx )
		{
			INIT_MSG_WITH_BUFFER ( MSG_FN_NPCSERVER_NEW_CHANGE_OBJECT , T_FN_NPCSERVER_NEW_CHANGE_OBJECT , pChObjMsg , ChObjSendBuf );
				pChObjMsg->mapChann			= m_pInfinityMapChannel->GetMapChannelIndex();
				pChObjMsg->DeleteObjectUID	= itrCinema->ObjectIdx;
				pChObjMsg->NewObjectNum		= itrCinema->ChangeObjectIdx;

			((CFieldIOCP*)(m_pInfinityMapChannel->m_pFieldMapWorkspace->m_pIOCPServer))->Send2NPCServerByTCP ( ChObjSendBuf , MSG_SIZE(MSG_FN_NPCSERVER_NEW_CHANGE_OBJECT) );
		}
		
		if ( itrCinema->MonsterIdx ) 
		{
			// 2010-03-31 by dhjin, 인피니티(기지방어) - // 몬스터 소환
			INIT_MSG_WITH_BUFFER ( MSG_FN_NPCSERVER_CINEMA_MONSTER_CREATE , T_FN_NPCSERVER_CINEMA_MONSTER_CREATE , pSummon , SendBuf2 );
				pSummon->mapChann			= m_pInfinityMapChannel->GetMapChannelIndex();
				pSummon->MonsterUnitKind	= itrCinema->MonsterIdx;
			
			// 2010. 05. 31 by hsLee 인피니티 필드 2차 난이도 조절. (오브젝트 몬스터 밸런스 적용 문제 수정.) - 몬스터 밸런스 아군 몬스터 구분 예외처리.
			bool bDefaultBalance = false;
			bool bKey_BossMonster = false;

			// 2010. 06. 14 by hsLee 인피니티 필드 2차 난이도 조절. (아군 몬스터 밸런스 상한선 제한.) - 기지방어 관련 아군 몬스터 밸런스 예외처리.
			bool bLimitBalance = false;
			
			// 2010. 06. 08 by hsLee 인피니티 필드 2차 난이도 조절. (아군 동일 밸런스 적용.) - 기존 구별 처리된 부분 생략.
 			if ( ms_pFieldIOCP4 )
 			{
 				MONSTER_INFO *pMonsterInfo = ms_pFieldIOCP4->GetMonsterInfo ( itrCinema->MonsterIdx );
 				
// 				if ( pMonsterInfo && pMonsterInfo->Belligerence != BELL_INFINITY_DEFENSE_MONSTER )
// 					bDefaultBalance = false;

				// 2010. 06. 14 by hsLee 인피니티 필드 2차 난이도 조절. (아군 몬스터 밸런스 상한선 제한.) - 기지방어 관련 아군 몬스터 밸런스 예외처리.
				if ( pMonsterInfo && pMonsterInfo->Belligerence == BELL_INFINITY_DEFENSE_MONSTER )
					bLimitBalance = true;

 				if ( COMPARE_MPOPTION_BIT(pMonsterInfo->MPOption, MPOPTION_BIT_KEY_MONSTER_ALIVE_FOR_GAMECLEAR) )
 					bKey_BossMonster = true;
 			}

			// 2010. 05. 31 by hsLee 인피니티 필드 2차 난이도 조절. (오브젝트 몬스터 밸런스 적용 문제 수정.) - 몬스터 밸런스 아군 몬스터 구분 예외처리.
			
			// 2010. 05. 19 by hsLee 인피니티 필드 2차 난이도 조절. (신호처리 + 몬스터 처리(서버) ) - 몬스터 밸런스 정보 추가.
			// 2010. 05. 31 by hsLee 인피니티 필드 2차 난이도 조절. (오브젝트 몬스터 밸런스 적용 문제 수정.) - 몬스터 소환 카운트 '0'이 될수 없게 예외처리.
			if ( bDefaultBalance || bKey_BossMonster )
				pSummon->MonsterSummonCount = itrCinema->SummonCount;
			else
				pSummon->MonsterSummonCount	= itrCinema->SummonCount * this->m_InfinityDifficultyData.fSummonCountRatio;
			
			pSummon->MonsterSummonCount = max ( pSummon->MonsterSummonCount , 1 );
			// End 2010. 05. 31 by hsLee 인피니티 필드 2차 난이도 조절. (오브젝트 몬스터 밸런스 적용 문제 수정.)
			// End 2010. 05. 19 by hsLee 인피니티 필드 2차 난이도 조절. (신호처리 + 몬스터 처리(서버) ) - 몬스터 밸런스 정보 추가.
			
			pSummon->MaxRandomDistance	= itrCinema->SummonRandomPos;
			
			// 2010. 05. 19 by hsLee 인피니티 필드 2차 난이도 조절. (신호처리 + 몬스터 처리(서버) ) - 몬스터 밸런스 정보 추가.
			// 2010. 05. 31 by hsLee 인피니티 필드 2차 난이도 조절. (오브젝트 몬스터 밸런스 적용 문제 수정.) - 몬스터 밸런스 아군 몬스터 구분 예외처리.
			if ( bDefaultBalance )
				pSummon->MonsterBalanceInfo.Init(0);
			else
			{

				// 2010. 06. 14 by hsLee 인피니티 필드 2차 난이도 조절. (아군 몬스터 밸런스 상한선 제한.) - 기지방어 관련 아군 몬스터 밸런스 예외처리.
				if ( bLimitBalance && this->m_InfinityDifficultyLevel > DEFAULT_LEVEL_INFINITY_DIFFICULTY )
				{
					if ( ms_pFieldIOCP4 )
					{
						MONSTER_BALANCE_DATA sUpdateBalanceData ( DEFAULT_LEVEL_INFINITY_DIFFICULTY );
						
						const INFINITY_DIFFICULTY_BONUS_INFO *a_pInfinity_BonusInfo = ms_pFieldIOCP4->m_InfinityManager.Get_Difficulty_BonusInfo( sUpdateBalanceData.DifficultyStep );
						const INFINITY_DIFFICULTY_MONSTER_SETTING_INFO *a_pInfinity_MonsterInfo = ms_pFieldIOCP4->m_InfinityManager.Get_Difficulty_MonsterInfo ( sUpdateBalanceData.DifficultyStep );
						
						if ( a_pInfinity_BonusInfo )
							sUpdateBalanceData += *a_pInfinity_BonusInfo;
						
						if ( a_pInfinity_MonsterInfo )
							sUpdateBalanceData += *a_pInfinity_MonsterInfo;
						
						pSummon->MonsterBalanceInfo = sUpdateBalanceData;
					}
					else
					{
						pSummon->MonsterBalanceInfo.Init(0);
					}
				}
				else
				{
					pSummon->MonsterBalanceInfo = this->m_InfinityDifficultyData;
				}
			}
			// End 2010. 05. 31 by hsLee 인피니티 필드 2차 난이도 조절. (오브젝트 몬스터 밸런스 적용 문제 수정.)
			// End 2010. 05. 19 by hsLee 인피니티 필드 2차 난이도 조절. (신호처리 + 몬스터 처리(서버) ) - 몬스터 밸런스 정보 추가.
			
			((CFieldIOCP*)(m_pInfinityMapChannel->m_pFieldMapWorkspace->m_pIOCPServer))->Send2NPCServerByTCP ( SendBuf2 , MSG_SIZE ( MSG_FN_NPCSERVER_CINEMA_MONSTER_CREATE ) );
		}
		
		this->SetDeleteCinemaInfo ( &*itrCinema );		// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 팅긴 유저 재접속 처리

		CAtumLogSender::SendLogMessageInfinityCinema( &*itrCinema , this->GetInfinityCreateUID() );			// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 게임로그 추가, 연출 관련 로그

		itrCinema = m_CinemaInfoList.erase(itrCinema);

		nExquteCnt++;

		if ( nExquteCnt >= nUpdateScene )
			break;
	}

}
// End 2010. 06. 04 by hsLee 인티피니 필드 2차 난이도 조절. (GM 명령어 추가. /nextscene(다음 시네마 씬 호출.) )