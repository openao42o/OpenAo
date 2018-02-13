// HappyHourEventManager.cpp: implementation of the CHappyHourEventManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HappyHourEventManager.h"
#include "FieldIOCP.h"
#include "GameEventManager.h"
#include "FieldGlobal.h"				// 2007-10-30 by cmkwon, 세력별 해피아워 이벤트 구현 - 

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CHappyHourEventManager::CHappyHourEventManager(CFieldIOCP *i_pFIOCP, CGameEventManager *i_pGameEventManager)
{
	m_pFieldIOCP13			= i_pFIOCP;
	m_pGameEventManager		= i_pGameEventManager;
// 2007-10-30 by cmkwon, 세력별 해피아워 이벤트 구현 - 주석 처리함
//	m_nAllEventUniqueNumber	= 0;
	util::zero(m_arrPeriodEachInfluenceType, sizeof(m_arrPeriodEachInfluenceType[0]) * INFLUENCE_TYPE_COUNT);	// 2007-10-30 by cmkwon, 세력별 해피아워 이벤트 구현 - 초기화
	util::zero(m_pArrCurHappyHourEventEachInfluenceType, sizeof(m_pArrCurHappyHourEventEachInfluenceType[0]) * INFLUENCE_TYPE_COUNT);	// 2007-10-30 by cmkwon, 세력별 해피아워 이벤트 구현 - 초기화


	// 2007-11-01 by cmkwon, 세력별 HappyHourEvent 기간 초기화
	m_arrPeriodEachInfluenceType[0].InitSHAPPY_HOUR_EVENT_PERIOD(INFLUENCE_TYPE_NORMAL);
	m_arrPeriodEachInfluenceType[1].InitSHAPPY_HOUR_EVENT_PERIOD(INFLUENCE_TYPE_VCN);
	m_arrPeriodEachInfluenceType[2].InitSHAPPY_HOUR_EVENT_PERIOD(INFLUENCE_TYPE_ANI);
	m_arrPeriodEachInfluenceType[3].InitSHAPPY_HOUR_EVENT_PERIOD(INFLUENCE_TYPE_ALL_MASK);

	this->ResetHappyHourEventManager();
}
CHappyHourEventManager::~CHappyHourEventManager()
{
}

BOOL CHappyHourEventManager::InitHappyHourEventManager(mtvectHAPPY_HOUR_EVENT *i_pvectHappyEventList)
{
	mt_auto_lock mtA(this->GetVectHappyHourEventListPtr());				// 2006-08-24 by cmkwon

	if(i_pvectHappyEventList->empty())
	{
		return FALSE;
	}

	m_mtvectHappyHourEventList.clear();
	ATUM_DATE_TIME atimeCur { true };
	for(int i=0; i < i_pvectHappyEventList->size(); i++)
	{
		SHAPPY_HOUR_EVENT *pHappyEvent = &(*i_pvectHappyEventList)[i];
		switch(pHappyEvent->DayOfWeek)
		{
		case DAY_OF_WEEK_SUMDAY:
		case DAY_OF_WEEK_MONDAY:
		case DAY_OF_WEEK_TUESDAY:
		case DAY_OF_WEEK_WEDNESDAY:
		case DAY_OF_WEEK_THURSDAY:	
		case DAY_OF_WEEK_FRIDAY:
		case DAY_OF_WEEK_SATURDAY:
			{
				pHappyEvent->atimeStartTime2.Year	= pHappyEvent->atimeEndTime2.Year	= atimeCur.Year;
				pHappyEvent->atimeStartTime2.Month	= pHappyEvent->atimeEndTime2.Month	= atimeCur.Month;
				pHappyEvent->atimeStartTime2.Day	= pHappyEvent->atimeEndTime2.Day	= atimeCur.Day;

				if(pHappyEvent->atimeStartTime2 >= pHappyEvent->atimeEndTime2)
				{// 요일 이벤트 시간 설정 오류
					g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ERROR] HappyHourEvent time error !!, HappyHourEventUniqueNumber(%u) InfluenceType(%d) %s ServerGroupID(%d) StartTime(%s) EndTime(%s)\r\n"
						, pHappyEvent->EventUniqueNumber, pHappyEvent->InfluenceType3, GetDayOfWeekString(pHappyEvent->DayOfWeek)
						, pHappyEvent->atimeStartTime2.GetDateTimeString().GetBuffer()
						, pHappyEvent->atimeEndTime2.GetDateTimeString().GetBuffer());
				}
				else
				{
					m_mtvectHappyHourEventList.push_back(*pHappyEvent);
				}
			}
			break;
		case DAY_OF_WEEK_HAPPYHOUREVENT_PERIOD:
			{				
				this->SetHappyHourEventPeriod(pHappyEvent->InfluenceType3, pHappyEvent->atimeStartTime2, pHappyEvent->atimeEndTime2);
			}
			break;
		case DAY_OF_WEEK_PCBANG_HAPPYHOUREVENT:
			{
				m_byPCBangHappyEvState				= PCBANG_HAPPYEV_STATE_TYPE_END;
				m_PCBangHappyHourEvent				= *pHappyEvent;
			}
			break;
		default:			
			{// 2007-10-30 by cmkwon, DayOfWeek 필드값이 유효하지 않음
				g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ERROR] HappyHourEvent day of week error !!, HappyHourEventUniqueNumber(%u) InfluenceType(%d) %s ServerGroupID(%d) StartTime(%s) EndTime(%s)\r\n"
					, pHappyEvent->EventUniqueNumber, pHappyEvent->InfluenceType3, GetDayOfWeekString(pHappyEvent->DayOfWeek)
					, pHappyEvent->atimeStartTime2.GetDateTimeString().GetBuffer()
					, pHappyEvent->atimeEndTime2.GetDateTimeString().GetBuffer());
			}
		}
	}// END - for(int i=0; i < i_pvectHappyEventList->size(); i++)

	return TRUE;
}

// 2007-10-30 by cmkwon, 세력별 해피아워 이벤트 구현 - 인자 추가(BOOL i_bNotify=FALSE)
void CHappyHourEventManager::ResetHappyHourEventManager(BOOL i_bNotify/*=FALSE*/)
{
	mt_auto_lock mtA(this->GetVectHappyHourEventListPtr());				// 2006-08-24 by cmkwon

	if(i_bNotify)
	{
		INIT_MSG_WITH_BUFFER(MSG_FC_INFO_GET_HAPPY_HOUR_EVENT_INFO_OK, T_FC_INFO_GET_HAPPY_HOUR_EVENT_INFO_OK, pSHappyEvEnd, SendBuf);
		int i=0;
		for(i=0; i < INFLUENCE_TYPE_COUNT; i++)
		{
			if(NULL == m_pArrCurHappyHourEventEachInfluenceType[i])
			{
				continue;
			}

			SHAPPY_HOUR_EVENT *pHappyEv		= m_pArrCurHappyHourEventEachInfluenceType[i];

			pSHappyEvEnd->byStateType		= HAPPYEV_STATE_TYPE_END;
			pSHappyEvEnd->byInfluenceType4	= pHappyEv->InfluenceType3;
			pSHappyEvEnd->StartATime		= pHappyEv->atimeStartTime2;
			pSHappyEvEnd->EndATime			= pHappyEv->atimeEndTime2;
			pSHappyEvEnd->fEXPRate2			= pHappyEv->fEXPRate2;
			pSHappyEvEnd->fSPIRate2			= pHappyEv->fSPIRate2;
			pSHappyEvEnd->fEXPRepairRate2	= pHappyEv->fEXPRepairRate2;
			pSHappyEvEnd->fDropItemRate2	= pHappyEv->fDropItemRate2;
			pSHappyEvEnd->fDropRareRate2	= pHappyEv->fDropRareItemRate2;
			pSHappyEvEnd->fWarPointRate2	= pHappyEv->fWarPointRate2;
			m_pFieldIOCP13->SendMessageToAllClients(SendBuf, MSG_SIZE(MSG_FC_INFO_GET_HAPPY_HOUR_EVENT_INFO_OK), pHappyEv->InfluenceType3, pHappyEv->MinLevel, pHappyEv->MaxLevel);		// 2008-03-13 by dhjin, Level별 해피아워 이벤트 구현 - 
		}

		if(PCBANG_HAPPYEV_STATE_TYPE_END != m_byPCBangHappyEvState)
		{
			SHAPPY_HOUR_EVENT *pHappyEv		= &m_PCBangHappyHourEvent;

			pSHappyEvEnd->byStateType		= PCBANG_HAPPYEV_STATE_TYPE_END;
			pSHappyEvEnd->byInfluenceType4	= pHappyEv->InfluenceType3;
			pSHappyEvEnd->StartATime		= pHappyEv->atimeStartTime2;
			pSHappyEvEnd->EndATime			= pHappyEv->atimeEndTime2;
			pSHappyEvEnd->fEXPRate2			= pHappyEv->fEXPRate2;
			pSHappyEvEnd->fSPIRate2			= pHappyEv->fSPIRate2;
			pSHappyEvEnd->fEXPRepairRate2	= pHappyEv->fEXPRepairRate2;
			pSHappyEvEnd->fDropItemRate2	= pHappyEv->fDropItemRate2;
			pSHappyEvEnd->fDropRareRate2	= pHappyEv->fDropRareItemRate2;
			pSHappyEvEnd->fWarPointRate2	= pHappyEv->fWarPointRate2;
			m_pFieldIOCP13->SendMessageToAllClients(SendBuf, MSG_SIZE(MSG_FC_INFO_GET_HAPPY_HOUR_EVENT_INFO_OK), INFLUENCE_TYPE_ALL_MASK, pHappyEv->MinLevel, pHappyEv->MaxLevel, NULL, TRUE);		// 2008-03-13 by dhjin, Level별 해피아워 이벤트 구현 - 
		}
	}

// 2007-10-30 by cmkwon, 세력별 해피아워 이벤트 구현 - 변수 변경 함
//	m_pCurrentHappyHourEvent	= NULL;
//	m_atimeStartTime.Reset();
//	m_atimeEndTime.Reset();
	// 2007-11-01 by cmkwon, 세력별 HappyHourEvent 기간 초기화

	m_arrPeriodEachInfluenceType[0].InitSHAPPY_HOUR_EVENT_PERIOD(INFLUENCE_TYPE_NORMAL);
	m_arrPeriodEachInfluenceType[1].InitSHAPPY_HOUR_EVENT_PERIOD(INFLUENCE_TYPE_VCN);
	m_arrPeriodEachInfluenceType[2].InitSHAPPY_HOUR_EVENT_PERIOD(INFLUENCE_TYPE_ANI);
	m_arrPeriodEachInfluenceType[3].InitSHAPPY_HOUR_EVENT_PERIOD(INFLUENCE_TYPE_ALL_MASK);
	util::zero(m_pArrCurHappyHourEventEachInfluenceType, sizeof(m_pArrCurHappyHourEventEachInfluenceType[0]) * INFLUENCE_TYPE_COUNT);	// 2007-10-30 by cmkwon, 세력별 해피아워 이벤트 구현 - 초기화

	m_mtvectHappyHourEventList.clear();
	m_byPCBangHappyEvState		= PCBANG_HAPPYEV_STATE_TYPE_END;
	util::zero(&m_PCBangHappyHourEvent, sizeof(m_PCBangHappyHourEvent));		// 2006-08-23 by cmkwon
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			mtvectHAPPY_HOUR_EVENT *CHappyHourEventManager::GetVectHappyHourEventListPtr(void)
/// \brief		
/// \author		cmkwon
/// \date		2006-08-22 ~ 2006-08-22
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
mtvectHAPPY_HOUR_EVENT *CHappyHourEventManager::GetVectHappyHourEventListPtr(void)
{
	return &m_mtvectHappyHourEventList;
}

// 2007-10-30 by cmkwon, 세력별 해피아워 이벤트 구현 - 함수 주석 처리함
//void CHappyHourEventManager::SetHappyHourEventTime(ATUM_DATE_TIME i_atimeStart
//												   , ATUM_DATE_TIME i_atimeEnd
//												   , BOOL i_bStoreDB/*=FALSE*/)
//{
//	m_atimeStartTime	= i_atimeStart;
//	m_atimeEndTime		= i_atimeEnd;
//
//	if(i_bStoreDB)
//	{
//		QPARAM_UPDATE_HAPPY_HOUR_EVENT_DATE *pQParam = new QPARAM_UPDATE_HAPPY_HOUR_EVENT_DATE;
//		pQParam->atimeStartTime2	= m_atimeStartTime;
//		pQParam->atimeEndTime2		= m_atimeEndTime;
//		m_pFieldIOCP13->m_pAtumDBManager->MakeAndEnqueueQuery(QT_UpdateHappyHourEventDate, NULL, 0, pQParam);
//	}
//}
//
//SHAPPY_HOUR_EVENT *CHappyHourEventManager::GetCurrentHappyHourEvent(void)
//{
//	return m_pCurrentHappyHourEvent;
//}

// 2007-10-30 by cmkwon, 세력별 해피아워 이벤트 구현 - 아래와 같이 함수를 수정함
//SHAPPY_HOUR_EVENT *CHappyHourEventManager::FindHappyHourEventByTime(ATUM_DATE_TIME *pCurDateTime, int i_CurDayOfWeek)
//{
//	for(int i=0; i < m_mtvectHappyHourEventList.size(); i++)
//	{
//		SHAPPY_HOUR_EVENT *pHappyEv = &m_mtvectHappyHourEventList[i];
//		if(pHappyEv->DayOfWeek == i_CurDayOfWeek)
//		{
//			pHappyEv->atimeStartTime2.Year	= pHappyEv->atimeEndTime2.Year	= pCurDateTime->Year;
//			pHappyEv->atimeStartTime2.Month	= pHappyEv->atimeEndTime2.Month	= pCurDateTime->Month;
//			pHappyEv->atimeStartTime2.Day	= pHappyEv->atimeEndTime2.Day	= pCurDateTime->Day;
//
//			if(pHappyEv->atimeStartTime2 <= *pCurDateTime
//				&& pHappyEv->atimeEndTime2 > *pCurDateTime)
//			{
//				return pHappyEv;
//			}
//		}
//	}
//
//	return NULL;
//}
//
/////////////////////////////////////////////////////////////////////////////////
///// \fn			BOOL CHappyHourEventManager::OnDoMinutelyWorkHappyHourEventManager(ATUM_DATE_TIME *pDateTime)
///// \brief		
///// \author		cmkwon
///// \date		
///// \warning	
/////
///// \param		
///// \return		FALSE	: GameEvent Time 체크를 진행한다.
/////				TRUE	: GameEvent Time 체크를 진행하지 않는다
/////////////////////////////////////////////////////////////////////////////////
//BOOL CHappyHourEventManager::OnDoMinutelyWorkHappyHourEventManager(ATUM_DATE_TIME *pDateTime)
//{
//	mt_auto_lock mtHappy(this->GetVectHappyHourEventListPtr());	// 2006-08-23 by cmkwon
//
//	///////////////////////////////////////////////////////////////////////////////
//	// 2006-08-23 by cmkwon, check PCBang event
//	BOOL bSendPCBangMessage = FALSE;
//	INIT_MSG_WITH_BUFFER(MSG_FC_INFO_GET_HAPPY_HOUR_EVENT_INFO_OK, T_FC_INFO_GET_HAPPY_HOUR_EVENT_INFO_OK, pSPCBang, PCBangSendBuf);
//	if(m_PCBangHappyHourEvent.atimeStartTime2 >= *pDateTime
//		|| m_PCBangHappyHourEvent.atimeEndTime2 <= *pDateTime)
//	{// 2006-08-23 by cmkwon, not PCBang event time.
//
//		if(PCBANG_HAPPYEV_STATE_TYPE_END != m_byPCBangHappyEvState)
//		{
//			bSendPCBangMessage			= TRUE;
//			m_byPCBangHappyEvState		= PCBANG_HAPPYEV_STATE_TYPE_END;
//			pSPCBang->byStateType		= m_byPCBangHappyEvState;
//			pSPCBang->StartATime		= m_PCBangHappyHourEvent.atimeStartTime2;
//			pSPCBang->EndATime			= m_PCBangHappyHourEvent.atimeEndTime2;
//			pSPCBang->fEXPRate2			= m_PCBangHappyHourEvent.fEXPRate2;
//			pSPCBang->fSPIRate2			= m_PCBangHappyHourEvent.fSPIRate2;
//			pSPCBang->fEXPRepairRate2	= m_PCBangHappyHourEvent.fEXPRepairRate2;
//			pSPCBang->fDropItemRate2	= m_PCBangHappyHourEvent.fDropItemRate2;
//			pSPCBang->fDropRareRate2	= m_PCBangHappyHourEvent.fDropRareItemRate2;
//			pSPCBang->fWarPointRate2	= m_PCBangHappyHourEvent.fWarPointRate2;			// 2007-06-26 by dhjin, 워포인트 이벤트 관련 추가
//			m_pGameEventManager->EndPCBangGameEvent();
//		}
//	}
//	else
//	{
//		if(PCBANG_HAPPYEV_STATE_TYPE_END == m_byPCBangHappyEvState)
//		{
//			int nEventTimeMinute	= m_PCBangHappyHourEvent.atimeEndTime2.GetTimeDiffTimeInMinutes(m_PCBangHappyHourEvent.atimeStartTime2);
//			if(nEventTimeMinute > 0)
//			{
//				bSendPCBangMessage			= TRUE;
//				m_byPCBangHappyEvState		= PCBANG_HAPPYEV_STATE_TYPE_START;
//				pSPCBang->byStateType		= m_byPCBangHappyEvState;
//				pSPCBang->StartATime		= m_PCBangHappyHourEvent.atimeStartTime2;
//				pSPCBang->EndATime			= m_PCBangHappyHourEvent.atimeEndTime2;
//				pSPCBang->fEXPRate2			= m_PCBangHappyHourEvent.fEXPRate2;
//				pSPCBang->fSPIRate2			= m_PCBangHappyHourEvent.fSPIRate2;
//				pSPCBang->fEXPRepairRate2	= m_PCBangHappyHourEvent.fEXPRepairRate2;
//				pSPCBang->fDropItemRate2	= m_PCBangHappyHourEvent.fDropItemRate2;
//				pSPCBang->fDropRareRate2	= m_PCBangHappyHourEvent.fDropRareItemRate2;
//				pSPCBang->fWarPointRate2	= m_PCBangHappyHourEvent.fWarPointRate2;			// 2007-06-26 by dhjin, 워포인트 이벤트 관련 추가
//				
//				m_pGameEventManager->StartPCBangGameEvent(GAME_EVENT_TYPE_EXP, m_PCBangHappyHourEvent.fEXPRate2, nEventTimeMinute);
//				m_pGameEventManager->StartPCBangGameEvent(GAME_EVENT_TYPE_SPI, m_PCBangHappyHourEvent.fSPIRate2, nEventTimeMinute);
//				m_pGameEventManager->StartPCBangGameEvent(GAME_EVENT_TYPE_EXP_NO_DOWN, m_PCBangHappyHourEvent.fEXPRepairRate2, nEventTimeMinute);
//				m_pGameEventManager->StartPCBangGameEvent(GAME_EVENT_TYPE_DROP_ITEM, m_PCBangHappyHourEvent.fDropItemRate2, nEventTimeMinute);
//				m_pGameEventManager->StartPCBangGameEvent(GAME_EVENT_TYPE_DROP_REAR_ITEM, m_PCBangHappyHourEvent.fDropRareItemRate2, nEventTimeMinute);
//				m_pGameEventManager->StartPCBangGameEvent(GAME_EVENT_TYPE_WARPOINT, m_PCBangHappyHourEvent.fWarPointRate2, nEventTimeMinute);			// 2007-06-26 by dhjin, 워포인트 이벤트 관련 추가	
//			}
//		}
//	}
//
//	SHAPPY_HOUR_EVENT *pHappyEv = GetCurrentHappyHourEvent();
//
//	if(m_atimeStartTime >= *pDateTime
//		|| m_atimeEndTime <= *pDateTime
//		|| m_mtvectHappyHourEventList.empty())
//	{// 2006-04-12 by cmkwon, HappyHourEvent의 기간이 종료 되었거나 세부이벤트 설정이 없음
//
//		if(pHappyEv)
//		{// 이벤트 종료
//			SHAPPY_HOUR_EVENT tmHappyEv = *pHappyEv;				// 2006-08-23 by cmkwon, 임시 변수
//			mtHappy.auto_unlock_cancel();							// 2006-08-23 by cmkwon
//
//			m_pGameEventManager->EndAllGameEvent();
//			m_pCurrentHappyHourEvent		= NULL;
//
//			INIT_MSG_WITH_BUFFER(MSG_FC_INFO_GET_HAPPY_HOUR_EVENT_INFO_OK, T_FC_INFO_GET_HAPPY_HOUR_EVENT_INFO_OK, pSHappy, SendBuf);
//			pSHappy->byStateType			= HAPPYEV_STATE_TYPE_END;
//			pSHappy->StartATime				= tmHappyEv.atimeStartTime2;
//			pSHappy->EndATime				= tmHappyEv.atimeEndTime2;
//			pSHappy->fEXPRate2				= tmHappyEv.fEXPRate2;
//			pSHappy->fSPIRate2				= tmHappyEv.fSPIRate2;
//			pSHappy->fEXPRepairRate2		= tmHappyEv.fEXPRepairRate2;
//			pSHappy->fDropItemRate2			= tmHappyEv.fDropItemRate2;
//			pSHappy->fDropRareRate2			= tmHappyEv.fDropRareItemRate2;
//			pSHappy->fWarPointRate2			= tmHappyEv.fWarPointRate2;					// 2007-06-26 by dhjin, 워포인트 이벤트 관련 추가
//			m_pFieldIOCP13->SendMessageToAllClients(SendBuf, MSG_SIZE(MSG_FC_INFO_GET_HAPPY_HOUR_EVENT_INFO_OK));
//		}
//		mtHappy.auto_unlock_cancel();							// 2006-08-23 by cmkwon
//		if(bSendPCBangMessage)
//		{// 2006-08-23 by cmkwon, PCBang 이벤트 관련 정보 전송
//			m_pFieldIOCP13->SendMessageToAllClients(PCBangSendBuf, MSG_SIZE(MSG_FC_INFO_GET_HAPPY_HOUR_EVENT_INFO_OK), INFLUENCE_TYPE_ALL_MASK, 0, 0, NULL, TRUE);
//		}
//		return FALSE;
//	}
//
//	struct tm localTime;	
//	pDateTime->Convert(localTime);
//	if(pHappyEv)
//	{
//		pHappyEv->atimeStartTime2.Year	= pHappyEv->atimeEndTime2.Year	= pDateTime->Year;
//		pHappyEv->atimeStartTime2.Month	= pHappyEv->atimeEndTime2.Month	= pDateTime->Month;
//		pHappyEv->atimeStartTime2.Day	= pHappyEv->atimeEndTime2.Day	= pDateTime->Day;
//
//		if(pHappyEv->DayOfWeek != localTime.tm_wday
//			|| pHappyEv->atimeStartTime2 > *pDateTime
//			|| pHappyEv->atimeEndTime2 <= *pDateTime)
//		{// 세부 이벤트 종료
//			SHAPPY_HOUR_EVENT tmHappyEv = *pHappyEv;				// 2006-08-23 by cmkwon, 임시 변수
//			mtHappy.auto_unlock_cancel();							// 2006-08-23 by cmkwon
//
//			m_pGameEventManager->EndAllGameEvent();
//			m_pCurrentHappyHourEvent		= NULL;
//
//			INIT_MSG_WITH_BUFFER(MSG_FC_INFO_GET_HAPPY_HOUR_EVENT_INFO_OK, T_FC_INFO_GET_HAPPY_HOUR_EVENT_INFO_OK, pSHappy, SendBuf);
//			pSHappy->byStateType			= HAPPYEV_STATE_TYPE_END;
//			pSHappy->StartATime				= tmHappyEv.atimeStartTime2;
//			pSHappy->EndATime				= tmHappyEv.atimeEndTime2;
//			pSHappy->fEXPRate2				= tmHappyEv.fEXPRate2;
//			pSHappy->fSPIRate2				= tmHappyEv.fSPIRate2;
//			pSHappy->fEXPRepairRate2		= tmHappyEv.fEXPRepairRate2;
//			pSHappy->fDropItemRate2			= tmHappyEv.fDropItemRate2;
//			pSHappy->fDropRareRate2			= tmHappyEv.fDropRareItemRate2;
//			pSHappy->fWarPointRate2			= tmHappyEv.fWarPointRate2;					// 2007-06-26 by dhjin, 워포인트 이벤트 관련 추가
//			m_pFieldIOCP13->SendMessageToAllClients(SendBuf, MSG_SIZE(MSG_FC_INFO_GET_HAPPY_HOUR_EVENT_INFO_OK));
//		}
//		mtHappy.auto_unlock_cancel();							// 2006-08-23 by cmkwon
//		if(bSendPCBangMessage)
//		{// 2006-08-23 by cmkwon, PCBang 이벤트 관련 정보 전송
//			m_pFieldIOCP13->SendMessageToAllClients(PCBangSendBuf, MSG_SIZE(MSG_FC_INFO_GET_HAPPY_HOUR_EVENT_INFO_OK), INFLUENCE_TYPE_ALL_MASK, 0, 0, NULL, TRUE);
//		}
//		return TRUE;
//	}
//
//	pHappyEv = FindHappyHourEventByTime(pDateTime, localTime.tm_wday);
//	if(NULL == pHappyEv)
//	{
//		mtHappy.auto_unlock_cancel();							// 2006-08-23 by cmkwon
//		if(bSendPCBangMessage)
//		{// 2006-08-23 by cmkwon, PCBang 이벤트 관련 정보 전송
//			m_pFieldIOCP13->SendMessageToAllClients(PCBangSendBuf, MSG_SIZE(MSG_FC_INFO_GET_HAPPY_HOUR_EVENT_INFO_OK), INFLUENCE_TYPE_ALL_MASK, 0, 0, NULL, TRUE);
//		}
//		return FALSE;
//	}
//
//	int nEventTimeMinute	= pHappyEv->atimeEndTime2.GetTimeDiffTimeInMinutes(pHappyEv->atimeStartTime2);
//	if(nEventTimeMinute <= 0)
//	{// 이벤트 진행 시간 오류
//		mtHappy.auto_unlock_cancel();							// 2006-08-23 by cmkwon
//		if(bSendPCBangMessage)
//		{// 2006-08-23 by cmkwon, PCBang 이벤트 관련 정보 전송
//			m_pFieldIOCP13->SendMessageToAllClients(PCBangSendBuf, MSG_SIZE(MSG_FC_INFO_GET_HAPPY_HOUR_EVENT_INFO_OK), INFLUENCE_TYPE_ALL_MASK, 0, 0, NULL, TRUE);
//		}
//		return FALSE;
//	}
//	m_pCurrentHappyHourEvent		= pHappyEv;
//	SHAPPY_HOUR_EVENT tmHappyEv		= *pHappyEv;				// 2006-08-23 by cmkwon, 임시 변수
//	mtHappy.auto_unlock_cancel();								// 2006-08-23 by cmkwon
//	if(bSendPCBangMessage)
//	{// 2006-08-23 by cmkwon, PCBang 이벤트 관련 정보 전송
//		m_pFieldIOCP13->SendMessageToAllClients(PCBangSendBuf, MSG_SIZE(MSG_FC_INFO_GET_HAPPY_HOUR_EVENT_INFO_OK), INFLUENCE_TYPE_ALL_MASK, 0, 0, NULL, TRUE);
//	}
//	
//	INIT_MSG_WITH_BUFFER(MSG_FC_INFO_GET_HAPPY_HOUR_EVENT_INFO_OK, T_FC_INFO_GET_HAPPY_HOUR_EVENT_INFO_OK, pSHappy, SendBuf);
//	pSHappy->byStateType			= HAPPYEV_STATE_TYPE_START;
//	pSHappy->StartATime				= tmHappyEv.atimeStartTime2;
//	pSHappy->EndATime				= tmHappyEv.atimeEndTime2;
//	pSHappy->fEXPRate2				= tmHappyEv.fEXPRate2;
//	pSHappy->fSPIRate2				= tmHappyEv.fSPIRate2;
//	pSHappy->fEXPRepairRate2		= tmHappyEv.fEXPRepairRate2;
//	pSHappy->fDropItemRate2			= tmHappyEv.fDropItemRate2;
//	pSHappy->fDropRareRate2			= tmHappyEv.fDropRareItemRate2;
//	pSHappy->fWarPointRate2			= tmHappyEv.fWarPointRate2;					// 2007-06-26 by dhjin, 워포인트 이벤트 관련 추가
//	m_pFieldIOCP13->SendMessageToAllClients(SendBuf, MSG_SIZE(MSG_FC_INFO_GET_HAPPY_HOUR_EVENT_INFO_OK));
//
//	m_pGameEventManager->StartGameEvent(GAME_EVENT_TYPE_EXP, tmHappyEv.fEXPRate2, nEventTimeMinute);
//	m_pGameEventManager->StartGameEvent(GAME_EVENT_TYPE_SPI, tmHappyEv.fSPIRate2, nEventTimeMinute);
//	m_pGameEventManager->StartGameEvent(GAME_EVENT_TYPE_EXP_NO_DOWN, tmHappyEv.fEXPRepairRate2, nEventTimeMinute);
//	m_pGameEventManager->StartGameEvent(GAME_EVENT_TYPE_DROP_ITEM, tmHappyEv.fDropItemRate2, nEventTimeMinute);
//	m_pGameEventManager->StartGameEvent(GAME_EVENT_TYPE_DROP_REAR_ITEM, tmHappyEv.fDropRareItemRate2, nEventTimeMinute);
//	m_pGameEventManager->StartGameEvent(GAME_EVENT_TYPE_WARPOINT, tmHappyEv.fWarPointRate2, nEventTimeMinute);			// 2007-06-26 by dhjin, 워포인트 이벤트 관련 추가
//	
//	return TRUE;
//}
//
/////////////////////////////////////////////////////////////////////////////////
///// \fn			BOOL CHappyHourEventManager::GetPCBangHappyHourEventInfo(MSG_FC_INFO_GET_HAPPY_HOUR_EVENT_INFO_OK *o_pHappyInfoOK)
///// \brief		
///// \author		cmkwon
///// \date		2006-08-23 ~ 2006-08-23
///// \warning	
/////
///// \param		
///// \return		
/////////////////////////////////////////////////////////////////////////////////
//BOOL CHappyHourEventManager::GetPCBangHappyHourEventInfo(MSG_FC_INFO_GET_HAPPY_HOUR_EVENT_INFO_OK *o_pHappyInfoOK)
//{
//	if(PCBANG_HAPPYEV_STATE_TYPE_END == m_byPCBangHappyEvState)
//	{
//		return FALSE;
//	}
//
//	o_pHappyInfoOK->byStateType		= PCBANG_HAPPYEV_STATE_TYPE_STARTING;
//	o_pHappyInfoOK->StartATime		= m_PCBangHappyHourEvent.atimeStartTime2;
//	o_pHappyInfoOK->EndATime		= m_PCBangHappyHourEvent.atimeEndTime2;
//	o_pHappyInfoOK->fEXPRate2		= m_PCBangHappyHourEvent.fEXPRate2;
//	o_pHappyInfoOK->fSPIRate2		= m_PCBangHappyHourEvent.fSPIRate2;
//	o_pHappyInfoOK->fEXPRepairRate2	= m_PCBangHappyHourEvent.fEXPRepairRate2;
//	o_pHappyInfoOK->fDropItemRate2	= m_PCBangHappyHourEvent.fDropItemRate2;
//	o_pHappyInfoOK->fDropRareRate2	= m_PCBangHappyHourEvent.fDropRareItemRate2;
//	o_pHappyInfoOK->fWarPointRate2	= m_PCBangHappyHourEvent.fWarPointRate2;
//
//	return TRUE;
//}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CHappyHourEventManager::OnDoMinutelyWorkHappyHourEventManager(ATUM_DATE_TIME *pDateTime)
/// \brief		// 2007-10-30 by cmkwon, 세력별 해피아워 이벤트 구현 - 함수를 수정함
/// \author		cmkwon
/// \date		2007-10-31 ~ 2007-10-31
/// \warning	
///
/// \param		
/// \return		FALSE	: GameEvent Time 체크를 진행한다.
///				TRUE	: GameEvent Time 체크를 진행하지 않는다
///////////////////////////////////////////////////////////////////////////////
BOOL CHappyHourEventManager::OnDoMinutelyWorkHappyHourEventManager(ATUM_DATE_TIME *pDateTime)
{
	mt_auto_lock mtHappy(this->GetVectHappyHourEventListPtr());	// 2006-08-23 by cmkwon

	///////////////////////////////////////////////////////////////////////////////
	// 2007-10-30 by cmkwon, 세력별 해피아워 이벤트 구현 - PCBang HappyHourEvent 처리
	this->OnDoMinutelyWorkHappyHourEventManagerOfPCBang(pDateTime);

	///////////////////////////////////////////////////////////////////////////////
	// 2007-10-30 by cmkwon, 세력별 해피아워 이벤트 구현 - 모든 세력별 HappyHourEvent 처리
	this->OnDoMinutelyWorkHappyHourEventManagerAllInfluenceType(pDateTime);
	return TRUE;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			void CHappyHourEventManager::OnDoMinutelyWorkHappyHourEventManagerOfPCBang(ATUM_DATE_TIME *pDateTime)
/// \brief		// 2007-10-30 by cmkwon, 세력별 해피아워 이벤트 구현 - CHappyHourEventManager::OnDoMinutelyWorkHappyHourEventManagerOfPCBang() 함수 추가
/// \author		cmkwon
/// \date		2007-10-31 ~ 2007-10-31
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CHappyHourEventManager::OnDoMinutelyWorkHappyHourEventManagerOfPCBang(ATUM_DATE_TIME *pDateTime)
{
	if(m_PCBangHappyHourEvent.atimeStartTime2 >= *pDateTime
		|| m_PCBangHappyHourEvent.atimeEndTime2 <= *pDateTime)
	{// 2007-10-31 by cmkwon, PCBang HappyHourEvent 기간이 아니다

		if(PCBANG_HAPPYEV_STATE_TYPE_END != m_byPCBangHappyEvState)
		{
			g_pFieldGlobal->WriteSystemLogEX(TRUE, "[Notify] PCBang HappyHourEvent End !!, Period(%s ~ %s) ExpRate(%4.2f) SPIRate(%4.2f) ExpRepireRate(%4.2f) DropItemRate(%4.2f) DropRareItemRate(%4.2f) WarPointRate(%4.2f) MinLevel(%d) MaxLevel(%d)\r\n"
				, m_PCBangHappyHourEvent.atimeStartTime2.GetDateTimeString().GetBuffer(), m_PCBangHappyHourEvent.atimeEndTime2.GetDateTimeString().GetBuffer()
				, m_PCBangHappyHourEvent.fEXPRate2, m_PCBangHappyHourEvent.fSPIRate2, m_PCBangHappyHourEvent.fEXPRepairRate2
				, m_PCBangHappyHourEvent.fDropItemRate2, m_PCBangHappyHourEvent.fDropRareItemRate2, m_PCBangHappyHourEvent.fWarPointRate2, m_PCBangHappyHourEvent.MinLevel, m_PCBangHappyHourEvent.MaxLevel);

			///////////////////////////////////////////////////////////////////////////////
			// 2007-10-31 by cmkwon, PCBang HappyHourEvent 종료로 설정
			m_byPCBangHappyEvState		= PCBANG_HAPPYEV_STATE_TYPE_END;

			///////////////////////////////////////////////////////////////////////////////
			// 2007-10-31 by cmkwon, GameEvent 에서 PCBang HappyHourEvent 를 종료함
			m_pGameEventManager->EndPCBangGameEvent();

			///////////////////////////////////////////////////////////////////////////////
			// 2007-10-31 by cmkwon, 모든 PCBang 유저에게 전송
			SHAPPY_HOUR_EVENT *pHappyEv	= &m_PCBangHappyHourEvent;
			INIT_MSG_WITH_BUFFER(MSG_FC_INFO_GET_HAPPY_HOUR_EVENT_INFO_OK, T_FC_INFO_GET_HAPPY_HOUR_EVENT_INFO_OK, pSPCBang, SendBuf);			
			pSPCBang->byStateType		= m_byPCBangHappyEvState;
			pSPCBang->byInfluenceType4	= pHappyEv->InfluenceType3;
			pSPCBang->StartATime		= pHappyEv->atimeStartTime2;
			pSPCBang->EndATime			= pHappyEv->atimeEndTime2;
			pSPCBang->fEXPRate2			= pHappyEv->fEXPRate2;
			pSPCBang->fSPIRate2			= pHappyEv->fSPIRate2;
			pSPCBang->fEXPRepairRate2	= pHappyEv->fEXPRepairRate2;
			pSPCBang->fDropItemRate2	= pHappyEv->fDropItemRate2;
			pSPCBang->fDropRareRate2	= pHappyEv->fDropRareItemRate2;
			pSPCBang->fWarPointRate2	= pHappyEv->fWarPointRate2;			// 2007-06-26 by dhjin, 워포인트 이벤트 관련 추가
			m_pFieldIOCP13->SendMessageToAllClients(SendBuf, MSG_SIZE(MSG_FC_INFO_GET_HAPPY_HOUR_EVENT_INFO_OK), INFLUENCE_TYPE_ALL_MASK, pHappyEv->MinLevel, pHappyEv->MaxLevel, NULL, TRUE);			// 2008-03-13 by dhjin, Level별 해피아워 이벤트 구현 - 
		}
		return;
	}

	///////////////////////////////////////////////////////////////////////////////
	// 2007-10-31 by cmkwon, PCBang HappyHourEvent 기간


	if(PCBANG_HAPPYEV_STATE_TYPE_END != m_byPCBangHappyEvState)
	{// 2007-10-31 by cmkwon, PCBang HappyHourEvent 가 진행 중이다
		return;
	}

	int nEventTimeMinute	= m_PCBangHappyHourEvent.atimeEndTime2.GetTimeDiffTimeInMinutes(m_PCBangHappyHourEvent.atimeStartTime2);
	if(0 >= nEventTimeMinute)
	{// 2007-10-31 by cmkwon, PCBang HappyHourEvent 기간에 오류가 있다
		return;
	}

	g_pFieldGlobal->WriteSystemLogEX(TRUE, "[Notify] PCBang HappyHourEvent Start !!, Period(%s ~ %s) ExpRate(%4.2f) SPIRate(%4.2f) ExpRepireRate(%4.2f) DropItemRate(%4.2f) DropRareItemRate(%4.2f) WarPointRate(%4.2f) MinLevel(%d) MaxLevel(%d)\r\n"
		, m_PCBangHappyHourEvent.atimeStartTime2.GetDateTimeString().GetBuffer(), m_PCBangHappyHourEvent.atimeEndTime2.GetDateTimeString().GetBuffer()
		, m_PCBangHappyHourEvent.fEXPRate2, m_PCBangHappyHourEvent.fSPIRate2, m_PCBangHappyHourEvent.fEXPRepairRate2
		, m_PCBangHappyHourEvent.fDropItemRate2, m_PCBangHappyHourEvent.fDropRareItemRate2, m_PCBangHappyHourEvent.fWarPointRate2, m_PCBangHappyHourEvent.MinLevel, m_PCBangHappyHourEvent.MaxLevel);
	
	///////////////////////////////////////////////////////////////////////////////
	// 2007-10-31 by cmkwon, PCBang HappyHourEvent 진행 시작 으로 변경
	m_byPCBangHappyEvState		= PCBANG_HAPPYEV_STATE_TYPE_START;
	
	///////////////////////////////////////////////////////////////////////////////
	// 2007-10-31 by cmkwon, GameEvent 에 PCBang HappyHourEvent 를 적용함
	m_pGameEventManager->StartPCBangGameEvent(GAME_EVENT_TYPE_EXP, m_PCBangHappyHourEvent.fEXPRate2, nEventTimeMinute);
	m_pGameEventManager->StartPCBangGameEvent(GAME_EVENT_TYPE_SPI, m_PCBangHappyHourEvent.fSPIRate2, nEventTimeMinute);
	m_pGameEventManager->StartPCBangGameEvent(GAME_EVENT_TYPE_EXP_NO_DOWN, m_PCBangHappyHourEvent.fEXPRepairRate2, nEventTimeMinute);
	m_pGameEventManager->StartPCBangGameEvent(GAME_EVENT_TYPE_DROP_ITEM, m_PCBangHappyHourEvent.fDropItemRate2, nEventTimeMinute);
	m_pGameEventManager->StartPCBangGameEvent(GAME_EVENT_TYPE_DROP_REAR_ITEM, m_PCBangHappyHourEvent.fDropRareItemRate2, nEventTimeMinute);
	m_pGameEventManager->StartPCBangGameEvent(GAME_EVENT_TYPE_WARPOINT, m_PCBangHappyHourEvent.fWarPointRate2, nEventTimeMinute);

	///////////////////////////////////////////////////////////////////////////////
	// 2007-10-31 by cmkwon, 모든 PCBang 유저에게 전송
	SHAPPY_HOUR_EVENT *pHappyEv	= &m_PCBangHappyHourEvent;
	INIT_MSG_WITH_BUFFER(MSG_FC_INFO_GET_HAPPY_HOUR_EVENT_INFO_OK, T_FC_INFO_GET_HAPPY_HOUR_EVENT_INFO_OK, pSPCBang, SendBuf);
	pSPCBang->byStateType		= m_byPCBangHappyEvState;
	pSPCBang->byInfluenceType4	= pHappyEv->InfluenceType3;
	pSPCBang->StartATime		= pHappyEv->atimeStartTime2;
	pSPCBang->EndATime			= pHappyEv->atimeEndTime2;
	pSPCBang->fEXPRate2			= pHappyEv->fEXPRate2;
	pSPCBang->fSPIRate2			= pHappyEv->fSPIRate2;
	pSPCBang->fEXPRepairRate2	= pHappyEv->fEXPRepairRate2;
	pSPCBang->fDropItemRate2	= pHappyEv->fDropItemRate2;
	pSPCBang->fDropRareRate2	= pHappyEv->fDropRareItemRate2;
	pSPCBang->fWarPointRate2	= pHappyEv->fWarPointRate2;			// 2007-06-26 by dhjin, 워포인트 이벤트 관련 추가
	m_pFieldIOCP13->SendMessageToAllClients(SendBuf, MSG_SIZE(MSG_FC_INFO_GET_HAPPY_HOUR_EVENT_INFO_OK), INFLUENCE_TYPE_ALL_MASK, pHappyEv->MinLevel, pHappyEv->MaxLevel, NULL, TRUE);		// 2008-03-13 by dhjin, Level별 해피아워 이벤트 구현 - 
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CHappyHourEventManager::OnDoMinutelyWorkHappyHourEventManagerAllInfluenceType(ATUM_DATE_TIME *pDateTime)
/// \brief		// 2007-10-30 by cmkwon, 세력별 해피아워 이벤트 구현 - CHappyHourEventManager::OnDoMinutelyWorkHappyHourEventManagerAllInfluenceType() 함수 추가
/// \author		cmkwon
/// \date		2007-10-31 ~ 2007-10-31
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CHappyHourEventManager::OnDoMinutelyWorkHappyHourEventManagerAllInfluenceType(ATUM_DATE_TIME *pDateTime)
{
	int i=0;
	for(i=0; i < INFLUENCE_TYPE_COUNT; i++)
	{
		SHAPPY_HOUR_EVENT_PERIOD *pHappyEvPeriod = &m_arrPeriodEachInfluenceType[i];
		this->OnDoMinutelyWorkHappyHourEventManagerOfInfluenceType(pDateTime, pHappyEvPeriod);
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CHappyHourEventManager::OnDoMinutelyWorkHappyHourEventManagerOfInfluenceType(ATUM_DATE_TIME *pDateTime, SHAPPY_HOUR_EVENT_PERIOD *i_pHappyEvPeriod)
/// \brief		// 2007-10-30 by cmkwon, 세력별 해피아워 이벤트 구현 - CHappyHourEventManager::OnDoMinutelyWorkHappyHourEventManagerOfInfluenceType() 함수 추가
/// \author		cmkwon
/// \date		2007-10-31 ~ 2007-10-31
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CHappyHourEventManager::OnDoMinutelyWorkHappyHourEventManagerOfInfluenceType(ATUM_DATE_TIME *pDateTime, SHAPPY_HOUR_EVENT_PERIOD *i_pHappyEvPeriod)
{
	SHAPPY_HOUR_EVENT *pHappyEv = this->GetCurrentHappyHourEventByInfluenceType(i_pHappyEvPeriod->InfluenceType3);

	if(i_pHappyEvPeriod->atimeStartTime2 >= *pDateTime
		|| i_pHappyEvPeriod->atimeEndTime2 <= *pDateTime)
	{// 2007-10-31 by cmkwon, HappyHourEvent 기간이 아니다

		if(pHappyEv)
		{// 2007-10-31 by cmkwon, 진행중이던 HappyHourEvnet 가 있다, HappyHourEvnet 종료 처리해야 함

			g_pFieldGlobal->WriteSystemLogEX(TRUE, "[Notify] HappyHourEvent End !!, InfluenceType(%3d) Day(%s:%d) Period(%s ~ %s) ExpRate(%4.2f) SPIRate(%4.2f) ExpRepireRate(%4.2f) DropItemRate(%4.2f) DropRareItemRate(%4.2f) WarPointRate(%4.2f) MinLevel(%3d) MaxLevel(%3d)\r\n"
				, pHappyEv->InfluenceType3, GetDayOfWeekString(pHappyEv->DayOfWeek), pHappyEv->DayOfWeek
				, pHappyEv->atimeStartTime2.GetDateTimeString().GetBuffer(), pHappyEv->atimeEndTime2.GetDateTimeString().GetBuffer()
				, pHappyEv->fEXPRate2, pHappyEv->fSPIRate2, pHappyEv->fEXPRepairRate2
				, pHappyEv->fDropItemRate2, pHappyEv->fDropRareItemRate2, pHappyEv->fWarPointRate2
				, pHappyEv->MinLevel, pHappyEv->MaxLevel);		// 2007-10-31 by cmkwon, 해당 세력의 GameEvent 를 초기화 한다.

			///////////////////////////////////////////////////////////////////////////////
			// 2007-10-31 by cmkwon, 해당 이벤트를 임시 변수에 할당
			SHAPPY_HOUR_EVENT tmHappyEv = *pHappyEv;

			///////////////////////////////////////////////////////////////////////////////
			// 2007-10-31 by cmkwon, 해당 세력의 Current HappyHourEvent 를 NULL 로 초기화 한다., pHappyEv 변수는 유효하지 않음
			this->SetCurrentHappyHourEventByInfluenceType(tmHappyEv.InfluenceType3, NULL);

			///////////////////////////////////////////////////////////////////////////////
			// 2007-10-31 by cmkwon, 해당 세력의 GameEvent 를 초기화 한다.
			m_pGameEventManager->EndGameEventAllByInfluenceType(tmHappyEv.InfluenceType3);

			///////////////////////////////////////////////////////////////////////////////
			// 2007-10-31 by cmkwon, 해당 세력의 모든 유저에게 전송한다.
			INIT_MSG_WITH_BUFFER(MSG_FC_INFO_GET_HAPPY_HOUR_EVENT_INFO_OK, T_FC_INFO_GET_HAPPY_HOUR_EVENT_INFO_OK, pSHappy, SendBuf);
			pSHappy->byStateType			= HAPPYEV_STATE_TYPE_END;
			pSHappy->byInfluenceType4		= tmHappyEv.InfluenceType3;
			pSHappy->StartATime				= tmHappyEv.atimeStartTime2;
			pSHappy->EndATime				= tmHappyEv.atimeEndTime2;
			pSHappy->fEXPRate2				= tmHappyEv.fEXPRate2;
			pSHappy->fSPIRate2				= tmHappyEv.fSPIRate2;
			pSHappy->fEXPRepairRate2		= tmHappyEv.fEXPRepairRate2;
			pSHappy->fDropItemRate2			= tmHappyEv.fDropItemRate2;
			pSHappy->fDropRareRate2			= tmHappyEv.fDropRareItemRate2;
			pSHappy->fWarPointRate2			= tmHappyEv.fWarPointRate2;					// 2007-06-26 by dhjin, 워포인트 이벤트 관련 추가
			m_pFieldIOCP13->SendMessageToAllClients(SendBuf, MSG_SIZE(MSG_FC_INFO_GET_HAPPY_HOUR_EVENT_INFO_OK), tmHappyEv.InfluenceType3, tmHappyEv.MinLevel, tmHappyEv.MaxLevel);	// 2008-03-13 by dhjin, Level별 해피아워 이벤트 구현 - 
		}

		return;
	}

	auto localTime = tm(*pDateTime);
	if(pHappyEv)
	{// 2007-10-31 by cmkwon, 진행 중인 HappyHourEvent 가 있다, 종료 체크 해야 함
		
		pHappyEv->atimeStartTime2.Year	= pHappyEv->atimeEndTime2.Year	= pDateTime->Year;
		pHappyEv->atimeStartTime2.Month	= pHappyEv->atimeEndTime2.Month	= pDateTime->Month;
		pHappyEv->atimeStartTime2.Day	= pHappyEv->atimeEndTime2.Day	= pDateTime->Day;

		if(pHappyEv->DayOfWeek != localTime.tm_wday
			|| pHappyEv->atimeStartTime2 > *pDateTime
			|| pHappyEv->atimeEndTime2 <= *pDateTime)
		{// HappyHourEvent 종료 처리 해야 함

			g_pFieldGlobal->WriteSystemLogEX(TRUE, "[Notify] HappyHourEvent End !!, InfluenceType(%3d) Day(%s:%d) Period(%s ~ %s) ExpRate(%4.2f) SPIRate(%4.2f) ExpRepireRate(%4.2f) DropItemRate(%4.2f) DropRareItemRate(%4.2f) WarPointRate(%4.2f) MinLevel(%3d) MaxLevel(%3d)\r\n"
				, pHappyEv->InfluenceType3, GetDayOfWeekString(pHappyEv->DayOfWeek), pHappyEv->DayOfWeek
				, pHappyEv->atimeStartTime2.GetDateTimeString().GetBuffer(), pHappyEv->atimeEndTime2.GetDateTimeString().GetBuffer()
				, pHappyEv->fEXPRate2, pHappyEv->fSPIRate2, pHappyEv->fEXPRepairRate2
				, pHappyEv->fDropItemRate2, pHappyEv->fDropRareItemRate2, pHappyEv->fWarPointRate2
				, pHappyEv->MinLevel, pHappyEv->MaxLevel);		// 2007-10-31 by cmkwon, 해당 세력의 GameEvent 를 초기화 한다.

			///////////////////////////////////////////////////////////////////////////////
			// 2007-10-31 by cmkwon, 해당 이벤트를 임시 변수에 할당
			SHAPPY_HOUR_EVENT tmHappyEv = *pHappyEv;

			///////////////////////////////////////////////////////////////////////////////
			// 2007-10-31 by cmkwon, 해당 세력의 Current HappyHourEvent 를 NULL 로 초기화 한다., pHappyEv 변수는 유효하지 않음
			this->SetCurrentHappyHourEventByInfluenceType(tmHappyEv.InfluenceType3, NULL);

			///////////////////////////////////////////////////////////////////////////////
			// 2007-10-31 by cmkwon, 해당 세력의 GameEvent 를 초기화 한다.
			m_pGameEventManager->EndGameEventAllByInfluenceType(tmHappyEv.InfluenceType3);

			///////////////////////////////////////////////////////////////////////////////
			// 2007-10-31 by cmkwon, 해당 세력의 모든 유저에게 전송한다.
			INIT_MSG_WITH_BUFFER(MSG_FC_INFO_GET_HAPPY_HOUR_EVENT_INFO_OK, T_FC_INFO_GET_HAPPY_HOUR_EVENT_INFO_OK, pSHappy, SendBuf);
			pSHappy->byStateType			= HAPPYEV_STATE_TYPE_END;
			pSHappy->byInfluenceType4		= tmHappyEv.InfluenceType3;
			pSHappy->StartATime				= tmHappyEv.atimeStartTime2;
			pSHappy->EndATime				= tmHappyEv.atimeEndTime2;
			pSHappy->fEXPRate2				= tmHappyEv.fEXPRate2;
			pSHappy->fSPIRate2				= tmHappyEv.fSPIRate2;
			pSHappy->fEXPRepairRate2		= tmHappyEv.fEXPRepairRate2;
			pSHappy->fDropItemRate2			= tmHappyEv.fDropItemRate2;
			pSHappy->fDropRareRate2			= tmHappyEv.fDropRareItemRate2;
			pSHappy->fWarPointRate2			= tmHappyEv.fWarPointRate2;					// 2007-06-26 by dhjin, 워포인트 이벤트 관련 추가
			m_pFieldIOCP13->SendMessageToAllClients(SendBuf, MSG_SIZE(MSG_FC_INFO_GET_HAPPY_HOUR_EVENT_INFO_OK), tmHappyEv.InfluenceType3, tmHappyEv.MinLevel, tmHappyEv.MaxLevel);	// 2008-03-13 by dhjin, Level별 해피아워 이벤트 구현 - 
		}
		return;
	}

	pHappyEv = FindHappyHourEventByInfluenceType8Time(i_pHappyEvPeriod->InfluenceType3, pDateTime, localTime.tm_wday);
	if(NULL == pHappyEv)
	{// 2007-10-31 by cmkwon, 진행 할 HappyHourEvent 가 없다
		return;
	}

	int nEventTimeMinute	= pHappyEv->atimeEndTime2.GetTimeDiffTimeInMinutes(pHappyEv->atimeStartTime2);
	if(0 >= nEventTimeMinute)
	{// HappyHourEvent 진행 시간 오류
		return;
	}

	g_pFieldGlobal->WriteSystemLogEX(TRUE, "[Notify] HappyHourEvent Start !!, InfluenceType(%3d) Day(%s:%d) Period(%s ~ %s) ExpRate(%4.2f) SPIRate(%4.2f) ExpRepireRate(%4.2f) DropItemRate(%4.2f) DropRareItemRate(%4.2f) WarPointRate(%4.2f) MinLevel(%3d) MaxLevel(%3d)\r\n"
		, pHappyEv->InfluenceType3, GetDayOfWeekString(pHappyEv->DayOfWeek), pHappyEv->DayOfWeek
		, pHappyEv->atimeStartTime2.GetDateTimeString().GetBuffer(), pHappyEv->atimeEndTime2.GetDateTimeString().GetBuffer()
		, pHappyEv->fEXPRate2, pHappyEv->fSPIRate2, pHappyEv->fEXPRepairRate2
		, pHappyEv->fDropItemRate2, pHappyEv->fDropRareItemRate2, pHappyEv->fWarPointRate2
		, pHappyEv->MinLevel, pHappyEv->MaxLevel);		// 2007-10-31 by cmkwon, 해당 세력의 GameEvent 를 초기화 한다.
	
	///////////////////////////////////////////////////////////////////////////////
	// 2007-10-31 by cmkwon, 해당 세력의 Current HappyHourEvent 를 설정한다.
	this->SetCurrentHappyHourEventByInfluenceType(pHappyEv->InfluenceType3, pHappyEv);
	
	///////////////////////////////////////////////////////////////////////////////
	// 2007-10-31 by cmkwon, 해당 세력의 Current HappyHourEvent 를 GameEvent 에 적용 한다.
	m_pGameEventManager->StartGameEvent(GAME_EVENT_TYPE_EXP, pHappyEv->fEXPRate2, nEventTimeMinute, pHappyEv->MinLevel, pHappyEv->MaxLevel, pHappyEv->InfluenceType3);		// 2008-03-14 by dhjin, Level별 해피아워 이벤트 구현 -		
	m_pGameEventManager->StartGameEvent(GAME_EVENT_TYPE_SPI, pHappyEv->fSPIRate2, nEventTimeMinute, pHappyEv->MinLevel, pHappyEv->MaxLevel, pHappyEv->InfluenceType3);		// 2008-03-14 by dhjin, Level별 해피아워 이벤트 구현 -
	m_pGameEventManager->StartGameEvent(GAME_EVENT_TYPE_EXP_NO_DOWN, pHappyEv->fEXPRepairRate2, nEventTimeMinute, pHappyEv->MinLevel, pHappyEv->MaxLevel, pHappyEv->InfluenceType3);		// 2008-03-14 by dhjin, Level별 해피아워 이벤트 구현 -
	m_pGameEventManager->StartGameEvent(GAME_EVENT_TYPE_DROP_ITEM, pHappyEv->fDropItemRate2, nEventTimeMinute, pHappyEv->MinLevel, pHappyEv->MaxLevel, pHappyEv->InfluenceType3);			// 2008-03-14 by dhjin, Level별 해피아워 이벤트 구현 -
	m_pGameEventManager->StartGameEvent(GAME_EVENT_TYPE_DROP_REAR_ITEM, pHappyEv->fDropRareItemRate2, nEventTimeMinute, pHappyEv->MinLevel, pHappyEv->MaxLevel, pHappyEv->InfluenceType3);	// 2008-03-14 by dhjin, Level별 해피아워 이벤트 구현 -
	m_pGameEventManager->StartGameEvent(GAME_EVENT_TYPE_WARPOINT, pHappyEv->fWarPointRate2, nEventTimeMinute, pHappyEv->MinLevel, pHappyEv->MaxLevel, pHappyEv->InfluenceType3);			// 2008-03-14 by dhjin, Level별 해피아워 이벤트 구현 -
	
	///////////////////////////////////////////////////////////////////////////////
	// 2007-10-31 by cmkwon, 해당 세력의 모든 유저에게 전송한다.
	INIT_MSG_WITH_BUFFER(MSG_FC_INFO_GET_HAPPY_HOUR_EVENT_INFO_OK, T_FC_INFO_GET_HAPPY_HOUR_EVENT_INFO_OK, pSHappy, SendBuf);
	pSHappy->byStateType			= HAPPYEV_STATE_TYPE_START;
	pSHappy->byInfluenceType4		= pHappyEv->InfluenceType3;
	pSHappy->StartATime				= pHappyEv->atimeStartTime2;
	pSHappy->EndATime				= pHappyEv->atimeEndTime2;
	pSHappy->fEXPRate2				= pHappyEv->fEXPRate2;
	pSHappy->fSPIRate2				= pHappyEv->fSPIRate2;
	pSHappy->fEXPRepairRate2		= pHappyEv->fEXPRepairRate2;
	pSHappy->fDropItemRate2			= pHappyEv->fDropItemRate2;
	pSHappy->fDropRareRate2			= pHappyEv->fDropRareItemRate2;
	pSHappy->fWarPointRate2			= pHappyEv->fWarPointRate2;
	m_pFieldIOCP13->SendMessageToAllClients(SendBuf, MSG_SIZE(MSG_FC_INFO_GET_HAPPY_HOUR_EVENT_INFO_OK), pHappyEv->InfluenceType3, pHappyEv->MinLevel, pHappyEv->MaxLevel);		// 2008-03-13 by dhjin, Level별 해피아워 이벤트 구현 - 
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			SHAPPY_HOUR_EVENT *CHappyHourEventManager::GetCurrentHappyHourEventByInfluenceType(BYTE i_byInflTy)
/// \brief		// 2007-10-30 by cmkwon, 세력별 해피아워 이벤트 구현 - CHappyHourEventManager::GetCurrentHappyHourEventByInfluenceType() 함수 추가 함
/// \author		cmkwon
/// \date		2007-10-31 ~ 2007-10-31
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
SHAPPY_HOUR_EVENT *CHappyHourEventManager::GetCurrentHappyHourEventByInfluenceType(BYTE i_byInflTy)
{
	int nArrIdx = GetArrayIndexByInfluenceType(i_byInflTy);
	if(0 > nArrIdx)
	{
		return NULL;
	}
	return m_pArrCurHappyHourEventEachInfluenceType[nArrIdx];
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CHappyHourEventManager::SetCurrentHappyHourEventByInfluenceType(BYTE i_byInflTy, SHAPPY_HOUR_EVENT *i_pHappyEv)
/// \brief		// 2007-10-30 by cmkwon, 세력별 해피아워 이벤트 구현 - CHappyHourEventManager::SetCurrentHappyHourEventByInfluenceType() 함수 추가 함
/// \author		cmkwon
/// \date		2007-10-31 ~ 2007-10-31
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CHappyHourEventManager::SetCurrentHappyHourEventByInfluenceType(BYTE i_byInflTy, SHAPPY_HOUR_EVENT *i_pHappyEv)
{
	int nArrIdx = GetArrayIndexByInfluenceType(i_byInflTy);
	if(0 > nArrIdx)
	{
		return FALSE;
	}
	
	m_pArrCurHappyHourEventEachInfluenceType[nArrIdx] = i_pHappyEv;
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			SHAPPY_HOUR_EVENT *CHappyHourEventManager::FindHappyHourEventByInfluenceType8Time(BYTE i_byInflTy, ATUM_DATE_TIME *pCurDateTime, int i_CurDayOfWeek)
/// \brief		// 2007-10-30 by cmkwon, 세력별 해피아워 이벤트 구현 - CHappyHourEventManager::FindHappyHourEventByInfluenceType8Time() 함수 추가
/// \author		cmkwon
/// \date		2007-10-31 ~ 2007-10-31
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
SHAPPY_HOUR_EVENT *CHappyHourEventManager::FindHappyHourEventByInfluenceType8Time(BYTE i_byInflTy, ATUM_DATE_TIME *pCurDateTime, int i_CurDayOfWeek)
{
	for(int i=0; i < m_mtvectHappyHourEventList.size(); i++)
	{
		SHAPPY_HOUR_EVENT *pHappyEv = &m_mtvectHappyHourEventList[i];
		if(i_byInflTy != pHappyEv->InfluenceType3)
		{// 2007-10-31 by cmkwon, 세력 체크
			continue;
		}

		// 2007-10-31 by cmkwon, 실제 요일(일~토)만 체크 해야함
		switch(pHappyEv->DayOfWeek)
		{
		case DAY_OF_WEEK_SUMDAY:
		case DAY_OF_WEEK_MONDAY:
		case DAY_OF_WEEK_TUESDAY:
		case DAY_OF_WEEK_WEDNESDAY:
		case DAY_OF_WEEK_THURSDAY:	
		case DAY_OF_WEEK_FRIDAY:
		case DAY_OF_WEEK_SATURDAY:
			break;
		default:
			{
				continue;		// 2007-10-31 by cmkwon, 요일이 아닌 데이터는 체크 필요 없다
			}
		}
		
		if(pHappyEv->DayOfWeek != i_CurDayOfWeek)
		{// 2007-10-31 by cmkwon, 요일이 같은지 체크
			continue;
		}

		pHappyEv->atimeStartTime2.Year	= pHappyEv->atimeEndTime2.Year	= pCurDateTime->Year;
		pHappyEv->atimeStartTime2.Month	= pHappyEv->atimeEndTime2.Month	= pCurDateTime->Month;
		pHappyEv->atimeStartTime2.Day	= pHappyEv->atimeEndTime2.Day	= pCurDateTime->Day;

		if(pHappyEv->atimeStartTime2 > *pCurDateTime
			|| pHappyEv->atimeEndTime2 <= *pCurDateTime)
		{// 2007-10-31 by cmkwon, 시간을 체크
			continue;
		}

		return pHappyEv;	// 2007-10-31 by cmkwon, 유효한 HappyHourEvent 를 리턴
	}

	return NULL;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CHappyHourEventManager::SendHappyHourEventInfo(CFieldIOCPSocket *i_pToFISock, BYTE i_byInflTy, MessageType_t i_msgTy)
/// \brief		// 2007-10-30 by cmkwon, 세력별 해피아워 이벤트 구현 - CHappyHourEventManager::SendHappyHourEventInfo() 함수 추가 함
///				// 2008-05-20 by cmkwon, 모든 이벤트(HappyHoure,MotherShip,Item) 그룹 동시에 가능하게 - CHappyHourEventManager::SendHappyHourEventInfo() 인자 추가
/// \author		cmkwon
/// \date		2007-10-31 ~ 2007-10-31
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CHappyHourEventManager::SendHappyHourEventInfo(CFieldIOCPSocket *i_pToFISock, BYTE i_byInflTy, MessageType_t i_msgTy)
{
	mt_auto_lock mtHappy(this->GetVectHappyHourEventListPtr());	// 2006-08-23 by cmkwon

	if(FALSE == i_pToFISock->IsValidCharacter(FALSE))
	{
		return FALSE;
	}

	BOOL bRet = FALSE;
	
	// 2008-05-20 by cmkwon, 모든 이벤트(HappyHoure,MotherShip,Item) 그룹 동시에 가능하게 - 아래와 같이 i_msgTy 를 인자로 받아서 설정 한다.
	//INIT_MSG_WITH_BUFFER(MSG_FC_INFO_GET_HAPPY_HOUR_EVENT_INFO_OK, T_FC_INFO_GET_HAPPY_HOUR_EVENT_INFO_OK, pSHappyEv, SendBuf);
	
	INIT_MSG_WITH_BUFFER(MSG_FC_INFO_GET_HAPPY_HOUR_EVENT_INFO_OK, 0, pSHappyEv, SendBuf);		// 2008-05-20 by cmkwon, 모든 이벤트(HappyHoure,MotherShip,Item) 그룹 동시에 가능하게 - 
	*(MessageType_t*)SendBuf = i_msgTy;

	//if(i_pToFISock->m_bIsPCBangClient
	//	&& PCBANG_HAPPYEV_STATE_TYPE_END != m_byPCBangHappyEvState)
	//{
	//	SHAPPY_HOUR_EVENT *pHappyEv		= &m_PCBangHappyHourEvent;		

	//	bRet							= TRUE;

	//	pSHappyEv->byStateType			= PCBANG_HAPPYEV_STATE_TYPE_STARTING;
	//	pSHappyEv->byInfluenceType4		= pHappyEv->InfluenceType3;
	//	pSHappyEv->StartATime			= pHappyEv->atimeStartTime2;
	//	pSHappyEv->EndATime				= pHappyEv->atimeEndTime2;
	//	pSHappyEv->fEXPRate2			= pHappyEv->fEXPRate2;
	//	pSHappyEv->fSPIRate2			= pHappyEv->fSPIRate2;
	//	pSHappyEv->fEXPRepairRate2		= pHappyEv->fEXPRepairRate2;
	//	pSHappyEv->fDropItemRate2		= pHappyEv->fDropItemRate2;
	//	pSHappyEv->fDropRareRate2		= pHappyEv->fDropRareItemRate2;
	//	pSHappyEv->fWarPointRate2		= pHappyEv->fWarPointRate2;
	//	i_pToFISock->SendAddData(SendBuf, MSG_SIZE(MSG_FC_INFO_GET_HAPPY_HOUR_EVENT_INFO_OK));
	//}

	int i=0;
	for(i=0; i < INFLUENCE_TYPE_COUNT; i++)
	{
		SHAPPY_HOUR_EVENT *pHappyEv = m_pArrCurHappyHourEventEachInfluenceType[i];
		if(NULL == pHappyEv
			|| FALSE == util::in_range(pHappyEv->MinLevel, i_pToFISock->m_character.Level, pHappyEv->MaxLevel))
		{// 2008-03-13 by dhjin, Level별 해피아워 이벤트 구현 - 레벨 체크 추가
			continue;
		}

		if(INFLUENCE_TYPE_ALL_MASK == pHappyEv->InfluenceType3
			|| i_pToFISock->GetCharacter()->InfluenceType == pHappyEv->InfluenceType3)
		{
			bRet							= TRUE;

			pSHappyEv->byStateType			= HAPPYEV_STATE_TYPE_STARTING;
			pSHappyEv->byInfluenceType4		= pHappyEv->InfluenceType3;
			pSHappyEv->StartATime			= pHappyEv->atimeStartTime2;
			pSHappyEv->EndATime				= pHappyEv->atimeEndTime2;
			pSHappyEv->fEXPRate2			= pHappyEv->fEXPRate2;
			pSHappyEv->fSPIRate2			= pHappyEv->fSPIRate2;
			pSHappyEv->fEXPRepairRate2		= pHappyEv->fEXPRepairRate2;
			pSHappyEv->fDropItemRate2		= pHappyEv->fDropItemRate2;
			pSHappyEv->fDropRareRate2		= pHappyEv->fDropRareItemRate2;
			pSHappyEv->fWarPointRate2		= pHappyEv->fWarPointRate2;
			i_pToFISock->SendAddData(SendBuf, MSG_SIZE(MSG_FC_INFO_GET_HAPPY_HOUR_EVENT_INFO_OK));
		}
	}

	return bRet;
}


//////////////////////////////////////////////////////////////////////
// 2012-10-05 by jhseol, NPlay PC방 모듈 - 해피아워 정보 가져오기
///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CHappyHourEventManager::LoadHappyHourEventInfo(CFieldIOCPSocket *i_pToFISock, BYTE i_byInflTy, MSG_FC_INFO_GET_HAPPY_HOUR_EVENT_INFO_OK* i_msg)
/// \brief		해피아워 정보 가져오기
/// \author		jhseol
/// \date		2012-10-05
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CHappyHourEventManager::LoadHappyHourEventInfo(CFieldIOCPSocket *i_pToFISock, BYTE i_byInflTy, MSG_FC_INFO_GET_HAPPY_HOUR_EVENT_INFO_OK* i_msg)
{
	mt_auto_lock mtHappy(this->GetVectHappyHourEventListPtr());	// 2006-08-23 by cmkwon
	
	if(FALSE == i_pToFISock->IsValidCharacter(FALSE))
	{
		return FALSE;
	}
	
	BOOL bRet = FALSE;
	
	//if(i_pToFISock->m_bIsPCBangClient)
	//{
	//	if( PCBANG_HAPPYEV_STATE_TYPE_END != m_byPCBangHappyEvState)
	//	{
	//		SHAPPY_HOUR_EVENT *pHappyEv		= &m_PCBangHappyHourEvent;		
	//		bRet							= TRUE;
	//		i_msg->byStateType			= PCBANG_HAPPYEV_STATE_TYPE_STARTING;
	//		i_msg->byInfluenceType4		= pHappyEv->InfluenceType3;
	//		i_msg->StartATime			= pHappyEv->atimeStartTime2;
	//		i_msg->EndATime				= pHappyEv->atimeEndTime2;
	//		i_msg->fEXPRate2			= pHappyEv->fEXPRate2;
	//		i_msg->fSPIRate2			= pHappyEv->fSPIRate2;
	//		i_msg->fEXPRepairRate2		= pHappyEv->fEXPRepairRate2;
	//		i_msg->fDropItemRate2		= pHappyEv->fDropItemRate2;
	//		i_msg->fDropRareRate2		= pHappyEv->fDropRareItemRate2;
	//		i_msg->fWarPointRate2		= pHappyEv->fWarPointRate2;
	//	}
	//	else
	//	{
	//		i_msg->byStateType			= PCBANG_HAPPYEV_STATE_TYPE_END;
	//	}
	//}
	//else
	//{
		i_msg->byStateType			= PCBANG_HAPPYEV_STATE_TYPE_END;
	//}
	
	return bRet;
}
// 2012-10-05 by jhseol, NPlay PC방 모듈 - 해피아워 정보 가져오기

// 2008-05-20 by cmkwon, 모든 이벤트(HappyHoure,MotherShip,Item) 그룹 동시에 가능하게 - CHappyHourEventManager::SendHappyHourEventInfo() 함수에 i_msgTy 인자를 추가하여 처리함. 이 함수는 필요 없음
// ///////////////////////////////////////////////////////////////////////////////
// /// \fn			BOOL CHappyHourEventManager::SendHappyHourEventInfoByLevel(CFieldIOCPSocket *i_pToFISock, BYTE i_byInflTy)
// /// \brief		Level별 해피아워 이벤트 구현 -	
// /// \author		dhjin
// /// \date		2008-03-14 ~ 2008-03-14
// /// \warning	
// ///
// /// \param		
// /// \return		
// ///////////////////////////////////////////////////////////////////////////////
// BOOL CHappyHourEventManager::SendHappyHourEventInfoByLevel(CFieldIOCPSocket *i_pToFISock, BYTE i_byInflTy)
// {
// 	mt_auto_lock mtHappy(this->GetVectHappyHourEventListPtr());	// 2006-08-23 by cmkwon
// 
// 	if(FALSE == i_pToFISock->IsValidCharacter(FALSE))
// 	{
// 		return FALSE;
// 	}
// 
// 	BOOL bRet = FALSE;
// 	
// 	INIT_MSG_WITH_BUFFER(MSG_FC_INFO_GET_HAPPY_HOUR_EVENT_INFO_OK, T_FC_INFO_GET_HAPPY_HOUR_EVENT_INFO_BY_LEVEL, pSHappyEv, SendBuf);
// 	if(i_pToFISock->m_bIsPCBangClient
// 		&& PCBANG_HAPPYEV_STATE_TYPE_END != m_byPCBangHappyEvState)
// 	{
// 		SHAPPY_HOUR_EVENT *pHappyEv		= &m_PCBangHappyHourEvent;		
// 
// 		bRet							= TRUE;
// 
// 		pSHappyEv->byStateType			= PCBANG_HAPPYEV_STATE_TYPE_STARTING;
// 		pSHappyEv->byInfluenceType4		= pHappyEv->InfluenceType3;
// 		pSHappyEv->StartATime			= pHappyEv->atimeStartTime2;
// 		pSHappyEv->EndATime				= pHappyEv->atimeEndTime2;
// 		pSHappyEv->fEXPRate2			= pHappyEv->fEXPRate2;
// 		pSHappyEv->fSPIRate2			= pHappyEv->fSPIRate2;
// 		pSHappyEv->fEXPRepairRate2		= pHappyEv->fEXPRepairRate2;
// 		pSHappyEv->fDropItemRate2		= pHappyEv->fDropItemRate2;
// 		pSHappyEv->fDropRareRate2		= pHappyEv->fDropRareItemRate2;
// 		pSHappyEv->fWarPointRate2		= pHappyEv->fWarPointRate2;
// 		i_pToFISock->SendAddData(SendBuf, MSG_SIZE(MSG_FC_INFO_GET_HAPPY_HOUR_EVENT_INFO_OK));
// 	}
// 
// 	int i=0;
// 	for(i=0; i < INFLUENCE_TYPE_COUNT; i++)
// 	{
// 		SHAPPY_HOUR_EVENT *pHappyEv = m_pArrCurHappyHourEventEachInfluenceType[i];
// 		if(NULL == pHappyEv
// 			|| FALSE == util::in_range(pHappyEv->MinLevel, i_pToFISock->m_character.Level, pHappyEv->MaxLevel))
// 		{// 2008-03-13 by dhjin, Level별 해피아워 이벤트 구현 - 레벨 체크 추가
// 			continue;
// 		}
// 
// 		if(INFLUENCE_TYPE_ALL_MASK == pHappyEv->InfluenceType3
// 			|| i_pToFISock->GetCharacter()->InfluenceType == pHappyEv->InfluenceType3)
// 		{
// 			bRet							= TRUE;
// 
// 			pSHappyEv->byStateType			= HAPPYEV_STATE_TYPE_STARTING;
// 			pSHappyEv->byInfluenceType4		= pHappyEv->InfluenceType3;
// 			pSHappyEv->StartATime			= pHappyEv->atimeStartTime2;
// 			pSHappyEv->EndATime				= pHappyEv->atimeEndTime2;
// 			pSHappyEv->fEXPRate2			= pHappyEv->fEXPRate2;
// 			pSHappyEv->fSPIRate2			= pHappyEv->fSPIRate2;
// 			pSHappyEv->fEXPRepairRate2		= pHappyEv->fEXPRepairRate2;
// 			pSHappyEv->fDropItemRate2		= pHappyEv->fDropItemRate2;
// 			pSHappyEv->fDropRareRate2		= pHappyEv->fDropRareItemRate2;
// 			pSHappyEv->fWarPointRate2		= pHappyEv->fWarPointRate2;
// 			i_pToFISock->SendAddData(SendBuf, MSG_SIZE(MSG_FC_INFO_GET_HAPPY_HOUR_EVENT_INFO_OK));
// 		}
// 	}
// 
// 	return bRet;
// }

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CHappyHourEventManager::SetHappyHourEventPeriod(BYTE i_byInflTy, ATUM_DATE_TIME i_atimeStart, ATUM_DATE_TIME i_atimeEnd, BOOL i_bStoreDB/*=FALSE*/)
/// \brief		// 2007-10-30 by cmkwon, 세력별 해피아워 이벤트 구현 - CHappyHourEventManager::SetHappyHourEventPeriod() 함수 추가
/// \author		cmkwon
/// \date		2007-11-01 ~ 2007-11-01
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CHappyHourEventManager::SetHappyHourEventPeriod(BYTE i_byInflTy, ATUM_DATE_TIME i_atimeStart, ATUM_DATE_TIME i_atimeEnd, BOOL i_bStoreDB/*=FALSE*/)
{
	int nArrIdx = GetArrayIndexByInfluenceType(i_byInflTy);
	if(0 > nArrIdx)
	{
		return FALSE;
	}
	
	SHAPPY_HOUR_EVENT_PERIOD *pHappyEvPeriod = &m_arrPeriodEachInfluenceType[nArrIdx];
	
	pHappyEvPeriod->atimeStartTime2			= i_atimeStart;
	pHappyEvPeriod->atimeEndTime2			= i_atimeEnd;


	if(i_bStoreDB)
	{
		QPARAM_UPDATE_HAPPY_HOUR_EVENT_DATE *pQParam = new QPARAM_UPDATE_HAPPY_HOUR_EVENT_DATE;
		pQParam->byInfluenceType0	= i_byInflTy;
		pQParam->atimeStartTime2	= i_atimeStart;
		pQParam->atimeEndTime2		= i_atimeStart;
		m_pFieldIOCP13->m_pAtumDBManager->MakeAndEnqueueQuery(QT_UpdateHappyHourEventDate, NULL, 0, pQParam);
	}
	return TRUE;
}