// FieldWinSocket.h: interface for the CFieldWinSocket class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FIELDWINSOCKET_H__CB2738BC_78A1_4334_A35F_5AB344D9BE08__INCLUDED_)
#define AFX_FIELDWINSOCKET_H__CB2738BC_78A1_4334_A35F_5AB344D9BE08__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "WinSocket.h"

// 2007-11-22 by bhsohn 아레나 통합서버
// 아레나 이벤트 
#define	WM_ASYNC_ARENA_EVENT		(WM_USER_BASE + 3)				
#define	WM_PACKET_ARENA_NOTIFY		(WM_USER_BASE + 4)				// HWND에 전달되는 메시지


class CFieldWinSocket : public CWinSocket  
{
public:
	CFieldWinSocket(HWND hWnd, UINT wmSock = WM_ASYNC_EVENT, UINT wmNotify = WM_PACKET_NOTIFY);
	virtual ~CFieldWinSocket();

	virtual BOOL OnRecvdPacket(LPSTR pPacket, int nLength, BYTE nSeq);		// 하나의 패킷이 도착했을 때 발생
	BOOL SendMsg( MessageType_t nType, char *pPacket, int nSize );					// 프로토콜 전송
public:
	int		m_nOldType;
};

#endif // !defined(AFX_FIELDWINSOCKET_H__CB2738BC_78A1_4334_A35F_5AB344D9BE08__INCLUDED_)
