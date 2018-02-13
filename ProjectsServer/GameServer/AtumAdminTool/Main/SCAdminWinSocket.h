// SCAdminWinSocket.h: interface for the CSCAdminWinSocket class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SCUSERADMINWINSOCKET_H__245CC7E0_9331_43D6_B094_5FD4246A8668__INCLUDED_)
#define AFX_SCUSERADMINWINSOCKET_H__245CC7E0_9331_43D6_B094_5FD4246A8668__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "winsocket.h"
#include "AtumParam.h"

#define	WM_PRE_ASYNC_EVENT				(WM_USER_BASE + 3)
#define	WM_PRE_PACKET_NOTIFY			(WM_USER_BASE + 4)

#define	WM_IM_ASYNC_EVENT				(WM_USER_BASE + 5)
#define	WM_IM_PACKET_NOTIFY				(WM_USER_BASE + 6)

#define	WM_FIELD_ASYNC_EVENT			(WM_USER_BASE + 7)
#define	WM_FIELD_PACKET_NOTIFY			(WM_USER_BASE + 8)

class CSCAdminWinSocket : public CWinSocket
{
public:
	CSCAdminWinSocket(const char* i_szSocketName, CDialog *i_pMainDlg, HWND hWnd, UINT wmSock = WM_ASYNC_EVENT, UINT wmNotify = WM_PACKET_NOTIFY);
	virtual ~CSCAdminWinSocket();

	BOOL OnConnect(int nErrorCode);
	BOOL OnCloseSocket(int nErrorCode);
	BOOL OnRecvdPacket(LPSTR pPacket, int nLength, BYTE nSeq);

	BOOL OnWS_Closed(void);
public:
	CDialog		*m_pMainDlg;
	const char	*m_szSocketName;
};

class CSCAdminPreWinSocket : public CSCAdminWinSocket
{
public:
	CSCAdminPreWinSocket(const char* i_szSocketName, CDialog *i_pMainDlg, HWND hWnd, UINT wmSock = WM_PRE_ASYNC_EVENT, UINT wmNotify = WM_PRE_PACKET_NOTIFY);
};

class CSCAdminIMWinSocket : public CSCAdminWinSocket
{
public:
	CSCAdminIMWinSocket(const char* i_szSocketName, char *i_szServerGroupName, CDialog *i_pMainDlg, HWND hWnd, UINT wmSock = WM_IM_ASYNC_EVENT, UINT wmNotify = WM_IM_PACKET_NOTIFY);

	INT		m_UserCount;
	char	m_szServerGroupName1[SIZE_MAX_SERVER_NAME];
};

class CSCAdminFieldWinSocket : public CSCAdminWinSocket
{
public:
	CSCAdminFieldWinSocket(const char* i_szSocketName, char *i_szServerGroupName, CDialog *i_pMainDlg, HWND hWnd, UINT wmSock = WM_FIELD_ASYNC_EVENT, UINT wmNotify = WM_FIELD_PACKET_NOTIFY);

public:
	SERVER_ID	m_ServerID;
	char		m_szServerGroupName1[SIZE_MAX_SERVER_NAME];
};

#endif // !defined(AFX_SCUSERADMINWINSOCKET_H__245CC7E0_9331_43D6_B094_5FD4246A8668__INCLUDED_)
