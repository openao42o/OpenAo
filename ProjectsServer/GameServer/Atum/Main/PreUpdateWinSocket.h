// PreUpdateSocket.h: interface for the CPreUpdateSocket class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PREUPDATEWINSOCKET_H__16674AE6_F54A_491F_98FD_768A1AEF0757__INCLUDED_)
#define AFX_PREUPDATEWINSOCKET_H__16674AE6_F54A_491F_98FD_768A1AEF0757__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "winsocket.h"

class CPreUpdateWinSocket : public CWinSocket  
{
	friend class CAtumDlg;
public:
	CPreUpdateWinSocket(HWND hWnd, UINT wmSock = WM_ASYNC_EVENT, UINT wmNotify = WM_PACKET_NOTIFY);
	virtual ~CPreUpdateWinSocket();

	SOCKET GetSocketDescriptor()
	{
		return m_Socket;
	}



public:
	BOOL OnConnect(int nErrorCode);
	BOOL OnCloseSocket(int nErrorCode);
	BOOL OnRecvdPacket(LPSTR pPacket, int nLength, BYTE nSeq);
//	int CreateDirectory(CString strParent, CList<CString, CString&> &lsDir);
};

#endif // !defined(AFX_PREUPDATEWINSOCKET_H__16674AE6_F54A_491F_98FD_768A1AEF0757__INCLUDED_)
