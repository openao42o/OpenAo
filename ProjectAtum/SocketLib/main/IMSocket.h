// IMSocket.h: interface for the CIMSocket class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IMSOCKET_H__49244E40_E618_4B1C_8358_EBAD9361542D__INCLUDED_)
#define AFX_IMSOCKET_H__49244E40_E618_4B1C_8358_EBAD9361542D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Winsocket.h"

// 2007-11-22 by bhsohn 아레나 통합서버
// Arena Chat
#define WM_IM_ARENA_ASYNC		(WM_USER_BASE + 772)
#define WM_IM_ARENA_NOTIFY		(WM_USER_BASE + 773)
// end 2007-11-22 by bhsohn 아레나 통합서버

class CIMSocket : public CWinSocket  
{
public:
	CIMSocket(HWND hWnd, UINT wmSock = WM_IM_ASYNC, UINT wmNotify = WM_IM_NOTIFY);
	virtual ~CIMSocket();

	BOOL SendMsg( int nType, char *pPacket, int nSize );					// 프로토콜 전송

	// 2007-11-22 by bhsohn 아레나 통합서버
	void SetChaterInfo(CHARACTER* pInfo);

protected:
	int m_nLoginType;
	BOOL m_bLogin;
	int		m_nOldType;
public:
	CHARACTER m_character;

public:
	void SetLoginType(int nLoginType) { m_nLoginType = nLoginType; }
	int GetLoginType() { return m_nLoginType; }
	void SetLogin(BOOL bLogin) { m_bLogin = bLogin; }
	BOOL GetLogin() { return m_bLogin; }

public:
	void SendAuthLogin(char* strAccountName, char* strCharacter, char* strPassword, char* strServerName, BYTE nLoginType);
	void SendChat(int nType, BYTE nameColor, const char* strCharacter, const char* strChat);
	void SendGetGuild(UINT nCharacterUniqueNumber){};// 자신의 unique number
	void SendGetParty(UINT nIndex){};//파티장 번호
	void SendChangeParty(UINT nIndex, char* strName, BYTE nJoin){};//strName은 NULL로 해도 될듯
	void SendChangeGuild(UINT nCharacterUniqueNumber, char* strCharacterName,char* strGuildName, BYTE bJoined){};
	void SendChatOnOff( int nType, BYTE bOnOff);

public:
	virtual BOOL OnRecvdPacket(LPSTR pPacket, int nLength, BYTE nSeq);

//	void OnRecvdIMLoginOK(MSG_IC_CONNECT_LOGIN_OK* pMsg);
	void OnRecvdGetGuildOK(MSG_IC_CHAT_GET_GUILD_OK* pMsg);
	void OnRecvdChangeGuild(MSG_IC_CHAT_CHANGE_GUILD* pMsg);
//	void OnRecvdGetPartyOK(MSG_IC_CHAT_GET_PARTY_OK* pMsg);
	void OnRecvdChangeParty(MSG_IC_CHAT_CHANGE_PARTY* pMsg);
};

#endif // !defined(AFX_IMSOCKET_H__49244E40_E618_4B1C_8358_EBAD9361542D__INCLUDED_)
