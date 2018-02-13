// PreIMWinSocket.h: interface for the CPreIMWinSocket class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PREIMWINSOCKET_H__CE9F7F73_C6DF_4E82_A0FE_5DB8F40B2E69__INCLUDED_)
#define AFX_PREIMWINSOCKET_H__CE9F7F73_C6DF_4E82_A0FE_5DB8F40B2E69__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "WinSocket.h"

#define	WM_PREIM_ASYNC_EVENT			(WM_USER + 7)				// 
#define	WM_PREIM_PACKET_NOTIFY			(WM_USER + 8)				// HWND에 전달되는 메시지

class CIMIOCP;

class CPreIMWinSocket : public CWinSocket  
{
public:
	CPreIMWinSocket(HWND hWnd, UINT wmSock, UINT wmNotify);
	virtual ~CPreIMWinSocket();
	
	BOOL OnCloseSocket(int nErrorCode);
	BOOL OnConnect(int nErrorCode);

	void SetIMIOCP(CIMIOCP *pIMIOCP);
	
public:
	virtual BOOL OnRecvdPacket(LPSTR pPacket, int nLength, BYTE nSeq);

public:
	CIMIOCP		*m_pIMIOCP3;
};

#endif // !defined(AFX_PREIMWINSOCKET_H__CE9F7F73_C6DF_4E82_A0FE_5DB8F40B2E69__INCLUDED_)
