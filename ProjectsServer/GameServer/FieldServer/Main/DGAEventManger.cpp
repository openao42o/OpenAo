// CDGAEventManager.cpp: implementation of the CDGAEventManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DGAEventManager.h"
#include "FieldIOCP.h"
#include "GameEventManager.h"
#include "FieldGlobal.h"				// 2007-10-30 by cmkwon, 세력별 해피아워 이벤트 구현 - 

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CDGAEventManager::CDGAEventManager(CFieldIOCP *i_pFIOCP)
{
	m_pFieldIOCP = i_pFIOCP;
	m_pRndDailyDGACount = 0;
	resetDGAEventManager();
}
CDGAEventManager::~CDGAEventManager()
{
}

BOOL CDGAEventManager::OnDoMinutelyWorkDGAEventManager(ATUM_DATE_TIME *pDateTime)
{
	//g_pFieldGlobal->WriteSystemLogEX(false,"[DEBUG] [DGAEventManager] Hour: %d,Minute: %d,Second: %d\r\n",pDateTime->Hour,pDateTime->Minute,pDateTime->Second);
	if (g_pFieldGlobal->m_dgaMode == 0)
	{
		return FALSE;
	}

	if (pDateTime->Hour == 0 && pDateTime->Minute == 0)
	{
		resetDGAEventManager();
	}

	if ((g_pFieldGlobal->m_dgaMode == 1 || g_pFieldGlobal->m_dgaMode == 3) && pDateTime->Hour == g_pFieldGlobal->m_dgaTime.Hour && pDateTime->Minute == g_pFieldGlobal->m_dgaTime.Minute)
	{
		vector<dgaItem*> items = getItems(g_pFieldGlobal->m_dgaItemMinCount, g_pFieldGlobal->m_dgaItemMaxCount, g_pFieldGlobal->m_dgaItems);
		m_pFieldIOCP->SendString128ToAllClients(STRING_128_SYSTEM_NOTICE, "\\mGood Evening Every One\\m. \\pThis Is Ares DGA!\\p \\cHope You Enjoy The Game\\c");
		for (int i = 0; i < items.size(); i++)
		{
			if (items.at(i)->itemNum == DGA_WP_ITEMNUM)
			{
				m_pFieldIOCP->SendWarPointToInflClientByDGA(items.at(i)->count);
				g_pFieldGlobal->WriteSystemLogEX(false, "[INFO] [DGAEventManager WPAllUser] Count(%d)\r\n", items.at(i)->count);
			}
			else if (m_pFieldIOCP->GetItemInfo(items.at(i)->itemNum))
			{
				g_pFieldGlobal->WriteSystemLogEX(false, "[INFO] [DGAEventManager ItemAllUser] ItemNum(%d), Count(%d)\r\n", items.at(i)->itemNum, items.at(i)->count);
				m_pFieldIOCP->InsertItemAllCharacter(items.at(i)->itemNum, items.at(i)->count, INFLUENCE_TYPE_ALL_MASK);
			}
			else
			{
				g_pFieldGlobal->WriteSystemLogEX(false, "[ERROR] [DGAEventManager ItemAllUser] Invalid Item: ItemNum(%d), Count(%d)\r\n", items.at(i)->itemNum, items.at(i)->count);
			}
		}
	}

	if (g_pFieldGlobal->m_dgaMode == 2 || g_pFieldGlobal->m_dgaMode == 3)
	{
		bool isTime = false;
		for (int i = 0; i < m_pRndDGATimes.size(); i++)
		{
			ATUM_DATE_TIME t = m_pRndDGATimes.at(i);

			if (t.Hour == pDateTime->Hour && t.Minute == pDateTime->Minute)
			{
				isTime = true;
				break;
			}

		}

		if (isTime)
		{
			vector<dgaItem*> items = getItems(g_pFieldGlobal->m_rndDgaItemMinCount, g_pFieldGlobal->m_rndDgaItemMaxCount, g_pFieldGlobal->m_rndDgaItems);
			for (int i = 0; i < items.size(); i++)
			{
				if (items.at(i)->itemNum == DGA_WP_ITEMNUM)
				{
					int map = getMap(g_pFieldGlobal->m_rndDgaMaps);
					m_pFieldIOCP->GetFieldMapProjectByMapIndex(map)->GetFirstFieldMapChannel(true)->AddWarPointInMap(items.at(i)->count, false, INFLUENCE_TYPE_ALL_MASK);
					g_pFieldGlobal->WriteSystemLogEX(false, "[INFO] [DGAEventManager WPInMap] Map(%d), Count(%d)\r\n", map, items.at(i)->count);
				}
				else if (m_pFieldIOCP->GetItemInfo(items.at(i)->itemNum))
				{

					int map = getMap(g_pFieldGlobal->m_rndDgaMaps);
					m_pFieldIOCP->GetFieldMapProjectByMapIndex(map)->GetFirstFieldMapChannel(true)->InsertItemInMap(items.at(i)->itemNum, items.at(i)->count, INFLUENCE_TYPE_ALL_MASK);
					g_pFieldGlobal->WriteSystemLogEX(false, "[INFO] [DGAEventManager ItemInMap] Map(%d),ItemNum(%d), Count(%d)\r\n", map, items.at(i)->itemNum, items.at(i)->count);
				}
			}
		}
	}
	return TRUE;
}

void CDGAEventManager::resetDGAEventManager(void)
{
	generateTimes(g_pFieldGlobal->m_rndDgaDailyMaxCount);
}

void CDGAEventManager::generateTimes(int count)
{
	m_pRndDGATimes.clear();
	for (int i = 0; i < count; i++)
	{
		ATUM_DATE_TIME time;
		time.SetCurrentDateTime();
		time.Hour = RANDI(0, 23);
		time.Minute = RANDI(0, 59);
		time.Second = 0;
		m_pRndDGATimes.push_back(time);

		g_pFieldGlobal->WriteSystemLogEX(false, "[INFO] [DGAEventManager RndTimes] Hour(%d), Minute(%d)\r\n", time.Hour, time.Minute);
	}
}

vector<dgaItem*> CDGAEventManager::getItems(int min, int max, vector<dgaItem*> itemPool)
{
	int count = RANDI(min, max); // todo : previous implementation used max - 1
	
	vector<dgaItem*> items;

	for (int i = 0; i < count; i++)
	{
		if (itemPool.empty()) break;

		int itemRnd = RANDI(0, itemPool.size() - 1);

		items.push_back(itemPool.at(itemRnd));
		itemPool.erase(itemPool.begin() + itemRnd);
	}
	
	return items;
}

int CDGAEventManager::getMap(vector<int> &maps)
{
	return maps.at(RANDI(0, maps.size() - 1));
}