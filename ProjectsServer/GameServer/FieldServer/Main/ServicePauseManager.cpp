#include "stdafx.h"
#include "ServicePauseManager.h"

CServicePauseManager::CServicePauseManager()
{
	m_bQuestService = TRUE;
}

void CServicePauseManager::SetQuestServiceState(BOOL i_bQuestService)
{
	m_bQuestService = i_bQuestService;
}

BOOL CServicePauseManager::GetQuestServiceState()
{
	return m_bQuestService;
}
