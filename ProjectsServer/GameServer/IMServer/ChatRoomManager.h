// ChatRoomManager.h: interface for the CChatRoomManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHATROOMMANAGER_H__F642DFB7_6CB5_4B50_AA36_789F705005C4__INCLUDED_)
#define AFX_CHATROOMMANAGER_H__F642DFB7_6CB5_4B50_AA36_789F705005C4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CIMIOCP;
class CIMIOCPSocket;
class CChatRoom;

typedef mt_list<CChatRoom*>					mtListChatRoom;

class CChatRoomManager  
{
public:
	CChatRoomManager();
	virtual ~CChatRoomManager();

	void InitChatRoomManager(CIMIOCP *i_pIIOCP);
	INT	 FindEmptyChatRoomNum();
	Err_t CreateChatRoom(CIMIOCPSocket * i_pIISock, MSG_IC_CHATROOM_CREATE * i_pChatRoomCreateInfo);
	void SetReserveChatNum(INT i_nReserveChatNum);

	mtListChatRoom		m_mtlistChatRoom;

private:
	CIMIOCP				*ms_pIMIOCP4;
	INT					m_nReserveChatNum;
	
};

#endif // !defined(AFX_CHATROOMMANAGER_H__F642DFB7_6CB5_4B50_AA36_789F705005C4__INCLUDED_)
