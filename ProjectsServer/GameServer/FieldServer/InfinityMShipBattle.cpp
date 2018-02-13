// InfinityMShipBattle.cpp: implementation of the CInfinityMShipBattle class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "InfinityMShipBattle.h"
#include "FieldGlobal.h"
#include "FieldIOCPSocket.h"
#include "InfinityTickManager.h"
#include "FieldMapChannel.h"
#include "AtumLogSender.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CInfinityMShipBattle::CInfinityMShipBattle()
{
	this->InitInfinityMShipBattle();

	m_BonusStepLimitTime = 0;
}

CInfinityMShipBattle::~CInfinityMShipBattle()
{
}

void CInfinityMShipBattle::InitInfinityMShipBattle()
{
}

void CInfinityMShipBattle::DoSecondlyWorkInfinity(ATUM_DATE_TIME *pDateTime)
{
	eINFINITY_STATE tmInfiState = m_eInfinityState;

	if(FALSE == this->DoSecondlyWorkInfinityBaseIsContinue()) 
	{
		return;
	}

	DWORD dwCurTick = timeGetTime();
	DWORD dwPassTick = dwCurTick - m_InfinityCreatedTick;

	// 전체 시간 제한 종료
	if( tmInfiState < INFINITY_STATE_PLAYING_STEP8 && dwPassTick > this->m_InfinityModeInfo.LimitTime + m_InfinityLimitTick)
	{
		this->Fin(FALSE, INFINITY_FIN_FAIL_BY_TIMEOVER);
		
		return;
	}

	if( TRUE == IS_INFINITY_STATE_PLAYING_STEP(tmInfiState) )
	{
		dwPassTick = dwCurTick - m_InfinityStepStartTick;
	}

	// 보나스 시간 제한 종료
	if( m_BonusStepLimitTime != 0 && tmInfiState >= INFINITY_STATE_PLAYING_STEP8 && dwPassTick > m_BonusStepLimitTime )
	{
		this->Fin(TRUE, INFINITY_FIN_SUCCESS_BY_TIMEOVER);
	
		return;
	}

	// 모선 파괴시 종료
	if( FALSE == this->CheckEmptyMonster_AliveForGameClearAlive() )
	{
		this->Fin(FALSE, INFINITY_FIN_FAIL_BY_DEAD_KEYMONSTER);

		return;
	}

	// 각 스텝 진행
	if( 0 >= m_CinemaInfoList.size() )
	{
 		if( TRUE == this->CheckEmptyMonster_DeadForNextStepAlive() )
		{
      		if( CINEMA_PLAYING_STEP_MAX == m_PlayingStepCount )		// 마지막 일경우
			{
				this->Fin(TRUE, INFINITY_FIN_SUCCESS_BY_PASS_ALL_STEP);

				return ;
			}

			if( TRUE == IS_INFINITY_STATE_PLAYING(tmInfiState) )
			{
				// 복사 처리
   				vectorCinemaInfo::iterator itrCinema = m_CinemaStepInfoList[m_PlayingStepCount].begin();
				for(; itrCinema != m_CinemaStepInfoList[m_PlayingStepCount].end(); itrCinema++)
				{
					m_CinemaInfoList.push_back(*itrCinema);
				}

				m_InfinityStepStartTick = timeGetTime();
				dwCurTick = timeGetTime();

				dwPassTick = dwCurTick - m_InfinityStepStartTick;
				m_PlayingStepCount++;

				SetInfinityState(GetInfinityStatePlayingNext(tmInfiState));
				this->SetKeyMonsterList();
			}
		}
	}

	this->ProcessingCinema(dwCurTick, dwPassTick);
}

/**************************************************************************************************************************
**
**	인피니티 3차 - 방 생성 작업 - 시네마 연출 처리.
**
**	Create Info :	2011-02-17 by hskim
**
**	Update Info :
**
**
***************************************************************************************************************************/
void CInfinityMShipBattle::ProcessingCinema(DWORD i_dwCurTick, DWORD i_dwPassTick) 
{
	vectorCinemaInfo::iterator itrCinema = m_CinemaInfoList.begin();

	while(itrCinema != m_CinemaInfoList.end()) 
	{
		if(i_dwPassTick >= itrCinema->StartTime) 
		{
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

			if( itrCinema->CustomIdx )
			{
				switch( itrCinema->CustomIdx )
				{
				case INFINITY3_STEP5_MOVIE_START:
					{
						INIT_MSG_WITH_BUFFER(MSG_FC_INFINITY_MOVIE, T_FC_INFINITY_MOVIE, pMovieMsg, MovieBuf);
						pMovieMsg->UniqueNumber	= INFINITY_MOVIE_001_INF3_STEP5;
						pMovieMsg->Command		= MOVIE_COMMAND_START;

						this->SendAddDataPlayerList(MovieBuf, MSG_SIZE(MSG_FC_INFINITY_MOVIE));

						break;
					}

				case INFINITY3_STEP5_MOVIE_END:
					{
						INIT_MSG_WITH_BUFFER(MSG_FC_INFINITY_MOVIE, T_FC_INFINITY_MOVIE, pMovieMsg, MovieBuf);
						pMovieMsg->UniqueNumber	= INFINITY_MOVIE_001_INF3_STEP5;
						pMovieMsg->Command		= MOVIE_COMMAND_END;

						this->SendAddDataPlayerList(MovieBuf, MSG_SIZE(MSG_FC_INFINITY_MOVIE));

						break;
					}

				case INFINITY3_STEP5_TIMELIMIT_SET:
					{
						INIT_MSG_WITH_BUFFER(MSG_FC_INFINITY_SET_LIMITTIME, T_FC_INFINITY_SET_LIMITTIME, pLimitTimeMsg, LimitTimeBuf);
						pLimitTimeMsg->LimitTime = INFINITY3_STEP6_TIMELIMIT_VALUE;
						this->SendAddDataPlayerList(LimitTimeBuf, MSG_SIZE(MSG_FC_INFINITY_SET_LIMITTIME));
						
					break;
					}

				case INFINITY3_STEP6_MOVE_SUCCESS_POSITION:
					{
						this->MoveAllPlayer(D3DXVECTOR3(INFINITY3_STEP6_MOVE_SUCCESS_POSTION_X, INFINITY3_STEP6_MOVE_SUCCESS_POSTION_Y, INFINITY3_STEP6_MOVE_SUCCESS_POSTION_Z));
						
						break;
					}
					
				case INFINITY3_STEP6_MOVE_FAIL_POSITION:
					{
						this->MoveAllPlayer(D3DXVECTOR3(INFINITY3_STEP6_MOVE_FAIL_POSTION_X, INFINITY3_STEP6_MOVE_FAIL_POSTION_Y, INFINITY3_STEP6_MOVE_FAIL_POSTION_Z));

						break;
					}

				case INFINITY3_FAIL_MOVIE_START:
					{
						INIT_MSG_WITH_BUFFER(MSG_FC_INFINITY_MOVIE, T_FC_INFINITY_MOVIE, pMovieMsg, MovieBuf);
						pMovieMsg->UniqueNumber	= INFINITY_MOVIE_002_INF3_FAIL;
						pMovieMsg->Command		= MOVIE_COMMAND_START;
						
						this->SendAddDataPlayerList(MovieBuf, MSG_SIZE(MSG_FC_INFINITY_MOVIE));
						
						break;
					}

				default:
					break;
				}
			}

			if( itrCinema->MonsterIdx ) 
			{
				// start 2011-04-28 by hskim, 인피니티 3차 - 시네마 관련 기능 추가 - 해당 맵채널 특정 몬스터 삭제/변경 기능 추가
				switch( itrCinema->MonsterIdxType )
				{
					case CINEMA_MONSTER_INDEX_REGEN_ON :
					{
							INIT_MSG_WITH_BUFFER(MSG_FN_NPCSERVER_CINEMA_MONSTER_REGEN, T_FN_NPCSERVER_CINEMA_MONSTER_REGEN, pRegen, SendBufRegen);
							pRegen->mapChann			= m_pInfinityMapChannel->GetMapChannelIndex();
							pRegen->iMonsterUnitKind	= itrCinema->MonsterIdx;
							pRegen->bRegen			= TRUE;
							
						bool bLimitBalance = false;

						MONSTER_INFO *pMonsterInfo = ms_pFieldIOCP4->GetMonsterInfo ( itrCinema->MonsterIdx );

						if ( ms_pFieldIOCP4 )
						{
							pMonsterInfo = ms_pFieldIOCP4->GetMonsterInfo ( itrCinema->MonsterIdx );
							
							if ( pMonsterInfo && pMonsterInfo->Belligerence == BELL_INFINITY_DEFENSE_MONSTER )
								bLimitBalance = true;
						}
					
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
								
								pRegen->MonsterBalanceInfo = sUpdateBalanceData;
							}
							else
							{
								pRegen->MonsterBalanceInfo.Init(0);
							}
						}
						else
						{
							pRegen->MonsterBalanceInfo = this->m_InfinityDifficultyData;
						}

							((CFieldIOCP*)(m_pInfinityMapChannel->m_pFieldMapWorkspace->m_pIOCPServer))->Send2NPCServerByTCP(SendBufRegen, MSG_SIZE(MSG_FN_NPCSERVER_CINEMA_MONSTER_REGEN));
							
							break;
					}
						
					case CINEMA_MONSTER_INDEX_REGEN_OFF :
					{
							INIT_MSG_WITH_BUFFER(MSG_FN_NPCSERVER_CINEMA_MONSTER_REGEN, T_FN_NPCSERVER_CINEMA_MONSTER_REGEN, pRegen, SendBufRegen);
							pRegen->mapChann			= m_pInfinityMapChannel->GetMapChannelIndex();
							pRegen->iMonsterUnitKind	= itrCinema->MonsterIdx;
							pRegen->bRegen			= FALSE;
							
							((CFieldIOCP*)(m_pInfinityMapChannel->m_pFieldMapWorkspace->m_pIOCPServer))->Send2NPCServerByTCP(SendBufRegen, MSG_SIZE(MSG_FN_NPCSERVER_CINEMA_MONSTER_REGEN));
							
							break;
					}

					case CINEMA_MONSTER_INDEX_DESTORY :
					{
						INIT_MSG_WITH_BUFFER(MSG_FN_NPCSERVER_CINEMA_MONSTER_DESTROY, T_FN_NPCSERVER_CINEMA_MONSTER_DESTROY, pDestroy, SendBufDestroy);
						pDestroy->mapChann			= m_pInfinityMapChannel->GetMapChannelIndex();
						pDestroy->MonsterUnitKind	= itrCinema->MonsterIdx;
						
						((CFieldIOCP*)(m_pInfinityMapChannel->m_pFieldMapWorkspace->m_pIOCPServer))->Send2NPCServerByTCP(SendBufDestroy, MSG_SIZE(MSG_FN_NPCSERVER_CINEMA_MONSTER_DESTROY));
						
						break;
					}

					case CINEMA_MONSTER_INDEX_CHANGE :
					{
 						INIT_MSG_WITH_BUFFER(MSG_FN_NPCSERVER_CINEMA_MONSTER_CHANGE, T_FN_NPCSERVER_CINEMA_MONSTER_CHANGE, pChange, SendBufChange);
						pChange->mapChann			= m_pInfinityMapChannel->GetMapChannelIndex();
						pChange->MonsterUnitKind	= itrCinema->MonsterIdx;
						pChange->ChangeMonsterUnitKind	= itrCinema->UpdateMonsterIdx;
						
						((CFieldIOCP*)(m_pInfinityMapChannel->m_pFieldMapWorkspace->m_pIOCPServer))->Send2NPCServerByTCP(SendBufChange, MSG_SIZE(MSG_FN_NPCSERVER_CINEMA_MONSTER_CHANGE));

						break;
					}

					case CINEMA_MONSTER_INDEX_CREATE:
					default :	// CINEMA_MONSTER_INDEX_CREATE
					{
						INIT_MSG_WITH_BUFFER(MSG_FN_NPCSERVER_CINEMA_MONSTER_CREATE, T_FN_NPCSERVER_CINEMA_MONSTER_CREATE, pSummon, SendBuf2);
						pSummon->mapChann			= m_pInfinityMapChannel->GetMapChannelIndex();
						pSummon->MonsterUnitKind	= itrCinema->MonsterIdx;
						
						bool bDefaultBalance = false;
						bool bKey_BossMonster = false;
						
						bool bLimitBalance = false;
						
						MONSTER_INFO *pMonsterInfo = ms_pFieldIOCP4->GetMonsterInfo ( itrCinema->MonsterIdx );
						
						if ( ms_pFieldIOCP4 )
						{
							pMonsterInfo = ms_pFieldIOCP4->GetMonsterInfo ( itrCinema->MonsterIdx );
							
							if ( pMonsterInfo && pMonsterInfo->Belligerence == BELL_INFINITY_DEFENSE_MONSTER )
								bLimitBalance = true;
							
							if ( COMPARE_MPOPTION_BIT(pMonsterInfo->MPOption, MPOPTION_BIT_KEY_MONSTER_ALIVE_FOR_GAMECLEAR) || itrCinema->KeyMonster > 0 )
								bKey_BossMonster = true;
						}
						
						if ( bDefaultBalance || bKey_BossMonster )
							pSummon->MonsterSummonCount = itrCinema->SummonCount;
						else
							pSummon->MonsterSummonCount	= itrCinema->SummonCount * this->m_InfinityDifficultyData.fSummonCountRatio;
						
						pSummon->MonsterSummonCount = max ( pSummon->MonsterSummonCount , 1 );
						
						pSummon->MaxRandomDistance	= itrCinema->SummonRandomPos;
						
						if ( bDefaultBalance )
						{
							pSummon->MonsterBalanceInfo.Init(0);
						}
						else
						{
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
						
						((CFieldIOCP*)(m_pInfinityMapChannel->m_pFieldMapWorkspace->m_pIOCPServer))->Send2NPCServerByTCP(SendBuf2, MSG_SIZE(MSG_FN_NPCSERVER_CINEMA_MONSTER_CREATE));

						break;
					}
				}
				// end 2011-04-28 by hskim, 인피니티 3차 - 시네마 관련 기능 추가 - 해당 맵채널 특정 몬스터 삭제/변경 기능 추가
			}

			this->SetDeleteCinemaInfo(&*itrCinema);
			CAtumLogSender::SendLogMessageInfinityCinema(&*itrCinema, this->GetInfinityCreateUID());
			itrCinema = m_CinemaInfoList.erase(itrCinema);

			continue;
		}

		break;
	}
}

/**********************************************************************************************
**
**	인피니티 3차 - 방 생성 작업 - 운영자가 다음씬으로 넘기기는 부분
**
**	Create Info : 2011. 02. 17 by hskim
**
**	Update Info : 
**
***********************************************************************************************/
void CInfinityMShipBattle::ProcessingCinema(CFieldIOCPSocket * i_MasterFISoc , int nUpdateScene /*= 1*/)
{
	if ( NULL == i_MasterFISoc || this->GetMasterPlayerClientIdx() != i_MasterFISoc->m_character.ClientIndex )
		return;

	nUpdateScene = max ( nUpdateScene , 1 );

	vectorCinemaInfo::iterator itrCinema = m_CinemaInfoList.begin();

	int nExquteCnt = 0;

	while ( itrCinema != m_CinemaInfoList.end() )
	{
		CINEMAINFO *pCinemaInfo = &*itrCinema;

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

		if( itrCinema->CustomIdx )
		{
			switch( itrCinema->CustomIdx )
			{
			case INFINITY3_STEP5_MOVIE_START:
				{
					INIT_MSG_WITH_BUFFER(MSG_FC_INFINITY_MOVIE, T_FC_INFINITY_MOVIE, pMovieMsg, MovieBuf);
					pMovieMsg->UniqueNumber	= INFINITY_MOVIE_001_INF3_STEP5;
					pMovieMsg->Command		= MOVIE_COMMAND_START;
					
					this->SendAddDataPlayerList(SendBuf, MSG_SIZE(MSG_FC_INFINITY_MOVIE));
					
					break;
				}
				
			case INFINITY3_STEP5_MOVIE_END:
				{
					INIT_MSG_WITH_BUFFER(MSG_FC_INFINITY_MOVIE, T_FC_INFINITY_MOVIE, pMovieMsg, MovieBuf);
					pMovieMsg->UniqueNumber	= INFINITY_MOVIE_001_INF3_STEP5;
					pMovieMsg->Command		= MOVIE_COMMAND_END;
					
					this->SendAddDataPlayerList(SendBuf, MSG_SIZE(MSG_FC_INFINITY_MOVIE));
					
					break;
				}
				
				case INFINITY3_STEP5_TIMELIMIT_SET:
					{
						INIT_MSG_WITH_BUFFER(MSG_FC_INFINITY_SET_LIMITTIME, T_FC_INFINITY_SET_LIMITTIME, pLimitTimeMsg, LimitTimeBuf);
						pLimitTimeMsg->LimitTime = INFINITY3_STEP6_TIMELIMIT_VALUE;
						this->SendAddDataPlayerList(LimitTimeBuf, MSG_SIZE(MSG_FC_INFINITY_SET_LIMITTIME));

						m_BonusStepLimitTime = timeGetTime() + INFINITY3_STEP6_TIMELIMIT_VALUE;
						
				break;
					}

				case INFINITY3_STEP6_MOVE_SUCCESS_POSITION:
					{
						this->MoveAllPlayer(D3DXVECTOR3(INFINITY3_STEP6_MOVE_SUCCESS_POSTION_X, INFINITY3_STEP6_MOVE_SUCCESS_POSTION_Y, INFINITY3_STEP6_MOVE_SUCCESS_POSTION_Z));
						
						break;
					}

				case INFINITY3_STEP6_MOVE_FAIL_POSITION:
					{
						this->MoveAllPlayer(D3DXVECTOR3(INFINITY3_STEP6_MOVE_FAIL_POSTION_X, INFINITY3_STEP6_MOVE_FAIL_POSTION_Y, INFINITY3_STEP6_MOVE_FAIL_POSTION_Z));
						
						break;
					}

				case INFINITY3_FAIL_MOVIE_START:
					{
						INIT_MSG_WITH_BUFFER(MSG_FC_INFINITY_MOVIE, T_FC_INFINITY_MOVIE, pMovieMsg, MovieBuf);
						pMovieMsg->UniqueNumber	= INFINITY_MOVIE_002_INF3_FAIL;
						pMovieMsg->Command		= MOVIE_COMMAND_START;
						
						this->SendAddDataPlayerList(MovieBuf, MSG_SIZE(MSG_FC_INFINITY_MOVIE));
						
						break;
					}
				
			default:

				break;
			}
		}

		if ( pCinemaInfo->MonsterIdx )
		{
			// start 2011-04-28 by hskim, 인피니티 3차 - 시네마 관련 기능 추가 - 해당 맵채널 특정 몬스터 삭제/변경 기능 추가
			switch( itrCinema->MonsterIdxType )
			{
			case CINEMA_MONSTER_INDEX_REGEN_ON :
				{
					INIT_MSG_WITH_BUFFER(MSG_FN_NPCSERVER_CINEMA_MONSTER_REGEN, T_FN_NPCSERVER_CINEMA_MONSTER_REGEN, pRegen, SendBufRegen);
					pRegen->mapChann			= m_pInfinityMapChannel->GetMapChannelIndex();
					pRegen->iMonsterUnitKind	= itrCinema->MonsterIdx;
					pRegen->bRegen				= TRUE;
					
					bool bLimitBalance = false;
					
					MONSTER_INFO *pMonsterInfo = ms_pFieldIOCP4->GetMonsterInfo ( itrCinema->MonsterIdx );
					
					if ( ms_pFieldIOCP4 )
					{
						pMonsterInfo = ms_pFieldIOCP4->GetMonsterInfo ( itrCinema->MonsterIdx );
						
						if ( pMonsterInfo && pMonsterInfo->Belligerence == BELL_INFINITY_DEFENSE_MONSTER )
							bLimitBalance = true;
					}
					
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
							
							pRegen->MonsterBalanceInfo = sUpdateBalanceData;
						}
						else
						{
							pRegen->MonsterBalanceInfo.Init(0);
						}
					}
					else
					{
						pRegen->MonsterBalanceInfo = this->m_InfinityDifficultyData;
					}
					
					((CFieldIOCP*)(m_pInfinityMapChannel->m_pFieldMapWorkspace->m_pIOCPServer))->Send2NPCServerByTCP(SendBufRegen, MSG_SIZE(MSG_FN_NPCSERVER_CINEMA_MONSTER_REGEN));
					
					break;
				}
				
			case CINEMA_MONSTER_INDEX_REGEN_OFF :
				{
					INIT_MSG_WITH_BUFFER(MSG_FN_NPCSERVER_CINEMA_MONSTER_REGEN, T_FN_NPCSERVER_CINEMA_MONSTER_REGEN, pRegen, SendBufRegen);
					pRegen->mapChann			= m_pInfinityMapChannel->GetMapChannelIndex();
					pRegen->iMonsterUnitKind	= itrCinema->MonsterIdx;
					pRegen->bRegen				= FALSE;
					
					((CFieldIOCP*)(m_pInfinityMapChannel->m_pFieldMapWorkspace->m_pIOCPServer))->Send2NPCServerByTCP(SendBufRegen, MSG_SIZE(MSG_FN_NPCSERVER_CINEMA_MONSTER_REGEN));
					
					break;
				}

				case CINEMA_MONSTER_INDEX_DESTORY:
			{
				INIT_MSG_WITH_BUFFER(MSG_FN_NPCSERVER_CINEMA_MONSTER_DESTROY, T_FN_NPCSERVER_CINEMA_MONSTER_DESTROY, pDestroy, SendBufDestroy);
				pDestroy->mapChann			= m_pInfinityMapChannel->GetMapChannelIndex();
				pDestroy->MonsterUnitKind	= itrCinema->MonsterIdx;
				
				((CFieldIOCP*)(m_pInfinityMapChannel->m_pFieldMapWorkspace->m_pIOCPServer))->Send2NPCServerByTCP(SendBufDestroy, MSG_SIZE(MSG_FN_NPCSERVER_CINEMA_MONSTER_DESTROY));

					break;
				}

				case CINEMA_MONSTER_INDEX_CHANGE :
				{
					INIT_MSG_WITH_BUFFER(MSG_FN_NPCSERVER_CINEMA_MONSTER_CHANGE, T_FN_NPCSERVER_CINEMA_MONSTER_CHANGE, pChange, SendBufChange);
					pChange->mapChann			= m_pInfinityMapChannel->GetMapChannelIndex();
					pChange->MonsterUnitKind	= itrCinema->MonsterIdx;
					pChange->ChangeMonsterUnitKind	= itrCinema->UpdateMonsterIdx;
					
					((CFieldIOCP*)(m_pInfinityMapChannel->m_pFieldMapWorkspace->m_pIOCPServer))->Send2NPCServerByTCP(SendBufChange, MSG_SIZE(MSG_FN_NPCSERVER_CINEMA_MONSTER_CHANGE));
					
					break;
				}

				case CINEMA_MONSTER_INDEX_CREATE:
				default :
			{
				INIT_MSG_WITH_BUFFER(MSG_FN_NPCSERVER_CINEMA_MONSTER_CREATE, T_FN_NPCSERVER_CINEMA_MONSTER_CREATE, pSummon, SendBuf2);
				pSummon->mapChann			= m_pInfinityMapChannel->GetMapChannelIndex();
				pSummon->MonsterUnitKind	= pCinemaInfo->MonsterIdx;

				bool bDefaultBalance = false;
				bool bKey_BossMonster = false;
				
 				if ( ms_pFieldIOCP4 )
 				{
 					MONSTER_INFO *pMonsterInfo = ms_pFieldIOCP4->GetMonsterInfo ( itrCinema->MonsterIdx );

					if ( NULL != GetKeyMonsterInfoFromKeyMonsterDeadForNextStep( pMonsterInfo->MonsterUnitKind ) 
						|| COMPARE_MPOPTION_BIT(pMonsterInfo->MPOption, MPOPTION_BIT_KEY_MONSTER_ALIVE_FOR_GAMECLEAR) || itrCinema->KeyMonster > 0 )
					{
						bKey_BossMonster = true;
					}
 				}

				if ( bDefaultBalance || bKey_BossMonster ) pSummon->MonsterSummonCount = pCinemaInfo->SummonCount;
				else pSummon->MonsterSummonCount	= pCinemaInfo->SummonCount * ( this->m_InfinityDifficultyData.fSummonCountRatio <= 0.0f ? 1.0f : this->m_InfinityDifficultyData.fSummonCountRatio );

				pSummon->MonsterSummonCount = max ( pSummon->MonsterSummonCount , 1 );

				pSummon->MaxRandomDistance	= pCinemaInfo->SummonRandomPos;

				if ( bDefaultBalance ) pSummon->MonsterBalanceInfo.Init(0);
				else pSummon->MonsterBalanceInfo	= this->m_InfinityDifficultyData;
							
				ms_pFieldIOCP4->Send2NPCServerByTCP ( SendBuf2 , MSG_SIZE( MSG_FN_NPCSERVER_CINEMA_MONSTER_CREATE ) );

					break;
				}
				// end 2011-04-28 by hskim, 인피니티 3차 - 시네마 관련 기능 추가 - 해당 맵채널 특정 몬스터 삭제 기능 추가
			}
		}

		this->SetDeleteCinemaInfo( pCinemaInfo );

		CAtumLogSender::SendLogMessageInfinityCinema ( pCinemaInfo , this->GetInfinityCreateUID() );

		itrCinema = m_CinemaInfoList.erase( itrCinema );

		nExquteCnt++;

		if ( nExquteCnt >= nUpdateScene ) break;
	}
}
