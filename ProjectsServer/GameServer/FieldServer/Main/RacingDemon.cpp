// RacingDemon.cpp: implementation of the CRacingDemon class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RacingDemon.h"
#include "FieldMapChannel.h"


DWORD WINAPI WorkerRacingDemon(LPVOID lpParam)
{
	return reinterpret_cast<CRacingDemon*>(lpParam)->Worker();
}



bool CRacingDemon::InitRacingDemon()
{
	if (m_threadWorker.joinable()) return false;

	m_bWorkerEndFlag = false;

	try
	{
		m_threadWorker = thread { WorkerRacingDemon, LPVOID(this) };

		return true;
	}
	catch (const system_error& error)
	{
		return false;
	}
}

void CRacingDemon::CleanRacingDemon()
{
	if (m_threadWorker.joinable())
	{
		m_bWorkerEndFlag = true;

		m_threadWorker.join();
	}
}


DWORD CRacingDemon::Worker()
{
	time_t ltime;

	while (!m_bWorkerEndFlag)
	{
		m_mtvectorRacingMapChannelList.lock();

		time(&ltime);

		auto today = localtime(&ltime);

		for (auto map : m_mtvectorRacingMapChannelList)
			
			map->ProcessRacing(today);

		m_mtvectorRacingMapChannelList.unlock();

		this_thread::sleep_for(100ms);
	}

	return 0;
}


bool CRacingDemon::PutRacingMapChannel(CFieldMapChannel *i_racingMapChannel)
{
	m_mtvectorRacingMapChannelList.lock();

	for (auto map : m_mtvectorRacingMapChannelList)
	{
		if (map == i_racingMapChannel)
		{
			m_mtvectorRacingMapChannelList.unlock();
			
			return true;
		}
	}

	m_mtvectorRacingMapChannelList.push_back(i_racingMapChannel);
	m_mtvectorRacingMapChannelList.unlock();
	
	return true;
}

void CRacingDemon::PopRacingMapChannel(CFieldMapChannel *i_racingMapChannel)
{
	m_mtvectorRacingMapChannelList.lock();

	for (auto itr = m_mtvectorRacingMapChannelList.begin(); itr != m_mtvectorRacingMapChannelList.end(); ++itr)
	{
		if (*itr == i_racingMapChannel)
		{
			m_mtvectorRacingMapChannelList.erase(itr);
			
			break;
		}
	}

	m_mtvectorRacingMapChannelList.unlock();
}


CFieldMapChannel *CRacingDemon::FindRacingMapChannel(MAP_CHANNEL_INDEX i_mapChanIdx)
{
	CFieldMapChannel* pRet = nullptr;

	m_mtvectorRacingMapChannelList.lock();

	for (auto map : m_mtvectorRacingMapChannelList)
	{
		if (i_mapChanIdx == map->GetMapChannelIndex())
		{
			pRet = map;

			break;
		}
	}

	m_mtvectorRacingMapChannelList.unlock();

	return pRet;
}



bool CRacingDemon::MakeMessageRacingDemon(MessageType_t i_msgType, void *o_pMsg)
{
	switch(i_msgType)
	{

	case T_FC_RACING_RACINGLIST_REQUEST_ACK:

		{
			MSG_FC_RACING_RACINGLIST_REQUEST_ACK *pMsg = (MSG_FC_RACING_RACINGLIST_REQUEST_ACK*)o_pMsg;
			MAP_CHANNEL_INDEX *pMapChanIdx = (MAP_CHANNEL_INDEX*)((BYTE*)pMsg + sizeof(MSG_FC_RACING_RACINGLIST_REQUEST_ACK));
			pMsg->byRacingListCounts = m_mtvectorRacingMapChannelList.size();
			int nIdx = 0;
			while (nIdx < m_mtvectorRacingMapChannelList.size())
			{
				pMapChanIdx[nIdx] = m_mtvectorRacingMapChannelList[nIdx]->GetMapChannelIndex();
				nIdx++;
			}
		}

		return true;
	
	default: return false;

	}
}