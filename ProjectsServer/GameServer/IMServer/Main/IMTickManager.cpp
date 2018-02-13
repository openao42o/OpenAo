#include "stdafx.h"
#include "IMTickManager.h"
#include "IMIOCP.h"
#include "AtumIMDBManager.h"

CIMTickManager::CIMTickManager(CIMIOCP *i_pIMIOCP, DWORD i_nTickInterval /* = IM_DEFAULT_TICK_INTERVAL */)
: CTickManager(i_nTickInterval)
{
	m_pIMIOCP4 = i_pIMIOCP;
}

CIMTickManager::~CIMTickManager()
{
}

void CIMTickManager::DoTickEvent(ATUM_DATE_TIME *pDateTime, TICK_EVENT *pTickEvent)
{
//#ifdef _DEBUG
//	DBGOUT("타이머 동작 %s\r\n", pDateTime->GetDateTimeString(STRNBUF(SIZE_MAX_ATUM_DATE_TIME_STRING)));
//#endif

	switch (pTickEvent->TickEventType)
	{
	case IM_TICK_EVENT_START_GUILD_WAR:
		{
			m_pIMIOCP4->GuildWarEnd(BATTLE_END_TIME_LIMITE, pTickEvent->nTickEventParam1, pTickEvent->nTickEventParam2);
		}
		break;
	default:
		break;
	}

	return;
}

void CIMTickManager::DoEveryTickWork(ATUM_DATE_TIME *pDateTime)
{
}

void CIMTickManager::DoDailyWork(ATUM_DATE_TIME *pDateTime)
{
}

void CIMTickManager::DoHourlyWork(ATUM_DATE_TIME *pDateTime)
{
	m_pIMIOCP4->m_chatBlockManager.CheckChatBlockList();
}

void CIMTickManager::DoMinutelyWork(ATUM_DATE_TIME *pDateTime)
{
	// 길드 해산
	m_pIMIOCP4->DismemberAllPendingGuild(pDateTime);

	///////////////////////////////////////////////////////////////////////////////
	// 2007-11-19 by cmkwon, 진정시스템 업데이트 - 
	m_pIMIOCP4->OnDoMinutelyWorkIMIOCP(pDateTime);

// 2006-05-08 by cmkwon, 필요 없음
// 	// 디버깅용 메시지 보내기
// 	INIT_MSG_WITH_BUFFER(MSG_IC_STRING_128, T_IC_STRING_128, pString, pStringBuf);
// 	pString->PrintType = STRING_128_ADMIN_CMD;
// 	sprintf(pString->String, "DoMinutelyWork: %s", pDateTime->GetDateTimeString(STRNBUF(SIZE_MAX_SQL_DATETIME_STRING)));
// 	m_pIMIOCP4->SendMessageToAllAdmin(pStringBuf, MSG_SIZE(MSG_IC_STRING_128));

	///////////////////////////////////////////////////////////////////////////////	
	// 2008-12-30 by cmkwon, 지도자 채팅 제한 카드 구현 - CIMTickManager::DoMinutelyWork(), 1분에 1번씩 체크, 해제시 접속중인 유저에 메시지 전송
	vectChatBlockInfoPtr vectReleasedList;
	m_pIMIOCP4->m_chatBlockManagerByLeader.CheckChatBlockList(&vectReleasedList);
	if(false == vectReleasedList.empty())
	{
		vectChatBlockInfoPtr::iterator itr(vectReleasedList.begin());
		for(; itr != vectReleasedList.end(); itr++)
		{
			SChatBlockInfo	*pChatBlockInfo	= *itr;
			CIMIOCPSocket	*pIISoc			= m_pIMIOCP4->GetIMIOCPSocketByCharacterName(pChatBlockInfo->szBlockedCharacterName);
			if(pIISoc && pIISoc->IsValidCharacter())
			{
				pIISoc->SendString128(STRING_128_USER_NOTICE, STRMSG_081230_0003);
			}
		}
		for (auto x : vectReleasedList) delete x;	// 2008-12-30 by cmkwon, 메모리제거
	}
}

void CIMTickManager::DoSecondlyWork(ATUM_DATE_TIME *pDateTime)
{
// 2006-05-08 by cmkwon, 필요 없음
// 	// 디버깅용 메시지 보내기
// 	INIT_MSG_WITH_BUFFER(MSG_IC_STRING_128, T_IC_STRING_128, pString, pStringBuf);
// 	pString->PrintType = STRING_128_ADMIN_CMD;
// 	sprintf(pString->String, "DoSecondlyWork: %s", pDateTime->GetDateTimeString(STRNBUF(SIZE_MAX_SQL_DATETIME_STRING)));
// 	m_pIMIOCP4->SendMessageToAllAdmin(pStringBuf, MSG_SIZE(MSG_IC_STRING_128));

	// 2009-01-14 by cmkwon, 운영자 자동 공지 시스템 구현 - CIMTickManager::DoSecondlyWork()에서 CIMIOCP::OnDoSecondlyWorkIMIOCP() 호출
	m_pIMIOCP4->OnDoSecondlyWorkIMIOCP(pDateTime);
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			void CIMTickManager::DoMonthlyWork(ATUM_DATE_TIME *pDateTime)
/// \brief		// 2005-12-27 by cmkwon, 한달에 한 번 씩 해야 할 일
/// \author		cmkwon
/// \date		2005-12-27 ~ 2005-12-27
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CIMTickManager::DoMonthlyWork(ATUM_DATE_TIME *pDateTime)
{
}