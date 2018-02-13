// FieldWebIOCP.cpp: implementation of the CFieldWebIOCP class.
// 2013-03-13 by hskim, 웹 캐시 상점
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FieldWebIOCP.h"
#include "FieldWebIOCPSocket.h"
#include "Wininet.h"
#include "config.h"
#include "FieldGlobal.h"
#include "AtumError.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFieldWebIOCP::CFieldWebIOCP(int nPort, char *szLocalIP)
:CIOCP(0, SIZE_MAX_FIELDWEBSERVER_SESSION, nPort, szLocalIP, ST_FIELD_WEB_SERVER)
{
	CFieldWebIOCPSocket::ms_pFieldWebIOCP		= this;
	
	CFieldWebIOCPSocket * pFieldWebIOCPSocket = new CFieldWebIOCPSocket[m_dwArrayClientSize];
	m_pArrayIOCPSocket = pFieldWebIOCPSocket;
	for(int i = 0; i < m_dwArrayClientSize; i++)
	{
		m_ArrayClient[i] = &pFieldWebIOCPSocket[i];
		m_ArrayClient[i]->InitIOCPSocket(i);
	}

	m_pFieldWebTickManager		= NULL;
	m_pFieldIOCP				= NULL;
}

CFieldWebIOCP::~CFieldWebIOCP()
{
	ListenerClose();
	IOCPClean();
	memset(m_ArrayClient, 0x00, sizeof(CIOCPSocket*) * COUNT_MAX_SOCKET_SESSION);
	if (NULL != m_pArrayIOCPSocket)
	{
		delete[] (CFieldWebIOCPSocket*)m_pArrayIOCPSocket;
		m_pArrayIOCPSocket = NULL;
	}

	util::del(m_pFieldWebTickManager);
}

BOOL CFieldWebIOCP::IOCPInit()
{
	g_pFieldGlobal->WriteSystemLogEX(TRUE, "  [Notify] CFieldWebIOCP::IOCPInit# 10000 \r\n");

	m_pFieldWebTickManager = new CFieldWebTickManager(this);
	if (m_pFieldWebTickManager->InitTickManager() == FALSE)
	{
		MessageBox(NULL, "m_pFieldWebTickManager initialize fail", "ERROR - ACEOnline WebCash Module", NULL);
		return FALSE;
	}

	g_pFieldGlobal->WriteSystemLogEX(TRUE, "  [Notify] CFieldWebIOCP::IOCPInit# 16000 CFieldWebTickManager::InitTickManager# !\r\n");

	g_pFieldGlobal->WriteSystemLogEX(TRUE, "  [Notify] CFieldWebIOCP::IOCPInit# 20000 end !\r\n");

	CIOCP::IOCPInit();

	return TRUE;
}

void CFieldWebIOCP::SetFieldIOCP(CFieldIOCP *pFieldIOCP)
{
	m_pFieldIOCP = pFieldIOCP;	
}

void CFieldWebIOCP::IOCPClean(void)
{
	CIOCP::IOCPClean();

	if (m_pFieldWebTickManager != NULL)
	{
		m_pFieldWebTickManager->CleanTickManager();
		util::del(m_pFieldWebTickManager);
	}
}

SThreadInfo *CFieldWebIOCP::CheckIOCPThread(DWORD i_dwThreadIdToExclude)
{
	SThreadInfo* pTInfo = CIOCP::CheckIOCPThread(i_dwThreadIdToExclude);

	if(pTInfo
		&& !m_MonitorIOCPSocketPtrVector.empty())
	{
	}

	return pTInfo;
}

void CFieldWebIOCP::OnDoMinutelyWorkIOCP(ATUM_DATE_TIME *pDateTime)
{	
}