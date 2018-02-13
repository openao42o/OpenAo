// GameEventManager.cpp: implementation of the CGameEventManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GameEventManager.h"
#include "FieldIOCP.h"
#include "HappyHourEventManager.h"
#include "FieldGlobal.h"

float	CGameEventManager::ms_fDefaultExpRate			= 1.0f;
float	CGameEventManager::ms_fDefaultSPIRate			= 1.0f;
float	CGameEventManager::ms_fDefaultExpRepareRate		= 0.0f;
float	CGameEventManager::ms_fDefaultDropItemRate		= 1.0f;
float	CGameEventManager::ms_fDefaultDropRareItemRate	= 1.0f;
float	CGameEventManager::ms_fDefaultWarPointRate		= 1.0f;					// 2007-06-26 by dhjin, 워포인트 이벤트 관련 추가


char *CGameEventManager::GetGameEventTypeString(int i_nEvType)
{
	switch(i_nEvType)
	{
	case GAME_EVENT_TYPE_EXP:				return STRMSG_S_F2EVENTTYPE_0000;
	case GAME_EVENT_TYPE_SPI:				return STRMSG_S_F2EVENTTYPE_0001;
	case GAME_EVENT_TYPE_EXP_NO_DOWN:		return STRMSG_S_F2EVENTTYPE_0002;
	case GAME_EVENT_TYPE_DROP_ITEM:			return STRMSG_S_F2EVENTTYPE_0003;
	case GAME_EVENT_TYPE_DROP_REAR_ITEM:	return STRMSG_S_F2EVENTTYPE_0004;
	case GAME_EVENT_TYPE_WARPOINT:			return STRMSG_S_F2EVENTTYPE_0006;
	default:
		{
			return STRMSG_S_F2EVENTTYPE_0005;
		}
	}
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGameEventManager::CGameEventManager()
{
	m_pFieldIOCP10 = NULL;
}

CGameEventManager::~CGameEventManager()
{

}

void CGameEventManager::InitEventManager(CFieldIOCP *i_pFIOCP)
{
	m_pFieldIOCP10 = i_pFIOCP;
	util::zero(m_arrGameEvent, sizeof(m_arrGameEvent[0][0])*INFLUENCE_TYPE_COUNT*GAME_EVENT_TYPE_MAX_COUNT);
	util::zero(m_arrGameEventofMotherShip, sizeof(m_arrGameEventofMotherShip[0][0])*INFLUENCE_TYPE_COUNT*GAME_EVENT_TYPE_MAX_COUNT);	// 2008-05-20 by cmkwon, 모든 이벤트(HappyHoure,MotherShip,Item) 그룹 동시에 가능하게 - 
	util::zero(m_arrGameEventofItem, sizeof(m_arrGameEventofItem[0][0])*INFLUENCE_TYPE_COUNT*GAME_EVENT_TYPE_MAX_COUNT);				// 2008-05-20 by cmkwon, 모든 이벤트(HappyHoure,MotherShip,Item) 그룹 동시에 가능하게 - 
	util::zero(m_arrGameEventofCrystal, sizeof(m_arrGameEventofCrystal[0][0])*INFLUENCE_TYPE_COUNT*GAME_EVENT_TYPE_MAX_COUNT);				// 2011-10-28 by hskim, EP4 [트리거 시스템] - 크리스탈 시스템
	util::zero(m_arrGameEventofCrystal2ND, sizeof(m_arrGameEventofCrystal2ND[0][0])*INFLUENCE_TYPE_COUNT*GAME_EVENT_TYPE_MAX_COUNT);	// 2011-10-28 by hskim, EP4 [트리거 시스템] - 크리스탈 시스템
// 2008-05-20 by cmkwon, 모든 이벤트(HappyHoure,MotherShip,Item) 그룹 동시에 가능하게 - 
// 	for(int i=0; i < GAME_EVENT_TYPE_MAX_COUNT; i++)
// 	{
// 		m_arrGameEvent[0][i].byInfluenceMask = INFLUENCE_TYPE_NORMAL;
// 	}
// 	for(i=0; i < GAME_EVENT_TYPE_MAX_COUNT; i++)
// 	{
// 		m_arrGameEvent[1][i].byInfluenceMask = INFLUENCE_TYPE_VCN;
// 	}
// 	for(i=0; i < GAME_EVENT_TYPE_MAX_COUNT; i++)
// 	{
// 		m_arrGameEvent[2][i].byInfluenceMask = INFLUENCE_TYPE_ANI;
// 	}
// 	for(i=0; i < GAME_EVENT_TYPE_MAX_COUNT; i++)
// 	{
// 		m_arrGameEvent[3][i].byInfluenceMask = INFLUENCE_TYPE_ALL_MASK;
// 	}
	// 2008-05-20 by cmkwon, 모든 이벤트(HappyHoure,MotherShip,Item) 그룹 동시에 가능하게 - 
	int i;
	for(i=0; i < INFLUENCE_TYPE_COUNT; i++)
	{
		for(int j=0; j < GAME_EVENT_TYPE_MAX_COUNT; j++)
		{
			m_arrGameEvent[i][j].byInfluenceMask				= GetInfluenceTypeByArrayIndex(i);
			m_arrGameEventofMotherShip[i][j].byInfluenceMask	= GetInfluenceTypeByArrayIndex(i);
			m_arrGameEventofItem[i][j].byInfluenceMask			= GetInfluenceTypeByArrayIndex(i);
			m_arrGameEventofCrystal[i][j].byInfluenceMask		= GetInfluenceTypeByArrayIndex(i);		// 2011-10-28 by hskim, EP4 [트리거 시스템] - 크리스탈 시스템
			m_arrGameEventofCrystal2ND[i][j].byInfluenceMask	= GetInfluenceTypeByArrayIndex(i);		// 2012-05-08 by hskim, EP4 [트리거 시스템] - 크리스탈 시스템 2차
		}
	}

	///////////////////////////////////////////////////////////////////////////////
	// 2006-08-23 by cmkwon
	util::zero(m_PCBangGameEvent, sizeof(m_PCBangGameEvent[0])*GAME_EVENT_TYPE_MAX_COUNT);
	for(i=0; i < GAME_EVENT_TYPE_MAX_COUNT; i++)
	{
		m_PCBangGameEvent[i].byInfluenceMask = INFLUENCE_TYPE_ALL_MASK;
	}
}

// 2008-05-20 by cmkwon, 모든 이벤트(HappyHoure,MotherShip,Item) 그룹 동시에 가능하게 - 사용 필요 없어진 함수 임
// ///////////////////////////////////////////////////////////////////////////////
// /// \fn			BOOL CGameEventManager::IsDoingAllGameEvent(BYTE i_byInflTy)
// /// \brief		
// /// \author		cmkwon
// /// \date		2006-04-21 ~ 2006-04-21
// /// \warning	
// ///
// /// \param		
// /// \return		
// ///////////////////////////////////////////////////////////////////////////////
// BOOL CGameEventManager::IsDoingAllGameEvent(BYTE i_byInflTy)
// {
// // 2007-10-30 by cmkwon, 세력별 해피아워 이벤트 구현 - 아래와 같이 수정함
// //	SGameEvent *pGEvent = m_arrGameEvent[3];		// 2006-08-22 by cmkwon, 모든 세력 이벤트 설정 - HappyHourEvent 는 항상 모든 세력에 적용된다.
// //	for(int i=0; i < GAME_EVENT_TYPE_MAX_COUNT; i++)
// //	{
// //		if(pGEvent[i].bUsingFlag
// //			|| pGEvent[i].bSendEventEndMessage)
// //		{
// //			return TRUE;
// //		}
// //	}
// //	
// //	switch(i_byInflTy)
// //	{
// //	case INFLUENCE_TYPE_NORMAL:
// //		pGEvent = m_arrGameEvent[0];
// //		break;
// //	case INFLUENCE_TYPE_VCN:
// //		pGEvent = m_arrGameEvent[1];
// //		break;
// //	case INFLUENCE_TYPE_ANI:
// //		pGEvent = m_arrGameEvent[2];
// //		break;
// //	default:
// //		{
// //			return FALSE;
// //		}
// //	}
// //
// //	for(i=0; i < GAME_EVENT_TYPE_MAX_COUNT; i++)
// //	{
// //		if(pGEvent[i].bUsingFlag
// //			|| pGEvent[i].bSendEventEndMessage)
// //		{
// //			return TRUE;
// //		}
// //	}
// 
// 	///////////////////////////////////////////////////////////////////////////////
// 	// 2007-10-30 by cmkwon, 세력별 해피아워 이벤트 구현 - 전체 세력 이벤트가 진행 중인지 체크
// 	SGameEvent *pArrGameEv = this->GetArrGameEventByInflType(INFLUENCE_TYPE_ALL_MASK);
// 	if(pArrGameEv)
// 	{
// 		for(int i=0; i < GAME_EVENT_TYPE_MAX_COUNT; i++)
// 		{
// 			if(pArrGameEv[i].bUsingFlag
// 				|| pArrGameEv[i].bSendEventEndMessage)
// 			{
// 				return TRUE;
// 			}
// 		}
// 	}
// 
// 	///////////////////////////////////////////////////////////////////////////////
// 	// 2007-11-01 by cmkwon, 전체 세력 이벤트 체크라면 여기에서 리턴한다.
// 	if(INFLUENCE_TYPE_ALL_MASK == i_byInflTy)
// 	{
// 		return FALSE;
// 	}
// 
// 	///////////////////////////////////////////////////////////////////////////////
// 	// 2007-10-30 by cmkwon, 세력별 해피아워 이벤트 구현 - 전체 세력 이벤트가 진행 중인지 체크
// 	pArrGameEv = this->GetArrGameEventByInflType(i_byInflTy);
// 	if(pArrGameEv)
// 	{
// 		for(int i=0; i < GAME_EVENT_TYPE_MAX_COUNT; i++)
// 		{
// 			if(pArrGameEv[i].bUsingFlag
// 				|| pArrGameEv[i].bSendEventEndMessage)
// 			{
// 				return TRUE;
// 			}
// 		}	
// 	}
// 
// 	return FALSE;
// }

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CGameEventManager::IsProgressGameEventGroup(BYTE i_byInflType, EN_GAME_EVENT_GROUP i_enGameEvGroup/*=GAME_EVENT_GROUP_HAPPYHOUR*/, SGameEvent **o_ppUsingGameEv/*=NULL*/)
/// \brief		// 2008-05-20 by cmkwon, 모든 이벤트(HappyHoure,MotherShip,Item) 그룹 동시에 가능하게 - CGameEventManager::IsProgressGameEventGroup() 추가
///										 해당 Game_Event_group 에 해당 세력 이벤트가 진행 중인지 체크
/// \author		cmkwon
/// \date		2008-05-20 ~ 2008-05-20
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CGameEventManager::IsProgressGameEventGroup(BYTE i_byInflType, EN_GAME_EVENT_GROUP i_enGameEvGroup/*=GAME_EVENT_GROUP_HAPPYHOUR*/, SGameEvent **o_ppUsingGameEv/*=NULL*/)
{
	if(o_ppUsingGameEv)
	{
		*o_ppUsingGameEv	= NULL;		// 2008-05-21 by cmkwon, 초기화
	}

	SGAEM_EVENT_PPTR ppGameEvent = this->GetGameEventGroupArray(i_enGameEvGroup);
	if(NULL == ppGameEvent)
	{
		return FALSE;
	}

	for(int i=0; i < GAME_EVENT_TYPE_MAX_COUNT; i++)
	{
		SGameEvent *pUsingGameEv = this->GetUsingGameEventPtr((EN_GAME_EVENT_TYPE)i, i_byInflType, i_enGameEvGroup);
		if(pUsingGameEv)
		{
			if(o_ppUsingGameEv)
			{
				*o_ppUsingGameEv	= pUsingGameEv;
			}
			return TRUE;
		}
	}

	return FALSE;
}

// 2008-05-20 by cmkwon, 모든 이벤트(HappyHoure,MotherShip,Item) 그룹 동시에 가능하게 - 인자 추가(, EN_GAME_EVENT_GROUP i_enGameEvGroup/*=GAME_EVENT_GROUP_HAPPYHOUR*/)
SGameEvent *CGameEventManager::GetGameEventPtr(EN_GAME_EVENT_TYPE i_eventType, BYTE i_byInflType, EN_GAME_EVENT_GROUP i_enGameEvGroup/*=GAME_EVENT_GROUP_HAPPYHOUR*/)
{
	if(i_eventType >= GAME_EVENT_TYPE_MAX_COUNT)
	{
		return NULL;
	}

// 2008-05-20 by cmkwon, 모든 이벤트(HappyHoure,MotherShip,Item) 그룹 동시에 가능하게 - 아래와 같이 수정 함
//	int nArrIdx = GetArrayIndexByInfluenceType(i_byInflType);
//	if(0 > nArrIdx)
//	{
//		return NULL;
//	}
//
//	//return &m_arrGameEvent[nArrIdx][i_eventType];

	///////////////////////////////////////////////////////////////////////////////
	// 2008-05-20 by cmkwon, 모든 이벤트(HappyHoure,MotherShip,Item) 그룹 동시에 가능하게 - 
	SGameEvent *pArrGameEvent = this->GetArrGameEventByInflType(i_byInflType, i_enGameEvGroup);
	if(NULL == pArrGameEvent)
	{
		return NULL;
	}
	return &(pArrGameEvent[i_eventType]);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			SGameEvent *CGameEventManager::GetUsingGameEventPtr(EN_GAME_EVENT_TYPE i_eventType, BYTE i_byInflType, EN_GAME_EVENT_GROUP i_enGameEvGroup/*=GAME_EVENT_GROUP_HAPPYHOUR*/)
/// \brief		// 2008-05-20 by cmkwon, 모든 이벤트(HappyHoure,MotherShip,Item) 그룹 동시에 가능하게 - 인자 추가(, EN_GAME_EVENT_GROUP i_enGameEvGroup/*=GAME_EVENT_GROUP_HAPPYHOUR*/)
/// \author		cmkwon
/// \date		2006-04-12 ~ 2006-04-12
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
SGameEvent *CGameEventManager::GetUsingGameEventPtr(EN_GAME_EVENT_TYPE i_eventType, BYTE i_byInflType, EN_GAME_EVENT_GROUP i_enGameEvGroup/*=GAME_EVENT_GROUP_HAPPYHOUR*/)
{
// 2007-10-30 by cmkwon, 세력별 해피아워 이벤트 구현 - 아래와 같이 수정
//	SGameEvent *pGEv = GetGameEventPtr(i_eventType, INFLUENCE_TYPE_ALL_MASK);
//	if(pGEv
//		&& pGEv->bUsingFlag)
//	{// 2006-04-17 by cmkwon, 모든세력 이벤트부터 체크
//		return pGEv;
//	}
//
//	pGEv = GetGameEventPtr(i_eventType, i_byInflType);
//	if(NULL == pGEv
//		|| FALSE == pGEv->bUsingFlag)
//	{
//		return NULL;
//	}

	SGameEvent *pGEv = GetGameEventPtr(i_eventType, i_byInflType, i_enGameEvGroup);	// 2008-05-20 by cmkwon, 모든 이벤트(HappyHoure,MotherShip,Item) 그룹 동시에 가능하게 - 
	if(NULL == pGEv
		|| FALSE == pGEv->bUsingFlag)
	{
		return NULL;
	}
	return pGEv;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			SGAEM_EVENT_PPTR CGameEventManager::GetGameEventGroupArray(EN_GAME_EVENT_GROUP i_enGameEvGroup/*=GAME_EVENT_GROUP_HAPPYHOUR*/)
/// \brief		// 2008-05-20 by cmkwon, 모든 이벤트(HappyHoure,MotherShip,Item) 그룹 동시에 가능하게 - CGameEventManager::GetGameEventGroupArray() 추가
/// \author		cmkwon
/// \date		2008-05-20 ~ 2008-05-20
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
SGAEM_EVENT_PPTR CGameEventManager::GetGameEventGroupArray(EN_GAME_EVENT_GROUP i_enGameEvGroup/*=GAME_EVENT_GROUP_HAPPYHOUR*/)
{
	switch(i_enGameEvGroup)
	{
	case GAME_EVENT_GROUP_HAPPYHOUR:			return m_arrGameEvent;
	case GAME_EVENT_GROUP_MOTHERSHIP:			return m_arrGameEventofMotherShip;
	case GAME_EVENT_GROUP_ITEM:					return m_arrGameEventofItem;
	case GAME_EVENT_GROUP_CRYSTAL_SYSTEM:		return m_arrGameEventofCrystal;		// 2011-10-28 by hskim, EP4 [트리거 시스템] - 크리스탈 시스템
	case GAME_EVENT_GROUP_CRYSTAL_SYSTEM_2ND:	return m_arrGameEventofCrystal2ND;	// 2012-05-08 by hskim, EP4 [트리거 시스템] - 크리스탈 시스템 2차
	}
	return NULL;
}

void CGameEventManager::OnDoMinutelyWorkGameEventTime(ATUM_DATE_TIME *pDateTime)
{
// 2007-10-30 by cmkwon, 세력별 해피아워 이벤트 구현 - 함수명을 수정하고 위치도 수정함
//	if(m_pFieldIOCP10->m_pHappyHourEventManager->GetCurrentHappyHourEvent())
//	{
//		return;
//	}
// 2007-11-26 by cmkwon, HappyHourEvent 버그수정 - 아래 for 문에서 처리됨
//	if(m_pFieldIOCP10->m_pHappyHourEventManager->GetCurrentHappyHourEventByInfluenceType(INFLUENCE_TYPE_ALL_MASK))
//	{// 2007-10-31 by cmkwon, 모든 세력 HappyHourEvent 가 GameEvent 관련 메시지는 전송 할 필요가 없다
//		return;
//	}
//

// 2008-05-20 by cmkwon, 모든 이벤트(HappyHoure,MotherShip,Item) 그룹 동시에 가능하게 - 아래와 같이 수정 함
// 	char szTemp[1024];
// 	for(int j=0; j < INFLUENCE_TYPE_COUNT; j++)
// 	{
// 		if(m_pFieldIOCP10->m_pHappyHourEventManager->GetCurrentHappyHourEventByInfluenceType(m_arrGameEvent[j][0].byInfluenceMask))
// 		{// 2007-10-31 by cmkwon, 해당 세력의 HappyHourEvent 가 진행시 GameEvent 관련 메시지는 전송 할 필요가 없다
// 			continue;
// 		}
// 
// 		for(int i=0; i < GAME_EVENT_TYPE_MAX_COUNT; i++)
// 		{
// 			if(m_arrGameEvent[j][i].bUsingFlag)
// 			{
// 				if(m_arrGameEvent[j][i].atimeEndTime <= *pDateTime)
// 				{
// 					m_arrGameEvent[j][i].bUsingFlag				= FALSE;
// 					m_arrGameEvent[j][i].bSendEventEndMessage	= TRUE;
// 				}
// 			}
// 
// 			if(GetGameEventInfoString(szTemp, NULL, (EN_GAME_EVENT_TYPE)i, FALSE, m_arrGameEvent[j][i].byInfluenceMask))
// 			{
// 				m_pFieldIOCP10->SendString128ToAllClients(STRING_128_USER_NOTICE, szTemp, TRUE, m_arrGameEvent[j][i].byInfluenceMask);
// 			}
// 		}
// 	}
	///////////////////////////////////////////////////////////////////////////////
	// 2008-05-20 by cmkwon, 모든 이벤트(HappyHoure,MotherShip,Item) 그룹 동시에 가능하게 - GAME_EVENT_GROUP_MOTHERSHIP, GAME_EVENT_GROUP_ITEM 는 유효성 체크를 해야 한다.(GAME_EVENT_GROUP_HAPPYHOUR 는 HappyHourEvent 시스템에서 처리)
	for(int i=1; i < GAME_EVENT_GROUP_MAX_COUNT; i++)
	{
		SGAEM_EVENT_PPTR ppGameEvent = GetGameEventGroupArray((EN_GAME_EVENT_GROUP)i);
		if(NULL == ppGameEvent)
		{
			continue;
		}

		for(int j=0; j < INFLUENCE_TYPE_COUNT; j++)
		{
			for(int k=0; k < GAME_EVENT_TYPE_MAX_COUNT; k++)
			{
				SGameEvent *pGameEv = &(ppGameEvent[j][k]);

				if(FALSE == this->IsProgressGameEventGroup(pGameEv->byInfluenceMask, (EN_GAME_EVENT_GROUP)i))
				{// 2008-05-20 by cmkwon, 세력-GAME_EVENT_GROUP_XXX 에 진행 중인 이벤트가 1개라도 있는지 체크
					break;
				}

				if(pGameEv->bUsingFlag
					&& pGameEv->atimeEndTime <= *pDateTime)
				{// 2008-05-20 by cmkwon, 1개라도 종료되는 것이 있다면 해당 세력-GAME_EVENT_GROUP_XXX 의 모든 이벤트는 종료 된다.
					g_pGlobal->WriteSystemLogEX(TRUE, "[Notify] CGameEventManager::OnDoMinutelyWorkGameEventTime end GameEvent !!, InfluenceType(%20s) GameEventGroup(%d)\r\n"
						, CAtumSJ::GetInfluenceTypeString(pGameEv->byInfluenceMask), i);

					this->EndGameEventAllByInfluenceType(pGameEv->byInfluenceMask, FALSE, (EN_GAME_EVENT_GROUP)i);

					// 2008-05-20 by cmkwon, 모든 이벤트(HappyHoure,MotherShip,Item) 그룹 동시에 가능하게 - 해당 세력 유저에게 전송
					INIT_MSG_WITH_BUFFER(MSG_FC_INFO_GET_HAPPY_HOUR_EVENT_INFO_OK, T_FC_INFO_GET_HAPPY_HOUR_EVENT_INFO_OK, pSHappy, SendBuf);

					// start 2011-10-28 by hskim, EP4 [트리거 시스템] - 크리스탈 시스템
					//
					/////////
					// 기존
					//pSHappy->byStateType		= (i == GAME_EVENT_GROUP_MOTHERSHIP) ? GAME_EVENT_GROUP_MOTHERSHIP_END : GAME_EVENT_GROUP_ITEM_END;

					/////////
					// 수정
					switch( i )
					{
					case GAME_EVENT_GROUP_MOTHERSHIP:
						pSHappy->byStateType = GAME_EVENT_GROUP_MOTHERSHIP_END;
						break;
					case GAME_EVENT_GROUP_ITEM:
						pSHappy->byStateType = GAME_EVENT_GROUP_ITEM_END;
						break;
					case GAME_EVENT_GROUP_CRYSTAL_SYSTEM:
						pSHappy->byStateType = GAME_EVENT_GROUP_CRYSTAL_SYSTEM_END;
						break;
					case GAME_EVENT_GROUP_CRYSTAL_SYSTEM_2ND:
						pSHappy->byStateType = GAME_EVENT_GROUP_CRYSTAL_SYSTEM_2ND_END;
						break;						
					default:
						// 이쪽으로 들어올수 없음
						break;
					}
					// end 2011-10-28 by hskim, EP4 [트리거 시스템] - 크리스탈 시스템
					pSHappy->byInfluenceType4	= pGameEv->byInfluenceMask;
					pSHappy->StartATime.SetCurrentDateTime(TRUE);
					pSHappy->EndATime			= pSHappy->StartATime;
					m_pFieldIOCP10->SendMessageToAllClients(SendBuf, MSG_SIZE(MSG_FC_INFO_GET_HAPPY_HOUR_EVENT_INFO_OK), pSHappy->byInfluenceType4);
					break;
				}
			}
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CGameEventManager::StartGameEvent(EN_GAME_EVENT_TYPE i_eventType, float i_fRate, int i_nLimitMinute, int i_nMinLevel, int i_nMaxLevel, BYTE i_byInflMask/*=INFLUENCE_TYPE_ALL_MASK*/, EN_GAME_EVENT_GROUP i_enGameEvGroup/*=GAME_EVENT_GROUP_HAPPYHOUR*/)
/// \brief		// 2008-03-14 by dhjin, Level별 해피아워 이벤트 구현 - MinLevel, MaxLevel 추가
///				// 2008-05-20 by cmkwon, 모든 이벤트(HappyHoure,MotherShip,Item) 그룹 동시에 가능하게 - 인자 추가(, EN_GAME_EVENT_GROUP i_enGameEvGroup/*=GAME_EVENT_GROUP_HAPPYHOUR*/)
/// \author		cmkwon
/// \date		2006-04-12 ~ 2006-04-12
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CGameEventManager::StartGameEvent(EN_GAME_EVENT_TYPE i_eventType, float i_fRate, int i_nLimitMinute, int i_nMinLevel, int i_nMaxLevel, BYTE i_byInflMask/*=INFLUENCE_TYPE_ALL_MASK*/, EN_GAME_EVENT_GROUP i_enGameEvGroup/*=GAME_EVENT_GROUP_HAPPYHOUR*/, MapIndex_t i_Mapindex /*= 0*/)		// 2012-06-16 by hskim, [트리거 시스템] - 크리스탈 시스템 2차 : 특정 맵에서만 해피아워 이벤트 적용
{
	if(0.0f >= i_fRate){				return FALSE;}

	SGameEvent *pGEvent = GetGameEventPtr(i_eventType, i_byInflMask, i_enGameEvGroup);	// 2008-05-20 by cmkwon, 모든 이벤트(HappyHoure,MotherShip,Item) 그룹 동시에 가능하게 - 
	if(NULL == pGEvent)
	{
		return FALSE;
	}

// 2007-10-30 by cmkwon, 세력별 해피아워 이벤트 구현 - 무조건 다시 설정한다
//	if(INFLUENCE_TYPE_ALL_MASK == i_byInflMask)
//	{// 2006-04-17 by cmkwon, 전체이벤트 시작시 세력별 이벤트는 모두 종료
//		EndGameEvent(i_eventType, FALSE, INFLUENCE_TYPE_NORMAL);
//		EndGameEvent(i_eventType, FALSE, INFLUENCE_TYPE_VCN);
//		EndGameEvent(i_eventType, FALSE, INFLUENCE_TYPE_ANI);
//	}
//	else
//	{// 2006-04-17 by cmkwon,
//
//		SGameEvent *pAllGEvent = GetGameEventPtr(i_eventType, INFLUENCE_TYPE_ALL_MASK);
//		if(pAllGEvent
//			&& pAllGEvent->bUsingFlag)
//		{// 2006-04-17 by cmkwon, 이미 전체 이벤트가 진행중이다.
//			return FALSE;
//		}
//	}
	
	pGEvent->bUsingFlag			= TRUE;
	pGEvent->atimeStartedTime.SetCurrentDateTime(TRUE);
	pGEvent->atimeEndTime		= pGEvent->atimeStartedTime;
	pGEvent->atimeEndTime.AddDateTime(0, 0, 0, 0, i_nLimitMinute, 0);
	pGEvent->dwTimeLimitMinute	= i_nLimitMinute;
	pGEvent->fApplyRate			= i_fRate;
	pGEvent->MinLevel			= i_nMinLevel;			// 2008-03-14 by dhjin, Level별 해피아워 이벤트 구현 -
	pGEvent->MaxLevel			= i_nMaxLevel;			// 2008-03-14 by dhjin, Level별 해피아워 이벤트 구현 -
	pGEvent->MapIndex			= i_Mapindex;			// 2012-06-16 by hskim, [트리거 시스템] - 크리스탈 시스템 2차 : 특정 맵에서만 해피아워 이벤트 적용

// 2008-05-20 by cmkwon, 모든 이벤트(HappyHoure,MotherShip,Item) 그룹 동시에 가능하게 - 아래와 같이 수정 함
// 	// 2007-11-03 by cmkwon, 시스템로그 추가 - CGameEventManager::StartGameEvent()
// 	float		fPCBangPlusRate	= 0.0f;
// 	SGameEvent	*SPCBangGameEv	= GetPCBangUsingGameEventPtr(i_eventType);
// 	if(SPCBangGameEv)
// 	{
// 		fPCBangPlusRate = SPCBangGameEv->fApplyRate;
// 	}
// 	g_pGlobal->WriteSystemLogEX(TRUE, "[Notify] CGameEventManager::StartGameEvent !!, InfluenceType(%20s) EventType(%15s) InfluenceTotalPlusRate(%6.2f) PCBangPlusRate(%6.2f) TimeMinute(%d) MinLevel(%d) MaxLevel(%d)\r\n"
// 		, CAtumSJ::GetInfluenceTypeString(pGEvent->byInfluenceMask), CGameEventManager::GetGameEventTypeString(i_eventType)
// 		, this->GetTotalEventPlusRate(i_eventType, pGEvent->byInfluenceMask, FALSE, CHARACTER_MAX_LEVEL), fPCBangPlusRate, i_nLimitMinute, i_nMinLevel, i_nMaxLevel);	// 2008-03-14 by dhjin, Level별 해피아워 이벤트 구현 -
	// 2008-05-20 by cmkwon, 모든 이벤트(HappyHoure,MotherShip,Item) 그룹 동시에 가능하게 - 
 	g_pGlobal->WriteSystemLogEX(TRUE, "[Notify] CGameEventManager::StartGameEvent !!, InfluenceType(%20s) EventType(%15s) PlusRate(%6.2f) TimeMinute(%d) MinLevel(%d) MaxLevel(%d) GameEventGroup(%d) MapIndex(%d)\r\n"		// 2012-06-16 by hskim, [트리거 시스템] - 크리스탈 시스템 2차 : 특정 맵에서만 해피아워 이벤트 적용
 		, CAtumSJ::GetInfluenceTypeString(pGEvent->byInfluenceMask), CGameEventManager::GetGameEventTypeString(i_eventType)
 		, i_fRate, i_nLimitMinute, i_nMinLevel, i_nMaxLevel, i_enGameEvGroup, i_Mapindex);

	return TRUE;
}

// 2008-05-20 by cmkwon, 모든 이벤트(HappyHoure,MotherShip,Item) 그룹 동시에 가능하게 - CGameEventManager::EndAllGameEvent() 인자 추가
void CGameEventManager::EndAllGameEvent(BOOL i_bEndNotify/*=FALSE*/, EN_GAME_EVENT_GROUP i_enGameEvGroup/*=GAME_EVENT_GROUP_HAPPYHOUR*/)
{

// 2008-05-20 by cmkwon, 모든 이벤트(HappyHoure,MotherShip,Item) 그룹 동시에 가능하게 - 아래와 같이 수정 함
// 	for(int j=0; j < INFLUENCE_TYPE_COUNT; j++)
// 	{
// 		for(int i=0; i < GAME_EVENT_TYPE_MAX_COUNT; i++)
// 		{
// 			m_arrGameEvent[j][i].bUsingFlag				= FALSE;
// 			m_arrGameEvent[j][i].bSendEventEndMessage	= i_bEndNotify;
// 		}
// 	}
	// 2008-05-20 by cmkwon, 모든 이벤트(HappyHoure,MotherShip,Item) 그룹 동시에 가능하게 - 해당 GAME_EVENT_GROUP_XXX 만 초기화 한다.
	SGAEM_EVENT_PPTR ppGameEvent = this->GetGameEventGroupArray(i_enGameEvGroup);
	if(NULL == ppGameEvent)
	{
		return;
	}
	for(int j=0; j < INFLUENCE_TYPE_COUNT; j++)
	{
		for(int i=0; i < GAME_EVENT_TYPE_MAX_COUNT; i++)
		{
			ppGameEvent[j][i].bUsingFlag			= FALSE;
			ppGameEvent[j][i].bSendEventEndMessage	= i_bEndNotify;
		}
	}
}

// 2008-05-20 by cmkwon, 모든 이벤트(HappyHoure,MotherShip,Item) 그룹 동시에 가능하게 - 인자 수정(, EN_GAME_EVENT_GROUP i_enGameEvGroup/*=GAME_EVENT_GROUP_HAPPYHOUR*/)
BOOL CGameEventManager::EndGameEvent(EN_GAME_EVENT_TYPE i_eventType, BOOL i_bSendEndMsg, BYTE i_byInflMask/*=INFLUENCE_TYPE_ALL_MASK*/, EN_GAME_EVENT_GROUP i_enGameEvGroup/*=GAME_EVENT_GROUP_HAPPYHOUR*/)
{
	SGameEvent *pGEvent = GetGameEventPtr(i_eventType, i_byInflMask, i_enGameEvGroup);		// 2008-05-20 by cmkwon, 모든 이벤트(HappyHoure,MotherShip,Item) 그룹 동시에 가능하게 - 
	if(NULL == pGEvent
		|| FALSE == pGEvent->bUsingFlag)
	{
		return FALSE;
	}

// 2007-10-30 by cmkwon, 세력별 해피아워 이벤트 구현 - 해당 세력의 이벤트만 종료 처리한다.
//	if(INFLUENCE_TYPE_ALL_MASK == i_byInflMask)
//	{// 2006-04-17 by cmkwon, 전제이벤트 종료시 모든 세력별 이벤트도 종료
//		EndGameEvent(i_eventType, FALSE, INFLUENCE_TYPE_NORMAL);
//		EndGameEvent(i_eventType, FALSE, INFLUENCE_TYPE_VCN);
//		EndGameEvent(i_eventType, FALSE, INFLUENCE_TYPE_ANI);
//	}

	pGEvent->bUsingFlag				= FALSE;
	pGEvent->bSendEventEndMessage	= i_bSendEndMsg;
	return TRUE;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CGameEventManager::GetGameEventInfoString(char *o_szInfo
///											   , BYTE *o_pbyInflMask
///											   , EN_GAME_EVENT_TYPE i_eventType
///											   , BOOL i_bAdminFlag/*=FALSE*/
///											   , BYTE i_byInflType/*=INFLUENCE_TYPE_ALL_MASK*/)
/// \brief		
///				// 2006-04-13 by cmkwon, 파라미터 두개 추가 - o_pbyInflMask, i_byInflType
/// \author		cmkwon
/// \date		2006-04-13 ~ 2006-04-13
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CGameEventManager::GetGameEventInfoString(char *o_szInfo
											   , BYTE *o_pbyInflMask
											   , EN_GAME_EVENT_TYPE i_eventType
											   , BOOL i_bAdminFlag/*=FALSE*/
											   , BYTE i_byInflType/*=INFLUENCE_TYPE_ALL_MASK*/)
{
	SGameEvent *pGEvent = GetGameEventPtr(i_eventType, i_byInflType);
	if(NULL == pGEvent)
	{
		return FALSE;
	}
	if(o_pbyInflMask)
	{// 2006-04-13 by cmkwon, 이벤트 허용
		*o_pbyInflMask = pGEvent->byInfluenceMask;
	}
	
	if(FALSE == pGEvent->bUsingFlag)
	{
		if(FALSE == pGEvent->bSendEventEndMessage)
		{
			return FALSE;
		}

		pGEvent->bSendEventEndMessage = FALSE;
		sprintf(o_szInfo, STRMSG_S_F2NOTIFY_0148
			, GetGameEventTypeString(i_eventType));
		return TRUE;
	}

	ATUM_DATE_TIME atimeCur;
	atimeCur.SetCurrentDateTime(TRUE);
	int nRemainMinute = max(0, pGEvent->atimeEndTime.GetTimeDiffTimeInMinutes(atimeCur));

	if(FALSE == i_bAdminFlag
		&& nRemainMinute > 5
		&& nRemainMinute%5 != 0)
	{
		return FALSE;
	}

	sprintf(o_szInfo, STRMSG_S_F2NOTIFY_0149
		, GetGameEventTypeString(i_eventType), pGEvent->fApplyRate, nRemainMinute);	
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CGameEventManager::SenGameEventGroupMotherShip8ItemInfo(CFieldIOCPSocket *i_pToFISock, BYTE i_byInflTy)
/// \brief		// 2008-05-20 by cmkwon, 모든 이벤트(HappyHoure,MotherShip,Item) 그룹 동시에 가능하게 - CGameEventManager::SenGameEventGroupMotherShip8ItemInfo() 추가
/// \author		cmkwon
/// \date		2008-05-21 ~ 2008-05-21
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CGameEventManager::SenGameEventGroupMotherShip8ItemInfo(CFieldIOCPSocket *i_pToFISock, BYTE i_byInflTy)
{
	// 2008-05-20 by cmkwon, 모든 이벤트(HappyHoure,MotherShip,Item) 그룹 동시에 가능하게 - 
	INIT_MSG_WITH_BUFFER(MSG_FC_INFO_GET_HAPPY_HOUR_EVENT_INFO_OK, T_FC_INFO_GET_HAPPY_HOUR_EVENT_INFO_OK, pSHappy, SendBuf);
	pSHappy->byInfluenceType4	= i_byInflTy;
	
	SGameEvent *pUsingGameEv	= NULL;
	EN_GAME_EVENT_GROUP	enGameEvGroup;
	// 2008-05-21 by cmkwon, GAME_EVENT_GROUP_MOTHERSHIP 정보 전송
	enGameEvGroup				= GAME_EVENT_GROUP_MOTHERSHIP;
	pSHappy->byStateType		= GAME_EVENT_GROUP_MOTHERSHIP_STARTING;
	if(this->IsProgressGameEventGroup(i_byInflTy, enGameEvGroup, &pUsingGameEv)
		&& pUsingGameEv)
	{
		SGameEvent *pArrGameEvOfInfo = GetArrGameEventByInflType(i_byInflTy, enGameEvGroup);
		pSHappy->StartATime			= pUsingGameEv->atimeStartedTime;
		pSHappy->EndATime			= pUsingGameEv->atimeEndTime;
		pSHappy->fEXPRate2			= pArrGameEvOfInfo[GAME_EVENT_TYPE_EXP].fApplyRate;
		pSHappy->fSPIRate2			= pArrGameEvOfInfo[GAME_EVENT_TYPE_SPI].fApplyRate;
		pSHappy->fEXPRepairRate2	= pArrGameEvOfInfo[GAME_EVENT_TYPE_EXP_NO_DOWN].fApplyRate;
		pSHappy->fDropItemRate2		= pArrGameEvOfInfo[GAME_EVENT_TYPE_DROP_ITEM].fApplyRate;
		pSHappy->fDropRareRate2		= pArrGameEvOfInfo[GAME_EVENT_TYPE_DROP_REAR_ITEM].fApplyRate;
		pSHappy->fWarPointRate2		= pArrGameEvOfInfo[GAME_EVENT_TYPE_WARPOINT].fApplyRate;
		i_pToFISock->SendAddData(SendBuf, MSG_SIZE(MSG_FC_INFO_GET_HAPPY_HOUR_EVENT_INFO_OK));
	}

	// 2008-05-21 by cmkwon, GAME_EVENT_GROUP_ITEM 정보 전송
	enGameEvGroup				= GAME_EVENT_GROUP_ITEM;
	pSHappy->byStateType		= GAME_EVENT_GROUP_ITEM_STARTING;
	if(this->IsProgressGameEventGroup(i_byInflTy, enGameEvGroup, &pUsingGameEv)
		&& pUsingGameEv)
	{
		SGameEvent *pArrGameEvOfInfo = GetArrGameEventByInflType(i_byInflTy, enGameEvGroup);
		pSHappy->StartATime			= pUsingGameEv->atimeStartedTime;
		pSHappy->EndATime			= pUsingGameEv->atimeEndTime;
		pSHappy->fEXPRate2			= pArrGameEvOfInfo[GAME_EVENT_TYPE_EXP].fApplyRate;
		pSHappy->fSPIRate2			= pArrGameEvOfInfo[GAME_EVENT_TYPE_SPI].fApplyRate;
		pSHappy->fEXPRepairRate2	= pArrGameEvOfInfo[GAME_EVENT_TYPE_EXP_NO_DOWN].fApplyRate;
		pSHappy->fDropItemRate2		= pArrGameEvOfInfo[GAME_EVENT_TYPE_DROP_ITEM].fApplyRate;
		pSHappy->fDropRareRate2		= pArrGameEvOfInfo[GAME_EVENT_TYPE_DROP_REAR_ITEM].fApplyRate;
		pSHappy->fWarPointRate2		= pArrGameEvOfInfo[GAME_EVENT_TYPE_WARPOINT].fApplyRate;
		i_pToFISock->SendAddData(SendBuf, MSG_SIZE(MSG_FC_INFO_GET_HAPPY_HOUR_EVENT_INFO_OK));
	}

	// start 2011-10-28 by hskim, EP4 [트리거 시스템] - 크리스탈 시스템
	enGameEvGroup				= GAME_EVENT_GROUP_CRYSTAL_SYSTEM;
	pSHappy->byStateType		= GAME_EVENT_GROUP_CRYSTAL_SYSTEM_STARTING;
	if(this->IsProgressGameEventGroup(i_byInflTy, enGameEvGroup, &pUsingGameEv)
		&& pUsingGameEv)
	{
		SGameEvent *pArrGameEvOfInfo = GetArrGameEventByInflType(i_byInflTy, enGameEvGroup);
		pSHappy->StartATime			= pUsingGameEv->atimeStartedTime;
		pSHappy->EndATime			= pUsingGameEv->atimeEndTime;
		pSHappy->fEXPRate2			= pArrGameEvOfInfo[GAME_EVENT_TYPE_EXP].fApplyRate;
		pSHappy->fSPIRate2			= pArrGameEvOfInfo[GAME_EVENT_TYPE_SPI].fApplyRate;
		pSHappy->fEXPRepairRate2	= pArrGameEvOfInfo[GAME_EVENT_TYPE_EXP_NO_DOWN].fApplyRate;
		pSHappy->fDropItemRate2		= pArrGameEvOfInfo[GAME_EVENT_TYPE_DROP_ITEM].fApplyRate;
		pSHappy->fDropRareRate2		= pArrGameEvOfInfo[GAME_EVENT_TYPE_DROP_REAR_ITEM].fApplyRate;
		pSHappy->fWarPointRate2		= pArrGameEvOfInfo[GAME_EVENT_TYPE_WARPOINT].fApplyRate;
		i_pToFISock->SendAddData(SendBuf, MSG_SIZE(MSG_FC_INFO_GET_HAPPY_HOUR_EVENT_INFO_OK));
	}
	// end 2011-10-28 by hskim, EP4 [트리거 시스템] - 크리스탈 시스템

	// start 2012-05-08 by hskim, EP4 [트리거 시스템] - 크리스탈 시스템 2차
	enGameEvGroup				= GAME_EVENT_GROUP_CRYSTAL_SYSTEM_2ND;
	pSHappy->byStateType		= GAME_EVENT_GROUP_CRYSTAL_SYSTEM_2ND_STARTING;
	if(this->IsProgressGameEventGroup(i_byInflTy, enGameEvGroup, &pUsingGameEv)
		&& pUsingGameEv)
	{
		SGameEvent *pArrGameEvOfInfo = GetArrGameEventByInflType(i_byInflTy, enGameEvGroup);
		pSHappy->StartATime			= pUsingGameEv->atimeStartedTime;
		pSHappy->EndATime			= pUsingGameEv->atimeEndTime;
		pSHappy->fEXPRate2			= pArrGameEvOfInfo[GAME_EVENT_TYPE_EXP].fApplyRate;
		pSHappy->fSPIRate2			= pArrGameEvOfInfo[GAME_EVENT_TYPE_SPI].fApplyRate;
		pSHappy->fEXPRepairRate2	= pArrGameEvOfInfo[GAME_EVENT_TYPE_EXP_NO_DOWN].fApplyRate;
		pSHappy->fDropItemRate2		= pArrGameEvOfInfo[GAME_EVENT_TYPE_DROP_ITEM].fApplyRate;
		pSHappy->fDropRareRate2		= pArrGameEvOfInfo[GAME_EVENT_TYPE_DROP_REAR_ITEM].fApplyRate;
		pSHappy->fWarPointRate2		= pArrGameEvOfInfo[GAME_EVENT_TYPE_WARPOINT].fApplyRate;
		i_pToFISock->SendAddData(SendBuf, MSG_SIZE(MSG_FC_INFO_GET_HAPPY_HOUR_EVENT_INFO_OK));
	}
	// end 2012-05-08 by hskim, EP4 [트리거 시스템] - 크리스탈 시스템 2차
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			SGameEvent *CGameEventManager::GetPCBangGameEventPtr(EN_GAME_EVENT_TYPE i_eventType)
/// \brief		
/// \author		cmkwon
/// \date		2006-08-23 ~ 2006-08-23
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
SGameEvent *CGameEventManager::GetPCBangGameEventPtr(EN_GAME_EVENT_TYPE i_eventType)
{
	if(i_eventType >= GAME_EVENT_TYPE_MAX_COUNT)
	{
		return NULL;
	}

	return &m_PCBangGameEvent[i_eventType];
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			SGameEvent *CGameEventManager::GetPCBangUsingGameEventPtr(EN_GAME_EVENT_TYPE i_eventType)
/// \brief		
/// \author		cmkwon
/// \date		2006-08-23 ~ 2006-08-23
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
SGameEvent *CGameEventManager::GetPCBangUsingGameEventPtr(EN_GAME_EVENT_TYPE i_eventType)
{
	SGameEvent *pPCBangEv = GetPCBangGameEventPtr(i_eventType);
	if(NULL == pPCBangEv
		|| FALSE == pPCBangEv->bUsingFlag)
	{
		return NULL;
	}

	return pPCBangEv;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CGameEventManager::StartPCBangGameEvent(EN_GAME_EVENT_TYPE i_eventType, float i_fRate, int i_nLimitMinute)
/// \brief		
/// \author		cmkwon
/// \date		2006-08-23 ~ 2006-08-23
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CGameEventManager::StartPCBangGameEvent(EN_GAME_EVENT_TYPE i_eventType, float i_fRate, int i_nLimitMinute)
{
	if(0.0f >= i_fRate){				return FALSE;}

	SGameEvent *pGEvent = GetPCBangGameEventPtr(i_eventType);
	if(NULL == pGEvent)
	{
		return FALSE;
	}

	pGEvent->bUsingFlag			= TRUE;
	pGEvent->atimeStartedTime.SetCurrentDateTime(TRUE);
	pGEvent->atimeEndTime		= pGEvent->atimeStartedTime;
	pGEvent->atimeEndTime.AddDateTime(0, 0, 0, 0, i_nLimitMinute, 0);
	pGEvent->dwTimeLimitMinute	= i_nLimitMinute;
	pGEvent->fApplyRate			= i_fRate;
	pGEvent->MinLevel			= 1;								// 2008-03-14 by dhjin, Level별 해피아워 이벤트 구현 - 
	pGEvent->MaxLevel			= CHARACTER_MAX_LEVEL;				// 2008-03-14 by dhjin, Level별 해피아워 이벤트 구현 - 

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CGameEventManager::EndPCBangGameEvent(void)
/// \brief		
/// \author		cmkwon
/// \date		2006-08-23 ~ 2006-08-23
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CGameEventManager::EndPCBangGameEvent(void)
{
	///////////////////////////////////////////////////////////////////////////////
	// 2006-08-23 by cmkwon
	for(int i=0; i < GAME_EVENT_TYPE_MAX_COUNT; i++)
	{
		m_PCBangGameEvent[i].bUsingFlag				= FALSE;
		m_PCBangGameEvent[i].bSendEventEndMessage	= FALSE;		
	}
	return TRUE;
}

// 2007-10-30 by cmkwon, 세력별 해피아워 이벤트 구현 - 시스템 변경으로 함수명과 함수 수정함
/////////////////////////////////////////////////////////////////////////////////
///// \fn			float CGameEventManager::GetExperienceRate(BYTE i_byInflType, BOOL i_bPCBangClient);
///// \brief		경치 이벤트시에 경험치 배수 가져오는 함수
///// \author		cmkwon
///// \date		2004-12-03 ~ 2004-12-03
///// \warning	
/////
///// \param		
///// \return		
/////////////////////////////////////////////////////////////////////////////////
//float CGameEventManager::GetExperienceRate(BYTE i_byInflType, BOOL i_bPCBangClient)
//{
//	float fRetExpRate = ms_fDefaultExpRate;
//	SGameEvent *pGEvent = GetUsingGameEventPtr(GAME_EVENT_TYPE_EXP, i_byInflType);
//	if(pGEvent)
//	{
//		fRetExpRate		= pGEvent->fApplyRate;
//	}
//
//	if(i_bPCBangClient)
//	{
//		pGEvent = GetPCBangUsingGameEventPtr(GAME_EVENT_TYPE_EXP);
//		if(pGEvent)
//		{
//			fRetExpRate += pGEvent->fApplyRate;
//		}
//	}
//	return fRetExpRate;
//}
//
//
/////////////////////////////////////////////////////////////////////////////////
///// \fn			float CGameEventManager::GetSPIRate(BYTE i_byInflType, BOOL i_bPCBangClient)
///// \brief		스피 이벤트
///// \author		cmkwon
///// \date		2004-12-03 ~ 2004-12-03
///// \warning	
/////
///// \param		
///// \return		
/////////////////////////////////////////////////////////////////////////////////
//float CGameEventManager::GetSPIRate(BYTE i_byInflType, BOOL i_bPCBangClient)
//{
//	float fRetSPIRate = ms_fDefaultSPIRate;
//	SGameEvent *pGEvent = GetUsingGameEventPtr(GAME_EVENT_TYPE_SPI, i_byInflType);
//	if(pGEvent)
//	{
//		fRetSPIRate		= pGEvent->fApplyRate;
//	}
//
//	if(i_bPCBangClient)
//	{
//		pGEvent = GetPCBangUsingGameEventPtr(GAME_EVENT_TYPE_SPI);
//		if(pGEvent)
//		{
//			fRetSPIRate	+= pGEvent->fApplyRate;
//		}
//	}
//
//	return fRetSPIRate;
//}
//
//
/////////////////////////////////////////////////////////////////////////////////
///// \fn			BOOL CGameEventManager::IsExperienceRepair(BYTE i_byInflType, BOOL i_bPCBangClient)
///// \brief		경치복구 이벤트
///// \author		cmkwon
///// \date		2004-12-03 ~ 2004-12-03
///// \warning	
/////
///// \param		
///// \return		
/////////////////////////////////////////////////////////////////////////////////
//float CGameEventManager::GetExpRepairRate(BYTE i_byInflType, BOOL i_bPCBangClient)
//{
//	float fRetRate = ms_fDefaultExpRepareRate;
//	SGameEvent *pGEvent = GetUsingGameEventPtr(GAME_EVENT_TYPE_EXP_NO_DOWN, i_byInflType);
//	if(pGEvent)
//	{
//		fRetRate = pGEvent->fApplyRate;
//	}
//
//	if(i_bPCBangClient)
//	{
//		pGEvent			= GetPCBangUsingGameEventPtr(GAME_EVENT_TYPE_EXP_NO_DOWN);
//		if(pGEvent)
//		{
//			fRetRate	+= pGEvent->fApplyRate; 
//		}
//	}
//
//	return fRetRate;
//}
//
//
//float CGameEventManager::GetDropItemRate(BYTE i_byInflType, BOOL i_bPCBangClient)
//{
//	float fRetDropItemRate = ms_fDefaultDropItemRate;
//	SGameEvent *pGEvent = GetUsingGameEventPtr(GAME_EVENT_TYPE_DROP_ITEM, i_byInflType);
//	if(pGEvent)
//	{
//		fRetDropItemRate = pGEvent->fApplyRate;
//	}
//
//	if(i_bPCBangClient)
//	{
//		pGEvent			= GetPCBangUsingGameEventPtr(GAME_EVENT_TYPE_DROP_ITEM);
//		if(pGEvent)
//		{
//			fRetDropItemRate += pGEvent->fApplyRate;
//		}
//	}
//
//	return fRetDropItemRate;
//}
//
//float CGameEventManager::GetDropRareItemRate(BYTE i_byInflType, BOOL i_bPCBangClient)
//{
//	float fRetDropRearItemRate = ms_fDefaultDropRareItemRate;
//	SGameEvent *pGEvent = GetUsingGameEventPtr(GAME_EVENT_TYPE_DROP_REAR_ITEM, i_byInflType);
//	if(pGEvent)
//	{
//		fRetDropRearItemRate = pGEvent->fApplyRate;
//	}
//
//	if(i_bPCBangClient)
//	{
//		pGEvent			= GetPCBangUsingGameEventPtr(GAME_EVENT_TYPE_DROP_REAR_ITEM);
//		if(pGEvent)
//		{
//			fRetDropRearItemRate += pGEvent->fApplyRate;
//		}
//	}
//
//	return fRetDropRearItemRate;
//}
//
/////////////////////////////////////////////////////////////////////////////////
///// \fn			float CGameEventManager::GetWarPointRate(BYTE i_byInflType, BOOL i_bPCBangClient)
///// \brief		워포인트 이벤트 관련 추가
///// \author		dhjin
///// \date		2007-06-26 ~ 2007-06-26
///// \warning	
/////
///// \param		
///// \return		
/////////////////////////////////////////////////////////////////////////////////
//float CGameEventManager::GetWarPointRate(BYTE i_byInflType, BOOL i_bPCBangClient)
//{
//	float fRetWarPointRate = ms_fDefaultWarPointRate;
//	SGameEvent *pGEvent = GetUsingGameEventPtr(GAME_EVENT_TYPE_WARPOINT, i_byInflType);
//	if(pGEvent)
//	{
//		fRetWarPointRate = pGEvent->fApplyRate;
//	}
//
//	if(i_bPCBangClient)
//	{
//		pGEvent			= GetPCBangUsingGameEventPtr(GAME_EVENT_TYPE_WARPOINT);
//		if(pGEvent)
//		{
//			fRetWarPointRate += pGEvent->fApplyRate;
//		}
//	}
//
//	return fRetWarPointRate;
//}

///////////////////////////////////////////////////////////////////////////////
/// \fn			SGameEvent *CGameEventManager::GetArrGameEventByInflType(BYTE i_byInflType, EN_GAME_EVENT_GROUP i_enGameEvGroup/*=GAME_EVENT_GROUP_HAPPYHOUR*/)
/// \brief		// 2007-10-30 by cmkwon, 세력별 해피아워 이벤트 구현 - CGameEventManager::GetArrGameEventByInflType() 함수 추가함
///				// 2008-05-20 by cmkwon, 모든 이벤트(HappyHoure,MotherShip,Item) 그룹 동시에 가능하게 - 인자 추가(, EN_GAME_EVENT_GROUP i_enGameEvGroup/*=GAME_EVENT_GROUP_HAPPYHOUR*/)
/// \author		cmkwon
/// \date		2007-10-30 ~ 2007-10-30
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
SGameEvent *CGameEventManager::GetArrGameEventByInflType(BYTE i_byInflType, EN_GAME_EVENT_GROUP i_enGameEvGroup/*=GAME_EVENT_GROUP_HAPPYHOUR*/)
{
	int nArrIdx = GetArrayIndexByInfluenceType(i_byInflType);
	if(0 > nArrIdx)
	{
		return NULL;
	}


	// 2008-05-20 by cmkwon, 모든 이벤트(HappyHoure,MotherShip,Item) 그룹 동시에 가능하게 - 아래와 같이 수정 함
	//return m_arrGameEvent[nArrIdx];

	///////////////////////////////////////////////////////////////////////////////
	// 2008-05-20 by cmkwon, 모든 이벤트(HappyHoure,MotherShip,Item) 그룹 동시에 가능하게 - 
	SGAEM_EVENT_PPTR ppGameEvent = this->GetGameEventGroupArray(i_enGameEvGroup);
	if(NULL == ppGameEvent)
	{
		return NULL;
	}
	return ppGameEvent[nArrIdx];
}

// 2008-03-14 by dhjin, Level별 해피아워 이벤트 구현 - 아래의 CGameEventManager::GetTotalEventPlusRate() 함수로 대체함
// ///////////////////////////////////////////////////////////////////////////////
// /// \fn			float CGameEventManager::GetTotalEventPlusRate(EN_GAME_EVENT_TYPE i_eventType, BYTE i_byInflType, BOOL i_bPCBangClient)
// /// \brief		// 2007-10-30 by cmkwon, 세력별 해피아워 이벤트 구현 - 
// /// \author		cmkwon
// /// \date		2007-10-30 ~ 2007-10-30
// /// \warning	
// ///
// /// \param		
// /// \return		
// ///////////////////////////////////////////////////////////////////////////////
// float CGameEventManager::GetTotalEventPlusRate(EN_GAME_EVENT_TYPE i_eventType, BYTE i_byInflType, BOOL i_bPCBangClient)
// {
// 	float fRetPlusRate = 0.0f;
// 
// 	///////////////////////////////////////////////////////////////////////////////
// 	// 2007-10-30 by cmkwon, 자신의 세력 이벤트 Plus Rate 더하기
// 	SGameEvent *pGameEv = GetUsingGameEventPtr(i_eventType, i_byInflType);
// 	if(pGameEv)
// 	{
// 		fRetPlusRate += pGameEv->fApplyRate;
// 	}
// 
// 	if(INFLUENCE_TYPE_ALL_MASK != i_byInflType)
// 	{
// 		///////////////////////////////////////////////////////////////////////////////
// 		// 2007-10-30 by cmkwon, 모든 세력 이벤트 Plus Rate 더하기
// 		pGameEv = GetUsingGameEventPtr(i_eventType, INFLUENCE_TYPE_ALL_MASK);
// 		if(pGameEv)
// 		{
// 			fRetPlusRate += pGameEv->fApplyRate;
// 		}
// 	}
// 
// 	///////////////////////////////////////////////////////////////////////////////
// 	// 2007-10-30 by cmkwon, PCBang 이벤트 Plus Rate 더하기
// 	if(i_bPCBangClient)
// 	{
// 		pGameEv = GetPCBangUsingGameEventPtr(i_eventType);
// 		if(pGameEv)
// 		{
// 			fRetPlusRate += pGameEv->fApplyRate;
// 		}
// 	}
// 
// 	return fRetPlusRate;
// }

///////////////////////////////////////////////////////////////////////////////
/// \fn			float CGameEventManager::GetTotalEventPlusRate(EN_GAME_EVENT_TYPE i_eventType, BYTE i_byInflType, BOOL i_bPCBangClient, INT i_nLevel)
/// \brief		// 2008-03-14 by dhjin, Level별 해피아워 이벤트 구현 -	
/// \author		dhjin
/// \date		2008-03-14 ~ 2008-03-14
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
float CGameEventManager::GetTotalEventPlusRate(EN_GAME_EVENT_TYPE i_eventType, BYTE i_byInflType, BOOL i_bPCBangClient, INT i_nLevel, MapIndex_t i_MapIndex/* = 0*/)		// 2012-06-16 by hskim, [트리거 시스템] - 크리스탈 시스템 2차 : 특정 맵에서만 해피아워 이벤트 적용
{
	float fRetPlusRate = 0.0f;
	
	// 2008-05-20 by cmkwon, 모든 이벤트(HappyHoure,MotherShip,Item) 그룹 동시에 가능하게 - 전체 세력이면 그냥 리턴
	if(INFLUENCE_TYPE_ALL_MASK == i_byInflType)
	{
		return fRetPlusRate;
	}

	///////////////////////////////////////////////////////////////////////////////
	// 2007-10-30 by cmkwon, 1. HappyHourEvent - 모든세력, 자신의세력 이벤트 추가하기 
#ifdef _ARENA_WP_HH
	SGameEvent *pGameEv;
	if (!g_pFieldGlobal->IsArenaServer() && i_eventType == GAME_EVENT_TYPE_WARPOINT)
	{
	}
	else
	{
		pGameEv = GetUsingGameEventPtr(i_eventType, INFLUENCE_TYPE_ALL_MASK);
		if (pGameEv
			&& util::in_range(pGameEv->MinLevel, i_nLevel, pGameEv->MaxLevel))			// 2008-03-14 by dhjin, Level별 해피아워 이벤트 구현 -
		{
			fRetPlusRate += pGameEv->fApplyRate;
		}
		pGameEv = GetUsingGameEventPtr(i_eventType, i_byInflType);
		if (pGameEv
			&& util::in_range(pGameEv->MinLevel, i_nLevel, pGameEv->MaxLevel))			// 2008-03-14 by dhjin, Level별 해피아워 이벤트 구현 -
		{
			fRetPlusRate += pGameEv->fApplyRate;
		}
	}
#else
		SGameEvent *pGameEv = GetUsingGameEventPtr(i_eventType, INFLUENCE_TYPE_ALL_MASK);
		if (pGameEv
			&& util::in_range(pGameEv->MinLevel, i_nLevel, pGameEv->MaxLevel))			// 2008-03-14 by dhjin, Level별 해피아워 이벤트 구현 -
		{
			fRetPlusRate += pGameEv->fApplyRate;
		}
		pGameEv = GetUsingGameEventPtr(i_eventType, i_byInflType);
		if (pGameEv
			&& util::in_range(pGameEv->MinLevel, i_nLevel, pGameEv->MaxLevel))			// 2008-03-14 by dhjin, Level별 해피아워 이벤트 구현 -
		{
			fRetPlusRate += pGameEv->fApplyRate;
		}
#endif
	///////////////////////////////////////////////////////////////////////////////	
	// 2008-05-20 by cmkwon, 모든 이벤트(HappyHoure,MotherShip,Item) 그룹 동시에 가능하게 - 
	// 2008-05-20 by cmkwon, 2. MotherShipEvent - 자신의세력 이벤트 추가하기
	pGameEv = GetUsingGameEventPtr(i_eventType, i_byInflType, GAME_EVENT_GROUP_MOTHERSHIP);
	if(pGameEv
		&& util::in_range(pGameEv->MinLevel, i_nLevel, pGameEv->MaxLevel))
	{
		fRetPlusRate += pGameEv->fApplyRate;
	}

	///////////////////////////////////////////////////////////////////////////////	
	// 2008-05-20 by cmkwon, 모든 이벤트(HappyHoure,MotherShip,Item) 그룹 동시에 가능하게 - 
	// 2008-05-20 by cmkwon, 3. ItemEvent - 자신의세력 이벤트 추가하기
	pGameEv = GetUsingGameEventPtr(i_eventType, i_byInflType, GAME_EVENT_GROUP_ITEM);
	if(pGameEv
		&& util::in_range(pGameEv->MinLevel, i_nLevel, pGameEv->MaxLevel))
	{
		fRetPlusRate += pGameEv->fApplyRate;
	}

	// start 2011-10-28 by hskim, EP4 [트리거 시스템] - 크리스탈 시스템
	pGameEv = GetUsingGameEventPtr(i_eventType, i_byInflType, GAME_EVENT_GROUP_CRYSTAL_SYSTEM);
	if(pGameEv
		&& util::in_range(pGameEv->MinLevel, i_nLevel, pGameEv->MaxLevel))
	{
		if( pGameEv->MapIndex == 0 || pGameEv->MapIndex == i_MapIndex )		// 2012-06-16 by hskim, [트리거 시스템] - 크리스탈 시스템 2차 : 특정 맵에서만 해피아워 이벤트 적용
		{
		fRetPlusRate += pGameEv->fApplyRate;
		}
	}
	// end 2011-10-28 by hskim, EP4 [트리거 시스템] - 크리스탈 시스템

	// start 2012-05-08 by hskim, EP4 [트리거 시스템] - 크리스탈 시스템 2차
	pGameEv = GetUsingGameEventPtr(i_eventType, i_byInflType, GAME_EVENT_GROUP_CRYSTAL_SYSTEM_2ND);
	if(pGameEv
		&& util::in_range(pGameEv->MinLevel, i_nLevel, pGameEv->MaxLevel))
	{
		if( pGameEv->MapIndex == 0 || pGameEv->MapIndex == i_MapIndex )		// 2012-06-16 by hskim, [트리거 시스템] - 크리스탈 시스템 2차 : 특정 맵에서만 해피아워 이벤트 적용
		{
			fRetPlusRate += pGameEv->fApplyRate;
		}
	}
	// end 2012-05-08 by hskim, EP4 [트리거 시스템] - 크리스탈 시스템 2차

	///////////////////////////////////////////////////////////////////////////////
	// 2007-10-30 by cmkwon, PCBang 이벤트 Plus Rate 더하기
	if(i_bPCBangClient)
	{
		pGameEv = GetPCBangUsingGameEventPtr(i_eventType);
		if(pGameEv
			&& util::in_range(pGameEv->MinLevel, i_nLevel, pGameEv->MaxLevel))			// 2008-03-14 by dhjin, Level별 해피아워 이벤트 구현 -
		{
			fRetPlusRate += pGameEv->fApplyRate;
		}
	}

	return fRetPlusRate;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			float CGameEventManager::GetExperiencePlusRate(BYTE i_byInflType, BOOL i_bPCBangClient, INT i_nLevel)
/// \brief		// 2008-03-14 by dhjin, Level별 해피아워 이벤트 구현 -// 2007-10-30 by cmkwon, 세력별 해피아워 이벤트 구현 -
/// \author		cmkwon
/// \date		2007-10-30 ~ 2007-10-30
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
float CGameEventManager::GetExperiencePlusRate(BYTE i_byInflType, BOOL i_bPCBangClient, INT i_nLevel, MapIndex_t i_MapIndex /*=0*/)			// 2012-06-16 by hskim, [트리거 시스템] - 크리스탈 시스템 2차 : 특정 맵에서만 해피아워 이벤트 적용
{
	return CGameEventManager::GetTotalEventPlusRate(GAME_EVENT_TYPE_EXP, i_byInflType, i_bPCBangClient, i_nLevel, i_MapIndex);				// 2012-06-16 by hskim, [트리거 시스템] - 크리스탈 시스템 2차 : 특정 맵에서만 해피아워 이벤트 적용
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			float CGameEventManager::GetSPIPlusRate(BYTE i_byInflType, BOOL i_bPCBangClient, INT i_nLevel)
/// \brief		// 2008-03-14 by dhjin, Level별 해피아워 이벤트 구현 -// 2007-10-30 by cmkwon, 세력별 해피아워 이벤트 구현 -
/// \author		cmkwon
/// \date		2007-10-30 ~ 2007-10-30
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
float CGameEventManager::GetSPIPlusRate(BYTE i_byInflType, BOOL i_bPCBangClient, INT i_nLevel, MapIndex_t i_MapIndex /*=0*/)				// 2012-06-16 by hskim, [트리거 시스템] - 크리스탈 시스템 2차 : 특정 맵에서만 해피아워 이벤트 적용
{
	return CGameEventManager::GetTotalEventPlusRate(GAME_EVENT_TYPE_SPI, i_byInflType, i_bPCBangClient, i_nLevel, i_MapIndex);				// 2012-06-16 by hskim, [트리거 시스템] - 크리스탈 시스템 2차 : 특정 맵에서만 해피아워 이벤트 적용
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			float CGameEventManager::GetExpRepairPlusRate(BYTE i_byInflType, BOOL i_bPCBangClient, INT i_nLevel)
/// \brief		// 2008-03-14 by dhjin, Level별 해피아워 이벤트 구현 -// 2007-10-30 by cmkwon, 세력별 해피아워 이벤트 구현 -
/// \author		cmkwon
/// \date		2007-10-30 ~ 2007-10-30
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
float CGameEventManager::GetExpRepairPlusRate(BYTE i_byInflType, BOOL i_bPCBangClient, INT i_nLevel, MapIndex_t i_MapIndex /*=0*/)			// 2012-06-16 by hskim, [트리거 시스템] - 크리스탈 시스템 2차 : 특정 맵에서만 해피아워 이벤트 적용
{
	return CGameEventManager::GetTotalEventPlusRate(GAME_EVENT_TYPE_EXP_NO_DOWN, i_byInflType, i_bPCBangClient, i_nLevel, i_MapIndex);		// 2012-06-16 by hskim, [트리거 시스템] - 크리스탈 시스템 2차 : 특정 맵에서만 해피아워 이벤트 적용
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			float CGameEventManager::GetDropItemPlusRate(BYTE i_byInflType, BOOL i_bPCBangClient, INT i_nLevel)
/// \brief		// 2008-03-14 by dhjin, Level별 해피아워 이벤트 구현 -// 2007-10-30 by cmkwon, 세력별 해피아워 이벤트 구현 -
/// \author		cmkwon
/// \date		2007-10-30 ~ 2007-10-30
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
float CGameEventManager::GetDropItemPlusRate(BYTE i_byInflType, BOOL i_bPCBangClient, INT i_nLevel, MapIndex_t i_MapIndex /*=0*/)			// 2012-06-16 by hskim, [트리거 시스템] - 크리스탈 시스템 2차 : 특정 맵에서만 해피아워 이벤트 적용
{
	return CGameEventManager::GetTotalEventPlusRate(GAME_EVENT_TYPE_DROP_ITEM, i_byInflType, i_bPCBangClient, i_nLevel, i_MapIndex);		// 2012-06-16 by hskim, [트리거 시스템] - 크리스탈 시스템 2차 : 특정 맵에서만 해피아워 이벤트 적용
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			float CGameEventManager::GetDropRareItemPlusRate(BYTE i_byInflType, BOOL i_bPCBangClient, INT i_nLevel)
/// \brief		// 2008-03-14 by dhjin, Level별 해피아워 이벤트 구현 -// 2007-10-30 by cmkwon, 세력별 해피아워 이벤트 구현 -
/// \author		cmkwon
/// \date		2007-10-30 ~ 2007-10-30
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
float CGameEventManager::GetDropRareItemPlusRate(BYTE i_byInflType, BOOL i_bPCBangClient, INT i_nLevel, MapIndex_t i_MapIndex /*=0*/)		// 2012-06-16 by hskim, [트리거 시스템] - 크리스탈 시스템 2차 : 특정 맵에서만 해피아워 이벤트 적용
{
	return CGameEventManager::GetTotalEventPlusRate(GAME_EVENT_TYPE_DROP_REAR_ITEM, i_byInflType, i_bPCBangClient, i_nLevel, i_MapIndex);	// 2012-06-16 by hskim, [트리거 시스템] - 크리스탈 시스템 2차 : 특정 맵에서만 해피아워 이벤트 적용
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			float CGameEventManager::GetWarPointPlusRate(BYTE i_byInflType, BOOL i_bPCBangClient, INT i_nLevel)
/// \brief		// 2008-03-14 by dhjin, Level별 해피아워 이벤트 구현 -// 2007-10-30 by cmkwon, 세력별 해피아워 이벤트 구현 -
/// \author		cmkwon
/// \date		2007-10-30 ~ 2007-10-30
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
float CGameEventManager::GetWarPointPlusRate(BYTE i_byInflType, BOOL i_bPCBangClient, INT i_nLevel, MapIndex_t i_MapIndex /*=0*/)			// 2012-06-16 by hskim, [트리거 시스템] - 크리스탈 시스템 2차 : 특정 맵에서만 해피아워 이벤트 적용
{
	return CGameEventManager::GetTotalEventPlusRate(GAME_EVENT_TYPE_WARPOINT, i_byInflType, i_bPCBangClient, i_nLevel, i_MapIndex);			// 2012-06-16 by hskim, [트리거 시스템] - 크리스탈 시스템 2차 : 특정 맵에서만 해피아워 이벤트 적용
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CGameEventManager::EndGameEventAllByInfluenceType(BYTE i_byInflMask, BOOL i_bSendEndMsg/*=FALSE*/, EN_GAME_EVENT_GROUP i_enGameEvGroup/*=GAME_EVENT_GROUP_HAPPYHOUR*/)
/// \brief		// 2007-10-30 by cmkwon, 세력별 해피아워 이벤트 구현 - CGameEventManager::EndGameEventAllByInfluenceType() 함수 추가 됨
///				// 2008-05-20 by cmkwon, 모든 이벤트(HappyHoure,MotherShip,Item) 그룹 동시에 가능하게 - 인자 추가(, EN_GAME_EVENT_GROUP i_enGameEvGroup/*=GAME_EVENT_GROUP_HAPPYHOUR*/)
/// \author		cmkwon
/// \date		2007-10-31 ~ 2007-10-31
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CGameEventManager::EndGameEventAllByInfluenceType(BYTE i_byInflMask, BOOL i_bSendEndMsg/*=FALSE*/, EN_GAME_EVENT_GROUP i_enGameEvGroup/*=GAME_EVENT_GROUP_HAPPYHOUR*/)
{
	SGameEvent *pArrGameEvent = this->GetArrGameEventByInflType(i_byInflMask, i_enGameEvGroup);	// 2008-05-20 by cmkwon, 모든 이벤트(HappyHoure,MotherShip,Item) 그룹 동시에 가능하게 - 
	if(NULL == pArrGameEvent)
	{
		return FALSE;
	}

	int i=0;
	for(i=0; i < GAME_EVENT_TYPE_MAX_COUNT; i++)
	{
		if(pArrGameEvent[i].bUsingFlag)
		{// 2007-10-31 by cmkwon, 진행중이던 이벤트
			pArrGameEvent[i].bSendEventEndMessage	= i_bSendEndMsg;
		}
		else
		{
			pArrGameEvent[i].bSendEventEndMessage	= FALSE;
		}
		pArrGameEvent[i].bUsingFlag		= FALSE;
	}
	return TRUE;
}