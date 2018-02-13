// InfinityBase.cpp: implementation of the CInfinityBase class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "InfinityBase.h"
#include "FieldGlobal.h"
#include "FieldIOCPSocket.h"
#include "InfinityTickManager.h"
#include "FieldMapChannel.h"
#include "AtumLogSender.h"				// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 게임로그 추가
#include "FieldParty.h"		// 2013-05-16 by jhseol, 인피 재접속 시 파티지원


///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2010-04-09 by cmkwon, 인피2차 추가 수정(단계별 보상 추가) - 
/// \author		cmkwon
/// \date		2010-04-12 ~ 2010-04-12
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
CTenderItemInfo::CTenderItemInfo()
{
	m_ItemFieldIndex		= 0;
	//m_TenderStartTick		= 0;
	m_TenderStartTime.Reset();
	m_TenderItemNum			= 0;
	m_TenderItemCount		= 0;
	m_IsTimeOver			= FALSE;
	m_BidderList.clear();
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2010-04-09 by cmkwon, 인피2차 추가 수정(단계별 보상 추가) - 
/// \author		cmkwon
/// \date		2010-04-12 ~ 2010-04-12
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
CTenderItemInfo::CTenderItemInfo(const CTenderItemInfo & i_tenderingInfo)
{
	m_ItemFieldIndex		= i_tenderingInfo.m_ItemFieldIndex;
	//m_TenderStartTick		= i_tenderingInfo.m_TenderStartTick;
	m_TenderStartTime		= i_tenderingInfo.m_TenderStartTime;
	m_TenderItemPosition	= i_tenderingInfo.m_TenderItemPosition;
	m_TenderItemNum			= i_tenderingInfo.m_TenderItemNum;
	m_TenderItemCount		= i_tenderingInfo.m_TenderItemCount;
	m_IsTimeOver			= i_tenderingInfo.m_IsTimeOver;
	m_BidderList			= i_tenderingInfo.m_BidderList;
}

struct find_if_ClientIndex_IN_CFieldIOCPSocketPtr
{
	find_if_ClientIndex_IN_CFieldIOCPSocketPtr(int i_CliIdx): m_nFindIndex(i_CliIdx){};
	bool operator()(const CFieldIOCPSocket *i_pFISoc)
	{
		return i_pFISoc->m_character.ClientIndex == m_nFindIndex;
	}
	int m_nFindIndex;
};

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		cmkwon
/// \date		2010-04-12 ~ 2010-04-12
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CTenderItemInfo::CheckAllUserChoice(VectCFISoc *i_pUserList)
{
	int nChoicedUserCnt = 0;
	vectBIDDER_INFO::iterator itr(m_BidderList.begin());
	while(itr != m_BidderList.end())
	{
		BIDDER_INFO *pBidderInfo = &*itr;

		VectCFISoc::iterator itrCFISoc = find_if(i_pUserList->begin(), i_pUserList->end(), find_if_ClientIndex_IN_CFieldIOCPSocketPtr(pBidderInfo->UserClientIdx));
		if(itrCFISoc == i_pUserList->end())
		{
			// 사라진 유저
			itr = m_BidderList.erase(itr);
			continue;
		}

		nChoicedUserCnt++;
		itr++;
	}

	return nChoicedUserCnt == i_pUserList->size();
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2010-04-09 by cmkwon, 인피2차 추가 수정(단계별 보상 추가) - 
/// \author		cmkwon
/// \date		2010-04-12 ~ 2010-04-12
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BIDDER_INFO *CTenderItemInfo::GetBidderInfo(ClientIndex_t i_cliIdx)
{
	vectBIDDER_INFO::iterator itr(m_BidderList.begin());
	for(; itr != m_BidderList.end(); itr++)
	{
		BIDDER_INFO *pBidderInfo = &*itr;
		if(i_cliIdx == pBidderInfo->UserClientIdx)
		{
			return pBidderInfo;
		}		
	}
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2010-04-09 by cmkwon, 인피2차 추가 수정(단계별 보상 추가) - 
/// \author		cmkwon
/// \date		2010-04-12 ~ 2010-04-12
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
Err_t CTenderItemInfo::PlayBidding(DiceCnt_t *o_pDiceResult, ClientIndex_t i_cliIdx, BOOL i_bGiveUp)
{
	if(NULL != this->GetBidderInfo(i_cliIdx))
	{
		return ERR_INFINITY_TENDER_ALREADY_CHOICED;
	}

	*o_pDiceResult		= 0;	// 0이면 포기
	BIDDER_INFO bidderInfo;
	util::zero(&bidderInfo, sizeof(bidderInfo));
	bidderInfo.UserClientIdx	= i_cliIdx;
	bidderInfo.GiveUp			= i_bGiveUp;
	
	if(bidderInfo.GiveUp)
	{
		this->PushBidder(&bidderInfo);
		return ERR_NO_ERROR;
	}

	int nDiceValue	= RANDI(MIN_DICE_COUNT, MAX_DICE_COUNT);
	if(this->IsExistDiceValue(nDiceValue))
	{
		BOOL bValidValue	= FALSE;
		BOOL bIsPlus		= RANDI(0,1);
		if(bIsPlus)
		{
			for(int i=1; i <= MAX_DICE_COUNT; i++)
			{
				int nTmDiceVal = (nDiceValue + i)%MAX_DICE_COUNT;
				if(FALSE == this->IsExistDiceValue(nTmDiceVal))
				{
					nDiceValue	= nTmDiceVal;
					bValidValue	= TRUE;
					break;
				}
			}
		}
		else
		{
			for(int i=1; i <= MAX_DICE_COUNT; i++)
			{
				int nTmDiceVal = (nDiceValue + MAX_DICE_COUNT - i)%MAX_DICE_COUNT;
				if(FALSE == this->IsExistDiceValue(nTmDiceVal))
				{
					nDiceValue	= nTmDiceVal;
					bValidValue	= TRUE;
					break;
				}
			}
		}

		if(FALSE == bValidValue)
		{// 2010-04-12 by cmkwon, 더이상 유니크한 주사위 값이 없다. 포기 처리함.
			bidderInfo.GiveUp	= TRUE;
			this->PushBidder(&bidderInfo);
			return ERR_INFINITY_NOT_PROCESSING_TENDERITEM;
		}
	}

	*o_pDiceResult			= nDiceValue;
	bidderInfo.DiceCount	= nDiceValue;
	this->PushBidder(&bidderInfo);
	return ERR_NO_ERROR;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2010-04-09 by cmkwon, 인피2차 추가 수정(단계별 보상 추가) - 
/// \author		cmkwon
/// \date		2010-04-12 ~ 2010-04-12
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CTenderItemInfo::IsExistDiceValue(DiceCnt_t i_nDiceValue)
{
	vectBIDDER_INFO::iterator itr(m_BidderList.begin());
	for(; itr != m_BidderList.end(); itr++)
	{
		BIDDER_INFO *pBidderInfo = &*itr;
		if(FALSE == pBidderInfo->GiveUp
			&& i_nDiceValue == pBidderInfo->DiceCount)
		{
			return TRUE;
		}
	}

	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2010-04-09 by cmkwon, 인피2차 추가 수정(단계별 보상 추가) - 
/// \author		cmkwon
/// \date		2010-04-12 ~ 2010-04-12
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BIDDER_INFO *CTenderItemInfo::FindSuccessfulBidder(void)
{
	BIDDER_INFO *pMaxDiceValBidder = NULL;
	vectBIDDER_INFO::iterator itr(m_BidderList.begin());
	for(; itr != m_BidderList.end(); itr++)
	{
		BIDDER_INFO *pBidderInfo = &*itr;
		if(FALSE == pBidderInfo->GiveUp)
		{
			if(NULL == pMaxDiceValBidder
				|| pMaxDiceValBidder->DiceCount < pBidderInfo->DiceCount)
			{
				pMaxDiceValBidder = pBidderInfo;
			}
		}
	}
	
	return pMaxDiceValBidder;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2010-04-09 by cmkwon, 인피2차 추가 수정(단계별 보상 추가) - 
/// \author		cmkwon
/// \date		2010-04-14 ~ 2010-04-14
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CTenderItemInfo::PushBidder(BIDDER_INFO *i_pBidderInfo)
{
	if(NULL != this->GetBidderInfo(i_pBidderInfo->UserClientIdx))
	{
		return FALSE;
	}

	m_BidderList.push_back(*i_pBidderInfo);
	return TRUE;
}

CFieldIOCP *CInfinityBase::ms_pFieldIOCP4	= NULL;		// 2010-03-26 by cmkwon, 인피2차 수정 - CFieldIOCP 관리구조 추가

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CInfinityBase::CInfinityBase()
{
	InitializeCriticalSection(&m_criticalLimitTime);	// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 사망시 패널티 추가
	this->InitInfinityBase();
}

CInfinityBase::~CInfinityBase()
{
	DeleteCriticalSection(&m_criticalLimitTime);		// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 사망시 패널티 추가
}

void CInfinityBase::InitInfinityBase() 
{
	m_mtVectPlayer.clear();
	m_pInfinityMapChannel = NULL;
	m_CinemaOrder = 0;
	util::zero(&m_InfinityModeInfo, sizeof(INFINITY_MODEINFO));
	this->SetInfinityState(INFINITY_STATE_UNPREPARED);
	m_KeyMonsterDeadForNextStepList.clear();			// 2010-03-31 by dhjin, 인피니티(기지방어) - 이름변경 m_KeyMonsterList -> m_KeyMonsterDeadForNextStepList
	util::zero(&m_KeyMonsterAliveForGameClear, sizeof(KEY_MONSTER_INFO));			// 2010-03-31 by dhjin, 인피니티(기지방어) - 해당 몬스터가 다 죽으면 게임 오버~
	m_mtTenderItemList.clearLock();
	m_DeleteCinemaInfoList.clear();		// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 팅긴 유저 재접속 처리
	m_FinSuccessCinemaInfoList.clear(); // 2010-03-31 by dhjin, 인피니티(기지방어) -
	m_FinFailCinemaInfoList.clear();	// 2010-03-31 by dhjin, 인피니티(기지방어) -
	m_InfinityDifficultyLevel = DEFAULT_LEVEL_INFINITY_DIFFICULTY;	// 2010. 05. 19 by hsLee 인피니티 필드 2차 난이도 조절. (신호처리 + 몬스터 처리(서버) ) - 방의 난이도 레벨값.
	m_InfinityDifficultyData.Init( m_InfinityDifficultyLevel );
	ResetValue_SkipEndingCinema();									// 2010. 07. 27 by hsLee 인피니티 2차 거점 방어 시네마 연출 스킵 처리.

	// start 2011-04-11 by hskim, 인피니티 3차 - 시네마 단계별 기능 구현
	for(int a=0; a<CINEMA_PLAYING_STEP_MAX; a++) m_CinemaStepInfoList[a].clear();

	m_PlayingStepCount = 0;
	m_InfinityStepStartTick = 0;
	// end 2011-04-11 by hskim, 인피니티 3차 - 시네마 단계별 기능 구현

	m_bPenaltyApply = TRUE;		// 2011-05-30 by hskim, 인피니티 3차 - 플레이 시간 재설정 기능
}

void CInfinityBase::DeleteNullFISocPtrNoLock()
{
	// CFieldIOCPSocket NULL 포인터를 삭제한다. 단 Lock을 잡지 않으므로 함수 호출 상위에서 잡아야한다.!
	mtVectCFISoc::iterator itr = m_mtVectPlayer.begin();
	while(itr != m_mtVectPlayer.end())
	{
		CFieldIOCPSocket *pFISoc = *itr;
		if(NULL == pFISoc
			|| FALSE == pFISoc->IsUsing()
			|| INFINITY_STATE_NONE == pFISoc->m_InfinityPlayingInfo.InfinityState)
		{
			itr = m_mtVectPlayer.erase(itr);
			continue;
		}
		itr++;
	}
}

void CInfinityBase::SetInfinityCreateUID(InfinityCreateUID_t i_nInfinityCreateUID) {
	m_nInfinityCreateUID = i_nInfinityCreateUID;
}


void CInfinityBase::InitMasterPlayer(CFieldIOCPSocket * i_pMasterPlayerSoc) 
{
	if(NULL != i_pMasterPlayerSoc) {
		m_MasterPlayerSoc = i_pMasterPlayerSoc;
		// 좀 더 빠른 처리를 위해 몇 가지 정보를 저장한다.
		m_MasterPlayerSoc->m_InfinityPlayingInfo.InfinityCreateUID	= m_nInfinityCreateUID;
		m_MasterPlayerSoc->m_InfinityPlayingInfo.InfinityModeUID	= m_InfinityModeInfo.InfinityModeUID;
		m_MasterPlayerSoc->m_InfinityPlayingInfo.ModeTypeNum		= m_InfinityModeInfo.ModeTypeNum;
		m_MasterPlayerSoc->m_InfinityPlayingInfo.MapIdx				= m_InfinityModeInfo.MapIdx;
		m_MasterPlayerSoc->m_InfinityPlayingInfo.InfinityState		= INFINITY_STATE_READY;
		m_MasterPlayerSoc->m_InfinityPlayingInfo.TimePenaltyValue	= m_InfinityModeInfo.TimePenaltyValue;		// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 사망시 패널티 추가
		m_MasterPlayerSoc->m_InfinityPlayingInfo.HPPenaltyValue		= m_InfinityModeInfo.HPPenaltyValue;		// 2011-06-14 by hskim, 인피니티 3차 - 패널티 기능 추가 (HP 및 시간 동시 지원을 위해)
		m_mtVectPlayer.push_back(m_MasterPlayerSoc);
		m_InfluenceType = m_MasterPlayerSoc->m_character.InfluenceType;

		// 2010. 07. 27 by hsLee 인피니티 2차 거점 방어 시네마 연출 스킵 처리.
		ResetValue_SkipEndingCinema();

		g_pFieldGlobal->WriteSystemLogEX(TRUE, "[Notify] [Infinity][%I64d] CInfinityBase::InitMasterPlayer# %s \r\n", m_nInfinityCreateUID, GetCharacterString(m_MasterPlayerSoc->GetCharacter(), string()));
	}
}

void CInfinityBase::SetModeInfo(INFINITY_MODEINFO * i_pInfinityModeInfo) {
	if(NULL == i_pInfinityModeInfo) {
		// 2009-09-09 ~ 2010-01 by dhjin, 인피니티 - 소스 체크
		return;
	}

	m_InfinityModeInfo = *i_pInfinityModeInfo;
}

struct SSort_CINEMA_INFO_BY_STARTTIME: binary_function<CINEMAINFO, CINEMAINFO, bool>
{
	bool operator()(CINEMAINFO param1, CINEMAINFO param2)
	{
        return param1.StartTime < param2.StartTime;		// 2010-03-31 by dhjin, 인피니티(기지방어) -  // 오름 차순 정렬을 위해 반대로 함
    };
};


void CInfinityBase::SetCinemaInfo(vectorCinemaInfo * i_pVectCinemaInfo)
{
	if(NULL == i_pVectCinemaInfo)
	{
		// 2009-09-09 ~ 2010-01 by dhjin, 인피니티 - 소스 체크
		return;
	}

	// 2010-03-31 by dhjin, 인피니티(기지방어) -
    // 진행 시네마 설정
	vectorCinemaInfo::iterator itrCinema = i_pVectCinemaInfo->begin();
	// 초기 시네마 단계 설정
	m_CinemaOrder	= itrCinema->CinemaOrder;
	for(; itrCinema != i_pVectCinemaInfo->end(); itrCinema++) 
	{
		// start 2011-04-11 by hskim, 인피니티 3차 - 시네마 단계별 기능 구현
		//////////
		// 기존 //
		//if(CINEMA_FIN_SUCCESS == itrCinema->CinemaOrder) 
		//{	// 성공 종료 시네마 설정
		//	m_FinSuccessCinemaInfoList.push_back(*itrCinema);
		//}
		//else if(CINEMA_FIN_FAIL == itrCinema->CinemaOrder)
		//{	// 실패 종료 시네마 설정
		//	m_FinFailCinemaInfoList.push_back(*itrCinema);
		//}
		//else
		//{
		//	m_CinemaInfoList.push_back(*itrCinema);
		//}

		//////////
		// 수정 //
		switch( itrCinema->CinemaOrder )
		{
		case CINEMA_FIN_SUCCESS :

			m_FinSuccessCinemaInfoList.push_back(*itrCinema);
			break;

		case CINEMA_FIN_FAIL :

			m_FinFailCinemaInfoList.push_back(*itrCinema);
			break;

		default:

			bool bBreak = FALSE;

			for(int a=0; a<CINEMA_PLAYING_STEP_MAX; a++)
			{
				if( (CINEMA_PLAYING_STEP_START_NUMBER + a) == itrCinema->CinemaOrder )
				{
					m_CinemaStepInfoList[a].push_back(*itrCinema);

					bBreak = TRUE;
					break;
				}
			}

			if( TRUE == bBreak) break;

			m_CinemaInfoList.push_back(*itrCinema);
		}
		// end 2011-04-11 by hskim, 인피니티 3차 - 시네마 단계별 기능 구현
	}

	// 시간순으로 정렬
	sort(m_CinemaInfoList.begin(), m_CinemaInfoList.end(), SSort_CINEMA_INFO_BY_STARTTIME());
	sort(m_FinSuccessCinemaInfoList.begin(), m_FinSuccessCinemaInfoList.end(), SSort_CINEMA_INFO_BY_STARTTIME());
	sort(m_FinFailCinemaInfoList.begin(), m_FinFailCinemaInfoList.end(), SSort_CINEMA_INFO_BY_STARTTIME());

	// start 2011-04-11 by hskim, 인피니티 3차 - 시네마 단계별 기능 구현
	for(int a=0; a<CINEMA_PLAYING_STEP_MAX; a++)
	{
		if( FALSE == m_CinemaStepInfoList[a].empty() ) sort(m_CinemaStepInfoList[a].begin(), m_CinemaStepInfoList[a].end(), SSort_CINEMA_INFO_BY_STARTTIME());
	}
	// end 2011-04-11 by hskim, 인피니티 3차 - 시네마 단계별 기능 구현

	// 키 몬스터 설정
	this->SetKeyMonsterList();		
}

void CInfinityBase::SetKeyMonsterList()
{
	vectorCinemaInfo::iterator itr = m_CinemaInfoList.begin();
	for(; itr != m_CinemaInfoList.end(); itr++)
	{
		// 2010-03-26 by cmkwon, 인피2차 수정 - 
		CINEMAINFO *pCinemaInfo = &*itr;

//		g_pFieldGlobal->WriteSystemLogEX(TRUE, "[TEMP] [Infinity][%I64d] CInfinityBase::SetKeyMonsterList# 10000 ! Cinema<Num(%6d) Order(%d) StartTime(%6d) m_CinemaOrder(%d) \r\n"
//			, m_nInfinityCreateUID, pCinemaInfo->CinemaNum, pCinemaInfo->CinemaOrder, pCinemaInfo->StartTime, m_CinemaOrder);

		if(0 == pCinemaInfo->MonsterIdx) 
		{
			continue;
		}

		// start 2011-05-31 by hskim, 인피니티 3차 - 시네마 단계별 기능 구현
		if( CINEMA_MONSTER_INDEX_CREATE != itr->MonsterIdxType )
		{
			continue;
		}
		// start 2011-05-31 by hskim, 인피니티 3차 - 시네마 단계별 기능 구현

		MONSTER_INFO *pKeyMonInfo = ms_pFieldIOCP4->GetMonsterInfo(pCinemaInfo->MonsterIdx);
		if(NULL == pKeyMonInfo)
		{
			g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ERROR] [Infinity][%I64d] CInfinityBase::SetKeyMonsterList# Invalid MonsterIdx !! ModeUID(%4d) ModeType(%4d) MonIdx(%d) CinemaOrder(%d) \r\n"
				, this->m_nInfinityCreateUID, m_InfinityModeInfo.InfinityModeUID, m_InfinityModeInfo.ModeTypeNum, pCinemaInfo->MonsterIdx, pCinemaInfo->CinemaOrder);
			continue;
		}

		KEY_MONSTER_INFO KeyMonsterInfo;
		util::zero(&KeyMonsterInfo, sizeof(KEY_MONSTER_INFO));
		KeyMonsterInfo.KeyMonsterAlive	= FALSE;
		KeyMonsterInfo.KeyMonsterIdx	= pCinemaInfo->MonsterIdx;
		KeyMonsterInfo.KeyMonsterCount	= pCinemaInfo->SummonCount;		// 2010-03-31 by dhjin, 인피니티(기지방어) -
		KeyMonsterInfo.KeyMonsterOrder	= pCinemaInfo->CinemaOrder;		// 2010-03-31 by dhjin, 인피니티(기지방어) -

		if(COMPARE_MPOPTION_BIT(pKeyMonInfo->MPOption, MPOPTION_BIT_KEY_MONSTER))
		{ 
			// 2010-04-09 by cmkwon, 인피2차 추가 수정(단계별 보상 추가) - 
			//this->SetKeyMonster_DeadForNextStep(&KeyMonsterInfo);
			///////////////////////////////////////////////////////////////////////////////
			// 2010-04-09 by cmkwon, 인피2차 추가 수정(단계별 보상 추가) - 

			// start 2011-04-19 by hskim, 인피니티 3차 - 시네마 단계별 기능 구현
			if( pCinemaInfo->KeyMonster != 0 )
			{
				// MAP에서 각기 다른 좌표를 가진 --> 같은 Idx 사용하는 KEY 몬스터의 --> 전체 갯수 반영
				// MAP에 같은 Idx 가지는 몬스터를 여러 좌표에 찍었을 경우 SummonCount를 1로 설정하면 모든 몬스터(맵에 찍힌)가 생성이 됨
				// 여기서 KEY 몬스터로 생성할경우 소환되는 몬스터와 SummonCount 몬스터의 값이 틀려지게 됨
				// 그래서 기존에 사용하지 않는 KeyMonster 가지고 전체 맵에 찍힌 KeyMonster의 갯수로 사용하도록 수정
				KeyMonsterInfo.KeyMonsterCount = KeyMonsterInfo.KeyMonsterCount * pCinemaInfo->KeyMonster;
			}
			// end 2011-04-019 by hskim, 인피니티 3차 - 시네마 단계별 기능 구현

			if(FALSE == this->SetKeyMonster_DeadForNextStep(&KeyMonsterInfo))
			{
				g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ERROR] [Infinity][%I64d] CInfinityBase::SetKeyMonsterList# duplicate key Monster !! ModeUID(%4d) ModeType(%4d) MonIdx(%d) CinemaOrder(%d) \r\n"
					, this->m_nInfinityCreateUID, m_InfinityModeInfo.InfinityModeUID, m_InfinityModeInfo.ModeTypeNum, pCinemaInfo->MonsterIdx, pCinemaInfo->CinemaOrder);
				continue;
			}
		}
		else if(COMPARE_MPOPTION_BIT(pKeyMonInfo->MPOption, MPOPTION_BIT_KEY_MONSTER_ALIVE_FOR_GAMECLEAR)) 
		{
			this->SetKeyMonster_AliveForGameClear(&KeyMonsterInfo);
		}
	}
}

// 2010-04-09 by cmkwon, 인피2차 추가 수정(단계별 보상 추가) - 
BOOL CInfinityBase::SetKeyMonster_DeadForNextStep(KEY_MONSTER_INFO * i_pKeyMonsterInfo)
{	// 2010-03-31 by dhjin, 인피니티(기지방어) -
// 2010-04-09 by cmkwon, 인피2차 추가 수정(단계별 보상 추가) - 키몬스터로 설정된 것은 1개만 설정 할수 있다.
// 	VectKeyMonsterInfo::iterator itr = m_KeyMonsterDeadForNextStepList.begin();
// 	for(; itr != m_KeyMonsterDeadForNextStepList.end(); itr++) 
// 	{	
// 		if(itr->KeyMonsterIdx == i_pKeyMonsterInfo->KeyMonsterIdx
// 			&& itr->KeyMonsterOrder == i_pKeyMonsterInfo->KeyMonsterOrder) 
// 		{	// 이미 존재하면 카운트만 증가 시킨다.
// 			itr->KeyMonsterCount += i_pKeyMonsterInfo->KeyMonsterCount;
// 			return;
// 		}
// 	}
	///////////////////////////////////////////////////////////////////////////////
	// 2010-04-09 by cmkwon, 인피2차 추가 수정(단계별 보상 추가) - 
	if(NULL != GetKeyMonsterInfoFromKeyMonsterDeadForNextStep(i_pKeyMonsterInfo->KeyMonsterIdx))
	{
		return FALSE;
	}

//	g_pFieldGlobal->WriteSystemLogEX(TRUE, "[TEMP] [Infinity][%I64d] CInfinityBase::SetKeyMonster_DeadForNextStep# MapIdx(%d) KeyMonNum(%d) MonCount(%3d) CinemaOrder(%d) \r\n"
//		, this->m_nInfinityCreateUID, this->m_InfinityModeInfo.MapIdx, i_pKeyMonsterInfo->KeyMonsterIdx, i_pKeyMonsterInfo->KeyMonsterCount, i_pKeyMonsterInfo->KeyMonsterOrder);
	
	m_KeyMonsterDeadForNextStepList.push_back(*i_pKeyMonsterInfo);
	return TRUE;
}

void CInfinityBase::SetKeyMonster_AliveForGameClear(KEY_MONSTER_INFO * i_pKeyMonsterInfo)
{	// 2010-03-31 by dhjin, 인피니티(기지방어) -
	m_KeyMonsterAliveForGameClear = *i_pKeyMonsterInfo;

	// 2010-03-26 by cmkwon, 인피니티 시스템 로그 추가 - 
	g_pFieldGlobal->WriteSystemLogEX(TRUE, "[Notify] [Infinity][%I64d] Set Key Monster ForGameClear !! ModeUID(%4d) ModeType(%4d) MonsterUnitkind(%d) Order(%d)\r\n"
		, this->m_nInfinityCreateUID, m_InfinityModeInfo.InfinityModeUID, m_InfinityModeInfo.ModeTypeNum, i_pKeyMonsterInfo->KeyMonsterIdx, i_pKeyMonsterInfo->KeyMonsterOrder);
}

void CInfinityBase::CreateKeyMonster_DeadForNextStep(MonIdx_t i_CreateMonsterIdx) {
	VectKeyMonsterInfo::iterator itr = m_KeyMonsterDeadForNextStepList.begin();
	for(; itr != m_KeyMonsterDeadForNextStepList.end(); itr++) {
		if(i_CreateMonsterIdx == itr->KeyMonsterIdx
			&& FALSE == itr->KeyMonsterAlive
			&& m_CinemaOrder == itr->KeyMonsterOrder	// 2010-03-31 by dhjin, 인피니티(기지방어) -
			)
		{
			itr->KeyMonsterAlive = TRUE;
			return;
		}
	}
}

void CInfinityBase::CreateKeyMonster_AliveForGameClear(MonIdx_t i_CreateMonsterIdx) 
{	// 2010-03-31 by dhjin, 인피니티(기지방어) -
	if(m_KeyMonsterAliveForGameClear.KeyMonsterIdx == i_CreateMonsterIdx) 
	{
		m_KeyMonsterAliveForGameClear.KeyMonsterAlive = TRUE;
	}
}

// 2010-04-09 by cmkwon, 인피2차 추가 수정(단계별 보상 추가) - 리턴값은 보상 처리 여부이다. TRUE이면 해당 키몬스터의 보상처리 해야함.
void CInfinityBase::DeleteKeyMonster_DeadForNextStep(BOOL *o_pbCompensationFlag, MonIdx_t i_DeadMonsterIdx)
{
// 2010-04-09 by cmkwon, 인피2차 추가 수정(단계별 보상 추가) - 
// 	VectKeyMonsterInfo::iterator itr = m_KeyMonsterDeadForNextStepList.begin();
// 	for(; itr != m_KeyMonsterDeadForNextStepList.end(); itr++) {
// 		if(i_DeadMonsterIdx == itr->KeyMonsterIdx
// 			&& TRUE == itr->KeyMonsterAlive
// 			&& m_CinemaOrder == itr->KeyMonsterOrder		// 2010-03-31 by dhjin, 인피니티(기지방어) -
// 			) 
// 		{
// 			itr->KeyMonsterCount--;
// 			if(0 >= itr->KeyMonsterCount) 
// 			{
// 				m_KeyMonsterDeadForNextStepList.erase(itr);
// 			}
// 			return;
// 		}
// 	}
	///////////////////////////////////////////////////////////////////////////////
	// 2010-04-09 by cmkwon, 인피2차 추가 수정(단계별 보상 추가) - 
	KEY_MONSTER_INFO *pKeyMonInfo = this->GetKeyMonsterInfoFromKeyMonsterDeadForNextStep(i_DeadMonsterIdx);
	if(NULL == pKeyMonInfo)
	{
		return;
	}

	pKeyMonInfo->KeyMonsterCount--;

	if(0 < pKeyMonInfo->KeyMonsterCount)
	{
		return;
	}

	// 2010-04-09 by cmkwon, 해당 키몬스터의 정보를 삭제한다.
	DeleteKeyMonsterInfoFromKeyMonsterDeadForNextStep(i_DeadMonsterIdx);
	*o_pbCompensationFlag	= TRUE;	// 보상 처리
}

void CInfinityBase::DeleteKeyMonster_AliveForGameClear(MonIdx_t i_DeadMonsterIdx) 
{	// 2010-03-31 by dhjin, 인피니티(기지방어) -
	if(m_KeyMonsterAliveForGameClear.KeyMonsterIdx == i_DeadMonsterIdx) 
	{
		util::zero(&m_KeyMonsterAliveForGameClear, sizeof(KEY_MONSTER_INFO));
	}
}

BOOL CInfinityBase::CheckEmptyMonster_DeadForNextStepAlive() {
	if(0 >= m_KeyMonsterDeadForNextStepList.size()) {
		return TRUE;
	}
	
	return FALSE;
}

BOOL CInfinityBase::CheckEmptyMonster_AliveForGameClearAlive() 
{	// 2010-03-31 by dhjin, 인피니티(기지방어) -
	if(0 != m_KeyMonsterAliveForGameClear.KeyMonsterIdx) 
	{
		return TRUE;
	}

	return FALSE;
}

BOOL CInfinityBase::CheckKeyMonster_DeadForNextStepAlive() {
	// 살아있는 키몬스터가 있는지 체크한다.	
	VectKeyMonsterInfo::iterator itr = m_KeyMonsterDeadForNextStepList.begin();
	for(; itr != m_KeyMonsterDeadForNextStepList.end(); itr++) {
		if(TRUE == itr->KeyMonsterAlive) {
			return TRUE;
		}
	}
	
	return FALSE;
}

BOOL CInfinityBase::CheckKeyMonster_AliveForGameClearAlive() 
{	// 2010-03-31 by dhjin, 인피니티(기지방어) -
	if(TRUE == m_KeyMonsterAliveForGameClear.KeyMonsterAlive) 
	{
		return TRUE;
	}
	
	return FALSE;
}

void CInfinityBase::SetInfinityTeamName(CHAR * i_pTeamName) {
	if(NULL == i_pTeamName) {
		// 2009-09-09 ~ 2010-01 by dhjin, 인피니티 - 소스 체크
		return;
	}

	util::strncpy(m_szTeamName, i_pTeamName, SIZE_MAX_PARTY_NAME);
}

// 2010-04-06 by cmkwon, 인피2차 추가 수정 - 
void CInfinityBase::GetPlayerList(VectCFISoc * o_pVectPlayer, ClientIndex_t * o_pMasterUserClientIdx/*=NULL*/)
{
	mt_auto_lock mta(&m_mtVectPlayer);

	o_pVectPlayer->assign(m_mtVectPlayer.begin(), m_mtVectPlayer.end());

	if(o_pMasterUserClientIdx)
	{// 2010-04-06 by cmkwon, 인피2차 추가 수정 - 
		*o_pMasterUserClientIdx = this->GetMasterPlayerClientIdx();
	}
}

INT CInfinityBase::GetPlayerListSize() {
	return m_mtVectPlayer.size();
}

INT CInfinityBase::GetMaxPlayerSize() {
	return m_InfinityModeInfo.MaxAdmissionCount;
}

CFieldIOCPSocket * CInfinityBase::GetMasterPlayer() {
	return m_MasterPlayerSoc;
}

ClientIndex_t CInfinityBase::GetMasterPlayerClientIdx() {
	if(NULL != m_MasterPlayerSoc){
		return m_MasterPlayerSoc->m_character.ClientIndex;
	}

	return FALSE;
}

CHAR * CInfinityBase::GetMasterPlayerName() {
	if(NULL != m_MasterPlayerSoc){
		return m_MasterPlayerSoc->m_character.CharacterName;
	}
	
	return NULL;
}

InfinityCreateUID_t CInfinityBase::GetInfinityCreateUID() {
	return m_nInfinityCreateUID;
}

eINFINITY_MODE CInfinityBase::GetInfinityMode() {
	return this->m_InfinityModeInfo.ModeTypeNum;
}

CHAR * CInfinityBase::GetInfinityTeamName()
{
	if(NULL != m_szTeamName){
		return m_szTeamName;
	}
	
	return NULL;
}

eINFINITY_STATE CInfinityBase::GetInfinityState() {
	return m_eInfinityState;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2010-04-05 by cmkwon, 인피 재입장 카드 관련 시스템 수정 - 
/// \author		cmkwon
/// \date		2010-04-06 ~ 2010-04-06
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CInfinityBase::SetInfinityState(eINFINITY_STATE i_infiState)
{
//	g_pFieldGlobal->WriteSystemLogEX(TRUE, "[TEMP] [Infinity][%I64d] CInfinityBase::SetInfinityState# !! State(%d <== %d) \r\n"
//		, this->m_nInfinityCreateUID, i_infiState, m_eInfinityState);

	m_eInfinityState = i_infiState;

	m_dwTickChangedInfinityState	= timeGetTime();

}

// start 2011-04-08 by hskim, 인피니티 3차 - 시네마 단계별 기능 구현
eINFINITY_STATE CInfinityBase::GetInfinityStatePlayingNext(eINFINITY_STATE Now)
{
	if( FALSE == IS_INFINITY_STATE_PLAYING(Now) ) return Now;
	
	if( INFINITY_STATE_PLAYING == Now )	return INFINITY_STATE_PLAYING_STEP1;
	if( TRUE == IS_INFINITY_STATE_PLAYING_LAST(Now) ) return Now;
	
	return (eINFINITY_STATE)(Now + 1);
}
// end 2011-04-08 by hskim, 인피니티 3차 - 시네마 단계별 기능 구현

// start 2011-06-01 인피니티 3차 스텝 7 - 캐릭터 위치 이동
void CInfinityBase::MoveAllPlayer(D3DXVECTOR3 vecPostion)
{
	mt_auto_lock mta(&m_mtVectPlayer);
	this->DeleteNullFISocPtrNoLock();
	
	vectCFieldIOCPSocket tmPlayerList;
	this->GetPlayerList(&tmPlayerList);
	mta.auto_unlock_cancel();

	vectCFieldIOCPSocket::iterator itr(tmPlayerList.begin());
	for(; itr != tmPlayerList.end(); itr++)
	{
		CFieldIOCPSocket *pFISoc = *itr;
		if(NULL == pFISoc
			|| FALSE == pFISoc->IsValidCharacter(FALSE))
		{
			continue;
		}
		pFISoc->WarpToMap(m_pInfinityMapChannel->GetMapChannelIndex(), &vecPostion);
	}
}
// end 2011-06-01 인피니티 3차 스텝 7 - 캐릭터 위치 이동

// 2012-07-17 by hskim, 인피니티 인원수 부족할대 START 할경우 예외 처리
BOOL CInfinityBase::CheckInfinityMinAdmission()
{
	mt_auto_lock mta(&m_mtVectPlayer);
	this->DeleteNullFISocPtrNoLock();

	if(this->m_InfinityModeInfo.MinAdmissionCount > this->GetPlayerListSize())
	{
		return FALSE;
	}
	
	return TRUE;
}
// end 2012-07-17 by hskim, 인피니티 인원수 부족할대 START 할경우 예외 처리

BYTE CInfinityBase::GetInfluenceType() {
	return m_InfluenceType;
}

MapIndex_t CInfinityBase::GetInfinityMapIndex() {
	return m_InfinityModeInfo.MapIdx;
}

ChannelIndex_t CInfinityBase::GetInfinityChannelIndex() {
	if(NULL == m_pInfinityMapChannel) {
		return -1;
	}
	return m_pInfinityMapChannel->GetMapChannelIndex().ChannelIndex;
}

Err_t CInfinityBase::JoinCheck(CFieldIOCPSocket * i_pJoinRequestPlayerSoc) {
	// 가입 가능한 유저인지 체크
	if(NULL == i_pJoinRequestPlayerSoc){
		return ERR_INFINITY_JOIN_FAIL_SOCKET;
	}	

	if(m_InfinityModeInfo.MaxAdmissionCount <= m_mtVectPlayer.size()) {
		// 인피 최고 인원수 보다 크다.
		return ERR_INFINITY_MAX_ADMISSIONCNT;
	}

	if(m_InfinityModeInfo.MaxLv < i_pJoinRequestPlayerSoc->m_character.Level 
		|| m_InfinityModeInfo.MinLv > i_pJoinRequestPlayerSoc->m_character.Level) {
		// 레벨이 맞지 않다.
		return ERR_INFINITY_MISMATCH_LEVEL;
	}

	if(INFINITY_STATE_UNPREPARED !=	m_eInfinityState) {
		return ERR_INFINITY_STATE;
	}

// 2010-03-23 by cmkwon, 인피니티 입장 캐쉬 아이템 구현 - 아래와 같이 수정
// 	if(0 >= i_pJoinRequestPlayerSoc->m_mtVecInfiImpute.size()) {
// 		if(NULL == m_MasterPlayerSoc) {
// 			return ERR_INFINITY_NO_SUCH_MASTER_USER;
// 		}
// 		INIT_MSG_WITH_BUFFER(MSG_FC_INFINITY_JOIN_REQUEST_MASTERUSER, T_FC_INFINITY_JOIN_REQUEST_MASTERUSER, pSMsg, SendBuf);
// 		pSMsg->ReQuestClientIdx		= i_pJoinRequestPlayerSoc->m_character.ClientIndex;
// 		util::strncpy(pSMsg->ReQuestCharacterName, i_pJoinRequestPlayerSoc->m_character.CharacterName, SIZE_MAX_CHARACTER_NAME);
// 		pSMsg->Gear					= i_pJoinRequestPlayerSoc->m_character.UnitKind;
// 		pSMsg->Lv					= i_pJoinRequestPlayerSoc->m_character.Level;
// 		m_MasterPlayerSoc->SendAddData(SendBuf, MSG_SIZE(MSG_FC_INFINITY_JOIN_REQUEST_MASTERUSER));
// 		return ERR_NO_ERROR;		
// 	}
//	else {
// 		mt_auto_lock mtImpute(&i_pJoinRequestPlayerSoc->m_mtVecInfiImpute);
// 		mt_vectorInfinityImpute::iterator itr = i_pJoinRequestPlayerSoc->m_mtVecInfiImpute.begin();
// 		for(; itr != i_pJoinRequestPlayerSoc->m_mtVecInfiImpute.end(); itr++) {
// 			// 진입 체크를 위한 값
// 			if(itr->InfinityModeUID == m_InfinityModeInfo.InfinityModeUID) {
// 				if(itr->EntranceCount >= m_InfinityModeInfo.EntranceCount) {
// 					return ERR_INFINITY_OVER_ENTRANCECOUNT;
// 				}
// 				else {
// 					// 인피 마스터에게 진입 요청 
// 					if(NULL == m_MasterPlayerSoc) {
// 						return ERR_INFINITY_NO_SUCH_MASTER_USER;
// 					}
// 					INIT_MSG_WITH_BUFFER(MSG_FC_INFINITY_JOIN_REQUEST_MASTERUSER, T_FC_INFINITY_JOIN_REQUEST_MASTERUSER, pSMsg, SendBuf);
// 					pSMsg->ReQuestClientIdx		= i_pJoinRequestPlayerSoc->m_character.ClientIndex;
// 					util::strncpy(pSMsg->ReQuestCharacterName, i_pJoinRequestPlayerSoc->m_character.CharacterName, SIZE_MAX_CHARACTER_NAME);
// 					pSMsg->Gear					= i_pJoinRequestPlayerSoc->m_character.UnitKind;
// 					pSMsg->Lv					= i_pJoinRequestPlayerSoc->m_character.Level;
// 					m_MasterPlayerSoc->SendAddData(SendBuf, MSG_SIZE(MSG_FC_INFINITY_JOIN_REQUEST_MASTERUSER));
// 					return ERR_NO_ERROR;
// 				}
// 			}
// 		}
//	}
	///////////////////////////////////////////////////////////////////////////////
	// 2010-03-23 by cmkwon, 인피니티 입장 캐쉬 아이템 구현 - CFieldIOCPSocket::Process_FC_INFINITY_JOIN# 입장카운트 체크
	// 인피 마스터에게 진입 요청 
	if(NULL == m_MasterPlayerSoc)
	{
		return ERR_INFINITY_NO_SUCH_MASTER_USER;
	}
	INIT_MSG_WITH_BUFFER(MSG_FC_INFINITY_JOIN_REQUEST_MASTERUSER, T_FC_INFINITY_JOIN_REQUEST_MASTERUSER, pSMsg, SendBuf);
	pSMsg->ReQuestClientIdx		= i_pJoinRequestPlayerSoc->m_character.ClientIndex;
	util::strncpy(pSMsg->ReQuestCharacterName, i_pJoinRequestPlayerSoc->m_character.CharacterName, SIZE_MAX_CHARACTER_NAME);
	pSMsg->Gear					= i_pJoinRequestPlayerSoc->m_character.UnitKind;
	pSMsg->Lv					= i_pJoinRequestPlayerSoc->m_character.Level;
	m_MasterPlayerSoc->SendAddData(SendBuf, MSG_SIZE(MSG_FC_INFINITY_JOIN_REQUEST_MASTERUSER));
	return ERR_NO_ERROR;
}

Err_t CInfinityBase::Join(CFieldIOCPSocket * i_pJoinPlayerSoc) {
	// 유저 가입 
	if(NULL == i_pJoinPlayerSoc){
		return ERR_INFINITY_JOIN_FAIL_SOCKET;
	}	
	
	if(m_InfinityModeInfo.MaxAdmissionCount <= m_mtVectPlayer.size()) {
		// 인피 최고 인원수 보다 크다.
		return ERR_INFINITY_MAX_ADMISSIONCNT;
	}
	
	if(m_InfinityModeInfo.MaxLv < i_pJoinPlayerSoc->m_character.Level 
		|| m_InfinityModeInfo.MinLv > i_pJoinPlayerSoc->m_character.Level) {
		// 레벨이 맞지 않다.
		return ERR_INFINITY_MISMATCH_LEVEL;
	}

	if(INFINITY_STATE_UNPREPARED !=	m_eInfinityState) {
		return ERR_INFINITY_STATE;
	}

	// 좀 더 빠른 처리를 위해 몇 가지 정보를 저장한다.
	i_pJoinPlayerSoc->m_InfinityPlayingInfo.InfinityCreateUID	= m_nInfinityCreateUID;
	i_pJoinPlayerSoc->m_InfinityPlayingInfo.InfinityModeUID		= m_InfinityModeInfo.InfinityModeUID;
	i_pJoinPlayerSoc->m_InfinityPlayingInfo.ModeTypeNum			= m_InfinityModeInfo.ModeTypeNum;
	i_pJoinPlayerSoc->m_InfinityPlayingInfo.MapIdx				= m_InfinityModeInfo.MapIdx;
	i_pJoinPlayerSoc->m_InfinityPlayingInfo.InfinityState		= INFINITY_STATE_UNPREPARED;
	i_pJoinPlayerSoc->m_InfinityPlayingInfo.TimePenaltyValue	= m_InfinityModeInfo.TimePenaltyValue;		// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 사망시 패널티 추가
	i_pJoinPlayerSoc->m_InfinityPlayingInfo.HPPenaltyValue		= m_InfinityModeInfo.HPPenaltyValue;		// 2011-06-14 by hskim, 인피니티 3차 - 패널티 기능 추가 (HP 및 시간 동시 지원을 위해)

	i_pJoinPlayerSoc->SendMFSInfinityStateChange(INFINITY_STATE_UNPREPARED);		// 2009-09-09 ~ 2010-01-20 by dhjin, 인피니티 - 인피 상태 값을 메인서버로 전송한다.

	INIT_MSG_WITH_BUFFER(MSG_FC_INFINITY_JOIN_OK, T_FC_INFINITY_JOIN_OK, pSMsg, SendBuf);
	pSMsg->bAccept				= TRUE;
	pSMsg->JoinClientIdx		= i_pJoinPlayerSoc->m_character.ClientIndex;
	util::strncpy(pSMsg->JoinCharacterName, i_pJoinPlayerSoc->m_character.CharacterName, SIZE_MAX_CHARACTER_NAME);
	pSMsg->Gear					= i_pJoinPlayerSoc->m_character.UnitKind;
	pSMsg->Lv					= i_pJoinPlayerSoc->m_character.Level;	

	mt_auto_lock mta(&m_mtVectPlayer);
	this->DeleteNullFISocPtrNoLock();
	m_mtVectPlayer.push_back(i_pJoinPlayerSoc);
	mtVectCFISoc::iterator itr = m_mtVectPlayer.begin();
	for(; itr != m_mtVectPlayer.end(); itr++) 
	{
		if(NULL != *itr) 
		{
			// 파티 가입된 유저 정보 전송전송
			(*itr)->SendAddData(SendBuf, MSG_SIZE(MSG_FC_INFINITY_JOIN_OK));
		}
	}

	// 2010. 06. 01 by hsLee 인피니티 필드 2차 난이도 조절. (에디트 박스 추가 + 난이도 변경 결과 패킷 변경.)
	INIT_MSG_WITH_BUFFER(MSG_FC_INFINITY_CHANGE_DIFFICULTY_LEVEL_OK , T_FC_INFINITY_CHANGE_DIFFICULTY_LEVEL_OK , pSMsg_DifficultyLevel , SendBuff_DifficultyLevel );
	pSMsg_DifficultyLevel->bUpdate = false;
	pSMsg_DifficultyLevel->InfinityDifficultyLevel = m_InfinityDifficultyLevel;

	i_pJoinPlayerSoc->SendAddData ( SendBuff_DifficultyLevel , MSG_SIZE( MSG_FC_INFINITY_CHANGE_DIFFICULTY_LEVEL_OK ) );
	// 2010. 06. 01 by hsLee 인피니티 필드 2차 난이도 조절. (에디트 박스 추가 + 난이도 변경 결과 패킷 변경.)

	return ERR_NO_ERROR;
}

Err_t CInfinityBase::ChangeMasterUser(CFieldIOCPSocket * i_pChangeMasterUserFISoc) {
	if(NULL == i_pChangeMasterUserFISoc) {
		return ERR_INVALID_PEER_CHARACTER;
	}
	
	if(m_MasterPlayerSoc->m_character.ClientIndex == i_pChangeMasterUserFISoc->m_character.ClientIndex) {
		return ERR_INFINITY_SAME_MASTER;
	}
	
	INIT_MSG_WITH_BUFFER(MSG_FC_INFINITY_READY_CANCEL_OK, T_FC_INFINITY_READY_CANCEL_OK, pCancelSMsg, CancelSendBuf);
	pCancelSMsg->ReadyCancelClientIdx	= m_MasterPlayerSoc->m_character.ClientIndex;
	if(INFINITY_STATE_PLAYING > m_MasterPlayerSoc->m_InfinityPlayingInfo.InfinityState) {
		// 2009-09-09 ~ 2010-01 by dhjin, 인피니티 - 소스 체크, 인피 진행중이면 값을 변경하지 않는다.
		m_MasterPlayerSoc->m_InfinityPlayingInfo.InfinityState = INFINITY_STATE_UNPREPARED;
	}
	INIT_MSG_WITH_BUFFER(MSG_FC_INFINITY_READY_OK, T_FC_INFINITY_READY_OK, pReadySMsg, ReadySendBuf);
	pReadySMsg->ReadyClientIdx	= i_pChangeMasterUserFISoc->m_character.ClientIndex;
	if(INFINITY_STATE_PLAYING > i_pChangeMasterUserFISoc->m_InfinityPlayingInfo.InfinityState) {
		// 2009-09-09 ~ 2010-01 by dhjin, 인피니티 - 소스 체크, 인피 진행중이면 값을 변경하지 않는다.
		i_pChangeMasterUserFISoc->m_InfinityPlayingInfo.InfinityState = INFINITY_STATE_READY;
	}
	m_MasterPlayerSoc = i_pChangeMasterUserFISoc;

	INIT_MSG_WITH_BUFFER(MSG_FC_INFINITY_CHANGE_MASTER_OK, T_FC_INFINITY_CHANGE_MASTER_OK, pSMsg, SendBuf);
	pSMsg->ChangeMasterClientIdx = m_MasterPlayerSoc->m_character.ClientIndex;
	mt_auto_lock mta(&m_mtVectPlayer);
	this->DeleteNullFISocPtrNoLock();
	mtVectCFISoc::iterator itr = m_mtVectPlayer.begin();
	for(; itr != m_mtVectPlayer.end(); itr++) {
		if(NULL != *itr) {
			(*itr)->SendAddData(SendBuf, MSG_SIZE(MSG_FC_INFINITY_CHANGE_MASTER_OK));
			(*itr)->SendAddData(ReadySendBuf, MSG_SIZE(MSG_FC_INFINITY_READY_OK));
			(*itr)->SendAddData(CancelSendBuf, MSG_SIZE(MSG_FC_INFINITY_READY_CANCEL_OK));
		}
	}

	return ERR_NO_ERROR;
}

Err_t CInfinityBase::Leave(CFieldIOCPSocket * i_pLeavePlayerSoc) {
	// 유저 탈퇴
	if(NULL == i_pLeavePlayerSoc) {
		return ERR_INVALID_CHARACTER;
	}
	
	i_pLeavePlayerSoc->SendMFSInfinityStateChange(INFINITY_STATE_NONE);		// 2009-09-09 ~ 2010-01-20 by dhjin, 인피니티 - 인피 상태 값을 메인서버로 전송한다.

// 2010-04-05 by cmkwon, 인피 재입장 카드 관련 시스템 수정 - 
// 	///////////////////////////////////////////////////////////////////////////////
// 	// 2010-03-23 by cmkwon, 인피니티 입장 캐쉬 아이템 구현 - Start 처리 도중 유저가 나가는 경우 처리
// 	if(INFINITY_STATE_READY_ENTER_CHECK == this->m_eInfinityState)
// 	{
// 		this->m_eInfinityState		= INFINITY_STATE_READY;
// 	}

	INIT_MSG_WITH_BUFFER(MSG_FC_INFINITY_LEAVE_OK, T_FC_INFINITY_LEAVE_OK, pSMsg, SendBuf);
	pSMsg->LeaveClientIdx = i_pLeavePlayerSoc->m_character.ClientIndex;
	
	mt_auto_lock mta(&m_mtVectPlayer);
	this->DeleteNullFISocPtrNoLock();
	mtVectCFISoc::iterator itr = m_mtVectPlayer.begin();
	while(itr != m_mtVectPlayer.end()) {
		if(NULL != *itr) {
			(*itr)->SendAddData(SendBuf, MSG_SIZE(MSG_FC_INFINITY_LEAVE_OK));
			if((*itr) == i_pLeavePlayerSoc) {
				this->LogInfinityLeave(*itr);	// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 게임로그 추가, 인피 탈퇴시 아이템 복사 정보
				// 탈퇴 유저 초기화
				util::zero(&i_pLeavePlayerSoc->m_InfinityPlayingInfo, sizeof(INFINITY_PLAYING_INFO));
				itr = m_mtVectPlayer.erase(itr);
				continue;
			}
		}
		itr++;
	}

	// 방장이었다면 새로운 방장~
	if(m_MasterPlayerSoc == i_pLeavePlayerSoc
		&& 1 <= this->GetPlayerListSize()) {
		itr = m_mtVectPlayer.begin();
		for(; itr != m_mtVectPlayer.end(); itr++) {
			if(NULL != *itr) {
				m_MasterPlayerSoc = *itr;
			}
		}			
		
		INIT_MSG_WITH_BUFFER(MSG_FC_INFINITY_CHANGE_MASTER_OK, T_FC_INFINITY_CHANGE_MASTER_OK, pMasterSMsg, MasterSendBuf);
		pMasterSMsg->ChangeMasterClientIdx = m_MasterPlayerSoc->m_character.ClientIndex;
		INIT_MSG_WITH_BUFFER(MSG_FC_INFINITY_READY_OK, T_FC_INFINITY_READY_OK, pReadySMsg, ReadySendBuf);
		pReadySMsg->ReadyClientIdx	= m_MasterPlayerSoc->m_character.ClientIndex;
		if(INFINITY_STATE_UNPREPARED == m_MasterPlayerSoc->m_InfinityPlayingInfo.InfinityState) {
			// 2009-09-09 ~ 2010-01-19 by dhjin, 인피니티 - 인피 진행 중 방장 위임 시, 방장 인피 상태값이 잘못 수정되는 버그 수정
			m_MasterPlayerSoc->m_InfinityPlayingInfo.InfinityState = INFINITY_STATE_READY;
		}
		itr = m_mtVectPlayer.begin();
		for(; itr != m_mtVectPlayer.end(); itr++) {
			if(NULL != *itr) {
				(*itr)->SendAddData(MasterSendBuf, MSG_SIZE(MSG_FC_INFINITY_CHANGE_MASTER_OK));
				(*itr)->SendAddData(ReadySendBuf, MSG_SIZE(MSG_FC_INFINITY_READY_OK));
			}
		}		
	}
	mta.auto_unlock_cancel();

	return ERR_NO_ERROR;
}


Err_t CInfinityBase::Ban(CFieldIOCPSocket * i_pBanPlayerSoc) {
	// 유저 탈퇴
	if(NULL == i_pBanPlayerSoc) {
		return ERR_INVALID_PEER_CHARACTER;
	}
	
	i_pBanPlayerSoc->SendMFSInfinityStateChange(INFINITY_STATE_NONE);		// 2009-09-09 ~ 2010-01-20 by dhjin, 인피니티 - 인피 상태 값을 메인서버로 전송한다.

	INIT_MSG_WITH_BUFFER(MSG_FC_INFINITY_BAN_OK, T_FC_INFINITY_BAN_OK, pSMsg, SendBuf);
	pSMsg->BanClientIdx = i_pBanPlayerSoc->m_character.ClientIndex;
	
	mt_auto_lock mta(&m_mtVectPlayer);
	this->DeleteNullFISocPtrNoLock();
	mtVectCFISoc::iterator itr = m_mtVectPlayer.begin();
	while(itr != m_mtVectPlayer.end()) {
		if(NULL != *itr) {
			(*itr)->SendAddData(SendBuf, MSG_SIZE(MSG_FC_INFINITY_BAN_OK));
			if((*itr) == i_pBanPlayerSoc) {
				// 탈퇴 유저 초기화
				util::zero(&i_pBanPlayerSoc->m_InfinityPlayingInfo, sizeof(INFINITY_PLAYING_INFO));
				itr = m_mtVectPlayer.erase(itr);
				continue;
			}
		}
		itr++;
	}
	
	return ERR_NO_ERROR;
}

// 2010-04-05 by cmkwon, 인피 재입장 카드 관련 시스템 수정 - 
Err_t CInfinityBase::Start(CFieldIOCPSocket * i_pMasterPlayerSoc, CFieldMapChannel *i_pFMChann)
{
//	g_pFieldGlobal->WriteSystemLogEX(TRUE, "[TEMP] [Infinity][%I64d] CInfinityBase::Start# !!, %s BaseInfiState(%d) MasterInfiState(%d) curMapChannel(0x%X) \r\n"
//		, m_nInfinityCreateUID, GetCharacterString(i_pMasterPlayerSoc->GetCharacter(), string()), m_eInfinityState, i_pMasterPlayerSoc->m_InfinityPlayingInfo.InfinityState, m_pInfinityMapChannel);

	// 시작
	if(NULL == i_pMasterPlayerSoc)
	{
		return ERR_INVALID_CHARACTER;
	}

	if(i_pMasterPlayerSoc != m_MasterPlayerSoc)
	{
		return ERR_INFINITY_MASTER;
	}

	if(NULL == i_pFMChann)
	{
		return ERR_INFINITY_CREATE_MAPCHANNEL;
	}

	
	mt_auto_lock mta(&m_mtVectPlayer);
	this->DeleteNullFISocPtrNoLock();

// 2010-03-23 by cmkwon, 인피니티 입장 캐쉬 아이템 구현 - 아래와 같이 수정
// 	if(FALSE == this->CheckAllReady()) {
// 		return ERR_INFINITY_NOT_ALL_READY;
// 	}
	///////////////////////////////////////////////////////////////////////////////
	// 2010-03-23 by cmkwon, 인피니티 입장 캐쉬 아이템 구현 - 
	if( FALSE == this->CheckInfinityAllPlayerState ( INFINITY_STATE_READY_ENTER_CHECKED ) )
	{
 		return ERR_INFINITY_NOT_ALL_READY;
	}

	if(this->m_InfinityModeInfo.MinAdmissionCount > this->GetPlayerListSize())
	{
		return ERR_INFINITY_MIN_ADMISSIONCNT;
	}

	mtVectCFISoc::iterator itr = m_mtVectPlayer.begin();
	for(; itr != m_mtVectPlayer.end(); itr++)
	{
		CFieldIOCPSocket *pFISoc = *itr;
		if(NULL == pFISoc
			|| FALSE == pFISoc->IsUsing())
		{
			g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ERROR] [Infinity][%I64d] CInfinityBase::Start# Client Socket is invalid !!, %s \r\n"
				, m_nInfinityCreateUID, GetCharacterString(pFISoc->GetCharacter(), string()));
			return ERR_INFINITY_NOT_ALL_READY;
		}

		INIT_MSG_WITH_BUFFER(MSG_FtoA_INFINITY_START, T_FtoA_INFINITY_START, pSMsg, SendBuf);
		pSMsg->AccountUID		= pFISoc->m_character.AccountUniqueNumber;
		pSMsg->MFSCharacterUID	= pFISoc->m_AfsNeedMfsInfo.MFSCharacterUID;
		pSMsg->MFSClientIndex	= pFISoc->m_AfsNeedMfsInfo.MFSClientIdx;
		pSMsg->AFSCharacterUID	= pFISoc->m_character.CharacterUniqueNumber;
		pSMsg->AFSClientIndex	= pFISoc->m_character.ClientIndex;
		if(FALSE == pFISoc->InfinitySendAddDataFromAFS2MFS(SendBuf, MSG_SIZE(MSG_FtoA_INFINITY_START)))
		{
			g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ERROR] [Infinity][%I64d] CInfinityBase::Start# Can't send to MFS !!, %s \r\n"
				, m_nInfinityCreateUID, GetCharacterString(pFISoc->GetCharacter(), string()));
			return ERR_INFINITY_NOT_ALL_READY;
		}

		pFISoc->m_InfinityPlayingInfo.InfinityState	= INFINITY_STATE_ENTERING;	// 2009-09-09 ~ 2010-01-29 by dhjin, 인피니티 - 인피니티 시작 버튼 누른 상태 추가
		pFISoc->SetCurrentFieldMapChannel(i_pFMChann);							// 2010-04-06 by cmkwon, 인피2차 추가 수정 - 
		// 2010-06-25 by shcho, 인피니티 관련로그 찍기 - 시작시 유저들의 정보 (레벨,케릭터이름), 인피 난이도 정보도 추가
		pFISoc->m_InfCurrentAllEXP = pFISoc->m_character.Experience;	//시작시 유저 경험치 저장
		g_pFieldGlobal->WriteSystemLogEX(TRUE, "  INFINITYSTART!!! USERINFO[ MapChannel = ( %d ) Character( %s ) Level( %d ) InfinityDifficultLevel( %d )] \r\n", 
										 pFISoc->GetCharacter()->MapChannelIndex ,GetCharacterString(pFISoc->GetCharacter(), string()) ,pFISoc->GetCharacter()->Level,this->m_InfinityDifficultyLevel); //인피니티 난이도 정보 추가
		pFISoc->m_SkillManager.ResetDebuffSkill();	// 2013-03-27 by jhseol, 인피시작시 디버프 및 도트 정보 리셋
		pFISoc->m_SkillManager.ResetDotSkill();	// 2013-03-27 by jhseol, 인피시작시 디버프 및 도트 정보 리셋
	}
	i_pMasterPlayerSoc->m_InfinityPlayingInfo.InfinityState = INFINITY_STATE_ENTERING;	// 2009-09-09 ~ 2010-01-29 by dhjin, 인피니티 - 인피니티 시작 버튼 누른 상태 추가

	// 2010. 06. 04 by hsLee 인피니티 필드 2차 난이도 조절. (서버 - 키 몬스터 리스트 등록 정보의 소환 카운트에도 밸런스 적용.)
	SetUpdate_KeyMonsterSummonCount();
	// End 2010. 06. 04 by hsLee 인피니티 필드 2차 난이도 조절. (서버 - 키 몬스터 리스트 등록 정보의 소환 카운트에도 밸런스 적용.)

	// 2010. 06. 17 by hsLee. 인피니티 파일 로그 기록.
	//..


// 2010-04-05 by cmkwon, 인피 재입장 카드 관련 시스템 수정 - 
//	this->SetInfinityState(INFINITY_STATE_READY);
	this->m_pInfinityMapChannel		= i_pFMChann;			// 2010-04-05 by cmkwon, 인피 재입장 카드 관련 시스템 수정 - 여기에서 할당한다 
	return ERR_NO_ERROR;
}

Err_t CInfinityBase::CreateMap(CFieldMapChannel * i_pInfinityMapChannel)
{
// 2010-04-05 by cmkwon, 인피 재입장 카드 관련 시스템 수정 - 
// 	// 맵 채널 할당
// 	if(NULL == i_pInfinityMapChannel) {
// 
// 		// 2010-04-02 by cmkwon, 인피2차 추가 수정 - 
// 		g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ERROR] [Infinity][%I64d] CInfinityBase::CreateMap# FieldMapChannel is NULL !! MapIdx(%d) \r\n"
// 			, this->m_nInfinityCreateUID, this->m_InfinityModeInfo.MapIdx);
// 		return ERR_INFINITY_CREATE_MAPCHANNEL;
// 	}
// 	m_pInfinityMapChannel = i_pInfinityMapChannel;
	///////////////////////////////////////////////////////////////////////////////
	// 2010-04-05 by cmkwon, 인피 재입장 카드 관련 시스템 수정 - 
	if(NULL == m_pInfinityMapChannel)
	{
		// 2010-04-02 by cmkwon, 인피2차 추가 수정 - 
		g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ERROR] [Infinity][%I64d] CInfinityBase::CreateMap# FieldMapChannel is NULL !! MapIdx(%d) \r\n"
			, this->m_nInfinityCreateUID, this->m_InfinityModeInfo.MapIdx);
		return ERR_INFINITY_CREATE_MAPCHANNEL;
	}

	// 맵 할당
	mt_auto_lock mta(&m_mtVectPlayer);
	this->DeleteNullFISocPtrNoLock();
	mtVectCFISoc::iterator itr = m_mtVectPlayer.begin();
	for(; itr != m_mtVectPlayer.end(); itr++) {
		if(NULL != *itr) {
			// 인피 시작 전 맵 관련 설정
			(*itr)->SetInfinityMap(this->m_InfinityModeInfo.MapIdx,	m_pInfinityMapChannel->GetMapChannelIndex().ChannelIndex);
		}
	}	

	// 틱
	m_InfinityCreatedTick = timeGetTime();
	m_InfinityLimitTick = 0;	// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 사망시 패널티 추가
	m_InfinityCreatedTime.SetCurrentDateTime();
	
	this->SetInfinityState(INFINITY_STATE_PLAYING);

	// 2009-09-09 ~ 2010-01-26 by dhjin, 인피니티 - 자동 편대 
	this->AutoPartyCreate();

	// 맵 입장 전송
	INIT_MSG_WITH_BUFFER(MSG_FC_INFINITY_ENTER, T_FC_INFINITY_ENTER, pSMsg, SendBuf);
	pSMsg->InfinityChannelIdx	= m_pInfinityMapChannel->GetMapChannelIndex().ChannelIndex;
	itr = m_mtVectPlayer.begin();
	for(; itr != m_mtVectPlayer.end(); itr++) {
		if(NULL != *itr) {
			// MFS로 인피 시작 버튼이 눌렸음을 전송
			pSMsg->CharacterPosition	= (*itr)->m_character.PositionVector;
			pSMsg->StartTime			= m_InfinityCreatedTime;
			pSMsg->LimitTime			= this->m_InfinityModeInfo.LimitTime;
			(*itr)->SendAddData(SendBuf, MSG_SIZE(MSG_FC_INFINITY_ENTER));
			(*itr)->m_InfinityPlayingInfo.InfinityState = INFINITY_STATE_PLAYING;
			
			(*itr)->StatisticsInfinityStart();		// 2012-01-16 by hskim, 통계 - 화패
			
//			g_pFieldGlobal->WriteSystemLogEX(TRUE, "[TEMP] [Infinity][%I64d] T_FC_INFINITY_ENTER , CharacterName = %s \r\n", (*itr)->m_InfinityPlayingInfo.InfinityCreateUID, (*itr)->m_character.CharacterName);	// 2009-09-09 ~ 2010-01-29 by dhjin, 인피니티 - 인피니티 체크를 위한 로그 추가

			// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 게임로그 추가
			CAtumLogSender::SendLogMessageInfinityStart(*itr);
		}
	}

// 2010-04-05 by cmkwon, 인피 재입장 카드 관련 시스템 수정 - 방장이 시작버튼을 눌렀을때 귀속 처리로 수정
// 	// 인피 귀속 처리
// 	INIT_MSG_WITH_BUFFER(MSG_FtoA_INFINITY_IMPUTE, T_FtoA_INFINITY_IMPUTE, pImputeSMsg, ImputeSendBuf);
// 	itr = m_mtVectPlayer.begin();
// 	for(; itr != m_mtVectPlayer.end(); itr++) {
// 		if(NULL != *itr) {
// 			// MFS로 인피 시작 버튼이 눌렸음을 전송
// 			CIOCPSocket	*	MFSSock;
// 			MFSSock = (*itr)->ms_pFieldIOCP->GetMFSSockForArenaServer((*itr)->m_AfsNeedMfsInfo.MFSServerID);
// 			if(NULL == MFSSock) {// 2007-12-28 by dhjin, 필드 서버에 대한 정보가 없으면 오류
// 				g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ERROR][Infinity][%I64d] MFS NULL, CharacterName = %s \r\n", m_nInfinityCreateUID, (*itr)->m_character.CharacterName);
// 				continue;
// 			}
// 			// 진입 수 갱신
// 			BOOL CheckExistImpute = FALSE;
// 			mt_auto_lock mtImpute(&(*itr)->m_mtVecInfiImpute);
// 			mt_vectorInfinityImpute::iterator ImputeItr = (*itr)->m_mtVecInfiImpute.begin();
// 			for(; ImputeItr != (*itr)->m_mtVecInfiImpute.end(); ImputeItr++) {
// 				if(this->m_InfinityModeInfo.InfinityModeUID == ImputeItr->InfinityModeUID){
// 					// 한 번 입장한 적이 있으면 ++
// 					ImputeItr->EntranceCount++;
// 					CheckExistImpute = TRUE;
// 					mtImpute.auto_unlock_cancel();
// 					break;
// 				}
// 			}
// 			mtImpute.auto_unlock_cancel();
// 
// 			// 2010-03-23 by cmkwon, 인피니티 입장 캐쉬 아이템 구현 - 입장 아이템을 소유하고 있는지 체크
// 			if((*itr)->m_InfinityPlayingInfo.bHaveReentryTicket)
// 			{
// 				if(FALSE == (*itr)->m_ItemManager.DeleteItemGeneralByDestParam(DES_INFINITY_REENTRY_TICKET))
// 				{
// 					g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ERROR][Infinity][%I64d] Delete INFINITY_REENTRY_TICKET, CharacterName = %s \r\n", m_nInfinityCreateUID, (*itr)->m_character.CharacterName);
// 				}
// 			}
// 
// 			if(FALSE == CheckExistImpute) {
// 				// 첫 입장이면 정보 추가
// 				INFINITY_IMPUTE		InfinityImpute;
// 				util::zero(&InfinityImpute, sizeof(INFINITY_IMPUTE));
// 				InfinityImpute.InfinityModeUID	= this->m_InfinityModeInfo.InfinityModeUID;
// 				InfinityImpute.EntranceCount	= 1;
// 				(*itr)->m_mtVecInfiImpute.pushBackLock(InfinityImpute);
// 			}
// 			pImputeSMsg->AccountUID			= (*itr)->m_character.AccountUniqueNumber;
// 			pImputeSMsg->MFSCharacterUID	= (*itr)->m_AfsNeedMfsInfo.MFSCharacterUID;
// 			pImputeSMsg->MFSClientIndex		= (*itr)->m_AfsNeedMfsInfo.MFSClientIdx;
// 			pImputeSMsg->InfinityModeUID	= this->m_InfinityModeInfo.InfinityModeUID;
// 			pImputeSMsg->AFSClientIndex		= (*itr)->m_character.ClientIndex;
// 			pImputeSMsg->CheckExistImpute	= CheckExistImpute;
// 			MFSSock->SendAddData(ImputeSendBuf, MSG_SIZE(MSG_FtoA_INFINITY_IMPUTE));
// 		}
// 	}

	mta.auto_unlock_cancel();
	
	g_pFieldGlobal->WriteSystemLogEX(TRUE, "[Notify] [Infinity][%I64d] CInfinityBase::CreateMap# created MapChannel !, MapInfo(%s) \r\n", m_nInfinityCreateUID, GetMapString(m_pInfinityMapChannel->GetMapChannelIndex(), string()));

	return ERR_NO_ERROR;
}


// 2010. 05. 19 by hsLee 인피니티 필드 2차 난이도 조절. (신호처리 + 몬스터 처리(서버) )
/**************************************************************************************************************
**
**	인피니티 - 방의 난이도 변경.
**
**	Create Info :	2010. 05. 24. by hsLee.
**
***************************************************************************************************************/
Err_t CInfinityBase	:: ChangeDifficultyLevel ( CFieldIOCPSocket * i_pMasterPlayerSoc , const INT a_ChangeDifficultyLevel )
{

	if ( NULL == i_pMasterPlayerSoc )
		return ERR_INVALID_CHARACTER;

	if ( i_pMasterPlayerSoc->m_character.ClientIndex != m_MasterPlayerSoc->m_character.ClientIndex )
		return ERR_INFINITY_MASTER;

 	if ( a_ChangeDifficultyLevel == GetDifficultyLevel() )
 		return ERR_INFINITY_DIFFICULTY_LEVEL_SAME;

	bool bUpdate = false;

	if ( INFINITY_STATE_ENTERING > m_MasterPlayerSoc->m_InfinityPlayingInfo.InfinityState )
	{
		if ( FALSE == SetDifficultyLevel ( a_ChangeDifficultyLevel ) )
			return ERR_INFINITY_DIFFICULTY_LEVEL_INVALID;

		bUpdate = true;
	}

	INIT_MSG_WITH_BUFFER ( MSG_FC_INFINITY_CHANGE_DIFFICULTY_LEVEL_OK , T_FC_INFINITY_CHANGE_DIFFICULTY_LEVEL_OK , pSendMsg , SendBuff );

	pSendMsg->bUpdate = TRUE;
	pSendMsg->InfinityDifficultyLevel = GetDifficultyLevel();

	mt_auto_lock mta( &m_mtVectPlayer );
	this->DeleteNullFISocPtrNoLock();
	mtVectCFISoc::iterator itr = m_mtVectPlayer.begin();

	for (; itr != m_mtVectPlayer.end(); itr++ )
	{
		if ( NULL != *itr )
		{
			(*itr)->SendAddData ( SendBuff , MSG_SIZE( MSG_FC_INFINITY_CHANGE_DIFFICULTY_LEVEL_OK ) );
		}
	}


	// Ready 해제 처리. ( 업데이트 있을 경우에만. )
 	itr = m_mtVectPlayer.begin();

 	INIT_MSG_WITH_BUFFER ( MSG_FC_INFINITY_READY_CANCEL_ALL_OK , T_FC_INFINITY_READY_CANCEL_ALL_OK , pCancelAllSMsg , CancelAllSendBuff );

	pCancelAllSMsg->InfinityCreateUID = m_MasterPlayerSoc->m_InfinityPlayingInfo.InfinityCreateUID;

 	for (; itr != m_mtVectPlayer.end(); itr++ )
 	{
 		if ( NULL == *itr )
 			continue;

		if ( m_MasterPlayerSoc->m_character.ClientIndex != (*itr)->m_character.ClientIndex )
			(*itr)->m_InfinityPlayingInfo.InfinityState = INFINITY_STATE_UNPREPARED;

		(*itr)->SendAddData ( CancelAllSendBuff , MSG_SIZE( MSG_FC_INFINITY_READY_CANCEL_ALL_OK ) );
 	}

	mta.auto_unlock_cancel();

	return ERR_NO_ERROR;

}


// 
/********************************************************************************************************
**
**	방의 난이도 값 대입. - 대입하려는 값이 유효하지 않을 경우 일단 기본값을 대입 시도. ( 1회. )
**
**	Create Info : 2010. 05. 25. by hsLee.
**
*********************************************************************************************************/
BOOL CInfinityBase :: SetDifficultyLevel ( INT DifficultyLevel )
{

	if ( NULL == m_MasterPlayerSoc || NULL == m_MasterPlayerSoc->ms_pFieldIOCP )
		return FALSE;

	
	CInfinityManager *pcInfiMgr = &m_MasterPlayerSoc->ms_pFieldIOCP->m_InfinityManager;

	const INFINITY_DIFFICULTY_BONUS_INFO *a_pDifficultyBonusInfo = pcInfiMgr->Get_Difficulty_BonusInfo ( DifficultyLevel );
	const INFINITY_DIFFICULTY_MONSTER_SETTING_INFO *a_pDifficultyMonsterInfo = pcInfiMgr->Get_Difficulty_MonsterInfo ( DifficultyLevel );

	if ( NULL == a_pDifficultyBonusInfo || NULL == a_pDifficultyMonsterInfo )
	{
		DifficultyLevel = DEFAULT_LEVEL_INFINITY_DIFFICULTY;
		
		a_pDifficultyBonusInfo = pcInfiMgr->Get_Difficulty_BonusInfo ( DifficultyLevel );
		a_pDifficultyMonsterInfo = pcInfiMgr->Get_Difficulty_MonsterInfo ( DifficultyLevel );
	}

	if ( NULL == a_pDifficultyBonusInfo || NULL == a_pDifficultyMonsterInfo )
		return FALSE;

	m_InfinityDifficultyLevel = DifficultyLevel;
	
	m_InfinityDifficultyData.Init( DifficultyLevel );
	
	m_InfinityDifficultyData += *a_pDifficultyBonusInfo;
	m_InfinityDifficultyData += *a_pDifficultyMonsterInfo;

	return TRUE;

}


// 2010. 07. 27 by hsLee 인피니티 2차 거점 방어 시네마 연출 스킵 처리.
/***************************************************************************************************************************************
**
**	인피니티 엔딩 연출 스킵 처리.
**
**	Create Info :	2010. 07. 27. by hsLee.
**
**	Update Info :	스킵 처리에 '정상 종료' 처리 추가.	'a_bNormalEnding' 2010. 08. 26. by hsLee.
**
****************************************************************************************************************************************/
Err_t CInfinityBase :: SkipEndingCinema ( CFieldIOCPSocket * i_pPlayerSoc , const bool a_bNormalEnding /*= false*/ )
{

	if ( NULL == i_pPlayerSoc || !GetPlayerInVectPlayerByClientIndex ( i_pPlayerSoc->GetCharacter()->ClientIndex ) )
		return ERR_INVALID_CHARACTER;

	if ( !SetValue_SkipEndingCinema ( i_pPlayerSoc->GetCharacter()->CharacterName , a_bNormalEnding ) )
		return ERR_INFINITY_ALWAYS_SKIP_ENDINGCINEMA;

	g_pFieldGlobal->WriteSystemLogEX ( TRUE , "  [Notify] [Infinity][%I64d] Request Skip Cinema. (InfinityMode:%d), RequestUser(%s:%d)  \r\n" , 
										m_nInfinityCreateUID , 
										m_InfinityModeInfo.ModeTypeNum , 
										i_pPlayerSoc->m_character.CharacterName , 
										i_pPlayerSoc->m_character.AccountUniqueNumber );

	INIT_MSG_WITH_BUFFER ( MSG_FC_INFINITY_SKIP_ENDING_CINEMA_OK , T_FC_INFINITY_SKIP_ENDING_CINEMA_OK , pSendMsg , SendBuff );
	
	pSendMsg->EnableSkip = true;
	pSendMsg->NormalEnding = a_bNormalEnding;
	strcpy ( pSendMsg->szCharName , m_szRqSkipEndingCinema_UserName );
	pSendMsg->sUpdateTenderItemStartTime = m_sCinemaSkipTime;
	
	mt_auto_lock mta( &m_mtVectPlayer );
	this->DeleteNullFISocPtrNoLock();
	mtVectCFISoc::iterator itr = m_mtVectPlayer.begin();
	
	for (; itr != m_mtVectPlayer.end(); itr++ )
	{
		if ( NULL != *itr )
		{
			(*itr)->SendAddData ( SendBuff , MSG_SIZE( MSG_FC_INFINITY_SKIP_ENDING_CINEMA_OK ) );
		}
	}

	mta.auto_unlock_cancel();

	return ERR_NO_ERROR;

}

// End 2010. 07. 27 by hsLee 인피니티 2차 거점 방어 시네마 연출 스킵 처리.

// End 2010. 05. 19 by hsLee 인피니티 필드 2차 난이도 조절. (신호처리 + 몬스터 처리(서버) )


// 2010. 06. 04 by hsLee 인피니티 필드 2차 난이도 조절. (서버 - 키 몬스터 리스트 등록 정보의 소환 카운트에도 밸런스 적용.)
/******************************************************************************
**
**	방의 등록된 키 몬스터 소환 Count 횟수 난이도 밸런스 적용. 
**
**	Create Info :	2010. 06. 03. by hsLee.
**
*******************************************************************************/
void CInfinityBase :: SetUpdate_KeyMonsterSummonCount( void )
{

	vectorCinemaInfo::iterator itr_CinemaInfo;

	VectKeyMonsterInfo::iterator itr(m_KeyMonsterDeadForNextStepList.begin());
	for(; itr != m_KeyMonsterDeadForNextStepList.end(); itr++) 
	{
		KEY_MONSTER_INFO *pKeyMonInfo = &*itr;

		if( pKeyMonInfo->KeyMonsterIdx != 0 ) 
		{
			itr_CinemaInfo = m_CinemaInfoList.begin();

			for (; itr_CinemaInfo != m_CinemaInfoList.end(); ++itr_CinemaInfo )
			{
				if ( itr_CinemaInfo->MonsterIdx == pKeyMonInfo->KeyMonsterIdx )
				{
					MONSTER_INFO *pMonsterinfo = ms_pFieldIOCP4->GetMonsterInfo ( itr_CinemaInfo->MonsterIdx );
					
					if ( NULL == pMonsterinfo || COMPARE_MPOPTION_BIT(pMonsterinfo->MPOption, MPOPTION_BIT_KEY_MONSTER_ALIVE_FOR_GAMECLEAR) )
						break;

					switch ( m_InfinityModeInfo.ModeTypeNum )
					{
						case INFINITY_MODE_BOSSRUSH :

							if ( pMonsterinfo->Belligerence != BELL_INFINITY_DEFENSE_MONSTER && NULL == GetKeyMonsterInfoFromKeyMonsterDeadForNextStep ( pMonsterinfo->MonsterUnitKind ) )
								pKeyMonInfo->KeyMonsterCount = ( itr_CinemaInfo->SummonCount * m_InfinityDifficultyData.fSummonCountRatio <= .0f ? 1.0f : m_InfinityDifficultyData.fSummonCountRatio );

							break;

						case INFINITY_MODE_DEFENCE :

							if ( pMonsterinfo->Belligerence != BELL_INFINITY_DEFENSE_MONSTER )
								pKeyMonInfo->KeyMonsterCount = ( itr_CinemaInfo->SummonCount * m_InfinityDifficultyData.fSummonCountRatio <= .0f ? 1.0f : m_InfinityDifficultyData.fSummonCountRatio );

							break;

                        // 2011-02-18 by hskim, 인피니티 3차 - 방 생성 작업
						case INFINITY_MODE_MSHIPBATTLE :

							if ( pMonsterinfo->Belligerence != BELL_INFINITY_DEFENSE_MONSTER )
								pKeyMonInfo->KeyMonsterCount = ( itr_CinemaInfo->SummonCount * m_InfinityDifficultyData.fSummonCountRatio <= .0f ? 1.0f : m_InfinityDifficultyData.fSummonCountRatio );
							
                            break;
					   // end 2011-02-18 by hskim, 인피니티 3차 - 방 생성 작업
					}
					
					pKeyMonInfo->KeyMonsterCount = max ( pKeyMonInfo->KeyMonsterCount , 1 );
					break;
				}
			}
		}
	}

}
// End 2010. 06. 04 by hsLee 인피니티 필드 2차 난이도 조절. (서버 - 키 몬스터 리스트 등록 정보의 소환 카운트에도 밸런스 적용.)


void CInfinityBase::Fin(BOOL i_bSuccess, INFINITY_FIN_t i_finType)
{
	// 종료
	
	// 인피 종료 메세지 전송
	mt_auto_lock mta(&m_mtVectPlayer);
	this->DeleteNullFISocPtrNoLock();
	mtVectCFISoc::iterator itr = m_mtVectPlayer.begin();
	for(; itr != m_mtVectPlayer.end(); itr++) {
		if(NULL != *itr) 
		{
			// 2010-06-25 by shcho, 인피니티 관련로그 찍기 - 함수 구조 변경
			// 시네마 정보 전송
			// 2010-03-31 by dhjin, 인피니티(기지방어) - 밑과 같이 수정
//			(*itr)->m_InfinityPlayingInfo.InfinityState = INFINITY_STATE_DONE;
			INIT_MSG_WITH_BUFFER(MSG_FtoA_INFINITY_LOG, T_FtoA_INFINITY_LOG, pLogSMsg, LogSendBuf);
			CIOCPSocket	*	MFSSock;
			MFSSock = (*itr)->ms_pFieldIOCP->GetMFSSockForArenaServer();
		
			if(NULL == MFSSock) 
			{// 2007-12-28 by dhjin, 필드 서버에 대한 정보가 없으면 오류
				g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ERROR][Infinity][%I64d] MFS NULL, CharacterName = %s \r\n", m_nInfinityCreateUID, (*itr)->m_character.CharacterName);
				continue;
			}

			if(FALSE == i_bSuccess) 
			{
				// 실패 종료
				(*itr)->m_InfinityPlayingInfo.InfinityState = INFINITY_STATE_DONE_FAIL;		// 2010-03-31 by dhjin, 인피니티(기지방어) - 
				(*itr)->SendAddMessageType(T_FC_INFINITY_FAIL_FIN);	
			}
			else 
			{
				// 성공 종료
				(*itr)->m_InfinityPlayingInfo.InfinityState = INFINITY_STATE_DONE_SUCCESS;		// 2010-03-31 by dhjin, 인피니티(기지방어) - 
				(*itr)->SendAddMessageType(T_FC_INFINITY_SUCCESS_FIN);
				}

				pLogSMsg->AccountUID			= (*itr)->m_character.AccountUniqueNumber;
				pLogSMsg->MFSCharacterUID		= (*itr)->m_AfsNeedMfsInfo.MFSCharacterUID;
				pLogSMsg->MFSClientIndex		= (*itr)->m_AfsNeedMfsInfo.MFSClientIdx;
				pLogSMsg->InfinityModeUID		= this->m_InfinityModeInfo.InfinityModeUID;
				pLogSMsg->AFSClientIndex		= (*itr)->m_character.ClientIndex;
				pLogSMsg->DeathCount			= (*itr)->m_InfinityPlayingInfo.DeathCount;
				pLogSMsg->KillCount				= (*itr)->m_InfinityPlayingInfo.KillCount;
				pLogSMsg->StartTime				= this->m_InfinityCreatedTime;
				pLogSMsg->EndTime.SetCurrentDateTime();
			pLogSMsg->AllEXP				= ((*itr)->m_character.Experience - (*itr)->m_InfCurrentAllEXP); // 2010-06-25 by shcho, 인피니티 관련로그 찍기 - 종료시 최대 경험치 추가
				MFSSock->SendAddData(LogSendBuf, MSG_SIZE(MSG_FtoA_INFINITY_LOG));
			
			this->LogInfinityFinUser(*itr, i_bSuccess);	// 2009-09-09 ~ 2010-01-13 by dhjin, 인피니티 - 유저 정보 제외 종료 관련 로그 여러번 남는 버그 한 번만 남게 수정 // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 게임로그 추가, 종료 관련 정보		

			(*itr)->m_InfinityPlayingInfo.DeathCount = 0;
			(*itr)->m_InfinityPlayingInfo.KillCount = 0;
			// End 2010-06-25 by shcho, 인피니티 관련로그 찍기 - 함수 구조 변경
		}
	}
	mta.auto_unlock_cancel();

	this->LogInfinityFinAliveKeyMonster();			// 2009-09-09 ~ 2010-01-13 by dhjin, 인피니티 - 유저 정보 제외 종료 관련 로그 여러번 남는 버그 한 번만 남게 수정

	// 2010-03-31 by dhjin, 인피니티(기지방어) - 밑과 같이 수정
//	m_eInfinityState = INFINITY_STATE_DONE;
	if(FALSE == i_bSuccess) {
		this->SetInfinityState(INFINITY_STATE_DONE_FAIL);
	}
	else {
		this->SetInfinityState(INFINITY_STATE_DONE_SUCCESS);
	}
	m_dwForcedTick = timeGetTime();

	// 2009-09-09 ~ 2010-01-27 by dhjin, 인피니티 - Success, Fail시 몬스터 초기화 
	INIT_MSG_WITH_BUFFER(MSG_FN_NPCSERVER_RESET_CHANGE_OBJECT, T_FN_NPCSERVER_RESET_CHANGE_OBJECT, pResetObjMsg, ResetObjSendBuf);
	pResetObjMsg->mapChann	= m_pInfinityMapChannel->GetMapChannelIndex();
	m_pInfinityMapChannel->m_pFieldIOCPMapChannel->Send2NPCServerByTCP(ResetObjSendBuf, MSG_SIZE(MSG_FN_NPCSERVER_RESET_CHANGE_OBJECT));
	INIT_MSG_WITH_BUFFER(MSG_FN_NPCSERVER_DELETE_MONSTER_IN_MAPCHANNEL, T_FN_NPCSERVER_DELETE_MONSTER_IN_MAPCHANNEL, pMonster, SendBuf2);
	pMonster->mapChann = m_pInfinityMapChannel->GetMapChannelIndex();
	pMonster->bAllFlag = TRUE;
	m_pInfinityMapChannel->m_pFieldIOCPMapChannel->Send2NPCServerByTCP(SendBuf2, MSG_SIZE(MSG_FN_NPCSERVER_DELETE_MONSTER_IN_MAPCHANNEL));

	g_pFieldGlobal->WriteSystemLogEX(TRUE, "[Notify] [Infinity][%I64d] CInfinityBase::Fin !! %s Success(%d) FinType(%d) \r\n", m_nInfinityCreateUID, GetMapString(m_pInfinityMapChannel->GetMapChannelIndex(), string()), i_bSuccess, i_finType);
}

BOOL CInfinityBase::CheckDestory() {
	// 파괴 체크
	mt_auto_lock mta(&m_mtVectPlayer);
	this->DeleteNullFISocPtrNoLock();
	if(0 >= this->GetPlayerListSize()) {
		return TRUE;
	}

	return FALSE;
}

BOOL CInfinityBase::CheckAllReady() {
	mtVectCFISoc::iterator itr = m_mtVectPlayer.begin();
	for(; itr != m_mtVectPlayer.end(); itr++) {
		if(NULL != *itr
			&& INFINITY_STATE_READY != (*itr)->m_InfinityPlayingInfo.InfinityState) {
				return FALSE;
		}
	}
	return TRUE;
}

BOOL CInfinityBase::CheckTenderItemSize() {
	if(1 <= m_mtTenderItemList.size()) {
		return TRUE;
	}
	return FALSE;
}


// void CInfinityBase::TenderProcessing()
// {
// 	// Tender 진행
// 	mt_auto_lock mtTender(&m_mtTenderItemList);
// 	if(FALSE == this->CheckTenderItemSize()) {
// 		return;
// 	}
// 
// 	mtvectTenderItemInfo::iterator itr = m_mtTenderItemList.begin();
// 	if(NULL == itr) {
// 		return;
// 	}
// 
// 	DWORD dwCurTick = timeGetTime();
// 	if(FALSE == itr->TenderIng) {
// 		// 새로운 입찰 진행
// 		INIT_MSG_WITH_BUFFER(MSG_FC_INFINITY_TENDER_START, T_FC_INFINITY_TENDER_START, pTenderStartSMsg, TenderStartBuf);
// 		pTenderStartSMsg->ItemFieldIndex	= itr->ItemFieldIndex;
// 		pTenderStartSMsg->TenderItemNum		= itr->TenderItemNum;
// 		pTenderStartSMsg->Count				= itr->TenderItemCount;
// 		this->m_pInfinityMapChannel->SendMessageToAllInChannel(TenderStartBuf, MSG_SIZE(MSG_FC_INFINITY_TENDER_START));
// 
// 		itr->TenderIng = TRUE;
// 		itr->TenderStartTick = dwCurTick;
// 		m_TenderUserInfoList.clear();
// 		m_TenderProcessingInfo.UserTenderCnt	= this->m_pInfinityMapChannel->GetNumClients();
// 		m_TenderProcessingInfo.GiveUpCnt		= 0;
// 		return;
// 	}
// 	else {
// 		// 기존 진행하던 입찰 아이템 진행
// 		
// 		// 모든 유저가 GiveUp인지 체크
// 		if(m_TenderProcessingInfo.GiveUpCnt >= m_TenderProcessingInfo.UserTenderCnt) {
// 			INIT_MSG_WITH_BUFFER(MSG_FC_INFINITY_TENDER_RESULT_ALLGIVEUP, T_FC_INFINITY_TENDER_RESULT_ALLGIVEUP, pTenderGiveUpSMsg, TenderGiveUpBuf);
// 			pTenderGiveUpSMsg->ItemFieldIndex	= itr->ItemFieldIndex;
// 			this->m_pInfinityMapChannel->SendMessageToAllInChannel(TenderGiveUpBuf, MSG_SIZE(MSG_FC_INFINITY_TENDER_RESULT_ALLGIVEUP));			
// 
// 			this->TenderFailProcessing(itr);
// 			itr = m_mtTenderItemList.erase(itr);
// 			return;
// 		}
// 		
// 		// 모든 유저 입찰 완료했는지 체크
// 		if(m_TenderProcessingInfo.UserTenderCnt <= m_TenderUserInfoList.size()) {
// 			// 모든 유저 입찰 했음.
// 			this->SendTenderWinResult(itr);
// 			itr = m_mtTenderItemList.erase(itr);
// 			return;
// 		}
// 
// 		// 입찰 시간 종료 했는지 체크
// 		DWORD dwPassTick = dwCurTick - itr->TenderStartTick;
// 		if(TICK_TENDER_PROCESSING_TERM < dwPassTick) {
// 			INIT_MSG_WITH_BUFFER(MSG_FC_INFINITY_TENDER_RESULT_TIMEOVER, T_FC_INFINITY_TENDER_RESULT_TIMEOVER, pTenderTimeOverSMsg, TenderTimeOverBuf);
// 			pTenderTimeOverSMsg->ItemFieldIndex	= itr->ItemFieldIndex;
// 			this->m_pInfinityMapChannel->SendMessageToAllInChannel(TenderTimeOverBuf, MSG_SIZE(MSG_FC_INFINITY_TENDER_RESULT_TIMEOVER));			
// 			
// 			if(1 <= m_TenderUserInfoList.size()) {
// 				// 입찰자가 있다 넣어주자.
// 				this->SendTenderWinResult(itr);
// 				itr = m_mtTenderItemList.erase(itr);
// 			}
// 			else{
// 				this->TenderFailProcessing(itr);
// 				itr = m_mtTenderItemList.erase(itr);
// 			}
// 			return;
// 		}
// 	}
// }

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2010-04-09 by cmkwon, 인피2차 추가 수정(단계별 보상 추가) - 
/// \author		cmkwon
/// \date		2010-04-12 ~ 2010-04-12
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CInfinityBase::TenderProcessing(void)
{// Tender 진행
	if(m_mtTenderItemList.empty()){			return;}

	// 2010-04-12 by cmkwon, 유저리스트를 가져온다
	mt_auto_lock mta(&m_mtVectPlayer);
	this->DeleteNullFISocPtrNoLock();
	
	VectCFISoc tmPlayerList;
	this->GetPlayerList(&tmPlayerList);
	mta.auto_unlock_cancel();

	mt_auto_lock mtTender(&m_mtTenderItemList);
	if(FALSE == this->CheckTenderItemSize())
	{
		return;
	}

	mtvectTenderItemInfo	 tmDisposedTenderList;
	mtvectTenderItemInfo::iterator itr(m_mtTenderItemList.begin());
	while(itr != m_mtTenderItemList.end())
	{

		if ( m_InfinityModeInfo.ModeTypeNum == INFINITY_MODE_DEFENCE )
		{	// 기지 방어 모드 - 엔딩 시네마 연출 중에는 입찰 아이템 타임 오버 처리 지연.
			if ( m_eInfinityState > INFINITY_STATE_PLAYING && m_eCinemaSkipState == INFINITY_SKIP_CINEMA_NONE )
			{
				itr++;
				continue;
			}
		}

		CTenderItemInfo *pTenderingInfo = &*itr;

		//DWORD dwPassTick = timeGetTime() - pTenderingInfo->m_TenderStartTick;
		ATUM_DATE_TIME CurrentTime(true);
		int iPassTick = CurrentTime.GetTimeDiffTimeInSeconds ( pTenderingInfo->m_TenderStartTime );

		//if(TICK_TENDER_PROCESSING_TERM + 1000 < dwPassTick)
		if ( (TICK_TENDER_PROCESSING_TERM + 1000) / 1000 < iPassTick )
		{
//			g_pFieldGlobal->WriteSystemLogEX(TRUE, "[TEMP] [Infinity][%I64d] CInfinityBase::TenderProcessing# 10000 !, Map(%s) Tender(ItemNum:%d ItemCnt:%d) \r\n"
//				, this->m_nInfinityCreateUID, GetMapString(m_pInfinityMapChannel->GetMapChannelIndex(), string()), pTenderingInfo->m_TenderItemNum, pTenderingInfo->m_TenderItemCount);

			pTenderingInfo->m_IsTimeOver	= TRUE;
			tmDisposedTenderList.push_back(*pTenderingInfo);

			itr = m_mtTenderItemList.erase(itr);
			continue;
		}

		if(pTenderingInfo->CheckAllUserChoice(&tmPlayerList))
		{
//			g_pFieldGlobal->WriteSystemLogEX(TRUE, "[TEMP] [Infinity][%I64d] CInfinityBase::TenderProcessing# 20000 !, Map(%s) Tender(ItemNum:%d ItemCnt:%d) \r\n"
//				, this->m_nInfinityCreateUID, GetMapString(m_pInfinityMapChannel->GetMapChannelIndex(), string()), pTenderingInfo->m_TenderItemNum, pTenderingInfo->m_TenderItemCount);

			tmDisposedTenderList.push_back(*pTenderingInfo);

			itr = m_mtTenderItemList.erase(itr);
			continue;
		}
		itr++;
	}
	mtTender.auto_unlock_cancel();

	
	itr = tmDisposedTenderList.begin();
	for(; itr != tmDisposedTenderList.end(); itr++)
	{

		CTenderItemInfo *pTenderingInfo = &*itr;
		if(FALSE == this->PickupTenderItem(pTenderingInfo, &tmPlayerList))
		{
			g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ERROR] [Infinity][%I64d] CInfinityBase::TenderProcessing# 30000 !, Map(%s) Tender(ItemNum:%d ItemCnt:%d) \r\n"
				, this->m_nInfinityCreateUID, GetMapString(m_pInfinityMapChannel->GetMapChannelIndex(), string()), pTenderingInfo->m_TenderItemNum, pTenderingInfo->m_TenderItemCount);
		}
	}
	tmDisposedTenderList.clear();
}


Err_t CInfinityBase::ChoiceTenderItem(DiceCnt_t *o_pDiceResult, ClientIndex_t i_UserClientIdx, UINT i_ItemFieldIndex, BOOL i_bGiveUp)
{// 유저 Tender
// 2010-04-09 by cmkwon, 인피2차 추가 수정(단계별 보상 추가) - 
// 	mt_auto_lock mtTender(&m_mtTenderItemList);
// 	mtvectTenderItemInfo::iterator itr = m_mtTenderItemList.begin();
// 	for(; itr != m_mtTenderItemList.end(); itr++) {
// 		if(i_ItemFieldIndex == itr->ItemFieldIndex) {
// 			if(FALSE == itr->TenderIng) {
// 				// 입찰 진행중이 아닌 품목이다.
// 				return ERR_INFINITY_NOT_PROCESSING_TENDERITEM;
// 			}
// 			
// 			DiceCnt_t DiceCnt = 0;
// 			if(FALSE == i_bGiveUp) {
// 				// 주사위 굴리기
// 				DiceCnt = this->CalcDiceCount();
// 			}
// 			else {
// 				// 포기
// 				m_TenderProcessingInfo.GiveUpCnt++;
// 			}
// 			
// 			// 값 입력 및 전송
// 			TENDER_USER_INFO TenderUserInfo;
// 			util::zero(&TenderUserInfo, sizeof(TENDER_USER_INFO));
// 			TenderUserInfo.ItemFieldIndex	= i_ItemFieldIndex;
// 			TenderUserInfo.UserClientIdx	= i_UserClientIdx;
// 			TenderUserInfo.DiceCount		= DiceCnt;
// 			TenderUserInfo.GiveUp			= i_bGiveUp;
// 			m_TenderUserInfoList.push_back(TenderUserInfo);
// 			
// 			INIT_MSG_WITH_BUFFER(MSG_FC_INFINITY_TENDER_PUT_IN_TENDER_OK, T_FC_INFINITY_TENDER_PUT_IN_TENDER_OK, pTenderPutInSMsg, TenderPutInBuf);
// 			pTenderPutInSMsg->ItemFieldIndex		= i_ItemFieldIndex;
// 			pTenderPutInSMsg->DiceUserIdx			= i_UserClientIdx;
// 			pTenderPutInSMsg->DiceCount				= DiceCnt;
// 			this->m_pInfinityMapChannel->SendMessageToAllInChannel(TenderPutInBuf, MSG_SIZE(MSG_FC_INFINITY_TENDER_PUT_IN_TENDER_OK));			
// 						
// 			return ERR_NO_ERROR;
// 		}
// 	}
	///////////////////////////////////////////////////////////////////////////////
	// 2010-04-09 by cmkwon, 인피2차 추가 수정(단계별 보상 추가) - 
	CFieldIOCPSocket *pFISoc = this->GetPlayerInVectPlayerByClientIndex(i_UserClientIdx);
	if(NULL == pFISoc
		|| FALSE == pFISoc->IsValidCharacter(FALSE))
	{
		return ERR_COMMON_NO_SUCH_CHARACTER;
	}

 	mt_auto_lock mtTender(&m_mtTenderItemList);
	CTenderItemInfo *pTenderItemInfo = this->GetTenderItemInfoNoLock(i_ItemFieldIndex);
	if(NULL == pTenderItemInfo)
	{
		return ERR_INFINITY_NOT_PROCESSING_TENDERITEM;
	}

	*o_pDiceResult		= 0;	// 0 이면 포기
	return pTenderItemInfo->PlayBidding(o_pDiceResult, i_UserClientIdx, i_bGiveUp);
}

void CInfinityBase::CalcLimitTimeByUserDeath(char * i_szCharacterName) {
	if(NULL == i_szCharacterName) {
		// 2009-09-09 ~ 2010-01 by dhjin, 인피니티 - 소스 체크
		return;
	}

	// start 2011-05-30 by hskim, 인피니티 3차 - 플레이 시간 재설정 기능 
	if( FALSE == IsPenaltyApply() )
	{
		return ;
	}
	// end 2011-05-30 by hskim, 인피니티 3차 - 플레이 시간 재설정 기능

	// 2009-09-09 ~ 2010-01-13 by dhjin, 인피니티 - 죽은 유저 이름 정보 전송 추가, // 인피니티 - 인피 사망시 패널티 추가
	EnterCriticalSection(&m_criticalLimitTime);
	DWORD dwCurTick = timeGetTime();
	DWORD dwPassTick = dwCurTick - m_InfinityCreatedTick;

	int CheckTime = (this->m_InfinityModeInfo.LimitTime - dwPassTick + m_InfinityLimitTick) + (this->m_InfinityModeInfo.TimePenaltyValue * 60 * 1000);
	if(0 > CheckTime) {
		LeaveCriticalSection(&m_criticalLimitTime);
		return;
	}

	m_InfinityLimitTick += (this->m_InfinityModeInfo.TimePenaltyValue * 60 * 1000);	// Min -> MSec 변경

	INIT_MSG_WITH_BUFFER(MSG_FC_INFINITY_CHANGE_LIMITTIME, T_FC_INFINITY_CHANGE_LIMITTIME, pSMsg, Buf);
	util::strncpy(pSMsg->DeadCharactarName, i_szCharacterName, SIZE_MAX_CHARACTER_NAME);		// 2009-09-09 ~ 2010-01-13 by dhjin, 인피니티 - 죽은 유저 이름 정보 전송 추가,
	pSMsg->LimitTime	= m_InfinityLimitTick;
	this->m_pInfinityMapChannel->SendMessageToAllInChannel(Buf, MSG_SIZE(MSG_FC_INFINITY_CHANGE_LIMITTIME));
	LeaveCriticalSection(&m_criticalLimitTime);
}

void CInfinityBase::CalcAliveForGameClearMonsterHPByUserDeath(char * i_szCharacterName)
{	// 2010-03-31 by dhjin, 인피니티(기지방어) -
	if(NULL == i_szCharacterName) {
		// 2009-09-09 ~ 2010-01 by dhjin, 인피니티 - 소스 체크
		return;
	}

	// start 2011-05-30 by hskim, 인피니티 3차 - 플레이 시간 재설정 기능 
	if( FALSE == IsPenaltyApply() )
	{
		return ;
	}
	// end 2011-05-30 by hskim, 인피니티 3차 - 플레이 시간 재설정 기능

	// 2010-03-31 by dhjin, 인피니티(기지방어) - 기지방어 모드는 몬스터 피를 감소시킨다.

	// start 2011-06-14 by hskim, 인피니티 3차 - 패널티 기능 추가 (HP 및 시간 동시 지원을 위해)
	//////////
	// 기존
	//this->m_pInfinityMapChannel->PenaltyKeyMonster_AliveForGameClear(i_szCharacterName, this->m_InfinityModeInfo.TimePenaltyValue);		

	//////////
	// 수정
	this->m_pInfinityMapChannel->PenaltyKeyMonster_AliveForGameClear(i_szCharacterName, this->m_InfinityModeInfo.HPPenaltyValue);		
	// end 2011-06-14 by hskim, 인피니티 3차 - 패널티 기능 추가 (HP 및 시간 동시 지원을 위해)
}

void CInfinityBase::LogInfinityFinAliveKeyMonster()	{
	// 2009-09-09 ~ 2010-01-13 by dhjin, 인피니티 - 유저 정보 제외 종료 관련 로그 여러번 남는 버그 한 번만 남게 수정 
	VectKeyMonsterInfo::iterator itrKeyMon = m_KeyMonsterDeadForNextStepList.begin();
	for(; itrKeyMon != m_KeyMonsterDeadForNextStepList.end(); itrKeyMon++) {
		if(TRUE == itrKeyMon->KeyMonsterAlive) {
			CAtumLogSender::SendLogMessageInfinityFinAliveKeyMonster(itrKeyMon->KeyMonsterIdx);
		}
	}
}

void CInfinityBase::LogInfinityFinUser(CFieldIOCPSocket * i_pPlayerSoc, BOOL i_bClear) 
{

	if(NULL == i_pPlayerSoc) 
	{
		// 2009-09-09 ~ 2010-01 by dhjin, 인피니티 - 소스 체크
		return;
	}

	// 2009-09-09 ~ 2010-01-13 by dhjin, 인피니티 - 유저 정보 제외 종료 관련 로그 여러번 남는 버그 한 번만 남게 수정 // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 게임로그 추가, 종료 관련 정보 
	CAtumLogSender::SendLogMessageInfinityFin(i_pPlayerSoc, i_bClear);
	
	mt_auto_lock igLock(&i_pPlayerSoc->m_ItemManager.m_mapItemGeneral);
	mtmapUID2ItemGeneral::iterator itr = i_pPlayerSoc->m_ItemManager.m_mapItemGeneral.begin();
	for(; itr != i_pPlayerSoc->m_ItemManager.m_mapItemGeneral.end(); itr++) {
		CAtumLogSender::SendLogMessageInfinityFinItem(i_pPlayerSoc, itr->second);
	}
	igLock.auto_unlock_cancel();

	i_pPlayerSoc->m_ItemManager.LogInfinityUsingItem();	
}

void CInfinityBase::LogInfinityLeave(CFieldIOCPSocket * i_pPlayerSoc) 
{

	if(NULL == i_pPlayerSoc) 
	{
		// 2009-09-09 ~ 2010-01 by dhjin, 인피니티 - 소스 체크
		return;
	}

	// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 게임로그 추가, 인피 탈퇴시 아이템 복사 정보
	mt_auto_lock igLock(&i_pPlayerSoc->m_ItemManager.m_mapItemGeneral);
	mtmapUID2ItemGeneral::iterator itr = i_pPlayerSoc->m_ItemManager.m_mapItemGeneral.begin();
	for(; itr != i_pPlayerSoc->m_ItemManager.m_mapItemGeneral.end(); itr++) {
		CAtumLogSender::SendLogMessageInfinityLeaveItem(i_pPlayerSoc, itr->second);
	}
	igLock.auto_unlock_cancel();

	i_pPlayerSoc->m_ItemManager.LogInfinityUsingItem();
}

void CInfinityBase::SendInfinityTeamChat(BYTE * i_pDATA, int i_nSize) 
{

	// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 채팅
	mt_auto_lock mta(&m_mtVectPlayer);
	this->DeleteNullFISocPtrNoLock();
	mtVectCFISoc::iterator itr = m_mtVectPlayer.begin();
	while(itr != m_mtVectPlayer.end()) {
		if(NULL != *itr) {
			(*itr)->SendAddData(i_pDATA, i_nSize);
		}
		itr++;
	}
	mta.auto_unlock_cancel();
}

void CInfinityBase::ReStartDisConnectUser(CFieldIOCPSocket * i_pUserFISoc) 
{
	// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 팅긴 유저 재접속 처리 고고씽
	
	// 1. 팀에 넣고
	// 2. 시작하고
	
	if(NULL == i_pUserFISoc) 
	{
		return;
	}

	// 좀 더 빠른 처리를 위해 몇 가지 정보를 저장한다.
	i_pUserFISoc->m_InfinityPlayingInfo.InfinityModeUID		= m_InfinityModeInfo.InfinityModeUID;
	i_pUserFISoc->m_InfinityPlayingInfo.ModeTypeNum			= m_InfinityModeInfo.ModeTypeNum;
	i_pUserFISoc->m_InfinityPlayingInfo.MapIdx				= m_InfinityModeInfo.MapIdx;
	i_pUserFISoc->m_InfinityPlayingInfo.TimePenaltyValue	= m_InfinityModeInfo.TimePenaltyValue;		// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 사망시 패널티 추가
	i_pUserFISoc->m_InfinityPlayingInfo.HPPenaltyValue		= m_InfinityModeInfo.HPPenaltyValue;		// 2011-06-14 by hskim, 인피니티 3차 - 패널티 기능 추가 (HP 및 시간 동시 지원을 위해)
	
// 2010-03-31 by dhjin, 클라이언트 요청으로 팅긴유저 재접속시에는 전송하지 않는다. 밑과 같이 처리
// 	INIT_MSG_WITH_BUFFER(MSG_FC_INFINITY_JOIN_OK, T_FC_INFINITY_JOIN_OK, pSMsg, SendBuf);
// 	pSMsg->bAccept				= TRUE;
// 	pSMsg->JoinClientIdx		= i_pUserFISoc->m_character.ClientIndex;
// 	util::strncpy(pSMsg->JoinCharacterName, i_pUserFISoc->m_character.CharacterName, SIZE_MAX_CHARACTER_NAME);
// 	pSMsg->Gear					= i_pUserFISoc->m_character.UnitKind;
// 	pSMsg->Lv					= i_pUserFISoc->m_character.Level;	
// 
// 	mt_auto_lock mta(&m_mtVectPlayer);
// 	this->DeleteNullFISocPtrNoLock();
// 	m_mtVectPlayer.push_back(i_pUserFISoc);
// 	mtVectCFISoc::iterator itr = m_mtVectPlayer.begin();
// 	for(; itr != m_mtVectPlayer.end(); itr++) {
// 		if(NULL != *itr) {
// 			// 파티 가입된 유저 정보 전송
// 			(*itr)->SendAddData(SendBuf, MSG_SIZE(MSG_FC_INFINITY_JOIN_OK));
// 		}
// 	}	
// 	mta.auto_unlock_cancel();	// 2009-09-09 ~ 2010-01 by dhjin, 인피니티 - 소스 체크
	m_mtVectPlayer.pushBackLock(i_pUserFISoc);

	i_pUserFISoc->SetInfinityMap(this->m_InfinityModeInfo.MapIdx, m_pInfinityMapChannel->GetMapChannelIndex().ChannelIndex);
	
	// 팅긴 유저 입장 시키자~
	INIT_MSG_WITH_BUFFER(MSG_FC_INFINITY_ENTER_BY_DISCONNECT, T_FC_INFINITY_ENTER_BY_DISCONNECT, pEnterSMsg, EnterSendBuf);
	pEnterSMsg->InfinityMode		= this->GetInfinityMode();
	pEnterSMsg->TimePenaltyValue	= this->m_InfinityModeInfo.TimePenaltyValue;
	pEnterSMsg->HPPenaltyValue		= this->m_InfinityModeInfo.HPPenaltyValue;		// 2011-06-14 by hskim, 인피니티 3차 - 패널티 기능 추가 (HP 및 시간 동시 지원을 위해)
	pEnterSMsg->InfinityChannelIdx	= m_pInfinityMapChannel->GetMapChannelIndex().ChannelIndex;
	pEnterSMsg->CharacterPosition	= i_pUserFISoc->m_character.PositionVector;
	pEnterSMsg->StartTime			= m_InfinityCreatedTime;
	pEnterSMsg->LimitTime			= this->m_InfinityModeInfo.LimitTime;
	pEnterSMsg->DecreaseTimeByDead	= this->m_InfinityLimitTick;
	pEnterSMsg->InfinityCreateUID	= this->GetInfinityCreateUID();
	pEnterSMsg->MasterClientIdx		= this->GetMasterPlayerClientIdx();
	i_pUserFISoc->SendAddData(EnterSendBuf, MSG_SIZE(MSG_FC_INFINITY_ENTER_BY_DISCONNECT));
	i_pUserFISoc->m_InfinityPlayingInfo.InfinityState = INFINITY_STATE_PLAYING;
	
	this->SendDeleteCinemaInfoList(i_pUserFISoc);

	// 2010-03-31 by dhjin, 인피니티(기지방어) -
	this->m_pInfinityMapChannel->SendKeyMonsterHP_AliveForGameClear(i_pUserFISoc);		

	CheckInfinityPartyState();		// 2013-05-16 by jhseol, 인피 재접속 시 파티지원
		
	i_pUserFISoc->StatisticsInfinityStart();		// 2012-01-16 by hskim, 통계 - 화패
		
	// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 게임로그 추가
	CAtumLogSender::SendLogMessageInfinityStart(i_pUserFISoc);	
}

void CInfinityBase::SetDeleteCinemaInfo(CINEMAINFO * i_pDeleteCinemaInfo) 
{
	if(NULL == i_pDeleteCinemaInfo) 
	{
		// 2009-09-09 ~ 2010-01 by dhjin, 인피니티 - 소스 체크
		return;
	}

	// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 팅긴 유저 재접속 처리
	m_DeleteCinemaInfoList.push_back(*i_pDeleteCinemaInfo);
}

void CInfinityBase::SendDeleteCinemaInfoList(CFieldIOCPSocket * i_pReStartPlayerSoc) 
{

	if(NULL == i_pReStartPlayerSoc) 
	{
		// 2009-09-09 ~ 2010-01 by dhjin, 인피니티 - 소스 체크
		return;
	}

	// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 팅긴 유저 재접속 처리
	BYTE SendBuf[SIZE_MAX_PACKET];
	int			nSendBytes		= 0;
	*(MessageType_t*)(SendBuf)	= T_FC_INFINITY_DELETED_CINEMA_HEADER;
	nSendBytes					= SIZE_FIELD_TYPE_HEADER;
	
	vectorCinemaInfo::iterator itrDeleteCinemaInfo = this->m_DeleteCinemaInfoList.begin();
	for (; itrDeleteCinemaInfo != this->m_DeleteCinemaInfoList.end(); itrDeleteCinemaInfo++) 
	{
		if(nSendBytes + MSG_SIZE(MSG_FC_INFINITY_DELETED_CINEMA) > SIZE_MAX_PACKET) 
		{
			i_pReStartPlayerSoc->SendAddData(SendBuf, nSendBytes);
			nSendBytes					= 0;
		}
		
		*((MessageType_t*)(SendBuf + nSendBytes))	= T_FC_INFINITY_DELETED_CINEMA;
		nSendBytes									+= SIZE_FIELD_TYPE_HEADER;
		MSG_FC_INFINITY_DELETED_CINEMA *pRMsg		= (MSG_FC_INFINITY_DELETED_CINEMA*)(SendBuf + nSendBytes);
		*pRMsg										= *itrDeleteCinemaInfo;
		nSendBytes									+= sizeof(MSG_FC_INFINITY_DELETED_CINEMA);
	}
	
	if(nSendBytes > 0) 
	{
		i_pReStartPlayerSoc->SendAddData(SendBuf, nSendBytes);
		nSendBytes						= 0;
	}
	
	i_pReStartPlayerSoc->SendAddMessageType(T_FC_INFINITY_DELETED_CINEMA_DONE);	
}

void CInfinityBase::AutoPartyCreate() 
{
	// 2009-09-09 ~ 2010-01-26 by dhjin, 인피니티 - 자동 편대 
	if(NULL == this->m_MasterPlayerSoc) 
	{
		return;
	}

	INIT_MSG_WITH_BUFFER(MSG_FI_PARTY_AUTO_CREATE, T_FI_PARTY_AUTO_CREATE, pSendMsg, SendBuf);
	int	PartyMemberListCount = 0;
	UID32_t * PartyMemberUID = (UID32_t*)(SendBuf+MSG_SIZE(MSG_FI_PARTY_AUTO_CREATE));
	mtVectCFISoc::iterator itr = m_mtVectPlayer.begin(); // 함수 호출 전에 락을 잡았다.
	for(; itr != m_mtVectPlayer.end(); itr++) 
	{
		if(NULL != *itr)
		{
			// 2010-04-02 by cmkwon, 인피2차 추가 수정 - 자동 파티 버그 수정, 최대 6명만 파티가능
			if(SIZE_MAX_PARTY_MEMBER-1 <= PartyMemberListCount)
			{
				break;
			}

			if(this->m_MasterPlayerSoc == *itr) 
			{
				// 방장
				pSendMsg->MasterUID		= (*itr)->m_character.CharacterUniqueNumber;
			}
			else 
			{
				// 파티원
				PartyMemberUID[PartyMemberListCount] = (*itr)->m_character.CharacterUniqueNumber;
				PartyMemberListCount++;
			}
		}
	}
	pSendMsg->PartyMemberListCount	= PartyMemberListCount;

	if(NULL != this->m_MasterPlayerSoc) 
	{
		this->m_MasterPlayerSoc->ms_pFieldIOCP->m_pIMWinSocket->Write(SendBuf, MSG_SIZE(MSG_FI_PARTY_AUTO_CREATE) + PartyMemberListCount * sizeof(UID32_t));
	}
}

void CInfinityBase::SendAddDataPlayerList(BYTE* pData, int nSize) 
{ // 2010-03-31 by dhjin, 인피니티(기지방어) -
	mt_auto_lock mta(&m_mtVectPlayer);
	this->DeleteNullFISocPtrNoLock();

	vectCFieldIOCPSocket tmPlayerList;
	this->GetPlayerList(&tmPlayerList);
	mta.auto_unlock_cancel();

	vectCFieldIOCPSocket::iterator itr(tmPlayerList.begin());
	for(; itr != tmPlayerList.end(); itr++)
	{
		CFieldIOCPSocket *pFISoc = *itr;
		if(NULL == pFISoc
			|| FALSE == pFISoc->IsValidCharacter(FALSE))
		{
			continue;
		}
		pFISoc->SendAddData(pData, nSize);
	}
}

void CInfinityBase::ProcessingCinemaFinSuccess() 
{	// 2010-03-31 by dhjin, 인피니티(기지방어) - 성공 종료 연출 진행
	DWORD dwCurTick = timeGetTime();
	DWORD dwPassTick = dwCurTick - m_dwForcedTick;

	vectorCinemaInfo::iterator itrCinema = m_FinSuccessCinemaInfoList.begin();
	while(itrCinema != m_FinSuccessCinemaInfoList.end())
	{
		CINEMAINFO *pCinemaInfo = &*itrCinema;
		if(dwPassTick >= pCinemaInfo->StartTime)
		{
			//g_pFieldGlobal->WriteSystemLogEX(TRUE, "[TEMP] [Infinity][%I64d] CInfinityBase::ProcessingCinemaFinSuccess# ! CinemaNum(%d) Order(%d) StartTime(%d) ObjIdx(%d) ChangeObjectNum(%d) BodyObjIdx(%d) ChangeBody(%I64d) SoundFile(%s) \r\n"
			//	, m_nInfinityCreateUID, pCinemaInfo->CinemaNum, pCinemaInfo->CinemaOrder, pCinemaInfo->StartTime, pCinemaInfo->ObjectIdx, pCinemaInfo->ChangeObjectIdx
			//	, pCinemaInfo->ObjectBodyConditionIdx, pCinemaInfo->BodyCondition, pCinemaInfo->SoundFileName);

			// 시네마 걸렸어~!! 정보 처리 후 지우기
			INIT_MSG_WITH_BUFFER(MSG_FC_INFINITY_CINEMA, T_FC_INFINITY_CINEMA, pSMsg, SendBuf);
			*pSMsg = *pCinemaInfo;	
			this->SendAddDataPlayerList(SendBuf, MSG_SIZE(MSG_FC_INFINITY_CINEMA));
			CAtumLogSender::SendLogMessageInfinityCinema(pCinemaInfo, this->GetInfinityCreateUID());			// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 게임로그 추가, 연출 관련 로그
			itrCinema = m_FinSuccessCinemaInfoList.erase(itrCinema);	
			continue;
		}
		itrCinema++;
	}
}

void CInfinityBase::ProcessingCinemaFinFail()
{	// 2010-03-31 by dhjin, 인피니티(기지방어) - 실패 종료 연출 진행
	DWORD dwCurTick = timeGetTime();
	DWORD dwPassTick = dwCurTick - m_dwForcedTick;
	
	vectorCinemaInfo::iterator itrCinema = m_FinFailCinemaInfoList.begin();
	while(itrCinema != m_FinFailCinemaInfoList.end())
	{
		CINEMAINFO *pCinemaInfo = &*itrCinema;

		if(dwPassTick >= pCinemaInfo->StartTime)
		{
			//g_pFieldGlobal->WriteSystemLogEX(TRUE, "[TEMP] [Infinity][%I64d] CInfinityBase::ProcessingCinemaFinFail# ! CinemaNum(%d) Order(%d) StartTime(%d) ObjIdx(%d) ChangeObjectNum(%d) BodyObjIdx(%d) ChangeBody(%I64d) SoundFile(%s) \r\n"
			//	, m_nInfinityCreateUID, pCinemaInfo->CinemaNum, pCinemaInfo->CinemaOrder, pCinemaInfo->StartTime, pCinemaInfo->ObjectIdx, pCinemaInfo->ChangeObjectIdx
			//	, pCinemaInfo->ObjectBodyConditionIdx, pCinemaInfo->BodyCondition, pCinemaInfo->SoundFileName);

			// start 2011-06-06 by hskim, 인피니티 3차 - 연출 관련 이동 구현 (추후 시네마 파일로 변경시 삭제 예정)
			if( itrCinema->CustomIdx )
			{
				switch( itrCinema->CustomIdx )
				{
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
			// end 2011-06-06 by hskim, 인피니티 3차 - 연출 관련 이동 구현 (추후 시네마 파일로 변경시 삭제 예정)

			// 시네마 걸렸어~!! 정보 처리 후 지우기
			INIT_MSG_WITH_BUFFER(MSG_FC_INFINITY_CINEMA, T_FC_INFINITY_CINEMA, pSMsg, SendBuf);
			*pSMsg = *pCinemaInfo;	
			this->SendAddDataPlayerList(SendBuf, MSG_SIZE(MSG_FC_INFINITY_CINEMA));
			CAtumLogSender::SendLogMessageInfinityCinema(pCinemaInfo, this->GetInfinityCreateUID());			// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 게임로그 추가, 연출 관련 로그
			itrCinema = m_FinFailCinemaInfoList.erase(itrCinema);	
			continue;
		} 
        itrCinema++;
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CInfinityBase::DoSecondlyWorkInfinityBaseIsContinue()
/// \brief		인피니티(기지방어) - 공통된 루틴 빼기
/// \author		dhjin
/// \date		2010-03-31
/// \warning	
///
/// \param		
/// \return		TRUE : 초 틱 계속 수행, FALSE : 초 틱 수행 멈춤
///////////////////////////////////////////////////////////////////////////////
BOOL CInfinityBase::DoSecondlyWorkInfinityBaseIsContinue()
{	// 2010-03-31 by dhjin, 인피니티(기지방어) - 공통된 루틴 빼기
	// 입찰 체크
	this->TenderProcessing();
	
	// start 2011-04-08 by hskim, 인피니티 3차 - 시네마 단계별 기능 구현
	// 기존
	//if(INFINITY_STATE_PLAYING != m_eInfinityState) 

	// 수정
	if( TRUE != IS_INFINITY_STATE_PLAYING(m_eInfinityState) )		// 2011-04-08 by hskim, 인피니티 3차 - 시네마 단계별 기능 구현
	// end 2011-04-08 by hskim, 인피니티 3차 - 시네마 단계별 기능 구현
	{
		// 2010-03-31 by dhjin, 인피니티(기지방어) -
		if(INFINITY_STATE_DONE_SUCCESS == m_eInfinityState) 
		{
			// 성공 종료 연출 처리
			this->ProcessingCinemaFinSuccess();
		}
		else if(INFINITY_STATE_DONE_FAIL == m_eInfinityState) 
		{
			// 성공 종료 연출 처리
			this->ProcessingCinemaFinFail();
		}
		else if(INFINITY_STATE_READY_ENTER_CHECK == this->GetInfinityState())
		{
			DWORD dwCurTick = timeGetTime();
			if(30000 < dwCurTick - m_dwTickChangedInfinityState)
			{
				this->CreateMap(NULL);
			}
		}
		return FALSE;
	}

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2010-03-23 by cmkwon, 인피니티 입장 캐쉬 아이템 구현 - 
/// \author		cmkwon
/// \date		2010-03-23 ~ 2010-03-23
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
Err_t CInfinityBase::SendFtoA_INFINITY_START_CHECK(void)
{
	mt_auto_lock mta(&m_mtVectPlayer);
	mtVectCFISoc::iterator itr = m_mtVectPlayer.begin();
	for(; itr != m_mtVectPlayer.end(); itr++)
	{
		CFieldIOCPSocket *pFISoc = *itr;
		if(NULL == pFISoc
			|| FALSE == pFISoc->IsValidCharacter())
		{
			return ERR_INFINITY_JOIN_FAIL_SOCKET;
		}

//		g_pFieldGlobal->WriteSystemLogEX(TRUE, "[TEMP] [Infinity][%I64d] CInfinityBase::SendFtoA_INFINITY_START_CHECK# ! %s %d<-%d \r\n"
//			, this->m_nInfinityCreateUID, GetCharacterString(pFISoc->GetCharacter(), string()), INFINITY_STATE_READY_ENTER_CHECK, pFISoc->m_InfinityPlayingInfo.InfinityState);

		INIT_MSG_WITH_BUFFER(MSG_FtoA_INFINITY_START_CHECK, T_FtoA_INFINITY_START_CHECK, pSMsg, SendBuf);
		pSMsg->AccountUID			= pFISoc->GetCharacter()->AccountUniqueNumber;
		pSMsg->MFSCharacterUID		= pFISoc->m_AfsNeedMfsInfo.MFSCharacterUID;
		pSMsg->MFSClientIndex		= pFISoc->m_AfsNeedMfsInfo.MFSClientIdx;
		pSMsg->AFSClientIndex		= pFISoc->GetCharacter()->ClientIndex;
		pSMsg->bCheckReentryTicket	= pFISoc->m_InfinityPlayingInfo.bHaveReentryTicket;
		pSMsg->InfinityMode			= this->m_InfinityModeInfo.ModeTypeNum;
		pSMsg->InfinityCreateUID	= this->m_nInfinityCreateUID;
		if(FALSE == pFISoc->InfinitySendAddDataFromAFS2MFS(SendBuf, MSG_SIZE(MSG_FtoA_INFINITY_START_CHECK)))
		{
			return ERR_INFINITY_JOIN_FAIL_SOCKET;
		}
		pFISoc->m_InfinityPlayingInfo.InfinityState = INFINITY_STATE_READY_ENTER_CHECK;
	}

	this->SetInfinityState(INFINITY_STATE_READY_ENTER_CHECK);

	return ERR_NO_ERROR;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2010-03-23 by cmkwon, 인피니티 입장 캐쉬 아이템 구현 - 
/// \author		cmkwon
/// \date		2010-03-23 ~ 2010-03-23
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CInfinityBase::CheckInfinityStartCheckAck(eINFINITY_STATE *o_pInfiRoomState, MSG_FtoA_INFINITY_START_CHECK_ACK *i_pStartCheckAck)
{
	mt_auto_lock mta(&m_mtVectPlayer);
	this->DeleteNullFISocPtrNoLock();

	CFieldIOCPSocket *pFISoc = this->GetPlayerInVectPlayerByClientIndex(i_pStartCheckAck->AFSClientIndex);
	if(NULL == pFISoc
		|| FALSE == pFISoc->IsValidCharacter(FALSE)
		|| i_pStartCheckAck->AccountUID != pFISoc->GetCharacter()->AccountUniqueNumber)
	{
		*o_pInfiRoomState	= this->m_eInfinityState;
		return (this->GetPlayerListSize() == this->GetPlayerStateCount(INFINITY_STATE_READY) + this->GetPlayerStateCount(INFINITY_STATE_READY_ENTER_CHECKED));
	}

	if(ERR_NO_ERROR != i_pStartCheckAck->ErrorCode)
	{
		this->SetInfinityState(INFINITY_STATE_READY);
		pFISoc->m_InfinityPlayingInfo.InfinityState	= INFINITY_STATE_READY;

		*o_pInfiRoomState	= this->m_eInfinityState;
		return (this->GetPlayerListSize() == this->GetPlayerStateCount(INFINITY_STATE_READY) + this->GetPlayerStateCount(INFINITY_STATE_READY_ENTER_CHECKED));
	}
	
	pFISoc->m_InfinityPlayingInfo.InfinityState	= INFINITY_STATE_READY_ENTER_CHECKED;

	*o_pInfiRoomState	= this->m_eInfinityState;
	return (this->GetPlayerListSize() == this->GetPlayerStateCount(INFINITY_STATE_READY) + this->GetPlayerStateCount(INFINITY_STATE_READY_ENTER_CHECKED));
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2010-03-23 by cmkwon, 인피니티 입장 캐쉬 아이템 구현 - 
/// \author		cmkwon
/// \date		2010-03-23 ~ 2010-03-23
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CInfinityBase::CheckInfinityAllPlayerState(eINFINITY_STATE i_infiState)
{
	mt_auto_lock mta(&m_mtVectPlayer);
	mtVectCFISoc::iterator itr = m_mtVectPlayer.begin();
	for(; itr != m_mtVectPlayer.end(); itr++)
	{
		CFieldIOCPSocket *pFISoc = *itr;

//		g_pFieldGlobal->WriteSystemLogEX(TRUE, "[TEMP] [Infinity][%I64d] CInfinityBase::CheckInfinityAllPlayerState# !, %s PlayerInfiState(%d) CheckInfiState(%d) Cnt(%d) \r\n"
//			, this->m_nInfinityCreateUID, GetCharacterString(pFISoc->GetCharacter(), string()), pFISoc->m_InfinityPlayingInfo.InfinityState, i_infiState, m_mtVectPlayer.size());

		if(NULL == pFISoc
			|| FALSE == pFISoc->IsValidCharacter(FALSE)
			|| i_infiState != pFISoc->m_InfinityPlayingInfo.InfinityState)
		{
			return FALSE;
		}

	}

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2010-03-23 by cmkwon, 인피니티 입장 캐쉬 아이템 구현 - 
/// \author		cmkwon
/// \date		2010-03-23 ~ 2010-03-23
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
CFieldIOCPSocket *CInfinityBase::GetPlayerInVectPlayerByClientIndex(ClientIndex_t i_cliIdx)
{
	mt_auto_lock mta(&m_mtVectPlayer);
	mtVectCFISoc::iterator itr = m_mtVectPlayer.begin();
	for(; itr != m_mtVectPlayer.end(); itr++)
	{
		CFieldIOCPSocket *pFISoc = *itr;
		if(pFISoc
			&& pFISoc->IsValidCharacter(FALSE)
			&& i_cliIdx == pFISoc->GetCharacter()->ClientIndex)
		{
			return pFISoc;
		}
	}

	return NULL;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2010-03-23 by cmkwon, 인피니티 입장 캐쉬 아이템 구현 - 
/// \author		cmkwon
/// \date		2010-03-23 ~ 2010-03-23
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CInfinityBase::SetAllPlayerState(eINFINITY_STATE i_infiState)
{
	mt_auto_lock mta(&m_mtVectPlayer);
	this->DeleteNullFISocPtrNoLock();
	mtVectCFISoc::iterator itr = m_mtVectPlayer.begin();
	for(; itr != m_mtVectPlayer.end(); itr++)
	{
		CFieldIOCPSocket *pFISoc = *itr;
		pFISoc->m_InfinityPlayingInfo.InfinityState = i_infiState;
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2010-04-05 by cmkwon, 인피 재입장 카드 관련 시스템 수정 - 
/// \author		cmkwon
/// \date		2010-04-05 ~ 2010-04-05
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CInfinityBase::ImputeInfinity(void)
{
	mt_auto_lock mta(&m_mtVectPlayer);

	if(m_mtVectPlayer.empty())
	{
		return FALSE;
	}

	vectCFieldIOCPSocket tmPlayerList;
	this->GetPlayerList(&tmPlayerList);
	mta.auto_unlock_cancel();
	
	vectCFieldIOCPSocket::iterator itr(tmPlayerList.begin());
	for(; itr != tmPlayerList.end(); itr++)
	{
		CFieldIOCPSocket *pFISoc = *itr;
		if(NULL == pFISoc
			|| FALSE == pFISoc->IsUsing())
		{
			if(pFISoc)
			{
				g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ERROR] [Infinity][%I64d] Client socket is invalid !!, %s \r\n", m_nInfinityCreateUID, GetCharacterString(pFISoc->GetCharacter(), string()));
			}
			else
			{
				g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ERROR] [Infinity][%I64d] Client socket is invalid !!, pFISoc(0x%X) \r\n", m_nInfinityCreateUID, pFISoc);
			}
			continue;
		}

		pFISoc->InfinityAddImpute(this->m_InfinityModeInfo.InfinityModeUID);

	 	INIT_MSG_WITH_BUFFER(MSG_FtoA_INFINITY_IMPUTE, T_FtoA_INFINITY_IMPUTE, pSMsg, SendBuf);
		pSMsg->AccountUID			= pFISoc->m_character.AccountUniqueNumber;
		pSMsg->MFSCharacterUID		= pFISoc->m_AfsNeedMfsInfo.MFSCharacterUID;
		pSMsg->MFSClientIndex		= pFISoc->m_AfsNeedMfsInfo.MFSClientIdx;
		pSMsg->InfinityModeUID		= this->m_InfinityModeInfo.InfinityModeUID;
		pSMsg->AFSClientIndex		= pFISoc->m_character.ClientIndex;
		pFISoc->InfinitySendAddDataFromAFS2MFS(SendBuf, MSG_SIZE(MSG_FtoA_INFINITY_IMPUTE));
	}

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2010-04-06 by cmkwon, 인피2차 추가 수정 - 
/// \author		cmkwon
/// \date		2010-04-06 ~ 2010-04-06
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int CInfinityBase::GetPlayerStateCount(eINFINITY_STATE i_infiState)
{
	mt_auto_lock mta(&m_mtVectPlayer);
	this->DeleteNullFISocPtrNoLock();

	int nRetCnt = 0;
	mtVectCFISoc::iterator itr = m_mtVectPlayer.begin();
	for(; itr != m_mtVectPlayer.end(); itr++)
	{
		CFieldIOCPSocket *pFISoc = *itr;
		if(i_infiState == pFISoc->m_InfinityPlayingInfo.InfinityState)
		{
			nRetCnt++;
		}
	}
	return nRetCnt;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2010-04-09 by cmkwon, 인피2차 추가 수정(단계별 보상 추가) - 
/// \author		cmkwon
/// \date		2010-04-09 ~ 2010-04-09
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
KEY_MONSTER_INFO *CInfinityBase::GetKeyMonsterInfoFromKeyMonsterDeadForNextStep(MonIdx_t i_MonNum)
{
	VectKeyMonsterInfo::iterator itr(m_KeyMonsterDeadForNextStepList.begin());
	for(; itr != m_KeyMonsterDeadForNextStepList.end(); itr++) 
	{
		KEY_MONSTER_INFO *pKeyMonInfo = &*itr;
		if(pKeyMonInfo->KeyMonsterIdx == i_MonNum) 
		{			
			return pKeyMonInfo;
		}
	}
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2010-04-09 by cmkwon, 인피2차 추가 수정(단계별 보상 추가) - 
/// \author		cmkwon
/// \date		2010-04-09 ~ 2010-04-09
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CInfinityBase::DeleteKeyMonsterInfoFromKeyMonsterDeadForNextStep(MonIdx_t i_MonNum)
{
	VectKeyMonsterInfo::iterator itr(m_KeyMonsterDeadForNextStepList.begin());
	for(; itr != m_KeyMonsterDeadForNextStepList.end(); itr++) 
	{
		KEY_MONSTER_INFO *pKeyMonInfo = &*itr;
		if(pKeyMonInfo->KeyMonsterIdx == i_MonNum) 
		{
			m_KeyMonsterDeadForNextStepList.erase(itr);
			return TRUE;
		}
	}
	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2010-04-09 by cmkwon, 인피2차 추가 수정(단계별 보상 추가) - 
/// \author		cmkwon
/// \date		2010-04-12 ~ 2010-04-12
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
CTenderItemInfo *CInfinityBase::GetTenderItemInfoNoLock(UINT i_ItemFieldIndex)
{
	mtvectTenderItemInfo::iterator itr(m_mtTenderItemList.begin());
	for(; itr != m_mtTenderItemList.end(); itr++)
	{
		CTenderItemInfo *pTenderItemInfo = &*itr;
		if(i_ItemFieldIndex == pTenderItemInfo->m_ItemFieldIndex)
		{
			return pTenderItemInfo;
		}
	}

	return NULL;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2010-04-09 by cmkwon, 인피2차 추가 수정(단계별 보상 추가) - 
/// \author		cmkwon
/// \date		2010-04-12 ~ 2010-04-12
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CInfinityBase::PickupTenderItem(CTenderItemInfo *i_pTenderItemInfo, VectCFISoc *i_pPlayerList)
{
	if(NULL == m_pInfinityMapChannel)
	{
		return FALSE;
	}

	if(i_pTenderItemInfo->m_IsTimeOver)
	{
		VectCFISoc::iterator itr(i_pPlayerList->begin());
		for(; itr != i_pPlayerList->end(); itr++)
		{
			CFieldIOCPSocket *pPlayerFISoc = *itr;
			if(NULL == pPlayerFISoc
				|| FALSE == pPlayerFISoc->IsValidCharacter(FALSE)
				|| NULL != i_pTenderItemInfo->GetBidderInfo(pPlayerFISoc->GetCharacter()->ClientIndex))
			{
				continue;
			}

			// 2010-06-25 by shcho, 인피니티 관련로그 찍기 - 시간으로 입찰포기 되었을 때 로그추가
			g_pFieldGlobal->WriteSystemLogEX(TRUE, "[Infinity][%I64d] CInfinityBase::PickupTenderItem# | TIMEOVER GIVE UP TENDER!! |, Map(%s) Character(%s) ItemNum(%d)\r\n"
				, this->m_nInfinityCreateUID, GetMapString(m_pInfinityMapChannel->GetMapChannelIndex(), string()), GetCharacterString(pPlayerFISoc->GetCharacter(),string()), i_pTenderItemInfo->m_TenderItemNum);
			// 2010-06-25 by shcho, 인피니티 관련로그 찍기 - 시간으로 입찰포기 되었을 때 로그추가
			
			// 해당 유저에게 TimeOver 메시지 전달
			INIT_MSG_WITH_BUFFER(MSG_FC_INFINITY_TENDER_RESULT_TIMEOVER, T_FC_INFINITY_TENDER_RESULT_TIMEOVER, pTenderTimeOverSMsg, TenderTimeOverBuf);
			pTenderTimeOverSMsg->ItemFieldIndex	= i_pTenderItemInfo->m_ItemFieldIndex;
			pPlayerFISoc->SendAddData(TenderTimeOverBuf, MSG_SIZE(MSG_FC_INFINITY_TENDER_RESULT_TIMEOVER));

			// 맵의 모든 유저에게 해당 유저 포기 정보 전달
			INIT_MSG_WITH_BUFFER(MSG_FC_INFINITY_TENDER_PUT_IN_TENDER_OK, T_FC_INFINITY_TENDER_PUT_IN_TENDER_OK, pTenderPutInSMsg, TenderPutInBuf);
			pTenderPutInSMsg->ItemFieldIndex		= i_pTenderItemInfo->m_ItemFieldIndex;
			pTenderPutInSMsg->DiceUserIdx			= pPlayerFISoc->GetCharacter()->ClientIndex;
			pTenderPutInSMsg->DiceCount				= 0;		// 0이면 포기
			m_pInfinityMapChannel->SendMessageToAllInChannel(TenderPutInBuf, MSG_SIZE(MSG_FC_INFINITY_TENDER_PUT_IN_TENDER_OK));
		}
	}

	BIDDER_INFO *pSuccessfullBidder = i_pTenderItemInfo->FindSuccessfulBidder();
	if(NULL == pSuccessfullBidder)
	{
		// 시간 경과로 
		INIT_MSG_WITH_BUFFER(MSG_FC_INFINITY_TENDER_RESULT_ALLGIVEUP, T_FC_INFINITY_TENDER_RESULT_ALLGIVEUP, pSMsg, SendBuf);
		pSMsg->ItemFieldIndex	= i_pTenderItemInfo->m_ItemFieldIndex;
		m_pInfinityMapChannel->SendMessageToAllInChannel(SendBuf, MSG_SIZE(MSG_FC_INFINITY_TENDER_RESULT_ALLGIVEUP));

		if(FALSE == this->CancelTenderItemInfo(i_pTenderItemInfo))
		{
			g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ERROR] [Infinity][%I64d] CInfinityBase::PickupTenderItem# 10000 !, Map(%s) Tender(ItemNum:%d ItemCnt:%d) \r\n"
				, this->m_nInfinityCreateUID, GetMapString(m_pInfinityMapChannel->GetMapChannelIndex(), string()), i_pTenderItemInfo->m_TenderItemNum, i_pTenderItemInfo->m_TenderItemCount);
		}
		return TRUE;
	}

	CFieldIOCPSocket *pBidderFISoc = this->GetPlayerInVectPlayerByClientIndex(pSuccessfullBidder->UserClientIdx);
	if(NULL == pBidderFISoc
		|| FALSE == pBidderFISoc->IsValidCharacter(FALSE))
	{
		if(FALSE == this->CancelTenderItemInfo(i_pTenderItemInfo))
		{
			g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ERROR] [Infinity][%I64d] CInfinityBase::PickupTenderItem# 20000 !, Map(%s) Tender(ItemNum:%d ItemCnt:%d) \r\n"
				, this->m_nInfinityCreateUID, GetMapString(m_pInfinityMapChannel->GetMapChannelIndex(), string()), i_pTenderItemInfo->m_TenderItemNum, i_pTenderItemInfo->m_TenderItemCount);
		}
		return TRUE;
	}

	CMapBlock *pTmpMapBlock	= m_pInfinityMapChannel->GetBlock(i_pTenderItemInfo->m_TenderItemPosition.x, i_pTenderItemInfo->m_TenderItemPosition.z);
	if (pTmpMapBlock == NULL)
	{
		g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ERROR] [Infinity][%I64d] CInfinityBase::PickupTenderItem# 30000 !, Map(%s) Tender(ItemNum:%d ItemCnt:%d) \r\n"
			, this->m_nInfinityCreateUID, GetMapString(m_pInfinityMapChannel->GetMapChannelIndex(), string()), i_pTenderItemInfo->m_TenderItemNum, i_pTenderItemInfo->m_TenderItemCount);
		return FALSE;
	}
	DROPITEM *pDropItem		= pTmpMapBlock->GetDROPITEM_SetProcess((ITEM_GENERAL*)i_pTenderItemInfo->m_ItemFieldIndex, TRUE);
	if(NULL == pDropItem)
	{
		g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ERROR] [Infinity][%I64d] CInfinityBase::PickupTenderItem# 40000 !, Map(%s) Tender(ItemNum:%d ItemCnt:%d) \r\n"
			, this->m_nInfinityCreateUID, GetMapString(m_pInfinityMapChannel->GetMapChannelIndex(), string()), i_pTenderItemInfo->m_TenderItemNum, i_pTenderItemInfo->m_TenderItemCount);
		return FALSE;
	}

	if(FALSE == pBidderFISoc->m_ItemManager.InsertItemGeneralByPointer(pDropItem->pItem, IUT_DROP_ITEM))
	{
		pDropItem->bTendering	= FALSE;
		pDropItem->bProcessing	= FALSE;
		pBidderFISoc->SendErrorMessage(T_FC_ITEM_GET_ITEM, ERR_PROTOCOL_INVENTORY_FULL_ITEM, 0, pBidderFISoc->m_ItemManager.GetMaxInventoryCount());
		return TRUE;
	}

	// file 로그 처리임
//	g_pFieldGlobal->WriteSystemLogEX(TRUE, "[TEMP] [Infinity][%I64d] CInfinityBase::PickupTenderItem# Map(%s) %s get Item! ItemNum(%d) ItemCnt(%d) Dice(%d) \r\n"
//		, this->m_nInfinityCreateUID, GetMapString(m_pInfinityMapChannel->GetMapChannelIndex(), string()), GetCharacterString(pBidderFISoc->GetCharacter(), string())
//		, i_pTenderItemInfo->m_TenderItemNum, i_pTenderItemInfo->m_TenderItemCount, pSuccessfullBidder->DiceCount);

	// 2010-06-25 by shcho, 인피니티 관련로그 찍기 - 습득 아이템 정보 DB저장
	SendInfinityItemTender(pBidderFISoc, pSuccessfullBidder->DiceCount, i_pTenderItemInfo->m_TenderItemNum);
	// END 2010-06-25 by shcho, 인피니티 관련로그 찍기 -

	m_pInfinityMapChannel->DeleteDropItem(i_pTenderItemInfo->m_TenderItemPosition, (ITEM_GENERAL*)i_pTenderItemInfo->m_ItemFieldIndex);	

	// 최고 주사위 입찰자 정보 전송 및 아이템 넣어주기
	INIT_MSG_WITH_BUFFER(MSG_FC_INFINITY_TENDER_RESULT, T_FC_INFINITY_TENDER_RESULT, pTenderResultSMsg, TenderResultBuf);
	pTenderResultSMsg->ItemFieldIndex		= i_pTenderItemInfo->m_ItemFieldIndex;
	pTenderResultSMsg->TenderItemNum		= i_pTenderItemInfo->m_TenderItemNum;
	pTenderResultSMsg->Count				= i_pTenderItemInfo->m_TenderItemCount;
	pTenderResultSMsg->DiceCount			= pSuccessfullBidder->DiceCount;
	pTenderResultSMsg->DiceUserIdx			= pSuccessfullBidder->UserClientIdx;
	m_pInfinityMapChannel->SendMessageToAllInChannel(TenderResultBuf, MSG_SIZE(MSG_FC_INFINITY_TENDER_RESULT));
	
	INIT_MSG_WITH_BUFFER(MSG_FC_ITEM_HIDE_ITEM, T_FC_ITEM_HIDE_ITEM, pHideItem, SendHideBuf);
	pHideItem->ItemFieldIndex	= i_pTenderItemInfo->m_ItemFieldIndex;
	pHideItem->ClientIndex		= pSuccessfullBidder->UserClientIdx;
	this->m_pInfinityMapChannel->SendMessageToAllInChannel(SendHideBuf, MSG_SIZE(MSG_FC_ITEM_HIDE_ITEM));
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2010-04-09 by cmkwon, 인피2차 추가 수정(단계별 보상 추가) - 
/// \author		cmkwon
/// \date		2010-04-12 ~ 2010-04-12
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CInfinityBase::CancelTenderItemInfo(CTenderItemInfo *i_pTenderItemInfo)
{
	if(NULL == m_pInfinityMapChannel)
	{
		return FALSE;
	}

	CMapBlock *pTmpMapBlock	= this->m_pInfinityMapChannel->GetBlock(i_pTenderItemInfo->m_TenderItemPosition.x, i_pTenderItemInfo->m_TenderItemPosition.z);
	if (NULL == pTmpMapBlock)
	{
		return FALSE;
	}
	
	DROPITEM *pDropItem		= pTmpMapBlock->GetDROPITEM_SetProcess((ITEM_GENERAL*)i_pTenderItemInfo->m_ItemFieldIndex, TRUE);
	if(NULL == pDropItem)
	{
		return FALSE;
	}

	pDropItem->bProcessing	= FALSE;
	pDropItem->bTendering	= FALSE;
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2010-04-09 by cmkwon, 인피2차 추가 수정(단계별 보상 추가) - 
/// \author		cmkwon
/// \date		2010-04-12 ~ 2010-04-12
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
Err_t CInfinityBase::PushTenderItem(CTenderItemInfo *i_pTenderItemInfo)
{
	mt_auto_lock mtA(&m_mtTenderItemList);

	if(NULL != this->GetTenderItemInfoNoLock(i_pTenderItemInfo->m_ItemFieldIndex))
	{
		return ERR_DB_INVALID_UNIQUE_NUMBER;
	}

//	g_pFieldGlobal->WriteSystemLogEX(TRUE, "[TEMP] [Infinity][%I64d] CInfinityBase::PushTenderItem# ! Map(%s) ItemNum(%d) ItemFieldIdx(0x%X) Pos(%5.2f, %5.2f) \r\n"
//		, this->m_nInfinityCreateUID, GetMapString(m_pInfinityMapChannel->GetMapChannelIndex(), string()), i_pTenderItemInfo->m_TenderItemNum, i_pTenderItemInfo->m_ItemFieldIndex
//		, i_pTenderItemInfo->m_TenderItemPosition.x, i_pTenderItemInfo->m_TenderItemPosition.z);

	//i_pTenderItemInfo->m_TenderStartTick	= timeGetTime();
	i_pTenderItemInfo->m_TenderStartTime.SetCurrentDateTime();

	m_mtTenderItemList.push_back(*i_pTenderItemInfo);

	return ERR_NO_ERROR;
}



// 2010. 07. 27 by hsLee 인피니티 2차 거점 방어 시네마 연출 스킵 처리.
/**********************************************************************************
**
**	시네마 스킵 관련 변수 초기화.
**
**	Create Info :	2010. 07. 27. by hsLee.
**
***********************************************************************************/
void CInfinityBase :: ResetValue_SkipEndingCinema ( void )
{
	m_sCinemaSkipTime.Reset();

	m_eCinemaSkipState = INFINITY_SKIP_CINEMA_NONE;

	memset ( m_szRqSkipEndingCinema_UserName , 0x00, sizeof( SIZE_MAX_CHARACTER_NAME ) );
}


/**************************************************************************************************************
**
**	시네마 스킵 활성화 & 요청 유저정보 저장.
**
**	Create Info :	2010. 07. 27. by hsLee.
**
**	Update Info :	스킵 처리에 '정상 종료' 처리 추가. 'a_bNormalEnding'	2010. 08. 26. by hsLee.
**
***************************************************************************************************************/
bool CInfinityBase :: SetValue_SkipEndingCinema ( const CHAR *pszRqUserName , const bool a_bNormalEnding )
{
	if ( m_eCinemaSkipState != INFINITY_SKIP_CINEMA_NONE )
		return false;

	m_sCinemaSkipTime.SetCurrentDateTime();

	UpdateTenderItemInfoAll_TenderStartTick ( m_sCinemaSkipTime );

	if ( a_bNormalEnding )
		m_eCinemaSkipState = INFINITY_SKIP_CINEMA_NORMAL;
	else
		m_eCinemaSkipState = INFINITY_SKIP_CINEMA_REQ_SKIP;

	strcpy ( m_szRqSkipEndingCinema_UserName , pszRqUserName );
	
	return true;
}


// 등록되어 있는 입찰 아이템들의 입찰 시작 시간을 일괄 갱신.
void CInfinityBase :: UpdateTenderItemInfoAll_TenderStartTick ( ATUM_DATE_TIME sUpdateTime )
{

	if ( 0 == m_mtTenderItemList.size() || 0 == sUpdateTime.GetTimeInSeconds() ) return;

	mt_auto_lock mta ( &m_mtTenderItemList );

	for (auto item : m_mtTenderItemList)
	{
		if (item.m_IsTimeOver) continue;

		item.m_TenderStartTime = sUpdateTime;
	}

	mta.auto_unlock_cancel();

}
// End 2010. 07. 27 by hsLee 인피니티 2차 거점 방어 시네마 연출 스킵 처리.

// 2010-06-25 by shcho, 인피니티 관련로그 찍기 - 습득 아이템 정보 DB저장
BOOL CInfinityBase :: SendInfinityItemTender( CFieldIOCPSocket* i_pBidderFISoc, DiceCnt_t i_DiceCount, INT i_ItemNum )
{

	INIT_MSG_WITH_BUFFER( MSG_FtoA_INFINITY_TENDER_ITEM, T_FtoA_LOG_INFINITY_USER_GET_TENDERITEM, pTenderItemMsg, SendBuf );
	CIOCPSocket* MFSSock = i_pBidderFISoc->ms_pFieldIOCP->GetMFSSockForArenaServer();

	util::strncpy(pTenderItemMsg->AccountName, i_pBidderFISoc->GetCharacter()->AccountName, SIZE_MAX_ACCOUNT_NAME);
	util::strncpy(pTenderItemMsg->CharacterName, i_pBidderFISoc->GetCharacter()->CharacterName, SIZE_MAX_CHARACTER_NAME);
	pTenderItemMsg->AccountUID		= i_pBidderFISoc->GetCharacter()->AccountUniqueNumber;
	pTenderItemMsg->CharacterUID	= i_pBidderFISoc->GetCharacter()->CharacterUniqueNumber;
	pTenderItemMsg->GDicecount		= i_DiceCount;
	pTenderItemMsg->TenderItemNum	= i_ItemNum;
	pTenderItemMsg->MFSClientIdx	= i_pBidderFISoc->m_AfsNeedMfsInfo.MFSClientIdx;

	MFSSock->SendAddData( SendBuf, MSG_SIZE(MSG_FtoA_INFINITY_TENDER_ITEM) );
	return TRUE;
}
// END 2010-06-25 by shcho, 인피니티 관련로그 찍기 - 습득 아이템 정보 DB저장

// start 2011-05-30 by hskim, 인피니티 3차 - 플레이 시간 재설정 기능 
void CInfinityBase::EnablePenaltyApply()
{
	m_bPenaltyApply = TRUE;
}

void CInfinityBase::DisablePenaltyApply()
{
	m_bPenaltyApply = FALSE;
}

BOOL CInfinityBase::IsPenaltyApply()
{
	return m_bPenaltyApply;
}
// end 2011-05-30 by hskim, 인피니티 3차 - 플레이 시간 재설정 기능 

// 2013-05-16 by jhseol, 인피 재접속 시 파티지원
void CInfinityBase::CheckInfinityPartyState()
{
	UID32_t nPartyMasterUID = 0;
	PartyID_t nPartyID = 0;

	mt_auto_lock mtAuto(&m_mtVectPlayer);
	mtVectCFISoc::iterator itr = m_mtVectPlayer.begin();
	for(; itr != m_mtVectPlayer.end(); itr++)
	{
		CFieldIOCPSocket *pFISoc = *itr;
		if( pFISoc && pFISoc->IsValidCharacter(FALSE) )
		{
			if ( NULL != pFISoc->m_pFieldParty )
			{
				nPartyMasterUID = pFISoc->m_pFieldParty->m_MasterUniqueNumber;
				nPartyID = pFISoc->m_pFieldParty->m_PartyID;
				break;
			}
		}
	}

	if ( 0 == nPartyID )
	{
		AutoPartyCreate();
	}
	else
	{
		for(itr = m_mtVectPlayer.begin(); itr != m_mtVectPlayer.end(); itr++)
		{
			CFieldIOCPSocket *pFISoc = *itr;
			if( NULL != pFISoc )
			{
				if ( TRUE == pFISoc->IsValidCharacter(FALSE) && NULL == pFISoc->m_pFieldParty )
				{
					// 파티가 없는 유저임으로 기존에 있는 파티에 가입을 시킨다.
					INIT_MSG_WITH_BUFFER(MSG_FC_PARTY_REQUEST_INVITE_QUESTION_THE_ARENA, T_FC_PARTY_REQUEST_INVITE_QUESTION_THE_ARENA, pQuestion, pQuestionBuf);
					pQuestion->PartyID = nPartyID;
					pFISoc->SendAddData(pQuestionBuf, MSG_SIZE(MSG_FC_PARTY_REQUEST_INVITE_QUESTION_THE_ARENA));
				}
			}
		}
	}
}
// end 2013-05-16 by jhseol, 인피 재접속 시 파티지원