// MGameMonitor.cpp: implementation of the CMGameMonitor class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "atumadmintool.h"
#include "MGameMonitor.h"
#include "AtumDateTime.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMGameMonitor::CMGameMonitor(HWND i_hWnd, UINT i_wmSockEvent, UINT i_wmNotify, int i_nListenPort)
: CWinSocket(ST_ALL, i_hWnd, i_wmSockEvent, i_wmNotify)
{
	m_nMonitorListenPort	= i_nListenPort;
}

CMGameMonitor::~CMGameMonitor()
{
	this->CleanMGameMonitor();
}

BOOL CMGameMonitor::InitMGameMonitor()
{
	if(INVALID_SOCKET != this->GetSocketHandle())
	{
		return FALSE;
	}

	if(FALSE == this->Bind(m_nMonitorListenPort))
	{
		return FALSE;
	}

	if(FALSE == this->Listen())
	{
		return FALSE;
	}

	//DbgOut("[Noti] InitMGameMonitor\r\n");	
	return TRUE;
}
void CMGameMonitor::ResetMGameMonitor(void)
{
	for(int i=0; i < m_vectConnectedSocketList.size(); i++)
	{
		if(m_vectConnectedSocketList[i]->IsConnected())
		{
			m_vectConnectedSocketList[i]->CloseSocket();
		}
	}
	this->CloseSocket();

	DBGOUT("0310 - void CMGameMonitor::ResetMGameMonitor(void)\r\n");
}
void CMGameMonitor::CleanMGameMonitor(void)
{
	this->ResetMGameMonitor();

	for (auto x : m_vectConnectedSocketList) delete x;
	m_vectConnectedSocketList.clear();
}


BOOL CMGameMonitor::OnWS_Accept(WPARAM wParam, LPARAM lParam)
{
	SOCKADDR_IN tmSockAddr;
	memset(&tmSockAddr, 0x00, sizeof(SOCKADDR_IN));
	int	nLen = sizeof(tmSockAddr);
	SOCKET tmSock = accept(this->GetSocketHandle(), (LPSOCKADDR)&tmSockAddr, (int *)&nLen);
    if (INVALID_SOCKET == tmSock)
	{
		return FALSE;
	}


	CMGameMonitor *pNew = new CMGameMonitor(m_hMsgWnd, WM_ASYNC_EVENT_MGAME_MONITOR, WM_PACKET_NOTIFY_MGAME_MONITOR, PORT_MGAME_MONITOR_LISTENER);
	pNew->CloseSocket();

	pNew->m_Socket = tmSock;
	pNew->SetAsyncFlag(FD_READ | FD_WRITE | FD_OOB | FD_CLOSE);
	pNew->AsyncSelect();

	m_vectConnectedSocketList.push_back(pNew);

	ATUM_DATE_TIME	tmATime { true };
	char			tmStrBuf[SIZE_MAX_ATUM_DATE_TIME_STRING];
	//DbgOut("[Noti] Accepted, %s\r\n", tmATime.GetDateTimeString(tmStrBuf, SIZE_MAX_ATUM_DATE_TIME_STRING));
	return TRUE;
}

CMGameMonitor *CMGameMonitor::FindConnectedSocketBySocketHandle(SOCKET i_hSock)
{
	if(this->GetSocketHandle() == i_hSock)
	{
		return this;
	}

	for(int i=0; i < m_vectConnectedSocketList.size(); i++)
	{		
		if(i_hSock == m_vectConnectedSocketList[i]->GetSocketHandle())
		{
			return m_vectConnectedSocketList[i];
		}
	}

	return NULL;
}

void CMGameMonitor::ReleaseConnectedSocket(CMGameMonitor *i_pMGameSock)
{
	for(int i=0; i < m_vectConnectedSocketList.size(); i++)
	{
		CMGameMonitor *ptmMG = m_vectConnectedSocketList[i];
		if(i_pMGameSock == ptmMG)
		{
			ptmMG->CloseSocket();
			delete ptmMG;

			m_vectConnectedSocketList.erase(m_vectConnectedSocketList.begin() + i);
			return;
		}
	}
}