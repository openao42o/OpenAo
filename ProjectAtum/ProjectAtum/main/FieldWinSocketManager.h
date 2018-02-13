// FieldWinSocketManager.h: interface for the CFieldWinSocketManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FIELDWINSOCKETMANAGER_H__52C754A0_683B_4C24_B626_84476785ADFA__INCLUDED_)
#define AFX_FIELDWINSOCKETMANAGER_H__52C754A0_683B_4C24_B626_84476785ADFA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CFieldWinSocketManager  
{
public:
	CFieldWinSocketManager(HWND hwnd);
	virtual ~CFieldWinSocketManager();

	BOOL Connect(LPCSTR strPeerIP, int nPort);
	BOOL CloseSocket();
	BOOL Write(LPCSTR pPacket, int nLength);
	BOOL Write(BYTE *pPacket, int nLength) ;
	BOOL SendMsg( int nType, char *pPacket, int nSize );
	BOOL WriteMessageType(MessageType_t msgType);

	BOOL IsConnected();	
	LONG OnAsyncEvent(LONG lParam);
	LONG OnArenaAsyncEvent(LONG lParam);
	//int Read(LPSTR *pPacket, int &nLength);

	// 아레나 소켓 연결
	BOOL ConnectArena(LPCSTR strPeerIP, int nPort, BOOL* bAlReadyConnect);
	BOOL CloseSocketArena();

	// 아레나 필드소켓
	CFieldWinSocket* GetFieldWinSocket(UINT nSocketNotifyType);
	CFieldWinSocket* GetArenaFieldWinSocket();

	// 아레나 서버 연결 상태 
	BOOL IsFCConnectedArenaServer();

	BOOL IsRequestArenaSocketClose();

private:
	BOOL IsArenaLoadSuccess();

	
private:
	CFieldWinSocket *			m_pMainFieldWinSocket;				// 메인 서버 소켓
	CFieldWinSocket *			m_pArenaFieldWinSocket;				// 아레나 서버 소켓

	HWND						m_hMainWnd;

	BOOL						m_bRequestArenaSocketClose;			// 소켓 요청에 의한 클로우즈냐?

};

#endif // !defined(AFX_FIELDWINSOCKETMANAGER_H__52C754A0_683B_4C24_B626_84476785ADFA__INCLUDED_)
