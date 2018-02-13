// MGameMonitor.h: interface for the CMGameMonitor class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MGAMEMONITOR_H__D74555B7_FCCB_49ED_AC57_274AD383DC08__INCLUDED_)
#define AFX_MGAMEMONITOR_H__D74555B7_FCCB_49ED_AC57_274AD383DC08__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\..\..\COMMON\WinSocket.h"

#define	WM_ASYNC_EVENT_MGAME_MONITOR		(WM_USER + 300)		//
#define	WM_PACKET_NOTIFY_MGAME_MONITOR		(WM_USER + 301)		// HWND에 전달되는 메시지

#define PORT_MGAME_MONITOR_LISTENER			15198

class CMGameMonitor;
typedef vector<CMGameMonitor*>			vectMGameMonitorPtr;


class CMGameMonitor : public CWinSocket  
{
public:
	CMGameMonitor(HWND i_hWnd, UINT i_wmSockEvent, UINT i_wmNotify, int i_nListenPort);
	virtual ~CMGameMonitor();

	BOOL InitMGameMonitor(void);
	void ResetMGameMonitor(void);
	void CleanMGameMonitor(void);
	
	BOOL OnWS_Accept(WPARAM wParam, LPARAM lParam);

	CMGameMonitor *FindConnectedSocketBySocketHandle(SOCKET i_hSock);
	void ReleaseConnectedSocket(CMGameMonitor *i_pMGameSock);
		
protected:
	int					m_nMonitorListenPort;

	vectMGameMonitorPtr	m_vectConnectedSocketList;
};

#endif // !defined(AFX_MGAMEMONITOR_H__D74555B7_FCCB_49ED_AC57_274AD383DC08__INCLUDED_)
