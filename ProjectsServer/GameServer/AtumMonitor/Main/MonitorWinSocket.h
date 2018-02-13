#ifndef	__MONITORWINSOCKET_H__
#define	__MONITORWINSOCKET_H__

#include "stdafx.h"
#include "WinSocket.h"
#include "LeftView.h"

#define TIMEOUT 5000

class CMonitorWinSocket : public CWinSocket
{
public:
	CMonitorWinSocket(HWND hWnd, CLeftView *cWnd, UINT wmSock = WM_ASYNC_EVENT, UINT wmNotify = WM_PACKET_NOTIFY);
	virtual ~CMonitorWinSocket();

public:
	BOOL OnConnect(int nErrorCode);
	BOOL OnCloseSocket(int nErrorCode);
	virtual BOOL OnRecvdPacket(LPSTR pPacket, int nLength, BYTE nSeq);
	void SetTreeItem(HTREEITEM hItem){	m_hTreeItem = hItem;}
	HTREEITEM GetTreeItem(void){		return m_hTreeItem;}

	SOCKET GetSocket() {
		return m_Socket;
	}

	void SetTimeOut(UINT nElapse)
	{
		if (!m_bTimeOutFlag)
		{
			m_LeftView->SetTimer(m_Socket, nElapse, NULL);
			m_bTimeOutFlag = TRUE;
		}
	}

	void KillTimeOut()
	{
		if (m_bTimeOutFlag)
		{
			m_LeftView->KillTimer(m_Socket);
			m_bTimeOutFlag = FALSE;
		}
	}

	BOOL GetTimeOutFlag()
	{
		return m_bTimeOutFlag;
	}

public:
	HTREEITEM	m_hTreeItem;
	BOOL		m_bTimeOutFlag;
	CLeftView	*m_LeftView;

	// long packet
	BOOL		m_bReceivingLongPacket;
	int			m_LongPacketOffset;
};

#endif
