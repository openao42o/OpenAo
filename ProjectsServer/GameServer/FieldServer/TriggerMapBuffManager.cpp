// TriggerMapBuff.cpp: implementation of the CTriggerMapBuffManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TriggerMapBuffManager.h"
#include "FieldIOCPSocket.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

vectorTriggerMapBuff *CTriggerMapBuffManager::s_pMapBuffList = NULL;

CTriggerMapBuffManager::CTriggerMapBuffManager(DWORD i_nTickInterval /* = 1000 */) 
	: CTickManager(i_nTickInterval)
{
	Clear();
}

CTriggerMapBuffManager::~CTriggerMapBuffManager()
{
	Destroy();
}

void CTriggerMapBuffManager::Clear()
{
	m_pFieldMapChannel = NULL;

	CleanThread();
}

BOOL CTriggerMapBuffManager::Create(CFieldMapChannel *pFieldMapChannel, vectorTriggerMapBuff *pMapBuffList)
{
	if( NULL == pFieldMapChannel )
	{
		return FALSE;
	}

	m_pFieldMapChannel = pFieldMapChannel;
	s_pMapBuffList = pMapBuffList;

	InitThread();

	return TRUE;
}

void CTriggerMapBuffManager::Destroy()
{
	Clear();
}

void CTriggerMapBuffManager::SetMapBuff(UID32_t iCharacterUniqueNumber, CFieldIOCPSocket *pFieldIOCPSocket)
{
	vectorTriggerMapBuff::iterator itr(s_pMapBuffList->begin());
	
	if( NULL != pFieldIOCPSocket && TRUE == pFieldIOCPSocket->IsUsing() )
	{
		pFieldIOCPSocket->m_nMapMoveCount++;
		
		for( ; itr != s_pMapBuffList->end(); itr++)
		{
			if( itr->MapIndex == m_pFieldMapChannel->m_pFieldMapProject->m_nMapIndex )
			{
				ITEM *pItemInfo = m_pFieldMapChannel->m_pFieldIOCPMapChannel->GetItemInfo(itr->ItemNum);

				if( NULL != pItemInfo )
				{
					int iRand = RANDI(0, (int)pItemInfo->GetParameterValue(DES_MAPBUFF_RANDOM_ADD_REATTACKTIME));
					INT nTimeInterval = pItemInfo->ReAttacktime + iRand;

					InsertTickEvent(SM_TICK_EVENT_TRIGGER_MAPBUFF, nTimeInterval, itr->ItemNum, pFieldIOCPSocket->m_nMapMoveCount, (void *)iCharacterUniqueNumber, (void *)pFieldIOCPSocket);
				}
			}
		}
	}
}

void CTriggerMapBuffManager::DoTickEvent(ATUM_DATE_TIME *pDateTime, TICK_EVENT *pTickEvent)
{
#ifdef _DEBUG
	DBGOUT(STRMSG_S_F2NOTIFY_0141, pDateTime->GetDateTimeString(STRNBUF(SIZE_MAX_ATUM_DATE_TIME_STRING)));
#endif

	switch( pTickEvent->TickEventType )
	{
		case SM_TICK_EVENT_TRIGGER_MAPBUFF:
		{
			UID32_t	CharacterUniqueNumber = (UID32_t)pTickEvent->vpTickEventParam1;
			CFieldIOCPSocket *pFieldIOCPSocket = (CFieldIOCPSocket *)pTickEvent->vpTickEventParam2;
			ItemNum_t ItemNum = (ItemNum_t)pTickEvent->nTickEventParam1;
			INT nMapMoveCount = pTickEvent->nTickEventParam2;

			mt_auto_lock mta(&m_pFieldMapChannel->m_mapCharacterUniqueNumberMapProject);

			mtmapUID2FieldIOCPSocket::iterator itrChar = m_pFieldMapChannel->m_mapCharacterUniqueNumberMapProject.find(CharacterUniqueNumber);

			if( itrChar != m_pFieldMapChannel->m_mapCharacterUniqueNumberMapProject.end() )
			{
				CFieldIOCPSocket *pChannelFieldSocket = itrChar->second;

				if( pFieldIOCPSocket == pChannelFieldSocket && TRUE == pChannelFieldSocket->IsUsing() && nMapMoveCount == pChannelFieldSocket->m_nMapMoveCount )
				{
					ITEM *pItemInfo = m_pFieldMapChannel->m_pFieldIOCPMapChannel->GetItemInfo(ItemNum);

					if( NULL != pItemInfo )
					{
						int iRand = RANDI(0, (int)pItemInfo->GetParameterValue(DES_MAPBUFF_RANDOM_ADD_TIME));		// 랜덤 시간 추가
				
						// 패킷 전송

						INIT_MSG_WITH_BUFFER(MSG_FC_TRIGGER_MAP_BUFF_SKILL, T_FC_TRIGGER_MAP_BUFF_SKILL, pSTriggerMapBuff, SendBuf);
						pSTriggerMapBuff->ClientIndex = pChannelFieldSocket->m_character.ClientIndex;
						pSTriggerMapBuff->SkillItemNum = ItemNum;
						pSTriggerMapBuff->AddTime = iRand;
						pChannelFieldSocket->SendAddData(SendBuf, MSG_SIZE(MSG_FC_TRIGGER_MAP_BUFF_SKILL));

						// 다음 적용 설정

						int iRandTime = RANDI(0, (int)pItemInfo->GetParameterValue(DES_MAPBUFF_RANDOM_ADD_REATTACKTIME));
						INT nTimeInterval = pItemInfo->ReAttacktime + iRandTime;

						InsertTickEvent(SM_TICK_EVENT_TRIGGER_MAPBUFF, nTimeInterval, ItemNum, pChannelFieldSocket->m_nMapMoveCount, (void *)CharacterUniqueNumber, (void *)pChannelFieldSocket);
					}
				}
			}
		}
		break;

	default:
		break;
	}

	return;
}

void CTriggerMapBuffManager::DoMonthlyWork(ATUM_DATE_TIME *pDateTime)
{
}

void CTriggerMapBuffManager::DoEveryTickWork(ATUM_DATE_TIME *pDateTime)
{
}

void CTriggerMapBuffManager::DoDailyWork(ATUM_DATE_TIME *pDateTime)
{
}

void CTriggerMapBuffManager::DoHourlyWork(ATUM_DATE_TIME *pDateTime)
{
}

void CTriggerMapBuffManager::DoMinutelyWork(ATUM_DATE_TIME *pDateTime)
{
}

void CTriggerMapBuffManager::DoSecondlyWork(ATUM_DATE_TIME *pDateTime)
{
}
