// ServicePauseManager.h: interface for the CServicePauseManager class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _ATUM_SERVICE_PAUSE_MANAGER_H_
#define _ATUM_SERVICE_PAUSE_MANAGER_H_

class CServicePauseManager  
{
public:
	CServicePauseManager();

	void SetQuestServiceState(BOOL i_bQuestService);	// 퀘스트 서비스 관리용
	BOOL GetQuestServiceState();						// 퀘스트 서비스 관리용

private:
	BOOL m_bQuestService; // 퀘스트 서비스 관리용
};

#endif // _ATUM_SERVICE_PAUSE_MANAGER_H_
