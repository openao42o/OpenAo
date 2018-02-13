// IMSocketManager.h: interface for the CIMSocketManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IMSOCKETMANAGER_H__C5A0D56A_5D4A_45C2_9AD8_4B13F65AD40D__INCLUDED_)
#define AFX_IMSOCKETMANAGER_H__C5A0D56A_5D4A_45C2_9AD8_4B13F65AD40D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CIMSocket;

class CIMSocketManager  
{
public:
	CIMSocketManager(HWND hwnd);
	virtual ~CIMSocketManager();	
	BOOL Connect(LPCSTR strPeerIP, int nPort);
	BOOL CloseSocket();
	BOOL Write(LPCSTR pPacket, int nLength, BOOL bArenaSend=FALSE);
	BOOL Write(BYTE *pPacket, int nLength) ;
	BOOL SendMsg( int nType, char *pPacket, int nSize );
	BOOL WriteMessageType(MessageType_t msgType);
	void SendChat(int nType, const char* strCharacter, const char* strChat);
	BOOL IsConnected();	
	LONG OnAsyncEvent(LONG lParam);
	int Read(LPSTR *pPacket, int &nLength, int nSoketType);
	void OnRecvdGetGuildOK(MSG_IC_CHAT_GET_GUILD_OK* pMsg, int nSoketType);
	void OnRecvdChangeGuild(MSG_IC_CHAT_CHANGE_GUILD* pMsg, int nSoketType);
	void OnRecvdChangeParty(MSG_IC_CHAT_CHANGE_PARTY* pMsg, int nSoketType);

	void SetChaterInfo(CHARACTER* pInfo);
	void SetArenaChaterInfo(CHARACTER* pInfo);	

	// 아레나 서버에 연결
	BOOL ConnectArena(LPCSTR strPeerIP, int nPort, BOOL* bAlReadyConnect);
	LONG OnArenaAsyncEvent(LONG lParam);
	BOOL CloseArenaIMSocket();

	CIMSocket* GetArenaIMSocket();
	CIMSocket* GetIMWinSocket(UINT nSocketNotifyType);

	BOOL IsIMArenaServerConnected();
	
private:
	BOOL IsArenaLoadSuccess();
	// 아레나 IM서버에만 보내는 커맨드인지 판단
	BOOL IsOnlySendArenaCmd(const char* pChat);
	// 2008-07-11 by bhsohn 과도한 채팅시, 채팅금지 시스템 추가
	BOOL IsPossibleSendChat();

private:
	CIMSocket*			m_pMainIMSocket;					// 채팅 서버
	CIMSocket*			m_pArenaIMSocket;					// 아레나 채팅 서버

	HWND				m_hwnd;
	
	// 2008-07-11 by bhsohn 과도한 채팅시, 채팅금지 시스템 추가
	ATUM_DATE_TIME		m_timeLastSendChat;
	ATUM_DATE_TIME		m_timeForbidSendChat;
	int					m_nMinChatCount;

};

#endif // !defined(AFX_IMSOCKETMANAGER_H__C5A0D56A_5D4A_45C2_9AD8_4B13F65AD40D__INCLUDED_)
