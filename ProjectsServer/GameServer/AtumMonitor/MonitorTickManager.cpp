// MonitorTickManager.cpp: implementation of the CMonitorTickManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AtumMonitor.h"
#include "MonitorTickManager.h"
#include "MonitorNode.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMonitorTickManager::CMonitorTickManager()
: CTickManager(1000)
{
	m_hFile = NULL;
}

CMonitorTickManager::~CMonitorTickManager()
{
	CleanTickManager();
}

BOOL CMonitorTickManager::InitTickManager(CMonitorServerNode *i_pMonitorServerNode)
{
	m_pMonitorServerNode = i_pMonitorServerNode;
//	m_hFile = CreateFile("user_stat.txt", GENERIC_WRITE, FILE_SHARE_READ, NULL,
//							CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
//
//	if ( m_hFile == INVALID_HANDLE_VALUE)
//	{
//		return FALSE;
//	}
//
//
//	// 파일 닫기
//	CloseHandle(m_hFile);

	return CTickManager::InitThread();
}

void CMonitorTickManager::CleanTickManager()
{
	CTickManager::CleanThread();
}

void CMonitorTickManager::DoEveryTickWork(ATUM_DATE_TIME *pDateTime)
{
}

void CMonitorTickManager::DoDailyWork(ATUM_DATE_TIME *pDateTime)
{
}

void CMonitorTickManager::DoHourlyWork(ATUM_DATE_TIME *pDateTime)
{
}

void CMonitorTickManager::DoMinutelyWork(ATUM_DATE_TIME *pDateTime)
{
	if (m_pMonitorServerNode->m_NodeType == NODE_TYPE_FIELD_SERVER)
	{
		CFieldServerNode *pFieldServerNode = (CFieldServerNode*)m_pMonitorServerNode;
		list<CChannelInfoNode*>::iterator itr = pFieldServerNode->m_listChannelInfoNodePtr.begin();
		while (itr != pFieldServerNode->m_listChannelInfoNodePtr.end())
		{
			CChannelInfoNode *pChannelInfoNode = *itr;

			INIT_MSG_WITH_BUFFER(MSG_FM_GET_MAP_USER_COUNTS, T_FM_GET_MAP_USER_COUNTS, pSendGetMapUser, buff);
			pSendGetMapUser->MapChannIndex.MapIndex		= pChannelInfoNode->m_projectInfo.m_nMapIndex;
			pSendGetMapUser->MapChannIndex.ChannelIndex	= pChannelInfoNode->m_ChannelIndex;
			pChannelInfoNode->m_pMonitorWinSocket->Write(buff, MSG_SIZE(MSG_FM_GET_MAP_USER_COUNTS));

			itr++;
		}

		pFieldServerNode->m_pMonitorWinSocket->WriteMessageType(T_FM_GET_NUM_CLIENTS);
	}
//	DBGOUT("DoMinutelyWork %s\r\n", pDateTime->GetDateTimeString(STRNBUF(SIZE_MAX_ATUM_DATE_TIME_STRING)));
//	// 파일 쓰기
//	char lineBuffer[512];
//	sprintf(lineBuffer, "%s %s", pDateTime->GetDateTimeString(STRNBUF(SIZE_MAX_ATUM_DATE_TIME_STRING)), "22");
//
//	DWORD nWritten = 0;
//	WriteFile(hFile, lineBuffer, strlen(lineBuffer), &nWritten, NULL);
}

void CMonitorTickManager::DoSecondlyWork(ATUM_DATE_TIME *pDateTime)
{
}
