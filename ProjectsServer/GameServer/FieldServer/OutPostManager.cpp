// OutPostManager.cpp: implementation of the COutPostManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "OutPostManager.h"
#include "OutPost.h"
#include "FieldIOCP.h"
#include "FieldGlobal.h"
#include "AtumLogSender.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

COutPostManager::COutPostManager()
{
}

COutPostManager::~COutPostManager()
{
	mt_auto_lock mta(&m_mtvectOutPostManager);		// 2009-06-24 by cmkwon, Lock 추가함. - 

	for (auto x : m_mtvectOutPostManager) delete x;
	m_mtvectOutPostManager.clear();
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void COutPostManager::InitOutPostManager(CFieldIOCP * i_pFIOCP)
/// \brief		초기화
/// \author		dhjin
/// \date		2007-08-16 ~ 2007-08-16
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void COutPostManager::InitOutPostManager(CFieldIOCP * i_pFIOCP)
{
	m_pFieldIOCP18	= i_pFIOCP;
	m_mtvectOutPostManager.clearLock();		// 2009-06-24 by cmkwon, Lock 추가함. - 
	m_vectOutPostNextWarTimeInfo.clear();
}

COutPost * COutPostManager::GetOutPost(int i_nMapIndex)
{
	mt_auto_lock mta(&m_mtvectOutPostManager);

	mtvectOutPost::iterator	itr = m_mtvectOutPostManager.begin();
	for(; itr != m_mtvectOutPostManager.end(); itr++)
	{
		if(i_nMapIndex == (*itr)->GetOutPostMapIndex()
			|| i_nMapIndex == (*itr)->GetOutPostCityMapIndex())		// 2007-09-28 by cmkwon, 추가함
		{
			return *itr;
		}
	}
	
	return NULL;
}

COutPost * COutPostManager::GetOutPostCityMap(int i_nCityMapIndex)
{
	mt_auto_lock mta(&m_mtvectOutPostManager);

	mtvectOutPost::iterator	itr = m_mtvectOutPostManager.begin();
	for(; itr != m_mtvectOutPostManager.end(); itr++)
	{
		if(i_nCityMapIndex == (*itr)->GetOutPostCityMapIndex())
		{
			return *itr;
		}
	}
	
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void COutPostManager::SetOutPostManagerByDBLoad(vectSOutPostInfo * i_pVectOutPostInfo)	// 전진기지 관련 정보를 DB로 부터 읽어 전진기지 정보 수 만큼 Class OutPost를 생성하여 m_mtvectSOutPost에 추가한다.
/// \brief		전진기지 관련 정보를 DB로 부터 읽어 전진기지 정보 수 만큼 Class OutPost를 생성하여 m_mtvectSOutPost에 추가한다.
/// \author		dhjin
/// \date		2007-08-16 ~ 2007-08-16
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void COutPostManager::SetOutPostManagerByDBLoad(vectSOutPostInfo * i_pVectOutPostInfo)
{
	mt_auto_lock mta(&m_mtvectOutPostManager);

	// 2007-08-16 by dhjin, 전진기지 정보 만큼 COutPost 생성
	vectSOutPostInfo::iterator	itr = i_pVectOutPostInfo->begin();
	for(; itr != i_pVectOutPostInfo->end(); itr++)
	{
		COutPost * tmOutPost = new COutPost(&*itr, m_pFieldIOCP18);
		m_mtvectOutPostManager.push_back(tmOutPost);
	}

}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void COutPostManager::SetOutPostNextWarTimeInfoByDBLoad(vectATUM_DATE_TIME * i_pvectOutPostNextWarTimeInfo)
/// \brief		전진기지 전쟁 시간 관련 정보를 DB에서 읽어온다.
/// \author		dhjin
/// \date		2007-08-21 ~ 2007-08-21
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void COutPostManager::SetOutPostNextWarTimeInfoByDBLoad(vectATUM_DATE_TIME * i_pvectOutPostNextWarTimeInfo)
{
	vectATUM_DATE_TIME::iterator	itr = i_pvectOutPostNextWarTimeInfo->begin();
	for(; itr != i_pvectOutPostNextWarTimeInfo->end(); itr++)
	{
		m_vectOutPostNextWarTimeInfo.push_back(*itr);
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void COutPostManager::OnDoMinutelyWorkOutPostManager(ATUM_DATE_TIME *i_pCurDateTime)
/// \brief		분 마다 전진기지 관련 이벤트 체크
/// \author		dhjin
/// \date		2007-08-16 ~ 2007-08-16
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void COutPostManager::OnDoMinutelyWorkOutPostManager(ATUM_DATE_TIME *i_pCurDateTime)
{
	// 2010-03-02 by cmkwon, ArenaServer는 필요없음 - 
	if(g_pFieldGlobal->IsArenaServer()){				return;}

	///////////////////////////////////////////////////////////////////////////////	
	// 2009-07-08 by cmkwon, 전진 기지전 버그 수정 - 
	vectSOUTPOSTWAR_EV_RESET_SUCCESS	tmVectEvResetSuccessList;
	vectSOUTPOSTWAR_EV_END				tmVectEvEndList;


	mt_auto_lock mta(&m_mtvectOutPostManager);
	
	mtvectOutPost::iterator	itr = m_mtvectOutPostManager.begin();
	for(; itr != m_mtvectOutPostManager.end(); itr++)
	{
		if((*itr)->CheckSetOutPostWarDay(i_pCurDateTime))
		{// 2007-09-03 by dhjin, 전진기지 전쟁 시간 설정을 하는 날자가 지났으면 설정한다.
			continue;
		}
		
		// 2007-08-17 by dhjin, 전진기지 전이 발생할 시간을 체크한다.
		int	CheckRemainWarTimeAtMin = (*itr)->CheckOutPostWarTime(i_pCurDateTime);
		
		if(30 == CheckRemainWarTimeAtMin || 10 == CheckRemainWarTimeAtMin || 5 == CheckRemainWarTimeAtMin)
		{// 2007-08-17 by dhjin, 전쟁시작 30분 전, 10분 전, 5분전 남은 시간을 클라이언트에 전송한다.
			this->SendOutPostWarRemainTime(CheckRemainWarTimeAtMin, (*itr)->GetOutPostInfluence(), (*itr)->GetOutPostMapIndex());
			continue;
		}

		if(0 >= CheckRemainWarTimeAtMin && OUTPOST_NORMAL == (*itr)->GetOutPostState())
		{// 2007-08-17 by dhjin, 전쟁시작~!!
			this->OutPostWarStart(*itr);
		}
		else if(0 > CheckRemainWarTimeAtMin && IS_OUTPOST( (*itr)->GetOutPostState()) )
		{// 2007-08-20 by dhjin, 전진기지 전 중이라면 체크 해야될 사항
			
			// 2007-08-20 by dhjin, 전진기지 시작 하고 난 다음 지난 시간을 구하기 위해 음수를 양수로 변환한다.
			CheckRemainWarTimeAtMin = abs(CheckRemainWarTimeAtMin);
			
			if(OUTPOST_WARTIME <= CheckRemainWarTimeAtMin
				|| (g_pFieldGlobal->IsTestServer() && OUTPOST_WARTIME_FOR_TESTSERVER <= CheckRemainWarTimeAtMin))		// 2008-10-29 by cmkwon, 테섭 시스템 수정(전진기지전 1시간(60분)으로 설정) - 
			{// 2007-08-20 by dhjin, 전진기지 전 시간이 OUTPOST_WARTIME 지났다면 전진기지 전쟁을 끝낸다.
				// 2009-07-08 by cmkwon, 전진 기지전 버그 수정 - 인자추가(, vectSOUTPOSTWAR_EV_END *io_pEvEndList)
				this->OutPostWarEnd(*itr, &tmVectEvEndList);
			}

			// 2007-08-20 by dhjin, 전진기지 각인 시간 체크
			int CheckPassageResetTime = (*itr)->CheckOutPostResetTime(i_pCurDateTime);
			
			if(0 == CheckPassageResetTime && IS_OUTPOST_RESETING((*itr)->GetOutPostState()) )
			{// 2007-08-20 by dhjin, 각인 성공~!!

				// 2009-07-08 by cmkwon, 전진 기지전 버그 수정 - 인자추가(, vectSOUTPOSTWAR_EV_RESET_SUCCESS *io_pVectEvResetSuccessList)
				this->OutPostResetSuccess(*itr, &tmVectEvResetSuccessList);
			}

		}

	}
	mta.auto_unlock_cancel();	// 2009-07-08 by cmkwon, 전진 기지전 버그 수정 - deadlock 을 피하기 위해 여기에서 락해제

	///////////////////////////////////////////////////////////////////////////////
	// 2009-07-08 by cmkwon, 전진 기지전 버그 수정 - 
	vectSOUTPOSTWAR_EV_RESET_SUCCESS::iterator evResetItr(tmVectEvResetSuccessList.begin());
	for(; evResetItr != tmVectEvResetSuccessList.end(); evResetItr++)
	{
		SOUTPOSTWAR_EV_RESET_SUCCESS *pReset = &*evResetItr;
		if(NULL == pReset->pFMChanOfOutPostWarMap)
		{
			continue;
		}

		// 2009-07-08 by cmkwon, 전진 기지전 버그 수정 - 
		g_pFieldGlobal->WriteSystemLogEX(TRUE, "  [Notify] OutPostWarNoti COutPostManager::OnDoMinutelyWorkOutPostManager# EvResetSuccess OutpostWarMapIndex(%d) \r\n"
			, pReset->pFMChanOfOutPostWarMap->GetMapChannelIndex().MapIndex);

		pReset->pFMChanOfOutPostWarMap->WarpToCityMapFieldMapChannelByInfluence(pReset->bySuccessInfluenceTy);
	}
	vectSOUTPOSTWAR_EV_END::iterator evEndItr(tmVectEvEndList.begin());
	for(; evEndItr != tmVectEvEndList.end(); evEndItr++)
	{
		SOUTPOSTWAR_EV_END *pEnd = &*evEndItr;
		if(NULL == pEnd->pFMChanOfOutPostWarMap)
		{
			continue;
		}

		// 2009-07-08 by cmkwon, 전진 기지전 버그 수정 - 
		g_pFieldGlobal->WriteSystemLogEX(TRUE, "  [Notify] OutPostWarNoti COutPostManager::OnDoMinutelyWorkOutPostManager# EvEnd OutpostWarMapIndex(%d) WinInfl(%d) LossInfl(%d) WinGuildUID(%ld) CityMapIndexOfOutpost(%d) \r\n"
			, pEnd->pFMChanOfOutPostWarMap->GetMapChannelIndex().MapIndex, pEnd->byWinInfluenceTy, pEnd->byLossInfluenceTy, pEnd->GuildUIDPossessOutPost, pEnd->nCityMapIndexForWarp);

		// 2009-07-08 by cmkwon, 1. WarPoint, 아이템 보상 처리
		if(COMPARE_INFLUENCE(pEnd->byWinInfluenceTy, INFLUENCE_TYPE_VCN|INFLUENCE_TYPE_ANI))
		{			
			pEnd->pFMChanOfOutPostWarMap->AddWarPointInMap(OUTPOST_PAY_WARPOINT, FALSE, pEnd->byWinInfluenceTy, WPUT_OUTPOSTWAR);				// 2009-01-22 by cmkwon, 전쟁 관련 게임 로그 수정 - CFieldMapChannel::AddWarPointInMap()에 인자(, BYTE i_byWPUpdateTy=WPUT_GENERAL) 추가, // 2008-05-20 by dhjin, 승리 세력 관련 버그 수정. pQParam 값 사용하는 부분 수정
			pEnd->pFMChanOfOutPostWarMap->AddWarPointInMap(OUTPOST_PAY_WARPOINT_BY_LOSE, FALSE, pEnd->byLossInfluenceTy, WPUT_OUTPOSTWAR);	// 2009-01-22 by cmkwon, 전쟁 관련 게임 로그 수정 - CFieldMapChannel::AddWarPointInMap()에 인자(, BYTE i_byWPUpdateTy=WPUT_GENERAL) 추가, // 2008-01-07 by dhjin, 세력 보상 수정 - 
			pEnd->pFMChanOfOutPostWarMap->InsertItemInMap(OUTPOST_PAY_ITEM, OUTPOST_PAY_ITEM_COUNT, pEnd->byWinInfluenceTy);	// 2008-05-20 by dhjin, 승리 세력 관련 버그 수정. pQParam 값 사용하는 부분 수정 // 2007-11-06 by dhjin, 아이템 지급 추가 

			CFieldMapChannel		*pBackFMChann = m_pFieldIOCP18->GetFieldMapChannelByMapChannelIndex(MAP_CHANNEL_INDEX(pEnd->pFMChanOfOutPostWarMap->m_pFieldMapProject->m_nBeforeMapIndex, 0));
			if(pBackFMChann)
			{
				pBackFMChann->AddWarPointInMap(OUTPOST_PAY_WARPOINT, FALSE, pEnd->byWinInfluenceTy, WPUT_OUTPOSTWAR);				// 2009-01-22 by cmkwon, 전쟁 관련 게임 로그 수정 - CFieldMapChannel::AddWarPointInMap()에 인자(, BYTE i_byWPUpdateTy=WPUT_GENERAL) 추가// 2008-05-20 by dhjin, 승리 세력 관련 버그 수정. pQParam 값 사용하는 부분 수정
				pBackFMChann->AddWarPointInMap(OUTPOST_PAY_WARPOINT_BY_LOSE, FALSE, pEnd->byLossInfluenceTy, WPUT_OUTPOSTWAR);	// 2009-01-22 by cmkwon, 전쟁 관련 게임 로그 수정 - CFieldMapChannel::AddWarPointInMap()에 인자(, BYTE i_byWPUpdateTy=WPUT_GENERAL) 추가	// 2008-01-07 by dhjin, 세력 보상 수정 - 
				pBackFMChann->InsertItemInMap(OUTPOST_PAY_ITEM, OUTPOST_PAY_ITEM_COUNT, pEnd->byWinInfluenceTy);	// 2008-05-20 by dhjin, 승리 세력 관련 버그 수정. pQParam 값 사용하는 부분 수정	// 2007-11-06 by dhjin, 아이템 지급 추가 
			}

			// 2008-06-20 by cmkwon, EP3 백맵 시스템 수정(맵당 백맵을 2개 설정 할 수 있다) - 전진기지전 종료시 백맵2 에도 보상
			pBackFMChann = m_pFieldIOCP18->GetFieldMapChannelByMapChannelIndex(MAP_CHANNEL_INDEX(pEnd->pFMChanOfOutPostWarMap->m_pFieldMapProject->m_nBeforeMapIndex2, 0));
			if(pBackFMChann)
			{
				pBackFMChann->AddWarPointInMap(OUTPOST_PAY_WARPOINT, FALSE, pEnd->byWinInfluenceTy, WPUT_OUTPOSTWAR);				// 2009-01-22 by cmkwon, 전쟁 관련 게임 로그 수정 - CFieldMapChannel::AddWarPointInMap()에 인자(, BYTE i_byWPUpdateTy=WPUT_GENERAL) 추가// 2008-05-20 by dhjin, 승리 세력 관련 버그 수정. pQParam 값 사용하는 부분 수정
				pBackFMChann->AddWarPointInMap(OUTPOST_PAY_WARPOINT_BY_LOSE, FALSE, pEnd->byLossInfluenceTy, WPUT_OUTPOSTWAR);	// 2009-01-22 by cmkwon, 전쟁 관련 게임 로그 수정 - CFieldMapChannel::AddWarPointInMap()에 인자(, BYTE i_byWPUpdateTy=WPUT_GENERAL) 추가		// 2008-01-07 by dhjin, 세력 보상 수정 - 
				pBackFMChann->InsertItemInMap(OUTPOST_PAY_ITEM, OUTPOST_PAY_ITEM_COUNT, pEnd->byWinInfluenceTy);	// 2008-05-20 by dhjin, 승리 세력 관련 버그 수정. pQParam 값 사용하는 부분 수정	// 2007-11-06 by dhjin, 아이템 지급 추가 
			}
		}

		// 2009-07-08 by cmkwon, 2. 개인 보상 처리
		m_pFieldIOCP18->PayCOutPostWarContributionCalculation(pEnd->pFMChanOfOutPostWarMap->GetMapChannelIndex().MapIndex, pEnd->byWinInfluenceTy);
		m_pFieldIOCP18->DeleteCOutPostWarContributionCalculation(pEnd->pFMChanOfOutPostWarMap->GetMapChannelIndex().MapIndex);	

		// 2009-07-08 by cmkwon, 3. 강제 워프 처리
		pEnd->pFMChanOfOutPostWarMap->WarpToCityMapFieldMapChannelByGuildUID(pEnd->GuildUIDPossessOutPost);
		if(0 < pEnd->nCityMapIndexForWarp)
		{
			pEnd->pFMChanOfOutPostWarMap->WarpToOutPostCityMapFieldMapChannelByGuildUID(pEnd->GuildUIDPossessOutPost, pEnd->nCityMapIndexForWarp);
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void COutPostManager::SendOutPostWarRemainTime(int i_nRemainTime, BYTE i_byInfluence, int i_nMapIndex)
/// \brief		전진기기 전투 시작 남은 시간을 클라이언트에게 전송한다.
/// \author		dhjin
/// \date		2007-08-17 ~ 2007-08-17
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void COutPostManager::SendOutPostWarRemainTime(int i_nRemainTime, BYTE i_byInfluence, int i_nMapIndex)
{
	INIT_MSG_WITH_BUFFER(MSG_FC_OUTPOST_WAR_BEFORE, T_FC_OUTPOST_WAR_BEFORE, SendMsg, SendBuf);
	SendMsg->Influence		= i_byInfluence;
	SendMsg->MapIndex		= i_nMapIndex;
	SendMsg->RemainTime		= i_nRemainTime;		
	m_pFieldIOCP18->SendMessageToAllClients(SendBuf,MSG_SIZE(MSG_FC_OUTPOST_WAR_BEFORE),INFLUENCE_TYPE_VCN|INFLUENCE_TYPE_ANI);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void COutPostManager::OutPostWarStart(COutPost * i_pOutPost)
/// \brief		전진기지 전 시작
/// \author		dhjin
/// \date		2007-08-17 ~ 2007-08-17
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void COutPostManager::OutPostWarStart(COutPost * i_pOutPost)
{
	//////////////////////////////////////////////////////////////////////////
	//  1. 전진기지 상태값 변경	OUTPOST_WARSTART
	//	2. 전진기지 몬스터 소환
	//  3. 유저에게 정보 전송
	//  4. 전진기지 상태값 변경 OUTPOST_WARING
	
	// 2007-08-17 by dhjin, 전진기지 상태값 변경
	i_pOutPost->SetOutPostState(OUTPOST_WARSTART);

	// 2007-08-17 by dhjin, 몬스터 소환
	CFieldMapChannel *pFMChann = m_pFieldIOCP18->GetFieldMapChannelByMapChannelIndex(MAP_CHANNEL_INDEX(i_pOutPost->GetOutPostMapIndex(), 0));
	if(NULL == pFMChann)
	{
		// 2009-07-08 by cmkwon, 전진 기지전 버그 수정 - 
		g_pFieldGlobal->WriteSystemLogEX(TRUE, "  [Notify] OutPostWarNoti COutPostManager::OutPostWarStart# OutpostWarMapIndex(%d) pFMChann(0x%X) \r\n", i_pOutPost->GetOutPostMapIndex(), pFMChann);
		return;
	}

	// 2008-03-26 by dhjin, 소유 세력은 있고 여단이 없을 경우 처리
	if(FALSE != i_pOutPost->GetOutPostInfluence()
		&& FALSE == i_pOutPost->GetOutPostGuildUID())
	{
		i_pOutPost->SetOutPostPossessInfluence(INFLUENCE_TYPE_UNKNOWN);
	}

	INIT_MSG_WITH_BUFFER(MSG_FN_MONSTER_OUTPOST_INIT, T_FN_MONSTER_OUTPOST_INIT, pSummon, SendBuf);
	pSummon->mapChann	= pFMChann->GetMapChannelIndex();
	this->SetInitOutPostMonsterSummonByBell(i_pOutPost->GetOutPostInfluence(), &(pSummon->bell1), &(pSummon->bell2), &(pSummon->bell3) );
	pFMChann->Send2NPCServerW(SendBuf, MSG_SIZE(MSG_FN_MONSTER_OUTPOST_INIT));
	
	// 2007-10-02 by dhjin, 테스트 용
	g_pFieldGlobal->WriteSystemLogEX(TRUE, "  [Notify] OutPostWarNoti OutPostWarStart  OUTPOST_INIT bell1 : [%6d], bell2 : [%6d], bell3 : [%6d], NextWarTimeDB : [%s]\r\n"
		, pSummon->bell1, pSummon->bell2, pSummon->bell3, i_pOutPost->GetOutPostNextWarTime()->GetDateTimeString().GetBuffer());	// 2008-06-27 by dhjin, 로그에 시작시간이 언제 였는지 추가

	// 2007-08-17 by dhjin, 클라이언트에 정보 전송
	INIT_MSG_WITH_BUFFER(MSG_FC_OUTPOST_WAR_START, T_FC_OUTPOST_WAR_START, SendMsg, SendBuf2);
	SendMsg->OutPostState	= i_pOutPost->GetOutPostState();
	SendMsg->Influence		= i_pOutPost->GetOutPostInfluence();
	SendMsg->MapIndex		= i_pOutPost->GetOutPostMapIndex();
	SendMsg->WarStartTime	= *(i_pOutPost->GetOutPostNextWarTime());
	SendMsg->WarEndTime		= SendMsg->WarStartTime;
	SendMsg->WarEndTime.AddDateTime(0,0,0,0,OUTPOST_WARTIME,0);
	if(g_pFieldGlobal->IsTestServer())
	{// 2008-10-29 by cmkwon, 테섭 시스템 수정(전진기지전 1시간(60분)으로 설정) - 
		SendMsg->WarEndTime		= SendMsg->WarStartTime;	
		SendMsg->WarEndTime.AddDateTime(0,0,0,0,OUTPOST_WARTIME_FOR_TESTSERVER,0);
	}
	m_pFieldIOCP18->SendMessageToAllClients(SendBuf2,MSG_SIZE(MSG_FC_OUTPOST_WAR_START),INFLUENCE_TYPE_VCN|INFLUENCE_TYPE_ANI);

	//////////////////////////////////////////////////////////////////////////
	// 2008-12-23 by dhjin, 전쟁 보상 추가안
	m_pFieldIOCP18->InsertCOutPostWarContributionCalculation(i_pOutPost->GetOutPostMapIndex());
	m_pFieldIOCP18->WarStartOtherInflStayTimeForAllUser();

	// 2007-08-17 by dhjin, 전진기지 상태값 변경
	i_pOutPost->SetOutPostState(OUTPOST_WARING);

	// 2007-10-02 by dhjin, 로그 남기기
	// 2007-10-16 by cmkwon, 로그 추가 - SendLogMessageOutPostStart() 함수 인자 수정
	CAtumLogSender::SendLogMessageOutPostStart(i_pOutPost->GetOutPostMapIndex(), i_pOutPost->GetOutPostInfluence(), i_pOutPost->GetOutPostGuildUID());
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void COutPostManager::OnOutPostProtectorDestroy(int i_nMapIndex)
/// \brief		전진기지 보호막 파괴시 처리 
/// \author		dhjin
/// \date		2007-08-24 ~ 2007-08-24
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void COutPostManager::OnOutPostProtectorDestroy(int i_nMapIndex)
{
	//////////////////////////////////////////////////////////////////////////
	// 1. 데미지를 가장 많이 준 여단 선택
	// 2. 선택된 여단 각인 가능하게 설정
	// 3. 보호막 파괴 정보 클라이언트에게 전송
	// 4. 전진기지 각인 생성
	// 5. 전진기지 상태값 변경 OUTPOST_RESETSTART
	
	mt_auto_lock mta(&m_mtvectOutPostManager);

	COutPost * tmOutPost = this->GetOutPost(i_nMapIndex);
	if(NULL == tmOutPost)
	{
		return;
	}

	CFieldMapChannel *pFMChann = m_pFieldIOCP18->GetFieldMapChannelByMapChannelIndex(MAP_CHANNEL_INDEX(tmOutPost->GetOutPostMapIndex(), 0));
	if(NULL == pFMChann)
	{
		// 2009-07-08 by cmkwon, 전진 기지전 버그 수정 - 
		g_pFieldGlobal->WriteSystemLogEX(TRUE, "  [Notify] OutPostWarNoti COutPostManager::OnOutPostProtectorDestroy# OutpostWarMapIndex(%d) pFMChann(0x%X) \r\n", tmOutPost->GetOutPostMapIndex(), pFMChann);
		return;
	}

	mtvectSOutPostProtectorDamage tmvectDamageList;
	
	INIT_MSG_WITH_BUFFER(MSG_FC_OUTPOST_PROTECTOR_DESTROY, T_FC_OUTPOST_PROTECTOR_DESTROY, SendMsg, SendBuf);
	// 2007-08-24 by dhjin, 데미지를 가장 많이 준 여단 선택, 선택된 여단 각인 가능하게 설정, 보호막 파괴 정보 클라이언트에게 전송
	SendMsg->MapIndex	= pFMChann->GetMapChannelIndex().MapIndex;
	SendMsg->GuildUID	= tmOutPost->GetTopGuildOfProtectorDamage(&tmvectDamageList);		// 2007-10-16 by cmkwon, 로그 추가 - 인자추가(mtvectSOutPostProtectorDamage *o_pvectDamageList)

	// 2007-10-16 by cmkwon, 로그 추가 - 
	CAtumLogSender::SendLogMessageOutPostProtectorDestroy(tmOutPost->GetOutPostMapIndex(), tmOutPost->GetOutPostInfluence(), tmOutPost->GetOutPostGuildUID(), tmOutPost->GetOutPostResetPossibleInfluence(), SendMsg->GuildUID, &tmvectDamageList);

	if(0 == SendMsg->GuildUID)
	{// 2007-10-05 by dhjin, 만약 중앙처리장치 리셋 가능한 여단이 없다면 프로텍터를 다시 소환한다.
		// 2007-10-01 by dhjin, 전진기지 세력 소유
		tmOutPost->SetOutPostPossessInfluenceByReset();

		// 2007-10-01 by dhjin, 세력에 따라 마을로 워프 처리
		pFMChann->WarpToCityMapFieldMapChannelByInfluence(tmOutPost->GetOutPostInfluence());

		// 2007-08-24 by dhjin, 전진기지 몬스터 다시 소환
		INIT_MSG_WITH_BUFFER(MSG_FN_MONSTER_OUTPOST_INIT, T_FN_MONSTER_OUTPOST_INIT, pSummon, SendBuf2);
		pSummon->mapChann	= pFMChann->GetMapChannelIndex();
		this->SetInitOutPostMonsterSummonByBell(tmOutPost->GetOutPostInfluence(), &(pSummon->bell1), &(pSummon->bell2), &(pSummon->bell3) );
		pFMChann->Send2NPCServerW(SendBuf2, MSG_SIZE(MSG_FN_MONSTER_OUTPOST_INIT));
		
		// 2007-10-02 by dhjin, 테스트 용
		g_pFieldGlobal->WriteSystemLogEX(TRUE, "  [Notify] OutPostWarNoti OnOutPostProtectorDestroy  OUTPOST_INIT bell1 : [%6d], bell2 : [%6d], bell3 : [%6d]\r\n"
			, pSummon->bell1, pSummon->bell2, pSummon->bell3);
		
		m_pFieldIOCP18->SendMessageToAllClients(SendBuf,MSG_SIZE(MSG_FC_OUTPOST_PROTECTOR_DESTROY),INFLUENCE_TYPE_VCN|INFLUENCE_TYPE_ANI);
		return;
	}
	SendMsg->Influence	= tmOutPost->GetOutPostResetPossibleInfluence();
	util::strncpy(SendMsg->GuildName, tmOutPost->GetOutPostResetPossibleGuildName(), SIZE_MAX_GUILD_NAME);
	m_pFieldIOCP18->SendMessageToAllClients(SendBuf,MSG_SIZE(MSG_FC_OUTPOST_PROTECTOR_DESTROY),INFLUENCE_TYPE_VCN|INFLUENCE_TYPE_ANI);

	tmOutPost->InitProtectorDamage();

	// 2007-08-24 by dhjin, 전진기지 각인 생성
	INIT_MSG_WITH_BUFFER(MSG_FN_MONSTER_OUTPOST_RESET_SUMMON, T_FN_MONSTER_OUTPOST_RESET_SUMMON, pSummon, SendBuf2);
	pSummon->mapChann	= pFMChann->GetMapChannelIndex();
	if(INFLUENCE_TYPE_VCN == tmOutPost->GetOutPostResetPossibleInfluence())
	{
		pSummon->bell1		= BELL_OUTPOST_RESET_VCN;
	}
	else
	{
		pSummon->bell1		= BELL_OUTPOST_RESET_ANI;
	}
	pFMChann->Send2NPCServerW(SendBuf2, MSG_SIZE(MSG_FN_MONSTER_OUTPOST_RESET_SUMMON));
	
	// 2007-08-24 by dhjin, 전진기지 상태값 변경 OUTPOST_RESETSTART
	tmOutPost->SetOutPostState(OUTPOST_RESETSTART);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void COutPostManager::OnOutPostResetDestroy(int i_nMapIndex)
/// \brief		전진기지 중앙처리 파괴시 처리 
/// \author		dhjin
/// \date		2007-08-24 ~ 2007-08-24
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void COutPostManager::OnOutPostResetDestroy(int i_nMapIndex)
{
	//////////////////////////////////////////////////////////////////////////
	// 1. 중앙처리 파괴 정보 전송
	// 2. 전진기지 상태값 변경 OUTPOST_RESETEND
	// 3. 전진기지 NPC소유 처리
	// 4. 전진기지 맵 유저 모두 마을로 워프 처리
	// 5. 전진기지 몬스터 다시 소환
	// 6. 전진기지 상태값을 변경한다. OUTPOST_WARING

	mt_auto_lock mta(&m_mtvectOutPostManager);

	COutPost * tmOutPost = this->GetOutPost(i_nMapIndex);
	if(NULL == tmOutPost)
	{
		return;
	}

	CFieldMapChannel *pFMChann = m_pFieldIOCP18->GetFieldMapChannelByMapChannelIndex(MAP_CHANNEL_INDEX(tmOutPost->GetOutPostMapIndex(), 0));
	if(NULL == pFMChann)
	{
		// 2009-07-08 by cmkwon, 전진 기지전 버그 수정 - 
		g_pFieldGlobal->WriteSystemLogEX(TRUE, "  [Notify] OutPostWarNoti COutPostManager::OnOutPostResetDestroy# OutpostWarMapIndex(%d) pFMChann(0x%X) \r\n", tmOutPost->GetOutPostMapIndex(), pFMChann);
		return;
	}		
	
	INIT_MSG_WITH_BUFFER(MSG_FC_OUTPOST_RESET_DESTROY, T_FC_OUTPOST_RESET_DESTROY, SendMsg, SendBuf);
	// 2007-08-24 by dhjin, 중앙처리 파괴 정보 전송
	SendMsg->MapIndex	= i_nMapIndex;
	m_pFieldIOCP18->SendMessageToAllClients(SendBuf,MSG_SIZE(MSG_FC_OUTPOST_RESET_DESTROY),INFLUENCE_TYPE_VCN|INFLUENCE_TYPE_ANI);

	// 2007-08-24 by dhjin, 전진기지 상태값 변경 OUTPOST_RESETEND
	tmOutPost->SetOutPostState(OUTPOST_RESETEND);

	//////////////////////////////////////////////////////////////////////////
	// 2007-10-01 by dhjin, 전에 소유하던 세력이 NPC이면 NPC 소유이다.
	if(FALSE == tmOutPost->GetOutPostInfluence())
	{
		// 2007-08-24 by dhjin, 전진기지 NPC소유 처리
		tmOutPost->SetOutPostPossessNPCByReset();

		// 2007-08-24 by dhjin, 맵 유저 모두 마을로 워프 처리
		pFMChann->WarpToCityMapFieldMapChannelByGuildUID(tmOutPost->GetOutPostGuildUID());
	}
	else
	{// 2007-10-01 by dhjin, VCN, ANI 세력이 소유 했을 경우 그대로 둔다.
		// 2007-10-01 by dhjin, 전진기지 세력 소유
		tmOutPost->SetOutPostPossessInfluenceByReset();

		// 2007-10-01 by dhjin, 세력에 따라 마을로 워프 처리
		pFMChann->WarpToCityMapFieldMapChannelByInfluence(tmOutPost->GetOutPostInfluence());

		m_pFieldIOCP18->m_InflWarManager.CalcConsecutiveVictoriesPoint(tmOutPost->GetOutPostInfluence());		// 2013-05-09 by hskim, 세력 포인트 개선	
	}

	// 2007-08-24 by dhjin, 전진기지 몬스터 다시 소환
	INIT_MSG_WITH_BUFFER(MSG_FN_MONSTER_OUTPOST_INIT, T_FN_MONSTER_OUTPOST_INIT, pSummon, SendBuf2);
	pSummon->mapChann	= pFMChann->GetMapChannelIndex();
	this->SetInitOutPostMonsterSummonByBell(tmOutPost->GetOutPostInfluence(), &(pSummon->bell1), &(pSummon->bell2), &(pSummon->bell3) );
	pFMChann->Send2NPCServerW(SendBuf2, MSG_SIZE(MSG_FN_MONSTER_OUTPOST_INIT));

	// 2007-10-02 by dhjin, 테스트 용
	g_pFieldGlobal->WriteSystemLogEX(TRUE, "  [Notify] OutPostWarNoti OnOutPostResetDestroy  OUTPOST_INIT bell1 : [%6d], bell2 : [%6d], bell3 : [%6d]\r\n"
		, pSummon->bell1, pSummon->bell2, pSummon->bell3);

	// 2007-08-24 by dhjin, 전진기지 상태값 변경 OUTPOST_WARING
	tmOutPost->SetOutPostState(OUTPOST_WARING);

	// 2007-10-16 by cmkwon, 로그 추가 - 
	CAtumLogSender::SendLogMessageOutPostResetDestroy(tmOutPost->GetOutPostMapIndex(), tmOutPost->GetOutPostInfluence(), tmOutPost->GetOutPostGuildUID(), tmOutPost->GetOutPostResetPossibleInfluence(), tmOutPost->GetOutPostResetPossibleGuildUID());
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL COutPostManager::CheckValidGuildUID(int i_nMapIndex, UID32_t i_nGuildUID)
/// \brief		유효한 여단인지 체크
/// \author		dhjin
/// \date		2007-08-22 ~ 2007-08-22
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL COutPostManager::CheckValidGuildUID(int i_nMapIndex, UID32_t i_nGuildUID)
{
	// 2007-09-21 by cmkwon, 여단 UID가 유효한지 미리 체크
	if(FALSE == IS_VALID_UNIQUE_NUMBER(i_nGuildUID))
	{
		return FALSE;
	}

	mt_auto_lock mta(&m_mtvectOutPostManager);

	// 2007-08-20 by dhjin, 유효한 길드인지 체크
	mtvectOutPost::iterator	itr = m_mtvectOutPostManager.begin();
	for(; itr != m_mtvectOutPostManager.end(); itr++)
	{
		if(i_nMapIndex == (*itr)->GetOutPostMapIndex()
			&& i_nGuildUID == (*itr)->GetOutPostGuildUID())
		{
			return TRUE;
		}
	}	

	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			INT	COutPostManager::GetOutPostCityMapIndexByGuildUID(UID32_t i_nGuildUID)
/// \brief		유효한 여단이면 전진기지 도시 맵 번호를 리턴한다.
/// \author		dhjin
/// \date		2007-09-05 ~ 2007-09-05
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
INT	COutPostManager::GetOutPostCityMapIndexByGuildUID(UID32_t i_nGuildUID)
{
	// 2007-09-21 by cmkwon, 체크 추가
	if(FALSE == IS_VALID_UNIQUE_NUMBER(i_nGuildUID))
	{
		return 0;
	}

	mt_auto_lock mta(&m_mtvectOutPostManager);

	mtvectOutPost::iterator	itr = m_mtvectOutPostManager.begin();
	for(; itr != m_mtvectOutPostManager.end(); itr++)
	{
		if(i_nGuildUID == (*itr)->GetOutPostGuildUID())
		{
			return (*itr)->GetOutPostCityMapIndex();
		}
	}	

	return NULL;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			INT	COutPostManager::GetOutPostMapIndexByGuildUID(UID32_t i_nGuildUID)
/// \brief		유효한 여단이면 전진기지 맵 번호를 리턴한다.
/// \author		dhjin
/// \date		2007-09-15 ~ 2007-09-15
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
INT	COutPostManager::GetOutPostMapIndexByGuildUID(UID32_t i_nGuildUID)
{
	mt_auto_lock mta(&m_mtvectOutPostManager);

	mtvectOutPost::iterator	itr = m_mtvectOutPostManager.begin();
	for(; itr != m_mtvectOutPostManager.end(); itr++)
	{
		if(i_nGuildUID == (*itr)->GetOutPostGuildUID())
		{
			return (*itr)->GetOutPostMapIndex();
		}
	}	

	return NULL;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL COutPostManager::CheckResetValidGuildUID(int i_nMapIndex, UID32_t i_nGuildUID)
/// \brief		각인이 유효한 유저인지 체크
/// \author		dhjin
/// \date		2007-08-20 ~ 2007-08-20
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL COutPostManager::CheckResetValidGuildUID(int i_nMapIndex, UID32_t i_nGuildUID)
{
	mt_auto_lock mta(&m_mtvectOutPostManager);

	// 2007-08-20 by dhjin, 전진기지 리셋 가능한 길드인지 체크
	mtvectOutPost::iterator	itr = m_mtvectOutPostManager.begin();
	for(; itr != m_mtvectOutPostManager.end(); itr++)
	{
		if(i_nMapIndex == (*itr)->GetOutPostMapIndex()
			&& i_nGuildUID == (*itr)->GetOutPostResetPossibleGuildUID())
		{
			return TRUE;
		}
	}	

	return FALSE;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			BYTE COutPostManager::GetOutPostInfluenceByMapIndex(int i_nMapIdx)
/// \brief		// 2007-09-28 by cmkwon, 전진기지맵 워프 체크 수정 - 해당 전진기지의 소유 세력을 리턴한다.
/// \author		cmkwon
/// \date		2007-09-28 ~ 2007-09-28
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BYTE COutPostManager::GetOutPostInfluenceByMapIndex(int i_nMapIdx)
{
	mt_auto_lock mta(&m_mtvectOutPostManager);

	mtvectOutPost::iterator	itr = m_mtvectOutPostManager.begin();
	for(; itr != m_mtvectOutPostManager.end(); itr++)
	{
		COutPost *pOutPost = *itr;
		if(i_nMapIdx == pOutPost->GetOutPostMapIndex()
			|| i_nMapIdx == pOutPost->GetOutPostCityMapIndex())
		{// 2007-09-28 by cmkwon, 전진기지맵과 전진기지도시맵을 두다 체크한다
			return pOutPost->GetOutPostInfluence();
		}
	}

	return INFLUENCE_TYPE_UNKNOWN;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL COutPostManager::SendOutPostResetStartByGuildCommander(int i_nMapIndex, UID32_t i_guildUID, UID32_t i_charUID)
/// \brief		// 2007-10-16 by cmkwon, 로그 추가 - 인자추가(, UID32_t i_guildUID, UID32_t i_charUID), 여단장이 각인을 시작 함
/// \author		dhjin
/// \date		2007-08-20 ~ 2007-08-20
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL COutPostManager::SendOutPostResetStartByGuildCommander(int i_nMapIndex, UID32_t i_guildUID, UID32_t i_charUID)
{
	mt_auto_lock mta(&m_mtvectOutPostManager);

	ATUM_DATE_TIME ResetStartTime;

	COutPost * tmOutPost = this->GetOutPost(i_nMapIndex);
	if(NULL == tmOutPost)
	{
		return FALSE;
	}

	if(IS_OUTPOST_RESETING(tmOutPost->GetOutPostState()) )
	{// 2007-09-11 by dhjin, 이미 각인 중이면 리턴한다.
		return FALSE;
	}

	tmOutPost->SetOutPostState(OUTPOST_RESETING);
	tmOutPost->SetOutPostResetStartTime( &(ResetStartTime.GetCurrentDateTime()) );

	// 2007-10-16 by cmkwon, 로그 추가 -
	CAtumLogSender::SendLogMessageOutPostResetStart(tmOutPost->GetOutPostMapIndex(), tmOutPost->GetOutPostInfluence(), tmOutPost->GetOutPostGuildUID(), tmOutPost->GetOutPostResetPossibleInfluence(), tmOutPost->GetOutPostResetPossibleGuildUID(), i_guildUID, i_charUID);

	// 2007-08-20 by dhjin, 클라이언트에 정보 전송
	INIT_MSG_WITH_BUFFER(MSG_FC_OUTPOST_RESET_START_OK, T_FC_OUTPOST_RESET_START_OK, SendMsg, SendBuf);
	SendMsg->Influence		= tmOutPost->GetOutPostResetPossibleInfluence();
	SendMsg->MapIndex		= tmOutPost->GetOutPostMapIndex();
	SendMsg->GuildUID		= tmOutPost->GetOutPostResetPossibleGuildUID();
	util::strncpy(SendMsg->GuildName, tmOutPost->GetOutPostResetPossibleGuildName(), SIZE_MAX_GUILD_NAME);
	SendMsg->ResetStartTime = *(tmOutPost->GetOutPostResetStartTime());
	SendMsg->ResetEndTime = *(tmOutPost->GetOutPostResetEndTime());
	m_pFieldIOCP18->SendMessageToAllClients(SendBuf,MSG_SIZE(MSG_FC_OUTPOST_RESET_START_OK),INFLUENCE_TYPE_VCN|INFLUENCE_TYPE_ANI);

	return	TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void COutPostManager::OutPostResetSuccess(COutPost * i_pOutPost)
/// \brief		전진기지 각인 성공
///				// 2009-07-08 by cmkwon, 전진 기지전 버그 수정 - 인자추가(, vectSOUTPOSTWAR_EV_RESET_SUCCESS *io_pVectEvResetSuccessList)
/// \author		dhjin
/// \date		2007-08-20 ~ 2007-08-20
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void COutPostManager::OutPostResetSuccess(COutPost * i_pOutPost, vectSOUTPOSTWAR_EV_RESET_SUCCESS *io_pVectEvResetSuccessList)
{
	//////////////////////////////////////////////////////////////////////////
	// 1. 전진기지 상태값을 변경한다. OUTPOST_RESETEND
	// 2. 전진기지 소유를 각인 성공한 여단, 세력으로 설정한다.
	// 3. 전진기지 각인 종료를 클라이언트에게 전송
	// 4. 전진기지 소유 세력이 아닌 세력의 유저는 마을로 워프.
	// 5. 전진기지 세력 몬스터로 다시 소환한다.
	// 6. 전진기지 상태값을 변경한다. OUTPOST_WARING

	// 2007-08-20 by dhjin, 전진기지 상태값을 변경한다. OUTPOST_RESETEND
	i_pOutPost->SetOutPostState(OUTPOST_RESETEND);

	// 2007-08-20 by dhjin, 전진기지 소유를 각인 성공한 여단, 세력으로 설정한다.
	i_pOutPost->SetOutPostPossessGuildAndInfluenceByResetSuccess();

	// 2007-08-20 by dhjin, 전진기지 각인 종료를 클라이언트에게 전송
	INIT_MSG_WITH_BUFFER(MSG_FC_OUTPOST_RESET_SUCCESS, T_FC_OUTPOST_RESET_SUCCESS, SendMsg, SendBuf);
	SendMsg->Influence		= i_pOutPost->GetOutPostInfluence();
	SendMsg->MapIndex		= i_pOutPost->GetOutPostMapIndex();
	SendMsg->GuildUID		= i_pOutPost->GetOutPostGuildUID();
	util::strncpy(SendMsg->GuildName, i_pOutPost->GetOutPostGuildName(), SIZE_MAX_GUILD_NAME);
	m_pFieldIOCP18->SendMessageToAllClients(SendBuf,MSG_SIZE(MSG_FC_OUTPOST_RESET_SUCCESS),INFLUENCE_TYPE_VCN|INFLUENCE_TYPE_ANI);

	// 2007-08-20 by dhjin, 전진기지 소유 세력이 아닌 세력의 유저는 마을로 워프.
	CFieldMapChannel *pFMChann = m_pFieldIOCP18->GetFieldMapChannelByMapChannelIndex(MAP_CHANNEL_INDEX(i_pOutPost->GetOutPostMapIndex(), 0));
	if(NULL == pFMChann)
	{
		// 2009-07-08 by cmkwon, 전진 기지전 버그 수정 - 
		g_pFieldGlobal->WriteSystemLogEX(TRUE, "  [Notify] OutPostWarNoti COutPostManager::OutPostResetSuccess# OutpostWarMapIndex(%d) pFMChann(0x%X) \r\n", i_pOutPost->GetOutPostMapIndex(), pFMChann);
		return;
	}

// 2009-07-08 by cmkwon, 전진 기지전 버그 수정 - COutPostManager::OnDoMinutelyWorkOutPostManager()에서 처리하도록 수정함.
//	pFMChann->WarpToCityMapFieldMapChannelByInfluence(i_pOutPost->GetOutPostInfluence());
	///////////////////////////////////////////////////////////////////////////////
	// 2009-07-08 by cmkwon, 전진 기지전 버그 수정 - 아래와 같이 결과를 전달하여 처리하도록 수정함.
	SOUTPOSTWAR_EV_RESET_SUCCESS tmReset;
	util::zero(&tmReset, sizeof(tmReset));
	tmReset.bySuccessInfluenceTy	= i_pOutPost->GetOutPostInfluence();
	tmReset.pFMChanOfOutPostWarMap	= pFMChann;
	io_pVectEvResetSuccessList->push_back(tmReset);

	pFMChann->m_ChangeBodyConMonsterIndex	= 0;					// 2007-09-10 by dhjin, 중앙처리 장치 바디컨디션 바꾸는 몬스터 Index를 0으로 설정한다.	

	// 2007-08-20 by dhjin, 전진기지 세력 몬스터로 다시 소환한다.
	INIT_MSG_WITH_BUFFER(MSG_FN_MONSTER_OUTPOST_INIT, T_FN_MONSTER_OUTPOST_INIT, pSummon, SendBuf2);
	pSummon->mapChann	= pFMChann->GetMapChannelIndex();
	this->SetInitOutPostMonsterSummonByBell(i_pOutPost->GetOutPostInfluence(), &(pSummon->bell1), &(pSummon->bell2), &(pSummon->bell3) );
	pFMChann->Send2NPCServerW(SendBuf2, MSG_SIZE(MSG_FN_MONSTER_OUTPOST_INIT));

	// 2007-10-02 by dhjin, 테스트 용
	g_pFieldGlobal->WriteSystemLogEX(TRUE, "  [Notify] OutPostWarNoti OutPostResetSuccess  OUTPOST_INIT bell1 : [%6d], bell2 : [%6d], bell3 : [%6d]\r\n"
		, pSummon->bell1, pSummon->bell2, pSummon->bell3);

	// 2007-08-20 by dhjin, 전진기지 상태값을 변경한다. OUTPOST_WARING
	i_pOutPost->SetOutPostState(OUTPOST_WARING);
	
	// 2007-10-16 by cmkwon, 로그 추가 - 
	CAtumLogSender::SendLogMessageOutPostResetComplete(i_pOutPost->GetOutPostMapIndex(), i_pOutPost->GetOutPostInfluence(), i_pOutPost->GetOutPostGuildUID());
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void COutPostManager::OutPostWarEnd(COutPost * i_pOutPost)
/// \brief		전진기지 전 종료	
///				// 2009-07-08 by cmkwon, 전진 기지전 버그 수정 - 인자추가(, vectSOUTPOSTWAR_EV_END *io_pEvEndList)
/// \author		dhjin
/// \date		2007-08-20 ~ 2007-08-20
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void COutPostManager::OutPostWarEnd(COutPost * i_pOutPost, vectSOUTPOSTWAR_EV_END *io_pEvEndList)
{
	//////////////////////////////////////////////////////////////////////////
	// 1. 전진기지 상태값을 변경한다. OUTPOST_WAREND
	// 2. 전진기지 몬스터를 초기화한다.
	// 3. 전진기지 최종 소유자를 설정한다.
	// 4. 전진기지전 종료를 클라이언트에게 전송한다.
	// 5. 전진기지 상태값을 변경한다. OUTPOST_NORMAL
	// 6. 전진기지 맵에 있는 유저를 조건에 맞게 워프시킨다. (소유 여단이 있다면 소유 여단원은 전진기지 도시 맵으로 워프)
	// 7. 판공비를 설정한다.

	///////////////////////////////////////////////////////////////////////////////
	// 2009-07-08 by cmkwon, 전진 기지전 버그 수정 - 
	SOUTPOSTWAR_EV_END	tmEvEnd;
	util::zero(&tmEvEnd, sizeof(tmEvEnd));
	

	// 2007-08-20 by dhjin, 전진기지 상태값을 변경한다. OUTPOST_WAREND
	i_pOutPost->SetOutPostState(OUTPOST_WAREND);
	
	
	// 2007-08-20 by dhjin, 전진기지 몬스터를 초기화한다.
	CFieldMapChannel *pFMChann = m_pFieldIOCP18->GetFieldMapChannelByMapChannelIndex(MAP_CHANNEL_INDEX(i_pOutPost->GetOutPostMapIndex(), 0));
	if(NULL == pFMChann)
	{
		// 2009-07-08 by cmkwon, 전진 기지전 버그 수정 - 
		g_pFieldGlobal->WriteSystemLogEX(TRUE, "  [Notify] OutPostWarNoti COutPostManager::OutPostWarEnd# OutpostWarMapIndex(%d) pFMChann(0x%X) \r\n", i_pOutPost->GetOutPostMapIndex(), pFMChann);
		return;
	}

	INIT_MSG_WITH_BUFFER(MSG_FN_NPCSERVER_DELETE_MONSTER_IN_MAPCHANNEL, T_FN_NPCSERVER_DELETE_MONSTER_IN_MAPCHANNEL, pMonster, SendBuf);
	pMonster->mapChann = pFMChann->GetMapChannelIndex();
	pMonster->bAllFlag = TRUE;
	m_pFieldIOCP18->Send2NPCServerByTCP(SendBuf, MSG_SIZE(MSG_FN_NPCSERVER_DELETE_MONSTER_IN_MAPCHANNEL));

	
	// 2007-08-20 by dhjin, 전진기지 최종 소유자를 설정한다.
	// 1. 우선 전진기지 소유 값을 설정하고
	// 2. NPC소유 인지 여단 소유 인지 체크하여 다음 전쟁 시간 설정을 결정한다.
	i_pOutPost->SetOutPostPossessByWarEnd();
	if(FALSE == i_pOutPost->GetOutPostGuildUID()
		&& FALSE == i_pOutPost->GetOutPostInfluence())
	{// 2007-08-20 by dhjin, NPC소유가 된 전진기지는 다음 전쟁 시간을 설정한다.
		if(0 == m_vectOutPostNextWarTimeInfo.size())
		{// 2007-08-20 by dhjin, DB값이 설정 되어 있지 않으면 임의로 설정한다.
			ATUM_DATE_TIME tmNextTime;
			tmNextTime.SetDateTime(0,0,0,20,0,0);
			i_pOutPost->SetOutPostNextWarTime(&tmNextTime);
		}
		else
		{
			// 2007-09-03 by dhjin, 2번째 설정해둔 여단 기지 전 시간으로 설정.
			vectATUM_DATE_TIME::iterator itr = m_vectOutPostNextWarTimeInfo.begin();
			itr++;
			i_pOutPost->SetOutPostNextWarTime(&*itr);
		}
		i_pOutPost->SetOutPostNextWarTimeSet(TRUE);
		// 2007-08-21 by dhjin, DB 업데이트
		QPARAM_SET_NPCPOSSESSION_OUTPOST *pQParam = new QPARAM_SET_NPCPOSSESSION_OUTPOST;
		pQParam->MapIndex			= i_pOutPost->GetOutPostMapIndex();
		pQParam->GetTime			= *(i_pOutPost->GetPotPostGetTime());
		pQParam->NextWarTime		= *(i_pOutPost->GetOutPostNextWarTime());
		m_pFieldIOCP18->m_pAtumDBManager->MakeAndEnqueueQuery(QT_SetNPCPossessionOutPost, NULL, NULL, pQParam);

	}
	else
	{// 2007-08-21 by dhjin, 여단 소유 전진기지이다.
		// 2007-09-03 by dhjin, 2번째 설정해둔 여단 기지 전 시간으로 설정.
		vectATUM_DATE_TIME::iterator itr = m_vectOutPostNextWarTimeInfo.begin();
		itr++;
		i_pOutPost->SetOutPostNextWarTime(&*itr);
		i_pOutPost->SetOutPostNextWarTimeSet(FALSE);
		// 2007-08-21 by dhjin, DB 업데이트
		QPARAM_SET_GUILDPOSSESSION_OUTPOST *pQParam = new QPARAM_SET_GUILDPOSSESSION_OUTPOST;
		pQParam->MapIndex			= i_pOutPost->GetOutPostMapIndex();
		pQParam->Influence			= i_pOutPost->GetOutPostInfluence();
		pQParam->GuildUID			= i_pOutPost->GetOutPostGuildUID();
		pQParam->GetTime			= *(i_pOutPost->GetPotPostGetTime());
		pQParam->NextWarTime		= *(i_pOutPost->GetOutPostNextWarTime());
		m_pFieldIOCP18->m_pAtumDBManager->MakeAndEnqueueQuery(QT_SetGuildPossessionOutPost, NULL, NULL, pQParam);

#ifdef _OUTPOST_BUFFS
		m_pFieldIOCP18->DeleteBuffSkillAllCharacter(INFLUENCE_TYPE_ANI | INFLUENCE_TYPE_VCN, DES_SKILLTYPE_OUTPOST_BUFF);

		int itemNum = 0;
		switch (i_pOutPost->GetOutPostMapIndex())
		{
			case 6016:
				itemNum = OUTPOST_BUFF_SKILL_AKRON;
				break;
			case 6017:
				itemNum = OUTPOST_BUFF_SKILL_RAKION;
				break;
			case 6018:
				itemNum = OUTPOST_BUFF_SKILL_CORONADO;
				break;
			case 4303:
				itemNum = OUTPOST_BUFF_SKILL_NGC_MILITARY_BASE;
				break;
		}
		
		if (itemNum != 0)
		{
			m_pFieldIOCP18->ApplyBuffSkillToGuild(itemNum, i_pOutPost->GetOutPostGuildUID());
		}
#endif
		// 2008-01-07 by dhjin, 세력 보상 수정 - 
		BYTE LoseInfluence = INFLUENCE_TYPE_UNKNOWN;
		BYTE WinInfluence = i_pOutPost->GetOutPostInfluence();		// 2008-05-20 by dhjin, 승리 세력 관련 버그 수정. pQParam 값 사용하는 부분 수정
		if(INFLUENCE_TYPE_VCN == WinInfluence)			// 2008-05-20 by dhjin, 승리 세력 관련 버그 수정. pQParam 값 사용하는 부분 수정
		{
			LoseInfluence = INFLUENCE_TYPE_ANI;
		}
		else
		{
			LoseInfluence = INFLUENCE_TYPE_VCN;
		}

// 2009-07-08 by cmkwon, 전진 기지전 버그 수정 - 보상 처리는 COutPostManager::OnDoMinutelyWorkOutPostManager()에서 처리하고 수정
// 		// 2007-09-14 by dhjin, 승리 세력에게 WarPoint를 지급한다.
// 		pFMChann->AddWarPointInMap(OUTPOST_PAY_WARPOINT, FALSE, WinInfluence, WPUT_OUTPOSTWAR);				// 2009-01-22 by cmkwon, 전쟁 관련 게임 로그 수정 - CFieldMapChannel::AddWarPointInMap()에 인자(, BYTE i_byWPUpdateTy=WPUT_GENERAL) 추가, // 2008-05-20 by dhjin, 승리 세력 관련 버그 수정. pQParam 값 사용하는 부분 수정
// 		pFMChann->AddWarPointInMap(OUTPOST_PAY_WARPOINT_BY_LOSE, FALSE, LoseInfluence, WPUT_OUTPOSTWAR);	// 2009-01-22 by cmkwon, 전쟁 관련 게임 로그 수정 - CFieldMapChannel::AddWarPointInMap()에 인자(, BYTE i_byWPUpdateTy=WPUT_GENERAL) 추가, // 2008-01-07 by dhjin, 세력 보상 수정 - 
// 		pFMChann->InsertItemInMap(OUTPOST_PAY_ITEM, OUTPOST_PAY_ITEM_COUNT, WinInfluence);	// 2008-05-20 by dhjin, 승리 세력 관련 버그 수정. pQParam 값 사용하는 부분 수정 // 2007-11-06 by dhjin, 아이템 지급 추가 
// 
// 		CFieldMapChannel		*pBackFMChann = m_pFieldIOCP18->GetFieldMapChannelByMapChannelIndex(MAP_CHANNEL_INDEX(pFMChann->m_pFieldMapProject->m_nBeforeMapIndex, 0));
// 		if(pBackFMChann)
// 		{
// 			pBackFMChann->AddWarPointInMap(OUTPOST_PAY_WARPOINT, FALSE, WinInfluence, WPUT_OUTPOSTWAR);				// 2009-01-22 by cmkwon, 전쟁 관련 게임 로그 수정 - CFieldMapChannel::AddWarPointInMap()에 인자(, BYTE i_byWPUpdateTy=WPUT_GENERAL) 추가// 2008-05-20 by dhjin, 승리 세력 관련 버그 수정. pQParam 값 사용하는 부분 수정
// 			pBackFMChann->AddWarPointInMap(OUTPOST_PAY_WARPOINT_BY_LOSE, FALSE, LoseInfluence, WPUT_OUTPOSTWAR);	// 2009-01-22 by cmkwon, 전쟁 관련 게임 로그 수정 - CFieldMapChannel::AddWarPointInMap()에 인자(, BYTE i_byWPUpdateTy=WPUT_GENERAL) 추가	// 2008-01-07 by dhjin, 세력 보상 수정 - 
// 			pBackFMChann->InsertItemInMap(OUTPOST_PAY_ITEM, OUTPOST_PAY_ITEM_COUNT, WinInfluence);	// 2008-05-20 by dhjin, 승리 세력 관련 버그 수정. pQParam 값 사용하는 부분 수정	// 2007-11-06 by dhjin, 아이템 지급 추가 
// 		}
// 
// 		// 2008-06-20 by cmkwon, EP3 백맵 시스템 수정(맵당 백맵을 2개 설정 할 수 있다) - 전진기지전 종료시 백맵2 에도 보상
// 		pBackFMChann = m_pFieldIOCP18->GetFieldMapChannelByMapChannelIndex(MAP_CHANNEL_INDEX(pFMChann->m_pFieldMapProject->m_nBeforeMapIndex2, 0));
// 		if(pBackFMChann)
// 		{
// 			pBackFMChann->AddWarPointInMap(OUTPOST_PAY_WARPOINT, FALSE, WinInfluence, WPUT_OUTPOSTWAR);				// 2009-01-22 by cmkwon, 전쟁 관련 게임 로그 수정 - CFieldMapChannel::AddWarPointInMap()에 인자(, BYTE i_byWPUpdateTy=WPUT_GENERAL) 추가// 2008-05-20 by dhjin, 승리 세력 관련 버그 수정. pQParam 값 사용하는 부분 수정
// 			pBackFMChann->AddWarPointInMap(OUTPOST_PAY_WARPOINT_BY_LOSE, FALSE, LoseInfluence, WPUT_OUTPOSTWAR);	// 2009-01-22 by cmkwon, 전쟁 관련 게임 로그 수정 - CFieldMapChannel::AddWarPointInMap()에 인자(, BYTE i_byWPUpdateTy=WPUT_GENERAL) 추가		// 2008-01-07 by dhjin, 세력 보상 수정 - 
// 			pBackFMChann->InsertItemInMap(OUTPOST_PAY_ITEM, OUTPOST_PAY_ITEM_COUNT, WinInfluence);	// 2008-05-20 by dhjin, 승리 세력 관련 버그 수정. pQParam 값 사용하는 부분 수정	// 2007-11-06 by dhjin, 아이템 지급 추가 
// 		}
		///////////////////////////////////////////////////////////////////////////////
		// 2009-07-08 by cmkwon, 전진 기지전 버그 수정 - 승패 세력 설정
		tmEvEnd.byWinInfluenceTy	= WinInfluence;
		tmEvEnd.byLossInfluenceTy	= LoseInfluence;
	}

	// 2007-08-21 by dhjin, 전진기지전 종료를 클라이언트에게 전송한다.
	INIT_MSG_WITH_BUFFER(MSG_FC_OUTPOST_WAR_END, T_FC_OUTPOST_WAR_END, SendMsg, SendBuf2);
	SendMsg->Influence		= i_pOutPost->GetOutPostInfluence();
	SendMsg->MapIndex		= i_pOutPost->GetOutPostMapIndex();
	SendMsg->GuildUID		= i_pOutPost->GetOutPostGuildUID();
	util::strncpy(SendMsg->GuildName, i_pOutPost->GetOutPostGuildName(), SIZE_MAX_GUILD_NAME);
	m_pFieldIOCP18->SendMessageToAllClients(SendBuf2,MSG_SIZE(MSG_FC_OUTPOST_RESET_SUCCESS),INFLUENCE_TYPE_VCN|INFLUENCE_TYPE_ANI);

// 2013-05-09 by hskim, 세력 포인트 개선
#ifdef S_UPGRADE_INFLUENCE_POINT_HSKIM
	m_pFieldIOCP18->m_InflWarManager.CalcContributionPoint(i_pOutPost->GetOutPostInfluence(), CONTRIBUTIONPOINT_OUTPOST_WAR_WINNER, CONTRIBUTIONPOINT_OUTPOST_WAR_LOSER);
#endif
// end 2013-05-09 by hskim, 세력 포인트 개선	

// 2009-07-08 by cmkwon, 전진 기지전 버그 수정 - 보상 처리는 COutPostManager::OnDoMinutelyWorkOutPostManager()에서 처리하고 수정	
// 	//////////////////////////////////////////////////////////////////////////
// 	// 2008-12-23 by dhjin, 전쟁 보상 추가안
// 	m_pFieldIOCP18->PayCOutPostWarContributionCalculation(i_pOutPost->GetOutPostMapIndex(), i_pOutPost->GetOutPostInfluence());
// 	m_pFieldIOCP18->DeleteCOutPostWarContributionCalculation(i_pOutPost->GetOutPostMapIndex());	

	// 2009-02-11 by dhjin, 시스템 로그 보완
	g_pFieldGlobal->WriteSystemLogEX(TRUE, "  [Notify] OutPostWarNoti COutPostManager::OutPostWarEnd# 10000 OutPostMapIndex(%6d) \r\n", i_pOutPost->GetOutPostMapIndex());

	// 2007-08-21 by dhjin, 전진기지 상태값을 변경한다. OUTPOST_NORMAL
	i_pOutPost->SetOutPostState(OUTPOST_NORMAL);
	m_pFieldIOCP18->m_InflWarManager.SetDoingInfluenceWar();	// 2012-11-26 by jhseol, 전쟁 시스템 리뉴얼 - 전쟁시 트리거 무적상태(일시정지), 전쟁 종료를 실시간으로 확인 하기 위해 함수호출(전진기지전 종료)

	//////////////////////////////////////////////////////////////////////////
	// 2007-08-21 by dhjin, 전진기지 맵에 있는 유저를 조건에 맞게 워프시킨다. (소유 여단이 있다면 소유 여단원은 전진기지 도시 맵으로 워프)
	// 1. 우선 소유 여단UID를 제외한 모든 유저 마을로 워프 (NPC가 소유하면 여단 번호가 0이기 때문에 상관없이 모두 워프된다.)
	// 2. 만약 소유가 NPC가 아니라면 소유 여단원을 전진기지 도시 맵으로 워프시킨다.

// 2009-07-08 by cmkwon, 전진 기지전 버그 수정 - 보상 처리는 COutPostManager::OnDoMinutelyWorkOutPostManager()에서 처리하고 수정
//	pFMChann->WarpToCityMapFieldMapChannelByGuildUID(i_pOutPost->GetOutPostGuildUID());
// 	if(FALSE != i_pOutPost->GetOutPostGuildUID()
// 		&& FALSE != i_pOutPost->GetOutPostInfluence()
// 		&& FALSE == i_pOutPost->GetOutPostNextWarTimeSet())
// 	{
// 		pFMChann->WarpToOutPostCityMapFieldMapChannelByGuildUID(i_pOutPost->GetOutPostGuildUID(), i_pOutPost->GetOutPostCityMapIndex());
// 	}
	///////////////////////////////////////////////////////////////////////////////
	// 2009-07-08 by cmkwon, 전진 기지전 버그 수정 - 아래와 같이 결과를 넘겨서 처리하도록 한다.
	tmEvEnd.GuildUIDPossessOutPost		= i_pOutPost->GetOutPostGuildUID();
	tmEvEnd.pFMChanOfOutPostWarMap		= pFMChann;
	if(FALSE != i_pOutPost->GetOutPostGuildUID()
		&& FALSE != i_pOutPost->GetOutPostInfluence()
		&& FALSE == i_pOutPost->GetOutPostNextWarTimeSet())
	{
		tmEvEnd.nCityMapIndexForWarp	= i_pOutPost->GetOutPostCityMapIndex();
	}
	io_pEvEndList->push_back(tmEvEnd);


	// 2009-02-11 by dhjin, 시스템 로그 보완
	g_pFieldGlobal->WriteSystemLogEX(TRUE, "  [Notify] OutPostWarNoti COutPostManager::OutPostWarEnd# 10100 OutPostMapIndex(%6d) \r\n", i_pOutPost->GetOutPostMapIndex());

	//////////////////////////////////////////////////////////////////////////
	// 2007-08-27 by dhjin, 판공비를 설정한다.
	this->SetExpediencyFundByOutPostWarEnd(i_pOutPost->GetOutPostMapIndex(), i_pOutPost->GetOutPostCityMapIndex(), i_pOutPost->GetOutPostInfluence(), i_pOutPost->GetOutPostGuildUID());

	// 2009-02-11 by dhjin, 시스템 로그 보완
	g_pFieldGlobal->WriteSystemLogEX(TRUE, "  [Notify] OutPostWarNoti COutPostManager::OutPostWarEnd# 10200 OutPostMapIndex(%6d) \r\n", i_pOutPost->GetOutPostMapIndex());

	// 2007-10-02 by dhjin, 로그 남기기
	// 2007-10-16 by cmkwon, 로그 추가 - SendLogMessageOutPostEnd() 함수 인자 수정
	CAtumLogSender::SendLogMessageOutPostEnd(i_pOutPost->GetOutPostMapIndex(), i_pOutPost->GetOutPostInfluence(), i_pOutPost->GetOutPostGuildUID(), i_pOutPost->GetOutPostGuildName());

	//////////////////////////////////////////////////////////////////////////
	// 2008-05-21 by dhjin, EP3 - 여단 수정 사항 - 전진기지 관련
	// 2007-10-16 by cmkwon, 로그 추가 - SendLogMessageOutPostEnd() 함수 인자 수정
	INIT_MSG_WITH_BUFFER(MSG_FI_GUILD_OUTPOST, T_FI_GUILD_OUTPOST, pGuildOutPost, GuildOutPostSendBuf);
	pGuildOutPost->GuildUID			= i_pOutPost->GetOutPostGuildUID();
	pGuildOutPost->MapIndex			= i_pOutPost->GetOutPostCityMapIndex();
	m_pFieldIOCP18->Send2IMServer(GuildOutPostSendBuf, MSG_SIZE(MSG_FI_GUILD_OUTPOST));


}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void COutPostManager::SetExpediencyFundByOutPostWarEnd(int i_nMapindex, int i_nCityMapindex, byte i_byInfluence, UID32_t i_nGuildUID)
/// \brief		판공비 설정
/// \author		dhjin
/// \date		2007-08-27 ~ 2007-08-27
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void COutPostManager::SetExpediencyFundByOutPostWarEnd(int i_nMapindex, int i_nCityMapindex, byte i_byInfluence, UID32_t i_nGuildUID)
{
	//////////////////////////////////////////////////////////////////////////
	// 1. 판공비율 정보 설정(현재는 지도자만 변화한다.)
	// 2. 판공비 설정 초기화
	UID32_t GuildCommanderUID = 0;

	if(FALSE != i_nGuildUID)
	{// 2007-08-27 by dhjin, NPC소유가 아니면 여단장 UID를 가져온다.
		GuildCommanderUID = m_pFieldIOCP18->m_GuildWarManager.GetGuildCommanderUID(i_nGuildUID);
	}

	if(FALSE == i_nGuildUID
		&& FALSE == i_byInfluence)
	{// 2007-08-27 by dhjin, NPC소유가 된 전진기지의 판공비 설정.
		// 1. 지도자 혜택 줄임
		SCITYLEADER_INFO	tmCityLeaderInfo;
		util::zero(&tmCityLeaderInfo, sizeof(SCITYLEADER_INFO));
		m_pFieldIOCP18->m_CityLeader.GetExpediencyfund(i_nCityMapindex, &tmCityLeaderInfo);
		if(INFLUENCE_TYPE_VCN == tmCityLeaderInfo.Influence)
		{// 2007-08-27 by dhjin, NPC소유 전에 바이제니유 여단이 소유하고 있었다면 바이제니유 지도자 혜택을 줄인다.
			this->UpdateExpediencyfundRateLeader(VCN_CITY_MAP_INDEX, INFLUENCE_TYPE_VCN, -EXPEDIENCYFUND_LEADER_OUTPOST);
		}
		else if(INFLUENCE_TYPE_ANI == tmCityLeaderInfo.Influence)
		{// 2007-08-27 by dhjin, NPC소유 전에 알링턴 여단이 소유하고 있었다면 알링턴 지도자 혜택을 줄인다.
			this->UpdateExpediencyfundRateLeader(ANI_CITY_MAP_INDEX, INFLUENCE_TYPE_ANI, -EXPEDIENCYFUND_LEADER_OUTPOST);
		}

		// 2. 전진기지 판공비 초기화
		m_pFieldIOCP18->m_CityLeader.InitExpediencyfund(i_nCityMapindex, i_byInfluence, GuildCommanderUID, EXPEDIENCYFUND_GUILDCOMMANDER);
		QPARAM_INIT_EXPEDIENCY_FUND *pQParam = new QPARAM_INIT_EXPEDIENCY_FUND;
		pQParam->MapIndex					= i_nCityMapindex;
		pQParam->Influence					= i_byInfluence;
		pQParam->CharacterUID				= GuildCommanderUID;
		pQParam->ExpediencyFundCumulative	= 0;
		pQParam->ExpediencyFund				= 0;
		pQParam->ExpediencyRate				= EXPEDIENCYFUND_GUILDCOMMANDER;
		m_pFieldIOCP18->m_pAtumDBManager->MakeAndEnqueueQuery(QT_InitExpediencyFund, NULL, NULL, pQParam);
	}
	else
	{// 2007-08-21 by dhjin, 여단 소유 전진기지이다.
		// 1. 지도자 혜택을 증감한다.
		SCITYLEADER_INFO	tmCityLeaderInfo;
		util::zero(&tmCityLeaderInfo, sizeof(SCITYLEADER_INFO));
		m_pFieldIOCP18->m_CityLeader.GetExpediencyfund(i_nCityMapindex, &tmCityLeaderInfo);
		
		// 2007-08-27 by dhjin, 전에 점령한 같은 소유의 세력이 점령하였다면 지도자 혜택에 변화는 없다.
		if(INFLUENCE_TYPE_UNKNOWN == tmCityLeaderInfo.Influence && INFLUENCE_TYPE_ANI == i_byInfluence) 	// 2007-10-22 by dhjin, 자신의 세력 소유일때만 혜택없게 수정.
		{// 2007-08-27 by dhjin, 전에 점령한 여단이 NPC이고 새로 점령한 여단이 알링턴이면 알링턴 지도자 혜택은 증가시킨다.
//			this->UpdateExpediencyfundRateLeader(VCN_CITY_MAP_INDEX, INFLUENCE_TYPE_ANI, EXPEDIENCYFUND_LEADER_OUTPOST);
			this->UpdateExpediencyfundRateLeader(ANI_CITY_MAP_INDEX, INFLUENCE_TYPE_ANI, EXPEDIENCYFUND_LEADER_OUTPOST);	// 2009-02-17 by dhjin, 판공비 버그 수정(K0001196) 
		}
		else if(INFLUENCE_TYPE_UNKNOWN == tmCityLeaderInfo.Influence && INFLUENCE_TYPE_VCN == i_byInfluence) 	// 2007-10-22 by dhjin, 자신의 세력 소유일때만 혜택없게 수정.
		{// 2007-08-27 by dhjin, 전에 점령한 여단이 NPC이고 새로 점령한 여단이 바이제니유이면 바이제니유 지도자 혜택은 증가시킨다.
			this->UpdateExpediencyfundRateLeader(VCN_CITY_MAP_INDEX, INFLUENCE_TYPE_VCN, EXPEDIENCYFUND_LEADER_OUTPOST);
		}
		else if(INFLUENCE_TYPE_VCN == tmCityLeaderInfo.Influence && INFLUENCE_TYPE_ANI == i_byInfluence)	// 2007-10-22 by dhjin, 자신의 세력 소유일때만 혜택없게 수정.
		{// 2007-08-27 by dhjin, 전에 점령한 여단이 바이제니유이고 새로 점령한 여단이 알링턴이면 바이제니유 지도자 혜택은 줄이고 알링턴 지도자 혜택은 증가시킨다.
			this->UpdateExpediencyfundRateLeader(VCN_CITY_MAP_INDEX, INFLUENCE_TYPE_VCN, -EXPEDIENCYFUND_LEADER_OUTPOST);
			this->UpdateExpediencyfundRateLeader(ANI_CITY_MAP_INDEX, INFLUENCE_TYPE_ANI, EXPEDIENCYFUND_LEADER_OUTPOST);
		}
		else if(INFLUENCE_TYPE_ANI == tmCityLeaderInfo.Influence && INFLUENCE_TYPE_VCN == i_byInfluence) 	// 2007-10-22 by dhjin, 자신의 세력 소유일때만 혜택없게 수정.
		{// 2007-08-27 by dhjin, 전에 점령한 여단이 바이제니유이고 새로 점령한 여단이  바이제니유이면 알링턴 지도자 혜택은 줄이고 바이제니유 지도자 혜택은 증가시킨다.
			this->UpdateExpediencyfundRateLeader(VCN_CITY_MAP_INDEX, INFLUENCE_TYPE_VCN, EXPEDIENCYFUND_LEADER_OUTPOST);
			this->UpdateExpediencyfundRateLeader(ANI_CITY_MAP_INDEX, INFLUENCE_TYPE_ANI, -EXPEDIENCYFUND_LEADER_OUTPOST);
		}

		// 2. 전진기지 판공비 초기화
		m_pFieldIOCP18->m_CityLeader.InitExpediencyfund(i_nCityMapindex, i_byInfluence, GuildCommanderUID, EXPEDIENCYFUND_GUILDCOMMANDER);
		QPARAM_INIT_EXPEDIENCY_FUND *pQParam = new QPARAM_INIT_EXPEDIENCY_FUND;
		pQParam->MapIndex					= i_nCityMapindex;
		pQParam->Influence					= i_byInfluence;
		pQParam->CharacterUID				= GuildCommanderUID;
		pQParam->ExpediencyFundCumulative	= 0;
		pQParam->ExpediencyFund				= 0;
		pQParam->ExpediencyRate				= EXPEDIENCYFUND_GUILDCOMMANDER;
		m_pFieldIOCP18->m_pAtumDBManager->MakeAndEnqueueQuery(QT_InitExpediencyFund, NULL, NULL, pQParam);
	}	
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void COutPostManager::UpdateExpediencyfundRateLeader(int i_nMapIndx, byte i_byInfluence, int i_nChangeExpediencyFundRate)
/// \brief		지도자 판공비율 설정을 증감한다.
/// \author		dhjin
/// \date		2007-08-28 ~ 2007-08-28
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void COutPostManager::UpdateExpediencyfundRateLeader(int i_nMapIndx, byte i_byInfluence, int i_nChangeExpediencyFundRate)
{
	SCITYLEADER_INFO tmCityLeaderInfo;
	util::zero(&tmCityLeaderInfo, sizeof(SCITYLEADER_INFO));
	m_pFieldIOCP18->m_CityLeader.UpdateExpediencyfundRateATLeader(i_nMapIndx, i_nChangeExpediencyFundRate, &tmCityLeaderInfo);
	
	QPARAM_SET_EXPEDIENCY_FUNDRATE *pQParam = new QPARAM_SET_EXPEDIENCY_FUNDRATE;
	pQParam->MapIndex					= i_nMapIndx;
	pQParam->Influence					= i_byInfluence;
	pQParam->CharacterUID				= tmCityLeaderInfo.CharacterUID;
	pQParam->ExpediencyRate				= tmCityLeaderInfo.ExpediencyFundRate;
	m_pFieldIOCP18->m_pAtumDBManager->MakeAndEnqueueQuery(QT_SetExpediencyFundRate, NULL, NULL, pQParam);

}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void COutPostManager::SetInitOutPostMonsterSummonByBell(byte i_byInfl, int * o_byBell1, int * o_byBell2, int * o_byBell3)
/// \brief		전진기지 초기화 할 때 몬스터를 생성하기 위한 Bell값 설정
/// \author		dhjin
/// \date		2007-08-24 ~ 2007-08-24
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void COutPostManager::SetInitOutPostMonsterSummonByBell(byte i_byInfl, int * o_byBell1, int * o_byBell2, int * o_byBell3)
{
	// 2007-08-24 by dhjin, Bell 기본값 설정 -1 이어야 한다.
	*o_byBell1 = -1;
	*o_byBell2 = -1;
	*o_byBell3 = -1;

	// 2007-08-24 by dhjin, NPC 소유일때
	if(INFLUENCE_TYPE_UNKNOWN == i_byInfl)
	{
		*o_byBell1 = BELL_ATATTACK;
		*o_byBell2 = BELL_ATTACK_OUTPOST_PROTECTOR;

	}
	else if(INFLUENCE_TYPE_VCN == i_byInfl)
	{
		*o_byBell1 = BELL_INFLUENCE_VCN;
		*o_byBell2 = BELL_OUTPOST_PROTECTOR_VCN;
	}
	else if(INFLUENCE_TYPE_ANI== i_byInfl)
	{
		*o_byBell1 = BELL_INFLUENCE_ANI;
		*o_byBell2 = BELL_OUTPOST_PROTECTOR_ANI;
	}

}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void COutPostManager::AddDamageOfProtectorW(int i_nMapIndex, BYTE i_byInfluence, UID32_t i_nGuildUID, CHAR * i_szGuildName, float i_fDamage)
/// \brief		전진기지 보호막에 데미지를 준 여단과 데미지 값을 저장한다.
/// \author		dhjin
/// \date		2007-08-24 ~ 2007-08-24
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void COutPostManager::AddDamageOfProtectorW(int i_nMapIndex, BYTE i_byInfluence, UID32_t i_nGuildUID, CHAR * i_szGuildName, float i_fDamage)
{
	mt_auto_lock mta(&m_mtvectOutPostManager);

	COutPost * tmOutPost = this->GetOutPost(i_nMapIndex);
	if(NULL == tmOutPost)
	{
		return;
	}

	tmOutPost->AddDamageOfProtector(i_byInfluence, i_nGuildUID, i_szGuildName, i_fDamage);

}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void COutPostManager::MakeMsgOutPostNextWarInfoRequest(int i_nMapIndex, BOOL i_bCheckGuildCommander, BYTE * o_pData, int * o_nSize)
/// \brief		전진기지 시간 설정 정보 전송
/// \author		dhjin
/// \date		2007-08-28 ~ 2007-08-28
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void COutPostManager::MakeMsgOutPostNextWarInfoRequest(int i_nMapIndex, BOOL i_bCheckGuildCommander, BYTE * o_pData, int * o_nSize)
{
	mt_auto_lock mta(&m_mtvectOutPostManager);

	COutPost * tmOutPost = this->GetOutPostCityMap(i_nMapIndex);
	if(NULL == tmOutPost)
	{
		return;
	}
	
	INIT_MSG_WITH_BUFFER(MSG_FC_OUTPOST_NEXTWAR_INFO_REQUEST_OK, T_FC_OUTPOST_NEXTWAR_INFO_REQUEST_OK, SendMsg, SendBuf);
	SendMsg->OutPostNextWarTime		= *(tmOutPost->GetOutPostNextWarTime());
	if(i_bCheckGuildCommander)
	{// 2007-08-28 by dhjin, 여단장이면 선택하는 시간 값 목록을 전송한다.
		SendMsg->OutPostNextWarTimeListCount = MakeMSG_FC_OUTPOST_NEXTWAR_INFO_REQUEST_OK((ATUM_DATE_TIME*)(SendBuf+MSG_SIZE(MSG_FC_OUTPOST_NEXTWAR_INFO_REQUEST_OK)), &(SendMsg->OutPostNextWarSelectTimeChoice), &(SendMsg->OutPostNextWarTime));
		if(0 > SendMsg->OutPostNextWarTimeListCount)
		{
			return;
		}
		
	}

	*o_nSize = MSG_SIZE(MSG_FC_OUTPOST_NEXTWAR_INFO_REQUEST_OK)+SendMsg->OutPostNextWarTimeListCount*sizeof(ATUM_DATE_TIME);
	memcpy(o_pData, SendBuf, *o_nSize);
	
}

INT	COutPostManager::MakeMSG_FC_OUTPOST_NEXTWAR_INFO_REQUEST_OK(ATUM_DATE_TIME * o_pOutPostNextWarTimeListInfo, int * o_nSelectTimeChoice, ATUM_DATE_TIME * i_OutPostNextWarTime)
{
	INT	OutPostNextWarTimeListCount = 0;

	vectATUM_DATE_TIME::iterator itr(m_vectOutPostNextWarTimeInfo.begin());
	for(; itr != m_vectOutPostNextWarTimeInfo.end(); itr++)
	{
		if(i_OutPostNextWarTime->Hour == itr->Hour
			&& i_OutPostNextWarTime->Minute == itr->Minute
			&& i_OutPostNextWarTime->Second == itr->Second)
		{// 2007-08-28 by dhjin, 설정된 시간과 같으면 시간값 선택된 인덱스 값을 설정한다.
			*o_nSelectTimeChoice = OutPostNextWarTimeListCount;
		}
		o_pOutPostNextWarTimeListInfo[OutPostNextWarTimeListCount]		= *itr;
		OutPostNextWarTimeListCount++;
	}

	return OutPostNextWarTimeListCount;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			Err_t COutPostManager::OutPostNextWarTimeSetByChoiceUserW(int i_nMapIndex, int i_nSelectTimeChoice, UID32_t i_guildUID, UID32_t i_charUID)
/// \brief		// 2007-10-16 by cmkwon, 로그 추가 - 인자추가(, UID32_t i_guildUID, UID32_t i_charUID), 전진기지 다음 전투 시간 설정
/// \author		dhjin
/// \date		2007-08-28 ~ 2007-08-28
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
Err_t COutPostManager::OutPostNextWarTimeSetByChoiceUserW(int i_nMapIndex, int i_nSelectTimeChoice, UID32_t i_guildUID, UID32_t i_charUID)
{
	mt_auto_lock mta(&m_mtvectOutPostManager);

	COutPost * tmOutPost = this->GetOutPostCityMap(i_nMapIndex);
	if(NULL == tmOutPost)
	{
		return ERR_CANNOT_SEARCH_OUTPOST;
	}

	if(tmOutPost->GetOutPostNextWarTimeSet())
	{
		return ERR_ALREADY_SET_OUTPOST_NEXTWARTIME;
	}
	
	int TempCount = 0;			// 2007-08-28 by dhjin, 클라이언트가 선택한 시간 순서번째를 찾기 위해 
	vectATUM_DATE_TIME::iterator itr(m_vectOutPostNextWarTimeInfo.begin());
	for(; itr != m_vectOutPostNextWarTimeInfo.end(); itr++)
	{
		if(i_nSelectTimeChoice == TempCount)
		{// 2007-08-28 by dhjin, 클라이언트가 선택한 시간 순서번째와 같은 순서이면 다음 전쟁 시간으로 설정한다.
			tmOutPost->SetOutPostNextWarTimeByChoiceUser(&*itr);
	
			// 2007-09-11 by dhjin, 전진기지 다음 전쟁 시간 설정 불가로 변경
			tmOutPost->SetOutPostNextWarTimeSet(TRUE);
			
			// 2007-09-11 by dhjin, DB에 설정
			QPARAM_SET_OUTPOST_NEXTWAR_TIME *pQParam = new QPARAM_SET_OUTPOST_NEXTWAR_TIME;
			pQParam->MapIndex					= i_nMapIndex;
			pQParam->Influence					= tmOutPost->GetOutPostInfluence();
			pQParam->GuildUID					= tmOutPost->GetOutPostGuildUID();
			pQParam->NextWarTime				= *(tmOutPost->GetOutPostNextWarTime());
			m_pFieldIOCP18->m_pAtumDBManager->MakeAndEnqueueQuery(QT_SetOutPostNextWarTime, NULL, NULL, pQParam);

			// 2007-10-16 by cmkwon, 로그 추가 - 
			CAtumLogSender::SendLogMessageOutPostSetNextTime(tmOutPost->GetOutPostMapIndex(), tmOutPost->GetOutPostInfluence(), tmOutPost->GetOutPostGuildUID(), i_guildUID, i_charUID, tmOutPost->GetOutPostNextWarTime());

			return ERR_NO_ERROR;
		}
		TempCount++;
	}	
	
	return ERR_CANNOT_SEARCH_OUTPOST_NEXTWARTIME;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL COutPostManager::CheckOutPostWarTimeStrategyPoint(ATUM_DATE_TIME *i_pStrategyPointSummonTime)
/// \brief		전략포인트 소환 시 전진기지 전쟁 시간에 걸리는지 체크
/// \author		dhjin
/// \date		2007-09-03 ~ 2007-09-03
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL COutPostManager::CheckOutPostWarTimeStrategyPoint(ATUM_DATE_TIME *i_pStrategyPointSummonTime)
{
	mt_auto_lock mta(&m_mtvectOutPostManager);
	
	mtvectOutPost::iterator	itr = m_mtvectOutPostManager.begin();
	for(; itr != m_mtvectOutPostManager.end(); itr++)
	{
		ATUM_DATE_TIME	tmOutPostWarStartTime; 
		ATUM_DATE_TIME	tmOutPostWarEndTime;
		tmOutPostWarStartTime = *((*itr)->GetOutPostNextWarTime());
		tmOutPostWarStartTime.AddDateTime(0,0,0,-1,0,0);	// 2007-09-03 by dhjin, 전진기지 전쟁 앞뒤로 1시간씩 여유를 둔다.
		tmOutPostWarEndTime = tmOutPostWarStartTime;
		tmOutPostWarEndTime.AddDateTime(0,0,0,4,0,0);
		if(tmOutPostWarStartTime <= *i_pStrategyPointSummonTime
			&& tmOutPostWarEndTime >= *i_pStrategyPointSummonTime)
		{// 2007-09-03 by dhjin, 전진기지 전쟁이 발생하는 시간이면 전략포인트를 생성하지 않는다.
			return FALSE;
		}
	}

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			ATUM_DATE_TIME * COutPostManager::GetOutPostNextWarTime(int i_nMapIndex)
/// \brief		전진기지 다음 전투 시간 얻어오기
/// \author		dhjin
/// \date		2007-09-06 ~ 2007-09-06
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
ATUM_DATE_TIME * COutPostManager::GetOutPostNextWarTime(int i_nMapIndex)
{
	mt_auto_lock mta(&m_mtvectOutPostManager);
	
	COutPost * tmOutPost = this->GetOutPost(i_nMapIndex);
	if(NULL == tmOutPost)
	{
		return NULL;
	}

	return tmOutPost->GetOutPostNextWarTime();
	
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			ATUM_DATE_TIME * COutPostManager::GetOutPostNextWarTimeByCityMapIndex(int i_nCityMapIndex)
/// \brief		도시 맵으로 전진기지 다음 전투 시간 얻어오기
/// \author		dhjin
/// \date		2007-09-11 ~ 2007-09-11
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
ATUM_DATE_TIME * COutPostManager::GetOutPostNextWarTimeByCityMapIndex(int i_nCityMapIndex)
{
	mt_auto_lock mta(&m_mtvectOutPostManager);
	
	COutPost * tmOutPost = this->GetOutPostCityMap(i_nCityMapIndex);
	if(NULL == tmOutPost)
	{
		return NULL;
	}

	return tmOutPost->GetOutPostNextWarTime();
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL COutPostManager::CheckOutPostWaring(int i_nMapIndex)
/// \brief		전진기지 전쟁이 진행중인지 체크
/// \author		dhjin
/// \date		2007-09-07 ~ 2007-09-07
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL COutPostManager::CheckOutPostWaring(int i_nMapIndex)
{
	mt_auto_lock mta(&m_mtvectOutPostManager);
	
	COutPost * tmOutPost = this->GetOutPost(i_nMapIndex);
	if(NULL == tmOutPost)
	{
		return FALSE;
	}

	if(IS_OUTPOST(tmOutPost->GetOutPostState()))
	{
		return TRUE;
	}
	
	return FALSE;	
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL COutPostManager::CheckALLOutPostWaring()
/// \brief		전진기지 전쟁이 진행중인지 체크 모든 맵
/// \author		dhjin
/// \date		2007-11-09 ~ 2007-11-09
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL COutPostManager::CheckALLOutPostWaring()
{
	mt_auto_lock mta(&m_mtvectOutPostManager);
	
	mtvectOutPost::iterator itr = m_mtvectOutPostManager.begin();
	for(; itr != m_mtvectOutPostManager.end(); itr++)
	{
		if(IS_OUTPOST((*itr)->GetOutPostState()))
		{
			return TRUE;
		}
	}	

	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL COutPostManager::CheckOutPostPossessByGuildUID(UID32_t i_nGuildUID)
/// \brief		전진기지를 소유한 여단인지 체크
/// \author		dhjin
/// \date		2007-11-09 ~ 2007-11-09
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL COutPostManager::CheckOutPostPossessByGuildUID(UID32_t i_nGuildUID)
{
	mt_auto_lock mta(&m_mtvectOutPostManager);
	
	mtvectOutPost::iterator itr = m_mtvectOutPostManager.begin();
	for(; itr != m_mtvectOutPostManager.end(); itr++)
	{
		if(i_nGuildUID == (*itr)->GetOutPostGuildUID())
		{
			return TRUE;
		}
	}	

	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL COutPostManager::CheckOutPostResetByGuildUID(UID32_t i_nGuildUID)
/// \brief		전진기지를 리셋중인 여단인지 체크
/// \author		dhjin
/// \date		2007-12-13 ~ 2007-12-13
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL COutPostManager::CheckOutPostResetByGuildUID(UID32_t i_nGuildUID)
{
	mt_auto_lock mta(&m_mtvectOutPostManager);
	
	mtvectOutPost::iterator itr = m_mtvectOutPostManager.begin();
	for(; itr != m_mtvectOutPostManager.end(); itr++)
	{
		if(i_nGuildUID == (*itr)->GetOutPostResetPossibleGuildUID())
		{
			return TRUE;
		}
	}	

	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			INT	COutPostManager::CheckAllOutPostWaringAndSendClient(SOUTPOST_WAR_INFO *o_pOutPostWarInfo)
/// \brief		전진기지 전쟁이 발발했는지 체크하여 클라이언트에게 정보를 전송한다.
/// \author		dhjin
/// \date		2007-09-14 ~ 2007-09-14
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
INT	COutPostManager::CheckAllOutPostWaringAndSendClient(SOUTPOST_WAR_INFO *o_pOutPostWarInfo)
{
	mt_auto_lock mta(&m_mtvectOutPostManager);
	
	int	OutPostWarInfoCount = 0;

	ATUM_DATE_TIME tmCurrentTime;
	tmCurrentTime.SetCurrentDateTime();

	mtvectOutPost::iterator itr = m_mtvectOutPostManager.begin();
	for(; itr != m_mtvectOutPostManager.end(); itr++)
	{
		if(IS_OUTPOST((*itr)->GetOutPostState()))
		{
			o_pOutPostWarInfo[OutPostWarInfoCount].OutPostState			= (*itr)->GetOutPostState();
			o_pOutPostWarInfo[OutPostWarInfoCount].OutPostResetIngInfluence		= (*itr)->GetOutPostResetPossibleInfluence();
			o_pOutPostWarInfo[OutPostWarInfoCount].MapIndex				= (*itr)->GetOutPostMapIndex();
			o_pOutPostWarInfo[OutPostWarInfoCount].OutPostWarStartTime	= *((*itr)->GetOutPostNextWarTime());
			o_pOutPostWarInfo[OutPostWarInfoCount].OutPostWarEndTime	= o_pOutPostWarInfo[OutPostWarInfoCount].OutPostWarStartTime;
			o_pOutPostWarInfo[OutPostWarInfoCount].OutPostWarEndTime.AddDateTime(0,0,0,0,OUTPOST_WARTIME,0);
			if(g_pFieldGlobal->IsTestServer())
			{// 2008-10-29 by cmkwon, 테섭 시스템 수정(전진기지전 1시간(60분)으로 설정) - 
				o_pOutPostWarInfo[OutPostWarInfoCount].OutPostWarEndTime	= o_pOutPostWarInfo[OutPostWarInfoCount].OutPostWarStartTime;
				o_pOutPostWarInfo[OutPostWarInfoCount].OutPostWarEndTime.AddDateTime(0,0,0,0,OUTPOST_WARTIME_FOR_TESTSERVER,0);
			}
			o_pOutPostWarInfo[OutPostWarInfoCount].OutPostWarResetRamainSecondTime = (*itr)->GetOutPostResetEndTime()->GetTimeDiffTimeInSeconds(tmCurrentTime);
//			o_pOutPostWarInfo[OutPostWarInfoCount].OutPostWarResetStartTime = *((*itr)->GetOutPostResetStartTime());
//			o_pOutPostWarInfo[OutPostWarInfoCount].OutPostWarResetEndTime = *((*itr)->GetOutPostResetEndTime());
			OutPostWarInfoCount++;
		}
	}
	
	return OutPostWarInfoCount;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			void COutPostManager::OutPostPossessNPCByDeleteGuild(UID32_t i_nGuildUID)
/// \brief		여단 삭제 요청으로 전진기지 소유 하고 있던 여단이었을 경우 NPC소유로 변경
/// \author		dhjin
/// \date		2007-11-09 ~ 2007-11-09
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void COutPostManager::OutPostPossessNPCByDeleteGuild(UID32_t i_nGuildUID)
{
	mt_auto_lock mta(&m_mtvectOutPostManager);

	mtvectOutPost::iterator	itr = m_mtvectOutPostManager.begin();
	for(; itr != m_mtvectOutPostManager.end(); itr++)
	{
		if(i_nGuildUID == (*itr)->GetOutPostGuildUID())
		{
			// 2007-08-24 by dhjin, 전진기지 NPC소유 처리
			(*itr)->SetOutPostPossessNPCByReset();
			QPARAM_SET_NPCPOSSESSION_OUTPOST *pQParamNPC = new QPARAM_SET_NPCPOSSESSION_OUTPOST;
			pQParamNPC->MapIndex			= (*itr)->GetOutPostMapIndex();
			pQParamNPC->GetTime			= *((*itr)->GetPotPostGetTime());
			pQParamNPC->NextWarTime		= *((*itr)->GetOutPostNextWarTime());
			m_pFieldIOCP18->m_pAtumDBManager->MakeAndEnqueueQuery(QT_SetNPCPossessionOutPost, NULL, NULL, pQParamNPC);
			
			
			INT CityMapIndex = (*itr)->GetOutPostCityMapIndex();

			SCITYLEADER_INFO	tmCityLeaderInfo;
			util::zero(&tmCityLeaderInfo, sizeof(SCITYLEADER_INFO));
			m_pFieldIOCP18->m_CityLeader.GetExpediencyfund(CityMapIndex, &tmCityLeaderInfo);
			BYTE	tmInfluence = tmCityLeaderInfo.Influence;
			if(INFLUENCE_TYPE_VCN == tmCityLeaderInfo.Influence)
			{// 2007-08-27 by dhjin, NPC소유 전에 바이제니유 여단이 소유하고 있었다면 바이제니유 지도자 혜택을 줄인다.
				this->UpdateExpediencyfundRateLeader(VCN_CITY_MAP_INDEX, INFLUENCE_TYPE_VCN, -EXPEDIENCYFUND_LEADER_OUTPOST);
			}
			else if(INFLUENCE_TYPE_ANI == tmCityLeaderInfo.Influence)
			{// 2007-08-27 by dhjin, NPC소유 전에 알링턴 여단이 소유하고 있었다면 알링턴 지도자 혜택을 줄인다.
				this->UpdateExpediencyfundRateLeader(ANI_CITY_MAP_INDEX, INFLUENCE_TYPE_ANI, -EXPEDIENCYFUND_LEADER_OUTPOST);
			}

			// 2. 전진기지 판공비 초기화
			m_pFieldIOCP18->m_CityLeader.InitExpediencyfund(CityMapIndex, 0, 0, EXPEDIENCYFUND_GUILDCOMMANDER);
			QPARAM_INIT_EXPEDIENCY_FUND *pQParam = new QPARAM_INIT_EXPEDIENCY_FUND;
			pQParam->MapIndex					= CityMapIndex;
			pQParam->Influence					= 0;
			pQParam->CharacterUID				= 0;
			pQParam->ExpediencyFundCumulative	= 0;
			pQParam->ExpediencyFund				= 0;
			pQParam->ExpediencyRate				= EXPEDIENCYFUND_GUILDCOMMANDER;
			m_pFieldIOCP18->m_pAtumDBManager->MakeAndEnqueueQuery(QT_InitExpediencyFund, NULL, NULL, pQParam);	

			return;
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			INT	COutPostManager::MakeAllOutPostInfoMsg(SCITY_WARINFO_OUTPOST *o_pOutPostWarInfo)
/// \brief		전진기지 모든 정보 리스트 만들기
/// \author		dhjin
/// \date		2007-12-03 ~ 2007-12-03
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
INT	COutPostManager::MakeAllOutPostInfoMsg(SCITY_WARINFO_OUTPOST *o_pOutPostWarInfo)
{
	mt_auto_lock mta(&m_mtvectOutPostManager);
	
	int	OutPostWarInfoCount = 0;

	mtvectOutPost::iterator itr = m_mtvectOutPostManager.begin();
	for(; itr != m_mtvectOutPostManager.end(); itr++)
	{
		o_pOutPostWarInfo[OutPostWarInfoCount].Influence	= (*itr)->GetOutPostInfluence();
		o_pOutPostWarInfo[OutPostWarInfoCount].MapIndex		= (*itr)->GetOutPostCityMapIndex();
		o_pOutPostWarInfo[OutPostWarInfoCount].GuildUID		= (*itr)->GetOutPostGuildUID();
		m_pFieldIOCP18->m_GuildWarManager.GetGuildCommanderName(o_pOutPostWarInfo[OutPostWarInfoCount].GuildUID, o_pOutPostWarInfo[OutPostWarInfoCount].GuildCommander);
		util::strncpy(o_pOutPostWarInfo[OutPostWarInfoCount].GuildName, (*itr)->GetOutPostGuildName(), SIZE_MAX_GUILD_NAME);
		o_pOutPostWarInfo[OutPostWarInfoCount].OutpostDate	= *((*itr)->GetOutPostNextWarTime());
		OutPostWarInfoCount++;
	}
	
	return OutPostWarInfoCount;
}

