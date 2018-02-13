// LogWinSocket.h: interface for the CLogWinSocket class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGWINSOCKET_H__CE9F7F73_C6DF_4E82_A0FE_5DB8F40B2E69__INCLUDED_)
#define AFX_LOGWINSOCKET_H__CE9F7F73_C6DF_4E82_A0FE_5DB8F40B2E69__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "WinSocket.h"

#define	WM_LOG_ASYNC_EVENT				(WM_USER + 1)				// 
#define	WM_LOG_PACKET_NOTIFY			(WM_USER + 2)				// HWND에 전달되는 메시지

class CLogWinSocket : public CWinSocket  
{
public:
	CLogWinSocket(HWND hWnd, UINT wmSock, UINT wmNotify);
	virtual ~CLogWinSocket();

public:
	BOOL OnRecvdPacket(LPSTR pPacket, int nLength, BYTE nSeq);
};

#endif // !defined(AFX_LOGWINSOCKET_H__CE9F7F73_C6DF_4E82_A0FE_5DB8F40B2E69__INCLUDED_)
